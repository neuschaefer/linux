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
 * $RCSfile: sm_sess_rcrd.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/1 $
 * $SWAuthor: Lianming Lin $
 * $MD5HEX: d052cebe5bec682f2354c25548053f2d $
 *
 * Description: 
 *         This file implements the Stream Manager's record session.
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

#include "hash/hash_api.h"
#include "hash/_hash.h"
#include "conn_mngr/x_cm.h"

#include "strm_mngr/sess_rcrd_hdlr/sm_sess_rcrd.h"
#include "strm_mngr/sess_rcrd_hdlr/_sm_sess_rcrd_state.h"
#include "strm_mngr/sess_rcrd_hdlr/_sm_sess_rcrd_rutil.h"
#include "strm_mngr/sess_rcrd_hdlr/_sm_sess_rcrd_pvr.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define SM_SESS_RCRD_MSGQ_MAX_SIZE     256
#define SM_SESS_RCRD_MSGQ_SND_MAX_CNT  10

#define SM_RSS_MAX_STMT_LEN            512

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static CHAR        aui1_rss_dbg_stmt[SM_RSS_MAX_STMT_LEN];   /* Debug statement. */
static HANDLE_T    h_rss_output_sema;                        /* References the output semaphore. */
 
static HANDLE_T    gh_sess_rcrd_hdlr = NULL_HANDLE;
static HANDLE_T    gh_sess_rcrd_msgq = NULL_HANDLE;


/*Record Session State Machine*/
static SM_STATE_DESCR_T   at_rcrd_sess_state_descr_lst[] =
{
/*  ui4_state,  b_prcss_pending,  b_allow_pending,  pf_state_do,   pf_state_do_exception */
  {SM_SESS_COND_IGNORE,     FALSE,     FALSE,   NULL,                             sm_rcrd_sess_state_do_ignore_excpt},
  {SM_SESS_COND_OPENING,    FALSE,     TRUE,    sm_rcrd_sess_state_do_opening,    sm_rcrd_sess_state_do_opening_excpt},
  {SM_SESS_COND_OPENED,     TRUE,      FALSE,   sm_rcrd_sess_state_do_opened,     sm_rcrd_sess_state_do_opened_excpt},
  {SM_SESS_COND_STARTING,   FALSE,     TRUE,    sm_rcrd_sess_state_do_starting,   sm_rcrd_sess_state_do_starting_excpt},
  {SM_SESS_COND_STARTED,    TRUE,      FALSE,   sm_rcrd_sess_state_do_started,    sm_rcrd_sess_state_do_started_excpt},
  {SM_SESS_COND_STOPPING,   FALSE,     TRUE,    sm_rcrd_sess_state_do_stopping,   sm_rcrd_sess_state_do_stopping_excpt},
  {SM_SESS_COND_CLOSING,    FALSE,     TRUE,    sm_rcrd_sess_state_do_closing,    sm_rcrd_sess_state_do_closing_excpt},
  {SM_SESS_COND_CLOSED,     FALSE,     FALSE,   sm_rcrd_sess_state_do_closed,     sm_rcrd_sess_state_do_closed_excpt},
  {SM_SESS_COND_ERROR,      TRUE,      FALSE,   sm_rcrd_sess_state_do_error,      sm_rcrd_sess_state_do_error_excpt},
  SM_STATE_DESCR_NULL  
};

static UINT32     at_rcrd_sess_allowed_pending_event_lst[] =
{
    SM_EVN_SESS_RCRD_APP_PLAY_REQ,
    SM_EVN_SESS_RCRD_APP_STOP_REQ,
    SM_EVN_SESS_RCRD_APP_CLOSE_REQ,
    SM_EVN_SESS_RCRD_SCDB_CLOSED,
    SM_EVN_IGNORE
};

static SM_STATE_COND_T    at_rcrd_sess_state_cond_lst[] =
{
  /* ui4_curr_state,       ui4_event,                  ui4_next_state,          ui4_pending_event,          pf_event_do */
  /* ignore state conditions *************************************************/ 
  {SM_SESS_COND_IGNORE,   SM_EVN_SESS_RCRD_APP_OPEN_REQ,   SM_SESS_COND_OPENING,    SM_EVN_IGNORE,                  NULL},
  
  /* opening state conditions ************************************************/ 
  {SM_SESS_COND_OPENING,  SM_EVN_SESS_RCRD_ALL_OPENED,     SM_SESS_COND_OPENED,     SM_EVN_IGNORE,                  NULL},
  {SM_SESS_COND_OPENING,  SM_EVN_SESS_RCRD_APP_PLAY_REQ,   SM_SESS_COND_OPENING,    SM_EVN_SESS_RCRD_APP_PLAY_REQ,  NULL},
  {SM_SESS_COND_OPENING,  SM_EVN_SESS_RCRD_APP_CLOSE_REQ,  SM_SESS_COND_OPENING,    SM_EVN_SESS_RCRD_APP_CLOSE_REQ, NULL},
  {SM_SESS_COND_OPENING,  SM_EVN_SESS_RCRD_SCDB_CLOSED,    SM_SESS_COND_OPENING,    SM_EVN_SESS_RCRD_APP_CLOSE_REQ, NULL},
  {SM_SESS_COND_OPENING,  SM_EVN_SESS_RCRD_ALL_PLAYED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,                  NULL},
  {SM_SESS_COND_OPENING,  SM_EVN_SESS_RCRD_ALL_STOPPED,    SM_SESS_COND_ERROR,      SM_EVN_IGNORE,                  NULL},
  {SM_SESS_COND_OPENING,  SM_EVN_SESS_RCRD_ALL_CLOSED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,                  NULL},
  {SM_SESS_COND_OPENING,  SM_EVN_SESS_RCRD_COMP_ERR,       SM_SESS_COND_ERROR,      SM_EVN_IGNORE,                  NULL},
  {SM_SESS_COND_OPENING,  SM_EVN_SESS_RCRD_INTERNAL_ERR,   SM_SESS_COND_ERROR,      SM_EVN_IGNORE,                  NULL},
  
  /* opened state conditions *************************************************/ 
  {SM_SESS_COND_OPENED,   SM_EVN_SESS_RCRD_APP_PLAY_REQ,   SM_SESS_COND_STARTING,   SM_EVN_IGNORE,              NULL},
  {SM_SESS_COND_OPENED,   SM_EVN_SESS_RCRD_APP_CLOSE_REQ,  SM_SESS_COND_CLOSING,    SM_EVN_IGNORE,              NULL},
  {SM_SESS_COND_OPENED,   SM_EVN_SESS_RCRD_SCDB_CLOSED,    SM_SESS_COND_CLOSING,    SM_EVN_IGNORE,              NULL},
  {SM_SESS_COND_OPENED,   SM_EVN_SESS_RCRD_ALL_OPENED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL},
  {SM_SESS_COND_OPENED,   SM_EVN_SESS_RCRD_ALL_PLAYED,     SM_SESS_COND_STARTING,   SM_EVN_IGNORE,              NULL},
  {SM_SESS_COND_OPENED,   SM_EVN_SESS_RCRD_ALL_STOPPED,    SM_STATE_IGNORE,         SM_EVN_IGNORE,              NULL},
  {SM_SESS_COND_OPENED,   SM_EVN_SESS_RCRD_ALL_CLOSED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL},
  {SM_SESS_COND_OPENED,   SM_EVN_SESS_RCRD_COMP_ERR,       SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL},
  {SM_SESS_COND_OPENED,   SM_EVN_SESS_RCRD_INTERNAL_ERR,   SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL},

  /* starting state conditions ***********************************************/ 
  {SM_SESS_COND_STARTING, SM_EVN_SESS_RCRD_ALL_PLAYED,     SM_SESS_COND_STARTED,    SM_EVN_IGNORE,                   NULL},
  {SM_SESS_COND_STARTING, SM_EVN_SESS_RCRD_APP_STOP_REQ,   SM_SESS_COND_STARTING,   SM_EVN_SESS_RCRD_APP_STOP_REQ,   NULL},
  {SM_SESS_COND_STARTING, SM_EVN_SESS_RCRD_APP_CLOSE_REQ,  SM_SESS_COND_STARTING,   SM_EVN_SESS_RCRD_APP_CLOSE_REQ,  NULL},
  {SM_SESS_COND_STARTING, SM_EVN_SESS_RCRD_SCDB_CLOSED,    SM_SESS_COND_STARTING,   SM_EVN_SESS_RCRD_APP_CLOSE_REQ,  NULL},
  {SM_SESS_COND_STARTING, SM_EVN_SESS_RCRD_ALL_OPENED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,                   NULL},
  {SM_SESS_COND_STARTING, SM_EVN_SESS_RCRD_ALL_STOPPED,    SM_STATE_IGNORE,         SM_EVN_IGNORE,                   NULL},
  {SM_SESS_COND_STARTING, SM_EVN_SESS_RCRD_ALL_CLOSED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,                   NULL},
  {SM_SESS_COND_STARTING, SM_EVN_SESS_RCRD_COMP_ERR,       SM_SESS_COND_ERROR,      SM_EVN_IGNORE,                   NULL},
  {SM_SESS_COND_STARTING, SM_EVN_SESS_RCRD_INTERNAL_ERR,   SM_SESS_COND_ERROR,      SM_EVN_IGNORE,                   NULL},

  /* started state conditions ************************************************/ 
  {SM_SESS_COND_STARTED,  SM_EVN_SESS_RCRD_APP_STOP_REQ,      SM_SESS_COND_STOPPING,   SM_EVN_IGNORE,                  NULL},
  {SM_SESS_COND_STARTED,  SM_EVN_SESS_RCRD_APP_CLOSE_REQ,     SM_SESS_COND_STOPPING,   SM_EVN_SESS_RCRD_APP_CLOSE_REQ, NULL},
  {SM_SESS_COND_STARTED,  SM_EVN_SESS_RCRD_SCDB_CLOSED,       SM_SESS_COND_STOPPING,   SM_EVN_SESS_RCRD_SCDB_CLOSED,   NULL},
  {SM_SESS_COND_STARTED,  SM_EVN_SESS_RCRD_PVR_CTRL_OVERFLOW, SM_SESS_COND_STOPPING,   SM_EVN_SESS_RCRD_APP_PLAY_REQ,       NULL},
  {SM_SESS_COND_STARTED,  SM_EVN_SESS_RCRD_ALL_OPENED,        SM_SESS_COND_ERROR,      SM_EVN_IGNORE,                  NULL},
  {SM_SESS_COND_STARTED,  SM_EVN_SESS_RCRD_ALL_PLAYED,        SM_STATE_IGNORE,         SM_EVN_IGNORE,                  NULL},
  {SM_SESS_COND_STARTED,  SM_EVN_SESS_RCRD_ALL_STOPPED,       SM_SESS_COND_STOPPING,   SM_EVN_IGNORE,                  NULL},
  {SM_SESS_COND_STARTED,  SM_EVN_SESS_RCRD_ALL_CLOSED,        SM_SESS_COND_ERROR,      SM_EVN_IGNORE,                  NULL},
  {SM_SESS_COND_STARTED,  SM_EVN_SESS_RCRD_STRG_FULL,         SM_EVN_IGNORE,   SM_EVN_IGNORE,  NULL},
  {SM_SESS_COND_STARTED,  SM_EVN_SESS_RCRD_REACH_BOUND,       SM_EVN_IGNORE,   SM_EVN_IGNORE,  NULL},
  {SM_SESS_COND_STARTED,  SM_EVN_SESS_RCRD_COMP_ERR,          SM_SESS_COND_ERROR,      SM_EVN_IGNORE,                  NULL},
  {SM_SESS_COND_STARTED,  SM_EVN_SESS_RCRD_INTERNAL_ERR,      SM_SESS_COND_ERROR,      SM_EVN_IGNORE,                  NULL},

  /* stopping state conditions ***********************************************/ 
  {SM_SESS_COND_STOPPING, SM_EVN_SESS_RCRD_ALL_STOPPED,    SM_SESS_COND_OPENED,     SM_EVN_IGNORE,                   NULL},
  {SM_SESS_COND_STOPPING, SM_EVN_SESS_RCRD_APP_CLOSE_REQ,  SM_SESS_COND_STOPPING,   SM_EVN_SESS_RCRD_APP_CLOSE_REQ,  NULL},
  {SM_SESS_COND_STOPPING, SM_EVN_SESS_RCRD_SCDB_CLOSED,    SM_SESS_COND_STOPPING,   SM_EVN_SESS_RCRD_SCDB_CLOSED,    NULL},
  {SM_SESS_COND_STOPPING, SM_EVN_SESS_RCRD_APP_PLAY_REQ,   SM_SESS_COND_STOPPING,   SM_EVN_SESS_RCRD_APP_PLAY_REQ,   NULL},
  {SM_SESS_COND_STOPPING, SM_EVN_SESS_RCRD_ALL_OPENED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,                   NULL},
  {SM_SESS_COND_STOPPING, SM_EVN_SESS_RCRD_ALL_PLAYED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,                   NULL},
  {SM_SESS_COND_STOPPING, SM_EVN_SESS_RCRD_ALL_CLOSED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,                   NULL},
  {SM_SESS_COND_STOPPING, SM_EVN_SESS_RCRD_COMP_ERR,       SM_SESS_COND_ERROR,      SM_EVN_IGNORE,                   NULL},
  {SM_SESS_COND_STOPPING, SM_EVN_SESS_RCRD_INTERNAL_ERR,   SM_SESS_COND_ERROR,      SM_EVN_IGNORE,                   NULL},

  /* closing state conditions ************************************************/ 
  {SM_SESS_COND_CLOSING,  SM_EVN_SESS_RCRD_ALL_CLOSED,     SM_SESS_COND_CLOSED,     SM_EVN_IGNORE,              NULL},

  /* error state conditions **************************************************/ 
  {SM_SESS_COND_ERROR,    SM_EVN_SESS_RCRD_APP_CLOSE_REQ,  SM_SESS_COND_CLOSING,    SM_EVN_IGNORE,              NULL},
  {SM_SESS_COND_ERROR,    SM_EVN_SESS_RCRD_SCDB_CLOSED,    SM_SESS_COND_CLOSING,    SM_EVN_IGNORE,              NULL},
  {SM_SESS_COND_ERROR,    SM_EVN_SESS_RCRD_APP_PLAY_REQ,   SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL},
  {SM_SESS_COND_ERROR,    SM_EVN_SESS_RCRD_APP_STOP_REQ,   SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL},
  {SM_SESS_COND_ERROR,    SM_EVN_SESS_RCRD_ALL_OPENED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL},
  {SM_SESS_COND_ERROR,    SM_EVN_SESS_RCRD_ALL_PLAYED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL},
  {SM_SESS_COND_ERROR,    SM_EVN_SESS_RCRD_ALL_STOPPED,    SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL},
  {SM_SESS_COND_ERROR,    SM_EVN_SESS_RCRD_ALL_CLOSED,     SM_SESS_COND_CLOSING,    SM_EVN_IGNORE,              NULL},

  /* ANY state events ********************************************************/ 
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_PVR_OPENED,       SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_PVR_PLAYED,       SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_PVR_STOPPED,      SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_PVR_CLOSED,       SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_PVR_OVERFLOW,     SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_PVR_ERR,          SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},

  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_PM_COND_OPENED,   SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_PM_COND_PLAYED,   SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_PM_COND_STOPPED,  SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_PM_COND_CLOSED,   SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_PM_COND_ERR,      SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},

  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_RUTI_OPENED,      SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_RUTI_READY,       SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_RUTI_TRANSITION,  SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_RUTI_PLAYED,      SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_RUTI_STOPPED,     SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_RUTI_CLOSED,      SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_RUTI_REACH_BOUND, SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_RUTI_STRG_FULL,   SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_RUTI_ERR,         SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},

  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_STRM_ADD,         SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_STRM_DEL,         SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_STRM_OPENED,      SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_STRM_PLAYED,      SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_STRM_CLOSED,      SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_STRM_ALL_OPENED,  SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_STRM_ALL_PLAYED,  SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_STRM_ALL_CLOSED,  SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  {SM_STATE_IGNORE,       SM_EVN_SESS_RCRD_STRM_ERR,         SM_STATE_IGNORE,    SM_EVN_IGNORE,     sm_rcrd_sess_state_do_drv_events},
  
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
 * Name:  sm_sess_rcrd_dbg_ctrl_stmt_api
 *
 * Description: This function prints record session api debug info. 
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
INT32 sm_sess_rcrd_dbg_ctrl_stmt_api(const CHAR*  ps_format, ...)
{
    INT32           i4_return;
    VA_LIST         arg_list;


    i4_return = SMR_INV_ARG;
    if (ps_format != NULL)
    {
        i4_return = SMR_OK;
        
        VA_START(arg_list, ps_format);
        
        /* Wait for other debug statement to be finished. */
        x_sema_lock(h_rss_output_sema, X_SEMA_OPTION_WAIT);
        
        x_memset(aui1_rss_dbg_stmt, 0, SM_RSS_MAX_STMT_LEN);
        /* Create debug statement string and send it to output device. */
        x_vsnprintf(aui1_rss_dbg_stmt, SM_RSS_MAX_STMT_LEN - 1, ps_format, arg_list);
        SM_DBG_API(SM_DBG_MOD_RSS, 
                  ("{SM} API: [RSS] %s", aui1_rss_dbg_stmt));
        
        x_sema_unlock(h_rss_output_sema);
        
        VA_END(arg_list);
    }
    else
    {
        
    }

    return (i4_return);
}
/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_dbg_ctrl_stmt_err
 *
 * Description: This function prints record session error debug info. 
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
INT32 sm_sess_rcrd_dbg_ctrl_stmt_err(const CHAR*  ps_format, ...)
{
    INT32           i4_return;
    VA_LIST         arg_list;


    i4_return = SMR_INV_ARG;
    if (ps_format != NULL)
    {
        i4_return = SMR_OK;
        
        VA_START(arg_list, ps_format);
        
        /* Wait for other debug statement to be finished. */
        x_sema_lock(h_rss_output_sema, X_SEMA_OPTION_WAIT);
        
        x_memset(aui1_rss_dbg_stmt, 0, SM_RSS_MAX_STMT_LEN);
        /* Create debug statement string and send it to output device. */
        x_vsnprintf(aui1_rss_dbg_stmt, SM_RSS_MAX_STMT_LEN - 1, ps_format, arg_list);
        SM_DBG_ERR(SM_DBG_MOD_RSS, 
                  ("{SM} ERR: [RSS] %s", aui1_rss_dbg_stmt));
        
        x_sema_unlock(h_rss_output_sema);
        
        VA_END(arg_list);
    }
    else
    {
        
    }

    return (i4_return);
}
/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_dbg_ctrl_stmt_info
 *
 * Description: This function prints record session info debug info. 
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
INT32 sm_sess_rcrd_dbg_ctrl_stmt_info(const CHAR*  ps_format, ...)
{
    INT32           i4_return;
    VA_LIST         arg_list;


    i4_return = SMR_INV_ARG;
    if (ps_format != NULL)
    {
        i4_return = SMR_OK;
        
        VA_START(arg_list, ps_format);
        
        /* Wait for other debug statement to be finished. */
        x_sema_lock(h_rss_output_sema, X_SEMA_OPTION_WAIT);
        
        x_memset(aui1_rss_dbg_stmt, 0, SM_RSS_MAX_STMT_LEN);
        /* Create debug statement string and send it to output device. */
        x_vsnprintf(aui1_rss_dbg_stmt, SM_RSS_MAX_STMT_LEN - 1, ps_format, arg_list);
        SM_DBG_INFO(SM_DBG_MOD_RSS, 
                    ("{SM} INFO: [RSS] %s", aui1_rss_dbg_stmt));
        
        x_sema_unlock(h_rss_output_sema);
        
        VA_END(arg_list);
    }
    else
    {
        
    }

    return (i4_return);
}

/*-----------------------------------------------------------------------------
 * Name:  x_sm_sess_rcrd_init
 *
 * Description: This function inits record session handler module. 
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
INT32 x_sm_sess_rcrd_init(VOID)
{
    INT32                    i4_ret;
    SM_SESS_HDLR_FCT_TBL_T   t_sess_hdlr_fct_tbl;
    SM_HDLR_DESC_T           t_hdlr_desc;

    /* Create the required semaphores. */
    if (x_sema_create(&h_rss_output_sema, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK)
           != OSR_OK)
    {
        SM_ABORT(SM_DBG_ABRT_CANNOT_CREATE_SEMAPHORE);
    }
    
    t_sess_hdlr_fct_tbl.pf_open_sess = sm_sess_rcrd_open;
    t_sess_hdlr_fct_tbl.pf_open_stream = sm_sess_rcrd_strm_open;
    t_sess_hdlr_fct_tbl.pf_close = sm_sess_rcrd_close;
    t_sess_hdlr_fct_tbl.pf_set = sm_sess_rcrd_set;
    t_sess_hdlr_fct_tbl.pf_get = sm_sess_rcrd_get;
    t_sess_hdlr_fct_tbl.pf_strm_nfy = sm_sess_rcrd_strm_nfy;
    t_sess_hdlr_fct_tbl.pf_task = sm_sess_rcrd_task;
    
    x_strcpy(t_hdlr_desc.s_hdlr_name, SM_RCRD_SESS_HDLR_NAME);
    t_hdlr_desc.ui4_strm_type_mask = ST_MASK_RCRD_STRM;
    
    i4_ret = sm_reg_sess_handler(&t_hdlr_desc,
                                 &t_sess_hdlr_fct_tbl,
                                 FALSE,
                                 NULL,
                                 &gh_sess_rcrd_hdlr);
    if ((i4_ret != SMR_OK) || (NULL_HANDLE == gh_sess_rcrd_hdlr))
    {
        SM_RSS_DBG_ERR(("x_sm_sess_rcrd_init:reg rcrd_sm session fail(%d)\r\n", i4_ret));
        return SMR_INTERNAL_ERROR;
    }

    
    /* Create message queue for Record Session Task */
    if (NULL_HANDLE == gh_sess_rcrd_msgq)
    {
        i4_ret = x_msg_q_create(&gh_sess_rcrd_msgq,
                                SM_RCRD_SESS_HDLR_NAME,  
                                sizeof( SM_SESS_RCRD_MSG_T),
                                SM_SESS_RCRD_MSGQ_MAX_SIZE);
        if (i4_ret != OSR_OK)
        {
            SM_RSS_DBG_ERR(("x_sm_sess_rcrd_init:create rcrd_sm queue fail(%d)\r\n", i4_ret));
            return SMR_INTERNAL_ERROR;
        }
    }    
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_sess_send_msg
 *
 * Description: This function sends message to record handler's queque and triggers
 *              selector thread to call record session handler task callback to handle it.
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
 *   (1)2009-03-06 : initial
 ------------------------------------------------------------------------------*/
VOID sm_rcrd_sess_send_msg(const SM_SESS_RCRD_MSG_T* pt_msg)
{
    INT32       i4_ret = SMR_OK;
    UINT32      i4_i = 0;
    HANDLE_T    h_thread = NULL_HANDLE;

    /*Check parameters*/
    if (NULL == pt_msg)
    {
        SM_RSS_DBG_ERR(("sm_rcrd_sess_send_msg:NULL message!\r\n"));
        return;
    }

    /*Send message to record session handler and trigger selector thread to call
      handler's corresponding task callback to handle it */
    while (i4_i++ < SM_SESS_RCRD_MSGQ_SND_MAX_CNT)
    {
        i4_ret = x_msg_q_send(gh_sess_rcrd_msgq,
                              (VOID*)pt_msg,
                              sizeof(SM_SESS_RCRD_MSG_T ),
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
    
        i4_ret = sm_trigger_task(gh_sess_rcrd_hdlr, (VOID*)gh_sess_rcrd_hdlr);
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
 * Name:  sm_rcrd_sess_send_msg_do_event
 *
 * Description: This function sends event message to record handler's queque and triggers
 *              selector thread to call record handler's task callback to handle it.
 *
 * Inputs:  
 *       ui4_event :event
 *       h_sess:    session handle
 *       ui4_data:  e.g. additional data which come from RM or driver(connect/disconnect reason)
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
VOID sm_rcrd_sess_send_msg_do_event(UINT32      ui4_event,
                                    HANDLE_T    h_sess,
                                    UINT32      ui4_data)
{
    SM_SESS_RCRD_MSG_T  t_sess_msg;
    
    t_sess_msg.e_msg_type = SM_SESS_RCRD_MSG_TYPE_DO_EVENT;
    t_sess_msg.h_sess     = h_sess;
    t_sess_msg.u.t_rcrd_event_msg.ui4_event = ui4_event;
    t_sess_msg.u.t_rcrd_event_msg.ui4_data  = ui4_data;
    
    sm_rcrd_sess_send_msg(&t_sess_msg);

    return;
}

static VOID _rcrd_strm_ctrl_tbl_free_fct(VOID* pv_obj)
{
    /* nothing to do currently */
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  _sess_rcrd_free_fct
 *
 * Description: This function free record session object resource
 *
 * Inputs:  
 *       h_handle :session handle
 *       e_type :  session handle type
 *       pv_obj :  session object pointer
 *       h_handle :session object tag
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
static BOOL _sess_rcrd_free_fct(HANDLE_T       h_handle,
                                HANDLE_TYPE_T  e_type,
                                VOID*          pv_obj,
                                VOID*          pv_tag,
                                BOOL           b_req_handle)
{
    SM_SESS_RCRD_T*  pt_sess_obj;
    INT32            i4_ret;
    SIZE_T           z_size = 0;


    /*It's not allowed for application to call x_handle_free() to free the 
      session handle.*/
    if (TRUE == b_req_handle)
    {
        SM_RSS_DBG_ERR(("sess_rcrd_free_fct:prohibit free!\r\n"));
        return FALSE;    
    }
    
    pt_sess_obj = (SM_SESS_RCRD_T*)pv_obj;
    
    if ((NULL == pt_sess_obj)
        ||(NULL == pv_tag)
        || (e_type != SM_HT_SESSION))
    {
        SM_RSS_DBG_ERR(("sess_rcrd_free_fct:invalid parameters!\r\n"));
        return FALSE;    
    }

    
    SM_LOCK(pt_sess_obj->t_base.h_lock)

    /* Not allow to free the handle if PM service is not closed yet */
    if (pt_sess_obj->t_base.h_pm_service != NULL_HANDLE)
    {
        SM_UNLOCK(pt_sess_obj->t_base.h_lock)
        SM_RSS_DBG_ERR(("sess_rcrd_free_fct:PM is active!\r\n"));
        return FALSE;
    }
    
    /* Not allow to free the handle if there are still streams in the table */
    hash_lock(pt_sess_obj->t_base.h_strm_ctrl_tbl);
    i4_ret = hash_get_size(pt_sess_obj->t_base.h_strm_ctrl_tbl, &z_size);
    hash_unlock(pt_sess_obj->t_base.h_strm_ctrl_tbl);
    if ((i4_ret != SMR_OK) || (z_size != 0))
    {
        SM_UNLOCK( pt_sess_obj->t_base.h_lock)
        SM_RSS_DBG_ERR(("sess_rcrd_free_fct:Stream is active!\r\n"));
        return FALSE;
    }

    /*Destroy stream hash table*/
    if (pt_sess_obj->t_base.h_strm_ctrl_tbl)
    {
        hash_delete(pt_sess_obj->t_base.h_strm_ctrl_tbl, &_rcrd_strm_ctrl_tbl_free_fct);
        pt_sess_obj->t_base.h_strm_ctrl_tbl = NULL_HANDLE;
    }

    /*Free session state machine tag*/
    if (pt_sess_obj->t_state.pv_state_nfy_tag != NULL)
    {
        x_mem_free(pt_sess_obj->t_state.pv_state_nfy_tag);
        pt_sess_obj->t_state.pv_state_nfy_tag = NULL;
    }
    
    if (pt_sess_obj->t_sess_cfg.s_name_rec_util != NULL)
    {
        x_mem_free((VOID*)(pt_sess_obj->t_sess_cfg.s_name_rec_util));
        pt_sess_obj->t_sess_cfg.s_name_rec_util = NULL;
    }
    
    if (pt_sess_obj->t_rutil_info.pt_rec_util_write_req != NULL)
    {
        x_mem_free(pt_sess_obj->t_rutil_info.pt_rec_util_write_req);
        pt_sess_obj->t_rutil_info.pt_rec_util_write_req = NULL;
    }

    SM_UNLOCK(pt_sess_obj->t_base.h_lock)

    /*Destroy session object mutex*/
    i4_ret = x_sema_delete(pt_sess_obj->t_base.h_lock);
    if (i4_ret != OSR_OK)
    {
        SM_RSS_DBG_ERR(("sess_rcrd_free_fct: delete sema failed(%d)!\r\n", i4_ret));
        /*Need not return*/
    }
    pt_sess_obj->t_base.h_lock = NULL_HANDLE;

    /*Free session object itself*/
    x_mem_free(pt_sess_obj);
    
    SM_RSS_DBG_INFO(("sess_rcrd_free_fct: free session obj(hdlr=%u) ok!\r\n",
                     h_handle));
    
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name:  _sess_rcrd_add_stream
 *
 * Description: This function adds a stream to session stream ctrl hash table
 *
 * Inputs:  
 *       h_handle :    session handle
 *       pt_sess_obj : session object pointer
 *       h_stream :    stream handle
 *       
 * Outputs: NULL  
 *
 * Returns: TRUE,  Success. 
 *          FALSE: Fail to add
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-07 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sess_rcrd_add_stream(HANDLE_T        h_sess,
                                   SM_SESS_RCRD_T* pt_sess_obj,
                                   HANDLE_T        h_stream)
{
    INT32   i4_ret = SMR_OK;


    /*Check*/
    if (NULL == pt_sess_obj)

    {
        SM_RSS_DBG_ERR(("_sess_rcrd_add_stream:invalid arguments!\r\n",
                        i4_ret));
        return SMR_INV_ARG;
    }
    
    hash_lock(pt_sess_obj->t_base.h_strm_ctrl_tbl);
    
    i4_ret = hash_add_obj(pt_sess_obj->t_base.h_strm_ctrl_tbl,
                          (INT8*)&h_stream,
                          sizeof(h_stream),
                          (VOID*)h_stream,
                          NULL);
    
    hash_unlock(pt_sess_obj->t_base.h_strm_ctrl_tbl);
    
    if (i4_ret != HASH_OK)
    {
        SM_RSS_DBG_ERR(("_sess_rcrd_add_stream:add hash err=%d!\r\n",
                        i4_ret));
        return SMR_INTERNAL_ERROR;        
    }
    
    sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_STRM_ADD,
                                   h_sess,
                                   (UINT32)h_stream);
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _sess_rcrd_remove_stream
 *
 * Description: This function deletes a stream from session stream ctrl hash table
 *
 * Inputs:  
 *       h_handle :    session handle
 *       pt_sess_obj : session object pointer
 *       h_stream :    stream handle
 *       
 * Outputs: NULL  
 *
 * Returns: TRUE,  Success. 
 *          FALSE: Fail to add
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sess_rcrd_remove_stream(HANDLE_T        h_sess,
                                      SM_SESS_RCRD_T* pt_sess_obj,
                                      HANDLE_T        h_stream)
{
    INT32   i4_ret;
    
    hash_lock(pt_sess_obj->t_base.h_strm_ctrl_tbl);
    
    i4_ret = hash_remove_obj(pt_sess_obj->t_base.h_strm_ctrl_tbl,
                            (INT8*)&h_stream,
                             sizeof(h_stream),
                             NULL);
    
    hash_unlock(pt_sess_obj->t_base.h_strm_ctrl_tbl);
    
    if (i4_ret != HASH_OK)
    {
        SM_RSS_DBG_ERR(("_sess_rcrd_remove_stream:rmv hash err=%d!\r\n",
                        i4_ret));
        return SMR_INTERNAL_ERROR;        
    }

    sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_STRM_DEL,
                                   h_sess,
                                   (UINT32)h_stream);
    
    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  _sess_rcrd_sess_set
 *
 * Description: This function sets command to record session
 *
 * Inputs:  
 *       h_sess :           session handle
 *       pt_sess_obj :      session object pointer
 *       e_set_type :       set command
 *       pv_set_info:       set info
 *       ui4_set_info_size: set info length
 *       
 * Outputs: NULL  
 *
 * Returns: SMR_OK: Success. 
 *          Others: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-07 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sess_rcrd_sess_set(HANDLE_T           h_sess,
                                 SM_SESS_RCRD_T*    pt_sess_obj,
                                 SM_SET_TYPE_T      e_set_type,
                                 VOID*              pv_set_info,
                                 SIZE_T             ui4_set_info_size)
{
    switch (e_set_type)
    {
        case SM_SESS_SET_TYPE_PLAY:
            /*Need compare speed?*/
            if (SM_SESS_COND_STARTED == pt_sess_obj->t_base.e_sess_cond)
            {
                SM_RSS_DBG_INFO(("_sess_rcrd_sess_set:session has been started!!!\r\n"));
                return SMR_ALREADY_PLAYED;
            }
            else 
            {
                /* Change Speed for all streams? */
            }
            
            pt_sess_obj->t_base.i4_speed = (INT32)pv_set_info;
            sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_APP_PLAY_REQ,
                                           h_sess,
                                           0);
            SM_RSS_DBG_INFO(("_sess_rcrd_sess_set:play session and get to do starting...!\r\n"));
            break;
            
        case SM_SESS_SET_TYPE_STOP:
            if (SM_SESS_COND_OPENED == pt_sess_obj->t_base.e_sess_cond)
            {
                SM_RSS_DBG_INFO(("_sess_rcrd_sess_set:session has been stoppped!!!\r\n"));
                return SMR_ALREADY_STOPPED;    
            }
            
            sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_APP_STOP_REQ,
                                           h_sess,
                                           0);
            SM_RSS_DBG_INFO(("_sess_rcrd_sess_set:stop session and get to do stopping...!\r\n"));
            break;
        case SM_SESS_RCRD_SET_TYPE_ENCRYPTION:
            /*break;*/
        default:
                return SMR_NOT_SUPPORT;
    }

    return SMR_OK;
}

static INT32 _sm_rcrd_ctrl_tbl_get_strm_handles_hash_cb(HANDLE_T     h_hash,
                                                        UINT32       ui4_counter,
                                                        VOID*        pv_obj,
                                                        VOID*        pv_parse_tag)
{
    HANDLE_T   h_stream = (HANDLE_T)pv_obj;
    HANDLE_T*  ph_hdl_array = (HANDLE_T*)pv_parse_tag;
    
    SM_ASSERT(h_stream != NULL_HANDLE);
    SM_ASSERT(ph_hdl_array); 
    
    ph_hdl_array[ui4_counter] = h_stream;
    
    return HASH_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _sess_rcrd_sess_get
 *
 * Description: This function gets info of a record session
 *
 * Inputs:  
 *       h_sess :            session handle
 *       pt_sess_obj :       session object pointer
 *       e_get_type :        get command
 *       pv_get_info:        get info buffer 
 *       pui4_get_info_size: get info buffer length
 *       
 * Outputs: 
 *       pv_get_info:        get info
 *       pui4_get_info_size: get info length
 *
 * Returns: SMR_OK: Success. 
 *          Others: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-07 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sess_rcrd_sess_get(HANDLE_T           h_sess,
                                 SM_SESS_RCRD_T*    pt_sess_obj,
                                 SM_SESS_HANDLER_T* pt_sess_hdlr_obj,
                                 SM_GET_TYPE_T      e_get_type,
                                 VOID*              pv_get_info,
                                 SIZE_T*            pui4_get_info_size)
{
    INT32              i4_ret;
    SIZE_T             z_buff_size;
    SIZE_T             z_num_strm;

    if ((NULL == pv_get_info) || (NULL == pui4_get_info_size))
    {
    
        SM_RSS_DBG_ERR(("_sess_rcrd_sess_get: arg is null!\r\n"));
        return SMR_INV_GET_INFO;    
    }
    
    switch (e_get_type)
    {
        case SM_SESS_GET_TYPE_STATE:
            if ( *pui4_get_info_size < sizeof(SM_SESS_COND_T))
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            *((SM_SESS_COND_T*)(pv_get_info)) = pt_sess_obj->t_base.e_sess_cond;
            *pui4_get_info_size = sizeof(SM_SESS_COND_T);
            break;
        
        case SM_SESS_GET_TYPE_HDLR_NAME:
            if (*pui4_get_info_size <= 
                x_strlen(pt_sess_hdlr_obj->t_hdlr_desc.s_hdlr_name))
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            
            x_strncpy((CHAR*)(pv_get_info), 
                       pt_sess_hdlr_obj->t_hdlr_desc.s_hdlr_name,
                       x_strlen(pt_sess_hdlr_obj->t_hdlr_desc.s_hdlr_name));
                      
            *pui4_get_info_size = 
                       x_strlen(pt_sess_hdlr_obj->t_hdlr_desc.s_hdlr_name) + 1;
            break;
        
        case SM_SESS_GET_TYPE_H_CONNECTION:
            if (*pui4_get_info_size < sizeof(HANDLE_T))
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            *((HANDLE_T*)(pv_get_info)) = pt_sess_obj->t_base.h_connection;
            *pui4_get_info_size = sizeof(HANDLE_T);
            break;
        
        case SM_SESS_GET_TYPE_STRM_NUM:
            hash_lock(pt_sess_obj->t_base.h_strm_ctrl_tbl);
            i4_ret = hash_get_size(pt_sess_obj->t_base.h_strm_ctrl_tbl,
                                   (SIZE_T*)pv_get_info);
            hash_unlock(pt_sess_obj->t_base.h_strm_ctrl_tbl);

            if (i4_ret != HASH_OK)
            {
                return SMR_INTERNAL_ERROR;
            }
            *pui4_get_info_size = sizeof(SIZE_T);
            break;
        
        case SM_SESS_GET_TYPE_STRM_HANDLES:
            {
                z_buff_size = (*pui4_get_info_size)/sizeof(UINT32);
                z_num_strm = 0;

                if (*pui4_get_info_size < sizeof(UINT32))
                {
                    return SMR_INSUFFICIENT_SIZE;
                }
                
                hash_lock(pt_sess_obj->t_base.h_strm_ctrl_tbl);
                i4_ret = hash_get_size(pt_sess_obj->t_base.h_strm_ctrl_tbl,
                                       (SIZE_T*)&z_num_strm);
                if (i4_ret != HASH_OK)
                {
                    hash_unlock(pt_sess_obj->t_base.h_strm_ctrl_tbl);
                    return SMR_INTERNAL_ERROR;
                }
                if (z_num_strm > z_buff_size)
                {
                    hash_unlock(pt_sess_obj->t_base.h_strm_ctrl_tbl);
                    return SMR_INSUFFICIENT_SIZE;
                }
                x_memset(pv_get_info, 0, *pui4_get_info_size);
                
                i4_ret = hash_parse(pt_sess_obj->t_base.h_strm_ctrl_tbl,
                                    _sm_rcrd_ctrl_tbl_get_strm_handles_hash_cb,
                                    pv_get_info);
                hash_unlock(pt_sess_obj->t_base.h_strm_ctrl_tbl);
                if (i4_ret != HASH_OK)
                {
                    SM_RSS_DBG_ERR(("_sess_rcrd_sess_get:SESS(0x%.8x), "
                                    "hash_parse(_strm_ctrl_tbl_get_strm_handles_hash_cb)"
                                    " error, return %d!\r\n",
                                    (UINT32)h_sess, 
                                    i4_ret));
                    return SMR_INTERNAL_ERROR;
                }
            }
            break;

        case SM_SESS_GET_TYPE_DEST_GRP_NAME:
            if (*pui4_get_info_size <= 
                 x_strlen(pt_sess_obj->t_base.s_dest_grp_name))
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            
            x_strncpy((CHAR*)(pv_get_info), 
                       pt_sess_obj->t_base.s_dest_grp_name,
                       x_strlen(pt_sess_obj->t_base.s_dest_grp_name));
                      
            *pui4_get_info_size = 
                     x_strlen(pt_sess_obj->t_base.s_dest_grp_name) + 1;
            break;
        case SM_SESS_RCRD_GET_TYPE_PVR_BUFFER_INFO:
            i4_ret = sm_sess_rcrd_pvr_get(pt_sess_obj->t_base.h_sess,
                                          PVR_GET_TYPE_BUFFER_INFO,
                                          pv_get_info,
                                          pui4_get_info_size);
            if (i4_ret != RMR_OK)
            {
                return SMR_INTERNAL_ERROR;
            }
            break;
        case SM_SESS_RCRD_GET_TYPE_PVR_BUFFER_STATUS:
            i4_ret = sm_sess_rcrd_pvr_get(pt_sess_obj->t_base.h_sess,
                                          PVR_GET_TYPE_BUFFER_STATUS,
                                          pv_get_info,
                                          pui4_get_info_size);
            if (i4_ret != RMR_OK)
            {
                return SMR_INTERNAL_ERROR;
            }
            break;
        
        default:
            return SMR_NOT_SUPPORT;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _sess_rcrd_is_valid_stream
 *
 * Description: This function checks if a stream is in a session
 *
 * Inputs:  
 *       h_sess:      session handle
 *       pt_sess_obj: session object
 *       h_stream:    stream handle
 *         
 * Outputs: 
 *       NULL
 *
 * Returns: TRUE:     valid. 
 *          FALSE:    invalid
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
static BOOL _sess_rcrd_is_valid_stream(HANDLE_T        h_sess,
                                       SM_SESS_RCRD_T* pt_sess_obj,
                                       HANDLE_T        h_stream)
{
    INT32        i4_ret;
    HANDLE_T     h_strm_hdl;

    /*Check arguments*/
    if (NULL == pt_sess_obj)
    {
        SM_RSS_DBG_ERR(("_sess_rcrd_is_valid_stream:pt_sess_obj is NULL\r\n"));
        return FALSE;  
    }
    
    hash_lock(pt_sess_obj->t_base.h_strm_ctrl_tbl);
    i4_ret = hash_get_obj(pt_sess_obj->t_base.h_strm_ctrl_tbl,
                          (INT8*)&h_stream,
                          sizeof(h_stream),
                          (VOID*)&h_strm_hdl);
    hash_unlock(pt_sess_obj->t_base.h_strm_ctrl_tbl);
    
    if (i4_ret != HASH_OK)
    {
        return FALSE;        
    }
    
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name:  _rcrd_strm_get_common_state_parse_fct
 *
 * Description: This hash client function is called by hash API which
 *              checks if every element satisifies the tag contidion
 *
 * Inputs:  
 *       pt_hash:      hash handle
 *       ui4_counter:  hash element index
 *       pv_obj:hash   element object pointer
 *       pv_parse_tag: hash client parse function tag
 *         
 * Outputs: 
 *       NULL
 *
 * Returns: HASH_PARSE_BREAK: At least one element can not match client tag. 
 *          Other:            Succeed or internal error
 *
 * Author: lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
static INT32 _rcrd_strm_get_common_state_parse_fct(HANDLE_T  pt_hash,
                                                   UINT32    ui4_counter,
                                                   VOID*     pv_obj,
                                                   VOID*     pv_parse_tag)
{
    HANDLE_T        h_stream = (HANDLE_T)pv_obj;
    SM_STREAM_T*    pt_strm_obj;
    SM_COND_T*      pt_cmm_state;

    /* get stream object */
    pt_strm_obj = sm_get_strm_obj(h_stream);
    if (NULL == pt_strm_obj)
    {
        /* error, but try to process other streams */
        SM_RSS_DBG_ERR(("_rcrd_strm_get_common_state_parse_fct:pt_strm_obj"
                        "(hdlr=%u) is NULL\r\n",
                        h_stream));
        return HASH_OK;
    }
    
    pt_cmm_state = (SM_COND_T*)pv_parse_tag;
    if (NULL == pt_cmm_state)
    {
        SM_RSS_DBG_ERR(("_rcrd_strm_get_common_state_parse_fct:pt_cmm_state is NULL"));
        return HASH_INV_ARG;    
    }
    
    if (pt_strm_obj->e_condition != *pt_cmm_state)
    {
        *pt_cmm_state = SM_COND_IGNORE;
        return HASH_PARSE_BREAK;
    }
    
    return HASH_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _sess_rcrd_is_all_stream_opened
 *
 * Description: This function checks if all stream in a session are opened
 *
 * Inputs:  
 *       pt_sess_obj:  session object
 *         
 * Outputs: 
 *       NULL
 *
 * Returns: TRUE:     All are opened. 
 *          FALSE:    At least one is not opened
 *
 * Author: lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
static BOOL _sess_rcrd_is_all_stream_opened(SM_SESS_RCRD_T* pt_sess_obj)
{
    INT32        i4_ret;
    SM_COND_T    t_cmm_state;

    t_cmm_state = SM_COND_OPENED;
    
    /* check if all streams are in started state */
    hash_lock(pt_sess_obj->t_base.h_strm_ctrl_tbl);
    
    i4_ret = hash_parse(pt_sess_obj->t_base.h_strm_ctrl_tbl,
                        &_rcrd_strm_get_common_state_parse_fct,
                        &t_cmm_state);

    hash_unlock(pt_sess_obj->t_base.h_strm_ctrl_tbl);

    if (HASH_OK == i4_ret)
    {
        return TRUE;
    }
    
    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name:  _sess_rcrd_is_all_stream_started
 *
 * Description: This function checks is all stream in a session are opened
 *
 * Inputs:  
 *       pt_sess_obj:  session object
 *         
 * Outputs: 
 *       NULL
 *
 * Returns: TRUE:     All are started. 
 *          FALSE:    At least one is not started
 *
 * Author: lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
static BOOL _sess_rcrd_is_all_stream_started(SM_SESS_RCRD_T* pt_sess_obj)
{
    INT32        i4_ret;
    SM_COND_T    t_cmm_state;

    t_cmm_state = SM_COND_STARTED;
    
    /* check if all streams are in started state */
    hash_lock(pt_sess_obj->t_base.h_strm_ctrl_tbl);
    
    i4_ret = hash_parse(pt_sess_obj->t_base.h_strm_ctrl_tbl,
                        &_rcrd_strm_get_common_state_parse_fct,
                        &t_cmm_state);

    hash_unlock(pt_sess_obj->t_base.h_strm_ctrl_tbl);

    if (HASH_OK == i4_ret)
    {
        return TRUE;
    }
    
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name:  _sess_rcrd_prc_strm_opened
 *
 * Description: This function mainly handles messages stream opened messages
 *
 * Inputs:  
 *       h_sess  :     session handle
 *       h_stream:     stream handle
 *       pt_sess_obj:  session object
 *       e_event:      stream event
 *       ui4_data:     additional data e.g. reason.
 *         
 * Outputs: 
 *       NULL
 *
 * Returns: SMR_OK:     Success. 
 *          Others:     Fail.
 *
 * Author: lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sess_rcrd_prc_strm_opened(HANDLE_T        h_sess,
                                        HANDLE_T        h_stream,
                                        SM_SESS_RCRD_T* pt_sess_obj,
                                        SM_EVN_TYPE_T   e_event,
                                        UINT32          ui4_data)
{
    /*Check arguments*/
    if (NULL == pt_sess_obj)
    {
        SM_RSS_DBG_ERR(("_sess_rcrd_prc_strm_opened:pt_sess_obj is NULL\r\n"));
        return SMR_INV_ARG;
    }
    
    /*Notify session itself this stream is opened*/
    sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_STRM_OPENED,
                                   h_sess,
                                   (UINT32)h_stream);

    /* If all streams are in the same state, notify session state machine */
    if (_sess_rcrd_is_all_stream_opened(pt_sess_obj))
    {
        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_STRM_ALL_OPENED,
                                       h_sess,
                                       0);
    }
        
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _sess_rcrd_prc_strm_started
 *
 * Description: This function mainly handles messages stream started messages
 *
 * Inputs:  
 *       h_sess  :     session handle
 *       h_stream:     stream handle
 *       pt_sess_obj:  session object
 *       e_event:      stream event
 *       ui4_data:     additional data e.g. reason.
 *         
 * Outputs: 
 *       NULL
 *
 * Returns: SMR_OK,     Success. 
 *          Others:     Fail
 *
 * Author: lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sess_rcrd_prc_strm_started(HANDLE_T        h_sess,
                                         HANDLE_T        h_stream,
                                         SM_SESS_RCRD_T* pt_sess_obj,
                                         SM_EVN_TYPE_T   e_event,
                                         UINT32          ui4_data)
{
    /*Check arguments*/
    if (NULL == pt_sess_obj)
    {
        SM_RSS_DBG_ERR(("_sess_rcrd_prc_strm_started:pt_sess_obj is NULL\r\n"));
        return SMR_INV_ARG;
    }
    
    /*Notify session itself this stream is started*/
    sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_STRM_PLAYED,
                                   h_sess,
                                   (UINT32)h_stream);

    /* If all streams are in the same state, notify session state machine */
    if (_sess_rcrd_is_all_stream_started(pt_sess_obj))
    {
        if (pt_sess_obj->t_base.e_sess_cond != SM_SESS_COND_STARTED)
        {
            sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_STRM_ALL_PLAYED,
                                           h_sess,
                                           0);
        }
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _sess_rcrd_prc_strm_closed
 *
 * Description: This function mainly handle messages stream closed messages
 *
 * Inputs:  
 *       h_sess  :     session handle
 *       h_stream:     stream handle
 *       pt_sess_obj:  session object
 *       e_event:      stream event
 *       ui4_data:     additional data e.g. reason.
 *         
 * Outputs: 
 *       NULL
 *
 * Returns: SMR_OK,     Success. 
 *          Others:     Fail
 *
 * Author: lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
INT32 _sess_rcrd_prc_strm_closed(HANDLE_T        h_sess,
                                 HANDLE_T        h_stream,
                                 SM_SESS_RCRD_T* pt_sess_obj,
                                 SM_EVN_TYPE_T   e_event,
                                 UINT32          ui4_data)
{
    INT32                         i4_ret;
    SIZE_T                        z_size = 0;

    if (NULL == pt_sess_obj)
    {
        SM_RSS_DBG_ERR(("_sess_rcrd_prc_strm_closed:pt_sess_obj is NULL\r\n"));
        return SMR_INV_ARG;
    }

    /*Notify session itself this stream is closed*/
    sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_STRM_CLOSED,
                                   h_sess,
                                   (UINT32)h_stream);

    /*Remove the stream from the session*/
    i4_ret = _sess_rcrd_remove_stream(h_sess, pt_sess_obj, h_stream);
    if (i4_ret != SMR_OK)
    {
        SM_RSS_DBG_ERR(("_sess_rcrd_prc_strm_closed:fail return %d\r\n",
                        i4_ret));
        SM_ASSERT(0);
        return SMR_INTERNAL_ERROR;    
    }
    
    hash_lock(pt_sess_obj->t_base.h_strm_ctrl_tbl);
    i4_ret = hash_get_size(pt_sess_obj->t_base.h_strm_ctrl_tbl, &z_size);
    hash_unlock(pt_sess_obj->t_base.h_strm_ctrl_tbl);
    if (i4_ret != HASH_OK)
    {
        return SMR_INTERNAL_ERROR;
    }

    /*If all streams are closed then notify session itself*/
    if (0 == z_size)
    {
        /* all streams are closed, notify session state machine */
        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_STRM_ALL_CLOSED,
                                       h_sess,
                                       0);
    }
    else
    {
        if (_sess_rcrd_is_all_stream_started(pt_sess_obj))
        {
            sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_STRM_ALL_PLAYED,
                                           h_sess,
                                           0);
        }
        else if (_sess_rcrd_is_all_stream_opened(pt_sess_obj))
        {
            sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_STRM_ALL_OPENED,
                                           h_sess,
                                           0);
        }
        else
        {
            /*Nothing*/
        }
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _sess_rcrd_prc_hdlr_msg
 *
 * Description: This function mainly handles messages come from stream handler
 *
 * Inputs:  
 *       h_stream:     stream handle
 *       e_cond_type:  stream condition type
 *       pv_sm_nfy_tag:stream notify tag which is usally zero
 *       e_event:      stream event
 *       ui4_data:     additional data e.g. reason.
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
static VOID _sess_rcrd_prc_hdlr_msg(HANDLE_T        h_stream,
                                    SM_COND_T       e_cond_type,
                                    VOID*           pv_sm_nfy_tag,
                                    SM_EVN_TYPE_T   e_event,
                                    UINT32          ui4_data)
{
    INT32             i4_ret = SMR_OK;
    SM_SESS_RCRD_T*   pt_sess_obj;
    SM_STREAM_T*      pt_strm_obj;

    /*Get stream object*/
    pt_strm_obj = (SM_STREAM_T*)sm_get_strm_obj(h_stream);
    if (NULL == pt_strm_obj)
    {
        /* The stream is already destroyed */
        SM_RSS_DBG_ERR(("_sess_rcrd_prc_hdlr_msg:pt_strm_obj(hdlr=%u) is NULL\r\n", 
                        h_stream));
        return;
    }

    /*Get session object*/
    pt_sess_obj = (SM_SESS_RCRD_T*)sm_get_sess_obj(pt_strm_obj->h_sess);
    if (NULL == pt_sess_obj)
    {
        /*Abort*/
        SM_ABORT(SM_DBG_ABRT_INV_SESS_OBJ);
    }
    
    SM_LOCK(pt_sess_obj->t_base.h_lock)
    
    if (x_sema_lock(pt_strm_obj->h_lock, X_SEMA_OPTION_WAIT) != OSR_OK)
    {
        /* if stream lock fail, assume the stream is already closed */
        SM_UNLOCK(pt_sess_obj->t_base.h_lock)
        SM_RSS_DBG_ERR(("_sess_rcrd_prc_hdlr_msg:strm_obj is failed to lock\r\n"));
        return;
    }


    /*If stream is valid?*/
    if (_sess_rcrd_is_valid_stream(pt_strm_obj->h_sess,
                                   pt_sess_obj,
                                   h_stream))
    {
        /* notify stream event to application */
        pt_strm_obj->pf_nfy(h_stream, 
                            e_cond_type, 
                            pt_strm_obj->pv_nfy_tag, 
                            e_event,
                            ui4_data); 
        
        switch (e_cond_type)
        {
        case SM_COND_OPENED:
            i4_ret = _sess_rcrd_prc_strm_opened(pt_strm_obj->h_sess,
                                                h_stream, 
                                                pt_sess_obj,
                                                e_event,
                                                ui4_data);
            break;
            
        case SM_COND_STARTED:
            i4_ret = _sess_rcrd_prc_strm_started(pt_strm_obj->h_sess,
                                                 h_stream, 
                                                 pt_sess_obj,
                                                 e_event,
                                                 ui4_data);
            break;
    
        case SM_COND_CLOSED:
            i4_ret = _sess_rcrd_prc_strm_closed(pt_strm_obj->h_sess,
                                                h_stream, 
                                                pt_sess_obj,
                                                e_event,
                                                ui4_data);
            break;
        
        case SM_COND_ERROR:
            /* close the stream if the stream is in error state */
            i4_ret = sm_sess_rcrd_close(h_stream);
            break;
            
        case SM_COND_IGNORE:
        case SM_COND_OPENING:
        case SM_COND_STARTING:
        case SM_COND_STOPPING:
        case SM_COND_CLOSING:    
        default:
            /* break if  e_cond_type is a temporary state. */
            break;
        }

        /*If i4_ret is not ok just print error, doesn't send error event*/
        if (i4_ret != SMR_OK)
        {
            SM_RSS_DBG_ERR(("_sess_rcrd_prc_hdlr_msg:fail and return %d, NFY_STRM_COND(%u), "
                            "STRM_COND(%u), SESS_COND(%u)\r\n",
                            i4_ret, 
                            e_cond_type,
                            pt_strm_obj->e_condition,
                            pt_sess_obj->t_base.e_sess_cond));
        }
    }

    SM_UNLOCK(pt_strm_obj->h_lock)
    SM_UNLOCK(pt_sess_obj->t_base.h_lock)
    
    /*It's session's responsibility to free the stream handle!*/
    if (SM_COND_CLOSED == e_cond_type)
    {
        
        /*SM_UNLOCK(pt_strm_obj->h_lock)*/
        if ( sm_stream_lock() != SMR_OK )
        {
            SM_ABORT(SM_DBG_ABRT_CANNOT_LOCK_SEMAPHORE);
        }  
        handle_free(h_stream, FALSE);
        sm_stream_unlock();
    }
    else
    {
        /*SM_UNLOCK(pt_strm_obj->h_lock)*/
    }
   
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  _sess_rcrd_prc_pvr_msg
 *
 * Description: This function mainly handles messages come from RM for PVR
 *
 * Inputs:  
 *       h_sess:      session handle
 *       pt_sess_obj: session object
 *       pt_pvr_msg:  PVR message
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
static VOID _sess_rcrd_prc_pvr_tick_msg(HANDLE_T                         h_sess,
                                        SM_SESS_RCRD_T*                  pt_sess_obj,
                                        SM_SESS_RCRD_MSG_PVR_CTRL_NFY_T* pt_pvr_tick_msg)
{

    PVR_TICK_DATA_T*     pt_tick_data;
    PVR_NFY_COND_T       e_nfy_cond;
    INT32                i4_ret;

    if ((NULL == pt_sess_obj) || (NULL == pt_pvr_tick_msg))
    {
        SM_RSS_DBG_ERR(("_sess_rcrd_prc_pvr_tick_msg:invalid arguments\r\n"));
        return;
    }

    pt_tick_data = &(pt_pvr_tick_msg->t_tick_data);
    
    e_nfy_cond = pt_pvr_tick_msg->e_nfy_cond;
    if (PVR_COND_TICK == e_nfy_cond)
    {
        /*Write data*/
        i4_ret = sm_sess_rcrd_rutil_record(pt_sess_obj, 
                                           pt_tick_data);
        if (i4_ret != SMR_OK)
        {
            return;
        }
        /*show record utility is working*/
        sm_state_on_event(&pt_sess_obj->t_state,
                          SM_EVN_SESS_RCRD_RUTI_PLAYED,
                          (VOID*)pt_sess_obj,
                          NULL,
                          NULL);
        
    }
    else if (PVR_COND_OVERFLOW == e_nfy_cond)
    {
        extern INT32 Printf(const CHAR *format, ...);
        
        /* this is extremely important message, so must dump it even in release version */
        Printf ("\r\nPVR Buf Overflow! Reason=%s\r\n",
                (PVR_OVERFLOW_REASON_TS_BUF_FULL == pt_pvr_tick_msg->ui4_data)
                ? "PVR_OVERFLOW_REASON_TS_BUF_FULL"
                : "PVR_OVERFLOW_REASON_METADATA_BUF_FULL");
        
        /*Over flow: PVR driver will disable automatically*/
        sm_state_on_event(&pt_sess_obj->t_state,
                          SM_EVN_SESS_RCRD_PVR_OVERFLOW,
                          (VOID*)pt_sess_obj,
                          NULL,
                          NULL);
    }
    else
    {
        /*Error:*/
        sm_state_on_event(&pt_sess_obj->t_state,
                          SM_EVN_SESS_RCRD_COMP_ERR,
                          (VOID*)pt_sess_obj,
                          NULL,
                          NULL);
    }

    return;
}
/*-----------------------------------------------------------------------------
 * Name:  _sess_rcrd_prc_pvr_msg
 *
 * Description: This function mainly handles messages come from RM for PVR Ctrl
 *
 * Inputs:  
 *       h_sess:      session handle
 *       pt_sess_obj: session object
 *       pt_pvr_msg:  PVR message
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
static VOID _sess_rcrd_prc_pvr_msg(HANDLE_T                   h_sess,
                                   SM_SESS_RCRD_T*            pt_sess_obj,
                                   SM_SESS_RCRD_MSG_RM_NFY_T* pt_pvr_msg)
{

    RM_COND_T                     e_new_cond;
    RM_COND_T                     e_old_cond;

    if ((NULL == pt_sess_obj) || (NULL == pt_pvr_msg))
    {
        SM_RSS_DBG_ERR(("_sess_rcrd_prc_pvr_msg:invalid arguments\r\n"));
        return;
    }

    /*Check:if it is the opened PVR driver component?*/
    if ((pt_sess_obj->t_pvr_descr.t_info.ui1_in_port != pt_pvr_msg->ui1_port)
         || (pt_sess_obj->t_pvr_descr.h_comp != pt_pvr_msg->h_comp))
    {
        SM_RSS_DBG_ERR(("_sess_rcrd_prc_pvr_msg:input port(%u,%u) or "
                        "PVR Ctrl handle(0x%.8X, 0x%.8X) unmatch!\r\n",
                        pt_sess_obj->t_pvr_descr.t_info.ui1_in_port,
                        pt_pvr_msg->ui1_port,
                        pt_sess_obj->t_pvr_descr.h_comp,
                        pt_pvr_msg->h_comp));

        sm_state_on_event(&pt_sess_obj->t_state,
                          SM_EVN_SESS_RCRD_COMP_ERR,
                          (VOID*)pt_sess_obj,
                          NULL,
                          NULL);
        return;
    }

    /* Filter out all set/get operation notify. */
    e_new_cond = rm_cond_chg(pt_pvr_msg->e_nfy_cond, 
                             RM_COND_IGNORE, 
                             COND_GRP_OPERATION);
    if(e_new_cond != RM_COND_NO_CHG)
    {
        SM_RSS_DBG_INFO(("_sess_rcrd_prc_pvr_msg:Filter out RM operation  "
                         "condition(0x%.8x) notify.\r\n",
                         pt_pvr_msg->e_nfy_cond));
        return;
    }

    /*Update driver conditin if necessary*/
    if ((pt_pvr_msg->b_should_update_comp_descr) 
        && (pt_sess_obj->t_pvr_descr.e_comp_cond != RM_COND_CLOSED))
    {
        e_old_cond = pt_sess_obj->t_pvr_descr.e_comp_cond;
        pt_sess_obj->t_pvr_descr.e_comp_cond_org = e_old_cond;
        pt_sess_obj->t_pvr_descr.e_comp_cond = pt_pvr_msg->e_nfy_cond;
    }
    else
    {
        e_old_cond = pt_sess_obj->t_pvr_descr.e_comp_cond_org;
    }

    e_new_cond = rm_cond_chg(pt_pvr_msg->e_nfy_cond, 
                             e_old_cond,
                             COND_GRP_OBJ_STATE);
    if (RM_COND_OPENED == e_new_cond)
    {
        sm_state_on_event(&pt_sess_obj->t_state,
                          SM_EVN_SESS_RCRD_PVR_OPENED,
                          (VOID*)pt_sess_obj,
                          NULL,
                          NULL);
        return;
    }
    else if (RM_COND_CLOSED == e_new_cond)
    {
        sm_state_on_event(&pt_sess_obj->t_state,
                          SM_EVN_SESS_RCRD_PVR_CLOSED,
                          (VOID*)pt_sess_obj,
                          NULL,
                          NULL);
        return;
    }
    else
    {
        /*Do nothing*/
    }
    
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  _sess_rcrd_prc_rutil_msg
 *
 * Description: This function mainly handles messages come from Record Utility
 *
 * Inputs:  
 *       h_sess:      session handle
 *       pt_sess_obj: session object
 *       pt_rutil_msg:Record Utility message
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
static VOID _sess_rcrd_prc_rutil_msg(HANDLE_T                 h_sess,
                                     SM_SESS_RCRD_T*          pt_sess_obj,
                                     SM_SESS_RCRD_MSG_UTIL_T* pt_rutil_msg)
{
    INT32                       i4_ret;
    REC_UTIL_COND_T             e_cond;
    REC_UTIL_WRITE_ACK_INFO_T*  pt_rutil_wr_ack;
    /*PVR_UNLOCK_BUFFER_INFO_T    t_pvr_unlock_buf_info;*/

    if ((NULL == pt_sess_obj) || (NULL == pt_rutil_msg))
    {
        SM_RSS_DBG_ERR(("_sess_rcrd_prc_rutil_msg:invalid arguments\r\n"));
        return;
    }

    /*Get record utility state*/
    if (REC_UTIL_NTFY_CODE_COND_CHANGED == pt_rutil_msg->e_rutil_cond)
    {
        i4_ret = rec_util_get_cond(pt_sess_obj->t_rutil_info.h_record_util,
                                   &e_cond);
        if (SMR_OK == i4_ret)
        {
            if (REC_UTIL_COND_READY == e_cond)
            {
                sm_state_on_event(&pt_sess_obj->t_state,
                                  SM_EVN_SESS_RCRD_RUTI_READY,
                                  (VOID*)pt_sess_obj,
                                  NULL,
                                  NULL);
                pt_sess_obj->t_rutil_info.e_rutil_cond = REC_UTIL_COND_READY;
            }
            else
            {
                sm_state_on_event(&pt_sess_obj->t_state,
                                  SM_EVN_SESS_RCRD_RUTI_TRANSITION,
                                  (VOID*)pt_sess_obj,
                                  NULL,
                                  NULL);
                pt_sess_obj->t_rutil_info.e_rutil_cond = REC_UTIL_COND_TRANSITION;
            }
        }
    }
    /*write ack*/
    else if (REC_UTIL_NTFY_CODE_WRITE_ACK == pt_rutil_msg->e_rutil_cond)
    {
        /*Get if write done*/
        pt_rutil_wr_ack = (REC_UTIL_WRITE_ACK_INFO_T*)
                          &(pt_rutil_msg->u_util_ack.t_write_ack);
        if (pt_rutil_wr_ack != NULL)
        {
            /*1. free req ID*/
            i4_ret = sm_sess_rcrd_rutil_put_req(pt_sess_obj, pt_rutil_wr_ack->ui4_req_id);
            if (i4_ret != SMR_OK)
            {
                 /*debug error,do not send error event*/
                 SM_RSS_DBG_ERR(("_sess_rcrd_prc_rutil_msg:free req id(%u)"
                                 " err=%d!\r\n",
                                 pt_rutil_wr_ack->ui4_req_id,
                                 i4_ret));
            }

            /*2. handle ack*/
            if (REC_UTIL_REQR_OK == pt_rutil_wr_ack->i4_result)
            {
            }
            else if (REC_UTIL_REQR_ABORTED == pt_rutil_wr_ack->i4_result)
            {
                /*What to do?*/
                SM_RSS_DBG_ERR(("_sess_rcrd_prc_rutil_msg:record utility aborted!\r\n"));
            }
            else if (REC_UTIL_REQR_REACH_BOUND == pt_rutil_wr_ack->i4_result)
            {
                SM_RSS_DBG_INFO(("_sess_rcrd_prc_rutil_msg:reach bound!\r\n"));
                sm_state_on_event(&pt_sess_obj->t_state,
                                  SM_EVN_SESS_RCRD_RUTI_REACH_BOUND,
                                  (VOID*)pt_sess_obj,
                                  NULL,
                                  NULL);
            }
            else if (REC_UTIL_REQR_STRG_FULL == pt_rutil_wr_ack->i4_result)
            {
               
                SM_RSS_DBG_INFO(("_sess_rcrd_prc_rutil_msg:strg full!\r\n"));
                sm_state_on_event(&pt_sess_obj->t_state,
                                  SM_EVN_SESS_RCRD_RUTI_STRG_FULL,
                                  (VOID*)pt_sess_obj,
                                  NULL,
                                  NULL);
            }
            else
            {
                SM_RSS_DBG_ERR(("_sess_rcrd_prc_rutil_msg:pt_rutil_wr_ack->i4_result=%d!\r\n",
                                pt_rutil_wr_ack->i4_result));
            }

            /*2.Unlock pvr data*/
            sm_sess_rcrd_pvr_unlock_data(h_sess, pt_rutil_wr_ack);
            
            /*3 If all reqs are done*/
            if (sm_sess_rcrd_rutil_is_stopped(pt_sess_obj))
            {
                sm_state_on_event(&pt_sess_obj->t_state,
                                  SM_EVN_SESS_RCRD_RUTI_STOPPED,
                                  (VOID*)pt_sess_obj,
                                  NULL,
                                  NULL);
            }
        }
        else
        {
            /*debug error*/
            SM_RSS_DBG_ERR(("_sess_rcrd_prc_rutil_msg:rutil_wr_ack is null!\r\n"));
        }
    }
    /*If reset permit?*/
    else if (REC_UTIL_NTFY_CODE_RESET_REQ == pt_rutil_msg->e_rutil_cond)
    {
        SM_RSS_DBG_INFO(("_sess_rcrd_prc_rutil_msg:reset req comes!\r\n"));
        if (!sm_sess_rcrd_rutil_is_stopped(pt_sess_obj))
        {
            SM_RSS_DBG_INFO(("_sess_rcrd_prc_rutil_msg:not allow to reset db!\r\n"));
            return;
        }
        
        i4_ret = rec_util_permit_reset_req(pt_sess_obj->t_rutil_info.h_record_util,
                                           pt_rutil_msg->u_util_ack.ui4_reset_req_id);
        if (i4_ret != SMR_OK)
        {
            SM_RSS_DBG_ERR(("_sess_rcrd_prc_rutil_msg:permit reset error=%d!\r\n",
                            i4_ret));
        }
    }
    else
    {
        /*SM_RSS_DBG_ERR(("_sess_rcrd_prc_rutil_msg:pt_rutil_msg->e_rutil_cond = %d!\r\n",
                        pt_rutil_msg->e_rutil_cond));*/
    }

    return;
}

/*-----------------------------------------------------------------------------
 * Name:  _sess_rcrd_prc_scdb_msg
 *
 * Description: This function mainly handles messages come from SCDB
 *
 * Inputs:  
 *       h_sess:      session handle
 *       pt_sess_obj: session object
 *       pt_scdb_msg: SCDB message
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
static VOID _sess_rcrd_prc_scdb_msg(HANDLE_T                 h_sess,
                                    SM_SESS_RCRD_T*          pt_sess_obj,
                                    SM_SESS_RCRD_MSG_SCDB_T* pt_scdb_msg)
{
    /*INT32          i4_ret;*/
    SCDB_COND_T    e_scdb_cond;
    UINT32         ui4_reason;
    /*SCDB_COMMON_T  t_scdb_cmmn_old;*/

    /*Init*/
    e_scdb_cond = pt_scdb_msg->e_scdb_cond;
    ui4_reason = pt_scdb_msg->ui4_reason;
    
    /* keep using old data if updating*/
    if (SCDB_UPDATING == e_scdb_cond)
    {
        return;
    }
    else if (SCDB_UPDATED == e_scdb_cond)
    {
        if (ui4_reason & SCDB_RECORD_ADD)
        {
            /* do nothing, left it to be processed by stream handler */
        }
        if (ui4_reason & SCDB_RECORD_DEL)
        {
            /* do nothing, left it to be processed by stream handler */
        }
        if (ui4_reason & SCDB_RECORD_MOD)
        {
            /* do nothing, left it to be processed by stream handler */
        }
        if (ui4_reason & SCDB_COMMON_MOD)
        {
            /* do nothing, left it to be processed by stream handler */
        }
        if (ui4_reason & SCDB_REASON_UNK)
        {
            /* do nothing */
        }
    }
    else if (SCDB_CLOSED == e_scdb_cond)
    {
        pt_sess_obj->t_base.h_scdb = NULL_HANDLE;
        
        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_SCDB_CLOSED,
                                       h_sess,
                                       0);
        return;
    }
    else
    {
        /* improper SCDB state */
        SM_RSS_DBG_INFO(("_sess_rcrd_prc_scdb_msg:Improper SCDB state!\r\n"));
        SM_ASSERT(0);
    }

    return;
}

/*-----------------------------------------------------------------------------
 * Name:  _sess_rcrd_prc_do_event_msg
 *
 * Description: This function mainly handle events which are generated in internal  
 *              module when receive some external messages e.g. from RM/PM/Record 
 *              Utility/SCDB etc.
 *                
 * Inputs:  
 *       h_sess:         session handle
 *       pt_sess_obj:    session object
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
static VOID _sess_rcrd_prc_do_event_msg(HANDLE_T                           h_sess,
                                        SM_SESS_RCRD_T*                    pt_sess_obj,
                                        const SM_SESS_RCRD_MSG_DO_EVENT_T* pt_do_event_msg)
{
    sm_state_on_event(&pt_sess_obj->t_state,
                      pt_do_event_msg->ui4_event,
                      (VOID*)pt_sess_obj,
                      (VOID*)pt_do_event_msg->ui4_data,
                      NULL);
}



/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_open
 *
 * Description: This function open a record session. 
 *
 * Inputs:  
 *       h_connection:     connction handle 
 *       h_sess_hdlr:      record session handler object handle
 *       ps_dest_grp_name: group name for RM select component
 *       pv_sess_data:     session data come from applicatin
 *       z_sess_data_size: session data length
 *       i4_speed:         session speed 
 *       pv_sess_nfy_tag:  session notify
 *
 * Outputs: 
 *       ph_sess: record session object handle for application and sm
 *
 * Returns: SMR_OK,     Success. 
 *          Others:     Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-06 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_sess_rcrd_open(HANDLE_T              h_connection,
                        HANDLE_T              h_sess_hdlr,
                        const CHAR*           ps_dest_grp_name,
                        VOID*                 pv_sess_data,
                        SIZE_T                z_sess_data_size,
                        INT32                 i4_speed,
                        VOID*                 pv_sess_nfy_tag,
                        HANDLE_T*             ph_sess)
{
    INT32                       i4_ret;
    SM_SESS_RCRD_T*             pt_sess_obj;
    CM_CTRL_RM_SRC_COMP_T       t_ctrl_rm_src_comp;
    SM_RCRD_SESS_STATE_TAG_T*   pt_state_tag;
    UINT32                      ui4_name_len;
    SM_SESS_RCRD_CFG_T*         pt_sess_cfg; 


    /*Parameters Check*/
    if (NULL == ph_sess)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_open:ph_sess is NULL\r\n"));
        return SMR_INV_ARG;
    }
    if (z_sess_data_size != sizeof(SM_SESS_RCRD_CFG_T))
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_open:z_sess_data_size=%d\r\n", 
                        z_sess_data_size));
        return SMR_INV_ARG;
    }

    /*Init */
    i4_ret       = SMR_OK;
    pt_state_tag = NULL;
    pt_sess_obj  = NULL;

    
    /*Do.*/
    do 
    {

        /* 1. Create session object */
        pt_sess_obj = (SM_SESS_RCRD_T*)x_mem_alloc(sizeof(SM_SESS_RCRD_T));
        if (NULL == pt_sess_obj)
        {
            i4_ret = SMR_INSUFFICIENT_MEMORY;    
            SM_RSS_DBG_ERR(("sm_sess_rcrd_open:pt_sess_obj is NULL\r\n"));
            break;
        }
        x_memset(pt_sess_obj, 0, sizeof(SM_SESS_RCRD_T));

        /*Hash table to contain streams*/
        i4_ret = hash_create(SM_STRM_CTRL_TBL_SIZE,
                             &pt_sess_obj->t_base.h_strm_ctrl_tbl);
        if ((i4_ret != HASH_OK)
             || (NULL_HANDLE == pt_sess_obj->t_base.h_strm_ctrl_tbl))
        {
            i4_ret = SMR_INTERNAL_ERROR; 
            SM_RSS_DBG_ERR(("sm_sess_rcrd_open:fail to create strm ctrl hash(%d)\r\n", 
                            i4_ret));
            break;
        }
        
        /*Session object mutex*/
        pt_sess_obj->t_base.h_lock = NULL_HANDLE;
        i4_ret = x_sema_create(&pt_sess_obj->t_base.h_lock,
                               X_SEMA_TYPE_MUTEX,
                               X_SEMA_STATE_UNLOCK);
        if ((i4_ret != OSR_OK)
            || (NULL_HANDLE == pt_sess_obj->t_base.h_lock))
        {
            i4_ret = SMR_INTERNAL_ERROR;   
            SM_RSS_DBG_ERR(("sm_sess_rcrd_open:fail to create sess obj sema(%d)\r\n", 
                            i4_ret));
            break;
        }

        pt_sess_obj->t_base.h_connection = h_connection;
        pt_sess_obj->t_base.h_sess_hdlr = h_sess_hdlr;
        pt_sess_obj->t_base.e_sess_cond = SM_SESS_COND_IGNORE;
        pt_sess_obj->t_base.e_sess_type = SM_SESS_TYPE_RCRD;
        pt_sess_obj->t_base.pv_sess_nfy_tag = pv_sess_nfy_tag;
        pt_sess_obj->t_base.i4_speed = i4_speed;
        pt_sess_obj->t_base.h_sess = NULL_HANDLE;
        x_strcpy( pt_sess_obj->t_base.s_dest_grp_name, ps_dest_grp_name);

        /* Get source driver component */
        i4_ret = x_cm_get(pt_sess_obj->t_base.h_connection,
                           CM_CTRL_RM_SRC_COMP,
                           (VOID*)&t_ctrl_rm_src_comp);
        if (i4_ret != CMR_OK)
        {
            i4_ret = SMR_INTERNAL_ERROR;   
            SM_RSS_DBG_ERR(("sm_sess_rcrd_open:fail to get source(%d)\r\n", 
                            i4_ret));
            break;
        }
        pt_sess_obj->h_source = t_ctrl_rm_src_comp.h_rm_comp;

        pt_sess_obj->t_pvr_descr = t_sm_drv_comp_initor;
        
        pt_sess_obj->t_rutil_info.h_record_util = NULL_HANDLE;
        pt_sess_obj->t_rutil_info.e_rutil_cond = REC_UTIL_COND_TRANSITION;

        pt_sess_cfg = (SM_SESS_RCRD_CFG_T*)pv_sess_data;
        pt_sess_obj->t_sess_cfg = *pt_sess_cfg;
        ui4_name_len = x_strlen(pt_sess_cfg->s_name_rec_util);
        if (ui4_name_len >= SM_SESS_RCRD_UTIL_NAME_LEN)
        {
            i4_ret = SMR_INTERNAL_ERROR;   
            SM_RSS_DBG_ERR(("sm_sess_rcrd_open:err record util name(%s)\r\n", 
                            pt_sess_cfg->s_name_rec_util));
        }
        pt_sess_obj->t_sess_cfg.s_name_rec_util = x_mem_alloc(ui4_name_len + 1);
        if (NULL == pt_sess_obj->t_sess_cfg.s_name_rec_util)
        {
            i4_ret = SMR_INSUFFICIENT_MEMORY;   
            SM_RSS_DBG_ERR(("sm_sess_rcrd_open:fail to alloc mem\r\n"));
        }
        x_strcpy(pt_sess_obj->t_sess_cfg.s_name_rec_util ,
                 pt_sess_cfg->s_name_rec_util);
        
        pt_sess_obj->ui4_lastest_pm_event = SM_EVN_IGNORE;
        pt_sess_obj->ui4_lastest_pvr_event = SM_EVN_IGNORE;
        pt_sess_obj->ui4_lastest_rutil_event = SM_EVN_IGNORE;
        pt_sess_obj->ui4_lastest_strm_event = SM_EVN_IGNORE;


        /* 2. Create session handle */
        i4_ret = handle_alloc(SM_HT_SESSION,
                              pt_sess_obj,
                              (VOID*)h_connection,
                              &_sess_rcrd_free_fct,
                              ph_sess);
        if ((i4_ret != HR_OK) || (NULL_HANDLE == *ph_sess))
        {
            i4_ret = SMR_OUT_OF_HANDLES;   
            SM_RSS_DBG_ERR(("sm_sess_rcrd_open:fail to alloc sess handle(%d)\r\n", 
                            i4_ret));
            break;
            
        }
        pt_sess_obj->t_base.h_sess = *ph_sess;
       

        /*3.Init State Machine */
        pt_state_tag = (SM_RCRD_SESS_STATE_TAG_T*)
                        x_mem_alloc(sizeof(SM_RCRD_SESS_STATE_TAG_T));
        if (NULL == pt_state_tag)
        {
            i4_ret = SMR_INSUFFICIENT_MEMORY; 
            SM_RSS_DBG_ERR(("sm_sess_rcrd_open:pt_state_tag is NULL\r\n"));
            break;
        }

        pt_state_tag->h_sess = *ph_sess;
        pt_state_tag->pt_sess_obj = pt_sess_obj;
        i4_ret = sm_state_init(at_rcrd_sess_state_cond_lst,
                               at_rcrd_sess_state_descr_lst,
                               at_rcrd_sess_allowed_pending_event_lst,
                               sm_rcrd_sess_state_notify_app,
                               (VOID*)pt_state_tag,
                               &pt_sess_obj->t_state);
        if (i4_ret != SMR_OK)
        {
            i4_ret = SMR_INTERNAL_ERROR;   
            SM_RSS_DBG_ERR(("sm_sess_rcrd_open:fail to init state machine(%d)\r\n", 
                            i4_ret));
            break;   
        }


        /*4.Send out App event to start record session state machine to do openning*/
        SM_LOCK(pt_sess_obj->t_base.h_lock)
        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_APP_OPEN_REQ,
                                       *ph_sess,
                                       0);
        SM_UNLOCK(pt_sess_obj->t_base.h_lock)
        
        /*Normal*/
        SM_RSS_DBG_INFO(("sm_sess_rcrd_open:open session and get to do openning...!\r\n"));
        return SMR_OK;
        
    } while(0);


    if (i4_ret != SMR_OK)
    {
        /*free state tag */
        if (pt_sess_obj != NULL)
        {
            
            /*destroy session object mutex*/
            if (pt_sess_obj->t_base.h_lock != NULL_HANDLE)
            {
                x_sema_delete(pt_sess_obj->t_base.h_lock);
                pt_sess_obj->t_base.h_lock = NULL_HANDLE;
            }
            
            /*destroy strm ctrl hash*/
            if (pt_sess_obj->t_base.h_strm_ctrl_tbl != NULL_HANDLE)
            {
                hash_delete(pt_sess_obj->t_base.h_strm_ctrl_tbl, 
                           (hash_obj_free_fct)NULL);
                pt_sess_obj->t_base.h_strm_ctrl_tbl = NULL_HANDLE;
            }

            /*free record util name*/
            if (pt_sess_obj->t_sess_cfg.s_name_rec_util != NULL)
            {
                x_mem_free(pt_sess_obj->t_sess_cfg.s_name_rec_util);
                pt_sess_obj->t_sess_cfg.s_name_rec_util = NULL;
            }
            
            /*free session object*/
            x_mem_free(pt_sess_obj);
            pt_sess_obj = NULL;
        }
    
        /*free session object*/
        if (pt_state_tag != NULL)
        {
            x_mem_free(pt_state_tag);
            pt_state_tag = NULL;
        }
    
    }
    
    /*Abnormal*/
    SM_RSS_DBG_ERR(("sm_sess_rcrd_open:Open record session failed!\r\n"));
    return i4_ret;
    
}

/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_strm_nfy
 *
 * Description: This function will be passed to stream handler as a stream notify tag.
 *              Here will get stream status and notify to session task to handle.
 *
 * Inputs:  
 *       h_stream:     stream handle
 *       e_cond_type:  stream condition
 *       pv_sm_nfy_tag:stream tag, is zero
 *       e_event:      stream notify function
 *       ui4_data:     stream additional info e.g. reason info
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
 *   (1)2009-03-06 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_sess_rcrd_strm_nfy(HANDLE_T              h_stream,
                            SM_COND_T             e_cond_type,
                            VOID*                 pv_sm_nfy_tag,
                            SM_EVN_TYPE_T         e_event,
                            UINT32                ui4_data)
{
    SM_SESS_RCRD_MSG_T      t_msg;
         
    t_msg.e_msg_type = SM_SESS_RCRD_MSG_TYPE_HDLR;
    t_msg.u.t_rcrd_strm_msg.h_stream = h_stream;
    t_msg.u.t_rcrd_strm_msg.e_strm_cond = e_cond_type;
    t_msg.u.t_rcrd_strm_msg.pv_tag = pv_sm_nfy_tag;
    t_msg.u.t_rcrd_strm_msg.e_strm_evn = e_event;
    t_msg.u.t_rcrd_strm_msg.ui4_data = ui4_data;
    
    sm_rcrd_sess_send_msg(&t_msg);
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_strm_open
 *
 * Description: This function opens a stream in a record session. 
 *
 * Inputs:  
 *       pt_stream_base:   stream object base
 *       pt_cmd_sets:      commands come from application
 *       b_auto_play:      singal if automatically playback
 *       pf_sm_nfy:        stream notify function
 *       pv_sm_nfy_tag:    stream notify tag
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
 *   (1)2009-03-06 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_sess_rcrd_strm_open(const SM_STREAM_T*  pt_stream_base,
                             const SM_COMMAND_T* pt_cmd_sets,
                             BOOL                b_auto_play,
                             x_stream_nfy_fct    pf_sm_nfy,
                             VOID*               pv_sm_nfy_tag,
                             HANDLE_T*           ph_stream) /* OUT */
{
    INT32             i4_ret;
    HANDLE_TYPE_T     t_hdl_type;
    SM_HANDLER_T*     pt_hdlr_obj = NULL;
    SM_SESS_RCRD_T*   pt_sess_obj = NULL;

    /*Check arguments*/
    if ((NULL == ph_stream) || (NULL == pt_stream_base))
    {
        return SMR_INV_ARG;
    }
    
    /*Get stream handler object*/
    i4_ret = handle_get_type_obj(pt_stream_base->h_handler,
                                 &t_hdl_type,
                                 (VOID**)&pt_hdlr_obj);
    if ((i4_ret != HR_OK) || (t_hdl_type != SM_HT_HANDLER))
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_strm_open:find strm handler obj err(%d),"
                        "(handle=%d)!\r\n",
                        i4_ret, 
                        pt_stream_base->h_handler));
        return SMR_INV_HANDLE;
    }

    /*Get session object*/
    pt_sess_obj = (SM_SESS_RCRD_T*)sm_get_sess_obj(pt_stream_base->h_sess);
    if (pt_sess_obj == NULL)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_strm_open:sess obj(%u) is not existed!\r\n",
                        pt_stream_base->h_sess));
        return SMR_SESS_NOT_EXIST;
    }

    /* Check if session state allows */
    if ((SM_SESS_COND_OPENING == pt_sess_obj->t_base.e_sess_cond)
         || (SM_SESS_COND_CLOSING == pt_sess_obj->t_base.e_sess_cond)
         || (SM_SESS_COND_CLOSED == pt_sess_obj->t_base.e_sess_cond)
         || (SM_SESS_COND_IGNORE == pt_sess_obj->t_base.e_sess_cond))
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_strm_open:not allow open stream in "
                        "current status=%d!\r\n",
                        pt_sess_obj->t_base.e_sess_cond));
        return SMR_NOT_ALLOW;
    }

    /*If auto playback*/
    if ((SM_SESS_COND_STARTING == pt_sess_obj->t_base.e_sess_cond)
         || (SM_SESS_COND_STARTED == pt_sess_obj->t_base.e_sess_cond))
    {
        b_auto_play = TRUE;
    }
        
    i4_ret = pt_hdlr_obj->t_fct_tbl.pf_open(pt_stream_base,
                                            pt_cmd_sets,
                                            b_auto_play,
                                            pf_sm_nfy,
                                            pv_sm_nfy_tag,
                                            ph_stream);/* OUT */
    if ((i4_ret != SMR_OK) || (NULL_HANDLE== *ph_stream))
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_strm_open:Open stream err=%d!\r\n",
                        i4_ret));
        return i4_ret;
    }

    /*Add to session stream ctrl hash table*/
    i4_ret = _sess_rcrd_add_stream(pt_stream_base->h_sess,
                                   pt_sess_obj,
                                   *ph_stream);
    if (i4_ret != SMR_OK)
    {
        pt_stream_base = sm_get_strm_obj(*ph_stream);
        if (pt_stream_base != NULL)
        {
            SM_LOCK(pt_stream_base->h_lock)
            pt_hdlr_obj->t_fct_tbl.pf_close(*ph_stream);
            SM_UNLOCK(pt_stream_base->h_lock)
        }
        SM_RSS_DBG_ERR(("sm_sess_rcrd_strm_open:add stream err=%d!\r\n",
                        i4_ret));
        return i4_ret;
    }
        
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_close
 *
 * Description: This function closes a stream or a session. 
 *
 * Inputs:  
 *       h_handle:   session or stream handle
 *
 * Outputs: 
 *       NULL
 *
 * Returns: SMR_OK,     Success. 
 *          Others:     Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-07 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_sess_rcrd_close(HANDLE_T  h_handle)
{
    INT32             i4_ret;
    HANDLE_TYPE_T     e_hdl_type;
    VOID*             pv_obj;

    SM_HANDLER_T*     pt_hdlr_obj;
    SM_STREAM_T*      pt_strm_obj;
    SM_SESS_RCRD_T*   pt_sess_obj;
    

    /*Get object to know if a stream or session request?*/
    i4_ret = handle_get_type_obj(h_handle, &e_hdl_type, (VOID**)&pv_obj);    
    if (i4_ret != HR_OK)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_close:get object type err=%d!\r\n",
                        i4_ret));
        return SMR_INV_HANDLE;
    }

    /*If a session only starts session state machine to do closing*/
    if (SM_HT_SESSION == e_hdl_type)
    {
        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_APP_CLOSE_REQ,
                                       h_handle,
                                       0);
        SM_RSS_DBG_INFO(("sm_sess_rcrd_close:close session(%u) and get to do closing...!\r\n",
                         h_handle));
    }
    else if (SM_HT_STREAM == e_hdl_type)
    {
        pt_strm_obj = (SM_STREAM_T*)pv_obj;
        pt_sess_obj = (SM_SESS_RCRD_T*)sm_get_sess_obj(pt_strm_obj->h_sess);
        if (NULL == pt_sess_obj)
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_close:get sess object err!\r\n"));
            return SMR_SESS_NOT_EXIST;
        }
        
        /* check if session state allows */
        if ((SM_SESS_COND_OPENING == pt_sess_obj->t_base.e_sess_cond)
             || (SM_SESS_COND_CLOSING == pt_sess_obj->t_base.e_sess_cond)
             || (SM_SESS_COND_CLOSED == pt_sess_obj->t_base.e_sess_cond)
             || (SM_SESS_COND_IGNORE == pt_sess_obj->t_base.e_sess_cond))
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_close:not allow to close stream(0x%8X)!\r\n",
                            h_handle));
            return SMR_NOT_ALLOW;
        }
            
        i4_ret = handle_get_type_obj(pt_strm_obj->h_handler,
                                     &e_hdl_type,
                                     (VOID**)&pt_hdlr_obj);
        if ((i4_ret != HR_OK) || (e_hdl_type != SM_HT_HANDLER))
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_close:get stream handler err(%d)!\r\n",
                            i4_ret));
            return SMR_INV_HANDLE;
        }
        
        i4_ret = pt_hdlr_obj->t_fct_tbl.pf_close(h_handle);
        if (i4_ret != SMR_OK)
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_close:close stream err(%d)!\r\n",
                            i4_ret));
            return i4_ret;
        }
    }
    else
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_close:err handle type(%d)!\r\n",
                        e_hdl_type));
        return SMR_INV_HANDLE;    
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_set
 *
 * Description: This function is a entry for setting something to a stream 
 *              or a session. 
 *
 * Inputs:  
 *       h_handle:       session or stream handle
 *       e_set_type:     set type defined in u_sm.h
 *       pv_set_inf:     set info
 *       z_set_info_len: set info length
 *
 * Outputs: 
 *       NULL
 *
 * Returns: SMR_OK,     Success. 
 *          Others:     Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-07 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_sess_rcrd_set(HANDLE_T       h_handle,
                       SM_SET_TYPE_T  e_set_type,
                       VOID*          pv_set_info,
                       SIZE_T         z_set_info_len)
{
    INT32             i4_ret;
    HANDLE_TYPE_T     e_hdl_type;
    VOID*             pv_obj;
    SM_SESS_RCRD_T*   pt_sess_obj;
    SM_HANDLER_T*     pt_hdlr_obj;
    SM_STREAM_T*      pt_strm_obj ;
    
    /*Get object to know if a stream or session request?*/
    i4_ret = handle_get_type_obj(h_handle, &e_hdl_type, (VOID**)&pv_obj);    
    if (i4_ret != HR_OK)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_set:get object type err=%d!\r\n",
                        i4_ret));
        return SMR_INV_HANDLE;
    }

    
    if (SM_HT_SESSION == e_hdl_type)
    {
        pt_sess_obj = (SM_SESS_RCRD_T*)sm_get_sess_obj(h_handle);
        if (NULL == pt_sess_obj)
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_set:get session object error!\r\n"));
            return SMR_INV_HANDLE;
        }
    
        i4_ret = _sess_rcrd_sess_set(h_handle,
                                     pt_sess_obj,
                                     e_set_type,
                                     pv_set_info,
                                     z_set_info_len);
        if (i4_ret != SMR_OK)
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_set:set session err=%d!\r\n",
                            i4_ret));
            return i4_ret;    
        }
    }
    else if (SM_HT_STREAM == e_hdl_type)
    {
        pt_strm_obj = (SM_STREAM_T*)pv_obj;

        i4_ret = handle_get_type_obj(pt_strm_obj->h_handler,
                                     &e_hdl_type,
                                     (VOID**)&pt_hdlr_obj);
        if ((i4_ret != HR_OK) || (e_hdl_type != SM_HT_HANDLER))
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_set:get stream hdlr err=%d, e_hdl_type(%d)!\r\n",
                            i4_ret, 
                            e_hdl_type));
            return SMR_INV_HANDLE;
        }

        /*Set type check*/
        if ((SM_GET_GRP(e_set_type) != SM_GRP_STRM)
             && (SM_GET_GRP(e_set_type) < (SM_GRP_LAST_ENTRY<<SM_CODE_BITS)))
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_set:error set command type=%d!\r\n",
                            e_set_type));
            return SMR_INV_ARG;
        }
        
        i4_ret = pt_hdlr_obj->t_fct_tbl.pf_set(h_handle,
                                               e_set_type,
                                               pv_set_info,
                                               z_set_info_len);
        if (i4_ret != SMR_OK)
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_set:stream set err=%d!\r\n",
                            i4_ret));
            return i4_ret;
        }        
    }
    else
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_set:invalid handle type(%d)!\r\n",
                        e_hdl_type));
        return SMR_INV_HANDLE;    
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_get
 *
 * Description: This function is a entry for getting info of a stream 
 *              or a session. 
 *
 * Inputs:  
 *       h_handle:   session or stream handle
 *       e_set_type: get type defined in u_sm.h
 *
 * Outputs: 
 *       pv_set_info:     get info buffer
 *       z_set_info_len:  get info buffer length
 *
 * Returns: SMR_OK,     Success. 
 *          Others:     Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-07 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_sess_rcrd_get(HANDLE_T       h_handle,
                       SM_GET_TYPE_T  e_get_type,
                       VOID*          pv_get_info,     /* IN/OUT */
                       SIZE_T*        pz_get_info_len) /* IN/OUT */
{
    INT32              i4_ret;
    HANDLE_TYPE_T      e_hdl_type;
    VOID*              pv_obj;
    SM_SESS_RCRD_T*    pt_sess_obj;
    SM_SESS_HANDLER_T* pt_sess_hdlr_obj;
    SM_HANDLER_T*      pt_hdlr_obj;
    SM_STREAM_T*       pt_strm_obj;

    /*Check Args */
    if ((NULL == pv_get_info) || (NULL == pz_get_info_len))
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_get:invalid args!\r\n"));
        return SMR_INV_ARG;
    }
    
    /*Check is it a session or stream request?*/
    i4_ret = handle_get_type_obj(h_handle, &e_hdl_type, (VOID**)&pv_obj);    
    if (i4_ret != HR_OK)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_get:get type obj err(%d) handle type(%d)!\r\n",
                        i4_ret, 
                        e_hdl_type));
        return SMR_INV_HANDLE;
    }
    
    if (SM_HT_SESSION == e_hdl_type)
    {
        pt_sess_hdlr_obj = (SM_SESS_HANDLER_T*)
                            sm_get_sess_hdlr_obj(h_handle,
                                                (VOID**)&pt_sess_obj);
        if ((NULL == pt_sess_hdlr_obj) || (NULL == pt_sess_obj))
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_get:get session obj & sess hdlr obj err!\r\n"));
            return SMR_INV_HANDLE;
        }
        
        i4_ret = _sess_rcrd_sess_get(h_handle,
                                     pt_sess_obj,
                                     pt_sess_hdlr_obj,
                                     e_get_type,
                                     pv_get_info,
                                     pz_get_info_len);
        
        if (i4_ret != SMR_OK)
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_get:set session err(%d)!\r\n",
                            i4_ret));
            return i4_ret;    
        }
    }
    else if (SM_HT_STREAM == e_hdl_type)
    {
        pt_strm_obj = (SM_STREAM_T*)pv_obj;
        i4_ret = handle_get_type_obj(pt_strm_obj->h_handler,
                                     &e_hdl_type,
                                     (VOID**)&pt_hdlr_obj);
        if ((i4_ret != HR_OK) 
            || (e_hdl_type != SM_HT_HANDLER) 
            || (NULL == pt_hdlr_obj))
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_get:get obj type err(%d) e_hdl_type(%d)!\r\n",
                            i4_ret, e_hdl_type));
            return SMR_INV_HANDLE;
        }

        if ( (SM_GET_GRP(e_get_type) != SM_GRP_STRM)
              && (SM_GET_GRP(e_get_type) < (SM_GRP_LAST_ENTRY<<SM_CODE_BITS)))
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_get:error get command type(%d)!\r\n",
                            e_get_type));
            return SMR_INV_ARG;
        }
        
        i4_ret = pt_hdlr_obj->t_fct_tbl.pf_get(h_handle,
                                               e_get_type,
                                               pv_get_info,
                                               pz_get_info_len);
        if (i4_ret != SMR_OK)
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_get:stream get err(%d)!\r\n",
                            i4_ret));
            return i4_ret;
        }
    }
    else
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_get:invalid handle type(%d)!\r\n",
                        e_hdl_type));
        return SMR_INV_HANDLE;    
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_task
 *
 * Description: This function handles record session message which may come from
 *              Driver/RM/PM/Record Utilty/Stream/Session itself/etc. 
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
VOID sm_sess_rcrd_task(VOID* pv_arg)
{
    INT32                      i4_ret;
    UINT16                     ui2_index;
    SM_SESS_RCRD_T*            pt_sess_obj;
    SM_SESS_RCRD_MSG_T         t_msg;
    SIZE_T                     z_msg_size;

    /*Init*/
    x_memset(&t_msg, 0, sizeof(SM_SESS_RCRD_MSG_T));
    z_msg_size = sizeof(SM_SESS_RCRD_MSG_T);
    
    /*Receive mesage*/
    i4_ret = x_msg_q_receive(&ui2_index,
                             &t_msg,
                             &z_msg_size,
                             &gh_sess_rcrd_msgq,
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

    /*message from record stream handler*/
    if (SM_SESS_RCRD_MSG_TYPE_HDLR == t_msg.e_msg_type)
    {
        _sess_rcrd_prc_hdlr_msg(t_msg.u.t_rcrd_strm_msg.h_stream,
                                t_msg.u.t_rcrd_strm_msg.e_strm_cond,
                                t_msg.u.t_rcrd_strm_msg.pv_tag,
                                t_msg.u.t_rcrd_strm_msg.e_strm_evn,
                                t_msg.u.t_rcrd_strm_msg.ui4_data);
        return;
    }

    pt_sess_obj = (SM_SESS_RCRD_T*)sm_get_sess_obj(t_msg.h_sess);
    if (NULL == pt_sess_obj)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_task:WARNING, receive msg(%d) for a invalid "
                        "session handle(0x%8X)\r\n",
                        (UINT32)t_msg.e_msg_type, 
                        (UINT32)t_msg.h_sess));
        return;
    }
    
    /*Return code NULL*/
    SM_LOCK_RETURN_ON_FAIL(pt_sess_obj->t_base.h_lock, )
    
    switch(t_msg.e_msg_type)
    {
        case SM_SESS_RCRD_MSG_TYPE_PVR_CTRL_NFY:
            _sess_rcrd_prc_pvr_tick_msg(t_msg.h_sess,
                                        pt_sess_obj,
                                        &t_msg.u.t_pvr_tick_msg);
            break;
            
        case SM_SESS_RCRD_MSG_TYPE_PVR:
            _sess_rcrd_prc_pvr_msg(t_msg.h_sess,
                                   pt_sess_obj,
                                   &t_msg.u.t_pvr_msg);
            break;

        case SM_SESS_RCRD_MSG_TYPE_RUTIL:
            _sess_rcrd_prc_rutil_msg(t_msg.h_sess,
                                     pt_sess_obj,
                                     &t_msg.u.t_rutil_msg);
            break;
            
        case SM_SESS_RCRD_MSG_TYPE_SCDB:
            _sess_rcrd_prc_scdb_msg(t_msg.h_sess,
                                    pt_sess_obj,
                                    &t_msg.u.t_rcrd_scdb_msg);
            break;

        case SM_SESS_RCRD_MSG_TYPE_DO_EVENT:
            _sess_rcrd_prc_do_event_msg(t_msg.h_sess,
                                        pt_sess_obj,
                                        &t_msg.u.t_rcrd_event_msg);
            break;

        default:
            SM_RSS_DBG_ERR(("sm_sess_rcrd_task undefined session message(%u)\r\n",
                            (UINT32)t_msg.e_msg_type));
            SM_ABORT(SM_DBG_ABRT_INV_CASE_STMT);
    }
    SM_UNLOCK(pt_sess_obj->t_base.h_lock)

    /*Free session*/
    if (SM_SESS_COND_CLOSED == pt_sess_obj->t_base.e_sess_cond)
    {
        if ( sm_sess_lock() != SMR_OK )
        {
            SM_ABORT(SM_DBG_ABRT_CANNOT_LOCK_SEMAPHORE);
        }
        if ( sm_stream_lock() != SMR_OK )
        {
            SM_ABORT(SM_DBG_ABRT_CANNOT_LOCK_SEMAPHORE);
        }    
        handle_free(t_msg.h_sess, FALSE);
        sm_stream_unlock();
        sm_sess_unlock();
    }
    
    return;
}



