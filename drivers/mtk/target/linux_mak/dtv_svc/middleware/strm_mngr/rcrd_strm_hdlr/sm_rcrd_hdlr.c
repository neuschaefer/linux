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
 * $RCSfile: sm_rcrd_hdlr.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/1 $
 * $SWAuthor: Lianming Lin $
 * $MD5HEX: d052cebe5bec682f2354c25548053f2d $
 *
 * Description: 
 *         This file implements the Stream Manager's record stream handler.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "inc/x_mm_common.h"
#include "handle/u_handle.h"
#include "handle/x_handle.h"
#include "util/x_lnk_list.h"
#include "os/inc/x_os.h"

#include "strm_mngr/rcrd_strm_hdlr/sm_rcrd_hdlr.h"
#include "strm_mngr/rcrd_strm_hdlr/_sm_rcrd_hdlr_state.h"
#include "strm_mngr/rcrd_strm_hdlr/_sm_rcrd_hdlr_pat_pmt.h"
#include "strm_mngr/rcrd_strm_hdlr/_sm_rcrd_hdlr_pvr.h"

#include "strm_mngr/sess_rcrd_hdlr/u_sm_sess_rcrd.h"
#include "tbl_mngr/x_tm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define SM_RCRD_STRM_HDLR_MSGQ_MAX_SIZE     256
#define SM_RCRD_STRM_HDLR_MSGQ_SND_MAX_CNT  10
#define SM_RSH_MAX_STMT_LEN                 512
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static CHAR        aui1_rsh_dbg_stmt[SM_RSH_MAX_STMT_LEN];   /* Debug statement. */
static HANDLE_T    h_rsh_output_sema;                    /* References the output semaphore. */

static HANDLE_T    gh_rcrd_strm_hdlr = NULL_HANDLE;
static HANDLE_T    gh_rcrd_strm_msgq = NULL_HANDLE;


/*Record Stream State Machine*/
static SM_STATE_DESCR_T   at_rcrd_strm_state_descr_lst[] =
{
/*  ui4_state,  b_prcss_pending,  b_allow_pending,  pf_state_do,   pf_state_do_exception */
  {SM_COND_IGNORE,     FALSE,     FALSE,   NULL,                             sm_rcrd_strm_state_do_ignore_excpt},
  {SM_COND_OPENING,    FALSE,     TRUE,    sm_rcrd_strm_state_do_opening,    sm_rcrd_strm_state_do_opening_excpt},
  {SM_COND_OPENED,     TRUE,      FALSE,   sm_rcrd_strm_state_do_opened,     sm_rcrd_strm_state_do_opened_excpt},
  {SM_COND_STARTING,   FALSE,     TRUE,    sm_rcrd_strm_state_do_starting,   sm_rcrd_strm_state_do_starting_excpt},
  {SM_COND_STARTED,    TRUE,      FALSE,   sm_rcrd_strm_state_do_started,    sm_rcrd_strm_state_do_started_excpt},
  {SM_COND_STOPPING,   FALSE,     TRUE,    sm_rcrd_strm_state_do_stopping,   sm_rcrd_strm_state_do_stopping_excpt},
  {SM_COND_CLOSING,    FALSE,     TRUE,    sm_rcrd_strm_state_do_closing,    sm_rcrd_strm_state_do_closing_excpt},
  {SM_COND_CLOSED,     FALSE,     FALSE,   sm_rcrd_strm_state_do_closed,     sm_rcrd_strm_state_do_closed_excpt},
  {SM_COND_ERROR,      TRUE,      FALSE,   sm_rcrd_strm_state_do_error,      sm_rcrd_strm_state_do_error_excpt},
  SM_STATE_DESCR_NULL  
};

static UINT32     at_rcrd_strm_allowed_pending_event_lst[] =
{
    SM_EVN_RSH_APP_PLAY_REQ,
    SM_EVN_RSH_APP_STOP_REQ,
    SM_EVN_RSH_APP_CLOSE_REQ,
    SM_EVN_RSH_SCDB_CLOSED,
    SM_EVN_SESS_RCRD_APP_STOP_REQ,
    SM_EVN_SESS_RCRD_APP_PLAY_REQ,
    SM_EVN_IGNORE
};

static SM_STATE_COND_T    at_rcrd_strm_state_cond_lst[] =
{
  /* ui4_curr_state,       ui4_event,                  ui4_next_state,          ui4_pending_event,          pf_event_do */
  /* ignore state conditions *************************************************/ 
  {SM_COND_IGNORE,   SM_EVN_RSH_APP_OPEN_REQ,   SM_SESS_COND_OPENING,    SM_EVN_IGNORE,                  NULL},
  
  /* opening state conditions ************************************************/ 
  {SM_COND_OPENING,  SM_EVN_RSH_ALL_OPENED,           SM_COND_OPENED,     SM_EVN_IGNORE,                  NULL},
  {SM_COND_OPENING,  SM_EVN_RSH_APP_PLAY_REQ,         SM_COND_OPENING,    SM_EVN_RSH_APP_PLAY_REQ,        NULL},
  {SM_COND_OPENING,  SM_EVN_SESS_RCRD_APP_PLAY_REQ,   SM_COND_OPENING,    SM_EVN_SESS_RCRD_APP_PLAY_REQ,  NULL},
  {SM_COND_OPENING,  SM_EVN_RSH_APP_CLOSE_REQ,        SM_COND_OPENING,    SM_EVN_RSH_APP_CLOSE_REQ,       NULL},
  {SM_COND_OPENING,  SM_EVN_RSH_SCDB_CLOSED,          SM_COND_OPENING,    SM_EVN_RSH_APP_CLOSE_REQ,       NULL},
  {SM_COND_OPENING,  SM_EVN_RSH_ALL_PLAYED,           SM_COND_ERROR,      SM_EVN_IGNORE,                  NULL},
  {SM_COND_OPENING,  SM_EVN_RSH_ALL_STOPPED,          SM_COND_ERROR,      SM_EVN_IGNORE,                  NULL},
  {SM_COND_OPENING,  SM_EVN_RSH_ALL_CLOSED,           SM_COND_ERROR,      SM_EVN_IGNORE,                  NULL},
  {SM_COND_OPENING,  SM_EVN_RSH_COMP_ERR,             SM_COND_ERROR,      SM_EVN_IGNORE,                  NULL},
  {SM_COND_OPENING,  SM_EVN_RSH_INTERNAL_ERR,         SM_COND_ERROR,      SM_EVN_IGNORE,                  NULL},
  {SM_COND_OPENING,  SM_EVN_SESS_RCRD_APP_STOP_REQ,   SM_COND_OPENING,    SM_EVN_SESS_RCRD_APP_STOP_REQ,  NULL},
  {SM_COND_OPENING,  SM_EVN_RSH_APP_STOP_REQ,         SM_COND_OPENING,    SM_EVN_RSH_APP_STOP_REQ,        NULL},

  /* opened state conditions *************************************************/ 
  {SM_COND_OPENED,   SM_EVN_RSH_APP_PLAY_REQ,         SM_COND_STARTING,   SM_EVN_IGNORE,              NULL},
  {SM_COND_OPENED,   SM_EVN_SESS_RCRD_APP_PLAY_REQ,   SM_COND_STARTING,   SM_EVN_IGNORE,              NULL},
  {SM_COND_OPENED,   SM_EVN_RSH_APP_CLOSE_REQ,        SM_COND_CLOSING,    SM_EVN_IGNORE,              NULL},
  {SM_COND_OPENED,   SM_EVN_RSH_SCDB_CLOSED,          SM_COND_CLOSING,    SM_EVN_IGNORE,              NULL},
  {SM_COND_OPENED,   SM_EVN_RSH_ALL_OPENED,           SM_COND_ERROR,      SM_EVN_IGNORE,              NULL},
  {SM_COND_OPENED,   SM_EVN_RSH_ALL_PLAYED,           SM_COND_STARTING,   SM_EVN_IGNORE,              NULL},
  {SM_COND_OPENED,   SM_EVN_RSH_ALL_STOPPED,          SM_STATE_IGNORE,    SM_EVN_IGNORE,              NULL},
  {SM_COND_OPENED,   SM_EVN_RSH_ALL_CLOSED,           SM_COND_ERROR,      SM_EVN_IGNORE,              NULL},
  {SM_COND_OPENED,   SM_EVN_RSH_COMP_ERR,             SM_COND_ERROR,      SM_EVN_IGNORE,              NULL},
  {SM_COND_OPENED,   SM_EVN_RSH_INTERNAL_ERR,         SM_COND_ERROR,      SM_EVN_IGNORE,              NULL},

  /* starting state conditions ***********************************************/ 
  {SM_COND_STARTING, SM_EVN_RSH_ALL_PLAYED,           SM_COND_STARTED,    SM_EVN_IGNORE,                   NULL},
  {SM_COND_STARTING, SM_EVN_RSH_APP_STOP_REQ,         SM_COND_STARTING,   SM_EVN_RSH_APP_STOP_REQ,         NULL},
  {SM_COND_STARTING, SM_EVN_SESS_RCRD_APP_STOP_REQ,   SM_COND_STARTING,   SM_EVN_SESS_RCRD_APP_STOP_REQ,   NULL},
  {SM_COND_STARTING, SM_EVN_RSH_APP_CLOSE_REQ,        SM_COND_STARTING,   SM_EVN_RSH_APP_CLOSE_REQ,        NULL},
  {SM_COND_STARTING, SM_EVN_RSH_SCDB_CLOSED,          SM_COND_STARTING,   SM_EVN_RSH_APP_CLOSE_REQ,        NULL},
  {SM_COND_STARTING, SM_EVN_RSH_ALL_OPENED,           SM_COND_ERROR,      SM_EVN_IGNORE,                   NULL},
  {SM_COND_STARTING, SM_EVN_RSH_ALL_STOPPED,          SM_STATE_IGNORE,    SM_EVN_IGNORE,                   NULL},
  {SM_COND_STARTING, SM_EVN_RSH_ALL_CLOSED,           SM_COND_ERROR,      SM_EVN_IGNORE,                   NULL},
  {SM_COND_STARTING, SM_EVN_RSH_COMP_ERR,             SM_COND_ERROR,      SM_EVN_IGNORE,                   NULL},
  {SM_COND_STARTING, SM_EVN_RSH_INTERNAL_ERR,         SM_COND_ERROR,      SM_EVN_IGNORE,                   NULL},
  
  /* started state conditions ************************************************/ 
  {SM_COND_STARTED,  SM_EVN_RSH_APP_STOP_REQ,         SM_COND_STOPPING,   SM_EVN_IGNORE,                  NULL},
  {SM_COND_STARTED,  SM_EVN_SESS_RCRD_APP_STOP_REQ,   SM_COND_STOPPING,   SM_EVN_IGNORE,                  NULL},
  {SM_COND_STARTED,  SM_EVN_RSH_APP_CLOSE_REQ,        SM_COND_STOPPING,   SM_EVN_RSH_APP_CLOSE_REQ,       NULL},
  {SM_COND_STARTED,  SM_EVN_RSH_SCDB_CLOSED,          SM_COND_STOPPING,   SM_EVN_RSH_SCDB_CLOSED,         NULL},
  {SM_COND_STARTED,  SM_EVN_RSH_ALL_OPENED,           SM_COND_ERROR,      SM_EVN_IGNORE,                  NULL},
  {SM_COND_STARTED,  SM_EVN_RSH_ALL_PLAYED,           SM_STATE_IGNORE,    SM_EVN_IGNORE,                  NULL},
  {SM_COND_STARTED,  SM_EVN_RSH_ALL_STOPPED,          SM_COND_STOPPING,   SM_EVN_IGNORE,                  NULL},
  {SM_COND_STARTED,  SM_EVN_RSH_ALL_CLOSED,           SM_COND_ERROR,      SM_EVN_IGNORE,                  NULL},
  {SM_COND_STARTED,  SM_EVN_RSH_COMP_ERR,             SM_COND_ERROR,      SM_EVN_IGNORE,                  NULL},
  {SM_COND_STARTED,  SM_EVN_RSH_INTERNAL_ERR,         SM_COND_ERROR,      SM_EVN_IGNORE,                  NULL},

   /* stopping state conditions ***********************************************/ 
  {SM_COND_STOPPING, SM_EVN_RSH_ALL_STOPPED,         SM_COND_OPENED,     SM_EVN_IGNORE,                   NULL},
  {SM_COND_STOPPING, SM_EVN_RSH_APP_CLOSE_REQ,       SM_COND_STOPPING,   SM_EVN_RSH_APP_CLOSE_REQ,        NULL},
  {SM_COND_STOPPING, SM_EVN_RSH_SCDB_CLOSED,         SM_COND_STOPPING,   SM_EVN_RSH_SCDB_CLOSED,          NULL},
  {SM_COND_STOPPING, SM_EVN_RSH_APP_PLAY_REQ,        SM_COND_STOPPING,   SM_EVN_RSH_APP_PLAY_REQ,         NULL},
  {SM_COND_STOPPING, SM_EVN_SESS_RCRD_APP_PLAY_REQ,  SM_COND_STOPPING,   SM_EVN_SESS_RCRD_APP_PLAY_REQ,   NULL},
  {SM_COND_STOPPING, SM_EVN_RSH_ALL_OPENED,          SM_COND_ERROR,      SM_EVN_IGNORE,                   NULL},
  {SM_COND_STOPPING, SM_EVN_RSH_ALL_PLAYED,          SM_COND_ERROR,      SM_EVN_IGNORE,                   NULL},
  {SM_COND_STOPPING, SM_EVN_RSH_ALL_CLOSED,          SM_COND_ERROR,      SM_EVN_IGNORE,                   NULL},
  {SM_COND_STOPPING, SM_EVN_RSH_COMP_ERR,            SM_COND_ERROR,      SM_EVN_IGNORE,                   NULL},
  {SM_COND_STOPPING, SM_EVN_RSH_INTERNAL_ERR,        SM_COND_ERROR,      SM_EVN_IGNORE,                   NULL},

  /* closing state conditions ************************************************/ 
  {SM_COND_CLOSING,  SM_EVN_RSH_ALL_CLOSED,     SM_COND_CLOSED,     SM_EVN_IGNORE,                    NULL},

  /* error state conditions **************************************************/ 
  {SM_COND_ERROR,    SM_EVN_RSH_APP_CLOSE_REQ,        SM_COND_CLOSING,    SM_EVN_IGNORE,              NULL},
  {SM_COND_ERROR,    SM_EVN_RSH_SCDB_CLOSED,          SM_COND_CLOSING,    SM_EVN_IGNORE,              NULL},
  {SM_COND_ERROR,    SM_EVN_RSH_APP_PLAY_REQ,         SM_COND_ERROR,      SM_EVN_IGNORE,              NULL},
  {SM_COND_ERROR,    SM_EVN_RSH_APP_STOP_REQ,         SM_COND_ERROR,      SM_EVN_IGNORE,              NULL},
  {SM_COND_ERROR,    SM_EVN_SESS_RCRD_APP_PLAY_REQ,   SM_COND_ERROR,      SM_EVN_IGNORE,              NULL},
  {SM_COND_ERROR,    SM_EVN_SESS_RCRD_APP_STOP_REQ,   SM_COND_ERROR,      SM_EVN_IGNORE,              NULL},
  {SM_COND_ERROR,    SM_EVN_RSH_ALL_OPENED,           SM_COND_ERROR,      SM_EVN_IGNORE,              NULL},
  {SM_COND_ERROR,    SM_EVN_RSH_ALL_PLAYED,           SM_COND_ERROR,      SM_EVN_IGNORE,              NULL},
  {SM_COND_ERROR,    SM_EVN_RSH_ALL_STOPPED,          SM_COND_ERROR,      SM_EVN_IGNORE,              NULL},
  {SM_COND_ERROR,    SM_EVN_RSH_ALL_CLOSED,           SM_COND_CLOSING,    SM_EVN_IGNORE,              NULL},

  /* ANY state events ********************************************************/ 
  { SM_STATE_IGNORE,  SM_EVN_CCI_INFORMATION,              SM_STATE_IGNORE,  SM_EVN_IGNORE,   sm_rcrd_strm_state_do_cci_events },

  { SM_STATE_IGNORE,  SM_EVN_CA_COND_CLOSED,               SM_STATE_IGNORE,  SM_EVN_IGNORE,   sm_rcrd_strm_state_do_ca_events },
  { SM_STATE_IGNORE,  SM_EVN_CA_COND_UNKNOWN,              SM_STATE_IGNORE,  SM_EVN_IGNORE,   sm_rcrd_strm_state_do_ca_events },
  { SM_STATE_IGNORE,  SM_EVN_CA_COND_FREE,                 SM_STATE_IGNORE,  SM_EVN_IGNORE,   sm_rcrd_strm_state_do_ca_events },
  { SM_STATE_IGNORE,  SM_EVN_CA_COND_AUTHORIZED,           SM_STATE_IGNORE,  SM_EVN_IGNORE,   sm_rcrd_strm_state_do_ca_events },
  { SM_STATE_IGNORE,  SM_EVN_CA_COND_AUTHORIZED_CA_SYS_ID, SM_STATE_IGNORE,  SM_EVN_IGNORE,   sm_rcrd_strm_state_do_ca_events },
  { SM_STATE_IGNORE,  SM_EVN_CA_COND_NOT_AUTHORIZED,       SM_STATE_IGNORE,  SM_EVN_IGNORE,   sm_rcrd_strm_state_do_ca_events },

  {SM_STATE_IGNORE,       SM_EVN_RSH_PVR_OPENED,       SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_strm_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_RSH_PVR_PLAYED,       SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_strm_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_RSH_PVR_STOPPED,      SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_strm_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_RSH_PVR_CONNECTED,    SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_strm_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_RSH_PVR_DISCONNECTED, SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_strm_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_RSH_PVR_CLOSED,       SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_strm_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_RSH_PVR_ERR,          SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_strm_state_do_drv_events},

  {SM_STATE_IGNORE,       SM_EVN_RSH_PM_COND_PLAYED,   SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_strm_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_RSH_PM_COND_STOPPED,  SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_strm_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_RSH_PM_COND_CLOSED,   SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_strm_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_RSH_PM_COND_ERR,      SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_strm_state_do_drv_events},

  {SM_STATE_IGNORE,       SM_EVN_RSH_RUTI_OPENED,      SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_strm_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_RSH_RUTI_READY,       SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_strm_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_RSH_RUTI_TRANSITION,  SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_strm_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_RSH_RUTI_CLOSED,      SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_strm_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_RSH_RUTI_ERR,         SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_strm_state_do_drv_events},

  {SM_STATE_IGNORE,       SM_EVN_RSH_TM_PAT_SWAPED,    SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_strm_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_RSH_TM_PMT_SWAPED,    SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_strm_state_do_drv_events},
  
  /* !!! Don't remove this !!!************************************************/
  SM_STATE_COND_NULL
};



/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions implementation
 ----------------------------------------------------------------------------*/
 
/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_dbg_ctrl_stmt_api
 *
 * Description: This function prints record stream api debug info. 
 *
 * Inputs:  ps_format 
 *
 * Outputs: NULL  
 *
 * Returns: SMR_OK,             Success. 
 *          SMR_INTERNAL_ERROR: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-04-26 : initial
 ----------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_dbg_ctrl_stmt_api(const CHAR*  ps_format, ...)
{
    INT32           i4_return;
    VA_LIST         arg_list;


    i4_return = SMR_INV_ARG;
    if (ps_format != NULL)
    {
        i4_return = SMR_OK;
        
        VA_START(arg_list, ps_format);
        
        /* Wait for other debug statement to be finished. */
        x_sema_lock(h_rsh_output_sema, X_SEMA_OPTION_WAIT);
        
        x_memset(aui1_rsh_dbg_stmt, 0, SM_RSH_MAX_STMT_LEN);
        /* Create debug statement string and send it to output device. */
        x_vsnprintf(aui1_rsh_dbg_stmt, SM_RSH_MAX_STMT_LEN - 1, ps_format, arg_list);
        SM_DBG_API(SM_DBG_MOD_RSH, 
                  ("{SM} API: [RSH] %s", aui1_rsh_dbg_stmt));
        
        x_sema_unlock(h_rsh_output_sema);
        
        VA_END(arg_list);
    }
    else
    {
    }

    return (i4_return);
}
/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_dbg_ctrl_stmt_err
 *
 * Description: This function prints record stream info debug err. 
 *
 * Inputs:  ps_format 
 *
 * Outputs: NULL  
 *
 * Returns: SMR_OK,             Success. 
 *          SMR_INTERNAL_ERROR: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-04-26 : initial
 ----------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_dbg_ctrl_stmt_err(const CHAR*  ps_format, ...)
{
    INT32           i4_return;
    VA_LIST         arg_list;


    i4_return = SMR_INV_ARG;
    if (ps_format != NULL)
    {
        i4_return = SMR_OK;
        
        VA_START(arg_list, ps_format);
        
        /* Wait for other debug statement to be finished. */
        x_sema_lock(h_rsh_output_sema, X_SEMA_OPTION_WAIT);
        
        x_memset(aui1_rsh_dbg_stmt, 0, SM_RSH_MAX_STMT_LEN);
        /* Create debug statement string and send it to output device. */
        x_vsnprintf(aui1_rsh_dbg_stmt, SM_RSH_MAX_STMT_LEN - 1, ps_format, arg_list);
        SM_DBG_ERR(SM_DBG_MOD_RSH, 
                  ("{SM} ERR: [RSH] %s", aui1_rsh_dbg_stmt));
        
        x_sema_unlock(h_rsh_output_sema);
        
        VA_END(arg_list);
    }
    else
    {
    }

    return (i4_return);
}
/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_dbg_ctrl_stmt_info
 *
 * Description: This function prints record stream info debug info. 
 *
 * Inputs:  ps_format 
 *
 * Outputs: NULL  
 *
 * Returns: SMR_OK,             Success. 
 *          SMR_INTERNAL_ERROR: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-04-26 : initial
 ----------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_dbg_ctrl_stmt_info(const CHAR*  ps_format, ...)
{
    INT32           i4_return;
    VA_LIST         arg_list;


    i4_return = SMR_INV_ARG;
    if (ps_format != NULL)
    {
        i4_return = SMR_OK;
        
        VA_START(arg_list, ps_format);
        
        /* Wait for other debug statement to be finished. */
        x_sema_lock(h_rsh_output_sema, X_SEMA_OPTION_WAIT);
        
        x_memset(aui1_rsh_dbg_stmt, 0, SM_RSH_MAX_STMT_LEN);
        /* Create debug statement string and send it to output device. */
        x_vsnprintf(aui1_rsh_dbg_stmt, SM_RSH_MAX_STMT_LEN - 1, ps_format, arg_list);
        SM_DBG_INFO(SM_DBG_MOD_RSH, 
                   ("{SM} INFO: [RSH] %s", aui1_rsh_dbg_stmt));
        
        x_sema_unlock(h_rsh_output_sema);
        
        VA_END(arg_list);
    }
    else
    {
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name:  x_sm_rcrd_strm_hldr_init
 *
 * Description: This function inits record stream handler module. 
 *
 * Inputs:  NULL 
 *
 * Outputs: NULL  
 *
 * Returns: SMR_OK,             Success. 
 *          SMR_INTERNAL_ERROR: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-06 : initial
 ----------------------------------------------------------------------------*/

INT32 x_sm_rcrd_strm_hldr_init(VOID)
{
    INT32                    i4_ret;
    SM_HDLR_FCT_TBL_T        t_hdlr_fct_tbl;
    SM_HDLR_DESC_T           t_hdlr_desc;

    /* Create the required semaphores. */
    if (x_sema_create(&h_rsh_output_sema, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK)
           != OSR_OK)
    {
        SM_ABORT(SM_DBG_ABRT_CANNOT_CREATE_SEMAPHORE);
    }
    
    t_hdlr_fct_tbl.pf_open  = sm_rcrd_strm_hdlr_open;
    t_hdlr_fct_tbl.pf_close = sm_rcrd_strm_hdlr_close;
    t_hdlr_fct_tbl.pf_set   = sm_rcrd_strm_hdlr_set;
    t_hdlr_fct_tbl.pf_get   = sm_rcrd_strm_hdlr_get;
    t_hdlr_fct_tbl.pf_task  = sm_rcrd_strm_hdlr_task;
    
    x_strcpy(t_hdlr_desc.s_hdlr_name, SM_RCRD_STRM_HDLR_NAME);
    t_hdlr_desc.ui4_strm_type_mask = ST_MASK_RCRD_STRM;
    
    i4_ret = sm_reg_strm_handler(&t_hdlr_desc,
                                 SM_RCRD_SESS_HDLR_NAME,
                                 &t_hdlr_fct_tbl,
                                 FALSE,
                                 NULL,
                                 &gh_rcrd_strm_hdlr);
    if ((i4_ret != SMR_OK) || (NULL_HANDLE == gh_rcrd_strm_hdlr))
    {
        SM_RSH_DBG_ERR(("x_sm_rcrd_strm_hldr_init:"
                        "reg rcrd_strm handler fail(%d)\r\n", 
                        i4_ret));
        return SMR_INTERNAL_ERROR;
    }
    
    /* Create message queue for Record Session Task */
    if (NULL_HANDLE == gh_rcrd_strm_msgq)
    {
        i4_ret = x_msg_q_create(&gh_rcrd_strm_msgq,
                                SM_RCRD_STRM_HDLR_NAME,  
                                sizeof(SM_RCRD_STRM_HDLR_MSG_T),
                                SM_RCRD_STRM_HDLR_MSGQ_MAX_SIZE);
        if (i4_ret != OSR_OK)
        {
            SM_RSH_DBG_ERR(("x_sm_rcrd_strm_hldr_init:"
                            "create rcrd_strm queue fail(%d)\r\n", 
                            i4_ret));
            return SMR_INTERNAL_ERROR;
        }
    }   
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rs_send_msg
 *
 * Description: This function sends message to record stream handler's queque and 
 *              trigger selector thread to call record handler task callback to 
 *              handle it.
 *
 * Inputs:  
 *       pt_msg : message
 *       
 * Outputs: NULL  
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-12 : initial
 ------------------------------------------------------------------------------*/
VOID sm_rs_send_msg(const SM_RCRD_STRM_HDLR_MSG_T* pt_msg)
{
    INT32   i4_ret;
    UINT32  i4_i = 0;
    HANDLE_T    h_thread = NULL_HANDLE;

    /*Check parameters*/
    if (NULL == pt_msg)
    {
        SM_RSH_DBG_ERR(("sm_rs_send_msg:NULL message!\r\n"));
        return;
    }

    if (!x_handle_valid(pt_msg->h_stream))
    {
        SM_RSH_DBG_ERR(("sm_rs_send_msg:invalid handle(0X%8x)!\r\n", 
                        pt_msg->h_stream));
        return;
    }

    /*Send message to record stream handler and trigger selector thread to call
      handler's corresponding task callback to handle it */
    while (i4_i++ < SM_RCRD_STRM_HDLR_MSGQ_SND_MAX_CNT)
    {
        i4_ret = x_msg_q_send(gh_rcrd_strm_msgq,
                              (VOID*)pt_msg,
                              sizeof(SM_RCRD_STRM_HDLR_MSG_T),
                              0); /* highest priority */
        if (i4_ret != OSR_OK)
        {
            i4_ret = x_thread_self(&h_thread);
            if ( (i4_ret != OSR_OK )
                 || ((i4_ret == OSR_OK) && (FALSE == x_handle_valid(h_thread))))
            {
                break;
            }

            /* Send message fail, wait 10 ms and retry */
            x_thread_delay( 10 );
            continue;
        }
    
        i4_ret = sm_trigger_task(gh_rcrd_strm_hdlr, (VOID*)gh_rcrd_strm_hdlr);
        if (i4_ret != SMR_OK)
        {
            SM_ABORT(SM_DBG_ABRT_INV_CASE_STMT);
        }
        
        return;  /* success */
    }

    SM_ABORT(SM_DBG_ABRT_CANNOT_SEND_TO_MSG_Q);

    return;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rs_send_msg_do_event
 *
 * Description: This function sends event message to record stream  handler's queque 
 *              and trigger selector thread to call record stream handler's task  
 *              callback to handle it.
 *
 * Inputs:  
 *       ui4_event :event
 *       h_stream:  stream handle
 *       ui4_data:  additional data which comes from RM or driver(connect/disconnect reason)
 *
 * Outputs: NULL  
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-06 : initial
 ------------------------------------------------------------------------------*/
VOID sm_rs_send_msg_do_event(UINT32      ui4_event,
                             HANDLE_T    h_stream,
                             UINT32      ui4_data)
{
    SM_RCRD_STRM_HDLR_MSG_T  t_strm_msg;
    
    t_strm_msg.e_msg_type = SM_RCRD_STRM_MSG_TYPE_DO_EVENT;
    t_strm_msg.h_stream   = h_stream;
    t_strm_msg.u.t_rcrd_event_msg.ui4_event = ui4_event;
    t_strm_msg.u.t_rcrd_event_msg.ui4_data  = ui4_data;
    
    sm_rs_send_msg(&t_strm_msg);

    return;
}

/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_prc_pvr_msg
 *
 * Description: This function mainly handles messages come from RM for PVR
 *
 * Inputs:  
 *       h_stream:    stream handle
 *       pt_strm_obj: stream object
 *       pt_pvr_msg:  PVR message
 *         
 * Outputs: 
 *       NULL
 *
 * Returns: 
 *        SMR_OK: Succeed
 *        Others: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sm_rcrd_prc_pvr_msg(HANDLE_T                   h_stream,
                                  SM_RCRD_STREAM_T*          pt_rs_obj,
                                  SM_RCRD_STRM_MSG_RM_NFY_T* pt_pvr_msg)
{

    RM_COND_T                     e_new_cond;
    RM_COND_T                     e_old_cond;

    /*1.Check:if it is the opened PVR driver component?*/
    if ((pt_rs_obj->t_pvr_descr.t_info.ui1_in_port != pt_pvr_msg->ui1_port)
         || (pt_rs_obj->t_pvr_descr.h_comp != pt_pvr_msg->h_comp))
    {
        SM_RSH_DBG_ERR(("_sm_rcrd_prc_pvr_msg:input port(%d,%d) or "
                        "PVR handle(0x%.8X, 0x%.8X) unmatch!\r\n",
                        pt_rs_obj->t_pvr_descr.t_info.ui1_in_port,
                        pt_pvr_msg->ui1_port,
                        pt_rs_obj->t_pvr_descr.h_comp,
                        pt_pvr_msg->h_comp));

        sm_state_on_event(&pt_rs_obj->t_state,
                          SM_EVN_RSH_COMP_ERR,
                          (VOID*)pt_rs_obj,
                          NULL,
                          NULL);
        return SMR_INTERNAL_ERROR;
    }

    /*2.Filter out all set/get operation notify. */
    e_new_cond = rm_cond_chg(pt_pvr_msg->e_nfy_cond, 
                             RM_COND_IGNORE, 
                             COND_GRP_OPERATION);
    if(e_new_cond != RM_COND_NO_CHG)
    {
        SM_RSH_DBG_ERR(("_sm_rcrd_prc_pvr_msg:Filter out RM operation condition(0x%.8x) "
                        "notify.\r\n",
                        pt_pvr_msg->e_nfy_cond));
        return SMR_OK;
    }

    /*3.Update driver object state condition if necessary*/
    if ((pt_pvr_msg->b_should_update_comp_descr) 
        && (pt_rs_obj->t_pvr_descr.e_comp_cond != RM_COND_CLOSED))
    {
        e_old_cond = pt_rs_obj->t_pvr_descr.e_comp_cond;
        pt_rs_obj->t_pvr_descr.e_comp_cond_org = e_old_cond;
        pt_rs_obj->t_pvr_descr.e_comp_cond = pt_pvr_msg->e_nfy_cond;
    }
    else
    {
        e_old_cond = pt_rs_obj->t_pvr_descr.e_comp_cond_org;
    }

    e_new_cond = rm_cond_chg(pt_pvr_msg->e_nfy_cond, 
                             e_old_cond,
                             COND_GRP_OBJ_STATE);
    if (RM_COND_OPENED == e_new_cond)
    {
        sm_state_on_event(&pt_rs_obj->t_state,
                          SM_EVN_RSH_PVR_OPENED,
                          (VOID*)pt_rs_obj,
                          NULL,
                          NULL);
                          
        return SMR_OK;
    }
    else if (RM_COND_CLOSED == e_new_cond)
    {
        pt_rs_obj->t_pvr_descr.h_comp = NULL_HANDLE;
        sm_state_on_event(&pt_rs_obj->t_state,
                          SM_EVN_RSH_PVR_CLOSED,
                          (VOID*)pt_rs_obj,
                          NULL,
                          NULL);

        return SMR_OK;
    }
    else
    {
        /*Do nothing*/
    }

    
    /*4.Update driver object connect state condition if necesary*/
    e_new_cond = rm_cond_chg(pt_pvr_msg->e_nfy_cond,
                             e_old_cond,
                             COND_GRP_CONN_STATE);
    if (RM_COND_CONNECTED == e_new_cond)
    {
        sm_state_on_event(&pt_rs_obj->t_state,
                          SM_EVN_RSH_PVR_CONNECTED,
                          (VOID*)pt_rs_obj,
                          NULL,
                          NULL);
        return SMR_OK;
    }
    else if (RM_COND_DISCONNECTED == e_new_cond)
    {
        sm_state_on_event(&pt_rs_obj->t_state,
                          SM_EVN_RSH_PVR_DISCONNECTED,
                          (VOID*)pt_rs_obj,
                          NULL,
                          NULL);
        return SMR_OK;
    }
    else
    {
        /*Do nothing*/
    }
    
    return SMR_OK;
}

static INT32 _sm_rcrd_prc_pm_msg(HANDLE_T                   h_stream,
                                 SM_RCRD_STREAM_T*          pt_rs_obj,
                                 SM_RCRD_STRM_MSG_PM_NFY_T* pt_pm_msg)
{
    
    sm_state_on_event(&pt_rs_obj->t_state,
                      pt_pm_msg->ui4_pm_event,
                      (VOID*)pt_rs_obj,
                      NULL,
                      NULL);
    
    return SMR_OK;
}    
static VOID _sm_rcrd_prc_ca_msg(HANDLE_T                   h_stream,
                                        SM_RCRD_STREAM_T*           pt_rs_obj,
                                        SM_RCRD_STRM_MSG_CA_NFY_T*  pt_msg)
{
    sm_state_on_event(&pt_rs_obj->t_state,
                      pt_msg->ui4_ca_event,
                      (VOID*)pt_rs_obj,
                      (VOID*)pt_msg->ui4_detail_code,
                      NULL);
}

static VOID _sm_rcrd_prc_cp_msg(HANDLE_T                    h_stream,
                                        SM_RCRD_STREAM_T*            pt_rs_obj,
                                        SM_RCRD_STRM_MSG_CCI_NFY_T*  pt_msg)
{
    sm_state_on_event(&pt_rs_obj->t_state,
                      SM_EVN_CCI_INFORMATION,
                      (VOID*)pt_rs_obj,
                      (VOID*)&(pt_msg->t_pm_cp_info_lst),
                      NULL);
}


/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_prc_tm_msg
 *
 * Description: This function mainly handle messages come from Table Manager
 *
 * Inputs:  
 *       h_stream:    stream handle
 *       pt_strm_obj: stream object
 *       pt_tm_msg:   Table Manager message
 *         
 * Outputs: NULL
 *
 * Returns: 
 *        SMR_OK: Succeed
 *        Others: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sm_rcrd_prc_tm_msg(HANDLE_T                    h_stream,
                                 SM_RCRD_STREAM_T*           pt_rs_obj,
                                 SM_RCRD_STRM_MSG_TM_NFY_T*  pt_tm_msg)
{
    INT32                i4_ret;
    BOOL                 b_null_pat = FALSE;
    BOOL                 b_null_pmt = FALSE;
    
    if (NULL == pt_rs_obj)
    {
        SM_RSH_DBG_ERR(("_sm_rcrd_prc_tm_msg:pt_rs_obj is NULL \r\n"));
        return SMR_INV_ARG;
    }

    i4_ret = SMR_OK;

    if (SM_RCRD_TM_PAT == pt_tm_msg->ui4_table)
    {
        pt_rs_obj->e_pat_cond = pt_tm_msg->e_nfy_cond;
        if ((TM_COND_AVAILABLE == pt_rs_obj->e_pat_cond) 
             || (TM_COND_UPDATE == pt_rs_obj->e_pat_cond))
        {
            SM_RSH_DBG_INFO(("_sm_rcrd_prc_tm_msg:PAT state(%d)\r\n", 
                            pt_rs_obj->e_pat_cond));
            if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
                 && (RSH_TBL_GRP_PSI_PAT == pt_rs_obj->t_sect_info.e_tbl_grp))
            {
                if ((SM_COND_STARTING == pt_rs_obj->t_base.e_condition)
                     || (SM_COND_STARTED == pt_rs_obj->t_base.e_condition))
                {
                    i4_ret = sm_rcrd_strm_swap_pat(pt_rs_obj);
                    if (i4_ret != SMR_OK)
                    {
                        SM_RSH_DBG_ERR(("_sm_rcrd_prc_tm_msg:swap pat err=%d \r\n", 
                                        i4_ret));
                    }
                }
                else
                {
                }
            }
            else if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
                      && (RSH_TBL_GRP_PSI_PMT == pt_rs_obj->t_sect_info.e_tbl_grp))
            {
                if (TRUE == pt_rs_obj->b_pmt_loaded)
                {
                    x_tm_free(pt_rs_obj->h_pmt);
                    pt_rs_obj->b_pmt_loaded = FALSE;
                }
                i4_ret = sm_rcrd_strm_load_pmt(pt_rs_obj);
                if (i4_ret != SMR_OK)
                {
                    SM_RSH_DBG_ERR(("_sm_rcrd_prc_tm_msg:load pmt err=%d \r\n", 
                                    i4_ret));
                }
            }
            else
            {
                SM_RSH_DBG_ERR(("_sm_rcrd_prc_tm_msg:invalid state(%d)\r\n", 
                                pt_rs_obj->e_pat_cond));
            }
        }
        else if (TM_COND_UNAVAILABLE == pt_rs_obj->e_pat_cond)
        {
            if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
                 && (RSH_TBL_GRP_PSI_PAT == pt_rs_obj->t_sect_info.e_tbl_grp))
            {
                if ((SM_COND_STARTING == pt_rs_obj->t_base.e_condition)
                     || (SM_COND_STARTED == pt_rs_obj->t_base.e_condition))
                {
                    b_null_pat = TRUE;
                }
                
            }
            else if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
                      && (RSH_TBL_GRP_PSI_PMT == pt_rs_obj->t_sect_info.e_tbl_grp))
            {
                if ((SM_COND_STARTING == pt_rs_obj->t_base.e_condition)
                     || (SM_COND_STARTED == pt_rs_obj->t_base.e_condition))
                {
                    b_null_pmt = TRUE;
                }
            }
            else
            {
            }
            
            if ((SM_COND_STARTING == pt_rs_obj->t_base.e_condition)
                 || (SM_COND_STARTED == pt_rs_obj->t_base.e_condition))
            {
                pt_rs_obj->h_pat        = NULL_HANDLE;
                pt_rs_obj->b_pat_loaded = FALSE;
            }
        }
        else
        {
        }

        if (b_null_pat || b_null_pmt)
        {
            i4_ret = sm_rcrd_strm_pvr_set_null_pacekt(pt_rs_obj->t_base.h_stream,
                                                      pt_rs_obj);
            if (SMR_OK == i4_ret)
            {
                SM_RSH_DBG_INFO(("_sm_rcrd_prc_tm_msg:set PVR null pat OK\r\n"));
            }
            else
            {
                 SM_RSH_DBG_ERR(("_sm_rcrd_prc_tm_msg:set PVR null pat err=%d!\r\n", 
                                 i4_ret));
                 sm_rs_send_msg_do_event(SM_EVN_RSH_COMP_ERR, h_stream, 0);
                return SMR_DRV_COMP_ERROR;
            }
        }
    }
    else if (SM_RCRD_TM_PMT == pt_tm_msg->ui4_table)
    {
        if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
            && (RSH_TBL_GRP_PSI_PMT == pt_rs_obj->t_sect_info.e_tbl_grp))
        {
            pt_rs_obj->e_pmt_cond = pt_tm_msg->e_nfy_cond;
            SM_RSH_DBG_INFO(("_sm_rcrd_prc_tm_msg:PMT state(%d)\r\n", 
                            pt_rs_obj->e_pmt_cond));
            if ((TM_COND_AVAILABLE == pt_rs_obj->e_pmt_cond) 
                 || (TM_COND_UPDATE == pt_rs_obj->e_pmt_cond))
            {
                if (((SM_COND_STARTING == pt_rs_obj->t_base.e_condition)
                       || (SM_COND_STARTED == pt_rs_obj->t_base.e_condition)))
                {
                    if (0 != pt_rs_obj->t_pmt_info.ui2_num_es)
                    {
                        i4_ret = sm_rcrd_strm_swap_pmt(pt_rs_obj);
                        if (i4_ret != SMR_OK)
                        {
                            SM_RSH_DBG_ERR(("_sm_rcrd_prc_tm_msg:swap pmt err=%d \r\n", 
                                            i4_ret));
                        }
                    }
                    else
                    {
                        b_null_pmt = TRUE;
                    }
                }
                else
                {
                }
            }
            else if (TM_COND_UNAVAILABLE == pt_rs_obj->e_pmt_cond)
            {
                if ((SM_COND_STARTING == pt_rs_obj->t_base.e_condition)
                       || (SM_COND_STARTED == pt_rs_obj->t_base.e_condition))
                {
                    if (pt_tm_msg->h_table == pt_rs_obj->h_pmt)
                    {
                        pt_rs_obj->h_pmt    = NULL_HANDLE;
                    }
                    pt_rs_obj->b_pmt_loaded = FALSE;
                    b_null_pmt              = TRUE;
                }
            }
            else
            {
            }
            
            if (b_null_pmt)
            {
                i4_ret = sm_rcrd_strm_pvr_set_null_pacekt(pt_rs_obj->t_base.h_stream,
                                                          pt_rs_obj);
                if (SMR_OK == i4_ret)
                {
                    SM_RSH_DBG_INFO(("_sm_rcrd_prc_tm_msg:set PVR null pmt OK\r\n"));
                }
                else
                {
                     SM_RSH_DBG_ERR(("_sm_rcrd_prc_tm_msg:set PVR null pmt err=%d!\r\n", 
                                     i4_ret));
                     sm_rs_send_msg_do_event(SM_EVN_RSH_COMP_ERR, h_stream, 0);
                    return SMR_DRV_COMP_ERROR;
                }
            }
        }
        else
        {
            SM_RSH_DBG_ERR(("_sm_rcrd_prc_tm_msg:stream table error!\r\n"));
        }
    }
    else
    {
        i4_ret = SMR_INTERNAL_ERROR;
        SM_RSH_DBG_INFO(("_sm_rcrd_prc_tm_msg:invalid table(%d)!\r\n",
                         pt_tm_msg->ui4_table));
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_prc_rutil_msg
 *
 * Description: This function mainly handles messages come from Record Utility
 *
 * Inputs:  
 *       h_stream:    stream handle
 *       pt_strm_obj: stream object
 *       pt_rutil_msg:Record Utility message
 *         
 * Outputs: NULL
 *
 * Returns: 
 *        SMR_OK: Succeed
 *        Others: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sm_rcrd_prc_rutil_msg(HANDLE_T                  h_stream,
                                    SM_RCRD_STREAM_T*         pt_rs_obj,
                                    SM_RCRD_STRM_MSG_UTIL_T*  pt_rutil_msg)
{
    INT32                      i4_ret;
    REC_UTIL_COND_T            e_cond;

    if (NULL == pt_rs_obj)
    {
        SM_RSH_DBG_ERR(("_sm_rcrd_prc_rutil_msg:pt_rs_obj is NULL \r\n"));
        return SMR_INV_ARG;
    }
    
    /*Get record utility state*/
    i4_ret = rec_util_get_cond(pt_rs_obj->t_rutil_info.h_record_util,
                               &e_cond);
    if (SMR_OK == i4_ret)
    {
        if (REC_UTIL_COND_READY == e_cond)
        {
            sm_state_on_event(&pt_rs_obj->t_state,
                              SM_EVN_RSH_RUTI_READY,
                              (VOID*)pt_rs_obj,
                              NULL,
                              NULL);
            pt_rs_obj->t_rutil_info.e_rutil_cond = REC_UTIL_COND_READY;
        }
        else
        {
            sm_state_on_event(&pt_rs_obj->t_state,
                              SM_EVN_RSH_RUTI_TRANSITION,
                              (VOID*)pt_rs_obj,
                              NULL,
                              NULL);
            pt_rs_obj->t_rutil_info.e_rutil_cond = REC_UTIL_COND_TRANSITION;
        }
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_prc_scdb_msg
 *
 * Description: This function mainly handles messages comes from SCDB
 *
 * Inputs:  
 *       h_stream:    stream handle
 *       pt_strm_obj: stream object
 *       pt_scdb_msg: SCDB message
 *         
 * Outputs: 
 *       NULL
 *
 * Returns: 
 *        SMR_OK: Succeed
 *        Others: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-12 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sm_rcrd_prc_scdb_msg(HANDLE_T                 h_stream,
                                   SM_RCRD_STREAM_T*        pt_rs_obj,
                                   SM_RCRD_STRM_MSG_SCDB_T* pt_scdb_msg)
{
    /*INT32          i4_ret;*/
    SCDB_COND_T    e_scdb_cond;
    UINT32         ui4_reason;
    /*SCDB_COMMON_T  t_scdb_cmmn_old;*/
    /*SCDB_REC_T     t_scdb_rec;*/

    /*Init*/
    e_scdb_cond = pt_scdb_msg->e_scdb_cond;
    ui4_reason = pt_scdb_msg->ui4_reason;
    
    /* keep using old data if updating*/
    if (SCDB_UPDATING == e_scdb_cond)
    {
        return SMR_OK;
    }
    else if (SCDB_UPDATED == e_scdb_cond)
    {
        if (ui4_reason & SCDB_RECORD_ADD)
        {
            /* do nothing */
        }
        if (ui4_reason & SCDB_RECORD_DEL)
        {
            #if 0
            /* close if it is me */
            i4_ret = x_scdb_get_rec(pt_rs_obj->t_base.h_scdb, 
                                    &(pt_rs_obj->t_rcrd_strm_comp_id),
                                    &t_scdb_rec);
            if (( i4_ret != SCDBR_OK) || (SCDBR_REC_NOT_FOUND == i4_ret))
            {
                sm_state_on_event(&pt_rs_obj->t_state,
                                  SM_EVN_RSH_SCDB_RECORD_DEL,
                                  (VOID*)pt_rs_obj,
                                  NULL,
                                  NULL);
                
            }
            #endif
        }
        if (ui4_reason & SCDB_RECORD_MOD)
        {
            /* close if PMT/PAT  what to do? */
        }
        if (ui4_reason & SCDB_COMMON_MOD)
        {
            /* do nothing, left it to be processed by stream handler */
        }
        if ( ui4_reason & SCDB_REASON_UNK )
        {
            /* do nothing */
        }
    }
    else if (SCDB_CLOSED == e_scdb_cond)
    {
        pt_rs_obj->t_base.h_scdb = NULL_HANDLE;
        sm_state_on_event(&pt_rs_obj->t_state,
                          SM_EVN_RSH_SCDB_CLOSED,
                          (VOID*)pt_rs_obj,
                          NULL,
                          NULL);
        return SMR_OK;
    }
    else
    {
        /* improper SCDB state */
        SM_RSH_DBG_ERR(("_sm_rcrd_prc_scdb_msg:Improper SCDB state!\r\n"));
        SM_ASSERT(0);
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_prc_do_event_msg
 *
 * Description: This function mainly handles events which are generated in internal  
 *              module when receives some external messages e.g. from RM/PM/Record 
 *              Utility/SCDB etc.
 *                
 * Inputs:  
 *       h_stream:       stream handle
 *       pt_rs_obj:      stream object
 *       pt_do_event_msg:event message
 *         
 * Outputs: 
 *       NULL
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
static VOID _sm_rcrd_prc_do_event_msg(HANDLE_T                           h_stream,
                                      SM_RCRD_STREAM_T*                  pt_rs_obj,
                                      const SM_RCRD_STRM_MSG_DO_EVENT_T* pt_do_event_msg)
{
   sm_state_on_event(&pt_rs_obj->t_state,
                     pt_do_event_msg->ui4_event,
                     (VOID*)pt_rs_obj,
                     (VOID*)pt_do_event_msg->ui4_data,
                     NULL);
}

/*-----------------------------------------------------------------------------
 * Name:  _sm_rs_exec_cmd
 *
 * Description: This function parses commands comes from Application
 *
 * Inputs:  
 *       pt_cmd_sets :commands come from Application
 *
 * Outputs: 
 *       pt_rs_obj :stream object store application's config
 *
 * Returns: TRUE,  Success. 
 *          FALSE: Fail to free handle
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-16 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sm_rs_exec_cmd(SM_RCRD_STREAM_T*    pt_rs_obj, 
                             const SM_COMMAND_T*  pt_cmd_sets)
{
    SM_COMMAND_T* pt_iter      = (SM_COMMAND_T*)pt_cmd_sets;
    VOID*         pv_rcrd_info = NULL;
    
    if ((NULL == pt_rs_obj) || (NULL == pt_cmd_sets))
    {
        SM_RSH_DBG_ERR(("_sm_rs_exec_cmd:NULL arguments!\r\n"));
        return SMR_INV_ARG;
    }

    while (pt_iter->e_code != SM_CMD_CODE_END)
    {
        /*If it is not primary commnad*/
        if (!(pt_iter->e_code & SM_CMD_CODE_PRIMARY_BIT_FLAG))
        {
            SM_RSH_DBG_ERR(("_sm_rs_exec_cmd:invalid command!\r\n"));
            return SMR_INV_COMMAND;
        }
        
        switch (pt_iter->e_code)
        {
            case SM_RS_CMD_CODE_SET_RCRD_TYPE:
                pt_rs_obj->t_rcrd_type = (RSH_RCRD_TYPE_T)(pt_iter->u.ui1_data);
                break;
            case SM_RS_CMD_CODE_SET_PID:
                pt_rs_obj->t_pid = (MPEG_2_PID_T)(pt_iter->u.ui2_data);
                break;
            case SM_RS_CMD_CODE_SET_REC_UTIL_NAME:
                if (x_strlen(pt_iter->u.ps_text) >= SM_RCRD_UTIL_NAME_LEN)
                {
                    return SMR_INV_COMMAND;    
                }
                x_strcpy(pt_rs_obj->t_pmt_info.ai1_name_rec_util, pt_iter->u.ps_text);
                
                break;
            case SM_RS_CMD_CODE_SET_RCRD_INFO:
                pv_rcrd_info = pt_iter->u.pv_data;
                #if 0
                x_memcpy((VOID*)(&(pt_rs_obj->t_sect_info)), 
                         pt_iter->u.pv_data, 
                         sizeof(RSH_RCRD_INFO_SECT_T));
                #endif
                break;
            case SM_RS_CMD_CODE_SET_PRG_NUM:
                pt_rs_obj->ui2_program_num = pt_iter->u.ui2_data;
                break;
            case SM_RS_CMD_CODE_SET_STRM_COMP_ID:
                x_memcpy((VOID*)(&(pt_rs_obj->t_rcrd_strm_comp_id)), 
                         pt_iter->u.pv_data, 
                         sizeof(STREAM_COMP_ID_T));
                break;
            default:
                /**/
                return SMR_INV_COMMAND;
        }
        
        pt_iter = & pt_iter[1];
    }

    if (pv_rcrd_info != NULL)
    {
        if (RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
        {
            x_memcpy((VOID*)(&(pt_rs_obj->t_sect_info)), pv_rcrd_info, sizeof(RSH_RCRD_INFO_SECT_T));
        }
        else if (RSH_RCRD_TYPE_ES == pt_rs_obj->t_rcrd_type)
        {
            x_memcpy((VOID*)(&(pt_rs_obj->t_es_rcrd_info)), pv_rcrd_info, sizeof(RSH_RCRD_INFO_ES_T));
        }
        else
        {
        }
    }
    
    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_free_fct
 *
 * Description: This function frees record stream object resource
 *
 * Inputs:  
 *       h_handle :stream handle
 *       e_type :  stream handle type
 *       pv_obj :  stream object pointer
 *       h_handle :stream object tag
 *       b_req_handle :free is allowed or not flag
 *
 * Outputs: NULL  
 *
 * Returns: TRUE,  Success. 
 *          FALSE: Fail to free handle
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-06 : initial
 ------------------------------------------------------------------------------*/
static BOOL _sm_rcrd_free_fct(HANDLE_T       h_handle,
                              HANDLE_TYPE_T  e_type,
                              VOID*          pv_obj,
                              VOID*          pv_tag,
                              BOOL           b_req_handle)
{
    /*INT32                i4_ret;*/
    SM_RCRD_STREAM_T*    pt_rs_obj;

    if (TRUE ==  b_req_handle)
    {
        return FALSE;
    }
    pt_rs_obj = (SM_RCRD_STREAM_T*)pv_obj;
    
    if ((NULL == pt_rs_obj)
        ||(NULL == pv_tag)
        || (e_type != SM_HT_STREAM))
    {
        SM_RSH_DBG_ERR(("_sm_rcrd_free_fct:invalid parameters!\r\n"));
        return FALSE;    
    }

    SM_LOCK(pt_rs_obj->t_base.h_lock)
        
    if (pt_rs_obj->t_state.pv_state_nfy_tag != NULL)
    {
        x_mem_free(pt_rs_obj->t_state.pv_state_nfy_tag);
        pt_rs_obj->t_state.pv_state_nfy_tag = NULL;
    }
    
    if (pt_rs_obj->t_pmt_info.at_pids_es != NULL)
    {
        x_mem_free(pt_rs_obj->t_pmt_info.at_pids_es);
        pt_rs_obj->t_pmt_info.at_pids_es = NULL;
    }
    
    if (pt_rs_obj->t_pmt_info.at_scdb_recs != NULL)
    {
        x_mem_free(pt_rs_obj->t_pmt_info.at_scdb_recs);
        pt_rs_obj->t_pmt_info.at_scdb_recs = NULL;
    }
    
    SM_UNLOCK(pt_rs_obj->t_base.h_lock)
        
    /*destroy stream object mutex*/
    if (pt_rs_obj->t_base.h_lock != NULL_HANDLE)
    {
        x_sema_delete(pt_rs_obj->t_base.h_lock);
        pt_rs_obj->t_base.h_lock = NULL_HANDLE;
    }
    
    /*free stream object*/
    x_mem_free(pt_rs_obj);
    pt_rs_obj = NULL;
    

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_hdlr_open
 *
 * Description: This function opens record stream. 
 *
 * Inputs:  
 *       pt_stream_base: stream common info
 *       pt_cmd_sets:    application commands
 *       b_auto_play:    if should play automatically
 *       pf_sm_nfy:      stream notify function
 *       pv_sm_nfy_tag:  stream notify tag
 *
 * Outputs: 
 *       ph_stream: record stream object handle for application and sm
 *
 * Returns: SMR_OK,     Success. 
 *          Others:     Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-16 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_hdlr_open(const SM_STREAM_T*  pt_stream_base,
                             const SM_COMMAND_T* pt_cmd_sets,
                             BOOL                b_auto_play,
                             x_stream_nfy_fct    pf_sm_nfy,
                             VOID*               pv_sm_nfy_tag,
                             HANDLE_T*           ph_stream)
{
    INT32                       i4_ret;
    SM_SESSION_T*               pt_sess_obj;
    
    SM_RCRD_STREAM_T*           pt_rs_obj;
    SM_RCRD_STRM_STATE_TAG_T*   pt_state_tag;


    /*Parameters Check*/
    if ((NULL == pt_stream_base) 
        || (NULL == ph_stream) 
        || (NULL == pf_sm_nfy))
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_open:pt_stream_base is NULL\r\n"));
        return SMR_INV_ARG;
    }

    /*Init */
    i4_ret       = SMR_OK;
    pt_state_tag = NULL;
    pt_rs_obj    = NULL;

    /*Get session object*/
    pt_sess_obj = (SM_SESSION_T*)sm_get_sess_obj(pt_stream_base->h_sess);
    if ( NULL == pt_sess_obj )
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_open:pt_sess_obj is NULL\r\n"));
        return SMR_SESS_NOT_EXIST;
    }
    
    /*Do.*/
    do 
    {
        /* 1. Create stream object */
        pt_rs_obj = (SM_RCRD_STREAM_T*)x_mem_alloc(sizeof(SM_RCRD_STREAM_T));
        if (NULL == pt_rs_obj)
        {
            i4_ret = SMR_INSUFFICIENT_MEMORY;    
            SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_open:pt_rs_obj is NULL\r\n"));
            break;
        }
        x_memset(pt_rs_obj, 0, sizeof(SM_RCRD_STREAM_T));
        
        /*2.Init Stream Object*/
        
        /*2.1 init*/
        pt_rs_obj->t_base                  = *pt_stream_base;
        pt_rs_obj->t_base.h_pm_comp        = NULL_HANDLE;
        
        pt_rs_obj->pf_sess_hdlr_sm_nfy     = pf_sm_nfy;
        pt_rs_obj->pv_sess_hdlr_sm_nfy_tag = pv_sm_nfy_tag;

        
        pt_rs_obj->t_pvr_descr             = t_sm_drv_comp_initor;
        /*x_strcpy(pt_rs_obj->t_pvr_descr.t_info.s_logical_name,
                  pt_sess_obj->s_dest_grp_name);*/
        pt_rs_obj->t_pvr_descr.t_info.e_type  = DRVT_PVR_STREAM;
        
        pt_rs_obj->t_rutil_info.h_record_util = NULL_HANDLE;
        pt_rs_obj->t_rutil_info.e_rutil_cond  = REC_UTIL_COND_TRANSITION;
            
        pt_rs_obj->ui4_lastest_pm_event    = SM_EVN_IGNORE;
        pt_rs_obj->ui4_lastest_pvr_event   = SM_EVN_IGNORE;
        pt_rs_obj->ui4_lastest_rutil_event = SM_EVN_IGNORE;

        /*2.2 Create stream object mutex */
        pt_rs_obj->t_base.h_lock = NULL_HANDLE;
        i4_ret = x_sema_create(&pt_rs_obj->t_base.h_lock,
                               X_SEMA_TYPE_MUTEX,
                               X_SEMA_STATE_UNLOCK);
        if ((i4_ret != OSR_OK)
            || (NULL_HANDLE == pt_rs_obj->t_base.h_lock))
        {
            i4_ret = SMR_INTERNAL_ERROR;   
            SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_open:fail to create strm obj sema(%d)\r\n", 
                            i4_ret));
            break;
        }

        /* 3. Create stream handle */
        i4_ret = handle_alloc(SM_HT_STREAM,
                              pt_rs_obj,
                              (VOID*)pv_sm_nfy_tag,
                              &_sm_rcrd_free_fct,
                              ph_stream);
        if ((i4_ret != HR_OK) || (NULL_HANDLE == *ph_stream))
        {
            i4_ret = SMR_OUT_OF_HANDLES;   
            SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_open:fail to alloc strm handle(%d)\r\n", 
                            i4_ret));
            break;
            
        }
        pt_rs_obj->t_base.h_stream= *ph_stream;

        /*2.3 parse commands come from Application*/
        if (pt_cmd_sets != NULL)
        {
            i4_ret = _sm_rs_exec_cmd(pt_rs_obj, pt_cmd_sets);
            if (i4_ret != SMR_OK)
            {
                i4_ret = SMR_INTERNAL_ERROR;   
                SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_open:fail to parse cmd(%d)\r\n", 
                                i4_ret));
                break;
            }
        }

        /*4.Init State Machine */
        pt_state_tag = (SM_RCRD_STRM_STATE_TAG_T*)x_mem_alloc(sizeof(SM_RCRD_STRM_STATE_TAG_T));
        if (NULL == pt_state_tag)
        {
            i4_ret = SMR_INSUFFICIENT_MEMORY; 
            SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_open:pt_state_tag is NULL\r\n"));
            break;
        }

        pt_state_tag->h_stream    = *ph_stream;
        pt_state_tag->pt_strm_obj = pt_rs_obj;
        i4_ret = sm_state_init(at_rcrd_strm_state_cond_lst,
                               at_rcrd_strm_state_descr_lst,
                               at_rcrd_strm_allowed_pending_event_lst,
                               sm_rcrd_strm_state_notify_app,
                               (VOID*)pt_state_tag,
                               &pt_rs_obj->t_state );
        if (i4_ret != SMR_OK)
        {
            i4_ret = SMR_INTERNAL_ERROR;   
            SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_open:fail to init state machine(%d)\r\n", 
                            i4_ret));
            break;   
        }


        /*4.Send out App event to start record session state machine to do openning*/
        SM_LOCK(pt_rs_obj->t_base.h_lock)
        sm_rs_send_msg_do_event(SM_EVN_RSH_APP_OPEN_REQ, *ph_stream, 0);
        
        if (TRUE == b_auto_play)
        {
            sm_rs_send_msg_do_event(SM_EVN_RSH_APP_PLAY_REQ, *ph_stream,  0);
        }
        SM_UNLOCK(pt_rs_obj->t_base.h_lock)
        
        /*Normal*/
        SM_RSH_DBG_INFO(("sm_rcrd_strm_hdlr_open:open stream and get to do openning...!\r\n"));
        return SMR_OK;
        
    } while(0);


    if (i4_ret != SMR_OK)
    {
        /*free state tag */
        if (pt_rs_obj != NULL)
        {
            /*destroy stream object mutex*/
            if (pt_rs_obj->t_base.h_lock != NULL_HANDLE)
            {
                x_sema_delete(pt_rs_obj->t_base.h_lock);
                pt_rs_obj->t_base.h_lock = NULL_HANDLE;
            }
            
            /*free session object*/
            x_mem_free(pt_rs_obj);
            pt_rs_obj = NULL;
        }
    
        /*free stream object*/
        if (pt_state_tag != NULL)
        {
            x_mem_free(pt_state_tag);
            pt_state_tag = NULL;
        }
    }
    
    /*Abnormal*/
    SM_RSH_DBG_INFO(("sm_rcrd_strm_hdlr_open:Open failed!\r\n"));
    return i4_ret;
    
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_hdlr_close
 *
 * Description: This function closes a stream. 
 *
 * Inputs:  
 *       h_handle: stream handle
 *
 * Outputs: 
 *       start closing
 *
 * Returns: SMR_OK,     Success. 
 *          Others:     Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-16 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_hdlr_close(HANDLE_T h_stream)
{
    SM_RCRD_STREAM_T*      pt_rs_obj;
    
    pt_rs_obj = (SM_RCRD_STREAM_T*)sm_get_strm_obj(h_stream);
    if (NULL == pt_rs_obj)
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_close:NULL argument\r\n"));
        return SMR_INV_HANDLE;
    }

    sm_rs_send_msg_do_event(SM_EVN_RSH_APP_CLOSE_REQ, h_stream, 0);
    SM_RSH_DBG_INFO(("sm_rcrd_strm_hdlr_close:close stream and get to closing...!\r\n"));

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_hdlr_set
 *
 * Description: This function is a entry for setting something to a stream 
 *
 * Inputs:  
 *       h_handle:   stream handle
 *       e_set_type: set type defined in u_sm.h
 *       pv_set_inf: set info
 *       z_set_info_len: set info length
 *
 * Outputs: 
 *       Set something to SM or driver
 *
 * Returns: SMR_OK,     Success. 
 *          Others:     Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-16 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_hdlr_set(HANDLE_T       h_stream,
                            SM_SET_TYPE_T  e_set_type,
                            VOID*          pv_set_info,
                            SIZE_T         z_set_info_len)
{
    INT32                         i4_ret;
    SM_RCRD_STREAM_T*             pt_rs_obj;
    SM_RSH_PMT_ES_LIST_INFO_T*    pt_es_list; 
    BOOL                          b_null_pmt = FALSE;

    pt_rs_obj = (SM_RCRD_STREAM_T*)sm_get_strm_obj(h_stream);
    if (NULL == pt_rs_obj)
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_set:pt_rs_obj is null\r\n"));
        return SMR_INV_HANDLE;
    }

    i4_ret = SMR_OK;

    if ((pt_rs_obj->t_base.e_condition != SM_COND_IGNORE)
         && (pt_rs_obj->t_base.e_condition != SM_COND_OPENING)
         && (pt_rs_obj->t_base.e_condition != SM_COND_OPENED)
         && (pt_rs_obj->t_base.e_condition != SM_COND_STARTING)
         && (pt_rs_obj->t_base.e_condition != SM_COND_STARTED)
         && (pt_rs_obj->t_base.e_condition != SM_COND_STOPPING))
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_set:not allow!\r\n"));
        return SMR_CAN_NOT_OP_IN_THIS_STATE;
    }
    
    switch (e_set_type)
    {
        case SM_STRM_SET_TYPE_PLAY:
            if (SM_COND_STARTED == pt_rs_obj->t_base.e_condition)
            {
                SM_RSH_DBG_INFO(("sm_rcrd_strm_hdlr_set:has been played!\r\n"));
                return SMR_ALREADY_PLAYED;
            }
            sm_rs_send_msg_do_event(SM_EVN_RSH_APP_PLAY_REQ, 
                                    h_stream,
                                    0);
            break;
            
        case SM_STRM_SET_TYPE_STOP:
            if (SM_COND_OPENED == pt_rs_obj->t_base.e_condition)
            {
                SM_RSH_DBG_INFO(("sm_rcrd_strm_hdlr_set:has been stopped!\r\n"));
                return SMR_ALREADY_STOPPED;
            }
            sm_rs_send_msg_do_event(SM_EVN_RSH_APP_STOP_REQ, 
                                    h_stream,
                                    0);
            break;
            
        case SM_SESS_SET_TYPE_PLAY:
            if (SM_COND_STARTED == pt_rs_obj->t_base.e_condition)
            {
                SM_RSH_DBG_INFO(("sm_rcrd_strm_hdlr_set:has been played!\r\n"));
                return SMR_ALREADY_PLAYED;
            }
            sm_rs_send_msg_do_event(SM_EVN_SESS_RCRD_APP_PLAY_REQ, 
                                    h_stream,
                                    0);
            break;
            
        case SM_SESS_SET_TYPE_STOP:
            if (SM_COND_OPENED == pt_rs_obj->t_base.e_condition)
            {
                SM_RSH_DBG_INFO(("sm_rcrd_strm_hdlr_set:has been stopped!\r\n"));
                return SMR_ALREADY_STOPPED;
            }
            sm_rs_send_msg_do_event(SM_EVN_SESS_RCRD_APP_STOP_REQ, 
                                    h_stream,
                                    0);
            break;
            
        case SM_RSH_SET_TYPE_PMT_ES_LIST_INFO:
            if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
                && (RSH_TBL_GRP_PSI_PMT == pt_rs_obj->t_sect_info.e_tbl_grp))
            {
                pt_es_list = (SM_RSH_PMT_ES_LIST_INFO_T*)pv_set_info;
                if (NULL == pt_es_list)
                {
                    SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_set:NULL ES list!\r\n"));
                    return SMR_INV_ARG;
                }
                
                if (pt_es_list->ui2_num_es != 0)
                {
                    pt_rs_obj->t_pmt_info.ui2_num_es = pt_es_list->ui2_num_es;
                    
                    if (pt_rs_obj->t_pmt_info.at_pids_es != NULL)
                    {
                        x_mem_free(pt_rs_obj->t_pmt_info.at_pids_es);
                        pt_rs_obj->t_pmt_info.at_pids_es = NULL;
                    }
                    
                    pt_rs_obj->t_pmt_info.at_pids_es = 
                         x_mem_alloc(sizeof(MPEG_2_PID_T) * pt_es_list->ui2_num_es);
                    if (NULL == pt_rs_obj->t_pmt_info.at_pids_es)
                    {   
                        SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_set:fail to alloc at_pids_es mem!\r\n"));
                        return SMR_INSUFFICIENT_MEMORY;
                    }
                    x_memcpy(pt_rs_obj->t_pmt_info.at_pids_es,
                             pt_es_list->at_pids_es,
                             (sizeof(MPEG_2_PID_T) * pt_es_list->ui2_num_es));
                }
                
                if ((pt_es_list->ui4_flags & SM_RSH_PMT_ES_LIST_INFO_FLAG_AVAIL_SCDB_INFO) != 0)
                {
                    pt_rs_obj->t_pmt_info.ui2_num_recs = pt_es_list->ui2_num_recs;
                    
                    if (pt_rs_obj->t_pmt_info.at_scdb_recs != NULL)
                    {
                        x_mem_free(pt_rs_obj->t_pmt_info.at_scdb_recs);
                        pt_rs_obj->t_pmt_info.at_scdb_recs = NULL;
                    }
                    
                    pt_rs_obj->t_pmt_info.at_scdb_recs = (SCDB_REC_T*)
                         x_mem_alloc(sizeof(SCDB_REC_T) * pt_es_list->ui2_num_recs);
                    if (NULL == pt_rs_obj->t_pmt_info.at_scdb_recs)
                    {
                        x_mem_free(pt_rs_obj->t_pmt_info.at_pids_es);
                        pt_rs_obj->t_pmt_info.at_pids_es = NULL;
                        SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_set:fail to alloc at_scdb_recs mem!\r\n"));
                        return SMR_INSUFFICIENT_MEMORY;
                    }
                    x_memcpy(pt_rs_obj->t_pmt_info.at_scdb_recs,
                             pt_es_list->at_scdb_recs,
                             (sizeof(SCDB_REC_T) * pt_es_list->ui2_num_recs));
                }

                /*should set PVR swap*/
                if ((SM_COND_STARTING == pt_rs_obj->t_base.e_condition)
                     || (SM_COND_STARTED == pt_rs_obj->t_base.e_condition))
                {
                    if ((pt_rs_obj->e_pmt_cond == TM_COND_AVAILABLE)
                         || (pt_rs_obj->e_pmt_cond == TM_COND_UPDATE))
                    {
                        if (0 != pt_rs_obj->t_pmt_info.ui2_num_es)
                        {
                            /*Generate new PMT*/
                            i4_ret = sm_rcrd_strm_swap_pmt(pt_rs_obj);
                            if (SMR_OK != i4_ret)
                            {
                                SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_set:fail(%d) "
                                                "to load PMT!\r\n", i4_ret));
                            }
                        }
                        else
                        {
                            b_null_pmt = TRUE;
                        }
                    }
                    else
                    {
                        b_null_pmt = TRUE;
                    }

                    if (b_null_pmt)
                    {
                        i4_ret = sm_rcrd_strm_pvr_set_null_pacekt(pt_rs_obj->t_base.h_stream,
                                                                  pt_rs_obj);
                        if (SMR_OK == i4_ret)
                        {
                            SM_RSH_DBG_INFO(("sm_rcrd_strm_hdlr_set:set PVR null pmt OK\r\n"));
                        }
                        else
                        {
                            SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_set:set PVR null pmt err=%d!\r\n", 
                                             i4_ret));
                            sm_rs_send_msg_do_event(SM_EVN_RSH_COMP_ERR, h_stream, 0);
                            return SMR_DRV_COMP_ERROR;
                        }
                    }
                }
            }    
            else
            {
                /*invalid command*/
            }
            break;
            
        case SM_RSH_SET_TYPE_EN_PIC_SEARCH:
            /*check if PVR is opened */
            if ((pt_rs_obj->t_pvr_descr.h_comp != NULL_HANDLE)
                 && (pt_rs_obj->t_pvr_descr.e_comp_cond != RM_COND_CLOSED))
            {
                i4_ret = sm_rcrd_strm_pvr_set(pt_rs_obj->t_base.h_stream, 
                                              PVR_SET_TYPE_EN_PIC_SEARCH, 
                                              pv_set_info, 
                                              z_set_info_len);
                if (i4_ret != RMR_OK)
                {
                    SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_set:set PVR_SET_TYPE_EN_PIC_SEARCH,"
                                    "ret=%d!\r\n", 
                                    i4_ret));
                    return SMR_DRV_COMP_ERROR;
                }
                else
                {
                    SM_RSH_DBG_INFO(("sm_rcrd_strm_hdlr_set:set stream(pid=%u)"
                                     "PVR_SET_TYPE_EN_PIC_SEARCH OK!!!\r\n",
                                     pt_rs_obj->t_pid));
                }
            }
            else
            {
                pt_rs_obj->b_en_pic_search = TRUE;
                pt_rs_obj->b_en_pic_search_value = (BOOL)(UINT32)pv_set_info;
                SM_RSH_DBG_INFO(("sm_rcrd_strm_hdlr_set:set stream(pid=%u)"
                                 "PVR_SET_TYPE_EN_PIC_SEARCH failed "
                                 "because PVR is not opened!!!\r\n",
                                 pt_rs_obj->t_pid));
                return SMR_OK;
            }
            break;
            
        case SM_RSH_SET_TYPE_ES_ENCRYPTION:
            if (z_set_info_len != sizeof(MM_CRYPT_INFO_T))
            {
                SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_set:error encryption length(%d)!\r\n", 
                                z_set_info_len));
                return SMR_INV_ARG;
            }
            /*check if PVR is opened */
            if ((pt_rs_obj->t_pvr_descr.h_comp != NULL_HANDLE)
                 && (pt_rs_obj->t_pvr_descr.e_comp_cond != RM_COND_CLOSED))
            {
                i4_ret = sm_rcrd_strm_pvr_set(pt_rs_obj->t_base.h_stream, 
                                              PVR_SET_TYPE_ENCRYPTION, 
                                              pv_set_info, 
                                              z_set_info_len);
                if (i4_ret != SMR_OK)
                {
                    SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_set:set PVR_SET_TYPE_ENCRYPTION,"
                                    "ret=%d!\r\n", 
                                    i4_ret));
                    return SMR_DRV_COMP_ERROR;
                }
                else
                {
                    SM_RSH_DBG_INFO(("sm_rcrd_strm_hdlr_set:set stream(pid=%u)"
                                     "PVR_SET_TYPE_ENCRYPTION OK!!!\r\n",
                                     pt_rs_obj->t_pid));
                }
            }
            else
            {
                x_memcpy(&(pt_rs_obj->t_sess_key), 
                         pv_set_info, 
                         sizeof(MM_CRYPT_INFO_T));
                pt_rs_obj->b_en_encryption = TRUE;
                SM_RSH_DBG_INFO(("sm_rcrd_strm_hdlr_set:set stream(pid=%u)"
                                 "PVR_SET_TYPE_ENCRYPTION failed "
                                 "because PVR is not opened!!!\r\n",
                                 pt_rs_obj->t_pid));
                return SMR_OK;
            }
            break;
            
        case SM_RSH_SET_TYPE_CTRL_OUTPUT:
            if (z_set_info_len != sizeof(BOOL))
            {
                SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_set:error PVR_SET_TYPE_CTRL_OUTPUT length(%d)!\r\n", 
                                z_set_info_len));
                return SMR_INV_ARG;
            }
            
            /*check if PVR is opened */
            if ((pt_rs_obj->t_pvr_descr.h_comp != NULL_HANDLE)
                 && (pt_rs_obj->t_pvr_descr.e_comp_cond != RM_COND_CLOSED))
            {
                i4_ret = sm_rcrd_strm_pvr_set(h_stream, PVR_SET_TYPE_CTRL_OUTPUT, pv_set_info, z_set_info_len);
                if (i4_ret != SMR_OK)
                {
                    SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_set:stream(pid=%u) set PVR_SET_TYPE_CTRL_OUTPUT(%d),"
                                    "ret=%d!\r\n", pt_rs_obj->t_pid, (BOOL)(UINT32)pv_set_info, i4_ret));
                    return SMR_DRV_COMP_ERROR;
                }
                else
                {
                    SM_RSH_DBG_INFO(("sm_rcrd_strm_hdlr_set:stream(pid=%u) set"
                                     "PVR_SET_TYPE_CTRL_OUTPUT OK!!!\r\n", pt_rs_obj->t_pid));
                }
            }
            else
            {
                SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_set:stream(pid=%u) can not set PVR_SET_TYPE_CTRL_OUTPUT",
                                pt_rs_obj->t_pid));
                return SMR_DRV_COMP_ERROR;
            }
            break;
             
        default:
            SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_set:invalid set command(%d)!\r\n", 
                            e_set_type));
            return SMR_NOT_SUPPORT;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_hdlr_get
 *
 * Description: This function is for getting info of a stream 
 *
 * Inputs:  
 *       h_handle:       stream handle
 *       e_get_type:     get type defined in u_sm.h
 *       pv_get_inf:     get info
 *       z_get_info_len: get info length
 *
 * Outputs: 
 *       pv_get_inf: get info buffer
 *       z_get_info_len: get info buffer length
 *
 * Returns: SMR_OK,     Success. 
 *          Others:     Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-07 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_hdlr_get(HANDLE_T       h_stream,
                            SM_GET_TYPE_T  e_get_type,
                            VOID*          pv_get_info,       /* IN/OUT */
                            SIZE_T*        pz_get_info_len)
{
    INT32                      i4_ret;
    SM_RCRD_STREAM_T*          pt_rs_obj;

    pt_rs_obj = (SM_RCRD_STREAM_T*)sm_get_strm_obj(h_stream);
    if (NULL == pt_rs_obj)
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_get:pt_rs_obj is null\r\n"));
        return SMR_INV_HANDLE;
    }

    if(NULL == pv_get_info)
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_get:pv_get_info is null\r\n"));
        return SMR_INV_ARG;
    }
    
    i4_ret = SMR_OK;
    
    switch (e_get_type)
    {
        case SM_RSH_GET_TYPE_RCRD_TYPE:
            if (*pz_get_info_len < sizeof(RSH_RCRD_TYPE_T))
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            *(RSH_RCRD_TYPE_T*)pv_get_info = pt_rs_obj->t_rcrd_type;
            *pz_get_info_len = sizeof(RSH_RCRD_TYPE_T);
            break;  
            
        case SM_STRM_GET_TYPE_ID:
            /*i4_ret = SMR_NOT_SUPPORT;*/
            if (*pz_get_info_len < sizeof(STREAM_COMP_ID_T))
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            (*(STREAM_COMP_ID_T*)pv_get_info).e_type = ST_RCRD_STRM;
            (*(STREAM_COMP_ID_T*)pv_get_info).pv_stream_tag = NULL;
            *pz_get_info_len = sizeof(STREAM_COMP_ID_T);
            break;
            
        case SM_STRM_GET_TYPE_STATE:
            if (*pz_get_info_len < sizeof(SM_COND_T))
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            *((SM_COND_T*)pv_get_info) = pt_rs_obj->t_base.e_condition;
            *pz_get_info_len = sizeof(pt_rs_obj->t_base.e_condition);
            break;
            
        default:
            return SMR_INV_ARG;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_task
 *
 * Description: This function handles record stream message which may comes from
 *              Driver/RM/PM/Record Utilty/stream itself/etc. 
 *
 * Inputs:  
 *       pv_arg:   thread tag which is set in session registration
 *
 * Outputs: NULL
 *
 * Returns: SMR_OK,     Success. 
 *          Others:     Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-07 : initial
 ------------------------------------------------------------------------------*/
VOID sm_rcrd_strm_hdlr_task(VOID* pv_arg)
{
    INT32                      i4_ret;
    UINT16                     ui2_index;
    SM_RCRD_STREAM_T*          pt_rs_obj;
    SM_RCRD_STRM_HDLR_MSG_T    t_msg;
    SIZE_T                     z_msg_size;

    /*Init*/
    x_memset(&t_msg, 0, sizeof(SM_RCRD_STRM_HDLR_MSG_T));
    z_msg_size = sizeof(SM_RCRD_STRM_HDLR_MSG_T);
    
    /*Receive mesage*/
    i4_ret = x_msg_q_receive(&ui2_index,
                             &t_msg,
                             &z_msg_size,
                             &gh_rcrd_strm_msgq,
                             (UINT16)1,
                             X_MSGQ_OPTION_NOWAIT);
    if (OSR_NO_MSG == i4_ret)
    {
        return;    
    }
    
    if (i4_ret != OSR_OK)
    {
        SM_ABORT(SM_DBG_ABRT_CANNOT_RECV_FROM_MSG_Q);
    }
    
    pt_rs_obj = (SM_RCRD_STREAM_T*)sm_get_strm_obj(t_msg.h_stream);
    if (NULL == pt_rs_obj)
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_task:WARNING, receive msg(%d) for a invalid "
                        "stream handle(0x%8X)\r\n",
                        (UINT32)t_msg.e_msg_type, 
                        (UINT32)t_msg.h_stream));
        return;
    }
    /*Return code NULL*/
    SM_LOCK_RETURN_ON_FAIL(pt_rs_obj->t_base.h_lock,)
    
    switch(t_msg.e_msg_type)
    {
        case SM_RCRD_STRM_MSG_TYPE_PVR:
            _sm_rcrd_prc_pvr_msg(t_msg.h_stream,
                                 pt_rs_obj,
                                 &(t_msg.u.t_pvr_msg));
            break;

        case SM_RCRD_STRM_MSG_TYPE_RUTIL:
            _sm_rcrd_prc_rutil_msg(t_msg.h_stream,
                                   pt_rs_obj,
                                   &(t_msg.u.t_rutil_msg));
            break;
        case SM_RCRD_STRM_MSG_TYPE_TM:
            _sm_rcrd_prc_tm_msg(t_msg.h_stream,
                                pt_rs_obj,
                                &(t_msg.u.t_tm_msg));
            break;
        
        case SM_RCRD_STRM_MSG_TYPE_SCDB:
            _sm_rcrd_prc_scdb_msg(t_msg.h_stream,
                                  pt_rs_obj,
                                  &(t_msg.u.t_rcrd_scdb_msg));
            break;

        case SM_RCRD_STRM_MSG_TYPE_DO_EVENT:
            _sm_rcrd_prc_do_event_msg(t_msg.h_stream,
                                      pt_rs_obj,
                                      &(t_msg.u.t_rcrd_event_msg));
            break;
            
        case SM_RCRD_STRM_MSG_TYPE_PM:
            _sm_rcrd_prc_pm_msg(t_msg.h_stream,
                                pt_rs_obj,
                                &(t_msg.u.t_rcrd_pm_msg));
            break;
        case SM_RCRD_STRM_MSG_TYPE_CA:
            _sm_rcrd_prc_ca_msg(t_msg.h_stream,
                                pt_rs_obj,
                                &(t_msg.u.t_msg_ca));
            break;
            
        case SM_RCRD_STRM_MSG_TYPE_CP:
            _sm_rcrd_prc_cp_msg(t_msg.h_stream,
                                pt_rs_obj,
                                &(t_msg.u.t_msg_cci));
            break;
        default:
            SM_RSH_DBG_ERR(("sm_rcrd_strm_hdlr_task: undefined stream message(%u)\r\n",
                            (UINT32)(t_msg.e_msg_type)));
            SM_ABORT(SM_DBG_ABRT_INV_CASE_STMT);
    }
    
    SM_UNLOCK(pt_rs_obj->t_base.h_lock)

    return;
}


