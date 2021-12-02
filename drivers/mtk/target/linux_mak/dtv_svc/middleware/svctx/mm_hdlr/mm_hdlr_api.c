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
 * $RCSfile: mm_hdlr_api.c,v $
 * $Revision: #6 $
 * $Date: 2012/08/08 $
 * $Author: zhenlu.liu $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/46 $
 * $SWAuthor: Weider Chang $
 * $MD5HEX: f0b308e28852e901dc8386943dc577db $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "svctx/mm_hdlr/mm_hdlr.h"
#include "svctx/mm_hdlr/mm_hdlr_cli.h"
#include "svctx/svctx_dbg.h"
#include "strm_mngr/sbtl_hdlr/u_sm_sbtl_hdlr.h"
#include "conn_mngr/x_cm.h"
#include "svctx/svctx_cli.h"
#include "conn_mngr/cm_playback/x_playback_handler.h"
#include "strm_mngr/sess_gnrc_hdlr/u_sm_sess_gnrc.h"
#include "strm_mngr/mm_sbtl_hdlr/u_sm_mm_sbtl_hdlr.h"
#include "mutil/sbtl_engine/u_mm_sbtl_engine.h"
#include "mutil/minfo/x_minfo.h"
#include "mutil/midxbuld/x_midxbuld.h"
#include "mutil/mm_util.h"
#ifdef DLNA_SUPPORT
#include "file_mngr/dlnafs/x_fm_dlnafs.h"
#endif
#include "font/x_fe.h"  /*add for font info*/

#ifndef ANDROID
#include "graphic/x_gl.h"
#include "wgl/x_wgl.h"
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

#define SVC_REQ_AMOUNT_FACTOR   (2)
#define STRM_COMP_FILTER_LOOP_BASE              (((UINT16) 0x01) << STRM_COMP_FILTER_LOOP_START_BIT_IDX)
#define STRM_COMP_FILTER_MAKE_INDEX(x,loop)     ((UINT16) (x) | (STRM_COMP_FILTER_LOOP_BASE * (loop)))

#define MM_HDLE_VID_TRACK_IDX_ONE                 (0)
#define MM_HDLE_VID_TRACK_IDX_TWO                 (1)

/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
#ifdef TIME_SHIFT_SUPPORT
static VOID _mm_hdlr_reload_cc_strm_as_presenting (
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_new_svc_req
                    );
#endif
static INT32 _mm_hdlr_set(
                    SVCTX_T*                    pt_svctx,
                    SVCTX_SET_TYPE_T            e_set_type,
                    VOID*                       pv_set_info,
                    SIZE_T                      z_set_info_size
                    );
static INT32 _mm_hdlr_get(
                    SVCTX_T*                    pt_svctx,
                    SVCTX_GET_TYPE_T            e_get_type,
                    VOID*                       pv_get_info,
                    SIZE_T*                     pz_get_info_size
                    );

/*-----------------------------------------------------------------------------
 * private variable declarations
 *---------------------------------------------------------------------------*/
static BOOL                     b_g_init               = FALSE;
extern const STREAM_TYPE_T      e_g_mm_strm_types[]; /* QQQQQQ */
BOOL                     gb_tm_on_off = FALSE;
/*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_allocate_svc_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
static SVC_REQ_T* _mm_hdlr_allocate_svc_req(
                    VOID
                    )
{
    MM_SVC_REQ_T* pt_svc_req;
    INT32         i4_ret;

    i4_ret = mm_svc_req_allocate(&pt_svc_req);
    if (i4_ret == SVCTXR_OK) 
    {
        return ((SVC_REQ_T*)pt_svc_req);
    } 
    else 
    {
        return NULL;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_free_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
static VOID _mm_hdlr_free_strm_req(
                    STRM_REQ_T*                 pt_strm_req
                    )
{
    mm_strm_req_free(pt_strm_req);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_free_svc_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
static VOID _mm_hdlr_free_svc_req(
                    SVC_REQ_T*                  pt_svc_req
                    )
{
    mm_svc_req_free((MM_SVC_REQ_T*)pt_svc_req);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_allocate_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
static STRM_REQ_T* _mm_hdlr_allocate_strm_req(
                    VOID
                    )
{

    STRM_REQ_T* pt_strm_req;
    INT32       i4_ret;

    i4_ret = mm_strm_req_allocate(&pt_strm_req);
    if (i4_ret == SVCTXR_OK) 
    {
        return pt_strm_req;
    } 
    else 
    {
        return NULL;
    }
}
#ifdef TIME_SHIFT_SUPPORT
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_resent_nfy_as_replacing
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static VOID _mm_hdlr_resent_nfy_as_replacing(
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req
                    )
{
    if (pt_svctx->e_state == SVCTX_COND_PRESENTING) 
    {
        pt_svctx->e_state = SVCTX_COND_PRESENTING_SELECT_SAME_REQ;
    } 
    else if (pt_svctx->e_state == SVCTX_COND_PENDING) 
    {
        pt_svctx->e_state = SVCTX_COND_PENDING_SELECT_SAME_REQ;
    }

    do 
    {
        mm_hdlr_check_svc_block(pt_svctx);

        if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOCKED) 
        {
            mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx, 
                                              SVCTX_NTFY_CODE_SIGNAL_LOCKED, 
                                              ST_UNKNOWN);
        } 
        else if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) 
        {
            if (mm_hdlr_check_signal_loss_priority(pt_svctx, 
                                                   pt_running_svc_req, 
                                                   FALSE) == TRUE) 
            {
                mm_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_SIGNAL_LOSS,
                        ST_UNKNOWN
                        );
                break;
            }
        } 
        else if (pt_svctx->e_signal_status == SIGNAL_STATUS_DETECTING) 
        {
            mm_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_SIGNAL_DETECTING,
                    ST_UNKNOWN
                    );
        }

        mm_hdlr_get_current_event(pt_svctx);

        if (pt_svctx->b_is_blocked == TRUE) 
        {
            mm_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx, 
                    SVCTX_NTFY_CODE_SERVICE_BLOCKED, 
                    ST_UNKNOWN);
            break;
        }

        /* If it is still under "delay SCDB empty check" after session opened, just don't
         * need to check AV status as replacing service request.
         */
        /*if (pt_running_svc_req->b_in_scdb_empty_check_delay == FALSE) 
        {
            svctx_set_av_status(pt_svctx, SVC_AV_STATUS_UNKNOWN);
            strm_fmt_chker_init_present_status(pt_running_svc_req->pt_strm_fmt_chker);

            if (pt_svctx->e_state == SVCTX_COND_PRESENTING_SELECT_SAME_REQ) 
            {
                mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx, SVCTX_NTFY_CODE_NORMAL, ST_UNKNOWN);
                mm_hdlr_check_strms_status(pt_svctx);
            }

            mm_hdlr_check_video_fmt(pt_svctx, pt_running_svc_req);
            mm_hdlr_check_audio_fmt(pt_svctx, pt_running_svc_req);
        } 
        else*/ 
        {
            if (pt_svctx->e_state == SVCTX_COND_PRESENTING_SELECT_SAME_REQ) 
            {
#ifdef TIME_SHIFT_SUPPORT
                INT32               i4_ret = SVCTXR_OK;
                MEDIA_DESCRIBT_T*   pt_media_desc = NULL;
                i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc,
                                                &pt_media_desc); 
#endif
                
                svctx_set_av_status(pt_svctx, SVC_AV_STATUS_UNKNOWN);
                mm_hdlr_invoke_client_svc_nfy_fct(
                        pt_svctx, 
                        SVCTX_NTFY_CODE_NORMAL, 
                        ST_UNKNOWN);

                if (TRUE == svctx_is_valid_video_fmt(pt_svctx))
                {
                    mm_hdlr_invoke_client_strm_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE,
                        &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]));
                }

                if (TRUE == svctx_is_valid_audio_fmt(pt_svctx))
                {
                    mm_hdlr_invoke_client_strm_nfy_fct(
                        pt_svctx,
                        SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE,
                        &(pt_running_svc_req->t_strm_reqs[ST_AUDIO]));
                }
#ifdef TIME_SHIFT_SUPPORT
                else if ((i4_ret == SVCTXR_OK)                           &&
                         (MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt))  &&
                         (pt_media_desc->b_is_scdb_updated == FALSE)     &&
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
#endif
            }
        }

        if (pt_svctx->e_cc_avil_cond == SVCTX_CC_AVIL_COND_YES) 
        {
            mm_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_WITH_CC_AVIL_INDICATOR,
                    ST_CLOSED_CAPTION
                    );
        } 
        else if (pt_svctx->e_cc_avil_cond == SVCTX_CC_AVIL_COND_NO) 
        {
            mm_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    SVCTX_NTFY_CODE_NO_CC_AVIL_INDICATOR,
                    ST_CLOSED_CAPTION
                    );
        }
    } while (0);

    if (pt_svctx->e_state == SVCTX_COND_PRESENTING_SELECT_SAME_REQ) 
    {
        pt_svctx->e_state = SVCTX_COND_PRESENTING;
    } 
    else if (pt_svctx->e_state == SVCTX_COND_PENDING_SELECT_SAME_REQ) 
    {
        pt_svctx->e_state = SVCTX_COND_PENDING;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_replace_strm_req
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_replace_strm_req (
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_new_svc_req,
                    MM_SVC_REQ_T*               pt_old_svc_req
                    )
{
    STREAM_TYPE_T e_strm_type;
    UINT8         ui1_idx;

    for (ui1_idx = 0; ui1_idx < (UINT8) STRM_TYPE_COUNT; ui1_idx++) 
    {
        e_strm_type = e_g_mm_strm_types[ui1_idx];
        if (e_strm_type == ST_UNKNOWN) 
        {
            continue;
        }
        if (e_strm_type >= STRM_TYPE_COUNT) 
        {
            break;
        }

        if ((pt_new_svc_req->t_parent.ui4_stream_mode & MAKE_BIT_MASK_32(e_strm_type)) == 0) 
        {
            /* if invisible CC, still count it in */
            if (!(e_strm_type == ST_CLOSED_CAPTION
                    && SVCTX_IS_INVISIBLE_CC_FILTER(pt_old_svc_req->t_cc_filter))) 
            {
                continue;
            }
        }

        if ((pt_old_svc_req->t_parent.ui4_stream_mode & MAKE_BIT_MASK_32(e_strm_type)) == 0) 
        {
            if (pt_svctx->b_is_blocked == FALSE) 
            {
                switch(e_strm_type) 
                {
                case ST_AUDIO:
                    mm_hdlr_reload_stream(pt_svctx, pt_new_svc_req, ST_AUDIO);
                    break;
                case ST_VIDEO:   
                    mm_hdlr_reload_stream(pt_svctx, pt_new_svc_req, ST_VIDEO);
                    break;
                /*case STRM_REQ_AD_INDEX:
                    mm_hdlr_reload_stream(pt_svctx, pt_new_svc_req, STRM_REQ_AD_INDEX);                    
                    break;*/
                default:
                    break;
                }
            }
            continue;
        }
        
        /*if (STRM_REQ_AD_INDEX == (INT32) ui1_idx)
        {
            pt_new_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX]          = pt_old_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX];
            pt_new_svc_req->pt_pending_strm_reqs[STRM_REQ_AD_INDEX] = pt_old_svc_req->pt_pending_strm_reqs[STRM_REQ_AD_INDEX];

            pt_new_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX].h_svctx         = pt_new_svc_req->t_parent.h_svctx;
            pt_new_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX].pf_strm_nfy     = NULL;
            pt_new_svc_req->t_strm_reqs[STRM_REQ_AD_INDEX].pv_strm_nfy_tag = NULL;
        }
        else*/
        {
        pt_new_svc_req->t_strm_reqs[e_strm_type]
                            = pt_old_svc_req->t_strm_reqs[e_strm_type];
        pt_new_svc_req->pt_pending_strm_reqs[e_strm_type] 
                            = pt_old_svc_req->pt_pending_strm_reqs[e_strm_type];

        pt_new_svc_req->t_strm_reqs[e_strm_type].h_svctx         
                            = pt_new_svc_req->t_parent.h_svctx;
        pt_new_svc_req->t_strm_reqs[e_strm_type].pf_strm_nfy     
                            = pt_new_svc_req->t_parent.pf_svc_nfy;
        pt_new_svc_req->t_strm_reqs[e_strm_type].pv_strm_nfy_tag 
                            = pt_new_svc_req->t_parent.pv_svc_nfy_tag;
        }
    }

    if (pt_svctx->b_in_stopping)
    {
        /* reset the stream req array */
        for (ui1_idx = 0; ui1_idx < (UINT8) STRM_TYPE_COUNT; ui1_idx++)
        {
            pt_old_svc_req->pt_pending_strm_reqs[ui1_idx] = NULL;
            svctx_reset_strm_req(&(pt_old_svc_req->t_strm_reqs[ui1_idx]));
        }
    }
        
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_replace_video_attributes
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static VOID _mm_hdlr_replace_video_attributes (
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_new_svc_req,
                    MM_SVC_REQ_T*               pt_old_svc_req
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
 *      _mm_hdlr_replace_svc_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_replace_svc_req(
                    SVCTX_T*                    pt_svctx
                    )
{
    INT32 i4_ret = SVCTXR_FAIL;

    do {
        MM_SVC_REQ_T* pt_replacer = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
        MM_SVC_REQ_T* pt_replacee = (MM_SVC_REQ_T*)pt_svctx->pt_pending_svc_req;
        BOOL          b_same_cc_req      = TRUE;

        ASSERT(pt_replacer != NULL, ("%s(), pt_replacer is NULL\r\n", __func__));
        ASSERT(pt_replacee != NULL, ("%s(), pt_replacee is NULL\r\n", __func__));

        if (mm_svc_req_equals(pt_replacer, pt_replacee) == FALSE) 
        {
            i4_ret = SVCTXR_FAIL;
            break;
        }

    
        _mm_hdlr_replace_video_attributes(
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
        pt_replacer->e_crnt_tuner_type           = pt_replacee->e_crnt_tuner_type;
        pt_replacer->pt_player                   = pt_replacee->pt_player;
        pt_replacer->pt_player_enhance           = pt_replacee->pt_player_enhance;
        pt_replacer->pt_player_act               = pt_replacee->pt_player_act;
        pt_replacer->pt_crnt_event               = pt_replacee->pt_crnt_event;
        pt_replacer->pt_rating_chker             = pt_replacee->pt_rating_chker;
        pt_replacer->pt_strm_fmt_chker           = pt_replacee->pt_strm_fmt_chker;
        pt_replacer->h_media_desc                = pt_replacee->h_media_desc;
        pt_replacer->t_uop_fct_tbl               = pt_replacee->t_uop_fct_tbl;
        pt_replacer->e_init_speed                = pt_replacee->e_init_speed;
        pt_replacer->t_init_pos_info             = pt_replacee->t_init_pos_info;

        pt_replacee->pt_player         = NULL;
        pt_replacee->pt_crnt_event     = NULL;
        pt_replacee->pt_rating_chker   = NULL;
        pt_replacee->pt_strm_fmt_chker = NULL;

        _mm_hdlr_replace_strm_req(pt_svctx, pt_replacer, pt_replacee);

        if (pt_svctx->e_state == SVCTX_COND_PRESENTING) 
        {
            if (b_same_cc_req == FALSE) 
            {
                if (pt_replacer->t_cc_filter.e_cc_type == SM_CC_TYPE_NTSC_LINE21
                        || pt_replacer->t_cc_filter.e_cc_type == SM_CC_TYPE_DTVCC)
                {
                    _mm_hdlr_reload_cc_strm_as_presenting(pt_svctx, pt_replacer);
                }
            }
        }

        _mm_hdlr_resent_nfy_as_replacing(pt_svctx, pt_replacer);
        i4_ret = SVCTXR_OK;
    } while (0);

    return i4_ret;
}
#endif

static INT32 _mm_hdlr_buf_info_convert(MEDIA_DESCRIBT_T*   pt_media_desc)
{
    MM_SVC_BUFFCTRL_INFO_T* pt_bufctrl_info = NULL;
    if(pt_media_desc == NULL)
    {
        return SVCTXR_FAIL;
    }
    pt_bufctrl_info = &(pt_media_desc->t_bufctrl_info);
    if((pt_bufctrl_info->ui4_keepbuf_threshold > 100)
        || (pt_bufctrl_info->ui4_rebuf_threshold > 100))/*input: x%*/
    {
        return SVCTXR_INV_ARG;
    }
    
    if(pt_bufctrl_info->e_ctrl_type == MM_SVC_BUFFCTRL_TYPE_BYTE)
    {
        pt_bufctrl_info->ui4_keepbuf_threshold
            *= pt_bufctrl_info->ui4_buf_size.ui4_bytes/100;
        pt_bufctrl_info->ui4_rebuf_threshold
            *= pt_bufctrl_info->ui4_buf_size.ui4_bytes/100;
    }
    else if(pt_bufctrl_info->e_ctrl_type == MM_SVC_BUFFCTRL_TYPE_SEC)
    {
        pt_bufctrl_info->ui4_buf_size.ui4_bytes
            = (UINT32)(_mm_div64(pt_media_desc->ui8_media_sz 
                                        * pt_bufctrl_info->ui4_buf_size.ui4_duration*90, 
                                 pt_media_desc->ui8_media_dur, 
                                 NULL));
        pt_bufctrl_info->ui4_keepbuf_threshold
            *= pt_bufctrl_info->ui4_buf_size.ui4_bytes/100*pt_media_desc->ui8_media_sz;
        pt_bufctrl_info->ui4_rebuf_threshold
            *= pt_bufctrl_info->ui4_buf_size.ui4_bytes/100*pt_media_desc->ui8_media_sz;
    }
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_preproc_svc_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_preproc_svc_req(
                    SVCTX_T*                    pt_svctx,
                    UINT32                      ui4_stream_mode,
                    const SRC_DESC_T*           pt_src_desc,
                    const SNK_DESC_T*           pt_snk_desc,
                    x_svctx_select_nfy_fct      pf_nfy,
                    VOID*                       pv_nfy_tag,
                    MM_SVC_REQ_T*               pt_svc_req
                    )
{
    INT32         i4_ret;
    UINT8         ui1_idx;
#ifdef TIME_SHIFT_SUPPORT
    SIZE_T        z_size;
    CHAR*         ps_src_grp_name    = NULL;
    CHAR*         ps_snk_grp_name    = NULL;
    CHAR*         ps_snk_grp_name_aux = NULL;
#endif
    MM_SVC_REQ_T* pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req != NULL) 
    {
        for (ui1_idx = 0; ui1_idx < (UINT8)STRM_TYPE_COUNT; ui1_idx++) 
        {
            STREAM_TYPE_T e_strm_type = e_g_mm_strm_types[ui1_idx];

            if (e_strm_type >= STRM_TYPE_COUNT || e_strm_type == ST_UNKNOWN) 
            {
                break;
            }
            if (pt_running_svc_req->t_strm_reqs[e_strm_type].h_stream == NULL_HANDLE) 
            {
                continue;
            }
            /* close stream first if new request without related stream */
            if ((pt_running_svc_req->t_parent.ui4_stream_mode & MAKE_BIT_MASK_32(e_strm_type)) == 0
                    || (ui4_stream_mode & MAKE_BIT_MASK_32(e_strm_type)) != 0) 
            {
                continue;
            }

            i4_ret = x_sm_close_stream(pt_running_svc_req->t_strm_reqs[e_strm_type].h_stream);
            if (i4_ret == SMR_OK || i4_ret == SMR_PENDING) 
            {
                pt_running_svc_req->t_strm_reqs[e_strm_type].h_stream  = NULL_HANDLE;
                pt_running_svc_req->t_strm_reqs[e_strm_type].e_req_type= STRM_REQ_SCDB;
            }
        }
    }

    /* 1. check source related parameters */
    if (pt_src_desc == NULL) 
    {
        return SVCTXR_INV_ARG;
    }
    if (pt_svc_req == NULL)
    {
         return SVCTXR_INV_ARG;
    }
    switch (pt_src_desc->e_type) 
    {
#ifdef TIME_SHIFT_SUPPORT
    case SRC_TYPE_MEDIA_TIME_SHIFT:
        if (pt_src_desc->ui2_svc_lst_id != 0 && pt_src_desc->ui2_svl_rec_id != 0) 
        {
            pt_svc_req->t_src_info.u_info.t_svl.ui2_svc_lst_id = pt_src_desc->ui2_svc_lst_id;
            pt_svc_req->t_src_info.u_info.t_svl.ui2_svl_rec_id = pt_src_desc->ui2_svl_rec_id;
        } 
        else if (pt_src_desc->pv_details != NULL) 
        {
            pt_svc_req->t_src_info.u_info.t_svl = *((SRC_SVL_INFO_T*)pt_src_desc->pv_details);
        } 
        else 
        {
            pt_svc_req->t_src_info.u_info.t_svl = pt_src_desc->u_info.t_svl;
        }
        
        /*i4_ret = mm_hdlr_get_ids(pt_svc_req);
        if (SVCTXR_OK != i4_ret)
        {
            return i4_ret;
        }*/
        
        /* source name */
        if (pt_running_svc_req != NULL) 
        {
            ps_src_grp_name = pt_running_svc_req->t_src_info.s_src_grp_name;
        }
        
        if (pt_src_desc != NULL) 
        {
            z_size = x_strlen(pt_src_desc->ps_conn_src_name);
            if (z_size > 0 && z_size <= SVCTX_SRC_NAME_MAX_SIZE) 
            {
                ps_src_grp_name = pt_src_desc->ps_conn_src_name;
            }
        }
        
        if (ps_src_grp_name == NULL) 
        {
            return SVCTXR_INV_ARG;
        }
        
        x_strcpy(pt_svc_req->t_src_info.s_src_grp_name, ps_src_grp_name);
#endif /* TIME_SHIFT_SUPPORT   intention missing break*/
    case SRC_TYPE_MEDIA_STORGE:
    case SRC_TYPE_MEDIA_PVR:
    case SRC_TYPE_MEDIA_MEM_CLIP:
    case SRC_TYPE_MEDIA_NETWORK:
        pt_svc_req->t_parent.e_src_type = pt_src_desc->e_type;
        pt_svc_req->h_media_desc = pt_src_desc->u_info.t_mm.h_media;
#if 1
        pt_svc_req->e_init_speed = pt_src_desc->u_info.t_mm.t_speed;
        pt_svc_req->t_init_pos_info = pt_src_desc->u_info.t_mm.t_pos_info;
#endif /* TIME_SHIFT_SUPPORT */
#if 1/*MKV DivXPlus*/
        pt_svc_req->ui4_title_idx =  pt_src_desc->ui4_title_idx;
        pt_svc_req->ui4_playlist_idx=  pt_src_desc->ui4_playlist_idx;
        pt_svc_req->ui4_chap_idx =  pt_src_desc->ui4_chap_idx;
#endif

        /*add for common API*/        
        {
            HANDLE_TYPE_T       e_type;
            MEDIA_DESCRIBT_T*   pt_media_desc;
            i4_ret = handle_get_type_obj(pt_src_desc->u_info.t_mm.h_media, &e_type, (VOID**)(&pt_media_desc));
            if (i4_ret != HR_OK || e_type != HT_MEDIA_STORGE_ELM_TYPE || pt_media_desc == NULL)
            {
                DBG_ERROR((_ERROR_HEADER
                           "handle_get_type_obj(h_media_desc=%d) failed!\r\n"
                           "i4_ret=%d, e_type=%d, pv_obj=%d\r\n",
                           pt_src_desc->u_info.t_mm.h_media, i4_ret, e_type, pt_media_desc));
                return SVCTXR_FAIL;
            }
            pt_media_desc->ui2_svc_id = 0/*pt_src_desc->u_info.t_mm.ui2_svc_id*/;
            pt_media_desc->ui2_svc_pid = pt_src_desc->u_info.t_mm.ui2_svc_pid;
            pt_media_desc->b_still_at_last = pt_src_desc->u_info.t_mm.b_still_at_last;
            pt_media_desc->b_sub_free_run = pt_src_desc->u_info.t_mm.b_sub_free_run;

            if ((pt_media_desc->e_svc_protocol == PROTOCL_MODE_URI) ||
                (pt_media_desc->e_svc_protocol == PROTOCL_MODE_PULL) ||
                (pt_media_desc->e_svc_protocol == PROTOCL_MODE_PUSH))
            {
                pt_media_desc->t_bufctrl_info.b_is_prebuff = TRUE;
                pt_media_desc->t_bufctrl_info = pt_src_desc->u_info.t_mm.t_bufctrl_info;
                _mm_hdlr_buf_info_convert(pt_media_desc);
                pt_svc_req->t_bufctrl_info = pt_media_desc->t_bufctrl_info;
            }
#ifdef MM_LAST_MEMORY_SUPPORT
            if(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)
            {
                if(pt_media_desc->ui2_last_num < 10)
                {
                   pt_media_desc->t_mm_pmt_last_info[pt_media_desc->ui2_last_num].ui4_title_idx = pt_src_desc->u_info.t_mm.ui4_title_idx;
                   pt_media_desc->t_mm_pmt_last_info[pt_media_desc->ui2_last_num].ui4_playlist_idx= 0;
                   pt_media_desc->t_mm_pmt_last_info[pt_media_desc->ui2_last_num].ui4_chap_idx= 0;
                   pt_media_desc->t_mm_pmt_last_info[pt_media_desc->ui2_last_num].t_mm_svc_pos_info = pt_src_desc->u_info.t_mm.t_pos_info;
                   pt_media_desc->ui2_last_num++;
                }
                else if(pt_media_desc->ui2_last_num == 10 || pt_media_desc->ui2_last_num > 10)
                {
                   UINT16 ui2_idx;
                   for(ui2_idx = 0; ui2_idx < 9; ui2_idx++)
                   {
                       x_memcpy(&pt_media_desc->t_mm_pmt_last_info[ui2_idx], &pt_media_desc->t_mm_pmt_last_info[ui2_idx+1], sizeof(MM_SVC_STOP_INFO));
                   }
                   x_memset(&pt_media_desc->t_mm_pmt_last_info[9], 0, sizeof(MM_SVC_STOP_INFO));
                   pt_media_desc->t_mm_pmt_last_info[9].ui4_title_idx = pt_src_desc->u_info.t_mm.ui4_title_idx;
                   pt_media_desc->t_mm_pmt_last_info[9].ui4_playlist_idx= 0;
                   pt_media_desc->t_mm_pmt_last_info[9].ui4_chap_idx= 0;
                   pt_media_desc->t_mm_pmt_last_info[9].t_mm_svc_pos_info = pt_src_desc->u_info.t_mm.t_pos_info;
                }
            }
            else if(pt_src_desc->u_info.t_mm.ui4_title_idx < 3 && 
                pt_src_desc->u_info.t_mm.ui4_playlist_idx < 3)
            {
                pt_media_desc->t_mm_title_last_info[pt_src_desc->u_info.t_mm.ui4_title_idx].ui4_playlist_idx = pt_src_desc->u_info.t_mm.ui4_playlist_idx;
                pt_media_desc->t_mm_title_last_info[pt_src_desc->u_info.t_mm.ui4_title_idx].ui4_chap_idx = pt_src_desc->u_info.t_mm.ui4_chap_idx;
                pt_media_desc->t_mm_playlist_last_info[pt_src_desc->u_info.t_mm.ui4_playlist_idx].t_mm_svc_pos_info = pt_src_desc->u_info.t_mm.t_pos_info;
                pt_media_desc->t_mm_playlist_last_info[pt_src_desc->u_info.t_mm.ui4_playlist_idx].ui4_chap_idx = pt_src_desc->u_info.t_mm.ui4_chap_idx;
                pt_media_desc->t_mm_title_last_info[pt_src_desc->u_info.t_mm.ui4_title_idx].t_mm_svc_pos_info = pt_src_desc->u_info.t_mm.t_pos_info;
            }
            else
            {
                x_memset(&pt_media_desc->t_mm_last_info.t_mm_svc_pos_info, 0, sizeof(MM_SVC_POS_INFO_T));
            }
#endif            
        }
    
        break;
        default:
            return SVCTXR_INV_ARG;
        }

#ifdef SYS_RECORD_SUPPORT
    {
        HANDLE_TYPE_T       e_type;
        MEDIA_DESCRIBT_T*   pt_media_desc;
        i4_ret = handle_get_type_obj(pt_src_desc->u_info.t_mm.h_media, &e_type, (VOID**)(&pt_media_desc));
        if (i4_ret != HR_OK || e_type != HT_MEDIA_STORGE_ELM_TYPE || pt_media_desc == NULL)
        {
            DBG_ERROR((_ERROR_HEADER
                       "handle_get_type_obj(h_media_desc=%d) failed!\r\n"
                       "i4_ret=%d, e_type=%d, pv_obj=%d\r\n",
                       pt_src_desc->u_info.t_mm.h_media, i4_ret, e_type, pt_media_desc));
            return SVCTXR_FAIL;
        }
        if(pt_media_desc->e_src_type == SRC_TYPE_MEDIA_PVR)
        {
            pt_svc_req->t_parent.e_src_type = SRC_TYPE_MEDIA_PVR;
        }
    }
#endif

#ifdef TIME_SHIFT_SUPPORT
    /* 2. check sink related parameters */
    /* sink name */
    if (pt_running_svc_req != NULL) 
    {
        ps_snk_grp_name = pt_running_svc_req->t_parent.s_snk_grp_name;
    }

    if (pt_snk_desc != NULL) 
    {
        z_size = x_strlen(pt_snk_desc->ps_snk_grp_name);
        if (z_size > 0 && z_size <= SVCTX_SNK_NAME_MAX_SIZE) 
        {
            ps_snk_grp_name = pt_snk_desc->ps_snk_grp_name;
        }

        /* modified by Allen: Fix me after having the correct group name */
        z_size = x_strlen(pt_snk_desc->ps_snk_grp_name_aux);
        if (z_size > 0 && z_size <= SVCTX_SNK_NAME_MAX_SIZE) 
        {
            ps_snk_grp_name_aux = pt_snk_desc->ps_snk_grp_name_aux;
        }
        
        if (pt_snk_desc->pt_disp_region != NULL) 
        {
            pt_svctx->t_raw_disp_region = pt_svctx->t_disp_region;
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
    if (i4_ret != SVCTXR_OK) 
    {
        return i4_ret;
    }
#else
    /* copy name */
    if (pt_svc_req != NULL && pt_snk_desc != NULL)
    {
        if (pt_snk_desc->ps_snk_grp_name != NULL)
        {
            x_strcpy(pt_svc_req->t_parent.s_snk_grp_name, pt_snk_desc->ps_snk_grp_name);
        }
    }

    if (pt_snk_desc != NULL && pt_snk_desc->pt_video_region != NULL) 
    {
        pt_svctx->t_video_region = *(pt_snk_desc->pt_video_region);
    }
    if (pt_snk_desc != NULL && pt_snk_desc->pt_disp_region != NULL) 
    {
        pt_svctx->t_raw_disp_region = pt_svctx->t_disp_region;
        pt_svctx->t_disp_region = *(pt_snk_desc->pt_disp_region);
    }
#endif

    /* video stream filter */
    if (pt_snk_desc != NULL && pt_snk_desc->pt_video_strm_fltr_fct != NULL) 
    {
        pt_svc_req->t_strm_reqs[ST_VIDEO].pt_strm_fltr_fct 
                                    = pt_snk_desc->pt_video_strm_fltr_fct;
        pt_svc_req->t_strm_reqs[ST_VIDEO].pv_strm_fltr_tag 
                                    = pt_snk_desc->pv_video_strm_fltr_tag;
    } 
    else 
    {
        pt_svc_req->t_strm_reqs[ST_VIDEO].pt_strm_fltr_fct = NULL;
        pt_svc_req->t_strm_reqs[ST_VIDEO].pv_strm_fltr_tag = NULL;
    }

    /* audio stream filter */
    if ((pt_snk_desc != NULL) && (pt_snk_desc->pt_audio_strm_fltr_fct != NULL))
    {
        pt_svc_req->t_strm_reqs[ST_AUDIO].pt_strm_fltr_fct 
                                    = pt_snk_desc->pt_audio_strm_fltr_fct;
        pt_svc_req->t_strm_reqs[ST_AUDIO].pv_strm_fltr_tag 
                                    = pt_snk_desc->pv_audio_strm_fltr_tag;
    } 
    else 
    {
        pt_svc_req->t_strm_reqs[ST_AUDIO].pt_strm_fltr_fct = NULL;
        pt_svc_req->t_strm_reqs[ST_AUDIO].pv_strm_fltr_tag = NULL;
    }
    
    if (pt_snk_desc != NULL) 
    {
        switch(pt_snk_desc->e_text_strm_type) 
        {
        case ST_CLOSED_CAPTION:
            /* cc stream filter */
            if (pt_snk_desc != NULL && pt_snk_desc->u_text_strm.t_cc.pt_fltr_fct != NULL) 
            {
                pt_svc_req->t_strm_reqs[ST_CLOSED_CAPTION].pt_strm_fltr_fct = pt_snk_desc->u_text_strm.t_cc.pt_fltr_fct;
                pt_svc_req->t_strm_reqs[ST_CLOSED_CAPTION].pv_strm_fltr_tag = pt_snk_desc->u_text_strm.t_cc.pv_fltr_tag;
            } 
            else 
            {
                pt_svc_req->t_strm_reqs[ST_CLOSED_CAPTION].pt_strm_fltr_fct = NULL;
                pt_svc_req->t_strm_reqs[ST_CLOSED_CAPTION].pv_strm_fltr_tag = NULL;
            }

            /* set closed captioning request */
            if (pt_snk_desc == NULL || pt_snk_desc->u_text_strm.t_cc.pt_filter == NULL) 
            {
                pt_svc_req->h_gl_plane            = NULL_HANDLE;
                pt_svc_req->t_cc_filter.e_cc_type = SM_CC_TYPE_IGNORE;
            } 
            else 
            {
                pt_svc_req->h_gl_plane  = pt_snk_desc->u_text_strm.t_cc.h_gl_plane;
                pt_svc_req->t_cc_filter = *(pt_snk_desc->u_text_strm.t_cc.pt_filter);
            }
            break;
        case ST_TELETEXT:
            if (pt_snk_desc != NULL && pt_snk_desc->u_text_strm.t_telex.pt_fltr_fct != NULL) 
            {
                pt_svc_req->t_strm_reqs[ST_TELETEXT].pt_strm_fltr_fct = pt_snk_desc->u_text_strm.t_telex.pt_fltr_fct;
                pt_svc_req->t_strm_reqs[ST_TELETEXT].pv_strm_fltr_tag = pt_snk_desc->u_text_strm.t_telex.pv_fltr_tag;
            } 
            else 
            {
                pt_svc_req->t_strm_reqs[ST_TELETEXT].pt_strm_fltr_fct = NULL;
                pt_svc_req->t_strm_reqs[ST_TELETEXT].pv_strm_fltr_tag = NULL;
            }
            pt_svc_req->h_gl_plane = pt_snk_desc->u_text_strm.t_telex.h_gl_plane;
            break;
        case ST_SUBTITLE:
            if (pt_snk_desc != NULL && pt_snk_desc->u_text_strm.t_subtitle.pt_fltr_fct != NULL) 
            {
                pt_svc_req->t_strm_reqs[ST_SUBTITLE].pt_strm_fltr_fct = pt_snk_desc->u_text_strm.t_subtitle.pt_fltr_fct;
                pt_svc_req->t_strm_reqs[ST_SUBTITLE].pv_strm_fltr_tag = pt_snk_desc->u_text_strm.t_subtitle.pv_fltr_tag;
            } 
            else 
            {
                pt_svc_req->t_strm_reqs[ST_SUBTITLE].pt_strm_fltr_fct = NULL;
                pt_svc_req->t_strm_reqs[ST_SUBTITLE].pv_strm_fltr_tag = NULL;
            }
            pt_svc_req->h_gl_plane = pt_snk_desc->u_text_strm.t_subtitle.h_gl_plane;
            break;
        default:
            // NOTICE: for temp sulotion
            pt_svc_req->h_gl_plane = pt_snk_desc->u_text_strm.t_subtitle.h_gl_plane;
            //pt_svc_req->h_gl_plane = NULL_HANDLE;
            break;
        }
    }
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_create_svc_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
static SVC_REQ_T* _mm_hdlr_create_svc_req(
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
    MM_SVC_REQ_T* pt_svc_req = NULL;
    INT32         i4_ret;
    UINT8         ui1_idx;

    /*assume the pending svc req is created in media open stage*/
    /*pt_pending_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_pending_svc_req;
    if ((pt_pending_svc_req!=NULL)&&
       (pt_src_desc->u_info.t_mm.h_media == pt_pending_svc_req->h_media_desc))
    {
        pt_svc_req = pt_pending_svc_req;
        pt_svctx->pt_pending_svc_req = NULL;
    }
    else*/
    {
        pt_svc_req = (MM_SVC_REQ_T*)svctx_allocate_svc_req(pt_hdlr); /* QQQQQQ */
        if (pt_svc_req == NULL) 
        {
            DBG_ERROR((_ERROR_HEADER"create SVC_REQ_T failed!\r\n"));
            return NULL;
        }
    }
    i4_ret = _mm_hdlr_preproc_svc_req(
                    pt_svctx,
                    ui4_stream_mode,
                    pt_src_desc,
                    pt_snk_desc,
                    pf_nfy,
                    pv_nfy_tag,
                    pt_svc_req
                    );
    if (i4_ret != SVCTXR_OK) 
    {
        svctx_free_svc_req((SVC_REQ_T*)pt_svc_req); /* QQQQQQ */
        return NULL;
    }

    pt_svc_req->t_parent.h_svctx                   = h_svctx;
    pt_svc_req->t_parent.pf_svc_nfy                = pf_nfy;
    pt_svc_req->t_parent.pv_svc_nfy_tag            = pv_nfy_tag;
    pt_svc_req->t_parent.ui4_stream_mode           = ui4_stream_mode;
    pt_svc_req->t_parent.pt_hdlr                   = pt_hdlr;
    pt_svc_req->t_parent.i4_speed                  = SESS_NORMAL_SPEED;

    for (ui1_idx = 0; ui1_idx < (UINT8)STRM_TYPE_COUNT; ui1_idx++) 
    {
        pt_svc_req->t_strm_reqs[ui1_idx].pt_hdlr = pt_hdlr;
    }

    return ((SVC_REQ_T*)pt_svc_req);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_cc_comp_count_filter
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
static BOOL _mm_hdlr_cc_comp_count_filter (
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

    if (ui2_idx == ui2_num_recs) 
    {
        return FALSE;
    }

    switch(pt_scdb_rec->u.t_atsc_cc.e_type) 
    {
    case CC_ATSC_LINE_21:
        if (pt_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_line_21.b_det_in_descr == TRUE) 
        {
            (*pui2_num_recs) += 1;
        }
        break;
    case CC_ATSC_DTV:
        if (pt_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_dtv.ui1_caption_svc_num == 0) 
        {
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
 *      _mm_hdlr_strm_comp_id_filter
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
static BOOL _mm_hdlr_strm_comp_id_filter (
                    UINT16                      ui2_num_recs,
                    UINT16                      ui2_idx,
                    const STREAM_COMP_ID_T*     pt_comp_id,
                    const SCDB_REC_T*           pt_scdb_rec,
                    VOID*                       pv_tag
                    )
{
    STREAM_COMP_ID_T* pt_target_comp_id = (STREAM_COMP_ID_T*)pv_tag;


    if (pt_target_comp_id->e_type == pt_comp_id->e_type
            /*&& pt_target_comp_id->pv_stream_tag == pt_comp_id->pv_stream_tag*/) 
    {
        /* tricky: pv_stream_tag is a unique key and assigned by CM.*/
        return TRUE;
    }

    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_get_strm_comp_id_ex
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_get_strm_comp_id_ex(
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
    MM_SVC_REQ_T*    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T         h_scdb             = NULL_HANDLE;
    UINT8            ui1_reloop_count   = 0;
    STREAM_COMP_ID_T t_first_comp_id;
    SCDB_REC_T       t_first_scdb_rec;
    SCDB_REC_T       t_scdb_rec;
    INT32            i4_ret;
    STREAM_COMP_ID_T t_orig_comp_id;

    ASSERT(pt_running_svc_req != NULL, 
           ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if ((pf_filter == NULL) || (pt_comp_id == NULL)) 
    {
        return SVCTXR_INV_ARG;
    }

    t_orig_comp_id = *pt_comp_id;

    h_scdb = player_get_scdb_handle(pt_running_svc_req->pt_player);
    if (x_handle_valid(h_scdb) == FALSE) 
    {
        return SVCTXR_WRONG_STATE;
    }

    while (b_is_searching) 
    {
        b_is_searching = FALSE;
        i4_ret = x_scdb_get_num_recs(
                    h_scdb,
                    e_strm_type,
                    &ui2_num_recs,
                    &ui4_db_version
                    );

        if (i4_ret != SCDBR_OK) 
        {
            DBG_ERROR((_ERROR_HEADER"x_scdb_get_num_recs() failed, return = %d\r\n", i4_ret));
            return SVCTXR_FAIL;
        }

        for (ui2_idx = 0; ui2_idx < ui2_num_recs; ui2_idx++) 
        {
            x_memset(&t_scdb_rec, 0, sizeof(SCDB_REC_T));
            
            i4_ret = x_scdb_get_rec_by_idx(
                            h_scdb,
                            e_strm_type,
                            ui2_idx,
                            pt_comp_id,
                            &t_scdb_rec,
                            &ui4_db_version
                            );
            if (i4_ret == SCDBR_OK) 
            {
                /*
                If the file only has one video with MVC tag, clear the mvc_ext flag 
                */
                if((e_strm_type == ST_VIDEO) && (ui2_num_recs == 1))
                {
                    if ((pt_running_svc_req->pt_player != NULL) &&
                    	(t_scdb_rec.e_rec_type == SCDB_REC_TYPE_VIDEO_MPEG))
                    {
                        if((t_scdb_rec.u.t_video_mpeg.t_mvc_ext.b_mvc_video && 
                           (t_scdb_rec.u.t_video_mpeg.t_mvc_ext.e_mvc_src == MVC_SRC_TYPE_STRM_TAG)))
                        {
                           PLAYBACK_HANDLER_ITEM_SCDB_INFO_T   t_scdb_info;
                           
                            x_memset(&t_scdb_info, 0, sizeof(PLAYBACK_HANDLER_ITEM_SCDB_INFO_T));
                            
                            t_scdb_rec.u.t_video_mpeg.t_mvc_ext.b_mvc_video = FALSE;
                            t_scdb_rec.u.t_video_mpeg.t_mvc_ext.b_mvc_ext_desc = FALSE;
                            
                            t_scdb_info.t_scdb_rec = t_scdb_rec;
                            t_scdb_info.t_stream_id = *pt_comp_id;
                            i4_ret = x_cm_set(pt_running_svc_req->pt_player->h_conn,
                                              PLAYBACK_CTRL_SET_MODIFY_SCDB,
                                              (VOID*) & t_scdb_info);
                        }
                        else if((t_scdb_rec.u.t_video_mpeg.t_mvc_ext.b_mvc_video && 
                           (t_scdb_rec.u.t_video_mpeg.t_mvc_ext.e_mvc_src == MVC_SRC_TYPE_STRM_MVC)))
                        {/*If only one mvc stream, not allowed to play*/
                            continue;
                        }
                    }
                 }
            
                /* intergation from 0 to (ui2_num_recs-1) */
                if (pf_filter(
                            ui2_num_recs,
                            STRM_COMP_FILTER_MAKE_INDEX(ui2_idx, ui1_reloop_count),
                            pt_comp_id,
                            &t_scdb_rec,
                            pv_tag
                            ) == TRUE) 
                {
                    if (pt_scdb_rec != NULL) 
                    {
                        *pt_scdb_rec = t_scdb_rec;
                    }

                    if (pui2_idx != NULL) 
                    {
                        *pui2_idx = ui2_idx;
                    }
                    return SVCTXR_OK;
                }

                if (ui2_idx == 0) 
                {
                    t_first_comp_id  = *pt_comp_id;
                    t_first_scdb_rec = t_scdb_rec;
                }

                if (ui2_idx < (UINT16)(ui2_num_recs-1)) 
                {
                    continue;
                }

                if (pf_filter(
                            ui2_num_recs,
                            STRM_COMP_FILTER_MAKE_INDEX(ui2_num_recs, ui1_reloop_count),
                            &t_first_comp_id,
                            &t_first_scdb_rec,
                            pv_tag
                            ) == TRUE) 
                {
                    if (pt_scdb_rec != NULL) 
                    {
                        *pt_comp_id  = t_first_comp_id;
                        *pt_scdb_rec = t_first_scdb_rec;
                    }

                    if (pui2_idx != NULL) 
                    {
                        *pui2_idx = 0;
                    }
                    return SVCTXR_OK;
                } 
                else 
                {
                    /* one more loop for audio/video*/
                    if ((e_strm_type == ST_VIDEO) || (e_strm_type == ST_AUDIO)) 
                    {
                        ui1_reloop_count++;
                        if (ui1_reloop_count < (UINT8)STRM_COMP_FILTER_MAX_LOOP) 
                        {
                            b_is_searching = TRUE;
                        }
                    }
                }
            } 
            else if (i4_ret == SCDBR_REC_NOT_FOUND) 
            {
                pt_comp_id->pv_stream_tag = t_orig_comp_id.pv_stream_tag;
                /* done all processing, no more record. */
                return SVCTXR_NOT_EXIST;
            } 
            else if (i4_ret == SCDBR_DB_MODIFIED) 
            {
                /* continue the search again */
                if (b_auto_version_update == TRUE) 
                {
                    b_is_searching = TRUE;
                } 
                else 
                {
                    return SVCTXR_DB_MODIFIED;
                }
                break;
            } 
            else 
            {
                pt_comp_id->pv_stream_tag = t_orig_comp_id.pv_stream_tag;
                DBG_ERROR((_ERROR_HEADER"x_scdb_get_rec_by_idx failed - return = %d\r\n", i4_ret));
                return SVCTXR_FAIL;
            }
        }
    }

    pt_comp_id->pv_stream_tag = t_orig_comp_id.pv_stream_tag;
    
    return SVCTXR_NOT_EXIST;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_get_strm_comp_id
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_get_strm_comp_id(
                    SVCTX_T*                     pt_svctx,
                    STREAM_TYPE_T                e_strm_type,
                    BOOL                         b_auto_version_update,
                    x_svctx_strm_comp_filter_fct pf_filter,
                    VOID*                        pv_tag,
                    STREAM_COMP_ID_T*            pt_comp_id,
                    SCDB_REC_T*                  pt_scdb_rec
                    )
{
    return _mm_hdlr_get_strm_comp_id_ex(
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
 *      _mm_hdlr_get_using_scdb_info_ex
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_get_using_scdb_info_ex(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    STREAM_COMP_ID_T*           pt_comp_id,
                    SCDB_REC_T*                 pt_scdb_rec,
                    UINT16*                     pui2_idx
                    )
{
    INT32         i4_ret;
    STRM_REQ_T*   pt_strm_req;
    MM_SVC_REQ_T* pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T      h_scdb             = NULL_HANDLE;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (mm_hdlr_is_supported_strm_type(e_strm_type) == FALSE) 
    {
        return SVCTXR_NOT_SUPPORT;
    }

    h_scdb      = player_get_scdb_handle(pt_running_svc_req->pt_player);
    if ((e_strm_type >= STRM_TYPE_COUNT) || (e_strm_type == ST_UNKNOWN))
    {
        return SVCTXR_FAIL;
    }
    pt_strm_req = &(pt_running_svc_req->t_strm_reqs[e_strm_type]);

    /* marked for CR DTV00209034 */
    /*if (x_handle_valid(pt_strm_req->h_stream) == FALSE) 
    {
        return SVCTXR_NOT_EXIST;
    }*/

    if (pt_strm_req->e_req_type == STRM_REQ_SCDB) 
    {
        if (pt_strm_req->u.t_scdb.t_comp_id.e_type != e_strm_type
                || pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag == NULL) 
        {
            return SVCTXR_NOT_EXIST;
        }

        i4_ret = x_scdb_get_rec (
                            h_scdb,
                            &(pt_strm_req->u.t_scdb.t_comp_id),
                            &(pt_strm_req->u.t_scdb.t_rec)
                            );
        if (i4_ret != SCDBR_OK) 
        {
            return SVCTXR_FAIL;
        }

        if (pt_comp_id != NULL) 
        {
            *pt_comp_id = pt_strm_req->u.t_scdb.t_comp_id;
        }

        if (pt_scdb_rec != NULL) 
        {
            *pt_scdb_rec = pt_strm_req->u.t_scdb.t_rec;
        }

        if (pui2_idx != NULL) 
        {
            STREAM_COMP_ID_T t_comp_id;
            i4_ret = _mm_hdlr_get_strm_comp_id_ex(
                            pt_svctx,
                            e_strm_type,
                            TRUE,
                            _mm_hdlr_strm_comp_id_filter,
                            (VOID*)(&(pt_strm_req->u.t_scdb.t_comp_id)),
                            &t_comp_id,
                            NULL,
                            pui2_idx
                            );
        }
    }
    else
    {
        return SVCTXR_NOT_EXIST;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_get_using_scdb_info
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_get_using_scdb_info(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    STREAM_COMP_ID_T*           pt_comp_id,
                    SCDB_REC_T*                 pt_scdb_rec
                    )
{
    return _mm_hdlr_get_using_scdb_info_ex(
                    pt_svctx,
                    e_strm_type,
                    pt_comp_id,
                    pt_scdb_rec,
                    NULL
                    );
}
/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_set_sess_attr
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
static INT32 _mm_hdlr_set_sess_attr(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    SM_SET_TYPE_T               e_set_type,
                    VOID*                       pv_set_info,
                    SIZE_T                      z_set_info_size
                    )
{
    MM_SVC_REQ_T* pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T      h_sess;
    INT32         i4_ret;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    h_sess = player_get_sess_handle(pt_running_svc_req->pt_player);
    
    if (x_handle_valid(h_sess) == FALSE) 
    {
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
 * Name: _mm_hdlr_set_strm_attr
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
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_set_strm_attr(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    SM_SET_TYPE_T               e_set_type,
                    VOID*                       pv_set_info,
                    SIZE_T                      z_set_info_size
                    )
{
    MM_SVC_REQ_T* pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    INT32         i4_ret;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if ((mm_hdlr_is_supported_strm_type(e_strm_type) == FALSE) ||
        (e_strm_type == ST_UNKNOWN) ||
        (e_strm_type >= STRM_TYPE_COUNT)) 
    {
        return SVCTXR_NOT_SUPPORT;
    }

    i4_ret = svctx_set_stream_attr(
                    pt_svctx,
                    pt_running_svc_req->t_strm_reqs[e_strm_type].h_stream,
                    e_set_type,
                    pv_set_info,
                    z_set_info_size
                    );
    if (i4_ret != SVCTXR_OK) 
    {
        return i4_ret;
    }
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_get_strm_handle
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *-----------------------------------------------------------------------------*/
static BOOL _mm_hdlr_get_strm_handle(
                    SVC_REQ_T*                  pt_svc_req,
                    STREAM_TYPE_T               e_stream_type,
                    HANDLE_T*                   ph_stream
                    )
{
    MM_SVC_REQ_T* pt_mm_svc_req = (MM_SVC_REQ_T*)pt_svc_req;

    if (pt_mm_svc_req == NULL) 
    {
        return FALSE;
    }

    if (mm_hdlr_is_supported_strm_type(e_stream_type) == FALSE) 
    {
        return FALSE;
    }

    if ((e_stream_type == ST_UNKNOWN) || (e_stream_type >= STRM_TYPE_COUNT))
    {
        return FALSE;
    }

    *ph_stream = pt_mm_svc_req->t_strm_reqs[e_stream_type].h_stream;
    return TRUE;
}
#ifdef TIME_SHIFT_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_get_current_event_info_len
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *-----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_get_current_event_info_len (
                    SVCTX_T*                        pt_svctx,
                    SVC_REQ_T*                      pt_svc_req,
#ifdef MW_EDB_SUPPORT
                    EDB_KEY_TYPE_T                    e_key_type,
#else                    
                    EVCTX_KEY_TYPE_T                e_key_type,
#endif    
                    VOID*                           pv_key_info,
                    SIZE_T*                         pz_event_info_len
                    )
{
    MM_SVC_REQ_T*     pt_running_svc_req = (MM_SVC_REQ_T*)pt_svc_req;
    INT32             i4_ret;
    MEDIA_DESCRIBT_T* pt_media_desc;

    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc,&pt_media_desc);    
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;   
    }

    if (pt_svctx->e_block_rule == SVCTX_BLOCK_RULE_INP_CH_PROG_SIGNAL_LOSS
#ifndef MW_EDB_SUPPORT        
            && e_key_type == EVCTX_KEY_TYPE_RATING
#else
            && e_key_type == EDB_KEY_TYPE_RATING_LIST_BY_ID
#endif
            && pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) 
   {
        /* get rating size from crnt_evn's buffer */
        if (pz_event_info_len == NULL) 
        {
            return SVCTXR_INV_ARG;
        }

        *pz_event_info_len = crnt_evn_get_rating_size(pt_running_svc_req->pt_crnt_event);
    } 
    else 
    {
        if (svctx_with_event_capability(
                    pt_svctx,
                    pt_running_svc_req->e_crnt_tuner_type,
                    MAKE_BIT_MASK_32(e_key_type)) == FALSE) 
        {
            return SVCTXR_EVN_INFO_NOT_SUPPORT;
        }

        i4_ret = rec_util_get_event_info_len(pt_media_desc->h_rec_util,
                                             pt_media_desc->ui4_rec_mntr_lba_last,
                                             e_key_type,
                                             pz_event_info_len);
        if (i4_ret != REC_UTILR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"rec_util_get_event_info_len(key=%d,lba=0x%.8X) failed, ret=%d\r\n", 
                       e_key_type, 
                       pt_media_desc->ui4_rec_mntr_lba_last, 
                       i4_ret));
            return SVCTXR_FAIL;
        }
        DBG_INFO((_INFO_HEADER"rec_util_get_event_info_len(key=%d,lba=0x%.8X,len=%u)\r\n", 
                  e_key_type,
                  pt_media_desc->ui4_rec_mntr_lba_last, 
                  (UINT32)(*pz_event_info_len)));
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_get_current_event_info
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *-----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_get_current_event_info (
                    SVCTX_T*                        pt_svctx,
                    SVC_REQ_T*                      pt_svc_req,
#ifdef MW_EDB_SUPPORT
                    EDB_KEY_TYPE_T                    e_key_type,
#else                    
                    EVCTX_KEY_TYPE_T                e_key_type,
#endif    
                    VOID*                           pv_key_info,
                    SIZE_T*                         pz_event_info_len,
                    VOID*                           pv_event_info
                    )
{
    MM_SVC_REQ_T*     pt_running_svc_req = (MM_SVC_REQ_T*)pt_svc_req;
    SIZE_T            z_orig_len;
    INT32             i4_ret;
    VOID*             pv_crnt_evn_rating = NULL;
    MEDIA_DESCRIBT_T* pt_media_desc;

    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc,&pt_media_desc);    
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;   
    }

    if (pt_svctx->e_block_rule == SVCTX_BLOCK_RULE_INP_CH_PROG_SIGNAL_LOSS
#ifndef MW_EDB_SUPPORT        
              && e_key_type == EVCTX_KEY_TYPE_RATING
#else
              && e_key_type == EDB_KEY_TYPE_RATING_LIST_BY_ID
#endif

            && pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) 
    {
        /* get rating from crnt_evn's buffer */
        if (pz_event_info_len == NULL || pv_event_info == NULL) 
        {
            return SVCTXR_INV_ARG;
        }

        z_orig_len = crnt_evn_get_rating_size(pt_running_svc_req->pt_crnt_event);
        
        if ((z_orig_len == 0) || crnt_evn_get_rating(pt_running_svc_req->pt_crnt_event) == NULL) 
        {
            return SVCTXR_EVN_INFO_NOT_FOUND;
        }

        if (z_orig_len > (*pz_event_info_len)) 
        {
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
    } 
    else 
    {
        if (svctx_with_event_capability(
                    pt_svctx,
                    pt_running_svc_req->e_crnt_tuner_type,
                    MAKE_BIT_MASK_32(e_key_type)) == FALSE) 
        {
            return SVCTXR_EVN_INFO_NOT_SUPPORT;
        }

        i4_ret = rec_util_get_event_info(pt_media_desc->h_rec_util,
                                         pt_media_desc->ui4_rec_mntr_lba_last,
                                         e_key_type,
                                         pz_event_info_len,
                                         pv_event_info);
        if (i4_ret != REC_UTILR_OK) 
        {
            pz_event_info_len = 0;
            pv_event_info     = NULL;
            DBG_ERROR((_ERROR_HEADER"rec_util_get_event_info(key=%d) failed, ret=%d\r\n", 
                       e_key_type, 
                       i4_ret));
            return SVCTXR_FAIL;
        }
        DBG_INFO((_INFO_HEADER"rec_util_get_event_info(key=%d)\r\n", e_key_type));
    }

    return SVCTXR_OK;
}
#endif /* TIME_SHIFT_SUPPORT */
/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_is_handler
 *
 * Description: This API is used to check the handler is response for this request
 * Inputs:  e_src_type         The source type
 *
 * Outputs: -
 *
 * Returns: TRUE               It is handler.
 *          FALSE              It is not handler
 *---------------------------------------------------------------------------*/
static BOOL _mm_hdlr_is_handler(
                    SRC_TYPE_T                      e_src_type
                    )
{
    switch(e_src_type) 
    {
    case SRC_TYPE_MEDIA_STORGE:
    case SRC_TYPE_MEDIA_MEM_CLIP:
    case SRC_TYPE_MEDIA_TIME_SHIFT:
    case SRC_TYPE_MEDIA_PVR:
    case SRC_TYPE_MEDIA_NETWORK:
        return TRUE;
    default:
        break;
    }

    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_get_par
 *
 * Description: This API is used to check the handler is response for this request
 * Inputs:  e_src_type         The source type
 *
 * Outputs: -
 *
 * Returns: TRUE               It is handler.
 *          FALSE              It is not handler
 *-----------------------------------------------------------------------------*/
static VOID _mm_hdlr_par_sm2svctx(VSH_SRC_ASPECT_RATIO_T e_src_par,
                                  UINT8* pui_par_x,
                                  UINT8* pui_par_y,
                                  BOOL*  b_is_src )
{
    *b_is_src = FALSE;
    switch (e_src_par)
    {
        case VSH_SRC_ASPECT_RATIO_PXL_10_11:
            *pui_par_x = 10;
            *pui_par_y = 11;
            break;       
        case VSH_SRC_ASPECT_RATIO_PXL_40_33:
            *pui_par_x = 40;
            *pui_par_y = 33;
            break;       
        case VSH_SRC_ASPECT_RATIO_PXL_16_11:
            *pui_par_x = 16;
            *pui_par_y = 11;
            break;       
        case VSH_SRC_ASPECT_RATIO_PXL_12_11:
            *pui_par_x = 12;
            *pui_par_y = 11;
            break;        
        case VSH_SRC_ASPECT_RATIO_PXL_3_2:
            *pui_par_x = 3;
            *pui_par_y = 2;
            break;       
        case VSH_SRC_ASPECT_RATIO_PXL_1_1:
            *pui_par_x = 1;
            *pui_par_y = 1;
            break;
        case VSH_SRC_ASPECT_RATIO_16_9:
            *pui_par_x = 16;
            *pui_par_y = 9;
            *b_is_src = TRUE;
             break;
        case VSH_SRC_ASPECT_RATIO_4_3:
            *pui_par_x = 4;
            *pui_par_y = 3;
            *b_is_src = TRUE;        
            break;
        default:
            *pui_par_x = 1;
            *pui_par_y = 1;
             break;            
    }
}

/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_get_sess_attr
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
static INT32 _mm_hdlr_get_sess_attr(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    SM_GET_TYPE_T               e_get_type,
                    VOID*                       pv_get_info,
                    SIZE_T*                     pui4_get_info_size
                    )
{
    MM_SVC_REQ_T* pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T      h_sess;
    INT32         i4_ret;

    ASSERT(pt_running_svc_req != NULL, 
           ("%s(), pt_running_svc_req is NULL\r\n", 
           __func__));

    h_sess = player_get_sess_handle(pt_running_svc_req->pt_player);

    if (x_handle_valid(h_sess) == FALSE) 
    {
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
 * Name: _mm_hdlr_get_strm_attr
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
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_get_strm_attr(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    SM_GET_TYPE_T               e_get_type,
                    VOID*                       pv_get_info,
                    SIZE_T*                     pui4_get_info_size
                    )
{
    MM_SVC_REQ_T* pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    INT32         i4_ret             = SVCTXR_OK;

    ASSERT(pt_running_svc_req != NULL, 
          ("%s(), pt_running_svc_req is NULL\r\n", 
          __func__));

    if (mm_hdlr_is_supported_strm_type(e_strm_type) == FALSE) 
    {
        return SVCTXR_NOT_SUPPORT;
    }
    if (e_strm_type >= STRM_TYPE_COUNT || e_strm_type == ST_UNKNOWN)
    {
        return SVCTXR_INV_ARG;
    }
    if (x_handle_valid(pt_running_svc_req->t_strm_reqs[e_strm_type].h_stream) == FALSE) 
    {
        return SVCTXR_WRONG_STATE;
    }

    i4_ret = x_sm_get(
                    pt_running_svc_req->t_strm_reqs[e_strm_type].h_stream,
                    e_get_type,
                    pv_get_info,
                    pui4_get_info_size
                    );
    return ((i4_ret == SMR_OK) ? SVCTXR_OK : SVCTXR_FAIL);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_create_generic_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_create_generic_strm_req(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_svctx,
                    STREAM_COMP_ID_T*           pt_comp_id,
                    x_svctx_select_nfy_fct      pf_nfy,
                    VOID*                       pv_nfy_tag,
                    STRM_REQ_T**                ppt_strm_req
                    )
{
    STRM_REQ_T*   pt_strm_req        = NULL;
    MM_SVC_REQ_T* pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T      h_scdb             = NULL_HANDLE;
    SCDB_REC_T    t_scdb_rec;
    INT32         i4_ret;

    ASSERT(pt_running_svc_req != NULL, 
           ("%s(), pt_running_svc_req is NULL\r\n", 
           __func__));

    if (ppt_strm_req == NULL) 
    {
        return SVCTXR_INV_ARG;
    }

    if (pt_comp_id == NULL) 
    {
        return SVCTXR_INV_ARG;
    }

    if (mm_hdlr_is_supported_strm_type(pt_comp_id->e_type) == FALSE) 
    {
        return SVCTXR_NOT_SUPPORT;
    }

    ppt_strm_req[STRM_IDX_NORMAL] = NULL;

    pt_strm_req = svctx_allocate_strm_req(pt_svctx->pt_running_svc_req->pt_hdlr);
    if (pt_strm_req == NULL) 
    {
        DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
        return SVCTXR_NO_RESOURCE;
    }

    pt_strm_req->h_svctx            = h_svctx;
    pt_strm_req->pf_strm_nfy        = pf_nfy;
    pt_strm_req->pv_strm_nfy_tag    = pv_nfy_tag;
    pt_strm_req->e_req_type         = STRM_REQ_SCDB;
    pt_strm_req->u.t_scdb.t_comp_id = *pt_comp_id;
    pt_strm_req->b_aux_audio        = FALSE;

    if (pf_nfy == NULL)
    {
        pt_strm_req->pf_strm_nfy     = pt_running_svc_req->t_parent.pf_svc_nfy;
        pt_strm_req->pv_strm_nfy_tag = pt_running_svc_req->t_parent.pv_svc_nfy_tag;
    }
    h_scdb = player_get_scdb_handle(pt_running_svc_req->pt_player);

    i4_ret = x_scdb_get_rec (
                    h_scdb,
                    &(pt_strm_req->u.t_scdb.t_comp_id),
                    &t_scdb_rec
                    );
    if (i4_ret == SCDBR_REC_NOT_FOUND) 
    {
        svctx_free_strm_req(pt_strm_req);
        return SVCTXR_NO_RECORD;
    }

    if (i4_ret != SCDBR_OK) 
    {
        DBG_ERROR((_ERROR_HEADER"failed to x_scdb_get_rec, ret=%d\r\n", i4_ret));
        svctx_free_strm_req(pt_strm_req);
        return SVCTXR_FAIL;
    }

    pt_strm_req->u.t_scdb.t_rec = t_scdb_rec;
    ppt_strm_req[0] = pt_strm_req;
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_create_generic_subtitle_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_create_generic_subtitle_strm_req(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_svctx,
                    HANDLE_T                    h_gl_plane,
                    STREAM_COMP_ID_T*           pt_comp_id,
                    x_svctx_select_nfy_fct      pf_nfy,
                    VOID*                       pv_nfy_tag,
                    STRM_REQ_T**                ppt_strm_req
                    )
{
    STRM_REQ_T*   pt_strm_req        = NULL;
    MM_SVC_REQ_T* pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    INT32         i4_ret;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (ppt_strm_req == NULL) 
    {
        return SVCTXR_INV_ARG;
    }

    if (x_handle_valid(h_gl_plane) == FALSE || pt_comp_id == NULL) 
    {
        return SVCTXR_INV_ARG;
    }

    if (mm_hdlr_is_supported_strm_type(ST_SUBTITLE) == FALSE) 
    {
        return SVCTXR_NOT_SUPPORT;
    }

    pt_strm_req = svctx_allocate_strm_req(pt_running_svc_req->t_parent.pt_hdlr);
    if (pt_strm_req == NULL) 
    {
        DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
        return SVCTXR_NO_RESOURCE;
    }
    pt_strm_req->h_svctx            = h_svctx;
    pt_strm_req->pf_strm_nfy        = pf_nfy;
    pt_strm_req->pv_strm_nfy_tag    = pv_nfy_tag;
    pt_strm_req->h_gl_plane         = h_gl_plane;
    pt_strm_req->e_req_type         = STRM_REQ_SCDB;
    pt_strm_req->u.t_scdb.t_comp_id = *pt_comp_id;

    i4_ret = x_scdb_get_rec (
                    player_get_scdb_handle(pt_running_svc_req->pt_player),
                    &(pt_strm_req->u.t_scdb.t_comp_id),
                    &(pt_strm_req->u.t_scdb.t_rec)
                    );
    if (i4_ret == SCDBR_REC_NOT_FOUND) 
    {
        svctx_free_strm_req(pt_strm_req);
        return SVCTXR_NO_RECORD;
    }

    if (i4_ret != SCDBR_OK) 
    {
        svctx_free_strm_req(pt_strm_req);
        return SVCTXR_FAIL;
    }

    *ppt_strm_req = pt_strm_req;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_create_cc_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_create_cc_strm_req(
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

    if (x_handle_valid(h_gl_plane) == FALSE || pt_cc_filter == NULL) 
    {
        return SVCTXR_INV_ARG;
    }

    if (mm_hdlr_is_supported_strm_type(ST_CLOSED_CAPTION) == FALSE) 
    {
        return SVCTXR_NOT_SUPPORT;
    }

    pt_strm_req = svctx_allocate_strm_req(pt_svctx->pt_running_svc_req->pt_hdlr);
    if (pt_strm_req == NULL) 
    {
        DBG_ERROR((_ERROR_HEADER"create STRM_REQ_T failed!\r\n"));
        return SVCTXR_NO_RESOURCE;
    }

    pt_strm_req->h_svctx         = h_svctx;
    pt_strm_req->pf_strm_nfy     = pf_nfy;
    pt_strm_req->pv_strm_nfy_tag = pv_nfy_tag;
    pt_strm_req->t_cc_filter     = *pt_cc_filter;
    pt_strm_req->h_gl_plane      = h_gl_plane;
    pt_strm_req->e_req_type      = STRM_REQ_SCDB;

    i4_ret = _mm_hdlr_get_strm_comp_id(
                    pt_svctx,
                    ST_CLOSED_CAPTION,
                    TRUE,
                    svctx_strm_comp_cc_filter,
                    (VOID*)pt_cc_filter,
                    &(pt_strm_req->u.t_scdb.t_comp_id),
                    &(pt_strm_req->u.t_scdb.t_rec)
                    );
    /* if can't find any stream comp id, then force to set it by cc filter */
    if (i4_ret != SVCTXR_OK) 
    {
        pt_strm_req->u.t_scdb.t_comp_id.e_type        = ST_CLOSED_CAPTION;
        pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag = NULL;
    }

    *ppt_strm_req = pt_strm_req;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_create_strm_req
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_create_strm_req(
                    SVCTX_T*                    pt_svctx,
                    HANDLE_T                    h_svctx,
                    VOID*                       pv_info,
                    x_svctx_select_nfy_fct      pf_nfy,
                    VOID*                       pv_nfy_tag,
                    STRM_REQ_T**                ppt_strm_req
                    )
{
    CH_STRM_SEL_INFO_T* pt_info = (CH_STRM_SEL_INFO_T*) pv_info;
    INT32 i4_ret = SVCTXR_OK;

    if (pt_info == NULL) 
    {
        return SVCTXR_INV_ARG;
    }

    switch (pt_info->e_type) {
    case CH_STRM_SEL_INFO_TYPE_AUDIO:
        /*i4_ret = _ch_hdlr_create_audio_strm_req(
                    pt_svctx,
                    h_svctx,
                    pt_info->u.t_audio.pt_lang,
                    pf_nfy,
                    pv_nfy_tag,
                    ppt_strm_req
                    );*/
       break;
    case CH_STRM_SEL_INFO_TYPE_CC:
        i4_ret = _mm_hdlr_create_cc_strm_req(
                    pt_svctx,
                    h_svctx,
                    pt_info->u.t_cc.h_gl_plane,
                    pt_info->u.t_cc.pt_cc_filter,
                    pf_nfy,
                    pv_nfy_tag,
                    ppt_strm_req
                    );
        break;
    case CH_STRM_SEL_INFO_TYPE_GENERIC_SUBTITLE:
        i4_ret = _mm_hdlr_create_generic_subtitle_strm_req(
                    pt_svctx,
                    h_svctx,
                    pt_info->u.t_subtitle_generic.h_gl_plane,
                    pt_info->u.t_subtitle_generic.pt_comp_id,
                    pf_nfy,
                    pv_nfy_tag,
                    ppt_strm_req
                    );
        break;
    case CH_STRM_SEL_INFO_TYPE_GENERIC:
        i4_ret = _mm_hdlr_create_generic_strm_req(
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
#ifdef TIME_SHIFT_SUPPORT
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_reload_cc_strm_as_presenting
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static VOID _mm_hdlr_reload_cc_strm_as_presenting (
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_new_svc_req
                    )
{
    STRM_REQ_T*      pt_strm_req = &(pt_new_svc_req->t_strm_reqs[ST_CLOSED_CAPTION]);
    STREAM_COMP_ID_T t_comp_id;
    INT32 i4_ret;

    pt_strm_req->e_req_type                = STRM_REQ_SCDB;
    pt_strm_req->u.t_scdb.t_comp_id.e_type = ST_CLOSED_CAPTION;
    i4_ret = mm_hdlr_open_strm(
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

    i4_ret = _mm_hdlr_get_strm_comp_id(
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
#endif
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_deinit
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static VOID _mm_hdlr_deinit(
                    VOID
                    )
{
    mm_svc_req_deinit();
    mm_strm_req_deinit();
    crnt_evn_deinit();
    rating_chker_deinit();

    b_g_init = FALSE;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_get
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_get(
                    SVCTX_T*                    pt_svctx,
                    SVCTX_GET_TYPE_T            e_get_type,
                    VOID*                       pv_get_info,
                    SIZE_T*                     pz_get_info_size
                    )
{
    MM_SVC_REQ_T*                       pt_running_svc_req;
    MM_SVC_UOP_HANDLER_FCT_TBL_T        t_fct_tbl;
    MEDIA_DESCRIBT_T*                   pt_media_desc;
    STRM_REQ_T*                         pt_video_strm_req;
    STRM_REQ_T*                         pt_audio_strm_req;
    MM_SVC_TIME_CODE_INFO_T*            pt_tm_code;
    MM_SVC_POS_INFO_T*                  pt_pos_info;
    MM_SVC_DUR_INFO_T*                  pt_dur_code;
    MM_SVC_PTS_INFO_T*                  pt_pts_info;
    MM_SVC_DUR_INFO_T                   t_dur_code;
    MM_VIDEO_INFO_T*                    pt_video_info;
    VSH_PICTURE_INFO_T                  t_vsh_pic_info;
    MINFO_INFO_T                        t_minfo_rec;
    MM_SPEED_TYPE_T                     e_speed;
    SIZE_T                              z_info_len = 0;
    INT32                               i4_ret = SVCTXR_OK;
    STC_T                               t_stc_value = 0;
    MEDIA_FORMAT_T*                     pt_media_fmt;
    UINT8*                              pui1_curr_progress;

    VSH_SRC_RESOLUTION_INFO_T           t_vsh_res;

#ifdef TIME_SHIFT_SUPPORT
    MM_TIMESHIFT_RANGE_INFO_T*          pt_ts_range_info;
    PVR_TICK_INDEX_T                    t_tick_index = {0};
    UINT32                              ui4_lba = 0;
    UINT32                              ui4_tick = 0;
#endif /* TIME_SHIFT_SUPPORT */
    PLAYER_T*                           pt_player;
    BOOL                                b_get_pos_as_time = FALSE;
    BOOL                                b_get_pts_as_time = FALSE;

    if(pt_svctx == NULL || pv_get_info == NULL || pz_get_info_size == NULL)
    {
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req  = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    if(pt_running_svc_req == NULL)
    {
        return SVCTXR_INV_ARG;
    }
    if (e_get_type == SVCTX_COMMON_GET_TYPE_BLOCK_COND)
    {
        if (((*pz_get_info_size) != sizeof(SVC_BLOCK_COND_T)) || (pv_get_info == NULL)) 
        {
            return SVCTXR_INV_GET_INFO;
        }
    
        if (pt_running_svc_req != NULL) 
        {
            *((SVC_BLOCK_COND_T*)pv_get_info) = 
                rating_chker_get_block_cond(pt_running_svc_req->pt_rating_chker);
        } 
        else 
        {
            *((SVC_BLOCK_COND_T*)pv_get_info) = SVC_BLOCK_COND_PASSED;
        }
        return SVCTXR_OK;
    }

    t_fct_tbl = pt_running_svc_req->t_uop_fct_tbl;

    if (t_fct_tbl.pf_open == NULL)
    {
        return SVCTXR_NOT_SUPPORT;
    }
    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc,&pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }

    pt_video_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]);
    pt_audio_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_AUDIO]);

    x_memset(&t_minfo_rec, 0, sizeof(MINFO_INFO_T));

    switch (e_get_type) {
    case SVCTX_MM_GET_CURRENT_TICK:
        if ((*pz_get_info_size) != sizeof(UINT32))
        {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        i4_ret = mm_hdlr_tshift_get_curr_tick (pt_svctx, (UINT32*)pv_get_info);

        break;

    case SVCTX_MM_GET_TYPE_POSIT_INFO:
        pt_video_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]);
        pt_pos_info = (MM_SVC_POS_INFO_T*)pv_get_info;
        if (x_handle_valid(pt_video_strm_req->h_stream) == TRUE)
        {
            z_info_len = sizeof(VSH_PICTURE_INFO_T);
            t_vsh_pic_info.e_type = VSH_PIC_INFO_TYPE_PICTURE;
            i4_ret = x_sm_get(pt_video_strm_req->h_stream,
                            SM_VSH_GET_TYPE_PICTURE_INFO,
                            (VOID*)&t_vsh_pic_info,
                            &z_info_len);
            if (i4_ret != SMR_OK)
            {
                return SVCTXR_FAIL;
            }
            if( pt_pos_info->t_pos_type != MM_SVC_POS_TYPE_TT_OFFSET&&
                pt_pos_info->t_pos_type != MM_SVC_POS_TYPE_CH_OFFSET&&
                pt_pos_info->t_pos_type != MM_SVC_POS_TYPE_ALL_OFFSET)
            {
                return SVCTXR_FAIL;
            }
            pt_pos_info->ui8_pos_val = t_vsh_pic_info.ui8_offset;
        }
        else if(MM_HDLR_AUDIO_ES_WITHOUT_KEY(pt_media_desc) || 
            x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream))
        {
            SM_SESS_GNRC_POST_INFO_T t_pos_info;
            z_info_len = sizeof(SM_SESS_GNRC_POST_INFO_T);
            t_pos_info.e_type = _SM_SESS_GNRC_POST_INFO_MEDIA_BTE_OFST;
            t_pos_info.ui8_pos_info = 0;
            if(pt_running_svc_req != NULL)
            {
                pt_player = pt_running_svc_req->pt_player;            
            }
            else
            {
                return SVCTXR_FAIL;
            }
            i4_ret = x_sm_get(pt_player->h_sess,
                            SM_SESS_GNRC_GET_TYPE_POST_INFO,
                            (VOID*)&t_pos_info,
                            &z_info_len);
            if (i4_ret != SMR_OK)
            {
                return SVCTXR_FAIL;
            }
            pt_pos_info->ui8_pos_val = t_pos_info.ui8_pos_info;
        }
        else
        {
            return SVCTXR_FAIL;
        }
        break;
#if (MM_AB_REPEAT_SUPPORT || defined(MM_LAST_MEMORY_SUPPORT))
    case SVCTX_MM_GET_TYPE_POS_PTS:
        pt_pos_info = (MM_SVC_POS_INFO_T*)pv_get_info;
        e_speed = pt_media_desc->e_speed;
        if ((x_handle_valid(pt_video_strm_req->h_stream) == TRUE &&
             !(pt_media_desc->ui1_stream_eos_status & (ST_VIDEO+1))) ||
             (x_handle_valid(pt_audio_strm_req->h_stream) == TRUE &&
                  !(pt_media_desc->ui1_stream_eos_status & (ST_AUDIO+1)))
            )
        {
#ifndef MM_LAST_MEMORY_SUPPORT        
            SM_SESS_GNRC_UOP_CAP_INFO_T         t_uop_cap_info;
            t_uop_cap_info.e_uop = _SM_SESS_GNRC_UOP_SEEK;
            t_uop_cap_info.u.t_speed = pt_media_desc->e_speed;
            z_info_len = sizeof(SM_SESS_GNRC_UOP_CAP_INFO_T);
            i4_ret = x_sm_get(pt_running_svc_req->pt_player->h_sess,
                              SM_SESS_GNRC_GET_TYPE_UOP_CAP,
                              (VOID*)&t_uop_cap_info,
                              &z_info_len);
            if((i4_ret != SVCTXR_OK) || !t_uop_cap_info.b_is_allowed)
            {
                return SVCTXR_FAIL;
            }
#endif           
            if(x_handle_valid(pt_video_strm_req->h_stream) == TRUE) 
            {
                SM_SESS_GNRC_POST_INFO_T t_pos_info;
                t_pos_info.e_type = _SM_SESS_GNRC_POST_INFO_MEDIA_RENDER_OFST;
                z_info_len = sizeof(SM_SESS_GNRC_POST_INFO_T);
                if(pt_running_svc_req != NULL)
                {
                    pt_player = pt_running_svc_req->pt_player;            
                }
                else
                {
                    return SVCTXR_FAIL;
                }
                i4_ret = x_sm_get(pt_player->h_sess,
                                SM_SESS_GNRC_GET_TYPE_POST_INFO,
                                &t_pos_info,
                                &z_info_len);
                if (i4_ret != SMR_OK)
                {
                    return SVCTXR_FAIL;
                }
                pt_pos_info->u.render_info.ui2_decoding_order 
                    = t_pos_info.u.render_info.ui2_decoding_order;
                pt_pos_info->u.render_info.ui8_frame_position 
                    = t_pos_info.u.render_info.ui8_frame_position;
                pt_pos_info->u.render_info.ui8_frame_position_disp 
                    = t_pos_info.u.render_info.ui8_frame_position_disp; 
                pt_pos_info->u.render_info.ui8_i_frame_position 
                    = t_pos_info.u.render_info.ui8_i_frame_position;
                pt_pos_info->u.render_info.ui8_i_pts_info
                    = t_pos_info.u.render_info.ui8_i_pts_info;
                pt_pos_info->u.render_info.ui8_pts_info
                    = t_pos_info.u.render_info.ui8_pts_info;
                pt_pos_info->u.render_info.i4_temporal_reference
                    = t_pos_info.u.render_info.i4_temporal_reference;
                pt_pos_info->u.render_info.ui8_aud_pts_info
                    = t_pos_info.u.render_info.ui8_aud_pts_info;
                pt_pos_info->u.render_info.ui8_aud_frame_position
                    = t_pos_info.u.render_info.ui8_aud_frame_position;
                pt_pos_info->u.render_info.ui8_stc
                    = t_pos_info.u.render_info.ui8_stc;
                pt_pos_info->u.render_info.ui4_timestap
                    = t_pos_info.u.render_info.ui4_timestap;
            }
        }
        else
        {
             return SVCTXR_OK;
        }
        break;
#endif
    case SVCTX_MM_GET_TYPE_PTS:
    {
        UINT64  ui8_cur_pts;
        pt_pts_info = (MM_SVC_PTS_INFO_T*)pv_get_info;

        if (x_handle_valid(pt_video_strm_req->h_stream))
        {
            z_info_len = sizeof(VSH_PICTURE_INFO_T);
            t_vsh_pic_info.e_type = VSH_PIC_INFO_TYPE_PICTURE;
            i4_ret = x_sm_get(pt_video_strm_req->h_stream,
                            SM_VSH_GET_TYPE_PICTURE_INFO,
                            (VOID*)&t_vsh_pic_info,
                            &z_info_len);
            DBG_INFO(("\n[MM_HDLR_API]SM_VSH_GET_TYPE_PICTURE_INFO ret=%d\n",i4_ret));
            if (i4_ret != SMR_OK)
            {
                return SVCTXR_FAIL;
            }
            pt_pts_info->ui8_vid_pts = t_vsh_pic_info.u.ui8_pts;                
        }

        if (x_handle_valid(pt_audio_strm_req->h_stream))
        {
            z_info_len = sizeof(ui8_cur_pts);
            i4_ret = x_sm_get(
                    pt_audio_strm_req->h_stream,
                    SM_ASH_GET_TYPE_AUDIO_PTS,
                    (VOID*)&ui8_cur_pts,
                    &z_info_len
                    );
            if (i4_ret != SMR_OK)
            {
                return SVCTXR_FAIL;
            }
            pt_pts_info->ui8_aud_pts = ui8_cur_pts;
        }
    }
        break;
    case SVCTX_MM_GET_TYPE_TIME_CODE:
    case SVCTX_MM_GET_TYPE_TIME_CODE_MS:
        if (pt_media_desc->b_is_seeking)
        {
            return SVCTXR_FAIL;
        }
        if(pt_media_desc->b_is_buff_underflow)
        {
            return SVCTXR_FAIL;
        }
        pt_tm_code =  (MM_SVC_TIME_CODE_INFO_T*)pv_get_info;
        i4_ret = t_fct_tbl.pf_get_tm(pt_media_desc,pt_tm_code);
        if (i4_ret != SVCTXR_OK && i4_ret != SVCTXR_GET_POS_AS_TIME)
        {
            return i4_ret;
        }

        if(i4_ret == SVCTXR_GET_POS_AS_TIME)
        {
            b_get_pos_as_time = TRUE;
        }

        if(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_AUDIO_ES)
        {
            if(pt_media_desc->e_speed >= MM_SPEED_TYPE_REWIND_32X && 
               pt_media_desc->e_speed <= MM_SPEED_TYPE_REWIND_2X && 
               pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype != MEDIA_AUD_SUBTYPE_WMA)
            {
                b_get_pos_as_time = TRUE;
            }
            else if((pt_media_desc->e_speed != MM_SPEED_TYPE_FORWARD_1X && 
                     pt_media_desc->e_speed != MM_SPEED_TYPE_PAUSE) &&
                    pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_WMA)
            {
                b_get_pts_as_time = TRUE;
            }
        }
        
        t_dur_code.t_dur_type = MM_SVC_DUR_TYPE_ALL;
        t_dur_code.ui2_idx = 1;
        t_dur_code.ui8_duration = (UINT64)0xFFFFFFFFFFFFFFFF;

        i4_ret = t_fct_tbl.pf_get_dur(pt_media_desc,&t_dur_code);
        if (i4_ret != SVCTXR_OK || t_dur_code.ui8_duration == 0)
        {
            /*ignoer , in case of MPEG file the dur value is not exceptable*/
            t_dur_code.ui8_duration = (UINT64)0xFFFFFFFFFFFFFFFF;
        }

        e_speed = pt_media_desc->e_speed;
        if ((x_handle_valid(pt_video_strm_req->h_stream) == TRUE &&
             !(pt_media_desc->ui1_stream_eos_status & (ST_VIDEO+1))) ||
             (x_handle_valid(pt_audio_strm_req->h_stream) == TRUE &&
                  !(pt_media_desc->ui1_stream_eos_status & (ST_AUDIO+1)))
            )
        {
            UINT64 ui8_tmp = 0;
            if ((x_handle_valid(pt_video_strm_req->h_stream) == TRUE)
               &&((IS_FORWARD_DIR(e_speed)&& e_speed >= MM_SPEED_TYPE_FORWARD_4X) ||
                 (IS_BACKWARD_DIR(e_speed)) || (e_speed == MM_SPEED_TYPE_STEP)))
            {
                z_info_len = sizeof(VSH_PICTURE_INFO_T);
                t_vsh_pic_info.e_type = VSH_PIC_INFO_TYPE_PICTURE;
                i4_ret = x_sm_get(pt_video_strm_req->h_stream,
                                SM_VSH_GET_TYPE_PICTURE_INFO,
                                (VOID*)&t_vsh_pic_info,
                                &z_info_len);
                if (i4_ret != SMR_OK)
                {
                    return SVCTXR_FAIL;
                }
                pt_tm_code->ui8_time = t_vsh_pic_info.u.ui8_pts + pt_tm_code->ui8_time;
                if(e_get_type != SVCTX_MM_GET_TYPE_TIME_CODE_MS)
                {
                    pt_tm_code->ui8_time = _mm_div64(pt_tm_code->ui8_time, 
                                                 90000, 
                                                 NULL);
                }
                else
                {
                    pt_tm_code->ui8_time = _mm_div64(pt_tm_code->ui8_time, 
                                                     90, 
                                                     NULL);
                }
                if (gb_tm_on_off)
                {
                    x_dbg_stmt(_INFO_HEADER"SM_VSH_GET_TYPE_PICTURE_INFO ui8_pts = %lld\n",pt_tm_code->ui8_time);
                }
            }
            else
            {
                /*get stc time*/
                z_info_len = sizeof(STC_T);
                i4_ret = x_sm_get(
                                pt_running_svc_req->pt_player->h_sess,
                                SM_SESS_GNRC_GET_TYPE_STC,
                                (VOID*)&t_stc_value,
                                &z_info_len);
                if (i4_ret != SMR_OK)
                {
                    x_dbg_stmt("mm_hdlr : Get Time Code ERROR!\r\n");
                    return SVCTXR_FAIL;
                }
                if (gb_tm_on_off)
                {
                    x_dbg_stmt(_INFO_HEADER"SM_SESS_GNRC_GET_TYPE_STC ui8_stc = %lld\n", _mm_div64(t_stc_value, 90000, NULL));
                }
                if(b_get_pos_as_time)
                {
                    SM_SESS_GNRC_POST_INFO_T t_pos_info;
                    UINT64  ui8_start_offset;
                    z_info_len = sizeof(SM_SESS_GNRC_POST_INFO_T);
                    t_pos_info.e_type = _SM_SESS_GNRC_POST_INFO_MEDIA_BTE_OFST;
                    t_pos_info.ui8_pos_info = 0;
                    if(pt_running_svc_req != NULL)
                    {
                        pt_player = pt_running_svc_req->pt_player;            
                    }
                    else
                    {
                        return SVCTXR_FAIL;
                    }
                    i4_ret = x_sm_get(pt_player->h_sess,
                                    SM_SESS_GNRC_GET_TYPE_POST_INFO,
                                    (VOID*)&t_pos_info,
                                    &z_info_len);
                    if (i4_ret != SMR_OK)
                    {
                        return SVCTXR_FAIL;
                    }

                    if(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_AUDIO_ES &&  
                        pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_MP3)
                    {
                        ui8_start_offset = pt_tm_code->ui8_time;
                    }
                    else
                    {
                        ui8_start_offset = 0;
                    }
                    /*ASSERT(ui8_start_offset <= t_pos_info.ui8_pos_info, 
                         ("%s(), get audio file pos wrong !!\r\n", __func__));*/
                    if(ui8_start_offset <= t_pos_info.ui8_pos_info)
                    {
                        if(pt_media_desc->ui8_media_dur < (UINT64)0xFFFFFFFF)
                        {
                            if(e_get_type != SVCTX_MM_GET_TYPE_TIME_CODE_MS)
                            {
                                pt_tm_code->ui8_time = 
                                    _mm_div64((t_pos_info.ui8_pos_info - ui8_start_offset) * 
                                              pt_media_desc->ui8_media_dur, 
                                          (pt_media_desc->ui8_media_sz - ui8_start_offset)
                                           * 90000, NULL);
                            }
                            else
                            {
                                pt_tm_code->ui8_time = 
                                _mm_div64((t_pos_info.ui8_pos_info - ui8_start_offset) * 
                                              pt_media_desc->ui8_media_dur, 
                                          (pt_media_desc->ui8_media_sz - ui8_start_offset)
                                           * 90, NULL);
                            }
                        }
                        else  /*to avoid calculating overflow*/
                        {
                            if(e_get_type != SVCTX_MM_GET_TYPE_TIME_CODE_MS)
                            {
                                pt_tm_code->ui8_time = _mm_div64(pt_media_desc->ui8_media_dur, 90000, NULL);
                            }
                            else
                            {
                                pt_tm_code->ui8_time = _mm_div64(pt_media_desc->ui8_media_dur, 90, NULL);
                            }
                            
                            pt_tm_code->ui8_time *= (t_pos_info.ui8_pos_info - ui8_start_offset);
                            pt_tm_code->ui8_time = _mm_div64(pt_tm_code->ui8_time, 
                                                       (pt_media_desc->ui8_media_sz - ui8_start_offset), 
                                                        NULL);
                        }
                    }
                    else
                    {
                        x_dbg_stmt("start offset is %d, get pos is %d \n",(UINT32)ui8_start_offset, (UINT32)t_pos_info.ui8_pos_info );
                        pt_tm_code->ui8_time = 0;
                    }

                }
                else if(b_get_pts_as_time) /*for wma trick*/
                {
                    UINT64 ui8_cur_pts;
                    z_info_len = sizeof(ui8_cur_pts);
                    if(pt_running_svc_req == NULL)
                    {
                        return SVCTXR_FAIL;           
                    }
                    
                    if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream))
                    {
                        i4_ret = x_sm_get(
                                pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream,
                                SM_ASH_GET_TYPE_AUDIO_PTS,
                                (VOID*)&ui8_cur_pts,
                                &z_info_len
                                );
                        if (i4_ret != SMR_OK)
                        {
                            return SVCTXR_FAIL;
                        }
                        pt_tm_code->ui8_time = ui8_cur_pts;
                        if(e_get_type != SVCTX_MM_GET_TYPE_TIME_CODE_MS)
                        {
                            pt_tm_code->ui8_time = _mm_div64(pt_tm_code->ui8_time, 
                                                         90000, 
                                                         NULL);  
                        }
                        else
                        {
                            pt_tm_code->ui8_time = _mm_div64(pt_tm_code->ui8_time, 
                                                         90, 
                                                         NULL); 
                        }

                        if(pt_tm_code->ui8_time == 0 &&(
                           (pt_media_desc->e_speed >= MM_SPEED_TYPE_REWIND_32X 
                           &&pt_media_desc->e_speed <= MM_SPEED_TYPE_REWIND_2X
                           &&pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype != MEDIA_AUD_SUBTYPE_WMA)||
                           (pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype==MEDIA_AUD_SUBTYPE_WMA
                            &&pt_media_desc->e_speed>MM_SPEED_TYPE_REWIND_2X
                            ))
                           )
                        {
                            pt_tm_code->ui8_time = t_stc_value;
                            if(e_get_type != SVCTX_MM_GET_TYPE_TIME_CODE_MS)
                            {
                                pt_tm_code->ui8_time = _mm_div64(pt_tm_code->ui8_time, 
                                                             90000, 
                                                             NULL);
                            }
                            else
                            {
                                pt_tm_code->ui8_time = _mm_div64(pt_tm_code->ui8_time, 
                                                             90, 
                                                             NULL);
                            }                            
                        }
                    }
                }
                else
                {
                    pt_tm_code->ui8_time = t_stc_value;
                    if(e_get_type != SVCTX_MM_GET_TYPE_TIME_CODE_MS)
                    {
                        pt_tm_code->ui8_time = _mm_div64(pt_tm_code->ui8_time, 
                                                     90000, 
                                                     NULL);
                    }
                    else
                    {
                        pt_tm_code->ui8_time = _mm_div64(pt_tm_code->ui8_time, 
                                                     90, 
                                                     NULL);
                    }
                }
            }

            if(pt_media_desc->ui8_media_dur != 0)
            {
                if(e_get_type != SVCTX_MM_GET_TYPE_TIME_CODE_MS)
                {
                    ui8_tmp = _mm_div64(pt_media_desc->ui8_media_dur, 90000, NULL);
                }
                else
                {
                    ui8_tmp = _mm_div64(pt_media_desc->ui8_media_dur, 90, NULL);
                }
                
                if (pt_tm_code->ui8_time > ui8_tmp)
                {
                    pt_tm_code->ui8_time = ui8_tmp;
                }
            }
#ifdef DIVX_PLUS_CER /*MKV DivXPlus*/
            if(x_handle_valid(pt_video_strm_req->h_stream) == TRUE)/*wqyin*/            
            {                
                if(t_fct_tbl.pf_get_cur_chap)                
                {                    
                    MINFO_CHAP_INFO_T t_chap_info;                    
                    if(t_fct_tbl.pf_get_cur_chap(pt_media_desc, 0, &t_chap_info) == SVCTXR_OK)                    
                    {                        
                        if (pt_media_desc->b_is_seeking)                        
                        {                            
                            return SVCTXR_FAIL;                        
                        }                        
                        if (pt_tm_code->ui8_time + t_chap_info.ui8_pl_time >= t_chap_info.ui8_start_time)                        
                        {                            
                            pt_tm_code->ui8_time = pt_tm_code->ui8_time + t_chap_info.ui8_pl_time - t_chap_info.ui8_start_time;                        
                        }                        
                        else                        
                        {                            
                            pt_tm_code->ui8_time = 0;                        
                        }                    
                    }                
                }            
            }
#endif
        }
        else
        {
             return SVCTXR_FAIL;
        }
        /*check total time update*/
        if (x_handle_valid(pt_video_strm_req->h_stream) == TRUE)
        {
            z_info_len = sizeof(VSH_PICTURE_INFO_T);
            t_vsh_pic_info.e_type = VSH_PIC_INFO_TYPE_GOP;
            i4_ret = x_sm_get(pt_video_strm_req->h_stream,
                              SM_VSH_GET_TYPE_PICTURE_INFO,
                              (VOID*)&t_vsh_pic_info,
                              &z_info_len);
            if (i4_ret != SMR_OK)
            {
                return SVCTXR_FAIL;
            }
            if (t_vsh_pic_info.u.t_time_code.i4_total_dur_adj != 0)
            {
                i4_ret = t_fct_tbl.pf_get_dur(pt_media_desc,&t_dur_code);
                if (i4_ret != SVCTXR_OK)
                {
                    return i4_ret;
                }
                pt_media_desc->ui8_media_dur = t_dur_code.ui8_duration + t_vsh_pic_info.u.t_time_code.i4_total_dur_adj;
                DBG_INFO(("\n[MM_HDLR_API]i4_total_dur_adj:%d\n",t_vsh_pic_info.u.t_time_code.i4_total_dur_adj));
                i4_ret = _mm_hdlr_send_msg(pt_media_desc->h_svctx,
                                           (UINT32)NULL,
                                           _SVCTX_MSG_MNAV_INFO_UPDATE_NTFY,
                                           NULL,
                                           0,
                                           (UINT32)MM_NAVI_INFO_DURATION,
                                           (UINT32)0);
            }
        }
        else if (x_handle_valid(pt_audio_strm_req->h_stream) == TRUE)
        {   /*for mp3 total time is incorrect in minfo*/
            if((pt_media_desc->ui8_media_dur != 0) && 
                (t_dur_code.ui8_duration != (UINT64)0xFFFFFFFFFFFFFFFF)    &&
               (pt_media_desc->ui8_media_dur != t_dur_code.ui8_duration))
            {
                pt_media_desc->ui8_media_dur = t_dur_code.ui8_duration;
                i4_ret = _mm_hdlr_send_msg(pt_media_desc->h_svctx,
                                           (UINT32)NULL,
                                           _SVCTX_MSG_MNAV_INFO_UPDATE_NTFY,
                                           NULL,
                                           0,
                                           (UINT32)MM_NAVI_INFO_DURATION,
                                           (UINT32)0);
            }
        }
        break;
    case SVCTX_MM_GET_TYPE_DUR:
        pt_dur_code = (MM_SVC_DUR_INFO_T*)pv_get_info;
       
        /*Byte duration*/
        if( pt_dur_code->t_dur_type == MM_SVC_DUR_TYPE_ALL_OFFSET )
        {
            z_info_len = sizeof(MINFO_INFO_T);
            i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                      MINFO_INFO_TYPE_GENERIC,
                                      1,
                                      &t_minfo_rec,
                                      &z_info_len);
            if (i4_ret == MINFOR_OK)
            {
                if(t_minfo_rec.u.t_general_info.ui8_media_sz == 0xFFFFFFFFFFFFFFF)
                {
                    pt_dur_code->ui8_duration = 0;
                }
                else
                {
                    pt_dur_code->ui8_duration = t_minfo_rec.u.t_general_info.ui8_media_sz;
                }
            }
            else
            {   /*get file size for file system directly*/
                pt_dur_code->ui8_duration =
                     pt_media_desc->ui8_media_sz;
            }
            return SVCTXR_OK;
        }/*Time duration*/
        else if( pt_dur_code->t_dur_type == MM_SVC_DUR_TYPE_ALL )
        {            
#ifdef DIVX_PLUS_CER /*MKV DivXPlus*/
            if(t_fct_tbl.pf_get_cur_chap)            
            {                
                MINFO_CHAP_INFO_T t_chap_info;                
                MINFO_PLAYLIST_INFO_T t_pl_info;                
                SIZE_T  z_len = sizeof(MINFO_PLAYLIST_INFO_T); 
                
                if(t_fct_tbl.pf_get_cur_chap(pt_media_desc, 0, &t_chap_info) == SVCTXR_OK)                
                {                    
                    t_pl_info.ui4_title_idx = t_chap_info.ui4_title_idx;                    
                    t_pl_info.ui4_playlist_idx = t_chap_info.ui4_playlist_idx;                    
                    i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,                             
                        MINFO_INFO_TYPE_PLAYLIST_INFO,                             
                        0,                             
                        &t_pl_info,                             
                        &z_len);                    
                    if(i4_ret != MINFOR_OK)                    
                    {                         
                        return SVCTXR_NOT_SUPPORT;                    
                    }                    
                    pt_dur_code->ui8_duration = t_pl_info.ui8_total_time;                    
                    return SVCTXR_OK;                
                }            
            }
#endif
            i4_ret = t_fct_tbl.pf_get_dur(pt_media_desc,pt_dur_code);            
            if (pt_dur_code->ui8_duration == 0)
            {
                i4_ret = SVCTXR_NOT_SUPPORT;
            }
            else if ((pt_dur_code->ui8_duration != pt_media_desc->ui8_media_dur) &&
                     (pt_media_desc->ui8_media_dur!= 0))
            {
                pt_dur_code->ui8_duration = _mm_div64(pt_media_desc->ui8_media_dur, 90000, NULL);
            }
            else
            {
                pt_dur_code->ui8_duration = _mm_div64(pt_dur_code->ui8_duration, 90000, NULL);
            }
            if (i4_ret != SVCTXR_OK)
            {
                return i4_ret;
            }
            /*cr:315779 */
            if(pt_dur_code->ui8_duration == 0xFFFFFFFF)
            {
                pt_dur_code->ui8_duration = 0;
            }
        }
        break;
    case SVCTX_MM_GET_TYPE_MEDIA_FORMAT:
        if ((*pz_get_info_size) != sizeof(MEDIA_FORMAT_T) || pv_get_info == NULL) 
        {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }
        pt_media_fmt = (MEDIA_FORMAT_T*)pv_get_info;
        *pt_media_fmt = pt_media_desc->t_media_fmt;
        break;

    case SVCTX_MM_GET_TYPE_SPEED:
        if ((*pz_get_info_size) < sizeof(MM_SPEED_TYPE_T) || pv_get_info == NULL) 
        {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }
        (*(UINT32*)pv_get_info) = pt_media_desc->e_speed;
        break;
    case SVCTX_MM_GET_TYPE_VIDEO_INFO:
        pt_video_info = (MM_VIDEO_INFO_T*)pv_get_info;

        if (MM_HDLR_WITH_MINFO(pt_media_desc))
        {
            z_info_len = sizeof(MINFO_INFO_T);
            i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                      MINFO_INFO_TYPE_VIDEO_ATTR,
                                      1,
                                      &t_minfo_rec,
                                      &z_info_len);
            if (MINFOR_OK != i4_ret)
            {
                DBG_ERROR(("_mm_hdlr_get:get video info error(%d)\r\n", i4_ret));
            }
        }

         /*query pixel aspect ratio*/
        z_info_len = sizeof(VSH_SRC_RESOLUTION_INFO_T); 
        i4_ret = x_sm_get(pt_video_strm_req->h_stream,
                              SM_VSH_GET_TYPE_VIDEO_SRC_RESOLUTION,
                              (VOID*)&t_vsh_res,
                              &z_info_len);
        if(i4_ret != SMR_OK)
        {
            return SVCTXR_FAIL;            
        }
        if (t_vsh_res.e_src_asp_ratio == VSH_SRC_ASPECT_RATIO_USR)        
        {            
            pt_video_info->ui1_par_w = t_vsh_res.ui4_aspect_ratio_w;            
            pt_video_info->ui1_par_h = t_vsh_res.ui4_aspect_ratio_h;            
            pt_video_info->b_src_asp = FALSE;        
        }         
        else        
        {   
            _mm_hdlr_par_sm2svctx(t_vsh_res.e_src_asp_ratio,
                                  &pt_video_info->ui1_par_w,
                                  &pt_video_info->ui1_par_h,
                                  &pt_video_info->b_src_asp);
        }
         
        if (pt_media_desc->t_media_fmt.e_media_type  == MEDIA_TYPE_CONTAINER)
        {
            switch(pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype)
            {
            case MEDIA_CONTNR_SUBTYPE_AVI:
                pt_video_info->ui4_height =
                    (UINT32)t_minfo_rec.u.t_stm_attribute.u.t_avi_video_stm_attr.i4_height;
                pt_video_info->ui4_width =
                    (UINT32)t_minfo_rec.u.t_stm_attribute.u.t_avi_video_stm_attr.i4_width;
                break;
            case MEDIA_CONTNR_SUBTYPE_ASF:
                pt_video_info->ui4_height =
                    (UINT32)t_minfo_rec.u.t_stm_attribute.u.t_asf_video_stm_attr.i4_height;
                pt_video_info->ui4_width =
                    (UINT32)t_minfo_rec.u.t_stm_attribute.u.t_asf_video_stm_attr.i4_width;
                break;
            case MEDIA_CONTNR_SUBTYPE_MP4:
                pt_video_info->ui4_height =
                    (UINT32)t_minfo_rec.u.t_stm_attribute.u.t_mp4i_video_stm_attr.i4_height;
                pt_video_info->ui4_width =
                    (UINT32)t_minfo_rec.u.t_stm_attribute.u.t_mp4i_video_stm_attr.i4_width;
                break;
            case MEDIA_CONTNR_SUBTYPE_OGG:
                pt_video_info->ui4_height =
                    t_minfo_rec.u.t_stm_attribute.u.t_ogg_video_stm_attr.ui4_height;
                pt_video_info->ui4_width =
                    t_minfo_rec.u.t_stm_attribute.u.t_ogg_video_stm_attr.ui4_width;
                break;
            case MEDIA_CONTNR_SUBTYPE_MATROSKA:
                {
#ifdef DIVX_PLUS_CER
                    UINT64  ui8_tmp;
#endif

                    pt_video_info->ui4_height =
                        (UINT32)t_minfo_rec.u.t_stm_attribute.u.t_mkv_video_stm_attr.i4_height;
                    pt_video_info->ui4_width =
                        (UINT32)t_minfo_rec.u.t_stm_attribute.u.t_mkv_video_stm_attr.i4_width;
#ifdef DIVX_PLUS_CER
                    if(!t_minfo_rec.u.t_stm_attribute.u.t_mkv_video_stm_attr.i4_dsp_width ||
                        !t_minfo_rec.u.t_stm_attribute.u.t_mkv_video_stm_attr.i4_dsp_height ||
                        !pt_video_info->ui4_width ||
                        !pt_video_info->ui4_height)
                    {
                        pt_video_info->ui1_par_h = 0;
                        pt_video_info->ui1_par_w = 0;
                        pt_video_info->b_src_asp = TRUE;
                    }
                    else
                    {
                        /*(DisplayWidth / DisplayHeight) * (PixelHeight / PixelWidth)
                                            0.9       -> 10:11 ui1_par_w = 10,  ui1_par_h = 11
                                            1          -> 1:1
                                            1.09     -> 12:11
                                            1.2121  -> 40:33
                                            1.3333  -> 4:3
                                            1.3636  -> 15:11
                                            1.4545  -> 16:11
                                            1.5       -> 3:2
                                            1.6161  -> 160:99
                                            1.6363  -> 18:11
                                            1.8181  -> 20:11
                                            1.9393  -> 64:33
                                            2          -> 2:1
                                            2.1818  -> 24:11
                                            2.4242  -> 80:33
                                            2.9       -> 32:11*/
                        pt_video_info->b_src_asp = FALSE;
                        ui8_tmp = (UINT64)pt_video_info->ui4_height * 10000
                            * (UINT64)t_minfo_rec.u.t_stm_attribute.u.t_mkv_video_stm_attr.i4_dsp_width;
                        ui8_tmp /= ((UINT64)pt_video_info->ui4_width 
                            * (UINT64)t_minfo_rec.u.t_stm_attribute.u.t_mkv_video_stm_attr.i4_dsp_height);
                        if (ui8_tmp < 9500)/*0.9*/
                        {
                            pt_video_info->ui1_par_w = 10;
                            pt_video_info->ui1_par_h = 11;
                        }
                        else if (ui8_tmp >= 9500 && ui8_tmp < 10450)/*1*/
                        {
                            pt_video_info->ui1_par_w = 1;
                            pt_video_info->ui1_par_h = 1;
                        }
                        else if (ui8_tmp >= 10450 && ui8_tmp < 11510)/*1.09*/
                        {
                            pt_video_info->ui1_par_w = 12;
                            pt_video_info->ui1_par_h = 11;
                        }
                        else if (ui8_tmp >= 11510 && ui8_tmp < 12727)/*1.2121*/
                        {
                            pt_video_info->ui1_par_w = 40;
                            pt_video_info->ui1_par_h = 33;
                        }
                        else if (ui8_tmp >= 12727 && ui8_tmp < 13484)/*1.3333*/
                        {
                            pt_video_info->ui1_par_w = 4;
                            pt_video_info->ui1_par_h = 3;
                        }
                        else if (ui8_tmp >= 13484 && ui8_tmp < 14090)/*1.3636*/
                        {
                            pt_video_info->ui1_par_w = 15;
                            pt_video_info->ui1_par_h = 11;
                        }
                        else if (ui8_tmp >= 14090 && ui8_tmp < 14772)/*1.4545*/
                        {
                            pt_video_info->ui1_par_w = 16;
                            pt_video_info->ui1_par_h = 11;
                        }
                        else if (ui8_tmp >= 14772 && ui8_tmp < 15580)/*1.5*/
                        {
                            pt_video_info->ui1_par_w = 3;
                            pt_video_info->ui1_par_h = 2;
                        }
                        else if (ui8_tmp >= 15580 && ui8_tmp < 16262)/*1.6161*/
                        {
                            pt_video_info->ui1_par_w = 160;
                            pt_video_info->ui1_par_h = 99;
                        }
                        else if (ui8_tmp >= 16262 && ui8_tmp < 17272)/*1.6363*/
                        {
                            pt_video_info->ui1_par_w = 18;
                            pt_video_info->ui1_par_h = 11;
                        }
                        else if (ui8_tmp >= 17272 && ui8_tmp < 18787)/*1.8181*/
                        {
                            pt_video_info->ui1_par_w = 20;
                            pt_video_info->ui1_par_h = 11;
                        }
                        else if (ui8_tmp >= 18787 && ui8_tmp < 19696)/*1.9393*/
                        {
                            pt_video_info->ui1_par_w = 64;
                            pt_video_info->ui1_par_h = 33;
                        }
                        else if (ui8_tmp >= 19696 && ui8_tmp < 20909)/*2*/
                        {
                            pt_video_info->ui1_par_w = 2;
                            pt_video_info->ui1_par_h = 1;
                        }
                        else if (ui8_tmp >= 20909 && ui8_tmp < 23030)/*2.1818*/
                        {
                            pt_video_info->ui1_par_w = 24;
                            pt_video_info->ui1_par_h = 11;
                        }
                        else if (ui8_tmp >= 23030 && ui8_tmp < 26621)/*2.4242*/
                        {
                            pt_video_info->ui1_par_w = 80;
                            pt_video_info->ui1_par_h = 33;
                        }
                        else if (ui8_tmp >= 26621)/*2.9*/
                        {
                            pt_video_info->ui1_par_w = 32;
                            pt_video_info->ui1_par_h = 11;
                        }
                    }
#endif
                }
                break;
            case MEDIA_CONTNR_SUBTYPE_RM:
                pt_video_info->ui4_height =
                    (UINT32)t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui2_frm_height;
                pt_video_info->ui4_width =
                    (UINT32)t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui2_frm_width;
                break;
            case MEDIA_CONTNR_SUBTYPE_FLV:
            {
                VSH_SRC_RESOLUTION_INFO_T           t_vsh_res_flv;
                z_info_len = sizeof(VSH_SRC_RESOLUTION_INFO_T); 
                i4_ret = x_sm_get(pt_video_strm_req->h_stream,
                                  SM_VSH_GET_TYPE_VIDEO_SRC_RESOLUTION,
                                  (VOID*)&t_vsh_res_flv,
                                  &z_info_len);
                if(i4_ret != SMR_OK)
                {
                    return SVCTXR_FAIL;            
                }
                pt_video_info->ui4_height = t_vsh_res_flv.ui4_height;
                pt_video_info->ui4_width  = t_vsh_res_flv.ui4_width;
                break;
            }
            default:
                return SVCTXR_NOT_SUPPORT;
            }
            i4_ret = SVCTXR_OK;
        }
        else if (pt_media_desc->t_media_fmt.e_media_type  == MEDIA_TYPE_PS)
        {
            pt_video_info->ui4_height =
                (UINT32)t_minfo_rec.u.t_stm_attribute.u.t_ps_video_stm_attr.i4_height;
            pt_video_info->ui4_width =
                (UINT32)t_minfo_rec.u.t_stm_attribute.u.t_ps_video_stm_attr.i4_width;
            i4_ret = SVCTXR_OK;
        }
        else
        {
            pt_video_info->ui4_height = pt_svctx->t_video_info.ui4_height;
            pt_video_info->ui4_width = pt_svctx->t_video_info.ui4_width;
        }
        break;
    case SVCTX_MM_GET_TYPE_CURR_PROCESS_PROGRESS:
        pui1_curr_progress = (UINT8*)pv_get_info;
        z_info_len = sizeof(UINT8);
        i4_ret = x_midxbuld_get_info(pt_media_desc->h_index_info,
                                     MIDXBULD_GET_INFO_TYPE_BULD_PROGRESS,
                                     (VOID*)pui1_curr_progress,
                                     &z_info_len);
        if (i4_ret != MIDXBULDR_OK)
        {
            x_dbg_stmt("mm_hdlr : Get Index Building Progress ERROR!\r\n");
            return SVCTXR_FAIL;
        }
        break;
#ifdef TIME_SHIFT_SUPPORT
    case SVCTX_MM_GET_TYPE_TIMESHIFT_RANGE_INFO:
        pt_ts_range_info = (MM_TIMESHIFT_RANGE_INFO_T*)pv_get_info;
#if 0/*146400*/
        if(((!pt_media_desc->b_is_scdb_updated
                && !x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream)) 
            || (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream) 
                && !svctx_is_valid_video_fmt(pt_svctx)))
         && !pt_media_desc->b_bad_tick)
        {
            return SVCTXR_FAIL;
        }
#endif
        i4_ret = rec_util_get_tick_index(pt_media_desc->h_rec_util,
                                         pt_media_desc->t_safe_range.ui4_start,
                                         &t_tick_index);
        if (i4_ret != REC_UTILR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"rec_util_get_tick_index() failed, ret=%d\r\n", i4_ret));
            return SVCTXR_FAIL;
        }
        pt_ts_range_info->ui4_start = t_tick_index.ui4_tick_num;
        
        i4_ret = rec_util_get_tick_index(pt_media_desc->h_rec_util,
                                         pt_media_desc->t_safe_range.ui4_end,
                                         &t_tick_index);
        if (i4_ret != REC_UTILR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"rec_util_get_tick_index() failed, ret=%d\r\n", i4_ret));
            return SVCTXR_FAIL;
        }

        pt_ts_range_info->ui4_end = t_tick_index.ui4_tick_num;

        pt_ts_range_info->e_type = MM_TIMESHIFT_RANGE_INFO_TYPE_TICK;

        i4_ret = mm_hdlr_tshift_get_curr_pos(pt_svctx, &ui4_lba, &ui4_tick);
        if (i4_ret != SVCTXR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"mm_hdlr_tshift_get_curr_pos() failed, ret=%d\r\n", i4_ret));
            return SVCTXR_FAIL; 
        }
#if 1
        i4_ret = rec_util_get_tick_index(pt_media_desc->h_rec_util,
                                         ui4_tick,
                                         &t_tick_index);
        if (i4_ret != REC_UTILR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"rec_util_get_tick_index() failed, ret=%d\r\n", i4_ret));
            return SVCTXR_FAIL;
        }
#else
        i4_ret = rec_util_get_tick_by_lba(pt_media_desc->h_rec_util,
                                          ui4_lba,
                                          &t_tick_index);
        if (i4_ret != REC_UTILR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"rec_util_get_tick_by_lba() failed, ret=%d\r\n", i4_ret));
            return SVCTXR_FAIL;
        }
#endif
        pt_ts_range_info->ui4_current = t_tick_index.ui4_tick_num;
        pt_ts_range_info->b_bad_tick = pt_media_desc->b_bad_tick;

        if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOCKED)
        {
            i4_ret = x_cm_get( pt_running_svc_req->pt_player->h_conn,
                               PLAYBACK_CTRL_GET_CURR_UTC,
                               (VOID*)&(pt_ts_range_info->t_curr_sys_time));
            if ( i4_ret != CMR_OK )
            {
                DBG_ERROR((_ERROR_HEADER"cm_get_op(PLAYBACK_CTRL_GET_CURR_UTC) failed, ret=%d\r\n", i4_ret));
                return SVCTXR_FAIL;
            }
        }
        else
        {
            i4_ret = rec_util_get_sys_time_by_tick(pt_media_desc->h_rec_util,
                                                   &t_tick_index,
                                                   &(pt_ts_range_info->t_curr_sys_time));
            if (i4_ret != REC_UTILR_OK)
            {
                DBG_ERROR((_ERROR_HEADER"rec_util_get_sys_time_by_tick() failed, ret=%d\r\n", i4_ret));
                return SVCTXR_FAIL;
            }
        }
#if 1
        DBG_INFO((_INFO_HEADER"tshift range info(%u/%u/%u)\r\n", 
                  pt_ts_range_info->ui4_start,
                  pt_ts_range_info->ui4_current,
                  pt_ts_range_info->ui4_end));

#else
        x_dbg_stmt(_INFO_HEADER"timeshift range info is (%u/%u/%u)\r\n", 
                  pt_ts_range_info->ui4_start,
                  pt_ts_range_info->ui4_current,
                  pt_ts_range_info->ui4_end);
#endif
        break;

    case SVCTX_MM_GET_BRDCST_TYPE:
        if ((*pz_get_info_size) != sizeof(BRDCST_TYPE_T) || pv_get_info == NULL) 
        {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        *((BRDCST_TYPE_T*) pv_get_info) = pt_running_svc_req->e_brdcst_type;
        break;
    case SVCTX_COMMON_GET_TYPE_CRNT_VIDEO_DEVICE_TYPE:
        if ((*pz_get_info_size) != sizeof(DEVICE_TYPE_T) || pv_get_info == NULL) 
        {
            i4_ret = SVCTXR_INV_GET_INFO;
            break;
        }

        *((DEVICE_TYPE_T*)pv_get_info) = pt_running_svc_req->e_crnt_tuner_type;
        break;
#endif /* TIME_SHIFT_SUPPORT */

#if 1/*MKV DivXPlus*/
    case SVCTX_MM_GET_TYPE_CUR_CHAP_INFO:
        {
            MINFO_CHAP_INFO_T* pt_chap = (MINFO_CHAP_INFO_T*)pv_get_info;
            z_info_len = sizeof(MINFO_CHAP_INFO_T);
            x_memset(pt_chap, 0, z_info_len);

            if(t_fct_tbl.pf_get_cur_chap != NULL)
            {
            i4_ret = t_fct_tbl.pf_get_cur_chap(pt_media_desc, 0, pt_chap);                
                if(i4_ret != SVCTXR_OK)
                {
                    return SVCTXR_FAIL;
                }
            }
            else
            {
                return SVCTXR_FAIL;
            }
        }
        break;
    
    case SVCTX_MM_GET_TYPE_TITLE_NUM:
        {
            z_info_len = sizeof(UINT32);
            i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                      MINFO_INFO_TYPE_TITLE_NUM,
                                      0,
                                      pv_get_info,
                                      &z_info_len);
        }        
        break;
    
    case SVCTX_MM_GET_TYPE_TITLE_INFO:
        {
            z_info_len = sizeof(MINFO_TITLE_INFO_T);
            i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                      MINFO_INFO_TYPE_TITLE_INFO,
                                      0,
                                      pv_get_info,
                                      &z_info_len);
        }        
        break;
    
    case SVCTX_MM_GET_TYPE_PLAYLIST_INFO:
        {
            z_info_len = sizeof(MINFO_PLAYLIST_INFO_T);
            i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                      MINFO_INFO_TYPE_PLAYLIST_INFO,
                                      0,
                                      pv_get_info,
                                      &z_info_len);
        }        
        break;
    
    case SVCTX_MM_GET_TYPE_CHAP_INFO:
        {
            z_info_len = sizeof(MINFO_CHAP_INFO_T);
            i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                      MINFO_INFO_TYPE_CHAP_INFO,
                                      0,
                                      pv_get_info,
                                      &z_info_len);
        }        
        break;
    
    case SVCTX_MM_GET_TYPE_DISP_INFO:
        {
            z_info_len = sizeof(MINFO_DISP_INFO_T);
            i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                      MINFO_INFO_TYPE_DISP_INFO,
                                      0,
                                      pv_get_info,
                                      &z_info_len);
        }
        break;
    case SVCTX_MM_GET_TYPE_CUR_POS:
        {
            MINFO_CHAP_INFO_T*    pt_chap_id;

            pt_chap_id = (MINFO_CHAP_INFO_T*)pv_get_info;
            pt_chap_id->ui4_title_idx = pt_media_desc->ui4_title_idx;
            pt_chap_id->ui4_playlist_idx = pt_media_desc->ui4_playlist_idx;
            pt_chap_id->ui4_chap_idx = pt_media_desc->ui4_chap_idx;
        }
        break;
#endif
#if 1//MM_LYRIC_SUPPORT
    case SVCTX_MM_SBTL_GET_TYPE_LYRIC_INFO:
        {
            SVC_MM_SBTL_LYRIC_INFO_T* pt_lyric_info = (SVC_MM_SBTL_LYRIC_INFO_T*)pv_get_info;
            pt_lyric_info->b_has_lyric = pt_media_desc->b_is_lyric;
        }
        break;
#endif
     case SVCTX_MM_GET_VS_BIT_RATE:
        if((*pz_get_info_size) != sizeof(UINT32))
        {
            i4_ret = SVCTXR_INV_ARG;
            break;
        }
        *((UINT32*)pv_get_info) = pt_media_desc->ui4_bit_rate;
        break;
        
#ifdef MM_LAST_MEMORY_SUPPORT
    case SVCTX_MM_GET_STOP_INFO:
    {   
        MM_SVC_STOP_INFO*    pt_mm_stop_info = NULL;

        pt_mm_stop_info = (MM_SVC_STOP_INFO*)pv_get_info;
        if(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)
        {
            pt_mm_stop_info->ui4_title_idx = pt_media_desc->ui2_pmt_idx;
            pt_mm_stop_info->ui4_playlist_idx = 0;
            pt_mm_stop_info->ui4_chap_idx = 0;
        }
        else
        {
            pt_mm_stop_info->ui4_title_idx = pt_media_desc->ui4_title_idx;
            pt_mm_stop_info->ui4_playlist_idx = pt_media_desc->ui4_playlist_idx;
            pt_mm_stop_info->ui4_chap_idx = pt_media_desc->ui4_chap_idx;
        }
        pt_mm_stop_info->ui8_clip_id = pt_media_desc->ui8_mm_identifier;
        z_info_len = sizeof(MM_SVC_STOP_INFO);
        
        if (pt_media_desc->e_src_type == SRC_TYPE_MEDIA_NETWORK)
        {
            return SVCTXR_FAIL;
        }

        /*Just support the video*/
        if (!x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream))
        {
            return SVCTXR_FAIL;
        }
        
        i4_ret = mm_hdlr_get(pt_svctx,
                    SVCTX_MM_GET_TYPE_POS_PTS,
                    &pt_mm_stop_info->t_mm_svc_pos_info,
                    &z_info_len);
        if (i4_ret != SVCTXR_OK)
        {
            if(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)
            {
                x_memcpy(&pt_mm_stop_info->t_mm_svc_pos_info, 
                         &pt_media_desc->t_mm_last_info.t_mm_svc_pos_info, 
                         sizeof(MM_SVC_POS_INFO_T));
                i4_ret = SVCTXR_OK;
            }
            else
            {
                x_dbg_stmt("\n[mm_hdlr_api][error] _mm_hdlr_get SVCTX_MM_GET_TYPE_POS_PTS fail ret %d\n", i4_ret);
                x_memset(&pt_mm_stop_info->t_mm_svc_pos_info, 0, sizeof(MM_SVC_POS_INFO_T));
            }
        }
    }
        break;
#endif

#ifdef SUPPORT_SET_SBTL_ENCODING
    case SVCTX_MM_GET_SUBTITLE_TYPE:
        {
            MM_SVC_SUBTITLE_INFO_T* pt_subtl_info = (MM_SVC_SUBTITLE_INFO_T*)pv_get_info;
            pt_subtl_info->u.b_is_extra_subtitle = pt_media_desc->b_is_ext_first;
            break;
        }
#endif
        
    case SVCTX_MM_GET_TYPE_AFIFO_AMOUNT:
        {
            UINT32  ui4_afifo_sz = 0;
            z_info_len = sizeof( UINT32 );
            i4_ret = x_sm_get(
                    pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream,
                    SM_ASH_GET_TYPE_AFIFO_AMOUNT,
                    (VOID*)&ui4_afifo_sz,
                    &z_info_len
                    );
            if (i4_ret != SMR_OK)
            {
                return SVCTXR_FAIL;
            }
            *((UINT32*)pv_get_info) = ui4_afifo_sz;
            *pz_get_info_size = sizeof( UINT32 );
        }

    default:
        i4_ret = SVCTXR_NOT_SUPPORT;
        break;
    }

    return i4_ret;
}
#if 1//MM_LYRIC_SUPPORT
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_set_lyric_sbtl
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
INT32 _mm_hdlr_set_lyric_sbtl(
                    SVCTX_T*                    pt_svctx,
                    SVCTX_SET_TYPE_T            e_set_type,
                    VOID*                       pv_set_info,
                    SIZE_T                      z_set_info_size
                    )
{
    INT32           i4_ret;
    MM_SVC_REQ_T*   pt_running_svc_req  = NULL;
    SM_SET_TYPE_T   e_set_sbtl_type     = 0;
    VOID*           pv_set_sbtl_info    = NULL;
    SIZE_T          z_size              = 0;
    MEDIA_DESCRIBT_T*        pt_media_desc;
    
    MM_SBTL_ENCODING_T*      pe_sbtl_enc    = NULL;
    WGL_FONT_INFO_T*         pt_fnt_info    = NULL;
    MM_SBTL_DISP_PAGE_T*     pt_page_info   = NULL;
    MM_SBTL_TM_OFST_INF*     pt_tm_ofst     = NULL;
    WGL_COLOR_INFO_T*        pt_clr_info    = NULL;
    GL_RECT_T*               pt_rect        = NULL;
    GL_POINT_T*              pt_osd_oft     = NULL;
    BOOL*                    pb_sbtl_show   = NULL;
    
    
    if(!pt_svctx || (!pv_set_info && SVCTX_MM_SBTL_SET_TYPE_DEFAULT != e_set_type))
    {
        return SVCTXR_INV_ARG;
    }

    #if 0
    x_memset(pt_fnt_info, 0, sizeof(WGL_FONT_INFO_T));
    x_memset(pt_page_info, 0, sizeof(MM_SBTL_DISP_PAGE_T));
    x_memset(pt_tm_ofst, 0, sizeof(MM_SBTL_TM_OFST_INF));
    x_memset(pt_clr_info, 0, sizeof(WGL_COLOR_INFO_T));
    x_memset(pt_rect, 0, sizeof(GL_RECT_T));
    #else
    pe_sbtl_enc  = &pt_svctx->t_mm_sbtl_lyric_info.e_enc;
    pt_fnt_info  = &pt_svctx->t_mm_sbtl_lyric_info.t_font;
    pt_page_info = &pt_svctx->t_mm_sbtl_lyric_info.t_page;
    pt_tm_ofst   = &pt_svctx->t_mm_sbtl_lyric_info.t_tm_ofst;
    pt_clr_info  = &pt_svctx->t_mm_sbtl_lyric_info.t_bg_clr;
    pt_rect      = &pt_svctx->t_mm_sbtl_lyric_info.t_rect;
    pt_osd_oft   = &pt_svctx->t_mm_sbtl_lyric_info.t_osd_offset;
    pb_sbtl_show = &pt_svctx->t_mm_sbtl_lyric_info.b_hide;
    #endif
    
    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    #if 0
    if(!pt_running_svc_req)
    {
        return SVCTXR_INV_ARG;
    }
    #endif

    i4_ret = SVCTXR_OK;
    switch(e_set_type)
    {
        case SVCTX_MM_SBTL_SET_TYPE_DEFAULT:
            {
                CHAR a_name[] = MM_SBTL_DFT_FONT_NAME;
                GL_COLOR_T t_bg_dis = MM_SBTL_DFT_BG_DIS_COLOR;
                GL_COLOR_T t_bg_en = MM_SBTL_DFT_BG_EN_COLOR;
                GL_COLOR_T t_bg_hl = MM_SBTL_DFT_BG_HLT_COLOR;
                GL_COLOR_T t_fg_dis = MM_SBTL_DFT_FG_DIS_COLOR;
                GL_COLOR_T t_fg_en = MM_SBTL_DFT_FG_EN_COLOR;
                GL_COLOR_T t_fg_hl = MM_SBTL_DFT_FG_HLT_COLOR;
                
                pt_svctx->t_mm_sbtl_lyric_info.e_enc = MM_SBTL_DFT_ENCODING;
                x_memcpy(pt_svctx->t_mm_sbtl_lyric_info.t_font.a_c_font_name,
                    a_name,
                    sizeof(MM_SBTL_DFT_FONT_NAME) < WGL_MAX_FONT_NAME ? sizeof(MM_SBTL_DFT_FONT_NAME) : WGL_MAX_FONT_NAME);
                pt_svctx->t_mm_sbtl_lyric_info.t_font.e_font_cmap = MM_SBTL_DFT_FONT_CMAP;
                pt_svctx->t_mm_sbtl_lyric_info.t_font.e_font_size = MM_SBTL_DFT_FONT_SIZE;
                pt_svctx->t_mm_sbtl_lyric_info.t_font.e_font_style = MM_SBTL_DFT_FONT_STYLE;
                pt_svctx->t_mm_sbtl_lyric_info.t_font.i2_width = MM_SBTL_DFT_FONT_WIDTH;
                pt_svctx->t_mm_sbtl_lyric_info.t_font.ui1_custom_size = MM_SBTL_DFT_FONT_CSTM_SIZE;
                pt_svctx->t_mm_sbtl_lyric_info.t_page.e_page_mode = MM_SBTL_DFT_PAGE_MODE;
                pt_svctx->t_mm_sbtl_lyric_info.t_page.i4_line_gap = MM_SBTL_DFT_PAGE_LINE_GAP;
                pt_svctx->t_mm_sbtl_lyric_info.t_page.ui2_hlt_fix_line = MM_SBTL_DFT_PAGE_HLT_FIX_LINE;
                pt_svctx->t_mm_sbtl_lyric_info.t_page.ui2_interval_line = MM_SBTL_DFT_PAGE_INTERVAL_LINE;
                pt_svctx->t_mm_sbtl_lyric_info.t_page.ui2_page_line_num = MM_SBTL_DFT_PAGE_LINE;
                pt_svctx->t_mm_sbtl_lyric_info.t_tm_ofst.e_ofst_mode = MM_SBTL_TM_OFST_MODE_OFF;
                pt_svctx->t_mm_sbtl_lyric_info.t_tm_ofst.i8_time_ofst = 0;
                pt_svctx->t_mm_sbtl_lyric_info.t_bg_clr.e_type = MM_SBTL_DFT_BG_TYPE;
                x_memcpy(&pt_svctx->t_mm_sbtl_lyric_info.t_bg_clr.u_color_data.t_standard.t_disable,
                    &t_bg_dis,
                    sizeof(GL_COLOR_T));
                x_memcpy(&pt_svctx->t_mm_sbtl_lyric_info.t_bg_clr.u_color_data.t_standard.t_enable,
                    &t_bg_en,
                    sizeof(GL_COLOR_T));
                x_memcpy(&pt_svctx->t_mm_sbtl_lyric_info.t_bg_clr.u_color_data.t_standard.t_highlight,
                    &t_bg_hl,
                    sizeof(GL_COLOR_T));
                pt_svctx->t_mm_sbtl_lyric_info.t_fg_clr.e_type = MM_SBTL_DFT_FG_TYPE;
                x_memcpy(&pt_svctx->t_mm_sbtl_lyric_info.t_fg_clr.u_color_data.t_standard.t_disable,
                    &t_fg_dis,
                    sizeof(GL_COLOR_T));
                x_memcpy(&pt_svctx->t_mm_sbtl_lyric_info.t_fg_clr.u_color_data.t_standard.t_enable,
                    &t_fg_en,
                    sizeof(GL_COLOR_T));
                x_memcpy(&pt_svctx->t_mm_sbtl_lyric_info.t_fg_clr.u_color_data.t_standard.t_highlight,
                    &t_fg_hl,
                    sizeof(GL_COLOR_T));
                pt_svctx->t_mm_sbtl_lyric_info.b_hide = FALSE;
                
            }
            break;
            
        case SVCTX_MM_SBTL_SET_TYPE_DISP_MODE:
            {
                MM_SVC_SBTL_DISP_MODE_T* pt_disp_mode;
                pt_disp_mode = (MM_SVC_SBTL_DISP_MODE_T*)pv_set_info;
				DBG_INFO((_INFO_HEADER" Set SVCTX_MM_SBTL_SET_TYPE_DISP_MODE, e_disp_mode=%d, ui2_param=0x%x\n", 
						pt_disp_mode->e_disp_mode, pt_disp_mode->ui2_param));
                switch(pt_disp_mode->e_disp_mode)
                {
                case MM_SBTL_DISP_MODE_OFF:
                    pt_page_info->ui2_page_line_num = 0;
                    break;
                case MM_SBTL_DISP_MODE_SINGLE_LINE:
                    pt_page_info->ui2_page_line_num = 1;
                    break;
                case MM_SBTL_DISP_MODE_MULTI_LINE:
                    pt_page_info->ui2_page_line_num = pt_disp_mode->ui2_param;
                    break;
                default:
                    return SVCTXR_INV_SET_INFO;
                }
				
                e_set_sbtl_type = SM_MM_SBTL_SET_TYPE_PAGE_INFO;
                pv_set_sbtl_info = pt_page_info;
                z_size = sizeof(MM_SBTL_DISP_PAGE_T);
            }
            break;
            
        case SVCTX_MM_SBTL_SET_TYPE_HILT_STL:
            {
                MM_SVC_SBTL_HILT_STL_T* pt_hlt_stl;

                pt_hlt_stl = (MM_SVC_SBTL_HILT_STL_T*)pv_set_info;
				DBG_INFO((_INFO_HEADER" Set SVCTX_MM_SBTL_SET_TYPE_HILT_STL, e_hlt_style=%d, ui2_param=0x%x\n", 
						pt_hlt_stl->e_hlt_style, pt_hlt_stl->ui2_param));
                switch(pt_hlt_stl->e_hlt_style)
                {
                case MM_SBTL_HILT_STL_BY_LINE:
                    pt_page_info->e_page_mode = MM_SBTL_PAGE_SCROLL_LINE;
                    break;
                case MM_SBTL_HILT_STL_KARAOKE:
                    pt_page_info->e_page_mode = MM_SBTL_PAGE_HLT_FIXED;
                    pt_page_info->ui2_hlt_fix_line = pt_hlt_stl->ui2_param;
                    break;
                default:
                    return SVCTXR_INV_SET_INFO;
                }
                e_set_sbtl_type = SM_MM_SBTL_SET_TYPE_PAGE_INFO;
                pv_set_sbtl_info = pt_page_info;
                z_size = sizeof(MM_SBTL_DISP_PAGE_T);
            }
            break;
            
        case SVCTX_MM_SBTL_SET_TYPE_TIME_OFST:
            {
                MM_SVC_SBTL_TIME_OFST_T* pt_tm_ofst_tmp;

                pt_tm_ofst_tmp = (MM_SVC_SBTL_TIME_OFST_T*)pv_set_info;
				DBG_INFO((_INFO_HEADER" Set SVCTX_MM_SBTL_SET_TYPE_TIME_OFST, e_time_ofst=%d, ui4_ofst_value=0x%x\n", 
					pt_tm_ofst_tmp->e_time_ofst, pt_tm_ofst_tmp->ui4_ofst_value));
                switch(pt_tm_ofst_tmp->e_time_ofst)
                {
                case MM_SBTL_TIME_OFST_OFF:
                    pt_tm_ofst->e_ofst_mode = MM_SBTL_TM_OFST_MODE_OFF;
                    break;
                case MM_SBTL_TIME_OFST_AUTO:
                    pt_tm_ofst->e_ofst_mode = MM_SBTL_TM_OFST_MODE_AUTO;
                    break;
                case MM_SBTL_TIME_OFST_USER_DEF:
                    pt_tm_ofst->e_ofst_mode = MM_SBTL_TM_OFST_MODE_USER;
                    pt_tm_ofst->i8_time_ofst = (INT64)pt_tm_ofst_tmp->ui4_ofst_value;
                    break;
                default:
                    return SVCTXR_INV_SET_INFO;
                }
                e_set_sbtl_type = SM_MM_SBTL_SET_TYPE_ADJUST_TIME;
                pv_set_sbtl_info = pt_tm_ofst;
                z_size = sizeof(MM_SBTL_TM_OFST_INF);
            }
            break;
            
        case SVCTX_MM_SBTL_SET_TYPE_FONT_ENC:
            {
                MM_SVC_SBTL_FONT_ENC_T* pt_fnt_enc;
                
                pt_fnt_enc = (MM_SVC_SBTL_FONT_ENC_T*)pv_set_info;
				DBG_INFO((_INFO_HEADER" Set SVCTX_MM_SBTL_SET_TYPE_FONT_ENC, e_enc_type=%d, ui2_param=0x%x\n", 
					pt_fnt_enc->e_enc_type, pt_fnt_enc->ui2_param));
                switch(pt_fnt_enc->e_enc_type)
                {
                case MM_SBTL_FONT_ENC_AUTO:
                    *pe_sbtl_enc = MM_SBTL_ENCODE_AUTO;
                    break;
                case MM_SBTL_FONT_ENC_UTF8:
                    *pe_sbtl_enc = MM_SBTL_ENCODE_UTF8;
                    break;
                case MM_SBTL_FONT_ENC_UTF16:
                    *pe_sbtl_enc = MM_SBTL_ENCODE_UTF16;
                    break;
                case MM_SBTL_FONT_ENC_BIG5:
                    *pe_sbtl_enc = MM_SBTL_ENCODE_BIG5;
                    break;
                case MM_SBTL_FONT_ENC_GB:
                    *pe_sbtl_enc = MM_SBTL_ENCODE_GB2312;
                    break;
                default:
                    return SVCTXR_INV_SET_INFO;
                }

                e_set_sbtl_type = SM_MM_SBTL_SET_TYPE_ENCODING;
                pv_set_sbtl_info = pe_sbtl_enc;
                z_size = sizeof(MM_SBTL_ENCODING_T);
            }
            break;
            
        case SVCTX_MM_SBTL_SET_TYPE_HILT_ATTR:
            break;
            
        case SVCTX_MM_SBTL_SET_TYPE_NRML_ATTR:
            {
                MM_SVC_SBTL_DISP_ATTR_T* pt_nm_attr = (MM_SVC_SBTL_DISP_ATTR_T*)pv_set_info;

                
                if(!pt_running_svc_req)
                {
                    return SVCTXR_INV_ARG;
                }
                
                if(MM_SBTL_ATTR_FNT_INFO & pt_nm_attr->t_attr_field)
                {
                    x_memcpy(pt_fnt_info, &pt_nm_attr->t_font_info, sizeof(WGL_FONT_INFO_T));
                    e_set_sbtl_type = SM_MM_SBTL_SET_TYPE_OSD_FONT_INFO;
                    pv_set_sbtl_info = pt_fnt_info;
                    z_size = sizeof(WGL_FONT_INFO_T);
					DBG_INFO((_INFO_HEADER" Set MM_SBTL_ATTR_FNT_INFO, size[%d]style[%d]cmap[%d]name[%s]width[%d]custom_size[%d]\n", 
							pt_fnt_info->e_font_size, 	pt_fnt_info->e_font_style, 	pt_fnt_info->e_font_cmap,
							pt_fnt_info->a_c_font_name, pt_fnt_info->i2_width, 		pt_fnt_info->ui1_custom_size));
					
                    if(pt_running_svc_req && (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream)))
                    {
                        i4_ret = x_sm_set(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream,
                                         e_set_sbtl_type,
                                         pv_set_sbtl_info,
                                         z_size);
                        if (i4_ret!= SMR_OK)
                        {
                            DBG_ERROR((_ERROR_HEADER"block PSI fail!\r\n"));
                        }
                    }
                }
                if(MM_SBTL_ATTR_BKG_CLR & pt_nm_attr->t_attr_field)
                {
                    pt_clr_info->e_type = WGL_COLOR_SET_BASIC;
                    pt_clr_info->u_color_data.t_basic.t_enable = pt_nm_attr->t_bkg_clr;
					DBG_INFO((_INFO_HEADER" Set MM_SBTL_ATTR_BKG_CLR-argb=%d,%d,%d,%d\n", 
							pt_nm_attr->t_bkg_clr.a, pt_nm_attr->t_bkg_clr.u1.r, pt_nm_attr->t_bkg_clr.u2.g, pt_nm_attr->t_bkg_clr.u3.b));
					
                    e_set_sbtl_type = SM_MM_SBTL_SET_TYPE_OSD_BG_COLOR;
                    pv_set_sbtl_info = pt_clr_info;
                    z_size = sizeof(WGL_COLOR_INFO_T);
                    if(pt_running_svc_req && (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream)))
                    {
                        i4_ret = x_sm_set(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream,
                                         e_set_sbtl_type,
                                         pv_set_sbtl_info,
                                         z_size);
                        
                        if (i4_ret!= SMR_OK)
                        {
                            DBG_ERROR((_ERROR_HEADER"block PSI fail!\r\n"));
                        }
                    }
                }
                if(MM_SBTL_ATTR_TXT_CLR & pt_nm_attr->t_attr_field)
                {
                    pt_clr_info  = &pt_svctx->t_mm_sbtl_lyric_info.t_fg_clr;
                    x_memset(pt_clr_info, 0, sizeof(WGL_COLOR_INFO_T));
                    pt_clr_info->e_type = WGL_COLOR_SET_STANDARD;
                    pt_clr_info->u_color_data.t_basic.t_enable = pt_nm_attr->t_txt_clr;
					DBG_INFO((_INFO_HEADER" Set MM_SBTL_ATTR_TXT_CLR-argb=%d,%d,%d,%d\n", 
							pt_nm_attr->t_txt_clr.a, pt_nm_attr->t_txt_clr.u1.r, pt_nm_attr->t_txt_clr.u2.g, pt_nm_attr->t_txt_clr.u3.b));
                    
                    e_set_sbtl_type = SM_MM_SBTL_SET_TYPE_OSD_FG_COLOR;
                    pv_set_sbtl_info = pt_clr_info;
                    z_size = sizeof(WGL_COLOR_INFO_T);
                    if(pt_running_svc_req && (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream)))
                    {
                        i4_ret = x_sm_set(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream,
                                         e_set_sbtl_type,
                                         pv_set_sbtl_info,
                                         z_size);
                        
                        if (i4_ret!= SMR_OK)
                        {
                            DBG_ERROR((_ERROR_HEADER"block PSI fail!\r\n"));
                        }
                    }
                }
				if(MM_SBTL_ATTR_EDG_CLR & pt_nm_attr->t_attr_field)
			   {
				   pt_clr_info	= &pt_svctx->t_mm_sbtl_lyric_info.t_edg_clr;
				   x_memset(pt_clr_info, 0, sizeof(WGL_COLOR_INFO_T));
				   pt_clr_info->e_type = WGL_COLOR_SET_STANDARD;
				   pt_clr_info->u_color_data.t_basic.t_enable = pt_nm_attr->t_edge_clr;
				   DBG_INFO((_INFO_HEADER" Set MM_SBTL_ATTR_EDG_CLR-argb=%d,%d,%d,%d\n", 
						   pt_nm_attr->t_edge_clr.a, pt_nm_attr->t_edge_clr.u1.r, pt_nm_attr->t_edge_clr.u2.g, pt_nm_attr->t_edge_clr.u3.b));
				   
				   e_set_sbtl_type = SM_MM_SBTL_SET_TYPE_OSD_FONT_EDGE_COLOR;
				   pv_set_sbtl_info = pt_clr_info;
				   z_size = sizeof(WGL_COLOR_INFO_T);
				   if(pt_running_svc_req && (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream)))
				   {
					   i4_ret = x_sm_set(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream,
										e_set_sbtl_type,
										pv_set_sbtl_info,
										z_size);
					   
					   if (i4_ret!= SMR_OK)
					   {
						   DBG_ERROR((_ERROR_HEADER"block PSI fail!\r\n"));
					   }
				   }
			    }
                if(MM_SBTL_ATTR_BDR_TYPE  & pt_nm_attr->t_attr_field)
                {
                }
                if(MM_SBTL_ATTR_BDR_WIDTH & pt_nm_attr->t_attr_field)
                {
                }
                if(MM_SBTL_ATTR_ROLL_TYPE & pt_nm_attr->t_attr_field)
                {
                }
                if(MM_SBTL_ATTR_DISP_RECT & pt_nm_attr->t_attr_field)
                {
                    x_memcpy(pt_rect, &pt_nm_attr->t_disp_rect, sizeof(GL_RECT_T));
                    
                    e_set_sbtl_type = SM_MM_SBTL_SET_TYPE_OSD_RECT;
                    pv_set_sbtl_info = pt_rect;
                    z_size = sizeof(GL_RECT_T);
					DBG_INFO((_INFO_HEADER" Set MM_SBTL_ATTR_DISP_RECT-ltrb=%d,%d,%d,%d\n", 
							pt_rect->i4_left, pt_rect->i4_top, pt_rect->i4_right, pt_rect->i4_bottom));
					
                    if(pt_running_svc_req && (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream)))
                    {
                        i4_ret = x_sm_set(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream,
                                         e_set_sbtl_type,
                                         pv_set_sbtl_info,
                                         z_size);
                        
                        if (i4_ret!= SMR_OK)
                        {
                            DBG_ERROR((_ERROR_HEADER"block PSI fail!\r\n"));
                        }
                    }
                }
                if(MM_SBTL_ATTR_OSD_OFFSET & pt_nm_attr->t_attr_field)
                {
                    x_memcpy(pt_osd_oft, &pt_nm_attr->t_osd_offset, sizeof(GL_POINT_T));
                    
                    e_set_sbtl_type = SM_MM_SBTL_SET_TYPE_OSD_OFFSET;
                    pv_set_sbtl_info = pt_osd_oft;
                    z_size = sizeof(GL_POINT_T);
					DBG_INFO((_INFO_HEADER" Set MM_SBTL_ATTR_OSD_OFFSET-(x,y)=%d,%d\n", 
							pt_osd_oft->i4_x, pt_osd_oft->i4_y));
					
                    if(pt_running_svc_req && (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream)))
                    {
                        i4_ret = x_sm_set(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream,
                                         e_set_sbtl_type,
                                         pv_set_sbtl_info,
                                         z_size);
                        
                        if (i4_ret!= SMR_OK)
                        {
                            DBG_ERROR((_ERROR_HEADER"block PSI fail!\r\n"));
                        }
                    }
                }
                return i4_ret;
            }
            
        case SVCTX_MM_SBTL_SET_TYPE_SHOW_HIDE:
            {
                MM_SVC_SBTL_SHOW_HIDE_T* pt_show_hide = NULL;

                if(!pt_running_svc_req)
                {
                    return SVCTXR_INV_ARG;
                }
				
                pt_show_hide = (MM_SVC_SBTL_SHOW_HIDE_T*)pv_set_info;
                *pb_sbtl_show = pt_show_hide->b_sbtl_show;
				DBG_INFO((_INFO_HEADER" Set SVCTX_MM_SBTL_SET_TYPE_SHOW_HIDE-show=%d\n", 
						*pb_sbtl_show));
				
                i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc,&pt_media_desc);
                if(i4_ret != SVCTXR_OK)
                {
                    return i4_ret;
                }
                if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)
                {
                    e_set_sbtl_type = SM_SBTLH_SET_TYPE_VISIBLE;
                }
                else
                {
                    e_set_sbtl_type = SM_MM_SBTL_SET_TYPE_SHOW_HIDE;
                }
                pv_set_sbtl_info = pb_sbtl_show;
                z_size = sizeof(BOOL);
            }
            break;

        default:
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
    }

    if(NULL != pv_set_sbtl_info && z_size != 0 && pt_running_svc_req)
    {
        if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream))
        {
            i4_ret = x_sm_set(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream,
                         e_set_sbtl_type,
                         pv_set_sbtl_info,
                         z_size);
        
            if (i4_ret!= SMR_OK)
            {
                DBG_ERROR((_ERROR_HEADER"block PSI fail!\r\n"));
            }
        }
    }
    return i4_ret;
}
#endif
/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_get_play_mode
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
INT32 mm_hdlr_get_play_mode(
                    MM_SPEED_TYPE_T                     e_speed,
                    MM_SVC_REQ_T*                       pt_running_svc_req,
                    MM_PLAY_MODE_TYPE_T*                pt_mm_play_mode)
{
    MEDIA_DESCRIBT_T*                   pt_media_desc = NULL;
    INT32                               i4_ret = SVCTXR_OK;

    if (pt_running_svc_req == NULL || pt_mm_play_mode == NULL)
    {
        return SVCTXR_INV_ARG;
    }
    
    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc,&pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }
    
    if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream))
    {
        if(pt_media_desc->b_has_key_table)
        {
            if ((pt_media_desc->b_is_no_idexing)    ||
                (e_speed >= MM_SPEED_TYPE_PAUSE     &&
                 e_speed <= MM_SPEED_TYPE_FORWARD_2X))
            {
                *pt_mm_play_mode = MM_PLAY_MODE_TYPE_NORMAL;
            }
            else
            {
                *pt_mm_play_mode = MM_PLAY_MODE_TYPE_IFRAME;
            }
        }
        else
        {
            *pt_mm_play_mode = MM_PLAY_MODE_TYPE_NORMAL;
        }
    }
    else if(x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream))
    {
        if (e_speed == MM_SPEED_TYPE_PAUSE || e_speed==MM_SPEED_TYPE_FORWARD_1X)
        {
            *pt_mm_play_mode = MM_PLAY_MODE_TYPE_NORMAL;
        }
        else
        {
            *pt_mm_play_mode = MM_PLAY_MODE_TYPE_IFRAME;
        }
    }
    else
    {
        *pt_mm_play_mode = MM_PLAY_MODE_TYPE_NORMAL;
    }
    
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_set_change_strm
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_set_change_strm(
                    SVCTX_T*                    pt_svctx,
                    VOID*                       pv_set_info,
                    SIZE_T                      z_set_info_size
                    )
{
    MM_STRM_ATRBT_T*                    pt_mm_stm_atrbt = NULL;
    PLAYBACK_HANDLER_ITEM_SCDB_INFO_T   t_scdb_info;
    UINT16                              ui2_idx = 0;
    UINT8                               ui1_i = 0;
    BOOL                                b_found = FALSE;
    MM_SVC_REQ_T*                       pt_running_svc_req = NULL;
    MEDIA_DESCRIBT_T*                   pt_media_desc = NULL;
    INT32                               i4_ret = 0;

    /* Check Parameters*/
    if (pt_svctx == NULL || pv_set_info == NULL || z_set_info_size != sizeof(MM_STRM_ATRBT_T))
    {
        return SVCTXR_INV_ARG;
    }

    x_memset(&t_scdb_info, 0, sizeof(PLAYBACK_HANDLER_ITEM_SCDB_INFO_T));    
    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    pt_mm_stm_atrbt = (MM_STRM_ATRBT_T*)pv_set_info;
    
    if ((NULL==pt_mm_stm_atrbt) || (NULL==pt_running_svc_req))
    {
        return SVCTXR_FAIL;
    }
    

    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc,&pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }
    if (NULL == pt_media_desc->pt_strm_info)
    {
        return SVCTXR_FAIL;
    }
    
    if ((pt_mm_stm_atrbt->e_type!=ST_VIDEO) &&
        (pt_mm_stm_atrbt->e_type!=ST_AUDIO))
    {
        return SVCTXR_FAIL;
    }

    for (ui1_i=0; ui1_i<pt_media_desc->pt_strm_info->ui2_num_stm; ui1_i++)
    {
        if(ui1_i >= SUPPORTED_STRM_NUM_MAX)
        {
            ui1_i = SUPPORTED_STRM_NUM_MAX - 1;
            break;
        }
        if (pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_i].e_type == pt_mm_stm_atrbt->e_type)
        {
            pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_i] = *pt_mm_stm_atrbt;
            b_found = TRUE;
            break;
        }
    }
    if (!b_found)
    {
        pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_i] = *pt_mm_stm_atrbt;
    }

    i4_ret = _mm_hdlr_get_using_scdb_info_ex(pt_svctx,
                                             pt_mm_stm_atrbt->e_type,
                                             &(t_scdb_info.t_stream_id),
                                             &(t_scdb_info.t_scdb_rec),
                                             &ui2_idx);

    if (i4_ret == SVCTXR_OK)
    {
        if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)
        {
            if(pt_mm_stm_atrbt->e_type == ST_VIDEO)
            {
                t_scdb_info.t_scdb_rec.u.t_video_mpeg.ui2_pid =
                    pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_i].t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_stm_id;
                
                t_scdb_info.t_scdb_rec.u.t_video_mpeg.e_enc =
                    _minfo_video_enc_to_scdb_enc(pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_i].t_stm_atrbt.u.t_ps_video_stm_attr.e_enc);
                
                t_scdb_info.t_scdb_rec.u.t_video_mpeg.e_vid_fmt =
                    pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_i].t_stm_atrbt.u.t_ps_video_stm_attr.e_vid_fmt;
                
                t_scdb_info.t_scdb_rec.u.t_video_ps.b_default =
                    pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_i].t_stm_atrbt.u.t_ps_video_stm_attr.b_default;
                

                t_scdb_info.t_scdb_rec.e_rec_type = SCDB_REC_TYPE_VIDEO_MPEG;
            }
            else if(pt_mm_stm_atrbt->e_type == ST_AUDIO)
            {
                t_scdb_info.t_scdb_rec.e_rec_type = SCDB_REC_TYPE_AUDIO_MPEG;

                t_scdb_info.t_scdb_rec.u.t_audio_mpeg.ui2_pid
                    = pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_i].t_stm_atrbt.t_stm_id.u.t_ps_stm_id.ui1_stm_id;

                t_scdb_info.t_scdb_rec.u.t_audio_mpeg.e_enc =
                    _minfo_audio_enc_to_scdb_enc(pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_i].t_stm_atrbt.u.t_ps_audio_stm_attr.e_enc);
                
                t_scdb_info.t_scdb_rec.u.t_audio_mpeg.e_layer = 
                    pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_i].t_stm_atrbt.u.t_ps_audio_stm_attr.e_layer;

                t_scdb_info.t_scdb_rec.u.t_audio_mpeg.e_aud_fmt = AUD_FMT_UNKNOWN;
                
                t_scdb_info.t_scdb_rec.u.t_audio_mpeg.ui1_index = 0;
                
                t_scdb_info.t_scdb_rec.u.t_audio_ps.b_default = TRUE;
            }
            else if(pt_mm_stm_atrbt->e_type == ST_SUBTITLE)
            {
                t_scdb_info.t_scdb_rec.e_rec_type = SCDB_REC_TYPE_SUBTITLE_DVB;

                x_memcpy(&(t_scdb_info.t_scdb_rec.u.t_dvb_sbtl), 
                         &(pt_media_desc->pt_strm_info->at_stm_atrbt[ui1_i].t_stm_atrbt.u.t_cmpb_subtl_stm_attr),
                         sizeof(MINFO_CMPB_SUBTL_STM_ATRBT_T));
            }
        }
        
        if (pt_running_svc_req->pt_player)
        {
            i4_ret = x_cm_set(pt_running_svc_req->pt_player->h_conn,
                              PLAYBACK_CTRL_SET_MODIFY_SCDB,
                              (VOID*) & t_scdb_info);

            if (i4_ret != CMR_OK)
            {
                return SVCTXR_FAIL;
            }
        }
        else
        {
            return SVCTXR_FAIL;
        }
    }
    else
    {
        return i4_ret;
    }

    return SVCTXR_OK;
}

INT32 mm_hdlr_set_seek(
                    SVCTX_T*                    pt_svctx,
                    VOID*                       pv_set_info,
                    SIZE_T                      z_set_info_size
                    )
{
    MM_SVC_REQ_T*                       pt_running_svc_req = NULL;
    MEDIA_DESCRIBT_T*                   pt_media_desc = NULL;
    PLAYER_T*                           pt_player = NULL;
    INT32                               i4_set_seek = SVCTXR_OK;
    SIZE_T                              z_get_info_size = sizeof(SM_SESS_GNRC_UOP_CAP_INFO_T);
    SM_SESS_GNRC_UOP_CAP_INFO_T         t_uop_cap_info;
    INT32                               i4_ret = SVCTXR_OK;
    MM_RANGE_POS_INFO_T                 t_set_info;
    MM_SVC_DUR_INFO_T                   t_dur_code;
    MM_SVC_UOP_HANDLER_FCT_TBL_T        t_fct_tbl;
    MM_SPEED_TYPE_T                     e_speed_type;
    MM_PLAY_MODE_PL_T  t_mm_play_mode;
    /* Check Parameters*/
    if (pt_svctx == NULL || pv_set_info == NULL || z_set_info_size != sizeof(MM_SVC_POS_INFO_T))
    {
        return SVCTXR_INV_ARG;
    }
    /* initialize variable */
    x_memset(&t_uop_cap_info,0,sizeof(SM_SESS_GNRC_UOP_CAP_INFO_T));
    x_memset(&t_set_info,0,sizeof(MM_RANGE_POS_INFO_T));    
    x_memset(&t_dur_code,0,sizeof(MM_SVC_DUR_INFO_T));
    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    t_fct_tbl          = pt_running_svc_req->t_uop_fct_tbl;
    pt_player          = pt_running_svc_req->pt_player;
    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc,&pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }   
    
    if(pt_media_desc->b_is_seeking)
    {
        return SVCTXR_FAIL;
    }
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
    if (pt_media_desc->b_is_bad_intlevd)        
    {            
        return SVCTXR_FAIL;        
    }
#endif

    if (pt_running_svc_req->pt_player->e_sess_cond !=SM_SESS_COND_STARTED)
    {
        return SVCTXR_WRONG_STATE;
    }

    /*check uop capability first*/
    //for seek with offset & pts, do not check uop
    DBG_INFO((_INFO_HEADER"type = %d,  pos_val = %llu\n", ((MM_SVC_POS_INFO_T*)pv_set_info)->t_pos_type, ((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val));
    if ((((MM_SVC_POS_INFO_T*)pv_set_info)->t_pos_type != MM_SVC_POS_TYPE_ALL_OFFSET) ||
        (((MM_SVC_POS_INFO_T*)pv_set_info)->u.render_info.ui8_pts_info == 0))
    {
        t_uop_cap_info.e_uop = _SM_SESS_GNRC_UOP_SEEK;
        t_uop_cap_info.u.t_speed = pt_media_desc->e_speed;
        i4_ret = x_sm_get(pt_player->h_sess,
                      SM_SESS_GNRC_GET_TYPE_UOP_CAP,
                      (VOID*)&t_uop_cap_info,
                      &z_get_info_size);
        if((i4_ret != SVCTXR_OK) || !t_uop_cap_info.b_is_allowed)
        {
            x_dbg_stmt(_ERROR_HEADER"get seek capability fail ret=%d,b_is_allow=%d\n",i4_ret,t_uop_cap_info.b_is_allowed);
            return SVCTXR_FAIL;
        }
    }

    if (((MM_SVC_POS_INFO_T*)pv_set_info)->t_pos_type ==  MM_SVC_POS_TYPE_ALL_TIME)
    {
#if MM_AB_REPEAT_SUPPORT
        if(pt_media_desc->t_repeat_ab_info.b_set_repeat_a &&
           pt_media_desc->t_repeat_ab_info.b_set_repeat_b)
        {
            UINT64 ui8_a_time = 0;
            UINT64 ui8_b_time = 0;
            if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream))
            {
                if (pt_media_desc->t_media_fmt.e_media_type != MEDIA_TYPE_VIDEO_ES)
                {
                    
                    ui8_a_time = _mm_div64(pt_media_desc->t_repeat_ab_info.t_repeat_info_a.u.render_info.ui8_pts_info, 
                                           90000, 
                                           NULL);
                    ui8_b_time = _mm_div64(pt_media_desc->t_repeat_ab_info.t_repeat_info_b.u.render_info.ui8_pts_info, 
                                           90000, 
                                           NULL);
                }
            }
            else
            {
                ui8_a_time = pt_media_desc->t_repeat_ab_info.t_repeat_info_a.ui8_pos_val;
                ui8_b_time = pt_media_desc->t_repeat_ab_info.t_repeat_info_b.ui8_pos_val;
            }
            if(((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val > ui8_b_time || 
               ((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val < ui8_a_time)
            {
                x_dbg_stmt(_ERROR_HEADER" seek time is out of ab repeat range!\n");
                return SVCTXR_FAIL;
            }
        }
#endif
        t_set_info.e_pos_type =  MM_RANGE_POS_TYPE_TIME_RANGE_CURR;
        if (((pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER) && 
             (pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MP4)) || 
            (MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt)))
        {
            t_set_info.e_pos_type =  MM_RANGE_POS_TYPE_TIME_MEDIA;
        }
    }
    else if (((MM_SVC_POS_INFO_T*)pv_set_info)->t_pos_type ==  MM_SVC_POS_TYPE_ALL_OFFSET)
    {
        /*add cmpb pts seek*/
        if(((MM_SVC_POS_INFO_T*)pv_set_info)->u.render_info.ui8_pts_info == 0)
        {
            t_set_info.e_pos_type =  MM_RANGE_POS_TYPE_OFFSET_RANGE_CURR;            
        }
        else
        {
            if(((MM_SVC_POS_INFO_T*)pv_set_info)->u.render_info.ui8_pts_info == (UINT64)-1)
            {
                ((MM_SVC_POS_INFO_T*)pv_set_info)->u.render_info.ui8_pts_info = 0;
            }
            ((MM_SVC_POS_INFO_T*)pv_set_info)->t_pos_type = MM_SVC_POS_TYPE_RANGE_AB_START;
            t_set_info.ui4_idx = 0;
            t_set_info.ui8_pos_val = 0;
            t_set_info.e_pos_type = MM_RANGE_POS_TYPE_RANGE_AB_START;  
            t_set_info.t_start_pos.i4_temporal_reference
                = ((MM_SVC_POS_INFO_T*)pv_set_info)->u.render_info.i4_temporal_reference;
            t_set_info.t_start_pos.ui8_frame_position
                = ((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val;
            t_set_info.t_start_pos.ui8_frame_position_disp
                = ((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val;
            t_set_info.t_start_pos.ui8_i_frame_position
                = ((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val;
            t_set_info.t_start_pos.ui8_i_pts_info
                = ((MM_SVC_POS_INFO_T*)pv_set_info)->u.render_info.ui8_pts_info;
            t_set_info.t_start_pos.ui8_pts_info
                = ((MM_SVC_POS_INFO_T*)pv_set_info)->u.render_info.ui8_pts_info;
            t_set_info.t_start_pos.ui2_decoding_order
                = ((MM_SVC_POS_INFO_T*)pv_set_info)->u.render_info.ui2_decoding_order;
            t_set_info.t_start_pos.ui8_aud_pts_info
                = ((MM_SVC_POS_INFO_T*)pv_set_info)->u.render_info.ui8_pts_info;
            t_set_info.t_start_pos.ui8_aud_frame_position
                = ((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val;
            t_set_info.t_start_pos.ui8_stc
                = ((MM_SVC_POS_INFO_T*)pv_set_info)->u.render_info.ui8_pts_info;

            x_dbg_stmt("\n==========CMPBADDAPI================ui8_pts_info:%llu\n",
            t_set_info.t_start_pos.ui8_pts_info);
            
            /*set seek to sess*/
            i4_ret = x_sm_set(pt_player->h_sess,
                              SM_SESS_GNRC_SET_TYPE_SEEK_WITH_PTS,
                              (VOID*)(((MM_RANGE_POS_INFO_T*)&t_set_info)),
                              sizeof(MM_RANGE_POS_INFO_T));
            DBG_INFO((_INFO_HEADER"set seek to sess ret=%d\n",i4_ret));
            if (i4_ret != SMR_OK)
            {
                x_dbg_stmt("x_sm_set(SM_SESS_GNRC_SET_TYPE_SEEK) failed, ret = %d \n", i4_ret);
                i4_set_seek =  SVCTXR_FAIL;
            }
            else
            {
                pt_media_desc->b_is_seeking = TRUE;
            }

            x_dbg_stmt("mm_Hdlr: byteseek with pts: current speed = %d \n", pt_media_desc->e_speed);

            /*set speed to session*/
            e_speed_type = pt_media_desc->e_speed;
            /* only do STEP for video stream exists case */            

            if((e_speed_type != MM_SPEED_TYPE_PAUSE)
		       && (e_speed_type != MM_SPEED_TYPE_STEP))
	    {
        	    t_mm_play_mode.eSpeed = e_speed_type;
	            i4_ret = x_sm_set(pt_player->h_sess,
	                              SM_SESS_GNRC_SET_TYPE_SPEED_WITH_PTS, 
	                              (VOID*)&t_mm_play_mode,
                                   sizeof(MM_PLAY_MODE_PL_T));
	            DBG_INFO((_INFO_HEADER"set speed to sess MM_SPEED_TYPE_PAUSE ret=%d\n",i4_ret));
	            if (i4_ret != SMR_OK)
	            {
	                x_dbg_stmt("x_sm_set(SM_SESS_GNRC_SET_TYPE_SPEED) failed, ret = %d \n", i4_ret);
	                i4_ret =  SVCTXR_FAIL;
	            }
	
	            i4_ret = mm_hdlr_strm_trick_req(pt_svctx,
	                                            ST_VIDEO,
	                                            e_speed_type,
	                                            FALSE);
	            DBG_INFO((_INFO_HEADER"mm_hdlr_strm_trick_req(ST_VIDEO) e_speed_type=%d \n",((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type));
	            if (i4_ret != SVCTXR_OK)
	            {
	                i4_ret = SVCTXR_FAIL;
	                x_dbg_stmt("mm_hdlr_strm_trick_req video failed, ret = %d \r\n", i4_ret);
	            }
	
	            i4_ret = mm_hdlr_strm_trick_req(pt_svctx,
	                                            ST_AUDIO,
	                                            e_speed_type,
	                                            FALSE);
	            DBG_INFO((_INFO_HEADER"mm_hdlr_strm_trick_req(ST_VIDEO) e_speed_type=%d \n",((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type));
	            if (i4_ret != SVCTXR_OK)
	            {
	                x_dbg_stmt("mm_hdlr_strm_trick_req audio failed, ret = %d \r\n", i4_ret);
	                i4_ret = SVCTXR_FAIL;
	            }
	        }
	        else
        	{
        		if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream))
		        {
		            /*set speed to session*/              
		            e_speed_type = MM_SPEED_TYPE_STEP;
                    t_mm_play_mode.eSpeed = e_speed_type;
		            i4_ret = x_sm_set(pt_player->h_sess,
		                              SM_SESS_GNRC_SET_TYPE_SPEED_WITH_PTS,
		                              (VOID*)&t_mm_play_mode,
                                   sizeof(MM_PLAY_MODE_PL_T));
		            DBG_INFO((_INFO_HEADER"set speed to sess e_speed_type=%d\n",e_speed_type));
		            if (i4_ret != SMR_OK)
		            {
		                x_dbg_stmt(_ERROR_HEADER"x_sm_set(SM_SESS_GNRC_SET_TYPE_SPEED) failed, ret = %d)\r\n", i4_ret);
		                i4_ret =  SVCTXR_FAIL;
		            }
		            mm_hdlr_strm_trick_req(pt_svctx,
		                                   ST_VIDEO,
		                                   e_speed_type,
		                                   FALSE);
		            DBG_INFO((_INFO_HEADER"mm_hdlr_strm_trick_req(ST_VIDEO) e_speed_type=%d\n",e_speed_type));
		        }
        	}
            
            return i4_ret;
        }
    }
#ifdef DIVX_PLUS_CER
    else if(((MM_SVC_POS_INFO_T*)pv_set_info)->t_pos_type ==  MM_SVC_POS_TYPE_CHAPTER)
    {
        MM_SVC_UOP_HANDLER_FCT_TBL_T    t_fct_tbl = pt_running_svc_req->t_uop_fct_tbl;
        MM_SVC_CUR_CHAP_ID_T            t_chap_id;
        MINFO_CHAP_INFO_T               t_chap;
        MINFO_CHAP_INFO_T               t_chap_next;

        x_memset(&pt_media_desc->t_mm_edition_last_info, 0, sizeof(MM_SVC_STOP_INFO));    
        t_chap_id.ui4_chapter_id = MM_DIVX_PLUS_POS_TO_CHAP_ID(((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val);
        t_chap_id.ui4_playlist_id = MM_DIVX_PLUS_POS_TO_PL_ID(((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val);
        if(t_fct_tbl.pf_set_cur_chap == NULL || 
           t_fct_tbl.pf_get_cur_chap == NULL || 
           pt_media_desc->b_is_seeking)
        {
            return SVCTXR_FAIL;
        }
#ifdef MM_LAST_MEMORY_SUPPORT
        if((t_chap_id.ui4_playlist_id != pt_media_desc->ui4_playlist_idx) && (pt_media_desc->ui4_playlist_idx < 3))
        {
            SIZE_T  z_info_len = 0;
            
            z_info_len = sizeof(MM_SVC_STOP_INFO);
            i4_ret = mm_hdlr_get(pt_svctx,
                    SVCTX_MM_GET_TYPE_POS_PTS,
                    &pt_media_desc->t_mm_playlist_last_info[pt_media_desc->ui4_playlist_idx].t_mm_svc_pos_info,
                    &z_info_len);
            if (i4_ret != SVCTXR_OK)
            {
                x_memset(&pt_media_desc->t_mm_playlist_last_info[pt_media_desc->ui4_playlist_idx], 0, sizeof(MM_SVC_STOP_INFO));
            }
            pt_media_desc->t_mm_playlist_last_info[pt_media_desc->ui4_playlist_idx].ui4_chap_idx = pt_media_desc->ui4_chap_idx;
            pt_media_desc->t_mm_edition_last_info.t_mm_svc_pos_info = pt_media_desc->t_mm_playlist_last_info[t_chap_id.ui4_playlist_id].t_mm_svc_pos_info;
            t_chap_id.ui4_chapter_id = pt_media_desc->t_mm_playlist_last_info[t_chap_id.ui4_playlist_id].ui4_chap_idx;
        }
#endif

        i4_ret = t_fct_tbl.pf_set_cur_chap(pt_media_desc, FALSE, &t_chap_id);
        if(i4_ret == SVCTXR_OK)
        {
            i4_ret = t_fct_tbl.pf_get_cur_chap(pt_media_desc, 0, &t_chap);
            if(i4_ret == SVCTXR_OK)
            {
                MM_SVC_POS_INFO_T t_pos_info;
                VSH_CHAP_INFO_T t_vset_info = {0};
                HANDLE_T h_stream = pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream;

                if(x_handle_valid(h_stream))
                {
                    t_vset_info.b_seek = FALSE;
                    t_vset_info.b_first = FALSE;
                    t_vset_info.ui8_start_time = 0;
                    t_vset_info.ui8_end_time = 0xFFFFFFFFFFFFFFFF;
                    i4_ret = x_sm_set(h_stream,SM_VSH_SET_TYPE_CHAPTER,(VOID*)&t_vset_info,(SIZE_T)sizeof(VSH_CHAP_INFO_T));
                }
                
                t_pos_info.t_pos_type = MM_SVC_POS_TYPE_ALL_TIME;
                if (pt_media_desc->t_mm_edition_last_info.t_mm_svc_pos_info.u.render_info.ui8_stc != 0)
                {
                    t_pos_info.ui8_pos_val = _mm_div64(pt_media_desc->t_mm_edition_last_info.t_mm_svc_pos_info.u.render_info.ui8_stc, (UINT64)90000, NULL);
                }
                else
                {
                    t_pos_info.ui8_pos_val = t_chap.ui8_start_time;
                }
                pt_media_desc->b_divxplus_seek = TRUE;
                i4_ret = _mm_hdlr_set(
                                pt_svctx,
                                SVCTX_MM_SET_TYPE_SEEK,
                                (VOID*)&t_pos_info,
                                sizeof(MM_SVC_POS_INFO_T)
                                );
                pt_media_desc->b_divxplus_seek = FALSE;
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
                            t_vset_info.b_seek = TRUE;
                        }
                        else
                        {
                            t_vset_info.b_seek = FALSE;
                        }
                        t_vset_info.b_first = FALSE;
                        t_vset_info.ui8_start_time = t_chap.ui8_start_time;
                        t_vset_info.ui8_end_time = t_chap.ui8_end_time;
                        i4_ret = x_sm_set(h_stream,SM_VSH_SET_TYPE_CHAPTER,(VOID*)&t_vset_info,(SIZE_T)sizeof(VSH_CHAP_INFO_T));
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
                    pt_media_desc->b_is_seeking = TRUE;
                }
            }
        }
        return i4_ret;
    }
#endif
#if MM_AB_REPEAT_SUPPORT
    else if (((MM_SVC_POS_INFO_T*)pv_set_info)->t_pos_type == MM_SVC_POS_TYPE_RANGE_AB_START)
    {
        t_set_info.ui4_idx = 0;
        t_set_info.ui8_pos_val = 0;
        t_set_info.e_pos_type = MM_RANGE_POS_TYPE_RANGE_AB_START;  
        t_set_info.t_start_pos.i4_temporal_reference
            = ((MM_SVC_POS_INFO_T*)pv_set_info)->u.render_info.i4_temporal_reference;
        t_set_info.t_start_pos.ui8_frame_position
            = ((MM_SVC_POS_INFO_T*)pv_set_info)->u.render_info.ui8_frame_position;
        t_set_info.t_start_pos.ui8_frame_position_disp
            = ((MM_SVC_POS_INFO_T*)pv_set_info)->u.render_info.ui8_frame_position_disp;
        t_set_info.t_start_pos.ui8_i_frame_position
            = ((MM_SVC_POS_INFO_T*)pv_set_info)->u.render_info.ui8_i_frame_position;
        t_set_info.t_start_pos.ui8_i_pts_info
            = ((MM_SVC_POS_INFO_T*)pv_set_info)->u.render_info.ui8_i_pts_info;
        t_set_info.t_start_pos.ui8_pts_info
            = ((MM_SVC_POS_INFO_T*)pv_set_info)->u.render_info.ui8_pts_info;
        t_set_info.t_start_pos.ui2_decoding_order
            = ((MM_SVC_POS_INFO_T*)pv_set_info)->u.render_info.ui2_decoding_order;
        t_set_info.t_start_pos.ui8_aud_pts_info
            = ((MM_SVC_POS_INFO_T*)pv_set_info)->u.render_info.ui8_aud_pts_info;
        t_set_info.t_start_pos.ui8_aud_frame_position
            = ((MM_SVC_POS_INFO_T*)pv_set_info)->u.render_info.ui8_aud_frame_position;
        t_set_info.t_start_pos.ui8_stc
            = ((MM_SVC_POS_INFO_T*)pv_set_info)->u.render_info.ui8_stc;
        t_set_info.t_start_pos.ui4_timestap
            = ((MM_SVC_POS_INFO_T*)pv_set_info)->u.render_info.ui4_timestap;
    }
#endif
    else
    {
        i4_ret =  SVCTXR_FAIL;
    }
    //convert edition time to chapter time
#ifdef DIVX_PLUS_CER        
    if (!pt_media_desc->b_divxplus_seek &&
        t_fct_tbl.pf_set_cur_chap != NULL && 
        t_fct_tbl.pf_get_cur_chap != NULL && 
        !pt_media_desc->b_is_seeking &&
        ((MM_SVC_POS_INFO_T*)pv_set_info)->t_pos_type ==  MM_SVC_POS_TYPE_ALL_TIME)
    {
   
        MM_SVC_TIME_CODE_INFO_T  pt_time_info;
        MM_SVC_CUR_CHAP_ID_T t_chap_id;
        MINFO_CHAP_INFO_T t_chap;
        MINFO_CHAP_INFO_T t_chap_next;

        pt_media_desc->b_divxplus_seek = FALSE;
        pt_time_info.t_tm_code_type = MM_SVC_TM_CODE_TYPE_CHAPTER;
        pt_time_info.ui8_time = ((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val;
        i4_ret = t_fct_tbl.pf_get_tm(pt_media_desc,&pt_time_info);
        if (i4_ret == SVCTXR_OK)
        {
            ((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val = pt_time_info.ui8_time;
            
            i4_ret = t_fct_tbl.pf_get_cur_chap(pt_media_desc, 0, &t_chap);
            if (i4_ret == SVCTXR_OK)
            {
                t_chap_id.ui4_playlist_id = t_chap.ui4_playlist_idx;
                t_chap_id.ui4_chapter_id = pt_time_info.ui2_idx;
                i4_ret = t_fct_tbl.pf_set_cur_chap(pt_media_desc, FALSE, &t_chap_id);
                if (i4_ret != SVCTXR_OK)
                {
                    i4_ret =  SVCTXR_FAIL;
                }
            }
        } 
        if (i4_ret == SVCTXR_OK)
        {
            VSH_CHAP_INFO_T t_vset_info = {0};
            MM_SVC_POS_INFO_T t_pos_info;
            HANDLE_T h_stream = pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream;

            if(x_handle_valid(h_stream))
            {
                i4_ret = t_fct_tbl.pf_get_cur_chap(pt_media_desc, 0, &t_chap);
                if (i4_ret == SVCTXR_OK)
                {
                    if(x_handle_valid(h_stream))
                    {
                        t_vset_info.b_seek = FALSE;
                        t_vset_info.b_first = FALSE;
                        t_vset_info.ui8_start_time = 0;
                        t_vset_info.ui8_end_time = 0xFFFFFFFFFFFFFFFF;
                        i4_ret = x_sm_set(h_stream,SM_VSH_SET_TYPE_CHAPTER,(VOID*)&t_vset_info,(SIZE_T)sizeof(VSH_CHAP_INFO_T));
                    }
                    
                    t_pos_info.t_pos_type = MM_SVC_POS_TYPE_ALL_TIME;
                    t_pos_info.ui8_pos_val = pt_time_info.ui8_time;
                    pt_media_desc->b_divxplus_seek = TRUE;
                    i4_ret = _mm_hdlr_set(
                                    pt_svctx,
                                    SVCTX_MM_SET_TYPE_SEEK,
                                    (VOID*)&t_pos_info,
                                    sizeof(MM_SVC_POS_INFO_T)
                                    );
                    pt_media_desc->b_divxplus_seek = FALSE;
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
                                t_vset_info.b_seek = TRUE;
                            }
                            else
                            {
                                t_vset_info.b_seek = FALSE;
                            }
                            t_vset_info.b_first = FALSE;
                            t_vset_info.ui8_start_time = t_chap.ui8_start_time;
                            t_vset_info.ui8_end_time = t_chap.ui8_end_time;
                            i4_ret = x_sm_set(h_stream,SM_VSH_SET_TYPE_CHAPTER,(VOID*)&t_vset_info,(SIZE_T)sizeof(VSH_CHAP_INFO_T));
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
                        pt_media_desc->b_is_seeking = TRUE;
                    }
                }
            }
        }
    }
#endif
#ifdef TIME_SHIFT_SUPPORT
    if (MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt))
    {
        t_set_info.ui4_idx = 0;
        if (((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val <= (UINT64)pt_media_desc->ui4_tick_num_safe_start)
        {
            if (pt_media_desc->e_speed <= MM_SPEED_TYPE_REWIND_1X)
            {
                return SVCTXR_NOT_SUPPORT;
            }
            t_set_info.ui8_pos_val = (UINT64)pt_media_desc->ui4_tick_num_safe_start;
        }
        else if (((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val >= (UINT64)pt_media_desc->ui4_tick_num_safe_end)
        {
            if (pt_media_desc->e_speed > MM_SPEED_TYPE_FORWARD_1X)
            {
                return SVCTXR_NOT_SUPPORT;
            }
            t_set_info.ui8_pos_val = (UINT64)pt_media_desc->ui4_tick_num_safe_end;
        }
        else
        {
            t_set_info.ui8_pos_val = ((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val;
        }
        
        t_set_info.ui8_pos_val = (UINT64)PVR_GET_TICK_IDX_BY_TICK_NUM(&pt_media_desc->t_rec_tick_ctrl_blk, 
                                                                      (UINT32)t_set_info.ui8_pos_val);
        
        DBG_INFO((_INFO_HEADER"tshift seek to #%u\r\n", (UINT32)t_set_info.ui8_pos_val));
    }
    else
#endif /* TIME_SHIFT_SUPPORT */
        if (MM_IS_TIME_SHIFT_CMPB(pt_media_desc->t_media_fmt))
        {
            t_set_info.e_pos_type = MM_RANGE_POS_TYPE_TIME_MEDIA;
            t_set_info.ui8_pos_val = ((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val;
        }
#if MM_AB_REPEAT_SUPPORT
        else if (((MM_SVC_POS_INFO_T*)pv_set_info)->t_pos_type !=  MM_SVC_POS_TYPE_RANGE_AB_START)
#endif
    {
        t_dur_code.t_dur_type = MM_SVC_DUR_TYPE_ALL;
        t_dur_code.ui2_idx = (UINT16)0;
        i4_ret = t_fct_tbl.pf_get_dur(pt_media_desc, &t_dur_code);
        if (i4_ret != SVCTXR_OK)
        {
           i4_ret =  SVCTXR_FAIL;     
        }

        t_set_info.ui4_idx = ((MM_SVC_POS_INFO_T*)pv_set_info)->ui2_idx;

        /*for mp3 has no index table case, seek with postion*/
        if(t_dur_code.t_dur_type == MM_SVC_DUR_TYPE_ALL_OFFSET)
        {
            UINT64  ui8_start_offset;
            MM_SVC_TIME_CODE_INFO_T t_tm_code;
            
            t_tm_code.t_tm_code_type = MM_SVC_TM_CODE_TYPE_ALL;
            t_tm_code.ui2_idx = 0;
            i4_ret = t_fct_tbl.pf_get_tm(pt_media_desc,&t_tm_code);
            if (i4_ret != SVCTXR_OK && i4_ret != SVCTXR_GET_POS_AS_TIME)
            {
                return i4_ret;
            }

            ui8_start_offset = t_tm_code.ui8_time;
            t_dur_code.ui8_duration = _mm_div64(t_dur_code.ui8_duration, 90000, NULL);
            
            ((MM_SVC_POS_INFO_T*)pv_set_info)->t_pos_type = MM_SVC_POS_TYPE_ALL_OFFSET;
            if(t_dur_code.ui8_duration >= 2*3600 && /*larger than 2hours*/
                ((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val < t_dur_code.ui8_duration) 
            {
                ((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val += 1;
            }
            ((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val = 
                ((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val * 
                 (pt_media_desc->ui8_media_sz - ui8_start_offset);
            ((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val = 
                _mm_div64(((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val, 
                           t_dur_code.ui8_duration, NULL);
            ((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val += ui8_start_offset;

            t_set_info.e_pos_type =  MM_RANGE_POS_TYPE_OFFSET_RANGE_CURR;
        }

        if (((MM_SVC_POS_INFO_T*)pv_set_info)->t_pos_type !=  MM_SVC_POS_TYPE_ALL_OFFSET)
        {
            t_set_info.ui8_pos_val = 
                ((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val * 90000;
            if (t_dur_code.ui8_duration <= t_set_info.ui8_pos_val)
            {
                t_set_info.ui8_pos_val = t_dur_code.ui8_duration;
            }
        }
        else
        {
            t_set_info.ui8_pos_val = ((MM_SVC_POS_INFO_T*)pv_set_info)->ui8_pos_val;
        }
    }

    t_set_info.e_auto_speed = pt_media_desc->e_speed;

    /*stop audio stream*/
    if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream))
    {
        i4_ret = x_sm_sync_stop_stream(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream, 3000);
        DBG_INFO((_INFO_HEADER"x_sm_stop_stream(ST_AUDIO) ret=%d\n",i4_ret));
        /*CR DTV00329257 add SMR_CAN_NOT_OP_IN_THIS_STATE*/
        if (i4_ret != SMR_OK && i4_ret != SMR_ALREADY_STOPPED && i4_ret != SMR_CAN_NOT_OP_IN_THIS_STATE)
        {
            x_dbg_stmt(_ERROR_HEADER"x_sm_stop_stream(ST_AUDIO) failed, ret = %d)\n", i4_ret);
            return SVCTXR_FAIL;
        }
        pt_media_desc->b_set_audio_stop = TRUE;
    }
    /*stop subtitle stream*/
    if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream) &&
        x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream))
    {
        i4_ret = x_sm_stop_stream(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream);
        DBG_INFO((_INFO_HEADER"x_sm_stop_stream(ST_SUBTITLE) ret=%d\n",i4_ret));
        if (i4_ret != SMR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"x_sm_stop_stream(ST_SUBTITLE) failed, ret = %d)\n", i4_ret);
            i4_ret =  SVCTXR_FAIL;
        }
    }

    /*set pause to session*/
    e_speed_type = MM_SPEED_TYPE_PAUSE;
	t_mm_play_mode.eSpeed = MM_SPEED_TYPE_PAUSE;
	i4_ret = mm_hdlr_get_play_mode(t_mm_play_mode.eSpeed, pt_running_svc_req, &t_mm_play_mode.ePlayMode);
	if (i4_ret != SVCTXR_OK)
    {
    	x_dbg_stmt("\n------get play mode fail! i4_ret = %d--------\n",i4_ret);
    }
    
    if (pt_media_desc->e_speed != MM_SPEED_TYPE_PAUSE)
    {
        i4_ret = x_sm_set(pt_player->h_sess,
                          SM_SESS_GNRC_SET_TYPE_SPEED,
                          (VOID*)&t_mm_play_mode,
                          sizeof(MM_PLAY_MODE_PL_T));
        DBG_INFO((_INFO_HEADER"set_speed_to_sess(MM_SPEED_TYPE_PAUSE) ret=%d \n",i4_ret));
        if (i4_ret != SMR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"x_sm_set(SM_SESS_GNRC_SET_TYPE_SPEED_PAUSE) failed, ret = %d)\r\n", i4_ret);
            i4_ret =  SVCTXR_FAIL;
        }
    }

    /*set seek to sess*/
    i4_ret = x_sm_set(pt_player->h_sess,
                      SM_SESS_GNRC_SET_TYPE_SEEK,
                      (VOID*)(((MM_RANGE_POS_INFO_T*)&t_set_info)),
                      sizeof(MM_RANGE_POS_INFO_T));
    DBG_INFO((_INFO_HEADER"set seek to sess ret=%d\n",i4_ret));
    if (i4_ret != SMR_OK)
    {
        x_dbg_stmt(_ERROR_HEADER"x_sm_set(SM_SESS_GNRC_SET_TYPE_SEEK) failed, ret = %d)\r\n", i4_ret);
        i4_set_seek =  SVCTXR_FAIL;
    }
    else
    {
        pt_media_desc->b_is_seeking = TRUE;
    }   
    
    /*restore the speed setting*/
    e_speed_type = pt_media_desc->e_speed;

    if((e_speed_type != MM_SPEED_TYPE_PAUSE)
       && (e_speed_type != MM_SPEED_TYPE_STEP))
    {
        if (e_speed_type == MM_SPEED_TYPE_FORWARD_1X)
        {
            e_speed_type = MM_SPEED_TYPE_FORWARD_1X;
            mm_hdlr_strm_trick_req(pt_svctx, ST_VIDEO, e_speed_type, FALSE);
            DBG_INFO((_INFO_HEADER"mm_hdlr_strm_trick_req(ST_VIDEO) e_speed_type=MM_SPEED_TYPE_FORWARD_1X\n"));

            if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream))
            {
                SM_SESS_STC_AV_SYNC_MODE_T e_av_sync_mode;
                SM_SESS_GNRC_DATA_T *           pt_sess_data;

                pt_sess_data = (SM_SESS_GNRC_DATA_T*)(pt_media_desc->pv_sess_data);
                if((pt_sess_data && pt_sess_data->b_disable_av_sync && MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt) ) ||                          
                    !MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt) )                    
                {
                    e_av_sync_mode = SM_SESS_STC_AV_SYNC_MODE_AUDIO_MASTER;
                    i4_ret = x_sm_set(pt_player->h_sess,                                  
                        SM_SESS_GNRC_SET_TYPE_AV_SYNC_EX,                                  
                        (VOID*)(&e_av_sync_mode),                                  
                        sizeof(SM_SESS_STC_AV_SYNC_MODE_T));                         
                    if (i4_ret != SMR_OK)                        
                    {                            
                        return SVCTXR_FAIL;      
                    }
                }
                i4_ret = x_sm_sync_play_stream(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream, 3000);
                DBG_INFO((_INFO_HEADER"x_sm_play_stream(ST_AUDIO) ret=%d\n",i4_ret));
                if (i4_ret != SMR_OK && i4_ret != SMR_ALREADY_STOPPED)
                {
                    x_dbg_stmt(_ERROR_HEADER"x_sm_play_stream(ST_AUDIO) failed, ret = %d)\r\n", i4_ret);
                    return SVCTXR_FAIL;
                }
                pt_media_desc->b_set_audio_stop = FALSE;
            }
        }
    }
    else
    {
        /* only do STEP for video stream exists case */            
        if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream))
        {
            /*set speed to session*/              
        	t_mm_play_mode.eSpeed = MM_SPEED_TYPE_STEP;
            e_speed_type = MM_SPEED_TYPE_STEP;
        	i4_ret = mm_hdlr_get_play_mode(t_mm_play_mode.eSpeed, pt_running_svc_req, &t_mm_play_mode.ePlayMode);
        	if (i4_ret != SVCTXR_OK)
            {
            	x_dbg_stmt("\n------get play mode fail! i4_ret = %d--------\n",i4_ret);
            }

            i4_ret = x_sm_set(pt_player->h_sess,
                              SM_SESS_GNRC_SET_TYPE_SPEED,
                              (VOID*)&t_mm_play_mode,
                              sizeof(MM_PLAY_MODE_PL_T));
            DBG_INFO((_INFO_HEADER"set speed to sess e_speed_type=%d\n",e_speed_type));
            if (i4_ret != SMR_OK)
            {
                x_dbg_stmt(_ERROR_HEADER"x_sm_set(SM_SESS_GNRC_SET_TYPE_SPEED) failed, ret = %d)\r\n", i4_ret);
                i4_ret =  SVCTXR_FAIL;
            }
            mm_hdlr_strm_trick_req(pt_svctx,
                                   ST_VIDEO,
                                   e_speed_type,
                                   FALSE);
            DBG_INFO((_INFO_HEADER"mm_hdlr_strm_trick_req(ST_VIDEO) e_speed_type=%d\n",e_speed_type));
        }
    }

    /*restore speed(1x or play) must be after x_sm_play_stream audio(enable audio)*/
	t_mm_play_mode.eSpeed = e_speed_type;
	i4_ret = mm_hdlr_get_play_mode(t_mm_play_mode.eSpeed, pt_running_svc_req, &t_mm_play_mode.ePlayMode);
	if (i4_ret != SVCTXR_OK)
    {
    	x_dbg_stmt("\n------get play mode fail! i4_ret = %d--------\n",i4_ret);
    }

    i4_ret = x_sm_set(pt_player->h_sess,
                      SM_SESS_GNRC_SET_TYPE_SPEED,
                      (VOID*)&t_mm_play_mode,
                      sizeof(MM_PLAY_MODE_PL_T));
    DBG_INFO((_INFO_HEADER"set speed to e_speed_type=%d\n",e_speed_type));
    if (i4_ret != SMR_OK)
    {
        x_dbg_stmt(_ERROR_HEADER"x_sm_set(SM_SESS_GNRC_SET_TYPE_SPEED) failed, ret = %d)\r\n", i4_ret);
        return SVCTXR_FAIL;
    }
    mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx,
                                      SVCTX_NTFY_CODE_MEDIA_SPEED_UPDATE,
                                      ST_UNKNOWN);
    
    return i4_set_seek;
}

INT32 mm_hdlr_set_trick(
                    SVCTX_T*                    pt_svctx,
                    VOID*                       pv_set_info,
                    SIZE_T                      z_set_info_size
                    )
{ 
    MM_SVC_REQ_T*                       pt_running_svc_req = NULL;
    MEDIA_DESCRIBT_T*                   pt_media_desc = NULL;
    PLAYER_T*                           pt_player = NULL;
    SM_SESS_GNRC_UOP_CAP_INFO_T         t_uop_cap_info;
    MM_SPEED_TYPE_T                     e_speed_type;
    MM_SPEED_TYPE_T                     e_orig_speed = MM_SPEED_TYPE_FORWARD_1X;
    INT32                               i4_ret = SVCTXR_OK;
    SIZE_T                              z_get_info_size = sizeof(SM_SESS_GNRC_UOP_CAP_INFO_T);
#ifdef TIME_SHIFT_SUPPORT
    UINT32                              ui4_lba = 0;
    UINT32                              ui4_tick = 0;
#endif
    MM_PLAY_MODE_PL_T  t_mm_play_mode;

    /* Check Parameters*/
    if (pt_svctx == NULL || pv_set_info == NULL || z_set_info_size != sizeof(MM_SVC_TRICK_INFO_T))
    {
        return SVCTXR_INV_ARG;
    }
    DBG_INFO((_INFO_HEADER"set speed = %d\n", ((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type));
    x_memset(&t_uop_cap_info,0,sizeof(SM_SESS_GNRC_UOP_CAP_INFO_T));
    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    pt_player          = pt_running_svc_req->pt_player;
    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc,&pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }

    if((x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream)) 
        && (pt_media_desc->b_is_buff_underflow) && 
        (((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type != MM_SPEED_TYPE_PAUSE &&            
        ((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type != MM_SPEED_TYPE_FORWARD_1X))
    {
        return SVCTXR_FAIL;
    }

    if (pt_running_svc_req->pt_player->e_sess_cond !=SM_SESS_COND_STARTED)
    {
       return SVCTXR_WRONG_STATE;
    }

    if (pt_media_desc->b_is_seeking)
    {
        return SVCTXR_WRONG_STATE;
    }

#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
    if (pt_media_desc->b_is_bad_intlevd &&            
        (((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type != MM_SPEED_TYPE_PAUSE &&            
        ((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type != MM_SPEED_TYPE_FORWARD_1X))        
    {            
        return SVCTXR_WRONG_STATE;        
    }
#endif

#ifdef TIME_SHIFT_SUPPORT
    if (!MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt) &&
        !MM_IS_TIME_SHIFT_CMPB(pt_media_desc->t_media_fmt))
#endif
    {
        /*If video is not opened it should not support trick.*/
        if (!x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream))
        {
            if (!x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream))
            {
                return SVCTXR_FAIL;
            }
        }
        else if((! (pt_svctx))
                 && !svctx_is_scrambled_strm(&pt_running_svc_req->t_strm_reqs[ST_VIDEO].t_ca)
                )
        {
            return SVCTXR_FAIL;
        }
    }
#ifdef DLNA_SUPPORT
    if( (pt_media_desc->e_src_type == SRC_TYPE_MEDIA_NETWORK)
      &&(pt_media_desc->e_svc_protocol == PROTOCL_MODE_DLNA)
      &&(((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type == MM_SPEED_TYPE_PAUSE))
    {
        if(!x_fm_dlna_is_pause_supported(pt_media_desc->ps_media_path))
        {
            return SVCTXR_NOT_SUPPORT;
        }
    }
#endif

#ifdef TIME_SHIFT_SUPPORT
    if (MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt))
    {
        PVR_TICK_INDEX_T                    t_tick_idx = {0};
        UINT32                              ui4_tick_num_curr = 0;

        do {
            i4_ret = mm_hdlr_tshift_get_curr_pos(pt_svctx, &ui4_lba, &ui4_tick);
            if (i4_ret != SVCTXR_OK)
            {
                DBG_ERROR((_ERROR_HEADER"%s(L%d), mm_hdlr_tshift_get_curr_pos() failed, ret=%d\r\n", __func__, __LINE__, i4_ret));
                break;
            }

            i4_ret = rec_util_get_tick_index(pt_media_desc->h_rec_util,
                                             ui4_tick,
                                             &t_tick_idx);
            if (i4_ret != REC_UTILR_OK)
            {
                DBG_ERROR((_ERROR_HEADER"%s(L%d), rec_util_get_tick_index() failed, ret=%d\r\n", __func__, __LINE__, i4_ret));
                break;
            }
            ui4_tick_num_curr = t_tick_idx.ui4_tick_num;

            if (ui4_tick_num_curr >= pt_media_desc->ui4_tick_num_safe_end)
            {
                if (((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type > MM_SPEED_TYPE_FORWARD_1X)
                {
                    return SVCTXR_NOT_SUPPORT;
                }
            }
            else if (ui4_tick_num_curr <= pt_media_desc->ui4_tick_num_safe_start)
            {
                if (((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type < MM_SPEED_TYPE_REWIND_1X)
                {
                    return SVCTXR_NOT_SUPPORT;
                }                    
            }
        } while(0);
    }
#endif
    /*check uop capability first*/
    t_uop_cap_info.e_uop = _SM_SESS_GNRC_UOP_TRICK;
    t_uop_cap_info.u.t_speed = ((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type;
    if(x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream))
    {
        if(pt_media_desc->b_non_seekable)
        {/*When server not support seek, it only allowed these three speed*/
            if ((t_uop_cap_info.u.t_speed != MM_SPEED_TYPE_PAUSE)       && 
                (t_uop_cap_info.u.t_speed != MM_SPEED_TYPE_FORWARD_1X))
            {
                return SVCTXR_NOT_SUPPORT;
            }
        }
    }
    i4_ret = x_sm_get(pt_player->h_sess,
                      SM_SESS_GNRC_GET_TYPE_UOP_CAP,
                      (VOID*)&t_uop_cap_info,
                      &z_get_info_size);
    if((i4_ret != SVCTXR_OK) || !t_uop_cap_info.b_is_allowed)
    {
        if (FMT_IS_NEW_TRICK_CATEGORY(pt_media_desc->t_media_fmt) && 
            pt_media_desc->b_has_key_table == FALSE &&
            ((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type >= MM_SPEED_TYPE_PAUSE)
        {
            x_dbg_stmt(_INFO_HEADER"container has no indextable ! get capability fail!\n");
            return SVCTXR_NEW_TRICK_FLOW;
        }
        x_dbg_stmt(_ERROR_HEADER"get trick capability fail ret=%d,b_is_allow=%d\n",i4_ret,t_uop_cap_info.b_is_allowed);
        return SVCTXR_FAIL;
    }
    
        /*Must pause swdmx before enable audio stream(x_sm_play_stream)*/
    if(x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream))
    {
        if (pt_media_desc->e_speed != MM_SPEED_TYPE_PAUSE && 
            pt_media_desc->e_speed != MM_SPEED_TYPE_FORWARD_1X)
        {
            /*set speed to session*/
            e_speed_type = MM_SPEED_TYPE_PAUSE;
        	t_mm_play_mode.eSpeed = e_speed_type;
        	i4_ret = mm_hdlr_get_play_mode(t_mm_play_mode.eSpeed, pt_running_svc_req, &t_mm_play_mode.ePlayMode);
        	if (i4_ret != SVCTXR_OK)
            {
            	x_dbg_stmt("\n------get play mode fail! i4_ret = %d--------\n",i4_ret);
            }
            i4_ret = x_sm_set(pt_player->h_sess,
                              SM_SESS_GNRC_SET_TYPE_SPEED,
                              (VOID*)&t_mm_play_mode,
                              sizeof(MM_PLAY_MODE_PL_T));
            DBG_INFO((_INFO_HEADER"set speed to sess MM_SPEED_TYPE_PAUSE ret=%d\n",i4_ret));
            if (i4_ret != SMR_OK)
            {
                x_dbg_stmt(_ERROR_HEADER"x_sm_set(SM_SESS_GNRC_SET_TYPE_SPEED) failed, ret = %d)\r\n", i4_ret);
                i4_ret =  SVCTXR_FAIL;
            }
            pt_media_desc->e_speed = MM_SPEED_TYPE_PAUSE;
        }
        
        e_orig_speed = pt_media_desc->e_speed;
        pt_media_desc->e_speed = ((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type;
        
        do {
            i4_ret = mm_hdlr_strm_trick_req(pt_svctx,
                                            ST_VIDEO,
                                            ((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type,
                                            FALSE);
            DBG_INFO((_INFO_HEADER"mm_hdlr_strm_trick_req(ST_VIDEO) e_speed_type=%d\n",((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type));
            if (i4_ret != SVCTXR_OK)
            {
                i4_ret = SVCTXR_FAIL;
                break;
            }
            if (((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type != MM_SPEED_TYPE_PAUSE &&
                ((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type != MM_SPEED_TYPE_FORWARD_1X)
            {
                if (IS_FORWARD_DIR(e_orig_speed)&&
                    IS_BACKWARD_DIR(((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type))
                {
                    /*clear EOS flag*/
                    pt_media_desc->ui1_stream_eos_status = 0;
                }
                
                if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) &&
                    (pt_running_svc_req->t_strm_reqs[ST_AUDIO].e_strm_cond == SM_COND_STARTED))
                {
                    i4_ret = x_sm_sync_stop_stream(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream, 3000);
                    DBG_INFO((_INFO_HEADER"x_sm_stop_stream(ST_AUDIO) ret=%d\n",i4_ret));
                    if (i4_ret != SMR_OK && i4_ret != SMR_ALREADY_STOPPED)
                    {
                        i4_ret = SVCTXR_FAIL;
                        break;
                    }
                    pt_media_desc->b_set_audio_stop = TRUE;
                    
                }
                
                if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream))
                {
                    i4_ret = x_sm_stop_stream(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream);
                    DBG_INFO((_INFO_HEADER"x_sm_stop_stream(ST_SUBTITLE) ret=%d\n",i4_ret));
                    if (i4_ret != SMR_OK)
                    {
                        i4_ret = SVCTXR_FAIL;
                        break;
                    }
                }
            }
            else if (((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type == MM_SPEED_TYPE_FORWARD_1X)
            {
                /*resume audio before play it,  otherwise will cause as timing issue in linux platform*/
                if (e_orig_speed == MM_SPEED_TYPE_PAUSE
                    && !(pt_media_desc->b_set_audio_stop))
                {                    
                    mm_hdlr_strm_trick_req(pt_svctx,
                                           ST_AUDIO,
                                           ((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type, 
                                           FALSE);
                    DBG_INFO((_INFO_HEADER"mm_hdlr_strm_trick_req(ST_AUDIO) e_speed_type=%d\n",((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type));
                }
                if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream)
                    && (pt_media_desc->b_set_audio_stop))
                {
                    if(pt_running_svc_req->t_strm_reqs[ST_AUDIO].e_strm_cond != SM_COND_STARTED)
                    {
                        SM_SESS_STC_AV_SYNC_MODE_T e_av_sync_mode;
                        SM_SESS_GNRC_DATA_T *           pt_sess_data;
                        
                        pt_sess_data = (SM_SESS_GNRC_DATA_T*)(pt_media_desc->pv_sess_data);
                        if((pt_sess_data && pt_sess_data->b_disable_av_sync && MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt) ) ||                          
                            (!MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt) &&
                            !MM_IS_TIME_SHIFT_CMPB(pt_media_desc->t_media_fmt)))                    
                        {
                            e_av_sync_mode = SM_SESS_STC_AV_SYNC_MODE_AUDIO_MASTER;
                            i4_ret = x_sm_set(pt_player->h_sess,                                  
                                SM_SESS_GNRC_SET_TYPE_AV_SYNC_EX,                                  
                                (VOID*)(&e_av_sync_mode),                                  
                                sizeof(SM_SESS_STC_AV_SYNC_MODE_T));                         
                            if (i4_ret != SMR_OK)                        
                            {                            
                                i4_ret = SVCTXR_FAIL;                            
                                break;                        
                            }
                        }
                        i4_ret = x_sm_sync_play_stream(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream, 3000);
                        DBG_INFO((_INFO_HEADER"x_sm_play_stream(ST_AUDIO) ret=%d\n",i4_ret));
                        if (i4_ret != SMR_OK && i4_ret != SMR_ALREADY_STOPPED)
                        {
                            i4_ret = SVCTXR_FAIL;
                            break;
                        }
                    }
                    pt_media_desc->b_set_audio_stop = FALSE;

                    if(x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream)
                            && pt_running_svc_req->t_strm_reqs[ST_VIDEO].e_strm_cond != SM_COND_STARTED)
                    {
                        pt_media_desc->e_strm_starting = ST_AUDIO;
                    }
                }
                if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream))
                {
                    BOOL                b_set_sbtl_info;
                    SIZE_T              z_set_info_size;
                    
                    i4_ret = x_sm_play_stream(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream);
                    DBG_INFO((_INFO_HEADER"x_sm_play_stream(ST_SUBTITLE) ret=%d\n",i4_ret));
                    if (i4_ret != SMR_OK)
                    {
                        i4_ret = SVCTXR_FAIL;
                        break;
                    }
                    
                    b_set_sbtl_info = TRUE;
                    z_set_info_size = sizeof(BOOL);

                    i4_ret = x_sm_set(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream,
                                     SM_MM_SBTL_SET_TYPE_SHOW_HIDE,
                                     &b_set_sbtl_info,
                                     z_set_info_size);
                    if (i4_ret!= SMR_OK)
                    {
                        x_dbg_stmt(_ERROR_HEADER" show subtitle failed, ret = %d!\r\n", i4_ret);
                        break;
                    }
                }
            }
            else
            {
                mm_hdlr_strm_trick_req(pt_svctx,
                                       ST_AUDIO,
                                       ((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type, 
                                       FALSE);
                DBG_INFO((_INFO_HEADER"mm_hdlr_strm_trick_req(ST_AUDIO) e_speed_type=%d\n",((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type));
            }
        } while(0);                
    }
    else if(x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream))
    {
        if(((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type == MM_SPEED_TYPE_PAUSE && 
            (pt_media_desc->e_speed == MM_SPEED_TYPE_FORWARD_1X))
        {
            /*only pause in playing*/
            mm_hdlr_strm_trick_req(pt_svctx, 
                                   ST_AUDIO,
                                   ((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type, 
                                   FALSE);
                
        }
        else if(((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type == MM_SPEED_TYPE_FORWARD_1X &&
            (pt_media_desc->e_speed == MM_SPEED_TYPE_PAUSE))   /* pause to play */
        {
            if(pt_media_desc->b_set_audio_stop)
            {
                SM_SESS_STC_AV_SYNC_MODE_T e_av_sync_mode;
                SM_SESS_GNRC_DATA_T *           pt_sess_data;
                        
                pt_sess_data = (SM_SESS_GNRC_DATA_T*)(pt_media_desc->pv_sess_data);
                if((pt_sess_data && pt_sess_data->b_disable_av_sync && MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt) ) ||                          
                    !MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt) )                    
                {
                    e_av_sync_mode = SM_SESS_STC_AV_SYNC_MODE_AUDIO_MASTER;
                    i4_ret = x_sm_set(pt_player->h_sess,                                  
                        SM_SESS_GNRC_SET_TYPE_AV_SYNC_EX,                                  
                        (VOID*)(&e_av_sync_mode),                                  
                        sizeof(SM_SESS_STC_AV_SYNC_MODE_T));                         
                    if (i4_ret != SMR_OK)                        
                    {                            
                        return SVCTXR_FAIL;      
                    }
                }
                i4_ret = x_sm_sync_play_stream(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream, 3000);
                DBG_INFO((_INFO_HEADER"x_sm_play_stream(ST_AUDIO) ret=%d\n",i4_ret));
                if (i4_ret != SMR_OK && i4_ret != SMR_ALREADY_STOPPED)
                {
                    return SVCTXR_FAIL;
                }

                pt_media_desc->b_set_audio_stop = FALSE;
            }
            else
            {
                mm_hdlr_strm_trick_req(pt_svctx, 
                                       ST_AUDIO,
                                       ((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type, 
                                       FALSE);
            }

        }
        else /*it's audio trick*/
        {
            if(pt_media_desc->t_media_fmt.e_media_type != MEDIA_TYPE_AUDIO_ES && 
               ((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type != MM_SPEED_TYPE_FORWARD_1X &&
               pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype != MEDIA_CONTNR_SUBTYPE_OGG)
            {
                return SVCTXR_NOT_SUPPORT;
            }
            
            if(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_AUDIO_ES && 
               pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_WMA)                   
            {   /*if wma has no key table, do not support trick*/
                SIZE_T        z_rec_length;
                MINFO_INFO_T  t_minfo_rec;
                z_rec_length = sizeof(MINFO_INFO_T);
                i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                          MINFO_INFO_TYPE_IDX_INFO,
                                          1,
                                          &t_minfo_rec,
                                          &z_rec_length);
                if(i4_ret != SVCTXR_OK ||
                   ((t_minfo_rec.u.t_idx_info.ui4_idx_type & MM_MINFO_IDX_TYPE_NONE) ==
                            MM_MINFO_IDX_TYPE_NONE))
                {
                    return SVCTXR_NOT_SUPPORT;
                }                   
            }
            
            /*check speed is support or not*/
            if(!( (((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type == MM_SPEED_TYPE_PAUSE)      
                   ||
                   (((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type == MM_SPEED_TYPE_FORWARD_1X) 
                   ||
                   (((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type >= MM_SPEED_TYPE_REWIND_32X &&
                    ((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type <= MM_SPEED_TYPE_REWIND_2X
                   ) 
                   ||
                   (((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type >= MM_SPEED_TYPE_FORWARD_2X &&
                    ((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type <= MM_SPEED_TYPE_FORWARD_32X
                   )
                 )
              )
            {
                return SVCTXR_NOT_SUPPORT;
            }
            
            if(pt_media_desc->e_speed == MM_SPEED_TYPE_PAUSE) /*should resume audio first*/
            {
                MM_SPEED_TYPE_T e_play_speed = MM_SPEED_TYPE_FORWARD_1X;
                
                if(pt_media_desc->b_set_audio_stop)
                {
                    SM_SESS_STC_AV_SYNC_MODE_T e_av_sync_mode;
                    SM_SESS_GNRC_DATA_T *           pt_sess_data;
                        
                    pt_sess_data = (SM_SESS_GNRC_DATA_T*)(pt_media_desc->pv_sess_data);
                    if((pt_sess_data && pt_sess_data->b_disable_av_sync && MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt) ) ||                          
                        !MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt) )                    
                    {
                        e_av_sync_mode = SM_SESS_STC_AV_SYNC_MODE_AUDIO_MASTER;
                        i4_ret = x_sm_set(pt_player->h_sess,                                  
                            SM_SESS_GNRC_SET_TYPE_AV_SYNC_EX,                                  
                            (VOID*)(&e_av_sync_mode),                                  
                            sizeof(SM_SESS_STC_AV_SYNC_MODE_T));                         
                        if (i4_ret != SMR_OK)                        
                        {                            
                            return SVCTXR_FAIL;          
                        }
                    }
                    i4_ret = x_sm_sync_play_stream(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream, 3000);
                    DBG_INFO((_INFO_HEADER"x_sm_play_stream(ST_AUDIO) ret=%d\n",i4_ret));
                    if (i4_ret != SMR_OK && i4_ret != SMR_ALREADY_STOPPED)
                    {
                        return SVCTXR_FAIL;
                    }

                    pt_media_desc->b_set_audio_stop = FALSE;
                }
                else
                {
                     i4_ret = mm_hdlr_strm_trick_req(pt_svctx,
                                            ST_AUDIO,
                                            e_play_speed,
                                            FALSE); 
                     if (i4_ret != SVCTXR_OK)
                     {
                         return SVCTXR_FAIL;
                     }
                }
            }
            if((((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type >= MM_SPEED_TYPE_REWIND_32X && 
                ((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type <= MM_SPEED_TYPE_REWIND_2X))
            {
                MM_SVC_TIME_CODE_INFO_T    t_time_code;
                SIZE_T                     z_size;

                t_time_code.t_tm_code_type = MM_SVC_TM_CODE_TYPE_ALL;
                t_time_code.ui2_idx = 0;
                z_size = sizeof(t_time_code);
                i4_ret = _mm_hdlr_get(pt_svctx,
                        SVCTX_MM_GET_TYPE_TIME_CODE,
                        &t_time_code,
                        &z_size);
                if (i4_ret != SVCTXR_OK)
                {
                    return SVCTXR_FAIL;
                }
                if(t_time_code.ui8_time < 2) /*less than 1 second, do not do trick operation*/
                {
                    if(pt_media_desc->e_speed >= MM_SPEED_TYPE_REWIND_32X && 
                       pt_media_desc->e_speed <= MM_SPEED_TYPE_REWIND_2X)
                    {
                        return SVCTXR_OK;
                    }
                }
                
                i4_ret = x_sm_sync_stop_stream(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream, 3000);
                DBG_INFO(("\n[MM_HDLR_API]x_sm_stop_stream(ST_AUDIO) ret=%d\n",i4_ret));
                if (i4_ret != SMR_OK && i4_ret != SMR_ALREADY_STOPPED)
                {
                    return SVCTXR_FAIL;
                }
                pt_media_desc->b_set_audio_stop = TRUE;
                SM_SESS_STC_AV_SYNC_MODE_T e_av_sync_mode;

                SM_SESS_GNRC_DATA_T *           pt_sess_data;
                        
                pt_sess_data = (SM_SESS_GNRC_DATA_T*)(pt_media_desc->pv_sess_data);
                if((pt_sess_data && pt_sess_data->b_disable_av_sync && MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt) ) ||                          
                    !MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt) )                    
                {
                    e_av_sync_mode = SM_SESS_STC_AV_SYNC_MODE_AUDIO_MASTER;
                    i4_ret = x_sm_set(pt_player->h_sess,                                  
                        SM_SESS_GNRC_SET_TYPE_AV_SYNC_EX,                                  
                        (VOID*)(&e_av_sync_mode),                                  
                        sizeof(SM_SESS_STC_AV_SYNC_MODE_T));                         
                    if (i4_ret != SMR_OK)                        
                    {                            
                        return SVCTXR_FAIL;           
                    }
                }
                i4_ret = x_sm_sync_play_stream(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream, 3000);
                DBG_INFO(("\n[MM_HDLR_API]x_sm_play_stream(ST_AUDIO) ret=%d\n",i4_ret));
                if (i4_ret != SMR_OK && i4_ret != SMR_ALREADY_STOPPED)
                {
                    return SVCTXR_FAIL;
                }
                pt_media_desc->b_set_audio_stop = FALSE;
            }
            else
            {                    
                i4_ret = mm_hdlr_strm_trick_req(pt_svctx,
                                                ST_AUDIO,
                                                ((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type,
                                                TRUE);
            }
            if (i4_ret != SVCTXR_OK)
            {
                return SVCTXR_FAIL;
            }                
        }

        if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream) &&
            (((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type == MM_SPEED_TYPE_FORWARD_1X))
        {
            BOOL                b_set_sbtl_info;
            SIZE_T              z_set_info_size;
            
            i4_ret = x_sm_play_stream(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream);
            DBG_INFO((_INFO_HEADER"x_sm_play_stream(ST_SUBTITLE) ret=%d\n",i4_ret));
            if (i4_ret != SMR_OK)
            {
                return SVCTXR_FAIL;
            }
            
            b_set_sbtl_info = TRUE;
            z_set_info_size = sizeof(BOOL);

            i4_ret = x_sm_set(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream,
                             SM_MM_SBTL_SET_TYPE_SHOW_HIDE,
                             &b_set_sbtl_info,
                             z_set_info_size);
            if (i4_ret!= SMR_OK)
            {
                x_dbg_stmt(_ERROR_HEADER"show subtitle failed, ret = %d!\r\n", i4_ret);
                return SVCTXR_FAIL;
            }
        }
    }
    else
    {
        return SVCTXR_FAIL;
    }
    /*set speed to sess*/
	t_mm_play_mode.eSpeed = ((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type;
	i4_ret = mm_hdlr_get_play_mode(t_mm_play_mode.eSpeed, pt_running_svc_req, &t_mm_play_mode.ePlayMode);
	if (i4_ret != SVCTXR_OK)
    {
    	x_dbg_stmt("\n------get play mode fail! i4_ret = %d--------\n",i4_ret);
    }
    i4_ret = x_sm_set(pt_player->h_sess,
                  SM_SESS_GNRC_SET_TYPE_SPEED,
                  (VOID*)&t_mm_play_mode,
                  sizeof(MM_PLAY_MODE_PL_T));
    DBG_INFO((_INFO_HEADER"set speed to sess e_speed_type=%d\n",((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type));
    if (i4_ret != SMR_OK)
    {
        return SVCTXR_FAIL;
    }

    if (i4_ret == SVCTXR_OK)
    {
#ifdef TIME_SHIFT_SUPPORT
        /* if the playback direction is changed, then reset the monitor lba */
        if (MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt) &&
            (IS_BACKWARD_DIR(e_orig_speed) != IS_BACKWARD_DIR(pt_media_desc->e_speed)))
        {
            mm_hdlr_get_current_event(pt_svctx);
        }
#endif
        pt_media_desc->e_speed = ((MM_SVC_TRICK_INFO_T*)pv_set_info)->e_speed_type;

        if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)
        {
            BOOL b_block_psi = FALSE;

            /* fix trick mode NG on audio only content */
            if (TRUE == x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream))
            {
                b_block_psi = (pt_media_desc->e_speed <= MM_SPEED_TYPE_REWIND_2X) ||
                              (pt_media_desc->e_speed >= MM_SPEED_TYPE_FORWARD_2X);
            }
            i4_ret = x_sm_set(pt_player->h_sess,
                             SM_SESS_GNRC_SET_TYPE_BLOCK_PSI,
                             (void*)(UINT32)b_block_psi,
                             sizeof(BOOL));
            DBG_INFO(("\n[MM_HDLR_API]SM_SESS_GNRC_SET_TYPE_BLOCK_PSI ret=%d\n",i4_ret));
            
            if (i4_ret!= SMR_OK)
            {
                DBG_ERROR((_ERROR_HEADER"block PSI fail!\r\n"));
            }
        }

        DBG_INFO((_INFO_HEADER"SVCTX_MM_SET_TYPE_TRICK(%u)\r\n", (UINT32)pt_media_desc->e_speed));
        
#ifdef TIME_SHIFT_SUPPORT
        if (MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt))
        {
            i4_ret = mm_hdlr_tshift_get_curr_pos(pt_svctx, &ui4_lba, &ui4_tick);
            if (i4_ret == SVCTXR_OK)
            {
                x_dbg_stmt(_INFO_HEADER"%s(SVCTX_MM_SET_TYPE_TRICK), tshift tick is #%d\r\n", __func__, ui4_tick);
            }
        }
#endif
    
        mm_hdlr_invoke_client_svc_nfy_fct(pt_svctx,
                                          SVCTX_NTFY_CODE_MEDIA_SPEED_UPDATE,
                                          ST_UNKNOWN);
    }
    else
    {
        /*restore original speed*/
        pt_media_desc->e_speed = e_orig_speed;
        DBG_INFO(("mm_hdlr : set trick ERROR!\r\n"));
    }

    return SVCTXR_OK;
}

#if MM_AB_REPEAT_SUPPORT
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_set_ab_repeat
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_set_ab_repeat(
                    SVCTX_T*                    pt_svctx,
                    VOID*                       pv_set_info,
                    SIZE_T                      z_set_info_size
                    )
{
    MM_SVC_REQ_T*                       pt_running_svc_req = NULL;
    MEDIA_DESCRIBT_T*                   pt_media_desc = NULL;
    INT32                               i4_ret = SVCTXR_OK;
    SIZE_T                              z_get_info_size = sizeof(SM_SESS_GNRC_UOP_CAP_INFO_T);
    SM_SESS_GNRC_UOP_CAP_INFO_T         t_uop_cap_info;
    MM_SVC_REPEAT_INFO_T*               pt_repeat = NULL;


    /* Check Parameters*/
    if (pt_svctx == NULL || pv_set_info == NULL || z_set_info_size != sizeof(MM_SVC_REPEAT_INFO_T))
    {
        return SVCTXR_INV_ARG;
    }
    /* initialize variable */
    x_memset(&t_uop_cap_info,0,sizeof(SM_SESS_GNRC_UOP_CAP_INFO_T));
    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc,&pt_media_desc);
    if (i4_ret != SVCTXR_OK || pt_media_desc == NULL)
    {
        return i4_ret;
    }

    pt_repeat = (MM_SVC_REPEAT_INFO_T*)pv_set_info;
    if((pt_repeat != NULL) && (pt_repeat->t_repeat_type == MM_SVC_REPEAT_AB))
    {
        MM_SVC_TIME_CODE_INFO_T t_time_code;
        MM_SVC_POS_INFO_T* pt_pos_info = NULL;
        BOOL *pb_repeat_flg = NULL;
        MM_SVC_TRICK_INFO_T             t_trick_info;
        MM_SPEED_TYPE_T e_speed_type = pt_media_desc->e_speed;
        SIZE_T z_size = 0;

        if(pt_repeat == NULL || z_set_info_size != sizeof(MM_SVC_REPEAT_INFO_T))
        {
            return SVCTXR_INV_ARG;
        }
        
        
        if(!x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream))
        {
            x_dbg_stmt(_INFO_HEADER"SVCTX_MM_SET_TYPE_REPEAT: no valid video streams exist!\n");
            return SVCTXR_NOT_SUPPORT;
        }

        x_memset(&t_time_code, 0, sizeof(t_time_code));
        
        t_time_code.t_tm_code_type = MM_SVC_TM_CODE_TYPE_ALL;
        
        switch(pt_repeat->u.e_repeat_ab_type)
        {
        case MM_SVC_REPEAT_AB_TYPE_SET_A:
            if(pt_media_desc->t_repeat_ab_info.b_seeking)
            {
                return SVCTXR_WRONG_STATE;
            }
            if(pt_media_desc->b_is_seeking)
            {
                return SVCTXR_FAIL;
            }
            x_memset(&t_uop_cap_info, 0, sizeof(SM_SESS_GNRC_UOP_CAP_INFO_T));
            t_uop_cap_info.e_uop = _SM_SESS_GNRC_UOP_TRICK;
            t_uop_cap_info.u.t_speed = pt_media_desc->e_speed;
            i4_ret = x_sm_get(pt_running_svc_req->pt_player->h_sess,
                              SM_SESS_GNRC_GET_TYPE_UOP_CAP,
                              (VOID*)&t_uop_cap_info,
                              &z_get_info_size);
            if((i4_ret != SVCTXR_OK) || !t_uop_cap_info.b_is_allowed)
            {
                x_dbg_stmt(_ERROR_HEADER"SVCTX_MM_SET_TYPE_REPEAT(A) Err: Get Trick Capability Error!\n");
                return SVCTXR_FAIL;
            }
            
            x_memset(&t_uop_cap_info, 0, sizeof(SM_SESS_GNRC_UOP_CAP_INFO_T));
            t_uop_cap_info.e_uop = _SM_SESS_GNRC_UOP_SEEK;
            t_uop_cap_info.u.t_speed = pt_media_desc->e_speed;
            i4_ret = x_sm_get(pt_running_svc_req->pt_player->h_sess,
                              SM_SESS_GNRC_GET_TYPE_UOP_CAP,
                              (VOID*)&t_uop_cap_info,
                              &z_get_info_size);
            if((i4_ret != SVCTXR_OK) || !t_uop_cap_info.b_is_allowed)
            {
                x_dbg_stmt(_ERROR_HEADER"SVCTX_MM_SET_TYPE_REPEAT(A) Err: Get Seek Capability Error!\n");
                return SVCTXR_FAIL;
            }
            
            pt_media_desc->t_repeat_ab_info.b_seeking = FALSE;
            pt_pos_info = &pt_media_desc->t_repeat_ab_info.t_repeat_info_a;
            pb_repeat_flg = &pt_media_desc->t_repeat_ab_info.b_set_repeat_a;
            break;
        case MM_SVC_REPEAT_AB_TYPE_SET_B:
            if(pt_media_desc->t_repeat_ab_info.b_seeking)
            {
                return SVCTXR_WRONG_STATE;
            }
            if(pt_media_desc->b_is_seeking)
            {
                return SVCTXR_FAIL;
            }
            if(!pt_media_desc->t_repeat_ab_info.b_set_repeat_a ||
                pt_media_desc->b_is_seeking)
            {
                x_dbg_stmt(_ERROR_HEADER"SVCTX_MM_SET_TYPE_REPEAT(B) Err: Is in seeking!\n");
                return SVCTXR_WRONG_STATE;
            }
            pt_pos_info = &pt_media_desc->t_repeat_ab_info.t_repeat_info_b;
            pb_repeat_flg = &pt_media_desc->t_repeat_ab_info.b_set_repeat_b;
            break;
        case MM_SVC_REPEAT_AB_TYPE_CANCEL_ALL:
            {
                MM_AB_POS_T  t_ab_pos = {0};
                BOOL b_seek_orig = pt_media_desc->t_repeat_ab_info.b_seeking;
                t_ab_pos.b_valid = FALSE; 
                i4_ret = x_sm_set(pt_running_svc_req->pt_player->h_sess,
                                  SM_SESS_GNRC_SET_TYPE_AB_POSITION,
                                  (VOID*)&t_ab_pos,
                                  sizeof(MM_AB_POS_T));
                if (i4_ret != SMR_OK)
                {
                    x_dbg_stmt(_ERROR_HEADER"SM_SESS_GNRC_SET_TYPE_AB_POSITION Err: cancel ab repeat fail!\r\n");
                    i4_ret =  SVCTXR_FAIL;
                }
                x_memset(&pt_media_desc->t_repeat_ab_info, 0, sizeof(MM_REPEAT_AB_INFO_T));
                pt_media_desc->t_repeat_ab_info.b_seeking = b_seek_orig;
            }
            return SVCTXR_OK;
        default:
            return SVCTXR_NOT_SUPPORT;
        }

        if(pt_pos_info == NULL || pb_repeat_flg == NULL)
        {
            return SVCTXR_NOT_SUPPORT;
        }
        x_memset(pt_pos_info, 0, sizeof(MM_SVC_POS_INFO_T));

        if(x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream))    
        {
            
            /*Get B position and only it must pause*/
            if(pt_repeat->u.e_repeat_ab_type == MM_SVC_REPEAT_AB_TYPE_SET_B)
            {
                /*trick to pause*/
                t_trick_info.e_speed_type = MM_SPEED_TYPE_PAUSE;
                i4_ret = _mm_hdlr_set(pt_svctx, SVCTX_MM_SET_TYPE_TRICK, &t_trick_info, sizeof(t_trick_info));
                if (i4_ret != SMR_OK)
                {
                    x_dbg_stmt(_ERROR_HEADER"SVCTX_MM_SET_TYPE_REPEAT(B) Err: pause!\n");
                    return SVCTXR_FAIL;
                }
            }

            i4_ret = _mm_hdlr_get(pt_svctx,
                     SVCTX_MM_GET_TYPE_POS_PTS,
                     pt_pos_info,
                     &z_size);
            if(i4_ret != SVCTXR_OK)
            {
                t_trick_info.e_speed_type = MM_SPEED_TYPE_FORWARD_1X;
                _mm_hdlr_set(pt_svctx, SVCTX_MM_SET_TYPE_TRICK, &t_trick_info, sizeof(t_trick_info));
                x_dbg_stmt(_ERROR_HEADER"SVCTX_MM_SET_TYPE_REPEAT(B) Err: SVCTX_MM_GET_TYPE_POS_PTS!\n");
                return SVCTXR_FAIL;
            }
            /*After get position, it will do seek or swap A&B, after do seek or swap A&B the speed will trick to 1x*/
        }
        
        x_dbg_stmt("==========================ui8_frame_position:%llu\n",
            pt_pos_info->u.render_info.ui8_frame_position);
        x_dbg_stmt("==========================ui8_frame_position_disp:%llu\n",
            pt_pos_info->u.render_info.ui8_frame_position_disp);                
        x_dbg_stmt("==========================ui8_i_frame_position:%llu\n",
            pt_pos_info->u.render_info.ui8_i_frame_position);
        x_dbg_stmt("==========================ui8_i_pts_info:%llu\n",
            pt_pos_info->u.render_info.ui8_i_pts_info);
        x_dbg_stmt("==========================ui8_pts_info:%llu\n",
            pt_pos_info->u.render_info.ui8_pts_info);
        x_dbg_stmt("==========================ui8_aud_pts_info:%llu\n",
            pt_pos_info->u.render_info.ui8_aud_pts_info);
        x_dbg_stmt("==========================ui8_aud_frame_position:%llu\n\n",
            pt_pos_info->u.render_info.ui8_aud_frame_position);
        x_dbg_stmt("==========================ui8_stc:%llu\n\n",
            pt_pos_info->u.render_info.ui8_stc);
            
        if (i4_ret != SVCTXR_OK)
        {
            x_memset(pt_pos_info, 0, sizeof(MM_SVC_POS_INFO_T));
            *pb_repeat_flg = FALSE;
        }
        else
        {
            BOOL b_swap = FALSE;
            *pb_repeat_flg = TRUE;
            
            if(pt_repeat->u.e_repeat_ab_type == MM_SVC_REPEAT_AB_TYPE_SET_B)
            {
                if(x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream))    
                {
                    BOOL b_seek = FALSE;
                    if(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_PS ||
                        pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS ||
                        pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_VIDEO_ES)
                    {
                        if(pt_media_desc->t_repeat_ab_info.t_repeat_info_a.u.render_info.ui8_frame_position <
                            pt_media_desc->t_repeat_ab_info.t_repeat_info_b.u.render_info.ui8_frame_position) 
                        {
                            b_seek = TRUE;
                        }
                    }
                    else
                    {
                        if (pt_media_desc->t_repeat_ab_info.t_repeat_info_a.u.render_info.ui8_pts_info ==
                            pt_media_desc->t_repeat_ab_info.t_repeat_info_b.u.render_info.ui8_pts_info)
                        {                    
                            t_trick_info.e_speed_type = e_speed_type;
                            i4_ret = _mm_hdlr_set(pt_svctx, SVCTX_MM_SET_TYPE_TRICK, &t_trick_info, sizeof(t_trick_info));
                            return SVCTXR_FAIL;
                        }
                        
                        if(pt_media_desc->t_repeat_ab_info.t_repeat_info_a.u.render_info.ui8_pts_info <
                        pt_media_desc->t_repeat_ab_info.t_repeat_info_b.u.render_info.ui8_pts_info)
                        {
                            b_seek = TRUE;
                        }
                    }
                        
                    if(!b_seek)
                    {
                        /*swap a & b*/
                        MM_SVC_POS_INFO_T    t_pos_temp;
                        t_pos_temp = pt_media_desc->t_repeat_ab_info.t_repeat_info_a;
                        pt_media_desc->t_repeat_ab_info.t_repeat_info_a = pt_media_desc->t_repeat_ab_info.t_repeat_info_b;
                        pt_media_desc->t_repeat_ab_info.t_repeat_info_b = t_pos_temp;
                        b_swap = TRUE;
                        
                        t_trick_info.e_speed_type = MM_SPEED_TYPE_FORWARD_1X;
                        i4_ret = _mm_hdlr_set(pt_svctx, SVCTX_MM_SET_TYPE_TRICK, &t_trick_info, sizeof(t_trick_info));
                        if (i4_ret != SMR_OK)
                        {
                            x_dbg_stmt(_ERROR_HEADER"SVCTX_MM_SET_TYPE_REPEAT(B) Err: trick to MM_SPEED_TYPE_FORWARD_1X!\n");
                            i4_ret =  SVCTXR_FAIL;
                        }
                        mm_hdlr_invoke_client_svc_nfy_fct(
                                 pt_svctx,
                                 SVCTX_NTFY_CODE_MEDIA_AB_REPEAT_DONE,
                                 ST_UNKNOWN
                                 );
                    }
                    else
                    {
                        /* seek to the A position */
                        pt_media_desc->t_repeat_ab_info.t_repeat_info_a.t_pos_type = MM_SVC_POS_TYPE_RANGE_AB_START;
                        i4_ret = _mm_hdlr_set(pt_svctx,
                                          SVCTX_MM_SET_TYPE_SEEK,
                                          (VOID*)&pt_media_desc->t_repeat_ab_info.t_repeat_info_a,
                                          sizeof(MM_SVC_POS_INFO_T));
                        if (i4_ret != SMR_OK)
                        {
                            x_dbg_stmt(_ERROR_HEADER"SVCTX_MM_SET_TYPE_REPEAT(B) Err: Seek to A!\n");
                            i4_ret =  SVCTXR_FAIL;
                        }
                        pt_media_desc->t_repeat_ab_info.b_seeking = TRUE;
                        pt_media_desc->b_is_seeking = TRUE;
                    }
                }
            }
            
            if(pt_repeat->u.e_repeat_ab_type == MM_SVC_REPEAT_AB_TYPE_SET_B)
            {
                MM_AB_POS_T  t_ab_pos = {0};
                t_ab_pos.b_valid = TRUE; 
                t_ab_pos.t_pos_a.i4_temporal_reference 
                    = pt_media_desc->t_repeat_ab_info.t_repeat_info_a.u.render_info.i4_temporal_reference;
                t_ab_pos.t_pos_a.ui2_decoding_order 
                    = pt_media_desc->t_repeat_ab_info.t_repeat_info_a.u.render_info.ui2_decoding_order;
                t_ab_pos.t_pos_a.ui8_frame_position 
                    = pt_media_desc->t_repeat_ab_info.t_repeat_info_a.u.render_info.ui8_frame_position;
                t_ab_pos.t_pos_a.ui8_frame_position_disp
                    = pt_media_desc->t_repeat_ab_info.t_repeat_info_a.u.render_info.ui8_frame_position_disp;
                t_ab_pos.t_pos_a.ui8_i_frame_position 
                    = pt_media_desc->t_repeat_ab_info.t_repeat_info_a.u.render_info.ui8_i_frame_position;
                t_ab_pos.t_pos_a.ui8_i_pts_info 
                    = pt_media_desc->t_repeat_ab_info.t_repeat_info_a.u.render_info.ui8_i_pts_info;
                t_ab_pos.t_pos_a.ui8_pts_info 
                    = pt_media_desc->t_repeat_ab_info.t_repeat_info_a.u.render_info.ui8_pts_info;
                t_ab_pos.t_pos_a.ui8_stc 
                    = pt_media_desc->t_repeat_ab_info.t_repeat_info_a.u.render_info.ui8_stc;
                t_ab_pos.t_pos_a.ui8_aud_pts_info 
                    = pt_media_desc->t_repeat_ab_info.t_repeat_info_a.u.render_info.ui8_aud_pts_info;
                t_ab_pos.t_pos_a.ui8_aud_frame_position 
                    = pt_media_desc->t_repeat_ab_info.t_repeat_info_a.u.render_info.ui8_aud_frame_position;

                t_ab_pos.t_pos_b.i4_temporal_reference 
                    = pt_media_desc->t_repeat_ab_info.t_repeat_info_b.u.render_info.i4_temporal_reference;
                t_ab_pos.t_pos_b.ui2_decoding_order 
                    = pt_media_desc->t_repeat_ab_info.t_repeat_info_b.u.render_info.ui2_decoding_order;
                t_ab_pos.t_pos_b.ui8_frame_position 
                    = pt_media_desc->t_repeat_ab_info.t_repeat_info_b.u.render_info.ui8_frame_position;
                t_ab_pos.t_pos_b.ui8_frame_position_disp
                    = pt_media_desc->t_repeat_ab_info.t_repeat_info_b.u.render_info.ui8_frame_position_disp;
                t_ab_pos.t_pos_b.ui8_i_frame_position 
                    = pt_media_desc->t_repeat_ab_info.t_repeat_info_b.u.render_info.ui8_i_frame_position;
                t_ab_pos.t_pos_b.ui8_i_pts_info 
                    = pt_media_desc->t_repeat_ab_info.t_repeat_info_b.u.render_info.ui8_i_pts_info;
                t_ab_pos.t_pos_b.ui8_pts_info 
                    = pt_media_desc->t_repeat_ab_info.t_repeat_info_b.u.render_info.ui8_pts_info;
                t_ab_pos.t_pos_b.ui8_stc 
                    = pt_media_desc->t_repeat_ab_info.t_repeat_info_b.u.render_info.ui8_stc;
                t_ab_pos.t_pos_b.ui8_aud_pts_info 
                    = pt_media_desc->t_repeat_ab_info.t_repeat_info_b.u.render_info.ui8_aud_pts_info;
                t_ab_pos.t_pos_b.ui8_aud_frame_position 
                    = pt_media_desc->t_repeat_ab_info.t_repeat_info_b.u.render_info.ui8_aud_frame_position;

                i4_ret = x_sm_set(pt_running_svc_req->pt_player->h_sess,
                                  SM_SESS_GNRC_SET_TYPE_AB_POSITION,
                                  (VOID*)&t_ab_pos,
                                  sizeof(MM_AB_POS_T));
                if (i4_ret != SMR_OK)
                {
                    x_dbg_stmt(_ERROR_HEADER"SVCTX_MM_SET_TYPE_REPEAT(B) Err: SM_SESS_GNRC_SET_TYPE_AB_POSITION!\n");
                    i4_ret =  SVCTXR_FAIL;
                }
            }
            if(b_swap)
            {
                return SVCTXR_GET_POS_AS_TIME;
            }
        }    
    }

    return SVCTXR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_set
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_set(
                    SVCTX_T*                    pt_svctx,
                    SVCTX_SET_TYPE_T            e_set_type,
                    VOID*                       pv_set_info,
                    SIZE_T                      z_set_info_size
                    )
{
    MM_SVC_REQ_T*                       pt_running_svc_req = NULL;
    MEDIA_DESCRIBT_T*                   pt_media_desc = NULL;     
    MM_SVC_UOP_HANDLER_FCT_TBL_T        t_fct_tbl;
    INT32                               i4_ret = SVCTXR_OK;    
    MM_SVC_NAV_INFO_T                   pt_nav_info;    
    MM_SVC_BTN_INFO_T*                  pt_btn_info = NULL;

    /*check parameters*/
    if (pt_svctx == NULL || pv_set_info == NULL)
    {
        return SVCTXR_INV_ARG;
    }
    
    pt_running_svc_req  = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    if (pt_running_svc_req == NULL)
    {
        return SVCTXR_NOT_SUPPORT;
    }
    
    t_fct_tbl           = pt_running_svc_req->t_uop_fct_tbl;
    if (t_fct_tbl.pf_open == NULL)
    {
        return SVCTXR_NOT_SUPPORT;
    }

    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc,&pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }

    switch (e_set_type)
    {
    case SVCTX_MM_SET_TYPE_CHANGE_STRM:
        i4_ret = _mm_hdlr_set_change_strm(pt_svctx,pv_set_info,z_set_info_size);
        break;

    case SVCTX_COMMON_SET_VIDEO_MUTE:
        if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) 
        {
            if (pt_svctx->t_video_mute.b_enable == TRUE) 
            {
                /* set video plan to blank to show bg color */
                svctx_set_video_plane(pt_svctx, SCC_VID_BLANK);
            } 
            else 
            {
                /* set video plan to not blank to show snow noise */
                svctx_set_video_plane(pt_svctx, pt_svctx->e_default_vid_mode);
            }
        }
        break;
        
    case SVCTX_MM_SET_TYPE_BTN:
        pt_btn_info = (MM_SVC_BTN_INFO_T*)pv_set_info;
        x_memset(&pt_nav_info,0,sizeof(MM_SVC_NAV_INFO_T));        
        x_memcpy(&pt_nav_info.t_btn_action_info.t_btn_type,pt_btn_info,sizeof(MM_SVC_BTN_INFO_T));        
        t_fct_tbl.pf_btn(pt_media_desc,&pt_nav_info);
        break;
        
#ifdef MULTIMEDIA_AUTOTEST_SUPPORT
    case SVCTX_MM_SET_TYPE_AUTOTEST_INFO:
    {
        MM_AUTOTEST_INFO_T* prAutoTestInfo = (MM_AUTOTEST_INFO_T *)pv_set_info;
        PLAYER_T*           pt_player = pt_running_svc_req->pt_player;
        
        i4_ret = x_sm_set(pt_player->h_sess,
                          SM_SESS_GNRC_SET_TYPE_MM_AUTOTEST,
                          (VOID*)(prAutoTestInfo),
                          sizeof(MM_AUTOTEST_INFO_T));
        DBG_INFO(("\n[MM_HDLR_API]SM_SESS_GNRC_SET_TYPE_MM_AUTOTEST ret=%d\n",i4_ret));
        if (i4_ret != SMR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"x_sm_set(%u) failed, ret = %d)\r\n", SM_SESS_GNRC_SET_TYPE_MM_AUTOTEST, i4_ret);
        }
        break;
    }
#endif

    case SVCTX_MM_SET_TYPE_SEEK:
        x_dbg_stmt(_INFO_HEADER"DO SEEK OPERATION\n");
        i4_ret = mm_hdlr_set_seek(pt_svctx,pv_set_info,z_set_info_size);
        if (i4_ret != SVCTXR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"mm_hdlr_set_seek fail!!\n");
        }
        break;

    case SVCTX_MM_SET_TYPE_TRICK:
        x_dbg_stmt(_INFO_HEADER"DO TRICK OPERATION\n");
        i4_ret = mm_hdlr_set_trick(pt_svctx,pv_set_info,z_set_info_size); 
        if (i4_ret != SVCTXR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"mm_hdlr_set_trick fail!!\n");
        }
        break;
    
#if MM_AB_REPEAT_SUPPORT
    case SVCTX_MM_SET_TYPE_REPEAT:
        x_dbg_stmt(_INFO_HEADER"DO AB REPEART OPERATION\n");
#ifdef DIVX_PLUS_CER        
        if (!pt_media_desc->b_has_chapter)
        {
            i4_ret = _mm_hdlr_set_ab_repeat(pt_svctx,pv_set_info,z_set_info_size);
            if (i4_ret != SVCTXR_OK)
            {
                x_dbg_stmt(_ERROR_HEADER"mm_hdlr_set_ab_repeat fail!!\n");
            }
        }
        else
        {
            i4_ret = SVCTXR_NOT_SUPPORT;
        }
#else
        i4_ret = _mm_hdlr_set_ab_repeat(pt_svctx,pv_set_info,z_set_info_size);
        if (i4_ret != SVCTXR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"mm_hdlr_set_ab_repeat fail!!\n");
        }
#endif
        break;
#endif

    //MM_LYRIC_SUPPORT
    case SVCTX_MM_SBTL_SET_TYPE_DISP_MODE:
    case SVCTX_MM_SBTL_SET_TYPE_HILT_STL:
    case SVCTX_MM_SBTL_SET_TYPE_TIME_OFST:
    case SVCTX_MM_SBTL_SET_TYPE_FONT_ENC:
    case SVCTX_MM_SBTL_SET_TYPE_HILT_ATTR:
    case SVCTX_MM_SBTL_SET_TYPE_NRML_ATTR:
    case SVCTX_MM_SBTL_SET_TYPE_SHOW_HIDE:
		x_dbg_stmt(_INFO_HEADER"AP SET SUBTITLE ATTR\n");
        i4_ret = _mm_hdlr_set_lyric_sbtl(pt_svctx,e_set_type,pv_set_info,z_set_info_size);
        if (i4_ret != SVCTXR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"_mm_hdlr_set_lyric_sbtl fail!!\n");
        }
        break;

#ifdef SUPPORT_SET_SBTL_ENCODING
    case SVCTX_MM_SET_SUBTITLE_ENCODING:
        {
            MM_SVC_SUBTITLE_INFO_T* pt_mm_sbtl_info = (MM_SVC_SUBTITLE_INFO_T*)pv_set_info;
            VOID* pt_mm_subtl_encoding = &pt_mm_sbtl_info->u.t_mm_sbtl_encoding;

            x_dbg_stmt(_INFO_HEADER"AP SET SUBTITLE ENCODING\n");
            if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream))
            {
                i4_ret = x_sm_set(pt_running_svc_req->t_strm_reqs[ST_SUBTITLE].h_stream,
                             SM_MM_SBTL_SET_TYPE_ENCODING,
                             pt_mm_subtl_encoding,
                             z_set_info_size);
            
                if (i4_ret!= SMR_OK)
                {
                    DBG_ERROR((_ERROR_HEADER"set encoding fail!\r\n"));
                }
            }
        }
        break;
#endif

    default:
        t_fct_tbl.pf_trick(NULL,NULL);
        i4_ret = SVCTXR_NOT_SUPPORT;
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_default_strm_comp_filter
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
BOOL _mm_hdlr_default_strm_comp_filter (
                    UINT16                      ui2_num_recs,
                    UINT16                      ui2_idx,
                    const STREAM_COMP_ID_T*     pt_comp_id,
                    const SCDB_REC_T*           pt_scdb_rec,
                    VOID*                       pv_tag
                    )
{
    SVCTX_T*                       pt_svctx            = NULL;
    MM_SVC_REQ_T*                  pt_running_svc_req  = NULL; 
    MEDIA_DESCRIBT_T*              pt_media_desc       = NULL;

    pt_svctx           = (SVCTX_T*)pv_tag;     
    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc, &pt_media_desc);

    if(pt_comp_id->e_type == ST_SUBTITLE)
    {
        return TRUE;
    }
    else if(pt_comp_id->e_type != ST_VIDEO)
    {
        return FALSE;
    }
    
    if(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)
    {        
        if((ui2_num_recs > 1) && (pt_scdb_rec->u.t_video_mpeg.t_mvc_ext.b_mvc_video))
        {
            pt_media_desc->b_multi_track_3d = TRUE;
        }

        if(ui2_idx == MM_HDLE_VID_TRACK_IDX_ONE)
        {
            if(!pt_scdb_rec->u.t_video_mpeg.t_mvc_ext.b_mvc_video)
            {
                return TRUE;
            }
        }
        else if(ui2_idx == MM_HDLE_VID_TRACK_IDX_TWO)
        {
            if((pt_media_desc->b_multi_track_3d) && 
               (!pt_scdb_rec->u.t_video_mpeg.t_mvc_ext.b_mvc_video))
            {
                return TRUE;
            }
        }
        return FALSE;
    }
    return (ui2_idx == 0 ? TRUE : FALSE);
}

#ifdef MW_MM_DUAL_VID_SUPPORT
 BOOL _mm_hdlr_3d_multi_video_track_filter (
                    UINT16                      ui2_num_recs,
                    UINT16                      ui2_idx,
                    const STREAM_COMP_ID_T*     pt_comp_id,
                    const SCDB_REC_T*           pt_scdb_rec,
                    VOID*                       pv_tag
                    )

{
    UINT32                                  ui4_sz;
    SVCTX_T*                             pt_svctx;
    MM_SVC_REQ_T*                   pt_running_svc_req; 
    MEDIA_DESCRIBT_T*             pt_media_desc;
    MINFO_INFO_T                       t_minfo_gen_rec;
    pt_svctx =                              (SVCTX_T*)pv_tag;

    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc, &pt_media_desc);

    if(pt_comp_id->e_type != ST_VIDEO)
    {
        return FALSE;
    }
    if(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)
    {
        if(ui2_num_recs <= 1)/*Only one video*/
        {
            pt_media_desc->b_multi_track_3d = FALSE;
        }
        else
        {
            if(pt_scdb_rec->u.t_video_mpeg.t_mvc_ext.b_mvc_video)
            {
                pt_media_desc->b_multi_track_3d = TRUE;
            }
        }

        if(ui2_idx == MM_HDLE_VID_TRACK_IDX_ONE)
        {
            if((ui2_num_recs >= 1) && pt_scdb_rec->u.t_video_mpeg.t_mvc_ext.b_mvc_video )
            {
                return TRUE;
            }
        }
        else /* 2nd track or more.....*/
        { 
            if(pt_scdb_rec->u.t_video_mpeg.t_mvc_ext.b_mvc_video)
            {
                return TRUE;
            }
        }
        return FALSE;
    }

    if(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER && 
      (pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_ASF ||
       pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_AVI))
    {
       ui4_sz = sizeof(MINFO_INFO_T);
       x_minfo_get_info(pt_media_desc->h_media_info,
                          MINFO_INFO_TYPE_GENERIC,
                          1,
                          &t_minfo_gen_rec,
                          (SIZE_T*)&ui4_sz);
       if(t_minfo_gen_rec.u.t_general_info.b_stereo_scopic && 
          t_minfo_gen_rec.u.t_general_info.e_scopic_type == MM_SCOPIC_DS)
       {
           pt_media_desc->b_multi_track_3d = TRUE;
       } 
       /* 3D*/
       if(pt_media_desc->b_multi_track_3d )
       {
            if (ui2_idx >0)
            {
                 return TRUE;
            }
            else
            {
                 return FALSE;
            }
       }
    }
    return (ui2_idx == 0 ? TRUE : FALSE);
}
#endif
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_strm_nfy_fct
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
 *---------------------------------------------------------------------------*/
static INT32 _mm_hdlr_strm_nfy_fct(
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
 * Name: _mm_hdlr_get_conn_attr
 *
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_get_conn_attr(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    CM_CTRL_TYPE_T              e_ctrl,
                    VOID*                       pv_ctrl_data
                    )
{
    MM_SVC_REQ_T* pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
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
 * Name: _mm_hdlr_get_conn_handle
 *
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_get_conn_handle(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    HANDLE_T*                   ph_conn
                    )
{
    MM_SVC_REQ_T* pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T      h_conn;

    h_conn = player_get_conn_handle(pt_running_svc_req->pt_player);
    if (x_handle_valid(h_conn) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    *ph_conn = h_conn;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _mm_hdlr_get_cm_op
 *
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_get_cm_op(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    CM_CTRL_TYPE_T              e_ctrl,
                    VOID*                       pv_ctrl_data
                    )
{
    MM_SVC_REQ_T* pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
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
 * Name: _mm_hdlr_set_cm_op
 *
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_set_cm_op(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    CM_CTRL_TYPE_T              e_ctrl,
                    VOID*                       pv_ctrl_data
                    )
{
    MM_SVC_REQ_T* pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
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
 *      _mm_hdlr_open_scdb
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_open_scdb (
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    HANDLE_T*                   ph_scdb,
                    VOID*                       pv_tag,
                    x_scdb_nfy_fct              pf_nfy
                    )
{
    MM_SVC_REQ_T* pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T      h_conn             = NULL_HANDLE;
    INT32         i4_ret             = SVCTXR_OK;
    CM_COND_T     e_cm_cond;

    h_conn = player_get_conn_handle(pt_running_svc_req->pt_player);
    if (x_handle_valid(h_conn) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    e_cm_cond = player_get_conn_cond(pt_running_svc_req->pt_player);
    if (e_cm_cond != CM_COND_CONNECTED && e_cm_cond != CM_PLAYBACK_CONN_INFO_ITEM_TRANSFER_START) {
        return SVCTXR_WRONG_STATE;
    }

    i4_ret = x_scdb_open(h_conn, ph_scdb, pv_tag, pf_nfy);

    return svctx_translate_scdbr_2_svctxr(i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_open_stream
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static INT32 _mm_hdlr_open_stream (
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
    MM_SVC_REQ_T*  pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T       h_sess             = NULL_HANDLE;
    INT32          i4_ret             = SVCTXR_OK;
    SM_SESS_COND_T e_sess_cond;
    HANDLE_T            h_media ;
    MEDIA_DESCRIBT_T*   pt_media_desc;  

    h_media = pt_running_svc_req->h_media_desc;
    i4_ret = _mm_hdlr_get_media_obj(h_media, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
       return i4_ret;
    }    

    h_sess = player_get_sess_handle(pt_running_svc_req->pt_player);
    if (x_handle_valid(h_sess) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    e_sess_cond = player_get_sess_cond(pt_running_svc_req->pt_player);
    if (svctx_is_able_to_select_stream(e_sess_cond) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    CLI_FUNC_CALL_TRACE_FOR_STRM("x_sm_open_stream", pt_strm_comp_id->e_type);
    SVCTX_DBG_LOG_MW_0(pt_media_desc->t_video_thumbnail_info.h_surf != 0,
        ("MM_hdlr: begine open stream.\r\n"));
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
 * Name
 *      _mm_hdlr_mkv_strm_info_2_cmd
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static VOID _mm_hdlr_mkv_strm_info_2_cmd(MINFO_INFO_T*          pt_minfo_rec,
                                        SBTL_MKV_SBTL_CMD_T*    pt_mkv_sbtl_cmd)
{
    MINFO_MKV_SUBTITLE_STM_ATRBT_T* pt_mkv_sbtl_stm_att = NULL;
    MINFO_MKV_CONTENT_ENCODING_T*   pt_minfo_encoding = NULL;
    SBTL_MKV_CONTENT_ENCODING_T*    pt_sbtl_encoding = NULL;
    UINT8 ui1_count = 0;
    if((pt_minfo_rec == NULL) || (pt_mkv_sbtl_cmd == NULL))
    {
        return;
    }

    pt_mkv_sbtl_stm_att = &pt_minfo_rec->u.t_stm_attribute.u.t_mkv_sp_stm_attr;

    pt_mkv_sbtl_cmd->t_mkv_sbtl_prv.ui8_codec_private_len
        = pt_mkv_sbtl_stm_att->ui4_codec_info_size;
    if(pt_mkv_sbtl_cmd->t_mkv_sbtl_prv.ui8_codec_private_len != 0)
    {
        pt_mkv_sbtl_cmd->t_mkv_sbtl_prv.aui1_codec_private
            = pt_mkv_sbtl_stm_att->pui1_codec_info;
    }

    ui1_count = (UINT8)pt_mkv_sbtl_stm_att->ui4_encoding_num;
    pt_mkv_sbtl_cmd->t_mkv_sbtl_cont_encode.ui1_encoding_number = ui1_count;
    pt_mkv_sbtl_cmd->t_mkv_sbtl_cont_encode.at_encodings 
        = (SBTL_MKV_CONTENT_ENCODING_T*)x_mem_alloc(ui1_count * sizeof(SBTL_MKV_CONTENT_ENCODING_T));
    if(pt_mkv_sbtl_cmd->t_mkv_sbtl_cont_encode.at_encodings == NULL)
    {
        return;
    }
    pt_minfo_encoding = pt_mkv_sbtl_stm_att->pt_content_encoding;
    while((ui1_count > 0) && (pt_minfo_encoding != NULL))
    {
        pt_sbtl_encoding = &pt_mkv_sbtl_cmd->t_mkv_sbtl_cont_encode.at_encodings   \
                            [pt_mkv_sbtl_stm_att->ui4_encoding_num - ui1_count];
        pt_sbtl_encoding->ui1_order = (UINT8)pt_minfo_encoding->ui8_order;
        pt_sbtl_encoding->ui1_scope = (UINT8)pt_minfo_encoding->ui8_scope;
        pt_sbtl_encoding->ui1_type = (UINT8)pt_minfo_encoding->ui8_type;
        pt_sbtl_encoding->ui1_compress_algo = (UINT8)pt_minfo_encoding->ui8_compress_algo;
        if(pt_minfo_encoding->ui8_comp_settings_length > MM_MAX_MKV_COMP_SETTINGS_LENGTH)
        {
            pt_sbtl_encoding->ui4_comp_settings_length
                = MM_MAX_MKV_COMP_SETTINGS_LENGTH;
        }
        else
        {
            pt_sbtl_encoding->ui4_comp_settings_length
                = (UINT32)pt_minfo_encoding->ui8_comp_settings_length;
        }
        if( pt_minfo_encoding->pui1_comp_settings_data )
        {
            x_memcpy((VOID*)pt_sbtl_encoding->aui1_comp_settings_data,
                    pt_minfo_encoding->pui1_comp_settings_data,
                    pt_sbtl_encoding->ui4_comp_settings_length);
        }
        ui1_count--;
    }
}

/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_init
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
INT32 mm_hdlr_init(
                    VOID
                    )
{
    SVCTX_HDLR_FCT_TABLE_T t_fct_table = {0};
    INT32                  i4_ret;

    if (b_g_init == TRUE) {
        return SVCTXR_OK;
    }

    /* service relative function table*/
    t_fct_table.pf_allocate_svc_req           = _mm_hdlr_allocate_svc_req;
    t_fct_table.pf_free_svc_req               = _mm_hdlr_free_svc_req;
    t_fct_table.pf_create_svc_req             = _mm_hdlr_create_svc_req;
    t_fct_table.pf_create_surf_svc_req        =NULL;

    /* stream relative function table*/
    t_fct_table.pf_allocate_strm_req          = _mm_hdlr_allocate_strm_req;
    t_fct_table.pf_free_strm_req              = _mm_hdlr_free_strm_req;
    t_fct_table.pf_create_strm_req            = _mm_hdlr_create_strm_req;

    t_fct_table.pf_get_scdb_num_recs          = mm_hdlr_get_scdb_num_recs;
    t_fct_table.pf_get_cp_info                = NULL;
    t_fct_table.pf_get_ca_info                = NULL;
    t_fct_table.pf_get_using_scdb_info        = _mm_hdlr_get_using_scdb_info;
    t_fct_table.pf_get_strm_req_type          = NULL;
    t_fct_table.pf_get_strm_comp_id           = _mm_hdlr_get_strm_comp_id;

#ifdef TIME_SHIFT_SUPPORT
    t_fct_table.pf_replace_svc_req            = _mm_hdlr_replace_svc_req;
#else
    t_fct_table.pf_replace_svc_req            = NULL;
#endif    
    t_fct_table.pf_select_svc_req             = mm_hdlr_select_svc;
    t_fct_table.pf_stop_svc_req               = mm_hdlr_stop_svc;
    t_fct_table.pf_unblock_svc_req            = mm_hdlr_check_svc_block;
    t_fct_table.pf_select_strm_req            = mm_hdlr_select_strm;
    t_fct_table.pf_stop_strm_req              = mm_hdlr_stop_strm;
    t_fct_table.pf_internal_msg               = mm_hdlr_internal_msg;
    t_fct_table.pf_is_blocked_source          = NULL;

    t_fct_table.pf_set_session_attr           = _mm_hdlr_set_sess_attr;
    t_fct_table.pf_get_session_attr           = _mm_hdlr_get_sess_attr;
    t_fct_table.pf_set_stream_attr            = _mm_hdlr_set_strm_attr;
    t_fct_table.pf_get_stream_attr            = _mm_hdlr_get_strm_attr;
    t_fct_table.pf_get_signal_level           = NULL;
    t_fct_table.pf_get_dbm_signal_level       = NULL;
    t_fct_table.pf_get_tuner_status           = NULL;
    t_fct_table.pf_get_cell_id                = NULL;
    t_fct_table.pf_get_vbi_broker             = NULL;
    t_fct_table.pf_get_stream_handle          = _mm_hdlr_get_strm_handle;
#ifdef TIME_SHIFT_SUPPORT
    t_fct_table.pf_get_current_event_info_len = _mm_hdlr_get_current_event_info_len;
    t_fct_table.pf_get_current_event_info     = _mm_hdlr_get_current_event_info;
#else
    t_fct_table.pf_get_current_event_info_len = NULL;
    t_fct_table.pf_get_current_event_info     = NULL;
#endif /*TIME_SHIFT_SUPPORT*/

    t_fct_table.pf_freeze                     = NULL;/*mm_hdlr_freeze;*/
    t_fct_table.pf_unfreeze                   = NULL;/*mm_hdlr_unfreeze;*/

    t_fct_table.pf_open_scdb                  = _mm_hdlr_open_scdb;
    t_fct_table.pf_open_stream                = _mm_hdlr_open_stream;
    t_fct_table.pf_get_conn_attr              = _mm_hdlr_get_conn_attr;
    t_fct_table.pf_set_conn_attr              = mm_hdlr_set_conn_attr;
    t_fct_table.pf_get_conn_op                = _mm_hdlr_get_cm_op;
    t_fct_table.pf_set_conn_op                = _mm_hdlr_set_cm_op;
    t_fct_table.pf_get_conn_handle            = _mm_hdlr_get_conn_handle;

    t_fct_table.pf_get                        = _mm_hdlr_get;
    t_fct_table.pf_set                        = _mm_hdlr_set;
    t_fct_table.pf_is_handler                 = _mm_hdlr_is_handler;
    t_fct_table.pf_deinit                     = _mm_hdlr_deinit;

    i4_ret = svctx_register_handler(
                    MM_DEFAULT_HDLR_NAME,
                    &t_fct_table
                    );
    if (i4_ret != SVCTXR_OK) {
        DBG_ERROR((_ERROR_HEADER"svctx_register_handler() failed, ret = %d)\r\n", i4_ret));
        return i4_ret;
    }

    i4_ret = mm_svc_req_init(
                    (UINT16)(svctx_get_obj_amount() * SVC_REQ_AMOUNT_FACTOR)
                    );
    if (i4_ret != SVCTXR_OK) {
        DBG_ERROR((_ERROR_HEADER"mm_svc_req_init() failed, ret = %d)\r\n", i4_ret));
        return i4_ret;
    }

    i4_ret = mm_strm_req_init(
                    (UINT16)(svctx_get_obj_amount() * SVC_REQ_AMOUNT_FACTOR * STRM_TYPE_COUNT)
                    );
    if (i4_ret != SVCTXR_OK) {
        DBG_ERROR((_ERROR_HEADER"mm_strm_req_init() failed, ret = %d)\r\n", i4_ret));
        return i4_ret;
    }
    /*This is for the order control of
    stream format update notify and service status notify*/
    i4_ret = strm_fmt_chker_init(
                    svctx_get_obj_amount()
                    );
    if (i4_ret != SVCTXR_OK) {
        DBG_ERROR((_ERROR_HEADER"strm_fmt_chker_init() failed, ret = %d)\r\n", i4_ret));
        return i4_ret;
    }

    /*
    i4_ret = crnt_evn_init(
                    svctx_get_obj_amount()
                    );
    if (i4_ret != SVCTXR_OK) {
        DBG_ERROR((_ERROR_HEADER"crnt_evn_init() failed, ret = %d)\r\n", i4_ret));
        return i4_ret;
    }
    */

#ifdef CLI_LVL_ALL
     mm_hdlr_cli_attach_cmd_tbl();
#endif

    b_g_init = TRUE;
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_invoke_client_svc_nfy_fct
 * Description
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
VOID mm_hdlr_invoke_client_svc_nfy_fct(
                    SVCTX_T*                    pt_svctx,
                    SVCTX_NTFY_CODE_T           e_code,
                    STREAM_TYPE_T               e_stream_type
                    )
{
    MM_SVC_REQ_T* pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    switch (e_code) {
    case SVCTX_NTFY_CODE_SIGNAL_LOCKED:
        pt_svctx->b_signal_loss_notified = FALSE;
        pt_svctx->e_signal_status        = SIGNAL_STATUS_LOCKED;
        pt_svctx->b_enable_snow          = FALSE;
        break;
    case SVCTX_NTFY_CODE_SIGNAL_DETECTING:
        break;
    case SVCTX_NTFY_CODE_NORMAL:
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
            svctx_set_video_plane(pt_svctx, SCC_VID_NORMAL);/*pt_svctx->e_default_vid_mode*/
            svctx_mute_video(pt_svctx, FALSE);
        }
        break;
    case SVCTX_NTFY_CODE_TUNER_SIGNAL_LOCKED:
    case SVCTX_NTFY_CODE_TUNER_SIGNAL_LOSS:
        break;
    case SVCTX_NTFY_CODE_SIGNAL_LOSS:
#ifdef TIME_SHIFT_SUPPORT
        pt_svctx->e_signal_status = SIGNAL_STATUS_LOSS;
        if (pt_svctx->e_state != SVCTX_COND_PRESENTING_SELECT_SAME_REQ
                && pt_svctx->e_state != SVCTX_COND_PENDING_SELECT_SAME_REQ) {
            pt_svctx->e_state = SVCTX_COND_PENDING;
        }

        if (mm_hdlr_check_signal_loss_priority(pt_svctx, pt_running_svc_req, FALSE) == FALSE) {
            return;
        }
        
        pt_svctx->b_is_blocked     = FALSE;
        pt_svctx->b_block_notified = FALSE;

        if (pt_svctx->b_signal_loss_notified == TRUE) {
            return;
        }

#ifdef MW_PIP_POP_SUPPORT
        if (svctx_get_scrn_status(pt_svctx) == SVCTX_SCREEN_STATUS_SINGLE || 
            svctx_get_scrn_status(pt_svctx) == SVCTX_SCREEN_STATUS_MULTIPLE_FULL)
#endif
        {
            /* only enable video mute in full screen */
            svctx_mute_video(pt_svctx, pt_svctx->t_video_mute.b_enable);
        }

        if (pt_svctx->t_video_mute.b_enable == TRUE) {
            svctx_set_video_plane(pt_svctx, SCC_VID_BLANK);
        } else {
            svctx_set_video_plane(pt_svctx, pt_svctx->e_default_vid_mode);
        }

        pt_svctx->b_signal_loss_notified = TRUE;
        break;
    case SVCTX_NTFY_CODE_SERVICE_UNBLOCKED:
        pt_svctx->b_is_blocked     = FALSE;
        pt_svctx->b_block_notified = FALSE;
        break;
    case SVCTX_NTFY_CODE_SERVICE_BLOCKED:
        pt_svctx->b_is_blocked = TRUE;
        svctx_mute_video(pt_svctx, pt_svctx->t_video_mute.b_enable);
        svctx_set_video_plane(pt_svctx, SCC_VID_BLANK);

        /*if (pt_svctx->e_signal_status != SIGNAL_STATUS_LOCKED 
                && ch_hdlr_check_signal_loss_priority(pt_svctx, pt_running_svc_req, FALSE) == TRUE) {
            return;
        }*/

        if (pt_svctx->b_block_notified == TRUE) {
            return;
        }

        pt_svctx->b_block_notified = TRUE;
        break;
#endif /* TIME_SHIFT_SUPPORT */
    case SVCTX_NTFY_CODE_CRNT_EVN_START_QUERY:
    case SVCTX_NTFY_CODE_CRNT_EVN_NOT_READY:
    case SVCTX_NTFY_CODE_CRNT_EVN_READY:
    case SVCTX_NTFY_CODE_CH_SRC_FROM_TUNER:
    case SVCTX_NTFY_CODE_CH_SRC_FROM_SCART:
    case SVCTX_NTFY_CODE_INTERRUPTED:
    case SVCTX_NTFY_CODE_IN_STOPPING:
        break;
    case SVCTX_NTFY_CODE_NO_AUDIO_VIDEO_SVC:
#ifdef TIME_SHIFT_SUPPORT
        {
            INT32                         i4_ret = SVCTXR_OK;
            MEDIA_DESCRIBT_T*             pt_media_desc;

            i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc,&pt_media_desc);    
            if (i4_ret != SVCTXR_OK ||
                MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt))
            {
                return;   
            }    
        }
#endif /* TIME_SHIFT_SUPPORT */
        break;
#ifdef TIME_SHIFT_SUPPORT
    case SVCTX_NTFY_CODE_MEDIA_SPEED_UPDATE:
        break;
#endif /* TIME_SHIFT_SUPPORT */
    default:
        if (pt_svctx->b_is_blocked == TRUE) {
            return;
        }
        break;
    }

    DBG_INFO((_INFO_HEADER"%s() stream_type=%s, e_code=%s, e_nfy_cond=%s\r\n", 
              __func__,
              s_g_strm_types[e_stream_type],
              s_g_svctx_ntfy_codes[e_code],
              s_g_state_strings[pt_svctx->e_state]
              ));
    /*Only PVR need send this msg*/
    if(e_code == SVCTX_NTFY_CODE_NO_AUDIO_VIDEO_SVC)
    {
        INT32                         i4_ret = SVCTXR_OK;
        MEDIA_DESCRIBT_T*             pt_media_desc = NULL;

        i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc, &pt_media_desc);
        if (i4_ret != SVCTXR_OK)
        {
           return ;
        }
        /*maybe svc has been stopped first, the pointer will be set to NULL*/
        if(!pt_running_svc_req->pt_strm_fmt_chker)
        {
           return;
        }
        if(!(pt_running_svc_req->pt_strm_fmt_chker->ui4_strm_format_cond
            & MAKE_BIT_MASK_32(ST_AUDIO)) &&
            !(pt_running_svc_req->pt_strm_fmt_chker->ui4_strm_format_cond
            & MAKE_BIT_MASK_32(ST_VIDEO)) &&
             (!pt_media_desc->b_is_scdb_updated) &&
             (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS) &&
             (pt_media_desc->t_media_fmt.t_media_subtype.u.e_ts_subtype
                    == MEDIA_TS_SUBTYPE_TS_192_ENCRYPTION))
        {
            svctx_notify_clients_svc_msg(
                            pt_svctx,
                            SVCTX_NTFY_CODE_MEDIA_NO_CONTENT,
                            ST_UNKNOWN,
                            pt_svctx->pt_running_svc_req->h_svctx,
                            pt_svctx->pt_running_svc_req->pf_svc_nfy
                            );
            return;
        }
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
 *      mm_hdlr_choose_strm_filter
 * Description
 *      This API is used to select proper stream filter when the client doesn't
 *      provide one when invoking x_svctx_select_svc()
 * Input arguments
 *      pt_svctx        the pointer points to an SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK       the routine is successfully
 *---------------------------------------------------------------------------*/
VOID mm_hdlr_choose_strm_filter(
                    SVCTX_T*                      pt_svctx,
                    STREAM_TYPE_T                 e_strm_type,
                    x_svctx_strm_comp_filter_fct* ppt_strm_fltr_fct,
                    VOID**                        ppv_strm_fltr_tag
                    )
{
    MM_SVC_REQ_T* pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    /* DTV00001085 chose the proper stream filter function
     * 1. specified in x_svc_select_svc
     * 2. specified as default stream filter function
     * 3. system default (first one)
     */
    if (e_strm_type >= STRM_TYPE_COUNT || e_strm_type == ST_UNKNOWN)
    {
        return;
    }
    if (pt_running_svc_req->t_strm_reqs[e_strm_type].pt_strm_fltr_fct != NULL) {
        *ppt_strm_fltr_fct = pt_running_svc_req->t_strm_reqs[e_strm_type].pt_strm_fltr_fct;
        *ppv_strm_fltr_tag = pt_running_svc_req->t_strm_reqs[e_strm_type].pv_strm_fltr_tag;
    } else {
        if (pt_svctx->pt_def_strm_fltr_fcts[e_strm_type] != NULL) {
            *ppt_strm_fltr_fct = pt_svctx->pt_def_strm_fltr_fcts[e_strm_type];
            *ppv_strm_fltr_tag = pt_svctx->pv_def_strm_fltr_tags[e_strm_type];
        } else {
            *ppt_strm_fltr_fct = _mm_hdlr_default_strm_comp_filter;
            *ppv_strm_fltr_tag = (VOID*)pt_svctx;
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_check_strms_status
 * Description
 * Input arguments
 *      pt_svctx    a pointer points to the Service Context
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
VOID mm_hdlr_check_strms_status(
                    SVCTX_T*                    pt_svctx
                    )
{
    UINT32            ui4_strm_present_cond    = 0;
    UINT32            ui4_strm_scrambled_cond  = 0;
    UINT32            ui4_old_strm_format_cond = 0;
    MM_SVC_REQ_T*     pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    SVCTX_NTFY_CODE_T e_code;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    ui4_old_strm_format_cond  = strm_fmt_chker_get_format_status(pt_running_svc_req->pt_strm_fmt_chker);

    if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream) == TRUE)
    {
        /* check scrambled or not */
        if (svctx_is_scrambled_strm(&pt_running_svc_req->t_strm_reqs[ST_VIDEO].t_ca)) 
        {
            ui4_strm_scrambled_cond |= ST_MASK_VIDEO;
        } 
        else 
        {
            if ((ui4_old_strm_format_cond & ST_MASK_VIDEO) == 0)
            {
                /* video format not available */
                return;
            }

            /* video format is available */
            if (svctx_is_valid_video_fmt(pt_svctx) == TRUE)
            {
                ui4_strm_present_cond |= ST_MASK_VIDEO;
            }
        }
    }
    if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_AUDIO].h_stream) == TRUE)
    {
        /* check scrambled or not */
        if (svctx_is_scrambled_strm(&pt_running_svc_req->t_strm_reqs[ST_AUDIO].t_ca)) 
        {
            ui4_strm_scrambled_cond |= ST_MASK_AUDIO;
        } 
        else 
        {
            /* audio is opened */
            if ((ui4_old_strm_format_cond & ST_MASK_AUDIO) == 0)
            {
                /* audio format not available */
                return;
            }

            /* audio format is available */
            if (svctx_is_valid_audio_fmt(pt_svctx) == TRUE)
            {
                ui4_strm_present_cond |= ST_MASK_AUDIO;
            }
        }
    }

    strm_fmt_chker_force_present_status(pt_running_svc_req->pt_strm_fmt_chker, ui4_strm_present_cond);

    if ((ui4_strm_present_cond & ST_MASK_VIDEO) != 0) 
    {
        if ((ui4_strm_present_cond & ST_MASK_AUDIO) != 0) 
        { /* audio + video */
            /* clear audio + clear video  */
            e_code = SVCTX_NTFY_CODE_AUDIO_VIDEO_SVC;
        } 
        else 
        { /* video */
            if ((ui4_strm_scrambled_cond & ST_MASK_AUDIO) != 0) 
            {
                /* scrambled audio + clear video */
                e_code = SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_CLEAR_VIDEO_SVC;
            } 
            else 
            {
                e_code = SVCTX_NTFY_CODE_VIDEO_ONLY_SVC;
            }
        }
    }
    else 
    {
        if ((ui4_strm_present_cond & ST_MASK_AUDIO) != 0) 
        { /* audio */
            if ((ui4_strm_scrambled_cond & ST_MASK_VIDEO) != 0) 
            {
                /* clear audio + scrambled video */
                e_code = SVCTX_NTFY_CODE_SCRAMBLED_VIDEO_CLEAR_AUDIO_SVC;
            } 
            else 
            {
                /* clear audio + no video */
                e_code = SVCTX_NTFY_CODE_AUDIO_ONLY_SVC;
            }
        } 
        else 
        {
            if ((ui4_strm_scrambled_cond & ST_MASK_VIDEO) != 0) 
            {
                if ((ui4_strm_scrambled_cond & ST_MASK_AUDIO) != 0) 
                {
                    /* scrambled audio + scrambled video */
                    e_code = SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_VIDEO_SVC;
                } 
                else 
                {
                    /* no audio + scrambled video */
                    e_code = SVCTX_NTFY_CODE_SCRAMBLED_VIDEO_NO_AUDIO_SVC;
                }
            } 
            else 
            {
                if ((ui4_strm_scrambled_cond & ST_MASK_AUDIO) != 0) 
                {
                    /* scrambled audio + no video */
                    e_code = SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_NO_VIDEO_SVC;
                } 
                else 
                {
                    /* no audio + no video */
                    e_code = SVCTX_NTFY_CODE_NO_AUDIO_VIDEO_SVC;
                }
            }
        }
    }

    if (svctx_delay_no_video_related_nfy(
             pt_svctx,
             e_code,
             mm_hdlr_invoke_client_svc_nfy_fct) == TRUE) 
    {
        return;
    }

    mm_hdlr_invoke_client_svc_nfy_fct(
                    pt_svctx,
                    e_code,
                    ST_UNKNOWN
                    );
}

/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_subtl_get_timecode_callback
 * Description
 *      The API is used to get timecode for mp3 subtitle display when fast rewind. 
 * Input arguments
 *      pt_tag        the pointer points to an SVCTX_T object.
 * Output arguments
 *      ui8_timecode
 * Returns
 *      TRUE       the routine is successfully.
 *      FALSE     the routine is failed.
 *---------------------------------------------------------------------------*/
static BOOL _mm_hdlr_get_timecode_callback(
        VOID*                pv_tag,
        UINT64*                ui8_timecode
        )
{
    INT32                               i4_ret = 0;
    SIZE_T                              z_info_len = 0;
    MM_SVC_TIME_CODE_INFO_T             t_tm_code;
    /*MM_SPEED_TYPE_T                     e_speed;*/
    MEDIA_DESCRIBT_T*                   pt_media_desc = NULL;
    MM_SVC_REQ_T*                       pt_running_svc_req =  NULL;
    SVCTX_T*                            pt_svctx = NULL;
    
    x_memset(&t_tm_code,0,sizeof(MM_SVC_TIME_CODE_INFO_T));
    /*check parameters*/
    if (pv_tag == NULL || ui8_timecode ==NULL)
    {
        return FALSE;
    }
    
    pt_svctx = (SVCTX_T*)pv_tag;
    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    *ui8_timecode = (UINT64)(~0);
    
    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc,&pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        return FALSE;
    }
    /*check states*/
    if (pt_svctx->b_in_stopping || pt_svctx->pt_running_svc_req == NULL)
    {
        return FALSE;
    }
    
    /*e_speed = pt_media_desc->e_speed;*/
    if (!(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_AUDIO_ES && 
          (pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_MP3 ||
           pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_WMA)))
    {
        return FALSE;
    }
    /*get timecode*/
    z_info_len = sizeof(MM_SVC_TIME_CODE_INFO_T);
    t_tm_code.t_tm_code_type = MM_SVC_TM_CODE_TYPE_ALL;
    i4_ret = _mm_hdlr_get(pt_svctx, SVCTX_MM_GET_TYPE_TIME_CODE, &t_tm_code, &z_info_len);
    if (i4_ret != SVCTXR_OK)
    {
        return FALSE;
    }
    
    *ui8_timecode = t_tm_code.ui8_time;
    
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_open_strm
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
 *---------------------------------------------------------------------------*/
INT32 mm_hdlr_open_strm(
                    SVCTX_T*                    pt_svctx,
                    STRM_REQ_T*                 pt_strm_req,
                    BOOL                        b_auto_play
                    )
{
    SM_COMMAND_T      t_sm_cmd[20];
    MM_SVC_REQ_T*     pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    INT32             i4_ret             = SVCTXR_OK;
    STREAM_TYPE_T     e_strm_type        = svctx_get_strm_req_strm_type(pt_strm_req);
    UINT8             ui1_idx            = 0;
    STREAM_COMP_ID_T  t_strm_comp_id;
    MEDIA_DESCRIBT_T* pt_media_desc = NULL;
    SBTL_MKV_SBTL_CMD_T t_mkv_sbtl_cmd ;
    MINFO_INFO_T        t_minfo_data;
    /*add for font info*/
    WGL_FONT_INFO_T t_wgl_font_info;
    FONT_INFO_T*    pt_cur_font = NULL;
    MM_SUBTL_PTS_CB_T    t_mm_subtl_pts_cb;        
    UINT32              ui4_scdb_tag = 0;
    UINT32              ui4_scdb_idx = 0;
    UINT16              ui2_rec_nums = 0;       
    MM_CRYPT_INFO_T     t_crypt_info;    

    x_memset(&t_minfo_data,0,sizeof(MINFO_INFO_T));
    x_memset(&t_mkv_sbtl_cmd,0,sizeof(SBTL_MKV_SBTL_CMD_T));
#if 1
    BOOL              b_is_pvr_file = FALSE;
#endif
#ifdef DIVX_PLUS_CER
    SBTL_DISPLAY_INFO_CMD_T t_video_disp_info;
#endif

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    x_memset(&t_minfo_data, 0, sizeof(MINFO_INFO_T));
    x_memset(&t_crypt_info, 0, sizeof(MM_CRYPT_INFO_T));
    

    i4_ret = _mm_hdlr_get_media_obj(pt_running_svc_req->h_media_desc,&pt_media_desc);
    if(i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }

    if (pt_strm_req->e_req_type == STRM_REQ_PID)
    {
        t_strm_comp_id.e_type        = e_strm_type;
        t_strm_comp_id.pv_stream_tag = NULL;
    }
    else /* STRM_REQ_SCDB */
    {
        t_strm_comp_id = pt_strm_req->u.t_scdb.t_comp_id;
    }
#ifdef ENABLE_NAUTILUS
    if(pt_media_desc->t_crypt_info.e_mm_crypt_type == MM_CRYPT_TYPE_MLN)
    {
        x_memcpy(&t_crypt_info, &(pt_media_desc->t_crypt_info), sizeof(MM_CRYPT_INFO_T));
    }
    else
#endif
    {
        t_crypt_info.u.t_pvr_crypt.e_mode = MM_CRYPT_MODE_INVALID;
    }

#if 0
    if (MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt))
    {
        i4_ret = rec_util_get_sess_key(pt_media_desc->h_rec_util, &t_crypt_info);
        if (i4_ret != REC_UTILR_OK)
        {
            ASSERT(FALSE, ("%s(), rec_util_get_sess_key() failed\r\n", __func__));
        }
    }
#endif
#if 0
    if(SVCTXR_OK == svctx_rec_check_pvr_file(pt_media_desc->ps_media_path, 
                                            &b_is_pvr_file))
    {
        if(b_is_pvr_file)
        {
            svctx_rec_get_crypt_info(&t_crypt_info);
            t_crypt_info.e_mm_crypt_type = MM_CRYPT_TYPE_PVR;
        }
    }
#endif
    if (pt_media_desc->e_src_sub_type == SRC_SUB_TYPE_VESTEL_TS)
    {
        x_memcpy(&t_crypt_info, &(pt_media_desc->t_crypt_info), sizeof(MM_CRYPT_INFO_T));
        b_is_pvr_file = TRUE;
    }
    
    else if (MM_IS_TIME_SHIFT_CMPB(pt_media_desc->t_media_fmt))
    {
        x_memcpy(&t_crypt_info, &(pt_media_desc->t_crypt_info), sizeof(MM_CRYPT_INFO_T));
        x_dbg_stmt("t_crypt_info mode %d, key: %d, %d, %d, %d, %d, %d, %d, %d, index %d\n",
            t_crypt_info.u.t_pvr_crypt.e_mode, 
            t_crypt_info.u.t_pvr_crypt.aui4_odd_key_1[0],t_crypt_info.u.t_pvr_crypt.aui4_odd_key_1[1],
            t_crypt_info.u.t_pvr_crypt.aui4_odd_key_2[0],t_crypt_info.u.t_pvr_crypt.aui4_odd_key_2[1],
            t_crypt_info.u.t_pvr_crypt.aui4_even_key_1[0],t_crypt_info.u.t_pvr_crypt.aui4_even_key_1[1],
            t_crypt_info.u.t_pvr_crypt.aui4_even_key_2[0],t_crypt_info.u.t_pvr_crypt.aui4_even_key_2[1],
            t_crypt_info.u.t_pvr_crypt.e_key_idx);
    }
    switch(e_strm_type) {
    case ST_VIDEO:
        x_dbg_stmt(_INFO_HEADER"open stream type ST_VIDEO\n");
        if (pt_strm_req->e_req_type == STRM_REQ_PID)
        {
            t_sm_cmd[ui1_idx].e_code       = SM_VS_CMD_CODE_SET_FORCE_PID;
            t_sm_cmd[ui1_idx++].u.ui2_data = (UINT16)pt_strm_req->u.t_pid.t_mpeg_2_pid;
        }
        t_sm_cmd[ui1_idx].e_code      = SM_VS_CMD_CODE_SET_VIDEO_REGION;
        t_sm_cmd[ui1_idx++].u.pv_data = &(pt_svctx->t_video_region);
        t_sm_cmd[ui1_idx].e_code      = SM_VS_CMD_CODE_SET_DISP_REGION;
        t_sm_cmd[ui1_idx++].u.pv_data = &(pt_svctx->t_disp_region);
#if (defined(MW_MM_DUAL_VID_SUPPORT) && !(defined(MW_MM_FAKE_DAUL_VID))) 
        /*3d second track*/
        if ( pt_media_desc->b_multi_track_3d &&
             (pt_running_svc_req->t_strm_reqs[ST_VIDEO].e_strm_cond == SM_COND_OPENED ||
             pt_running_svc_req->t_strm_reqs[ST_VIDEO].e_strm_cond == SM_COND_OPENING))
        {
            t_sm_cmd[ui1_idx].e_code      =  SM_VS_CMD_CODE_SET_VIDEO_DECODER;
            t_sm_cmd[ui1_idx++].u.pv_data = NULL;
            t_sm_cmd[ui1_idx].e_code      = SM_VS_CMD_CODE_SET_LOGICAL_DESC;
            t_sm_cmd[ui1_idx++].u.ps_text = SN_PRES_SUB_DISPLAY; 

            /* enable VDP multi-input port*/
            /*          
            t_sm_cmd[ui1_idx].e_code      =  SM_VS_CMD_CODE_SET_VIDEO_PLANE;
            t_sm_cmd[ui1_idx++].u.pv_data = NULL;
            t_sm_cmd[ui1_idx].e_code      = SM_VS_CMD_CODE_SET_IN_PORT;
            t_sm_cmd[ui1_idx++].u.ui1_data = 1; 
            */
        }
#endif
        if (0
#if (defined(TIME_SHIFT_SUPPORT) || defined(SYS_RECORD_SUPPORT))
            ||((MM_IS_TIME_SHIFT_CMPB(pt_media_desc->t_media_fmt) || b_is_pvr_file)&& 
               (t_crypt_info.e_mm_crypt_type == MM_CRYPT_TYPE_PVR)            &&
               (t_crypt_info.u.t_pvr_crypt.e_mode != MM_CRYPT_MODE_INVALID))
#endif
#ifdef ENABLE_NAUTILUS
            || (t_crypt_info.e_mm_crypt_type == MM_CRYPT_TYPE_MLN)
#endif
           )
        {
            t_sm_cmd[ui1_idx].e_code  = SM_VS_CMD_CODE_SET_MM_CRYPT_INFO;
            t_sm_cmd[ui1_idx++].u.pv_data = &t_crypt_info;
        }
        t_sm_cmd[ui1_idx].e_code      = SM_CMD_CODE_END;
        break;
    case ST_AUDIO:
        x_dbg_stmt(_INFO_HEADER"open stream type ST_AUDIO\n");
        if (0
#if (defined(TIME_SHIFT_SUPPORT) || defined(SYS_RECORD_SUPPORT))
            ||((MM_IS_TIME_SHIFT_CMPB(pt_media_desc->t_media_fmt) || b_is_pvr_file)&& 
               (t_crypt_info.e_mm_crypt_type == MM_CRYPT_TYPE_PVR)            &&
               (t_crypt_info.u.t_pvr_crypt.e_mode != MM_CRYPT_MODE_INVALID))
#endif
#ifdef ENABLE_NAUTILUS
            || (t_crypt_info.e_mm_crypt_type == MM_CRYPT_TYPE_MLN)
#endif
        )
        {
            t_sm_cmd[ui1_idx].e_code  = SM_AS_CMD_CODE_SET_MM_CRYPT_INFO;
            t_sm_cmd[ui1_idx++].u.pv_data = &t_crypt_info;
        }
        t_sm_cmd[ui1_idx].e_code = SM_CMD_CODE_END;
        break;
    case ST_CLOSED_CAPTION:
        x_dbg_stmt(_INFO_HEADER"open stream type ST_CLOSED_CAPTION\n");
        if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream) == FALSE) {
            DBG_ERROR((_ERROR_HEADER"%s() failed, no any video stream\r\n", __func__));
            return SVCTXR_FAIL;
        }

        if (x_handle_valid(pt_running_svc_req->h_gl_plane) == FALSE) {
            DBG_ERROR((_ERROR_HEADER"%s() failed, no graphic plane for cc widget\r\n", __func__));
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
   case ST_SUBTITLE:
        x_dbg_stmt(_INFO_HEADER"open stream type ST_SUBTITLE\n");
        if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)
        {
            if (x_handle_valid(pt_running_svc_req->t_strm_reqs[ST_VIDEO].h_stream) == FALSE) {
                DBG_ERROR((_ERROR_HEADER"%s() failed, no any video stream\r\n", __func__));
                return SVCTXR_FAIL;
            }
            
            if (x_handle_valid(pt_running_svc_req->h_gl_plane) == FALSE) {
                DBG_ERROR((_ERROR_HEADER"%s() failed, no graphic plane for subtitle widget\r\n", __func__));
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
            t_sm_cmd[ui1_idx].e_code    = SM_CMD_CODE_END;
            break;
        }
        t_strm_comp_id.e_type = ST_MM_SUBTITLE;

        i4_ret = _mm_hdlr_get_minfo_by_scdb_tag(&pt_strm_req->u.t_scdb.t_comp_id,
                                        pt_media_desc,
                                        &t_minfo_data);
#if 1//MM_LYRIC_SUPPORT
        ui1_idx = 0;
        if(pt_media_desc->b_is_lyric)
        {
            t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_OSD_HANDLE;
            t_sm_cmd[ui1_idx++].u.h_obj = pt_running_svc_req->h_gl_plane;;

            t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_SPEED;
            t_sm_cmd[ui1_idx++].u.ui4_data = 100;

            t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_SUBTITLE_FILE_HANDLE;
            t_sm_cmd[ui1_idx++].u.h_obj = pt_media_desc->h_lyric;

            t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_PREFERED_LANG_LIST;
            t_sm_cmd[ui1_idx++].u.ps_text = pt_media_desc->pt_lyric_info->ps_prf_lang_lst;

            t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_SUBTITLE_TYPE;
            t_sm_cmd[ui1_idx++].u.ui4_data = MM_SBTL_TYPE_LYRIC_LRC;

            t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_PAGE_INFO;
            t_sm_cmd[ui1_idx++].u.pv_data = &pt_svctx->t_mm_sbtl_lyric_info.t_page;
            t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_ADJ_TM_OFST;
            t_sm_cmd[ui1_idx++].u.pv_data = &pt_svctx->t_mm_sbtl_lyric_info.t_tm_ofst;
            t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_ENCODING_TYPE;
            t_sm_cmd[ui1_idx++].u.ui4_data = pt_svctx->t_mm_sbtl_lyric_info.e_enc;
            t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_OSD_FONT_INFO;
            t_sm_cmd[ui1_idx++].u.pv_data = &pt_svctx->t_mm_sbtl_lyric_info.t_font;
            t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_OSD_BG_COLOR;
            t_sm_cmd[ui1_idx++].u.pv_data = &pt_svctx->t_mm_sbtl_lyric_info.t_bg_clr;
            t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_OSD_FG_COLOR;
            t_sm_cmd[ui1_idx++].u.pv_data = &pt_svctx->t_mm_sbtl_lyric_info.t_fg_clr;
            t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_OSD_RECT;
            t_sm_cmd[ui1_idx++].u.pv_data = &pt_svctx->t_mm_sbtl_lyric_info.t_rect;
            t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_SHOW_HIDE;
            t_sm_cmd[ui1_idx++].u.b_boolean = pt_svctx->t_mm_sbtl_lyric_info.b_hide;
            
            t_mm_subtl_pts_cb.pf_pts_cb = _mm_hdlr_get_timecode_callback;
            t_mm_subtl_pts_cb.pv_tag = (VOID*)pt_svctx;
        
            t_sm_cmd[ui1_idx].e_code      = SM_MM_SBTL_CMD_CODE_SET_PTS_CB;
            t_sm_cmd[ui1_idx++].u.pv_data = (VOID*)(&t_mm_subtl_pts_cb);
            
            t_sm_cmd[ui1_idx].e_code      = SM_MM_SBTL_SET_TYPE_SRC_HTTP_FLAG;
            t_sm_cmd[ui1_idx++].u.b_boolean = pt_media_desc->b_is_ext_sbtl_http_file;
            
            t_sm_cmd[ui1_idx].e_code = SM_CMD_CODE_END;
            x_dbg_stmt(_INFO_HEADER"lyric subtitle\n");
        }
        else
#endif
        if(pt_media_desc->b_is_cur_ext_first) /* external subtitle case */
        {
            ui4_scdb_tag = (UINT32)pt_strm_req->u.t_scdb.t_comp_id.pv_stream_tag;
            ui4_scdb_idx = (ui4_scdb_tag & 0x000000ff);

            i4_ret = _mm_hdlr_get_support_strm_num(pt_media_desc,ST_VIDEO,&ui2_rec_nums);
            if(i4_ret != MINFOR_OK)
            {
                return SVCTXR_NO_RECORD;
            }
            if (ui4_scdb_idx >= ui2_rec_nums)
            {
                ui4_scdb_idx -= ui2_rec_nums;
            }
            else
            {
                return SVCTXR_FAIL;
            }
            
            i4_ret = _mm_hdlr_get_support_strm_num(pt_media_desc,ST_AUDIO,&ui2_rec_nums);
            if(i4_ret != MINFOR_OK)
            {
                return SVCTXR_NO_RECORD;
            }
            if (ui4_scdb_idx >= ui2_rec_nums)
            {
                ui4_scdb_idx -= ui2_rec_nums;
            }
            else 
            {
                return SVCTXR_FAIL;
            }

            i4_ret = _mm_hdlr_get_support_strm_num(pt_media_desc,ST_SUBTITLE,&ui2_rec_nums);
            if(i4_ret != MINFOR_OK)
            {
                return    SVCTXR_NO_RECORD;
            }
            if (ui4_scdb_idx >= ui2_rec_nums)
            {
                ui4_scdb_idx -= ui2_rec_nums;
            }
            else 
            {
                return SVCTXR_FAIL;
            }
            if(ui4_scdb_idx >= EXT_SUBTITLE_MAX_NUM)
            {
                return SVCTXR_FAIL;
            }

            t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_OSD_HANDLE;
            t_sm_cmd[ui1_idx++].u.h_obj = pt_running_svc_req->h_gl_plane;

            t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_SPEED;
            t_sm_cmd[ui1_idx++].u.ui4_data = 100;

            t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_SUBTITLE_FILE_HANDLE;
            t_sm_cmd[ui1_idx++].u.h_obj = pt_media_desc->t_mm_svctx_sbtl_desc[ui4_scdb_idx].h_ext_subtitle;

            if (pt_media_desc->pt_ext_sbtl_info != NULL && 
                pt_media_desc->pt_ext_sbtl_info->ps_prf_lang_lst != NULL)
            {
                t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_PREFERED_LANG_LIST;
                t_sm_cmd[ui1_idx++].u.ps_text = pt_media_desc->pt_ext_sbtl_info->ps_prf_lang_lst;
            }
#ifdef SUPPORT_SET_SBTL_ENCODING
            t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_ENCODING_TYPE;
            t_sm_cmd[ui1_idx++].u.ui4_data = MM_SBTL_ENCODE_ISO8859_2;
#endif            
            if (pt_media_desc->t_mm_svctx_sbtl_desc[ui4_scdb_idx].b_is_related) /* sub + idx case */
            {
                t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_STREAM_ID;
                t_sm_cmd[ui1_idx++].u.ui4_data = (UINT32)pt_media_desc->t_mm_svctx_sbtl_desc[ui4_scdb_idx].ui2_idx;
                t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_SUBTITLE_TYPE;
                t_sm_cmd[ui1_idx++].u.ui4_data = MM_SBTL_TYPE_VOBSUB;
                t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_INDEX_FILE_HANDLE;
                t_sm_cmd[ui1_idx++].u.h_obj = pt_media_desc->t_mm_svctx_sbtl_desc[ui4_scdb_idx].h_related_subtitle;
                t_sm_cmd[ui1_idx].e_code = SM_CMD_CODE_END;
            }
            else
            {
                t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_SUBTITLE_TYPE;
                t_sm_cmd[ui1_idx++].u.ui4_data = MM_SBTL_TYPE_TEXT;
                t_sm_cmd[ui1_idx].e_code = SM_CMD_CODE_END;
            }

            if(pt_svctx->t_mm_sbtl_lyric_info.b_is_font_bg_clr_set)
            {
                t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_SET_TYPE_FONT_BG_COLOR;
                t_sm_cmd[ui1_idx++].u.ui4_data = (UINT32)&pt_svctx->t_mm_sbtl_lyric_info.t_font_bg_color;
            }
            
            t_sm_cmd[ui1_idx].e_code      = SM_MM_SBTL_SET_TYPE_SRC_HTTP_FLAG;
            t_sm_cmd[ui1_idx++].u.b_boolean = pt_media_desc->b_is_ext_sbtl_http_file;
            
            t_sm_cmd[ui1_idx].e_code = SM_CMD_CODE_END;
            x_dbg_stmt(_INFO_HEADER"extra subtitle\n");
        }
        else if((pt_media_desc->t_media_fmt.e_media_type
                == MEDIA_TYPE_CONTAINER)
        && (pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype
                == MEDIA_CONTNR_SUBTYPE_MATROSKA))
        {
            /*add for font info*/
            /*add new font that needed by this subtitle to system*/
            _mm_hdlr_add_subtitle_font(&pt_strm_req->u.t_scdb.t_comp_id,
                                        pt_media_desc,
                                        &t_minfo_data);
            _mm_hdlr_mkv_strm_info_2_cmd(&t_minfo_data, &t_mkv_sbtl_cmd);

            t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_OSD_HANDLE;
            t_sm_cmd[ui1_idx++].u.h_obj = pt_running_svc_req->h_gl_plane;;

            t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_SPEED;
            t_sm_cmd[ui1_idx++].u.ui4_data = 100;

            t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_SUBTITLE_TYPE;
            switch (t_minfo_data.u.t_stm_attribute.u.t_mkv_sp_stm_attr.e_subtitle_type)
            {
            case MINFO_INFO_MKV_SUBTITLE_TYPE_TEXT_UTF8:
                t_sm_cmd[ui1_idx++].u.ui4_data = MM_SBTL_TYPE_MKV_SRT;
                break;

            case MINFO_INFO_MKV_SUBTITLE_TYPE_TEXT_SSA:
                t_sm_cmd[ui1_idx++].u.ui4_data = MM_SBTL_TYPE_MKV_SSA;
                break;

            case MINFO_INFO_MKV_SUBTITLE_TYPE_VOBSUB:
                t_sm_cmd[ui1_idx++].u.ui4_data = MM_SBTL_TYPE_MKV_VOBSUB;
                break;

            default:
                return SVCTXR_NOT_SUPPORT;
            }

            if(t_mkv_sbtl_cmd.t_mkv_sbtl_prv.ui8_codec_private_len != 0)
            {
                t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_CODE_PRIVATE;
                t_sm_cmd[ui1_idx].u.pv_data = (VOID*)&t_mkv_sbtl_cmd.t_mkv_sbtl_prv;
                ui1_idx++;
            }

            if(t_mkv_sbtl_cmd.t_mkv_sbtl_cont_encode.ui1_encoding_number != 0)
            {
                t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_MKV_ENCODING_LIST;
                t_sm_cmd[ui1_idx].u.pv_data = (VOID*)&t_mkv_sbtl_cmd.t_mkv_sbtl_cont_encode;
                ui1_idx++;
            }

            /*add for font info*/
            pt_cur_font = (FONT_INFO_T*)pt_media_desc->pt_font_info;
            while(pt_cur_font)
            {
                if(pt_cur_font->ui8_font_id == 
                    t_minfo_data.u.t_stm_attribute.u.t_mkv_sp_stm_attr.ui8_attach_link_id)
                {
                    x_strcpy(t_wgl_font_info.a_c_font_name, 
                                   (CHAR*)(pt_cur_font->ac_file_name));
                    t_wgl_font_info.e_font_cmap = FE_CMAP_ENC_UNICODE;
                    t_wgl_font_info.e_font_size   = FE_FNT_SIZE_MEDIUM;
                    /*t_wgl_font_info.e_font_style  = FE_FNT_STYLE_ITALIC;
                                    t_wgl_font_info.e_font_style  |= FE_FNT_STYLE_BOLD;*/
                    t_wgl_font_info.e_font_style  = FE_FNT_STYLE_REGULAR;
                    t_wgl_font_info.i2_width        = 0;
                
                    t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_OSD_FONT_INFO;
                    t_sm_cmd[ui1_idx].u.pv_data = (VOID*)&t_wgl_font_info;
                    ui1_idx++;

                    break;
                }
                pt_cur_font = pt_cur_font->pt_next_font;
            }    

            /*add divx info*/
#ifdef DIVX_PLUS_CER
            {
                VSH_REGION_INFO_T       t_vsh_disp_info;
                SCC_DISP_RESOLUTION_T   t_resolution;
                UINT16                  ui2_scrn_w;
                UINT16                  ui2_scrn_h;
                UINT16                  ui2_time_out = 10000;
                SIZE_T  z_info_len = sizeof(VSH_REGION_INFO_T);
                STRM_REQ_T* pt_video_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]);
                i4_ret = x_scc_disp_get_resolution(
                            pt_svctx->h_display_scc_comp,
                            &t_resolution);
                if(i4_ret != SVCTXR_OK)
                {
                    return SVCTXR_FAIL;
                }
                ui2_scrn_w = t_resolution.ui2_width;
                ui2_scrn_h = t_resolution.ui2_height;
                do{
                    i4_ret = x_sm_get(pt_video_strm_req->h_stream,
                                    SM_VSH_GET_TYPE_FINAL_DISP_REGION,
                                    (VOID*)&t_vsh_disp_info,
                                    &z_info_len);
                    ui2_time_out--;
                }while((i4_ret == SMR_CAN_NOT_OP_IN_THIS_STATE) && ui2_time_out);
                if(i4_ret != SVCTXR_OK)
                {
                    return SVCTXR_FAIL;
                }
                t_video_disp_info.b_display_info = TRUE;
                t_video_disp_info.t_rect_refer.i4_left 
                    = (INT32)((t_vsh_disp_info.ui4_x * ui2_scrn_w) / VSH_REGION_MAX_WIDTH);
                t_video_disp_info.t_rect_refer.i4_right 
                    = (INT32)(((t_vsh_disp_info.ui4_x + t_vsh_disp_info.ui4_width) 
                    * ui2_scrn_w) / VSH_REGION_MAX_WIDTH);
                t_video_disp_info.t_rect_refer.i4_top
                    = (INT32)((t_vsh_disp_info.ui4_y * ui2_scrn_h) / VSH_REGION_MAX_HEIGHT);
                t_video_disp_info.t_rect_refer.i4_bottom
                    = (INT32)(((t_vsh_disp_info.ui4_y + t_vsh_disp_info.ui4_height) 
                    * ui2_scrn_h) / VSH_REGION_MAX_HEIGHT);
                t_video_disp_info.ui4_scrn_width = (UINT32)ui2_scrn_w;
                t_video_disp_info.ui4_scrn_height = (UINT32)ui2_scrn_h;
                t_video_disp_info.ui1_b_mgn_percent = 5;     /*bottom > 5%*/
                t_video_disp_info.ui1_font_max_grade = 18; /*font height < 1/18*/
                t_video_disp_info.ui1_font_min_grade = 26; /*font height > 1/26*/
                t_video_disp_info.ui1_h_percent = 20;        /*height < 20%*/
                t_video_disp_info.ui1_l_mgn_percent = 5;     /*left > 5%*/
                t_video_disp_info.ui1_r_mgn_percent = 5;     /*right > 5%*/

                t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_SUBTITLE_DISPLAY_INFO;
                t_sm_cmd[ui1_idx].u.pv_data = (VOID*)&t_video_disp_info;
                ui1_idx++;
            }
#endif

            t_sm_cmd[ui1_idx].e_code = SM_CMD_CODE_END;
        }
        else if((pt_media_desc->t_media_fmt.e_media_type
                == MEDIA_TYPE_CONTAINER)
        && (pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype
                == MEDIA_CONTNR_SUBTYPE_AVI))
        {
           

            t_sm_cmd[0].e_code = SM_MM_SBTL_CMD_CODE_SET_OSD_HANDLE;
            t_sm_cmd[0].u.h_obj = pt_running_svc_req->h_gl_plane;

            t_sm_cmd[1].e_code = SM_MM_SBTL_CMD_CODE_SET_SPEED;
            t_sm_cmd[1].u.ui4_data = 100;

            t_sm_cmd[2].e_code = SM_MM_SBTL_CMD_CODE_SET_SUBTITLE_TYPE;
            t_sm_cmd[2].u.ui4_data = MM_SBTL_TYPE_DIVX;

            ui1_idx = 3;

            if(t_minfo_data.u.t_stm_attribute.u.t_avi_sp_stm_attr.ui4_subtitle_type ==
                MINFO_INFO_AVI_SUBTITLE_TYPE_BITMAP_DXSA)
            {
                t_sm_cmd[ui1_idx].e_code = SM_MM_SBTL_CMD_CODE_SET_DIVX_HD;

                ui1_idx ++;
            }

            t_sm_cmd[ui1_idx].e_code = SM_CMD_CODE_END;
        }
        else if (pt_media_desc->t_media_fmt.e_media_type== MEDIA_TYPE_PS)
        {
            t_sm_cmd[0].e_code = SM_MM_SBTL_CMD_CODE_SET_OSD_HANDLE;
            t_sm_cmd[0].u.h_obj = pt_running_svc_req->h_gl_plane;

            t_sm_cmd[1].e_code = SM_MM_SBTL_CMD_CODE_SET_SPEED;
            t_sm_cmd[1].u.ui4_data = 100;

            t_sm_cmd[2].e_code = SM_MM_SBTL_CMD_CODE_SET_SUBTITLE_TYPE;
            t_sm_cmd[2].u.ui4_data = MM_SBTL_TYPE_PS_VOBSUB;

            t_sm_cmd[3].e_code = SM_CMD_CODE_END;   
        }
        else if((pt_media_desc->t_media_fmt.e_media_type
                == MEDIA_TYPE_CONTAINER)
        && (pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype
                == MEDIA_CONTNR_SUBTYPE_MP4))
        {
            t_sm_cmd[0].e_code = SM_MM_SBTL_CMD_CODE_SET_OSD_HANDLE;
            t_sm_cmd[0].u.h_obj = pt_running_svc_req->h_gl_plane;

            t_sm_cmd[1].e_code = SM_MM_SBTL_CMD_CODE_SET_SPEED;
            t_sm_cmd[1].u.ui4_data = 100;

            t_sm_cmd[2].e_code = SM_MM_SBTL_CMD_CODE_SET_SUBTITLE_TYPE; 
            switch (t_minfo_data.u.t_stm_attribute.u.t_mp4_sp_stm_attr.e_subtitle_type)
            {
                case MINFO_INFO_MP4_SUBTITLE_TYPE_BLUERAY:
                    t_sm_cmd[2].u.ui4_data = MM_SBTL_TYPE_MP4_BLURAY;
                    break;
                case MINFO_INFO_MP4_SUBTITLE_TYPE_VOBSBTL:
                    t_sm_cmd[2].u.ui4_data = MM_SBTL_TYPE_MP4_VOBSUB;
                    break;
                default:
                    return SVCTXR_NOT_SUPPORT;
            }
            t_sm_cmd[3].e_code = SM_CMD_CODE_END;  
        }
        else
        {
            t_sm_cmd[0].e_code = SM_MM_SBTL_CMD_CODE_SET_OSD_HANDLE;
            t_sm_cmd[0].u.h_obj = pt_running_svc_req->h_gl_plane;

            t_sm_cmd[1].e_code = SM_MM_SBTL_CMD_CODE_SET_SPEED;
            t_sm_cmd[1].u.ui4_data = 100;

            t_sm_cmd[2].e_code = SM_MM_SBTL_CMD_CODE_SET_SUBTITLE_TYPE;
            t_sm_cmd[2].u.ui4_data = MM_SBTL_TYPE_DIVX;

            t_sm_cmd[3].e_code = SM_CMD_CODE_END;
        }
        break;
    default:
        return SVCTXR_NOT_SUPPORT;
    }
    /* WILLY_BAD_INTLVE*/
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT    
    if ( pt_running_svc_req->pt_player_enhance != NULL && pt_media_desc->t_video_thumbnail_info.h_surf == NULL_HANDLE)
    {
        if (e_strm_type == ST_AUDIO)
        {
            if (pt_media_desc->pt_player_vid == NULL)
            {
                pt_media_desc->pt_player_aud = pt_running_svc_req->pt_player_act;
            }
            else
            {
                if (pt_running_svc_req->pt_player != NULL && pt_running_svc_req->pt_player != pt_media_desc->pt_player_vid)
                {
                    pt_media_desc->pt_player_aud = pt_running_svc_req->pt_player;
                    
                }
                else
                {
                    pt_media_desc->pt_player_aud = pt_running_svc_req->pt_player_enhance;
                }
            }
            i4_ret = x_sm_open_stream(
                player_get_sess_handle(pt_media_desc->pt_player_aud),
                        &t_strm_comp_id,
                        t_sm_cmd,
                        _mm_hdlr_strm_nfy_fct,
                        (VOID*)(pt_svctx->h_sys_svctx),
                        b_auto_play,
                        &(pt_strm_req->h_stream)
                        );
        }
        else
        {
            if (pt_media_desc->pt_player_aud == NULL)
            {
                pt_media_desc->pt_player_vid = pt_running_svc_req->pt_player_act;
            }
            else
            {
                if (pt_running_svc_req->pt_player != NULL && pt_running_svc_req->pt_player != pt_media_desc->pt_player_aud)
                {
                    pt_media_desc->pt_player_vid = pt_running_svc_req->pt_player;
                }
                else
                {
                    pt_media_desc->pt_player_vid = pt_running_svc_req->pt_player_enhance;
                }
            }
            i4_ret = x_sm_open_stream(
                player_get_sess_handle(pt_media_desc->pt_player_vid),
                    &t_strm_comp_id,
                    t_sm_cmd,
                    _mm_hdlr_strm_nfy_fct,
                    (VOID*)(pt_svctx->h_sys_svctx),
                    b_auto_play,
                    &(pt_strm_req->h_stream)
                    );
        }
    }
    else
#endif        
    {
        i4_ret = x_sm_open_stream(
                    player_get_sess_handle(pt_running_svc_req->pt_player_act),
                    &t_strm_comp_id,
                    t_sm_cmd,
                    _mm_hdlr_strm_nfy_fct,
                    (VOID*)(pt_svctx->h_sys_svctx),
                    b_auto_play,
                    &(pt_strm_req->h_stream)
                    );
    }
    if (i4_ret != SMR_OK && i4_ret != SMR_PENDING) {
        x_dbg_stmt(_ERROR_HEADER"x_sm_open_stream() failed, i4_ret = %d line = %d\n", i4_ret, __LINE__);
        mm_hdlr_invoke_client_svc_nfy_fct(
                pt_svctx,
                SVCTX_NTFY_CODE_INTERNAL_ERROR,
                ST_UNKNOWN
                );
        if(t_mkv_sbtl_cmd.t_mkv_sbtl_cont_encode.at_encodings)
        {
            x_mem_free(t_mkv_sbtl_cmd.t_mkv_sbtl_cont_encode.at_encodings);
        }
        return SVCTXR_FAIL;
    }

    /*add for font info*/
    if((e_strm_type == ST_SUBTITLE) && 
        (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
        (pt_media_desc->t_media_fmt.t_media_subtype.u.e_contnr_subtype
                == MEDIA_CONTNR_SUBTYPE_MATROSKA))
    {
        /*remove unused font first*/
            _mm_hdlr_remove_subtitle_font(&pt_strm_req->u.t_scdb.t_comp_id,
                                           pt_media_desc,
                                           &t_minfo_data);
    }

    pt_strm_req->e_strm_cond = SM_COND_OPENING;

    if((e_strm_type == ST_SUBTITLE) && 
       (pt_media_desc->e_speed != MM_SPEED_TYPE_FORWARD_1X)&&
       x_handle_valid(pt_strm_req->h_stream) && 
       (pt_media_desc->t_media_fmt.e_media_type != MEDIA_TYPE_TS))
    {
        BOOL                b_set_sbtl_info;
        SIZE_T              z_set_info_size;      
        
        b_set_sbtl_info = FALSE;
        z_set_info_size = sizeof(BOOL);

        i4_ret = x_sm_set(pt_strm_req->h_stream,
                         SM_MM_SBTL_SET_TYPE_SHOW_HIDE,
                         &b_set_sbtl_info,
                         z_set_info_size);
        if (i4_ret!= SMR_OK)
        {
            x_dbg_stmt(_ERROR_HEADER"hide subtitle failed, ret = %d line = %d\n", i4_ret, __LINE__);
            return SVCTXR_FAIL;
        }
    }

    if(t_mkv_sbtl_cmd.t_mkv_sbtl_cont_encode.at_encodings)
    {
        x_mem_free(t_mkv_sbtl_cmd.t_mkv_sbtl_cont_encode.at_encodings);
    }
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_reload_stream
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *---------------------------------------------------------------------------*/
VOID mm_hdlr_reload_stream (
                    SVCTX_T*                    pt_svctx,
                    MM_SVC_REQ_T*               pt_running_svc_req,
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

    if (e_stream_type < STRM_TYPE_COUNT && e_stream_type != ST_UNKNOWN)
    {
        pt_crnt_strm_req = &(pt_running_svc_req->t_strm_reqs[e_stream_type]);
    }
    else
    {
        return;
    }

    if ((pt_running_svc_req->t_parent.ui4_stream_mode & MAKE_BIT_MASK_32(e_stream_type)) == 0) {
        return;
    }

    if (pt_crnt_strm_req->e_req_type == STRM_REQ_SCDB) {
        mm_hdlr_choose_strm_filter(
                    pt_svctx,
                    e_stream_type,
                    &pt_strm_fltr_fct,
                    &pv_strm_fltr_tag
                    );
        /* get new audio stream component id */
        i4_ret = _mm_hdlr_get_strm_comp_id(
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

        /* if no stream is playing, don't need to do the comparison */
        if (pt_crnt_strm_req->u.t_scdb.t_comp_id.e_type == e_stream_type) {
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

    pt_strm_req->h_svctx         = pt_running_svc_req->t_parent.h_svctx;
    pt_strm_req->pf_strm_nfy     = pt_running_svc_req->t_parent.pf_svc_nfy;
    pt_strm_req->pv_strm_nfy_tag = pt_running_svc_req->t_parent.pv_svc_nfy_tag;

    t_msg.h_svctx       = pt_running_svc_req->t_parent.h_svctx;
    t_msg.ui1_msg_id    = _SVCTX_MSG_SELECT_STRM_REQ;
    t_msg.u.pt_strm_req = pt_strm_req;

    i4_ret = mm_hdlr_select_strm(pt_svctx, &t_msg);
}
/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_invoke_client_strm_nfy_fct
 * Description
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
VOID mm_hdlr_invoke_client_strm_nfy_fct(
                    SVCTX_T*                    pt_svctx,           /* in  */
                    SVCTX_NTFY_CODE_T           e_code,             /* in  */
                    STRM_REQ_T*                 pt_strm_req         /* in  */
                    )
{
    MM_SVC_REQ_T* pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    BOOL          b_check_svc_type   = FALSE;
    STREAM_TYPE_T e_strm_type        = svctx_get_strm_req_strm_type(pt_strm_req);

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    if (pt_strm_req == NULL || pt_svctx->b_is_blocked == TRUE) {
        return;
    }

    switch (e_code) {
    case SVCTX_NTFY_CODE_STREAM_STARTED:
      {/*removed for cr210632*/
        /*switch(e_strm_type) {
        case ST_VIDEO:
            strm_fmt_chker_start_delay(
                    pt_running_svc_req->pt_strm_fmt_chker,
                    ST_VIDEO
                    );
            break;
        case ST_AUDIO:
            if (!pt_strm_req->b_aux_audio)
    {
                strm_fmt_chker_start_delay(
                        pt_running_svc_req->pt_strm_fmt_chker,
                        ST_AUDIO
                        );
            }
            break;
        default:
            break;
        }*/
        break;
      }
    case SVCTX_NTFY_CODE_STREAM_STOPPED:
        switch(e_strm_type) {
        case ST_VIDEO:
        case ST_AUDIO:
            if (pt_svctx->e_state != SVCTX_COND_PRESENTING) {
                break;
            }
            if (pt_running_svc_req->pt_strm_fmt_chker)
            {
                if((pt_running_svc_req->pt_strm_fmt_chker->ui4_strm_format_cond
                    & MAKE_BIT_MASK_32(e_strm_type)) != 0)
                {
                    b_check_svc_type = TRUE;
                    strm_fmt_chker_reset_format_status(
                            pt_running_svc_req->pt_strm_fmt_chker,
                            e_strm_type
                            );
                }
            }
            break;
        default:
            break;
        }
        break;
    case SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE:
        /* on purpose to without break */
    case SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE:
    case SVCTX_NTFY_CODE_MEDIA_VS_CODEC_NOT_SUPPORT:
    case SVCTX_NTFY_CODE_MEDIA_AS_CODEC_NOT_SUPPORT:
    case SVCTX_NTFY_CODE_MEDIA_FRAMERATE_NOT_SUPPORT:
        strm_fmt_chker_set_format_status(
                 pt_running_svc_req->pt_strm_fmt_chker,
                 e_strm_type
                 );
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
        mm_hdlr_check_strms_status(pt_svctx);
    }
}

/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_get_non_fake_cc_scdb_num_recs
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
INT32 mm_hdlr_get_non_fake_cc_scdb_num_recs(
                    SVCTX_T*                    pt_svctx,
                    UINT16*                     pui2_num_recs
                    )
{
    STREAM_COMP_ID_T t_comp_id;
    INT32            i4_ret;

    i4_ret = _mm_hdlr_get_strm_comp_id(
                    pt_svctx,
                    ST_CLOSED_CAPTION,
                    TRUE,
                    _mm_hdlr_cc_comp_count_filter,
                    (VOID*)pui2_num_recs,
                    &t_comp_id,
                    NULL
                    );

    return (i4_ret == SVCTXR_NOT_EXIST) ? SVCTXR_OK : SVCTXR_FAIL;
}
/*-----------------------------------------------------------------------------
 * Name: mm_hdlr_set_conn_attr
 *
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 mm_hdlr_set_conn_attr(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    CM_CTRL_TYPE_T              e_ctrl,
                    VOID*                       pv_ctrl_data
                    )
{
    MM_SVC_REQ_T*           pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T                h_conn;
    INT32                   i4_ret = CMR_OK;

    h_conn = player_get_conn_handle(pt_running_svc_req->pt_player);
    if (x_handle_valid(h_conn) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    /*i4_ret = x_cm_set(
                    h_conn,
                    e_ctrl,
                    pv_ctrl_data
                    );*/
    /*if (i4_ret >= CMR_OK)*/ {
        /*TBD*/
    }

    return svctx_translate_cmr_2_svctxr(i4_ret);
}
/*-----------------------------------------------------------------------------
 * Name
 *      mm_hdlr_get_scdb_num_recs
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
INT32 mm_hdlr_get_scdb_num_recs(
                    SVCTX_T*                    pt_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    UINT16*                     pui2_num_recs
                    )
{
    MM_SVC_REQ_T* pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    UINT32        ui4_db_version     = SCDB_NULL_VER_ID;
    HANDLE_T      h_scdb             = NULL_HANDLE;
    INT32         i4_ret;

    ASSERT(pt_running_svc_req != NULL, ("%s(), pt_running_svc_req is NULL\r\n", __func__));

    h_scdb = player_get_scdb_handle(pt_running_svc_req->pt_player);
    if (x_handle_valid(h_scdb) == FALSE) {
        return SVCTXR_WRONG_STATE;
    }

    i4_ret = x_scdb_get_num_recs(
                        h_scdb,
                        e_strm_type,
                        pui2_num_recs,
                        &ui4_db_version
                        );
    i4_ret = (i4_ret == SCDBR_OK) ? SVCTXR_OK : SVCTXR_FAIL;


    return i4_ret;
}

INT32 mm_hdlr_get(
                    SVCTX_T*                    pt_svctx,
                    SVCTX_GET_TYPE_T            e_get_type,
                    VOID*                       pv_get_info,
                    SIZE_T*                     pz_get_info_size
                    )
{
    return _mm_hdlr_get(
                        pt_svctx,
                        e_get_type,
                        pv_get_info,
                        pz_get_info_size
                        );
}

INT32 mm_hdlr_set(
                    SVCTX_T*                    pt_svctx,
                    SVCTX_SET_TYPE_T            e_set_type,
                    VOID*                       pv_set_info,
                    SIZE_T                      z_set_info_size
                    )
{
    return _mm_hdlr_set(
                        pt_svctx,
                        e_set_type,
                        pv_set_info,
                        z_set_info_size
                        );
}

/*-----------------------------------------------------------------------------
 * export methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      x_mm_hdlr_reg_handler
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *---------------------------------------------------------------------------*/
INT32 x_mm_hdlr_reg_handler(VOID )
{

    return mm_hdlr_init();
}

