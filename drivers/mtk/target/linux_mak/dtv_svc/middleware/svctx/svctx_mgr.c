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
 * $RCSfile: svctx_mgr.c,v $
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
#ifdef MW_TV_AV_BYPASS_SUPPORT
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
#include "time_msrt/x_time_msrt.h"
#include "svctx/activator.h"

#include "svctx/ch_hdlr/u_ch_hdlr.h"
#include "svctx/ch_hdlr/ch_svc_req.h"
#include "svctx/inp_hdlr/u_inp_hdlr.h"
#include "svctx/inp_hdlr/inp_svc_req.h"
#ifdef SYS_RECORD_SUPPORT
#include "svctx/rec_hdlr/rec_svc_req.h"
#endif
#include "svctx/util/player.h"
#include "svctx/mm_hdlr/mm_svc_req.h"

#include "svctx/svctx_mgr.h"

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#define MAX_ASSOCIATION_MAP_ENTRY   ((INT32) 6)
#ifdef MW_PIP_POP_SUPPORT
#define MAX_EXCLUSION_MAP_ENTRY     ((INT32) 4)
#endif
#define MAX_ACTIVE_ASSOCIATION_LINK ((INT32) 3)

/* In some MHEG5 scenario, all of main, rec, TvBypas, monitor and sub can
 * be running simultanuously, so we need 5*2 for MAX. */
#define MAX_LINK_ENTRY              ((INT32) 10)
#define MAX_QUEUED_REQ              ((INT32) 16)

typedef enum _CMP_RES_T
{
    CMP_RES_POSTPONE,
    CMP_RES_FURTHER_EVALUATE,
    CMP_RES_FORCE_PROCEED
} CMP_RES_T;

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

typedef CMP_RES_T (*pf_compare_priority_fct)(
    SVC_SELECT_TYPE_T   e_svc_type,
    SVCTX_T*            pt_svctx_cnd,
    SVCTX_T*            pt_svctx_ref,
    const VAR_INFO_T*   pt_var_info,
    INT32               i4_cnt);

typedef BOOL (*pf_filter_candidate_fct)(
    SVCTX_T*            pt_svctx_cnd,
    SVCTX_T*            pt_svctx_ref);

/*-----------------------------------------------------------------------------
 * SVCTX private methods declarations
 *---------------------------------------------------------------------------*/
static ACTIVE_LINK_HEAD_T* _svctx_mgr_get_link_head_array(ACTIVE_LINK_TYPE_T e_link_type);
static INT32 _svctx_mgr_insert_obj(
    ACTIVE_LINK_HEAD_T*     pt_head,
    SVCTX_T*                pt_svctx,
    ACTIVE_LINK_ENTRY_T*    pt_new);
static VOID _svctx_mgr_link_qr(
    QUEUED_REQ_T**  ppt_head,
    QUEUED_REQ_T*   pt_new,
    BOOL            b_push_top);
static ACTIVE_LINK_ENTRY_T* _svctx_mgr_new_link_entry(SVCTX_T* pt_svctx);
static QUEUED_REQ_T* _svctx_mgr_new_qr(VOID);
static VOID _svctx_mgr_get_svc_status (
    PLAYER_TYPE_T       e_player_type,
    SVCTX_T*            pt_svctx,
    BOOL*               pb_svcst_ok,
    SVC_STATUS_T*       pt_svcst);

/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
static UINT32               _ui4_seq_num   = 0;
static INT32                _i4_assc_map_idx = 0;
static ASSOCIATION_T        _at_association_map[MAX_ASSOCIATION_MAP_ENTRY];
#ifdef MW_PIP_POP_SUPPORT
static INT32                _i4_excl_map_idx = 0;
static EXCLUSION_T          _at_exclusion_map[MAX_EXCLUSION_MAP_ENTRY];
#endif
static ACTIVE_LINK_HEAD_T   _at_active_link_1[MAX_ACTIVE_ASSOCIATION_LINK];
static ACTIVE_LINK_HEAD_T   _at_active_link_2[MAX_ACTIVE_ASSOCIATION_LINK];
static ACTIVE_LINK_ENTRY_T  _at_link_entry[MAX_LINK_ENTRY];
static QUEUED_REQ_T         _at_queued_req[MAX_QUEUED_REQ];

static ACTIVE_LINK_HEAD_T*  _pt_active_link_now = _at_active_link_1;
static ACTIVE_LINK_HEAD_T*  _pt_active_link_nxt = _at_active_link_2;
static QUEUED_REQ_T*        _pt_free_qr         = NULL;

static BOOL _svctx_mgr_exist_same_qr(
                const SVCTX_T*             pt_svctx,
                const ACTIVE_LINK_ENTRY_T* pt_entry,
                const VAR_INFO_T*          pt_ref_var_info)
{
    QUEUED_REQ_T* pt_qr = NULL;
    VAR_INFO_T*   pt_var_info = NULL;

    pt_qr = pt_entry->pt_qr_head;

    while (pt_qr)
    {
        pt_var_info = &(pt_qr->t_var_info);

        if (pt_qr->pt_svctx == pt_svctx
            && pt_var_info->ui1_msg_id == pt_ref_var_info->ui1_msg_id
            && pt_var_info->e_var_type == pt_ref_var_info->e_var_type
            && pt_var_info->e_op_type  == pt_ref_var_info->e_op_type)
        {
            if (VAR_TYPE_SERVICE == pt_var_info->e_var_type)
            {
                return TRUE;
            }
            else if (VAR_TYPE_PLAYER == pt_var_info->e_var_type
                     && pt_var_info->u.t_player_info.e_player_type  == pt_ref_var_info->u.t_player_info.e_player_type
                     && pt_var_info->u.t_player_info.pt_player      == pt_ref_var_info->u.t_player_info.pt_player
                     && pt_var_info->u.t_player_info.pf_invoke_func == pt_ref_var_info->u.t_player_info.pf_invoke_func)
            {
                return TRUE;
            }
            else if (VAR_TYPE_STREAM == pt_var_info->e_var_type
                     && pt_var_info->u.t_strm_info.h_stream    == pt_ref_var_info->u.t_strm_info.h_stream
                     && pt_var_info->u.t_strm_info.e_strm_type == pt_ref_var_info->u.t_strm_info.e_strm_type
                     && pt_var_info->u.t_strm_info.i4_idx      == pt_ref_var_info->u.t_strm_info.i4_idx
                     && pt_var_info->u.t_strm_info.b_aux_audio == pt_ref_var_info->u.t_strm_info.b_aux_audio
                     && pt_var_info->u.t_strm_info.b_auto_play == pt_ref_var_info->u.t_strm_info.b_auto_play)
            {
                return TRUE;
            }
        }
        pt_qr = pt_qr->pt_next;
    }
    return FALSE;
}

static INT32 _svctx_mgr_append_qr(
    ACTIVE_LINK_ENTRY_T*    pt_entry,
    SVCTX_T*                pt_svctx,
    const VAR_INFO_T*       pt_var_info)
{
    INT32           i4_ret = SVCTXR_NO_RESOURCE;
    QUEUED_REQ_T*   pt_qr;

    if (_svctx_mgr_exist_same_qr(pt_svctx, pt_entry, pt_var_info))
    {
#ifdef CLI_LVL_ALL
        DBG_SVCTXMGR_MSG((
                "{SVCTX_MGR}(%s) same qr(type=%d) already exits, not to append new one.\n",
                pt_svctx->s_name, pt_var_info->e_var_type));
#endif
        return SVCTXR_OK;
    }

    pt_qr = _svctx_mgr_new_qr();

    if (pt_qr)
    {
        pt_qr->pt_svctx    = pt_svctx;
        pt_qr->t_var_info  = *pt_var_info;

        _svctx_mgr_link_qr(&(pt_entry->pt_qr_head), pt_qr, FALSE);

        pt_entry->ui4_num_qr++;

        i4_ret = SVCTXR_OK;
    }
    else
    {
        x_dbg_stmt("\n{SVCTX_MGR} ERROR! Create new qr failed!\n");
        i4_ret = SVCTXR_NO_RESOURCE;
    }

    return (i4_ret);
}

static INT32 _svctx_mgr_build_priority_link(
    SVCTX_T*            pt_svctx,
    SVC_SELECT_TYPE_T   e_type,
    ACTIVE_LINK_HEAD_T* pt_head)
{
    INT32                   i;
    ASSOCIATION_T*          pt_assc;
    UINT32                  ui4_src_flag;

    ui4_src_flag = (SVC_SELECT_TV == e_type) ? SVCTX_SVC_SELECT_FLAG_TV : SVCTX_SVC_SELECT_FLAG_AV;

    for (i = 0; i < _i4_assc_map_idx; i++)
    {
        pt_assc = &(_at_association_map[i]);

        if (((pt_svctx == pt_assc->pt_lead) || (IS_SVCTX_ASSOCIATE(pt_svctx) && (pt_svctx == pt_assc->pt_assc)))
            && (ui4_src_flag & pt_assc->ui4_src_mask) )
        {
#ifdef MW_PIP_POP_SUPPORT
            if (!IS_SVCTX_EXCLUDED(pt_assc->pt_assc->ui4_assc_state))
#endif
            {
                if (SVCTXR_OK != _svctx_mgr_insert_obj(pt_head, pt_assc->pt_assc, NULL))
                {
                    return SVCTXR_NO_RESOURCE;
                }
            }
        }
    }

    pt_head->e_type  = e_type;
    pt_head->pt_lead = (IS_SVCTX_ASSOCIATE(pt_svctx) 
#ifdef SYS_RECORD_SUPPORT
                        || IS_SVCTX_RECORD (pt_svctx)
#endif    
                       ? NULL : pt_svctx);

    return (SVCTXR_OK);
}

static BOOL _svctx_mgr_compare_precedence(
    SVCTX_T*        pt_svctx_cnd,
    SVC_REQ_T*      pt_svc_req_cnd,
    SVC_STATUS_T*   pt_svcst_cnd,
    SVCTX_T*        pt_svctx_ref,
    SVC_REQ_T*      pt_svc_req_ref,
    SVC_STATUS_T*   pt_svcst_ref,
    PLAYER_TYPE_T   e_player_type)
{
    BOOL        b_higher = TRUE;
    HANDLE_T*   ph_obj = NULL;
    
    switch (pt_svcst_cnd->e_phase)
    {
        case PLAYER_ACTION_PIPE_OPEN:
            ph_obj = pt_svcst_ref->ph_pipe;
            if (ph_obj)
            {
                b_higher = (CM_COND_OPENED == *(pt_svcst_ref->pe_pipe_cond));
            }

            /* special case for handling S-First switch */
            if (b_higher && PLAYER_TYPE_INP_SVC_VIDEO == e_player_type)
            {
                INP_SVC_REQ_T* pt_inp_sr_cnd = (INP_SVC_REQ_T*) pt_svc_req_cnd;
                INP_SVC_REQ_T* pt_inp_sr_ref = (INP_SVC_REQ_T*) pt_svc_req_ref;

                if ((DEV_AVC_COMBI ==
                     pt_inp_sr_cnd->t_src_info.t_av_src.e_video_type) &&
                    (pt_inp_sr_cnd->t_src_info.t_av_src.e_video_type ==
                     pt_inp_sr_ref->t_src_info.t_av_src.e_video_type) &&
                    ((pt_inp_sr_cnd->t_src_info.ui2_crnt_video_combi_index !=
                      pt_inp_sr_ref->t_src_info.ui2_crnt_video_combi_index) ||
                    ((pt_inp_sr_cnd->t_src_info.ui2_crnt_video_combi_index ==
                      pt_inp_sr_ref->t_src_info.ui2_crnt_video_combi_index) &&
                     (pt_inp_sr_cnd->b_combi_switch && pt_inp_sr_ref->b_combi_switch))))
                {
                    b_higher = FALSE;
                }
            }

            break;

        case PLAYER_ACTION_PIPE_CLOSE:
            ph_obj = pt_svcst_ref->ph_pipe;
            if (ph_obj)
            {
                b_higher = (CM_COND_CLOSED == *(pt_svcst_ref->pe_pipe_cond));
            }
            return (b_higher);

        case PLAYER_ACTION_CONN_OPEN:
            ph_obj = pt_svcst_ref->ph_conn;
            if (ph_obj)
            {
                b_higher = (CM_COND_CONNECTED  == *(pt_svcst_ref->pe_conn_cond));
            }
            break;

        case PLAYER_ACTION_CONN_CLOSE:
            ph_obj = pt_svcst_ref->ph_conn;
            if (ph_obj)
            {
                b_higher = (CM_COND_DISCONNECTED == *(pt_svcst_ref->pe_conn_cond));
            }
            return (b_higher);

        case PLAYER_ACTION_SESS_PLAY:
            ph_obj = pt_svcst_ref->ph_sess;
            if (ph_obj)
            {
                b_higher = (SM_SESS_COND_STARTING == *(pt_svcst_ref->pe_sess_cond) ||
                            SM_SESS_COND_STARTED  == *(pt_svcst_ref->pe_sess_cond));
            }
            break;

        case PLAYER_ACTION_SESS_OPEN:
            ph_obj = pt_svcst_ref->ph_sess;
            if (ph_obj)
            {
                b_higher = (SM_SESS_COND_OPENED   == *(pt_svcst_ref->pe_sess_cond) ||
                            SM_SESS_COND_STARTED  == *(pt_svcst_ref->pe_sess_cond) ||
                            SM_SESS_COND_STARTING == *(pt_svcst_ref->pe_sess_cond));
            }
            break;

        case PLAYER_ACTION_SESS_CLOSE:
            ph_obj = pt_svcst_ref->ph_sess;
            if (ph_obj)
            {
                b_higher = (SM_SESS_COND_CLOSED == *(pt_svcst_ref->pe_sess_cond));
            }

            return (b_higher);

        default:
            return (b_higher);
    }

    b_higher = (b_higher && (ph_obj == NULL || x_handle_valid(*ph_obj)));

    return (b_higher);    
}    

static CMP_RES_T _svctx_mgr_compare_priority_with_player_type(
    SVC_SELECT_TYPE_T   e_svc_type,
    SVCTX_T*            pt_svctx_cnd,
    SVCTX_T*            pt_svctx_ref,
    const VAR_INFO_T*   pt_var_info,
    INT32               i4_cnt)
{
    SVC_STATUS_T        t_svcst_cnd;
    SVC_STATUS_T        t_svcst_ref;
    BOOL                b_svcst_cnd, b_svcst_ref;
    
#ifdef MW_PIP_POP_SUPPORT
    if (!IS_SVCTX_ASSOCIATE(pt_svctx_cnd) && !IS_SVCTX_ASSOCIATE(pt_svctx_ref))
    {
        return CMP_RES_FURTHER_EVALUATE;
    }
#endif
    
    _svctx_mgr_get_svc_status (pt_var_info->u.t_player_info.e_player_type,
                               pt_svctx_cnd,
                               & b_svcst_cnd,
                               & t_svcst_cnd);

    _svctx_mgr_get_svc_status (pt_var_info->u.t_player_info.e_player_type,
                               pt_svctx_ref,
                               & b_svcst_ref,
                               & t_svcst_ref);
    
    if (b_svcst_cnd && b_svcst_ref)
    {
        if (FALSE == _svctx_mgr_compare_precedence (
            pt_svctx_cnd,
            pt_svctx_cnd->pt_running_svc_req,
            & t_svcst_cnd,
            pt_svctx_ref,
            pt_svctx_ref->pt_running_svc_req,
            & t_svcst_ref,
            pt_var_info->u.t_player_info.e_player_type))
        {
            return CMP_RES_POSTPONE;
        }
    }
    
    return CMP_RES_FURTHER_EVALUATE;
}    

static CMP_RES_T _svctx_mgr_compare_priority_with_service_type(
    SVC_SELECT_TYPE_T   e_svc_type,
    SVCTX_T*            pt_svctx_cnd,
    SVCTX_T*            pt_svctx_ref,
    const VAR_INFO_T*   pt_var_info,
    INT32               i4_cnt)
{
    if (0 == i4_cnt)
    {
        if (pt_svctx_cnd != pt_svctx_ref)
        {
            return CMP_RES_POSTPONE;
        }
        else
        {
            return CMP_RES_FURTHER_EVALUATE;
        }
    }
    else
    {
        if (pt_svctx_ref->b_in_stopping)
        {
            return CMP_RES_POSTPONE;
        }
        else
        {
            return CMP_RES_FURTHER_EVALUATE;
        }
    }
}

static CMP_RES_T _svctx_mgr_compare_priority_with_stream_type(
    SVC_SELECT_TYPE_T   e_svc_type,
    SVCTX_T*            pt_svctx_cnd,
    SVCTX_T*            pt_svctx_ref,
    const VAR_INFO_T*   pt_var_info,
    INT32               i4_cnt)
{
    CMP_RES_T       e_res = CMP_RES_FURTHER_EVALUATE;
    STREAM_TYPE_T   e_strm_type = pt_var_info->u.t_strm_info.e_strm_type;
    STRM_REQ_T*     pt_strm_req_ref = NULL;;

#ifdef MW_PIP_POP_SUPPORT
    if (!IS_SVCTX_ASSOCIATE(pt_svctx_cnd) && !IS_SVCTX_ASSOCIATE(pt_svctx_ref))
    {
        return CMP_RES_FURTHER_EVALUATE;
    }
#endif

    if (0 == (pt_svctx_ref->ui4_stream_mode & MAKE_BIT_MASK_32(e_strm_type)))
    {
        e_res = CMP_RES_FORCE_PROCEED;
    }
    else
    {
        if (pt_svctx_ref->pt_running_svc_req)
        {
            switch (e_svc_type)
            {
                case SVC_SELECT_TV:
                    if ((e_strm_type == ST_UNKNOWN) || (e_strm_type >= (STRM_TYPE_COUNT + 1))) {
                        DBG_ABORT(DBG_MOD_SVCTX);
                    }

                    pt_strm_req_ref = &(((CH_SVC_REQ_T*) (pt_svctx_ref->pt_running_svc_req))->t_strm_reqs[e_strm_type]);
                    break;

                case SVC_SELECT_AV:
                    if ((e_strm_type == ST_UNKNOWN) || (e_strm_type >= INP_STRM_TYPE_COUNT)) {
                        DBG_ABORT(DBG_MOD_SVCTX);
                    }

                    pt_strm_req_ref = &(((INP_SVC_REQ_T*) (pt_svctx_ref->pt_running_svc_req))->t_strm_reqs[e_strm_type]);
                    break;

                case SVC_SELECT_MM:
                    if ((e_strm_type == ST_UNKNOWN) || (e_strm_type >= STRM_TYPE_COUNT)) {
                        DBG_ABORT(DBG_MOD_SVCTX);
                    }
                    pt_strm_req_ref = &(((MM_SVC_REQ_T*) (pt_svctx_ref->pt_running_svc_req))->t_strm_reqs[e_strm_type]);
                    break;

                default:
                    break;
            }
        }
    }

    if (pt_strm_req_ref)
    {
        switch (pt_var_info->e_op_type)
        {
            case OP_TYPE_OPEN:
                if (
                    /* lower priority */
                    (pt_svctx_cnd->i1_prio_diff > pt_svctx_ref->i1_prio_diff &&
                     ((pt_strm_req_ref->b_open_queued) ||
                      (SM_COND_OPENED   != pt_strm_req_ref->e_strm_cond &&
                       SM_COND_STARTED  != pt_strm_req_ref->e_strm_cond &&
                       SM_COND_STARTING != pt_strm_req_ref->e_strm_cond)))
                    ||
                    /* higher priority */
                    (pt_svctx_cnd->i1_prio_diff < pt_svctx_ref->i1_prio_diff &&
                     SM_COND_CLOSED != pt_strm_req_ref->e_strm_cond &&
                     SM_COND_IGNORE != pt_strm_req_ref->e_strm_cond &&
                     !(SM_COND_OPENING == pt_strm_req_ref->e_strm_cond && pt_strm_req_ref->b_open_queued)))
                {
#ifdef CLI_LVL_ALL
                    DBG_SVCTXMGR_MSG((
                        "{SVCTX_MGR} >>> (%s) OPEN %s stream POSTPONED due to (%s) cond %s.\r\n",
                        pt_svctx_cnd->s_name,
                        s_g_stream[e_strm_type],
                        pt_svctx_ref->s_name,
                        s_g_strm_conds[pt_strm_req_ref->e_strm_cond]));
#endif
                    e_res = CMP_RES_POSTPONE;
                }

                break;

            case OP_TYPE_CLOSE:
                if ((pt_svctx_cnd != pt_svctx_ref && pt_strm_req_ref->b_close_queued) ||
                    (SM_COND_CLOSED != pt_strm_req_ref->e_strm_cond &&
                     !(SM_COND_OPENING == pt_strm_req_ref->e_strm_cond && pt_strm_req_ref->b_open_queued)))
                {
#ifdef CLI_LVL_ALL
                    DBG_SVCTXMGR_MSG((
                        "{SVCTX_MGR} >>> (%s) CLOSE %s stream POSTPONED due to (%s) cond %s.\r\n",
                        pt_svctx_cnd->s_name,
                        s_g_stream[e_strm_type],
                        pt_svctx_ref->s_name,
                        s_g_strm_conds[pt_strm_req_ref->e_strm_cond]));
#endif
                    e_res = CMP_RES_POSTPONE;
                }

                break;

            default:
                break;
        }
    }

    return (e_res);
}

static BOOL _svctx_mgr_filter_candidate(
    SVCTX_T*    pt_svctx_cnd,
    SVCTX_T*    pt_svctx_ref)
{
    if (pt_svctx_cnd == pt_svctx_ref)
    {
        return TRUE;
    }

    return FALSE;
}

static VOID _svctx_mgr_get_svc_status (
    PLAYER_TYPE_T       e_player_type,
    SVCTX_T*            pt_svctx,
    BOOL*               pb_svcst_ok,
    SVC_STATUS_T*       pt_svcst)
{
    SVC_REQ_T*  pt_svc_req = pt_svctx->pt_running_svc_req;

    *pb_svcst_ok = FALSE;
    
    if (!pt_svc_req)
    {
        *pb_svcst_ok = FALSE;
        return;
    }

#ifdef SYS_RECORD_SUPPORT
    if (IS_SVCTX_RECORD (pt_svctx))
    {
        *pt_svcst = ((REC_SVC_REQ_T *) pt_svc_req)->t_svcst;
        *pb_svcst_ok = TRUE;
    }
    else
#endif  
    {
        PLAYER_T*   pt_player = NULL;
        
        switch (e_player_type)
        {
            case PLAYER_TYPE_CH_SVC:
                pt_player = ((CH_SVC_REQ_T*) pt_svc_req)->pt_player;
                break;
                
            case PLAYER_TYPE_INP_SVC_VIDEO:
                pt_player = ((INP_SVC_REQ_T*) pt_svc_req)->pt_video_player;
                break;
                
            case PLAYER_TYPE_INP_SVC_AUDIO:
                pt_player = ((INP_SVC_REQ_T*) pt_svc_req)->pt_audio_player;
                break;
                
            default:
                DBG_ABORT(DBG_MOD_SVCTX);
        }
        
        if (pt_player && player_valid(pt_player))
        {
            pt_svcst->e_phase      = pt_player->e_phase;
            pt_svcst->ph_pipe      = & pt_player->h_pipe;
            pt_svcst->pe_pipe_cond = & pt_player->e_pipe_cond;
            pt_svcst->ph_conn      = & pt_player->h_conn;
            pt_svcst->pe_conn_cond = & pt_player->e_conn_cond;
            pt_svcst->ph_sess      = & pt_player->h_sess;
            pt_svcst->pe_sess_cond = & pt_player->e_sess_cond;
            *pb_svcst_ok = TRUE;
        }
        else
        {
            *pb_svcst_ok = FALSE;
        }
    }
}        

static VOID _svctx_mgr_free_qr(QUEUED_REQ_T* pt_qr)
{
    _svctx_mgr_link_qr(&_pt_free_qr, pt_qr, TRUE);
}

static VOID _svctx_mgr_free_link_entry(ACTIVE_LINK_ENTRY_T* pt_entry)
{
    pt_entry->pt_worker  = NULL;
    pt_entry->e_last_op  = OP_TYPE_NONE;
    while (pt_entry->pt_qr_head)
    {
        QUEUED_REQ_T* pt_tmp = pt_entry->pt_qr_head;
        pt_entry->pt_qr_head = pt_tmp->pt_next;
        _svctx_mgr_free_qr(pt_tmp);
        pt_entry->pv_tag     = NULL;
    }
    pt_entry->ui4_num_qr = 0;
}

static ACTIVE_LINK_HEAD_T* _svctx_mgr_get_link_head(
    ACTIVE_LINK_TYPE_T  e_link_type,
    SRC_TYPE_T          e_src_type)
{
    ACTIVE_LINK_HEAD_T* pt_head = NULL;
    SVC_SELECT_TYPE_T   e_svc_type;
    INT32               i;

    pt_head = _svctx_mgr_get_link_head_array(e_link_type);

    if (pt_head)
    {
        e_svc_type = svctx_mgr_convert_to_svc_type(e_src_type);

        if (SVC_SELECT_NONE != e_svc_type)
        {
            for (i = 0; i < MAX_ACTIVE_ASSOCIATION_LINK; i++)
            {
                if (e_svc_type == pt_head[i].e_type)
                {
                    return (&(pt_head[i]));
                }
            }
        }
    }

    return NULL;
}

static ACTIVE_LINK_HEAD_T* _svctx_mgr_get_link_head_array(
    ACTIVE_LINK_TYPE_T e_link_type)
{
    if (ACTIVE_LINK_NOW == e_link_type)
    {
        return (_pt_active_link_now);
    }
    else if (ACTIVE_LINK_NEXT == e_link_type)
    {
        return (_pt_active_link_nxt);
    }
    else
    {
        return NULL; /* invalid link type */
    }
}

static INT32 _svctx_mgr_insert_obj(
    ACTIVE_LINK_HEAD_T*     pt_head,
    SVCTX_T*                pt_svctx,
    ACTIVE_LINK_ENTRY_T*    pt_new)
{
    UINT32                  ui4_cnt = 0;
    ACTIVE_LINK_ENTRY_T*    pt_entry = NULL;
    ACTIVE_LINK_ENTRY_T*    pt_prev  = NULL;

    if (!pt_new)
    {
        pt_new = _svctx_mgr_new_link_entry(pt_svctx);
    }

    if (!pt_new)
    {
        return SVCTXR_NO_RESOURCE;
    }

    pt_entry = DLIST_HEAD(&(pt_head->t_list));

    if (pt_entry)
    {
        while (1)
        {
            if (!pt_entry)
            {
                ui4_cnt++;
                DLIST_INSERT_AFTER(pt_new, pt_prev, &(pt_head->t_list), t_next);
                break;
            }

            if (pt_svctx->i1_prio_diff <= pt_entry->pt_worker->i1_prio_diff)
            {
                ui4_cnt++;
                DLIST_INSERT_BEFORE(pt_new, pt_entry, &(pt_head->t_list), t_next);
                break;
            }

            pt_prev = pt_entry;
            pt_entry = DLIST_NEXT(pt_entry, t_next);
        }
    }
    else
    {
        ui4_cnt++;
        DLIST_INSERT_HEAD(pt_new, &(pt_head->t_list), t_next);
    }

    pt_new->pv_tag = (VOID*) pt_head;
    pt_head->ui4_cnt += ui4_cnt;

    return SVCTXR_OK;
}

static ACTIVE_LINK_ENTRY_T* _svctx_mgr_new_link_entry(SVCTX_T* pt_svctx)
{
    INT32 i;

    for (i = 0; i < MAX_LINK_ENTRY; i++)
    {
        if (!_at_link_entry[i].pt_worker)
        {
            _at_link_entry[i].pt_worker = pt_svctx;
            return (&(_at_link_entry[i]));
        }
    }

    return NULL;
}

static VOID _svctx_mgr_link_qr(
    QUEUED_REQ_T**  ppt_head,
    QUEUED_REQ_T*   pt_new,
    BOOL            b_push_top)
{
    if (b_push_top)
    {
        pt_new->pt_next = *ppt_head;
        *ppt_head = pt_new;
    }
    else
    {
        QUEUED_REQ_T* pt_ptr = *ppt_head;

        if (pt_ptr)
        {
            while (pt_ptr->pt_next)
            {
                pt_ptr = pt_ptr->pt_next;
            }

            pt_ptr->pt_next = pt_new;
        }
        else
        {
            *ppt_head = pt_new;
        }
    }
}

static QUEUED_REQ_T* _svctx_mgr_new_qr(VOID)
{
    QUEUED_REQ_T* pt_qr = NULL;

    if (_pt_free_qr)
    {
        pt_qr = _pt_free_qr;
        _pt_free_qr = _pt_free_qr->pt_next;
        pt_qr->pt_next = NULL;
    }
    else
    {
        DBG_ABORT(DBG_MOD_SVCTX);
    }

    return (pt_qr);
}

static VOID _svctx_mgr_release_active_link(ACTIVE_LINK_HEAD_T* pt_link_head)
{
    ACTIVE_LINK_ENTRY_T* pt_entry;

    while (NULL != (pt_entry = DLIST_HEAD(&(pt_link_head->t_list))))
    {
        DLIST_REMOVE(pt_entry, &(pt_link_head->t_list), t_next);

        _svctx_mgr_free_link_entry(pt_entry);
    }

    pt_link_head->e_type  = SVC_SELECT_NONE;
    pt_link_head->pt_lead = NULL;
    pt_link_head->ui4_cnt = 0;
}

static INT32 _svctx_mgr_send_queued_msg(
    const SVCTX_T*      pt_svctx,
    const VAR_INFO_T*   pt_var_info)
{
    SVCTX_MSG_T t_msg;

    t_msg.ui1_msg_id = pt_var_info->ui1_msg_id;

    switch (pt_var_info->ui1_msg_id)
    {
        case _SVCTX_MSG_QUEUED_SELECT_SVC_REQ:
            DBG_SVCTXMGR_MSG(("{SVCTX_MGR} Send _SVCTX_MSG_QUEUED_SELECT_SVC_REQ...\r\n"));
            t_msg.pf_invoke_func = NULL;
            t_msg.pv_tag         = NULL;
            break;

        case _SVCTX_MSG_QUEUED_CLOSE_STRM_NTFY:
            t_msg.pf_invoke_func = NULL;
            t_msg.u.t_strm.h_obj = pt_var_info->u.t_strm_info.h_stream;
            break;

        case _SVCTX_MSG_QUEUED_OPEN_STRM_NTFY:
        case _SVCTX_MSG_QUEUED_OPEN_AUDIO_STRM_NTFY:
        case _SVCTX_MSG_QUEUED_OPEN_VIDEO_STRM_NTFY:
            t_msg.pf_invoke_func = NULL;
            t_msg.pv_tag         = (VOID*)(UINT32) pt_var_info->u.t_strm_info.b_auto_play;
            t_msg.u.i4_idx       = pt_var_info->u.t_strm_info.i4_idx;
            break;

        case _PLAYER_MSG_QUEUED_PLAY_NTFY:
        case _PLAYER_MSG_QUEUED_STOP_NTFY:
        case _PLAYER_MSG_QUEUED_PAUSE_NTFY:
        case _PLAYER_MSG_QUEUED_PIPE_OPEN_NTFY:
        case _PLAYER_MSG_QUEUED_PIPE_CLOSE_NTFY:
        case _PLAYER_MSG_QUEUED_CONN_OPEN_NTFY:
        case _PLAYER_MSG_QUEUED_CONN_CLOSE_NTFY:
        case _PLAYER_MSG_QUEUED_SESS_OPEN_NTFY:
        case _PLAYER_MSG_QUEUED_SESS_PLAY_NTFY:
        case _PLAYER_MSG_QUEUED_SESS_CLOSE_NTFY:
        case _PLAYER_MSG_QUEUED_RECONNECT_NTFY:
            t_msg.pf_invoke_func = pt_var_info->u.t_player_info.pf_invoke_func;
            t_msg.pv_tag         = pt_var_info->u.t_player_info.pt_player;
            break;

#ifdef SYS_RECORD_SUPPORT
        case SVCTX_MSG_REC_QUEUED_PIPE_OPEN:
        case SVCTX_MSG_REC_QUEUED_PIPE_CLOSE:
        case SVCTX_MSG_REC_QUEUED_CONN_OPEN:
        case SVCTX_MSG_REC_QUEUED_CONN_CLOSE:
            t_msg.pf_invoke_func = pt_var_info->u.t_player_info.pf_invoke_func;
            t_msg.pv_tag         = (VOID *) pt_svctx;
            break;
#endif

        default:
            DBG_ABORT(DBG_MOD_SVCTX);
    }

    return (svctx_send_msg(pt_svctx->h_sys_svctx, &t_msg));
}

static BOOL _svctx_mgr_stream_excluded(
    const STREAM_INFO_T* pt_strm_info,
    OP_TYPE_T            e_op_type)
{
    if ((pt_strm_info->b_aux_audio && (OP_TYPE_CLOSE == e_op_type)) ||
        (ST_VIDEO != pt_strm_info->e_strm_type && ST_AUDIO != pt_strm_info->e_strm_type))
    {
        return TRUE;
    }

    return FALSE;
}

SVC_SELECT_TYPE_T svctx_mgr_convert_to_svc_type(SRC_TYPE_T e_type)
{
    switch (e_type)
    {
        case SRC_TYPE_BRDCST_SVL:
        case SRC_TYPE_BRDCST_FREQ:
            return SVC_SELECT_TV;

        case SRC_TYPE_AVC:
            return SVC_SELECT_AV;

        case SRC_TYPE_MEDIA_STORGE:
        case SRC_TYPE_MEDIA_MEM_CLIP:
        case SRC_TYPE_MEDIA_NETWORK:
        case SRC_TYPE_MEDIA_TIME_SHIFT:
        case SRC_TYPE_MEDIA_PVR:
            return SVC_SELECT_MM;

        default:
            return SVC_SELECT_NONE;
    }
}

INT32 svctx_mgr_add_association(
    SVCTX_T*                pt_svctx_lead,
    SVCTX_T*                pt_svctx_assc,
    const SVCTX_ASSC_CFG_T* pt_assc_cfg)
{
    INT32               i4_ret;
    ASSOCIATION_T*      pt_link;
    UINT32              ui4_assc_src_mask;

    i4_ret = SVCTXR_NO_RESOURCE;

    if (_i4_assc_map_idx < MAX_ASSOCIATION_MAP_ENTRY)
    {
        i4_ret = SVCTXR_INV_ARG;

        ui4_assc_src_mask = (SVCTX_SVC_SELECT_FLAG_TV | SVCTX_SVC_SELECT_FLAG_AV);

        if (pt_assc_cfg)
        {
            ui4_assc_src_mask &= (pt_assc_cfg->ui4_assc_src_mask);
        }

        if (0 != ui4_assc_src_mask)
        {
            i4_ret = SVCTXR_OK;
            pt_link = &(_at_association_map[_i4_assc_map_idx++]);
            pt_link->ui4_src_mask = ui4_assc_src_mask;
            pt_link->pt_lead      = pt_svctx_lead;
            pt_link->pt_assc      = pt_svctx_assc;

            if (pt_assc_cfg)
            {
                pt_link->t_assc_cfg = *pt_assc_cfg;
            }
            else
            {
                x_memset(&(pt_link->t_assc_cfg), 0, sizeof(pt_link->t_assc_cfg));
            }
        }
    }

    return (i4_ret);
}

#ifdef MW_PIP_POP_SUPPORT
INT32 svctx_mgr_add_exclusion(
    SVCTX_T*    pt_svctx_lead,
    SVCTX_T*    pt_svctx_excl,
    UINT32      ui4_excl_mask)
{
    INT32           i4_ret;
    EXCLUSION_T*    pt_link;
    UINT32          ui4_excl_src_mask;

    i4_ret = SVCTXR_NO_RESOURCE;

    if (_i4_excl_map_idx < MAX_EXCLUSION_MAP_ENTRY)
    {
        i4_ret = SVCTXR_INV_ARG;

        ui4_excl_src_mask = (SVCTX_SVC_SELECT_FLAG_TV | SVCTX_SVC_SELECT_FLAG_AV) & ui4_excl_mask;

        if (0 != ui4_excl_src_mask)
        {
            i4_ret = SVCTXR_OK;
            pt_link = &(_at_exclusion_map[_i4_excl_map_idx++]);
            pt_link->ui4_src_mask = ui4_excl_src_mask;
            pt_link->pt_lead      = pt_svctx_lead;
            pt_link->pt_excl      = pt_svctx_excl;
        }
    }

    return (i4_ret);
}
#endif

BOOL svctx_mgr_be_active(const SVCTX_T* pt_svctx)
{
    BOOL                    b_ok = FALSE;
    INT32                   i = 0;
    ACTIVE_LINK_ENTRY_T*    pt_link;

    while (1)
    {
        pt_link = svctx_mgr_get_link_obj_all(ACTIVE_LINK_NOW, i);

        if (pt_link)
        {
#ifdef MW_PIP_POP_SUPPORT
            if (!IS_SVCTX_EXCLUDED(pt_svctx->ui4_assc_state) &&
                (pt_svctx == pt_link->pt_worker))
#else
            if (pt_svctx == pt_link->pt_worker)
#endif
            {
                b_ok = TRUE;
                break;
            }
        }
        else
        {
            break;
        }

        i++;
    }

    return (b_ok);
}

BOOL svctx_mgr_be_associated(
    const SVCTX_T*  pt_lead,
    const SVCTX_T*  pt_assc)
{
    BOOL    b_ok = FALSE;
    INT32   i;

    if (pt_lead && pt_assc && !IS_SVCTX_ASSOCIATE(pt_lead) && IS_SVCTX_ASSOCIATE(pt_assc))
    {
        for (i = 0; i < _i4_assc_map_idx; i++)
        {
            if ((pt_assc == _at_association_map[i].pt_assc) &&
                (pt_lead == _at_association_map[i].pt_lead))
            {
                b_ok = TRUE;
                break;
            }
        }
    }

    return (b_ok);
}

BOOL svctx_mgr_be_associate_on(
    SRC_TYPE_T      e_src_type,
    const SVCTX_T*  pt_svctx)
{
    BOOL                b_ok = FALSE;
    INT32               i;
    SVC_SELECT_TYPE_T   e_svc_type;
    UINT32              ui4_src_flag;

    if (IS_SVCTX_ASSOCIATE(pt_svctx))
    {
        if (SVC_SELECT_NONE != (e_svc_type = svctx_mgr_convert_to_svc_type(e_src_type)))
        {
            ui4_src_flag = (SVC_SELECT_TV == e_svc_type) ? SVCTX_SVC_SELECT_FLAG_TV : SVCTX_SVC_SELECT_FLAG_AV;

            for (i = 0; i < _i4_assc_map_idx; i++)
            {
                if ((pt_svctx == _at_association_map[i].pt_assc) &&
                    (_at_association_map[i].ui4_src_mask & ui4_src_flag))
                {
                    b_ok = TRUE;
                }
            }
        }
    }

    return (b_ok);
}

#ifdef MW_PIP_POP_SUPPORT
BOOL svctx_mgr_be_excluded(
    const SVCTX_T*  pt_lead,
    const SVCTX_T*  pt_excl)
{
    BOOL    b_ok = FALSE;
    INT32   i;

    if (pt_lead && pt_excl && !IS_SVCTX_ASSOCIATE(pt_lead) && IS_SVCTX_ASSOCIATE(pt_excl))
    {
        for (i = 0; i < _i4_excl_map_idx; i++)
        {
            if ((pt_excl == _at_exclusion_map[i].pt_excl) &&
                (pt_lead == _at_exclusion_map[i].pt_lead))
            {
                b_ok = TRUE;
                break;
            }
        }
    }

    return (b_ok);
}
#endif

INT32 svctx_mgr_build_active_link(
    ACTIVE_LINK_TYPE_T  e_link_type,
    SRC_TYPE_T          e_src_type,
    SVCTX_T*            pt_svctx)
{
    INT32                   i;
    INT32                   i4_ret;
    ACTIVE_LINK_HEAD_T*     pt_head       = NULL;
    ACTIVE_LINK_HEAD_T*     pt_head_array = NULL;
    SVC_SELECT_TYPE_T       e_svc_type;
    UINT32                  ui4_retry;

    DBG_SVCTXMGR_MSG(("{SVCTX_MGR} Build active link for link type <%d>\r\n", e_link_type));

    i4_ret = SVCTXR_INV_ARG;

    if (SVC_SELECT_NONE != (e_svc_type = svctx_mgr_convert_to_svc_type(e_src_type)))
    {
        pt_head_array = _svctx_mgr_get_link_head_array(e_link_type);

        if (pt_head_array)
        {
            /* release all active link on the specified active link array*/
            for (i = 0; i < MAX_ACTIVE_ASSOCIATION_LINK; i++)
            {
                pt_head = &(pt_head_array[i]);
                ui4_retry = 0;

                while ((svctx_mgr_convert_to_svc_type(e_src_type) == pt_head->e_type) &&
                       (ui4_retry < REL_ACTIVE_LINK_MAX_RETRY_TIME))
                {
                    svctx_class_unlock();
                    x_thread_delay(100);
                    ui4_retry++;
                    svctx_class_lock();
                }

                if (SVC_SELECT_NONE != pt_head->e_type)
                {                
                    _svctx_mgr_release_active_link(pt_head);
                }                    
                
            }

            i4_ret = _svctx_mgr_build_priority_link(
                pt_svctx,
                e_svc_type,
                &(pt_head_array[0]));
        }
    }

    return (i4_ret);
}

INT32 svctx_mgr_check_queued_req(
    ACTIVE_LINK_TYPE_T  e_link_type,
    SRC_TYPE_T          e_src_type,
    SVCTX_T*            pt_svctx,
    BOOL                b_no_check)
{
    INT32                   i4_ret = SVCTXR_OK;
    ACTIVE_LINK_HEAD_T*     pt_head;
    ACTIVE_LINK_ENTRY_T*    pt_entry;
    QUEUED_REQ_T*           pt_qr_head;
    QUEUED_REQ_T*           pt_tmp;

    DBG_SVCTXMGR_MSG(("\r\n{SVCTX_MGR} Starting svctx_mgr_check_queued_req...\r\n\r\n"));

    pt_head = _svctx_mgr_get_link_head(e_link_type, e_src_type);

    if (pt_head)
    {
        pt_entry = DLIST_HEAD(&(pt_head->t_list));

        while (pt_entry)
        {
            if ((NULL == pt_svctx) ||
                (pt_entry->pt_worker == pt_svctx))
            {
                if (pt_entry->pt_qr_head) /* REQs queued */
                {
                    pt_qr_head = pt_entry->pt_qr_head;
                    pt_entry->pt_qr_head = NULL;
                    pt_entry->ui4_num_qr = 0;

                    while (pt_qr_head)
                    {
                        if ((b_no_check) ||
                            (TRUE == svctx_mgr_ready_to_proceed(
                                e_link_type,
                                e_src_type,
                                pt_qr_head->pt_svctx,
                                &(pt_qr_head->t_var_info))))
                        {
                            DBG_SVCTXMGR_MSG((
                                "{SVCTX_MGR} Send (%s) queued req. VAR(%d), OP(%d).\r\n",
                                pt_qr_head->pt_svctx->s_name,
                                pt_qr_head->t_var_info.e_var_type,
                                pt_qr_head->t_var_info.e_op_type));

                            /* send message */
                            _svctx_mgr_send_queued_msg(
                                pt_qr_head->pt_svctx,
                                &(pt_qr_head->t_var_info));
                        }

                        pt_tmp = pt_qr_head;
                        pt_qr_head = pt_qr_head->pt_next;

                        _svctx_mgr_free_qr(pt_tmp);
                    }
                }
            }
            pt_entry = DLIST_NEXT(pt_entry, t_next);
        }
    }

    DBG_SVCTXMGR_MSG(("\r\n{SVCTX_MGR} End of svctx_mgr_check_queued_req...\r\n\r\n"));

    return (i4_ret);
}

SVCTX_ASSC_CFG_T* svctx_mgr_get_assc_cfg(const SVCTX_T* pt_svctx_assc)
{
    INT32 i;
#ifdef MW_PIP_POP_SUPPORT
    SVCTX_T*        pt_lead;
    LINK_POS_T      t_pos = NULL;
#endif

    for (i = 0; i < _i4_assc_map_idx; i++)
    {
        if ((pt_svctx_assc == _at_association_map[i].pt_assc))
        {
#ifdef MW_PIP_POP_SUPPORT
            pt_lead = _at_association_map[i].pt_lead;  
                      
            if (((pt_lead && pt_lead->pt_running_svc_req) &&
                 (pt_svctx_assc != svctx_mgr_get_excl_obj_by_lead(pt_lead, pt_lead->pt_running_svc_req->e_src_type, &t_pos))) ||
                (!pt_lead || !pt_lead->pt_running_svc_req))
#endif                
            {                
                return &(_at_association_map[i].t_assc_cfg);
            }                
        }
    }

    return NULL;
}

#ifdef MW_PIP_POP_SUPPORT
SVCTX_T* svctx_mgr_get_lead_obj(const ACTIVE_LINK_ENTRY_T* pt_link)
{
    if (pt_link && pt_link->pv_tag)
    {
        return (((ACTIVE_LINK_HEAD_T*) (pt_link->pv_tag))->pt_lead);
    }
    else
    {
        return NULL;
    }
}
#endif

SVCTX_T* svctx_mgr_get_link_lead(
    ACTIVE_LINK_TYPE_T  e_link_type,
    SRC_TYPE_T          e_src_type)
{
    ACTIVE_LINK_HEAD_T*     pt_head;

    pt_head = _svctx_mgr_get_link_head(e_link_type, e_src_type);

    if (pt_head)
    {
        return (pt_head->pt_lead);
    }

    return NULL;
}

UINT32 svctx_mgr_get_link_num(
    ACTIVE_LINK_TYPE_T  e_link_type,
    SRC_TYPE_T          e_src_type)
{
    ACTIVE_LINK_HEAD_T*     pt_head;

    pt_head = _svctx_mgr_get_link_head(e_link_type, e_src_type);

    if (pt_head)
    {
        return (pt_head->ui4_cnt);
    }

    return 0;
}

ACTIVE_LINK_ENTRY_T* svctx_mgr_get_link_obj(
    ACTIVE_LINK_TYPE_T  e_link_type,
    SRC_TYPE_T          e_src_type,
    UINT32              ui4_id)
{
    UINT32                  i;
    ACTIVE_LINK_HEAD_T*     pt_head;
    ACTIVE_LINK_ENTRY_T*    pt_entry = NULL;

    pt_head = _svctx_mgr_get_link_head(e_link_type, e_src_type);

    if (pt_head)
    {
        pt_entry = DLIST_HEAD(&(pt_head->t_list));

        for (i = 0; pt_entry && (i < ui4_id); i++)
        {
            pt_entry = DLIST_NEXT(pt_entry, t_next);
        }
    }

    return (pt_entry);
}

ACTIVE_LINK_ENTRY_T* svctx_mgr_get_link_obj_all(
    ACTIVE_LINK_TYPE_T  e_link_type,
    UINT32              ui4_id)
{
    UINT32                  i, j;
    ACTIVE_LINK_HEAD_T*     pt_head;
    ACTIVE_LINK_ENTRY_T*    pt_entry = NULL;

    pt_head = _svctx_mgr_get_link_head_array(e_link_type);

    if (pt_head)
    {
        for (i = 0; i < MAX_ACTIVE_ASSOCIATION_LINK; i++)
        {
            if (0 == pt_head[i].ui4_cnt)
            {
                continue;
            }

            if (ui4_id >= pt_head[i].ui4_cnt)
            {
                ui4_id -= pt_head[i].ui4_cnt;
            }
            else
            {
                pt_entry = DLIST_HEAD(&(pt_head[i].t_list));

                for (j = 0; pt_entry && (j < ui4_id); j++)
                {
                    pt_entry = DLIST_NEXT(pt_entry, t_next);
                }

                break;
            }
        }
    }

    return (pt_entry);
}

SVCTX_T* svctx_mgr_get_assc_obj_by_lead(
    SVCTX_T*    pt_svctx,
    SRC_TYPE_T  e_src_type,
    LINK_POS_T* pt_pos)
{
    SVCTX_T*            pt_assc = NULL;
    SVC_SELECT_TYPE_T   e_svc_type;
    INT32               i4_idx;
    UINT32              ui4_src_flag;
    ASSOCIATION_T*      pt_entry;

    if (pt_pos)
    {
        e_svc_type = svctx_mgr_convert_to_svc_type(e_src_type);

        if (SVC_SELECT_NONE != e_svc_type)
        {
            ui4_src_flag = (SVC_SELECT_TV == e_svc_type) ? SVCTX_SVC_SELECT_FLAG_TV : SVCTX_SVC_SELECT_FLAG_AV;

            if (NULL == *pt_pos)
            {
                i4_idx = 0;
            }
            else
            {
                i4_idx = (INT32)(*pt_pos);
            }

            while (i4_idx < _i4_assc_map_idx)
            {
                pt_entry = &(_at_association_map[i4_idx]);
                i4_idx++;

                if ((pt_svctx == pt_entry->pt_lead) &&
                    (0 != (pt_entry->ui4_src_mask & ui4_src_flag)))
                {
                    pt_assc = pt_entry->pt_assc;
                    *pt_pos = (LINK_POS_T) (i4_idx);
                    break;
                }
            }
        }
    }

    return (pt_assc);
}

#ifdef MW_PIP_POP_SUPPORT
SVCTX_T* svctx_mgr_get_excl_obj_by_lead(
    SVCTX_T*    pt_svctx,
    SRC_TYPE_T  e_src_type,
    LINK_POS_T* pt_pos)
{
    SVCTX_T*            pt_excl = NULL;
    SVC_SELECT_TYPE_T   e_svc_type;
    INT32               i4_idx;
    UINT32              ui4_src_flag;
    EXCLUSION_T*        pt_entry;

    if (pt_pos)
    {
        e_svc_type = svctx_mgr_convert_to_svc_type(e_src_type);

        if (SVC_SELECT_NONE != e_svc_type)
        {
            ui4_src_flag = (SVC_SELECT_TV == e_svc_type) ? SVCTX_SVC_SELECT_FLAG_TV : SVCTX_SVC_SELECT_FLAG_AV;

            if (NULL == *pt_pos)
            {
                i4_idx = 0;
            }
            else
            {
                i4_idx = (INT32)(*pt_pos);
            }

            while (i4_idx < _i4_excl_map_idx)
            {
                pt_entry = &(_at_exclusion_map[i4_idx]);
                i4_idx++;

                if ((pt_svctx == pt_entry->pt_lead) &&
                    (0 != (pt_entry->ui4_src_mask & ui4_src_flag)))
                {
                    pt_excl = pt_entry->pt_excl;
                    *pt_pos = (LINK_POS_T) (i4_idx);
                    break;
                }
            }
        }
    }

    return (pt_excl);
}
#endif

INT32 svctx_mgr_init(VOID)
{
    INT32 i;

    for (i = 0; i < MAX_QUEUED_REQ-1; i++)
    {
        _at_queued_req[i].pt_next = &(_at_queued_req[i+1]);
    }

    _at_queued_req[MAX_QUEUED_REQ-1].pt_next = NULL;
    _pt_free_qr = _at_queued_req;

    return SVCTXR_OK;
}

UINT32 svctx_mgr_new_seq_num(VOID)
{
    return (++_ui4_seq_num);
}

VOID svctx_mgr_print_active_link(ACTIVE_LINK_TYPE_T e_link_type)
{
#ifdef CLI_LVL_ALL
    ACTIVE_LINK_HEAD_T*         pt_head_array;
    ACTIVE_LINK_HEAD_T*         pt_head;
    ACTIVE_LINK_ENTRY_T*        pt_entry;
    INT32                       i = 0;

    x_dbg_stmt("++++++++++++++++++++++++++++++++++++ active link list %s\r\n", e_link_type == ACTIVE_LINK_NOW ? "now" : (e_link_type == ACTIVE_LINK_NEXT ? "next" : "unknown"));
    pt_head_array = _svctx_mgr_get_link_head_array(e_link_type);

    if (pt_head_array)
    {
        for (i = 0; i < MAX_ACTIVE_ASSOCIATION_LINK; i++)
        {
            pt_head = &(pt_head_array[i]);
            x_dbg_stmt("------------------------------------ index %d\r\n", i);
            x_dbg_stmt("e_type                  - %s\r\n", s_g_mgr_svc_select_types[pt_head->e_type]);
            x_dbg_stmt("pt_lead->s_name         - %s\r\n", pt_head->pt_lead != NULL ? pt_head->pt_lead->s_name : "<none>");
            x_dbg_stmt("ui4_cnt                 - %u\r\n", pt_head->ui4_cnt);
            x_dbg_stmt("---------------- Entries:\r\n");

            if (SVC_SELECT_NONE != pt_head->e_type)
            {
                pt_entry = DLIST_HEAD(&(pt_head->t_list));

                while (pt_entry)
                {
                    x_dbg_stmt("  pt_worker               - %s\r\n", pt_entry->pt_worker ? pt_entry->pt_worker->s_name : "<none>");
                    x_dbg_stmt("  e_last_op               - %s\r\n", s_g_mgr_op_types[pt_entry->e_last_op]);
                    x_dbg_stmt("  ui4_num_qr              - %d\r\n", pt_entry->ui4_num_qr);
                    if (pt_entry->pt_qr_head != NULL)
                    {
                        QUEUED_REQ_T*           pt_qr = pt_entry->pt_qr_head;
                        INT32                   j = 1;
                        while (pt_qr != NULL)
                        {
                            x_dbg_stmt("-------- Queued request %d:\r\n", j++);
                            x_dbg_stmt("  pt_svctx->s_name        - %s\r\n", pt_qr->pt_svctx ? pt_qr->pt_svctx->s_name : "<none>");
                            x_dbg_stmt("  t_var_info.e_var_type   - %s\r\n", s_g_mgr_var_types[pt_qr->t_var_info.e_var_type]);
                            x_dbg_stmt("  t_var_info.ui1_msg_id   - %d\r\n", pt_qr->t_var_info.ui1_msg_id);
                            if (VAR_TYPE_PLAYER == pt_qr->t_var_info.e_var_type)
                            {
                                x_dbg_stmt("  e_player_type           - %s\r\n", s_g_player_types[pt_qr->t_var_info.u.t_player_info.e_player_type]);
                                x_dbg_stmt("  pt_player               - 0x%x (%s)\r\n", pt_qr->t_var_info.u.t_player_info.pt_player, pt_qr->t_var_info.u.t_player_info.pt_player ? pt_qr->t_var_info.u.t_player_info.pt_player->s_snk_grp_name : "<none>");
                                x_dbg_stmt("  pf_invoke_func          - 0x%x\r\n", pt_qr->t_var_info.u.t_player_info.pf_invoke_func);
                            }
                            else if (VAR_TYPE_STREAM == pt_qr->t_var_info.e_var_type)
                            {
                                x_dbg_stmt("  h_stream                - 0x%x\r\n", pt_qr->t_var_info.u.t_strm_info.h_stream);
                                x_dbg_stmt("  e_strm_type             - %s\r\n", s_g_stream[pt_qr->t_var_info.u.t_strm_info.e_strm_type]);
                                x_dbg_stmt("  i4_idx                  - %d\r\n", pt_qr->t_var_info.u.t_strm_info.i4_idx);
                                x_dbg_stmt("  b_aux_audio             - %s\r\n", (pt_qr->t_var_info.u.t_strm_info.b_aux_audio == TRUE ? "Y" : "N"));
                                x_dbg_stmt("  b_auto_play             - %s\r\n", (pt_qr->t_var_info.u.t_strm_info.b_auto_play == TRUE ? "Y" : "N"));
                            }
                            pt_qr  = pt_qr->pt_next;
                        }
                    }
                    x_dbg_stmt("================\r\n");
                    pt_entry = DLIST_NEXT(pt_entry, t_next);
                }
            }
        }
    }
    x_dbg_stmt("====================================\r\n");
#endif
}

BOOL svctx_mgr_ready_to_proceed(
    ACTIVE_LINK_TYPE_T  e_link_type,
    SRC_TYPE_T          e_src_type,
    SVCTX_T*            pt_svctx,
    const VAR_INFO_T*   pt_var_info)
{
    ACTIVE_LINK_HEAD_T*         pt_head;
    ACTIVE_LINK_ENTRY_T*        pt_entry;
    SVC_SELECT_TYPE_T           e_svc_type;
    pf_compare_priority_fct     pf_comp_fct = NULL;
    pf_filter_candidate_fct     pf_filter_fct = NULL;
    CMP_RES_T                   e_res;
    INT32                       i = 0;

    DBG_SVCTXMGR_MSG((
        "{SVCTX_MGR} SVCTX(%s): Checking priority...\r\n",
        pt_svctx->s_name));

    switch (pt_var_info->e_var_type)
    {
        case VAR_TYPE_SERVICE:
            pf_comp_fct = _svctx_mgr_compare_priority_with_service_type;
            break;

        case VAR_TYPE_PLAYER:
            pf_comp_fct = _svctx_mgr_compare_priority_with_player_type;
            pf_filter_fct = _svctx_mgr_filter_candidate;
            break;

        case VAR_TYPE_STREAM:
            if (TRUE == _svctx_mgr_stream_excluded(
                &(pt_var_info->u.t_strm_info),
                pt_var_info->e_op_type))
            {
                return TRUE;
            }

            pf_comp_fct = _svctx_mgr_compare_priority_with_stream_type;

            if (OP_TYPE_CLOSE == pt_var_info->e_op_type)
            {
                pf_filter_fct = _svctx_mgr_filter_candidate;
            }
            break;

        default:
            return TRUE;;
    }

    pt_head = _svctx_mgr_get_link_head(e_link_type, e_src_type);

    if (pt_head)
    {
        e_svc_type = svctx_mgr_convert_to_svc_type(e_src_type);

        pt_entry = (OP_TYPE_OPEN == pt_var_info->e_op_type) ?
            DLIST_HEAD(&(pt_head->t_list)) : DLIST_TAIL(&(pt_head->t_list));

        while (pt_entry)
        {
            /* special case handling.... EXTREMELY UGLY. Just to overcome HW (driver) limitation */
            if ((VAR_TYPE_STREAM == pt_var_info->e_var_type) &&
                (ST_AUDIO == pt_var_info->u.t_strm_info.e_strm_type) &&
                (pt_var_info->u.t_strm_info.b_aux_audio))
            {
                if (!IS_SVCTX_ASSOCIATE(pt_svctx) && (pt_entry->pt_worker == pt_svctx))
                {
                    CH_SVC_REQ_T* pt_svc_req = (CH_SVC_REQ_T*) pt_svctx->pt_running_svc_req;

                    if (pt_svc_req)
                    {
                        if (SM_COND_STARTED != pt_svc_req->t_strm_reqs[ST_AUDIO].e_strm_cond)
                        {
                            _svctx_mgr_append_qr(
                                pt_entry,
                                pt_svctx,
                                pt_var_info);

                            return FALSE;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                if (pf_filter_fct)
                {
                    if (TRUE == (*pf_filter_fct)(pt_entry->pt_worker, pt_svctx))
                    {
                        if (VAR_TYPE_PLAYER == pt_var_info->e_var_type)
                        {
                            pt_entry->e_last_op = pt_var_info->e_op_type;
                        }

                        return TRUE; /* no postpone, execute the command right away */
                    }
                }

                e_res = (*pf_comp_fct)(
                    e_svc_type,
                    pt_svctx,
                    pt_entry->pt_worker,
                    pt_var_info,
                    i++);

                if ((CMP_RES_POSTPONE == e_res)
                #if 0
                    ||
                    ((CMP_RES_FURTHER_EVALUATE == e_res) &&
                     /* direction check */
                     (VAR_TYPE_PLAYER == pt_var_info->e_var_type) &&
                     (OP_TYPE_NONE != pt_entry->e_last_op) &&
                     (pt_entry->e_last_op != pt_var_info->e_op_type))
                 #endif
                     )
                {
                    _svctx_mgr_append_qr(
                        pt_entry,
                        pt_svctx,
                        pt_var_info);

                    return FALSE;
                }
            }

            pt_entry = (OP_TYPE_OPEN == pt_var_info->e_op_type) ?
                DLIST_NEXT(pt_entry, t_next) : DLIST_PREV(pt_entry, t_next);
        }
    }

    return TRUE;
}

INT32 svctx_mgr_reconfig_link_with_obj(
    ACTIVE_LINK_TYPE_T  e_link_type,
    SRC_TYPE_T          e_src_type,
    SVCTX_T*            pt_svctx,
    BOOL                b_lead)
{
    INT32               i;
    INT32               i4_ret;
    ACTIVE_LINK_HEAD_T* pt_head_array;
    SVC_SELECT_TYPE_T   e_svc_type;
    SVC_SELECT_TYPE_T   e_svc_type_ins;

    ACTIVE_LINK_HEAD_T*     pt_head_src = NULL;
    ACTIVE_LINK_HEAD_T*     pt_head_dst = NULL;
    ACTIVE_LINK_ENTRY_T*    pt_entry    = NULL;
    ACTIVE_LINK_ENTRY_T*    pt_tmp      = NULL;

    i4_ret = SVCTXR_INV_ARG;

    e_svc_type_ins = svctx_mgr_convert_to_svc_type(e_src_type);

    if (SVC_SELECT_NONE != e_svc_type_ins)
    {
        pt_head_array = _svctx_mgr_get_link_head_array(e_link_type);

        if (pt_head_array)
        {
            i4_ret = SVCTXR_NO_RECORD;

            for (i = 0; i < MAX_ACTIVE_ASSOCIATION_LINK; i++)
            {
                if (pt_head_src != NULL && pt_head_dst != NULL)
                {
                    break;
                }
                
                e_svc_type = pt_head_array[i].e_type;

                if (e_svc_type_ins == e_svc_type)
                {
                    /* find the old entry if any */
                    pt_tmp = DLIST_HEAD(&(pt_head_array[i].t_list));

                    while (pt_tmp)
                    {
                        if (pt_svctx == pt_tmp->pt_worker)
                        {
                            /* no further action needed */
                            return SVCTXR_OK;
                        }

                        pt_tmp = DLIST_NEXT(pt_tmp, t_next);
                    }

                    pt_head_dst = &(pt_head_array[i]);
                }
                else if ((SVC_SELECT_NONE == e_svc_type) && !pt_head_dst)
                {
                    pt_head_dst = &(pt_head_array[i]);
                }
                else if (SVC_SELECT_NONE != e_svc_type) /* other svc select type */
                {
                    /* find the old entry if any */
                    pt_entry = DLIST_HEAD(&(pt_head_array[i].t_list));

                    while (pt_entry)
                    {
                        if (pt_svctx == pt_entry->pt_worker)
                        {
                            pt_head_src = &(pt_head_array[i]);
                            break;
                        }

                        pt_entry = DLIST_NEXT(pt_entry, t_next);
                    }
                }
            }
        }
    }

    if (pt_head_dst)
    {
        if (pt_entry && pt_head_src)
        {
            DLIST_REMOVE(pt_entry, &(pt_head_src->t_list), t_next);

            if (pt_head_src->pt_lead == pt_entry->pt_worker)
            {
                pt_head_src->pt_lead = NULL;
            }

            if (0 == --pt_head_src->ui4_cnt)
            {
                _svctx_mgr_release_active_link(pt_head_src);
            }
        }

        i4_ret = _svctx_mgr_insert_obj(pt_head_dst, pt_svctx, pt_entry);

        if ((SVCTXR_OK == i4_ret))
        {
#ifdef MW_PIP_POP_SUPPORT
            LINK_POS_T  t_pos = 0;

            if (b_lead &&
                (!pt_head_dst->pt_lead ||
                 (svctx_mgr_get_excl_obj_by_lead(pt_svctx, e_src_type, &t_pos) == NULL)))
#else
            if (b_lead)
#endif

            {
                pt_head_dst->pt_lead = pt_svctx;
            }

            pt_head_dst->e_type = e_svc_type_ins;
        }
    }

    return (i4_ret);
}

INT32 svctx_mgr_remove_link_with_obj(
    ACTIVE_LINK_TYPE_T  e_link_type,
    SVCTX_T*            pt_svctx)
{
    INT32                   i;
    INT32                   i4_ret;
    ACTIVE_LINK_HEAD_T*     pt_head_array;
    ACTIVE_LINK_HEAD_T*     pt_head;
    ACTIVE_LINK_ENTRY_T*    pt_entry;

    i4_ret = SVCTXR_INV_ARG;

    pt_head_array =_svctx_mgr_get_link_head_array(e_link_type);

    if (pt_head_array)
    {
        i4_ret = SVCTXR_OK;

        for (i = 0; i < MAX_ACTIVE_ASSOCIATION_LINK; i++)
        {
            pt_head = &(pt_head_array[i]);

            if (SVC_SELECT_NONE != pt_head->e_type)
            {
                pt_entry = DLIST_HEAD(&(pt_head->t_list));

                while (pt_entry)
                {
                    if (pt_svctx == pt_entry->pt_worker)
                    {
                        if (pt_svctx == pt_head->pt_lead)
                        {
                            pt_head->pt_lead = NULL;
                        }

                        DLIST_REMOVE(pt_entry, &(pt_head->t_list), t_next);
                        _svctx_mgr_free_link_entry(pt_entry);

                        if (0 == --pt_head->ui4_cnt)
                        {
                            _svctx_mgr_release_active_link(pt_head);
                        }

                        return SVCTXR_OK;
                    }

                    pt_entry = DLIST_NEXT(pt_entry, t_next);
                }
            }
        }
    }

    return (i4_ret);
}

INT32 svctx_mgr_remove_queued_req(
    ACTIVE_LINK_TYPE_T  e_link_type,
    SRC_TYPE_T          e_src_type,
    SVCTX_T*            pt_svctx)
{
    INT32                   i4_ret = SVCTXR_OK;
    ACTIVE_LINK_HEAD_T*     pt_head;
    ACTIVE_LINK_ENTRY_T*    pt_entry;
    QUEUED_REQ_T*           pt_qr;
    QUEUED_REQ_T*           pt_tmp;
    QUEUED_REQ_T*           pt_prev;
    STRM_REQ_T*             pt_strm_req;
    VAR_INFO_T*             pt_var_info;

    pt_head = _svctx_mgr_get_link_head(e_link_type, e_src_type);

    if (pt_head)
    {
        pt_entry = DLIST_HEAD(&(pt_head->t_list));

        while (pt_entry)
        {
        #if 0
            if (pt_entry->pt_worker != pt_svctx)
        #endif
            {
                if (pt_entry->pt_qr_head) /* REQs queued */
                {
                    pt_qr   = pt_entry->pt_qr_head;
                    pt_prev = NULL;

                    while (pt_qr)
                    {
                        pt_tmp = pt_qr;
                        pt_qr  = pt_qr->pt_next;

                        if (pt_tmp->pt_svctx == pt_svctx)
                        {
                            /* remove QR from the link list */
                            if (pt_prev)
                            {
                                pt_prev->pt_next = pt_qr;
                            }
                            else
                            {
                                pt_entry->pt_qr_head = pt_qr;
                            }

                            pt_var_info = &(pt_tmp->t_var_info);

                            if (VAR_TYPE_PLAYER == pt_var_info->e_var_type)
                            {
#ifdef CLI_LVL_ALL
                                DBG_SVCTXMGR_MSG((
                                    "{SVCTX_MGR} Remove PLAYER(%s:%d) %s message: %d\r\n",
                                    pt_svctx->s_name,
                                    pt_var_info->u.t_player_info.e_player_type,
                                    pt_var_info->u.t_player_info.pt_player ? s_g_player_action[pt_var_info->u.t_player_info.pt_player->e_phase] : "<none>",
                                    pt_var_info->ui1_msg_id));
#endif
                                if (pt_var_info->u.t_player_info.pt_player)
                                {
                                    (pt_var_info->u.t_player_info.pt_player->ui1_act_queued)--;
                                }
                            }
                            else if (VAR_TYPE_STREAM == pt_var_info->e_var_type)
                            {
                                SVC_SELECT_TYPE_T e_svc_type;

                                e_svc_type = svctx_mgr_convert_to_svc_type(e_src_type);

                                if (pt_svctx->pt_running_svc_req)
                                {
                                    if (SVC_SELECT_TV == e_svc_type)
                                    {
                                        pt_strm_req = &(((CH_SVC_REQ_T*) pt_svctx->pt_running_svc_req)->t_strm_reqs[pt_var_info->u.t_strm_info.i4_idx]);
                                    }
                                    else if (SVC_SELECT_AV == e_svc_type)
                                    {
                                        pt_strm_req = &(((INP_SVC_REQ_T*) pt_svctx->pt_running_svc_req)->t_strm_reqs[pt_var_info->u.t_strm_info.i4_idx]);
                                    }
                                    else if (SVC_SELECT_MM == e_svc_type)
                                    {
                                        pt_strm_req = &(((MM_SVC_REQ_T*) pt_svctx->pt_running_svc_req)->t_strm_reqs[pt_var_info->u.t_strm_info.i4_idx]);
                                    }
                                    else
                                    {
                                        pt_strm_req = NULL;
                                    }

                                    if (pt_strm_req)
                                    {
                                        if (OP_TYPE_OPEN == pt_var_info->e_op_type)
                                        {
#ifdef CLI_LVL_ALL
                                            DBG_SVCTXMGR_MSG((
                                                "{SVCTX_MGR} Remove OPEN_STREAM(%s:%s) message: %d\r\n",
                                                pt_svctx->s_name,
                                                s_g_strm_types[pt_var_info->u.t_strm_info.e_strm_type],
                                                pt_var_info->ui1_msg_id));
#endif
                                            pt_strm_req->b_open_queued = FALSE;
                                        }
                                        else if (OP_TYPE_CLOSE == pt_var_info->e_op_type)
                                        {
#ifdef CLI_LVL_ALL
                                            DBG_SVCTXMGR_MSG((
                                                "{SVCTX_MGR} Remove CLOSE_STREAM(%s:%s) message: %d\r\n",
                                                pt_svctx->s_name,
                                                s_g_strm_types[pt_var_info->u.t_strm_info.e_strm_type],
                                                pt_var_info->ui1_msg_id));
#endif
                                            pt_strm_req->b_close_queued = FALSE;
                                        }
                                    }
                                }
                            }
                            else if (VAR_TYPE_SERVICE == pt_var_info->e_var_type)
                            {
                                if (OP_TYPE_OPEN == pt_var_info->e_op_type)
                                {
#ifdef CLI_LVL_ALL
                                    DBG_SVCTXMGR_MSG((
                                        "{SVCTX_MGR} Remove OPEN_SERVICE(%s) message: %d\r\n",
                                        pt_svctx->s_name,
                                        pt_var_info->ui1_msg_id));
#endif
                                }
                                else if (OP_TYPE_CLOSE == pt_var_info->e_op_type)
                                {
#ifdef CLI_LVL_ALL
                                    DBG_SVCTXMGR_MSG((
                                        "{SVCTX_MGR} Remove CLOSE_SERVICE(%s) message: %d\r\n",
                                        pt_svctx->s_name,
                                        pt_var_info->ui1_msg_id));
#endif
                                }
                            }
                            else
                            {
                                DBG_ABORT(DBG_MOD_SVCTX);
                            }

                            _svctx_mgr_free_qr(pt_tmp);
                            pt_entry->ui4_num_qr--;
                        }
                        else
                        {
                            pt_prev = pt_tmp;
                        }
                    }
                }
            }
            pt_entry = DLIST_NEXT(pt_entry, t_next);
        }
    }

    return (i4_ret);
}

INT32 svctx_mgr_remove_queued_stream_op(
    ACTIVE_LINK_TYPE_T  e_link_type,
    SRC_TYPE_T          e_src_type,
    SVCTX_T*            pt_svctx,
    UINT32              ui4_op_mask)
{
    INT32                   i4_ret = SVCTXR_OK;
    ACTIVE_LINK_HEAD_T*     pt_head;
    ACTIVE_LINK_ENTRY_T*    pt_entry;
    QUEUED_REQ_T*           pt_qr;
    QUEUED_REQ_T*           pt_tmp;
    QUEUED_REQ_T*           pt_prev;
    STRM_REQ_T*             pt_strm_req;
    VAR_INFO_T*             pt_var_info;
    BOOL                    b_remove;

    pt_head = _svctx_mgr_get_link_head(e_link_type, e_src_type);

    if (pt_head)
    {
        pt_entry = DLIST_HEAD(&(pt_head->t_list));

        while (pt_entry)
        {
            if (pt_entry->pt_qr_head) /* REQs queued */
            {
                pt_qr   = pt_entry->pt_qr_head;
                pt_prev = NULL;

                while (pt_qr)
                {
                    pt_tmp = pt_qr;
                    pt_qr  = pt_qr->pt_next;

                    if (pt_tmp->pt_svctx == pt_svctx)
                    {
                        b_remove = FALSE;

                        pt_var_info = &(pt_tmp->t_var_info);

                        if (VAR_TYPE_STREAM == pt_var_info->e_var_type)
                        {
                            SVC_SELECT_TYPE_T e_svc_type;

                            e_svc_type = svctx_mgr_convert_to_svc_type(e_src_type);

                            if (pt_svctx->pt_running_svc_req)
                            {
                                if (SVC_SELECT_TV == e_svc_type)
                                {
                                    pt_strm_req = &(((CH_SVC_REQ_T*) pt_svctx->pt_running_svc_req)->t_strm_reqs[pt_var_info->u.t_strm_info.i4_idx]);
                                }
                                else if (SVC_SELECT_AV == e_svc_type)
                                {
                                    pt_strm_req = &(((INP_SVC_REQ_T*) pt_svctx->pt_running_svc_req)->t_strm_reqs[pt_var_info->u.t_strm_info.i4_idx]);
                                }
                                else if (SVC_SELECT_MM == e_svc_type)
                                {
                                    pt_strm_req = &(((MM_SVC_REQ_T*) pt_svctx->pt_running_svc_req)->t_strm_reqs[pt_var_info->u.t_strm_info.i4_idx]);
                                }
                                else
                                {
                                    pt_strm_req = NULL;
                                }

                                if (pt_strm_req)
                                {
                                    if ((OP_TYPE_OPEN == pt_var_info->e_op_type) &&
                                        (ui4_op_mask & MASK_STRM_OP_OPEN))
                                    {
                                        DBG_SVCTXMGR_MSG((
                                            "{SVCTX_MGR} Remove OPEN_STREAM(%s:%s) message: %d\r\n",
                                            pt_svctx->s_name,
                                            s_g_strm_types[pt_var_info->u.t_strm_info.e_strm_type],
                                            pt_var_info->ui1_msg_id));

                                        pt_strm_req->b_open_queued = FALSE;
                                        b_remove = TRUE;
                                    }
                                    else if (
                                        (OP_TYPE_CLOSE == pt_var_info->e_op_type) &&
                                        (ui4_op_mask & MASK_STRM_OP_CLOSE))
                                    {
                                        DBG_SVCTXMGR_MSG((
                                            "{SVCTX_MGR} Remove CLOSE_STREAM(%s:%s) message: %d\r\n",
                                            pt_svctx->s_name,
                                            s_g_strm_types[pt_var_info->u.t_strm_info.e_strm_type],
                                            pt_var_info->ui1_msg_id));

                                        pt_strm_req->b_close_queued = FALSE;
                                        b_remove = TRUE;
                                    }
                                }
                            }
                        }

                        if (b_remove)
                        {
                            /* remove QR from the link list */
                            if (pt_prev)
                            {
                                pt_prev->pt_next = pt_qr;
                            }
                            else
                            {
                                pt_entry->pt_qr_head = pt_qr;
                            }

                            _svctx_mgr_free_qr(pt_tmp);
                            pt_entry->ui4_num_qr--;
                        }
                        else
                        {
                            pt_prev = pt_tmp;
                        }
                    }
                    else
                    {
                        pt_prev = pt_tmp;
                    }
                }
            }

            pt_entry = DLIST_NEXT(pt_entry, t_next);
        }
    }

    return (i4_ret);
}

INT32 svctx_mgr_remove_queued_strm_op_by_strm_type(
    ACTIVE_LINK_TYPE_T  e_link_type,
    SRC_TYPE_T          e_src_type,
    SVCTX_T*            pt_svctx,
    UINT32              ui4_op_mask,
    STREAM_TYPE_T       e_strm_type)
{
    INT32                   i4_ret = SVCTXR_OK;
    ACTIVE_LINK_HEAD_T*     pt_head;
    ACTIVE_LINK_ENTRY_T*    pt_entry;
    QUEUED_REQ_T*           pt_qr;
    QUEUED_REQ_T*           pt_tmp;
    QUEUED_REQ_T*           pt_prev;
    STRM_REQ_T*             pt_strm_req;
    VAR_INFO_T*             pt_var_info;
    BOOL                    b_remove;

    pt_head = _svctx_mgr_get_link_head(e_link_type, e_src_type);

    if (pt_head)
    {
        pt_entry = DLIST_HEAD(&(pt_head->t_list));

        while (pt_entry)
        {
            if (pt_entry->pt_qr_head) /* REQs queued */
            {
                pt_qr   = pt_entry->pt_qr_head;
                pt_prev = NULL;

                while (pt_qr)
                {
                    pt_tmp = pt_qr;
                    pt_qr  = pt_qr->pt_next;

                    if (pt_tmp->pt_svctx == pt_svctx)
                    {
                        b_remove = FALSE;

                        pt_var_info = &(pt_tmp->t_var_info);

                        if (VAR_TYPE_STREAM == pt_var_info->e_var_type)
                        {
                            SVC_SELECT_TYPE_T e_svc_type;

                            e_svc_type = svctx_mgr_convert_to_svc_type(e_src_type);

                            if (pt_svctx->pt_running_svc_req)
                            {
                                if (SVC_SELECT_TV == e_svc_type)
                                {
                                    pt_strm_req = &(((CH_SVC_REQ_T*) pt_svctx->pt_running_svc_req)->t_strm_reqs[pt_var_info->u.t_strm_info.i4_idx]);
                                }
                                else if (SVC_SELECT_AV == e_svc_type)
                                {
                                    pt_strm_req = &(((INP_SVC_REQ_T*) pt_svctx->pt_running_svc_req)->t_strm_reqs[pt_var_info->u.t_strm_info.i4_idx]);
                                }
                                else if (SVC_SELECT_MM == e_svc_type)
                                {
                                    pt_strm_req = &(((MM_SVC_REQ_T*) pt_svctx->pt_running_svc_req)->t_strm_reqs[pt_var_info->u.t_strm_info.i4_idx]);
                                }
                                else
                                {
                                    pt_strm_req = NULL;
                                }

                                if (pt_strm_req)
                                {
                                    if ((OP_TYPE_OPEN == pt_var_info->e_op_type) &&
                                        (ui4_op_mask & MASK_STRM_OP_OPEN) &&
                                        (e_strm_type == pt_var_info->u.t_strm_info.e_strm_type))
                                    {
                                        DBG_SVCTXMGR_MSG((
                                            "{SVCTX_MGR} Remove OPEN_STREAM(%s:%s) message: %d\r\n",
                                            pt_svctx->s_name,
                                            s_g_strm_types[pt_var_info->u.t_strm_info.e_strm_type],
                                            pt_var_info->ui1_msg_id));

                                        pt_strm_req->b_open_queued = FALSE;
                                        b_remove = TRUE;
                                    }
                                    else if (
                                        (OP_TYPE_CLOSE == pt_var_info->e_op_type) &&
                                        (ui4_op_mask & MASK_STRM_OP_CLOSE) &&
                                        (e_strm_type == pt_var_info->u.t_strm_info.e_strm_type))
                                    {
                                        DBG_SVCTXMGR_MSG((
                                            "{SVCTX_MGR} Remove CLOSE_STREAM(%s:%s) message: %d\r\n",
                                            pt_svctx->s_name,
                                            s_g_strm_types[pt_var_info->u.t_strm_info.e_strm_type],
                                            pt_var_info->ui1_msg_id));

                                        pt_strm_req->b_close_queued = FALSE;
                                        b_remove = TRUE;
                                    }
                                }
                            }
                        }

                        if (b_remove)
                        {
                            /* remove QR from the link list */
                            if (pt_prev)
                            {
                                pt_prev->pt_next = pt_qr;
                            }
                            else
                            {
                                pt_entry->pt_qr_head = pt_qr;
                            }

                            _svctx_mgr_free_qr(pt_tmp);
                            pt_entry->ui4_num_qr--;
                        }
                        else
                        {
                            pt_prev = pt_tmp;
                        }
                    }
                    else
                    {
                        pt_prev = pt_tmp;
                    }
                }
            }

            pt_entry = DLIST_NEXT(pt_entry, t_next);
        }
    }

    return (i4_ret);
}

INT32 svctx_mgr_remove_solved_qr(
    ACTIVE_LINK_TYPE_T  e_link_type,
    SRC_TYPE_T          e_src_type,
    SVCTX_T*            pt_svctx,
    VAR_INFO_T*         pt_var_info)
{
    /* remove the QR that already solved and reset the flag */
    INT32                   i4_ret = SVCTXR_OK;
    ACTIVE_LINK_HEAD_T*     pt_head;
    ACTIVE_LINK_ENTRY_T*    pt_entry;
    QUEUED_REQ_T*           pt_qr;
    QUEUED_REQ_T*           pt_tmp;
    QUEUED_REQ_T*           pt_prev;
    STRM_REQ_T*             pt_strm_req;
    BOOL                    b_remove;
    SVC_SELECT_TYPE_T       e_svc_type;

    DBG_SVCTXMGR_MSG(("\r\n{SVCTX_MGR} Starting svctx_mgr_remove_solved_qr...\r\n\r\n"));

    pt_head = _svctx_mgr_get_link_head(e_link_type, e_src_type);

    if (pt_head)
    {
        e_svc_type = svctx_mgr_convert_to_svc_type(e_src_type);

        pt_entry = DLIST_HEAD(&(pt_head->t_list));

        while (pt_entry)
        {
        #if 0
            if (pt_entry->pt_worker != pt_svctx)
        #endif
            {
                if (pt_entry->pt_qr_head) /* REQs queued */
                {
                    pt_qr   = pt_entry->pt_qr_head;
                    pt_prev = NULL;

                    while (pt_qr)
                    {
                        b_remove = FALSE;
                        pt_tmp   = pt_qr;
                        pt_qr    = pt_qr->pt_next;

                        if (pt_tmp->pt_svctx == pt_svctx)
                        {
                            if ((VAR_TYPE_STREAM == pt_var_info->e_var_type) &&
                                (OP_TYPE_CLOSE == pt_var_info->e_op_type) &&
                                (VAR_TYPE_STREAM == pt_tmp->t_var_info.e_var_type) &&
                                (OP_TYPE_CLOSE == pt_tmp->t_var_info.e_op_type) &&
                                (pt_tmp->t_var_info.u.t_strm_info.e_strm_type == pt_var_info->u.t_strm_info.e_strm_type))
                            {
                                b_remove = TRUE;

                                if (pt_svctx->pt_running_svc_req)
                                {
                                    if (SVC_SELECT_TV == e_svc_type)
                                    {
                                        pt_strm_req = &(((CH_SVC_REQ_T*) pt_svctx->pt_running_svc_req)->t_strm_reqs[pt_tmp->t_var_info.u.t_strm_info.i4_idx]);
                                    }
                                    else if (SVC_SELECT_AV == e_svc_type)
                                    {
                                        pt_strm_req = &(((INP_SVC_REQ_T*) pt_svctx->pt_running_svc_req)->t_strm_reqs[pt_tmp->t_var_info.u.t_strm_info.i4_idx]);
                                    }
                                    else if (SVC_SELECT_MM == e_svc_type)
                                    {
                                        pt_strm_req = &(((MM_SVC_REQ_T*) pt_svctx->pt_running_svc_req)->t_strm_reqs[pt_tmp->t_var_info.u.t_strm_info.i4_idx]);
                                    }
                                    else
                                    {
                                        pt_strm_req = NULL;
                                    }

                                    if (pt_strm_req)
                                    {
                                        pt_strm_req->b_close_queued = FALSE;
                                    }

#ifdef CLI_LVL_ALL
                                    DBG_SVCTXMGR_MSG((
                                        "{SVCTX_MGR} Remove solved CLOSE_STREAM(%s:%s) message: %d\r\n",
                                        pt_svctx->s_name,
                                        s_g_strm_types[pt_tmp->t_var_info.u.t_strm_info.e_strm_type],
                                        pt_tmp->t_var_info.ui1_msg_id));
#endif
                                }
                            }

                            if (b_remove)
                            {
                                /* remove QR from the link list */
                                if (pt_prev)
                                {
                                    pt_prev->pt_next = pt_qr;
                                }
                                else
                                {
                                    pt_entry->pt_qr_head = pt_qr;
                                }

                                _svctx_mgr_free_qr(pt_tmp);
                                pt_entry->ui4_num_qr--;
                            }
                        }
                        else
                        {
                            pt_prev = pt_tmp;
                        }
                    }
                }
            }
            pt_entry = DLIST_NEXT(pt_entry, t_next);
        }
    }

    DBG_SVCTXMGR_MSG(("\r\n{SVCTX_MGR} End of svctx_mgr_remove_solved_qr...\r\n\r\n"));

    return (i4_ret);
}
#endif
