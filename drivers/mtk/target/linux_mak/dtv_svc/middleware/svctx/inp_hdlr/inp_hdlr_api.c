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
 * $RCSfile: inp_hdlr_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/26 $
 * $SWAuthor: Weider Chang $
 * $MD5HEX: 9cacb426bc6af60b7f3e5a1ad05af25e $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "svctx/inp_hdlr/x_inp_hdlr.h"
#include "svctx/inp_hdlr/inp_hdlr.h"
#include "svctx/inp_hdlr/inp_svc_req.h"
#include "svctx/inp_hdlr/inp_hdlr_cli.h"
#include "svctx/util/av_monitor.h"
#include "svctx/svctx_dbg.h"
#include "svctx/svctx_cli.h"
#ifdef MW_TV_AV_BYPASS_SUPPORT
#include "svctx/svctx_mgr.h"
#endif

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#define SVC_REQ_AMOUNT_FACTOR   (2)

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

#define SVCTX_SM_BROKER_CMD_BIT_DEVICE_TYPE     MAKE_BIT_MASK_32(0)
#define SVCTX_SM_BROKER_CMD_BIT_SRC_NAME        MAKE_BIT_MASK_32(1)

#define SVCTX_SM_BROKER_CMD_BIT_AVC            (SVCTX_SM_BROKER_CMD_BIT_DEVICE_TYPE| \
                                                SVCTX_SM_BROKER_CMD_BIT_SRC_NAME)
/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * private variable declarations
 *---------------------------------------------------------------------------*/
static BOOL          b_g_init = FALSE;
const  STREAM_TYPE_T e_g_inp_strm_types[INP_STRM_TYPE_COUNT] = {ST_VIDEO, ST_AUDIO, ST_CLOSED_CAPTION};
/*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_get_player
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static PLAYER_T* _inp_hdlr_get_player (
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    STREAM_TYPE_T               e_pipe_type
                    )
{
    if (e_pipe_type == ST_AUDIO) {
        if (pt_running_svc_req->b_play_audio_from_video_device == TRUE) {
            return pt_running_svc_req->pt_video_player;
        } else {
            return pt_running_svc_req->pt_audio_player;
        }
    } else if (e_pipe_type == ST_VIDEO) {
        return pt_running_svc_req->pt_video_player;
    } else {
        return NULL;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_get_scdb_handle
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static HANDLE_T _inp_hdlr_get_scdb_handle(
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    STREAM_TYPE_T               e_strm_type
                    )
{
    PLAYER_T* pt_player = _inp_hdlr_get_player(pt_running_svc_req, e_strm_type);

    if (pt_player != NULL) {
        return player_get_scdb_handle(pt_player);
    }

    return NULL_HANDLE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_is_supported_strm_type
 * Description
 *      The API is used to check a stream type is the service context supported
 * Input arguments
 *      e_strm_type            the stream type
 * Output arguments
 *      None
 * Returns
 *      TRUE                supported stream type
 *      FALSE               not-supported stream type
 *-----------------------------------------------------------------------------*/
static BOOL inp_hdlr_is_supported_strm_type(
                    STREAM_TYPE_T           e_strm_type
                    )
{   /* QQQQQQQ */
    STREAM_TYPE_T e_supported_strm_type;
    UINT8         ui1_idx;

    for (ui1_idx = 0; ui1_idx < INP_STRM_TYPE_COUNT; ui1_idx++) {
        e_supported_strm_type = e_g_inp_strm_types[ui1_idx];
        if (e_supported_strm_type == ST_UNKNOWN) continue;
        if (e_supported_strm_type == e_strm_type) {
            return TRUE;
        }
    }
    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_allocate_svc_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static SVC_REQ_T* _inp_hdlr_allocate_svc_req(
                    VOID
                    )
{
    INP_SVC_REQ_T* pt_svc_req;
    INT32         i4_ret;

    i4_ret = inp_svc_req_allocate(&pt_svc_req);
    if (i4_ret == SVCTXR_OK) {
        return ((SVC_REQ_T*)pt_svc_req);
    } else {
        return NULL;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_free_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static VOID _inp_hdlr_free_strm_req(
                    STRM_REQ_T*                 pt_strm_req
                    )
{
    inp_strm_req_free(pt_strm_req);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_free_svc_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static VOID _inp_hdlr_free_svc_req(
                    SVC_REQ_T*                  pt_svc_req
                    )
{
    inp_svc_req_free((INP_SVC_REQ_T*)pt_svc_req);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_allocate_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static STRM_REQ_T* _inp_hdlr_allocate_strm_req(
                    VOID
                    )
{
    STRM_REQ_T* pt_strm_req;
    INT32       i4_ret;

    i4_ret = inp_strm_req_allocate(&pt_strm_req);
    if (i4_ret == SVCTXR_OK) {
        return pt_strm_req;
    } else {
        return NULL;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_resent_nfy_as_replacing
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static VOID _inp_hdlr_resent_nfy_as_replacing(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    if (pt_svctx->e_state == SVCTX_COND_PRESENTING) {
        pt_svctx->e_state = SVCTX_COND_PRESENTING_SELECT_SAME_REQ;
    } else if (pt_svctx->e_state == SVCTX_COND_PENDING) {
        pt_svctx->e_state = SVCTX_COND_PENDING_SELECT_SAME_REQ;
    }

    do {
        inp_hdlr_check_svc_block(pt_svctx);

        if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOCKED) {
            if (pt_svctx->b_is_blocked)
            {
                inp_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOCKED_AS_BLOCKED, ST_UNKNOWN);
            }
            else
            {
                inp_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOCKED, ST_UNKNOWN);
            }
        } else if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) {
            if (inp_hdlr_check_signal_loss_priority(pt_svctx, pt_running_svc_req, FALSE, FALSE) == TRUE) {
                inp_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_SIGNAL_LOSS,
                        ST_UNKNOWN
                        );
                break;
            }
        } else {
            if (pt_svctx->e_signal_status == SIGNAL_STATUS_DETECTING) {
                inp_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_SIGNAL_DETECTING,
                        ST_UNKNOWN
                        );
            }

            if (pt_svctx->e_block_rule == SVCTX_BLOCK_RULE_INP_CH_PROG_SIGNAL_LOSS) {
                if (pt_svctx->b_is_blocked == TRUE) {
                    inp_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SERVICE_BLOCKED, ST_UNKNOWN);
                }
            }

            /* should not return if video type is unknown, i.e. audio only */
            if (!(pt_running_svc_req->t_src_info.t_av_src.e_audio_type != DEV_UNKNOWN
                    && pt_running_svc_req->t_src_info.t_av_src.e_video_type == DEV_UNKNOWN)) {
                break;
            }
        }

        if (pt_svctx->b_is_blocked == TRUE) {
            inp_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SERVICE_BLOCKED, ST_UNKNOWN);
            if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) {
                inp_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOSS_AS_BLOCKED, ST_UNKNOWN);
            }
            break;
        }

        inp_hdlr_resend_crnt_evn_nfy(pt_svctx, pt_running_svc_req);
        svctx_set_av_status(pt_svctx, SVC_AV_STATUS_UNKNOWN);
        inp_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_NORMAL, ST_UNKNOWN);
        inp_hdlr_check_video_fmt(pt_svctx, pt_running_svc_req);
        inp_hdlr_check_audio_fmt(pt_svctx, pt_running_svc_req);
    } while (0);

    if (pt_svctx->e_state == SVCTX_COND_PRESENTING_SELECT_SAME_REQ) {
        pt_svctx->e_state = SVCTX_COND_PRESENTING;
    } else if (pt_svctx->e_state == SVCTX_COND_PENDING_SELECT_SAME_REQ) {
        pt_svctx->e_state = SVCTX_COND_PENDING;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_replace_video_attributes
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static VOID _inp_hdlr_replace_video_attributes (
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_new_svc_req,
                    INP_SVC_REQ_T*              pt_old_svc_req
                    )
{
    STRM_REQ_T* pt_video_strm_req;

    if (pt_new_svc_req == NULL || pt_old_svc_req == NULL) {
        return;
    }

    pt_video_strm_req = &(pt_old_svc_req->t_strm_reqs[ST_VIDEO]);

    if (pt_video_strm_req == NULL || x_handle_valid(pt_video_strm_req->h_stream) == FALSE) {
        return;
    }

    svctx_set_video_stream_attrs(pt_svctx, pt_video_strm_req->h_stream);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_replace_strm_req
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_replace_strm_req (
                    INP_SVC_REQ_T*              pt_new_svc_req,
                    INP_SVC_REQ_T*              pt_old_svc_req
                    )
{
    STREAM_TYPE_T e_strm_type;
    UINT8         ui1_idx;

    for (ui1_idx = 0; ui1_idx < INP_STRM_TYPE_COUNT; ui1_idx++) {
        e_strm_type = e_g_inp_strm_types[ui1_idx];
        if (e_strm_type == ST_UNKNOWN) {
            continue;
        }

        if (((UINT32)e_strm_type >= INP_STRM_TYPE_COUNT) || (e_strm_type==ST_UNKNOWN))
        {
            ASSERT(0, ("%s(), e_strm_type=%d out of boundary\r\n", __func__, e_strm_type));
            continue;
        }

        if ((pt_new_svc_req->t_parent.ui4_stream_mode & MAKE_BIT_MASK_32(e_strm_type)) == 0) {
            /* if invisible CC, still count it in */
            if (!(e_strm_type == ST_CLOSED_CAPTION
                    && SVCTX_IS_INVISIBLE_CC_FILTER(pt_old_svc_req->t_cc_filter))) {
                pt_new_svc_req->t_parent.ui4_stream_mode |= ST_MASK_CLOSED_CAPTION;
                continue;
            }
        }

        if ((pt_old_svc_req->t_parent.ui4_stream_mode & MAKE_BIT_MASK_32(e_strm_type)) == 0) {
            continue;
        }

        pt_new_svc_req->t_strm_reqs[e_strm_type]          = pt_old_svc_req->t_strm_reqs[e_strm_type];
        /*
        pt_new_svc_req->pt_running_strm_reqs[e_strm_type] = pt_old_svc_req->pt_running_strm_reqs[e_strm_type];
        */
        pt_new_svc_req->pt_pending_strm_reqs[e_strm_type] = pt_old_svc_req->pt_pending_strm_reqs[e_strm_type];

        pt_new_svc_req->t_strm_reqs[e_strm_type].h_svctx         = pt_new_svc_req->t_parent.h_svctx;
        pt_new_svc_req->t_strm_reqs[e_strm_type].pf_strm_nfy     = pt_new_svc_req->t_parent.pf_svc_nfy;
        pt_new_svc_req->t_strm_reqs[e_strm_type].pv_strm_nfy_tag = pt_new_svc_req->t_parent.pv_svc_nfy_tag;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_reload_cc_strm_as_presenting
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static VOID _inp_hdlr_reload_cc_strm_as_presenting (
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_new_svc_req
                    )
{
    STRM_REQ_T*      pt_strm_req = &(pt_new_svc_req->t_strm_reqs[ST_CLOSED_CAPTION]);
    STREAM_COMP_ID_T t_comp_id;
    INT32 i4_ret;

    pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_CLOSED_CAPTION;
    i4_ret = inp_hdlr_open_strm(
                    pt_svctx,
                    pt_new_svc_req,
                    pt_new_svc_req->pt_video_player,
                    pt_strm_req,
                    TRUE
                    );
    if (i4_ret != SVCTXR_OK) {
        pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
        pt_strm_req->e_penging_status      = STRM_PENDING_COND_NONE;
        return;
    }

    pt_strm_req->h_svctx         = pt_new_svc_req->t_parent.h_svctx;
    pt_strm_req->pf_strm_nfy     = pt_new_svc_req->t_parent.pf_svc_nfy;
    pt_strm_req->pv_strm_nfy_tag = pt_new_svc_req->t_parent.pv_svc_nfy_tag;

    i4_ret = svctx_get_strm_comp_id(
                    pt_svctx,
                    ST_CLOSED_CAPTION,
                    TRUE,
                    svctx_strm_comp_cc_filter,
                    (VOID*)&pt_new_svc_req->t_cc_filter,
                    &t_comp_id,
                    &pt_strm_req->u.t_scdb.t_rec
                    );
    if (i4_ret == SVCTXR_OK) {
        pt_strm_req->u.t_scdb.t_comp_id = t_comp_id;
    } else {
        pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag = NULL;
        x_memset(&(pt_strm_req->u.t_scdb.t_rec), 0, sizeof(SCDB_REC_T));
    }
    pt_strm_req->e_penging_status = STRM_PENDING_COND_SELECT_STRM;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_replace_svc_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_replace_svc_req(
                    SVCTX_T*                    pt_svctx
                    )
{
    INT32 i4_ret = SVCTXR_OK;

    do {
        INP_SVC_REQ_T* pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
        INP_SVC_REQ_T* pt_pending_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_pending_svc_req;
        BOOL           b_same_cc_req      = TRUE;

        ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));
        ASSERT(pt_pending_svc_req != NULL, ("%s(), pt_pending_svc_req is NULL\r\n", __func__));

#ifdef MW_TV_AV_BYPASS_SUPPORT
        if (inp_svc_req_equals(IS_SVCTX_ASSOCIATE(pt_svctx)?TRUE:FALSE, pt_running_svc_req, pt_pending_svc_req) == FALSE) {
            i4_ret = SVCTXR_FAIL;
            break;
        }
#else /* MW_TV_AV_BYPASS_SUPPORT */
        if (inp_svc_req_equals(FALSE, pt_running_svc_req, pt_pending_svc_req) == FALSE) {
            i4_ret = SVCTXR_FAIL;
            break;
        }
#endif

        _inp_hdlr_replace_video_attributes(
                    pt_svctx,
                    pt_running_svc_req,
                    pt_pending_svc_req
                    );

        if (pt_svctx->e_state == SVCTX_COND_PRESENTING
                && svctx_cc_filter_equals(
                    &pt_running_svc_req->t_cc_filter,
                    &pt_pending_svc_req->t_cc_filter
                    ) == FALSE) {
            b_same_cc_req = FALSE;
            svctx_stop_cc_strm (&(pt_pending_svc_req->t_strm_reqs[ST_CLOSED_CAPTION]));
        }

        pt_running_svc_req->pt_audio_player                = pt_pending_svc_req->pt_audio_player;
        pt_running_svc_req->pt_video_player                = pt_pending_svc_req->pt_video_player;
        pt_running_svc_req->pt_audio_avm                   = pt_pending_svc_req->pt_audio_avm;
        pt_running_svc_req->pt_video_avm                   = pt_pending_svc_req->pt_video_avm;
        pt_running_svc_req->pt_crnt_event                  = pt_pending_svc_req->pt_crnt_event;
        pt_running_svc_req->pt_rating_chker                = pt_pending_svc_req->pt_rating_chker;
        pt_running_svc_req->t_src_info                     = pt_pending_svc_req->t_src_info;
        pt_running_svc_req->b_play_audio_from_video_device = pt_pending_svc_req->b_play_audio_from_video_device;
        pt_running_svc_req->b_hdmi_signal                  = pt_pending_svc_req->b_hdmi_signal;
        pt_running_svc_req->b_replace_video                = pt_pending_svc_req->b_replace_video;
        pt_running_svc_req->b_replace_audio                = pt_pending_svc_req->b_replace_audio;
#ifdef MW_TV_AV_BYPASS_SUPPORT
        pt_running_svc_req->b_combi_switch                 = pt_pending_svc_req->b_combi_switch;
#endif
        pt_pending_svc_req->pt_audio_player                = NULL;
        pt_pending_svc_req->pt_video_player                = NULL;
        pt_pending_svc_req->pt_audio_avm                   = NULL;
        pt_pending_svc_req->pt_video_avm                   = NULL;
        pt_pending_svc_req->pt_crnt_event                  = NULL;
        pt_pending_svc_req->pt_rating_chker                = NULL;
        pt_pending_svc_req->b_play_audio_from_video_device = FALSE;
        pt_pending_svc_req->b_hdmi_signal                  = FALSE;
        pt_pending_svc_req->b_replace_video                = FALSE;
        pt_pending_svc_req->b_replace_audio                = FALSE;
#ifdef MW_TV_AV_BYPASS_SUPPORT
        pt_pending_svc_req->b_combi_switch                 = FALSE;
#endif

        _inp_hdlr_replace_strm_req(pt_running_svc_req, pt_pending_svc_req);

        if (pt_svctx->e_state == SVCTX_COND_PRESENTING && b_same_cc_req == FALSE){
            if (pt_running_svc_req->t_cc_filter.e_cc_type == SM_CC_TYPE_NTSC_LINE21
                    || pt_running_svc_req->t_cc_filter.e_cc_type == SM_CC_TYPE_DTVCC) {
                _inp_hdlr_reload_cc_strm_as_presenting(pt_svctx, pt_running_svc_req);
            }
        }

        _inp_hdlr_resent_nfy_as_replacing(pt_svctx, pt_running_svc_req);
        i4_ret = SVCTXR_OK;
    } while (0);

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_preproc_surf_svc_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_preproc_surf_svc_req(
                    SVCTX_T*                    pt_svctx,
                    UINT32                      ui4_nw_chk_mask,
                    UINT32                      ui4_nw_chk_value,
                    SURF_MODE_T                 e_surf_mode,
                    INP_SVC_REQ_T*              pt_svc_req
                    )
{
    /* QQQQQQQQ */

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_create_surf_svc_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static SVC_REQ_T* _inp_hdlr_create_surf_svc_req(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_svctx,
                    SVCTX_HDLR_T*               pt_hdlr,
                    UINT32                      ui4_nw_chk_mask,
                    UINT32                      ui4_nw_chk_value,
                    SURF_MODE_T                 e_surf_mode,
                    SVL_REC_T*                  pt_svl_rec
                    )
{
    INP_SVC_REQ_T* pt_svc_req = NULL;
    INT32          i4_ret;

    pt_svc_req = (INP_SVC_REQ_T*)svctx_allocate_svc_req(pt_hdlr);
    if (pt_svc_req == NULL) {
        return NULL;
    }

    i4_ret = _inp_hdlr_preproc_surf_svc_req(
                    pt_svctx,
                    ui4_nw_chk_mask,
                    ui4_nw_chk_value,
                    e_surf_mode,
                    pt_svc_req
                    );
    if (i4_ret != SVCTXR_OK) {
        svctx_free_svc_req((SVC_REQ_T*)pt_svc_req);
        return NULL;
    }

    if (pt_svctx->pt_running_svc_req->h_svctx != h_svctx) {
        pt_svc_req->t_parent.h_svctx        = h_svctx;
        pt_svc_req->t_parent.pf_svc_nfy     = NULL;
        pt_svc_req->t_parent.pv_svc_nfy_tag = NULL;
    }

    return ((SVC_REQ_T*)pt_svc_req);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_preproc_svc_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_preproc_svc_req(
                    SVCTX_T*                    pt_svctx,
                    UINT32                      ui4_stream_mode,
                    const SRC_DESC_T*           pt_src_desc,
                    const SNK_DESC_T*           pt_snk_desc,
                    x_svctx_select_nfy_fct      pf_nfy,
                    VOID*                       pv_nfy_tag,
                    INP_SVC_REQ_T*              pt_svc_req
                    )
{
    SIZE_T         z_size;
    INT32          i4_ret;
    CHAR*          ps_src_grp_name    = NULL;
    CHAR*          ps_snk_grp_name    = NULL;
    INP_SVC_REQ_T* pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    UINT8          ui1_idx;

    if (pt_running_svc_req != NULL) {
        for (ui1_idx = 0; ui1_idx < INP_STRM_TYPE_COUNT; ui1_idx++) {
            STREAM_TYPE_T e_strm_type = e_g_inp_strm_types[ui1_idx];

            if (((UINT32)e_strm_type >= INP_STRM_TYPE_COUNT) || (e_strm_type==ST_UNKNOWN))
            {
                ASSERT(0, ("%s(), e_strm_type=%d out of boundary\r\n", __func__, e_strm_type));
                return SVCTXR_NOT_SUPPORT;
            }

            if (x_handle_valid(pt_running_svc_req->t_strm_reqs[e_strm_type].h_stream) == FALSE) {
                continue;
            }
            /* close stream first if new request without related stream */
            if ((pt_running_svc_req->t_parent.ui4_stream_mode & MAKE_BIT_MASK_32(e_strm_type)) == 0
                    || (ui4_stream_mode & MAKE_BIT_MASK_32(e_strm_type)) != 0) {
                continue;
            }

            if (e_strm_type == ST_CLOSED_CAPTION
                    && SVCTX_IS_INVISIBLE_CC_FILTER(pt_running_svc_req->t_cc_filter)) {
                continue;
            }

            i4_ret = x_sm_close_stream(pt_running_svc_req->t_strm_reqs[e_strm_type].h_stream);
            if (i4_ret == SMR_OK || i4_ret == SMR_PENDING) {
                pt_running_svc_req->t_strm_reqs[e_strm_type].h_stream                  = NULL_HANDLE;
                pt_running_svc_req->t_strm_reqs[e_strm_type].u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
            }
        }
    }

    /* 1. check source related parameters */
    if (pt_src_desc == NULL || pt_src_desc->e_type != SRC_TYPE_AVC || pt_src_desc->pv_details == NULL) {
        return SVCTXR_INV_ARG;
    }

    pt_svc_req->t_parent.e_src_type = SRC_TYPE_AVC;
    pt_svc_req->t_src_info.t_av_src = *((SRC_AVC_T*)pt_src_desc->pv_details);

    if (pt_svc_req->t_src_info.t_av_src.e_video_type != DEV_UNKNOWN
            && DEV_GET_GROUP_ID(pt_svc_req->t_src_info.t_av_src.e_video_type) != DEV_AVC) {
        return SVCTXR_INV_ARG;
    }

    if (pt_svc_req->t_src_info.t_av_src.e_audio_type != DEV_UNKNOWN
            && DEV_GET_GROUP_ID(pt_svc_req->t_src_info.t_av_src.e_audio_type) != DEV_AVC) {
        return SVCTXR_INV_ARG;
    }

    /* audio COMBI */
    if (pt_svc_req->t_src_info.t_av_src.e_audio_type == DEV_AVC_COMBI) {
        switch (pt_svc_req->t_src_info.t_av_src.t_audio_hint.e_type) {
        case SRC_AVC_HINT_SPECIFIC_DEVICE_FIRST:
            if (DEV_GET_GROUP_ID(pt_svc_req->t_src_info.t_av_src.t_audio_hint.e_dev_type) != DEV_AVC) {
                return SVCTXR_INV_ARG;
            }

            if (svctx_with_av_capability(pt_svc_req->t_src_info.t_av_src.t_audio_hint.e_dev_type, ST_AUDIO) == FALSE) {
                return SVCTXR_INV_ARG;
            }

            break;
        case SRC_AVC_HINT_NONE:
        case SRC_AVC_HINT_BETTER_QUALITY_DEVICE_FIRST:
            break;
        default:
            break;
        }
    }

    /* video COMBI */
    if (pt_svc_req->t_src_info.t_av_src.e_video_type == DEV_AVC_COMBI) {
        switch (pt_svc_req->t_src_info.t_av_src.t_video_hint.e_type) {
        case SRC_AVC_HINT_SPECIFIC_DEVICE_FIRST:
            if (DEV_GET_GROUP_ID(pt_svc_req->t_src_info.t_av_src.t_video_hint.e_dev_type) != DEV_AVC) {
                return SVCTXR_INV_ARG;
            }

            if (svctx_with_av_capability(pt_svc_req->t_src_info.t_av_src.t_video_hint.e_dev_type, ST_VIDEO) == FALSE) {
                return SVCTXR_INV_ARG;
            }

            break;
        case SRC_AVC_HINT_NONE:
        case SRC_AVC_HINT_BETTER_QUALITY_DEVICE_FIRST:
            break;
        default:
            break;
        }
    }

    /* source name */
    if (pt_running_svc_req != NULL) {
        ps_src_grp_name = pt_running_svc_req->t_src_info.s_src_grp_name;
    }

    if (pt_src_desc != NULL) {
        z_size = x_strlen(pt_src_desc->ps_conn_src_name);
        if (z_size > 0 && z_size <= SVCTX_SRC_NAME_MAX_SIZE) {
            ps_src_grp_name = pt_src_desc->ps_conn_src_name;
        }
    }

    if (ps_src_grp_name == NULL) {
        return SVCTXR_INV_ARG;
    }

    x_strcpy(pt_svc_req->t_src_info.s_src_grp_name, ps_src_grp_name);


    /* scart out parameter */
    if (pt_src_desc->t_scart_out.b_valid == TRUE
            && pt_src_desc->t_scart_out.ui1_num_descs > 0
            && pt_src_desc->t_scart_out.pt_descs != NULL) {
        pt_svc_req->t_parent.t_scart_out.b_valid       = pt_src_desc->t_scart_out.b_valid;
        pt_svc_req->t_parent.t_scart_out.ui1_num_descs = pt_src_desc->t_scart_out.ui1_num_descs;
        pt_svc_req->t_parent.t_scart_out.pt_descs =
                (SCART_OUT_DESC_T*) x_mem_alloc(sizeof(SCART_OUT_DESC_T) * pt_svc_req->t_parent.t_scart_out.ui1_num_descs);
        if (pt_svc_req->t_parent.t_scart_out.pt_descs == NULL) {
            return SVCTXR_NO_RESOURCE;
        }

        x_memcpy(pt_svc_req->t_parent.t_scart_out.pt_descs,
                    pt_src_desc->t_scart_out.pt_descs,
                    sizeof(SCART_OUT_DESC_T) * pt_svc_req->t_parent.t_scart_out.ui1_num_descs
                    );
    } else {
        pt_svc_req->t_parent.t_scart_out.b_valid       = FALSE;
        pt_svc_req->t_parent.t_scart_out.ui1_num_descs = 0;
        pt_svc_req->t_parent.t_scart_out.pt_descs      = NULL;
    }

    /* 2. check sink related parameters */
    /* sink name */
    if (pt_running_svc_req != NULL) {
        ps_snk_grp_name = pt_running_svc_req->t_parent.s_snk_grp_name;
    }

    if (pt_snk_desc != NULL) {
        z_size = x_strlen(pt_snk_desc->ps_snk_grp_name);
        if (z_size > 0 && z_size <= SVCTX_SNK_NAME_MAX_SIZE) {
            ps_snk_grp_name = pt_snk_desc->ps_snk_grp_name;
        }
    }

    i4_ret = svctx_preproc_svc_req_snk_part(
                    pt_svctx,
                    pt_src_desc,
                    pt_snk_desc,
                    ps_snk_grp_name,
                    NULL,
                    (SVC_REQ_T*)pt_svc_req,
                    (SVC_REQ_T*)pt_running_svc_req
                    );
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    /* video stream filter */
    if (pt_snk_desc != NULL && pt_snk_desc->pt_video_strm_fltr_fct != NULL) {
        pt_svc_req->t_strm_reqs[ST_VIDEO].pt_strm_fltr_fct = pt_snk_desc->pt_video_strm_fltr_fct;
        pt_svc_req->t_strm_reqs[ST_VIDEO].pv_strm_fltr_tag = pt_snk_desc->pv_video_strm_fltr_tag;
    } else {
        pt_svc_req->t_strm_reqs[ST_VIDEO].pt_strm_fltr_fct = NULL;
        pt_svc_req->t_strm_reqs[ST_VIDEO].pv_strm_fltr_tag = NULL;
    }

    /* audio stream filter */
    if (pt_snk_desc != NULL && pt_snk_desc->pt_audio_strm_fltr_fct != NULL) {
        pt_svc_req->t_strm_reqs[ST_AUDIO].pt_strm_fltr_fct = pt_snk_desc->pt_audio_strm_fltr_fct;
        pt_svc_req->t_strm_reqs[ST_AUDIO].pv_strm_fltr_tag = pt_snk_desc->pv_audio_strm_fltr_tag;
    } else {
        pt_svc_req->t_strm_reqs[ST_AUDIO].pt_strm_fltr_fct = NULL;
        pt_svc_req->t_strm_reqs[ST_AUDIO].pv_strm_fltr_tag = NULL;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_create_svc_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static SVC_REQ_T* _inp_hdlr_create_svc_req(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_svctx,
                    SVCTX_HDLR_T*               pt_hdlr,
                    UINT32                      ui4_stream_mode,
                    const SRC_DESC_T*           pt_src_desc,
                    const SNK_DESC_T*           pt_snk_desc,
                    x_svctx_select_nfy_fct      pf_nfy,
                    VOID*                       pv_nfy_tag
                    )
{
    INP_SVC_REQ_T* pt_svc_req = NULL;
    INT32          i4_ret;
    UINT8          ui1_idx;

    pt_svc_req = (INP_SVC_REQ_T*)svctx_allocate_svc_req(pt_hdlr); /* QQQQQQ */
    if (pt_svc_req == NULL) {
        return NULL;
    }

    i4_ret = _inp_hdlr_preproc_svc_req(
                    pt_svctx,
                    ui4_stream_mode,
                    pt_src_desc,
                    pt_snk_desc,
                    pf_nfy,
                    pv_nfy_tag,
                    pt_svc_req
                    );
    if (i4_ret != SVCTXR_OK) {
        svctx_free_svc_req((SVC_REQ_T*)pt_svc_req); /* QQQQQQ */
        return NULL;
    }

    pt_svc_req->t_parent.h_svctx                      = h_svctx;
    pt_svc_req->t_parent.pf_svc_nfy                   = pf_nfy;
    pt_svc_req->t_parent.pv_svc_nfy_tag               = pv_nfy_tag;
    pt_svc_req->t_parent.ui4_stream_mode              = ui4_stream_mode;
    pt_svc_req->t_parent.pt_hdlr                      = pt_hdlr;
    pt_svc_req->t_parent.i4_speed                     = SESS_NORMAL_SPEED;
    pt_svc_req->pt_video_player                       = NULL;
    pt_svc_req->pt_audio_player                       = NULL;
    pt_svc_req->pt_audio_avm                          = NULL;
    pt_svc_req->pt_video_avm                          = NULL;
    pt_svc_req->t_src_info.ui2_crnt_video_combi_index = INVALID_CONN_COMBI_INDEX;
    pt_svc_req->t_src_info.ui2_crnt_audio_combi_index = INVALID_CONN_COMBI_INDEX;
    pt_svc_req->t_src_info.e_crnt_video_type          = DEV_UNKNOWN;
    pt_svc_req->t_src_info.e_crnt_audio_type          = DEV_UNKNOWN;

    for (ui1_idx = 0; ui1_idx < INP_STRM_TYPE_COUNT; ui1_idx++) {
        pt_svc_req->t_strm_reqs[ui1_idx].pt_hdlr = pt_hdlr;
    }

    return ((SVC_REQ_T*)pt_svc_req);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_get_scdb_num_recs
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_get_scdb_num_recs(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    UINT16*                     pui2_num_recs
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    UINT32         ui4_db_version     = SCDB_NULL_VER_ID;
    HANDLE_T       h_scdb             = NULL_HANDLE;
    UINT16         ui2_num_recs       = 0;
    INT32          i4_ret             = SVCTXR_OK;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    do {
        h_scdb = _inp_hdlr_get_scdb_handle(pt_running_svc_req, e_strm_type);
        if (x_handle_valid(h_scdb) == FALSE) {
            break;
        }

        i4_ret = x_scdb_get_num_recs(
                    h_scdb,
                    e_strm_type,
                    &ui2_num_recs,
                    &ui4_db_version
                    );
        if (i4_ret != SCDBR_OK) {
            ui2_num_recs = 0;
        }
    } while (0);

    *pui2_num_recs = ui2_num_recs;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_get_cp_info
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_get_cp_info(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    CP_INFO_LIST_T*             pt_cp_info
                    )
{
    return SVCTXR_NOT_SUPPORT;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_get_ca_info
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_get_ca_info(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    SVCTX_CA_INFO_T*            pt_ca_info
                    )
{
    return SVCTXR_NOT_SUPPORT;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_get_using_scdb_info
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_get_using_scdb_info(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    STREAM_COMP_ID_T*           pt_comp_id,
                    SCDB_REC_T*                 pt_scdb_rec
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T       h_scdb             = NULL_HANDLE;
    INT32          i4_ret;
    STRM_REQ_T*    pt_strm_req = NULL;
    STRM_REQ_T*    pt_pending_strm_req = NULL;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    h_scdb = _inp_hdlr_get_scdb_handle(pt_running_svc_req, e_strm_type);
    if (h_scdb == NULL_HANDLE) {
        return SVCTXR_NOT_SUPPORT;
    } else if (x_handle_valid(h_scdb) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    pt_strm_req = &(pt_running_svc_req->t_strm_reqs[e_strm_type]);
    pt_pending_strm_req = pt_running_svc_req->pt_pending_strm_reqs[e_strm_type];

    if (x_handle_valid(pt_strm_req->h_stream) == FALSE
            || pt_strm_req->u.t_scdb.t_comp_id.e_type != e_strm_type
            || pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag == NULL) {
        return SVCTXR_NOT_EXIST;
    }

    if (pt_pending_strm_req)
    {
        if (pt_comp_id != NULL) {
            *pt_comp_id  = pt_pending_strm_req->u.t_scdb.t_comp_id;
        }

        if (pt_scdb_rec != NULL) {
            *pt_scdb_rec = pt_pending_strm_req->u.t_scdb.t_rec;
        }
    }
    else
    {
        i4_ret = x_scdb_get_rec (
                        h_scdb,
                        &(pt_strm_req->u.t_scdb.t_comp_id),
                        &(pt_strm_req->u.t_scdb.t_rec)
                        );
        if (i4_ret != SCDBR_OK) {
            return SVCTXR_FAIL;
        }

        if (pt_comp_id != NULL) {
            *pt_comp_id  = pt_strm_req->u.t_scdb.t_comp_id;
        }

        if (pt_scdb_rec != NULL) {
            *pt_scdb_rec = pt_strm_req->u.t_scdb.t_rec;
        }
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _inp_hdlr_set_sess_attr
 *
 * Description: This function is used to set the attribuites of a session
 *
 * Inputs:  h_svctx           Contains a pointer to a service context.
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
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_set_sess_attr(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    SM_SET_TYPE_T               e_set_type,
                    VOID*                       pv_set_info,
                    SIZE_T                      z_set_info_size
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    PLAYER_T*      pt_player;
    HANDLE_T       h_sess;
    INT32          i4_ret;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    pt_player = _inp_hdlr_get_player(pt_running_svc_req, e_pipe_type);
    h_sess    = player_get_sess_handle(pt_player);

    if (x_handle_valid(h_sess) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    i4_ret = x_sm_set(
                    h_sess,
                    e_set_type,
                    pv_set_info,
                    z_set_info_size
                    );
    return svctx_translate_smr_2_svctxr(i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name: _inp_hdlr_set_strm_attr
 *
 * Description: This function is used to set the attribuites of a stream
 *
 * Inputs:  h_svctx           Contains a pointer to a service context.
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
static INT32 _inp_hdlr_set_strm_attr(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    SM_SET_TYPE_T               e_set_type,
                    VOID*                       pv_set_info,
                    SIZE_T                      z_set_info_size
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    INT32          i4_ret;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (inp_hdlr_is_supported_strm_type(e_strm_type) == FALSE) {
        return SVCTXR_NOT_SUPPORT;
    }

    if (((UINT32)e_strm_type >= INP_STRM_TYPE_COUNT) || (e_strm_type==ST_UNKNOWN))
    {
        ASSERT(0, ("%s(), e_strm_type=%d out of boundary\r\n", __func__, e_strm_type));
        return SVCTXR_NOT_SUPPORT;
    }

    i4_ret = svctx_set_stream_attr(
                    pt_svctx,
                    pt_running_svc_req->t_strm_reqs[e_strm_type].h_stream,
                    e_set_type,
                    pv_set_info,
                    z_set_info_size
                    );
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    switch (e_set_type) {
    case SM_CCH_SET_TYPE_CC_FLTR:
        pt_running_svc_req->t_cc_filter = *((SM_CC_FILTER_T*)pv_set_info);
        pt_running_svc_req->t_strm_reqs[e_strm_type].t_cc_filter = pt_running_svc_req->t_cc_filter;
        break;
    default:
        break;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _inp_hdlr_get_strm_handle
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *-----------------------------------------------------------------------------*/
static BOOL _inp_hdlr_get_strm_handle(
                    SVC_REQ_T*                  pt_svc_req,
                    STREAM_TYPE_T               e_stream_type,
                    HANDLE_T*                   ph_stream
                    )
{
    INP_SVC_REQ_T* pt_inp_svc_req = (INP_SVC_REQ_T*)pt_svc_req;
    UINT8          ui1_idx;

    if (pt_inp_svc_req == NULL) {
        return FALSE;
    }

    if (((UINT32)e_stream_type >= INP_STRM_TYPE_COUNT) || (e_stream_type==ST_UNKNOWN))
    {
        ASSERT(0, ("%s(), e_stream_type=%d out of boundary\r\n", __func__, e_stream_type));
        return FALSE;
    }

    for (ui1_idx = 0; ui1_idx < INP_STRM_TYPE_COUNT; ui1_idx++) {
        if (e_stream_type == e_g_inp_strm_types[ui1_idx]) {
            *ph_stream = pt_inp_svc_req->t_strm_reqs[e_stream_type].h_stream;
            return TRUE;
        }
    }

    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name: _inp_hdlr_get_vbi_broker
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *-----------------------------------------------------------------------------*/
static BOOL _inp_hdlr_get_vbi_broker(
                    SVCTX_T*                        pt_svctx,
                    SVC_REQ_T*                      pt_svc_req,
                    const SVCTX_SM_BROKER_CMD_T*    pt_cmd_sets,
                    SVCTX_VBI_BROKER_T*             pt_vbi_broker
                    )
{
    INP_SVC_REQ_T*  pt_inp_svc_req    = (INP_SVC_REQ_T*)pt_svc_req;
    BOOL            b_found_strm_type = FALSE;
    SRC_OPER_TYPE_T e_src_oper_type;
    SNK_OPER_TYPE_T e_snk_oper_type;
    INT32           i4_ret;
    UINT8           ui1_idx;
    CHAR*           ps_src_name  = NULL;
    UINT32          ui4_cmd_bits = 0;

    if (pt_inp_svc_req == NULL
            || pt_cmd_sets  == NULL
            || pt_vbi_broker == NULL) {
        return FALSE;
    }

    while (pt_cmd_sets->e_code != SVCTX_SM_BROKER_CMD_CODE_END) {
        switch(pt_cmd_sets->e_code) {
        case SVCTX_SM_BROKER_CMD_CODE_DEVICE_TYPE:
            pt_vbi_broker->e_device_type = DEV_GET_GROUP_ID(pt_cmd_sets->u.e_device_type);
            ui4_cmd_bits                |= SVCTX_SM_BROKER_CMD_BIT_DEVICE_TYPE;
            break;
        case SVCTX_SM_BROKER_CMD_CODE_SRC_NAME:
            ps_src_name                 = pt_cmd_sets->u.ps_text;
            ui4_cmd_bits               |= SVCTX_SM_BROKER_CMD_BIT_SRC_NAME;
            break;
        default:
            break;
        }
        pt_cmd_sets++;
    }

    if (pt_vbi_broker->e_device_type != DEV_AVC
            || ui4_cmd_bits != SVCTX_SM_BROKER_CMD_BIT_AVC) {
        return FALSE;
    }

    for (ui1_idx = 0; ui1_idx < INP_STRM_TYPE_COUNT; ui1_idx++) {
        if (ST_VIDEO == e_g_inp_strm_types[ui1_idx]) {
            b_found_strm_type = TRUE;
            break;
        }
    }
    if (b_found_strm_type == FALSE) {
        return FALSE;
    }

    if (x_strcmp(ps_src_name, pt_inp_svc_req->t_src_info.s_src_grp_name) != 0) {
        return FALSE;
    }

    if (svctx_with_event_capability(
                    pt_svctx,
                    pt_inp_svc_req->t_src_info.e_crnt_video_type,
                    DEV_EVN_CAPABILITY_ALL) == FALSE) {
        return FALSE;
    }

    if (pt_inp_svc_req->pt_video_player != NULL) {
        i4_ret = player_get_oper_types(
                    pt_inp_svc_req->pt_video_player,
                    &e_src_oper_type,
                    &e_snk_oper_type
                    );
        if (i4_ret != SVCTXR_OK || e_src_oper_type != SRC_OPER_TYPE_CONNECT) {
            return FALSE;
        }
    }

    pt_vbi_broker->h_stream         = pt_inp_svc_req->t_strm_reqs[ST_VIDEO].h_stream;
    pt_vbi_broker->h_conn           = player_get_conn_handle(pt_inp_svc_req->pt_video_player);
    pt_vbi_broker->b_is_digital_vbi = FALSE;
    return TRUE;
}
/*-----------------------------------------------------------------------------
 * Name: _inp_hdlr_get_current_event_info_len
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *-----------------------------------------------------------------------------*/
#ifndef MW_EDB_SUPPORT
static INT32 _inp_hdlr_get_current_event_info_len (
                    SVCTX_T*                        pt_svctx,
                    SVC_REQ_T*                      pt_svc_req,
                    EVCTX_KEY_TYPE_T                e_key_type,
                    VOID*                           pv_key_info,
                    SIZE_T*                         pz_event_info_len
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = (INP_SVC_REQ_T*)pt_svc_req;
    HANDLE_T       h_evctx = crnt_evn_get_handle(pt_running_svc_req->pt_crnt_event);
    EVCTX_COND_T   e_cond  = EVCTX_COND_UNAVAIL;
    INT32          i4_ret;

    if (pt_svctx->e_block_rule == SVCTX_BLOCK_RULE_INP_CH_PROG_SIGNAL_LOSS
            && e_key_type == EVCTX_KEY_TYPE_RATING
            && pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) {
        /* get rating size from crnt_evn's buffer */
        if (pz_event_info_len == NULL) {
            return SVCTXR_INV_ARG;
        }

        *pz_event_info_len = crnt_evn_get_rating_size(pt_running_svc_req->pt_crnt_event);
    } else {
        if (svctx_with_event_capability(
                    pt_svctx,
                    pt_running_svc_req->t_src_info.e_crnt_video_type,
                    MAKE_BIT_MASK_32(e_key_type)) == FALSE) {
            return SVCTXR_EVN_INFO_NOT_SUPPORT;
        }

        if (x_handle_valid(h_evctx) == FALSE) {
            return SVCTXR_EVN_INFO_NOT_FOUND;
        }

        i4_ret = x_evctx_get_event_info_len(
                    h_evctx,
                    e_key_type,
                    pv_key_info,
                    pz_event_info_len,
                    &e_cond
                    );
        if (i4_ret == EVCTXR_INFO_NOT_FOUND) {
            return SVCTXR_EVN_INFO_NOT_FOUND;
        }

        if (i4_ret != EVCTXR_OK) {
            return SVCTXR_FAIL;
        }
    }
    return SVCTXR_OK;
}
#else
static INT32 _inp_hdlr_get_current_event_info_len (
                    SVCTX_T*                        pt_svctx,
                    SVC_REQ_T*                      pt_svc_req,
                    EDB_KEY_TYPE_T                  e_key_type,
                    VOID*                           pv_key_info,
                    SIZE_T*                         pz_event_info_len
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = (INP_SVC_REQ_T*)pt_svc_req;
    HANDLE_T       h_edb = crnt_evn_get_handle(pt_running_svc_req->pt_crnt_event);
    INT32          i4_ret;

    if (pt_svctx->e_block_rule == SVCTX_BLOCK_RULE_INP_CH_PROG_SIGNAL_LOSS
            && e_key_type == EDB_KEY_TYPE_RATING_LIST_BY_ID
            && pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) {
        /* get rating size from crnt_evn's buffer */
        if (pz_event_info_len == NULL) {
            return SVCTXR_INV_ARG;
        }

        *pz_event_info_len = crnt_evn_get_rating_size(pt_running_svc_req->pt_crnt_event);
    } else {
        if (svctx_with_event_capability(
                    pt_svctx,
                    pt_running_svc_req->t_src_info.e_crnt_video_type,
                    MAKE_BIT_MASK_32(e_key_type)) == FALSE) {
            return SVCTXR_EVN_INFO_NOT_SUPPORT;
        }

        if (x_handle_valid(h_edb) == FALSE) {
            return SVCTXR_EVN_INFO_NOT_FOUND;
        }

        i4_ret = x_edb_get_info_len(
                    h_edb,
                    e_key_type,
                    pv_key_info,
                    pz_event_info_len
                    );
        if (i4_ret == EDBR_INFO_NOT_FOUND) {
            return SVCTXR_EVN_INFO_NOT_FOUND;
        }

        if (i4_ret != EDBR_OK) {
            return SVCTXR_FAIL;
        }
    }
    
    return SVCTXR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: _inp_hdlr_get_current_event_info
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *-----------------------------------------------------------------------------*/
#ifndef MW_EDB_SUPPORT
static INT32 _inp_hdlr_get_current_event_info (
                    SVCTX_T*                        pt_svctx,
                    SVC_REQ_T*                      pt_svc_req,
                    EVCTX_KEY_TYPE_T                e_key_type,
                    VOID*                           pv_key_info,
                    SIZE_T*                         pz_event_info_len,
                    VOID*                           pv_event_info
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = (INP_SVC_REQ_T*)pt_svc_req;
    HANDLE_T       h_evctx = crnt_evn_get_handle(pt_running_svc_req->pt_crnt_event);
    EVCTX_COND_T   e_cond  = EVCTX_COND_UNAVAIL;
    SIZE_T         z_orig_len;
    INT32          i4_ret;

    if (pt_svctx->e_block_rule == SVCTX_BLOCK_RULE_INP_CH_PROG_SIGNAL_LOSS
            && e_key_type == EVCTX_KEY_TYPE_RATING
            && pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) {
        /* get rating from crnt_evn's buffer */
        if (pz_event_info_len == NULL || pv_event_info == NULL) {
            return SVCTXR_INV_ARG;
        }

        z_orig_len = crnt_evn_get_rating_size(pt_running_svc_req->pt_crnt_event);

        if (z_orig_len > (*pz_event_info_len)) {
            return SVCTXR_INV_ARG;
        }

        *pz_event_info_len = z_orig_len;
        x_memcpy(pv_event_info,
                    crnt_evn_get_rating(pt_running_svc_req->pt_crnt_event),
                    z_orig_len
                    );
    } else {
        if (svctx_with_event_capability(
                    pt_svctx,
                    pt_running_svc_req->t_src_info.e_crnt_video_type,
                    MAKE_BIT_MASK_32(e_key_type)) == FALSE) {
            return SVCTXR_EVN_INFO_NOT_SUPPORT;
        }

        if (x_handle_valid(h_evctx) == FALSE) {
            return SVCTXR_EVN_INFO_NOT_FOUND;
        }

        i4_ret = x_evctx_get_event_info(
                    h_evctx,
                    e_key_type,
                    pv_key_info,
                    pz_event_info_len,
                    pv_event_info,
                    &e_cond
                    );
        if (i4_ret == EVCTXR_INFO_NOT_FOUND) {
            return SVCTXR_EVN_INFO_NOT_FOUND;
        }

        if (i4_ret != EVCTXR_OK) {
            return SVCTXR_FAIL;
        }
    }
    return SVCTXR_OK;
}
#else
static INT32 _inp_hdlr_get_current_event_info (
                    SVCTX_T*                        pt_svctx,
                    SVC_REQ_T*                      pt_svc_req,
                    EDB_KEY_TYPE_T                  e_key_type,
                    VOID*                           pv_key_info,
                    SIZE_T*                         pz_event_info_len,
                    VOID*                           pv_event_info
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = (INP_SVC_REQ_T*)pt_svc_req;
    HANDLE_T       h_edb = crnt_evn_get_handle(pt_running_svc_req->pt_crnt_event);
    SIZE_T         z_orig_len;
    INT32          i4_ret;

    if (pt_svctx->e_block_rule == SVCTX_BLOCK_RULE_INP_CH_PROG_SIGNAL_LOSS
            && e_key_type == EDB_KEY_TYPE_RATING_LIST_BY_ID
            && pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) {
        /* get rating from crnt_evn's buffer */
        if (pz_event_info_len == NULL || pv_event_info == NULL) {
            return SVCTXR_INV_ARG;
        }

        z_orig_len = crnt_evn_get_rating_size(pt_running_svc_req->pt_crnt_event);

        if (z_orig_len > (*pz_event_info_len)) {
            return SVCTXR_INV_ARG;
        }

        *pz_event_info_len = z_orig_len;
        x_memcpy(pv_event_info,
                    crnt_evn_get_rating(pt_running_svc_req->pt_crnt_event),
                    z_orig_len
                    );
    } else {
        if (svctx_with_event_capability(
                    pt_svctx,
                    pt_running_svc_req->t_src_info.e_crnt_video_type,
                    MAKE_BIT_MASK_32(e_key_type)) == FALSE) {
            return SVCTXR_EVN_INFO_NOT_SUPPORT;
        }

        if (x_handle_valid(h_edb) == FALSE) {
            return SVCTXR_EVN_INFO_NOT_FOUND;
        }

        i4_ret = x_edb_get_info(
                    h_edb,
                    e_key_type,
                    pv_key_info,
                    pz_event_info_len,
                    pv_event_info
                    );
        if (i4_ret == EDBR_INFO_NOT_FOUND) {
            return SVCTXR_EVN_INFO_NOT_FOUND;
        }

        if (i4_ret != EDBR_OK) {
            return SVCTXR_FAIL;
        }
    }
    
    return SVCTXR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: _inp_hdlr_is_handler
 *
 * Description: This API is used to check the handler is response for this request
 * Inputs:  e_src_type         The source type
 *
 * Outputs: -
 *
 * Returns: TRUE               It is handler.
 *          FALSE              It is not handler
 *-----------------------------------------------------------------------------*/
static BOOL _inp_hdlr_is_handler(
                    SRC_TYPE_T                      e_src_type
                    )
{
    switch(e_src_type) {
    case SRC_TYPE_AVC:
        return TRUE;
    default:
        break;
    }

    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name: _inp_hdlr_get_signal_level
 *
 * Description: This API is used to get tuner's signal level. The range
 *              of the signal level is from 0 to 100.
 * Inputs:  pt_svctx           Contains a pointer to a service context.
 *
 * Outputs: pui1_signal_level  References to the variable containing signal level
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_ARG     pui1_signal_level is NULL
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_get_signal_level(
                    SVCTX_T*                    pt_svctx,
                    UINT8*                      pui1_signal_level
                    )
{
    return SVCTXR_NOT_SUPPORT;
}
/*-----------------------------------------------------------------------------
 * Name: _inp_hdlr_get_dbm_signal_level
 *
 * Description: This API is used to get tuner's dbm signal level.
 *
 * Inputs:  pt_svctx           Contains a pointer to a service context.
 *
 * Outputs: pi2_dbm_signal_level  References to the variable containing signal level
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pi2_dbm_signal_level is NULL
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_get_dbm_signal_level(
                    SVCTX_T*                    pt_svctx,
                    INT16*                      pi2_dbm_signal_level
                    )
{
    return SVCTXR_NOT_SUPPORT;
}
/*-----------------------------------------------------------------------------
 * Name: _inp_hdlr_get_sess_attr
 *
 * Description: This function is used to get the attribuites of a session
 *
 * Inputs:  pt_svctx           Contains a pointer to a service context.
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
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_get_sess_attr(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    SM_GET_TYPE_T               e_get_type,
                    VOID*                       pv_get_info,
                    SIZE_T*                     pui4_get_info_size
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    PLAYER_T*      pt_player;
    HANDLE_T       h_sess;
    INT32          i4_ret;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    pt_player = _inp_hdlr_get_player(pt_running_svc_req, e_pipe_type);
    h_sess    = player_get_sess_handle(pt_player);

    if (x_handle_valid(h_sess) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    i4_ret = x_sm_get(
                    h_sess,
                    e_get_type,
                    pv_get_info,
                    pui4_get_info_size
                    );
    return svctx_translate_smr_2_svctxr(i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name: _inp_hdlr_get_strm_attr
 *
 * Description: This function is used to get the attribuites of a stream
 *
 * Inputs:  pt_svctx           Contains a pointer to a service context.
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
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_get_strm_attr(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    SM_GET_TYPE_T               e_get_type,
                    VOID*                       pv_get_info,
                    SIZE_T*                     pui4_get_info_size
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    INT32          i4_ret             = SVCTXR_OK;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (inp_hdlr_is_supported_strm_type(e_strm_type) == FALSE) {
        return SVCTXR_NOT_SUPPORT;
    }

    if (((UINT32)e_strm_type >= INP_STRM_TYPE_COUNT) || (e_strm_type==ST_UNKNOWN))
    {
        ASSERT(0, ("%s(), e_strm_type=%d out of boundary\r\n", __func__, e_strm_type));
        return SVCTXR_NOT_SUPPORT;
    }

    if (x_handle_valid(pt_running_svc_req->t_strm_reqs[e_strm_type].h_stream) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    i4_ret = x_sm_get(
                    pt_running_svc_req->t_strm_reqs[e_strm_type].h_stream,
                    e_get_type,
                    pv_get_info,
                    pui4_get_info_size
                    );
    return (i4_ret == SMR_OK ? SVCTXR_OK : SVCTXR_FAIL);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_get_strm_comp_id
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_get_strm_comp_id(
                    SVCTX_T*                     pt_svctx,
                    STREAM_TYPE_T                e_strm_type,
                    BOOL                         b_auto_version_update,
                    x_svctx_strm_comp_filter_fct pf_filter,
                    VOID*                        pv_tag,
                    STREAM_COMP_ID_T*            pt_comp_id,
                    SCDB_REC_T*                  pt_scdb_rec
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T       h_scdb             = NULL_HANDLE;
    INT32          i4_ret             = SVCTXR_OK;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (pf_filter == NULL || pt_comp_id == NULL) {
        return SVCTXR_INV_ARG;
    }

    do {
        h_scdb = _inp_hdlr_get_scdb_handle(pt_running_svc_req, e_strm_type);
        if (x_handle_valid(h_scdb) == FALSE) {
            i4_ret = SVCTXR_NOT_EXIST;
            break;
        }

        i4_ret = inp_hdlr_iterate_strm_comp_id(
                    h_scdb,
                    e_strm_type,
                    b_auto_version_update,
                    pf_filter,
                    pv_tag,
                    pt_comp_id,
                    pt_scdb_rec
                    );
        if (i4_ret == SVCTXR_OK) {
            return i4_ret;
        }
    } while (0);

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_create_generic_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_create_generic_strm_req(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_svctx,
                    STREAM_COMP_ID_T*           pt_comp_id,
                    x_svctx_select_nfy_fct      pf_nfy,
                    VOID*                       pv_nfy_tag,
                    STRM_REQ_T**                ppt_strm_req
                    )
{
    STRM_REQ_T*    pt_strm_req        = NULL;
    INP_SVC_REQ_T* pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T       h_scdb             = NULL_HANDLE;
    INT32          i4_ret;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (ppt_strm_req == NULL || pt_comp_id == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (inp_hdlr_is_supported_strm_type(pt_comp_id->e_type) == FALSE) {
        return SVCTXR_NOT_SUPPORT;
    }

    h_scdb = _inp_hdlr_get_scdb_handle(pt_running_svc_req, pt_comp_id->e_type);
    if (h_scdb == NULL_HANDLE) {
        return SVCTXR_NOT_SUPPORT;
    } else if (x_handle_valid(h_scdb) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    pt_strm_req = svctx_allocate_strm_req(pt_svctx->pt_running_svc_req->pt_hdlr);
    if (pt_strm_req == NULL) {
        return SVCTXR_NO_RESOURCE;
    }

    pt_strm_req->h_svctx             = h_svctx;
    pt_strm_req->pf_strm_nfy         = pf_nfy;
    pt_strm_req->pv_strm_nfy_tag     = pv_nfy_tag;
    pt_strm_req->u.t_scdb.t_comp_id  = *pt_comp_id;

    i4_ret = x_scdb_get_rec (
                    h_scdb,
                    &(pt_strm_req->u.t_scdb.t_comp_id),
                    &(pt_strm_req->u.t_scdb.t_rec)
                    );
    if (i4_ret == SCDBR_REC_NOT_FOUND) {
        svctx_free_strm_req(pt_strm_req);
        return SVCTXR_NO_RECORD;
    }

    if (i4_ret != SCDBR_OK) {
        svctx_free_strm_req(pt_strm_req);
        return SVCTXR_FAIL;
    }

    *ppt_strm_req = pt_strm_req;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_create_cc_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_create_cc_strm_req(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_svctx,
                    HANDLE_T                    h_cc_gl_plane,
                    SM_CC_FILTER_T*             pt_cc_filter,
                    x_svctx_select_nfy_fct      pf_nfy,
                    VOID*                       pv_nfy_tag,
                    STRM_REQ_T**                ppt_strm_req
                    )
{
    STRM_REQ_T* pt_strm_req = NULL;

    if (ppt_strm_req == NULL || x_handle_valid(h_cc_gl_plane) == FALSE || pt_cc_filter == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (inp_hdlr_is_supported_strm_type(ST_CLOSED_CAPTION) == FALSE) {
        return SVCTXR_NOT_SUPPORT;
    }

    pt_strm_req = svctx_allocate_strm_req(pt_svctx->pt_running_svc_req->pt_hdlr);
    if (pt_strm_req == NULL) {
        return SVCTXR_NO_RESOURCE;
    }

    pt_strm_req->h_svctx                          = h_svctx;
    pt_strm_req->pf_strm_nfy                      = pf_nfy;
    pt_strm_req->pv_strm_nfy_tag                  = pv_nfy_tag;
    pt_strm_req->t_cc_filter                      = *pt_cc_filter;
    pt_strm_req->h_gl_plane                       = h_cc_gl_plane;
    pt_strm_req->u.t_scdb.t_comp_id.e_type        = ST_CLOSED_CAPTION;
    pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag = NULL;

    *ppt_strm_req = pt_strm_req;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_create_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_create_strm_req(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_svctx,
                    VOID*                       pv_info,
                    x_svctx_select_nfy_fct      pf_nfy,
                    VOID*                       pv_nfy_tag,
                    STRM_REQ_T**                ppt_strm_req
                    )
{
    CH_STRM_SEL_INFO_T* pt_info = (CH_STRM_SEL_INFO_T*) pv_info;
    INT32 i4_ret;

    if (pt_info == NULL) {
        return SVCTXR_INV_ARG;
    }

    switch (pt_info->e_type) {
    case CH_STRM_SEL_INFO_TYPE_AUDIO:
        i4_ret = SVCTXR_NOT_SUPPORT;
        break;
    case CH_STRM_SEL_INFO_TYPE_CC:
        i4_ret = _inp_hdlr_create_cc_strm_req(
                    pt_svctx,
                    h_svctx,
                    pt_info->u.t_cc.h_gl_plane,
                    pt_info->u.t_cc.pt_cc_filter,
                    pf_nfy,
                    pv_nfy_tag,
                    ppt_strm_req
                    );
        break;
    case CH_STRM_SEL_INFO_TYPE_GENERIC:
        i4_ret = _inp_hdlr_create_generic_strm_req(
                    pt_svctx,
                    h_svctx,
                    pt_info->u.t_generic.pt_comp_id,
                    pf_nfy,
                    pv_nfy_tag,
                    ppt_strm_req
                    );
        break;
    default:
        i4_ret = SVCTXR_INV_ARG;
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_deinit
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static VOID _inp_hdlr_deinit(
                    VOID
                    )
{
    inp_svc_req_deinit();
    inp_strm_req_deinit();
    avm_deinit();

    b_g_init = FALSE;
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
static INT32 _inp_hdlr_iterate_devices_in_combi(
                    AV_MONITOR_T*               pt_this,
                    UINT16                      ui2_num_dev,
                    UINT16                      ui2_idx,
                    DEVICE_TYPE_T               e_dev_type,
                    AVM_SIGNAL_STATUS_T         e_video_signal_status,
                    AVM_SIGNAL_STATUS_T         e_audio_signal_status,
                    VOID*                       pv_tag
                    )
{
    BOOL b_with_video;
    BOOL b_with_audio;

    INP_ITERATE_DEVICES_IN_COMBI_INFO_T* pt_info    = (INP_ITERATE_DEVICES_IN_COMBI_INFO_T*)pv_tag;
    BOOL                                 b_in_using = FALSE;

    if (pt_info->e_query_type == QUERY_TYPE_VIDEO) {
        if (pt_info->pt_running_svc_req->t_src_info.e_crnt_video_type == e_dev_type
                && pt_info->pt_running_svc_req->t_src_info.ui2_crnt_video_combi_index == ui2_idx) {
            b_in_using = TRUE;
        }
    } else if (pt_info->e_query_type == QUERY_TYPE_AUDIO) {
        if (pt_info->pt_running_svc_req->t_src_info.e_crnt_audio_type == e_dev_type
                && pt_info->pt_running_svc_req->t_src_info.ui2_crnt_audio_combi_index == ui2_idx) {
            b_in_using = TRUE;
        }
    }

    b_with_video = (AVM_SIGNAL_STATUS_WITH_SIGNAL == e_video_signal_status) ? TRUE : FALSE;
    b_with_audio = (AVM_SIGNAL_STATUS_WITH_SIGNAL == e_audio_signal_status) ? TRUE : FALSE;

    pt_info->pf_iterate_fct (
                    ui2_num_dev,
                    ui2_idx,
                    e_dev_type,
                    b_in_using,
                    b_with_video,
                    b_with_audio
                    );

    return SVCTXR_CONTINUE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_set
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_get(
                    SVCTX_T*                    pt_svctx,
                    SVCTX_GET_TYPE_T            e_get_type,
                    VOID*                       pv_get_info,
                    SIZE_T*                     pz_get_info_size
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    INT32          i4_ret             = SVCTXR_OK;
    INP_ITERATE_DEVICES_IN_COMBI_INFO_T t_info;

    if (pt_running_svc_req == NULL)  {
        return SVCTXR_WRONG_STATE;
    }

    switch (e_get_type) {
    case SVCTX_COMMON_GET_TYPE_CRNT_VIDEO_DEVICE_TYPE:
        if ((*pz_get_info_size) != sizeof(DEVICE_TYPE_T) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        *((DEVICE_TYPE_T*)pv_get_info) = pt_running_svc_req->t_src_info.e_crnt_video_type;
        break;
    case SVCTX_COMMON_GET_TYPE_CRNT_AUDIO_DEVICE_TYPE:
        if ((*pz_get_info_size) != sizeof(DEVICE_TYPE_T) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        *((DEVICE_TYPE_T*)pv_get_info) = pt_running_svc_req->t_src_info.e_crnt_audio_type;
        break;
    case SVCTX_INP_GET_TYPE_ITERATE_DEVICES_IN_VIDEO_COMBI:
        if ((*pz_get_info_size) != sizeof(DEVICE_TYPE_T) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        if (pt_running_svc_req->pt_video_avm == NULL
                || pt_running_svc_req->t_src_info.t_av_src.e_video_type != DEV_AVC_COMBI) {
            i4_ret = SVCTXR_NOT_EXIST;
            break;
        }

        t_info.e_query_type       = QUERY_TYPE_VIDEO;
        t_info.pf_iterate_fct     = (x_inp_hdlr_iterate_devices_in_combi_fct)pv_get_info;
        t_info.pt_running_svc_req = pt_running_svc_req;

        avm_iterate_devs(
                    pt_running_svc_req->pt_video_avm,
                    _inp_hdlr_iterate_devices_in_combi,
                    &t_info
                    );
        break;
    case SVCTX_INP_GET_TYPE_ITERATE_DEVICES_IN_AUDIO_COMBI:
        if ((*pz_get_info_size) != sizeof(DEVICE_TYPE_T) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        if (pt_running_svc_req->pt_audio_avm == NULL
                || pt_running_svc_req->t_src_info.t_av_src.e_audio_type != DEV_AVC_COMBI) {
            i4_ret = SVCTXR_NOT_EXIST;
            break;
        }

        t_info.e_query_type       = QUERY_TYPE_AUDIO;
        t_info.pf_iterate_fct     = (x_inp_hdlr_iterate_devices_in_combi_fct)pv_get_info;
        t_info.pt_running_svc_req = pt_running_svc_req;

        avm_iterate_devs(
                    pt_running_svc_req->pt_audio_avm,
                    _inp_hdlr_iterate_devices_in_combi,
                    &t_info
                    );
        break;
    case SVCTX_COMMON_GET_TYPE_BLOCK_COND:
        if ((*pz_get_info_size) != sizeof(SVC_BLOCK_COND_T) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        if (pt_running_svc_req != NULL) {
            *((SVC_BLOCK_COND_T*)pv_get_info) = rating_chker_get_block_cond(pt_running_svc_req->pt_rating_chker);
        } else {
            *((SVC_BLOCK_COND_T*)pv_get_info) = SVC_BLOCK_COND_PASSED;
        }
        break;
    case SVCTX_INP_GET_TYPE_VIDEO_DEVICE_AUDIO_COND:
        if ((*pz_get_info_size) != sizeof(VIDEO_DEVICE_AUDIO_COND_T) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        if (pt_running_svc_req != NULL
                && pt_svctx->e_signal_status == SIGNAL_STATUS_LOCKED
                && x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream) == TRUE) {
            if (inp_hdlr_with_audio_from_video_player(pt_svctx, pt_running_svc_req) == TRUE) {
                *((VIDEO_DEVICE_AUDIO_COND_T*)pv_get_info) = VIDEO_DEVICE_AUDIO_COND_WITH_AUDIO;
            } else {
                *((VIDEO_DEVICE_AUDIO_COND_T*)pv_get_info) = VIDEO_DEVICE_AUDIO_COND_NO_AUDIO;
            }
        } else {
            *((VIDEO_DEVICE_AUDIO_COND_T*)pv_get_info) = VIDEO_DEVICE_AUDIO_COND_UNKNOWN;
        }
        break;
    case SVCTX_INP_GET_TYPE_SRC_AVC_INFO:
        if ((*pz_get_info_size) != sizeof(SRC_AVC_T) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        if (pt_running_svc_req->t_parent.e_src_type != SRC_TYPE_AVC)
        {
            i4_ret = SVCTXR_NOT_EXIST;
            break;
        }

        *((SRC_AVC_T*) pv_get_info) = pt_running_svc_req->t_src_info.t_av_src;
        break;
    default:
        i4_ret = SVCTXR_NOT_SUPPORT;
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_next_device_in_combi
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _inp_hdlr_next_device_in_combi(
                    AV_MONITOR_T*               pt_this,
                    UINT16                      ui2_num_dev,
                    UINT16                      ui2_idx,
                    DEVICE_TYPE_T               e_dev_type,
                    AVM_SIGNAL_STATUS_T         e_video_signal_status,
                    AVM_SIGNAL_STATUS_T         e_audio_signal_status,
                    VOID*                       pv_tag
                    )
{
    INP_SRC_AVM_QUERY_INFO_T* pt_info       = (INP_SRC_AVM_QUERY_INFO_T*)pv_tag;
    BOOL                      b_with_signal = FALSE;

    if (pt_info->u.b_need_signaled == TRUE) {
        if (ui2_idx == 0) {
            pt_info->ui2_first_index  = ui2_num_dev;
            pt_info->e_first_dev_type = DEV_UNKNOWN;
        }

        if ((pt_info->e_query_type == QUERY_TYPE_VIDEO && e_video_signal_status == AVM_SIGNAL_STATUS_WITH_SIGNAL)
                || (pt_info->e_query_type == QUERY_TYPE_AUDIO && e_audio_signal_status == AVM_SIGNAL_STATUS_WITH_SIGNAL)) {
            b_with_signal = TRUE;
        }

        if (b_with_signal == TRUE) {
            if (pt_info->ui2_target_index == INVALID_CONN_COMBI_INDEX
                    && pt_info->e_target_type == DEV_UNKNOWN) {
                pt_info->ui2_target_index = ui2_idx;
                pt_info->e_target_type    = e_dev_type;
                return SVCTXR_DONE;
            }

            if (pt_info->ui2_first_index == ui2_num_dev) {
                pt_info->ui2_first_index  = ui2_idx;
                pt_info->e_first_dev_type = e_dev_type;
            }
        }

        if (pt_info->ui2_target_index == ui2_idx && pt_info->e_target_type == e_dev_type) {
            pt_info->ui2_target_index = INVALID_CONN_COMBI_INDEX;
            pt_info->e_target_type    = DEV_UNKNOWN;
        }

        /* last device, find nothing */
        if ((UINT16) (ui2_idx + 1) == ui2_num_dev && pt_info->ui2_first_index < ui2_num_dev) {
            pt_info->ui2_target_index = pt_info->ui2_first_index;
            pt_info->e_target_type    = pt_info->e_first_dev_type;
            return SVCTXR_DONE;
        }
    } else {
        if (ui2_idx == 0) {
            pt_info->ui2_first_index  = ui2_idx;
            pt_info->e_first_dev_type = e_dev_type;
        }

        if (pt_info->ui2_target_index == INVALID_CONN_COMBI_INDEX && pt_info->e_target_type == DEV_UNKNOWN) {
            pt_info->ui2_target_index = ui2_idx;
            pt_info->e_target_type    = e_dev_type;
            return SVCTXR_DONE;
        }

        if (pt_info->ui2_target_index == ui2_idx && pt_info->e_target_type == e_dev_type) {
            pt_info->ui2_target_index = INVALID_CONN_COMBI_INDEX;
            pt_info->e_target_type    = DEV_UNKNOWN;

            if ((UINT16) (ui2_idx + 1) == ui2_num_dev) {
                pt_info->ui2_target_index = pt_info->ui2_first_index;
                pt_info->e_target_type    = pt_info->e_first_dev_type;
                return SVCTXR_DONE;
            }
        }
    }
    return SVCTXR_CONTINUE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_specific_device_in_combi
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _inp_hdlr_specific_device_in_combi(
                    AV_MONITOR_T*               pt_this,
                    UINT16                      ui2_num_dev,
                    UINT16                      ui2_idx,
                    DEVICE_TYPE_T               e_dev_type,
                    AVM_SIGNAL_STATUS_T         e_video_signal_status,
                    AVM_SIGNAL_STATUS_T         e_audio_signal_status,
                    VOID*                       pv_tag
                    )
{
    INP_SRC_AVM_QUERY_INFO_T* pt_info = (INP_SRC_AVM_QUERY_INFO_T*)pv_tag;

    if (e_dev_type == pt_info->u.t_hint.e_dev_type) {
        pt_info->ui2_target_index = ui2_idx;
        pt_info->e_target_type    = e_dev_type;
        return SVCTXR_DONE;
    }

    return SVCTXR_CONTINUE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_operate_combi_video_device
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_operate_combi_video_device(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    avm_signal_status_parse_fct pf_parse_fct,
                    INP_SRC_AVM_QUERY_INFO_T*   pt_info
                    )
{
    INT32 i4_ret;

    i4_ret = avm_iterate_devs(
                    pt_running_svc_req->pt_video_avm,
                    pf_parse_fct,
                    pt_info
                    );
    if (i4_ret != SVCTXR_DONE) {
        return SVCTXR_NO_RECORD;
    }

    if (pt_running_svc_req->t_src_info.ui2_crnt_video_combi_index == pt_info->ui2_target_index
            && pt_running_svc_req->t_src_info.e_crnt_video_type == pt_info->e_target_type) {
        if (player_get_conn_cond(pt_running_svc_req->pt_video_player) != CM_COND_DISCONNECTED) { /* QQQQQQQQ */
            return SVCTXR_OK;
        }
    }

    pt_running_svc_req->t_src_info.t_av_src.t_video_hint.e_type     = SRC_AVC_HINT_SPECIFIC_DEVICE_ONLY;
    pt_running_svc_req->t_src_info.t_av_src.t_video_hint.e_dev_type = pt_info->e_target_type;
    pt_running_svc_req->t_src_info.ui2_crnt_video_combi_index       = pt_info->ui2_target_index;
    pt_running_svc_req->t_src_info.e_crnt_video_type                = pt_info->e_target_type;

    i4_ret = inp_hdlr_stop_video_player(
                    pt_svctx,
                    pt_running_svc_req,
                    FALSE
                    );
    if (i4_ret == SVCTXR_OK) {
        i4_ret = inp_hdlr_play_video_player(
                    pt_svctx,
                    pt_running_svc_req
                    );
    } else if (i4_ret == SVCTXR_PENDING) {
        pt_running_svc_req->b_replace_video = TRUE;
        i4_ret = SVCTXR_OK;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_operate_combi_audio_device
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_operate_combi_audio_device(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    avm_signal_status_parse_fct pf_parse_fct,
                    INP_SRC_AVM_QUERY_INFO_T*   pt_info
                    )
{
    INT32 i4_ret;

    i4_ret = avm_iterate_devs(
                    pt_running_svc_req->pt_audio_avm,
                    pf_parse_fct,
                    pt_info
                    );
    if (i4_ret != SVCTXR_DONE) {
        return SVCTXR_NO_RECORD;
    }

    if (pt_running_svc_req->t_src_info.ui2_crnt_audio_combi_index == pt_info->ui2_target_index
            && pt_running_svc_req->t_src_info.e_crnt_audio_type == pt_info->e_target_type) {
        if (player_get_conn_cond(pt_running_svc_req->pt_audio_player) != CM_COND_DISCONNECTED) { /* QQQQQQQQ */
            return SVCTXR_OK;
        }
    }

    pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_type     = SRC_AVC_HINT_SPECIFIC_DEVICE_ONLY;
    pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_dev_type = pt_info->e_target_type;
    pt_running_svc_req->t_src_info.ui2_crnt_audio_combi_index       = pt_info->ui2_target_index;
    pt_running_svc_req->t_src_info.e_crnt_audio_type                = pt_info->e_target_type;

    i4_ret = inp_hdlr_stop_audio_player(
                    pt_svctx,
                    pt_running_svc_req
                    );
    if (i4_ret == SVCTXR_OK) {
        i4_ret = inp_hdlr_play_audio_player(
                    pt_svctx,
                    pt_running_svc_req
                    );
    } else if (i4_ret == SVCTXR_PENDING) {
        pt_running_svc_req->b_replace_audio = TRUE;
        i4_ret = SVCTXR_OK;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_set
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_set(
                    SVCTX_T*                    pt_svctx,
                    SVCTX_SET_TYPE_T            e_set_type,
                    VOID*                       pv_set_info,
                    SIZE_T                      z_set_info_size
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    INT32          i4_ret             = SVCTXR_OK;
    INP_SRC_AVM_QUERY_INFO_T t_info;

    if (pt_running_svc_req == NULL)  {
        return SVCTXR_WRONG_STATE;
    }

    switch (e_set_type) {
    case SVCTX_COMMON_SET_VIDEO_MUTE:
        break;
    case SVCTX_INP_SET_TYPE_SELECT_DEVICE_IN_VIDEO_COMBI:
        if (pt_running_svc_req->pt_video_avm == NULL
                || pt_running_svc_req->t_src_info.t_av_src.e_video_type != DEV_AVC_COMBI) {
            i4_ret = SVCTXR_NOT_EXIST;
            break;
        }

        if (z_set_info_size != sizeof(DEVICE_TYPE_T) || pv_set_info == NULL) {
            i4_ret = SVCTXR_INV_SET_INFO;
            break;
        }

        t_info.e_query_type        = QUERY_TYPE_VIDEO;
        t_info.u.t_hint.e_type     = SRC_AVC_HINT_SPECIFIC_DEVICE_ONLY;
        t_info.u.t_hint.e_dev_type = *((DEVICE_TYPE_T*)pv_set_info);

        i4_ret = _inp_hdlr_operate_combi_video_device(
                    pt_svctx,
                    pt_running_svc_req,
                    _inp_hdlr_specific_device_in_combi,
                    &t_info
                    );
        break;
    case SVCTX_INP_SET_TYPE_NEXT_SIGNALED_IN_VIDEO_COMBI:
    case SVCTX_INP_SET_TYPE_NEXT_DEVICE_IN_VIDEO_COMBI:
        if (pt_running_svc_req->pt_video_avm == NULL
                || pt_running_svc_req->t_src_info.t_av_src.e_video_type != DEV_AVC_COMBI) {
            i4_ret = SVCTXR_NOT_EXIST;
            break;
        }

        t_info.e_query_type      = QUERY_TYPE_VIDEO;
        t_info.u.b_need_signaled = e_set_type == SVCTX_INP_SET_TYPE_NEXT_SIGNALED_IN_VIDEO_COMBI ? TRUE : FALSE;
        t_info.ui2_target_index  = pt_running_svc_req->t_src_info.ui2_crnt_video_combi_index;
        t_info.e_target_type     = pt_running_svc_req->t_src_info.e_crnt_video_type;

        i4_ret = _inp_hdlr_operate_combi_video_device(
                    pt_svctx,
                    pt_running_svc_req,
                    _inp_hdlr_next_device_in_combi,
                    &t_info
                    );
        break;
    case SVCTX_INP_SET_TYPE_SELECT_DEVICE_IN_AUDIO_COMBI:
        if (pt_running_svc_req->pt_audio_avm == NULL
                || pt_running_svc_req->t_src_info.t_av_src.e_audio_type != DEV_AVC_COMBI) {
            i4_ret = SVCTXR_NOT_EXIST;
            break;
        }

        if (z_set_info_size != sizeof(DEVICE_TYPE_T) || pv_set_info == NULL) {
            i4_ret = SVCTXR_INV_SET_INFO;
            break;
        }

        t_info.e_query_type        = QUERY_TYPE_AUDIO;
        t_info.u.t_hint.e_type     = SRC_AVC_HINT_SPECIFIC_DEVICE_ONLY;
        t_info.u.t_hint.e_dev_type = *((DEVICE_TYPE_T*)pv_set_info);

        i4_ret = _inp_hdlr_operate_combi_audio_device(
                    pt_svctx,
                    pt_running_svc_req,
                    _inp_hdlr_specific_device_in_combi,
                    &t_info
                    );
        break;
    case SVCTX_INP_SET_TYPE_NEXT_SIGNALED_IN_AUDIO_COMBI:
    case SVCTX_INP_SET_TYPE_NEXT_DEVICE_IN_AUDIO_COMBI:
        if (pt_running_svc_req->pt_audio_avm == NULL
                || pt_running_svc_req->t_src_info.t_av_src.e_audio_type != DEV_AVC_COMBI) {
            i4_ret = SVCTXR_NOT_EXIST;
            break;
        }

        t_info.e_query_type      = QUERY_TYPE_AUDIO;
        t_info.u.b_need_signaled = e_set_type == SVCTX_INP_SET_TYPE_NEXT_SIGNALED_IN_AUDIO_COMBI ? TRUE : FALSE;
        t_info.ui2_target_index  = pt_running_svc_req->t_src_info.ui2_crnt_audio_combi_index;
        t_info.e_target_type     = pt_running_svc_req->t_src_info.e_crnt_audio_type;

        i4_ret = _inp_hdlr_operate_combi_audio_device(
                    pt_svctx,
                    pt_running_svc_req,
                    _inp_hdlr_next_device_in_combi,
                    &t_info
                    );
        break;
    case SVCTX_INP_SET_TYPE_AUDIO_SRC_INFO:
    {
        SRC_AVC_INFO_T        t_auio_src_info;
        DEVICE_TYPE_PROFILE_T t_profile;

        if (z_set_info_size != sizeof(SRC_AVC_INFO_T) || pv_set_info == NULL) {
            i4_ret = SVCTXR_INV_SET_INFO;
            break;
        }

        t_auio_src_info = *((SRC_AVC_INFO_T*)pv_set_info);

        if (t_auio_src_info.e_type == DEV_UNKNOWN) {
            pt_running_svc_req->t_src_info.t_av_src.e_audio_type = t_auio_src_info.e_type;
            pt_running_svc_req->t_src_info.t_av_src.t_audio_hint = t_auio_src_info.t_hint;

            if (pt_running_svc_req->b_play_audio_from_video_device == TRUE) {
                if (pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_type != SRC_AVC_HINT_NONE) {
                    /* if hint is not SRC_AVC_HINT_NONE, still follow the hint
                     * for the case "with audio in video pipe"
                     */
                    i4_ret = SVCTXR_OK;
                    break;
                } else {
                    /* stop current audio from video player */
                    if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) == TRUE) {
                        i4_ret = x_sm_close_stream(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream);
                        if (i4_ret == SMR_OK || i4_ret == SMR_PENDING) {
                            pt_running_svc_req->t_strm_reqs[ST_AUDIO].e_strm_cond = SM_COND_CLOSING;
                            pt_running_svc_req->t_src_info.e_crnt_audio_type      = DEV_UNKNOWN;
                        }

                        pt_running_svc_req->b_play_audio_from_video_device = FALSE;

                        i4_ret = SVCTXR_OK;
                        break;
                    }
                }
            } else {
                /* stop current audio from audio player */
                i4_ret = inp_hdlr_stop_audio_player(pt_svctx, pt_running_svc_req);
                if (i4_ret < SVCTXR_OK) {
                    break;
                }

                pt_running_svc_req->t_src_info.e_crnt_audio_type = DEV_UNKNOWN;
                
                if( pt_svctx->b_is_blocked == TRUE )
                {
                    pt_running_svc_req->b_replace_audio = FALSE;
                }
                else {
                    pt_running_svc_req->b_replace_audio = TRUE;
                }
                
                i4_ret = SVCTXR_OK;
                break;
            }
        }

        if (DEV_GET_GROUP_ID(t_auio_src_info.e_type) != DEV_AVC) {
            i4_ret = SVCTXR_INV_ARG;
            break;
        }

        i4_ret = x_svctx_get_device_profile(t_auio_src_info.e_type, &t_profile);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (t_profile.ui4_av_class_mask != ST_MASK_AUDIO) {
            i4_ret = SVCTXR_INV_ARG;
            break;
        }

        switch(t_auio_src_info.t_hint.e_type) {
        case SRC_AVC_HINT_NONE:
            if (t_auio_src_info.e_type == pt_running_svc_req->t_src_info.e_crnt_audio_type) 
            {
                /* for CR DTV00088045 */
                pt_running_svc_req->t_src_info.t_av_src.e_audio_type = t_auio_src_info.e_type;
                pt_running_svc_req->t_src_info.t_av_src.t_audio_hint = t_auio_src_info.t_hint;
                return SVCTXR_OK;
            }
            break;
        case SRC_AVC_HINT_SPECIFIC_DEVICE_FIRST:
            if (t_auio_src_info.t_hint.e_dev_type == pt_running_svc_req->t_src_info.e_crnt_audio_type) 
            {
                /* for CR DTV00082092 */
                pt_running_svc_req->t_src_info.t_av_src.e_audio_type = t_auio_src_info.e_type;
                pt_running_svc_req->t_src_info.t_av_src.t_audio_hint = t_auio_src_info.t_hint;
                return SVCTXR_OK;
            }

            /* for CR DTV00311509 & DTV00311815 */
            if ( t_auio_src_info.e_type == DEV_AVC_AUDIO_INP &&
                 pt_running_svc_req->t_src_info.t_av_src.e_audio_type == DEV_AVC_AUDIO_INP &&
                 pt_running_svc_req->b_hdmi_signal == FALSE)  
            {
                pt_running_svc_req->t_src_info.t_av_src.e_audio_type = t_auio_src_info.e_type;
                pt_running_svc_req->t_src_info.t_av_src.t_audio_hint = t_auio_src_info.t_hint;
                return SVCTXR_OK;
            }

            if (t_auio_src_info.t_hint.e_dev_type != pt_running_svc_req->t_src_info.e_crnt_video_type) {
                return SVCTXR_INV_ARG;
            }
            break;
        default:
            break;
        }

        pt_running_svc_req->t_src_info.t_av_src.e_audio_type = t_auio_src_info.e_type;
        pt_running_svc_req->t_src_info.t_av_src.t_audio_hint = t_auio_src_info.t_hint;
/* still allow to perfrom the following action even no audio in stream mode
        if ((pt_running_svc_req->t_parent.ui4_stream_mode & ST_MASK_AUDIO) == 0) {
            return SVCTXR_OK;
        }
*/
        if( pt_svctx->b_is_blocked == TRUE )
        {
            pt_running_svc_req->b_replace_audio = FALSE;
        }
        else {
            pt_running_svc_req->b_replace_audio = TRUE;
        }

        if (pt_running_svc_req->b_play_audio_from_video_device == TRUE) {
            /* stop current audio from video player */
            if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) == TRUE) {
                i4_ret = x_sm_close_stream(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream);
                if (i4_ret == SMR_OK || i4_ret == SMR_PENDING) {
                    pt_running_svc_req->t_strm_reqs[ST_AUDIO].e_strm_cond = SM_COND_CLOSING;
                }

                pt_running_svc_req->b_play_audio_from_video_device = FALSE;
                i4_ret = SVCTXR_OK;
                break;
            }
        } else {
            /* stop current audio from audio player */
            i4_ret = inp_hdlr_stop_audio_player(pt_svctx, pt_running_svc_req);
            if (i4_ret < SVCTXR_OK) {
                break;
            }
        }

        if (i4_ret == SVCTXR_OK) {
            if ((pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_type != SRC_AVC_HINT_NONE) &&
                inp_hdlr_able_to_play_audio_from_video_player(pt_svctx, pt_running_svc_req)) {
                i4_ret = inp_hdlr_check_to_play_audio_from_video_player(
                                pt_svctx,
                                pt_running_svc_req
                                );
            } else {
                i4_ret = inp_hdlr_play_audio_player(
                                pt_svctx,
                                pt_running_svc_req
                                );
            }
        }
        break;
    }
    case SVCTX_COMMON_SET_ALL_SCART_OUT_DISABLE:
    case SVCTX_COMMON_SET_SCART_OUT:
        svctx_set_scart_out_params(
                    pt_svctx,
                    player_get_sess_handle(pt_running_svc_req->pt_video_player),
                    FALSE,
                    ST_MASK_AUDIO | ST_MASK_VIDEO,
                    &(pt_running_svc_req->t_parent)
                    );
        break;
    default:
        i4_ret = SVCTXR_NOT_SUPPORT;
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_strm_nfy_fct
 * Description
 *      The API is used to receive the notification of a stream, and
 *      transfer to an internal event, and send to the attached application's
 *      event dispatcher for further processing.
 * Input arguments
 *      h_stream      the handle of a stream that causes the notification
 *      e_nfy_cond    the state of a stream
 *      pv_nfy_tag    an APP_EVN_SRC_T object
 *      e_event       the specific event to indicate more detail information of
 *                    the notification
 *      ui4_data      additional data
 * Output arguments
 *      None
 * Returns
 *      SMR_OK        the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_strm_nfy_fct(
                    HANDLE_T                    h_stream,
                    SM_COND_T                   e_nfy_cond,
                    VOID*                       pv_nfy_tag,
                    SM_EVN_TYPE_T               e_event,
                    UINT32                      ui4_data
                    )
{
    HANDLE_T    h_svctx = (HANDLE_T)pv_nfy_tag;
    SVCTX_MSG_T t_msg   = {0};

    t_msg.ui1_msg_id       = _SVCTX_MSG_STRM_NTFY;
    t_msg.u.t_strm.h_obj   = h_stream;
    t_msg.u.t_strm.e_cond  = e_nfy_cond;
    t_msg.u.t_strm.e_event = e_event;

    svctx_get_strm_msg_info(e_event, ui4_data, &t_msg.u.t_strm);
    svctx_send_msg(h_svctx, &t_msg);

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _inp_hdlr_get_conn_attr
 *
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_get_conn_attr(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    CM_CTRL_TYPE_T              e_ctrl,
                    VOID*                       pv_ctrl_data
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    PLAYER_T*      pt_player          = _inp_hdlr_get_player(pt_running_svc_req, e_pipe_type);
    HANDLE_T       h_conn;
    INT32          i4_ret;

    if (pt_player == NULL) {
        return SVCTXR_NOT_SUPPORT;
    }

    h_conn = player_get_conn_handle(pt_player);
    if (x_handle_valid(h_conn) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    i4_ret = x_cm_get(
                    h_conn,
                    e_ctrl,
                    pv_ctrl_data
                    );

    return svctx_translate_cmr_2_svctxr(i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name: _inp_hdlr_set_conn_attr
 *
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_set_conn_attr(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    CM_CTRL_TYPE_T              e_ctrl,
                    VOID*                       pv_ctrl_data
                    )
{
    INP_SVC_REQ_T*          pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    PLAYER_T*               pt_player          = _inp_hdlr_get_player(pt_running_svc_req, e_pipe_type);
    HANDLE_T                h_conn;
    INT32                   i4_ret;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    ACTIVE_LINK_ENTRY_T*    pt_link;
    SVCTX_T*                pt_obj;
    INT32                   i = 0;
#endif

    if (pt_player == NULL) {
        return SVCTXR_NOT_SUPPORT;
    }

    h_conn = player_get_conn_handle(pt_player);
    if (x_handle_valid(h_conn) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    i4_ret = x_cm_set(
                    h_conn,
                    e_ctrl,
                    pv_ctrl_data
                    );
    if (i4_ret >= CMR_OK) {
        switch(e_ctrl) {
        case AVC_HANDLER_CTRL_SET_SCART_INP_MODE:
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
                pt_running_svc_req = (INP_SVC_REQ_T*)pt_obj->pt_running_svc_req;
#endif
                pt_running_svc_req->t_src_info.t_av_src.e_scart_mode = *((AVC_HANDLER_SCART_MODE_T*)pv_ctrl_data);
                inp_hdlr_update_scart_crnt_video_type(pt_running_svc_req);
#ifdef MW_TV_AV_BYPASS_SUPPORT
            }
#endif
            break;
        default:
            break;
        }
    }

    return svctx_translate_cmr_2_svctxr(i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_open_scdb
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_open_scdb (
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    HANDLE_T*                   ph_scdb,
                    VOID*                       pv_tag,
                    x_scdb_nfy_fct              pf_nfy
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    PLAYER_T*      pt_player          = _inp_hdlr_get_player(pt_running_svc_req, e_pipe_type);
    HANDLE_T       h_conn;
    INT32          i4_ret;
    CM_COND_T      e_cm_cond;

    if (pt_player == NULL) {
        return SVCTXR_NOT_SUPPORT;
    }

    h_conn = player_get_conn_handle(pt_player);
    if (x_handle_valid(h_conn) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    e_cm_cond = player_get_conn_cond(pt_player);
    if (e_cm_cond != CM_COND_CONNECTED) {
        return SVCTXR_WRONG_STATE;
    }

    i4_ret = x_scdb_open(h_conn, ph_scdb, pv_tag, pf_nfy);

    return svctx_translate_scdbr_2_svctxr(i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_open_stream
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_open_stream (
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    const STREAM_COMP_ID_T*     pt_strm_comp_id,
                    const SM_COMMAND_T*         pt_cmd_sets,
                    x_stream_nfy_fct            pf_strm_nfy,
                    VOID*                       pv_strm_nfy_tag,
                    BOOL                        b_auto_play,
                    HANDLE_T*                   ph_stream
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    PLAYER_T*      pt_player          = _inp_hdlr_get_player(pt_running_svc_req, e_pipe_type);
    HANDLE_T       h_sess;
    INT32          i4_ret;
    SM_SESS_COND_T e_sess_cond;

    if (pt_player == NULL) {
        return SVCTXR_NOT_SUPPORT;
    }

    h_sess = player_get_sess_handle(pt_player);
    if (x_handle_valid(h_sess) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    e_sess_cond = player_get_sess_cond(pt_player);
    if (svctx_is_able_to_select_stream(e_sess_cond) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    CLI_FUNC_CALL_TRACE_FOR_STRM("x_sm_open_stream", pt_strm_comp_id->e_type);
    i4_ret = x_sm_open_stream(
                    h_sess,
                    pt_strm_comp_id,
                    pt_cmd_sets,
                    pf_strm_nfy,
                    pv_strm_nfy_tag,
                    b_auto_play,
                    ph_stream
                    );
    return svctx_translate_smr_2_svctxr(i4_ret);
}
/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_init
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 inp_hdlr_init(
                    VOID
                    )
{
    SVCTX_HDLR_FCT_TABLE_T t_fct_table = {0};
    INT32                  i4_ret;

    if (b_g_init == TRUE) {
         return SVCTXR_OK;
    }

    /* service relative function table*/
    t_fct_table.pf_allocate_svc_req           = _inp_hdlr_allocate_svc_req;
    t_fct_table.pf_free_svc_req               = _inp_hdlr_free_svc_req;
    t_fct_table.pf_create_svc_req             = _inp_hdlr_create_svc_req;
    t_fct_table.pf_create_surf_svc_req        = _inp_hdlr_create_surf_svc_req;

    /* stream relative function table*/
    t_fct_table.pf_allocate_strm_req          = _inp_hdlr_allocate_strm_req;
    t_fct_table.pf_free_strm_req              = _inp_hdlr_free_strm_req;
    t_fct_table.pf_create_strm_req            = _inp_hdlr_create_strm_req;

    t_fct_table.pf_get_scdb_num_recs          = _inp_hdlr_get_scdb_num_recs;
    t_fct_table.pf_get_cp_info                = _inp_hdlr_get_cp_info;
    t_fct_table.pf_get_ca_info                = _inp_hdlr_get_ca_info;
    t_fct_table.pf_get_using_scdb_info        = _inp_hdlr_get_using_scdb_info;
    t_fct_table.pf_get_strm_req_type          = NULL;
    t_fct_table.pf_get_strm_comp_id           = _inp_hdlr_get_strm_comp_id;

    t_fct_table.pf_replace_svc_req            = _inp_hdlr_replace_svc_req;
    t_fct_table.pf_select_svc_req             = inp_hdlr_select_svc;
    t_fct_table.pf_stop_svc_req               = inp_hdlr_stop_svc;
    t_fct_table.pf_unblock_svc_req            = inp_hdlr_check_svc_block;
    t_fct_table.pf_select_strm_req            = inp_hdlr_select_strm;
    t_fct_table.pf_stop_strm_req              = inp_hdlr_stop_strm;
    t_fct_table.pf_internal_msg               = inp_hdlr_internal_msg;
    t_fct_table.pf_is_blocked_source          = inp_hdlr_is_blocked_source;

    t_fct_table.pf_set_session_attr           = _inp_hdlr_set_sess_attr;
    t_fct_table.pf_get_session_attr           = _inp_hdlr_get_sess_attr;
    t_fct_table.pf_set_stream_attr            = _inp_hdlr_set_strm_attr;
    t_fct_table.pf_get_stream_attr            = _inp_hdlr_get_strm_attr;
    t_fct_table.pf_get_signal_level           = _inp_hdlr_get_signal_level;
    t_fct_table.pf_get_dbm_signal_level       = _inp_hdlr_get_dbm_signal_level;
    t_fct_table.pf_get_tuner_status           = NULL;
    t_fct_table.pf_get_cell_id                = NULL;
    t_fct_table.pf_get_vbi_broker             = _inp_hdlr_get_vbi_broker;
    t_fct_table.pf_get_stream_handle          = _inp_hdlr_get_strm_handle;
    t_fct_table.pf_get_current_event_info_len = _inp_hdlr_get_current_event_info_len;
    t_fct_table.pf_get_current_event_info     = _inp_hdlr_get_current_event_info;

    t_fct_table.pf_freeze                     = inp_hdlr_freeze;
    t_fct_table.pf_unfreeze                   = inp_hdlr_unfreeze;

    t_fct_table.pf_open_scdb                  = _inp_hdlr_open_scdb;
    t_fct_table.pf_open_stream                = _inp_hdlr_open_stream;
    t_fct_table.pf_get_conn_attr              = _inp_hdlr_get_conn_attr;
    t_fct_table.pf_set_conn_attr              = _inp_hdlr_set_conn_attr;

    t_fct_table.pf_get                        = _inp_hdlr_get;
    t_fct_table.pf_set                        = _inp_hdlr_set;
    t_fct_table.pf_is_handler                 = _inp_hdlr_is_handler;
    t_fct_table.pf_deinit                     = _inp_hdlr_deinit;

    i4_ret = svctx_register_handler(
                    INP_HDLR_DEFAULT_NAME,
                    &t_fct_table
                    );
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    i4_ret = inp_svc_req_init(
                    (UINT16) (svctx_get_obj_amount() * SVC_REQ_AMOUNT_FACTOR)
                    );
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    i4_ret = inp_strm_req_init(
                    (UINT16) (svctx_get_obj_amount() * SVC_REQ_AMOUNT_FACTOR * INP_STRM_TYPE_COUNT)
                    );
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    i4_ret = avm_init(
                    (UINT16) (svctx_get_obj_amount() * 2) /* one for video, one for audio */
                    );
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

#ifdef CLI_LVL_ALL
    inp_hdlr_cli_attach_cmd_tbl();
#endif

    b_g_init = TRUE;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_invoke_client_svc_nfy_fct
 * Description
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
VOID inp_hdlr_invoke_client_svc_nfy_fct(
                    SVCTX_T*                    pt_svctx,
                    SVCTX_NTFY_CODE_T           e_code,
                    STREAM_TYPE_T               e_stream_type
                    )
{

    INP_SVC_REQ_T*  pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    switch (e_code) {
    case SVCTX_NTFY_CODE_SIGNAL_LOCKED_AS_BLOCKED:
    case SVCTX_NTFY_CODE_SIGNAL_LOCKED:
        pt_svctx->b_signal_loss_notified = FALSE;
        pt_svctx->e_signal_status        = SIGNAL_STATUS_LOCKED;

        if (pt_running_svc_req->pt_video_player == NULL
                || pt_running_svc_req->pt_video_player->e_conn_cond != CM_COND_CONNECTED) {
            return;
        }
        break;
    case SVCTX_NTFY_CODE_SIGNAL_DETECTING:
        pt_svctx->b_signal_loss_notified = FALSE;
        pt_svctx->e_signal_status        = SIGNAL_STATUS_DETECTING;
        break;

    case SVCTX_NTFY_CODE_SIGNAL_LOSS:
        pt_svctx->e_signal_status = SIGNAL_STATUS_LOSS;

        if (pt_svctx->e_state != SVCTX_COND_PRESENTING_SELECT_SAME_REQ
                && pt_svctx->e_state != SVCTX_COND_PENDING_SELECT_SAME_REQ) {
            pt_svctx->e_state = SVCTX_COND_PENDING;
        }

        if (svctx_delay_signal_loss_nfy(
                        pt_svctx,
                        inp_hdlr_invoke_client_svc_nfy_fct) == TRUE) {
            svctx_set_video_plane(pt_svctx, SCC_VID_BLANK);
            return;
        }

        if (inp_hdlr_check_signal_loss_priority(pt_svctx, pt_running_svc_req, FALSE, FALSE) == FALSE) {
            return;
        }

        svctx_ctrl_block(pt_svctx, FALSE);
        pt_svctx->b_block_notified = FALSE;

        if (pt_svctx->b_signal_loss_notified == TRUE) {
            return;
        }

        /* for CR DTV00084518, NOT notify signal loss when stopping */
        if (pt_svctx->b_in_stopping == TRUE) {
            return;
        }

        if ((pt_svctx->ui1_stop_audio_as_no_signal & MAKE_BIT_MASK_8(pt_running_svc_req->t_parent.e_src_type)) == 0) {
            if (pt_running_svc_req->t_src_info.t_av_src.e_video_type == DEV_AVC_COMBI) {
                if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) == FALSE) {
                    inp_hdlr_play_audio_player(pt_svctx, pt_running_svc_req);
                }
            }
        }
#if 0
        if (pt_svctx->e_scrn_status == SVCTX_SCREEN_STATUS_SINGLE
                || pt_svctx->e_scrn_status == SVCTX_SCREEN_STATUS_MULTIPLE_FULL) {
            /* only enable video mute in full screen */
            svctx_mute_video(pt_svctx, pt_svctx->t_video_mute.b_enable);
        }
#else
        svctx_mute_video(pt_svctx, pt_svctx->t_video_mute.b_enable);
#endif
        svctx_set_video_plane(pt_svctx, SCC_VID_BLANK);

        pt_svctx->b_signal_loss_notified = TRUE;
        break;
    case SVCTX_NTFY_CODE_NORMAL:
        if (pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED) {
            return;
        }
        if (pt_svctx->b_is_blocked == TRUE) {
            return;
        }

        if (pt_svctx->e_state != SVCTX_COND_PRESENTING_SELECT_SAME_REQ
                && pt_svctx->e_state != SVCTX_COND_PENDING_SELECT_SAME_REQ) {
            pt_svctx->e_state = SVCTX_COND_PRESENTING;
        }

        svctx_set_video_plane(pt_svctx, pt_svctx->e_default_vid_mode);
        svctx_mute_video(pt_svctx, FALSE);
        svctx_set_scart_out_params(
                    pt_svctx,
                    player_get_sess_handle(pt_running_svc_req->pt_video_player),
                    FALSE,
                    ST_MASK_AUDIO | ST_MASK_VIDEO,
                    &(pt_running_svc_req->t_parent)
                    );
        break;
    case SVCTX_NTFY_CODE_SERVICE_UNBLOCKED:
        svctx_ctrl_block(pt_svctx, FALSE);
        pt_svctx->b_block_notified = FALSE;
        break;
    case SVCTX_NTFY_CODE_SERVICE_BLOCKED:
        svctx_ctrl_block(pt_svctx, TRUE);
        svctx_mute_video(pt_svctx, pt_svctx->t_video_mute.b_enable);
        svctx_set_video_plane(pt_svctx, SCC_VID_BLANK);
        svctx_force_disable_scart_out(
                    pt_svctx,
                    player_get_sess_handle(pt_running_svc_req->pt_video_player),
                    FALSE,
                    ST_MASK_AUDIO | ST_MASK_VIDEO,
                    &(pt_running_svc_req->t_parent)
                    );

        if (pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED
                && inp_hdlr_check_signal_loss_priority(pt_svctx, pt_running_svc_req, FALSE, FALSE) == TRUE) {
            return;
        }

        if (pt_svctx->b_block_notified == TRUE) {
            return;
        }

        pt_svctx->b_block_notified = TRUE;
        break;
    case SVCTX_NTFY_CODE_CRNT_EVN_START_QUERY:
    case SVCTX_NTFY_CODE_CRNT_EVN_NOT_READY:
    case SVCTX_NTFY_CODE_CRNT_EVN_READY:
        break;
    case SVCTX_NTFY_CODE_SIGNAL_LOSS_AS_BLOCKED:
        break;
    default:
        if (pt_svctx->b_is_blocked == TRUE) {
            return;
        }
        break;
    }

    svctx_notify_clients_svc_msg(
                    pt_svctx,
                    e_code,
                    e_stream_type,
                    pt_svctx->pt_running_svc_req->h_svctx,
                    pt_svctx->pt_running_svc_req->pf_svc_nfy
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_check_strms_status
 * Description
 * Input arguments
 *      pt_svctx    a pointer points to the Service Context
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
VOID inp_hdlr_check_strms_status(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    SVCTX_NTFY_CODE_T e_code;
    UINT32            ui4_desired_strm_mode = 0;
    UINT32            ui4_current_strm_mode = 0;

    if (pt_running_svc_req->t_src_info.t_av_src.e_video_type != DEV_UNKNOWN) {
        /* if the source with video type source, it needs to guarantee
         * the video has signal (LOCKED), then do the notification
         */
        if (pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED) {
            return;
        }
    }

    if (pt_svctx->b_is_blocked == TRUE
            || pt_svctx->t_video_info.e_timing_type == VSH_SRC_TIMING_NOT_SUPPORT) {
        return;
    }

    ui4_desired_strm_mode = pt_running_svc_req->t_parent.ui4_stream_mode & (ST_MASK_VIDEO | ST_MASK_AUDIO);

    if (pt_running_svc_req->t_src_info.t_av_src.e_video_type == DEV_UNKNOWN) {
        ui4_desired_strm_mode &= ~ST_MASK_VIDEO;
    }

    if (pt_running_svc_req->t_src_info.t_av_src.e_audio_type == DEV_UNKNOWN) {
        /* no audio pipe configuration */
        if (pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_dev_type == DEV_UNKNOWN) {
            /* no audio pipe hint type configuration */
            ui4_desired_strm_mode &= ~ST_MASK_AUDIO;
        } else if (pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_dev_type == DEV_AVC_HDMI) {
            /* audio pipe hint type = HDMI (preferred) */
            if (inp_hdlr_with_audio_from_video_player(pt_svctx, pt_running_svc_req) == FALSE) {
                /* no audio - DVI */
                ui4_desired_strm_mode &= ~ST_MASK_AUDIO;
            }
        }
    }

    if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream) == TRUE) {
        if (svctx_is_valid_video_fmt(pt_svctx) == TRUE) {
            ui4_current_strm_mode |= ST_MASK_VIDEO;
        }
    }

    if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) == TRUE) {
        if (svctx_is_valid_audio_fmt(pt_svctx) == TRUE) {
            ui4_current_strm_mode |= ST_MASK_AUDIO;
        }
    }

    if (ui4_desired_strm_mode != ui4_current_strm_mode) {
        return;
    }

    if (ui4_current_strm_mode & ST_MASK_VIDEO) {
        if (ui4_current_strm_mode & ST_MASK_AUDIO) {
            e_code = SVCTX_NTFY_CODE_AUDIO_VIDEO_SVC;
            if (pt_svctx->e_av_status == SVC_AV_STATUS_AUDIO_VIDEO) {
                return;
            }
        } else {
            e_code = SVCTX_NTFY_CODE_VIDEO_ONLY_SVC;
            if (pt_svctx->e_av_status == SVC_AV_STATUS_VIDEO_ONLY) {
                return;
            }
        }
    } else {
        if (ui4_current_strm_mode & ST_MASK_AUDIO) {
            if (pt_running_svc_req->t_src_info.t_av_src.e_video_type != DEV_UNKNOWN
                    && pt_svctx->e_signal_status == SIGNAL_STATUS_LOCKED) {
                e_code = SVCTX_NTFY_CODE_AUDIO_VIDEO_SVC;
                if (pt_svctx->e_av_status == SVC_AV_STATUS_AUDIO_VIDEO) {
                    return;
                }
            } else {
                e_code = SVCTX_NTFY_CODE_AUDIO_ONLY_SVC;
                if (pt_svctx->e_av_status == SVC_AV_STATUS_AUDIO_ONLY) {
                    return;
                }
            }
        } else {
            if (pt_running_svc_req->t_src_info.t_av_src.e_video_type != DEV_UNKNOWN
                    && pt_svctx->e_signal_status == SIGNAL_STATUS_LOCKED) {
                e_code = SVCTX_NTFY_CODE_VIDEO_ONLY_SVC;
                if (pt_svctx->e_av_status == SVC_AV_STATUS_VIDEO_ONLY) {
                    return;
                }
            } else {
                e_code = SVCTX_NTFY_CODE_NO_AUDIO_VIDEO_SVC;
                if (pt_svctx->e_av_status == SVC_AV_STATUS_NO_AUDIO_VIDEO) {
                    return;
                }
            }
        }
    }

    inp_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    e_code,
                    ST_UNKNOWN
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_iterate_strm_comp_id
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 inp_hdlr_iterate_strm_comp_id(
                    HANDLE_T                     h_scdb,
                    STREAM_TYPE_T                e_strm_type,
                    BOOL                         b_auto_version_update,
                    x_svctx_strm_comp_filter_fct pf_filter,
                    VOID*                        pv_tag,
                    STREAM_COMP_ID_T*            pt_comp_id,
                    SCDB_REC_T*                  pt_scdb_rec
                    )
{
    UINT32           ui4_db_version     = SCDB_NULL_VER_ID;
    UINT16           ui2_num_recs       = 0;
    UINT16           ui2_idx            = 0;
    BOOL             b_is_searching     = TRUE;
    STREAM_COMP_ID_T t_first_comp_id;
    SCDB_REC_T       t_first_scdb_rec;
    SCDB_REC_T       t_scdb_rec;
    INT32            i4_ret;

    while (b_is_searching) {
        b_is_searching = FALSE;
        i4_ret = x_scdb_get_num_recs(
                    h_scdb,
                    e_strm_type,
                    &ui2_num_recs,
                    &ui4_db_version
                    );

        if (i4_ret != SCDBR_OK) {
            return SVCTXR_FAIL;
        }

        for (ui2_idx = 0; ui2_idx < ui2_num_recs; ui2_idx++) {
            i4_ret = x_scdb_get_rec_by_idx(
                            h_scdb,
                            e_strm_type,
                            ui2_idx,
                            pt_comp_id,
                            &t_scdb_rec,
                            &ui4_db_version
                            );
            if (i4_ret == SCDBR_OK) {
                if (pf_filter(
                            ui2_num_recs,
                            ui2_idx,
                            pt_comp_id,
                            &t_scdb_rec,
                            pv_tag) == TRUE) {
                    if (pt_scdb_rec != NULL) {
                        *pt_scdb_rec = t_scdb_rec;
                    }
                    return SVCTXR_OK;
                } else {
                    if (ui2_idx == 0) {
                        t_first_comp_id  = *pt_comp_id;
                        t_first_scdb_rec = t_scdb_rec;
                    }
                    if (ui2_idx == (UINT16) (ui2_num_recs-1)) {
                        if (pf_filter(
                                ui2_num_recs,
                                ui2_num_recs,
                                &t_first_comp_id,
                                &t_first_scdb_rec,
                                pv_tag) == TRUE) {
                            if (pt_scdb_rec != NULL) {
                                *pt_comp_id  = t_first_comp_id;
                                *pt_scdb_rec = t_first_scdb_rec;
                            }
                            return SVCTXR_OK;
                        }
                    }
                }
            } else if (i4_ret == SCDBR_REC_NOT_FOUND) {
                /* done all processing, no more record. */
                return SVCTXR_NOT_EXIST;
            } else if (i4_ret == SCDBR_DB_MODIFIED) {
                /* continue the search again */
                if (b_auto_version_update == TRUE) {
                    b_is_searching = TRUE;
                } else {
                    return SVCTXR_DB_MODIFIED;
                }
                break;
            } else {
                return SVCTXR_FAIL;
            }
        }
    }
    return SVCTXR_NOT_EXIST;
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_open_strm
 * Description
 *      The API is used to open a stream after the proper stream comp id is
 *      selected.
 * Input arguments
 *      pt_svctx        the pointer points to an SVCTX_T object.
 *      pt_strm_req     the pointer points to an STRM_REQ_T object.
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK       the routine is successfully.
 *      SVCTXR_FAIL     the routine is failed.
 *-----------------------------------------------------------------------------*/
#ifdef MW_TV_AV_BYPASS_SUPPORT
INT32 inp_hdlr_open_strm(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    PLAYER_T*                   pt_player,
                    STRM_REQ_T*                 pt_strm_req,
                    BOOL                        b_auto_play
                    )
{
    return (inp_hdlr_try_opening_stream(
        pt_svctx,
        pt_running_svc_req,
        pt_player,
        pt_strm_req,
        b_auto_play));
}

INT32 inp_hdlr_open_strm_ex(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    PLAYER_T*                   pt_player,
                    STRM_REQ_T*                 pt_strm_req,
                    BOOL                        b_auto_play
                    )
{
    STREAM_COMP_ID_T    t_strm_comp_id;
    SM_COMMAND_T        t_sm_cmd[4];
    INT32               i4_ret = SVCTXR_OK;

    t_strm_comp_id = pt_strm_req->u.t_scdb.t_comp_id;

    switch(t_strm_comp_id.e_type) {
    case ST_VIDEO:
        if (IS_SVCTX_ASSOCIATE(pt_svctx))
        {
            t_strm_comp_id.e_type = ST_BYPASS;
            t_sm_cmd[0].e_code = SM_CMD_CODE_END;
        }
        else
        {
            t_sm_cmd[0].e_code    = SM_VS_CMD_CODE_SET_VIDEO_REGION;
            t_sm_cmd[0].u.pv_data = &(pt_svctx->t_video_region);
            t_sm_cmd[1].e_code    = SM_VS_CMD_CODE_SET_DISP_REGION;
            t_sm_cmd[1].u.pv_data = &(pt_svctx->t_disp_region);
            t_sm_cmd[2].e_code    = SM_CMD_CODE_END;
        }

        break;
    case ST_CLOSED_CAPTION:
        if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream) == FALSE) {
            return SVCTXR_FAIL;
        }

        if (x_handle_valid(pt_running_svc_req->h_gl_plane) == FALSE) {
            return SVCTXR_FAIL;
        }

        /* set commonds for open closed caption */
        t_sm_cmd[0].e_code    = SM_CC_CMD_CODE_SET_VS_HANDLE;
        t_sm_cmd[0].u.h_obj   = pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream;
        t_sm_cmd[1].e_code    = SM_CC_CMD_CODE_USE_OSD_DISP;
        t_sm_cmd[1].u.h_obj   = pt_running_svc_req->h_gl_plane;
        t_sm_cmd[2].e_code    = SM_CC_CMD_CODE_SET_CC_FILTER;
        t_sm_cmd[2].u.pv_data = (VOID*)(&pt_running_svc_req->t_cc_filter);
        t_sm_cmd[3].e_code    = SM_CMD_CODE_END;
        break;
    case ST_AUDIO:
        t_sm_cmd[0].e_code    = SM_CMD_CODE_END;
        break;
    default:
        return SVCTXR_NOT_SUPPORT;
    }

    CLI_FUNC_CALL_TRACE_FOR_STRM("x_sm_open_stream", t_strm_comp_id.e_type);
    i4_ret = x_sm_open_stream(
                player_get_sess_handle(pt_player),
                &t_strm_comp_id,
                t_sm_cmd,
                _inp_hdlr_strm_nfy_fct,
                (VOID*)(pt_svctx->h_sys_svctx),
                b_auto_play,
                &(pt_strm_req->h_stream)
                );
    if (i4_ret != SMR_OK && i4_ret != SMR_PENDING) {
        inp_hdlr_invoke_client_svc_nfy_fct(
                pt_svctx,
                SVCTX_NTFY_CODE_INTERNAL_ERROR,
                pt_strm_req->u.t_scdb.t_comp_id.e_type
                );
        return SVCTXR_FAIL;
    }

    pt_strm_req->e_strm_cond = SM_COND_OPENING;

    return SVCTXR_OK;
}
#else
INT32 inp_hdlr_open_strm(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req,
                    PLAYER_T*                   pt_player,
                    STRM_REQ_T*                 pt_strm_req,
                    BOOL                        b_auto_play
                    )
{
    SM_COMMAND_T t_sm_cmd[4];
    INT32        i4_ret = SVCTXR_OK;

    switch(pt_strm_req->u.t_scdb.t_comp_id.e_type) {
    case ST_VIDEO:
        t_sm_cmd[0].e_code    = SM_VS_CMD_CODE_SET_VIDEO_REGION;
        t_sm_cmd[0].u.pv_data = &(pt_svctx->t_video_region);
        t_sm_cmd[1].e_code    = SM_VS_CMD_CODE_SET_DISP_REGION;
        t_sm_cmd[1].u.pv_data = &(pt_svctx->t_disp_region);
        t_sm_cmd[2].e_code    = SM_CMD_CODE_END;
        break;
    case ST_CLOSED_CAPTION:
        if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream) == FALSE) {
            return SVCTXR_FAIL;
        }

        if (x_handle_valid(pt_running_svc_req->h_gl_plane) == FALSE) {
            return SVCTXR_FAIL;
        }

        /* set commonds for open closed caption */
        t_sm_cmd[0].e_code    = SM_CC_CMD_CODE_SET_VS_HANDLE;
        t_sm_cmd[0].u.h_obj   = pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream;
        t_sm_cmd[1].e_code    = SM_CC_CMD_CODE_USE_OSD_DISP;
        t_sm_cmd[1].u.h_obj   = pt_running_svc_req->h_gl_plane;
        t_sm_cmd[2].e_code    = SM_CC_CMD_CODE_SET_CC_FILTER;
        t_sm_cmd[2].u.pv_data = (VOID*)(&pt_running_svc_req->t_cc_filter);
        t_sm_cmd[3].e_code    = SM_CMD_CODE_END;
        break;
    case ST_AUDIO:
        t_sm_cmd[0].e_code    = SM_CMD_CODE_END;
        break;
    default:
        return SVCTXR_NOT_SUPPORT;
    }

    CLI_FUNC_CALL_TRACE_FOR_STRM("x_sm_open_stream", pt_strm_req->u.t_scdb.t_comp_id.e_type);
    i4_ret = x_sm_open_stream(
                player_get_sess_handle(pt_player),
                &(pt_strm_req->u.t_scdb.t_comp_id),
                t_sm_cmd,
                _inp_hdlr_strm_nfy_fct,
                (VOID*)(pt_svctx->h_sys_svctx),
                b_auto_play,
                &(pt_strm_req->h_stream)
                );
    if (i4_ret != SMR_OK && i4_ret != SMR_PENDING) {
        inp_hdlr_invoke_client_svc_nfy_fct(
                pt_svctx,
                SVCTX_NTFY_CODE_INTERNAL_ERROR,
                pt_strm_req->u.t_scdb.t_comp_id.e_type
                );
        return SVCTXR_FAIL;
    }

    pt_strm_req->e_strm_cond = SM_COND_OPENING;

    return SVCTXR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_invoke_client_strm_nfy_fct
 * Description
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
VOID inp_hdlr_invoke_client_strm_nfy_fct(
                    SVCTX_T*                    pt_svctx,
                    SVCTX_NTFY_CODE_T           e_code,
                    STRM_REQ_T*                 pt_strm_req
                    )
{
    INP_SVC_REQ_T* pt_running_svc_req = (INP_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    BOOL           b_check_svc_type   = FALSE;
    INT32          i4_ret;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (pt_strm_req == NULL
            ||(pt_strm_req->u.t_scdb.t_comp_id.e_type == ST_VIDEO && pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED)
            || pt_svctx->b_is_blocked == TRUE) {
        if (pt_svctx->b_is_blocked == TRUE) {
            if (e_code == SVCTX_NTFY_CODE_STREAM_STOPPED) {
                e_code = SVCTX_NTFY_CODE_STREAM_STOPPED_AS_BLOCKED;
            } else if (e_code == SVCTX_NTFY_CODE_STREAM_STARTED) {
                e_code = SVCTX_NTFY_CODE_STREAM_STARTED_AS_BLOCKED;
            } else if (e_code == SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE) {
                e_code = SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE_AS_BLOCKED;
            } else {
                return;
            }
        } else {
            return;
        }
    }

    switch (e_code) {
    case SVCTX_NTFY_CODE_STREAM_STOPPED_AS_BLOCKED:
    case SVCTX_NTFY_CODE_STREAM_STOPPED:
        if (pt_strm_req)
        {
            switch(pt_strm_req->u.t_scdb.t_comp_id.e_type) {
            case ST_VIDEO:
            case ST_AUDIO:
                if ((pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED) &&
                    (pt_svctx->t_audio_info.e_aud_fmt == ASH_AUDIO_FMT_UNKNOWN))
                {
                    return;
                }

                if (pt_svctx->e_state != SVCTX_COND_PRESENTING) {
                    break;
                }
                b_check_svc_type = TRUE;
                break;
            default:
                break;
            }
        }
        else
        {
            DBG_INFO((_INFO_HEADER"%s() e_code=%d pt_strm_req=NULL \n", __func__, e_code));
        }
        break;
    case SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE:
        if (pt_svctx->b_is_blocked == FALSE) {
            if (pt_svctx->t_video_info.e_timing_type == VSH_SRC_TIMING_NOT_SUPPORT) {
                svctx_mute_video(pt_svctx, pt_svctx->t_video_mute.b_enable);
                svctx_set_video_plane(pt_svctx, SCC_VID_BLANK);

                if (pt_running_svc_req->t_src_info.t_av_src.t_audio_hint.e_dev_type == DEV_AVC_HDMI) {
                    /* if audio is preferred HDMI, and video is out of range, stop the audio stream */
                    if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) == TRUE
                            && pt_running_svc_req->b_play_audio_from_video_device == TRUE) {
                        i4_ret = x_sm_close_stream(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream);
                        if (i4_ret == SMR_OK || i4_ret == SMR_PENDING) {
                            pt_running_svc_req->t_strm_reqs[ST_AUDIO].e_strm_cond = SM_COND_CLOSING;
                        }
                    } else {
                        inp_hdlr_stop_audio_player(pt_svctx, pt_running_svc_req);
                    }
                } else {
                    if ((pt_svctx->ui1_stop_audio_as_no_signal & MAKE_BIT_MASK_8(pt_running_svc_req->t_parent.e_src_type)) > 0) {
                        inp_hdlr_stop_audio_player(pt_svctx, pt_running_svc_req);
                    }
                }
            } else if (pt_svctx->t_video_info.e_timing_type != VSH_SRC_TIMING_UNKNOWN) {
                svctx_set_video_plane(pt_svctx, pt_svctx->e_default_vid_mode);
                svctx_mute_video(pt_svctx, FALSE);

                if ((x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) == FALSE) &&
                    (pt_svctx->b_in_stopping != TRUE)){
                    inp_hdlr_play_audio_stream(pt_svctx, pt_running_svc_req);
                    }
#if 0 /* for CR DTV00148203 */
                if (pt_running_svc_req->t_src_info.t_av_src.e_video_type == DEV_AVC_COMBI
                        && pt_running_svc_req->t_src_info.e_crnt_video_type == DEV_AVC_COMP_VIDEO) {
                    /* video stream reports video format update but the video
                     * plane is not enabled yet. Wait 150 ms for video to show.
                     * = tricky solution - weider chang 1/13/2008
                     */
                    x_thread_delay(150);
                }
#endif                
            }
        }
        /* on purpose to without break */
    case SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE:
        b_check_svc_type = TRUE;
        break;
    case SVCTX_NTFY_CODE_NO_STREAM:
    case SVCTX_NTFY_CODE_CA:
        b_check_svc_type = TRUE;
        break;
    default:
        break;
    }

    svctx_notify_clients_strm_msg(
                    pt_svctx,
                    e_code,
                    pt_strm_req
                    );
    if (b_check_svc_type == TRUE) {
        inp_hdlr_check_strms_status(pt_svctx, pt_running_svc_req);
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_resend_crnt_evn_nfy
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
VOID inp_hdlr_resend_crnt_evn_nfy(
                    SVCTX_T*                    pt_svctx,
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    if (pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED) {
        return;
    }
    
#ifndef MW_EDB_SUPPORT 
    switch (crnt_evn_get_cond(pt_running_svc_req->pt_crnt_event)) {
    case EVCTX_COND_ACTIVE:
    case EVCTX_COND_AVAIL:
    case EVCTX_COND_UPDATED:
        inp_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CRNT_EVN_READY,
                    ST_UNKNOWN
                    );
        break;
    case EVCTX_COND_UNAVAIL:
        inp_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CRNT_EVN_NOT_READY,
                    ST_UNKNOWN
                    );
        break;
    case EVCTX_COND_TRANSITION:
        inp_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CRNT_EVN_START_QUERY,
                    ST_UNKNOWN
                    );
        break;
    default:
        break;
    }
#else
    switch (crnt_evn_get_cond(pt_running_svc_req->pt_crnt_event)) {
    case EDB_COND_UNCONFIG:
        inp_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CRNT_EVN_NOT_READY,
                    ST_UNKNOWN
                    );
        break;
    case EDB_COND_STOPPED:
        inp_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CRNT_EVN_NOT_READY,
                    ST_UNKNOWN
                    );
        break;
    case EDB_COND_STARTED:
        inp_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CRNT_EVN_READY,
                    ST_UNKNOWN
                    );
        break;
    default:
        break;
    }
#endif
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_hdlr_update_scart_crnt_video_type
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 inp_hdlr_update_scart_crnt_video_type(
                    INP_SVC_REQ_T*              pt_running_svc_req
                    )
{
    if (pt_running_svc_req == NULL
            || pt_running_svc_req->t_src_info.t_av_src.e_video_type != DEV_AVC_SCART) {
        return SVCTXR_INV_ARG;
    }

    switch(pt_running_svc_req->t_src_info.t_av_src.e_scart_mode) {
    case AVC_HANDLER_SCART_MODE_S_VIDEO:
        pt_running_svc_req->t_src_info.e_crnt_video_type = DEV_AVC_S_VIDEO;
        break;
    case AVC_HANDLER_SCART_MODE_COMPOSITE:
        pt_running_svc_req->t_src_info.e_crnt_video_type = DEV_AVC_COMP_VIDEO;
        break;
    case AVC_HANDLER_SCART_MODE_RGB:
        pt_running_svc_req->t_src_info.e_crnt_video_type = DEV_AVC_VGA;
        break;
    case AVC_HANDLER_SCART_MODE_MIX:
        pt_running_svc_req->t_src_info.e_crnt_video_type = DEV_AVC_COMP_VIDEO; /* | DEV_AVC_VGA */
        break;
    case AVC_HANDLER_SCART_MODE_AUTO:
        pt_running_svc_req->t_src_info.e_crnt_video_type = DEV_UNKNOWN;
        break;
    default:
        pt_running_svc_req->t_src_info.e_crnt_video_type = DEV_UNKNOWN;
        break;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * export methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      x_inp_hdlr_reg_handler
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 x_inp_hdlr_reg_handler(
                    VOID
                    )
{
    return inp_hdlr_init();
}
