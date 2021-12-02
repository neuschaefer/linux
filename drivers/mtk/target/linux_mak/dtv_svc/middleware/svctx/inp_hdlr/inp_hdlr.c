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
 * $RCSfile: inp_hdlr.c,v $
 * $Revision: #3 $
 * $Date: 2012/11/06 $
 * $Author: xingju.wang $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/26 $
 * $SWAuthor: Weider Chang $
 * $MD5HEX: 9cacb426bc6af60b7f3e5a1ad05af25e $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "svctx/inp_hdlr/inp_svc_req.h"
#include "svctx/inp_hdlr/u_inp_hdlr.h"
#include "svctx/inp_hdlr/x_inp_hdlr.h"
#include "svctx/inp_hdlr/inp_hdlr.h"
#include "svctx/util/crnt_evn.h"
#include "svctx/svctx_dbg.h"
#include "conn_mngr/avc_handler/u_avc_handler.h"
#include "svctx/svctx_sm_broker.h"
#include "dbg/x_dbg.h"

#ifdef MW_TV_AV_BYPASS_SUPPORT
#include "svctx/svctx_mgr.h"
#include "strm_mngr/bypass_hdlr/u_sm_bypass_event.h"
#endif
/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#undef  _INFO_HEADER
#undef  _API_HEADER
#undef  _ERROR_HEADER
#define _INFO_HEADER        "{SVCTX:INP_HDLR}Info: "
#define _API_HEADER         "{SVCTX:INP_HDLR}API:  "
#define _ERROR_HEADER       "{SVCTX:INP_HDLR}Err:  "

#if 0
#undef  DBG_ERROR
#define DBG_ERROR(_stmt)    x_dbg_stmt _stmt
#endif
#if 0
#undef  DBG_API
#define DBG_API(_stmt)      x_dbg_stmt _stmt
#endif
#if 0
#undef  DBG_INFO
#define DBG_INFO(_stmt)     x_dbg_stmt _stmt
#endif

/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/ 
static INT32 _inp_hdlr_handle_video_player_avm_nty(
                    AV_MONITOR_T*               pt_avm,
                    AVM_NTFY_CODE_T             e_code,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data
                    );
static INT32 _inp_hdlr_handle_audio_player_avm_nty(
                    AV_MONITOR_T*               pt_avm,
                    AVM_NTFY_CODE_T             e_code,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data
                    );
static INT32 _inp_hdlr_handle_audio_player_scdb_updated(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    PLAYER_T*                   pt_audio_player,
                    UINT32                      ui4_reason
                    );
static INT32 _inp_hdlr_handle_video_player_scdb_updated(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    PLAYER_T*                   pt_video_player,
                    UINT32                      ui4_reason
                    );
static BOOL _inp_hdlr_is_blocked_service(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    SVC_BLOCK_CHECK_GUDIE_T     e_check_guide,
                    BOOL                        b_auto_notify_client
                    );
static INT32 _inp_hdlr_block_service (
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    );
/*-----------------------------------------------------------------------------
 * private variable declarations
 *---------------------------------------------------------------------------*/
extern const STREAM_TYPE_T e_g_inp_strm_types[]; /* QQQQQQ */
/*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_close_text_strms
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_close_text_strms(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
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

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_get_video_player_cm_cmds
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_get_video_player_cm_cmds(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    CM_COMMAND_T*               pt_cm_cmds
                    )
{
    INT32 i4_ret = SVCTXR_OK;
    INT32 i4_idx = 0;

    pt_cm_cmds[0].e_code = CONN_END;

    switch (pt_running_svc_req->t_parent.e_src_type) {
    case SRC_TYPE_AVC:
        pt_cm_cmds[i4_idx].e_code         = CONN_SRC_TYPE;
        pt_cm_cmds[i4_idx++].u.ps_name    = AVC_HANDLER_DEFAULT_NAME;
        pt_cm_cmds[i4_idx].e_code         = CONN_SRC_NAME;
        pt_cm_cmds[i4_idx++].u.ps_name    = pt_running_svc_req->t_src_info.s_src_grp_name;
        pt_cm_cmds[i4_idx].e_code         = AVC_CONN_TV_DECODER_NAME;
        pt_cm_cmds[i4_idx++].u.ps_name    = pt_svctx->s_snk_grp_name;
        pt_cm_cmds[i4_idx].e_code         = AVC_CONN_CONNECTOR_TYPE;
        pt_cm_cmds[i4_idx++].u.ui4_number = pt_running_svc_req->t_src_info.t_av_src.e_video_type;
        pt_cm_cmds[i4_idx].e_code         = AVC_CONN_DISC_IF_TV_SIG_LOST;
        pt_cm_cmds[i4_idx++].u.b_boolean  = FALSE;
        if (pt_running_svc_req->t_src_info.t_av_src.e_video_type == DEV_AVC_COMBI) {
            pt_cm_cmds[i4_idx].e_code         = AVC_CONN_COMBI_INDEX;
            pt_cm_cmds[i4_idx++].u.ui2_number = pt_running_svc_req->t_src_info.ui2_crnt_video_combi_index;
        } else {
            pt_running_svc_req->t_src_info.e_crnt_video_type = pt_running_svc_req->t_src_info.t_av_src.e_video_type;
        }

        if (pt_running_svc_req->t_src_info.t_av_src.e_video_type == DEV_AVC_SCART
                && pt_running_svc_req->t_src_info.t_av_src.e_scart_mode != AVC_HANDLER_SCART_MODE_UNKNOWN) {
            pt_cm_cmds[i4_idx].e_code        = AVC_CONN_SCART_INP_MODE;
            pt_cm_cmds[i4_idx++].u.i4_number = (INT32)(pt_running_svc_req->t_src_info.t_av_src.e_scart_mode);
            inp_hdlr_update_scart_crnt_video_type(pt_running_svc_req);
        }
#ifdef MW_TV_AV_BYPASS_SUPPORT
        pt_cm_cmds[i4_idx].e_code        = AVC_CONN_DISC_IF_COMP_BUSY;
        pt_cm_cmds[i4_idx++].u.b_boolean = FALSE;
        pt_cm_cmds[i4_idx].e_code        = AVC_CONN_INPUT_NO_RIGHTS_OK;
        pt_cm_cmds[i4_idx++].u.b_boolean = TRUE;
#endif
        pt_cm_cmds[i4_idx].e_code        = CONN_END;
        break;
    default:
        i4_ret = SVCTXR_FAIL;
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_get_audio_player_cm_cmds
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static  INT32 _inp_hdlr_get_audio_player_cm_cmds(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    CM_COMMAND_T*               pt_cm_cmds
                    )
{
    INT32 i4_ret = SVCTXR_OK;
    INT32 i4_idx = 0;

    pt_cm_cmds[0].e_code = CONN_END;

    switch (pt_running_svc_req->t_parent.e_src_type) {
    case SRC_TYPE_AVC:
        pt_cm_cmds[i4_idx].e_code         = CONN_SRC_TYPE;
        pt_cm_cmds[i4_idx++].u.ps_name    = AVC_HANDLER_DEFAULT_NAME;
        pt_cm_cmds[i4_idx].e_code         = CONN_SRC_NAME;
        pt_cm_cmds[i4_idx++].u.ps_name    = pt_running_svc_req->t_src_info.s_src_grp_name;
        pt_cm_cmds[i4_idx].e_code         = AVC_CONN_CONNECTOR_TYPE;
        pt_cm_cmds[i4_idx++].u.ui4_number = pt_running_svc_req->t_src_info.t_av_src.e_audio_type;
        if (pt_running_svc_req->t_src_info.t_av_src.e_audio_type == DEV_AVC_COMBI) {
            pt_cm_cmds[i4_idx].e_code         = AVC_CONN_COMBI_INDEX;
            pt_cm_cmds[i4_idx++].u.ui2_number = pt_running_svc_req->t_src_info.ui2_crnt_audio_combi_index;
        } else {
            pt_running_svc_req->t_src_info.e_crnt_audio_type = pt_running_svc_req->t_src_info.t_av_src.e_audio_type;
        }
#ifdef MW_TV_AV_BYPASS_SUPPORT
        pt_cm_cmds[i4_idx].e_code        = AVC_CONN_INPUT_NO_RIGHTS_OK;
        pt_cm_cmds[i4_idx++].u.b_boolean = TRUE;
#endif
        pt_cm_cmds[i4_idx].e_code        = CONN_END;
        break;
    default:
        i4_ret = SVCTXR_FAIL;
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_video_player_stopped
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_video_player_stopped(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    INT32 i4_ret;

    if (pt_running_svc_req->b_replace_video == TRUE) {
        pt_running_svc_req->b_replace_video = FALSE;
        i4_ret = player_play(
                    pt_running_svc_req->pt_video_player,
                    pt_running_svc_req->t_parent.i4_speed
                    );
        return i4_ret;
    }

    i4_ret = player_close(pt_running_svc_req->pt_video_player);
    if (i4_ret == SVCTXR_OK) {
        pt_running_svc_req->pt_video_player = NULL;
    }

    if (pt_running_svc_req->pt_audio_player == NULL && pt_svctx->b_in_stopping == TRUE) {
        svctx_ntfy_svc_req_stopped(pt_svctx);
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_strm_comp_filter
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static BOOL _inp_hdlr_strm_comp_filter (
                    UINT16                      ui2_num_recs,
                    UINT16                      ui2_idx,
                    const STREAM_COMP_ID_T*     pt_comp_id,
                    const SCDB_REC_T*           pt_scdb_rec,
                    VOID*                       pv_tag
                    )
{
    return (ui2_idx == 0 ? TRUE : FALSE);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_choose_strm_filter
 * Description
 *      This API is used to select proper stream filter when the client doesn't
 *      provide one when invoking x_svctx_select_svc()
 * Input arguments
 *      pt_svctx        the pointer points to an SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK       the routine is successfully
 *-----------------------------------------------------------------------------*/
static VOID _inp_hdlr_choose_strm_filter(
                    SVCTX_T*                      pt_svctx,
                    INP_SVC_REQ_T*                pt_running_svc_req,
                    STREAM_TYPE_T                 e_strm_type,
                    x_svctx_strm_comp_filter_fct* ppt_strm_fltr_fct,
                    VOID**                        ppv_strm_fltr_tag
                    )
{
    if ((e_strm_type<INP_STRM_TYPE_COUNT) &&
        (e_strm_type>=0) &&
        (pt_running_svc_req->t_strm_reqs[e_strm_type].pt_strm_fltr_fct!= NULL))
    {
        *ppt_strm_fltr_fct = pt_running_svc_req->t_strm_reqs[e_strm_type].pt_strm_fltr_fct;
        *ppv_strm_fltr_tag = pt_running_svc_req->t_strm_reqs[e_strm_type].pv_strm_fltr_tag;
    }
    else
    {
        if ((e_strm_type<STRM_TYPE_COUNT_WITH_AD) &&
            (e_strm_type>=0) &&
            (pt_svctx->pt_def_strm_fltr_fcts[e_strm_type]!=NULL))
        {
            *ppt_strm_fltr_fct = pt_svctx->pt_def_strm_fltr_fcts[e_strm_type];
            *ppv_strm_fltr_tag = pt_svctx->pv_def_strm_fltr_tags[e_strm_type];
        }
        else
        {
            *ppt_strm_fltr_fct = _inp_hdlr_strm_comp_filter;
            *ppv_strm_fltr_tag = (VOID*)pt_svctx;
        }

    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_play_strm
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _inp_hdlr_play_strm(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    PLAYER_T*                   pt_player,
                    STREAM_TYPE_T               e_strm_type,
                    BOOL                        b_auto_play
                    )
{
    x_svctx_strm_comp_filter_fct pt_strm_fltr_fct;
    VOID*                        pv_strm_fltr_tag;
    STRM_REQ_T*                  pt_strm_req;
    STREAM_COMP_ID_T             t_strm_comp_id;
    SCDB_REC_T                   t_scdb_rec;
    INT32                        i4_ret;

    if (((UINT32)e_strm_type >= INP_STRM_TYPE_COUNT) || (e_strm_type==ST_UNKNOWN))
    {
        ASSERT(0, ("%s(), e_strm_type=%d out of boundary\r\n", __func__, e_strm_type));
        return SVCTXR_INV_ARG;
    }

    pt_strm_req = &(pt_running_svc_req->t_strm_reqs[e_strm_type]);

    _inp_hdlr_choose_strm_filter(
                    pt_svctx,
                    pt_running_svc_req,
                    e_strm_type,
                    &pt_strm_fltr_fct,
                    &pv_strm_fltr_tag
                    );

    i4_ret = inp_hdlr_iterate_strm_comp_id(
                    player_get_scdb_handle(pt_player),
                    e_strm_type,
                    TRUE,
                    pt_strm_fltr_fct,
                    pv_strm_fltr_tag,
                    &t_strm_comp_id,
                    &t_scdb_rec
                    );
    if (i4_ret == SVCTXR_NOT_EXIST) {
        pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag = NULL;
        pt_strm_req->u.t_scdb.t_comp_id.e_type        = e_strm_type;
        inp_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_NO_STREAM,
                    pt_strm_req
                    );
        pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
        return i4_ret;
    }

    if (x_handle_valid(pt_strm_req->h_stream) == TRUE) {
        pt_strm_req = svctx_allocate_strm_req(pt_running_svc_req->t_parent.pt_hdlr);
        if (pt_strm_req == NULL) {
            return SVCTXR_NO_RESOURCE;
        }

        pt_running_svc_req->t_strm_reqs[ST_AUDIO].e_penging_status = STRM_PENDING_COND_SELECT_STRM;
        pt_strm_req->h_svctx            = pt_running_svc_req->t_parent.h_svctx;
        pt_strm_req->h_stream           = NULL_HANDLE;
        pt_strm_req->pf_strm_nfy        = pt_running_svc_req->t_parent.pf_svc_nfy;
        pt_strm_req->pv_strm_nfy_tag    = pt_running_svc_req->t_parent.pv_svc_nfy_tag;
        pt_strm_req->u.t_scdb.t_comp_id = t_strm_comp_id;
        pt_strm_req->u.t_scdb.t_rec     = t_scdb_rec;

        if (pt_running_svc_req->pt_pending_strm_reqs[ST_AUDIO] != NULL) {
             svctx_free_strm_req(pt_running_svc_req->pt_pending_strm_reqs[ST_AUDIO]);
        }
        pt_running_svc_req->pt_pending_strm_reqs[ST_AUDIO] = pt_strm_req;
        i4_ret = SVCTXR_OK;
    } else {
        pt_strm_req->h_svctx            = pt_running_svc_req->t_parent.h_svctx;
        pt_strm_req->h_stream           = NULL_HANDLE;
        pt_strm_req->pf_strm_nfy        = pt_running_svc_req->t_parent.pf_svc_nfy;
        pt_strm_req->pv_strm_nfy_tag    = pt_running_svc_req->t_parent.pv_svc_nfy_tag;
        pt_strm_req->u.t_scdb.t_comp_id = t_strm_comp_id;
        pt_strm_req->u.t_scdb.t_rec     = t_scdb_rec;

        i4_ret = inp_hdlr_open_strm(
                    pt_svctx,
                    pt_running_svc_req,
                    pt_player,
                    pt_strm_req,
                    b_auto_play
                    );
    }



    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_with_audio_streams_in_video_player
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static BOOL _inp_hdlr_with_audio_streams_in_video_player(
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    UINT16 ui2_num_recs   = 0;
    UINT32 ui4_db_version = SCDB_NULL_VER_ID;
    INT32  i4_ret         = SVCTXR_NO_ACTION;

    i4_ret = x_scdb_get_num_recs(
                    player_get_scdb_handle(pt_running_svc_req->pt_video_player),
                    ST_AUDIO,
                    &ui2_num_recs,
                    &ui4_db_version
                    );
    if (i4_ret != SCDBR_OK) {
        return FALSE;
    }

    if (ui2_num_recs == 0) {
        return FALSE;
    }

    return TRUE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_able_to_play_audio_from_video_player
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
BOOL inp_hdlr_able_to_play_audio_from_video_player(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    pt_running_svc_req->b_hdmi_signal = TRUE;
    
    if (svctx_with_av_capability(pt_running_svc_req->t_src_info.e_crnt_video_type, ST_AUDIO) == FALSE) {
        pt_running_svc_req->b_hdmi_signal = FALSE;
        return FALSE;
    }

    if (_inp_hdlr_with_audio_streams_in_video_player(pt_running_svc_req) == FALSE) {
        pt_running_svc_req->b_hdmi_signal = FALSE;
        return FALSE;
    }
    
    if (pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_type == SRC_AVC_HINT_NONE) {
        return FALSE;
    }

    if (pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_dev_type
            != pt_running_svc_req->t_src_info.e_crnt_video_type) {
        return FALSE;
    }

    return TRUE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_audio_player_stopped
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_audio_player_stopped(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    BOOL  b_play_audio_in_video_device = FALSE;
    INT32 i4_ret;

    if (pt_running_svc_req->b_replace_audio == TRUE) {
        if (svctx_with_av_capability(pt_running_svc_req->t_src_info.e_crnt_video_type, ST_AUDIO) == TRUE
                && pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_dev_type
                    == pt_running_svc_req->t_src_info.e_crnt_video_type) {
            b_play_audio_in_video_device        = TRUE;
            pt_running_svc_req->b_replace_audio = FALSE;
        } else {
            pt_running_svc_req->b_replace_audio = FALSE;
            i4_ret = player_play(
                        pt_running_svc_req->pt_audio_player,
                        pt_running_svc_req->t_parent.i4_speed
                        );
            return i4_ret;
        }
    }

    i4_ret = player_close(pt_running_svc_req->pt_audio_player);
    if (i4_ret == SVCTXR_OK) {
        pt_running_svc_req->pt_audio_player = NULL;
    }

    if (b_play_audio_in_video_device == TRUE) {
        /* as audio player is stopped,
         * 1. it could be to play another audio player    -- AUDIO_INP <--> SPDIF
         * 2. it could be to play audio from video player -- AUDIO_INP  --> HDMI
         */
        i4_ret = inp_hdlr_check_to_play_audio_from_video_player(pt_svctx, pt_running_svc_req);
        if ((i4_ret == SVCTXR_NO_ACTION) && 
            (pt_running_svc_req->b_play_audio_from_video_device == FALSE)) {
            i4_ret = inp_hdlr_play_audio_player(pt_svctx, pt_running_svc_req);
        }
        return i4_ret;
    }

    if (pt_running_svc_req->pt_video_player == NULL && pt_svctx->b_in_stopping == TRUE) {
        svctx_ntfy_svc_req_stopped(pt_svctx);
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_audio_player_cm_connected
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_audio_player_cm_connected(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_video_player_cm_connected
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_video_player_cm_connected(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_video_player_cm_disconnected
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_video_player_cm_disconnected(
                    SVCTX_T*                    pt_svctx,
                    BOOL                        b_signal_loss
                    )
{
    pt_svctx->e_state = SVCTX_COND_PENDING;

    if (b_signal_loss == TRUE) {
        inp_hdlr_check_signal_loss_priority(
                    pt_svctx,
                    (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req,
                    TRUE,
                    TRUE
                    );
        pt_svctx->e_signal_status = SIGNAL_STATUS_LOSS;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_video_player_cm_info_ntfy
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_video_player_cm_info_ntfy(
                    SVCTX_T*                    pt_svctx,
                    UINT32                      ui4_data
                    )
{
    if (ui4_data == CM_CONN_INFO_LOST_TV_SIGNAL) {
        inp_hdlr_check_signal_loss_priority(
                    pt_svctx,
                    (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req,
                    TRUE,
                    FALSE
                    );
        pt_svctx->e_signal_status = SIGNAL_STATUS_LOSS;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_audio_player_cm_disconnected
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_audio_player_cm_disconnected(
                    SVCTX_T*                    pt_svctx,
                    BOOL                        b_signal_loss
                    )
{
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_video_player_sess_opened
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_video_player_sess_opened(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    PLAYER_T*                   pt_player
                    )
{
    INT32 i4_ret;

    svctx_check_analog_tuner_out_block(
                    pt_svctx,
                    player_get_sess_handle(pt_running_svc_req->pt_video_player)
                    );

    i4_ret = _inp_hdlr_play_strm(
                    pt_svctx,
                    pt_running_svc_req,
                    pt_player,
                    ST_VIDEO,
                    FALSE
                    );
    /* still to play stream when no stream existed */
    if (i4_ret != SVCTXR_OK && i4_ret != SVCTXR_NOT_EXIST) {
        return i4_ret;
    }

    i4_ret = inp_hdlr_check_to_play_audio_from_video_player(
                    pt_svctx,
                    pt_running_svc_req
                    );
    if (i4_ret != SVCTXR_OK && i4_ret != SVCTXR_NO_ACTION) {
        return i4_ret;
    }

    i4_ret = player_play(
                    pt_player,
                    pt_running_svc_req->t_parent.i4_speed
                    );
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_audio_player_sess_opened
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_audio_player_sess_opened(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    PLAYER_T*                   pt_player
                    )
{
    INT32 i4_ret;

    if (pt_running_svc_req->t_parent.ui4_stream_mode & ST_MASK_AUDIO) {
        i4_ret = _inp_hdlr_play_strm(
                    pt_svctx,
                    pt_running_svc_req,
                    pt_player,
                    ST_AUDIO,
                    FALSE
                    );
        /* still to play session when no stream existed */
        if (i4_ret != SVCTXR_OK && i4_ret != SVCTXR_NOT_EXIST) {
            return i4_ret;
        }
    }

    i4_ret = player_play(
                    pt_player,
                    pt_running_svc_req->t_parent.i4_speed
                    );
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_video_player_sess_started
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_video_player_sess_started(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_audio_player_sess_started
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_audio_player_sess_started(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_video_player_sess_in_closing
 * Description
 * Input arguments
 *      pt_svctx            the pointer points to a SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK
 *      SVCTXR_PENDING
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_video_player_sess_in_closing(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    pt_running_svc_req->t_strm_reqs[ST_VIDEO].e_penging_status = STRM_PENDING_COND_NONE;
    svctx_reset_video_info(pt_svctx);
    if (pt_running_svc_req->b_play_audio_from_video_device == TRUE) {
        svctx_reset_audio_info(pt_svctx);
    }
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_audio_player_sess_in_closing
 * Description
 * Input arguments
 *      pt_svctx            the pointer points to a SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK
 *      SVCTXR_PENDING
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_audio_player_sess_in_closing(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    pt_running_svc_req->t_strm_reqs[ST_AUDIO].e_penging_status = STRM_PENDING_COND_NONE;
    svctx_reset_audio_info(pt_svctx);
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_video_player_sess_in_stopping
 * Description
 * Input arguments
 *      pt_svctx            the pointer points to a SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK
 *      SVCTXR_PENDING
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_video_player_sess_in_stopping(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    svctx_reset_video_info(pt_svctx);
    if (pt_running_svc_req->b_play_audio_from_video_device == TRUE) {
        svctx_reset_audio_info(pt_svctx);
    }
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_audio_player_sess_in_stopping
 * Description
 * Input arguments
 *      pt_svctx            the pointer points to a SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK
 *      SVCTXR_PENDING
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_audio_player_sess_in_stopping(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    svctx_reset_audio_info(pt_svctx);
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_scdb_updated
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_scdb_updated(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    PLAYER_T*                   pt_player,
                    UINT32                      ui4_reason
                    )
{
    SM_SESS_COND_T e_sess_cond;

    if (ui4_reason == SCDB_REASON_UNK) {
        return SVCTXR_DONE;
    }

    e_sess_cond = player_get_sess_cond(pt_player);
    if (e_sess_cond != SM_SESS_COND_OPENED
            && e_sess_cond != SM_SESS_COND_STARTING
            && e_sess_cond != SM_SESS_COND_STARTED) {
        return SVCTXR_DONE;
    }

    if ((ui4_reason & SCDB_RECORD_DEL) > 0) {
        inp_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_SCDB_DEL,
                    ST_UNKNOWN
                    );
    }

    if ((ui4_reason & SCDB_RECORD_ADD) > 0) {
        inp_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_SCDB_ADD,
                    ST_UNKNOWN
                    );
    }

    if ((ui4_reason & SCDB_RECORD_MOD) > 0) {
        inp_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_SCDB_MODIFY,
                    ST_UNKNOWN
                    );
    }

    if (ui4_reason == SCDB_COMMON_MOD) {
        inp_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_SCDB_MODIFY,
                    ST_UNKNOWN
                    );
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_video_player_scdb_updated
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_video_player_scdb_updated(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    PLAYER_T*                   pt_video_player,
                    UINT32                      ui4_reason
                    )
{
    STRM_REQ_T* pt_strm_req;
    SCDB_REC_T  t_scdb_rec;
    INT32       i4_ret;

    if (pt_svctx->b_is_blocked == TRUE) {
        return SVCTXR_OK;
    }

    i4_ret = _inp_hdlr_handle_scdb_updated(
                            pt_svctx,
                            pt_running_svc_req,
                            pt_video_player,
                            ui4_reason
                            );
    if (i4_ret == SVCTXR_DONE) {
        return SVCTXR_OK;
    }

    /* check old video stream is still in SCDB */
    pt_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]);
    i4_ret      = SCDBR_REC_NOT_FOUND;
    if (x_handle_valid(pt_strm_req->h_stream) == TRUE && pt_strm_req->u.t_scdb.t_comp_id.e_type == ST_VIDEO) {
        i4_ret = x_scdb_get_rec (
                            player_get_scdb_handle(pt_video_player),
                            &(pt_strm_req->u.t_scdb.t_comp_id),
                            &t_scdb_rec
                            );
    }

    if (i4_ret != SCDBR_OK) {
        if (x_handle_valid(pt_strm_req->h_stream) == TRUE) {
            pt_running_svc_req->b_replace_video = TRUE;
            i4_ret = x_sm_close_stream(pt_strm_req->h_stream);
        } 
        else if( player_get_sess_cond(pt_video_player) != SM_SESS_COND_OPENED &&
                 player_get_sess_cond(pt_video_player) != SM_SESS_COND_STARTING &&
                 player_get_sess_cond(pt_video_player) != SM_SESS_COND_STARTED ){
                 /* handle race condition when open stream in session opened and scdb updated */
            i4_ret = _inp_hdlr_play_strm(
                            pt_svctx,
                            pt_running_svc_req,
                            pt_video_player,
                            ST_VIDEO,
                            (player_get_sess_cond(pt_video_player) == SM_SESS_COND_STARTED ? TRUE : FALSE)
                            );
        }
    }

    if (pt_running_svc_req->b_play_audio_from_video_device == TRUE) {
        /* check old audio stream is still in */
        pt_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_AUDIO]);
        i4_ret      = SCDBR_REC_NOT_FOUND;
        if (x_handle_valid(pt_strm_req->h_stream) == TRUE && pt_strm_req->u.t_scdb.t_comp_id.e_type == ST_AUDIO) {
            i4_ret = x_scdb_get_rec (
                            player_get_scdb_handle(pt_video_player),
                            &(pt_strm_req->u.t_scdb.t_comp_id),
                            &t_scdb_rec
                            );
        }

        if (i4_ret == SCDBR_OK) {
            return SVCTXR_OK;
        }

        /* if video of range and SCDB update - audio SCDB removed, just stop audio stream
         * don't need to select audio from audio player.
         */
        if (pt_svctx->t_video_info.e_timing_type == VSH_SRC_TIMING_NOT_SUPPORT
                && pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_dev_type == DEV_AVC_HDMI) {
            if (x_handle_valid(pt_strm_req->h_stream) == TRUE) {
                i4_ret = x_sm_close_stream(pt_strm_req->h_stream);
                if (i4_ret == SMR_OK || i4_ret == SMR_PENDING) {
                    pt_running_svc_req->t_strm_reqs[ST_AUDIO].e_strm_cond = SM_COND_CLOSING;
                    return SVCTXR_OK;
                } else {
                    return SVCTXR_FAIL;
                }
            }
        }

        /* the old audio stream is absent */
        if (_inp_hdlr_with_audio_streams_in_video_player(pt_running_svc_req) == FALSE) {
            pt_running_svc_req->b_play_audio_from_video_device        = FALSE;
            if ((pt_running_svc_req->t_parent.ui4_stream_mode & ST_MASK_AUDIO) == 0) {
                pt_running_svc_req->b_replace_audio = FALSE;
            } else {
                pt_running_svc_req->b_replace_audio = TRUE;
            }

            if (x_handle_valid(pt_strm_req->h_stream) == TRUE) {
                i4_ret = x_sm_close_stream(pt_strm_req->h_stream);
                return (i4_ret != SMR_OK && i4_ret != SMR_PENDING) ? SVCTXR_FAIL : SVCTXR_OK;
            } else {
                if (pt_running_svc_req->b_replace_audio == FALSE) {
                    return SVCTXR_OK;
                } else {
                    return inp_hdlr_play_audio_player(
                            pt_svctx,
                            pt_running_svc_req
                            );
                }
            }
        }
    }

    return inp_hdlr_check_to_play_audio_from_video_player(
                            pt_svctx,
                            pt_running_svc_req
                            );
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_audio_player_scdb_updated
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_audio_player_scdb_updated(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    PLAYER_T*                   pt_audio_player,
                    UINT32                      ui4_reason
                    )
{
    STRM_REQ_T* pt_strm_req;
    SCDB_REC_T  t_scdb_rec;
    INT32       i4_ret;

    i4_ret = _inp_hdlr_handle_scdb_updated(
                    pt_svctx,
                    pt_running_svc_req,
                    pt_audio_player,
                    ui4_reason
                    );
    if (i4_ret == SVCTXR_DONE) {
        return SVCTXR_OK;
    }

    if (pt_running_svc_req->b_play_audio_from_video_device == TRUE) {
        return i4_ret;
    }

    /* check old audio stream is still in SCDB */
    pt_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_AUDIO]);
    i4_ret      = SCDBR_REC_NOT_FOUND;
    if (x_handle_valid(pt_strm_req->h_stream) == TRUE && pt_strm_req->u.t_scdb.t_comp_id.e_type == ST_AUDIO) {
        i4_ret = x_scdb_get_rec (
                    player_get_scdb_handle(pt_audio_player),
                    &(pt_strm_req->u.t_scdb.t_comp_id),
                    &t_scdb_rec
                    );
    }

    if (i4_ret == SCDBR_OK) {
        return SVCTXR_OK;
    }

    if (x_handle_valid(pt_strm_req->h_stream) == TRUE) {
        pt_running_svc_req->b_replace_audio = TRUE;
        i4_ret = x_sm_close_stream(pt_strm_req->h_stream);
    } else {
        if (pt_running_svc_req->t_parent.ui4_stream_mode & ST_MASK_AUDIO) {
            i4_ret = _inp_hdlr_play_strm(
                    pt_svctx,
                    pt_running_svc_req,
                    pt_audio_player,
                    ST_AUDIO,
                    (player_get_sess_cond(pt_audio_player) == SM_SESS_COND_STARTED ? TRUE : FALSE)
                    );
        }
    }
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_video_player_nty
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_video_player_nty(
                    PLAYER_T*                   pt_player,
                    PLAYER_NTFY_CODE_T          e_code,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data
                    )
{
    SVCTX_T*       pt_svctx           = (SVCTX_T*) pv_tag;
    INP_SVC_REQ_T* pt_running_svc_req = NULL;
    INT32          i4_ret             = SVCTXR_OK;

    ASSERT(pt_svctx != NULL, ("%s(), pt_svctx is NULL",__func__));

    pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL, player=0x%x\r\n", __func__, pt_player));

    switch(e_code) {
    case PLAYER_NTFY_CODE_PIPE_OPENED_GET_CMDS:
#ifdef MW_TV_AV_BYPASS_SUPPORT
        pt_svctx->b_fake_disc = FALSE;
#endif
        i4_ret = _inp_hdlr_get_video_player_cm_cmds(pt_svctx, pt_running_svc_req, (CM_COMMAND_T*)ui4_data);
        break;
    case PLAYER_NTFY_CODE_PIPE_CLOSED:
        i4_ret = _inp_hdlr_handle_video_player_stopped(pt_svctx, pt_running_svc_req);
        break;
    case PLAYER_NTFY_CODE_CONN_CONNECTED:
#ifdef MW_TV_AV_BYPASS_SUPPORT
        pt_running_svc_req->b_combi_switch = FALSE;
#endif
        i4_ret = _inp_hdlr_handle_video_player_cm_connected(pt_svctx, pt_running_svc_req);
        break;
    case PLAYER_NTFY_CODE_CONN_DISCONNECTED:
        i4_ret = _inp_hdlr_handle_video_player_cm_disconnected(pt_svctx, (BOOL)ui4_data);
        break;
    case PLAYER_NTFY_CODE_CONN_INFO_NTFY:
        i4_ret = _inp_hdlr_handle_video_player_cm_info_ntfy(pt_svctx, ui4_data);
        break;
    case PLAYER_NTFY_CODE_SCDB_UPDATED:
        i4_ret = _inp_hdlr_handle_video_player_scdb_updated(pt_svctx, pt_running_svc_req, pt_player, ui4_data);
        break;
    case PLAYER_NTFY_CODE_SESS_OPENED:
        i4_ret = _inp_hdlr_handle_video_player_sess_opened(pt_svctx, pt_running_svc_req, pt_player);
        break;
    case PLAYER_NTFY_CODE_SESS_STARTED:
        i4_ret = _inp_hdlr_handle_video_player_sess_started(pt_svctx, pt_running_svc_req);
        break;
    case PLAYER_NTFY_CODE_SESS_CLOSING:
        i4_ret = _inp_hdlr_handle_video_player_sess_in_closing(pt_svctx, pt_running_svc_req);
        break;
    case PLAYER_NTFY_CODE_SESS_STOPPING:
        i4_ret = _inp_hdlr_handle_video_player_sess_in_stopping(pt_svctx, pt_running_svc_req);
        break;
    case PLAYER_NTFY_CODE_REQ_SEND_MSG:
        i4_ret = svctx_send_msg(pt_svctx->h_sys_svctx, (SVCTX_MSG_T*)ui4_data);
        break;

#ifdef MW_TV_AV_BYPASS_SUPPORT
    case PLAYER_NTFY_CODE_QUERY_ACTION:
        i4_ret = svctx_handle_action_query(
            pt_svctx,
            pt_player,
            PLAYER_TYPE_INP_SVC_VIDEO,
            (SVCTX_MSG_T*) ui4_data);
        break;

    case PLAYER_NTFY_CODE_CHECK_QUEUED_REQ:
        i4_ret = svctx_mgr_check_queued_req(
            ACTIVE_LINK_NOW,
            pt_svctx->pt_running_svc_req->e_src_type,
            pt_svctx,
            FALSE);
        break;
#endif

    default:
        break;
    }
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_audio_player_nty
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_audio_player_nty(
                    PLAYER_T*                   pt_player,
                    PLAYER_NTFY_CODE_T          e_code,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data
                    )
{
    SVCTX_T*       pt_svctx           = (SVCTX_T*) pv_tag;
    INP_SVC_REQ_T* pt_running_svc_req = NULL;
    INT32          i4_ret             = SVCTXR_OK;

    ASSERT(pt_svctx != NULL, ("%s(), pt_svctx is NULL",__func__));

    pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL, player=0x%x\r\n", __func__, pt_player));

    switch(e_code) {
    case PLAYER_NTFY_CODE_PIPE_OPENED_GET_CMDS:
        i4_ret = _inp_hdlr_get_audio_player_cm_cmds(pt_svctx, pt_running_svc_req, (CM_COMMAND_T*)ui4_data);
        break;
    case PLAYER_NTFY_CODE_PIPE_CLOSED:
        i4_ret = _inp_hdlr_handle_audio_player_stopped(pt_svctx, pt_running_svc_req);
        break;
    case PLAYER_NTFY_CODE_CONN_CONNECTED:
        i4_ret = _inp_hdlr_handle_audio_player_cm_connected(pt_svctx, pt_running_svc_req);
        break;
    case PLAYER_NTFY_CODE_CONN_DISCONNECTED:
        i4_ret = _inp_hdlr_handle_audio_player_cm_disconnected(pt_svctx, (BOOL)ui4_data);
        break;
    case PLAYER_NTFY_CODE_SCDB_UPDATED:
        i4_ret = _inp_hdlr_handle_audio_player_scdb_updated(pt_svctx, pt_running_svc_req, pt_player, ui4_data);
        break;
    case PLAYER_NTFY_CODE_SESS_OPENED:
        i4_ret = _inp_hdlr_handle_audio_player_sess_opened(pt_svctx, pt_running_svc_req, pt_player);
        break;
    case PLAYER_NTFY_CODE_SESS_STARTED:
        i4_ret = _inp_hdlr_handle_audio_player_sess_started(pt_svctx, pt_running_svc_req);
        break;
    case PLAYER_NTFY_CODE_SESS_CLOSING:
        i4_ret = _inp_hdlr_handle_audio_player_sess_in_closing(pt_svctx, pt_running_svc_req);
        break;
    case PLAYER_NTFY_CODE_SESS_STOPPING:
        i4_ret = _inp_hdlr_handle_audio_player_sess_in_stopping(pt_svctx, pt_running_svc_req);
        break;
    case PLAYER_NTFY_CODE_REQ_SEND_MSG:
        i4_ret = svctx_send_msg(pt_svctx->h_sys_svctx, (SVCTX_MSG_T*)ui4_data);
        break;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    case PLAYER_NTFY_CODE_QUERY_ACTION:
        i4_ret = svctx_handle_action_query(
            pt_svctx,
            pt_player,
            PLAYER_TYPE_INP_SVC_AUDIO,
            (SVCTX_MSG_T*) ui4_data);
        break;
    case PLAYER_NTFY_CODE_CHECK_QUEUED_REQ:
        i4_ret = svctx_mgr_check_queued_req(
            ACTIVE_LINK_NOW,
            pt_svctx->pt_running_svc_req->e_src_type,
            pt_svctx,
            FALSE);
        break;
#endif
    default:
        break;
    }
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_open_cc_stream_after_video_started
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _inp_hdlr_open_cc_stream_after_video_started(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    STRM_REQ_T*      pt_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_CLOSED_CAPTION]);
    INT32            i4_ret      = SVCTXR_OK;
    STREAM_COMP_ID_T t_comp_id;

    /* if there's a closed stream is already opened, bypass it.
     * It could happen in a blocked channel after it is unblocked.
     */
    if (pt_strm_req == NULL
            || pt_strm_req->u.t_scdb.t_comp_id.e_type == ST_UNKNOWN
            || x_handle_valid(pt_strm_req->h_stream) == TRUE
            || pt_running_svc_req->t_cc_filter.e_cc_type == SM_CC_TYPE_IGNORE) {
        return SVCTXR_OK;
    }

    /* if in frozen mode, bypass it */
    if ((pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_CLOSED_CAPTION) > 0) {
        return SVCTXR_OK;
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
    if (i4_ret == SVCTXR_OK) {
        pt_strm_req->u.t_scdb.t_comp_id = t_comp_id;
    } else {
        pt_strm_req->u.t_scdb.t_comp_id.e_type        = ST_CLOSED_CAPTION;
        pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag = NULL;
    }

    i4_ret = inp_hdlr_open_strm(
                    pt_svctx,
                    pt_running_svc_req,
                    pt_running_svc_req->pt_video_player,
                    pt_strm_req,
                    TRUE
                    );
    if (i4_ret == SVCTXR_OK) {
        pt_strm_req->e_penging_status      = STRM_PENDING_COND_SELECT_STRM;
    } else {
        pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
        pt_strm_req->e_penging_status      = STRM_PENDING_COND_NONE;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_video_strm_post_start_handler
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _inp_hdlr_video_strm_post_start_handler(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    INT32       i4_ret            = SVCTXR_OK;
    STRM_REQ_T* pt_video_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]);

    do {
        if (x_handle_valid(pt_video_strm_req->h_stream) == FALSE) {
            break;
        }

        /* CR Fixed -- DTV00043282
           H/V position needs to reset in VGA only.
           We don't need to set H/V position if input is HDMI */
        if (pt_running_svc_req->t_src_info.t_av_src.e_video_type == DEV_AVC_HDMI)
        {
            if (pt_svctx->ui2_video_h_pos != SM_VSH_VID_H_POS_UNKNOWN)
            {
                pt_svctx->ui2_video_h_pos = SM_VSH_VID_H_POS_UNKNOWN;
            }

            if (pt_svctx->ui2_video_v_pos != SM_VSH_VID_V_POS_UNKNOWN)
            {
                pt_svctx->ui2_video_v_pos = SM_VSH_VID_V_POS_UNKNOWN;
            }
        }

        i4_ret = svctx_set_video_stream_attrs(pt_svctx,  pt_video_strm_req->h_stream);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->b_is_blocked == TRUE) {
            break;
        }

        if ((pt_running_svc_req->t_parent.ui4_stream_mode & ST_MASK_CLOSED_CAPTION) > 0) {
            i4_ret = _inp_hdlr_open_cc_stream_after_video_started(pt_svctx, pt_running_svc_req);
        }
    } while (0);

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_update_video_clk_pos_phs_attrs
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_update_video_clk_pos_phs_attrs (
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_video_stream
                    )
{
    SIZE_T z_size;
    INT32  i4_ret;

    if (pt_svctx == NULL || x_handle_valid(h_video_stream) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    z_size = sizeof(pt_svctx->i2_video_clock);
    i4_ret = x_sm_get(
                    h_video_stream,
                    SM_VSH_GET_TYPE_VID_CLOCK,
                    (VOID*)&pt_svctx->i2_video_clock,
                    &z_size
                    );
    if (i4_ret != SMR_OK && i4_ret != SMR_NOT_SUPPORT) {
        DBG_ERROR((_ERROR_HEADER"x_sm_set(SM_VSH_GET_TYPE_VID_CLOCK, handle= %d) failed, ret=%d\r\n",
                    h_video_stream,
                    i4_ret));
    }

    z_size = sizeof(pt_svctx->ui2_video_phase);
    i4_ret = x_sm_get(
                    h_video_stream,
                    SM_VSH_GET_TYPE_VID_PHASE,
                    (VOID*)&pt_svctx->ui2_video_phase,
                    &z_size
                    );
    if (i4_ret != SMR_OK && i4_ret != SMR_NOT_SUPPORT) {
        DBG_ERROR((_ERROR_HEADER"x_sm_set(SM_VSH_GET_TYPE_VID_PHASE, handle= %d) failed, ret=%d\r\n",
                    h_video_stream,
                    i4_ret));
    }

    z_size = sizeof(pt_svctx->ui2_video_h_pos);
    i4_ret = x_sm_get(
                    h_video_stream,
                    SM_VSH_GET_TYPE_VID_H_POS,
                    (VOID*)&pt_svctx->ui2_video_h_pos,
                    &z_size
                    );
    if (i4_ret != SMR_OK && i4_ret != SMR_NOT_SUPPORT) {
        DBG_ERROR((_ERROR_HEADER"x_sm_set(SM_VSH_GET_TYPE_VID_H_POS, handle= %d) failed, ret=%d\r\n",
                    h_video_stream,
                    i4_ret));
    }

    z_size = sizeof(pt_svctx->ui2_video_v_pos);
    i4_ret = x_sm_get(
                    h_video_stream,
                    SM_VSH_GET_TYPE_VID_V_POS,
                    (VOID*)&pt_svctx->ui2_video_v_pos,
                    &z_size
                    );
    if (i4_ret != SMR_OK && i4_ret != SMR_NOT_SUPPORT) {
        DBG_ERROR((_ERROR_HEADER"x_sm_set(SM_VSH_GET_TYPE_VID_V_POS, handle= %d) failed, ret=%d\r\n",
                    h_video_stream,
                    i4_ret));
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_hdmi_timeout_to_play_audio
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_hdmi_timeout_to_play_audio(
                    VOID*                   pv_tag,
                    SVCTX_MSG_T*            pt_msg
                    )
{
    SVCTX_T*       pt_svctx = (SVCTX_T*)pv_tag;
    INP_SVC_REQ_T* pt_running_svc_req;

    if (pt_svctx == NULL) {
        return SVCTXR_OK;
    }

    pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        return SVCTXR_OK;
    }

#if 0 /* mark for CR DTV00066441 */
    if ((pt_running_svc_req->t_parent.ui4_stream_mode & ST_MASK_AUDIO) == 0) {
        return SVCTXR_OK;
    }
#endif

    if (pt_svctx->t_video_info.e_timing_type != VSH_SRC_TIMING_NOT_SUPPORT
            && pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_dev_type == DEV_AVC_HDMI) {
        /* if audio is preferred HDMI, but no audio from HDMI */
        if (pt_running_svc_req->t_src_info.e_crnt_audio_type != DEV_AVC_HDMI
                && pt_running_svc_req->pt_audio_player == NULL 
                && pt_running_svc_req->b_play_audio_from_video_device == FALSE) {
            /* start to play audio player */
            inp_hdlr_play_audio_player(pt_svctx, pt_running_svc_req);
        }
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_hdmi_timeout_cb
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static VOID _inp_hdlr_hdmi_timeout_cb(
                    HANDLE_T                    h_timer,
                    VOID*                       pv_tag
                    )
{
    SVCTX_MSG_T t_msg    = {0};
    SVCTX_T*    pt_svctx = (SVCTX_T*)pv_tag;

    if (pt_svctx == NULL) {
        return;
    }

    t_msg.pf_invoke_func = _inp_hdlr_hdmi_timeout_to_play_audio;
    t_msg.pv_tag         = pv_tag;

    svctx_send_msg(pt_svctx->h_sys_svctx, &t_msg);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_strm_event
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_strm_event(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    STRM_REQ_T*                 pt_strm_req,
                    SVCTX_STRM_MSG_T*           pt_strm_msg
                    )
{
    INT32 i4_ret;
    SCC_PROTECT_INFO_T      e_audio_protect_info;

    switch(pt_strm_msg->e_event) {
    case SM_EVN_VS_ANA_TV_SIGNAL:
#ifdef MW_TV_AV_BYPASS_SUPPORT
    case SM_EVN_BYPASS_ANA_TV_SIGNAL:
#endif
        if (pt_svctx->b_in_stopping == TRUE) {
            break;
        }

        if (pt_strm_msg->uinfo.e_tv_signal_status == VSH_ANA_TV_SIG_STABLE) {
            /* for DTV00359016  */                        
            if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOCKED)                        
            {                                
                svctx_set_av_status(pt_svctx, SVC_AV_STATUS_UNKNOWN);                        
            }

            if (pt_svctx->b_is_blocked)
            {
                inp_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOCKED_AS_BLOCKED, ST_UNKNOWN);
            }
            else
            {
                inp_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOCKED, ST_UNKNOWN);
            }

            /* don't do the audio/video fmt query actively
            svctx_update_video_info_from_sm(pt_svctx,pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream);
            svctx_update_audio_info_from_sm(pt_svctx,pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream);
            */
            /* svctx_with_event_capability need video resolution */
            if (svctx_with_event_capability(
                    pt_svctx,
                    pt_running_svc_req->t_src_info.e_crnt_video_type,
                    DEV_EVN_CAPABILITY_ALL) == TRUE) {
                crnt_evn_start_query(
                    pt_running_svc_req->pt_crnt_event,
                    DEV_AVC,
                    pt_running_svc_req->t_src_info.s_src_grp_name,
                    player_get_conn_handle(pt_running_svc_req->pt_video_player),
                    NULL
                    );
            }

            if (pt_svctx->b_is_blocked == FALSE) {
                pt_svctx->e_state = SVCTX_COND_PRESENTING;
                inp_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_NORMAL, ST_UNKNOWN);
                if (!(pt_svctx->t_video_info.e_timing_type == VSH_SRC_TIMING_NOT_SUPPORT
                        && pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_dev_type == DEV_AVC_HDMI)) {
                    if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) == FALSE) {
                        if (inp_hdlr_able_to_play_audio_from_video_player(
                                pt_svctx,
                                pt_running_svc_req
                                ) == TRUE) {
                            pt_running_svc_req->b_play_audio_from_video_device = TRUE;
                            if ((pt_running_svc_req->t_parent.ui4_stream_mode & ST_MASK_AUDIO) > 0) {
                                inp_hdlr_play_audio_from_video_player(pt_svctx, pt_running_svc_req);
                            }
                        } else {
                            /* if client doesn't want to play audio in the meantime,
                             * it still needs to create a connection to have SCDB
                             * Then, as the client want to select an audio stream,
                             * svctx know how to get the SCDB
                             */
                            inp_hdlr_play_audio_player(pt_svctx, pt_running_svc_req);
                        }
                    } else {
                        inp_hdlr_check_audio_fmt(pt_svctx, pt_running_svc_req);
                    }
                    inp_hdlr_check_video_fmt(pt_svctx, pt_running_svc_req);
                    _inp_hdlr_video_strm_post_start_handler(pt_svctx, pt_running_svc_req);
                }
            } else {
                inp_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOCKED_AS_BLOCKED, ST_UNKNOWN);
            }

            if ((pt_svctx->ui1_stop_audio_as_no_signal & MAKE_BIT_MASK_8(pt_running_svc_req->t_parent.e_src_type)) == 0) {
                inp_hdlr_check_to_play_hdmi_audio(pt_svctx, pt_running_svc_req);
            }
        } 
        else if (pt_strm_msg->uinfo.e_tv_signal_status == VSH_ANA_TV_SIG_NONE)
        {
            pt_running_svc_req->b_hdmi_signal = FALSE;

            if (pt_running_svc_req->t_src_info.t_av_src.e_video_type == DEV_AVC_COMBI
                    && pt_running_svc_req->t_src_info.e_crnt_video_type == DEV_AVC_S_VIDEO
                    && pt_svctx->e_detection_mode == DETECTION_MODE_BY_SIGNAL_STATUS) {
         /* it is caused from S-Video signal cable is pulled out,
                 * and it needs more time to confirm the CVBS with signal,
                 * even driver already report No signal!
                 */
                svctx_set_signal_loss_chk_delay(pt_svctx, DEFAULT_COMBI_SIGNAL_LOSS_CHK_TIMEOUT);
                inp_hdlr_invoke_client_svc_nfy_fct(
                            pt_svctx,
                            SVCTX_NTFY_CODE_IN_STOPPING,
                            ST_UNKNOWN
                            );
                inp_hdlr_stop_video_player(pt_svctx, pt_running_svc_req, FALSE);
                /* if no signal from COMBI, delay to until ACM start to check it is
                 * no signal or just other high priority source is connected.
                 */
            } else {
                inp_hdlr_check_signal_loss_priority(
                            pt_svctx,
                            pt_running_svc_req,
                            TRUE,
                            TRUE
                            );
                pt_svctx->e_signal_status = SIGNAL_STATUS_LOSS;
                if (pt_svctx->b_is_blocked == TRUE) {
                    inp_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOSS_AS_BLOCKED, ST_UNKNOWN);
                }
            }

            crnt_evn_stop_query(pt_running_svc_req->pt_crnt_event);
            svctx_reset_video_info(pt_svctx);
            _inp_hdlr_close_text_strms(
                    pt_svctx,
                    pt_running_svc_req,
                    ST_MASK_CLOSED_CAPTION
                    );
            if ((pt_svctx->ui1_stop_audio_as_no_signal & MAKE_BIT_MASK_8(pt_running_svc_req->t_parent.e_src_type)) > 0) {
                if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) == TRUE) {
                    if (pt_running_svc_req->b_play_audio_from_video_device == TRUE) {
                        i4_ret = x_sm_close_stream(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream);
                        if (i4_ret == SMR_OK || i4_ret == SMR_PENDING) {
                            pt_running_svc_req->t_strm_reqs[ST_AUDIO].e_strm_cond = SM_COND_CLOSING;
                        }
                        break;
                    }
                }

                i4_ret = inp_hdlr_stop_audio_player(pt_svctx, pt_running_svc_req);
                if (i4_ret != SVCTXR_OK && i4_ret != SVCTXR_PENDING) {
                    return SVCTXR_FAIL;
                }
            } else {
                if (pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_dev_type == DEV_AVC_HDMI) {
                    inp_hdlr_check_to_play_hdmi_audio(pt_svctx, pt_running_svc_req);
                } else {
                    if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) == FALSE) {
                        inp_hdlr_play_audio_player(pt_svctx, pt_running_svc_req);
                    }
                }
            }
        }
        else /* VSH_ANA_TV_SIG_UNKNOWN */
        {
            inp_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_INP_SIGNAL_UNKNOWN, ST_UNKNOWN);
        }
        break;
    case SM_EVN_VS_AUTO_CLK_POS_PHS_DONE:
        pt_svctx->e_auto_clk_pos_phs_cond = (pt_strm_msg->uinfo.b_auto_clk_pos_phs_done == TRUE) ? AUTO_CLK_POS_PHS_COND_SUCCESS : AUTO_CLK_POS_PHS_COND_FAILED;
        /* Only VGA need to remember h/v position */
        if (pt_running_svc_req->t_src_info.t_av_src.e_video_type == DEV_AVC_VGA)
        {
            _inp_hdlr_update_video_clk_pos_phs_attrs(pt_svctx, pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream);
        }
        inp_hdlr_invoke_client_strm_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_VIDEO_AUTO_CLK_POS_PHS_DONE, pt_strm_req);
        break;
    case SM_EVN_VS_AUTO_COLOR_DONE:
        pt_svctx->e_auto_color_cond = (pt_strm_msg->uinfo.b_auto_color_done == TRUE) ? AUTO_COLOR_COND_SUCCESS : AUTO_COLOR_COND_FAILED;
        inp_hdlr_invoke_client_strm_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_VIDEO_AUTO_COLOR_DONE, pt_strm_req);
        break;
    case SM_EVN_VS_ANA_COLOR_SYSTEM:
        svctx_set_color_sys(pt_svctx, &pt_strm_msg->uinfo.t_clr_sys);
        inp_hdlr_invoke_client_strm_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_VIDEO_ANA_COLOR_SYSTEM, pt_strm_req);
        break;
    case SM_EVN_VS_VIDEO_RESOLUTION_CHG:
#ifdef MW_TV_AV_BYPASS_SUPPORT
    case SM_EVN_BYPASS_VIDEO_RESOLUTION_CHG:
#endif
        svctx_set_video_info(pt_svctx, &pt_strm_msg->uinfo.t_video);
        inp_hdlr_invoke_client_strm_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE, pt_strm_req);

        /* only check to start or stop current event if signal status = SIGNAL_STATUS_LOCKED */
        if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOCKED) {
            if (svctx_with_event_capability(
                    pt_svctx,
                    pt_running_svc_req->t_src_info.e_crnt_video_type,
                    DEV_EVN_CAPABILITY_ALL) == TRUE) {
                if (crnt_evn_is_in_querying(pt_running_svc_req->pt_crnt_event) == FALSE) {
                    crnt_evn_start_query(
                        pt_running_svc_req->pt_crnt_event,
                        DEV_AVC,
                        pt_running_svc_req->t_src_info.s_src_grp_name,
                        player_get_conn_handle(pt_running_svc_req->pt_video_player),
                        NULL
                        );
                }
            } else {
                inp_hdlr_check_svc_block(pt_svctx);
                if (crnt_evn_is_in_querying(pt_running_svc_req->pt_crnt_event) == TRUE) {
                    crnt_evn_stop_query(pt_running_svc_req->pt_crnt_event);
                    /* force client to re-get event again */
                    inp_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_CRNT_EVN_READY,
                        ST_UNKNOWN
                        );
                }
            }
        }
        break;
    case SM_EVN_AS_AUD_INFO_CHG:
        svctx_set_audio_info(pt_svctx, &pt_strm_msg->uinfo.t_audio);
        inp_hdlr_invoke_client_strm_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE, pt_strm_req);
        break;

    case SM_EVN_AS_DECODE_STATUS_CHG:
        pt_svctx->e_audio_decode_status = pt_strm_msg->uinfo.e_audio_decode_status;
        switch(pt_strm_msg->uinfo.e_audio_decode_status) {
        case ASH_AUDIO_DECODE_NOT_SUPPORT:
            svctx_reset_audio_info(pt_svctx);
            break;
        default:
            break;
        }
        inp_hdlr_check_audio_fmt(pt_svctx, pt_running_svc_req);
        break;

    case SM_EVN_CCS_EXISTED_CC_IND_CHG:
        inp_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_EXISTED_CC_IND_CHG,
                    ST_CLOSED_CAPTION
                    );
        break;
    case SM_EVN_VS_CC_INDICATOR:
        if ((pt_strm_msg->uinfo.b_cc_indicator == TRUE  && pt_svctx->e_cc_avil_cond == SVCTX_CC_AVIL_COND_YES) ||
            (pt_strm_msg->uinfo.b_cc_indicator == FALSE && pt_svctx->e_cc_avil_cond == SVCTX_CC_AVIL_COND_NO)) {
            /* nothing change */
            break;
        }

        if (pt_strm_msg->uinfo.b_cc_indicator == TRUE) {
            pt_svctx->e_cc_avil_cond = SVCTX_CC_AVIL_COND_YES;
            inp_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_WITH_CC_AVIL_INDICATOR,
                    ST_CLOSED_CAPTION
                    );
        } else {
            pt_svctx->e_cc_avil_cond = SVCTX_CC_AVIL_COND_NO;
            inp_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_NO_CC_AVIL_INDICATOR,
                    ST_CLOSED_CAPTION
                    );
        }
        break;
    case SM_EVN_VS_ANA_TV_SIGNAL_DETECTING:
        inp_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_SIGNAL_DETECTING,
                    ST_UNKNOWN
                    );
        break;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    case SM_EVN_VS_HDCP_CONTENT:
    case SM_EVN_VS_MACROVISION:
    case SM_EVN_BYPASS_HDCP_CONTENT:
    case SM_EVN_BYPASS_MACROVISION:
        svctx_handle_copy_protection(
                pt_svctx,
                pt_strm_req,
                pt_strm_msg);

        if (pt_strm_msg->e_event == SM_EVN_BYPASS_HDCP_CONTENT)
        {
            inp_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    (pt_strm_msg->uinfo.ui4_data ? SVCTX_NTFY_CODE_HDCP_DETECT : SVCTX_NTFY_CODE_HDCP_NOT_DETECT),
                    pt_strm_req
                    );
        }
        else if (pt_strm_msg->e_event == SM_EVN_BYPASS_MACROVISION)
        {
            inp_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    (pt_strm_msg->uinfo.ui4_data ? SVCTX_NTFY_CODE_MACROVISION_DETECT : SVCTX_NTFY_CODE_MACROVISION_NOT_DETECT),
                    pt_strm_req
                    );
        }
        break;
#endif
    case SM_EVN_VS_AUD_PROTECT_INFO:
        switch( pt_strm_msg->uinfo.e_aud_protect_info )
        {
        case VSH_AUD_PROTECT_INFO_DMUTE_AMUTE:
            e_audio_protect_info = SCC_PROTECT_INFO_DMUTE_AMUTE;
            break;
        case VSH_AUD_PROTECT_INFO_DUNMUTE_AMUTE:
            e_audio_protect_info = SCC_PROTECT_INFO_DUNMUTE_AMUTE;
            break;
        case VSH_AUD_PROTECT_INFO_DUNMUTE_AUNMUTE:
            e_audio_protect_info = SCC_PROTECT_INFO_DUNMUTE_AUNMUTE;
            break;
        case VSH_AUD_PROTECT_INFO_DMUTE_AUNMUTE:
            e_audio_protect_info = SCC_PROTECT_INFO_DMUTE_AUNMUTE;
            break;
        default:
            e_audio_protect_info = SCC_PROTECT_INFO_UNKNOWN;
            break;
        }
        
        i4_ret = x_scc_aud_set_protect_info( pt_svctx->h_audio_scc_comp, e_audio_protect_info );
        if(i4_ret != SMR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"set audio protect information failed, ret=%d\r\n", i4_ret ));
            break;
        }
        
        switch( e_audio_protect_info )
        {
        case SCC_PROTECT_INFO_DMUTE_AMUTE:
            inp_hdlr_invoke_client_strm_nfy_fct( pt_svctx, SVCTX_NTFY_CODE_AUDIO_DMUTE_AMUTE, pt_strm_req );
            break;
        case SCC_PROTECT_INFO_DUNMUTE_AMUTE:
            inp_hdlr_invoke_client_strm_nfy_fct( pt_svctx, SVCTX_NTFY_CODE_AUDIO_DUNMUTE_AMUTE, pt_strm_req );
            break;
        case SCC_PROTECT_INFO_DUNMUTE_AUNMUTE:
            inp_hdlr_invoke_client_strm_nfy_fct( pt_svctx, SVCTX_NTFY_CODE_AUDIO_DUNMUTE_AUNMUTE, pt_strm_req );
            break;
        case SCC_PROTECT_INFO_DMUTE_AUNMUTE:
            inp_hdlr_invoke_client_strm_nfy_fct( pt_svctx, SVCTX_NTFY_CODE_AUDIO_DMUTE_AUNMUTE, pt_strm_req );
            break;
        default:
            break;
        }
       
        break;
    default:
        break;
    }
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_get_strm_req_from_handle
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_get_strm_req_from_handle(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    HANDLE_T                    h_stream,
#ifdef MW_TV_AV_BYPASS_SUPPORT
                    STRM_REQ_T**                ppt_strm_req,
                    STREAM_TYPE_T*              pe_strm_type
#else
                    STRM_REQ_T**                ppt_strm_req
#endif
                    )
{
    STREAM_TYPE_T e_strm_type;
    UINT8         ui1_idx;

    if (h_stream == NULL_HANDLE) {
        return SVCTXR_INV_HANDLE;
    }

    for (ui1_idx = 0; ui1_idx < INP_STRM_TYPE_COUNT; ui1_idx++) {
        e_strm_type = e_g_inp_strm_types[ui1_idx];
        if ((e_strm_type == ST_UNKNOWN) || ((UINT32)e_strm_type>=INP_STRM_TYPE_COUNT)) {
            continue;
        }

        if (pt_running_svc_req->t_strm_reqs[e_strm_type].h_stream != h_stream) {
            continue;
        }

        *ppt_strm_req = &(pt_running_svc_req->t_strm_reqs[e_strm_type]);
#ifdef MW_TV_AV_BYPASS_SUPPORT
        *pe_strm_type = e_strm_type;
#endif
        return SVCTXR_OK;
    }

    return SVCTXR_INV_HANDLE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_strm_started_nty_as_presenting
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_strm_started_nty_as_presenting(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    STRM_REQ_T*                 pt_strm_req
                    )
{
    STREAM_TYPE_T e_strm_type = pt_strm_req->u.t_scdb.t_comp_id.e_type;
    INT32         i4_ret      = SVCTXR_OK;

    switch(pt_strm_req->e_penging_status) {
    case STRM_PENDING_COND_SELECT_STRM:

        if (e_strm_type != ST_UNKNOWN )
        {
            if ((UINT32)e_strm_type<INP_STRM_TYPE_COUNT)
            {
                if (pt_running_svc_req->pt_pending_strm_reqs[e_strm_type] != NULL)
                {
                    /* for CR DTV00311718 */
                    if((e_strm_type == ST_AUDIO) &&
                       (pt_running_svc_req->b_play_audio_from_video_device == TRUE) &&
                       (pt_running_svc_req->t_src_info.e_crnt_audio_type == pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_dev_type))
                    {
                        svctx_free_strm_req(pt_running_svc_req->pt_pending_strm_reqs[e_strm_type]);
                        pt_running_svc_req->pt_pending_strm_reqs[e_strm_type] = NULL;
                        pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;
                        
                        break;
                    }
                
                    i4_ret = x_sm_close_stream(pt_strm_req->h_stream);
                    break;
                }
            }
            else
            {
                ASSERT(0, ("%s(), e_strm_type=%d out of boundary\r\n", __func__, e_strm_type));
            }
        }
        inp_hdlr_invoke_client_strm_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_STREAM_STARTED,
                        pt_strm_req
                        );
        pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;
        if (e_strm_type == ST_VIDEO) {
            i4_ret = _inp_hdlr_video_strm_post_start_handler(pt_svctx, pt_running_svc_req);
        }
        break;
    case STRM_PENDING_COND_STOP_STRM:
        i4_ret = x_sm_close_stream(pt_strm_req->h_stream);
        break;
    case STRM_PENDING_COND_NONE:
        inp_hdlr_invoke_client_strm_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_STREAM_STARTED,
                        pt_strm_req
                        );
        if (e_strm_type == ST_VIDEO) {
            i4_ret = _inp_hdlr_video_strm_post_start_handler(pt_svctx, pt_running_svc_req);
        }
        break;
    default:
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_strm_started_nty_as_pending
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_strm_started_nty_as_pending(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    STRM_REQ_T*                 pt_strm_req
                    )
{
    inp_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_STREAM_STARTED,
                    pt_strm_req
                    );
    if (pt_strm_req->u.t_scdb.t_comp_id.e_type == ST_VIDEO) {
        _inp_hdlr_video_strm_post_start_handler(pt_svctx, pt_running_svc_req);
    }
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_reset_strm_data
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static VOID _inp_hdlr_reset_strm_data(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    STRM_REQ_T*                 pt_strm_req
                    )
{
    INP_SVC_REQ_T* pt_pending_svc_req = NULL;
    switch (pt_strm_req->u.t_scdb.t_comp_id.e_type) {
    case ST_VIDEO:
        if (pt_running_svc_req->b_replace_video == FALSE) {
            pt_running_svc_req->t_src_info.ui2_crnt_video_combi_index = INVALID_CONN_COMBI_INDEX;
            pt_running_svc_req->t_src_info.e_crnt_video_type          = DEV_UNKNOWN;
        }
        break;
    case ST_AUDIO:
        if (pt_running_svc_req->b_replace_audio == FALSE) {
            pt_running_svc_req->t_src_info.ui2_crnt_audio_combi_index = INVALID_CONN_COMBI_INDEX;
            pt_running_svc_req->t_src_info.e_crnt_audio_type          = DEV_UNKNOWN;
            pt_pending_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_pending_svc_req;
            if ((pt_pending_svc_req != NULL) && (pt_pending_svc_req->t_parent.e_src_type == SRC_TYPE_AVC))
            {
                pt_pending_svc_req->t_src_info.ui2_crnt_audio_combi_index = INVALID_CONN_COMBI_INDEX;
                pt_pending_svc_req->t_src_info.e_crnt_audio_type          = DEV_UNKNOWN;
            }
        }
        break;
    default:
        break;
    }

    pt_strm_req->h_stream         = NULL_HANDLE;
    pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;

    if (pt_strm_req->u.t_scdb.t_comp_id.e_type != ST_UNKNOWN) {
        if (pt_svctx->ui4_frz_frozen_strm_mask & MAKE_BIT_MASK_32(pt_strm_req->u.t_scdb.t_comp_id.e_type)) {
            return;
        }
    }

    pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_check_session_is_ready
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
BOOL _inp_hdlr_check_session_is_ready(
                SM_SESS_COND_T                  e_sess_cond
                )
{
    if (e_sess_cond == SM_SESS_COND_STARTED
            || e_sess_cond == SM_SESS_COND_STARTING
            || e_sess_cond == SM_SESS_COND_OPENED
            || e_sess_cond == SM_SESS_COND_OPENED) {
        return TRUE;
    } else {
        return FALSE;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_reselect_strm
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_reselect_strm(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    STREAM_TYPE_T               e_strm_type
                    )
{
    SM_SESS_COND_T e_sess_cond;
    INT32          i4_ret = SVCTXR_OK;

    switch (e_strm_type) {
    case ST_VIDEO:
        if (pt_running_svc_req->b_replace_video == FALSE) {
            break;
        }

        if (pt_svctx->b_in_stopping == TRUE) {
            break;
        }

        if (player_get_conn_cond(pt_running_svc_req->pt_video_player) != CM_COND_CONNECTED) {
            break;
        }

        e_sess_cond = player_get_sess_cond(pt_running_svc_req->pt_video_player);
        if (_inp_hdlr_check_session_is_ready(e_sess_cond) == FALSE) {
            break;
        }

        i4_ret = _inp_hdlr_play_strm(
                    pt_svctx,
                    pt_running_svc_req,
                    pt_running_svc_req->pt_video_player,
                    ST_VIDEO,
                    (e_sess_cond == SM_SESS_COND_STARTED ? TRUE : FALSE)
                    );
        if (i4_ret == SVCTXR_OK) {
            pt_running_svc_req->b_replace_video = FALSE;
        }
        break;
    case ST_AUDIO:
        if (pt_running_svc_req->b_replace_audio == FALSE) {
            break;
        }

        if (pt_svctx->b_in_stopping == TRUE) {
            break;
        }

        i4_ret = inp_hdlr_check_to_play_audio_from_video_player(pt_svctx, pt_running_svc_req);
        if (i4_ret != SVCTXR_NO_ACTION) {
            break;
        }

        if ((pt_running_svc_req->t_parent.ui4_stream_mode & ST_MASK_AUDIO) == 0) {
            break;
        }

        if (player_get_conn_cond(pt_running_svc_req->pt_audio_player) != CM_COND_CONNECTED) {
            i4_ret = inp_hdlr_play_audio_player (
                            pt_svctx,
                            pt_running_svc_req
                            );
            if (i4_ret != SVCTXR_OK) {
                break;
            }
        }

        e_sess_cond = player_get_sess_cond(pt_running_svc_req->pt_audio_player);
        if (_inp_hdlr_check_session_is_ready(e_sess_cond) == FALSE) {
            break;
        }

        i4_ret = _inp_hdlr_play_strm(
                    pt_svctx,
                    pt_running_svc_req,
                    pt_running_svc_req->pt_audio_player,
                    ST_AUDIO,
                    (e_sess_cond == SM_SESS_COND_STARTED ? TRUE : FALSE)
                    );
        if (i4_ret == SVCTXR_OK) {
            pt_running_svc_req->b_replace_audio = FALSE;
        }
        break;
    default:
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_strm_closed
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_strm_closed(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    STRM_REQ_T*                 pt_strm_req
                    )
{
    STREAM_TYPE_T e_strm_type = pt_strm_req->u.t_scdb.t_comp_id.e_type;
    INT32         i4_ret      = SVCTXR_OK;
    PLAYER_T*     pt_player   = NULL;

    switch(pt_strm_req->e_penging_status) {
    case STRM_PENDING_COND_SELECT_STRM:
        if (e_strm_type == ST_UNKNOWN) {
            pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;
            break;
        }

        if ((UINT32)e_strm_type >= INP_STRM_TYPE_COUNT)
        {
            ASSERT(0, ("%s(), e_strm_type=%d out of boundary\r\n", __func__, e_strm_type));
            pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;
            break;
        }

        if (pt_running_svc_req->pt_pending_strm_reqs[e_strm_type] != NULL) {
            *pt_strm_req = *(pt_running_svc_req->pt_pending_strm_reqs[e_strm_type]);
            svctx_free_strm_req(pt_running_svc_req->pt_pending_strm_reqs[e_strm_type]);
            pt_running_svc_req->pt_pending_strm_reqs[e_strm_type] = NULL;

            if (e_strm_type == ST_AUDIO) {
                if (pt_running_svc_req->b_play_audio_from_video_device == TRUE) {
                    pt_player = pt_running_svc_req->pt_video_player;
                } else {
                    pt_player = pt_running_svc_req->pt_audio_player;
                }
            } else {
                pt_player = pt_running_svc_req->pt_video_player;
            }


            i4_ret = inp_hdlr_open_strm(
                            pt_svctx,
                            pt_running_svc_req,
                            pt_player,
                            pt_strm_req,
                            TRUE
                            );
            if (i4_ret == SVCTXR_OK) {
                pt_strm_req->e_penging_status = STRM_PENDING_COND_SELECT_STRM;
            } else {
                _inp_hdlr_reset_strm_data(
                    pt_svctx,
                    pt_running_svc_req,
                    pt_strm_req
                    );
            }
            break;
        }
        /* on purpose to without break command */
    case STRM_PENDING_COND_STOP_STRM:
    default:
        inp_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_STREAM_STOPPED,
                    pt_strm_req
                    );
        _inp_hdlr_reset_strm_data(
                    pt_svctx,
                    pt_running_svc_req,
                    pt_strm_req
                    );
        i4_ret = _inp_hdlr_handle_reselect_strm(
                    pt_svctx,
                    pt_running_svc_req,
                    e_strm_type
                    );
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_strm_msg
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_strm_msg (
                    SVCTX_T*                    pt_svctx,
                    SVCTX_STRM_MSG_T*           pt_strm_msg
                    )
{
    STRM_REQ_T*    pt_strm_req        = NULL;
    INP_SVC_REQ_T* pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    INT32          i4_ret             = SVCTXR_OK;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    STREAM_TYPE_T  e_strm_type;
#endif

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    i4_ret = _inp_hdlr_get_strm_req_from_handle(
                    pt_svctx,
                    pt_running_svc_req,
                    pt_strm_msg->h_obj,
#ifdef MW_TV_AV_BYPASS_SUPPORT
                    &pt_strm_req,
                    &e_strm_type
#else
                    &pt_strm_req
#endif
                    );
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    _inp_hdlr_handle_strm_event(
                    pt_svctx,
                    pt_running_svc_req,
                    pt_strm_req,
                    pt_strm_msg
                    );
    if (pt_strm_req->e_strm_cond == pt_strm_msg->e_cond) {
        return i4_ret;
    }

    pt_strm_req->e_strm_cond = pt_strm_msg->e_cond;

    switch(pt_strm_req->e_strm_cond) {
    case SM_COND_STARTED:
#ifdef MW_TV_AV_BYPASS_SUPPORT
        i4_ret = svctx_mgr_check_queued_req(
            ACTIVE_LINK_NOW,
            pt_svctx->pt_running_svc_req->e_src_type,
            NULL,
            FALSE);

        if (ST_VIDEO == e_strm_type)
        {
            svctx_enable_slave_audio(pt_svctx, SLAVE_AUDIO_CTRL_ENABLE);
        }
        else if (ST_AUDIO == e_strm_type)
        {
            svctx_ctrl_main_audio(pt_svctx, FALSE);
        }
#endif
        if (pt_svctx->e_state == SVCTX_COND_PRESENTING) {
            i4_ret = _inp_hdlr_handle_strm_started_nty_as_presenting(pt_svctx, pt_running_svc_req, pt_strm_req);
        } else if (pt_svctx->e_state == SVCTX_COND_PENDING) {
            i4_ret = _inp_hdlr_handle_strm_started_nty_as_pending(pt_svctx, pt_running_svc_req, pt_strm_req);
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

        if (pt_svctx->e_state == SVCTX_COND_PRESENTING || pt_svctx->e_state == SVCTX_COND_PENDING) {
            i4_ret = _inp_hdlr_handle_strm_closed(pt_svctx, pt_running_svc_req, pt_strm_req);
        }
        break;
    default:
        break;
    }
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_iterate_dev_status
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _inp_hdlr_iterate_dev_status(
                    AV_MONITOR_T*               pt_this,
                    UINT16                      ui2_num_dev,
                    UINT16                      ui2_idx,
                    DEVICE_TYPE_T               e_dev_type,
                    AVM_SIGNAL_STATUS_T         e_video_signal_status, /* cable status if QUERY_TYPE_CABLE */
                    AVM_SIGNAL_STATUS_T         e_audio_signal_status,
                    VOID*                       pv_tag
                    )
{
    INP_SRC_AVM_QUERY_INFO_T* pt_info        = (INP_SRC_AVM_QUERY_INFO_T*)pv_tag;
    INT8                      i1_crnt_order  = ORDER_UNKNOWN;
    INT8                      i1_first_order = ORDER_UNKNOWN;

    if (ui2_idx == 0) {
        pt_info->ui2_first_index  = ui2_num_dev;
        pt_info->e_first_dev_type = DEV_UNKNOWN;

        if (pt_info->e_query_type == QUERY_TYPE_CABLE
                || pt_info->e_query_type == QUERY_TYPE_VIDEO) {
            pt_info->b_worse_quality_enable = TRUE;
        }
    }

    if ((pt_info->e_query_type == QUERY_TYPE_VIDEO && e_video_signal_status == AVM_SIGNAL_STATUS_WITH_SIGNAL)
            || (pt_info->e_query_type == QUERY_TYPE_AUDIO && e_audio_signal_status == AVM_SIGNAL_STATUS_WITH_SIGNAL)
            || (pt_info->e_query_type == QUERY_TYPE_CABLE && e_video_signal_status == AVM_SIGNAL_STATUS_WITH_CABLE_PLUG_IN)) {
        switch (pt_info->u.t_hint.e_type) {
        case SRC_AVC_HINT_NONE:
            pt_info->ui2_target_index = ui2_idx;
            pt_info->e_target_type    = e_dev_type;
            return SVCTXR_DONE;
        case SRC_AVC_HINT_SPECIFIC_DEVICE_FIRST:
        case SRC_AVC_HINT_SPECIFIC_DEVICE_ONLY:
            if (pt_info->u.t_hint.e_dev_type == e_dev_type) {
                pt_info->ui2_target_index       = ui2_idx;
                pt_info->e_target_type          = e_dev_type;
                pt_info->b_worse_quality_enable = FALSE;
                return SVCTXR_DONE;
            }

            if (pt_info->ui2_first_index == ui2_num_dev
                    || pt_info->b_worse_quality_enable == TRUE) {
                pt_info->ui2_first_index        = ui2_idx;
                pt_info->e_first_dev_type       = e_dev_type;
                pt_info->b_worse_quality_enable = FALSE;
            }
            break;
        case SRC_AVC_HINT_BETTER_QUALITY_DEVICE_FIRST:
            if (pt_info->ui2_first_index == ui2_num_dev) {
                pt_info->ui2_first_index        = ui2_idx;
                pt_info->e_first_dev_type       = e_dev_type;
                pt_info->b_worse_quality_enable = FALSE;
                break;
            }

            if (pt_info->b_worse_quality_enable == TRUE) {
                pt_info->ui2_first_index  = ui2_num_dev;
                pt_info->e_first_dev_type = DEV_UNKNOWN;
            }

            svctx_get_dev_quality_orders(
                            pt_info->e_query_type,
                            e_dev_type,
                            pt_info->e_first_dev_type,
                            &i1_crnt_order,
                            &i1_first_order
                            );

            if (i1_crnt_order > i1_first_order) {
                pt_info->ui2_first_index        = ui2_idx;
                pt_info->e_first_dev_type       = e_dev_type;
                pt_info->b_worse_quality_enable = FALSE;
            }

            break;
        default:
            break;
        }
    } else if ((pt_info->e_query_type == QUERY_TYPE_CABLE && e_video_signal_status == AVM_SIGNAL_STATUS_UNKNOWN)
                    || pt_info->e_query_type == QUERY_TYPE_VIDEO) {
        /* tricky: for Strong S-first, try to use worse quality first, since GPIO is only for higher quality device
         * weiderchang 3/6/2008
         */
        if (pt_info->b_worse_quality_enable == TRUE) {
            svctx_get_dev_quality_orders(
                            QUERY_TYPE_VIDEO,
                            e_dev_type,
                            pt_info->e_first_dev_type,
                            &i1_crnt_order,
                            &i1_first_order
                            );

            if (i1_crnt_order < i1_first_order || i1_first_order == ORDER_UNKNOWN) {
                pt_info->b_worse_quality_enable = TRUE;
                pt_info->ui2_first_index        = ui2_idx;
                pt_info->e_first_dev_type       = e_dev_type;
            }
        }
    }

    /* last device, find nothing */
    if ((ui2_idx + ((UINT16)1)) == ui2_num_dev && pt_info->ui2_first_index < ui2_num_dev) {
        if (pt_info->u.t_hint.e_type != SRC_AVC_HINT_SPECIFIC_DEVICE_ONLY) {
            pt_info->ui2_target_index = pt_info->ui2_first_index;
            pt_info->e_target_type    = pt_info->e_first_dev_type;
            return SVCTXR_DONE;
        }
    }

    return SVCTXR_CONTINUE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_video_avm_signal_loss
 * Description
 *      The API is used to handle the priority between input source block,
 *      program block, and signal loss.
 * Input arguments
 *      pt_svctx                        The service context object
 *      pt_svc_req                      The service request to check
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static VOID _inp_hdlr_handle_video_avm_signal_loss(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_svc_req
                    )
{
    SVC_BLOCK_COND_T e_block_cond;

    if (pt_svc_req == NULL) {
        return;
    }

    e_block_cond = rating_chker_get_block_cond(pt_svc_req->pt_rating_chker);

    switch(pt_svctx->e_block_rule) {
    case SVCTX_BLOCK_RULE_INP_SIGNAL_LOSS_CH_PROG:
    case SVCTX_BLOCK_RULE_INP_CH_SIGNAL_LOSS_PROG:
        if (e_block_cond == SVC_BLOCK_COND_USER_BLOCK_INP) {
            break;
        }

        if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOCKED) {
            /* it is caused from S-Video signal cable is pulled out,
             * and it needs more time to confirm the CVBS with signal,
             * even driver already report No signal!
             */
            svctx_set_signal_loss_chk_delay(
                        pt_svctx,
                        DEFAULT_COMBI_SIGNAL_LOSS_CHK_TIMEOUT
                        );
        }

        inp_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_SIGNAL_LOSS,
                        ST_UNKNOWN
                        );
        break;
    case SVCTX_BLOCK_RULE_INP_CH_PROG_SIGNAL_LOSS:
        if (e_block_cond == SVC_BLOCK_COND_USER_BLOCK_INP
            || e_block_cond == SVC_BLOCK_COND_EXCEED_RATING) {
            break;
        }

        if (_inp_hdlr_is_blocked_service(
                    pt_svctx,
                    pt_svc_req,
                    SVC_BLOCK_CHECK_ALL,
                    TRUE) == TRUE) {
            _inp_hdlr_block_service(pt_svctx, pt_svc_req);
            pt_svctx->e_signal_status = SIGNAL_STATUS_LOSS;
        } else {
            if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOCKED) {
                /* it is caused from S-Video signal cable is pulled out,
                 * and it needs more time to confirm the CVBS with signal,
                 * even driver already report No signal!
                 */
                svctx_set_signal_loss_chk_delay(
                            pt_svctx,
                            DEFAULT_COMBI_SIGNAL_LOSS_CHK_TIMEOUT
                            );
            }

            inp_hdlr_invoke_client_svc_nfy_fct(
                            pt_svctx,
                            SVCTX_NTFY_CODE_SIGNAL_LOSS,
                            ST_UNKNOWN
                            );

        }
        break;
    default:
        break;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_video_player_avm_nty
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_video_player_avm_nty(
                    AV_MONITOR_T*               pt_avm,
                    AVM_NTFY_CODE_T             e_code,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data
                    )
{
    SVCTX_T*       pt_svctx           = (SVCTX_T*) pv_tag;
    INP_SVC_REQ_T* pt_running_svc_req = NULL;
    INT32          i4_ret             = SVCTXR_OK;

    ASSERT(pt_svctx != NULL, ("%s(), pt_svctx is NULL",__func__));

    pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    switch(e_code) {
    case AVM_NTFY_CODE_CABLE_STATUS:
    case AVM_NTFY_CODE_VIDEO_SIGNAL_STATUS:
    {
        INP_SRC_AVM_QUERY_INFO_T t_info = {0};
#ifdef MW_TV_AV_BYPASS_SUPPORT
#ifdef CLI_LVL_ALL
        DBG_SVCTXMGR_MSG((
            "\r\n{SVCTX_MGR} (%s) AVM video notify (%d).\r\n\r\n",
            pt_svctx->s_name,
            ui4_data));
#endif
        if (IS_SVCTX_ASSOCIATE(pt_svctx))
        {
            INP_SVC_REQ_T*  pt_lead_svc_req;
            SVCTX_T*        pt_lead;

            pt_lead = svctx_mgr_get_link_lead(
                ACTIVE_LINK_NOW, pt_svctx->pt_running_svc_req->e_src_type);

            if (pt_lead)
            {
                pt_lead_svc_req = (INP_SVC_REQ_T*) (pt_lead->pt_running_svc_req);

                t_info.ui2_target_index =
                    pt_lead_svc_req->t_src_info.ui2_crnt_video_combi_index;
                t_info.e_target_type =
                    pt_lead_svc_req->t_src_info.e_crnt_video_type;
            }
            else
            {
                break;
            }
        }
        else
        {
#endif
        if (player_get_conn_cond(pt_running_svc_req->pt_video_player) != CM_COND_DISCONNECTED) {
            if (pt_running_svc_req->t_src_info.t_av_src.t_video_hint.e_type == SRC_AVC_HINT_NONE
                    || pt_running_svc_req->t_src_info.t_av_src.t_video_hint.e_type == SRC_AVC_HINT_SPECIFIC_DEVICE_ONLY) {
                if (pt_running_svc_req->t_src_info.e_crnt_video_type != DEV_UNKNOWN) {
                    break;
                }
            }
        }

        t_info.e_query_type = (e_code == AVM_NTFY_CODE_CABLE_STATUS) ? QUERY_TYPE_CABLE : QUERY_TYPE_VIDEO;
        t_info.u.t_hint     = pt_running_svc_req->t_src_info.t_av_src.t_video_hint;

        i4_ret = avm_iterate_devs(
                    pt_running_svc_req->pt_video_avm,
                    _inp_hdlr_iterate_dev_status,
                    &t_info
                    );
        if (i4_ret != SVCTXR_DONE) {
            _inp_hdlr_handle_video_avm_signal_loss(pt_svctx, pt_running_svc_req);
            pt_svctx->e_signal_status = SIGNAL_STATUS_LOSS;
            if (pt_svctx->b_is_blocked == TRUE) {
                inp_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOSS_AS_BLOCKED, ST_UNKNOWN);
            }

            inp_hdlr_stop_video_player(
                    pt_svctx,
                    pt_running_svc_req,
                    FALSE
                    );

            if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) == FALSE
                    && (pt_running_svc_req->t_parent.ui4_stream_mode & ST_MASK_AUDIO) > 0
                    && pt_running_svc_req->b_play_audio_from_video_device == FALSE) {
                inp_hdlr_play_audio_player(pt_svctx, pt_running_svc_req);
            }
            break;
        }
#ifdef MW_TV_AV_BYPASS_SUPPORT
        }
#endif

        if (t_info.ui2_target_index == pt_running_svc_req->t_src_info.ui2_crnt_video_combi_index
                && t_info.e_target_type == pt_running_svc_req->t_src_info.e_crnt_video_type) {
            if (player_get_conn_cond(pt_running_svc_req->pt_video_player) != CM_COND_DISCONNECTED) {
                break;
            }
#ifdef MW_TV_AV_BYPASS_SUPPORT
            if (pt_running_svc_req->b_combi_switch)
            {
                break;
            }
#endif
        }

        if (pt_running_svc_req->t_src_info.t_av_src.e_video_type == DEV_AVC_COMBI
                && pt_svctx->e_detection_mode == DETECTION_MODE_BY_SIGNAL_STATUS
                && pt_running_svc_req->t_src_info.e_crnt_video_type != DEV_UNKNOWN
                && pt_running_svc_req->t_src_info.e_crnt_video_type != t_info.e_target_type) {
            /* it is caused from S-Video signal cable is pulled out,
             * and it needs more time to confirm the CVBS with signal,
             * even driver already report No signal!
             */
            svctx_set_signal_loss_chk_delay(pt_svctx, DEFAULT_COMBI_SIGNAL_LOSS_CHK_TIMEOUT);
        }

        if (pt_svctx->ui4_frz_frozen_strm_mask != 0) {
            pt_svctx->ui4_frz_frozen_strm_mask = 0;
            svctx_set_video_plane(pt_svctx, pt_svctx->e_default_vid_mode);
            inp_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_UNFREEZE,
                        ST_UNKNOWN
                        );
        }

#ifdef MW_TV_AV_BYPASS_SUPPORT
        pt_running_svc_req->b_combi_switch = TRUE;
#endif
        pt_running_svc_req->t_src_info.ui2_crnt_video_combi_index = t_info.ui2_target_index;
        pt_running_svc_req->t_src_info.e_crnt_video_type          = t_info.e_target_type;

        /* the video is swapping in COMBI, should set the signal status to SIGNAL_STATUS_UNKNOWN */
#ifdef MW_TV_AV_BYPASS_SUPPORT
        svctx_reset_copy_protection_status_params(pt_svctx);
#endif
        pt_svctx->e_signal_status = SIGNAL_STATUS_UNKNOWN;
        inp_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_INTERRUPTED,
                        ST_UNKNOWN
                        );

#ifdef MW_TV_AV_BYPASS_SUPPORT
#ifdef CLI_LVL_ALL
        DBG_SVCTXMGR_MSG((
            "\r\n{SVCTX_MGR} (%s) Stop video player. combi_index = %d, video_type = 0x%x\r\n",
            pt_svctx->s_name,
            t_info.ui2_target_index,
            t_info.e_target_type));
#endif
#endif
        i4_ret = inp_hdlr_stop_video_player(pt_svctx, pt_running_svc_req, FALSE);
        if (i4_ret == SVCTXR_OK) {
#ifdef MW_TV_AV_BYPASS_SUPPORT
#ifdef CLI_LVL_ALL
            DBG_SVCTXMGR_MSG((
                "\r\n{SVCTX_MGR} (%s) Start video player. combi_index = %d, video_type = 0x%x\r\n\r\n",
                pt_svctx->s_name,
                t_info.ui2_target_index,
                t_info.e_target_type));
#endif
#endif
            i4_ret = inp_hdlr_play_video_player(pt_svctx, pt_running_svc_req);
        } else if (i4_ret == SVCTXR_PENDING) {
#ifdef MW_TV_AV_BYPASS_SUPPORT
#ifdef CLI_LVL_ALL
            DBG_SVCTXMGR_MSG((
                "\r\n{SVCTX_MGR} (%s) Pending...set replace_video flag. combi_index = %d, video_type = 0x%x\r\n\r\n",
                pt_svctx->s_name,
                t_info.ui2_target_index,
                t_info.e_target_type));
#endif
#endif
            pt_running_svc_req->b_replace_video = TRUE;
        }
        break;
    }
    case AVM_NTFY_CODE_AUDIO_SIGNAL_STATUS:
        i4_ret = inp_hdlr_check_to_play_audio_from_video_player(
                    pt_svctx,
                    pt_running_svc_req
                    );
        break;
    case AVM_NTFY_CODE_REQ_SEND_MSG:
        i4_ret = svctx_send_msg(pt_svctx->h_sys_svctx, (SVCTX_MSG_T*)ui4_data);
        break;
    default:
        break;
    }
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_audio_player_avm_nty
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_audio_player_avm_nty(
                    AV_MONITOR_T*               pt_avm,
                    AVM_NTFY_CODE_T             e_code,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data
                    )
{
    SVCTX_T*       pt_svctx           = (SVCTX_T*) pv_tag;
    INP_SVC_REQ_T* pt_running_svc_req = NULL;
    INT32          i4_ret             = SVCTXR_OK;

    ASSERT(pt_svctx != NULL, ("%s(), pt_svctx is NULL",__func__));

    pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    switch(e_code) {
    case AVM_NTFY_CODE_AUDIO_SIGNAL_STATUS:
    {
        INP_SRC_AVM_QUERY_INFO_T t_info = {0};

        if (player_get_conn_cond(pt_running_svc_req->pt_audio_player) != CM_COND_DISCONNECTED) {
            if (pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_type == SRC_AVC_HINT_NONE
                    || pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_type == SRC_AVC_HINT_SPECIFIC_DEVICE_ONLY) {
                if (pt_running_svc_req->t_src_info.e_crnt_audio_type != DEV_UNKNOWN) {
                    break;
                }
            }
        }

        t_info.e_query_type = QUERY_TYPE_AUDIO;
        t_info.u.t_hint     = pt_running_svc_req->t_src_info.t_av_src.t_audio_hint;

        i4_ret = avm_iterate_devs(
                    pt_running_svc_req->pt_audio_avm,
                    _inp_hdlr_iterate_dev_status,
                    &t_info
                    );
        if (i4_ret != SVCTXR_DONE) {
            inp_hdlr_stop_audio_player (
                    pt_svctx,
                    pt_running_svc_req
                    );
            break;
        }

        if (t_info.ui2_target_index == pt_running_svc_req->t_src_info.ui2_crnt_audio_combi_index
                && t_info.e_target_type == pt_running_svc_req->t_src_info.e_crnt_audio_type) {
            if (player_get_conn_cond(pt_running_svc_req->pt_audio_player) != CM_COND_DISCONNECTED) {
                break;
            }
        }

        pt_running_svc_req->t_src_info.ui2_crnt_audio_combi_index = t_info.ui2_target_index;
        pt_running_svc_req->t_src_info.e_crnt_audio_type          = t_info.e_target_type;

        i4_ret = inp_hdlr_stop_audio_player(pt_svctx, pt_running_svc_req);
        if (i4_ret == SVCTXR_OK) {
            i4_ret = inp_hdlr_play_audio_player(pt_svctx, pt_running_svc_req);
        } else if (i4_ret == SVCTXR_PENDING) {
            pt_running_svc_req->b_replace_audio = TRUE;
        }
        break;
    }
    case AVM_NTFY_CODE_VIDEO_SIGNAL_STATUS:
        break;
    case AVM_NTFY_CODE_REQ_SEND_MSG:
        i4_ret = svctx_send_msg(pt_svctx->h_sys_svctx, (SVCTX_MSG_T*)ui4_data);
        break;
    default:
        break;
    }
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_monitor_video_player
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_monitor_video_player (
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    INT32 i4_ret;

    i4_ret = avm_open (
                    pt_running_svc_req->t_src_info.s_src_grp_name,
                    pt_svctx->s_snk_grp_name,
                    pt_running_svc_req->t_src_info.t_av_src.e_video_type,
                    _inp_hdlr_handle_video_player_avm_nty,
                    pt_svctx,
                    &pt_running_svc_req->pt_video_avm
                    );
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    i4_ret = avm_set_detection_mode(
                    pt_running_svc_req->pt_video_avm,
                    pt_svctx->e_detection_mode
                    );
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    i4_ret = avm_start(pt_running_svc_req->pt_video_avm);

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_monitor_audio_player
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_monitor_audio_player (
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    INT32 i4_ret;

    i4_ret = avm_open (
                    pt_running_svc_req->t_src_info.s_src_grp_name,
                    NULL,
                    pt_running_svc_req->t_src_info.t_av_src.e_audio_type,
                    _inp_hdlr_handle_audio_player_avm_nty,
                    pt_svctx,
                    &pt_running_svc_req->pt_audio_avm
                    );
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    i4_ret = avm_start(pt_running_svc_req->pt_audio_avm);
    if (i4_ret != SVCTXR_OK) {
        DBG_ERROR((_ERROR_HEADER"%s() avm_start(0x%x) failed, i4_ret = %d\r\n",
                    __func__,
                    pt_running_svc_req->pt_audio_avm,
                    i4_ret
                    ));
    }

    /* for AUDIO_INP cannot detect signal, i.e. don't know it with or without signal
     * so, after start monitoring, try to signal QQQQQQQQ
     */
    _inp_hdlr_handle_audio_player_avm_nty(
                    pt_running_svc_req->pt_audio_avm,
                    AVM_NTFY_CODE_AUDIO_SIGNAL_STATUS,
                    pt_svctx,
                    0
                    );

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_crnt_evn_nty
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_crnt_evn_nty(
                    CRNT_EVENT_T*               pt_crnt_evn,
                    CRNT_EVENT_NTFY_CODE_T      e_code,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data
                    )
{
    SVCTX_T*       pt_svctx = (SVCTX_T*) pv_tag;
    INT32          i4_ret   = SVCTXR_OK;
    INP_SVC_REQ_T* pt_running_svc_req;

    ASSERT(pt_svctx != NULL, ("%s(), pt_svctx is NULL",__func__));

    pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (pt_crnt_evn != pt_running_svc_req->pt_crnt_event) {
        return SVCTXR_FAIL;
    }

    switch(e_code) {
    case CRNT_EVENT_NTFY_CODE_START_QUERY:
        inp_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CRNT_EVN_START_QUERY,
                    ST_UNKNOWN
                    );
        break;
    case CRNT_EVENT_NTFY_CODE_EVN_NOT_READY:
        inp_hdlr_check_svc_block(pt_svctx);
        if (pt_svctx->e_block_rule == SVCTX_BLOCK_RULE_INP_SIGNAL_LOSS_CH_PROG
                || pt_svctx->e_block_rule == SVCTX_BLOCK_RULE_INP_CH_SIGNAL_LOSS_PROG) {
            inp_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CRNT_EVN_NOT_READY,
                    ST_UNKNOWN
                    );
        } else {
            inp_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CRNT_EVN_READY,
                    ST_UNKNOWN
                    );
        }
        break;
    case CRNT_EVENT_NTFY_CODE_EVN_READY:
        inp_hdlr_check_svc_block(pt_svctx);
        inp_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CRNT_EVN_READY,
                    ST_UNKNOWN
                    );
        break;
    case CRNT_EVENT_NTFY_CODE_REQ_SEND_MSG:
        i4_ret = svctx_send_msg(pt_svctx->h_sys_svctx, (SVCTX_MSG_T*)ui4_data);
        break;
    default:
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_handle_rating_chker_nty
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_handle_rating_chker_nty(
                    RATING_CHKER_T*             pt_rating_chker,
                    RATING_CHKER_NTFY_CODE_T    e_code,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data
                    )
{
    SVCTX_T* pt_svctx = (SVCTX_T*) pv_tag;
    INT32    i4_ret   = SVCTXR_OK;

    ASSERT(pt_svctx != NULL, ("%s(), pt_svctx is NULL",__func__));
    switch(e_code) {
    case RATING_CHKER_NTFY_CODE_REQ_SEND_MSG:
        i4_ret = svctx_send_msg(pt_svctx->h_sys_svctx, (SVCTX_MSG_T*)ui4_data);
        break;
    default:
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_is_blocked_service
 * Description
 *
 * Input arguments
 *      pt_svctx        the pointer points to an SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK       the routine is successfully
 *-----------------------------------------------------------------------------*/
static BOOL _inp_hdlr_is_blocked_service(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    SVC_BLOCK_CHECK_GUDIE_T     e_check_guide,
                    BOOL                        b_auto_notify_client
                    )
{
    SVC_BLOCK_COND_T e_old_block_cond = SVC_BLOCK_COND_PASSED;
    SVC_BLOCK_COND_T e_new_block_cond = SVC_BLOCK_COND_PASSED;
#ifndef MW_EDB_SUPPORT
    EVCTX_COND_T     e_evctx_cond;
#endif

    if (pt_running_svc_req == NULL) {
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
        if (e_check_guide & SVC_BLOCK_CHECK_RATING) 
        {        
#ifndef MW_EDB_SUPPORT
            if (svctx_with_event_capability(
                    pt_svctx,
                    pt_running_svc_req->t_src_info.e_crnt_video_type,
                    MAKE_BIT_MASK_32(EVCTX_KEY_TYPE_RATING)
                ) == TRUE) 
            {
                e_evctx_cond = crnt_evn_get_cond(pt_running_svc_req->pt_crnt_event);
                if ((pt_svctx->e_block_rule == SVCTX_BLOCK_RULE_INP_SIGNAL_LOSS_CH_PROG ||
                     pt_svctx->e_block_rule == SVCTX_BLOCK_RULE_INP_CH_SIGNAL_LOSS_PROG ) && 
                     e_evctx_cond == EVCTX_COND_TRANSITION ) 
                {
                    e_check_guide &= ~SVC_BLOCK_CHECK_RATING;
                }
            } 
            else 
            {
                e_check_guide &= ~SVC_BLOCK_CHECK_RATING;
            }
#else
            if (svctx_with_event_capability(
                    pt_svctx,
                    pt_running_svc_req->t_src_info.e_crnt_video_type,
                    MAKE_BIT_MASK_32(EDB_KEY_TYPE_RATING_LIST_BY_ID)
                 ) == TRUE) 
            {
                if ( pt_svctx->e_block_rule == SVCTX_BLOCK_RULE_INP_SIGNAL_LOSS_CH_PROG ||
                     pt_svctx->e_block_rule == SVCTX_BLOCK_RULE_INP_CH_SIGNAL_LOSS_PROG )
                {
                    e_check_guide &= ~SVC_BLOCK_CHECK_RATING;
                }
            } 
            else 
            {
                e_check_guide &= ~SVC_BLOCK_CHECK_RATING;
            }
#endif
        }

        e_new_block_cond = rating_chker_check(
                            pt_running_svc_req->pt_rating_chker,
                            pt_running_svc_req->t_src_info.s_src_grp_name,
                            SVL_NULL_REC_ID,
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
        inp_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_SERVICE_UNBLOCKED,
                    ST_UNKNOWN
                    );
        inp_hdlr_resend_crnt_evn_nfy(
                    pt_svctx,
                    pt_running_svc_req
                    );
        return FALSE;
    case SVC_BLOCK_COND_USER_BLOCK_CH:
    case SVC_BLOCK_COND_USER_BLOCK_INP:
    case SVC_BLOCK_COND_EXCEED_RATING:
    default:
        inp_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_SERVICE_BLOCKED,
                    ST_UNKNOWN
                    );
        return TRUE;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_block_service
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_block_service (
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    STRM_REQ_T* pt_cc_strm_req     = NULL;
    STRM_REQ_T* pt_audio_strm_req  = NULL;
    INT32       i4_ret             = SVCTXR_OK;

    svctx_set_av_status(pt_svctx, SVC_AV_STATUS_UNKNOWN);

    if (pt_running_svc_req->b_play_audio_from_video_device == TRUE) {
        pt_audio_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_AUDIO]);
        if (pt_audio_strm_req != NULL && x_handle_valid(pt_audio_strm_req->h_stream) == TRUE) {
            i4_ret = x_sm_close_stream(pt_audio_strm_req->h_stream);
            if (i4_ret != SMR_OK && i4_ret != SMR_PENDING) {
                return i4_ret;
            }
            pt_audio_strm_req->e_strm_cond = SM_COND_CLOSING;
            pt_running_svc_req->t_src_info.e_crnt_audio_type = DEV_UNKNOWN;
        }
    } else {
        i4_ret = inp_hdlr_stop_audio_player(pt_svctx, pt_running_svc_req);
        if (i4_ret != SVCTXR_OK && i4_ret != SVCTXR_PENDING) {
            return SVCTXR_FAIL;
        }
    }

    pt_svctx->e_state = SVCTX_COND_PENDING;

    if (!(SVCTX_IS_INVISIBLE_CC_FILTER(pt_running_svc_req->t_cc_filter))) {
        pt_cc_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_CLOSED_CAPTION]);
        if (pt_cc_strm_req != NULL && x_handle_valid(pt_cc_strm_req->h_stream) == TRUE) {
            i4_ret = x_sm_close_stream(pt_cc_strm_req->h_stream);
            if (i4_ret != SMR_OK && i4_ret != SMR_PENDING) {
                return SVCTXR_FAIL;
            }
            pt_cc_strm_req->e_strm_cond = SM_COND_CLOSING;
        }
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_check_video_stream
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_check_video_stream (
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    STRM_REQ_T* pt_video_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]);

    if (x_handle_valid(pt_video_strm_req->h_stream) == FALSE) {
        inp_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_NO_STREAM,
                    pt_video_strm_req
                    );
        return SVCTXR_OK;
    }

    if (pt_video_strm_req->e_strm_cond == SM_COND_STARTED) {
        inp_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_STREAM_STARTED,
                    pt_video_strm_req
                    );
    }
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_check_video_format
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_check_video_format (
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    if (svctx_is_valid_video_fmt(pt_svctx) == TRUE) {
        inp_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE,
                    &(pt_running_svc_req->t_strm_reqs[ST_VIDEO])
                    );
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_unblock_service
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_unblock_service (
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) {
        BOOL b_delay_video_mute_check = pt_svctx->b_delay_video_mute_check;

        pt_svctx->b_signal_loss_notified   = FALSE;
        pt_svctx->b_delay_video_mute_check = TRUE;
        inp_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOSS, ST_UNKNOWN);
        pt_svctx->b_delay_video_mute_check = b_delay_video_mute_check;

        if ((pt_svctx->ui1_stop_audio_as_no_signal & MAKE_BIT_MASK_8(pt_running_svc_req->t_parent.e_src_type)) == 0) {
            inp_hdlr_play_audio_stream(pt_svctx, pt_running_svc_req);
        }
    } else if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOCKED) {
        svctx_set_av_status(pt_svctx, SVC_AV_STATUS_UNKNOWN);
        inp_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOCKED, ST_UNKNOWN);
        svctx_update_video_info_from_sm(pt_svctx,pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream);
        _inp_hdlr_check_video_stream(pt_svctx, pt_running_svc_req);
        inp_hdlr_play_audio_stream(pt_svctx, pt_running_svc_req);
        inp_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_NORMAL, ST_UNKNOWN);
        _inp_hdlr_video_strm_post_start_handler(pt_svctx, pt_running_svc_req);
        _inp_hdlr_check_video_format(pt_svctx, pt_running_svc_req);
    }
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_prepare_block_check_stuff
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_prepare_block_check_stuff (
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    INT32 i4_ret;

    if (pt_running_svc_req->pt_crnt_event == NULL) {
        i4_ret = crnt_evn_open(
                        DEFAULT_REQUERY_EVN_DELAY,
                        pt_svctx->e_block_rule == SVCTX_BLOCK_RULE_INP_CH_PROG_SIGNAL_LOSS
                            ? FALSE : TRUE,
                        _inp_hdlr_handle_crnt_evn_nty,
                        pt_svctx,
                        &pt_running_svc_req->pt_crnt_event
                        );
        if (i4_ret != SVCTXR_OK) {
            return i4_ret;
        }
    }

    if (pt_svctx->pf_block_check_fct == NULL) {
        pt_svctx->pf_block_check_fct = svctx_null_block_check_fct;
#ifdef SVCTX_SPLIT_SUPPORT
        pt_svctx->pf_block_check_fct_tag = NULL;
#endif
    }

    if (pt_running_svc_req->pt_rating_chker == NULL) {
        i4_ret = rating_chker_open(
                        pt_svctx->pf_block_check_fct,
#ifdef SVCTX_SPLIT_SUPPORT
                        pt_svctx->pf_block_check_fct_tag,
#endif
                        0,
                        _inp_hdlr_handle_rating_chker_nty,
                        pt_svctx,
                        &pt_running_svc_req->pt_rating_chker
                        );
        if (i4_ret != SVCTXR_OK) {
            return i4_ret;
        }

        /* check inpur source lock first */
        _inp_hdlr_is_blocked_service(
                        pt_svctx,
                        pt_running_svc_req,
                        SVC_BLOCK_CHECK_USER_BLOCK_INP,
                        TRUE
                        );
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_check_svc_block
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 inp_hdlr_check_svc_block (
                    SVCTX_T*                    pt_svctx
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    BOOL           b_old_is_blocked   = pt_svctx->b_is_blocked;
    INT32          i4_ret             = SVCTXR_OK;

    if (pt_running_svc_req == NULL) {
        return SVCTXR_WRONG_STATE;
    }

    svctx_check_analog_tuner_out_block(
                                pt_svctx,
                                player_get_sess_handle(pt_running_svc_req->pt_video_player)
                                );

    if (b_old_is_blocked == _inp_hdlr_is_blocked_service(
                                pt_svctx,
                                pt_running_svc_req,
                                SVC_BLOCK_CHECK_ALL,
                                TRUE
                                )) {
        return i4_ret;
    }

    if (pt_svctx->b_is_blocked == TRUE) {
        i4_ret = _inp_hdlr_block_service(pt_svctx, pt_running_svc_req);
    } else {
        i4_ret = _inp_hdlr_unblock_service(pt_svctx, pt_running_svc_req);
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_select_svc
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 inp_hdlr_select_svc (
                    SVCTX_T*                    pt_svctx
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = NULL;
    INT32          i4_ret = SVCTXR_OK;

    pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    do {
        pt_running_svc_req->b_play_audio_from_video_device = FALSE;

        svctx_reset_video_info(pt_svctx);
        svctx_reset_audio_info(pt_svctx);

        svctx_update_src_snk_names(
                    pt_svctx,
                    pt_running_svc_req->t_src_info.s_src_grp_name,
                    pt_running_svc_req->t_parent.s_snk_grp_name
                    );
        _inp_hdlr_prepare_block_check_stuff(pt_svctx, pt_running_svc_req);

        if (pt_running_svc_req->t_parent.ui4_stream_mode & ST_MASK_VIDEO) {
            switch (pt_running_svc_req->t_src_info.t_av_src.e_video_type) {
            case DEV_AVC_COMBI:
                pt_running_svc_req->b_replace_video = FALSE;
                i4_ret = _inp_hdlr_monitor_video_player(pt_svctx, pt_running_svc_req);
                break;
            case DEV_UNKNOWN:
                i4_ret = SVCTXR_OK;
                break;
            default:
                pt_running_svc_req->t_src_info.ui2_crnt_video_combi_index = INVALID_CONN_COMBI_INDEX;
                pt_running_svc_req->t_src_info.e_crnt_video_type          = DEV_UNKNOWN;
                i4_ret = inp_hdlr_play_video_player(pt_svctx, pt_running_svc_req);
                break;
            }

            if (i4_ret < SVCTXR_OK) {
                return i4_ret;
            }
        }

        switch (pt_running_svc_req->t_src_info.t_av_src.e_audio_type) {
        case DEV_AVC_COMBI:
            if (pt_running_svc_req->t_parent.ui4_stream_mode & ST_MASK_AUDIO) {
                pt_running_svc_req->b_replace_audio = FALSE;
                i4_ret = _inp_hdlr_monitor_audio_player(pt_svctx, pt_running_svc_req);
            }
            break;
        case DEV_UNKNOWN:
            i4_ret = SVCTXR_OK;
            break;
        default:
            if (pt_running_svc_req->t_src_info.t_av_src.e_video_type != DEV_UNKNOWN) {
                if ((pt_svctx->ui1_stop_audio_as_no_signal & MAKE_BIT_MASK_8(pt_running_svc_req->t_parent.e_src_type)) > 0) {
                    inp_hdlr_stop_audio_player(pt_svctx, pt_running_svc_req);
                    i4_ret = SVCTXR_OK;
                    break;
                } else {
                    if (pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_dev_type == DEV_AVC_HDMI) {
                        /* if preferred HDMI, don't start the audio player until the video with/without signal */
                        inp_hdlr_stop_audio_player(pt_svctx, pt_running_svc_req);
                        i4_ret = SVCTXR_OK;
                        break;
                    }
                }
                /* if with video type, wait until SM_EVN_VS_ANA_TV_SIGNAL to play audio if AV is seperated
                 * to play, i.e. relative bit in pt_svctx->ui1_stop_audio_as_no_signal is '1'.
                 */
            } else {
                i4_ret = inp_hdlr_play_audio_player(pt_svctx, pt_running_svc_req);
            }
            break;
        }

        if (i4_ret < SVCTXR_OK) {
            return i4_ret;
        }
    } while (0);

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_stop_svc
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 inp_hdlr_stop_svc (
                    SVCTX_T*                    pt_svctx,
                    BOOL                        b_stop_only
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = NULL;
    INT32          i4_video_ret       = SVCTXR_OK;
    INT32          i4_audio_ret       = SVCTXR_OK;

    pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        return SVCTXR_ALREADY_STOPPED;
    }

    if (pt_svctx->e_state != SVCTX_COND_READY) {
        if (pt_svctx->pt_pending_svc_req != NULL) {
            inp_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_INTERRUPTED,
                        ST_UNKNOWN
                        );
        } else {
            inp_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_IN_STOPPING,
                        ST_UNKNOWN
                        );
        }
    }

    if (pt_running_svc_req->pt_video_avm != NULL) {
        i4_video_ret = avm_stop(pt_running_svc_req->pt_video_avm);
        if (i4_video_ret != SVCTXR_OK) {
            return i4_video_ret;
        }

        i4_video_ret = avm_close(pt_running_svc_req->pt_video_avm);
        if (i4_video_ret != SVCTXR_OK) {
            return i4_video_ret;
        }
        pt_running_svc_req->pt_video_avm = NULL;
    }

    if (pt_running_svc_req->pt_audio_avm != NULL) {
        i4_audio_ret = avm_stop(pt_running_svc_req->pt_audio_avm);
        if (i4_audio_ret != SVCTXR_OK) {
            return i4_audio_ret;
        }

        i4_audio_ret = avm_close(pt_running_svc_req->pt_audio_avm);
        if (i4_audio_ret != SVCTXR_OK) {
            return i4_audio_ret;
        }
        pt_running_svc_req->pt_audio_avm = NULL;
    }

    pt_running_svc_req->b_replace_audio = FALSE;
    pt_running_svc_req->b_replace_video = FALSE;

    i4_video_ret = inp_hdlr_stop_video_player(pt_svctx, pt_running_svc_req, TRUE);
    if (i4_video_ret != SVCTXR_OK && i4_video_ret != SVCTXR_PENDING) {
        return i4_video_ret;
    }

    i4_audio_ret = inp_hdlr_stop_audio_player(pt_svctx, pt_running_svc_req);
    if (i4_audio_ret != SVCTXR_OK && i4_audio_ret != SVCTXR_PENDING) {
        return i4_audio_ret;
    }

    return (i4_video_ret == SVCTXR_PENDING ? i4_video_ret : i4_audio_ret);
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_select_strm
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 inp_hdlr_select_strm (
                    SVCTX_T*                    pt_svctx,
                    SVCTX_MSG_T*                pt_msg
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = NULL;
    INT32          i4_ret             = SVCTXR_OK;
    STREAM_TYPE_T  e_strm_type        = pt_msg->u.pt_strm_req->u.t_scdb.t_comp_id.e_type;
    STRM_REQ_T*    pt_strm_req        = NULL;
    SM_SESS_COND_T e_sess_cond        = SM_SESS_COND_CLOSED;
    PLAYER_T*      pt_player          = NULL;

    pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        return SVCTXR_WRONG_STATE;
    }

    if (((UINT32)e_strm_type >= INP_STRM_TYPE_COUNT) || (e_strm_type==ST_UNKNOWN))
    {
        ASSERT(0, ("%s(), e_strm_type=%d out of boundary\r\n", __func__, e_strm_type));
        return SVCTXR_INV_ARG;
    }

    pt_strm_req = &(pt_running_svc_req->t_strm_reqs[e_strm_type]);
    pt_running_svc_req->t_parent.ui4_stream_mode |= MAKE_BIT_MASK_32(e_strm_type);

    switch(e_strm_type){
    case ST_CLOSED_CAPTION:
        if (!SVCTX_IS_INVISIBLE_CC_FILTER(pt_running_svc_req->t_cc_filter)) {
            if (pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED
                || pt_svctx->b_is_blocked == TRUE) {
                svctx_free_strm_req(pt_msg->u.pt_strm_req);
                return SVCTXR_WRONG_STATE;
            }
        }

        pt_player = pt_running_svc_req->pt_video_player;
        e_sess_cond = player_get_sess_cond(pt_player);
        if (svctx_is_able_to_select_stream(e_sess_cond) == FALSE) {
            return SVCTXR_WRONG_STATE;
        }

        pt_running_svc_req->h_gl_plane  = pt_msg->u.pt_strm_req->h_gl_plane;
        pt_running_svc_req->t_cc_filter = pt_msg->u.pt_strm_req->t_cc_filter;
        break;
    case ST_AUDIO:
        if (pt_svctx->b_is_blocked == TRUE) {
            svctx_free_strm_req(pt_msg->u.pt_strm_req);
            return SVCTXR_WRONG_STATE;
        }

        if (pt_running_svc_req->b_play_audio_from_video_device == TRUE) {
            pt_player = pt_running_svc_req->pt_video_player;
            pt_running_svc_req->t_src_info.e_crnt_audio_type = pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_dev_type;
        } else {
            pt_player = pt_running_svc_req->pt_audio_player;
        }

        e_sess_cond = player_get_sess_cond(pt_player);
        if (svctx_is_able_to_select_stream(e_sess_cond) == FALSE) {
            return SVCTXR_WRONG_STATE;
        }

        break;
    case ST_VIDEO:
      {
        STRM_REQ_T* pt_cc_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_CLOSED_CAPTION]);

        if (pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED
                || pt_svctx->b_is_blocked == TRUE) {
            svctx_free_strm_req(pt_msg->u.pt_strm_req);
            return SVCTXR_WRONG_STATE;
        }

        pt_player = pt_running_svc_req->pt_video_player;

        e_sess_cond = player_get_sess_cond(pt_player);
        if (svctx_is_able_to_select_stream(e_sess_cond) == FALSE) {
            return SVCTXR_WRONG_STATE;
        }

        if (pt_cc_strm_req != NULL && x_handle_valid(pt_cc_strm_req->h_stream) == TRUE) {
            /* this video stream has related closed cpation in using*/
            i4_ret = x_sm_close_stream(pt_cc_strm_req->h_stream);
            if (i4_ret == SMR_OK || i4_ret == SMR_PENDING) {
                pt_cc_strm_req->e_strm_cond = SM_COND_CLOSING;
                /* set closed caption's stream handle to null will let the stream
                 * to replay when the new video stream is playing.
                 */
                pt_cc_strm_req->h_stream  = NULL_HANDLE;
            }
        }
        break;
      }
    default:
        return SVCTXR_NOT_SUPPORT;
    }

    if ((pt_svctx->ui4_frz_frozen_strm_mask & MAKE_BIT_MASK_32(e_strm_type)) > 0) {
        svctx_free_strm_req(pt_msg->u.pt_strm_req);
        return SVCTXR_FROZEN;
    }

    switch(pt_strm_req->e_strm_cond) {
    case SM_COND_CLOSED:
        *pt_strm_req = *(pt_msg->u.pt_strm_req);
        i4_ret = inp_hdlr_open_strm(
                    pt_svctx,
                    pt_running_svc_req,
                    pt_player,
                    pt_strm_req,
                    e_sess_cond == SM_SESS_COND_OPENED ? FALSE : TRUE
                    );
        if (i4_ret == SVCTXR_OK) {
            pt_strm_req->e_penging_status      = STRM_PENDING_COND_SELECT_STRM;
        } else {
            pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
            pt_strm_req->e_penging_status      = STRM_PENDING_COND_NONE;
        }
        svctx_free_strm_req(pt_msg->u.pt_strm_req);
        break;
    case SM_COND_OPENED:
    case SM_COND_STARTED:
        if (pt_running_svc_req->pt_pending_strm_reqs[e_strm_type] != NULL) {
            svctx_free_strm_req(pt_running_svc_req->pt_pending_strm_reqs[e_strm_type]);
            pt_running_svc_req->pt_pending_strm_reqs[e_strm_type] = NULL;
        }

        i4_ret = x_sm_close_stream(pt_strm_req->h_stream);
        if (i4_ret == SMR_OK || i4_ret == SMR_PENDING) {
            pt_strm_req->e_penging_status              = STRM_PENDING_COND_SELECT_STRM;
            pt_strm_req->e_strm_cond                   = SM_COND_CLOSING;
            pt_running_svc_req->pt_pending_strm_reqs[e_strm_type] = pt_msg->u.pt_strm_req;
            i4_ret = SVCTXR_OK;
        } else if (i4_ret == SMR_INV_HANDLE) {
            *pt_strm_req             = *(pt_msg->u.pt_strm_req);
            pt_strm_req->e_strm_cond = SM_COND_CLOSED;
            i4_ret = inp_hdlr_open_strm(
                        pt_svctx,
                        pt_running_svc_req,
                        pt_player,
                        pt_strm_req,
                        e_sess_cond == SM_SESS_COND_OPENED ? FALSE : TRUE
                        );
            if (i4_ret == SVCTXR_OK) {
                pt_strm_req->e_penging_status      = STRM_PENDING_COND_SELECT_STRM;
            } else {
                pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
                pt_strm_req->e_penging_status      = STRM_PENDING_COND_NONE;
            }
            svctx_free_strm_req(pt_msg->u.pt_strm_req);
        } else {
            svctx_free_strm_req(pt_msg->u.pt_strm_req);
            i4_ret = SVCTXR_FAIL;
        }
        break;
    default:
        if (pt_running_svc_req->pt_pending_strm_reqs[e_strm_type] != NULL) {
            svctx_free_strm_req(pt_running_svc_req->pt_pending_strm_reqs[e_strm_type]);
            pt_running_svc_req->pt_pending_strm_reqs[e_strm_type] = NULL;
        }

        pt_strm_req->e_penging_status = STRM_PENDING_COND_SELECT_STRM;
        pt_running_svc_req->pt_pending_strm_reqs[e_strm_type] = pt_msg->u.pt_strm_req;
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_stop_strm
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_NOT_SUPPORT      not support stream type
 *      SVCTXR_ALREADY_STOPPED  already stopped
 *      SVCTXR_NOT_EXIST        no matched stream type
 *      SVCTXR_PENDING          stop is in executing
 *      SVCTXR_FAIL             SM failed
 *-----------------------------------------------------------------------------*/
INT32 inp_hdlr_stop_strm (
                    SVCTX_T*                    pt_svctx,
                    SVCTX_MSG_T*                pt_msg
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = NULL;
    INT32          i4_ret             = SVCTXR_OK;
    STREAM_TYPE_T  e_strm_type        = pt_msg->u.t_stop_strm_req.e_type;
    STRM_REQ_T*    pt_strm_req        = NULL;

    pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        return SVCTXR_ALREADY_STOPPED;
    }

    switch(e_strm_type) {
    case ST_CLOSED_CAPTION:
        pt_running_svc_req->t_cc_filter.e_cc_type         = SM_CC_TYPE_IGNORE;
        pt_running_svc_req->t_cc_filter.u.ui1_service_num = 0xFF;
        break;
    case ST_AUDIO:
        break;
    case ST_VIDEO:
      {
        STRM_REQ_T* pt_cc_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_CLOSED_CAPTION]);

        /* this video stream has related closed cpation in using*/
        if (pt_cc_strm_req != NULL && x_handle_valid(pt_cc_strm_req->h_stream) == TRUE) {
            pt_cc_strm_req->e_penging_status = STRM_PENDING_COND_STOP_STRM;
            i4_ret = x_sm_close_stream(pt_cc_strm_req->h_stream);
            if (i4_ret == SMR_OK || i4_ret == SMR_PENDING || i4_ret == SMR_INV_HANDLE) {
                pt_cc_strm_req->e_strm_cond = SM_COND_CLOSING;
            }
        }
        break;
      }
    default:
        return SVCTXR_NOT_SUPPORT;
    }

    if (!pt_msg->u.t_stop_strm_req.b_aux_audio)
    {
        pt_strm_req          = &(pt_running_svc_req->t_strm_reqs[e_strm_type]);
        pt_running_svc_req->t_parent.ui4_stream_mode &= ~(MAKE_BIT_MASK_32(e_strm_type));
    }

    if (pt_strm_req)
    {
        switch(pt_strm_req->e_strm_cond) {
        case SM_COND_CLOSED:
            pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
            pt_strm_req->e_penging_status      = STRM_PENDING_COND_NONE;
            i4_ret = SVCTXR_ALREADY_STOPPED;

            if (pt_msg->u.t_stop_strm_req.t_sync.pf_nfy)
            {
                (pt_msg->u.t_stop_strm_req.t_sync.pf_nfy)(pt_msg->u.t_stop_strm_req.t_sync.pv_tag);
                pt_msg->u.t_stop_strm_req.t_sync.pf_nfy = NULL;
                pt_msg->u.t_stop_strm_req.t_sync.pv_tag = NULL;
            }
            
            break;
        default:
            if (pt_strm_req->u.t_scdb.t_comp_id.e_type != e_strm_type) {
                i4_ret = SVCTXR_NOT_EXIST;
                break;
            }

            if (pt_running_svc_req->pt_pending_strm_reqs[e_strm_type] != NULL) {
                svctx_free_strm_req(pt_running_svc_req->pt_pending_strm_reqs[e_strm_type]);
                pt_running_svc_req->pt_pending_strm_reqs[e_strm_type] = NULL;
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
            } else {
                i4_ret = SVCTXR_FAIL;
            }
            break;
        }
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_internal_msg
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 inp_hdlr_internal_msg(
                    SVCTX_T*                    pt_svctx,
                    SVCTX_MSG_T*                pt_msg
                    )
{
    INT32 i4_ret = SVCTXR_OK;

    switch(pt_msg->ui1_msg_id) {
    case _SVCTX_MSG_STRM_NTFY:
        i4_ret = _inp_hdlr_handle_strm_msg(
                    pt_svctx,
                    &pt_msg->u.t_strm
                    );
        break;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    case _SVCTX_MSG_QUEUED_OPEN_AUDIO_STRM_NTFY:
        inp_hdlr_try_opening_stream(
            pt_svctx,
            (INP_SVC_REQ_T*) pt_svctx->pt_running_svc_req,
            ((INP_SVC_REQ_T*) pt_svctx->pt_running_svc_req)->pt_audio_player,
            &(((INP_SVC_REQ_T*) pt_svctx->pt_running_svc_req)->t_strm_reqs[pt_msg->u.i4_idx]),
            (BOOL) (UINT32) pt_msg->pv_tag);
        break;

    case _SVCTX_MSG_QUEUED_OPEN_VIDEO_STRM_NTFY:
        inp_hdlr_try_opening_stream(
            pt_svctx,
            (INP_SVC_REQ_T*) pt_svctx->pt_running_svc_req,
            ((INP_SVC_REQ_T*) pt_svctx->pt_running_svc_req)->pt_video_player,
            &(((INP_SVC_REQ_T*) pt_svctx->pt_running_svc_req)->t_strm_reqs[pt_msg->u.i4_idx]),
            (BOOL) (UINT32) pt_msg->pv_tag);
        break;

    case _SVCTX_MSG_QUEUED_CLOSE_STRM_NTFY:
        inp_hdlr_try_closing_stream(
            pt_svctx,
            pt_msg->u.t_strm.h_obj);
        break;
#endif
    default:
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_stop_video_player
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 inp_hdlr_stop_video_player (
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    BOOL                        b_close_block_check_objs
                    )
{
    INT32 i4_ret = SVCTXR_OK;

    if (b_close_block_check_objs == FALSE) {
        i4_ret = crnt_evn_stop_query(pt_running_svc_req->pt_crnt_event);
        if (i4_ret != SVCTXR_OK) {
            return i4_ret;
        }
    } else {
        i4_ret = rating_chker_close(pt_running_svc_req->pt_rating_chker);
        if (i4_ret != SVCTXR_OK) {
            return i4_ret;
        }
        pt_running_svc_req->pt_rating_chker = NULL;

        i4_ret = crnt_evn_close(pt_running_svc_req->pt_crnt_event);
        if (i4_ret != SVCTXR_OK) {
            return i4_ret;
        }
        pt_running_svc_req->pt_crnt_event = NULL;
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (pt_running_svc_req->pt_video_player)
#endif
    {
        i4_ret = player_stop(pt_running_svc_req->pt_video_player);
        if (i4_ret == SVCTXR_OK) {
            i4_ret = player_close(pt_running_svc_req->pt_video_player);
            if (i4_ret != SVCTXR_OK && i4_ret != SVCTXR_PENDING) {
                return i4_ret;
            }

            if (i4_ret == SVCTXR_OK) {
                pt_running_svc_req->pt_video_player = NULL;
            }
        } else if (i4_ret == SVCTXR_NOT_EXIST) {
            i4_ret = SVCTXR_OK;
        }
    }
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_stop_audio_player
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 inp_hdlr_stop_audio_player (
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    INT32 i4_ret = SVCTXR_OK;

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (pt_running_svc_req->pt_audio_player)
#endif
{
        i4_ret = player_stop(pt_running_svc_req->pt_audio_player);
        if (i4_ret == SVCTXR_OK) {
            i4_ret = player_close(pt_running_svc_req->pt_audio_player);
            if (i4_ret != SVCTXR_OK && i4_ret != SVCTXR_PENDING) {
                return i4_ret;
            }

            if (i4_ret == SVCTXR_OK) {
                pt_running_svc_req->pt_audio_player = NULL;
            }
        } else if (i4_ret == SVCTXR_NOT_EXIST) {
            i4_ret = SVCTXR_OK;
        }
    }
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_play_video_player
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 inp_hdlr_play_video_player (
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    INT32 i4_ret = SVCTXR_OK;

    pt_running_svc_req->b_replace_video = FALSE;

    if (pt_running_svc_req->t_src_info.t_av_src.e_video_type == DEV_UNKNOWN) {
        return SVCTXR_OK;
    }

    _inp_hdlr_prepare_block_check_stuff(pt_svctx, pt_running_svc_req);

    if (pt_running_svc_req->pt_video_player == NULL) {
        i4_ret = player_open (
                    pt_svctx->s_snk_grp_name,
                    DEFAULT_RECONNECT_DELAY,
                    _inp_hdlr_handle_video_player_nty,
                    pt_svctx,
#ifdef MW_TV_AV_BYPASS_SUPPORT
                    pt_svctx->i1_prio_diff,
#endif
                    &pt_running_svc_req->pt_video_player
                    );
        if (i4_ret != SVCTXR_OK && i4_ret != SVCTXR_PENDING) {
            return i4_ret;
        }
    }

    if (i4_ret == SVCTXR_OK) {
        i4_ret = player_play(
                    pt_running_svc_req->pt_video_player,
                    pt_running_svc_req->t_parent.i4_speed
                    );
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_play_audio_player
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 inp_hdlr_play_audio_player (
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    INT32 i4_ret = SVCTXR_OK;
    
    if (pt_svctx->b_is_blocked == TRUE) {
        return SVCTXR_OK;
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (IS_SVCTX_ASSOCIATE(pt_svctx) &&
        0 == (pt_svctx->ui4_stream_mode & ST_MASK_AUDIO))
    {
        return SVCTXR_OK;
    }
#endif

    pt_running_svc_req->b_replace_audio                       = FALSE;
    pt_running_svc_req->b_play_audio_from_video_device        = FALSE;
    pt_running_svc_req->t_src_info.ui2_crnt_audio_combi_index = INVALID_CONN_COMBI_INDEX;
    pt_running_svc_req->t_src_info.e_crnt_audio_type          = DEV_UNKNOWN;

    if (pt_running_svc_req->t_src_info.t_av_src.e_audio_type == DEV_UNKNOWN) {
        return SVCTXR_OK;
    }

    if (pt_running_svc_req->t_src_info.t_av_src.e_video_type != DEV_UNKNOWN) {
        /* if the source with video type source, then do the following check */
        if ((pt_svctx->ui1_stop_audio_as_no_signal & MAKE_BIT_MASK_8(pt_running_svc_req->t_parent.e_src_type)) > 0) {
            if (pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED) {
                return SVCTXR_OK;
            }
        }
    }

    if (pt_running_svc_req->pt_audio_player == NULL) {
        /* open audio player */
        i4_ret = player_open (
                        pt_svctx->s_snk_grp_name,
                        DEFAULT_RECONNECT_DELAY,
                        _inp_hdlr_handle_audio_player_nty,
                        pt_svctx,
#ifdef MW_TV_AV_BYPASS_SUPPORT
                        pt_svctx->i1_prio_diff,
#endif
                        &pt_running_svc_req->pt_audio_player
                        );
        if (i4_ret != SVCTXR_OK && i4_ret != SVCTXR_PENDING) {
            return i4_ret;
        }
    }

    if (i4_ret == SVCTXR_OK) {
        i4_ret = player_play(
                    pt_running_svc_req->pt_audio_player,
                    pt_running_svc_req->t_parent.i4_speed
                    );
        if (i4_ret != SVCTXR_OK && i4_ret != SVCTXR_PENDING) {
            DBG_ERROR((_ERROR_HEADER"%s() player_play(0x%x) failed, i4_ret = %d\r\n",
                    __func__,
                    pt_running_svc_req->pt_audio_player,
                    i4_ret
                    ));
        } else {
            pt_running_svc_req->t_src_info.e_crnt_audio_type = pt_running_svc_req->t_src_info.t_av_src.e_audio_type;
        }
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_check_audio_fmt
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 inp_hdlr_check_audio_fmt (
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    STRM_REQ_T* pt_audio_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_AUDIO]);

    if (pt_audio_strm_req == NULL) {
        return SVCTXR_OK;
    }

    if (x_handle_valid(pt_audio_strm_req->h_stream) == FALSE) {
        inp_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_NO_STREAM,
                    pt_audio_strm_req
                    );
        return SVCTXR_OK;
    }

    if (pt_audio_strm_req->e_strm_cond == SM_COND_STARTED) {
        inp_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_STREAM_STARTED,
                    pt_audio_strm_req
                    );
    }

    inp_hdlr_invoke_client_strm_nfy_fct(
                pt_svctx,
                SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE,
                pt_audio_strm_req
                );

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_check_video_fmt
 * Description
 *      The API is used to check to resent video stream relative notification
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 inp_hdlr_check_video_fmt (
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    INT32 i4_ret;

    i4_ret = _inp_hdlr_check_video_stream(pt_svctx, pt_running_svc_req);
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    i4_ret = _inp_hdlr_check_video_format(pt_svctx, pt_running_svc_req);
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: inp_hdlr_freeze
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *-----------------------------------------------------------------------------*/
INT32 inp_hdlr_freeze (
                    SVCTX_T*                    pt_svctx
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = NULL;
    INT32          i4_ret             = SVCTXR_OK;

    pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        return SVCTXR_PENDING;
    }

    /* freeze */
    if (pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_AUDIO) {
    }

    if (pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_CLOSED_CAPTION) {
        i4_ret = _inp_hdlr_close_text_strms(
                    pt_svctx,
                    pt_running_svc_req,
                    ST_MASK_CLOSED_CAPTION
                    );
        if (i4_ret != SVCTXR_OK) {
            return i4_ret;
        }
    }
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: inp_hdlr_unfreeze
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *-----------------------------------------------------------------------------*/
INT32 inp_hdlr_unfreeze (
                    SVCTX_T*                    pt_svctx
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = NULL;
    INT32          i4_ret             = SVCTXR_OK;

    pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        return SVCTXR_PENDING;
    }

    i4_ret = _inp_hdlr_video_strm_post_start_handler(
                    pt_svctx,
                    pt_running_svc_req
                    );

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_check_to_play_audio_from_video_player
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 inp_hdlr_check_to_play_audio_from_video_player(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    INT32 i4_ret = SVCTXR_NO_ACTION;

    do {
        if (inp_hdlr_able_to_play_audio_from_video_player(
                    pt_svctx,
                    pt_running_svc_req
                    ) == FALSE) {
            break;
        }

        if ((pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_dev_type
             == pt_running_svc_req->t_src_info.e_crnt_audio_type)  && 
             pt_running_svc_req->b_play_audio_from_video_device == TRUE) {
            break;
        }
        
        if ((pt_running_svc_req->t_parent.ui4_stream_mode & ST_MASK_AUDIO) == 0) {
            /* if don't need to play AUDIO, and if audio can be play from
             * video device, need to stop audio player (audio device)
             */
            pt_running_svc_req->b_play_audio_from_video_device = TRUE;
            inp_hdlr_stop_audio_player(pt_svctx, pt_running_svc_req);
            break;
        }

        i4_ret = inp_hdlr_stop_audio_player(pt_svctx, pt_running_svc_req);
        if (i4_ret == SVCTXR_PENDING) {
            pt_running_svc_req->b_replace_audio = TRUE;
            i4_ret = SVCTXR_OK;
        } else if (i4_ret == SVCTXR_OK) {
            i4_ret = inp_hdlr_play_audio_from_video_player(pt_svctx, pt_running_svc_req);
        }
    } while (0);

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_check_to_play_hdmi_audio
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 inp_hdlr_check_to_play_hdmi_audio(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    if (pt_svctx->t_video_info.e_timing_type != VSH_SRC_TIMING_NOT_SUPPORT
            && pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_dev_type == DEV_AVC_HDMI) {
        /* if audio is preferred HDMI, but no audio from HDMI */
#if 0 /* update for CR DTV00066441 */
        if (pt_running_svc_req->t_src_info.e_crnt_audio_type != DEV_AVC_HDMI
                && pt_running_svc_req->pt_audio_player == NULL
                && (pt_running_svc_req->t_parent.ui4_stream_mode & ST_MASK_AUDIO) > 0) {
#else
        if (pt_running_svc_req->t_src_info.e_crnt_audio_type != DEV_AVC_HDMI
                && pt_running_svc_req->pt_audio_player == NULL ) {
#endif     
            /* start a timer to play audio player */
            svctx_timer_start(
                    pt_svctx,
                    DEFAULT_HDMI_WITH_SIGNAL_CHK_AUDIO_TIMEOUT,
                    _inp_hdlr_hdmi_timeout_cb,
                    (VOID*)pt_svctx
                    );
        } else {
            svctx_timer_stop(pt_svctx);
        }
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_play_audio_from_video_player
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 inp_hdlr_play_audio_from_video_player(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    SM_SESS_COND_T e_sess_cond = player_get_sess_cond(pt_running_svc_req->pt_video_player);
    INT32          i4_ret      = SVCTXR_OK;

    if (pt_svctx->b_is_blocked == TRUE) {
        return SVCTXR_OK;
    }

    if (e_sess_cond != SM_SESS_COND_OPENED
            && e_sess_cond != SM_SESS_COND_STARTING
            && e_sess_cond != SM_SESS_COND_STARTED) {
        return i4_ret;
    }

    if (pt_running_svc_req->t_src_info.t_av_src.e_audio_type == DEV_UNKNOWN
            && pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_type == SRC_AVC_HINT_NONE) {
        return SVCTXR_OK;
    }

    if (pt_running_svc_req->b_play_audio_from_video_device == TRUE
            && pt_running_svc_req->t_src_info.e_crnt_audio_type == pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_dev_type) {
        return SVCTXR_OK;
    }

    pt_running_svc_req->t_src_info.e_crnt_audio_type = pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_dev_type;

    i4_ret = _inp_hdlr_play_strm(
                    pt_svctx,
                    pt_running_svc_req,
                    pt_running_svc_req->pt_video_player,
                    ST_AUDIO,
                    (e_sess_cond == SM_SESS_COND_STARTED ? TRUE : FALSE)
                    );
    if (i4_ret >= SVCTXR_OK) {
        pt_running_svc_req->b_play_audio_from_video_device = TRUE;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_play_audio_stream
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 inp_hdlr_play_audio_stream (
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    if (pt_running_svc_req->b_replace_audio == TRUE) {
        return SVCTXR_OK;
    }

    if (inp_hdlr_able_to_play_audio_from_video_player(
                    pt_svctx,
                    pt_running_svc_req
                    ) == TRUE) {
        pt_running_svc_req->b_play_audio_from_video_device = TRUE;
        if ((pt_running_svc_req->t_parent.ui4_stream_mode & ST_MASK_AUDIO) > 0) {
            inp_hdlr_play_audio_from_video_player(pt_svctx, pt_running_svc_req);
        }
    } else {
        pt_running_svc_req->b_play_audio_from_video_device = FALSE;
#if 0   /* for CR DTV00336539 */
        if ((pt_running_svc_req->t_parent.ui4_stream_mode & ST_MASK_AUDIO) > 0) {
#endif
            inp_hdlr_play_audio_player(pt_svctx, pt_running_svc_req);
#if 0
        }
#endif
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_with_audio_from_video_player
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
BOOL inp_hdlr_with_audio_from_video_player(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    if (svctx_with_av_capability(pt_running_svc_req->t_src_info.e_crnt_video_type, ST_AUDIO) == FALSE) {
        return FALSE;
    }

    if (_inp_hdlr_with_audio_streams_in_video_player(pt_running_svc_req) == FALSE) {
        return FALSE;
    }

    return TRUE;
}
/*-----------------------------------------------------------------------------
 * export methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_check_signal_loss_priority
 * Description
 *      The API is used to check the priority between input source block,
 *      program block, and signal loss.
 * Input arguments
 *      pt_svctx                        The service context object
 *      pt_svc_req                      The service request to check
 *      b_signal_loss_and_check_block   TRUE - check block and send signal loss
 *      b_send_signal_loss_nfy_no_check_combi
 * Output arguments
 *      None
 * Returns
 *      TRUE    signal_loss with higher priority than parental block
 *      FALSE   signal_loss with lower priority than parental block
 *---------------------------------------------------------------------------*/
BOOL inp_hdlr_check_signal_loss_priority(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_svc_req,
                    BOOL                        b_signal_loss_and_check_block,
                    BOOL                        b_send_signal_loss_nfy_no_check_combi
                    )
{
    SVC_BLOCK_COND_T        e_block_cond;
    SVC_BLOCK_CHECK_GUDIE_T e_check_guide;

    if (pt_svc_req == NULL) {
        return FALSE;
    }

    e_block_cond = rating_chker_get_block_cond(pt_svc_req->pt_rating_chker);

    switch(pt_svctx->e_block_rule) {
    case SVCTX_BLOCK_RULE_INP_SIGNAL_LOSS_CH_PROG:
    case SVCTX_BLOCK_RULE_INP_CH_SIGNAL_LOSS_PROG:
        if (e_block_cond == SVC_BLOCK_COND_USER_BLOCK_INP) {
            return FALSE;
        }

        e_check_guide = SVC_BLOCK_CHECK_USER_BLOCK_INP;
        break;
    case SVCTX_BLOCK_RULE_INP_CH_PROG_SIGNAL_LOSS:
        if (e_block_cond == SVC_BLOCK_COND_USER_BLOCK_INP
            || e_block_cond == SVC_BLOCK_COND_USER_BLOCK_CH
            || e_block_cond == SVC_BLOCK_COND_EXCEED_RATING) {
            return FALSE;
        }

        e_check_guide = SVC_BLOCK_CHECK_ALL;
        break;
    default:
        e_check_guide = SVC_BLOCK_CHECK_ALL;
        break;
    }

    if (b_signal_loss_and_check_block == TRUE) {
        /* if no signal from COMBI, delay to until ACM start to check it is
         * no signal or just other high priority source is connected.
         */
        if (b_send_signal_loss_nfy_no_check_combi == TRUE
                || pt_svc_req->t_src_info.t_av_src.e_video_type != DEV_AVC_COMBI) {
            if (_inp_hdlr_is_blocked_service(
                    pt_svctx,
                    pt_svc_req,
                    e_check_guide,
                    TRUE) == TRUE) {
                _inp_hdlr_block_service(pt_svctx, pt_svc_req);
                pt_svctx->e_signal_status = SIGNAL_STATUS_LOSS;
            } else {
                inp_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_SIGNAL_LOSS,
                    ST_UNKNOWN
                    );
            }
        }
    }

    return TRUE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_is_blocked_source
 * Description
 *      The API is used to check the input source is blocked or not
 * Input arguments
 *      pt_svctx             the pointer points to an SVCTX_T object
 *      pt_svc_req           the pointer points to a svc request to check
 * Output arguments
 *      None
 * Returns
 *      TRUE            blocked service
 *      FALSE           not blocked service
 *-----------------------------------------------------------------------------*/
BOOL inp_hdlr_is_blocked_source(
                    SVCTX_T*                    pt_svctx,
                    SVC_REQ_T*                  pt_svc_req
                    )
{
    if (pt_svctx == NULL || pt_svc_req == NULL) {
        return FALSE;
    }

#ifdef SVCTX_SPLIT_SUPPORT
    if (pt_svctx->pf_block_check_fct (
                    SVC_BLOCK_CHECK_USER_BLOCK_INP,
                    ((INP_SVC_REQ_T*)pt_svc_req)->t_src_info.s_src_grp_name,
                    NULL,
                    NULL,
                    pt_svctx->pf_block_check_fct_tag
                    ) == SVC_BLOCK_COND_USER_BLOCK_INP) {
#else
    if (pt_svctx->pf_block_check_fct (
                    SVC_BLOCK_CHECK_USER_BLOCK_INP,
                    ((INP_SVC_REQ_T*)pt_svc_req)->t_src_info.s_src_grp_name,
                    NULL,
                    NULL
                    ) == SVC_BLOCK_COND_USER_BLOCK_INP) {
#endif
        return TRUE;
    }

    return FALSE;
}

#ifdef MW_TV_AV_BYPASS_SUPPORT
INT32 inp_hdlr_try_closing_stream(
    SVCTX_T*    pt_svctx,
    HANDLE_T    h_stream)
{
    INT32           i4_ret;
    STRM_REQ_T*     pt_strm_req;
    VAR_INFO_T      t_var_info;
    INP_SVC_REQ_T*  pt_inp_svc_req;
    STREAM_TYPE_T   e_strm_type;
    INT32           i4_idx;

    i4_ret = SVCTXR_FAIL;

    pt_inp_svc_req = (INP_SVC_REQ_T*) pt_svctx->pt_running_svc_req;

    if (pt_inp_svc_req)
    {
        i4_ret = _inp_hdlr_get_strm_req_from_handle(
            pt_svctx,
            pt_inp_svc_req,
            h_stream,
            &pt_strm_req,
            &e_strm_type);

        if (SVCTXR_OK == i4_ret)
        {
            i4_idx = pt_strm_req - pt_inp_svc_req->t_strm_reqs;

            if (i4_idx >= INP_STRM_TYPE_COUNT)
            {
                DBG_ABORT(DBG_MOD_SVCTX);
            }

            t_var_info.ui1_msg_id                = _SVCTX_MSG_QUEUED_CLOSE_STRM_NTFY;
            t_var_info.e_var_type                = VAR_TYPE_STREAM;
            t_var_info.e_op_type                 = OP_TYPE_CLOSE;
            t_var_info.u.t_strm_info.h_stream    = h_stream;
            t_var_info.u.t_strm_info.e_strm_type = e_strm_type;
            t_var_info.u.t_strm_info.i4_idx      = i4_idx;
            t_var_info.u.t_strm_info.b_aux_audio = FALSE;
            t_var_info.u.t_strm_info.b_auto_play = FALSE;

            if (FALSE == svctx_mgr_ready_to_proceed(
                ACTIVE_LINK_NOW,
                pt_svctx->pt_running_svc_req->e_src_type,
                pt_svctx,
                &t_var_info))
            {
#ifdef CLI_LVL_ALL
                DBG_SVCTXMGR_MSG((
                    "{SVCTX_MGR} SVCTX(%s): Closing %s stream POSTPONED.\r\n",
                    pt_svctx->s_name,
                    s_g_stream[t_var_info.u.t_strm_info.e_strm_type]));
#endif
                pt_strm_req->b_close_queued = TRUE;
            #if 0
                /* send message */
                t_msg.ui1_msg_id     = _SVCTX_MSG_QUEUED_CLOSE_STRM_NTFY;
                t_msg.pf_invoke_func = NULL;
                t_msg.u.t_strm.h_obj = h_stream;

                svctx_send_msg(pt_svctx->h_sys_svctx, &t_msg);
            #endif
                i4_ret = SVCTXR_QUEUED;
            }
            else
            {
#ifdef CLI_LVL_ALL
                DBG_SVCTXMGR_MSG((
                    "{SVCTX_MGR} SVCTX(%s): Closing %s stream CONTINUED.\r\n",
                    pt_svctx->s_name,
                    s_g_stream[t_var_info.u.t_strm_info.e_strm_type]));
#endif
                i4_ret = x_sm_close_stream(h_stream);

                pt_strm_req->e_strm_cond    = SM_COND_CLOSING;
                pt_strm_req->b_close_queued = FALSE;

                if (SMR_OK == i4_ret || SMR_PENDING == i4_ret)
                {
                    i4_ret = SVCTXR_OK;
                }
                else if (SMR_INV_HANDLE == i4_ret)
                {
                    i4_ret = SVCTXR_INV_HANDLE;
                }
                else
                {
                    i4_ret = SVCTXR_FAIL;
                }
            }
        }
    }

    return (i4_ret);
}

INT32 inp_hdlr_try_opening_stream(
    SVCTX_T*        pt_svctx,
    INP_SVC_REQ_T*  pt_running_svc_req,
    PLAYER_T*       pt_player,
    STRM_REQ_T*     pt_strm_req,
    BOOL            b_auto_play)
{
    INT32           i4_ret;
    INT32           i4_idx;
    VAR_INFO_T      t_var_info;

    i4_ret = SVCTXR_FAIL;

    if (pt_running_svc_req)
    {
        i4_idx = pt_strm_req - pt_running_svc_req->t_strm_reqs;

        if (i4_idx >= INP_STRM_TYPE_COUNT)
        {
            DBG_ABORT(DBG_MOD_SVCTX);
        }

        if (pt_player == pt_running_svc_req->pt_video_player)
        {
            t_var_info.ui1_msg_id = _SVCTX_MSG_QUEUED_OPEN_VIDEO_STRM_NTFY;
        }
        else if (pt_player == pt_running_svc_req->pt_audio_player)
        {
            t_var_info.ui1_msg_id = _SVCTX_MSG_QUEUED_OPEN_AUDIO_STRM_NTFY;
        }
        else
        {
            DBG_ABORT(DBG_MOD_SVCTX);
        }

        t_var_info.e_var_type                = VAR_TYPE_STREAM;
        t_var_info.e_op_type                 = OP_TYPE_OPEN;
        t_var_info.u.t_strm_info.h_stream    = NULL_HANDLE;
        t_var_info.u.t_strm_info.e_strm_type = (STREAM_TYPE_T) i4_idx;
        t_var_info.u.t_strm_info.i4_idx      = i4_idx;
        t_var_info.u.t_strm_info.b_aux_audio = FALSE;
        t_var_info.u.t_strm_info.b_auto_play = b_auto_play;

        if (FALSE == svctx_mgr_ready_to_proceed(
            ACTIVE_LINK_NOW,
            pt_svctx->pt_running_svc_req->e_src_type,
            pt_svctx,
            &t_var_info))
        {
            if (pt_player == pt_running_svc_req->pt_video_player)
            {
#ifdef CLI_LVL_ALL
                DBG_SVCTXMGR_MSG((
                    "{SVCTX_MGR} SVCTX(%s): Opening %s stream at VIDEO player POSTPONED (autoPlay = %d)...\r\n",
                    pt_svctx->s_name,
                    s_g_stream[t_var_info.u.t_strm_info.e_strm_type],
                    b_auto_play));
#endif
            }
            else if (pt_player == pt_running_svc_req->pt_audio_player)
            {
#ifdef CLI_LVL_ALL
                DBG_SVCTXMGR_MSG((
                    "{SVCTX_MGR} SVCTX(%s): Opening %s stream at AUDIO player POSTPONED (autoPlay = %d)...\r\n",
                    pt_svctx->s_name,
                    s_g_stream[t_var_info.u.t_strm_info.e_strm_type],
                    b_auto_play));
#endif
            }

            pt_strm_req->b_open_queued = TRUE;
        #if 0
            /* send message */
            t_msg.pf_invoke_func = NULL;
            t_msg.pv_tag         = (VOID*) b_auto_play;
            t_msg.u.pt_strm_req  = pt_strm_req;

            svctx_send_msg(pt_svctx->h_sys_svctx, &t_msg);
        #endif
            i4_ret = SVCTXR_OK;
        }
        else
        {

            if (pt_player == pt_running_svc_req->pt_video_player)
            {
#ifdef CLI_LVL_ALL
                DBG_SVCTXMGR_MSG((
                    "{SVCTX_MGR} SVCTX(%s): Opening %s stream at VIDEO player CONTINUED (autoPlay = %d)...\r\n",
                    pt_svctx->s_name,
                    s_g_stream[t_var_info.u.t_strm_info.e_strm_type],
                    b_auto_play));
#endif
            }
            else if (pt_player == pt_running_svc_req->pt_audio_player)
            {
#ifdef CLI_LVL_ALL
                DBG_SVCTXMGR_MSG((
                    "{SVCTX_MGR} SVCTX(%s): Opening %s stream ay AUDIO player CONTINUED (autoPlay = %d)...\r\n",
                    pt_svctx->s_name,
                    s_g_stream[t_var_info.u.t_strm_info.e_strm_type],
                    b_auto_play));
#endif
            }

            i4_ret = inp_hdlr_open_strm_ex(
                pt_svctx,
                pt_running_svc_req,
                pt_player,
                pt_strm_req,
                b_auto_play);

            pt_strm_req->b_open_queued = FALSE;

            if (SMR_OK == i4_ret || SMR_PENDING == i4_ret)
            {
                i4_ret = SVCTXR_OK;
            }
            else if (SMR_INV_HANDLE == i4_ret)
            {
                i4_ret = SVCTXR_INV_HANDLE;
            }
            else
            {
                i4_ret = SVCTXR_FAIL;
            }
        }
    }

    return (i4_ret);
}
#endif

