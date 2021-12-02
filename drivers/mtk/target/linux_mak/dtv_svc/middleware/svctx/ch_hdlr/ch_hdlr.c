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
 * $RCSfile: ch_hdlr.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/46 $
 * $SWAuthor: Weider Chang $
 * $MD5HEX: f0b308e28852e901dc8386943dc577db $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "svl/x_svl.h"
#include "tsl/x_tsl.h"
#include "svl_bldr/u_sb.h"
#include "res_mngr/drv/x_tuner.h"
#include "svctx/svctx.h"
#include "svctx/svctx_dbg.h"
#include "conn_mngr/analog_brdcst/u_analog_brdcst.h"
#include "conn_mngr/fm_radio/u_fm_radio.h"
#include "time_msrt/x_time_msrt.h"
#include "svctx/ch_hdlr/ch_hdlr.h"
#include "svctx/svctx_cli.h"
#include "svctx/util/player.h"
#include "svctx/svctx_sm_broker.h"
#include "dbg/x_dbg.h"

#ifdef MW_TV_AV_BYPASS_SUPPORT
#include "svctx/svctx_mgr.h"
#include "strm_mngr/bypass_hdlr/u_sm_bypass_event.h"
#endif

#include "svctx/svctx_rikstv.h"

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#undef  _INFO_HEADER
#undef  _API_HEADER
#undef  _ERROR_HEADER
#define _INFO_HEADER        "{SVCTX:CH_HDLR}Info: "
#define _API_HEADER         "{SVCTX:CH_HDLR}API:  "
#define _ERROR_HEADER       "{SVCTX:CH_HDLR}Err:  "

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

#ifndef ATSC_PAT_QUERY_TIMEOUT
    #define ATSC_PAT_QUERY_TIMEOUT      (2000)
#endif

#ifndef ATSC_PMT_QUERY_TIMEOUT
    #define ATSC_PMT_QUERY_TIMEOUT      (2000)
#endif

#ifndef ATSC_MGT_QUERY_TIMEOUT
    #define ATSC_MGT_QUERY_TIMEOUT      (1000)
#endif

#ifndef DVB_PAT_QUERY_TIMEOUT
    #define DVB_PAT_QUERY_TIMEOUT       (3000)
#endif

#ifndef DVB_PMT_QUERY_TIMEOUT
    #define DVB_PMT_QUERY_TIMEOUT       (3000)
#endif

/* for EU analog TV */
#if 0
#define TV_SYS_MASK_EU      (TV_SYS_MASK_A         \
                            |TV_SYS_MASK_B         \
                            |TV_SYS_MASK_C         \
                            |TV_SYS_MASK_D         \
                            |TV_SYS_MASK_E         \
                            |TV_SYS_MASK_F         \
                            |TV_SYS_MASK_G         \
                            |TV_SYS_MASK_H         \
                            |TV_SYS_MASK_I         \
                            |TV_SYS_MASK_J         \
                            |TV_SYS_MASK_K         \
                            |TV_SYS_MASK_K_PRIME   \
                            |TV_SYS_MASK_L         \
                            |TV_SYS_MASK_L_PRIME )
#endif

#define GET_ACTIVE_STRM_REQ(psr, id, st)        ((STRM_REQ_AD_INDEX == (INT32) (id)) ? &((psr)->t_strm_reqs[STRM_REQ_AD_INDEX]) : &((psr)->t_strm_reqs[st]))
#define GET_PENDING_STRM_REQ_PPTR(psr, ad, st)  ((ad) ? &((psr)->pt_pending_strm_reqs[STRM_REQ_AD_INDEX]) : &((psr)->pt_pending_strm_reqs[st]))
#define GET_PENDING_STRM_REQ(psr, ad, st)       (*GET_PENDING_STRM_REQ_PPTR(psr, ad, st))

/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_strm_select_req (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req,
                    STRM_REQ_T*                 pt_new_strm_req,
                    BOOL                        b_need_to_free_req
                    );
static INT32 _ch_hdlr_block_service (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    );

static INT32 _ch_hdlr_get_svl_rec_by_req(
                    CH_SVC_REQ_T*               pt_svc_req,
                    SVL_REC_T*                  pt_svl_rec
                    );
static INT32 _ch_hdlr_monitor_scart_signal (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    );
#ifdef MW_TV_AV_BYPASS_SUPPORT
static BOOL _ch_hdlr_same_strm_req(
                    STRM_REQ_T*                 pt_strm_req_1,
                    STRM_REQ_T*                 pt_strm_req_2);
static INT32 _ch_hdlr_check_stream_conflict(
                    SVCTX_T*                    pt_svctx,
                    STRM_REQ_T*                 pt_strm_req,
                    STREAM_TYPE_T               e_strm_type,
                    CH_SVC_REQ_T*               pt_svc_req);
#endif

/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
const STREAM_TYPE_T e_g_ch_strm_types[] = {
                    ST_VIDEO,
                    ST_AUDIO,
                    ST_CLOSED_CAPTION,
                    ST_ISDB_CAPTION,
                    ST_ISDB_TEXT,
                    ST_TELETEXT,
                    ST_SUBTITLE,
                    ST_AUDIO
                    };
static UINT8 ui1_ch_suf_ind = 0;    /* 0/1: 1st ch surf, 2: other ch surf */ 

/*-----------------------------------------------------------------------------
 * SVCTX private methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_handle_audio_fmt_chk
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_audio_fmt_chk (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    STRM_REQ_T* pt_audio_strm_req = NULL;

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }
    
    pt_audio_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_AUDIO]);

    if (pt_audio_strm_req == NULL
            || x_handle_valid(pt_audio_strm_req->h_stream) == FALSE) {
        return SVCTXR_OK;
    }

    if (svctx_is_valid_audio_fmt(pt_svctx) == TRUE) {
        return SVCTXR_OK;
    }

    /* if audio format is still not available, the timeout check will send
     * notify to client
     */
    ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE,
                    pt_audio_strm_req
                    );
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_handle_video_fmt_chk
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_video_fmt_chk (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    STRM_REQ_T* pt_video_strm_req = NULL;

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_video_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]);

    if (pt_video_strm_req == NULL
            || x_handle_valid(pt_video_strm_req->h_stream) == FALSE) {
        return SVCTXR_OK;
    }

    if (svctx_is_valid_video_fmt(pt_svctx) == TRUE) {
        return SVCTXR_OK;
    }

    /* if video format is still not available, the timeout check will send
     * notify to client
     */
    ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE,
                    pt_video_strm_req
                    );
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_get_strm_req_from_handle
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_get_strm_req_from_handle(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_stream,
                    STRM_REQ_T**                ppt_strm_req,
                    STREAM_TYPE_T*              pe_strm_type
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = NULL;
    STRM_REQ_T*   pt_strm_req        = NULL;
    STREAM_TYPE_T e_strm_type;
    UINT8         ui1_idx;

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_svctx is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    if (pt_svctx->pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    for (ui1_idx = 0; ui1_idx < (UINT8) STRM_TYPE_COUNT_WITH_AD; ui1_idx++) {
        e_strm_type = e_g_ch_strm_types[ui1_idx];
        if ((e_strm_type == ST_UNKNOWN) || (e_strm_type >= STRM_TYPE_COUNT)) continue;

        pt_strm_req = GET_ACTIVE_STRM_REQ(pt_running_svc_req, ui1_idx, e_strm_type);

        if (pt_strm_req == NULL || pt_strm_req->h_stream != h_stream) {
            continue;
        }

        *ppt_strm_req = pt_strm_req;
        *pe_strm_type = e_strm_type;
        return SVCTXR_OK;
    }

    return SVCTXR_INV_HANDLE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_is_blocked_service
 * Description
 *
 * Input arguments
 *      pt_svctx        the pointer points to an SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK       the routine is successfully
 *-----------------------------------------------------------------------------*/
static BOOL _ch_hdlr_is_blocked_service(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req,
                    SVC_BLOCK_CHECK_GUDIE_T     e_check_guide,
                    BOOL                        b_auto_notify_client
                    )
{
    SVC_BLOCK_COND_T e_old_block_cond = SVC_BLOCK_COND_PASSED;
    SVC_BLOCK_COND_T e_new_block_cond = SVC_BLOCK_COND_PASSED;
#ifndef MW_EDB_SUPPORT
    EVCTX_COND_T     e_evctx_cond;
#endif

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
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
        if (e_check_guide & SVC_BLOCK_CHECK_RATING) {
            if (svctx_with_event_capability(
                    pt_svctx,
                    pt_running_svc_req->e_crnt_tuner_type,
#ifndef MW_EDB_SUPPORT
                    MAKE_BIT_MASK_32(EVCTX_KEY_TYPE_RATING)
#else
                    MAKE_BIT_MASK_32(EDB_KEY_TYPE_RATING_LIST_BY_ID)
#endif

                    ) == TRUE) {
#ifndef MW_EDB_SUPPORT                    
                    e_evctx_cond = crnt_evn_get_cond(pt_running_svc_req->pt_crnt_event);
                    if ((pt_svctx->e_block_rule == SVCTX_BLOCK_RULE_INP_SIGNAL_LOSS_CH_PROG
                       ||pt_svctx->e_block_rule == SVCTX_BLOCK_RULE_INP_CH_SIGNAL_LOSS_PROG)
                            && e_evctx_cond == EVCTX_COND_TRANSITION                     
                            ) {
                        e_check_guide &= ~SVC_BLOCK_CHECK_RATING;
                    }
#endif

            } else {
                e_check_guide &= ~SVC_BLOCK_CHECK_RATING;
            }
        }

        {
            e_new_block_cond = rating_chker_check(
                                pt_running_svc_req->pt_rating_chker,
                                pt_running_svc_req->t_src_info.s_src_grp_name,
                                pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svl_rec_id,
                                e_check_guide,
                                crnt_evn_get_rating(pt_running_svc_req->pt_crnt_event)
                                );
        }
    }

    if (b_auto_notify_client == FALSE) {
        return TRUE;
    }

    switch (e_new_block_cond) {
    case SVC_BLOCK_COND_PASSED:
    case SVC_BLOCK_COND_PASSED_AND_NO_RATING_SET:
        if (e_old_block_cond == SVC_BLOCK_COND_PASSED
            || e_old_block_cond == SVC_BLOCK_COND_PASSED_AND_NO_RATING_SET
            || pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED) {
            return FALSE;
        }
        ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_SERVICE_UNBLOCKED,
                    ST_UNKNOWN
                    );
        ch_hdlr_resend_crnt_evn_nfy(
                    pt_svctx,
                    pt_running_svc_req
                    );
        return FALSE;
    case SVC_BLOCK_COND_USER_BLOCK_CH:
    case SVC_BLOCK_COND_USER_BLOCK_INP:
    case SVC_BLOCK_COND_EXCEED_RATING:
    default:
        ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_SERVICE_BLOCKED,
                    ST_UNKNOWN
                    );
        return TRUE;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_close_text_strms
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_close_text_strms(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req,
                    UINT32                      ui4_text_strm_maks
                    )
{

    INT32 i4_ret;

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

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

#ifdef MW_ISDB_SUPPORT
    if (ui4_text_strm_maks & ST_MASK_ISDB_CAPTION) {
        STRM_REQ_T* pt_isdb_caption_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_ISDB_CAPTION]);
        /* this video stream has related ISDB caption in using*/
        if (x_handle_valid(pt_isdb_caption_strm_req->h_stream) == TRUE) {
            pt_isdb_caption_strm_req->e_penging_status = STRM_PENDING_COND_STOP_STRM;
            i4_ret = x_sm_close_stream(pt_isdb_caption_strm_req->h_stream);
            if (i4_ret == SMR_OK || i4_ret == SMR_PENDING || i4_ret == SMR_INV_HANDLE) {
                pt_isdb_caption_strm_req->e_strm_cond = SM_COND_CLOSING;
            } else {
                return SVCTXR_FAIL;
            }
        }
    }

    if (ui4_text_strm_maks & ST_MASK_ISDB_TEXT) {
        STRM_REQ_T* pt_isdb_text_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_ISDB_TEXT]);
        /* this video stream has related ISDB text in using*/
        if (x_handle_valid(pt_isdb_text_strm_req->h_stream) == TRUE) {
            pt_isdb_text_strm_req->e_penging_status = STRM_PENDING_COND_STOP_STRM;
            i4_ret = x_sm_close_stream(pt_isdb_text_strm_req->h_stream);
            if (i4_ret == SMR_OK || i4_ret == SMR_PENDING || i4_ret == SMR_INV_HANDLE) {
                pt_isdb_text_strm_req->e_strm_cond = SM_COND_CLOSING;
            } else {
                return SVCTXR_FAIL;
            }
        }
    }
#endif

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
 *      _ch_hdlr_open_cc_stream_after_video_started
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _ch_hdlr_open_cc_stream_after_video_started(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    STRM_REQ_T*      pt_strm_req = NULL;
    INT32            i4_ret      = SVCTXR_OK;
    STREAM_COMP_ID_T t_comp_id;

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_CLOSED_CAPTION]);

    /* if there's a closed stream is already opened, bypass it.
     * It could happen in a blocked channel after it is unblocked.
     */
    if (pt_strm_req == NULL
            || pt_strm_req->e_req_type != STRM_REQ_SCDB
            || pt_strm_req->u.t_scdb.t_comp_id.e_type == ST_UNKNOWN
            || x_handle_valid(pt_strm_req->h_stream) == TRUE
            || pt_running_svc_req->t_cc_filter.e_cc_type == SM_CC_TYPE_IGNORE) {
        return SVCTXR_OK;
    }

    /* if in frozen mode, bypass it */
    if ((pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_CLOSED_CAPTION) > 0) {
        if (!SVCTX_IS_INVISIBLE_CC_FILTER(pt_running_svc_req->t_cc_filter)) {
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
    if (i4_ret == SVCTXR_OK) {
        pt_strm_req->u.t_scdb.t_comp_id = t_comp_id;
    } else {
        pt_strm_req->u.t_scdb.t_comp_id.e_type        = ST_CLOSED_CAPTION;
        pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag = NULL;
    }

    i4_ret = _ch_hdlr_handle_strm_select_req (
                    pt_svctx,
                    pt_running_svc_req,
                    pt_strm_req,
                    FALSE
                    );
    if (i4_ret == SVCTXR_OK) {
        pt_strm_req->e_penging_status = STRM_PENDING_COND_SELECT_STRM;
    } else {
        pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
        pt_strm_req->e_penging_status          = STRM_PENDING_COND_NONE;
    }

    return i4_ret;
}
#ifdef MW_ISDB_SUPPORT
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_open_isdb_cc_stream_after_video_started
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _ch_hdlr_open_isdb_cc_stream_after_video_started(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req,
                    STREAM_TYPE_T               t_stream_type,
                    UINT32                      ui4_stream_mask
                    )
{
    STRM_REQ_T*         pt_strm_req = NULL;
    STREAM_TYPE_T       e_strm_type = ST_UNKNOWN;
    INT32               i4_ret      = SVCTXR_OK;

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_strm_req = t_stream_type == ST_ISDB_CAPTION ?
        &(pt_running_svc_req->t_strm_reqs[ST_ISDB_CAPTION]) :
            ( t_stream_type == ST_ISDB_TEXT ?
              &(pt_running_svc_req->t_strm_reqs[ST_ISDB_TEXT]) : NULL );

    e_strm_type = svctx_get_strm_req_strm_type(pt_strm_req);

    /* if there's a ISDB CC stream is already opened, bypass it.
     * It could happen in a blocked channel after it is unblocked.
     */
    if (pt_strm_req == NULL
            || e_strm_type == ST_UNKNOWN
            || pt_strm_req->e_req_type != STRM_REQ_SCDB
            || x_handle_valid(pt_strm_req->h_stream) == TRUE) {
        return SVCTXR_OK;
    }

    /* if in frozen mode, bypass it */
    if ((pt_svctx->ui4_frz_frozen_strm_mask & ui4_stream_mask) > 0) {
        return SVCTXR_OK;
    }

    pt_strm_req->u.t_scdb.t_comp_id.e_type        = t_stream_type;
    pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag = NULL;

    i4_ret = _ch_hdlr_handle_strm_select_req (
                    pt_svctx,
                    pt_running_svc_req,
                    pt_strm_req,
                    FALSE
                    );
    if (i4_ret == SVCTXR_OK) {
        pt_strm_req->e_penging_status = STRM_PENDING_COND_SELECT_STRM;
    } else {
        pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
        pt_strm_req->e_penging_status          = STRM_PENDING_COND_NONE;
    }

    return i4_ret;
}
#endif
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_check_strm_scdb_record
 * Description
 * Input arguments
 *      pt_svctx        the pointer points to an SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK       the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_check_strm_scdb_record(
                    HANDLE_T                    h_scdb,
                    STREAM_TYPE_T               e_strm_type,
                    STRM_REQ_T*                 pt_strm_req,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    SCDB_REC_T t_scdb_rec;
    INT32      i4_ret = SCDBR_REC_NOT_FOUND;
    UINT16     ui2_num_recs; 
    UINT32     ui4_db_version;

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
    if (i4_ret == SCDBR_REC_NOT_FOUND) {
        return SVCTXR_NO_RECORD;
    }

    if (pt_running_svc_req->e_brdcst_type==BRDCST_TYPE_ANALOG)
    {
        SCDB_STRM_REQ_T* pt_old_rec = &pt_strm_req->u.t_scdb;
        if (e_strm_type == ST_AUDIO)
        {
            if ( (t_scdb_rec.u.t_audio_analog.e_enc != 
            pt_old_rec->t_rec.u.t_audio_analog.e_enc) ||
            (t_scdb_rec.u.t_audio_analog.ui4_tv_sys !=
            pt_old_rec->t_rec.u.t_audio_analog.ui4_tv_sys) ||
            (t_scdb_rec.u.t_audio_analog.ui4_audio_sys !=
            pt_old_rec->t_rec.u.t_audio_analog.ui4_audio_sys) )
            {
                return SVCTXR_NO_RECORD;
            }
        }
    }
    

    i4_ret = x_scdb_get_num_recs(
                    h_scdb,
                    e_strm_type,
                    &ui2_num_recs,
                    &ui4_db_version
                    );

    if ((i4_ret != SCDBR_OK) || (ui2_num_recs > 1)) {
        return SVCTXR_NO_RECORD;
    }

    return (i4_ret == SCDBR_OK ? SVCTXR_OK : SVCTXR_FAIL);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_open_sbtl_stream_after_video_started
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _ch_hdlr_open_sbtl_stream_after_video_started(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    STRM_REQ_T*                  pt_strm_req = NULL;
    INT32                        i4_ret      = SVCTXR_OK;
    STREAM_TYPE_T                e_strm_type = ST_UNKNOWN;
    STREAM_COMP_ID_T             t_comp_id;
    x_svctx_strm_comp_filter_fct pt_strm_fltr_fct;
    VOID*                        pv_strm_fltr_tag;

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE]);
    e_strm_type = svctx_get_strm_req_strm_type(pt_strm_req);
    
    /* if there's a subtitle stream is already opened, bypass it.
     * It could happen in a blocked channel after it is unblocked.
     */
    if (pt_strm_req == NULL
            || e_strm_type == ST_UNKNOWN
            || x_handle_valid(pt_strm_req->h_stream) == TRUE) {
        return SVCTXR_OK;
    }

    /* if in frozen mode, bypass it */
    if ((pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_SUBTITLE) > 0) {
        return SVCTXR_OK;
    }

    if (pt_strm_req->e_req_type == STRM_REQ_SCDB) {
        SCDB_REC_T t_scdb_rec;

        i4_ret = x_scdb_get_rec (
                        player_get_scdb_handle(pt_running_svc_req->pt_player),
                        &pt_strm_req->u.t_scdb.t_comp_id,
                        &t_scdb_rec
                        );
        if (i4_ret != SCDBR_OK) {
            ch_hdlr_choose_strm_filter(
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
            if (i4_ret == SVCTXR_OK) {
                pt_strm_req->u.t_scdb.t_comp_id = t_comp_id;
            } else {
                return SVCTXR_OK;
            }
        }

        i4_ret = ch_hdlr_open_strm(
                        pt_svctx,
                        pt_strm_req,
                        TRUE
                        );
        if (i4_ret == SVCTXR_OK) {
            pt_strm_req->e_penging_status = STRM_PENDING_COND_SELECT_STRM;
        } else {
            pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
            pt_strm_req->e_penging_status          = STRM_PENDING_COND_NONE;
        }
    } else if (pt_strm_req->e_req_type == STRM_REQ_PID) {
        i4_ret = ch_hdlr_open_strm(
                        pt_svctx,
                        pt_strm_req,
                        TRUE
                        );
        if (i4_ret == SVCTXR_OK) {
            pt_strm_req->e_penging_status = STRM_PENDING_COND_SELECT_STRM;
        } else {
            pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;
        }
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_reload_video_stream_as_blocked
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_reload_video_stream_as_blocked (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream) == FALSE) {
        svctx_ctrl_block(pt_svctx, FALSE);
        ch_hdlr_reload_video_stream(pt_svctx, pt_running_svc_req);
        svctx_ctrl_block(pt_svctx, TRUE);
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_video_strm_post_start_handler
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _ch_hdlr_video_strm_post_start_handler(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    STRM_REQ_T* pt_video_strm_req  = NULL;
    INT32       i4_ret             = SVCTXR_OK;

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_video_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]);

    do {
        if (pt_video_strm_req == NULL) {
            break;
        }

        if (x_handle_valid(pt_video_strm_req->h_stream) == FALSE) {
            break;
        }

        i4_ret = svctx_set_video_stream_attrs(pt_svctx, pt_video_strm_req->h_stream);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_svctx->b_is_blocked == TRUE) {
            break;
        }

        if (pt_video_strm_req->t_ca.e_event == SM_EVN_CA_COND_NOT_AUTHORIZED) {
            break;
        }

        if (x_handle_valid(pt_running_svc_req->h_gl_plane) == FALSE) {
            break;
        }

        if ((pt_running_svc_req->t_parent.ui4_stream_mode & MAKE_BIT_MASK_32(ST_CLOSED_CAPTION)) > 0) {
            _ch_hdlr_open_cc_stream_after_video_started(pt_svctx, pt_running_svc_req);
        }

        if ((pt_running_svc_req->t_parent.ui4_stream_mode & MAKE_BIT_MASK_32(ST_SUBTITLE)) > 0) {
            _ch_hdlr_open_sbtl_stream_after_video_started(pt_svctx, pt_running_svc_req);
        }

#ifdef MW_ISDB_SUPPORT
        if ((pt_running_svc_req->t_parent.ui4_stream_mode & MAKE_BIT_MASK_32(ST_ISDB_CAPTION)) > 0) {
            _ch_hdlr_open_isdb_cc_stream_after_video_started(pt_svctx, pt_running_svc_req, ST_ISDB_CAPTION, ST_MASK_ISDB_CAPTION );
        }

        if ((pt_running_svc_req->t_parent.ui4_stream_mode & MAKE_BIT_MASK_32(ST_ISDB_TEXT)) > 0) {
            _ch_hdlr_open_isdb_cc_stream_after_video_started(pt_svctx, pt_running_svc_req, ST_ISDB_TEXT, ST_MASK_ISDB_TEXT );
        }
#endif

    } while (0);

    return SVCTXR_OK;
}

static VOID _ch_hdlr_handle_strm_event_analog_signal(
                    SVCTX_T*                    pt_svctx,
                    SVCTX_STRM_MSG_T*           pt_strm_msg
                    )
{
    CH_SVC_REQ_T*           pt_running_svc_req = NULL;

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_svctx is NULL\r\n", __func__));
        return;
    }

    pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return ;
    }
    
    if (pt_svctx->b_in_stopping == TRUE || pt_svctx->b_in_fine_tuning == TRUE) {
        return;
    }

    if (pt_strm_msg->uinfo.e_tv_signal_status == VSH_ANA_TV_SIG_STABLE) {
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOCKED, ST_UNKNOWN);

        if (svctx_with_event_capability(
                    pt_svctx,
                    pt_running_svc_req->e_crnt_tuner_type,
                    DEV_EVN_CAPABILITY_ALL) == TRUE) {
            if (crnt_evn_is_in_querying(pt_running_svc_req->pt_crnt_event) == FALSE) {
                CRNT_EVENT_KEY_T t_key;

                t_key.t_svl.ui2_svc_lst_id = pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id;
                t_key.t_svl.ui2_svl_rec_id = pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svl_rec_id;
#ifdef MW_EDB_SUPPORT                
                t_key.t_svl.ui4_channel_id = pt_running_svc_req->t_src_info.u_info.t_svl.ui4_channel_id;
                t_key.t_svl.e_brdcst_type  = pt_running_svc_req->e_brdcst_type;
#endif

                crnt_evn_start_query(
                        pt_running_svc_req->pt_crnt_event,
                        DEV_TUNER,
                        pt_running_svc_req->t_src_info.s_src_grp_name,
                        player_get_conn_handle(pt_running_svc_req->pt_player),
                        &t_key
                        );
            }
        }

        if (pt_svctx->b_is_blocked == FALSE) {
            pt_svctx->e_state = SVCTX_COND_PRESENTING;
            ch_hdlr_invoke_client_svc_nfy_fct( pt_svctx, SVCTX_NTFY_CODE_NORMAL, ST_UNKNOWN);

            if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) == FALSE) {
                ch_hdlr_reload_audio_stream(pt_svctx, pt_running_svc_req);
            } else {
                ch_hdlr_check_audio_fmt(pt_svctx, pt_running_svc_req);
            }

            if (x_handle_valid(pt_running_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX].h_stream) == FALSE) {
                ch_hdlr_reload_ad_stream(pt_svctx, pt_running_svc_req);
            }

            if (svctx_is_valid_video_fmt(pt_svctx) == FALSE) {
                svctx_update_video_info_from_sm(
                        pt_svctx,
                        pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream
                        );
            }

            ch_hdlr_check_video_fmt(pt_svctx, pt_running_svc_req);
            _ch_hdlr_video_strm_post_start_handler(pt_svctx, pt_running_svc_req);

#ifdef MW_TV_AV_BYPASS_SUPPORT
        if (!IS_SVCTX_ASSOCIATE(pt_svctx)
                && pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ANALOG
                && pt_running_svc_req->e_ch_src_type == CH_SRC_TYPE_TUNER
                &&(pt_running_svc_req->t_src_info.u_info.t_svl.t_ch_encode.e_type == CHANNEL_DECODE_TYPE_MANUAL
                || pt_running_svc_req->t_src_info.u_info.t_svl.t_ch_encode.e_type == CHANNEL_DECODE_TYPE_AUTO))
        {
                _ch_hdlr_monitor_scart_signal(
                    pt_svctx,
                    pt_running_svc_req
                    );
            }
#endif
        }
    }
    else if (pt_strm_msg->uinfo.e_tv_signal_status == VSH_ANA_TV_SIG_NONE)
    {
        pt_svctx->e_state = SVCTX_COND_PENDING;
        ch_hdlr_check_signal_loss_priority(
                pt_svctx,
                pt_running_svc_req,
                TRUE
                );
        pt_svctx->e_signal_status = SIGNAL_STATUS_LOSS;
        crnt_evn_stop_query(pt_running_svc_req->pt_crnt_event);
        svctx_reset_video_info(pt_svctx);
        svctx_reset_audio_info(pt_svctx);
        ch_hdlr_close_streams_as_no_signal(pt_svctx, pt_running_svc_req);
    }
    else /* VSH_ANA_TV_SIG_UNKNOWN */
    {
        /* do nothing by tv signal unknown status */
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_handle_strm_event
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_strm_event(
                    SVCTX_T*                    pt_svctx,
                    STRM_REQ_T*                 pt_strm_req,
                    SVCTX_STRM_MSG_T*           pt_strm_msg
                    )
{
    CH_SVC_REQ_T*           pt_running_svc_req = NULL;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    ACTIVE_LINK_ENTRY_T*    pt_link;
    INT32                   i = 0;
#endif

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_svctx is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }


    switch(pt_strm_msg->e_event) {
    case SM_EVN_VS_ANA_TV_SIGNAL:
#ifdef MW_TV_AV_BYPASS_SUPPORT
    case SM_EVN_BYPASS_ANA_TV_SIGNAL:
#endif
        svctx_cli_print_channel_change_log(pt_svctx, CH_XNG_STAGE_GOT_TV_SIGNAL);
        _ch_hdlr_handle_strm_event_analog_signal(pt_svctx, pt_strm_msg);

#ifdef MW_TV_AV_BYPASS_SUPPORT
        /* Special process for some cases of SCART Bypass involved, when
         * the ANA_TV_SIGNAL for SCART may not be notified by driver.
         * However, we should do process for main in advance so as to 
         * show audio ASAP(DTV00323270).  */
        if (!IS_SVCTX_ASSOCIATE(pt_svctx))
        {
            while (1)
            {
                pt_link = svctx_mgr_get_link_obj(ACTIVE_LINK_NOW, pt_svctx->pt_running_svc_req->e_src_type, i++);

                if (pt_link != NULL)
                {
                    if (pt_link->pt_worker->i1_prio_diff < pt_svctx->i1_prio_diff)
                    {
                        _ch_hdlr_handle_strm_event_analog_signal(pt_link->pt_worker, pt_strm_msg);
                    }
                }
                else
                {
                    break;
                }
            }
        }
#endif
        break;

    case SM_EVN_VS_AUTO_CLK_POS_PHS_DONE:
        ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_VIDEO_AUTO_CLK_POS_PHS_DONE,
                    pt_strm_req
                    );
        break;
    case SM_EVN_VS_AUTO_COLOR_DONE:
        pt_svctx->e_auto_color_cond = (pt_strm_msg->uinfo.b_auto_color_done == TRUE) ? AUTO_COLOR_COND_SUCCESS : AUTO_COLOR_COND_FAILED;
        ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_VIDEO_AUTO_COLOR_DONE,
                    pt_strm_req
                    );
        break;
    case SM_EVN_VS_ANA_COLOR_SYSTEM:
        svctx_set_color_sys(pt_svctx, &pt_strm_msg->uinfo.t_clr_sys);
        ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_VIDEO_ANA_COLOR_SYSTEM,
                    pt_strm_req
                    );
        break;
    case SM_EVN_VS_VIDEO_RESOLUTION_CHG:
#ifdef MW_TV_AV_BYPASS_SUPPORT
    case SM_EVN_BYPASS_VIDEO_RESOLUTION_CHG:
#endif

        if (pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED) {
            /* if not signal locked, any video resolution change
             * notification is discarded.
             */
            break;
        }
        TMS_END("SELECT_SVC_2_VIDEO_FMT_NTY");
        svctx_set_video_info(pt_svctx, &pt_strm_msg->uinfo.t_video);
        svctx_cli_print_channel_change_log(pt_svctx, CH_XNG_STAGE_RECEIVED_RESOLUTION_CHG);
        ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE,
                    pt_strm_req
                    );

        /* svctx_with_event_capability need video resolution */
        if (svctx_with_event_capability(
                pt_svctx,
                pt_running_svc_req->e_crnt_tuner_type,
                DEV_EVN_CAPABILITY_ALL) == TRUE) {
            if (crnt_evn_is_in_querying(pt_running_svc_req->pt_crnt_event) == FALSE) {
                CRNT_EVENT_KEY_T t_key;

                t_key.t_svl.ui2_svc_lst_id = pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id;
                t_key.t_svl.ui2_svl_rec_id = pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svl_rec_id;
#ifdef MW_EDB_SUPPORT                
                t_key.t_svl.ui4_channel_id = pt_running_svc_req->t_src_info.u_info.t_svl.ui4_channel_id;
                t_key.t_svl.e_brdcst_type  = pt_running_svc_req->e_brdcst_type;
#endif

                crnt_evn_start_query(
                    pt_running_svc_req->pt_crnt_event,
                    DEV_TUNER,
                    pt_running_svc_req->t_src_info.s_src_grp_name,
                    player_get_conn_handle(pt_running_svc_req->pt_player),
                    &t_key
                    );
            }
        } else {
            ch_hdlr_check_svc_block(pt_svctx);
            if (crnt_evn_is_in_querying(pt_running_svc_req->pt_crnt_event) == TRUE) {
                crnt_evn_stop_query(pt_running_svc_req->pt_crnt_event);
                /* force client to re-get event again */
                ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CRNT_EVN_READY,
                    ST_UNKNOWN
                    );
            }
        }
        break;
    case SM_EVN_AS_SIG_MONITOR_AUTO_MUTE:
        if (pt_strm_msg->uinfo.ui4_data == (UINT32)ASH_NO_SIG_AUTO_MUTE_MUTE)
        {
            ch_hdlr_invoke_client_strm_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_AUDIO_AUTO_MUTE,
                        pt_strm_req
                        );
        }
        else if (pt_strm_msg->uinfo.ui4_data == (UINT32)ASH_NO_SIG_AUTO_MUTE_UNMUTE)
        {
            ch_hdlr_invoke_client_strm_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_AUDIO_AUTO_UNMUTE,
                        pt_strm_req
                        );            
        }
        else
        {
            DBG_ERROR((_ERROR_HEADER"%s(), ASH_NO_SIG_AUTO_MUTE_UNKNOWN\r\n", __func__));
        }
        break;
    case SM_EVN_AS_AUD_INFO_CHG:
        if (!pt_strm_req->b_aux_audio)
        {
            STRM_REQ_T* pt_ad_strm_req = &pt_running_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX];

            svctx_set_audio_info(pt_svctx, &pt_strm_msg->uinfo.t_audio);

            /** For Dolby MS10 Certification: **/
            if (ch_hdlr_is_valid_for_ad_select(pt_svctx, pt_running_svc_req))
            {
                /* Only try to reload ad after telling main audio's format. */
                if ((x_handle_valid(pt_ad_strm_req->h_stream) == FALSE
                        || pt_ad_strm_req->e_strm_cond != SM_COND_STARTED) 
                    && pt_strm_req->ui1_mixed_substrm_idx == AUDIO_INDEX_UNKNOW)
                {
                    ch_hdlr_reload_ad_stream(pt_svctx, pt_running_svc_req);
                }
            }
            else
            {
                /* Should not play ad if main audio is dual_mono. */
                INT32       i4_ret    = SVCTXR_OK;
                if (x_handle_valid(pt_ad_strm_req->h_stream) == TRUE)
                {
                #ifdef MW_TV_AV_BYPASS_SUPPORT
                    i4_ret = ch_hdlr_try_closing_stream(pt_svctx, pt_ad_strm_req->h_stream);
                    if (i4_ret == SVCTXR_OK)
                #else
                    i4_ret = x_sm_close_stream(pt_ad_strm_req->h_stream);
                    if (i4_ret == SMR_OK || i4_ret == SMR_PENDING)
                #endif
                    {
                        pt_running_svc_req->t_strm_reqs[ST_AUDIO].e_strm_cond = SM_COND_CLOSING;
                    }
                    else
                    {
                        DBG_ERROR((_ERROR_HEADER"%s(), [%s]faile to close ad. ret=%d\r\n", __func__, pt_svctx->s_name, i4_ret));
                    }
                }
            }
        }

        ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE,
                    pt_strm_req
                    );
        break;
    case SM_EVN_AS_TV_AUD_SYS_DETECTED:
        pt_svctx->t_ana_auto_detected_aud_sys.ui4_aud_sys = pt_strm_msg->uinfo.t_aud_sys.ui4_aud_sys;
        pt_svctx->t_ana_auto_detected_aud_sys.ui4_tv_sys = pt_strm_msg->uinfo.t_aud_sys.ui4_tv_sys;
        ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_ANA_AUTO_DETECTED_AUD_SYS,
                    pt_strm_req
                    );
        break;
    case SM_EVN_CA_COND_NOT_AUTHORIZED:
        switch(svctx_get_strm_req_strm_type(pt_strm_req)) {
        case ST_VIDEO:
            /* it could be from clear to scrambled,
             * and need to set video plane to blank(background) mode
             * and reset video format information.
             */
            pt_running_svc_req->b_scramble_video = TRUE;
            if (pt_svctx->ui4_frz_frozen_strm_mask != 0)
            {
                pt_svctx->ui4_frz_frozen_strm_mask = 0;
                ch_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_UNFREEZE,
                        ST_UNKNOWN
                        );
            }

            ch_hdlr_set_video_plane(pt_svctx, SCC_VID_BLANK);
            svctx_reset_video_info(pt_svctx);
            strm_fmt_chker_stop_delay(pt_running_svc_req->pt_strm_fmt_chker, ST_VIDEO);

            /* To conquer driver's limitation, we need tell audio driver
             *    the video is scrambled. */
            x_scc_aud_set_video_scramble_status(pt_svctx->h_audio_scc_comp, TRUE);

            #ifdef MW_NO_AUDIO_FOR_SCRAMBLED_VIDEO
              svctx_ctrl_main_audio(pt_svctx, TRUE);
              DBG_INFO((_INFO_HEADER"Mute audio for scrambled video\n" ));
            #endif

            break;
        case ST_AUDIO:
            pt_running_svc_req->b_scramble_audio = TRUE;
            strm_fmt_chker_stop_delay(pt_running_svc_req->pt_strm_fmt_chker, ST_AUDIO);
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
        ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CA,
                    pt_strm_req
                    );
        break;
    case SM_EVN_CA_COND_UNKNOWN:
#ifdef MW_TV_AV_BYPASS_SUPPORT
        svctx_handle_copy_protection(
                        pt_svctx,
                        pt_strm_req,
                        pt_strm_msg);
#endif
        pt_strm_req->t_ca = pt_strm_msg->uinfo.t_ca;
        return SVCTXR_OK;
    case SM_EVN_CA_COND_CLOSED:
    case SM_EVN_CA_COND_FREE:
    case SM_EVN_CA_COND_AUTHORIZED:
    case SM_EVN_CA_COND_AUTHORIZED_CA_SYS_ID:
        switch(svctx_get_strm_req_strm_type(pt_strm_req)) {
        case ST_VIDEO:
            pt_running_svc_req->b_scramble_video = (pt_strm_msg->e_event == SM_EVN_CA_COND_AUTHORIZED_CA_SYS_ID);
            
            if (svctx_is_scrambled_strm(&pt_strm_req->t_ca) == TRUE
                    && pt_svctx->e_signal_status == SIGNAL_STATUS_LOCKED
                    && pt_svctx->b_is_blocked == FALSE) {
                /* if it is from scrambled to clear and current content is not
                 * blocked, it need to set video plane to normal/dinit mode
                 * and re-get and re-send video format information again
                 */
                ch_hdlr_set_video_plane(pt_svctx, pt_svctx->e_default_vid_mode);

                /* For its limitation, tell audio driver the video is clear. */
                x_scc_aud_set_video_scramble_status(pt_svctx->h_audio_scc_comp, FALSE);

                pt_strm_req->t_ca = pt_strm_msg->uinfo.t_ca;
                if (svctx_is_valid_video_fmt(pt_svctx) == FALSE) {
                    svctx_update_video_info_from_sm(pt_svctx, pt_strm_req->h_stream);
                    if (svctx_is_valid_video_fmt(pt_svctx) == TRUE) {
                        ch_hdlr_invoke_client_strm_nfy_fct(
                                    pt_svctx,
                                    SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE,
                                    pt_strm_req
                                    );
                    }
                    _ch_hdlr_video_strm_post_start_handler(pt_svctx, pt_running_svc_req);
                }
            } else {
                pt_strm_req->t_ca = pt_strm_msg->uinfo.t_ca;
            }

            #ifdef MW_NO_AUDIO_FOR_SCRAMBLED_VIDEO
                svctx_ctrl_main_audio(pt_svctx, FALSE);
                DBG_INFO((_INFO_HEADER"Unmute audio for clear or descrambled video\n" ));
            #endif

            break;
        case ST_AUDIO:
            pt_running_svc_req->b_scramble_audio = (pt_strm_msg->e_event == SM_EVN_CA_COND_AUTHORIZED_CA_SYS_ID);
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
        ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CA,
                    pt_strm_req
                    );
        break;
    case SM_EVN_CCI_INFORMATION:
        pt_strm_req->t_cp = pt_strm_msg->uinfo.t_cp;
        
#ifdef MW_TV_AV_BYPASS_SUPPORT
        svctx_handle_copy_protection(
                        pt_svctx,
                        pt_strm_req,
                        pt_strm_msg);
#endif

        ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CP,
                    pt_strm_req
                    );
        break;
    case SM_EVN_CCS_EXISTED_CC_IND_CHG:
        ch_hdlr_invoke_client_svc_nfy_fct(
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
            ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_WITH_CC_AVIL_INDICATOR,
                    ST_CLOSED_CAPTION
                    );
        } else {
            pt_svctx->e_cc_avil_cond = SVCTX_CC_AVIL_COND_NO;
            ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_NO_CC_AVIL_INDICATOR,
                    ST_CLOSED_CAPTION
                    );
        }
        break;
    case SM_EVN_VS_ANA_TV_SIGNAL_DETECTING:
        ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_SIGNAL_DETECTING,
                    ST_UNKNOWN
                    );
        break;
    case SM_EVN_VS_DIG_DECODE_STATUS_CHG:
        switch(pt_strm_msg->uinfo.e_video_decode_status) {
        case VSH_DIG_DECODE_DATA_RECEIVE:
            svctx_cancel_no_video_related_delay_nfy(pt_svctx);
            break;
        case VSH_DIG_DECODE_NO_DATA:
        case VSH_DIG_DECODE_ERROR:
        case VSH_DIG_DECODE_HD_NOT_SUPPORT:
            pt_svctx->e_video_decode_status = pt_strm_msg->uinfo.e_video_decode_status;
            ch_hdlr_set_video_plane(pt_svctx, SCC_VID_BLANK);
            svctx_reset_video_info(pt_svctx);
            if (pt_strm_msg->uinfo.e_video_decode_status == VSH_DIG_DECODE_ERROR) {
                strm_fmt_chker_set_format_status(pt_running_svc_req->pt_strm_fmt_chker, ST_VIDEO);
            }
            ch_hdlr_check_strms_status(pt_svctx);
            break;
        case VSH_DIG_DECODE_NORMAL:
            if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOCKED
                    && pt_svctx->b_is_blocked == FALSE
                    &&(pt_svctx->e_video_decode_status == VSH_DIG_DECODE_NO_DATA
                        || pt_svctx->e_video_decode_status == VSH_DIG_DECODE_ERROR)) {

                pt_svctx->e_video_decode_status = pt_strm_msg->uinfo.e_video_decode_status;
                ch_hdlr_set_video_plane(pt_svctx, pt_svctx->e_default_vid_mode);

                if (svctx_is_valid_video_fmt(pt_svctx) == FALSE) {
                    svctx_update_video_info_from_sm(pt_svctx, pt_strm_req->h_stream);
                    if (svctx_is_valid_video_fmt(pt_svctx) == TRUE) {
                        ch_hdlr_invoke_client_strm_nfy_fct(
                                    pt_svctx,
                                    SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE,
                                    pt_strm_req
                                    );
                    }
                    _ch_hdlr_video_strm_post_start_handler(pt_svctx, pt_running_svc_req);
                }
            } else {
                pt_svctx->e_video_decode_status = pt_strm_msg->uinfo.e_video_decode_status;
            }
            break;
        case VSH_DIG_DECODE_UNKNOWN:
        default:
            pt_svctx->e_video_decode_status = pt_strm_msg->uinfo.e_video_decode_status;
            break;
        }
        break;
    case SM_EVN_VS_CODEC_NOT_SUPPORT:
#ifdef MW_TV_AV_BYPASS_SUPPORT
    case SM_EVN_BYPASS_CODEC_NOT_SUPPORT:
#endif
        pt_svctx->e_video_decode_status = VSH_DIG_DECODE_CODEC_NOT_SUPPORT;
        ch_hdlr_set_video_plane(pt_svctx, SCC_VID_BLANK);
        svctx_reset_video_info(pt_svctx);
        strm_fmt_chker_set_format_status(pt_running_svc_req->pt_strm_fmt_chker, ST_VIDEO);
        ch_hdlr_check_strms_status(pt_svctx);

#ifdef MW_TV_AV_BYPASS_SUPPORT
        while (1)
        {
            pt_link = svctx_mgr_get_link_obj(
                ACTIVE_LINK_NOW,
                pt_svctx->pt_running_svc_req->e_src_type,
                i++);

            if (pt_link)
            {
                if (pt_link->pt_worker != pt_svctx)
                {
                    pt_link->pt_worker->e_video_decode_status = pt_svctx->e_video_decode_status;
                    ch_hdlr_check_strms_status(pt_link->pt_worker);
                }
            }
            else
            {
                break;
            }
        }
#endif
        break;
    case SM_EVN_AS_CODEC_NOT_SUPPORT:
        ch_hdlr_invoke_client_strm_nfy_fct(
                       pt_svctx,
                       SVCTX_NTFY_CODE_MEDIA_AS_CODEC_NOT_SUPPORT,
                       pt_strm_req
                       );
        break;
    case SM_EVN_AS_DECODE_STATUS_CHG:
        if (!pt_strm_req->b_aux_audio) {
            switch(pt_strm_msg->uinfo.e_audio_decode_status) {
            case ASH_AUDIO_DECODE_NO_DATA:
            case ASH_AUDIO_DECODE_ERROR:
                pt_svctx->e_audio_decode_status = pt_strm_msg->uinfo.e_audio_decode_status;
                svctx_reset_audio_info(pt_svctx);
                ch_hdlr_check_strms_status(pt_svctx);
                break;
            case ASH_AUDIO_DECODE_NORMAL:
                if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOCKED
                        && pt_svctx->b_is_blocked == FALSE
                        && (pt_svctx->e_audio_decode_status == ASH_AUDIO_DECODE_NO_DATA
                            || pt_svctx->e_audio_decode_status == ASH_AUDIO_DECODE_ERROR)) {

                    pt_svctx->e_audio_decode_status = pt_strm_msg->uinfo.e_audio_decode_status;
                    if (svctx_is_valid_audio_fmt(pt_svctx) == FALSE) {
                        svctx_update_audio_info_from_sm(pt_svctx, pt_strm_req->h_stream);
                        if (svctx_is_valid_audio_fmt(pt_svctx) == TRUE) {
                            ch_hdlr_invoke_client_strm_nfy_fct(
                                        pt_svctx,
                                        SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE,
                                        pt_strm_req
                                        );
                        }
                    }
                } else {
                    pt_svctx->e_audio_decode_status = pt_strm_msg->uinfo.e_audio_decode_status;
                }
                break;
            case ASH_AUDIO_DECODE_UNKNOWN:
            default:
                pt_svctx->e_audio_decode_status = pt_strm_msg->uinfo.e_audio_decode_status;
                break;
            }
        }
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
            ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    (pt_strm_msg->uinfo.ui4_data ? SVCTX_NTFY_CODE_HDCP_DETECT : SVCTX_NTFY_CODE_HDCP_NOT_DETECT),
                    pt_strm_req
                    );
        }
        else if (pt_strm_msg->e_event == SM_EVN_BYPASS_MACROVISION)
        {
            ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    (pt_strm_msg->uinfo.ui4_data ? SVCTX_NTFY_CODE_MACROVISION_DETECT : SVCTX_NTFY_CODE_MACROVISION_NOT_DETECT),
                    pt_strm_req
                    );
        }
        break;
#endif

    /* This is a special case. When the audio encoding format changes, it's up
     * to SM to decide whether to restart the audio. If yes, SM sends below
     * event to notify us to do the process properly. */
    case SM_EVN_AS_REQ_RESTART:
#ifdef MW_TV_AV_BYPASS_SUPPORT
        ch_hdlr_try_closing_stream(pt_svctx, pt_strm_req->h_stream);
#else
        x_sm_close_stream(pt_strm_req->h_stream);
        pt_strm_req->e_strm_cond = SM_COND_CLOSING;
#endif
        ch_hdlr_reload_audio_stream(pt_svctx, pt_running_svc_req);
        break;

    case SM_EVN_VS_REQ_RESTART:
#ifdef MW_TV_AV_BYPASS_SUPPORT
        ch_hdlr_try_closing_stream(pt_svctx, pt_strm_req->h_stream);
#else
        x_sm_close_stream(pt_strm_req->h_stream);
        pt_strm_req->e_strm_cond = SM_COND_CLOSING;
#endif
        ch_hdlr_reload_video_stream(pt_svctx, pt_running_svc_req);
        break;

    default:
        /*x_dbg_stmt("%s() --- unhandled event 0x%x\r\n", __func__, pt_strm_msg->e_event);*/
        break;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_handle_strm_started_nty_as_presenting
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_strm_started_nty_as_presenting(
                    SVCTX_T*                    pt_svctx,
                    STRM_REQ_T*                 pt_strm_req
                    )
{
    INT32         i4_ret               = SVCTXR_OK;
    CH_SVC_REQ_T* pt_running_svc_req   = NULL;
    STREAM_TYPE_T e_strm_type          = svctx_get_strm_req_strm_type(pt_strm_req);
    STRM_REQ_T**  ppt_pending_strm_req = NULL;

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_svctx is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req   = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    if ((e_strm_type == ST_UNKNOWN) || (e_strm_type >= STRM_TYPE_COUNT)) {
        return SVCTXR_INV_ARG;
    }

    ppt_pending_strm_req = GET_PENDING_STRM_REQ_PPTR(
        pt_running_svc_req, pt_strm_req->b_aux_audio, e_strm_type);

    switch(pt_strm_req->e_penging_status) {
    case STRM_PENDING_COND_SELECT_STRM:
        if (e_strm_type != ST_UNKNOWN && (*ppt_pending_strm_req) != NULL) {
#ifdef MW_TV_AV_BYPASS_SUPPORT
            i4_ret = ch_hdlr_try_closing_stream(pt_svctx, pt_strm_req->h_stream);
#else
            i4_ret = x_sm_close_stream(pt_strm_req->h_stream);
#endif
        } else {
            ch_hdlr_invoke_client_strm_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_STREAM_STARTED,
                        pt_strm_req
                        );
            pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;
            if (e_strm_type == ST_VIDEO) {
                _ch_hdlr_video_strm_post_start_handler(pt_svctx, pt_running_svc_req);
            }
        }
        break;
    case STRM_PENDING_COND_STOP_STRM:
#ifdef MW_TV_AV_BYPASS_SUPPORT
        i4_ret = ch_hdlr_try_closing_stream(pt_svctx, pt_strm_req->h_stream);
#else
        i4_ret = x_sm_close_stream(pt_strm_req->h_stream);
#endif
        break;
    case STRM_PENDING_COND_NONE:
        ch_hdlr_invoke_client_strm_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_STREAM_STARTED,
                        pt_strm_req
                        );
        if (e_strm_type == ST_VIDEO) {
            i4_ret = _ch_hdlr_video_strm_post_start_handler(pt_svctx, pt_running_svc_req);
        }
        break;
    default:
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_handle_strm_started_nty_as_pending
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_strm_started_nty_as_pending(
                    SVCTX_T*                    pt_svctx,
                    STRM_REQ_T*                 pt_strm_req
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = NULL;
    STREAM_TYPE_T e_strm_type        = svctx_get_strm_req_strm_type(pt_strm_req);

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_svctx is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    if (e_strm_type == ST_VIDEO) {
        _ch_hdlr_video_strm_post_start_handler(pt_svctx, pt_running_svc_req);
    }

    ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_STREAM_STARTED,
                    pt_strm_req
                    );

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_handle_strm_closed_nty_as_presenting
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_strm_closed_nty_as_presenting(
                    SVCTX_T*                    pt_svctx,
                    STRM_REQ_T*                 pt_strm_req
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req   = NULL;
    STREAM_TYPE_T e_strm_type          = svctx_get_strm_req_strm_type(pt_strm_req);
    STRM_REQ_T**  ppt_pending_strm_req = NULL;
    INT32         i4_ret               = SVCTXR_OK;

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_svctx is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    if ((e_strm_type == ST_UNKNOWN) || (e_strm_type >= STRM_TYPE_COUNT)) {
        return SVCTXR_INV_ARG;
    }

    switch(pt_strm_req->e_penging_status) {
    case STRM_PENDING_COND_SELECT_STRM:
        if (e_strm_type == ST_UNKNOWN) {
            pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;
            break;
        }

        ppt_pending_strm_req = GET_PENDING_STRM_REQ_PPTR(
            pt_running_svc_req, pt_strm_req->b_aux_audio, e_strm_type);

        if (*ppt_pending_strm_req != NULL && pt_svctx->b_is_blocked == FALSE) {
            *pt_strm_req = *(*ppt_pending_strm_req);
            svctx_free_strm_req(*ppt_pending_strm_req);
            *ppt_pending_strm_req = NULL;
            i4_ret = ch_hdlr_open_strm(pt_svctx, pt_strm_req, TRUE);
            if (i4_ret == SVCTXR_OK) {
                pt_strm_req->e_penging_status = STRM_PENDING_COND_SELECT_STRM;
            } else {
                pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;
            }
        } else {
            ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_STREAM_STOPPED,
                    pt_strm_req
                    );
            pt_strm_req->h_stream         = NULL_HANDLE;
            pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;
        }
        break;
    case STRM_PENDING_COND_STOP_STRM:
        ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_STREAM_STOPPED,
                    pt_strm_req
                    );
        pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;
        pt_strm_req->h_stream         = NULL_HANDLE;
        break;
    default:
        ch_hdlr_invoke_client_strm_nfy_fct(
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
 *      _ch_hdlr_handle_strm_closed_nty_as_pending
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_strm_closed_nty_as_pending(
                    SVCTX_T*                    pt_svctx,
                    STRM_REQ_T*                 pt_strm_req
                    )
{
    CH_SVC_REQ_T*    pt_running_svc_req = NULL;
    STREAM_TYPE_T    e_strm_type        = svctx_get_strm_req_strm_type(pt_strm_req);
    SVC_BLOCK_COND_T e_block_cond;
    STRM_REQ_T**     ppt_pending_strm_req = NULL;
    INT32            i4_ret               = SVCTXR_OK;

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_svctx is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    if ((e_strm_type == ST_UNKNOWN) || (e_strm_type >= STRM_TYPE_COUNT)) {
        return SVCTXR_INV_ARG;
    }

    /* Added for DTV00324698. If the scdb record of video changes, we have to
     * reload the video even if channel blocked. So here we need go ahead to
     * open the new video stream after old one closed */
    if (pt_svctx->b_is_blocked 
            && e_strm_type == ST_VIDEO 
            && pt_strm_req->e_penging_status == STRM_PENDING_COND_SELECT_STRM) {

        ppt_pending_strm_req = GET_PENDING_STRM_REQ_PPTR(
                    pt_running_svc_req, pt_strm_req->b_aux_audio, e_strm_type);

        if (*ppt_pending_strm_req != NULL) {
            *pt_strm_req = *(*ppt_pending_strm_req);
            svctx_free_strm_req(*ppt_pending_strm_req);
            *ppt_pending_strm_req = NULL;
            i4_ret = ch_hdlr_open_strm(pt_svctx, pt_strm_req, TRUE);
            if (i4_ret == SVCTXR_OK) {
                pt_strm_req->e_penging_status = STRM_PENDING_COND_SELECT_STRM;
            } else {
                pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;
            }
        } else {
            ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_STREAM_STOPPED,
                    pt_strm_req
                    );
            pt_strm_req->h_stream         = NULL_HANDLE;
            pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;
        }
        return i4_ret;
    }

    ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_STREAM_STOPPED,
                    pt_strm_req
                    );
    pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;
    pt_strm_req->h_stream         = NULL_HANDLE;

#ifdef MW_TV_AV_BYPASS_SUPPORT
    ppt_pending_strm_req = GET_PENDING_STRM_REQ_PPTR(
        pt_running_svc_req, pt_strm_req->b_aux_audio, e_strm_type);

    if (*ppt_pending_strm_req != NULL)
    {
        svctx_free_strm_req(*ppt_pending_strm_req);
        *ppt_pending_strm_req = NULL;
    }
#endif

    if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) {
        return SVCTXR_OK;
    }

    /* if service block, retain the stream type */

    e_block_cond = rating_chker_get_block_cond(pt_running_svc_req->pt_rating_chker);

    if (e_strm_type == ST_CLOSED_CAPTION
        && e_block_cond != SVC_BLOCK_COND_PASSED
        && e_block_cond != SVC_BLOCK_COND_PASSED_AND_NO_RATING_SET) {
        /* pt_strm_req[ST_CLOSED_CAPTION] must be come from SCDB */
        pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_CLOSED_CAPTION;
    } else {
        pt_strm_req->e_req_type                = STRM_REQ_SCDB;
        pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_handle_strm_msg
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_strm_msg (
                    SVCTX_T*                    pt_svctx,
                    SVCTX_STRM_MSG_T*           pt_strm_msg
                    )
{
    STRM_REQ_T* pt_strm_req = NULL;
    INT32       i4_ret = SVCTXR_OK;
    STREAM_TYPE_T   e_strm_type;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    CH_SVC_REQ_T*   pt_svc_req = NULL;
    VAR_INFO_T      t_var_info;
#endif

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_svctx is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    pt_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }
#endif

    i4_ret = _ch_hdlr_get_strm_req_from_handle(
                    pt_svctx,
                    pt_strm_msg->h_obj,
                    &pt_strm_req,
                    &e_strm_type
                    );
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    svctx_cli_print_stream_component_idx(pt_svctx, 2, e_strm_type, pt_strm_req->h_stream, pt_strm_msg->e_event, pt_strm_msg->e_cond, __FILE__, __LINE__);

    _ch_hdlr_handle_strm_event(
                    pt_svctx,
                    pt_strm_req,
                    pt_strm_msg
                    );
    if (pt_strm_req->e_strm_cond == pt_strm_msg->e_cond) {
        return i4_ret;
    }

    /* During closing a stream, the status in SM will go OPENED after 
     * driver stopped it, but we should regard it as CLOSING in SVCTX */
    if (pt_strm_msg->e_event == SM_EVN_AS_DRV_STOPPED
            && pt_strm_msg->e_cond == SM_COND_OPENED) {
        pt_strm_req->e_strm_cond = SM_COND_CLOSING;
    } else {
        pt_strm_req->e_strm_cond = pt_strm_msg->e_cond;
    }

    switch(pt_strm_req->e_strm_cond) {
    case SM_COND_STARTED:
#ifdef MW_TV_AV_BYPASS_SUPPORT
#ifdef CLI_LVL_ALL
        DBG_SVCTXMGR_MSG((
            "{SVCTX_MGR} <<< (%s) %s stream became STARTED.\r\n",
            pt_svctx->s_name,
            s_g_stream[e_strm_type]));
#endif
        i4_ret = svctx_mgr_check_queued_req(
            ACTIVE_LINK_NOW,
            pt_svctx->pt_running_svc_req->e_src_type,
            NULL,
            FALSE);

        if (ST_VIDEO == e_strm_type)
        {
            if (pt_strm_msg->e_event == SM_EVN_VS_DRV_PLAYED)
            {
                svctx_cancel_no_video_related_delay_nfy(pt_svctx);
            }
            
            if (pt_svc_req->e_brdcst_type == BRDCST_TYPE_ANALOG)
            {
                svctx_enable_slave_audio(pt_svctx, SLAVE_AUDIO_CTRL_FORCE_ENABLE);
            }
            else
            {
                svctx_enable_slave_audio(pt_svctx, SLAVE_AUDIO_CTRL_ENABLE);
            }
        }
        else if (ST_AUDIO == e_strm_type)
        {
            /* Handle audio stream only channel case */
            if (pt_svc_req->t_strm_reqs[ST_VIDEO].h_stream == NULL_HANDLE)
            {
                svctx_enable_slave_audio(pt_svctx, SLAVE_AUDIO_CTRL_FORCE_ENABLE);
            }
        }
#endif
        if (pt_svctx->e_state == SVCTX_COND_PRESENTING) {
            i4_ret = _ch_hdlr_handle_strm_started_nty_as_presenting(pt_svctx, pt_strm_req);
        } else if (pt_svctx->e_state == SVCTX_COND_PENDING) {
            i4_ret = _ch_hdlr_handle_strm_started_nty_as_pending(pt_svctx, pt_strm_req);
        }
        break;
    case SM_COND_CLOSED:
#ifdef MW_TV_AV_BYPASS_SUPPORT
#ifdef CLI_LVL_ALL
        DBG_SVCTXMGR_MSG((
            "{SVCTX_MGR} <<< (%s) %s stream became CLOSED.\r\n",
            pt_svctx->s_name,
            s_g_stream[e_strm_type]));
#endif

        if (ST_VIDEO == e_strm_type)
        {
            svctx_enable_slave_audio(pt_svctx, SLAVE_AUDIO_CTRL_DISABLE);
        }

        t_var_info.e_var_type = VAR_TYPE_STREAM;
        t_var_info.e_op_type  = OP_TYPE_CLOSE;
        t_var_info.u.t_strm_info.e_strm_type = e_strm_type;

        svctx_mgr_remove_solved_qr(
            ACTIVE_LINK_NOW,
            pt_svctx->pt_running_svc_req->e_src_type,
            pt_svctx,
            &t_var_info);

        i4_ret = svctx_mgr_check_queued_req(
            ACTIVE_LINK_NOW,
            pt_svctx->pt_running_svc_req->e_src_type,
            NULL,
            FALSE);
#endif

        if (pt_strm_req->t_stop_cb.pf_nfy != NULL) {
            pt_strm_req->t_stop_cb.pf_nfy(pt_strm_req->t_stop_cb.pv_tag);
            pt_strm_req->t_stop_cb.pf_nfy = NULL;
            pt_strm_req->t_stop_cb.pv_tag = NULL;
        }

        if (pt_svctx->e_state == SVCTX_COND_PRESENTING) {
            i4_ret = _ch_hdlr_handle_strm_closed_nty_as_presenting(pt_svctx, pt_strm_req);
        } else if (pt_svctx->e_state == SVCTX_COND_PENDING) {
            i4_ret = _ch_hdlr_handle_strm_closed_nty_as_pending(pt_svctx, pt_strm_req);
        }
        break;
    case SM_COND_ERROR:
        break;
    default:
        break;
    }
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_get_svl
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
static INT32 _ch_hdlr_get_svl(
                    CH_SVC_REQ_T*               pt_svc_req,
                    HANDLE_T*                   ph_svl
                    )
{
    INT32 i4_ret;
#ifdef SYS_MHP_SUPPORT
    UINT16 ui2_svl_lst_id;
#endif

    ASSERT(pt_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));
    ASSERT(pt_svc_req->t_parent.e_src_type == SRC_TYPE_BRDCST_SVL, ("%s(), not SRC_TYPE_BRDCST_SVL\r\n", __func__));
#ifdef SYS_MHP_SUPPORT
    if (pt_svc_req->t_src_info.u_info.t_svl.t_silent.b_enable == TRUE) {
        if (pt_svc_req->t_src_info.u_info.t_svl.t_silent.ui2_svc_lst_id == SVL_NULL_ID) {
            return SVCTXR_FAIL;
        }
        ui2_svl_lst_id = pt_svc_req->t_src_info.u_info.t_svl.t_silent.ui2_svc_lst_id;
    } else {
        if (pt_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id == SVL_NULL_ID) {
            return SVCTXR_FAIL;
        }
        ui2_svl_lst_id = pt_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id;
    }

    i4_ret = rating_chker_open_svl(
                    pt_svc_req->pt_rating_chker,
                    ui2_svl_lst_id
                    );
#else
    if (pt_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id == SVL_NULL_ID) {
        return SVCTXR_FAIL;
    }

    i4_ret = rating_chker_open_svl(
                    pt_svc_req->pt_rating_chker,
                    pt_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id
                    );
#endif
    if (i4_ret == SVCTXR_OK) {
        *ph_svl = rating_chker_get_svl_handle(pt_svc_req->pt_rating_chker);
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_update_tuner_type
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_update_tuner_type(
                    CH_SVC_REQ_T*               pt_running_svc_req,
                    SVL_REC_T*                  pt_svl_rec
                    )
{
    INT32     i4_ret;
    TSL_REC_T t_tsl_rec;

    if (pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    i4_ret = ch_hdlr_get_crnt_tsl_rec(
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
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_get_cm_cmds
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static  INT32 _ch_hdlr_get_cm_cmds(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req,
                    CM_COMMAND_T*               pt_cm_cmds
                    )
{
    INT32 i4_ret = SVCTXR_OK;
    INT32 i4_idx = 0;

    if (pt_svctx == NULL || pt_running_svc_req == NULL || pt_cm_cmds == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_cm_cmds[0].e_code = CONN_END;

    switch (pt_running_svc_req->t_parent.e_src_type) {
    case SRC_TYPE_BRDCST_SVL:
    {
        SVL_REC_T t_svl_rec = {0};

        i4_ret = ch_hdlr_get_crnt_svl_rec(
                    pt_running_svc_req,
                    &t_svl_rec
                    );
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if ((t_svl_rec.uheader.t_rec_hdr.ui4_nw_mask & pt_svctx->ui4_invalid_svl_nw_mask_bit) > 0) {
            ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_SIGNAL_LOSS,
                    ST_UNKNOWN
                    );
            break;
        }

        _ch_hdlr_update_tuner_type(pt_running_svc_req, &t_svl_rec);

        /* source name should be after source type */
        pt_running_svc_req->e_brdcst_type = t_svl_rec.uheader.t_rec_hdr.e_brdcst_type;
        switch(pt_running_svc_req->e_brdcst_type) {
        case BRDCST_TYPE_UNKNOWN:
        case BRDCST_TYPE_ATSC:
        case BRDCST_TYPE_SCTE:
            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_TYPE;
            pt_cm_cmds[i4_idx++].u.ps_name    = BRDCST_HANDLER_DEFAULT_NAME;
            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_NAME;
            pt_cm_cmds[i4_idx++].u.ps_name    = pt_running_svc_req->t_src_info.s_src_grp_name;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SVC_LST_ID;
            pt_cm_cmds[i4_idx++].u.ui2_number = pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SVL_REC_ID;
            pt_cm_cmds[i4_idx++].u.ui2_number = t_svl_rec.ui2_svl_rec_id;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_PAT_TIME_OUT;
            pt_cm_cmds[i4_idx++].u.ui4_number = ATSC_PAT_QUERY_TIMEOUT;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_PMT_TIME_OUT;
            pt_cm_cmds[i4_idx++].u.ui4_number = ATSC_PMT_QUERY_TIMEOUT;
#ifdef MW_TV_AV_BYPASS_SUPPORT                        
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_MGT_TIME_OUT;
            pt_cm_cmds[i4_idx++].u.ui4_number = ATSC_MGT_QUERY_TIMEOUT;
#endif              
#ifdef BRDCST_CONN_REUSE_CONNECTION
            if (pt_running_svc_req->pt_player->b_reuse_pipe == TRUE) {
                pt_cm_cmds[i4_idx].e_code       = BRDCST_CONN_REUSE_CONNECTION;
                pt_cm_cmds[i4_idx++].u.h_handle = player_get_conn_handle(pt_running_svc_req->pt_player);
            }
#endif
#if (defined (MW_TV_AV_BYPASS_SUPPORT) || defined (SYS_RECORD_SUPPORT))
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_TUNER_NO_RIGHTS_OK;
            pt_cm_cmds[i4_idx++].u.b_boolean  = TRUE;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SIGNAL_LOST_DISCONNECT;
            pt_cm_cmds[i4_idx++].u.b_boolean  = FALSE;
#endif
            pt_cm_cmds[i4_idx].e_code         = CONN_END;
            break;
        case BRDCST_TYPE_DTMB:
            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_TYPE;
            pt_cm_cmds[i4_idx++].u.ps_name    = BRDCST_HANDLER_DEFAULT_NAME;
            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_NAME;
            pt_cm_cmds[i4_idx++].u.ps_name    = pt_running_svc_req->t_src_info.s_src_grp_name;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SVC_LST_ID;
            pt_cm_cmds[i4_idx++].u.ui2_number = pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SVL_REC_ID;
            pt_cm_cmds[i4_idx++].u.ui2_number = t_svl_rec.ui2_svl_rec_id;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_PAT_TIME_OUT;
            pt_cm_cmds[i4_idx++].u.ui4_number = DVB_PAT_QUERY_TIMEOUT;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_PMT_TIME_OUT;
            pt_cm_cmds[i4_idx++].u.ui4_number = DVB_PMT_QUERY_TIMEOUT;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_2_LANGUAGES_1_STREAM;
            pt_cm_cmds[i4_idx++].u.b_boolean  = FALSE; /* To have 2 DUAL_MONO streams when there are 2 languages */
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_N_LANGUAGES_1_STREAM;
            pt_cm_cmds[i4_idx++].u.b_boolean  = FALSE; /* To have n UNKNOWN streams when there are n languages (n > 2) */
#ifdef BRDCST_CONN_REUSE_CONNECTION
            if (pt_running_svc_req->pt_player->b_reuse_pipe == TRUE) {
                pt_cm_cmds[i4_idx].e_code       = BRDCST_CONN_REUSE_CONNECTION;
                pt_cm_cmds[i4_idx++].u.h_handle = player_get_conn_handle(pt_running_svc_req->pt_player);
            }
#endif
#if (defined (MW_TV_AV_BYPASS_SUPPORT) || defined (SYS_RECORD_SUPPORT))
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_TUNER_NO_RIGHTS_OK;
            pt_cm_cmds[i4_idx++].u.b_boolean  = TRUE;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SIGNAL_LOST_DISCONNECT;
            pt_cm_cmds[i4_idx++].u.b_boolean  = FALSE;
#endif
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_DUAL_MONO_FAKE_STEREO;
            pt_cm_cmds[i4_idx++].u.b_boolean  = pt_running_svc_req->t_src_info.u_info.t_svl.b_enable_fake_stereo;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SUBSTREAM_FAKE_STEREO;
            pt_cm_cmds[i4_idx++].u.b_boolean  = pt_running_svc_req->t_src_info.u_info.t_svl.b_substream_fake_stereo;
            pt_cm_cmds[i4_idx].e_code         = CONN_END;
            break;

        case BRDCST_TYPE_DVB:
            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_TYPE;
            pt_cm_cmds[i4_idx++].u.ps_name    = BRDCST_HANDLER_DEFAULT_NAME;
            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_NAME;
            pt_cm_cmds[i4_idx++].u.ps_name    = pt_running_svc_req->t_src_info.s_src_grp_name;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SVC_LST_ID;
            pt_cm_cmds[i4_idx++].u.ui2_number = pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SVL_REC_ID;
            pt_cm_cmds[i4_idx++].u.ui2_number = t_svl_rec.ui2_svl_rec_id;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_PAT_TIME_OUT;
            pt_cm_cmds[i4_idx++].u.ui4_number = DVB_PAT_QUERY_TIMEOUT;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_PMT_TIME_OUT;
            pt_cm_cmds[i4_idx++].u.ui4_number = DVB_PMT_QUERY_TIMEOUT;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_2_LANGUAGES_1_STREAM;
            pt_cm_cmds[i4_idx++].u.b_boolean  = FALSE; /* To have 2 DUAL_MONO streams when there are 2 languages */
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_N_LANGUAGES_1_STREAM;
            pt_cm_cmds[i4_idx++].u.b_boolean  = FALSE; /* To have n UNKNOWN streams when there are n languages (n > 2) */
#ifdef BRDCST_CONN_REUSE_CONNECTION
            if (pt_running_svc_req->pt_player->b_reuse_pipe == TRUE) {
                pt_cm_cmds[i4_idx].e_code       = BRDCST_CONN_REUSE_CONNECTION;
                pt_cm_cmds[i4_idx++].u.h_handle = player_get_conn_handle(pt_running_svc_req->pt_player);
            }
#endif
#if (defined (MW_TV_AV_BYPASS_SUPPORT) || defined (SYS_RECORD_SUPPORT))
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_TUNER_NO_RIGHTS_OK;
            pt_cm_cmds[i4_idx++].u.b_boolean  = TRUE;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SIGNAL_LOST_DISCONNECT;
            pt_cm_cmds[i4_idx++].u.b_boolean  = FALSE;
#endif
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_DUAL_MONO_FAKE_STEREO;
            pt_cm_cmds[i4_idx++].u.b_boolean  = pt_running_svc_req->t_src_info.u_info.t_svl.b_enable_fake_stereo;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SUBSTREAM_FAKE_STEREO;
            pt_cm_cmds[i4_idx++].u.b_boolean  = pt_running_svc_req->t_src_info.u_info.t_svl.b_substream_fake_stereo;
            /* For Riks TV */
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_ALTER_FREQ_TUNE;
            pt_cm_cmds[i4_idx++].u.b_boolean  = pt_running_svc_req->t_riks_tv.b_tune_alter_freq_if_loss_signal;

            /* Dirty solution for SHARP:DTV00333099. For Finland, the AD is
             * broadcaster-mixed, so tell CM to convert AD to Normal. */ 
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_AUDIO_AD_TO_INDEPENDENT_AD;
            pt_cm_cmds[i4_idx++].u.b_boolean  = pt_running_svc_req->t_src_info.u_info.t_svl.b_ad_to_independent_ad;

            pt_cm_cmds[i4_idx].e_code         = CONN_END;
            break;
#ifdef SYS_MHP_SUPPORT
        case BRDCST_TYPE_MHP:
            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_TYPE;
            pt_cm_cmds[i4_idx++].u.ps_name    = BRDCST_HANDLER_DEFAULT_NAME;
            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_NAME;
            pt_cm_cmds[i4_idx++].u.ps_name    = pt_running_svc_req->t_src_info.s_src_grp_name;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SVC_LST_ID;
            if (pt_running_svc_req->t_src_info.u_info.t_svl.t_silent.b_enable == TRUE)
            {
                pt_cm_cmds[i4_idx++].u.ui2_number = pt_running_svc_req->t_src_info.u_info.t_svl.t_silent.ui2_svc_lst_id;
            }
            else
            {
                pt_cm_cmds[i4_idx++].u.ui2_number = pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id;
            }
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SVL_REC_ID;
            pt_cm_cmds[i4_idx++].u.ui2_number = t_svl_rec.ui2_svl_rec_id;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_PAT_TIME_OUT;
            pt_cm_cmds[i4_idx++].u.ui4_number = 0;     /* Don't load PAT */
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_PMT_TIME_OUT;
            pt_cm_cmds[i4_idx++].u.ui4_number = 0;     /* Don't load PMT */
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_2_LANGUAGES_1_STREAM;
            pt_cm_cmds[i4_idx++].u.b_boolean  = FALSE; /* To have 2 DUAL_MONO streams when there are 2 languages */
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_N_LANGUAGES_1_STREAM;
            pt_cm_cmds[i4_idx++].u.b_boolean  = FALSE; /* To have n UNKNOWN streams when there are n languages (n > 2) */
#ifdef BRDCST_CONN_REUSE_CONNECTION
            if (pt_running_svc_req->pt_player->b_reuse_pipe == TRUE) {
                pt_cm_cmds[i4_idx].e_code       = BRDCST_CONN_REUSE_CONNECTION;
                pt_cm_cmds[i4_idx++].u.h_handle = player_get_conn_handle(pt_running_svc_req->pt_player);
            }
#endif
#if (defined (MW_TV_AV_BYPASS_SUPPORT) || defined (SYS_RECORD_SUPPORT))
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_TUNER_NO_RIGHTS_OK;
            pt_cm_cmds[i4_idx++].u.b_boolean  = TRUE;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SIGNAL_LOST_DISCONNECT;
            pt_cm_cmds[i4_idx++].u.b_boolean  = FALSE;
#endif
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_DUAL_MONO_FAKE_STEREO;
            pt_cm_cmds[i4_idx++].u.b_boolean  = pt_running_svc_req->t_src_info.u_info.t_svl.b_enable_fake_stereo;
            /* For Riks TV */
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_ALTER_FREQ_TUNE;
            pt_cm_cmds[i4_idx++].u.b_boolean  = pt_running_svc_req->t_riks_tv.b_tune_alter_freq_if_loss_signal;
            pt_cm_cmds[i4_idx].e_code         = CONN_END;
            break;
#endif
#ifdef MW_ISDB_SUPPORT
        case BRDCST_TYPE_ISDB:
            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_TYPE;
            pt_cm_cmds[i4_idx++].u.ps_name    = BRDCST_HANDLER_DEFAULT_NAME;
            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_NAME;
            pt_cm_cmds[i4_idx++].u.ps_name    = pt_running_svc_req->t_src_info.s_src_grp_name;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SVC_LST_ID;
            pt_cm_cmds[i4_idx++].u.ui2_number = pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SVL_REC_ID;
            pt_cm_cmds[i4_idx++].u.ui2_number = t_svl_rec.ui2_svl_rec_id;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_PAT_TIME_OUT;
            pt_cm_cmds[i4_idx++].u.ui4_number = DVB_PAT_QUERY_TIMEOUT;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_PMT_TIME_OUT;
            pt_cm_cmds[i4_idx++].u.ui4_number = DVB_PMT_QUERY_TIMEOUT;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_2_LANGUAGES_1_STREAM;
            pt_cm_cmds[i4_idx++].u.b_boolean  = FALSE; /* To have 2 DUAL_MONO streams when there are 2 languages */
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_N_LANGUAGES_1_STREAM;
            pt_cm_cmds[i4_idx++].u.b_boolean  = FALSE; /* To have n UNKNOWN streams when there are n languages (n > 2) */

#ifdef BRDCST_CONN_REUSE_CONNECTION
            if (pt_running_svc_req->pt_player->b_reuse_pipe == TRUE) {
                pt_cm_cmds[i4_idx].e_code       = BRDCST_CONN_REUSE_CONNECTION;
                pt_cm_cmds[i4_idx++].u.h_handle = player_get_conn_handle(pt_running_svc_req->pt_player);
            }
#endif
#if (defined (MW_TV_AV_BYPASS_SUPPORT) || defined (SYS_RECORD_SUPPORT))
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_TUNER_NO_RIGHTS_OK;
            pt_cm_cmds[i4_idx++].u.b_boolean  = TRUE;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SIGNAL_LOST_DISCONNECT;
            pt_cm_cmds[i4_idx++].u.b_boolean  = FALSE;
#endif
            pt_cm_cmds[i4_idx].e_code         = CONN_END;
            break;
#endif
        case BRDCST_TYPE_ANALOG:
            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_TYPE;
            pt_cm_cmds[i4_idx++].u.ps_name    = ANALOG_BRDCST_HANDLER_DEFAULT_NAME;
            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_NAME;
            pt_cm_cmds[i4_idx++].u.ps_name    = pt_running_svc_req->t_src_info.s_src_grp_name;
            pt_cm_cmds[i4_idx].e_code         = ANALOG_BRDCST_CONN_SVC_LST_ID;
            pt_cm_cmds[i4_idx++].u.ui2_number = pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id;
            pt_cm_cmds[i4_idx].e_code         = ANALOG_BRDCST_CONN_SVL_REC_ID;
            pt_cm_cmds[i4_idx++].u.ui2_number = t_svl_rec.ui2_svl_rec_id;
            pt_cm_cmds[i4_idx].e_code         = ANALOG_BRDCST_CONN_TV_DECODER_NAME;
            pt_cm_cmds[i4_idx++].u.ps_name    = pt_svctx->s_snk_grp_name;
            pt_cm_cmds[i4_idx].e_code         = ANALOG_BRDCST_CONN_AUDIO_DECODER_NAME;
            pt_cm_cmds[i4_idx++].u.ps_name    = pt_svctx->s_snk_grp_name;
            pt_cm_cmds[i4_idx].e_code         = ANALOG_BRDCST_CONN_DISC_IF_TV_SIG_LOST;
            pt_cm_cmds[i4_idx++].u.b_boolean  = FALSE;
            pt_cm_cmds[i4_idx].e_code         = ANALOG_BRDCST_CONN_DISC_IF_TUNER_SIG_LOST;
            pt_cm_cmds[i4_idx++].u.b_boolean  = FALSE;

#ifdef ANALOG_BRDCST_CONN_DETECT_TV_SYSTEM
            if (pt_running_svc_req->t_src_info.u_info.t_svl.b_detect_tv_sys == TRUE
#ifdef MW_TV_AV_BYPASS_SUPPORT
                && !IS_SVCTX_ASSOCIATE(pt_svctx)
#endif                
                ) {
                pt_cm_cmds[i4_idx].e_code         = ANALOG_BRDCST_CONN_DETECT_TV_SYSTEM;
                pt_cm_cmds[i4_idx++].u.b_boolean  = pt_running_svc_req->t_src_info.u_info.t_svl.b_detect_tv_sys;
            }
#endif

#if 0
            if (t_svl_rec.u_data.t_analog.ui4_tv_sys & TV_SYS_MASK_EU)
#endif
            {
                BOOL    b_fine_tune;

                pt_cm_cmds[i4_idx].e_code        = ANALOG_BRDCST_CONN_FINE_TUNE;
                if ((t_svl_rec.uheader.t_rec_hdr.ui4_nw_mask & SB_VNET_FREQ_EDITED) || /* For compatibility reason, will be removed later */
                    (t_svl_rec.uheader.t_rec_hdr.ui4_option_mask & SB_VOPT_FREQ_EDITED))
                {
                    b_fine_tune = FALSE;
                }
                else
                {
                    b_fine_tune = !t_svl_rec.u_data.t_analog.b_no_aft;
                }

                pt_cm_cmds[i4_idx++].u.b_boolean = b_fine_tune;
                pt_cm_cmds[i4_idx].e_code        = ANALOG_BRDCST_CONN_FINE_TUNE_OFFSET;
                pt_cm_cmds[i4_idx++].u.i2_number = t_svl_rec.u_data.t_analog.i2_aft_offset;
            }

#ifdef MW_TV_AV_BYPASS_SUPPORT
            if (0 == (pt_svctx->ui4_attribute & SVCTX_ASSC_ATTR_IGNORE_SCART))
#endif
            {
                if (pt_running_svc_req->t_src_info.u_info.t_svl.e_scart_mode != ANALOG_BRDCST_SCART_MODE_UNKNOWN) {
                    pt_cm_cmds[i4_idx].e_code        = ANALOG_BRDCST_CONN_USE_SCART;
#ifdef MW_TV_AV_BYPASS_SUPPORT
#ifdef MW_PIP_POP_SUPPORT
                    pt_cm_cmds[i4_idx++].u.b_boolean = !svctx_is_pap(pt_svctx);
#else
                    pt_cm_cmds[i4_idx++].u.b_boolean = TRUE;
#endif
#else
                    pt_cm_cmds[i4_idx++].u.b_boolean = FALSE;
#endif
                    pt_cm_cmds[i4_idx].e_code        = ANALOG_BRDCST_CONN_SCART_NAME;
                    pt_cm_cmds[i4_idx++].u.ps_name   = pt_running_svc_req->t_src_info.u_info.t_svl.t_ch_encode.s_scart_name;
                    pt_cm_cmds[i4_idx].e_code        = ANALOG_BRDCST_CONN_SCART_INP_MODE;
                    pt_cm_cmds[i4_idx++].u.i4_number = (INT32)(pt_running_svc_req->t_src_info.u_info.t_svl.e_scart_mode);
                }
            }
#ifdef MW_TV_AV_BYPASS_SUPPORT
            pt_cm_cmds[i4_idx].e_code         = ANALOG_BRDCST_CONN_ALL_NO_RIGHTS_OK;
            pt_cm_cmds[i4_idx++].u.b_boolean  = TRUE;
#endif

#ifdef MW_TV_AV_BYPASS_SUPPORT
            if (pt_svctx == svctx_mgr_get_link_lead(ACTIVE_LINK_NOW, pt_running_svc_req->t_parent.e_src_type))
            {
                if (t_svl_rec.u_data.t_analog.ui4_tv_sys & TV_SYS_MASK_AUTO)
                {
                    pt_cm_cmds[i4_idx].e_code     = ANALOG_BRDCST_CONN_CH_CHANGE_AUTO_DETECT_AUD_SYS;
                    pt_cm_cmds[i4_idx++].u.b_boolean = TRUE;
                }
            }
#endif
            pt_cm_cmds[i4_idx].e_code = CONN_END;
            break;
        case BRDCST_TYPE_FMRDO:
            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_TYPE;
            pt_cm_cmds[i4_idx++].u.ps_name    = FM_RADIO_HANDLER_DEFAULT_NAME;
            pt_cm_cmds[i4_idx].e_code         = FM_RADIO_CONN_SVC_LST_ID;
            pt_cm_cmds[i4_idx++].u.ui2_number = pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id;
            pt_cm_cmds[i4_idx].e_code         = FM_RADIO_CONN_SVL_REC_ID;
            pt_cm_cmds[i4_idx++].u.ui2_number = t_svl_rec.ui2_svl_rec_id;
            pt_cm_cmds[i4_idx].e_code = CONN_END;
            break;
        default:
            i4_ret = SVCTXR_FAIL;
            break;
        }
        break;
    }
    case SRC_TYPE_BRDCST_FREQ:
        switch (pt_running_svc_req->t_src_info.u_info.t_freq.e_type){
        case FREQ_INFO_TYPE_DIG_TER:
        {
            TUNER_TER_DIG_TUNE_INFO_T* pt_tune_info = &(pt_running_svc_req->t_src_info.u_info.t_freq.u_data.t_dig_ter);

            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_TYPE;
            pt_cm_cmds[i4_idx++].u.ps_name    = BRDCST_HANDLER_DEFAULT_NAME;
            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_NAME;
            pt_cm_cmds[i4_idx++].u.ps_name    = pt_running_svc_req->t_src_info.s_src_grp_name;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_DELIVERY_SYS;
            pt_cm_cmds[i4_idx++].u.ui4_number = TUNER_CONN_TYPE_TER_DIG;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_FREQUENCY;
            pt_cm_cmds[i4_idx++].u.ui4_number = pt_tune_info->ui4_freq;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_MODULATION;
            pt_cm_cmds[i4_idx++].u.ui4_number = pt_tune_info->e_mod;
            /* brdcst handler doesn't handle it well - weiderchang 10/5/2005
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_CODE_RATE_HP;
            pt_cm_cmds[i4_idx++].u.ui4_number = pt_tune_info->e_fec_outer;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_CODE_RATE_LP;
            pt_cm_cmds[i4_idx++].u.ui4_number = pt_tune_info->e_fec_inner;
            */
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_BANDWIDTH;
            pt_cm_cmds[i4_idx++].u.ui4_number = pt_tune_info->e_bandwidth;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_HIERARCHY;
            pt_cm_cmds[i4_idx++].u.ui4_number = pt_tune_info->e_hierarchy;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_GUARD_INTERVAL;
            pt_cm_cmds[i4_idx++].u.ui4_number = pt_tune_info->e_guard_int;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_TRANS_MODE;
            pt_cm_cmds[i4_idx++].u.ui4_number = pt_tune_info->e_trans_mode;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_OTHER_FREQ_FLAG;
            pt_cm_cmds[i4_idx++].u.b_boolean  = pt_tune_info->b_other_freq;
#ifdef MW_TV_AV_BYPASS_SUPPORT
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_TUNER_NO_RIGHTS_OK;
            pt_cm_cmds[i4_idx++].u.b_boolean  = TRUE;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SIGNAL_LOST_DISCONNECT;
            pt_cm_cmds[i4_idx++].u.b_boolean  = FALSE;
#endif
            if (svctx_rikstv_in_tuning_state(pt_svctx))
            {
                pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_PAT_TIME_OUT;
                pt_cm_cmds[i4_idx++].u.ui4_number = DVB_PAT_QUERY_TIMEOUT;
            }
            pt_cm_cmds[i4_idx].e_code         = CONN_END;

            pt_running_svc_req->e_crnt_tuner_type  = DEV_TUNER_TER_DIG;
            break;
        }
        case FREQ_INFO_TYPE_DIG_CAB:
        {
            TUNER_CAB_DIG_TUNE_INFO_T* pt_tune_info = &(pt_running_svc_req->t_src_info.u_info.t_freq.u_data.t_dig_cab);

            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_TYPE;
            pt_cm_cmds[i4_idx++].u.ps_name    = BRDCST_HANDLER_DEFAULT_NAME;
            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_NAME;
            pt_cm_cmds[i4_idx++].u.ps_name    = pt_running_svc_req->t_src_info.s_src_grp_name;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_DELIVERY_SYS;
            pt_cm_cmds[i4_idx++].u.ui4_number = TUNER_CONN_TYPE_CAB_DIG;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_FREQUENCY;
            pt_cm_cmds[i4_idx++].u.ui4_number = pt_tune_info->ui4_freq;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SYM_RATE;
            pt_cm_cmds[i4_idx++].u.ui4_number = pt_tune_info->ui4_sym_rate;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_MODULATION;
            pt_cm_cmds[i4_idx++].u.ui4_number = pt_tune_info->e_mod;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_FEC_OUTER;
            pt_cm_cmds[i4_idx++].u.ui4_number = pt_tune_info->e_fec_outer;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_FEC_INNER;
            pt_cm_cmds[i4_idx++].u.ui4_number = pt_tune_info->e_fec_inner;
#ifdef MW_TV_AV_BYPASS_SUPPORT
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_TUNER_NO_RIGHTS_OK;
            pt_cm_cmds[i4_idx++].u.b_boolean  = TRUE;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SIGNAL_LOST_DISCONNECT;
            pt_cm_cmds[i4_idx++].u.b_boolean  = FALSE;
#endif
            pt_cm_cmds[i4_idx].e_code         = CONN_END;

            pt_running_svc_req->e_crnt_tuner_type  = DEV_TUNER_CAB_DIG;
            break;
        }
        case FREQ_INFO_TYPE_DIG_SAT:
        {
            TUNER_SAT_DIG_TUNE_INFO_T* pt_tune_info = &(pt_running_svc_req->t_src_info.u_info.t_freq.u_data.t_dig_sat);

            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_TYPE;
            pt_cm_cmds[i4_idx++].u.ps_name    = BRDCST_HANDLER_DEFAULT_NAME;
            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_NAME;
            pt_cm_cmds[i4_idx++].u.ps_name    = pt_running_svc_req->t_src_info.s_src_grp_name;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_DELIVERY_SYS;
            pt_cm_cmds[i4_idx++].u.ui4_number = TUNER_CONN_TYPE_SAT_DIG;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_FREQUENCY;
            pt_cm_cmds[i4_idx++].u.ui4_number = pt_tune_info->ui4_freq;
#ifdef MW_DVBS_DEV_ENABLE            
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SATL_REC_ID;
            pt_cm_cmds[i4_idx++].u.ui2_number = pt_running_svc_req->t_src_info.u_info.t_freq.ui2_sat_lst_rec_id;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SAT_LST_ID;
            pt_cm_cmds[i4_idx++].u.ui2_number = pt_running_svc_req->t_src_info.u_info.t_freq.ui2_sat_lst_id;;
#else
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SYM_RATE;
            pt_cm_cmds[i4_idx++].u.ui4_number = pt_tune_info->ui4_sym_rate;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_ORBITAL_POSITION;
            pt_cm_cmds[i4_idx++].u.i2_number  = pt_tune_info->i2_orb_pos;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_MODULATION;
            pt_cm_cmds[i4_idx++].u.ui4_number = pt_tune_info->e_mod;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_FEC_INNER;
            pt_cm_cmds[i4_idx++].u.ui4_number = pt_tune_info->e_fec_inner;
#endif            
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_POLARIZATION;
            pt_cm_cmds[i4_idx++].u.ui4_number = pt_tune_info->e_pol;
#ifdef MW_TV_AV_BYPASS_SUPPORT
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_TUNER_NO_RIGHTS_OK;
            pt_cm_cmds[i4_idx++].u.b_boolean  = TRUE;
            pt_cm_cmds[i4_idx].e_code         = BRDCST_CONN_SIGNAL_LOST_DISCONNECT;
            pt_cm_cmds[i4_idx++].u.b_boolean  = FALSE;
#endif
            pt_cm_cmds[i4_idx].e_code         = CONN_END;

            pt_running_svc_req->e_crnt_tuner_type  = DEV_TUNER_SAT_DIG;
            break;
        }
        case FREQ_INFO_TYPE_ANA_TER:
        case FREQ_INFO_TYPE_FM_RADIO:
        {
            TUNER_TER_ANA_TUNE_INFO_T* pt_tune_info = &(pt_running_svc_req->t_src_info.u_info.t_freq.u_data.t_ana_ter);

            if (pt_running_svc_req->t_src_info.u_info.t_freq.e_type == FREQ_INFO_TYPE_ANA_TER)
            {
                pt_cm_cmds[i4_idx].e_code         = CONN_SRC_TYPE;
                pt_cm_cmds[i4_idx++].u.ps_name    = ANALOG_BRDCST_HANDLER_DEFAULT_NAME;
                pt_cm_cmds[i4_idx].e_code         = CONN_SRC_NAME;
                pt_cm_cmds[i4_idx++].u.ps_name    = pt_running_svc_req->t_src_info.s_src_grp_name;
                pt_cm_cmds[i4_idx].e_code         = ANALOG_BRDCST_CONN_DELIVERY_SYS;
                pt_cm_cmds[i4_idx++].u.ui4_number = TUNER_CONN_TYPE_TER_ANA;
                pt_cm_cmds[i4_idx].e_code         = ANALOG_BRDCST_CONN_FREQUENCY;
                pt_cm_cmds[i4_idx++].u.ui4_number = pt_tune_info->ui4_freq;
            }
            else
            {
                pt_cm_cmds[i4_idx].e_code         = CONN_SRC_TYPE;
                pt_cm_cmds[i4_idx++].u.ps_name    = FM_RADIO_HANDLER_DEFAULT_NAME;
                pt_cm_cmds[i4_idx].e_code         = FM_RADIO_CONN_DELIVERY_SYS;
                pt_cm_cmds[i4_idx++].u.ui4_number = TUNER_CONN_TYPE_TER_ANA;
                pt_cm_cmds[i4_idx].e_code         = FM_RADIO_CONN_FREQUENCY;
                pt_cm_cmds[i4_idx++].u.ui4_number = pt_tune_info->ui4_freq;
            }
            pt_cm_cmds[i4_idx].e_code         = CONN_END;

            pt_running_svc_req->e_crnt_tuner_type  = DEV_TUNER_TER_ANA;
            break;
        }
        case FREQ_INFO_TYPE_ANA_CAB:
        {
            TUNER_CAB_ANA_TUNE_INFO_T* pt_tune_info = &(pt_running_svc_req->t_src_info.u_info.t_freq.u_data.t_ana_cab);

            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_TYPE;
            pt_cm_cmds[i4_idx++].u.ps_name    = ANALOG_BRDCST_HANDLER_DEFAULT_NAME;
            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_NAME;
            pt_cm_cmds[i4_idx++].u.ps_name    = pt_running_svc_req->t_src_info.s_src_grp_name;
            pt_cm_cmds[i4_idx].e_code         = ANALOG_BRDCST_CONN_DELIVERY_SYS;
            pt_cm_cmds[i4_idx++].u.ui4_number = TUNER_CONN_TYPE_CAB_ANA;
            pt_cm_cmds[i4_idx].e_code         = ANALOG_BRDCST_CONN_FREQUENCY;
            pt_cm_cmds[i4_idx++].u.ui4_number = pt_tune_info->ui4_freq;
            pt_cm_cmds[i4_idx].e_code         = CONN_END;

            pt_running_svc_req->e_crnt_tuner_type  = DEV_TUNER_CAB_ANA;
            break;
        }
        case FREQ_INFO_TYPE_ANA_SAT:
        {
            TUNER_SAT_ANA_TUNE_INFO_T* pt_tune_info = &(pt_running_svc_req->t_src_info.u_info.t_freq.u_data.t_ana_sat);

            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_TYPE;
            pt_cm_cmds[i4_idx++].u.ps_name    = ANALOG_BRDCST_HANDLER_DEFAULT_NAME;
            pt_cm_cmds[i4_idx].e_code         = CONN_SRC_NAME;
            pt_cm_cmds[i4_idx++].u.ps_name    = pt_running_svc_req->t_src_info.s_src_grp_name;
            pt_cm_cmds[i4_idx].e_code         = ANALOG_BRDCST_CONN_DELIVERY_SYS;
            pt_cm_cmds[i4_idx++].u.ui4_number = TUNER_CONN_TYPE_CAB_ANA;
            pt_cm_cmds[i4_idx].e_code         = ANALOG_BRDCST_CONN_FREQUENCY;
            pt_cm_cmds[i4_idx++].u.ui4_number = pt_tune_info->ui4_freq;
            pt_cm_cmds[i4_idx].e_code         = CONN_END;

            pt_running_svc_req->e_crnt_tuner_type  = DEV_TUNER_SAT_ANA;
            break;
        }
        default:
            i4_ret = SVCTXR_FAIL;
            break;
        }
        break;
    default:
        i4_ret = SVCTXR_FAIL;
        break;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_handle_cm_info_ntfy
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_cm_info_ntfy(
                    SVCTX_T*                    pt_svctx,
                    UINT32                      ui4_data
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = NULL;

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_svctx is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }    

    switch(ui4_data) {
    case CM_CONN_INFO_LOST_TV_SIGNAL:
        ch_hdlr_check_signal_loss_priority(
                    pt_svctx,
                    pt_running_svc_req,
                    TRUE
                    );
        pt_svctx->e_signal_status = SIGNAL_STATUS_LOSS;
        if (pt_svctx->b_is_blocked) {
            ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOSS_AS_BLOCKED, ST_UNKNOWN);
        }
        break;
    case CM_CONN_INFO_GOT_TV_SIGNAL:
        svctx_cli_print_channel_change_log(pt_svctx, CH_XNG_STAGE_GOT_TV_SIGNAL);
        if (pt_svctx->b_in_fine_tuning == TRUE || pt_svctx->b_in_restoring_orig_freq == TRUE) {
            ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOCKED, ST_UNKNOWN);
            /* stop & play audio stream to avoid broken sound & no sound after fine-tuning*/
            ch_hdlr_reload_audio_stream(pt_svctx, pt_running_svc_req);
            ch_hdlr_reload_ad_stream(pt_svctx, pt_running_svc_req);
            pt_svctx->b_in_fine_tuning = FALSE;
        } else if (pt_svctx->b_is_blocked) {
            ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOCKED_AS_BLOCKED, ST_UNKNOWN);
        }
        break;
#ifdef CM_CONN_INFO_GOT_TUNER_SIGNAL
    case CM_CONN_INFO_GOT_TUNER_SIGNAL:
        if (pt_svctx->b_in_fine_tuning == TRUE || pt_svctx->b_in_restoring_orig_freq == TRUE) {
            /* stop & play audio stream to avoid broken sound & no sound after fine-tuning*/
            ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_TUNER_SIGNAL_LOCKED, ST_UNKNOWN);
            ch_hdlr_reload_audio_stream(pt_svctx, pt_running_svc_req);
            ch_hdlr_reload_ad_stream(pt_svctx, pt_running_svc_req);
        } else {
            ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_TUNER_SIGNAL_LOCKED, ST_UNKNOWN);
        }
        break;
#endif
#ifdef CM_CONN_INFO_LOST_TUNER_SIGNAL
    case CM_CONN_INFO_LOST_TUNER_SIGNAL:
        if (pt_svctx->b_in_fine_tuning == TRUE) {
            /* stop & play audio stream to avoid broken sound & no sound after fine-tuning*/
            ch_hdlr_reload_audio_stream(pt_svctx, pt_running_svc_req);
            ch_hdlr_reload_ad_stream(pt_svctx, pt_running_svc_req);
        }
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_TUNER_SIGNAL_LOSS, ST_UNKNOWN);
        break;
#endif
        /* Riks TV begin */
#ifdef CM_CONN_INFO_SIGNAL_LOST_TIMER_EXPIRED
    case CM_CONN_INFO_SIGNAL_LOST_TIMER_EXPIRED:
        svctx_rikstv_handle_signal_loss_check(pt_svctx);
        break;
#endif
        /* Riks TV end */
#ifdef CM_CONN_INFO_PAT_TS_ID_CHANGED
    case CM_CONN_INFO_PAT_TS_ID_CHANGED:
/*
        x_dbg_stmt("\n[SVCTX/ch_hdlr.c] CM_CONN_INFO_PAT_TS_ID_CHANGED received.\n\n");
*/
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_RF_SCAN_REQUIRED, ST_UNKNOWN);
        break;
#endif
#ifdef CM_CONN_INFO_TV_AUD_SYS_DETECTED
    case CM_CONN_INFO_TV_AUD_SYS_DETECTED:
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_AUD_SYS_DETECTED, ST_UNKNOWN);
        break;
#endif
#ifdef CM_CONN_INFO_START_ALTER_FREQ_TUNE
    case CM_CONN_INFO_START_ALTER_FREQ_TUNE:
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_START_ALTER_FREQ_TUNE, ST_UNKNOWN);
        break;
#endif
#ifdef CM_CONN_INFO_END_ALTER_FREQ_TUNE
    case CM_CONN_INFO_END_ALTER_FREQ_TUNE:
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_END_ALTER_FREQ_TUNE, ST_UNKNOWN);
        break;
#endif
#ifdef CM_CONN_INFO_START_FIRST_TUNE
    case CM_CONN_INFO_START_FIRST_TUNE:
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_START_FIRST_TUNE, ST_UNKNOWN);
        break;
#endif
#ifdef CM_CONN_INFO_END_FIRST_TUNE
    case CM_CONN_INFO_END_FIRST_TUNE:
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_END_FIRST_TUNE, ST_UNKNOWN);
        break;
#endif
#ifdef CM_CONN_INFO_START_FREQ_LIST_UPDATED_CHK
    case CM_CONN_INFO_START_FREQ_LIST_UPDATED_CHK:
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_START_FREQ_LIST_UPDATED_CHK, ST_UNKNOWN);
        break;
#endif
#ifdef CM_CONN_INFO_END_FREQ_LIST_UPDATED_CHK
    case CM_CONN_INFO_END_FREQ_LIST_UPDATED_CHK:
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_END_FREQ_LIST_UPDATED_CHK, ST_UNKNOWN);
        break;
#endif
#ifdef MW_ISDB_SUPPORT /* For ISDB/SBTVD standard */
#ifdef CM_CONN_INFO_EWS_ON
    case CM_CONN_INFO_EWS_ON:
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_EWS_ON, ST_UNKNOWN);
        break;
#endif
#ifdef CM_CONN_INFO_EWS_OFF
    case CM_CONN_INFO_EWS_OFF:
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_EWS_OFF, ST_UNKNOWN);
        break;
#endif
#ifdef CM_CONN_INFO_ISDB_BRDCST_SVC
    case CM_CONN_INFO_ISDB_BRDCST_SVC:
        ch_hdlr_check_non_brdcsting_svc(pt_svctx, TRUE);
        break;
#endif
#ifdef CM_CONN_INFO_ISDB_NON_BRDCST_SVC
    case CM_CONN_INFO_ISDB_NON_BRDCST_SVC:
        ch_hdlr_check_non_brdcsting_svc(pt_svctx, FALSE);
        break;
#endif
#ifdef CM_CONN_INFO_ISDB_UNKNOWN_BRDCST
    case CM_CONN_INFO_ISDB_UNKNOWN_BRDCST:
        do {
            SVL_REC_T t_svl_rec;
            BOOL      b_brdcsting = TRUE;

            /* get BRDCSTING flag from svl record */
            if(SVLR_OK == _ch_hdlr_get_svl_rec_by_req(pt_running_svc_req, &t_svl_rec))
            {
                /* set broadcasting type according to the optional mask */
                if((SB_VOPT_HAS_SMD & t_svl_rec.uheader.t_rec_hdr.ui4_option_mask)>0)
                {
                    b_brdcsting = (SB_VOPT_IS_BRDCSTING & t_svl_rec.uheader.t_rec_hdr.ui4_option_mask) > 0;
                }
                else
                {
#ifdef MW_ISDB_TREAT_NO_SMD_AS_BRDCSTING
                    b_brdcsting = TRUE;
#else
                    b_brdcsting = FALSE;
#endif
                }

                /* check block when broadcasting type changed */
                ch_hdlr_check_non_brdcsting_svc(pt_svctx, b_brdcsting);
            }
        } while(0);
        break;
#endif
#endif
    default:
        break;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_handle_player_stopped
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_player_stopped(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    CH_SVC_REQ_T* pt_pending_svc_req = NULL;
    INT32 i4_ret;

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_pending_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_pending_svc_req;

    i4_ret = strm_fmt_chker_close(pt_running_svc_req->pt_strm_fmt_chker);
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }
    pt_running_svc_req->pt_strm_fmt_chker = NULL;

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

    if (pt_running_svc_req->pt_scart_avm) {
        i4_ret = avm_stop(pt_running_svc_req->pt_scart_avm);
        if (i4_ret != SVCTXR_OK) {
            return i4_ret;
        }

        i4_ret = avm_close(pt_running_svc_req->pt_scart_avm);
        if (i4_ret != SVCTXR_OK) {
            return i4_ret;
        }
        pt_running_svc_req->pt_scart_avm = NULL;
    }

    if (pt_running_svc_req->pt_player) {
        if (pt_pending_svc_req == NULL) {
            pt_running_svc_req->pt_player->b_reuse_pipe = FALSE;
        }

        if (pt_running_svc_req->pt_player->b_reuse_pipe == FALSE) {
            i4_ret = player_close(pt_running_svc_req->pt_player);
            if (i4_ret != SVCTXR_OK) {
                return i4_ret;
            }
        } else {
            pt_pending_svc_req->pt_player = pt_running_svc_req->pt_player;
        }
        pt_running_svc_req->pt_player = NULL;
    }

    #ifdef TIME_MEASUREMENT
    TMS_END_EX(TMS_FLAG_CHG_CHL, "CLOSE_OLD_SESS", NULL);
    #endif

    svctx_ntfy_svc_req_stopped(pt_svctx);
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_handle_scart_avm_nty
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_scart_avm_nty(
                    AV_MONITOR_T*               pt_avm,
                    AVM_NTFY_CODE_T             e_code,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data
                    )
{
    SVCTX_T*      pt_svctx           = (SVCTX_T*) pv_tag;
    CH_SVC_REQ_T* pt_running_svc_req = NULL;
    INT32         i4_ret             = SVCTXR_OK;

    ASSERT(pt_svctx != NULL, ("%s(), pt_svctx is NULL",__func__));

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_svctx is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    switch(e_code) {
    case AVM_NTFY_CODE_VIDEO_SIGNAL_STATUS:
        if ((ui4_data & DSM_VIDEO_SIGNAL_ON) == DSM_VIDEO_SIGNAL_ON
             || (ui4_data & DSM_VIDEO_SIGNAL_OFF) == DSM_VIDEO_SIGNAL_OFF) {
            ANALOG_BRDCST_CTRL_SCART_BYPASS_T t_scart_bypass;
            SVL_REC_T                         t_svl_rec;

#ifdef MW_TV_AV_BYPASS_SUPPORT
            if (IS_SVCTX_ASSOCIATE(pt_svctx)) /* No need to check signal status for CANAL+ here */
            {
                break;
            }
#endif

            i4_ret = ch_hdlr_get_crnt_svl_rec(pt_running_svc_req, &t_svl_rec);
            if (i4_ret != SVCTXR_OK) {
                break;
            }

            if (t_svl_rec.uheader.t_rec_hdr.e_brdcst_type != BRDCST_TYPE_ANALOG) {
                break;
            }

            /* DSM_VIDEO_SIGNAL_ON bit is 1, assume it is from CANAL+ */
            t_scart_bypass.b_bypass = (ui4_data & DSM_VIDEO_SIGNAL_ON) ? FALSE : TRUE;
            if (pt_running_svc_req->t_src_info.u_info.t_svl.t_ch_encode.e_type == CHANNEL_DECODE_TYPE_MANUAL) {
                if ((t_svl_rec.uheader.t_rec_hdr.ui4_nw_mask & SB_VNET_USE_DECODER) == 0) {
                    break;
                }
            } else if (pt_running_svc_req->t_src_info.u_info.t_svl.t_ch_encode.e_type == CHANNEL_DECODE_TYPE_AUTO) {
                if (pt_svctx->e_signal_status == SIGNAL_STATUS_UNKNOWN
                        && (ui4_data & DSM_VIDEO_SIGNAL_ON) == DSM_VIDEO_SIGNAL_ON) {
                    t_scart_bypass.b_bypass = FALSE; /* PIN8 ON is from CANAL+ device*/
                } else {
                    t_scart_bypass.b_bypass = TRUE;  /* PIN8 ON/OFF is from NORMAL SCART device*/
                }
            } else {
                break;
            }

            i4_ret = ch_hdlr_set_conn_attr(
                            pt_svctx,
                            ST_VIDEO,
                            ANALOG_BRDCST_CTRL_SET_SCART_BYPASS,
                            &t_scart_bypass
                            );
        }
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
 *      _ch_hdlr_monitor_scart_signal
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_monitor_scart_signal (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    SVL_REC_T t_svl_rec;
    INT32     i4_ret;

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (IS_SVCTX_ASSOCIATE(pt_svctx))
    {
        return SVCTXR_OK;
    }
#endif

    /* if SB_VNET_USE_DECODER, auto scart_bypass */
    i4_ret = ch_hdlr_get_crnt_svl_rec(
                    pt_running_svc_req,
                    &t_svl_rec
                    );
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    if (t_svl_rec.uheader.t_rec_hdr.e_brdcst_type != BRDCST_TYPE_ANALOG) {
        return SVCTXR_OK;
    }

#ifdef MW_PIP_POP_SUPPORT
    if (svctx_is_pap(pt_svctx))
    {
        ch_hdlr_invoke_client_svc_nfy_fct(
                            pt_svctx,
                            SVCTX_NTFY_CODE_CH_SRC_FROM_TUNER,
                            ST_UNKNOWN
                            );
        return SVCTXR_OK;
    }
#endif

    switch(pt_running_svc_req->t_src_info.u_info.t_svl.t_ch_encode.e_type) {
    case CHANNEL_DECODE_TYPE_MANUAL:
        if ((t_svl_rec.uheader.t_rec_hdr.ui4_nw_mask & SB_VNET_USE_DECODER) == 0) {
            ANALOG_BRDCST_CTRL_SCART_BYPASS_T t_scart_bypass;

            t_scart_bypass.b_bypass = TRUE;

            i4_ret = ch_hdlr_set_conn_attr(
                        pt_svctx,
                        ST_VIDEO,
                        ANALOG_BRDCST_CTRL_SET_SCART_BYPASS,
                        &t_scart_bypass
                        );
            return i4_ret;
        }
        break;
    case CHANNEL_DECODE_TYPE_AUTO:
        break;
    default:
        return SVCTXR_OK;
    }

    if (pt_running_svc_req->pt_scart_avm == NULL) {
        i4_ret = avm_open(
                        pt_running_svc_req->t_src_info.u_info.t_svl.t_ch_encode.s_scart_name,
                        NULL,
                        DEV_AVC_SCART,
                        _ch_hdlr_handle_scart_avm_nty,
                        pt_svctx,
                        &pt_running_svc_req->pt_scart_avm
                        );
        if (i4_ret != SVCTXR_OK) {
            return i4_ret;
        }

        i4_ret = avm_start(pt_running_svc_req->pt_scart_avm);
        if (i4_ret != SVCTXR_OK) {
            return i4_ret;
        }
    }

    if (pt_running_svc_req->t_src_info.u_info.t_svl.t_ch_encode.e_type == CHANNEL_DECODE_TYPE_MANUAL) {
        /* if CHANNEL_DECODE_TYPE_MANUAL, get the current pin8 status to decide to enable SCART by pass
         * if CHANNEL_DECODE_TYPE_AUTO, should wait the pin8 from off to on to decide to enable SCART by pass
         */
        i4_ret = avm_notify_video_status(pt_running_svc_req->pt_scart_avm);
        if (i4_ret != SVCTXR_OK) {
            return i4_ret;
        }
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_handle_cm_connected
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_cm_connected(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    INT32 i4_ret = SVCTXR_OK;

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    if (pt_running_svc_req->e_brdcst_type != BRDCST_TYPE_ANALOG) {
        /* Since ANALOG_BRDCST_CONN_DISC_IF_TV_SIG_LOST
         * and ANALOG_BRDCST_CONN_DISC_IF_TUNER_SIG_LOST are all set to FALSE when
         * e_brdcst_type == ANALOG, so when the connection manager notify
         * the connection is connected doesn't mean the signal is locked.
         */
        svctx_cli_print_channel_change_log(pt_svctx,CH_XNG_STAGE_GOT_TV_SIGNAL);
        ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_SIGNAL_LOCKED,
                    ST_UNKNOWN
                    );
    }

    if ((pt_running_svc_req->t_parent.e_src_type == SRC_TYPE_BRDCST_FREQ)
        && (pt_running_svc_req->t_src_info.u_info.t_freq.e_type != FREQ_INFO_TYPE_FM_RADIO)) {
        return SVCTXR_DONE;
    }


#ifdef MW_ISDB_SUPPORT
    if (pt_svctx->b_is_brdcsting_svc == FALSE)
    {
        /* When unplug and plug cable in a non-broadcasting service,
         * b_is_blocked is reset to FALSE, making broadcasting state
         * inconsistent. Thus we make it consistent again.
         */
        ch_hdlr_check_non_brdcsting_svc(pt_svctx, FALSE);
    }
#endif

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if ((pt_running_svc_req->e_brdcst_type != BRDCST_TYPE_ANALOG) ||
        (BRDCST_TYPE_ANALOG == pt_running_svc_req->e_brdcst_type && FALSE == pt_running_svc_req->pt_crnt_event->b_in_requerying)) {
#else
    if (pt_running_svc_req->e_brdcst_type != BRDCST_TYPE_ANALOG) {
#endif
        CRNT_EVENT_KEY_T t_key;
        /* analog current event is from VBI, don't query at this point */
        t_key.t_svl.ui2_svc_lst_id = pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id;
        t_key.t_svl.ui2_svl_rec_id = pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svl_rec_id;
#ifdef MW_EDB_SUPPORT                
        t_key.t_svl.ui4_channel_id = pt_running_svc_req->t_src_info.u_info.t_svl.ui4_channel_id;
        t_key.t_svl.e_brdcst_type  = pt_running_svc_req->e_brdcst_type;
#endif

        i4_ret = crnt_evn_start_query(
                    pt_running_svc_req->pt_crnt_event,
                    DEV_TUNER,
                    pt_running_svc_req->t_src_info.s_src_grp_name,
                    player_get_conn_handle(pt_running_svc_req->pt_player),
                    &t_key
                    );
    } 
 
#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ANALOG) {
#else
    else {
#endif
        i4_ret = _ch_hdlr_monitor_scart_signal(
                    pt_svctx,
                    pt_running_svc_req
                    );
    }
        
     _ch_hdlr_is_blocked_service(
                    pt_svctx,
                    pt_running_svc_req,
                    SVC_BLOCK_CHECK_ALL,
                    TRUE
                    );
                    
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_reset_ca_info
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static VOID _ch_hdlr_reset_ca_info(
                    STRM_REQ_T*                 pt_strm_req
                    )
{
    if (pt_strm_req == NULL) return;
    pt_strm_req->t_ca.e_event = SM_EVN_IGNORE;
    pt_strm_req->t_ca.e_code  = POD_CA;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_reset_cp_info
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static VOID _ch_hdlr_reset_cp_info(
                    STRM_REQ_T*                 pt_strm_req
                    )
{
    if (pt_strm_req == NULL) return;
    pt_strm_req->t_cp.ui1_ctrl_size = 0;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_handle_sess_in_closing
 * Description
 * Input arguments
 *      pt_svctx            the pointer points to a SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK
 *      SVCTXR_PENDING
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_sess_in_closing(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    UINT8               ui1_idx;
    STREAM_TYPE_T       e_strm_type;
    STRM_REQ_T*         pt_strm_req;
    STRM_REQ_T**        ppt_strm_req;
    SVC_BLOCK_COND_T    e_block_cond;

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    for (ui1_idx = 0; ui1_idx < (UINT8) STRM_TYPE_COUNT_WITH_AD; ui1_idx++) {

        e_strm_type = e_g_ch_strm_types[ui1_idx];
        if ((e_strm_type == ST_UNKNOWN) || (e_strm_type >= STRM_TYPE_COUNT)) continue;

        ppt_strm_req = GET_PENDING_STRM_REQ_PPTR(pt_running_svc_req, (STRM_REQ_AD_INDEX == (INT32) ui1_idx), e_strm_type);

        if ((*ppt_strm_req) != NULL) {
            svctx_free_strm_req(*ppt_strm_req);
            *ppt_strm_req = NULL;
        }

        pt_strm_req = GET_ACTIVE_STRM_REQ(pt_running_svc_req, ui1_idx, e_strm_type);
        pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;

        _ch_hdlr_reset_ca_info(pt_strm_req);
        _ch_hdlr_reset_cp_info(pt_strm_req);

        if ((e_strm_type == ST_AUDIO) && (STRM_REQ_AD_INDEX != (INT32) ui1_idx)){
            ch_hdlr_reset_audio_info(pt_svctx, pt_running_svc_req);
        }

        if (e_strm_type == ST_VIDEO) {
            ch_hdlr_reset_video_info(pt_svctx, pt_running_svc_req);
        }
    }

    strm_fmt_chker_reset(pt_running_svc_req->pt_strm_fmt_chker);

    crnt_evn_stop_query(pt_running_svc_req->pt_crnt_event);
    avm_stop(pt_running_svc_req->pt_scart_avm);
    e_block_cond = rating_chker_get_block_cond(pt_running_svc_req->pt_rating_chker);

    switch(pt_svctx->e_block_rule) {
    case SVCTX_BLOCK_RULE_INP_SIGNAL_LOSS_CH_PROG:
        if (e_block_cond != SVC_BLOCK_COND_USER_BLOCK_INP) {
            rating_chker_set_block_cond(pt_running_svc_req->pt_rating_chker, SVC_BLOCK_COND_PASSED);
        }
        break;
    case SVCTX_BLOCK_RULE_INP_CH_SIGNAL_LOSS_PROG:
        if (e_block_cond != SVC_BLOCK_COND_USER_BLOCK_INP
             && e_block_cond != SVC_BLOCK_COND_USER_BLOCK_CH) {
            rating_chker_set_block_cond(pt_running_svc_req->pt_rating_chker, SVC_BLOCK_COND_PASSED);
        }
        break;
    case SVCTX_BLOCK_RULE_INP_CH_PROG_SIGNAL_LOSS:
        break;
    default:
        break;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_handle_sess_in_stopping
 * Description
 * Input arguments
 *      pt_svctx            the pointer points to a SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK
 *      SVCTXR_PENDING
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_sess_in_stopping(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }
    
    if (pt_running_svc_req->pt_strm_fmt_chker != NULL) {
        strm_fmt_chker_reset(pt_running_svc_req->pt_strm_fmt_chker);
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_strm_comp_pid_filter
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static BOOL _ch_hdlr_strm_comp_pid_filter (
                    UINT16                      ui2_num_recs,
                    UINT16                      ui2_idx,
                    const STREAM_COMP_ID_T*     pt_comp_id,
                    const SCDB_REC_T*           pt_scdb_rec,
                    VOID*                       pv_tag
                    )
{
    SCDB_REC_T* pt_old_scdb_rec = (SCDB_REC_T*) pv_tag;

    if (ui2_num_recs == ui2_idx) {
        return FALSE;
    }

    switch(pt_comp_id->e_type) {
    case ST_AUDIO:
        if ((pt_scdb_rec->u.t_audio_mpeg.ui2_pid   == pt_old_scdb_rec->u.t_audio_mpeg.ui2_pid) &&
            (pt_scdb_rec->u.t_audio_mpeg.ui1_index == pt_old_scdb_rec->u.t_audio_mpeg.ui1_index))
        {
            return TRUE;
        }
        break;
    case ST_VIDEO:
        if (pt_scdb_rec->u.t_video_mpeg.ui2_pid == pt_old_scdb_rec->u.t_video_mpeg.ui2_pid) {
            return TRUE;
        }
        break;
    case ST_SUBTITLE:
        if (pt_scdb_rec->u.t_dvb_sbtl.ui2_pid == pt_old_scdb_rec->u.t_dvb_sbtl.ui2_pid) {
            return TRUE;
        }
        break;
#ifdef MW_ISDB_SUPPORT
    case ST_ISDB_CAPTION:
        if (pt_scdb_rec->u.t_isdb_caption.ui2_pid == pt_old_scdb_rec->u.t_isdb_caption.ui2_pid) {
            return TRUE;
        }
        break;
    case ST_ISDB_TEXT:
        if (pt_scdb_rec->u.t_isdb_text.ui2_pid == pt_old_scdb_rec->u.t_isdb_text.ui2_pid) {
            return TRUE;
        }
        break;
#endif
    default:
        break;
    }

    return FALSE;
}

const CHAR* _ch_hdlr_get_lang_alias(const CHAR* s639_lang)
{
    static const CHAR* s_str_lang[] = {
        "hrv", "ces", "nld", "esl", "fra", "kat", "deu", "ell", "srp", "slk", "gae"};

    static const CHAR* s_str_alias[] = {
        "scr", "cze", "dut", "spa", "fre", "geo", "ger", "gre", "scc", "slo", "gla"};

    static INT32 s_i4_sz = (INT32) (sizeof(s_str_lang) / sizeof(s_str_lang[0]));

    INT32 i;

    for (i = 0; i < s_i4_sz; i++)
    {
        if (0 == x_strcmp(s639_lang, s_str_lang[i]))
        {
            return (s_str_alias[i]);
        }
    }

    return NULL;
}

/*-----------------------------------------------------------------------------
 * Sony Spec: The AD and main audio shall use the same coding standard and same
 * sampling rate. 
 * 1. The following code combination is allowed for dual pid decoding:
 * -------------------------------------------------------
 * |         Main           |        Sub(AD)             |
 * |------------------------|----------------------------|
 * |        MPEG            |        MPEG                |
 * |       E-AC3/AC3        |      E-AC3/AC3             |
 * |    AAC/HEAAC/HEAACv2   |    AAC/HEAAC/HEAACv2       |
 * -------------------------------------------------------
 * 2. The following codec combination is allowed for single pid decoding:
 * -------------------------------------------------------
 * |         Main           |        Sub(AD)             |
 * |------------------------|----------------------------|
 * |       E-AC3/AC3        |         E-AC3              |
 * -------------------------------------------------------
 *---------------------------------------------------------------------------*/
static BOOL _ch_hdlr_check_ad_param(
                    const SCDB_REC_T*  pt_ad_scdb_rec,   /* in */
                    const SCDB_REC_T*  pt_aud_scdb_rec   /* in */
                    )
{
    AUD_ENC_T e_ad_enc = pt_ad_scdb_rec->u.t_audio_mpeg.e_enc;
    AUD_ENC_T e_aud_enc = pt_aud_scdb_rec->u.t_audio_mpeg.e_enc;

    if (AUD_FMT_SUBSTREAM == pt_ad_scdb_rec->u.t_audio_mpeg.e_aud_fmt)
    {
        if ((e_aud_enc == AUD_ENC_AC3 || e_aud_enc == AUD_ENC_E_AC3)
            && e_ad_enc == AUD_ENC_E_AC3)
        {
            return TRUE;
        }
    }
    else
    {
        if ((e_aud_enc == AUD_ENC_MPEG_1 || e_aud_enc == AUD_ENC_MPEG_2)
            && (e_ad_enc == AUD_ENC_MPEG_1 || e_ad_enc == AUD_ENC_MPEG_2))
        {
            return TRUE;
        }
        else if ((e_aud_enc == AUD_ENC_AC3 || e_aud_enc == AUD_ENC_E_AC3)
                 && (e_ad_enc == AUD_ENC_AC3 || e_ad_enc == AUD_ENC_E_AC3))
        {
            return TRUE;
        }
        else if (e_aud_enc == AUD_ENC_AAC 
                 && e_ad_enc == AUD_ENC_AAC)
        {
            return TRUE;
        }
    }
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_strm_comp_ad_filter
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
BOOL ch_hdlr_strm_comp_ad_filter (
                    UINT16                      ui2_num_recs,
                    UINT16                      ui2_idx,
                    const STREAM_COMP_ID_T*     pt_comp_id,
                    const SCDB_REC_T*           pt_scdb_rec,
                    VOID*                       pv_tag
                    )
{
    SCDB_REC_T* pt_aud_scdb_rec = (SCDB_REC_T*) pv_tag;
    BOOL        b_lang_matched  = FALSE;

    b_lang_matched = ch_hdlr_strm_comp_ad_lang_filter(
            ui2_num_recs,
            ui2_idx,
            pt_comp_id,
            pt_scdb_rec,
            (VOID*)pt_aud_scdb_rec->u.t_audio_mpeg.s_lang
            );
    return (b_lang_matched && _ch_hdlr_check_ad_param(pt_scdb_rec, pt_aud_scdb_rec));
}

BOOL ch_hdlr_strm_comp_ad_lang_filter(
                    UINT16                      ui2_num_recs,
                    UINT16                      ui2_idx,       /* in */
                    const STREAM_COMP_ID_T*     pt_comp_id,    /* in */
                    const SCDB_REC_T*           pt_scdb_rec,   /* in */
                    VOID*                       pv_tag         /* in */
                    )
{
    ISO_639_LANG_T* pt_lang = (ISO_639_LANG_T*)pv_tag;
    const SCDB_AUDIO_MPEG_T* pt_audio_mpeg = &(pt_scdb_rec->u.t_audio_mpeg);

    if (ui2_num_recs == ui2_idx) {
        return FALSE;
    }

    if (ST_AUDIO != pt_comp_id->e_type) {
        return FALSE;
    }

    /* DONOT choose the independ ad to mix with other audio. */
    if (AUD_MIX_INDEPENDENT == pt_audio_mpeg->e_mix_type) {
        return FALSE;
    }

    if (AUD_TYPE_VISUAL_IMPAIRED == pt_audio_mpeg->e_type
        || AUD_CLASS_VISUAL_IMPAIRED_AD == pt_audio_mpeg->e_class)
    {
        CHAR    s_lang_scdb[ISO_639_LANG_LEN];
        CHAR    s_lang_usr[ISO_639_LANG_LEN];

        if (0 == x_strncmp((CHAR*)pt_lang, pt_audio_mpeg->s_lang, ISO_639_LANG_LEN))
        {
            return TRUE;
        }

        x_strncpy(s_lang_scdb, pt_audio_mpeg->s_lang, ISO_639_LANG_LEN);
        x_strncpy(s_lang_usr, (CHAR *)pt_lang, ISO_639_LANG_LEN);

        x_str_tolower(s_lang_scdb);
        x_str_tolower(s_lang_usr);

        if (0 == x_strncmp(s_lang_scdb, s_lang_usr, ISO_639_LANG_LEN))
        {
            return TRUE;
        }
        else
        {
            const CHAR* ps_alias = _ch_hdlr_get_lang_alias(s_lang_scdb);

            if (ps_alias &&
                (0 == x_strcmp(ps_alias, s_lang_usr)))
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_is_valid_for_strm_select
 * Description
 * Input arguments
 *      pt_svctx         the pointer points to an SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK       the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_is_valid_for_strm_select(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }
    
    if (svctx_cli_enable_strm_select(e_strm_type) == FALSE) {
        return SVCTXR_FAIL;
    }

    switch(e_strm_type) {
    case ST_VIDEO:
        if ((pt_running_svc_req->t_parent.ui4_stream_mode & ST_MASK_VIDEO) == 0) {
            if (pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ANALOG) {
                /* [specail case for analog audio only program]
                 * Even client set stream mode to without video, we still need to play
                 * video stream in background
                 */
                return SVCTXR_OK;
            }
        }
        break;
    case ST_AUDIO:
        if (pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ANALOG
                && pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED
                && (pt_svctx->ui1_stop_audio_as_no_signal & MAKE_BIT_MASK_8(SRC_TYPE_BRDCST_SVL)) != 0) {
            /* only play analog stream until TVD report signal locked */
            return SVCTXR_FAIL;
        }
        break;
    case ST_CLOSED_CAPTION:
    case ST_SUBTITLE:
    case ST_TELETEXT:
        if ((pt_running_svc_req->t_parent.ui4_stream_mode & MAKE_BIT_MASK_32(e_strm_type)) == 0) {
            return SVCTXR_FAIL;
        }

        if (x_handle_valid(pt_running_svc_req->h_gl_plane) == FALSE) {
            return SVCTXR_FAIL;
        }
        break;
#ifdef MW_ISDB_SUPPORT
    case ST_ISDB_CAPTION:
    case ST_ISDB_TEXT:
        if ((pt_running_svc_req->t_parent.ui4_stream_mode & MAKE_BIT_MASK_32(e_strm_type)) == 0) {
            return SVCTXR_FAIL;
        }
        break;
#endif
    default:
        return SVCTXR_FAIL;
    }

    if ((pt_running_svc_req->t_parent.ui4_stream_mode & MAKE_BIT_MASK_32(e_strm_type)) == 0) {
        return SVCTXR_FAIL;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_block_service
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_block_service (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    STRM_REQ_T* pt_audio_strm_req = NULL;
    INT32       i4_ret            = SVCTXR_OK;

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    svctx_set_av_status(pt_svctx, SVC_AV_STATUS_UNKNOWN);
    strm_fmt_chker_init_present_status(pt_running_svc_req->pt_strm_fmt_chker);

    pt_audio_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_AUDIO]);
    if (pt_audio_strm_req != NULL) {
        if (x_handle_valid(pt_audio_strm_req->h_stream) == TRUE) {
#ifdef MW_TV_AV_BYPASS_SUPPORT
            i4_ret = ch_hdlr_try_closing_stream(pt_svctx, pt_audio_strm_req->h_stream);
            if (SVCTXR_OK != i4_ret && SVCTXR_QUEUED != i4_ret) {
                return SVCTXR_FAIL;
            }
#else
            i4_ret = x_sm_close_stream(pt_audio_strm_req->h_stream);
            if (i4_ret != SMR_OK && i4_ret != SMR_PENDING) {
                return SVCTXR_FAIL;
            }
#endif

            pt_audio_strm_req->e_strm_cond = SM_COND_CLOSING;
        }
#ifdef MW_TV_AV_BYPASS_SUPPORT
        else
        {
            /* Remove queued audio open stream operation */
            i4_ret = svctx_mgr_remove_queued_strm_op_by_strm_type(
                        ACTIVE_LINK_NOW,
                        pt_running_svc_req->t_parent.e_src_type,
                        pt_svctx,
                        MASK_STRM_OP_OPEN,
                        ST_AUDIO);
            if (i4_ret != SVCTXR_OK) {
                return SVCTXR_FAIL;
            }

            pt_audio_strm_req->e_strm_cond = SM_COND_CLOSED;
        }
#endif

        if (pt_audio_strm_req->u.t_scdb.t_rec.e_rec_type == SCDB_REC_TYPE_AUDIO_MPEG) {
            pt_audio_strm_req->u.t_scdb.t_rec.u.t_audio_mpeg.ui2_pid = MPEG_2_NULL_PID;
        }
    }

    pt_audio_strm_req = &(pt_running_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX]);
    if (pt_svctx->b_ad_enabled && pt_audio_strm_req != NULL) {
        if (x_handle_valid(pt_audio_strm_req->h_stream) == TRUE) {
#ifdef MW_TV_AV_BYPASS_SUPPORT
            i4_ret = ch_hdlr_try_closing_stream(pt_svctx, pt_audio_strm_req->h_stream);
            if (SVCTXR_OK != i4_ret && SVCTXR_QUEUED != i4_ret) {
                return SVCTXR_FAIL;
            }
#else
            i4_ret = x_sm_close_stream(pt_audio_strm_req->h_stream);
            if (i4_ret != SMR_OK && i4_ret != SMR_PENDING) {
                return SVCTXR_FAIL;
            }
#endif

            pt_audio_strm_req->e_strm_cond = SM_COND_CLOSING;
        }
#ifdef MW_TV_AV_BYPASS_SUPPORT
        else
        {
            /* Remove queued audio open stream operation */
            i4_ret = svctx_mgr_remove_queued_strm_op_by_strm_type(
                        ACTIVE_LINK_NOW,
                        pt_running_svc_req->t_parent.e_src_type,
                        pt_svctx,
                        MASK_STRM_OP_OPEN,
                        ST_AUDIO);
            if (i4_ret != SVCTXR_OK) {
                return SVCTXR_FAIL;
            }

            pt_audio_strm_req->e_strm_cond = SM_COND_CLOSED;
        }
#endif

        if (pt_audio_strm_req->u.t_scdb.t_rec.e_rec_type == SCDB_REC_TYPE_AUDIO_MPEG) {
            pt_audio_strm_req->u.t_scdb.t_rec.u.t_audio_mpeg.ui2_pid = MPEG_2_NULL_PID;
        }
    }

#if 1
    _ch_hdlr_reload_video_stream_as_blocked(pt_svctx, pt_running_svc_req);

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
        i4_ret = _ch_hdlr_close_text_strms(
                    pt_svctx,
                    pt_running_svc_req,
#ifdef MW_ISDB_SUPPORT
                    ST_MASK_ISDB_CAPTION|ST_MASK_ISDB_TEXT|ST_MASK_SUBTITLE
#else
                    ST_MASK_SUBTITLE
#endif
                    );
    } else {
        i4_ret = _ch_hdlr_close_text_strms(
                    pt_svctx,
                    pt_running_svc_req,
#ifdef MW_ISDB_SUPPORT
                    ST_MASK_CLOSED_CAPTION|ST_MASK_ISDB_CAPTION|ST_MASK_ISDB_TEXT|ST_MASK_SUBTITLE
#else
                    ST_MASK_CLOSED_CAPTION|ST_MASK_SUBTITLE
#endif
                    );
    }
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_check_video_fmt
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_check_video_fmt (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    STRM_REQ_T* pt_video_strm_req = NULL;

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_video_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]);

    if (svctx_is_valid_video_fmt(pt_svctx) == TRUE) {
        ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE,
                    pt_video_strm_req
                    );
    }
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_check_video_stream
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_check_video_stream (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    STRM_REQ_T* pt_video_strm_req = NULL;

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_video_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]);

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if ((x_handle_valid(pt_video_strm_req->h_stream) == FALSE) &&
        !pt_video_strm_req->b_open_queued) {
#else
    if (x_handle_valid(pt_video_strm_req->h_stream) == FALSE) {
#endif
        pt_video_strm_req->u.t_scdb.t_comp_id.e_type = ST_VIDEO;
        ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_NO_STREAM,
                    pt_video_strm_req
                    );
        pt_video_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
        return SVCTXR_OK;
    }

    if (pt_video_strm_req->e_strm_cond == SM_COND_STARTED) {
        ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_STREAM_STARTED,
                    pt_video_strm_req
                    );
        if (pt_running_svc_req->e_brdcst_type != BRDCST_TYPE_ANALOG) {
            if (pt_video_strm_req->t_ca.e_event != SM_EVN_CA_COND_UNKNOWN) {
                ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CA,
                    pt_video_strm_req
                    );
            }
            ch_hdlr_invoke_client_strm_nfy_fct(
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
 *      _ch_hdlr_unblock_service
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_unblock_service (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    STRM_REQ_T* pt_audio_strm_req   = NULL;
    STRM_REQ_T* pt_video_strm_req   = NULL;
    STRM_REQ_T* pt_ad_strm_req      = NULL;
    INT32       i4_ret              = SVCTXR_OK;

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_audio_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_AUDIO]);
    pt_video_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]);
    pt_ad_strm_req = &(pt_running_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX]);    

    if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) {
        pt_svctx->b_signal_loss_notified = FALSE;
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOSS, ST_UNKNOWN);

        /* just for fix CR: DTV00342092: No audio output after unblock when no signal*/ 
        if((pt_svctx->ui1_stop_audio_as_no_signal & MAKE_BIT_MASK_8(pt_running_svc_req->t_parent.e_src_type))== 0)
        {
            /* Reload audio stream */
           if (x_handle_valid(pt_audio_strm_req->h_stream) == FALSE
                || pt_audio_strm_req->e_strm_cond != SM_COND_STARTED) {
                   ch_hdlr_reload_audio_stream(pt_svctx, pt_running_svc_req);
               }
        }
        return SVCTXR_OK;
    }

    svctx_set_av_status(pt_svctx, SVC_AV_STATUS_UNKNOWN);
    strm_fmt_chker_init_present_status(pt_running_svc_req->pt_strm_fmt_chker);

    /* Reload video stream */
    if (x_handle_valid(pt_video_strm_req->h_stream) == FALSE) {
        ch_hdlr_reload_video_stream(pt_svctx, pt_running_svc_req);
    } else {
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOCKED, ST_UNKNOWN);

        if (svctx_is_valid_video_fmt(pt_svctx) == FALSE) {
            svctx_update_video_info_from_sm(pt_svctx, pt_video_strm_req->h_stream);
        }

        _ch_hdlr_check_video_stream(pt_svctx, pt_running_svc_req);
        _ch_hdlr_video_strm_post_start_handler(pt_svctx, pt_running_svc_req);
    }

    if (x_handle_valid(pt_video_strm_req->h_stream) == FALSE) {
        pt_video_strm_req->u.t_scdb.t_comp_id.e_type = ST_VIDEO;
        ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_NO_STREAM,
                    pt_video_strm_req
                    );
        pt_video_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
    }

    /* Reload audio stream */
    if (x_handle_valid(pt_audio_strm_req->h_stream) == FALSE
                || pt_audio_strm_req->e_strm_cond != SM_COND_STARTED) {
        ch_hdlr_reload_audio_stream(pt_svctx, pt_running_svc_req);
    }

    if (x_handle_valid(pt_audio_strm_req->h_stream) == FALSE
                && pt_audio_strm_req->e_strm_cond == SM_COND_CLOSED) {
        pt_audio_strm_req->u.t_scdb.t_comp_id.e_type = ST_AUDIO;
        ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_NO_STREAM,
                    pt_audio_strm_req
                    );
        pt_audio_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
    }

    /* Reload ad stream */
    if (x_handle_valid(pt_ad_strm_req->h_stream) == FALSE
                        || pt_ad_strm_req->e_strm_cond != SM_COND_STARTED) {
        ch_hdlr_reload_ad_stream(pt_svctx, pt_running_svc_req);
    }

    if (x_handle_valid(pt_ad_strm_req->h_stream) == FALSE
                && pt_ad_strm_req->e_strm_cond == SM_COND_CLOSED) {
        pt_ad_strm_req->u.t_scdb.t_comp_id.e_type = ST_AUDIO;
        ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_NO_STREAM,
                    pt_ad_strm_req
                    );
        pt_ad_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
    }

    ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_NORMAL, ST_UNKNOWN);
    _ch_hdlr_check_video_fmt(pt_svctx, pt_running_svc_req);

    if (crnt_evn_is_ready(pt_running_svc_req->pt_crnt_event) == TRUE) {
        /* force to notify event is ready */
        ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CRNT_EVN_READY,
                    ST_UNKNOWN
                    );
    }

    rating_chker_stop_delay(pt_running_svc_req->pt_rating_chker);
    i4_ret = player_play(
                    pt_running_svc_req->pt_player,
                    pt_running_svc_req->t_parent.i4_speed
                    );
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_handle_scdb_delay_chk
 * Description
 *      This API is used to delay check SCDB record is empty or not,
 *      and it only check the stream type as it is empty as session is opened
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_scdb_delay_chk(
                    VOID*                   pv_tag,
                    SVCTX_MSG_T*            pt_msg
                    )
{
    SVCTX_T*      pt_svctx = (SVCTX_T*)pv_tag;
    CH_SVC_REQ_T* pt_running_svc_req = NULL;
    STREAM_TYPE_T e_strm_type;
    UINT8         ui1_idx;
    STRM_REQ_T*   pt_strm_req;
    UINT16        ui2_num_recs;
    INT32         i4_ret;

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_svctx is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req->b_in_scdb_empty_check_delay = FALSE;

    /* if empty comp condition is zero, ignore */
    if (pt_running_svc_req->ui4_empty_strm_comp_cond == 0) {
        return SVCTXR_OK;
    }

    for (ui1_idx = 0; ui1_idx < (UINT8) STRM_TYPE_COUNT; ui1_idx++) {
        e_strm_type = e_g_ch_strm_types[ui1_idx];
        if ((e_strm_type == ST_UNKNOWN) || (e_strm_type >= STRM_TYPE_COUNT)) continue;

        /* if the relative bit is empty as session is opened */
        if (pt_running_svc_req->ui4_empty_strm_comp_cond & MAKE_BIT_MASK_32(e_strm_type)) {
            ui2_num_recs = 0;
            i4_ret = ch_hdlr_get_scdb_num_recs(pt_svctx, e_strm_type, &ui2_num_recs);
            if (i4_ret != SVCTXR_OK) {
                continue;
            }

            pt_strm_req = GET_ACTIVE_STRM_REQ(pt_running_svc_req, ui1_idx, e_strm_type);
            if (ui2_num_recs == 0 && x_handle_valid(pt_strm_req->h_stream) == FALSE) {
                pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag = NULL;
                pt_strm_req->u.t_scdb.t_comp_id.e_type        = e_strm_type;
                ch_hdlr_invoke_client_strm_nfy_fct(
                            pt_svctx,
                            SVCTX_NTFY_CODE_NO_STREAM,
                            pt_strm_req
                            );
                pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
            } else {
                /* after the delay, exist SCDB record for this e_strm_type */
                pt_running_svc_req->ui4_exist_strm_comp_cond |= MAKE_BIT_MASK_32(e_strm_type);
            }
        }
    }

    if (pt_running_svc_req->ui4_exist_strm_comp_cond == 0) {
        ch_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_NO_ANY_STREAM,
                        ST_UNKNOWN
                        );
    }

    return  SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_delay_scdb_check_nfy_timeout_cb
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static VOID _ch_hdlr_delay_scdb_check_nfy_timeout_cb(
                    HANDLE_T                    h_timer,
                    VOID*                       pv_tag
                    )
{
    SVCTX_MSG_T t_msg    = {0};
    SVCTX_T*    pt_svctx = (SVCTX_T*)pv_tag;

    if (pt_svctx == NULL) {
        return;
    }

    t_msg.pf_invoke_func = _ch_hdlr_handle_scdb_delay_chk;
    t_msg.pv_tag         = pv_tag;

    svctx_send_msg(pt_svctx->h_sys_svctx, &t_msg);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_handle_sess_opened
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
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_sess_opened(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    INT32                        i4_ret;
    UINT8                        ui1_idx;
    STREAM_TYPE_T                e_strm_type;
    STRM_REQ_T*                  pt_strm_req;
    x_svctx_strm_comp_filter_fct pt_strm_fltr_fct;
    VOID*                        pv_strm_fltr_tag;
#ifdef SVCTX_RATING_BLOCK_DELAY
    SVC_BLOCK_COND_T             e_block_cond;
#endif
    BOOL                         b_prv_stream_reselect = FALSE;

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req->ui4_empty_strm_comp_cond    = 0;
    pt_running_svc_req->ui4_exist_strm_comp_cond    = 0;
    pt_running_svc_req->b_in_scdb_empty_check_delay = FALSE;

    for (ui1_idx = 0; ui1_idx < (UINT8) STRM_TYPE_COUNT_WITH_AD; ui1_idx++) {
         b_prv_stream_reselect = FALSE;
        if (STRM_REQ_AD_INDEX == (INT32) ui1_idx)
        {
          #ifdef MW_TV_AV_BYPASS_SUPPORT
            if ((!pt_svctx->b_ad_enabled) ||
                (!x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) &&
                 (!(SM_COND_OPENING == pt_running_svc_req->t_strm_reqs[ST_AUDIO].e_strm_cond) &&
                  pt_running_svc_req->t_strm_reqs[ST_AUDIO].b_open_queued)) ||
                ((STRM_REQ_SCDB == pt_running_svc_req->t_strm_reqs[ST_AUDIO].e_req_type) &&
                 (AUD_TYPE_VISUAL_IMPAIRED == pt_running_svc_req->t_strm_reqs[ST_AUDIO].u.t_scdb.t_rec.u.t_audio_mpeg.e_type)))
          #else
            if ((!pt_svctx->b_ad_enabled) ||
                ((!x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream))) ||
                 ((STRM_REQ_SCDB == pt_running_svc_req->t_strm_reqs[ST_AUDIO].e_req_type) &&
                  (AUD_TYPE_VISUAL_IMPAIRED == pt_running_svc_req->t_strm_reqs[ST_AUDIO].u.t_scdb.t_rec.u.t_audio_mpeg.e_type)))
          #endif
            {
                continue;
            }
            
            if (!ch_hdlr_is_valid_for_ad_select(pt_svctx, pt_running_svc_req))
            {
                continue;
            }                 
        }

        e_strm_type = e_g_ch_strm_types[ui1_idx];
        i4_ret = _ch_hdlr_is_valid_for_strm_select(
                    pt_svctx,
                    e_strm_type,
                    pt_running_svc_req
                    );
        if (i4_ret == SVCTXR_FAIL) {
            continue;
        }

        pt_strm_req = GET_ACTIVE_STRM_REQ(pt_running_svc_req, ui1_idx, e_strm_type);

        if (x_handle_valid(pt_strm_req->h_stream) == TRUE) {
            pt_strm_req->pf_strm_nfy     = pt_running_svc_req->t_parent.pf_svc_nfy;
            pt_strm_req->pv_strm_nfy_tag = pt_running_svc_req->t_parent.pv_svc_nfy_tag;
            pt_running_svc_req->ui4_exist_strm_comp_cond |= MAKE_BIT_MASK_32(e_strm_type);
            continue;
        }

        pt_strm_req->h_svctx         = pt_running_svc_req->t_parent.h_svctx;
        pt_strm_req->h_stream        = NULL_HANDLE;
        pt_strm_req->pf_strm_nfy     = pt_running_svc_req->t_parent.pf_svc_nfy;
        pt_strm_req->pv_strm_nfy_tag = pt_running_svc_req->t_parent.pv_svc_nfy_tag;

        i4_ret = SVCTXR_NOT_EXIST;

        if (pt_strm_req->e_req_type == STRM_REQ_SCDB) {
            if ((svctx_get_strm_req_strm_type(pt_strm_req) == e_strm_type) &&
                (e_strm_type != ST_CLOSED_CAPTION) &&
                (STRM_REQ_AD_INDEX != (INT32) ui1_idx))
            {
                /* select stream after signal loss */
                i4_ret = svctx_get_strm_comp_id(
                                pt_svctx,
                                e_strm_type,
                                FALSE,
                                _ch_hdlr_strm_comp_pid_filter,
                                &(pt_strm_req->u.t_scdb.t_rec),
                                &(pt_strm_req->u.t_scdb.t_comp_id),
                                &(pt_strm_req->u.t_scdb.t_rec)
                                );
            }

            if (i4_ret == SVCTXR_NOT_EXIST) {
                /* modified by Allen */
                if (STRM_REQ_AD_INDEX == (INT32) ui1_idx)
                {
                    pt_strm_fltr_fct = ch_hdlr_strm_comp_ad_filter;
                    pv_strm_fltr_tag = &(pt_running_svc_req->t_strm_reqs[ST_AUDIO].u.t_scdb.t_rec);
                }
                else
                {
                    ch_hdlr_choose_strm_filter(
                                    pt_svctx,
                                    e_strm_type,
                                    &pt_strm_fltr_fct,
                                    &pv_strm_fltr_tag
                                    );
                }

#ifdef MW_TV_AV_BYPASS_SUPPORT
                if (IS_SVCTX_ASSOCIATE(pt_svctx) && e_strm_type == ST_AUDIO && pt_svctx->e_prv_scrn_status != pt_svctx->e_scrn_status)
                {
                    SVCTX_T* pt_svctx_lead = svctx_mgr_get_link_lead(ACTIVE_LINK_NOW, pt_svctx->pt_running_svc_req->e_src_type);
                    if (pt_svctx_lead != NULL && pt_svctx_lead->i1_prio_diff > pt_svctx->i1_prio_diff)
                    {
                        pt_svctx->ui4_prv_stream_cond = pt_svctx_lead->ui4_prv_stream_cond;
                        pt_svctx->t_prv_aud_strm_req  = pt_svctx_lead->t_prv_aud_strm_req;
                    }
                }

                if ((e_strm_type == ST_AUDIO) &&
                    (pt_svctx->e_prv_scrn_status != pt_svctx->e_scrn_status) &&
                    ((pt_svctx->ui4_prv_stream_cond & MAKE_BIT_MASK_32(ST_AUDIO)) != 0) &&
                    (pt_svctx->t_prv_aud_strm_req.e_req_type == STRM_REQ_SCDB) &&
                    (STRM_REQ_AD_INDEX != (INT32) ui1_idx))
                {    
                    i4_ret = svctx_get_strm_comp_id(
                                    pt_svctx,
                                    e_strm_type,
                                    FALSE,
                                    _ch_hdlr_strm_comp_pid_filter,
                                    &(pt_svctx->t_prv_aud_strm_req.u.t_scdb.t_rec),
                                    &(pt_strm_req->u.t_scdb.t_comp_id),
                                    &(pt_strm_req->u.t_scdb.t_rec)
                                    );
                    if (i4_ret == SVCTXR_NOT_EXIST)
                    {
                        DBG_ERROR((_ERROR_HEADER"%s(), Previous audio stream not found after PIP/POP mode change\r\n", __func__));
                        i4_ret = svctx_get_strm_comp_id(
                                        pt_svctx,
                                        e_strm_type,
                                        FALSE,
                                        pt_strm_fltr_fct,
                                        pv_strm_fltr_tag,
                                        &(pt_strm_req->u.t_scdb.t_comp_id),
                                        &(pt_strm_req->u.t_scdb.t_rec)
                                        );
                    }
                    
                    if (IS_SVCTX_ASSOCIATE(pt_svctx))
                    {
                        pt_svctx->e_prv_scrn_status = SVCTX_SCREEN_STATUS_SINGLE;
                    }
                    /* just for fix CR 366741: from normal mode enter PIP and the MTS will restore:
                                         and the SCDB has not ready when connectd for MGT, just to reselect the stream when SCDB updated */
                    if (i4_ret == SVCTXR_NOT_EXIST){
                        b_prv_stream_reselect = TRUE;
                        pt_strm_req->u.t_scdb.t_rec = pt_svctx->t_prv_aud_strm_req.u.t_scdb.t_rec;
                        pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_AUDIO;
                    }
                  
                }                
                else
#endif                    
                {
                    i4_ret = svctx_get_strm_comp_id(
                                    pt_svctx,
                                    e_strm_type,
                                    FALSE,
                                    pt_strm_fltr_fct,
                                    pv_strm_fltr_tag,
                                    &(pt_strm_req->u.t_scdb.t_comp_id),
                                    &(pt_strm_req->u.t_scdb.t_rec)
                                    );
                }
            }

            if (i4_ret == SVCTXR_NOT_EXIST) {
                if (e_strm_type == ST_CLOSED_CAPTION) {
                    pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag = NULL;
                    pt_strm_req->u.t_scdb.t_comp_id.e_type        = ST_CLOSED_CAPTION;
                    i4_ret = SVCTXR_OK;
                } else if (!b_prv_stream_reselect){
                    /* in order to avoid the SCDB empty after CONNECTED issue */
                    pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag = NULL;
                    pt_strm_req->u.t_scdb.t_comp_id.e_type        = ST_UNKNOWN;
                    pt_running_svc_req->ui4_empty_strm_comp_cond |= MAKE_BIT_MASK_32(e_strm_type);
                    continue;
                }
            }
            if (i4_ret == SVCTXR_DB_MODIFIED) {
                /* scdb modified, search again! */
                ui1_idx = 0;
                continue;
            }

            if (i4_ret != SVCTXR_OK) {
                ch_hdlr_invoke_client_svc_nfy_fct(
                            pt_svctx,
                            SVCTX_NTFY_CODE_INTERNAL_ERROR,
                            e_strm_type
                            );
                return SVCTXR_FAIL;
            }
        } else if (pt_strm_req->e_req_type == STRM_REQ_PID) {
            /* by-pass to direct invoke ch_hdlr_open_strm */
        } else {
            continue;
        }

#ifdef MW_ISDB_SUPPORT
        if (e_strm_type == ST_CLOSED_CAPTION || e_strm_type == ST_SUBTITLE ||
            e_strm_type == ST_ISDB_CAPTION || e_strm_type == ST_ISDB_TEXT) {
#else
        if (e_strm_type == ST_CLOSED_CAPTION || e_strm_type == ST_SUBTITLE) {
#endif
            /* wait until video is played, then play cc */
            continue;
        }

        /* [MS10] if substream may be selected to mix, set the main audio's mix
         * flag for sequential process. */
        if (ST_AUDIO == e_strm_type /*&& pt_svctx->b_ad_enabled*/)
        {
            SCDB_REC_T         t_ad_scdb_rec = {0};
            STREAM_COMP_ID_T   t_comp_id;

            i4_ret = svctx_get_strm_comp_id(
                    pt_svctx,
                    e_strm_type,
                    FALSE,
                    ch_hdlr_strm_comp_ad_filter,
                    &(pt_running_svc_req->t_strm_reqs[ST_AUDIO].u.t_scdb.t_rec),
                    &t_comp_id,
                    &t_ad_scdb_rec
                    );
            if (SVCTXR_OK == i4_ret 
                && AUD_FMT_SUBSTREAM == t_ad_scdb_rec.u.t_audio_mpeg.e_aud_fmt)
            {
                pt_strm_req->b_mix_substrm = TRUE;
                pt_strm_req->ui1_mixed_substrm_idx = AUDIO_INDEX_UNKNOW;
                DBG_INFO((_INFO_HEADER"substream is a AD candidate, set the mix flag with sub's idx=AUDIO_INDEX_UNKNOW. \n"));
            }
        }

#ifdef MW_TV_AV_BYPASS_SUPPORT
        _ch_hdlr_check_stream_conflict(pt_svctx, pt_strm_req, e_strm_type, pt_running_svc_req);
#endif

        i4_ret = ch_hdlr_open_strm(pt_svctx, pt_strm_req, FALSE);
        if (i4_ret != SVCTXR_OK) {
            return i4_ret;
        }

        pt_running_svc_req->ui4_exist_strm_comp_cond |= MAKE_BIT_MASK_32(e_strm_type);
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    /* Record current stream request activity */
    pt_svctx->e_prv_scrn_status = pt_svctx->e_scrn_status;
#endif                    

    if (pt_running_svc_req->ui4_empty_strm_comp_cond != 0) {
        /* the svctx_scdb_chk_timer_start(), _ch_hdlr_delay_scdb_check_nfy_timeout_cb(),
         * and _ch_hdlr_handle_scdb_delay_chk() are used to handle the empty as connection is
         * connected. The mechanism is delay to check SCDB again after DEFAULT_SCDB_EMPTY_CHECK_DELAY ms.
         */
        pt_running_svc_req->b_in_scdb_empty_check_delay = TRUE;
        svctx_scdb_chk_timer_start(
                    pt_svctx,
                    DEFAULT_SCDB_EMPTY_CHECK_DELAY,
                    _ch_hdlr_delay_scdb_check_nfy_timeout_cb,
                    (VOID*)pt_svctx
                    );
    }

    if (_ch_hdlr_is_blocked_service(pt_svctx, pt_running_svc_req, SVC_BLOCK_CHECK_ALL, TRUE) == TRUE) {
        i4_ret = _ch_hdlr_block_service(pt_svctx, pt_running_svc_req);
        return i4_ret;
    }
#ifdef MW_ISDB_SUPPORT
    else if (pt_svctx->b_is_brdcsting_svc == FALSE)
    {
        return _ch_hdlr_block_service(pt_svctx, pt_running_svc_req);
    }
#endif

#ifdef SVCTX_RATING_BLOCK_DELAY
    e_block_cond = rating_chker_get_block_cond(pt_running_svc_req->pt_rating_chker);
    if (e_block_cond != SVC_BLOCK_COND_PASSED_AND_NO_RATING_SET)
    {
        static BOOL b_1st_sess_opened = TRUE;

        if (b_1st_sess_opened)
        {
            pt_running_svc_req->pt_rating_chker->ui4_block_delay = BOOT_UP_BLOCK_DELAY;
            i4_ret = rating_chker_start_delay(pt_running_svc_req->pt_rating_chker);
            pt_running_svc_req->pt_rating_chker->ui4_block_delay = DEFAULT_BLOCK_DELAY;
            b_1st_sess_opened = FALSE;
        }
        else if (pt_svctx->b_avc_to_tv_block_delay)
        {
            pt_running_svc_req->pt_rating_chker->ui4_block_delay = AVC_TO_TV_BLOCK_DELAY;
            i4_ret = rating_chker_start_delay(pt_running_svc_req->pt_rating_chker);
            pt_running_svc_req->pt_rating_chker->ui4_block_delay = DEFAULT_BLOCK_DELAY;
            pt_svctx->b_avc_to_tv_block_delay = FALSE;
        }        
        else
        {
            pt_running_svc_req->pt_rating_chker->ui4_block_delay = DEFAULT_BLOCK_DELAY;
            i4_ret = rating_chker_start_delay(pt_running_svc_req->pt_rating_chker);
        }
    } 
    else 
#endif
    {
        rating_chker_stop_delay(pt_running_svc_req->pt_rating_chker);
        i4_ret = player_play(
                    pt_running_svc_req->pt_player,
                    pt_running_svc_req->t_parent.i4_speed
                    );
    }
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_handle_sess_started
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_sess_started(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    if (pt_running_svc_req->e_brdcst_type != BRDCST_TYPE_ANALOG) {
        ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_NORMAL,
                    ST_UNKNOWN
                    );

        ch_hdlr_check_strms_status(pt_svctx);
    }

    TMS_END("SELECT_SVC_2_SESS_STARTED");

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_handle_strm_select_req
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_strm_select_req (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req,
                    STRM_REQ_T*                 pt_new_strm_req,
                    BOOL                        b_need_to_free_req
                    )
{
    INT32          i4_ret      = SVCTXR_OK;
    STRM_REQ_T*    pt_strm_req = NULL;
    STRM_REQ_T**   ppt_pending_strm_req = NULL;
    SM_SESS_COND_T e_sess_cond = SM_SESS_COND_CLOSED;
    STREAM_TYPE_T  e_strm_type = svctx_get_strm_req_strm_type(pt_new_strm_req);

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    if ((e_strm_type == ST_UNKNOWN) || (e_strm_type >= STRM_TYPE_COUNT))
    {
        return SVCTXR_INV_ARG;
    }

    /* should set this ui4_stream_mode before any check to retain
     * the stream mode setting.
     */
    pt_running_svc_req->t_parent.ui4_stream_mode |= MAKE_BIT_MASK_32(e_strm_type);

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (pt_svctx->b_svc_selected && (pt_running_svc_req->pt_player->e_sess_cond == SM_SESS_COND_STARTED))
#endif
    {
        if (!(e_strm_type == ST_CLOSED_CAPTION
            && SVCTX_IS_INVISIBLE_CC_FILTER(pt_new_strm_req->t_cc_filter)))
        {
            /* Signal unlock or channel block but require to open a new non-video stream
             * will skip the request */
            if (!(e_strm_type == ST_AUDIO && (pt_svctx->ui1_stop_audio_as_no_signal == 0)) &&
                (pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED ||
                 ((pt_svctx->b_is_blocked == TRUE) &&
                 (e_strm_type!=ST_VIDEO))))
            {
                if (b_need_to_free_req == TRUE)
                {
                    svctx_free_strm_req(pt_new_strm_req);
                }
                return SVCTXR_WRONG_STATE;
            }
        }
    }

#ifndef MW_TV_AV_BYPASS_SUPPORT
    e_sess_cond = player_get_sess_cond(pt_running_svc_req->pt_player);
    if (svctx_is_able_to_select_stream(e_sess_cond) == FALSE) {
        if (b_need_to_free_req == TRUE) {
            svctx_free_strm_req(pt_new_strm_req);
        }
        return SVCTXR_WRONG_STATE;
    }
#endif

    if (pt_new_strm_req->b_aux_audio)
    {
        pt_strm_req          = &(pt_running_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX]);
        ppt_pending_strm_req = &(pt_running_svc_req->pt_pending_strm_reqs[STRM_REQ_AD_INDEX]);
    }
    else
    {
        pt_strm_req          = &(pt_running_svc_req->t_strm_reqs[e_strm_type]);
        ppt_pending_strm_req = &(pt_running_svc_req->pt_pending_strm_reqs[e_strm_type]);
    }

    switch(e_strm_type){
    case ST_CLOSED_CAPTION:
        pt_running_svc_req->h_gl_plane  = pt_new_strm_req->h_gl_plane;
        pt_running_svc_req->t_cc_filter = pt_new_strm_req->t_cc_filter;
        break;
#ifdef MW_ISDB_SUPPORT
    case ST_ISDB_CAPTION:
    case ST_ISDB_TEXT:
        pt_running_svc_req->h_gl_plane     = pt_new_strm_req->h_gl_plane;
        pt_running_svc_req->ui4_isdbcc_idx = pt_new_strm_req->ui4_isdbcc_idx;
        break;
#endif
    case ST_SUBTITLE:
        pt_running_svc_req->h_gl_plane  = pt_new_strm_req->h_gl_plane;
        pt_running_svc_req->t_sbtl_info = pt_new_strm_req->t_sbtl_info;
        break;
    case ST_AUDIO:
        if (!pt_new_strm_req->b_aux_audio)
        {
            ch_hdlr_reset_audio_info(pt_svctx, pt_running_svc_req);
        }
        break;
    case ST_VIDEO:
        ch_hdlr_reset_video_info(pt_svctx, pt_running_svc_req);
        _ch_hdlr_close_text_strms(
                    pt_svctx,
                    pt_running_svc_req,
#ifdef MW_ISDB_SUPPORT
                    ST_MASK_CLOSED_CAPTION|ST_MASK_ISDB_CAPTION|ST_MASK_ISDB_TEXT|ST_MASK_SUBTITLE
#else
                    ST_MASK_CLOSED_CAPTION|ST_MASK_SUBTITLE
#endif
                    );
        break;
    default:
        break;
    }

    if (!(e_strm_type == ST_CLOSED_CAPTION
            && SVCTX_IS_INVISIBLE_CC_FILTER(pt_new_strm_req->t_cc_filter))) {
        if ((pt_svctx->ui4_frz_frozen_strm_mask & MAKE_BIT_MASK_32(e_strm_type)) > 0) {
            if (b_need_to_free_req == TRUE) {
                svctx_free_strm_req(pt_new_strm_req);
            }
            return SVCTXR_FROZEN;
        }
    }

    switch(pt_strm_req->e_strm_cond) {
    case SM_COND_CLOSED:
        *pt_strm_req = *pt_new_strm_req;
        i4_ret = ch_hdlr_open_strm(
                    pt_svctx,
                    pt_strm_req,
                    e_sess_cond == SM_SESS_COND_OPENED ? FALSE : TRUE
                    );
        if (i4_ret == SVCTXR_OK) {
            pt_strm_req->e_penging_status = STRM_PENDING_COND_SELECT_STRM;
        } else {
            pt_strm_req->e_req_type                = STRM_REQ_SCDB;
            pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
            pt_strm_req->e_penging_status          = STRM_PENDING_COND_NONE;
        }
        if (b_need_to_free_req == TRUE) {
            svctx_free_strm_req(pt_new_strm_req);
        }
        break;
    case SM_COND_OPENED:
    case SM_COND_STARTED:
        if (*ppt_pending_strm_req!= NULL) {
            svctx_free_strm_req(*ppt_pending_strm_req);
            *ppt_pending_strm_req = NULL;
        }

#ifdef MW_TV_AV_BYPASS_SUPPORT
        i4_ret = ch_hdlr_try_closing_stream(pt_svctx, pt_strm_req->h_stream);
        if (SVCTXR_OK == i4_ret || SVCTXR_QUEUED == i4_ret) {
#else
        i4_ret = x_sm_close_stream(pt_strm_req->h_stream);
        if (i4_ret == SMR_OK || i4_ret == SMR_PENDING) {
#endif
            pt_strm_req->e_penging_status = STRM_PENDING_COND_SELECT_STRM;
            pt_strm_req->e_strm_cond      = SM_COND_CLOSING;
            *ppt_pending_strm_req = pt_new_strm_req;
            i4_ret = SVCTXR_OK;
#ifdef MW_TV_AV_BYPASS_SUPPORT
        } else if (i4_ret == SVCTXR_INV_HANDLE) {
#else
        } else if (i4_ret == SMR_INV_HANDLE) {
#endif
            *pt_strm_req             = *pt_new_strm_req;
            pt_strm_req->e_strm_cond = SM_COND_CLOSED;
            i4_ret = ch_hdlr_open_strm(
                        pt_svctx,
                        pt_strm_req,
                        e_sess_cond == SM_SESS_COND_OPENED ? FALSE : TRUE
                        );
            if (i4_ret == SVCTXR_OK) {
                pt_strm_req->e_penging_status = STRM_PENDING_COND_SELECT_STRM;
            } else {
                pt_strm_req->e_req_type                = STRM_REQ_SCDB;
                pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
                pt_strm_req->e_penging_status          = STRM_PENDING_COND_NONE;
            }
            if (b_need_to_free_req == TRUE) {
                svctx_free_strm_req(pt_new_strm_req);
            }
        } else {
            if (b_need_to_free_req == TRUE) {
                svctx_free_strm_req(pt_new_strm_req);
            }
            i4_ret = SVCTXR_FAIL;
        }
        break;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    case SM_COND_OPENING:
        if (pt_strm_req->b_open_queued && *ppt_pending_strm_req == NULL)
        {
            pt_new_strm_req->e_strm_cond = SM_COND_OPENING;
            pt_new_strm_req->b_open_queued = TRUE;
            pt_new_strm_req->e_penging_status = STRM_PENDING_COND_SELECT_STRM;
    
            if (pt_new_strm_req->b_aux_audio)
            {
                pt_running_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX] = *pt_new_strm_req;
            }
            else
            {
                pt_running_svc_req->t_strm_reqs[e_strm_type] = *pt_new_strm_req;
            }
            if (b_need_to_free_req == TRUE) 
            {
                svctx_free_strm_req(pt_new_strm_req);
            }
            break;
        }
        else
        {
            /* don't break */
        }
#endif
    default:
        if (*ppt_pending_strm_req != NULL) {
            svctx_free_strm_req(*ppt_pending_strm_req);
            *ppt_pending_strm_req = NULL;
        }

        pt_strm_req->e_penging_status = STRM_PENDING_COND_SELECT_STRM;
        *ppt_pending_strm_req = pt_new_strm_req;
        break;
    }

    _ch_hdlr_reset_ca_info(pt_strm_req);
    _ch_hdlr_reset_cp_info(pt_strm_req);

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_create_new_strm_req_as_reselecting
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK       the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_create_new_strm_req_as_reselecting(
                    SVCTX_T*                     pt_svctx,
                    STREAM_TYPE_T                e_strm_type,
                    CH_SVC_REQ_T*                pt_running_svc_req,
                    STRM_REQ_T*                  pt_old_strm_req,
                    BOOL                         b_aux_audio,
                    STRM_REQ_T**                 ppt_new_strm_req
                    )
{
    x_svctx_strm_comp_filter_fct pt_strm_fltr_fct = NULL;
    VOID*                        pv_strm_fltr_tag = NULL;
    INT32                        i4_ret           = SVCTXR_NOT_EXIST;
    STRM_REQ_T*                  pt_new_strm_req  = NULL;
    SVCTX_MSG_T                  t_stop_msg       = {0};

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_new_strm_req = svctx_allocate_strm_req(pt_running_svc_req->t_parent.pt_hdlr);
    if (pt_new_strm_req == NULL) {
        return SVCTXR_NO_RESOURCE;
    }

    pt_new_strm_req->e_req_type  = STRM_REQ_SCDB;
    pt_new_strm_req->b_aux_audio = b_aux_audio;

    /* check cached PID value */
    if (!b_aux_audio)
    {
        if (pt_old_strm_req->e_req_type == STRM_REQ_SCDB
                && (pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ATSC ||
                    pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_SCTE
           /* just for fix CR DTV00352676*/
   #ifdef MW_ISDB_SUPPORT
                    ||(pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ISDB &&
                       e_strm_type == ST_AUDIO)
   #endif
                    )
                && svctx_get_strm_req_strm_type(pt_old_strm_req) == e_strm_type
                && e_strm_type != ST_CLOSED_CAPTION) {
            i4_ret = svctx_get_strm_comp_id(
                        pt_svctx,
                        e_strm_type,
                        FALSE,
                        _ch_hdlr_strm_comp_pid_filter,
                        &(pt_old_strm_req->u.t_scdb.t_rec),
                        &(pt_new_strm_req->u.t_scdb.t_comp_id),
                        &(pt_new_strm_req->u.t_scdb.t_rec)
                        );

            /* just for Fix CR:366741: MTS will restore when enter PIP */
            if((pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ATSC ||
                pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_SCTE) &&
                SVCTXR_OK == i4_ret && 
                e_strm_type == ST_AUDIO &&
                pt_old_strm_req->e_strm_cond == SM_COND_CLOSED){
                *ppt_new_strm_req = pt_new_strm_req;
                return SVCTXR_OK;
             }
                
            /* for CR DTV00329923: will black screen once after video play, but maybe 
                          have to reselect stream for Labwise Certification   */
            if(SVCTXR_OK == i4_ret)
            {
                svctx_free_strm_req(pt_new_strm_req);
                return SVCTXR_CONTINUE;
            }
        }
    }

    if (i4_ret != SVCTXR_OK) {
        /* get new matched SCDB record */
        if (b_aux_audio)
        {
            pt_strm_fltr_fct = ch_hdlr_strm_comp_ad_filter;
            pv_strm_fltr_tag = &(pt_old_strm_req->u.t_scdb.t_rec);
        }
        else
        {
            ch_hdlr_choose_strm_filter(
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

        if (SVCTXR_OK == i4_ret &&
            (pt_old_strm_req->e_strm_cond == SM_COND_OPENING ||
             pt_old_strm_req->e_strm_cond == SM_COND_OPENED ||
             pt_old_strm_req->e_strm_cond == SM_COND_STARTING ||/*for CR DTV00343731: no audio & no video sometimes*/
             pt_old_strm_req->e_strm_cond == SM_COND_STARTED) &&
            pt_old_strm_req->u.t_scdb.t_comp_id.e_type == e_strm_type)
        {
            if (ST_AUDIO == e_strm_type)
            {
                if ((pt_old_strm_req->u.t_scdb.t_rec.e_rec_type == SCDB_REC_TYPE_AUDIO_MPEG) &&
                    (pt_new_strm_req->u.t_scdb.t_rec.u.t_audio_mpeg.ui2_pid ==
                     pt_old_strm_req->u.t_scdb.t_rec.u.t_audio_mpeg.ui2_pid) &&
                    (pt_new_strm_req->u.t_scdb.t_rec.u.t_audio_mpeg.e_aud_fmt ==
                     pt_old_strm_req->u.t_scdb.t_rec.u.t_audio_mpeg.e_aud_fmt) &&
#if 0  /* Don't compare the e_enc value, 
          Strm_mngr will care such change and notify us via SM_EVN_AS_RESTART_REQ. */
                    (pt_new_strm_req->u.t_scdb.t_rec.u.t_audio_mpeg.e_enc ==
                     pt_old_strm_req->u.t_scdb.t_rec.u.t_audio_mpeg.e_enc) &&
#endif
                    (pt_new_strm_req->u.t_scdb.t_comp_id.pv_stream_tag == 
                     pt_old_strm_req->u.t_scdb.t_comp_id.pv_stream_tag)) 
                {
                /* If scdb updated event affects the AD, MUST restart the main
                 * audio so that the ad related setting can be updated.
                 */
                    SCDB_REC_T       t_new_ad_rec = {0};
                    STREAM_COMP_ID_T t_comp_id;
                    STRM_REQ_T*      pt_ad_req = &(pt_running_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX]);

                    i4_ret = svctx_get_strm_comp_id(
                            pt_svctx,
                            e_strm_type,
                            FALSE,
                            ch_hdlr_strm_comp_ad_filter,
                            &(pt_running_svc_req->t_strm_reqs[ST_AUDIO].u.t_scdb.t_rec),
                            &t_comp_id,
                            &t_new_ad_rec
                            );
                    if ((SVCTXR_OK == i4_ret 
                            && x_handle_valid(pt_ad_req->h_stream)
                            && t_new_ad_rec.u.t_audio_mpeg.ui2_pid == pt_ad_req->u.t_scdb.t_rec.u.t_audio_mpeg.ui2_pid)
                        ||(SVCTXR_OK != i4_ret 
                            && !x_handle_valid(pt_ad_req->h_stream)))
                    {
                        svctx_free_strm_req(pt_new_strm_req);
                        return SVCTXR_CONTINUE;
                    }
                }

                /* If main audio is to be restarted, stop the AD first. "New" AD
                 * will be opened after main audio started. */
                if (x_handle_valid(pt_running_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX].h_stream)
                    || (pt_old_strm_req->b_mix_substrm && pt_old_strm_req->ui1_mixed_substrm_idx != AUDIO_INDEX_UNKNOW))
                {
                    t_stop_msg.h_svctx                       = pt_old_strm_req->h_svctx;
                    t_stop_msg.ui1_msg_id                    = _SVCTX_MSG_STOP_STRM_REQ;
                    t_stop_msg.u.t_stop_strm_req.e_type      = e_strm_type;
                    t_stop_msg.u.t_stop_strm_req.b_aux_audio = TRUE;
                    ch_hdlr_stop_strm(pt_svctx, &t_stop_msg);
                    DBG_INFO((_INFO_HEADER"Stop the AD for scdb update.\n"));
                }
            }
            else if (ST_VIDEO == e_strm_type)
            {
                /* DONOT compare the e_enc(encoding) value here. SM will check it */
                if (pt_old_strm_req->u.t_scdb.t_rec.e_rec_type == SCDB_REC_TYPE_VIDEO_MPEG &&
                    pt_new_strm_req->u.t_scdb.t_rec.u.t_video_mpeg.ui2_pid ==
                    pt_old_strm_req->u.t_scdb.t_rec.u.t_video_mpeg.ui2_pid )
                {
                    svctx_free_strm_req(pt_new_strm_req);
                    DBG_INFO((_INFO_HEADER"[%s]Cancel reselecting video stream due to same pid.", pt_svctx->s_name));
                    return SVCTXR_CONTINUE;
                }
            }
            else if (ST_SUBTITLE == e_strm_type)
            {
                if (pt_old_strm_req->u.t_scdb.t_rec.e_rec_type == SCDB_REC_TYPE_SUBTITLE_DVB &&
                    pt_new_strm_req->u.t_scdb.t_rec.u.t_dvb_sbtl.ui2_pid ==
                    pt_old_strm_req->u.t_scdb.t_rec.u.t_dvb_sbtl.ui2_pid )
                {
                    svctx_free_strm_req(pt_new_strm_req);
                    return SVCTXR_CONTINUE;
                }
            }
#ifdef MW_ISDB_SUPPORT
            else if (ST_ISDB_CAPTION == e_strm_type)
            {
               if(pt_old_strm_req->u.t_scdb.t_rec.e_rec_type == SCDB_REC_TYPE_ISDB_CAPTION &&
                  pt_old_strm_req->u.t_scdb.t_rec.u.t_isdb_caption.ui2_pid ==
                  pt_new_strm_req->u.t_scdb.t_rec.u.t_isdb_caption.ui2_pid &&
                  pt_old_strm_req->u.t_scdb.t_rec.u.t_isdb_caption.ui1_caption_tag ==
                  pt_new_strm_req->u.t_scdb.t_rec.u.t_isdb_caption.ui1_caption_tag &&
                  pt_new_strm_req->u.t_scdb.t_comp_id.pv_stream_tag == 
                  pt_old_strm_req->u.t_scdb.t_comp_id.pv_stream_tag)
                {
                     svctx_free_strm_req(pt_new_strm_req);
                     return SVCTXR_CONTINUE;
                }
               else{  
                   pt_new_strm_req->h_gl_plane = pt_old_strm_req->h_gl_plane;
                }
            }
#endif
        }
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
            if (!b_aux_audio ||
                (x_handle_valid(pt_running_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX].h_stream)))
            {
                t_stop_msg.h_svctx                       = pt_old_strm_req->h_svctx;
                t_stop_msg.ui1_msg_id                    = _SVCTX_MSG_STOP_STRM_REQ;
                t_stop_msg.u.t_stop_strm_req.e_type      = e_strm_type;
                t_stop_msg.u.t_stop_strm_req.b_aux_audio = b_aux_audio;

                ch_hdlr_stop_strm(pt_svctx, &t_stop_msg);
                pt_running_svc_req->t_parent.ui4_stream_mode |= MAKE_BIT_MASK_32(e_strm_type);
            }
            break;
        }
    }

    *ppt_new_strm_req = pt_new_strm_req;
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_update_video_relative_strms
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK       the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_update_video_relative_strms(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req,
                    STRM_REQ_T*                 pt_new_strm_req
                    )
{
    STRM_REQ_T*   pt_video_strm_req = NULL;
    STRM_REQ_T*   pt_cc_strm_req    = NULL;
    STRM_REQ_T*   pt_sbtl_strm_req  = NULL;
    STREAM_TYPE_T e_strm_type       = svctx_get_strm_req_strm_type(pt_new_strm_req);
    INT32         i4_ret            = SVCTXR_OK;

#ifdef MW_ISDB_SUPPORT
    STRM_REQ_T*   pt_isdbcc_strm_req= NULL;
#endif

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_video_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]);
    pt_cc_strm_req    = &(pt_running_svc_req->t_strm_reqs[ST_CLOSED_CAPTION]);
    pt_sbtl_strm_req  = &(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE]);

#ifdef MW_ISDB_SUPPORT
    pt_isdbcc_strm_req= e_strm_type == ST_ISDB_CAPTION ?
            &(pt_running_svc_req->t_strm_reqs[ST_ISDB_CAPTION]) :
                ( e_strm_type == ST_ISDB_TEXT ?
                  &(pt_running_svc_req->t_strm_reqs[ST_ISDB_TEXT]) : NULL );
#endif

    switch (e_strm_type) {
    case ST_VIDEO:
        i4_ret = _ch_hdlr_close_text_strms(
                    pt_svctx,
                    pt_running_svc_req,
#ifdef MW_ISDB_SUPPORT
                    ST_MASK_CLOSED_CAPTION|ST_MASK_ISDB_CAPTION|ST_MASK_ISDB_TEXT|ST_MASK_SUBTITLE
#else
                    ST_MASK_CLOSED_CAPTION|ST_MASK_SUBTITLE
#endif
                    );
        break;
    case ST_CLOSED_CAPTION:
        if (pt_cc_strm_req == NULL) {
            break;
        }
        /* update CC comp_id & scdb_rec */
        pt_cc_strm_req->e_req_type = pt_new_strm_req->e_req_type;
        pt_cc_strm_req->u.t_scdb   = pt_new_strm_req->u.t_scdb;

        svctx_free_strm_req(pt_new_strm_req);
        if (pt_video_strm_req->e_strm_cond != SM_COND_STARTED) {
            /* video stream is not started, don't play CC */
            i4_ret = SVCTXR_NO_ACTION;
        }
        break;
#ifdef MW_ISDB_SUPPORT
    case ST_ISDB_CAPTION:
    case ST_ISDB_TEXT:
        if (pt_isdbcc_strm_req == NULL) {
            break;
        }

        pt_isdbcc_strm_req->e_req_type = pt_new_strm_req->e_req_type;
        pt_isdbcc_strm_req->u.t_scdb   = pt_new_strm_req->u.t_scdb;

        svctx_free_strm_req(pt_new_strm_req);
        if (pt_isdbcc_strm_req->e_strm_cond != SM_COND_STARTED) {
            /* video stream is not started, don't play ISDB CC */
            i4_ret = SVCTXR_NO_ACTION;
        }

        break;
#endif
    case ST_SUBTITLE:
        if (pt_sbtl_strm_req == NULL) {
            break;
        }

        pt_sbtl_strm_req->e_req_type = pt_new_strm_req->e_req_type;

        if (pt_new_strm_req->e_req_type == STRM_REQ_SCDB) {
            pt_sbtl_strm_req->u.t_scdb = pt_new_strm_req->u.t_scdb;
        } else if (pt_new_strm_req->e_req_type == STRM_REQ_PID) {
            pt_sbtl_strm_req->u.t_pid = pt_new_strm_req->u.t_pid;
        }

        svctx_free_strm_req(pt_new_strm_req);
        if (pt_video_strm_req->e_strm_cond != SM_COND_STARTED) {
            /* video stream is not started, don't play subtitle */
            i4_ret = SVCTXR_NO_ACTION;
        }
        break;
    default:
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_reselect_strms
 * Description
 * Input arguments
 *      pt_svctx        the pointer points to an SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK       the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_reselect_strms(
                    SVCTX_T*                    pt_svctx
                    )
{
    INT32            i4_ret             = SVCTXR_OK;
    UINT8            ui1_idx            = 0;
    STREAM_TYPE_T    e_strm_type        = ST_UNKNOWN;
    STRM_REQ_T*      pt_old_strm_req    = NULL;
    STRM_REQ_T*      pt_new_strm_req    = NULL;
    CH_SVC_REQ_T*    pt_running_svc_req = NULL;
    HANDLE_T         h_scdb             = NULL_HANDLE;
    SM_SESS_COND_T   e_sess_cond        = SM_SESS_COND_IGNORE;
    SVCTX_MSG_T      t_msg;

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_svctx is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    h_scdb       = player_get_scdb_handle(pt_running_svc_req->pt_player);
    e_sess_cond  = player_get_sess_cond(pt_running_svc_req->pt_player);

    for (ui1_idx = 0; ui1_idx < (UINT8) STRM_TYPE_COUNT_WITH_AD; ui1_idx++) {
        if (STRM_REQ_AD_INDEX == (INT32) ui1_idx) continue;
        
        e_strm_type = e_g_ch_strm_types[ui1_idx];
        i4_ret = _ch_hdlr_is_valid_for_strm_select(
                    pt_svctx,
                    e_strm_type,
                    pt_running_svc_req
                    );
        if (i4_ret == SVCTXR_FAIL) {
            continue;
        }

        pt_old_strm_req = &(pt_running_svc_req->t_strm_reqs[e_strm_type]);
        
        if (pt_old_strm_req->e_req_type != STRM_REQ_SCDB){
            continue;
        }

        if (pt_old_strm_req->u.t_scdb.t_comp_id.e_type == ST_CLOSED_CAPTION) {
            if (pt_old_strm_req->t_cc_filter.e_cc_type == SM_CC_TYPE_AUTO_SEL) {
                continue;
            }
        }
        
        /* For DTV00322908. For ATV, NOT to reload video stream if its scdb 
         * records not changed, or Analogue scan will fail because we stop 
         * the no_video_check_timer and will no signal loss notify sent for
         * pal-secam engine. 
         * * * Can be optimized later * * */
        if (pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ANALOG){
            i4_ret = _ch_hdlr_check_strm_scdb_record(
                        h_scdb,
                        e_strm_type,
                        pt_old_strm_req,
                        pt_running_svc_req
                        );
            if (i4_ret != SVCTXR_NO_RECORD) {
                continue;
            }
        }

        /* Labwise Certification: task945
         * When DTV, process the following logic also for the using SCDB recs 
         * which do NOT change. It is a chance to select another stream within 
         * one type when a new rec of this type added.   */
        i4_ret =_ch_hdlr_create_new_strm_req_as_reselecting(
                    pt_svctx,
                    e_strm_type,
                    pt_running_svc_req,
                    pt_old_strm_req,
                    FALSE,
                    &pt_new_strm_req
                    );
        if (i4_ret != SVCTXR_OK) {
            continue;
        }

        pt_new_strm_req->h_svctx         = pt_running_svc_req->t_parent.h_svctx;
        pt_new_strm_req->h_stream        = NULL_HANDLE;
        pt_new_strm_req->pf_strm_nfy     = pt_running_svc_req->t_parent.pf_svc_nfy;
        pt_new_strm_req->pv_strm_nfy_tag = pt_running_svc_req->t_parent.pv_svc_nfy_tag;

        i4_ret = _ch_hdlr_update_video_relative_strms(
                    pt_svctx,
                    pt_running_svc_req,
                    pt_new_strm_req
                    );
        if (i4_ret == SVCTXR_NO_ACTION) {
            continue;
        }

        switch (e_sess_cond) {
        case SM_SESS_COND_OPENED:
            if (e_strm_type == ST_VIDEO) {
                svctx_cancel_no_video_related_delay_nfy(pt_svctx);
            }
            *pt_old_strm_req = *pt_new_strm_req;
            svctx_free_strm_req(pt_new_strm_req);
            /* reslect stream when SM_SESS_COND_OPENED */
            i4_ret = ch_hdlr_open_strm(
                            pt_svctx,
                            pt_old_strm_req,
                            FALSE
                            );
            break;
        case SM_SESS_COND_STARTING:
        case SM_SESS_COND_STARTED:
            if (e_strm_type == ST_VIDEO) {
                svctx_cancel_no_video_related_delay_nfy(pt_svctx);
                if(pt_svctx->ui4_frz_frozen_strm_mask  != 0){ 
                    pt_svctx->ui4_frz_frozen_strm_mask = 0;
                    ch_hdlr_invoke_client_svc_nfy_fct(
                            pt_svctx,
                            SVCTX_NTFY_CODE_UNFREEZE,
                            ST_VIDEO
                            );
                }        
            }
            pt_new_strm_req->h_svctx         = pt_old_strm_req->h_svctx;
            pt_new_strm_req->pf_strm_nfy     = pt_old_strm_req->pf_strm_nfy;
            pt_new_strm_req->pv_strm_nfy_tag = pt_old_strm_req->pv_strm_nfy_tag;

            t_msg.h_svctx                    = pt_old_strm_req->h_svctx;
            t_msg.ui1_msg_id                 = _SVCTX_MSG_SELECT_STRM_REQ;
            t_msg.u.pt_strm_req              = pt_new_strm_req;

            i4_ret = ch_hdlr_select_strm(pt_svctx, &t_msg);
            break;
        default:
            svctx_free_strm_req(pt_new_strm_req);
            break;
        }
    }

    if (e_sess_cond != SM_SESS_COND_OPENED) {
        return SVCTXR_OK;
    }

    rating_chker_stop_delay(pt_running_svc_req->pt_rating_chker);
    i4_ret = player_play(
                    pt_running_svc_req->pt_player,
                    pt_running_svc_req->t_parent.i4_speed
                    );
    return i4_ret;
}

#ifdef MW_TV_AV_BYPASS_SUPPORT
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_create_new_strm_req_as_restoring
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK       the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_create_new_strm_req_as_restoring(
                    SVCTX_T*                     pt_svctx,
                    STREAM_TYPE_T                e_strm_type,
                    CH_SVC_REQ_T*                pt_running_svc_req,
                    STRM_REQ_T*                  pt_old_strm_req,
                    BOOL                         b_aux_audio,
                    STRM_REQ_T**                 ppt_new_strm_req
                    )
{
    x_svctx_strm_comp_filter_fct pt_strm_fltr_fct = NULL;
    VOID*                        pv_strm_fltr_tag = NULL;
    INT32                        i4_ret           = SVCTXR_NOT_EXIST;
    STRM_REQ_T*                  pt_new_strm_req  = NULL;
    SVCTX_MSG_T                  t_stop_msg       = {0};

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_new_strm_req = svctx_allocate_strm_req(pt_running_svc_req->t_parent.pt_hdlr);
    if (pt_new_strm_req == NULL) {
        return SVCTXR_NO_RESOURCE;
    }

    pt_new_strm_req->e_req_type  = STRM_REQ_SCDB;
    pt_new_strm_req->b_aux_audio = b_aux_audio;

    /* check cached PID value */
    if (!b_aux_audio)
    {
        if (pt_old_strm_req->e_req_type == STRM_REQ_SCDB
                && svctx_get_strm_req_strm_type(pt_old_strm_req) == e_strm_type
                && e_strm_type != ST_CLOSED_CAPTION) {
            i4_ret = svctx_get_strm_comp_id(
                        pt_svctx,
                        e_strm_type,
                        FALSE,
                        _ch_hdlr_strm_comp_pid_filter,
                        &(pt_old_strm_req->u.t_scdb.t_rec),
                        &(pt_new_strm_req->u.t_scdb.t_comp_id),
                        &(pt_new_strm_req->u.t_scdb.t_rec)
                        );
        }
    }

    if (i4_ret != SVCTXR_OK) {
        /* get new matched SCDB record */
        if (b_aux_audio)
        {
            pt_strm_fltr_fct = ch_hdlr_strm_comp_ad_filter;
            pv_strm_fltr_tag = &(pt_old_strm_req->u.t_scdb.t_rec);
        }
        else
        {
            ch_hdlr_choose_strm_filter(
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

        if (pt_old_strm_req->e_strm_cond == SM_COND_STARTED &&
            pt_old_strm_req->u.t_scdb.t_comp_id.e_type == e_strm_type)
        {
            if (ST_AUDIO == e_strm_type)
            {
                if ((pt_old_strm_req->u.t_scdb.t_rec.e_rec_type == SCDB_REC_TYPE_AUDIO_MPEG) &&
                    (pt_new_strm_req->u.t_scdb.t_rec.u.t_audio_mpeg.ui2_pid ==
                     pt_old_strm_req->u.t_scdb.t_rec.u.t_audio_mpeg.ui2_pid) &&
                    (pt_new_strm_req->u.t_scdb.t_rec.u.t_audio_mpeg.e_enc ==
                     pt_old_strm_req->u.t_scdb.t_rec.u.t_audio_mpeg.e_enc) &&
                    (pt_new_strm_req->u.t_scdb.t_comp_id.pv_stream_tag == 
                     pt_old_strm_req->u.t_scdb.t_comp_id.pv_stream_tag)) 
                {
                    svctx_free_strm_req(pt_new_strm_req);
                    return SVCTXR_CONTINUE;
                }
            }
            else if (ST_VIDEO == e_strm_type)
            {
                if (pt_old_strm_req->u.t_scdb.t_rec.e_rec_type == SCDB_REC_TYPE_VIDEO_MPEG &&
                    pt_new_strm_req->u.t_scdb.t_rec.u.t_video_mpeg.ui2_pid ==
                    pt_old_strm_req->u.t_scdb.t_rec.u.t_video_mpeg.ui2_pid )
                {
                    svctx_free_strm_req(pt_new_strm_req);
                    return SVCTXR_CONTINUE;
                }
            }
        }

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
            if (!b_aux_audio ||
                (x_handle_valid(pt_running_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX].h_stream)))
            {
                t_stop_msg.h_svctx                       = pt_old_strm_req->h_svctx;
                t_stop_msg.ui1_msg_id                    = _SVCTX_MSG_STOP_STRM_REQ;
                t_stop_msg.u.t_stop_strm_req.e_type      = e_strm_type;
                t_stop_msg.u.t_stop_strm_req.b_aux_audio = b_aux_audio;

                ch_hdlr_stop_strm(pt_svctx, &t_stop_msg);
                pt_running_svc_req->t_parent.ui4_stream_mode |= MAKE_BIT_MASK_32(e_strm_type);
            }
            break;
        }
    }

    *ppt_new_strm_req = pt_new_strm_req;
    return i4_ret;
}


static INT32 _ch_hdlr_restore_strms(
                    SVCTX_T*                    pt_svctx
                    )
{
    INT32            i4_ret;
    UINT8            ui1_idx;
    STREAM_TYPE_T    e_strm_type;
    STRM_REQ_T*      pt_old_strm_req    = NULL;
    STRM_REQ_T*      pt_new_strm_req    = NULL;
    CH_SVC_REQ_T*    pt_running_svc_req = NULL;
    SM_SESS_COND_T   e_sess_cond;
    SVCTX_MSG_T      t_msg;
    BOOL             b_aux_audio;

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_svctx is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    e_sess_cond  = player_get_sess_cond(pt_running_svc_req->pt_player);

    /*svctx_scdb_chk_timer_stop(pt_svctx);*/
    for (ui1_idx = 0; ui1_idx < STRM_TYPE_COUNT_WITH_AD; ui1_idx++) {
        e_strm_type = e_g_ch_strm_types[ui1_idx];
        i4_ret = _ch_hdlr_is_valid_for_strm_select(
                    pt_svctx,
                    e_strm_type,
                    pt_running_svc_req
                    );
        if (i4_ret == SVCTXR_FAIL) {
            if (e_strm_type == ST_VIDEO || e_strm_type == ST_AUDIO)
            {
                pt_old_strm_req =  &(pt_running_svc_req->t_strm_reqs[e_strm_type]);
                pt_old_strm_req->u.t_scdb.t_comp_id.e_type = e_strm_type;
                ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_NO_STREAM,
                    pt_old_strm_req
                    );
                pt_old_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
            }
            
            continue;
        }

        b_aux_audio = (STRM_REQ_AD_INDEX == (INT32) ui1_idx) ? TRUE : FALSE;

        if (b_aux_audio &&
            !ch_hdlr_is_valid_for_ad_select(pt_svctx, pt_running_svc_req))
        {
            continue;
        }
                    
        pt_old_strm_req =
            (b_aux_audio) ? &(pt_running_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX]) : &(pt_running_svc_req->t_strm_reqs[e_strm_type]);

        if (pt_old_strm_req->e_req_type != STRM_REQ_SCDB && pt_old_strm_req->e_req_type != STRM_REQ_PID){
            continue;
        }

        if (pt_old_strm_req->u.t_scdb.t_comp_id.e_type == ST_CLOSED_CAPTION) {
            if (pt_old_strm_req->t_cc_filter.e_cc_type == SM_CC_TYPE_AUTO_SEL) {
                continue;
            }
        }

        if (SM_COND_STARTED == pt_old_strm_req->e_strm_cond)
        {
            /* still playing, no need to restore */
            DBG_SVCTXMGR_MSG((
                "{SVCTX_MGR} %s stream started, no need to restore (%s).\r\n",
                s_g_stream[e_strm_type],
                pt_svctx->s_name));
            continue;
        }

        if ((SM_COND_CLOSING == pt_old_strm_req->e_strm_cond) &&
            (pt_old_strm_req->b_close_queued))
        {
            DBG_SVCTXMGR_MSG((
                "{SVCTX_MGR} %s stream closing queued, no need to restore (%s).\r\n",
                s_g_stream[e_strm_type],
                pt_svctx->s_name));

            /* remove any queued stream close operation */
            svctx_mgr_remove_queued_strm_op_by_strm_type(
                ACTIVE_LINK_NOW,
                pt_running_svc_req->t_parent.e_src_type,
                pt_svctx,
                MASK_STRM_OP_CLOSE,
                e_strm_type);

            pt_old_strm_req->e_strm_cond = SM_COND_STARTED;
            pt_old_strm_req->b_close_queued = FALSE;
            continue;
        }

        if ((SM_COND_CLOSED != pt_old_strm_req->e_strm_cond) &&
            (pt_old_strm_req->b_close_queued || pt_old_strm_req->b_open_queued))
        {
            DBG_SVCTXMGR_MSG((
                "{SVCTX_MGR} %s stream closing processed (%s)...\r\n",
                s_g_stream[e_strm_type],
                pt_svctx->s_name));

            /* remove any queued stream operation */
            svctx_mgr_remove_queued_strm_op_by_strm_type(
                ACTIVE_LINK_NOW,
                pt_running_svc_req->t_parent.e_src_type,
                pt_svctx,
                MASK_STRM_OP_CLOSE | MASK_STRM_OP_OPEN,
                e_strm_type);

            pt_old_strm_req->e_strm_cond = SM_COND_CLOSED;
            pt_old_strm_req->b_close_queued = FALSE;
            pt_old_strm_req->b_open_queued  = FALSE;
        }

        /* only process the following logic when SCDB record is absent */
        i4_ret =_ch_hdlr_create_new_strm_req_as_restoring(
                    pt_svctx,
                    e_strm_type,
                    pt_running_svc_req,
                    pt_old_strm_req,
                    b_aux_audio,
                    &pt_new_strm_req
                    );
        if (i4_ret != SVCTXR_OK) {
            continue;
        }

        pt_new_strm_req->h_svctx  = pt_running_svc_req->t_parent.h_svctx;
        pt_new_strm_req->h_stream = NULL_HANDLE;

        if (b_aux_audio)
        {
            pt_new_strm_req->pf_strm_nfy     = NULL;
            pt_new_strm_req->pv_strm_nfy_tag = NULL;
        }
        else
        {
            pt_new_strm_req->pf_strm_nfy     = pt_running_svc_req->t_parent.pf_svc_nfy;
            pt_new_strm_req->pv_strm_nfy_tag = pt_running_svc_req->t_parent.pv_svc_nfy_tag;
        }

       /* [MS10] for substream AD support */
        if (e_strm_type == ST_AUDIO)
        {
            pt_new_strm_req->b_mix_substrm = pt_old_strm_req->b_mix_substrm;
            pt_new_strm_req->ui1_mixed_substrm_idx = pt_old_strm_req->ui1_mixed_substrm_idx;
        }

        i4_ret = _ch_hdlr_update_video_relative_strms(
                    pt_svctx,
                    pt_running_svc_req,
                    pt_new_strm_req
                    );
        if (i4_ret == SVCTXR_NO_ACTION) {
            continue;
        }

        switch (e_sess_cond) {
        case SM_SESS_COND_OPENED:
            if (e_strm_type == ST_VIDEO) {
                svctx_cancel_no_video_related_delay_nfy(pt_svctx);
            }
            *pt_old_strm_req = *pt_new_strm_req;
            svctx_free_strm_req(pt_new_strm_req);

            DBG_SVCTXMGR_MSG((
                "{SVCTX_MGR} Opening %s stream (%s)...\r\n",
                s_g_stream[e_strm_type],
                pt_svctx->s_name));

            /* reslect stream when SM_SESS_COND_OPENED */
            i4_ret = ch_hdlr_open_strm(
                            pt_svctx,
                            pt_old_strm_req,
                            FALSE
                            );
            break;
        case SM_SESS_COND_STARTING:
        case SM_SESS_COND_STARTED:
            if (e_strm_type == ST_VIDEO) {
                svctx_cancel_no_video_related_delay_nfy(pt_svctx);
            }
            pt_new_strm_req->h_svctx         = pt_old_strm_req->h_svctx;
            pt_new_strm_req->pf_strm_nfy     = pt_old_strm_req->pf_strm_nfy;
            pt_new_strm_req->pv_strm_nfy_tag = pt_old_strm_req->pv_strm_nfy_tag;

            t_msg.h_svctx                    = pt_old_strm_req->h_svctx;
            t_msg.ui1_msg_id                 = _SVCTX_MSG_SELECT_STRM_REQ;
            t_msg.u.pt_strm_req              = pt_new_strm_req;

            DBG_SVCTXMGR_MSG((
                "{SVCTX_MGR} Opening %s stream (%s)...\r\n",
                s_g_stream[e_strm_type],
                pt_svctx->s_name));

            i4_ret = ch_hdlr_select_strm(pt_svctx, &t_msg);
            break;
        default:
            svctx_free_strm_req(pt_new_strm_req);
            break;
        }
    }

    /* Alec-013109: Added for resolving DTV00132044 */
#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (!IS_SVCTX_ASSOCIATE(pt_svctx))
#endif
    {
        if ((x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) != TRUE) &&
            (pt_running_svc_req->t_strm_reqs[ST_AUDIO].e_penging_status != STRM_PENDING_COND_SELECT_STRM) &&
            (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream) != TRUE) &&
            (pt_running_svc_req->t_strm_reqs[ST_VIDEO].e_penging_status != STRM_PENDING_COND_SELECT_STRM))
        {
            ch_hdlr_check_strms_status(pt_svctx);
        }
    }

    if (e_sess_cond != SM_SESS_COND_OPENED) {
        return SVCTXR_OK;
    }

    rating_chker_stop_delay(pt_running_svc_req->pt_rating_chker);
    i4_ret = player_play(
                    pt_running_svc_req->pt_player,
                    pt_running_svc_req->t_parent.i4_speed
                    );
    return i4_ret;
}
#endif

/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_handle_scdb_updated
 * Description
 *      The API is used to open a pipe.
 * Input arguments
 *      pt_svctx            the pointer points to a SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK         the routine is successful
 *      SVCTXR_FAIL       the routine is failed
 *-----------------------------------------------------------------------------*/
 static INT32 _ch_hdlr_handle_scdb_updated(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req,
                    UINT32                      ui4_reason
                    )
{
    STREAM_COMP_ID_T t_comp_id;
    INT32            i4_ret         = SVCTXR_OK;
    STRM_REQ_T*      pt_cc_strm_req = NULL;
    SM_SESS_COND_T   e_sess_cond;

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    if (ui4_reason == SCDB_REASON_UNK) {
        /* Don't care */
        return SVCTXR_OK;
    }

#if 1  /* for MW_NINJA_CHANGE_CHANNEL */
    if (pt_running_svc_req->t_src_info.b_ninja_change && pt_running_svc_req->pt_player->b_ninja_change)
    {
        return SVCTXR_OK;
    }
#endif

    pt_cc_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_CLOSED_CAPTION]);

    e_sess_cond = player_get_sess_cond(pt_running_svc_req->pt_player);
    if (e_sess_cond != SM_SESS_COND_OPENED
            && e_sess_cond != SM_SESS_COND_STARTING
            && e_sess_cond != SM_SESS_COND_STARTED) {
        /* No streams opened yet, so ignore it. */
        return SVCTXR_OK;
    }

    /* For DTV00324698. If the video's scdb record changes, we have to
     * reload the video even if the channel blocked.  */
    if (pt_svctx->b_is_blocked 
            && pt_running_svc_req->e_brdcst_type != BRDCST_TYPE_ANALOG) {
        ch_hdlr_reload_video_stream(pt_svctx, pt_running_svc_req);
        return SVCTXR_OK;
    }

    if (pt_cc_strm_req != NULL
            && pt_cc_strm_req->e_req_type == STRM_REQ_SCDB
            && pt_running_svc_req->t_cc_filter.e_cc_type != SM_CC_TYPE_IGNORE) {
        i4_ret = svctx_get_strm_comp_id(
                        pt_svctx,
                        ST_CLOSED_CAPTION,
                        TRUE,
                        svctx_strm_comp_cc_filter,
                        (VOID*)(&pt_running_svc_req->t_cc_filter),
                        &t_comp_id,
                        &(pt_cc_strm_req->u.t_scdb.t_rec)
                        );
        if (i4_ret == SVCTXR_OK) {
            pt_cc_strm_req->u.t_scdb.t_comp_id = t_comp_id;
        } else {
            pt_cc_strm_req->u.t_scdb.t_comp_id.pv_stream_tag = NULL;
            x_memset(&(pt_cc_strm_req->u.t_scdb.t_rec), 0, sizeof(SCDB_REC_T));
        }
    }

    if ((ui4_reason & SCDB_RECORD_DEL) > 0) {
        ch_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_SCDB_DEL,
                        ST_UNKNOWN
                        );
    }

    if ((ui4_reason & SCDB_RECORD_ADD) > 0) {
        ch_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_SCDB_ADD,
                        ST_UNKNOWN
                        );
    }

    if ((ui4_reason & SCDB_RECORD_MOD) > 0) {
        ch_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_SCDB_MODIFY,
                        ST_UNKNOWN
                        );
    }

    if (ui4_reason == SCDB_COMMON_MOD) {
        ch_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_SCDB_MODIFY,
                        ST_UNKNOWN
                        );
    }

    /* If I-frame is played by MHEG5, don't reselect streams. */
    {
      #ifdef MW_TV_AV_BYPASS_SUPPORT
        SVCTX_T* pt_lead = NULL;
        pt_lead = svctx_mgr_get_link_lead(ACTIVE_LINK_NOW, pt_svctx->pt_running_svc_req->e_src_type);
        if (pt_lead && pt_lead->pt_running_svc_req->b_enable_i_frame)
      #else
        if (pt_running_svc_req->t_parent.b_enable_i_frame) 
      #endif
        {
            DBG_INFO((_INFO_HEADER"(%13s)I-Frame enabled, skip streams reselecting.\n", pt_svctx->s_name));
            return i4_ret;
        }
    }

    return  _ch_hdlr_reselect_strms(pt_svctx);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_check_cc_avail_cond
 * Description
 *      The API is used to check currrent service's non-fake cc availability
 * Input arguments
 *      pt_svctx            the pointer points to a SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK         the routine is successful
 *      SVCTXR_FAIL       the routine is failed
 *-----------------------------------------------------------------------------*/
 static INT32 _ch_hdlr_check_cc_avail_cond(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    UINT16 ui2_num_recs   = 0;
    INT32  i4_ret;

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    if (pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ANALOG) {
        /* the approach for check cc availability is only valid for DIGITAL */
        return SVCTXR_OK;
    }

    /* get the amount of non-fake CC SCDB records */
    i4_ret = ch_hdlr_get_non_fake_cc_scdb_num_recs(
                    pt_svctx,
                    &ui2_num_recs
                    );
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    if ((ui2_num_recs  > 0 && pt_svctx->e_cc_avil_cond == SVCTX_CC_AVIL_COND_YES) ||
        (ui2_num_recs == 0 && pt_svctx->e_cc_avil_cond == SVCTX_CC_AVIL_COND_NO)) {
            /* nothing change */
        return SVCTXR_OK;
    }

    if (ui2_num_recs > 0) {
        pt_svctx->e_cc_avil_cond = SVCTX_CC_AVIL_COND_YES;
        ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_WITH_CC_AVIL_INDICATOR,
                    ST_CLOSED_CAPTION
                    );
    } else {
        pt_svctx->e_cc_avil_cond = SVCTX_CC_AVIL_COND_NO;
        ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_NO_CC_AVIL_INDICATOR,
                    ST_CLOSED_CAPTION
                    );
    }
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_handle_player_nty
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_player_nty(
                    PLAYER_T*                   pt_player,
                    PLAYER_NTFY_CODE_T          e_code,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data
                    )
{
    SVCTX_T*       pt_svctx           = (SVCTX_T*) pv_tag;
    CH_SVC_REQ_T*  pt_running_svc_req = NULL;
    INT32          i4_ret             = SVCTXR_OK;

    ASSERT(pt_svctx != NULL, ("%s(), pt_svctx is NULL",__func__));

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_svctx is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }
      /* for fix CR DTV00345068: after unplug signal cable, the TV show no-audio & no Video 
         after change source back to TV, for the CM will notify the connected & connect info*/
#ifdef MW_TV_AV_BYPASS_SUPPORT        
           if((pt_running_svc_req->e_brdcst_type != BRDCST_TYPE_ANALOG) &&
              (e_code == PLAYER_NTFY_CODE_CONN_CONNECTED ||
              ((e_code == PLAYER_NTFY_CODE_CONN_INFO_NTFY) &&              
              (
        #ifdef CM_CONN_INFO_GOT_TUNER_SIGNAL
               (ui4_data == CM_CONN_INFO_GOT_TUNER_SIGNAL)||
        #endif
                (ui4_data == CM_CONN_INFO_GOT_TV_SIGNAL)))))
           {
                INT32  i = 0;
                BOOL   b_process = TRUE;
                ACTIVE_LINK_ENTRY_T* pt_link = NULL;
                pt_link = svctx_mgr_get_link_obj(
                                     ACTIVE_LINK_NOW,
                                     pt_running_svc_req->t_parent.e_src_type,
                                     i++);
                    if (!pt_link || !pt_link->pt_worker)
                    {
                        return i4_ret;
                    } 
                   if( pt_link->pt_worker->b_fake_disc && 
                       pt_svctx->i1_prio_diff > pt_link->pt_worker->i1_prio_diff)
                    {
                        b_process = FALSE;
                        pt_svctx->b_fake_disc = TRUE;
                        i4_ret = ch_hdlr_handle_cm_disconnected(pt_svctx, TRUE);
                        return i4_ret;
                    }              
                if(!b_process)
                {
                     return i4_ret;
                }    
           }
#endif

#if 0
    if (pt_running_svc_req == NULL)
    {
        if ((pt_svctx->e_signal_status == SIGNAL_STATUS_UNKNOWN) &&
            (pt_svctx->e_state == SVCTX_COND_READY))
        {
            /* Alec-041709: Added for resolving DTV00056220 */
            /* pt_running_svc_req may be deleted before _ch_hdlr_handle_player_nty() is executed */
            return i4_ret;
        }
        else
        {
            ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));
        }
    }
#endif

    switch(e_code) {
    case PLAYER_NTFY_CODE_PIPE_OPENED_GET_CMDS:
#ifdef MW_TV_AV_BYPASS_SUPPORT
        pt_svctx->b_fake_disc = FALSE;
#endif
        i4_ret = _ch_hdlr_get_cm_cmds(pt_svctx, pt_running_svc_req, (CM_COMMAND_T*)ui4_data);
        break;
    case PLAYER_NTFY_CODE_PIPE_CLOSED:
        i4_ret = _ch_hdlr_handle_player_stopped(pt_svctx, pt_running_svc_req);
        break;
    case PLAYER_NTFY_CODE_CONN_CONNECTED:
        svctx_cli_print_channel_change_log(pt_svctx,CH_XNG_STAGE_RECEIVED_CONNECTED);
        i4_ret = _ch_hdlr_handle_cm_connected(pt_svctx, pt_running_svc_req);
		if (ui4_data == BRDCST_CONNECT_WITHOUT_PMT_TABLE)
        {
            ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_NORMAL, ST_UNKNOWN);
        }
#if 1  /* for MW_NINJA_CHANGE_CHANNEL */
        if (pt_running_svc_req->t_src_info.b_ninja_change && pt_running_svc_req->pt_player->b_ninja_change)
        {
            /* INT32 i = 0;*/
            /* INT32 j = 0;*/
            svctx_cancel_no_video_related_delay_nfy(pt_svctx);
            ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_NORMAL, ST_UNKNOWN);
            ch_hdlr_check_video_fmt(pt_svctx, pt_running_svc_req);
            ch_hdlr_check_audio_fmt(pt_svctx, pt_running_svc_req);
#if 0
#ifdef MW_TV_AV_BYPASS_SUPPORT
            while(TRUE)
            {
                ACTIVE_LINK_ENTRY_T* pt_entry = NULL;
                pt_entry = svctx_mgr_get_link_obj(ACTIVE_LINK_NOW, pt_running_svc_req->t_parent.e_src_type, i);
                if (pt_entry == NULL)
                {
                    break;
                }

                if (pt_entry->pt_worker == pt_svctx)
                {
                    j = i;
                }
                else
                {
                    if (i-1 == j)
                    {
                         player_play(
                            ((CH_SVC_REQ_T*)(pt_entry->pt_worker->pt_running_svc_req))->pt_player,
                            ((CH_SVC_REQ_T*)(pt_entry->pt_worker->pt_running_svc_req))->t_parent.i4_speed);
                    }
                }

                i++;
            }
#endif            
#endif
        }
#endif

        break;
    case PLAYER_NTFY_CODE_CONN_DISCONNECTED:
        i4_ret = ch_hdlr_handle_cm_disconnected(pt_svctx, (BOOL)ui4_data);
#ifdef MW_TV_AV_BYPASS_SUPPORT
        if (IS_SVCTX_ASSOCIATE(pt_svctx))
        {
            SVCTX_T* pt_lead;

            pt_lead = svctx_mgr_get_link_lead(ACTIVE_LINK_NOW, pt_svctx->pt_running_svc_req->e_src_type);
            if (pt_lead && !pt_svctx->b_fake_disc && pt_lead->i1_prio_diff > pt_svctx->i1_prio_diff)
            {
                i4_ret = ch_hdlr_handle_cm_disconnected(pt_lead, (BOOL)ui4_data);
            }
        }
#endif
        break;
    case PLAYER_NTFY_CODE_CONN_INFO_NTFY:
        i4_ret = _ch_hdlr_handle_cm_info_ntfy(pt_svctx, ui4_data);
        break;
    case PLAYER_NTFY_CODE_CONN_ANA_FREQ:
        /*Printf(__FILE__"[%d] is PLAYER_NTFY_CODE_CONN_ANA_FREQ freq=%d\r\n",__LINE__,ui4_data);*/
        pt_svctx->ui4_ana_freq = ui4_data;
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_CONNECTION_ANA_FREQ, ST_UNKNOWN);
        break;
    case PLAYER_NTFY_CODE_SCDB_OPENED:
        x_scdb_reg_cmp_fcts(pt_running_svc_req->pt_player->h_scdb, t_g_svctx_class.t_scdb_sort_param.pv_tag, 
            t_g_svctx_class.t_scdb_sort_param.pf_scdb_sort_fct);
        break;
    case PLAYER_NTFY_CODE_SCDB_UPDATED:
#ifdef MW_TV_AV_BYPASS_SUPPORT
        /* For DTV00319526 (Ziggo Certification)(DTV only):
         * The SCDB_UPDATED notification may be in random sequence with intervals,
         * for example, notification for main arriving before that for tv_bypass, 
         * such timing will cause error of conflicting streams among the presenting
         * instances. So here we try to garantee the right timing/flow by our own:
         *   We only trigger the process for SCDB update after all the necessary 
         *   instances are notified.  */
        if (pt_running_svc_req->e_brdcst_type != BRDCST_TYPE_ANALOG)
        {
            INT32  i = 0;
            BOOL   b_process = TRUE;
            ACTIVE_LINK_ENTRY_T* pt_link = NULL;
    
            pt_svctx->b_scdb_updated = TRUE;
    
            while(TRUE)
            {
                pt_link = svctx_mgr_get_link_obj(
                                 ACTIVE_LINK_NOW,
                                 pt_running_svc_req->t_parent.e_src_type,
                                 i++);
                if (!pt_link || !pt_link->pt_worker)
                {
                    break;
                }
    
#ifdef SYS_RECORD_SUPPORT
                if ( IS_SVCTX_RECORD(pt_link->pt_worker) ) continue;
#endif
    
                if (pt_link->pt_worker->b_scdb_updated == FALSE) 
                {
                    b_process = FALSE;
                    break;
                }
            }
    
            if (b_process == TRUE)
            {
                i = 0;
                
                while(TRUE)
                {
                    pt_link = svctx_mgr_get_link_obj(
                                     ACTIVE_LINK_NOW,
                                     pt_running_svc_req->t_parent.e_src_type,
                                     i++);
                    if (!pt_link || !pt_link->pt_worker)
                    {
                        break;
                    }
    
#ifdef SYS_RECORD_SUPPORT
                    if ( IS_SVCTX_RECORD(pt_link->pt_worker) ) continue;
#endif
    
                    pt_link->pt_worker->b_scdb_updated = FALSE;
                
                    _ch_hdlr_check_cc_avail_cond(pt_link->pt_worker, 
                                (CH_SVC_REQ_T*)pt_link->pt_worker->pt_running_svc_req);
                    i4_ret = _ch_hdlr_handle_scdb_updated(pt_link->pt_worker, 
                                 (CH_SVC_REQ_T*)pt_link->pt_worker->pt_running_svc_req,
                                 ui4_data);
                }
            }
        }
        else
#endif /* MW_TV_AV_BYPASS_SUPPORT */
        {
            _ch_hdlr_check_cc_avail_cond(pt_svctx, pt_running_svc_req);
            i4_ret = _ch_hdlr_handle_scdb_updated(pt_svctx, pt_running_svc_req, ui4_data);
        }
        break;
    case PLAYER_NTFY_CODE_SESS_OPENED:
        if (pt_running_svc_req->e_brdcst_type != BRDCST_TYPE_ANALOG
                && pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) {
            /* it is caused from signal loss to return to OPENED state
             * so, don't need to select sterams
             */
            break;
        }
        _ch_hdlr_check_cc_avail_cond(pt_svctx, pt_running_svc_req);
        i4_ret = _ch_hdlr_handle_sess_opened(pt_svctx, pt_running_svc_req);
        break;
    case PLAYER_NTFY_CODE_SESS_STARTED:
        i4_ret = _ch_hdlr_handle_sess_started(pt_svctx, pt_running_svc_req);
        break;
    case PLAYER_NTFY_CODE_SESS_CLOSING:
        i4_ret = _ch_hdlr_handle_sess_in_closing(pt_svctx, pt_running_svc_req);
        break;
    case PLAYER_NTFY_CODE_SESS_STOPPING:
        i4_ret = _ch_hdlr_handle_sess_in_stopping(pt_svctx, pt_running_svc_req);
        break;
    case PLAYER_NTFY_CODE_REQ_SEND_MSG:
        i4_ret = svctx_send_msg(pt_svctx->h_sys_svctx, (SVCTX_MSG_T*)ui4_data);
        break;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    case PLAYER_NTFY_CODE_QUERY_ACTION:
        i4_ret = svctx_handle_action_query(
            pt_svctx,
            pt_player,
            PLAYER_TYPE_CH_SVC,
            (SVCTX_MSG_T*) ui4_data);
        break;
    case PLAYER_NTFY_CODE_CHECK_QUEUED_REQ:
        i4_ret = svctx_mgr_check_queued_req(
            ACTIVE_LINK_NOW,
            pt_svctx->pt_running_svc_req->e_src_type,
            pt_svctx,
            FALSE);
        break;
    case PLAYER_NTFY_CODE_CONN_FAKE_DISCONNECTED:
        DBG_SVCTXMGR_MSG(("\r\n{SVCTX_MGR} Signal loss (%s) handling starts...\r\n\r\n", pt_svctx->s_name));

        if (pt_running_svc_req->e_brdcst_type != BRDCST_TYPE_ANALOG)
        {
            pt_svctx->b_fake_disc = TRUE;

            i4_ret = ch_hdlr_handle_cm_disconnected(pt_svctx, (BOOL)ui4_data);
        }

        DBG_SVCTXMGR_MSG(("\r\n{SVCTX_MGR} Signal loss (%s) handling ends...\r\n\r\n", pt_svctx->s_name));
        break;
    case PLAYER_NTFY_CODE_CONN_FAKE_CONNECTED:
        DBG_SVCTXMGR_MSG(("\r\n{SVCTX_MGR} Signal retrieved (%s) handling starts...\r\n\r\n", pt_svctx->s_name));

        if (pt_running_svc_req->e_brdcst_type != BRDCST_TYPE_ANALOG)
        {
            i4_ret = _ch_hdlr_handle_cm_connected(pt_svctx, pt_running_svc_req);

            if (pt_svctx->b_is_blocked)
            {
                i4_ret = _ch_hdlr_block_service(pt_svctx, pt_running_svc_req);
            }
            else
            {
                i4_ret = _ch_hdlr_restore_strms(pt_svctx);
            }

            ch_hdlr_invoke_client_svc_nfy_fct(
                            pt_svctx,
                            SVCTX_NTFY_CODE_SIGNAL_LOCKED,
                            ST_UNKNOWN
                            );

            if (pt_svctx->b_fake_disc)
            {
                pt_svctx->t_video_info = pt_svctx->t_x_video_info;
                pt_svctx->t_clr_sys    = pt_svctx->t_x_clr_sys;
                pt_svctx->t_audio_info = pt_svctx->t_x_audio_info;
                pt_svctx->e_av_status  = pt_svctx->e_x_av_status;
            }

            ch_hdlr_invoke_client_svc_nfy_fct(
                            pt_svctx,
                            SVCTX_NTFY_CODE_NORMAL,
                            ST_UNKNOWN
                            );

            if (TRUE == svctx_is_valid_video_fmt(pt_svctx))
            {
                ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE,
                    &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]));
            }

            pt_svctx->b_fake_disc = FALSE;

            if (pt_svctx->e_video_decode_status == VSH_DIG_DECODE_CODEC_NOT_SUPPORT)
            {
                 ch_hdlr_check_strms_status(pt_svctx);
            }
        }

        DBG_SVCTXMGR_MSG(("\r\n{SVCTX_MGR} Signal retrieved (%s) handling ends...\r\n\r\n", pt_svctx->s_name));
        break;
#endif
    default:
        break;
    }
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: _ch_hdlr_update_crnt_svc_name
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_update_crnt_svc_name (
                    SVCTX_T*                        pt_svctx,
                    CH_SVC_REQ_T*                   pt_running_svc_req
                    )
{
    HANDLE_T     h_evctx      = NULL_HANDLE;
#ifndef MW_EDB_SUPPORT    
    EVCTX_COND_T e_cond       = EVCTX_COND_UNAVAIL;
#else
    EDB_CH_EVT_ID_T      t_ch_evt_id = {0};
#endif
    SIZE_T       z_len        = 0;
    UINT8*       pui1_ch_name = NULL;
    HANDLE_T     h_svl        = NULL_HANDLE;
    UINT32       ui4_ver_id   = SVL_NULL_VER_ID;
    SVL_REC_T    t_svl_rec;
    INT32        i4_ret;

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_svctx is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    /* get SVL handle */
    i4_ret = _ch_hdlr_get_svl(pt_running_svc_req, &h_svl);
    if (i4_ret != SVCTXR_OK) {
        return SVCTXR_FAIL;
    }

    /* get current SVL record */
    i4_ret = x_svl_get_rec (
                    h_svl,
                    pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svl_rec_id,
                    &t_svl_rec,
                    &ui4_ver_id
                    );
    if (i4_ret != SVLR_OK) {
        return SVCTXR_FAIL;
    }

    /* if it is not analog, service context should not update it */
    if (t_svl_rec.uheader.t_rec_hdr.e_brdcst_type != BRDCST_TYPE_ANALOG) {
        return SVCTXR_OK;
    }

    if ((t_svl_rec.uheader.t_rec_hdr.ui4_nw_mask & SB_VNET_CH_NAME_EDITED) > 0 || /* For compatibility reason, will be removed later */
        (t_svl_rec.uheader.t_rec_hdr.ui4_option_mask & SB_VOPT_CH_NAME_EDITED) > 0) {
        return SVCTXR_OK;
    }

    h_evctx = crnt_evn_get_handle(pt_running_svc_req->pt_crnt_event);
    if (x_handle_valid(h_evctx) == FALSE) {
        return SVCTXR_EVN_INFO_NOT_FOUND;
    }

    /* check there's service name information */
#ifndef MW_EDB_SUPPORT    
    i4_ret = x_evctx_get_event_info_len(
                    h_evctx,
                    EVCTX_KEY_TYPE_SVC_NAME,
                    NULL, /* get first language one */
                    &z_len,
                    &e_cond
                    );
#else
    t_ch_evt_id.e_brdcst_type     = pt_running_svc_req->e_brdcst_type;
    t_ch_evt_id.ui2_svl_id        = pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id;
    t_ch_evt_id.ui4_channel_id    = t_svl_rec.uheader.t_rec_hdr.ui4_channel_id;
    t_ch_evt_id.b_auto_event_id   = TRUE;
    i4_ret = x_edb_get_info_len(
                    h_evctx,
                    EDB_KEY_TYPE_EVENT_TITLE_BY_ID,
                    &t_ch_evt_id,
                    &z_len
                    );
#endif
    if (i4_ret < 0 || z_len == 0) {
        return SVCTXR_EVN_INFO_NOT_FOUND;
    }

    /* allocate temp memory to get service name information */
    pui1_ch_name = (UINT8*) x_mem_alloc(z_len);
    if (pui1_ch_name == NULL) {
        return SVCTXR_NO_RESOURCE;
    }
    x_memset(pui1_ch_name, 0x0, z_len);

    do {
        /* get service name information */
#ifndef MW_EDB_SUPPORT        
        i4_ret = x_evctx_get_event_info(
                    h_evctx,
                    EVCTX_KEY_TYPE_SVC_NAME,
                    NULL, /* get first language one */
                    &z_len,
                    (VOID*)pui1_ch_name,
                    &e_cond
                    );
#else
        i4_ret = x_edb_get_info(
                    h_evctx,
                    EDB_KEY_TYPE_EVENT_TITLE_BY_ID,
                    &t_ch_evt_id, /* get first language one */
                    &z_len,
                    (VOID*)pui1_ch_name
                    );
#endif
        if (i4_ret < 0) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        /* check if the name is the same or not */
        if (x_strncmp(t_svl_rec.uheader.t_rec_hdr.ac_name, (CHAR*)pui1_ch_name, MAX_PROG_NAME_LEN) == 0) {
            /* the same */
            i4_ret = SVCTXR_OK;
            break;
        }

        /* update current one to the new service name */
        x_svl_lock(h_svl);
        x_strncpy(t_svl_rec.uheader.t_rec_hdr.ac_name, (CHAR*)pui1_ch_name, MAX_PROG_NAME_LEN);
        t_svl_rec.uheader.t_rec_hdr.ac_name[MAX_PROG_NAME_LEN] = '\0';
        i4_ret = x_svl_update_rec(h_svl, &t_svl_rec, TRUE);
        i4_ret = (i4_ret == SVLR_OK) ? SVCTXR_OK : SVCTXR_FAIL;
        x_svl_unlock(h_svl);
    } while (0);

    x_mem_free(pui1_ch_name);

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_handle_crnt_evn_nty
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_crnt_evn_nty(
                    CRNT_EVENT_T*               pt_crnt_evn,
                    CRNT_EVENT_NTFY_CODE_T      e_code,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data
                    )
{
    SVCTX_T*      pt_svctx = (SVCTX_T*) pv_tag;
    INT32         i4_ret   = SVCTXR_OK;
    CH_SVC_REQ_T* pt_running_svc_req = NULL;

    ASSERT(pt_svctx != NULL, ("%s(), pt_svctx is NULL",__func__));

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_svctx is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (CRNT_EVENT_NTFY_CODE_REQ_SEND_MSG != e_code) {
        /* as e_code = CRNT_EVENT_NTFY_CODE_REQ_SEND_MSG, it is still in Evctx's thread context
         * So, do not access the pt_running_svc_req and do the != check
         * to avoid the race-condition
         */
        if (pt_running_svc_req == NULL)
        {
            if ((pt_svctx->e_signal_status == SIGNAL_STATUS_UNKNOWN) &&
                (pt_svctx->e_state == SVCTX_COND_READY))
            {
                return i4_ret;
            }
            else
            {
                ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));
            }
        }

        if (pt_crnt_evn != pt_running_svc_req->pt_crnt_event) {
            return SVCTXR_FAIL;
        }
    }

    switch(e_code) {
    case CRNT_EVENT_NTFY_CODE_START_QUERY:
        if (pt_running_svc_req->t_src_info.u_info.t_svl.t_silent.b_enable == FALSE) {
            ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CRNT_EVN_START_QUERY,
                    ST_UNKNOWN
                    );
        }
        break;
    case CRNT_EVENT_NTFY_CODE_EVN_NOT_READY:
        ch_hdlr_check_svc_block(pt_svctx);
        if (pt_svctx->e_block_rule == SVCTX_BLOCK_RULE_INP_SIGNAL_LOSS_CH_PROG
            || pt_svctx->e_block_rule == SVCTX_BLOCK_RULE_INP_CH_SIGNAL_LOSS_PROG) {
            if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) {
                /* when pull-out signal cable, the video stream will be closed
                 * and this causes EVENT Context to be in unavailable state, i.e.
                 * CRNT_EVENT_NTFY_CODE_EVN_NOT_READY. This notification is
                 * useless.
                 */
                break;
            }

            ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CRNT_EVN_NOT_READY,
                    ST_UNKNOWN
                    );
        } else {
            ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CRNT_EVN_READY,
                    ST_UNKNOWN
                    );
        }
        break;
    case CRNT_EVENT_NTFY_CODE_EVN_READY:
        _ch_hdlr_update_crnt_svc_name(pt_svctx, pt_running_svc_req);
        ch_hdlr_check_svc_block(pt_svctx);
        ch_hdlr_invoke_client_svc_nfy_fct(
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
 *      _ch_hdlr_handle_strm_fmt_chker_nty
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_strm_fmt_chker_nty(
                    STRM_FMT_CHKER_T*           pt_strm_fmt_chker,
                    STRM_FMT_CHKER_NTFY_CODE_T  e_code,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data
                    )
{
    SVCTX_T*      pt_svctx           = (SVCTX_T*) pv_tag;
    CH_SVC_REQ_T* pt_running_svc_req = NULL;
    INT32         i4_ret             = SVCTXR_OK;

    ASSERT(pt_svctx != NULL, ("%s(), pt_svctx is NULL",__func__));

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_svctx is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

#if 0
    if (pt_running_svc_req == NULL)
    {
        if ((pt_svctx->e_signal_status == SIGNAL_STATUS_UNKNOWN) &&
            (pt_svctx->e_state == SVCTX_COND_READY))
        {
            return i4_ret;
        }
        else
        {
            ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));
        }
    }
#endif

    switch(e_code) {
    case STRM_FMT_CHKER_NTFY_CODE_DELAY_TIMEOUT_MSG:
        switch((STREAM_TYPE_T)ui4_data) {
        case ST_AUDIO:
            i4_ret = _ch_hdlr_handle_audio_fmt_chk(pt_svctx, pt_running_svc_req);
            break;
        case ST_VIDEO:
            i4_ret = _ch_hdlr_handle_video_fmt_chk(pt_svctx, pt_running_svc_req);
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
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_handle_rating_chker_nty
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_handle_rating_chker_nty(
                    RATING_CHKER_T*             pt_rating_chker,
                    RATING_CHKER_NTFY_CODE_T    e_code,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data
                    )
{
    SVCTX_T*      pt_svctx = (SVCTX_T*) pv_tag;
    INT32         i4_ret   = SVCTXR_OK;
    CH_SVC_REQ_T* pt_running_svc_req;

    ASSERT(pt_svctx != NULL, ("%s(), pt_svctx is NULL",__func__));

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_svctx is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

#if 0
    if (pt_running_svc_req == NULL)
    {
        if ((pt_svctx->e_signal_status == SIGNAL_STATUS_UNKNOWN) &&
            (pt_svctx->e_state == SVCTX_COND_READY))
        {
            return i4_ret;
        }
        else
        {
            ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));
        }
    }
#endif

    switch(e_code) {
    case RATING_CHKER_NTFY_CODE_DELAY_TIMEOUT_MSG:
        i4_ret = player_play(
                    pt_running_svc_req->pt_player,
                    pt_running_svc_req->t_parent.i4_speed
                    );
        break;
    case RATING_CHKER_NTFY_CODE_SVL_UPDATE:
        if (pt_running_svc_req->t_src_info.u_info.t_svl.t_ch_encode.e_type == CHANNEL_DECODE_TYPE_MANUAL) {
            i4_ret = _ch_hdlr_monitor_scart_signal(
                    pt_svctx,
                    pt_running_svc_req
                    );
        }
        break;
    case RATING_CHKER_NTFY_CODE_REQ_SEND_MSG:
        i4_ret = svctx_send_msg(pt_svctx->h_sys_svctx, (SVCTX_MSG_T*)ui4_data);
        break;
    default:
        break;
    }

    return i4_ret;
}

static VOID _ch_hdlr_tsl_nfy(HANDLE_T   h_tsl,
                                 TSL_COND_T e_cond,
                                 UINT32     ui4_reason,
                                 VOID*      pv_tag,
                                 UINT32     ui4_data)
{
    /* To be implemented */
}

#ifdef MW_TV_AV_BYPASS_SUPPORT
static BOOL _ch_hdlr_same_strm_req(
    STRM_REQ_T*         pt_strm_req_1,
    STRM_REQ_T*         pt_strm_req_2)
{
    BOOL b_ret = FALSE;

    if ((STRM_REQ_SCDB == pt_strm_req_1->e_req_type) && (STRM_REQ_SCDB == pt_strm_req_2->e_req_type))
    {
        SCDB_REC_T* pt_rec_1 = &(pt_strm_req_1->u.t_scdb.t_rec);
        SCDB_REC_T* pt_rec_2 = &(pt_strm_req_2->u.t_scdb.t_rec);

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

static INT32 _ch_hdlr_check_stream_conflict(
    SVCTX_T*            pt_svctx,
    STRM_REQ_T*         pt_strm_req,
    STREAM_TYPE_T       e_strm_type,
    CH_SVC_REQ_T*       pt_svc_req)
{
    INT32                           i4_ret = SVCTXR_OK;
    ACTIVE_LINK_ENTRY_T*            pt_link;
    SVCTX_T*                        pt_assc;
    CH_SVC_REQ_T*                   pt_assc_svc_req;
    STRM_REQ_T*                     pt_assc_strm_req;
    STRM_REQ_T*                     pt_new_strm_req;
    UINT32                          i;
    x_svctx_strm_comp_filter_fct    pt_strm_fltr_fct;
    VOID*                           pv_strm_fltr_tag;

    if (pt_svctx == NULL || pt_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_svctx is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    if ((e_strm_type == ST_UNKNOWN) || (e_strm_type >= STRM_TYPE_COUNT))
    {
        return SVCTXR_INV_ARG;
    }

    if ((BRDCST_TYPE_ANALOG != pt_svc_req->e_brdcst_type) &&
        (!IS_SVCTX_ASSOCIATE(pt_svctx)) &&
        (TRUE != pt_strm_req->b_aux_audio))
    {
        for (i = 0; ; i++)
        {
            pt_link = svctx_mgr_get_link_obj(
                ACTIVE_LINK_NOW,
                pt_svc_req->t_parent.e_src_type,
                i);

            if (!pt_link)
            {
                break;
            }

            pt_assc = pt_link->pt_worker;

            if (pt_assc &&
                IS_SVCTX_ASSOCIATE(pt_assc) &&
                (pt_assc->i1_prio_diff < pt_svctx->i1_prio_diff))
            {
                if (0 == (pt_assc->ui4_stream_mode & MAKE_BIT_MASK_32(e_strm_type)))
                {
                    continue;
                }

                pt_assc_svc_req = (CH_SVC_REQ_T*) pt_assc->pt_running_svc_req;

                if (!pt_assc_svc_req)
                {
                    continue;
                }

                pt_assc_strm_req = &(pt_assc_svc_req->t_strm_reqs[e_strm_type]);

                if ((FALSE == x_handle_valid(pt_assc_strm_req->h_stream)) ||
                    (TRUE == _ch_hdlr_same_strm_req(pt_assc_strm_req, pt_strm_req)))
                {
                    continue;
                }

                /* allocate stream request */
                pt_new_strm_req = svctx_allocate_strm_req(pt_assc_svc_req->t_parent.pt_hdlr);

                if (!pt_new_strm_req)
                {
                    continue;
                }

                pt_new_strm_req->h_svctx         = pt_assc_strm_req->h_svctx;
                pt_new_strm_req->pf_strm_nfy     = pt_assc_strm_req->pf_strm_nfy;
                pt_new_strm_req->pv_strm_nfy_tag = pt_assc_strm_req->pv_strm_nfy_tag;
                pt_new_strm_req->e_req_type      = pt_assc_strm_req->e_req_type;
                pt_new_strm_req->b_aux_audio     = pt_assc_strm_req->b_aux_audio;

                ch_hdlr_choose_strm_filter(
                    pt_assc,
                    e_strm_type,
                    &pt_strm_fltr_fct,
                    &pv_strm_fltr_tag);

                SCDB_STRM_REQ_T     t_scdb;

                i4_ret = svctx_get_strm_comp_id(
                    pt_assc,
                    e_strm_type,
                    FALSE,
                    pt_strm_fltr_fct,
                    pv_strm_fltr_tag,
                    &t_scdb.t_comp_id,
                    &t_scdb.t_rec);

                if (SVCTXR_OK == i4_ret)
                {
                    if (pt_new_strm_req->e_req_type == STRM_REQ_PID)
                    {
                        pt_new_strm_req->u.t_pid.e_strm_type = t_scdb.t_comp_id.e_type;

                        if (t_scdb.t_rec.e_rec_type == SCDB_REC_TYPE_AUDIO_MPEG)
                        {
                            pt_new_strm_req->u.t_pid.t_mpeg_2_pid = t_scdb.t_rec.u.t_audio_mpeg.ui2_pid;
                            pt_new_strm_req->u.t_pid.u.t_aud_stream.e_aud_enc = t_scdb.t_rec.u.t_audio_mpeg.e_enc;
                        }
                        else if (t_scdb.t_rec.e_rec_type == SCDB_REC_TYPE_VIDEO_MPEG)
                        {
                            pt_new_strm_req->u.t_pid.t_mpeg_2_pid = t_scdb.t_rec.u.t_video_mpeg.ui2_pid;
                            pt_new_strm_req->u.t_pid.u.t_vid_stream.e_vid_enc = t_scdb.t_rec.u.t_video_mpeg.e_enc;
                        }
                    }
                    else if (pt_new_strm_req->e_req_type == STRM_REQ_SCDB)
                    {
                        pt_new_strm_req->u.t_scdb = t_scdb;
                    }

                    i4_ret = _ch_hdlr_handle_strm_select_req(
                        pt_assc,
                        pt_assc_svc_req,
                        pt_new_strm_req,
                        TRUE);
                }
                else
                {
                    continue;
                }
            }
        }
    }

    return (i4_ret);
}
#endif

INT32 ch_hdlr_get_ids(CH_SVC_REQ_T* pt_svc_req)
{
    INT32           i4_ret;
    SVL_REC_T       t_svl_rec;
    TSL_REC_T       t_tsl_rec;
    HANDLE_T        h_tsl;
    UINT32          ui4_tsl_ver = TSL_NULL_VER_ID;
    CH_SRC_INFO_T*  pt_src_info = NULL;

    if (pt_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_src_info = &(pt_svc_req->t_src_info);

    pt_src_info->ui2_on_id  = 0;
    pt_src_info->ui2_ts_id  = 0;
    pt_src_info->ui2_svc_id = 0;
    pt_src_info->ui4_freq   = 0;

    i4_ret = _ch_hdlr_get_svl_rec_by_req(pt_svc_req, &t_svl_rec);

    if (SVCTXR_OK == i4_ret)
    {
        if ((BRDCST_TYPE_ANALOG != t_svl_rec.uheader.t_rec_hdr.e_brdcst_type) &&
            (BRDCST_TYPE_FMRDO != t_svl_rec.uheader.t_rec_hdr.e_brdcst_type))
        {
            /* Obtain data from TSL */
            i4_ret = x_tsl_open(
                t_svl_rec.uheader.t_rec_hdr.ui2_tsl_id,
                NULL,
                _ch_hdlr_tsl_nfy,
                &h_tsl);

            if (TSLR_OK == i4_ret)
            {
                i4_ret = x_tsl_get_rec(
                    h_tsl,
                    t_svl_rec.uheader.t_rec_hdr.ui2_tsl_rec_id,
                    &t_tsl_rec,
                    &ui4_tsl_ver);

                if (TSLR_OK == i4_ret)
                {
                    pt_src_info->ui2_on_id  = t_tsl_rec.uheader.t_desc.ui2_on_id;
                    pt_src_info->ui2_ts_id  = t_tsl_rec.uheader.t_desc.ui2_ts_id;
                    pt_src_info->ui2_svc_id = t_svl_rec.uheader.t_rec_hdr.ui2_prog_id;
                    switch (t_tsl_rec.uheader.t_desc.e_bcst_medium) {
                    case BRDCST_MEDIUM_DIG_TERRESTRIAL:
                        pt_src_info->ui4_freq = t_tsl_rec.udata.t_ter_dig.ui4_freq;
                        break;
                    case BRDCST_MEDIUM_DIG_CABLE:
                        pt_src_info->ui4_freq = t_tsl_rec.udata.t_cab_dig.ui4_freq;
                        break;
                    case BRDCST_MEDIUM_DIG_SATELLITE:
                        pt_src_info->ui4_freq = t_tsl_rec.udata.t_sat_dig.ui4_freq;
                        break;
                    case BRDCST_MEDIUM_ANA_TERRESTRIAL:
                        pt_src_info->ui4_freq = t_tsl_rec.udata.t_ter_ana.ui4_freq;
                        break;
                    case BRDCST_MEDIUM_ANA_CABLE:
                        pt_src_info->ui4_freq = t_tsl_rec.udata.t_cab_ana.ui4_freq;
                        break;
                    case BRDCST_MEDIUM_ANA_SATELLITE:
                        pt_src_info->ui4_freq = t_tsl_rec.udata.t_sat_ana.ui4_freq;
                        break;
                    default:
                        pt_src_info->ui4_freq = 0;
                        break;
                    }

                    i4_ret = SVCTXR_OK;
                }
                else
                {
                    i4_ret = SVCTXR_FAIL;
                }

                x_tsl_close(h_tsl);
            }
            else
            {
                i4_ret = SVCTXR_FAIL;
            }
        }
        else
        {
            pt_src_info->b_fine_tune =
                ((t_svl_rec.uheader.t_rec_hdr.ui4_nw_mask & SB_VNET_FREQ_EDITED) || (t_svl_rec.uheader.t_rec_hdr.ui4_option_mask & SB_VOPT_FREQ_EDITED) ? FALSE : TRUE);
        }
    }

    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * SVCTX public methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_check_audio_fmt
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 ch_hdlr_check_audio_fmt (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    STRM_REQ_T* pt_audio_strm_req    = NULL;
    UINT32      ui4_strm_format_cond = 0;

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_audio_strm_req    = &(pt_running_svc_req->t_strm_reqs[ST_AUDIO]);

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if ((x_handle_valid(pt_audio_strm_req->h_stream) == FALSE) &&
        !pt_audio_strm_req->b_open_queued) {
#else
    if (x_handle_valid(pt_audio_strm_req->h_stream) == FALSE) {
#endif    
        pt_audio_strm_req->u.t_scdb.t_comp_id.e_type = ST_AUDIO;
        ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_NO_STREAM,
                    pt_audio_strm_req
                    );
        pt_audio_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
        return SVCTXR_OK;
    }

    if (pt_audio_strm_req->e_strm_cond == SM_COND_STARTED) {
        ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_STREAM_STARTED,
                    pt_audio_strm_req
                    );
        if (pt_running_svc_req->e_brdcst_type != BRDCST_TYPE_ANALOG) {
            ch_hdlr_invoke_client_strm_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_CA,
                        pt_audio_strm_req
                        );
            ch_hdlr_invoke_client_strm_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_CP,
                        pt_audio_strm_req
                        );
        }
    }

    ui4_strm_format_cond = strm_fmt_chker_get_format_status(pt_running_svc_req->pt_strm_fmt_chker);
    if (svctx_is_valid_audio_fmt(pt_svctx) == TRUE || (ui4_strm_format_cond & ST_MASK_AUDIO) > 0) {
        ch_hdlr_invoke_client_strm_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE,
                    pt_audio_strm_req
                    );
    }
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_check_video_fmt
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 ch_hdlr_check_video_fmt (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    INT32 i4_ret;

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    i4_ret = _ch_hdlr_check_video_stream(pt_svctx, pt_running_svc_req);
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    i4_ret = _ch_hdlr_check_video_fmt(pt_svctx, pt_running_svc_req);
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_is_supported_strm_type
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
BOOL ch_hdlr_is_supported_strm_type(
                    STREAM_TYPE_T           e_strm_type
                    )
{
    UINT8 ui1_idx;

    for (ui1_idx = 0; ui1_idx < (UINT8) STRM_TYPE_COUNT_WITH_AD; ui1_idx++) {
        if (e_g_ch_strm_types[ui1_idx] == e_strm_type) {
            return TRUE;
        }
    }
    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_reset_audio_info
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
VOID ch_hdlr_reset_audio_info(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return;
    }
    
    if (pt_running_svc_req->pt_strm_fmt_chker != NULL) {
        strm_fmt_chker_reset_format_status(pt_running_svc_req->pt_strm_fmt_chker, ST_AUDIO);
    }
    svctx_reset_audio_info(pt_svctx);
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_reset_video_info
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
VOID ch_hdlr_reset_video_info(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return;
    }
    
    if (pt_running_svc_req->pt_strm_fmt_chker != NULL) {
        strm_fmt_chker_reset_format_status(pt_running_svc_req->pt_strm_fmt_chker, ST_VIDEO);
    }
    svctx_reset_video_info(pt_svctx);
}
/*-----------------------------------------------------------------------------
 * Name: ch_hdlr_freeze
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *-----------------------------------------------------------------------------*/
INT32 ch_hdlr_freeze (
                    SVCTX_T*                    pt_svctx
                    )
{
    CH_SVC_REQ_T*  pt_running_svc_req = NULL;
    INT32          i4_ret             = SVCTXR_OK;

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_PENDING;
    }

    /* freeze */
    if (pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_AUDIO) {
    }

    if (pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_CLOSED_CAPTION) {
        i4_ret = _ch_hdlr_close_text_strms(
                    pt_svctx,
                    pt_running_svc_req,
                    ST_MASK_CLOSED_CAPTION
                    );
        if (i4_ret != SVCTXR_OK) {
            return i4_ret;
        }
    }

#ifdef MW_ISDB_SUPPORT
    if (pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_ISDB_CAPTION) {
        i4_ret = _ch_hdlr_close_text_strms(
                    pt_svctx,
                    pt_running_svc_req,
                    ST_MASK_ISDB_CAPTION
                    );
        if (i4_ret != SVCTXR_OK) {
            return i4_ret;
        }
    }

    if (pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_ISDB_TEXT) {
        i4_ret = _ch_hdlr_close_text_strms(
                    pt_svctx,
                    pt_running_svc_req,
                    ST_MASK_ISDB_TEXT
                    );
        if (i4_ret != SVCTXR_OK) {
            return i4_ret;
        }
    }
#endif

    if (pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_SUBTITLE) {
        i4_ret = _ch_hdlr_close_text_strms(
                    pt_svctx,
                    pt_running_svc_req,
                    ST_MASK_SUBTITLE
                    );
        if (i4_ret != SVCTXR_OK) {
            return i4_ret;
        }
    }

    if (pt_svctx->ui4_frz_frozen_strm_mask & ST_MASK_TELETEXT) {
        i4_ret = _ch_hdlr_close_text_strms(
                    pt_svctx,
                    pt_running_svc_req,
                    ST_MASK_TELETEXT
                    );
        if (i4_ret != SVCTXR_OK) {
            return i4_ret;
        }
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: ch_hdlr_unfreeze
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *-----------------------------------------------------------------------------*/
INT32 ch_hdlr_unfreeze (
                    SVCTX_T*                    pt_svctx
                    )
{
    CH_SVC_REQ_T*  pt_running_svc_req = NULL;
    INT32          i4_ret             = SVCTXR_OK;

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_PENDING;
    }

    i4_ret = _ch_hdlr_video_strm_post_start_handler(
                    pt_svctx,
                    pt_running_svc_req
                    );

    return i4_ret;
}

INT32 _ch_hdlr_select_normal_ad(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req,
                    STRM_REQ_T*                 pt_new_ad_strm_req,
                    const BOOL                  b_need_to_free_req
                    )
{
    INT32 i4_ret = SVCTXR_OK;
    STRM_REQ_T* pt_new_aud_strm_req = NULL;
    do
    {
        if (!pt_running_svc_req->t_strm_reqs[ST_AUDIO].b_mix_substrm)
        {
            break;
        }

        /* Main audio is mixed with substream, so restart main audio with no substream mixed. */
        pt_new_aud_strm_req = svctx_allocate_strm_req(pt_running_svc_req->t_parent.pt_hdlr);
        if (pt_new_aud_strm_req == NULL) {
            i4_ret = SVCTXR_NO_RESOURCE;
            break;
        }

        *pt_new_aud_strm_req = pt_running_svc_req->t_strm_reqs[ST_AUDIO];

        pt_new_aud_strm_req->b_mix_substrm         = FALSE; 
        pt_new_aud_strm_req->ui1_mixed_substrm_idx = AUDIO_INDEX_UNKNOW; 
        i4_ret = _ch_hdlr_handle_strm_select_req (
                        pt_svctx,
                        pt_running_svc_req,
                        pt_new_aud_strm_req,
                        TRUE
                        );
        if (SVCTXR_OK != i4_ret)
        {
            break;
        }

    }while(0);

    if (SVCTXR_OK != i4_ret)
    {
        svctx_free_strm_req(pt_new_aud_strm_req);
        return i4_ret;
    }

    i4_ret = _ch_hdlr_handle_strm_select_req (
                        pt_svctx,
                        pt_running_svc_req,
                        pt_new_ad_strm_req,
                        b_need_to_free_req
                        );
    return i4_ret;
}

INT32 _ch_hdlr_select_substrm_ad(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req,
                    STRM_REQ_T*                 pt_new_substrm_req,
                    const BOOL                  b_need_to_free_req
                    )
{
    INT32         i4_ret               = SVCTXR_OK;
    UINT8         ui1_new_substrm_idx  = AUDIO_INDEX_UNKNOW;
    SVCTX_MSG_T   t_stop_msg           = {0};
    STRM_REQ_T*   pt_crnt_aud_strm_req = &pt_running_svc_req->t_strm_reqs[ST_AUDIO];
    STRM_REQ_T*   pt_new_aud_strm_req  = NULL;

    ui1_new_substrm_idx = pt_new_substrm_req->u.t_scdb.t_rec.u.t_audio_mpeg.ui1_index;
    do
    {
        /* Do nothing if Main Audio's already mixing the new substream */
        if (pt_crnt_aud_strm_req->b_mix_substrm
                && ui1_new_substrm_idx == pt_crnt_aud_strm_req->ui1_mixed_substrm_idx)
        {
            i4_ret = SVCTXR_OK;
            break;
        }

        /* if normal AD running, stop it first. */
        if (x_handle_valid(pt_running_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX].h_stream))
        {
            t_stop_msg.h_svctx                       = pt_crnt_aud_strm_req->h_svctx;
            t_stop_msg.ui1_msg_id                    = _SVCTX_MSG_STOP_STRM_REQ;
            t_stop_msg.u.t_stop_strm_req.e_type      = ST_AUDIO;
            t_stop_msg.u.t_stop_strm_req.b_aux_audio = TRUE;

            ch_hdlr_stop_strm(pt_svctx, &t_stop_msg);
        }

        /* Restart main audio with new requested substream mixed. */
        pt_new_aud_strm_req = svctx_allocate_strm_req(pt_running_svc_req->t_parent.pt_hdlr);
        if (pt_new_aud_strm_req == NULL) {
            i4_ret = SVCTXR_NO_RESOURCE;
            break;
        }

        *pt_new_aud_strm_req = *pt_crnt_aud_strm_req;
        pt_new_aud_strm_req->b_mix_substrm = TRUE; 
        pt_new_aud_strm_req->ui1_mixed_substrm_idx = ui1_new_substrm_idx; 

        i4_ret = _ch_hdlr_handle_strm_select_req (
                        pt_svctx,
                        pt_running_svc_req,
                        pt_new_aud_strm_req,
                        TRUE
                        );
    }while (0);

    if (b_need_to_free_req)
    {
        svctx_free_strm_req(pt_new_aud_strm_req);
    }
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_select_strm
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 ch_hdlr_select_strm (
                    SVCTX_T*                    pt_svctx,
                    SVCTX_MSG_T*                pt_msg
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = NULL;

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_WRONG_STATE;
    }

#ifdef GINGA_SUPPORT
   if((NULL != pt_msg) && (NULL != pt_msg->u.pt_strm_req))
   {
   	  STREAM_TYPE_T  e_strm_type = svctx_get_strm_req_strm_type(pt_msg->u.pt_strm_req);
    	svctx_pre_select_stream_cb(pt_msg->u.pt_strm_req->h_svctx, e_strm_type);  
   }
   else
   {
   	  DBG_ERROR((_ERROR_HEADER"%s(), pt_svctx->pt_msg->u.pt_strm_req is NULL\r\n", __func__));
   }
#endif

    if (!pt_msg->u.pt_strm_req->b_aux_audio)
    {
        return _ch_hdlr_handle_strm_select_req (
                pt_svctx,
                pt_running_svc_req,
                pt_msg->u.pt_strm_req,
                TRUE
                );
    }
    else
    {
        if (pt_msg->u.pt_strm_req->u.t_scdb.t_rec.u.t_audio_mpeg.e_aud_fmt == AUD_FMT_SUBSTREAM)
        {
            return _ch_hdlr_select_substrm_ad(
                    pt_svctx,
                    pt_running_svc_req,
                    pt_msg->u.pt_strm_req,
                    TRUE
                    );
        }
        else
        {
            return _ch_hdlr_select_normal_ad(
                    pt_svctx,
                    pt_running_svc_req,
                    pt_msg->u.pt_strm_req,
                    TRUE
                    );
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_select_svc
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 ch_hdlr_select_svc (
                    SVCTX_T*                    pt_svctx
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = NULL;
    HANDLE_T      h_svl              = NULL_HANDLE;
    INT32         i4_ret             = SVCTXR_OK;

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_WRONG_STATE;
    }

#if 1  /* for MW_NINJA_CHANGE_CHANNEL */
    if (!(pt_running_svc_req->t_src_info.b_ninja_change && pt_running_svc_req->pt_player!=NULL && 
        pt_running_svc_req->pt_player->b_ninja_change))
#endif
{
    /* Riks TV begin */
#ifdef MW_EDB_SUPPORT
#ifdef MW_EDB_FBM_SHARE_MEM
    i4_ret = svctx_rikstv_set_edb(pt_running_svc_req->t_riks_tv.b_tune_alter_freq_if_loss_signal);
    if (SVCTXR_OK != i4_ret)
    {
        return i4_ret;
    }
#endif
#endif
    i4_ret = svctx_rikstv_handle_first_tune_or_freq_list_update_check(pt_svctx);
    if (i4_ret == SVCTXR_OK)
    {
        return SVCTXR_PENDING;
    }
    /* Riks TV end */

    svctx_reset_x_av_info(pt_svctx);

    svctx_update_src_snk_names(
                    pt_svctx,
                    pt_running_svc_req->t_src_info.s_src_grp_name,
                    pt_running_svc_req->t_parent.s_snk_grp_name
                    );

    i4_ret = crnt_evn_open(
                    DEFAULT_REQUERY_EVN_DELAY,
                    pt_svctx->e_block_rule == SVCTX_BLOCK_RULE_INP_CH_PROG_SIGNAL_LOSS
                        ? FALSE : TRUE,
                    _ch_hdlr_handle_crnt_evn_nty,
                    pt_svctx,
                    &pt_running_svc_req->pt_crnt_event
                    );
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    i4_ret = strm_fmt_chker_open(
                    DEFAULT_STRM_FMT_CHK_TIMEOUT,
                    _ch_hdlr_handle_strm_fmt_chker_nty,
                    pt_svctx,
                    &pt_running_svc_req->pt_strm_fmt_chker
                    );
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    if (pt_svctx->pf_block_check_fct == NULL) {
        pt_svctx->pf_block_check_fct = svctx_null_block_check_fct;
#ifdef SVCTX_SPLIT_SUPPORT
        pt_svctx->pf_block_check_fct_tag    = NULL;
#endif
    }

    i4_ret = rating_chker_open(
                    pt_svctx->pf_block_check_fct,
#ifdef SVCTX_SPLIT_SUPPORT
                    pt_svctx->pf_block_check_fct_tag,
#endif
                    DEFAULT_BLOCK_DELAY,
                    _ch_hdlr_handle_rating_chker_nty,
                    pt_svctx,
                    &pt_running_svc_req->pt_rating_chker
                    );
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }
}

    if (pt_running_svc_req->t_parent.e_src_type == SRC_TYPE_BRDCST_SVL)
    {
        SVC_BLOCK_CHECK_GUDIE_T e_guide;

        i4_ret = _ch_hdlr_get_svl(pt_running_svc_req, &h_svl);
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
        _ch_hdlr_is_blocked_service(
                    pt_svctx,
                    pt_running_svc_req,
                    e_guide,
                    TRUE
                    );
    }

    x_scc_vid_set_sig_booster_flag(
        pt_svctx->h_video_scc_comp,
        pt_running_svc_req->t_parent.b_enable_signal_booster);

    if (pt_running_svc_req->pt_player == NULL
            || pt_running_svc_req->pt_player->b_reuse_pipe == FALSE) {
        i4_ret = player_open (
                    pt_svctx->s_snk_grp_name,
                    DEFAULT_RECONNECT_DELAY,
                    _ch_hdlr_handle_player_nty,
                    pt_svctx,
#ifdef MW_TV_AV_BYPASS_SUPPORT
                    pt_svctx->i1_prio_diff,
#endif
                    &pt_running_svc_req->pt_player
                    );
        if (i4_ret != SVCTXR_OK) {
            return i4_ret;
        }
    }

    i4_ret = player_play(
                    pt_running_svc_req->pt_player,
                    pt_running_svc_req->t_parent.i4_speed
                    );

    if ((ui1_ch_suf_ind < 2)
#ifdef MW_TV_AV_BYPASS_SUPPORT  
        && !IS_SVCTX_ASSOCIATE(pt_svctx)
#endif        
        )
    {        
        ui1_ch_suf_ind++;
    }        

#if 0
#ifdef MW_NINJA_CHANGE_CHANNEL
    if (pt_running_svc_req->t_src_info.b_ninja_change && pt_running_svc_req->pt_player!=NULL && 
            pt_running_svc_req->pt_player->b_ninja_change)
    {
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_TUNER_SIGNAL_LOCKED, ST_UNKNOWN);
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOCKED, ST_UNKNOWN);
        svctx_cancel_no_video_related_delay_nfy(pt_svctx);
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_NORMAL, ST_UNKNOWN);
        ch_hdlr_check_video_fmt(pt_svctx, pt_running_svc_req);
        ch_hdlr_check_audio_fmt(pt_svctx, pt_running_svc_req);
    }
#endif    
#endif    

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_get_svl_rec_by_req
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_get_svl_rec_by_req(
                    CH_SVC_REQ_T*               pt_svc_req,
                    SVL_REC_T*                  pt_svl_rec
                    )
{
    HANDLE_T  h_svl          = NULL_HANDLE;
    UINT32    ui4_ver_id     = SVL_NULL_VER_ID;
    UINT16    ui2_svl_id     = SVL_NULL_ID;
    UINT16    ui2_svl_rec_id = SVL_NULL_REC_ID;
    INT32     i4_ret;
#ifdef SYS_MHP_SUPPORT
    if (pt_svc_req->t_src_info.u_info.t_svl.t_silent.b_enable == TRUE) {
        ui2_svl_rec_id = pt_svc_req->t_src_info.u_info.t_svl.t_silent.ui2_svl_rec_id;
        ui2_svl_id = pt_svc_req->t_src_info.u_info.t_svl.t_silent.ui2_svc_lst_id;
    } else {
        ui2_svl_rec_id = pt_svc_req->t_src_info.u_info.t_svl.ui2_svl_rec_id;
        ui2_svl_id = pt_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id;
    }
#else
    ui2_svl_id = pt_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id;

    if (pt_svc_req->t_src_info.u_info.t_svl.t_silent.b_enable == TRUE) {
        ui2_svl_rec_id = pt_svc_req->t_src_info.u_info.t_svl.t_silent.ui2_svl_rec_id;
    } else {
        ui2_svl_rec_id = pt_svc_req->t_src_info.u_info.t_svl.ui2_svl_rec_id;
    }
#endif
    do {
        i4_ret = x_svl_open(ui2_svl_id, NULL, NULL, &h_svl);
        if (i4_ret != SVLR_OK) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = x_svl_get_rec (h_svl, ui2_svl_rec_id,  pt_svl_rec, &ui4_ver_id);
        if (i4_ret != SVLR_OK) {
            i4_ret = SVCTXR_NO_RECORD;
            break;
        }

        i4_ret = SVCTXR_OK;
    } while (0);

    if (x_handle_valid(h_svl) == TRUE) {
        x_svl_close(h_svl);
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_stop_svc
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 ch_hdlr_stop_svc (
                    SVCTX_T*                    pt_svctx,
                    BOOL                        b_stop_only
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = NULL;
    CH_SVC_REQ_T* pt_pending_svc_req = NULL;
    INT32         i4_ret             = SVCTXR_OK;

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    pt_pending_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_pending_svc_req;

    if (pt_running_svc_req == NULL) {
        return SVCTXR_ALREADY_STOPPED;
    }

#if 1  /* for MW_NINJA_CHANGE_CHANNEL */
    if (pt_pending_svc_req != NULL && pt_pending_svc_req->t_src_info.b_ninja_change)
    {
        do {
            SVL_REC_T t_running_svl_rec;
            SVL_REC_T t_pending_svl_rec;

            if (pt_running_svc_req->t_parent.e_src_type != SRC_TYPE_BRDCST_SVL
                || pt_pending_svc_req->t_parent.e_src_type != SRC_TYPE_BRDCST_SVL) {
                break;
            }

            i4_ret = ch_hdlr_get_crnt_svl_rec(pt_running_svc_req, &t_running_svl_rec);
            if (i4_ret != SVCTXR_OK) {
                break;
            }

            i4_ret = _ch_hdlr_get_svl_rec_by_req(pt_pending_svc_req, &t_pending_svl_rec);
            if (i4_ret != SVCTXR_OK) {
                break;
            }

            if (t_running_svl_rec.uheader.t_rec_hdr.ui2_tsl_id == t_pending_svl_rec.uheader.t_rec_hdr.ui2_tsl_id
                && t_running_svl_rec.uheader.t_rec_hdr.ui2_tsl_rec_id == t_pending_svl_rec.uheader.t_rec_hdr.ui2_tsl_rec_id)
            {
                /* will really do process for ninja change as below */
            }
            else
            {
                /* NOT within a same TS, so not a real ninja change. */
                pt_pending_svc_req->t_src_info.b_ninja_change = FALSE;
                pt_pending_svc_req->t_parent.ui4_stream_mode  = 0;

                /* Sometime MHEG5 asks such a fake ninja change to keep i-frame
                 * playing without interruption during service change. */
                pt_pending_svc_req->t_parent.b_enable_i_frame = pt_running_svc_req->t_parent.b_enable_i_frame;
                break;
            }

            /* Do process for ninja change then return OK directly */
            pt_pending_svc_req->t_strm_reqs[0] = pt_running_svc_req->t_strm_reqs[0];
            pt_pending_svc_req->t_strm_reqs[1] = pt_running_svc_req->t_strm_reqs[1];

            pt_pending_svc_req->pt_player = pt_running_svc_req->pt_player;
            pt_running_svc_req->pt_player = NULL;
            pt_pending_svc_req->pt_crnt_event = pt_running_svc_req->pt_crnt_event;
            pt_running_svc_req->pt_crnt_event = NULL;
            pt_pending_svc_req->pt_rating_chker = pt_running_svc_req->pt_rating_chker;
            pt_running_svc_req->pt_rating_chker = NULL;
            pt_pending_svc_req->pt_strm_fmt_chker = pt_running_svc_req->pt_strm_fmt_chker;
            pt_running_svc_req->pt_strm_fmt_chker = NULL;

            pt_pending_svc_req->pt_player->b_reuse_pipe = TRUE;
            pt_pending_svc_req->pt_player->b_ninja_change = TRUE;

            pt_pending_svc_req->pt_player->e_conn_cond = CM_COND_CLOSED;
            pt_pending_svc_req->pt_player->h_conn_bak  = pt_pending_svc_req->pt_player->h_conn;

            pt_pending_svc_req->t_parent.b_enable_i_frame = pt_running_svc_req->t_parent.b_enable_i_frame;

            return SVCTXR_OK;

        }while (0);
    }
#endif

    pt_running_svc_req->b_in_scdb_empty_check_delay = FALSE;
    svctx_scdb_chk_timer_stop(pt_svctx);
    if (pt_svctx->e_state != SVCTX_COND_READY) {
        if (pt_svctx->pt_pending_svc_req != NULL) {
            ch_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_INTERRUPTED,
                        ST_UNKNOWN
                        );
        } else {
            ch_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_IN_STOPPING,
                        ST_UNKNOWN
                        );
        }
    }

    if (pt_running_svc_req->pt_player)
    {
        pt_running_svc_req->pt_player->b_reuse_pipe = FALSE;

        if (pt_pending_svc_req != NULL && b_stop_only == FALSE) {
            do {
                SVL_REC_T t_running_svl_rec;
                SVL_REC_T t_pending_svl_rec;

                if (pt_running_svc_req->t_parent.e_src_type != SRC_TYPE_BRDCST_SVL
                        || pt_pending_svc_req->t_parent.e_src_type != SRC_TYPE_BRDCST_SVL) {
                    break;
                }

                i4_ret = ch_hdlr_get_crnt_svl_rec(pt_running_svc_req, &t_running_svl_rec);
                if (i4_ret != SVCTXR_OK) {
                    break;
                }

                i4_ret = _ch_hdlr_get_svl_rec_by_req(pt_pending_svc_req, &t_pending_svl_rec);
                if (i4_ret != SVCTXR_OK) {
                    break;
                }

                /* only running and pending(next request) are all digital, the "reuse pipe"
                 * mechanism will be applied.
                 */
                if (t_running_svl_rec.uheader.t_rec_hdr.e_brdcst_type == BRDCST_TYPE_ANALOG
                        || t_pending_svl_rec.uheader.t_rec_hdr.e_brdcst_type == BRDCST_TYPE_ANALOG) {
                    break;
                }

                if (t_running_svl_rec.uheader.t_rec_hdr.ui2_tsl_id == t_pending_svl_rec.uheader.t_rec_hdr.ui2_tsl_id
                        && t_running_svl_rec.uheader.t_rec_hdr.ui2_tsl_rec_id == t_pending_svl_rec.uheader.t_rec_hdr.ui2_tsl_rec_id
                        && pt_running_svc_req->t_src_info.ui2_on_id == pt_pending_svc_req->t_src_info.ui2_on_id
                        && pt_running_svc_req->t_src_info.ui2_ts_id == pt_pending_svc_req->t_src_info.ui2_ts_id
                        && pt_running_svc_req->t_src_info.ui4_freq  == pt_pending_svc_req->t_src_info.ui4_freq
                        )
                {
#ifdef MW_TV_AV_BYPASS_SUPPORT
                    SVCTX_T* pt_lead;
                    pt_lead = svctx_mgr_get_link_lead(ACTIVE_LINK_NOW, pt_svctx->pt_running_svc_req->e_src_type);

                    if (pt_svctx->b_fake_disc) 
                    {
                        /* Check the consistency between lead and associated services */
                        if (pt_lead) {
                            pt_running_svc_req->pt_player->b_reuse_pipe = !pt_lead->b_fake_disc;
                        }
                    }
                    else 
                    {
                        if (pt_lead) {
                            if (((CH_SVC_REQ_T*)pt_lead->pt_running_svc_req)->pt_player->e_conn_cond == CM_COND_CONNECTED) {
                                pt_running_svc_req->pt_player->b_reuse_pipe = !svctx_is_reuse_pipe_disabled();
                            }
                        } else {
                            pt_running_svc_req->pt_player->b_reuse_pipe = !svctx_is_reuse_pipe_disabled();
                        }
                    }
#else
                    pt_running_svc_req->pt_player->b_reuse_pipe = TRUE;
#endif
                }
            } while (0);
        }
    }

    if (pt_running_svc_req->pt_strm_fmt_chker)
    {
        i4_ret = strm_fmt_chker_close(pt_running_svc_req->pt_strm_fmt_chker);
        if (i4_ret == SVCTXR_OK) {
            pt_running_svc_req->pt_strm_fmt_chker = NULL;
        }
    }

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

    if (pt_running_svc_req->pt_player)
    {
#if 1  /* for MW_NINJA_CHANGE_CHANNEL */
        pt_running_svc_req->pt_player->b_ninja_change = FALSE;
#endif

        i4_ret = player_stop(pt_running_svc_req->pt_player);
        if (i4_ret != SVCTXR_OK) {
            return i4_ret;
        }

        if (pt_running_svc_req->pt_player->b_reuse_pipe == FALSE) {
            i4_ret = player_close(pt_running_svc_req->pt_player);
            if (i4_ret == SVCTXR_OK) {
                pt_running_svc_req->pt_player = NULL;
            }
        } else {
            if (pt_pending_svc_req)
            {
                pt_pending_svc_req->pt_player = pt_running_svc_req->pt_player;
                pt_running_svc_req->pt_player = NULL;
                i4_ret = SVCTXR_OK;
            }
        }
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_check_svc_block
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 ch_hdlr_check_svc_block (
                    SVCTX_T*                    pt_svctx
                    )
{
    CH_SVC_REQ_T*   pt_running_svc_req = NULL;
    BOOL            b_old_is_blocked;
    INT32           i4_ret             = SVCTXR_OK;
    SRC_OPER_TYPE_T e_src_oper_type;
    SNK_OPER_TYPE_T e_snk_oper_type;

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    b_old_is_blocked = pt_svctx->b_is_blocked;
    pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        return SVCTXR_WRONG_STATE;
    }

    if (b_old_is_blocked == _ch_hdlr_is_blocked_service(
                                pt_svctx,
                                pt_running_svc_req,
                                SVC_BLOCK_CHECK_ALL,
                                TRUE)) {
        return i4_ret;
    }
#ifdef MW_ISDB_SUPPORT
    if (pt_svctx->b_is_brdcsting_svc == FALSE)
    {
        if (pt_svctx->b_is_blocked == FALSE)
        {
            ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx,
                                              SVCTX_NTFY_CODE_NON_BRDCSTING_SVC,
                                              ST_UNKNOWN);
        }
        pt_svctx->b_is_blocked = TRUE;
        pt_svctx->b_block_notified = TRUE;
        return SVCTXR_NO_ACTION;
    }
      /* check SBTVD : 1 - seg, Don't play */
    if( SRC_TYPE_BRDCST_FREQ != pt_running_svc_req->t_parent.e_src_type )  /* Ignore FM Radio case */
    {
        SVL_REC_T t_svl_rec = {0};

        i4_ret = ch_hdlr_get_crnt_svl_rec(
                    pt_running_svc_req,
                    &t_svl_rec
                    );

        x_dbg_stmt("ch_hdlr_get_crnt_svl_rec: ret = %d, svc layer = %d\r\n", i4_ret, t_svl_rec.u_data.t_isdb.e_svc_layer);
        
        if (i4_ret == SVCTXR_OK) {
            if (t_svl_rec.uheader.t_rec_hdr.ui4_option_mask & SB_VOPT_IS_ONE_SEGMENT) {

                pt_svctx->b_is_blocked = FALSE;
                
                svctx_set_av_status(pt_svctx, SVC_AV_STATUS_NO_AUDIO_VIDEO);
                
                ch_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_NO_AUDIO_VIDEO_SVC,
                        ST_UNKNOWN
                        );
                
                return SVCTXR_NO_ACTION;
            }
        }
    }
#endif

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

    if (pt_svctx->b_is_blocked == TRUE) {
        i4_ret = _ch_hdlr_block_service(pt_svctx, pt_running_svc_req);
    } else {
        i4_ret = _ch_hdlr_unblock_service(pt_svctx, pt_running_svc_req);
    }

    return i4_ret;
}
#ifdef MW_ISDB_SUPPORT
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_check_non_brdcsting_svc
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 ch_hdlr_check_non_brdcsting_svc (
                    SVCTX_T*                    pt_svctx,
                    BOOL                        b_brdcsting
                    )
{
    CH_SVC_REQ_T*   pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    BOOL            b_old_is_brdcsting = pt_svctx->b_is_brdcsting_svc;
    INT32           i4_ret             = SVCTXR_OK;
    SRC_OPER_TYPE_T e_src_oper_type;
    SNK_OPER_TYPE_T e_snk_oper_type;

    if (pt_running_svc_req == NULL) {
        return SVCTXR_WRONG_STATE;
    }

    if (b_old_is_brdcsting == b_brdcsting && (
                    b_brdcsting ||
                    (pt_svctx->b_is_blocked && pt_svctx->b_block_notified))) {
        return i4_ret;
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

    pt_svctx->b_is_brdcsting_svc = b_brdcsting;

    if (pt_svctx->b_is_brdcsting_svc == FALSE)
    {
        /* fake signal lock */
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOCKED, ST_UNKNOWN);
        /* fake block */
        pt_svctx->b_is_blocked = TRUE;
        pt_svctx->b_block_notified = TRUE;
        svctx_mute_video(pt_svctx, pt_svctx->t_video_mute.b_enable);
        ch_hdlr_set_video_plane(pt_svctx, SCC_VID_BLANK);
        i4_ret = _ch_hdlr_block_service(pt_svctx, pt_running_svc_req);
        /* fake event update */
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_CRNT_EVN_NOT_READY, ST_UNKNOWN);

        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_NON_BRDCSTING_SVC, ST_UNKNOWN);
    }
    else
    {
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_BRDCSTING_SVC, ST_UNKNOWN);
        i4_ret = ch_hdlr_check_svc_block(pt_svctx);
    }

    return i4_ret;
}
#endif
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_stop_strm
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
 *-----------------------------------------------------------------------------*/
INT32 ch_hdlr_stop_strm (
                    SVCTX_T*                    pt_svctx,
                    SVCTX_MSG_T*                pt_msg
                    )
{
    INT32         i4_ret                = SVCTXR_OK;
    STREAM_TYPE_T e_strm_type           = pt_msg->u.t_stop_strm_req.e_type;
    CH_SVC_REQ_T* pt_running_svc_req    = NULL;
    STRM_REQ_T*   pt_strm_req           = NULL;
    STRM_REQ_T**  ppt_pending_strm_req  = NULL;

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req  = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL) {
        return SVCTXR_ALREADY_STOPPED;
    }

    /* [MS10] To stop the substream type AD, just cancel the mixed index. */
    if (pt_msg->u.t_stop_strm_req.b_aux_audio)
    {
        STRM_REQ_T* pt_aud_req = &(pt_running_svc_req->t_strm_reqs[ST_AUDIO]);
        ASH_AUD_DEC_DDC_ID_T e_ddc_id = ch_hdlr_substrm_idx_cm2sm(AUDIO_INDEX_UNKNOW);
        if (pt_aud_req->b_mix_substrm 
                && pt_aud_req->ui1_mixed_substrm_idx != AUDIO_INDEX_UNKNOW)
        {
            i4_ret = svctx_set_stream_attr(
                    pt_svctx,
                    pt_aud_req->h_stream,
                    SM_ASH_SET_TYPE_MS10_MIX_ID,
                    &e_ddc_id,
                    sizeof(ASH_AUD_DEC_DDC_ID_T));
            if (SVCTXR_OK == i4_ret)
            {
                DBG_INFO((_INFO_HEADER"[MS10] set substream idx to UNKNOW to stop the AD"));
                pt_aud_req->ui1_mixed_substrm_idx = AUDIO_INDEX_UNKNOW;
            }
            if (pt_msg->u.t_stop_strm_req.t_sync.pf_nfy)
            {
                (pt_msg->u.t_stop_strm_req.t_sync.pf_nfy)(pt_msg->u.t_stop_strm_req.t_sync.pv_tag);
                pt_msg->u.t_stop_strm_req.t_sync.pf_nfy = NULL;
                pt_msg->u.t_stop_strm_req.t_sync.pv_tag = NULL;
            }
            return i4_ret;
        }
    } /* [MS10] END */

    switch(e_strm_type) {
    case ST_CLOSED_CAPTION:
        pt_running_svc_req->t_cc_filter.e_cc_type         = SM_CC_TYPE_IGNORE;
        pt_running_svc_req->t_cc_filter.u.ui1_service_num = 0xFF;
        break;
#ifdef MW_ISDB_SUPPORT
    case ST_ISDB_CAPTION:
        break;
    case ST_ISDB_TEXT:
        break;
#endif
    case ST_AUDIO:
        if (!pt_msg->u.t_stop_strm_req.b_aux_audio)
        {
            ch_hdlr_reset_audio_info(pt_svctx, pt_running_svc_req);
        }
        break;
    case ST_VIDEO:
        ch_hdlr_reset_video_info(pt_svctx, pt_running_svc_req);
        _ch_hdlr_close_text_strms(
                    pt_svctx,
                    pt_running_svc_req,
#ifdef MW_ISDB_SUPPORT
                    ST_MASK_CLOSED_CAPTION|ST_MASK_ISDB_CAPTION|ST_MASK_ISDB_TEXT|ST_MASK_SUBTITLE
#else
                    ST_MASK_CLOSED_CAPTION|ST_MASK_SUBTITLE
#endif
                    );
    case ST_SUBTITLE:
        break;
    default:
        return SVCTXR_NOT_SUPPORT;
    }

    if (pt_msg->u.t_stop_strm_req.b_aux_audio)
    {
        pt_strm_req          = &(pt_running_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX]);
        ppt_pending_strm_req = &(pt_running_svc_req->pt_pending_strm_reqs[STRM_REQ_AD_INDEX]);
    }
    else
    {
        pt_strm_req          = &(pt_running_svc_req->t_strm_reqs[e_strm_type]);
        ppt_pending_strm_req = &(pt_running_svc_req->pt_pending_strm_reqs[e_strm_type]);
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
#ifdef MW_TV_AV_BYPASS_SUPPORT
        i4_ret = ch_hdlr_try_closing_stream(pt_svctx, pt_strm_req->h_stream);
        if (SVCTXR_OK == i4_ret || SVCTXR_QUEUED == i4_ret || SVCTXR_INV_HANDLE == i4_ret) {
#else
        i4_ret = x_sm_close_stream(pt_strm_req->h_stream);
        if (i4_ret == SMR_OK || i4_ret == SMR_PENDING || i4_ret == SMR_INV_HANDLE) {
#endif
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

    _ch_hdlr_reset_ca_info(pt_strm_req);
    _ch_hdlr_reset_cp_info(pt_strm_req);

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_internal_msg
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 ch_hdlr_internal_msg (
                    SVCTX_T*                    pt_svctx,
                    SVCTX_MSG_T*                pt_msg
                    )
{
    INT32 i4_ret = SVCTXR_OK;

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    switch(pt_msg->ui1_msg_id) {
    case _SVCTX_MSG_STRM_NTFY:
        i4_ret = _ch_hdlr_handle_strm_msg(
                    pt_svctx,
                    &pt_msg->u.t_strm
                    );
        break;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    case _SVCTX_MSG_QUEUED_OPEN_STRM_NTFY:
    {
        SM_COND_T e_cond = player_get_sess_cond(((CH_SVC_REQ_T*) pt_svctx->pt_running_svc_req)->pt_player);
        BOOL      b_play = (SM_COND_STARTING == e_cond || SM_COND_STARTED == e_cond) ? TRUE : ((BOOL) (UINT32) pt_msg->pv_tag);

        ch_hdlr_try_opening_stream(
            pt_svctx,
            &(((CH_SVC_REQ_T*) pt_svctx->pt_running_svc_req)->t_strm_reqs[pt_msg->u.i4_idx]),
            b_play);

        break;
    }

    case _SVCTX_MSG_QUEUED_CLOSE_STRM_NTFY:
        ch_hdlr_try_closing_stream(
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
 *      ch_hdlr_get_crnt_svl_rec
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 ch_hdlr_get_crnt_svl_rec(
                    CH_SVC_REQ_T*               pt_running_svc_req,
                    SVL_REC_T*                  pt_svl_rec
                    )
{
    HANDLE_T  h_svl          = NULL_HANDLE;
    UINT32    ui4_ver_id     = SVL_NULL_VER_ID;
    UINT16    ui2_svl_rec_id = SVL_NULL_REC_ID;
    INT32     i4_ret;

    if (pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    i4_ret = _ch_hdlr_get_svl(pt_running_svc_req, &h_svl);
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
 *      ch_hdlr_get_crnt_tsl_rec
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 ch_hdlr_get_crnt_tsl_rec(
                    CH_SVC_REQ_T*               pt_running_svc_req,
                    TSL_REC_T*                  pt_tsl_rec
                    )
{
    UINT32    ui4_ver_id = TSL_NULL_VER_ID;
    INT32     i4_ret     = SVCTXR_OK;
    SVL_REC_T t_svl_rec;
    HANDLE_T  h_tsl;

    if (pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    i4_ret = ch_hdlr_get_crnt_svl_rec(
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
 *      ch_hdlr_close_streams_as_no_signal
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 ch_hdlr_close_streams_as_no_signal(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    INT32 i4_ret;

    if (pt_svctx == NULL || pt_running_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    if ((pt_svctx->ui1_stop_audio_as_no_signal & MAKE_BIT_MASK_8(pt_running_svc_req->t_parent.e_src_type)) > 0) {
        if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) == TRUE) {
#ifdef MW_TV_AV_BYPASS_SUPPORT
            i4_ret = ch_hdlr_try_closing_stream(pt_svctx, pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream);
            if (SVCTXR_OK == i4_ret || SVCTXR_QUEUED == i4_ret) {
#else
            i4_ret = x_sm_close_stream(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream);
            if (i4_ret == SMR_OK || i4_ret == SMR_PENDING) {
#endif
                pt_running_svc_req->t_strm_reqs[ST_AUDIO].e_strm_cond = SM_COND_CLOSING;
            }
        }
        if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream) == TRUE &&
            pt_running_svc_req->e_brdcst_type != BRDCST_TYPE_ANALOG) {
#ifdef MW_TV_AV_BYPASS_SUPPORT
            i4_ret = ch_hdlr_try_closing_stream(pt_svctx, pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream);
            if (SVCTXR_OK == i4_ret || SVCTXR_QUEUED == i4_ret) {
#else
            i4_ret = x_sm_close_stream(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream);
            if (i4_ret == SMR_OK || i4_ret == SMR_PENDING) {
#endif
                pt_running_svc_req->t_strm_reqs[ST_VIDEO].e_strm_cond = SM_COND_CLOSING;
            }
        }
        if (pt_svctx->b_ad_enabled &&
            x_handle_valid(pt_running_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX].h_stream) == TRUE) {
#ifdef MW_TV_AV_BYPASS_SUPPORT
            i4_ret = ch_hdlr_try_closing_stream(pt_svctx, pt_running_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX].h_stream);
            if (SVCTXR_OK == i4_ret || SVCTXR_QUEUED == i4_ret) {
#else
            i4_ret = x_sm_close_stream(pt_running_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX].h_stream);
            if (i4_ret == SMR_OK || i4_ret == SMR_PENDING) {
#endif
                pt_running_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX].e_strm_cond = SM_COND_CLOSING;
            }
        }
    }

    _ch_hdlr_close_text_strms(
                    pt_svctx,
                    pt_running_svc_req,
#ifdef MW_ISDB_SUPPORT
                    ST_MASK_CLOSED_CAPTION|ST_MASK_ISDB_CAPTION|ST_MASK_ISDB_TEXT|ST_MASK_SUBTITLE
#else
                    ST_MASK_CLOSED_CAPTION|ST_MASK_SUBTITLE
#endif
                    );

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_check_signal_loss_priority
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
BOOL ch_hdlr_check_signal_loss_priority(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_svc_req,
                    BOOL                        b_signal_loss_and_check_block
                    )
{
    SVC_BLOCK_COND_T        e_block_cond;
    SVC_BLOCK_CHECK_GUDIE_T e_check_guide;
    BOOL                    b_check_block = FALSE;

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return FALSE;
    }

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
                && _ch_hdlr_is_blocked_service(
                    pt_svctx,
                    pt_svc_req,
                    e_check_guide,
                    TRUE) == TRUE) {
            _ch_hdlr_block_service(pt_svctx, pt_svc_req);
            pt_svctx->e_signal_status = SIGNAL_STATUS_LOSS;
        } else {
            /* in this API for SVCTX_NTFY_CODE_SIGNAL_LOSS, it will set SIGNAL_STATUS_LOSS */
            ch_hdlr_invoke_client_svc_nfy_fct(
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
 *      ch_hdlr_is_blocked_source
 * Description
 *      The API is used to check the input source is blocked or not
 * Input arguments
 *      pt_svctx             the pointer points to an SVCTX_T object
 *      pt_svc_req           the pointer points to a svc request to check
 * Output arguments
 *      None
 * Returns
 *      TRUE            blocked source
 *      FALSE           not blocked source
 *-----------------------------------------------------------------------------*/
BOOL ch_hdlr_is_blocked_source(
                    SVCTX_T*                    pt_svctx,
                    SVC_REQ_T*                  pt_svc_req
                    )
{
    if (pt_svctx == NULL || pt_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return FALSE;
    }

#ifdef SVCTX_SPLIT_SUPPORT
    if (pt_svctx->pf_block_check_fct (
                    SVC_BLOCK_CHECK_USER_BLOCK_INP,
                    ((CH_SVC_REQ_T*)pt_svc_req)->t_src_info.s_src_grp_name,
                    NULL,
                    NULL,
                    pt_svctx->pf_block_check_fct_tag
                    ) == SVC_BLOCK_COND_USER_BLOCK_INP) {
#else
    if (pt_svctx->pf_block_check_fct (
                    SVC_BLOCK_CHECK_USER_BLOCK_INP,
                    ((CH_SVC_REQ_T*)pt_svc_req)->t_src_info.s_src_grp_name,
                    NULL,
                    NULL
                    ) == SVC_BLOCK_COND_USER_BLOCK_INP) {
#endif
        return TRUE;
    }

    return FALSE;
}

#ifdef MW_TV_AV_BYPASS_SUPPORT
INT32 ch_hdlr_try_closing_stream(
    SVCTX_T*    pt_svctx,
    HANDLE_T    h_stream)
{
    INT32           i4_ret;
    STRM_REQ_T*     pt_strm_req;
    VAR_INFO_T      t_var_info;
    CH_SVC_REQ_T*   pt_ch_svc_req;
    STREAM_TYPE_T   e_strm_type;
    INT32           i4_idx;

    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    i4_ret = SVCTXR_FAIL;

    pt_ch_svc_req = (CH_SVC_REQ_T*) pt_svctx->pt_running_svc_req;

    if (pt_ch_svc_req)
    {
        i4_ret = _ch_hdlr_get_strm_req_from_handle(
            pt_svctx,
            h_stream,
            &pt_strm_req,
            &e_strm_type);

        if (SVCTXR_OK == i4_ret)
        {
            i4_idx = pt_strm_req - pt_ch_svc_req->t_strm_reqs;

            if (i4_idx > STRM_REQ_AD_INDEX)
            {
                DBG_ABORT(DBG_MOD_SVCTX);
            }

            t_var_info.ui1_msg_id                = _SVCTX_MSG_QUEUED_CLOSE_STRM_NTFY;
            t_var_info.e_var_type                = VAR_TYPE_STREAM;
            t_var_info.e_op_type                 = OP_TYPE_CLOSE;
            t_var_info.u.t_strm_info.h_stream    = h_stream;
            t_var_info.u.t_strm_info.e_strm_type = e_strm_type;
            t_var_info.u.t_strm_info.i4_idx      = i4_idx;
            t_var_info.u.t_strm_info.b_aux_audio = pt_strm_req->b_aux_audio;
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
                pt_strm_req->e_strm_cond    = SM_COND_CLOSING;
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
                    "{SVCTX_MGR} SVCTX (%s) Closing %s stream CONTINUED.\r\n",
                    pt_svctx->s_name,
                    s_g_stream[t_var_info.u.t_strm_info.e_strm_type]));
#endif
                if (!IS_SVCTX_ASSOCIATE(pt_svctx) &&
                    (h_stream == pt_ch_svc_req->t_strm_reqs[ST_VIDEO].h_stream) &&
                    (svctx_is_scrambled_strm(&pt_strm_req->t_ca) == FALSE) &&
                    (pt_svctx->e_signal_status == SIGNAL_STATUS_LOCKED) &&
                    (pt_svctx->b_is_blocked == FALSE))
                {
                    pt_svctx->ui4_frz_frozen_strm_mask &= ~ST_MASK_VIDEO;
                    svctx_set_video_plane(pt_svctx, pt_svctx->e_default_vid_mode);
                }

                svctx_cli_print_stream_component_idx(pt_svctx, 1, t_var_info.u.t_strm_info.e_strm_type, h_stream, 0, 0, __FILE__, __LINE__);
                CLI_FUNC_CALL_TRACE_FOR_STRM("x_sm_close_stream", e_strm_type);

                i4_ret = x_sm_close_stream(h_stream);
                if (SMR_OK == i4_ret || SMR_PENDING == i4_ret)
                {
                    /* Record audio stream request status */
                    if ((t_var_info.u.t_strm_info.e_strm_type == ST_AUDIO) &&
                        (pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS))
                    {
                        pt_svctx->ui4_prv_stream_cond &= ~MAKE_BIT_MASK_32(ST_AUDIO);
                    }                       

                    pt_strm_req->e_strm_cond    = SM_COND_CLOSING;
                    pt_strm_req->b_close_queued = FALSE;
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

INT32 ch_hdlr_try_opening_stream(
    SVCTX_T*        pt_svctx,
    STRM_REQ_T*     pt_strm_req,
    BOOL            b_auto_play)
{
    INT32           i4_ret;
    INT32           i4_idx;
    VAR_INFO_T      t_var_info;
    CH_SVC_REQ_T*   pt_ch_svc_req;

    if (pt_svctx == NULL || pt_strm_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    i4_ret = SVCTXR_FAIL;

    pt_ch_svc_req = (CH_SVC_REQ_T*) pt_svctx->pt_running_svc_req;

    if (pt_ch_svc_req)
    {
        i4_idx = pt_strm_req - pt_ch_svc_req->t_strm_reqs;

        if (i4_idx > STRM_REQ_AD_INDEX)
        {
            DBG_ABORT(DBG_MOD_SVCTX);
        }

        /* Skip opening ad stream if ad is not enabled */
        if (!pt_svctx->b_ad_enabled &&
            (STRM_REQ_AD_INDEX == i4_idx))
        {
            return SVCTXR_OK;
        }

        t_var_info.ui1_msg_id                = _SVCTX_MSG_QUEUED_OPEN_STRM_NTFY;
        t_var_info.e_var_type                = VAR_TYPE_STREAM;
        t_var_info.e_op_type                 = OP_TYPE_OPEN;
        t_var_info.u.t_strm_info.h_stream    = NULL_HANDLE;
        t_var_info.u.t_strm_info.e_strm_type = (STRM_REQ_AD_INDEX == i4_idx) ? ST_AUDIO : (STREAM_TYPE_T) i4_idx;
        t_var_info.u.t_strm_info.i4_idx      = i4_idx;
        t_var_info.u.t_strm_info.b_aux_audio = pt_strm_req->b_aux_audio;
        t_var_info.u.t_strm_info.b_auto_play = b_auto_play;

        if (FALSE == svctx_mgr_ready_to_proceed(
            ACTIVE_LINK_NOW,
            pt_svctx->pt_running_svc_req->e_src_type,
            pt_svctx,
            &t_var_info))
        {
#ifdef CLI_LVL_ALL
            DBG_SVCTXMGR_MSG((
                "{SVCTX_MGR} SVCTX(%s): Opening %s stream POSTPONED (autoPlay = %d)...\r\n",
                pt_svctx->s_name,
                s_g_stream[t_var_info.u.t_strm_info.e_strm_type],
                b_auto_play));
#endif
            pt_strm_req->e_strm_cond   = SM_COND_OPENING;
            pt_strm_req->b_open_queued = TRUE;

        #if 0
            /* send message */
            t_msg.ui1_msg_id     = _SVCTX_MSG_QUEUED_OPEN_STRM_NTFY;
            t_msg.pf_invoke_func = NULL;
            t_msg.pv_tag         = (VOID*) b_auto_play;
            t_msg.u.pt_strm_req  = pt_strm_req;

            svctx_send_msg(pt_svctx->h_sys_svctx, &t_msg);
        #endif

            i4_ret = SVCTXR_OK;
        }
        else
        {
            if (pt_svctx->b_is_blocked &&
                ((t_var_info.u.t_strm_info.e_strm_type == ST_AUDIO) ||
                 (t_var_info.u.t_strm_info.e_strm_type == STRM_REQ_AD_INDEX)))
            {
                /* Ignore audio open stream request if service is blocked */
                pt_strm_req->e_strm_cond = SM_COND_CLOSED;
                pt_strm_req->e_penging_status = STRM_PENDING_COND_NONE;
                pt_strm_req->b_open_queued = FALSE;
                i4_ret = SVCTXR_OK;
            }
            else
            {
#ifdef CLI_LVL_ALL
                DBG_SVCTXMGR_MSG((
                    "{SVCTX_MGR} SVCTX(%s): Opening %s stream CONTINUED (autoPlay = %d)...\r\n",
                    pt_svctx->s_name,
                    s_g_stream[t_var_info.u.t_strm_info.e_strm_type],
                    b_auto_play));
#endif
                i4_ret = ch_hdlr_open_strm_ex(pt_svctx, pt_strm_req, b_auto_play);

                pt_strm_req->b_open_queued = FALSE;

                if (SMR_OK == i4_ret || SMR_PENDING == i4_ret)
                {
#if MW_TV_AV_BYPASS_SUPPORT
                    if (!IS_SVCTX_ASSOCIATE(pt_svctx))
#endif
                    {
                        /* Record audio stream request status */
                        if (t_var_info.u.t_strm_info.e_strm_type == ST_AUDIO && (STRM_REQ_AD_INDEX != (INT32)i4_idx) )
                        {
                            pt_svctx->t_prv_aud_strm_req = *pt_strm_req;
                            pt_svctx->ui4_prv_stream_cond |= MAKE_BIT_MASK_32(ST_AUDIO);

#ifdef MW_PIP_POP_SUPPORT
                            if (svctx_is_pap(pt_svctx))
                            {
                                /* clean other last oprator */
                                INT32 i = 0;
                                while (1)
                                {
                                    ACTIVE_LINK_ENTRY_T* pt_link;
                                    pt_link = svctx_mgr_get_link_obj_all(ACTIVE_LINK_NOW, i++);
                                
                                    if (!pt_link)
                                    {
                                        break;
                                    }

                                    if ( (pt_link->pt_worker != pt_svctx) 
#if MW_TV_AV_BYPASS_SUPPORT
                                         && !IS_SVCTX_ASSOCIATE(pt_link->pt_worker)
#endif
                                        )
                                    {
                                        pt_link->pt_worker->ui4_prv_stream_cond &= ~MAKE_BIT_MASK_32(ST_AUDIO);
                                    }
                                }
                            }
#endif
                        }
                    }                    
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
#endif

/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_handle_cm_disconnected
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 ch_hdlr_handle_cm_disconnected(
                    SVCTX_T*                    pt_svctx,
                    BOOL                        b_signal_loss
                    )
{
    if (pt_svctx == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s(), pt_running_svc_req is NULL\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    pt_svctx->e_state = SVCTX_COND_PENDING;

    if (b_signal_loss == TRUE) {
        crnt_evn_stop_query(
                    ((CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req)->pt_crnt_event
                    );
        ch_hdlr_check_signal_loss_priority(
                    pt_svctx,
                    (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req,
                    TRUE
                    );

        if (pt_svctx->b_is_blocked) {
            ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOSS_AS_BLOCKED, ST_UNKNOWN);
        }

        pt_svctx->e_signal_status = SIGNAL_STATUS_LOSS;
    }

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_is_first_ch_surf
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *-----------------------------------------------------------------------------*/
BOOL ch_hdlr_is_first_ch_surf(VOID)
{
    return ((ui1_ch_suf_ind > 1) ? FALSE : TRUE);
}

/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_is_valid_for_ad_select
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *-----------------------------------------------------------------------------*/
BOOL ch_hdlr_is_valid_for_ad_select(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    return (pt_svctx->b_ad_enabled 
            && ch_hdlr_is_suitable_for_ad(pt_svctx, pt_running_svc_req));
}

/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_is_suitable_for_ad
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *-----------------------------------------------------------------------------*/
BOOL ch_hdlr_is_suitable_for_ad(
                    SVCTX_T*        pt_svctx, 
                    CH_SVC_REQ_T*   pt_running_svc_req
                    )
{
    STRM_REQ_T*         pt_aud_strm_req = NULL;

    ASSERT(pt_svctx != NULL, ("%s(), pt_svctx is NULL",__func__));

    if (!pt_running_svc_req)
    {
        return FALSE;
    }

    pt_aud_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_AUDIO]);

    if (x_handle_valid(pt_aud_strm_req->h_stream) == FALSE
            || pt_aud_strm_req->e_strm_cond != SM_COND_STARTED)
    {
        DBG_INFO((_INFO_HEADER"Ad is not selectable when main audio not opened\r\n"));
        return FALSE;
    }
    /* Dolby MS10 Certification:
     * Skip selecting AD stream when main audio format is dual_mono */
    if ((pt_aud_strm_req->e_req_type == STRM_REQ_SCDB) &&
        (pt_aud_strm_req->u.t_scdb.t_comp_id.e_type == ST_AUDIO) &&
        (pt_aud_strm_req->u.t_scdb.t_rec.u.t_audio_mpeg.e_aud_fmt == AUD_FMT_DUAL_MONO))
    {

        DBG_INFO((_INFO_HEADER"Ad is not selectable for main audio in dual_mono.\r\n"));
        return FALSE;
    }
    if (pt_svctx->t_audio_info.e_channels == ASH_CHANNELS_DUAL_MONO)
    {
        DBG_INFO((_INFO_HEADER"Ad is not selectable for main audio in dual_mono.\r\n"));
        return FALSE;
    }

    /* Not to select ad if main's format uncertained. */
    if (pt_svctx->t_audio_info.e_channels == ASH_CHANNELS_UNKNOWN)
    {
        DBG_INFO((_INFO_HEADER"Ad is not selectable for unknown format of main audio.\r\n"));
        return FALSE;
    }

    DBG_INFO((_INFO_HEADER"Ad is selectable.\r\n"));
    return TRUE;
}
 
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_is_dtv
 * Description
 *      Tell whether the service is digtal tv(TRUE) or not(FALSE).
 *-----------------------------------------------------------------------------*/
BOOL ch_hdlr_is_dtv(
             CH_SVC_REQ_T* pt_svc_req
             )
{
    ASSERT(pt_svc_req != NULL, ("pt_svc_req is NULL!\r\n"));
    return (pt_svc_req->e_brdcst_type != BRDCST_TYPE_ANALOG 
            && pt_svc_req->e_brdcst_type != BRDCST_TYPE_FMRDO);
}

#if 0 /* disabled temporarily to avoid Klockworc warning */
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_is_atv
 * Description
 *      Tell whether the service is analogue tv(TRUE) or not(FALSE).
 *-----------------------------------------------------------------------------*/
BOOL ch_hdlr_is_atv(
             CH_SVC_REQ_T* pt_svc_req
             )
{
    ASSERT(pt_svc_req != NULL, ("pt_svc_req is NULL!\r\n"));
    return (pt_svc_req->e_brdcst_type == BRDCST_TYPE_ANALOG);
}
#endif 
