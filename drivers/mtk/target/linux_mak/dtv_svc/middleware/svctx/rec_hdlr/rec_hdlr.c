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
 * $RCSfile:  $
 * $Revision: #2 $
 * $Date: 2012/07/05 $
 * $Author: pan.huang $
 *
 * Description:
 *         This file contains Record Handler implementations.
 *---------------------------------------------------------------------------*/

#ifdef SYS_RECORD_SUPPORT

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "svctx/rec_hdlr/rec_hdlr_cli.h"
#include "svctx/rec_hdlr/rec_hdlr.h"
#include "svctx/rec_hdlr/rec_svc_req.h"
#include "svctx/util/player.h"
#include "svctx/svctx.h"
#include "strm_mngr/rcrd_strm_hdlr/u_sm_rcrd_hdlr.h"
#include "strm_mngr/sess_rcrd_hdlr/u_sm_sess_rcrd.h"
#include "strm_mngr/u_sm.h"
#include "file_mngr/x_fm_mfw.h"
#include "util/x_util_scdb.h"
#include "svl_bldr/u_sb.h"
#include "rrctx/x_rrctx.h"
#include "svctx/svctx_rikstv.h"
#include "evctx/evctx_atsc_eng/u_evctx_atsc_eng.h"
#ifdef MW_EDB_SUPPORT
#include "edb/u_edb.h"
#endif
#include "tbl_mngr/psi_eng/x_psi_eng.h"
#include "tbl_mngr/x_tm.h"
#include "svl/x_svl.h"
#include "tsl/x_tsl.h"
#include "dt/x_dt.h"
#include "dbg/x_dbg.h"
#include "os/inc/x_os.h" 
#include "dbg/u_dbg.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#undef DBG_LEVEL_MODULE
#define DBG_LEVEL_MODULE     rec_hdlr_get_dbg_level()


#define REC_SVC_PAT_PID               ((MPEG_2_PID_T) 0)
#define KEEP_PIPE                     TRUE
#define NOT_KEEP_PIPE                 FALSE
#define ALSO_PIPE                     TRUE
#define NOT_ALSO_PIPE                 FALSE
#define NFY_ERR                       TRUE
#define NOT_NFY_ERR                   FALSE

typedef enum _REC_SVC_EVCTX_KEY_INFO_TYPE_T
{
    REC_SVC_EVCTX_KEY_INFO_TYPE_NULL,
    REC_SVC_EVCTX_KEY_INFO_TYPE_LANG
    
} REC_SVC_EVCTX_KEY_INFO_TYPE_T;

#define REC_SVC_SESS_COND_CAN_OPEN_STRM(_e_cond_sess)   \
    (!((_e_cond_sess) == SM_SESS_COND_OPENING ||        \
       (_e_cond_sess) == SM_SESS_COND_CLOSING ||        \
       (_e_cond_sess) == SM_SESS_COND_CLOSED  ||        \
       (_e_cond_sess) == SM_SESS_COND_IGNORE ))
        
#define REC_SVC_STRM_COND_CAN_SET_CTRL_OUT(_e_cond_strm)\
    ((_e_cond_strm) == SM_COND_OPENED    ||             \
     (_e_cond_strm) == SM_COND_STARTING  ||             \
     (_e_cond_strm) == SM_COND_STARTED   ||             \
     (_e_cond_strm) == SM_COND_STOPPING )
        
#define REC_SVC_SUPPORTED_BRDCST_TYPE(_e_brdcst_type)   \
    ((_e_brdcst_type) == BRDCST_TYPE_UNKNOWN  ||        \
     (_e_brdcst_type) == BRDCST_TYPE_ATSC     ||        \
     (_e_brdcst_type) == BRDCST_TYPE_SCTE     ||        \
     (_e_brdcst_type) == BRDCST_TYPE_DVB      ||        \
     (_e_brdcst_type) == BRDCST_TYPE_MHP      ||        \
     (_e_brdcst_type) == BRDCST_TYPE_ISDB     ||        \
     (_e_brdcst_type) == BRDCST_TYPE_DTMB)
    
#define REC_SVC_EVCTX_INFO_AVAIL(_e_evctx_cond)         \
    ((_e_evctx_cond) == EVCTX_COND_AVAIL  ||            \
     (_e_evctx_cond) == EVCTX_COND_UPDATED)

     
#define REC_SVC_CA_CP_SM_EVENTS(_e_sm_evn)              \
    ((_e_sm_evn) == SM_EVN_CA_COND_CLOSED         ||    \
     (_e_sm_evn) == SM_EVN_CA_COND_UNKNOWN        ||    \
     (_e_sm_evn) == SM_EVN_CA_COND_FREE           ||    \
     (_e_sm_evn) == SM_EVN_CA_COND_AUTHORIZED     ||    \
     (_e_sm_evn) == SM_EVN_CA_COND_AUTHORIZED_CA_SYS_ID || \
     (_e_sm_evn) == SM_EVN_CA_COND_NOT_AUTHORIZED ||    \
     (_e_sm_evn) == SM_EVN_CCI_INFORMATION)

#define REC_SVC_VOLATILE_RECORD(_pt_svc_req)            \
    ((_pt_svc_req)->pt_rec_cfg->e_stop_opt == SVCTX_REC_STOP_OPT_NO_CARE)
     
#define REC_SVC_ES_AUTHORIZED(_pt_svc_req, _e_auth_st_es)                   \
     ( REC_SVC_VOLATILE_RECORD (_pt_svc_req)    ||                          \
       ( ((_pt_svc_req)->e_auth_st == SVCTX_REC_AUTH_STATUS_AUTHORIZED ||   \
          (_pt_svc_req)->e_auth_st == SVCTX_REC_AUTH_STATUS_UNKNOWN) &&     \
         ((_e_auth_st_es) == SVCTX_REC_AUTH_STATUS_AUTHORIZED ||            \
          (_e_auth_st_es) == SVCTX_REC_AUTH_STATUS_UNKNOWN ) ) )

#define REC_SVC_COPY_TO_SM_ES_INFO(_pt_sm_es_info, _pt_rec_es_info)         \
    {                                                                       \
        (_pt_sm_es_info)->t_pid         = (_pt_rec_es_info)->t_pid;         \
        (_pt_sm_es_info)->ui4_type_mask = (_pt_rec_es_info)->ui4_type_mask; \
    }

#define REC_SVC_PROFILE_SUPPORT_EVENT_INFO(_pt_svc_req)                     \
        ((_pt_svc_req)->pt_rec_cfg->e_profile == SVCTX_REC_PROFILE_ADVANCED)

#define CM_CONNECT          \
        x_cm_connect
#define CM_DISCONNECT       \
        x_cm_disconnect
#define CM_OPEN_PIPE        \
        x_cm_open_pipe
#define CM_CLOSE_PIPE       \
        x_cm_close_pipe
    

#ifdef CLI_LVL_ALL




#define DBG_INFO_EVN_AT_ST(_pt_svctx, _e_event)                                         \
    DBG_INFO (( s_g_dbg_msg_evn_at_st,                                                  \
                (_e_event),                                                             \
                ((REC_SVC_REQ_T *) (_pt_svctx)->pt_running_svc_req)->e_state,           \
                __LINE__))


#define DBG_ERROR_EVN_AT_INV_ST(_pt_svctx, _e_event)                                    \
    DBG_ERROR (( s_g_dbg_msg_evn_at_inv_st,                                             \
                 (_e_event),                                                            \
                 ((REC_SVC_REQ_T *) (_pt_svctx)->pt_running_svc_req)->e_state,          \
                 __LINE__))

#define DBG_INFO_DUMP_Q_EVENTS(_pt_svc_req)                                             \
    _rec_svc_state_util_dbg_dump_q_events (_pt_svc_req)

#ifdef MW_TV_AV_BYPASS_SUPPORT
#define DBG_INFO_DUMP_SVCTX_MGR_REQ_Q(_pt_svctx)                                        \
    _rec_svc_state_util_dbg_dump_svctx_mgr_qr ((_pt_svctx), __LINE__)
#endif

#else




#define DBG_INFO_EVN_AT_ST(_pt_svctx, _e_event)
#define DBG_ERROR_EVN_AT_INV_ST(_pt_svctx, _e_event)
#define DBG_INFO_DUMP_Q_EVENTS(_pt_svc_req)
#define DBG_INFO_DUMP_SVCTX_MGR_REQ_Q(_pt_svctx)




#endif

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
typedef struct _REC_SVC_EVCTX_KEY_INFO_T 
{
#ifdef MW_EDB_SUPPORT 
    EDB_KEY_TYPE_T                  e_key_type;
#else                    
    EVCTX_KEY_TYPE_T                e_key_type;
#endif                    
    
    REC_SVC_EVCTX_KEY_INFO_TYPE_T   e_info_type;
    
} REC_SVC_EVCTX_KEY_INFO_T ;

/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static CHAR* s_g_dbg_msg_evn_at_st     = "{SVCTX} INFO: [REC_HDLR] Event (%d) received at state (%d). (L%d)\n";
static CHAR* s_g_dbg_msg_evn_at_inv_st = "{SVCTX}  ERR: [REC_HDLR] Event (%d) received at invalid state (%d). (L%d)\n";
#endif

static UINT32 ui4_g_rand_seed = 0;

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
static INT32 _rec_svc_util_get_src_brdcst_svl (
    REC_SVC_REQ_T*              pt_svc_req,
    REC_SVC_SRC_BRDCST_SVL_T**  ppt_src_brdcst_svl);
    
static INT32 _rec_svc_util_get_res_brdcst_svl (
    REC_SVC_REQ_T*              pt_svc_req,
    REC_SVC_RES_BRDCST_SVL_T**  ppt_res_brdcst_svl);

static INT32 _rec_svc_state_util_close_conn (
    SVCTX_T*                    pt_svctx,
    BOOL                        b_chk_proceed,
    BOOL                        b_also_pipe,
    BOOL*                       pb_cm_pipe_closing);
    
static INT32 _rec_svc_state_util_update_pmt_es_list_info (
    SVCTX_T*                    pt_svctx,
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl,
    UINT16                      ui2_num_es,
    SVCTX_REC_ES_INFO_T*        at_es_info );

#ifdef MW_TV_AV_BYPASS_SUPPORT
static INT32 _rec_svc_msg_hdlr_queued_action (
    VOID*                       pv_tag,
    struct _SVCTX_MSG_T*        pt_msg);
#endif    
static INT32 _rec_svc_msg_hdlr_crnt_evn_nfy (
    struct _CRNT_EVENT_T*       pt_this,
    CRNT_EVENT_NTFY_CODE_T      e_code,
    VOID*                       pv_tag,
    UINT32                      ui4_data);
    
static INT32 _rec_svc_state_transit_sess_error (
    SVCTX_T*                    pt_svctx);

static VOID _rec_svc_state_transit_error (
    SVCTX_T*                    pt_svctx);

static INT32 _rec_svc_state_error_on_res_closed (
    SVCTX_T*                    pt_svctx);

static INT32 _rec_svc_state_append_event (
    REC_SVC_REQ_T*              pt_svc_req,
    REC_SVC_EVN_TYPE_T          e_event,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data,
    REC_SVC_EVN_INFO_T**        ppt_evn_new);

static INT32 _rec_svc_state_handle_event (
    SVCTX_T*                    pt_svctx,
    REC_SVC_EVN_TYPE_T          e_event_new,
    const VOID*                 pv_data_new,
    SIZE_T                      z_len_data_new);

static VOID _rec_svc_state_clear_event_q (
    REC_SVC_REQ_T*              pt_svc_req);

static VOID _rec_svc_state_act_msg_q_init (
    REC_SVC_REQ_T*              pt_svc_req);

static INT32 _rec_svc_state_act_msg_q_append (
    REC_SVC_REQ_T*              pt_svc_req,
    UINT8                       ui1_act_msg);

static BOOL _rec_svc_state_act_msg_q_check_empty (
    REC_SVC_REQ_T*              pt_svc_req);
    
static INT32 _rec_svc_state_act_msg_q_peek (
    REC_SVC_REQ_T*              pt_svc_req,
    UINT8*                      pui1_act_msg);

static INT32 _rec_svc_state_act_msg_q_remove (
    REC_SVC_REQ_T*              pt_svc_req,
    UINT8*                      pui1_act_msg);
    
static INT32 _rec_svc_state_conning_util_open_pipe (
    SVCTX_T*                    pt_svctx);
    
static INT32 _rec_svc_state_util_close_cm_pipe (
    SVCTX_T*                    pt_svctx);
    
static INT32 _rec_svc_state_conning_util_open_conn (
    SVCTX_T*                    pt_svctx);

static INT32 _rec_svc_state_util_remove_strm (
    REC_SVC_REQ_T*              pt_running_svc_req,
    HANDLE_T                    h_strm);
    
#ifdef CLI_LVL_ALL
static VOID _rec_svc_state_util_dbg_dump_q_events (
    const REC_SVC_REQ_T*        pt_svc_req);

#ifdef MW_TV_AV_BYPASS_SUPPORT
static VOID _rec_svc_state_util_dbg_dump_svctx_mgr_qr (
    const SVCTX_T*              pt_svctx,
    UINT32                      ui4_line_num);
#endif
#endif
#ifdef MW_TV_AV_BYPASS_SUPPORT
static INT32 _rec_svc_util_start_query_crnt_evn( 
    REC_SVC_REQ_T*      pt_running_svc_req );

static INT32 _rec_svc_util_stop_query_crnt_evn(
    REC_SVC_REQ_T*      pt_running_svc_req  );
#endif
static INT32  _rec_svc_util_check_close_conn(
      SVCTX_T*          pt_svctx,
      BOOL*               pb_proceed
#ifdef MW_TV_AV_BYPASS_SUPPORT
	  ,PLAYER_ACTION_T e_phase
#endif
							);

static INT32  _rec_svc_util_check_close_pipe(
      SVCTX_T*          pt_svctx,
      BOOL*               pb_proceed      
#ifdef MW_TV_AV_BYPASS_SUPPORT
	   ,PLAYER_ACTION_T e_phase
#endif
                                );

#ifdef MW_TV_AV_BYPASS_SUPPORT
static PLAYER_TYPE_T _rec_svc_state_util_get_player_type (
    const REC_SVC_REQ_T*    pt_svc_req);
#endif
    
/*-----------------------------------------------------------------------------
                    internal functions
 ----------------------------------------------------------------------------*/
static INT32  _rec_svc_util_check_close_conn(
      SVCTX_T*          pt_svctx,
      BOOL*               pb_proceed      
#ifdef MW_TV_AV_BYPASS_SUPPORT
	  ,PLAYER_ACTION_T e_phase
#endif
								    )
{
    INT32       i4_ret;
    
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;

    if  (!pb_proceed)
    {
        return SVCTXR_FAIL;
    }
    
    if (pt_running_svc_req->b_act_q_in_mgr)
    {
        /* queue this action to process later */
        i4_ret = _rec_svc_state_act_msg_q_append (pt_running_svc_req,
                                                  SVCTX_MSG_REC_QUEUED_CONN_CLOSE);
        if (RET_OK (i4_ret))
        {
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TCC> Act queued -> append ACTION_CONN_CLOSE. (L%d)\n", __LINE__));
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] <TCC> Fail to append ACTION_CONN_CLOSE (%d). (L%d)\n", i4_ret, __LINE__));
        }
        
        *pb_proceed = FALSE;
    }
    else
    {
    #ifdef MW_TV_AV_BYPASS_SUPPORT
        {
            VAR_INFO_T   t_var_info;
            BOOL             b_proceed;
            
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TCC> Check proceed: ACTION_CONN_CLOSE. (L%d)\n", __LINE__));
            
            pt_running_svc_req->t_svcst.e_phase = e_phase;
            
            t_var_info.e_var_type = VAR_TYPE_PLAYER;
            t_var_info.e_op_type  = OP_TYPE_CLOSE;
            t_var_info.ui1_msg_id = SVCTX_MSG_REC_QUEUED_CONN_CLOSE;
            t_var_info.u.t_player_info.e_player_type  = _rec_svc_state_util_get_player_type (pt_running_svc_req);
            t_var_info.u.t_player_info.pt_player      = NULL;
            t_var_info.u.t_player_info.pf_invoke_func = _rec_svc_msg_hdlr_queued_action;
            
            b_proceed = svctx_mgr_ready_to_proceed (ACTIVE_LINK_NOW,
                                                    pt_running_svc_req->e_src_rec,
                                                    pt_svctx,
                                                    & t_var_info);

            if (!b_proceed)
            {
                pt_running_svc_req->b_act_q_in_mgr = TRUE;
                
                i4_ret = _rec_svc_state_act_msg_q_append (pt_running_svc_req,
                                                          SVCTX_MSG_REC_QUEUED_CONN_CLOSE);
                
                if (RET_OK (i4_ret))
                {
                    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TCC> Action queued: ACTION_CONN_CLOSE. (L%d)\n", __LINE__));
                }
                else
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] <TCC> Fail to queue: ACTION_CONN_CLOSE (%d). (L%d)\n", i4_ret, __LINE__));
                }
                
                DBG_INFO_DUMP_SVCTX_MGR_REQ_Q (pt_svctx);
            }
            *pb_proceed = b_proceed;
        }
    #else
        {
            *pb_proceed = TRUE;
        }
    #endif
        
    }
    return SVCTXR_OK;
}

static INT32  _rec_svc_util_check_close_pipe(
      SVCTX_T*          pt_svctx,
      BOOL*               pb_proceed      
#ifdef MW_TV_AV_BYPASS_SUPPORT
	  ,PLAYER_ACTION_T e_phase
#endif
									)
{
    INT32       i4_ret;

    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;

    if  (!pb_proceed)
    {
        return SVCTXR_FAIL;
    }
    
    if (pt_running_svc_req->b_act_q_in_mgr)
    {
        /* queue this action to process later */
        i4_ret = _rec_svc_state_act_msg_q_append (pt_running_svc_req,
                                                  SVCTX_MSG_REC_QUEUED_PIPE_CLOSE);
        if (RET_OK (i4_ret))
        {
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TCP> Act queued -> append ACTION_PIPE_CLOSE. (L%d)\n", __LINE__));
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] <TCP> Fail to append ACTION_PIPE_CLOSE (%d). (L%d)\n", i4_ret, __LINE__));
        }                
        
        *pb_proceed = FALSE;
    }
    else
    {
    #ifdef MW_TV_AV_BYPASS_SUPPORT
        {
            VAR_INFO_T   t_var_info;
            BOOL             b_proceed;
            
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TCP> Check proceed: ACTION_PIPE_CLOSE. (L%d)\n", __LINE__));
            
            pt_running_svc_req->t_svcst.e_phase = e_phase;
            
            t_var_info.e_var_type = VAR_TYPE_PLAYER;
            t_var_info.e_op_type  = OP_TYPE_CLOSE;
            t_var_info.ui1_msg_id = SVCTX_MSG_REC_QUEUED_PIPE_CLOSE;
            t_var_info.u.t_player_info.e_player_type  = _rec_svc_state_util_get_player_type (pt_running_svc_req);
            t_var_info.u.t_player_info.pt_player      = NULL;
            t_var_info.u.t_player_info.pf_invoke_func = _rec_svc_msg_hdlr_queued_action;
            
            b_proceed = svctx_mgr_ready_to_proceed (ACTIVE_LINK_NOW,
                                                    pt_running_svc_req->e_src_rec,
                                                    pt_svctx,
                                                    & t_var_info);
            if (!b_proceed)
            {
                pt_running_svc_req->b_act_q_in_mgr = TRUE;
                
                i4_ret = _rec_svc_state_act_msg_q_append (pt_running_svc_req,
                                                          SVCTX_MSG_REC_QUEUED_PIPE_CLOSE);
                if (RET_OK (i4_ret))
                {
                    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TCP> Action queued: ACTION_PIPE_CLOSE. (L%d)\n", __LINE__));
                }
                else
                {
                    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TCP> Fail to queue: ACTION_PIPE_CLOSE. (L%d)\n", __LINE__));
                }
                
                DBG_INFO_DUMP_SVCTX_MGR_REQ_Q (pt_svctx);
            }
            *pb_proceed = b_proceed;
        }
    #else
        {
            *pb_proceed = TRUE;
        }
    #endif
    }
    
    return SVCTXR_OK;
    
}

#ifdef MW_TV_AV_BYPASS_SUPPORT
static INT32 _rec_svc_util_start_query_crnt_evn( REC_SVC_REQ_T*      pt_running_svc_req )
{
    INT32       i4_ret;
    REC_SVC_SRC_BRDCST_SVL_T*   pt_src_brdcst_svl;
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl;
    
    i4_ret = _rec_svc_util_get_src_brdcst_svl (pt_running_svc_req,
                                               & pt_src_brdcst_svl);
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get SRC_BRDCST_SVL (%d). (L%d)\n", i4_ret, __LINE__));
        pt_src_brdcst_svl = NULL;
        i4_ret = SVCTXR_FAIL;
    }
    
    /* get res-brdcst-svl */
    i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req,
                                               & pt_res_brdcst_svl);
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Not supported SRC_TYPE. (L%d)\n", __LINE__));
        pt_res_brdcst_svl = NULL;
        i4_ret = SVCTXR_FAIL;
    }    
    
    if (pt_src_brdcst_svl  &&
        pt_res_brdcst_svl  &&
        pt_res_brdcst_svl->e_brdcst_type != BRDCST_TYPE_ANALOG)
    {
        CRNT_EVENT_KEY_T    t_key;
        
        t_key.t_svl.ui2_svc_lst_id = pt_src_brdcst_svl->t_param.ui2_svc_lst_id;
        t_key.t_svl.ui2_svl_rec_id = pt_src_brdcst_svl->t_param.ui2_svl_rec_id;
#ifdef MW_EDB_SUPPORT                
        t_key.t_svl.ui4_channel_id = pt_src_brdcst_svl->t_param.ui4_channel_id;
#endif
        
        /* start query event */
        i4_ret = crnt_evn_start_query (pt_running_svc_req->pt_crnt_evn,
                                       DEV_TUNER, /* pt_res_brdcst_svl != NULL so must be TUNER */
                                       pt_running_svc_req->s_src_grp_name,
                                       pt_running_svc_req->h_conn,
                                       & t_key );
        if (RET_OK (i4_ret))
        {
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Start query event (h_event=%d, evctx_cond=%d). (L%d)\n", 
                       crnt_evn_get_handle (pt_running_svc_req->pt_crnt_evn),
                       crnt_evn_get_cond (pt_running_svc_req->pt_crnt_evn),
                       __LINE__));
            return SVCTXR_OK;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to start query event (%d). (L%d)\n", i4_ret, __LINE__));
            return SVCTXR_FAIL;
        }

    }
    return SVCTXR_OK;

}
static INT32 _rec_svc_util_stop_query_crnt_evn(REC_SVC_REQ_T*      pt_running_svc_req  )
{
    INT32       i4_ret;
    i4_ret = crnt_evn_stop_query (pt_running_svc_req->pt_crnt_evn);
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to stop query CRNT_EVN (%d). (L%d)\n", i4_ret, __LINE__));
        return SVCTXR_FAIL;
    }
    return SVCTXR_OK;
}
#endif

static INT32 _rec_svc_intr_get_cm_cmds (
    SVCTX_T*                    pt_svctx,
    REC_SVC_REQ_T*              pt_running_svc_req,
    CM_COMMAND_T*               at_cm_cmds,
    UINT32                      ui4_max_cm_cmds)
{
    UINT32      ui4_idx = 0;
    INT32       i4_ret;
    
    at_cm_cmds[0].e_code = CONN_END;
    
    switch (pt_running_svc_req->e_src_rec) 
    {
    case SRC_TYPE_BRDCST_SVL:
    {
        REC_SVC_SRC_BRDCST_SVL_T*  pt_src_brdcst_svl = &pt_running_svc_req->u_src.t_svl;
        REC_SVC_RES_BRDCST_SVL_T*  pt_res_brdcst_svl = &pt_running_svc_req->u_res.t_svl;
        SVL_REC_T                  t_svl_rec = {0};
        
        i4_ret = rec_svc_req_get_svl_rec (pt_running_svc_req,
                                          & t_svl_rec);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get current SVL Record (%d). (L%d)\n", i4_ret, __LINE__));
            break;
        }
        
        switch (pt_res_brdcst_svl->e_brdcst_type)
        {
        case BRDCST_TYPE_UNKNOWN:
        case BRDCST_TYPE_ATSC:
        case BRDCST_TYPE_SCTE:
        {   
            at_cm_cmds[ui4_idx].e_code       = CONN_SRC_TYPE;
            at_cm_cmds[ui4_idx].u.ps_name    = BRDCST_HANDLER_DEFAULT_NAME;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = CONN_SRC_NAME;
            at_cm_cmds[ui4_idx].u.ps_name    = pt_running_svc_req->s_src_grp_name;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_SVC_LST_ID;
            at_cm_cmds[ui4_idx].u.ui2_number = pt_src_brdcst_svl->t_param.ui2_svc_lst_id;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_SVL_REC_ID;
            at_cm_cmds[ui4_idx].u.ui2_number = t_svl_rec.ui2_svl_rec_id;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_PAT_TIME_OUT;
            at_cm_cmds[ui4_idx].u.ui4_number = ATSC_PAT_QUERY_TIMEOUT;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_PMT_TIME_OUT;
            at_cm_cmds[ui4_idx].u.ui4_number = ATSC_PMT_QUERY_TIMEOUT;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_2_LANGUAGES_1_STREAM;
            at_cm_cmds[ui4_idx].u.b_boolean  = FALSE; /* To have 2 DUAL_MONO streams when there are 2 languages */
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_N_LANGUAGES_1_STREAM;
            at_cm_cmds[ui4_idx].u.b_boolean  = FALSE; /* To have n UNKNOWN streams when there are n languages (n > 2) */
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_TUNER_NO_RIGHTS_OK;
            at_cm_cmds[ui4_idx].u.b_boolean  = TRUE;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_SIGNAL_LOST_DISCONNECT;
            at_cm_cmds[ui4_idx].u.b_boolean  = FALSE;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code         = BRDCST_CONN_MGT_TIME_OUT;
            at_cm_cmds[ui4_idx].u.ui4_number = ATSC_MGT_QUERY_TIMEOUT;
            ui4_idx++;
            if (pt_running_svc_req->b_preset_conn_info)
            {
                if (x_handle_valid (pt_running_svc_req->t_conn_info_preset.h_conn))
                {
                    at_cm_cmds[ui4_idx].e_code     = BRDCST_CONN_REUSE_CONNECTION;
                    at_cm_cmds[ui4_idx].u.h_handle = pt_running_svc_req->t_conn_info_preset.h_conn;
                    ui4_idx++;
                    pt_running_svc_req->h_conn_reusing_by_cm = pt_running_svc_req->t_conn_info_preset.h_conn;
                }
                pt_running_svc_req->t_conn_info_preset.h_conn = NULL_HANDLE;
            }
            
            at_cm_cmds[ui4_idx].e_code       = CONN_END;
            
            i4_ret = SVCTXR_OK;
            break;
        }    
            
        case BRDCST_TYPE_DVB:
            at_cm_cmds[ui4_idx].e_code       = CONN_SRC_TYPE;
            at_cm_cmds[ui4_idx].u.ps_name    = BRDCST_HANDLER_DEFAULT_NAME;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = CONN_SRC_NAME;
            at_cm_cmds[ui4_idx].u.ps_name    = pt_running_svc_req->s_src_grp_name;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_SVC_LST_ID;
            at_cm_cmds[ui4_idx].u.ui2_number = pt_src_brdcst_svl->t_param.ui2_svc_lst_id;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_SVL_REC_ID;
            at_cm_cmds[ui4_idx].u.ui2_number = t_svl_rec.ui2_svl_rec_id;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_PAT_TIME_OUT;
            at_cm_cmds[ui4_idx].u.ui4_number = DVB_PAT_QUERY_TIMEOUT;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_PMT_TIME_OUT;
            at_cm_cmds[ui4_idx].u.ui4_number = DVB_PMT_QUERY_TIMEOUT;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_TUNER_NO_RIGHTS_OK;
            at_cm_cmds[ui4_idx].u.b_boolean  = TRUE;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_SIGNAL_LOST_DISCONNECT;
            at_cm_cmds[ui4_idx].u.b_boolean  = FALSE;
            ui4_idx++;
            
            if (pt_running_svc_req->b_preset_conn_info)
            {
                if (x_handle_valid (pt_running_svc_req->t_conn_info_preset.h_conn))
                {
                    at_cm_cmds[ui4_idx].e_code     = BRDCST_CONN_REUSE_CONNECTION;
                    at_cm_cmds[ui4_idx].u.h_handle = pt_running_svc_req->t_conn_info_preset.h_conn;
                    ui4_idx++;
                    pt_running_svc_req->h_conn_reusing_by_cm = pt_running_svc_req->t_conn_info_preset.h_conn;
                }
                pt_running_svc_req->t_conn_info_preset.h_conn = NULL_HANDLE;
            }
            at_cm_cmds[ui4_idx].e_code         = BRDCST_CONN_ALTER_FREQ_TUNE;
            at_cm_cmds[ui4_idx].u.b_boolean    = pt_running_svc_req->t_riks_tv.b_tune_alter_freq_if_loss_signal;
	        ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = CONN_END;
            
            i4_ret = SVCTXR_OK;
            break;
#ifdef SYS_MHP_SUPPORT
        case BRDCST_TYPE_MHP:
            at_cm_cmds[ui4_idx].e_code       = CONN_SRC_TYPE;
            at_cm_cmds[ui4_idx].u.ps_name    = BRDCST_HANDLER_DEFAULT_NAME;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = CONN_SRC_NAME;
            at_cm_cmds[ui4_idx].u.ps_name    = pt_running_svc_req->s_src_grp_name;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_SVC_LST_ID;
            if (pt_src_brdcst_svl->t_param.t_silent.b_enable == TRUE)
            {
                at_cm_cmds[ui4_idx].u.ui2_number = pt_src_brdcst_svl->t_param.t_silent.ui2_svc_lst_id;
            }
            else
            {
                at_cm_cmds[ui4_idx].u.ui2_number = pt_src_brdcst_svl->t_param.ui2_svc_lst_id;
            }
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_SVL_REC_ID;
            at_cm_cmds[ui4_idx].u.ui2_number = t_svl_rec.ui2_svl_rec_id;
			ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_PAT_TIME_OUT;
            at_cm_cmds[ui4_idx].u.ui4_number = 0;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_PMT_TIME_OUT;
            at_cm_cmds[ui4_idx].u.ui4_number = 0;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_TUNER_NO_RIGHTS_OK;
            at_cm_cmds[ui4_idx].u.b_boolean  = TRUE;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_SIGNAL_LOST_DISCONNECT;
            at_cm_cmds[ui4_idx].u.b_boolean  = FALSE;
            ui4_idx++;
            
            if (pt_running_svc_req->b_preset_conn_info)
            {
                if (x_handle_valid (pt_running_svc_req->t_conn_info_preset.h_conn))
                {
                    at_cm_cmds[ui4_idx].e_code     = BRDCST_CONN_REUSE_CONNECTION;
                    at_cm_cmds[ui4_idx].u.h_handle = pt_running_svc_req->t_conn_info_preset.h_conn;
                    ui4_idx++;
                    pt_running_svc_req->h_conn_reusing_by_cm = pt_running_svc_req->t_conn_info_preset.h_conn;
                }
                pt_running_svc_req->t_conn_info_preset.h_conn = NULL_HANDLE;
            }

            at_cm_cmds[ui4_idx].e_code       = CONN_END;
            
            i4_ret = SVCTXR_OK;
            break;
#endif
        case BRDCST_TYPE_DTMB:
            at_cm_cmds[ui4_idx].e_code       = CONN_SRC_TYPE;
            at_cm_cmds[ui4_idx].u.ps_name    = BRDCST_HANDLER_DEFAULT_NAME;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = CONN_SRC_NAME;
            at_cm_cmds[ui4_idx].u.ps_name    = pt_running_svc_req->s_src_grp_name;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_SVC_LST_ID;
            at_cm_cmds[ui4_idx].u.ui2_number = pt_src_brdcst_svl->t_param.ui2_svc_lst_id;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_SVL_REC_ID;
            at_cm_cmds[ui4_idx].u.ui2_number = t_svl_rec.ui2_svl_rec_id;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_PAT_TIME_OUT;
            at_cm_cmds[ui4_idx].u.ui4_number = DTMB_PAT_QUERY_TIMEOUT;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_PMT_TIME_OUT;
            at_cm_cmds[ui4_idx].u.ui4_number = DTMB_PMT_QUERY_TIMEOUT;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_TUNER_NO_RIGHTS_OK;
            at_cm_cmds[ui4_idx].u.b_boolean  = TRUE;
            ui4_idx++;
            at_cm_cmds[ui4_idx].e_code       = BRDCST_CONN_SIGNAL_LOST_DISCONNECT;
            at_cm_cmds[ui4_idx].u.b_boolean  = FALSE;
            ui4_idx++;
            
            if (pt_running_svc_req->b_preset_conn_info)
            {
                if (x_handle_valid (pt_running_svc_req->t_conn_info_preset.h_conn))
                {
                    at_cm_cmds[ui4_idx].e_code     = BRDCST_CONN_REUSE_CONNECTION;
                    at_cm_cmds[ui4_idx].u.h_handle = pt_running_svc_req->t_conn_info_preset.h_conn;
                    ui4_idx++;
                    pt_running_svc_req->h_conn_reusing_by_cm = pt_running_svc_req->t_conn_info_preset.h_conn;
                }
                pt_running_svc_req->t_conn_info_preset.h_conn = NULL_HANDLE;
            }

            at_cm_cmds[ui4_idx].e_code       = CONN_END;
            
            i4_ret = SVCTXR_OK;
            break;
		case BRDCST_TYPE_ISDB:
			at_cm_cmds[ui4_idx].e_code         = CONN_SRC_TYPE;
            at_cm_cmds[ui4_idx].u.ps_name      = BRDCST_HANDLER_DEFAULT_NAME;
			ui4_idx++;
            at_cm_cmds[ui4_idx].e_code         = CONN_SRC_NAME;
            at_cm_cmds[ui4_idx].u.ps_name      = pt_running_svc_req->s_src_grp_name;
			ui4_idx++;
            at_cm_cmds[ui4_idx].e_code         = BRDCST_CONN_SVC_LST_ID;
            at_cm_cmds[ui4_idx].u.ui2_number = pt_src_brdcst_svl->t_param.ui2_svc_lst_id;
			ui4_idx++;
            at_cm_cmds[ui4_idx].e_code         = BRDCST_CONN_SVL_REC_ID;
            at_cm_cmds[ui4_idx].u.ui2_number   = t_svl_rec.ui2_svl_rec_id;
			ui4_idx++;
            at_cm_cmds[ui4_idx].e_code         = BRDCST_CONN_PAT_TIME_OUT;
            at_cm_cmds[ui4_idx].u.ui4_number   = DVB_PAT_QUERY_TIMEOUT;
			ui4_idx++;
            at_cm_cmds[ui4_idx].e_code         = BRDCST_CONN_PMT_TIME_OUT;
            at_cm_cmds[ui4_idx].u.ui4_number   = DVB_PMT_QUERY_TIMEOUT;
			ui4_idx++;
            at_cm_cmds[ui4_idx].e_code         = BRDCST_CONN_2_LANGUAGES_1_STREAM;
            at_cm_cmds[ui4_idx].u.b_boolean    = FALSE; 
			ui4_idx++;
            at_cm_cmds[ui4_idx].e_code         = BRDCST_CONN_N_LANGUAGES_1_STREAM;
            at_cm_cmds[ui4_idx].u.b_boolean    = FALSE; 
			ui4_idx++;			
            at_cm_cmds[ui4_idx].e_code         = BRDCST_CONN_TUNER_NO_RIGHTS_OK;
            at_cm_cmds[ui4_idx].u.b_boolean    = TRUE;
			ui4_idx++;
            at_cm_cmds[ui4_idx].e_code         = BRDCST_CONN_SIGNAL_LOST_DISCONNECT;
            at_cm_cmds[ui4_idx].u.b_boolean    = FALSE;
			ui4_idx++;
			if (pt_running_svc_req->b_preset_conn_info)
			{
				if (x_handle_valid (pt_running_svc_req->t_conn_info_preset.h_conn))
				{
					at_cm_cmds[ui4_idx].e_code	   = BRDCST_CONN_REUSE_CONNECTION;
					at_cm_cmds[ui4_idx].u.h_handle = pt_running_svc_req->t_conn_info_preset.h_conn;
					ui4_idx++;
					pt_running_svc_req->h_conn_reusing_by_cm = pt_running_svc_req->t_conn_info_preset.h_conn;
				}
				pt_running_svc_req->t_conn_info_preset.h_conn = NULL_HANDLE;
			}
            at_cm_cmds[ui4_idx].e_code         = CONN_END;
            break;                        
        case BRDCST_TYPE_ANALOG:
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Not supported Broadcast Type (%d). (L%d)\n", pt_res_brdcst_svl->e_brdcst_type, __LINE__));
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        
        default:
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Not supported Broadcast Type (%d). (L%d)\n", pt_res_brdcst_svl->e_brdcst_type, __LINE__));
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }
        
        break;
    }
        
    case SRC_TYPE_BRDCST_FREQ:
    case SRC_TYPE_AVC:
    default:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Not supported Source Type (%d). (L%d)\n", pt_running_svc_req->e_src_rec, __LINE__));
        i4_ret = SVCTXR_NOT_SUPPORT;
        break;
    } 

    DBG_ASSERT (ui4_idx < ui4_max_cm_cmds, DBG_MOD_SVCTX);

    return i4_ret;
}    

static INT32 _rec_svc_intr_update_event_info (
    SVCTX_T*                    pt_svctx)
{
#ifndef MW_EDB_SUPPORT    
    static const REC_SVC_EVCTX_KEY_INFO_T  at_key_info_unknown[] = 
    {
        {EVCTX_KEY_TYPE_START_TIME,     REC_SVC_EVCTX_KEY_INFO_TYPE_NULL},
        {EVCTX_KEY_TYPE_DURATION,       REC_SVC_EVCTX_KEY_INFO_TYPE_NULL},
        {EVCTX_KEY_TYPE_EVENT_TITLE,    REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EVCTX_KEY_TYPE_EVENT_TEXT,     REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EVCTX_KEY_TYPE_SVC_NAME,       REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EVCTX_KEY_TYPE_SVC_TEXT,       REC_SVC_EVCTX_KEY_INFO_TYPE_LANG}
    };

    static const REC_SVC_EVCTX_KEY_INFO_T  at_key_info_atsc[] = 
    {
        {EVCTX_KEY_TYPE_START_TIME,     REC_SVC_EVCTX_KEY_INFO_TYPE_NULL},
        {EVCTX_KEY_TYPE_DURATION,       REC_SVC_EVCTX_KEY_INFO_TYPE_NULL},
        {EVCTX_KEY_TYPE_EVENT_TITLE,    REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EVCTX_KEY_TYPE_EVENT_TEXT,     REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EVCTX_KEY_TYPE_SVC_NAME,       REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EVCTX_KEY_TYPE_SVC_TEXT,       REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EVCTX_KEY_TYPE_RATING,         REC_SVC_EVCTX_KEY_INFO_TYPE_LANG}
    };

    static const REC_SVC_EVCTX_KEY_INFO_T  at_key_info_dvb[] = 
    {
        {EVCTX_KEY_TYPE_START_TIME,     REC_SVC_EVCTX_KEY_INFO_TYPE_NULL},
        {EVCTX_KEY_TYPE_DURATION,       REC_SVC_EVCTX_KEY_INFO_TYPE_NULL},
        {EVCTX_KEY_TYPE_EVENT_TITLE,    REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EVCTX_KEY_TYPE_EVENT_TEXT,     REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EVCTX_KEY_TYPE_SVC_NAME,       REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EVCTX_KEY_TYPE_SVC_TEXT,       REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EVCTX_KEY_TYPE_RATING,         REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EVCTX_KEY_TYPE_EVENT_EXT_TEXT, REC_SVC_EVCTX_KEY_INFO_TYPE_LANG}
    };
    static const REC_SVC_EVCTX_KEY_INFO_T  at_key_info_dtmb[] = 
    {
        {EVCTX_KEY_TYPE_START_TIME,     REC_SVC_EVCTX_KEY_INFO_TYPE_NULL},
        {EVCTX_KEY_TYPE_DURATION,       REC_SVC_EVCTX_KEY_INFO_TYPE_NULL},
        {EVCTX_KEY_TYPE_EVENT_TITLE,    REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EVCTX_KEY_TYPE_EVENT_TEXT,     REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EVCTX_KEY_TYPE_SVC_NAME,       REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EVCTX_KEY_TYPE_SVC_TEXT,       REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EVCTX_KEY_TYPE_RATING,         REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EVCTX_KEY_TYPE_EVENT_EXT_TEXT, REC_SVC_EVCTX_KEY_INFO_TYPE_LANG}
    };
	static const REC_SVC_EVCTX_KEY_INFO_T  at_key_info_isdb[] = 
    {
        {EVCTX_KEY_TYPE_START_TIME,     REC_SVC_EVCTX_KEY_INFO_TYPE_NULL},
        {EVCTX_KEY_TYPE_DURATION,       REC_SVC_EVCTX_KEY_INFO_TYPE_NULL},
        {EVCTX_KEY_TYPE_EVENT_TITLE,    REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EVCTX_KEY_TYPE_EVENT_TEXT,     REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EVCTX_KEY_TYPE_SVC_NAME,       REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EVCTX_KEY_TYPE_SVC_TEXT,       REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EVCTX_KEY_TYPE_RATING,         REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EVCTX_KEY_TYPE_EVENT_EXT_TEXT, REC_SVC_EVCTX_KEY_INFO_TYPE_LANG}
    };
#else
    static const REC_SVC_EVCTX_KEY_INFO_T  at_key_info_unknown[] = 
    {
        {EDB_KEY_TYPE_START_TIME,         REC_SVC_EVCTX_KEY_INFO_TYPE_NULL},
        {EDB_KEY_TYPE_DURATION,           REC_SVC_EVCTX_KEY_INFO_TYPE_NULL},
        {EDB_KEY_TYPE_EVENT_TITLE_BY_ID,  REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EDB_KEY_TYPE_EVENT_DETAIL_BY_ID, REC_SVC_EVCTX_KEY_INFO_TYPE_LANG}
        
    };

    static const REC_SVC_EVCTX_KEY_INFO_T  at_key_info_atsc[] = 
    {
        {EDB_KEY_TYPE_START_TIME,         REC_SVC_EVCTX_KEY_INFO_TYPE_NULL},
        {EDB_KEY_TYPE_DURATION,           REC_SVC_EVCTX_KEY_INFO_TYPE_NULL},
	    {EDB_KEY_TYPE_EVENT_TITLE_BY_ID,  REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EDB_KEY_TYPE_EVENT_DETAIL_BY_ID, REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},        
        {EDB_KEY_TYPE_RATING_LIST_BY_ID,  REC_SVC_EVCTX_KEY_INFO_TYPE_LANG}
    };

    static const REC_SVC_EVCTX_KEY_INFO_T  at_key_info_dvb[] = 
    {
        {EDB_KEY_TYPE_START_TIME,        REC_SVC_EVCTX_KEY_INFO_TYPE_NULL},
        {EDB_KEY_TYPE_DURATION,          REC_SVC_EVCTX_KEY_INFO_TYPE_NULL},
        {EDB_KEY_TYPE_EVENT_TITLE_BY_ID, REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EDB_KEY_TYPE_EVENT_DETAIL_BY_ID,REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},        
        {EDB_KEY_TYPE_RATING_LIST_BY_ID, REC_SVC_EVCTX_KEY_INFO_TYPE_LANG}
    };
    static const REC_SVC_EVCTX_KEY_INFO_T  at_key_info_isdb[] = 
    {
        {EDB_KEY_TYPE_START_TIME,          REC_SVC_EVCTX_KEY_INFO_TYPE_NULL},
        {EDB_KEY_TYPE_DURATION,            REC_SVC_EVCTX_KEY_INFO_TYPE_NULL},
        {EDB_KEY_TYPE_EVENT_TITLE_BY_ID,   REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EDB_KEY_TYPE_EVENT_DETAIL_BY_ID,  REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EDB_KEY_TYPE_RATING_LIST_BY_ID,   REC_SVC_EVCTX_KEY_INFO_TYPE_LANG}
    };
	static const REC_SVC_EVCTX_KEY_INFO_T  at_key_info_dtmb[] = 
    {
        {EDB_KEY_TYPE_UNKNOWN,       REC_SVC_EVCTX_KEY_INFO_TYPE_NULL},
        {EDB_KEY_TYPE_UNKNOWN,       REC_SVC_EVCTX_KEY_INFO_TYPE_NULL},
        {EDB_KEY_TYPE_UNKNOWN,       REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EDB_KEY_TYPE_UNKNOWN,       REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EDB_KEY_TYPE_UNKNOWN,       REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EDB_KEY_TYPE_UNKNOWN,       REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EDB_KEY_TYPE_UNKNOWN,       REC_SVC_EVCTX_KEY_INFO_TYPE_LANG},
        {EDB_KEY_TYPE_UNKNOWN,       REC_SVC_EVCTX_KEY_INFO_TYPE_LANG}
    };
#endif    
    REC_SVC_REQ_T*                   pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    HANDLE_T                         h_event     = crnt_evn_get_handle (pt_running_svc_req->pt_crnt_evn);
#ifndef MW_EDB_SUPPORT   
    EVCTX_COND_T                     e_cond_evn  = crnt_evn_get_cond (pt_running_svc_req->pt_crnt_evn);
#endif

    const REC_SVC_EVCTX_KEY_INFO_T*  at_key_info;
    UINT32                           ui4_num_key;
    REC_SVC_RES_BRDCST_SVL_T*        pt_res_brdcst_svl;
    PVR_BUFFER_STATUS_T              t_pvr_buf_st;
    SIZE_T                           z_event_info_len, z_event_info_len1,z_get_info_len;
#ifndef MW_EDB_SUPPORT    
    EVCTX_COND_T                     e_cond_evn_cur;
	VOID*							 pv_key_info;
#else
	EDB_CH_EVT_ID_T                  t_ch_evt_id;
#endif

    UINT32                           ui4_lba, ui4_i;
    INT32                            i4_ret;
    
    /* check pre-conditions */
    if (!(pt_running_svc_req->e_cond_rec_util == REC_UTIL_COND_READY  &&
          pt_running_svc_req->e_cond_sess == SM_SESS_COND_STARTED     &&
#ifndef MW_EDB_SUPPORT 
          REC_SVC_EVCTX_INFO_AVAIL (e_cond_evn)                       &&
#endif
          pt_running_svc_req->pt_rec_cfg ))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid state to update event info. (L%d)\n", __LINE__));
        return EVCTXR_INV_COND;
    }
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Update event info. (L%d)\n", __LINE__));

    i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req, 
                                               & pt_res_brdcst_svl );
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get RES_BRDCST_SVL (%d). (L%d)\n", i4_ret, __LINE__));
        pt_res_brdcst_svl = NULL;
    }
#ifdef MW_EDB_SUPPORT
    /* Key info prepare */
    t_ch_evt_id.e_brdcst_type     = pt_res_brdcst_svl ? pt_res_brdcst_svl->e_brdcst_type : BRDCST_TYPE_UNKNOWN;
    t_ch_evt_id.ui2_svl_id        = pt_running_svc_req->u_src.t_svl.t_param.ui2_svc_lst_id;
    t_ch_evt_id.ui4_channel_id    = pt_running_svc_req->u_src.t_svl.t_param.ui4_channel_id;
    t_ch_evt_id.ui2_event_id      = 0;
    t_ch_evt_id.b_auto_event_id   = TRUE;
#endif
    if (pt_res_brdcst_svl)
    {
        switch (pt_res_brdcst_svl->e_brdcst_type)
        {
        case BRDCST_TYPE_UNKNOWN:
            at_key_info = at_key_info_unknown;
            ui4_num_key = (UINT32) (sizeof (at_key_info_unknown) / sizeof (REC_SVC_EVCTX_KEY_INFO_T));
            break;
            
        case BRDCST_TYPE_ATSC:
            at_key_info = at_key_info_atsc;
            ui4_num_key = (UINT32) (sizeof (at_key_info_atsc) / sizeof (REC_SVC_EVCTX_KEY_INFO_T));
            break;
        
        case BRDCST_TYPE_DVB:
            at_key_info = at_key_info_dvb;
            ui4_num_key = (UINT32) (sizeof (at_key_info_dvb) / sizeof (REC_SVC_EVCTX_KEY_INFO_T));
            break;

        case BRDCST_TYPE_DTMB:
            at_key_info = at_key_info_dtmb;
            ui4_num_key = (UINT32) (sizeof (at_key_info_dtmb) / sizeof (REC_SVC_EVCTX_KEY_INFO_T));
            break;
		case BRDCST_TYPE_ISDB:
			at_key_info = at_key_info_isdb;
            ui4_num_key = (UINT32) (sizeof (at_key_info_isdb) / sizeof (REC_SVC_EVCTX_KEY_INFO_T));
			break; 
            
        default:
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal error. (L%d)\n", __LINE__));
            return EVCTXR_INTERNAL_ERROR;
        }
    }
    else
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Not supported src type. (L%d)\n", __LINE__));
        return SVCTXR_NOT_SUPPORT;
    }
    
    for (ui4_i = 0; ui4_i < ui4_num_key; ui4_i++)
    {
#ifndef MW_EDB_SUPPORT
        switch (at_key_info[ui4_i].e_info_type)
        {
        case REC_SVC_EVCTX_KEY_INFO_TYPE_NULL:
            pv_key_info = NULL;
            break;
            
        case REC_SVC_EVCTX_KEY_INFO_TYPE_LANG:
            pv_key_info = (VOID *) pt_running_svc_req->pt_rec_cfg->s_lang;
            break;
            
        default:
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal error. (L%d)\n", __LINE__));
            return EVCTXR_INTERNAL_ERROR;
        }
        i4_ret = x_evctx_get_event_info_len (h_event,
                                             at_key_info[ui4_i].e_key_type,
                                             pv_key_info,
                                             & z_event_info_len,
                                             & e_cond_evn_cur);		
#else
	    i4_ret = x_edb_get_info_len(h_event,
			                        at_key_info[ui4_i].e_key_type,
			                        &t_ch_evt_id,
			                        &z_event_info_len
			                        );    	
#endif
#ifndef MW_EDB_SUPPORT        
        if (i4_ret == EVCTXR_INFO_NOT_FOUND) {   
		 	pv_key_info = NULL;

	        i4_ret = x_evctx_get_event_info_len (h_event,
	                                             at_key_info[ui4_i].e_key_type,
	                                             pv_key_info,
	                                             & z_event_info_len,
	                                             & e_cond_evn_cur);

        }
#endif  
        if (i4_ret != EVCTXR_OK)/*the same value with EDBR_OK*/
        {
            if (i4_ret == EVCTXR_INFO_NOT_FOUND)/*the same value with */
            {
                /* try next key type */
                continue;
            }
            else
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get event info len (%d) (key=%d). (L%d)\n", 
                            i4_ret, 
                            at_key_info[ui4_i].e_key_type,
                            __LINE__));
                return SVCTXR_FAIL;
            }
        }
        
        if (z_event_info_len > pt_running_svc_req->z_crnt_evn_len)
        {
            VOID*     pv_new_buf;
            SIZE_T    z_new_len;
            
            /* reallocate event data buffer */
            if (pt_running_svc_req->pv_crnt_evn_data != (VOID *) pt_running_svc_req->aui1_crnt_evn)
            {
                x_mem_free (pt_running_svc_req->pv_crnt_evn_data);
                pt_running_svc_req->pv_crnt_evn_data = (VOID *) pt_running_svc_req->aui1_crnt_evn;
                pt_running_svc_req->z_crnt_evn_len   = REC_SVC_PRG_EVN_DATA_SIZE_INIT;
            }
            
            z_new_len = z_event_info_len + REC_SVC_PRG_EVN_DATA_SIZE_EXPAND;
            pv_new_buf = x_mem_alloc (z_new_len);
            
            if (pv_new_buf == NULL)
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Out of memory. (L%d)\n", __LINE__));
                return SVCTXR_NO_RESOURCE;
            }
            
            pt_running_svc_req->pv_crnt_evn_data = pv_new_buf;
            pt_running_svc_req->z_crnt_evn_len   = z_new_len;
        }
        
        z_event_info_len1 = pt_running_svc_req->z_crnt_evn_len;		
#ifndef MW_EDB_SUPPORT        
		i4_ret = x_evctx_get_event_info (h_event,
                                         at_key_info[ui4_i].e_key_type,
                                         pv_key_info,
                                         & z_event_info_len1,
                                         pt_running_svc_req->pv_crnt_evn_data,
                                         & e_cond_evn_cur );
		if (i4_ret == EVCTXR_OK) {
#else
		i4_ret = x_edb_get_info(h_event,
								at_key_info[ui4_i].e_key_type,
								&t_ch_evt_id, 
								& z_event_info_len1,
								pt_running_svc_req->pv_crnt_evn_data);
		if (i4_ret == EDBR_OK) {
#endif

            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Got event info (key=%d). (L%d)\n", 
                       at_key_info[ui4_i].e_key_type,
                       __LINE__));
        }
        else
        {
            if (i4_ret == EVCTXR_INFO_NOT_FOUND)
            {
                /* try next key type */
                continue;
            }
            else
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get event info (%d) (key=%d). (L%d)\n", 
                            i4_ret, 
                            at_key_info[ui4_i].e_key_type,
                            __LINE__));
                return SVCTXR_FAIL;
            }
        }
        
        z_get_info_len = sizeof (PVR_BUFFER_STATUS_T);
        i4_ret = x_sm_get (pt_running_svc_req->h_rcrd_sess,
                           SM_SESS_RCRD_GET_TYPE_PVR_BUFFER_STATUS,
                           (VOID *) & t_pvr_buf_st,
                           & z_get_info_len);

        if (i4_ret != SMR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get PVR buffer status (%d). (L%d)\n", i4_ret, __LINE__));
            return SVCTXR_FAIL;
        } 
        
        i4_ret = rec_util_get_lba_by_pvr_buf_status (pt_running_svc_req->h_rec_util,
                                                     & t_pvr_buf_st,
                                                     & ui4_lba );
        if (i4_ret != REC_UTILR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get LBA by PVR buffer status (%d). (L%d)\n", i4_ret, __LINE__));
            return SVCTXR_FAIL;
        } 
        
        i4_ret = rec_util_update_event_info (pt_running_svc_req->h_rec_util,
                                             ui4_lba,
                                             at_key_info[ui4_i].e_key_type,
                                             z_event_info_len,
                                             pt_running_svc_req->pv_crnt_evn_data );
        if (i4_ret == REC_UTILR_OK)
        {
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Update info of event (key=%d) @ LBA=%d. (L%d)\n", 
                       at_key_info[ui4_i].e_key_type,
                       ui4_lba,
                       __LINE__));
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to update event info (%d). (L%d)\n", i4_ret, __LINE__));
            return SVCTXR_FAIL;
        } 
    }
    
    return SVCTXR_OK;
}

static INT32 _rec_svc_intr_check_encrypt_stream (
    REC_SVC_REQ_T*              pt_svc_req,
    MPEG_2_PID_T                t_pid_es,
    BOOL*                       pb_encrypt)
{
    UINT32              ui4_scdb_ver;
    UINT16              ui2_num_recs, ui2_i;
    STREAM_COMP_ID_T    t_strm_comp_id;
    SCDB_REC_T          t_scdb_rec = {0};
    BOOL                b_retry;
    INT32               i4_ret;
    
    /* 
       [Note] Currently we only encrypt Video & Audio streams by the same key.
              This is an implicit contract between MM_HDLR.
    */
    
    *pb_encrypt = FALSE;
    ui4_scdb_ver = SCDB_NULL_VER_ID;
    
    /* try video first */ 
    do 
    {
        b_retry = FALSE;
        
        i4_ret = x_scdb_get_num_recs (pt_svc_req->h_scdb,
                                      ST_VIDEO,
                                      & ui2_num_recs,
                                      & ui4_scdb_ver );
        if (i4_ret == SCDBR_OK)
        {
            for (ui2_i = 0; ui2_i < ui2_num_recs; ui2_i ++)
            {
                i4_ret = x_scdb_get_rec_by_idx (pt_svc_req->h_scdb,
                                                ST_VIDEO,
                                                ui2_i,
                                                & t_strm_comp_id,
                                                & t_scdb_rec,
                                                & ui4_scdb_ver );
                if (i4_ret == SCDBR_OK)
                {
                    switch (t_scdb_rec.e_rec_type)
                    {
                    case SCDB_REC_TYPE_VIDEO_MPEG:
                        if (t_scdb_rec.u.t_video_mpeg.ui2_pid == t_pid_es)
                        {
                            *pb_encrypt = TRUE;
                        }
                        break;
                        
                    default:
                        break;
                    }
                    
                    if (*pb_encrypt)
                    {
                        break;
                    }
                }
                else if (i4_ret == SCDBR_DB_MODIFIED)
                {
                    i4_ret = SVCTXR_OK;
                    b_retry = TRUE;
                    break;
                }
                else
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get video scdb rec [%d/%d] (%d). (L%d)\n", 
                                ui2_i,
                                ui2_num_recs,
                                i4_ret, 
                                __LINE__));
                    return SVCTXR_FAIL;
                }
            }
        }
        else if (i4_ret == SCDBR_DB_MODIFIED)
        {
            b_retry = TRUE;
            i4_ret = SVCTXR_OK;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get video scdb rec num (%d). (L%d)\n", i4_ret, __LINE__));
            return SVCTXR_FAIL;
        }
        
    } while (b_retry);
    
    if (*pb_encrypt)
    {
        return SVCTXR_OK;
    }
   
    /* try audio */ 
    do 
    {
        b_retry = FALSE;
        
        i4_ret = x_scdb_get_num_recs (pt_svc_req->h_scdb,
                                      ST_AUDIO,
                                      & ui2_num_recs,
                                      & ui4_scdb_ver );
        if (i4_ret == SCDBR_OK)
        {
            for (ui2_i = 0; ui2_i < ui2_num_recs; ui2_i ++)
            {
                i4_ret = x_scdb_get_rec_by_idx (pt_svc_req->h_scdb,
                                                ST_AUDIO,
                                                ui2_i,
                                                & t_strm_comp_id,
                                                & t_scdb_rec,
                                                & ui4_scdb_ver );
                if (i4_ret == SCDBR_OK)
                {
                    switch (t_scdb_rec.e_rec_type)
                    {
                    case SCDB_REC_TYPE_AUDIO_MPEG:
                        if (t_scdb_rec.u.t_audio_mpeg.ui2_pid == t_pid_es)
                        {
                            *pb_encrypt = TRUE;
                        }
                        break;
                        
                    default:
                        break;
                    }
                    
                    if (*pb_encrypt)
                    {
                        break;
                    }
                }
                else if (i4_ret == SCDBR_DB_MODIFIED)
                {
                    i4_ret = SVCTXR_OK;
                    b_retry = TRUE;
                    break;
                }
                else
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get audio scdb rec [%d/%d] (%d). (L%d)\n", 
                                ui2_i,
                                ui2_num_recs,
                                i4_ret, 
                                __LINE__));
                    return SVCTXR_FAIL;
                }
            }
        }
        else if (i4_ret == SCDBR_DB_MODIFIED)
        {
            b_retry = TRUE;
            i4_ret = SVCTXR_OK;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get audio scdb rec num (%d). (L%d)\n", i4_ret, __LINE__));
            return SVCTXR_FAIL;
        }
        
    } while (b_retry);    
    
    return SVCTXR_OK;
}
        
/*-----------------------------------------------------------------------------
                    notify functions
 ----------------------------------------------------------------------------*/
static VOID _rec_svc_cb_cm_pipe_nfy (
    HANDLE_T                    h_cm_pipe,
    CM_COND_T                   e_nfy_cond,
    VOID*                       pv_nfy_tag,
    UINT32                      ui4_data)
{
    HANDLE_T                     h_svctx = (HANDLE_T) pv_nfy_tag;
    SVCTX_MSG_T                  t_msg;
    SVCTX_REC_CM_PIPE_NFY_MSG_T* pt_msg_cm_pipe;
    INT32                        i4_ret;
    
    t_msg.h_svctx        = h_svctx;
    t_msg.ui1_msg_id     = SVCTX_MSG_REC_CM_PIPE_NFY;
    t_msg.pf_invoke_func = NULL;
    t_msg.pv_tag         = NULL;
    
    pt_msg_cm_pipe            = (SVCTX_REC_CM_PIPE_NFY_MSG_T *) t_msg.u.t_hdlr.aui1_data;
    t_msg.u.t_hdlr.z_len_data = sizeof (SVCTX_REC_CM_PIPE_NFY_MSG_T);
    
    pt_msg_cm_pipe->b_fake    = FALSE;
    pt_msg_cm_pipe->h_cm_pipe = h_cm_pipe;
    pt_msg_cm_pipe->e_cond    = e_nfy_cond;
    pt_msg_cm_pipe->ui4_data  = ui4_data;
    
    i4_ret = svctx_send_msg (h_svctx,
                             & t_msg );
                             
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send message. (L%d)\n", __LINE__));
    }
}

static VOID _rec_svc_cb_conn_nfy (
    HANDLE_T                    h_conn,
    CM_COND_T                   e_nfy_cond,
    VOID*                       pv_nfy_tag,
    UINT32                      ui4_data)
{
    HANDLE_T                    h_svctx = (HANDLE_T) pv_nfy_tag;
    SVCTX_MSG_T                 t_msg;
    SVCTX_REC_CONN_NFY_MSG_T*   pt_msg_conn;
    INT32                       i4_ret;
    
    t_msg.h_svctx           = h_svctx;
    t_msg.ui1_msg_id        = SVCTX_MSG_REC_CONN_NFY;
    t_msg.pf_invoke_func    = NULL;
    t_msg.pv_tag            = NULL;
    
    pt_msg_conn = (SVCTX_REC_CONN_NFY_MSG_T *) t_msg.u.t_hdlr.aui1_data;
    t_msg.u.t_hdlr.z_len_data = sizeof (SVCTX_REC_CONN_NFY_MSG_T);
    
    pt_msg_conn->b_fake   = FALSE;
    pt_msg_conn->h_conn   = h_conn;
    pt_msg_conn->e_cond   = e_nfy_cond;
    pt_msg_conn->ui4_data = ui4_data;
    
    i4_ret = svctx_send_msg (h_svctx,
                             & t_msg );
                             
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send message. (L%d)\n", __LINE__));
    }
}    

static VOID _rec_svc_cb_scdb_nfy (
    HANDLE_T                    h_scdb,
    SCDB_COND_T                 e_cond,
    UINT32                      ui4_reason,
    VOID*                       pv_tag,
    UINT32                      ui4_data)
{
    HANDLE_T        h_svctx = (HANDLE_T) pv_tag;
    SVCTX_MSG_T     t_msg;
    INT32           i4_ret;
    
    t_msg.h_svctx             = h_svctx;
    t_msg.ui1_msg_id          = SVCTX_MSG_REC_SCDB_NFY;
    t_msg.pf_invoke_func      = NULL;
    t_msg.pv_tag              = NULL;
    t_msg.u.t_scdb.h_obj      = h_scdb;
    t_msg.u.t_scdb.e_cond     = e_cond;
    t_msg.u.t_scdb.ui4_reason = ui4_reason;
    t_msg.u.t_scdb.ui4_data   = ui4_data;
    
    i4_ret = svctx_send_msg (h_svctx,
                             & t_msg );
                             
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send message. (L%d)\n", __LINE__));
    }
}    

static NFY_RET_T _rec_svc_cb_load_pat_nfy (
    HANDLE_T                    h_obj,
    HANDLE_TYPE_T               e_obj_type,
    TM_COND_T                   e_nfy_cond,
    VOID*                       pv_nfy_tag,
    UINT32                      ui4_data)
{
    HANDLE_T                    h_svctx = (HANDLE_T) pv_nfy_tag;
    SVCTX_MSG_T                 t_msg;
    SVCTX_REC_PAT_NFY_MSG_T*    pt_msg_pat;
    INT32                       i4_ret;
    
    t_msg.h_svctx             = h_svctx;
    t_msg.ui1_msg_id          = SVCTX_MSG_REC_PAT_NFY;
    t_msg.pf_invoke_func      = NULL;
    t_msg.pv_tag              = NULL;
    t_msg.u.t_hdlr.z_len_data = sizeof (SVCTX_REC_PAT_NFY_MSG_T);
    
    pt_msg_pat = (SVCTX_REC_PAT_NFY_MSG_T *) t_msg.u.t_hdlr.aui1_data;
    pt_msg_pat->h_pat      = h_obj;
    pt_msg_pat->e_obj_type = e_obj_type;
    pt_msg_pat->e_cond     = e_nfy_cond;
    pt_msg_pat->ui4_data   = ui4_data;

    i4_ret = svctx_send_msg (h_svctx,
                             & t_msg );
                             
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send message. (L%d)\n", __LINE__));
    }
    
    return NFY_RET_PROCESSED;
}    

static VOID _rec_svc_cb_rrctx_nfy (
    HANDLE_T                    h_rrctx,
    VOID*                       pv_nfy_tag,
    UINT32                      ui4_reason)
{
    HANDLE_T                    h_svctx = (HANDLE_T) pv_nfy_tag;
    SVCTX_MSG_T                 t_msg;
    SVCTX_REC_RRCTX_MSG_T*      pt_msg_info;
    INT32                       i4_ret;
    
    t_msg.h_svctx        = h_svctx;
    t_msg.ui1_msg_id     = SVCTX_MSG_REC_RRCTX_NFY;
    t_msg.pf_invoke_func = NULL;
    t_msg.pv_tag         = NULL;
    
    t_msg.u.t_hdlr.z_len_data = sizeof (SVCTX_REC_RRCTX_MSG_T);
    pt_msg_info = (SVCTX_REC_RRCTX_MSG_T *) t_msg.u.t_hdlr.aui1_data;
    pt_msg_info->h_rrctx    = h_rrctx;
    pt_msg_info->ui4_reason = ui4_reason;
    
    i4_ret = svctx_send_msg (h_svctx,
                             & t_msg );
                             
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send message. (L%d)\n", __LINE__));
    }
}    

static BOOL _rec_svc_cb_rec_util_nfy (
    HANDLE_T                    h_rec_util,
    VOID*                       pv_nfy_tag,
    REC_UTIL_NTFY_CODE_T        e_ntfy_code,
    UINT32                      ui4_data)
{
    HANDLE_T                    h_svctx = (HANDLE_T) pv_nfy_tag;
    SVCTX_MSG_T                 t_msg;
    SVCTX_REC_UTIL_NFY_MSG_T*   pt_msg_info;
    BOOL                        b_send_msg, b_ret;
    INT32                       i4_ret;
    
    t_msg.h_svctx           = h_svctx;
    t_msg.ui1_msg_id        = SVCTX_MSG_REC_UTIL_NFY;
    t_msg.pf_invoke_func    = NULL;
    t_msg.pv_tag            = NULL;
    
    t_msg.u.t_hdlr.z_len_data = sizeof (SVCTX_REC_UTIL_NFY_MSG_T);
    pt_msg_info = (SVCTX_REC_UTIL_NFY_MSG_T *) t_msg.u.t_hdlr.aui1_data;
    pt_msg_info->h_rec_util  = h_rec_util;
    pt_msg_info->e_ntfy_code = e_ntfy_code;
    
    b_ret      = FALSE;
    b_send_msg = FALSE;
    switch (e_ntfy_code)
    {
    case REC_UTIL_NTFY_CODE_COND_CHANGED:
        pt_msg_info->u.t_cond_chg = *((REC_UTIL_COND_CHG_NTFY_INFO_T *) ui4_data);
        b_send_msg = TRUE;
        break;
        
    case REC_UTIL_NTFY_CODE_RESET_REQ:
        b_ret      = TRUE;
        break;
        
    case REC_UTIL_NTFY_CODE_SET_CONFIG_ACK:
        pt_msg_info->u.t_set_cfg_ack = *((REC_UTIL_SET_CONFIG_ACK_INFO_T *) ui4_data);
        b_send_msg = TRUE;
        break;

    case REC_UTIL_NTFY_CODE_VALID_RANGE_UPDATED:
        pt_msg_info->u.t_valid_range = *((REC_UTIL_VALID_RANGE_NTFY_INFO_T *) ui4_data);
        b_send_msg = TRUE;
       
    default:
        b_send_msg = FALSE;
    }

    if (b_send_msg)
    {
        i4_ret = svctx_send_msg (h_svctx,
                                 & t_msg );
                                 
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send message. (L%d)\n", __LINE__));
        }
    }
    
    return b_ret;
}

static INT32 _rec_svc_cb_sess_nfy (
    HANDLE_T                    h_sess,
    SM_SESS_COND_T              e_sess_cond,
    VOID*                       pv_sess_tag,
    SM_EVN_TYPE_T               e_event,
    UINT32                      ui4_data)
{
    HANDLE_T        h_svctx = (HANDLE_T) pv_sess_tag;
    SVCTX_MSG_T     t_msg;
    INT32           i4_ret;
    
    t_msg.h_svctx           = h_svctx;
    t_msg.ui1_msg_id        = SVCTX_MSG_REC_SESS_NFY;
    t_msg.pf_invoke_func    = NULL;
    t_msg.pv_tag            = NULL;
    t_msg.u.t_sess.h_obj    = h_sess;
    t_msg.u.t_sess.e_cond   = e_sess_cond;
    t_msg.u.t_sess.e_event  = e_event;
    t_msg.u.t_sess.ui4_data = ui4_data;
    
    i4_ret = svctx_send_msg (h_svctx,
                             & t_msg );
                             
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send message. (L%d)\n", __LINE__));
    }
        
    return SMR_OK;
}    

static INT32 _rec_svc_cb_strm_nfy (
    HANDLE_T                    h_strm,
    SM_COND_T                   e_nfy_cond,
    VOID*                       pv_nfy_tag,
    SM_EVN_TYPE_T               e_event,
    UINT32                      ui4_data )
{
    HANDLE_T                h_svctx = (HANDLE_T) pv_nfy_tag;
    SVCTX_REC_STRM_MSG_T*   pt_msg_strm;
    SVCTX_MSG_T             t_msg;
    INT32                   i4_ret;
    
    t_msg.h_svctx             = h_svctx;
    t_msg.ui1_msg_id          = SVCTX_MSG_REC_STRM_NFY;
    t_msg.pf_invoke_func      = NULL;
    t_msg.pv_tag              = NULL;
    t_msg.u.t_hdlr.z_len_data = sizeof (SVCTX_REC_STRM_MSG_T);
    
    pt_msg_strm = (SVCTX_REC_STRM_MSG_T *) t_msg.u.t_hdlr.aui1_data;
    
    pt_msg_strm->h_strm     = h_strm;
    pt_msg_strm->e_cond     = e_nfy_cond;
    pt_msg_strm->e_event    = e_event;
    
    switch (e_event)
    {
    case SM_EVN_CA_COND_CLOSED:
    case SM_EVN_CA_COND_UNKNOWN:
    case SM_EVN_CA_COND_FREE:
    case SM_EVN_CA_COND_AUTHORIZED:
    case SM_EVN_CA_COND_AUTHORIZED_CA_SYS_ID:
    case SM_EVN_CA_COND_NOT_AUTHORIZED:
        pt_msg_strm->u.ui4_ca_detail = ui4_data;
        break;
        
    case SM_EVN_CCI_INFORMATION:
        pt_msg_strm->u.t_cp_info = *((CP_INFO_LIST_T *) ui4_data);
        break;
        
    default:
        pt_msg_strm->u.ui4_data = ui4_data;
    }
    
    i4_ret = svctx_send_msg (h_svctx,
                             & t_msg );
                             
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send message. (L%d)\n", __LINE__));
    }

    return SMR_OK;
}    

static VOID _rec_svc_cb_timer_watchdog_nfy (
    HANDLE_T                    h_timer_wd,
    VOID*                       pv_tag)
{
    HANDLE_T        h_svctx = (HANDLE_T) pv_tag;
    SVCTX_MSG_T     t_msg;
    INT32           i4_ret;
    
    t_msg.h_svctx           = h_svctx;
    t_msg.ui1_msg_id        = SVCTX_MSG_REC_WATCHDOG_TIMER_NFY;
    t_msg.pf_invoke_func    = NULL;
    t_msg.pv_tag            = NULL;
    
    i4_ret = svctx_send_msg (h_svctx,
                             & t_msg );
                             
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send message. (L%d)\n", __LINE__));
    }
}

static VOID _rec_svc_cb_timer_reconn_nfy (
    HANDLE_T                    h_timer_conn,
    VOID*                       pv_tag)
{
    HANDLE_T        h_svctx = (HANDLE_T) pv_tag;
    SVCTX_MSG_T     t_msg;
    INT32           i4_ret;

    t_msg.h_svctx           = h_svctx;
    t_msg.ui1_msg_id        = SVCTX_MSG_REC_RECONN_TIMER_NFY;
    t_msg.pf_invoke_func    = NULL;
    t_msg.pv_tag            = NULL;
    
    i4_ret = svctx_send_msg (h_svctx,
                             & t_msg );
                             
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send message. (L%d)\n", __LINE__));
    }
}

static VOID _rec_svc_cb_conning_timeout_nfy (
    HANDLE_T                    h_timer_conn,
    VOID*                       pv_tag)
{
    HANDLE_T        h_svctx = (HANDLE_T) pv_tag;
    SVCTX_MSG_T     t_msg;
    INT32           i4_ret;
    
    t_msg.h_svctx        = h_svctx;
    t_msg.ui1_msg_id     = SVCTX_MSG_REC_CONNING_TIMEOUT_NFY;
    t_msg.pf_invoke_func = NULL;
    t_msg.pv_tag         = NULL;

    i4_ret = svctx_send_msg (h_svctx,
                             & t_msg );
                             
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send message. (L%d)\n", __LINE__));
    }
}



/*-----------------------------------------------------------------------------
                    stream list functions
 ----------------------------------------------------------------------------*/
static INT32 _rec_svc_slst_add_strm (
    REC_SVC_REQ_T*              pt_svc_req,
    const REC_SVC_STRM_INFO_T*  pt_strm_new )
{
    REC_SVC_STRM_INFO_T*    pt_strm_alloc;
    
    if (DLIST_IS_EMPTY (& pt_svc_req->t_free_strms))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to add stream since already too many streams. (L%d)\n", __LINE__));
        return SVCTXR_NO_RESOURCE;
    }
    
    pt_strm_alloc = DLIST_HEAD (& pt_svc_req->t_free_strms);

    DLIST_REMOVE (pt_strm_alloc,
                  & pt_svc_req->t_free_strms,
                  t_dlink);
    
    *pt_strm_alloc = *pt_strm_new;
    
    DLIST_INSERT_TAIL (pt_strm_alloc,
                       & pt_svc_req->t_wking_strms,
                       t_dlink );
    
    return SVCTXR_OK;
}

static INT32 _rec_svc_slst_remove_strm (
    REC_SVC_REQ_T*              pt_svc_req,
    HANDLE_T                    h_strm )
{
    REC_SVC_STRM_INFO_T*    pt_strm;
    BOOL                    b_found = FALSE;
    INT32                   i4_ret;
    
    DLIST_FOR_EACH (pt_strm, & pt_svc_req->t_wking_strms, t_dlink)
    {
        if (pt_strm->h_strm == h_strm)
        {
            b_found = TRUE;
            break;
        }
    }
    
    if (b_found)
    {
        DLIST_REMOVE (pt_strm,
                      & pt_svc_req->t_wking_strms,
                      t_dlink);
        
        DLIST_INSERT_TAIL (pt_strm,
                           & pt_svc_req->t_free_strms,
                           t_dlink);
                           
        i4_ret = SVCTXR_OK;
    }
    else
    {
        i4_ret = SVCTXR_INV_ARG;
    } 
    
    return i4_ret;
}

static REC_SVC_STRM_INFO_T* _rec_svc_slst_get_actv_strm_by_type (
    const REC_SVC_REQ_T*        pt_svc_req,
    REC_SVC_STRM_TYPE_T         e_strm_type )
{
    REC_SVC_STRM_INFO_T*        pt_strm;
    REC_SVC_STRM_INFO_T*        pt_strm_found;
    
    pt_strm_found = NULL;
    
    DLIST_FOR_EACH (pt_strm, &pt_svc_req->t_wking_strms, t_dlink)
    {
        if (pt_strm->e_type == e_strm_type &&
            REC_SVC_STRM_GET_STATE (pt_strm) == REC_SVC_STRM_FLAG_STATE_ACTIVE)
        {
            pt_strm_found = pt_strm;
            break;
        }
    }        
    
    return pt_strm_found;
}    

static REC_SVC_STRM_INFO_T* _rec_svc_slst_get_strm_by_handle (
    const REC_SVC_REQ_T*        pt_svc_req,
    HANDLE_T                    h_strm)
{
    REC_SVC_STRM_INFO_T*        pt_strm;
    REC_SVC_STRM_INFO_T*        pt_strm_found;
    
    pt_strm_found = NULL;
    
    DLIST_FOR_EACH (pt_strm, &pt_svc_req->t_wking_strms, t_dlink)
    {
        if (pt_strm->h_strm == h_strm)
        {
            pt_strm_found = pt_strm;
            break;
        }
    }        
    
    return pt_strm_found;
}

static REC_SVC_STRM_INFO_T* _rec_svc_slst_get_es_strm_by_pid (
    const REC_SVC_REQ_T*        pt_svc_req,
    MPEG_2_PID_T                t_pid)
{
    REC_SVC_STRM_INFO_T*        pt_strm;
    REC_SVC_STRM_INFO_T*        pt_strm_found;
    
    pt_strm_found = NULL;
    
    DLIST_FOR_EACH (pt_strm, &pt_svc_req->t_wking_strms, t_dlink)
    {
        if (pt_strm->e_type == REC_SVC_STRM_TYPE_ES)
        {
            if (pt_strm->u.t_es.t_pid == t_pid)
            {
                pt_strm_found = pt_strm;
                break;
            }
        }
    }
    
    return pt_strm_found;
}

static BOOL _rec_svc_slst_check_all_opened (
    const REC_SVC_REQ_T*        pt_svc_req)
{
    BOOL                        b_ret;
    REC_SVC_STRM_INFO_T*        pt_strm;
    
    b_ret = TRUE;
    
    DLIST_FOR_EACH (pt_strm, &pt_svc_req->t_wking_strms, t_dlink)
    {
        if (pt_strm->e_cond_strm != SM_COND_OPENED)
        {
            b_ret = FALSE;
            break;
        }
    }        
    
    return b_ret;
}


/*-----------------------------------------------------------------------------
                    utility functions
 ----------------------------------------------------------------------------*/

static INT32 _rec_svc_tool_get_rec_cfg_fld_sizes (
    const SVCTX_REC_CONFIG_T*   pt_rec_cfg,
    SIZE_T*                     pz_bound_info,
    SIZE_T*                     pz_db_name,
    SIZE_T*                     pz_ts_path,
    SIZE_T*                     pz_cont_info,
    SIZE_T*                     pz_total)
{
    switch (pt_rec_cfg->e_bound_type)
    {
    case SVCTX_REC_BOUND_TYPE_TIME:
        *pz_bound_info = ALIGN_32 (sizeof (SVCTX_REC_TIME_BOUND_INFO_T));
        break;
    
    case SVCTX_REC_BOUND_TYPE_SPACE:
        *pz_bound_info = ALIGN_32 (sizeof (SVCTX_REC_SPACE_BOUND_INFO_T));
        break;
    
    default:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    switch (pt_rec_cfg->e_cont_opt)
    {
    case SVCTX_REC_CONT_OPT_NORMAL:
        *pz_cont_info = ALIGN_32 (sizeof (SVCTX_REC_CONT_INFO_NORMAL_T));
        break;
        
    case SVCTX_REC_CONT_OPT_CONT:
        *pz_cont_info = ALIGN_32 (sizeof (SVCTX_REC_CONT_INFO_CONT_T));
        break;
        
    default:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    *pz_db_name = ALIGN_32 (x_strlen (pt_rec_cfg->s_db_name) + 1);
    *pz_ts_path = ALIGN_32 (x_strlen (pt_rec_cfg->t_strg_desc.s_strg_path) + 1);
    
    *pz_total = ALIGN_32 (sizeof (SVCTX_REC_CONFIG_T)) + *pz_bound_info + 
                   *pz_db_name + *pz_ts_path + *pz_cont_info ; 
        
    return SVCTXR_OK;
}

static INT32 _rec_svc_tool_calc_rec_cfg_size (
    const SVCTX_REC_CONFIG_T*   pt_rec_cfg,
    SIZE_T*                     pz_total )
{
    SIZE_T  z_bound_info, z_ts_path, z_db_name, z_cont_info;
    INT32   i4_ret;
    
    i4_ret = _rec_svc_tool_get_rec_cfg_fld_sizes (pt_rec_cfg,
                                                  & z_bound_info,
                                                  & z_db_name,
                                                  & z_ts_path,
                                                  & z_cont_info,
                                                  pz_total );
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to calculate size (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    return i4_ret;
}
    
static INT32 _rec_svc_tool_copy_rec_cfg (
    VOID*                       pv_buf_to,
    SIZE_T                      z_len_buf_to,
    const SVCTX_REC_CONFIG_T*   pt_rec_cfg_from)
{
    SVCTX_REC_CONFIG_T*  pt_rec_cfg_to = (SVCTX_REC_CONFIG_T *) pv_buf_to;
    SIZE_T               z_bound_info, z_ts_path, z_db_name, z_cont_info, z_total;
    UINT8*               pui1_ptr;
    INT32                i4_ret;
    
    i4_ret = _rec_svc_tool_get_rec_cfg_fld_sizes (pt_rec_cfg_from,
                                                  & z_bound_info,
                                                  & z_db_name,
                                                  & z_ts_path,
                                                  & z_cont_info,
                                                  & z_total );
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to calculate size (%d). (L%d)\n", i4_ret, __LINE__));
        return i4_ret;
    }

    if (z_total > z_len_buf_to)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Not enough buffer size. (L%d)\n", __LINE__));
        return SVCTXR_NO_RESOURCE;
    }
    
    pui1_ptr = ((UINT8 *) pv_buf_to) + ALIGN_32 (sizeof (SVCTX_REC_CONFIG_T));
    x_memcpy (pt_rec_cfg_to, pt_rec_cfg_from, sizeof (SVCTX_REC_CONFIG_T));
    pt_rec_cfg_to->pv_bound_info = (VOID *) pui1_ptr;
    pui1_ptr += z_bound_info;
    pt_rec_cfg_to->s_db_name = (CHAR *) pui1_ptr;
    pui1_ptr += z_db_name;
    pt_rec_cfg_to->t_strg_desc.s_strg_path = (CHAR *) pui1_ptr;
    pui1_ptr += z_ts_path;
    pt_rec_cfg_to->pv_cont_info = (VOID *) pui1_ptr;
    pui1_ptr += z_cont_info;
    
    switch (pt_rec_cfg_from->e_bound_type)
    {
    case SVCTX_REC_BOUND_TYPE_TIME:
        x_memcpy (pt_rec_cfg_to->pv_bound_info, 
                  pt_rec_cfg_from->pv_bound_info,
                  sizeof (SVCTX_REC_TIME_BOUND_INFO_T) );
        break;
    
    case SVCTX_REC_BOUND_TYPE_SPACE:
        x_memcpy (pt_rec_cfg_to->pv_bound_info, 
                  pt_rec_cfg_from->pv_bound_info,
                  sizeof (SVCTX_REC_SPACE_BOUND_INFO_T) );
        break;
    
    default:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    switch (pt_rec_cfg_from->e_cont_opt)
    {
    case SVCTX_REC_CONT_OPT_NORMAL:
        x_memcpy (pt_rec_cfg_to->pv_cont_info, 
                  pt_rec_cfg_from->pv_cont_info,
                  sizeof (SVCTX_REC_CONT_INFO_NORMAL_T) );
        break;
        
    case SVCTX_REC_CONT_OPT_CONT:
        x_memcpy (pt_rec_cfg_to->pv_cont_info, 
                  pt_rec_cfg_from->pv_cont_info,
                  sizeof (SVCTX_REC_CONT_INFO_CONT_T) );
        break;
        
    default:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    x_strcpy ((CHAR *) pt_rec_cfg_to->s_db_name,
              (CHAR *) pt_rec_cfg_from->s_db_name);

    x_strcpy ((CHAR *) pt_rec_cfg_to->t_strg_desc.s_strg_path,
              (CHAR *) pt_rec_cfg_from->t_strg_desc.s_strg_path);
    
    return SVCTXR_OK;
}

static VOID *_rec_svc_util_mem_alloc (
    REC_SVC_REQ_T*              pt_svc_req,
    SIZE_T                      z_size )
{
    VOID*   pv_mem;
    
    pv_mem = NULL;
    
    if (DLIST_IS_EMPTY (& pt_svc_req->t_free_blks))
    {
        pv_mem = x_mem_alloc (z_size);
        
        if (pv_mem == NULL)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Out of memory. (L%d)\n", __LINE__));
        }
    }
    else if (z_size <= REC_SVC_MAX_MEM_BLK_SIZE)
    {
        REC_SVC_MEM_BLK_T*    pt_mem_new;
        
        pt_mem_new = DLIST_HEAD (& pt_svc_req->t_free_blks);
        
        DLIST_REMOVE (pt_mem_new, 
                      & pt_svc_req->t_free_blks, 
                      t_dlink);
        
        DLIST_INSERT_TAIL (pt_mem_new, 
                           & pt_svc_req->t_alloc_blks, 
                           t_dlink);
        
        pv_mem = (VOID *) pt_mem_new->aui1_buf;
    }
    else
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Memory block size not enough (alloc:%d, max:%d). (L%d)\n", 
                    z_size, 
                    REC_SVC_MAX_MEM_BLK_SIZE,
                    __LINE__));
        
        pv_mem = x_mem_alloc (z_size);
        
        if (pv_mem == NULL)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Out of memory. (L%d)\n", __LINE__));
        }
    }
    
    return pv_mem;
}      

static VOID _rec_svc_util_mem_free (
    REC_SVC_REQ_T*              pt_svc_req,
    VOID*                       pv_mem)
{
    REC_SVC_MEM_BLK_T*          pt_mem;
    BOOL                        b_found;
    
    b_found = FALSE;
    
    DLIST_FOR_EACH (pt_mem, & pt_svc_req->t_alloc_blks, t_dlink)
    {
        if (pv_mem == (VOID *) pt_mem->aui1_buf)
        {
            b_found = TRUE;
            break;
        }
    }
    
    if (b_found)
    {
        DLIST_REMOVE (pt_mem, 
                      & pt_svc_req->t_alloc_blks, 
                      t_dlink);
        
        DLIST_INSERT_TAIL (pt_mem, 
                           & pt_svc_req->t_free_blks, 
                           t_dlink);
    }    
    else
    {
        x_mem_free (pv_mem);
    }   
}
    
static INT32 _rec_svc_util_get_src_brdcst_svl (
    REC_SVC_REQ_T*              pt_svc_req,
    REC_SVC_SRC_BRDCST_SVL_T**  ppt_src_brdcst_svl)
{
    INT32       i4_ret;
    
    *ppt_src_brdcst_svl = NULL;
    i4_ret = SVCTXR_OK;
    
    switch (pt_svc_req->e_src_rec)
    {
    case SRC_TYPE_BRDCST_SVL:
        *ppt_src_brdcst_svl = & pt_svc_req->u_src.t_svl;
        break;
        
    case SRC_TYPE_BRDCST_FREQ:
    case SRC_TYPE_AVC:
    default:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid source type (%d). (L%d)\n", pt_svc_req->e_src_rec, __LINE__));
        i4_ret = SVCTXR_REC_INTERNAL_ERROR;
    }
    
    return i4_ret;
}        

static INT32 _rec_svc_util_get_res_brdcst_svl (
    REC_SVC_REQ_T*              pt_svc_req,
    REC_SVC_RES_BRDCST_SVL_T**  ppt_res_brdcst_svl)
{
    INT32   i4_ret;
    
    *ppt_res_brdcst_svl = NULL;
    i4_ret = SVCTXR_OK;
    
    switch (pt_svc_req->e_src_rec)
    {
    case SRC_TYPE_BRDCST_SVL:
        *ppt_res_brdcst_svl = & pt_svc_req->u_res.t_svl;
        break;
    
    case SRC_TYPE_BRDCST_FREQ:
    case SRC_TYPE_AVC:
    default:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid source type (%d). (L%d)\n", pt_svc_req->e_src_rec, __LINE__));
        i4_ret = SVCTXR_REC_INTERNAL_ERROR;
    }
    
    return i4_ret;
}

#ifdef MW_TV_AV_BYPASS_SUPPORT
static PLAYER_TYPE_T _rec_svc_state_util_get_player_type (
    const REC_SVC_REQ_T*    pt_svc_req)
{
    PLAYER_TYPE_T   e_player_type = PLAYER_TYPE_CH_SVC;
    
    switch (pt_svc_req->e_src_rec)
    {
    case SRC_TYPE_BRDCST_SVL:
        e_player_type = PLAYER_TYPE_CH_SVC;
        break;
    
    default:
        DBG_ABORT (DBG_MOD_SVCTX);
    }
    
    return e_player_type;
}
#endif
    
static VOID _rec_svc_util_send_cm_disconned_to_lead_svctx (
    SVCTX_T*                    pt_svctx,
    BOOL                        b_signal_loss)
{
    
    #ifdef MW_TV_AV_BYPASS_SUPPORT
    {
        if (IS_SVCTX_ASSOCIATE(pt_svctx))
        {
            SVCTX_T*        pt_lead;
            REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
            INT32           i4_ret;
            
            pt_lead = svctx_mgr_get_link_lead (ACTIVE_LINK_NOW, 
                                               pt_running_svc_req->e_src_rec);
            
            if (pt_lead                &&
                !pt_svctx->b_fake_disc && 
                pt_lead->i1_prio_diff > pt_svctx->i1_prio_diff)
            {
                if (pt_lead->pt_running_svc_req          &&
                    pt_lead->pt_running_svc_req->pt_hdlr &&
                    pt_lead->pt_running_svc_req->pt_hdlr->t_fcts.pf_set)
                {
                    i4_ret = pt_lead->pt_running_svc_req->pt_hdlr->t_fcts.pf_set (
                                pt_lead,
                                SVCTX_INTL_COMMON_SET_TYPE_FAKE_CM_DISCTED,
                                (VOID *) & b_signal_loss,
                                sizeof (BOOL) );
                    
                    if (RET_FAIL (i4_ret))
                    {
                        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to info fake cm disconnected (%d). (L%d)\n", i4_ret, __LINE__));
                    }
                }
            }
        }        
    }   
    #endif 
    
}

static INT32 _rec_svc_state_util_check_block (
    SVCTX_T*                    pt_svctx,
    BOOL*                       pb_blocked)
{
    REC_SVC_REQ_T*           pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;    
    SVC_BLOCK_COND_T         e_blk_cond;
    SVC_BLOCK_CHECK_GUDIE_T  e_chk_guide;
    SVL_REC_T                t_svl_rec = {0};
    INT32                    i4_ret;
    
    *pb_blocked = FALSE;
    
    if (pt_svctx->pf_block_check_fct == NULL)
    {
        *pb_blocked = FALSE;
        return SVCTXR_OK;
    }
    
    i4_ret = rec_svc_req_get_svl_rec (pt_running_svc_req,
                                      & t_svl_rec);
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get svl rec (%d). (L%d)\n", i4_ret, __LINE__));
        x_memset (&t_svl_rec, 0, sizeof (SVL_REC_T));
    }                                      
    
    /* check guide */
    e_chk_guide = SVC_BLOCK_CHECK_ALL;
    
    if (!(crnt_evn_is_ready (pt_running_svc_req->pt_crnt_evn)))
    {
        e_chk_guide &= (~SVC_BLOCK_CHECK_RATING);
    }
    
    /* check block */
    e_blk_cond = pt_svctx->pf_block_check_fct (
            e_chk_guide,
            pt_running_svc_req->s_src_grp_name,
            & t_svl_rec,
            crnt_evn_get_rating (pt_running_svc_req->pt_crnt_evn)
#ifdef SVCTX_SPLIT_SUPPORT
            ,
            pt_svctx->pf_block_check_fct_tag
#endif            
            );
    
    switch (e_blk_cond)
    {
    case SVC_BLOCK_COND_EXCEED_RATING:
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Service blocked: SVC_BLOCK_COND_EXCEED_RATING. (L%d)\n", __LINE__));
        *pb_blocked = TRUE;
        break;
    case SVC_BLOCK_COND_USER_BLOCK_CH:
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Service blocked: SVC_BLOCK_COND_USER_BLOCK_CH. (L%d)\n", __LINE__));
        *pb_blocked = TRUE;
        break;
    case SVC_BLOCK_COND_USER_BLOCK_INP:
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Service blocked: SVC_BLOCK_COND_USER_BLOCK_INP. (L%d)\n", __LINE__));
        *pb_blocked = TRUE;
        break;
        
    case SVC_BLOCK_COND_PASSED:
    case SVC_BLOCK_COND_PASSED_AND_NO_RATING_SET:
        *pb_blocked = FALSE;
        break;
    
    default:
        DBG_ABORT (DBG_MOD_SVCTX);
    }
    
    return SVCTXR_OK;
}

static INT32 _rec_svc_util_pvr_rfi_write_bef_rec (
    REC_SVC_REQ_T*              pt_running_svc_req)
{
    TIME_T                t_utc_start_time;
    INT32                 i4_ret;
    
    if (pt_running_svc_req->e_media_type != REC_SVC_MEDIA_TYPE_PVR       ||
        !REC_SVC_STATE_IN_ANY_SESS_STATES (pt_running_svc_req->e_state ) ||
        pt_running_svc_req->e_cond_rec_util != REC_UTIL_COND_READY)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid condition to write RFI. (L%d)\n", __LINE__));
        return SVCTXR_WRONG_STATE;
    }
    
    switch (pt_running_svc_req->e_src_rec)
    {
    case SRC_TYPE_BRDCST_SVL:
    {
        SVL_REC_T   t_svl_rec;
        
        /* SVCTX_REC_META_KEY_TYPE_CH_ID */
        i4_ret = rec_util_rfi_set_info (pt_running_svc_req->h_rec_util,
                                        (UINT32) SVCTX_REC_META_KEY_TYPE_CH_ID,
                                        (VOID *) &pt_running_svc_req->u_src.t_svl.t_param.ui4_channel_id,
                                        sizeof (UINT32));
        if (i4_ret != REC_UTILR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to set CH_ID to RFI (%d). (L%d)\n", i4_ret, __LINE__));
        }
    
        /* SVCTX_REC_META_KEY_TYPE_CH_NAME */
        i4_ret = rec_svc_req_get_svl_rec (pt_running_svc_req,
                                          & t_svl_rec);
        if (RET_OK (i4_ret))
        {
            i4_ret = rec_util_rfi_set_info (pt_running_svc_req->h_rec_util,
                                            (UINT32) SVCTX_REC_META_KEY_TYPE_CH_NAME,
                                            (VOID *) t_svl_rec.uheader.t_rec_hdr.ac_name,
                                            x_strlen (t_svl_rec.uheader.t_rec_hdr.ac_name) + 1);
            if (i4_ret != REC_UTILR_OK)
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to set CH_NAME to RFI (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }    
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get svl rec (%d). (L%d)\n", i4_ret, __LINE__));
        }
                                        
        break;
    }    
    case SRC_TYPE_BRDCST_FREQ:
    case SRC_TYPE_AVC:
    default:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid source type (%d). (L%d)\n", pt_running_svc_req->e_src_rec, __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    /* SVCTX_REC_META_KEY_TYPE_START_TIME */
    t_utc_start_time = x_dt_get_utc (NULL, NULL);
    i4_ret = rec_util_rfi_set_info (pt_running_svc_req->h_rec_util,
                                    (UINT32) SVCTX_REC_META_KEY_TYPE_START_TIME,
                                    (VOID *) &t_utc_start_time,
                                    sizeof (TIME_T));
    if (i4_ret != REC_UTILR_OK)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to set START_TIME to RFI (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    return SVCTXR_OK;
}

static INT32 _rec_svc_util_pvr_rfi_write_aft_rec (
    REC_SVC_REQ_T*              pt_running_svc_req)
{
    UINT32                ui4_rec_tks;
    INT32                 i4_ret;
    
    if (pt_running_svc_req->e_media_type != REC_SVC_MEDIA_TYPE_PVR       ||
        !REC_SVC_STATE_IN_ANY_SESS_STATES (pt_running_svc_req->e_state ) ||
        pt_running_svc_req->e_cond_rec_util != REC_UTIL_COND_READY)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid condition to write RFI. (L%d)\n", __LINE__));
        return SVCTXR_WRONG_STATE;
    }
    
    /* SVCTX_REC_META_KEY_TYPE_DURATION */
    i4_ret = rec_util_get_tick_count (pt_running_svc_req->h_rec_util,
                                      & ui4_rec_tks);
    if (i4_ret == REC_UTILR_OK)
    {
        UINT32   ui4_sec_dur;
        
        ui4_sec_dur = (UINT32) ((((UINT64) ui4_rec_tks) * 
                            ((UINT64) pt_running_svc_req->pt_rec_cfg->ui4_ms_tick_period)) / 1000);
        
        i4_ret = rec_util_rfi_set_info (pt_running_svc_req->h_rec_util,
                                        (UINT32) SVCTX_REC_META_KEY_TYPE_DURATION,
                                        (VOID *) &ui4_sec_dur,
                                        sizeof (UINT32));
        if (i4_ret != REC_UTILR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to set DURATION to RFI (%d). (L%d)\n", i4_ret, __LINE__));
        }    
    }
    else
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get recorded ticks (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    return SVCTXR_OK;
}    
    
static INT32 _rec_svc_util_pvr_rfi_write_fst_evn_ttl (
    REC_SVC_REQ_T*              pt_running_svc_req)
{
    REC_SVC_MEDIA_PVR_T*  pt_media_pvr;
    HANDLE_T              h_event;
    SIZE_T                z_ttl_len = 0, z_get_info;
#ifndef MW_EDB_SUPPORT
    EVCTX_COND_T          e_evn_cond;
#else
    EDB_CH_EVT_ID_T       t_ch_evt_id;
#endif
    REC_SVC_RES_BRDCST_SVL_T*        pt_res_brdcst_svl;
    INT32                            i4_ret;
    
    if (pt_running_svc_req->e_media_type != REC_SVC_MEDIA_TYPE_PVR       ||
        !REC_SVC_STATE_IN_ANY_SESS_STATES (pt_running_svc_req->e_state ) ||
        pt_running_svc_req->e_cond_rec_util != REC_UTIL_COND_READY )
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid state to write FST_EVN_TTL. (L%d)\n", __LINE__));
        return SVCTXR_WRONG_STATE;
    }
    
    pt_media_pvr = & pt_running_svc_req->t_pvr;
    
    if (
#ifndef MW_EDB_SUPPORT
        !REC_SVC_EVCTX_INFO_AVAIL (crnt_evn_get_cond (pt_running_svc_req->pt_crnt_evn))
#else
        crnt_evn_get_cond (pt_running_svc_req->pt_crnt_evn) != EDB_COND_STARTED
#endif
        )
    {
        return SVCTXR_OK;
    }
    
    i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req, 
                                               & pt_res_brdcst_svl );
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get RES_BRDCST_SVL (%d). (L%d)\n", i4_ret, __LINE__));
        pt_res_brdcst_svl = NULL;
    }
#ifdef MW_EDB_SUPPORT
    /* Key info prepare */
    t_ch_evt_id.e_brdcst_type     = pt_res_brdcst_svl ? pt_res_brdcst_svl->e_brdcst_type : BRDCST_TYPE_UNKNOWN;
    t_ch_evt_id.ui2_svl_id        = pt_running_svc_req->u_src.t_svl.t_param.ui2_svc_lst_id;
    t_ch_evt_id.ui4_channel_id    = pt_running_svc_req->u_src.t_svl.t_param.ui4_channel_id;
    t_ch_evt_id.ui2_event_id      = 0;
    t_ch_evt_id.b_auto_event_id   = TRUE;
#endif
    if (pt_media_pvr->b_got_fst_evn_ttl)
    {
        return SVCTXR_OK;
    }
    
    h_event = crnt_evn_get_handle (pt_running_svc_req->pt_crnt_evn);
#ifndef MW_EDB_SUPPORT    
    i4_ret = x_evctx_get_event_info_len (h_event,
                                         EVCTX_KEY_TYPE_EVENT_TITLE,
                                         (VOID *) pt_running_svc_req->pt_rec_cfg->s_lang,
                                         & z_ttl_len,
                                         & e_evn_cond );
#else
    i4_ret = x_edb_get_info_len(h_event,
    							EDB_KEY_TYPE_EVENT_TITLE_BY_ID,
    							&t_ch_evt_id, 
    							& z_ttl_len);
#endif
    if (i4_ret != EVCTXR_OK)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get EVN_TTL len (%d). (L%d)\n", i4_ret, __LINE__));
        return SVCTXR_REC_EXTNL_MODULE_FAIL;
    }
    
    if (pt_media_pvr->z_buf_fst_evn_ttl < z_ttl_len)
    {
        SIZE_T  z_new_sz;
        
        z_new_sz = z_ttl_len * 2;
        
        if (pt_media_pvr->pv_fst_evn_ttl)
        {
            x_mem_free (pt_media_pvr->pv_fst_evn_ttl);
            pt_media_pvr->pv_fst_evn_ttl = NULL;
            pt_media_pvr->z_buf_fst_evn_ttl = 0;
        }
        
        pt_media_pvr->pv_fst_evn_ttl = x_mem_alloc (z_new_sz);
        if (pt_media_pvr->pv_fst_evn_ttl == NULL)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to allocate memory. (L%d)\n", __LINE__));
            return SVCTXR_NO_RESOURCE;
        }
        pt_media_pvr->z_buf_fst_evn_ttl = z_new_sz;
    }
    
    z_get_info = pt_media_pvr->z_buf_fst_evn_ttl;
    
#ifndef MW_EDB_SUPPORT        
    i4_ret = x_evctx_get_event_info (h_event,
                                     EVCTX_KEY_TYPE_EVENT_TITLE,
                                     (VOID *) pt_running_svc_req->pt_rec_cfg->s_lang,
                                     &z_get_info,
                                     pt_media_pvr->pv_fst_evn_ttl,
                                     & e_evn_cond);
    if (i4_ret != EVCTXR_OK) {
#else
    i4_ret = x_edb_get_info(h_event,
                            EDB_KEY_TYPE_EVENT_TITLE_BY_ID,
                            &t_ch_evt_id, 
                            & z_get_info,
                            pt_media_pvr->pv_fst_evn_ttl);
    if (i4_ret != EDBR_OK) {
#endif
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get EVN_TTL (%d). (L%d)\n", i4_ret, __LINE__));
        return SVCTXR_REC_EXTNL_MODULE_FAIL;
    }
    pt_media_pvr->z_fst_evn_ttl = z_get_info;
    pt_media_pvr->b_got_fst_evn_ttl = TRUE;
    
    i4_ret = rec_util_rfi_set_info (pt_running_svc_req->h_rec_util,
                                    (UINT32) SVCTX_REC_META_KEY_TYPE_FST_EVN_TITLE,
                                    pt_media_pvr->pv_fst_evn_ttl,
                                    pt_media_pvr->z_fst_evn_ttl + 1);
    if (i4_ret != REC_UTILR_OK)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to set FST_ENV_TTL to RFI (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    return SVCTXR_OK;
}    
        
/* [TODO] >// RRT */
#if 0

static INT32 _rec_svc_util_get_rrctx_data (
    const REC_SVC_REQ_T*        pt_running_svc_req,
    VOID**                      ppv_data,
    SIZE_T*                     pz_len_data)
{
    RRCTX_RRT_KEY_PARAM_T   t_rrt_key_param;
    SIZE_T                  z_get_info_len, z_rrt_obj_size;
    INT32                   i4_ret;
    
    *ppv_data    = NULL;
    *pz_len_data = 0;
    
    if (pt_running_svc_req->h_rrctx == NULL_HANDLE)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong context to get rrctx data (%d). (L%d)\n", i4_ret, __LINE__));
        return SVCTXR_NOT_SUPPORT;
    }
    
    /* get rrt size */
    t_rrt_key_param.e_rrt_key_type   = RRCTX_RRT_KEY_REC_TBL_SIZE;
    t_rrt_key_param.pv_rrt_key_param = NULL;
    
    z_get_info_len = sizeof (SIZE_T);
    i4_ret = x_rrctx_get_rating_info (pt_running_svc_req->h_rrctx,
                                      RRCTX_KEY_TYPE_ENG_SPECIFIC,
                                      
                                      (VOID *) & t_rrt_key_param,
                                      & z_get_info_len,
                                      (VOID *) & z_rrt_obj_size)
    
    if (i4_ret != RRCTXR_OK)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get rrt obj size (%d). (L%d)\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    /* report rrt size info */
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] RRT Object Size = %d. (L%d)\n", z_rrt_obj_size, __LINE__));
    
    if (z_rrt_obj_size > 0)
    {
        RRCTX_RRT_REC_TBL_OBJ_T     t_rrt_rec_tbl;
        
        /* check working buffer size */
        if (pt_running_svc_req->z_len_buf_rrctx < z_rrt_obj_size)
        {
            if (pt_running_svc_req->pv_buf_rrctx)
            {
                x_mem_free (pt_running_svc_req->pv_buf_rrctx);
                pt_running_svc_req->pv_buf_rrctx = NULL;
                pt_running_svc_req->z_len_buf_rrctx = 0;
            }
            
            pt_running_svc_req->pv_buf_rrctx = x_mem_alloc (pt_running_svc_req->z_len_buf_rrctx);
            
            if (pt_running_svc_req->pv_buf_rrctx == NULL)
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Out of memory. (L%d)\n", __LINE__));
                return RRCTXR_OUT_OF_MEM;
            }

            pt_running_svc_req->z_len_buf_rrctx = z_rrt_obj_size;
        }
        
        t_rrt_key_param.e_rrt_key_type   = RRCTX_RRT_KEY_REC_TBL_GET
        t_rrt_key_param.pv_rrt_key_param = NULL;
        
        t_rrt_rec_tbl.pv_buf = pt_running_svc_req->pv_buf_rrctx;
        t_rrt_rec_tbl.z_size = pt_running_svc_req->z_len_buf_rrctx;
        
        /* get rrt object */
        z_get_info_len = sizeof (RRCTX_RRT_REC_TBL_OBJ_T);
        
        i4_ret = x_rrctx_get_rating_info (pt_running_svc_req->h_rrctx,
                                          RRCTX_KEY_TYPE_ENG_SPECIFIC,
                                          (VOID *) & t_rrt_key_param,
                                          & z_get_info_len,
                                          (VOID *) & t_rrt_rec_tbl);
        if (i4_ret != RRCTXR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get rrt obj size (%d). (L%d)\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
    else
    {
        *ppv_data = NULL;
        *pz_len_data = 0;
    }
    
    return SVCTXR_OK;
}

static INT32 _rec_svc_util_update_rrctx_data (
    REC_SVC_REQ_T*              pt_running_svc_req,
    VOID*                       pv_data,
    SIZE_T                      z_len_data)
{
    PVR_BUFFER_STATUS_T     t_pvr_buf_st;
    SIZE_T                  z_get_info_len;
    UINT32                  ui4_lba;
    INT32                   i4_ret;
    
    if (pt_running_svc_req->e_cond_rec_util != REC_UTIL_COND_READY ||
        pt_running_svc_req->e_state != REC_SVC_STATE_SESS_STARTED)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong state (%d) to record rrctx data. (L%d)\n", pt_running_svc_req->e_state, __LINE__));
        return SVCTXR_WRONG_STATE;
    }
    
    z_get_info_len = sizeof (PVR_BUFFER_STATUS_T);
    
    i4_ret = x_sm_get (pt_running_svc_req->h_rcrd_sess,
                       SM_SESS_RCRD_GET_TYPE_PVR_BUFFER_STATUS,
                       (VOID *) & t_pvr_buf_st,
                       & z_get_info_len);

    if (i4_ret != SMR_OK)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get PVR buffer status (%d). (L%d)\n", i4_ret, __LINE__));
        return SVCTXR_FAIL;
    } 
    
    i4_ret = rec_util_get_lba_by_pvr_buf_status (pt_running_svc_req->h_rec_util,
                                                 & t_pvr_buf_st,
                                                 & ui4_lba );
    if (i4_ret != REC_UTILR_OK)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get LBA by PVR buffer status (%d). (L%d)\n", i4_ret, __LINE__));
        return SVCTXR_FAIL;
    } 
    
    i4_ret = rec_util_update_rrt_info (pt_running_svc_req->h_rec_util,
                                       ui4_lba,
                                       z_len_data,
                                       pv_data);
    if (i4_ret != REC_UTILR_OK)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to update rrt info (%d). (L%d)\n", i4_ret, __LINE__));
        return SVCTXR_FAIL;
    } 
    
    return SVCTXR_OK;
}

#endif

static VOID _rec_svc_util_srand (
    UINT32              ui4_seed)
{
    ui4_g_rand_seed = ui4_seed;
}

static UINT32 _rec_svc_util_rand_knuth (VOID)
{
    /* This is the code supplied in Knuth Vol 2 section 3.6 p.185 bottom */
    
    #define MM        ((UINT32) 2147483647)    /* a Mersenne prime */
    #define AA        ((UINT32) 48271)         /* this does well in the spectral test */
    #define QQ        ((UINT32) 44488)         /* (long)(MM/AA) */
    #define RR        ((UINT32) 3399)          /* MM % AA; it is important that RR<QQ */

    UINT32  ui4_s, ui4_t;
    
    ui4_s = AA * (ui4_g_rand_seed % QQ);
    ui4_t = RR * (UINT32) (ui4_g_rand_seed / QQ);
    if (ui4_s > ui4_t)
    {
        ui4_g_rand_seed = ui4_s - ui4_t; 
    }
    else
    {
        ui4_g_rand_seed = MM + ui4_s - ui4_t;
    }

	return ui4_g_rand_seed;
}    

static UINT32 _rec_svc_util_get_time_fraction (VOID)
{
    #ifdef LINUX_TURNKEY_SOLUTION
    {
        return x_os_get_sys_tick ();
    }
    #else
{
    typedef struct
    {
        UINT32      ui4_ticks; 
        UINT32      ui4_cycles;
    } HAL_RAW_TIME_T ;
    
    extern VOID HAL_GetRawTime(HAL_RAW_TIME_T* pRawTime);
    
        HAL_RAW_TIME_T  t_rt = {0};
        
        HAL_GetRawTime (&t_rt);
    
        return t_rt.ui4_cycles;
    }
    #endif    
    }
    
static UINT32 _rec_svc_util_rand_low_freq (VOID)
    {
    static BOOL b_g_inited_seed = FALSE;
        
    if (!b_g_inited_seed)
    {
        _rec_svc_util_srand (_rec_svc_util_get_time_fraction () ^ ((UINT32) _rec_svc_util_rand_knuth ()));
        b_g_inited_seed = TRUE;
    }
    
    return (UINT32) _rec_svc_util_rand_knuth ();
}

static UINT32 _rec_svc_util_rand (
    UINT32      ui4_seq_seed)
{   
    UINT32      ui4_i, ui4_loop;
    
    ui4_loop = ((UINT32) _rec_svc_util_rand_knuth ()) % (ui4_seq_seed + 1);
    
    for (ui4_i = 0; ui4_i < ui4_loop; ui4_i++)
    {
        _rec_svc_util_rand_knuth ();
    }
    
    return (UINT32) _rec_svc_util_rand_knuth ();
}    

static VOID _rec_svc_util_gen_sess_crypt_key (
    REC_SVC_REQ_T*      pt_rec_svc,
    MM_CRYPT_INFO_T*    pt_crypt_info)
{
    pt_crypt_info->e_mm_crypt_type = MM_CRYPT_TYPE_PVR;
    switch (pt_rec_svc->pt_rec_cfg->e_profile)
    {
    case SVCTX_REC_PROFILE_SIMPLE:
        svctx_rec_get_crypt_info (pt_crypt_info);
        break;
        
    case SVCTX_REC_PROFILE_ADVANCED:
    {
        #define SEQ_SEED  ((UINT32) (((UINT32) _rec_svc_util_rand_knuth ()) % 100))
        
        _rec_svc_util_rand_low_freq ();
        
        pt_crypt_info->u.t_pvr_crypt.e_mode           = MM_CRYPT_MODE_3_DES_EBC;
        
        pt_crypt_info->u.t_pvr_crypt.aui4_odd_key_1[0]  = _rec_svc_util_rand (SEQ_SEED);
        pt_crypt_info->u.t_pvr_crypt.aui4_odd_key_1[1]  = _rec_svc_util_rand (SEQ_SEED);
        pt_crypt_info->u.t_pvr_crypt.aui4_odd_key_2[0]  = _rec_svc_util_rand (SEQ_SEED);
        pt_crypt_info->u.t_pvr_crypt.aui4_odd_key_2[1]  = _rec_svc_util_rand (SEQ_SEED);
        pt_crypt_info->u.t_pvr_crypt.aui4_even_key_1[0] = _rec_svc_util_rand (SEQ_SEED);
        pt_crypt_info->u.t_pvr_crypt.aui4_even_key_1[1] = _rec_svc_util_rand (SEQ_SEED);
        pt_crypt_info->u.t_pvr_crypt.aui4_even_key_2[0] = _rec_svc_util_rand (SEQ_SEED);
        pt_crypt_info->u.t_pvr_crypt.aui4_even_key_2[1] = _rec_svc_util_rand (SEQ_SEED);
        
        pt_crypt_info->u.t_pvr_crypt.e_key_idx = MM_CRYPT_KEY_ALTERNATE;
        
        break;
    }
    
    default:
        DBG_ABORT (DBG_MOD_SVCTX);
    }
}

    
/*-----------------------------------------------------------------------------
                    state utility functions
 ----------------------------------------------------------------------------*/
static INT32 _rec_svc_util_proc_queued_act (
    SVCTX_T*                    pt_svctx,
    REC_SVC_REQ_T*              pt_running_svc_req)
{
    REC_SVC_EVN_TYPE_T  e_event = REC_SVC_EVN_IGNORE;
    BOOL                b_next_msg;
    UINT8               ui1_msg_id;
    INT32               i4_ret = SVCTXR_OK;
    
    if (_rec_svc_state_act_msg_q_check_empty (pt_running_svc_req))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Act msg queue is empty. (L%d)\n", __LINE__));
        return SVCTXR_INV_COMMAND;
    }
    
    b_next_msg = TRUE;
    while (b_next_msg)
    {
        i4_ret = _rec_svc_state_act_msg_q_peek (pt_running_svc_req, &ui1_msg_id);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to peek message (%d). (L%d)\n", i4_ret, __LINE__));
            break;
        }
        
        switch (ui1_msg_id)
        {
        case SVCTX_MSG_REC_QUEUED_CONN_OPEN:
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Proc queued act: QUEUED_CONN_OPEN. (L%d)\n", __LINE__));
            e_event = REC_SVC_EVN_QUEUED_OPEN_CONN;
            break;
            
        case SVCTX_MSG_REC_QUEUED_CONN_CLOSE:
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Proc queued act: QUEUED_CONN_CLOSE. (L%d)\n", __LINE__));
            e_event = REC_SVC_EVN_QUEUED_CLOSE_CONN;
            break;

        case SVCTX_MSG_REC_QUEUED_PIPE_OPEN:
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Proc queued act: QUEUED_PIPE_OPEN. (L%d)\n", __LINE__));
            e_event = REC_SVC_EVN_QUEUED_OPEN_PIPE;
            break;
            
        case SVCTX_MSG_REC_QUEUED_PIPE_CLOSE:
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Proc queued act: QUEUED_PIPE_CLOSE. (L%d)\n", __LINE__));
            e_event = REC_SVC_EVN_QUEUED_CLOSE_PIPE;
            break;
            
        default:
            DBG_ABORT (DBG_MOD_SVCTX);
        }
        
        i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                              e_event, 
                                              NULL, 
                                              0);
        if (RET_OK (i4_ret))
        {
            UINT8   ui1_msg_temp;
            
            if (i4_ret == SVCTXR_OK)
            {
                b_next_msg = TRUE;
            }
            else if (i4_ret == SVCTXR_REC_MSG_NOT_PROCED)
            {
                b_next_msg = FALSE;
            }
            else
            {
                b_next_msg = TRUE;
            }
            
            if (b_next_msg)
            {
                i4_ret = _rec_svc_state_act_msg_q_remove (pt_running_svc_req, &ui1_msg_temp);
                if (RET_FAIL (i4_ret))
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to remove act msg (%d). (L%d)\n", i4_ret, __LINE__));
                    b_next_msg = FALSE;
                }
            }
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event [%d] (%d). (L%d)\n", e_event, i4_ret, __LINE__));
            b_next_msg = FALSE;
        }
        
        if (b_next_msg &&
            _rec_svc_state_act_msg_q_check_empty (pt_running_svc_req))
        {
            b_next_msg = FALSE;
        }
    }
    
    return SVCTXR_OK;
}    

static INT32 _rec_svc_state_util_close_conn (
    SVCTX_T*                    pt_svctx,
    BOOL                        b_chk_proceed,
    BOOL                        b_also_pipe,
    BOOL*                       pb_cm_pipe_closing)
{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    BOOL            b_close_cm_pipe = FALSE;
    INT32           i4_ret;
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TCC> Try to close conn. (L%d)\n", 
                __LINE__));
    
    if (pb_cm_pipe_closing)
    {
        *pb_cm_pipe_closing = FALSE;
    }
    
    if (pt_running_svc_req->e_cond_conn != CM_COND_DISCONNECTED &&
        x_handle_valid (pt_running_svc_req->h_conn))
    {
        BOOL        b_send_msg = FALSE;
        BOOL        b_fake_msg = TRUE;
        CM_COND_T   e_cond_conn;
        
        if (!(pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_DEINIT_SVC && 
              (pt_running_svc_req->ui4_src_op_flag & REC_SVC_DEINIT_SVC_FLAG_REUSE_PIPE)))
        {    
            BOOL    b_proceed = FALSE;
            
            if (b_chk_proceed)
            {
                i4_ret = _rec_svc_util_check_close_conn(
														pt_svctx, 
														&b_proceed
														#ifdef MW_TV_AV_BYPASS_SUPPORT
														,PLAYER_ACTION_CONN_CLOSE
														#endif
														);
                if (RET_FAIL (i4_ret))
                    {
                    DBG_INFO (("{SVCTX} ERR: [REC_HDLR] can not check close conn!!. (L%d)\n", __LINE__));
                }
            }
            else
            {
                b_proceed = TRUE;
                i4_ret = SVCTXR_OK;
            }

            if (b_proceed)
            {
                DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Close connection (%d). (L%d)\n", pt_running_svc_req->h_conn, __LINE__));
                
                i4_ret = CM_DISCONNECT (pt_running_svc_req->h_conn,
                                          & e_cond_conn );
                
                if (i4_ret == CMR_OK)
                {
                    b_send_msg = TRUE;
                    b_fake_msg = FALSE;
                    i4_ret = SVCTXR_OK;
                }
                else if (i4_ret == CMR_INV_HANDLE)
                {
                    /* don't need to send message here since the connection must have already sent DISCONNECTED notify when it's disconnected */
                    b_close_cm_pipe = TRUE;
                    i4_ret = SVCTXR_OK;
                    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] don't need to send message here since the connection must have already sent DISCONNECTED notify when it's disconnected(L%d)\n", __LINE__));
                }    
                else
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to disconnect the connection (%d). (L%d)\n", i4_ret, __LINE__));
                    
                    /* the only thing we can do now is to ignore this error and send a fake DISCONNECTED notify */
                    e_cond_conn = CM_COND_DISCONNECTED;
                    b_send_msg  = TRUE;
                    b_fake_msg  = TRUE;
                    i4_ret = SVCTXR_OK;
                }    
                
                if  (pt_running_svc_req->b_cm_real_disc)
                {
                    pt_running_svc_req->b_cm_real_disc = FALSE;
                }
                
            }
            else
            {
                i4_ret = SVCTXR_REC_MSG_NOT_PROCED;
            }
        }
        else /* (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_DEINIT_SVC && 
                 (pt_running_svc_req->ui4_src_op_flag & REC_SVC_DEINIT_SVC_FLAG_REUSE_PIPE)) */
        {
            if (!(pt_running_svc_req->ui4_src_op_flag & 
                        REC_SVC_DEINIT_SVC_FLAG_RUP_CONN_FAKE_CLOSED))
            {
                BOOL    b_proceed = FALSE;
                if (b_chk_proceed)
                {
                    i4_ret = _rec_svc_util_check_close_conn(pt_svctx,
															&b_proceed
															#ifdef MW_TV_AV_BYPASS_SUPPORT
															,PLAYER_ACTION_SESS_CLOSE
															#endif
															);
                    if (RET_FAIL (i4_ret))
                    {
                        DBG_INFO (("{SVCTX} ERR: [REC_HDLR] can not check close conn!!. (L%d)\n", __LINE__));
                    }
                }
                else
                {
                    b_proceed = TRUE;
                    i4_ret = SVCTXR_OK;
                }
                
                if (b_proceed)
                {
                DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TCC> Fake close conn & send fake msg for pipe reuse. (L%d)\n", __LINE__));
                
                /* send a fake message to simulate that the connection is disconnected */
                e_cond_conn     = CM_COND_DISCONNECTED;
                b_send_msg      = TRUE;
                b_fake_msg      = TRUE;
                b_close_cm_pipe = FALSE;
                i4_ret          = SVCTXR_OK;
                
                pt_running_svc_req->ui4_src_op_flag |= REC_SVC_DEINIT_SVC_FLAG_RUP_CONN_FAKE_CLOSED;
            }
            }
            else
            {
                DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TCC> Duplicated fake closing conn is ignored. (L%d)\n", __LINE__));
                i4_ret = SVCTXR_OK;
            }
        }
        
        if (b_send_msg)
        {
            SVCTX_MSG_T                 t_msg;
            SVCTX_REC_CONN_NFY_MSG_T*   pt_msg_conn;
            
            t_msg.h_svctx        = pt_svctx->h_sys_svctx;
            t_msg.ui1_msg_id     = SVCTX_MSG_REC_CONN_NFY;
            t_msg.pf_invoke_func = NULL;
            t_msg.pv_tag         = NULL;
            
            pt_msg_conn = (SVCTX_REC_CONN_NFY_MSG_T *) t_msg.u.t_hdlr.aui1_data;
            t_msg.u.t_hdlr.z_len_data = sizeof (SVCTX_REC_CONN_NFY_MSG_T);
            
            pt_msg_conn->b_fake   = b_fake_msg;
            pt_msg_conn->h_conn   = pt_running_svc_req->h_conn;
            pt_msg_conn->e_cond   = e_cond_conn;
            pt_msg_conn->ui4_data = 0;
            
            i4_ret = svctx_send_msg (pt_svctx->h_sys_svctx,
                                     & t_msg );
                                     
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send message. (L%d)\n", __LINE__));
            }            
        }
    }    
    else
    {
        b_close_cm_pipe = TRUE;
        i4_ret = SVCTXR_OK;
    }

    if ( b_also_pipe &&
         b_close_cm_pipe )
    {    
        i4_ret = _rec_svc_state_util_close_cm_pipe (pt_svctx);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close cm pipe (%d). (L%d)\n", i4_ret, __LINE__));
        }

        if (pb_cm_pipe_closing)
        {
            *pb_cm_pipe_closing = TRUE;
        }
        
        i4_ret = SVCTXR_OK;
    }   
    
    
    return i4_ret;
}    

static INT32 _rec_svc_state_util_close_cm_pipe (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32           i4_ret;
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TCP> Try to close cm pipe. (L%d)\n", 
                __LINE__));

    if (pt_running_svc_req->e_cond_cm_pipe != CM_COND_CLOSED && 
        x_handle_valid (pt_running_svc_req->h_cm_pipe))
    {
        CM_COND_T     e_cond_cm_pipe;
        BOOL          b_send_msg = FALSE;
        BOOL          b_fake_msg = TRUE;
        
        if (!(pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_DEINIT_SVC && 
              (pt_running_svc_req->ui4_src_op_flag & REC_SVC_DEINIT_SVC_FLAG_REUSE_PIPE)))
        {
            BOOL  b_proceed = FALSE;
            
            i4_ret = _rec_svc_util_check_close_pipe(pt_svctx,
													&b_proceed
													#ifdef MW_TV_AV_BYPASS_SUPPORT
													,PLAYER_ACTION_PIPE_CLOSE
													#endif
													);
            if (RET_FAIL (i4_ret))
                {
                DBG_INFO (("{SVCTX} ERR: [REC_HDLR] can not check close pipe!!. (L%d)\n", __LINE__));
            }
            
            if (b_proceed)
            {
                DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Close CM Pipe (%d). (L%d)\n", 
                            pt_running_svc_req->h_cm_pipe,
                            __LINE__));

                i4_ret = CM_CLOSE_PIPE (pt_running_svc_req->h_cm_pipe,
                                        & e_cond_cm_pipe);
                
                if (i4_ret == CMR_OK)
                {
                    b_send_msg = TRUE;
                    b_fake_msg = FALSE;
                }
                else if (i4_ret == CMR_INV_HANDLE)
                {
                    /* don't need to send message here since the pipe must have already sent CLOSED notify when it's closed */
                    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] don't need to send message here since the pipe must have already sent CLOSED notify when it's closed(L%d)\n", __LINE__));
                    if  (pt_running_svc_req->b_pipe_real_close)
                    {
                        b_send_msg = TRUE;
                        b_fake_msg = FALSE;
                        e_cond_cm_pipe = CM_COND_CLOSED;
                    }
                }
                else
                {
                    /* the only thing we can do now is to ignore this error and send a fake CLOSED notify */
                    e_cond_cm_pipe = CM_COND_CLOSED;
                    b_send_msg = TRUE;
                    b_fake_msg = TRUE;
                }
                
            }
            else
            {
                b_send_msg = FALSE;
                i4_ret = SVCTXR_REC_MSG_NOT_PROCED;
            }
        }
        else /* (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_DEINIT_SVC && 
                 (pt_running_svc_req->ui4_src_op_flag & REC_SVC_DEINIT_SVC_FLAG_REUSE_PIPE)) */
        {
            if (!(pt_running_svc_req->ui4_src_op_flag & REC_SVC_DEINIT_SVC_FLAG_RUP_CM_PIPE_FAKE_CLOSED))
            {
                BOOL    b_proceed = FALSE;
                
                i4_ret = _rec_svc_util_check_close_pipe(pt_svctx,
														&b_proceed
														#ifdef MW_TV_AV_BYPASS_SUPPORT
														,PLAYER_ACTION_SESS_CLOSE
														#endif
														);
                if (RET_FAIL (i4_ret))
                {
                    DBG_INFO (("{SVCTX} ERR: [REC_HDLR] can not check close pipe!!. (L%d)\n", __LINE__));
                }

                if(b_proceed)
                {
                DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TCP> Fake close pipe & send fake msg for pipe reuse. (L%d)\n", __LINE__));
                
                /* send a fake message to simulate CM-Pipe closed */
                e_cond_cm_pipe = CM_COND_CLOSED;
                b_send_msg = TRUE;
                b_fake_msg = TRUE;
                
                pt_running_svc_req->ui4_src_op_flag |= REC_SVC_DEINIT_SVC_FLAG_RUP_CM_PIPE_FAKE_CLOSED;
            }
            }
            else
            {
                DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TCP> Duplicated fake closing pipe is ignored. (L%d)\n", __LINE__));
                i4_ret = SVCTXR_OK;
            }
        }
        
        if (b_send_msg)
        {
            SVCTX_MSG_T                  t_msg;
            SVCTX_REC_CM_PIPE_NFY_MSG_T* pt_msg_cm_pipe;
            
            t_msg.h_svctx           = pt_svctx->h_sys_svctx;
            t_msg.ui1_msg_id        = SVCTX_MSG_REC_CM_PIPE_NFY;
            t_msg.pf_invoke_func    = NULL;
            t_msg.pv_tag            = NULL;
            
            pt_msg_cm_pipe            = (SVCTX_REC_CM_PIPE_NFY_MSG_T *) t_msg.u.t_hdlr.aui1_data;
            t_msg.u.t_hdlr.z_len_data = sizeof (SVCTX_REC_CM_PIPE_NFY_MSG_T);
            
            pt_msg_cm_pipe->b_fake    = b_fake_msg;
            pt_msg_cm_pipe->h_cm_pipe = pt_running_svc_req->h_cm_pipe;
            pt_msg_cm_pipe->e_cond    = e_cond_cm_pipe;
            pt_msg_cm_pipe->ui4_data  = 0;
            
            i4_ret = svctx_send_msg (pt_svctx->h_sys_svctx,
                                     & t_msg );
                                     
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send message. (L%d)\n", __LINE__));
            }            
        }
    }
    else
    {
        i4_ret = SVCTXR_OK;
    }
        
    return i4_ret;
}

static INT32 _rec_svc_state_util_free_pat_res (
    SVCTX_T*                    pt_svctx,
    REC_SVC_REQ_T*              pt_svc_req,
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl)
{
    INT32   i4_ret = SVCTXR_OK;
    
    if (pt_res_brdcst_svl == NULL)
    {
        return SVCTXR_INV_ARG;
    }    
    
    if (pt_res_brdcst_svl->h_pat != NULL_HANDLE)
    {
        /* free PAT handle */
        i4_ret = x_tm_free (pt_res_brdcst_svl->h_pat);
        
        if (i4_ret != TMR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to free PAT (%d). (L%d)\n", i4_ret, __LINE__));
        }
        
        /* send a fake UNAVAILABLE notification */
        _rec_svc_cb_load_pat_nfy (pt_res_brdcst_svl->h_pat, 
                                  0,
                                  TM_COND_UNAVAILABLE,
                                  (VOID *) pt_svctx->h_sys_svctx,
                                  0 );
    }
    
    /* free TM Root handle */
    if (pt_res_brdcst_svl->h_tm_root != NULL_HANDLE)
    {
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Free TM Root (%d). (L%d)\n", 
                    pt_res_brdcst_svl->h_tm_root,
                    __LINE__));
                    
        i4_ret = x_tm_free (pt_res_brdcst_svl->h_tm_root);
        
        if (i4_ret != TMR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to free TM Root (%d). (L%d)\n", i4_ret, __LINE__));
        }
        
        pt_res_brdcst_svl->h_tm_root  = NULL_HANDLE;
    }
    
    return i4_ret;
}

static INT32 _rec_svc_state_util_open_pat_strm (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_SRC_BRDCST_SVL_T*   pt_src_brdcst_svl;
    STREAM_COMP_ID_T            t_strm_comp_id;
    SM_COMMAND_T                at_sm_cmd[10];
    UINT8                       ui1_idx = 0;
    RSH_RCRD_INFO_SECT_T        t_info_sect;
    REC_SVC_STRM_INFO_T         t_strm_pat;
    BOOL                        b_strm_opened, b_strm_added;
    HANDLE_T                    h_strm_pat;
    INT32                       i4_ret;
    
    /* make sure record session is alive */
    if (!REC_SVC_SESS_COND_CAN_OPEN_STRM (pt_running_svc_req->e_cond_sess))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong Record Session state (%d) when open PAT stream. (L%d)\n", pt_running_svc_req->e_cond_sess, __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    i4_ret = _rec_svc_util_get_src_brdcst_svl (pt_running_svc_req,
                                               & pt_src_brdcst_svl);
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get SRC_BRDCST_SVL (%d). (L%d)\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    b_strm_opened = FALSE;
    b_strm_added  = FALSE;
    
    do 
    {
        t_strm_comp_id.e_type         = ST_RCRD_STRM;
        t_strm_comp_id.pv_stream_tag  = NULL;
        
        t_info_sect.ui4_flags         = 0;
        t_info_sect.e_tbl_grp         = RSH_TBL_GRP_PSI_PAT;
        
        at_sm_cmd[ui1_idx].e_code     = SM_RS_CMD_CODE_SET_RCRD_TYPE;
        at_sm_cmd[ui1_idx].u.ui1_data = (UINT8) RSH_RCRD_TYPE_SECT;
        ui1_idx++;
        at_sm_cmd[ui1_idx].e_code     = SM_RS_CMD_CODE_SET_PID;
        at_sm_cmd[ui1_idx].u.ui2_data = (UINT16) REC_SVC_PAT_PID;
        ui1_idx++;
        at_sm_cmd[ui1_idx].e_code     = SM_RS_CMD_CODE_SET_RCRD_INFO;
        at_sm_cmd[ui1_idx].u.pv_data  = (VOID *) & t_info_sect;
        ui1_idx++;
        at_sm_cmd[ui1_idx].e_code     = SM_RS_CMD_CODE_SET_PRG_NUM;
        at_sm_cmd[ui1_idx].u.ui2_data = pt_src_brdcst_svl->ui2_svc_id;
        ui1_idx++;
        at_sm_cmd[ui1_idx].e_code     = SM_CMD_CODE_END;
        
        i4_ret = x_sm_open_stream (pt_running_svc_req->h_rcrd_sess,
                                   & t_strm_comp_id,
                                   at_sm_cmd,
                                   _rec_svc_cb_strm_nfy,
                                   (VOID *) pt_svctx->h_sys_svctx,
                                   FALSE,
                                   & h_strm_pat );
        
        if (i4_ret != SMR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open PAT stream (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_FAIL;
            break;
        }

        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Open PAT stream (Pid=%d) (%d). (L%d)\n", 
                    REC_SVC_PAT_PID,
                    h_strm_pat,
                    __LINE__));
        
        b_strm_opened = TRUE;
        
        /* init PAT stream info */
        t_strm_pat.ui4_flag      = 0;
        t_strm_pat.e_type        = REC_SVC_STRM_TYPE_PAT;
        t_strm_pat.h_strm        = h_strm_pat;
        t_strm_pat.e_cond_strm   = SM_COND_CLOSED;
        t_strm_pat.u.t_pat.t_pid = REC_SVC_PAT_PID;
    
        REC_SVC_STRM_SET_STATE (&t_strm_pat, REC_SVC_STRM_FLAG_STATE_ACTIVE);
        
        i4_ret = _rec_svc_slst_add_strm (pt_running_svc_req,
                                         & t_strm_pat );
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to add PAT stream (%d). (L%d)\n", i4_ret, __LINE__));
            break;
        }
        
        b_strm_added = TRUE;
        
    } while (FALSE);
    
    if (i4_ret == SVCTXR_OK)
    {
    }
    else
    {
        if (b_strm_added)
        {
            i4_ret = _rec_svc_state_util_remove_strm (pt_running_svc_req,
                                                      h_strm_pat);
                                                 
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to remove stream info (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
        
        if (b_strm_opened)
        {
            i4_ret = x_sm_close_stream (h_strm_pat);
            
            if (i4_ret != SMR_OK)
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close PAT stream (%d). (L%d)\n", i4_ret, __LINE__));
                i4_ret = SVCTXR_FAIL;
            }
        }
    }
    
    return i4_ret;
}        

static INT32 _rec_svc_state_util_open_pmt_strm (
    SVCTX_T*                    pt_svctx,
    MPEG_2_PID_T                t_pid_pmt)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_SRC_BRDCST_SVL_T*   pt_src_brdcst_svl;
    STREAM_COMP_ID_T            t_strm_comp_id;
    SM_COMMAND_T                at_sm_cmd[10];
    UINT8                       ui1_idx = 0;
    RSH_RCRD_INFO_SECT_T        t_info_sect;
    HANDLE_T                    h_strm_pmt;
    BOOL                        b_strm_opened, b_strm_added;
    REC_SVC_STRM_INFO_T         t_strm_pmt;
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl;
    INT32                       i4_ret;

    /* make sure record session is alive */
    if ( !REC_SVC_SESS_COND_CAN_OPEN_STRM (pt_running_svc_req->e_cond_sess))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong Record Session state (%d) when update ES List. (L%d)\n", pt_running_svc_req->e_cond_sess, __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }

    i4_ret = _rec_svc_util_get_src_brdcst_svl (pt_running_svc_req,
                                               & pt_src_brdcst_svl);
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get SRC_BRDCST_SVL (%d). (L%d)\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    b_strm_opened = FALSE;
    b_strm_added  = FALSE;
    
    do 
    {
        t_strm_comp_id.e_type        = ST_RCRD_STRM;
        t_strm_comp_id.pv_stream_tag = NULL;
        
        t_info_sect.ui4_flags = 0;
        t_info_sect.e_tbl_grp = RSH_TBL_GRP_PSI_PMT;
        
        at_sm_cmd[ui1_idx].e_code     = SM_RS_CMD_CODE_SET_RCRD_TYPE;
        at_sm_cmd[ui1_idx].u.ui1_data = (UINT8) RSH_RCRD_TYPE_SECT;
        ui1_idx++;
        at_sm_cmd[ui1_idx].e_code     = SM_RS_CMD_CODE_SET_PID;
        at_sm_cmd[ui1_idx].u.ui2_data = (UINT16) t_pid_pmt;
        ui1_idx++;
        at_sm_cmd[ui1_idx].e_code     = SM_RS_CMD_CODE_SET_REC_UTIL_NAME;
        at_sm_cmd[ui1_idx].u.ps_text  = pt_running_svc_req->pt_rec_cfg->s_db_name;
        ui1_idx++;
        at_sm_cmd[ui1_idx].e_code     = SM_RS_CMD_CODE_SET_RCRD_INFO;
        at_sm_cmd[ui1_idx].u.pv_data  = (VOID *) & t_info_sect;
        ui1_idx++;        
        at_sm_cmd[ui1_idx].e_code     = SM_RS_CMD_CODE_SET_PRG_NUM;
        at_sm_cmd[ui1_idx].u.ui2_data = pt_src_brdcst_svl->ui2_svc_id;
        ui1_idx++;
        at_sm_cmd[ui1_idx].e_code     = SM_CMD_CODE_END;
        
        i4_ret = x_sm_open_stream (pt_running_svc_req->h_rcrd_sess,
                                   & t_strm_comp_id,
                                   at_sm_cmd,
                                   _rec_svc_cb_strm_nfy,
                                   (VOID *) pt_svctx->h_sys_svctx,
                                   FALSE,
                                   & h_strm_pmt );
        
        if (i4_ret != SMR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open PMT stream (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_FAIL;
            break;
        }
        
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Open PMT stream (Pid=%d) (%d). (L%d)\n", 
                    t_pid_pmt,
                    h_strm_pmt,
                    __LINE__));
        
        b_strm_opened = TRUE;
        
        t_strm_pmt.ui4_flag      = 0;
        t_strm_pmt.e_type        = REC_SVC_STRM_TYPE_PMT;
        t_strm_pmt.h_strm        = h_strm_pmt;
        t_strm_pmt.e_cond_strm   = SM_COND_CLOSED;
        t_strm_pmt.u.t_pmt.t_pid = t_pid_pmt;
        
        REC_SVC_STRM_SET_STATE (&t_strm_pmt, REC_SVC_STRM_FLAG_STATE_ACTIVE);
        
        i4_ret = _rec_svc_slst_add_strm (pt_running_svc_req,
                                         & t_strm_pmt );
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to add PMT stream (%d). (L%d)\n", i4_ret, __LINE__));
            break;
        }
        
        b_strm_added = TRUE;
        
        i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req, 
                                                   & pt_res_brdcst_svl );
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get RES_BRDCST_SVL (%d). (L%d)\n", i4_ret, __LINE__));
            pt_res_brdcst_svl = NULL;
        }
        
        /* set ES list to PMT stream if ui2_es_num != 0 */
        if (pt_res_brdcst_svl &&
            pt_res_brdcst_svl->t_es_list.ui2_es_num != 0)
        {
            i4_ret = _rec_svc_state_util_update_pmt_es_list_info (pt_svctx,
                                                                  pt_res_brdcst_svl,
                                                                  pt_res_brdcst_svl->t_es_list.ui2_es_num,
                                                                  pt_res_brdcst_svl->t_es_list.at_es_info);
            if (RET_FAIL (i4_ret))                                                      
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to update pmt es-list (%d). (L%d)\n", i4_ret, __LINE__));
                break;
            }
        }
         
    } while (FALSE);
    
    if (RET_FAIL (i4_ret))
    {
        if (b_strm_added)
        {
            i4_ret = _rec_svc_state_util_remove_strm (pt_running_svc_req,
                                                      h_strm_pmt);
                                                 
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to remove stream info (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
        
        if (b_strm_opened)
        {
            i4_ret = x_sm_close_stream (h_strm_pmt);
            
            if (i4_ret != SMR_OK)
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close PMT stream (%d). (L%d)\n", i4_ret, __LINE__));
                i4_ret = SVCTXR_FAIL;
            }
        }
    }
    
    return i4_ret;
}

static INT32 _rec_svc_state_util_open_pcr_strm (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    STREAM_COMP_ID_T            t_strm_comp_id;
    SM_COMMAND_T                at_sm_cmd[4];
    UINT8                       ui1_idx = 0;
    SCDB_COMMON_T               t_scdb_cmn;
    HANDLE_T                    h_strm_pcr;
    REC_SVC_STRM_INFO_T         t_strm_pcr;
    BOOL                        b_strm_opened, b_strm_added;
    INT32                       i4_ret;

    /* make sure record session is alive */
    if ( !REC_SVC_SESS_COND_CAN_OPEN_STRM (pt_running_svc_req->e_cond_sess))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong Record Session state (%d) when update ES List. (L%d)\n", pt_running_svc_req->e_cond_sess, __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    b_strm_opened = FALSE;
    b_strm_added  = FALSE;
    
    do
    {
        i4_ret = x_scdb_get_common (pt_running_svc_req->h_scdb,
                                    & t_scdb_cmn);
        if (i4_ret != SCDBR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get SCDB common (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_REC_SCDB_ERROR;
            break;
        }
        
        if (t_scdb_cmn.e_data_fmt != DATA_FMT_MPEG_2)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Not supported SCDB data format (%d). (L%d)\n", 
                        t_scdb_cmn.e_data_fmt, 
                        __LINE__));
            break;
        }

        t_strm_comp_id.e_type        = ST_RCRD_STRM;
        t_strm_comp_id.pv_stream_tag = NULL;
        
        at_sm_cmd[ui1_idx].e_code     = SM_RS_CMD_CODE_SET_RCRD_TYPE;
        at_sm_cmd[ui1_idx].u.ui1_data = (UINT8) RSH_RCRD_TYPE_PCR;
        ui1_idx++;
        at_sm_cmd[ui1_idx].e_code     = SM_RS_CMD_CODE_SET_PID;
        at_sm_cmd[ui1_idx].u.ui2_data = (UINT16) t_scdb_cmn.u_data.t_mpeg.ui2_pcr;
        ui1_idx++;
        at_sm_cmd[ui1_idx].e_code     = SM_CMD_CODE_END;
        
        i4_ret = x_sm_open_stream (pt_running_svc_req->h_rcrd_sess,
                                   & t_strm_comp_id,
                                   at_sm_cmd,
                                   _rec_svc_cb_strm_nfy,
                                   (VOID *) pt_svctx->h_sys_svctx,
                                   FALSE,
                                   & h_strm_pcr );
        
        if (i4_ret != SMR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open PCR stream (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_FAIL;
            break;
        }
        
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Open PCR stream (Pid=%d) (%d). (L%d)\n", 
                    t_scdb_cmn.u_data.t_mpeg.ui2_pcr,
                    h_strm_pcr,
                    __LINE__));
        
        b_strm_opened = TRUE;
        
        t_strm_pcr.ui4_flag      = 0;
        t_strm_pcr.e_type        = REC_SVC_STRM_TYPE_PCR;
        t_strm_pcr.h_strm        = h_strm_pcr;
        t_strm_pcr.e_cond_strm   = SM_COND_CLOSED;
        t_strm_pcr.u.t_pcr.t_pid = t_scdb_cmn.u_data.t_mpeg.ui2_pcr;
        
        REC_SVC_STRM_SET_STATE (&t_strm_pcr, REC_SVC_STRM_FLAG_STATE_ACTIVE);
        
        i4_ret = _rec_svc_slst_add_strm (pt_running_svc_req,
                                         & t_strm_pcr );
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to add PCR stream (%d). (L%d)\n", i4_ret, __LINE__));
            break;
        }
        
        b_strm_added = TRUE;
        
    } while (FALSE);
    
    if (RET_FAIL (i4_ret))
    {
        if (b_strm_added)
        {
            i4_ret = _rec_svc_state_util_remove_strm (pt_running_svc_req,
                                                      h_strm_pcr);
                                                 
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to remove stream (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
        
        if (b_strm_opened)
        {
            i4_ret = x_sm_close_stream (h_strm_pcr);
            
            if (i4_ret != SMR_OK)
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close PMT stream (%d). (L%d)\n", i4_ret, __LINE__));
                i4_ret = SVCTXR_FAIL;
            }
        }
    }
    
    return i4_ret;
}    

static INT32 _rec_svc_state_util_open_es_strm (
    SVCTX_T*                    pt_svctx,
    const SVCTX_REC_ES_INFO_T*  pt_es_info)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    STREAM_COMP_ID_T            t_strm_comp_id;
    RSH_RCRD_INFO_ES_T          t_rcrd_es_info;
    SM_COMMAND_T                at_sm_cmd[6];
    UINT8                       ui1_idx = 0;
    HANDLE_T                    h_strm_es;
    BOOL                        b_strm_opened, b_strm_added;
    BOOL                        b_encrypt;
    REC_SVC_STRM_INFO_T         t_strm_es;
    INT32                       i4_ret;
    
    /* make sure record session is alive */
    if ( !REC_SVC_SESS_COND_CAN_OPEN_STRM (pt_running_svc_req->e_cond_sess))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong Record Session state (%d) when update ES List. (L%d)\n", pt_running_svc_req->e_cond_sess, __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    b_strm_opened = FALSE;
    b_strm_added  = FALSE;
    
    do
    {
        t_strm_comp_id.e_type        = ST_RCRD_STRM;
        t_strm_comp_id.pv_stream_tag = NULL;
        
        t_rcrd_es_info.ui4_flags     = 0;
        t_rcrd_es_info.ui4_type_mask = pt_es_info->ui4_type_mask;
        
        at_sm_cmd[ui1_idx].e_code     = SM_RS_CMD_CODE_SET_RCRD_TYPE;
        at_sm_cmd[ui1_idx].u.ui1_data = (UINT8) RSH_RCRD_TYPE_ES;
        ui1_idx++;
        at_sm_cmd[ui1_idx].e_code     = SM_RS_CMD_CODE_SET_PID;
        at_sm_cmd[ui1_idx].u.ui2_data = (UINT16) pt_es_info->t_pid;
        ui1_idx++;
        at_sm_cmd[ui1_idx].e_code     = SM_RS_CMD_CODE_SET_RCRD_INFO;
        at_sm_cmd[ui1_idx].u.pv_data  = (VOID *) & t_rcrd_es_info;
        ui1_idx++;
        at_sm_cmd[ui1_idx].e_code     = SM_CMD_CODE_END;
                
        i4_ret = x_sm_open_stream (pt_running_svc_req->h_rcrd_sess,
                                   & t_strm_comp_id,
                                   at_sm_cmd,
                                   _rec_svc_cb_strm_nfy,
                                   (VOID *) pt_svctx->h_sys_svctx,
                                   FALSE,
                                   & h_strm_es );
        
        if (i4_ret != SMR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open ES stream (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_FAIL;
            break;
        }
        
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Open ES stream (Pid=%d) (%d). (L%d)\n", 
                    pt_es_info->t_pid,
                    h_strm_es,
                    __LINE__));
        
        b_strm_opened = TRUE;
        
        t_strm_es.ui4_flag        = 0;
        t_strm_es.e_type          = REC_SVC_STRM_TYPE_ES;
        t_strm_es.h_strm          = h_strm_es;
        t_strm_es.e_cond_strm     = SM_COND_CLOSED;
        
        t_strm_es.u.t_es.ui4_flag      = 0;
        t_strm_es.u.t_es.t_pid         = pt_es_info->t_pid;
        t_strm_es.u.t_es.e_auth_st     = SVCTX_REC_AUTH_STATUS_UNKNOWN;
        t_strm_es.u.t_es.b_output      = TRUE;
        t_strm_es.u.t_es.e_ca_event    = SM_EVN_CA_COND_AUTHORIZED;
        t_strm_es.u.t_es.ui4_ca_detail = 0;
        x_memset (& t_strm_es.u.t_es.t_cp_info, 0, sizeof (CP_INFO_LIST_T));
        t_strm_es.u.t_es.t_cp_info.ui1_ctrl_size = 0;
        
        REC_SVC_STRM_SET_STATE (&t_strm_es, REC_SVC_STRM_FLAG_STATE_ACTIVE);
        
        i4_ret = _rec_svc_slst_add_strm (pt_running_svc_req,
                                         & t_strm_es );
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to add PCR stream (%d). (L%d)\n", i4_ret, __LINE__));
            break;
        }
        
        b_strm_added = TRUE;
        
        /* set encryption if video / audio stream */
        i4_ret = _rec_svc_intr_check_encrypt_stream (pt_running_svc_req, 
                                                     pt_es_info->t_pid,
                                                     & b_encrypt );
        if (i4_ret != SVCTXR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to check stream (%d/%d) encryption (%d). (L%d)\n", 
                        h_strm_es,
                        pt_es_info->t_pid,
                        i4_ret, 
                        __LINE__));
            break;
        }

        if (b_encrypt)                                                
        {
#ifndef TIME_SHIFT_NO_ENCRYPTION
            i4_ret = x_sm_set (h_strm_es,
                               SM_RSH_SET_TYPE_ES_ENCRYPTION,
                               (VOID *) & pt_running_svc_req->t_sess_key,
                               sizeof (MM_CRYPT_INFO_T));
            if (i4_ret != SMR_OK)
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to set stream (%d/%d) encryption (%d). (L%d)\n", 
                            h_strm_es,
                            pt_es_info->t_pid,
                            i4_ret, 
                            __LINE__));
                i4_ret = SVCTXR_FAIL;
                break;
            }
#endif
        }
        
    } while (FALSE);
    
    
    if (RET_FAIL (i4_ret))
    {
        if (b_strm_added)
        {
            i4_ret = _rec_svc_state_util_remove_strm (pt_running_svc_req,
                                                      h_strm_es);
                                                 
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to remove stream (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
        
        if (b_strm_opened)
        {
            i4_ret = x_sm_close_stream (h_strm_es);
            
            if (i4_ret != SMR_OK)
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close ES stream (%d). (L%d)\n", i4_ret, __LINE__));
                i4_ret = SVCTXR_FAIL;
            }
        }
    }
    
    return i4_ret;    
}    

static INT32 _rec_svc_state_util_remove_strm (
    REC_SVC_REQ_T*              pt_running_svc_req,
    HANDLE_T                    h_strm)
{
    REC_SVC_STRM_INFO_T*        pt_strm;
    INT32                       i4_ret = SVCTXR_OK;
    
    pt_strm = _rec_svc_slst_get_strm_by_handle (pt_running_svc_req,
                                                h_strm);
    if (pt_strm)
    {
        /* specially take care the ES type stream */
        if (pt_strm->e_type == REC_SVC_STRM_TYPE_ES)
        {
            REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl;
            
            i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req, 
                                                       & pt_res_brdcst_svl );
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get RES_BRDCST_SVL (%d). (L%d)\n", i4_ret, __LINE__));
                pt_res_brdcst_svl = NULL;
            }
            
            if (pt_res_brdcst_svl)
            {
                SVCTX_REC_ES_LIST_T*  pt_es_list = &pt_res_brdcst_svl->t_es_list;
                UINT16                ui2_i;
                BOOL                  b_found = FALSE;
                
                for (ui2_i = 0; ui2_i < pt_es_list->ui2_es_num; ui2_i++)
                {
                    if (pt_es_list->at_es_info[ui2_i].t_pid == pt_strm->u.t_es.t_pid)
                    {
                        b_found = TRUE;
                        break;                        
                    }    
                }
                
                if (b_found)
                {
                    /* found the ES, remove it */
                    if (ui2_i == pt_es_list->ui2_idx_prim_video)
                    {
                        pt_es_list->ui2_idx_prim_video = SVCTX_REC_ES_LIST_IDX_NULL;
                    }
                    
                    if ((ui2_i + 1) < pt_es_list->ui2_es_num)
                    {
                        x_memmove (pt_es_list->at_es_info + ui2_i,
                                   pt_es_list->at_es_info + ui2_i + 1,
                                   sizeof (SVCTX_REC_ES_INFO_T) * (pt_es_list->ui2_es_num - ui2_i - 1));
                    }
                    
                    pt_es_list->ui2_es_num --;
                }
            }
        }
        
        i4_ret = _rec_svc_slst_remove_strm (pt_running_svc_req,
                                            h_strm);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to remove strm info (%d). (L%d)\n", i4_ret, __LINE__));
        }
    }
    else
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] The stream (%d) to be removed is not found. (L%d)\n", h_strm, __LINE__));
    }                                                
                                                
    return SVCTXR_OK;
}
    
static INT32 _rec_svc_state_util_close_strm (
    REC_SVC_REQ_T*              pt_svc_req,
    REC_SVC_STRM_INFO_T*        pt_strm)
{
    INT32   i4_ret;
    
    i4_ret = x_sm_close_stream (pt_strm->h_strm);
    
    if (i4_ret == SMR_OK)
    {
        REC_SVC_STRM_SET_STATE (pt_strm, REC_SVC_STRM_FLAG_STATE_CLOSING);
        i4_ret = SVCTXR_OK;
    }
    else
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close stream (%d). (L%d)\n", i4_ret, __LINE__));
        
        /* remove the stream right away */
        i4_ret = _rec_svc_state_util_remove_strm (pt_svc_req,
                                                  pt_strm->h_strm);

        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to remove strm info (%d). (L%d)\n", i4_ret, __LINE__));
        }             
        
        i4_ret = SVCTXR_FAIL;                                        
    }
    
    return i4_ret;
}    

static INT32 _rec_svc_state_util_update_pcr_strm (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_STRM_INFO_T*        pt_strm_pcr;
    SCDB_COMMON_T               t_scdb_cmn;
    BOOL                        b_req_pcr_strm;
    INT32                       i4_ret;
    
    b_req_pcr_strm = FALSE;
    
    pt_strm_pcr = _rec_svc_slst_get_actv_strm_by_type (pt_running_svc_req,
                                                       REC_SVC_STRM_TYPE_PCR);
    if (pt_strm_pcr)
    {
        i4_ret = x_scdb_get_common (pt_running_svc_req->h_scdb,
                                    & t_scdb_cmn);
        if (i4_ret != SCDBR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get SCDB common (%d). (L%d)\n", i4_ret, __LINE__));
            return SVCTXR_REC_SCDB_ERROR;
        }
        
        if (t_scdb_cmn.u_data.t_mpeg.ui2_pcr != pt_strm_pcr->u.t_pcr.t_pid)
        {
            i4_ret = _rec_svc_state_util_close_strm (pt_running_svc_req, 
                                                     pt_strm_pcr);
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close PCR stream (%d). (L%d)\n", i4_ret, __LINE__));
                i4_ret = SVCTXR_OK;
            }
            
            b_req_pcr_strm = TRUE;
        }
    }
    else
    {
        if (REC_SVC_SESS_COND_CAN_OPEN_STRM (pt_running_svc_req->e_cond_sess))
        {
            b_req_pcr_strm = TRUE;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid session state to open PCR stream. (L%d)\n", __LINE__));
            return SVCTXR_FAIL;
        }
    }                                    
    
    if (b_req_pcr_strm)
    {
        i4_ret = _rec_svc_state_util_open_pcr_strm (pt_svctx);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open PCR stream (%d). (L%d)\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
    
    return SVCTXR_OK;
}

static INT32 _rec_svc_state_util_update_pmt_es_list_info (
    SVCTX_T*                    pt_svctx,
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl,
    UINT16                      ui2_num_es,
    SVCTX_REC_ES_INFO_T*        at_es_info )
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_STRM_INFO_T*        pt_strm_pmt;
    REC_SVC_STRM_INFO_T*        pt_strm_es;
    SM_RSH_PMT_ES_LIST_INFO_T   t_sm_es_list;
    UINT16                      ui2_num_recs, ui2_total_recs, ui2_i;
    UINT32                      ui4_scdb_ver;
    BOOL                        b_restart;
    UINT32                      ui4_i;
    INT32                       i4_ret = SVCTXR_OK;

    /* check if PMT stream available now */
    pt_strm_pmt =  _rec_svc_slst_get_actv_strm_by_type (pt_running_svc_req,
                                                        REC_SVC_STRM_TYPE_PMT);
    if (pt_strm_pmt == NULL)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to find PMT stream. (L%d)\n", __LINE__));
        return SVCTXR_FAIL;
    }

    /* copy the es info */
    t_sm_es_list.ui4_flags = 0;
    
    if (pt_running_svc_req->pt_rec_cfg->e_profile == SVCTX_REC_PROFILE_ADVANCED)
    {
        t_sm_es_list.ui4_flags |= SM_RSH_PMT_ES_LIST_INFO_FLAG_AVAIL_SCDB_INFO;
    }
        
    t_sm_es_list.ui2_num_es = 0;
    t_sm_es_list.at_pids_es = pt_res_brdcst_svl->at_esp_tmp;

    for (ui4_i = 0; ui4_i < ui2_num_es; ui4_i++)
    {
        pt_strm_es = _rec_svc_slst_get_es_strm_by_pid (pt_running_svc_req,
                                                       at_es_info[ui4_i].t_pid);
        if (pt_strm_es)
        {
            if (REC_SVC_ES_AUTHORIZED (pt_running_svc_req, pt_strm_es->u.t_es.e_auth_st))
            {
                pt_res_brdcst_svl->at_esp_tmp[t_sm_es_list.ui2_num_es] = at_es_info[ui4_i].t_pid;
                t_sm_es_list.ui2_num_es ++;
            }
        }
        else
        {
            DBG_ABORT (DBG_MOD_SVCTX);
        }
    }
    
    /* get SCDB records */
    t_sm_es_list.ui2_num_recs = 0;
    t_sm_es_list.at_scdb_recs = NULL;
    
    ui4_scdb_ver = SCDB_NULL_VER_ID;
    
    do
    {
        i4_ret = SVCTXR_OK;
        b_restart = FALSE;

        /* calculate total record number */
        ui2_total_recs = 0;
        for (ui2_i = 0; ui2_i < t_sm_es_list.ui2_num_es; ui2_i ++)
        {
            ui2_num_recs = 1;
            i4_ret = x_util_scdb_get_stream_info (pt_running_svc_req->h_scdb,
                                                  ST_UNKNOWN,
                                                  t_sm_es_list.at_pids_es[ui2_i],
                                                  pt_running_svc_req->at_scdb_recs,
                                                  & ui2_num_recs,
                                                  & ui4_scdb_ver );
            if (i4_ret == SCDBR_OK)
            {
                ui2_total_recs += ui2_num_recs;
                i4_ret = SVCTXR_OK;
            }
            else if (i4_ret == SCDBR_BUF_TOO_SMALL)
            {
                ui2_total_recs += ui2_num_recs;
                i4_ret = SVCTXR_OK;
            }
            else if (i4_ret == SCDBR_DB_MODIFIED)
            {
                b_restart = TRUE;
                i4_ret = SVCTXR_OK;
                break;
            }
            else
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get scdb record number of pid [%d] (%d). (L%d)\n", 
                            t_sm_es_list.at_pids_es[ui2_i],
                            i4_ret, 
                            __LINE__));
                i4_ret = SVCTXR_FAIL;
                break;
            }
        }
        
        if (b_restart)
        {
            continue;
        }
        
        if (RET_FAIL (i4_ret))
        {
            break;
        }
        
        /* check buffer size */
        if (ui2_total_recs > pt_running_svc_req->ui2_num_scdb_recs)
        {
            UINT16      ui2_num_new;
            VOID*       pv_buf_new;
            
            if (pt_running_svc_req->at_scdb_recs != pt_running_svc_req->at_scdb_rec_buf)
            {
                x_mem_free (pt_running_svc_req->at_scdb_recs);
                pt_running_svc_req->at_scdb_recs = pt_running_svc_req->at_scdb_rec_buf;
                pt_running_svc_req->ui2_num_scdb_recs = REC_SVC_SCDB_BUF_REC_NUM_INIT;
            }
            
            ui2_num_new = ui2_total_recs + (UINT16) REC_SVC_SCDB_BUF_REC_NUM_EXPAND;
            
            pv_buf_new = x_mem_alloc (sizeof (SCDB_REC_T) * ui2_num_new);
            
            if (pv_buf_new != NULL)
            {
                pt_running_svc_req->at_scdb_recs = (SCDB_REC_T *) pv_buf_new;
                pt_running_svc_req->ui2_num_scdb_recs = ui2_num_new;
                
                i4_ret = SVCTXR_OK;
            }    
            else
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Out of memory. (L%d)\n", __LINE__));
                i4_ret = SVCTXR_NO_RESOURCE;
            }
        }
        
        if (RET_FAIL (i4_ret))
        {
            break;
        }
        
        /* get scdb info */
        ui2_total_recs = 0;
        for (ui2_i = 0; ui2_i < t_sm_es_list.ui2_num_es; ui2_i ++)
        {
            if (ui2_total_recs < pt_running_svc_req->ui2_num_scdb_recs)
            {
                ui2_num_recs = pt_running_svc_req->ui2_num_scdb_recs - ui2_total_recs;
            }
            else
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
                i4_ret = SVCTXR_REC_INTERNAL_ERROR;
                break;
            }
            
            i4_ret = x_util_scdb_get_stream_info (pt_running_svc_req->h_scdb,
                                                  ST_UNKNOWN,
                                                  t_sm_es_list.at_pids_es[ui2_i],
                                                  pt_running_svc_req->at_scdb_recs + ui2_total_recs,
                                                  & ui2_num_recs,
                                                  & ui4_scdb_ver );
            if (i4_ret == SCDBR_OK)
            {
                ui2_total_recs += ui2_num_recs;
                i4_ret = SVCTXR_OK;
            }
            else if (i4_ret == SCDBR_DB_MODIFIED)
            {
                b_restart = TRUE;
                i4_ret = SVCTXR_OK;
                break;
            }
            else
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get scdb info of pid [%d] (%d). (L%d)\n", 
                            t_sm_es_list.at_pids_es[ui2_i],
                            i4_ret, 
                            __LINE__));
                i4_ret = SVCTXR_FAIL;
                break;
            }
        }            
        
        if (b_restart)
        {
            continue;
        }
        
        if (RET_FAIL (i4_ret))
        {
            break;
        }
    
    } while (b_restart);
    
    if (RET_OK (i4_ret))
    {
        t_sm_es_list.ui2_num_recs = ui2_total_recs;
        t_sm_es_list.at_scdb_recs = pt_running_svc_req->at_scdb_recs;
    }
    else
    {
        t_sm_es_list.ui2_num_recs = 0;
        t_sm_es_list.at_scdb_recs = NULL;
    }
    
    i4_ret = x_sm_set (pt_strm_pmt->h_strm,
                       SM_RSH_SET_TYPE_PMT_ES_LIST_INFO,
                       (VOID *) & t_sm_es_list,
                       sizeof (SM_RSH_PMT_ES_LIST_INFO_T));
 
    if (i4_ret != SMR_OK)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to set ES-List to PMT stream (%d). (L%d)\n", i4_ret, __LINE__));
        return SVCTXR_FAIL;
    }
    
    return SVCTXR_OK;
}    

static SVCTX_REC_AUTH_STATUS_T _rec_svc_state_util_decide_es_auth_status (
    const REC_SVC_STRM_INFO_T*    pt_strm)
{
    SVCTX_REC_AUTH_STATUS_T     e_auth_st;
    BOOL                        b_pass = FALSE;
    
    DBG_ASSERT (pt_strm->e_type == REC_SVC_STRM_TYPE_ES, DBG_MOD_SVCTX);
    
    switch (pt_strm->u.t_es.e_ca_event)
    {
    case SM_EVN_CA_COND_CLOSED:
    case SM_EVN_CA_COND_UNKNOWN:
    case SM_EVN_CA_COND_FREE:
    case SM_EVN_CA_COND_AUTHORIZED_CA_SYS_ID:
    case SM_EVN_CA_COND_NOT_AUTHORIZED:
        b_pass = FALSE;
        break;
        
    case SM_EVN_CA_COND_AUTHORIZED:
        b_pass = TRUE;
        break;
        
    default:
        DBG_ABORT (DBG_MOD_SVCTX);
    }

    if (b_pass)
    {
        /* further check cp info */
        const CP_INFO_T*  pt_cp_info;
        UINT8             ui1_i;
        
        for (ui1_i = 0; ui1_i < pt_strm->u.t_es.t_cp_info.ui1_ctrl_size; ui1_i++)
        {
            pt_cp_info = & pt_strm->u.t_es.t_cp_info.at_cp_ctrl_list[ui1_i];
            
            switch (pt_cp_info->t_cp_type)
            {
            case CP_TYPE_EMI:
                
                switch (pt_cp_info->u.e_emi_ctrl)
                {
                case EMI_COPY_NOT_RESTRICTED:
                    b_pass = TRUE;
                    break;
                
                case EMI_NO_FURTHER_COPY:
                    b_pass = FALSE;
                    break;
                    
                case EMI_ONE_GENERATION_COPY:
                    /* TODO, current implementation is not able to signal EMI_NO_FURTHER_COPY,
                       to the downstream devices, so simply rejecting recording is a safe way. */
                    b_pass = FALSE;
                    break;
                    
                case EMI_COPY_PROHIBITED:
                    b_pass = FALSE;
                    break;
                    
                default:
                    DBG_ABORT (DBG_MOD_SVCTX);
                }
                
                break;
            
            default:
                DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Un-handled cp type: %d. (L%d)\n", 
                            pt_cp_info->t_cp_type,
                            __LINE__));
            }
        }
    }

    if (b_pass)
    {
        e_auth_st = SVCTX_REC_AUTH_STATUS_AUTHORIZED;
    }
    else
    {
        e_auth_st = SVCTX_REC_AUTH_STATUS_UNAUTHORIZED;
    }
    
    return e_auth_st;
}     

static INT32 _rec_svc_state_util_ctrl_es_list_output (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl;
    REC_SVC_STRM_INFO_T*        pt_strm;
    INT32                       i4_ret;
    
    /* turn on output of es that becomes authorized */
    DLIST_FOR_EACH (pt_strm, & pt_running_svc_req->t_wking_strms, t_dlink)
    {
        if (pt_strm->e_type == REC_SVC_STRM_TYPE_ES &&
            REC_SVC_STRM_COND_CAN_SET_CTRL_OUT (pt_strm->e_cond_strm))
        {
            if (!pt_strm->u.t_es.b_output &&
                REC_SVC_ES_AUTHORIZED (pt_running_svc_req, pt_strm->u.t_es.e_auth_st))
            {
                i4_ret = x_sm_set (pt_strm->h_strm, 
                                   SM_RSH_SET_TYPE_CTRL_OUTPUT,
                                   (VOID *) TRUE,
                                   sizeof (BOOL));
                                   
                if (i4_ret == SMR_OK)
                {
                    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] ES (%d) turns on ctrl output. (L%d)\n", 
                                pt_strm->h_strm,
                                __LINE__));
                    
                    pt_strm->u.t_es.b_output = TRUE;
                }
                else
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to ctrl pvr output (%d). (L%d)\n", i4_ret, __LINE__));
                }
            }
        }
    }
    
    /* update es list */
    i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req, 
                                               & pt_res_brdcst_svl );
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get RES_BRDCST_SVL (%d). (L%d)\n", i4_ret, __LINE__));
        pt_res_brdcst_svl = NULL;
    }
    
    /* set ES list to PMT stream if ui2_es_num != 0 */
    if (pt_res_brdcst_svl)
    {
        i4_ret = _rec_svc_state_util_update_pmt_es_list_info (pt_svctx,
                                                              pt_res_brdcst_svl,
                                                              pt_res_brdcst_svl->t_es_list.ui2_es_num,
                                                              pt_res_brdcst_svl->t_es_list.at_es_info);
        if (RET_FAIL (i4_ret))                                                      
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to update pmt es-list (%d). (L%d)\n", i4_ret, __LINE__));
        }
    }

    /* turn off output of es that becomes unauthorized */
    DLIST_FOR_EACH (pt_strm, & pt_running_svc_req->t_wking_strms, t_dlink)
    {
        if (pt_strm->e_type == REC_SVC_STRM_TYPE_ES &&
            REC_SVC_STRM_COND_CAN_SET_CTRL_OUT (pt_strm->e_cond_strm))
        {
            if (pt_strm->u.t_es.b_output &&
                !REC_SVC_ES_AUTHORIZED (pt_running_svc_req, pt_strm->u.t_es.e_auth_st))
            {
                i4_ret = x_sm_set (pt_strm->h_strm, 
                                   SM_RSH_SET_TYPE_CTRL_OUTPUT,
                                   (VOID *) FALSE,
                                   sizeof (BOOL));
                                   
                if (i4_ret == SMR_OK)
                {
                    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] ES (%d) turns off ctrl output. (L%d)\n", 
                                pt_strm->h_strm,
                                __LINE__));
                    
                    pt_strm->u.t_es.b_output = FALSE;
                }
                else
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to ctrl pvr output (%d). (L%d)\n", i4_ret, __LINE__));
                }
            }
        }
    }
    
    return SVCTXR_OK;
}

static VOID _rec_svc_state_util_set_pending_es_list (
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl,
    const SVCTX_REC_ES_LIST_T*  pt_es_list)
{
    pt_res_brdcst_svl->b_es_list_pding  = TRUE;
    pt_res_brdcst_svl->t_es_list_pding.ui2_es_num = pt_es_list->ui2_es_num;
    pt_res_brdcst_svl->t_es_list_pding.ui2_idx_prim_video = pt_es_list->ui2_idx_prim_video;
    x_memcpy ((VOID *) pt_res_brdcst_svl->t_es_list_pding.at_es_info,
              (VOID *) pt_es_list->at_es_info,
              sizeof (SVCTX_REC_ES_INFO_T) * pt_es_list->ui2_es_num);
}    

static INT32 _rec_svc_state_util_update_es_list (
    SVCTX_T*                    pt_svctx,
    const SVCTX_REC_ES_LIST_T*  pt_es_list)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl;
    REC_SVC_STRM_INFO_T*        pt_strm;
    REC_SVC_STRM_INFO_T*        pt_strm_next;
    REC_SVC_STRM_INFO_T*        pt_strm_pmt;
    MPEG_2_PID_T                t_pid_prim_vdo_old, t_pid_prim_vdo_new;
    INT32                       i4_ret;
    
    /* make sure record session is alive */
    if (!REC_SVC_SESS_COND_CAN_OPEN_STRM (pt_running_svc_req->e_cond_sess))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong Record Session state (%d) when update ES List. (L%d)\n", pt_running_svc_req->e_cond_sess, __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req,
                                               & pt_res_brdcst_svl);

    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get RES_BRDCST_SVL (%d). (L%d)\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    /* setup t_pid_prim_vdo_old */
    if (pt_res_brdcst_svl->t_es_list.ui2_idx_prim_video != SVCTX_REC_ES_LIST_IDX_NULL)
    {
        t_pid_prim_vdo_old = pt_res_brdcst_svl->t_es_list.at_es_info[pt_res_brdcst_svl->t_es_list.ui2_idx_prim_video].t_pid;
    }
    else
    {
        t_pid_prim_vdo_old = MPEG_2_NULL_PID;
    }
    
    /* setup t_pid_prim_vdo_new */
    if (pt_es_list->ui2_idx_prim_video != SVCTX_REC_ES_LIST_IDX_NULL)
    {
        t_pid_prim_vdo_new = pt_es_list->at_es_info[pt_es_list->ui2_idx_prim_video].t_pid;
    }
    else
    {
        t_pid_prim_vdo_new = MPEG_2_NULL_PID;
    }
    
    /* check if PMT stream available now */
    pt_strm_pmt = _rec_svc_slst_get_actv_strm_by_type (pt_running_svc_req,
                                                       REC_SVC_STRM_TYPE_PMT);
    
    /* 1. remove relative items in the ES List of PMT stream before ES streams is closed */
    if (pt_strm_pmt)
    {
        /* remove pid from es pid list */
        
        BOOL    b_modified;
        
        b_modified     = FALSE;
        
        if (pt_es_list->ui2_es_num == 0 || pt_es_list->at_es_info == NULL)
        {
            if (pt_res_brdcst_svl->t_es_list.ui2_es_num > 0)
            {
                pt_res_brdcst_svl->t_es_list.ui2_es_num = 0;
                b_modified = TRUE;
            }
        }
        else
        {
            UINT16               ui2_i, ui2_k, ui2_num_new;
            SVCTX_REC_ES_INFO_T* at_esi_tmp = pt_res_brdcst_svl->at_esi_tmp;
            
            ui2_num_new = 0;
            
            for (ui2_i = 0; ui2_i < pt_res_brdcst_svl->t_es_list.ui2_es_num; ui2_i ++)
            {
                for (ui2_k = 0; ui2_k < pt_es_list->ui2_es_num; ui2_k ++)
                {
                    if ( pt_es_list->at_es_info[ui2_k].t_pid == pt_res_brdcst_svl->t_es_list.at_es_info[ui2_i].t_pid )
                    {
                        /* found matched pid in the new comming ES List */
                        
                        at_esi_tmp[ui2_num_new] = pt_res_brdcst_svl->t_es_list.at_es_info[ui2_i];
                        
                        ui2_num_new ++;
                        
                        break;
                    }
                }
            }
            
            if (ui2_num_new > 0)
            {
                x_memcpy (pt_res_brdcst_svl->t_es_list.at_es_info, 
                          at_esi_tmp,
                          sizeof (SVCTX_REC_ES_INFO_T) * ui2_num_new);
            }
            
            if (ui2_num_new != pt_res_brdcst_svl->t_es_list.ui2_es_num)
            {
                pt_res_brdcst_svl->t_es_list.ui2_es_num = ui2_num_new;
                b_modified = TRUE;
            }
        }
        
        if (b_modified)
        {
            i4_ret = _rec_svc_state_util_update_pmt_es_list_info (pt_svctx,
                                                                  pt_res_brdcst_svl,
                                                                  pt_res_brdcst_svl->t_es_list.ui2_es_num,
                                                                  pt_res_brdcst_svl->t_es_list.at_es_info);
                                                                   
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to update PMT ES List info (%d). (L%d)\n", i4_ret, __LINE__));
            }                                                                   
        }
    }

    /* 2. close removed ES Streams */
    pt_strm = DLIST_HEAD (&pt_running_svc_req->t_wking_strms);
    
    while (pt_strm)
    {
        pt_strm_next = DLIST_NEXT (pt_strm, t_dlink);
        
        if (pt_strm->e_type == REC_SVC_STRM_TYPE_ES &&
            REC_SVC_STRM_GET_STATE(pt_strm) == REC_SVC_STRM_FLAG_STATE_ACTIVE)
        {
            BOOL     b_remove = FALSE;
            
            if (pt_es_list->ui2_es_num == 0 || pt_es_list->at_es_info == NULL)
            {
                b_remove = TRUE;
            }
            else
            {
                UINT16   ui2_i;
                
                b_remove = TRUE;
                for (ui2_i = 0; ui2_i < pt_es_list->ui2_es_num; ui2_i ++)
                {
                    if (pt_es_list->at_es_info[ui2_i].t_pid == pt_strm->u.t_es.t_pid )
                    {
                        b_remove = FALSE;
                        break;
                    }
                }
            }
            
            if (b_remove)
            {
                if (pt_strm->u.t_es.t_pid == t_pid_prim_vdo_old)
                {
                    pt_strm->u.t_es.ui4_flag &= (~REC_SVC_STRM_ES_FLAG_PRIM_VIDEO);
                    t_pid_prim_vdo_old = MPEG_2_NULL_PID;
                }
                
                i4_ret = _rec_svc_state_util_close_strm (pt_running_svc_req,
                                                         pt_strm);
                
                if (RET_FAIL (i4_ret))
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close stream (%d). (L%d)\n", i4_ret, __LINE__));
                }
            }
        }
        
        pt_strm = pt_strm_next;
    }
    
    /* 3. open newly added ES Streams */
    if (!(pt_es_list->ui2_es_num == 0 || pt_es_list->at_es_info == NULL))
    {
        UINT16               ui2_i;
        BOOL                 b_add;
        SVCTX_REC_ES_INFO_T  t_esi_add;
        
        for (ui2_i = 0; ui2_i < pt_es_list->ui2_es_num; ui2_i ++)
        {
            b_add     = TRUE;
            t_esi_add = pt_es_list->at_es_info[ui2_i];
            
            DLIST_FOR_EACH (pt_strm, &pt_running_svc_req->t_wking_strms, t_dlink)
            {
                if (pt_strm->e_type == REC_SVC_STRM_TYPE_ES &&
                    REC_SVC_STRM_GET_STATE(pt_strm) == REC_SVC_STRM_FLAG_STATE_ACTIVE)
                {
                    if (pt_strm->u.t_es.t_pid == pt_es_list->at_es_info[ui2_i].t_pid)
                    {
                        b_add = FALSE;
                        break;
                    }
                }
            }
            
            if (b_add)
            {
                i4_ret = _rec_svc_state_util_open_es_strm (pt_svctx,
                                                           & t_esi_add);
                                                            
                if (RET_FAIL (i4_ret))
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open ES stream (%d). (L%d)\n", i4_ret, __LINE__));
                }
            }
        }
    }

    /* 4. add relative items in the ES List of PMT stream after new ES streams is opened */
    
    if (pt_strm_pmt)
    {
        if (pt_es_list->ui2_es_num == 0 || pt_es_list->at_es_info == NULL)
        {
            /* do nothing since this case has been processed */
        }
        else
        {
            UINT16          ui2_i, ui2_k;
            BOOL            b_add;
            MPEG_2_PID_T    t_pid_add;
            BOOL            b_modified;
            UINT16          ui2_es_num_orig = pt_res_brdcst_svl->t_es_list.ui2_es_num;
            
            b_modified = FALSE;
            for (ui2_i = 0; ui2_i < pt_es_list->ui2_es_num; ui2_i++ )
            {
                b_add     = TRUE;
                t_pid_add = pt_es_list->at_es_info[ui2_i].t_pid;
                
                for (ui2_k = 0; ui2_k < ui2_es_num_orig; ui2_k ++ )
                {
                    if (pt_res_brdcst_svl->t_es_list.at_es_info[ui2_k].t_pid == t_pid_add)
                    {
                        b_add = FALSE;
                        break;
                    }
                }
                
                if (b_add)
                {
                    pt_res_brdcst_svl->t_es_list.at_es_info[pt_res_brdcst_svl->t_es_list.ui2_es_num].t_pid = t_pid_add;
                    pt_res_brdcst_svl->t_es_list.ui2_es_num ++;
                    b_modified = TRUE;
                }
            }
            
            if (b_modified)
            {
                i4_ret = _rec_svc_state_util_update_pmt_es_list_info (pt_svctx,
                                                                      pt_res_brdcst_svl,
                                                                      pt_res_brdcst_svl->t_es_list.ui2_es_num,
                                                                      pt_res_brdcst_svl->t_es_list.at_es_info);
                
                if (RET_FAIL (i4_ret))
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to update PMT ES List info (%d). (L%d)\n", i4_ret, __LINE__));
                }                                                                   
            }
        }
    }
    else
    {
        /* simply update the ES list */
        if (pt_es_list->ui2_es_num == 0 || pt_es_list->at_es_info == NULL)
        {
            pt_res_brdcst_svl->t_es_list.ui2_es_num = 0;
        }
        else
        {
            pt_res_brdcst_svl->t_es_list.ui2_es_num = pt_es_list->ui2_es_num;
            x_memcpy (pt_res_brdcst_svl->t_es_list.at_es_info, 
                      pt_es_list->at_es_info,
                      sizeof (SVCTX_REC_ES_INFO_T) * pt_es_list->ui2_es_num );
        }
    }
    
    /* 5. enable picture search to new primary video */
    if (t_pid_prim_vdo_old != t_pid_prim_vdo_new)
    {
        BOOL    b_found;
        
        if (t_pid_prim_vdo_old != MPEG_2_NULL_PID)
        {
            b_found = FALSE;
            DLIST_FOR_EACH (pt_strm, &pt_running_svc_req->t_wking_strms, t_dlink)
            {
                if (pt_strm->e_type == REC_SVC_STRM_TYPE_ES &&
                    REC_SVC_STRM_GET_STATE(pt_strm) == REC_SVC_STRM_FLAG_STATE_ACTIVE)
                {
                    if (pt_strm->u.t_es.t_pid == t_pid_prim_vdo_old)
                    {
                        b_found = TRUE;
                        break;
                    }
                }
            }
            
            if (b_found)
            {
                pt_strm->u.t_es.ui4_flag &= (~REC_SVC_STRM_ES_FLAG_PRIM_VIDEO);
                
                i4_ret = x_sm_set (pt_strm->h_strm,
                                   SM_RSH_SET_TYPE_EN_PIC_SEARCH,
                                   (VOID *) FALSE,
                                   0 );
                
                if (i4_ret != SMR_OK)
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to disable picture search (%d). (L%d)\n", i4_ret, __LINE__));
                }
            }
            else
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
            }
        }
        
        if (t_pid_prim_vdo_new != MPEG_2_NULL_PID)
        {
            b_found = FALSE;
            DLIST_FOR_EACH (pt_strm, &pt_running_svc_req->t_wking_strms, t_dlink)
            {
                if (pt_strm->e_type == REC_SVC_STRM_TYPE_ES &&
                    REC_SVC_STRM_GET_STATE(pt_strm) == REC_SVC_STRM_FLAG_STATE_ACTIVE)
                {
                    if (pt_strm->u.t_es.t_pid == t_pid_prim_vdo_new)
                    {
                        b_found = TRUE;
                        break;
                    }
                }
            }
            
            if (b_found)
            {
                pt_strm->u.t_es.ui4_flag |= REC_SVC_STRM_ES_FLAG_PRIM_VIDEO;

                i4_ret = x_sm_set (pt_strm->h_strm,
                                   SM_RSH_SET_TYPE_EN_PIC_SEARCH,
                                   (VOID *) TRUE,
                                   0 );
                
                if (i4_ret != SMR_OK)
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to enable picture search (%d). (L%d)\n", i4_ret, __LINE__));
                }
            }
            else
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
            }
        }
    }
    
    /* setup pt_res_brdcst_svl->t_es_list.ui2_idx_prim_video */
    pt_res_brdcst_svl->t_es_list.ui2_idx_prim_video = SVCTX_REC_ES_LIST_IDX_NULL;
    if (pt_res_brdcst_svl->t_es_list.ui2_es_num > 0 && 
        t_pid_prim_vdo_new != MPEG_2_NULL_PID)
    {
        UINT16      ui2_i;
        
        for (ui2_i = 0; ui2_i < pt_res_brdcst_svl->t_es_list.ui2_es_num; ui2_i ++)
        {
            if (t_pid_prim_vdo_new == pt_res_brdcst_svl->t_es_list.at_es_info[ui2_i].t_pid)
            {
                pt_res_brdcst_svl->t_es_list.ui2_idx_prim_video = ui2_i;
                break;
            }
        }
    }

    return SVCTXR_OK;
}    

static INT32 _rec_svc_state_util_free_sess_res (
    SVCTX_T*                    pt_svctx,
    BOOL                        b_free_rec_cfg)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl;
    INT32                       i4_ret;
    
    if (b_free_rec_cfg)
    {
    if (pt_running_svc_req->pt_rec_cfg)
    {
        _rec_svc_util_mem_free (pt_running_svc_req,
                                pt_running_svc_req->pt_rec_cfg);
                                
        pt_running_svc_req->pt_rec_cfg = NULL;
    }
    }

    /* close session */
    if (pt_running_svc_req->h_rcrd_sess != NULL_HANDLE)
    {
        /* not received SM_SESS_COND_CLOSED yet */
        
        i4_ret = x_sm_close_sess (pt_running_svc_req->h_rcrd_sess);
        
        if (i4_ret != SMR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close Record Session (%d). (L%d)\n", i4_ret, __LINE__));
        }
    }
    
    /* close record utility */
    if (pt_running_svc_req->h_rec_util != NULL_HANDLE)
    {
        i4_ret = rec_util_close (pt_running_svc_req->h_rec_util);
        
        if (i4_ret != REC_UTILR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close Record Utility (%d). (L%d)\n", i4_ret, __LINE__));
        }
        
        pt_running_svc_req->h_rec_util         = NULL_HANDLE;
        pt_running_svc_req->e_cond_rec_util    = REC_UTIL_COND_TRANSITION;
        pt_running_svc_req->ui4_req_id_set_cfg = REC_UTIL_INV_REQ_ID;
    }
    
    i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req,
                                               & pt_res_brdcst_svl);

    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Not supported SRC_TYPE. (L%d)\n", __LINE__));
        pt_res_brdcst_svl = NULL;
        i4_ret = SVCTXR_OK;
    }
    
    if (pt_res_brdcst_svl)
    {
        pt_res_brdcst_svl->t_es_list.ui2_es_num         = 0;
        pt_res_brdcst_svl->t_es_list.ui2_idx_prim_video = SVCTX_REC_ES_LIST_IDX_NULL;
        pt_res_brdcst_svl->b_es_list_pding                    = FALSE;
        pt_res_brdcst_svl->t_es_list_pding.ui2_es_num         = 0;
        pt_res_brdcst_svl->t_es_list_pding.ui2_idx_prim_video = SVCTX_REC_ES_LIST_IDX_NULL;
    }
        
    return SVCTXR_OK;
}

static INT32 _rec_svc_util_preproc_svc_brdcst_svl (
    SVCTX_T*                    pt_svctx,
    REC_SVC_REQ_T*              pt_svc_req,
    const SRC_SVL_INFO_T*       pt_src_svl_info)
{
    REC_SVC_SRC_BRDCST_SVL_T*   pt_src_brdcst_svl;
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl;
    HANDLE_T                    h_svl, h_tsl;
    SVL_REC_T                   t_svl_rec;
    TSL_REC_T                   t_tsl_rec;
    UINT32                      ui4_svl_ver = SVL_NULL_VER_ID;
    UINT32                      ui4_tsl_ver = TSL_NULL_VER_ID;
    BOOL                        b_svl_opened, b_tsl_opened;
    INT32                       i4_ret;
    UINT16                      ui2_lst_id;
    UINT16                      ui2_rec_id;
    
    if (pt_svc_req->e_src_rec != SRC_TYPE_BRDCST_SVL)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid Record SRC_TYPE. (L%d)\n", __LINE__));
        return SVCTXR_INV_ARG;
    }

    if (SB_ATSC_IS_ONE_PART_CHANNEL_ID (pt_src_svl_info->ui4_channel_id))
    {
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Pre-proc svc req (0x%X) for CH-%d. (L%d)\n", 
                    (UINT32) pt_svc_req,
                    SB_ATSC_GET_MAJOR_CHANNEL_NUM (pt_src_svl_info->ui4_channel_id),
                    __LINE__));
    }
    else
    {
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Pre-proc svc req (0x%X) for CH-%d.%d. (L%d)\n", 
                    (UINT32) pt_svc_req,
                    SB_ATSC_GET_MAJOR_CHANNEL_NUM (pt_src_svl_info->ui4_channel_id),
                    SB_ATSC_GET_MINOR_CHANNEL_NUM (pt_src_svl_info->ui4_channel_id),
                    __LINE__));
    }

    b_svl_opened = FALSE;
    b_tsl_opened = FALSE;
#ifdef SYS_MHP_SUPPORT
    if (pt_src_svl_info->t_silent.b_enable == TRUE)
    {
        ui2_lst_id = pt_src_svl_info->t_silent.ui2_svc_lst_id;
        ui2_rec_id = pt_src_svl_info->t_silent.ui2_svl_rec_id;
    } 
    else 
    {
        ui2_lst_id = pt_src_svl_info->ui2_svc_lst_id;
        ui2_rec_id = pt_src_svl_info->ui2_svl_rec_id;
    }    
#else
    ui2_lst_id = pt_src_svl_info->ui2_svc_lst_id;
    if (pt_src_svl_info->t_silent.b_enable == TRUE)
    {
        ui2_rec_id = pt_src_svl_info->t_silent.ui2_svl_rec_id;
    } 
    else 
    {
        ui2_rec_id = pt_src_svl_info->ui2_svl_rec_id;
    } 
#endif
    do
    {          
        i4_ret = x_svl_open (ui2_lst_id,
                             NULL,
                             NULL,
                             & h_svl );
        if (i4_ret != SVLR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open SVL (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_FAIL;
            break;
        }
        
        b_svl_opened = TRUE;
        
        i4_ret = x_svl_get_rec (h_svl,
                                ui2_rec_id,
                                & t_svl_rec,
                                & ui4_svl_ver);
        if (i4_ret != SVLR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get SVL record (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_FAIL;
            break;
        }
        
        i4_ret = x_tsl_open (t_svl_rec.uheader.t_rec_hdr.ui2_tsl_id,
                             NULL,
                             NULL,
                             & h_tsl);
        if (i4_ret != TSLR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open TSL (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_FAIL;
            break;
        }
        
        b_tsl_opened = TRUE;
        
        i4_ret = x_tsl_get_rec (h_tsl,
                                t_svl_rec.uheader.t_rec_hdr.ui2_tsl_rec_id,
                                & t_tsl_rec,
                                & ui4_tsl_ver);
        if (i4_ret != TSLR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get TSL record (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_FAIL;
            break;
        }
        
        /* check if the broadcast type is recordable */
        if (!REC_SVC_SUPPORTED_BRDCST_TYPE (t_svl_rec.uheader.t_rec_hdr.e_brdcst_type))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Not Supported Broadcast Type: %d. (L%d)\n", 
                        t_svl_rec.uheader.t_rec_hdr.e_brdcst_type, 
                        __LINE__));
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }
        
        pt_src_brdcst_svl = & pt_svc_req->u_src.t_svl;
        pt_res_brdcst_svl = & pt_svc_req->u_res.t_svl;
        
        pt_src_brdcst_svl->t_param     = *pt_src_svl_info;
        pt_src_brdcst_svl->ui2_on_id   = t_tsl_rec.uheader.t_desc.ui2_on_id;
        pt_src_brdcst_svl->ui2_ts_id   = t_tsl_rec.uheader.t_desc.ui2_ts_id;
        pt_src_brdcst_svl->ui2_svc_id  = t_svl_rec.uheader.t_rec_hdr.ui2_prog_id;
        pt_src_brdcst_svl->b_fine_tune = ((t_svl_rec.uheader.t_rec_hdr.ui4_nw_mask & SB_VNET_FREQ_EDITED) || 
                                           (t_svl_rec.uheader.t_rec_hdr.ui4_option_mask & SB_VOPT_FREQ_EDITED) ? 
                                              FALSE : TRUE);
#ifdef MW_NOT_DISCONNECT_WHEN_NW_CHANGE
        switch (t_tsl_rec.uheader.t_desc.e_bcst_medium) {
        case BRDCST_MEDIUM_DIG_TERRESTRIAL:
            pt_src_brdcst_svl->ui4_freq = t_tsl_rec.udata.t_ter_dig.ui4_freq;
            break;
        case BRDCST_MEDIUM_DIG_CABLE:
            pt_src_brdcst_svl->ui4_freq = t_tsl_rec.udata.t_cab_dig.ui4_freq;
            break;
        case BRDCST_MEDIUM_DIG_SATELLITE:
            pt_src_brdcst_svl->ui4_freq = t_tsl_rec.udata.t_sat_dig.ui4_freq;
            break;
        case BRDCST_MEDIUM_ANA_TERRESTRIAL:
            pt_src_brdcst_svl->ui4_freq = t_tsl_rec.udata.t_ter_ana.ui4_freq;
            break;
        case BRDCST_MEDIUM_ANA_CABLE:
            pt_src_brdcst_svl->ui4_freq = t_tsl_rec.udata.t_cab_ana.ui4_freq;
            break;
        case BRDCST_MEDIUM_ANA_SATELLITE:
            pt_src_brdcst_svl->ui4_freq = t_tsl_rec.udata.t_sat_ana.ui4_freq;
            break;
        default:
            pt_src_brdcst_svl->ui4_freq = 0;
            break;
        }
#endif

        pt_res_brdcst_svl->e_brdcst_type    = t_svl_rec.uheader.t_rec_hdr.e_brdcst_type;
        pt_res_brdcst_svl->e_brdcst_medium  = t_tsl_rec.uheader.t_desc.e_bcst_medium;
        pt_res_brdcst_svl->h_tm_root        = NULL_HANDLE;
        pt_res_brdcst_svl->h_pat            = NULL_HANDLE;
        pt_res_brdcst_svl->e_cond_pat       = TM_COND_UNAVAILABLE;
        pt_res_brdcst_svl->t_es_list.ui4_strct_ver      = 1;
        pt_res_brdcst_svl->t_es_list.ui2_es_num         = 0;
        pt_res_brdcst_svl->t_es_list.at_es_info         = pt_res_brdcst_svl->at_esi_1;
        pt_res_brdcst_svl->t_es_list.ui2_idx_prim_video = SVCTX_REC_ES_LIST_IDX_NULL;
        pt_res_brdcst_svl->b_es_list_pding                    = FALSE;
        pt_res_brdcst_svl->t_es_list_pding.ui4_strct_ver      = 1;
        pt_res_brdcst_svl->t_es_list_pding.ui2_es_num         = 0;
        pt_res_brdcst_svl->t_es_list_pding.at_es_info         = pt_res_brdcst_svl->at_esi_2;
        pt_res_brdcst_svl->t_es_list_pding.ui2_idx_prim_video = SVCTX_REC_ES_LIST_IDX_NULL;
        x_memset ((VOID *) pt_res_brdcst_svl->at_esi_tmp,    0, sizeof (SVCTX_REC_ES_INFO_T) * REC_SVC_MAX_RCRD_STRM_NUM);
        x_memset ((VOID *) pt_res_brdcst_svl->at_esi_1,      0, sizeof (SVCTX_REC_ES_INFO_T) * REC_SVC_MAX_RCRD_STRM_NUM);
        x_memset ((VOID *) pt_res_brdcst_svl->at_esi_2,      0, sizeof (SVCTX_REC_ES_INFO_T) * REC_SVC_MAX_RCRD_STRM_NUM);
        x_memset ((VOID *) pt_res_brdcst_svl->at_esp_tmp,    0, sizeof (MPEG_2_PID_T)        * REC_SVC_MAX_RCRD_STRM_NUM);
    
    } while (FALSE);
    
    if (b_svl_opened)
    {
        INT32   i4_ret_svl;
        
        i4_ret_svl = x_svl_close (h_svl);
        
        if (i4_ret_svl != SVLR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close SVL (%d). (L%d)\n", i4_ret, __LINE__));
        }
    }
    
    if (b_tsl_opened)
    {
        INT32   i4_ret_tsl;
        
        i4_ret_tsl = x_tsl_close (h_tsl);
        
        if (i4_ret_tsl != TSLR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close TSL (%d). (L%d)\n", i4_ret, __LINE__));
        }
    }
    
    return i4_ret;
}


/*-----------------------------------------------------------------------------
                    common event handler
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static VOID _rec_svc_state_util_dbg_dump_q_events (
    const REC_SVC_REQ_T*        pt_svc_req)
{
    REC_SVC_EVN_INFO_T*  pt_evn;
    
    if (!DLIST_IS_EMPTY (& pt_svc_req->t_evn_q))
    {
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Queued events: "));
        
        DLIST_FOR_EACH (pt_evn, & pt_svc_req->t_evn_q, t_dlink)
        {
            DBG_INFO (("(%d:%d)", pt_evn->ui4_sn, pt_evn->e_event));
        }
        
        DBG_INFO (("\n"));
    }
}    

#ifdef MW_TV_AV_BYPASS_SUPPORT
static VOID _rec_svc_state_util_dbg_dump_link (
    ACTIVE_LINK_TYPE_T          e_link_type,
    SRC_TYPE_T                  e_src_type)
{
    ACTIVE_LINK_ENTRY_T* pt_link;
    QUEUED_REQ_T*        pt_qr;
    UINT32               ui4_i, ui4_num_link, ui4_q;
    
    ui4_num_link = svctx_mgr_get_link_num (e_link_type, e_src_type);
    
    for (ui4_i = 0; ui4_i < ui4_num_link; ui4_i++)
    {
        pt_link = svctx_mgr_get_link_obj (e_link_type, 
                                          e_src_type,
                                          ui4_i );
        
        DBG_INFO (("    (%d) SVCTX: \"%s\" (last_op: %d) \n", 
                   ui4_i, 
                   pt_link->pt_worker->s_name,
                   pt_link->e_last_op));
        
        pt_qr = pt_link->pt_qr_head;
        ui4_q = 0;
        
        while (pt_qr)
        {
            DBG_INFO (("        (%d.%d) Queued: \"%s\"\n", ui4_i, ui4_q, pt_qr->pt_svctx->s_name));
            
            switch (pt_qr->t_var_info.e_var_type)
            {
            case VAR_TYPE_SERVICE:
                DBG_INFO (("              Type: SERVICE, op_type: %d, msg_id: %d\n", 
                           pt_qr->t_var_info.e_op_type, 
                           pt_qr->t_var_info.ui1_msg_id));
                break;
                
            case VAR_TYPE_PLAYER:
                DBG_INFO (("              Type: PLAYER, op_type: %d, msg_id: %d\n", 
                           pt_qr->t_var_info.e_op_type, 
                           pt_qr->t_var_info.ui1_msg_id));
                           
                DBG_INFO (("              player_type: %d \n", 
                           pt_qr->t_var_info.u.t_player_info.e_player_type ));
                
                break;
            
            default:
                break;
            }
            
            ui4_q ++;
            pt_qr = pt_qr->pt_next;
        }
    }
}
    
static VOID _rec_svc_state_util_dbg_dump_svctx_mgr_qr (
    const SVCTX_T*              pt_svctx,
    UINT32                      ui4_line_num)
{
    REC_SVC_REQ_T*      pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Dump SVCTX_MGR queued req. (L%d)\n\n", ui4_line_num));
    
    DBG_INFO (("    ===== SRC_TYPE: %d, ACTIVE_LINK_NOW  =====\n", 
               pt_running_svc_req->e_src_rec));
    
    _rec_svc_state_util_dbg_dump_link (ACTIVE_LINK_NOW,
                                       pt_running_svc_req->e_src_rec);

    DBG_INFO (("    ===== SRC_TYPE: %d, ACTIVE_LINK_NEXT =====\n", 
               pt_running_svc_req->e_src_rec));
    
    _rec_svc_state_util_dbg_dump_link (ACTIVE_LINK_NEXT,
                                       pt_running_svc_req->e_src_rec);
                                       
    DBG_INFO (("\n"));
}
#endif
#endif

static INT32 _rec_svc_state_util_sess_on_scdb_updated (
    SVCTX_T*                    pt_svctx,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data)
{
    REC_SVC_REQ_T*       pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    SVCTX_SCDB_MSG_T*    pt_msg_scdb = (SVCTX_SCDB_MSG_T *) pv_data;
    INT32                i4_ret;
    
    if (z_len_data != sizeof (SVCTX_SCDB_MSG_T))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    if (pt_running_svc_req->e_state == REC_SVC_STATE_SESS_OPENED   ||
        pt_running_svc_req->e_state == REC_SVC_STATE_SESS_STARTING ||
        pt_running_svc_req->e_state == REC_SVC_STATE_SESS_STARTED  ||
        pt_running_svc_req->e_state == REC_SVC_STATE_SESS_STOPPING )
    {
        if (pt_msg_scdb->ui4_reason & (SCDB_RECORD_ADD | SCDB_RECORD_DEL | SCDB_RECORD_MOD))
        {
            if (pt_running_svc_req->pf_rec_nfy)
            {
                if(pt_running_svc_req->pt_rec_cfg != NULL)
                {
                    pt_running_svc_req->pf_rec_nfy (pt_running_svc_req->pt_rec_cfg->h_svctx_nfy,
                                                pt_running_svc_req->pv_rec_nfy_tag,
                                                SVCTX_NTFY_CODE_REC_REQ_ES_LIST,
                                                0 );
                }
            }
            else
            {
                DBG_ABORT (DBG_MOD_SVCTX);
            }  
        }
        
        if (pt_msg_scdb->ui4_reason & SCDB_COMMON_MOD)
        {
            /* update PCR stream */
            if (REC_SVC_SESS_COND_CAN_OPEN_STRM (pt_running_svc_req->e_cond_sess))
            {
                i4_ret = _rec_svc_state_util_update_pcr_strm (pt_svctx);
                
                if (RET_FAIL (i4_ret))
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open PCR stream (%d). (L%d)\n", i4_ret, __LINE__));
                    return i4_ret;
                }
            }
        }
    }
    else
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    return SVCTXR_OK;
}
    
static INT32 _rec_svc_state_util_on_req_set_es_list (
    SVCTX_T*                    pt_svctx,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data)
{
    SVCTX_REC_REQ_SET_ES_LIST_MSG_T*  pt_msg_info = (SVCTX_REC_REQ_SET_ES_LIST_MSG_T *) pv_data;
    INT32                             i4_ret;
    
    if (z_len_data != sizeof (SVCTX_REC_REQ_SET_ES_LIST_MSG_T))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    i4_ret = _rec_svc_state_util_update_es_list (pt_svctx,
                                                 & pt_msg_info->t_es_list);
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to update ES List (%d). (L%d)\n", i4_ret, __LINE__));
        return i4_ret;        
    }
    
    return SVCTXR_OK;
}

static INT32 _rec_svc_state_util_on_pat_updated (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl;
    REC_SVC_SRC_BRDCST_SVL_T*   pt_src_brdcst_svl;
    PSI_PID_SVC_ID_INFO_T       t_pid_svc_id;
    UINT16                      ui2_num_entry;
    SIZE_T                      z_len;
    TM_COND_T                   e_cond_pat;
    MPEG_2_PID_T                t_pid_pmt;
    BOOL                        b_found_pmt_entry;
    REC_SVC_STRM_INFO_T*        pt_strm;
    UINT16                      ui2_i;
    BOOL                        b_open_pmt_strm;
    INT32                       i4_ret;
    
    i4_ret = _rec_svc_util_get_src_brdcst_svl (pt_running_svc_req,
                                               & pt_src_brdcst_svl );
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get SRC_BRDCST_SVL (%d). (L%d)\n", i4_ret, __LINE__));
        return i4_ret;
    }

    i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req,
                                               & pt_res_brdcst_svl);

    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get RES_BRDCST_SVL (%d). (L%d)\n", i4_ret, __LINE__));
        return i4_ret;
    }

    z_len = sizeof (UINT16);
    i4_ret = x_tm_get_obj ( pt_res_brdcst_svl->h_pat,
                            TM_GET_TYPE_NUM_FIRST_LOOP_ENTRIES,
                            NULL,
                            (VOID *) & ui2_num_entry,
                            & z_len,
                            & e_cond_pat );

    if ( i4_ret != TMR_OK )
    {
        if (i4_ret == TMR_INV_HANDLE_STATE)
        {
            /* wait for next PAT notify for stable state */
            return SVCTXR_OK;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get pat entry number (%d). (L%d)\n", i4_ret, __LINE__));
            return SVCTXR_FAIL;
        }
    }
    
    if (!(e_cond_pat == TM_COND_AVAILABLE ||
          e_cond_pat == TM_COND_UPDATE))
    {
        /* wait for next PAT notify for stable state */
        return SVCTXR_OK;
    }
    
    /* get PMT pid by program id */
    b_found_pmt_entry = FALSE;
    t_pid_pmt         = MPEG_2_NULL_PID;
    
    for (ui2_i = 0; ui2_i < ui2_num_entry; ui2_i ++)
    {
        i4_ret = x_psi_get_pat_entry (pt_res_brdcst_svl->h_pat, 
                                      ui2_i,
                                      & t_pid_svc_id,
                                      & e_cond_pat );
        
        if (i4_ret == TMR_OK)
        {
            if (!(e_cond_pat == TM_COND_AVAILABLE ||
                  e_cond_pat == TM_COND_UPDATE))
            {
                /* wait for next PAT notify for stable state */
                return SVCTXR_OK;
            }
            
            if (t_pid_svc_id.ui2_svc_id == pt_src_brdcst_svl->ui2_svc_id)
            {
                t_pid_pmt = t_pid_svc_id.t_pid;
                b_found_pmt_entry = TRUE;
                break;
            }
        }
        else
        {
            if (i4_ret == TMR_INV_HANDLE_STATE)
            {
                /* wait for next PAT notify for stable state */
                return SVCTXR_OK;
            }
            else
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get pat entry [%d/%d] (i4_ret=%d). (L%d)\n", 
                            ui2_i, ui2_num_entry, i4_ret, __LINE__));
                
                return SVCTXR_FAIL;
            }
        }
    }
    
    if (!b_found_pmt_entry)
    {
        /* TODO: error handling, currently report this error anyway */
        
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to find PMT entry in PAT. (L%d)\n", __LINE__));
        
        /* Simply return OK here since SCDB will be cleared by CM and 
           AP will set a new ES list (which is emtpy). Then this exception 
           case is natually handled. */
        return SVCTXR_OK;
    }
    
    /* ---- PMT stream ---- */
    b_open_pmt_strm = FALSE;
    pt_strm = _rec_svc_slst_get_actv_strm_by_type (pt_running_svc_req,
                                                   REC_SVC_STRM_TYPE_PMT );
    
    if (pt_strm)
    { 
        /* check if PMT pid is changed */
        /* if PMT pid is changed, should close original PMT stream and reopen another one */
        if (t_pid_pmt != pt_strm->u.t_pmt.t_pid)
        {
            i4_ret = _rec_svc_state_util_close_strm (pt_running_svc_req,
                                                     pt_strm );
                                                      
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close stream (%d). (L%d)\n", i4_ret, __LINE__));
            }

            /* reopen another PMT stream */
            b_open_pmt_strm = TRUE;
        }
    }
    else
    {
        /* open PMT stream if it's not opened yet */
        b_open_pmt_strm = TRUE;
    }
    
    if (b_open_pmt_strm)
    {
        i4_ret = _rec_svc_state_util_open_pmt_strm (pt_svctx, 
                                                    t_pid_pmt);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open PMT stream (%d). (L%d)\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
    
    return SVCTXR_OK;
}    

static INT32 _rec_svc_state_util_on_qreq_removed (
    SVCTX_T*            pt_svctx)
{
    REC_SVC_REQ_T*   pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32            i4_ret;

    pt_running_svc_req->b_act_q_in_mgr = FALSE;
    
    if (_rec_svc_state_act_msg_q_check_empty (pt_running_svc_req))
    {
        return SVCTXR_OK;
    }
    
    i4_ret = _rec_svc_util_proc_queued_act (pt_svctx, pt_running_svc_req);
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to process queued act (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    return i4_ret;
}

#ifdef CLI_LVL_ALL
static CHAR  *as_g_state_name[] = { "STATE_NOT_INIT",
                                    "STATE_DISCONNECTED",
                                    "STATE_CONNECTING",
                                    "STATE_CONNECTED",
                                    "STATE_DISCONNECTING",
                                    "STATE_WAIT_RECONN",
                                    "STATE_SESS_OPENING",
                                    "STATE_SESS_OPENED",
                                    "STATE_SESS_STARTING",
                                    "STATE_SESS_STARTED",
                                    "STATE_SESS_STOPPING",
                                    "STATE_SESS_CLOSING",
                                    "STATE_SESS_ERROR",
                                    "STATE_ERROR" };
#endif

/*-----------------------------------------------------------------------------
                    state transition functions
 ----------------------------------------------------------------------------*/
static INT32 _rec_svc_state_transit_disconned (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*   pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_STATE_T  e_state_old;
    BOOL             b_blocked;
    INT32            i4_ret;

    /* check previous state and preconditions */
    if (pt_running_svc_req->e_state != REC_SVC_STATE_NOT_INIT      &&
        pt_running_svc_req->e_state != REC_SVC_STATE_CONNECTING    &&
        pt_running_svc_req->e_state != REC_SVC_STATE_DISCONNECTING &&
        pt_running_svc_req->e_state != REC_SVC_STATE_WAIT_RECONN   &&
        pt_running_svc_req->e_state != REC_SVC_STATE_ERROR )
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong state (%d) when transiting to DISCONNECTED. (L%d)\n", pt_running_svc_req->e_state, __LINE__));
        
        return SVCTXR_REC_INTERNAL_ERROR;
    }

    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] State Transition: %s(%d) -> %s(%d). (L%d)\n", 
                as_g_state_name[pt_running_svc_req->e_state],
                pt_running_svc_req->e_state,
                as_g_state_name[REC_SVC_STATE_DISCONNECTED],
                REC_SVC_STATE_DISCONNECTED,
                __LINE__));
    
    e_state_old = pt_running_svc_req->e_state;
    pt_running_svc_req->e_state     = REC_SVC_STATE_DISCONNECTED;
    pt_running_svc_req->ui4_st_flag = 0;
    
    if (e_state_old == REC_SVC_STATE_NOT_INIT)
    {
        /* TODO: do some svc init here */
        /* do following if fail 
            (1) free resources
            (2) roll-back to NOT_INIT state 
            (3) return SVCTXR_FAIL
        */
        REC_SVC_SRC_BRDCST_SVL_T*   pt_src_brdcst_svl;

        i4_ret = _rec_svc_util_get_src_brdcst_svl (pt_running_svc_req,
                                                   & pt_src_brdcst_svl );
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get SRC_BRDCST_SVL (%d). (L%d)\n", i4_ret, __LINE__));
            pt_src_brdcst_svl = NULL;
        }
        
        if (pt_src_brdcst_svl)
        {
            if (SB_ATSC_IS_ONE_PART_CHANNEL_ID (pt_src_brdcst_svl->t_param.ui4_channel_id))
            {
                DBG_INFO (("{SVCTX} INFO: [REC_HDLR] New running svc req: 0x%X (CH-%d). (L%d)\n", 
                            (UINT32) pt_running_svc_req,
                            SB_ATSC_GET_MAJOR_CHANNEL_NUM (pt_src_brdcst_svl->t_param.ui4_channel_id),
                            __LINE__));
            }
            else
            {
                DBG_INFO (("{SVCTX} INFO: [REC_HDLR] New running svc req: 0x%X (CH-%d.%d). (L%d)\n", 
                            (UINT32) pt_running_svc_req,
                            SB_ATSC_GET_MAJOR_CHANNEL_NUM (pt_src_brdcst_svl->t_param.ui4_channel_id),
                            SB_ATSC_GET_MINOR_CHANNEL_NUM (pt_src_brdcst_svl->t_param.ui4_channel_id),
                            __LINE__));
            }        
        }
        
        /* update b_svc_blked */
        i4_ret = _rec_svc_state_util_check_block (pt_svctx,
                                                  & b_blocked);
        if (RET_OK (i4_ret))
        {
            pt_running_svc_req->b_svc_blked = b_blocked;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to check service block (%d). (L%d)\n", i4_ret, __LINE__));
        }                                                  
    }
    else  /* if (e_state_old == REC_SVC_STATE_DISCONNECTING || 
                 e_state_old == REC_SVC_STATE_CONNECTING    || 
                 e_state_old == REC_SVC_STATE_WAIT_RECONN   ||
                 e_state_old == REC_SVC_STATE_ERROR)        */
    {
        if (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_INIT_SVC)
        {
            rec_hdlr_invoke_client_svc_nfy_fct (pt_svctx,
                                                SVCTX_NTFY_CODE_SIGNAL_LOSS,
                                                ST_UNKNOWN );
        }
    }
     
    return SVCTXR_OK;
}    

static VOID _rec_svc_state_transit_not_init (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32           i4_ret;

    /* check previous state and preconditions */
    if (pt_running_svc_req->e_state != REC_SVC_STATE_DISCONNECTED)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong state (%d) when transiting to NOT_INIT. (L%d)\n", pt_running_svc_req->e_state, __LINE__));
    }
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] State Transition: %s(%d) -> %s(%d). (L%d)\n", 
                as_g_state_name[pt_running_svc_req->e_state],
                pt_running_svc_req->e_state,
                as_g_state_name[REC_SVC_STATE_NOT_INIT],
                REC_SVC_STATE_NOT_INIT,
                __LINE__));
    
    pt_running_svc_req->e_state = REC_SVC_STATE_NOT_INIT;
    pt_running_svc_req->ui4_st_flag = 0;
    
    /* stop connecting timer */
    if (pt_running_svc_req->b_on_timer_conn)
    {
        i4_ret = x_timer_stop (pt_running_svc_req->h_timer_conn);
        
        if (i4_ret != OSR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to stop h_timer_conn (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_OK;
        }
        
        pt_running_svc_req->b_on_timer_conn = FALSE;
    }
    
    /* stop watchdog timer */
    if (pt_running_svc_req->b_on_timer_wd)
    {
        i4_ret = x_timer_stop (pt_running_svc_req->h_timer_wd);
        
        if (i4_ret != OSR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to stop h_timer_wd (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_OK;
        }
        
        pt_running_svc_req->b_on_timer_wd = FALSE;
    }
    
    DBG_ASSERT (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_DEINIT_SVC, DBG_MOD_SVCTX);
    
    /* check if preset conn info for the next service req */
    if (pt_running_svc_req->ui4_src_op_flag & REC_SVC_DEINIT_SVC_FLAG_REUSE_PIPE)
    {
        REC_SVC_REQ_T*  pt_pending_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_pending_svc_req;
        
        if (pt_pending_svc_req)
        {
            /* [TODO] check if pt_pending_svc_req is still in the same TS */
            
            if (!REC_SVC_STATE_PERMIT_PRESET_CONN_INFO (pt_pending_svc_req->e_state))
            {
                DBG_ABORT (DBG_MOD_SVCTX);
            }
            
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Preset conn info 0x%X->0x%X "
                       "(h_pipe=%d, e_cond_pipe=%d, h_conn=%d). (L%d)\n", 
                       (UINT32) pt_running_svc_req,
                       (UINT32) pt_pending_svc_req,
                       pt_running_svc_req->t_conn_info_reuse.h_cm_pipe,
                       pt_running_svc_req->t_conn_info_reuse.e_cond_cm_pipe,
                       pt_running_svc_req->t_conn_info_reuse.h_conn,
                       __LINE__));
            
            i4_ret = rec_svc_preset_conn_info (pt_pending_svc_req,
                                               TRUE,
                                               & pt_running_svc_req->t_conn_info_reuse );
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to preset conn info (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
        else
        {
            /* this case should not happen */
            /*DBG_ABORT (DBG_MOD_SVCTX);*/
        }
        
        x_memset (& pt_running_svc_req->t_conn_info_reuse, 0, sizeof (REC_SVC_CONN_INFO_T));
    }
    
}   

static INT32 _rec_svc_state_transit_conning (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_STATE_T             e_state_old;
    INT32                       i4_ret;

    /* check previous state and preconditions */
    if (!(pt_running_svc_req->e_state == REC_SVC_STATE_DISCONNECTED ||
          pt_running_svc_req->e_state == REC_SVC_STATE_WAIT_RECONN))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong state (%d) when transiting to CONNECTING. (L%d)\n", pt_running_svc_req->e_state, __LINE__));
        
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] State Transition: %s(%d) -> %s(%d). (L%d)\n", 
                as_g_state_name[pt_running_svc_req->e_state],
                pt_running_svc_req->e_state,
                as_g_state_name[REC_SVC_STATE_CONNECTING],
                REC_SVC_STATE_CONNECTING,
                __LINE__));
    
    e_state_old = pt_running_svc_req->e_state;
    pt_running_svc_req->e_state = REC_SVC_STATE_CONNECTING;
    pt_running_svc_req->ui4_st_flag = 0;

    do
    {
        /* stop connecting timer */
        if (pt_running_svc_req->b_on_timer_conn)
        {
            /* the timer should alreadly be stopped before leaving WAIT_RECONN state */
            
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
            
            i4_ret = x_timer_stop (pt_running_svc_req->h_timer_conn);
            
            if (i4_ret != OSR_OK)
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to stop h_timer_conn (%d). (L%d)\n", i4_ret, __LINE__));
                i4_ret = SVCTXR_OK;
            }
            
            pt_running_svc_req->b_on_timer_conn = FALSE;
        }
        
        if (e_state_old == REC_SVC_STATE_DISCONNECTED)
        {
            i4_ret = _rec_svc_state_conning_util_open_pipe (pt_svctx);
            
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open pipe by util (%d). (L%d)\n", i4_ret, __LINE__));
                break;
            }
        }
        else /* e_state_old == REC_SVC_STATE_WAIT_RECONN */
        {
            if (pt_running_svc_req->e_cond_cm_pipe == CM_COND_OPENED)
            {
                i4_ret = _rec_svc_state_conning_util_open_conn (pt_svctx);
                
                if (RET_FAIL (i4_ret))
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open conn by util (%d). (L%d)\n", i4_ret, __LINE__));
                    break;
                }
            }
            else
            {
                i4_ret = _rec_svc_state_conning_util_open_pipe (pt_svctx);
                
                if (RET_FAIL (i4_ret))
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open pipe by util (%d). (L%d)\n", i4_ret, __LINE__));
                    break;
                }
            }
        }
                                            
    } while (0);
    
    if (RET_FAIL (i4_ret))
    {
        /* return FAIL to enter REC_SVC_STATE_ERROR state */
        /* let temporal resources be freed in REC_SVC_STATE_ERROR state */
    }
    
    return i4_ret;
}

static INT32 _rec_svc_state_transit_force_conned (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_STATE_T            e_state_old;
    INT32                              i4_ret;
    
    /* check previous state and preconditions */
    if  ( pt_running_svc_req->e_state != REC_SVC_STATE_DISCONNECTING   && 
          pt_running_svc_req->e_state != REC_SVC_STATE_DISCONNECTED    && 
          pt_running_svc_req->e_state != REC_SVC_STATE_NOT_INIT       && 
          pt_running_svc_req->e_state != REC_SVC_STATE_CONNECTING   && 
          pt_running_svc_req->e_state != REC_SVC_STATE_WAIT_RECONN && 
          pt_running_svc_req->e_state != REC_SVC_STATE_ERROR )
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong state (%d) when transiting to CONNECTED. (L%d)\n", pt_running_svc_req->e_state, __LINE__));

        return SVCTXR_REC_INTERNAL_ERROR;
    }

    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] State Transition: %s(%d) -> %s(%d). (L%d)\n", 
                      as_g_state_name[pt_running_svc_req->e_state],
                      pt_running_svc_req->e_state,
                      as_g_state_name[REC_SVC_STATE_CONNECTED],
                      REC_SVC_STATE_CONNECTED,
                      __LINE__));

    /*1. state change*/
    e_state_old                                      = pt_running_svc_req->e_state;
    pt_running_svc_req->e_state              = REC_SVC_STATE_CONNECTED;
    pt_running_svc_req->ui4_st_flag          = 0;
    pt_running_svc_req->b_pipe_real_close = TRUE; 
    pt_running_svc_req->b_cm_real_disc    = TRUE;
    
    /*2.*/
    if (e_state_old == REC_SVC_STATE_NOT_INIT)
    {
        /*not int + b_t_conn_info_preset valid + not use  t_conn_info_preset  
          1) so current cm pipe is null , restore running pt_running_svc_req's  h_conn ,h_cm_pipe, e_cond_cm_pipe !!!
       */ 
        if (pt_running_svc_req->b_preset_conn_info)
        {
            /*b_preset_conn_info and t_conn_info_preset are  set only when pt_running_svc_req-->pt_pending_svc_req
            for pipe reuse, and when pending req becomes running one it will reuse the preset pipe and cm handles followed
             by clearing t_conn_info_preset. This case indicates the following case:
            <1> pt_running_svc_req: < connected transition-->connecting-->disconnected-->not init>
            <2>when at not init : set pt_pending_svc_req's t_conn_info_preset and b_preset_conn_info by t_conn_info_reuse
            <3>pt_pending_svc_req-->pt_running_svc_req but there is no the following expected sequence:
            <4> < not init-->disconnected-->connecting-->connected transition>
            */ 
            DBG_INFO(("{SVCTX}  INFO: [REC_HDLR] repeated stop req but need release previous reused pipe!!!. (L%d)\n", __LINE__));
            if (x_handle_valid(pt_running_svc_req->t_conn_info_preset.h_conn))
            {
                pt_running_svc_req->h_conn         = pt_running_svc_req->t_conn_info_preset.h_conn;
                pt_running_svc_req->e_cond_conn =  CM_COND_CONNECTED;
                
                DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] need  disconnet reused connection (%d). (L%d)\n",
                                   pt_running_svc_req->h_conn, __LINE__));
            }
            else
            {
                DBG_ASSERT (NULL_HANDLE,   DBG_MOD_SVCTX);
            }

            if (x_handle_valid(pt_running_svc_req->t_conn_info_preset.h_cm_pipe))
            {
                pt_running_svc_req->h_cm_pipe         = pt_running_svc_req->t_conn_info_preset.h_cm_pipe;
                pt_running_svc_req->e_cond_cm_pipe = CM_COND_OPENED;
                
                DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] need  disconnet reused pipe (%d). (L%d)\n", 
                                  pt_running_svc_req->h_cm_pipe, __LINE__));
            }
            else
            {
                DBG_ASSERT (NULL_HANDLE,   DBG_MOD_SVCTX);
            }
        }
    }
    
    /*3.*/
    if ((e_state_old == REC_SVC_STATE_CONNECTING)
         || (e_state_old == REC_SVC_STATE_WAIT_RECONN)
         || (e_state_old == REC_SVC_STATE_ERROR))
    {
          /*connecting + running req's  cm handle , pipe handle are possible valid 
           1)if h_conn is not valid need restore it by pt_running_svc_req->t_conn_info_preset.h_conn
           2)if if h_cm_pipe  is not valid need restore it by pt_running_svc_req->t_conn_info_preset.h_cm_pipe
              so does e_cond_cm_pipe
           3) need remove any queued message!!!
         */
         /*restore h_conn if possible*/
          
         UINT8   ui1_msg_temp;
         if (!(x_handle_valid(pt_running_svc_req->h_conn)))
         {
             if (x_handle_valid(pt_running_svc_req->t_conn_info_preset.h_conn))
             {
                 pt_running_svc_req->h_conn         = pt_running_svc_req->t_conn_info_preset.h_conn;
                 pt_running_svc_req->e_cond_conn =  CM_COND_CONNECTED;
                 
                 DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] need  disconnet reused connection (%d). (L%d)\n",
                                    pt_running_svc_req->h_conn, __LINE__));
             }
             else if (x_handle_valid(pt_running_svc_req->t_conn_info_reuse.h_conn))
             {
                 pt_running_svc_req->h_conn         = pt_running_svc_req->t_conn_info_reuse.h_conn;
                 pt_running_svc_req->e_cond_conn =  CM_COND_CONNECTED;
                 
                 DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] need  disconnet reused connection (%d). (L%d)\n",
                                    pt_running_svc_req->h_conn, __LINE__));
             }
             else
             {
                 DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] connection (%d). (L%d)\n", 
                                pt_running_svc_req->h_conn, __LINE__));
             }
         }
     
         /*restore h_cm_pipe if possible*/
        if (!(x_handle_valid(pt_running_svc_req->h_cm_pipe)))
        {
            if (x_handle_valid(pt_running_svc_req->t_conn_info_preset.h_cm_pipe))
            {
                pt_running_svc_req->h_cm_pipe         = pt_running_svc_req->t_conn_info_preset.h_cm_pipe;
                pt_running_svc_req->e_cond_cm_pipe = CM_COND_OPENED;

                DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] need  disconnet reused pipe (%d). (L%d)\n", 
                               pt_running_svc_req->h_cm_pipe, __LINE__));
            }
            else if (x_handle_valid(pt_running_svc_req->t_conn_info_reuse.h_cm_pipe))
            {
                pt_running_svc_req->h_cm_pipe         = pt_running_svc_req->t_conn_info_reuse.h_cm_pipe;
                pt_running_svc_req->e_cond_cm_pipe = CM_COND_OPENED;

                DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] need  disconnet reused pipe (%d). (L%d)\n", 
                               pt_running_svc_req->h_cm_pipe, __LINE__));
            }
            else
            {
                DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] pipe (%d). (L%d)\n", 
                               pt_running_svc_req->h_cm_pipe, __LINE__));
            }
        }

        /*remove queued message*/
        while ((pt_running_svc_req->ui2_num_act_msg != 0))
        {
            i4_ret = _rec_svc_state_act_msg_q_remove (pt_running_svc_req, &ui1_msg_temp);
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to remove act ret=%d. (L%d)\n", i4_ret, __LINE__));
            }
            
            DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] remove act msg (%d). (L%d)\n", ui1_msg_temp, __LINE__));
        }
    }

    /*4.*/
    if (e_state_old == REC_SVC_STATE_DISCONNECTING)
    {
         /*disconnecting + running req's  cm handle , pipe handle are possible valid 
          1)if h_conn is not valid need restore it by pt_running_svc_req->t_conn_info_reuse.h_conn
          2)if if h_cm_pipe  is not valid need restore it by pt_running_svc_req->t_conn_info_reuse.h_cm_pipe
             so does e_cond_cm_pipe
          3) need remove any queued message!!!
        */
    
        /*restore h_conn if possible*/
         
         UINT8   ui1_msg_temp;
        if (!(x_handle_valid(pt_running_svc_req->h_conn)))
        {
            if (x_handle_valid(pt_running_svc_req->t_conn_info_reuse.h_conn))
            {
                pt_running_svc_req->h_conn         = pt_running_svc_req->t_conn_info_reuse.h_conn;
                pt_running_svc_req->e_cond_conn =  CM_COND_CONNECTED;

                DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] need  disconnet reused connection (%d). (L%d)\n",
                             pt_running_svc_req->h_conn, __LINE__));
            }
            else
            {
                DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] connection (%d). (L%d)\n", 
                               pt_running_svc_req->h_conn, __LINE__));
            }
      
        }

        /*restore h_cm_pipe if possible*/
        if (!(x_handle_valid(pt_running_svc_req->h_cm_pipe)))
        {
            if (x_handle_valid(pt_running_svc_req->t_conn_info_reuse.h_cm_pipe))
            {
                pt_running_svc_req->h_cm_pipe         = pt_running_svc_req->t_conn_info_reuse.h_cm_pipe;
                pt_running_svc_req->e_cond_cm_pipe = CM_COND_OPENED;

                DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] need  disconnet reused pipe (%d). (L%d)\n", 
                                   pt_running_svc_req->h_cm_pipe, __LINE__));
            }
            else
            {
                DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] pipe (%d). (L%d)\n", 
                                   pt_running_svc_req->h_cm_pipe, __LINE__));
            }
        }

        /*remove queued message*/
        while ((pt_running_svc_req->ui2_num_act_msg != 0))
        {
            i4_ret = _rec_svc_state_act_msg_q_remove (pt_running_svc_req, &ui1_msg_temp);
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to remove act ret=%d. (L%d)\n", i4_ret, __LINE__));
            }
            DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] remove act msg (%d). (L%d)\n", ui1_msg_temp, __LINE__));
        }
    }

    /*5.*/
    if (e_state_old == REC_SVC_STATE_DISCONNECTED)
    {
         /*disconnected + pt_running_svc_req->t_conn_info_reuse  is valid 
         1) there is not queued message 
         2) so need restore h_conn ,h_cm_pipe, e_cond_cm_pipe  by pt_running_svc_req->t_conn_info_reuse!!!!!
        */
         /*restore h_conn if possible*/
         if (!(x_handle_valid(pt_running_svc_req->h_conn)))
         {
             if (x_handle_valid(pt_running_svc_req->t_conn_info_reuse.h_conn))
             {
                 pt_running_svc_req->h_conn         = pt_running_svc_req->t_conn_info_reuse.h_conn;
                 pt_running_svc_req->e_cond_conn =  CM_COND_CONNECTED;
         
                 DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] need  disconnet reused connection (%d). (L%d)\n",
                              pt_running_svc_req->h_conn, __LINE__));
             }
             else
             {
                 DBG_ASSERT (NULL_HANDLE,   DBG_MOD_SVCTX);
             }
         
         }
             
         /*restore h_cm_pipe if possible*/
         if (!(x_handle_valid(pt_running_svc_req->h_cm_pipe)))
         {
             if (x_handle_valid(pt_running_svc_req->t_conn_info_reuse.h_cm_pipe))
             {
                 pt_running_svc_req->h_cm_pipe         = pt_running_svc_req->t_conn_info_reuse.h_cm_pipe;
                 pt_running_svc_req->e_cond_cm_pipe = CM_COND_OPENED;
         
                 DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] need  disconnet reused pipe (%d). (L%d)\n", 
                             pt_running_svc_req->h_cm_pipe, __LINE__));
             }
             else
             {
                 DBG_ASSERT (NULL_HANDLE,   DBG_MOD_SVCTX);
             }
         }
     
    }

    /*6. need clear t_conn_info_preset? It seem not necessary*/
   #if 0
    x_memset (& pt_running_svc_req->t_conn_info_preset, 0, sizeof (REC_SVC_CONN_INFO_T));
    pt_running_svc_req->b_preset_conn_info = FALSE;
    #endif

    /*remove all events*/
    DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] clean event queue . (L%d)\n", __LINE__));
    _rec_svc_state_clear_event_q (pt_running_svc_req);
    
    return SVCTXR_OK;
}


static INT32 _rec_svc_state_transit_conned (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_STATE_T             e_state_old;
    REC_SVC_SRC_BRDCST_SVL_T*   pt_src_brdcst_svl;
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl;
    SVCTX_MSG_T                 t_msg;
    HANDLE_T                    h_rrctx = NULL_HANDLE;
    INT32                       i4_ret;
    
    /* check previous state and preconditions */
    if ((pt_running_svc_req->e_state != REC_SVC_STATE_CONNECTING   && 
         pt_running_svc_req->e_state != REC_SVC_STATE_SESS_CLOSING && 
         pt_running_svc_req->e_state != REC_SVC_STATE_SESS_ERROR   )  ||
        (pt_running_svc_req->e_cond_cm_pipe != CM_COND_OPENED  && 
         pt_running_svc_req->e_cond_conn != CM_COND_CONNECTED)            )
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong state (%d) when transiting to CONNECTED. (L%d)\n", pt_running_svc_req->e_state, __LINE__));

        return SVCTXR_REC_INTERNAL_ERROR;
    }

    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] State Transition: %s(%d) -> %s(%d). (L%d)\n", 
                as_g_state_name[pt_running_svc_req->e_state],
                pt_running_svc_req->e_state,
                as_g_state_name[REC_SVC_STATE_CONNECTED],
                REC_SVC_STATE_CONNECTED,
                __LINE__));

    e_state_old = pt_running_svc_req->e_state;
    pt_running_svc_req->e_state = REC_SVC_STATE_CONNECTED;
    pt_running_svc_req->ui4_st_flag = 0;

    /* get src-brdcst-svl */
    i4_ret = _rec_svc_util_get_src_brdcst_svl (pt_running_svc_req,
                                               & pt_src_brdcst_svl);
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get SRC_BRDCST_SVL (%d). (L%d)\n", i4_ret, __LINE__));
        pt_src_brdcst_svl = NULL;
        i4_ret = SVCTXR_OK;
    }
    
    /* get res-brdcst-svl */
    i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req,
                                               & pt_res_brdcst_svl);

    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Not supported SRC_TYPE. (L%d)\n", __LINE__));
        pt_res_brdcst_svl = NULL;
        i4_ret = SVCTXR_OK;
    }                                                
    
    if (e_state_old == REC_SVC_STATE_CONNECTING)
    {
        CRNT_EVENT_T*   pt_crnt_evn;
        HANDLE_T        h_scdb;
        
        rec_hdlr_invoke_client_svc_nfy_fct (pt_svctx,
                                            SVCTX_NTFY_CODE_SIGNAL_LOCKED,
                                            ST_UNKNOWN );
        
        /* open rrctx for ATSC RRT */
        h_rrctx = NULL_HANDLE;
        
        if (pt_res_brdcst_svl &&
            pt_res_brdcst_svl->e_brdcst_type == BRDCST_TYPE_ATSC)
        {
            i4_ret = x_rrctx_open_rating (RRCTX_RATING_TYPE_RRT,
                                          NULL,
                                          _rec_svc_cb_rrctx_nfy,
                                          (VOID *) pt_svctx->h_sys_svctx,
                                          & h_rrctx);
            if (i4_ret != RRCTXR_OK)
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open rrctx (%d). (L%d)\n", i4_ret, __LINE__));
                return SVCTXR_FAIL;
            }
            
            pt_running_svc_req->h_rrctx = h_rrctx;
        }
        else
        {
            pt_running_svc_req->h_rrctx = NULL_HANDLE;
        }
        
        /* open current event */
        pt_crnt_evn = NULL;
        i4_ret = crnt_evn_open (DEFAULT_REQUERY_EVN_DELAY,
                                TRUE,
                                _rec_svc_msg_hdlr_crnt_evn_nfy,
                                (VOID *) pt_svctx,
                                & pt_crnt_evn);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Open current event error (%d). (L%d)\n", i4_ret, __LINE__));
            return SVCTXR_FAIL;
        }
        
        pt_running_svc_req->pt_crnt_evn = pt_crnt_evn;
        
        /* start query event */
        if (pt_src_brdcst_svl  &&
            pt_res_brdcst_svl  &&
            pt_res_brdcst_svl->e_brdcst_type != BRDCST_TYPE_ANALOG)
        {
            CRNT_EVENT_KEY_T    t_key;
            
            t_key.t_svl.ui2_svc_lst_id = pt_src_brdcst_svl->t_param.ui2_svc_lst_id;
            t_key.t_svl.ui2_svl_rec_id = pt_src_brdcst_svl->t_param.ui2_svl_rec_id;
#ifdef MW_EDB_SUPPORT                
	        t_key.t_svl.ui4_channel_id = pt_src_brdcst_svl->t_param.ui4_channel_id;
#endif
            
            /* start query event */
            i4_ret = crnt_evn_start_query (pt_running_svc_req->pt_crnt_evn,
                                           DEV_TUNER, /* pt_res_brdcst_svl != NULL so must be TUNER */
                                           pt_running_svc_req->s_src_grp_name,
                                           pt_running_svc_req->h_conn,
                                           & t_key );
            if (RET_OK (i4_ret))
            {
                DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Start query event (h_event=%d, evctx_cond=%d). (L%d)\n", 
                           crnt_evn_get_handle (pt_running_svc_req->pt_crnt_evn),
                           crnt_evn_get_cond (pt_running_svc_req->pt_crnt_evn),
                           __LINE__));
            }
            else
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to start query event (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
        
        /* open SCDB */
        i4_ret = x_scdb_open (pt_running_svc_req->h_conn,
                              & h_scdb,
                              (VOID *) pt_svctx->h_sys_svctx,
                              _rec_svc_cb_scdb_nfy );
    
        if (i4_ret != SCDBR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open SCDB (%d). (L%d)\n", i4_ret, __LINE__));
            return SVCTXR_FAIL;
        }
        
        pt_running_svc_req->h_scdb      = h_scdb;
        pt_running_svc_req->e_cond_scdb = SCDB_CLOSED;
        
        /* send a fake SCDB update message anyway */
        t_msg.h_svctx             = pt_svctx->h_sys_svctx;
        t_msg.ui1_msg_id          = SVCTX_MSG_REC_SCDB_NFY;
        t_msg.pf_invoke_func      = NULL;
        t_msg.pv_tag              = NULL;
        t_msg.u.t_scdb.h_obj      = h_scdb;
        t_msg.u.t_scdb.e_cond     = SCDB_UPDATED;
        t_msg.u.t_scdb.ui4_reason = (SCDB_RECORD_ADD | SCDB_RECORD_DEL | SCDB_RECORD_MOD | SCDB_COMMON_MOD);
        t_msg.u.t_scdb.ui4_data   = 0;
        
        i4_ret = svctx_send_msg (pt_svctx->h_sys_svctx,
                                 & t_msg );
                                 
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send message. (L%d)\n", __LINE__));
        }
    
        /* open PAT */
        if (pt_res_brdcst_svl  &&
            pt_res_brdcst_svl->e_brdcst_type != BRDCST_TYPE_ANALOG)
        {    
            HANDLE_T                    h_tm_root, h_pat;
            TM_COND_T                   e_cond_pat;
            SVCTX_MSG_T                 t_msg;
            SVCTX_REC_PAT_NFY_MSG_T*    pt_msg_pat;
                
            /* open TM root */
            i4_ret = x_tm_open_root (REC_SVC_TM_ROOT_NAME,
                                     TM_SRC_TYPE_CONN_HANDLE,
                                     (VOID *) pt_running_svc_req->h_conn,
                                     NULL,
                                     NULL, 
                                     & h_tm_root,
                                     NULL );
            
            if (i4_ret != TMR_OK)
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open TM Root (%d). (L%d)\n", i4_ret, __LINE__));
                DBG_ABORT (DBG_MOD_SVCTX);
                return SVCTXR_FAIL;
            }
            
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Open TM Root (%d). (L%d)\n", 
                        h_tm_root,
                        __LINE__));
            
            pt_res_brdcst_svl->h_tm_root = h_tm_root;
            
            /* load PAT */
            i4_ret = x_psi_load_pat (pt_res_brdcst_svl->h_tm_root,
                                     TM_SRC_TYPE_SAME_AS_ROOT,
                                     NULL,
                                     (VOID *) pt_svctx->h_sys_svctx,
                                     _rec_svc_cb_load_pat_nfy,
                                     & h_pat,
                                     & e_cond_pat );
            
            if (i4_ret != TMR_OK)
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to load PAT (%d). (L%d)\n", i4_ret, __LINE__));
                return SVCTXR_FAIL;
            }
            
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Load PAT (%d) (cond=%d). (L%d)\n", 
                        h_pat,
                        e_cond_pat,
                        __LINE__));
            
            pt_res_brdcst_svl->h_pat      = h_pat;
            pt_res_brdcst_svl->e_cond_pat = e_cond_pat;
            
            /* send message for loading PAT */
            t_msg.h_svctx              = pt_svctx->h_sys_svctx;
            t_msg.ui1_msg_id           = SVCTX_MSG_REC_PAT_NFY;
            t_msg.pf_invoke_func       = NULL;
            t_msg.pv_tag               = NULL;
            t_msg.u.t_hdlr.z_len_data  = sizeof (SVCTX_REC_PAT_NFY_MSG_T);
        
            pt_msg_pat = (SVCTX_REC_PAT_NFY_MSG_T *) t_msg.u.t_hdlr.aui1_data;
            
            pt_msg_pat->h_pat      = h_pat;
            pt_msg_pat->e_obj_type = TMT_PSI_PAT;
            pt_msg_pat->e_cond     = e_cond_pat;
            pt_msg_pat->ui4_data   = 0;
            
            i4_ret = svctx_send_msg (pt_svctx->h_sys_svctx,
                                     & t_msg );
                                     
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send message. (L%d)\n", __LINE__));
                return SVCTXR_FAIL;
            }
        }
    }
    else /* if (e_state_old == REC_SVC_STATE_SESS_CLOSING || REC_SVC_STATE_SESS_ERROR) */
    {
        if (pt_running_svc_req->h_rcrd_sess != NULL_HANDLE  ||
            pt_running_svc_req->e_cond_sess != SM_SESS_COND_CLOSED)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Record Session is not completely closed. (L%d)\n", __LINE__));
            
            pt_running_svc_req->h_rcrd_sess = NULL_HANDLE;
            pt_running_svc_req->e_cond_sess = SM_SESS_COND_CLOSED;
        }
        
        if (!DLIST_IS_EMPTY (& pt_running_svc_req->t_wking_strms))
        {
            REC_SVC_STRM_INFO_T*    pt_strm;
            
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Record Stream are not all closed. (L%d)\n", __LINE__));
            
            pt_strm = DLIST_HEAD (& pt_running_svc_req->t_wking_strms);
            while (pt_strm)
            {
                i4_ret = _rec_svc_state_util_remove_strm (pt_running_svc_req,
                                                          pt_strm->h_strm);
                if (RET_FAIL (i4_ret))
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to remove stream (%d). (L%d)\n", i4_ret, __LINE__));
                }
                
                pt_strm = DLIST_HEAD (& pt_running_svc_req->t_wking_strms);
            }
        }
        
        /* check if need to delete */
        /* >//TODO */
        
        /* notify record session is closed */
        
        if (pt_running_svc_req->pf_rec_nfy)
        {
            if(pt_running_svc_req->pt_rec_cfg != NULL)
            {
                pt_running_svc_req->pf_rec_nfy (pt_running_svc_req->pt_rec_cfg->h_svctx_nfy,
                                                pt_running_svc_req->pv_rec_nfy_tag,
                                                SVCTX_NTFY_CODE_REC_STOPPED,
                                                0 );
            }
        }
        else
        {
            DBG_ABORT (DBG_MOD_SVCTX);
        }  

        /* it's the time to free rec_cfg & clear pf_rec_nfy */
        if (pt_running_svc_req->pt_rec_cfg)
        {
            _rec_svc_util_mem_free (pt_running_svc_req,
                                    pt_running_svc_req->pt_rec_cfg);
                                    
            pt_running_svc_req->pt_rec_cfg = NULL;
        }
        pt_running_svc_req->pf_rec_nfy     = NULL;
        pt_running_svc_req->pv_rec_nfy_tag = NULL;
        pt_running_svc_req->ui4_sess_flag  = 0;
        
        /* deinit the media type related info */
        switch (pt_running_svc_req->e_media_type)
        {
        case REC_SVC_MEDIA_TYPE_PVR:
            pt_running_svc_req->t_pvr.b_got_fst_evn_ttl = FALSE;
            pt_running_svc_req->t_pvr.z_fst_evn_ttl = 0;
            break;
        
        case REC_SVC_MEDIA_TYPE_OTHER:
            break;
        
        default:
            break;
        }
        pt_running_svc_req->e_media_type = REC_SVC_MEDIA_TYPE_INVALID;

        rec_hdlr_invoke_client_svc_nfy_fct (pt_svctx,
                                            SVCTX_NTFY_CODE_REC_STOPPED,
                                            ST_UNKNOWN );
    
    }
     
    return SVCTXR_OK;
}    

static INT32 _rec_svc_state_transit_disconning (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl;
    INT32                       i4_ret;
    
    /* check previous state and preconditions */
    if (pt_running_svc_req->e_state != REC_SVC_STATE_CONNECTED && pt_running_svc_req->e_state != REC_SVC_STATE_CONNECTING)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong state (%d) when transiting to DISCONNECTING. (L%d)\n", pt_running_svc_req->e_state, __LINE__));

        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] State Transition: %s(%d) -> %s(%d). (L%d)\n", 
                as_g_state_name[pt_running_svc_req->e_state],
                pt_running_svc_req->e_state,
                as_g_state_name[REC_SVC_STATE_DISCONNECTING],
                REC_SVC_STATE_DISCONNECTING,
                __LINE__));
    
    pt_running_svc_req->e_state = REC_SVC_STATE_DISCONNECTING;
    pt_running_svc_req->ui4_st_flag = 0;
    if (pt_running_svc_req->pt_crnt_evn)
    {
        /* stop querying event */
        i4_ret = crnt_evn_stop_query (pt_running_svc_req->pt_crnt_evn);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to stop query CRNT_EVN (%d). (L%d)\n", i4_ret, __LINE__));
        }
           
        /* close event */
        i4_ret = crnt_evn_close (pt_running_svc_req->pt_crnt_evn);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close CRNT_EVN (%d). (L%d)\n", i4_ret, __LINE__));
        }
        pt_running_svc_req->pt_crnt_evn = NULL;
    }
    /* close SCDB */
    if (x_handle_valid (pt_running_svc_req->h_scdb))
    {
        i4_ret = x_scdb_close (pt_running_svc_req->h_scdb);
        
        if (i4_ret != SCDBR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close SCDB (%d). (L%d)\n", i4_ret, __LINE__));
        }
    }
    pt_running_svc_req->h_scdb = NULL_HANDLE;
    
    /* free RRCTX resources */
    if (pt_running_svc_req->h_rrctx != NULL_HANDLE)
    {
        i4_ret = x_rrctx_close_rating (pt_running_svc_req->h_rrctx);
        
        if (i4_ret != RRCTXR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close rrctx (%d). (L%d)\n", i4_ret, __LINE__));
        }
        
        pt_running_svc_req->h_rrctx = NULL_HANDLE;
    }
    
    /* free RRCTX buffer */
    if (pt_running_svc_req->pv_buf_rrctx != NULL)
    {
        x_mem_free (pt_running_svc_req->pv_buf_rrctx);
        pt_running_svc_req->pv_buf_rrctx    = NULL;
        pt_running_svc_req->z_len_buf_rrctx = 0;
    }

    i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req,
                                               & pt_res_brdcst_svl);
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Not supported SRC_TYPE. (L%d)\n", __LINE__));
        pt_res_brdcst_svl = NULL;
        i4_ret = SVCTXR_OK;
    }    

    if (pt_res_brdcst_svl)
    {
        /* free PAT related resources */
        i4_ret = _rec_svc_state_util_free_pat_res (pt_svctx,
                                                   pt_running_svc_req,
                                                   pt_res_brdcst_svl);
        
        if (i4_ret != TMR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to free PAT (%d). (L%d)\n", i4_ret, __LINE__));
        }
    }
    
    /* disconnect connection */
    i4_ret = _rec_svc_state_util_close_conn (
            pt_svctx, 
            TRUE,
            (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_INIT_SVC) ? NOT_ALSO_PIPE : ALSO_PIPE,
            NULL);
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to disconnet connection (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    return SVCTXR_OK;    
}    

static INT32 _rec_svc_state_transit_wait_reconn (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*      pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32               i4_ret;
    
    if (!(pt_running_svc_req->e_state == REC_SVC_STATE_CONNECTING    ||
          pt_running_svc_req->e_state == REC_SVC_STATE_DISCONNECTING ||
          pt_running_svc_req->e_state == REC_SVC_STATE_ERROR))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong state (%d) when transiting to WAIT_RECONN. (L%d)\n", 
                    pt_running_svc_req->e_state, 
                    __LINE__));

        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    if (pt_running_svc_req->e_src_op_type != REC_SVC_SRC_OP_TYPE_INIT_SVC)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong source oper (%d) when transiting to WAIT_RECONN. (L%d)\n", 
                    pt_running_svc_req->e_src_op_type, 
                    __LINE__));

        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] State Transition: %s(%d) -> %s(%d). (L%d)\n", 
                as_g_state_name[pt_running_svc_req->e_state],
                pt_running_svc_req->e_state,
                as_g_state_name[REC_SVC_STATE_WAIT_RECONN],
                REC_SVC_STATE_WAIT_RECONN,
                __LINE__));
    
    pt_running_svc_req->e_state = REC_SVC_STATE_WAIT_RECONN;
    pt_running_svc_req->ui4_st_flag = 0;
    
    if (pt_running_svc_req->b_on_timer_conn)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        
        i4_ret = x_timer_stop (pt_running_svc_req->h_timer_conn);
        
        if (i4_ret != OSR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to stop timer (%d). (L%d)\n", i4_ret, __LINE__));
        }
        
        pt_running_svc_req->b_on_timer_conn = FALSE;
    }
    
    i4_ret = x_timer_start (pt_running_svc_req->h_timer_conn,
                            REC_SVC_RECONN_DELAY,
                            X_TIMER_FLAG_ONCE,
                            _rec_svc_cb_timer_reconn_nfy,
                            (VOID *) pt_svctx->h_sys_svctx );
    if (i4_ret == OSR_OK)
    {
        pt_running_svc_req->b_on_timer_conn = TRUE;
        i4_ret = SVCTXR_OK;
    }
    else
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to start reconn timer (%d). (L%d)\n", i4_ret, __LINE__));
        i4_ret = SVCTXR_FAIL;
    }
    
    return i4_ret;
}    

static INT32 _rec_svc_state_transit_sess_opening (
    SVCTX_T*                              pt_svctx,
    const SVCTX_REC_REQ_START_REC_MSG_T*  pt_msg_info)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_UTIL_CONFIG_T           t_ru_cfg;
    HANDLE_T                    h_rec_util;
    SVCTX_REC_STRG_DESC_T*      pt_strg_desc;
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl = NULL;
    UINT32                      ui4_req_id;
    BOOL                        b_pvr_file;
    INT32                       i4_ret;
    
    /* check previous state and preconditions */
    if (pt_running_svc_req->e_state != REC_SVC_STATE_CONNECTED)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong state (%d) when transiting to SESS_OPENING. (L%d)\n", pt_running_svc_req->e_state, __LINE__));

        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    /* make sure the precondition before session opening */
    if (!(pt_running_svc_req->e_cond_cm_pipe == CM_COND_OPENED &&
          pt_running_svc_req->e_cond_conn    == CM_COND_CONNECTED ))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Incorrect condition before opening sess (%d/%d). (L%d)\n", 
                    pt_running_svc_req->e_cond_cm_pipe,
                    pt_running_svc_req->e_cond_conn,
                    __LINE__));
        
        return SVCTXR_REC_INTERNAL_ERROR;
    }

    if (!(pt_running_svc_req->pt_rec_cfg == NULL             &&
          pt_running_svc_req->h_rcrd_sess == NULL_HANDLE     &&
          pt_running_svc_req->e_cond_sess == SM_SESS_COND_CLOSED))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    if (!DLIST_IS_EMPTY (& pt_running_svc_req->t_wking_strms))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }

    i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req,
                                               & pt_res_brdcst_svl);
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Not supported SRC_TYPE. (L%d)\n", __LINE__));
        pt_res_brdcst_svl = NULL;
        i4_ret = SVCTXR_OK;
    }                                                
    
    /* check PAT condition */
    if (pt_res_brdcst_svl &&
        !REC_SVC_TABLE_AVAIL (pt_res_brdcst_svl->e_cond_pat))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] PAT not available (%d) when transiting to SESS_OPENING. (L%d)\n", 
                    pt_res_brdcst_svl->e_cond_pat, 
                    __LINE__));
        return SVCTXR_WRONG_STATE;
    }
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] State Transition: %s(%d) -> %s(%d). (L%d)\n", 
                as_g_state_name[pt_running_svc_req->e_state],
                pt_running_svc_req->e_state,
                as_g_state_name[REC_SVC_STATE_SESS_OPENING],
                REC_SVC_STATE_SESS_OPENING,
                __LINE__));
    
    pt_running_svc_req->e_state = REC_SVC_STATE_SESS_OPENING;
    pt_running_svc_req->ui4_st_flag   = 0;
    pt_running_svc_req->ui4_sess_flag = 0;
    
    do
    {
        
        /* init res_brdcst_svl */
        if (pt_res_brdcst_svl)
        {
            pt_res_brdcst_svl->t_es_list.ui2_es_num         = 0;
            pt_res_brdcst_svl->t_es_list.ui2_idx_prim_video = SVCTX_REC_ES_LIST_IDX_NULL;
            pt_res_brdcst_svl->b_es_list_pding                    = FALSE;
            pt_res_brdcst_svl->t_es_list_pding.ui2_es_num         = 0;
            pt_res_brdcst_svl->t_es_list_pding.ui2_idx_prim_video = SVCTX_REC_ES_LIST_IDX_NULL;
        }
        
        pt_running_svc_req->pt_rec_cfg = (SVCTX_REC_CONFIG_T *) _rec_svc_util_mem_alloc (
                                                                    pt_running_svc_req,
                                                                    pt_msg_info->z_len_cfg);
        
        if (pt_running_svc_req->pt_rec_cfg == NULL)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Out of memory. (L%d)\n", __LINE__));
            i4_ret = SVCTXR_NO_RESOURCE;
            break;
        }
        
        i4_ret = _rec_svc_tool_copy_rec_cfg (pt_running_svc_req->pt_rec_cfg,
                                             pt_msg_info->z_len_cfg,
                                             pt_msg_info->pt_rec_cfg);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to copy rec cfg. (L%d)\n", __LINE__));
            break;
        }
        
        pt_running_svc_req->pf_rec_nfy     = pt_msg_info->pt_rec_cfg->pf_rec_nfy;
        pt_running_svc_req->pv_rec_nfy_tag = pt_msg_info->pt_rec_cfg->pv_nfy_tag;
        
        /* session flag */
        i4_ret = svctx_rec_check_pvr_file (pt_running_svc_req->pt_rec_cfg->t_strg_desc.s_strg_path,
                                           & b_pvr_file);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to judge pvr file (%d). (L%d)\n", i4_ret, __LINE__));
            break;
        }
        
        pt_running_svc_req->e_media_type = b_pvr_file ? REC_SVC_MEDIA_TYPE_PVR : REC_SVC_MEDIA_TYPE_OTHER;        
                /* media specific init */
        switch (pt_running_svc_req->e_media_type)
        {
        case REC_SVC_MEDIA_TYPE_PVR:
            pt_running_svc_req->t_pvr.b_got_fst_evn_ttl = FALSE;
            pt_running_svc_req->t_pvr.z_fst_evn_ttl = 0;
            break;
            
        case REC_SVC_MEDIA_TYPE_OTHER:
            /* do nothing for t_other */
            break;
            
        default:
            DBG_ABORT (DBG_MOD_SVCTX);
        }

        /* generage session key */
        _rec_svc_util_gen_sess_crypt_key (pt_running_svc_req,
                                          & pt_running_svc_req->t_sess_key);
        
        /* check if send request of set es list to app */
        if (pt_running_svc_req->e_cond_scdb == SCDB_UPDATED)
        {
            if (pt_running_svc_req->pf_rec_nfy)
            {
                if(pt_running_svc_req->pt_rec_cfg != NULL)
                {
                     pt_running_svc_req->pf_rec_nfy (pt_running_svc_req->pt_rec_cfg->h_svctx_nfy,
                                                pt_running_svc_req->pv_rec_nfy_tag,
                                                SVCTX_NTFY_CODE_REC_REQ_ES_LIST,
                                                0 );
                }
            }
            else
            {
                DBG_ABORT (DBG_MOD_SVCTX);
            }  
        }
        
        /* open record utility */
        i4_ret = rec_util_open (pt_running_svc_req->pt_rec_cfg->s_db_name,
                                _rec_svc_cb_rec_util_nfy,
                                (VOID *) pt_svctx->h_sys_svctx,
                                & h_rec_util);
    
        if (i4_ret != REC_UTILR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open Record Utility (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_FAIL;
            break;
        }
        
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Open REC_UTIL (%d). (L%d)\n", 
                    h_rec_util,
                    __LINE__));
        
        pt_running_svc_req->h_rec_util      = h_rec_util;
        pt_running_svc_req->e_cond_rec_util = REC_UTIL_COND_TRANSITION;

        pt_strg_desc = & pt_running_svc_req->pt_rec_cfg->t_strg_desc;
        
        /* init Record Utility config */
        t_ru_cfg.ui4_flags = 0;
        
        switch (pt_running_svc_req->pt_rec_cfg->e_buf_mode)
        {
        case SVCTX_REC_BUF_MODE_PLAIN:
            t_ru_cfg.e_buf_type = REC_UTIL_BUF_TYPE_PLAIN;
            break;
            
        case SVCTX_REC_BUF_MODE_RING:
            t_ru_cfg.e_buf_type = REC_UTIL_BUF_TYPE_RING;
            break;
        
        default:
            DBG_ABORT (DBG_MOD_SVCTX);
        }
        
        switch (pt_running_svc_req->pt_rec_cfg->e_bound_type)
        {
        case SVCTX_REC_BOUND_TYPE_TIME:
        {
            SVCTX_REC_TIME_BOUND_INFO_T*  pt_time_bound = 
                    (SVCTX_REC_TIME_BOUND_INFO_T *) pt_running_svc_req->pt_rec_cfg->pv_bound_info;
            
            t_ru_cfg.e_bound_type                       = REC_UTIL_BOUND_TYPE_TIME;
            t_ru_cfg.u_data.t_time_bound_info.ui4_flags = 0;
            t_ru_cfg.u_data.t_time_bound_info.ui4_ticks = pt_time_bound->ui4_ticks;
            
            t_ru_cfg.u_data.t_time_bound_info.ui8_tsb_sec_ns = 
                pt_time_bound->ui8_tsb_cap / ((UINT64) pt_strg_desc->ui4_sec_size);
            
            break;
        }
            
        case SVCTX_REC_BOUND_TYPE_SPACE:
        {
            SVCTX_REC_SPACE_BOUND_INFO_T* pt_sp_bound = 
                    (SVCTX_REC_SPACE_BOUND_INFO_T *) pt_running_svc_req->pt_rec_cfg->pv_bound_info;
            
            t_ru_cfg.e_bound_type = REC_UTIL_BOUND_TYPE_SPACE;
            t_ru_cfg.u_data.t_space_bound_info.ui4_flags = 0;
            
            t_ru_cfg.u_data.t_space_bound_info.ui8_sec_ns = 
                (pt_sp_bound->ui8_size == SVCTX_REC_SPACE_BOUND_UNLIMIT) ? 
                        REC_UTIL_SPACE_BOUND_UNLIMIT : 
                        (pt_sp_bound->ui8_size / ((UINT64) pt_strg_desc->ui4_sec_size));
            
            t_ru_cfg.u_data.t_space_bound_info.ui4_idx_tbl_ticks = 
                (pt_sp_bound->ui4_tick_cap == SVCTX_REC_SPACE_BOUND_TIME_UNLIMIT) ? 
                        REC_UTIL_SPACE_BOUND_TIME_UNLIMIT : pt_sp_bound->ui4_tick_cap;
                            
            break;
        }
            
        default:
            DBG_ABORT (DBG_MOD_SVCTX);
        }
        
        if (pt_res_brdcst_svl)
        {
            t_ru_cfg.e_brdcst_type   = pt_res_brdcst_svl->e_brdcst_type;
            t_ru_cfg.e_brdcst_medium = pt_res_brdcst_svl->e_brdcst_medium;
        }
        else
        {
            t_ru_cfg.e_brdcst_type   = BRDCST_TYPE_UNKNOWN;
            t_ru_cfg.e_brdcst_medium = BRDCST_MEDIUM_UNKNOWN;
        }
        
        switch (pt_running_svc_req->pt_rec_cfg->e_no_data_opt)
        {
        case SVCTX_REC_NO_DATA_OPT_KEEP_REC:
            t_ru_cfg.e_empty_tk_opt = REC_UTIL_EMPTY_TK_OPT_KEEP;
            break;
            
        case SVCTX_REC_NO_DATA_OPT_SKIP_REC:
            t_ru_cfg.e_empty_tk_opt = REC_UTIL_EMPTY_TK_OPT_IGNORE;
            break;
            
        default:
            DBG_ABORT (DBG_MOD_SVCTX);
        }
        
        switch (pt_running_svc_req->pt_rec_cfg->e_profile)
        {
        case SVCTX_REC_PROFILE_SIMPLE:
            t_ru_cfg.e_idx_tbl_opt = REC_UTIL_IDX_TBL_OPT_DISABLE;
            break;
            
        case SVCTX_REC_PROFILE_ADVANCED:
            t_ru_cfg.e_idx_tbl_opt = REC_UTIL_IDX_TBL_OPT_ENABLE;
            break;
         
        default:
            DBG_ABORT (DBG_MOD_SVCTX);
        }
        
        t_ru_cfg.pv_rec_purpose     = pt_running_svc_req->pt_rec_cfg->pv_rec_purpose;
        t_ru_cfg.ui4_ms_tick_period = pt_running_svc_req->pt_rec_cfg->ui4_ms_tick_period;
        t_ru_cfg.ui4_tk_extra       = REC_SVC_EXTRA_TICK;
        
        /* Start from random point to prevent beginning location 
           being written more time than other location.
           Only Time-Bound and Ring-Buffer can apply this mechanism. */
        
        if (pt_running_svc_req->pt_rec_cfg->e_bound_type == SVCTX_REC_BOUND_TYPE_TIME &&
            pt_running_svc_req->pt_rec_cfg->e_buf_mode   == SVCTX_REC_BUF_MODE_RING)
        {
            SVCTX_REC_TIME_BOUND_INFO_T*  pt_time_bound = 
                    (SVCTX_REC_TIME_BOUND_INFO_T *) pt_running_svc_req->pt_rec_cfg->pv_bound_info;
            UINT32  ui4_num_lb_total, ui4_fac;
            
            ui4_num_lb_total = (UINT32) (pt_time_bound->ui8_tsb_cap / ((UINT64) REC_SVC_LB_SIZE));
            
            ui4_fac = pt_running_svc_req->pt_rec_cfg->t_strg_desc.ui4_sec_size / REC_SVC_LB_SIZE_2_POW;
            
            t_ru_cfg.ui4_lba_init = (UINT32) (((_rec_svc_util_rand_low_freq () % ui4_num_lb_total) / ui4_fac) * ui4_fac);
        }
        else
        {
            t_ru_cfg.ui4_lba_init = 0;
        }
        
        /* pvr buffer info & status will be setup later in the STARTING state of Record Session Handler */
        t_ru_cfg.t_pvr_buf_info.pui1_buf_addr    = NULL;
        t_ru_cfg.t_pvr_buf_info.z_buf_len        = 0;
        t_ru_cfg.t_pvr_buf_status.pui1_write_ptr = NULL;
        t_ru_cfg.t_pvr_buf_status.pui1_read_ptr  = NULL;
        
        t_ru_cfg.t_sess_key = pt_running_svc_req->t_sess_key;
        
        t_ru_cfg.t_strg_desc.ps_strg_path           = pt_strg_desc->s_strg_path;
        t_ru_cfg.t_strg_desc.ui8_offset_sec_ns      = (UINT64) (pt_strg_desc->ui8_tsb_offset / ((UINT64) pt_strg_desc->ui4_sec_size));
        t_ru_cfg.t_strg_desc.ui4_mem_align          = pt_strg_desc->ui4_mem_align;
        t_ru_cfg.t_strg_desc.ui4_sec_size           = pt_strg_desc->ui4_sec_size;
        t_ru_cfg.t_strg_desc.ui4_min_access_sec_ns  = pt_strg_desc->ui4_min_access_sec_ns;
        t_ru_cfg.t_strg_desc.ui4_max_access_sec_ns  = pt_strg_desc->ui4_max_access_sec_ns;
        t_ru_cfg.t_strg_desc.ui4_best_access_sec_ns = pt_strg_desc->ui4_best_access_sec_ns;

        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Req set config (offset=0x%llx, lba_init=%d, path=\"%s\"). (L%d)\n", 
                    t_ru_cfg.t_strg_desc.ui8_offset_sec_ns * ((UINT64) t_ru_cfg.t_strg_desc.ui4_sec_size),
                    t_ru_cfg.ui4_lba_init,
                    t_ru_cfg.t_strg_desc.ps_strg_path,
                    __LINE__));
        
        /* set config to record utility */
        i4_ret = rec_util_req_set_config (pt_running_svc_req->h_rec_util,
                                          & t_ru_cfg,
                                          & ui4_req_id );
        
        if (i4_ret != REC_UTILR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to set config of Record Utility (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_FAIL;
            break;
        }
        
        pt_running_svc_req->ui4_req_id_set_cfg = ui4_req_id;
        
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Requested to set config of REC_UTIL (h=%d, req_id=%d). (L%d)\n", 
                    h_rec_util,
                    ui4_req_id,
                    __LINE__));

    } while (FALSE);
    
    if (RET_FAIL (i4_ret))
    {
        /* return the error value to transit to SESS_ERROR state */
        return i4_ret;
    }
      
    return SVCTXR_OK;
}    

static INT32 _rec_svc_state_transit_sess_opened (
    SVCTX_T*                              pt_svctx)
{
    REC_SVC_REQ_T*      pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_STATE_T     e_state_old;
    
    /* check previous state and preconditions */
    if (pt_running_svc_req->e_state != REC_SVC_STATE_SESS_OPENING && 
        pt_running_svc_req->e_state != REC_SVC_STATE_SESS_STOPPING )
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong state (%d) when transiting to SESS_OPENED. (L%d)\n", pt_running_svc_req->e_state, __LINE__));

        return SVCTXR_REC_INTERNAL_ERROR;
    }

    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] State Transition: %s(%d) -> %s(%d). (L%d)\n", 
                as_g_state_name[pt_running_svc_req->e_state],
                pt_running_svc_req->e_state,
                as_g_state_name[REC_SVC_STATE_SESS_OPENED],
                REC_SVC_STATE_SESS_OPENED,
                __LINE__));
    
    e_state_old = pt_running_svc_req->e_state;
    pt_running_svc_req->e_state = REC_SVC_STATE_SESS_OPENED;
    pt_running_svc_req->ui4_st_flag = 0;
    
    if (e_state_old == REC_SVC_STATE_SESS_OPENING)
    {
        /* do nothing */
    }
    else /* if (e_state_old == REC_SVC_STATE_SESS_STOPPING) */
    {
        if (pt_running_svc_req->pf_rec_nfy)
        {
            if(pt_running_svc_req->pt_rec_cfg != NULL)
            {
                pt_running_svc_req->pf_rec_nfy (pt_running_svc_req->pt_rec_cfg->h_svctx_nfy,
                                            pt_running_svc_req->pv_rec_nfy_tag,
                                            SVCTX_NTFY_CODE_REC_PAUSED,
                                            0 );
            }
        }
        else
        {
            DBG_ABORT (DBG_MOD_SVCTX);
        }  
        
        rec_hdlr_invoke_client_svc_nfy_fct (pt_svctx,
                                            SVCTX_NTFY_CODE_REC_PAUSED,
                                            ST_UNKNOWN );
                                            
        #ifdef CLI_LVL_ALL
        {
            if (pt_running_svc_req->b_inform_paused)
            {
                rec_hdlr_cli_dump_paused_info (pt_svctx, pt_running_svc_req);
                pt_running_svc_req->b_inform_paused = FALSE;
            }
        }   
        #endif
                                            
    }
    
    return SVCTXR_OK;
}    

static INT32 _rec_svc_state_transit_sess_starting (
    SVCTX_T*                              pt_svctx)
{
    REC_SVC_REQ_T*      pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32               i4_ret;

    /* check previous state and preconditions */
    if (pt_running_svc_req->e_state != REC_SVC_STATE_SESS_OPENED)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong state (%d) when transiting to SESS_STARTING. (L%d)\n", pt_running_svc_req->e_state, __LINE__));

        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] State Transition: %s(%d) -> %s(%d). (L%d)\n", 
                as_g_state_name[pt_running_svc_req->e_state],
                pt_running_svc_req->e_state,
                as_g_state_name[REC_SVC_STATE_SESS_STARTING],
                REC_SVC_STATE_SESS_STARTING,
                __LINE__));
    
    pt_running_svc_req->e_state = REC_SVC_STATE_SESS_STARTING;
    pt_running_svc_req->ui4_st_flag = 0;
    
    /* capture pvr FRI info */
    if (pt_running_svc_req->e_media_type == REC_SVC_MEDIA_TYPE_PVR)
    {
        i4_ret = rec_util_rfi_load (pt_running_svc_req->h_rec_util);
        if (i4_ret != REC_UTILR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to load RFI (%d). (L%d)\n", i4_ret, __LINE__));
            return SVCTXR_REC_EXTNL_MODULE_FAIL;
        }
        
        i4_ret = _rec_svc_util_pvr_rfi_write_bef_rec (pt_running_svc_req);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to write RFI defore rec (%d). (L%d)\n", i4_ret, __LINE__));
        }

        /* capture first event title for PVR */
        if (
#ifndef MW_EDB_SUPPORT
            REC_SVC_EVCTX_INFO_AVAIL (crnt_evn_get_cond (pt_running_svc_req->pt_crnt_evn))
#else
            crnt_evn_get_cond (pt_running_svc_req->pt_crnt_evn) == EDB_COND_STARTED
#endif
            )
        {
            i4_ret = _rec_svc_util_pvr_rfi_write_fst_evn_ttl (pt_running_svc_req);
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to write FST_EVN_TTL (%d). (L%d)\n", i4_ret, __LINE__));
                i4_ret = SVCTXR_OK;
            }
        }
        /* store before record so that clients can aquire RFI-info for the file under recording */
        i4_ret = rec_util_rfi_store (pt_running_svc_req->h_rec_util);
        if (i4_ret != REC_UTILR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to store RFI (%d). (L%d)\n", i4_ret, __LINE__));
        }
    }
    
    /* start session */    
    i4_ret = x_sm_play_sess (pt_running_svc_req->h_rcrd_sess,
                             pt_running_svc_req->t_parent.i4_speed);

    if (i4_ret != SMR_OK)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to play Record Session (%d). (L%d)\n", i4_ret, __LINE__));
        return SVCTXR_FAIL;
    }

    return SVCTXR_OK;
}

static INT32 _rec_svc_state_transit_sess_closing (
    SVCTX_T*                              pt_svctx)
{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32           i4_ret;
    
    /* check previous state and preconditions */
    if (pt_running_svc_req->e_state != REC_SVC_STATE_SESS_OPENED)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong state (%d) when transiting to SESS_STARTED. (L%d)\n", pt_running_svc_req->e_state, __LINE__));

        return SVCTXR_REC_INTERNAL_ERROR;
    }

    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] State Transition: %s(%d) -> %s(%d). (L%d)\n", 
                as_g_state_name[pt_running_svc_req->e_state],
                pt_running_svc_req->e_state,
                as_g_state_name[REC_SVC_STATE_SESS_CLOSING],
                REC_SVC_STATE_SESS_CLOSING,
                __LINE__));
    
    pt_running_svc_req->e_state = REC_SVC_STATE_SESS_CLOSING;
    pt_running_svc_req->ui4_st_flag = 0;
    
    /* check if need to finalize */
    switch (pt_running_svc_req->pt_rec_cfg->e_stop_opt)
    {
    case SVCTX_REC_STOP_OPT_NO_CARE:
        break;
        
    case SVCTX_REC_STOP_OPT_FINALIZE:
        i4_ret = rec_util_set_finalized (pt_running_svc_req->h_rec_util,
                                         TRUE);
        if (i4_ret != REC_UTILR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to finalize (%d). (L%d)\n", i4_ret, __LINE__));
        }
        break;
        
    default:
        DBG_ABORT (DBG_MOD_SVCTX);
    }
    
    /* post process after record */
    switch (pt_running_svc_req->e_media_type)
    {
    case REC_SVC_MEDIA_TYPE_INVALID:
        break;
        
    case REC_SVC_MEDIA_TYPE_PVR:
    {
        UINT32  ui4_rec_tks;
        
        i4_ret = rec_util_get_tick_count (pt_running_svc_req->h_rec_util,
                                          & ui4_rec_tks);
        if (i4_ret == REC_UTILR_OK)
        {
            if (ui4_rec_tks > 0)
            {
                i4_ret = _rec_svc_util_pvr_rfi_write_aft_rec (pt_running_svc_req);
                if (RET_FAIL (i4_ret))
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to write RFI after record (%d). (L%d)\n", i4_ret, __LINE__));
                }
                
                i4_ret = rec_util_rfi_store (pt_running_svc_req->h_rec_util);
                if (i4_ret != REC_UTILR_OK)
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to store RFI (%d). (L%d)\n", i4_ret, __LINE__));
                }
            }
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get tick count (%d). (L%d)\n", i4_ret, __LINE__));
        }
        break;
    }
        
    case REC_SVC_MEDIA_TYPE_OTHER:
        break;
        
    default:
        DBG_ABORT (DBG_MOD_SVCTX);
    }
    
    /* free session related resources */
    i4_ret = _rec_svc_state_util_free_sess_res (pt_svctx, FALSE);
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to free session resource (%d). (L%d)\n", i4_ret, __LINE__));
    }

    return SVCTXR_OK;
}    

static INT32 _rec_svc_state_transit_sess_started (
    SVCTX_T*                              pt_svctx)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
#ifndef MW_EDB_SUPPORT   
    EVCTX_COND_T			    e_cond_evn  = crnt_evn_get_cond (pt_running_svc_req->pt_crnt_evn);
#endif

    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl;
    INT32                       i4_ret;

    /* check previous state and preconditions */
    if (pt_running_svc_req->e_state != REC_SVC_STATE_SESS_STARTING)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong state (%d) when transiting to SESS_STARTED. (L%d)\n", pt_running_svc_req->e_state, __LINE__));

        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] State Transition: %s(%d) -> %s(%d). (L%d)\n", 
                as_g_state_name[pt_running_svc_req->e_state],
                pt_running_svc_req->e_state,
                as_g_state_name[REC_SVC_STATE_SESS_STARTED],
                REC_SVC_STATE_SESS_STARTED,
                __LINE__));
    
    pt_running_svc_req->e_state = REC_SVC_STATE_SESS_STARTED;
    pt_running_svc_req->ui4_st_flag = 0;

    if(pt_running_svc_req->pt_rec_cfg != NULL)
    {
        if (REC_SVC_PROFILE_SUPPORT_EVENT_INFO (pt_running_svc_req)     &&
            pt_running_svc_req->e_cond_rec_util == REC_UTIL_COND_READY  &&
            pt_running_svc_req->e_cond_sess == SM_SESS_COND_STARTED     
#ifndef MW_EDB_SUPPORT
            && REC_SVC_EVCTX_INFO_AVAIL (e_cond_evn)
#endif
			)
        {
            i4_ret = _rec_svc_intr_update_event_info (pt_svctx);
            
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to update event info. (L%d)\n", __LINE__));
                i4_ret = SVCTXR_OK;
            }
        }
    }
    else
    {
       /* DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] pt_running_svc_req->pt_rec_cfg is NULL. (L%d)\n", __LINE__));*/
    }  
    
    if (pt_running_svc_req->pf_rec_nfy)
    {
        if(pt_running_svc_req->pt_rec_cfg != NULL)
        {
            pt_running_svc_req->pf_rec_nfy (pt_running_svc_req->pt_rec_cfg->h_svctx_nfy,
                                        pt_running_svc_req->pv_rec_nfy_tag,
                                        SVCTX_NTFY_CODE_REC_STARTED,
                                        0 );
        }
    }
    else
    {
        DBG_ABORT (DBG_MOD_SVCTX);
    }  

    rec_hdlr_invoke_client_svc_nfy_fct (pt_svctx,
                                        SVCTX_NTFY_CODE_REC_STARTED,
                                        ST_UNKNOWN );
    
    i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req,
                                               & pt_res_brdcst_svl);

    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Not supported SRC_TYPE. (L%d)\n", __LINE__));
        pt_res_brdcst_svl = NULL;
        i4_ret = SVCTXR_OK;
    }    
    
    /* record rrtctx data */
    
    /* [TODO] >// RRT */
#if 0
    if (pt_running_svc_req->pt_rec_cfg->e_profile == SVCTX_REC_PROFILE_ADVANCED &&
        pt_res_brdcst_svl                                                       &&
        pt_res_brdcst_svl->e_brdcst_type == BRDCST_TYPE_ATSC)
    {
        VOID*   pv_rrctx_data;
        SIZE_T  z_len_rrctx_data;
        
        i4_ret = _rec_svc_util_get_rrctx_data (pt_running_svc_req,
                                               & pv_rrctx_data,
                                               & z_len_rrctx_data);
        if (RET_OK (i4_ret))
        {
            i4_ret = _rec_svc_util_update_rrctx_data (pt_running_svc_req,
                                                      pv_rrctx_data,
                                                      z_len_rrctx_data);
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to update rrctx data (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }    
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get rrctx data (%d). (L%d)\n", i4_ret, __LINE__));
        }
    }
#endif    
    
    return SVCTXR_OK;
}

static INT32 _rec_svc_state_transit_sess_stopping (
    SVCTX_T*                              pt_svctx)
{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32           i4_ret;

    /* check previous state and preconditions */
    if (pt_running_svc_req->e_state != REC_SVC_STATE_SESS_STARTED)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong state (%d) when transiting to SESS_STOPPING. (L%d)\n", pt_running_svc_req->e_state, __LINE__));

        return SVCTXR_REC_INTERNAL_ERROR;
    }

    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] State Transition: %s(%d) -> %s(%d). (L%d)\n", 
                as_g_state_name[pt_running_svc_req->e_state],
                pt_running_svc_req->e_state,
                as_g_state_name[REC_SVC_STATE_SESS_STOPPING],
                REC_SVC_STATE_SESS_STOPPING,
                __LINE__));

    pt_running_svc_req->e_state = REC_SVC_STATE_SESS_STOPPING;
    pt_running_svc_req->ui4_st_flag = 0;
    
    i4_ret = x_sm_stop_sess (pt_running_svc_req->h_rcrd_sess);
    
    if (i4_ret != SMR_OK)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to stop Record Session (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    return SVCTXR_OK;
}

static INT32 _rec_svc_state_transit_sess_error (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    BOOL                        b_sess_not_opened;
    INT32                       i4_ret;
    
    /* check previous state and preconditions */
    if (pt_running_svc_req->e_state == REC_SVC_STATE_SESS_ERROR)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Already in REC_SVC_STATE_SESS_ERROR state. (L%d)\n", __LINE__));
        
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] State Transition: %s(%d) -> %s(%d). (L%d)\n", 
                as_g_state_name[pt_running_svc_req->e_state],
                pt_running_svc_req->e_state,
                as_g_state_name[REC_SVC_STATE_SESS_ERROR],
                REC_SVC_STATE_SESS_ERROR,
                __LINE__));
    
    pt_running_svc_req->e_state     = REC_SVC_STATE_SESS_ERROR;
    pt_running_svc_req->ui4_st_flag = 0;
    
    b_sess_not_opened = pt_running_svc_req->h_rcrd_sess == NULL_HANDLE ? TRUE : FALSE;
    
    /* notify client */
    if (pt_running_svc_req->pf_rec_nfy)
    {
        if(pt_running_svc_req->pt_rec_cfg != NULL)
        {
            pt_running_svc_req->pf_rec_nfy (pt_running_svc_req->pt_rec_cfg->h_svctx_nfy,
                                        pt_running_svc_req->pv_rec_nfy_tag,
                                        SVCTX_NTFY_CODE_REC_INTERNAL_ERROR,
                                        0 );
         }
    }
    else
    {
        DBG_ABORT (DBG_MOD_SVCTX);
    }  
    
    rec_hdlr_invoke_client_svc_nfy_fct (pt_svctx,
                                        SVCTX_NTFY_CODE_REC_INTERNAL_ERROR,
                                        ST_UNKNOWN );
    
    /* free session related resources */
    i4_ret = _rec_svc_state_util_free_sess_res (pt_svctx, FALSE);
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to free session resource (%d). (L%d)\n", i4_ret, __LINE__));
    }    
    
    if (b_sess_not_opened)
    {
        i4_ret = _rec_svc_state_transit_conned (pt_svctx);
        
        if (i4_ret != SVCTXR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit from SESS_ERROR to CONNECTED (%d). (L%d)\n", i4_ret, __LINE__));
            
            /* return this error value to transit to ERROR state */
            return i4_ret;
        }
    }

    return SVCTXR_OK;
}

static VOID _rec_svc_state_transit_error (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl;
    BOOL                        b_cm_pipe_closing;
    INT32                       i4_ret;

    if (!REC_SVC_STATE_CAN_TRANSIT_ERROR (pt_running_svc_req->e_state))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Incompatible state before transit to REC_SVC_STATE_ERROR (L%d)\n", __LINE__));
        return;
    }
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] State Transition: %s(%d) -> %s(%d). (L%d)\n", 
                as_g_state_name[pt_running_svc_req->e_state],
                pt_running_svc_req->e_state,
                as_g_state_name[REC_SVC_STATE_ERROR],
                REC_SVC_STATE_ERROR,
                __LINE__));
    
    pt_running_svc_req->e_state = REC_SVC_STATE_ERROR;
    pt_running_svc_req->ui4_st_flag = 0;

    /* notify client */    
    rec_hdlr_invoke_client_svc_nfy_fct (pt_svctx,
                                        SVCTX_NTFY_CODE_REC_INTERNAL_ERROR,
                                        ST_UNKNOWN );
    
    i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req,
                                               & pt_res_brdcst_svl);

    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Not supported SRC_TYPE. (L%d)\n", __LINE__));
        pt_res_brdcst_svl = NULL;
        i4_ret = SVCTXR_OK;
    }    
    
    /* free session related resources */
    i4_ret = _rec_svc_state_util_free_sess_res (pt_svctx, TRUE);
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to free session resource (%d). (L%d)\n", i4_ret, __LINE__));
    }    
    
    /* close CRNT_EVN */
    if (pt_running_svc_req->pt_crnt_evn != NULL)
    {
        i4_ret = crnt_evn_close (pt_running_svc_req->pt_crnt_evn);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close CRNT_EVN (%d). (L%d)\n", i4_ret, __LINE__));
        }
        
        pt_running_svc_req->pt_crnt_evn = NULL;
    }
    
    /* try to close SCDB */
    if (pt_running_svc_req->h_scdb != NULL_HANDLE)
    {
        i4_ret = x_scdb_close (pt_running_svc_req->h_scdb);
        
        if (i4_ret != SCDBR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close SCDB (%d). (L%d)\n", i4_ret, __LINE__));
        }
        
        pt_running_svc_req->h_scdb = NULL_HANDLE;
    }
    
    /* try to close rrctx */
    if (pt_running_svc_req->h_rrctx != NULL_HANDLE)
    {
        i4_ret = x_rrctx_close_rating (pt_running_svc_req->h_rrctx);
        
        if (i4_ret != RRCTXR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close rrctx (%d). (L%d)\n", i4_ret, __LINE__));
        }
        
        pt_running_svc_req->h_rrctx = NULL_HANDLE;
    }

    /* free rrctx buffer */
    if (pt_running_svc_req->pv_buf_rrctx != NULL)
    {
        x_mem_free (pt_running_svc_req->pv_buf_rrctx);
        pt_running_svc_req->pv_buf_rrctx    = NULL;
        pt_running_svc_req->z_len_buf_rrctx = 0;
    }
    
    /* clear queued events */
    /*For CR313940, when in dtmb,in no signal, cm send disconnted notify to slowly, record enter
      watchdog timeout ,enter error, here not to clear msg queque,let record can enter not init state.*/
    /*_rec_svc_state_clear_event_q (pt_running_svc_req);*/
    
    /* try to close PAT */
    if (pt_res_brdcst_svl)
    {
        i4_ret = _rec_svc_state_util_free_pat_res (pt_svctx,
                                                   pt_running_svc_req,
                                                   pt_res_brdcst_svl );
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to free pat resources (%d). (L%d)\n", i4_ret, __LINE__));
        }
    }
    
    /* try to close connection */    
    i4_ret = _rec_svc_state_util_close_conn (pt_svctx, 
                                             FALSE, 
                                             ALSO_PIPE, 
                                             & b_cm_pipe_closing);
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to disconnet connection (%d). (L%d)\n", i4_ret, __LINE__));
        b_cm_pipe_closing = FALSE;
    }
    
    if (!b_cm_pipe_closing)
    {
        /* try to close cm pipe */    
        i4_ret = _rec_svc_state_util_close_cm_pipe (pt_svctx);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close CM Pipe (%d). (L%d)\n", i4_ret, __LINE__));
        }
    }
    
    /* check if all related resource are closed */
    i4_ret = _rec_svc_state_error_on_res_closed (pt_svctx);

    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to check all closed resources (%d). (L%d)\n", i4_ret, __LINE__));
    }
}    


/*-----------------------------------------------------------------------------
                    state event handling functions
 ----------------------------------------------------------------------------*/
static INT32 _rec_svc_state_conning_util_open_pipe (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*               pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    HANDLE_T                     h_cm_pipe;
    CM_COND_T                    e_cond_cm_pipe;
    BOOL                         b_valid_preset_pipe;
    SVCTX_MSG_T                  t_msg;
    SVCTX_REC_CM_PIPE_NFY_MSG_T* pt_msg_cm_pipe;
    BOOL                         b_fake_msg = TRUE;
    INT32                        i4_ret;

    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TOP> Try to open pipe. (L%d)\n", 
                __LINE__));
    
    if (pt_running_svc_req->e_state != REC_SVC_STATE_CONNECTING)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid state to open pipe. (L%d)\n", __LINE__));
        return SVCTXR_WRONG_STATE;
    }
    
    do
    {
        h_cm_pipe      = NULL_HANDLE;
        e_cond_cm_pipe = CM_COND_CLOSED;
        
        b_valid_preset_pipe = pt_running_svc_req->b_preset_conn_info                            &&
                              x_handle_valid (pt_running_svc_req->t_conn_info_preset.h_cm_pipe) &&
                              (pt_running_svc_req->t_conn_info_preset.e_cond_cm_pipe == CM_COND_OPENING || 
                               pt_running_svc_req->t_conn_info_preset.e_cond_cm_pipe == CM_COND_OPENED );
        
        /* try to open/reuse cm pipe */
        if (!b_valid_preset_pipe)
        {
            BOOL  b_proceed = FALSE;
            BOOL  b_chk_proceed;
                
            /* not to check proceed if pipe is to be reused */
            b_chk_proceed = pt_running_svc_req->b_preset_conn_info ? FALSE : TRUE;
            
            if (b_chk_proceed)
            {
                if (pt_running_svc_req->b_act_q_in_mgr)
                {
                    /* queue this action to process later */
                    i4_ret = _rec_svc_state_act_msg_q_append (pt_running_svc_req,
                                                              SVCTX_MSG_REC_QUEUED_PIPE_OPEN);
                    if (RET_OK (i4_ret))
                    {
                        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TOP> Act queued -> append ACTION_PIPE_OPEN. (L%d)\n", __LINE__));
                    }
                    else
                    {
                        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] <TOP> Fail to append ACTION_PIPE_OPEN (%d). (L%d)\n", i4_ret, __LINE__));
                    }                
                    
                    b_proceed = FALSE;
                }
                else
                {
                    #ifdef MW_TV_AV_BYPASS_SUPPORT
                    {
                        VAR_INFO_T   t_var_info;
    
                        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TOP> Check proceed: ACTION_PIPE_OPEN. (L%d)\n", __LINE__));
    
                        pt_running_svc_req->t_svcst.e_phase = PLAYER_ACTION_PIPE_OPEN;
                        
                        t_var_info.e_var_type = VAR_TYPE_PLAYER;
                        t_var_info.e_op_type  = OP_TYPE_OPEN;
                        t_var_info.ui1_msg_id = SVCTX_MSG_REC_QUEUED_PIPE_OPEN;
                        t_var_info.u.t_player_info.e_player_type  = _rec_svc_state_util_get_player_type (pt_running_svc_req);
                        t_var_info.u.t_player_info.pt_player      = NULL;
                        t_var_info.u.t_player_info.pf_invoke_func = _rec_svc_msg_hdlr_queued_action;
                        
                        b_proceed = svctx_mgr_ready_to_proceed (ACTIVE_LINK_NOW,
                                                                pt_running_svc_req->e_src_rec,
                                                                pt_svctx,
                                                                & t_var_info);
                        if (!b_proceed)
                        {
                            pt_running_svc_req->b_act_q_in_mgr = TRUE;
                            
                            i4_ret = _rec_svc_state_act_msg_q_append (pt_running_svc_req,
                                                                      SVCTX_MSG_REC_QUEUED_PIPE_OPEN);
                            if (RET_OK (i4_ret))
                            {
                                DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TOP> Action queued: ACTION_PIPE_OPEN. (L%d)\n", __LINE__));
                            }
                            else
                            {
                                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] <TOP> Fail to append ACTION_PIPE_OPEN (%d). (L%d)\n", i4_ret, __LINE__));
                            }
                        
                            DBG_INFO_DUMP_SVCTX_MGR_REQ_Q (pt_svctx);
                        }
                    }
                    #else
                    {
                        b_proceed = TRUE;
                    }
                    #endif
                }
            }
            else
            {
                DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TOP> Not to check proceed because pipe reuse. (L%d)\n", __LINE__));
                
                b_proceed = TRUE;
            }
            
            if (!b_proceed)
            {
                i4_ret = SVCTXR_REC_MSG_NOT_PROCED;
                break;
            }
            
            i4_ret = CM_OPEN_PIPE (NULL, 
                                   (UINT8) (((INT16) PLAYER_PIPE_PRIORITY) + ((INT16) pt_svctx->i1_prio_diff)),
                                   0,
                                   (VOID *) pt_svctx->h_sys_svctx,
                                   _rec_svc_cb_cm_pipe_nfy,
                                   & h_cm_pipe,
                                   & e_cond_cm_pipe );
            
            if (i4_ret != CMR_OK)
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Open CM Pipe error (%d). (L%d)\n", i4_ret, __LINE__));
                i4_ret = SVCTXR_FAIL;
                break;
            }
            
            if (e_cond_cm_pipe == CM_COND_CLOSED)
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Opening CM Pipe returns CLOSED condition. (L%d)\n", __LINE__));
                i4_ret = SVCTXR_FAIL;
                break;
            }
    
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Open CM Pipe (%d) (cond=%d). (L%d)\n", 
                        h_cm_pipe,
                        e_cond_cm_pipe,
                        __LINE__));

            b_fake_msg = FALSE;
        }
        else
        {
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TOP> Reuse pipe (%d) (cond=%d) & send fake msg. (L%d)\n", 
                        pt_running_svc_req->t_conn_info_preset.h_cm_pipe,
                        pt_running_svc_req->t_conn_info_preset.e_cond_cm_pipe,
                        __LINE__));

            h_cm_pipe      = pt_running_svc_req->t_conn_info_preset.h_cm_pipe;
            e_cond_cm_pipe = pt_running_svc_req->t_conn_info_preset.e_cond_cm_pipe;
            b_fake_msg     = TRUE;
        }

        pt_running_svc_req->h_cm_pipe      = h_cm_pipe;
        pt_running_svc_req->e_cond_cm_pipe = e_cond_cm_pipe;
        
        /* send message for pipe's current condition */
        t_msg.h_svctx           = pt_svctx->h_sys_svctx;
        t_msg.ui1_msg_id        = SVCTX_MSG_REC_CM_PIPE_NFY;
        t_msg.pf_invoke_func    = NULL;
        t_msg.pv_tag            = NULL;
        
        pt_msg_cm_pipe            = (SVCTX_REC_CM_PIPE_NFY_MSG_T *) t_msg.u.t_hdlr.aui1_data;
        t_msg.u.t_hdlr.z_len_data = sizeof (SVCTX_REC_CM_PIPE_NFY_MSG_T);
        
        pt_msg_cm_pipe->b_fake    = b_fake_msg;
        pt_msg_cm_pipe->h_cm_pipe = h_cm_pipe;
        pt_msg_cm_pipe->e_cond    = e_cond_cm_pipe;
        pt_msg_cm_pipe->ui4_data  = 0;
        
        i4_ret = svctx_send_msg (pt_svctx->h_sys_svctx,
                                 & t_msg );
                                 
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send message. (L%d)\n", __LINE__));
        }

    } while (FALSE);

    /* clear the pipe info to prevent affecting the next time pipe open action */
    pt_running_svc_req->t_conn_info_preset.h_cm_pipe = NULL_HANDLE;
    pt_running_svc_req->t_conn_info_preset.e_cond_cm_pipe = CM_COND_CLOSED;
    
    return i4_ret;
}

static INT32 _rec_svc_state_conning_util_open_conn (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*            pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    CM_COMMAND_T              at_cm_cmds[MAX_SIZE_OF_CM_COMMAND];
    HANDLE_T                  h_conn = NULL_HANDLE;
    CM_COND_T                 e_cond_conn;
    BOOL                      b_chk_proceed;
    SVCTX_MSG_T               t_msg;
    SVCTX_REC_CONN_NFY_MSG_T* pt_msg_conn;
    INT32                     i4_ret;

    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TOC> Try to open conn. (L%d)\n", 
                __LINE__));

    if (pt_running_svc_req->e_state != REC_SVC_STATE_CONNECTING)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid state to open conn. (L%d)\n", __LINE__));
        return SVCTXR_WRONG_STATE;
    }
    
    b_chk_proceed = TRUE;
    
    /* check validity of preset conn info */
    if (pt_running_svc_req->b_preset_conn_info)
    {
        CM_COND_T   e_cond_conn_now;
        BOOL        b_valid_conn_info = FALSE;
        
        /* not to check proceed if pipe & conn is to be reused */
        b_chk_proceed = FALSE;
        
        i4_ret = x_cm_get_cond (pt_running_svc_req->t_conn_info_preset.h_conn,
                                & e_cond_conn_now);
        if (i4_ret == CMR_OK)
        {
            if (e_cond_conn_now == CM_COND_CONNECTING ||
                e_cond_conn_now == CM_COND_CONNECTED)
            {
                b_valid_conn_info = TRUE;
            }
        }
        else
        {
            if (i4_ret != CMR_INV_HANDLE)
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get conn cond (%d). (L%d)\n", i4_ret, __LINE__));
            }
            
            b_valid_conn_info = FALSE;
        }
        
        if (b_valid_conn_info)
        {
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TOC> Try to reuse conn [%d] for open conn. (L%d)\n",
                       pt_running_svc_req->t_conn_info_preset.h_conn, 
                       __LINE__));
        }
        else
        {
            pt_running_svc_req->b_preset_conn_info = FALSE;
            x_memset (& pt_running_svc_req->t_conn_info_preset, 0, sizeof (REC_SVC_CONN_INFO_T));
        }
    }
    
    /* consistency checking before preparing connection */
    if (pt_running_svc_req->h_conn_reusing_by_cm != NULL_HANDLE)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
    }
    
    /* get connection commands */
    i4_ret = _rec_svc_intr_get_cm_cmds (pt_svctx,
                                        pt_running_svc_req,
                                        at_cm_cmds,
                                        MAX_SIZE_OF_CM_COMMAND );
    
    if (i4_ret == SVCTXR_OK)
    {
        BOOL    b_proceed = FALSE;
        
#ifdef MW_TV_AV_BYPASS_SUPPORT
        if (pt_svctx->b_fake_disc)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] b_fake_disc should be FALSE. (L%d)\n", __LINE__));
            pt_svctx->b_fake_disc = FALSE;
        }
#endif         

        if (b_chk_proceed)
        {
            if (pt_running_svc_req->b_act_q_in_mgr)
            {
                /* queue this action to process later */
                i4_ret = _rec_svc_state_act_msg_q_append (pt_running_svc_req,
                                                          SVCTX_MSG_REC_QUEUED_CONN_OPEN);
                if (RET_OK (i4_ret))
                {
                    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TOC> Act queued -> append ACTION_CONN_OPEN. (L%d)\n", __LINE__));
                }
                else
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] <TOC> Fail to append ACTION_CONN_OPEN (%d). (L%d)\n", i4_ret, __LINE__));
                }
                
                b_proceed = FALSE;
            }
            else
            {
                #ifdef MW_TV_AV_BYPASS_SUPPORT
                {
                    VAR_INFO_T   t_var_info;
                    
                    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TOC> Check proceed: ACTION_CONN_OPEN. (L%d)\n", __LINE__));
                    
                    pt_running_svc_req->t_svcst.e_phase = PLAYER_ACTION_CONN_OPEN;
                    
                    t_var_info.e_var_type = VAR_TYPE_PLAYER;
                    t_var_info.e_op_type  = OP_TYPE_OPEN;
                    t_var_info.ui1_msg_id = SVCTX_MSG_REC_QUEUED_CONN_OPEN;
                    t_var_info.u.t_player_info.e_player_type  = _rec_svc_state_util_get_player_type (pt_running_svc_req);
                    t_var_info.u.t_player_info.pt_player      = NULL;
                    t_var_info.u.t_player_info.pf_invoke_func = _rec_svc_msg_hdlr_queued_action;
                    
                    b_proceed = svctx_mgr_ready_to_proceed (ACTIVE_LINK_NOW,
                                                            pt_running_svc_req->e_src_rec,
                                                            pt_svctx,
                                                            & t_var_info);
                    if (!b_proceed)
                    {
                        pt_running_svc_req->b_act_q_in_mgr = TRUE;
                        
                        i4_ret = _rec_svc_state_act_msg_q_append (pt_running_svc_req,
                                                                  SVCTX_MSG_REC_QUEUED_CONN_OPEN);
                        if (RET_OK (i4_ret))
                        {
                            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TOC> Action queued: ACTION_CONN_OPEN. (L%d)\n", __LINE__));
                        }
                        else
                        {
                            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] <TOC> Fail to append ACTION_CONN_OPEN (%d). (L%d)\n", i4_ret, __LINE__));
                        }
                    
                        DBG_INFO_DUMP_SVCTX_MGR_REQ_Q (pt_svctx);
                    }
                }
                #else
                {
                    b_proceed = TRUE;
                }
                #endif
            }
        }
        else
        {
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] <TOC> Not to check proceed. (L%d)\n", __LINE__));
            
            b_proceed = TRUE;
        }
        
        if (b_proceed)
        {
            /* open connection */
            i4_ret = CM_CONNECT (pt_running_svc_req->h_cm_pipe,
                                 at_cm_cmds,
                                 (VOID *) pt_svctx->h_sys_svctx,
                                 _rec_svc_cb_conn_nfy,
                                 & h_conn,
                                 & e_cond_conn );
            
            if (i4_ret == CMR_OK)
            {
                DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Open connection (%d) (conn_cond=%d, reuse_conn=%d). (L%d)\n", 
                            h_conn,
                            e_cond_conn,
                            pt_running_svc_req->b_preset_conn_info ? pt_running_svc_req->t_conn_info_preset.h_conn : NULL_HANDLE,
                            __LINE__));
                
                if (e_cond_conn != CM_COND_DISCONNECTED)
                {
                    pt_running_svc_req->h_conn      = h_conn;
                    pt_running_svc_req->e_cond_conn = e_cond_conn;
                    
                    if (pt_running_svc_req->b_on_timer_conn)
                    {
                        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
                        
                        i4_ret = x_timer_stop (pt_running_svc_req->h_timer_conn);
                        
                        if (i4_ret != OSR_OK)
                        {
                            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to stop conn timer (%d). (L%d)\n", i4_ret, __LINE__));
                        }
                        
                        pt_running_svc_req->b_on_timer_conn = FALSE;
                    }
                    
                    i4_ret = x_timer_start (pt_running_svc_req->h_timer_conn,
                                            REC_SVC_CONNING_TIMEOUT,
                                            X_TIMER_FLAG_ONCE,
                                            _rec_svc_cb_conning_timeout_nfy,
                                            (VOID *) pt_svctx->h_sys_svctx );
                    if (i4_ret == OSR_OK)
                    {
                        pt_running_svc_req->b_on_timer_conn = TRUE;
                    }
                    else
                    {
                        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to start conn timer (%d). (L%d)\n", i4_ret, __LINE__));
                        i4_ret = SVCTXR_OK;
                    }
                    if (e_cond_conn != CM_COND_CONNECTING)
                    {
                        /* send message for connection's current condition */
                        t_msg.h_svctx           = pt_svctx->h_sys_svctx;
                        t_msg.ui1_msg_id        = SVCTX_MSG_REC_CONN_NFY;
                        t_msg.pf_invoke_func    = NULL;
                        t_msg.pv_tag            = NULL;
                        
                        pt_msg_conn = (SVCTX_REC_CONN_NFY_MSG_T *) t_msg.u.t_hdlr.aui1_data;
                        t_msg.u.t_hdlr.z_len_data = sizeof (SVCTX_REC_CONN_NFY_MSG_T);
                        
                        pt_msg_conn->b_fake   = FALSE;
                        pt_msg_conn->h_conn   = h_conn;
                        pt_msg_conn->e_cond   = e_cond_conn;
                        pt_msg_conn->ui4_data = 0;
                        
                        i4_ret = svctx_send_msg (pt_svctx->h_sys_svctx,
                                                 & t_msg );
                                                 
                        if (RET_FAIL (i4_ret))
                        {
                            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send message. (L%d)\n", __LINE__));
                        }
                    }
                }
                else
                {
                    /* ignore the CM_COND_DISCONNECTED condition */
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Connecting CM returns DISCONNECTED condition. (L%d)\n", __LINE__));
                    i4_ret = SVCTXR_REC_INTERNAL_ERROR;
                }
            }
            else
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to do CM connect (%d). (L%d)\n", i4_ret, __LINE__));
                i4_ret = SVCTXR_REC_INTERNAL_ERROR;
            }
        }
        else
        {
            i4_ret = SVCTXR_REC_MSG_NOT_PROCED;
        }
    }
    else
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Get CM commands error (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    /* the preset conn info should take effect for the first time connection */
    if (pt_running_svc_req->b_preset_conn_info)
    {
        pt_running_svc_req->b_preset_conn_info = FALSE;
        x_memset (& pt_running_svc_req->t_conn_info_preset, 0, sizeof (REC_SVC_CONN_INFO_T));
    }
    
    return i4_ret;
}

static INT32 _rec_svc_state_conning_on_req_disconn (
    SVCTX_T*                    pt_svctx)
{
    INT32               i4_ret;
    
    i4_ret = _rec_svc_state_transit_disconning(pt_svctx);
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to disconnect in connecting state (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    return i4_ret;
}

static INT32 _rec_svc_state_conning_on_cm_pipe_opened (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*      pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32               i4_ret;
    
    /* make sure the connection pt_running_svc_req->h_conn is not beginning yet. */
    if (pt_running_svc_req->e_cond_conn == CM_COND_DISCONNECTED)
    {
        i4_ret = _rec_svc_state_conning_util_open_conn (pt_svctx);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open conn by util (%d). (L%d)\n", i4_ret, __LINE__));
        }
    }
    else
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Un-handled CM Pipe Opened Notify. (L%d)\n", __LINE__));
        
        /* CM Pipe should be in OPENED state currently */
        if (pt_running_svc_req->e_cond_cm_pipe == CM_COND_OPENED)
        {
            /* ignore this event */
            i4_ret = SVCTXR_OK;
        }
        else
        {
            /* something must be wrong */
            i4_ret = SVCTXR_REC_INTERNAL_ERROR;
        }
    }
    
    return i4_ret;
}

static INT32 _rec_svc_state_conning_on_cm_pipe_closed (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*      pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32               i4_ret;
    
    i4_ret = _rec_svc_state_util_close_conn (pt_svctx, 
                                             FALSE, 
                                             NOT_ALSO_PIPE,
                                             NULL);

    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to disconnet connection (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    /* don't need to check cm-pipe handle since it has been automatically closed */
    
    if (pt_running_svc_req->e_cond_conn == CM_COND_DISCONNECTED &&
        pt_running_svc_req->e_cond_cm_pipe == CM_COND_CLOSED)
    {
        if (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_INIT_SVC)
        {
            /* can transit to WAIT_RECONN state now */
            i4_ret = _rec_svc_state_transit_wait_reconn (pt_svctx);
            
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to DISC (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
        else /* REC_SVC_SRC_OP_TYPE_DEINIT_SVC */
        {
            /* can transit to disconnected state now */
            i4_ret = _rec_svc_state_transit_disconned (pt_svctx);
            
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to DISC (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
    }
    else
    {
        /* return the result of _rec_svc_state_util_close_conn () */
        i4_ret = i4_ret; 
    }

    return i4_ret;
}    

static INT32 _rec_svc_state_conning_on_conn_conned (
    SVCTX_T*                    pt_svctx,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32                       i4_ret;
    
    if (z_len_data != sizeof (SVCTX_REC_CONN_NFY_MSG_T)    ||
        pt_running_svc_req->e_cond_cm_pipe != CM_COND_OPENED)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    i4_ret = _rec_svc_state_transit_conned (pt_svctx);
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit connected (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    return i4_ret;
}

static INT32 _rec_svc_state_conning_on_conn_disconned (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*      pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32               i4_ret;
    
    /* don't need to check connection handle since it must have been automatically closed */
    
    if (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_INIT_SVC)
    {
        if (pt_running_svc_req->e_cond_conn == CM_COND_DISCONNECTED)
        {
            /* can transit to WAIT_RECONN state now */
            i4_ret = _rec_svc_state_transit_wait_reconn (pt_svctx);
            
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to DISC (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
        else
        {
            i4_ret = SVCTXR_OK;
        }
    }
    else /* REC_SVC_SRC_OP_TYPE_DEINIT_SVC */
    {
        /* close cm pipe if necessary */
        i4_ret = _rec_svc_state_util_close_cm_pipe (pt_svctx);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close cm pipe (%d). (L%d)\n", i4_ret, __LINE__));
        }
        
        if (pt_running_svc_req->e_cond_conn == CM_COND_DISCONNECTED &&
            pt_running_svc_req->e_cond_cm_pipe == CM_COND_CLOSED)
        {
            /* can transit to disconnected state now */
            i4_ret = _rec_svc_state_transit_disconned (pt_svctx);
            
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to DISC (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
        else
        {
            i4_ret = SVCTXR_OK;
        }
    }
    
    return i4_ret;
}

static INT32 _rec_svc_state_conned_on_disconn (
    SVCTX_T*                    pt_svctx)
{
    INT32   i4_ret;
    
    i4_ret = _rec_svc_state_transit_disconning (pt_svctx);

    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit DISCONNECTING (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    return i4_ret;
}    

static INT32 _rec_svc_state_conned_on_req_open_sess (
    SVCTX_T*                    pt_svctx,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data)
{
    REC_SVC_REQ_T*                  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    SVCTX_REC_REQ_START_REC_MSG_T*  pt_msg_info = (SVCTX_REC_REQ_START_REC_MSG_T *) pv_data;
    INT32                           i4_ret;
    
    if (z_len_data != sizeof (SVCTX_REC_REQ_START_REC_MSG_T))
    {
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    if (!pt_running_svc_req->b_svc_blked)
    {
        i4_ret = _rec_svc_state_transit_sess_opening (pt_svctx, pt_msg_info);
    
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit SESS_OPENING (%d). (L%d)\n", i4_ret, __LINE__));
           
            if (pt_running_svc_req->e_state == REC_SVC_STATE_SESS_OPENING)
            {
                /* return the error value to transit to SESS_ERROR state */
            }
            else
            {
                /* not transit to SESS_OPENING yet, so simply ignore this event */
                i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
            }
        }
    }
    else
    {
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Cancel opening session since service blocked. (L%d)\n", 
                    __LINE__));
        
        i4_ret = SVCTXR_OK;
    }
    
    return i4_ret;
}

static INT32 _rec_svc_state_disconning_on_deinit_svc (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*   pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32            i4_ret;
    
    if (pt_running_svc_req->e_cond_conn == CM_COND_DISCONNECTED)
    {
        if (!(pt_running_svc_req->ui4_st_flag & REC_SVC_ST_FLAG_DISCONNING_CLOSED_PIPE_BY_DEINIT_SVC))
        {
            i4_ret = _rec_svc_state_util_close_cm_pipe (pt_svctx);
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close cm pipe by util (%d). (L%d)\n", i4_ret, __LINE__));
            }
            pt_running_svc_req->ui4_st_flag |= REC_SVC_ST_FLAG_DISCONNING_CLOSED_PIPE_BY_DEINIT_SVC;
        }
    }
    
    return SVCTXR_REC_MSG_QUEUE_EVN;
}

static INT32 _rec_svc_state_disconning_on_conn_disconned (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl;
    INT32                       i4_ret;

    i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req,
                                               & pt_res_brdcst_svl);

    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Not supported SRC_TYPE. (L%d)\n", __LINE__));
        pt_res_brdcst_svl = NULL;
        i4_ret = SVCTXR_OK;
    }    

    if (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_INIT_SVC)
    {
        if (pt_running_svc_req->e_cond_conn == CM_COND_DISCONNECTED &&
            ( pt_res_brdcst_svl == NULL     ||
              pt_res_brdcst_svl->e_cond_pat == TM_COND_UNAVAILABLE ) )
        {
            /* it's time to transit to WAIT_RECONN state */
            
            i4_ret = _rec_svc_state_transit_wait_reconn (pt_svctx);
            
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to DISCONNED (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
    }
    else /* REC_SVC_SRC_OP_TYPE_DEINIT_SVC */
    {
        if (pt_running_svc_req->ui4_st_flag & REC_SVC_ST_FLAG_DISCONNING_CLOSED_PIPE_BY_DEINIT_SVC)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        }
        
        i4_ret = _rec_svc_state_util_close_cm_pipe (pt_svctx);
    
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close cm pipe (%d). (L%d)\n", i4_ret, __LINE__));
        }
        
        if (pt_running_svc_req->e_cond_conn == CM_COND_DISCONNECTED &&
            pt_running_svc_req->e_cond_cm_pipe == CM_COND_CLOSED    &&
            ( pt_res_brdcst_svl == NULL     ||
              pt_res_brdcst_svl->e_cond_pat == TM_COND_UNAVAILABLE ) )
        {
            /* can transit to disconnected state now */
            
            i4_ret = _rec_svc_state_transit_disconned (pt_svctx);
            
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to DISCONNED (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
        else
        {
            /* return the result of _rec_svc_state_util_close_cm_pipe () */
            i4_ret = i4_ret; 
        }
    }  

    return i4_ret;
} 

static INT32 _rec_svc_state_disconning_on_cm_pipe_closed (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl;
    INT32                       i4_ret;
    
    i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req,
                                               & pt_res_brdcst_svl);

    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Not supported SRC_TYPE. (L%d)\n", __LINE__));
        pt_res_brdcst_svl = NULL;
        i4_ret = SVCTXR_OK;
    }    
    
    /* don't need to check connection handle since it must being disconnected or disconnecting */
    
    if (pt_running_svc_req->e_cond_conn == CM_COND_DISCONNECTED &&
        pt_running_svc_req->e_cond_cm_pipe == CM_COND_CLOSED )
    {
        /* should also check the PAT's condition */
        if ( pt_res_brdcst_svl == NULL     ||
             pt_res_brdcst_svl->e_cond_pat == TM_COND_UNAVAILABLE )
        {
            if (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_INIT_SVC)
            {
                /* can transit to WAIT_RECONN state now */
                
                i4_ret = _rec_svc_state_transit_wait_reconn (pt_svctx);
                
                if (RET_FAIL (i4_ret))
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to DISCONNED (%d). (L%d)\n", i4_ret, __LINE__));
                }
            }
            else /* REC_SVC_SRC_OP_TYPE_DEINIT_SVC */
            {    
                /* can transit to disconnected state now */
                
                i4_ret = _rec_svc_state_transit_disconned (pt_svctx);
                
                if (RET_FAIL (i4_ret))
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to DISCONNED (%d). (L%d)\n", i4_ret, __LINE__));
                }
            }
       }
    }
    else
    {
        /* seems cm pipe is closed earlier than connection */
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Pipe is closed earlier than conn. (L%d)\n", 
                    __LINE__));
        
        i4_ret = SVCTXR_OK; 
    }
    
    return i4_ret; 
}

static INT32 _rec_svc_state_disconning_on_pat_unavail (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl;
    INT32                       i4_ret;
    
    i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req,
                                               & pt_res_brdcst_svl);

    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Not supported SRC_TYPE. (L%d)\n", __LINE__));
        pt_res_brdcst_svl = NULL;
        i4_ret = SVCTXR_OK;
    }    

    if (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_INIT_SVC)
    {
        if (pt_running_svc_req->e_cond_conn == CM_COND_DISCONNECTED &&
            ( pt_res_brdcst_svl == NULL     ||
              pt_res_brdcst_svl->e_cond_pat == TM_COND_UNAVAILABLE ) )
        {
            /* can transit to WAIT_RECONN state now */
            
            i4_ret = _rec_svc_state_transit_wait_reconn (pt_svctx);
            
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to WAIT_RECONN (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }        
    }
    else /* REC_SVC_SRC_OP_TYPE_DEINIT_SVC */
    {
        if (pt_running_svc_req->e_cond_conn == CM_COND_DISCONNECTED &&
            pt_running_svc_req->e_cond_cm_pipe == CM_COND_CLOSED    && 
            ( pt_res_brdcst_svl == NULL     ||
              pt_res_brdcst_svl->e_cond_pat == TM_COND_UNAVAILABLE ) )
        {
            /* can transit to disconnected state now */
            
            i4_ret = _rec_svc_state_transit_disconned (pt_svctx);
            
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to DISCONNED (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }        
    }
    
    return i4_ret;
}

static INT32 _rec_svc_state_wait_reconn_on_req_conn (
    SVCTX_T*                    pt_svctx )
{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32           i4_ret;
    
    if (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_INIT_SVC)
    {
        i4_ret = _rec_svc_state_transit_conning (pt_svctx);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to CONNING (%d). (L%d)\n", i4_ret, __LINE__));
        }
    }
    else
    {
        /* queue this event */
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
    }
        
    return i4_ret;
}

static INT32 _rec_svc_state_wait_reconn_on_req_disconn (
    SVCTX_T*                    pt_svctx )
{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32           i4_ret;

    /* stop the reconnect timer */
    if (pt_running_svc_req->b_on_timer_conn)
    {
        i4_ret = x_timer_stop (pt_running_svc_req->h_timer_conn);
        
        if (i4_ret != OSR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to stop conn timer (%d). (L%d)\n", i4_ret, __LINE__));
        }
        
        pt_running_svc_req->b_on_timer_conn = FALSE;
    }
    else
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
    }
    
    /* close residual resources */
    i4_ret = _rec_svc_state_util_close_cm_pipe (pt_svctx);
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close cm pipe by util (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    /* transit to DISCONNECTED when residual res are all closed */
    if (pt_running_svc_req->e_cond_cm_pipe == CM_COND_CLOSED)
    {
        i4_ret = _rec_svc_state_transit_disconned (pt_svctx);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to DISCONNED (%d). (L%d)\n", i4_ret, __LINE__));
        }
    }
    else
    {
        i4_ret = SVCTXR_OK;
    }
    
    return i4_ret;
}    

static INT32 _rec_svc_state_wait_reconn_on_cm_pipe_closed (
    SVCTX_T*                    pt_svctx )
{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32           i4_ret;
    
    if (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_INIT_SVC)
    {
        /* do nothing, keep in WAIT_RECONN state */
        i4_ret = SVCTXR_OK;
    }
    else /* REC_SVC_SRC_OP_TYPE_DEINIT_SVC */
    {
        i4_ret = _rec_svc_state_transit_disconned (pt_svctx);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to DISCONNED (%d). (L%d)\n", i4_ret, __LINE__));
        }
    }
    
    return i4_ret;
}

static BOOL _rec_svc_state_sess_opening_util_check_ready (
    const SVCTX_T*              pt_svctx )
{
    REC_SVC_REQ_T*      pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    BOOL                b_ret = FALSE;
    
    /* shall wait for the following conditions 
        - already request to open ES
        - already request to open PAT/PMT/PCR
        - session opened
        - REC_UTIL ready
     */
    
    if ( (pt_running_svc_req->ui4_st_flag & REC_SVC_ST_FLAG_SESS_OPENING_REQ_PAT_STRM)      &&
         (pt_running_svc_req->ui4_st_flag & REC_SVC_ST_FLAG_SESS_OPENING_REQ_PMT_STRM)      &&
         (pt_running_svc_req->ui4_st_flag & REC_SVC_ST_FLAG_SESS_OPENING_REQ_PCR_STRM)      &&
         (pt_running_svc_req->ui4_st_flag & REC_SVC_ST_FLAG_SESS_OPENING_REQ_ALL_ES_STRM)   &&
         pt_running_svc_req->e_cond_sess == SM_SESS_COND_OPENED                             &&
         pt_running_svc_req->e_cond_rec_util == REC_UTIL_COND_READY                         &&
         _rec_svc_slst_check_all_opened (pt_running_svc_req) )
    {
        b_ret = TRUE;
    }
    
    return b_ret;
}        

static INT32 _rec_svc_state_sess_opening_on_req_set_es_list (
    SVCTX_T*                    pt_svctx,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data)
{
    REC_SVC_REQ_T*                    pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    SVCTX_REC_REQ_SET_ES_LIST_MSG_T*  pt_msg_info = (SVCTX_REC_REQ_SET_ES_LIST_MSG_T *) pv_data;
    REC_SVC_RES_BRDCST_SVL_T*         pt_res_brdcst_svl;
    INT32                             i4_ret;
    
    if (z_len_data != sizeof (SVCTX_REC_REQ_SET_ES_LIST_MSG_T))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req,
                                               & pt_res_brdcst_svl);
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get RES_BRDCST_SVL (%d). (L%d)\n", i4_ret, __LINE__));
        return i4_ret;
    }

    if (!REC_SVC_SESS_COND_CAN_OPEN_STRM (pt_running_svc_req->e_cond_sess))
    {
        /* simply store it to the pending ES list */
        _rec_svc_state_util_set_pending_es_list (pt_res_brdcst_svl,
                                                 & pt_msg_info->t_es_list);
    }
    else
    {
        /* compare the difference between the new es list and original es list */
        i4_ret = _rec_svc_state_util_update_es_list (pt_svctx,
                                                     & pt_msg_info->t_es_list);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to update ES List (%d). (L%d)\n", i4_ret, __LINE__));
            return i4_ret;        
        }
        
        pt_running_svc_req->ui4_st_flag |= REC_SVC_ST_FLAG_SESS_OPENING_REQ_ALL_ES_STRM;
        
        /* check if ready to transit to next state */
        if (_rec_svc_state_sess_opening_util_check_ready (pt_svctx))
        {
            i4_ret = _rec_svc_state_transit_sess_opened (pt_svctx);
            
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to SESS_OPENED (%d). (L%d)\n", i4_ret, __LINE__));
                return i4_ret;
            }
        }
    }

    return SVCTXR_OK;
}    

static INT32 _rec_svc_state_sess_opening_on_pat_updated (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32           i4_ret;

    /* check if can open PMT stream */
    if (REC_SVC_SESS_COND_CAN_OPEN_STRM (pt_running_svc_req->e_cond_sess))
    {
        i4_ret = _rec_svc_state_util_on_pat_updated (pt_svctx);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle PAT updated (%d). (L%d)\n", i4_ret, __LINE__));
            return i4_ret;
        }
    
        pt_running_svc_req->ui4_st_flag |= REC_SVC_ST_FLAG_SESS_OPENING_REQ_PMT_STRM;
    }
    
    return SVCTXR_OK;
}    

static INT32 _rec_svc_state_sess_opening_on_scdb_updated (
    SVCTX_T*                    pt_svctx,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data)
{
    REC_SVC_REQ_T*      pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    SVCTX_SCDB_MSG_T*   pt_msg_scdb = (SVCTX_SCDB_MSG_T *) pv_data;
    INT32               i4_ret;
    
    if (z_len_data != sizeof (SVCTX_SCDB_MSG_T))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    if (pt_msg_scdb->ui4_reason & (SCDB_RECORD_ADD | SCDB_RECORD_DEL | SCDB_RECORD_MOD))
    {
        /* send request to APP for new ES List */
        if (pt_running_svc_req->pf_rec_nfy)
        {
            if(pt_running_svc_req->pt_rec_cfg != NULL)
            {
                pt_running_svc_req->pf_rec_nfy (pt_running_svc_req->pt_rec_cfg->h_svctx_nfy,
                                            pt_running_svc_req->pv_rec_nfy_tag,
                                            SVCTX_NTFY_CODE_REC_REQ_ES_LIST,
                                            0 );
            }
        }
        else
        {
            DBG_ABORT (DBG_MOD_SVCTX);
        }  
    }
    
    if (pt_msg_scdb->ui4_reason & SCDB_COMMON_MOD)
    {
        /* update PCR stream */
        if (REC_SVC_SESS_COND_CAN_OPEN_STRM (pt_running_svc_req->e_cond_sess))
        {
            i4_ret = _rec_svc_state_util_update_pcr_strm (pt_svctx);
            
            if (RET_OK (i4_ret))
            {
                pt_running_svc_req->ui4_st_flag |= REC_SVC_ST_FLAG_SESS_OPENING_REQ_PCR_STRM;
            }    
            else
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open PCR stream (%d). (L%d)\n", i4_ret, __LINE__));
                return i4_ret;
            }
        }
    }    

    return SVCTXR_OK;
}
        
static INT32 _rec_svc_state_sess_opening_on_rec_util_ready (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*          pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    SM_SESS_RCRD_CFG_T      t_sess_cfg;
    HANDLE_T                h_rcrd_sess;
    INT32                   i4_ret;

    /* open record session */
    t_sess_cfg.ui4_flags          = 0;
    t_sess_cfg.s_name_rec_util    = pt_running_svc_req->pt_rec_cfg->s_db_name;
    t_sess_cfg.ui1_frame_mask     = SM_SESS_RCRD_FRAME_TYPE_MASK_I;
    t_sess_cfg.ui4_ms_tick_period = pt_running_svc_req->pt_rec_cfg->ui4_ms_tick_period;
    t_sess_cfg.ui4_pvr_buf_align  = pt_running_svc_req->pt_rec_cfg->t_strg_desc.ui4_mem_align;
    
    switch (pt_running_svc_req->pt_rec_cfg->e_no_data_opt)
    {
    case SVCTX_REC_NO_DATA_OPT_KEEP_REC:
        t_sess_cfg.e_pvr_overflow_opt = SM_SESS_RCRD_OVERFLOW_OPT_CONTINUOUS_TICK;
        break;
    
    case SVCTX_REC_NO_DATA_OPT_SKIP_REC:
        t_sess_cfg.e_pvr_overflow_opt = SM_SESS_RCRD_OVERFLOW_OPT_CONTINUOUS_TICK;
        break;
        
    default:
        DBG_ABORT (DBG_MOD_SVCTX);
    }
    
    i4_ret = x_sm_open_sess (pt_running_svc_req->h_conn,
                             SM_RCRD_SESS_HDLR_NAME,
                             pt_running_svc_req->t_parent.s_snk_grp_name,
                             (VOID *) & t_sess_cfg,
                             sizeof (SM_SESS_RCRD_CFG_T),
                             pt_running_svc_req->t_parent.i4_speed,
                             _rec_svc_cb_sess_nfy,
                             (VOID *) pt_svctx->h_sys_svctx,
                             & h_rcrd_sess );
    
    if (i4_ret != SMR_OK)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open record session (%d). (L%d)\n", i4_ret, __LINE__));
        return SVCTXR_FAIL;
    }
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Open Record Session (%d). (L%d)\n", 
                h_rcrd_sess,
                __LINE__));
    
    pt_running_svc_req->h_rcrd_sess = h_rcrd_sess;
    pt_running_svc_req->e_cond_sess = SM_SESS_COND_CLOSED;
    
    return SVCTXR_OK;
}

static INT32 _rec_svc_state_sess_opening_on_sess_opened (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl;
    INT32                       i4_ret;

    /* get the svl brdcst res */    
    i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req,
                                               & pt_res_brdcst_svl);
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get RES_BRDCST_SVL (%d). (L%d)\n", i4_ret, __LINE__));
        pt_res_brdcst_svl = NULL;
    }
    
    if (pt_res_brdcst_svl)
    {
        /* open PAT stream */
        i4_ret = _rec_svc_state_util_open_pat_strm (pt_svctx);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open PAT stream (%d). (L%d)\n", i4_ret, __LINE__));
            return i4_ret;
        }
        
        pt_running_svc_req->ui4_st_flag |= REC_SVC_ST_FLAG_SESS_OPENING_REQ_PAT_STRM;
        
        /* check if can open PMT stream */
        if (REC_SVC_TABLE_AVAIL (pt_res_brdcst_svl->e_cond_pat) && 
            REC_SVC_SESS_COND_CAN_OPEN_STRM (pt_running_svc_req->e_cond_sess))
        {
            i4_ret = _rec_svc_state_util_on_pat_updated (pt_svctx);
            
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle PAT updated (%d). (L%d)\n", i4_ret, __LINE__));
                return i4_ret;
            }
        
            pt_running_svc_req->ui4_st_flag |= REC_SVC_ST_FLAG_SESS_OPENING_REQ_PMT_STRM;
        }
        
        /* check if can open PCR stream */
        if (pt_running_svc_req->e_cond_scdb == SCDB_UPDATED)
        {
            i4_ret = _rec_svc_state_util_update_pcr_strm (pt_svctx);
            
            if (RET_OK (i4_ret))
            {
                pt_running_svc_req->ui4_st_flag |= REC_SVC_ST_FLAG_SESS_OPENING_REQ_PCR_STRM;
            }    
            else
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open PCR stream (%d). (L%d)\n", i4_ret, __LINE__));
                return i4_ret;
            }
        }
        
        /* check if can open ES stream now */
        if (pt_res_brdcst_svl->b_es_list_pding)
        {
            /* compare the difference between the new es list and original es list */
            i4_ret = _rec_svc_state_util_update_es_list (pt_svctx,
                                                         & pt_res_brdcst_svl->t_es_list_pding);
            
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to update ES List (%d). (L%d)\n", i4_ret, __LINE__));
                return i4_ret;        
            }
 
            pt_res_brdcst_svl->b_es_list_pding = FALSE;

            pt_running_svc_req->ui4_st_flag |= REC_SVC_ST_FLAG_SESS_OPENING_REQ_ALL_ES_STRM;
            
            /* check if ready to transit to next state */
            if (_rec_svc_state_sess_opening_util_check_ready (pt_svctx))
            {
                i4_ret = _rec_svc_state_transit_sess_opened (pt_svctx);
                
                if (RET_FAIL (i4_ret))
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to SESS_OPENED (%d). (L%d)\n", i4_ret, __LINE__));
                    return i4_ret;
                }
            }
        }        
    }
    
    if (_rec_svc_state_sess_opening_util_check_ready (pt_svctx))
    {
        i4_ret = _rec_svc_state_transit_sess_opened (pt_svctx);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to SESS_OPENED (%d). (L%d)\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
    
    return SVCTXR_OK;
}    
    
static INT32 _rec_svc_state_sess_opening_on_strm_opened (
    SVCTX_T*                    pt_svctx)
{
    INT32           i4_ret;
    
    if (_rec_svc_state_sess_opening_util_check_ready (pt_svctx))
    {
        i4_ret = _rec_svc_state_transit_sess_opened (pt_svctx);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to SESS_OPENED (%d). (L%d)\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
    
    return SVCTXR_OK;
}

static INT32 _rec_svc_state_sess_opening_on_strm_closed (
    SVCTX_T*                    pt_svctx)
{
    INT32           i4_ret;
    
    if (_rec_svc_state_sess_opening_util_check_ready (pt_svctx))
    {
        i4_ret = _rec_svc_state_transit_sess_opened (pt_svctx);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to SESS_OPENED (%d). (L%d)\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
    
    return SVCTXR_OK;
}

static INT32 _rec_svc_state_sess_opened_on_req_start_sess (
    SVCTX_T*                    pt_svctx)
{
    INT32            i4_ret;
    
    i4_ret = _rec_svc_state_transit_sess_starting (pt_svctx);
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit SESS_STARTING (%d). (L%d)\n", i4_ret, __LINE__));
        
        /* return the error value to transit to SESS_ERROR state */
    }
    
    return i4_ret;
}

static INT32 _rec_svc_state_sess_opened_on_close_sess (
    SVCTX_T*                    pt_svctx)
{
    INT32     i4_ret;
    
    i4_ret = _rec_svc_state_transit_sess_closing (pt_svctx);
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit SESS_CLOSING (%d). (L%d)\n", i4_ret, __LINE__));
        /* return the error value to transit to SESS_ERROR state */
    }
    
    return i4_ret;
}    

static INT32 _rec_svc_state_sess_starting_on_sess_started (
    SVCTX_T*                    pt_svctx)
{
    INT32   i4_ret;

    i4_ret = _rec_svc_state_transit_sess_started (pt_svctx);

    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit SESS_STARTED (%d). (L%d)\n", i4_ret, __LINE__));
        
        /* return the error value to transit to SESS_ERROR state */
    }
    
    return i4_ret;
}

static INT32  _rec_svc_state_sess_started_on_stop_sess (
    SVCTX_T*                    pt_svctx,
    BOOL                        b_nfy_err)
{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32           i4_ret;
    
    if (b_nfy_err)
    {
        if (pt_running_svc_req->pf_rec_nfy)
        {
            if(pt_running_svc_req->pt_rec_cfg != NULL)
            {
                pt_running_svc_req->pf_rec_nfy (pt_running_svc_req->pt_rec_cfg->h_svctx_nfy,
                                            pt_running_svc_req->pv_rec_nfy_tag,
                                            SVCTX_NTFY_CODE_REC_INTERNAL_ERROR,
                                            0 );
            }
        }
        else
        {
            DBG_ABORT (DBG_MOD_SVCTX);
        }  
    
        rec_hdlr_invoke_client_svc_nfy_fct (pt_svctx,
                                            SVCTX_NTFY_CODE_REC_INTERNAL_ERROR,
                                            ST_UNKNOWN );
    }
    
    i4_ret = _rec_svc_state_transit_sess_stopping (pt_svctx);
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit SESS_STOPPING (%d). (L%d)\n", i4_ret, __LINE__));
        
        /* return the error value to transit to SESS_ERROR state */
    }
    
    return i4_ret;
}        

static INT32 _rec_svc_state_sess_stopping_on_sess_stopped (
    SVCTX_T*                    pt_svctx)
{
    INT32           i4_ret;
    
    i4_ret = _rec_svc_state_transit_sess_opened (pt_svctx);
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit SESS_OPENED (%d). (L%d)\n", i4_ret, __LINE__));
        
        /* return the error value to transit to SESS_ERROR state */
    }
    
    return i4_ret;
}        

static INT32 _rec_svc_state_sess_closing_on_sess_closed (
    SVCTX_T*                    pt_svctx)
{
    INT32           i4_ret;

    i4_ret = _rec_svc_state_transit_conned (pt_svctx);
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit from SESS_CLOSING to CONNECTED (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    return i4_ret;
}        

static INT32 _rec_svc_state_sess_error_on_sess_closed (
    SVCTX_T*                    pt_svctx)
{
    INT32           i4_ret;

    i4_ret = _rec_svc_state_transit_conned (pt_svctx);
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit from SESS_ERROR to CONNECTED (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    return i4_ret;
}        

static INT32 _rec_svc_state_error_on_res_closed (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl;
    INT32                       i4_ret;

    i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req, 
                                               & pt_res_brdcst_svl );
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get RES_BRDCST_SVL (%d). (L%d)\n", i4_ret, __LINE__));
        pt_res_brdcst_svl = NULL;
    }
    
    /* check if can transit to DISCONNED */
    if (pt_running_svc_req->e_cond_sess == SM_SESS_COND_CLOSED  &&
        pt_running_svc_req->e_cond_conn == CM_COND_DISCONNECTED &&
        pt_running_svc_req->e_cond_cm_pipe == CM_COND_CLOSED    &&
        ( pt_res_brdcst_svl == NULL     ||
          pt_res_brdcst_svl->e_cond_pat == TM_COND_UNAVAILABLE ) )
    {
        if (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_INIT_SVC)
        {
            /* try to reconnect */
            i4_ret = _rec_svc_state_transit_wait_reconn (pt_svctx);
            
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to WAIT_RECONN (%d). (L%d)\n", i4_ret, __LINE__));

                /* force transit to disconnected anyway */
                pt_running_svc_req->e_state     = REC_SVC_STATE_DISCONNECTED;
                pt_running_svc_req->ui4_st_flag = 0;
            }
        }
        else
        {
            /* can directly transit to DISCONNECTED now.. */
            i4_ret = _rec_svc_state_transit_disconned (pt_svctx);
            
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to DISCONNED (%d). (L%d)\n", i4_ret, __LINE__));
                
                /* force transit to disconnected anyway */
                pt_running_svc_req->e_state     = REC_SVC_STATE_DISCONNECTED;
                pt_running_svc_req->ui4_st_flag = 0;
            }
        }
    }
    
    return SVCTXR_OK;
}        

/*-----------------------------------------------------------------------------
                    state event entry function
 ----------------------------------------------------------------------------*/
static INT32 _rec_svc_state_not_init_evn_hdlr (
    SVCTX_T*                    pt_svctx,
    REC_SVC_EVN_TYPE_T          e_event,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data)
{
    INT32           i4_ret = SVCTXR_REC_INTERNAL_ERROR;
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    REC_SVC_REQ_T*  pt_pending_svc_req = (REC_SVC_REQ_T*)pt_svctx->pt_pending_svc_req;
    
    switch (e_event)
    {
    case REC_SVC_EVN_REQ_INIT_SVC:
        i4_ret = _rec_svc_state_transit_disconned (pt_svctx);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to DISCONNED (%d). (L%d)\n", i4_ret, __LINE__));
            
            /* _rec_svc_state_transit_disconned() will make sure to keep in NOT_INIT state if encountering fail */
            /* The result just like the event is ignored */
            i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        }
        break;
        
    case REC_SVC_EVN_REQ_DEINIT_SVC:
        
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Preset conn info 0x%X->0x%X "
                           "(h_pipe=%d, e_cond_pipe=%d, h_conn=%d, reuse_pipe=%s). (L%d)\n", 
                           (UINT32) pt_running_svc_req,
                           (UINT32) pt_pending_svc_req,
                           pt_running_svc_req->t_conn_info_preset.h_cm_pipe,
                           pt_running_svc_req->t_conn_info_preset.e_cond_cm_pipe,
                           pt_running_svc_req->t_conn_info_preset.h_conn,
                           (((pt_running_svc_req->ui4_src_op_flag) & REC_SVC_DEINIT_SVC_FLAG_REUSE_PIPE) ?  "True":"False"),
                           __LINE__));
        if  (((pt_running_svc_req->ui4_src_op_flag) & REC_SVC_DEINIT_SVC_FLAG_REUSE_PIPE)
            && (x_handle_valid(pt_running_svc_req->t_conn_info_preset.h_cm_pipe))
            && (x_handle_valid(pt_running_svc_req->t_conn_info_preset.h_conn))
            && (pt_pending_svc_req != NULL))
        {
            
            i4_ret = rec_svc_preset_conn_info (pt_pending_svc_req,
                                                               TRUE,
                                                               & (pt_running_svc_req->t_conn_info_preset) );
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to preset conn info (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
        else
        {
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        }
        break;
        
    case REC_SVC_EVN_REQ_CONNECT:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
   
    case REC_SVC_EVN_REQ_DISCONNECT:
        if (!(pt_running_svc_req->b_pipe_real_close && pt_running_svc_req->b_cm_real_disc))
        {
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        }
        else
        {
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] really do cm_disconnect  0x%X->0x%X "
                               "(h_pipe=%d, e_cond_pipe=%d, h_conn=%d, reuse_pipe=%s). (L%d)\n", 
                               (UINT32) pt_running_svc_req,
                               (UINT32) pt_pending_svc_req,
                               pt_running_svc_req->h_cm_pipe,
                               pt_running_svc_req->e_cond_cm_pipe,
                               pt_running_svc_req->h_conn,
                               (((pt_running_svc_req->ui4_src_op_flag) & REC_SVC_DEINIT_SVC_FLAG_REUSE_PIPE) ?  "True":"False"),
                               __LINE__));
            /* try to close connection */    
            i4_ret = _rec_svc_state_util_close_conn (pt_svctx, 
                                                                         TRUE, 
                                                                         ALSO_PIPE, 
                                                                         NULL);
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to disconnet connection (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
        break;
        
    case REC_SVC_EVN_REQ_OPEN_SESS:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_START_SESS:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
   
    case REC_SVC_EVN_REQ_STOP_SESS:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REQ_CLOSE_SESS:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REQ_SET_ES_LIST:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Set ES List when NOT_INIT. (L%d)\n", 
                    __LINE__));
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_QUEUED_OPEN_PIPE:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_QUEUED_CLOSE_PIPE:
        if (pt_running_svc_req->b_pipe_real_close)
        {
            DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] close reused pipe (%d). (L%d)\n", 
                                              pt_running_svc_req->h_cm_pipe, __LINE__));
            i4_ret = _rec_svc_state_util_close_cm_pipe (pt_svctx);
        }
        else
        {
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        }
        break;
        
    case REC_SVC_EVN_QUEUED_OPEN_CONN:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
    
    case REC_SVC_EVN_QUEUED_CLOSE_CONN:
        if (pt_running_svc_req->b_cm_real_disc)
        {
            DBG_INFO (("{SVCTX}  INFO: [REC_HDLR]  disconnet reused connection (%d). (L%d)\n", 
                                              pt_running_svc_req->h_conn, __LINE__));
            i4_ret = _rec_svc_state_util_close_conn (pt_svctx, TRUE, ALSO_PIPE, NULL);
        }
        else
        {
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        }
        break;

    case REC_SVC_EVN_QREQ_REMOVED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_ERROR:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
    
    case REC_SVC_EVN_CM_PIPE_OPENED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_CM_PIPE_CLOSED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_CONN_CONNECTED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_CONN_DISCONNECTED:
         if (pt_running_svc_req->b_pipe_real_close)
         {
             DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] close reused pipe (%d). (L%d)\n", 
                                               pt_running_svc_req->h_cm_pipe, __LINE__));
             /* close cm pipe if necessary */
             i4_ret = _rec_svc_state_util_close_cm_pipe (pt_svctx);
             
             if (RET_FAIL (i4_ret))
             {
                 DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close cm pipe (%d). (L%d)\n", i4_ret, __LINE__));
             }
         }
         else
         {
             DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
             i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
         }

    case REC_SVC_EVN_PAT_UPDATED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_PAT_UNAVAIL:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SCDB_UPDATED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
    
    case REC_SVC_EVN_REC_UTIL_TRANS:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_REC_UTIL_READY:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SVC_BLOCKED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SESS_OPENED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SESS_STARTED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
    
    case REC_SVC_EVN_SESS_STOPPED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SESS_CLOSED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SESS_ERROR:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_STRM_OPENED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_STRM_CLOSED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_STRM_ERROR:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REUSE_PIPE_TO_NOT_REUSED:
        i4_ret = _rec_svc_state_transit_force_conned (pt_svctx);
        break;
        
    default:
        DBG_ABORT (DBG_MOD_SVCTX);
    }
    
    return i4_ret;
}
    
static INT32 _rec_svc_state_disconned_evn_hdlr (
    SVCTX_T*                    pt_svctx,
    REC_SVC_EVN_TYPE_T          e_event,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data)
{
    INT32           i4_ret = SVCTXR_REC_INTERNAL_ERROR;
    
    switch (e_event)
    {
    case REC_SVC_EVN_REQ_INIT_SVC:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REQ_DEINIT_SVC:
        /* _rec_svc_state_transit_not_init () must be successful, 
           so don't need to return any value */
        _rec_svc_state_transit_not_init (pt_svctx);
        i4_ret = SVCTXR_REC_MSG_SVC_DEINITED;
        break;
                
    case REC_SVC_EVN_REQ_CONNECT:
        i4_ret = _rec_svc_state_transit_conning (pt_svctx);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to CONNING (%d). (L%d)\n", i4_ret, __LINE__));
        }
        break;
        
    case REC_SVC_EVN_REQ_DISCONNECT:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
                
    case REC_SVC_EVN_REQ_OPEN_SESS:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_START_SESS:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_STOP_SESS:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
   
    case REC_SVC_EVN_REQ_CLOSE_SESS:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REQ_SET_ES_LIST:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Set ES List when DISCONNECTED. (L%d)\n", 
                    __LINE__));
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_OPEN_PIPE:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_CLOSE_PIPE:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_OPEN_CONN:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_CLOSE_CONN:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QREQ_REMOVED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_ERROR:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_CM_PIPE_OPENED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_CM_PIPE_CLOSED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_CONN_CONNECTED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_CONN_DISCONNECTED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_PAT_UPDATED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_PAT_UNAVAIL:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SCDB_UPDATED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REC_UTIL_TRANS:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REC_UTIL_READY:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SVC_BLOCKED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SESS_OPENED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SESS_STARTED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SESS_STOPPED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SESS_CLOSED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SESS_ERROR:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_STRM_OPENED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_STRM_CLOSED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_STRM_ERROR:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REUSE_PIPE_TO_NOT_REUSED:
        i4_ret = _rec_svc_state_transit_force_conned (pt_svctx);
        break;

    default:
        DBG_ABORT (DBG_MOD_SVCTX);
    }

    return i4_ret;
}

static INT32 _rec_svc_state_conning_evn_hdlr (
    SVCTX_T*                    pt_svctx,
    REC_SVC_EVN_TYPE_T          e_event,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data)
{
    INT32           i4_ret = SVCTXR_REC_INTERNAL_ERROR;

    switch (e_event)
    {
    case REC_SVC_EVN_REQ_INIT_SVC:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REQ_DEINIT_SVC:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;

    case REC_SVC_EVN_REQ_CONNECT:
        /*i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
               break;*/
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_DISCONNECT:
       i4_ret = _rec_svc_state_conning_on_req_disconn(pt_svctx);
        break;

    case REC_SVC_EVN_REQ_OPEN_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;

    case REC_SVC_EVN_REQ_START_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_STOP_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_CLOSE_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_SET_ES_LIST:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Set ES List when CONNECTING. (L%d)\n", 
                    __LINE__));
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_OPEN_PIPE:
        i4_ret = _rec_svc_state_conning_util_open_pipe (pt_svctx);
        break;

    case REC_SVC_EVN_QUEUED_CLOSE_PIPE:
        i4_ret = _rec_svc_state_util_close_cm_pipe (pt_svctx);
        break;

    case REC_SVC_EVN_QUEUED_OPEN_CONN:
        i4_ret = _rec_svc_state_conning_util_open_conn (pt_svctx);
        break;

    case REC_SVC_EVN_QUEUED_CLOSE_CONN:
        i4_ret = _rec_svc_state_util_close_conn (pt_svctx, TRUE, ALSO_PIPE, NULL);
        break;

    case REC_SVC_EVN_QREQ_REMOVED:
        i4_ret = _rec_svc_state_util_on_qreq_removed (pt_svctx);
        break;
        
    case REC_SVC_EVN_ERROR:
        _rec_svc_state_transit_error (pt_svctx);
        i4_ret = SVCTXR_OK;
        break;
        
    case REC_SVC_EVN_CM_PIPE_OPENED:
        i4_ret = _rec_svc_state_conning_on_cm_pipe_opened (pt_svctx);
        break;

    case REC_SVC_EVN_CM_PIPE_CLOSED:
        i4_ret = _rec_svc_state_conning_on_cm_pipe_closed (pt_svctx);
        break;
    
    case REC_SVC_EVN_CONN_CONNECTED:
        i4_ret = _rec_svc_state_conning_on_conn_conned (pt_svctx, 
                                                        pv_data, 
                                                        z_len_data);
        break;
    
    case REC_SVC_EVN_CONN_DISCONNECTED:
        i4_ret = _rec_svc_state_conning_on_conn_disconned (pt_svctx);
        break;
        
    case REC_SVC_EVN_PAT_UPDATED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_PAT_UNAVAIL:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SCDB_UPDATED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REC_UTIL_TRANS:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REC_UTIL_READY:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SVC_BLOCKED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
    
    case REC_SVC_EVN_SESS_OPENED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SESS_STARTED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SESS_STOPPED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SESS_CLOSED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SESS_ERROR:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_STRM_OPENED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_STRM_CLOSED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_STRM_ERROR:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_REUSE_PIPE_TO_NOT_REUSED:
        i4_ret = _rec_svc_state_transit_force_conned (pt_svctx);
        break;
        
    default:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Un-handled event type (%d). (L%d)\n", e_event, __LINE__));
        
        /* ignore the unhandled event */
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
    }
    
    return i4_ret;
}    

static INT32 _rec_svc_state_conned_evn_hdlr (
    SVCTX_T*                    pt_svctx,
    REC_SVC_EVN_TYPE_T          e_event,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data)
{
    INT32           i4_ret = SVCTXR_REC_INTERNAL_ERROR;
    
    switch (e_event)
    {
    case REC_SVC_EVN_REQ_INIT_SVC:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REQ_DEINIT_SVC:
        i4_ret = _rec_svc_state_conned_on_disconn (pt_svctx);
        if (i4_ret == SVCTXR_OK)
        {
            i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        }  
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to disconnect (%d). (L%d)\n", i4_ret, __LINE__));
            
            /* this is a special case in which we need to transit to error state 
               and inform to queue this event */
            _rec_svc_state_transit_error (pt_svctx);
            
            i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        }
        break;
        
    case REC_SVC_EVN_REQ_CONNECT:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_DISCONNECT:
        i4_ret = _rec_svc_state_conned_on_disconn (pt_svctx);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to disconnect (%d). (L%d)\n", i4_ret, __LINE__));
        }
        break;
        
    case REC_SVC_EVN_REQ_OPEN_SESS:
        i4_ret = _rec_svc_state_conned_on_req_open_sess (pt_svctx, 
                                                         pv_data, 
                                                         z_len_data);
        break;
        
    case REC_SVC_EVN_REQ_START_SESS:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_STOP_SESS:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_CLOSE_SESS:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_SET_ES_LIST:
        /* report error that the set es list request is not processed */
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Set ES List when REC_SVC_STATE_CONNECTED. (L%d)\n", 
                    __LINE__));
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_QUEUED_OPEN_PIPE:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_CLOSE_PIPE:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_OPEN_CONN:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_QUEUED_CLOSE_CONN:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QREQ_REMOVED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_ERROR:
        _rec_svc_state_transit_error (pt_svctx);
        i4_ret = SVCTXR_OK;
        break;
        
    case REC_SVC_EVN_CM_PIPE_OPENED:
        DBG_INFO_EVN_AT_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
   case REC_SVC_EVN_CM_PIPE_CLOSED:
        i4_ret = _rec_svc_state_conned_on_disconn (pt_svctx);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to disconnect (%d). (L%d)\n", i4_ret, __LINE__));
        }
        break;

    case REC_SVC_EVN_CONN_CONNECTED:
        DBG_INFO_EVN_AT_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_CONN_DISCONNECTED:
        i4_ret = _rec_svc_state_conned_on_disconn (pt_svctx);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to disconnect (%d). (L%d)\n", i4_ret, __LINE__));
        }
        break;
        
    case REC_SVC_EVN_PAT_UPDATED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
        
    case REC_SVC_EVN_PAT_UNAVAIL:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
        
    case REC_SVC_EVN_SCDB_UPDATED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REC_UTIL_TRANS:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_REC_UTIL_READY:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SVC_BLOCKED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SESS_OPENED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SESS_STARTED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SESS_STOPPED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SESS_CLOSED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SESS_ERROR:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_STRM_OPENED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_STRM_CLOSED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_STRM_ERROR:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    default:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Un-handled event type (%d). (L%d)\n", e_event, __LINE__));
        
        /* ignore the unhandled event */
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
    }
    
    return i4_ret;
}

static INT32 _rec_svc_state_disconning_evn_hdlr (
    SVCTX_T*                    pt_svctx,
    REC_SVC_EVN_TYPE_T          e_event,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data)
{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32           i4_ret = SVCTXR_REC_INTERNAL_ERROR;
    
    switch (e_event)
    {
    case REC_SVC_EVN_REQ_INIT_SVC:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REQ_DEINIT_SVC:
        i4_ret = _rec_svc_state_disconning_on_deinit_svc (pt_svctx);
        break;
        
    case REC_SVC_EVN_REQ_CONNECT:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;

    case REC_SVC_EVN_REQ_DISCONNECT:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_OPEN_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_START_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_STOP_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_CLOSE_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_SET_ES_LIST:
        /* report error that the set es list request is not processed */
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Set ES List when DISCONNECTING. (L%d)\n", 
                    __LINE__));
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_QUEUED_OPEN_PIPE:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_CLOSE_PIPE:
        i4_ret = _rec_svc_state_util_close_cm_pipe (pt_svctx);
        break;

    case REC_SVC_EVN_QUEUED_OPEN_CONN:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_CLOSE_CONN:
        i4_ret = _rec_svc_state_util_close_conn (
                pt_svctx, 
                TRUE, 
                (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_INIT_SVC) ? NOT_ALSO_PIPE : ALSO_PIPE,
                NULL);
        break;
        
    case REC_SVC_EVN_QREQ_REMOVED:
        i4_ret = _rec_svc_state_util_on_qreq_removed (pt_svctx);
        break;

    case REC_SVC_EVN_ERROR:
        _rec_svc_state_transit_error (pt_svctx);
        i4_ret = SVCTXR_OK;
        break;
        
    case REC_SVC_EVN_CM_PIPE_OPENED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_CM_PIPE_CLOSED:
        i4_ret = _rec_svc_state_disconning_on_cm_pipe_closed (pt_svctx);
        break;
        
    case REC_SVC_EVN_CONN_CONNECTED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_CONN_DISCONNECTED:
        i4_ret = _rec_svc_state_disconning_on_conn_disconned (pt_svctx);
        break;
        
    case REC_SVC_EVN_PAT_UPDATED:
        /* this should be late comming event */
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
    
    case REC_SVC_EVN_PAT_UNAVAIL:
        i4_ret = _rec_svc_state_disconning_on_pat_unavail (pt_svctx);
        break;

    case REC_SVC_EVN_SCDB_UPDATED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REC_UTIL_TRANS:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_REC_UTIL_READY:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SVC_BLOCKED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SESS_OPENED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SESS_STARTED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SESS_STOPPED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SESS_CLOSED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SESS_ERROR:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_STRM_OPENED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_STRM_CLOSED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_STRM_ERROR:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
    case REC_SVC_EVN_REUSE_PIPE_TO_NOT_REUSED:
        i4_ret = _rec_svc_state_transit_force_conned (pt_svctx);
        break;
        
    default:
        DBG_ABORT (DBG_MOD_SVCTX);
    }
    
    return i4_ret;
}

static INT32 _rec_svc_state_wait_reconn_evn_hdlr (
    SVCTX_T*                    pt_svctx,
    REC_SVC_EVN_TYPE_T          e_event,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data)
{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32           i4_ret = SVCTXR_REC_INTERNAL_ERROR;
    
    switch (e_event)
    {
    case REC_SVC_EVN_REQ_INIT_SVC:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REQ_DEINIT_SVC:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_CONNECT:
        if (pt_running_svc_req->b_on_timer_conn)
        {
            i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        }
        else
        {
            i4_ret = _rec_svc_state_wait_reconn_on_req_conn (pt_svctx);
        }
        break;
        
    case REC_SVC_EVN_REQ_DISCONNECT:
        i4_ret = _rec_svc_state_wait_reconn_on_req_disconn (pt_svctx);
        break;
        
    case REC_SVC_EVN_REQ_OPEN_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_START_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_STOP_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_CLOSE_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_SET_ES_LIST:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Set ES List when WAIT_RECONN. (L%d)\n", 
                    __LINE__));
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_QUEUED_OPEN_PIPE:
        /* should see this event in CONNECTING state only */
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_CLOSE_PIPE:
        if (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_DEINIT_SVC)
        {
            i4_ret = _rec_svc_state_util_close_cm_pipe (pt_svctx);
        }
        else
        {
            /* should see this event when source op is DISCONNECT only */
            DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
            i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        }
        break;

    case REC_SVC_EVN_QUEUED_OPEN_CONN:
        /* should see this event in CONNECTING state only */
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_QUEUED_CLOSE_CONN:
        /* should see this event in DISCONNECTING state only */
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_QREQ_REMOVED:
        i4_ret = _rec_svc_state_util_on_qreq_removed (pt_svctx);
        break;

    case REC_SVC_EVN_ERROR:
        _rec_svc_state_transit_error (pt_svctx);
        i4_ret = SVCTXR_OK;
        break;
        
    case REC_SVC_EVN_CM_PIPE_OPENED:
        /* should see this event in CONNECTING state only */
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_CM_PIPE_CLOSED:
        if (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_DEINIT_SVC)
        {
            i4_ret = _rec_svc_state_wait_reconn_on_cm_pipe_closed (pt_svctx);
        }
        else
        {
            /* should see this event when source op is DISCONNECT only */
            DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
            i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        }
        break;

    case REC_SVC_EVN_CONN_CONNECTED:
        /* should see this event in CONNECTING state only */
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_CONN_DISCONNECTED:
        /* should see this event in DISCONNECTING state only */
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_PAT_UPDATED:
        /* connection is disconnected currently, should not receive this event */
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;        
        
    case REC_SVC_EVN_PAT_UNAVAIL:
        /* should see this event in DISCONNECTING state only */
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;        
        
    case REC_SVC_EVN_SCDB_UPDATED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REC_UTIL_TRANS:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_REC_UTIL_READY:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SVC_BLOCKED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SESS_OPENED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SESS_STARTED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SESS_STOPPED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SESS_CLOSED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SESS_ERROR:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_STRM_OPENED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_STRM_CLOSED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_STRM_ERROR:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_REUSE_PIPE_TO_NOT_REUSED:
        i4_ret = _rec_svc_state_transit_force_conned (pt_svctx);
        break;
        
    default:
        DBG_ABORT (DBG_MOD_SVCTX);
    }
    
    return i4_ret;    
}
    
static INT32 _rec_svc_state_sess_opening_evn_hdlr (
    SVCTX_T*                    pt_svctx,
    REC_SVC_EVN_TYPE_T          e_event,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data)
{
    INT32           i4_ret = SVCTXR_REC_INTERNAL_ERROR;
    
    switch (e_event)
    {
    case REC_SVC_EVN_REQ_INIT_SVC:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REQ_DEINIT_SVC:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;

    case REC_SVC_EVN_REQ_CONNECT:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;

    case REC_SVC_EVN_REQ_DISCONNECT:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_OPEN_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_START_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_STOP_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;

    case REC_SVC_EVN_REQ_CLOSE_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;

    case REC_SVC_EVN_REQ_SET_ES_LIST:
        i4_ret = _rec_svc_state_sess_opening_on_req_set_es_list (pt_svctx,
                                                                 pv_data,
                                                                 z_len_data);
        break;

    case REC_SVC_EVN_QUEUED_OPEN_PIPE:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_CLOSE_PIPE:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_OPEN_CONN:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_CLOSE_CONN:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QREQ_REMOVED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_ERROR:
        _rec_svc_state_transit_error (pt_svctx);
        i4_ret = SVCTXR_OK;
        break;

    case REC_SVC_EVN_CM_PIPE_OPENED:
        DBG_INFO_EVN_AT_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_CM_PIPE_CLOSED:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;

    case REC_SVC_EVN_CONN_CONNECTED:
        DBG_INFO_EVN_AT_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_CONN_DISCONNECTED:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;

    case REC_SVC_EVN_PAT_UPDATED:
        i4_ret = _rec_svc_state_sess_opening_on_pat_updated (pt_svctx);
        break;

    case REC_SVC_EVN_PAT_UNAVAIL:
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] PAT Unavailable received at state (%d). (L%d)\n", 
                    ((REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req)->e_state,
                    __LINE__));
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        

    case REC_SVC_EVN_SCDB_UPDATED:
        i4_ret = _rec_svc_state_sess_opening_on_scdb_updated (pt_svctx, pv_data, z_len_data);
        break;

    case REC_SVC_EVN_REC_UTIL_TRANS:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_REC_UTIL_READY:
        i4_ret = _rec_svc_state_sess_opening_on_rec_util_ready (pt_svctx);
        break;
    
    case REC_SVC_EVN_SVC_BLOCKED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SESS_OPENED:
        i4_ret = _rec_svc_state_sess_opening_on_sess_opened (pt_svctx);
        break;
    
    case REC_SVC_EVN_SESS_STARTED:

        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        
        /* return the error value to transit to SESS_ERROR state */
        i4_ret = SVCTXR_FAIL;
        break;
    
    case REC_SVC_EVN_SESS_STOPPED:

        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        
        /* return the error value to transit to SESS_ERROR state */
        i4_ret = SVCTXR_FAIL;
        break;
    
    case REC_SVC_EVN_SESS_CLOSED:
        /* There're 2 possible cases:
           (1) SM_SESS_COND_OPENING->SM_SESS_COND_OPENED->SM_SESS_COND_CLOSING->SM_SESS_COND_CLOSED
           (2) SM_SESS_COND_OPENING->SM_SESS_COND_ERROR->SM_SESS_COND_CLOSING->SM_SESS_COND_CLOSED
           Here must be the case (1), so queue it up */
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
    
    case REC_SVC_EVN_SESS_ERROR:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Received RCRD_SESS_ERROR at state (%d). (L%d)\n", 
                    ((REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req)->e_state,
                    __LINE__));

        /* transit to REC_SVC_STATE_SESS_ERROR state */
        i4_ret = _rec_svc_state_transit_sess_error (pt_svctx);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit SESS_ERROR (%d). (L%d)\n", i4_ret, __LINE__));
            
            /* return the value to transit to REC_SVC_STATE_ERROR state */
        }
        break;
    
    case REC_SVC_EVN_STRM_OPENED:
        i4_ret = _rec_svc_state_sess_opening_on_strm_opened (pt_svctx);
        break;

    case REC_SVC_EVN_STRM_CLOSED:
        i4_ret = _rec_svc_state_sess_opening_on_strm_closed (pt_svctx);
        break;        
        
    case REC_SVC_EVN_STRM_ERROR:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
        
    default:
        DBG_ABORT (DBG_MOD_SVCTX);
    }
    
    return i4_ret;
}    

static INT32 _rec_svc_state_sess_opened_evn_hdlr (
    SVCTX_T*                    pt_svctx,
    REC_SVC_EVN_TYPE_T          e_event,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data)
{    
    INT32           i4_ret = SVCTXR_REC_INTERNAL_ERROR;
    
    switch (e_event)
    {
    case REC_SVC_EVN_REQ_INIT_SVC:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REQ_DEINIT_SVC:
        i4_ret = _rec_svc_state_sess_opened_on_close_sess (pt_svctx);
        if (i4_ret == SVCTXR_OK)
        {
            i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close session (%d). (L%d)\n", i4_ret, __LINE__));
            
            /* this is a special case in which we need to transit to error state 
               and inform to queue this event */
            _rec_svc_state_transit_error (pt_svctx);
            
            i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        }
        break;
        
    case REC_SVC_EVN_REQ_CONNECT:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REQ_DISCONNECT:
        i4_ret = _rec_svc_state_sess_opened_on_close_sess (pt_svctx);
        if (i4_ret == SVCTXR_OK)
        {
            i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close session (%d). (L%d)\n", i4_ret, __LINE__));
            
            /* return fail and go to ERROR state */
        }
        break;
        
    case REC_SVC_EVN_REQ_OPEN_SESS:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_START_SESS:
        i4_ret = _rec_svc_state_sess_opened_on_req_start_sess (pt_svctx);
        break;
        
    case REC_SVC_EVN_REQ_STOP_SESS:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REQ_CLOSE_SESS:
        i4_ret = _rec_svc_state_sess_opened_on_close_sess (pt_svctx);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close session (%d). (L%d)\n", i4_ret, __LINE__));
        }
        break;
        
    case REC_SVC_EVN_REQ_SET_ES_LIST:
        i4_ret = _rec_svc_state_util_on_req_set_es_list (pt_svctx,
                                                         pv_data,
                                                         z_len_data);
                                                         
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to set ES List (%d). (L%d)\n", i4_ret, __LINE__));
        }                                                         
        break;

    case REC_SVC_EVN_QUEUED_OPEN_PIPE:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_QUEUED_CLOSE_PIPE:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_OPEN_CONN:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_QUEUED_CLOSE_CONN:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QREQ_REMOVED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_ERROR:
        _rec_svc_state_transit_error (pt_svctx);
        i4_ret = SVCTXR_OK;
        break;
    
    case REC_SVC_EVN_CM_PIPE_OPENED:
        DBG_INFO_EVN_AT_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
    
    case REC_SVC_EVN_CM_PIPE_CLOSED:
        i4_ret = _rec_svc_state_sess_opened_on_close_sess (pt_svctx);
        if (i4_ret == SVCTXR_OK)
        {
            i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close session (%d). (L%d)\n", i4_ret, __LINE__));
            
            /* return fail and go to ERROR state */
        }
        break;
    
    case REC_SVC_EVN_CONN_CONNECTED:
        DBG_INFO_EVN_AT_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
    
    case REC_SVC_EVN_CONN_DISCONNECTED:
        i4_ret = _rec_svc_state_sess_opened_on_close_sess (pt_svctx);
        if (i4_ret == SVCTXR_OK)
        {
            i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close session (%d). (L%d)\n", i4_ret, __LINE__));
            
            /* return fail and go to ERROR state */
        }
        break;
    
    case REC_SVC_EVN_PAT_UPDATED:
        i4_ret = _rec_svc_state_util_on_pat_updated (pt_svctx);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle PAT updated event (%d). (L%d)\n", i4_ret, __LINE__));
        }
        break;

    case REC_SVC_EVN_PAT_UNAVAIL:       
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
        
    case REC_SVC_EVN_SCDB_UPDATED:
        i4_ret = _rec_svc_state_util_sess_on_scdb_updated (pt_svctx, pv_data, z_len_data);
        break;        
    
    case REC_SVC_EVN_REC_UTIL_TRANS:
        i4_ret = _rec_svc_state_sess_opened_on_close_sess (pt_svctx);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close session (%d). (L%d)\n", i4_ret, __LINE__));
            
            /* return fail and go to ERROR state */
        }
        break;
    
    case REC_SVC_EVN_REC_UTIL_READY:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
    
    case REC_SVC_EVN_SVC_BLOCKED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
    
    case REC_SVC_EVN_SESS_OPENED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        
        /* return the error value to transit to SESS_ERROR state */
        i4_ret = SVCTXR_FAIL;
        break;
    
    case REC_SVC_EVN_SESS_STARTED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        
        /* return the error value to transit to SESS_ERROR state */
        i4_ret = SVCTXR_FAIL;
        break;
    
    case REC_SVC_EVN_SESS_STOPPED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        
        /* return the error value to transit to SESS_ERROR state */
        i4_ret = SVCTXR_FAIL;
        break;
        
    case REC_SVC_EVN_SESS_CLOSED:
        i4_ret = _rec_svc_state_sess_opened_on_close_sess (pt_svctx);
        if (RET_OK (i4_ret))
        {
            i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle EVN_SESS_CLOSED in SESS_OPENED (%d). (L%d)\n", i4_ret, __LINE__));
            
            /* return fail and go to ERROR state */
        }
        break;        
    
    case REC_SVC_EVN_SESS_ERROR:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Received RCRD_SESS_ERROR at state (%d). (L%d)\n", 
                    ((REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req)->e_state,
                    __LINE__));

        /* transit to REC_SVC_STATE_SESS_ERROR state */
        i4_ret = _rec_svc_state_transit_sess_error (pt_svctx);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit SESS_ERROR (%d). (L%d)\n", i4_ret, __LINE__));
            
            /* transit to REC_SVC_STATE_ERROR state */
        }
        break;
    
    case REC_SVC_EVN_STRM_OPENED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
    
    case REC_SVC_EVN_STRM_CLOSED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
    
    case REC_SVC_EVN_STRM_ERROR:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
    
    default:
        DBG_ABORT (DBG_MOD_SVCTX);
    }
    
    return i4_ret;
}    

static INT32 _rec_svc_state_sess_starting_evn_hdlr (
    SVCTX_T*                    pt_svctx,
    REC_SVC_EVN_TYPE_T          e_event,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data)
{
    INT32           i4_ret = SVCTXR_REC_INTERNAL_ERROR;
    
    switch (e_event)
    {
    case REC_SVC_EVN_REQ_INIT_SVC:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);

        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REQ_DEINIT_SVC:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;

    case REC_SVC_EVN_REQ_CONNECT:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;

    case REC_SVC_EVN_REQ_DISCONNECT:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;

    case REC_SVC_EVN_REQ_OPEN_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_START_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_STOP_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_CLOSE_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_SET_ES_LIST:
        i4_ret = _rec_svc_state_util_on_req_set_es_list (pt_svctx,
                                                         pv_data,
                                                         z_len_data);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to set ES List (%d). (L%d)\n", i4_ret, __LINE__));
        }                                                         
        break;

    case REC_SVC_EVN_QUEUED_OPEN_PIPE:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_CLOSE_PIPE:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_OPEN_CONN:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_CLOSE_CONN:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QREQ_REMOVED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_ERROR:
        _rec_svc_state_transit_error (pt_svctx);
        i4_ret = SVCTXR_OK;
        break;
        
    case REC_SVC_EVN_CM_PIPE_OPENED:
        DBG_INFO_EVN_AT_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_CM_PIPE_CLOSED:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_CONN_CONNECTED:
        DBG_INFO_EVN_AT_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_CONN_DISCONNECTED:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_PAT_UPDATED:
        i4_ret = _rec_svc_state_util_on_pat_updated (pt_svctx);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle PAT updated event (%d). (L%d)\n", i4_ret, __LINE__));
        }
        break;

    case REC_SVC_EVN_PAT_UNAVAIL:
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] PAT Unavailable received at state (%d). (L%d)\n", 
                    ((REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req)->e_state,
                    __LINE__));
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
        
    case REC_SVC_EVN_SCDB_UPDATED:
        i4_ret = _rec_svc_state_util_sess_on_scdb_updated (pt_svctx, pv_data, z_len_data);
        break;        
        
    case REC_SVC_EVN_REC_UTIL_TRANS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REC_UTIL_READY:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SVC_BLOCKED:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;

    case REC_SVC_EVN_SESS_OPENED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        
        /* return the error value to transit to SESS_ERROR state */
        i4_ret = SVCTXR_FAIL;
        break;
        
    case REC_SVC_EVN_SESS_STARTED:
        i4_ret = _rec_svc_state_sess_starting_on_sess_started (pt_svctx);
        break;
    
    case REC_SVC_EVN_SESS_STOPPED:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;

    case REC_SVC_EVN_SESS_CLOSED:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
    
    case REC_SVC_EVN_SESS_ERROR:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Received RCRD_SESS_ERROR at state (%d). (L%d)\n", 
                    ((REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req)->e_state,
                    __LINE__));

        /* transit to REC_SVC_STATE_SESS_ERROR state */
        i4_ret = _rec_svc_state_transit_sess_error (pt_svctx);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit SESS_ERROR (%d). (L%d)\n", i4_ret, __LINE__));
            
            /* transit to REC_SVC_STATE_ERROR state */
        }
        break;
    
    case REC_SVC_EVN_STRM_OPENED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
    
    case REC_SVC_EVN_STRM_CLOSED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
    
    case REC_SVC_EVN_STRM_ERROR:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        

    default:
        DBG_ABORT (DBG_MOD_SVCTX);
    }

    return i4_ret;
}

static INT32 _rec_svc_state_sess_started_evn_hdlr (
    SVCTX_T*                    pt_svctx,
    REC_SVC_EVN_TYPE_T          e_event,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data)
{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32           i4_ret = SVCTXR_REC_INTERNAL_ERROR;
    
    switch (e_event)
    {
    case REC_SVC_EVN_REQ_INIT_SVC:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REQ_DEINIT_SVC:
        i4_ret = _rec_svc_state_sess_started_on_stop_sess (pt_svctx, NOT_NFY_ERR);
        if (i4_ret == SVCTXR_OK)
        {
            i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to stop session (%d). (L%d)\n", i4_ret, __LINE__));
            
            /* this is a special case in which we need to transit to error state 
               and inform to queue this event */
            _rec_svc_state_transit_error (pt_svctx);
            
            i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        }
        break;

    case REC_SVC_EVN_REQ_CONNECT:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_DISCONNECT:
        i4_ret = _rec_svc_state_sess_started_on_stop_sess (pt_svctx, NOT_NFY_ERR);
        if (i4_ret == SVCTXR_OK)
        {
            i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close session (%d). (L%d)\n", i4_ret, __LINE__));
            
            /* return fail and go to ERROR state */
        }
        break;
        
    case REC_SVC_EVN_REQ_OPEN_SESS:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_START_SESS:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_STOP_SESS:
        i4_ret = _rec_svc_state_sess_started_on_stop_sess (pt_svctx, NOT_NFY_ERR);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to stop session (%d). (L%d)\n", i4_ret, __LINE__));
        }
        break;
    
    case REC_SVC_EVN_REQ_CLOSE_SESS:
        i4_ret = _rec_svc_state_sess_started_on_stop_sess (pt_svctx, NOT_NFY_ERR);
        if (i4_ret == SVCTXR_OK)
        {
            i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to stop session (%d). (L%d)\n", i4_ret, __LINE__));
            
            /* return fail and go to ERROR state */
        }
        break;
    
    case REC_SVC_EVN_REQ_SET_ES_LIST:
        i4_ret = _rec_svc_state_util_on_req_set_es_list (pt_svctx,
                                                         pv_data,
                                                         z_len_data);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to set ES List (%d). (L%d)\n", i4_ret, __LINE__));
        }                                                         
        break;

    case REC_SVC_EVN_QUEUED_OPEN_PIPE:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_CLOSE_PIPE:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_OPEN_CONN:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
    
    case REC_SVC_EVN_QUEUED_CLOSE_CONN:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QREQ_REMOVED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_ERROR:
        _rec_svc_state_transit_error (pt_svctx);
        i4_ret = SVCTXR_OK;
        break;
    
    case REC_SVC_EVN_CM_PIPE_OPENED:
        DBG_INFO_EVN_AT_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
    
    case REC_SVC_EVN_CM_PIPE_CLOSED:
        i4_ret = _rec_svc_state_sess_started_on_stop_sess (pt_svctx, NFY_ERR);
        if (i4_ret == SVCTXR_OK)
        {
            i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to stop session (%d). (L%d)\n", i4_ret, __LINE__));
            
            /* return fail and go to ERROR state */
        }
        break;
    
    case REC_SVC_EVN_CONN_CONNECTED:
        DBG_INFO_EVN_AT_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
    
    case REC_SVC_EVN_CONN_DISCONNECTED:
        i4_ret = _rec_svc_state_sess_started_on_stop_sess (pt_svctx, NFY_ERR);
        if (i4_ret == SVCTXR_OK)
        {
            i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close session (%d). (L%d)\n", i4_ret, __LINE__));
            
            /* return fail and go to ERROR state */
        }
        break;

    case REC_SVC_EVN_PAT_UPDATED:
        i4_ret = _rec_svc_state_util_on_pat_updated (pt_svctx);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle PAT updated event (%d). (L%d)\n", i4_ret, __LINE__));
        }
        break;

    case REC_SVC_EVN_PAT_UNAVAIL:
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] PAT Unavailable received at state (%d). (L%d)\n", 
                    ((REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req)->e_state,
                    __LINE__));
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        

    case REC_SVC_EVN_SCDB_UPDATED:
        i4_ret = _rec_svc_state_util_sess_on_scdb_updated (pt_svctx, pv_data, z_len_data);
        break;        
        
    case REC_SVC_EVN_REC_UTIL_TRANS:
        i4_ret = _rec_svc_state_sess_started_on_stop_sess (pt_svctx, NFY_ERR);
        if (i4_ret == SVCTXR_OK)
        {
            i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to stop session (%d). (L%d)\n", i4_ret, __LINE__));
            
            /* return fail and go to ERROR state */
        }
        break;

    case REC_SVC_EVN_REC_UTIL_READY:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
    
    case REC_SVC_EVN_SVC_BLOCKED:
        if (pt_running_svc_req->ui4_sess_flag & REC_SVC_SESS_FLAG_PVR_SVC)
        {
            i4_ret = _rec_svc_state_sess_started_on_stop_sess (pt_svctx, NOT_NFY_ERR);
            if (i4_ret == SVCTXR_OK)
            {
                i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
            }
            else
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to stop session (%d). (L%d)\n", i4_ret, __LINE__));
                
                /* return fail and go to ERROR state */
            }
        }
        else
        {
            i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        }
        break;

    case REC_SVC_EVN_SESS_OPENED:
        
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        
        /* return the error value to transit to SESS_ERROR state */
        i4_ret = SVCTXR_FAIL;
        break;

    case REC_SVC_EVN_SESS_STARTED:
        
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        
        /* return the error value to transit to SESS_ERROR state */
        i4_ret = SVCTXR_FAIL;
        break;

    case REC_SVC_EVN_SESS_STOPPED:
        i4_ret = _rec_svc_state_sess_started_on_stop_sess (pt_svctx, NFY_ERR);
        if (RET_OK (i4_ret))
        {
            i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle EVN_SESS_STOPPED in SESS_STARTED (%d). (L%d)\n", i4_ret, __LINE__));
            
            /* return fail and go to ERROR state */
        }
        break;

    case REC_SVC_EVN_SESS_CLOSED:
        
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);

        /* return the error value to transit to SESS_ERROR state */
        i4_ret = SVCTXR_FAIL;
        break;

    case REC_SVC_EVN_SESS_ERROR:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Received RCRD_SESS_ERROR at state (%d). (L%d)\n", 
                    ((REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req)->e_state,
                    __LINE__));

        /* transit to REC_SVC_STATE_SESS_ERROR state */
        i4_ret = _rec_svc_state_transit_sess_error (pt_svctx);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit SESS_ERROR (%d). (L%d)\n", i4_ret, __LINE__));
            
            /* transit to REC_SVC_STATE_ERROR state */
        }
        break;
            
    case REC_SVC_EVN_STRM_OPENED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        

    case REC_SVC_EVN_STRM_CLOSED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        

    case REC_SVC_EVN_STRM_ERROR:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        

    default:
        DBG_ABORT (DBG_MOD_SVCTX);
    }
    
    return i4_ret;
}    

static INT32 _rec_svc_state_sess_stopping_evn_hdlr (
    SVCTX_T*                    pt_svctx,
    REC_SVC_EVN_TYPE_T          e_event,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data)
{
    INT32           i4_ret = SVCTXR_REC_INTERNAL_ERROR;
    
    switch (e_event)
    {
    case REC_SVC_EVN_REQ_INIT_SVC:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REQ_DEINIT_SVC:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_CONNECT:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_DISCONNECT:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_OPEN_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_START_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_STOP_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_CLOSE_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_SET_ES_LIST:
        i4_ret = _rec_svc_state_util_on_req_set_es_list (pt_svctx,
                                                         pv_data,
                                                         z_len_data);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to set ES List (%d). (L%d)\n", i4_ret, __LINE__));
        }                                                         
        break;

    case REC_SVC_EVN_QUEUED_OPEN_PIPE:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_CLOSE_PIPE:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_OPEN_CONN:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_CLOSE_CONN:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QREQ_REMOVED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_ERROR:
        _rec_svc_state_transit_error (pt_svctx);
        i4_ret = SVCTXR_OK;
        break;
        
    case REC_SVC_EVN_CM_PIPE_OPENED:
        DBG_INFO_EVN_AT_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_CM_PIPE_CLOSED:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_CONN_CONNECTED:
        DBG_INFO_EVN_AT_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_CONN_DISCONNECTED:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_PAT_UPDATED:
        i4_ret = _rec_svc_state_util_on_pat_updated (pt_svctx);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle PAT updated event (%d). (L%d)\n", i4_ret, __LINE__));
        }
        break;
        
    case REC_SVC_EVN_PAT_UNAVAIL:
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] PAT Unavailable received at state (%d). (L%d)\n", 
                    ((REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req)->e_state,
                    __LINE__));
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
        
    case REC_SVC_EVN_SCDB_UPDATED:
        i4_ret = _rec_svc_state_util_sess_on_scdb_updated (pt_svctx, pv_data, z_len_data);
        break;        

    case REC_SVC_EVN_REC_UTIL_TRANS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REC_UTIL_READY:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SVC_BLOCKED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SESS_OPENED:
        
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        
        /* return the error value to transit to SESS_ERROR state */
        i4_ret = SVCTXR_FAIL;
        break;
        
    case REC_SVC_EVN_SESS_STARTED:
        
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        
        /* return the error value to transit to SESS_ERROR state */
        i4_ret = SVCTXR_FAIL;
        break;
        
    case REC_SVC_EVN_SESS_STOPPED:
        i4_ret = _rec_svc_state_sess_stopping_on_sess_stopped (pt_svctx);
        break;
        
    case REC_SVC_EVN_SESS_CLOSED:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
    
    case REC_SVC_EVN_SESS_ERROR:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Received RCRD_SESS_ERROR at state (%d). (L%d)\n", 
                    ((REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req)->e_state,
                    __LINE__));

        /* transit to REC_SVC_STATE_SESS_ERROR state */
        i4_ret = _rec_svc_state_transit_sess_error (pt_svctx);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit SESS_ERROR (%d). (L%d)\n", i4_ret, __LINE__));
            
            /* transit to REC_SVC_STATE_ERROR state */
        }
        break;
    
    case REC_SVC_EVN_STRM_OPENED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
    
    case REC_SVC_EVN_STRM_CLOSED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
    
    case REC_SVC_EVN_STRM_ERROR:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
    
    default:
        DBG_ABORT (DBG_MOD_SVCTX);
    }
    
    return i4_ret;
}

static INT32 _rec_svc_state_sess_closing_evn_hdlr (
    SVCTX_T*                    pt_svctx,
    REC_SVC_EVN_TYPE_T          e_event,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data)
{
    INT32           i4_ret = SVCTXR_REC_INTERNAL_ERROR;
    
    switch (e_event)
    {
    case REC_SVC_EVN_REQ_INIT_SVC:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REQ_DEINIT_SVC:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_CONNECT:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_DISCONNECT:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_OPEN_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_START_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;

    case REC_SVC_EVN_REQ_STOP_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_CLOSE_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;

    case REC_SVC_EVN_REQ_SET_ES_LIST:
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Set ES List when SESS_CLOSING. (L%d)\n", 
                    __LINE__));
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;        

    case REC_SVC_EVN_QUEUED_OPEN_PIPE:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_CLOSE_PIPE:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_OPEN_CONN:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_QUEUED_CLOSE_CONN:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QREQ_REMOVED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_ERROR:
        _rec_svc_state_transit_error (pt_svctx);
        i4_ret = SVCTXR_OK;
        break;
        
    case REC_SVC_EVN_CM_PIPE_OPENED:
        DBG_INFO_EVN_AT_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_CM_PIPE_CLOSED:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_CONN_CONNECTED:
        DBG_INFO_EVN_AT_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_CONN_DISCONNECTED:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_PAT_UPDATED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
        
    case REC_SVC_EVN_PAT_UNAVAIL:
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] PAT Unavailable received at state (%d). (L%d)\n", 
                    ((REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req)->e_state,
                    __LINE__));
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
        
    case REC_SVC_EVN_SCDB_UPDATED:
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] SCDB Updated received at state (%d). (L%d)\n",
                    ((REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req)->e_state,
                    __LINE__));
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;        
        
    case REC_SVC_EVN_REC_UTIL_TRANS:
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] REC_UTIL Transition received at state (%d). (L%d)\n", 
                    ((REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req)->e_state,
                    __LINE__));
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;
        
    case REC_SVC_EVN_REC_UTIL_READY:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SVC_BLOCKED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SESS_OPENED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN; 
        break;
        
    case REC_SVC_EVN_SESS_STARTED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SESS_STOPPED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SESS_CLOSED:
        i4_ret = _rec_svc_state_sess_closing_on_sess_closed (pt_svctx);
        /* return fail and go to ERROR state */
        break;
        
    case REC_SVC_EVN_SESS_ERROR:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] RCRD_SESS_ERROR received at REC_SVC_STATE_SESS_CLOSING state. (L%d)\n", 
                    __LINE__));
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_STRM_OPENED:
        /* ignore this late comming event */
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
        
    case REC_SVC_EVN_STRM_CLOSED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
        
    case REC_SVC_EVN_STRM_ERROR:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
        
    default:
        DBG_ABORT (DBG_MOD_SVCTX);
    }
    
    return i4_ret;
}

static INT32 _rec_svc_state_sess_error_evn_hdlr (
    SVCTX_T*                    pt_svctx,
    REC_SVC_EVN_TYPE_T          e_event,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data)
{
    INT32           i4_ret = SVCTXR_REC_INTERNAL_ERROR;
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Event (%d) received at SESS_ERR state. (L%d)\n", 
                e_event,
                __LINE__));
    
    switch (e_event)
    {
    case REC_SVC_EVN_REQ_INIT_SVC:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Req init svc when SESS_ERR. (L%d)\n", 
                    __LINE__));
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REQ_DEINIT_SVC:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_CONNECT:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_DISCONNECT:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_OPEN_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_START_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_STOP_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_CLOSE_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_SET_ES_LIST:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Set ES List when SESS_ERROR. (L%d)\n", 
                    __LINE__));
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;        

    case REC_SVC_EVN_QUEUED_OPEN_PIPE:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_CLOSE_PIPE:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_OPEN_CONN:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_QUEUED_CLOSE_CONN:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QREQ_REMOVED:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_ERROR:
        _rec_svc_state_transit_error (pt_svctx);
        i4_ret = SVCTXR_OK;
        break;
        
    case REC_SVC_EVN_CM_PIPE_OPENED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_CM_PIPE_CLOSED:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_CONN_CONNECTED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_CONN_DISCONNECTED:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_PAT_UPDATED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
        
    case REC_SVC_EVN_PAT_UNAVAIL:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
        
    case REC_SVC_EVN_SCDB_UPDATED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        

    case REC_SVC_EVN_REC_UTIL_TRANS:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;
        
    case REC_SVC_EVN_REC_UTIL_READY:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SVC_BLOCKED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SESS_OPENED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SESS_STARTED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SESS_STOPPED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SESS_CLOSED:
        i4_ret = _rec_svc_state_sess_error_on_sess_closed (pt_svctx);
        break;

    case REC_SVC_EVN_SESS_ERROR:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] RCRD_SESS_ERROR received at REC_SVC_STATE_SESS_ERROR state. (L%d)\n", 
                    __LINE__));
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_STRM_OPENED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        

    case REC_SVC_EVN_STRM_CLOSED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        

    case REC_SVC_EVN_STRM_ERROR:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
        
    default:
        DBG_ABORT (DBG_MOD_SVCTX);
    }
    
    return i4_ret;
}    

static INT32 _rec_svc_state_error_evn_hdlr (
    SVCTX_T*                    pt_svctx,
    REC_SVC_EVN_TYPE_T          e_event,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data)
{
    INT32           i4_ret = SVCTXR_REC_INTERNAL_ERROR;

    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Event (%d) received at ERROR state. (L%d)\n", 
                e_event,
                __LINE__));

    switch (e_event)
    {
    case REC_SVC_EVN_REQ_INIT_SVC:
        DBG_ERROR_EVN_AT_INV_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_REQ_DEINIT_SVC:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_CONNECT:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_DISCONNECT:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_OPEN_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_START_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_STOP_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_CLOSE_SESS:
        i4_ret = SVCTXR_REC_MSG_QUEUE_EVN;
        break;
        
    case REC_SVC_EVN_REQ_SET_ES_LIST:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Set ES List in ERROR state. (L%d)\n", 
                    __LINE__));
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;        

    case REC_SVC_EVN_QUEUED_OPEN_PIPE:
        DBG_INFO_EVN_AT_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_QUEUED_CLOSE_PIPE:
        i4_ret = _rec_svc_state_util_close_cm_pipe (pt_svctx);
        break;

    case REC_SVC_EVN_QUEUED_OPEN_CONN:
        DBG_INFO_EVN_AT_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_QUEUED_CLOSE_CONN:
        i4_ret = _rec_svc_state_util_close_conn (pt_svctx, FALSE, ALSO_PIPE, NULL);
        break;

    case REC_SVC_EVN_QREQ_REMOVED:
        i4_ret = _rec_svc_state_util_on_qreq_removed (pt_svctx);
        break;

    case REC_SVC_EVN_ERROR:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Received EVN_ERROR in ERROR state. (L%d)\n", 
                    __LINE__));
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_CM_PIPE_OPENED:
        DBG_INFO_EVN_AT_ST (pt_svctx, e_event);
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_CM_PIPE_CLOSED:
        i4_ret = _rec_svc_state_error_on_res_closed (pt_svctx);
        break;
        
    case REC_SVC_EVN_CONN_CONNECTED:
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Event (%d) received at ERROR state. (L%d)\n", 
                    e_event,
                    __LINE__));
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_CONN_DISCONNECTED:
        i4_ret = _rec_svc_state_error_on_res_closed (pt_svctx);
        break;
        
    case REC_SVC_EVN_PAT_UPDATED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        
        
    case REC_SVC_EVN_PAT_UNAVAIL:
        i4_ret = _rec_svc_state_error_on_res_closed (pt_svctx);
        break;        
        
    case REC_SVC_EVN_SCDB_UPDATED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        

    case REC_SVC_EVN_REC_UTIL_TRANS:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;
        
    case REC_SVC_EVN_REC_UTIL_READY:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;
        
    case REC_SVC_EVN_SVC_BLOCKED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SESS_OPENED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        

    case REC_SVC_EVN_SESS_STARTED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        

    case REC_SVC_EVN_SESS_STOPPED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;
        break;

    case REC_SVC_EVN_SESS_CLOSED:
        i4_ret = _rec_svc_state_error_on_res_closed (pt_svctx);
        break;        

    case REC_SVC_EVN_SESS_ERROR:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        

    case REC_SVC_EVN_STRM_OPENED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        

    case REC_SVC_EVN_STRM_CLOSED:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        

    case REC_SVC_EVN_STRM_ERROR:
        i4_ret = SVCTXR_REC_MSG_IGNORE_EVN;     
        break;        

    case REC_SVC_EVN_REUSE_PIPE_TO_NOT_REUSED:
        i4_ret = _rec_svc_state_transit_force_conned (pt_svctx);
        break;

    default:
        DBG_ABORT (DBG_MOD_SVCTX);
    }
    
    return i4_ret;
}    

/*-----------------------------------------------------------------------------
                    common event entry function
 ----------------------------------------------------------------------------*/

static VOID _rec_svc_state_post_proc_finished_event (
    SVCTX_T*                    pt_svctx,
    REC_SVC_EVN_TYPE_T          e_event,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data)
{
    REC_SVC_REQ_T*      pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    
    switch (e_event)
    {
    case REC_SVC_EVN_REQ_OPEN_SESS:
    {
        SVCTX_REC_REQ_START_REC_MSG_T*  pt_msg_info = (SVCTX_REC_REQ_START_REC_MSG_T *) pv_data;
        
        /* free the pending record config data */
        _rec_svc_util_mem_free (pt_running_svc_req,
                                (VOID *) pt_msg_info->pt_rec_cfg);
                
        break;
    }
    
    case REC_SVC_EVN_REQ_SET_ES_LIST:
    {
        SVCTX_REC_REQ_SET_ES_LIST_MSG_T* pt_msg_info = (SVCTX_REC_REQ_SET_ES_LIST_MSG_T *) pv_data;
     
        /* free the pending es list info data */
        _rec_svc_util_mem_free (pt_running_svc_req,
                                (VOID *) pt_msg_info->t_es_list.at_es_info );
                                
        pt_msg_info->t_es_list.at_es_info = NULL;
        pt_msg_info->t_es_list.ui2_es_num = 0;
        break;
    }
        
    default:
        break;
    }
}

static INT32 _rec_svc_state_append_event (
    REC_SVC_REQ_T*              pt_svc_req,
    REC_SVC_EVN_TYPE_T          e_event,
    const VOID*                 pv_data,
    SIZE_T                      z_len_data,
    REC_SVC_EVN_INFO_T**        ppt_evn_new)
{
    REC_SVC_EVN_INFO_T*     pt_evn_new;
    
    if (ppt_evn_new)
    {
        *ppt_evn_new = NULL;
    }
    
    if (z_len_data > REC_SVC_MAX_EVN_DATA_SIZE)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Data size too large (%d). (L%d)\n", z_len_data, __LINE__));
        return SVCTXR_INV_ARG;
    }
    
    if (pt_svc_req->ui2_num_q_evn >= REC_SVC_EVN_Q_MAX_NUM)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to append event (%d) since too many events in queue. (L%d)\n", 
                    e_event, 
                    __LINE__));
        return SVCTXR_NO_RESOURCE;
    }
    
    if (DLIST_IS_EMPTY (& pt_svc_req->t_free_evns))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal error. (L%d)\n", __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    pt_evn_new = DLIST_HEAD(& pt_svc_req->t_free_evns);
    
    DLIST_REMOVE (pt_evn_new, 
                  & pt_svc_req->t_free_evns, 
                  t_dlink);
    
    pt_evn_new->e_event    = e_event;
    pt_evn_new->z_len_data = z_len_data;   
    x_memcpy ((VOID *) pt_evn_new->aui1_data, pv_data, z_len_data);
    pt_evn_new->ui4_sn     = pt_svc_req->ui4_sn_evn ++;
    
    DLIST_INSERT_TAIL (pt_evn_new, 
                       & pt_svc_req->t_evn_q, 
                       t_dlink);
    
    pt_svc_req->ui2_num_q_evn ++;
    
    if (ppt_evn_new)
    {
        *ppt_evn_new = pt_evn_new;
    }
    
    return SVCTXR_OK;
}

static INT32 _rec_svc_state_remove_event (
    REC_SVC_REQ_T*              pt_svc_req,
    REC_SVC_EVN_INFO_T*         pt_evn_rm)
{
    if (pt_svc_req->ui2_num_q_evn == 0)
    {
        DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] No event to remove. (L%d)\n", __LINE__));
        return SVCTXR_FAIL;
    }
    
    if (DLIST_IS_EMPTY (& pt_svc_req->t_evn_q))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal error. (L%d)\n", __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    DLIST_REMOVE (pt_evn_rm, 
                  & pt_svc_req->t_evn_q, 
                  t_dlink);

    DLIST_INSERT_TAIL (pt_evn_rm, 
                       & pt_svc_req->t_free_evns, 
                       t_dlink);
    
    pt_svc_req->ui2_num_q_evn --;
    
    if (DLIST_IS_EMPTY (& pt_svc_req->t_evn_q))
    {
        /* reset the event serial number */
        pt_svc_req->ui4_sn_evn = 0;
    }
    
    return SVCTXR_OK;
}    

static VOID _rec_svc_state_clear_event_q (
    REC_SVC_REQ_T*              pt_svc_req)
{
    REC_SVC_EVN_INFO_T*     pt_evn;
    
    pt_evn = DLIST_HEAD (& pt_svc_req->t_evn_q);
    
    while (pt_evn)
    {
        DLIST_REMOVE (pt_evn, 
                      & pt_svc_req->t_evn_q, 
                      t_dlink);
    
        DLIST_INSERT_TAIL (pt_evn, 
                           & pt_svc_req->t_free_evns, 
                           t_dlink);
        
        pt_evn = DLIST_HEAD (& pt_svc_req->t_evn_q);
    }
    
    pt_svc_req->ui2_num_q_evn = 0;

    pt_svc_req->ui4_sn_evn = 0;
}    

static INT32 _rec_svc_state_proc_events (
    SVCTX_T*                    pt_svctx,
    REC_SVC_REQ_T*              pt_running_svc_req,
    REC_SVC_EVN_INFO_T*         pt_evn_from)
{
    REC_SVC_EVN_INFO_T*  pt_evn;
    REC_SVC_EVN_INFO_T*  pt_evn_next;
    REC_SVC_STATE_T      e_state_prev;
    BOOL                 b_state_chged;
    INT32                i4_ret = SVCTXR_OK;
    
    b_state_chged = FALSE;
    pt_evn = pt_evn_from;
    
    while (pt_evn)
    {
        pt_evn_next = DLIST_NEXT (pt_evn, t_dlink);
        
        e_state_prev = pt_running_svc_req->e_state;
        
        /* handle the event */
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] << BEGIN - handle event [%d] at state [%d]. (L%d)\n", 
                   pt_evn->e_event,
                   pt_running_svc_req->e_state, 
                   __LINE__));
        
        switch (pt_running_svc_req->e_state)
        {
        case REC_SVC_STATE_NOT_INIT:
            i4_ret = _rec_svc_state_not_init_evn_hdlr (pt_svctx, pt_evn->e_event, (VOID *) pt_evn->aui1_data, pt_evn->z_len_data);
            break;
    
        case REC_SVC_STATE_DISCONNECTED:
            i4_ret = _rec_svc_state_disconned_evn_hdlr (pt_svctx, pt_evn->e_event, (VOID *) pt_evn->aui1_data, pt_evn->z_len_data);
            break;
        
        case REC_SVC_STATE_CONNECTING:
            i4_ret = _rec_svc_state_conning_evn_hdlr (pt_svctx, pt_evn->e_event, (VOID *) pt_evn->aui1_data, pt_evn->z_len_data);
            break;
    
        case REC_SVC_STATE_CONNECTED:
            i4_ret = _rec_svc_state_conned_evn_hdlr (pt_svctx, pt_evn->e_event, (VOID *) pt_evn->aui1_data, pt_evn->z_len_data);
            break;
    
        case REC_SVC_STATE_DISCONNECTING:
            i4_ret = _rec_svc_state_disconning_evn_hdlr (pt_svctx, pt_evn->e_event, (VOID *) pt_evn->aui1_data, pt_evn->z_len_data);
            break;
            
        case REC_SVC_STATE_WAIT_RECONN:
            i4_ret = _rec_svc_state_wait_reconn_evn_hdlr (pt_svctx, pt_evn->e_event, (VOID *) pt_evn->aui1_data, pt_evn->z_len_data);
            break;
            
        case REC_SVC_STATE_SESS_OPENING:
            i4_ret = _rec_svc_state_sess_opening_evn_hdlr (pt_svctx, pt_evn->e_event, (VOID *) pt_evn->aui1_data, pt_evn->z_len_data);
            break;
            
        case REC_SVC_STATE_SESS_OPENED:
            i4_ret = _rec_svc_state_sess_opened_evn_hdlr (pt_svctx, pt_evn->e_event, (VOID *) pt_evn->aui1_data, pt_evn->z_len_data);
            break;

        case REC_SVC_STATE_SESS_STARTING:
            i4_ret = _rec_svc_state_sess_starting_evn_hdlr (pt_svctx, pt_evn->e_event, (VOID *) pt_evn->aui1_data, pt_evn->z_len_data);
            break;
            
        case REC_SVC_STATE_SESS_STARTED:
            i4_ret = _rec_svc_state_sess_started_evn_hdlr (pt_svctx, pt_evn->e_event, (VOID *) pt_evn->aui1_data, pt_evn->z_len_data);
            break;
            
        case REC_SVC_STATE_SESS_STOPPING:
            i4_ret = _rec_svc_state_sess_stopping_evn_hdlr (pt_svctx, pt_evn->e_event, (VOID *) pt_evn->aui1_data, pt_evn->z_len_data);
            break;
        
        case REC_SVC_STATE_SESS_CLOSING:
            i4_ret = _rec_svc_state_sess_closing_evn_hdlr (pt_svctx, pt_evn->e_event, (VOID *) pt_evn->aui1_data, pt_evn->z_len_data);
            break;
            
        case REC_SVC_STATE_SESS_ERROR:
            i4_ret = _rec_svc_state_sess_error_evn_hdlr (pt_svctx, pt_evn->e_event, (VOID *) pt_evn->aui1_data, pt_evn->z_len_data);
            break;
        
        case REC_SVC_STATE_ERROR:
            i4_ret = _rec_svc_state_error_evn_hdlr (pt_svctx, pt_evn->e_event, (VOID *) pt_evn->aui1_data, pt_evn->z_len_data);
            break;
            
        default:
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Un-handled state type (%d). (L%d)\n", pt_running_svc_req->e_state, __LINE__));
            
            i4_ret = SVCTXR_REC_INTERNAL_ERROR;
        }

        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] >> END - handled event [%d] => current-state [%d], ret=%d (%s). (L%d)\n", 
                   pt_evn->e_event,
                   pt_running_svc_req->e_state, 
                   i4_ret,
                   ( i4_ret == SVCTXR_REC_MSG_QUEUE_EVN ? "QUEUE" : 
                     ( i4_ret == SVCTXR_REC_MSG_IGNORE_EVN ? "IGNORE" : 
                       ( RET_FAIL (i4_ret) ? "ERR" : 
                         ( i4_ret == SVCTXR_OK ? "OK" : "other") ) ) ),
                   __LINE__));
                
        /* remove event if it's not to keep in queue */
        if (i4_ret != SVCTXR_REC_MSG_QUEUE_EVN)
        {
            INT32   i4_ret_tmp;
            
            /* post process finished event */
            _rec_svc_state_post_proc_finished_event (pt_svctx, 
                                                     pt_evn->e_event, 
                                                     (VOID *) pt_evn->aui1_data,
                                                     pt_evn->z_len_data);
            
            /* remove the event */
            i4_ret_tmp = _rec_svc_state_remove_event (pt_running_svc_req,
                                                      pt_evn);
            if (RET_FAIL (i4_ret_tmp))
            {
                DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] unsuccessfull to remove event (%d). (L%d)\n", i4_ret_tmp, __LINE__));
            }
        }
        
        /* handle error cases */
        if (RET_FAIL (i4_ret))
        {
            switch (pt_running_svc_req->e_state)
            {
            case REC_SVC_STATE_NOT_INIT:
            case REC_SVC_STATE_DISCONNECTED:
            case REC_SVC_STATE_ERROR:
            {
                /* ignore this error */
                break;
            }
               
            case REC_SVC_STATE_CONNECTING:
            case REC_SVC_STATE_CONNECTED:
            case REC_SVC_STATE_DISCONNECTING:
            case REC_SVC_STATE_WAIT_RECONN:
            case REC_SVC_STATE_SESS_ERROR:
            {
                /* transit to REC_SVC_STATE_ERROR state */
                if (REC_SVC_STATE_CAN_TRANSIT_ERROR (pt_running_svc_req->e_state))
                {
                    _rec_svc_state_transit_error (pt_svctx);
                }
                break;
            }
                
            case REC_SVC_STATE_SESS_OPENING:
            case REC_SVC_STATE_SESS_OPENED:
            case REC_SVC_STATE_SESS_STARTING:
            case REC_SVC_STATE_SESS_STARTED:
            case REC_SVC_STATE_SESS_STOPPING:
            case REC_SVC_STATE_SESS_CLOSING:
            {
                /* transit to REC_SVC_STATE_SESS_ERROR state */
                i4_ret = _rec_svc_state_transit_sess_error (pt_svctx);

                if (RET_FAIL (i4_ret))
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit SESS_ERROR (%d). (L%d)\n", i4_ret, __LINE__));
                    
                    /* transit to ERROR state */
                    _rec_svc_state_transit_error (pt_svctx);
                }
                break;
            }
                
            default:
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Un-handled state type (%d). (L%d)\n", pt_running_svc_req->e_state, __LINE__));
                i4_ret = SVCTXR_REC_INTERNAL_ERROR;
            }
        }        
        /* check if service already deinited */
        else if (i4_ret == SVCTXR_REC_MSG_SVC_DEINITED)
        {
            /* the event queue must be already empty */
            if (!DLIST_IS_EMPTY (&pt_running_svc_req->t_evn_q))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Event queue is not empty after service is deinited. (L%d)\n", __LINE__));
            }
            
            break;
        }
        
        /* check if the event causes state change */
        if (pt_running_svc_req->e_state != e_state_prev)
        {
            b_state_chged = TRUE;
        }
        
        if (!pt_evn_next &&
             b_state_chged)
        {
            /* traverse each event from head for the new state */
            pt_evn_next = DLIST_HEAD (&pt_running_svc_req->t_evn_q);
            b_state_chged = FALSE;
        }
        
        pt_evn = pt_evn_next;
    }         

    DBG_INFO_DUMP_Q_EVENTS (pt_running_svc_req);
    
    return i4_ret;
}

static INT32 _rec_svc_state_handle_event (
    SVCTX_T*                    pt_svctx,
    REC_SVC_EVN_TYPE_T          e_event_new,
    const VOID*                 pv_data_new,
    SIZE_T                      z_len_data_new)
{
    REC_SVC_REQ_T*          pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_EVN_INFO_T*     pt_evn_new;
    INT32                   i4_ret;

    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Handle new event [%d]. (L%d)\n", 
               e_event_new,
               __LINE__));
    
    /* queue the new event */
    i4_ret = _rec_svc_state_append_event (pt_running_svc_req,
                                          e_event_new,
                                          pv_data_new,
                                          z_len_data_new,
                                          & pt_evn_new );
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to append event (%d). (L%d)\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    i4_ret = _rec_svc_state_proc_events (pt_svctx,
                                         pt_running_svc_req,
                                         pt_evn_new );
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to proc events (%d). (L%d)\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
                    action message queue functions
 ----------------------------------------------------------------------------*/
static VOID _rec_svc_state_act_msg_q_init (
    REC_SVC_REQ_T*              pt_svc_req)
{
    x_memset (pt_svc_req->aui1_act_msg_q, 0, REC_SVC_ACT_MSG_Q_MAX_NUM);
    pt_svc_req->ui2_num_act_msg = 0;
}
        
static INT32 _rec_svc_state_act_msg_q_append (
    REC_SVC_REQ_T*              pt_svc_req,
    UINT8                       ui1_act_msg)
{
    UINT16  ui2_i;
    BOOL    b_exist;
    
    /* do nothing if the message already in queue */
    b_exist = FALSE;
    for (ui2_i = 0; ui2_i < pt_svc_req->ui2_num_act_msg; ui2_i++)
    {
        if (pt_svc_req->aui1_act_msg_q[ui2_i] == ui1_act_msg)
        {
            b_exist = TRUE;
            break;
        }
    }

    if (!b_exist)    
    {
        if (pt_svc_req->ui2_num_act_msg == REC_SVC_ACT_MSG_Q_MAX_NUM)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
            return SVCTXR_REC_INTERNAL_ERROR;
        }
        
        pt_svc_req->aui1_act_msg_q[pt_svc_req->ui2_num_act_msg] = ui1_act_msg;
        pt_svc_req->ui2_num_act_msg++;
    }
    
    return SVCTXR_OK;
}

static BOOL _rec_svc_state_act_msg_q_check_empty (
    REC_SVC_REQ_T*              pt_svc_req)
{
    return pt_svc_req->ui2_num_act_msg == 0 ? TRUE : FALSE;
}

static INT32 _rec_svc_state_act_msg_q_peek (
    REC_SVC_REQ_T*              pt_svc_req,
    UINT8*                      pui1_act_msg)
{
    if (pt_svc_req->ui2_num_act_msg == 0)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid operation. (L%d)\n", __LINE__));
        return SVCTXR_INV_COMMAND;
    }
    
    *pui1_act_msg = pt_svc_req->aui1_act_msg_q[0];
    
    return SVCTXR_OK;
}
            
static INT32 _rec_svc_state_act_msg_q_remove (
    REC_SVC_REQ_T*              pt_svc_req,
    UINT8*                      pui1_act_msg)
{
    UINT16  ui2_i;
    
    if (pt_svc_req->ui2_num_act_msg == 0)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid operation. (L%d)\n", __LINE__));
        return SVCTXR_INV_COMMAND;
    }
    
    *pui1_act_msg = pt_svc_req->aui1_act_msg_q[0];
    pt_svc_req->ui2_num_act_msg -- ;
    
    for (ui2_i = 0; ui2_i < pt_svc_req->ui2_num_act_msg; ui2_i++)
    {
        pt_svc_req->aui1_act_msg_q[ui2_i] = pt_svc_req->aui1_act_msg_q[ui2_i+1];
    }
    
    pt_svc_req->aui1_act_msg_q[pt_svc_req->ui2_num_act_msg] = 0;
    
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
                    message handling functions
 ----------------------------------------------------------------------------*/
static INT32 _rec_svc_msg_hdlr_crnt_evn_nfy_ready (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
#ifndef MW_EDB_SUPPORT   
	EVCTX_COND_T				e_cond_evn  = crnt_evn_get_cond (pt_running_svc_req->pt_crnt_evn);
#endif
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl;
    BOOL                        b_blocked;
    INT32                       i4_ret;
    
    i4_ret = SVCTXR_OK;
    
    i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req, 
                                               & pt_res_brdcst_svl );
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get RES_BRDCST_SVL (%d). (L%d)\n", i4_ret, __LINE__));
        pt_res_brdcst_svl = NULL;
    }

    if(pt_running_svc_req->pt_rec_cfg != NULL)
    {
        if (pt_running_svc_req->e_cond_rec_util == REC_UTIL_COND_READY  &&
            pt_running_svc_req->e_cond_sess == SM_SESS_COND_STARTED     &&
            REC_SVC_PROFILE_SUPPORT_EVENT_INFO (pt_running_svc_req)     
#ifndef MW_EDB_SUPPORT
            && REC_SVC_EVCTX_INFO_AVAIL (e_cond_evn)
#endif
			)
        {
            i4_ret = _rec_svc_intr_update_event_info (pt_svctx);
            
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to update event info. (L%d)\n", __LINE__));
                i4_ret = SVCTXR_OK;
            }
        }
    }    
    else
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] pt_running_svc_req->pt_rec_cfg is NULL. (L%d)\n", __LINE__));
    }  
    
    /* check if service blocked */
    i4_ret = _rec_svc_state_util_check_block (pt_svctx,
                                              & b_blocked);
    if (RET_OK (i4_ret))
    {
        if (b_blocked)
        {
            if (!pt_running_svc_req->b_svc_blked)
            {
                pt_running_svc_req->b_svc_blked = TRUE;
                
                if (REC_SVC_STATE_IN_ANY_SESS_STATES (pt_running_svc_req->e_state) && 
                    (pt_running_svc_req->ui4_sess_flag & REC_SVC_SESS_FLAG_PVR_SVC))
                {
                    if (pt_running_svc_req->pf_rec_nfy)
                    {
                        if(pt_running_svc_req->pt_rec_cfg != NULL)
                        {
                            pt_running_svc_req->pf_rec_nfy (pt_running_svc_req->pt_rec_cfg->h_svctx_nfy,
                                                        pt_running_svc_req->pv_rec_nfy_tag,
                                                        SVCTX_NTFY_CODE_REC_BLOCKED,
                                                        0 );
                        }
                    }
                    else
                    {
                        DBG_ABORT (DBG_MOD_SVCTX);
                    }  
                
                    rec_hdlr_invoke_client_svc_nfy_fct (pt_svctx,
                                                        SVCTX_NTFY_CODE_REC_BLOCKED,
                                                        ST_UNKNOWN );
                }
                
                i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                                      REC_SVC_EVN_SVC_BLOCKED, 
                                                      NULL, 
                                                      0);
                if (RET_FAIL (i4_ret))
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
                }
            }
        }
        else
        {
            if (pt_running_svc_req->b_svc_blked)
            {
                pt_running_svc_req->b_svc_blked = FALSE;
            }
        }
    }
    else
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to check service block (%d). (L%d)\n", i4_ret, __LINE__));
        i4_ret = SVCTXR_OK;
    }
    
    /* check rc descriptor in ATSC broadcast type */
    if (pt_res_brdcst_svl &&
        pt_res_brdcst_svl->e_brdcst_type == BRDCST_TYPE_ATSC && 
        crnt_evn_is_ready (pt_running_svc_req->pt_crnt_evn))
    {
#ifndef MW_EDB_SUPPORT
        HANDLE_T                 h_event = crnt_evn_get_handle (pt_running_svc_req->pt_crnt_evn);
        EVCTX_ATSC_KEY_INFO_T    t_atsc_info;
        SIZE_T                   z_rc_info_len;        
        EVCTX_COND_T             e_cond_evctx;
        SVCTX_REC_AUTH_STATUS_T  e_auth_st_new;
        
        t_atsc_info.e_atsc_key_type  = EVCTX_ATSC_KEY_TYPE_RC_INFO;
        t_atsc_info.pv_atsc_key_info = NULL;
		
        i4_ret = x_evctx_get_event_info_len (h_event,
                                             EVCTX_KEY_TYPE_ENG_SPECIFIC,
                                             (VOID *) & t_atsc_info,
                                             & z_rc_info_len,
                                             & e_cond_evctx );
        if (i4_ret == EVCTXR_OK)
        {
            if (z_rc_info_len == 0)
            {
                e_auth_st_new = SVCTX_REC_AUTH_STATUS_AUTHORIZED;
            }
            else
            {
                e_auth_st_new = SVCTX_REC_AUTH_STATUS_UNAUTHORIZED;
            }
            
            if (e_auth_st_new != pt_running_svc_req->e_auth_st)
            {
                if (e_auth_st_new == SVCTX_REC_AUTH_STATUS_AUTHORIZED)
                {
                    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Become AUTHORIZED by ATSC RC Descriptor. (L%d)\n", __LINE__));
                }
                else
                {
                    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Become UNAUTHORIZED by ATSC RC Descriptor. (L%d)\n", __LINE__));
                }
                
                pt_running_svc_req->e_auth_st = e_auth_st_new;
                
                i4_ret = _rec_svc_state_util_ctrl_es_list_output (pt_svctx);
                if (RET_FAIL (i4_ret))
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to control stream list output (%d). (L%d)\n", i4_ret, __LINE__));
                }
            }
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get RC info length (%d). (L%d)\n", i4_ret, __LINE__));
        }
        
        i4_ret = SVCTXR_OK;
#endif
        /* capture first event title for PVR */
    if (pt_running_svc_req->e_media_type == REC_SVC_MEDIA_TYPE_PVR &&
        pt_running_svc_req->e_cond_rec_util == REC_UTIL_COND_READY &&
        REC_SVC_STATE_IN_ANY_SESS_STATES (pt_running_svc_req->e_state))
    {
        i4_ret = _rec_svc_util_pvr_rfi_write_fst_evn_ttl (pt_running_svc_req);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to update fst_evn_ttl (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_OK;
        }
    }

	}
    
    return i4_ret;
}     

static INT32 _rec_svc_msg_hdlr_crnt_evn_nfy_not_ready (
    SVCTX_T*                    pt_svctx)
{
    /* TODO */
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Event notify not ready. (L%d)\n", __LINE__));
    
    return SVCTXR_OK;
}    
 
static INT32 _rec_svc_msg_hdlr_crnt_evn_nfy (
    struct _CRNT_EVENT_T*       pt_this,
    CRNT_EVENT_NTFY_CODE_T      e_code,
    VOID*                       pv_tag,
    UINT32                      ui4_data)
{
    SVCTX_T*        pt_svctx = (SVCTX_T *) pv_tag;
    REC_SVC_REQ_T*  pt_running_svc_req = NULL;
    INT32           i4_ret;
    
    if (e_code != CRNT_EVENT_NTFY_CODE_REQ_SEND_MSG) 
    {
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Crnt event notify (code=%d, h_event=%d, evctx_cond=%d). (L%d)\n", 
                   e_code,
                   crnt_evn_get_handle (pt_this),
                   crnt_evn_get_cond (pt_this),
                   __LINE__));
        
        /* as e_code = CRNT_EVENT_NTFY_CODE_REQ_SEND_MSG, it is still in Evctx's thread context
         * So, do not access the pt_running_svc_req and do the != check 
         * to avoid the race-condition */
        pt_running_svc_req = (REC_SVC_REQ_T*) pt_svctx->pt_running_svc_req;    
        
        if (pt_running_svc_req == NULL  ||
            pt_this != pt_running_svc_req->pt_crnt_evn) 
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
            return SVCTXR_FAIL;
        }
    }
    
    i4_ret = SVCTXR_OK;
    switch (e_code)
    {
    case CRNT_EVENT_NTFY_CODE_START_QUERY:
        /* do nothing */
        break;

    case CRNT_EVENT_NTFY_CODE_EVN_READY:
        i4_ret = _rec_svc_msg_hdlr_crnt_evn_nfy_ready (pt_svctx);
        break;

    case CRNT_EVENT_NTFY_CODE_EVN_NOT_READY:
        i4_ret = _rec_svc_msg_hdlr_crnt_evn_nfy_not_ready (pt_svctx);
        break;

    case CRNT_EVENT_NTFY_CODE_REQ_SEND_MSG:
        i4_ret = svctx_send_msg (pt_svctx->h_sys_svctx, 
                                 (SVCTX_MSG_T*) ui4_data);
        if (i4_ret != SVCTXR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send msg. (L%d)\n", __LINE__));
        }
        break;
    
    default:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Unhandled CRNT_EVENT ntfy code (%d). (L%d)\n", e_code, __LINE__));
        return SVCTXR_NOT_SUPPORT;
    }

    return i4_ret;
}      

static INT32 _rec_svc_msg_hdlr_cm_pipe_nfy (
    SVCTX_T*                    pt_svctx,
    const SVCTX_MSG_T*          pt_msg)
{
    REC_SVC_REQ_T*               pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    SVCTX_REC_CM_PIPE_NFY_MSG_T* pt_msg_cm_pipe = (SVCTX_REC_CM_PIPE_NFY_MSG_T *) pt_msg->u.t_hdlr.aui1_data;
    INT32                        i4_ret = SVCTXR_OK;
    
    DBG_ASSERT (pt_msg_cm_pipe->h_cm_pipe != NULL_HANDLE, 
                DBG_MOD_SVCTX);
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] CM Pipe (%d) notify cond: %d. (L%d)\n", 
               pt_msg_cm_pipe->h_cm_pipe,
               pt_msg_cm_pipe->e_cond, __LINE__));
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Original CM Pipe info: src_op=%d (flag:0x%X), pipe=%d, cond=%d. (L%d)\n", 
               pt_running_svc_req->e_src_op_type,
               pt_running_svc_req->ui4_src_op_flag,
               pt_running_svc_req->h_cm_pipe,
               pt_running_svc_req->e_cond_cm_pipe, __LINE__));
    
    if (pt_running_svc_req->h_cm_pipe == NULL_HANDLE)
    {
        if ((pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_DEINIT_SVC) &&
            (pt_msg_cm_pipe->e_cond == CM_COND_CLOSING ||
             pt_msg_cm_pipe->e_cond == CM_COND_CLOSED) )
        {
            /* Case 1:
               Already received CM_COND_CLOSED, now this pt_msg_cm_pipe message may come 
               from the case that the pipe reusing is cancelled. A real CLOSED message is 
               then received after the fake CLOSED message received previously. 
               Simply ignore this kind of messages. */
            /* Case 2:
               Not yet transit to the state that opens pipe. Now this pt_msg_cm_pipe 
               message may come from the case that the pipe reusing is cancelled when 
               the running SVC_REQ is just selected but than stopped very soon. 
               The CM_PIPE handle kept in REC_SVC_REQ_T.t_conn_info_preset is forced to close. */
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Unexpected CM_PIPE msg (e_src_op=%d, h_cm_pipe=%d, e_cond=%d). (L%d)\n",  
                        pt_running_svc_req->e_src_op_type,
                        pt_msg_cm_pipe->h_cm_pipe,
                        pt_msg_cm_pipe->e_cond, __LINE__));
        }
        
        return SVCTXR_OK;
    }
    else
    {
        if (pt_msg_cm_pipe->h_cm_pipe != pt_running_svc_req->h_cm_pipe)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Unmatched cm pipe handle (curr=%d, new=%d). (L%d)\n", 
                        pt_running_svc_req->h_cm_pipe,
                        pt_msg_cm_pipe->h_cm_pipe,
                        __LINE__));
            
            /* transit to error state */
            i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                                  REC_SVC_EVN_ERROR, 
                                                  NULL, 
                                                  0);
            
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to error state (%d). (L%d)\n", i4_ret, __LINE__));
            }
            
            return i4_ret;
        }
    }
    
    /* clear h_cm_pipe if necessary */
    if (pt_msg_cm_pipe->e_cond == CM_COND_CLOSED)
    {
        if (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_DEINIT_SVC && 
            (pt_running_svc_req->ui4_src_op_flag & REC_SVC_DEINIT_SVC_FLAG_REUSE_PIPE))
        {
            pt_running_svc_req->t_conn_info_reuse.h_cm_pipe = pt_running_svc_req->h_cm_pipe;
            pt_running_svc_req->t_conn_info_reuse.e_cond_cm_pipe = pt_running_svc_req->e_cond_cm_pipe;
        }
        
        pt_running_svc_req->h_cm_pipe = NULL_HANDLE;
    }
    
    /* update the condition */
    if (pt_msg_cm_pipe->e_cond == CM_COND_CLOSED  ||
        pt_msg_cm_pipe->e_cond == CM_COND_OPENING ||
        pt_msg_cm_pipe->e_cond == CM_COND_OPENED  ||
        pt_msg_cm_pipe->e_cond == CM_COND_CLOSING)
    {
        pt_running_svc_req->e_cond_cm_pipe = pt_msg_cm_pipe->e_cond;
    }
    
    /* check queued msg */
    #ifdef MW_TV_AV_BYPASS_SUPPORT
    {
        if (pt_msg_cm_pipe->e_cond == CM_COND_OPENED ||
            pt_msg_cm_pipe->e_cond == CM_COND_CLOSED)
        {
            i4_ret = svctx_mgr_check_queued_req (ACTIVE_LINK_NOW,
                                                 pt_running_svc_req->e_src_rec,
                                                 pt_svctx,
                                                 FALSE);
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to check queued req (%d). (L%d)\n", i4_ret, __LINE__));
            }                                         
        }
    }
    #endif    
    
    switch (pt_msg_cm_pipe->e_cond)
    {
    case CM_COND_OPENING:
        /* do nothing */
        break;
        
    case CM_COND_OPENED:
        i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                              REC_SVC_EVN_CM_PIPE_OPENED, 
                                              (VOID *) pt_msg_cm_pipe, 
                                              sizeof (SVCTX_REC_CM_PIPE_NFY_MSG_T));

        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
        }
        break;

    case CM_COND_CLOSING:
        /* do nothing */
        break;
        
    case CM_COND_CLOSED:
        i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                              REC_SVC_EVN_CM_PIPE_CLOSED, 
                                              (VOID *) pt_msg_cm_pipe, 
                                              sizeof (SVCTX_REC_CM_PIPE_NFY_MSG_T));

        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
        }
        break;
        
    default:
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Un-handled CM Pipe notify (%d). (L%d)\n", pt_msg_cm_pipe->e_cond, __LINE__));
        i4_ret = SVCTXR_OK;
    }
    
    /* clear h_cm_pipe if necessary */
    if ((pt_msg_cm_pipe->e_cond == CM_COND_CLOSED) 
        && (pt_running_svc_req->b_pipe_real_close))
    {
        pt_running_svc_req->b_pipe_real_close = FALSE;
        i4_ret = SVCTXR_REC_MSG_SVC_DEINITED;
    }
    
    return i4_ret;
}

static INT32 _rec_svc_msg_hdlr_conn_nfy (
    SVCTX_T*                    pt_svctx,
    const SVCTX_MSG_T*          pt_msg)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    SVCTX_REC_CONN_NFY_MSG_T*   pt_msg_conn = (SVCTX_REC_CONN_NFY_MSG_T *) pt_msg->u.t_hdlr.aui1_data;
    INT32                       i4_ret = SVCTXR_OK;

    DBG_ASSERT (pt_msg_conn->h_conn != NULL_HANDLE, 
                DBG_MOD_SVCTX);

    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Connection (%d) notify cond: %d. (L%d)\n", 
               pt_msg_conn->h_conn,
               pt_msg_conn->e_cond, 
               __LINE__));

    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Original CONN info: src_op=%d (flag:0x%X), conn=%d, cond=%d. (L%d)\n", 
               pt_running_svc_req->e_src_op_type,
               pt_running_svc_req->ui4_src_op_flag,
               pt_running_svc_req->h_conn,
               pt_running_svc_req->e_cond_conn, __LINE__));

    if (pt_running_svc_req->h_conn == NULL_HANDLE)
    {
        if (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_DEINIT_SVC &&
            (pt_msg_conn->e_cond == CM_COND_CONNECTION_INFO ||
             pt_msg_conn->e_cond == CM_COND_DISCONNECTING   ||
             pt_msg_conn->e_cond == CM_COND_DISCONNECTED) )
        {
            /* Case 1:
               Already received CM_COND_DISCONNECTED, now this pt_msg_conn message may be coming 
               from the case that the pipe reusing is cancelled. A real disconnected message is 
               then received after the fake disconnected message received previously. 
               Simply ignore this kind of messages. */
            /* Case 2:
               Not yet transit to the state that open connection. Now this pt_msg_conn
               message may come from the case that the pipe reusing is cancelled when 
               the running SVC_REQ is just selected but than stopped very soon. 
               The CONN handle kept in REC_SVC_REQ_T.t_conn_info_preset is forced to disconnect. */
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Unexpected CONN msg (e_src_op=%d, conn=%d, e_cond=%d). (L%d)\n",  
                        pt_running_svc_req->e_src_op_type,
                        pt_msg_conn->h_conn,
                        pt_msg_conn->e_cond, __LINE__));
        }
        
        return SVCTXR_OK;
    }
    else
    {
        if (pt_msg_conn->h_conn != pt_running_svc_req->h_conn)
        {
            if (pt_msg_conn->h_conn == pt_running_svc_req->h_conn_reusing_by_cm)
            {
                /* when pipe-reusing by BRDCST_CONN_REUSE_CONNECTION, 
                   the old connection will notify DISCONNECTED, just ignore this notify */
                
                if (pt_msg_conn->e_cond == CM_COND_DISCONNECTED)
                {
                    pt_running_svc_req->h_conn_reusing_by_cm = NULL_HANDLE;
                }
                
                return SVCTXR_OK;
            }
            else
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Unmatched connection handle (curr=%d, new=%d). (L%d)\n", 
                            pt_running_svc_req->h_conn,
                            pt_msg_conn->h_conn,
                            __LINE__));
                return SVCTXR_OK;
            }
        }
    }
#if 0    
    /*If record handler reuse pipe and get cm conditon is CM_COND_CONNECTING ,just ignore this notify*/
#ifdef MW_TV_AV_BYPASS_SUPPORT
    if((!svctx_is_reuse_pipe_disabled()) &&
        pt_msg_conn->e_cond == CM_COND_CONNECTING)
    {
        return SVCTXR_OK;
    }
#endif
#endif
    /* clear h_conn if it is disconnected */
    if (pt_msg_conn->e_cond == CM_COND_DISCONNECTED)
    {
        if (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_DEINIT_SVC        && 
            (pt_running_svc_req->ui4_src_op_flag & REC_SVC_DEINIT_SVC_FLAG_REUSE_PIPE) &&
            x_handle_valid (pt_msg_conn->h_conn))
        {
            pt_running_svc_req->t_conn_info_reuse.h_conn = pt_running_svc_req->h_conn;
        }
        
        pt_running_svc_req->h_conn = NULL_HANDLE;
    }
    
    /* update the condition */
    if (pt_msg_conn->e_cond == CM_COND_DISCONNECTED  ||
        pt_msg_conn->e_cond == CM_COND_CONNECTING    ||
        pt_msg_conn->e_cond == CM_COND_CONNECTED     ||
        pt_msg_conn->e_cond == CM_COND_DISCONNECTING)
    {
        pt_running_svc_req->e_cond_conn = pt_msg_conn->e_cond;
    }
    
    /* consistency checking */
    if (pt_msg_conn->e_cond == CM_COND_CONNECTED)
    {
        if (pt_running_svc_req->h_conn_reusing_by_cm != NULL_HANDLE)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        }
    }
    
    /* check queued msg */
    #ifdef MW_TV_AV_BYPASS_SUPPORT
    {
        if (pt_msg_conn->e_cond == CM_COND_CONNECTED ||
            pt_msg_conn->e_cond == CM_COND_DISCONNECTED)
        {
            i4_ret = svctx_mgr_check_queued_req (ACTIVE_LINK_NOW,
                                                 pt_running_svc_req->e_src_rec,
                                                 pt_svctx,
                                                 FALSE);
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to check queued req (%d). (L%d)\n", i4_ret, __LINE__));
            }                                         
        }
    }
    #endif    
    
    switch (pt_msg_conn->e_cond)
    {
    case CM_COND_CONNECTING:
        /* do nothing */
        break;
        
    case CM_COND_CONNECTED:

        if (pt_running_svc_req->b_on_timer_conn)
        {
            i4_ret = x_timer_stop (pt_running_svc_req->h_timer_conn);
            
            if (i4_ret != OSR_OK)
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to stop conn timer (%d). (L%d)\n", i4_ret, __LINE__));
            }
            
            pt_running_svc_req->b_on_timer_conn = FALSE;
        }
        
        i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                              REC_SVC_EVN_CONN_CONNECTED, 
                                              (VOID *) pt_msg_conn, 
                                              sizeof (SVCTX_REC_CONN_NFY_MSG_T));
                                              
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
        }
        break;

    case CM_COND_DISCONNECTING:
        /* do nothing */
        break;
        
    case CM_COND_DISCONNECTED:

        if (pt_running_svc_req->b_on_timer_conn)
        {
            i4_ret = x_timer_stop (pt_running_svc_req->h_timer_conn);
            
            if (i4_ret != OSR_OK)
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to stop conn timer (%d). (L%d)\n", i4_ret, __LINE__));
            }
            
            pt_running_svc_req->b_on_timer_conn = FALSE;
        }

        i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                              REC_SVC_EVN_CONN_DISCONNECTED, 
                                              (VOID *) pt_msg_conn, 
                                              sizeof (SVCTX_REC_CONN_NFY_MSG_T));
                                              
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
        }
        
        _rec_svc_util_send_cm_disconned_to_lead_svctx (
                pt_svctx, 
                (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_INIT_SVC) ? TRUE : FALSE);
        break;
    
    case CM_COND_CONNECTED_TRANS:
        /* do nothing */
        break;
    
    case CM_COND_CONNECTION_INFO:
    {
        BOOL    b_handled = FALSE;
		
#if 0//def CM_CONN_INFO_SIGNAL_LOST_TIMER_EXPIRED  //mark for build error
  		if (pt_msg_conn->ui4_data == CM_CONN_INFO_SIGNAL_LOST_TIMER_EXPIRED)
		{
			 i4_ret = svctx_rikstv_handle_signal_loss_check(pt_svctx);
			 break;
		}
#endif
#ifdef MW_TV_AV_BYPASS_SUPPORT
        
        if (pt_running_svc_req->e_cond_conn == CM_COND_CONNECTED)
        {
            REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl;
            
            i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req,
                                                       & pt_res_brdcst_svl );
            if (i4_ret == SVCTXR_OK)
            {
                switch (pt_msg_conn->ui4_data)
                {
                case CM_CONN_INFO_GOT_TUNER_SIGNAL:
                    if (pt_res_brdcst_svl->e_brdcst_type != BRDCST_TYPE_ANALOG)
                    {
                        pt_svctx->b_fake_disc = FALSE;
                    }
                    b_handled = TRUE;

                    i4_ret =  _rec_svc_util_start_query_crnt_evn(pt_running_svc_req);
                    if(RET_FAIL (i4_ret))
                    {
                        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to start query  (L%d)\n",  __LINE__));
                    }
                    
                    break;
                    
                case CM_CONN_INFO_LOST_TUNER_SIGNAL:
                    if (pt_res_brdcst_svl->e_brdcst_type != BRDCST_TYPE_ANALOG)
                    {
                        pt_svctx->b_fake_disc = TRUE;
                    }
                    b_handled = TRUE;
                    i4_ret =  _rec_svc_util_stop_query_crnt_evn(pt_running_svc_req);
                    if (RET_FAIL (i4_ret))
                    {
                        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to stop query  (L%d)\n",  __LINE__));
                    }
                    
                    break;				
                    
                default:
                    b_handled = FALSE;
                }
            }
            else
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get broadcast base (%d). (L%d)\n", i4_ret, __LINE__));
                
                if (pt_msg_conn->ui4_data == CM_CONN_INFO_GOT_TUNER_SIGNAL ||
                    pt_msg_conn->ui4_data == CM_CONN_INFO_LOST_TUNER_SIGNAL)
                {
                    b_handled = TRUE;
                }
            }
        }
        
#endif      

        if (!b_handled)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Un-handled CM_COND_CONNECTION_INFO notify (%d). (L%d)\n", pt_msg_conn->ui4_data, __LINE__));
        }    
        break;
    }
            
    default:
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Un-handled Connection notify (%d). (L%d)\n", pt_msg_conn->e_cond, __LINE__));
        i4_ret = SVCTXR_OK;
    }
    
    return i4_ret;
}
#ifdef MW_TV_AV_BYPASS_SUPPORT
static INT32 _rec_svc_msg_hdlr_queued_action (
    VOID*                       pv_tag,
    struct _SVCTX_MSG_T*        pt_msg)
{
    SVCTX_T*            pt_svctx = (SVCTX_T *) pv_tag;
    REC_SVC_REQ_T*      pt_running_svc_req = NULL;
    UINT8               ui1_msg_queued;
    INT32               i4_ret;

    if (pt_svctx == NULL)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] pt_svctx is NULL. (L%d)\n", __LINE__));
        return SVCTXR_INV_ARG;
    }

    pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL)
    {
        DBG_ERROR (("{SVCTX}  ERR:  pt_running_svc_req is NULL. (L%d)\n", __LINE__));
        return SVCTXR_INV_ARG;
    }
    
    if (!pt_running_svc_req->b_act_q_in_mgr ||
        _rec_svc_state_act_msg_q_check_empty (pt_running_svc_req))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    else
    {
        pt_running_svc_req->b_act_q_in_mgr = FALSE;
        
        i4_ret = _rec_svc_state_act_msg_q_peek (pt_running_svc_req, &ui1_msg_queued);
        if (RET_OK (i4_ret))
        {
            if (pt_msg->ui1_msg_id != ui1_msg_queued)
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
            }
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to peek act msg. (%d) (L%d)\n", i4_ret, __LINE__));
            return i4_ret;
        }
    }
        
    i4_ret = _rec_svc_util_proc_queued_act (pt_svctx, pt_running_svc_req);
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to process queued act (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    return i4_ret;
}
#endif
static INT32 _rec_svc_msg_hdlr_scdb_nfy (
    SVCTX_T*                    pt_svctx,
    const SVCTX_MSG_T*          pt_msg)
{
    REC_SVC_REQ_T*           pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    const SVCTX_SCDB_MSG_T*  pt_msg_scdb = & pt_msg->u.t_scdb;
    INT32                    i4_ret = SVCTXR_OK;

    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] SCDB (%d) notify (cond: %d, reason=0x%X, data=%d). (L%d)\n", 
               pt_msg_scdb->h_obj,
               pt_msg_scdb->e_cond, 
               pt_msg_scdb->ui4_reason, 
               pt_msg_scdb->ui4_data, 
               __LINE__));
    
    if (pt_running_svc_req->h_scdb != NULL_HANDLE)
    {
        if (pt_msg_scdb->h_obj != pt_running_svc_req->h_scdb)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Unmatched SCDB handle (curr=%d, new=%d). (L%d)\n", 
                        pt_running_svc_req->h_scdb,
                        pt_msg_scdb->h_obj,
                        __LINE__));
            
            /* transit to error state */
            i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                                  REC_SVC_EVN_ERROR, 
                                                  NULL, 
                                                  0);
            
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to error state (%d). (L%d)\n", i4_ret, __LINE__));
            }
            
            return i4_ret;
        }
    }
    else
    {
        /* simply ignore this scdb notify */
        return SVCTXR_OK;
    }
     
    pt_running_svc_req->e_cond_scdb = pt_msg_scdb->e_cond;
     
    switch (pt_msg_scdb->e_cond)
    {
    case SCDB_UPDATING:
        /* do nothing */
        break;
        
    case SCDB_UPDATED:
        i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                              REC_SVC_EVN_SCDB_UPDATED, 
                                              (VOID *) pt_msg_scdb, 
                                              sizeof (SVCTX_SCDB_MSG_T) );
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle EVN_SCDB_UPDATED (%d). (L%d)\n", i4_ret, __LINE__));
        }
        break;
        
    case SCDB_CLOSED:
        pt_running_svc_req->h_scdb = NULL_HANDLE;
        break;
        
    case SCDB_UNKNOWN:
    default:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Unhandled SCDB condition: %d. (L%d)\n", pt_msg_scdb->e_cond, __LINE__));
        i4_ret = SVCTXR_OK;
    }
    
    return i4_ret;
}

static INT32 _rec_svc_msg_hdlr_pat_nfy (
    SVCTX_T*                    pt_svctx,
    const SVCTX_MSG_T*          pt_msg)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    SVCTX_REC_PAT_NFY_MSG_T*    pt_msg_pat = (SVCTX_REC_PAT_NFY_MSG_T *) pt_msg->u.t_hdlr.aui1_data;
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl;
    TM_COND_T                   e_cond_pat_old = TM_COND_UNAVAILABLE;
    INT32                       i4_ret = SVCTXR_OK;

    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] PAT (%d) notify cond: %d. (L%d)\n", 
               pt_msg_pat->h_pat,
               pt_msg_pat->e_cond, 
               __LINE__));
    
    if (pt_msg->u.t_hdlr.z_len_data != sizeof (SVCTX_REC_PAT_NFY_MSG_T))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req,
                                               & pt_res_brdcst_svl );

    if (i4_ret == SVCTXR_OK)
    {
        e_cond_pat_old = pt_res_brdcst_svl->e_cond_pat;
        
        if (pt_msg_pat->h_pat == pt_res_brdcst_svl->h_pat)
        {
            pt_res_brdcst_svl->e_cond_pat = pt_msg_pat->e_cond;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Unmatched PAT handle (curr=%d, new=%d). (L%d)\n", 
                        pt_res_brdcst_svl->h_pat,
                        pt_msg_pat->h_pat,
                        __LINE__));
                        
            if (pt_res_brdcst_svl->h_pat != NULL_HANDLE)
            {
                i4_ret = SVCTXR_REC_INTERNAL_ERROR;
            }
            else
            {
                /* could be duplicated PAT UNAVAILABLE notification */
            }
        }
    }
    else
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get broadcast base (%d). (L%d)\n", i4_ret, __LINE__));
    }

    if (RET_FAIL (i4_ret))
    {
        /* transit to error state */
        i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                              REC_SVC_EVN_ERROR, 
                                              NULL, 
                                              0);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to error state (%d). (L%d)\n", i4_ret, __LINE__));
        }
        
        return i4_ret;
    }    
    
    switch (pt_msg_pat->e_cond)
    {
    case TM_COND_UNAVAILABLE:
        
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] PAT Unavailable. (L%d)\n", __LINE__));
        
        /* TODO: check if need to trigger a retry ? */
        /* TODO: need to enter error state if currently it is in SESS_OPENING state ? */
        pt_res_brdcst_svl->h_pat = NULL_HANDLE;
        
        i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                              REC_SVC_EVN_PAT_UNAVAIL,
                                              (VOID *) pt_msg_pat,
                                              sizeof (SVCTX_REC_PAT_NFY_MSG_T));
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
        }
        break;

    case TM_COND_TRANSITION:
        if (e_cond_pat_old != TM_COND_UNAVAILABLE)
        {
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] PAT Transition. (L%d)\n", __LINE__));
            /* TODO: check if need to take care this case. */
            /* DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Un-handled PAT notify (TM_COND_TRANSITION). (L%d)\n", __LINE__)); */
        }
        break;

    case TM_COND_AVAILABLE:
    case TM_COND_UPDATE:
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] PAT Updated/Available. (L%d)\n", __LINE__));
        i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                              REC_SVC_EVN_PAT_UPDATED,
                                              (VOID *) pt_msg_pat,
                                              sizeof (SVCTX_REC_PAT_NFY_MSG_T));
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
        }
        break;

    case TM_COND_SECTION:
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] PAT: TM_COND_SECTION. (L%d)\n", __LINE__));
        /* ignore */
        break;
    
    default:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Un-handled PAT notify (%d). (L%d)\n", pt_msg_pat->e_cond, __LINE__));
        i4_ret = SVCTXR_OK;
    }
    
    return i4_ret;
}    

/* [TODO] >// RRT */
#if 1

static INT32 _rec_svc_msg_hdlr_rrctx_nfy (
    SVCTX_T*                    pt_svctx,
    const SVCTX_MSG_T*          pt_msg)
{
    return SVCTXR_OK;
}

#else

static INT32 _rec_svc_msg_hdlr_rrctx_nfy (
    SVCTX_T*                    pt_svctx,
    const SVCTX_MSG_T*          pt_msg)
{
    REC_SVC_REQ_T*          pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    SVCTX_REC_RRCTX_MSG_T*  pt_msg_rrctx = (SVCTX_REC_RRCTX_MSG_T *) pt_msg->u.t_hdlr.aui1_data;
    VOID*                   pv_rrctx_data;
    SIZE_T                  z_len_rrctx_data;
    INT32                   i4_ret = SVCTXR_OK;

    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] RRCTX (%d) notify reason: %d (L%d)\n", 
               pt_msg_rrctx->h_rrctx,
               pt_msg_rrctx->ui4_reason,
               __LINE__));
               
    switch (pt_msg_rrctx->ui4_reason)
    {
    case RRCTX_REASON_RATE_UPDATED:
    case RRCTX_REASON_RATE_AVAIL:
        if (pt_running_svc_req->pt_rec_cfg->e_profile == SVCTX_REC_PROFILE_ADVANCED)
        {
            i4_ret = _rec_svc_util_get_rrctx_data (pt_running_svc_req,
                                                   & pv_rrctx_data,
                                                   & z_len_rrctx_data);
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get rrctx data (%d). (L%d)\n", i4_ret, __LINE__));
                return i4_ret;
            }
            
            i4_ret = _rec_svc_util_update_rrctx_data (pt_running_svc_req,
                                                      pv_rrctx_data,
                                                      z_len_rrctx_data);
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to update rrctx data (%d). (L%d)\n", i4_ret, __LINE__));
                return i4_ret;
            }
        }
        break;
        
    case RRCTX_REASON_RATE_UNAVAIL:
    case RRCTX_REASON_HANDLE_CLOSED:
        if (pt_running_svc_req->pt_rec_cfg->e_profile == SVCTX_REC_PROFILE_ADVANCED)
        {
            i4_ret = _rec_svc_util_update_rrctx_data (pt_running_svc_req,
                                                      NULL,
                                                      0);
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to update rrctx data (%d). (L%d)\n", i4_ret, __LINE__));
                return i4_ret;
            }
        }
        break;
    
    default:
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Un-handled RRCTX nfy reason (%d). (L%d)\n", 
                   pt_msg_rrctx->ui4_reason, 
                   __LINE__));
        i4_ret = SVCTXR_OK;
    }
    
    return i4_ret;
}

#endif

static INT32 _rec_svc_msg_hdlr_rec_util_nfy (
    SVCTX_T*                    pt_svctx,
    const SVCTX_MSG_T*          pt_msg)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    SVCTX_REC_UTIL_NFY_MSG_T*   pt_msg_rec_util = (SVCTX_REC_UTIL_NFY_MSG_T *) pt_msg->u.t_hdlr.aui1_data;
    REC_UTIL_VALID_RANGE_NTFY_INFO_T*  pt_valid_range = NULL;
    PVR_TICK_INDEX_T                              t_tick_index ;
    INT32                       i4_ret = SVCTXR_OK;
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] REC_UTIL (%d) notify code: %d (L%d)\n", 
               pt_msg_rec_util->h_rec_util,
               pt_msg_rec_util->e_ntfy_code,
               __LINE__));

    if (pt_msg->u.t_hdlr.z_len_data != sizeof (SVCTX_REC_UTIL_NFY_MSG_T))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    if (pt_running_svc_req->h_rec_util == NULL_HANDLE || 
        !x_handle_valid (pt_msg_rec_util->h_rec_util))
    {
        /* the record utility must have been closed */
        return SVCTXR_OK;
    }
    
    /* check REC_UTIL handle */
    if (pt_msg_rec_util->h_rec_util != pt_running_svc_req->h_rec_util)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Unmatched REC_UTIL handle (curr=%d, new=%d). (L%d)\n", 
                    pt_running_svc_req->h_rec_util,
                    pt_msg_rec_util->h_rec_util,
                    __LINE__));
        
        /* transit to error state */
        i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                              REC_SVC_EVN_ERROR, 
                                              NULL, 
                                              0);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to error state (%d). (L%d)\n", i4_ret, __LINE__));
        }
        
        return i4_ret;
    }
    
    switch (pt_msg_rec_util->e_ntfy_code)
    {
    case REC_UTIL_NTFY_CODE_COND_CHANGED:
        
        switch (pt_msg_rec_util->u.t_cond_chg.e_cond_new)
        {
        case REC_UTIL_COND_TRANSITION:
            
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] REC_UTIL (%d) enters TRANSITION (L%d)\n", 
                       pt_msg_rec_util->h_rec_util,
                       __LINE__));
            
            pt_running_svc_req->e_cond_rec_util = REC_UTIL_COND_TRANSITION;
            
            i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                                  REC_SVC_EVN_REC_UTIL_TRANS,
                                                  (VOID *) pt_msg_rec_util,
                                                  sizeof (SVCTX_REC_UTIL_NFY_MSG_T));

            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
            }
            break;

        case REC_UTIL_COND_READY:

            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] REC_UTIL (%d) enters READY (L%d)\n", 
                       pt_msg_rec_util->h_rec_util,
                       __LINE__));

            pt_running_svc_req->e_cond_rec_util = REC_UTIL_COND_READY;

            i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                                  REC_SVC_EVN_REC_UTIL_READY,
                                                  (VOID *) pt_msg_rec_util,
                                                  sizeof (SVCTX_REC_UTIL_NFY_MSG_T));
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
            }
            break;
            
        default:
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Un-handled REC_UTIL condition (%d). (L%d)\n", pt_msg_rec_util->u.t_cond_chg.e_cond_new, __LINE__));
        }
        
        break;
        
    case REC_UTIL_NTFY_CODE_SET_CONFIG_ACK:
        if (pt_msg_rec_util->u.t_set_cfg_ack.ui4_req_id == pt_running_svc_req->ui4_req_id_set_cfg)
        {
            switch (pt_msg_rec_util->u.t_set_cfg_ack.i4_result)
            {
            case REC_UTIL_REQR_OK:

                DBG_INFO (("{SVCTX} INFO: [REC_HDLR] REC_UTIL set config successful (req_id=%d). (L%d)\n", 
                           pt_msg_rec_util->u.t_set_cfg_ack.ui4_req_id,
                           __LINE__));
                
                break;

            case REC_UTIL_REQR_FAIL:

                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] REC_UTIL set config fail (req_id=%d). (L%d)\n", 
                            pt_msg_rec_util->u.t_set_cfg_ack.ui4_req_id,
                            __LINE__));
                
                /* transit to session error state */
                i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                                      REC_SVC_EVN_SESS_ERROR,
                                                      NULL,
                                                      0);
                if (RET_FAIL (i4_ret))
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
                }
                break;
            
            default:
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Un-handled REC_UTIL set config ack i4_result : %d. (L%d)\n", pt_msg_rec_util->u.t_set_cfg_ack.i4_result, __LINE__));
            }
            
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Mismatched set-config request id. (L%d)\n", __LINE__));
            
            /* transit to session error state */
            i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                                  REC_SVC_EVN_SESS_ERROR,
                                                  NULL,
                                                  0);
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
        
        pt_running_svc_req->ui4_req_id_set_cfg = REC_UTIL_INV_REQ_ID;
        break;

    case REC_UTIL_NTFY_CODE_VALID_RANGE_INFO_UPDATED:
        pt_valid_range = & ( pt_msg_rec_util->u.t_valid_range );
        i4_ret =  rec_util_get_tick_index( pt_msg_rec_util->h_rec_util,
                                                          pt_valid_range->t_valid_range.ui4_end,
                                                          &t_tick_index );
        if  ( i4_ret != REC_UTILR_OK )
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] rec_util_get_tick_index error (%d). (L%d)\n", i4_ret, __LINE__));
        }
        else
        {
            if  ( t_tick_index.ui4_flags & PVR_TICK_INDEX_FLAG_BAD)
            {
                /* send notification */
                if (pt_running_svc_req->pf_rec_nfy)
                {
                    if(pt_running_svc_req->pt_rec_cfg != NULL)
                    {
                        pt_running_svc_req->pf_rec_nfy (pt_running_svc_req->pt_rec_cfg->h_svctx_nfy,
                                                    pt_running_svc_req->pv_rec_nfy_tag,
                                                    SVCTX_NTFY_CODE_REC_BAD_TICK,
                                                    0 );
                    }
                }
                else
                {
                    DBG_ABORT (DBG_MOD_SVCTX);
                }  
                
            }
        }
        break;
    
    default:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Un-handled REC_UTIL notify (%d). (L%d)\n", pt_msg_rec_util->e_ntfy_code, __LINE__));
        i4_ret = SVCTXR_OK;
    }
    
    return i4_ret;
}

static INT32 _rec_svc_msg_hdlr_sess_nfy (
    SVCTX_T*                    pt_svctx,
    const SVCTX_MSG_T*          pt_msg)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    const SVCTX_SESS_MSG_T*     pt_msg_sess = & pt_msg->u.t_sess;
    SM_SESS_COND_T              e_cond_old;
    INT32                       i4_ret = SVCTXR_OK;
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] RCRD_SESS (%d) notify (cond: %d, evn: 0x%08X, data: %d). (L%d)\n", 
               pt_msg_sess->h_obj,
               pt_msg_sess->e_cond, 
               pt_msg_sess->e_event, 
               pt_msg_sess->ui4_data, 
               __LINE__));
    
    if (pt_msg_sess->h_obj != pt_running_svc_req->h_rcrd_sess)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Unmatched RCRD_SESS handle (curr=%d, new=%d). (L%d)\n", 
                    pt_running_svc_req->h_rcrd_sess,
                    pt_msg_sess->h_obj,
                    __LINE__));
        
        /* transit to error state */
        i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                              REC_SVC_EVN_ERROR, 
                                              NULL, 
                                              0);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to transit to error state (%d). (L%d)\n", i4_ret, __LINE__));
        }
        
        return i4_ret;
    }
    
    e_cond_old = pt_running_svc_req->e_cond_sess;
    pt_running_svc_req->e_cond_sess = pt_msg_sess->e_cond;

    /* clear h_rcrd_sess if necessary */
    if (pt_msg_sess->e_cond == SM_SESS_COND_CLOSED)
    {
        pt_running_svc_req->h_rcrd_sess = NULL_HANDLE;
    }
    
    /* session condition */
    switch (pt_msg_sess->e_cond)
    {
    case SM_SESS_COND_OPENED:
        if (e_cond_old == SM_SESS_COND_OPENING)
        {
            i4_ret = _rec_svc_state_handle_event (pt_svctx,
                                                  REC_SVC_EVN_SESS_OPENED,
                                                  (VOID *) pt_msg_sess,
                                                  sizeof (SVCTX_SESS_MSG_T));
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
        else if (e_cond_old == SM_SESS_COND_STOPPING)
        {
            i4_ret = _rec_svc_state_handle_event (pt_svctx,
                                                  REC_SVC_EVN_SESS_STOPPED,
                                                  (VOID *) pt_msg_sess,
                                                  sizeof (SVCTX_SESS_MSG_T));
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
        else if (e_cond_old != SM_SESS_COND_OPENED)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Inconsistent session state (new=%d, old=%d). (L%d)\n", pt_msg_sess->e_cond, e_cond_old, __LINE__));
            i4_ret = SVCTXR_REC_INTERNAL_ERROR;
        }
        
        break;
   
    case SM_SESS_COND_STARTED:
        if (e_cond_old == SM_SESS_COND_STARTING)
        {
            i4_ret = _rec_svc_state_handle_event (pt_svctx,
                                                  REC_SVC_EVN_SESS_STARTED,
                                                  (VOID *) pt_msg_sess,
                                                  sizeof (SVCTX_SESS_MSG_T));
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
        else if (e_cond_old != SM_SESS_COND_STARTED)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Inconsistent session state (new=%d, old=%d). (L%d)\n", pt_msg_sess->e_cond, e_cond_old, __LINE__));
            i4_ret = SVCTXR_REC_INTERNAL_ERROR;
        }
        break;

    case SM_SESS_COND_CLOSED:
        
        if (e_cond_old != SM_SESS_COND_CLOSING)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Inconsistent session state (new=%d, old=%d). (L%d)\n", pt_msg_sess->e_cond, e_cond_old, __LINE__));
        }
        
        i4_ret = _rec_svc_state_handle_event (pt_svctx,
                                              REC_SVC_EVN_SESS_CLOSED,
                                              (VOID *) pt_msg_sess,
                                              sizeof (SVCTX_SESS_MSG_T));
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (ret=%d). (L%d)\n", i4_ret, __LINE__));
        }
        break;
    
    case SM_SESS_COND_ERROR:
        
        if (e_cond_old != SM_SESS_COND_ERROR)
        {
            i4_ret = _rec_svc_state_handle_event (pt_svctx,
                                                  REC_SVC_EVN_SESS_ERROR,
                                                  (VOID *) pt_msg_sess,
                                                  sizeof (SVCTX_SESS_MSG_T));
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (ret=%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
        
        break;
        
    default:
        /* ignore */
        i4_ret = SVCTXR_OK;
    }
    
    /* session events */
    switch (pt_msg_sess->e_event)
    {
    case SM_EVN_SESS_RCRD_REACH_BOUND:
    case SM_EVN_SESS_RCRD_STRG_FULL:
    {
        SVCTX_NTFY_CODE_T   e_ntfy_code = 
            (pt_msg_sess->e_event == SM_EVN_SESS_RCRD_REACH_BOUND) ? 
                   SVCTX_NTFY_CODE_REC_REACH_BOUND : SVCTX_NTFY_CODE_REC_STRG_FULL;
        
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] %s. (L%d)\n", 
                   ((pt_msg_sess->e_event == SM_EVN_SESS_RCRD_REACH_BOUND) ? 
                        "Reach bound!" : "Storage full!"), __LINE__));
        
        /* send notification */
        if (pt_running_svc_req->pf_rec_nfy)
        {
            if(pt_running_svc_req->pt_rec_cfg != NULL)
            {
                pt_running_svc_req->pf_rec_nfy (pt_running_svc_req->pt_rec_cfg->h_svctx_nfy,
                                            pt_running_svc_req->pv_rec_nfy_tag,
                                            e_ntfy_code,
                                            0 );
            }
        }
        else
        {
            DBG_ABORT (DBG_MOD_SVCTX);
        }  
        
        rec_hdlr_invoke_client_svc_nfy_fct (pt_svctx,
                                            e_ntfy_code,
                                            ST_UNKNOWN );
        
        /* trigger session closing */
        i4_ret = _rec_svc_state_handle_event (pt_svctx,
                                              REC_SVC_EVN_REQ_CLOSE_SESS,
                                              NULL,
                                              0);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (ret=%d). (L%d)\n", i4_ret, __LINE__));
        }
        
        break;
    }
        
    default:
        /* ignore */
        i4_ret = SVCTXR_OK;
    }
    
    return i4_ret;
}    

static INT32 _rec_svc_msg_hdlr_strm_nfy (
    SVCTX_T*                    pt_svctx,
    const SVCTX_MSG_T*          pt_msg)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    SVCTX_REC_STRM_MSG_T*       pt_msg_strm = (SVCTX_REC_STRM_MSG_T *) pt_msg->u.t_hdlr.aui1_data;
    REC_SVC_STRM_INFO_T*        pt_strm;
    INT32                       i4_ret = SVCTXR_OK;
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] RCRD_STRM (%d) notify (cond: %d, evn: 0x%08X, data: %d). (L%d)\n", 
               pt_msg_strm->h_strm,
               pt_msg_strm->e_cond, 
               pt_msg_strm->e_event, 
               pt_msg_strm->u.ui4_data, 
               __LINE__));
    
    if (pt_msg->u.t_hdlr.z_len_data != sizeof (SVCTX_REC_STRM_MSG_T))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    pt_strm = _rec_svc_slst_get_strm_by_handle (pt_running_svc_req,
                                                pt_msg_strm->h_strm);
    
    if (pt_strm == NULL)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Stream not found. (L%d)\n", __LINE__));
        return SVCTXR_FAIL;
    }
    
    /* stream condition change */
    if (pt_msg_strm->e_cond != pt_strm->e_cond_strm)
    {
        SM_COND_T    e_cond_old = pt_strm->e_cond_strm;
        
        pt_strm->e_cond_strm = pt_msg_strm->e_cond;
    
        /* currently we only take care condition change for OPENED / CLOSED */
        switch (pt_msg_strm->e_cond)
        {
        case SM_COND_OPENED:
        {
            if (e_cond_old == SM_COND_OPENING || 
                e_cond_old == SM_COND_STOPPING)
            {
                if (pt_strm->e_type == REC_SVC_STRM_TYPE_ES &&
                    e_cond_old == SM_COND_OPENING)
                {
                    /* set the output enable/disable for this es stream */
                    i4_ret = _rec_svc_state_util_ctrl_es_list_output (pt_svctx);
                    
                    if (RET_FAIL (i4_ret))
                    {
                        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to control stream list output (%d). (L%d)\n", i4_ret, __LINE__));
                    }
                }
                
                i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                                      REC_SVC_EVN_STRM_OPENED,
                                                      (VOID *) pt_msg_strm,
                                                      sizeof (SVCTX_REC_STRM_MSG_T));
                if (RET_FAIL (i4_ret))
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
                }
            }
            else if (e_cond_old != SM_COND_OPENED)
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Inconsistent stream state (new=%d, old=%d). (L%d)\n", pt_msg_strm->e_cond, e_cond_old, __LINE__));
                i4_ret = SVCTXR_REC_INTERNAL_ERROR;
            }
            break;
        }
            
        case SM_COND_CLOSED:
        {
            REC_SVC_EVN_STRM_CLOSED_INFO_T      t_evn_info;
            
            /* backup the stream info */
            t_evn_info.t_strm_info = *pt_strm;
            t_evn_info.t_msg_strm  = *pt_msg_strm;
            
            /* remove related stream info */
            i4_ret = _rec_svc_state_util_remove_strm (pt_running_svc_req,
                                                      pt_strm->h_strm);
            
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to remove stream info (%d). (L%d)\n", i4_ret, __LINE__));
            }
            
            i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                                  REC_SVC_EVN_STRM_CLOSED,
                                                  (VOID *) & t_evn_info,
                                                  sizeof (REC_SVC_EVN_STRM_CLOSED_INFO_T));
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
            }
            break;
        }
        
        case SM_COND_ERROR:
        {
            i4_ret = _rec_svc_state_handle_event (pt_svctx,
                                                  REC_SVC_EVN_STRM_ERROR,
                                                  (VOID *) pt_msg_strm,
                                                  sizeof (SVCTX_REC_STRM_MSG_T));
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
            }
            break;
        }
            
        default:
            /* ignore */
            i4_ret = SVCTXR_OK;
        }
    }
    
    /* ca/cp events of es streams */
    if (pt_strm->e_type == REC_SVC_STRM_TYPE_ES &&
        REC_SVC_CA_CP_SM_EVENTS (pt_msg_strm->e_event))
    {
        SVCTX_REC_AUTH_STATUS_T  e_auth_st_new;
        
        /* keep ca/cp event info */
        switch (pt_msg_strm->e_event)
        {
        case SM_EVN_CA_COND_CLOSED:
        case SM_EVN_CA_COND_UNKNOWN:
        case SM_EVN_CA_COND_FREE:
        case SM_EVN_CA_COND_AUTHORIZED:
        case SM_EVN_CA_COND_AUTHORIZED_CA_SYS_ID:
        case SM_EVN_CA_COND_NOT_AUTHORIZED:
            pt_strm->u.t_es.e_ca_event    = pt_msg_strm->e_event;
            pt_strm->u.t_es.ui4_ca_detail = pt_msg_strm->u.ui4_ca_detail;
            break;
            
        case SM_EVN_CCI_INFORMATION:
            pt_strm->u.t_es.t_cp_info = pt_msg_strm->u.t_cp_info;
            break;
            
        default:
            DBG_ABORT (DBG_MOD_SVCTX);
        }
        
        /* decide auth state */
        e_auth_st_new = _rec_svc_state_util_decide_es_auth_status (pt_strm);
        
        if (e_auth_st_new != pt_strm->u.t_es.e_auth_st)
        {
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] ES (%d) authorization status change: %d->%d. (L%d)\n", 
                       pt_strm->h_strm,
                       pt_strm->u.t_es.e_auth_st,
                       e_auth_st_new,
                       __LINE__));
            
            pt_strm->u.t_es.e_auth_st = e_auth_st_new;

            i4_ret = _rec_svc_state_util_ctrl_es_list_output (pt_svctx);
            
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to control stream list output (%d). (L%d)\n", i4_ret, __LINE__));
                i4_ret = SVCTXR_OK;
            }

            /* only notify to the record controller */
            if (pt_running_svc_req->pf_rec_nfy)
            {
                if(pt_running_svc_req->pt_rec_cfg != NULL)
                {
                    pt_running_svc_req->pf_rec_nfy (pt_running_svc_req->pt_rec_cfg->h_svctx_nfy,
                                                pt_running_svc_req->pv_rec_nfy_tag,
                                                SVCTX_NTFY_CODE_REC_ES_AUTH_UPDATED,
                                                0 );
                }
            }
            else
            {
                DBG_ABORT (DBG_MOD_SVCTX);
            }  
        }
    }
    
    return i4_ret;
}        

static INT32 _rec_svc_msg_hdlr_watchdog_timer_nfy (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32           i4_ret;
    
    /*maybe restart timer due to double deinit,*/
    if (TRUE == pt_running_svc_req->b_restart_timer_wd)
    {
        pt_running_svc_req->b_restart_timer_wd = FALSE;
        return SVCTXR_OK;
    }
    
    pt_running_svc_req->b_on_timer_wd = FALSE;
    
    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Watchdog timeout! Force to enter error state. (L%d)\n", __LINE__));
    
    #ifndef MW_REC_SVC_ABORT_WATCHDOG_TIMEOUT
    {
        if (REC_SVC_STATE_IN_ANY_SESS_STATES (pt_running_svc_req->e_state))
        {
            i4_ret = _rec_svc_state_handle_event (pt_svctx,
                                                  REC_SVC_EVN_SESS_ERROR,
                                                  NULL,
                                                  0);
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
        else
        {
            i4_ret = _rec_svc_state_handle_event (pt_svctx,
                                                  REC_SVC_EVN_ERROR,
                                                  NULL,
                                                  0);
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
    }
    #else
    {
#ifdef CLI_LVL_ALL
        {
            extern INT32 x_cli_parser(const CHAR* ps_cmd);
            x_cli_parser ("mw.os.spa");
            x_cli_parser ("mw.handle.s");
        }
        #endif
        
        DBG_ABORT (DBG_MOD_SVCTX);
        i4_ret = SVCTXR_OK;
    }
    #endif
    
    return i4_ret;
}

static INT32 _rec_svc_msg_hdlr_reconn_timer_nfy (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32           i4_ret = SVCTXR_OK;
    
    pt_running_svc_req->b_on_timer_conn = FALSE;
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Reconnect timer is up -> Request to reconnect. (L%d)\n", __LINE__));
    
    if (pt_running_svc_req->e_state == REC_SVC_STATE_WAIT_RECONN)
    {
        if (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_INIT_SVC)
        {
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] ==> Request to reconnect. (L%d)\n", __LINE__));
            i4_ret = _rec_svc_state_handle_event (pt_svctx,
                                                  REC_SVC_EVN_REQ_CONNECT,
                                                  NULL,
                                                  0);
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
        else
        {
            DBG_INFO (("{SVCTX} INFO: [REC_HDLR] ==> Ignore reconnect request. (L%d)\n", __LINE__));
            i4_ret = SVCTXR_OK;
        }
    }
    else
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        i4_ret = SVCTXR_REC_INTERNAL_ERROR;
    }
    
    return i4_ret;
}

static INT32 _rec_svc_msg_hdlr_conning_timeout_nfy (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    
    pt_running_svc_req->b_on_timer_conn = FALSE;
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Connecting timeout. (L%d)\n", __LINE__));
    
    _rec_svc_util_send_cm_disconned_to_lead_svctx (pt_svctx, TRUE);
    
    return SVCTXR_OK;
}

static INT32 _rec_svc_msg_hdlr_req_start_rec (
    SVCTX_T*                    pt_svctx,
    const SVCTX_MSG_T*          pt_msg)
{
    SVCTX_REC_REQ_START_REC_MSG_T*  pt_msg_info = 
                (SVCTX_REC_REQ_START_REC_MSG_T *) pt_msg->u.t_hdlr.aui1_data;
    INT32           i4_ret = SVCTXR_OK;
    
    if (pt_msg->u.t_hdlr.z_len_data != sizeof (SVCTX_REC_REQ_START_REC_MSG_T))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                          REC_SVC_EVN_REQ_OPEN_SESS, 
                                          (VOID *) pt_msg_info, 
                                          sizeof (SVCTX_REC_REQ_START_REC_MSG_T));
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
        return i4_ret;
    }

    i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                          REC_SVC_EVN_REQ_START_SESS, 
                                          NULL, 
                                          0);
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    return i4_ret;    
}    

static INT32 _rec_svc_msg_hdlr_req_stop_rec (
    SVCTX_T*                    pt_svctx)
{
    INT32    i4_ret;
    
    i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                          REC_SVC_EVN_REQ_STOP_SESS, 
                                          NULL, 
                                          0);

    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
    }

    i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                          REC_SVC_EVN_REQ_CLOSE_SESS, 
                                          NULL, 
                                          0);

    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    return i4_ret;
}    

static INT32 _rec_svc_msg_hdlr_req_pause_rec (
    SVCTX_T*                    pt_svctx)
{
    INT32    i4_ret;
    
    i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                          REC_SVC_EVN_REQ_STOP_SESS, 
                                          NULL, 
                                          0);

    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    return i4_ret;
}

static INT32 _rec_svc_msg_hdlr_req_resume_rec (
    SVCTX_T*                    pt_svctx)
{
    INT32    i4_ret;
    
    i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                          REC_SVC_EVN_REQ_START_SESS,
                                          NULL, 
                                          0);

    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    return i4_ret;
}    

static INT32 _rec_svc_msg_hdlr_req_set_es_list (
    SVCTX_T*                    pt_svctx,
    const SVCTX_MSG_T*          pt_msg)
{
    SVCTX_REC_REQ_SET_ES_LIST_MSG_T*    pt_msg_info = 
                (SVCTX_REC_REQ_SET_ES_LIST_MSG_T *) pt_msg->u.t_hdlr.aui1_data;
    INT32                               i4_ret;
                
    if (pt_msg->u.t_hdlr.z_len_data != sizeof (SVCTX_REC_REQ_SET_ES_LIST_MSG_T))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }
    
    i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                          REC_SVC_EVN_REQ_SET_ES_LIST, 
                                          (VOID *) pt_msg_info, 
                                          sizeof (SVCTX_REC_REQ_SET_ES_LIST_MSG_T));

    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
                    message entry point
 ----------------------------------------------------------------------------*/
INT32 rec_svc_internal_msg (
    SVCTX_T*                    pt_svctx,
    SVCTX_MSG_T*                pt_msg)
{
    INT32   i4_ret;
    
    i4_ret = SVCTXR_OK;
    
    switch (pt_msg->ui1_msg_id)
    {
    case SVCTX_MSG_REC_CM_PIPE_NFY:
        i4_ret = _rec_svc_msg_hdlr_cm_pipe_nfy (pt_svctx, pt_msg);
        break;
        
    case SVCTX_MSG_REC_CONN_NFY:
        i4_ret = _rec_svc_msg_hdlr_conn_nfy (pt_svctx, pt_msg);
        break;
        
    case SVCTX_MSG_REC_SCDB_NFY:
        i4_ret = _rec_svc_msg_hdlr_scdb_nfy (pt_svctx, pt_msg);
        break;

    case SVCTX_MSG_REC_PAT_NFY:
        i4_ret = _rec_svc_msg_hdlr_pat_nfy (pt_svctx, pt_msg);
        break;
        
    case SVCTX_MSG_REC_RRCTX_NFY:
        i4_ret = _rec_svc_msg_hdlr_rrctx_nfy (pt_svctx, pt_msg);
        break;
        
    case SVCTX_MSG_REC_UTIL_NFY:
        i4_ret = _rec_svc_msg_hdlr_rec_util_nfy (pt_svctx, pt_msg);
        break;

    case SVCTX_MSG_REC_SESS_NFY:
        i4_ret = _rec_svc_msg_hdlr_sess_nfy (pt_svctx, pt_msg);
        break;

    case SVCTX_MSG_REC_STRM_NFY:
        i4_ret = _rec_svc_msg_hdlr_strm_nfy (pt_svctx, pt_msg);
        break;

    case SVCTX_MSG_REC_WATCHDOG_TIMER_NFY:
        i4_ret = _rec_svc_msg_hdlr_watchdog_timer_nfy (pt_svctx);
        break;
        
    case SVCTX_MSG_REC_RECONN_TIMER_NFY:
        i4_ret = _rec_svc_msg_hdlr_reconn_timer_nfy (pt_svctx);
        break;
        
    case SVCTX_MSG_REC_CONNING_TIMEOUT_NFY:
        i4_ret = _rec_svc_msg_hdlr_conning_timeout_nfy (pt_svctx);
        break;

    case SVCTX_MSG_REC_REQ_START_REC:
        i4_ret = _rec_svc_msg_hdlr_req_start_rec (pt_svctx, pt_msg);
        break;
        
    case SVCTX_MSG_REC_REQ_STOP_REC:
        i4_ret = _rec_svc_msg_hdlr_req_stop_rec (pt_svctx);
        break;
        
    case SVCTX_MSG_REC_REQ_PAUSE_REC:
        i4_ret = _rec_svc_msg_hdlr_req_pause_rec (pt_svctx);
        break;

    case SVCTX_MSG_REC_REQ_RESUME_REC:
        i4_ret = _rec_svc_msg_hdlr_req_resume_rec (pt_svctx);
        break;
        
    case SVCTX_MSG_REC_REQ_SET_ES_LIST:
        i4_ret = _rec_svc_msg_hdlr_req_set_es_list (pt_svctx, pt_msg);
        break;

    default:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Un-handled message type (%d). (L%d)\n", pt_msg->ui1_msg_id, __LINE__));
        i4_ret = SVCTXR_REC_INTERNAL_ERROR;
    }
    
    return i4_ret;
}
    
/*-----------------------------------------------------------------------------
                    public functions implementations
 ----------------------------------------------------------------------------*/
INT32 rec_svc_preproc_svc (
    SVCTX_T*                    pt_svctx,
    HANDLE_T                    h_svctx,
    const SVCTX_HDLR_T*         pt_hdlr,
    const SRC_DESC_T*           pt_src_desc,
    const SNK_DESC_T*           pt_snk_desc,
    x_svctx_select_nfy_fct      pf_nfy,
    VOID*                       pv_nfy_tag,
    REC_SVC_REQ_T*              pt_svc_req)
{
    SRC_REC_INFO_T*     pt_src_rec_info;
    INT32               i4_ret;
    
    /* check source related parameters */
    if (pt_src_desc == NULL                                                || 
        pt_src_desc->ps_conn_src_name == NULL                              ||
        x_strlen (pt_src_desc->ps_conn_src_name) > SVCTX_SNK_NAME_MAX_SIZE ||
        pt_src_desc->e_type != SRC_TYPE_RECORD                             ||
        pt_src_desc->pv_details == NULL ) 
    {
        DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] Invalid source descriptor. (L%d)\n", __LINE__));
        return SVCTXR_INV_ARG;
    }

    /* check sink related parameters */
    if (pt_snk_desc == NULL                    ||
        pt_snk_desc->ps_snk_grp_name == NULL   ||
        x_strlen (pt_snk_desc->ps_snk_grp_name) > SVCTX_SNK_NAME_MAX_SIZE)
    {
        DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] Invalid sink descriptor. (L%d)\n", __LINE__));
        return SVCTXR_INV_ARG;
    }        

    /* 1. init source related info */    
    x_strcpy (pt_svc_req->s_src_grp_name,
              pt_src_desc->ps_conn_src_name);
                   
    pt_src_rec_info = (SRC_REC_INFO_T *) pt_src_desc->pv_details;

    pt_svc_req->e_src_rec = pt_src_rec_info->e_src_rec;
                   
    switch (pt_src_rec_info->e_src_rec)
    {
    case SRC_TYPE_BRDCST_SVL:
    {
        SRC_SVL_INFO_T*  pt_src_svl_info = (SRC_SVL_INFO_T *) pt_src_rec_info->pv_detail;

        if(pt_src_svl_info== NULL)
        {
            pt_src_svl_info = (SRC_SVL_INFO_T*) &pt_src_desc->u_info.t_svl;
        }
        
        i4_ret = _rec_svc_util_preproc_svc_brdcst_svl (pt_svctx,
                                                       pt_svc_req,
                                                       pt_src_svl_info );
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] Fail to preproc SVC_BRDCST_SVL (%d). (L%d)\n", i4_ret, __LINE__));
            return i4_ret;
        }
        break;
    }
        
    case SRC_TYPE_BRDCST_FREQ:
    case SRC_TYPE_AVC:
    default:
        DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] Not supported Source Type (%d) for recording. (L%d)\n", 
                   pt_src_rec_info->e_src_rec, 
                   __LINE__));
        return SVCTXR_NOT_SUPPORT;
    }

	/*for riks tv*/
    pt_svc_req->t_riks_tv = pt_src_desc->t_riks_tv;
    /* 2. init sink related info */    
    x_strcpy (pt_svc_req->t_parent.s_snk_grp_name,
              pt_snk_desc->ps_snk_grp_name);
    

    /* update source / sink group names */
    svctx_update_src_snk_names (pt_svctx,
                                pt_svc_req->s_src_grp_name,
                                pt_svc_req->t_parent.s_snk_grp_name );
    
    pt_svc_req->t_parent.h_svctx              = h_svctx;
    pt_svc_req->t_parent.pf_svc_nfy           = pf_nfy;
    pt_svc_req->t_parent.pv_svc_nfy_tag       = pv_nfy_tag;
    pt_svc_req->t_parent.ui4_stream_mode      = 0;                          /* NOT used */
    pt_svc_req->t_parent.t_stop_cb.pf_nfy     = NULL;                       /* NOT used */
    pt_svc_req->t_parent.t_stop_cb.pv_tag     = NULL;                       /* NOT used */
    pt_svc_req->t_parent.b_force_unblock      = TRUE;                       /* NOT used */
    pt_svc_req->t_parent.i4_speed             = REC_SVC_SESS_NORMAL_SPEED;

    #ifdef MW_TV_AV_BYPASS_SUPPORT
    {
        pt_svc_req->t_parent.e_src_type       = pt_src_rec_info->e_src_rec;
    }
    #else
    {
        pt_svc_req->t_parent.e_src_type       = pt_src_desc->e_type;
    }
    #endif

    pt_svc_req->t_parent.b_same_disp_region   = FALSE;                      /* NOT used */
    pt_svc_req->t_parent.t_scart_out.b_valid  = FALSE;                      /* NOT used */
    pt_svc_req->t_parent.t_scart_out.ui1_num_descs = 0;                     /* NOT used */
    pt_svc_req->t_parent.t_scart_out.pt_descs = NULL;                       /* NOT used */
    pt_svc_req->t_parent.pt_hdlr              = (SVCTX_HDLR_T *) pt_hdlr;
    
    pt_svc_req->pt_rec_cfg         = NULL;
    x_memset (&pt_svc_req->t_sess_key, 0, sizeof (MM_CRYPT_INFO_T));
    pt_svc_req->pf_rec_nfy         = NULL;
    pt_svc_req->pv_rec_nfy_tag     = NULL;
    pt_svc_req->e_media_type       = REC_SVC_MEDIA_TYPE_INVALID;
    pt_svc_req->e_state            = REC_SVC_STATE_NOT_INIT;
    pt_svc_req->e_src_op_type      = REC_SVC_SRC_OP_TYPE_NONE;
    pt_svc_req->ui4_src_op_flag    = 0;
    pt_svc_req->ui4_st_flag        = 0;
    pt_svc_req->ui2_num_q_evn      = 0;
    DLIST_INIT (&pt_svc_req->t_evn_q);
    pt_svc_req->ui4_sn_evn         = 0;
    pt_svc_req->h_cm_pipe          = NULL_HANDLE;
    pt_svc_req->e_cond_cm_pipe     = CM_COND_CLOSED;
    pt_svc_req->h_conn             = NULL_HANDLE;
    pt_svc_req->e_cond_conn        = CM_COND_DISCONNECTED;
    pt_svc_req->e_auth_st          = SVCTX_REC_AUTH_STATUS_UNKNOWN;
    pt_svc_req->b_svc_blked        = FALSE;
    #ifdef MW_TV_AV_BYPASS_SUPPORT
    pt_svc_req->t_svcst.e_phase    = PLAYER_ACTION_NONE;
    #endif
    pt_svc_req->b_act_q_in_mgr     = FALSE;
    _rec_svc_state_act_msg_q_init (pt_svc_req);
    pt_svc_req->h_scdb             = NULL_HANDLE;
    pt_svc_req->e_cond_scdb        = SCDB_CLOSED;
    pt_svc_req->h_rrctx            = NULL_HANDLE;
    pt_svc_req->pv_buf_rrctx       = NULL;
    pt_svc_req->z_len_buf_rrctx    = 0;
    pt_svc_req->pt_crnt_evn        = NULL;
    pt_svc_req->pv_crnt_evn_data   = (VOID *) pt_svc_req->aui1_crnt_evn;
    pt_svc_req->z_crnt_evn_len     = REC_SVC_PRG_EVN_DATA_SIZE_INIT;
    pt_svc_req->at_scdb_recs       = pt_svc_req->at_scdb_rec_buf;
    pt_svc_req->ui2_num_scdb_recs  = REC_SVC_SCDB_BUF_REC_NUM_INIT;
    pt_svc_req->h_rec_util         = NULL_HANDLE;
    pt_svc_req->e_cond_rec_util    = REC_UTIL_COND_TRANSITION;
    pt_svc_req->ui4_req_id_set_cfg = REC_UTIL_INV_REQ_ID;
    pt_svc_req->h_rcrd_sess        = NULL_HANDLE;
    pt_svc_req->e_cond_sess        = SM_SESS_COND_CLOSED;
    pt_svc_req->ui4_sess_flag      = 0;
    pt_svc_req->b_preset_conn_info = FALSE;
    x_memset (& pt_svc_req->t_conn_info_reuse, 0, sizeof (REC_SVC_CONN_INFO_T));
    x_memset (& pt_svc_req->t_conn_info_preset, 0, sizeof (REC_SVC_CONN_INFO_T));
    pt_svc_req->h_conn_reusing_by_cm = NULL_HANDLE;
    DLIST_INIT (& pt_svc_req->t_wking_strms);
    DLIST_INIT (& pt_svc_req->t_alloc_blks);
    x_memset (&pt_svc_req->t_pvr, 0, sizeof (REC_SVC_MEDIA_PVR_T));
    x_memset (&pt_svc_req->t_other, 0, sizeof (REC_SVC_MEDIA_OTHER_T));

#ifdef CLI_LVL_ALL 
    pt_svc_req->b_inform_paused = FALSE;
#endif    

    return SVCTXR_OK;
}
    
INT32 rec_svc_init_svc (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32           i4_ret;
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Init svc req 0x%X. (L%d)\n", 
               (UINT32) pt_running_svc_req,
               __LINE__));
    
    if (pt_running_svc_req == NULL)
    {
        DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] No running service request. (L%d)\n", __LINE__));
        return SVCTXR_WRONG_STATE;
    } 
    
    if (pt_running_svc_req->e_state != REC_SVC_STATE_NOT_INIT)
    {
        DBG_INFO(("{SVCTX}  INFO: [REC_HDLR] be stopping running service request. (L%d)\n", __LINE__));
    }
    
    if (pt_running_svc_req->e_src_op_type != REC_SVC_SRC_OP_TYPE_NONE)
    {
        DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] Inavid src op type when init service. (L%d)\n", __LINE__));
        return SVCTXR_WRONG_STATE;
    }
    
    pt_running_svc_req->e_src_op_type   = REC_SVC_SRC_OP_TYPE_INIT_SVC;
    pt_running_svc_req->ui4_src_op_flag = 0;
    
#ifdef MW_EDB_SUPPORT
#ifdef MW_EDB_FBM_SHARE_MEM
    i4_ret =  svctx_rikstv_set_edb(pt_running_svc_req->t_riks_tv.b_tune_alter_freq_if_loss_signal);
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to set riksTv flag to EDB. (L%d)\n", __LINE__));
        return i4_ret;
    }
#endif
#endif 

    /* >//TODO: check if recordable */
    i4_ret = _rec_svc_state_handle_event (pt_svctx,
                                          REC_SVC_EVN_REQ_INIT_SVC,
                                          NULL,
                                          0);
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    i4_ret = _rec_svc_state_handle_event (pt_svctx,
                                          REC_SVC_EVN_REQ_CONNECT,
                                          NULL,
                                          0);
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
    }

    return (pt_running_svc_req->e_cond_conn == CM_COND_CONNECTED) ? 
                    SVCTXR_OK : SVCTXR_PENDING;
}

INT32 rec_svc_deinit_svc (
    SVCTX_T*                    pt_svctx,
    BOOL                        b_reuse_pipe)
{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
   /* HANDLE_T        h_conn_disc = NULL_HANDLE, h_cm_pipe_close = NULL_HANDLE;*/
    /*CM_COND_T       e_cond_conn, e_cond_cm_pipe;*/
    INT32           i4_ret, i4_ret_status;

    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Deinit svc req 0x%X (reuse_pipe: %s). (L%d)\n", 
               (UINT32) pt_running_svc_req,
               b_reuse_pipe ? "Enable" : "Disable",
               __LINE__));

    if (pt_running_svc_req == NULL)
    {
        DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] No running service request. (L%d)\n", __LINE__));
        return SVCTXR_WRONG_STATE;
    }
    
    /* 1. If already in in REC_SVC_SRC_OP_TYPE_DEINIT_SVC, it can switch from PIPE_REUSE to 
          PIPE_NOT_REUSE mode and cannot switch from PIPE_NOT_REUSE to PIPE_REUSE mode.
       2. Close all preset resources if the running SVC_REQ is just selected and 
          not entering CONNECTING state yet.
     */
    do
    {
    if (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_DEINIT_SVC)
    {
        DBG_INFO(("\nREC_SVC_SRC_OP_TYPE_DEINIT_SVC,  ui4_src_op_flag= %d (L%d)\n",
                         pt_running_svc_req->ui4_src_op_flag, __LINE__));
        if ((pt_running_svc_req->ui4_src_op_flag & REC_SVC_DEINIT_SVC_FLAG_REUSE_PIPE) &&
            !b_reuse_pipe)
        {
            DBG_INFO(("\n  PIPE_REUSE--->PIPE_NOT_REUSE mode  (L%d)\n",__LINE__));
            
            /* switch to PIPE_NOT_REUSE mode */
            pt_running_svc_req->ui4_src_op_flag &= (~REC_SVC_DEINIT_SVC_FLAG_REUSE_PIPE);
            
            /*send REC_SVC_EVN_REUSE_PIPE_TO_NOT_REUSED*/
            i4_ret = _rec_svc_state_handle_event (pt_svctx,
                                                  REC_SVC_EVN_REUSE_PIPE_TO_NOT_REUSED,
                                                  NULL,
                                                  0);
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
            }
            #if 0
            if (pt_running_svc_req->ui4_src_op_flag & REC_SVC_DEINIT_SVC_FLAG_RUP_CONN_FAKE_CLOSED)
            {
                if (pt_running_svc_req->h_conn != NULL_HANDLE)
                {
                    h_conn_disc = pt_running_svc_req->h_conn;
                }
                else if (pt_running_svc_req->t_conn_info_reuse.h_conn != NULL_HANDLE)
                {
                    h_conn_disc = pt_running_svc_req->t_conn_info_reuse.h_conn;
                }
                else
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
                    h_conn_disc = NULL_HANDLE;
                }
            }

            if (pt_running_svc_req->ui4_src_op_flag & REC_SVC_DEINIT_SVC_FLAG_RUP_CM_PIPE_FAKE_CLOSED)
            {
                if (pt_running_svc_req->h_cm_pipe != NULL_HANDLE)
                {
                    h_cm_pipe_close = pt_running_svc_req->h_cm_pipe;
                }
                else if (pt_running_svc_req->t_conn_info_reuse.h_cm_pipe != NULL_HANDLE)
                {
                    h_cm_pipe_close = pt_running_svc_req->t_conn_info_reuse.h_cm_pipe;
                }
                else 
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
                    h_cm_pipe_close = NULL_HANDLE;
                }
            }
            #endif
        }
        else if (!(pt_running_svc_req->ui4_src_op_flag & REC_SVC_DEINIT_SVC_FLAG_REUSE_PIPE) &&
                   b_reuse_pipe)
        {
            /* ignore this case, keep in PIPE_NOT_REUSE mode */
            DBG_INFO(("\n Just keep in PIPE_NOT_REUSE mode  (L%d)\n",__LINE__));
                break; 
        }
        else
        {
            DBG_INFO(("\n ui4_src_op_flag=%d  (L%d)\n",pt_running_svc_req->ui4_src_op_flag, __LINE__));
                break;
        }
    }
    else if (pt_running_svc_req->e_src_op_type == REC_SVC_SRC_OP_TYPE_INIT_SVC)
    {
        BOOL  b_preset_applied;
        DBG_INFO(("\nREC_SVC_SRC_OP_TYPE_INIT_SVC, ui4_src_op_flag= %d (L%d)\n",
                          pt_running_svc_req->ui4_src_op_flag,__LINE__));
       /* switch to PIPE_NOT_REUSE mode */
        pt_running_svc_req->e_src_op_type   = REC_SVC_SRC_OP_TYPE_DEINIT_SVC;
        pt_running_svc_req->ui4_src_op_flag = 0;
        
        b_preset_applied = TRUE;
        
        #if 0
        if (pt_running_svc_req->t_conn_info_preset.h_conn != NULL_HANDLE)
        {
            h_conn_disc = pt_running_svc_req->t_conn_info_preset.h_conn;
            pt_running_svc_req->t_conn_info_preset.h_conn = NULL_HANDLE;
            b_preset_applied = FALSE;
        }
        
        if (pt_running_svc_req->t_conn_info_preset.h_cm_pipe != NULL_HANDLE)
        {
            h_cm_pipe_close = pt_running_svc_req->t_conn_info_preset.h_cm_pipe;
            pt_running_svc_req->t_conn_info_preset.h_cm_pipe = NULL_HANDLE;
            pt_running_svc_req->t_conn_info_preset.e_cond_cm_pipe = CM_COND_CLOSED;
            b_preset_applied = FALSE;
        }
        #endif
        
        if (b_reuse_pipe &&  b_preset_applied)
        {
            pt_running_svc_req->ui4_src_op_flag |= REC_SVC_DEINIT_SVC_FLAG_REUSE_PIPE;
        }
    }
    else
    {
    
        DBG_INFO(("\nREC_SVC_SRC_OP_TYPE_NONE, ui4_src_op_flag= %d (L%d)\n",
                       pt_running_svc_req->ui4_src_op_flag,__LINE__));
        pt_running_svc_req->e_src_op_type   = REC_SVC_SRC_OP_TYPE_DEINIT_SVC;
        pt_running_svc_req->ui4_src_op_flag = 0;
    
        /*repeated stop service req handle: maybe really do cm_disconnect and colse pipe*/
        if  ((x_handle_valid (pt_running_svc_req->t_conn_info_preset.h_cm_pipe))
            &&  (x_handle_valid (pt_running_svc_req->t_conn_info_preset.h_conn)))
        {
            if  (b_reuse_pipe)
            {
                 DBG_INFO(("{SVCTX}  INFO: [REC_HDLR] . repeated stop req wihin one ts stream and resue pipe again!!!(L%d)\n", __LINE__));
                 pt_running_svc_req->ui4_src_op_flag |= REC_SVC_DEINIT_SVC_FLAG_REUSE_PIPE;
            }
            else
            {
               /*send REC_SVC_EVN_REUSE_PIPE_TO_NOT_REUSED*/
                DBG_INFO(("{SVCTX}  INFO: [REC_HDLR] repeated stop req but need release previous reused pipe!!!. (L%d)\n", __LINE__));
                i4_ret = _rec_svc_state_handle_event (pt_svctx,
                                                      REC_SVC_EVN_REUSE_PIPE_TO_NOT_REUSED,
                                                      NULL,
                                                      0);
                if (RET_FAIL (i4_ret))
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
                }
                #if 0
                 if  (x_handle_valid(pt_running_svc_req->t_conn_info_preset.h_conn))
                 {
                     pt_running_svc_req->h_conn = pt_running_svc_req->t_conn_info_preset.h_conn;
                     pt_running_svc_req->e_cond_conn =  CM_COND_CONNECTED;
                     pt_running_svc_req->b_cm_real_disc= TRUE;
                     
                     DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] need  disconnet reused connection (%d). (L%d)\n",
                                        pt_running_svc_req->h_conn, __LINE__));
                 }
                 else
                 {
                     DBG_ASSERT (NULL_HANDLE,   DBG_MOD_SVCTX);
                 }
                 
                 if  (x_handle_valid(pt_running_svc_req->t_conn_info_preset.h_cm_pipe))
                 {
                     pt_running_svc_req->h_cm_pipe = pt_running_svc_req->t_conn_info_preset.h_cm_pipe;
                     pt_running_svc_req->e_cond_cm_pipe = CM_COND_OPENED;
                     pt_running_svc_req->b_pipe_real_close = TRUE;
                     
                     DBG_INFO (("{SVCTX}  INFO: [REC_HDLR] need  disconnet reused pipe (%d). (L%d)\n", 
                                       pt_running_svc_req->h_cm_pipe, __LINE__));
                 }
                 else
                 {
                     DBG_ASSERT (NULL_HANDLE,   DBG_MOD_SVCTX);
                 }
                 
                 x_memset (& pt_running_svc_req->t_conn_info_preset, 0, sizeof (REC_SVC_CONN_INFO_T));
                 pt_running_svc_req->b_preset_conn_info = FALSE;
                 #endif
                 
            }
        }
    }
 #if 0   
    if (x_handle_valid (h_conn_disc))
    {
        i4_ret = CM_DISCONNECT (h_conn_disc, & e_cond_conn);
        if (i4_ret != CMR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to call x_cm_disconnect(handle=%d) (%d). (L%d)\n", 
                         h_conn_disc, i4_ret, __LINE__));
            
            i4_ret = SVCTXR_OK;
        }
        DBG_INFO(("{SVCTX}  INFO: [REC_HDLR] do CM_DISCONNECT!!!. (L%d)\n", __LINE__));
    }
    
    if (x_handle_valid (h_cm_pipe_close))
    {
        i4_ret = CM_CLOSE_PIPE (h_cm_pipe_close, & e_cond_cm_pipe);
        if (i4_ret != CMR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to call x_cm_close_pipe(handle=%d) (%d). (L%d)\n", 
                         h_cm_pipe_close, i4_ret, __LINE__));
            
            i4_ret = SVCTXR_OK;
        }
        DBG_INFO(("{SVCTX}  INFO: [REC_HDLR] do CM_CLOSE_PIPE!!!. (L%d)\n", __LINE__));
    }
#endif    
    i4_ret = _rec_svc_state_handle_event (pt_svctx,
                                          REC_SVC_EVN_REQ_DISCONNECT,
                                          NULL,
                                          0);
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    i4_ret = _rec_svc_state_handle_event (pt_svctx,
                                          REC_SVC_EVN_REQ_DEINIT_SVC,
                                          NULL,
                                          0);
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
    }
    }while(0);
    
    if (pt_running_svc_req->e_state == REC_SVC_STATE_NOT_INIT)
    {
        if  (pt_running_svc_req->b_pipe_real_close || pt_running_svc_req->b_cm_real_disc)
        {
            i4_ret_status = SVCTXR_PENDING;
        }
        else
        {
        i4_ret_status = SVCTXR_OK;
    }
    }
    else
    {
        i4_ret_status = SVCTXR_PENDING;
        
       #if 0
        /* trigger the watchdog timer */
        if (!pt_running_svc_req->b_on_timer_wd)
        {
            i4_ret = x_timer_start (pt_running_svc_req->h_timer_wd,
                                    REC_SVC_WATCHDOG_DELAY,
                                    X_TIMER_FLAG_ONCE,
                                    _rec_svc_cb_timer_watchdog_nfy,
                                    (VOID *) pt_svctx->h_sys_svctx );
                                    
            if (i4_ret == OSR_OK)                                    
            {
                pt_running_svc_req->b_on_timer_wd = TRUE;
            }
            else
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to start watchdog timer (%d). (L%d)\n", i4_ret, __LINE__));
            }
        }
        #endif
    }

    if (SVCTXR_PENDING == i4_ret_status)
    {
        if (TRUE == pt_running_svc_req->b_on_timer_wd)
        {
            i4_ret = x_timer_stop (pt_running_svc_req->h_timer_wd);
            
            if (i4_ret != OSR_OK)
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to stop h_timer_wd (%d). (L%d)\n", i4_ret, __LINE__));
                i4_ret = SVCTXR_OK;
            }
            pt_running_svc_req->b_restart_timer_wd = TRUE;
            
        }

        i4_ret = x_timer_start (pt_running_svc_req->h_timer_wd,
                                REC_SVC_WATCHDOG_DELAY,
                                X_TIMER_FLAG_ONCE,
                                _rec_svc_cb_timer_watchdog_nfy,
                                (VOID *) pt_svctx->h_sys_svctx );
                                
        if (i4_ret == OSR_OK)                                    
        {
            pt_running_svc_req->b_on_timer_wd = TRUE;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to start watchdog timer (%d). (L%d)\n", i4_ret, __LINE__));
        }
        
    }
    
    DBG_INFO(("{SVCTX}  INFO: [REC_HDLR] deinit_svc  return %d!!!. (L%d)\n",i4_ret_status,  __LINE__));
    
    return i4_ret_status;
                        
}       

INT32 rec_svc_req_start_rec (
    SVCTX_T*                    pt_svctx,
    const SVCTX_REC_CONFIG_T*   pt_rec_cfg)
{
    REC_SVC_REQ_T*                  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    SIZE_T                          z_len_buf;
    UINT8*                          pui1_buf;
    SVCTX_REC_CONFIG_T*             pt_rec_cfg_msg;
    SVCTX_MSG_T                     t_msg;
    SVCTX_REC_REQ_START_REC_MSG_T*  pt_msg_info;
    BOOL                            b_mem_alloced;
    INT32                           i4_ret;
    
    if (pt_running_svc_req == NULL)
    {
        DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] No running service request. (L%d)\n", __LINE__));
        return SVCTXR_WRONG_STATE;
    }
    
    if (REC_SVC_STATE_DIRECT_FAIL_REQ_START_REC (pt_running_svc_req->e_state))
    {
        DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] Fail to request start record in wrong state. (L%d)\n", __LINE__));
        return SVCTXR_WRONG_STATE;
    }
    
    if (pt_running_svc_req->b_svc_blked)
    {
        DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] Fail to start record since service blocked. (L%d)\n", __LINE__));
        return SVCTXR_WRONG_STATE;
    }
    
    /* calculate pt_rec_cfg_msg's real size */
    i4_ret = _rec_svc_tool_calc_rec_cfg_size (pt_rec_cfg,
                                              & z_len_buf );
                                              
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to calculate size (%d). (L%d)\n", i4_ret, __LINE__));
        return i4_ret;
    }                                                
    
    b_mem_alloced = FALSE;
    
    do
    {
        pui1_buf = (UINT8 *) _rec_svc_util_mem_alloc (pt_running_svc_req, 
                                                      z_len_buf);
        
        if (pui1_buf == NULL)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Out of memory. (L%d)\n", __LINE__));
            i4_ret = SVCTXR_NO_RESOURCE;
            break;
        }
        
        b_mem_alloced = TRUE;
        
        pt_rec_cfg_msg = (SVCTX_REC_CONFIG_T *) pui1_buf;
        
        i4_ret = _rec_svc_tool_copy_rec_cfg (pt_rec_cfg_msg,
                                             z_len_buf,
                                             pt_rec_cfg);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to copy rec cfg. (L%d)\n", __LINE__));
            break;
        }
        
        t_msg.h_svctx             = pt_svctx->h_sys_svctx;
        t_msg.ui1_msg_id          = SVCTX_MSG_REC_REQ_START_REC;
        t_msg.pf_invoke_func      = NULL;
        t_msg.pv_tag              = NULL;
        t_msg.u.t_hdlr.z_len_data = sizeof (SVCTX_REC_REQ_START_REC_MSG_T);
        
        pt_msg_info = (SVCTX_REC_REQ_START_REC_MSG_T *) t_msg.u.t_hdlr.aui1_data;
        pt_msg_info->pt_rec_cfg = pt_rec_cfg_msg;
        pt_msg_info->z_len_cfg  = z_len_buf;
        
        i4_ret = svctx_send_msg (pt_svctx->h_sys_svctx,
                                 & t_msg );
                                 
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send message. (L%d)\n", __LINE__));
            break;
        }
        
    } while (FALSE);
    
    if (RET_FAIL (i4_ret))
    {
        if (b_mem_alloced)
        {
            _rec_svc_util_mem_free (pt_running_svc_req,
                                    (VOID *) pui1_buf);
        }
    }
        
    return i4_ret;    
}

INT32 rec_svc_req_stop_rec (
    SVCTX_T*                    pt_svctx)
{
    SVCTX_MSG_T   t_msg;
    INT32         i4_ret;

    if (pt_svctx->pt_running_svc_req == NULL)
    {
        DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] No running service request. (L%d)\n", __LINE__));
        return SVCTXR_WRONG_STATE;
    }

    t_msg.h_svctx             = pt_svctx->h_sys_svctx;
    t_msg.ui1_msg_id          = SVCTX_MSG_REC_REQ_STOP_REC;
    t_msg.pf_invoke_func      = NULL;
    t_msg.pv_tag              = NULL;
    t_msg.u.t_hdlr.z_len_data = 0;

    i4_ret = svctx_send_msg (pt_svctx->h_sys_svctx,
                             & t_msg );
                             
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send message. (L%d)\n", __LINE__));
    }
    
    return i4_ret;    
}

INT32 rec_svc_req_pause_rec (
    SVCTX_T*                    pt_svctx)
{
    SVCTX_MSG_T   t_msg;
    INT32         i4_ret;
 
    if (pt_svctx->pt_running_svc_req == NULL)
    {
        DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] No running service request. (L%d)\n", __LINE__));
        return SVCTXR_WRONG_STATE;
    }
 
    t_msg.h_svctx             = pt_svctx->h_sys_svctx;
    t_msg.ui1_msg_id          = SVCTX_MSG_REC_REQ_PAUSE_REC;
    t_msg.pf_invoke_func      = NULL;
    t_msg.pv_tag              = NULL;
    t_msg.u.t_hdlr.z_len_data = 0;

    i4_ret = svctx_send_msg (pt_svctx->h_sys_svctx,
                             & t_msg );
                             
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send message. (L%d)\n", __LINE__));
    }
    
    return i4_ret;    
}

INT32 rec_svc_req_resume_rec (
    SVCTX_T*                    pt_svctx)
{
    SVCTX_MSG_T   t_msg;
    INT32         i4_ret;

    if (pt_svctx->pt_running_svc_req == NULL)
    {
        DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] No running service request. (L%d)\n", __LINE__));
        return SVCTXR_WRONG_STATE;
    }
 
    t_msg.h_svctx             = pt_svctx->h_sys_svctx;
    t_msg.ui1_msg_id          = SVCTX_MSG_REC_REQ_RESUME_REC;
    t_msg.pf_invoke_func      = NULL;
    t_msg.pv_tag              = NULL;
    t_msg.u.t_hdlr.z_len_data = 0;

    i4_ret = svctx_send_msg (pt_svctx->h_sys_svctx,
                             & t_msg );
                             
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send message. (L%d)\n", __LINE__));
    }
    
    return i4_ret;    
}

INT32 rec_svc_req_set_es_list (
    SVCTX_T*                    pt_svctx,
    const SVCTX_REC_ES_LIST_T*  pt_es_list)
{
    REC_SVC_REQ_T*                    pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    SVCTX_MSG_T                       t_msg;
    SVCTX_REC_REQ_SET_ES_LIST_MSG_T*  pt_msg_info;
    SIZE_T                            z_len_esia;
    INT32                             i4_ret;

    if (pt_running_svc_req == NULL)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid state to set es list. (L%d)\n", __LINE__));
        return SVCTXR_WRONG_STATE;
    }

    if (pt_es_list->ui2_es_num > REC_SVC_MAX_RCRD_STRM_NUM)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Too many ES in ES_LIST. (L%d)\n", __LINE__));
        return SVCTXR_NO_RESOURCE;
    }

    t_msg.h_svctx             = pt_svctx->h_sys_svctx;
    t_msg.ui1_msg_id          = SVCTX_MSG_REC_REQ_SET_ES_LIST;
    t_msg.pf_invoke_func      = NULL;
    t_msg.pv_tag              = NULL;
    t_msg.u.t_hdlr.z_len_data = sizeof (SVCTX_REC_REQ_SET_ES_LIST_MSG_T);
    
    pt_msg_info = (SVCTX_REC_REQ_SET_ES_LIST_MSG_T *) t_msg.u.t_hdlr.aui1_data;

    pt_msg_info->t_es_list = *pt_es_list;
    
    if (pt_es_list->ui2_es_num > 0)
    {
        z_len_esia = sizeof (SVCTX_REC_ES_INFO_T) * pt_es_list->ui2_es_num;
        
        pt_msg_info->t_es_list.at_es_info = 
                (SVCTX_REC_ES_INFO_T *) _rec_svc_util_mem_alloc (pt_running_svc_req,
                                                                 z_len_esia);
        
        if (pt_msg_info->t_es_list.at_es_info == NULL)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Out of memory. (L%d)\n", __LINE__));
            return SVCTXR_NO_RESOURCE;
        }
        
        x_memcpy (pt_msg_info->t_es_list.at_es_info, 
                  pt_es_list->at_es_info,
                  z_len_esia );
    }
    else
    {
        pt_msg_info->t_es_list.at_es_info = NULL;
    }
    
    i4_ret = svctx_send_msg (pt_svctx->h_sys_svctx,
                             & t_msg );
                             
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to send message. (L%d)\n", __LINE__));
    }
 
    return i4_ret;
}

INT32 rec_svc_get_capability (
    SVCTX_T*                    pt_svctx,
    SVCTX_REC_CAP_INFO_T*       pt_cap_info)
{
    REC_SVC_REQ_T*              pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl;
    INT32                       i4_ret;
    
    if (pt_running_svc_req == NULL)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid state to get capability. (L%d)\n", __LINE__));
        return SVCTXR_WRONG_STATE;
    }
    
    i4_ret = _rec_svc_util_get_res_brdcst_svl (pt_running_svc_req, 
                                               & pt_res_brdcst_svl );
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get RES_BRDCST_SVL (%d). (L%d)\n", i4_ret, __LINE__));
        pt_res_brdcst_svl = NULL;
    }
    
    if (pt_res_brdcst_svl)
    {
        if (REC_SVC_SUPPORTED_BRDCST_TYPE (pt_res_brdcst_svl->e_brdcst_type))
        {
            pt_cap_info->b_recordable = TRUE;
        }
        else
        {
            pt_cap_info->b_recordable = FALSE;
        }
    }
    else
    {
        pt_cap_info->b_recordable = FALSE;
    }
    
    return SVCTXR_OK;
}

INT32 rec_svc_get_sys_res_info (
    SVCTX_T*                    pt_svctx,
    SVCTX_REC_SYS_RES_INFO_T*   pt_res_info)
{
    /* TODO */
    pt_res_info->ui2_max_es_num = 32 - 3;
    
    return SVCTXR_OK;
}        

INT32 rec_svc_get_rec_status (
    SVCTX_T*                    pt_svctx,
    SVCTX_REC_STATUS_T*         pt_rec_status)
{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32           i4_ret;
    
    x_memset (pt_rec_status, 0, sizeof (SVCTX_REC_STATUS_T));
    
    switch (pt_running_svc_req->e_state)
    {
    case REC_SVC_STATE_NOT_INIT:
        pt_rec_status->e_rec_cond = SVCTX_REC_COND_STOPPED;
        break;    
        
    case REC_SVC_STATE_DISCONNECTED:
        pt_rec_status->e_rec_cond = SVCTX_REC_COND_STOPPED;
        break;    
        
    case REC_SVC_STATE_CONNECTING:
        pt_rec_status->e_rec_cond = SVCTX_REC_COND_STOPPED;
        break;    
        
    case REC_SVC_STATE_CONNECTED:
        pt_rec_status->e_rec_cond = SVCTX_REC_COND_STOPPED;
        break;    
        
    case REC_SVC_STATE_DISCONNECTING:
        pt_rec_status->e_rec_cond = SVCTX_REC_COND_STOPPED;
        break;    
        
    case REC_SVC_STATE_WAIT_RECONN:
        pt_rec_status->e_rec_cond = SVCTX_REC_COND_STOPPED;
        break;
    
    case REC_SVC_STATE_SESS_OPENING:
        pt_rec_status->e_rec_cond = SVCTX_REC_COND_STOPPED;
        break;    
        
    case REC_SVC_STATE_SESS_OPENED:
        pt_rec_status->e_rec_cond = SVCTX_REC_COND_PAUSED;
        break;    
        
    case REC_SVC_STATE_SESS_STARTING:
        pt_rec_status->e_rec_cond = SVCTX_REC_COND_PAUSED;
        break;    
        
    case REC_SVC_STATE_SESS_STARTED:
        pt_rec_status->e_rec_cond = SVCTX_REC_COND_STARTED;
        break;    
        
    case REC_SVC_STATE_SESS_STOPPING:
        pt_rec_status->e_rec_cond = SVCTX_REC_COND_PAUSED;
        break;    
        
    case REC_SVC_STATE_SESS_CLOSING:
        pt_rec_status->e_rec_cond = SVCTX_REC_COND_STOPPED;
        break;    
        
    case REC_SVC_STATE_SESS_ERROR:
        pt_rec_status->e_rec_cond = SVCTX_REC_COND_ERROR;
        break;    
        
    case REC_SVC_STATE_ERROR:
        pt_rec_status->e_rec_cond = SVCTX_REC_COND_ERROR;
        break;    
    
    default:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        return SVCTXR_REC_INTERNAL_ERROR;
    }    
    
    /* TODO */
    if (pt_rec_status->e_rec_cond == SVCTX_REC_COND_STARTED ||
        pt_rec_status->e_rec_cond == SVCTX_REC_COND_PAUSED )
    {
        pt_rec_status->ui8_avail      = 0;
        pt_rec_status->ui8_fullness   = 0;
        
        switch (pt_running_svc_req->pt_rec_cfg->e_bound_type)
        {
        case SVCTX_REC_BOUND_TYPE_TIME:
            pt_rec_status->ui4_time_bound = ((SVCTX_REC_TIME_BOUND_INFO_T *) pt_running_svc_req->pt_rec_cfg->pv_bound_info)->ui4_ticks;
            break;
            
        case SVCTX_REC_BOUND_TYPE_SPACE:
            pt_rec_status->ui4_time_bound = 0;
            break;
            
        default:
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
            return SVCTXR_REC_INTERNAL_ERROR;
        }
        
        switch (pt_running_svc_req->pt_rec_cfg->e_profile)
        {
        case SVCTX_REC_PROFILE_SIMPLE:
        {
            UINT32  ui4_tick_count;
            
            i4_ret = rec_util_get_tick_count (pt_running_svc_req->h_rec_util,
                                              & ui4_tick_count);
            if (i4_ret != REC_UTILR_OK)
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get tick count (%d). (L%d)\n", i4_ret, __LINE__));
                return SVCTXR_FAIL;
            }
            
            pt_rec_status->ui4_rec_start = 0;
            pt_rec_status->ui4_rec_end   = (ui4_tick_count >= 1) ? (ui4_tick_count - 1) : 0;
            
            break;
        }   
            
        case SVCTX_REC_PROFILE_ADVANCED:
        {
            MM_BUF_RANGE_INFO_T     t_range;
            PVR_TICK_INDEX_T        t_tk_idx;
            
            i4_ret = rec_util_get_valid_range (pt_running_svc_req->h_rec_util,
                                               &t_range);
            if (i4_ret == REC_UTILR_OK)
            {                         
                i4_ret = rec_util_get_tick_index (pt_running_svc_req->h_rec_util,
                                                  t_range.ui4_start,
                                                  & t_tk_idx );
                if (i4_ret != REC_UTILR_OK)
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get tick index (%d). (L%d)\n", i4_ret, __LINE__));
                    return SVCTXR_FAIL;
                }
                
                pt_rec_status->ui4_rec_start = t_tk_idx.ui4_tick_num;
                
                i4_ret = rec_util_get_tick_index (pt_running_svc_req->h_rec_util,
                                                  t_range.ui4_end,
                                                  & t_tk_idx );
                if (i4_ret != REC_UTILR_OK)
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get tick index (%d). (L%d)\n", i4_ret, __LINE__));
                    return SVCTXR_FAIL;
                }
                
                pt_rec_status->ui4_rec_end = t_tk_idx.ui4_tick_num;
            }
            else if (i4_ret == REC_UTILR_INV_RANGE)
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid range (%d). (L%d)\n", i4_ret, __LINE__));
                return SVCTXR_FAIL;
            }
            else
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get valid range (%d). (L%d)\n", i4_ret, __LINE__));
                return SVCTXR_FAIL;
            }
            
            break;
        }
        
        default:
            DBG_ABORT (DBG_MOD_SVCTX);
        }
    }
    else
    {
        pt_rec_status->ui8_avail      = 0;
        pt_rec_status->ui8_fullness   = 0;
        pt_rec_status->ui4_time_bound = 0;
        pt_rec_status->ui4_rec_start  = 0;
        pt_rec_status->ui4_rec_end    = 0;
    }    
    
    return SVCTXR_OK;
}  
INT32 rec_svc_get_file_size(
    SVCTX_T*                    pt_svctx,
    UINT64*                     pui8_size)

{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32           i4_ret;
    
    *pui8_size = 0;    

    /* TODO */
    if (pt_running_svc_req->e_state == REC_SVC_STATE_SESS_OPENED||
        pt_running_svc_req->e_state == REC_SVC_STATE_SESS_STARTING||
        pt_running_svc_req->e_state == REC_SVC_STATE_SESS_STARTED ||
        pt_running_svc_req->e_state == REC_SVC_STATE_SESS_STOPPING)
    {
        
        switch (pt_running_svc_req->pt_rec_cfg->e_profile)
        {
            case SVCTX_REC_PROFILE_SIMPLE:
            {                
                i4_ret = rec_util_get_file_size(pt_running_svc_req->h_rec_util,
                                                pui8_size);
                if (i4_ret != REC_UTILR_OK)
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get tick count (%d). (L%d)\n", i4_ret, __LINE__));
                    return SVCTXR_FAIL;
                }
                
                break;
            }   
                
            case SVCTX_REC_PROFILE_ADVANCED:
            {
                break;   
            }
            
            default:
                DBG_ABORT (DBG_MOD_SVCTX);
        }
    }
    return SVCTXR_OK;
}

INT32 rec_svc_open_scdb (
    SVCTX_T*                    pt_svctx,
    HANDLE_T*                   ph_scdb,
    VOID*                       pv_tag,
    x_scdb_nfy_fct              pf_nfy)
{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32           i4_ret;
    
    if (pt_running_svc_req == NULL)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid state to open scdb. (L%d)\n", __LINE__));
        return SVCTXR_WRONG_STATE;
    }
    
    if (pt_running_svc_req->e_cond_conn == CM_COND_CONNECTED)
    {
        i4_ret = x_scdb_open (pt_running_svc_req->h_conn,
                              ph_scdb,
                              pv_tag,
                              pf_nfy );
        
        if (i4_ret != SCDBR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open scdb (%d). (L%d)\n", i4_ret, __LINE__));
            return SVCTXR_FAIL;
        }
    }
    else
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid state to open scdb. (L%d)\n", __LINE__));
        return SVCTXR_WRONG_STATE;
    }
    
    return SVCTXR_OK;
}
        
INT32 rec_svc_preset_conn_info (
    REC_SVC_REQ_T*              pt_svc_req,
    BOOL                        b_enabled,
    const REC_SVC_CONN_INFO_T*  pt_conn_info )
{
    if (!REC_SVC_STATE_PERMIT_PRESET_CONN_INFO (pt_svc_req->e_state))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid state [%d] to preset conn info. (L%d)\n", 
                    pt_svc_req->e_state, 
                    __LINE__));
                    
        return SVCTXR_WRONG_STATE;
    }
    
    /* check parameters */
    if ((!b_enabled && 
          pt_conn_info != NULL) ||
        ( b_enabled &&
         (pt_conn_info == NULL)))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid arguments. (L%d)\n", 
                    __LINE__));
        
        return SVCTXR_INV_ARG;
    }
    
    pt_svc_req->b_preset_conn_info = b_enabled;
    
    if (b_enabled)
    {
        pt_svc_req->t_conn_info_preset = *pt_conn_info;
    }
    else
    {
        x_memset (& pt_svc_req->t_conn_info_preset, 0, sizeof (REC_SVC_CONN_INFO_T));
    }
    
    return SVCTXR_OK;
}

INT32 rec_svc_update_block_status (
    SVCTX_T*            pt_svctx)
{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    BOOL            b_blocked;
    INT32           i4_ret;
    
    i4_ret = _rec_svc_state_util_check_block (pt_svctx,
                                              & b_blocked);
    if (RET_OK (i4_ret))
    {
        if (b_blocked)
        {
            if (!pt_running_svc_req->b_svc_blked)
            {
                pt_running_svc_req->b_svc_blked = TRUE;

                if (REC_SVC_STATE_IN_ANY_SESS_STATES (pt_running_svc_req->e_state) &&
                    (pt_running_svc_req->ui4_sess_flag & REC_SVC_SESS_FLAG_PVR_SVC))
                {
                    if (pt_running_svc_req->pf_rec_nfy)
                    {
                        if(pt_running_svc_req->pt_rec_cfg != NULL)
                        {
                            pt_running_svc_req->pf_rec_nfy (pt_running_svc_req->pt_rec_cfg->h_svctx_nfy,
                                                        pt_running_svc_req->pv_rec_nfy_tag,
                                                        SVCTX_NTFY_CODE_REC_BLOCKED,
                                                        0 );
                        }
                    }
                    else
                    {
                        DBG_ABORT (DBG_MOD_SVCTX);
                    }  
                
                    rec_hdlr_invoke_client_svc_nfy_fct (pt_svctx,
                                                        SVCTX_NTFY_CODE_SERVICE_BLOCKED,
                                                        ST_UNKNOWN );
                }
                
                i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                                      REC_SVC_EVN_SVC_BLOCKED, 
                                                      NULL, 
                                                      0);
                if (RET_FAIL (i4_ret))
                {
                    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
                }
            }
        }
        else
        {
            if (pt_running_svc_req->b_svc_blked)
            {
                pt_running_svc_req->b_svc_blked = FALSE;
            }
        }
    }
    else
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to check service block (%d). (L%d)\n", 
                    i4_ret,
                    __LINE__));
    }
    
    return i4_ret;
}

INT32 rec_svc_proc_queued_req_removed (
    SVCTX_T*            pt_svctx)
{
    INT32           i4_ret;
    
    i4_ret = _rec_svc_state_handle_event (pt_svctx, 
                                          REC_SVC_EVN_QREQ_REMOVED, 
                                          NULL, 
                                          0);
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle event (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    return i4_ret;
}

INT32 rec_svc_util_clear_rec_data (
    const CHAR*         s_head_path)
{
    INT32       i4_ret = SVCTXR_OK; 
    
    /* delete the main ts data file */
    
    i4_ret = rec_util_del_rec_data (s_head_path);
    if (i4_ret != REC_UTILR_OK)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to delete record data (%d). (L%d)\n", i4_ret, __LINE__));
        i4_ret = SVCTXR_FAIL;
    }                              
    
    return i4_ret;
}



#endif
