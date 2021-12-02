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
 * $RCSfile: svctx.c,v $
 * $Revision: #3 $
 * $Date: 2012/07/03 $
 * $Author: hongjun.chu $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/26 $
 * $SWAuthor: Weider Chang $
 * $MD5HEX: 9cacb426bc6af60b7f3e5a1ad05af25e $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "inc/common.h"
#include "handle/handle.h"
#include "conn_mngr/x_cm.h"
#include "conn_mngr/cm.h"
#include "conn_mngr/brdcst/x_brdcst.h"
#include "strm_mngr/x_sm.h"
#include "strm_mngr/x_sm_event.h"
#include "strm_mngr/video_hdlr/u_sm_video_hdlr.h"
#include "strm_mngr/sess_gnrc_hdlr/u_sm_sess_gnrc.h"
#include "prc_mngr/x_ca_mmi.h"
#include "scdb/x_scdb_api.h"
#include "svctx/svctx.h"
#include "svctx/svctx_sm_broker.h"
#include "svctx/svctx_dbg.h"
#include "svctx/svctx_cli.h"
#include "svl/x_svl.h"
#include "dbg/x_dbg.h"
#include "dbg/def_dbg_level_mw.h"
#include "svctx/activator.h"

#include "svctx/ch_hdlr/ch_svc_req.h"
#include "svctx/ch_hdlr/x_ch_hdlr.h"
#include "svctx/util/player.h"

#include "svctx/svctx_rikstv.h"

#ifdef SYS_RECORD_SUPPORT
#include "svctx/rec_hdlr/u_rec_hdlr.h"
#include "file_mngr/x_fm.h"
#include "res_mngr/x_rm.h"
#include "res_mngr/drv/x_pwr_ctrl.h"
#include "dlm/dlm_digest.h"
#endif

#include "time_msrt/x_time_msrt.h"
#include "time_msrt/u_time_msrt_name.h"

#ifdef MW_TV_AV_BYPASS_SUPPORT
#include "svctx/svctx_mgr.h"
#include "strm_mngr/bypass_hdlr/u_sm_bypass_event.h"
#include "strm_mngr/bypass_hdlr/u_sm_bypass_hdlr.h"
#endif

#define MW_TV_STATUS_SUPPORT

#ifdef MW_TV_STATUS_SUPPORT
#include "res_mngr/x_rm.h"
#include "res_mngr/u_rm_dev_types.h"
#include "res_mngr/drv/u_drv_cust.h"
#endif

#ifndef MW_TUNER_OUT_SUPPORT
    #define MW_TUNER_OUT_SUPPORT
#endif

#ifdef MW_TUNER_OUT_SUPPORT
#include "svctx/util/tuner_ctrlr.h"
#endif

#ifdef LINUX_TURNKEY_SOLUTION
typedef BOOL (*pf_same_select_listener_nfy_fct)(
        x_svctx_select_nfy_fct      pf_sel_nfy,
        VOID*                       pv_sel_tag,
        x_svctx_listener_nfy_fct    pf_lsn_nfy,
        VOID*                       pv_lsn_tag);
typedef VOID (*pf_free_rpc_cb_tag_if_needed)(
        VOID*                       pv_nfy,
        VOID*                       pv_nfy_tag);
#ifdef SVCTX_SPLIT_SUPPORT
typedef VOID* (*pf_get_block_check_fct_cb_addr)(
        svctx_svc_block_check_fct   pf_block_check_fct,
        VOID*                       pv_block_check_tag);
#endif
static pf_same_select_listener_nfy_fct ri_svctx_same_select_listener_nfy_fct = NULL;
static pf_free_rpc_cb_tag_if_needed    ri_svctx_free_rpc_cb_tag_if_needed = NULL;
#ifdef SVCTX_SPLIT_SUPPORT
static pf_get_block_check_fct_cb_addr  ri_svctx_get_block_check_fct_cb_addr = NULL;
#endif
#endif
/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#define MAX_SVCTX_NUMBER        (8)
#define DEFAULT_SVCTX_NUMBER    (1)
#define DFT_BYTE_ID             0x80
#define OFS_BYTE_ID             0x32

#if 0
#undef  DBG_ERROR
#define DBG_ERROR(_stmt)    x_dbg_stmt _stmt
#endif

#if 0
#undef  DBG_INFO
#define DBG_INFO(_stmt)     x_dbg_stmt _stmt
#endif

#if 0
#undef  DBG_API
#define DBG_API(_stmt)     x_dbg_stmt _stmt
#endif


/*-----------------------------------------------------------------------------
 * SVCTX private methods declarations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
static DLIST_T(_SVCTX_HDLR_T) t_g_handlers;

static BOOL               b_g_init                    = FALSE;
static THREAD_DESCR_T     t_g_svctx_thread_descr      = {0};
static HANDLE_T           h_g_module_sync_sema        = NULL_HANDLE;
static HANDLE_T           h_g_module_wait_sema        = NULL_HANDLE;
static HANDLE_T           h_g_module_sync_stop_sema   = NULL_HANDLE;
static ACTIVATOR_T*       pt_g_activator              = NULL;
SVCTX_CLASS_T             t_g_svctx_class             = {{0, 0}, TRUE};
/* service context pool */
static DLIST_T(_SVCTX_T)  t_g_using_svctx_q;
static DLIST_T(_SVCTX_T)  t_g_pooled_svctx_q;
static UINT16             ui2_g_svctx_amount          = 0;
static SVCTX_T*           pt_g_svctx_objs             = NULL;
static AVC_TYPE_MAPPING*  pt_g_avc_mapping_table      = NULL;
static SIZE_T             z_g_elem_count_of_avc_table = 0;
static UINT32             ui4_g_svc_req_serial_num    = 0;
#ifdef MW_TV_AV_BYPASS_SUPPORT
static BOOL               b_g_disable_reuse_pipe      = FALSE;
static BOOL               b_g_disable_replace_svc     = FALSE;
#endif

#ifdef GINGA_SUPPORT
x_svctx_pre_select_stream_nfy_fct  g_pf_svctx_pre_select_stream_nfy_fct = NULL;
#endif

#ifdef MW_TUNER_OUT_SUPPORT
static TUNER_OUT_DESC_T   t_g_tuner_out;
#endif

#define _RESET_VIDEO_RES(_pt_video_res)                                     \
{                                                                           \
    (_pt_video_res)->ui4_frame_rate         = 0;                            \
    (_pt_video_res)->ui4_width              = 0;                            \
    (_pt_video_res)->ui4_height             = 0;                            \
    (_pt_video_res)->b_is_progressive       = FALSE;                        \
    (_pt_video_res)->b_overscan_not_applied = FALSE;                        \
    (_pt_video_res)->e_src_asp_ratio        = VSH_SRC_ASPECT_RATIO_UNKNOWN; \
    (_pt_video_res)->e_timing_type          = VSH_SRC_TIMING_UNKNOWN;       \
    (_pt_video_res)->e_color_fmt            = VSH_SRC_COLOR_FMT_UNKNOWN;    \
    (_pt_video_res)->e_src_color_space      = VSH_SRC_COLOR_FMT_UNKNOWN;    \
    (_pt_video_res)->ui4_res_idx            = 0;                            \
    (_pt_video_res)->ui4_ext                = 0;                            \
}

#define _RESET_AUDIO_FMT(_pt_audio_fmt)                                     \
{                                                                           \
    (_pt_audio_fmt)->e_aud_fmt              = ASH_AUDIO_FMT_UNKNOWN;        \
    (_pt_audio_fmt)->e_channels             = ASH_CHANNELS_UNKNOWN;         \
    (_pt_audio_fmt)->ui4_sample_rate        = 0;                            \
    (_pt_audio_fmt)->ui4_data_rate          = 0;                            \
    (_pt_audio_fmt)->ui1_bit_depth          = 0;                            \
}

#define _STOP_SVC_ONLY          TRUE
#define _STOP_AND_SELECT_SVC    FALSE

/*-----------------------------------------------------------------------------
 * SVCTX private methods implementations
 *---------------------------------------------------------------------------*/
#ifdef MW_TV_AV_BYPASS_SUPPORT
static VOID _svctx_set_shared_block_checker(
    SVCTX_T*                    pt_svctx,
#ifdef SVCTX_SPLIT_SUPPORT
    svctx_svc_block_check_fct   pf_block_check_fct,
    VOID*                       pv_tag
#else
    x_svctx_svc_block_check_fct pf_block_check_fct
#endif
    );

static VOID _svctx_set_shared_strm_filter(
    SVCTX_T*                        pt_svctx,
    STREAM_TYPE_T                   e_strm_type,
    x_svctx_strm_comp_filter_fct    pt_filter_fct,
    VOID*                           pv_filter_tag);
#endif
/*-----------------------------------------------------------------------------
 * Name: _svctx_force_mute_video
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *-----------------------------------------------------------------------------*/
static INT32 _svctx_force_mute_video (
                    SVCTX_T*                    pt_svctx,
                    VIDEO_MUTE_CTRL_T           e_ctrl
                    )
{
    SCC_BG_COLOR_T  t_video_mute_color;
    INT32           i4_ret = SVCTXR_OK;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    BOOL            b_enable;
#endif

    if (((e_ctrl == VIDEO_MUTE_CTRL_ENABLE) || (e_ctrl == VIDEO_MUTE_CTRL_FORCE_ENABLE)
#ifdef MW_RIKS_TV_SUPPORT
            || svctx_rikstv_in_tuning_state(pt_svctx)
#endif
            )
            && pt_svctx->b_enable_video_mute_supervisor == TRUE
            && pt_svctx->b_enable_snow == FALSE) {
        if (pt_svctx->b_is_blocked == TRUE && pt_svctx->t_block_mute.b_enable == TRUE) {
            t_video_mute_color.ui1_red   = pt_svctx->t_block_mute.t_color.ui1_red;
            t_video_mute_color.ui1_green = pt_svctx->t_block_mute.t_color.ui1_green;
            t_video_mute_color.ui1_blue  = pt_svctx->t_block_mute.t_color.ui1_blue;
        } else {
            t_video_mute_color.ui1_red   = pt_svctx->t_video_mute.t_color.ui1_red;
            t_video_mute_color.ui1_green = pt_svctx->t_video_mute.t_color.ui1_green;
            t_video_mute_color.ui1_blue  = pt_svctx->t_video_mute.t_color.ui1_blue;
        }
#ifdef MW_TV_AV_BYPASS_SUPPORT
        b_enable = TRUE;
#endif
    } else {
        t_video_mute_color.ui1_red   = 0;
        t_video_mute_color.ui1_green = 0;
        t_video_mute_color.ui1_blue  = 0;
#ifdef MW_TV_AV_BYPASS_SUPPORT
        b_enable = (e_ctrl == VIDEO_MUTE_CTRL_SELECT_STOP_SVC);
#endif
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (NULL_HANDLE != pt_svctx->h_video_scc_comp)
#endif
    {
        /* Xia - 090723 for DTV00138760 && DTV00137140 */
        if ((pt_svctx->b_is_blocked ||
             pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS ||
             pt_svctx->t_video_info.e_timing_type == VSH_SRC_TIMING_NOT_SUPPORT ||
             pt_svctx->e_av_status == SVC_AV_STATUS_AUDIO_ONLY ||
             pt_svctx->e_av_status == SVC_AV_STATUS_NO_AUDIO_VIDEO ||
             pt_svctx->e_av_status == SVC_AV_STATUS_SCRAMBLED_AUDIO_VIDEO ||
             pt_svctx->e_av_status == SVC_AV_STATUS_SCRAMBLED_AUDIO_NO_VIDEO ||
             pt_svctx->e_av_status == SVC_AV_STATUS_SCRAMBLED_VIDEO_CLEAR_AUDIO ||
             pt_svctx->e_av_status == SVC_AV_STATUS_SCRAMBLED_VIDEO_NO_AUDIO) &&
             !pt_svctx->b_in_fine_tuning && !pt_svctx->pt_running_svc_req->b_enable_i_frame)
        {
            if (pt_svctx->t_mheg_scr_mode.b_enable)
            {
                i4_ret = x_scc_vid_set_scr_mode(
                                pt_svctx->h_video_scc_comp,
                                pt_svctx->t_mheg_scr_mode.e_scr_mode
                                );
            }
            else
            {
                i4_ret = x_scc_vid_set_scr_mode(
                                pt_svctx->h_video_scc_comp,
                                SCC_VID_SCREEN_MODE_UNKNOWN
                                );
            }
        }
        if (i4_ret != SMR_OK) {
            return SVCTXR_FAIL;
        }

        /* Dian - 090810 set video background moved down for CR139253 */
        i4_ret = x_scc_vid_set_bg(
                        pt_svctx->h_video_scc_comp,
                        &t_video_mute_color
                        );
        if (i4_ret != SMR_OK) {
            return SVCTXR_FAIL;
        }
    }

    if (pt_svctx->b_able_set_pmx_bg == TRUE
#ifdef MW_PIP_POP_SUPPORT
        && (svctx_get_scrn_status(pt_svctx) == SVCTX_SCREEN_STATUS_SINGLE ||
            svctx_get_scrn_status(pt_svctx) == SVCTX_SCREEN_STATUS_MULTIPLE_FULL)
#endif
    ) {
            i4_ret = x_scc_disp_set_bg(
                        pt_svctx->h_display_scc_comp,
                        &t_video_mute_color
                        );
            if (i4_ret != SMR_OK) {
                return SVCTXR_FAIL;
            }
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (IS_SVCTX_ASSOCIATE(pt_svctx))
    {
        if (pt_svctx->b_is_blocked &&
            ((e_ctrl == VIDEO_MUTE_CTRL_ENABLE) || (e_ctrl == VIDEO_MUTE_CTRL_DISABLE)))
        {
            b_enable = TRUE;
        }
        else
        {
            if (((pt_svctx->t_cp_mask.ui4_macrovision & CP_VIDEO) &&
                 (pt_svctx->t_cp_mask.ui4_macrovision & pt_svctx->t_cp_status.ui4_macrovision)) ||
                ((pt_svctx->t_cp_mask.ui4_hdcp & CP_VIDEO) &&
                 (pt_svctx->t_cp_mask.ui4_hdcp & pt_svctx->t_cp_status.ui4_hdcp)) ||
                ((pt_svctx->t_cp_mask.ui4_ca & CP_VIDEO) &&
                 (pt_svctx->t_cp_mask.ui4_ca & pt_svctx->t_cp_status.ui4_ca)) ||
                 pt_svctx->b_cci)
            {
                b_enable = TRUE;
            }
        }

#ifdef MW_PIP_POP_SUPPORT
        if (IS_SVCTX_EXECUTABLE(pt_svctx->ui4_assc_state))
#endif
        {
            svctx_set_bypass_mode(pt_svctx, !b_enable);
        }
    }
#endif

    return  SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_destroy_cb
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static VOID _svctx_destroy_cb(
                    VOID*                       pv_nfy_tag        /* in  */
                    )
{
    HANDLE_T h_destroy_sema = (HANDLE_T) pv_nfy_tag;
    INT32    i4_ret;

    i4_ret = _NOTIFY(h_destroy_sema);

    if(i4_ret != OSR_OK) {
        DBG_ABORT(DBG_MOD_SVCTX);
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_close_cb
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static VOID _svctx_close_cb(
                    VOID*                       pv_nfy_tag        /* in  */
                    )
{
    HANDLE_T h_close_sema = (HANDLE_T) pv_nfy_tag;
    INT32    i4_ret;

    i4_ret = _NOTIFY(h_close_sema);
    if(i4_ret != OSR_OK) {
        DBG_ABORT(DBG_MOD_SVCTX);
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_stop_sync_cb
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static VOID _svctx_stop_sync_cb(
                    VOID*                       pv_nfy_tag        /* in  */
                    )
{
    HANDLE_T h_stop_sema = (HANDLE_T) pv_nfy_tag;
    INT32    i4_ret;

    i4_ret = _NOTIFY(h_stop_sema);
    if(i4_ret != OSR_OK) {
        DBG_ABORT(DBG_MOD_SVCTX);
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_stop_wrapped_cb
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static VOID _svctx_stop_wrapped_cb(
                    VOID*                       pv_nfy_tag        /* in  */
                    )
{
    SVCTX_WRAPPED_TAG_T* pt_wrapped_tag = (SVCTX_WRAPPED_TAG_T*) pv_nfy_tag;

    if (pt_wrapped_tag != NULL)
    {
        pt_wrapped_tag->u.t_stop_svc_cb.pf_nfy(
                pt_wrapped_tag->h_svctx,
                pt_wrapped_tag->u.t_stop_svc_cb.pv_tag);
        x_mem_free(pt_wrapped_tag);
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_reset_video_clk_phs_pos_paramters
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static VOID _svctx_reset_video_clk_phs_pos_paramters(
                    SVCTX_T*                    pt_svctx
                    )
{
    pt_svctx->i2_video_clock          = SM_VSH_VID_COLCK_UNKNOWN;
    pt_svctx->ui2_video_phase         = SM_VSH_VID_PHASE_UNKNOWN;
    pt_svctx->ui2_video_h_pos         = SM_VSH_VID_H_POS_UNKNOWN;
    pt_svctx->ui2_video_v_pos         = SM_VSH_VID_V_POS_UNKNOWN;
    pt_svctx->b_need_auto_clk_phs_pos = FALSE;
    pt_svctx->b_need_auto_color       = FALSE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_reset_parameters_for_new_svc
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static VOID _svctx_reset_parameters_for_new_svc(
                    SVCTX_T*                    pt_svctx
                    )
{
    pt_svctx->t_crnt_nfy_msg.e_svc_code = SVCTX_NTFY_CODE_NONE;

#ifdef MW_TV_AV_BYPASS_SUPPORT
    pt_svctx->b_scdb_updated            = FALSE;
#endif    
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_allocate_object
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static SVCTX_T* _svctx_allocate_object(
                    VOID
                    )
{
    SVCTX_T* pt_svctx = DLIST_HEAD(&t_g_pooled_svctx_q);
    UINT8    ui1_idx;

    if (pt_svctx == NULL) {
        return NULL;
    }

    DLIST_REMOVE(pt_svctx, &t_g_pooled_svctx_q, t_dlink);
    DLIST_INSERT_HEAD(pt_svctx, &t_g_using_svctx_q, t_dlink);

    /* reset*/
    pt_svctx->h_svctx_owner            = NULL_HANDLE;
    pt_svctx->pf_block_check_fct       = NULL;
#ifdef SVCTX_SPLIT_SUPPORT
    pt_svctx->pf_block_check_fct_tag   = NULL;
#endif
    pt_svctx->h_video_scc_comp         = NULL_HANDLE;
    pt_svctx->h_audio_scc_comp         = NULL_HANDLE;
    pt_svctx->h_display_scc_comp       = NULL_HANDLE;
    pt_svctx->h_ad_scc_comp            = NULL_HANDLE;
    pt_svctx->s_src_grp_name[0]        = '\0';
    pt_svctx->s_snk_grp_name[0]        = '\0';
    pt_svctx->b_in_closing             = FALSE;
    pt_svctx->b_in_stopping            = FALSE;
    pt_svctx->pf_sync_close_nfy        = NULL;
    pt_svctx->pf_sync_close_tag        = NULL;
    pt_svctx->h_last_client            = NULL_HANDLE;
    pt_svctx->e_default_vid_mode       = SCC_VID_NORMAL;
    pt_svctx->ui4_frz_frozen_strm_mask = 0;
    pt_svctx->e_aud_av_sync_mode       = SVCTX_AV_SYNC_SLAVE;
    pt_svctx->pt_running_svc_req       = NULL;
    pt_svctx->pt_pending_svc_req       = NULL;

    pt_svctx->t_video_region.ui4_x              = 0;
    pt_svctx->t_video_region.ui4_y              = 0;
    pt_svctx->t_video_region.ui4_width          = VSH_REGION_MAX_WIDTH;
    pt_svctx->t_video_region.ui4_height         = VSH_REGION_MAX_HEIGHT;
    pt_svctx->t_disp_region.ui4_x               = 0;
    pt_svctx->t_disp_region.ui4_y               = 0;
    pt_svctx->t_disp_region.ui4_width           = VSH_REGION_MAX_WIDTH;
    pt_svctx->t_disp_region.ui4_height          = VSH_REGION_MAX_HEIGHT;
    pt_svctx->t_video_plane_order.e_order_ctrl  = VSH_PLANE_ORDER_CTRL_BOTTOM;
    pt_svctx->b_need_set_video_plane_order      = FALSE;
    pt_svctx->b_enable_video_mute_supervisor    = TRUE; /* the overall control */
    pt_svctx->t_video_mute.b_enable             = FALSE;
    pt_svctx->t_video_mute.t_color.ui1_red      = 0; /* t_video_mute_color = black */
    pt_svctx->t_video_mute.t_color.ui1_green    = 0;
    pt_svctx->t_video_mute.t_color.ui1_blue     = 0;
    pt_svctx->t_block_mute.b_enable             = FALSE;
    pt_svctx->t_block_mute.t_color.ui1_red      = 0; /* t_video_mute_color = black */
    pt_svctx->t_block_mute.t_color.ui1_green    = 0;
    pt_svctx->t_block_mute.t_color.ui1_blue     = 0;
    pt_svctx->b_is_blocked                      = FALSE;
    pt_svctx->e_cc_avil_cond                    = SVCTX_CC_AVIL_COND_UNKNOW;
    pt_svctx->b_able_set_pmx_bg                 = FALSE;
    pt_svctx->ui4_stream_mode                   = 0;
    pt_svctx->h_timer                           = NULL_HANDLE;
    pt_svctx->h_video_mute_chk_timer            = NULL_HANDLE;
    pt_svctx->h_scdb_chk_timer                  = NULL_HANDLE;
    pt_svctx->e_av_status                       = SVC_AV_STATUS_UNKNOWN;
    pt_svctx->ui1_stop_audio_as_no_signal       = 0xff; /* 0xff=any src type */ /*MAKE_BIT_MASK_8(SRC_TYPE_BRDCST_SVL) | MAKE_BIT_MASK_8(SRC_TYPE_BRDCST_FREQ);*/
    pt_svctx->e_block_rule                      = SVCTX_BLOCK_RULE_INP_SIGNAL_LOSS_CH_PROG;
    pt_svctx->b_enable_block_bias_freq          = FALSE;
    pt_svctx->ui4_block_bias_freq               = 0;
    pt_svctx->b_disable_no_signal_as_block_bias = FALSE;
    pt_svctx->b_in_restoring_orig_freq          = FALSE;
    pt_svctx->ui4_invalid_svl_nw_mask_bit       = 0;
    pt_svctx->b_disable_all_scart_out           = FALSE;
    pt_svctx->e_auto_color_cond                 = AUTO_COLOR_COND_UNKNOWN;
    pt_svctx->e_auto_clk_pos_phs_cond           = AUTO_CLK_POS_PHS_COND_UNKNOWN;
    pt_svctx->e_detection_mode                  = DETECTION_MODE_BY_SIGNAL_STATUS;
    pt_svctx->e_audio_decode_status             = ASH_AUDIO_DECODE_UNKNOWN;
    pt_svctx->e_video_decode_status             = VSH_DIG_DECODE_UNKNOWN;
    pt_svctx->b_analog_signal_loss_show_snow    = FALSE;
    pt_svctx->b_ad_enabled                      = FALSE;
    pt_svctx->b_select_strm_block               = FALSE;
    pt_svctx->t_crnt_nfy_msg.e_svc_code         = SVCTX_NTFY_CODE_NONE;

#ifdef MW_ISDB_SUPPORT
    pt_svctx->b_is_brdcsting_svc                = TRUE;
#endif
#ifdef MW_TV_AV_BYPASS_SUPPORT
    pt_svctx->b_svc_selected                    = FALSE;
    pt_svctx->b_piped_stop                      = FALSE;
#ifdef MW_PIP_POP_SUPPORT
    pt_svctx->ui4_assc_state                    = 0;
#else
    pt_svctx->b_assc_enabled                    = TRUE;
#endif
    pt_svctx->ui4_attribute                     = 0;
    pt_svctx->b_fake_disc                       = FALSE;
    pt_svctx->t_clr_sys                         = COLOR_SYS_UNKNOWN;
    pt_svctx->t_cp_mask.ui4_macrovision         = CP_NONE;
    pt_svctx->t_cp_mask.ui4_hdcp                = CP_NONE;
    pt_svctx->t_cp_mask.ui4_ca                  = CP_NONE;
    pt_svctx->b_cci                             = FALSE;
    pt_svctx->b_scdb_updated                    = FALSE;
    svctx_reset_copy_protection_status_params(pt_svctx);
#endif
    pt_svctx->e_x_av_status                     = SVC_AV_STATUS_UNKNOWN;
    _RESET_VIDEO_RES(&(pt_svctx->t_x_video_info));
    _RESET_AUDIO_FMT(&(pt_svctx->t_x_audio_info));

#if (defined(MW_TV_AV_BYPASS_SUPPORT) || defined(SYS_RECORD_SUPPORT))
    pt_svctx->i1_prio_diff                      = 0;
#endif

#ifdef SYS_RECORD_SUPPORT
    pt_svctx->b_rec_svctx                       = FALSE;
#endif

    _svctx_reset_video_clk_phs_pos_paramters(pt_svctx);
    svctx_reset_audio_info(pt_svctx);
    svctx_reset_video_info(pt_svctx);

    for (ui1_idx = 0; ui1_idx < (UINT8) STRM_TYPE_COUNT_WITH_AD; ui1_idx++) {   /* QQQQQQQ */
        pt_svctx->pt_def_strm_fltr_fcts[ui1_idx] = NULL;
        pt_svctx->pv_def_strm_fltr_tags[ui1_idx] = NULL;
    }

    pt_svctx->t_mheg_scr_mode.b_enable = FALSE;

    pt_svctx->b_freezed                         = FALSE;

    return (pt_svctx);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_create
 * Description
 *      This API is used to create an instance of the service context
 * Input arguments
 *      ps_svctx_name       the service context name
 * Output arguments
 *      ph_svctx              the handle of an newborn instance of the service
                            context.
 * Returns
 *      SVCTXR_OK            The routine was successful.
 *      SVCTXR_INV_ARG      pt_media_src is NULL
 *      SVCTXR_NO_RESOURCE  out of memory
 *      SVCTXR_FAIL         An unclassified error was happened.
 *-----------------------------------------------------------------------------*/
static INT32 _svctx_create(
                    const CHAR*                 ps_svctx_name,    /* in  */
                    svctx_nfy_fct               pf_nfy,           /* in  */
                    VOID*                       pv_nfy_tag,       /* in  */
                    SVCTX_T**                   ppt_svctx         /* out */
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret;

    pt_svctx = _svctx_allocate_object();
    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"allocate SVCTX_T failed\r\n"));
        return SVCTXR_NO_RESOURCE;
    }

    /* init a new service context */
    x_strcpy(pt_svctx->s_name, ps_svctx_name);
    pt_svctx->e_state            = SVCTX_COND_READY;
    pt_svctx->pt_running_svc_req = NULL;
    pt_svctx->pt_pending_svc_req = NULL;

    i4_ret = x_timer_create(&pt_svctx->h_timer);
    if (i4_ret != OSR_OK) {
        DBG_ERROR((_ERROR_HEADER"create pt_svctx->h_timer failed! ret=%d\r\n", i4_ret));
        svctx_free_object(pt_svctx);
        return SVCTXR_FAIL;
    }

    i4_ret = x_timer_create(&pt_svctx->h_video_mute_chk_timer);
    if (i4_ret != OSR_OK) {
        DBG_ERROR((_ERROR_HEADER"create pt_svctx->h_video_mute_chk_timer failed! ret=%d\r\n", i4_ret));
        svctx_free_object(pt_svctx);
        return SVCTXR_FAIL;
    }

    i4_ret = x_timer_create(&pt_svctx->h_scdb_chk_timer);
    if (i4_ret != OSR_OK) {
        DBG_ERROR((_ERROR_HEADER"create pt_svctx->h_scdb_chk_timer failed! ret=%d\r\n", i4_ret));
        svctx_free_object(pt_svctx);
        return SVCTXR_FAIL;
    }

    *ppt_svctx = pt_svctx;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_handle_free_fct
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *      TRUE
 *      FALSE
 *-----------------------------------------------------------------------------*/
static BOOL _svctx_handle_free_fct(
                    HANDLE_T                    h_handle,
                    HANDLE_TYPE_T               e_type,
                    VOID*                       pv_obj,
                    VOID*                       pv_tag,
                    BOOL                        b_req_handle
                    )
{
    SVCTX_T* pt_svctx = (SVCTX_T*)pv_obj;
    INT32    i4_ret;

    DBG_INFO((_INFO_HEADER"%s()\r\n", __func__));

    if ( b_req_handle == TRUE) {
        return TRUE;
    }

    if (pt_svctx == NULL || (e_type != SVCTXT_SVCTX
#ifdef MW_TV_AV_BYPASS_SUPPORT
        && e_type != SVCTXT_SVCTX_ASSOCIATE
#endif
        ))
    {
        return TRUE;
    }

    i4_ret = handle_delink(&pt_svctx->t_hdl_link, h_handle);
    if(i4_ret != HR_OK) {
        return FALSE;
    }

    if (pv_tag != NULL) { /* SVCTX_LSTNR_T */
        x_mem_free(pv_tag);
    }

    if (pt_svctx->h_sys_svctx == h_handle) {
        /* free the h_sys_svctx, so free everyone associated handle */
        DBG_INFO((_INFO_HEADER"%s(), free the h_sys_svctx, so free everyone associated handle\r\n", __func__));
        handle_free_all(&(pt_svctx->t_hdl_link));
        svctx_free_object(pt_svctx);
        pt_svctx->h_sys_svctx = NULL_HANDLE;
    } else if(handle_num_links(&(pt_svctx->t_hdl_link)) == 1) {
        /* free the last client h_svctx... */
        DBG_INFO((_INFO_HEADER"%s(), last client h_svctx, so free object \r\n", __func__));
        svctx_free_object(pt_svctx);
    }
    return TRUE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_unregister_all_handlers
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static VOID _svctx_unregister_all_handlers(
                    VOID
                    )
{
    SVCTX_HDLR_T* pt_hdlr = NULL;

    for (pt_hdlr = DLIST_HEAD(&t_g_handlers); pt_hdlr != NULL; pt_hdlr = DLIST_HEAD(&t_g_handlers)) {
        if (pt_hdlr->t_fcts.pf_deinit != NULL) {
            pt_hdlr->t_fcts.pf_deinit();
        }

        DLIST_REMOVE(pt_hdlr, &t_g_handlers, t_dlink);
        x_mem_free(pt_hdlr);
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_init_default_av_dev_profile_table
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static INT32 _svctx_init_default_av_dev_profile_table(
                    VOID
                    )
{
    /* default setting is for ATSC */

    AVC_TYPE_MAPPING t_avc_mapping_table[] =
    { /* driver type      ,device type        ,AV classification          ,stream capability                                 ,Audio Quality,Video Quality, event capability                                                        cc capability                subtitle capability            teletext capability        */
      {DRVT_AVC_COMBI     ,{DEV_AVC_COMBI     ,ST_MASK_VIDEO              ,0                                                 ,ORDER_UNKNOWN,ORDER_UNKNOWN, {FALSE, VIDEO_RES_MASK_NONE, DEV_EVN_CAPABILITY_NONE                }, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}}}
#ifndef MW_EDB_SUPPORT
     ,{DRVT_AVC_COMP_VIDEO,{DEV_AVC_COMP_VIDEO,ST_MASK_VIDEO              ,ST_MASK_VIDEO|ST_MASK_CLOSED_CAPTION              ,ORDER_UNKNOWN,1            , {FALSE, VIDEO_RES_MASK_ALL , MAKE_BIT_MASK_32(EVCTX_KEY_TYPE_RATING)}, {TRUE,  VIDEO_RES_MASK_480I_576I}, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}}}
     ,{DRVT_AVC_S_VIDEO   ,{DEV_AVC_S_VIDEO   ,ST_MASK_VIDEO              ,ST_MASK_VIDEO|ST_MASK_CLOSED_CAPTION              ,ORDER_UNKNOWN,2            , {FALSE, VIDEO_RES_MASK_ALL , MAKE_BIT_MASK_32(EVCTX_KEY_TYPE_RATING)}, {TRUE,  VIDEO_RES_MASK_480I_576I}, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}}}
     ,{DRVT_AVC_Y_PB_PR   ,{DEV_AVC_Y_PB_PR   ,ST_MASK_VIDEO              ,ST_MASK_VIDEO|ST_MASK_CLOSED_CAPTION              ,ORDER_UNKNOWN,3            , {TRUE , VIDEO_RES_MASK_480I, MAKE_BIT_MASK_32(EVCTX_KEY_TYPE_RATING)}, {TRUE,  VIDEO_RES_MASK_480I_576I}, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}}}
#else
    ,{DRVT_AVC_COMP_VIDEO,{DEV_AVC_COMP_VIDEO,ST_MASK_VIDEO              ,ST_MASK_VIDEO|ST_MASK_CLOSED_CAPTION              ,ORDER_UNKNOWN,1            , {FALSE, VIDEO_RES_MASK_ALL , MAKE_BIT_MASK_32(EDB_KEY_TYPE_RATING_LIST_BY_ID)}, {TRUE,  VIDEO_RES_MASK_480I_576I}, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}}}
    ,{DRVT_AVC_S_VIDEO   ,{DEV_AVC_S_VIDEO   ,ST_MASK_VIDEO              ,ST_MASK_VIDEO|ST_MASK_CLOSED_CAPTION              ,ORDER_UNKNOWN,2            , {FALSE, VIDEO_RES_MASK_ALL , MAKE_BIT_MASK_32(EDB_KEY_TYPE_RATING_LIST_BY_ID)}, {TRUE,  VIDEO_RES_MASK_480I_576I}, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}}}
    ,{DRVT_AVC_Y_PB_PR   ,{DEV_AVC_Y_PB_PR   ,ST_MASK_VIDEO              ,ST_MASK_VIDEO|ST_MASK_CLOSED_CAPTION              ,ORDER_UNKNOWN,3            , {TRUE , VIDEO_RES_MASK_480I, MAKE_BIT_MASK_32(EDB_KEY_TYPE_RATING_LIST_BY_ID)}, {TRUE,  VIDEO_RES_MASK_480I_576I}, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}}}
#endif
     ,{DRVT_AVC_VGA       ,{DEV_AVC_VGA       ,ST_MASK_VIDEO              ,ST_MASK_VIDEO                                     ,ORDER_UNKNOWN,3            , {FALSE, VIDEO_RES_MASK_NONE, DEV_EVN_CAPABILITY_NONE                }, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}}}
     ,{DRVT_AVC_SCART     ,{DEV_AVC_SCART     ,ST_MASK_VIDEO              ,ST_MASK_VIDEO                                     ,1            ,4            , {FALSE, VIDEO_RES_MASK_NONE, DEV_EVN_CAPABILITY_NONE                }, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}}}
     ,{DRVT_AVC_DVI       ,{DEV_AVC_DVI       ,ST_MASK_VIDEO              ,ST_MASK_VIDEO                                     ,ORDER_UNKNOWN,4            , {FALSE, VIDEO_RES_MASK_NONE, DEV_EVN_CAPABILITY_NONE                }, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}}}
     ,{DRVT_AVC_HDMI      ,{DEV_AVC_HDMI      ,ST_MASK_VIDEO              ,ST_MASK_AUDIO|ST_MASK_VIDEO                       ,2            ,4            , {FALSE, VIDEO_RES_MASK_NONE, DEV_EVN_CAPABILITY_NONE                }, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}}}
     ,{DRVT_TUNER_SAT_DIG ,{DEV_TUNER_SAT_DIG ,ST_MASK_AUDIO|ST_MASK_VIDEO,ST_MASK_AUDIO|ST_MASK_VIDEO|ST_MASK_CLOSED_CAPTION,2            ,5            , {FALSE, VIDEO_RES_MASK_ALL , DEV_EVN_CAPABILITY_ALL                 }, {FALSE, VIDEO_RES_MASK_ALL }, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}}}
     ,{DRVT_TUNER_CAB_DIG ,{DEV_TUNER_CAB_DIG ,ST_MASK_AUDIO|ST_MASK_VIDEO,ST_MASK_AUDIO|ST_MASK_VIDEO|ST_MASK_CLOSED_CAPTION,2            ,5            , {FALSE, VIDEO_RES_MASK_ALL , DEV_EVN_CAPABILITY_ALL                 }, {FALSE, VIDEO_RES_MASK_ALL }, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}}}
     ,{DRVT_TUNER_TER_DIG ,{DEV_TUNER_TER_DIG ,ST_MASK_AUDIO|ST_MASK_VIDEO,ST_MASK_AUDIO|ST_MASK_VIDEO|ST_MASK_CLOSED_CAPTION,2            ,5            , {FALSE, VIDEO_RES_MASK_ALL , DEV_EVN_CAPABILITY_ALL                 }, {FALSE, VIDEO_RES_MASK_ALL }, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}}}
     ,{DRVT_TUNER_SAT_ANA ,{DEV_TUNER_SAT_ANA ,ST_MASK_AUDIO|ST_MASK_VIDEO,ST_MASK_AUDIO|ST_MASK_VIDEO|ST_MASK_CLOSED_CAPTION,0            ,0            , {FALSE, VIDEO_RES_MASK_ALL , DEV_EVN_CAPABILITY_ALL                 }, {FALSE, VIDEO_RES_MASK_ALL }, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}}}
     ,{DRVT_TUNER_CAB_ANA ,{DEV_TUNER_CAB_ANA ,ST_MASK_AUDIO|ST_MASK_VIDEO,ST_MASK_AUDIO|ST_MASK_VIDEO|ST_MASK_CLOSED_CAPTION,0            ,0            , {FALSE, VIDEO_RES_MASK_ALL , DEV_EVN_CAPABILITY_ALL                 }, {FALSE, VIDEO_RES_MASK_ALL }, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}}}
     ,{DRVT_TUNER_TER_ANA ,{DEV_TUNER_TER_ANA ,ST_MASK_AUDIO|ST_MASK_VIDEO,ST_MASK_AUDIO|ST_MASK_VIDEO|ST_MASK_CLOSED_CAPTION,0            ,0            , {FALSE, VIDEO_RES_MASK_ALL , DEV_EVN_CAPABILITY_ALL                 }, {FALSE, VIDEO_RES_MASK_ALL }, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}}}
     ,{DRVT_AVC_AUDIO_INP ,{DEV_AVC_AUDIO_INP ,ST_MASK_AUDIO              ,ST_MASK_AUDIO                                     ,0            ,ORDER_UNKNOWN, {FALSE, VIDEO_RES_MASK_NONE, DEV_EVN_CAPABILITY_NONE                }, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}}}
     ,{DRVT_AVC_SPDIF     ,{DEV_AVC_SPDIF     ,ST_MASK_AUDIO              ,ST_MASK_AUDIO                                     ,1            ,ORDER_UNKNOWN, {FALSE, VIDEO_RES_MASK_NONE, DEV_EVN_CAPABILITY_NONE                }, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}, {FALSE, VIDEO_RES_MASK_NONE}}}
    };
    INT32 i4_ret;

    i4_ret = x_svctx_init_device_mapping_table(
                    t_avc_mapping_table,
                    sizeof(t_avc_mapping_table)/sizeof(AVC_TYPE_MAPPING)
                    );
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: svctx_close_snk_scc_comps
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *-----------------------------------------------------------------------------*/
static VOID svctx_close_snk_scc_comps(
                    SVCTX_T*                    pt_svctx
                    )
{
    INT32 i4_ret;

    if (x_handle_valid(pt_svctx->h_video_scc_comp) == TRUE) {
        i4_ret = x_scc_comp_close(
                            pt_svctx->h_video_scc_comp
                            );
        if (i4_ret != SMR_OK) {
            DBG_ERROR((_ERROR_HEADER"x_scc_comp_close(video) failed, ret=%d\r\n", i4_ret));
        }
    }
    pt_svctx->h_video_scc_comp = NULL_HANDLE;

    if (x_handle_valid(pt_svctx->h_audio_scc_comp) == TRUE) {
        i4_ret = x_scc_comp_close(
                            pt_svctx->h_audio_scc_comp
                            );
        if (i4_ret != SMR_OK) {
            DBG_ERROR((_ERROR_HEADER"x_scc_comp_close(audio) failed, ret=%d\r\n", i4_ret));
        }
    }
    pt_svctx->h_audio_scc_comp = NULL_HANDLE;

    if (x_handle_valid(pt_svctx->h_display_scc_comp) == TRUE) {
        i4_ret = x_scc_comp_close(
                    pt_svctx->h_display_scc_comp
                    );
        if (i4_ret != SMR_OK) {
            DBG_ERROR((_ERROR_HEADER"x_scc_comp_close(display) failed, ret=%d\r\n", i4_ret));
        }
    }
    pt_svctx->h_display_scc_comp = NULL_HANDLE;

    /* modified by Allen */
    if (x_handle_valid(pt_svctx->h_ad_scc_comp) == TRUE) {
        i4_ret = x_scc_comp_close(
                            pt_svctx->h_ad_scc_comp
                            );
        if (i4_ret != SMR_OK) {
            DBG_ERROR((_ERROR_HEADER"x_scc_comp_close(ad) failed, ret=%d\r\n", i4_ret));
        }
    }
    pt_svctx->h_ad_scc_comp = NULL_HANDLE;
}
/*-----------------------------------------------------------------------------
 * Name: svctx_open_snk_scc_comps
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *-----------------------------------------------------------------------------*/
static INT32 svctx_open_snk_scc_comps(
                    SVCTX_T*                    pt_svctx,
                    const CHAR*                 ps_snk_grp_name,
                    const CHAR*                 ps_snk_grp_name_aux
                    )
{
    SM_COMMAND_T t_cmd[3];
    INT32        i4_ret;

    if (x_strlen(ps_snk_grp_name) == 0) {
        return SVCTXR_FAIL;
    }

    if (x_handle_valid(pt_svctx->h_video_scc_comp) == FALSE
            || x_handle_valid(pt_svctx->h_audio_scc_comp) == FALSE
            || x_handle_valid(pt_svctx->h_display_scc_comp) == FALSE
            /* modified by Allen */
            || x_handle_valid(pt_svctx->h_ad_scc_comp) == FALSE
            || x_strcmp(pt_svctx->s_snk_grp_name, ps_snk_grp_name) != 0
            || x_strcmp(pt_svctx->s_snk_grp_name_aux, ps_snk_grp_name_aux) != 0) {
        svctx_close_snk_scc_comps(pt_svctx);
        x_strcpy(pt_svctx->s_snk_grp_name, ps_snk_grp_name);

        /* modified by Allen: Fix me after having the group name containing the second audio decoder */
        if (x_strlen(ps_snk_grp_name_aux) != 0){
            x_strcpy(pt_svctx->s_snk_grp_name_aux, ps_snk_grp_name_aux);
        }
    } else {
        return SVCTXR_OK;
    }

    t_cmd[0].e_code     = SCC_CMD_CODE_GRP_TYPE;
    t_cmd[1].e_code     = SCC_CMD_CODE_NAME;
    t_cmd[1].u.ps_text  = pt_svctx->s_snk_grp_name;
    t_cmd[2].e_code     = SM_CMD_CODE_END;

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (!IS_SVCTX_ASSOCIATE(pt_svctx))
    {
#endif
        t_cmd[0].u.ui4_data = SCC_CMD_TYPE_VIDEO;

        i4_ret = x_scc_comp_open(
                            t_cmd,
                            &(pt_svctx->h_video_scc_comp)
                            );

        if (i4_ret != SMR_OK) {
            DBG_ERROR((_ERROR_HEADER"x_scc_comp_open(video plane) failed, ret=%d\r\n", i4_ret));
            svctx_close_snk_scc_comps(pt_svctx);
            return SVCTXR_FAIL;
        }
#ifdef MW_TV_AV_BYPASS_SUPPORT
    }
    else
    {
        pt_svctx->h_video_scc_comp = NULL_HANDLE;
    }
#endif

    t_cmd[0].u.ui4_data = SCC_CMD_TYPE_AUDIO;
    i4_ret = x_scc_comp_open(
                        t_cmd,
                        &(pt_svctx->h_audio_scc_comp)
                        );
    if (i4_ret != SMR_OK) {
        DBG_ERROR((_ERROR_HEADER"x_scc_comp_open(audio) failed, ret=%d\r\n", i4_ret));
        svctx_close_snk_scc_comps(pt_svctx);
        return SVCTXR_FAIL;
    }

    t_cmd[0].u.ui4_data = SCC_CMD_TYPE_DISP;
    i4_ret = x_scc_comp_open(
                        t_cmd,
                        &(pt_svctx->h_display_scc_comp)
                        );
    if (i4_ret != SMR_OK) {
        DBG_ERROR((_ERROR_HEADER"x_scc_comp_open(display) failed, ret=%d\r\n", i4_ret));
        svctx_close_snk_scc_comps(pt_svctx);
        return SVCTXR_FAIL;
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (!IS_SVCTX_ASSOCIATE(pt_svctx))
    {
#endif
        t_cmd[0].u.ui4_data = SCC_CMD_TYPE_AUDIO;
        t_cmd[1].u.ps_text  = pt_svctx->s_snk_grp_name_aux;
        i4_ret = x_scc_comp_open(
                            t_cmd,
                            &(pt_svctx->h_ad_scc_comp)
                            );
        if (i4_ret != SMR_OK) {
            DBG_ERROR((_ERROR_HEADER"x_scc_comp_open(audio) failed, ret=%d\r\n", i4_ret));
            svctx_close_snk_scc_comps(pt_svctx);
            return SVCTXR_FAIL;
        }
#ifdef MW_TV_AV_BYPASS_SUPPORT
    }
    else
    {
        pt_svctx->h_ad_scc_comp = NULL_HANDLE;
    }
#endif

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_stop_running_svc
 * Description
 * Input arguments
 * Output arguments
 *      -
 * Returns
 *      -
 *-----------------------------------------------------------------------------*/
static INT32 _svctx_stop_running_svc(
                    SVCTX_T*                    pt_svctx,
                    BOOL                        b_stop_only
                    )
{
    INT32 i4_ret;

    svctx_timer_stop(pt_svctx);

    do {
        svctx_ctrl_snow(pt_svctx, FALSE);

        if (pt_svctx->pt_running_svc_req == NULL) {
            DBG_INFO((_INFO_HEADER"%s() failed, pt_running_svc_req is NULL\r\n", __func__));
            i4_ret = SVCTXR_OK;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_stop_svc_req == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }
        
        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_set == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }
        
#ifdef MW_TV_AV_BYPASS_SUPPORT
        pt_svctx->b_svc_selected = FALSE;

        svctx_mgr_remove_queued_req(
            ACTIVE_LINK_NOW,
            pt_svctx->pt_running_svc_req->e_src_type,
            pt_svctx);
         
        pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_set (
                    pt_svctx,
                    SVCTX_INTL_COMMON_SET_TYPE_QUEUED_REQ_REMOVED,
                    NULL,
                    0);
    
        if ((_STOP_SVC_ONLY == b_stop_only) && (FALSE == pt_svctx->b_piped_stop))
        {
            svctx_mgr_remove_link_with_obj(ACTIVE_LINK_NEXT, pt_svctx);
        }
#endif
        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_stop_svc_req(pt_svctx, b_stop_only);
    } while (0);

    if (i4_ret == SVCTXR_PENDING) {
        pt_svctx->b_in_stopping = TRUE;
        pt_svctx->e_state       = SVCTX_COND_PENDING;
    } else if (i4_ret == SVCTXR_OK) {
        pt_svctx->b_in_stopping = FALSE;
        pt_svctx->e_state       = SVCTX_COND_READY;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    } else if (SVCTXR_NOT_EXIST == i4_ret) {
        svctx_free_svc_req(pt_svctx->pt_running_svc_req);
        pt_svctx->pt_running_svc_req = NULL;

        pt_svctx->b_in_stopping = FALSE;
        pt_svctx->e_state       = SVCTX_COND_READY;
#endif
    }

    return i4_ret;
}
#ifdef MW_TUNER_OUT_SUPPORT
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_set_analog_tuner_out_attr
 * Description
 * Input arguments
 * Output arguments
 *      -
 * Returns
 *      -
 *-----------------------------------------------------------------------------*/
static VOID _svctx_set_analog_tuner_out_attr(
                    SVCTX_T*                    pt_svctx
                    )
{
    if (pt_svctx->b_enable_block_bias_freq == TRUE
            && pt_svctx->b_disable_no_signal_as_block_bias == TRUE) {
        /* if under bias freqency as block, don't handle
         * the set analog tuner out request
         */
        return;
    }

    switch(t_g_tuner_out.e_type) {
    case TUNER_OUT_TYPE_ENABLE:
      {
        TUNER_CTRLR_INFO_T t_tsl_info;

        t_tsl_info.ui2_tsl_id     = t_g_tuner_out.t_info.t_svl_rec.uheader.t_rec_hdr.ui2_tsl_id;
        t_tsl_info.ui2_tsl_rec_id = t_g_tuner_out.t_info.t_svl_rec.uheader.t_rec_hdr.ui2_tsl_rec_id;
        t_tsl_info.ui4_tv_sys     = t_g_tuner_out.t_info.t_svl_rec.u_data.t_analog.ui4_tv_sys;

        tuner_ctrlr_enable_ana_tuner(&t_tsl_info);
        break;
      }
    case TUNER_OUT_TYPE_DISABLE:
        tuner_ctrlr_disable_ana_tuner();
        break;
    default:
        break;
    }
}
#else
#define _svctx_set_analog_tuner_out_attr(p1)
#define tuner_ctrlr_disable_ana_tuner()
#define tuner_ctrlr_enable_ana_tuner(p1)
#endif

#ifdef MW_TV_AV_BYPASS_SUPPORT
static INT32 _svctx_try_selecting_svc(SVCTX_T* pt_svctx)
{
    INT32 i4_ret = SVCTXR_OK;

    if (pt_svctx->pt_running_svc_req)
    {
        VAR_INFO_T  t_var_info;
        SRC_TYPE_T  e_src_type;

        t_var_info.e_var_type = VAR_TYPE_SERVICE;
        t_var_info.e_op_type  = OP_TYPE_OPEN;
        t_var_info.ui1_msg_id = _SVCTX_MSG_QUEUED_SELECT_SVC_REQ;

        e_src_type = pt_svctx->pt_running_svc_req->e_src_type;

        if (FALSE == svctx_mgr_ready_to_proceed(
            ACTIVE_LINK_NEXT,
            e_src_type,
            pt_svctx,
            &t_var_info))
        {
            DBG_SVCTXMGR_MSG((
                "{SVCTX_MGR} SVCTX(%s): Selecting service POSTPONED...\r\n",
                pt_svctx->s_name));

            svctx_mgr_check_queued_req(
                ACTIVE_LINK_NEXT,
                e_src_type,
                pt_svctx,
                TRUE);

            i4_ret = SVCTXR_PENDING;
        }
        else
        {
            DBG_SVCTXMGR_MSG((
                "{SVCTX_MGR} SVCTX(%s): Selecting service CONTINUED...\r\n",
                pt_svctx->s_name));

            pt_svctx->b_svc_selected = TRUE;

            svctx_mgr_check_queued_req(
                ACTIVE_LINK_NEXT,
                e_src_type,
                pt_svctx,
                TRUE);

            /* remove the link object */
            svctx_mgr_remove_link_with_obj(
                ACTIVE_LINK_NEXT,
                pt_svctx);

            svctx_mgr_reconfig_link_with_obj(
                ACTIVE_LINK_NOW,
                pt_svctx->pt_running_svc_req->e_src_type,
                pt_svctx,
                (IS_SVCTX_ASSOCIATE(pt_svctx)
#ifdef SYS_RECORD_SUPPORT
                 || IS_SVCTX_RECORD(pt_svctx)
#endif
                ? FALSE : TRUE));

            i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_select_svc_req(pt_svctx);
        }
    }
    else
    {
        /* remove the link object */
        svctx_mgr_remove_link_with_obj(
            ACTIVE_LINK_NEXT,
            pt_svctx);
    }

    return (i4_ret);
}
#endif


#ifndef MW_TV_STATUS_SUPPORT
#define _svctx_set_tv_status(fgTVOn)        SVCTXR_OK
#else
#define _SVCTX_SRC_TYPE_IS_MMP(e_src_type)       \
     (!((e_src_type == SRC_TYPE_AVC) ||          \
      (e_src_type == SRC_TYPE_BRDCST_SVL) ||     \
      (e_src_type == SRC_TYPE_BRDCST_FREQ)))     
      
static INT32 _svctx_set_tv_status(BOOL fgTVon)
{
    DRV_CUST_OPERATION_INFO_T    t_op_info;
    UINT32                       ui4_tv_status = 0;
    SIZE_T                       z_size;
    INT32                        i4_ret = SVCTXR_OK;

   if(fgTVon)
   {
       ui4_tv_status |= MISC_TYPE_TV_STATUS_TV_ON;
       x_dbg_stmt("[SVCTX] Set TV Status TV ON!\n");
   }
   else
   {     
       ui4_tv_status &= ~MISC_TYPE_TV_STATUS_TV_ON; 
       x_dbg_stmt("[SVCTX] Set TV Status TV OFF!\n");
   }
    x_memset(&t_op_info, (UINT8)0, sizeof(t_op_info));
    z_size = sizeof(t_op_info);

    t_op_info.e_op_type                     = DRV_CUSTOM_OPERATION_TYPE_MISC_SET;
    t_op_info.u.t_misc_set_info.e_misc_type = DRV_CUSTOM_MISC_TYPE_TV_STATUS;
    t_op_info.u.t_misc_set_info.pv_set_info = &ui4_tv_status;
    t_op_info.u.t_misc_set_info.z_size      = sizeof(ui4_tv_status);

    i4_ret = x_rm_set_comp(DRVT_CUST_DRV,
                           DRV_CUST_COMP_ID,
                           FALSE,
                           ANY_PORT_NUM,
                           0,
                           &t_op_info,
                           z_size);
    if(RMR_OK != i4_ret){

        x_dbg_stmt("[SVCTX] x_rm_set_comp set TV Status Fail i4_ret = 0X%x\n", i4_ret);
        return SVCTXR_FAIL;
    }

    x_dbg_stmt("[SVCTX] x_rm_set_comp set TV Status Success i4_ret = 0X%x\n", i4_ret);
    
    return SVCTXR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_select_running_svc
 * Description
 * Input arguments
 * Output arguments
 *      -
 * Returns
 *      -
 *-----------------------------------------------------------------------------*/
static INT32 _svctx_select_running_svc(
                    SVCTX_T*                    pt_svctx
                    )
{
    INT32 i4_ret;

    do {
        if (pt_svctx->pt_running_svc_req == NULL)
        {
            i4_ret = SVCTXR_OK;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_select_svc_req == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        pt_svctx->e_cc_avil_cond                    = SVCTX_CC_AVIL_COND_UNKNOW;
        svctx_ctrl_block(pt_svctx, FALSE);
        pt_svctx->b_block_notified                  = FALSE;
        pt_svctx->e_signal_status                   = SIGNAL_STATUS_UNKNOWN;
        pt_svctx->b_signal_loss_notified            = FALSE;
        pt_svctx->b_delay_video_mute_check          = FALSE;
        pt_svctx->e_video_mute_code                 = SVCTX_NTFY_CODE_AUDIO_VIDEO_SVC;
        pt_svctx->ui4_video_mute_chk_delay          = DEFAULT_SIGNAL_LOSS_CHK_TIMEOUT;
        pt_svctx->e_av_status                       = SVC_AV_STATUS_UNKNOWN;
        pt_svctx->b_in_fine_tuning                  = FALSE;
        pt_svctx->b_disable_no_signal_as_block_bias = FALSE;
        pt_svctx->b_in_restoring_orig_freq          = FALSE;
        pt_svctx->e_auto_color_cond                 = AUTO_COLOR_COND_UNKNOWN;
        pt_svctx->e_auto_clk_pos_phs_cond           = AUTO_CLK_POS_PHS_COND_UNKNOWN;
        pt_svctx->e_audio_decode_status             = ASH_AUDIO_DECODE_UNKNOWN;
        pt_svctx->e_video_decode_status             = VSH_DIG_DECODE_UNKNOWN;
        svctx_ctrl_snow(pt_svctx, FALSE);

        if (pt_svctx->pt_running_svc_req->e_src_type != SRC_TYPE_BRDCST_FREQ) {
            if (pt_svctx->pt_running_svc_req->ui4_stream_mode == SVCTX_STRM_MODE_AS_LAST) {
                pt_svctx->pt_running_svc_req->ui4_stream_mode = pt_svctx->ui4_stream_mode;
            } else {
                pt_svctx->ui4_stream_mode = pt_svctx->pt_running_svc_req->ui4_stream_mode;
            }
        }

        svctx_notify_clients_svc_msg(
                    pt_svctx,
                    SVCTX_NTFY_CODE_SERVICE_CHANGING,
                    ST_UNKNOWN,
                    pt_svctx->pt_running_svc_req->h_svctx,
                    pt_svctx->pt_running_svc_req->pf_svc_nfy
                    );

        _svctx_set_analog_tuner_out_attr(pt_svctx);
#ifdef MW_TV_AV_BYPASS_SUPPORT
        i4_ret = _svctx_try_selecting_svc(pt_svctx);
#else
        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_select_svc_req(pt_svctx);
#endif
    } while (0);

    if (i4_ret == SVCTXR_PENDING) {
        pt_svctx->e_state = SVCTX_COND_PENDING;
    } else if (i4_ret == SVCTXR_OK) {
        pt_svctx->e_state = SVCTX_COND_PRESENTING;
    } else {
        i4_ret = _svctx_stop_running_svc(pt_svctx, _STOP_AND_SELECT_SVC);
        if (i4_ret == SVCTXR_OK) {
            svctx_ntfy_svc_req_stopped(pt_svctx);
        }
    }
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_is_same_svc_req_type
 * Description
 *      This API is used to check the two SVC_REQ_T is the same type or not
 * Input arguments
 *      pt_chkee            the pointer points to an SVC_REQ_T object
 *      pt_chker            the pointer points to an SVC_REQ_T object
 * Output arguments
 *      -
 * Returns
 *      BOOL
 *-----------------------------------------------------------------------------*/
static BOOL _svctx_is_same_svc_req_type(
                    SVC_REQ_T*                  pt_chkee,
                    SVC_REQ_T*                  pt_chker
                    )
{
    if (pt_chkee == NULL || pt_chker == NULL) {
        return FALSE;
    }

    if (pt_chkee->e_src_type == pt_chker->e_src_type) {
        return TRUE;
    }

    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_replace_running_svc
 * Description
 * Input arguments
 * Output arguments
 *      -
 * Returns
 *      -
 *-----------------------------------------------------------------------------*/
static INT32 _svctx_replace_running_svc(
                    SVCTX_T*                    pt_svctx
                    )
{
    INT32      i4_ret;
    SVC_REQ_T* pt_svc_req = NULL;

    if (pt_svctx->pt_running_svc_req == NULL || pt_svctx->pt_pending_svc_req == NULL) {
        return SVCTXR_FAIL;
    }

    if (pt_svctx->pt_pending_svc_req->ui4_stream_mode == SVCTX_STRM_MODE_AS_LAST) {
        pt_svctx->pt_pending_svc_req->ui4_stream_mode = pt_svctx->ui4_stream_mode;
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (b_g_disable_replace_svc) {
        DBG_INFO((_INFO_HEADER"svc not replaced, b_g_disable_replace_svc==TRUE.\r\n"));
        return SVCTXR_FAIL;
    }
#endif
    pt_svc_req                   = pt_svctx->pt_running_svc_req;
    pt_svctx->pt_running_svc_req = pt_svctx->pt_pending_svc_req;
    pt_svctx->pt_pending_svc_req = pt_svc_req;

    do {
        i4_ret = SVCTXR_FAIL;

        if (pt_svctx->b_in_stopping == TRUE)
        {
            break;
        }
        
        if (_svctx_is_same_svc_req_type(
                        pt_svctx->pt_pending_svc_req,
                        pt_svctx->pt_running_svc_req) == FALSE) {
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            break;
        }

#ifdef MW_PIP_POP_SUPPORT
        if (!svctx_is_pap(pt_svctx))
#endif
#ifdef MW_TV_AV_BYPASS_SUPPORT
        if (!IS_SVCTX_ASSOCIATE(pt_svctx))
        {
            SVCTX_T* pt_svctx_assc = NULL;
            LINK_POS_T      t_pos  = NULL;
                
            if (_IN_RUNNING_STATE(pt_svctx->e_state))
            {
                BOOL b_break = FALSE;
                
                while (1)
                {
                    pt_svctx_assc = svctx_mgr_get_assc_obj_by_lead(pt_svctx, pt_svctx->pt_pending_svc_req->e_src_type, &t_pos);
        
                    if (pt_svctx_assc)
                    {
#ifdef SYS_RECORD_SUPPORT
                        if (!IS_SVCTX_RECORD(pt_svctx_assc))
#endif
                        {
#ifdef MW_PIP_POP_SUPPORT
                            if (IS_SVCTX_DISABLED(pt_svctx_assc->ui4_assc_state))
#else
                            if (!pt_svctx_assc->b_assc_enabled)
#endif
                            {
                                if (pt_svctx_assc->i1_prio_diff < pt_svctx->i1_prio_diff)
                                {
                                    b_break = TRUE;
                                    break;
                                }
                            }
                            else
                            {
                                if (!_IN_RUNNING_STATE(pt_svctx_assc->e_state) && pt_svctx_assc->i1_prio_diff < pt_svctx->i1_prio_diff)
                                {
                                    b_break = TRUE;
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                }

                if (b_break)
                {
                    break;
                }
            }
        }
#endif

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_replace_svc_req == NULL) {
            break;
        }

        pt_svctx->b_block_notified          = FALSE;
        pt_svctx->b_signal_loss_notified    = FALSE;
        pt_svctx->ui4_video_mute_chk_delay  = DEFAULT_SIGNAL_LOSS_CHK_TIMEOUT;

        /* disable the delay mechanism */
        pt_svctx->b_delay_video_mute_check = TRUE;
        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_replace_svc_req(pt_svctx);
        pt_svctx->b_delay_video_mute_check = FALSE;
    } while (0);

    if (i4_ret == SVCTXR_OK || i4_ret == SVCTXR_PENDING)
    {
#ifdef SYS_RECORD_SUPPORT
        if (IS_SVCTX_RECORD (pt_svctx))
        {
            pt_svc_req                   = pt_svctx->pt_running_svc_req;
            pt_svctx->pt_running_svc_req = pt_svctx->pt_pending_svc_req;
            pt_svctx->pt_pending_svc_req = pt_svc_req;
        }
#endif

        if (pt_svctx->b_in_stopping == FALSE)
        {
            svctx_free_svc_req(pt_svctx->pt_pending_svc_req);
            pt_svctx->ui4_stream_mode    = pt_svctx->pt_running_svc_req->ui4_stream_mode;
            pt_svctx->pt_pending_svc_req = NULL;
        }
    } else {
        pt_svc_req                   = pt_svctx->pt_running_svc_req;
        pt_svctx->pt_running_svc_req = pt_svctx->pt_pending_svc_req;
        pt_svctx->pt_pending_svc_req = pt_svc_req;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_is_running_and_pending_req_inp_blocked
 * Description
 *      This API is used to check running and pending req are all input source locked
 *      or not.
 * Input arguments
 *      pt_svctx           References to a service context.
 * Output arguments
 *      -
 * Returns
 *      TRUE
 *      FALSE;
 *-----------------------------------------------------------------------------*/
static BOOL _svctx_is_running_and_pending_req_inp_blocked(
                    SVCTX_T*                    pt_svctx            /* in */
                    )
{
    if (pt_svctx->pt_running_svc_req == NULL
            || pt_svctx->pt_running_svc_req->pt_hdlr == NULL
            || pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_is_blocked_source == NULL) {
        return FALSE;
    }

    if (pt_svctx->pt_pending_svc_req == NULL
            || pt_svctx->pt_pending_svc_req->pt_hdlr == NULL
            || pt_svctx->pt_pending_svc_req->pt_hdlr->t_fcts.pf_is_blocked_source == NULL) {
        return FALSE;
    }

    if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_is_blocked_source(
                            pt_svctx,
                            pt_svctx->pt_running_svc_req) == TRUE
            && pt_svctx->pt_pending_svc_req->pt_hdlr->t_fcts.pf_is_blocked_source(
                            pt_svctx,
                            pt_svctx->pt_pending_svc_req) == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_process_message
 * Description
 *      This API is used to be invoked by _svctx_dispatcher_fct() method, and
 *      is used to process svc context related events
 * Input arguments
 *      h_svctx         the handle of an SVCTX_T object
 *      pt_msg          the pointer points to an SVCTX_MSG_T object
 * Output arguments
 *      -
 * Returns
 *      -
 *-----------------------------------------------------------------------------*/
static VOID _svctx_process_message(
                    SVCTX_T*                    pt_svctx,           /* in */
                    SVCTX_MSG_T*                pt_msg              /* in */
                    )
{
    INT32 i4_ret = SVCTXR_OK;

    ASSERT(pt_svctx != NULL && pt_msg != NULL,
                    ("Internal Error! pt_svctx or pt_msg is NULL!!!"));

    switch(pt_msg->ui1_msg_id) {
    case _SVCTX_MSG_CLOSE_REQ:
        DBG_INFO((_INFO_HEADER"%s(_SVCTX_MSG_CLOSE_REQ)\r\n", __func__));
        pt_svctx->pf_sync_close_nfy = pt_msg->u.t_close_req.pf_nfy;
        pt_svctx->pf_sync_close_tag = pt_msg->u.t_close_req.pv_tag;

        /* if the client is the last client, start to free realted resource
         */
        if (handle_num_links(&(pt_svctx->t_hdl_link)) == 2) {
            if (pt_svctx->pt_running_svc_req != NULL) {
                pt_svctx->pt_running_svc_req->t_stop_cb.pf_nfy = NULL;
                pt_svctx->pt_running_svc_req->t_stop_cb.pv_tag = NULL;
            }

            /* h_client + h_sys_svctx = 2 */
            i4_ret = _svctx_stop_running_svc(pt_svctx, _STOP_SVC_ONLY);
            if (i4_ret == SVCTXR_PENDING) {
                pt_svctx->h_last_client = pt_msg->h_svctx;
                pt_svctx->b_in_closing  = TRUE;
                break;
            }

            if (i4_ret != SVCTXR_OK) {
                break;
            }
        }

        i4_ret = handle_free(pt_msg->h_svctx, FALSE);
        if (i4_ret != HR_OK) {
            /* DTV00000560 - check x_dbg_abort() usage */
            DBG_ABORT(DBG_MOD_SVCTX);
        }

        if (pt_svctx->h_svctx_owner == pt_msg->h_svctx) {
            pt_svctx->h_svctx_owner = NULL_HANDLE;
        }

        if (pt_svctx->pf_sync_close_nfy != NULL) {
            pt_svctx->pf_sync_close_nfy(
                    pt_svctx->pf_sync_close_tag
                    );
        }
        break;
    case _SVCTX_MSG_SELECT_SVC_REQ:
        svctx_cli_print_channel_change_log(pt_svctx, CH_XNG_STAGE_START_SELECT_SVC);        
#ifdef MW_TV_AV_BYPASS_SUPPORT
        DBG_SVCTXMGR_MSG((
            "\r\n{SVCTX_MGR} Received _SVCTX_MSG_SELECT_SVC_REQ (%s)\r\n\r\n",
            pt_svctx->s_name));
#endif
        if (pt_msg->u.pt_svc_req == NULL) {
            break;
        }

#ifdef SYS_RECORD_SUPPORT
        if (!IS_SVCTX_RECORD (pt_svctx))
#endif
        {
            svctx_open_snk_scc_comps(
                            pt_svctx,
                            pt_msg->u.pt_svc_req->s_snk_grp_name,
                            pt_msg->u.pt_svc_req->s_snk_grp_name_aux
                            );
        }

        svctx_notify_clients_svc_msg(
                    pt_svctx,
                    SVCTX_NTFY_CODE_SVCTX_THREAD_KICK_OFF,
                    ST_UNKNOWN,
                    pt_msg->u.pt_svc_req->h_svctx,
                      pt_msg->u.pt_svc_req->pf_svc_nfy);

        /* free queued SVC_REQ */
        if (pt_svctx->pt_pending_svc_req != NULL) {
            svctx_free_svc_req(pt_svctx->pt_pending_svc_req);
            pt_svctx->pt_pending_svc_req = NULL;
        }

        _svctx_reset_video_clk_phs_pos_paramters(pt_svctx);
        _svctx_reset_parameters_for_new_svc(pt_svctx);

        if (pt_svctx->e_state == SVCTX_COND_READY) {

            /*x_dbg_stmt("[%s][%s][%d] select ready\r\n", pt_svctx->s_name, __func__, __LINE__);*/
            
            pt_svctx->ui4_frz_frozen_strm_mask = 0;
#ifdef MW_TV_AV_BYPASS_SUPPORT
            svctx_reset_copy_protection_status_params(pt_svctx);
#endif
            #ifdef TIME_MEASUREMENT
            if (x_strcmp(pt_svctx->s_name, "main_svctx") == 0)
            {
                TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME, "SEL_SVC");
            }
            TMS_BEGIN_EX(TMS_FLAG_CHG_CHL, "CLOSE_OLD_SESS", NULL);
            #endif

            TMS_BEGIN("SELECT_SVC");
            TMS_BEGIN("SELECT_SVC_2_SESS_STARTED");
            TMS_BEGIN("SELECT_SVC_2_VIDEO_FMT_NTY");

            pt_svctx->t_video_info.e_timing_type = VSH_SRC_TIMING_UNKNOWN;
            pt_svctx->e_av_status                = SVC_AV_STATUS_UNKNOWN;
#ifdef MW_ISDB_SUPPORT
            pt_svctx->b_is_brdcsting_svc        = TRUE;
#endif
            pt_svctx->pt_running_svc_req = pt_msg->u.pt_svc_req;

#ifdef SYS_RECORD_SUPPORT
            if (!IS_SVCTX_RECORD (pt_svctx))
#endif
            {
                _svctx_force_mute_video(pt_svctx, VIDEO_MUTE_CTRL_DISABLE);
                if (pt_svctx->pt_running_svc_req->b_freeze_change == FALSE)
                {
                    svctx_set_video_plane(pt_svctx, SCC_VID_BLANK);
                    svctx_ctrl_snow(pt_svctx, FALSE);
                }
            }

#ifdef MW_TV_STATUS_SUPPORT
            {  
                 /*  current src is not MMP */
                if(!_SVCTX_SRC_TYPE_IS_MMP(pt_svctx->pt_running_svc_req->e_src_type)) 
                {
                    x_dbg_stmt("[SVCTX] SVCTX Ready: SVC Src = %d is not MMP, set TV On\n",
                               pt_svctx->pt_running_svc_req->e_src_type);
                    _svctx_set_tv_status(TRUE);
                }
                else 
                {
                    x_dbg_stmt("[SVCTX] SVCTX Ready: SVC Src = %d is MMP,set TV Off\n",
                               pt_svctx->pt_running_svc_req->e_src_type);
                    _svctx_set_tv_status(FALSE);
                }
            }
#endif

            i4_ret = _svctx_select_running_svc(pt_svctx);
        } else {
#ifdef MW_TV_AV_BYPASS_SUPPORT
            SRC_TYPE_T e_src_type;
            e_src_type = pt_msg->u.pt_svc_req->e_src_type;
#endif
            pt_svctx->pt_pending_svc_req = pt_msg->u.pt_svc_req;

            i4_ret = _svctx_replace_running_svc(pt_svctx);
            if (i4_ret == SVCTXR_OK || i4_ret == SVCTXR_PENDING) {
                /*x_dbg_stmt("[%s][%s][%d] select replace\r\n", pt_svctx->s_name, __func__, __LINE__); */
#ifdef MW_TV_AV_BYPASS_SUPPORT
                DBG_SVCTXMGR_MSG((
                    "{SVCTX_MGR} Running service is replaced (%s).\r\n",
                    pt_svctx->s_name));
                svctx_mgr_check_queued_req(ACTIVE_LINK_NEXT, e_src_type, pt_svctx, TRUE);
                svctx_mgr_remove_link_with_obj(ACTIVE_LINK_NEXT, pt_svctx);
#endif
                break;
            }

#ifdef SVCTX_RATING_BLOCK_DELAY            
            if ((pt_svctx->pt_running_svc_req->e_src_type == SRC_TYPE_AVC) &&
                ((pt_svctx->pt_pending_svc_req->e_src_type == SRC_TYPE_BRDCST_SVL) ||
                 (pt_svctx->pt_pending_svc_req->e_src_type == SRC_TYPE_BRDCST_FREQ)))
            {
                pt_svctx->b_avc_to_tv_block_delay = TRUE;
            }
            else
            {
                pt_svctx->b_avc_to_tv_block_delay = FALSE;
            }
#endif            

            pt_svctx->ui4_frz_frozen_strm_mask = 0;
#ifdef MW_TV_AV_BYPASS_SUPPORT
            svctx_reset_copy_protection_status_params(pt_svctx);
#endif
            #ifdef TIME_MEASUREMENT
            if (x_strcmp(pt_svctx->s_name, "main_svctx") == 0)
            {
                TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME, "SEL_SVC");
            }
            TMS_BEGIN_EX(TMS_FLAG_CHG_CHL, "CLOSE_OLD_SESS", NULL);
            #endif

            TMS_BEGIN("SELECT_SVC");
            TMS_BEGIN("SELECT_SVC_2_SESS_STARTED");
            TMS_BEGIN("SELECT_SVC_2_VIDEO_FMT_NTY");

            pt_svctx->t_video_info.e_timing_type = VSH_SRC_TIMING_UNKNOWN;
            pt_svctx->e_av_status                = SVC_AV_STATUS_UNKNOWN;
#ifdef MW_ISDB_SUPPORT
            pt_svctx->b_is_brdcsting_svc        = TRUE;
#endif

#ifdef SYS_RECORD_SUPPORT
            if (!IS_SVCTX_RECORD (pt_svctx))
#endif
            {
                if (_svctx_is_running_and_pending_req_inp_blocked(pt_svctx) == FALSE) {
                    _svctx_force_mute_video(pt_svctx, VIDEO_MUTE_CTRL_SELECT_STOP_SVC);
                }

                if (pt_svctx->pt_pending_svc_req->b_freeze_change && pt_svctx->b_freezed == FALSE && pt_svctx->e_signal_status == SIGNAL_STATUS_LOCKED && (!pt_svctx->b_is_blocked)
                    && x_strcmp(pt_svctx->pt_running_svc_req->pt_hdlr->s_name, pt_svctx->pt_pending_svc_req->pt_hdlr->s_name) == 0)
                {
                    SCC_VID_MODE_T e_crnt_vid_mode = SCC_VID_BLANK;
                    SCC_VID_SUPER_FREEZE_T t_super_freeze = {SCC_VID_SUPER_FREEZE_TYPE_FORCE, TRUE};

                    x_scc_vid_get_mode(pt_svctx->h_video_scc_comp, &e_crnt_vid_mode);
                    if (e_crnt_vid_mode != SCC_VID_BLANK)
                    {
                        svctx_set_video_plane(pt_svctx, SCC_VID_FREEZE);
                        x_scc_vid_set_force_unmute(pt_svctx->h_video_scc_comp, SCC_VID_FORCE_UNMUTE_TYPE_ATV_CHG_CH, TRUE);
                        x_scc_vid_set_super_freeze(pt_svctx->h_video_scc_comp, &t_super_freeze);
                        pt_svctx->b_freezed = TRUE;
                        x_dbg_stmt("[%s][%s][%d] freezed +++ +++ !!!!!!!!\r\n", pt_svctx->s_name, __FILE__, __LINE__);
                    }
                    else
                    {
                        pt_svctx->pt_pending_svc_req->b_freeze_change = FALSE;
                    }
                }
                else
                {
                    pt_svctx->pt_pending_svc_req->b_freeze_change = FALSE;
                    svctx_set_video_plane(pt_svctx, SCC_VID_BLANK);
                    svctx_ctrl_snow(pt_svctx, FALSE);
                }

#ifdef MW_TV_AV_BYPASS_SUPPORT
                if (pt_svctx->pt_running_svc_req->ui4_stream_mode & MAKE_BIT_MASK_32(ST_AUDIO)) /* DTV00139322 */
                {
                    
#ifdef MHEG5_HD_SUPPORT
                    if (!(pt_svctx->pt_pending_svc_req->e_src_type == SRC_TYPE_BRDCST_SVL && 
                        ((CH_SVC_REQ_T*)pt_svctx->pt_pending_svc_req)->t_src_info.b_ninja_change == TRUE ))
#endif
                    {
                        svctx_ctrl_main_audio(pt_svctx, TRUE);
                    }

                }
#endif
            }

            i4_ret = _svctx_stop_running_svc(pt_svctx, _STOP_AND_SELECT_SVC);
            if (i4_ret == SVCTXR_OK) {
                svctx_ntfy_svc_req_stopped(pt_svctx);
            }
        }
        break;
    case _SVCTX_MSG_STOP_SVC_REQ:
#ifdef MW_TV_AV_BYPASS_SUPPORT
        DBG_SVCTXMGR_MSG((
            "\r\n{SVCTX_MGR} Received _SVCTX_MSG_STOP_SVC_REQ (%s)\r\n\r\n",
            pt_svctx->s_name));
#endif
        if (svctx_is_owner(pt_svctx, pt_msg->h_svctx) == FALSE) {
            pt_svctx->h_svctx_owner = NULL_HANDLE;
        }

        if (pt_svctx->e_state == SVCTX_COND_READY) {

            /*x_dbg_stmt("[%s][%s][%d] stop ready\r\n", pt_svctx->s_name, __func__, __LINE__);*/
            
            _svctx_force_mute_video(pt_svctx, VIDEO_MUTE_CTRL_DISABLE);
            svctx_set_video_plane(pt_svctx, SCC_VID_BLANK);
            if (pt_msg->u.t_sync_stop_svc_req.pf_nfy != NULL) {
                pt_msg->u.t_sync_stop_svc_req.pf_nfy(
                    pt_msg->u.t_sync_stop_svc_req.pv_tag
                    );
            }
        } else {

            /*x_dbg_stmt("[%s][%s][%d] stop not ready\r\n", pt_svctx->s_name, __func__, __LINE__);*/
            
            pt_svctx->t_video_info.e_timing_type = VSH_SRC_TIMING_UNKNOWN;
            pt_svctx->e_av_status                = SVC_AV_STATUS_UNKNOWN;

            if (pt_svctx->pt_pending_svc_req != NULL) {
                svctx_free_svc_req(pt_svctx->pt_pending_svc_req);
                pt_svctx->pt_pending_svc_req = NULL;
            }

#ifdef MW_TV_AV_BYPASS_SUPPORT
            svctx_enable_slave_audio(pt_svctx, SLAVE_AUDIO_CTRL_DISABLE); /* disable slave audio */
#endif
#ifdef SYS_RECORD_SUPPORT
            if (!IS_SVCTX_RECORD (pt_svctx))
#endif
            {
                if ((NULL_HANDLE != pt_svctx->h_video_scc_comp) && pt_svctx->b_analog_signal_loss_show_snow && pt_svctx->b_enable_snow)
                {
                    svctx_ctrl_snow(pt_svctx, FALSE);
                }
            }
            
            if (pt_svctx->pt_running_svc_req == NULL) {
                DBG_ERROR((_ERROR_HEADER"%s(_SVCTX_MSG_STOP_SVC_REQ), pt_running_svc_req is NULL \r\n", __func__));
                break;
            }
            
            _svctx_force_mute_video(pt_svctx, VIDEO_MUTE_CTRL_DISABLE);
            if (pt_svctx->pt_running_svc_req->b_freeze_change == FALSE)
            {
                svctx_set_video_plane(pt_svctx, SCC_VID_BLANK);
            }
            
            pt_svctx->pt_running_svc_req->t_stop_cb.pf_nfy = pt_msg->u.t_sync_stop_svc_req.pf_nfy;
            pt_svctx->pt_running_svc_req->t_stop_cb.pv_tag = pt_msg->u.t_sync_stop_svc_req.pv_tag;

            /* free queued SVC_REQ */
            if (pt_svctx->pt_pending_svc_req != NULL) {
                svctx_free_svc_req(pt_svctx->pt_pending_svc_req);
                pt_svctx->pt_pending_svc_req = NULL;
            }

            i4_ret = _svctx_stop_running_svc(pt_svctx, _STOP_SVC_ONLY);
            if (i4_ret == SVCTXR_OK) {
                svctx_ntfy_svc_req_stopped(pt_svctx);
            }
        }
        break;
    case _SVCTX_MSG_UNBLOCK_SVC_REQ:       
        if (pt_svctx->pt_running_svc_req == NULL
                || pt_svctx->pt_running_svc_req->pt_hdlr == NULL
                || pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_unblock_svc_req == NULL) {
            break;
        }
        pt_svctx->pt_running_svc_req->b_force_unblock = pt_msg->u.t_unlock_svc_req.b_force;
        pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_unblock_svc_req(
                            pt_svctx
                            );
        break;
    case _SVCTX_MSG_SELECT_STRM_REQ:
        if (pt_svctx->pt_running_svc_req == NULL
                || pt_svctx->pt_running_svc_req->pt_hdlr == NULL
                || pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_select_strm_req == NULL) {
            break;
        }

        pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_select_strm_req(
                            pt_svctx,
                            pt_msg
                            );
        pt_svctx->ui4_stream_mode = pt_svctx->pt_running_svc_req->ui4_stream_mode;
        break;
    case _SVCTX_MSG_STOP_STRM_REQ:
        if (pt_svctx->pt_running_svc_req == NULL
                || pt_svctx->pt_running_svc_req->pt_hdlr == NULL
                || pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_stop_strm_req == NULL) {
            if (pt_msg->u.t_stop_strm_req.t_sync.pf_nfy != NULL) {
                pt_msg->u.t_stop_strm_req.t_sync.pf_nfy(pt_msg->u.t_stop_strm_req.t_sync.pv_tag);
            }
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_stop_strm_req(
                            pt_svctx,
                            pt_msg
                            );
        pt_svctx->ui4_stream_mode = pt_svctx->pt_running_svc_req->ui4_stream_mode;

        if (i4_ret != SVCTXR_PENDING) {
            if (pt_msg->u.t_stop_strm_req.t_sync.pf_nfy != NULL) {
                pt_msg->u.t_stop_strm_req.t_sync.pf_nfy(pt_msg->u.t_stop_strm_req.t_sync.pv_tag);
            }
        }
        break;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    case _SVCTX_MSG_QUEUED_SELECT_SVC_REQ:
        DBG_SVCTXMGR_MSG((
            "{SVCTX_MGR} Received _SVCTX_MSG_QUEUED_SELECT_SVC_REQ (%s)...\r\n",
            pt_svctx->s_name));
        i4_ret = _svctx_try_selecting_svc(pt_svctx);
        break;
#endif
    default:
        if (pt_svctx->pt_running_svc_req == NULL
                || pt_svctx->pt_running_svc_req->pt_hdlr == NULL
                || pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_internal_msg == NULL) {
            break;
        }
        if (pt_msg->ui1_msg_id == _SVCTX_MSG_STRM_NTFY) {
            svctx_cli_print_strm_msg(pt_svctx, pt_msg);
        }
        pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_internal_msg(
                            pt_svctx,
                            pt_msg
                            );
        break;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_dispatcher_fct
 *
 * Description
 * Input arguments
 * Output arguments
 *    None
 * Returns
 *    None
 *-----------------------------------------------------------------------------*/
static INT32 _svctx_dispatcher_fct(
                    VOID*                       pv_msg,
                    VOID*                       pv_tag
                    )
{
    SVCTX_MSG_T* pt_msg   = (SVCTX_MSG_T*) pv_msg;
    SVCTX_T*     pt_svctx = NULL;
    INT32        i4_ret   = SVCTXR_OK;

    ASSERT(pt_msg != NULL,
           ("internal error, activator pass wrong parameters!"));

    if (pt_msg->ui1_msg_id == _SVCTX_MSG_DESTROY_REQ) {
        if (pt_msg->u.t_destroy_req.pf_nfy != NULL) {
            pt_msg->u.t_destroy_req.pf_nfy(
                        pt_msg->u.t_destroy_req.pv_tag
                        );
        }
        DBG_INFO((_INFO_HEADER"fire ACTVTRR_TO_EXIT\r\n"));
        return ACTVTRR_TO_EXIT;
    }

    svctx_class_lock();
    do {
        if (pt_msg->pf_invoke_func != NULL) {
            pt_msg->pf_invoke_func(pt_msg->pv_tag, pt_msg);
            break;
        }

#ifdef MW_TV_AV_BYPASS_SUPPORT
        i4_ret = svctx_get_obj_with_ext_type(pt_msg->h_svctx, &pt_svctx);
#else
        i4_ret = svctx_get_obj(pt_msg->h_svctx, &pt_svctx);
#endif
        if (i4_ret != SVCTXR_OK) {
            if (pt_msg->ui1_msg_id == _SVCTX_MSG_SELECT_SVC_REQ) {
                /* free orphan service request */
                svctx_free_svc_req(pt_msg->u.pt_svc_req);
            }
            break;
        }

        if (pt_svctx->e_state >= SVCTX_COND_LAST_VALID_ENTRY) {
            DBG_INFO((_INFO_HEADER"%s(), wrong state = %d\r\n", __func__, pt_svctx->e_state));
            break;
        }

        _svctx_process_message(pt_svctx, pt_msg);
    } while (0);
    svctx_class_unlock();
    return ACTVTRR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_handle_autofree
 *
 * Description: -
 * Inputs:      -
 * Outputs:     -
 * Returns:     -
 *-----------------------------------------------------------------------------*/
static INT32 _svctx_handle_autofree (
                    HANDLE_T                    h_handle,
                    HANDLE_TYPE_T               e_type
                    )
{
    INT32 i4_ret;

    if (e_type < HT_GROUP_SVCTX || e_type >= SVCTXT_LAST_ENTRY) {
        return HR_INV_HANDLE_TYPE;
    }

    switch (e_type) {
    case SVCTXT_SVCTX:
#ifdef MW_TV_AV_BYPASS_SUPPORT
    case SVCTXT_SVCTX_ASSOCIATE:
#endif
        i4_ret = x_svctx_close(h_handle);
        break;
    case SVCTXT_VBI_BROKER:
        i4_ret = x_svctx_vbi_broker_close(h_handle);
        break;
    default:
        i4_ret = SVCTXR_INV_HANDLE;
        break;
    }

    return (i4_ret == SVCTXR_OK ? HR_OK : HR_INV_HANDLE);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_handle_parse_cb
 * Description
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
static HP_TYPE_T _svctx_handle_parse_cb (
                    UINT16                      ui2_count,
                    UINT16                      ui2_max_count,
                    HANDLE_T                    h_elem,
                    HANDLE_TYPE_T               e_type,
                    VOID*                       pv_obj,
                    VOID*                       pv_tag,
                    VOID*                       pv_parse_data
                    )
{
    HP_TYPE_T                e_ret       = HP_NEXT;
    SVCTX_LSTNR_NTFY_DATA_T* pt_nfy_data = (SVCTX_LSTNR_NTFY_DATA_T*)pv_parse_data;
    SVCTX_LSTNR_T*           pt_lstnr    = (SVCTX_LSTNR_T*)pv_tag;
    SVCTX_T*                 pt_svctx    = (SVCTX_T*)pv_obj;
    SVCTX_NTFY_CODE_T        e_code      = pt_nfy_data->e_code;

    if ((pt_svctx->h_svctx_owner != NULL_HANDLE && pt_svctx->h_svctx_owner == h_elem)
            || (pt_svctx->h_svctx_owner == NULL_HANDLE && pt_nfy_data->h_crnt_svctx == h_elem)) {
        if (pt_nfy_data->pf_select_nfy_fct == NULL) {
            /* this is owner, but no pf_select_nfy_fct, try its listener! */
            if (e_code == SVCTX_NTFY_CODE_SERVICE_CHANGING) {
                e_code = SVCTX_NTFY_CODE_OWN_SERVICE_CHANGING;
            }
        } else if (!(pt_lstnr != NULL
                        && pt_lstnr->pf_listener_fct != NULL
#ifndef LINUX_TURNKEY_SOLUTION
                        && pt_lstnr->pf_listener_fct != pt_nfy_data->pf_select_nfy_fct)){
#else
                        && (ri_svctx_same_select_listener_nfy_fct == NULL ?
                            pt_lstnr->pf_listener_fct != pt_nfy_data->pf_select_nfy_fct
                            : !ri_svctx_same_select_listener_nfy_fct(
                                    pt_nfy_data->pf_select_nfy_fct,
                                    pt_nfy_data->pv_select_nfy_tag,
                                    pt_lstnr->pf_listener_fct,
                                    pt_lstnr->pv_nfy_tag)))) {
#endif
            /* this is owner, and should be notified, so bypass it! */
            return HP_NEXT;
        }
    }

    if (pt_svctx->h_sys_svctx == h_elem) {
        /* this is sys_svctx, bypass it! */
        return HP_NEXT;
    }

    if (pt_lstnr != NULL && pt_lstnr->pf_listener_fct != NULL) {
        pt_lstnr->pf_listener_fct(
                    h_elem,
                    pt_nfy_data->e_nfy_cond,
                    e_code,
                    pt_nfy_data->e_stream_type,
                    pt_lstnr->pv_nfy_tag
                    );
    } else {
        /* this has no listener! */
    }

    return (e_ret);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_find_handler
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static BOOL _svctx_find_handler(
                    SRC_TYPE_T                  e_src_type,
                    SVCTX_HDLR_T**              ppt_hdlr
                    )
{
    SVCTX_HDLR_T* pt_hdlr = NULL;

    if (ppt_hdlr == NULL) {
        return FALSE;
    }

    DLIST_FOR_EACH(pt_hdlr, &t_g_handlers, t_dlink) {
        if (pt_hdlr->t_fcts.pf_is_handler == NULL) continue;
        if (pt_hdlr->t_fcts.pf_is_handler(e_src_type) == TRUE) {
            *ppt_hdlr = pt_hdlr;
            return TRUE;
        }
    }
    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_get
 *
 * Description: This API is used to get the attribuites of a service context.
 *
 * Inputs:  pt_svctx           References to a service context.
 *          e_get_type         References to the type of attributes to get.
 *          pv_get_info        References to the data information of the get
 *                             type.
 *          pui4_get_info_size References to the size of data information of
 *                             the get type.
 *
 * Outputs: pv_get_info        References to the data information of the get
 *                             type.
 *          pui4_get_info_size References to the size of data information of
 *                             the get type.
 *
 ----------------------------------------------------------------------------*/
static INT32 _svctx_get(
                    SVCTX_T*                        pt_svctx,           /* in  */
                    SVCTX_GET_TYPE_T                e_get_type,         /* in  */
                    VOID*                           pv_get_info,        /* in/out */
                    SIZE_T*                         pz_get_info_size    /* in/out */
                    )
{
    INT32 i4_ret = SVCTXR_OK;

    do {
        switch(e_get_type) {
        case SVCTX_COMMON_GET_ANA_AUTO_DETECTED_AUD_SYS:
            if ((*pz_get_info_size) != sizeof(SVCTX_ANA_AUTO_DETECTED_AUD_SYS_T) || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }
            *((SVCTX_ANA_AUTO_DETECTED_AUD_SYS_T*)pv_get_info) = pt_svctx->t_ana_auto_detected_aud_sys;
            break;
        case SVCTX_COMMON_GET_TUNER_NOTIFIED_FREQ:
            if ((*pz_get_info_size) != sizeof(UINT32) || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }
            *((UINT32*)pv_get_info) = pt_svctx->ui4_ana_freq;
            break;
        case SVCTX_COMMON_GET_SCREEN_STATUS:
            if ((*pz_get_info_size) != sizeof(SVCTX_SCREEN_STATUS_T) || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }
            *((SVCTX_SCREEN_STATUS_T*)pv_get_info) = pt_svctx->e_scrn_status;
            break;            
        case SVCTX_COMMON_GET_TYPE_COLOR_SYSTEM:
            if ((*pz_get_info_size) != sizeof(COLOR_SYS_T) || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }
            *((COLOR_SYS_T*)pv_get_info) = pt_svctx->t_clr_sys;
            break;
        case SVCTX_COMMON_GET_TYPE_NAME:
            if ((*pz_get_info_size) != SVCTX_NAME_MAX_SIZE || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }
            x_strncpy((CHAR*)pv_get_info, pt_svctx->s_name, SVCTX_NAME_MAX_SIZE);
            *pz_get_info_size = x_strlen((CHAR*)pv_get_info);
            break;
        case SVCTX_COMMON_GET_TYPE_SRC_NAME:
            if ((*pz_get_info_size) != SVCTX_SRC_NAME_MAX_SIZE || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }
            x_strncpy((CHAR*)pv_get_info, pt_svctx->s_src_grp_name, SVCTX_SRC_NAME_MAX_SIZE);
            *pz_get_info_size = x_strlen((CHAR*)pv_get_info);
            break;
        case SVCTX_COMMON_GET_TYPE_SNK_NAME:
            if ((*pz_get_info_size) != SVCTX_SNK_NAME_MAX_SIZE || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }
            x_strncpy((CHAR*)pv_get_info, pt_svctx->s_snk_grp_name, SVCTX_SNK_NAME_MAX_SIZE);
            *pz_get_info_size = x_strlen((CHAR*)pv_get_info);
            break;
        case SVCTX_COMMON_GET_VIDEO_MUTE:
            if (*pz_get_info_size != sizeof(SVCTX_VIDEO_MUTE_INFO_T) || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }

            *((SVCTX_VIDEO_MUTE_INFO_T*)pv_get_info) = pt_svctx->t_video_mute;
            break;
        case SVCTX_COMMON_GET_MTS:
            if (*pz_get_info_size != sizeof(SCC_AUD_MTS_T) || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }
            i4_ret = x_scc_aud_get_mts(
                            pt_svctx->h_audio_scc_comp,
                            (SCC_AUD_MTS_T*)pv_get_info
                            );
            i4_ret = (i4_ret == SMR_OK) ? SVCTXR_OK : SVCTXR_FAIL;
            break;
        case SVCTX_COMMON_GET_CC_AVIL_COND:
            if (*pz_get_info_size != sizeof(SVCTX_CC_AVIL_COND_T) || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }

            *((SVCTX_CC_AVIL_COND_T*)pv_get_info) = pt_svctx->e_cc_avil_cond;
            break;
        case SVCTX_COMMON_GET_TYPE_IS_BLOCK:
            if (*pz_get_info_size != sizeof(BOOL) || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }

            *((BOOL*)pv_get_info) = pt_svctx->b_is_blocked;
            break;
        case SVCTX_COMMON_GET_STREAM_MODE:
            if ((*pz_get_info_size) != sizeof(UINT32) || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }

            if (pt_svctx->pt_running_svc_req)
            {
                *((UINT32*)pv_get_info) = pt_svctx->ui4_stream_mode;
            }
            else
            {
                i4_ret = SVCTXR_WRONG_STATE;
            }
            break;
        case SVCTX_COMMON_GET_SIGNAL_STATUS:
            if ((*pz_get_info_size) != sizeof(SIGNAL_STATUS_T) || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }

            if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS
                    && pt_svctx->b_enable_block_bias_freq == TRUE
                    && pt_svctx->b_is_blocked == TRUE) {
                *((SIGNAL_STATUS_T*)pv_get_info) = SIGNAL_STATUS_LOCKED;
            } else {
                *((SIGNAL_STATUS_T*)pv_get_info) = pt_svctx->e_signal_status;
            }
            break;
        case SVCTX_COMMON_GET_AUTO_COLOR_COND:
            if ((*pz_get_info_size) != sizeof(AUTO_COLOR_COND_T) || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }
            *((AUTO_COLOR_COND_T*)pv_get_info) = pt_svctx->e_auto_color_cond;
            break;
        case SVCTX_COMMON_GET_AUTO_CLK_POS_PHS_COND:
            if ((*pz_get_info_size) != sizeof(AUTO_CLK_POS_PHS_COND_T) || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }
            *((AUTO_CLK_POS_PHS_COND_T*)pv_get_info) = pt_svctx->e_auto_clk_pos_phs_cond;
            break;
        case SVCTX_COMMON_GET_VIDEO_DECODE_STATUS:
            if ((*pz_get_info_size) != sizeof(VSH_DIG_DECODE_STATUS_T) || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }
            *((VSH_DIG_DECODE_STATUS_T*)pv_get_info) = pt_svctx->e_video_decode_status;
            break;
        case SVCTX_COMMON_GET_AUDIO_DECODE_STATUS:
            if ((*pz_get_info_size) != sizeof(ASH_AUDIO_DECODE_STATUS_T) || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }
            *((ASH_AUDIO_DECODE_STATUS_T*)pv_get_info) = pt_svctx->e_audio_decode_status;
            break;
        case SVCTX_COMMON_GET_DEFALUT_BLOCK_CHECK_FCT:
            if ((*pz_get_info_size) != sizeof(UINT32) || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }
#ifndef LINUX_TURNKEY_SOLUTION
            *((x_svctx_svc_block_check_fct*)pv_get_info) = pt_svctx->pf_block_check_fct;
#elif defined(SVCTX_SPLIT_SUPPORT)
            if (ri_svctx_get_block_check_fct_cb_addr == NULL)
            {
                /* no RPC. pf_block_check_fct is orig_cb_wrapper */
                *((x_svctx_svc_block_check_fct*)pv_get_info) = pt_svctx->pf_block_check_fct_tag;
            }
            else
            {
                *((x_svctx_svc_block_check_fct*)pv_get_info) =
                    ri_svctx_get_block_check_fct_cb_addr(
                            pt_svctx->pf_block_check_fct,
                            pt_svctx->pf_block_check_fct_tag);
            }
#else
            *((x_svctx_svc_block_check_fct*)pv_get_info) = pt_svctx->pf_block_check_fct;
#endif
            break;
        case SVCTX_COMMON_GET_DEFAULT_VIDEO_MODE:
            if ((*pz_get_info_size) != sizeof(SCC_VID_MODE_T) || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }
            *((SCC_VID_MODE_T*)pv_get_info) = pt_svctx->e_default_vid_mode;
            break;
        case SVCTX_COMMON_GET_SVC_REQ_SERIAL_NUM:
            if ((*pz_get_info_size) != sizeof(UINT32) || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }
            if (pt_svctx->pt_running_svc_req)
            {
                *((UINT32*) pv_get_info) = pt_svctx->pt_running_svc_req->ui4_serial_num;
            }
            else
            {
                i4_ret = SVCTXR_WRONG_STATE;
            }
            break;
        case SVCTX_COMMON_GET_SRC_TYPE:
            if ((*pz_get_info_size) != sizeof(SRC_TYPE_T) || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }
            if (pt_svctx->pt_running_svc_req)
            {
                *((SRC_TYPE_T*) pv_get_info) = pt_svctx->pt_running_svc_req->e_src_type;
            }
            else
            {
                i4_ret = SVCTXR_WRONG_STATE;
            }
            break;
#ifdef MW_TV_AV_BYPASS_SUPPORT
        case SVCTX_COMMON_GET_ENABLE_SVC_SELECT_WITH_ASSOCIATE:
            if ((*pz_get_info_size) != sizeof(BOOL) || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }
#ifdef MW_PIP_POP_SUPPORT
            *((BOOL*) pv_get_info) = (BOOL) (pt_svctx->ui4_assc_state & SVCTX_ASSC_DISABLED) ? FALSE : TRUE;
#else
            *((BOOL*) pv_get_info) = pt_svctx->b_assc_enabled;
#endif
            break;

        case SVCTX_COMMON_GET_COPY_PROTECTION:
            if ((*pz_get_info_size) != sizeof(SVCTX_COPY_PROTECTION_T) || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }
            *((SVCTX_COPY_PROTECTION_T*) pv_get_info) = pt_svctx->t_cp_mask;
            break;

        case SVCTX_COMMON_GET_PRIORITY:
            if ((*pz_get_info_size) != sizeof(INT8) || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }
            *((INT8*) pv_get_info) = pt_svctx->i1_prio_diff;
            break;
#endif
#ifdef SYS_MHP_SUPPORT
        case SVCTX_COMMON_GET_AV_STATUS:
            if ((*pz_get_info_size) != sizeof(SVC_AV_STATUS_T) || pv_get_info == NULL) {
                i4_ret = SVCTXR_INV_GET_INFO;
                break;
            }
            *((SVC_AV_STATUS_T*) pv_get_info) = pt_svctx->e_av_status;
            break;
#endif /* SYS_MHP_SUPPORT*/

        default:
            i4_ret = SVCTXR_CONTINUE;
            break;
        }

        if (i4_ret != SVCTXR_CONTINUE) {
            break;
        }

       
        if (pt_svctx->pt_running_svc_req == NULL) 
        {
            SVCTX_HDLR_T* pt_hdlr = NULL;
        
            i4_ret = SVCTXR_WRONG_STATE;
        
            DLIST_FOR_EACH(pt_hdlr, &t_g_handlers, t_dlink)
            {
                if (pt_hdlr->t_fcts.pf_get == NULL) 
                    continue;
                i4_ret = pt_hdlr->t_fcts.pf_get(
                            pt_svctx,
                            e_get_type,
                            pv_get_info,
                            pz_get_info_size
                            );
                if (i4_ret == SVCTXR_OK) 
                {
            break;
        }
             }
             break;
        }
       

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get(
                        pt_svctx,
                        e_get_type,
                        pv_get_info,
                        pz_get_info_size
                        );
    } while (0);

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_get_video_res_mask
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
static UINT32 _svctx_get_video_res_mask(
                    const VSH_SRC_RESOLUTION_INFO_T*    pt_resolution
                    )
{
    UINT32 ui4_height = 0;

    if (pt_resolution == NULL) {
        return VIDEO_RES_MASK_NONE;
    }

    x_svctx_get_well_defined_video_heigh(pt_resolution->ui4_height, &ui4_height);

    switch(ui4_height) {
    case VIDEO_HEIGHT_1080:
        return (pt_resolution->b_is_progressive == TRUE ? VIDEO_RES_MASK_1080P : VIDEO_RES_MASK_1080I);
    case VIDEO_HEIGHT_720:
        return (pt_resolution->b_is_progressive == TRUE ? VIDEO_RES_MASK_720P : VIDEO_RES_MASK_720I);
    case VIDEO_HEIGHT_576:
        return (pt_resolution->b_is_progressive == TRUE ? VIDEO_RES_MASK_576P : VIDEO_RES_MASK_576I);
    case VIDEO_HEIGHT_480:
        return (pt_resolution->b_is_progressive == TRUE ? VIDEO_RES_MASK_480P : VIDEO_RES_MASK_480I);
    default:
        break;
    }
    return VIDEO_RES_MASK_NONE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_invoke_listener_nfy_fct
 * Description
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
static VOID _svctx_invoke_listener_nfy_fct(
                    SVCTX_T*                        pt_svctx,
                    SVCTX_NTFY_CODE_T               e_code,
                    STREAM_TYPE_T                   e_stream_type,
                    HANDLE_T                        h_crnt_svctx,
                    x_svctx_select_nfy_fct          pf_crnt_select_nfy_fct
#ifdef LINUX_TURNKEY_SOLUTION
                    ,
                    VOID*                           pv_crnt_select_nfy_tag
#endif
                    )
{
    SVCTX_LSTNR_NTFY_DATA_T t_nfy_data;

#ifdef MW_RIKS_TV_SUPPORT
    if (svctx_rikstv_in_tuning_state(pt_svctx))
    {
        /* suppress notify when alter freq tuning, for DTV00073997 */
        switch (e_code)
        {
            case SVCTX_NTFY_CODE_START_ALTER_FREQ_TUNE:
            case SVCTX_NTFY_CODE_END_ALTER_FREQ_TUNE:
            case SVCTX_NTFY_CODE_START_FIRST_TUNE:
            case SVCTX_NTFY_CODE_END_FIRST_TUNE:
            case SVCTX_NTFY_CODE_START_FREQ_LIST_UPDATED_CHK:
            case SVCTX_NTFY_CODE_END_FREQ_LIST_UPDATED_CHK:
                break;

            default:
                return;
        }
    }
#endif

    t_nfy_data.e_nfy_cond        = pt_svctx->e_state;
    t_nfy_data.e_code            = e_code;
    t_nfy_data.e_stream_type     = e_stream_type;
    t_nfy_data.h_crnt_svctx      = h_crnt_svctx;
    t_nfy_data.pf_select_nfy_fct = pf_crnt_select_nfy_fct;
#ifdef LINUX_TURNKEY_SOLUTION
    t_nfy_data.pv_select_nfy_tag = pv_crnt_select_nfy_tag;
#endif

    handle_parse(&(pt_svctx->t_hdl_link), _svctx_handle_parse_cb, &t_nfy_data);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_invoke_owner_svc_nfy_fct
 * Description
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
static VOID _svctx_invoke_owner_svc_nfy_fct(
                    SVCTX_T*                    pt_svctx,
                    SVCTX_NTFY_CODE_T           e_code,
                    STREAM_TYPE_T               e_stream_type
                    )
{
    if (SVCTX_NTFY_CODE_SERVICE_CHANGING == e_code) {
        e_code = SVCTX_NTFY_CODE_OWN_SERVICE_CHANGING;
    }

    if (pt_svctx->pt_running_svc_req == NULL) {
        return;
    }

    if (pt_svctx->pt_running_svc_req->pf_svc_nfy == NULL) {
        return;
    }

    if (SVCTX_NTFY_CODE_INTERNAL_ERROR == e_code) {
        DBG_INFO((_INFO_HEADER"got INTERNAL_ERROR!\r\n"));
    }

    pt_svctx->pt_running_svc_req->pf_svc_nfy(
                    pt_svctx->pt_running_svc_req->h_svctx,
                    pt_svctx->e_state,
                    e_code,
                    e_stream_type,
                    pt_svctx->pt_running_svc_req->pv_svc_nfy_tag
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_is_duplicated_nfy
 * Description
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
static BOOL _svctx_is_duplicated_nfy(
                    SVCTX_T*                    pt_svctx,
                    SVCTX_NTFY_CODE_T           e_code,
                    STREAM_TYPE_T               e_stream_type
                    )
{
    /* Check service type message only */
    if (e_stream_type == ST_UNKNOWN)
    {
        if ((e_code >= SVCTX_NTFY_CODE_NO_AUDIO_VIDEO_SVC) &&
             (e_code <= SVCTX_NTFY_CODE_SCRAMBLED_VIDEO_NO_AUDIO_SVC))
        {
            if (pt_svctx->t_crnt_nfy_msg.e_svc_code == e_code)
            {
                return TRUE;
            }

            pt_svctx->t_crnt_nfy_msg.e_svc_code = e_code;
        }
    }

    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_handle_no_signal_delay_chk
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _svctx_handle_no_signal_delay_chk(
                    VOID*                   pv_tag,
                    SVCTX_MSG_T*            pt_msg
                    )
{
    SVCTX_T* pt_svctx = (SVCTX_T*)pv_tag;

    if (pt_svctx == NULL) {
        return SVCTXR_OK;
    }

    if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) {
        pt_svctx->b_delay_video_mute_check = TRUE;
        if (pt_svctx->pf_video_mute_nfy_fct != NULL) {
            DBG_INFO((_INFO_HEADER"%s() ready to send no signal notifciation\r\n", __func__));
            pt_svctx->pf_video_mute_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_SIGNAL_LOSS,
                    ST_UNKNOWN
                    );
        }
        pt_svctx->b_delay_video_mute_check = FALSE;
    }

    /* reset to orignal default value */
    pt_svctx->ui4_video_mute_chk_delay = DEFAULT_SIGNAL_LOSS_CHK_TIMEOUT;

    return  SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_delay_signal_loss_nfy_timeout_cb
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static VOID _svctx_delay_signal_loss_nfy_timeout_cb(
                    HANDLE_T                    h_timer,
                    VOID*                       pv_tag
                    )
{
    SVCTX_MSG_T t_msg    = {0};
    SVCTX_T*    pt_svctx = (SVCTX_T*)pv_tag;

    if (pt_svctx == NULL) {
        return;
    }

    t_msg.pf_invoke_func = _svctx_handle_no_signal_delay_chk;
    t_msg.pv_tag         = pv_tag;

    svctx_send_msg(pt_svctx->h_sys_svctx, &t_msg);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_handle_no_video_delay_chk
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _svctx_handle_no_video_delay_chk(
                    VOID*                   pv_tag,
                    SVCTX_MSG_T*            pt_msg
                    )
{
    SVCTX_T* pt_svctx = (SVCTX_T*)pv_tag;

    if (pt_svctx == NULL) {
        return SVCTXR_OK;
    }

    if (pt_svctx->e_video_mute_code == SVCTX_NTFY_CODE_AUDIO_VIDEO_SVC) {
        return SVCTXR_OK;
    }

    pt_svctx->b_delay_video_mute_check = TRUE;
    if (pt_svctx->pf_video_mute_nfy_fct != NULL) {
        DBG_INFO((_INFO_HEADER"%s() send no video related notifciation\r\n", __func__));
        pt_svctx->pf_video_mute_nfy_fct(
                        pt_svctx,
                        pt_svctx->e_video_mute_code,
                        ST_UNKNOWN
                        );
    }
    pt_svctx->b_delay_video_mute_check = FALSE;
    pt_svctx->e_video_mute_code        = SVCTX_NTFY_CODE_AUDIO_VIDEO_SVC;

    return  SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_delay_no_video_related_nfy_timeout_cb
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static VOID _svctx_delay_no_video_related_nfy_timeout_cb(
                    HANDLE_T                    h_timer,
                    VOID*                       pv_tag
                    )
{
    SVCTX_MSG_T t_msg    = {0};
    SVCTX_T*    pt_svctx = (SVCTX_T*)pv_tag;

    if (pt_svctx == NULL) {
        return;
    }

    t_msg.pf_invoke_func = _svctx_handle_no_video_delay_chk;
    t_msg.pv_tag         = pv_tag;

    svctx_send_msg(pt_svctx->h_sys_svctx, &t_msg);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_handle_auto_release_ownership
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _svctx_handle_auto_release_ownership(
                    VOID*                   pv_tag,
                    SVCTX_MSG_T*            pt_msg
                    )
{
    SVCTX_T* pt_svctx = (SVCTX_T*)pv_tag;

    if (pt_svctx == NULL) {
        return SVCTXR_OK;
    }

    pt_svctx->h_svctx_owner = NULL_HANDLE;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_auto_release_ownership_cb
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static VOID _svctx_auto_release_ownership_cb(
                    HANDLE_T                    h_timer,
                    VOID*                       pv_tag
                    )
{
    SVCTX_MSG_T t_msg    = {0};
    SVCTX_T*    pt_svctx = (SVCTX_T*)pv_tag;

    if (pt_svctx == NULL) {
        return;
    }

    t_msg.pf_invoke_func = _svctx_handle_auto_release_ownership;
    t_msg.pv_tag         = pv_tag;

    svctx_send_msg(pt_svctx->h_sys_svctx, &t_msg);
}

#ifdef MW_TV_AV_BYPASS_SUPPORT
static INT32 _svctx_config_associate(
    SVCTX_T*                pt_svctx_lead,
    SVCTX_T*                pt_svctx_assc,
    const SVCTX_ASSC_CFG_T* pt_assc_cfg)
{
    pt_svctx_assc->i1_prio_diff  = pt_assc_cfg->i1_prio_diff;
    pt_svctx_assc->ui4_attribute = (pt_assc_cfg->ui4_attribute | SVCTX_ASSC_ATTR_ASSOCIATE);

    if (pt_svctx_assc->ui4_attribute & SVCTX_ASSC_ATTR_USE_SAME_STRM_FILTER)
    {
        x_memcpy(
            pt_svctx_assc->pt_def_strm_fltr_fcts,
            pt_svctx_lead->pt_def_strm_fltr_fcts,
            sizeof(pt_svctx_lead->pt_def_strm_fltr_fcts));

        x_memcpy(
            pt_svctx_assc->pv_def_strm_fltr_tags,
            pt_svctx_lead->pv_def_strm_fltr_tags,
            sizeof(pt_svctx_lead->pv_def_strm_fltr_tags));
    }

    if (pt_svctx_assc->ui4_attribute & SVCTX_ASSC_ATTR_USE_SAME_BLOCK_CHECKER)
    {
        pt_svctx_assc->pf_block_check_fct = pt_svctx_lead->pf_block_check_fct;
#ifdef SVCTX_SPLIT_SUPPORT
        pt_svctx_assc->pf_block_check_fct_tag   = pt_svctx_lead->pf_block_check_fct_tag;
#endif
    }

    return SVCTXR_OK;
}

#ifdef SYS_RECORD_SUPPORT
static VOID _svctx_set_src_desc (
    const SVCTX_T*      pt_svctx_worker,
    const SRC_DESC_T*   pt_src_desc_lead,
    SRC_REC_INFO_T*     pt_rec_info,
    SRC_DESC_T*         pt_src_desc_eff )
{
    if (IS_SVCTX_RECORD (pt_svctx_worker))
    {
        x_memset (pt_rec_info, 0, sizeof (SRC_REC_INFO_T));
        pt_rec_info->e_src_rec = pt_src_desc_lead->e_type;
        pt_rec_info->pv_detail = pt_src_desc_lead->pv_details;

        x_memset (pt_src_desc_eff, 0, sizeof (SRC_DESC_T));
        pt_src_desc_eff->ps_conn_src_name = pt_src_desc_lead->ps_conn_src_name;
        pt_src_desc_eff->e_type     = SRC_TYPE_RECORD;
        pt_src_desc_eff->pv_details = (VOID *) pt_rec_info;
        pt_src_desc_eff->t_riks_tv  = pt_src_desc_lead->t_riks_tv;
    }
    else
    {
        *pt_src_desc_eff = *pt_src_desc_lead;
    }
}
#else
static VOID _svctx_set_src_desc (
    const SVCTX_T*      pt_svctx_worker,
    const SRC_DESC_T*   pt_src_desc_lead,
    SRC_DESC_T*         pt_src_desc_eff )
{
    *pt_src_desc_eff = *pt_src_desc_lead;
}
#endif

static INT32 _svctx_set_snk_desc(
    const SVCTX_T*      pt_svctx,
    const SNK_DESC_T*   pt_snk_desc_lead,
    UINT32              ui4_strm_mode_lead,
    const SVCTX_T*      pt_svctx_worker,
    SNK_DESC_T*         pt_snk_desc_space,
    SNK_DESC_T**        ppt_snk_desc_eff,
    UINT32*             pui4_eff_strm_mode)
{
    INT32               i4_ret;

    if (!IS_SVCTX_ASSOCIATE(pt_svctx) && (pt_svctx_worker == pt_svctx))
    {
        i4_ret = SVCTXR_OK;

        if (pt_snk_desc_lead)
        {
            *ppt_snk_desc_eff  = pt_snk_desc_space;
            **ppt_snk_desc_eff = *pt_snk_desc_lead;
        }
        else
        {
            *ppt_snk_desc_eff = NULL;
        }

        *pui4_eff_strm_mode = ui4_strm_mode_lead;
    }
    else
    {
        const SVCTX_T*      pt_svctx_assc       = pt_svctx_worker;
        SNK_DESC_T*         pt_snk_desc_assc    = pt_snk_desc_space;
        UINT32*             pui4_assc_strm_mode = pui4_eff_strm_mode;
        SVCTX_ASSC_CFG_T*   pt_assc_cfg;

        i4_ret = SVCTXR_NOT_EXIST;

        pt_assc_cfg = svctx_mgr_get_assc_cfg(pt_svctx_assc);

        if (pt_assc_cfg)
        {
            pt_snk_desc_assc->ps_snk_grp_name        = (CHAR*) pt_assc_cfg->s_snk_grp_name;
            pt_snk_desc_assc->ps_snk_grp_name_aux    = NULL;

            pt_snk_desc_assc->pt_video_region        = NULL;
            pt_snk_desc_assc->pt_disp_region         = NULL;
            pt_snk_desc_assc->pt_video_plane_order   = NULL;
            pt_snk_desc_assc->pt_video_strm_fltr_fct = (pt_snk_desc_lead) ? pt_snk_desc_lead->pt_video_strm_fltr_fct : NULL;
            pt_snk_desc_assc->pv_video_strm_fltr_tag = pt_assc_cfg->pv_video_strm_fltr_tag;

            pt_snk_desc_assc->pt_volume_info         = NULL;
            pt_snk_desc_assc->pt_audio_strm_fltr_fct = (pt_snk_desc_lead) ? pt_snk_desc_lead->pt_audio_strm_fltr_fct : NULL;
            pt_snk_desc_assc->pv_audio_strm_fltr_tag = pt_assc_cfg->pv_audio_strm_fltr_tag;

            pt_snk_desc_assc->h_cc_gl_plane          = NULL_HANDLE;
            pt_snk_desc_assc->pt_cc_filter           = NULL;
            pt_snk_desc_assc->pt_cc_strm_fltr_fct    = NULL;
            pt_snk_desc_assc->pv_cc_strm_fltr_tag    = NULL;

        #if 0
            pt_snk_desc_assc->e_text_strm_type = pt_snk_desc_lead->e_text_strm_type;

            switch (pt_snk_desc_assc->e_text_strm_type)
            {
                case ST_CLOSED_CAPTION:
                    pt_snk_desc_assc->u_text_strm.t_cc.h_gl_plane  = NULL_HANDLE;
                    pt_snk_desc_assc->u_text_strm.t_cc.pt_filter   = NULL;
                    pt_snk_desc_assc->u_text_strm.t_cc.pt_fltr_fct = NULL;
                    pt_snk_desc_assc->u_text_strm.t_cc.pv_fltr_tag = NULL;
                    break;

                case ST_TELETEXT:
                    pt_snk_desc_assc->u_text_strm.t_subtitle.h_gl_plane  = NULL_HANDLE;
                    pt_snk_desc_assc->u_text_strm.t_subtitle.pt_fltr_fct = NULL;
                    pt_snk_desc_assc->u_text_strm.t_subtitle.pv_fltr_tag = NULL;
                    break;

                case ST_SUBTITLE:
                    pt_snk_desc_assc->u_text_strm.t_telex.h_gl_plane  = NULL_HANDLE;
                    pt_snk_desc_assc->u_text_strm.t_telex.pt_fltr_fct = NULL;
                    pt_snk_desc_assc->u_text_strm.t_telex.pv_fltr_tag = NULL;
                    break;

                default:
                    break;
            }
        #else
            pt_snk_desc_assc->e_text_strm_type = ST_UNKNOWN;
        #endif

            *ppt_snk_desc_eff    = pt_snk_desc_assc;
            *pui4_assc_strm_mode = (pt_assc_cfg->ui4_stream_mode) & (ST_MASK_VIDEO | ST_MASK_AUDIO);

            i4_ret = SVCTXR_OK;
        }
    }

    return (i4_ret);
}

#if 0
static BOOL _svctx_valid_to_select_svc(
    SRC_TYPE_T      e_src_type,
    const SVCTX_T*  pt_svctx)
{
    BOOL                    b_ok = TRUE;
    UINT32                  ui4_cnt;
    ACTIVE_LINK_ENTRY_T*    pt_link = NULL;

    if (IS_SVCTX_ASSOCIATE(pt_svctx))
    {
        b_ok = FALSE;

        if (TRUE == svctx_mgr_be_associate_on(e_src_type, pt_svctx))
        {
            ui4_cnt = svctx_mgr_get_link_num(ACTIVE_LINK_NOW, e_src_type);

            if ((0 == ui4_cnt))
            {
                b_ok = TRUE;
            }
            else if ((pt_link = svctx_mgr_get_link_obj(ACTIVE_LINK_NOW, e_src_type, 0)) != NULL)
            {
                if ((1 == ui4_cnt) &&
                    (pt_svctx == pt_link->pt_worker))
                {
                    b_ok = TRUE;
                }
            }
        }
    }

    return (b_ok);
}
#endif

static BOOL _svctx_is_type_supported(
    SVCTX_T*        pt_obj,
    STREAM_TYPE_T   e_type)
{
    BOOL                b_support = FALSE;
    SVCTX_ASSC_CFG_T*   pt_assc_cfg;
    UINT32              ui4_stream_mode;

    pt_assc_cfg = svctx_mgr_get_assc_cfg(pt_obj);

    if (pt_assc_cfg)
    {
        ui4_stream_mode = pt_assc_cfg->ui4_stream_mode;

        switch (e_type)
        {
            case ST_VIDEO:
                if (ui4_stream_mode & ST_MASK_VIDEO)
                {
                    b_support = TRUE;
                }
                break;
            case ST_AUDIO:
                if (ui4_stream_mode & ST_MASK_AUDIO)
                {
                    b_support = TRUE;
                }
                break;
            case ST_CLOSED_CAPTION:
                if (ui4_stream_mode & ST_MASK_CLOSED_CAPTION)
                {
                    b_support = TRUE;
                }
                break;
#ifdef MW_ISDB_SUPPORT
            case ST_ISDB_CAPTION:
                if (ui4_stream_mode & ST_MASK_ISDB_CAPTION)
                {
                    b_support = TRUE;
                }
                break;
            case ST_ISDB_TEXT:
                if (ui4_stream_mode & ST_MASK_ISDB_TEXT)
                {
                    b_support = TRUE;
                }
                break;
#endif
            case ST_TELETEXT:
                if (ui4_stream_mode & ST_MASK_TELETEXT)
                {
                    b_support = TRUE;
                }
                break;
            case ST_SUBTITLE:
                if (ui4_stream_mode & ST_MASK_SUBTITLE)
                {
                    b_support = TRUE;
                }
                break;
            case ST_DATA:
                if (ui4_stream_mode & ST_MASK_DATA)
                {
                    b_support = TRUE;
                }
                break;
            case ST_BYPASS:
                if (ui4_stream_mode & ST_MASK_BYPASS)
                {
                    b_support = TRUE;
                }
                break;

            default:
                break;
        }
    }

    return (b_support);
}

static VOID _svctx_set_shared_block_checker(
    SVCTX_T*                    pt_svctx,
#ifdef SVCTX_SPLIT_SUPPORT
    svctx_svc_block_check_fct   pf_block_check_fct,
    VOID*                       pv_tag
#else
    x_svctx_svc_block_check_fct pf_block_check_fct
#endif
    )
{
    SVCTX_T* pt_obj;

    DLIST_FOR_EACH(pt_obj, &t_g_using_svctx_q, t_dlink)
    {
        if (pt_svctx != pt_obj &&
            svctx_mgr_be_associated(pt_svctx, pt_obj) &&
            (pt_obj->ui4_attribute & SVCTX_ASSC_ATTR_USE_SAME_BLOCK_CHECKER))
        {
            pt_obj->pf_block_check_fct = pf_block_check_fct;
#ifdef SVCTX_SPLIT_SUPPORT
            pt_obj->pf_block_check_fct_tag = pv_tag;
#endif
        }
    }
}

static VOID _svctx_set_shared_stream_mode(
    SVCTX_T*                    pt_svctx,
    UINT32                      ui4_stream_mode)
{
    SVCTX_T* pt_obj;

    DLIST_FOR_EACH(pt_obj, &t_g_using_svctx_q, t_dlink)
    {
        if (pt_svctx != pt_obj &&
            svctx_mgr_be_associated(pt_svctx, pt_obj))
        {
            pt_obj->ui4_stream_mode = ui4_stream_mode;
            if (pt_obj->i1_prio_diff > pt_svctx->i1_prio_diff)
            {
                pt_obj->ui4_stream_mode &= ~MAKE_BIT_MASK_32(ST_AUDIO);
            }
        }
    }
}

static VOID _svctx_set_shared_strm_filter(
    SVCTX_T*                        pt_svctx,
    STREAM_TYPE_T                   e_strm_type,
    x_svctx_strm_comp_filter_fct    pt_filter_fct,
    VOID*                           pv_filter_tag)
{
    SVCTX_T* pt_obj;

    if ((e_strm_type == ST_UNKNOWN) || (e_strm_type >= STRM_TYPE_COUNT)) {
        return;
    }

    DLIST_FOR_EACH(pt_obj, &t_g_using_svctx_q, t_dlink)
    {
        if (pt_svctx != pt_obj &&
            svctx_mgr_be_associated(pt_svctx, pt_obj) &&
            (pt_obj->ui4_attribute & SVCTX_ASSC_ATTR_USE_SAME_STRM_FILTER))
        {
            pt_obj->pt_def_strm_fltr_fcts[e_strm_type] = pt_svctx->pt_def_strm_fltr_fcts[e_strm_type];
            pt_obj->pv_def_strm_fltr_tags[e_strm_type] = pt_svctx->pv_def_strm_fltr_tags[e_strm_type];
        }
    }
}

static BOOL _svctx_is_able_to_ctrl_slave_audio(
    SVCTX_T*    pt_svctx)
{
    BOOL            b_ctrl_slave_audio = FALSE;
    SVCTX_T*        pt_svctx_assc = NULL;
    LINK_POS_T      t_pos = NULL;

    if (IS_SVCTX_ASSOCIATE(pt_svctx))
    {
#ifdef MW_PIP_POP_SUPPORT
        if (IS_SVCTX_EXECUTABLE(pt_svctx->ui4_assc_state) &&
#else
        if (pt_svctx->b_assc_enabled &&
#endif
            (pt_svctx->ui4_attribute & SVCTX_ASSC_ATTR_AUDIO_SLAVE))
        {
            b_ctrl_slave_audio = TRUE;
        }
    }
    else
    {
        while (1)
        {
            if (pt_svctx->pt_running_svc_req == NULL)
            {
                break;
            }

            pt_svctx_assc = svctx_mgr_get_assc_obj_by_lead(
                pt_svctx,
                pt_svctx->pt_running_svc_req->e_src_type,
                &t_pos);

            if (pt_svctx_assc)
            {
#ifdef MW_PIP_POP_SUPPORT
                if (IS_SVCTX_EXECUTABLE(pt_svctx_assc->ui4_assc_state) &&
#else
                if (pt_svctx_assc->b_assc_enabled &&
#endif
                    (pt_svctx_assc->ui4_attribute & SVCTX_ASSC_ATTR_AUDIO_SLAVE))
                {
                    b_ctrl_slave_audio = TRUE;
                }
            }
            else
            {
                break;
            }
        }
    }

    return (b_ctrl_slave_audio);
}


#ifdef MW_PIP_POP_SUPPORT
static VOID _svctx_a_tricky_method_remove_pre_custom_audio(SRC_TYPE_T e_type)
{
    if (e_type == (SRC_TYPE_T)4)
    {
        SVCTX_T* pt_obj;

        DLIST_FOR_EACH(pt_obj, &t_g_using_svctx_q, t_dlink)
        {
            if(!svctx_is_pap(pt_obj))
            {
                pt_obj->ui4_prv_stream_cond = 0;
            }
        }
    }
}
#endif

static VOID _svctx_stop_disabled_n_non_associated_svc(
    SRC_TYPE_T                      e_type)
{
    ACTIVE_LINK_ENTRY_T*    pt_link;
    SVCTX_T*                pt_obj;
    INT32                   i = 0;

    while (1)
    {
        pt_link = svctx_mgr_get_link_obj_all(
            ACTIVE_LINK_NOW,
            i++);

        if (!pt_link)
        {
            break;
        }

        pt_obj = pt_link->pt_worker;

        if (IS_SVCTX_ASSOCIATE(pt_obj) &&
#ifdef MW_PIP_POP_SUPPORT
            IS_SVCTX_DISABLED(pt_obj->ui4_assc_state) &&
#else
            !pt_obj->b_assc_enabled &&
#endif
            pt_obj->pt_running_svc_req != NULL &&
            pt_obj->pt_running_svc_req->e_src_type != e_type)
        {
            x_svctx_stop_svc(pt_obj->h_sys_svctx);
            b_g_disable_reuse_pipe = TRUE;
        }
    }
}
#endif

/*-----------------------------------------------------------------------------
 * SVCTX public methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_translate_scdbr_2_svctxr
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
INT32 svctx_translate_scdbr_2_svctxr(
                    INT32                       i4_scdb_ret
                    )
{
    INT32 i4_svctx_ret;

    if (i4_scdb_ret >= SCDBR_OK) {
        return SVCTXR_OK;
    }

    switch(i4_scdb_ret) {
    case SCDBR_INV_ARG:       i4_svctx_ret = SVCTXR_INV_ARG;     break;
    case SCDBR_INV_HANDLE:    i4_svctx_ret = SVCTXR_INV_HANDLE;  break;
    case SCDBR_OUT_OF_HANDLE: i4_svctx_ret = SVCTXR_NO_RESOURCE; break;
    case SCDBR_OUT_OF_MEM:    i4_svctx_ret = SVCTXR_NO_RESOURCE; break;
    case SCDBR_NOT_FOUND:     i4_svctx_ret = SVCTXR_NOT_EXIST;   break;
    default:                  i4_svctx_ret = SVCTXR_FAIL;        break;
    }

    return i4_svctx_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_translate_smr_2_svctxr
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
INT32 svctx_translate_smr_2_svctxr(
                    INT32                       i4_sm_ret
                    )
{
    INT32 i4_svctx_ret;

    if (i4_sm_ret >= SMR_OK) {
        return SVCTXR_OK;
    }

    switch(i4_sm_ret) {
    case SMR_INV_ARG:             i4_svctx_ret = SVCTXR_INV_ARG;     break;
    case SMR_INV_HANDLE:          i4_svctx_ret = SVCTXR_INV_HANDLE;  break;
    case SMR_OUT_OF_HANDLES:      i4_svctx_ret = SVCTXR_NO_RESOURCE; break;
    case SMR_INSUFFICIENT_MEMORY: i4_svctx_ret = SVCTXR_NO_RESOURCE; break;
    case SMR_NOT_SUPPORT:         i4_svctx_ret = SVCTXR_NOT_SUPPORT; break;
    case SMR_INTERNAL_ERROR:      i4_svctx_ret = SVCTXR_FAIL;        break;
    default:                      i4_svctx_ret = SVCTXR_FAIL;        break;
    }

    return i4_svctx_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_translate_cmr_2_svctxr
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
INT32 svctx_translate_cmr_2_svctxr(
                    INT32                       i4_cm_ret
                    )
{
    INT32 i4_svctx_ret;

    switch(i4_cm_ret) {
    case CMR_ASYNC_NFY:      i4_svctx_ret = SVCTXR_ASYNC_NFY;   break;
    case CMR_OK:             i4_svctx_ret = SVCTXR_OK;          break;
    case CMR_INV_ARG:        i4_svctx_ret = SVCTXR_INV_ARG;     break;
    case CMR_INV_HANDLE:     i4_svctx_ret = SVCTXR_INV_HANDLE;  break;
    case CMR_OUT_OF_HANDLES: i4_svctx_ret = SVCTXR_NO_RESOURCE; break;
    case CMR_NOT_ENOUGH_MEM: i4_svctx_ret = SVCTXR_NO_RESOURCE; break;
    default:                 i4_svctx_ret = (i4_cm_ret >= CMR_OK) ? SVCTXR_OK : SVCTXR_FAIL; break;
    }
    return i4_svctx_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_timer_start
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 svctx_timer_start(
                    SVCTX_T*                    pt_svctx,
                    UINT32                      ui4_timeout,
                    x_os_timer_cb_fct           pf_invoke_fct,
                    VOID*                       pv_tag
                    )
{
    INT32 i4_ret;

    if (pt_svctx == NULL || ui4_timeout == 0) {
        return SVCTXR_INV_ARG;
    }

    i4_ret = x_timer_start(
                    pt_svctx->h_timer,
                    ui4_timeout,
                    X_TIMER_FLAG_ONCE,
                    pf_invoke_fct,
                    (VOID*)pv_tag
                    );
    return (i4_ret == OSR_OK ? SVCTXR_OK : SVCTXR_FAIL);
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_timer_stop
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 svctx_timer_stop(
                    SVCTX_T*                    pt_svctx
                    )
{
    INT32 i4_ret;

    if (pt_svctx == NULL) {
        return SVCTXR_INV_ARG;
    }

    i4_ret = x_timer_stop(pt_svctx->h_timer);

    return (i4_ret == OSR_OK ? SVCTXR_OK : SVCTXR_FAIL);
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_scdb_chk_timer_start
 * Description
 *      This API is used to check SCDB relative staff after a timeout.
 *      The actual check is performed in pf_invoke_fct
 * Input arguments
 *      pt_svctx
 *      ui4_timeout
 *      pf_invoke_fct
 *      pv_tag
 * Output arguments
 *      none
 * Returns:
 *      SVCTXR_OK          Routine successful.
 *      SVCTXR_FAIL        Routine failed
 *-----------------------------------------------------------------------------*/
INT32 svctx_scdb_chk_timer_start(
                    SVCTX_T*                    pt_svctx,
                    UINT32                      ui4_timeout,
                    x_os_timer_cb_fct           pf_invoke_fct,
                    VOID*                       pv_tag
                    )
{
    INT32 i4_ret;

    if (pt_svctx == NULL || ui4_timeout == 0) {
        return SVCTXR_INV_ARG;
    }

    i4_ret = x_timer_start(
                    pt_svctx->h_scdb_chk_timer,
                    ui4_timeout,
                    X_TIMER_FLAG_ONCE,
                    pf_invoke_fct,
                    pv_tag
                    );
    return (i4_ret == OSR_OK ? SVCTXR_OK : SVCTXR_FAIL);
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_av_scdb_chk_timer_stop
 * Description
 *      This API is used to stop the SCDB check timer.
 * Input arguments
 *      pt_svctx
 * Output arguments
 *      none
 * Returns:
 *      SVCTXR_OK          Routine successful.
 *      SVCTXR_FAIL        Routine failed
 *-----------------------------------------------------------------------------*/
INT32 svctx_scdb_chk_timer_stop(
                    SVCTX_T*                    pt_svctx
                    )
{
    INT32 i4_ret;

    if (pt_svctx == NULL) {
        return SVCTXR_INV_ARG;
    }

    i4_ret = x_timer_stop(pt_svctx->h_scdb_chk_timer);

    return (i4_ret == OSR_OK ? SVCTXR_OK : SVCTXR_FAIL);
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_is_owner
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
BOOL svctx_is_owner(
                    SVCTX_T*                        pt_svctx,
                    HANDLE_T                        h_svctx
                    )
{
    if (pt_svctx->h_svctx_owner == NULL_HANDLE || pt_svctx->h_svctx_owner == h_svctx) {
        return TRUE;
    } else {
        return FALSE;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_is_init
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
BOOL svctx_is_init(
                    VOID
                    )
{
    return b_g_init;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_class_lock
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 svctx_class_lock(
                    VOID
                    )
{
    INT32 i4_ret;

    i4_ret = x_sema_lock(h_g_module_sync_sema, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK) {
        DBG_ERROR((_ERROR_HEADER"x_sema_lock(h_g_module_sync_sema) failed, i4_ret=%d\r\n", i4_ret));
        return SVCTXR_FAIL;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_class_unlock
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 svctx_class_unlock(
                    VOID
                    )
{
    INT32 i4_ret;

    i4_ret = x_sema_unlock(h_g_module_sync_sema);
    if (i4_ret != OSR_OK) {
        DBG_ERROR((_ERROR_HEADER"x_sema_unlock(h_g_module_sync_sema) failed, i4_ret=%d\r\n", i4_ret));
        return SVCTXR_FAIL;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: svctx_is_equal_vsh_region_info
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE
 *          FALSE
 *-----------------------------------------------------------------------------*/
BOOL svctx_is_equal_vsh_region_info(
                    const VSH_REGION_INFO_T* pt_chker,
                    const VSH_REGION_INFO_T* pt_chkee
                    )
{
    if (pt_chker == NULL || pt_chkee == NULL) return FALSE;

    if (pt_chker->ui4_x == pt_chkee->ui4_x
            && pt_chker->ui4_y      == pt_chkee->ui4_y
            && pt_chker->ui4_width  == pt_chkee->ui4_width
            && pt_chker->ui4_height == pt_chkee->ui4_height) {
        return TRUE;
    }
    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name: svctx_init
 *
 * Description: This API initializes the Service Context Module
 *
 * Inputs:  ui2_num_svctxs    Contains the maximum number of service context
 *                            instances supported by the Service Context Module.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *-----------------------------------------------------------------------------*/
INT32 svctx_init(UINT16  ui2_num_svctxs, THREAD_DESCR_T*  pt_svctx_thread_descr)
{
    INT32  i4_ret;
    UINT16 ui2_idx;
    static handle_autofree_fct apf_autofree_fcts[SVCTXT_LAST_ENTRY - HT_GROUP_SVCTX] = {0};

    if (b_g_init == TRUE) {
        return SVCTXR_OK;
    }

    DBG_API((_API_HEADER"%s(ui2_amount=%d, each size=%d)\r\n", __func__, ui2_num_svctxs, sizeof(SVCTX_T)));

#ifdef CLI_LVL_ALL
    {
        extern UINT8 ui1_g_ntfy_code_num;

        if (ui1_g_ntfy_code_num != SVCTX_NTFY_CODE_LAST_ENTRY)
        {
       //     return SVCTXR_FAIL;
        }
    }
#endif

    svctx_set_dbg_level(DBG_INIT_LEVEL_MW_SVCTX);
    svctx_set_tms_level(TMS_LEVEL_NONE);

    if (ui2_num_svctxs > MAX_SVCTX_NUMBER) {
        ui2_num_svctxs = MAX_SVCTX_NUMBER;
    } else if (ui2_num_svctxs == 0) {
        ui2_num_svctxs = DEFAULT_SVCTX_NUMBER;
    }

    i4_ret = _svctx_init_default_av_dev_profile_table();
    if ( i4_ret != SVCTXR_OK){
        return SVCTXR_FAIL;
    }

    /* Setup Service Context Thread parameters*/
    if (pt_svctx_thread_descr == NULL) {
        t_g_svctx_thread_descr.ui1_priority = SVCTX_THREAD_DEFAULT_PRIORITY;
        t_g_svctx_thread_descr.ui2_num_msgs = SVCTX_NUM_OF_MSGS;
        t_g_svctx_thread_descr.z_stack_size = SVCTX_THREAD_DEFAULT_STACK_SIZE;
    } else {
        t_g_svctx_thread_descr = (*pt_svctx_thread_descr);

        if (t_g_svctx_thread_descr.ui1_priority == DEFAULT_PRIORITY) {
            t_g_svctx_thread_descr.ui1_priority = SVCTX_THREAD_DEFAULT_PRIORITY;
        }
        if (t_g_svctx_thread_descr.ui2_num_msgs == DEFAULT_NUM_MSGS) {
            t_g_svctx_thread_descr.ui2_num_msgs = SVCTX_NUM_OF_MSGS;
        }
        if (t_g_svctx_thread_descr.z_stack_size == DEFAULT_STACK_SIZE) {
            t_g_svctx_thread_descr.z_stack_size = SVCTX_THREAD_DEFAULT_STACK_SIZE;
        }
    }

    /* tricky & dirty solution (weiderchang 5/24/2007)
     * In order to avoid some PIP/POP blue/black screen side-effect as video stream
     * is stopped (i.e. the video plane is disable) and no background produced from
     * video plane, Navigator::POP sets the OSD blue/black screen via svctx's thead
     * context as receiving "No signal"/"BLOCK"/"Interrupt". But this OSD update
     * operation will cause Widget Manager to repaint any OSDs above PIP/POP frame
     * (e.g. Menu), then it would expense the svctx's stack, and cause it to over 100%
     *
     * The dirty & quick solution is to enlarge svctx's stack size
     * The correct solution is to not use OSD as blue/black screen in PIP/POP, and always
     * use video plane as blue/black screen, however this need the stream manager's modification.
     */
    t_g_svctx_thread_descr.z_stack_size += 10240;

    for (ui2_idx = 0; ui2_idx < (SVCTXT_LAST_ENTRY - HT_GROUP_SVCTX); ui2_idx++) {
        apf_autofree_fcts[ui2_idx] = _svctx_handle_autofree;
    }

    i4_ret = handle_set_autofree_tbl(
                    HT_GROUP_SVCTX,
                    apf_autofree_fcts
                    );
    if ( i4_ret != HR_OK){
        return SVCTXR_FAIL;
    }

    i4_ret = x_sema_create(
                    &h_g_module_sync_sema,
                    X_SEMA_TYPE_MUTEX,
                    X_SEMA_STATE_UNLOCK
                    );
    if(i4_ret != OSR_OK) {
        DBG_ERROR((_ERROR_HEADER"create sema for svctx failed, ret=%d\r\n", i4_ret));
        return SVCTXR_FAIL;
    }

    i4_ret = x_sema_create(
                    &h_g_module_sync_stop_sema,
                    X_SEMA_TYPE_MUTEX,
                    X_SEMA_STATE_UNLOCK
                    );
    if(i4_ret != OSR_OK) {
        DBG_ERROR((_ERROR_HEADER"create sema for svctx failed, ret=%d\r\n", i4_ret));
        return SVCTXR_FAIL;
    }

    i4_ret = x_sema_create(
                    &h_g_module_wait_sema,
                    X_SEMA_TYPE_BINARY,
                    X_SEMA_STATE_LOCK
                    );
    if(i4_ret != OSR_OK) {
        DBG_ERROR((_ERROR_HEADER"create sema for svctx failed, ret=%d\r\n", i4_ret));
        return SVCTXR_FAIL;
    }

    svctx_class_lock();
    do {
        b_g_init           = TRUE;
        ui2_g_svctx_amount = ui2_num_svctxs;

        i4_ret = activator_create(
                        SVCTX_THREAD_NAME,
                        SVCTX_MSG_Q_NAME,
                        t_g_svctx_thread_descr.ui2_num_msgs,
                        SVCTX_MSG_SIZE,
                        t_g_svctx_thread_descr.z_stack_size,
                        t_g_svctx_thread_descr.ui1_priority,
                        _svctx_dispatcher_fct,
                        NULL,
                        &pt_g_activator
                        );
        if (i4_ret != ACTVTRR_OK ) {
            b_g_init           = FALSE;
            ui2_g_svctx_amount = 0;
            pt_g_activator     = NULL;
            i4_ret = SVCTXR_FAIL;
            break;
        }

        DLIST_INIT(&t_g_using_svctx_q);
        DLIST_INIT(&t_g_pooled_svctx_q);
        DLIST_INIT(&t_g_handlers);

        pt_g_svctx_objs = (SVCTX_T*) x_mem_alloc(sizeof(SVCTX_T) * ui2_g_svctx_amount);
        if(pt_g_svctx_objs == NULL) {
            b_g_init           = FALSE;
            ui2_g_svctx_amount = 0;
            i4_ret = SVCTXR_FAIL;
            break;
        }

        for (ui2_idx = 0; ui2_idx < ui2_g_svctx_amount; ui2_idx++) {
            SVCTX_T* pt_svctx = &(pt_g_svctx_objs[ui2_idx]);

            i4_ret = SVCTXR_OK;
            x_memset(pt_svctx, 0, sizeof(SVCTX_T));
            pt_svctx->s_name[0] = '\0';
            pt_svctx->e_state   = SVCTX_COND_POOLED;

            DLIST_INSERT_HEAD(pt_svctx, &t_g_pooled_svctx_q, t_dlink);

            i4_ret = handle_link_init(&pt_svctx->t_hdl_link);
            if(i4_ret != HR_OK) {
                DBG_ERROR((_ERROR_HEADER"handle_link_init() failed\r\n"));
                i4_ret = SVCTXR_FAIL;
                break;
            }

            /* create the one - system service context handle */
            i4_ret = handle_alloc(
                        SVCTXT_SVCTX,
                        pt_svctx,
                        NULL,
                        _svctx_handle_free_fct,
                        &(pt_svctx->h_sys_svctx)
                        );
            if(i4_ret != HR_OK) {
                DBG_ERROR((_ERROR_HEADER"handle_alloc failed\r\n"));
                i4_ret = SVCTXR_FAIL;
                break;
            }

            i4_ret = handle_link(
                        &(pt_svctx->t_hdl_link),
                        pt_svctx->h_sys_svctx
                        );
            if(i4_ret != HR_OK) {
                DBG_ERROR((_ERROR_HEADER"handle_link() for %d-th service context failed\r\n"));
                i4_ret = SVCTXR_FAIL;
                break;
            }
        }

        player_init((UINT8) (ui2_g_svctx_amount * 2));
#ifdef MW_TUNER_OUT_SUPPORT
        t_g_tuner_out.e_type                         = TUNER_OUT_TYPE_IGONRE;
        t_g_tuner_out.t_info.ui2_svl_id              = SVL_NULL_ID;
        t_g_tuner_out.t_info.s_tuner_src_name[0]     = '\0';
        t_g_tuner_out.t_info.s_scart_out_src_name[0] = '\0';
#endif
#ifdef MW_TV_AV_BYPASS_SUPPORT
        svctx_mgr_init();
#endif
    } while (0);
    svctx_class_unlock();
#ifdef MW_TUNER_OUT_SUPPORT
    i4_ret = tuner_ctrlr_init();
#endif
    if (i4_ret != SVCTXR_OK) {
        svctx_deinit();
    }
    
#ifdef CLI_LVL_ALL
    svctx_cli_attach_cmd_tbl();
#endif

#ifdef GINGA_SUPPORT
    g_pf_svctx_pre_select_stream_nfy_fct = NULL;
#endif

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: svctx_deinit
 *
 * Description: This API destroys the Service Context Module
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *-----------------------------------------------------------------------------*/
VOID svctx_deinit(VOID)
{
    /* Caution: cannot invoke this function in service context's thread */
    INT32         i4_ret   = SVCTXR_OK;
    SVCTX_MSG_T   t_msg    = {0};
    SVCTX_T*      pt_svctx = NULL;

    svctx_class_lock();
    /* return to pooled */
    while ((pt_svctx = DLIST_HEAD(&t_g_using_svctx_q)) != NULL) {
        i4_ret = handle_free(pt_svctx->h_sys_svctx, FALSE);
        if(i4_ret != HR_OK) {
            DBG_ABORT(DBG_MOD_SVCTX);
        }
    }

    _svctx_unregister_all_handlers();

    DLIST_FOR_EACH(pt_svctx, &t_g_pooled_svctx_q, t_dlink) {
        if (pt_svctx->h_sys_svctx != NULL_HANDLE) {
            i4_ret = handle_free(pt_svctx->h_sys_svctx, FALSE);
            if(i4_ret != HR_OK) {
                DBG_ABORT(DBG_MOD_SVCTX);
            }
        }
    }
    player_deinit();
    x_mem_free(pt_g_svctx_objs);
    ui2_g_svctx_amount = 0;
    svctx_class_unlock();

    if (pt_g_activator != NULL) {
        t_msg.ui1_msg_id             = _SVCTX_MSG_DESTROY_REQ;
        t_msg.u.t_destroy_req.pf_nfy = _svctx_destroy_cb;
        t_msg.u.t_destroy_req.pv_tag = (VOID*)h_g_module_wait_sema;
        i4_ret = activator_send_msg(
                    pt_g_activator,
                    SVCTX_MSGQ_DEFAULT_PRIORITY,
                    &t_msg
                    );
        if(i4_ret != ACTVTRR_OK) {
            DBG_ABORT(DBG_MOD_SVCTX);
        }

        i4_ret = _WAIT(h_g_module_wait_sema);
        if(i4_ret != OSR_OK) {
            DBG_ABORT(DBG_MOD_SVCTX);
        }

        activator_destroy(pt_g_activator);
        pt_g_activator = NULL;
    }

    if (x_handle_valid(h_g_module_sync_sema) == TRUE) {
        x_sema_delete(h_g_module_sync_sema);
    }

    if (x_handle_valid(h_g_module_sync_stop_sema) == TRUE) {
        x_sema_delete(h_g_module_sync_stop_sema);
    }

    if (x_handle_valid(h_g_module_wait_sema) == TRUE) {
        x_sema_delete(h_g_module_wait_sema);
    }
    b_g_init = FALSE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_strm_comp_cc_filter
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
BOOL svctx_strm_comp_cc_filter (
                    UINT16                      ui2_num_recs,
                    UINT16                      ui2_idx,       /* in */
                    const STREAM_COMP_ID_T*     pt_comp_id,    /* in */
                    const SCDB_REC_T*           pt_scdb_rec,   /* in */
                    VOID*                       pv_tag         /* in */
                    )
{
    SM_CC_FILTER_T* pt_cc_filter = (SM_CC_FILTER_T*) pv_tag;

    ASSERT(pt_comp_id->e_type == ST_CLOSED_CAPTION,
                    ("stream type (%d) is not CLOSED_CAPTION)", pt_comp_id->e_type));


    switch(pt_cc_filter->e_cc_type) {
    case SM_CC_TYPE_NTSC_LINE21:
        if (pt_scdb_rec->u.t_atsc_cc.e_type == CC_ATSC_LINE_21) {
            if ((pt_cc_filter->u.e_data_type - SM_NTSC_DATA_TYPE_CC1)
                    == (pt_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_line_21.ui1_cc_id - CC_CC1)) {
                return TRUE;
            }
        }
        break;
    case SM_CC_TYPE_DTVCC:
        if (pt_scdb_rec->u.t_atsc_cc.e_type == CC_ATSC_DTV) {
            if (pt_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_dtv.ui1_caption_svc_num
                    == pt_cc_filter->u.ui1_service_num) {
                return TRUE;
            }
        }
        break;
    default:
        break;
    }
    return FALSE;
}

#ifdef MW_ISDB_SUPPORT
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_strm_comp_isdb_cc_filter
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
BOOL svctx_strm_comp_isdb_cc_filter (
                    UINT16                      ui2_num_recs,
                    UINT16                      ui2_idx,       /* in */
                    const STREAM_COMP_ID_T*     pt_comp_id,    /* in */
                    const SCDB_REC_T*           pt_scdb_rec,   /* in */
                    VOID*                       pv_tag         /* in */
                    )
{
    STRM_REQ_T*             pt_strm_req     = (STRM_REQ_T*) pv_tag;
    SCDB_ISDB_CAPTION_T*    pt_isdb_caption;
    UINT8                   ui1_this_tag;
    UINT8                   ui1_fst_tag     = 0x30;
    UINT8                   ui1_last_tag    = 0x37;
    UINT32                  ui4_user_tag    = 0;
    BOOL                    fg_need_select  = FALSE;
    BOOL                    fg_ret          = FALSE;

    ASSERT(pt_comp_id->e_type == ST_ISDB_CAPTION,("stream type (%d) is not ST_ISDB_CAPTION)", pt_comp_id->e_type));


#if 1   /* SBTVD */
    ui4_user_tag = pt_strm_req->ui4_isdbcc_idx - (UINT8)1;
#else   /* ISDB */
    ui4_user_tag = 0;
#endif

    ui4_user_tag += ui1_fst_tag;

    if (ui4_user_tag == pt_scdb_rec->u.t_isdb_caption.ui1_caption_tag)
    {
        fg_ret         = TRUE;
        fg_need_select = TRUE;
    }

    pt_isdb_caption = &(pt_strm_req->u.t_scdb.t_rec.u.t_isdb_caption);
    ui1_this_tag    = pt_scdb_rec->u.t_isdb_caption.ui1_caption_tag;

    if (0 == pt_isdb_caption->ui1_caption_tag && 0 == pt_isdb_caption->ui2_pid)
    {
        fg_need_select = TRUE;
    }
    else if ((ui1_fst_tag <= ui1_this_tag && ui1_this_tag <= ui1_last_tag)
              &&
             (ui1_this_tag < pt_isdb_caption->ui1_caption_tag))
    {
        fg_need_select = TRUE;
    }

    if (fg_need_select)
    {
        pt_strm_req->u.t_scdb.t_comp_id = *pt_comp_id;
        pt_strm_req->u.t_scdb.t_rec     = *pt_scdb_rec;
    }

    return fg_ret;
}


/*-----------------------------------------------------------------------------
 * Name
 *      svctx_strm_comp_isdb_text_filter
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
BOOL svctx_strm_comp_isdb_text_filter (
                    UINT16                      ui2_num_recs,
                    UINT16                      ui2_idx,       /* in */
                    const STREAM_COMP_ID_T*     pt_comp_id,    /* in */
                    const SCDB_REC_T*           pt_scdb_rec,   /* in */
                    VOID*                       pv_tag         /* in */
                    )
{
    STRM_REQ_T*             pt_strm_req     = (STRM_REQ_T*) pv_tag;
    SCDB_ISDB_TEXT_T*       pt_isdb_text;
    UINT8                   ui1_this_tag;
    UINT8                   ui1_fst_tag     = 0x38;
    UINT8                   ui1_last_tag    = 0x3F;
    UINT32                  ui4_user_tag    = 0;
    BOOL                    fg_need_select  = FALSE;
    BOOL                    fg_ret          = FALSE;

    ASSERT(pt_comp_id->e_type == ST_ISDB_TEXT,("stream type (%d) is not ST_ISDB_CAPTION)", pt_comp_id->e_type));


#if 1   /* SBTVD */
    ui4_user_tag = pt_strm_req->ui4_isdbcc_idx - (UINT8)1;
#else   /* ISDB */
    ui4_user_tag = 0;
#endif

    ui4_user_tag += ui1_fst_tag;

    if (ui4_user_tag == pt_scdb_rec->u.t_isdb_text.ui1_text_tag)
    {
        fg_ret         = TRUE;
        fg_need_select = TRUE;
    }

    pt_isdb_text    = &(pt_strm_req->u.t_scdb.t_rec.u.t_isdb_text);
    ui1_this_tag    = pt_scdb_rec->u.t_isdb_text.ui1_text_tag;

    if (0 == pt_isdb_text->ui1_text_tag && 0 == pt_isdb_text->ui2_pid)
    {
        fg_need_select = TRUE;
    }
    else if ((ui1_fst_tag <= ui1_this_tag && ui1_this_tag <= ui1_last_tag)
              &&
             (ui1_this_tag < pt_isdb_text->ui1_text_tag))
    {
        fg_need_select = TRUE;
    }

    if (fg_need_select)
    {
        pt_strm_req->u.t_scdb.t_comp_id = *pt_comp_id;
        pt_strm_req->u.t_scdb.t_rec     = *pt_scdb_rec;
    }

    return fg_ret;
}
#endif


/*-----------------------------------------------------------------------------
 * Name
 *      svctx_get_obj_amount
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
UINT16 svctx_get_obj_amount(
                    VOID
                    )
{
    return ui2_g_svctx_amount;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_register_handler
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 svctx_register_handler(
                    const CHAR*                 ps_name,
                    SVCTX_HDLR_FCT_TABLE_T*     pt_fcts
                    )
{
    SVCTX_HDLR_T* pt_hdlr;

    DBG_API((_API_HEADER"%s("
                    "ps_name = %s,"
                    "pt_fcts = 0x%x"
                    ")\r\n",
                    __func__,
                    ps_name,
                    pt_fcts
                    ));

    if (x_strlen(ps_name) == 0 || pt_fcts == NULL) {
        return SVCTXR_INV_ARG;
    }

    DLIST_FOR_EACH(pt_hdlr, &t_g_handlers, t_dlink) {
        if (x_strcmp(pt_hdlr->s_name, ps_name) == 0) {
            return SVCTXR_FAIL;
        }
    }

    pt_hdlr = (SVCTX_HDLR_T*) x_mem_alloc(sizeof(SVCTX_HDLR_T));
    if (pt_hdlr == NULL) {
        return SVCTXR_NO_RESOURCE;
    }

    x_strcpy(pt_hdlr->s_name, ps_name);
    pt_hdlr->t_fcts = *pt_fcts;
    pt_hdlr->ui4_video_mute_chk_delay = 0;

    DLIST_INSERT_TAIL(pt_hdlr, &t_g_handlers, t_dlink);

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_unregister_handler
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 svctx_unregister_handler(
                    const CHAR*                 ps_name
                    )
{
    SVCTX_HDLR_T* pt_hdlr;

    DBG_API((_API_HEADER"%s("
                    "ps_name = %s,"
                    ")\r\n",
                    __func__,
                    ps_name
                    ));

    if (x_strlen(ps_name) == 0) {
        return SVCTXR_INV_ARG;
    }

    DLIST_FOR_EACH(pt_hdlr, &t_g_handlers, t_dlink) {
        if (x_strcmp(pt_hdlr->s_name, ps_name) == 0) {
            DLIST_REMOVE(pt_hdlr, &t_g_handlers, t_dlink);
            x_mem_free(pt_hdlr);
            return SVCTXR_OK;
        }
    }

    return SVCTXR_NOT_EXIST;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_send_msg
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 svctx_send_msg(
                    HANDLE_T                    h_svctx,
                    SVCTX_MSG_T*                pt_msg
                    )
{
    INT32 i4_ret = SVCTXR_FAIL;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    pt_msg->h_svctx = h_svctx;
    if (pt_g_activator != NULL) {
        i4_ret = activator_send_msg(
                    pt_g_activator,
                    SVCTX_MSGQ_DEFAULT_PRIORITY,
                    pt_msg
                    );
        i4_ret = (i4_ret != ACTVTRR_OK) ? SVCTXR_FAIL: SVCTXR_OK;
    }
    return (i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_request_context_switch
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 svctx_request_context_switch(
                    svctx_invoke_fct            pf_invoke_func,
                    VOID*                       pv_tag
                    )
{
    INT32 i4_ret = SVCTXR_FAIL;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pt_g_activator != NULL) {
        SVCTX_MSG_T t_msg = {0};

        t_msg.pf_invoke_func  = pf_invoke_func;
        t_msg.pv_tag          = pv_tag;

        i4_ret = activator_send_msg(
                    pt_g_activator,
                    SVCTX_MSGQ_DEFAULT_PRIORITY,
                    &t_msg
                    );
        i4_ret = (i4_ret != ACTVTRR_OK) ? SVCTXR_FAIL: SVCTXR_OK;
    }
    return (i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_get_obj
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 svctx_get_obj(
                    HANDLE_T                    h_svctx,
                    SVCTX_T**                   ppt_svctx
                    )
{
    INT32          i4_ret;
    VOID*          pv_obj;
    HANDLE_TYPE_T  e_type;

    i4_ret = handle_get_type_obj(h_svctx, &e_type, &pv_obj);
    if (i4_ret != HR_OK || pv_obj == NULL || (e_type != SVCTXT_SVCTX
#ifdef MW_TV_AV_BYPASS_SUPPORT
        && e_type != SVCTXT_SVCTX_ASSOCIATE
#endif
    ))
    {
        DBG_ERROR((_ERROR_HEADER"handle_get_type_obj(h_svctx=%d) failed!\r\n"
                "i4_ret=%d, e_type=%d, pv_obj=%d",
                h_svctx, i4_ret, e_type, pv_obj));
        if (i4_ret == HR_INV_HANDLE) {
            i4_ret = SVCTXR_INV_HANDLE;
        } else {
            i4_ret = SVCTXR_FAIL;
        }
    } else {
        *ppt_svctx = (SVCTX_T*)pv_obj;
        i4_ret     = SVCTXR_OK;
    }

    return i4_ret;
}
#ifdef MW_TV_AV_BYPASS_SUPPORT
INT32 svctx_get_obj_with_ext_type(
                    HANDLE_T                    h_svctx,
                    SVCTX_T**                   ppt_svctx
                    )
{
    INT32          i4_ret;
    VOID*          pv_obj;
    HANDLE_TYPE_T  e_type;

    i4_ret = handle_get_type_obj(h_svctx, &e_type, &pv_obj);

    if ((i4_ret != HR_OK) ||
        (e_type != SVCTXT_SVCTX && e_type != SVCTXT_SVCTX_ASSOCIATE) ||
        (pv_obj == NULL))
    {
        DBG_ERROR((_ERROR_HEADER"handle_get_type_obj(h_svctx=%d) failed!\r\n"
                "i4_ret=%d, e_type=%d, pv_obj=%d",
                h_svctx, i4_ret, e_type, pv_obj));
        if (i4_ret == HR_INV_HANDLE) {
            i4_ret = SVCTXR_INV_HANDLE;
        } else {
            i4_ret = SVCTXR_FAIL;
        }
    } else {
        *ppt_svctx = (SVCTX_T*) pv_obj;
        i4_ret     = SVCTXR_OK;
    }

    return i4_ret;
}
#endif
#ifdef LINUX_TURNKEY_SOLUTION
static VOID 
_fake_select_nfy(
    HANDLE_T                    h_svctx,                /* in  */
    SVCTX_COND_T                e_nfy_cond,             /* in  */
    SVCTX_NTFY_CODE_T           e_code,                 /* in  */
    STREAM_TYPE_T               e_stream_type,          /* in  */
    VOID*                       pv_nfy_tag              /* in  */)
{    
    x_dbg_stmt("{"__FILE__"}%s():%d: this should not be called!\r\n", __func__, __LINE__);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _strm_cb_handle_free_fct
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *      TRUE
 *      FALSE
 *-----------------------------------------------------------------------------*/
static BOOL _strm_cb_handle_free_fct(
                    HANDLE_T                    h_handle,
                    HANDLE_TYPE_T               e_type,
                    VOID*                       pv_obj,
                    VOID*                       pv_tag,
                    BOOL                        b_req_handle
                    )
{

    DBG_INFO((_INFO_HEADER"%s()\r\n", __func__));

    return TRUE;
}
#endif
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_free_svc_req
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
VOID svctx_free_svc_req(
                    SVC_REQ_T*                  pt_svc_req
                    )
{
    do {
        if (pt_svc_req == NULL) {
            DBG_ERROR((_ERROR_HEADER"%s(pt_svc_req=0x%x) failed\r\n", __func__, pt_svc_req));
            break;
        }
#ifdef LINUX_TURNKEY_SOLUTION
        if (x_handle_valid(pt_svc_req->h_strm_cb))
        {
            if (handle_free(pt_svc_req->h_strm_cb, TRUE) != HR_OK)
            {
                DBG_ABORT(DBG_MOD_SVCTX);
            }
        }
        else
        {
            x_dbg_stmt("{"__FILE__"}%s():%d: invalid handle for freeing stream callback tags\r\n", __func__, __LINE__);
        }
        if (ri_svctx_free_rpc_cb_tag_if_needed != NULL)
        {
            ri_svctx_free_rpc_cb_tag_if_needed(
                    pt_svc_req->pf_svc_nfy,
                    pt_svc_req->pv_svc_nfy_tag);
        }
        pt_svc_req->pf_svc_nfy = _fake_select_nfy;
        pt_svc_req->pv_svc_nfy_tag = NULL;
#endif

        if (pt_svc_req->pt_hdlr == NULL) {
            DBG_ERROR((_ERROR_HEADER"%s(pt_svc_req=0x%x) no relative hdlr\r\n", __func__, pt_svc_req));
            break;
        }
        if (pt_svc_req->pt_hdlr->t_fcts.pf_free_svc_req == NULL) {
            break;
        }
        pt_svc_req->pt_hdlr->t_fcts.pf_free_svc_req(pt_svc_req);
    } while (0);
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_allocate_svc_req
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
SVC_REQ_T* svctx_allocate_svc_req(
                    SVCTX_HDLR_T*               pt_hdlr
                    )
{
    SVC_REQ_T* pt_svc_req = NULL;


    if (pt_hdlr == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(pt_hdlr=0x%x) failed\r\n", __func__, pt_hdlr));
        return NULL;
    }

    do {
        if (pt_hdlr->t_fcts.pf_allocate_svc_req == NULL) {
            break;
        }

        pt_svc_req = pt_hdlr->t_fcts.pf_allocate_svc_req();
        if (pt_svc_req == NULL) {
            break;
        }
        pt_svc_req->t_stop_cb.pf_nfy = NULL;
        pt_svc_req->t_stop_cb.pv_tag = NULL;
        pt_svc_req->b_force_unblock  = FALSE;
        pt_svc_req->ui4_serial_num   = ui4_g_svc_req_serial_num;
        pt_svc_req->pt_hdlr          = pt_hdlr;
#if 1 /* Xia - 090609 for Sony ATV Project */
        pt_svc_req->b_last_signal_booster = FALSE;
#endif
        pt_svc_req->b_freeze_change  = FALSE;
#ifdef LINUX_TURNKEY_SOLUTION
        if (handle_alloc(SVCTXT_SVCTX,
                         NULL,
                         NULL,
                         _strm_cb_handle_free_fct,
                         &(pt_svc_req->h_strm_cb)) != HR_OK)
        {
            pt_svc_req->h_strm_cb = NULL_HANDLE;
            x_dbg_stmt("{"__FILE__"}%s():%d: failed allocating handle for freeing stream callback tags\r\n", __func__, __LINE__);
        }
#endif

        ui4_g_svc_req_serial_num = ((ui4_g_svc_req_serial_num + 1) & 0xffff);
    } while (0);

    return (pt_svc_req);
}
#ifdef LINUX_TURNKEY_SOLUTION
HANDLE_T svctx_get_running_svc_req_strm_cb_handle(HANDLE_T  h_svctx)
{
    SVCTX_T*    pt_svctx;
    HANDLE_T    h_strm_cb;
    INT32       i4_ret;

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            h_strm_cb = NULL_HANDLE;
            break;
        }
        if (pt_svctx == NULL || pt_svctx->pt_running_svc_req == NULL)
        {
            h_strm_cb = NULL_HANDLE;
        }
        else
        {
            h_strm_cb = pt_svctx->pt_running_svc_req->h_strm_cb;
        }
    } while (0);
    svctx_class_unlock();
    if (h_strm_cb == NULL_HANDLE)
    {
        x_dbg_stmt("{"__FILE__"}%s():%d: failed getting h_strm_cb for %s running svc req\r\n",
                   __func__, __LINE__, pt_svctx->s_name);
    }
    return h_strm_cb;
}
VOID svctx_reg_rpc_handler_util_func(VOID*   pv_fct_1,
                                     VOID*   pv_fct_2,
                                     VOID*   pv_fct_3,
                                     VOID*   pv_fct_4)
{
    ri_svctx_same_select_listener_nfy_fct   = pv_fct_1;
    ri_svctx_free_rpc_cb_tag_if_needed      = pv_fct_2;
#ifdef SVCTX_SPLIT_SUPPORT
    ri_svctx_get_block_check_fct_cb_addr    = pv_fct_3;
#endif

}
#endif
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_free_strm_req
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
VOID svctx_free_strm_req(
                    STRM_REQ_T*                 pt_strm_req
                    )
{
    do {
        if (pt_strm_req == NULL) {
            DBG_ERROR((_ERROR_HEADER"%s(pt_strm_req=0x%x) failed\r\n", __func__, pt_strm_req));
            break;
        }

        if (pt_strm_req->pt_hdlr == NULL) {
            DBG_ERROR((_ERROR_HEADER"%s(pt_strm_req=0x%x) no relative hdlr\r\n", __func__, pt_strm_req));
            break;
        }
        if (pt_strm_req->pt_hdlr->t_fcts.pf_free_strm_req == NULL) {
            break;
        }
        pt_strm_req->pt_hdlr->t_fcts.pf_free_strm_req(pt_strm_req);
    } while (0);
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_allocate_strm_req
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
STRM_REQ_T* svctx_allocate_strm_req(
                    SVCTX_HDLR_T*               pt_hdlr
                    )
{
    STRM_REQ_T* pt_strm_req = NULL;

    do {
        if (pt_hdlr == NULL) {
            DBG_ERROR((_ERROR_HEADER"%s() pt_hdlr = 0x%x\r\n", __func__, pt_hdlr));
            break;
        }


        if (pt_hdlr->t_fcts.pf_allocate_strm_req == NULL) {
            break;
        }

        pt_strm_req = pt_hdlr->t_fcts.pf_allocate_strm_req();
        if (pt_strm_req != NULL) {
            pt_strm_req->pt_hdlr = pt_hdlr;
        }
    } while (0);

    return (pt_strm_req);
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_cc_filter_equals
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
BOOL svctx_cc_filter_equals(
                    const SM_CC_FILTER_T*       pt_chkee,
                    const SM_CC_FILTER_T*       pt_chker
                    )
{
    if (pt_chkee == NULL && pt_chker != NULL) return FALSE;
    if (pt_chkee != NULL && pt_chker == NULL) return FALSE;
    if (pt_chkee == NULL && pt_chker == NULL) return TRUE;

    if (pt_chkee->e_cc_type != pt_chker->e_cc_type) return FALSE;

    switch(pt_chkee->e_cc_type) {
    case SM_CC_TYPE_NTSC_LINE21:
        if (pt_chkee->u.e_data_type != pt_chkee->u.e_data_type) return FALSE;
        break;
    case SM_CC_TYPE_DTVCC:
        if (pt_chkee->u.ui1_service_num != pt_chkee->u.ui1_service_num) return FALSE;
        break;
    default:
        return TRUE;
    }

    return TRUE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_stop_cc_strm
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
VOID svctx_stop_cc_strm (
                    STRM_REQ_T*                 pt_strm_req
                    )
{
    INT32 i4_ret;

    DBG_API((_API_HEADER"%s("
                    "pt_strm_req = 0x%x"
                    ")\r\n",
                    __func__,
                    pt_strm_req
                    ));

    /* close previous closed caption if the request is not the same */
    if (x_handle_valid(pt_strm_req->h_stream) == FALSE) {
        return;
    }

    DBG_INFO((_INFO_HEADER"%s(), close previous closed caption(h_stream=%d)\r\n",
                    __func__,
                    pt_strm_req->h_stream
                    ));

    i4_ret = x_sm_close_stream(pt_strm_req->h_stream);
    if (i4_ret == SMR_OK || i4_ret == SMR_PENDING || i4_ret == SMR_INV_HANDLE) {
        pt_strm_req->e_strm_cond = SM_COND_CLOSING;
        DBG_INFO((_INFO_HEADER"x_sm_close_stream(ST_CLOSED_CAPTION, h_stream=%d)\r\n",
                    pt_strm_req->h_stream
                    ));
    } else {
        DBG_ERROR((_ERROR_HEADER"x_sm_close_stream(ST_CLOSED_CAPTION, handle= %d) failed, ret=%d\r\n",
                    pt_strm_req->h_stream,
                    i4_ret));
        pt_strm_req->h_stream                  = NULL_HANDLE;
        pt_strm_req->e_req_type                = STRM_REQ_SCDB;
        pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
        pt_strm_req->e_penging_status          = STRM_PENDING_COND_NONE;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_get_strm_comp_id
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 svctx_get_strm_comp_id(
                    SVCTX_T*                     pt_svctx,             /* in  */
                    STREAM_TYPE_T                e_strm_type  ,        /* in  */
                    BOOL                         b_auto_version_update,/* in  */
                    x_svctx_strm_comp_filter_fct pf_filter,            /* in  */
                    VOID*                        pv_tag,               /* in  */
                    STREAM_COMP_ID_T*            pt_comp_id,           /* out */
                    SCDB_REC_T*                  pt_scdb_rec           /* out */
                    )
{
    if (pt_svctx->pt_running_svc_req == NULL) {
        return SVCTXR_WRONG_STATE;
    }

    if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
        return SVCTXR_FAIL;
    }

    if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_strm_comp_id == NULL) {
        return SVCTXR_NOT_SUPPORT;
    }

    return pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_strm_comp_id(
                    pt_svctx,
                    e_strm_type  ,
                    b_auto_version_update,
                    pf_filter,
                    pv_tag,
                    pt_comp_id,
                    pt_scdb_rec
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_get_strm_req_strm_type
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
STREAM_TYPE_T svctx_get_strm_req_strm_type(
                    STRM_REQ_T*                  pt_strm_req
                    )
{
    if (pt_strm_req == NULL) {
        return ST_UNKNOWN;
    }

    if (pt_strm_req->e_req_type == STRM_REQ_SCDB) {
        return pt_strm_req->u.t_scdb.t_comp_id.e_type;
    } else if (pt_strm_req->e_req_type == STRM_REQ_PID) {
        return pt_strm_req->u.t_pid.e_strm_type;
    } else {
        return ST_UNKNOWN;
    }
}
/*-----------------------------------------------------------------------------
 * Name: svctx_set_stream_attr
 *
 * Description: This function is used to set the attribuites of a stream
 *
 * Inputs:  pt_svctx
 *          h_stream          Contains the stream handle.
 *          e_strm_type       Contains the stream type
 *          e_set_type        Contains the type of attributes to set.
 *          pv_set_info       References to the data information of the set
 *                            type.
 *          ui4_set_info_size Contains the size of data information of the
 *                            set type.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *-----------------------------------------------------------------------------*/
INT32 svctx_set_stream_attr(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_stream,
                    SM_SET_TYPE_T               e_set_type,
                    VOID*                       pv_set_info,
                    SIZE_T                      z_set_info_size
                    )
{
    INT32 i4_ret;

    i4_ret = x_sm_set(
                    h_stream,
                    e_set_type,
                    pv_set_info,
                    z_set_info_size
                    );
    if (i4_ret == SMR_NOT_SUPPORT || i4_ret == SMR_INTERNAL_ERROR) {
        return SVCTXR_NOT_SUPPORT;
    }

    switch (e_set_type) {
    case SM_VSH_SET_TYPE_PLANE_ORDER:
        pt_svctx->t_video_plane_order          = *((VSH_SET_PLANE_ORDER_INFO_T*)pv_set_info);
        pt_svctx->b_need_set_video_plane_order = (i4_ret == SMR_OK) ? FALSE : TRUE;
        break;
    case SM_VSH_SET_TYPE_VIDEO_REGION:
        pt_svctx->t_video_region = *((VSH_REGION_INFO_T*)pv_set_info);
        break;
    case SM_VSH_SET_TYPE_DISP_REGION:
        pt_svctx->t_disp_region = *((VSH_REGION_INFO_T*)pv_set_info);
        /* CAUTION:
         * pt_running_svc_req may be null when AP want to set only
         * the display region attributes in some scenarios.  
         */

#ifdef MW_PIP_POP_SUPPORT
#ifdef MW_TV_AV_BYPASS_SUPPORT
        if (!IS_SVCTX_ASSOCIATE(pt_svctx))
#endif
        {
            SCC_VID_MODE_EX_T   t_mode;

            if (NULL_HANDLE != pt_svctx->h_video_scc_comp)
            {
                if ((pt_svctx->t_disp_region.ui4_x == 0) &&
                    (pt_svctx->t_disp_region.ui4_y == 0) &&
                    (pt_svctx->t_disp_region.ui4_width == 0) &&
                    (pt_svctx->t_disp_region.ui4_height == 0))
                {
                    t_mode.e_mode = SCC_VID_NORMAL;
                }
                else
                {
                    t_mode.e_mode = pt_svctx->e_default_vid_mode;

                    if (!pt_svctx->b_is_blocked)
                    {
                        /* for DTV00137273 */
                        SVCTX_CA_INFO_T t_ca_info;

                        t_mode.e_mode = pt_svctx->e_default_vid_mode;

                        if (pt_svctx->pt_running_svc_req != NULL) {
                            if (pt_svctx->pt_running_svc_req->pt_hdlr != NULL) {
                                if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_cp_info != NULL) {
                                    i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_ca_info(
                                                        pt_svctx,
                                                        ST_VIDEO,
                                                        &t_ca_info
                                                        );

                                    if (i4_ret == SVCTXR_OK && t_ca_info.e_event == SM_EVN_CA_COND_NOT_AUTHORIZED)
                                    {
                                        t_mode.e_mode = SCC_VID_BLANK;
                                    }
                                }
                            }
                            /*for CR DTV00365307 mm_hdlr has no cp_info function*/
                            else if(pt_svctx->t_video_info.ui4_height == 0 && pt_svctx->t_video_info.ui4_width == 0)
                            {
                                t_mode.e_mode = SCC_VID_BLANK;                            
                            }
                        }
                    }
                    else
                    {
                        t_mode.e_mode = SCC_VID_BLANK;
                    }
                }

#if 1  /* for MW_NINJA_CHANGE_CHANNEL */
                if ((pt_svctx->pt_running_svc_req != NULL)
                    && (pt_svctx->pt_running_svc_req->e_src_type == SRC_TYPE_BRDCST_SVL))
                {
                    CH_SVC_REQ_T* pt_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
                    if (t_mode.e_mode == SCC_VID_BLANK && pt_req->t_src_info.b_ninja_change)
                    {
                        t_mode.e_mode = SCC_VID_NORMAL;
                    }
                }
#endif  

                if (t_mode.e_mode != SCC_VID_BLANK && pt_svctx->e_video_decode_status == VSH_DIG_DECODE_NO_DATA )
                {
                    t_mode.e_mode = SCC_VID_BLANK;
                }

                t_mode.t_disp_region = pt_svctx->t_disp_region;
                svctx_cli_print_video_region(pt_svctx, t_mode.e_mode, __func__, __LINE__);
                i4_ret = x_scc_vid_set_mode_ex(
                                pt_svctx->h_video_scc_comp,
                                &t_mode
                                );

                if (i4_ret != SMR_OK)
                {
                    return (SVCTXR_FAIL);
                }
            }
        }
#else
#ifdef MW_TV_AV_BYPASS_SUPPORT
        if (!IS_SVCTX_ASSOCIATE(pt_svctx))
#endif
        {
            i4_ret = x_scc_vid_set_disp_region(
                pt_svctx->h_video_scc_comp, 
                &(pt_svctx->t_disp_region));
            if (i4_ret != SMR_OK)
            {
                return (SVCTXR_FAIL);
            }
        }        
#endif
        break;
    case SM_VSH_SET_TYPE_AUTO_CLK_PHS_POS:
        pt_svctx->e_auto_clk_pos_phs_cond = AUTO_CLK_POS_PHS_COND_UNKNOWN;
        pt_svctx->b_need_auto_clk_phs_pos = i4_ret == SMR_OK ? FALSE : TRUE;
        break;
    case SM_VSH_SET_TYPE_AUTO_COLOR:
        pt_svctx->e_auto_color_cond = AUTO_COLOR_COND_UNKNOWN;
        pt_svctx->b_need_auto_color = i4_ret == SMR_OK ? FALSE : TRUE;
        break;
    case SM_VSH_SET_TYPE_VID_CLOCK:
        pt_svctx->i2_video_clock  = (INT16)(UINT32)pv_set_info;
        break;
    case SM_VSH_SET_TYPE_VID_PHASE:
        pt_svctx->ui2_video_phase = (UINT16)(UINT32)pv_set_info;
        break;
    case SM_VSH_SET_TYPE_VID_H_POS:
        pt_svctx->ui2_video_h_pos = (UINT16)(UINT32)pv_set_info;
        break;
    case SM_VSH_SET_TYPE_VID_V_POS:
        pt_svctx->ui2_video_v_pos = (UINT16)(UINT32)pv_set_info;
        break;
    default:
        return svctx_translate_smr_2_svctxr(i4_ret);
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_set_video_stream_attrs
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 svctx_set_video_stream_attrs (
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_video_stream
                    )
{
    INT32 i4_ret;

    if (pt_svctx == NULL || x_handle_valid(h_video_stream) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    i4_ret = x_sm_set(
                    h_video_stream,
                    SM_VSH_SET_TYPE_VIDEO_REGION,
                    &pt_svctx->t_video_region,
                    sizeof(VSH_REGION_INFO_T)
                    );
    if (i4_ret != SMR_OK) {
        DBG_ERROR((_ERROR_HEADER"x_sm_set(SM_VSH_SET_TYPE_VIDEO_REGION, handle=%d) failed, ret=%d\r\n",
                    h_video_stream,
                    i4_ret));
    }

    if (pt_svctx->b_need_set_video_plane_order == TRUE) {
        /* need to blank first, when set display resion & change ORDER
         * otherwise, 720p will have noise. Driver asks to do this operation
         * 2007/3/8 from Cosh
         */
        svctx_set_video_plane(pt_svctx, SCC_VID_BLANK);
    }

    i4_ret = x_sm_set(
                    h_video_stream,
                    SM_VSH_SET_TYPE_DISP_REGION,
                    &pt_svctx->t_disp_region,
                    sizeof(VSH_REGION_INFO_T)
                    );
    if (i4_ret != SMR_OK) {
        DBG_ERROR((_ERROR_HEADER"x_sm_set(SM_VSH_SET_TYPE_DISP_REGION, handle=%d) failed, ret=%d\r\n",
                    h_video_stream,
                    i4_ret));
    }

    if (pt_svctx->b_need_set_video_plane_order == TRUE) {
        x_thread_delay(50); /* 2006/7/12 from SA3's suggestion, and will remove it after SA3 fix it */
        i4_ret = x_sm_set(
                    h_video_stream,
                    SM_VSH_SET_TYPE_PLANE_ORDER,
                    &pt_svctx->t_video_plane_order,
                    sizeof(VSH_SET_PLANE_ORDER_INFO_T)
                    );
        pt_svctx->b_need_set_video_plane_order = (i4_ret == SMR_OK) ? FALSE : TRUE;
    }

    if (pt_svctx->i2_video_clock != SM_VSH_VID_COLCK_UNKNOWN) {
        i4_ret = x_sm_set(
                    h_video_stream,
                    SM_VSH_SET_TYPE_VID_CLOCK,
                    (VOID*)(UINT32)pt_svctx->i2_video_clock,
                    sizeof(INT16)
                    );
        if (i4_ret != SMR_OK && i4_ret != SMR_NOT_SUPPORT) {
            DBG_ERROR((_ERROR_HEADER"x_sm_set(SM_VSH_SET_TYPE_VID_CLOCK, handle= %d) failed, ret=%d\r\n",
                        h_video_stream,
                        i4_ret));
        }
    }

    if (pt_svctx->ui2_video_phase != SM_VSH_VID_PHASE_UNKNOWN) {
        i4_ret = x_sm_set(
                    h_video_stream,
                    SM_VSH_SET_TYPE_VID_PHASE,
                    (VOID*)(UINT32)pt_svctx->ui2_video_phase,
                    sizeof(UINT16)
                    );
        if (i4_ret != SMR_OK && i4_ret != SMR_NOT_SUPPORT) {
            DBG_ERROR((_ERROR_HEADER"x_sm_set(SM_VSH_SET_TYPE_VID_PHASE, handle=%d) failed, ret=%d\r\n",
                    h_video_stream,
                    i4_ret));
        }
    }

    if (pt_svctx->ui2_video_h_pos != SM_VSH_VID_H_POS_UNKNOWN) {
        i4_ret = x_sm_set(
                    h_video_stream,
                    SM_VSH_SET_TYPE_VID_H_POS,
                    (VOID*)(UINT32)pt_svctx->ui2_video_h_pos,
                    sizeof(UINT16)
                    );
        if (i4_ret != SMR_OK && i4_ret != SMR_NOT_SUPPORT) {
            DBG_ERROR((_ERROR_HEADER"x_sm_set(SM_VSH_SET_TYPE_VID_H_POS, handle= d) failed, ret=%d\r\n",
                    h_video_stream,
                    i4_ret));
        }
    }

    if (pt_svctx->ui2_video_v_pos != SM_VSH_VID_V_POS_UNKNOWN) {
        i4_ret = x_sm_set(
                    h_video_stream,
                    SM_VSH_SET_TYPE_VID_V_POS,
                    (VOID*)(UINT32)pt_svctx->ui2_video_v_pos,
                    sizeof(UINT16)
                    );
        if (i4_ret != SMR_OK && i4_ret != SMR_NOT_SUPPORT) {
            DBG_ERROR((_ERROR_HEADER"x_sm_set(SM_VSH_SET_TYPE_VID_V_POS, handle=%d) failed, ret=%d\r\n",
                    h_video_stream,
                    i4_ret));
        }
    }

    if (pt_svctx->b_need_auto_clk_phs_pos == TRUE) {
        i4_ret = x_sm_set(
                    h_video_stream,
                    SM_VSH_SET_TYPE_AUTO_CLK_PHS_POS,
                    NULL,
                    0
                    );
        if (i4_ret != SMR_OK && i4_ret != SMR_NOT_SUPPORT) {
            DBG_ERROR((_ERROR_HEADER"x_sm_set(SM_VSH_SET_TYPE_AUTO_CLK_PHS_POS, handle=%d) failed, ret=%d\r\n",
                    h_video_stream,
                    i4_ret));
        }
        pt_svctx->b_need_auto_clk_phs_pos = FALSE;
    }

    if (pt_svctx->b_need_auto_color == TRUE) {
        i4_ret = x_sm_set(
                    h_video_stream,
                    SM_VSH_SET_TYPE_AUTO_COLOR,
                    NULL,
                    0
                    );
        if (i4_ret != SMR_OK && i4_ret != SMR_NOT_SUPPORT) {
            DBG_ERROR((_ERROR_HEADER"x_sm_set(SM_VSH_SET_TYPE_AUTO_COLOR, handle=%d) failed, ret=%d\r\n",
                    h_video_stream,
                    i4_ret));
        }
        pt_svctx->b_need_auto_color = FALSE;
    }

    if (svctx_is_no_video(pt_svctx) == FALSE &&
        (pt_svctx->pt_running_svc_req != NULL) &&
        (pt_svctx->pt_running_svc_req->ui4_stream_mode & ST_MASK_VIDEO) > 0) {
        svctx_set_video_plane(pt_svctx, pt_svctx->e_default_vid_mode);
    }
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_preproc_svc_req_snk_part
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 svctx_preproc_svc_req_snk_part(
                    SVCTX_T*                    pt_svctx,
                    const SRC_DESC_T*           pt_src_desc,
                    const SNK_DESC_T*           pt_snk_desc,
                    CHAR*                       ps_snk_grp_name,
                    CHAR*                       ps_snk_grp_name_aux,
                    SVC_REQ_T*                  pt_new_svc_req,
                    SVC_REQ_T*                  pt_running_svc_req
                    )
{
    if (x_strlen(ps_snk_grp_name) == 0) {
        if (x_strlen(pt_svctx->s_snk_grp_name) == 0) {
            return SVCTXR_INV_ARG;
        }
        x_strcpy(pt_new_svc_req->s_snk_grp_name, pt_svctx->s_snk_grp_name);
    } else {
        x_strcpy(pt_new_svc_req->s_snk_grp_name, ps_snk_grp_name);
    }

    /* modified by Allen */
    if (x_strlen(ps_snk_grp_name_aux) == 0) {
#ifndef MW_TV_AV_BYPASS_SUPPORT
        if (x_strlen(pt_svctx->s_snk_grp_name_aux) == 0) {
            return SVCTXR_INV_ARG;
        }
#endif
        x_strcpy(pt_new_svc_req->s_snk_grp_name_aux, pt_svctx->s_snk_grp_name_aux);
    } else {
        x_strcpy(pt_new_svc_req->s_snk_grp_name_aux, ps_snk_grp_name_aux);
    }

    /* video region */
    if (pt_snk_desc != NULL && pt_snk_desc->pt_video_region != NULL) {
        pt_svctx->t_video_region = *(pt_snk_desc->pt_video_region);
    }

    /* display region */
    if (pt_snk_desc != NULL && pt_snk_desc->pt_disp_region != NULL) {
        pt_new_svc_req->b_same_disp_region
             = svctx_is_equal_vsh_region_info(
                    &pt_svctx->t_disp_region,
                    pt_snk_desc->pt_disp_region
                    );
        pt_svctx->t_disp_region = *(pt_snk_desc->pt_disp_region);
    } else {
        pt_new_svc_req->b_same_disp_region = TRUE;
    }

    /* video plane order */
    if (pt_snk_desc != NULL && pt_snk_desc->pt_video_plane_order != NULL) {
        pt_svctx->b_need_set_video_plane_order = TRUE;
        pt_svctx->t_video_plane_order = *(pt_snk_desc->pt_video_plane_order);
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: svctx_ntfy_svc_req_stopped
 *
 * Description:
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
INT32 svctx_ntfy_svc_req_stopped(
                    SVCTX_T*                    pt_svctx
                    )
{
    INT32 i4_ret = SVCTXR_OK;

    ASSERT(pt_svctx != NULL, ("Internall error in svctx_ntfy_svc_req_stopped()"));

    DBG_INFO((_INFO_HEADER"%s(state=%s)\r\n",  __func__, s_g_state_strings[pt_svctx->e_state]));
    
	ASSERT(pt_svctx->pt_running_svc_req != NULL, ("pt_running_svc_req = NULL in svctx_ntfy_svc_req_stopped()"));
	
    DBG_INFO((_INFO_HEADER"%s(), stop svc req = 0x%x!\r\n", __func__, pt_svctx->pt_running_svc_req));

    pt_svctx->b_in_stopping = FALSE;

    if (pt_svctx->b_in_closing == TRUE) {
        svctx_notify_clients_svc_msg(
                    pt_svctx,
                    SVCTX_NTFY_CODE_STOPPED,
                    ST_UNKNOWN,
                    pt_svctx->pt_running_svc_req->h_svctx,
                    pt_svctx->pt_running_svc_req->pf_svc_nfy
                    );

        if (handle_free(pt_svctx->h_last_client, FALSE) != HR_OK) {
            /* DTV00000560 - check x_dbg_abort() usage */
            DBG_ABORT(DBG_MOD_SVCTX);
        }

        if (pt_svctx->pf_sync_close_nfy != NULL) {
            pt_svctx->pf_sync_close_nfy(
                    pt_svctx->pf_sync_close_tag
                    );
        }

        pt_svctx->b_in_closing = FALSE;
    } else {
        /* Unmute audio which may have been muted since stopping begins */
        if (pt_svctx->pt_running_svc_req->ui4_stream_mode & MAKE_BIT_MASK_32(ST_AUDIO))
        {
            svctx_ctrl_main_audio(pt_svctx, FALSE);
        }

        if (pt_svctx->pt_pending_svc_req != NULL) {
#ifdef MW_TV_AV_BYPASS_SUPPORT
            DBG_SVCTXMGR_MSG((
                "{SVCTX_MGR} Old service stopped. Selecting new service (%s)...\r\n",
                pt_svctx->s_name));

            if ( pt_svctx->pt_pending_svc_req->e_src_type != pt_svctx->pt_running_svc_req->e_src_type) {
                svctx_mgr_remove_link_with_obj(ACTIVE_LINK_NOW, pt_svctx);
            }
#endif
            DBG_INFO((_INFO_HEADER"%s(), select next svc req = 0x%x!\r\n", __func__, pt_svctx->pt_pending_svc_req));
#ifdef MW_TV_STATUS_SUPPORT
            {
                BOOL  fgTVStatus_change = FALSE;    
                BOOL  fgTVOn = FALSE;
                /* old src is MMP  &&  current src is not MMP */
                if ((_SVCTX_SRC_TYPE_IS_MMP(pt_svctx->pt_running_svc_req->e_src_type)) &&
                     (!_SVCTX_SRC_TYPE_IS_MMP(pt_svctx->pt_pending_svc_req->e_src_type )))
               {
                  x_dbg_stmt("[SVCTX] Old SVC Source = %d is MMP and Current SVC Source = %d is not MMP\n",
                             pt_svctx->pt_running_svc_req->e_src_type,
                             pt_svctx->pt_pending_svc_req->e_src_type);
                  x_dbg_stmt("[SVCTX] Set TV Status ON\n");
                  fgTVStatus_change = TRUE;
                  fgTVOn = TRUE;
               }

                /* old src is not MMP  &&  current src is  MMP */
               else if((!_SVCTX_SRC_TYPE_IS_MMP(pt_svctx->pt_running_svc_req->e_src_type)) &&
                       (_SVCTX_SRC_TYPE_IS_MMP(pt_svctx->pt_pending_svc_req->e_src_type )))
               {
                  x_dbg_stmt("[SVCTX] Old SVC Source = %d is not MMP and Current SVC  Source = %d is MMP\n",
                             pt_svctx->pt_running_svc_req->e_src_type,
                             pt_svctx->pt_pending_svc_req->e_src_type);
                  x_dbg_stmt("[SVCTX] Set TV Status OFF\n");
                  fgTVStatus_change = TRUE;
                  fgTVOn = FALSE;
               }                
                
               if(fgTVStatus_change)
               {
                   _svctx_set_tv_status(fgTVOn);
               }
            }
#endif         
            svctx_free_svc_req(pt_svctx->pt_running_svc_req);
            pt_svctx->pt_running_svc_req = pt_svctx->pt_pending_svc_req;
            pt_svctx->pt_pending_svc_req = NULL;
            i4_ret = _svctx_select_running_svc(pt_svctx);
        } else {

            /* need to disable analog tuner to not output Tuner via SCART */
            if (pt_svctx->pt_running_svc_req->e_src_type == SRC_TYPE_BRDCST_FREQ
                    || pt_svctx->pt_running_svc_req->e_src_type == SRC_TYPE_BRDCST_SVL) {
                tuner_ctrlr_disable_ana_tuner();
            } else {
                _svctx_set_analog_tuner_out_attr(pt_svctx);
            }

            /* reset status when svctx stopped */
            pt_svctx->e_cc_avil_cond                    = SVCTX_CC_AVIL_COND_UNKNOW;
            pt_svctx->b_is_blocked                      = FALSE;
            pt_svctx->b_block_notified                  = FALSE;
            pt_svctx->e_signal_status                   = SIGNAL_STATUS_UNKNOWN;
            pt_svctx->b_signal_loss_notified            = FALSE;
            pt_svctx->b_delay_video_mute_check          = FALSE;
            pt_svctx->ui4_video_mute_chk_delay          = DEFAULT_SIGNAL_LOSS_CHK_TIMEOUT;
            pt_svctx->e_video_mute_code                 = SVCTX_NTFY_CODE_AUDIO_VIDEO_SVC;
            pt_svctx->e_av_status                       = SVC_AV_STATUS_UNKNOWN;
            pt_svctx->b_in_fine_tuning                  = FALSE;
            pt_svctx->b_disable_no_signal_as_block_bias = FALSE;
            pt_svctx->b_in_restoring_orig_freq          = FALSE;
            pt_svctx->e_audio_decode_status             = ASH_AUDIO_DECODE_UNKNOWN;
            pt_svctx->e_video_decode_status             = VSH_DIG_DECODE_UNKNOWN;

            if (pt_svctx->pt_running_svc_req->t_stop_cb.pf_nfy != NULL) {
                pt_svctx->pt_running_svc_req->t_stop_cb.pf_nfy(
                            pt_svctx->pt_running_svc_req->t_stop_cb.pv_tag
                            );
                pt_svctx->pt_running_svc_req->t_stop_cb.pf_nfy = NULL;
                pt_svctx->pt_running_svc_req->t_stop_cb.pv_tag = NULL;
            }

            if (pt_svctx->b_in_closing == TRUE) {
                pt_svctx->e_state = SVCTX_COND_POOLED;
            } else {
                pt_svctx->e_state = SVCTX_COND_READY;
            }

            svctx_notify_clients_svc_msg(
                    pt_svctx,
                    SVCTX_NTFY_CODE_STOPPED,
                    ST_UNKNOWN,
                    pt_svctx->pt_running_svc_req->h_svctx,
                    pt_svctx->pt_running_svc_req->pf_svc_nfy
                    );

            pt_svctx->pt_pending_svc_req = pt_svctx->pt_running_svc_req;
            pt_svctx->pt_running_svc_req = NULL;
#ifdef MW_TV_AV_BYPASS_SUPPORT
            if (pt_svctx->b_piped_stop)
            {
                svctx_mgr_check_queued_req(
                    ACTIVE_LINK_NEXT,
                    pt_svctx->e_next_src_type,
                    pt_svctx,
                    TRUE);

                svctx_mgr_remove_link_with_obj(ACTIVE_LINK_NEXT, pt_svctx);
                pt_svctx->b_piped_stop = FALSE;
            }

            svctx_mgr_remove_link_with_obj(ACTIVE_LINK_NOW, pt_svctx);
#endif
        }
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_reset_audio_info
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 svctx_reset_audio_info(
                    SVCTX_T*                    pt_svctx
                    )
{
    if (pt_svctx == NULL) {
        return SVCTXR_INV_ARG;
    }

    _RESET_AUDIO_FMT(&(pt_svctx->t_audio_info));

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_set_video_info
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 svctx_set_video_info(
                    SVCTX_T*                    pt_svctx,
                    VSH_SRC_RESOLUTION_INFO_T*  pt_video_info
                    )
{
    if (pt_svctx == NULL || pt_video_info == NULL) {
        return SVCTXR_INV_ARG;
    }

    pt_svctx->t_video_info = *pt_video_info;

    pt_svctx->t_x_video_info = *pt_video_info;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_reset_color_sys
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 svctx_reset_color_sys(
                    SVCTX_T*                    pt_svctx
                    )
{
    if (pt_svctx == NULL) {
        return SVCTXR_INV_ARG;
    }

    pt_svctx->t_clr_sys = COLOR_SYS_UNKNOWN;
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_set_av_status
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 svctx_set_av_status(
                    SVCTX_T*                    pt_svctx,
                    SVC_AV_STATUS_T             e_av_status
                    )
{
    if (e_av_status > SVC_AV_STATUS_UNKNOWN) {
        return SVCTXR_INV_ARG;
    }

    pt_svctx->e_av_status = e_av_status;
    if (e_av_status == SVC_AV_STATUS_UNKNOWN) {
        pt_svctx->t_crnt_nfy_msg.e_svc_code = SVCTX_NTFY_CODE_NONE;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_set_color_sys
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 svctx_set_color_sys(
                    SVCTX_T*                    pt_svctx,
                    COLOR_SYS_T*                pt_color_sys
                    )
{
    if (pt_color_sys == NULL) {
        return SVCTXR_INV_ARG;
    }

    pt_svctx->t_clr_sys = *pt_color_sys;

    pt_svctx->t_x_clr_sys = *pt_color_sys;

    DBG_INFO((_INFO_HEADER"%s(%s)\r\n", __func__, s_g_color_systems[pt_svctx->t_clr_sys]));

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_set_audio_info
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 svctx_set_audio_info(
                    SVCTX_T*                    pt_svctx,
                    ASH_AUDIO_INFO_T*           pt_audio_info
                    )
{
    if (pt_audio_info == NULL) {
        return SVCTXR_INV_ARG;
    }

    pt_svctx->t_audio_info = *pt_audio_info;

    pt_svctx->t_x_audio_info = *pt_audio_info;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_strm_nfy_fct
 * Description
 *      The API is used to receive the notification of a stream, and
 *      transfer to an internal event, and send to the attached application's
 *      event dispatcher for further processing.
 * Input arguments
 *      e_event       the specific event to indicate more detail information of
 *                    the notification
 *      ui4_data      additional data
 * Output arguments
 *      pt_strm_msg
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 svctx_get_strm_msg_info(
                    SM_EVN_TYPE_T               e_event,
                    UINT32                      ui4_data,
                    SVCTX_STRM_MSG_T*           pt_strm_msg
                    )
{
    if (pt_strm_msg == NULL) {
        return SVCTXR_INV_ARG;
    }

    switch (e_event) {
    case SM_EVN_AS_AUD_INFO_CHG:
        pt_strm_msg->uinfo.t_audio = *((ASH_AUDIO_INFO_T*)ui4_data);
        break;
    case SM_EVN_VS_VIDEO_RESOLUTION_CHG:
#ifdef MW_TV_AV_BYPASS_SUPPORT
    case SM_EVN_BYPASS_VIDEO_RESOLUTION_CHG:
#endif
        pt_strm_msg->uinfo.t_video = *((VSH_SRC_RESOLUTION_INFO_T*)ui4_data);
        break;
    case SM_EVN_CCI_INFORMATION:
        pt_strm_msg->uinfo.t_cp = *((CP_INFO_LIST_T*)ui4_data);
        break;
    case SM_EVN_VS_ANA_TV_SIGNAL:
#ifdef MW_TV_AV_BYPASS_SUPPORT
    case SM_EVN_BYPASS_ANA_TV_SIGNAL:
#endif
        pt_strm_msg->uinfo.e_tv_signal_status = (VSH_ANA_TV_SIG_STATUS_T)ui4_data;
        break;
    case SM_EVN_VS_ANA_COLOR_SYSTEM:
#ifdef MW_TV_AV_BYPASS_SUPPORT
    case SM_EVN_BYPASS_ANA_COLOR_SYSTEM:
#endif
        pt_strm_msg->uinfo.t_clr_sys = (COLOR_SYS_T)ui4_data;
        break;
    case SM_EVN_VS_ANA_CONTENT_ADV_INFO:
        pt_strm_msg->uinfo.t_cnt_advisory = *((VSH_CNT_ADV_INFO_T*)ui4_data);
        break;
    case SM_EVN_VS_CC_INDICATOR:
        pt_strm_msg->uinfo.b_cc_indicator = (BOOL)ui4_data;
        break;
    case SM_EVN_CCS_EXISTED_CC_IND_CHG:
        break;
    case SM_EVN_VS_AUTO_COLOR_DONE:
        pt_strm_msg->uinfo.b_auto_color_done = (BOOL)ui4_data;
        break;
    case SM_EVN_VS_AUTO_CLK_POS_PHS_DONE:
        pt_strm_msg->uinfo.b_auto_clk_pos_phs_done = (BOOL)ui4_data;
    case SM_EVN_VS_DIG_DECODE_STATUS_CHG:
        pt_strm_msg->uinfo.e_video_decode_status = (VSH_DIG_DECODE_STATUS_T)ui4_data;
        break;
    case SM_EVN_AS_DECODE_STATUS_CHG:
        pt_strm_msg->uinfo.e_audio_decode_status = (ASH_AUDIO_DECODE_STATUS_T)ui4_data;
        break;
    case SM_EVN_AS_TV_AUD_SYS_DETECTED:
        pt_strm_msg->uinfo.t_aud_sys = *((ASH_AUD_SYS_INFO_T*)ui4_data);
        break;
    case SM_EVN_VS_AUD_PROTECT_INFO:
        pt_strm_msg->uinfo.e_aud_protect_info = (VSH_AUD_PROTECT_INFO_T)ui4_data;
        break;
    default:
        if (e_event >= SM_EVN_GRP_CA && e_event < SM_EVN_GRP_CCI) {
            pt_strm_msg->uinfo.t_ca.e_event = e_event;
            pt_strm_msg->uinfo.t_ca.e_code  = (CA_DETAIL_COND_T)ui4_data;
        } else {
            pt_strm_msg->uinfo.ui4_data = ui4_data;
        }
        break;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_reset_video_info
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 svctx_reset_video_info(
                    SVCTX_T*                    pt_svctx
                    )
{
    if (pt_svctx == NULL) {
        return SVCTXR_INV_ARG;
    }

    _RESET_VIDEO_RES(&(pt_svctx->t_video_info));
    svctx_reset_color_sys(pt_svctx);

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      svctx_reset_x_av_info
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 svctx_reset_x_av_info(
                    SVCTX_T*                    pt_svctx
                    )
{
    if (pt_svctx == NULL) {
        return SVCTXR_INV_ARG;
    }

    _RESET_VIDEO_RES(&(pt_svctx->t_x_video_info));
    pt_svctx->t_x_clr_sys = COLOR_SYS_UNKNOWN;
    _RESET_AUDIO_FMT(&(pt_svctx->t_x_audio_info));
    pt_svctx->e_x_av_status = SVC_AV_STATUS_UNKNOWN;

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      svctx_update_video_info_from_sm
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 svctx_update_video_info_from_sm(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_video_stream
                    )
{
    SIZE_T z_get_info_size = sizeof(VSH_SRC_RESOLUTION_INFO_T);
    INT32  i4_ret;

    if (pt_svctx == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (x_handle_valid(h_video_stream) == FALSE) {
        return SVCTXR_INV_HANDLE;
    }

    i4_ret = x_sm_get(
                    h_video_stream,
#ifdef MW_TV_AV_BYPASS_SUPPORT
                    IS_SVCTX_ASSOCIATE(pt_svctx) ? SM_BSH_GET_TYPE_VIDEO_SRC_RESOLUTION :
#endif
                    SM_VSH_GET_TYPE_VIDEO_SRC_RESOLUTION,
                    (VOID*)&pt_svctx->t_video_info,
                    &z_get_info_size
                    );
    if (i4_ret != SMR_OK) {
        return SVCTXR_FAIL;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_update_audio_info_from_sm
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 svctx_update_audio_info_from_sm(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_audio_stream
                    )
{
    SIZE_T z_get_info_size = sizeof(ASH_AUDIO_INFO_T);
    INT32  i4_ret;

    if (pt_svctx == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (x_handle_valid(h_audio_stream) == FALSE) {
        return SVCTXR_INV_HANDLE;
    }

    i4_ret = x_sm_get(
                    h_audio_stream,
                    SM_ASH_GET_TYPE_AUDIO_INFO,
                    (VOID*)&pt_svctx->t_audio_info,
                    &z_get_info_size
                    );
    if (i4_ret != SMR_OK) {
        return SVCTXR_FAIL;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_is_valid_video_fmt
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
BOOL svctx_is_valid_video_fmt(
                    SVCTX_T*                    pt_svctx
                    )
{
    if (pt_svctx->t_video_info.e_timing_type == VSH_SRC_TIMING_NOT_SUPPORT) {
        return TRUE;
    }

    return ((pt_svctx->t_video_info.ui4_width != 0
                && pt_svctx->t_video_info.ui4_height != 0) ? TRUE : FALSE);
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_is_valid_audio_fmt
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
BOOL svctx_is_valid_audio_fmt(
                    SVCTX_T*                    pt_svctx
                    )
{
    return (pt_svctx->t_audio_info.e_channels != ASH_CHANNELS_UNKNOWN ? TRUE : FALSE);
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_is_scrambled_strm
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
BOOL svctx_is_scrambled_strm(
                    SVCTX_CA_INFO_T*            pt_ca
                    )
{
    return (pt_ca->e_event == SM_EVN_CA_COND_NOT_AUTHORIZED ? TRUE : FALSE);
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_is_able_to_select_stream
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
BOOL svctx_is_able_to_select_stream(
                    SM_SESS_COND_T              e_sess_cond
                    )
{
    if (e_sess_cond != SM_SESS_COND_OPENED
            && e_sess_cond != SM_SESS_COND_OPENING
            && e_sess_cond != SM_SESS_COND_STARTING
            && e_sess_cond != SM_SESS_COND_STARTED) {
        return FALSE;
    }

    return TRUE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_find_instance
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
BOOL svctx_find_instance(
                    const CHAR*                 ps_svctx_name,     /* in  */
                    SVCTX_T**                   ppt_svctx          /* out */
                    )
{
    INT32    i4_ret   = 0;
    SVCTX_T* pt_svctx = NULL;

    DLIST_FOR_EACH(pt_svctx, &t_g_using_svctx_q, t_dlink) {
        i4_ret = x_strcmp(pt_svctx->s_name, ps_svctx_name);
        if (i4_ret == 0) {
            *ppt_svctx = pt_svctx;
            return TRUE;
        }
    }
    return (FALSE);
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_get_running_stream_handle
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 svctx_get_running_stream_handle(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_stream_type,
                    HANDLE_T*                       ph_stream
                    )
{
    SVCTX_T* pt_svctx;
    BOOL     b_ret;
    INT32    i4_ret = SVCTXR_OK;

    i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    svctx_class_lock();

    do {
        if (pt_svctx->pt_running_svc_req == NULL
                || pt_svctx->pt_running_svc_req->pt_hdlr == NULL
                || pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_stream_handle == NULL) {
            i4_ret = SVCTXR_NOT_EXIST;
            break;
        }

        b_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_stream_handle(
                    pt_svctx->pt_running_svc_req,
                    e_stream_type,
                    ph_stream
                    );
        i4_ret = b_ret == TRUE ? SVCTXR_OK : SVCTXR_NOT_EXIST;
    } while (0);

    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_get_vbi_broker
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 svctx_get_vbi_broker(
                    const SVCTX_SM_BROKER_CMD_T*    pt_cmd_sets,
                    SVCTX_T**                       ppt_svctx,
                    SVCTX_VBI_BROKER_T*             pt_vbi_broker
                    )
{
    SVCTX_T* pt_svctx = NULL;
    INT32    i4_ret   = SVCTXR_NOT_EXIST;

    if (ppt_svctx == NULL || pt_vbi_broker == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();

    *ppt_svctx = NULL;

    DLIST_FOR_EACH(pt_svctx, &t_g_using_svctx_q, t_dlink) {
        if (!_IN_RUNNING_STATE(pt_svctx->e_state)) {
            continue;
        }

#if 1 /* Xia 090603 CR00136319 */
#ifdef MW_TV_AV_BYPASS_SUPPORT
        if (IS_SVCTX_ASSOCIATE(pt_svctx)) continue;
#endif
#endif

        if (pt_svctx->pt_running_svc_req != NULL
                && pt_svctx->pt_running_svc_req->pt_hdlr != NULL
                && pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_vbi_broker != NULL
                && pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_vbi_broker(
                                pt_svctx,
                                pt_svctx->pt_running_svc_req,
                                pt_cmd_sets,
                                pt_vbi_broker
                                ) == TRUE) {
            *ppt_svctx = pt_svctx;
            i4_ret = SVCTXR_OK;
            break;
        }

        if (pt_svctx->pt_pending_svc_req != NULL
                && pt_svctx->pt_pending_svc_req->pt_hdlr != NULL
                && pt_svctx->pt_pending_svc_req->pt_hdlr->t_fcts.pf_get_vbi_broker != NULL
                && pt_svctx->pt_pending_svc_req->pt_hdlr->t_fcts.pf_get_vbi_broker(
                                pt_svctx,
                                pt_svctx->pt_pending_svc_req,
                                pt_cmd_sets,
                                pt_vbi_broker
                                ) == TRUE) {
            *ppt_svctx = pt_svctx;
            i4_ret = SVCTXR_OK;
            break;
        }
    }

    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_free_object
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
VOID svctx_free_object(
                    SVCTX_T*                    pt_svctx
                    )
{
    if (pt_svctx == NULL) {
        return;
    }

    if (pt_svctx->e_state == SVCTX_COND_INVALID) {
        return;
    }

    pt_svctx->s_name[0] = '\0';
    pt_svctx->e_state   = SVCTX_COND_INVALID;

    svctx_close_snk_scc_comps(pt_svctx);

    if (x_handle_valid(pt_svctx->h_timer) == TRUE) {
        x_timer_delete(pt_svctx->h_timer);
    }

    if (x_handle_valid(pt_svctx->h_video_mute_chk_timer) == TRUE) {
        x_timer_delete(pt_svctx->h_video_mute_chk_timer);
    }

    if (x_handle_valid(pt_svctx->h_scdb_chk_timer) == TRUE) {
        x_timer_delete(pt_svctx->h_scdb_chk_timer);
    }

    pt_svctx->h_timer                = NULL_HANDLE;
    pt_svctx->h_video_mute_chk_timer = NULL_HANDLE;
    pt_svctx->h_scdb_chk_timer       = NULL_HANDLE;

    if (pt_svctx->pt_running_svc_req != NULL) {
        svctx_free_svc_req(pt_svctx->pt_running_svc_req);
        pt_svctx->pt_running_svc_req = NULL;
    }

    DLIST_REMOVE(pt_svctx, &t_g_using_svctx_q, t_dlink);
    DLIST_INSERT_HEAD(pt_svctx, &t_g_pooled_svctx_q, t_dlink);
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_notify_clients_strm_msg
 * Description
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
VOID svctx_notify_clients_strm_msg(
                    SVCTX_T*                        pt_svctx,
                    SVCTX_NTFY_CODE_T               e_code,
                    STRM_REQ_T*                     pt_strm_req
                    )
{
    STREAM_TYPE_T e_strm_type;

    if (pt_svctx == NULL || pt_strm_req == NULL) {
        return;
    }

    if (SVCTX_NTFY_CODE_INTERNAL_ERROR == e_code) {
        DBG_INFO((_INFO_HEADER"got INTERNAL_ERROR!\r\n"));
    }

    e_strm_type = svctx_get_strm_req_strm_type(pt_strm_req);

    switch (e_code) {
    case SVCTX_NTFY_CODE_STREAM_STOPPED:
        if (pt_strm_req->t_stop_cb.pf_nfy != NULL) {
            pt_strm_req->t_stop_cb.pf_nfy(pt_strm_req->t_stop_cb.pv_tag);
            pt_strm_req->t_stop_cb.pf_nfy = NULL;
            pt_strm_req->t_stop_cb.pv_tag = NULL;
        }

        switch(e_strm_type) {
        case ST_VIDEO:
        case ST_AUDIO:
            if ((pt_svctx->b_in_stopping == TRUE) && (!pt_strm_req->b_aux_audio)){
                /* if in stopping or change to next svc, don't send out the message */
                return;
            }
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    if (!pt_strm_req->b_aux_audio)
    {
        svctx_cli_print_nfy(pt_svctx, e_code, e_strm_type);

        if (pt_strm_req->pf_strm_nfy != NULL) {
            pt_strm_req->pf_strm_nfy(
                        pt_strm_req->h_svctx,
                        pt_svctx->e_state,
                        e_code,
                        e_strm_type,
                        pt_strm_req->pv_strm_nfy_tag
                        );
        }

        _svctx_invoke_listener_nfy_fct(
                        pt_svctx,
                        e_code,
                        e_strm_type,
                        pt_strm_req->h_svctx,
                        pt_strm_req->pf_strm_nfy
#ifdef LINUX_TURNKEY_SOLUTION
                        , pt_strm_req->pv_strm_nfy_tag
#endif
                        );
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_notify_clients
 * Description
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
VOID svctx_notify_clients_svc_msg(
                    SVCTX_T*                        pt_svctx,
                    SVCTX_NTFY_CODE_T               e_code,
                    STREAM_TYPE_T                   e_stream_type,
                    HANDLE_T                        h_crnt_svctx,
                    x_svctx_select_nfy_fct          pf_crnt_select_nfy_fct
                    )
{
    SVC_AV_STATUS_T e_old_av_status = pt_svctx->e_av_status;

    if (SVCTX_COND_PRESENTING_SELECT_SAME_REQ == pt_svctx->e_state)
    {
        pt_svctx->e_av_status = SVC_AV_STATUS_UNKNOWN;
    }

    switch(e_code) {
    case SVCTX_NTFY_CODE_SIGNAL_LOSS:
        svctx_set_av_status(pt_svctx, SVC_AV_STATUS_UNKNOWN);
        break;
    case SVCTX_NTFY_CODE_AUDIO_VIDEO_SVC:
        if (pt_svctx->b_in_stopping == TRUE) {
            /* if in stopping or change to next svc, don't send out the message */
            return;
        }
        svctx_set_av_status(pt_svctx, SVC_AV_STATUS_AUDIO_VIDEO);
        svctx_mute_video(pt_svctx, FALSE);
        break;
    case SVCTX_NTFY_CODE_VIDEO_ONLY_SVC:
        if (pt_svctx->b_in_stopping == TRUE) {
            /* if in stopping or change to next svc, don't send out the message */
            return;
        }
        svctx_set_av_status(pt_svctx, SVC_AV_STATUS_VIDEO_ONLY);
        svctx_mute_video(pt_svctx, FALSE);
        break;
    case SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_CLEAR_VIDEO_SVC:
        if (pt_svctx->b_in_stopping == TRUE) {
            /* if in stopping or change to next svc, don't send out the message */
            return;
        }
        svctx_set_av_status(pt_svctx, SVC_AV_STATUS_SCRAMBLED_AUDIO_CLEAR_VIDEO);
        svctx_mute_video(pt_svctx, FALSE);
        break;
    case SVCTX_NTFY_CODE_AUDIO_ONLY_SVC:
        if (pt_svctx->b_in_stopping == TRUE) {
            /* if in stopping or change to next svc, don't send out the message */
            return;
        }
        svctx_set_av_status(pt_svctx, SVC_AV_STATUS_AUDIO_ONLY);
        svctx_mute_video(pt_svctx, pt_svctx->t_video_mute.b_enable);
        break;
    case SVCTX_NTFY_CODE_NO_AUDIO_VIDEO_SVC:
        if (pt_svctx->b_in_stopping == TRUE) {
            /* if in stopping or change to next svc, don't send out the message */
            return;
        }
        svctx_set_av_status(pt_svctx, SVC_AV_STATUS_NO_AUDIO_VIDEO);
        svctx_mute_video(pt_svctx, pt_svctx->t_video_mute.b_enable);
        break;
    case SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_VIDEO_SVC:
        if (pt_svctx->b_in_stopping == TRUE) {
            /* if in stopping or change to next svc, don't send out the message */
            return;
        }
        svctx_set_av_status(pt_svctx, SVC_AV_STATUS_SCRAMBLED_AUDIO_VIDEO);
        svctx_mute_video(pt_svctx, pt_svctx->t_video_mute.b_enable);
        break;
    case SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_NO_VIDEO_SVC:
        if (pt_svctx->b_in_stopping == TRUE) {
            /* if in stopping or change to next svc, don't send out the message */
            return;
        }
        svctx_set_av_status(pt_svctx, SVC_AV_STATUS_SCRAMBLED_AUDIO_NO_VIDEO);
        svctx_mute_video(pt_svctx, pt_svctx->t_video_mute.b_enable);
        break;
    case SVCTX_NTFY_CODE_SCRAMBLED_VIDEO_CLEAR_AUDIO_SVC:
        if (pt_svctx->b_in_stopping == TRUE) {
            /* if in stopping or change to next svc, don't send out the message */
            return;
        }
        svctx_set_av_status(pt_svctx, SVC_AV_STATUS_SCRAMBLED_VIDEO_CLEAR_AUDIO);
        svctx_mute_video(pt_svctx, pt_svctx->t_video_mute.b_enable);
        break;
    case SVCTX_NTFY_CODE_SCRAMBLED_VIDEO_NO_AUDIO_SVC:
        if (pt_svctx->b_in_stopping == TRUE) {
            /* if in stopping or change to next svc, don't send out the message */
            return;
        }
        svctx_set_av_status(pt_svctx, SVC_AV_STATUS_SCRAMBLED_VIDEO_NO_AUDIO);
        svctx_mute_video(pt_svctx, pt_svctx->t_video_mute.b_enable);
        break;
    default:
        if (SVCTX_COND_PRESENTING_SELECT_SAME_REQ == pt_svctx->e_state)
        {
            svctx_set_av_status(pt_svctx, e_old_av_status);
        }
        break;
    }

    if (_svctx_is_duplicated_nfy(pt_svctx, e_code, e_stream_type))
    {
        return;
    }

    if (e_code == SVCTX_NTFY_CODE_AUDIO_VIDEO_SVC ||
        e_code == SVCTX_NTFY_CODE_VIDEO_ONLY_SVC ||
        e_code == SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_CLEAR_VIDEO_SVC)
    {
        if (pt_svctx->b_freezed && pt_svctx->e_default_vid_mode != SCC_VID_BLANK)
        {
            SCC_VID_SUPER_FREEZE_T t_super_freeze = {SCC_VID_SUPER_FREEZE_TYPE_FORCE, FALSE};
            x_scc_vid_set_super_freeze(pt_svctx->h_video_scc_comp, &t_super_freeze);
            x_scc_vid_set_force_unmute(pt_svctx->h_video_scc_comp, SCC_VID_FORCE_UNMUTE_TYPE_ATV_CHG_CH, FALSE);
            pt_svctx->b_freezed = FALSE;
            x_dbg_stmt("[%s][%s][%d] unfreezed !!!!!!!!\r\n", pt_svctx->s_name, __FILE__, __LINE__);
            svctx_set_video_plane(pt_svctx, pt_svctx->e_default_vid_mode);
        }
    }
        

    svctx_cli_print_nfy(pt_svctx, e_code, e_stream_type);

    _svctx_invoke_owner_svc_nfy_fct(
                    pt_svctx,
                    e_code,
                    e_stream_type
                    );
    _svctx_invoke_listener_nfy_fct(
                    pt_svctx,
                    e_code,
                    e_stream_type,
                    h_crnt_svctx,
                    pf_crnt_select_nfy_fct
#ifdef LINUX_TURNKEY_SOLUTION
                    , pt_svctx->pt_running_svc_req != NULL ?
                    pt_svctx->pt_running_svc_req->pv_svc_nfy_tag : NULL
#endif
                    );
}
/*-----------------------------------------------------------------------------
 * Name: svctx_set_video_plane
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *-----------------------------------------------------------------------------*/
INT32 svctx_set_video_plane(
                    SVCTX_T*                        pt_svctx,
                    SCC_VID_MODE_T                  e_vid_mode
                    )
{
    INT32   i4_ret = SVCTXR_OK;

    if (x_handle_valid(pt_svctx->h_video_scc_comp) == FALSE) {
        return i4_ret;
    }

    if (pt_svctx->pt_running_svc_req == NULL) {
        return i4_ret;
    }

    /* just for fix CR:DTV353351 The DTV have a flash after video freeze and exit POP mode */
    #if 0    
    if (pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_VIDEO) {
        e_vid_mode = SCC_VID_FREEZE;
    }
    #else
    if((e_vid_mode != SCC_VID_FREEZE) && 
       (pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_VIDEO)){
        pt_svctx->ui4_frz_frozen_strm_mask = 0;  
        svctx_notify_clients_svc_msg(pt_svctx, SVCTX_NTFY_CODE_UNFREEZE, 
                                     ST_UNKNOWN,
                                     pt_svctx->pt_running_svc_req->h_svctx,
                                     pt_svctx->pt_running_svc_req->pf_svc_nfy); 
    }
    #endif

    if (pt_svctx->t_video_info.e_timing_type == VSH_SRC_TIMING_NOT_SUPPORT
            && e_vid_mode != SCC_VID_BLANK) {
        return SVCTXR_OK;
    }

    if (e_vid_mode == SCC_VID_BLANK && pt_svctx->b_enable_snow == TRUE) {
        e_vid_mode = pt_svctx->e_default_vid_mode;
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (pt_svctx->b_fake_disc &&
        ((svctx_mgr_convert_to_svc_type(pt_svctx->pt_running_svc_req->e_src_type) == SVC_SELECT_TV) ||
         (svctx_mgr_convert_to_svc_type(pt_svctx->pt_running_svc_req->e_src_type) == SVC_SELECT_AV)))
    {
        e_vid_mode = SCC_VID_BLANK;
    }
#endif
    
    /* A tricky solution for DTV00095949 */
    if (pt_svctx->pt_running_svc_req && pt_svctx->pt_running_svc_req->e_src_type == SRC_TYPE_AVC)
    {         
        if (e_vid_mode != SCC_VID_BLANK )
        {
            i4_ret = x_scc_vid_set_scr_mode(
                    pt_svctx->h_video_scc_comp,
                    pt_svctx->t_mheg_scr_mode.b_enable?pt_svctx->t_mheg_scr_mode.e_scr_mode:pt_svctx->e_scr_mode
                    );
        }
    }
    else /* Common flow */
    {
        if (e_vid_mode != SCC_VID_BLANK && svctx_is_no_video(pt_svctx) == FALSE &&
            pt_svctx->e_av_status != SVC_AV_STATUS_UNKNOWN)
        {
            i4_ret = x_scc_vid_set_scr_mode(
                    pt_svctx->h_video_scc_comp,
                    pt_svctx->t_mheg_scr_mode.b_enable?pt_svctx->t_mheg_scr_mode.e_scr_mode:pt_svctx->e_scr_mode
                    );
        }
    }

    if (i4_ret == SVCTXR_OK)
    {
#if 1  /* for MW_NINJA_CHANGE_CHANNEL */
        if (pt_svctx->pt_running_svc_req != NULL && pt_svctx->pt_running_svc_req->e_src_type == SRC_TYPE_BRDCST_SVL)
        {
            CH_SVC_REQ_T* pt_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
            if (e_vid_mode == SCC_VID_BLANK && pt_req->t_src_info.b_ninja_change)
            {
                e_vid_mode = SCC_VID_NORMAL;
            }
        }

        if (pt_svctx->pt_pending_svc_req != NULL && pt_svctx->pt_pending_svc_req->e_src_type == SRC_TYPE_BRDCST_SVL)
        {
            CH_SVC_REQ_T* pt_req = (CH_SVC_REQ_T*)pt_svctx->pt_pending_svc_req;
            if (e_vid_mode == SCC_VID_BLANK && pt_req->t_src_info.b_ninja_change)
            {
                e_vid_mode = SCC_VID_NORMAL;
            }
        }
#endif
        if (x_handle_valid(pt_svctx->h_video_scc_comp))
        {
            static UINT32   ui4_last_set_time = 0;
            if (e_vid_mode == SCC_VID_FREEZE &&
                x_os_get_sys_tick() * x_os_get_sys_tick_period()
                - ui4_last_set_time < 200)
            {
                x_thread_delay(120);
            }
            ui4_last_set_time = x_os_get_sys_tick() * x_os_get_sys_tick_period();
        }

#ifdef MW_PIP_POP_SUPPORT
#ifdef MW_TV_AV_BYPASS_SUPPORT
        if (NULL_HANDLE != pt_svctx->h_video_scc_comp)
#endif
        {
            SCC_VID_MODE_EX_T       t_mode;

            t_mode.e_mode = e_vid_mode;
            t_mode.t_disp_region = pt_svctx->t_disp_region;
            svctx_cli_print_video_region(pt_svctx, e_vid_mode, __func__, __LINE__);
            i4_ret = x_scc_vid_set_mode_ex(
                            pt_svctx->h_video_scc_comp,
                            &t_mode
                            );
        }
#else
        i4_ret = x_scc_vid_set_mode(
                        pt_svctx->h_video_scc_comp,
                        e_vid_mode
                        );
#endif
    }

    return (i4_ret == SMR_OK) ? SVCTXR_OK : SVCTXR_FAIL;
}
/*-----------------------------------------------------------------------------
 * Name: svctx_is_no_video
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *-----------------------------------------------------------------------------*/
BOOL svctx_is_no_video(
                    SVCTX_T*                    pt_svctx
                    )
{
    BOOL b_ret = FALSE;

    if (_IN_RUNNING_STATE(pt_svctx->e_state)) {
        if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS
                || pt_svctx->b_is_blocked == TRUE
                || pt_svctx->t_video_info.e_timing_type == VSH_SRC_TIMING_NOT_SUPPORT
#ifdef MW_TV_AV_BYPASS_SUPPORT
                || (pt_svctx->b_fake_disc     /* for DTV00207734, DTV00138791 */
#ifdef MW_PIP_POP_SUPPORT                
                    && (svctx_get_scrn_status(pt_svctx)== SVCTX_SCREEN_STATUS_SINGLE)
#endif
                 )
#endif
                ) {
            return TRUE;
        }

        switch (pt_svctx->e_av_status) {
        case SVC_AV_STATUS_AUDIO_ONLY:
        case SVC_AV_STATUS_NO_AUDIO_VIDEO:
        case SVC_AV_STATUS_SCRAMBLED_AUDIO_VIDEO:
        case SVC_AV_STATUS_SCRAMBLED_AUDIO_NO_VIDEO:
        case SVC_AV_STATUS_SCRAMBLED_VIDEO_CLEAR_AUDIO:
        case SVC_AV_STATUS_SCRAMBLED_VIDEO_NO_AUDIO:
            /* no video */
            b_ret = TRUE;
            break;
        default:
            break;
        }
    } else {
        b_ret = TRUE;
    }

    return b_ret;
}
/*-----------------------------------------------------------------------------
 * Name: svctx_mute_video
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *-----------------------------------------------------------------------------*/
INT32 svctx_mute_video (
                    SVCTX_T*                    pt_svctx,
                    BOOL                        b_enable
                    )
{
    if (b_enable == FALSE
            && pt_svctx->b_enable_snow == FALSE
            && pt_svctx->t_video_mute.b_enable == TRUE
            && pt_svctx->b_enable_video_mute_supervisor == TRUE
            && svctx_is_no_video(pt_svctx) == TRUE) {
        b_enable = TRUE;
    }

    return _svctx_force_mute_video(pt_svctx, (b_enable ? VIDEO_MUTE_CTRL_ENABLE : VIDEO_MUTE_CTRL_DISABLE));
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_update_src_snk_names
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
VOID svctx_update_src_snk_names(
                    SVCTX_T*                    pt_svctx,
                    CHAR*                       ps_src_grp_name,
                    CHAR*                       ps_snk_grp_name
                    )
{
    if (x_strlen(ps_snk_grp_name) > 0
            && x_strcmp(pt_svctx->s_snk_grp_name, ps_snk_grp_name) != 0) {
        x_strcpy(pt_svctx->s_snk_grp_name, ps_snk_grp_name);
    }

    if (x_strlen(ps_src_grp_name) > 0
            && x_strcmp(pt_svctx->s_src_grp_name, ps_src_grp_name) != 0) {
        x_strcpy(pt_svctx->s_src_grp_name, ps_src_grp_name);
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_get_using_head
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
SVCTX_T* svctx_get_using_head(
                    VOID
                    )
{
    return DLIST_HEAD(&t_g_using_svctx_q);
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_get_pooled_head
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
SVCTX_T* svctx_get_pooled_head(
                    VOID
                    )
{
    return DLIST_HEAD(&t_g_pooled_svctx_q);
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_next
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
SVCTX_T* svctx_next(
                    SVCTX_T*                    pt_svctx
                    )
{
    if (pt_svctx == NULL) {
        return NULL;
    }

    return (DLIST_NEXT(pt_svctx, t_dlink));
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_print_self
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
VOID svctx_print_self(
                    SVCTX_T*                    pt_svctx
                    )
{
#ifdef CLI_LVL_ALL
    SCC_VID_MODE_T e_crnt_vid_mode = SCC_VID_NORMAL;
    SCC_AUD_MTS_T  e_aud_mts       = SCC_AUD_MTS_UNKNOWN;

    x_scc_vid_get_mode(pt_svctx->h_video_scc_comp, &e_crnt_vid_mode);
    x_scc_aud_get_mts(pt_svctx->h_audio_scc_comp, &e_aud_mts);

    x_dbg_stmt("------------------------------------ size = %d\r\n", sizeof(SVCTX_T));
        x_dbg_stmt("pt_svctx                - 0x%x\r\n", pt_svctx);
    if (pt_svctx != NULL) {
        x_dbg_stmt("s_name                  - %s\r\n", pt_svctx->s_name);
        x_dbg_stmt("e_state                 - %s\r\n", s_g_state_strings[pt_svctx->e_state]);
        x_dbg_stmt("s_src_grp_name          - %s\r\n", pt_svctx->s_src_grp_name);
        x_dbg_stmt("s_snk_grp_name          - %s\r\n", pt_svctx->s_snk_grp_name);
        x_dbg_stmt("s_snk_grp_name_aux      - %s\r\n", pt_svctx->s_snk_grp_name_aux);
        x_dbg_stmt("h_svctx_owner           - %d\r\n", pt_svctx->h_svctx_owner);
        x_dbg_stmt("pt_running_svc_req      - 0x%x\r\n", pt_svctx->pt_running_svc_req);
        x_dbg_stmt("pt_pending_svc_req      - 0x%x\r\n", pt_svctx->pt_pending_svc_req);
        x_dbg_stmt("e_default_vid_mode      - %s\r\n", s_g_vid_modes[pt_svctx->e_default_vid_mode]);
        x_dbg_stmt("e_crnt_vid_mode         - %s\r\n", s_g_vid_modes[e_crnt_vid_mode]);
        x_dbg_stmt("ui4_frz_frozen_strm_mask- 0x%x\r\n", pt_svctx->ui4_frz_frozen_strm_mask);
        x_dbg_stmt("video_mute_supervisor   - %s\r\n", (pt_svctx->b_enable_video_mute_supervisor == TRUE ? "Y" : "N"));
        x_dbg_stmt("b_able_set_pmx_bg       - %s\r\n", (pt_svctx->b_able_set_pmx_bg == TRUE ? "Y" : "N"));
        x_dbg_stmt("t_video_mute.b_enable   - %s\r\n", (pt_svctx->t_video_mute.b_enable == TRUE ? "Y" : "N"));
        x_dbg_stmt("t_block_mute.b_enable   - %s\r\n", (pt_svctx->t_block_mute.b_enable == TRUE ? "Y" : "N"));
        x_dbg_stmt("b_block_notified        - %s\r\n", (pt_svctx->b_block_notified == TRUE ? "Y" : "N"));
        x_dbg_stmt("e_cc_avil_cond          - %s\r\n", (pt_svctx->e_cc_avil_cond == SVCTX_CC_AVIL_COND_UNKNOW ? "?" : (pt_svctx->e_cc_avil_cond == SVCTX_CC_AVIL_COND_YES ? "Y" : "N")));
        x_dbg_stmt("b_is_blocked            - %s\r\n", (pt_svctx->b_is_blocked == TRUE ? "Y" : "N"));
#ifdef MW_ISDB_SUPPORT
        x_dbg_stmt("b_is_brdcsting_svc      - %s\r\n", (pt_svctx->b_is_brdcsting_svc == TRUE ? "Y" : "N"));
#endif
        x_dbg_stmt("b_signal_loss_notified  - %s\r\n", (pt_svctx->b_signal_loss_notified == TRUE ? "Y" : "N"));
        x_dbg_stmt("e_signal_status         - %s\r\n", s_g_signal_statuses[pt_svctx->e_signal_status]);
        x_dbg_stmt("b_delay_video_mute_chk  - %s\r\n", (pt_svctx->b_delay_video_mute_check == TRUE ? "Y" : "N"));
        x_dbg_stmt("signal_loss_chk_delay   - %d ms\r\n", pt_svctx->ui4_video_mute_chk_delay);
        x_dbg_stmt("b_in_closing            - %s\r\n", (pt_svctx->b_in_closing == TRUE ? "Y" : "N"));
        x_dbg_stmt("b_in_stopping           - %s\r\n", (pt_svctx->b_in_stopping == TRUE ? "Y" : "N"));
        x_dbg_stmt("b_in_fine_tuning        - %s\r\n", (pt_svctx->b_in_fine_tuning == TRUE ? "Y" : "N"));
#ifdef MW_TV_AV_BYPASS_SUPPORT
        x_dbg_stmt("b_svc_selected          - %s\r\n", (pt_svctx->b_svc_selected == TRUE ? "Y" : "N"));
        x_dbg_stmt("b_piped_stop            - %s\r\n", (pt_svctx->b_piped_stop == TRUE ? "Y" : "N"));
        x_dbg_stmt("b_fake_disc             - %s\r\n", (pt_svctx->b_fake_disc == TRUE ? "Y" : "N"));
#endif
        x_dbg_stmt("t_video_region          - (x=%d, y=%d, width=%d, height=%d)\r\n", pt_svctx->t_video_region.ui4_x, pt_svctx->t_video_region.ui4_y, pt_svctx->t_video_region.ui4_width, pt_svctx->t_video_region.ui4_height);
        x_dbg_stmt("t_disp_region           - (x=%d, y=%d, width=%d, height=%d)\r\n", pt_svctx->t_disp_region.ui4_x, pt_svctx->t_disp_region.ui4_y, pt_svctx->t_disp_region.ui4_width, pt_svctx->t_disp_region.ui4_height);
        x_dbg_stmt("t_video_plane_order     - (%s, num_layers=%d, layer=%d)\r\n", s_g_vsh_plane_order_ctrl[pt_svctx->t_video_plane_order.e_order_ctrl - VSH_PLANE_ORDER_CTRL_TOP], pt_svctx->t_video_plane_order.u.ui1_num_layers, pt_svctx->t_video_plane_order.u.ui1_layer);
        x_dbg_stmt("t_clr_sys               - %s\r\n", s_g_color_systems[pt_svctx->t_clr_sys]);
        x_dbg_stmt("ui4_stream_mode         - 0x%x\r\n", pt_svctx->ui4_stream_mode);
        x_dbg_stmt("mts                     - %s\r\n", s_g_aud_mtss[e_aud_mts]);
        x_dbg_stmt("auto color cond         - %s\r\n", s_g_auto_clr_conds[pt_svctx->e_auto_color_cond]);
        x_dbg_stmt("auto clk pos phs cond   - %s\r\n", s_g_auto_auto_clk_pos_phs_conds[pt_svctx->e_auto_clk_pos_phs_cond]);
        x_dbg_stmt("av_status               - %s\r\n", s_g_av_statuses[pt_svctx->e_av_status]);
        x_dbg_stmt("stop_audio_as_no_signal - 0x%x\r\n", pt_svctx->ui1_stop_audio_as_no_signal);
        x_dbg_stmt("enable_block_bias_freq  - %s\r\n", (pt_svctx->b_enable_block_bias_freq == TRUE ? "Y" : "N"));
        x_dbg_stmt("no_signal_as_block_bias - %s\r\n", (pt_svctx->b_disable_no_signal_as_block_bias == TRUE ? "Y" : "N"));
        x_dbg_stmt("ui4_block_bias_freq     - %d\r\n", pt_svctx->ui4_block_bias_freq);
        x_dbg_stmt("block_rule              - %s\r\n", s_g_block_rules[pt_svctx->e_block_rule]);
        x_dbg_stmt("invalid_svl_nw_mask_bit - 0x%x\r\n", pt_svctx->ui4_invalid_svl_nw_mask_bit);
        x_dbg_stmt("disable_all_scart_out   - %s\r\n", (pt_svctx->b_disable_all_scart_out == TRUE ? "Y" : "N"));
#ifdef MW_TUNER_OUT_SUPPORT
        x_dbg_stmt("tuner_out.e_type        - %s\r\n", s_g_tuner_out_types[t_g_tuner_out.e_type]);
        if (t_g_tuner_out.e_type == TUNER_OUT_TYPE_ENABLE) {
            x_dbg_stmt("tuner_out.ui2_svl_id    - %d\r\n", t_g_tuner_out.t_info.ui2_svl_id);
            x_dbg_stmt("tuner_out.ui2_svl_rec_id- %d\r\n", t_g_tuner_out.t_info.t_svl_rec.ui2_svl_rec_id);
            x_dbg_stmt("tuner_out.ui2_tsl_id    - %d\r\n", t_g_tuner_out.t_info.t_svl_rec.uheader.t_rec_hdr.ui2_tsl_id);
            x_dbg_stmt("tuner_out.ui2_tsl_rec_id- %d\r\n", t_g_tuner_out.t_info.t_svl_rec.uheader.t_rec_hdr.ui2_tsl_rec_id);
        }
#endif
        x_dbg_stmt("e_scr_mode              - %s\r\n", s_g_scrn_modes[pt_svctx->e_scr_mode]);
        x_dbg_stmt("detection_mode          - %s\r\n", s_g_detection_modes[pt_svctx->e_detection_mode]);
        x_dbg_stmt("t_audio_info            - fmt=%s, CHs=%s, srate=%d, drate=%d, bit depth=%d\r\n",
                                             s_g_audio_fmts[pt_svctx->t_audio_info.e_aud_fmt],
                                             s_g_audio_channels[pt_svctx->t_audio_info.e_channels],
                                             pt_svctx->t_audio_info.ui4_sample_rate,
                                             pt_svctx->t_audio_info.ui4_data_rate,
                                             pt_svctx->t_audio_info.ui1_bit_depth
                                             );
        x_dbg_stmt("t_video_info            - %d x %d (%s/%s/%s) frame rate=%d\r\n",
                                             pt_svctx->t_video_info.ui4_width,
                                             pt_svctx->t_video_info.ui4_height,
                                             pt_svctx->t_video_info.b_is_progressive == TRUE ? "p" : "i",
                                             s_g_aspect_ratios[pt_svctx->t_video_info.e_src_asp_ratio],
                                             s_g_timing_types[pt_svctx->t_video_info.e_timing_type],
                                             pt_svctx->t_video_info.ui4_frame_rate
                                             );
        x_dbg_stmt("e_audio_decode_status   - %s\r\n", s_audio_decode_statuses[pt_svctx->e_audio_decode_status]);
        x_dbg_stmt("e_video_decode_status   - %s\r\n", s_video_decode_statuses[pt_svctx->e_video_decode_status]);
        x_dbg_stmt("signal_loss_show_snow   - %s\r\n", pt_svctx->b_analog_signal_loss_show_snow == TRUE ? "Y" : "N");
#ifdef MW_TV_AV_BYPASS_SUPPORT
        x_dbg_stmt("i1_prio_diff            - %d\r\n", pt_svctx->i1_prio_diff);
        x_dbg_stmt("ui4_attribute           - 0x%x\r\n", pt_svctx->ui4_attribute);
#ifdef MW_PIP_POP_SUPPORT
        x_dbg_stmt("ui4_assc_state          - 0x%x\r\n", pt_svctx->ui4_assc_state);
#else
        x_dbg_stmt("b_assc_enabled          - %s\r\n", (pt_svctx->b_assc_enabled == TRUE ? "Y" : "N"));
#endif
        x_dbg_stmt("copy_protection         - mv=0x%x/0x%x, hdcp=0x%x/0x%x, ca=0x%x/0x%x\r\n",
                                            pt_svctx->t_cp_mask.ui4_macrovision, pt_svctx->t_cp_status.ui4_macrovision,
                                            pt_svctx->t_cp_mask.ui4_hdcp, pt_svctx->t_cp_status.ui4_hdcp,
                                            pt_svctx->t_cp_mask.ui4_ca, pt_svctx->t_cp_status.ui4_ca);
#endif
    }
    x_dbg_stmt("====================================\r\n");
#endif
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_print_strm_req
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
VOID svctx_print_strm_req(
                    STRM_REQ_T*                 pt_strm_req
                    )
{
#ifdef CLI_LVL_ALL
    if (pt_strm_req != NULL && pt_strm_req->pt_hdlr != NULL && pt_strm_req->pt_hdlr->t_fcts.pf_print_strm_req != NULL)
    {
        pt_strm_req->pt_hdlr->t_fcts.pf_print_strm_req(pt_strm_req);
    }
#endif
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_print_svc_req
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
VOID svctx_print_svc_req(
                    SVC_REQ_T*                  pt_svc_req
                    )
{
#ifdef CLI_LVL_ALL
    if (pt_svc_req != NULL && pt_svc_req->pt_hdlr != NULL && pt_svc_req->pt_hdlr->t_fcts.pf_print_svc_req != NULL)
    {
        pt_svc_req->pt_hdlr->t_fcts.pf_print_svc_req(pt_svc_req);
        return;
    }

    x_dbg_stmt("------------------------------------ size = %d\r\n", sizeof(SVC_REQ_T));
    x_dbg_stmt("pt_svc_req              - 0x%x\r\n", pt_svc_req);
    if (pt_svc_req != NULL) {
        x_dbg_stmt("pt_hdlr->s_name         - %s\r\n", pt_svc_req->pt_hdlr->s_name);
        x_dbg_stmt("ui4_stream_mode         - %u\r\n", pt_svc_req->ui4_stream_mode);
        x_dbg_stmt("s_snk_grp_name          - %s\r\n", pt_svc_req->s_snk_grp_name);
        x_dbg_stmt("s_snk_grp_name_aux      - %s\r\n", pt_svc_req->s_snk_grp_name_aux);
        x_dbg_stmt("i4_speed                - %d\r\n", pt_svc_req->i4_speed);
        x_dbg_stmt("e_src_type              - %d\r\n", pt_svc_req->e_src_type);
        x_dbg_stmt("b_force_unblock         - %s\r\n", (pt_svc_req->b_force_unblock == TRUE ? "Y" : "N"));
        x_dbg_stmt("b_same_disp_region      - %s\r\n", (pt_svc_req->b_same_disp_region == TRUE ? "Y" : "N"));
        x_dbg_stmt("b_enable_signal_booster - %s\r\n", (pt_svc_req->b_enable_signal_booster == TRUE ? "Y" : "N"));
        x_dbg_stmt("b_last_signal_booster   - %s\r\n", (pt_svc_req->b_last_signal_booster == TRUE ? "Y" : "N"));
        if (pt_svc_req->t_scart_out.b_valid == TRUE) {
            UINT8 ui1_idx;
            for (ui1_idx = 0; ui1_idx < pt_svc_req->t_scart_out.ui1_num_descs; ui1_idx++) {
                x_dbg_stmt("scart out              - [%d][%s][%s]\r\n",
                                ui1_idx,
                                pt_svc_req->t_scart_out.pt_descs[ui1_idx].b_enable ? "Y" : "N",
                                pt_svc_req->t_scart_out.pt_descs[ui1_idx].s_name
                                );
            }
        }
    }
    x_dbg_stmt("====================================\r\n");
#endif
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_print_av_profile_table
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
VOID svctx_print_av_profile_table(
                    VOID
                    )
{
#ifdef CLI_LVL_ALL
    AVC_TYPE_MAPPING*      pt_avc_mapping;
    DEVICE_TYPE_PROFILE_T* pt_dev_profile;
    UINT8                  ui1_idx;
    UINT8                  ui1_av_idx  = 0;
    CHAR                   s_string[6] = {0};

    for (ui1_idx = 0; ui1_idx < (UINT8)z_g_elem_count_of_avc_table; ui1_idx++) {

        pt_avc_mapping = &pt_g_avc_mapping_table[ui1_idx];
        pt_dev_profile = &pt_avc_mapping->t_profile;

        x_dbg_stmt("[%2d] - drv_type(%3d), ", ui1_idx, pt_avc_mapping->e_drv_type);
        if (DEV_GET_GROUP_ID(pt_dev_profile->e_dev_type) == DEV_TUNER) {
            x_dbg_stmt("%-14s, ", s_g_tuner_types[DEV_GET_DEV_ID(pt_dev_profile->e_dev_type)]);
        } else if (DEV_GET_GROUP_ID(pt_dev_profile->e_dev_type) == DEV_AVC) {
            x_dbg_stmt("%-14s, ", s_g_avc_comp_types[DEV_GET_DEV_ID(pt_dev_profile->e_dev_type)]);
        } else {
            x_dbg_stmt("%-14s, ", "UNKNOWN");
        }

        ui1_av_idx = 0;
        x_dbg_stmt("class(");
        if (pt_dev_profile->ui4_av_class_mask & ST_MASK_AUDIO) {
            s_string[ui1_av_idx++] = 'A';
        }
        if (pt_dev_profile->ui4_av_class_mask & ST_MASK_VIDEO) {
            s_string[ui1_av_idx++] = 'V';
        }
        if (pt_dev_profile->ui4_av_class_mask & ST_MASK_CLOSED_CAPTION) {
            s_string[ui1_av_idx++] = 'C';
        }
        if (pt_dev_profile->ui4_av_class_mask & ST_MASK_TELETEXT) {
            s_string[ui1_av_idx++] = 'T';
        }
        if (pt_dev_profile->ui4_av_class_mask & ST_MASK_SUBTITLE) {
            s_string[ui1_av_idx++] = 'S';
        }
        s_string[ui1_av_idx] = '\0';
        x_dbg_stmt("%-6s), ", s_string);

        ui1_av_idx = 0;
        x_dbg_stmt("capability(");
        if (pt_dev_profile->ui4_strm_capable_mask & ST_MASK_AUDIO) {
            s_string[ui1_av_idx++] = 'A';
        }
        if (pt_dev_profile->ui4_strm_capable_mask & ST_MASK_VIDEO) {
            s_string[ui1_av_idx++] = 'V';
        }
        if (pt_dev_profile->ui4_strm_capable_mask & ST_MASK_CLOSED_CAPTION) {
            s_string[ui1_av_idx++] = 'C';
        }
        if (pt_dev_profile->ui4_strm_capable_mask & ST_MASK_TELETEXT) {
            s_string[ui1_av_idx++] = 'T';
        }
        if (pt_dev_profile->ui4_strm_capable_mask & ST_MASK_SUBTITLE) {
            s_string[ui1_av_idx++] = 'S';
        }
        s_string[ui1_av_idx] = '\0';
        x_dbg_stmt("%-6s), ", s_string);

        if (pt_dev_profile->i1_audio_quality_order == ORDER_UNKNOWN) {
            x_dbg_stmt("audio_quality(---), ");
        } else {
            x_dbg_stmt("audio_quality(%3d), ", pt_dev_profile->i1_audio_quality_order);
        }

        if (pt_dev_profile->i1_video_quality_order == ORDER_UNKNOWN) {
            x_dbg_stmt("video_quality(---), ");
        } else {
            x_dbg_stmt("video_quality(%3d), ", pt_dev_profile->i1_video_quality_order);
        }

        if (pt_dev_profile->t_event_capability.b_video_res_dependent == TRUE) {
            x_dbg_stmt("evn_cap(res=0x%8x, 0x%8x), ",
                        pt_dev_profile->t_event_capability.ui4_video_res_mask,
                        pt_dev_profile->t_event_capability.ui4_capable_mask
                        );
        } else {
            x_dbg_stmt("evn_cap(0x%8x), ",
                        pt_dev_profile->t_event_capability.ui4_capable_mask
                        );
        }

        if (pt_dev_profile->t_cc_capability.b_video_res_dependent == TRUE) {
            x_dbg_stmt("cc_cap(res=0x%8x)",
                        pt_dev_profile->t_cc_capability.ui4_video_res_mask
                        );
        }

        if (pt_dev_profile->t_subtitle_capability.b_video_res_dependent == TRUE) {
            x_dbg_stmt("sbtl_cap(res=0x%8x)",
                        pt_dev_profile->t_subtitle_capability.ui4_video_res_mask
                        );
        }

        if (pt_dev_profile->t_teletext_capability.b_video_res_dependent == TRUE) {
            x_dbg_stmt("tt_cap(res=0x%8x)",
                        pt_dev_profile->t_teletext_capability.ui4_video_res_mask
                        );
        }

        x_dbg_stmt("\r\n");
    }
#endif
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_get_dev_quality_orders
 * Description This API is used to get device's quality order
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 svctx_get_dev_quality_orders(
                    QUERY_TYPE_T                e_query_type,
                    DEVICE_TYPE_T               e_crnt_dev_type,
                    DEVICE_TYPE_T               e_first_dev_type,
                    INT8*                       pi1_crnt_order,
                    INT8*                       pi1_first_order
                    )
{
    DEVICE_TYPE_PROFILE_T* pt_dev_profile;
    UINT8                  ui1_idx;

    if (pi1_crnt_order == NULL || pi1_first_order == NULL) {
        return SVCTXR_INV_ARG;
    }

    for (ui1_idx = 0; ui1_idx < (UINT8) z_g_elem_count_of_avc_table; ui1_idx++) {

        pt_dev_profile = &(pt_g_avc_mapping_table[ui1_idx].t_profile);

        if (pt_dev_profile->e_dev_type == e_crnt_dev_type) {
            if (e_query_type == QUERY_TYPE_VIDEO) {
                *pi1_crnt_order = pt_dev_profile->i1_video_quality_order;
            } else if (e_query_type == QUERY_TYPE_AUDIO) {
                *pi1_crnt_order = pt_dev_profile->i1_audio_quality_order;
            }
        }

        if (pt_dev_profile->e_dev_type == e_first_dev_type) {
            if (e_query_type == QUERY_TYPE_VIDEO) {
                *pi1_first_order = pt_dev_profile->i1_video_quality_order;
            } else if (e_query_type == QUERY_TYPE_AUDIO) {
                *pi1_first_order = pt_dev_profile->i1_audio_quality_order;
            }
        }
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_with_av_capability
 * Description
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
BOOL svctx_with_av_capability(
                    DEVICE_TYPE_T               e_dev_type,
                    STREAM_TYPE_T               e_strm_type
                    )
{
    UINT8 ui1_idx;

    for (ui1_idx = 0; ui1_idx < (UINT8) z_g_elem_count_of_avc_table; ui1_idx++) {

        if (pt_g_avc_mapping_table[ui1_idx].t_profile.e_dev_type != e_dev_type) {
            continue;
        }

        if (pt_g_avc_mapping_table[ui1_idx].t_profile.ui4_strm_capable_mask & MAKE_BIT_MASK_32(e_strm_type)) {
            return TRUE;
        } else {
            return FALSE;
        }
    }

    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_with_event_capability
 * Description
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
BOOL svctx_with_event_capability(
                    SVCTX_T*                    pt_svctx,
                    DEVICE_TYPE_T               e_dev_type,
                    UINT32                      ui4_event_capability
                    )
{
    DEVICE_TYPE_PROFILE_T* pt_profile         = NULL;
    UINT32                 ui4_video_res_mask = VIDEO_RES_MASK_NONE;
    UINT8                  ui1_idx;

    if (pt_svctx == NULL) {
        return FALSE;
    }

    ui4_video_res_mask = _svctx_get_video_res_mask(&pt_svctx->t_video_info);

    for (ui1_idx = 0; ui1_idx < (UINT8) z_g_elem_count_of_avc_table; ui1_idx++) {
        pt_profile = &(pt_g_avc_mapping_table[ui1_idx].t_profile);
        if (pt_profile->e_dev_type != e_dev_type) {
            continue;
        }

        /* depend on resolution */
        if (pt_profile->t_event_capability.b_video_res_dependent == TRUE) {
            /* no matched resolution */
            if ((pt_profile->t_event_capability.ui4_video_res_mask & ui4_video_res_mask) == 0) {
                return FALSE;
            }
        }

        if(DEV_EVN_CAPABILITY_NONE == pt_profile->t_event_capability.ui4_capable_mask){
            return FALSE;
        }
        else if (DEV_EVN_CAPABILITY_ALL == pt_profile->t_event_capability.ui4_capable_mask){
           return TRUE;
        }

        if (pt_profile->t_event_capability.ui4_capable_mask & ui4_event_capability) {
            return TRUE;
        } else {
            return FALSE;
        }
    }

    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_get_conn_op
 * Description This API will invoke CM's cm_get_op()
 * Inputs:  h_svctx        Specifies the svctx handle that was
 *                         used to create the SCDB object.
 *          e_pipe_type    Additional info for specifing pipe. it could be ST_VIDEO,
 *                         ST_AUDIO, or ST_UNKNOWN
 *          e_ctrl         Contains the control code, and it is defined in CM's header file
 * Outputs: pv_ctrl_data   Contains information about the connection.
 * Returns: SVCTXR_OK            successfully
 *          SVCTXR_NO_RESOURCE   No more handle available,
 *                               or Out of memory.
 *          SVCTXR_INV_HANDLE    bad 'h_svctx' handle.
 *          SVCTXR_WRONG_STATE   connection is not available
 *          SVCTXR_FAIL          failed
 *---------------------------------------------------------------------------*/
INT32 svctx_get_conn_op(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_pipe_type,
                    CM_CTRL_TYPE_T                  e_ctrl,
                    VOID*                           pv_ctrl_data
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "e_pipe_type = %d, "
                    "e_ctrl = %d, "
                    "pv_get_data = 0x%x, "
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_pipe_type,
                    e_ctrl,
                    pv_ctrl_data
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_conn_op == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_conn_op(
                    pt_svctx,
                    e_pipe_type,
                    e_ctrl,
                    pv_ctrl_data
                    );
    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_set_conn_op
 * Description This API will invoke CM's cm_set_op()
 * Inputs:  h_svctx        Specifies the svctx handle that was
 *                         used to create the SCDB object.
 *          e_pipe_type    Additional info for specifing pipe. it could be ST_VIDEO,
 *                         ST_AUDIO, or ST_UNKNOWN
 *          e_ctrl         Contains the control code, and it is defined in CM's header file
 *          pv_ctrl_data   References a data structure which content depends on
 *                         e_ctrl.
 *
 * Outputs: pv_ctrl_data   Contains values that may have been modified.
 * Returns: SVCTXR_OK            successfully
 *          SVCTXR_NO_RESOURCE   No more handle available,
 *                               or Out of memory.
 *          SVCTXR_INV_HANDLE    bad 'h_svctx' handle.
 *          SVCTXR_WRONG_STATE   connection is not available
 *          SVCTXR_FAIL          failed
 *---------------------------------------------------------------------------*/
INT32 svctx_set_conn_op(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_pipe_type,
                    CM_CTRL_TYPE_T                  e_ctrl,
                    VOID*                           pv_ctrl_data
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "e_pipe_type = %d, "
                    "e_ctrl = %d, "
                    "pv_get_data = 0x%x, "
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_pipe_type,
                    e_ctrl,
                    pv_ctrl_data
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_set_conn_op == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_set_conn_op(
                    pt_svctx,
                    e_pipe_type,
                    e_ctrl,
                    pv_ctrl_data
                    );
    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_get_conn_handle
 * Description
 *          This API will try to find out the connection handle using in
 *          current SVCTX
 * Inputs:  h_svctx        Specifies the svctx handle that was
 *                         used to create the SCDB object.
 *          e_pipe_type    Additional info for specifing pipe. it could be ST_VIDEO,
 *                         ST_AUDIO, or ST_UNKNOWN
 * Outputs: ph_conn        Contains the connection handle used in this SVCTX
 * Returns: SVCTXR_OK            successfully
 *          SVCTXR_NO_RESOURCE   No more handle available,
 *                               or Out of memory.
 *          SVCTXR_INV_HANDLE    bad 'h_svctx' handle.
 *          SVCTXR_INV_ARG       ph_handle is NULL
 *          SVCTXR_WRONG_STATE   connection is not available
 *          SVCTXR_FAIL          failed
 *---------------------------------------------------------------------------*/
INT32 svctx_get_conn_handle(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_pipe_type,
                    HANDLE_T*                       ph_handle
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "e_pipe_type = %d, "
                    "ph_handle = %x "
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_pipe_type,
                    ph_handle
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (ph_handle == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_conn_handle == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_conn_handle(
                    pt_svctx,
                    e_pipe_type,
                    ph_handle
                    );
    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_reset_strm_req
 * Description
 *          This API will reset a STRM_REQ_T object
 * Inputs:  pt_strm_req
 * Outputs: -
 * Returns:
 *---------------------------------------------------------------------------*/
VOID svctx_reset_strm_req(
                STRM_REQ_T*                     pt_strm_req
                )
{
    if (pt_strm_req == NULL) {
        return;
    }

    pt_strm_req->h_svctx                   = NULL_HANDLE;
    pt_strm_req->h_stream                  = NULL_HANDLE;
    pt_strm_req->pf_strm_nfy               = NULL;
    pt_strm_req->pv_strm_nfy_tag           = NULL;
    pt_strm_req->e_strm_cond               = SM_COND_CLOSED;
    pt_strm_req->e_penging_status          = STRM_PENDING_COND_NONE;
    pt_strm_req->e_req_type                = STRM_REQ_SCDB;
    pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
    pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag = NULL; /* system restart when play CMPB after paly zimu bitstream */
    pt_strm_req->t_cp.ui1_ctrl_size        = 0;
    pt_strm_req->t_ca.e_event              = SM_EVN_IGNORE;
    pt_strm_req->t_ca.e_code               = POD_CA;
    pt_strm_req->pt_strm_fltr_fct          = NULL;
    pt_strm_req->pv_strm_fltr_tag          = NULL;
    pt_strm_req->t_stop_cb.pf_nfy          = NULL;
    pt_strm_req->t_stop_cb.pv_tag          = NULL;
    pt_strm_req->pt_hdlr                   = NULL;
    pt_strm_req->h_gl_plane                = NULL_HANDLE;
    pt_strm_req->t_cc_filter.e_cc_type     = SM_CC_TYPE_IGNORE;
    pt_strm_req->b_aux_audio               = FALSE;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    pt_strm_req->b_close_queued            = FALSE;
    pt_strm_req->b_open_queued             = FALSE;
#endif
    pt_strm_req->b_mix_substrm             = FALSE;
    pt_strm_req->ui1_mixed_substrm_idx     = AUDIO_INDEX_UNKNOW;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_reset_svc_req
 * Description
 *          This API will reset a SVC_REQ_T object
 * Inputs:  pt_svc_req
 * Outputs: -
 * Returns:
 *---------------------------------------------------------------------------*/
VOID svctx_reset_svc_req(
                SVC_REQ_T*                      pt_svc_req
                )
{
    if (pt_svc_req == NULL) {
        return;
    }

    pt_svc_req->h_svctx                   = NULL_HANDLE;
    pt_svc_req->pf_svc_nfy                = NULL;
    pt_svc_req->pv_svc_nfy_tag            = NULL;
    pt_svc_req->ui4_stream_mode           = 0;
    pt_svc_req->t_stop_cb.pf_nfy          = NULL;
    pt_svc_req->t_stop_cb.pv_tag          = NULL;
    pt_svc_req->b_force_unblock           = FALSE;
    pt_svc_req->s_snk_grp_name[0]         = '\0';
    pt_svc_req->i4_speed                  = 0;
    pt_svc_req->e_src_type                = 0xff;
    pt_svc_req->b_same_disp_region        = FALSE;
    pt_svc_req->t_scart_out.b_valid       = FALSE;
    pt_svc_req->t_scart_out.ui1_num_descs = 0;
    pt_svc_req->t_scart_out.pt_descs      = NULL;
    pt_svc_req->pt_hdlr                   = NULL;

#if 1 /* Xia - 090609 for Sony ATV Project */
    pt_svc_req->b_last_signal_booster = FALSE;
#endif

}


#ifdef SYS_RECORD_SUPPORT

/*-----------------------------------------------------------------------------
 * Name
 *      svctx_rec_get_crypt_info
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
VOID svctx_rec_get_crypt_info (
    MM_CRYPT_INFO_T*                pt_crypt_info)
{
    INT32           i4_ret;
    PC_DEV_ID_T     t_dev_id;
    SIZE_T          z_len_get = sizeof (PC_DEV_ID_T);
    UCHAR           uc_t;
    UINT32          ui4_i;
    UINT32          aui4_dig1[8];
    UINT32          aui4_dig2[8];

    x_memset (pt_crypt_info, 0, sizeof (MM_CRYPT_INFO_T));
    x_memset (&t_dev_id, 0, sizeof (PC_DEV_ID_T));
    x_memset (aui4_dig1, 0, sizeof (aui4_dig1));
    x_memset (aui4_dig2, 0, sizeof (aui4_dig2));
        
    i4_ret = x_rm_get_comp (DRVT_PWR_CTRL,
                            0,
                            FALSE,
                            0,
                            PC_GET_TYPE_DEV_ID,
                            (VOID *) &t_dev_id,
                            & z_len_get);
    x_dbg_stmt("\n>>>>>>>>>>>>>PC_GET_TYPE_DEV_ID return (%d)<<<<<<<<<<<<\n",i4_ret);
    if (i4_ret != RMR_OK)
    {
        x_memset (t_dev_id.aui1_dev_id, DFT_BYTE_ID, 8);
    }
    
    dlm_digest_init ();
    for (ui4_i = 0; ui4_i < 8; ui4_i ++)
    {
        uc_t = (UCHAR) (((UINT32) t_dev_id.aui1_dev_id[ui4_i]) + ((((UINT32) OFS_BYTE_ID) - ui4_i) * (ui4_i + 1)) );
        dlm_digest_update (&uc_t, 1);
    }
    dlm_digest_final ((UCHAR *) aui4_dig1);
    dlm_digest_init ();
    dlm_digest_update ((UCHAR *) aui4_dig1, 32);
    dlm_digest_final ((UCHAR *) aui4_dig2);
    
    ui4_i = 0;
    pt_crypt_info->u.t_pvr_crypt.e_mode = MM_CRYPT_MODE_3_DES_EBC;
    pt_crypt_info->u.t_pvr_crypt.aui4_odd_key_1[0]  = aui4_dig1[ui4_i++];
    pt_crypt_info->u.t_pvr_crypt.aui4_even_key_1[0] = aui4_dig1[ui4_i++];
    pt_crypt_info->u.t_pvr_crypt.aui4_odd_key_1[1]  = aui4_dig2[ui4_i++];
    pt_crypt_info->u.t_pvr_crypt.aui4_even_key_1[1] = aui4_dig2[ui4_i++];
    
    dlm_digest_init ();
    dlm_digest_update ((UCHAR *) aui4_dig2, 32);
    dlm_digest_final ((UCHAR *) aui4_dig1);
    dlm_digest_init ();
    dlm_digest_update ((UCHAR *) aui4_dig1, 32);
    dlm_digest_final ((UCHAR *) aui4_dig2);
    
    ui4_i = 0;
    pt_crypt_info->u.t_pvr_crypt.aui4_odd_key_2[0]  = aui4_dig1[ui4_i++];
    pt_crypt_info->u.t_pvr_crypt.aui4_even_key_2[0] = aui4_dig1[ui4_i++];
    pt_crypt_info->u.t_pvr_crypt.aui4_odd_key_2[1]  = aui4_dig2[ui4_i++];
    pt_crypt_info->u.t_pvr_crypt.aui4_even_key_2[1] = aui4_dig2[ui4_i++];
    pt_crypt_info->u.t_pvr_crypt.e_key_idx = MM_CRYPT_KEY_ALTERNATE;
    
}    

/*-----------------------------------------------------------------------------
 * Name
 *      svctx_rec_check_pvr_file
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 svctx_rec_check_pvr_file (
    const CHAR*                     s_file_path,
    BOOL*                           pb_pvr_file)
{
    INT32               i4_ret;
    FM_FILE_INFO_T      t_file_info;
    SIZE_T              z_path_len;
    
    *pb_pvr_file = FALSE;
    
    /* determine by sub file name in current stage */
    i4_ret = x_fm_get_info_by_name (FM_ROOT_HANDLE, 
                                    s_file_path,
                                    & t_file_info );
    if (i4_ret == FMR_OK)
    {
        z_path_len = x_strlen (s_file_path);
        
        if (z_path_len <= 4)
        {
            return SVCTXR_INV_ARG;
        }
        
        if (!x_strcasecmp (s_file_path + z_path_len - 4, ".PVR" ))
        {
            *pb_pvr_file = TRUE;
        }
    }
    else
    {
        return SVCTXR_INV_ARG;
    }
    
    return SVCTXR_OK;
}

#endif


/*-----------------------------------------------------------------------------
 * Name
 *      svctx_delay_signal_loss_nfy
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
BOOL svctx_delay_signal_loss_nfy(
                    SVCTX_T*                    pt_svctx,
                    _svctx_video_mute_nfy_fct   pf_video_mute_nfy_fct
                    )
{
    if (pt_svctx == NULL || pf_video_mute_nfy_fct == NULL) {
        return FALSE;
    }

    if (pt_svctx->b_delay_video_mute_check == FALSE) {
        DBG_INFO((_INFO_HEADER"%s() delay to send signal loss notifciation\r\n", __func__));
        pt_svctx->pf_video_mute_nfy_fct = pf_video_mute_nfy_fct;

        if (pt_svctx->pt_running_svc_req != NULL
                && pt_svctx->pt_running_svc_req->pt_hdlr->ui4_video_mute_chk_delay != 0) {
            pt_svctx->ui4_video_mute_chk_delay = pt_svctx->pt_running_svc_req->pt_hdlr->ui4_video_mute_chk_delay;
        }

        x_timer_start(
                    pt_svctx->h_video_mute_chk_timer,
                    pt_svctx->ui4_video_mute_chk_delay,
                    X_TIMER_FLAG_ONCE,
                    _svctx_delay_signal_loss_nfy_timeout_cb,
                    (VOID*)pt_svctx
                    );

        /* reset to orignal default value */
        pt_svctx->ui4_video_mute_chk_delay = DEFAULT_SIGNAL_LOSS_CHK_TIMEOUT;
        return TRUE;
    } else {
        return FALSE;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_delay_no_video_related_nfy
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
BOOL svctx_delay_no_video_related_nfy(
                    SVCTX_T*                    pt_svctx,
                    SVCTX_NTFY_CODE_T           e_code,
                    _svctx_video_mute_nfy_fct   pf_video_mute_nfy_fct
                    )
{
    if (pt_svctx == NULL || pf_video_mute_nfy_fct == NULL) {
        return FALSE;
    }

    switch(e_code) {
    case SVCTX_NTFY_CODE_AUDIO_ONLY_SVC:
    case SVCTX_NTFY_CODE_NO_AUDIO_VIDEO_SVC:
    case SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_VIDEO_SVC:
    case SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_NO_VIDEO_SVC:
    case SVCTX_NTFY_CODE_SCRAMBLED_VIDEO_CLEAR_AUDIO_SVC:
    case SVCTX_NTFY_CODE_SCRAMBLED_VIDEO_NO_AUDIO_SVC:
        pt_svctx->e_video_mute_code = e_code;
        break;
    default:
        pt_svctx->e_video_mute_code = SVCTX_NTFY_CODE_AUDIO_VIDEO_SVC;
        return FALSE;
    }

    if (pt_svctx->b_delay_video_mute_check == FALSE) {
        UINT32  ui4_timeout = DEFAULT_NO_VIDEO_CHK_TIMEOUT;
        /* for DTV00212190, set no video delay longer than the delay for
         * acquiring PAT/PMT, to avoid the case when b_reuse_pipe=TRUE &&
         * cable unplugged, NO_AUDIO_VIDEO is sent before SIGNAL_LOSS. */
        if (e_code == SVCTX_NTFY_CODE_NO_AUDIO_VIDEO_SVC
            && pt_svctx->e_video_decode_status == VSH_DIG_DECODE_UNKNOWN
            && pt_svctx->e_audio_decode_status == ASH_AUDIO_DECODE_UNKNOWN)
        {
            /* DEFAULT_NO_VIDEO_CHK_TIMEOUT: 1000ms,
             * ATSC_PAT/PMT_QUERY_TIMEOUT: 2000 ms,
             * DVB_PAT/PMT_QUERY_TIMEOUT: 3000 ms */
            ui4_timeout = ui4_timeout * 3;
        }
        else if (e_code == SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_VIDEO_SVC
                    || e_code == SVCTX_NTFY_CODE_SCRAMBLED_VIDEO_NO_AUDIO_SVC
                   /* || e_code ==  SVCTX_NTFY_CODE_SCRAMBLED_VIDEO_CLEAR_AUDIO_SVC */)
        {
            /* When scrambled video with CI card, low layer will notify 
             * SCRAMBLED then DESCRAMBLED, so we need to avoid sending 
             * scrambled video related ntfy before video is descrambled.
             */
            ui4_timeout *= 3;
        }
        /* for CR DTV00341838: "Retrieving Data..."logo display time is too 
         *  long after change channel to audio program: set delay time to
          * 1.5 * ui4_timeout other than 3 * ui4_timeout  */
        #if 1
        else if (e_code == SVCTX_NTFY_CODE_SCRAMBLED_VIDEO_CLEAR_AUDIO_SVC)
        {
            ui4_timeout +=(ui4_timeout >> 1);
        }
        #endif
        else if (pt_svctx->e_state == SVCTX_COND_PENDING)
        {
            /* When FAKE_CONNECTED, a fake VIDEO_FMT_UPDATE may be sent when
             * e_state is PENDING, which would lead to stream status check and
             * NO_AUDIO_VIDEO being delayed to sent etc. For DTV00140578, we
             * need to avoid NO_AUDIO_VIDEO being sent before video is
             * descrambled and decoded. Therefore here we increase the timeout
             * by doubling it, giving more time for descrambled and decoding.
             */
            ui4_timeout *= 2;
        }
        DBG_INFO((_INFO_HEADER"%s() delay to send no video related notifciation\r\n", __func__));
        pt_svctx->pf_video_mute_nfy_fct = pf_video_mute_nfy_fct;
        x_timer_start(
                    pt_svctx->h_video_mute_chk_timer,
                    ui4_timeout,
                    X_TIMER_FLAG_ONCE,
                    _svctx_delay_no_video_related_nfy_timeout_cb,
                    (VOID*)pt_svctx
                    );
        return TRUE;
    } else {
        return FALSE;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_cancel_no_video_related_delay_nfy
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
VOID svctx_cancel_no_video_related_delay_nfy(
                    SVCTX_T*                    pt_svctx
                    )
{
    if (pt_svctx == NULL) {
        return;
    }

    pt_svctx->b_delay_video_mute_check = FALSE;
    pt_svctx->e_video_mute_code        = SVCTX_NTFY_CODE_AUDIO_VIDEO_SVC;
    x_timer_stop(pt_svctx->h_video_mute_chk_timer);
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_set_signal_loss_chk_delay
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 svctx_set_signal_loss_chk_delay(
                    SVCTX_T*                    pt_svctx,
                    UINT32                      ui4_delay
                    )
{
    pt_svctx->ui4_video_mute_chk_delay = ui4_delay;
    return SVCTXR_OK;
}

#ifdef MW_PIP_POP_SUPPORT
SVCTX_SCREEN_STATUS_T svctx_get_scrn_status(SVCTX_T* pt_svctx)
{
#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (IS_SVCTX_ASSOCIATE(pt_svctx)) {
        SVCTX_ASSC_CFG_T*   pt_config       = NULL;
        SVCTX_T*            pt_svctx_lead   = NULL;
        pt_config = svctx_mgr_get_assc_cfg(pt_svctx);
        if (pt_config == NULL ) {
            return SVCTX_SCREEN_STATUS_UNKNOWN;
        } else {
            svctx_get_obj(pt_config->h_svctx_lead, &pt_svctx_lead);
            if (pt_svctx_lead == NULL) {
                return SVCTX_SCREEN_STATUS_UNKNOWN;
            } else {
                return pt_svctx_lead->e_scrn_status;
            }
        }
    } else
#endif
    {
        return pt_svctx->e_scrn_status;
    }
}

BOOL svctx_is_pap(SVCTX_T* pt_svctx)
{
    SVCTX_SCREEN_STATUS_T t_scrn_status = svctx_get_scrn_status(pt_svctx);
    
    return (t_scrn_status == SVCTX_SCREEN_STATUS_MULTIPLE_FULL ||
            t_scrn_status == SVCTX_SCREEN_STATUS_MULTIPLE_PART );
}
#endif


/*----------------------------------------------------------------------------
 * Name
 *      svctx_null_block_check_fct
 * Description
 *      The API is the null parental control check function. It always
 *      return SVC_BLOCK_COND_PASSED.
 * Input arguments
 *      e_check_guide       The check guide
 *      ps_src_name         The source group name for get related ISL record
 *      pt_svl_rec          The SVL record if source group name is Tuner
 *      pv_evn_rating       The event rating buffer
 * Output arguments
 *      None
 * Returns
 *      SVC_BLOCK_COND_PASSED
 *----------------------------------------------------------------------------*/
SVC_BLOCK_COND_T svctx_null_block_check_fct(
                    SVC_BLOCK_CHECK_GUDIE_T     e_check_guide,
                    const CHAR*                 ps_src_name,
                    const SVL_REC_T*            pt_svl_rec,
                    const VOID*                 pv_evn_rating
#ifdef SVCTX_SPLIT_SUPPORT
                    ,VOID*                       pv_tag
#endif
                    )
{
    return SVC_BLOCK_COND_PASSED;
}
#ifndef MW_TV_AV_BYPASS_SUPPORT
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_set_scart_out_params
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
INT32 svctx_set_scart_out_params(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_sess,
                    BOOL                        b_include_tuner_bundle,
                    UINT32                      ui4_strm_mode,
                    SVC_REQ_T*                  pt_svc_req
                    )
{
    SM_SESS_GNRC_SCART_OUT_T t_sm_scart_out;
    INT32 i4_ret;
    UINT8 ui1_idx;

    if (x_handle_valid(h_sess) == FALSE) {
        return SVCTXR_INV_HANDLE;
    }

    if (pt_svc_req == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (pt_svc_req->t_scart_out.b_valid == FALSE) {
        return SVCTXR_OK;
    }

    if (pt_svctx->b_is_blocked == TRUE) {
        return SVCTXR_OK;
    }

    for (ui1_idx = 0; ui1_idx < pt_svc_req->t_scart_out.ui1_num_descs; ui1_idx++) {
        if (b_include_tuner_bundle == FALSE) {
            if (x_strcmp(t_g_tuner_out.t_info.s_scart_out_src_name, pt_svc_req->t_scart_out.pt_descs[ui1_idx].s_name) == 0) {
                continue;
            }
        }

        t_sm_scart_out.b_set_by_name  = TRUE;
        t_sm_scart_out.u.s_scart_name = pt_svc_req->t_scart_out.pt_descs[ui1_idx].s_name;
        if (pt_svctx->b_disable_all_scart_out == TRUE) {
            t_sm_scart_out.b_enable = FALSE;
        } else {
            t_sm_scart_out.b_enable = pt_svc_req->t_scart_out.pt_descs[ui1_idx].b_enable;
        }

        i4_ret = x_sm_set(
                    h_sess,
                    SM_SESS_GNRC_SET_TYPE_SCART_OUT,
                    &t_sm_scart_out,
                    sizeof(SM_SESS_GNRC_SCART_OUT_T)
                    );
        if (i4_ret != SMR_OK) {
            DBG_ERROR((_ERROR_HEADER"x_sm_set(SCART_OUT, %s) failed, i4_ret=%d\r\n",
                    t_sm_scart_out.u.s_scart_name,
                    i4_ret
                    ));
        }
    }
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_force_disable_scart_out
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
INT32 svctx_force_disable_scart_out (
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_sess,
                    BOOL                        b_include_tuner_bundle,
                    UINT32                      ui4_strm_mode,
                    SVC_REQ_T*                  pt_svc_req
                    )
{
    SM_SESS_GNRC_SCART_OUT_T t_sm_scart_out;
    INT32 i4_ret;
    UINT8 ui1_idx;

    if (x_handle_valid(h_sess) == FALSE) {
        return SVCTXR_INV_HANDLE;
    }

    if (pt_svc_req == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (pt_svc_req->t_scart_out.b_valid == FALSE) {
        return SVCTXR_OK;
    }

    for (ui1_idx = 0; ui1_idx < pt_svc_req->t_scart_out.ui1_num_descs; ui1_idx++) {
        if (b_include_tuner_bundle == FALSE) {
            if (x_strcmp(t_g_tuner_out.t_info.s_scart_out_src_name, pt_svc_req->t_scart_out.pt_descs[ui1_idx].s_name) == 0) {
                continue;
            }
        }

        t_sm_scart_out.b_set_by_name  = TRUE;
        t_sm_scart_out.b_enable       = FALSE;
        t_sm_scart_out.u.s_scart_name = pt_svc_req->t_scart_out.pt_descs[ui1_idx].s_name;

        i4_ret = x_sm_set(
                    h_sess,
                    SM_SESS_GNRC_SET_TYPE_SCART_OUT,
                    &t_sm_scart_out,
                    sizeof(SM_SESS_GNRC_SCART_OUT_T)
                    );
        if (i4_ret != SMR_OK) {
            DBG_ERROR((_ERROR_HEADER"x_sm_set(SCART_OUT, %s) failed, i4_ret=%d\r\n",
                    t_sm_scart_out.u.s_scart_name,
                    i4_ret
                    ));
        }
    }

    return SVCTXR_OK;
}
#else
INT32 svctx_set_scart_out_params(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_sess,
                    BOOL                        b_include_tuner_bundle,
                    UINT32                      ui4_strm_mode,
                    SVC_REQ_T*                  pt_svc_req
                    )
{
    return SVCTXR_OK;
}
INT32 svctx_force_disable_scart_out (
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_sess,
                    BOOL                        b_include_tuner_bundle,
                    UINT32                      ui4_strm_mode,
                    SVC_REQ_T*                  pt_svc_req
                    )
{
    return SVCTXR_OK;
}
#endif /* MW_TV_AV_BYPASS_SUPPORT */
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_disable_analog_tuner_out
 * Description
 * Input arguments
 * Output arguments
 *      -
 * Returns
 *      -
 *-----------------------------------------------------------------------------*/
INT32 svctx_disable_analog_tuner_out(
                    SVCTX_T*                    pt_svctx,
                    BOOL                        b_disable
                    )
{
#ifdef MW_TUNER_OUT_SUPPORT
    TUNER_CTRLR_INFO_T t_tsl_info;
    INT32 i4_ret;

    if (t_g_tuner_out.e_type == TUNER_OUT_TYPE_IGONRE) {
        return SVCTXR_OK;
    }

    if (pt_svctx->b_enable_block_bias_freq == FALSE) {
        return SVCTXR_OK;
    }

    t_tsl_info.ui2_tsl_id     = t_g_tuner_out.t_info.t_svl_rec.uheader.t_rec_hdr.ui2_tsl_id;
    t_tsl_info.ui2_tsl_rec_id = t_g_tuner_out.t_info.t_svl_rec.uheader.t_rec_hdr.ui2_tsl_rec_id;
    t_tsl_info.ui4_tv_sys     = t_g_tuner_out.t_info.t_svl_rec.u_data.t_analog.ui4_tv_sys;

    i4_ret = tuner_ctrlr_set_ana_tuner(
                    &t_tsl_info,
                    b_disable,
                    pt_svctx->ui4_block_bias_freq
                    );
    if (i4_ret != SVCTXR_OK) {
        DBG_ERROR((_ERROR_HEADER"tuner_ctrlr_set_ana_tuner() failed, i4_ret=%d\r\n",
                i4_ret
                ));
        return i4_ret;
    }
#endif
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_disable_tuner_bundled_scart_out
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
INT32 svctx_disable_tuner_bundled_scart_out (
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_sess,
                    BOOL                        b_disable
                    )
{
#ifndef MW_TV_AV_BYPASS_SUPPORT
    SM_SESS_GNRC_SCART_OUT_T t_sm_scart_out;
    INT32 i4_ret;

    if (t_g_tuner_out.e_type == TUNER_OUT_TYPE_IGONRE) {
        return SVCTXR_OK;
    }

    if (x_handle_valid(h_sess) == FALSE) {
        return SVCTXR_INV_HANDLE;
    }

    if (x_strlen(t_g_tuner_out.t_info.s_scart_out_src_name) == 0) {
        return SVCTXR_OK;
    }

    if (pt_svctx->b_disable_all_scart_out == TRUE) {
        t_sm_scart_out.b_enable = FALSE;
    } else {
        t_sm_scart_out.b_enable = b_disable == TRUE ? FALSE : TRUE;
    }

    t_sm_scart_out.b_set_by_name  = TRUE;
    t_sm_scart_out.u.s_scart_name = t_g_tuner_out.t_info.s_scart_out_src_name;

    i4_ret = x_sm_set(
                    h_sess,
                    SM_SESS_GNRC_SET_TYPE_SCART_OUT,
                    &t_sm_scart_out,
                    sizeof(SM_SESS_GNRC_SCART_OUT_T)
                    );
    if (i4_ret != SMR_OK) {
        DBG_ERROR((_ERROR_HEADER"x_sm_set(SCART_OUT, %s) failed, i4_ret=%d\r\n",
                t_sm_scart_out.u.s_scart_name,
                i4_ret
                ));
        return svctx_translate_smr_2_svctxr(i4_ret);
    }
#endif
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_check_analog_tuner_out_block
 * Description
 * Input arguments
 * Output arguments
 *      -
 * Returns
 *      -
 *-----------------------------------------------------------------------------*/
INT32 svctx_check_analog_tuner_out_block(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_sess
                    )
{
    SVC_BLOCK_COND_T e_svc_block_cond = SVC_BLOCK_COND_PASSED;
    HANDLE_T         h_svl            = NULL_HANDLE;
    UINT32           ui4_ver_id       = SVL_NULL_VER_ID;
    INT32            i4_ret           = SVCTXR_OK;

    if (pt_svctx == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (t_g_tuner_out.e_type == TUNER_OUT_TYPE_IGONRE) {
        return SVCTXR_OK;
    }

    if (x_strlen(t_g_tuner_out.t_info.s_tuner_src_name) == 0) {
        return SVCTXR_OK;
    }

    do {
        i4_ret = x_svl_open(
                        t_g_tuner_out.t_info.ui2_svl_id,
                        NULL,
                        NULL,
                        &h_svl
                        );
        if (i4_ret != SVLR_OK) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = x_svl_get_rec(
                        h_svl,
                        t_g_tuner_out.t_info.t_svl_rec.ui2_svl_rec_id,
                        &(t_g_tuner_out.t_info.t_svl_rec),
                        &ui4_ver_id
                        );
        if (i4_ret != SVLR_OK) {
            i4_ret = SVCTXR_NO_RECORD;
            break;
        }

#ifdef SVCTX_SPLIT_SUPPORT
        e_svc_block_cond = pt_svctx->pf_block_check_fct(
                        SVC_BLOCK_CHECK_USER_BLOCK,
                        t_g_tuner_out.t_info.s_tuner_src_name,
                        &(t_g_tuner_out.t_info.t_svl_rec),
                        NULL,
                        pt_svctx->pf_block_check_fct_tag
                        );
#else
        e_svc_block_cond = pt_svctx->pf_block_check_fct(
                        SVC_BLOCK_CHECK_USER_BLOCK,
                        t_g_tuner_out.t_info.s_tuner_src_name,
                        &(t_g_tuner_out.t_info.t_svl_rec),
                        NULL
                        );
#endif
        if (e_svc_block_cond ==  SVC_BLOCK_COND_PASSED
                || e_svc_block_cond == SVC_BLOCK_COND_PASSED_AND_NO_RATING_SET) {
            svctx_disable_analog_tuner_out(pt_svctx, FALSE);
            svctx_disable_tuner_bundled_scart_out(pt_svctx, h_sess, FALSE);
        } else {
            svctx_disable_analog_tuner_out(pt_svctx, TRUE);
            svctx_disable_tuner_bundled_scart_out(pt_svctx, h_sess, TRUE);
        }

        i4_ret = SVCTXR_OK;
    } while (0);

    if (x_handle_valid(h_svl) == TRUE) {
        x_svl_close(h_svl);
    }

    return i4_ret;
}

#ifdef MW_TV_AV_BYPASS_SUPPORT
INT32 svctx_handle_action_query(
    SVCTX_T*        pt_svctx,
    PLAYER_T*       pt_player,
    PLAYER_TYPE_T   e_player_type,
    SVCTX_MSG_T*    pt_msg)
{
    INT32       i4_ret = SVCTXR_OK;
    VAR_INFO_T  t_var_info;

    t_var_info.ui1_msg_id                    = pt_msg->ui1_msg_id;
    t_var_info.e_var_type                     = VAR_TYPE_PLAYER;
    t_var_info.u.t_player_info.e_player_type  = e_player_type;
    t_var_info.u.t_player_info.pt_player      = pt_player;
    t_var_info.u.t_player_info.pf_invoke_func = pt_msg->pf_invoke_func;

    switch (pt_player->e_phase)
    {
        case PLAYER_ACTION_PIPE_OPEN:
        case PLAYER_ACTION_CONN_OPEN:
        case PLAYER_ACTION_SESS_PLAY:
        case PLAYER_ACTION_SESS_OPEN:
            t_var_info.e_op_type = OP_TYPE_OPEN;
            break;

        case PLAYER_ACTION_PIPE_CLOSE:
        case PLAYER_ACTION_CONN_CLOSE:
        case PLAYER_ACTION_SESS_CLOSE:
            t_var_info.e_op_type = OP_TYPE_CLOSE;
            break;

        default:
            return SVCTXR_OK;
    }

    if (FALSE == svctx_mgr_ready_to_proceed(
        ACTIVE_LINK_NOW,
        pt_svctx->pt_running_svc_req->e_src_type,
        pt_svctx,
        &t_var_info))
    {
#ifdef CLI_LVL_ALL
        DBG_SVCTXMGR_MSG((
            "{SVCTX_MGR} SVCTX(%s): Action (%s) POSTPONED...\r\n",
            pt_svctx->s_name,
            s_g_player_action[pt_player->e_phase]));
#endif
        i4_ret = SVCTXR_QUEUED;
    }
    else
    {
#ifdef CLI_LVL_ALL
        DBG_SVCTXMGR_MSG((
            "{SVCTX_MGR} SVCTX(%s): Action (%s) CONTINUED...\r\n",
            pt_svctx->s_name,
            s_g_player_action[pt_player->e_phase]));
#endif
    }

    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name
 *      svctx_enable_slave_audio
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
VOID svctx_enable_slave_audio(SVCTX_T*                  pt_svctx,
                              SLAVE_AUDIO_CTRL_T        e_ctrl)
{
    BOOL        b_copy_protected = FALSE;
    BOOL        b_mute_bypass = FALSE;
    SVCTX_T*    pt_lead;
    UINT32      ui4_mute_mask;
    BOOL        b_forced = FALSE;
    SVCTX_T*    pt_obj   = NULL;

    if (!IS_SVCTX_ASSOCIATE(pt_svctx))
    {
        ACTIVE_LINK_ENTRY_T*    pt_link;
        UINT32                  ui4_i;
        BOOL                    b_found;

        pt_lead = svctx_mgr_get_link_lead(
            ACTIVE_LINK_NOW,
            pt_svctx->pt_running_svc_req->e_src_type);

        if (NULL != pt_lead)
        {
            if (pt_lead->pt_running_svc_req)
            {
                if (!(pt_lead->pt_running_svc_req->ui4_stream_mode & ST_MASK_AUDIO) ||
                    (!IS_SVCTX_ASSOCIATE(pt_svctx) && !(pt_svctx->pt_running_svc_req->ui4_stream_mode & ST_MASK_AUDIO)))
                {
                    return;
                }
            }

            x_scc_aud_get_mute_ex(
                pt_lead->h_audio_scc_comp,
                &ui4_mute_mask);

            ui4_mute_mask |= SCC_AUD_OUT_PORT_2_CH_BY_PASS;

            b_found = FALSE;
            ui4_i = 0;
            while (1)
            {
                pt_link = svctx_mgr_get_link_obj(
                    ACTIVE_LINK_NOW,
                    pt_svctx->pt_running_svc_req->e_src_type,
                    ui4_i++);

                if (pt_link)
                {
                    if (IS_SVCTX_ASSOCIATE(pt_link->pt_worker) && (pt_link->pt_worker->ui4_attribute & SVCTX_ASSC_ATTR_AUDIO_SLAVE))
                    {
                        b_found = TRUE;
                        break;
                    }
                }
                else
                {
                    b_found = FALSE;
                    break;
                }
            }

            if (!b_found)
            {
                x_scc_aud_switch_ex(
                    pt_lead->h_audio_scc_comp,
                    ui4_mute_mask);

                return;
            }
        }
    }


    if ((e_ctrl == SLAVE_AUDIO_CTRL_FORCE_ENABLE) ||
        (e_ctrl == SLAVE_AUDIO_CTRL_FORCE_DISABLE))
    {
        b_forced = TRUE;
    }

    if (IS_SVCTX_ASSOCIATE(pt_svctx) || b_forced)
    {
        if (e_ctrl == SLAVE_AUDIO_CTRL_FORCE_ENABLE)
        {
            svctx_reset_copy_protection_status_params(pt_svctx);
        }
        else if (((pt_svctx->t_cp_mask.ui4_macrovision & CP_VIDEO) &&
                  (pt_svctx->t_cp_mask.ui4_macrovision & pt_svctx->t_cp_status.ui4_macrovision)) ||
                 ((pt_svctx->t_cp_mask.ui4_hdcp & CP_VIDEO) &&
                  (pt_svctx->t_cp_mask.ui4_hdcp & pt_svctx->t_cp_status.ui4_hdcp)) ||
                 ((pt_svctx->t_cp_mask.ui4_ca & CP_VIDEO) &&
                  (pt_svctx->t_cp_mask.ui4_ca & pt_svctx->t_cp_status.ui4_ca)))
        {
            b_copy_protected = TRUE;
        }

        pt_lead = svctx_mgr_get_link_lead(
            ACTIVE_LINK_NOW,
            pt_svctx->pt_running_svc_req->e_src_type);

        if (pt_lead && x_handle_valid(pt_lead->h_audio_scc_comp))
        {
            if ((pt_lead) &&
                (SMR_OK == x_scc_aud_get_mute_ex(
                    pt_lead->h_audio_scc_comp,
                    &ui4_mute_mask)))
            {
                if ((e_ctrl == SLAVE_AUDIO_CTRL_ENABLE) ||
                    ((e_ctrl == SLAVE_AUDIO_CTRL_FORCE_ENABLE) && !b_copy_protected)) /* unmute */
                {
                    ui4_mute_mask &= ~SCC_AUD_OUT_PORT_2_CH_BY_PASS;
                    b_mute_bypass = FALSE;
                }
                else if (e_ctrl == SLAVE_AUDIO_CTRL_DISABLE ||
                         (e_ctrl == SLAVE_AUDIO_CTRL_FORCE_DISABLE) || b_copy_protected) /* mute */
                {
                    if (b_copy_protected)
                    {
                        ui4_mute_mask |= SCC_AUD_OUT_PORT_2_CH_BY_PASS;
                        b_mute_bypass = TRUE;                      
                    }
                    else
                    {
                        pt_obj = (pt_svctx->i1_prio_diff < pt_lead->i1_prio_diff) ? pt_svctx : pt_lead;
                        if (!(pt_obj->e_av_status == SVC_AV_STATUS_AUDIO_ONLY || 
                        pt_obj->e_av_status == SVC_AV_STATUS_SCRAMBLED_VIDEO_CLEAR_AUDIO) )
                        {
                            ui4_mute_mask |= SCC_AUD_OUT_PORT_2_CH_BY_PASS;
                            b_mute_bypass = TRUE;
                        }                      
                    }
                }

                /* TV bypass control */
                if (!(pt_svctx->ui4_attribute & SVCTX_ASSC_ATTR_AUDIO_SLAVE) || b_forced)
                {
                    x_scc_aud_set_output_port(
                            pt_svctx->h_audio_scc_comp,
                            SCC_AUD_OUT_PORT_TVSCART,
                            !b_mute_bypass);
                }

                /* Monitor bypass control */
                if ((pt_svctx->i1_prio_diff > pt_lead->i1_prio_diff) &&
                    ((pt_svctx->ui4_attribute & SVCTX_ASSC_ATTR_AUDIO_SLAVE) || b_forced) )
                {
                    x_scc_aud_switch_ex(
                        pt_lead->h_audio_scc_comp,
                        ui4_mute_mask);
                }
            }
        }
        else /* main play inp */
        {
            if (!(pt_svctx->ui4_attribute & SVCTX_ASSC_ATTR_AUDIO_SLAVE) || b_forced)
            {
                x_scc_aud_set_output_port(
                        pt_svctx->h_audio_scc_comp,
                        SCC_AUD_OUT_PORT_TVSCART,
                        !b_copy_protected);
            }
        }
    }
}
#endif
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_ctrl_main_audio
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 svctx_ctrl_main_audio(SVCTX_T*    pt_svctx,
                            BOOL        b_mute)
{
    INT32           i4_ret = SVCTXR_OK;
    UINT32          ui4_muted_out_port_msk = 0;

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (IS_SVCTX_ASSOCIATE(pt_svctx))
    {
        return SVCTXR_OK;
    }
#endif

    if (pt_svctx->pt_running_svc_req)
    {
        if (!(pt_svctx->pt_running_svc_req->ui4_stream_mode & ST_MASK_AUDIO))
        {
            return SVCTXR_OK;
        }
    }

    do {
        i4_ret = x_scc_aud_get_mute_ex(pt_svctx->h_audio_scc_comp, &ui4_muted_out_port_msk);
        if (SMR_OK != i4_ret) {
            break;
        }

        if (b_mute)
        {
            ui4_muted_out_port_msk |= SCC_AUD_OUT_PORT_SPEAKER;
            ui4_muted_out_port_msk |= SCC_AUD_OUT_PORT_HEADPHONE;
        }
        else
        {
            ui4_muted_out_port_msk &= ~SCC_AUD_OUT_PORT_SPEAKER;
            ui4_muted_out_port_msk &= ~SCC_AUD_OUT_PORT_HEADPHONE;
        }

        i4_ret = x_scc_aud_switch_ex(pt_svctx->h_audio_scc_comp, ui4_muted_out_port_msk);
    } while (0);

    return (i4_ret == SMR_OK) ? SVCTXR_OK : SVCTXR_FAIL;
}

#ifdef MW_TV_AV_BYPASS_SUPPORT
/*-----------------------------------------------------------------------------
 * Name
 *      svctx_reset_copy_protection_status_params
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
VOID svctx_reset_copy_protection_status_params(
            SVCTX_T*                    pt_svctx
            )
{
    pt_svctx->t_cp_status.ui4_macrovision = CP_NONE;
    pt_svctx->t_cp_status.ui4_hdcp        = CP_NONE;
    pt_svctx->t_cp_status.ui4_ca          = CP_NONE;
}

#ifdef CI_PLUS_SUPPORT
BOOL svctx_is_block_ci_plus(
    CP_INFO_LIST_T* pt_cp)
{
    INT32       i4_start    = 0;
    INT32       i4_end      = 0;
    UINT8       ui1_tmp     = 0;
    UINT8       ui1_value   = 0;
    CP_INFO_T*  pt_info     = NULL;
    BOOL        ab_set_flag[3] = {FALSE, FALSE, FALSE};

    BOOL ab_blank_map[32] = {
        0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,
        0,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,
    };

    if (NULL == pt_cp)
    {
        return FALSE;
    }

    i4_end = pt_cp->ui1_ctrl_size;
    while((i4_start < i4_end) && (i4_start < CP_TYPE_MAX))
    {
        pt_info = pt_cp->at_cp_ctrl_list+i4_start;

        switch(pt_info->t_cp_type)
        {
            case CP_TYPE_RCT:
                if (FALSE == ab_set_flag[0])
                {
                    ui1_tmp = (UINT8)(pt_info->u.e_rct_ctrl);
                    ui1_value |= (ui1_tmp<<4);
                    ab_set_flag[0] = TRUE;
                }
                break;
            case CP_TYPE_APS:
                if (FALSE == ab_set_flag[1])
                {
                    ui1_tmp = (UINT8)(pt_info->u.e_aps_ctrl);
                    ui1_value |= ui1_tmp;
                    ab_set_flag[1] = TRUE;
                }
                break;
            case CP_TYPE_EMI:
                if (FALSE == ab_set_flag[2])
                {
                    ui1_tmp = (UINT8)(pt_info->u.e_emi_ctrl);
                    ui1_value |= (ui1_tmp<<2);
                    ab_set_flag[2] = TRUE;
                }
                break;
            case CP_TYPE_CIT:
            case CP_TYPE_RL:
            case CP_TYPE_UNKNOWN:
            case CP_TYPE_NOTUSED:
            default:
                break;
        }

        i4_start++;
    }

    if (ui1_value >= 32)
    {
        return FALSE;
    }

    return (BOOL)(ab_blank_map[ui1_value]);
}
#endif

/*-----------------------------------------------------------------------------
 * Name
 *      svctx_handle_copy_protection
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 svctx_handle_copy_protection(
            SVCTX_T*                    pt_svctx,
            STRM_REQ_T*                 pt_strm_req,
            SVCTX_STRM_MSG_T*           pt_strm_msg
            )
{
    STREAM_TYPE_T       e_strm_type = svctx_get_strm_req_strm_type(pt_strm_req);
    BOOL                b_process_mv_hdcp = FALSE;
    BOOL                b_process_ca = FALSE;
    BOOL                b_mute_bypass = FALSE;
    UINT32              ui4_mute_mask;

    if ((pt_svctx == NULL) || (pt_strm_req == NULL) || (pt_strm_msg == NULL))
    {
        return SVCTXR_INV_ARG;
    }

    if (e_strm_type != ST_VIDEO)
    {
        return SVCTXR_OK;
    }

    /* Update copy protection status */
    switch(pt_strm_msg->e_event)
    {
#ifdef CI_PLUS_SUPPORT
        case SM_EVN_CCI_INFORMATION:
            pt_svctx->b_cci = b_mute_bypass = svctx_is_block_ci_plus(&(pt_strm_req->t_cp));
            b_process_ca = TRUE;
        break;
#endif
        case SM_EVN_CA_COND_FREE:
        case SM_EVN_CA_COND_AUTHORIZED:
            if (pt_svctx->t_cp_mask.ui4_ca & CP_VIDEO)
            {
                b_mute_bypass = FALSE;
                pt_svctx->t_cp_status.ui4_ca &= ~MAKE_BIT_MASK_32(e_strm_type);
                b_process_ca = TRUE;
            }
            break;
        case SM_EVN_CA_COND_NOT_AUTHORIZED:
        case SM_EVN_CA_COND_AUTHORIZED_CA_SYS_ID:
            if (pt_svctx->t_cp_mask.ui4_ca & CP_VIDEO)
            {
                b_mute_bypass = TRUE;
                pt_svctx->t_cp_status.ui4_ca |= MAKE_BIT_MASK_32(e_strm_type);
                b_process_ca = TRUE;
            }
            break;
        case SM_EVN_VS_MACROVISION:
        case SM_EVN_BYPASS_MACROVISION:
            if (pt_svctx->t_cp_mask.ui4_macrovision & CP_VIDEO)
            {
                b_mute_bypass = (pt_strm_msg->uinfo.ui4_data ? TRUE : FALSE);
                if (b_mute_bypass)
                {
                    pt_svctx->t_cp_status.ui4_macrovision |= MAKE_BIT_MASK_32(e_strm_type);
                }
                else
                {
                    pt_svctx->t_cp_status.ui4_macrovision &= ~MAKE_BIT_MASK_32(e_strm_type);
                }
                b_process_mv_hdcp = TRUE;
            }
            break;
        case SM_EVN_VS_HDCP_CONTENT:
        case SM_EVN_BYPASS_HDCP_CONTENT:
            if (pt_svctx->t_cp_mask.ui4_hdcp & CP_VIDEO)
            {
                b_mute_bypass = (pt_strm_msg->uinfo.ui4_data ? TRUE : FALSE);
                if (b_mute_bypass)
                {
                    pt_svctx->t_cp_status.ui4_hdcp |= MAKE_BIT_MASK_32(e_strm_type);
                }
                else
                {
                    pt_svctx->t_cp_status.ui4_hdcp &= ~MAKE_BIT_MASK_32(e_strm_type);
                }
                b_process_mv_hdcp = TRUE;
            }
            break;
        default:
            return SVCTXR_OK;
    }

    /* Block's priority is higher than copy protection */
    if (pt_svctx->b_is_blocked)
    {
        b_process_mv_hdcp = FALSE;
        b_process_ca = FALSE;
    }

    /* Video bypass control */
    if (b_process_mv_hdcp || b_process_ca)
    {
        svctx_set_bypass_mode(pt_svctx, !b_mute_bypass);
    }

    /* Audio bypass control */
    if (SMR_OK == x_scc_aud_get_mute_ex(pt_svctx->h_audio_scc_comp, &ui4_mute_mask))
    {
        SVCTX_T* pt_lead;

        pt_lead = svctx_mgr_get_link_lead(
            ACTIVE_LINK_NOW,
            pt_svctx->pt_running_svc_req->e_src_type);

        if (pt_lead && x_handle_valid(pt_lead->h_audio_scc_comp))
        {
            if (b_process_mv_hdcp)
            {
                /* Monitor bypass control */
                if (b_mute_bypass)
                {
                    ui4_mute_mask |= SCC_AUD_OUT_PORT_2_CH_BY_PASS;
                }
                else
                {
                    ui4_mute_mask &= ~SCC_AUD_OUT_PORT_2_CH_BY_PASS;
                }

                if (_svctx_is_able_to_ctrl_slave_audio(pt_svctx))
                {
                    x_scc_aud_switch_ex(
                            pt_lead->h_audio_scc_comp,
                            ui4_mute_mask);
                }
            }
            else if (b_process_ca)
            {
                if (b_mute_bypass)
                {
                    ui4_mute_mask |= SCC_AUD_OUT_PORT_2_CH_BY_PASS;
                }
                else
                {
                    ui4_mute_mask &= ~SCC_AUD_OUT_PORT_2_CH_BY_PASS;
                }

                /* TV bypass control */
                if (IS_SVCTX_ASSOCIATE(pt_svctx) && !(pt_svctx->ui4_attribute & SVCTX_ASSC_ATTR_AUDIO_SLAVE))
                {
                    x_scc_aud_set_output_port(
                            pt_svctx->h_audio_scc_comp,
                            SCC_AUD_OUT_PORT_TVSCART,
                            !b_mute_bypass);
                }

                /* Monitor bypass control */
                if (_svctx_is_able_to_ctrl_slave_audio(pt_svctx))
                {
                    x_scc_aud_switch_ex(
                            pt_lead->h_audio_scc_comp,
                            ui4_mute_mask);
                }
            }
        }
    }

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      svctx_set_bypass_mode
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          The routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 svctx_set_bypass_mode(
            SVCTX_T*                    pt_svctx,
            BOOL                        b_enable
            )
{
    INT32               i4_ret = SVCTXR_OK;
    SVCTX_T*            pt_lead = NULL;

    if (pt_svctx->pt_running_svc_req == NULL)
    {
        return i4_ret;
    }

    /* Force bypass to mute if CANAL+ is activated */
    pt_lead = svctx_mgr_get_link_lead(
        ACTIVE_LINK_NOW,
        pt_svctx->pt_running_svc_req->e_src_type);

    if (pt_lead &&
        (svctx_mgr_convert_to_svc_type(pt_lead->pt_running_svc_req->e_src_type) == SVC_SELECT_TV) &&
        (((CH_SVC_REQ_T*)pt_lead->pt_running_svc_req)->e_ch_src_type == CH_SRC_TYPE_SCART) &&
        (pt_svctx->ui4_attribute & SVCTX_ASSC_ATTR_AUDIO_SLAVE))
    {
        b_enable = FALSE;
    }

    if (x_handle_valid(pt_svctx->h_display_scc_comp))
    {
        i4_ret = x_scc_disp_set_bypass_mode(
                    pt_svctx->h_display_scc_comp,
                    b_enable ? SCC_DISP_BYPASS_NORMAL : SCC_DISP_BYPASS_BLANK);

        if (i4_ret == SMR_OK)
        {
            svctx_enable_slave_audio(
                pt_svctx,
                (b_enable ? SLAVE_AUDIO_CTRL_FORCE_ENABLE : SLAVE_AUDIO_CTRL_FORCE_DISABLE));
        }
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      svctx_set_ctrl
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 svctx_set_ctrl(
            SVCTX_CTRL_TYPE_T                e_ctrl_type
            )
{
#if 0    
    INT32       i4_ret = SVCTXR_OK;
    SVCTX_T*    pt_lead = NULL;

    svctx_class_lock();

    pt_lead = svctx_mgr_get_link_lead(
        ACTIVE_LINK_NOW,
        SRC_TYPE_BRDCST_SVL);

    if (pt_lead)
    {
        switch (e_ctrl_type)
        {
            case SVCTX_CTRL_TYPE_MUTE_MAIN_AUD:
                i4_ret = svctx_ctrl_main_audio(pt_lead, TRUE);
                break;
            default:
                i4_ret = SVCTXR_NOT_SUPPORT;
                break;
        }
    }
    else
    {
        i4_ret = SVCTXR_FAIL;
    }

    svctx_class_unlock();

    return i4_ret;
#else
    return SVCTXR_OK;
#endif
}

/*-----------------------------------------------------------------------------
 * Name
 *      svctx_is_reuse_pipe_disabled
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
BOOL svctx_is_reuse_pipe_disabled(
                    VOID
                    )
{
    return b_g_disable_reuse_pipe;
}
#endif

VOID svctx_ctrl_snow(
    SVCTX_T*    pt_svctx,
    BOOL        b_enable)
{
    /* Printf("[%s][%s] block=%d, snow=%d, new=%d\r\n" pt_svctx->s_name, __func__, pt_svctx->b_is_blocked, pt_svctx->b_enable_snow, b_enable); */

    pt_svctx->b_enable_snow = b_enable;
    if (!pt_svctx->b_is_blocked)
    {
#ifdef MW_TV_AV_BYPASS_SUPPORT
        if (NULL_HANDLE != pt_svctx->h_video_scc_comp)
#endif
        {
            x_scc_vid_set_force_unmute(pt_svctx->h_video_scc_comp, SCC_VID_FORCE_UNMUTE_TYPE_ON_NO_SIG, b_enable);
        }
    }
}

VOID svctx_ctrl_block(
    SVCTX_T*    pt_svctx,
    BOOL        b_enable)
{
    /* Printf("[%s][%s] block=%d, snow=%d, new=%d\r\n" pt_svctx->s_name, __func__, pt_svctx->b_is_blocked, pt_svctx->b_enable_snow, b_enable); */
    pt_svctx->b_is_blocked = b_enable;

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (NULL_HANDLE != pt_svctx->h_video_scc_comp)
#endif
    {
        x_scc_vid_set_force_unmute(pt_svctx->h_video_scc_comp, SCC_VID_FORCE_UNMUTE_TYPE_ON_NO_SIG, b_enable?FALSE:pt_svctx->b_enable_snow);
    }
}

/*-----------------------------------------------------------------------------
 * SVCTX export methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: x_svctx_open
 *
 * Description: This API open a new service context from the service context
 *              pool or attaches to an existing service context if the name
 *              is existed.
 *
 * Inputs:  ps_svctx_name      References the service context name for the open.
 *
 * Outputs: ph_svctx           Contains the service context handle.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_ARG     Invalid argument.
 *          SVCTXR_NO_RESOURCE Not enough memory to open the service context.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_FAIL        The Handle link operation is failed.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_open(
                    const CHAR*                 ps_svctx_name,
                    HANDLE_T*                   ph_svctx
                    )
{
    SVCTX_T* pt_svctx     = NULL;
    BOOL     b_new_object = FALSE;
    INT32    i4_ret;
    SIZE_T   z_size;

    DBG_API((_API_HEADER"%s("
                    "ps_svctx_name = %s,"
                    "ph_svctx = %d"
                    ")\r\n",
                    __func__,
                    ps_svctx_name,
                    ph_svctx
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    z_size = x_strlen(ps_svctx_name);
    if (z_size == 0 || z_size > SVCTX_NAME_MAX_SIZE || ph_svctx == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        if (svctx_find_instance(ps_svctx_name, &pt_svctx) == FALSE) {
            i4_ret = _svctx_create(
                            ps_svctx_name,
                            NULL,
                            NULL,
                            &pt_svctx
                            );
            if (i4_ret != SVCTXR_OK) {
                DBG_ERROR((_ERROR_HEADER"_svctx_create(%s) failed, i4_ret=%d\r\n", ps_svctx_name, i4_ret));
                break;
            }
#ifdef MW_TV_AV_BYPASS_SUPPORT
            i4_ret = svctx_mgr_add_association(pt_svctx, pt_svctx, NULL);

            if (i4_ret != SVCTXR_OK) {
                DBG_ERROR((_ERROR_HEADER"svctx_mgr_add_association() failed, i4_ret=%d\r\n", i4_ret));
                break;
            }
#endif
            b_new_object = TRUE;
        }

        i4_ret = handle_alloc(
                            SVCTXT_SVCTX,
                            (VOID*)pt_svctx,
                            NULL,
                            _svctx_handle_free_fct,
                            ph_svctx
                            );
        if ( i4_ret != HR_OK) {
            DBG_ERROR((_ERROR_HEADER"handle_alloc(%s) failed, i4_ret=%d\r\n", ps_svctx_name, i4_ret));
            i4_ret = SVCTXR_NO_RESOURCE;
            break;
        }

        i4_ret = handle_link(
                            &pt_svctx->t_hdl_link,
                            *ph_svctx
                            );
        if (i4_ret != HR_OK) {
            DBG_ERROR((_ERROR_HEADER"handle_link(%s) failed, i4_ret=%d\r\n", ps_svctx_name, i4_ret));
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = SVCTXR_OK;
    } while (0);

    if (i4_ret != SVCTXR_OK && b_new_object == TRUE) {
        svctx_free_object(pt_svctx);
    }

    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_close
 *
 * Description: This API performs a close. If the handle is the last client's
 *              handle, this service context instance will return to the service
 *              context pool.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_FAIL        Failed to close the service context
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_close(
                    HANDLE_T                    h_svctx
                    )
{
    SVCTX_MSG_T t_msg = {0};
    INT32       i4_ret;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d"
                    ")\r\n",
                    __func__,
                    h_svctx
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    x_sema_lock(h_g_module_sync_stop_sema, X_SEMA_OPTION_WAIT);
    do {
        t_msg.h_svctx              = h_svctx;
        t_msg.ui1_msg_id           = _SVCTX_MSG_CLOSE_REQ;
        t_msg.u.t_close_req.pf_nfy = _svctx_close_cb;
        t_msg.u.t_close_req.pv_tag = (VOID*)h_g_module_wait_sema;

        i4_ret = activator_send_msg(
                        pt_g_activator,
                        SVCTX_MSGQ_DEFAULT_PRIORITY,
                        &t_msg
                        );
        if (i4_ret != ACTVTRR_OK) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = _WAIT_TIMEOUT(h_g_module_wait_sema, DEFAULT_CLOSE_SVCTX_TIMEOUT);
        if (i4_ret != OSR_OK) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = SVCTXR_OK;
    } while (0);
    x_sema_unlock(h_g_module_sync_stop_sema);

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_stop_svc_ex
 *
 * Description: This API causes the service context instance to stop presenting
 *              content and enter the READY state in asynchronous way.
 *              Connection, SCDB, Session, and Streams used in the presentation
 *              will be released. The client
 *              that issues the service selection request will be notified
 *              via its pf_nfy (set in x_svctx_select_svc()) with
 *                  e_nfy_cond = SVCTX_COND_READY,
 *                  e_code     = SVCTX_NTFY_CODE_USER_STOP
 *              If the service is already in stopping, the client
 *              that issues the service selection request will still be notified
 *              via its pf_nfy (set in x_svctx_select_svc()) with
 *                  e_nfy_cond = SVCTX_COND_PENDING,
 *                  e_code     = SVCTX_NTFY_CODE_IN_STOPPING
 *              If the service context instance is in the READY state, the client
 *              that issues the service selection request will still be notified
 *              via its pf_nfy (set in x_svctx_select_svc()) with
 *                  e_nfy_cond = SVCTX_COND_READY,
 *                  e_code     = SVCTX_NTFY_CODE_STOPPED
 *              If the service context instance is in the POOLED state,
 *              no action is performed
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          pf_nfy             Notify function called when each svctx is stopped
 *          pv_tag             Tag for pf_nfy
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_FAIL        Failed to send request
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_stop_svc_ex(
                    HANDLE_T                    h_svctx,
                    x_svctx_stop_nfy_fct        pf_nfy,
                    VOID*                       pv_tag
                    )
{
#ifdef MW_TV_AV_BYPASS_SUPPORT
    SVCTX_T*                pt_obj = NULL;
    INT32                   i, i4_num;
    ACTIVE_LINK_ENTRY_T*    pt_link;
#endif
    SVCTX_MSG_T             t_msg    = {0};
    SVCTX_T*                pt_svctx = NULL;
    INT32                   i4_ret   = SVCTXR_OK;
    svctx_nfy_fct           pf_wrapped_nfy = NULL;
    SVCTX_WRAPPED_TAG_T*    pt_wrapped_tag = NULL;
    BOOL                    b_need_free_tag= TRUE;

    CLI_FUNC_CALL_TRACE(__func__);

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pf_nfy != NULL)
    {
        pt_wrapped_tag = (SVCTX_WRAPPED_TAG_T*) x_mem_alloc(
                sizeof(SVCTX_WRAPPED_TAG_T));
        if (pt_wrapped_tag == NULL)
        {
            return SVCTXR_NO_RESOURCE;
        }
        x_memset(pt_wrapped_tag, 0, sizeof(SVCTX_WRAPPED_TAG_T));
        pt_wrapped_tag->h_svctx = h_svctx;
        pt_wrapped_tag->u.t_stop_svc_cb.pf_nfy = pf_nfy;
        pt_wrapped_tag->u.t_stop_svc_cb.pv_tag = pv_tag;

        pf_wrapped_nfy = _svctx_stop_wrapped_cb;
    }

    x_sema_lock(h_g_module_sync_stop_sema, X_SEMA_OPTION_WAIT);
    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        /*x_dbg_stmt("[%s][%s][%d]\r\n", pt_svctx->s_name, __func__, __LINE__);*/

        if (!_IN_RUNNING_STATE(pt_svctx->e_state)) {
            _svctx_force_mute_video(pt_svctx, VIDEO_MUTE_CTRL_DISABLE);
            i4_ret = SVCTXR_ALREADY_STOPPED;
            break;
        }

#ifdef MW_RIKS_TV_SUPPORT
        if (svctx_rikstv_in_tuning_state(pt_svctx) && pf_nfy == NULL)
        {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }
#endif

#ifdef MW_TV_AV_BYPASS_SUPPORT
        i = i4_num = (INT32) svctx_mgr_get_link_num(
            ACTIVE_LINK_NOW,
            pt_svctx->pt_running_svc_req->e_src_type) - 1;

        while (i >= 0)
        {
            pt_link = svctx_mgr_get_link_obj(
                ACTIVE_LINK_NOW,
                pt_svctx->pt_running_svc_req->e_src_type,
                i);

            i4_ret = SVCTXR_OK;

            if (pt_link)
            {
                pt_obj = pt_link->pt_worker;

                if (pt_obj)
                {
                    if (pt_obj == pt_svctx)
                    {
                        t_msg.h_svctx = h_svctx;
                    }
                    else if (

                        !(pt_obj->ui4_attribute & SVCTX_ASSC_ATTR_KEEP_ALIVE_AS_STOP_LEAD) &&

#ifdef MW_PIP_POP_SUPPORT
                        IS_SVCTX_EXECUTABLE(pt_svctx->ui4_assc_state) &&
                        IS_SVCTX_EXECUTABLE(pt_obj->ui4_assc_state) &&
                        svctx_mgr_be_associated(pt_svctx, pt_obj))
#else
                        (TRUE == pt_obj->b_assc_enabled) &&
                        (TRUE == pt_svctx->b_assc_enabled))
#endif
                    {
                        t_msg.h_svctx = pt_obj->h_sys_svctx;
                    }
                    else
                    {
                        i--;
                        continue;
                    }

                    t_msg.ui1_msg_id                   = _SVCTX_MSG_STOP_SVC_REQ;
                    if (i == 0)
                    {
                        t_msg.u.t_sync_stop_svc_req.pf_nfy = pf_wrapped_nfy;
                        t_msg.u.t_sync_stop_svc_req.pv_tag = pt_wrapped_tag;
                    }
                    else
                    {
                        t_msg.u.t_sync_stop_svc_req.pf_nfy = NULL;
                        t_msg.u.t_sync_stop_svc_req.pv_tag = NULL;
                    }

                    i4_ret = activator_send_msg(
                                pt_g_activator,
                                SVCTX_MSGQ_DEFAULT_PRIORITY,
                                &t_msg
                                );

                    if (i4_ret != ACTVTRR_OK)
                    {
                        i4_ret = SVCTXR_FAIL;
                        break;
                    }
                    else if (t_msg.u.t_sync_stop_svc_req.pv_tag != NULL) 
                    {
                        /* if OK, tag will be freed in pf_wrapped_nfy(); */
                        b_need_free_tag = FALSE;
                    }
                }
            }
            else
            {
                if (i4_num == i)
                {
                    i4_ret = SVCTXR_FAIL;
                }
                break;
            }

            i--;
        }
#else
        t_msg.h_svctx                      = h_svctx;
        t_msg.ui1_msg_id                   = _SVCTX_MSG_STOP_SVC_REQ;
        t_msg.u.t_sync_stop_svc_req.pf_nfy = pf_wrapped_nfy;
        t_msg.u.t_sync_stop_svc_req.pv_tag = pt_wrapped_tag;

        i4_ret = activator_send_msg(
                    pt_g_activator,
                    SVCTX_MSGQ_DEFAULT_PRIORITY,
                    &t_msg
                    );
        i4_ret = (i4_ret != ACTVTRR_OK) ? SVCTXR_FAIL: SVCTXR_OK;
        if (i4_ret == SVCTXR_OK) {
            b_need_free_tag = FALSE;
        }
#endif
    } while (0);
    svctx_class_unlock();
    x_sema_unlock(h_g_module_sync_stop_sema);

    if (b_need_free_tag) {
        x_mem_free(pt_wrapped_tag);
    }

    return (i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_stop_svc
 *
 * Description: This API causes the service context instance to stop presenting
 *              content and enter the READY state in asynchronous way.
 *              Connection, SCDB, Session, and Streams used in the presentation
 *              will be released. The client
 *              that issues the service selection request will be notified
 *              via its pf_nfy (set in x_svctx_select_svc()) with
 *                  e_nfy_cond = SVCTX_COND_READY,
 *                  e_code     = SVCTX_NTFY_CODE_USER_STOP
 *              If the service is already in stopping, the client
 *              that issues the service selection request will still be notified
 *              via its pf_nfy (set in x_svctx_select_svc()) with
 *                  e_nfy_cond = SVCTX_COND_PENDING,
 *                  e_code     = SVCTX_NTFY_CODE_IN_STOPPING
 *              If the service context instance is in the READY state, the client
 *              that issues the service selection request will still be notified
 *              via its pf_nfy (set in x_svctx_select_svc()) with
 *                  e_nfy_cond = SVCTX_COND_READY,
 *                  e_code     = SVCTX_NTFY_CODE_STOPPED
 *              If the service context instance is in the POOLED state,
 *              no action is performed
 * Inputs:  h_svctx            Contains a handle to a service context.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_FAIL        Failed to send request
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_stop_svc(
                    HANDLE_T                    h_svctx
                    )
{
    return x_svctx_stop_svc_ex(h_svctx, NULL, NULL);
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_is_running
 *
 * Description: This API is used to check the service context instance is in
 *              running or not
 * Inputs:  h_svctx            Contains a handle to a service context.
 *
 * Outputs: pb_result          Contains the result.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_ARG     Invalid argument.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_FAIL        Failed to send request
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_is_running (
                    HANDLE_T                    h_svctx,
                    BOOL*                       pb_result
                    )
{
    SVCTX_T* pt_svctx = NULL;
    INT32    i4_ret   = SVCTXR_OK;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pb_result == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (_IN_RUNNING_STATE(pt_svctx->e_state)) {
            *pb_result = TRUE;
        } else {
            *pb_result = FALSE;
        }
    } while (0);
    svctx_class_unlock();
    return (SVCTXR_OK);
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_sync_stop_svc
 *
 * Description: This API causes the service context instance to stop presenting
 *              content and enter the READY state in synchronous way. Connection,
 *              SCDB, Session, Streams used in the presentation will be released
 *              and the return value is SVCTXR_OK. However, the client
 *              that issues the service selection request will still be notified
 *              via its pf_nfy (set in x_svctx_select_svc()) with
 *                  e_nfy_cond = SVCTX_COND_READY,
 *                  e_code     = SVCTX_NTFY_CODE_USER_STOP
 *              If the return value is SVCTXR_TIMEOUT, the stop operation is still
 *              processed in service context's thread. When the service
 *              is stopped, the client will receive the notification, too.
 *              If the service context instance is in the READY state, the client
 *              that issues the service selection request will still be notified
 *              via its pf_nfy (set in x_svctx_select_svc()) with
 *                  e_nfy_cond = SVCTX_COND_READY,
 *                  e_code     = SVCTX_NTFY_CODE_STOPPED
 *              If the service context instance is in the POOLED state,
 *              no action is performed
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          ui4_time           maximum time duration in ms to wait the API
 *                             to finish. if it is zero, it will be set to
 *                             DEFAULT_STOP_SVC_TIMEOUT
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_TIMEOUT     Failed to stop the service within "ui4_time" duration.
 *          SVCTXR_FAIL        Failed to send request.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_sync_stop_svc(
                    HANDLE_T                    h_svctx,
                    UINT32                      ui4_time
                    )
{
#ifdef MW_TV_AV_BYPASS_SUPPORT
    SVCTX_T*                pt_svctx = NULL;
    SVCTX_T*                pt_obj   = NULL;
    INT32                   i, i4_idx;
    ACTIVE_LINK_ENTRY_T*    pt_link;
    SRC_TYPE_T              e_src_type;
#endif
    SVCTX_MSG_T             t_msg  = {0};
    INT32                   i4_ret = SVCTXR_OK;


    CLI_FUNC_CALL_TRACE(__func__);

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    x_sema_lock(h_g_module_sync_stop_sema, X_SEMA_OPTION_WAIT);
    do {
        if (ui4_time == 0) {
            ui4_time = DEFAULT_STOP_SVC_TIMEOUT;
        }
#ifdef MW_TV_AV_BYPASS_SUPPORT
        i4_ret = svctx_get_obj_with_ext_type(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        /*x_dbg_stmt("[%s][%s][%d]\r\n", pt_svctx->s_name, __func__, __LINE__);*/
        /* A tricky method to resolve the race condation issue:
         *     1. AP select serice
         *     2. AP sync stop service when above request not processed
         */
        if (!_IN_RUNNING_STATE(pt_svctx->e_state)) {
            int i = 0;
            for (i=0; i<=4; i++)
            {
                x_thread_delay(20);
                if (_IN_RUNNING_STATE(pt_svctx->e_state)) {
                    break;
                }
            }
        }

        if (!_IN_RUNNING_STATE(pt_svctx->e_state)) {
            _svctx_force_mute_video(pt_svctx, VIDEO_MUTE_CTRL_DISABLE);
            i4_ret = SVCTXR_ALREADY_STOPPED;
            break;
        }

#ifdef MW_RIKS_TV_SUPPORT
        if (svctx_rikstv_in_tuning_state(pt_svctx))
        {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }
#endif

/*{
        x_dbg_stmt("[%s][%s][%d], current src=%d, ch number = %d, avc number = %d, mm number = %d\r\n", pt_svctx->s_name, __func__, __LINE__, pt_svctx->pt_running_svc_req->e_src_type,
            svctx_mgr_get_link_num(ACTIVE_LINK_NOW, SRC_TYPE_BRDCST_SVL), 
            svctx_mgr_get_link_num(ACTIVE_LINK_NOW, SRC_TYPE_AVC),
            svctx_mgr_get_link_num(ACTIVE_LINK_NOW, SRC_TYPE_MEDIA_STORGE));
}*/

        if (!IS_SVCTX_ASSOCIATE(pt_svctx))
        {
            e_src_type = pt_svctx->pt_running_svc_req->e_src_type;

            i = i4_idx = svctx_mgr_get_link_num(
                ACTIVE_LINK_NOW,
                e_src_type) - 1;

            while (i >= 0)
            {
                pt_link = svctx_mgr_get_link_obj(
                    ACTIVE_LINK_NOW,
                    e_src_type,
                    i);

                i4_ret = SVCTXR_OK;

                if (pt_link)
                {
                    pt_obj = pt_link->pt_worker;

                    if (pt_obj)
                    {
                        if (pt_obj == pt_svctx)
                        {
                            t_msg.h_svctx = h_svctx;
                        }
                        else if (

                            !(pt_obj->ui4_attribute & SVCTX_ASSC_ATTR_KEEP_ALIVE_AS_STOP_LEAD) &&

#ifdef MW_PIP_POP_SUPPORT
                            IS_SVCTX_EXECUTABLE(pt_svctx->ui4_assc_state) &&
                            IS_SVCTX_EXECUTABLE(pt_obj->ui4_assc_state) &&
                            svctx_mgr_be_associated(pt_svctx, pt_obj))
#else
                            (TRUE == pt_obj->b_assc_enabled) &&
                            (TRUE == pt_svctx->b_assc_enabled))
#endif
                        {
                            t_msg.h_svctx = pt_obj->h_sys_svctx;
                        }
                        else
                        {
                            i--;
                            continue;
                        }

                        t_msg.ui1_msg_id                   = _SVCTX_MSG_STOP_SVC_REQ;
                        t_msg.u.t_sync_stop_svc_req.pf_nfy = _svctx_stop_sync_cb;
                        t_msg.u.t_sync_stop_svc_req.pv_tag = (VOID*) h_g_module_wait_sema;

                        i4_ret = activator_send_msg(
                                    pt_g_activator,
                                    SVCTX_MSGQ_DEFAULT_PRIORITY,
                                    &t_msg
                                    );

                        if (i4_ret != ACTVTRR_OK)
                        {
                            i4_ret = SVCTXR_FAIL;
                            break;
                        }

                        i4_ret = _WAIT_TIMEOUT(h_g_module_wait_sema, ui4_time);

                        if (i4_ret == OSR_TIMEOUT)
                        {
                            x_dbg_stmt("[%s] sync stop fail !!!!!!!!!!\r\n\r\n", pt_obj->s_name);
                            i4_ret = SVCTXR_TIMEOUT;
                            break;
                        }

                        x_dbg_stmt("[%s] sync stop OK !!!!!!!!!!\r\n\r\n", pt_obj->s_name);
                    }
                }
                else
                {
                    if (i4_idx == i)
                    {
                        i4_ret = SVCTXR_FAIL;
                    }
                    break;
                }

                i--;
            }
        }
        else
        {
            if (pt_svctx->ui4_attribute & SVCTX_ASSC_ATTR_KEEP_ALIVE)
            {
                /* instead of stopping the service, just disable it */
            }
            else
            {
                t_msg.h_svctx                      = h_svctx;
                t_msg.ui1_msg_id                   = _SVCTX_MSG_STOP_SVC_REQ;
                t_msg.u.t_sync_stop_svc_req.pf_nfy = _svctx_stop_sync_cb;
                t_msg.u.t_sync_stop_svc_req.pv_tag = (VOID*) h_g_module_wait_sema;

                i4_ret = activator_send_msg(
                            pt_g_activator,
                            SVCTX_MSGQ_DEFAULT_PRIORITY,
                            &t_msg
                            );

                if (i4_ret != ACTVTRR_OK)
                {
                    i4_ret = SVCTXR_FAIL;
                    break;
                }

                i4_ret = _WAIT_TIMEOUT(h_g_module_wait_sema, ui4_time);

                if (i4_ret == OSR_TIMEOUT)
                {
                    i4_ret = SVCTXR_TIMEOUT;
                    break;
                }
            }
        }
#else
        t_msg.h_svctx                      = h_svctx;
        t_msg.ui1_msg_id                   = _SVCTX_MSG_STOP_SVC_REQ;
        t_msg.u.t_sync_stop_svc_req.pf_nfy = _svctx_stop_sync_cb;
        t_msg.u.t_sync_stop_svc_req.pv_tag = (VOID*)h_g_module_wait_sema;

        i4_ret = activator_send_msg(
                        pt_g_activator,
                        SVCTX_MSGQ_DEFAULT_PRIORITY,
                        &t_msg
                        );
        if (i4_ret != ACTVTRR_OK) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = _WAIT_TIMEOUT(h_g_module_wait_sema, ui4_time);
        if (i4_ret == OSR_TIMEOUT) {
            i4_ret = SVCTXR_TIMEOUT;
            break;
        }

        if (i4_ret != OSR_OK) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = SVCTXR_OK;
#endif
    } while (0);
    x_sema_unlock(h_g_module_sync_stop_sema);

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_select_svc
 *
 * Description: Selects a service to be presented in this service context
 *              instance in asynchronous way. If the instance is already
 *              presenting content, the new selection request will replace
 *              the previous request being presented. If the
 *              instance is not presenting, successful conclusion of this
 *              operation results in the instance entering the presenting
 *              state. Successful completion of the selection is notified
 *              via pf_nfy with
 *                  e_nfy_cond = SVCTX_COND_PRESENTING,
 *                  e_code     = SVCTX_NTFY_CODE_NORMAL
 *              If the signal is loss, the client will be notified via pf_nfy
 *                  e_nfy_cond = SVCTX_COND_READY,
 *                  e_code     = SVCTX_NTFY_CODE_SIGNAL_LOSS
 *              And in such case, the service context will try to re-connect.
 *
 *              In processing the selection, the client will be notified
 *              when the signal is locked (connection connected) via pf_nfy with
 *                  e_nfy_cond = SVCTX_COND_PENDING,
 *                  e_code     = SVCTX_NTFY_CODE_SIGNAL_LOCKED
 *              In processing the selection, the client will be notified
 *              when the service ls blocked via pf_nfy with
 *                  e_nfy_cond = SVCTX_COND_PENDING,
 *                  e_code     = SVCTX_NTFY_CODE_SERVICE_BLOCKED
 *              In processing the selection, the client will be notified
 *              when a kind of stream is not existed in SCDB  via pf_nfy with
 *                  e_nfy_cond    = SVCTX_COND_PENDING,
 *                  e_code        = SVCTX_NTFY_CODE_NO_STREAM
 *                  e_stream_type = the actual stream type
 *              If the selection process fails becuase lack of resources
 *              the client will be notified via pf_nfy.
 *                  e_nfy_cond    = current service context state,
 *                  e_code        = SVCTX_NTFY_CODE_NO_RESOURCES
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          ui4_stream_mode    References the desired stream types for
 *                             the select operation. Client should use the
 *                             macros ST_MASK_AUDIO, ST_MASK_VIDEO,
 *                             ST_MASK_CLOSED_CAPTION and so on (defined in
 *                             u_common.h) to OR them for this input argument.
 *          pt_src_desc        References the source description.
 *          pt_snk_desc        References the sink description.
 *          pf_nfy             References the service notify function.
 *          pv_nfy_tag         References the tag passed to the notify function.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     Invalid argument.
 *          SVCTXR_NO_RESOURCE Not enough memory to allocate this request.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_FAIL        Failed to send request
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_select_svc(
                    HANDLE_T                    h_svctx,
                    UINT32                      ui4_stream_mode,
                    const SRC_DESC_T*           pt_src_desc,
                    const SNK_DESC_T*           pt_snk_desc,
                    x_svctx_select_nfy_fct      pf_nfy,
                    VOID*                       pv_nfy_tag
                    )
{
#ifdef MW_TV_AV_BYPASS_SUPPORT
    SVCTX_T*                pt_obj;
    ACTIVE_LINK_ENTRY_T*    pt_link;
    HANDLE_T                h_obj;
    INT32                   i, i4_num;
    UINT32                  ui4_eff_strm_mode = 0;
    SRC_DESC_T              t_src_desc_eff;
    SNK_DESC_T              t_snk_desc;
    SNK_DESC_T*             pt_snk_desc_eff;
    SRC_TYPE_T              e_src_type_lead;
    BOOL                    b_skip_this;
#ifdef SYS_RECORD_SUPPORT
    SRC_REC_INFO_T          t_rec_info;
#endif
#endif
    SVCTX_MSG_T   t_msg      = {0};
    SVCTX_T*      pt_svctx   = NULL;
    INT32         i4_ret     = SVCTXR_OK;
    SVC_REQ_T*    pt_svc_req = NULL;
    SVCTX_HDLR_T* pt_hdlr    = NULL;

    CLI_FUNC_CALL_TRACE(__func__);

    #if defined(MW_FAST_INIT)
    //printf("x_svctx_select_svc entry\n\r");
    #endif

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (!pt_src_desc)
    {
        return SVCTXR_INV_ARG;
    }
#endif

    x_sema_lock(h_g_module_sync_stop_sema, X_SEMA_OPTION_WAIT);
    x_sema_unlock(h_g_module_sync_stop_sema);

    svctx_cli_clean_channel_change_log();
    
    /* construct the service select request object */
    svctx_class_lock();
    do {
#ifdef MW_TV_AV_BYPASS_SUPPORT
        i4_ret = svctx_get_obj_with_ext_type(h_svctx, &pt_svctx);
#else
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
#endif
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        /*x_dbg_stmt("[%s][%s][%d], src=%d, ch number = %d, avc number = %d, mm number = %d\r\n", pt_svctx->s_name, __func__, __LINE__, pt_src_desc->e_type,
            svctx_mgr_get_link_num(ACTIVE_LINK_NOW, SRC_TYPE_BRDCST_SVL), 
                                svctx_mgr_get_link_num(ACTIVE_LINK_NOW, SRC_TYPE_AVC),
                                svctx_mgr_get_link_num(ACTIVE_LINK_NOW, SRC_TYPE_MEDIA_STORGE));*/

        if (svctx_is_owner(pt_svctx, h_svctx) == FALSE) {
            i4_ret = SVCTXR_NO_RIGHT;
            break;
        }

#ifdef MW_RIKS_TV_SUPPORT
        if (svctx_rikstv_in_tuning_state(pt_svctx) && pf_nfy == NULL)
        {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }
#endif

#ifdef MW_TV_AV_BYPASS_SUPPORT

#if 0
        if (FALSE == _svctx_valid_to_select_svc(
            pt_src_desc->e_type,
            pt_svctx))
        {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }
#endif

#ifdef MW_PIP_POP_SUPPORT
        _svctx_a_tricky_method_remove_pre_custom_audio(pt_src_desc->e_type);
#endif

        /* Stop services which are disabled and not-associated when selecting a new service */
        _svctx_stop_disabled_n_non_associated_svc(pt_src_desc->e_type);            

        i4_ret = svctx_mgr_build_active_link(
            ACTIVE_LINK_NEXT,
            pt_src_desc->e_type,
            pt_svctx);

        if (SVCTXR_OK != i4_ret)
        {
            break;
        }
       
        b_g_disable_replace_svc = FALSE;
        
   #ifdef MW_PIP_POP_SUPPORT     
        if(!svctx_is_pap(pt_svctx))
   #endif
        {
            pt_obj = svctx_mgr_get_link_lead(ACTIVE_LINK_NOW, pt_src_desc->e_type);
            if (pt_obj)
            {
                UINT32 ui4_av_mask = ST_MASK_VIDEO | ST_MASK_AUDIO;
                b_g_disable_replace_svc = ((ui4_stream_mode & ui4_av_mask) != (pt_obj->ui4_stream_mode & ui4_av_mask));
            }
       }

        i = i4_num = (INT32) svctx_mgr_get_link_num(ACTIVE_LINK_NEXT, pt_src_desc->e_type) - 1;

        while (i >= 0)
        {
            pt_link = svctx_mgr_get_link_obj(
                ACTIVE_LINK_NEXT,
                pt_src_desc->e_type,
                i);

            pt_svc_req = NULL;

            i4_ret = SVCTXR_OK;

            if (pt_link)
            {
                pt_obj = pt_link->pt_worker;

                if (pt_obj)
                {
                    e_src_type_lead = pt_src_desc->e_type;

                    _svctx_set_src_desc (pt_obj,
                                         pt_src_desc,
                                         #ifdef SYS_RECORD_SUPPORT
                                         &t_rec_info,
                                         #endif
                                         &t_src_desc_eff);

                    i4_ret = _svctx_set_snk_desc(pt_svctx,
                                                 pt_snk_desc,
                                                 ui4_stream_mode,
                                                 pt_obj,
                                                 &t_snk_desc,
                                                 &pt_snk_desc_eff,
                                                 &ui4_eff_strm_mode);
                    if (i4_ret != SVCTXR_OK)
                    {
                        continue;
                    }

                    if (_svctx_find_handler(t_src_desc_eff.e_type, &pt_hdlr) == FALSE) {
                        DBG_ERROR((_ERROR_HEADER"%s(), no relative handler for this request!\r\n", __func__));
                        i4_ret = SVCTXR_NOT_SUPPORT;
                        break;
                    }

                    if (pt_hdlr->t_fcts.pf_create_svc_req == NULL) {
                        i4_ret = SVCTXR_NOT_SUPPORT;
                        break;
                    }

                    b_skip_this = FALSE;
                    if (!IS_SVCTX_ASSOCIATE(pt_svctx) && (pt_obj == pt_svctx))
                    {
                        /* handle the specified SVCTX object */
                        h_obj = h_svctx;

                        pt_svc_req = pt_hdlr->t_fcts.pf_create_svc_req(
                            pt_svctx,
                            h_obj,
                            pt_hdlr,
                            ui4_eff_strm_mode,
                            &t_src_desc_eff,
                            pt_snk_desc_eff,
                            pf_nfy,
                            pv_nfy_tag);

                        if (pt_svc_req == NULL)
                        {
                            i4_ret = SVCTXR_NO_RESOURCE;
                            break;
                        }

                        if (pt_svctx->b_in_stopping == FALSE)
                        {
                            pt_svc_req->b_freeze_change = t_src_desc_eff.b_freeze_change;
                        }

                        t_msg.ui1_msg_id   = _SVCTX_MSG_SELECT_SVC_REQ;
                        t_msg.u.pt_svc_req = pt_svc_req;
                    }
                    else if (
#ifdef MW_PIP_POP_SUPPORT
                        !IS_SVCTX_DISABLED(pt_svctx->ui4_assc_state) &&
                        !IS_SVCTX_DISABLED(pt_obj->ui4_assc_state))
#else
                        (TRUE == pt_svctx->b_assc_enabled) &&
                        (TRUE == pt_obj->b_assc_enabled))
#endif
                    {
                        /* Handle other associated SVCTX object */
#ifdef MW_PIP_POP_SUPPORT
                        if (IS_SVCTX_EXCLUDED(pt_obj->ui4_assc_state))
                        {
                            i--;
                            continue;
                        }
#endif

                        DBG_SVCTXMGR_MSG((
                            "\r\n{SVCTX_MGR} START the exclusive SVCTX (%s)\r\n\r\n",
                            pt_obj->s_name));

                        h_obj = pt_obj->h_sys_svctx;

                        pt_svc_req = pt_hdlr->t_fcts.pf_create_svc_req(
                            pt_obj,
                            h_obj,
                            pt_hdlr,
                            ((ui4_stream_mode == SVCTX_STRM_MODE_AS_LAST) ? ui4_stream_mode : (ui4_stream_mode & ui4_eff_strm_mode)),
                            &t_src_desc_eff,
                            pt_snk_desc_eff,
                            NULL,
                            NULL);

                        if (pt_svc_req)
                        {
                            if (pt_svctx->b_in_stopping == FALSE)
                            {
                                pt_svc_req->b_freeze_change = t_src_desc_eff.b_freeze_change;
                            }
                            t_msg.ui1_msg_id   = _SVCTX_MSG_SELECT_SVC_REQ;
                            t_msg.u.pt_svc_req = pt_svc_req;
                        }
                        else
                        {
                            b_skip_this = TRUE;
                        }
                    }
                    else
                    {
                        b_skip_this = TRUE;
                    }

                    if (b_skip_this)
                    {
                        if (svctx_mgr_be_active(pt_obj))
                        {
                            h_obj = pt_obj->h_sys_svctx;

                            t_msg.ui1_msg_id                   = _SVCTX_MSG_STOP_SVC_REQ;
                            t_msg.u.t_sync_stop_svc_req.pf_nfy = NULL;
                            t_msg.u.t_sync_stop_svc_req.pv_tag = NULL;

                            pt_obj->b_piped_stop    = TRUE;
                            pt_obj->e_next_src_type = e_src_type_lead;
                        }
                        else
                        {
                            svctx_mgr_remove_link_with_obj(ACTIVE_LINK_NEXT, pt_obj);
                            i--;
                            continue;
                        }
                    }

                    /* set the internal message */
                    t_msg.h_svctx = h_obj;

                    i4_ret = activator_send_msg(
                        pt_g_activator,
                        SVCTX_MSGQ_DEFAULT_PRIORITY,
                        &t_msg);

                    if (i4_ret != ACTVTRR_OK)
                    {
                        if (pt_svc_req)
                        {
                            svctx_free_svc_req(pt_svc_req);
                        }
                        i4_ret = SVCTXR_FAIL;
                        break;
                    }
                }
            }
            else
            {
                if (i4_num == i)
                {
                    i4_ret = SVCTXR_FAIL;
                }
                break;
            }

            i--;
        }
#else

        if (_svctx_find_handler(pt_src_desc->e_type, &pt_hdlr) == FALSE) {
            DBG_ERROR((_ERROR_HEADER"%s(), no relative handler for this request!\r\n", __func__));
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        if (pt_hdlr->t_fcts.pf_create_svc_req == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        pt_svc_req = pt_hdlr->t_fcts.pf_create_svc_req(
                            pt_svctx,
                            h_svctx,
                            pt_hdlr,
                            ui4_stream_mode,
                            pt_src_desc,
                            pt_snk_desc,
                            pf_nfy,
                            pv_nfy_tag
                            );
        if (pt_svc_req == NULL) {
            DBG_ERROR((_ERROR_HEADER"create SVC_REQ_T failed!\r\n"));
            i4_ret = SVCTXR_NO_RESOURCE;
            break;
        }

        if (pt_svctx->b_in_stopping == FALSE)
        {
            pt_svc_req->b_freeze_change = pt_src_desc->b_freeze_change;
        }
        
        /* set the internal message */
        t_msg.h_svctx      = h_svctx;
        t_msg.ui1_msg_id   = _SVCTX_MSG_SELECT_SVC_REQ;
        t_msg.u.pt_svc_req = pt_svc_req;

        i4_ret = activator_send_msg(
                            pt_g_activator,
                            SVCTX_MSGQ_DEFAULT_PRIORITY,
                            &t_msg
                            );
        i4_ret = (i4_ret != ACTVTRR_OK) ? SVCTXR_FAIL: SVCTXR_OK;
#endif
    } while (0);
    svctx_class_unlock();

#ifndef MW_TV_AV_BYPASS_SUPPORT
    if (i4_ret != SVCTXR_OK && pt_svc_req != NULL) {
        svctx_free_svc_req(pt_svc_req);
    }
#endif

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_surf_svc
 *
 * Description: This API is used to surf service from current service. It only allows
 *              the client already invoking x_svctx_select_svc to obtain the control of
 *              the service context. This API will uses the latest cached service
 *              selection parameters (source information, sink information, notification
 *              callback fucntion, notification tag) to surf service.
 *
 *              Basically, it will find the target service list record based on current
 *              service record id cached in current service context objcet,
 *              e_surf_mode (SURF_MODE_THIS, SURF_MODE_NEXT, and SURF_MODE_PREV),
 *              and ui4_nw_chk_mask and ui4_nw_chk_value.
 *
 *              The possible tagret service list record's ui4_nw_mask will be used to
 *              decide it is target service list record or not. For example,
 *              ui4_nw_chk_mask  = 0x00000007
 *              ui4_nw_chk_value = 0x00000073
 *              The API will find the service list record's ui4_nw_mask & ui4_nw_chk_mask
 *              is equals to ui4_nw_chk_mask & ui4_nw_chk_value,
 *              i.e. the first  bit of service list record's ui4_nw_mask = 1
 *                   the second bit of service list record's ui4_nw_mask = 1
 *                   the third bit of service list record's ui4_nw_mask  = 0
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          ui4_nw_chk_mask    Contains the target SVL record's network check mask
 *          ui4_nw_chk_value   Contains the target SVL record's network value for
 *                             comparison.
 *          e_surf_mode        Contains the variable for surf service direction
 * Outputs: pt_svl_rec         References to the structure containing the
 *                             target SVL record.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     ui4_nw_chk_mask = 0
 *          SVCTXR_NO_RECORD   Cannot find any service list record to surf
 *          SVCTXR_NO_RESOURCE No enough memory to allocate this request.
 *          SVCTXR_FAIL        No any previous service selection request is issued.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_surf_svc(
                    HANDLE_T                    h_svctx,
                    UINT32                      ui4_nw_chk_mask,
                    UINT32                      ui4_nw_chk_value,
                    SURF_MODE_T                 e_surf_mode,
                    SVL_REC_T*                  pt_svl_rec
                    )
{
    SVCTX_T*    pt_svctx   = NULL;
    INT32       i4_ret     = SVCTXR_OK;
    SVC_REQ_T*  pt_svc_req = NULL;
    SVCTX_MSG_T t_msg      = {0};

    CLI_FUNC_CALL_TRACE(__func__);

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (ui4_nw_chk_mask == 0) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_surf_svc_req == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

#ifdef MW_RIKS_TV_SUPPORT
        if (svctx_rikstv_in_tuning_state(pt_svctx))
        {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }
#endif

        pt_svc_req = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_surf_svc_req(
                            pt_svctx,
                            h_svctx,
                            pt_svctx->pt_running_svc_req->pt_hdlr,
                            ui4_nw_chk_mask,
                            ui4_nw_chk_value,
                            e_surf_mode,
                            pt_svl_rec
                            );
        if (pt_svc_req == NULL) {
            DBG_ERROR((_ERROR_HEADER"create SVC_REQ_T failed!\r\n"));
            i4_ret = SVCTXR_NO_RESOURCE;
            break;
        }

        /* set the internal message */
        t_msg.h_svctx      = h_svctx;
        t_msg.ui1_msg_id   = _SVCTX_MSG_SELECT_SVC_REQ;
        t_msg.u.pt_svc_req = pt_svc_req;

        i4_ret = activator_send_msg(
                            pt_g_activator,
                            SVCTX_MSGQ_DEFAULT_PRIORITY,
                            &t_msg
                            );
        i4_ret = (i4_ret != ACTVTRR_OK) ? SVCTXR_FAIL: SVCTXR_OK;
    } while (0);
    svctx_class_unlock();

    if (i4_ret != SVCTXR_OK && pt_svc_req != NULL) {
        svctx_free_svc_req(pt_svc_req);
    }

    return i4_ret;
}

INT32 x_svctx_get_strm_req_type(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type,        /* in  */
                    STRM_REQ_TYPE_T*                pe_strm_req_type    /* out */
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "e_strm_type = %d, "
                    "pe_strm_req_type = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_strm_type,
                    pe_strm_req_type
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pe_strm_req_type == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_strm_req_type == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_strm_req_type(
                        pt_svctx,
                        e_strm_type,
                        pe_strm_req_type
                        );
    } while (0);
    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_get_using_scdb_rec
 *
 * Description: This API retrieves the selected stream of a specific type from
 *              a presenting service context object.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          e_strm_type        Contains the stream component type for filtering
 *
 * Outputs: pt_scdb_rec        References to the structure containing the
 *                             stream component data.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_ARG     pt_scdb_rec is NULL.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NOT_EXIST   The stream component is not existed.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_FAIL        SCDB error, can not get the record
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_get_using_scdb_rec(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type,        /* in  */
                    SCDB_REC_T*                     pt_scdb_rec         /* out */
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "e_strm_type = %d, "
                    "pt_scdb_rec = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_strm_type,
                    pt_scdb_rec
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pt_scdb_rec == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_using_scdb_info == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_using_scdb_info(
                        pt_svctx,
                        e_strm_type,
                        NULL,
                        pt_scdb_rec
                        );
    } while (0);
    svctx_class_unlock();
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_svctx_get_using_strm_data
 *
 * Description: This API retrieves the data of selected stream of a specific type from
 *              a presenting service context object.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          e_strm_type        Contains the stream component type for filtering
 *
 * Outputs: pt_comp_id         References to the structure containing the
 *                             stream component id.
 *          pt_scdb_rec        References to the structure containing the
 *                             stream component data.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_ARG     pt_comp_id or pt_scdb_rec is NULL.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NOT_EXIST   The stream component is not existed.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_FAIL        SCDB error, can not get the record
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_get_using_strm_data(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type,        /* in  */
                    STREAM_COMP_ID_T*               pt_comp_id,         /* out */
                    SCDB_REC_T*                     pt_scdb_rec         /* out */
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "e_strm_type = %d, "
                    "pt_comp_id = 0x%x, "
                    "pt_scdb_rec = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_strm_type,
                    pt_comp_id,
                    pt_scdb_rec
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pt_scdb_rec == NULL || pt_comp_id == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_using_scdb_info == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_using_scdb_info(
                        pt_svctx,
                        e_strm_type,
                        pt_comp_id,
                        pt_scdb_rec
                        );
    } while (0);
    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_get_stream_comp_id
 *
 * Description: This API retrieves the stream id and stream data from
 *              stream component database by a specific type and a filter
 *              function provided by caller. The index sent to the filter
 *              function will be from 0 to total amount of this specific
 *              stream type, and when index = total amount, it means the
 *              ineration is looped back to first one component. This approach
 *              can provide client a way to get the first one when the target
 *              cannot be found.
 *              If the data is not populated into stream component database,
 *              caller will get a SVCTXR_WRONG_STATE return value.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          e_strm_type        Contains the stream component type for filtering
 *          pf_filter          References to the filter function provided by caller.
 *                             It can't be NULL.
 *          pv_fltr_tag        References to a caller-defined tag. It will pass to
 *                             to caller when the pf_filter is invoked.
 * Outputs: pt_comp_id         References to the structure containing the
 *                             stream component id. It can't be NULL.
 *          pt_scdb_rec        References to the structure containing the
 *                             stream component data.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pf_filter or pt_comp_id is NULL.
 *          SVCTXR_WRONG_STATE If related SCDB is not ready and opened.
 *          SVCTXR_NOT_EXIST   The stream component is not existed.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_get_stream_comp_id(
                    HANDLE_T                     h_svctx,            /* in  */
                    STREAM_TYPE_T                e_strm_type,        /* in  */
                    x_svctx_strm_comp_filter_fct pf_filter,          /* in  */
                    VOID*                        pv_tag,             /* in  */
                    STREAM_COMP_ID_T*            pt_comp_id,         /* out */
                    SCDB_REC_T*                  pt_scdb_rec         /* out */
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "e_strm_type = %d, "
                    "pf_filter = 0x%x, "
                    "pv_tag = 0x%x, "
                    "pt_comp_id = 0x%x, "
                    "pt_scdb_rec = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_strm_type,
                    pf_filter,
                    pv_tag,
                    pt_comp_id,
                    pt_scdb_rec
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        i4_ret = svctx_get_strm_comp_id(
                    pt_svctx,
                    e_strm_type,
                    TRUE,
                    pf_filter,
                    pv_tag,
                    pt_comp_id,
                    pt_scdb_rec
                    );
    } while (0);
    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_select_cc_stream
 *
 * Description: This API selects a specific closed captioning stream in
 *              asynchronous way. If the cc stream is selected successfully,
 *              caller will be notified via the pf_nfy with
 *                  e_nfy_cond    = SVCTX_COND_PRESENTING,
 *                  e_code        = SVCTX_NTFY_CODE_STREAM_STARTED
 *                  e_stream_type = ST_CLOSED_CAPTION
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          h_cc_gl_plane      Contains a handle to a graphic plane for closed
 *                             captioning handler to draw.
 *          pt_cc_filter       References to the structure containing the
 *                             cc filter data.
 *          pf_nfy             References the stream notify function.
 *          pv_nfy_tag         References the tag passed to the notify function.
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_ARG     pt_cc_filter is NULL or h_cc_gl_plane is NULL_HANDLE.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NO_RESOURCE Not enough memory to allocate this request.
 *          SVCTXR_FAIL        Send request failed
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_select_cc_stream(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_cc_gl_plane,    /* in  */
                    const SM_CC_FILTER_T*           pt_cc_filter,     /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    )
{
    INT32       i4_ret      = SVCTXR_OK;
    SVCTX_MSG_T t_msg       = {0};
    STRM_REQ_T* pt_strm_req = NULL;
    SVCTX_T*    pt_svctx    = NULL;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "h_cc_gl_plane = %d, "
                    "pt_cc_filter = 0x%x, "
                    "pf_nfy = 0x%x, "
                    "pv_nfy_tag = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    h_cc_gl_plane,
                    pt_cc_filter,
                    pf_nfy,
                    pv_nfy_tag
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        CH_STRM_SEL_INFO_T t_info;

        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if ((pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_CLOSED_CAPTION) > 0) {
            if (!(pt_cc_filter != NULL && SVCTX_IS_INVISIBLE_CC_FILTER((*pt_cc_filter)))) {
                i4_ret =  SVCTXR_FROZEN;
                break;
            }
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        t_info.e_type              = CH_STRM_SEL_INFO_TYPE_CC;
        t_info.u.t_cc.h_gl_plane   = h_cc_gl_plane;
        t_info.u.t_cc.pt_cc_filter = (SM_CC_FILTER_T*)pt_cc_filter;

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req(
                            pt_svctx,
                            h_svctx,
                            &t_info,
                            pf_nfy,
                            pv_nfy_tag,
                            &pt_strm_req
                            );
        if (i4_ret != SVCTXR_OK) {
            DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
            break;
        }

        t_msg.h_svctx       = h_svctx;
        t_msg.ui1_msg_id    = _SVCTX_MSG_SELECT_STRM_REQ;
        t_msg.u.pt_strm_req = pt_strm_req;

        i4_ret = activator_send_msg(
                            pt_g_activator,
                            SVCTX_MSGQ_DEFAULT_PRIORITY,
                            &t_msg
                            );
        i4_ret = (i4_ret != ACTVTRR_OK) ? SVCTXR_FAIL: SVCTXR_OK;
    } while (0);

    if (i4_ret != SVCTXR_OK) {
        svctx_free_strm_req(pt_strm_req);
    }

    svctx_class_unlock();
    return i4_ret;
}
#ifdef MW_ISDB_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: x_svctx_select_isdb_caption_stream
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_select_isdb_caption_stream(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_gl_plane,       /* in  */
                    UINT32                          ui4_caption_idx,  /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    )
{
    INT32       i4_ret      = SVCTXR_OK;
    SVCTX_MSG_T t_msg       = {0};
    STRM_REQ_T* pt_strm_req = NULL;
    SVCTX_T*    pt_svctx    = NULL;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "h_gl_plane = 0x%x, "
                    "ui4_caption_idx = %d, "
                    "pf_nfy = 0x%x, "
                    "pv_nfy_tag = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    h_gl_plane,
                    ui4_caption_idx,
                    pf_nfy,
                    pv_nfy_tag
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        CH_STRM_SEL_INFO_T t_info;

        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if ((pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_ISDB_CAPTION) > 0) {
            i4_ret =  SVCTXR_FROZEN;
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        t_info.e_type                           = CH_STRM_SEL_INFO_TYPE_ISDB_CAPTION;
        t_info.u.t_isdb_caption.h_gl_plane      = h_gl_plane;
        t_info.u.t_isdb_caption.ui4_caption_idx = ui4_caption_idx;

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req(
                            pt_svctx,
                            h_svctx,
                            &t_info,
                            pf_nfy,
                            pv_nfy_tag,
                            &pt_strm_req
                            );
        if (i4_ret != SVCTXR_OK) {
            DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
            break;
        }

        t_msg.h_svctx       = h_svctx;
        t_msg.ui1_msg_id    = _SVCTX_MSG_SELECT_STRM_REQ;
        t_msg.u.pt_strm_req = pt_strm_req;

        i4_ret = activator_send_msg(
                            pt_g_activator,
                            SVCTX_MSGQ_DEFAULT_PRIORITY,
                            &t_msg
                            );
        i4_ret = (i4_ret != ACTVTRR_OK) ? SVCTXR_FAIL: SVCTXR_OK;
    } while (0);

    if (i4_ret != SVCTXR_OK) {
        svctx_free_strm_req(pt_strm_req);
    }

    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_select_isdb_text_stream
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_select_isdb_text_stream(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_gl_plane,       /* in  */
                    UINT32                          ui4_text_idx,     /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    )
{
    INT32       i4_ret      = SVCTXR_OK;
    SVCTX_MSG_T t_msg       = {0};
    STRM_REQ_T* pt_strm_req = NULL;
    SVCTX_T*    pt_svctx    = NULL;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "h_gl_plane = 0x%x, "
                    "ui4_text_idx = %d, "
                    "pf_nfy = 0x%x, "
                    "pv_nfy_tag = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    h_gl_plane,
                    ui4_text_idx,
                    pf_nfy,
                    pv_nfy_tag
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        CH_STRM_SEL_INFO_T t_info;

        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if ((pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_ISDB_TEXT) > 0) {
            i4_ret =  SVCTXR_FROZEN;
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        t_info.e_type                     = CH_STRM_SEL_INFO_TYPE_ISDB_TEXT;
        t_info.u.t_isdb_text.h_gl_plane   = h_gl_plane;
        t_info.u.t_isdb_text.ui4_text_idx = ui4_text_idx;

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req(
                            pt_svctx,
                            h_svctx,
                            &t_info,
                            pf_nfy,
                            pv_nfy_tag,
                            &pt_strm_req
                            );
        if (i4_ret != SVCTXR_OK) {
            DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
            break;
        }

        t_msg.h_svctx       = h_svctx;
        t_msg.ui1_msg_id    = _SVCTX_MSG_SELECT_STRM_REQ;
        t_msg.u.pt_strm_req = pt_strm_req;

        i4_ret = activator_send_msg(
                            pt_g_activator,
                            SVCTX_MSGQ_DEFAULT_PRIORITY,
                            &t_msg
                            );
        i4_ret = (i4_ret != ACTVTRR_OK) ? SVCTXR_FAIL: SVCTXR_OK;
    } while (0);

    if (i4_ret != SVCTXR_OK) {
        svctx_free_strm_req(pt_strm_req);
    }

    svctx_class_unlock();
    return i4_ret;
}
#endif
/*-----------------------------------------------------------------------------
 * Name: x_svctx_select_subtitle_stream
 *
 * Description: This API selects a specific subtitle stream in
 *              asynchronous way. If the subtitle stream is selected successfully,
 *              caller will be notified via the pf_nfy with
 *                  e_code        = SVCTX_NTFY_CODE_STREAM_STARTED
 *                  e_stream_type = ST_SUBTITLE
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          h_gl_plane         Contains a handle to a graphic plane for closed
 *                             captioning handler to draw.
 *          pt_lang            References to the structure containing the
 *                             language code.
 *          pf_nfy             References the stream notify function.
 *          pv_nfy_tag         References the tag passed to the notify function.
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_ARG     pt_lang is NULL or h_gl_plane is NULL_HANDLE.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NO_RESOURCE Not enough memory to allocate this request.
 *          SVCTXR_FAIL        Send request failed
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_select_subtitle_stream(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_gl_plane,       /* in  */
                    const ISO_639_LANG_T*           pt_lang,          /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    )
{
    INT32       i4_ret      = SVCTXR_OK;
    SVCTX_MSG_T t_msg       = {0};
    STRM_REQ_T* pt_strm_req = NULL;
    SVCTX_T*    pt_svctx    = NULL;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "h_gl_plane = %d, "
                    "pt_lang = %s, "
                    "pf_nfy = 0x%x, "
                    "pv_nfy_tag = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    h_gl_plane,
                    pt_lang,
                    pf_nfy,
                    pv_nfy_tag
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        CH_STRM_SEL_INFO_T t_info;

        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if ((pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_SUBTITLE) > 0) {
            i4_ret =  SVCTXR_FROZEN;
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        x_memset(&t_info, 0, sizeof(CH_STRM_SEL_INFO_T));
        t_info.e_type                  = CH_STRM_SEL_INFO_TYPE_SUBTITLE;
        t_info.u.t_subtitle.h_gl_plane = h_gl_plane;
        t_info.u.t_subtitle.pt_lang    = (ISO_639_LANG_T*)pt_lang;

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req(
                            pt_svctx,
                            h_svctx,
                            &t_info,
                            pf_nfy,
                            pv_nfy_tag,
                            &pt_strm_req
                            );
        if (i4_ret != SVCTXR_OK) {
            DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
            break;
        }

        t_msg.h_svctx       = h_svctx;
        t_msg.ui1_msg_id    = _SVCTX_MSG_SELECT_STRM_REQ;
        t_msg.u.pt_strm_req = pt_strm_req;

        i4_ret = activator_send_msg(
                            pt_g_activator,
                            SVCTX_MSGQ_DEFAULT_PRIORITY,
                            &t_msg
                            );
        i4_ret = (i4_ret != ACTVTRR_OK) ? SVCTXR_FAIL: SVCTXR_OK;
    } while (0);

    if (i4_ret != SVCTXR_OK) {
        svctx_free_strm_req(pt_strm_req);
    }

    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_select_subtitle_stream_by_comp_id
 *
 * Description: This API selects a specific subtitle stream in
 *              asynchronous way. If the subtitle stream is selected successfully,
 *              caller will be notified via the pf_nfy with
 *                  e_code        = SVCTX_NTFY_CODE_STREAM_STARTED
 *                  e_stream_type = ST_SUBTITLE
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          h_gl_plane         Contains a handle to a graphic plane for closed
 *                             captioning handler to draw.
 *          pt_comp_id         References to the structure containing the
 *                             stream component id.
 *          pf_nfy             References the stream notify function.
 *          pv_nfy_tag         References the tag passed to the notify function.
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_ARG     pt_lang is NULL or h_gl_plane is NULL_HANDLE.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NO_RESOURCE Not enough memory to allocate this request.
 *          SVCTXR_FAIL        Send request failed
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_select_subtitle_stream_by_comp_id(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_gl_plane,       /* in  */
                    const STREAM_COMP_ID_T*         pt_comp_id,       /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    )
{
    INT32       i4_ret      = SVCTXR_OK;
    SVCTX_MSG_T t_msg       = {0};
    STRM_REQ_T* pt_strm_req = NULL;
    SVCTX_T*    pt_svctx    = NULL;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "h_gl_plane = %d, "
                    "pt_comp_id = 0x%x, "
                    "pf_nfy = 0x%x, "
                    "pv_nfy_tag = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    h_gl_plane,
                    pt_comp_id,
                    pf_nfy,
                    pv_nfy_tag
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        CH_STRM_SEL_INFO_T t_info;

        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if ((pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_SUBTITLE) > 0) {
            i4_ret =  SVCTXR_FROZEN;
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        x_memset(&t_info, 0, sizeof(CH_STRM_SEL_INFO_T));
        t_info.e_type                          = CH_STRM_SEL_INFO_TYPE_GENERIC_SUBTITLE;
        t_info.u.t_subtitle_generic.h_gl_plane = h_gl_plane;
        t_info.u.t_subtitle_generic.pt_comp_id = (STREAM_COMP_ID_T*)pt_comp_id;

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req(
                            pt_svctx,
                            h_svctx,
                            &t_info,
                            pf_nfy,
                            pv_nfy_tag,
                            &pt_strm_req
                            );
        if (i4_ret != SVCTXR_OK) {
            DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
            break;
        }

        t_msg.h_svctx       = h_svctx;
        t_msg.ui1_msg_id    = _SVCTX_MSG_SELECT_STRM_REQ;
        t_msg.u.pt_strm_req = pt_strm_req;

        i4_ret = activator_send_msg(
                            pt_g_activator,
                            SVCTX_MSGQ_DEFAULT_PRIORITY,
                            &t_msg
                            );
        i4_ret = (i4_ret != ACTVTRR_OK) ? SVCTXR_FAIL: SVCTXR_OK;
    } while (0);

    if (i4_ret != SVCTXR_OK) {
        svctx_free_strm_req(pt_strm_req);
    }

    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_select_subtitle_stream_by_comp_id_ex
 *
 * Description: This API selects a specific subtitle stream in
 *              asynchronous way. If the subtitle stream is selected successfully,
 *              caller will be notified via the pf_nfy with
 *                  e_code        = SVCTX_NTFY_CODE_STREAM_STARTED
 *                  e_stream_type = ST_SUBTITLE
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          h_gl_plane         Contains a handle to a graphic plane for closed
 *                             captioning handler to draw.
 *          pt_sbtl_info       Contains extern information for subtitle playback.
 *          pt_comp_id         References to the structure containing the
 *                             stream component id.
 *          pf_nfy             References the stream notify function.
 *          pv_nfy_tag         References the tag passed to the notify function.
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_ARG     pt_lang is NULL or h_gl_plane is NULL_HANDLE.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NO_RESOURCE Not enough memory to allocate this request.
 *          SVCTXR_FAIL        Send request failed
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_select_subtitle_stream_by_comp_id_ex(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_gl_plane,       /* in  */
                    const SM_SBTLH_STREAM_INFO*     pt_sbtl_info,     /* in  */
                    const STREAM_COMP_ID_T*         pt_comp_id,       /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    )
{
    INT32       i4_ret      = SVCTXR_OK;
    SVCTX_MSG_T t_msg       = {0};
    STRM_REQ_T* pt_strm_req = NULL;
    SVCTX_T*    pt_svctx    = NULL;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "h_gl_plane = %d, "
                    "pt_comp_id = 0x%x, "
                    "pf_nfy = 0x%x, "
                    "pv_nfy_tag = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    h_gl_plane,
                    pt_comp_id,
                    pf_nfy,
                    pv_nfy_tag
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        CH_STRM_SEL_INFO_T t_info;

        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if ((pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_SUBTITLE) > 0) {
            i4_ret =  SVCTXR_FROZEN;
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        x_memset(&t_info, 0, sizeof(CH_STRM_SEL_INFO_T));
        t_info.e_type                            = CH_STRM_SEL_INFO_TYPE_GENERIC_SUBTITLE;
        t_info.u.t_subtitle_generic.h_gl_plane   = h_gl_plane;
        t_info.u.t_subtitle_generic.pt_sbtl_info = (SM_SBTLH_STREAM_INFO*)pt_sbtl_info;
        t_info.u.t_subtitle_generic.pt_comp_id   = (STREAM_COMP_ID_T*)pt_comp_id;

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req(
                            pt_svctx,
                            h_svctx,
                            &t_info,
                            pf_nfy,
                            pv_nfy_tag,
                            &pt_strm_req
                            );
        if (i4_ret != SVCTXR_OK) {
            DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
            break;
        }

        t_msg.h_svctx       = h_svctx;
        t_msg.ui1_msg_id    = _SVCTX_MSG_SELECT_STRM_REQ;
        t_msg.u.pt_strm_req = pt_strm_req;

        i4_ret = activator_send_msg(
                            pt_g_activator,
                            SVCTX_MSGQ_DEFAULT_PRIORITY,
                            &t_msg
                            );
        i4_ret = (i4_ret != ACTVTRR_OK) ? SVCTXR_FAIL: SVCTXR_OK;
    } while (0);

    if (i4_ret != SVCTXR_OK) {
        svctx_free_strm_req(pt_strm_req);
    }

    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_select_audio_stream
 *
 * Description: This API selects a specific audio stream via ISO639 language
 *              code in asynchronous way.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          pt_lang            References to the structure containing the
 *                             language code.
 *          pf_nfy             References the stream notify function.
 *          pv_nfy_tag         References the tag passed to the notify function.
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_ARG     pt_lang is NULL.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NO_RESOURCE Not enough memory to allocate this request.
 *          SVCTXR_FAIL        Send request failed
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_NOT_EXIST   The stream component is not existed.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_select_audio_stream(
                    HANDLE_T                        h_svctx,            /* in  */
                    const ISO_639_LANG_T*           pt_lang,            /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,             /* in  */
                    VOID*                           pv_nfy_tag          /* in  */
                    )
{
    INT32       i4_ret      = SVCTXR_OK;
    SVCTX_MSG_T t_msg       = {0};
    STRM_REQ_T* apt_strm_req[STRM_MAX_CONCURRENT] = {NULL, NULL};
    SVCTX_T*    pt_svctx    = NULL;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    ACTIVE_LINK_ENTRY_T*    pt_link;
    SVCTX_T*                pt_obj;
    INT32                   i = 0;
#endif

    CLI_FUNC_CALL_TRACE(__func__);
    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pt_lang = %s, "
                    "pf_nfy = 0x%x, "
                    "pv_nfy_tag = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    (pt_lang == NULL ? "NULL" : (CHAR*)pt_lang),
                    pf_nfy,
                    pv_nfy_tag
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        CH_STRM_SEL_INFO_T t_info;

        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->b_select_strm_block)
        {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if ((pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_AUDIO) > 0) {
            i4_ret =  SVCTXR_FROZEN;
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        t_info.e_type = CH_STRM_SEL_INFO_TYPE_AUDIO;
        t_info.u.t_audio.pt_lang = (ISO_639_LANG_T*) pt_lang;

#ifdef MW_TV_AV_BYPASS_SUPPORT
        while (1)
        {
            pt_link = svctx_mgr_get_link_obj(
                ACTIVE_LINK_NOW,
                pt_svctx->pt_running_svc_req->e_src_type,
                i++);

            if (!pt_link)
            {
                break;
            }

            pt_obj = pt_link->pt_worker;

            if (pt_obj == pt_svctx)
            {
#endif
                i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req(
                                    pt_svctx,
                                    h_svctx,
                                    &t_info,
                                    pf_nfy,
                                    pv_nfy_tag,
                                    (STRM_REQ_T**) apt_strm_req
                                    );
#ifdef MW_TV_AV_BYPASS_SUPPORT
            }
            else
            {
                if (pt_obj->pt_running_svc_req == NULL) {
                    i4_ret = SVCTXR_WRONG_STATE;
                    continue;
                }

                if (pt_obj->pt_running_svc_req->pt_hdlr == NULL) {
                    i4_ret = SVCTXR_FAIL;
                    continue;
                }

                if (pt_obj->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req == NULL) {
                    i4_ret = SVCTXR_NOT_SUPPORT;
                    continue;
                }

                i4_ret = pt_obj->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req(
                                    pt_obj,
                                    pt_obj->h_sys_svctx,
                                    &t_info,
                                    NULL,
                                    NULL,
                                    (STRM_REQ_T**) apt_strm_req
                                    );
            }
#endif
            if (i4_ret != SVCTXR_OK) {
                DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
                break;
            }

            /* send message for playing the normal audio stream */
#ifdef MW_TV_AV_BYPASS_SUPPORT
            t_msg.h_svctx       = pt_obj->h_sys_svctx;
#else
            t_msg.h_svctx       = h_svctx;
#endif
            t_msg.ui1_msg_id    = _SVCTX_MSG_SELECT_STRM_REQ;
            t_msg.u.pt_strm_req = apt_strm_req[STRM_IDX_NORMAL];

            i4_ret = activator_send_msg(
                                pt_g_activator,
                                SVCTX_MSGQ_DEFAULT_PRIORITY,
                                &t_msg
                                );

            if (i4_ret != ACTVTRR_OK) {
                svctx_free_strm_req(apt_strm_req[STRM_IDX_NORMAL]);
                if (apt_strm_req[STRM_IDX_VISUAL_IMPAIRED])
                {
                    svctx_free_strm_req(apt_strm_req[STRM_IDX_VISUAL_IMPAIRED]);
                }
                i4_ret = SVCTXR_FAIL;
                break;
            }

#ifdef MW_TV_AV_BYPASS_SUPPORT
            if (!IS_SVCTX_ASSOCIATE(pt_obj))
#endif
            {
                /* send message for playing the visual impaired audio stream */
                if (apt_strm_req[STRM_IDX_VISUAL_IMPAIRED])
                {
                    t_msg.u.pt_strm_req = apt_strm_req[STRM_IDX_VISUAL_IMPAIRED];

                    i4_ret = activator_send_msg(
                                        pt_g_activator,
                                        SVCTX_MSGQ_DEFAULT_PRIORITY,
                                        &t_msg
                                        );

                    if (i4_ret != ACTVTRR_OK) {
                        svctx_free_strm_req(apt_strm_req[STRM_IDX_VISUAL_IMPAIRED]);
                        i4_ret = SVCTXR_OK;
                        break;
                    }
                }
            }
#ifdef MW_TV_AV_BYPASS_SUPPORT
        }
#endif
    } while(0);

    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_select_stream
 *
 * Description: This API is used to selects a specific stream via stream component id
 *              in asynchronous way.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          pt_comp_id         References to the structure containing the
 *                             stream component id.
 *          pf_nfy             References the stream notify function.
 *          pv_nfy_tag         References the tag passed to the notify function.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NO_RESOURCE Not enough memory to allocate this request.
 *          SVCTXR_INV_ARG     pt_comp_id is NULL.
 *          SVCTXR_FAIL        Send request failed
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_NOT_EXIST   The related SCDB record is not existed.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_select_stream(
                    HANDLE_T                        h_svctx,
                    const STREAM_COMP_ID_T*         pt_comp_id,
                    x_svctx_select_nfy_fct          pf_nfy,
                    VOID*                           pv_nfy_tag
                    )
{
    INT32       i4_ret      = SVCTXR_OK;
    SVCTX_MSG_T t_msg       = {0};
    STRM_REQ_T* apt_strm_req[STRM_MAX_CONCURRENT] = {NULL, NULL};
    SVCTX_T*    pt_svctx    = NULL;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    ACTIVE_LINK_ENTRY_T*    pt_link;
    SVCTX_T*                pt_obj;
    INT32                   i = 0;
#endif

    CLI_FUNC_CALL_TRACE_FOR_STRM(__func__,  pt_comp_id->e_type);
    DBG_API((_API_HEADER"%s("
                    "h_svctx       = %d, "
                    "pt_comp_id    = 0x%x, "
                    "pf_nfy        = 0x%x, "
                    "pv_nfy_tag    = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pt_comp_id,
                    pf_nfy,
                    pv_nfy_tag
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        CH_STRM_SEL_INFO_T t_info;

        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->b_select_strm_block)
        {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if ((pt_svctx->ui4_frz_frozen_strm_mask & MAKE_BIT_MASK_32(pt_comp_id->e_type)) > 0) {
            i4_ret = SVCTXR_FROZEN;
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        t_info.e_type = CH_STRM_SEL_INFO_TYPE_GENERIC;
        t_info.u.t_generic.pt_comp_id = (STREAM_COMP_ID_T*)pt_comp_id;

#ifdef MW_TV_AV_BYPASS_SUPPORT
        while (1)
        {
            pt_link = svctx_mgr_get_link_obj(
                ACTIVE_LINK_NOW,
                pt_svctx->pt_running_svc_req->e_src_type,
                i++);

            if (!pt_link)
            {
                break;
            }

            pt_obj = pt_link->pt_worker;

            if (pt_obj == pt_svctx)
            {
#endif
                i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req(
                                    pt_svctx,
                                    h_svctx,
                                    &t_info,
                                    pf_nfy,
                                    pv_nfy_tag,
                                    (STRM_REQ_T**) apt_strm_req
                                    );

                t_msg.h_svctx = h_svctx;

#ifdef MW_TV_AV_BYPASS_SUPPORT
            }
            else
            {
                if (FALSE == _svctx_is_type_supported(pt_obj, pt_comp_id->e_type))
                {
                    continue;
                }

                if (pt_obj->pt_running_svc_req == NULL) {
                    i4_ret = SVCTXR_WRONG_STATE;
                    continue;
                }

                if (pt_obj->pt_running_svc_req->pt_hdlr == NULL) {
                    i4_ret = SVCTXR_FAIL;
                    continue;
                }

                if (pt_obj->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req == NULL) {
                    i4_ret = SVCTXR_NOT_SUPPORT;
                    continue;
                }

                i4_ret = pt_obj->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req(
                                    pt_obj,
                                    pt_obj->h_sys_svctx,
                                    &t_info,
                                    NULL,
                                    NULL,
                                    (STRM_REQ_T**) apt_strm_req
                                    );

                t_msg.h_svctx = pt_obj->h_sys_svctx;
            }
#endif
            if (i4_ret != SVCTXR_OK) {
                DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
                break;
            }

            /* send message for playing the normal stream */
            t_msg.ui1_msg_id    = _SVCTX_MSG_SELECT_STRM_REQ;
            t_msg.u.pt_strm_req = apt_strm_req[STRM_IDX_NORMAL];

            i4_ret = activator_send_msg(
                                pt_g_activator,
                                SVCTX_MSGQ_DEFAULT_PRIORITY,
                                &t_msg
                                );

            if (i4_ret != ACTVTRR_OK) {
                svctx_free_strm_req(apt_strm_req[STRM_IDX_NORMAL]);
                if (apt_strm_req[STRM_IDX_VISUAL_IMPAIRED])
                {
                    svctx_free_strm_req(apt_strm_req[STRM_IDX_VISUAL_IMPAIRED]);
                }
                i4_ret = SVCTXR_FAIL;
                break;
            }

#ifdef MW_TV_AV_BYPASS_SUPPORT
            if (!IS_SVCTX_ASSOCIATE(pt_obj))
#endif
            {
                /* send message for playing the visual impaired audio stream */
                if (apt_strm_req[STRM_IDX_VISUAL_IMPAIRED])
                {
                    /* Dolby MS10 certification: 
                     * ad not allowed when main is dual mono
                     */
                    STRM_REQ_T* pt_aud_req = apt_strm_req[STRM_IDX_NORMAL];
                    if (AUD_FMT_DUAL_MONO == pt_aud_req->u.t_scdb.t_rec.u.t_audio_mpeg.e_aud_fmt)
                    {
                        svctx_free_strm_req(apt_strm_req[STRM_IDX_VISUAL_IMPAIRED]);
                        i4_ret = SVCTXR_OK;
                    }
                    else
                    {
                        t_msg.u.pt_strm_req = apt_strm_req[STRM_IDX_VISUAL_IMPAIRED];

                        i4_ret = activator_send_msg(
                                            pt_g_activator,
                                            SVCTX_MSGQ_DEFAULT_PRIORITY,
                                            &t_msg
                                            );

                        if (i4_ret != ACTVTRR_OK) {
                            svctx_free_strm_req(apt_strm_req[STRM_IDX_VISUAL_IMPAIRED]);
                            i4_ret = SVCTXR_OK;
                            break;
                        }
                    }
                }
                else if ((ST_AUDIO == pt_comp_id->e_type) && (pt_svctx->b_ad_enabled))
                {
                    t_msg.ui1_msg_id                      = _SVCTX_MSG_STOP_STRM_REQ;
                    t_msg.u.t_stop_strm_req.e_type        = ST_AUDIO;
                    t_msg.u.t_stop_strm_req.t_sync.pf_nfy = NULL;
                    t_msg.u.t_stop_strm_req.t_sync.pv_tag = NULL;
                    t_msg.u.t_stop_strm_req.b_aux_audio   = TRUE;

                    i4_ret = svctx_send_msg(h_svctx, &t_msg);
                }
            }
#ifdef MW_TV_AV_BYPASS_SUPPORT
        }
#endif
    } while (0);

    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_select_mpeg_stream_by_pid
 *
 * Description: This API is used to selects a specific stream via PID
 *              in asynchronous way.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          e_strm_type        Contains the stream type for selecting
 *          t_mpeg_2_pid       Contains the MPEG2 PID value
 *          pf_nfy             References the stream notify function.
 *          pv_nfy_tag         References the tag passed to the notify function.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NO_RESOURCE Not enough memory to allocate this request.
 *          SVCTXR_INV_ARG     pt_comp_id is NULL.
 *          SVCTXR_FAIL        Send request failed
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_NOT_EXIST   The related SCDB record is not existed.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_select_mpeg_stream_by_pid(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_strm_type,
                    MPEG_2_PID_T                    t_mpeg_2_pid,
                    x_svctx_select_nfy_fct          pf_nfy,
                    VOID*                           pv_nfy_tag
                    )
{
    INT32       i4_ret      = SVCTXR_OK;
    SVCTX_MSG_T t_msg       = {0};
    STRM_REQ_T* pt_strm_req = NULL;
    SVCTX_T*    pt_svctx    = NULL;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    ACTIVE_LINK_ENTRY_T*    pt_link;
    SVCTX_T*                pt_obj;
    INT32                   i = 0;
#endif

    CLI_FUNC_CALL_TRACE_FOR_STRM(__func__, e_strm_type);

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        CH_STRM_SEL_INFO_T t_info;

        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if ((pt_svctx->ui4_frz_frozen_strm_mask & MAKE_BIT_MASK_32(e_strm_type)) > 0) {
            i4_ret = SVCTXR_FROZEN;
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        t_info.e_type = CH_STRM_SEL_INFO_TYPE_MEPG_PID;
        t_info.u.t_mpeg.e_strm_type  = e_strm_type;
        t_info.u.t_mpeg.t_mpeg_2_pid = t_mpeg_2_pid;

#ifdef MW_TV_AV_BYPASS_SUPPORT
        while (1)
        {
            pt_link = svctx_mgr_get_link_obj(
                ACTIVE_LINK_NOW,
                pt_svctx->pt_running_svc_req->e_src_type,
                i++);

            if (!pt_link)
            {
                break;
            }

            pt_obj = pt_link->pt_worker;

            if (pt_obj == pt_svctx)
            {
#endif
                i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req(
                                    pt_svctx,
                                    h_svctx,
                                    &t_info,
                                    pf_nfy,
                                    pv_nfy_tag,
                                    &pt_strm_req
                                    );

                t_msg.h_svctx = h_svctx;
#ifdef MW_TV_AV_BYPASS_SUPPORT
            }
            else
            {
                if (FALSE == _svctx_is_type_supported(pt_obj, e_strm_type))
                {
                    continue;
                }

                if (pt_obj->pt_running_svc_req == NULL) {
                    i4_ret = SVCTXR_WRONG_STATE;
                    continue;
                }

                if (pt_obj->pt_running_svc_req->pt_hdlr == NULL) {
                    i4_ret = SVCTXR_FAIL;
                    continue;
                }

                if (pt_obj->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req == NULL) {
                    i4_ret = SVCTXR_NOT_SUPPORT;
                    continue;
                }

                i4_ret = pt_obj->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req(
                                    pt_obj,
                                    pt_obj->h_sys_svctx,
                                    &t_info,
                                    NULL,
                                    NULL,
                                    &pt_strm_req
                                    );

                t_msg.h_svctx = pt_obj->h_sys_svctx;
            }
#endif

            if (i4_ret != SVCTXR_OK) {
                DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
                break;
            }

            t_msg.ui1_msg_id    = _SVCTX_MSG_SELECT_STRM_REQ;
            t_msg.u.pt_strm_req = pt_strm_req;

            i4_ret = activator_send_msg(
                                pt_g_activator,
                                SVCTX_MSGQ_DEFAULT_PRIORITY,
                                &t_msg
                                );
            i4_ret = (i4_ret != ACTVTRR_OK) ? SVCTXR_FAIL: SVCTXR_OK;

#ifdef MW_TV_AV_BYPASS_SUPPORT
            if (i4_ret != SVCTXR_OK) {
                svctx_free_strm_req(pt_strm_req);
            }
        }
#endif
    } while (0);

#ifndef MW_TV_AV_BYPASS_SUPPORT
    if (i4_ret != SVCTXR_OK) {
        svctx_free_strm_req(pt_strm_req);
    }
#endif
    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_select_mpeg_stream_by_pid_ex
 *
 * Description: This API is used to selects a specific stream via PID
 *              in asynchronous way.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          pt_stream_info     References the stream info data structure.
 *          pf_nfy             References the stream notify function.
 *          pv_nfy_tag         References the tag passed to the notify function.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NO_RESOURCE Not enough memory to allocate this request.
 *          SVCTXR_INV_ARG     pt_comp_id is NULL.
 *          SVCTXR_FAIL        Send request failed
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_NOT_EXIST   The related SCDB record is not existed.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_select_mpeg_stream_by_pid_ex(
                    HANDLE_T                        h_svctx,
                    SVCTX_STREAM_INFO_T*            pt_stream_info,
                    x_svctx_select_nfy_fct          pf_nfy,
                    VOID*                           pv_nfy_tag
                    )
{
    INT32       i4_ret      = SVCTXR_OK;
    SVCTX_MSG_T t_msg       = {0};
    STRM_REQ_T* pt_strm_req = NULL;
    SVCTX_T*    pt_svctx    = NULL;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    ACTIVE_LINK_ENTRY_T*    pt_link;
    SVCTX_T*                pt_obj;
    INT32                   i = 0;
#endif

    CLI_FUNC_CALL_TRACE_FOR_STRM(__func__, pt_stream_info->e_strm_type);

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        CH_STRM_SEL_INFO_T t_info;

        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if ((pt_svctx->ui4_frz_frozen_strm_mask & MAKE_BIT_MASK_32(pt_stream_info->e_strm_type)) > 0) {
            i4_ret = SVCTXR_FROZEN;
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        t_info.e_type = CH_STRM_SEL_INFO_TYPE_MEPG_PID;
        t_info.u.t_mpeg = *(MEPG_PID_STRM_SEL_INFO_T*)pt_stream_info;

#ifdef MW_TV_AV_BYPASS_SUPPORT
        while (1)
        {
            pt_link = svctx_mgr_get_link_obj(
                ACTIVE_LINK_NOW,
                pt_svctx->pt_running_svc_req->e_src_type,
                i++);

            if (!pt_link)
            {
                break;
            }

            pt_obj = pt_link->pt_worker;

            if (pt_obj == pt_svctx)
            {
#endif
                i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req(
                                    pt_svctx,
                                    h_svctx,
                                    &t_info,
                                    pf_nfy,
                                    pv_nfy_tag,
                                    &pt_strm_req
                                    );

                t_msg.h_svctx = h_svctx;
#ifdef MW_TV_AV_BYPASS_SUPPORT
            }
            else
            {
                if (FALSE == _svctx_is_type_supported(pt_obj, pt_stream_info->e_strm_type))
                {
                    continue;
                }

                if (pt_obj->pt_running_svc_req == NULL) {
                    i4_ret = SVCTXR_WRONG_STATE;
                    continue;
                }

                if (pt_obj->pt_running_svc_req->pt_hdlr == NULL) {
                    i4_ret = SVCTXR_FAIL;
                    continue;
                }

                if (pt_obj->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req == NULL) {
                    i4_ret = SVCTXR_NOT_SUPPORT;
                    continue;
                }

                i4_ret = pt_obj->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req(
                                    pt_obj,
                                    pt_obj->h_sys_svctx,
                                    &t_info,
                                    NULL,
                                    NULL,
                                    &pt_strm_req
                                    );

                t_msg.h_svctx = pt_obj->h_sys_svctx;
            }
#endif

            if (i4_ret != SVCTXR_OK) {
                DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
                break;
            }

            t_msg.ui1_msg_id    = _SVCTX_MSG_SELECT_STRM_REQ;
            t_msg.u.pt_strm_req = pt_strm_req;

            i4_ret = activator_send_msg(
                                pt_g_activator,
                                SVCTX_MSGQ_DEFAULT_PRIORITY,
                                &t_msg
                                );
            i4_ret = (i4_ret != ACTVTRR_OK) ? SVCTXR_FAIL: SVCTXR_OK;

#ifdef MW_TV_AV_BYPASS_SUPPORT
            if (i4_ret != SVCTXR_OK) {
                svctx_free_strm_req(pt_strm_req);
            }
        }
#endif
    } while (0);

#ifndef MW_TV_AV_BYPASS_SUPPORT
    if (i4_ret != SVCTXR_OK) {
        svctx_free_strm_req(pt_strm_req);
    }
#endif
    svctx_class_unlock();
    return i4_ret;
}


INT32 x_svctx_select_mpeg_ad_stream_by_pid(
                    HANDLE_T                        h_svctx,
                    SVCTX_STREAM_INFO_T*            pt_stream_info,
                    x_svctx_select_nfy_fct          pf_nfy,
                    VOID*                           pv_nfy_tag
                    )
{
    INT32       i4_ret      = SVCTXR_OK;
    SVCTX_MSG_T t_msg       = {0};
    STRM_REQ_T* pt_strm_req = NULL;
    SVCTX_T*    pt_svctx    = NULL;

    CLI_FUNC_CALL_TRACE_FOR_STRM(__func__, pt_stream_info->e_strm_type);

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        CH_STRM_SEL_INFO_T t_info;

        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if ((pt_svctx->ui4_frz_frozen_strm_mask & MAKE_BIT_MASK_32(pt_stream_info->e_strm_type)) > 0) {
            i4_ret = SVCTXR_FROZEN;
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        t_info.e_type = CH_STRM_SEL_INFO_TYPE_MEPG_PID;
        t_info.u.t_mpeg = *(MEPG_PID_STRM_SEL_INFO_T*)pt_stream_info;

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req(
                pt_svctx,
                h_svctx,
                &t_info,
                pf_nfy,
                pv_nfy_tag,
                &pt_strm_req
                );
        if (i4_ret != SVCTXR_OK) {
            DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
            break;
        }

        pt_strm_req->b_aux_audio = TRUE;

        t_msg.h_svctx       = h_svctx;
        t_msg.ui1_msg_id    = _SVCTX_MSG_SELECT_STRM_REQ;
        t_msg.u.pt_strm_req = pt_strm_req;

        i4_ret = activator_send_msg(
                pt_g_activator,
                SVCTX_MSGQ_DEFAULT_PRIORITY,
                &t_msg
                );
        i4_ret = (i4_ret != ACTVTRR_OK) ? SVCTXR_FAIL: SVCTXR_OK;

    } while (0);

    if (i4_ret != SVCTXR_OK) {
        svctx_free_strm_req(pt_strm_req);
    }
    svctx_class_unlock();
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_svctx_stop_stream
 *
 * Description: This APIis used to stop a specific stream via stream type
 *              in asynchronous way.
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          e_strm_type        Contains the stream type for stopping
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_FAIL        Send request failed
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_stop_stream(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_strm_type
                    )
{
    SVCTX_MSG_T t_msg  = {0};
    INT32       i4_ret = SVCTXR_OK;

    CLI_FUNC_CALL_TRACE_FOR_STRM(__func__, e_strm_type);

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    t_msg.ui1_msg_id                      = _SVCTX_MSG_STOP_STRM_REQ;
    t_msg.u.t_stop_strm_req.e_type        = e_strm_type;
    t_msg.u.t_stop_strm_req.t_sync.pf_nfy = NULL;
    t_msg.u.t_stop_strm_req.t_sync.pv_tag = NULL;
    t_msg.u.t_stop_strm_req.b_aux_audio   = FALSE;

    i4_ret = svctx_send_msg(h_svctx, &t_msg);

    if (SVCTXR_OK != i4_ret)
    {
        return (i4_ret);
    }

    if (ST_AUDIO == e_strm_type)
    {
        t_msg.ui1_msg_id                      = _SVCTX_MSG_STOP_STRM_REQ;
        t_msg.u.t_stop_strm_req.e_type        = e_strm_type;
        t_msg.u.t_stop_strm_req.t_sync.pf_nfy = NULL;
        t_msg.u.t_stop_strm_req.t_sync.pv_tag = NULL;
        t_msg.u.t_stop_strm_req.b_aux_audio   = TRUE;

        i4_ret = svctx_send_msg(h_svctx, &t_msg);
    }

    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: x_svctx_sync_stop_stream
 *
 * Description: This APIis used to stop a specific stream via stream type
 *              in synchronous way.
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          e_strm_type        Contains the stream type for stopping
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_FAIL        Send request failed
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
#ifdef MW_TV_AV_BYPASS_SUPPORT
INT32 x_svctx_sync_stop_stream(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_strm_type,
                    UINT32                          ui4_time
                    )
{
    SVCTX_MSG_T             t_msg  = {0};
    INT32                   i4_ret = SVCTXR_OK;
    SVCTX_T*                pt_svctx = NULL;
    SVCTX_T*                pt_obj   = NULL;
    INT32                   i, j;
    INT32                   i4_idx = 0;
    ACTIVE_LINK_ENTRY_T*    pt_link;
    HANDLE_T                ah_svctx[MAX_SVCTX_NUMBER];

    CLI_FUNC_CALL_TRACE_FOR_STRM(__func__, e_strm_type);

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (ui4_time == 0) {
        ui4_time = DEFAULT_STOP_STRM_TIMEOUT;
    }

    i4_ret = svctx_class_lock();

    if (SVCTXR_OK == i4_ret)
    {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);

        if (SVCTXR_OK == i4_ret)
        {

            if (!_IN_RUNNING_STATE(pt_svctx->e_state)) {
                svctx_class_unlock();
                return SVCTXR_OK;
            }
            
            /* 138652 start */
            pt_svctx->t_crnt_nfy_msg.e_svc_code = SVCTX_NTFY_CODE_NONE; /* for 138652 */
            if (e_strm_type == ST_VIDEO) /* for 139354 */
            {
                switch(pt_svctx->e_av_status)
            {
                    case SVC_AV_STATUS_SCRAMBLED_AUDIO_VIDEO:
                    case SVC_AV_STATUS_SCRAMBLED_AUDIO_CLEAR_VIDEO:
                        svctx_set_av_status(pt_svctx, SVC_AV_STATUS_SCRAMBLED_AUDIO_NO_VIDEO);
                        break;
                    case SVC_AV_STATUS_AUDIO_VIDEO:
                    case SVC_AV_STATUS_SCRAMBLED_VIDEO_CLEAR_AUDIO:
                        svctx_set_av_status(pt_svctx, SVC_AV_STATUS_AUDIO_ONLY);
                        break;
                    case SVC_AV_STATUS_SCRAMBLED_VIDEO_NO_AUDIO:
                    case SVC_AV_STATUS_VIDEO_ONLY:
                        svctx_set_av_status(pt_svctx, SVC_AV_STATUS_NO_AUDIO_VIDEO);
                        break;
                    default:
                        break;
                }
            }
            else if (e_strm_type == ST_AUDIO)
            {
                switch(pt_svctx->e_av_status)
                {
                    case SVC_AV_STATUS_SCRAMBLED_AUDIO_VIDEO:
                    case SVC_AV_STATUS_SCRAMBLED_VIDEO_CLEAR_AUDIO:
                        svctx_set_av_status(pt_svctx, SVC_AV_STATUS_SCRAMBLED_VIDEO_NO_AUDIO);
                        break;
                    case SVC_AV_STATUS_AUDIO_VIDEO:
                    case SVC_AV_STATUS_SCRAMBLED_AUDIO_CLEAR_VIDEO:
                        svctx_set_av_status(pt_svctx, SVC_AV_STATUS_VIDEO_ONLY);
                        break;
                    case SVC_AV_STATUS_SCRAMBLED_AUDIO_NO_VIDEO:
                    case SVC_AV_STATUS_AUDIO_ONLY:
                        svctx_set_av_status(pt_svctx, SVC_AV_STATUS_NO_AUDIO_VIDEO);
                        break;
                    default:
                        break;
                }
            }
            /* 138652 end */

            i4_idx = svctx_mgr_get_link_num(
                ACTIVE_LINK_NOW,
                pt_svctx->pt_running_svc_req->e_src_type);

            if (i4_idx > 0)
            {
                i = 0;

                while (i < i4_idx)
                {
                    ah_svctx[i] = NULL_HANDLE;

                    pt_link = svctx_mgr_get_link_obj(
                        ACTIVE_LINK_NOW,
                        pt_svctx->pt_running_svc_req->e_src_type,
                        i);

                    if (pt_link)
                    {
                        pt_obj = pt_link->pt_worker;

                        if (pt_obj)
                        {
                            ah_svctx[i] = ((pt_obj == pt_svctx) ? h_svctx : pt_obj->h_sys_svctx);
                        }
                    }
                    i++;
                }
            }
        }
    }

    svctx_class_unlock();

    if ((SVCTXR_OK != i4_ret) || (0 == i4_idx))
    {
        return (i4_ret);
    }

    i = i4_idx - 1;

    x_sema_lock(h_g_module_sync_stop_sema, X_SEMA_OPTION_WAIT);

    while (i >= 0)
    {
        if (NULL_HANDLE != ah_svctx[i])
        {
            j = ((h_svctx == ah_svctx[i]) && (ST_AUDIO == e_strm_type) ? 1 : 0);

            while (j >= 0)
            {
                t_msg.ui1_msg_id                      = _SVCTX_MSG_STOP_STRM_REQ;
                t_msg.u.t_stop_strm_req.e_type        = e_strm_type;
                t_msg.u.t_stop_strm_req.t_sync.pf_nfy = _svctx_stop_sync_cb;
                t_msg.u.t_stop_strm_req.t_sync.pv_tag = (VOID*) h_g_module_wait_sema;
                t_msg.u.t_stop_strm_req.b_aux_audio   = ((j == 1) ? TRUE : FALSE);

                i4_ret = svctx_send_msg(ah_svctx[i], &t_msg);

                if (i4_ret != SVCTXR_OK)
                {
                    break;
                }

                i4_ret = _WAIT_TIMEOUT(h_g_module_wait_sema, ui4_time);

                if (i4_ret == OSR_TIMEOUT)
                {
                    i4_ret = SVCTXR_TIMEOUT;
                    break;
                }
                else
                {
                    i4_ret = SVCTXR_OK;
                }

                j--;
            }
        }

        i--;
    }

    x_sema_unlock(h_g_module_sync_stop_sema);

    return (i4_ret);
}
#else
INT32 x_svctx_sync_stop_stream(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_strm_type,
                    UINT32                          ui4_time
                    )
{
    SVCTX_MSG_T t_msg  = {0};
    INT32       i4_ret = SVCTXR_OK;

    CLI_FUNC_CALL_TRACE_FOR_STRM(__func__, e_strm_type);

    if (b_g_init == FALSE || h_svctx == NULL_HANDLE) {
        return SVCTXR_NOT_INIT;
    }

    if (ui4_time == 0) {
        ui4_time = DEFAULT_STOP_STRM_TIMEOUT;
    }

    x_sema_lock(h_g_module_sync_stop_sema, X_SEMA_OPTION_WAIT);
    do {
        t_msg.h_svctx                         = h_svctx;
        t_msg.ui1_msg_id                      = _SVCTX_MSG_STOP_STRM_REQ;
        t_msg.u.t_stop_strm_req.e_type        = e_strm_type;
        t_msg.u.t_stop_strm_req.t_sync.pf_nfy = _svctx_stop_sync_cb;
        t_msg.u.t_stop_strm_req.t_sync.pv_tag = (VOID*)h_g_module_wait_sema;
        t_msg.u.t_stop_strm_req.b_aux_audio   = FALSE;

        i4_ret = activator_send_msg(
                        pt_g_activator,
                        SVCTX_MSGQ_DEFAULT_PRIORITY,
                        &t_msg
                        );
        if (i4_ret != ACTVTRR_OK) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = _WAIT_TIMEOUT(h_g_module_wait_sema, ui4_time);
        if (i4_ret == OSR_TIMEOUT) {
            i4_ret = SVCTXR_TIMEOUT;
            break;
        }

        if (i4_ret != OSR_OK) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (ST_AUDIO == e_strm_type)
        {
            t_msg.h_svctx                         = h_svctx;
            t_msg.ui1_msg_id                      = _SVCTX_MSG_STOP_STRM_REQ;
            t_msg.u.t_stop_strm_req.e_type        = e_strm_type;
            t_msg.u.t_stop_strm_req.t_sync.pf_nfy = _svctx_stop_sync_cb;
            t_msg.u.t_stop_strm_req.t_sync.pv_tag = (VOID*)h_g_module_wait_sema;
            t_msg.u.t_stop_strm_req.b_aux_audio   = TRUE;

            i4_ret = activator_send_msg(
                            pt_g_activator,
                            SVCTX_MSGQ_DEFAULT_PRIORITY,
                            &t_msg
                            );
            if (i4_ret != ACTVTRR_OK) {
                i4_ret = SVCTXR_FAIL;
                break;
            }

            i4_ret = _WAIT_TIMEOUT(h_g_module_wait_sema, ui4_time);
            if (i4_ret == OSR_TIMEOUT) {
                i4_ret = SVCTXR_TIMEOUT;
                break;
            }

            if (i4_ret != OSR_OK) {
                i4_ret = SVCTXR_FAIL;
                break;
            }
        }
        i4_ret = SVCTXR_OK;
    } while (0);
     x_sema_unlock(h_g_module_sync_stop_sema);

    return i4_ret;
}
#endif


INT32 x_svctx_set_ad_state(
                    HANDLE_T                        h_svctx,          /* in  */
                    BOOL                            b_enable          /* in  */
                    )
{
    SVCTX_MSG_T t_msg  = {0};
    SVCTX_T*    pt_svctx;
    STRM_REQ_T* pt_strm_req;
    INT32       i4_ret;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "b_enable = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    b_enable
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    i4_ret = SVCTXR_OK;

    svctx_class_lock();

    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);

        if (SVCTXR_OK != i4_ret) {
            break;
        }

        if (pt_svctx->b_ad_enabled != b_enable)
        {
            pt_svctx->b_ad_enabled = b_enable;

            if ((pt_svctx->pt_running_svc_req) &&
                (SRC_TYPE_BRDCST_SVL == pt_svctx->pt_running_svc_req->e_src_type))
            {
                if (b_enable)
                {
                    CH_STRM_SEL_INFO_T  t_info;
                    SCDB_REC_T          t_scdb_rec;

                    if ((pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_AUDIO) > 0) {
                        i4_ret =  SVCTXR_OK;
                        break;
                    }

                    if (pt_svctx->pt_running_svc_req == NULL) {
                        i4_ret = SVCTXR_OK;
                        break;
                    }

                    if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
                        i4_ret = SVCTXR_OK;
                        break;
                    }

                    if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req == NULL) {
                        i4_ret = SVCTXR_NOT_SUPPORT;
                        break;
                    }

                    if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_using_scdb_info == NULL) {
                        i4_ret = SVCTXR_NOT_SUPPORT;
                        break;
                    }

                    i4_ret = (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_using_scdb_info)(
                        pt_svctx,
                        ST_AUDIO,
                        NULL,
                        &t_scdb_rec
                        );

                    if (SVCTXR_OK != i4_ret)
                    {
                        DBG_ERROR((_ERROR_HEADER"get using SCDB record failed!\r\n"));
                        break;
                    }

                    if (AUD_TYPE_VISUAL_IMPAIRED == t_scdb_rec.u.t_audio_mpeg.e_type)
                    {
                        DBG_INFO((_INFO_HEADER"AD is already played as the main audio!\r\n"));
                        break;
                    }

                    t_info.e_type = CH_STRM_SEL_INFO_TYPE_MIXED_AUDIO;
                    t_info.u.t_audio.pt_lang = (ISO_639_LANG_T*) (t_scdb_rec.u.t_audio_mpeg.s_lang);

                    i4_ret = (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req)(
                                        pt_svctx,
                                        h_svctx,
                                        &t_info,
                                        NULL,
                                        NULL,
                                        &pt_strm_req
                                        );

                    if (i4_ret != SVCTXR_OK) {
                        DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
                        break;
                    }

                    /* send message as playing the normal audio stream */
                    t_msg.ui1_msg_id    = _SVCTX_MSG_SELECT_STRM_REQ;
                    t_msg.u.pt_strm_req = pt_strm_req;

                    i4_ret = svctx_send_msg(h_svctx, &t_msg);
                }
                else
                {
                    t_msg.ui1_msg_id                      = _SVCTX_MSG_STOP_STRM_REQ;
                    t_msg.u.t_stop_strm_req.e_type        = ST_AUDIO;
                    t_msg.u.t_stop_strm_req.t_sync.pf_nfy = NULL;
                    t_msg.u.t_stop_strm_req.t_sync.pv_tag = NULL;
                    t_msg.u.t_stop_strm_req.b_aux_audio   = TRUE;

                    i4_ret = svctx_send_msg(h_svctx, &t_msg);
                }

                if (SVCTXR_OK != i4_ret)
                {
                    pt_svctx->b_ad_enabled = (b_enable ? FALSE : TRUE);
                    break;
                }
            }
        }
    } while (0);

    svctx_class_unlock();

    return (i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_set_ad_state_ex 
 *
 * Description: This API tries to open an ad stream specified by *pt_scdb_rec.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          pt_scdb_rec        References to the structure containing the
 *                               scdb data of the ad stream.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_ARG     pt_scdb_rec is NULL or the ui2_num_recs is zero.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NOT_EXIST   The stream component is not existed.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_FAIL        SCDB error, can not get the record
 *          SVCTXR_NOT_SUPPORT The operation is not supported in current
 *                               scenaro.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_set_ad_state_ex(
                    HANDLE_T                        h_svctx,          /* in  */
                    const SCDB_REC_T*               pt_scdb_rec       /* in  */
                    )
{
    SVCTX_MSG_T         t_msg  = {0};
    SVCTX_T*            pt_svctx;
    STRM_REQ_T*         pt_strm_req;
    INT32               i4_ret;
    CH_STRM_SEL_INFO_T  t_info;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pt_scdb_rec = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pt_scdb_rec
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (!pt_scdb_rec) {
        return SVCTXR_INV_ARG;
    }

    i4_ret = SVCTXR_OK;

    svctx_class_lock();

    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (SVCTXR_OK != i4_ret) {
            break;
        }

        if (!pt_svctx->b_ad_enabled
            || pt_svctx->pt_running_svc_req == NULL
            || pt_svctx->pt_running_svc_req->pt_hdlr == NULL)
        {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (SRC_TYPE_BRDCST_SVL != pt_svctx->pt_running_svc_req->e_src_type)
        {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        t_info.e_type = CH_STRM_SEL_INFO_TYPE_MIXED_AUDIO;
        t_info.u.t_audio.pt_lang = (ISO_639_LANG_T*) (pt_scdb_rec->u.t_audio_mpeg.s_lang);

        i4_ret = (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_create_strm_req)(
                            pt_svctx,
                            h_svctx,
                            &t_info,
                            NULL,
                            NULL,
                            &pt_strm_req
                            );
        if (i4_ret != SVCTXR_OK) {
            DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
            break;
        }

        /* send message as playing the normal audio stream */
        t_msg.ui1_msg_id    = _SVCTX_MSG_SELECT_STRM_REQ;
        t_msg.u.pt_strm_req = pt_strm_req;

        i4_ret = svctx_send_msg(h_svctx, &t_msg);
     
    } while (0);

    svctx_class_unlock();

    return (i4_ret);
}


INT32 x_svctx_get_ad_state(
                    HANDLE_T                        h_svctx,          /* in  */
                    BOOL*                           pb_enable         /* out */
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pb_enable = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pb_enable
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pb_enable == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();

    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);

        if (SVCTXR_OK != i4_ret) {
            break;
        }

        *pb_enable = pt_svctx->b_ad_enabled;

        i4_ret = SVCTXR_OK;
    } while (0);

    svctx_class_unlock();

    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: x_svctx_set_volume
 *
 * Description: This API is used to set the audio volume.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          pt_volume_info     References to the structure containing the
 *                             scc audio volume information data.
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_ARG     pt_volume_info is NULL
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_set_volume(
                    HANDLE_T                        h_svctx,            /* in  */
                    const SCC_AUD_VOLUME_INFO_T*    pt_volume_info      /* in  */
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pt_volume_info = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pt_volume_info
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pt_volume_info == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (x_handle_valid(pt_svctx->h_audio_scc_comp) == FALSE) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = x_scc_aud_set_volume(
                    pt_svctx->h_audio_scc_comp,
                    pt_volume_info
                    );
        if (i4_ret == SMR_INV_COMMAND) {
            i4_ret = SVCTXR_INV_ARG;
            break;
        }

        if (TRUE == x_handle_valid(pt_svctx->h_ad_scc_comp))
        {
            i4_ret = x_scc_aud_set_volume(
                pt_svctx->h_ad_scc_comp,
                pt_volume_info);

            if (SMR_INV_COMMAND == i4_ret)
            {
                i4_ret = SVCTXR_INV_ARG;
                break;
            }
        }

        if (i4_ret != SMR_OK) {
            i4_ret = SVCTXR_FAIL;
            break;
        }
        i4_ret = SVCTXR_OK;
    } while (0);
    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_get_volume
 *
 * Description: This API is used to get the audio volume.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 * Outputs: pt_volume_info     References to the structure containing the
 *                             scc audio volume information data.
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_ARG     pt_volume_info is NULL
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_get_volume(
                    HANDLE_T                        h_svctx,            /* in  */
                    SCC_AUD_VOLUME_INFO_T*          pt_volume_info      /* out */
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pt_volume_info = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pt_volume_info
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pt_volume_info == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (x_handle_valid(pt_svctx->h_audio_scc_comp) == FALSE) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = x_scc_aud_get_volume(
                    pt_svctx->h_audio_scc_comp,
                    pt_volume_info
                    );
        if (i4_ret == SMR_INV_COMMAND) {
            i4_ret = SVCTXR_INV_ARG;
            break;
        }

        if (i4_ret != SMR_OK) {
            i4_ret = SVCTXR_FAIL;
            break;
        }
        i4_ret = SVCTXR_OK;
    } while (0);
    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_set_mute
 *
 * Description: This API is used to set the audio to mute or unmute.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          b_is_mute          Contains a boolean value for setting audio
 *                             to mute or unmute.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_set_mute(
                    HANDLE_T                        h_svctx,            /* in  */
                    BOOL                            b_is_mute           /* in  */
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "b_is_mute = %d"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    b_is_mute
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (x_handle_valid(pt_svctx->h_audio_scc_comp) == FALSE) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (b_is_mute == TRUE) {
            i4_ret = x_scc_aud_mute(pt_svctx->h_audio_scc_comp);
        } else {
            i4_ret = x_scc_aud_unmute(pt_svctx->h_audio_scc_comp);
        }

        if (TRUE == x_handle_valid(pt_svctx->h_ad_scc_comp))
        {
            if (b_is_mute == TRUE) {
                x_scc_aud_mute(pt_svctx->h_ad_scc_comp);
            } else {
                x_scc_aud_unmute(pt_svctx->h_ad_scc_comp);
            }
        }

        i4_ret = (i4_ret != SMR_OK) ? SVCTXR_FAIL: SVCTXR_OK;
    } while (0);
    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_get_mute
 *
 * Description: This API is used to check the audio is mute or unmute.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *
 * Outputs: pb_is_mute         References to a boolean value for retrieving audio
 *                             is mute or unmute.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_ARG     pb_is_mute is NULL
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_get_mute(
                    HANDLE_T                        h_svctx,            /* in  */
                    BOOL*                           pb_is_mute          /* in  */
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pb_is_mute = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pb_is_mute
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pb_is_mute == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (x_handle_valid(pt_svctx->h_audio_scc_comp) == FALSE) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = x_scc_aud_is_muted(
                    pt_svctx->h_audio_scc_comp,
                    pb_is_mute
                    );
        i4_ret = (i4_ret != SMR_OK) ? SVCTXR_FAIL: SVCTXR_OK;
    } while (0);
    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_get_svc_rec_by_channel_id
 *
 * Description: This API is used to get service list record by channel id and
 *              surfing mode, i.e. next channel, previous channel, or this channel
 * Inputs:      h_svl               Contains a handle to a service list.
 *              ui4_nw_mask         Contains the network mask for filtering service
 *                                  record.
 *              e_surf_mode         Contains the surfing mode.
 *              pui4_channel_id     References to the variable containing the
 *                                  channel id.
 * Outputs:     pui4_channel_id     References to the variable containing the
 *                                  resulted channel id.
 *              pt_svl_rec          References to the structure containing the
 *                                  service list record data.
 * Returns: SVCTXR_OK           Routine successful.
 *          SVCTXR_INV_HANDLE   h_svl is invalid
 *          SVCTXR_INV_ARG      pui4_channel_id or pt_svl_rec is NULL.
 *          SVCTXR_NO_RECORD    There's no any record in this service list
 *          SVCTXR_FAIL         Accessing service list failed.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_get_svc_rec_by_channel_id(
                    HANDLE_T                        h_svl,              /* in */
                    UINT32                          ui4_nw_mask,        /* in */
                    SURF_MODE_T                     e_surf_mode,        /* in */
                    UINT32*                         pui4_channel_id,    /* i/o*/
                    SVL_REC_T*                      pt_svl_rec          /* out*/
                    )
{
    INT32  i4_ret;
    UINT32 ui4_channel_id;
    UINT32 ui4_svl_ver_id = SVL_NULL_VER_ID;

    DBG_API((_API_HEADER"%s("
                    "h_svl = %d, "
                    "ui4_nw_mask = 0x%x, "
                    "e_surf_mode = %d, "
                    "pui4_channel_id = 0x%x, "
                    "pt_svl_rec = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svl,
                    ui4_nw_mask,
                    e_surf_mode,
                    pui4_channel_id,
                    pt_svl_rec
                    ));

    if (pui4_channel_id == NULL || pt_svl_rec == NULL) {
        return SVCTXR_INV_ARG;
    }

    ui4_channel_id = *pui4_channel_id;

    if (ui4_channel_id == INVALID_CHANNEL_ID) {
        i4_ret = x_svl_iterate_rec (
                            h_svl,
                            ui4_channel_id,
                            ui4_nw_mask,
                            SVL_CHANNEL_SMALLEST,
                            pt_svl_rec,
                            &ui4_svl_ver_id
                            );
    } else {
        switch(e_surf_mode) {
        case SURF_MODE_PREV:
            i4_ret = x_svl_iterate_rec (
                            h_svl,
                            ui4_channel_id,
                            ui4_nw_mask,
                            SVL_CHANNEL_MINUS,
                            pt_svl_rec,
                            &ui4_svl_ver_id
                            );
            if (i4_ret == SVLR_END_OF_ITERATION) {
                /* current is the first one, get the last one */
                i4_ret = x_svl_iterate_rec (
                            h_svl,
                            ui4_channel_id,
                            ui4_nw_mask,
                            SVL_CHANNEL_LARGEST,
                            pt_svl_rec,
                            &ui4_svl_ver_id
                            );
            } else if (i4_ret == SVLR_REC_NOT_FOUND) {
                i4_ret = x_svl_iterate_rec (
                            h_svl,
                            ui4_channel_id,
                            ui4_nw_mask,
                            SVL_CHANNEL_SMALLEST,
                            pt_svl_rec,
                            &ui4_svl_ver_id
                            );
            }
            break;
        case SURF_MODE_NEXT:
            i4_ret = x_svl_iterate_rec (
                            h_svl,
                            ui4_channel_id,
                            ui4_nw_mask,
                            SVL_CHANNEL_PLUS,
                            pt_svl_rec,
                            &ui4_svl_ver_id
                            );
            if (i4_ret == SVLR_END_OF_ITERATION) {
                /* current is the last one, get the first one */
                i4_ret = x_svl_iterate_rec (
                            h_svl,
                            ui4_channel_id,
                            ui4_nw_mask,
                            SVL_CHANNEL_SMALLEST,
                            pt_svl_rec,
                            &ui4_svl_ver_id
                            );
            } else if (i4_ret == SVLR_REC_NOT_FOUND) {
                i4_ret = x_svl_iterate_rec (
                            h_svl,
                            ui4_channel_id,
                            ui4_nw_mask,
                            SVL_CHANNEL_SMALLEST,
                            pt_svl_rec,
                            &ui4_svl_ver_id
                            );
            }
            break;
        default:
            i4_ret = x_svl_get_rec_by_channel(
                            h_svl,
                            ui4_channel_id,
                            ui4_nw_mask,
                            0,
                            pt_svl_rec,
                            &ui4_svl_ver_id
                            );
            break;
        }
    }

    if (i4_ret == SVLR_END_OF_ITERATION) {
        return SVCTXR_NO_RECORD;
    } else if (i4_ret == SVLR_REC_NOT_FOUND) {
        return SVCTXR_NO_RECORD;
    } else if (i4_ret == SVLR_INV_HANDLE) {
        return SVCTXR_INV_HANDLE;
    } else if (i4_ret != SVLR_OK && i4_ret != SVLR_NEAREST_CHAN_MATCH) {
        return SVCTXR_FAIL;
    }

    *pui4_channel_id = pt_svl_rec->uheader.t_rec_hdr.ui4_channel_id;
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_get_display_resolution
 *
 * Description:
 *          This API is used to get display's resolution.
 *          The details about pt_resolution should refer to
 *          stream manager's SCC_DISP_RESOLUTION_T.
 * Inputs:  h_svctx         Contains a handle to a service context.
 *
 * Outputs: pt_resolution   References to the variable containing the
 *                          SCC_DISP_RESOLUTION_T.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pt_resolution is NULL
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_get_display_resolution(
                    HANDLE_T                    h_svctx,
                    SCC_DISP_RESOLUTION_T*      pt_resolution
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pt_resolution = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pt_resolution
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pt_resolution == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (x_handle_valid(pt_svctx->h_display_scc_comp) == FALSE) {
            i4_ret = SVCTXR_FAIL;
            break;
        }
        i4_ret = x_scc_disp_get_resolution(
                    pt_svctx->h_display_scc_comp,
                    pt_resolution
                    );
        i4_ret = (i4_ret == SMR_OK ? SVCTXR_OK : SVCTXR_FAIL);
    } while (0);
    svctx_class_unlock();
    return (i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_set_display_resolution
 *
 * Description:
 *          This API is used to set display's resolution.
 *          The details about pt_resolution should refer to
 *          stream manager's SCC_DISP_RESOLUTION_T.
 * Inputs:  h_svctx         Contains a handle to a service context.
 *          pt_resolution   References to the variable containing the
 *                          SCC_DISP_RESOLUTION_T.
 * Ouputs: -
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pt_resolution is NULL
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_set_display_resolution(
                    HANDLE_T                    h_svctx,
                    SCC_DISP_RESOLUTION_T*      pt_resolution
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pt_resolution = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pt_resolution
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pt_resolution == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (x_handle_valid(pt_svctx->h_display_scc_comp) == FALSE) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = x_scc_disp_set_resolution(
                    pt_svctx->h_display_scc_comp,
                    pt_resolution
                    );
        i4_ret = (i4_ret == SMR_OK ? SVCTXR_OK : SVCTXR_FAIL);
    } while (0);
    svctx_class_unlock();
    return (i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_get_display_aspect_ratio
 *
 * Description:
 *          This API is used to get display's aspect ratio.
 *          The details about pe_aspect_ratio should refer to
 *          stream manager's SCC_DISP_ASPECT_RATIO_T.
 * Inputs:  h_svctx         Contains a handle to a service context.
 *
 * Outputs: pe_aspect_ratio   References to the variable containing the
 *                            SCC_DISP_ASPECT_RATIO_T.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pe_aspect_ratio is NULL
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_get_display_aspect_ratio(
                    HANDLE_T                    h_svctx,
                    SCC_DISP_ASPECT_RATIO_T*    pe_aspect_ratio
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pe_aspect_ratio = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pe_aspect_ratio
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pe_aspect_ratio == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (x_handle_valid(pt_svctx->h_display_scc_comp) == FALSE) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = x_scc_disp_get_aspect_ratio(
                    pt_svctx->h_display_scc_comp,
                    pe_aspect_ratio
                    );
        i4_ret = (i4_ret == SMR_OK ? SVCTXR_OK : SVCTXR_FAIL);
    } while (0);
    svctx_class_unlock();
    return (i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_set_display_aspect_ratio
 *
 * Description:
 *          This API is used to set display's aspect ratio.
 *          The details about pe_aspect_ratio should refer to
 *          stream manager's SCC_DISP_ASPECT_RATIO_T.
 * Inputs:  h_svctx         Contains a handle to a service context.
 *          e_aspect_ratio  Contains the value of SCC_DISP_ASPECT_RATIO_T value
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_set_display_aspect_ratio(
                    HANDLE_T                    h_svctx,
                    SCC_DISP_ASPECT_RATIO_T     e_aspect_ratio
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "e_aspect_ratio = %d"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_aspect_ratio
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (x_handle_valid(pt_svctx->h_display_scc_comp) == FALSE) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = x_scc_disp_set_aspect_ratio(
                    pt_svctx->h_display_scc_comp,
                    e_aspect_ratio
                    );
        i4_ret = (i4_ret == SMR_OK ? SVCTXR_OK : SVCTXR_FAIL);
    } while (0);
    svctx_class_unlock();
    return (i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_get_video_src_resolution
 *
 * Description:
 *          This API is used to get video source's resolution.
 * Inputs:  h_svctx         Contains a handle to a service context.
 *
 * Outputs: pt_resolution   References to the variable containing the
 *                          VSH_SRC_RESOLUTION_INFO_T.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pt_resolution is NULL
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_get_video_src_resolution(
                    HANDLE_T                    h_svctx,
                    VSH_SRC_RESOLUTION_INFO_T*  pt_resolution
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pt_resolution = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pt_resolution
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pt_resolution == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->b_is_blocked == TRUE || pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED) {
            _RESET_VIDEO_RES(pt_resolution);
        } else {
            *pt_resolution = pt_svctx->t_video_info;
        }
    } while (0);
    svctx_class_unlock();

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_get_audio_info
 *
 * Description: This API is used to get audio information. If the audio
 *              information is not available, the return value is equal
 *              to SVCTXR_OK and pt_info->e_channels is equal to
 *              ASH_CHANNELS_UNKNOWN.
 * Inputs:  h_svctx         Contains a handle to a service context.
 *
 * Outputs: pt_info         References to the variable containing the
 *                          ASH_AUDIO_INFO_T.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pt_info is NULL
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_get_audio_info(
                    HANDLE_T                    h_svctx,
                    ASH_AUDIO_INFO_T*           pt_info
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pt_info = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pt_info
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pt_info == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->b_is_blocked == TRUE || pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED) {
            _RESET_AUDIO_FMT(pt_info);
        } else {
            *pt_info = pt_svctx->t_audio_info;
        }
    } while (0);
    svctx_class_unlock();

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_svctx_set_screen_mode
 *
 * Description: This API is used to set screen mode.
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          e_scr_mode         Contains the screen mode
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_set_screen_mode(
                    HANDLE_T                    h_svctx,
                    SCC_VID_SCREEN_MODE_T       e_scr_mode
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d,"
                    "e_scr_mode = %d"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_scr_mode
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        pt_svctx->e_scr_mode = e_scr_mode;

        if (pt_svctx->t_mheg_scr_mode.b_enable == TRUE)
        {
            break;
        }

        i4_ret = x_scc_vid_set_scr_mode(
                    pt_svctx->h_video_scc_comp,
                    e_scr_mode
                    );
    } while (0);
    svctx_class_unlock();
    return (i4_ret == SMR_OK ? SVCTXR_OK : SVCTXR_FAIL);
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_get_screen_mode
 *
 * Description: This API is used to set screen mode.
 * Inputs:  h_svctx            Contains a handle to a service context.
 *
 * Outputs: pe_scr_mode        References to the variable containing the
 *                             SCC_VID_SCREEN_MODE_T.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pe_scr_mode is NULL
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_get_screen_mode(
                    HANDLE_T                    h_svctx,
                    SCC_VID_SCREEN_MODE_T*      pe_scr_mode
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pe_scr_mode = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pe_scr_mode
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pe_scr_mode == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        i4_ret = x_scc_vid_get_scr_mode(
                    pt_svctx->h_video_scc_comp,
                    pe_scr_mode
                    );
    } while (0);
    svctx_class_unlock();
    return (i4_ret == SMR_OK ? SVCTXR_OK : SVCTXR_FAIL);
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_set_session_attr
 *
 * Description: This function is used to set the attribuites of a session
 *
 * Inputs:  h_svctx           Contains a handle to a service context.
 *          e_pipe_type       Contains the pipe type
 *          e_set_type        Contains the type of attributes to set.
 *          pv_set_info       References to the data information of the set
 *                            type.
 *          ui4_set_info_size Contains the size of data information of the
 *                            set type.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_SUPPORT The e_set_type is not supported.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_set_session_attr(
                    HANDLE_T                    h_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    SM_SET_TYPE_T               e_set_type,
                    VOID*                       pv_set_info,
                    SIZE_T                      z_set_info_size
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_set_session_attr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_set_session_attr(
                    pt_svctx,
                    e_pipe_type,
                    e_set_type,
                    pv_set_info,
                    z_set_info_size
                    );

    } while (0);
    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_set_stream_attr
 *
 * Description: This function is used to set the attribuites of a stream
 *
 * Inputs:  h_svctx           Contains a handle to a service context.
 *          e_strm_type       Contains the stream type
 *          e_set_type        Contains the type of attributes to set.
 *          pv_set_info       References to the data information of the set
 *                            type.
 *          ui4_set_info_size Contains the size of data information of the
 *                            set type.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_set_stream_attr(
                    HANDLE_T                    h_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    SM_SET_TYPE_T               e_set_type,
                    VOID*                       pv_set_info,
                    SIZE_T                      z_set_info_size
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = svctx_set_stream_attr(
                            pt_svctx,
                            NULL_HANDLE,
                            e_set_type,
                            pv_set_info,
                            z_set_info_size
                            );
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_set_stream_attr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_set_stream_attr(
                    pt_svctx,
                    e_strm_type,
                    e_set_type,
                    pv_set_info,
                    z_set_info_size
                    );

    } while (0);
    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_get_session_attr
 *
 * Description: This function is used to get the attribuites of a session
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          e_pipe_type        Contains the pipe type to get.
 *          e_get_type         Contains the type of attributes to get.
 *          pv_get_info        References to the data information of the get
 *                             type.
 *          pui4_get_info_size References to the size of data information of
 *                             the get type.
 *
 * Outputs: pv_get_info
 *          pui4_get_info_size
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_get_session_attr(
                    HANDLE_T                    h_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    SM_GET_TYPE_T               e_get_type,
                    VOID*                       pv_get_info,            /* IN/OUT */
                    SIZE_T*                     pz_get_info_size        /* IN/OUT */
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_session_attr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_session_attr(
                    pt_svctx,
                    e_pipe_type,
                    e_get_type,
                    pv_get_info,
                    pz_get_info_size
                    );

    } while (0);
    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_get_stream_attr
 *
 * Description: This function is used to get the attribuites of a stream
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          e_strm_type        Contains the stream type to get.
 *          e_get_type         Contains the type of attributes to get.
 *          pv_get_info        References to the data information of the get
 *                             type.
 *          pui4_get_info_size References to the size of data information of
 *                             the get type.
 *
 * Outputs: pv_get_info
 *          pui4_get_info_size
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_get_stream_attr(
                    HANDLE_T                    h_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    SM_GET_TYPE_T               e_get_type,
                    VOID*                       pv_get_info,            /* IN/OUT */
                    SIZE_T*                     pz_get_info_size        /* IN/OUT */
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_stream_attr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_stream_attr(
                    pt_svctx,
                    e_strm_type,
                    e_get_type,
                    pv_get_info,
                    pz_get_info_size
                    );

    } while (0);
    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_freeze
 *
 * Description: This API is used to freeze svctx. The minimum frozen stream
 *              is video stream. If ui4_freeze_strm_mask doesn't include
 *              ST_MASK_VIDEO, this API will add video stream to freeze
 *              automatically.
 *
 * Inputs:  h_svctx                Contains a handle to a service context.
 *          ui4_freeze_strm_mask   Contains the mask to indicate which streams
 *                                 will be frozen.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_freeze(
                    HANDLE_T                        h_svctx,
                    UINT32                          ui4_freeze_strm_mask
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    CLI_FUNC_CALL_TRACE(__func__);

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    ui4_freeze_strm_mask |= ST_MASK_VIDEO;

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        pt_svctx->ui4_frz_frozen_strm_mask = ui4_freeze_strm_mask;

        svctx_set_video_plane(
                    pt_svctx,
                    SCC_VID_FREEZE);

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_freeze == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_freeze(
                    pt_svctx
                    );
    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_unfreeze
 *
 * Description: This API is used to unfreeze svctx.
 * Inputs:  h_svctx                Contains a handle to a service context
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_unfreeze(
                    HANDLE_T                        h_svctx
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    CLI_FUNC_CALL_TRACE(__func__);

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->ui4_frz_frozen_strm_mask == 0) {
            break;
        }

        pt_svctx->ui4_frz_frozen_strm_mask = 0;

        if (svctx_is_no_video(pt_svctx) == TRUE) {
            svctx_set_video_plane(pt_svctx, SCC_VID_BLANK);
            i4_ret = SVCTXR_OK;
            break;
        }

        svctx_set_video_plane(pt_svctx, pt_svctx->e_default_vid_mode);

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_unfreeze == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_unfreeze(
                    pt_svctx
                    );
    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_set_video_plane
 *
 * Description: This API is used to set video plane to normal, blank or quat-video.
 *              The details about e_mode should refer to stream manager's
 *              SCC_VID_MODE_T.
 * Inputs:  h_svctx     Contains a handle to a service context.
 *          e_mode      Contains the mode to set the video plane.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_WRONG_STATE The Service Context object is blocked.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_set_video_plane(
                    HANDLE_T                    h_svctx,
                    SCC_VID_MODE_T              e_mode
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "e_mode = %d, "
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_mode
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    CLI_FUNC_CALL_TRACE(__func__); 
    
    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->b_is_blocked == TRUE) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if(pt_svctx->e_default_vid_mode == SCC_VID_DEINT || 
            pt_svctx->e_default_vid_mode == SCC_VID_NORMAL){
              svctx_cli_print_channel_change_log(pt_svctx, CH_XNG_STAGE_AP_SET_UNMUTE);
         }
        

        if (e_mode == SCC_VID_FREEZE) {
            pt_svctx->ui4_frz_frozen_strm_mask|= ST_MASK_VIDEO;
        } else {
            pt_svctx->ui4_frz_frozen_strm_mask = 0;
        }

        i4_ret = svctx_set_video_plane(
                    pt_svctx,
                    e_mode);
    } while(0);
    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_is_signal_loss
 *
 * Description: This API is used to query it is signal loss or not
 * Inputs:  h_svctx         Contains a handle to a service context.
 *
 * Outputs: pb_signal_loss  Contains the Boolean variable for signal loss or not.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pb_signal_loss is NULL
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_is_signal_loss(
                    HANDLE_T                    h_svctx,
                    BOOL*                       pb_signal_loss
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pb_signal_loss = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pb_signal_loss
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS
                && pt_svctx->b_enable_block_bias_freq == TRUE
                && pt_svctx->b_is_blocked == TRUE) {
            *pb_signal_loss = FALSE;
        } else {
            *pb_signal_loss = pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS ? TRUE : FALSE;
        }
    } while (0);
    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_unblock_svc
 *
 * Description: This API is used to unblock a blocked service (all streams are stopped).
 *              If b_force_unblock is TRUE, the blocked service will be unblocked
 *              and streams will be played. If b_force_unblock is FALSE, the client
 *              provided x_svctx_svc_block_check_fct function (if any) will be invoked
 *              again to check the service can be unblocked or not.
 *              If the service presented in the service context is not blocked,
 *              no action is performed
 * Inputs:  h_svctx         Contains a handle to a service context.
 *          b_force_unblock Contains the boolean to force unblock or not
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK               Routine successful.
 *          SVCTXR_ALREADY_STOPPED  The service conetxt is already stopped
 *          SVCTXR_NOT_INIT         The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE       Invalid service context handle.
 *          SVCTXR_FAIL             Failed to send request
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_unblock_svc(
                    HANDLE_T                    h_svctx,
                    BOOL                        b_force_unblock
                    )
{
    SVCTX_T*                pt_svctx = NULL;
    SVCTX_MSG_T             t_msg    = {0};
    INT32                   i4_ret   = SVCTXR_OK;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    SVC_SELECT_TYPE_T       e_svc_type;
    ACTIVE_LINK_ENTRY_T*    pt_link;
    SVCTX_T*                pt_obj;
    INT32                   i = 0;
    BOOL                    b_process;
#endif

    CLI_FUNC_CALL_TRACE(__func__);

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (!_IN_RUNNING_STATE(pt_svctx->e_state)) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

#ifdef MW_TV_AV_BYPASS_SUPPORT
        if (!pt_svctx->pt_running_svc_req)
        {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        e_svc_type = svctx_mgr_convert_to_svc_type(pt_svctx->pt_running_svc_req->e_src_type);
        while (1)
        {
            pt_link = svctx_mgr_get_link_obj_all(
                ACTIVE_LINK_NOW,
                i++);

            if (!pt_link)
            {
                break;
            }

            pt_obj = pt_link->pt_worker;
            b_process = FALSE;
            switch (e_svc_type)
            {
                case SVC_SELECT_TV:
                    if (svctx_mgr_convert_to_svc_type(pt_obj->pt_running_svc_req->e_src_type) == SVC_SELECT_TV)
                    {
                        b_process = TRUE;
                    }
                    break;

                case SVC_SELECT_AV:
                    if ((svctx_mgr_convert_to_svc_type(pt_obj->pt_running_svc_req->e_src_type) == SVC_SELECT_TV) ||
                        (svctx_mgr_convert_to_svc_type(pt_obj->pt_running_svc_req->e_src_type) == SVC_SELECT_AV))
                    {
                        b_process = TRUE;
                    }
                    break;

                case SVC_SELECT_MM:
                    if (svctx_mgr_convert_to_svc_type(pt_obj->pt_running_svc_req->e_src_type) == SVC_SELECT_MM)
                    {
                        b_process = TRUE;
                    }
                    break;

                default:
                    DBG_ERROR((_ERROR_HEADER"Invalid value of e_svc_type!\r\n"));
                    break;
            }

            if (b_process)
            {
                if (pt_obj == pt_svctx)
                {
#endif
                    t_msg.h_svctx                = h_svctx;
#ifdef MW_TV_AV_BYPASS_SUPPORT
                }
                else
                {
                    t_msg.h_svctx = pt_obj->h_sys_svctx;
                }
#endif
                t_msg.ui1_msg_id                 = _SVCTX_MSG_UNBLOCK_SVC_REQ;
                t_msg.u.t_unlock_svc_req.b_force = b_force_unblock;

                i4_ret = activator_send_msg(
                            pt_g_activator,
                            SVCTX_MSGQ_DEFAULT_PRIORITY,
                            &t_msg
                            );
                i4_ret = (i4_ret != ACTVTRR_OK) ? SVCTXR_FAIL: SVCTXR_OK;
#ifdef MW_TV_AV_BYPASS_SUPPORT
            }
        }
#endif
    } while (0);
    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_set_default_parameters
 *
 * Description: This API is used to set the default parameters for service context
 * Inputs:  h_svctx             Contains a handle to a service context.
 *          ui4_stream_mode     Contains the default desired stream types
 *          e_vid_mode          Contains the mode of the video plane.
 *          b_able_set_pmx_bg   Specifiy to be able to set pmx bg
 *          pf_block_check_fct  Contains the block check fucntion pointer
 *          ps_src_grp_name     Contains the connection source name
 *          ps_snk_grp_name     Contains the sink group name
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
#ifdef SVCTX_SPLIT_SUPPORT
INT32 svctx_set_default_parameters(
                    HANDLE_T                    h_svctx,
                    UINT32                      ui4_stream_mode,
                    SCC_VID_MODE_T              e_vid_mode,
                    BOOL                        b_able_set_pmx_bg,
                    svctx_svc_block_check_fct   pf_block_check_fct,
                    VOID*                       pv_tag,
                    const CHAR*                 ps_src_grp_name,
                    const CHAR*                 ps_snk_grp_name,
                    const CHAR*                 ps_snk_grp_name_aux
                    )
{
    SVCTX_T* pt_svctx;
    SIZE_T   z_size = 0;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pf_block_check_fct = 0x%x, "
                    "ps_src_grp_name = %s, "
                    "ps_snk_grp_name = %s, "
                    ")\r\n",
                    __func__,
                    h_svctx,
                    (ps_src_grp_name     == NULL ? "NULL" : ps_src_grp_name),
                    (ps_snk_grp_name     == NULL ? "NULL" : ps_snk_grp_name),
                    (ps_snk_grp_name_aux == NULL ? "NULL" : ps_snk_grp_name_aux)
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (ui4_stream_mode == SVCTX_STRM_MODE_AS_LAST) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (ps_src_grp_name != NULL) {
            z_size = x_strlen(ps_src_grp_name);
            if (z_size > 0 && z_size <= SVCTX_SRC_NAME_MAX_SIZE) {
                x_strcpy(
                    pt_svctx->s_src_grp_name,
                    ps_src_grp_name
                    );
                pt_svctx->s_src_grp_name[SVCTX_SRC_NAME_MAX_SIZE] = '\0';
            }
        }

        if (ps_snk_grp_name != NULL) {
            svctx_open_snk_scc_comps(
                    pt_svctx,
                    ps_snk_grp_name,
                    ps_snk_grp_name_aux
                    );
        }

#ifdef LINUX_TURNKEY_SOLUTION
        if (ri_svctx_free_rpc_cb_tag_if_needed != NULL)
        {
            ri_svctx_free_rpc_cb_tag_if_needed(
                    pt_svctx->pf_block_check_fct,
                    pt_svctx->pf_block_check_fct_tag);
        }
#endif
        pt_svctx->pf_block_check_fct = pf_block_check_fct;
        pt_svctx->pf_block_check_fct_tag = pv_tag;

#ifdef MW_TV_AV_BYPASS_SUPPORT
        _svctx_set_shared_block_checker(pt_svctx, pf_block_check_fct, pv_tag);
#endif

        if (e_vid_mode == SCC_VID_DEINT || e_vid_mode == SCC_VID_NORMAL) {
            pt_svctx->e_default_vid_mode = e_vid_mode;
        }

        pt_svctx->b_able_set_pmx_bg = b_able_set_pmx_bg;
        pt_svctx->ui4_stream_mode   = ui4_stream_mode;
#ifdef MW_TV_AV_BYPASS_SUPPORT
        _svctx_set_shared_stream_mode(pt_svctx, ui4_stream_mode);
#endif
    } while (0);
    svctx_class_unlock();
    return i4_ret;
}

SVC_BLOCK_COND_T orig_cb_wrapper(
                    SVC_BLOCK_CHECK_GUDIE_T     e_check_guide,
                    const CHAR*                 ps_src_name,
                    const SVL_REC_T*            pt_svl_rec,
                    const VOID*                 pv_evn_rating,
                    VOID*                       pv_tag
                    )
{
    x_svctx_svc_block_check_fct pf_cb;


    pf_cb = pv_tag;    

    return (pf_cb (e_check_guide, ps_src_name, pt_svl_rec, pv_evn_rating));
}

INT32 x_svctx_set_default_parameters(
                    HANDLE_T                    h_svctx,
                    UINT32                      ui4_stream_mode,
                    SCC_VID_MODE_T              e_vid_mode,
                    BOOL                        b_able_set_pmx_bg,
                    x_svctx_svc_block_check_fct pf_block_check_fct,
                    const CHAR*                 ps_src_grp_name,
                    const CHAR*                 ps_snk_grp_name,
                    const CHAR*                 ps_snk_grp_name_aux
                    )
{
    INT32 i4_return;


    if (pf_block_check_fct == NULL)
    {
        i4_return = svctx_set_default_parameters (h_svctx,
                                                  ui4_stream_mode,
                                                  e_vid_mode,
                                                  b_able_set_pmx_bg,
                                                  NULL,
                                                  NULL,
                                                  ps_src_grp_name,
                                                  ps_snk_grp_name,
                                                  ps_snk_grp_name_aux);
    }
    else
    {
        i4_return = svctx_set_default_parameters (h_svctx,
                                                  ui4_stream_mode,
                                                  e_vid_mode,
                                                  b_able_set_pmx_bg,
                                                  orig_cb_wrapper,
                                                  ((VOID*)pf_block_check_fct),
                                                  ps_src_grp_name,
                                                  ps_snk_grp_name,
                                                  ps_snk_grp_name_aux);
    }

    return (i4_return);
}
#else
INT32 x_svctx_set_default_parameters(
                    HANDLE_T                    h_svctx,
                    UINT32                      ui4_stream_mode,
                    SCC_VID_MODE_T              e_vid_mode,
                    BOOL                        b_able_set_pmx_bg,
                    x_svctx_svc_block_check_fct pf_block_check_fct,
                    const CHAR*                 ps_src_grp_name,
                    const CHAR*                 ps_snk_grp_name,
                    const CHAR*                 ps_snk_grp_name_aux
                    )
{
    SVCTX_T* pt_svctx;
    SIZE_T   z_size = 0;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pf_block_check_fct = 0x%x, "
                    "ps_src_grp_name = %s, "
                    "ps_snk_grp_name = %s, "
                    ")\r\n",
                    __func__,
                    h_svctx,
                    (ps_src_grp_name     == NULL ? "NULL" : ps_src_grp_name),
                    (ps_snk_grp_name     == NULL ? "NULL" : ps_snk_grp_name),
                    (ps_snk_grp_name_aux == NULL ? "NULL" : ps_snk_grp_name_aux)
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (ui4_stream_mode == SVCTX_STRM_MODE_AS_LAST) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (ps_src_grp_name != NULL) {
            z_size = x_strlen(ps_src_grp_name);
            if (z_size > 0 && z_size <= SVCTX_SRC_NAME_MAX_SIZE) {
                x_strcpy(
                    pt_svctx->s_src_grp_name,
                    ps_src_grp_name
                    );
                pt_svctx->s_src_grp_name[SVCTX_SRC_NAME_MAX_SIZE] = '\0';
            }
        }

        if (ps_snk_grp_name != NULL) {
            svctx_open_snk_scc_comps(
                    pt_svctx,
                    ps_snk_grp_name,
                    ps_snk_grp_name_aux
                    );
        }

        pt_svctx->pf_block_check_fct = pf_block_check_fct;

#ifdef MW_TV_AV_BYPASS_SUPPORT
        _svctx_set_shared_block_checker(pt_svctx, pf_block_check_fct);
#endif

        if (e_vid_mode == SCC_VID_DEINT || e_vid_mode == SCC_VID_NORMAL) {
            pt_svctx->e_default_vid_mode = e_vid_mode;
        }

        pt_svctx->b_able_set_pmx_bg = b_able_set_pmx_bg;
        pt_svctx->ui4_stream_mode   = ui4_stream_mode;
#ifdef MW_TV_AV_BYPASS_SUPPORT
        _svctx_set_shared_stream_mode(pt_svctx, ui4_stream_mode);
#endif
    } while (0);
    svctx_class_unlock();
    return i4_ret;
}
#endif
/*-----------------------------------------------------------------------------
 * Name: x_svctx_set_default_strm_filter
 *
 * Description: This API is used to set stream's default filter function and tag.
 *              When clients invoke APIs to select proper stream to play, if
 *              client doesn't provide any filter function, this filter function
 *              will be used. If this API doesn't be invoked, the service context
 *              will use the internal filter to get the first one of the same
 *              stream type. Each stream type has one and only one default
 *              stream filter in a service context.
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          e_strm_type        Contains the stream component type for filtering
 *          pf_filter          References to the filter function provided by caller.
 *                             It can't be NULL.
 *          pv_fltr_tag        References to a caller-defined tag. It will pass to
 *                             to caller when the pf_filter is invoked.
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_INV_ARG     pt_filter_fct is NULL
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_set_default_strm_filter(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_strm_type,
                    x_svctx_strm_comp_filter_fct    pt_filter_fct,
                    VOID*                           pv_filter_tag
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "e_strm_type = %d, "
                    "pt_filter_fct = 0x%x, "
                    "pv_filter_tag = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_strm_type,
                    pt_filter_fct,
                    pv_filter_tag
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pt_filter_fct == NULL) {
        return SVCTXR_INV_ARG;
    }

    if ((e_strm_type == ST_UNKNOWN) || (e_strm_type >= STRM_TYPE_COUNT)) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

#ifdef LINUX_TURNKEY_SOLUTION
        if (ri_svctx_free_rpc_cb_tag_if_needed != NULL)
        {
            ri_svctx_free_rpc_cb_tag_if_needed(
                    pt_svctx->pt_def_strm_fltr_fcts[e_strm_type],
                    pt_svctx->pv_def_strm_fltr_tags[e_strm_type]);
        }
#endif
        pt_svctx->pt_def_strm_fltr_fcts[e_strm_type] = pt_filter_fct;
        pt_svctx->pv_def_strm_fltr_tags[e_strm_type] = pv_filter_tag;

#ifdef MW_TV_AV_BYPASS_SUPPORT
        _svctx_set_shared_strm_filter(
            pt_svctx,
            e_strm_type,
            pt_filter_fct,
            pv_filter_tag);
#endif
    } while (0);
    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_get_signal_level
 *
 * Description: This API is used to get tuner's signal level. The range
 *              of the signal level is from 0 to 100.
 * Inputs:  h_svctx            Contains a handle to a service context.
 *
 * Outputs: pui1_signal_level  References to the variable containing signal level
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pui1_signal_level is NULL
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_get_signal_level(
                    HANDLE_T                    h_svctx,
                    UINT8*                      pui1_signal_level
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pui1_signal_level = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pui1_signal_level
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_signal_level == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_signal_level(
                    pt_svctx,
                    pui1_signal_level
                    );

    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_get_dbm_signal_level
 *
 * Description: This API is used to get tuner's dbm signal level.
 * Inputs:  h_svctx            Contains a handle to a service context.
 *
 * Outputs: pi2_dbm_signal_level  References to the variable containing signal level
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pi2_dbm_signal_level is NULL
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_get_dbm_signal_level(
                    HANDLE_T                    h_svctx,
                    INT16*                      pi2_dbm_signal_level
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pi2_dbm_signal_level = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pi2_dbm_signal_level
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_dbm_signal_level == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_dbm_signal_level(
                    pt_svctx,
                    pi2_dbm_signal_level
                    );

    } while (0);
    svctx_class_unlock();
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_get_tuner_status
 *
 * Description: This API is used to get tuner's status.
 * Inputs:  h_svctx            Contains a handle to a service context.
 *
 * Outputs: pui1_tuner_status  References to the variable containing tuner's
 *                             status.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pi2_dbm_signal_level is NULL
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_get_tuner_status(
                    HANDLE_T                    h_svctx,
                    UINT8*                      pui1_tuner_status
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pui1_tuner_status = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pui1_tuner_status
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_tuner_status == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_tuner_status(
                    pt_svctx,
                    pui1_tuner_status
                    );

    } while (0);
    svctx_class_unlock();
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_svctx_get_cell_id
 *
 * Description: This API is used to get cell id
 * Inputs:  h_svctx          Contains a handle to a service context.
 *          UINT16*             Contains cell id pointer.
 *
 * Outputs: pui2_cell_id    Contains cell id pointer
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_ARG     pui2_num_recs is zero
 *          SVCTXR_WRONG_STATE If related SCDB is not ready and opened.
 *          SVCTXR_FAIL        SCDB error, cannot get the value.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_get_cell_id(
                    HANDLE_T                    h_svctx,
                    UINT16*                     pui2_cell_id
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pui2_cell_id = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pui2_cell_id
                    ));

    if (pui2_cell_id == NULL)
    {
        return SVCTXR_INV_ARG;
    }
    
    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_cell_id == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_cell_id(
                    pt_svctx,
                    pui2_cell_id
                    );

    } while (0);
    svctx_class_unlock();
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_svctx_get_scdb_num_recs
 *
 * Description: This API is used to set the number of records for a specific
 *              stream type.
 *              Note: for closed caption, it only return the amount that
 *              detected in the Service Information (e.g. In ATSC, it is
 *              from caption_service_descriptor).
 * Inputs:  h_svctx          Contains a handle to a service context.
 *          e_strm_type      Contains the stream type value.
 *
 * Outputs: pui2_num_recs    References to the variable containing the number of
 *                           records
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pui2_num_recs is zero
 *          SVCTXR_WRONG_STATE If related SCDB is not ready and opened.
 *          SVCTXR_FAIL        SCDB error, cannot get the value.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_get_scdb_num_recs(
                    HANDLE_T                     h_svctx,            /* in  */
                    STREAM_TYPE_T                e_strm_type,        /* in  */
                    UINT16*                      pui2_num_recs       /* in  */
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "e_strm_type = %d, "
                    "pui2_num_recs = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_strm_type,
                    pui2_num_recs
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pui2_num_recs == NULL) {
        return SVCTXR_INV_ARG;
    }

    *pui2_num_recs = 0;

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            return SVCTXR_FAIL;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_scdb_num_recs == NULL) {
            return SVCTXR_FAIL;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_scdb_num_recs(
                        pt_svctx,
                        e_strm_type,
                        pui2_num_recs
                        );

    } while (0);
    svctx_class_unlock();

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: x_svctx_get_cp_info
 *
 * Description: This API is used to get CP(Copy Protection) information for
 *              a specific stream. Normally, it is used when SVCTX_NTFY_CODE_CP
 *              is received.
 * Inputs:  h_svctx          Contains a handle to a service context.
 *          e_strm_type      Contains the stream type value.
 *
 * Outputs: pt_cp_info      References to the variable containing the Copy
 *                          Protection information
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pt_cp_info is NULL
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_get_cp_info(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type,        /* in  */
                    CP_INFO_LIST_T*                 pt_cp_info          /* out  */
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "e_strm_type = %d, "
                    "pt_cp_info = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_strm_type,
                    pt_cp_info
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pt_cp_info == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_cp_info == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_cp_info(
                        pt_svctx,
                        e_strm_type,
                        pt_cp_info
                        );
    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_get_ca_info
 *
 * Description: This API is used to get CA(Conditional Access) information
 *              for a specific stream. Normally, it is used when
 *              SVCTX_NTFY_CODE_CA is received.
 * Inputs:  h_svctx          Contains a handle to a service context.
 *          e_strm_type      Contains the stream type value.
 *
 * Outputs: pt_ca_info      References to the variable containing the CA information
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pt_ca_info is NULL
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_get_ca_info(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type,        /* in  */
                    SVCTX_CA_INFO_T*                pt_ca_info          /* out  */
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "e_strm_type = %d, "
                    "pt_ca_info = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_strm_type,
                    pt_ca_info
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pt_ca_info == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_cp_info == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_ca_info(
                            pt_svctx,
                            e_strm_type,
                            pt_ca_info
                            );
    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_set_listener
 *
 * Description: This API is used to set the listener for monitoring service
 *              context's activities.
 * Inputs:  h_svctx          Contains a handle to a service context.
 *          pf_listener_fct  References the service notify function, it can be NULL
 *          pv_nfy_tag       References the tag passed to the notify function.
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_set_listener(
                    HANDLE_T                        h_svctx,            /* in  */
                    x_svctx_listener_nfy_fct        pf_listener_fct,    /* in  */
                    VOID*                           pv_nfy_tag          /* in  */
                    )
{
    SVCTX_T*        pt_svctx;
    INT32           i4_ret   = SVCTXR_OK;
    SVCTX_LSTNR_T*  pt_lstnr = NULL;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pf_listener_fct = 0x%x, "
                    "pv_nfy_tag = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pf_listener_fct,
                    pv_nfy_tag
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        i4_ret = x_handle_get_tag(h_svctx, (VOID**)&pt_lstnr);
        if (i4_ret != HR_OK) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_lstnr != NULL) {
            if (pf_listener_fct != NULL) {
                pt_lstnr->pf_listener_fct = pf_listener_fct;
                pt_lstnr->pv_nfy_tag      = pv_nfy_tag;
            } else {
                pt_lstnr->pf_listener_fct = NULL;
                pt_lstnr->pv_nfy_tag      = NULL;
            }
        } else { /* pt_lstnr = NULL */
            if (pf_listener_fct != NULL) {
                pt_lstnr = (SVCTX_LSTNR_T*) x_mem_alloc(sizeof(SVCTX_LSTNR_T));
                if (pt_lstnr != NULL) {
                    pt_lstnr->pf_listener_fct = pf_listener_fct;
                    pt_lstnr->pv_nfy_tag      = pv_nfy_tag;
                } else {
                    DBG_ERROR((_ERROR_HEADER"create memory for SVCTX_LSTNR_T failed\r\n"));
                }
            } else {
                pt_lstnr = NULL;
            }
        }

        i4_ret = x_handle_set_tag(h_svctx, (VOID*)pt_lstnr);
        i4_ret = i4_ret == HR_OK ? SVCTXR_OK : SVCTXR_FAIL;
    } while (0);

    svctx_class_unlock();

    return i4_ret;
}

INT32 x_svctx_set_listener_no_lock(
                    HANDLE_T                        h_svctx,            /* in  */
                    x_svctx_listener_nfy_fct        pf_listener_fct,    /* in  */
                    VOID*                           pv_nfy_tag          /* in  */
                    )
{
    SVCTX_T*        pt_svctx;
    INT32           i4_ret   = SVCTXR_OK;
    SVCTX_LSTNR_T*  pt_lstnr = NULL;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        i4_ret = x_handle_get_tag(h_svctx, (VOID**)&pt_lstnr);
        if (i4_ret != HR_OK) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_lstnr != NULL) {
            if (pf_listener_fct != NULL) {
                pt_lstnr->pf_listener_fct = pf_listener_fct;
                pt_lstnr->pv_nfy_tag      = pv_nfy_tag;
            } else {
                pt_lstnr->pf_listener_fct = NULL;
                pt_lstnr->pv_nfy_tag      = NULL;
            }
        } else { /* pt_lstnr = NULL */
            if (pf_listener_fct != NULL) {
                pt_lstnr = (SVCTX_LSTNR_T*) x_mem_alloc(sizeof(SVCTX_LSTNR_T));
                if (pt_lstnr != NULL) {
                    pt_lstnr->pf_listener_fct = pf_listener_fct;
                    pt_lstnr->pv_nfy_tag      = pv_nfy_tag;
                } else {
                    DBG_ERROR((_ERROR_HEADER"create memory for SVCTX_LSTNR_T failed\r\n"));
                }
            } else {
                pt_lstnr = NULL;
            }
        }

        i4_ret = x_handle_set_tag(h_svctx, (VOID*)pt_lstnr);
        i4_ret = i4_ret == HR_OK ? SVCTXR_OK : SVCTXR_FAIL;
    } while (0);

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: x_svctx_grab_ownership
 *
 * Description: This API is used to grab the ownership of service selection. When
 *              the ownership is grabbed, the other client cannot issue select
 *              service request. However, the adjustment for video region,
 *              destination  region, video plane order don't be prohibited.
 * Inputs:  h_svctx          Contains a handle to a service context.
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_grab_ownership(
                    HANDLE_T                        h_svctx             /* in  */
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret         = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d"
                    ")\r\n",
                    __func__,
                    h_svctx
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        pt_svctx->h_svctx_owner = h_svctx;

        svctx_timer_start(
                    pt_svctx,
                    AUTO_RELEASE_OWNERSHIP_TIMEOUT,
                    _svctx_auto_release_ownership_cb,
                    (VOID*)pt_svctx
                    );
    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_release_ownership
 *
 * Description: This API is used to release the ownership of service selection
 * Inputs:  h_svctx          Contains a handle to a service context.
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_release_ownership(
                    HANDLE_T                        h_svctx             /* in  */
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret         = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d"
                    ")\r\n",
                    __func__,
                    h_svctx
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        pt_svctx->h_svctx_owner = NULL_HANDLE;
    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_set
 *
 * Description: This API is used to set the attribuites of a service context
 *
 * Inputs:  h_svctx           References to handle of a service context.
 *          e_set_type        References to the type of attributes to set.
 *          pv_set_info       References to the data information of the set
 *                            type.
 *          ui4_set_info_size Contains the size of data information of the
 *                            set type.
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_set(
                    HANDLE_T                        h_svctx,            /* in  */
                    SVCTX_SET_TYPE_T                e_set_type,         /* in  */
                    VOID*                           pv_set_info,        /* in  */
                    SIZE_T                          z_set_info_size     /* in  */
                    )
{
    SVCTX_T*        pt_svctx;
    INT32           i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "e_set_type = %d, "
                    "pv_set_info = 0x%x, "
                    "z_set_info_size = %d"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_set_type,
                    pv_set_info,
                    z_set_info_size
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    CLI_FUNC_CALL_TRACE(__func__);
    switch(e_set_type)
    {
        case SVCTX_COMMON_SET_SCDB_SORT_FUNC:
            if (z_set_info_size != sizeof(SVCTX_SCDB_SORT_PARAM_T) || pv_set_info == NULL) {
                return SVCTXR_INV_SET_INFO;
            }
            t_g_svctx_class.t_scdb_sort_param = *((SVCTX_SCDB_SORT_PARAM_T*)pv_set_info);
            return SVCTXR_OK;
            
        case SVCTX_COMMON_SET_CONTROL_RIKS_TV:
            if (z_set_info_size != sizeof(BOOL) || pv_set_info == NULL) {
                return SVCTXR_INV_SET_INFO;
            }
            
            t_g_svctx_class.b_riks_tv = *((BOOL*)pv_set_info);
            return SVCTXR_OK;
        default:
            break;
    }

    svctx_class_lock();
    do {
#ifdef MW_TV_AV_BYPASS_SUPPORT
        i4_ret = svctx_get_obj_with_ext_type(h_svctx, &pt_svctx);
#else
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
#endif
        if (i4_ret != SVCTXR_OK) {
            break;
        }

#ifdef MW_TV_AV_BYPASS_SUPPORT
        if (!IS_SVCTX_ASSOCIATE(pt_svctx))
        {
#endif
        switch(e_set_type) {
        case SVCTX_COMMON_SET_MHEG_SCREEN_MODE:
            if (z_set_info_size != sizeof(SVCTX_MHEG_SCREEN_MODE_T) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }

            pt_svctx->t_mheg_scr_mode = *((SVCTX_MHEG_SCREEN_MODE_T*)pv_set_info);
            break;
        case SVCTX_COMMON_SET_ATOMIC_CMD_GRP:
            {
                SVCTX_ATOMIC_CMD_T* pt_cmd = (SVCTX_ATOMIC_CMD_T*)pv_set_info;
                SM_COMMAND_T        at_sm_cmd[3];
                BOOL    b_flag = TRUE;
                INT32   i4_idx = 0;

                while(b_flag && i4_idx<3) {
                    switch(pt_cmd->t_type) {
                        case SVCTX_COMMON_SET_ATOMIC_DISP_RGN: {
                            pt_svctx->t_disp_region = *((VSH_REGION_INFO_T*)pt_cmd->pv_data);
                            at_sm_cmd[i4_idx].e_code = SCC_CMD_CODE_VIDEO_DISP_REGION;
                            at_sm_cmd[i4_idx++].u.pv_data = pt_cmd->pv_data;
                        }
                            break;
                        case SVCTX_COMMON_SET_ATOMIC_SCRN_MODE: {
                            pt_svctx->e_scr_mode = (pt_svctx->t_mheg_scr_mode.b_enable)?(pt_svctx->t_mheg_scr_mode.e_scr_mode):((SCC_VID_SCREEN_MODE_T)(UINT32)pt_cmd->pv_data);
                            at_sm_cmd[i4_idx].e_code = SCC_CMD_CODE_VIDEO_SCREEN_MODE;
                            at_sm_cmd[i4_idx++].u.ui4_data = (UINT32)pt_svctx->e_scr_mode;
                        }
                            break;
                        case SVCTX_COMMON_SET_ATOMIC_OVERSCAN: {
                            at_sm_cmd[i4_idx].e_code = SCC_CMD_CODE_VIDEO_OVERSCAN_CLIPPER;
                            at_sm_cmd[i4_idx++].u.pv_data = pt_cmd->pv_data;
                        }
                            break;                            
                        case SVCTX_COMMON_SET_ATOMIC_CMD_END: {
                            at_sm_cmd[i4_idx].e_code = SM_CMD_CODE_END;
                        }
                        default:
                            b_flag = FALSE;
                            break;
                    }

                    pt_cmd++;
                }

                x_scc_vid_set_vdp_atomic_cmd(pt_svctx->h_video_scc_comp, at_sm_cmd);
            }
            break;
        case SVCTX_COMMON_SET_TYPE_SNK_NAME:
            if (z_set_info_size != SVCTX_SNK_NAME_MAX_SIZE || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }
            x_strncpy(pt_svctx->s_snk_grp_name, (CHAR*)pv_set_info, SVCTX_SNK_NAME_MAX_SIZE);
            break;
        case SVCTX_COMMON_SET_TYPE_SRC_NAME:
            if (z_set_info_size != SVCTX_SRC_NAME_MAX_SIZE || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }
            x_strncpy(pt_svctx->s_src_grp_name, (CHAR*)pv_set_info, SVCTX_SRC_NAME_MAX_SIZE);
            break;
        case SVCTX_COMMON_SET_PMX_BG_ENABLE_FLAG:
            if (z_set_info_size != sizeof(BOOL) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }

            pt_svctx->b_able_set_pmx_bg = *((BOOL*)pv_set_info);
            break;
        case SVCTX_COMMON_SET_VIDEO_MUTE_VIA_PMX_BG:
          {
            SCC_BG_COLOR_T t_video_mute_color;
            BOOL           b_enable;

            if (z_set_info_size != sizeof(BOOL) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }

            b_enable = *((BOOL*)pv_set_info);

            /* when the singal status is locked, don't need to set PMX's background
             * otherwise, it will has a transition video mute background (PMX)
             * when the video is resizing
             */
            if (b_enable == TRUE
                    && pt_svctx->b_enable_video_mute_supervisor == TRUE
                    && pt_svctx->t_video_mute.b_enable == TRUE
                    && svctx_is_no_video(pt_svctx) == TRUE) {
                t_video_mute_color.ui1_red   = pt_svctx->t_video_mute.t_color.ui1_red;
                t_video_mute_color.ui1_green = pt_svctx->t_video_mute.t_color.ui1_green;
                t_video_mute_color.ui1_blue  = pt_svctx->t_video_mute.t_color.ui1_blue;
            } else {
                t_video_mute_color.ui1_red   = 0;
                t_video_mute_color.ui1_green = 0;
                t_video_mute_color.ui1_blue  = 0;
            }

            pt_svctx->b_able_set_pmx_bg = b_enable;

            i4_ret = x_scc_disp_set_bg(
                            pt_svctx->h_display_scc_comp,
                            &t_video_mute_color
                            );
            i4_ret = (i4_ret == SMR_OK) ? SVCTXR_OK : SVCTXR_FAIL;
            break;
          }
        case SVCTX_COMMON_SET_VIDEO_MUTE:
        case SVCTX_COMMON_SET_VIDEO_MUTE_VIA_FLAG:
        case SVCTX_COMMON_SET_VIDEO_MUTE_ATTR:
            if (e_set_type == SVCTX_COMMON_SET_VIDEO_MUTE) {
                if (z_set_info_size != sizeof(SVCTX_VIDEO_MUTE_INFO_T) || pv_set_info == NULL) {
                    i4_ret = SVCTXR_INV_SET_INFO;
                    break;
                }

                if (((SVCTX_VIDEO_MUTE_INFO_T*)pv_set_info)->b_enable == TRUE) {
                    pt_svctx->t_video_mute = *((SVCTX_VIDEO_MUTE_INFO_T*)pv_set_info);
                } else {
                    pt_svctx->t_video_mute.b_enable = FALSE;
                }
            } else if (e_set_type == SVCTX_COMMON_SET_VIDEO_MUTE_VIA_FLAG) {
                if (z_set_info_size != sizeof(BOOL) || pv_set_info == NULL) {
                    i4_ret = SVCTXR_INV_SET_INFO;
                    break;
                }
                pt_svctx->b_enable_video_mute_supervisor = *((BOOL*)pv_set_info);
                /*pt_svctx->t_video_mute.b_enable = *((BOOL*)pv_set_info);*/
            } else { /* SVCTX_COMMON_SET_VIDEO_MUTE_ATTR */
                pt_svctx->t_video_mute = *((SVCTX_VIDEO_MUTE_INFO_T*)pv_set_info);
                break;
            }

            if (pt_svctx->t_video_mute.b_enable == TRUE) {
                pt_svctx->b_able_set_pmx_bg = TRUE;
            }

            /* tricky: if s_snk_grp_name or s_src_grp_name is empty
             * it means the svctx is not enabled, ignore the "mute
             * video" operation
             */
            if (x_strlen(pt_svctx->s_snk_grp_name) != 0
                    && x_strlen(pt_svctx->s_src_grp_name) != 0
                    && svctx_is_no_video(pt_svctx) == TRUE) {
                _svctx_force_mute_video(
                    pt_svctx,
                    (pt_svctx->t_video_mute.b_enable ? VIDEO_MUTE_CTRL_ENABLE : VIDEO_MUTE_CTRL_DISABLE));
            }

            if (pt_svctx->pt_running_svc_req == NULL) {
                i4_ret = SVCTXR_OK;
            } else {
                i4_ret = SVCTXR_CONTINUE;
            }
            break;
        case SVCTX_COMMON_SET_BLOCK_MUTE:
            if (z_set_info_size != sizeof(SVCTX_VIDEO_MUTE_INFO_T) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }

            if (((SVCTX_VIDEO_MUTE_INFO_T*)pv_set_info)->b_enable == TRUE) {
                pt_svctx->t_block_mute = *((SVCTX_VIDEO_MUTE_INFO_T*)pv_set_info);
            } else {
                pt_svctx->t_block_mute.b_enable = FALSE;
            }
            break;
        case SVCTX_COMMON_SET_SCREEN_STATUS:
            if (z_set_info_size != sizeof(SVCTX_SCREEN_STATUS_T) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }
            pt_svctx->e_scrn_status = *((SVCTX_SCREEN_STATUS_T*)pv_set_info);
              break;
        case SVCTX_COMMON_SET_MTS:
            if (z_set_info_size != sizeof(SCC_AUD_MTS_T) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }
            i4_ret = x_scc_aud_set_mts(
                            pt_svctx->h_audio_scc_comp,
                            *((SCC_AUD_MTS_T*)pv_set_info)
                            );
            i4_ret = (i4_ret == SMR_OK) ? SVCTXR_OK : SVCTXR_FAIL;
            break;
        case SVCTX_COMMON_SET_STOP_AUDIO_AS_NO_SIGNAL:
            if (z_set_info_size != sizeof(UINT8) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }
            pt_svctx->ui1_stop_audio_as_no_signal = *((UINT8*)pv_set_info);
            i4_ret = SVCTXR_OK;
            break;
        case SVCTX_COMMON_SET_BLOCK_RULE:
            if (z_set_info_size != sizeof(SVCTX_BLOCK_RULE_T) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }

            pt_svctx->e_block_rule = *((SVCTX_BLOCK_RULE_T*)pv_set_info);
            i4_ret = SVCTXR_OK;
            break;
        case SVCTX_COMMON_SET_NO_SIGNAL_PRECEDE_BLOCK:
            if (z_set_info_size != sizeof(BOOL) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }

            if ((*((BOOL*)pv_set_info)) == TRUE) {
                pt_svctx->e_block_rule = SVCTX_BLOCK_RULE_INP_SIGNAL_LOSS_CH_PROG;
            } else {
                pt_svctx->e_block_rule = SVCTX_BLOCK_RULE_INP_CH_PROG_SIGNAL_LOSS;
            }
            i4_ret = SVCTXR_OK;
            break;
        case SVCTX_COMMON_SET_DEFALUT_BLOCK_CHECK_FCT:
            if (z_set_info_size != sizeof(x_svctx_svc_block_check_fct) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }
#ifdef SVCTX_SPLIT_SUPPORT
            pt_svctx->pf_block_check_fct = orig_cb_wrapper;
            pt_svctx->pf_block_check_fct_tag = (VOID*)(x_svctx_svc_block_check_fct)pv_set_info;
#else
            pt_svctx->pf_block_check_fct = (x_svctx_svc_block_check_fct)(UINT32)pv_set_info;
#endif
#ifdef MW_TV_AV_BYPASS_SUPPORT
#ifdef SVCTX_SPLIT_SUPPORT
            _svctx_set_shared_block_checker(pt_svctx, pt_svctx->pf_block_check_fct, pt_svctx->pf_block_check_fct_tag);
#else
            _svctx_set_shared_block_checker(pt_svctx, pt_svctx->pf_block_check_fct);
#endif
#endif
            i4_ret = SVCTXR_OK;
            break;

#ifdef SVCTX_SPLIT_SUPPORT
        case SVCTX_COMMON_SET_DEFALUT_BLOCK_CHECK_FCT_TAG:
            if (z_set_info_size != sizeof(SVCTX_BLOCK_CHECK_FCT_TAG_T) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }
            pt_svctx->pf_block_check_fct = ((SVCTX_BLOCK_CHECK_FCT_TAG_T*)pv_set_info)->pf_block_check_fct;
            pt_svctx->pf_block_check_fct_tag = ((SVCTX_BLOCK_CHECK_FCT_TAG_T*)pv_set_info)->pv_tag;
#ifdef MW_TV_AV_BYPASS_SUPPORT
            _svctx_set_shared_block_checker(pt_svctx, pt_svctx->pf_block_check_fct, pt_svctx->pf_block_check_fct_tag);
#endif
            i4_ret = SVCTXR_OK;
            break;
#endif

        case SVCTX_COMMON_SET_STREAM_SELECT_BLOCK:
            if (z_set_info_size != sizeof(BOOL) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }

            pt_svctx->b_select_strm_block = *((BOOL*)pv_set_info);
            break;

        case SVCTX_COMMON_SET_DEFAULT_VIDEO_MODE:
            if (z_set_info_size != sizeof(SCC_VID_MODE_T) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }
            pt_svctx->e_default_vid_mode = *((SCC_VID_MODE_T*)pv_set_info);

            if(pt_svctx->e_default_vid_mode == SCC_VID_DEINT || 
                pt_svctx->e_default_vid_mode == SCC_VID_NORMAL){
                svctx_cli_print_channel_change_log(pt_svctx, CH_XNG_STAGE_AP_SET_UNMUTE);
            }
                
            if ((svctx_is_no_video(pt_svctx) == FALSE) || (pt_svctx->b_enable_snow == TRUE)) {
                if (pt_svctx->b_freezed && pt_svctx->e_default_vid_mode != SCC_VID_BLANK)
                {
                    SCC_VID_SUPER_FREEZE_T t_super_freeze = {SCC_VID_SUPER_FREEZE_TYPE_FORCE, FALSE};
                    x_scc_vid_set_super_freeze(pt_svctx->h_video_scc_comp, &t_super_freeze);
                    x_scc_vid_set_force_unmute(pt_svctx->h_video_scc_comp, SCC_VID_FORCE_UNMUTE_TYPE_ATV_CHG_CH, FALSE);
                    pt_svctx->b_freezed = FALSE;
                }
                svctx_set_video_plane(pt_svctx, pt_svctx->e_default_vid_mode);
            } else if (pt_svctx->pt_running_svc_req != NULL) {
                if (pt_svctx->pt_running_svc_req->b_enable_i_frame) {
                    svctx_set_video_plane(pt_svctx, pt_svctx->e_default_vid_mode);
                }
            }
            i4_ret = SVCTXR_OK;
            break;
        case SVCTX_COMMON_SET_ENABLE_VIDEO:
            if (z_set_info_size != sizeof(BOOL) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            } else {
                BOOL b_enable = *((BOOL*)pv_set_info);

                if (b_enable == TRUE) {
                    pt_svctx->pt_running_svc_req->ui4_stream_mode |= ST_MASK_VIDEO;
                    if (svctx_is_no_video(pt_svctx) == FALSE) {
                        svctx_set_video_plane(pt_svctx, pt_svctx->e_default_vid_mode);
                    }
                    svctx_mute_video(pt_svctx, FALSE);
                } else {
                    pt_svctx->pt_running_svc_req->ui4_stream_mode &=~ST_MASK_VIDEO;
                    svctx_set_video_plane(pt_svctx, SCC_VID_BLANK);
                    svctx_mute_video(pt_svctx, TRUE);
                }
            }
            i4_ret = SVCTXR_OK;
            break;
        case SVCTX_COMMON_SET_SCART_OUT:
            if (z_set_info_size != sizeof(SCART_OUT_INFO_T) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }

            if (pt_svctx->pt_running_svc_req != NULL) {
                SCART_OUT_INFO_T* pt_scart_out = (SCART_OUT_INFO_T*)pv_set_info;

                if (pt_svctx->pt_running_svc_req->t_scart_out.pt_descs != NULL) {
                    x_mem_free(pt_svctx->pt_running_svc_req->t_scart_out.pt_descs);
                    pt_svctx->pt_running_svc_req->t_scart_out.pt_descs = NULL;
                }

                pt_svctx->pt_running_svc_req->t_scart_out.b_valid       = pt_scart_out->b_valid;
                pt_svctx->pt_running_svc_req->t_scart_out.ui1_num_descs = pt_scart_out->ui1_num_descs;

                if (pt_scart_out->b_valid == TRUE
                        && pt_scart_out->ui1_num_descs > 0
                        && pt_scart_out->pt_descs != NULL) {
                    pt_svctx->pt_running_svc_req->t_scart_out.pt_descs =
                            (SCART_OUT_DESC_T*) x_mem_alloc(
                                sizeof(SCART_OUT_DESC_T) * pt_scart_out->ui1_num_descs
                                );
                    if (pt_svctx->pt_running_svc_req->t_scart_out.pt_descs == NULL) {
                        i4_ret = SVCTXR_NO_RESOURCE;
                        break;
                    }

                    x_memcpy(pt_svctx->pt_running_svc_req->t_scart_out.pt_descs,
                                pt_scart_out->pt_descs,
                                sizeof(SCART_OUT_DESC_T) * pt_scart_out->ui1_num_descs
                                );
                }
                i4_ret = SVCTXR_CONTINUE;
            }
            break;
        case SVCTX_COMMON_SET_ANALOG_BLOCK_BIAS_FREQ:
        {
            CH_BLOCK_BIAS_FREQ_T* pt_bias_freq;

            if (z_set_info_size != sizeof(CH_BLOCK_BIAS_FREQ_T) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }

            pt_bias_freq = (CH_BLOCK_BIAS_FREQ_T*)pv_set_info;

            pt_svctx->b_enable_block_bias_freq = pt_bias_freq->b_enable;
            pt_svctx->ui4_block_bias_freq      = pt_bias_freq->ui4_freq;
            break;
        }
        case SVCTX_COMMON_SET_TYPE_SIGNAL_LOSS_DELAY_TIME:
        {
            SIGNAL_LOSS_DELAY_T* pt_cfg;
            SVCTX_HDLR_T*        pt_hdlr;

            if (z_set_info_size != sizeof(SIGNAL_LOSS_DELAY_T) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }

            pt_cfg = (SIGNAL_LOSS_DELAY_T*)pv_set_info;

            if (_svctx_find_handler(pt_cfg->e_src_type, &pt_hdlr) == TRUE) {
                pt_hdlr->ui4_video_mute_chk_delay = pt_cfg->ui4_time;
            }
            break;
        }
        case SVCTX_COMMON_SET_ALL_SCART_OUT_DISABLE:
            if (z_set_info_size != sizeof(BOOL) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }

            pt_svctx->b_disable_all_scart_out = *((BOOL*)pv_set_info);
            i4_ret = SVCTXR_CONTINUE;
            break;
        case SVCTX_COMMON_SET_DETECTION_MODE:
            if (z_set_info_size != sizeof(DETECTION_MODE_T) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }
            pt_svctx->e_detection_mode = *((DETECTION_MODE_T*)pv_set_info);
            i4_ret = SVCTXR_OK;
            break;
        case SVCTX_COMMON_SET_SHOW_SNOW_AS_NO_SIGNAL:
            if (z_set_info_size != sizeof(BOOL) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }

            pt_svctx->b_analog_signal_loss_show_snow = *((BOOL*)pv_set_info);
            svctx_ctrl_snow(pt_svctx, pt_svctx->b_analog_signal_loss_show_snow);
            i4_ret = SVCTXR_OK;
            break;
        case SVCTX_COMMON_SET_ADD_STREAM_MODE_BIT:
            if (z_set_info_size != sizeof(UINT32) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }
            /* this set command just change the stream mode, but do not select
             * the relative stream accordingly.
             */
            pt_svctx->ui4_stream_mode |= (*((UINT32*)pv_set_info));
            break;
        case SVCTX_COMMON_SET_DEL_STREAM_MODE_BIT:
            if (z_set_info_size != sizeof(UINT32) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }
            /* this set command just change the stream mode, but do not close
             * the relative stream accordingly.
             */
            pt_svctx->ui4_stream_mode &= ~(*((UINT32*)pv_set_info));
            break;
#ifdef MW_TV_AV_BYPASS_SUPPORT
        case SVCTX_COMMON_SET_ENABLE_SVC_SELECT_WITH_ASSOCIATE:
            if (z_set_info_size != sizeof(BOOL) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }

#ifdef MW_PIP_POP_SUPPORT
            if (TRUE == *((BOOL*)pv_set_info))
            {
                pt_svctx->ui4_assc_state &= ~SVCTX_ASSC_DISABLED;
            }
            else
            {
                pt_svctx->ui4_assc_state |= SVCTX_ASSC_DISABLED;
            }
#else
            pt_svctx->b_assc_enabled = *((BOOL*)pv_set_info);
#endif
            i4_ret = SVCTXR_OK;
            break;
        case SVCTX_COMMON_SET_COPY_PROTECTION:
            if (z_set_info_size != sizeof(SVCTX_COPY_PROTECTION_T) || pv_set_info == NULL) {
                i4_ret = SVCTXR_INV_SET_INFO;
                break;
            }

            pt_svctx->t_cp_mask = *((SVCTX_COPY_PROTECTION_T*)pv_set_info);
            i4_ret = SVCTXR_OK;
            break;
#endif
        default:
            i4_ret = SVCTXR_CONTINUE;
            break;
        }
#ifdef MW_TV_AV_BYPASS_SUPPORT
        }
        else
        {
            switch(e_set_type)
            {
                case SVCTX_COMMON_SET_DEFALUT_BLOCK_CHECK_FCT:
                    if (z_set_info_size != sizeof(x_svctx_svc_block_check_fct) || pv_set_info == NULL) {
                        i4_ret = SVCTXR_INV_SET_INFO;
                        break;
                    }
#ifdef SVCTX_SPLIT_SUPPORT
                    pt_svctx->pf_block_check_fct = orig_cb_wrapper;
                    pt_svctx->pf_block_check_fct_tag = (VOID*)(x_svctx_svc_block_check_fct)pv_set_info;
#else
                    pt_svctx->pf_block_check_fct = (x_svctx_svc_block_check_fct)pv_set_info;
#endif
                    i4_ret = SVCTXR_OK;
                    break;
#ifdef SVCTX_SPLIT_SUPPORT
                case SVCTX_COMMON_SET_DEFALUT_BLOCK_CHECK_FCT_TAG:
                    if (z_set_info_size != sizeof(SVCTX_BLOCK_CHECK_FCT_TAG_T) || pv_set_info == NULL) {
                        i4_ret = SVCTXR_INV_SET_INFO;
                        break;
                    }
                    pt_svctx->pf_block_check_fct = ((SVCTX_BLOCK_CHECK_FCT_TAG_T*)pv_set_info)->pf_block_check_fct;
                    pt_svctx->pf_block_check_fct_tag = ((SVCTX_BLOCK_CHECK_FCT_TAG_T*)pv_set_info)->pv_tag;
                    i4_ret = SVCTXR_OK;
                    break;
#endif
                case SVCTX_COMMON_SET_VIDEO_MUTE:
                case SVCTX_COMMON_SET_VIDEO_MUTE_VIA_FLAG:
                    if (e_set_type == SVCTX_COMMON_SET_VIDEO_MUTE) {
                        if (z_set_info_size != sizeof(SVCTX_VIDEO_MUTE_INFO_T) || pv_set_info == NULL) {
                            i4_ret = SVCTXR_INV_SET_INFO;
                            break;
                        }

                        if (((SVCTX_VIDEO_MUTE_INFO_T*)pv_set_info)->b_enable == TRUE) {
                            pt_svctx->t_video_mute = *((SVCTX_VIDEO_MUTE_INFO_T*)pv_set_info);
                        } else {
                            pt_svctx->t_video_mute.b_enable = FALSE;
                        }
                    } else { /* SVCTX_COMMON_SET_VIDEO_MUTE_VIA_FLAG */
                        if (z_set_info_size != sizeof(BOOL) || pv_set_info == NULL) {
                            i4_ret = SVCTXR_INV_SET_INFO;
                            break;
                        }
                        pt_svctx->b_enable_video_mute_supervisor = *((BOOL*)pv_set_info);
                        /*pt_svctx->t_video_mute.b_enable = *((BOOL*)pv_set_info);*/
                    }

                #if 0
                    if (pt_svctx->t_video_mute.b_enable == TRUE) {
                        pt_svctx->b_able_set_pmx_bg = TRUE;
                    }
                #endif

                    /* tricky: if s_snk_grp_name or s_src_grp_name is empty
                     * it means the svctx is not enabled, ignore the "mute
                     * video" operation
                     */
                    if (x_strlen(pt_svctx->s_snk_grp_name) != 0
                            && x_strlen(pt_svctx->s_src_grp_name) != 0
                            && svctx_is_no_video(pt_svctx) == TRUE
                            && pt_svctx->e_state == SVCTX_COND_PRESENTING) {
                        _svctx_force_mute_video(
                            pt_svctx,
                            (pt_svctx->t_video_mute.b_enable ? VIDEO_MUTE_CTRL_ENABLE : VIDEO_MUTE_CTRL_DISABLE));
                    }

                    if (pt_svctx->pt_running_svc_req == NULL) {
                        i4_ret = SVCTXR_OK;
                    } else {
                        i4_ret = SVCTXR_CONTINUE;
                    }
                    break;
                case SVCTX_COMMON_SET_ENABLE_SVC_SELECT_WITH_ASSOCIATE:
                    if (z_set_info_size != sizeof(BOOL) || pv_set_info == NULL) {
                        i4_ret = SVCTXR_INV_SET_INFO;
                        break;
                    }

#ifdef MW_PIP_POP_SUPPORT
                    if (TRUE == *((BOOL*)pv_set_info))
                    {
                        pt_svctx->ui4_assc_state &= ~SVCTX_ASSC_DISABLED;
                    }
                    else
                    {
                        pt_svctx->ui4_assc_state |= SVCTX_ASSC_DISABLED;
                    }
#else
                    pt_svctx->b_assc_enabled = *((BOOL*)pv_set_info);
#endif
                    i4_ret = SVCTXR_OK;
                    break;
                    
                case SVCTX_COMMON_SET_KEEP_ALIVE_AS_STOP_LEAD:
                    if (z_set_info_size != sizeof(BOOL) || pv_set_info == NULL) 
                    {
                        i4_ret = SVCTXR_INV_SET_INFO;
                        break;
                    }
                    
                    if (*((BOOL*)pv_set_info))
                    {
                        pt_svctx->ui4_attribute |= SVCTX_ASSC_ATTR_KEEP_ALIVE_AS_STOP_LEAD;
                    }
                    else
                    {
                        pt_svctx->ui4_attribute &= (~SVCTX_ASSC_ATTR_KEEP_ALIVE_AS_STOP_LEAD);
                    }
                    i4_ret = SVCTXR_OK;
                    break;
                    
                case SVCTX_COMMON_SET_COPY_PROTECTION:
                    if (z_set_info_size != sizeof(SVCTX_COPY_PROTECTION_T) || pv_set_info == NULL) {
                        i4_ret = SVCTXR_INV_SET_INFO;
                        break;
                    }

                    pt_svctx->t_cp_mask = *((SVCTX_COPY_PROTECTION_T*)pv_set_info);
                    i4_ret = SVCTXR_OK;
                    break;

                default:
                    i4_ret = SVCTXR_CONTINUE;
                    break;
            }
        }
#endif

        if (i4_ret != SVCTXR_CONTINUE) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            SVCTX_HDLR_T* pt_hdlr = NULL;

            i4_ret = SVCTXR_WRONG_STATE;

            DLIST_FOR_EACH(pt_hdlr, &t_g_handlers, t_dlink) {
                if (pt_hdlr->t_fcts.pf_set == NULL) continue;
                i4_ret = pt_hdlr->t_fcts.pf_set(
                            pt_svctx,
                            e_set_type,
                            pv_set_info,
                            z_set_info_size
                            );
                if (i4_ret == SVCTXR_OK) {
                    break;
                }
            }
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_set == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_set(
                            pt_svctx,
                            e_set_type,
                            pv_set_info,
                            z_set_info_size
                            );
    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_get
 *
 * Description: This API is used to get the attribuites of a service context.
 *
 * Inputs:  h_svctx            References to handle of a service context.
 *          e_get_type         References to the type of attributes to get.
 *          pv_get_info        References to the data information of the get
 *                             type.
 *          pui4_get_info_size References to the size of data information of
 *                             the get type.
 *
 * Outputs: pv_get_info        References to the data information of the get
 *                             type.
 *          pui4_get_info_size References to the size of data information of
 *                             the get type.
 *
 * Returns: SVCTXR_OK              Success.
 *          SVCTXR_INV_HANDLE      The h_svctx is invalid.
 *          SVCTXR_WRONG_STATE     cannot get relative info when no any runngin request
 *          SVCTXR_NOT_SUPPORT     not support
 *          SVCTXR_FAIL            failed
 ----------------------------------------------------------------------------*/
INT32 x_svctx_get(
                    HANDLE_T                        h_svctx,            /* in  */
                    SVCTX_GET_TYPE_T                e_get_type,         /* in  */
                    VOID*                           pv_get_info,        /* in/out */
                    SIZE_T*                         pz_get_info_size    /* in/out */
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "e_get_type = %d, "
                    "pv_get_info = 0x%x, "
                    "pui4_get_info_size = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_get_type,
                    pv_get_info,
                    pz_get_info_size
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        i4_ret = _svctx_get(
                    pt_svctx,
                    e_get_type,
                    pv_get_info,
                    pz_get_info_size
                    );
    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_get_current_event_info_len
 *
 * Description:
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 x_svctx_get_current_event_info_len (
                    HANDLE_T                        h_svctx,            /* in  */
                    EVCTX_KEY_TYPE_T                e_key_type,
                    VOID*                           pv_key_info,        /* in  */
                    SIZE_T*                         pz_event_info_len   /* out */
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

#ifndef MW_EDB_SUPPORT
    EVCTX_KEY_TYPE_T    e_evn_key_type = e_key_type;
#else
    EDB_KEY_TYPE_T      e_evn_key_type = EDB_KEY_TYPE_UNKNOWN;
    switch (e_key_type)
    {
        case EVCTX_KEY_TYPE_RATING:            e_evn_key_type = EDB_KEY_TYPE_RATING_LIST_BY_ID;   break;
        case EVCTX_KEY_TYPE_EVENT_TEXT:        e_evn_key_type = EDB_KEY_TYPE_EVENT_DETAIL_BY_ID;  break;
        case EVCTX_KEY_TYPE_EVENT_ID:          e_evn_key_type = EDB_KEY_TYPE_EVENT_ID;            break;
        case EVCTX_KEY_TYPE_DURATION:          e_evn_key_type = EDB_KEY_TYPE_DURATION;            break;
        case EVCTX_KEY_TYPE_START_TIME:        e_evn_key_type = EDB_KEY_TYPE_START_TIME;          break;
        case EVCTX_KEY_TYPE_EVENT_TITLE:       e_evn_key_type = EDB_KEY_TYPE_EVENT_TITLE_BY_ID;   break;
        case EVCTX_KEY_TYPE_EVENT_TITLE_STYLE: e_evn_key_type = EDB_KEY_TYPE_EVENT_TITLE_BY_ID;   break;
        default: return SVCTXR_NOT_SUPPORT;
    }
#endif

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "e_key_type = %d, "
                    "pv_key_info = 0x%x, "
                    "pz_event_info_len = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_key_type,
                    pv_key_info,
                    pz_event_info_len
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_current_event_info_len == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_current_event_info_len(
                    pt_svctx,
                    pt_svctx->pt_running_svc_req,
                    e_evn_key_type,
                    pv_key_info,
                    pz_event_info_len
                    );
    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_get_current_event_info
 *
 * Description:
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 x_svctx_get_current_event_info (
                    HANDLE_T                        h_svctx,            /* in  */
                    EVCTX_KEY_TYPE_T                e_key_type,         /* in  */
                    VOID*                           pv_key_info,        /* in  */
                    SIZE_T*                         pz_event_info_len,  /* in/out */
                    VOID*                           pv_event_info       /* out */
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

#ifndef MW_EDB_SUPPORT        
    EVCTX_KEY_TYPE_T    e_evn_key_type = e_key_type;
#else
    EDB_KEY_TYPE_T      e_evn_key_type = EDB_KEY_TYPE_UNKNOWN;
    switch (e_key_type)
    {
        case EVCTX_KEY_TYPE_RATING:            e_evn_key_type = EDB_KEY_TYPE_RATING_LIST_BY_ID;   break;
        case EVCTX_KEY_TYPE_EVENT_TEXT:        e_evn_key_type = EDB_KEY_TYPE_EVENT_DETAIL_BY_ID;  break;
        case EVCTX_KEY_TYPE_EVENT_ID:          e_evn_key_type = EDB_KEY_TYPE_EVENT_ID;            break;
        case EVCTX_KEY_TYPE_DURATION:          e_evn_key_type = EDB_KEY_TYPE_DURATION;            break;
        case EVCTX_KEY_TYPE_START_TIME:        e_evn_key_type = EDB_KEY_TYPE_START_TIME;          break;
        case EVCTX_KEY_TYPE_EVENT_TITLE:       e_evn_key_type = EDB_KEY_TYPE_EVENT_TITLE_BY_ID;   break;
        case EVCTX_KEY_TYPE_EVENT_TITLE_STYLE: e_evn_key_type = EDB_KEY_TYPE_EVENT_TITLE_BY_ID;   break;
        default: return SVCTXR_NOT_SUPPORT;
    }
#endif

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "e_key_type = %d, "
                    "pv_key_info = 0x%x, "
                    "pz_event_info_len = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_key_type,
                    pv_key_info,
                    pz_event_info_len
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_current_event_info == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_current_event_info(
                    pt_svctx,
                    pt_svctx->pt_running_svc_req,
                    e_evn_key_type,
                    pv_key_info,
                    pz_event_info_len,
                    pv_event_info
                    );
    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_get_mapped_dev_info
 * Description This API is used to set AV device information
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 x_svctx_get_mapped_dev_info(
                    DRV_TYPE_T                  e_drv_type,
                    DEVICE_TYPE_T*              pe_video_dev_type,
                    DEVICE_TYPE_T*              pe_best_video_dev_type,
                    INT8*                       pi1_best_video_dev_order,
                    DEVICE_TYPE_T*              pe_audio_dev_type,
                    DEVICE_TYPE_T*              pe_best_audio_dev_type,
                    INT8*                       pi1_best_audio_dev_order
                    )
{
    DEVICE_TYPE_PROFILE_T* pt_dev_profile;
    DEVICE_TYPE_T          e_dev_type;
    UINT8                  ui1_idx;
    INT32                  i4_ret = SVCTXR_NOT_EXIST;

    for (ui1_idx = 0; ui1_idx < (UINT8) z_g_elem_count_of_avc_table; ui1_idx++) {

        if (pt_g_avc_mapping_table[ui1_idx].e_drv_type != e_drv_type) {
            continue;
        }

        pt_dev_profile = &(pt_g_avc_mapping_table[ui1_idx].t_profile);
        e_dev_type     = DEV_GET_GROUP_ID(pt_dev_profile->e_dev_type) == DEV_TUNER ? DEV_TUNER : pt_dev_profile->e_dev_type;

        /* video */
        if (pt_dev_profile->ui4_av_class_mask & ST_MASK_VIDEO) {
            /* if already in Tuner or COMBI, other video device is auxiliary */
            if (DEV_GET_GROUP_ID((*pe_video_dev_type)) != DEV_TUNER
                    && (*pe_video_dev_type) != DEV_AVC_COMBI) {
                *pe_video_dev_type = e_dev_type;
            }
        }

        if (pt_dev_profile->ui4_strm_capable_mask & ST_MASK_VIDEO) {
            if ((*pi1_best_video_dev_order) < pt_dev_profile->i1_video_quality_order) {
                if (DEV_GET_GROUP_ID((*pe_video_dev_type)) != DEV_TUNER || DEV_GET_GROUP_ID((pt_dev_profile->e_dev_type)) == DEV_TUNER) {
                    *pe_best_video_dev_type   = e_dev_type;
                    *pi1_best_video_dev_order = pt_dev_profile->i1_video_quality_order;
                }
            }
        }

        /* audio */
        if (pt_dev_profile->ui4_av_class_mask & ST_MASK_AUDIO) {
            /* if already in Tuner or COMBI, other audio device is auxiliary */
            if (DEV_GET_GROUP_ID((*pe_audio_dev_type)) != DEV_TUNER
                    && (*pe_audio_dev_type) != DEV_AVC_COMBI) {
                *pe_audio_dev_type = e_dev_type;
            }
        }

        if (pt_dev_profile->ui4_strm_capable_mask & ST_MASK_AUDIO) {
            if ((*pi1_best_audio_dev_order) < pt_dev_profile->i1_audio_quality_order) {
                if (DEV_GET_GROUP_ID((*pe_audio_dev_type)) != DEV_TUNER || DEV_GET_GROUP_ID((pt_dev_profile->e_dev_type)) == DEV_TUNER) {
                    *pe_best_audio_dev_type   = e_dev_type;
                    *pi1_best_audio_dev_order = pt_dev_profile->i1_audio_quality_order;
                }
            }
        }

        i4_ret = SVCTXR_OK;
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_init_device_mapping_table
 * Description This API is used to set AVC type mapping table
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 x_svctx_init_device_mapping_table(
                    const AVC_TYPE_MAPPING*     pt_mapping_table,
                    SIZE_T                      z_num_of_element
                    )
{
    UINT8 ui1_idx;

    if (z_num_of_element == 0) {
        return SVCTXR_INV_ARG;
    }

    if (z_num_of_element != z_g_elem_count_of_avc_table) {
        if (pt_g_avc_mapping_table != NULL) {
            x_mem_free(pt_g_avc_mapping_table);
            pt_g_avc_mapping_table      = NULL;
            z_g_elem_count_of_avc_table = 0;
        }

        pt_g_avc_mapping_table = x_mem_alloc(sizeof(AVC_TYPE_MAPPING) * z_num_of_element);
        if (pt_g_avc_mapping_table == NULL) {
            return SVCTXR_NO_RESOURCE;
        }
    }
    z_g_elem_count_of_avc_table = z_num_of_element;

    for (ui1_idx = 0; ui1_idx < (UINT8) z_num_of_element; ui1_idx++) {
        pt_g_avc_mapping_table[ui1_idx] = pt_mapping_table[ui1_idx];
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_get_device_profile
 * Description This API is used to set AV device profile structure
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 x_svctx_get_device_profile(
                    DEVICE_TYPE_T               e_dev_type,
                    DEVICE_TYPE_PROFILE_T*      pt_profile
                    )
{
    UINT8 ui1_idx;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pt_profile == NULL) {
        return SVCTXR_INV_ARG;
    }

    for (ui1_idx = 0; ui1_idx < (UINT8) z_g_elem_count_of_avc_table; ui1_idx++) {
        if ((e_dev_type == DEV_TUNER && DEV_GET_GROUP_ID(pt_g_avc_mapping_table[ui1_idx].t_profile.e_dev_type) == DEV_TUNER)
                || e_dev_type == pt_g_avc_mapping_table[ui1_idx].t_profile.e_dev_type) {
            *pt_profile = pt_g_avc_mapping_table[ui1_idx].t_profile;
            return SVCTXR_OK;
        }
    }

    return SVCTXR_NOT_EXIST;
}
/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_get_device_profile_by_drv_type
 * Description This API is used to set AV device profile structure
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 x_svctx_get_device_profile_by_drv_type(
                    DRV_TYPE_T                  e_drv_type,
                    DEVICE_TYPE_PROFILE_T*      pt_profile
                    )
{
    UINT8 ui1_idx;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pt_profile == NULL) {
        return SVCTXR_INV_ARG;
    }

    for (ui1_idx = 0; ui1_idx < (UINT8) z_g_elem_count_of_avc_table; ui1_idx++) {
        if (e_drv_type == pt_g_avc_mapping_table[ui1_idx].e_drv_type) {
            *pt_profile = pt_g_avc_mapping_table[ui1_idx].t_profile;
            return SVCTXR_OK;
        }
    }

    return SVCTXR_NOT_EXIST;
}
/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_is_frozen
 * Description This API is used to check svctx is frozen or not
 * Inputs:  h_svctx                Contains a handle to a service context
 * Outputs: pb_result              Contains a flag for the result
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 x_svctx_is_frozen(
                    HANDLE_T                        h_svctx,
                    BOOL*                           pb_result
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pb_result = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pb_result
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pb_result == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->ui4_frz_frozen_strm_mask == 0) {
            *pb_result = FALSE;
        } else {
            *pb_result = TRUE;
        }
    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_get_well_defined_video_heigh
 * Description This API is used to get well-defined video height
 * Inputs:  ui4_raw_height         Contains the raw video height, e.g. 1088
 * Outputs: pui4_defined_height    Contains the well-defined video height. e.g. 1080
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 x_svctx_get_well_defined_video_heigh(
                    UINT32                          ui4_raw_height,
                    UINT32*                         pui4_defined_height
                    )
{
    if (pui4_defined_height == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (ui4_raw_height >= VIDEO_HEIGHT_480_LOWER_BOUND && ui4_raw_height <= VIDEO_HEIGHT_480_UPPER_BOUND) {
        *pui4_defined_height = VIDEO_HEIGHT_480;
    } else if (ui4_raw_height >= VIDEO_HEIGHT_576_LOWER_BOUND && ui4_raw_height <= VIDEO_HEIGHT_576_UPPER_BOUND) {
        *pui4_defined_height = VIDEO_HEIGHT_576;
    } else if (ui4_raw_height >= VIDEO_HEIGHT_720_LOWER_BOUND && ui4_raw_height <= VIDEO_HEIGHT_720_UPPER_BOUND) {
        *pui4_defined_height = VIDEO_HEIGHT_720;
    } else if (ui4_raw_height >= VIDEO_HEIGHT_1080_LOWER_BOUND && ui4_raw_height <= VIDEO_HEIGHT_1080_UPPER_BOUND) {
        *pui4_defined_height = VIDEO_HEIGHT_1080;
    } else {
        *pui4_defined_height = ui4_raw_height;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_get_cc_capability
 * Description This API is used to get cc capability of a svctx
 * Inputs:  h_svctx                References to handle of a service context.
 * Outputs: pt_capablility         Contains the cc capable condition
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 x_svctx_get_cc_capability(
                    HANDLE_T                    h_svctx,
                    SVCTX_CAPABILITY_T*         pt_capablility
                    )
{
    SVCTX_T*                  pt_svctx           = NULL;
    DEVICE_TYPE_PROFILE_T*    pt_profile         = NULL;
    DEVICE_TYPE_T             e_video_dev_type   = DEV_UNKNOWN;
    SIZE_T                    z_get_info_size    = 0;
    UINT32                    ui4_video_res_mask = VIDEO_RES_MASK_NONE;
    INT32                     i4_ret             = SVCTXR_OK;
    UINT8                     ui1_idx            = 0;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pt_capablility = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pt_capablility
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pt_capablility == NULL) {
        return SVCTXR_INV_ARG;
    }

    *pt_capablility = SVCTX_CAPABILITY_NO;

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        z_get_info_size = sizeof(e_video_dev_type);
        i4_ret = _svctx_get(
                    pt_svctx,
                    SVCTX_COMMON_GET_TYPE_CRNT_VIDEO_DEVICE_TYPE,
                    &e_video_dev_type,
                    &z_get_info_size
                    );
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        ui4_video_res_mask = _svctx_get_video_res_mask(&pt_svctx->t_video_info);

        for (ui1_idx = 0; ui1_idx < (UINT8) z_g_elem_count_of_avc_table; ui1_idx++) {
            pt_profile = &(pt_g_avc_mapping_table[ui1_idx].t_profile);
            if (pt_profile->e_dev_type != e_video_dev_type) {
                continue;
            }

            /* no cc capability */
            if ((pt_profile->ui4_strm_capable_mask & ST_MASK_CLOSED_CAPTION) == 0) {
                *pt_capablility = SVCTX_CAPABILITY_NO;
                break;
            }

            /* depend on resolution */
            if (pt_profile->t_cc_capability.b_video_res_dependent == TRUE) {
                /* no matched resolution */
                if ((pt_profile->t_cc_capability.ui4_video_res_mask & ui4_video_res_mask) == 0) {
                    *pt_capablility = SVCTX_CAPABILITY_NO;
                    break;
                }
            }

            *pt_capablility = SVCTX_CAPABILITY_YES;
            break;
        }

    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_get_subtitle_capability
 * Description This API is used to get subtitle capability of a svctx
 * Inputs:  h_svctx                References to handle of a service context.
 * Outputs: pt_capablility         Contains the cc capable condition
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 x_svctx_get_subtitle_capability(
                    HANDLE_T                    h_svctx,
                    SVCTX_CAPABILITY_T*         pt_capablility
                    )
{
    SVCTX_T*               pt_svctx           = NULL;
    DEVICE_TYPE_PROFILE_T* pt_profile         = NULL;
    DEVICE_TYPE_T          e_video_dev_type   = DEV_UNKNOWN;
    SIZE_T                 z_get_info_size    = 0;
    UINT32                 ui4_video_res_mask = VIDEO_RES_MASK_NONE;
    INT32                  i4_ret             = SVCTXR_OK;
    UINT8                  ui1_idx            = 0;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pt_capablility = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pt_capablility
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pt_capablility == NULL) {
        return SVCTXR_INV_ARG;
    }

    *pt_capablility = SVCTX_CAPABILITY_NO;

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        z_get_info_size = sizeof(e_video_dev_type);
        i4_ret = _svctx_get(
                    pt_svctx,
                    SVCTX_COMMON_GET_TYPE_CRNT_VIDEO_DEVICE_TYPE,
                    &e_video_dev_type,
                    &z_get_info_size
                    );
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        ui4_video_res_mask = _svctx_get_video_res_mask(&pt_svctx->t_video_info);

        for (ui1_idx = 0; ui1_idx < (UINT8) z_g_elem_count_of_avc_table; ui1_idx++) {
            pt_profile = &(pt_g_avc_mapping_table[ui1_idx].t_profile);
            if (pt_profile->e_dev_type != e_video_dev_type) {
                continue;
            }

            /* no subtitle capability */
            if ((pt_profile->ui4_strm_capable_mask & ST_MASK_SUBTITLE) == 0) {
                *pt_capablility = SVCTX_CAPABILITY_NO;
                break;
            }

            /* depend on resolution */
            if (pt_profile->t_subtitle_capability.b_video_res_dependent == TRUE) {
                /* no matched resolution */
                if ((pt_profile->t_subtitle_capability.ui4_video_res_mask & ui4_video_res_mask) == 0) {
                    *pt_capablility = SVCTX_CAPABILITY_NO;
                    break;
                }
            }

            *pt_capablility = SVCTX_CAPABILITY_YES;
            break;
        }

    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_get_teletext_capability
 * Description This API is used to get teletext capability of a svctx
 * Inputs:  h_svctx                References to handle of a service context.
 * Outputs: pt_capablility         Contains the cc capable condition
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 x_svctx_get_teletext_capability(
                    HANDLE_T                    h_svctx,
                    SVCTX_CAPABILITY_T*         pt_capablility
                    )
{
    SVCTX_T*               pt_svctx           = NULL;
    DEVICE_TYPE_PROFILE_T* pt_profile         = NULL;
    DEVICE_TYPE_T          e_video_dev_type   = DEV_UNKNOWN;
    SIZE_T                 z_get_info_size    = 0;
    UINT32                 ui4_video_res_mask = VIDEO_RES_MASK_NONE;
    INT32                  i4_ret             = SVCTXR_OK;
    UINT8                  ui1_idx            = 0;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pt_capablility = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pt_capablility
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pt_capablility == NULL) {
        return SVCTXR_INV_ARG;
    }

    *pt_capablility = SVCTX_CAPABILITY_NO;

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        z_get_info_size = sizeof(e_video_dev_type);
        i4_ret = _svctx_get(
                    pt_svctx,
                    SVCTX_COMMON_GET_TYPE_CRNT_VIDEO_DEVICE_TYPE,
                    &e_video_dev_type,
                    &z_get_info_size
                    );
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        ui4_video_res_mask = _svctx_get_video_res_mask(&pt_svctx->t_video_info);

        for (ui1_idx = 0; ui1_idx < (UINT8) z_g_elem_count_of_avc_table; ui1_idx++) {
            pt_profile = &(pt_g_avc_mapping_table[ui1_idx].t_profile);
            if (pt_profile->e_dev_type != e_video_dev_type) {
                continue;
            }

            /* no teletext capability */
            if ((pt_profile->ui4_strm_capable_mask & ST_MASK_TELETEXT) == 0) {
                *pt_capablility = SVCTX_CAPABILITY_NO;
                break;
            }

            /* depend on resolution */
            if (pt_profile->t_teletext_capability.b_video_res_dependent == TRUE) {
                /* no matched resolution */
                if ((pt_profile->t_teletext_capability.ui4_video_res_mask & ui4_video_res_mask) == 0) {
                    *pt_capablility = SVCTX_CAPABILITY_NO;
                    break;
                }
            }

            *pt_capablility = SVCTX_CAPABILITY_YES;
            break;
        }

    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_get_event_capability
 * Description This API is used to get event capability of a svctx
 * Inputs:  h_svctx                References to handle of a service context.
 *          ui4_event_key_mask     = MAKE_BIT_MASK_32(EDB_KEY_TYPE_T)
 * Outputs: pt_capablility         Contains the cc capable condition
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 x_svctx_get_event_capability(
                    HANDLE_T                    h_svctx,
                    UINT32                      ui4_event_key_mask,
                    SVCTX_CAPABILITY_T*         pt_capablility
                    )
{
    SVCTX_T*      pt_svctx         = NULL;
    DEVICE_TYPE_T e_video_dev_type = DEV_UNKNOWN;
    SIZE_T        z_get_info_size  = 0;
    INT32         i4_ret           = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "pt_capablility = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    pt_capablility
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pt_capablility == NULL) {
        return SVCTXR_INV_ARG;
    }

    *pt_capablility = SVCTX_CAPABILITY_NO;

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        z_get_info_size = sizeof(e_video_dev_type);
        i4_ret = _svctx_get(
                    pt_svctx,
                    SVCTX_COMMON_GET_TYPE_CRNT_VIDEO_DEVICE_TYPE,
                    &e_video_dev_type,
                    &z_get_info_size
                    );
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if ( svctx_with_event_capability(
                    pt_svctx,
                    e_video_dev_type,
                    ui4_event_key_mask
                    ) == TRUE) {
            *pt_capablility = SVCTX_CAPABILITY_YES;
        } else {
            *pt_capablility = SVCTX_CAPABILITY_NO;
        }
    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_open_scdb
 * Description This API returns a handle to the SCDB object for a specified svctx.
 *             Basically, it is a help function to let caller to handle
 *             the SCDB by itself, but the return value  is SVCTXR_XXX
 *              not SMR_XXX
 * Inputs:  h_svctx        Specifies the svctx handle that was
 *                         used to create the SCDB object.
 *          e_pipe_type    Additional info for specifing pipe. it could be ST_VIDEO,
 *                         ST_AUDIO, or ST_UNKNOWN
 *          pv_tag:        Private tag value associated with the
 *                         calling client.
 *          pf_nfy:        Client notification function.
 * Outputs: ph_scdb        pointer to a SCDB handle
 * Returns: SVCTXR_OK           (SCDBR_OK)               A new handle is created
 *          SVCTXR_NO_RESOURCE  (SCDBR_OUT_OF_HANDLE)    No more handle available,
 *                              (SCDBR_OUT_OF_MEM)       or Out of memory.
 *          SVCTXR_INV_ARG      (SCDBR_INV_ARG)          ph_scdb is NULL.
 *          SVCTXR_NOT_EXIST    (SCDBR_NOT_FOUND)        Specified database object is not found.
 *          SVCTXR_INV_HANDLE                            bad 'h_svctx' handle.
 *          SVCTXR_WRONG_STATE                           connection is not available to create SCDB
 *          SVCTXR_FAIL                                  failed
 *---------------------------------------------------------------------------*/
INT32 x_svctx_open_scdb(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_pipe_type,
                    HANDLE_T*                       ph_scdb,
                    VOID*                           pv_tag,
                    x_scdb_nfy_fct                  pf_nfy
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "e_pipe_type = %d, "
                    "ph_scdb = 0x%x, "
                    "pv_tag = 0x%x, "
                    "pf_nfy =  0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_pipe_type,
                    ph_scdb,
                    pv_tag,
                    pf_nfy
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (ph_scdb == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_open_scdb == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_open_scdb(
                            pt_svctx,
                            e_pipe_type,
                            ph_scdb,
                            pv_tag,
                            pf_nfy
                            );
    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name:  x_svctx_close_scdb
 *
 * Description: This API frees the specified handle to the SCDB database.
 *              Basically, it is a help function to let caller to handle
 *              the SCDB by itself, but the return value is SVCTXR_XXX
 *              not SMR_XXX
 * Inputs:  h_scdb:  handle to be freed.
 *
 * Outputs: None.
 *
 * Returns: SVCTXR_OK           (SCDBR_OK)            SCDB handle and its resource are free.
 *          SVCTXR_INV_HANDLE   (SCDBR_INV_HANDLE)    Bad SCDB handle.
 ----------------------------------------------------------------------------*/
INT32 x_svctx_close_scdb(
                    HANDLE_T                        h_scdb
                    )
{
    INT32 i4_ret;

    i4_ret = x_scdb_close(h_scdb);

    return svctx_translate_scdbr_2_svctxr(i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_open_stream
 *
 * Description: This function is used to open a stream in a specific svctx.
 *              Basically, it is a help function to let caller to handle
 *              the stream by itself, but the return value  is SVCTXR_XXX
 *              not SMR_XXX
 * Inputs:  h_svctx         References to the svctx.
 *          e_pipe_type     Additional info for specifing pipe. it could be ST_VIDEO,
 *                          ST_AUDIO, or ST_UNKNOWN
 *          pt_strm_comp_id References to the component ID aquired from SCDB.
 *          pt_cmd          References to the command sets to initialize the
 *                          stream.
 *          pf_strm_nfy     References to a stream event callback function,
 *                          which is implemented by the caller.
 *          pv_strm_nfy_tag References to a tag to be pass to the stream event
 *                          callback function.
 *          b_auto_play     Indicate if the stream sould play automatically.
 *
 * Outputs: ph_stream       References to the stream handle created by Stream
 *                          Manager.
 *
 * Returns: SVCTXR_OK          (SMR_OK)             Success.
 *          SVCTXR_NOT_SUPPORT (SMR_NOT_SUPPORT)    There is no stream handler
 *                                                  that support this
 *                                                  type of stream.
 *          SVCTXR_INV_ARG     (SMR_INV_ARG)        The argument ph_stream,
 *                                                  pf_strm_nfy,or
 *                                                  pt_strm_comp_id is invalid.
 *          SVCTXR_INV_HANDLE                       The h_svctx is invalid.
 *          SVCTXR_WRONG_STATE                      session is not available
 *          SVCTXR_FAIL                             failed
  *---------------------------------------------------------------------------*/
INT32 x_svctx_open_stream(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_pipe_type,
                    const STREAM_COMP_ID_T*         pt_strm_comp_id,
                    const SM_COMMAND_T*             pt_cmd_sets,
                    x_stream_nfy_fct                pf_strm_nfy,
                    VOID*                           pv_strm_nfy_tag,
                    BOOL                            b_auto_play,
                    HANDLE_T*                       ph_stream
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "e_pipe_type = %d, "
                    "pt_strm_comp_id = 0x%x, "
                    "pt_cmd_sets = 0x%x, "
                    "pf_strm_nfy = 0x%x"
                    "pv_strm_nfy_tag = 0x%x"
                    "b_auto_play = %d"
                    "ph_stream = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_pipe_type,
                    pt_strm_comp_id,
                    pt_cmd_sets,
                    pf_strm_nfy,
                    pv_strm_nfy_tag,
                    b_auto_play,
                    ph_stream
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (ph_stream == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_open_stream == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_open_stream(
                            pt_svctx,
                            e_pipe_type,
                            pt_strm_comp_id,
                            pt_cmd_sets,
                            pf_strm_nfy,
                            pv_strm_nfy_tag,
                            b_auto_play,
                            ph_stream
                            );
    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_open_stream_ex
 *
 * Description: This function is used to open a stream in a specific svctx.
 *              Basically, it is a help function to let caller to handle
 *              the stream by itself, but the return value  is SVCTXR_XXX
 *              not SMR_XXX
 * Inputs:  h_svctx         References to the svctx.
 *          e_pipe_type     Additional info for specifing pipe. it could be ST_VIDEO,
 *                          ST_AUDIO, or ST_UNKNOWN
 *          pt_strm_comp_id References to the component ID aquired from SCDB.
 *          pt_cmd          References to the command sets to initialize the
 *                          stream.
 *          pf_strm_nfy     References to a stream event callback function,
 *                          which is implemented by the caller.
 *          pv_strm_nfy_tag References to a tag to be pass to the stream event
 *                          callback function.
 *          b_auto_play     Indicate if the stream sould play automatically.
 *          ui4_serial_num  Contain a serial number indicating the current
 *                          open stream operation.
 *
 * Outputs: ph_stream       References to the stream handle created by Stream
 *                          Manager.
 *
 * Returns: SVCTXR_OK          (SMR_OK)             Success.
 *          SVCTXR_NOT_SUPPORT (SMR_NOT_SUPPORT)    There is no stream handler
 *                                                  that support this
 *                                                  type of stream.
 *          SVCTXR_INV_ARG     (SMR_INV_ARG)        The argument ph_stream,
 *                                                  pf_strm_nfy,or
 *                                                  pt_strm_comp_id is invalid.
 *          SVCTXR_INV_HANDLE                       The h_svctx is invalid.
 *          SVCTXR_WRONG_STATE                      session is not available
 *          SVCTXR_FAIL                             failed
  *---------------------------------------------------------------------------*/
INT32 x_svctx_open_stream_ex(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_pipe_type,
                    const STREAM_COMP_ID_T*         pt_strm_comp_id,
                    const SM_COMMAND_T*             pt_cmd_sets,
                    x_stream_nfy_fct                pf_strm_nfy,
                    VOID*                           pv_strm_nfy_tag,
                    BOOL                            b_auto_play,
                    UINT32                          ui4_serial_num,
                    HANDLE_T*                       ph_stream
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "e_pipe_type = %d, "
                    "pt_strm_comp_id = 0x%x, "
                    "pt_cmd_sets = 0x%x, "
                    "pf_strm_nfy = 0x%x"
                    "pv_strm_nfy_tag = 0x%x"
                    "b_auto_play = %d"
                    "ph_stream = 0x%x"
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_pipe_type,
                    pt_strm_comp_id,
                    pt_cmd_sets,
                    pf_strm_nfy,
                    pv_strm_nfy_tag,
                    b_auto_play,
                    ph_stream
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (ph_stream == NULL) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_open_stream == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        if (e_pipe_type == ST_TELETEXT) {
            if (pt_svctx->pt_running_svc_req->ui4_serial_num != ui4_serial_num) {
                i4_ret = SVCTXR_WRONG_STATE;
                break;
            }
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_open_stream(
                            pt_svctx,
                            e_pipe_type,
                            pt_strm_comp_id,
                            pt_cmd_sets,
                            pf_strm_nfy,
                            pv_strm_nfy_tag,
                            b_auto_play,
                            ph_stream
                            );
    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_close_stream
 *
 * Description: This function is used to close a stream of Stream Manager.
 *              Basically, it is a help function to let caller to handle
 *              the stream by itself, but the return value  is SVCTXR_XXX
 *              not SMR_XXX
 * Inputs:  h_stream References to the handle of the stream.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          (SMR_OK)             Success.
 *          SVCTXR_INV_HANDLE  (SMR_INV_HANDLE)     The h_stream is invalid.
 *          SVCTXR_FAIL        (SMR_INTERNAL_ERROR) A serious internal error.
 ----------------------------------------------------------------------------*/
INT32 x_svctx_close_stream(
                    HANDLE_T                        h_stream
                    )
{
    INT32 i4_ret;

    i4_ret = x_sm_close_stream(h_stream);

    return svctx_translate_smr_2_svctxr(i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_get_conn_attr
 * Description This API gets information about a connection.
 * Inputs:  h_svctx        Specifies the svctx handle that was
 *                         used to create the SCDB object.
 *          e_pipe_type    Additional info for specifing pipe. it could be ST_VIDEO,
 *                         ST_AUDIO, or ST_UNKNOWN
 *          e_ctrl         Contains the control code, and it is defined in CM's header file
 * Outputs: pv_ctrl_data   Contains information about the connection.
 * Returns: SVCTXR_OK            successfully
 *          SVCTXR_NO_RESOURCE   No more handle available,
 *                               or Out of memory.
 *          SVCTXR_INV_HANDLE    bad 'h_svctx' handle.
 *          SVCTXR_WRONG_STATE   connection is not available
 *          SVCTXR_FAIL          failed
 *---------------------------------------------------------------------------*/
INT32 x_svctx_get_conn_attr(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_pipe_type,
                    CM_CTRL_TYPE_T                  e_ctrl,
                    VOID*                           pv_ctrl_data
                    )
{
    SVCTX_T* pt_svctx;
    INT32    i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "e_pipe_type = %d, "
                    "e_ctrl = %d, "
                    "pv_get_data = 0x%x, "
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_pipe_type,
                    e_ctrl,
                    pv_ctrl_data
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_conn_attr == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_get_conn_attr(
                    pt_svctx,
                    e_pipe_type,
                    e_ctrl,
                    pv_ctrl_data
                    );
    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_set_conn_attr
 * Description This API sets some attributes of a connection.
 * Inputs:  h_svctx        Specifies the svctx handle that was
 *                         used to create the SCDB object.
 *          e_pipe_type    Additional info for specifing pipe. it could be ST_VIDEO,
 *                         ST_AUDIO, or ST_UNKNOWN
 *          e_ctrl         Contains the control code, and it is defined in CM's header file
 *          pv_ctrl_data   References a data structure which content depends on
 *                         e_ctrl.
 *
 * Outputs: pv_ctrl_data   Contains values that may have been modified.
 * Returns: SVCTXR_OK            successfully
 *          SVCTXR_NO_RESOURCE   No more handle available,
 *                               or Out of memory.
 *          SVCTXR_INV_HANDLE    bad 'h_svctx' handle.
 *          SVCTXR_WRONG_STATE   connection is not available
 *          SVCTXR_FAIL          failed
 *---------------------------------------------------------------------------*/
INT32 x_svctx_set_conn_attr(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_pipe_type,
                    CM_CTRL_TYPE_T                  e_ctrl,
                    VOID*                           pv_ctrl_data
                    )
{
    SVCTX_T*                pt_svctx;
    INT32                   i4_ret = SVCTXR_OK;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    INT32                   i, i4_num;
    ACTIVE_LINK_ENTRY_T*    pt_link;
#endif

    DBG_API((_API_HEADER"%s("
                    "h_svctx = %d, "
                    "e_pipe_type = %d, "
                    "e_ctrl = %d, "
                    "pv_get_data = 0x%x, "
                    ")\r\n",
                    __func__,
                    h_svctx,
                    e_pipe_type,
                    e_ctrl,
                    pv_ctrl_data
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->pt_running_svc_req == NULL) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr == NULL) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_set_conn_attr == NULL) {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }

#ifdef MW_TV_AV_BYPASS_SUPPORT
        i = 0;
        i4_num = svctx_mgr_get_link_num(
            ACTIVE_LINK_NOW,
            pt_svctx->pt_running_svc_req->e_src_type);

        while (i < i4_num)
        {
            pt_link = svctx_mgr_get_link_obj(
                ACTIVE_LINK_NOW,
                pt_svctx->pt_running_svc_req->e_src_type,
                i);

            if (pt_link &&
                pt_link->pt_worker &&
                pt_link->pt_worker->pt_running_svc_req &&
                pt_link->pt_worker->pt_running_svc_req->pt_hdlr &&
                pt_link->pt_worker->pt_running_svc_req->pt_hdlr->t_fcts.pf_set_conn_attr)
            {
                i4_ret = pt_link->pt_worker->pt_running_svc_req->pt_hdlr->t_fcts.pf_set_conn_attr(
                    pt_link->pt_worker,
                    e_pipe_type,
                    e_ctrl,
                    pv_ctrl_data);

                if (SVCTXR_OK != i4_ret && SVCTXR_ASYNC_NFY != i4_ret)
                {
                    DBG_ERROR((
                        _ERROR_HEADER"set conn attr(%s) failed, i4_ret=%d\r\n",
                        pt_link->pt_worker->s_name,
                        i4_ret));

                    break;
                }
            }

            i++;
        }
#else
        i4_ret = pt_svctx->pt_running_svc_req->pt_hdlr->t_fcts.pf_set_conn_attr(
                    pt_svctx,
                    e_pipe_type,
                    e_ctrl,
                    pv_ctrl_data
                    );
#endif
    } while (0);
    svctx_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_set_analog_tuner_out_attr
 * Description This API sets analog tuner out attributes
 * Inputs:  pt_tuner_out   References a data structure which content tuner out
 *                         parameters.
 *
 * Outputs: -
 * Returns: SVCTXR_OK            successfully
 *          SVCTXR_INV_ARG       pt_tuner_out is NULL.
 *          SVCTXR_FAIL          failed
 *---------------------------------------------------------------------------*/
INT32 x_svctx_set_analog_tuner_out_attr(
                    TUNER_OUT_DESC_T*               pt_tuner_out
                    )
{
#ifdef MW_TUNER_OUT_SUPPORT
    if (pt_tuner_out == NULL || pt_tuner_out->e_type >= TUNER_OUT_TYPE_LAST_VALID_ENTRY) {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();
    t_g_tuner_out = *pt_tuner_out;
    svctx_class_unlock();
#endif
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_svc_block_check
 * Description
 *      This API is used to perfrom service block check, however, this
 *      API is just a check, it doesn't perfrom anu service block/unblock
 *      operation.
 * Inputs:
 *      h_svctx           Contains a handle to a service context.
 *      e_check_guide     Contains the block check guide
 *      ps_src_name       Contains the source group name
 *      pt_svl_rec        Contains the SVL record if source is TV
 *      pv_evn_rating     Contains the event's rating buffer obtained from EVctx
 * Outputs: -
 * Returns:
 *      SVC_BLOCK_COND_T        the check result
 *---------------------------------------------------------------------------*/
SVC_BLOCK_COND_T x_svctx_svc_block_check(
                    HANDLE_T                        h_svctx,
                    SVC_BLOCK_CHECK_GUDIE_T         e_check_guide,
                    const CHAR*                     ps_src_name,
                    const SVL_REC_T*                pt_svl_rec,
                    const VOID*                     pv_evn_rating
                    )
{
    SVCTX_T*                    pt_svctx           = NULL;
#ifdef SVCTX_SPLIT_SUPPORT
    svctx_svc_block_check_fct   pf_block_check_fct = NULL;
#else
    x_svctx_svc_block_check_fct pf_block_check_fct = NULL;
#endif
    SVC_BLOCK_COND_T            e_svc_block_cond   = SVC_BLOCK_COND_PASSED;
    INT32                       i4_ret             = SVCTXR_OK;

    CLI_FUNC_CALL_TRACE(__func__);

    if (b_g_init == FALSE) {
        return e_svc_block_cond;
    }

    svctx_class_lock();
    do {
        i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        pf_block_check_fct = pt_svctx->pf_block_check_fct;
    } while (0);
    svctx_class_unlock();

    if (pf_block_check_fct != NULL) {
#ifdef SVCTX_SPLIT_SUPPORT
        e_svc_block_cond = pf_block_check_fct(
                    e_check_guide,
                    ps_src_name,
                    pt_svl_rec,
                    pv_evn_rating,
                    pt_svctx->pf_block_check_fct_tag
                    );
#else
        e_svc_block_cond = pf_block_check_fct(
                    e_check_guide,
                    ps_src_name,
                    pt_svl_rec,
                    pv_evn_rating
                    );
#endif
    }

    return e_svc_block_cond;
}

#ifdef MW_TV_AV_BYPASS_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: x_svctx_create_associate
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *---------------------------------------------------------------------------*/
INT32 x_svctx_create_associate(
    const CHAR*                     ps_name,
    const SVCTX_ASSC_CFG_T*         pt_assc_cfg,
    HANDLE_T*                       ph_svctx)
{
    INT32       i4_ret;
    SIZE_T      z_size;
    SVCTX_T*    pt_svctx_assc;
    SVCTX_T*    pt_svctx_lead;

    if (b_g_init == FALSE)
    {
        return SVCTXR_NOT_INIT;
    }

    z_size = x_strlen(ps_name);
    if (z_size == 0 || z_size > SVCTX_NAME_MAX_SIZE || !pt_assc_cfg || !ph_svctx)
    {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();

    i4_ret = SVCTXR_INV_ARG;

    if (SVCTXR_OK == svctx_get_obj(pt_assc_cfg->h_svctx_lead, &pt_svctx_lead))
    {
        if (FALSE == svctx_find_instance(ps_name, &pt_svctx_assc))
        {
            i4_ret = _svctx_create(
                ps_name,
                NULL,
                NULL,
                &pt_svctx_assc);

            if (SVCTXR_OK == i4_ret)
            {
                i4_ret = handle_alloc(
                    SVCTXT_SVCTX_ASSOCIATE,
                    (VOID*) pt_svctx_assc,
                    NULL,
                    _svctx_handle_free_fct,
                    ph_svctx);

                if (SVCTXR_OK == i4_ret)
                {
                    i4_ret = _svctx_config_associate(
                        pt_svctx_lead,
                        pt_svctx_assc,
                        pt_assc_cfg);

                    if (SVCTXR_OK == i4_ret)
                    {
                        i4_ret = handle_link(
                            &(pt_svctx_assc->t_hdl_link),
                            *ph_svctx);

                        if (HR_OK == i4_ret) {
                            /* make a new entry in the association map */
                            i4_ret = svctx_mgr_add_association(
                                pt_svctx_lead,
                                pt_svctx_assc,
                                pt_assc_cfg);

                            if (SVCTXR_OK != i4_ret)
                            {
                                handle_free(*ph_svctx, TRUE);
                                *ph_svctx = NULL_HANDLE;
                            }
                        }
                        else
                        {
                            DBG_ERROR((_ERROR_HEADER"handle_link(%s) failed, i4_ret=%d\r\n", ps_name, i4_ret));
                            i4_ret = SVCTXR_FAIL;
                            handle_free(*ph_svctx, TRUE);
                            *ph_svctx = NULL_HANDLE;
                        }
                    }
                    else
                    {
                        handle_free(*ph_svctx, TRUE);
                        *ph_svctx = NULL_HANDLE;
                    }
                }
                else
                {
                    svctx_free_object(pt_svctx_assc);
                }
            }
        }
    }

    svctx_class_unlock();

    if (SVCTXR_OK != i4_ret)
    {
        DBG_ERROR((_ERROR_HEADER"_svctx_create(%s) failed, i4_ret = %d\r\n", ps_name, i4_ret));
    }

    return (i4_ret);
}

#ifdef MW_PIP_POP_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: x_svctx_add_associate
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *---------------------------------------------------------------------------*/
INT32 x_svctx_add_associate(
    HANDLE_T    h_svctx,
    HANDLE_T    h_svctx_assc)
{
    INT32           i4_ret;
    SVCTX_T*        pt_svctx;
    SVCTX_T*        pt_svctx_assc;

    if (b_g_init == FALSE)
    {
        return SVCTXR_NOT_INIT;
    }
    svctx_class_lock();

    i4_ret = SVCTXR_INV_ARG;

    if (SVCTXR_OK == svctx_get_obj(h_svctx, &pt_svctx) &&
        !IS_SVCTX_ASSOCIATE(pt_svctx))
    {
        if (SVCTXR_OK == svctx_get_obj_with_ext_type(h_svctx_assc, &pt_svctx_assc) &&
            IS_SVCTX_ASSOCIATE(pt_svctx_assc))
        {
            /* make a new entry in the association map */
            i4_ret = svctx_mgr_add_association(
                pt_svctx,
                pt_svctx_assc,
                svctx_mgr_get_assc_cfg(pt_svctx_assc));
        }
    }

    svctx_class_unlock();

    if (SVCTXR_OK != i4_ret)
    {
        DBG_ERROR((_ERROR_HEADER"%s failed, i4_ret = %d\r\n", __FUNCTION__, i4_ret));
    }

    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: x_svctx_add_exclusion
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *---------------------------------------------------------------------------*/
INT32 x_svctx_add_exclusion(
    HANDLE_T    h_svctx,
    HANDLE_T    h_svctx_excl,
    UINT32      ui4_excl_src_mask)
{
    INT32       i4_ret = SVCTXR_OK;
    SVCTX_T*    pt_svctx;
    SVCTX_T*    pt_svctx_excl;

    if (b_g_init == FALSE)
    {
        return SVCTXR_NOT_INIT;
    }

    if (0 == ui4_excl_src_mask)
    {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();

    i4_ret = SVCTXR_INV_ARG;

    if (SVCTXR_OK == svctx_get_obj(h_svctx, &pt_svctx) &&
        !IS_SVCTX_ASSOCIATE(pt_svctx))
    {
        if (SVCTXR_OK == svctx_get_obj_with_ext_type(h_svctx_excl, &pt_svctx_excl) &&
            IS_SVCTX_ASSOCIATE(pt_svctx_excl))
        {
            i4_ret = svctx_mgr_add_exclusion(pt_svctx, pt_svctx_excl, ui4_excl_src_mask);
        }
    }

    svctx_class_unlock();

    if (SVCTXR_OK != i4_ret)
    {
        DBG_ERROR((_ERROR_HEADER"%s failed, i4_ret = %d\r\n", __FUNCTION__, i4_ret));
    }

    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: x_svctx_start_exclusion
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *---------------------------------------------------------------------------*/
INT32 x_svctx_start_exclusion(
    HANDLE_T        h_svctx,
    SRC_TYPE_T      e_src_type)
{
    INT32           i4_ret        = SVCTXR_OK;
    SVCTX_T*        pt_svctx      = NULL;
    LINK_POS_T      t_pos         = NULL;
    SVCTX_T*        pt_svctx_excl = NULL;


    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    /* Construct the service select request object */
    svctx_class_lock();

    do{
        i4_ret = svctx_get_obj_with_ext_type(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (svctx_is_owner(pt_svctx, h_svctx) == FALSE) {
            i4_ret = SVCTXR_NO_RIGHT;
            break;
        }

        /* For DTV00338967: Clean the nodes in the NEXT link. */
        {
            INT32                 i4_idx    = 0;
            ACTIVE_LINK_ENTRY_T   *pt_entry = NULL;

            do {
                pt_entry = svctx_mgr_get_link_obj_all(ACTIVE_LINK_NEXT, i4_idx++);
            }while(pt_entry);

            for (i4_idx -= 2; i4_idx >= 0; i4_idx --)
            {
                pt_entry = svctx_mgr_get_link_obj_all(ACTIVE_LINK_NEXT, i4_idx);
                if (!pt_entry) {
                    break;
                }
                svctx_class_unlock();
                x_svctx_sync_stop_svc(pt_entry->pt_worker->h_sys_svctx, 0);
                svctx_class_lock();
            }
        }

        while (1)
        {
            pt_svctx_excl = svctx_mgr_get_excl_obj_by_lead(pt_svctx, e_src_type, &t_pos);

            if (pt_svctx_excl) 
            {
                if (pt_svctx_excl->pt_running_svc_req)
                {
                    SVCTX_T*    pt_lead;

                    pt_lead = svctx_mgr_get_link_lead(ACTIVE_LINK_NOW, pt_svctx_excl->pt_running_svc_req->e_src_type);
                    if (pt_lead)
                    {
                        svctx_class_unlock();
                        x_svctx_sync_stop_svc(pt_lead->h_sys_svctx, 0);
                        svctx_class_lock();
                    }
                    else
                    {
                        svctx_class_unlock();
                        x_svctx_sync_stop_svc(pt_svctx_excl->h_sys_svctx, 0);
                        svctx_class_lock();
                    }
                }
 
                pt_svctx_excl->ui4_assc_state |= SVCTX_ASSC_EXCLUDED;
            }
            else
            {
                break;
            }
        }
    } while (0);

    svctx_class_unlock();

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_svctx_stop_exclusion
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *---------------------------------------------------------------------------*/
INT32 x_svctx_stop_exclusion(
    HANDLE_T        h_svctx,
    SRC_TYPE_T      e_src_type)
{
    INT32                   i4_ret      = SVCTXR_OK;
    SVCTX_T*                pt_svctx    = NULL;
    LINK_POS_T              t_excl_pos = NULL;
    INT32                   i = 0;
    ACTIVE_LINK_ENTRY_T*    pt_link;
    SVCTX_T*                pt_svctx_excl;
    SCC_VID_MODE_EX_T       t_vid_mode = {SCC_VID_BLANK, {0}};
    STRM_REQ_T*             pt_strm_req = NULL;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    /* Construct the service select request object */
    svctx_class_lock();
    do{
        i4_ret = svctx_get_obj_with_ext_type(h_svctx, &pt_svctx);
        if (i4_ret != SVCTXR_OK) {
                break;
            }

        if (!svctx_is_pap(pt_svctx))
        {
            break;
        }

        if (svctx_is_owner(pt_svctx, h_svctx) == FALSE) {
            i4_ret = SVCTXR_NO_RIGHT;
                break;
            }

        /* set sub region to 0, right now */
        x_scc_vid_set_mode_ex(pt_svctx->h_video_scc_comp, &t_vid_mode);

        if ((pt_svctx->ui4_prv_stream_cond & MAKE_BIT_MASK_32(ST_AUDIO)) != 0)
        {
            pt_strm_req = &pt_svctx->t_prv_aud_strm_req;
        }
        
        if (pt_strm_req != NULL)
        {
            while (1)
            {
                pt_link = svctx_mgr_get_link_obj_all(ACTIVE_LINK_NOW, i++);
            
                if (!pt_link)
                {
                    break;
                }

                if (pt_link->pt_worker != pt_svctx)
                {
                    pt_link->pt_worker->t_prv_aud_strm_req = *pt_strm_req;
                    pt_link->pt_worker->ui4_prv_stream_cond |= MAKE_BIT_MASK_32(ST_AUDIO);
                }
            }
            i = 0;
        }

        while (1)
        {
            pt_link = svctx_mgr_get_link_obj_all(ACTIVE_LINK_NOW, i);
    
            if (!pt_link)
            {
                break;
            }

            if (!IS_SVCTX_ASSOCIATE(pt_link->pt_worker))
            {
                if (pt_strm_req == NULL && pt_link->pt_worker != pt_svctx)
                {
                    if ((pt_link->pt_worker->ui4_prv_stream_cond & MAKE_BIT_MASK_32(ST_AUDIO)) != 0)
                    {
                        pt_strm_req = &pt_link->pt_worker->t_prv_aud_strm_req;
                    }
                }
                svctx_class_unlock();
                x_svctx_sync_stop_svc(pt_link->pt_worker->h_sys_svctx, 0);
                svctx_class_lock();  
                i = 0;              
            }
            else
            {
                i++;
            }
        }

        while (1)
        {
            pt_svctx_excl = svctx_mgr_get_excl_obj_by_lead(pt_svctx, e_src_type, &t_excl_pos);

            if (pt_svctx_excl)
            {
                pt_svctx_excl->e_prv_scrn_status = SVCTX_SCREEN_STATUS_UNKNOWN;
                pt_svctx_excl->ui4_assc_state &= ~SVCTX_ASSC_EXCLUDED;

                if (pt_strm_req != NULL)
                {
                    pt_svctx_excl->t_prv_aud_strm_req = *pt_strm_req;
                    pt_svctx_excl->ui4_prv_stream_cond |= MAKE_BIT_MASK_32(ST_AUDIO);
                }
                
            }
            else
            {
                break;
            }
        }
    } while (0);

    if (pt_strm_req != NULL)
    {
        pt_svctx->ui4_prv_stream_cond = 0;
        x_memset(&pt_svctx->t_prv_aud_strm_req, 0, sizeof(STRM_REQ_T));
        pt_strm_req = NULL;
    }

    svctx_class_unlock();

    return SVCTXR_OK;
}

#endif
#endif

#ifdef SYS_RECORD_SUPPORT
 /*-----------------------------------------------------------------------------
 * Name: x_svctx_rec_create
 *
 * Description: This API creates a Recrod SVCTX.
 *
 * Inputs:  ps_svctx_name  Specifies the service context name for the create.
 *                         The name must be unique to other SVCTX.
 *
 *          pt_rec_attr    Contains the attributes for Record SVCTX.
 *
 * Outputs: ph_svctx_rec   Contains the Record SVCTX handle.
 *
 * Returns: SVCTXR_OK           Routine successful.
 *          SVCTXR_INV_ARG      Invalid argument.
 *          SVCTXR_NO_RESOURCE  Not enough memory to open the service context.
 *          SVCTXR_NOT_INIT     The Service Context Module is not initialized yet.
 *          SVCTXR_FAIL         The creation is fail.
 *          < 0                 Other errors.
 ----------------------------------------------------------------------------*/
INT32 x_svctx_rec_create (
    const CHAR*                 ps_svctx_name,
    const SVCTX_REC_ATTR_T*     pt_rec_attr,
    HANDLE_T*                   ph_svctx_rec)
{
    SVCTX_T*        pt_svctx = NULL;
    BOOL            b_new_object, b_new_handle;
    SIZE_T          z_size;
    INT32           i4_ret;

    if (ph_svctx_rec == NULL)
    {
        return SVCTXR_INV_ARG;
    }

    *ph_svctx_rec = NULL_HANDLE;

    if (b_g_init == FALSE)
    {
        return SVCTXR_NOT_INIT;
    }

    z_size = x_strlen (ps_svctx_name);

    if (z_size == 0 || z_size > SVCTX_NAME_MAX_SIZE)
    {
        return SVCTXR_INV_ARG;
    }

    svctx_class_lock();

    b_new_object = FALSE;
    b_new_handle = FALSE;
    i4_ret       = SVCTXR_OK;

    do
    {
        if (svctx_find_instance (ps_svctx_name, & pt_svctx))
        {
            /* duplicated name */
            DBG_ERROR((_ERROR_HEADER "Cannot create REC-SVCTX with duplicated name(%s), i4_ret=%d\n", ps_svctx_name, i4_ret));
            i4_ret = SVCTXR_INV_ARG;
            break;
        }

        i4_ret = _svctx_create (ps_svctx_name,
                                NULL,
                                NULL,
                                & pt_svctx );

        if (i4_ret != SVCTXR_OK)
        {
            DBG_ERROR((_ERROR_HEADER "_svctx_create(%s) failed, i4_ret=%d\n", ps_svctx_name, i4_ret));
            break;
        }

        b_new_object = TRUE;

        #ifdef MW_TV_AV_BYPASS_SUPPORT
        {
            SVCTX_ASSC_CFG_T    t_assc_cfg;
            SVCTX_T*            pt_svctx_lead;

            i4_ret = svctx_get_obj (pt_rec_attr->h_svctx_lead,
                                    & pt_svctx_lead);
            if (i4_ret != SVCTXR_OK)
            {
                DBG_ERROR((_ERROR_HEADER "Fail to get lead svctx (%d), L%d\r\n", i4_ret, __LINE__));
                break;
            }

            x_memset (& t_assc_cfg, 0, sizeof (SVCTX_ASSC_CFG_T));
            t_assc_cfg.h_svctx_lead           = pt_rec_attr->h_svctx_lead;
            x_strcpy (t_assc_cfg.s_snk_grp_name, SN_PRES_RECORD_TV);
            t_assc_cfg.pf_select_svc_nfy      = NULL;
            t_assc_cfg.pv_select_svc_nfy_tag  = NULL;
            t_assc_cfg.pv_video_strm_fltr_tag = NULL;
            t_assc_cfg.pv_audio_strm_fltr_tag = NULL;
            t_assc_cfg.pv_cc_strm_fltr_tag    = NULL;
            t_assc_cfg.pv_sbtl_strm_fltr_tag  = NULL;
            t_assc_cfg.pv_ttx_strm_fltr_tag   = NULL;
            t_assc_cfg.ui4_stream_mode        = 0;

            i4_ret = SVCTXR_OK;
            switch (pt_rec_attr->e_rec_src_type)
            {
            case SVCTX_REC_SRC_TYPE_TV:
                t_assc_cfg.ui4_assc_src_mask = SVCTX_SVC_SELECT_FLAG_TV;
                break;

            case SVCTX_REC_SRC_TYPE_AVC:
                t_assc_cfg.ui4_assc_src_mask = SVCTX_SVC_SELECT_FLAG_AV;
                break;

            default:
                DBG_ERROR((_ERROR_HEADER "Invalid record source type [%d], L%d\r\n",
                          pt_rec_attr->e_rec_src_type,
                          __LINE__));
                i4_ret = SVCTXR_INV_ARG;
                break;
            }

            if (i4_ret != SVCTXR_OK)
            {
                break;
            }

            t_assc_cfg.ui4_attribute = SVCTX_ASSC_ATTR_ASSOCIATE;
            t_assc_cfg.i1_prio_diff  = pt_rec_attr->i1_prio_diff;

            i4_ret = svctx_mgr_add_association(pt_svctx_lead,
                                               pt_svctx,
                                               & t_assc_cfg);
            if (i4_ret != SVCTXR_OK)
            {
                DBG_ERROR((_ERROR_HEADER "Fail to add association (%d), L%d\r\n", i4_ret, __LINE__));
                break;
            }
        }
        #endif

        i4_ret = handle_alloc (SVCTXT_SVCTX,
                               (VOID*) pt_svctx,
                               NULL,
                               _svctx_handle_free_fct,
                               ph_svctx_rec );

        if ( i4_ret != HR_OK )
        {
            DBG_ERROR ((_ERROR_HEADER "handle_alloc(%s) failed, i4_ret=%d\r\n", ps_svctx_name, i4_ret));
            i4_ret = SVCTXR_NO_RESOURCE;
            break;
        }

        b_new_handle = TRUE;

        i4_ret = handle_link (&pt_svctx->t_hdl_link,
                              *ph_svctx_rec );

        if (i4_ret != HR_OK)
        {
            DBG_ERROR((_ERROR_HEADER "handle_link(%s) failed, i4_ret=%d\r\n", ps_svctx_name, i4_ret));
            i4_ret = SVCTXR_FAIL;
            break;
        }

        pt_svctx->i1_prio_diff = pt_rec_attr->i1_prio_diff;
        pt_svctx->b_rec_svctx  = TRUE;
#ifdef MW_TV_AV_BYPASS_SUPPORT
        pt_svctx->ui4_attribute |= SVCTX_ASSC_ATTR_ASSOCIATE;
#endif

        i4_ret = SVCTXR_OK;

    } while (FALSE);

    if (i4_ret != SVCTXR_OK)
    {
        if (b_new_handle)
        {
            i4_ret = x_handle_free (*ph_svctx_rec);

            if (i4_ret != HR_OK)
            {
                DBG_ERROR((_ERROR_HEADER "Fail to free handle, i4_ret=%d\n", i4_ret));
            }

            *ph_svctx_rec = NULL_HANDLE;
        }
        else if (b_new_object)
        {
            svctx_free_object (pt_svctx);
        }
    }

    svctx_class_unlock();

    return i4_ret;
}

 /*-----------------------------------------------------------------------------
 * Name: x_svctx_rec_start
 *
 * Description: This API starts the recording session.
 *
 * Inputs:  h_svctx_rec  Specifies the Record SVCTX handle.
 *
 *          pt_config    Contains the config for recording.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK           Routine successful.
 *          SVCTXR_NOT_INIT     The Service Context Module is not initialized yet.
 *          SVCTXR_INV_ARG      Invalid argument.
 *          SVCTXR_INV_HANDLE   Invalid service context handle.
 *          SVCTXR_WRONG_STATE  Invalid service context state to start recording.
 *          SVCTXR_NO_RESOURCE  Not enough memory / resource.
 *          < 0                 Other errors.
 ----------------------------------------------------------------------------*/
INT32 x_svctx_rec_start (
    HANDLE_T                    h_svctx_rec,
    const SVCTX_REC_CONFIG_T*   pt_config )
{
    INT32               i4_ret;

    i4_ret = x_svctx_set (h_svctx_rec,
                          SVCTX_REC_SET_TYPE_START_REC,
                          (VOID *) pt_config,
                          sizeof (SVCTX_REC_CONFIG_T));

    if (i4_ret != SVCTXR_OK)
    {
        DBG_ERROR((_ERROR_HEADER "Fail to start record, i4_ret = %d (L%d) \r\n", i4_ret, __LINE__));
    }

    return i4_ret;
}

 /*-----------------------------------------------------------------------------
 * Name: x_svctx_rec_stop
 *
 * Description: This API stops the recording session. The
 *
 * Inputs:  h_svctx_rec  Specifies the Record SVCTX handle.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK           Routine successful.
 *          SVCTXR_NOT_INIT     The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE   Invalid service context handle.
 *          SVCTXR_WRONG_STATE  Invalid service context state to stop recording.
 *          < 0                 Other errors.
 ----------------------------------------------------------------------------*/
INT32 x_svctx_rec_stop (
    HANDLE_T                    h_svctx_rec)
{
    INT32               i4_ret;

    i4_ret = x_svctx_set (h_svctx_rec,
                          SVCTX_REC_SET_TYPE_STOP_REC,
                          NULL,
                          0);

    if (i4_ret != SVCTXR_OK)
    {
        DBG_ERROR((_ERROR_HEADER "Fail to stop record, i4_ret = %d (L%d) \r\n", i4_ret, __LINE__));
    }

    return i4_ret;
}

 /*-----------------------------------------------------------------------------
 * Name: x_svctx_rec_pause
 *
 * Description: This API pauses the recording session.
 *
 * Inputs:  h_svctx_rec  Specifies the Record SVCTX handle.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK           Routine successful.
 *          SVCTXR_NOT_INIT     The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE   Invalid service context handle.
 *          SVCTXR_WRONG_STATE  Invalid service context state to pause recording.
 *          < 0                 Other errors.
 ----------------------------------------------------------------------------*/
INT32 x_svctx_rec_pause (
    HANDLE_T                    h_svctx_rec)
{
    INT32               i4_ret;

    i4_ret = x_svctx_set (h_svctx_rec,
                          SVCTX_REC_SET_TYPE_PAUSE_REC,
                          NULL,
                          0);

    if (i4_ret != SVCTXR_OK)
    {
        DBG_ERROR((_ERROR_HEADER "Fail to pause record, i4_ret = %d (L%d) \r\n", i4_ret, __LINE__));
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_svctx_rec_resume
 *
 * Description: This API resumes the recording session.
 *
 * Inputs:  h_svctx_rec  Specifies the Record SVCTX handle.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK           Routine successful.
 *          SVCTXR_NOT_INIT     The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE   Invalid service context handle.
 *          SVCTXR_WRONG_STATE  Invalid service context state to pause recording.
 *          < 0                 Other errors.
 ----------------------------------------------------------------------------*/
INT32 x_svctx_rec_resume (
    HANDLE_T                    h_svctx_rec)
{
    INT32               i4_ret;

    i4_ret = x_svctx_set (h_svctx_rec,
                          SVCTX_REC_SET_TYPE_RESUME_REC,
                          NULL,
                          0);

    if (i4_ret != SVCTXR_OK)
    {
        DBG_ERROR((_ERROR_HEADER "Fail to resume record, i4_ret = %d (L%d) \r\n", i4_ret, __LINE__));
    }

    return i4_ret;
}
#endif

#ifdef GINGA_SUPPORT
/*------------------------------------------------------------------*/
/*! @brief  This API to register the pre-select stream.
 *  Inputs: pf_pre_select_stream_fct - Specifies pre-select stream function.
 *  Returns:
 *          SVCTXR_OK - Routine successful.
 *          SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *          SVCTXR_INV_ARG  - pf_pre_select_stream_fct is NULL. 
 ------------------------------------------------------------------*/
INT32 x_svctx_reg_pre_select_stream_cb(
                   x_svctx_pre_select_stream_nfy_fct pf_pre_select_stream_fct /* in*/                   
                   )
{

    DBG_API((_API_HEADER"%s("
             "pre_select_stream_cb = 0x%x, "
             ")\r\n",
             __func__,
             pf_pre_select_stream_fct
            ));

    if(!svctx_is_init())
    {
        return SVCTXR_NOT_INIT;
    }

    if(NULL == pf_pre_select_stream_fct)
    {
        return SVCTXR_INV_ARG; 
    }

    g_pf_svctx_pre_select_stream_nfy_fct = pf_pre_select_stream_fct;

    return SVCTXR_OK;

}
 
 /*-----------------------------------------------------------------------------
 * Name
 *      svctx_pre_select_stream_cb
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
 INT32 svctx_pre_select_stream_cb(HANDLE_T         h_svctx,
                                  STREAM_TYPE_T    e_stream_type)
{
    DBG_API((_API_HEADER"%s("
             "h_svctx = %d, "
             "e_stream_type = %d, "
             ")\r\n",
             __func__,                    
             h_svctx,
             e_stream_type
            ));

    if(!svctx_is_init())
    {
        return SVCTXR_NOT_INIT;
    }

    if(NULL == g_pf_svctx_pre_select_stream_nfy_fct)
    {
        return SVCTXR_INV_ARG; 
    }

    g_pf_svctx_pre_select_stream_nfy_fct(h_svctx, e_stream_type);

    return SVCTXR_OK;
} 

#endif

