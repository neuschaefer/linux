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
 * $RCSfile: ch_hdlr_api.c,v $
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
#include "svctx/ch_hdlr/ch_hdlr.h"
#include "svctx/ch_hdlr/ch_hdlr_cli.h"
#include "time_msrt/x_time_msrt.h"
#include "svctx/svctx_dbg.h"
#include "svl/x_svl.h"
#include "tsl/x_tsl.h"
#include "strm_mngr/sbtl_hdlr/u_sm_sbtl_hdlr.h"
#include "strm_mngr/cc_isdb_hdlr/u_sm_cc_isdb_hdlr.h"
#include "inc/u_device_type.h"
#include "svctx/x_svctx_sm_broker.h"
#include "conn_mngr/fm_radio/u_fm_radio.h"
#include "conn_mngr/x_cm.h"
#include "svctx/svctx_cli.h"
#include "svl_bldr/u_sb.h"
#ifdef MW_TV_AV_BYPASS_SUPPORT
#include "util/x_util_scdb.h"
#include "strm_mngr/bypass_hdlr/u_sm_bypass_hdlr.h"
#include "strm_mngr/sess_gnrc_hdlr/u_sm_sess_gnrc.h"
#include "svctx/svctx_mgr.h"
#include "scdb/scdb_api.h"
#endif
#include "svctx/svctx_rikstv.h"
#if 1 /* Xia : CR - DTV00134423 */
#include "res_mngr/drv/x_aud_dec.h"
#endif

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#define SVC_REQ_AMOUNT_FACTOR   (2)

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

#define SVCTX_SM_BROKER_CMD_BIT_DEVICE_TYPE     MAKE_BIT_MASK_32(0)
#define SVCTX_SM_BROKER_CMD_BIT_SRC_NAME        MAKE_BIT_MASK_32(1)
#define SVCTX_SM_BROKER_CMD_BIT_SVC_LST_ID      MAKE_BIT_MASK_32(2)
#define SVCTX_SM_BROKER_CMD_BIT_SVL_REC_ID      MAKE_BIT_MASK_32(3)

#define SVCTX_SM_BROKER_CMD_BIT_TUNER          (SVCTX_SM_BROKER_CMD_BIT_DEVICE_TYPE| \
                                                SVCTX_SM_BROKER_CMD_BIT_SRC_NAME   | \
                                                SVCTX_SM_BROKER_CMD_BIT_SVC_LST_ID | \
                                                SVCTX_SM_BROKER_CMD_BIT_SVL_REC_ID)
#define STRM_COMP_FILTER_LOOP_BASE              (((UINT16) 0x01) << STRM_COMP_FILTER_LOOP_START_BIT_IDX)
#define STRM_COMP_FILTER_MAKE_INDEX(x,loop)     ((UINT16) (x) | (STRM_COMP_FILTER_LOOP_BASE * (loop)))
/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static VOID _ch_hdlr_reload_cc_strm_as_presenting (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_new_svc_req
                    );
static INT32 _ch_hdlr_get_ad_num_recs(
                    SVCTX_T*                    pt_svctx,
                    UINT16*                     pui2_num_recs
                    );
static INT32 _ch_hdlr_get_ad_recs(
                    SVCTX_T*                    pt_svctx,
                    UINT16                      ui2_num_recs,
                    SCDB_REC_T*                 pt_scdb_rec
                    );
/*-----------------------------------------------------------------------------
 * private variable declarations
 *---------------------------------------------------------------------------*/
static BOOL                     b_g_init               = FALSE;
extern const STREAM_TYPE_T      e_g_ch_strm_types[]; /* QQQQQQ */
 /*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_allocate_svc_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static SVC_REQ_T* _ch_hdlr_allocate_svc_req(
                    VOID
                    )
{
    CH_SVC_REQ_T* pt_svc_req;
    INT32         i4_ret;

    i4_ret = ch_svc_req_allocate(&pt_svc_req);
    if (i4_ret == SVCTXR_OK) {
        return ((SVC_REQ_T*)pt_svc_req);
    } else {
        return NULL;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_free_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static VOID _ch_hdlr_free_strm_req(
                    STRM_REQ_T*                 pt_strm_req
                    )
{
    ch_strm_req_free(pt_strm_req);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_free_svc_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static VOID _ch_hdlr_free_svc_req(
                    SVC_REQ_T*                  pt_svc_req
                    )
{
    ch_svc_req_free((CH_SVC_REQ_T*)pt_svc_req);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_allocate_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static STRM_REQ_T* _ch_hdlr_allocate_strm_req(
                    VOID
                    )
{

    STRM_REQ_T* pt_strm_req;
    INT32       i4_ret;

    i4_ret = ch_strm_req_allocate(&pt_strm_req);
    if (i4_ret == SVCTXR_OK) {
        return pt_strm_req;
    } else {
        return NULL;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_print_svc_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static VOID _ch_hdlr_print_svc_req(
                    SVC_REQ_T*                  pt_svc_req
                    )
{
    ch_svc_req_print_self((CH_SVC_REQ_T*)pt_svc_req);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_print_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static VOID _ch_hdlr_print_strm_req(
                    STRM_REQ_T*                 pt_strm_req
                    )
{
    ch_strm_req_print_self(pt_strm_req);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_resent_nfy_as_replacing
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static VOID _ch_hdlr_resent_nfy_as_replacing(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    if (pt_svctx->e_state == SVCTX_COND_PRESENTING) {
        pt_svctx->e_state = SVCTX_COND_PRESENTING_SELECT_SAME_REQ;
    } else if (pt_svctx->e_state == SVCTX_COND_PENDING) {
        pt_svctx->e_state = SVCTX_COND_PENDING_SELECT_SAME_REQ;
    }

    do {
        ch_hdlr_check_svc_block(pt_svctx);

        if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOCKED) {
            ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SIGNAL_LOCKED, ST_UNKNOWN);
        } else if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) {
            if (ch_hdlr_check_signal_loss_priority(pt_svctx, pt_running_svc_req, FALSE) == TRUE) {
                ch_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_SIGNAL_LOSS,
                        ST_UNKNOWN
                        );
                break;
            }
        } else if (pt_svctx->e_signal_status == SIGNAL_STATUS_DETECTING) {
            ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_SIGNAL_DETECTING,
                    ST_UNKNOWN
                    );
        }

        ch_hdlr_check_scart_bypass(pt_svctx, pt_running_svc_req);
        ch_hdlr_resend_crnt_evn_nfy(pt_svctx, pt_running_svc_req);

        if (pt_svctx->b_is_blocked == TRUE) {
#ifdef MW_ISDB_SUPPORT
            if (pt_svctx->b_is_brdcsting_svc == FALSE)
            {
                pt_svctx->b_block_notified = TRUE;
                ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_NON_BRDCSTING_SVC, ST_UNKNOWN);
                break;
            }
#endif
            ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_SERVICE_BLOCKED, ST_UNKNOWN);
            break;
        }

        /* If it is still under "delay SCDB empty check" after session opened, just don't
         * need to check AV status as replacing service request.
         */
        if (pt_running_svc_req->b_in_scdb_empty_check_delay == FALSE) {
            svctx_set_av_status(pt_svctx, SVC_AV_STATUS_UNKNOWN);
            strm_fmt_chker_init_present_status(pt_running_svc_req->pt_strm_fmt_chker);

            if (pt_svctx->e_state == SVCTX_COND_PRESENTING_SELECT_SAME_REQ) {
                ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_NORMAL, ST_UNKNOWN);
                /* ch_hdlr_check_strms_status(pt_svctx); */ /* removed for DTV00140029 */
                /* to avoid NO_AUDIO_VIDEO_SVC being sent before STREAM_STARTED.
                 * stream status will be checked when CODE_CA/FMT_UPDATE/NO_STREAM is sent.*/
            }

            /* to avoid NO_AUDIO_VIDEO_SVC being sent before STREAM_STARTED. */
            if (SM_COND_STARTED == pt_running_svc_req->pt_player->e_sess_cond) {
                ch_hdlr_check_video_fmt(pt_svctx, pt_running_svc_req);
                ch_hdlr_check_audio_fmt(pt_svctx, pt_running_svc_req);
            }
        } else {
            if (pt_svctx->e_state == SVCTX_COND_PRESENTING_SELECT_SAME_REQ) {
                if (pt_svctx->e_av_status >= SVC_AV_STATUS_SCRAMBLED_AUDIO_VIDEO && 
                    pt_svctx->e_av_status <= SVC_AV_STATUS_SCRAMBLED_VIDEO_NO_AUDIO)
                {
                    svctx_set_av_status(pt_svctx, SVC_AV_STATUS_UNKNOWN);
                    strm_fmt_chker_init_present_status(pt_running_svc_req->pt_strm_fmt_chker);
                    ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_NORMAL, ST_UNKNOWN);
                    /* ch_hdlr_check_strms_status(pt_svctx); */ /* removed for DTV00140029 */
                    ch_hdlr_check_video_fmt(pt_svctx, pt_running_svc_req);
                    ch_hdlr_check_audio_fmt(pt_svctx, pt_running_svc_req);
                }
                else
                {
                svctx_set_av_status(pt_svctx, SVC_AV_STATUS_UNKNOWN);
                ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_NORMAL, ST_UNKNOWN);

                if (TRUE == svctx_is_valid_video_fmt(pt_svctx))
                {
                    ch_hdlr_invoke_client_strm_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE,
                        &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]));
                }

                if (TRUE == svctx_is_valid_audio_fmt(pt_svctx))
                {
                    ch_hdlr_invoke_client_strm_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE,
                        &(pt_running_svc_req->t_strm_reqs[ST_AUDIO]));
                }
            }
        }
        }

        if (pt_svctx->e_cc_avil_cond == SVCTX_CC_AVIL_COND_YES) {
            ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_WITH_CC_AVIL_INDICATOR,
                    ST_CLOSED_CAPTION
                    );
        } else if (pt_svctx->e_cc_avil_cond == SVCTX_CC_AVIL_COND_NO) {
            ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_NO_CC_AVIL_INDICATOR,
                    ST_CLOSED_CAPTION
                    );
        }
    } while (0);

    if (pt_svctx->e_state == SVCTX_COND_PRESENTING_SELECT_SAME_REQ) {
        pt_svctx->e_state = SVCTX_COND_PRESENTING;
    } else if (pt_svctx->e_state == SVCTX_COND_PENDING_SELECT_SAME_REQ) {
        pt_svctx->e_state = SVCTX_COND_PENDING;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_replace_strm_req
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_replace_strm_req (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_new_svc_req,
                    CH_SVC_REQ_T*               pt_old_svc_req
                    )
{
    STREAM_TYPE_T e_strm_type;
    UINT8         ui1_idx;

    for (ui1_idx = 0; ui1_idx < (UINT8) STRM_TYPE_COUNT_WITH_AD; ui1_idx++) {
        e_strm_type = e_g_ch_strm_types[ui1_idx];
        if ((e_strm_type == ST_UNKNOWN) || (e_strm_type >= STRM_TYPE_COUNT))
        {
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
            if (pt_svctx->b_is_blocked == FALSE) {
                switch(e_strm_type) {
                case ST_AUDIO:
                    ch_hdlr_reload_audio_stream(pt_svctx, pt_new_svc_req);
                    break;
                case ST_VIDEO:
                    ch_hdlr_reload_video_stream(pt_svctx, pt_new_svc_req);
                    break;
                case ST_DATA: /* AD index */
                    ch_hdlr_reload_ad_stream(pt_svctx, pt_new_svc_req);
                    break;
                default:
                    break;
                }
            }
            continue;
        }

        if (STRM_REQ_AD_INDEX == (INT32) ui1_idx)
        {
            pt_new_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX]          = pt_old_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX];
            pt_new_svc_req->pt_pending_strm_reqs[STRM_REQ_AD_INDEX] = pt_old_svc_req->pt_pending_strm_reqs[STRM_REQ_AD_INDEX];
            pt_old_svc_req->pt_pending_strm_reqs[STRM_REQ_AD_INDEX] = NULL;
            
            pt_new_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX].h_svctx         = pt_new_svc_req->t_parent.h_svctx;
            pt_new_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX].pf_strm_nfy     = NULL;
            pt_new_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX].pv_strm_nfy_tag = NULL;
        }
        else
        {
            pt_new_svc_req->t_strm_reqs[e_strm_type]          = pt_old_svc_req->t_strm_reqs[e_strm_type];
            pt_new_svc_req->pt_pending_strm_reqs[e_strm_type] = pt_old_svc_req->pt_pending_strm_reqs[e_strm_type];
            pt_old_svc_req->pt_pending_strm_reqs[e_strm_type] = NULL;
            
            pt_new_svc_req->t_strm_reqs[e_strm_type].h_svctx         = pt_new_svc_req->t_parent.h_svctx;
            pt_new_svc_req->t_strm_reqs[e_strm_type].pf_strm_nfy     = pt_new_svc_req->t_parent.pf_svc_nfy;
            pt_new_svc_req->t_strm_reqs[e_strm_type].pv_strm_nfy_tag = pt_new_svc_req->t_parent.pv_svc_nfy_tag;
        }
    }

    if (pt_svctx->b_in_stopping)
    {
        /* reset the stream req array */
        for (ui1_idx = 0; ui1_idx < (UINT8) STRM_TYPE_COUNT_WITH_AD; ui1_idx++)
        {
            pt_old_svc_req->pt_pending_strm_reqs[ui1_idx] = NULL;
            svctx_reset_strm_req(&(pt_old_svc_req->t_strm_reqs[ui1_idx]));
        }
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_replace_video_attributes
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static VOID _ch_hdlr_replace_video_attributes (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_new_svc_req,
                    CH_SVC_REQ_T*               pt_old_svc_req
                    )
{
    STRM_REQ_T* pt_video_strm_req;

    if (pt_new_svc_req == NULL || pt_old_svc_req == NULL) {
        return;
    }

    pt_video_strm_req = &(pt_old_svc_req->t_strm_reqs[ST_VIDEO]);

    if (pt_video_strm_req == NULL || x_handle_valid(pt_video_strm_req->h_stream) == FALSE) {

        /* Added for DTV00321062. Need to active the new display region setting
         * even if the video stream is not opened during svc replacing. */ 
        if (pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED) {
            ch_hdlr_set_video_plane(pt_svctx, SCC_VID_BLANK);
        }
        return;
    }

    svctx_set_video_stream_attrs(pt_svctx, pt_video_strm_req->h_stream);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_replace_svc_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_replace_svc_req(
                    SVCTX_T*                    pt_svctx
                    )
{
    INT32 i4_ret = SVCTXR_FAIL;

    do {
        CH_SVC_REQ_T* pt_replacer = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
        CH_SVC_REQ_T* pt_replacee = (CH_SVC_REQ_T*)pt_svctx->pt_pending_svc_req;
        BOOL          b_same_cc_req      = TRUE;

        ASSERT(pt_replacer != NULL, ("%s(), pt_replacer is NULL\r\n", __func__));
        ASSERT(pt_replacee != NULL, ("%s(), pt_replacee is NULL\r\n", __func__));

        if (pt_replacer->t_src_info.b_force_change)
        {
            i4_ret = SVCTXR_FAIL;
            break;
        }
        
        if (ch_svc_req_equals(pt_replacer, pt_replacee) == FALSE) {
            i4_ret = SVCTXR_FAIL;
            break;
        }

#ifdef MW_TV_AV_BYPASS_SUPPORT
        if (FALSE == pt_svctx->b_svc_selected)
        {
            i4_ret = SVCTXR_FAIL;
            break;
        }

        if (!IS_SVCTX_ASSOCIATE(pt_svctx))
#endif
        {
            pt_replacer->t_parent.b_last_signal_booster = pt_replacee->t_parent.b_last_signal_booster;
        }

        _ch_hdlr_replace_video_attributes(
                    pt_svctx,
                    pt_replacer,
                    pt_replacee
                    );

        if (pt_svctx->e_state == SVCTX_COND_PRESENTING
                && svctx_cc_filter_equals(
                    &pt_replacer->t_cc_filter,
                    &pt_replacee->t_cc_filter
                    ) == FALSE) {
            b_same_cc_req = FALSE;
            svctx_stop_cc_strm (&(pt_replacee->t_strm_reqs[ST_CLOSED_CAPTION]));
        }

        pt_replacer->e_brdcst_type               = pt_replacee->e_brdcst_type;
        pt_replacer->pt_player                   = pt_replacee->pt_player;
        pt_replacer->pt_crnt_event               = pt_replacee->pt_crnt_event;
        pt_replacer->pt_rating_chker             = pt_replacee->pt_rating_chker;
        pt_replacer->pt_strm_fmt_chker           = pt_replacee->pt_strm_fmt_chker;
        pt_replacer->e_crnt_tuner_type           = pt_replacee->e_crnt_tuner_type;
        pt_replacer->pt_scart_avm                = pt_replacee->pt_scart_avm;
        pt_replacer->e_ch_src_type               = pt_replacee->e_ch_src_type;
        pt_replacer->ui4_empty_strm_comp_cond    = pt_replacee->ui4_empty_strm_comp_cond;
        pt_replacer->ui4_exist_strm_comp_cond    = pt_replacee->ui4_exist_strm_comp_cond;
        pt_replacer->b_in_scdb_empty_check_delay = pt_replacee->b_in_scdb_empty_check_delay;
        pt_replacer->b_scramble_video            = pt_replacee->b_scramble_video;
        pt_replacer->b_scramble_audio            = pt_replacee->b_scramble_audio;

        pt_replacee->pt_player         = NULL;
        pt_replacee->pt_crnt_event     = NULL;
        pt_replacee->pt_rating_chker   = NULL;
        pt_replacee->pt_strm_fmt_chker = NULL;
        pt_replacee->pt_scart_avm      = NULL;

        _ch_hdlr_replace_strm_req(pt_svctx, pt_replacer, pt_replacee);

        if (pt_svctx->e_state == SVCTX_COND_PRESENTING) {
            if (b_same_cc_req == FALSE) {
                if (pt_replacer->t_cc_filter.e_cc_type == SM_CC_TYPE_NTSC_LINE21
                        || pt_replacer->t_cc_filter.e_cc_type == SM_CC_TYPE_DTVCC) {
                    _ch_hdlr_reload_cc_strm_as_presenting(pt_svctx, pt_replacer);
                }
            }
        }

        _ch_hdlr_resent_nfy_as_replacing(pt_svctx, pt_replacer);
        i4_ret = SVCTXR_OK;
    } while (0);

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_preproc_surf_svc_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_preproc_surf_svc_req(
                    SVCTX_T*                    pt_svctx,
                    UINT32                      ui4_nw_chk_mask,
                    UINT32                      ui4_nw_chk_value,
                    SURF_MODE_T                 e_surf_mode,
                    SVL_REC_T*                  pt_svl_rec,
                    CH_SVC_REQ_T*               pt_svc_req
                    )
{
    BOOL          b_found            = TRUE;
    INT32         i4_ret             = SVCTXR_OK;
    UINT32        ui4_crnt_ch_id     = 0;
    UINT32        ui4_next_ch_id     = 0;
    UINT32        ui4_ver_id         = SVL_NULL_VER_ID;
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T      h_svl              = NULL_HANDLE;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (pt_svl_rec == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (pt_svctx->pt_running_svc_req == NULL
            || pt_running_svc_req->t_parent.e_src_type != SRC_TYPE_BRDCST_SVL) {
        return SVCTXR_FAIL;
    }

    h_svl = rating_chker_get_svl_handle(pt_running_svc_req->pt_rating_chker);

    if (x_handle_valid(h_svl) == FALSE) {
        return SVCTXR_FAIL;
    }

    i4_ret = x_svl_get_rec (
                    h_svl,
                    pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svl_rec_id,
                    pt_svl_rec,
                    &ui4_ver_id
                    );
    if (i4_ret != SVLR_OK) {
        return SVCTXR_FAIL;
    }

    if (e_surf_mode != SURF_MODE_THIS) {
        ui4_crnt_ch_id = ui4_next_ch_id = pt_svl_rec->uheader.t_rec_hdr.ui4_channel_id;
        do {
            b_found = FALSE;
            i4_ret = x_svctx_get_svc_rec_by_channel_id(
                        h_svl,
                        ui4_nw_chk_value,
                        e_surf_mode,
                        &ui4_next_ch_id,
                        pt_svl_rec
                        );
            if (i4_ret != SVCTXR_OK) {
                break;
            }

            if ((ui4_nw_chk_mask & ui4_nw_chk_value) == (ui4_nw_chk_mask & pt_svl_rec->uheader.t_rec_hdr.ui4_nw_mask)) {
                if (ui4_crnt_ch_id == ui4_next_ch_id) {
                    b_found = FALSE;
                } else {
                    b_found = TRUE;
                }
                break;
            }
        } while (ui4_crnt_ch_id != ui4_next_ch_id);
    } else {
        b_found = TRUE;
    }

    if (b_found == FALSE) {
        return SVCTXR_NO_RECORD;
    }

    *pt_svc_req = *(pt_running_svc_req);
    pt_svc_req->t_src_info.u_info.t_svl.ui2_svl_rec_id          = pt_svl_rec->ui2_svl_rec_id;
    pt_svc_req->t_src_info.u_info.t_svl.t_silent.b_enable       = FALSE;
    pt_svc_req->t_src_info.u_info.t_svl.t_silent.ui2_svl_rec_id = SVL_NULL_REC_ID;
#ifdef SYS_MHP_SUPPORT
    pt_svc_req->t_src_info.u_info.t_svl.t_silent.ui2_svc_lst_id = SVL_NULL_ID;
#endif
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_create_surf_svc_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static SVC_REQ_T* _ch_hdlr_create_surf_svc_req(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_svctx,
                    SVCTX_HDLR_T*               pt_hdlr,
                    UINT32                      ui4_nw_chk_mask,
                    UINT32                      ui4_nw_chk_value,
                    SURF_MODE_T                 e_surf_mode,
                    SVL_REC_T*                  pt_svl_rec
                    )
{
    SVL_REC_T     t_svl_rec  = {0};
    CH_SVC_REQ_T* pt_svc_req = NULL;
    INT32         i4_ret;

    pt_svc_req = (CH_SVC_REQ_T*)svctx_allocate_svc_req(pt_hdlr);
    if (pt_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"create SVC_REQ_T failed!\r\n"));
        return NULL;
    }

    i4_ret = _ch_hdlr_preproc_surf_svc_req(
                    pt_svctx,
                    ui4_nw_chk_mask,
                    ui4_nw_chk_value,
                    e_surf_mode,
                    &t_svl_rec,
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

    if (pt_svl_rec != NULL) {
        *pt_svl_rec = t_svl_rec;
    }

    return ((SVC_REQ_T*)pt_svc_req);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_preproc_svc_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_preproc_svc_req(
                    SVCTX_T*                    pt_svctx,
                    UINT32                      ui4_stream_mode,
                    const SRC_DESC_T*           pt_src_desc,
                    const SNK_DESC_T*           pt_snk_desc,
                    x_svctx_select_nfy_fct      pf_nfy,
                    VOID*                       pv_nfy_tag,
                    CH_SVC_REQ_T*               pt_svc_req
                    )
{
    INT32         i4_ret;
    SIZE_T        z_size;
    UINT8         ui1_idx;
    CHAR*         ps_src_grp_name     = NULL;
    CHAR*         ps_snk_grp_name     = NULL;
    CHAR*         ps_snk_grp_name_aux = NULL;
    CH_SVC_REQ_T* pt_running_svc_req  = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    STRM_REQ_T*   pt_strm_req         = NULL;
    INT32         ai4_idx[STRM_MAX_CONCURRENT];
    INT32         i;

    if (pt_running_svc_req != NULL) {
        for (ui1_idx = 0; ui1_idx < (UINT8) STRM_TYPE_COUNT; ui1_idx++) {
            STREAM_TYPE_T e_strm_type = e_g_ch_strm_types[ui1_idx];

            if ((e_strm_type == ST_UNKNOWN) || (e_strm_type >= STRM_TYPE_COUNT))
            {
                continue;
            }

            if (pt_running_svc_req->t_strm_reqs[e_strm_type].h_stream == NULL_HANDLE) {
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

            /* modified by Allen: close audio description stream if the normal audio is to be closed */
            ai4_idx[0] = (INT32) e_strm_type;
            ai4_idx[1] = (ST_AUDIO == e_strm_type) ? STRM_REQ_AD_INDEX : (INT32) ST_UNKNOWN;

            for (i = 0; (i < STRM_MAX_CONCURRENT) && (ai4_idx[i] != (INT32) ST_UNKNOWN); i++)
            {
                pt_strm_req = &(pt_running_svc_req->t_strm_reqs[ai4_idx[i]]);
                i4_ret = x_sm_close_stream(pt_strm_req->h_stream);
                if (i4_ret == SMR_OK || i4_ret == SMR_PENDING) {                    
                    pt_strm_req->h_stream                  = NULL_HANDLE;
                    pt_strm_req->e_req_type                = STRM_REQ_SCDB;
                    pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;

                    if (STRM_REQ_AD_INDEX == ai4_idx[i])
                    {
                        pt_strm_req->b_aux_audio = TRUE;
                    }
                    else
                    {
                        pt_strm_req->b_aux_audio = FALSE;
                    }
                }
            }
        }
    }

    /* 1. check source related parameters */
    if (pt_src_desc == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (pt_src_desc->ui2_svc_lst_id != 0 && pt_src_desc->ui2_svl_rec_id != 0) {
        pt_svc_req->t_parent.e_src_type                    = SRC_TYPE_BRDCST_SVL;
        pt_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id = pt_src_desc->ui2_svc_lst_id;
        pt_svc_req->t_src_info.u_info.t_svl.ui2_svl_rec_id = pt_src_desc->ui2_svl_rec_id;

        i4_ret = ch_hdlr_get_ids(pt_svc_req);
        if (SVCTXR_OK != i4_ret)
        {
            return i4_ret;
        }
    } else {
        switch (pt_src_desc->e_type) {
        case SRC_TYPE_BRDCST_SVL:
            pt_svc_req->t_parent.e_src_type = SRC_TYPE_BRDCST_SVL;
            if (pt_src_desc->pv_details != NULL) {
                pt_svc_req->t_src_info.u_info.t_svl = *((SRC_SVL_INFO_T*)pt_src_desc->pv_details);
            } else {
                pt_svc_req->t_src_info.u_info.t_svl = pt_src_desc->u_info.t_svl;
            }

#if 1  /* for MW_NINJA_CHANGE_CHANNEL */
            pt_svc_req->t_src_info.b_ninja_change = pt_svc_req->t_src_info.u_info.t_svl.b_ninja_change;
#endif
            pt_svc_req->t_src_info.b_force_change = pt_svc_req->t_src_info.u_info.t_svl.b_force_change;

            i4_ret = ch_hdlr_get_ids(pt_svc_req);
            if (SVCTXR_OK != i4_ret)
            {
                return i4_ret;
            }
            break;
        case SRC_TYPE_BRDCST_FREQ:
            pt_svc_req->t_parent.e_src_type = SRC_TYPE_BRDCST_FREQ;
            if (pt_src_desc->pv_details != NULL) {
                pt_svc_req->t_src_info.u_info.t_freq = *((SRC_FREQ_INFO_T*)pt_src_desc->pv_details);
            } else {
                pt_svc_req->t_src_info.u_info.t_freq = pt_src_desc->u_info.t_freq;
            }
            break;
        default:
            return SVCTXR_INV_ARG;
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

    /* Riks TV configuration */
    pt_svc_req->t_riks_tv = pt_src_desc->t_riks_tv; 
    
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

        /* modified by Allen: Fix me after having the correct group name */
        z_size = x_strlen(pt_snk_desc->ps_snk_grp_name_aux);
        if (z_size > 0 && z_size <= SVCTX_SNK_NAME_MAX_SIZE) {
            ps_snk_grp_name_aux = pt_snk_desc->ps_snk_grp_name_aux;
        }
    }

    i4_ret = svctx_preproc_svc_req_snk_part(
                    pt_svctx,
                    pt_src_desc,
                    pt_snk_desc,
                    ps_snk_grp_name,
                    ps_snk_grp_name_aux,
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
    pt_svc_req->t_strm_reqs[ST_AUDIO].b_aux_audio = FALSE;

    /* modified by Allen: audio description stream filter */
    if (pt_snk_desc != NULL && pt_snk_desc->pt_audio_strm_fltr_fct != NULL) {
        pt_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX].pt_strm_fltr_fct = pt_snk_desc->pt_audio_strm_fltr_fct;
        pt_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX].pv_strm_fltr_tag = pt_snk_desc->pv_audio_strm_fltr_tag;
    } else {
        pt_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX].pt_strm_fltr_fct = NULL;
        pt_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX].pv_strm_fltr_tag = NULL;
    }
    pt_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX].b_aux_audio = TRUE;

    /* set closed captioning request */
    if (pt_snk_desc == NULL || pt_snk_desc->pt_cc_filter == NULL) {
        pt_svc_req->h_gl_plane            = NULL_HANDLE;
        pt_svc_req->t_cc_filter.e_cc_type = SM_CC_TYPE_IGNORE;
    } else {
        pt_svc_req->h_gl_plane  = pt_snk_desc->h_cc_gl_plane;
        pt_svc_req->t_cc_filter = *(pt_snk_desc->pt_cc_filter);
    }
    /* cc stream filter */
    if (pt_snk_desc != NULL && pt_snk_desc->pt_cc_strm_fltr_fct != NULL) {
        pt_svc_req->t_strm_reqs[ST_CLOSED_CAPTION].pt_strm_fltr_fct = pt_snk_desc->pt_cc_strm_fltr_fct;
        pt_svc_req->t_strm_reqs[ST_CLOSED_CAPTION].pv_strm_fltr_tag = pt_snk_desc->pv_cc_strm_fltr_tag;
    } else {
        pt_svc_req->t_strm_reqs[ST_CLOSED_CAPTION].pt_strm_fltr_fct = NULL;
        pt_svc_req->t_strm_reqs[ST_CLOSED_CAPTION].pv_strm_fltr_tag = NULL;
    }

    if (pt_snk_desc != NULL) {
        switch(pt_snk_desc->e_text_strm_type) {
        case ST_CLOSED_CAPTION:
            if (pt_snk_desc != NULL && pt_snk_desc->u_text_strm.t_cc.pt_fltr_fct != NULL) {
                pt_svc_req->t_strm_reqs[ST_CLOSED_CAPTION].pt_strm_fltr_fct = pt_snk_desc->u_text_strm.t_cc.pt_fltr_fct;
                pt_svc_req->t_strm_reqs[ST_CLOSED_CAPTION].pv_strm_fltr_tag = pt_snk_desc->u_text_strm.t_cc.pv_fltr_tag;
            } else {
                pt_svc_req->t_strm_reqs[ST_CLOSED_CAPTION].pt_strm_fltr_fct = NULL;
                pt_svc_req->t_strm_reqs[ST_CLOSED_CAPTION].pv_strm_fltr_tag = NULL;
            }

            if (pt_snk_desc == NULL || pt_snk_desc->u_text_strm.t_cc.pt_filter == NULL) {
                pt_svc_req->h_gl_plane            = NULL_HANDLE;
                pt_svc_req->t_cc_filter.e_cc_type = SM_CC_TYPE_IGNORE;
            } else {
                pt_svc_req->h_gl_plane  = pt_snk_desc->u_text_strm.t_cc.h_gl_plane;
                pt_svc_req->t_cc_filter = *(pt_snk_desc->u_text_strm.t_cc.pt_filter);
            }
            break;
        case ST_TELETEXT:
            if (pt_snk_desc != NULL && pt_snk_desc->u_text_strm.t_telex.pt_fltr_fct != NULL) {
                pt_svc_req->t_strm_reqs[ST_TELETEXT].pt_strm_fltr_fct = pt_snk_desc->u_text_strm.t_telex.pt_fltr_fct;
                pt_svc_req->t_strm_reqs[ST_TELETEXT].pv_strm_fltr_tag = pt_snk_desc->u_text_strm.t_telex.pv_fltr_tag;
            } else {
                pt_svc_req->t_strm_reqs[ST_TELETEXT].pt_strm_fltr_fct = NULL;
                pt_svc_req->t_strm_reqs[ST_TELETEXT].pv_strm_fltr_tag = NULL;
            }
            pt_svc_req->h_gl_plane = pt_snk_desc->u_text_strm.t_telex.h_gl_plane;
            break;
        case ST_SUBTITLE:
            if (pt_snk_desc != NULL && pt_snk_desc->u_text_strm.t_subtitle.pt_fltr_fct != NULL) {
                pt_svc_req->t_strm_reqs[ST_SUBTITLE].pt_strm_fltr_fct = pt_snk_desc->u_text_strm.t_subtitle.pt_fltr_fct;
                pt_svc_req->t_strm_reqs[ST_SUBTITLE].pv_strm_fltr_tag = pt_snk_desc->u_text_strm.t_subtitle.pv_fltr_tag;
            } else {
                pt_svc_req->t_strm_reqs[ST_SUBTITLE].pt_strm_fltr_fct = NULL;
                pt_svc_req->t_strm_reqs[ST_SUBTITLE].pv_strm_fltr_tag = NULL;
            }
            pt_svc_req->h_gl_plane = pt_snk_desc->u_text_strm.t_subtitle.h_gl_plane;
            break;
        default:
            pt_svc_req->h_gl_plane = NULL_HANDLE;
            break;
        }
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_create_svc_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static SVC_REQ_T* _ch_hdlr_create_svc_req(
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
    CH_SVC_REQ_T* pt_svc_req = NULL;
    INT32         i4_ret;
    UINT8         ui1_idx;

    pt_svc_req = (CH_SVC_REQ_T*)svctx_allocate_svc_req(pt_hdlr); /* QQQQQQ */
    if (pt_svc_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"create SVC_REQ_T failed!\r\n"));
        return NULL;
    }

    i4_ret = _ch_hdlr_preproc_svc_req(
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

#if 1 /* Xia - 090609 for Sony ATV Project */
    if (pt_src_desc->pv_details != NULL)
    {
        pt_svc_req->t_parent.b_enable_signal_booster = ((SRC_SVL_INFO_T*)(pt_src_desc->pv_details))->b_enable_signal_booster;
    }
#endif
    pt_svc_req->t_parent.h_svctx                   = h_svctx;
    pt_svc_req->t_parent.pf_svc_nfy                = pf_nfy;
    pt_svc_req->t_parent.pv_svc_nfy_tag            = pv_nfy_tag;
    pt_svc_req->t_parent.ui4_stream_mode           = ui4_stream_mode;
    pt_svc_req->t_parent.pt_hdlr                   = pt_hdlr;
    pt_svc_req->t_parent.i4_speed                  = SESS_NORMAL_SPEED;
    pt_svc_req->e_brdcst_type                      = BRDCST_TYPE_UNKNOWN;

    for (ui1_idx = 0; ui1_idx < (UINT8) STRM_TYPE_COUNT_WITH_AD; ui1_idx++) {
        pt_svc_req->t_strm_reqs[ui1_idx].pt_hdlr = pt_hdlr;
    }

    return ((SVC_REQ_T*)pt_svc_req);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_cc_comp_count_filter
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static BOOL _ch_hdlr_cc_comp_count_filter (
                    UINT16                      ui2_num_recs,
                    UINT16                      ui2_idx,       /* in */
                    const STREAM_COMP_ID_T*     pt_comp_id,    /* in */
                    const SCDB_REC_T*           pt_scdb_rec,   /* in */
                    VOID*                       pv_tag         /* in */
                    )
{
    UINT16* pui2_num_recs = (UINT16*) pv_tag;

    ASSERT(pt_comp_id->e_type == ST_CLOSED_CAPTION,
                    ("stream type (%d) is not CLOSED_CAPTION)", pt_comp_id->e_type));

    if (ui2_idx == ui2_num_recs) {
        return FALSE;
    }

    switch(pt_scdb_rec->u.t_atsc_cc.e_type) {
    case CC_ATSC_LINE_21:
        if (pt_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_line_21.b_det_in_descr == TRUE) {
            (*pui2_num_recs) += 1;
        }
        break;
    case CC_ATSC_DTV:
        if (pt_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_dtv.ui1_caption_svc_num == 0) {
            break;
        }

        (*pui2_num_recs) += 1;
        break;
    default:
        break;
    }
    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_get_cp_info
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_get_cp_info(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    CP_INFO_LIST_T*             pt_cp_info
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    STRM_REQ_T*   pt_strm_req;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if ((e_strm_type == ST_UNKNOWN) || (e_strm_type >= STRM_TYPE_COUNT))
    {
        return SVCTXR_INV_ARG;
    }

    if (ch_hdlr_is_supported_strm_type(e_strm_type) == FALSE) {
        return SVCTXR_NOT_SUPPORT;
    }

    pt_strm_req = &(pt_running_svc_req->t_strm_reqs[e_strm_type]);
    *pt_cp_info = pt_strm_req->t_cp;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_get_ca_info
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_get_ca_info(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    SVCTX_CA_INFO_T*            pt_ca_info
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    STRM_REQ_T*   pt_strm_req;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if ((e_strm_type == ST_UNKNOWN) || (e_strm_type >= STRM_TYPE_COUNT))
    {
        return SVCTXR_INV_ARG;
    }

    if (ch_hdlr_is_supported_strm_type(e_strm_type) == FALSE) {
        return SVCTXR_NOT_SUPPORT;
    }

    pt_strm_req = &(pt_running_svc_req->t_strm_reqs[e_strm_type]);
    *pt_ca_info = pt_strm_req->t_ca;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_mpeg2_strm_pid_filter
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static BOOL _ch_hdlr_mpeg2_strm_pid_filter (
                    UINT16                      ui2_num_recs,
                    UINT16                      ui2_idx,
                    const STREAM_COMP_ID_T*     pt_comp_id,
                    const SCDB_REC_T*           pt_scdb_rec,
                    VOID*                       pv_tag
                    )
{
    UINT16 ui2_pid = (UINT16)(UINT32)pv_tag;

    if (ui2_num_recs == ui2_idx) {
        return FALSE;
    }

    switch(pt_comp_id->e_type) {
    case ST_AUDIO:
        if (pt_scdb_rec->u.t_audio_mpeg.ui2_pid == ui2_pid) {
            return TRUE;
        }
        break;
    case ST_VIDEO:
        if (pt_scdb_rec->u.t_video_mpeg.ui2_pid == ui2_pid) {
            return TRUE;
        }
        break;
    case ST_SUBTITLE:
        if (pt_scdb_rec->u.t_dvb_sbtl.ui2_pid == ui2_pid) {
            return TRUE;
        }
        break;
#ifdef MW_ISDB_SUPPORT
    case ST_ISDB_CAPTION:
        if (pt_scdb_rec->u.t_isdb_caption.ui2_pid == ui2_pid) {
            return TRUE;
        }
        break;
    case ST_ISDB_TEXT:
        if (pt_scdb_rec->u.t_isdb_text.ui2_pid == ui2_pid) {
            return TRUE;
        }
        break;
#endif
    default:
        break;
    }

    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_strm_comp_id_filter
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static BOOL _ch_hdlr_strm_comp_id_filter (
                    UINT16                      ui2_num_recs,
                    UINT16                      ui2_idx,
                    const STREAM_COMP_ID_T*     pt_comp_id,
                    const SCDB_REC_T*           pt_scdb_rec,
                    VOID*                       pv_tag
                    )
{
    STREAM_COMP_ID_T* pt_target_comp_id = (STREAM_COMP_ID_T*)pv_tag;

    if (pt_target_comp_id->e_type == pt_comp_id->e_type
            && pt_target_comp_id->pv_stream_tag == pt_comp_id->pv_stream_tag) {
        /* tricky: pv_stream_tag is a unique key and assigned by CM.*/
        return TRUE;
    }

    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_get_strm_comp_id_ex
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_get_strm_comp_id_ex(
                    SVCTX_T*                     pt_svctx,
                    STREAM_TYPE_T                e_strm_type,
                    BOOL                         b_auto_version_update,
                    x_svctx_strm_comp_filter_fct pf_filter,
                    VOID*                        pv_tag,
                    STREAM_COMP_ID_T*            pt_comp_id,
                    SCDB_REC_T*                  pt_scdb_rec,
                    UINT16*                      pui2_idx
                    )
{
    UINT32           ui4_db_version     = SCDB_NULL_VER_ID;
    UINT16           ui2_num_recs       = 0;
    UINT16           ui2_idx            = 0;
    BOOL             b_is_searching     = TRUE;
    CH_SVC_REQ_T*    pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T         h_scdb             = NULL_HANDLE;
    UINT8            ui1_reloop_count   = 0;
    STREAM_COMP_ID_T t_first_comp_id;
    SCDB_REC_T       t_first_scdb_rec;
    SCDB_REC_T       t_scdb_rec         = {0};
    INT32            i4_ret;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (pf_filter == NULL || pt_comp_id == NULL) {
        return SVCTXR_INV_ARG;
    }

    h_scdb = player_get_scdb_handle(pt_running_svc_req->pt_player);
    if (x_handle_valid(h_scdb) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    while (b_is_searching) {
        b_is_searching = FALSE;
        i4_ret = x_scdb_get_num_recs(
                    h_scdb,
                    e_strm_type,
                    &ui2_num_recs,
                    &ui4_db_version
                    );

        if (i4_ret != SCDBR_OK) {
            DBG_ERROR((_ERROR_HEADER"x_scdb_get_num_recs() failed, return = %d\r\n", i4_ret));
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
                /* intergation from 0 to (ui2_num_recs-1) */
                
                if (pf_filter(
                            ui2_num_recs,
                            STRM_COMP_FILTER_MAKE_INDEX(ui2_idx, ui1_reloop_count),
                            pt_comp_id,
                            &t_scdb_rec,
                            pv_tag
                            ) == TRUE) {
                    if (pt_scdb_rec != NULL) {
                        *pt_scdb_rec = t_scdb_rec;
                    }

                    if (pui2_idx != NULL) {
                        *pui2_idx = ui2_idx;
                    }
                    return SVCTXR_OK;
                }

                if (ui2_idx == 0) {
                    t_first_comp_id  = *pt_comp_id;
                    t_first_scdb_rec = t_scdb_rec;
                }

                if (ui2_idx < (UINT16) (ui2_num_recs-1)) {
                    continue;
                }

                if (pf_filter(
                            ui2_num_recs,
                            STRM_COMP_FILTER_MAKE_INDEX(ui2_num_recs, ui1_reloop_count),
                            &t_first_comp_id,
                            &t_first_scdb_rec,
                            pv_tag
                            ) == TRUE) {
                    if (pt_scdb_rec != NULL) {
                        *pt_comp_id  = t_first_comp_id;
                        *pt_scdb_rec = t_first_scdb_rec;
                    }

                    if (pui2_idx != NULL) {
                        *pui2_idx = 0;
                    }
                    return SVCTXR_OK;
                } else {
                    /* one more loop for audio/video*/
                    if (e_strm_type == ST_VIDEO || e_strm_type == ST_AUDIO) {
                        ui1_reloop_count++;
                        if (ui1_reloop_count < (UINT8) STRM_COMP_FILTER_MAX_LOOP) {
                            b_is_searching = TRUE;
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
                DBG_ERROR((_ERROR_HEADER"x_scdb_get_rec_by_idx failed - return = %d\r\n", i4_ret));
                return SVCTXR_FAIL;
            }
        }
    }
    return SVCTXR_NOT_EXIST;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_get_strm_comp_id
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_get_strm_comp_id(
                    SVCTX_T*                     pt_svctx,
                    STREAM_TYPE_T                e_strm_type,
                    BOOL                         b_auto_version_update,
                    x_svctx_strm_comp_filter_fct pf_filter,
                    VOID*                        pv_tag,
                    STREAM_COMP_ID_T*            pt_comp_id,
                    SCDB_REC_T*                  pt_scdb_rec
                    )
{
    return _ch_hdlr_get_strm_comp_id_ex(
                    pt_svctx,
                    e_strm_type,
                    b_auto_version_update,
                    pf_filter,
                    pv_tag,
                    pt_comp_id,
                    pt_scdb_rec,
                    NULL
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_get_strm_req_type
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_get_strm_req_type(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    STRM_REQ_TYPE_T*            pe_strm_req_type
                    )
{
    STRM_REQ_T*   pt_strm_req = NULL;
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if ((e_strm_type == ST_UNKNOWN) || (e_strm_type >= STRM_TYPE_COUNT)) {
        return SVCTXR_INV_ARG;
    }

    if (ch_hdlr_is_supported_strm_type(e_strm_type) == FALSE) {
        return SVCTXR_NOT_SUPPORT;
    }

    pt_strm_req = &(pt_running_svc_req->t_strm_reqs[e_strm_type]);

    if (x_handle_valid(pt_strm_req->h_stream) == FALSE) {
        return SVCTXR_NOT_EXIST;
    }

    if ((pt_strm_req->e_req_type == STRM_REQ_SCDB) 
        || (pt_strm_req->e_req_type == STRM_REQ_PID)) {
        *pe_strm_req_type = pt_strm_req->e_req_type;
        return SVCTXR_OK;
    } else {
        return SVCTXR_NOT_EXIST;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_get_using_scdb_info_ex
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_get_using_scdb_info_ex(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    STREAM_COMP_ID_T*           pt_comp_id,
                    SCDB_REC_T*                 pt_scdb_rec,
                    UINT16*                     pui2_idx
                    )
{
    INT32         i4_ret;
    STRM_REQ_T*   pt_strm_req = NULL;
    STRM_REQ_T*   pt_pending_strm_req = NULL;
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T      h_scdb             = NULL_HANDLE;
    SCDB_REC_T       t_scdb_rec;
    STREAM_COMP_ID_T t_comp_id;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));
    x_memset(&t_scdb_rec, 0, sizeof(t_scdb_rec));
    x_memset(&t_comp_id, 0, sizeof(t_comp_id));

    if ((e_strm_type == ST_UNKNOWN) || (e_strm_type >= STRM_TYPE_COUNT_WITH_AD))
    {
        return SVCTXR_INV_ARG;
    }

    if (ch_hdlr_is_supported_strm_type(e_strm_type) == FALSE) {
        return SVCTXR_NOT_SUPPORT;
    }

    h_scdb      = player_get_scdb_handle(pt_running_svc_req->pt_player);
    pt_strm_req = &(pt_running_svc_req->t_strm_reqs[e_strm_type]);
    pt_pending_strm_req = pt_running_svc_req->pt_pending_strm_reqs[e_strm_type];

    /* It is not 100% correct to check the validality of h_stream.
     *  -------------------------------------------------------------- 
     * | h_stream      |      Other Conditions         | Exists using?|
     * |---------------+-------------------------------+--------------|
     * | valid/invalid | pt_pending_strm_req != NULL   |   Yes        |
     * | invalid       |  OPENING (b_open_queued)      |   Yes        |
     * | valid         |  STARTED                      |   Yes        |
     * | valid/invalid |  CLOSING                      |   No         |
     * | invalid       |  CLOSED                       |   No         |
     *  -------------------------------------------------------------- 
     */
    if (NULL == pt_pending_strm_req
        && pt_strm_req->e_strm_cond != SM_COND_STARTED
        && pt_strm_req->e_strm_cond != SM_COND_OPENING) {
        return SVCTXR_NOT_EXIST;
    }


    if (pt_strm_req->e_req_type == STRM_REQ_SCDB) {
        if (pt_strm_req->u.t_scdb.t_comp_id.e_type != e_strm_type
                || pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag == NULL) {
            return SVCTXR_NOT_EXIST;
        }

        /* Should be optimized later for ninja change case */
        if (pt_running_svc_req->t_src_info.b_ninja_change 
                && pt_running_svc_req->pt_player->b_ninja_change
                && pt_strm_req->e_strm_cond == SM_COND_STARTED)
        {
            if (pt_comp_id != NULL) {
                *pt_comp_id = pt_strm_req->u.t_scdb.t_comp_id;
            }

            if (pt_scdb_rec != NULL) {
                *pt_scdb_rec = pt_strm_req->u.t_scdb.t_rec;
            }
            return SVCTXR_OK;
        }

        /* If there is a pending request, should take it as the using info. */
        if (pt_pending_strm_req
               && pt_pending_strm_req->u.t_scdb.t_comp_id.e_type == e_strm_type)
        {
            if (pt_comp_id != NULL) {
                *pt_comp_id = pt_pending_strm_req->u.t_scdb.t_comp_id;
            }

            if (pt_scdb_rec != NULL) {
                *pt_scdb_rec = pt_pending_strm_req->u.t_scdb.t_rec;
            }

            if (pui2_idx != NULL) {
                i4_ret = _ch_hdlr_get_strm_comp_id_ex(
                                pt_svctx,
                                e_strm_type,
                                TRUE,
                                _ch_hdlr_strm_comp_id_filter,
                                (VOID*)(&(pt_pending_strm_req->u.t_scdb.t_comp_id)),
                                &t_comp_id,
                                NULL,
                                pui2_idx
                                );
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
                *pt_comp_id = pt_strm_req->u.t_scdb.t_comp_id;
            }

            if (pt_scdb_rec != NULL) {
                *pt_scdb_rec = pt_strm_req->u.t_scdb.t_rec;
            }

            if (pui2_idx != NULL) {
                i4_ret = _ch_hdlr_get_strm_comp_id_ex(
                                pt_svctx,
                                e_strm_type,
                                TRUE,
                                _ch_hdlr_strm_comp_id_filter,
                                (VOID*)(&(pt_strm_req->u.t_scdb.t_comp_id)),
                                &t_comp_id,
                                NULL,
                                pui2_idx
                                );
                if (i4_ret != SVCTXR_OK) {
                    return i4_ret;
                }
            }
        }
    } else if (pt_strm_req->e_req_type == STRM_REQ_PID) { /* Only for MHEG5 & MHP use */

        if (pt_strm_req->u.t_pid.e_strm_type != e_strm_type
                || pt_strm_req->u.t_pid.t_mpeg_2_pid >= MPEG_2_NULL_PID) {
            return SVCTXR_NOT_EXIST;
        }

        /* Try to find the corresponding info per PID from SCDB first. */
        i4_ret = _ch_hdlr_get_strm_comp_id_ex(
                pt_svctx,
                e_strm_type,
                TRUE,
                _ch_hdlr_mpeg2_strm_pid_filter,
                (VOID*)(UINT32)pt_strm_req->u.t_pid.t_mpeg_2_pid,
                &t_comp_id,
                &t_scdb_rec,
                pui2_idx
                );
        if (i4_ret == SVCTXR_OK) {
            if (pt_comp_id != NULL) {
                *pt_comp_id = t_comp_id;
            }
            if (pt_scdb_rec != NULL) {
                *pt_scdb_rec = t_scdb_rec;
            }
            return SVCTXR_OK;
        }

        if (pt_comp_id != NULL) {
            pt_comp_id->e_type        = pt_strm_req->u.t_pid.e_strm_type;
            pt_comp_id->pv_stream_tag = NULL;
        }
        if (pt_scdb_rec != NULL) {
            x_memset(pt_scdb_rec, 0, sizeof(SCDB_REC_T));
            switch(pt_strm_req->u.t_pid.e_strm_type) {
            case ST_AUDIO:
                pt_scdb_rec->e_rec_type             = SCDB_REC_TYPE_AUDIO_MPEG;
                pt_scdb_rec->u.t_audio_mpeg.ui2_pid = pt_strm_req->u.t_pid.t_mpeg_2_pid;
                break;
            case ST_VIDEO:
                pt_scdb_rec->e_rec_type             = SCDB_REC_TYPE_VIDEO_MPEG;
                pt_scdb_rec->u.t_video_mpeg.ui2_pid = pt_strm_req->u.t_pid.t_mpeg_2_pid;
                break;
            case ST_SUBTITLE:
                pt_scdb_rec->e_rec_type             = SCDB_REC_TYPE_SUBTITLE_DVB;
                pt_scdb_rec->u.t_dvb_sbtl.ui2_pid   = pt_strm_req->u.t_pid.t_mpeg_2_pid;
                break;
#ifdef MW_ISDB_SUPPORT
            case ST_ISDB_CAPTION:
                pt_scdb_rec->e_rec_type               = SCDB_REC_TYPE_ISDB_CAPTION;
                pt_scdb_rec->u.t_isdb_caption.ui2_pid = pt_strm_req->u.t_pid.t_mpeg_2_pid;
                break;
            case ST_ISDB_TEXT:
                pt_scdb_rec->e_rec_type             = SCDB_REC_TYPE_ISDB_TEXT;
                pt_scdb_rec->u.t_isdb_text.ui2_pid  = pt_strm_req->u.t_pid.t_mpeg_2_pid;
                break;
#endif
            default:
                ASSERT(0, ("Invalid stream type (%d)!\n", e_strm_type));
                break;
            }
        }
    } else {
        return SVCTXR_NOT_EXIST;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_get_using_scdb_info
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_get_using_scdb_info(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    STREAM_COMP_ID_T*           pt_comp_id,
                    SCDB_REC_T*                 pt_scdb_rec
                    )
{
    return _ch_hdlr_get_using_scdb_info_ex(
                    pt_svctx,
                    e_strm_type,
                    pt_comp_id,
                    pt_scdb_rec,
                    NULL
                    );
}

static INT32 _ch_hdlr_get_using_ad_info(
                    SVCTX_T*                    pt_svctx,
                    SCDB_REC_T*                 pt_scdb_rec
                    )
{
    INT32         i4_ret;
    STRM_REQ_T*   pt_strm_req = NULL;
    STRM_REQ_T*   pt_pending_strm_req = NULL;
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T      h_scdb             = NULL_HANDLE;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    h_scdb      = player_get_scdb_handle(pt_running_svc_req->pt_player);
    pt_strm_req = &(pt_running_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX]);
    pt_pending_strm_req = pt_running_svc_req->pt_pending_strm_reqs[STRM_REQ_AD_INDEX];

    if (!pt_svctx->b_ad_enabled 
            || (pt_strm_req->e_req_type != STRM_REQ_SCDB)
            || (x_handle_valid(pt_strm_req->h_stream) == FALSE)) {
        return SVCTXR_NOT_EXIST;
    }

    if (pt_strm_req->e_req_type == STRM_REQ_SCDB) {
        if (pt_strm_req->u.t_scdb.t_comp_id.e_type != ST_AUDIO
                || pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag == NULL) {
            return SVCTXR_NOT_EXIST;
        }

        if (pt_pending_strm_req)
        {
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
            if (pt_scdb_rec != NULL) {
                *pt_scdb_rec = pt_strm_req->u.t_scdb.t_rec;
            }
        }
    } 

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _ch_hdlr_set_sess_attr
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
static INT32 _ch_hdlr_set_sess_attr(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    SM_SET_TYPE_T               e_set_type,
                    VOID*                       pv_set_info,
                    SIZE_T                      z_set_info_size
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T      h_sess;
    INT32         i4_ret;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    h_sess = player_get_sess_handle(pt_running_svc_req->pt_player);

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
 * Name: _ch_hdlr_set_strm_attr
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
static INT32 _ch_hdlr_set_strm_attr(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    SM_SET_TYPE_T               e_set_type,
                    VOID*                       pv_set_info,
                    SIZE_T                      z_set_info_size
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    INT32         i4_ret;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if ((e_strm_type == ST_UNKNOWN) || (e_strm_type >= STRM_TYPE_COUNT))
    {
        return SVCTXR_INV_ARG;
    }

    if (ch_hdlr_is_supported_strm_type(e_strm_type) == FALSE) {
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
 * Name: _ch_hdlr_get_strm_handle
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *-----------------------------------------------------------------------------*/
static BOOL _ch_hdlr_get_strm_handle(
                    SVC_REQ_T*                  pt_svc_req,
                    STREAM_TYPE_T               e_stream_type,
                    HANDLE_T*                   ph_stream
                    )
{
    CH_SVC_REQ_T* pt_ch_svc_req = (CH_SVC_REQ_T*)pt_svc_req;

    if ((e_stream_type == ST_UNKNOWN) || (e_stream_type >= STRM_TYPE_COUNT))
    {
        return FALSE;
    }

    if (pt_ch_svc_req == NULL) {
        return FALSE;
    }

    if (ch_hdlr_is_supported_strm_type(e_stream_type) == FALSE) {
        return FALSE;
    }

    *ph_stream = pt_ch_svc_req->t_strm_reqs[e_stream_type].h_stream;
    return TRUE;
}

static INT32 _ch_hdlr_get_cell_id(
                    SVCTX_T*                    pt_svctx,
                    UINT16*                     pui2_cell_id
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T      h_conn             = NULL_HANDLE;
    INT32         i4_ret             = SVCTXR_OK;
    CM_COND_T     e_cm_cond;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (pui2_cell_id == NULL) {
        return SVCTXR_INV_ARG;
    }

    h_conn = player_get_conn_handle(pt_running_svc_req->pt_player);
    if (x_handle_valid(h_conn) == FALSE) {
        return SVCTXR_FAIL;
    }

    i4_ret = x_cm_get_cond (
                    h_conn,
                    &e_cm_cond
                    );
    if (i4_ret != CMR_OK || e_cm_cond != CM_COND_CONNECTED) {
        return SVCTXR_FAIL;
    }

    i4_ret = x_cm_get(
                    h_conn,
                    BRDCST_CTRL_GET_CELL_ID,
                    pui2_cell_id
                    );
    if (i4_ret != CMR_OK) {
        DBG_INFO((_INFO_HEADER"x_cm_get(TUNER_STATUS) failed, ret=%d\r\n", i4_ret));
        return SVCTXR_FAIL;
    }

    return SVCTXR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _ch_hdlr_get_vbi_broker
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *-----------------------------------------------------------------------------*/
static BOOL _ch_hdlr_get_vbi_broker(
                    SVCTX_T*                        pt_svctx,
                    SVC_REQ_T*                      pt_svc_req,
                    const SVCTX_SM_BROKER_CMD_T*    pt_cmd_sets,
                    SVCTX_VBI_BROKER_T*             pt_vbi_broker
                    )
{
    CH_SVC_REQ_T*   pt_ch_svc_req     = (CH_SVC_REQ_T*)pt_svc_req;
    SRC_OPER_TYPE_T e_src_oper_type;
    SNK_OPER_TYPE_T e_snk_oper_type;
    HANDLE_T        h_svl;
    UINT32          ui4_ver_id = SVL_NULL_VER_ID;
    SVL_REC_T       t_svl_rec;
    INT32           i4_ret;
    UINT16          ui2_svc_lst_id = SVL_NULL_ID;
    UINT16          ui2_svl_rec_id = SVL_NULL_REC_ID;
    CHAR*           ps_src_name    = NULL;
    UINT32          ui4_cmd_bits   = 0;
    BOOL            b_ret = FALSE;

    if (pt_ch_svc_req == NULL
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
            ps_src_name        = pt_cmd_sets->u.ps_text;
            ui4_cmd_bits      |= SVCTX_SM_BROKER_CMD_BIT_SRC_NAME;
            break;
        case SVCTX_SM_BROKER_CMD_CODE_SVC_LST_ID:
            ui2_svc_lst_id     = pt_cmd_sets->u.ui2_data;
            ui4_cmd_bits      |= SVCTX_SM_BROKER_CMD_BIT_SVC_LST_ID;
            break;
        case SVCTX_SM_BROKER_CMD_CODE_SVL_REC_ID:
            ui2_svl_rec_id     = pt_cmd_sets->u.ui2_data;
            ui4_cmd_bits      |= SVCTX_SM_BROKER_CMD_BIT_SVL_REC_ID;
            break;
        default:
            break;
        }
        pt_cmd_sets++;
    }

    if (pt_vbi_broker->e_device_type != DEV_TUNER
            || ui4_cmd_bits != SVCTX_SM_BROKER_CMD_BIT_TUNER
            || pt_ch_svc_req->t_parent.e_src_type != SRC_TYPE_BRDCST_SVL) {
        return FALSE;
    }

    if (ch_hdlr_is_supported_strm_type(ST_VIDEO) == FALSE) {
        return FALSE;
    }

    do {
        i4_ret = x_svl_open(ui2_svc_lst_id, NULL, NULL, &h_svl);
        if (i4_ret != SVLR_OK) {
            b_ret = FALSE;
            break;
        }

        i4_ret = x_svl_get_rec (h_svl, ui2_svl_rec_id, &t_svl_rec, &ui4_ver_id);
        if (i4_ret != SVLR_OK) {
            b_ret = FALSE;
            break;
        }

        if (ps_src_name == NULL
                || x_strcmp(ps_src_name, pt_ch_svc_req->t_src_info.s_src_grp_name) != 0
                || ui2_svc_lst_id != pt_ch_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id
                || ui2_svl_rec_id != pt_ch_svc_req->t_src_info.u_info.t_svl.ui2_svl_rec_id) {
            b_ret = FALSE;
            break;
        }

        if (svctx_with_event_capability(
                    pt_svctx,
                    pt_ch_svc_req->e_crnt_tuner_type,
                    DEV_EVN_CAPABILITY_ALL) == FALSE) {
            b_ret = FALSE;
            break;
        }

        if (pt_ch_svc_req->pt_player != NULL) {
            i4_ret = player_get_oper_types(
                        pt_ch_svc_req->pt_player,
                        &e_src_oper_type,
                        &e_snk_oper_type
                        );
            if (i4_ret != SVCTXR_OK || e_src_oper_type != SRC_OPER_TYPE_CONNECT) {
                b_ret = FALSE;
                break;
            }
        }

        pt_vbi_broker->h_conn           = player_get_conn_handle(pt_ch_svc_req->pt_player);
        pt_vbi_broker->h_stream         = pt_ch_svc_req->t_strm_reqs[ST_VIDEO].h_stream;
        pt_vbi_broker->b_is_digital_vbi = t_svl_rec.uheader.t_rec_hdr.e_brdcst_type == BRDCST_TYPE_ANALOG
                                        ? FALSE
                                        : TRUE;
        b_ret = TRUE;
    } while (0);

    if (x_handle_valid(h_svl) == TRUE) {
        i4_ret = x_svl_close(h_svl);
        if (i4_ret != SVLR_OK) {
            DBG_ERROR((_ERROR_HEADER"x_svl_close(h_svl=%d) failed, i4_ret = %d\r\n", h_svl, i4_ret));
        }
    }

    return b_ret;
}
/*-----------------------------------------------------------------------------
 * Name: _ch_hdlr_get_current_event_info_len
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_get_current_event_info_len (
                    SVCTX_T*                        pt_svctx,
                    SVC_REQ_T*                      pt_svc_req,
#ifdef MW_EDB_SUPPORT
                    EDB_KEY_TYPE_T                  e_key_type,
#else                    
                    EVCTX_KEY_TYPE_T                e_key_type,
#endif                    
                    VOID*                           pv_key_info,
                    SIZE_T*                         pz_event_info_len
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svc_req;
    HANDLE_T      h_evctx = crnt_evn_get_handle(pt_running_svc_req->pt_crnt_event);
#ifndef MW_EDB_SUPPORT
    EVCTX_COND_T  e_cond  = EVCTX_COND_UNAVAIL;
#else
    EDB_CH_EVT_ID_T      t_ch_evt_id = {0};
#endif
    INT32         i4_ret;

    if (pt_svctx->e_block_rule == SVCTX_BLOCK_RULE_INP_CH_PROG_SIGNAL_LOSS
#ifndef MW_EDB_SUPPORT            
            && e_key_type == EVCTX_KEY_TYPE_RATING
#else
            && e_key_type == EDB_KEY_TYPE_RATING_LIST_BY_ID
#endif
            && pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) {
        /* get rating size from crnt_evn's buffer */
        if (pz_event_info_len == NULL) {
            return SVCTXR_INV_ARG;
        }

        *pz_event_info_len = crnt_evn_get_rating_size(pt_running_svc_req->pt_crnt_event);
    } else {
        if (svctx_with_event_capability(
                    pt_svctx,
                    pt_running_svc_req->e_crnt_tuner_type,
                    MAKE_BIT_MASK_32(e_key_type)) == FALSE) {
            return SVCTXR_EVN_INFO_NOT_SUPPORT;
        }

        if (x_handle_valid(h_evctx) == FALSE) {
            return SVCTXR_EVN_INFO_NOT_FOUND;
        }

#ifndef MW_EDB_SUPPORT 
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
#else
        pv_key_info = NULL;
        if (pv_key_info == NULL)
        {
            /* Key info prepare */
            t_ch_evt_id.e_brdcst_type     = pt_running_svc_req->e_brdcst_type;
            t_ch_evt_id.ui2_svl_id        = pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id;
            t_ch_evt_id.ui4_channel_id    = pt_running_svc_req->t_src_info.u_info.t_svl.ui4_channel_id;
            t_ch_evt_id.b_auto_event_id   = TRUE;

            pv_key_info = &t_ch_evt_id;
        }
 
        i4_ret = x_edb_get_info_len(
                    h_evctx,
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
#endif

    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _ch_hdlr_get_current_event_info
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *-----------------------------------------------------------------------------*/

static INT32 _ch_hdlr_get_current_event_info (
                    SVCTX_T*                        pt_svctx,
                    SVC_REQ_T*                      pt_svc_req,
#ifdef MW_EDB_SUPPORT
                    EDB_KEY_TYPE_T                  e_key_type,
#else                    
                    EVCTX_KEY_TYPE_T                e_key_type,
#endif                    
                    VOID*                           pv_key_info,
                    SIZE_T*                         pz_event_info_len,
                    VOID*                           pv_event_info
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svc_req;
    HANDLE_T      h_evctx = crnt_evn_get_handle(pt_running_svc_req->pt_crnt_event);
#ifndef MW_EDB_SUPPORT    
    EVCTX_COND_T  e_cond  = EVCTX_COND_UNAVAIL;
#else
    EDB_CH_EVT_ID_T      t_ch_evt_id = {0};
#endif
    SIZE_T        z_orig_len;
    INT32         i4_ret;
    VOID*         pv_crnt_evn_rating = NULL;

    if (pt_svctx->e_block_rule == SVCTX_BLOCK_RULE_INP_CH_PROG_SIGNAL_LOSS
#ifndef MW_EDB_SUPPORT        
            && e_key_type == EVCTX_KEY_TYPE_RATING
#else
            && e_key_type == EDB_KEY_TYPE_RATING_LIST_BY_ID
#endif
            && pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) {
        /* get rating from crnt_evn's buffer */
        if (pz_event_info_len == NULL || pv_event_info == NULL) {
            return SVCTXR_INV_ARG;
        }

        z_orig_len = crnt_evn_get_rating_size(pt_running_svc_req->pt_crnt_event);

        if (z_orig_len == 0 || crnt_evn_get_rating(pt_running_svc_req->pt_crnt_event) == NULL) {
            return SVCTXR_EVN_INFO_NOT_FOUND;
        }

        if (z_orig_len > (*pz_event_info_len)) {
            return SVCTXR_INV_ARG;
        }

        *pz_event_info_len = z_orig_len;

        pv_crnt_evn_rating = crnt_evn_get_rating(pt_running_svc_req->pt_crnt_event);

        if (pv_crnt_evn_rating)
        {
            x_memcpy(pv_event_info,
                        pv_crnt_evn_rating,
                        z_orig_len
                        );
        }
    } else {
        if (svctx_with_event_capability(
                    pt_svctx,
                    pt_running_svc_req->e_crnt_tuner_type,
                    MAKE_BIT_MASK_32(e_key_type)) == FALSE) {
            return SVCTXR_EVN_INFO_NOT_SUPPORT;
        }

        if (x_handle_valid(h_evctx) == FALSE) {
            return SVCTXR_EVN_INFO_NOT_FOUND;
        }

#ifndef MW_EDB_SUPPORT
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
#else
        pv_key_info = NULL;
        if (pv_key_info == NULL)
        {
            /* Key info prepare */
            t_ch_evt_id.e_brdcst_type     = pt_running_svc_req->e_brdcst_type;
            t_ch_evt_id.ui2_svl_id        = pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id;
            t_ch_evt_id.ui4_channel_id    = pt_running_svc_req->t_src_info.u_info.t_svl.ui4_channel_id;
            t_ch_evt_id.b_auto_event_id   = TRUE;

            pv_key_info = &t_ch_evt_id;
        }
        
        i4_ret = x_edb_get_info(
                    h_evctx,
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
#endif

    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _ch_hdlr_is_handler
 *
 * Description: This API is used to check the handler is response for this request
 * Inputs:  e_src_type         The source type
 *
 * Outputs: -
 *
 * Returns: TRUE               It is handler.
 *          FALSE              It is not handler
 *-----------------------------------------------------------------------------*/
static BOOL _ch_hdlr_is_handler(
                    SRC_TYPE_T                      e_src_type
                    )
{
    switch(e_src_type) {
    case SRC_TYPE_BRDCST_SVL:
    case SRC_TYPE_BRDCST_FREQ:
        return TRUE;
    default:
        break;
    }

    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name: _ch_hdlr_get_signal_level
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
static INT32 _ch_hdlr_get_signal_level(
                    SVCTX_T*                    pt_svctx,
                    UINT8*                      pui1_signal_level
                    )
{
    CH_SVC_REQ_T*  pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T       h_conn             = NULL_HANDLE;
    INT32          i4_ret             = SVCTXR_OK;
    CM_COND_T      e_cm_cond;
    CM_CTRL_TYPE_T e_ctrl             = ANALOG_BRDCST_CTRL_GET_SIGNAL_LEVEL;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (pui1_signal_level == NULL) {
        return SVCTXR_INV_ARG;
    }

    h_conn = player_get_conn_handle(pt_running_svc_req->pt_player);
    if (x_handle_valid(h_conn) == FALSE) {
        return SVCTXR_FAIL;
    }

    i4_ret = x_cm_get_cond (
                    h_conn,
                    &e_cm_cond
                    );
    if (i4_ret != CMR_OK || e_cm_cond != CM_COND_CONNECTED) {
        return SVCTXR_FAIL;
    }

    if (ch_hdlr_is_dtv(pt_running_svc_req)) {
        e_ctrl = BRDCST_CTRL_GET_SIGNAL_LEVEL;
    }
    i4_ret = x_cm_get(
                    h_conn,
                    e_ctrl,
                    pui1_signal_level
                    );
    if (i4_ret != CMR_OK) {
        DBG_ERROR((_ERROR_HEADER"x_cm_get(SIGNAL_LEVEL) failed, ret=%d\r\n", i4_ret));
        return SVCTXR_FAIL;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _ch_hdlr_get_dbm_signal_level
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
static INT32 _ch_hdlr_get_dbm_signal_level(
                    SVCTX_T*                    pt_svctx,
                    INT16*                      pi2_dbm_signal_level
                    )
{
    CH_SVC_REQ_T*  pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T       h_conn             = NULL_HANDLE;
    INT32          i4_ret             = SVCTXR_OK;
    CM_COND_T      e_cm_cond;
    CM_CTRL_TYPE_T e_ctrl             = ANALOG_BRDCST_CTRL_GET_DBM_SIGNAL_LEVEL;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (pi2_dbm_signal_level == NULL) {
        return SVCTXR_INV_ARG;
    }

    h_conn = player_get_conn_handle(pt_running_svc_req->pt_player);
    if (x_handle_valid(h_conn) == FALSE) {
        return SVCTXR_FAIL;
    }

    i4_ret = x_cm_get_cond (
                    h_conn,
                    &e_cm_cond
                    );
    if (i4_ret != CMR_OK || e_cm_cond != CM_COND_CONNECTED) {
        return SVCTXR_FAIL;
    }

    if (ch_hdlr_is_dtv(pt_running_svc_req)) {
        e_ctrl = BRDCST_CTRL_GET_DBM_SIGNAL_LEVEL;
    }
    i4_ret = x_cm_get(
                    h_conn,
                    e_ctrl,
                    pi2_dbm_signal_level
                    );
    if (i4_ret != CMR_OK) {
        DBG_ERROR((_ERROR_HEADER"x_cm_get(DBM_SIGNAL_LEVEL) failed, ret=%d\r\n", i4_ret));
        return SVCTXR_FAIL;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _ch_hdlr_get_tuner_status
 *
 * Description: This API is used to get tuner's status.
 *
 * Inputs:  pt_svctx           Contains a pointer to a service context.
 *
 * Outputs: pui1_tuner_status  References to the variable containing tuner's
 *                             status.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_ARG     pui1_signal_level is NULL
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_get_tuner_status(
                    SVCTX_T*                    pt_svctx,
                    UINT8*                      pui1_tuner_status
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T      h_conn             = NULL_HANDLE;
    INT32         i4_ret             = SVCTXR_OK;
    CM_COND_T     e_cm_cond;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (pui1_tuner_status == NULL) {
        return SVCTXR_INV_ARG;
    }

    h_conn = player_get_conn_handle(pt_running_svc_req->pt_player);
    if (x_handle_valid(h_conn) == FALSE) {
        return SVCTXR_FAIL;
    }

    i4_ret = x_cm_get_cond (
                    h_conn,
                    &e_cm_cond
                    );
    if (i4_ret != CMR_OK || e_cm_cond != CM_COND_CONNECTED) {
        return SVCTXR_FAIL;
    }

    i4_ret = x_cm_get(
                    h_conn,
                    BRDCST_CTRL_GET_TUNER_STATUS,
                    pui1_tuner_status
                    );
    if (i4_ret != CMR_OK) {
        DBG_INFO((_INFO_HEADER"x_cm_get(TUNER_STATUS) failed, ret=%d\r\n", i4_ret));
        return SVCTXR_FAIL;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _ch_hdlr_get_sess_attr
 *
 * Description: This function is used to get the attribuites of a session
 *
 * Inputs:  pt_svctx           Contains a pointer to a service context.
 *          e_pipe_type        Contains the stream type to get.
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
static INT32 _ch_hdlr_get_sess_attr(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    SM_GET_TYPE_T               e_get_type,
                    VOID*                       pv_get_info,
                    SIZE_T*                     pui4_get_info_size
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T      h_sess;
    INT32         i4_ret;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    h_sess = player_get_sess_handle(pt_running_svc_req->pt_player);

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
 * Name: _ch_hdlr_get_strm_attr
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
static INT32 _ch_hdlr_get_strm_attr(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    SM_GET_TYPE_T               e_get_type,
                    VOID*                       pv_get_info,
                    SIZE_T*                     pui4_get_info_size
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    INT32         i4_ret             = SVCTXR_OK;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if ((e_strm_type == ST_UNKNOWN) || (e_strm_type >= STRM_TYPE_COUNT)) {
        return SVCTXR_INV_ARG;
    }

    if (ch_hdlr_is_supported_strm_type(e_strm_type) == FALSE) {
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
 *      _ch_hdlr_strm_comp_audio_lang_filter
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static BOOL _ch_hdlr_strm_comp_audio_lang_filter (
                    UINT16                      ui2_num_recs,
                    UINT16                      ui2_idx,       /* in */
                    const STREAM_COMP_ID_T*     pt_comp_id,    /* in */
                    const SCDB_REC_T*           pt_scdb_rec,   /* in */
                    VOID*                       pv_tag         /* in */
                    )
{
    ISO_639_LANG_T* pt_lang = (ISO_639_LANG_T*) pv_tag;

    ASSERT(pt_comp_id->e_type == ST_AUDIO,
                    ("stream type (%d) is not AUDIO)", pt_comp_id->e_type));

    if ((AUD_TYPE_VISUAL_IMPAIRED != pt_scdb_rec->u.t_audio_mpeg.e_type) &&
        (0 == x_strncmp(
            (CHAR*) pt_lang,
            pt_scdb_rec->u.t_audio_mpeg.s_lang,
            MAX_LANG_LEN)))
    {
        return TRUE;
    }

    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_create_audio_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_create_audio_strm_req(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_svctx,
                    ISO_639_LANG_T*             pt_lang,
                    x_svctx_select_nfy_fct      pf_nfy,
                    VOID*                       pv_nfy_tag,
                    STRM_REQ_T**                ppt_strm_req
                    )
{
    STRM_REQ_T*                     pt_strm_req = NULL;
    INT32                           i4_ret;
    INT32                           i, j;

    if (ppt_strm_req == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (pt_lang == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (ch_hdlr_is_supported_strm_type(ST_AUDIO) == FALSE) {
        return SVCTXR_NOT_SUPPORT;
    }

    ppt_strm_req[STRM_IDX_NORMAL] = ppt_strm_req[STRM_IDX_VISUAL_IMPAIRED] = NULL;

    j = ((pt_svctx->b_ad_enabled) ? STRM_MAX_CONCURRENT : 1);

    for (i = 0; i < j; i++)
    {
        pt_strm_req = svctx_allocate_strm_req(pt_svctx->pt_running_svc_req->pt_hdlr); /* QQQQQQ */

        if (NULL == pt_strm_req)
        {
            if (STRM_IDX_NORMAL == i)
            {
                DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
                return SVCTXR_NO_RESOURCE;
            }
            else
            {
                break;
            }
        }

        i4_ret = _ch_hdlr_get_strm_comp_id(
                        pt_svctx,
                        ST_AUDIO,
                        TRUE,
                        ((STRM_IDX_NORMAL == i) ?
                            _ch_hdlr_strm_comp_audio_lang_filter : ch_hdlr_strm_comp_ad_lang_filter),
                        pt_lang,
                        &(pt_strm_req->u.t_scdb.t_comp_id),
                        &(pt_strm_req->u.t_scdb.t_rec)
                        );

        if (i4_ret != SVCTXR_OK)
        {
            svctx_free_strm_req(pt_strm_req);

            if (STRM_IDX_NORMAL == i)
            {
                DBG_ERROR((_ERROR_HEADER"no such audio(%s) comp existed!\r\n", (CHAR*)pt_lang));
                return SVCTXR_NO_RECORD;
            }
            else
            {
                break;
            }
        }

        pt_strm_req->h_svctx    = h_svctx;
        pt_strm_req->e_req_type = STRM_REQ_SCDB;

        if (STRM_IDX_NORMAL == i)
        {
            pt_strm_req->pf_strm_nfy     = pf_nfy;
            pt_strm_req->pv_strm_nfy_tag = pv_nfy_tag;
            pt_strm_req->b_aux_audio     = FALSE;
        }
        else
        {
            pt_strm_req->pf_strm_nfy     = NULL;
            pt_strm_req->pv_strm_nfy_tag = NULL;
            pt_strm_req->b_aux_audio     = TRUE;
        }

        ppt_strm_req[i] = pt_strm_req;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_create_audio_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_create_mixed_audio_strm_req(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_svctx,
                    ISO_639_LANG_T*             pt_lang,
                    x_svctx_select_nfy_fct      pf_nfy,
                    VOID*                       pv_nfy_tag,
                    STRM_REQ_T**                ppt_strm_req
                    )
{
    STRM_REQ_T*         pt_strm_req = NULL;
    INT32               i4_ret;

    if (!ch_hdlr_is_valid_for_ad_select(pt_svctx, (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req))
    {
        return SVCTXR_NOT_SUPPORT;
    }

    if (ppt_strm_req == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (pt_lang == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (ch_hdlr_is_supported_strm_type(ST_AUDIO) == FALSE) {
        return SVCTXR_NOT_SUPPORT;
    }

    *ppt_strm_req = NULL;

    pt_strm_req = svctx_allocate_strm_req(pt_svctx->pt_running_svc_req->pt_hdlr); /* QQQQQQ */

    if (NULL == pt_strm_req)
    {
        DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
        return SVCTXR_NO_RESOURCE;
    }

    i4_ret = _ch_hdlr_get_strm_comp_id(
        pt_svctx,
        ST_AUDIO,
        TRUE,
        ch_hdlr_strm_comp_ad_lang_filter,
        pt_lang,
        &(pt_strm_req->u.t_scdb.t_comp_id),
        &(pt_strm_req->u.t_scdb.t_rec));

    if (i4_ret != SVCTXR_OK)
    {
        svctx_free_strm_req(pt_strm_req);

        DBG_ERROR((_ERROR_HEADER"no such audio(%s) comp existed!\r\n", (CHAR*)pt_lang));
        return SVCTXR_NO_RECORD;
    }

    pt_strm_req->h_svctx    = h_svctx;
    pt_strm_req->e_req_type = STRM_REQ_SCDB;

    pt_strm_req->pf_strm_nfy     = NULL;
    pt_strm_req->pv_strm_nfy_tag = NULL;
    pt_strm_req->b_aux_audio     = TRUE;

    *ppt_strm_req = pt_strm_req;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_create_generic_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_create_generic_strm_req(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_svctx,
                    STREAM_COMP_ID_T*           pt_comp_id,
                    x_svctx_select_nfy_fct      pf_nfy,
                    VOID*                       pv_nfy_tag,
                    STRM_REQ_T**                ppt_strm_req
                    )
{
    STRM_REQ_T*   pt_strm_req        = NULL;
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T      h_scdb             = NULL_HANDLE;
    SCDB_REC_T    t_scdb_rec         = {0};
    INT32         i4_ret;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (ppt_strm_req == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (pt_comp_id == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (ch_hdlr_is_supported_strm_type(pt_comp_id->e_type) == FALSE) {
        return SVCTXR_NOT_SUPPORT;
    }

    ppt_strm_req[STRM_IDX_NORMAL] = ppt_strm_req[STRM_IDX_VISUAL_IMPAIRED] = NULL;

    pt_strm_req = svctx_allocate_strm_req(pt_svctx->pt_running_svc_req->pt_hdlr);
    if (pt_strm_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
        return SVCTXR_NO_RESOURCE;
    }

    pt_strm_req->h_svctx            = h_svctx;
    pt_strm_req->pf_strm_nfy        = pf_nfy;
    pt_strm_req->pv_strm_nfy_tag    = pv_nfy_tag;
    pt_strm_req->e_req_type         = STRM_REQ_SCDB;
    pt_strm_req->b_aux_audio        = FALSE;

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (IS_SVCTX_ASSOCIATE(pt_svctx))
    {
        SVCTX_T*            pt_lead;
        CH_SVC_REQ_T*       pt_lead_svc_req;
        SCDB_REC_T          t_lead_scdb_rec = {0};
        UINT32              ui4_ver = SCDB_NULL_VER_ID;
        UINT16              ui2_idx = 0;
        BOOL                b_done  = FALSE;

        i4_ret = SCDBR_REC_NOT_FOUND;

        pt_lead = svctx_mgr_get_link_lead(
            ACTIVE_LINK_NOW,
            pt_svctx->pt_running_svc_req->e_src_type);

        if (pt_lead)
        {
            pt_lead_svc_req = (CH_SVC_REQ_T*) pt_lead->pt_running_svc_req;

            /* get the SCDB record of the lead SVCTX */
            i4_ret = x_scdb_get_rec(
                player_get_scdb_handle(pt_lead_svc_req->pt_player),
                pt_comp_id,
                &t_lead_scdb_rec);

            if (SCDBR_OK == i4_ret)
            {
                h_scdb = player_get_scdb_handle(pt_running_svc_req->pt_player);

                while (!b_done)
                {
                    if (SCDBR_OK == x_scdb_get_rec_by_idx(
                        h_scdb,
                        pt_comp_id->e_type,
                        ui2_idx,
                        &(pt_strm_req->u.t_scdb.t_comp_id),
                        &t_scdb_rec,
                        &ui4_ver))
                    {
                        if (t_scdb_rec.e_rec_type == t_lead_scdb_rec.e_rec_type)
                        {
                            switch (t_scdb_rec.e_rec_type)
                            {
                                case SCDB_REC_TYPE_AUDIO_MPEG:
                                    if ((t_scdb_rec.u.t_audio_mpeg.ui2_pid   == t_lead_scdb_rec.u.t_audio_mpeg.ui2_pid) &&
                                        (t_scdb_rec.u.t_audio_mpeg.ui1_index == t_lead_scdb_rec.u.t_audio_mpeg.ui1_index))
                                    {
                                        b_done = TRUE;
                                    }
                                    break;

                                case SCDB_REC_TYPE_VIDEO_MPEG:
                                    if (t_scdb_rec.u.t_video_mpeg.ui2_pid == t_lead_scdb_rec.u.t_video_mpeg.ui2_pid)
                                    {
                                        b_done = TRUE;
                                    }
                                    break;

#ifdef MW_PIP_POP_SUPPORT
                                case SCDB_REC_TYPE_AUDIO_ANALOG:
                                    b_done = TRUE;
                                    break;
#endif
                            }
                        }

                        ui2_idx++;
                    }
                    else
                    {
                        i4_ret = SCDBR_REC_NOT_FOUND;
                        break;
                    }
                }
            }
        }
    }
    else
#endif
    {
        pt_strm_req->u.t_scdb.t_comp_id = *pt_comp_id;

        h_scdb = player_get_scdb_handle(pt_running_svc_req->pt_player);

        i4_ret = x_scdb_get_rec (
                        h_scdb,
                        &(pt_strm_req->u.t_scdb.t_comp_id),
                        &t_scdb_rec
                        );
    }

    if (i4_ret == SCDBR_REC_NOT_FOUND) {
        svctx_free_strm_req(pt_strm_req);
        return SVCTXR_NO_RECORD;
    }

    if (i4_ret != SCDBR_OK) {
        DBG_ERROR((_ERROR_HEADER"failed to x_scdb_get_rec, ret=%d\r\n", i4_ret));
        svctx_free_strm_req(pt_strm_req);
        return SVCTXR_FAIL;
    }

    pt_strm_req->u.t_scdb.t_rec = t_scdb_rec;
    ppt_strm_req[0] = pt_strm_req;

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (!IS_SVCTX_ASSOCIATE(pt_svctx))
#endif
    {
        /* create stream request for AD */
        if ((pt_svctx->b_ad_enabled) && (ST_AUDIO == pt_comp_id->e_type))
        {
            pt_strm_req = svctx_allocate_strm_req(pt_svctx->pt_running_svc_req->pt_hdlr);
            if (pt_strm_req == NULL) {
                DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
                return SVCTXR_NO_RESOURCE;
            }

            pt_strm_req->h_svctx            = h_svctx;
            pt_strm_req->pf_strm_nfy        = NULL;
            pt_strm_req->pv_strm_nfy_tag    = NULL;
            pt_strm_req->e_req_type         = STRM_REQ_SCDB;
            pt_strm_req->b_aux_audio        = TRUE;

            i4_ret = _ch_hdlr_get_strm_comp_id(
                            pt_svctx,
                            ST_AUDIO,
                            TRUE,
                            ch_hdlr_strm_comp_ad_filter,
                            &t_scdb_rec,
                            &(pt_strm_req->u.t_scdb.t_comp_id),
                            &(pt_strm_req->u.t_scdb.t_rec)
                            );

            if (i4_ret != SVCTXR_OK)
            {
                svctx_free_strm_req(pt_strm_req);
                pt_strm_req = NULL;
            }

            ppt_strm_req[STRM_IDX_VISUAL_IMPAIRED] = pt_strm_req;
        }
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_create_mpeg_pid_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_create_mpeg_pid_strm_req(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_svctx,
                    MEPG_PID_STRM_SEL_INFO_T*   pt_mpeg,
                    x_svctx_select_nfy_fct      pf_nfy,
                    VOID*                       pv_nfy_tag,
                    STRM_REQ_T**                ppt_strm_req
                    )
{
    STRM_REQ_T*   pt_strm_req        = NULL;
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (ppt_strm_req == NULL || pt_mpeg->t_mpeg_2_pid >= MPEG_2_NULL_PID) {
        return SVCTXR_INV_ARG;
    }

    switch(pt_mpeg->e_strm_type) {
    case ST_AUDIO:
    case ST_VIDEO:
    case ST_SUBTITLE:
#ifdef MW_ISDB_SUPPORT
    case ST_ISDB_CAPTION:
    case ST_ISDB_TEXT:
#endif
        break;
    default:
        return SVCTXR_NOT_SUPPORT;
    }

    pt_strm_req = svctx_allocate_strm_req(pt_svctx->pt_running_svc_req->pt_hdlr);
    if (pt_strm_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
        return SVCTXR_NO_RESOURCE;
    }

    pt_strm_req->h_svctx         = h_svctx;
    pt_strm_req->pf_strm_nfy     = pf_nfy;
    pt_strm_req->pv_strm_nfy_tag = pv_nfy_tag;
    pt_strm_req->e_req_type      = STRM_REQ_PID;
    pt_strm_req->u.t_pid         = *pt_mpeg;

    *ppt_strm_req = pt_strm_req;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_strm_comp_subtitle_lang_filter
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static BOOL _ch_hdlr_strm_comp_subtitle_lang_filter (
                    UINT16                      ui2_num_recs,
                    UINT16                      ui2_idx,       /* in */
                    const STREAM_COMP_ID_T*     pt_comp_id,    /* in */
                    const SCDB_REC_T*           pt_scdb_rec,   /* in */
                    VOID*                       pv_tag         /* in */
                    )
{
    ISO_639_LANG_T* pt_lang = (ISO_639_LANG_T*) pv_tag;
    INT32           i4_ret  = 0;

    ASSERT(pt_comp_id->e_type == ST_SUBTITLE,
                    ("stream type (%d) is not SUBTITLE)", pt_comp_id->e_type));

    i4_ret = x_strncmp(
                    (CHAR*)pt_lang,
                    pt_scdb_rec->u.t_dvb_sbtl.s_lang,
                    MAX_LANG_LEN
                    );

    return (i4_ret == 0 ? TRUE : FALSE);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_create_subtitle_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_create_subtitle_strm_req(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_svctx,
                    HANDLE_T                    h_gl_plane,
                    ISO_639_LANG_T*             pt_lang,
                    x_svctx_select_nfy_fct      pf_nfy,
                    VOID*                       pv_nfy_tag,
                    STRM_REQ_T**                ppt_strm_req
                    )
{
    STRM_REQ_T* pt_strm_req = NULL;
    INT32       i4_ret;

    if (ppt_strm_req == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (x_handle_valid(h_gl_plane) == FALSE || pt_lang == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (ch_hdlr_is_supported_strm_type(ST_SUBTITLE) == FALSE) {
        return SVCTXR_NOT_SUPPORT;
    }

    pt_strm_req = svctx_allocate_strm_req(pt_svctx->pt_running_svc_req->pt_hdlr); /* QQQQQQ */
    if (pt_strm_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
        return SVCTXR_NO_RESOURCE;
    }
    pt_strm_req->h_svctx         = h_svctx;
    pt_strm_req->pf_strm_nfy     = pf_nfy;
    pt_strm_req->pv_strm_nfy_tag = pv_nfy_tag;
    pt_strm_req->h_gl_plane      = h_gl_plane;
    pt_strm_req->e_req_type      = STRM_REQ_SCDB;
    pt_strm_req->t_sbtl_info.h_scart_plane  = NULL_HANDLE;

    i4_ret = _ch_hdlr_get_strm_comp_id(
                    pt_svctx,
                    ST_SUBTITLE,
                    TRUE,
                    _ch_hdlr_strm_comp_subtitle_lang_filter,
                    pt_lang,
                    &(pt_strm_req->u.t_scdb.t_comp_id),
                    &(pt_strm_req->u.t_scdb.t_rec)
                    );
    if (i4_ret != SVCTXR_OK) {
        DBG_ERROR((_ERROR_HEADER"no such audio(%s) comp existed!\r\n", (CHAR*)pt_lang));
        svctx_free_strm_req(pt_strm_req);
        return SVCTXR_NO_RECORD;
    }

    *ppt_strm_req = pt_strm_req;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_create_generic_subtitle_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_create_generic_subtitle_strm_req(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_svctx,
                    HANDLE_T                    h_gl_plane,
                    SM_SBTLH_STREAM_INFO*       pt_sbtl_info,
                    STREAM_COMP_ID_T*           pt_comp_id,
                    x_svctx_select_nfy_fct      pf_nfy,
                    VOID*                       pv_nfy_tag,
                    STRM_REQ_T**                ppt_strm_req
                    )
{
    STRM_REQ_T*   pt_strm_req        = NULL;
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    INT32         i4_ret;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (ppt_strm_req == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (x_handle_valid(h_gl_plane) == FALSE || pt_comp_id == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (ch_hdlr_is_supported_strm_type(ST_SUBTITLE) == FALSE) {
        return SVCTXR_NOT_SUPPORT;
    }

    pt_strm_req = svctx_allocate_strm_req(pt_running_svc_req->t_parent.pt_hdlr);
    if (pt_strm_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
        return SVCTXR_NO_RESOURCE;
    }
    pt_strm_req->h_svctx            = h_svctx;
    pt_strm_req->pf_strm_nfy        = pf_nfy;
    pt_strm_req->pv_strm_nfy_tag    = pv_nfy_tag;
    pt_strm_req->h_gl_plane         = h_gl_plane;
    pt_strm_req->e_req_type         = STRM_REQ_SCDB;
    pt_strm_req->u.t_scdb.t_comp_id = *pt_comp_id;

    if (NULL == pt_sbtl_info)
    {
        pt_strm_req->t_sbtl_info.h_scart_plane = NULL_HANDLE;
        pt_strm_req->t_sbtl_info.b_open_main_invisible = FALSE;
    }
    else
    {
        pt_strm_req->t_sbtl_info = *pt_sbtl_info;
    }

    i4_ret = x_scdb_get_rec (
                    player_get_scdb_handle(pt_running_svc_req->pt_player),
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
 *      _ch_hdlr_create_cc_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_create_cc_strm_req(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_svctx,
                    HANDLE_T                    h_gl_plane,
                    SM_CC_FILTER_T*             pt_cc_filter,
                    x_svctx_select_nfy_fct      pf_nfy,
                    VOID*                       pv_nfy_tag,
                    STRM_REQ_T**                ppt_strm_req
                    )
{
    STRM_REQ_T* pt_strm_req = NULL;
    INT32       i4_ret;

    if (ppt_strm_req == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (x_handle_valid(h_gl_plane) == FALSE || pt_cc_filter == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (ch_hdlr_is_supported_strm_type(ST_CLOSED_CAPTION) == FALSE) {
        return SVCTXR_NOT_SUPPORT;
    }

    pt_strm_req = svctx_allocate_strm_req(pt_svctx->pt_running_svc_req->pt_hdlr);
    if (pt_strm_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
        return SVCTXR_NO_RESOURCE;
    }

    pt_strm_req->h_svctx         = h_svctx;
    pt_strm_req->pf_strm_nfy     = pf_nfy;
    pt_strm_req->pv_strm_nfy_tag = pv_nfy_tag;
    pt_strm_req->t_cc_filter     = *pt_cc_filter;
    pt_strm_req->h_gl_plane      = h_gl_plane;
    pt_strm_req->e_req_type      = STRM_REQ_SCDB;

    i4_ret = _ch_hdlr_get_strm_comp_id(
                    pt_svctx,
                    ST_CLOSED_CAPTION,
                    TRUE,
                    svctx_strm_comp_cc_filter,
                    (VOID*)pt_cc_filter,
                    &(pt_strm_req->u.t_scdb.t_comp_id),
                    &(pt_strm_req->u.t_scdb.t_rec)
                    );
    /* if can't find any stream comp id, then force to set it by cc filter */
    if (i4_ret != SVCTXR_OK) {
        pt_strm_req->u.t_scdb.t_comp_id.e_type        = ST_CLOSED_CAPTION;
        pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag = NULL;
    }

    *ppt_strm_req = pt_strm_req;

    return SVCTXR_OK;
}
#ifdef MW_ISDB_SUPPORT
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_create_isdb_caption_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_create_isdb_caption_strm_req(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_svctx,
                    HANDLE_T                    h_gl_plane,
                    UINT32                      ui4_caption_idx,
                    x_svctx_select_nfy_fct      pf_nfy,
                    VOID*                       pv_nfy_tag,
                    STRM_REQ_T**                ppt_strm_req
                    )
{
    STRM_REQ_T* pt_strm_req = NULL;
    STREAM_COMP_ID_T        t_comp_id   = {ST_UNKNOWN, NULL};
    SCDB_REC_T              t_rec       = {0};

    if (ppt_strm_req == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (ch_hdlr_is_supported_strm_type(ST_ISDB_CAPTION) == FALSE) {
        return SVCTXR_NOT_SUPPORT;
    }

    pt_strm_req = svctx_allocate_strm_req(pt_svctx->pt_running_svc_req->pt_hdlr);
    if (pt_strm_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
        return SVCTXR_NO_RESOURCE;
    }

    if (ui4_caption_idx < 1 || ui4_caption_idx > 8)
    {
        ui4_caption_idx = 1;
    }

    pt_strm_req->h_svctx                          = h_svctx;
    pt_strm_req->pf_strm_nfy                      = pf_nfy;
    pt_strm_req->pv_strm_nfy_tag                  = pv_nfy_tag;
    pt_strm_req->h_gl_plane                       = h_gl_plane;
    pt_strm_req->ui4_isdbcc_idx                   = ui4_caption_idx;
    pt_strm_req->e_req_type                       = STRM_REQ_SCDB;
    pt_strm_req->u.t_scdb.t_comp_id.e_type        = ST_ISDB_CAPTION;
    pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag = NULL;

    x_memset( &(pt_strm_req->u.t_scdb.t_rec), 0, sizeof(SCDB_REC_T) );
    
    _ch_hdlr_get_strm_comp_id(
                    pt_svctx,
                    ST_ISDB_CAPTION,
                    TRUE,
                    svctx_strm_comp_isdb_cc_filter,
                    (VOID*)pt_strm_req,
                    &t_comp_id,
                    &t_rec
                    );

    *ppt_strm_req = pt_strm_req;

    return SVCTXR_OK;
}

static INT32 _ch_hdlr_create_isdb_text_strm_req(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_svctx,
                    HANDLE_T                    h_gl_plane,
                    UINT32                      ui4_text_idx,
                    x_svctx_select_nfy_fct      pf_nfy,
                    VOID*                       pv_nfy_tag,
                    STRM_REQ_T**                ppt_strm_req
                    )
{
    STRM_REQ_T* pt_strm_req = NULL;
    STREAM_COMP_ID_T        t_comp_id   = {ST_UNKNOWN, NULL};
    SCDB_REC_T              t_rec       = {0};

    if (ppt_strm_req == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (ch_hdlr_is_supported_strm_type(ST_ISDB_TEXT) == FALSE) {
        return SVCTXR_NOT_SUPPORT;
    }

    pt_strm_req = svctx_allocate_strm_req(pt_svctx->pt_running_svc_req->pt_hdlr);
    if (pt_strm_req == NULL) {
        DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
        return SVCTXR_NO_RESOURCE;
    }

    
    if (ui4_text_idx < 1 || ui4_text_idx > 8)
    {
        ui4_text_idx = 1;
    }

    pt_strm_req->h_svctx                          = h_svctx;
    pt_strm_req->pf_strm_nfy                      = pf_nfy;
    pt_strm_req->pv_strm_nfy_tag                  = pv_nfy_tag;
    pt_strm_req->h_gl_plane                       = h_gl_plane;
    pt_strm_req->ui4_isdbcc_idx                   = ui4_text_idx;
    pt_strm_req->e_req_type                       = STRM_REQ_SCDB;
    pt_strm_req->u.t_scdb.t_comp_id.e_type        = ST_ISDB_TEXT;
    pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag = NULL;

    x_memset( &(pt_strm_req->u.t_scdb.t_rec), 0, sizeof(SCDB_REC_T) );
    
    _ch_hdlr_get_strm_comp_id(
                    pt_svctx,
                    ST_ISDB_TEXT,
                    TRUE,
                    svctx_strm_comp_isdb_text_filter,
                    (VOID*)pt_strm_req,
                    &t_comp_id,
                    &t_rec
                    );

    *ppt_strm_req = pt_strm_req;

    return SVCTXR_OK;
}
#endif
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_create_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_create_strm_req(
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
        i4_ret = _ch_hdlr_create_audio_strm_req(
                    pt_svctx,
                    h_svctx,
                    pt_info->u.t_audio.pt_lang,
                    pf_nfy,
                    pv_nfy_tag,
                    ppt_strm_req
                    );
        break;
    case CH_STRM_SEL_INFO_TYPE_MIXED_AUDIO:
        i4_ret = _ch_hdlr_create_mixed_audio_strm_req(
                    pt_svctx,
                    h_svctx,
                    pt_info->u.t_audio.pt_lang,
                    pf_nfy,
                    pv_nfy_tag,
                    ppt_strm_req
                    );
        break;
    case CH_STRM_SEL_INFO_TYPE_CC:
        i4_ret = _ch_hdlr_create_cc_strm_req(
                    pt_svctx,
                    h_svctx,
                    pt_info->u.t_cc.h_gl_plane,
                    pt_info->u.t_cc.pt_cc_filter,
                    pf_nfy,
                    pv_nfy_tag,
                    ppt_strm_req
                    );
        break;
#ifdef MW_ISDB_SUPPORT
    case CH_STRM_SEL_INFO_TYPE_ISDB_CAPTION:
        i4_ret = _ch_hdlr_create_isdb_caption_strm_req(
                    pt_svctx,
                    h_svctx,
                    pt_info->u.t_isdb_caption.h_gl_plane,
                    pt_info->u.t_isdb_caption.ui4_caption_idx,
                    pf_nfy,
                    pv_nfy_tag,
                    ppt_strm_req
                    );
        break;
    case CH_STRM_SEL_INFO_TYPE_ISDB_TEXT:
        i4_ret = _ch_hdlr_create_isdb_text_strm_req(
                    pt_svctx,
                    h_svctx,
                    pt_info->u.t_isdb_text.h_gl_plane,
                    pt_info->u.t_isdb_text.ui4_text_idx,
                    pf_nfy,
                    pv_nfy_tag,
                    ppt_strm_req
                    );
        break;
#endif
    case CH_STRM_SEL_INFO_TYPE_SUBTITLE:
        i4_ret = _ch_hdlr_create_subtitle_strm_req(
                    pt_svctx,
                    h_svctx,
                    pt_info->u.t_subtitle.h_gl_plane,
                    pt_info->u.t_subtitle.pt_lang,
                    pf_nfy,
                    pv_nfy_tag,
                    ppt_strm_req
                    );
        break;
    case CH_STRM_SEL_INFO_TYPE_GENERIC_SUBTITLE:
        i4_ret = _ch_hdlr_create_generic_subtitle_strm_req(
                    pt_svctx,
                    h_svctx,
                    pt_info->u.t_subtitle_generic.h_gl_plane,
                    pt_info->u.t_subtitle_generic.pt_sbtl_info,
                    pt_info->u.t_subtitle_generic.pt_comp_id,
                    pf_nfy,
                    pv_nfy_tag,
                    ppt_strm_req
                    );
        break;
    case CH_STRM_SEL_INFO_TYPE_GENERIC:
        i4_ret = _ch_hdlr_create_generic_strm_req(
                    pt_svctx,
                    h_svctx,
                    pt_info->u.t_generic.pt_comp_id,
                    pf_nfy,
                    pv_nfy_tag,
                    ppt_strm_req
                    );
        break;
    case CH_STRM_SEL_INFO_TYPE_MEPG_PID:
        i4_ret = _ch_hdlr_create_mpeg_pid_strm_req(
                    pt_svctx,
                    h_svctx,
                    &pt_info->u.t_mpeg,
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
 *      _ch_hdlr_reload_cc_strm_as_presenting
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static VOID _ch_hdlr_reload_cc_strm_as_presenting (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_new_svc_req
                    )
{
    STRM_REQ_T*      pt_strm_req = &(pt_new_svc_req->t_strm_reqs[ST_CLOSED_CAPTION]);
    STREAM_COMP_ID_T t_comp_id;
    INT32 i4_ret;

    pt_strm_req->e_req_type                = STRM_REQ_SCDB;
    pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_CLOSED_CAPTION;
    i4_ret = ch_hdlr_open_strm(
                    pt_svctx,
                    pt_strm_req,
                    TRUE
                    );
    if (i4_ret != SVCTXR_OK) {
        pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_UNKNOWN;
        pt_strm_req->e_penging_status          = STRM_PENDING_COND_NONE;
        return;
    }

    pt_strm_req->h_svctx         = pt_new_svc_req->t_parent.h_svctx;
    pt_strm_req->pf_strm_nfy     = pt_new_svc_req->t_parent.pf_svc_nfy;
    pt_strm_req->pv_strm_nfy_tag = pt_new_svc_req->t_parent.pv_svc_nfy_tag;

    i4_ret = _ch_hdlr_get_strm_comp_id(
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
 *      _ch_hdlr_deinit
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static VOID _ch_hdlr_deinit(
                    VOID
                    )
{
    ch_svc_req_deinit();
    ch_strm_req_deinit();
    crnt_evn_deinit();
    rating_chker_deinit();

    b_g_init = FALSE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_get
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_get(
                    SVCTX_T*                    pt_svctx,
                    SVCTX_GET_TYPE_T            e_get_type,
                    VOID*                       pv_get_info,
                    SIZE_T*                     pz_get_info_size
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    INT32         i4_ret             = SVCTXR_OK;
    UINT32        ui4_db_version     = SCDB_NULL_VER_ID;
    HANDLE_T      h_scdb;

#ifdef SYS_MHP_SUPPORT
    HANDLE_T      h_sess;
#endif
    if (NULL == pt_running_svc_req) {
        return SVCTXR_WRONG_STATE;
    }

    switch (e_get_type) {
    case SVCTX_COMMON_GET_TYPE_CRNT_VIDEO_DEVICE_TYPE:
    case SVCTX_COMMON_GET_TYPE_CRNT_AUDIO_DEVICE_TYPE:
        if ((*pz_get_info_size) != sizeof(DEVICE_TYPE_T) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }
        *((DEVICE_TYPE_T*)pv_get_info) = pt_running_svc_req->e_crnt_tuner_type;
        break;

    case SVCTX_COMMON_GET_TYPE_BLOCK_COND:
        if ((*pz_get_info_size) != sizeof(SVC_BLOCK_COND_T) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }
        *((SVC_BLOCK_COND_T*)pv_get_info) = rating_chker_get_block_cond(pt_running_svc_req->pt_rating_chker);
        break;

    case SVCTX_CH_GET_TYPE_CH_SRC:
        if ((*pz_get_info_size) != sizeof(CH_SRC_TYPE_T) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }
        *((CH_SRC_TYPE_T*)pv_get_info) = pt_running_svc_req->e_ch_src_type;
        break;

    case SVCTX_COMMON_GET_SCDB_NUM_RECS:
        if ((*pz_get_info_size) != sizeof(SVCTX_GET_SCDB_NUM_RECS_T) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        h_scdb = player_get_scdb_handle(pt_running_svc_req->pt_player);
        if (x_handle_valid(h_scdb) == FALSE) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        i4_ret = x_scdb_get_num_recs(
                    h_scdb,
                    ((SVCTX_GET_SCDB_NUM_RECS_T*)pv_get_info)->e_strm_type,
                    &(((SVCTX_GET_SCDB_NUM_RECS_T*)pv_get_info)->ui2_num_recs),
                    &ui4_db_version
                    );
        i4_ret = svctx_translate_scdbr_2_svctxr(i4_ret);
        break;
    case SVCTX_COMMON_GET_SCDB_REC_BY_IDX:
        if ((*pz_get_info_size) != sizeof(SVCTX_GET_SCDB_REC_BY_IDX_T) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        h_scdb = player_get_scdb_handle(pt_running_svc_req->pt_player);
        if (x_handle_valid(h_scdb) == FALSE) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }

        i4_ret = x_scdb_get_rec_by_idx(
                    h_scdb,
                    ((SVCTX_GET_SCDB_REC_BY_IDX_T*)pv_get_info)->e_strm_type,
                    ((SVCTX_GET_SCDB_REC_BY_IDX_T*)pv_get_info)->ui2_idx,
                    &(((SVCTX_GET_SCDB_REC_BY_IDX_T*)pv_get_info)->t_strm_comp_id),
                    &(((SVCTX_GET_SCDB_REC_BY_IDX_T*)pv_get_info)->t_scdb_rec),
                    &ui4_db_version
                    );
        i4_ret = svctx_translate_scdbr_2_svctxr(i4_ret);
        break;
    case SVCTX_COMMON_GET_USING_SCDB_REC:
        if ((*pz_get_info_size) != sizeof(SVCTX_GET_USING_SCDB_REC_T) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        i4_ret = _ch_hdlr_get_using_scdb_info_ex(
                    pt_svctx,
                    ((SVCTX_GET_USING_SCDB_REC_T*)pv_get_info)->e_strm_type,
                    &(((SVCTX_GET_USING_SCDB_REC_T*)pv_get_info)->t_strm_comp_id),
                    &(((SVCTX_GET_USING_SCDB_REC_T*)pv_get_info)->t_scdb_rec),
                    &(((SVCTX_GET_USING_SCDB_REC_T*)pv_get_info)->ui2_idx)
                    );
        break;
    case SVCTX_COMMON_GET_CRNT_SVL_REC_ID:
        if ((*pz_get_info_size) != sizeof(UINT16) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        if (svctx_rikstv_get_original_svl_id_rec_id(pt_svctx, NULL, (UINT16*)pv_get_info) != SVCTXR_OK)
        {
            *((UINT16*)pv_get_info) = pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svl_rec_id;
        }
        break;
    case SVCTX_COMMON_GET_CRNT_SVL_ID:
        if ((*pz_get_info_size) != sizeof(UINT16) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        if (svctx_rikstv_get_original_svl_id_rec_id(pt_svctx, (UINT16*)pv_get_info, NULL) != SVCTXR_OK)
        {
            *((UINT16*)pv_get_info) = pt_running_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id;
        }
        break;

    case SVCTX_COMMON_GET_AD_RECS_NUM:
        if ((*pz_get_info_size) != sizeof(UINT16)) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }
        i4_ret = _ch_hdlr_get_ad_num_recs(pt_svctx, (UINT16*)pv_get_info);
        break;

    case SVCTX_COMMON_GET_AD_RECS:
        if ((*pz_get_info_size) < sizeof(SCDB_REC_T)) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }
        i4_ret = _ch_hdlr_get_ad_recs(
                        pt_svctx,
                        (*pz_get_info_size)/sizeof(SCDB_REC_T),
                        (SCDB_REC_T*)pv_get_info);
        break;

    case SVCTX_COMMON_GET_AD_USING_REC:
        if ((*pz_get_info_size) != sizeof(SCDB_REC_T)) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }
        i4_ret = _ch_hdlr_get_using_ad_info(
                        pt_svctx,
                        (SCDB_REC_T*)pv_get_info);
        break;

    case SVCTX_COMMON_GET_AD_SUITABILITY:
        if ((*pz_get_info_size) != sizeof(BOOL)){
            i4_ret = SVCTXR_INV_GET_INFO;
        }
        *((BOOL*) pv_get_info) = ch_hdlr_is_suitable_for_ad(
                                     pt_svctx, 
                                     (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req);
        break;

    case SVCTX_CH_GET_SILENT_STATUS:
        if ((*pz_get_info_size) != sizeof(BOOL) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        *((BOOL*)pv_get_info) = pt_running_svc_req->t_src_info.u_info.t_svl.t_silent.b_enable;
        break;
    case SVCTX_CH_GET_SILENT_SVC_REC_ID:
        if ((*pz_get_info_size) != sizeof(UINT16) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        *((UINT16*)pv_get_info) = pt_running_svc_req->t_src_info.u_info.t_svl.t_silent.ui2_svl_rec_id;
        break;
#ifdef SYS_MHP_SUPPORT
    case SVCTX_CH_GET_SILENT_SVL_LST_ID:
        if ((*pz_get_info_size) != sizeof(UINT16) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }
    
        *((UINT16*)pv_get_info) = pt_running_svc_req->t_src_info.u_info.t_svl.t_silent.ui2_svc_lst_id;
        break;
#endif
    case SVCTX_CH_GET_AUX_AUDIO_STATUS:
        if ((*pz_get_info_size) != sizeof(SM_COND_T) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        *((SM_COND_T*) pv_get_info) = pt_running_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX].e_strm_cond;
        break;
    case SVCTX_CH_GET_BRDCST_TYPE:
        if ((*pz_get_info_size) != sizeof(BRDCST_TYPE_T) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        *((BRDCST_TYPE_T*) pv_get_info) = pt_running_svc_req->e_brdcst_type;
        break;
    case SVCTX_CH_GET_SRC_SVL_INFO:
        if ((*pz_get_info_size) != sizeof(SRC_SVL_INFO_T) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        if (pt_running_svc_req->t_parent.e_src_type != SRC_TYPE_BRDCST_SVL)
        {
            i4_ret = SVCTXR_NOT_EXIST;
            break;
        }

        *((SRC_SVL_INFO_T*) pv_get_info) = pt_running_svc_req->t_src_info.u_info.t_svl;
        break;
    case SVCTX_CH_GET_SRC_FREQ_INFO:
        if ((*pz_get_info_size) != sizeof(SRC_FREQ_INFO_T) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        if (pt_running_svc_req->t_parent.e_src_type != SRC_TYPE_BRDCST_FREQ)
        {
            i4_ret = SVCTXR_NOT_EXIST;
            break;
        }

        *((SRC_FREQ_INFO_T*) pv_get_info) = pt_running_svc_req->t_src_info.u_info.t_freq;
        break;
    case SVCTX_CH_GET_FIRST_CH_SURF:
        if ((*pz_get_info_size) != sizeof(BOOL) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        *((BOOL*)pv_get_info) = ch_hdlr_is_first_ch_surf();
        break;              
    case SVCTX_CH_GET_NON_BRDCSTING_SVC:
        if ((*pz_get_info_size) != sizeof(BOOL) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

#ifdef MW_ISDB_SUPPORT
        *((BOOL*)pv_get_info) = !pt_svctx->b_is_brdcsting_svc;
#else
        *((BOOL*)pv_get_info) = FALSE;
#endif
        break;
#ifdef SYS_MHP_SUPPORT
     case SVCTX_CH_GET_CUR_STC:
        if ((*pz_get_info_size) != sizeof(STC_T) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }
        
        h_sess = player_get_sess_handle(pt_running_svc_req->pt_player);
    
        if (x_handle_valid(h_sess) == FALSE) {
            i4_ret = SVCTXR_WRONG_STATE;
            break;
        }
    
        i4_ret = x_sm_get(
                        h_sess,
                        SM_SESS_GNRC_GET_TYPE_STC_SIGNED,
                        pv_get_info,
                        pz_get_info_size
                        );
        i4_ret = svctx_translate_smr_2_svctxr(i4_ret);        
        break;     
#else
    case SVCTX_CH_GET_CURRENT_TS_ID:
        if ((*pz_get_info_size) != sizeof(UINT16) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        *((UINT16*)pv_get_info) = pt_running_svc_req->t_src_info.ui2_ts_id;
        break;
#endif
    case SVCTX_CH_GET_AUD_ORIG_SCRAMBLE_STATUS:
        if ((*pz_get_info_size) != sizeof(BOOL) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }
        *((BOOL*) pv_get_info) = pt_running_svc_req->b_scramble_audio;
        break;
    case SVCTX_CH_GET_VID_ORIG_SCRAMBLE_STATUS:
        if ((*pz_get_info_size) != sizeof(BOOL) || pv_get_info == NULL) {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }
        *((BOOL*) pv_get_info) = pt_running_svc_req->b_scramble_video;
        break;

    default:
        i4_ret = SVCTXR_NOT_SUPPORT;
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_set
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_set(
                    SVCTX_T*                    pt_svctx,
                    SVCTX_SET_TYPE_T            e_set_type,
                    VOID*                       pv_set_info,
                    SIZE_T                      z_set_info_size
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    INT32         i4_ret = SVCTXR_OK;

    switch (e_set_type) {
    case SVCTX_CH_SET_DELIVERY_SYSTEM_CHANGE:
        ch_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_DELIVERY_SYSTEM_CHANGE, ST_UNKNOWN);
        break;
    case SVCTX_COMMON_SET_VIDEO_MUTE:
        if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) {
            if (pt_svctx->t_video_mute.b_enable == TRUE) {
                /* set video plan to blank to show bg color */
                ch_hdlr_set_video_plane(pt_svctx, SCC_VID_BLANK);
            } else {
                /* set video plan to not blank to show snow noise */
                ch_hdlr_set_video_plane(pt_svctx, pt_svctx->e_default_vid_mode);
            }
        }
        break;
    case SVCTX_COMMON_SET_ALL_SCART_OUT_DISABLE:
    case SVCTX_COMMON_SET_SCART_OUT:
        if (pt_running_svc_req != NULL)  {
            svctx_set_scart_out_params(
                    pt_svctx,
                    player_get_sess_handle(pt_running_svc_req->pt_player),
                    TRUE,
                    ST_MASK_AUDIO | ST_MASK_VIDEO,
                    &(pt_running_svc_req->t_parent)
                    );
        }
        break;

#ifdef MW_TV_AV_BYPASS_SUPPORT
    case SVCTX_INTL_COMMON_SET_TYPE_FAKE_CM_DISCTED:
        if (z_set_info_size != sizeof (BOOL) || pv_set_info == NULL) 
        {
            i4_ret = SVCTXR_INV_SET_INFO;
            break;
        }
        
        i4_ret = ch_hdlr_handle_cm_disconnected (pt_svctx,
                                                 *((BOOL *) (pv_set_info)));
                                                 
        break;
#endif

    case SVCTX_CH_SET_INVALID_SVL_REC_NW_MASK_BIT:
        if (z_set_info_size != sizeof(UINT32) || pv_set_info == NULL) {
            i4_ret = SVCTXR_INV_SET_INFO;
            break;
        }

        pt_svctx->ui4_invalid_svl_nw_mask_bit = *((UINT32*)pv_set_info);
        break;
    case SVCTX_CH_SET_AUD_AV_SYNC_MODE:
        if (z_set_info_size != sizeof(SVCTX_AV_SYNC_MODE_T) || pv_set_info == NULL) {
            i4_ret = SVCTXR_INV_SET_INFO;
            break;
        }

        pt_svctx->e_aud_av_sync_mode = *((SVCTX_AV_SYNC_MODE_T*)pv_set_info);
        break;
    case SVCTX_CH_SET_VID_I_FRAME_MODE:
        if (z_set_info_size != sizeof(BOOL) || pv_set_info == NULL) {
            i4_ret = SVCTXR_INV_SET_INFO;
            break;
        }

        if (pt_running_svc_req != NULL)
        {
            pt_running_svc_req->t_parent.b_enable_i_frame = *((BOOL*)pv_set_info);
        }
        break;
    case SVCTX_CH_SET_FREQ_LIST_UPDATED_CHK:
        i4_ret = svctx_rikstv_handle_freq_list_update_check(pt_svctx);
        break;
    case SVCTX_CH_SET_RECOVER_FROM_ALTER_TUNE:
        i4_ret = svctx_rikstv_recover_from_alter_tune(pt_svctx);
        break;
    default:
        i4_ret = SVCTXR_NOT_SUPPORT;
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_default_strm_comp_filter
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static BOOL _ch_hdlr_default_strm_comp_filter (
                    UINT16                      ui2_num_recs,
                    UINT16                      ui2_idx,
                    const STREAM_COMP_ID_T*     pt_comp_id,
                    const SCDB_REC_T*           pt_scdb_rec,
                    VOID*                       pv_tag
                    )
{
    /* modified by Allen */
    if (ST_AUDIO == pt_comp_id->e_type)
    {
        /* only select normal audio stream */
        return (AUD_TYPE_VISUAL_IMPAIRED == pt_scdb_rec->u.t_audio_mpeg.e_type) ? FALSE : TRUE;
    }
    else
    {
        return (ui2_idx == 0 ? TRUE : FALSE);
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_strm_nfy_fct
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
static INT32 _ch_hdlr_strm_nfy_fct(
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
 * Name: _ch_hdlr_get_conn_attr
 *
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_get_conn_attr(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    CM_CTRL_TYPE_T              e_ctrl,
                    VOID*                       pv_ctrl_data
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T      h_conn;
    INT32         i4_ret;

    h_conn = player_get_conn_handle(pt_running_svc_req->pt_player);
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
 * Name: _ch_hdlr_get_conn_handle
 *
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_get_conn_handle(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    HANDLE_T*                   ph_conn
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T      h_conn;

    h_conn = player_get_conn_handle(pt_running_svc_req->pt_player);
    if (x_handle_valid(h_conn) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    *ph_conn = h_conn;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _ch_hdlr_get_cm_op
 *
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_get_cm_op(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    CM_CTRL_TYPE_T              e_ctrl,
                    VOID*                       pv_ctrl_data
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T      h_conn;
    INT32         i4_ret;

    h_conn = player_get_conn_handle(pt_running_svc_req->pt_player);
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
 * Name: _ch_hdlr_set_cm_op
 *
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_set_cm_op(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    CM_CTRL_TYPE_T              e_ctrl,
                    VOID*                       pv_ctrl_data
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T      h_conn;
    INT32         i4_ret;

    h_conn = player_get_conn_handle(pt_running_svc_req->pt_player);
    if (x_handle_valid(h_conn) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    i4_ret = x_cm_set(
                    h_conn,
                    e_ctrl,
                    pv_ctrl_data
                    );

    return svctx_translate_cmr_2_svctxr(i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_open_scdb
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_open_scdb (
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    HANDLE_T*                   ph_scdb,
                    VOID*                       pv_tag,
                    x_scdb_nfy_fct              pf_nfy
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T      h_conn             = NULL_HANDLE;
    INT32         i4_ret             = SVCTXR_OK;
    CM_COND_T     e_cm_cond;

    h_conn = player_get_conn_handle(pt_running_svc_req->pt_player);
    if (x_handle_valid(h_conn) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    e_cm_cond = player_get_conn_cond(pt_running_svc_req->pt_player);
    if (e_cm_cond != CM_COND_CONNECTED) {
        return SVCTXR_WRONG_STATE;
    }

    i4_ret = x_scdb_open(h_conn, ph_scdb, pv_tag, pf_nfy);
    if (i4_ret == SCDBR_OK)
    {
        i4_ret = x_scdb_reg_cmp_fcts(*ph_scdb, t_g_svctx_class.t_scdb_sort_param.pv_tag, 
            t_g_svctx_class.t_scdb_sort_param.pf_scdb_sort_fct);
    }

    return svctx_translate_scdbr_2_svctxr(i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_open_stream
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_open_stream (
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
    CH_SVC_REQ_T*  pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T       h_sess             = NULL_HANDLE;
    INT32          i4_ret             = SVCTXR_OK;
    SM_SESS_COND_T e_sess_cond;

    h_sess = player_get_sess_handle(pt_running_svc_req->pt_player);
    if (x_handle_valid(h_sess) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    e_sess_cond = player_get_sess_cond(pt_running_svc_req->pt_player);
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

ASH_AUD_DEC_DDC_ID_T ch_hdlr_substrm_idx_cm2sm(UINT8 ui1_substrm_idx_from_cm)
{
    switch (ui1_substrm_idx_from_cm)
    {
        case AUDIO_INDEX_SUBSTREAM_1: return  ASH_AUD_DEC_DDC_ID_1;
        case AUDIO_INDEX_SUBSTREAM_2: return  ASH_AUD_DEC_DDC_ID_2;
        case AUDIO_INDEX_SUBSTREAM_3: return  ASH_AUD_DEC_DDC_ID_3;
        default:                      return  ASH_AUD_DEC_DDC_ID_UNKNOWN;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_reload_stream
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static VOID _ch_hdlr_reload_stream (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req,
                    STREAM_TYPE_T               e_stream_type
                    )
{
    STRM_REQ_T*                  pt_strm_req      = NULL;
    STRM_REQ_T*                  pt_crnt_strm_req = NULL;
    x_svctx_strm_comp_filter_fct pt_strm_fltr_fct = NULL;
    VOID*                        pv_strm_fltr_tag = NULL;
    SVCTX_MSG_T                  t_msg            = {0};
    STREAM_COMP_ID_T             t_strm_comp_id;
    SCDB_REC_T                   t_scdb_rec;
    INT32                        i4_ret;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if ((e_stream_type == ST_UNKNOWN) || (e_stream_type >= STRM_TYPE_COUNT_WITH_AD)) {
        return;
    }

    pt_crnt_strm_req = &(pt_running_svc_req->t_strm_reqs[e_stream_type]);

    if (pt_crnt_strm_req->b_aux_audio)
    {
        e_stream_type = ST_AUDIO;
    }

    if ((pt_running_svc_req->t_parent.ui4_stream_mode & MAKE_BIT_MASK_32(e_stream_type)) == 0) {
        return;
    }

    if (pt_crnt_strm_req->e_req_type == STRM_REQ_SCDB) {
        if (!pt_crnt_strm_req->b_aux_audio) {
            ch_hdlr_choose_strm_filter(
                    pt_svctx,
                    e_stream_type,
                    &pt_strm_fltr_fct,
                    &pv_strm_fltr_tag
                    );
            /* get new stream component id */
            i4_ret = _ch_hdlr_get_strm_comp_id(
                    pt_svctx,
                    e_stream_type,
                    FALSE,
                    pt_strm_fltr_fct,
                    pv_strm_fltr_tag,
                    &t_strm_comp_id,
                    &t_scdb_rec
                    );
            if (i4_ret != SVCTXR_OK) {
                return;
            }
        } else {
            /* get new ad stream component id */
            STRM_REQ_T* pt_aud_req = &(pt_running_svc_req->t_strm_reqs[ST_AUDIO]);
            i4_ret = _ch_hdlr_get_strm_comp_id(
                        pt_svctx,
                        e_stream_type,
                        FALSE,
                        ch_hdlr_strm_comp_ad_filter,
                        (VOID*)&(pt_aud_req->u.t_scdb.t_rec),
                        &t_strm_comp_id,
                        &t_scdb_rec
                        );
            if (i4_ret != SVCTXR_OK) {
                return;
            }
            /* [MS10] for multiple-program audio, substream should be mixed with
             * main audio in one stream */
            if (t_scdb_rec.u.t_audio_mpeg.e_aud_fmt == AUD_FMT_SUBSTREAM) {
                DBG_INFO((_INFO_HEADER"Default to select a substream AD. index=%d\n", t_scdb_rec.u.t_audio_mpeg.ui1_index));
                if (pt_running_svc_req->t_strm_reqs[ST_AUDIO].b_mix_substrm 
                    && x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream))
                {
                    ASH_AUD_DEC_DDC_ID_T e_ddc_id = ch_hdlr_substrm_idx_cm2sm(t_scdb_rec.u.t_audio_mpeg.ui1_index);
                    i4_ret = _ch_hdlr_set_strm_attr(
                                pt_svctx,
                                ST_AUDIO,
                                SM_ASH_SET_TYPE_MS10_MIX_ID,
                                &e_ddc_id,
                                sizeof(ASH_AUD_DEC_DDC_ID_T));
                    DBG_INFO((_INFO_HEADER"Set substream to be mixed with main audio. idx=%d, errno=%d\n", e_ddc_id, i4_ret));
                    if (SVCTXR_OK == i4_ret)
                    {
                        pt_running_svc_req->t_strm_reqs[ST_AUDIO].ui1_mixed_substrm_idx = t_scdb_rec.u.t_audio_mpeg.ui1_index;
                    }
                }
                return;
            }
        }

        /* if no stream is playing, don't need to do the comparison */
        if (pt_crnt_strm_req->e_strm_cond == SM_COND_STARTED &&
            pt_crnt_strm_req->u.t_scdb.t_comp_id.e_type == e_stream_type) {
            if (e_stream_type == ST_AUDIO) {
                if (pt_crnt_strm_req->u.t_scdb.t_rec.e_rec_type == SCDB_REC_TYPE_AUDIO_MPEG) {
                    if (t_scdb_rec.u.t_audio_mpeg.ui2_pid == pt_crnt_strm_req->u.t_scdb.t_rec.u.t_audio_mpeg.ui2_pid) {
                        return;
                    }
                }
            } else if (e_stream_type == ST_VIDEO) {
                if (pt_crnt_strm_req->u.t_scdb.t_rec.e_rec_type == SCDB_REC_TYPE_VIDEO_MPEG) {
                    if (t_scdb_rec.u.t_video_mpeg.ui2_pid == pt_crnt_strm_req->u.t_scdb.t_rec.u.t_video_mpeg.ui2_pid) {
                        return;
                    }
                }
            }
        }

        pt_strm_req = svctx_allocate_strm_req(pt_running_svc_req->t_parent.pt_hdlr);
        if (pt_strm_req == NULL) {
            return;
        }

        pt_strm_req->e_req_type         = STRM_REQ_SCDB;
        pt_strm_req->u.t_scdb.t_comp_id = t_strm_comp_id;
        pt_strm_req->u.t_scdb.t_rec     = t_scdb_rec;
    } else if (pt_crnt_strm_req->e_req_type == STRM_REQ_PID) {
        if (pt_strm_req == NULL) {
            return;
        }

        pt_strm_req->e_req_type = STRM_REQ_PID;
        pt_strm_req->u.t_pid    = pt_crnt_strm_req->u.t_pid;
    }

    if (pt_strm_req == NULL) {
        return;
    }

    pt_strm_req->h_svctx         = pt_running_svc_req->t_parent.h_svctx;
    pt_strm_req->pf_strm_nfy     = pt_running_svc_req->t_parent.pf_svc_nfy;
    pt_strm_req->pv_strm_nfy_tag = pt_running_svc_req->t_parent.pv_svc_nfy_tag;
    if (pt_crnt_strm_req->b_aux_audio) {
        pt_strm_req->b_aux_audio = pt_crnt_strm_req->b_aux_audio;
    }

    t_msg.h_svctx       = pt_running_svc_req->t_parent.h_svctx;
    t_msg.ui1_msg_id    = _SVCTX_MSG_SELECT_STRM_REQ;
    t_msg.u.pt_strm_req = pt_strm_req;

    i4_ret = ch_hdlr_select_strm(pt_svctx, &t_msg);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_get_current_freq
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_get_current_freq(
                    CH_SVC_REQ_T*               pt_running_svc_req,
                    UINT32*                     pui4_freq
                    )
{
    INT32     i4_ret;
    TSL_REC_T t_tsl_rec;

    i4_ret = ch_hdlr_get_crnt_tsl_rec(
                    pt_running_svc_req,
                    &t_tsl_rec
                    );
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

    switch(t_tsl_rec.uheader.t_desc.e_bcst_medium) {
    case BRDCST_MEDIUM_DIG_TERRESTRIAL:
        *pui4_freq = t_tsl_rec.udata.t_ter_dig.ui4_freq;
        break;
    case BRDCST_MEDIUM_DIG_CABLE:
        *pui4_freq = t_tsl_rec.udata.t_cab_dig.ui4_freq;
        break;
    case BRDCST_MEDIUM_DIG_SATELLITE:
        *pui4_freq = t_tsl_rec.udata.t_sat_dig.ui4_freq;
        break;
    case BRDCST_MEDIUM_ANA_TERRESTRIAL:
        *pui4_freq = t_tsl_rec.udata.t_ter_ana.ui4_freq;
        break;
    case BRDCST_MEDIUM_ANA_CABLE:
        *pui4_freq = t_tsl_rec.udata.t_cab_ana.ui4_freq;
        break;
    case BRDCST_MEDIUM_ANA_SATELLITE:
        *pui4_freq = t_tsl_rec.udata.t_sat_ana.ui4_freq;
        break;
    default:
        i4_ret = SVCTXR_FAIL;
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_bias_freq_as_blocked
 * Description
 *      This API will bias freq to a pre-defined value to let SCART 1 (the hard-wired
 *      with tuner) to show nosie snow.
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _ch_hdlr_bias_freq_as_blocked(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    ANALOG_BRDCST_CTRL_TUNE_ATTRIBUTES_T t_tuner_attr;
    INT32                                i4_ret = SVCTXR_OK;

    /* for analog channel, bias freq to disable SCART out */
    if (!(pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ANALOG
            && pt_svctx->b_enable_block_bias_freq == TRUE)) {
        return SVCTXR_OK;
    }

    if (pt_svctx->b_disable_no_signal_as_block_bias == TRUE) {
        return SVCTXR_OK;
    }

    pt_svctx->b_disable_no_signal_as_block_bias = TRUE;

    if (_ch_hdlr_get_current_freq(
                    pt_running_svc_req,
                    &t_tuner_attr.ui4_freq) == SVCTXR_OK
            && t_tuner_attr.ui4_freq == pt_svctx->ui4_block_bias_freq
            && pt_running_svc_req->e_ch_src_type == CH_SRC_TYPE_TUNER) {
        /* since the same freq setting will cause no any
         * tuner relative notification to update pt_svctx->e_signal_status, so
         * change the e_signal_status to SIGNAL_STATUS_LOSS
         */
        pt_svctx->e_signal_status = SIGNAL_STATUS_LOSS;
    } else {
        /* force to "signal locked" in this case to cheat clients even signal status
         * is "unknown" or "signal loss".
         */
        pt_svctx->e_signal_status = SIGNAL_STATUS_LOCKED;
    }

    t_tuner_attr.ui4_flags            = ANALOG_BRDCST_TUNE_ATTRIBUTES_FREQUENCY;
    t_tuner_attr.ui4_freq             = pt_svctx->ui4_block_bias_freq;

    i4_ret = x_cm_set(
                    player_get_conn_handle(pt_running_svc_req->pt_player),
                    ANALOG_BRDCST_CTRL_SET_TUNE_ATTRIBUTES,
                    (VOID*)&t_tuner_attr
                    );
    return svctx_translate_cmr_2_svctxr(i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_restore_freq_as_unblocked
 * Description
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _ch_hdlr_restore_freq_as_unblocked(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    ANALOG_BRDCST_CTRL_TUNE_ATTRIBUTES_T t_tuner_attr;
    INT32                                i4_ret = SVCTXR_OK;

    /* for analog channel, restore freq of current channel */
    if (!(pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ANALOG
            && pt_svctx->b_enable_block_bias_freq == TRUE)) {
        return SVCTXR_OK;
    }

    if (pt_svctx->b_disable_no_signal_as_block_bias == FALSE) {
        return SVCTXR_OK;
    }

    pt_svctx->b_disable_no_signal_as_block_bias = FALSE;
    pt_svctx->b_in_restoring_orig_freq          = TRUE;
    if (_ch_hdlr_get_current_freq(
                    pt_running_svc_req,
                    &t_tuner_attr.ui4_freq) == SVCTXR_OK) {
        if (t_tuner_attr.ui4_freq == pt_svctx->ui4_block_bias_freq
                && pt_running_svc_req->e_ch_src_type == CH_SRC_TYPE_TUNER) {
            /* since the same freq setting will cause no any
             * tuner relative notification to update pt_svctx->e_signal_status, so
             * change the e_signal_status to change the e_signal_status to SIGNAL_STATUS_LOSS
             * b_in_restoring_orig_freq = FALSE
             */
            pt_svctx->b_in_restoring_orig_freq = FALSE;
            pt_svctx->e_signal_status = SIGNAL_STATUS_LOSS;
        } else {
            pt_svctx->e_signal_status = SIGNAL_STATUS_UNKNOWN;
        }

        t_tuner_attr.ui4_flags            = ANALOG_BRDCST_TUNE_ATTRIBUTES_FREQUENCY;
        i4_ret = x_cm_set(
                    player_get_conn_handle(pt_running_svc_req->pt_player),
                    ANALOG_BRDCST_CTRL_SET_TUNE_ATTRIBUTES,
                    (VOID*)&t_tuner_attr
                    );
        i4_ret = svctx_translate_cmr_2_svctxr(i4_ret);
    }

    if (pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ANALOG
            && pt_running_svc_req->e_ch_src_type == CH_SRC_TYPE_SCART
            &&(pt_running_svc_req->t_src_info.u_info.t_svl.t_ch_encode.e_type == CHANNEL_DECODE_TYPE_MANUAL
            || pt_running_svc_req->t_src_info.u_info.t_svl.t_ch_encode.e_type == CHANNEL_DECODE_TYPE_AUTO)) {
        ANALOG_BRDCST_CTRL_SCART_BYPASS_T t_scart_bypass;

        t_scart_bypass.b_bypass = FALSE;
        ch_hdlr_set_conn_attr(
                    pt_svctx,
                    ST_VIDEO,
                    ANALOG_BRDCST_CTRL_SET_SCART_BYPASS,
                    &t_scart_bypass
                    );
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_init
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 ch_hdlr_init(
                    VOID
                    )
{
    SVCTX_HDLR_FCT_TABLE_T t_fct_table = {0};
    INT32                  i4_ret;

    if (b_g_init == TRUE) {
        return SVCTXR_OK;
    }

    /* service relative function table*/
    t_fct_table.pf_allocate_svc_req           = _ch_hdlr_allocate_svc_req;
    t_fct_table.pf_free_svc_req               = _ch_hdlr_free_svc_req;
    t_fct_table.pf_create_svc_req             = _ch_hdlr_create_svc_req;
    t_fct_table.pf_create_surf_svc_req        = _ch_hdlr_create_surf_svc_req;
    t_fct_table.pf_print_svc_req              = _ch_hdlr_print_svc_req;

    /* stream relative function table*/
    t_fct_table.pf_allocate_strm_req          = _ch_hdlr_allocate_strm_req;
    t_fct_table.pf_free_strm_req              = _ch_hdlr_free_strm_req;
    t_fct_table.pf_create_strm_req            = _ch_hdlr_create_strm_req;
    t_fct_table.pf_print_strm_req             = _ch_hdlr_print_strm_req;

    t_fct_table.pf_get_scdb_num_recs          = ch_hdlr_get_scdb_num_recs;
    t_fct_table.pf_get_cp_info                = _ch_hdlr_get_cp_info;
    t_fct_table.pf_get_ca_info                = _ch_hdlr_get_ca_info;
    t_fct_table.pf_get_using_scdb_info        = _ch_hdlr_get_using_scdb_info;
    t_fct_table.pf_get_strm_req_type          = _ch_hdlr_get_strm_req_type;
    t_fct_table.pf_get_strm_comp_id           = _ch_hdlr_get_strm_comp_id;

    t_fct_table.pf_replace_svc_req            = _ch_hdlr_replace_svc_req;
    t_fct_table.pf_select_svc_req             = ch_hdlr_select_svc;
    t_fct_table.pf_stop_svc_req               = ch_hdlr_stop_svc;
    t_fct_table.pf_unblock_svc_req            = ch_hdlr_check_svc_block;
    t_fct_table.pf_select_strm_req            = ch_hdlr_select_strm;
    t_fct_table.pf_stop_strm_req              = ch_hdlr_stop_strm;
    t_fct_table.pf_internal_msg               = ch_hdlr_internal_msg;
    t_fct_table.pf_is_blocked_source          = ch_hdlr_is_blocked_source;

    t_fct_table.pf_set_session_attr           = _ch_hdlr_set_sess_attr;
    t_fct_table.pf_get_session_attr           = _ch_hdlr_get_sess_attr;
    t_fct_table.pf_set_stream_attr            = _ch_hdlr_set_strm_attr;
    t_fct_table.pf_get_stream_attr            = _ch_hdlr_get_strm_attr;
    t_fct_table.pf_get_signal_level           = _ch_hdlr_get_signal_level;
    t_fct_table.pf_get_dbm_signal_level       = _ch_hdlr_get_dbm_signal_level;
    t_fct_table.pf_get_tuner_status           = _ch_hdlr_get_tuner_status;
    t_fct_table.pf_get_cell_id                = _ch_hdlr_get_cell_id;
    t_fct_table.pf_get_vbi_broker             = _ch_hdlr_get_vbi_broker;
    t_fct_table.pf_get_stream_handle          = _ch_hdlr_get_strm_handle;
    t_fct_table.pf_get_current_event_info_len = _ch_hdlr_get_current_event_info_len;
    t_fct_table.pf_get_current_event_info     = _ch_hdlr_get_current_event_info;

    t_fct_table.pf_freeze                     = ch_hdlr_freeze;
    t_fct_table.pf_unfreeze                   = ch_hdlr_unfreeze;

    t_fct_table.pf_open_scdb                  = _ch_hdlr_open_scdb;
    t_fct_table.pf_open_stream                = _ch_hdlr_open_stream;
    t_fct_table.pf_get_conn_attr              = _ch_hdlr_get_conn_attr;
    t_fct_table.pf_set_conn_attr              = ch_hdlr_set_conn_attr;
    t_fct_table.pf_get_conn_op                = _ch_hdlr_get_cm_op;
    t_fct_table.pf_set_conn_op                = _ch_hdlr_set_cm_op;
    t_fct_table.pf_get_conn_handle            = _ch_hdlr_get_conn_handle;

    t_fct_table.pf_get                        = _ch_hdlr_get;
    t_fct_table.pf_set                        = _ch_hdlr_set;
    t_fct_table.pf_is_handler                 = _ch_hdlr_is_handler;
    t_fct_table.pf_deinit                     = _ch_hdlr_deinit;

    i4_ret = svctx_register_handler(
                    CH_DEFAULT_HDLR_NAME,
                    &t_fct_table
                    );
    if (i4_ret != SVCTXR_OK) {
        DBG_ERROR((_ERROR_HEADER"svctx_register_handler() failed, ret = %d)\r\n", i4_ret));
        return i4_ret;
    }

    i4_ret = ch_svc_req_init(
                    (UINT16) (svctx_get_obj_amount() * SVC_REQ_AMOUNT_FACTOR)
                    );
    if (i4_ret != SVCTXR_OK) {
        DBG_ERROR((_ERROR_HEADER"ch_svc_req_init() failed, ret = %d)\r\n", i4_ret));
        return i4_ret;
    }

    i4_ret = ch_strm_req_init(
                    (UINT16) (svctx_get_obj_amount() * SVC_REQ_AMOUNT_FACTOR * STRM_TYPE_COUNT_WITH_AD)
                    );
    if (i4_ret != SVCTXR_OK) {
        DBG_ERROR((_ERROR_HEADER"ch_strm_req_init() failed, ret = %d)\r\n", i4_ret));
        return i4_ret;
    }

    i4_ret = crnt_evn_init(
                    svctx_get_obj_amount()
                    );
    if (i4_ret != SVCTXR_OK) {
        DBG_ERROR((_ERROR_HEADER"crnt_evn_init() failed, ret = %d)\r\n", i4_ret));
        return i4_ret;
    }

    i4_ret = rating_chker_init(
                    svctx_get_obj_amount()
                    );
    if (i4_ret != SVCTXR_OK) {
        DBG_ERROR((_ERROR_HEADER"rating_chker_init() failed, ret = %d)\r\n", i4_ret));
        return i4_ret;
    }

    i4_ret = strm_fmt_chker_init(
                    svctx_get_obj_amount()
                    );
    if (i4_ret != SVCTXR_OK) {
        DBG_ERROR((_ERROR_HEADER"strm_fmt_chker_init() failed, ret = %d)\r\n", i4_ret));
        return i4_ret;
    }

#ifdef CLI_LVL_ALL
    ch_hdlr_cli_attach_cmd_tbl();
#endif

    b_g_init = TRUE;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_invoke_client_svc_nfy_fct
 * Description
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
VOID ch_hdlr_invoke_client_svc_nfy_fct(
                    SVCTX_T*                    pt_svctx,
                    SVCTX_NTFY_CODE_T           e_code,
                    STREAM_TYPE_T               e_stream_type
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    switch (e_code) {
    case SVCTX_NTFY_CODE_SIGNAL_LOCKED:
    case SVCTX_NTFY_CODE_SIGNAL_LOCKED_AS_BLOCKED:
        pt_svctx->b_signal_loss_notified = FALSE;
        pt_svctx->e_signal_status        = SIGNAL_STATUS_LOCKED;
        svctx_ctrl_snow(pt_svctx, FALSE);
        if (player_get_conn_cond(pt_running_svc_req->pt_player) != CM_COND_CONNECTED) {
            return;
        }
        break;
    case SVCTX_NTFY_CODE_SIGNAL_DETECTING:
        pt_svctx->b_signal_loss_notified = FALSE;
        pt_svctx->e_signal_status        = SIGNAL_STATUS_DETECTING;
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

        if (svctx_is_scrambled_strm(&pt_running_svc_req->t_strm_reqs[ST_VIDEO].t_ca) == FALSE
                &&(pt_svctx->e_video_decode_status == VSH_DIG_DECODE_NORMAL
                || pt_svctx->e_video_decode_status == VSH_DIG_DECODE_UNKNOWN)) {
#ifndef LINUX_TURNKEY_SOLUTION_PHASE2 /* avoid transient black screen when no video */
            svctx_mute_video(pt_svctx, FALSE);
#endif
            ch_hdlr_set_video_plane(pt_svctx, pt_svctx->e_default_vid_mode);
        }

        svctx_set_scart_out_params(
                    pt_svctx,
                    player_get_sess_handle(pt_running_svc_req->pt_player),
                    TRUE,
                    ST_MASK_AUDIO | ST_MASK_VIDEO,
                    &(pt_running_svc_req->t_parent)
                    );

        TMS_END("SELECT_SVC");
        break;
    case SVCTX_NTFY_CODE_TUNER_SIGNAL_LOCKED:
        if (pt_svctx->b_in_restoring_orig_freq == TRUE) {
            pt_svctx->b_in_restoring_orig_freq = FALSE;
        }
        break;
    case SVCTX_NTFY_CODE_SIGNAL_LOSS_AS_BLOCKED:
        if (pt_svctx->b_signal_loss_notified)
        {
            return;
        }
        break;
    case SVCTX_NTFY_CODE_TUNER_SIGNAL_LOSS:
        if (!(pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ANALOG
                && pt_svctx->b_in_restoring_orig_freq == TRUE
                && pt_svctx->b_enable_block_bias_freq == TRUE)) {
            /* as unblock by setting actual freq, the TVD will
             * not report SIGNAL LOSS, use tuner's notification
             * to compensate this notification
             */
            break;
        }

        pt_svctx->b_in_restoring_orig_freq = FALSE;

        if (pt_svctx->b_in_fine_tuning == TRUE
                || pt_running_svc_req->e_ch_src_type == CH_SRC_TYPE_SCART) {
            /* don't need to handle this as fine tuning or source is from SCART */
            break;
        }
        /* on puropse to without break */
    case SVCTX_NTFY_CODE_SIGNAL_LOSS:
        if (pt_svctx->b_enable_block_bias_freq == TRUE
                && pt_svctx->b_disable_no_signal_as_block_bias == TRUE) {
            /* if "block bias freq" is enable, since this will cause
             * "no signal", so ignore the signal loss notification.
             */
            return;
        }

        if (pt_svctx->b_in_restoring_orig_freq == TRUE) {
            /* ignore signal loss as restoring orig freq, to avoid
             * false alarm as restoring the original freq
             */
            return;
        }

        pt_svctx->e_signal_status = SIGNAL_STATUS_LOSS;
        if (pt_svctx->e_state != SVCTX_COND_PRESENTING_SELECT_SAME_REQ
                && pt_svctx->e_state != SVCTX_COND_PENDING_SELECT_SAME_REQ) {
            pt_svctx->e_state = SVCTX_COND_PENDING;
        }

        if (pt_svctx->b_in_fine_tuning == FALSE
                && pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ANALOG) {
            if (pt_svctx->b_enable_block_bias_freq == TRUE) {
                /* if b_enable_block_bias_freq is set, it seems
                 * tuner/TVD needs more time to be stable
                 */
                svctx_set_signal_loss_chk_delay(
                        pt_svctx,
                        BIAS_FREQ_ANALOG_SIGNAL_LOSS_CHK_TIMEOUT
                        );
            }

            if (svctx_delay_signal_loss_nfy(
                        pt_svctx,
                        ch_hdlr_invoke_client_svc_nfy_fct) == TRUE) {
                return;
            }
        }

        if (ch_hdlr_check_signal_loss_priority(pt_svctx, pt_running_svc_req, FALSE) == FALSE) {
            return;
        }

        svctx_ctrl_block(pt_svctx, FALSE);
        pt_svctx->b_block_notified = FALSE;

        if (pt_svctx->b_signal_loss_notified == TRUE) {
            return;
        }

        if (pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ANALOG
                && pt_svctx->b_analog_signal_loss_show_snow == TRUE) {
            svctx_ctrl_snow(pt_svctx, TRUE);
        } else {
            svctx_ctrl_snow(pt_svctx, FALSE);
        }

        svctx_mute_video(pt_svctx, pt_svctx->t_video_mute.b_enable);

        if (pt_svctx->t_video_mute.b_enable == TRUE) {
            ch_hdlr_set_video_plane(pt_svctx, SCC_VID_BLANK);
        } else {
            ch_hdlr_set_video_plane(pt_svctx, pt_svctx->e_default_vid_mode);
        }
        TMS_END("SELECT_SVC");

        if ((pt_svctx->ui1_stop_audio_as_no_signal & MAKE_BIT_MASK_8(pt_running_svc_req->t_parent.e_src_type)) > 0) {
            ch_hdlr_reset_audio_info(pt_svctx, pt_running_svc_req);
            ch_hdlr_reset_video_info(pt_svctx, pt_running_svc_req);
            strm_fmt_chker_reset(pt_running_svc_req->pt_strm_fmt_chker); 
        }
        svctx_cancel_no_video_related_delay_nfy(pt_svctx);
        pt_svctx->e_audio_decode_status = ASH_AUDIO_DECODE_UNKNOWN;
        pt_svctx->e_video_decode_status = VSH_DIG_DECODE_UNKNOWN;

        /* need to check to close related stream if it is from
         * SVCTX_NTFY_CODE_TUNER_SIGNAL_LOSS by bias freq as block
         */
        ch_hdlr_close_streams_as_no_signal(pt_svctx, pt_running_svc_req);

        pt_svctx->b_signal_loss_notified = TRUE;

        if (pt_svctx->b_freezed)
        {
            SCC_VID_SUPER_FREEZE_T t_super_freeze = {SCC_VID_SUPER_FREEZE_TYPE_FORCE, FALSE};
            x_scc_vid_set_super_freeze(pt_svctx->h_video_scc_comp, &t_super_freeze);
            x_scc_vid_set_force_unmute(pt_svctx->h_video_scc_comp, SCC_VID_FORCE_UNMUTE_TYPE_ATV_CHG_CH, FALSE);
            pt_svctx->b_freezed = FALSE;
            x_dbg_stmt("[%s][%s][%d] unfreezed !!!!!!!!\r\n", pt_svctx->s_name, __FILE__, __LINE__);
            svctx_set_video_plane(pt_svctx, SCC_VID_BLANK);
        }
        
        break;
    case SVCTX_NTFY_CODE_SERVICE_UNBLOCKED:
        svctx_ctrl_block(pt_svctx, FALSE);
        pt_svctx->b_block_notified = FALSE;

        _ch_hdlr_restore_freq_as_unblocked(pt_svctx, pt_running_svc_req);
        break;
    case SVCTX_NTFY_CODE_SERVICE_BLOCKED:
        svctx_ctrl_block(pt_svctx, TRUE);
        svctx_mute_video(pt_svctx, pt_svctx->t_video_mute.b_enable);
        ch_hdlr_set_video_plane(pt_svctx, SCC_VID_BLANK);

        svctx_cancel_no_video_related_delay_nfy(pt_svctx);

        _ch_hdlr_bias_freq_as_blocked(pt_svctx, pt_running_svc_req);
        svctx_force_disable_scart_out(
                    pt_svctx,
                    player_get_sess_handle(pt_running_svc_req->pt_player),
                    TRUE,
                    ST_MASK_AUDIO | ST_MASK_VIDEO,
                    &(pt_running_svc_req->t_parent)
                    );
        if (pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED
                && ch_hdlr_check_signal_loss_priority(pt_svctx, pt_running_svc_req, FALSE) == TRUE) {

            if (pt_svctx->b_freezed)
            {
                SCC_VID_SUPER_FREEZE_T t_super_freeze = {SCC_VID_SUPER_FREEZE_TYPE_FORCE, FALSE};
                x_scc_vid_set_super_freeze(pt_svctx->h_video_scc_comp, &t_super_freeze);
                x_scc_vid_set_force_unmute(pt_svctx->h_video_scc_comp, SCC_VID_FORCE_UNMUTE_TYPE_ATV_CHG_CH, FALSE);
                pt_svctx->b_freezed = FALSE;
                x_dbg_stmt("[%s][%s][%d] unfreezed !!!!!!!!\r\n", pt_svctx->s_name, __FILE__, __LINE__);
            }
            return;
        }

        if (pt_svctx->b_block_notified == TRUE) {
            return;
        }

        pt_svctx->b_block_notified = TRUE;

        if (pt_svctx->b_freezed)
        {
            SCC_VID_SUPER_FREEZE_T t_super_freeze = {SCC_VID_SUPER_FREEZE_TYPE_FORCE, FALSE};
            x_scc_vid_set_super_freeze(pt_svctx->h_video_scc_comp, &t_super_freeze);
            x_scc_vid_set_force_unmute(pt_svctx->h_video_scc_comp, SCC_VID_FORCE_UNMUTE_TYPE_ATV_CHG_CH, FALSE);
            pt_svctx->b_freezed = FALSE;
            x_dbg_stmt("[%s][%s][%d] unfreezed !!!!!!!!\r\n", pt_svctx->s_name, __FILE__, __LINE__);
        }
        
        break;
    case SVCTX_NTFY_CODE_CRNT_EVN_START_QUERY:
    case SVCTX_NTFY_CODE_CRNT_EVN_NOT_READY:
    case SVCTX_NTFY_CODE_CRNT_EVN_READY:
        /* still to send out the notification as be blocked */

        /* not to send out as signal loss */
        if (
#ifdef MW_TV_AV_BYPASS_SUPPORT
#if 0
            pt_svctx->b_fake_disc ||
#endif
#endif
            pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) {
            return;
        }
        break;
    case SVCTX_NTFY_CODE_CH_SRC_FROM_TUNER:
    case SVCTX_NTFY_CODE_CH_SRC_FROM_SCART:
    case SVCTX_NTFY_CODE_INTERRUPTED:
    case SVCTX_NTFY_CODE_IN_STOPPING:
        /* always sent out */
        break;
    case SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_VIDEO_SVC:
    case SVCTX_NTFY_CODE_SCRAMBLED_VIDEO_CLEAR_AUDIO_SVC:
    case SVCTX_NTFY_CODE_SCRAMBLED_VIDEO_NO_AUDIO_SVC:
        if (pt_svctx->b_is_blocked) {
            return;
        }
        if (svctx_is_scrambled_strm(&pt_running_svc_req->t_strm_reqs[ST_VIDEO].t_ca) == TRUE) {
            ch_hdlr_set_video_plane(pt_svctx, SCC_VID_BLANK);
            svctx_reset_video_info(pt_svctx);
        }
        break;
    /* Alec-012209: Added for resolving DTV00131925 */
    case SVCTX_NTFY_CODE_VIDEO_ONLY_SVC:
    case SVCTX_NTFY_CODE_AUDIO_VIDEO_SVC:
        if ((pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED) ||
            (pt_svctx->b_is_blocked == TRUE)) {
            return;
        }

        if (svctx_is_scrambled_strm(&pt_running_svc_req->t_strm_reqs[ST_VIDEO].t_ca) == FALSE
                &&(pt_svctx->e_video_decode_status == VSH_DIG_DECODE_NORMAL
                || pt_svctx->e_video_decode_status == VSH_DIG_DECODE_UNKNOWN)) {
            svctx_cli_print_channel_change_log(pt_svctx, CH_XNG_STAGE_SVCTX_SET_UNMUTE);
            svctx_mute_video(pt_svctx, FALSE);
            ch_hdlr_set_video_plane(pt_svctx, pt_svctx->e_default_vid_mode);
        }

        svctx_set_scart_out_params(
                    pt_svctx,
                    player_get_sess_handle(pt_running_svc_req->pt_player),
                    TRUE,
                    ST_MASK_AUDIO | ST_MASK_VIDEO,
                    &(pt_running_svc_req->t_parent)
                    );
        break;
#ifdef CM_CONN_INFO_PAT_TS_ID_CHANGED
    case SVCTX_NTFY_CODE_RF_SCAN_REQUIRED:
        /* This message must be passed to AP no matter the pt_svctx->e_signal_status is "lost" or "locked" */
        break;
#endif
    case SVCTX_NTFY_CODE_START_ALTER_FREQ_TUNE:
    case SVCTX_NTFY_CODE_END_ALTER_FREQ_TUNE:
    case SVCTX_NTFY_CODE_START_FIRST_TUNE:
    case SVCTX_NTFY_CODE_END_FIRST_TUNE:
    case SVCTX_NTFY_CODE_START_FREQ_LIST_UPDATED_CHK:
    case SVCTX_NTFY_CODE_END_FREQ_LIST_UPDATED_CHK:
        break;
#ifdef MW_ISDB_SUPPORT
    /* TMCC event notification */
    case SVCTX_NTFY_CODE_EWS_ON:
    case SVCTX_NTFY_CODE_EWS_OFF:
        DBG_INFO((_INFO_HEADER"%s notified\r\n", ((e_code==SVCTX_NTFY_CODE_EWS_ON)?"SVCTX_NTFY_CODE_EWS_ON":"SVCTX_NTFY_CODE_EWS_OFF")));
        break;
    /* Broadcasting / non-broadcasting */
    case SVCTX_NTFY_CODE_BRDCSTING_SVC:
    case SVCTX_NTFY_CODE_NON_BRDCSTING_SVC:
        break;
#endif
    default:
        if (pt_svctx->b_is_blocked == TRUE) {
            return;
        }

        /* not to send out as signal loss (for svctx_delay_no_video_related_nfy() ) */
        if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) {
            if (SVCTX_NTFY_CODE_CONNECTION_ANA_FREQ == e_code)
            {
                break;
            }
            return;
        }
        break;
    }
#ifdef MW_ISDB_SUPPORT
    if (pt_svctx->b_is_brdcsting_svc == FALSE &&
        pt_svctx->b_is_blocked && pt_svctx->b_block_notified)
    {
        /* suppress svc ntfy for non-broadcasting service */
        switch (e_code)
        {
            case SVCTX_NTFY_CODE_SERVICE_BLOCKED:
            case SVCTX_NTFY_CODE_NON_BRDCSTING_SVC:
                break;

            case SVCTX_NTFY_CODE_SIGNAL_LOCKED:
            case SVCTX_NTFY_CODE_CRNT_EVN_START_QUERY:
                /* avoid changing screen saver message */
                return;

            default:
                break;
        }
    }
#endif

    svctx_notify_clients_svc_msg(
                    pt_svctx,
                    e_code,
                    e_stream_type,
                    pt_svctx->pt_running_svc_req->h_svctx,
                    pt_svctx->pt_running_svc_req->pf_svc_nfy
                    );

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if ((pt_svctx->b_is_blocked) &&
        (!pt_svctx->b_block_notified) &&
        (SVCTX_NTFY_CODE_SIGNAL_LOCKED == e_code))
    {
        svctx_notify_clients_svc_msg(
                        pt_svctx,
                        SVCTX_NTFY_CODE_SERVICE_BLOCKED,
                        e_stream_type,
                        pt_svctx->pt_running_svc_req->h_svctx,
                        pt_svctx->pt_running_svc_req->pf_svc_nfy
                        );
        pt_svctx->b_block_notified = TRUE;
    }
#endif
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_choose_strm_filter
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
VOID ch_hdlr_choose_strm_filter(
                    SVCTX_T*                      pt_svctx,
                    STREAM_TYPE_T                 e_strm_type,
                    x_svctx_strm_comp_filter_fct* ppt_strm_fltr_fct,
                    VOID**                        ppv_strm_fltr_tag
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if ((e_strm_type == ST_UNKNOWN) || (e_strm_type >= STRM_TYPE_COUNT))
    {
        return;
    }

    /* DTV00001085 chose the proper stream filter function
     * 1. specified in x_svc_select_svc
     * 2. specified as default stream filter function
     * 3. system default (first one)
     */
    if (pt_running_svc_req->t_strm_reqs[e_strm_type].pt_strm_fltr_fct != NULL) {
        *ppt_strm_fltr_fct = pt_running_svc_req->t_strm_reqs[e_strm_type].pt_strm_fltr_fct;
        *ppv_strm_fltr_tag = pt_running_svc_req->t_strm_reqs[e_strm_type].pv_strm_fltr_tag;
    } else {
        if (pt_svctx->pt_def_strm_fltr_fcts[e_strm_type] != NULL) {
            *ppt_strm_fltr_fct = pt_svctx->pt_def_strm_fltr_fcts[e_strm_type];
            *ppv_strm_fltr_tag = pt_svctx->pv_def_strm_fltr_tags[e_strm_type];
        } else {
            *ppt_strm_fltr_fct = _ch_hdlr_default_strm_comp_filter;
            *ppv_strm_fltr_tag = (VOID*)pt_svctx;
        }
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_check_strms_status
 * Description
 * Input arguments
 *      pt_svctx    a pointer points to the Service Context
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
VOID ch_hdlr_check_strms_status(
                    SVCTX_T*                    pt_svctx
                    )
{
    UINT32            ui4_strm_present_cond    = 0;
    UINT32            ui4_strm_scrambled_cond  = 0;
    UINT32            ui4_old_strm_format_cond = 0;
    CH_SVC_REQ_T*     pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    SVCTX_NTFY_CODE_T e_code;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS
            || pt_svctx->b_is_blocked == TRUE
            || pt_svctx->t_video_info.e_timing_type == VSH_SRC_TIMING_NOT_SUPPORT) {
        return;
    }

    ui4_old_strm_format_cond  = strm_fmt_chker_get_format_status(pt_running_svc_req->pt_strm_fmt_chker);

    if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream) == TRUE) {
        /* check scrambled or not */
        if (svctx_is_scrambled_strm(&pt_running_svc_req->t_strm_reqs[ST_VIDEO].t_ca)) {
            ui4_strm_scrambled_cond |= ST_MASK_VIDEO;
        } else {
            /* video is opened */
            if ((ui4_old_strm_format_cond & ST_MASK_VIDEO) == 0) {
                /* video format not available */
                return;
            }

            /* video format is available */
            if (pt_running_svc_req->e_brdcst_type != BRDCST_TYPE_ANALOG
                    || (pt_running_svc_req->t_parent.ui4_stream_mode & ST_MASK_VIDEO) != 0) {
                if (svctx_is_valid_video_fmt(pt_svctx) == TRUE) {
                    ui4_strm_present_cond |= ST_MASK_VIDEO;
                }
            }
        }
    }

    if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) == TRUE) {
        /* check scrambled or not */
        if (svctx_is_scrambled_strm(&pt_running_svc_req->t_strm_reqs[ST_AUDIO].t_ca)) {
            ui4_strm_scrambled_cond |= ST_MASK_AUDIO;
        } else {
            /* audio is opened */
            if ((ui4_old_strm_format_cond & ST_MASK_AUDIO) == 0) {
                /* audio format not available */
                return;
            }

            /* audio format is available */
            if (svctx_is_valid_audio_fmt(pt_svctx) == TRUE) {
                ui4_strm_present_cond |= ST_MASK_AUDIO;
            }
        }
    }

    strm_fmt_chker_force_present_status(pt_running_svc_req->pt_strm_fmt_chker, ui4_strm_present_cond);

    if ((ui4_strm_present_cond & ST_MASK_VIDEO) != 0) {
        if ((ui4_strm_present_cond & ST_MASK_AUDIO) != 0) { /* audio + video */
            /* clear audio + clear video  */
            e_code = SVCTX_NTFY_CODE_AUDIO_VIDEO_SVC;
        } else { /* video */
            if ((ui4_strm_scrambled_cond & ST_MASK_AUDIO) != 0) {
                /* scrambled audio + clear video */
                e_code = SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_CLEAR_VIDEO_SVC;
            } else {
                if (pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ANALOG) {
                    if (pt_running_svc_req->t_parent.ui4_stream_mode & ST_MASK_AUDIO) {
                        /* always audio + video for analog channel */
                        e_code = SVCTX_NTFY_CODE_AUDIO_VIDEO_SVC;
                    } else {
                        e_code = SVCTX_NTFY_CODE_VIDEO_ONLY_SVC;
                    }
                } else {
                    /* no audio + clear video*/
                    e_code = SVCTX_NTFY_CODE_VIDEO_ONLY_SVC;
                }
            }
        }
    } else {
        if ((ui4_strm_present_cond & ST_MASK_AUDIO) != 0) { /* audio */
            if ((ui4_strm_scrambled_cond & ST_MASK_VIDEO) != 0) {
                /* clear audio + scrambled video */
                e_code = SVCTX_NTFY_CODE_SCRAMBLED_VIDEO_CLEAR_AUDIO_SVC;
            } else {
                if (pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ANALOG) {
                    if (pt_running_svc_req->t_parent.ui4_stream_mode & ST_MASK_VIDEO) {
                        /* always audio + video for analog channel */
                        e_code = SVCTX_NTFY_CODE_AUDIO_VIDEO_SVC;
                    } else {
                        e_code = SVCTX_NTFY_CODE_AUDIO_ONLY_SVC;
                    }
                } else {
                    /* clear audio + no video */
                    e_code = SVCTX_NTFY_CODE_AUDIO_ONLY_SVC;
                }
            }
        } else {
            if ((ui4_strm_scrambled_cond & ST_MASK_VIDEO) != 0) {
                if ((ui4_strm_scrambled_cond & ST_MASK_AUDIO) != 0) {
                    /* scrambled audio + scrambled video */
                    e_code = SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_VIDEO_SVC;
                } else {
                    /* no audio + scrambled video */
                    e_code = SVCTX_NTFY_CODE_SCRAMBLED_VIDEO_NO_AUDIO_SVC;
                }
            } else {
                if ((ui4_strm_scrambled_cond & ST_MASK_AUDIO) != 0) {
                    /* scrambled audio + no video */
                    e_code = SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_NO_VIDEO_SVC;
                } else {
                    if (pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ANALOG) {
                        /* always audio + video for analog channel */
                        if ((pt_running_svc_req->t_parent.ui4_stream_mode & (ST_MASK_VIDEO|ST_MASK_AUDIO)) == 0) {
                            e_code = SVCTX_NTFY_CODE_NO_AUDIO_VIDEO_SVC;
                        } else {
                            e_code = SVCTX_NTFY_CODE_AUDIO_VIDEO_SVC;
                        }
                    } else {
                        /* no audio + no video */
                        e_code = SVCTX_NTFY_CODE_NO_AUDIO_VIDEO_SVC;
                    }
                }
            }
        }
    }

    if (svctx_delay_no_video_related_nfy(
                    pt_svctx,
                    e_code,
                    ch_hdlr_invoke_client_svc_nfy_fct) == TRUE) {
        return;
    }

    ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    e_code,
                    ST_UNKNOWN
                    );
}

#ifdef MW_TV_AV_BYPASS_SUPPORT
static INT32 _ch_hdlr_get_ttx_info(
    HANDLE_T                h_scdb,
    STREAM_TYPE_T           e_strm_type,
    SCDB_REC_TYPE_T         e_rec_type,
    SCDB_REC_T*             pt_scdb_rec)
{
    INT32           i4_ret;
    UINT32          ui4_ver_id;
    UINT16          ui2_pid_arr_sz = 16;
    UINT16          ui2_rec_arr_sz = 8;
    UINT16*         pui2_pid = NULL;
    SCDB_REC_T*     pt_rec   = NULL;
    INT32           i, j, i4_rec;
    INT32           i4_inner = TRUE;
    INT32           i4_outer = TRUE;

    while (i4_outer)
    {
        while (1)
        {
            /* allocate memory for PID array */
            if (!pui2_pid)
            {
                pui2_pid = (UINT16*) x_mem_alloc(sizeof(UINT16) * ui2_pid_arr_sz);

                if (!pui2_pid)
                {
                    i4_outer = i4_inner = FALSE;
                    i4_ret = SVCTXR_NO_RESOURCE;
                    break;
                }
            }

            /* get PID array */
            i4_ret = x_util_scdb_get_stream_pid_info(
                h_scdb,
                e_strm_type,
                pui2_pid,
                &ui2_pid_arr_sz,
                &ui4_ver_id);

            if (SCDBR_OK == i4_ret)
            {
                if (0 == ui2_pid_arr_sz)
                {
                    i4_outer = i4_inner = FALSE;
                    i4_ret = SVCTXR_NO_RECORD;
                }
                else
                {
                    i4_inner = TRUE;
                    i4_ret = SVCTXR_OK;
                }

                break;
            }
            else if (SCDBR_BUF_TOO_SMALL == i4_ret)
            {
                x_mem_free(pui2_pid);
                pui2_pid = NULL;
                continue;
            }
            else if (SCDBR_DB_MODIFIED == i4_ret)
            {
                continue;
            }
            else
            {
                i4_outer = i4_inner = FALSE;
                i4_ret = SVCTXR_FAIL;
                break;
            }
        }

        i = 0;

        while (i4_inner)
        {
            /* allocate memory for SCDB record array */
            if (!pt_rec)
            {
                pt_rec = (SCDB_REC_T*) x_mem_alloc(sizeof(SCDB_REC_T) * ui2_rec_arr_sz);

                if (!pt_rec)
                {
                    i4_outer = i4_inner = FALSE;
                    i4_ret = SVCTXR_NO_RESOURCE;
                    break;
                }
            }

            while (1)
            {
                /* get SCDB record array */
                i4_ret = x_util_scdb_get_stream_info(
                    h_scdb,
                    e_strm_type,
                    pui2_pid[i],
                    pt_rec,
                    &ui2_rec_arr_sz,
                    &ui4_ver_id);

                if (SVCTXR_OK == i4_ret)
                {
                    if (0 == ui2_rec_arr_sz)
                    {
                        i4_outer = i4_inner = FALSE;
                        i4_ret = SVCTXR_NO_RECORD;
                        break;
                    }

                    i4_rec = -1;

                    for (j = 0; j < (INT32) ui2_rec_arr_sz; j++)
                    {
                        if (e_rec_type == pt_rec[j].e_rec_type)
                        {
                            i4_rec = j;
                            *pt_scdb_rec = pt_rec[j];
                            break;
                        }
                    }

                    if (i4_rec >= 0)
                    {
                        i4_outer = i4_inner = FALSE;
                        i4_ret = SVCTXR_OK;
                        break;
                    }
                    else
                    {
                        if (++i >= ui2_pid_arr_sz)
                        {
                            i4_outer = i4_inner = FALSE;
                            i4_ret = SVCTXR_NO_RECORD;
                            break;
                        }
                    }
                }
                else if (SCDBR_BUF_TOO_SMALL == i4_ret)
                {
                    x_mem_free(pt_rec);
                    pt_rec = NULL;
                    break;
                }
                else if (SCDBR_DB_MODIFIED == i4_ret)
                {
                    i4_inner = FALSE;
                    break;
                }
                else
                {
                    i4_outer = i4_inner = FALSE;
                    i4_ret = SVCTXR_FAIL;
                    break;
                }
            }
        }
    }

    if (pt_rec)
    {
        x_mem_free(pt_rec);
    }

    if (pui2_pid)
    {
        x_mem_free(pui2_pid);
    }

    return (i4_ret);
}
#endif

/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_open_strm
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

static UINT8 _ch_hdlr_get_downmix_from_svl(SVL_REC_T* pt_svl_rec)
{
    UINT8 ui1_dm = 0;
    
    if (pt_svl_rec == NULL)
    {
        return 0;
    }

    switch(pt_svl_rec->uheader.t_rec_hdr.e_brdcst_type)
    {
        case BRDCST_TYPE_ANALOG:
            ui1_dm = pt_svl_rec->u_data.t_analog.ui1_aud_mts;
            break;
        case BRDCST_TYPE_DVB:
            ui1_dm = pt_svl_rec->u_data.t_dvb.ui1_downmix_mode;
            break;
        case BRDCST_TYPE_SCTE:
        case BRDCST_TYPE_ATSC:
        case BRDCST_TYPE_UNKNOWN:
            ui1_dm = pt_svl_rec->u_data.t_atsc.ui1_downmix_mode;
            break;
        case BRDCST_TYPE_ISDB:
            ui1_dm = pt_svl_rec->u_data.t_isdb.ui1_downmix_mode;
            break;
        case BRDCST_TYPE_DTMB:
            ui1_dm = pt_svl_rec->u_data.t_dtmb.ui1_downmix_mode;
            break;
        case BRDCST_TYPE_FMRDO:
        default:
            break;
    }

    return ui1_dm;
}


#ifdef MW_TV_AV_BYPASS_SUPPORT
INT32 ch_hdlr_open_strm_ex(
                    SVCTX_T*                    pt_svctx,
                    STRM_REQ_T*                 pt_strm_req,
                    BOOL                        b_auto_play)
{
    SM_COMMAND_T        t_sm_cmd[8];
    CH_SVC_REQ_T*       pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    INT32               i4_ret             = SVCTXR_OK;
    STREAM_TYPE_T       e_strm_type        = svctx_get_strm_req_strm_type(pt_strm_req);
    UINT8               ui1_idx            = 0;
    STREAM_COMP_ID_T    t_strm_comp_id;
    BSH_VBIF_SRC_INFO_T t_src_info;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (pt_strm_req->e_req_type == STRM_REQ_PID) {
        t_strm_comp_id.e_type        = e_strm_type;
        t_strm_comp_id.pv_stream_tag = NULL;
    } else { /* STRM_REQ_SCDB */
        t_strm_comp_id = pt_strm_req->u.t_scdb.t_comp_id;
    }

    switch(e_strm_type) {
    case ST_VIDEO:
        if (x_handle_valid(pt_strm_req->h_stream) == TRUE) {
            return i4_ret;
        }

        if (IS_SVCTX_ASSOCIATE(pt_svctx))
        {
            SCDB_REC_T  t_scdb_rec;

            t_strm_comp_id.e_type = ST_BYPASS;

            if (pt_strm_req->e_req_type == STRM_REQ_PID)
            {
                t_sm_cmd[ui1_idx].e_code       = SM_BYPASS_CMD_CODE_SET_FORCE_PID;
                t_sm_cmd[ui1_idx++].u.ui2_data = (UINT16)pt_strm_req->u.t_pid.t_mpeg_2_pid;
                t_sm_cmd[ui1_idx].e_code       = SM_BYPASS_CMD_CODE_SET_FORCE_CODEC;
                t_sm_cmd[ui1_idx++].u.ui1_data = (UINT8)pt_strm_req->u.t_pid.u.t_vid_stream.e_vid_enc;
            }

            t_sm_cmd[ui1_idx].e_code = SM_BYPASS_CMD_CODE_SET_VBIF_SRC;

            if (SVCTXR_OK == _ch_hdlr_get_ttx_info(
                player_get_scdb_handle(pt_running_svc_req->pt_player),
                ST_DATA,
                SCDB_REC_TYPE_VBI_DATA_DVB,
                &t_scdb_rec))
            {
                t_src_info.e_src_type = BSH_VBIF_SRC_TYPE_DIG_VBI_ES;
                t_src_info.ui2_pid    = t_scdb_rec.u.t_dvb_vbi_data.ui2_pid;

                t_sm_cmd[ui1_idx++].u.pv_data = &t_src_info;
            }
            else if (SVCTXR_OK == _ch_hdlr_get_ttx_info(
                player_get_scdb_handle(pt_running_svc_req->pt_player),
                ST_TELETEXT,
                SCDB_REC_TYPE_VBI_TELETEXT_DVB,
                &t_scdb_rec))
            {
                t_src_info.e_src_type = BSH_VBIF_SRC_TYPE_DIG_VBI_ES;
                t_src_info.ui2_pid    = t_scdb_rec.u.t_dvb_vbi_tltxt.ui2_pid;

                t_sm_cmd[ui1_idx++].u.pv_data = &t_src_info;
            }
            else if (SVCTXR_OK == _ch_hdlr_get_ttx_info(
                player_get_scdb_handle(pt_running_svc_req->pt_player),
                ST_TELETEXT,
                SCDB_REC_TYPE_TELETEXT_DVB,
                &t_scdb_rec))
            {
                t_src_info.e_src_type = BSH_VBIF_SRC_TYPE_DIG_TTX_ES;
                t_src_info.ui2_pid    = t_scdb_rec.u.t_dvb_tltxt.ui2_pid;

                t_sm_cmd[ui1_idx++].u.pv_data = &t_src_info;
            }
        }
        else
        {
            if (pt_strm_req->e_req_type == STRM_REQ_PID) {
                t_sm_cmd[ui1_idx].e_code       = SM_VS_CMD_CODE_SET_FORCE_PID;
                t_sm_cmd[ui1_idx++].u.ui2_data = (UINT16)pt_strm_req->u.t_pid.t_mpeg_2_pid;
                t_sm_cmd[ui1_idx].e_code       = SM_VS_CMD_CODE_SET_FORCE_CODEC;
                t_sm_cmd[ui1_idx++].u.ui1_data = (UINT8)pt_strm_req->u.t_pid.u.t_vid_stream.e_vid_enc;
            }

            t_sm_cmd[ui1_idx].e_code      = SM_VS_CMD_CODE_SET_VIDEO_REGION;
            t_sm_cmd[ui1_idx++].u.pv_data = &(pt_svctx->t_video_region);
            t_sm_cmd[ui1_idx].e_code      = SM_VS_CMD_CODE_SET_DISP_REGION;
            t_sm_cmd[ui1_idx++].u.pv_data = &(pt_svctx->t_disp_region);
        }

        t_sm_cmd[ui1_idx].e_code = SM_CMD_CODE_END;
        break;
    case ST_AUDIO:
        /* modified by Allen */
        if (pt_strm_req->b_aux_audio)
        {
            t_sm_cmd[ui1_idx].e_code      = SM_AS_CMD_CODE_SET_AUDIO_DECODER;
            t_sm_cmd[ui1_idx++].u.pv_data = NULL;
#ifdef MW_MS10_SUPPORT
            if (!(pt_strm_req->e_req_type == STRM_REQ_SCDB && 
                pt_strm_req->u.t_scdb.t_rec.u.t_audio_mpeg.e_aud_fmt == AUD_FMT_SUBSTREAM))
#endif
            {
                t_sm_cmd[ui1_idx].e_code      = SM_AS_CMD_CODE_SET_LOGICAL_DESC;
                t_sm_cmd[ui1_idx++].u.ps_text = (CHAR*) pt_svctx->s_snk_grp_name_aux;
            }
            if (pt_strm_req->e_req_type == STRM_REQ_PID) {
                t_sm_cmd[ui1_idx].e_code       = SM_AS_CMD_CODE_SET_FORCE_PID;
                t_sm_cmd[ui1_idx++].u.ui2_data = (UINT16)pt_strm_req->u.t_pid.t_mpeg_2_pid;

                t_sm_cmd[ui1_idx].e_code       = SM_AS_CMD_CODE_SET_FORCE_CODEC;
                t_sm_cmd[ui1_idx++].u.ui1_data = (UINT8)pt_strm_req->u.t_pid.u.t_aud_stream.e_aud_enc;

                t_sm_cmd[ui1_idx].e_code       = SM_AS_CMD_CODE_SET_FORCE_SYNC_MODE;
                t_sm_cmd[ui1_idx++].u.ui1_data = (UINT8)ASH_AV_SYNC_FREE_RUN;
            }
        }
        else
        {
            if (pt_strm_req->e_req_type == STRM_REQ_PID) {
                t_sm_cmd[ui1_idx].e_code       = SM_AS_CMD_CODE_SET_FORCE_PID;
                t_sm_cmd[ui1_idx++].u.ui2_data = (UINT16)pt_strm_req->u.t_pid.t_mpeg_2_pid;
#if 1 /* Alec: force audio codec set */
                t_sm_cmd[ui1_idx].e_code       = SM_AS_CMD_CODE_SET_FORCE_CODEC;
                t_sm_cmd[ui1_idx++].u.ui1_data = (UINT8)pt_strm_req->u.t_pid.u.t_aud_stream.e_aud_enc;
#endif
                t_sm_cmd[ui1_idx].e_code       = SM_AS_CMD_CODE_SET_FORCE_SYNC_MODE;
                t_sm_cmd[ui1_idx++].u.ui1_data = (UINT8)ASH_AV_SYNC_FREE_RUN;
            }
            else
            {
                switch (pt_running_svc_req->t_parent.e_src_type)
                {
                    case SRC_TYPE_BRDCST_SVL:
                        if (pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_FMRDO)
                        {
                            t_sm_cmd[ui1_idx].e_code       = SM_AS_CMD_CODE_SET_FORCE_SYNC_MODE;
                            t_sm_cmd[ui1_idx++].u.ui1_data = (UINT8)ASH_AV_SYNC_FREE_RUN;
                        }
                        else
                        {
                            SVL_REC_T t_svl_rec = {0};

                            i4_ret = ch_hdlr_get_crnt_svl_rec(
                                    pt_running_svc_req,
                                    &t_svl_rec
                                    );
                            if (i4_ret != SVCTXR_OK) {
                                break;
                            }

                            t_sm_cmd[ui1_idx].e_code = SM_AS_CMD_CODE_SET_FORCE_DOWNMIX_MODE;
                            t_sm_cmd[ui1_idx++].u.ui1_data = _ch_hdlr_get_downmix_from_svl(&t_svl_rec);

                            t_sm_cmd[ui1_idx].e_code = SM_AS_CMD_CODE_SET_FORCE_SYNC_MODE;

                            if ((t_svl_rec.uheader.t_rec_hdr.e_serv_type == SVL_SERVICE_TYPE_RADIO)
#ifdef MW_PIP_POP_SUPPORT                                 
                                || (svctx_is_pap(pt_svctx) && (pt_svctx->e_av_status == SVC_AV_STATUS_NO_AUDIO_VIDEO)) /* DTV00141024 fix */ 
#endif
                               )
                            {
                                t_sm_cmd[ui1_idx++].u.ui1_data = (UINT8)ASH_AV_SYNC_FREE_RUN;
                            }
                            else
                            {
                                t_sm_cmd[ui1_idx++].u.ui1_data = (UINT8)pt_svctx->e_aud_av_sync_mode;
                            }

                            /* Tell driver if we may play a AD or not after main played. */
                            if (pt_svctx->b_ad_enabled)
                            {
                                SCDB_REC_T         t_ad_scdb_rec;
                                STREAM_COMP_ID_T   t_comp_id;
                                i4_ret = _ch_hdlr_get_strm_comp_id(
                                        pt_svctx,
                                        ST_AUDIO,
                                        FALSE,
                                        ch_hdlr_strm_comp_ad_filter,
                                        (VOID*)&(pt_strm_req->u.t_scdb.t_rec),
                                        &t_comp_id,
                                        &t_ad_scdb_rec
                                        );                                
                                t_sm_cmd[ui1_idx].e_code        = SM_AS_CMD_CODE_SET_AD_MODE;
                                t_sm_cmd[ui1_idx++].u.b_boolean = (SVCTXR_OK == i4_ret);
                                DBG_INFO((_INFO_HEADER"Set the AD flag (%d) to driver.\n", (SVCTXR_OK == i4_ret)));
                            }
                        }

                        /* [MS10]: multi-program audio stream support.
                         * if substream may be selected to mix, set the main 
                         * audio's mix flag of both MAIN & BYPASS as per
                         * driver's demand */
                        {
                            SCDB_REC_T         t_ad_scdb_rec = {0};
                            STREAM_COMP_ID_T   t_comp_id;     

                            i4_ret = _ch_hdlr_get_strm_comp_id(
                                    pt_svctx,
                                    e_strm_type,
                                    FALSE,
                                    ch_hdlr_strm_comp_ad_filter,
                                    (VOID*)&(pt_running_svc_req->t_strm_reqs[ST_AUDIO].u.t_scdb.t_rec),
                                    &t_comp_id,
                                    &t_ad_scdb_rec
                                    );
                            if (SVCTXR_OK == i4_ret 
                                && AUD_FMT_SUBSTREAM == t_ad_scdb_rec.u.t_audio_mpeg.e_aud_fmt)
                            {
                                pt_strm_req->b_mix_substrm = TRUE;
                                pt_strm_req->ui1_mixed_substrm_idx = AUDIO_INDEX_UNKNOW;
                                DBG_INFO((_INFO_HEADER"[MS10] AD candidate is a substream, so set the mix flag to TRUE with idx=UNKNOWN. \n"));
                            }
                            else
                            {
                                pt_strm_req->b_mix_substrm = FALSE;
                                pt_strm_req->ui1_mixed_substrm_idx = AUDIO_INDEX_UNKNOW;
                            }

                            t_sm_cmd[ui1_idx].e_code = SM_AS_CMD_CODE_SET_MS10_MIX_FLAG;
                            t_sm_cmd[ui1_idx++].u.b_boolean = pt_strm_req->b_mix_substrm;
                            t_sm_cmd[ui1_idx].e_code = SM_AS_CMD_CODE_SET_MS10_MIX_ID;
                            t_sm_cmd[ui1_idx++].u.ui1_data = (UINT8)ch_hdlr_substrm_idx_cm2sm(pt_strm_req->ui1_mixed_substrm_idx);
                        } /* [MS10] END */
                        break;

                    case SRC_TYPE_BRDCST_FREQ:
                        t_sm_cmd[ui1_idx].e_code       = SM_AS_CMD_CODE_SET_FORCE_SYNC_MODE;
                        
                        if (pt_running_svc_req->t_src_info.u_info.t_freq.e_type == FREQ_INFO_TYPE_FM_RADIO)
                        {
                            t_sm_cmd[ui1_idx++].u.ui1_data = (UINT8)ASH_AV_SYNC_FREE_RUN;
                        }
                        else
                        {
                            t_sm_cmd[ui1_idx++].u.ui1_data = (UINT8)ASH_AV_SYNC_AUD_SLAVE;
                        }
                        break;
                }
            }
        }
        t_sm_cmd[ui1_idx].e_code = SM_CMD_CODE_END;
        break;
    case ST_CLOSED_CAPTION:
        if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream) == FALSE) {
            DBG_ERROR((_ERROR_HEADER"%s() failed, no any video stream\r\n", i4_ret));
            return SVCTXR_FAIL;
        }

        if (x_handle_valid(pt_running_svc_req->h_gl_plane) == FALSE) {
            DBG_ERROR((_ERROR_HEADER"%s() failed, no graphic plane for cc widget\r\n", i4_ret));
            return SVCTXR_FAIL;
        }

        /* set commonds for open closed caption */
        t_sm_cmd[ui1_idx].e_code      = SM_CC_CMD_CODE_SET_VS_HANDLE;
        t_sm_cmd[ui1_idx++].u.h_obj   = pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream;
        t_sm_cmd[ui1_idx].e_code      = SM_CC_CMD_CODE_USE_OSD_DISP;
        t_sm_cmd[ui1_idx++].u.h_obj   = pt_running_svc_req->h_gl_plane;
        t_sm_cmd[ui1_idx].e_code      = SM_CC_CMD_CODE_SET_CC_FILTER;
        t_sm_cmd[ui1_idx++].u.pv_data = (VOID*)(&pt_running_svc_req->t_cc_filter);
        t_sm_cmd[ui1_idx].e_code      = SM_CMD_CODE_END;
        break;
#ifdef MW_ISDB_SUPPORT
    case ST_ISDB_CAPTION:
    case ST_ISDB_TEXT:
        t_sm_cmd[ui1_idx].e_code      = SM_CCI_CMD_CODE_SET_LANG_ID;
        t_sm_cmd[ui1_idx++].u.ui4_data= pt_running_svc_req->ui4_isdbcc_idx;
        t_sm_cmd[ui1_idx].e_code      = SM_CCI_CMD_CODE_SET_OSD_HANDLE;
        t_sm_cmd[ui1_idx++].u.h_obj   = pt_running_svc_req->h_gl_plane;
        t_sm_cmd[ui1_idx].e_code      = SM_CMD_CODE_END;
        break;
#endif        
    case ST_SUBTITLE:
        if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream) == FALSE) {
            DBG_ERROR((_ERROR_HEADER"%s() failed, no any video stream\r\n", i4_ret));
            return SVCTXR_FAIL;
        }

        if (x_handle_valid(pt_running_svc_req->h_gl_plane) == FALSE) {
            DBG_ERROR((_ERROR_HEADER"%s() failed, no graphic plane for subtitle widget\r\n", i4_ret));
            return SVCTXR_FAIL;
        }

        if (pt_strm_req->e_req_type == STRM_REQ_PID) {
            /* QQQQ no related command for subtitle
            t_sm_cmd[ui1_idx].e_code       = SM_AS_CMD_CODE_SET_FORCE_PID;
            t_sm_cmd[ui1_idx++].u.ui2_data = (UINT16)pt_strm_req->u.t_pid.t_mpeg_2_pid;
            */
        }

        /* set commonds for open subtitle */
        t_sm_cmd[ui1_idx].e_code    = SM_SBTL_CMD_CODE_SET_VS_HANDLE;
        t_sm_cmd[ui1_idx++].u.h_obj = pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream;
        t_sm_cmd[ui1_idx].e_code    = SM_SBTL_CMD_CODE_SET_OSD_HANDLE;
        t_sm_cmd[ui1_idx++].u.h_obj = pt_running_svc_req->h_gl_plane;
        t_sm_cmd[ui1_idx].e_code    = SM_SBTL_CMD_CODE_SET_SCART_CANVAS_HANDLE;
        t_sm_cmd[ui1_idx++].u.h_obj = pt_running_svc_req->t_sbtl_info.h_scart_plane;
        t_sm_cmd[ui1_idx].e_code    = SM_SBTL_CMD_CODE_SET_OPEN_MAIN_INVISIBLE_FLAG;
        t_sm_cmd[ui1_idx++].u.b_boolean   = pt_running_svc_req->t_sbtl_info.b_open_main_invisible;        
        t_sm_cmd[ui1_idx].e_code    = SM_CMD_CODE_END;
        break;
    default:
        return SVCTXR_NOT_SUPPORT;
    }

    if (!x_handle_valid(pt_strm_req->h_stream))
    {
        CLI_FUNC_CALL_TRACE_FOR_STRM("x_sm_open_stream", e_strm_type);

        i4_ret = x_sm_open_stream(
                    player_get_sess_handle(pt_running_svc_req->pt_player),
                    &t_strm_comp_id,
                    t_sm_cmd,
                    _ch_hdlr_strm_nfy_fct,
                    (VOID*)(pt_svctx->h_sys_svctx),
                    b_auto_play,
                    &(pt_strm_req->h_stream)
                    );
        
        svctx_cli_print_stream_component_idx(pt_svctx, 0, e_strm_type, pt_strm_req->h_stream, 0, 0, __FILE__, __LINE__);
        
        if (i4_ret != SMR_OK && i4_ret != SMR_PENDING) {
            DBG_ERROR((_ERROR_HEADER"x_sm_open_stream() failed, i4_ret=%d\r\n", i4_ret));
            ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_INTERNAL_ERROR,
                    ST_UNKNOWN
                    );
            return SVCTXR_FAIL;
        }

        pt_strm_req->e_strm_cond = SM_COND_OPENING;
    }

    return SVCTXR_OK;
}

INT32 ch_hdlr_open_strm(
                    SVCTX_T*                    pt_svctx,
                    STRM_REQ_T*                 pt_strm_req,
                    BOOL                        b_auto_play
                    )
{
    return (ch_hdlr_try_opening_stream(pt_svctx, pt_strm_req, b_auto_play));
}
#else
INT32 ch_hdlr_open_strm(
                    SVCTX_T*                    pt_svctx,
                    STRM_REQ_T*                 pt_strm_req,
                    BOOL                        b_auto_play
                    )
{
    SM_COMMAND_T      t_sm_cmd[6];
    CH_SVC_REQ_T*     pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    INT32             i4_ret             = SVCTXR_OK;
    STREAM_TYPE_T     e_strm_type        = svctx_get_strm_req_strm_type(pt_strm_req);
    UINT8             ui1_idx            = 0;
    STREAM_COMP_ID_T  t_strm_comp_id;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (pt_strm_req->e_req_type == STRM_REQ_PID) {
        t_strm_comp_id.e_type        = e_strm_type;
        t_strm_comp_id.pv_stream_tag = NULL;
    } else { /* STRM_REQ_SCDB */
        t_strm_comp_id = pt_strm_req->u.t_scdb.t_comp_id;
    }

    switch(e_strm_type) {
    case ST_VIDEO:
        if (pt_strm_req->e_req_type == STRM_REQ_PID) {
            t_sm_cmd[ui1_idx].e_code       = SM_VS_CMD_CODE_SET_FORCE_PID;
            t_sm_cmd[ui1_idx++].u.ui2_data = (UINT16)pt_strm_req->u.t_pid.t_mpeg_2_pid;
            t_sm_cmd[ui1_idx].e_code       = SM_VS_CMD_CODE_SET_FORCE_CODEC;
            t_sm_cmd[ui1_idx++].u.ui1_data = (UINT8)pt_strm_req->u.t_pid.u.t_vid_stream.e_vid_enc;
        }
        t_sm_cmd[ui1_idx].e_code      = SM_VS_CMD_CODE_SET_VIDEO_REGION;
        t_sm_cmd[ui1_idx++].u.pv_data = &(pt_svctx->t_video_region);
        t_sm_cmd[ui1_idx].e_code      = SM_VS_CMD_CODE_SET_DISP_REGION;
        t_sm_cmd[ui1_idx++].u.pv_data = &(pt_svctx->t_disp_region);
        t_sm_cmd[ui1_idx].e_code      = SM_CMD_CODE_END;
        break;
    case ST_AUDIO:
        /* modified by Allen */
        if (pt_strm_req->b_aux_audio)
        {
            t_sm_cmd[ui1_idx].e_code      = SM_AS_CMD_CODE_SET_AUDIO_DECODER;
            t_sm_cmd[ui1_idx++].u.pv_data = NULL;
#ifdef MW_MS10_SUPPORT
            if (!(pt_strm_req->e_req_type == STRM_REQ_SCDB && 
                pt_strm_req->u.t_scdb.t_rec.u.t_audio_mpeg.e_aud_fmt == AUD_FMT_SUBSTREAM))
#endif
            {
                t_sm_cmd[ui1_idx].e_code      = SM_AS_CMD_CODE_SET_LOGICAL_DESC;
                t_sm_cmd[ui1_idx++].u.ps_text = (CHAR*) pt_svctx->s_snk_grp_name_aux;
            }
        }
        else
        {
            if (pt_strm_req->e_req_type == STRM_REQ_PID) {
                t_sm_cmd[ui1_idx].e_code       = SM_AS_CMD_CODE_SET_FORCE_PID;
                t_sm_cmd[ui1_idx++].u.ui2_data = (UINT16)pt_strm_req->u.t_pid.t_mpeg_2_pid;
#if 1 /* Alec: force audio codec set */
                t_sm_cmd[ui1_idx].e_code       = SM_AS_CMD_CODE_SET_FORCE_CODEC;
                t_sm_cmd[ui1_idx++].u.ui1_data = (UINT8)pt_strm_req->u.t_pid.u.t_aud_stream.e_aud_enc;
#endif
                t_sm_cmd[ui1_idx].e_code       = SM_AS_CMD_CODE_SET_FORCE_SYNC_MODE;
                t_sm_cmd[ui1_idx++].u.ui1_data = (UINT8)ASH_AV_SYNC_FREE_RUN;
            }
            
            else
            {
                switch (pt_running_svc_req->t_parent.e_src_type)
               {
                 case SRC_TYPE_BRDCST_SVL:
                    if (pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_FMRDO)
                    {
                        t_sm_cmd[ui1_idx].e_code       = SM_AS_CMD_CODE_SET_FORCE_SYNC_MODE;
                        t_sm_cmd[ui1_idx++].u.ui1_data = (UINT8)ASH_AV_SYNC_FREE_RUN;
                    }
                    else
                    {
                        SVL_REC_T t_svl_rec = {0};
            
                        i4_ret = ch_hdlr_get_crnt_svl_rec(
                                             pt_running_svc_req,
                                             &t_svl_rec
                                             );
                        if (i4_ret != SVCTXR_OK) {
                            break;
                        }
                        t_sm_cmd[ui1_idx].e_code = SM_AS_CMD_CODE_SET_FORCE_DOWNMIX_MODE;
                        t_sm_cmd[ui1_idx++].u.ui1_data = _ch_hdlr_get_downmix_from_svl(&t_svl_rec);
             
                   }
                   break;
                	}
             }
        }
        t_sm_cmd[ui1_idx].e_code = SM_CMD_CODE_END;
        break;
    case ST_CLOSED_CAPTION:
        if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream) == FALSE) {
            DBG_ERROR((_ERROR_HEADER"%s() failed, no any video stream\r\n", i4_ret));
            return SVCTXR_FAIL;
        }

        if (x_handle_valid(pt_running_svc_req->h_gl_plane) == FALSE) {
            DBG_ERROR((_ERROR_HEADER"%s() failed, no graphic plane for cc widget\r\n", i4_ret));
            return SVCTXR_FAIL;
        }

        /* set commonds for open closed caption */
        t_sm_cmd[ui1_idx].e_code      = SM_CC_CMD_CODE_SET_VS_HANDLE;
        t_sm_cmd[ui1_idx++].u.h_obj   = pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream;
        t_sm_cmd[ui1_idx].e_code      = SM_CC_CMD_CODE_USE_OSD_DISP;
        t_sm_cmd[ui1_idx++].u.h_obj   = pt_running_svc_req->h_gl_plane;
        t_sm_cmd[ui1_idx].e_code      = SM_CC_CMD_CODE_SET_CC_FILTER;
        t_sm_cmd[ui1_idx++].u.pv_data = (VOID*)(&pt_running_svc_req->t_cc_filter);
        t_sm_cmd[ui1_idx].e_code      = SM_CMD_CODE_END;
        break;
#ifdef MW_ISDB_SUPPORT
    case ST_ISDB_CAPTION:
    case ST_ISDB_TEXT:
        t_sm_cmd[ui1_idx].e_code      = SM_CCI_CMD_CODE_SET_LANG_ID;
        t_sm_cmd[ui1_idx++].u.ui4_data= pt_running_svc_req->ui4_isdbcc_idx;
        t_sm_cmd[ui1_idx].e_code      = SM_CCI_CMD_CODE_SET_OSD_HANDLE;
        t_sm_cmd[ui1_idx++].u.h_obj   = pt_running_svc_req->h_gl_plane;
        t_sm_cmd[ui1_idx].e_code      = SM_CMD_CODE_END;
        break;
#endif
    case ST_SUBTITLE:
        if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream) == FALSE) {
            DBG_ERROR((_ERROR_HEADER"%s() failed, no any video stream\r\n", i4_ret));
            return SVCTXR_FAIL;
        }

        if (x_handle_valid(pt_running_svc_req->h_gl_plane) == FALSE) {
            DBG_ERROR((_ERROR_HEADER"%s() failed, no graphic plane for subtitle widget\r\n", i4_ret));
            return SVCTXR_FAIL;
        }

        if (pt_strm_req->e_req_type == STRM_REQ_PID) {
            /* QQQQ no related command for subtitle
            t_sm_cmd[ui1_idx].e_code       = SM_AS_CMD_CODE_SET_FORCE_PID;
            t_sm_cmd[ui1_idx++].u.ui2_data = (UINT16)pt_strm_req->u.t_pid.t_mpeg_2_pid;
            */
        }

        /* set commonds for open subtitle */
        t_sm_cmd[ui1_idx].e_code    = SM_SBTL_CMD_CODE_SET_VS_HANDLE;
        t_sm_cmd[ui1_idx++].u.h_obj = pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream;
        t_sm_cmd[ui1_idx].e_code    = SM_SBTL_CMD_CODE_SET_OSD_HANDLE;
        t_sm_cmd[ui1_idx++].u.h_obj = pt_running_svc_req->h_gl_plane;
        t_sm_cmd[ui1_idx].e_code    = SM_SBTL_CMD_CODE_SET_SCART_CANVAS_HANDLE;
        t_sm_cmd[ui1_idx++].u.h_obj = pt_running_svc_req->t_sbtl_info.h_scart_plane;
        t_sm_cmd[ui1_idx].e_code    = SM_CMD_CODE_END;
        break;
    default:
        return SVCTXR_NOT_SUPPORT;
    }

    CLI_FUNC_CALL_TRACE_FOR_STRM("x_sm_open_stream", e_strm_type);
    i4_ret = x_sm_open_stream(
                player_get_sess_handle(pt_running_svc_req->pt_player),
                &t_strm_comp_id,
                t_sm_cmd,
                _ch_hdlr_strm_nfy_fct,
                (VOID*)(pt_svctx->h_sys_svctx),
                b_auto_play,
                &(pt_strm_req->h_stream)
                );
    if (i4_ret != SMR_OK && i4_ret != SMR_PENDING) {
        DBG_ERROR((_ERROR_HEADER"x_sm_open_stream() failed, i4_ret=%d\r\n", i4_ret));
        ch_hdlr_invoke_client_svc_nfy_fct(
                pt_svctx,
                SVCTX_NTFY_CODE_INTERNAL_ERROR,
                ST_UNKNOWN
                );
        return SVCTXR_FAIL;
    }

    pt_strm_req->e_strm_cond = SM_COND_OPENING;

    return SVCTXR_OK;
}
#endif
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_reload_audio_stream
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
VOID ch_hdlr_reload_audio_stream (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    _ch_hdlr_reload_stream (
                    pt_svctx,
                    pt_running_svc_req,
                    ST_AUDIO
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_reload_video_stream
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
VOID ch_hdlr_reload_video_stream (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    _ch_hdlr_reload_stream (
                    pt_svctx,
                    pt_running_svc_req,
                    ST_VIDEO
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_reload_ad_stream
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
VOID ch_hdlr_reload_ad_stream (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    if (!ch_hdlr_is_valid_for_ad_select(pt_svctx, pt_running_svc_req))
    {
        return;
    }     
    else
    {               
        _ch_hdlr_reload_stream (
                        pt_svctx,
                        pt_running_svc_req,
                        (STREAM_TYPE_T)STRM_REQ_AD_INDEX
                        );
    }                            
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_invoke_client_strm_nfy_fct
 * Description
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
VOID ch_hdlr_invoke_client_strm_nfy_fct(
                    SVCTX_T*                    pt_svctx,           /* in  */
                    SVCTX_NTFY_CODE_T           e_code,             /* in  */
                    STRM_REQ_T*                 pt_strm_req         /* in  */
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    BOOL          b_check_svc_type   = FALSE;
    STREAM_TYPE_T e_strm_type        = svctx_get_strm_req_strm_type(pt_strm_req);
    INT32         i4_ret;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (pt_running_svc_req == NULL || pt_strm_req == NULL)
    {
        return;
    }

    if ((pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED) ||
        (pt_svctx->b_is_blocked == TRUE)) {
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
            if (!(pt_svctx->b_analog_signal_loss_show_snow && (e_strm_type == ST_AUDIO)))
            {
                return;
            }                
        }
    }

    switch (e_code) {
    case SVCTX_NTFY_CODE_STREAM_STARTED_AS_BLOCKED:
    case SVCTX_NTFY_CODE_STREAM_STARTED:
      {
        switch(e_strm_type) {
        case ST_VIDEO:
            strm_fmt_chker_start_delay(
                    pt_running_svc_req->pt_strm_fmt_chker,
                    ST_VIDEO
                    );
            break;
        case ST_AUDIO:
            if (!pt_strm_req->b_aux_audio)
            {
                INT32 i4_ret;

                i4_ret = SVCTXR_FAIL;
                if (pt_svctx->b_analog_signal_loss_show_snow && (pt_running_svc_req->e_brdcst_type==BRDCST_TYPE_ANALOG))
                {
                    do
                    {
                        STRM_REQ_T* pt_audio_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_AUDIO]);
                        
                        if (pt_audio_strm_req == NULL
                                || x_handle_valid(pt_audio_strm_req->h_stream) == FALSE) 
                        {
                            i4_ret = SVCTXR_FAIL;
                            break;
                        }
                        
                        if (svctx_is_valid_audio_fmt(pt_svctx) == TRUE) 
                        {
                            i4_ret = SVCTXR_FAIL;
                            break;
                        }

                        if (pt_svctx->t_x_audio_info.e_channels == ASH_CHANNELS_UNKNOWN)
                        {
                            i4_ret = SVCTXR_FAIL;
                            break;
                        }
                        
                        i4_ret = strm_fmt_chker_set_format_status(
                                    pt_running_svc_req->pt_strm_fmt_chker,
                                    ST_AUDIO
                                    );

                        if (i4_ret != SVCTXR_OK)
                        {
                            break;
                        }
                        
                        /* if audio format is still not available, the timeout check will send
                         * notify to client
                         */
                        ch_hdlr_invoke_client_strm_nfy_fct(
                                        pt_svctx,
                                        SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE,
                                        pt_audio_strm_req
                                        );
                        
                        i4_ret = SVCTXR_OK;
                    }while (0);    
                }

                if (i4_ret != SVCTXR_OK)
                {
                    strm_fmt_chker_start_delay(
                            pt_running_svc_req->pt_strm_fmt_chker,
                            ST_AUDIO
                            );
                }
            }
            break;
        default:
            break;
        }
        break;
      }
    case SVCTX_NTFY_CODE_STREAM_STOPPED_AS_BLOCKED:
    case SVCTX_NTFY_CODE_STREAM_STOPPED:
        switch(e_strm_type) {
        case ST_VIDEO:
        case ST_AUDIO:
            if (pt_svctx->e_state != SVCTX_COND_PRESENTING) {
                break;
            }

            if (!pt_strm_req->b_aux_audio)
            {
                strm_fmt_chker_reset_format_status(
                        pt_running_svc_req->pt_strm_fmt_chker,
                        e_strm_type
                        );
            }
            b_check_svc_type = TRUE;
            break;
        default:
            break;
        }
        break;
    case SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE:
        if (pt_svctx->b_is_blocked == FALSE) {
            switch(pt_svctx->e_av_status) /* for DTV139538 */
            {
                case SVC_AV_STATUS_SCRAMBLED_AUDIO_VIDEO:
                case SVC_AV_STATUS_SCRAMBLED_AUDIO_NO_VIDEO:
                    svctx_set_av_status(pt_svctx, SVC_AV_STATUS_SCRAMBLED_AUDIO_CLEAR_VIDEO);
                    break;
                case SVC_AV_STATUS_SCRAMBLED_VIDEO_CLEAR_AUDIO:
                case SVC_AV_STATUS_AUDIO_ONLY:
                    svctx_set_av_status(pt_svctx, SVC_AV_STATUS_AUDIO_VIDEO);
                    break;
                case SVC_AV_STATUS_SCRAMBLED_VIDEO_NO_AUDIO:
                case SVC_AV_STATUS_NO_AUDIO_VIDEO:
                case SVC_AV_STATUS_UNKNOWN:
                    svctx_set_av_status(pt_svctx, SVC_AV_STATUS_VIDEO_ONLY);
                    break;
                default:
                    break;
            }
            if (pt_svctx->t_video_info.e_timing_type == VSH_SRC_TIMING_NOT_SUPPORT) {
                svctx_mute_video(pt_svctx, pt_svctx->t_video_mute.b_enable);
                ch_hdlr_set_video_plane(pt_svctx, SCC_VID_BLANK);

                if ((pt_svctx->ui1_stop_audio_as_no_signal & MAKE_BIT_MASK_8(pt_running_svc_req->t_parent.e_src_type)) > 0) {
                    if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) == TRUE) {
                        svctx_reset_audio_info(pt_svctx);
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
            } else if (pt_svctx->t_video_info.e_timing_type != VSH_SRC_TIMING_UNKNOWN){
                svctx_cli_print_channel_change_log(pt_svctx, CH_XNG_STAGE_RECEIVED_RESOLUTION_CHG);
                svctx_mute_video(pt_svctx, FALSE);
                ch_hdlr_set_video_plane(pt_svctx, pt_svctx->e_default_vid_mode);

                if ((x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) == FALSE) &&
                    (pt_running_svc_req->t_strm_reqs[ST_AUDIO].e_penging_status != STRM_PENDING_COND_SELECT_STRM)) {
                    ch_hdlr_reload_audio_stream(pt_svctx, pt_running_svc_req);
                }
#if 0 /* [MS10] Will open ad only after main audio started. */
                if ((x_handle_valid(pt_running_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX].h_stream) == FALSE) &&
                    (pt_running_svc_req->t_strm_reqs[ST_AUDIO].e_penging_status != STRM_PENDING_COND_SELECT_STRM)) {
                    ch_hdlr_reload_ad_stream(pt_svctx, pt_running_svc_req);
                }
#endif
            }
        }
        /* on purpose to without break */
    case SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE:
        strm_fmt_chker_set_format_status(
                    pt_running_svc_req->pt_strm_fmt_chker,
                    e_strm_type
                    );
        b_check_svc_type = TRUE;
        break;
    case SVCTX_NTFY_CODE_NO_STREAM:
        /* as in stream selecting (i.e. sess cond = SM_SESS_COND_OPENED),
         * ignore the stream status check
         */
        if (player_get_sess_cond(pt_running_svc_req->pt_player) != SM_SESS_COND_OPENED) {
            b_check_svc_type = TRUE;
        }
        break;
    case SVCTX_NTFY_CODE_CA:
        b_check_svc_type = TRUE;
        break;
    default:
        break;
    }
#ifdef MW_ISDB_SUPPORT
    if (pt_svctx->b_is_brdcsting_svc == FALSE &&
        pt_svctx->b_is_blocked && pt_svctx->b_block_notified)
    {
        /* suppress svc ntfy for non-broadcasting service */
        return;
    }
#endif

    if (!(e_strm_type == ST_VIDEO
            && pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ANALOG
            && (pt_running_svc_req->t_parent.ui4_stream_mode & ST_MASK_VIDEO) == 0)) {
        /* [specail case for analog audio only program]
         * only invoke svctx_notify_clients_strm_msg as not in analog audio only channel
         */
        svctx_notify_clients_strm_msg(
                    pt_svctx,
                    e_code,
                    pt_strm_req
                    );
    }

    if (b_check_svc_type == TRUE) {
        ch_hdlr_check_strms_status(pt_svctx);
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_resend_crnt_evn_nfy
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
VOID ch_hdlr_resend_crnt_evn_nfy(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED &&
        /* Maked sure service is not in replacing stage */
        !(pt_svctx->b_svc_selected && pt_svctx->pt_pending_svc_req))
#else
    if (pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED)
#endif
    {
        return;
    }

    switch (crnt_evn_get_cond(pt_running_svc_req->pt_crnt_event)) {
#ifndef MW_EDB_SUPPORT
    case EVCTX_COND_ACTIVE:
    case EVCTX_COND_AVAIL:
    case EVCTX_COND_UPDATED:
#else
    case EDB_COND_STARTED:
#endif /* MW_EDB_SUPPORT */
        ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CRNT_EVN_READY,
                    ST_UNKNOWN
                    );
        break;

#ifndef MW_EDB_SUPPORT
    case EVCTX_COND_UNAVAIL:
#else
    case EDB_COND_UNCONFIG:
    case EDB_COND_STOPPED:
#endif /* MW_EDB_SUPPORT */
        ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CRNT_EVN_NOT_READY,
                    ST_UNKNOWN
                    );
        break;

#ifndef MW_EDB_SUPPORT
    case EVCTX_COND_TRANSITION:
        ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CRNT_EVN_START_QUERY,
                    ST_UNKNOWN
                    );
        break;
#endif /* MW_EDB_SUPPORT */

    default:
        break;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_get_non_fake_cc_scdb_num_recs
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 ch_hdlr_get_non_fake_cc_scdb_num_recs(
                    SVCTX_T*                    pt_svctx,
                    UINT16*                     pui2_num_recs
                    )
{
    STREAM_COMP_ID_T t_comp_id;
    INT32            i4_ret;

    i4_ret = _ch_hdlr_get_strm_comp_id(
                    pt_svctx,
                    ST_CLOSED_CAPTION,
                    TRUE,
                    _ch_hdlr_cc_comp_count_filter,
                    (VOID*)pui2_num_recs,
                    &t_comp_id,
                    NULL
                    );

    return (i4_ret == SVCTXR_NOT_EXIST) ? SVCTXR_OK : SVCTXR_FAIL;
}

static BOOL _ch_hdlr_is_same_freq(
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req,
                    HANDLE_T                    h_conn,
                    CM_CTRL_TYPE_T              e_ctrl,
                    VOID*                       pv_ctrl_data
                    )
{
    ANALOG_BRDCST_CTRL_TUNE_ATTRIBUTES_T*   pt_tune_attr;
    TSL_REC_T                               t_tsl_rec = {0};
    INT32                                   i4_ret;

    if (pt_running_svc_req->t_parent.e_src_type == SRC_TYPE_BRDCST_FREQ)
    {
        return FALSE;
    }

    pt_tune_attr = (ANALOG_BRDCST_CTRL_TUNE_ATTRIBUTES_T*)pv_ctrl_data;
    if (!(pt_tune_attr->ui4_flags & ANALOG_BRDCST_TUNE_ATTRIBUTES_FINE_TUNE)) {
        return FALSE;
    }
    
    i4_ret = ch_hdlr_get_crnt_tsl_rec(pt_running_svc_req, &t_tsl_rec);
    if (i4_ret != SVCTXR_OK) {
        return FALSE;
    }

    switch(t_tsl_rec.uheader.t_desc.e_bcst_medium) {
    case BRDCST_MEDIUM_ANA_TERRESTRIAL:
        {
            TUNER_TER_ANA_TUNE_INFO_T t_tune_info_old;
            if ((i4_ret = x_cm_get(h_conn, ANALOG_BRDCST_CTRL_GET_TER_TUNE_INFO, &t_tune_info_old)) >= CMR_OK)
            {
                if (t_tune_info_old.ui4_freq == pt_tune_attr->ui4_freq)
                {
                    return TRUE;
                }
            }
        }
        break;
    case BRDCST_MEDIUM_ANA_CABLE:
        {
            TUNER_CAB_ANA_TUNE_INFO_T t_tune_info_old;
            if ((i4_ret = x_cm_get(h_conn, ANALOG_BRDCST_CTRL_GET_CAB_TUNE_INFO, &t_tune_info_old)) >= CMR_OK)
            {
                if (t_tune_info_old.ui4_freq == pt_tune_attr->ui4_freq)
                {
                    return TRUE;
                }
            }     
        }
        break;
    case BRDCST_MEDIUM_ANA_SATELLITE:
        {
            TUNER_SAT_ANA_TUNE_INFO_T t_tune_info_old;
            if ((i4_ret = x_cm_get(h_conn, ANALOG_BRDCST_CTRL_GET_SAT_TUNE_INFO, &t_tune_info_old)) >= CMR_OK)
            {
                if (t_tune_info_old.ui4_freq == pt_tune_attr->ui4_freq)
                {
                    return TRUE;
                }
            }
        }
        break;
    default:
        break;
    }
   
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: ch_hdlr_set_conn_attr
 *
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 ch_hdlr_set_conn_attr(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    CM_CTRL_TYPE_T              e_ctrl,
                    VOID*                       pv_ctrl_data
                    )
{
    CH_SVC_REQ_T*           pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T                h_conn;
    INT32                   i4_ret;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    ACTIVE_LINK_ENTRY_T*    pt_link;
    SVCTX_T*                pt_obj;
    INT32                   i = 0;
#endif

    h_conn = player_get_conn_handle(pt_running_svc_req->pt_player);
    if (x_handle_valid(h_conn) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    switch(e_ctrl) {
    case FM_RADIO_CTRL_SET_TUNE_FREQ:            /* FM finetune */
    case ANALOG_BRDCST_CTRL_SET_TUNE_ATTRIBUTES: /* ATV finetune */
        if (_ch_hdlr_is_same_freq(pt_svctx, pt_running_svc_req, h_conn, e_ctrl, pv_ctrl_data) == TRUE)
        {
            break;
        }
        
        if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) == TRUE 
                && pt_running_svc_req->t_strm_reqs[ST_AUDIO].e_strm_cond != SM_COND_CLOSING) {
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
        break;
    default:
        break;
    }

    i4_ret = x_cm_set(
                    h_conn,
                    e_ctrl,
                    pv_ctrl_data
                    );
    if (i4_ret >= CMR_OK) {
        switch(e_ctrl) {
        case ANALOG_BRDCST_CTRL_SET_SCART_INP_MODE:
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
                pt_running_svc_req = (CH_SVC_REQ_T*)pt_obj->pt_running_svc_req;
#endif
                pt_running_svc_req->t_src_info.u_info.t_svl.e_scart_mode = *((ANALOG_BRDCST_SCART_MODE_T*)pv_ctrl_data);
#ifdef MW_TV_AV_BYPASS_SUPPORT
            }
#endif
            break;
        case ANALOG_BRDCST_CTRL_SET_TUNE_ATTRIBUTES:
            pt_svctx->b_signal_loss_notified = FALSE;
            pt_svctx->b_in_fine_tuning       = TRUE;
            break;
        case ANALOG_BRDCST_CTRL_SET_SCART_BYPASS:
        {
            ANALOG_BRDCST_CTRL_SCART_BYPASS_T* pt_scart_bypass = (ANALOG_BRDCST_CTRL_SCART_BYPASS_T*)pv_ctrl_data;
#ifdef MW_TV_AV_BYPASS_SUPPORT
            SVCTX_T*        pt_svctx_assc = NULL;
            LINK_POS_T      t_pos = NULL;
#endif

#ifdef MW_TV_AV_BYPASS_SUPPORT
            /* Mute bypass when CANAL+ is activated and unmute bypass when CANAL+ is deactivated */
            while (1)
            {
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
                        (pt_svctx_assc->ui4_attribute & SVCTX_ASSC_ATTR_AUDIO_SLAVE) &&
                        !(pt_svctx_assc->ui4_attribute & SVCTX_ASSC_ATTR_SUPPORT_CANAL_PLUS))
                    {
                        svctx_set_bypass_mode(pt_svctx_assc, pt_scart_bypass->b_bypass);
                    }
                }
                else
                {
                    break;
                }
            }
#endif

            svctx_set_scart_out_params(
                    pt_svctx,
                    player_get_sess_handle(pt_running_svc_req->pt_player),
                    TRUE,
                    ST_MASK_AUDIO | ST_MASK_VIDEO,
                    &(pt_running_svc_req->t_parent)
                    );

            if (pt_scart_bypass->b_bypass == TRUE) {
                pt_running_svc_req->e_ch_src_type = CH_SRC_TYPE_TUNER;
                ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CH_SRC_FROM_TUNER,
                    ST_UNKNOWN
                    );
            } else {
                pt_running_svc_req->e_ch_src_type = CH_SRC_TYPE_SCART;
                ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CH_SRC_FROM_SCART,
                    ST_UNKNOWN
                    );
            }
            break;
        }
        default:
            break;
        }
    }

    return svctx_translate_cmr_2_svctxr(i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_check_scart_bypass
 * Description
 *      This API will resent last SCART bypass notification to client
 *      if bypass = TRUE  - notify SVCTX_NTFY_CODE_CH_SRC_FROM_TUNER
 *                  FALSE - notify SVCTX_NTFY_CODE_CH_SRC_FROM_SCART
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 ch_hdlr_check_scart_bypass (
                    SVCTX_T*                    pt_svctx,
                    CH_SVC_REQ_T*               pt_running_svc_req
                    )
{
    if (pt_running_svc_req->e_brdcst_type != BRDCST_TYPE_ANALOG) {
        return SVCTXR_OK;
    }

    if (pt_running_svc_req->t_src_info.u_info.t_svl.t_ch_encode.e_type != CHANNEL_DECODE_TYPE_MANUAL
            && pt_running_svc_req->t_src_info.u_info.t_svl.t_ch_encode.e_type != CHANNEL_DECODE_TYPE_AUTO) {
        return SVCTXR_OK;
    }

    if (pt_running_svc_req->e_ch_src_type == CH_SRC_TYPE_TUNER) {
        ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CH_SRC_FROM_TUNER,
                    ST_UNKNOWN
                    );
    } else if (pt_running_svc_req->e_ch_src_type == CH_SRC_TYPE_SCART){
        ch_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_CH_SRC_FROM_SCART,
                    ST_UNKNOWN
                    );
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: ch_hdlr_set_video_plane
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *-----------------------------------------------------------------------------*/
INT32 ch_hdlr_set_video_plane(
                    SVCTX_T*                        pt_svctx,
                    SCC_VID_MODE_T                  e_vid_mode
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req->e_brdcst_type == BRDCST_TYPE_ANALOG
            && (pt_running_svc_req->t_parent.ui4_stream_mode & ST_MASK_VIDEO) == 0
            && e_vid_mode != SCC_VID_BLANK) {
        /* [specail case for analog audio only program]
         * Even client set stream mode to without video, we still need to play
         * video stream in background
         */
        e_vid_mode = SCC_VID_BLANK;
    }

    return svctx_set_video_plane(pt_svctx, e_vid_mode);
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_hdlr_get_scdb_num_recs
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 ch_hdlr_get_scdb_num_recs(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    UINT16*                     pui2_num_recs
                    )
{
    CH_SVC_REQ_T* pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    UINT32        ui4_db_version     = SCDB_NULL_VER_ID;
    HANDLE_T      h_scdb             = NULL_HANDLE;
    INT32         i4_ret;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    h_scdb = player_get_scdb_handle(pt_running_svc_req->pt_player);
    if (x_handle_valid(h_scdb) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    if (e_strm_type == ST_CLOSED_CAPTION) {
        i4_ret = ch_hdlr_get_non_fake_cc_scdb_num_recs(
                        pt_svctx,
                        pui2_num_recs
                        );
    } else {
        i4_ret = x_scdb_get_num_recs(
                        h_scdb,
                        e_strm_type,
                        pui2_num_recs,
                        &ui4_db_version
                        );
        i4_ret = (i4_ret == SCDBR_OK) ? SVCTXR_OK : SVCTXR_FAIL;
    }

    return i4_ret;
}

static INT32 _ch_hdlr_get_ad_num_recs(
                    SVCTX_T*                    pt_svctx,
                    UINT16*                     pui2_num_recs
                    )
{
    CH_SVC_REQ_T*   pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    UINT32          ui4_db_version     = SCDB_NULL_VER_ID;
    HANDLE_T        h_scdb             = NULL_HANDLE;
    INT32           i4_ret             = SVCTXR_FAIL;
    UINT16          ui2_num_aud_recs   = 0;
    UINT16          ui2_num_ad_recs    = 0;
    UINT16          ui2_idx            = 0;
    STREAM_COMP_ID_T    t_comp_id;
    SCDB_REC_T          t_scdb_rec;
    STRM_REQ_TYPE_T     e_strm_req_type = STRM_REQ_PID;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    ui2_num_ad_recs = 0;

    i4_ret =  _ch_hdlr_get_strm_req_type(
                    pt_svctx,
                    ST_AUDIO,
                    &e_strm_req_type);
    if (SVCTXR_OK != i4_ret || STRM_REQ_SCDB != e_strm_req_type) { 
        return SVCTXR_NOT_SUPPORT;
    }

    h_scdb = player_get_scdb_handle(pt_running_svc_req->pt_player);
    if (x_handle_valid(h_scdb) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    /* Get audio record number */
    i4_ret = x_scdb_get_num_recs(
                    h_scdb,
                    ST_AUDIO,
                    &ui2_num_aud_recs,
                    &ui4_db_version
                    );
    if (i4_ret != SCDBR_OK) {
        DBG_ERROR((_ERROR_HEADER"x_scdb_get_num_recs() failed, return = %d\r\n", i4_ret));
        return SVCTXR_FAIL;
    }

    /* Iterate all the audio record, check and count the AD record number */
    for (ui2_idx = 0; ui2_idx < ui2_num_aud_recs; ui2_idx++)
    {
        i4_ret = x_scdb_get_rec_by_idx(
                        h_scdb,
                        ST_AUDIO,
                        ui2_idx,
                        &t_comp_id,
                        &t_scdb_rec,
                        &ui4_db_version
                        );
        if (i4_ret != SCDBR_OK) {
            DBG_ERROR((_ERROR_HEADER"x_scdb_get_num_recs() failed, return = %d\r\n", i4_ret));
            return SVCTXR_FAIL;
        }

        if (AUD_TYPE_VISUAL_IMPAIRED == t_scdb_rec.u.t_audio_mpeg.e_type 
                || AUD_CLASS_VISUAL_IMPAIRED_AD == t_scdb_rec.u.t_audio_mpeg.e_class) {
            ui2_num_ad_recs ++;
        }
    }

    *pui2_num_recs = ui2_num_ad_recs;
    return SVCTXR_OK;
}

static INT32 _ch_hdlr_get_ad_recs(
                    SVCTX_T*                    pt_svctx,
                    UINT16                      ui2_num_recs,
                    SCDB_REC_T*                 pt_scdb_rec
                    )
{
    CH_SVC_REQ_T*   pt_running_svc_req = (CH_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    UINT32          ui4_db_version     = SCDB_NULL_VER_ID;
    HANDLE_T        h_scdb             = NULL_HANDLE;
    INT32           i4_ret             = SVCTXR_FAIL;
    UINT16          ui2_num_aud_recs   = 0;
    UINT16          ui2_num_ad_recs    = 0;
    UINT16          ui2_idx            = 0;
    STREAM_COMP_ID_T    t_comp_id;
    SCDB_REC_T          t_scdb_rec;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    h_scdb = player_get_scdb_handle(pt_running_svc_req->pt_player);
    if (x_handle_valid(h_scdb) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    /* Get audio record number */
    i4_ret = x_scdb_get_num_recs(
                    h_scdb,
                    ST_AUDIO,
                    &ui2_num_aud_recs,
                    &ui4_db_version
                    );
    if (i4_ret != SCDBR_OK) {
        DBG_ERROR((_ERROR_HEADER"x_scdb_get_num_recs() failed, return = %d\r\n", i4_ret));
        return SVCTXR_FAIL;
    }

    /* Iterate all the audio record, copy and count the AD record number */
    ui2_num_ad_recs = 0;

    t_scdb_rec.u.t_audio_mpeg.e_type = AUD_TYPE_UNKNOWN;
    t_scdb_rec.u.t_audio_mpeg.e_class = AUD_CLASS_RESERVED;
    
    for (ui2_idx = 0; ui2_idx < ui2_num_aud_recs; ui2_idx++)
    {
        i4_ret = x_scdb_get_rec_by_idx(
                        h_scdb,
                        ST_AUDIO,
                        ui2_idx,
                        &t_comp_id,
                        &t_scdb_rec,
                        &ui4_db_version
                        );
        if (i4_ret != SCDBR_OK) {
            DBG_ERROR((_ERROR_HEADER"x_scdb_get_num_recs() failed, return = %d\r\n", i4_ret));
            return SVCTXR_FAIL;
        }

        if (AUD_TYPE_VISUAL_IMPAIRED == t_scdb_rec.u.t_audio_mpeg.e_type
                || AUD_CLASS_VISUAL_IMPAIRED_AD == t_scdb_rec.u.t_audio_mpeg.e_class) {
            *(pt_scdb_rec + ui2_num_ad_recs) = t_scdb_rec;
            ui2_num_ad_recs ++;
            if (ui2_num_ad_recs == ui2_num_recs) {
                break;
            }
        }
    }

    /* If current ad record num in scdb is less than requested, nullify the rest
     * unused records
     */
    if (ui2_num_ad_recs < ui2_num_recs) {
        x_memset(&t_scdb_rec, 0, sizeof(SCDB_REC_T));
        t_scdb_rec.e_rec_type = SCDB_REC_TYPE_UNKNOWN;
        t_scdb_rec.u.t_audio_mpeg.e_type = AUD_TYPE_UNKNOWN;

        for (ui2_idx = ui2_num_ad_recs; ui2_idx < ui2_num_recs; ui2_idx++) {
            *(pt_scdb_rec + ui2_idx) = t_scdb_rec;
        }
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * export methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      x_ch_hdlr_reg_handler
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 x_ch_hdlr_reg_handler(
                    VOID
                    )
{
    return ch_hdlr_init();
}
