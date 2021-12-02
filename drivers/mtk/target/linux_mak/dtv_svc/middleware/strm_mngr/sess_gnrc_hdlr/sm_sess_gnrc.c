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
 * $RCSfile: sm_sess_gnrc.c,v $
 * $Revision: #2 $
 * $Date: 2012/07/24 $
 * $Author: jiangbo.wang $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 1b21913f7ca173ceaa5cb8a8200f6796 $
 *
 * Description: 
 *         This file implements the Stream Manager's present session.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "handle/x_handle.h"
#include "handle/handle.h"
#include "dbg/x_dbg.h"
#include "dbg/dbg.h"
#include "prc_mngr/prc_mngr.h"
#include "conn_mngr/x_cm.h"
#include "scdb/x_scdb_api.h"

#include "strm_mngr/x_sm.h"
#include "hash/hash_api.h"
#include "strm_mngr/sm_slctr.h"
#include "strm_mngr/_sm_utils.h"
#include "strm_mngr/sm_drv_comp.h"
#include "strm_mngr/sm_drv_map.h"
#include "strm_mngr/sess_gnrc_hdlr/sm_sess_gnrc.h"
#include "strm_mngr/sess_gnrc_hdlr/x_sm_sess_gnrc.h"
#include "strm_mngr/sess_gnrc_hdlr/_sm_sess_gnrc_pcr.h"
#include "strm_mngr/sess_gnrc_hdlr/_sm_sess_gnrc_stc.h"
#include "strm_mngr/sess_gnrc_hdlr/_sm_gs_state.h"
#include "strm_mngr/sess_gnrc_hdlr/_sm_sess_gnrc_scart_out.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define SM_SESS_GNRC_MSGQ_MAX_SIZE    256

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static HANDLE_T        gh_sess_gnrc_hdlr = NULL_HANDLE;
static HANDLE_T        gh_sess_gnrc_msgq = NULL_HANDLE;

/* 
 *  Generic Session State Machine 
 */
static SM_STATE_DESCR_T   at_gs_state_descr_lst[] =
{
/*  ui4_state,                 b_prcss_pending,    b_allow_pending,  pf_state_do,                pf_state_do_exception */
  { SM_SESS_COND_IGNORE,       FALSE,              FALSE,            NULL,                       sm_gs_state_do_ignore_excpt },
  { SM_SESS_COND_OPENING,      FALSE,              TRUE,             sm_gs_state_do_opening,     sm_gs_state_do_opening_excpt },
  { SM_SESS_COND_OPENED,       TRUE,               FALSE,            sm_gs_state_do_opened,      sm_gs_state_do_opened_excpt },
  { SM_SESS_COND_STARTING,     FALSE,              TRUE,             sm_gs_state_do_starting,    sm_gs_state_do_starting_excpt },
  { SM_SESS_COND_STARTED,      TRUE,               FALSE,            sm_gs_state_do_started,     sm_gs_state_do_started_excpt },
  { SM_SESS_COND_STOPPING,     FALSE,              TRUE,             sm_gs_state_do_stopping,    sm_gs_state_do_stopping_excpt },
  { SM_SESS_COND_CLOSING,      FALSE,              TRUE,             sm_gs_state_do_closing,     sm_gs_state_do_closing_excpt },
  { SM_SESS_COND_CLOSED,       FALSE,              FALSE,            sm_gs_state_do_closed,      sm_gs_state_do_closed_excpt },
  { SM_SESS_COND_ERROR,        TRUE,               FALSE,            sm_gs_state_do_error,       sm_gs_state_do_error_excpt },
  SM_STATE_DESCR_NULL  
};

static UINT32             at_gs_allowed_pending_event_lst[] =
{
  SM_EVN_SESS_APP_PLAY_REQ,
  SM_EVN_SESS_APP_STOP_REQ,
  SM_EVN_SESS_APP_CLOSE_REQ,
  SM_EVN_SESS_SCDB_CLOSED,
  SM_EVN_IGNORE
};

static SM_STATE_COND_T    at_gs_state_cond_lst[] =
{
  /* ui4_curr_state,       ui4_event,                  ui4_next_state,          ui4_pending_event,          pf_event_do */
  /* ignore state conditions *************************************************/ 
  { SM_SESS_COND_IGNORE,   SM_EVN_SESS_APP_OPEN_REQ,   SM_SESS_COND_OPENING,    SM_EVN_IGNORE,              NULL },
  
  /* opening state conditions ************************************************/ 
  { SM_SESS_COND_OPENING,  SM_EVN_SESS_DRV_OPENED,     SM_SESS_COND_OPENED,     SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_OPENING,  SM_EVN_SESS_APP_PLAY_REQ,   SM_SESS_COND_OPENING,    SM_EVN_SESS_APP_PLAY_REQ,   NULL },
  { SM_SESS_COND_OPENING,  SM_EVN_SESS_APP_CLOSE_REQ,  SM_SESS_COND_OPENING,    SM_EVN_SESS_APP_CLOSE_REQ,  NULL },
  { SM_SESS_COND_OPENING,  SM_EVN_SESS_SCDB_CLOSED,    SM_SESS_COND_OPENING,    SM_EVN_SESS_APP_CLOSE_REQ,  NULL },
  { SM_SESS_COND_OPENING,  SM_EVN_SESS_DRV_PLAYED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_OPENING,  SM_EVN_SESS_DRV_STOPPED,    SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_OPENING,  SM_EVN_SESS_DRV_CLOSED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_OPENING,  SM_EVN_SESS_DRV_ERR,        SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_OPENING,  SM_EVN_SESS_INTERNAL_ERR,   SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_OPENING,  SM_EVN_SESS_STRM_ERR,       SM_SESS_COND_OPENING,    SM_EVN_SESS_STRM_ERR,       NULL },
  
  /* opened state conditions *************************************************/ 
  { SM_SESS_COND_OPENED,   SM_EVN_SESS_APP_PLAY_REQ,   SM_SESS_COND_STARTING,   SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_OPENED,   SM_EVN_SESS_APP_CLOSE_REQ,  SM_SESS_COND_CLOSING,    SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_OPENED,   SM_EVN_SESS_SCDB_CLOSED,    SM_SESS_COND_CLOSING,    SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_OPENED,   SM_EVN_SESS_DRV_OPENED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_OPENED,   SM_EVN_SESS_DRV_PLAYED,     SM_SESS_COND_STARTING,   SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_OPENED,   SM_EVN_SESS_DRV_STOPPED,    SM_STATE_IGNORE,         SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_OPENED,   SM_EVN_SESS_DRV_CLOSED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_OPENED,   SM_EVN_SESS_DRV_ERR,        SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_OPENED,   SM_EVN_SESS_INTERNAL_ERR,   SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_OPENED,   SM_EVN_SESS_STRM_ERR,       SM_SESS_COND_CLOSING,    SM_EVN_IGNORE,              NULL },
  
  /* starting state conditions ***********************************************/ 
  { SM_SESS_COND_STARTING, SM_EVN_SESS_DRV_PLAYED,     SM_SESS_COND_STARTED,    SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_STARTING, SM_EVN_SESS_APP_STOP_REQ,   SM_SESS_COND_STARTING,   SM_EVN_SESS_APP_STOP_REQ,   NULL },
  { SM_SESS_COND_STARTING, SM_EVN_SESS_APP_CLOSE_REQ,  SM_SESS_COND_STARTING,   SM_EVN_SESS_APP_CLOSE_REQ,  NULL },
  { SM_SESS_COND_STARTING, SM_EVN_SESS_SCDB_CLOSED,    SM_SESS_COND_STARTING,   SM_EVN_SESS_APP_CLOSE_REQ,  NULL },
  { SM_SESS_COND_STARTING, SM_EVN_SESS_DRV_OPENED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_STARTING, SM_EVN_SESS_DRV_STOPPED,    SM_STATE_IGNORE,         SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_STARTING, SM_EVN_SESS_DRV_CLOSED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_STARTING, SM_EVN_SESS_DRV_ERR,        SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_STARTING, SM_EVN_SESS_INTERNAL_ERR,   SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  
  /* started state conditions ************************************************/ 
  { SM_SESS_COND_STARTED,  SM_EVN_SESS_APP_STOP_REQ,   SM_SESS_COND_STOPPING,   SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_STARTED,  SM_EVN_SESS_APP_CLOSE_REQ,  SM_SESS_COND_STOPPING,   SM_EVN_SESS_APP_CLOSE_REQ,  NULL },
  { SM_SESS_COND_STARTED,  SM_EVN_SESS_SCDB_CLOSED,    SM_SESS_COND_STOPPING,   SM_EVN_SESS_SCDB_CLOSED,    NULL },
  { SM_SESS_COND_STARTED,  SM_EVN_SESS_DRV_OPENED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_STARTED,  SM_EVN_SESS_DRV_PLAYED,     SM_STATE_IGNORE,         SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_STARTED,  SM_EVN_SESS_DRV_STOPPED,    SM_SESS_COND_STOPPING,   SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_STARTED,  SM_EVN_SESS_DRV_CLOSED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_STARTED,  SM_EVN_SESS_DRV_ERR,        SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_STARTED,  SM_EVN_SESS_INTERNAL_ERR,   SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_STARTED,  SM_EVN_SESS_STRM_ERR,       SM_SESS_COND_STOPPING,   SM_EVN_SESS_STRM_ERR,       NULL },
  
  /* stopping state conditions ***********************************************/ 
  { SM_SESS_COND_STOPPING, SM_EVN_SESS_DRV_STOPPED,    SM_SESS_COND_OPENED,     SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_STOPPING, SM_EVN_SESS_APP_CLOSE_REQ,  SM_SESS_COND_STOPPING,   SM_EVN_SESS_APP_CLOSE_REQ,  NULL },
  { SM_SESS_COND_STOPPING, SM_EVN_SESS_SCDB_CLOSED,    SM_SESS_COND_STOPPING,   SM_EVN_SESS_SCDB_CLOSED,    NULL },
  { SM_SESS_COND_STOPPING, SM_EVN_SESS_APP_PLAY_REQ,   SM_SESS_COND_STOPPING,   SM_EVN_SESS_APP_PLAY_REQ,   NULL },
  { SM_SESS_COND_STOPPING, SM_EVN_SESS_APP_PLAY_REQ,   SM_SESS_COND_STOPPING,   SM_EVN_SESS_APP_PLAY_REQ,   NULL },
  { SM_SESS_COND_STOPPING, SM_EVN_SESS_DRV_OPENED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_STOPPING, SM_EVN_SESS_DRV_PLAYED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_STOPPING, SM_EVN_SESS_DRV_CLOSED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_STOPPING, SM_EVN_SESS_DRV_ERR,        SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_STOPPING, SM_EVN_SESS_INTERNAL_ERR,   SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_STOPPING, SM_EVN_SESS_STRM_ERR,       SM_SESS_COND_STOPPING,   SM_EVN_SESS_STRM_ERR,       NULL },

  /* closing state conditions ************************************************/ 
  { SM_SESS_COND_CLOSING,  SM_EVN_SESS_DRV_CLOSED,     SM_SESS_COND_CLOSED,     SM_EVN_IGNORE,              NULL },

  /* error state conditions **************************************************/ 
  { SM_SESS_COND_ERROR,    SM_EVN_SESS_APP_CLOSE_REQ,  SM_SESS_COND_CLOSING,    SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_ERROR,    SM_EVN_SESS_SCDB_CLOSED,    SM_SESS_COND_CLOSING,    SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_ERROR,    SM_EVN_SESS_APP_PLAY_REQ,   SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_ERROR,    SM_EVN_SESS_APP_STOP_REQ,   SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_ERROR,    SM_EVN_SESS_DRV_OPENED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_ERROR,    SM_EVN_SESS_DRV_OPENED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_ERROR,    SM_EVN_SESS_DRV_PLAYED,     SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_ERROR,    SM_EVN_SESS_DRV_STOPPED,    SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_ERROR,    SM_EVN_SESS_DRV_CLOSED,     SM_SESS_COND_CLOSING,    SM_EVN_IGNORE,              NULL },
  { SM_SESS_COND_ERROR,    SM_EVN_SESS_STRM_ERR,       SM_SESS_COND_ERROR,      SM_EVN_IGNORE,              NULL },

  /* ANY state events ********************************************************/ 
  { SM_STATE_IGNORE,       SM_EVN_SESS_PCR_OPENED,     SM_STATE_IGNORE,         SM_EVN_IGNORE,              sm_gs_state_do_drv_events },
  { SM_STATE_IGNORE,       SM_EVN_SESS_PCR_CLOSED,     SM_STATE_IGNORE,         SM_EVN_IGNORE,              sm_gs_state_do_drv_events },
  { SM_STATE_IGNORE,       SM_EVN_SESS_PCR_CONNECTED,  SM_STATE_IGNORE,         SM_EVN_IGNORE,              sm_gs_state_do_drv_events },
  { SM_STATE_IGNORE,       SM_EVN_SESS_PCR_DISCONNECTED,SM_STATE_IGNORE,        SM_EVN_IGNORE,              sm_gs_state_do_drv_events },
  { SM_STATE_IGNORE,       SM_EVN_SESS_PCR_PID_UPDATE, SM_STATE_IGNORE,         SM_EVN_IGNORE,              sm_gs_state_do_drv_events },
  { SM_STATE_IGNORE,       SM_EVN_SESS_PM_COND_CLOSED, SM_STATE_IGNORE,         SM_EVN_IGNORE,              sm_gs_state_do_drv_events },
  { SM_STATE_IGNORE,       SM_EVN_SESS_PM_COND_PLAYED, SM_STATE_IGNORE,         SM_EVN_IGNORE,              sm_gs_state_do_drv_events },
  { SM_STATE_IGNORE,       SM_EVN_SESS_PM_COND_STOPPED,SM_STATE_IGNORE,         SM_EVN_IGNORE,              sm_gs_state_do_drv_events },
  { SM_STATE_IGNORE,       SM_EVN_SESS_STRM_ADD,       SM_STATE_IGNORE,         SM_EVN_IGNORE,              sm_gs_state_do_drv_events },
  { SM_STATE_IGNORE,       SM_EVN_SESS_STRM_DEL,       SM_STATE_IGNORE,         SM_EVN_IGNORE,              sm_gs_state_do_drv_events },
  { SM_STATE_IGNORE,       SM_EVN_SESS_STRM_OPENED,    SM_STATE_IGNORE,         SM_EVN_IGNORE,              sm_gs_state_do_drv_events },
  { SM_STATE_IGNORE,       SM_EVN_SESS_STRM_PLAYED,    SM_STATE_IGNORE,         SM_EVN_IGNORE,              sm_gs_state_do_drv_events },
  { SM_STATE_IGNORE,       SM_EVN_SESS_STRM_CLOSED,    SM_STATE_IGNORE,         SM_EVN_IGNORE,              sm_gs_state_do_drv_events },
  { SM_STATE_IGNORE,       SM_EVN_SESS_STRM_ALL_OPENED,SM_STATE_IGNORE,         SM_EVN_IGNORE,              sm_gs_state_do_drv_events },
  { SM_STATE_IGNORE,       SM_EVN_SESS_STRM_ALL_PLAYED,SM_STATE_IGNORE,         SM_EVN_IGNORE,              sm_gs_state_do_drv_events },
  { SM_STATE_IGNORE,       SM_EVN_SESS_STRM_ALL_CLOSED,SM_STATE_IGNORE,         SM_EVN_IGNORE,              sm_gs_state_do_drv_events },
  { SM_STATE_IGNORE,       SM_EVN_SESS_STC_OPENED,     SM_STATE_IGNORE,         SM_EVN_IGNORE,              sm_gs_state_do_drv_events },
  { SM_STATE_IGNORE,       SM_EVN_SESS_STC_CLOSED,     SM_STATE_IGNORE,         SM_EVN_IGNORE,              sm_gs_state_do_drv_events },
  { SM_STATE_IGNORE,       SM_EVN_SESS_DMX_OPENED,     SM_STATE_IGNORE,         SM_EVN_IGNORE,              sm_gs_state_do_drv_events },
  { SM_STATE_IGNORE,       SM_EVN_SESS_DMX_CLOSED,     SM_STATE_IGNORE,         SM_EVN_IGNORE,              sm_gs_state_do_drv_events },
  
  /* !!! Don't remove this !!!************************************************/
  SM_STATE_COND_NULL
};

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
static INT32 sess_gnrc_add_stream( 
    HANDLE_T        h_sess,
    SM_SESS_GNRC_T* pt_sess_obj,
    HANDLE_T        h_stream );

static BOOL sess_gnrc_is_valid_stream( 
    HANDLE_T        h_sess,
    SM_SESS_GNRC_T* pt_sess_obj,
    HANDLE_T        h_stream );

static INT32 sess_gnrc_remove_stream( 
    HANDLE_T        h_sess,
    SM_SESS_GNRC_T* pt_sess_obj,
    HANDLE_T        h_stream );

/*-----------------------------------------------------------------------------
                    internal functions implementation
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: _strm_ctrl_tbl_free_fct
 *
 * Description: <function description>
 *
 * Inputs:  pv_obj References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _strm_ctrl_tbl_free_fct( VOID* pv_obj )
{
    /* nothing to do currently */
}

/*-----------------------------------------------------------------------------
 * Name: _strm_ctrl_tbl_get_strm_handles_hash_cb
 *
 * Description: <function description>
 *
 * Inputs:  h_hash       References.
 *          ui4_counter  References.
 *          pv_obj       References.
 *          pv_parse_tag References.
 *
 * Outputs: - 
 *
 * Returns: HASH_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _strm_ctrl_tbl_get_strm_handles_hash_cb( 
    HANDLE_T          h_hash,
    UINT32            ui4_counter,
    VOID*             pv_obj,
    VOID*             pv_parse_tag )
{
    HANDLE_T   h_stream = (HANDLE_T)pv_obj;
    HANDLE_T*  ph_hdl_array = (HANDLE_T*)pv_parse_tag;
    
    SM_ASSERT( h_stream != NULL_HANDLE );
    SM_ASSERT( ph_hdl_array ); 
    
    ph_hdl_array[ui4_counter] = h_stream;
    
    return HASH_OK;
}

static STC_AV_SYNC_MODE_T sess_gnrc_stc_mode_2_drv( 
    SM_SESS_STC_AV_SYNC_MODE_T e_stc_av_sync_mode )
{
    switch( e_stc_av_sync_mode )
    {
    case SM_SESS_STC_AV_SYNC_MODE_FREE_RUN:
        return STC_AV_SYNC_MODE_FREE_RUN;

    case SM_SESS_STC_AV_SYNC_MODE_AUDIO_MASTER:
        return STC_AV_SYNC_MODE_AUDIO_MASTER;

    case SM_SESS_STC_AV_SYNC_MODE_VIDEO_MASTER:
        return STC_AV_SYNC_MODE_VIDEO_MASTER;

    case SM_SESS_STC_AV_SYNC_MODE_SYSTEM_MASTER:
        return STC_AV_SYNC_MODE_SYSTEM_MASTER;


    default:
        return STC_AV_SYNC_MODE_FREE_RUN;
    }
}

/*-----------------------------------------------------------------------------
 * Name: sess_gnrc_free_fct
 *
 * Description: <function description>
 *
 * Inputs:  h_handle     References.
 *          e_type       References.
 *          pv_obj       References.
 *          pv_tag       References.
 *          b_req_handle References.
 *
 * Outputs: - 
 *
 * Returns: TRUE   Success.
 *          FALSE  Fail.
 *
 ----------------------------------------------------------------------------*/
static BOOL sess_gnrc_free_fct( 
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle )
{
    SM_SESS_GNRC_T*  pt_sess_obj = (SM_SESS_GNRC_T*)pv_obj;
    INT32            i4_ret;
    SIZE_T           z_size = 0;

    /* It's not allowed for application to call x_handle_free() to free the 
       session handle. */
    if ( b_req_handle )
    {
        return FALSE;    
    }

    if ( !pt_sess_obj || !pv_tag || e_type != SM_HT_SESSION )
    {
        return FALSE;    
    }
    
    x_sema_delete( pt_sess_obj->t_base.h_lock );
    pt_sess_obj->t_base.h_lock = NULL_HANDLE;
    
    /* not allow to free the handle if there are still streams in the table */
    hash_lock( pt_sess_obj->t_base.h_strm_ctrl_tbl );

    i4_ret = hash_get_size( pt_sess_obj->t_base.h_strm_ctrl_tbl, &z_size);

    hash_unlock( pt_sess_obj->t_base.h_strm_ctrl_tbl );

    if ( i4_ret != SMR_OK || z_size != 0 )
    {
        return FALSE;
    }
    
    if ( pt_sess_obj->t_base.h_scdb != NULL_HANDLE )
    {
        return FALSE;
    }
    
    /* not allow to free the handle if PM service is not closed yet */
    if ( pt_sess_obj->t_base.h_pm_service != NULL_HANDLE )
    {
        return FALSE;
    }
    
    if ( pt_sess_obj->t_base.h_strm_ctrl_tbl )
    {
        hash_delete( pt_sess_obj->t_base.h_strm_ctrl_tbl, &_strm_ctrl_tbl_free_fct );
    }

    if ( pt_sess_obj->t_state.pv_state_nfy_tag )
    {
        x_mem_free( pt_sess_obj->t_state.pv_state_nfy_tag );
    }
    
    if (!SLIST_IS_EMPTY(&pt_sess_obj->t_scart_list))
    {
        SM_SESS_GNRC_SCART_OUT_T    t_scart_out;
        t_scart_out.b_enable = FALSE;
        t_scart_out.b_set_by_name = TRUE;
        t_scart_out.u.s_scart_name = NULL;

        sm_sess_gnrc_scart_out_set(
            h_handle,
            pt_sess_obj,
            &t_scart_out);
    }
    
    x_mem_free( pt_sess_obj );
    
    return TRUE;
}


/*-----------------------------------------------------------------------------
 * Name: _sm_gs_chg_speed_hash_cb
 *
 * Description: <function description>
 *
 * Inputs:  h_hash       References.
 *          ui4_counter  References.
 *          pv_obj       References.
 *          pv_parse_tag References.
 *
 * Outputs: - 
 *
 * Returns: HASH_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sm_gs_chg_speed_hash_cb( 
    HANDLE_T          h_hash,
    UINT32            ui4_counter,
    VOID*             pv_obj,
    VOID*             pv_parse_tag )
{
    HANDLE_T        h_stream = (HANDLE_T)pv_obj;
    SM_SESS_GNRC_T* pt_sess_obj = (SM_SESS_GNRC_T*)pv_parse_tag;
    SM_STREAM_T*    pt_strm_obj = NULL;
    SM_HANDLER_T*   pt_hdlr_obj = NULL;
    
    /* get stream object */
    pt_hdlr_obj = (SM_HANDLER_T*)sm_get_strm_hdlr_obj( h_stream, 
                                                       (VOID**)&pt_strm_obj );
    if ( !pt_hdlr_obj || !pt_strm_obj )
    {
        /* error, but try to process other streams */
        return HASH_OK;
    }
    
    /* assume stream already closed, if lock is fail */
    SM_LOCK_RETURN_ON_FAIL( pt_strm_obj->h_lock, HASH_OK );
    
    pt_hdlr_obj->t_fct_tbl.pf_set( h_stream,
                                   SM_SESS_SET_TYPE_PLAY,
                                   (VOID*)pt_sess_obj->t_base.i4_speed,
                                   sizeof(pt_sess_obj->t_base.i4_speed) );
    
    SM_UNLOCK( pt_strm_obj->h_lock );

    return HASH_OK;
}

/*-----------------------------------------------------------------------------
 * Name: strm_get_common_state_parse_fct
 *
 * Description: <function description>
 *
 * Inputs:  pt_hash      References.
 *          ui4_counter  References.
 *          pv_obj       References.
 *          pv_parse_tag References.
 *
 * Outputs: - 
 *
 * Returns: HASH_OK              Success.
 *          HASH_INTERNAL_ERROR  Fail.
 *          HASH_PARSE_BREAK     Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 strm_get_common_state_parse_fct( 
    HANDLE_T       pt_hash,
    UINT32            ui4_counter,
    VOID*             pv_obj,
    VOID*             pv_parse_tag )
{
    HANDLE_T        h_stream = (HANDLE_T)pv_obj;
    SM_STREAM_T*    pt_strm_obj = NULL;
    SM_COND_T*      pt_cmm_state;

    /* get stream object */
    pt_strm_obj = sm_get_strm_obj( h_stream );
    if ( !pt_strm_obj )
    {
        /* error, but try to process other streams */
        return HASH_OK;
    }
    
    pt_cmm_state = (SM_COND_T*)pv_parse_tag;
    if ( !pt_cmm_state )
    {
        return HASH_INTERNAL_ERROR;    
    }
    
    /*
    if ( ui4_counter == 0 )
    {
        *pt_cmm_state = pt_strm_obj->e_condition;
        return HASH_OK;
    }
    */
    
    if ( pt_strm_obj->e_condition != *pt_cmm_state )
    {
        *pt_cmm_state = SM_COND_IGNORE;
        return HASH_PARSE_BREAK;
    }
    
    return HASH_OK;
}


static BOOL sess_gnrc_is_all_stream_started( SM_SESS_GNRC_T* pt_sess_obj )
{
    INT32                         i4_ret;
    SM_COND_T                     t_cmm_state = SM_COND_STARTED;
    
    /* check if all streams are in started state */
    hash_lock( pt_sess_obj->t_base.h_strm_ctrl_tbl );
    
    i4_ret = hash_parse( pt_sess_obj->t_base.h_strm_ctrl_tbl,
                         &strm_get_common_state_parse_fct,
                         &t_cmm_state );

    hash_unlock( pt_sess_obj->t_base.h_strm_ctrl_tbl );

    if ( i4_ret != HASH_PARSE_BREAK )
    {
        return TRUE;
    }
    
    return FALSE;
}

static BOOL sess_gnrc_is_all_stream_opened( SM_SESS_GNRC_T* pt_sess_obj )
{
    INT32                         i4_ret;
    SM_COND_T                     t_cmm_state = SM_COND_OPENED;
    
    /* check if all streams are in started state */
    hash_lock( pt_sess_obj->t_base.h_strm_ctrl_tbl );
    
    i4_ret = hash_parse( pt_sess_obj->t_base.h_strm_ctrl_tbl,
                         &strm_get_common_state_parse_fct,
                         &t_cmm_state );

    hash_unlock( pt_sess_obj->t_base.h_strm_ctrl_tbl );

    if ( i4_ret != HASH_PARSE_BREAK )
    {
        return TRUE;
    }
    
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: sess_gnrc_prc_strm_opened
 *
 * Description: <function description>
 *
 * Inputs:  h_sess      References.
 *          h_stream    References.
 *          pt_sess_obj References.
 *          e_event     References.
 *          ui4_data    References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK                        Success.
 *          SMR_INTERNAL_ERROR            Fail.
 *          SMR_CAN_NOT_OP_IN_THIS_STATE  Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 sess_gnrc_prc_strm_opened( 
    HANDLE_T        h_sess,
    HANDLE_T        h_stream,
    SM_SESS_GNRC_T* pt_sess_obj,
    SM_EVN_TYPE_T   e_event,
    UINT32          ui4_data )
{
    sm_gs_send_msg_do_event(
        SM_EVN_SESS_STRM_OPENED,
        h_sess,
        (UINT32)h_stream );
    
    if ( sess_gnrc_is_all_stream_opened(pt_sess_obj) )
    {
        /* all streams are in the same state, notify session state machine */            
        sm_gs_send_msg_do_event(
            SM_EVN_SESS_STRM_ALL_OPENED,
            h_sess,
            0 );
    }
        
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sess_gnrc_prc_strm_started
 *
 * Description: <function description>
 *
 * Inputs:  h_sess      References.
 *          h_stream    References.
 *          pt_sess_obj References.
 *          e_event     References.
 *          ui4_data    References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK                        Success.
 *          SMR_INTERNAL_ERROR            Fail.
 *          SMR_CAN_NOT_OP_IN_THIS_STATE  Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 sess_gnrc_prc_strm_started( 
    HANDLE_T        h_sess,
    HANDLE_T        h_stream,
    SM_SESS_GNRC_T* pt_sess_obj,
    SM_EVN_TYPE_T   e_event,
    UINT32          ui4_data )
{
    sm_gs_send_msg_do_event(
        SM_EVN_SESS_STRM_PLAYED,
        h_sess,
        (UINT32)h_stream );

    if ( sess_gnrc_is_all_stream_started(pt_sess_obj) )
    {
        if ( pt_sess_obj->t_base.e_sess_cond != SM_SESS_COND_STARTED )
        {
            /* all streams are in the same state, notify session state machine */
            sm_gs_send_msg_do_event(
                SM_EVN_SESS_STRM_ALL_PLAYED,
                h_sess,
                0 );
        }
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sess_gnrc_prc_strm_closed
 *
 * Description: <function description>
 *
 * Inputs:  h_sess      References.
 *          h_stream    References.
 *          pt_sess_obj References.
 *          e_event     References.
 *          ui4_data    References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sess_gnrc_prc_strm_closed( HANDLE_T        h_sess,
                                 HANDLE_T        h_stream,
                                 SM_SESS_GNRC_T* pt_sess_obj,
                                 SM_EVN_TYPE_T   e_event,
                                 UINT32          ui4_data )
{
    INT32                         i4_ret;
    SIZE_T                        z_size = 0;

    sm_gs_send_msg_do_event(
        SM_EVN_SESS_STRM_CLOSED,
        h_sess,
        (UINT32)h_stream );

    i4_ret = sess_gnrc_remove_stream( h_sess, pt_sess_obj, h_stream );
    if ( i4_ret != SMR_OK )
    {
        SM_GS_DBG_ERR(( "[SM] sess_gnrc_remove_stream() fail return %d\r\n",
                        i4_ret ));
        SM_ASSERT( 0 );
        return SMR_INTERNAL_ERROR;    
    }
    
    hash_lock( pt_sess_obj->t_base.h_strm_ctrl_tbl );
    
    i4_ret = hash_get_size( pt_sess_obj->t_base.h_strm_ctrl_tbl, &z_size );
    
    hash_unlock( pt_sess_obj->t_base.h_strm_ctrl_tbl );
    
    if ( i4_ret != HASH_OK )
    {
        return SMR_INTERNAL_ERROR;
    }

    if ( z_size == 0 )
    {
        /* all streams are closed, notify session state machine */
        sm_gs_send_msg_do_event(
            SM_EVN_SESS_STRM_ALL_CLOSED,
            h_sess,
            0 );
    }
    else
    {
        if ( sess_gnrc_is_all_stream_started(pt_sess_obj) )
        {
            sm_gs_send_msg_do_event(
                SM_EVN_SESS_STRM_ALL_PLAYED,
                h_sess,
                0 );
        }
        else if ( sess_gnrc_is_all_stream_opened(pt_sess_obj) )
        {
            sm_gs_send_msg_do_event(
                SM_EVN_SESS_STRM_ALL_OPENED,
                h_sess,
                0 );
        }
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sess_gnrc_prc_pcr_msg
 *
 * Description: <function description>
 *
 * Inputs:  pt_pcr_msg References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sess_gnrc_prc_pcr_msg( 
    HANDLE_T                     h_sess,
    SM_SESS_GNRC_T*              pt_sess_obj,
    SM_SESS_GNRC_MSG_RM_NFY_T*   pt_pcr_msg )
{
    RM_COND_T                     e_new_cond;
    RM_COND_T                     e_old_cond;
    
#ifdef MW_TV_AV_BYPASS_SUPPORT
    if ( ( pt_sess_obj->t_pcr_descr.t_info.ui1_in_port != pt_pcr_msg->ui1_port &&
           pt_sess_obj->t_pcr_descr.t_info.ui1_in_port != ANY_PORT_NUM ) ||
         pt_sess_obj->t_pcr_descr.h_comp != pt_pcr_msg->h_comp )
#else
    if ( pt_sess_obj->t_pcr_descr.t_info.ui1_in_port != pt_pcr_msg->ui1_port ||
         pt_sess_obj->t_pcr_descr.h_comp != pt_pcr_msg->h_comp )
#endif
    {
        SM_GS_DBG_ERR(( 
            "[SM] ERROR in sess_gnrc_prc_pcr_msg(), input port(%d,%d) or "
            "PCR handle(0x%.8X, 0x%.8X) unmatch!\r\n",
            pt_sess_obj->t_pcr_descr.t_info.ui1_in_port,
            pt_pcr_msg->ui1_port,
            pt_sess_obj->t_pcr_descr.h_comp,
            pt_pcr_msg->h_comp ));

        sm_state_on_event( 
            &pt_sess_obj->t_state,
            SM_EVN_SESS_DRV_ERR,
            (VOID*)pt_sess_obj,
            NULL,
            NULL );
        return;
    }

    /* Filter out all set/get operation notify.  */
    e_new_cond = rm_cond_chg( pt_pcr_msg->e_nfy_cond, 
                          RM_COND_IGNORE, 
                          COND_GRP_OPERATION );
    if( e_new_cond != RM_COND_NO_CHG )
    {
        SM_GS_DBG_INFO(( 
            "[SM] GS, Filter out RM operation condition(0x%.8x) notify.\r\n",
            pt_pcr_msg->e_nfy_cond ));
        return;
    }
    
    if ( pt_pcr_msg->b_should_update_comp_descr &&
         pt_sess_obj->t_pcr_descr.e_comp_cond != RM_COND_CLOSED )
    {
        e_old_cond = pt_sess_obj->t_pcr_descr.e_comp_cond;
        pt_sess_obj->t_pcr_descr.e_comp_cond_org = e_old_cond;
        pt_sess_obj->t_pcr_descr.e_comp_cond = pt_pcr_msg->e_nfy_cond;
    }
    else
    {
        e_old_cond = pt_sess_obj->t_pcr_descr.e_comp_cond_org;
    }
    
    e_new_cond = rm_cond_chg( pt_pcr_msg->e_nfy_cond, 
                              e_old_cond,
                              COND_GRP_OBJ_STATE );
    if ( e_new_cond == RM_COND_OPENED )
    {
        sm_state_on_event( 
            &pt_sess_obj->t_state,
            SM_EVN_SESS_PCR_OPENED,
            (VOID*)pt_sess_obj,
            NULL,
            NULL );
        return;
    }
    else if ( e_new_cond == RM_COND_CLOSED )
    {
        sm_state_on_event( 
            &pt_sess_obj->t_state,
            SM_EVN_SESS_PCR_CLOSED,
            (VOID*)pt_sess_obj,
            NULL,
            NULL );
        return;
    }
    
    e_new_cond = rm_cond_chg( pt_pcr_msg->e_nfy_cond, 
                              e_old_cond, 
                              COND_GRP_CONN_STATE );
    if ( e_new_cond == RM_COND_CONNECTED || e_new_cond == RM_COND_HARD_WIRED )
    {
        sm_state_on_event( 
            &pt_sess_obj->t_state,
            SM_EVN_SESS_PCR_CONNECTED,
            (VOID*)pt_sess_obj,
            NULL,
            NULL );
        return;
    }
    else if ( e_new_cond == RM_COND_DISCONNECTED )
    {
        sm_state_on_event( 
            &pt_sess_obj->t_state,
            SM_EVN_SESS_PCR_DISCONNECTED,
            (VOID*)pt_sess_obj,
            NULL,
            NULL );
        return;
    }
}

/*-----------------------------------------------------------------------------
 * Name: sess_gnrc_prc_stc_msg
 *
 * Description: <function description>
 *
 * Inputs:  pt_stc_msg References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sess_gnrc_prc_stc_msg( 
    HANDLE_T                     h_sess,
    SM_SESS_GNRC_T*              pt_sess_obj,
    SM_SESS_GNRC_MSG_RM_NFY_T*   pt_stc_msg )
{
    RM_COND_T                     e_new_cond;
    RM_COND_T                     e_old_cond;
    
      if ( ( pt_sess_obj->t_stc_descr.t_info.ui1_in_port != pt_stc_msg->ui1_port &&
            pt_sess_obj->t_stc_descr.t_info.ui1_in_port != ANY_PORT_NUM )
           ||  pt_sess_obj->t_stc_descr.h_comp != pt_stc_msg->h_comp )
    {
        SM_GS_DBG_ERR(( 
            "[SM] ERROR in sess_gnrc_prc_stc_msg(), input port(%d,%d) or "
            "STC handle(0x%.8X, 0x%.8X) unmatch!\r\n",
            pt_sess_obj->t_stc_descr.t_info.ui1_in_port,
            pt_stc_msg->ui1_port,
            pt_sess_obj->t_stc_descr.h_comp,
            pt_stc_msg->h_comp ));

        sm_state_on_event( 
            &pt_sess_obj->t_state,
            SM_EVN_SESS_DRV_ERR,
            (VOID*)pt_sess_obj,
            NULL,
            NULL );
        return;
    }

    /* Filter out all set/get operation notify.  */
    e_new_cond = rm_cond_chg( pt_stc_msg->e_nfy_cond, 
                          RM_COND_IGNORE, 
                          COND_GRP_OPERATION );
    if( e_new_cond != RM_COND_NO_CHG )
    {
        SM_GS_DBG_INFO(( 
            "[SM] GS, Filter out RM operation condition(0x%.8x) notify.\r\n",
            pt_stc_msg->e_nfy_cond ));
        return;
    }
    
    if ( pt_stc_msg->b_should_update_comp_descr &&
         pt_sess_obj->t_stc_descr.e_comp_cond != RM_COND_CLOSED )
    {
        e_old_cond = pt_sess_obj->t_stc_descr.e_comp_cond;
        pt_sess_obj->t_stc_descr.e_comp_cond_org = e_old_cond;
        pt_sess_obj->t_stc_descr.e_comp_cond = pt_stc_msg->e_nfy_cond;
    }
    else
    {
        e_old_cond = pt_sess_obj->t_stc_descr.e_comp_cond_org;
    }
    
    e_new_cond = rm_cond_chg( pt_stc_msg->e_nfy_cond, 
                              e_old_cond,
                              COND_GRP_OBJ_STATE );
    if ( e_new_cond == RM_COND_OPENED )
    {
        sm_state_on_event( 
            &pt_sess_obj->t_state,
            SM_EVN_SESS_STC_OPENED,
            (VOID*)pt_sess_obj,
            NULL,
            NULL );
        return;
    }
    else if ( e_new_cond == RM_COND_CLOSED )
    {
        sm_state_on_event( 
            &pt_sess_obj->t_state,
            SM_EVN_SESS_STC_CLOSED,
            (VOID*)pt_sess_obj,
            NULL,
            NULL );
        return;
    }
}

/*-----------------------------------------------------------------------------
 * Name: sess_gnrc_prc_dmx_msg
 *
 * Description: <function description>
 *
 * Inputs:  pt_dmx_msg References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sess_gnrc_prc_dmx_msg( 
    HANDLE_T                     h_sess,
    SM_SESS_GNRC_T*              pt_sess_obj,
    SM_SESS_GNRC_MSG_RM_NFY_T*   pt_dmx_msg )
{
    RM_COND_T                     e_new_cond;
    RM_COND_T                     e_old_cond;
    
    if ( ( pt_sess_obj->t_dmx_descr.t_info.ui1_in_port != pt_dmx_msg->ui1_port && 
           pt_sess_obj->t_dmx_descr.t_info.ui1_in_port != ANY_PORT_NUM ) ||
         pt_sess_obj->t_dmx_descr.h_comp != pt_dmx_msg->h_comp )
    {
        SM_GS_DBG_ERR(( 
            "[SM] ERROR in sess_gnrc_prc_dmx_msg(), input port(%d,%d) or "
            "DMX handle(0x%.8X, 0x%.8X) unmatch!\r\n",
            pt_sess_obj->t_dmx_descr.t_info.ui1_in_port,
            pt_dmx_msg->ui1_port,
            pt_sess_obj->t_dmx_descr.h_comp,
            pt_dmx_msg->h_comp ));

        sm_state_on_event( 
            &pt_sess_obj->t_state,
            SM_EVN_SESS_DRV_ERR,
            (VOID*)pt_sess_obj,
            NULL,
            NULL );
        return;
    }

    /* Filter out all set/get operation notify.  */
    e_new_cond = rm_cond_chg( pt_dmx_msg->e_nfy_cond, 
                          RM_COND_IGNORE, 
                          COND_GRP_OPERATION );
    if( e_new_cond != RM_COND_NO_CHG )
    {
        SM_GS_DBG_INFO(( 
            "[SM] GS, Filter out RM operation condition(0x%.8x) notify.\r\n",
            pt_dmx_msg->e_nfy_cond ));
        return;
    }
    
    if ( pt_dmx_msg->b_should_update_comp_descr &&
         pt_sess_obj->t_dmx_descr.e_comp_cond != RM_COND_CLOSED )
    {
        e_old_cond = pt_sess_obj->t_dmx_descr.e_comp_cond;
        pt_sess_obj->t_dmx_descr.e_comp_cond_org = e_old_cond;
        pt_sess_obj->t_dmx_descr.e_comp_cond = pt_dmx_msg->e_nfy_cond;
    }
    else
    {
        e_old_cond = pt_sess_obj->t_dmx_descr.e_comp_cond_org;
    }
    
    e_new_cond = rm_cond_chg( pt_dmx_msg->e_nfy_cond, 
                              e_old_cond,
                              COND_GRP_OBJ_STATE );
    if ( e_new_cond == RM_COND_OPENED )
    {
        sm_state_on_event( 
            &pt_sess_obj->t_state,
            SM_EVN_SESS_DMX_OPENED,
            (VOID*)pt_sess_obj,
            NULL,
            NULL );
        return;
    }
    else if ( e_new_cond == RM_COND_CLOSED )
    {
        sm_state_on_event( 
            &pt_sess_obj->t_state,
            SM_EVN_SESS_DMX_CLOSED,
            (VOID*)pt_sess_obj,
            NULL,
            NULL );
        return;
    }
}

/*-----------------------------------------------------------------------------
 * Name: sess_gnrc_prc_hdlr_msg
 *
 * Description: <function description>
 *
 * Inputs:  h_stream      References.
 *          e_cond_type   References.
 *          pv_sm_nfy_tag References.
 *          e_event       References.
 *          ui4_data      References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sess_gnrc_prc_hdlr_msg( HANDLE_T        h_stream,
                             SM_COND_T       e_cond_type,
                             VOID*           pv_sm_nfy_tag,
                             SM_EVN_TYPE_T   e_event,
                             UINT32          ui4_data )
{
    INT32                         i4_ret = SMR_OK;
    SM_SESS_GNRC_T*               pt_sess_obj = NULL;
    SM_STREAM_T*                  pt_strm_obj = NULL;
    VOID*                         pv_temp_tag = NULL;
	x_stream_nfy_fct              pf_temp_nfy = NULL;
    pt_strm_obj = (SM_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_strm_obj )
    {
        /* The stream is already destroyed, noop */
        return;
    }
    
    pt_sess_obj = (SM_SESS_GNRC_T*)sm_get_sess_obj( pt_strm_obj->h_sess );
    if ( !pt_sess_obj )
    {
        SM_ABORT( SM_DBG_ABRT_INV_SESS_OBJ );
    }
    
    SM_LOCK( pt_sess_obj->t_base.h_lock );
    
    if ( x_sema_lock( pt_strm_obj->h_lock, X_SEMA_OPTION_WAIT ) != OSR_OK )
    {
        /* if stream lock fail, assume the stream is already closed */
        SM_UNLOCK( pt_sess_obj->t_base.h_lock );
        return;
    }
   
    if ( sess_gnrc_is_valid_stream( pt_strm_obj->h_sess,
                                    pt_sess_obj,
                                    h_stream ) )
    {
        /* notify stream event to application */
        if(e_cond_type != SM_COND_CLOSED)
        {
            pt_strm_obj->pf_nfy( 
                h_stream, 
                e_cond_type, 
                pt_strm_obj->pv_nfy_tag, 
                e_event,
                ui4_data );    
        }
        
        switch ( e_cond_type )
        {
        case SM_COND_OPENED:
            i4_ret = sess_gnrc_prc_strm_opened( 
                pt_strm_obj->h_sess,
                h_stream, 
                pt_sess_obj,
                e_event,
                ui4_data );
            break;
            
        case SM_COND_STARTED:
            i4_ret = sess_gnrc_prc_strm_started( 
                pt_strm_obj->h_sess,
                h_stream, 
                pt_sess_obj,
                e_event,
                ui4_data );
            break;
    
        case SM_COND_CLOSED:
            i4_ret = sess_gnrc_prc_strm_closed( 
                pt_strm_obj->h_sess,
                h_stream, 
                pt_sess_obj,
                e_event,
                ui4_data );
            break;
        
        case SM_COND_ERROR:
            /* close the stream if the stream is in error state */
            i4_ret = sm_sess_gnrc_close( h_stream );
            break;
            
        case SM_COND_IGNORE:
        case SM_COND_OPENING:
        case SM_COND_STARTING:
        case SM_COND_STOPPING:
        case SM_COND_CLOSING:    
        default:
            /* break if $e_cond_type is a temporary state. */
            break;
        }
        
        if ( i4_ret != SMR_OK )
        {
            SM_GS_DBG_ERR(( 
                "[SM] sess_gnrc_prc_hdlr_msg() fail return %d, NFY_STRM_COND(%u), STRM_COND(%u), SESS_COND(%u)\r\n",
                i4_ret, 
                e_cond_type,
                pt_strm_obj->e_condition,
                pt_sess_obj->t_base.e_sess_cond ));
        }
    }
    
    if ( e_cond_type == SM_COND_CLOSED )
    {
        /*
         * It's session's responsibility to free the stream handle !!
         */ 
        pf_temp_nfy=pt_strm_obj->pf_nfy;
        pv_temp_tag = pt_strm_obj->pv_nfy_tag;
        SM_UNLOCK( pt_strm_obj->h_lock );
       
        pf_temp_nfy( 
            h_stream, 
            e_cond_type, 
            pv_temp_tag, 
            e_event,
            ui4_data );    
        pv_temp_tag = NULL;
		pf_temp_nfy= NULL;
		
    }
    else
    {
        SM_UNLOCK( pt_strm_obj->h_lock );
    }
    
    SM_UNLOCK( pt_sess_obj->t_base.h_lock );
    
    if ( e_cond_type == SM_COND_CLOSED )
    {
        if ( sm_stream_lock() != SMR_OK )
        {
            SM_ABORT(SM_DBG_ABRT_CANNOT_LOCK_SEMAPHORE);
        }  
        handle_free( h_stream, FALSE );
        sm_stream_unlock();
    }
    
    return;
}

/*-----------------------------------------------------------------------------
 * Name: sess_gnrc_prc_scdb_msg
 *
 * Description: <function description>
 *
 * Inputs:  h_sess      References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sess_gnrc_prc_scdb_msg( 
    HANDLE_T                       h_sess,
    SM_SESS_GNRC_T*                pt_sess_obj,
    const SM_SESS_GNRC_MSG_SCDB_T* pt_scdb_msg )
{
    INT32          i4_ret;
    SCDB_COND_T    e_scdb_cond = pt_scdb_msg->e_scdb_cond;
    UINT32         ui4_reason = pt_scdb_msg->ui4_reason;
    /*UINT32         ui4_data = pt_scdb_msg->ui4_data;*/
    SCDB_COMMON_T  t_scdb_cmmn_old;
    
    if ( e_scdb_cond == SCDB_UPDATING )
    {
        /* keep using old data */
        return;
    }
    else if ( e_scdb_cond == SCDB_UPDATED )
    {
        t_scdb_cmmn_old = pt_sess_obj->t_scdb_cmmn;
        
        i4_ret = x_scdb_get_common( pt_sess_obj->t_base.h_scdb, 
                                    &pt_sess_obj->t_scdb_cmmn );
        if ( i4_ret != SCDBR_OK )
        {
            sm_gs_send_msg_do_event( 
                SM_EVN_SESS_SCDB_CLOSED,
                h_sess,
                0 );
            return;
        }
        
        if ( ui4_reason & SCDB_RECORD_ADD )
        {
            /* do nothing, left it to be processed by stream layer */
        }
        if ( ui4_reason & SCDB_RECORD_DEL )
        {
            /* do nothing, left it to be processed by stream layer */
        }
        if ( ui4_reason & SCDB_RECORD_MOD )
        {
            /* do nothing, left it to be processed by stream layer */
        }
        if ( ui4_reason & SCDB_COMMON_MOD )
        {
            if ( pt_sess_obj->t_scdb_cmmn.e_data_fmt != t_scdb_cmmn_old.e_data_fmt )
            {
                SM_GS_DBG_INFO(( 
                    "[SM] SCDB update, t_scdb_cmmn.e_data_fmt = %u\r\n",
                    (UINT32)pt_sess_obj->t_scdb_cmmn.e_data_fmt ));
            }
            if ( pt_sess_obj->t_scdb_cmmn.e_feed_mode != t_scdb_cmmn_old.e_feed_mode )
            {
                SM_GS_DBG_INFO(( 
                    "[SM] SCDB update, t_scdb_cmmn.e_feed_mode = %u\r\n",
                    (UINT32)pt_sess_obj->t_scdb_cmmn.e_feed_mode ));
            }
            if ( pt_sess_obj->t_scdb_cmmn.e_data_fmt == DATA_FMT_MPEG_2 &&
                 pt_sess_obj->t_scdb_cmmn.u_data.t_mpeg.ui2_pcr != 
                 t_scdb_cmmn_old.u_data.t_mpeg.ui2_pcr )
            {
                SM_GS_DBG_INFO(( 
                    "[SM] SCDB update, PCR PID changed to 0x%.8X\r\n",
                    (UINT32)pt_sess_obj->t_scdb_cmmn.u_data.t_mpeg.ui2_pcr ));
#ifdef MW_TV_AV_BYPASS_SUPPORT
                if( pt_sess_obj->b_pcr_owner )
#endif
                {
                    sm_sess_gnrc_pcr_set_pid( 
                             h_sess, 
                             pt_sess_obj );
                }
            }
        }
        if ( ui4_reason & SCDB_REASON_UNK )
        {
            /* do nothing */
        }
    }
    else if ( e_scdb_cond == SCDB_CLOSED )
    {        
        /* if any problem occur in the future because of this mark ,the upper layer should check and resolve the issue*/
        #if 0
        pt_sess_obj->t_base.h_scdb = NULL_HANDLE;
        sm_gs_send_msg_do_event( 
            SM_EVN_SESS_SCDB_CLOSED,
            h_sess,
            0 );
        #endif
        return;
    }
    else
    {
        /* improper SCDB state */
        SM_GS_DBG_INFO(( "[SM] %s:%d Improper SCDB state!\r\n", __FILE__, __LINE__ ));
        SM_ASSERT( 0 );
    }
}

/*-----------------------------------------------------------------------------
 * Name: sess_gnrc_prc_scdb_msg
 *
 * Description: <function description>
 *
 * Inputs:  h_sess      References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sess_gnrc_prc_do_event_msg( 
    HANDLE_T                             h_sess,
    SM_SESS_GNRC_T*                      pt_sess_obj,
    const SM_SESS_GNRC_MSG_DO_EVENT_T*   pt_do_event_msg )
{
    sm_state_on_event( &pt_sess_obj->t_state,
                       pt_do_event_msg->ui4_event,
                       (VOID*)pt_sess_obj,
                       (VOID*)pt_do_event_msg->ui4_data,
                       NULL );
}

/*-----------------------------------------------------------------------------
 * Name: sess_gnrc_add_stream
 *
 * Description: <function description>
 *
 * Inputs:  pt_sess_obj References.
 *          h_stream    References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 sess_gnrc_add_stream( 
    HANDLE_T        h_sess,
    SM_SESS_GNRC_T* pt_sess_obj,
    HANDLE_T        h_stream )
{
    INT32                         i4_ret;
    
    hash_lock( pt_sess_obj->t_base.h_strm_ctrl_tbl );
    
    i4_ret = hash_add_obj( pt_sess_obj->t_base.h_strm_ctrl_tbl,
                           (INT8*)&h_stream,
                           sizeof(h_stream),
                           (VOID*)h_stream,
                           NULL );
    
    hash_unlock( pt_sess_obj->t_base.h_strm_ctrl_tbl );
    
    if ( i4_ret != HASH_OK )
    {
        return SMR_INTERNAL_ERROR;        
    }
    
    sm_gs_send_msg_do_event( 
        SM_EVN_SESS_STRM_ADD,
        h_sess,
        (UINT32)h_stream );
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sess_gnrc_is_valid_stream
 *
 * Description: <function description>
 *
 * Inputs:  pt_sess_obj References.
 *          h_stream    References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
static BOOL sess_gnrc_is_valid_stream( 
    HANDLE_T        h_sess,
    SM_SESS_GNRC_T* pt_sess_obj,
    HANDLE_T        h_stream )
{
    INT32                         i4_ret;
    HANDLE_T                      h_strm_hdl = NULL_HANDLE;
    
    hash_lock( pt_sess_obj->t_base.h_strm_ctrl_tbl );
    
    i4_ret = hash_get_obj( pt_sess_obj->t_base.h_strm_ctrl_tbl,
                           (INT8*)&h_stream,
                           sizeof(h_stream),
                           (VOID*)&h_strm_hdl );
    
    hash_unlock( pt_sess_obj->t_base.h_strm_ctrl_tbl );
    
    if ( i4_ret != HASH_OK )
    {
        return FALSE;        
    }
    
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: sess_gnrc_remove_stream
 *
 * Description: <function description>
 *
 * Inputs:  pt_sess_obj References.
 *          h_stream    References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 sess_gnrc_remove_stream( 
    HANDLE_T        h_sess,
    SM_SESS_GNRC_T* pt_sess_obj,
    HANDLE_T        h_stream )
{
    INT32                         i4_ret;
    
    hash_lock( pt_sess_obj->t_base.h_strm_ctrl_tbl );
    
    i4_ret = hash_remove_obj( pt_sess_obj->t_base.h_strm_ctrl_tbl,
                              (INT8*)&h_stream,
                              sizeof(h_stream),
                              NULL );
    
    hash_unlock( pt_sess_obj->t_base.h_strm_ctrl_tbl );
    
    if ( i4_ret != HASH_OK )
    {
        return SMR_INTERNAL_ERROR;        
    }

    sm_gs_send_msg_do_event( 
        SM_EVN_SESS_STRM_DEL,
        h_sess,
        (UINT32)h_stream );
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sess_gnrc_sess_set
 *
 * Description: <function description>
 *
 * Inputs:  h_sess            References.
 *          pt_sess_obj       References.
 *          e_set_type        References.
 *          pv_set_info       References.
 *          ui4_set_info_size References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK               Success.
 *          SMR_INV_SET_INFO     Fail.
 *          SMR_ALREADY_PLAYED   Fail.
 *          SMR_INTERNAL_ERROR   Fail.
 *          SMR_ALREADY_STOPPED  Fail.
 *          SMR_NOT_SUPPORT      Fail.
 *          SMR_INV_HANDLE       Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 sess_gnrc_sess_set( 
    HANDLE_T           h_sess,
    SM_SESS_GNRC_T*    pt_sess_obj,
    SM_SET_TYPE_T      e_set_type,
    VOID*              pv_set_info,
    SIZE_T             ui4_set_info_size )
{
    switch( e_set_type )
    {
    case SM_SESS_SET_TYPE_PLAY:
        if ( pt_sess_obj->t_base.e_sess_cond == SM_SESS_COND_STARTED &&
             pt_sess_obj->t_base.i4_speed == (INT32)pv_set_info )
        {
            return SMR_ALREADY_PLAYED;
        }
        else if ( pt_sess_obj->t_base.e_sess_cond == SM_SESS_COND_STARTED &&
                  pt_sess_obj->t_base.i4_speed != (INT32)pv_set_info )
        {
            /* Change Speed for all streams */
            pt_sess_obj->t_base.i4_speed = (INT32)pv_set_info;
            hash_lock( pt_sess_obj->t_base.h_strm_ctrl_tbl );
        
            hash_parse( pt_sess_obj->t_base.h_strm_ctrl_tbl,
                        _sm_gs_chg_speed_hash_cb,
                        (VOID*)pt_sess_obj );
        
            hash_unlock( pt_sess_obj->t_base.h_strm_ctrl_tbl );

            return SMR_OK;
        }
        
        pt_sess_obj->t_base.i4_speed = (INT32)pv_set_info;
        sm_gs_send_msg_do_event(
            SM_EVN_SESS_APP_PLAY_REQ,
            h_sess,
            0 );
        break;
        
    case SM_SESS_SET_TYPE_STOP:
        if ( pt_sess_obj->t_base.e_sess_cond == SM_SESS_COND_OPENED )
        {
            return SMR_ALREADY_STOPPED;    
        }
        
        sm_gs_send_msg_do_event(
            SM_EVN_SESS_APP_STOP_REQ,
            h_sess,
            0 );
        break;
        
    case SM_SESS_GNRC_SET_TYPE_SCART_OUT:
        if (ui4_set_info_size < sizeof(SM_SESS_GNRC_SCART_OUT_T*))
        {
            return SMR_INV_ARG;
        }

        return sm_sess_gnrc_scart_out_set(
                        h_sess,
                        pt_sess_obj,
                        (SM_SESS_GNRC_SCART_OUT_T*)pv_set_info);
    
    case SM_SESS_GNRC_SET_TYPE_AV_SYNC:
        if ( ui4_set_info_size != sizeof(SM_SESS_GNRC_AV_SYNC_INFO_T) ||
             !pv_set_info )
        {
            return SMR_INV_ARG;
        }
        return sm_sess_gnrc_pcr_apply_av_sync( 
                        h_sess,
                        pt_sess_obj,
                        (SM_SESS_GNRC_AV_SYNC_INFO_T*)pv_set_info);
        
    case SM_SESS_GNRC_SET_TYPE_AV_SYNC_EX:
        {
            STC_AV_SYNC_MODE_T e_stc_av_sync_mode ;
            if ( ui4_set_info_size != sizeof(SM_SESS_STC_AV_SYNC_MODE_T) ||
                 !pv_set_info )
            {
                return SMR_INV_ARG;
            }
            if ( pt_sess_obj->t_stc_descr.h_comp == NULL_HANDLE ||
                 pt_sess_obj->t_stc_descr.e_comp_cond == RM_COND_CLOSED )
            {
               return SMR_INV_HANDLE;
            }
            
            e_stc_av_sync_mode = sess_gnrc_stc_mode_2_drv(*( (SM_SESS_STC_AV_SYNC_MODE_T*)pv_set_info) );
            return sm_sess_gnrc_stc_av_sync_mode( pt_sess_obj , 
                                                                        e_stc_av_sync_mode );
         }

    case SM_SESS_GNRC_SET_TYPE_STC_CTRL:
        {
            BOOL b_start_stc = (BOOL)((UINT32)pv_set_info);
            STC_CTRL_TYPE_T  t_stc_ctrl;
            INT32 i4_ret;

            if(b_start_stc)
            {
                t_stc_ctrl = STC_CTRL_START;
            }
            else
            {
                t_stc_ctrl = STC_CTRL_STOP;
            }
            
            i4_ret = sm_sess_gnrc_stc_ctrl( pt_sess_obj, t_stc_ctrl );
            if ( i4_ret != SMR_OK )
            {
                SM_GS_DBG_INFO(( 
                    "[SM] SESS, sm_sess_gnrc_stc_ctrl(0x%.8X) error, return %d!\r\n",
                    (UINT32)pt_sess_obj->t_stc_descr.h_comp, i4_ret ));
            
                sm_gs_send_msg_do_event(
                    SM_EVN_SESS_DRV_ERR,
                    h_sess,
                    0 );
                return SMR_INTERNAL_ERROR;
            }
        }
        break;   

#ifdef ENABLE_MULTIMEDIA
    case SM_SESS_GNRC_SET_TYPE_3D_PROPERTY:
        {
            if ( PMR_OK != pm_set( pt_sess_obj->t_base.h_pm_service,
                                   PM_SET_3D_PROPERTY,
                                   pv_set_info ) )
            {
                return SMR_INTERNAL_ERROR;
            }            
        }
        break;

    case SM_SESS_GNRC_SET_TYPE_ADD_RANGE:
        if (ui4_set_info_size < sizeof(MM_RANGE_INFO_T*))
        {
            return SMR_INV_ARG;
        }
        if ( PMR_OK != pm_set( pt_sess_obj->t_base.h_pm_service,
                               PM_SET_ADD_RANGE,
                               pv_set_info) )
        {
            return SMR_INTERNAL_ERROR;
        }
        break;

    case SM_SESS_GNRC_SET_TYPE_DEL_RANGE:
        if (ui4_set_info_size < sizeof(VOID*))
        {
            return SMR_INV_ARG;
        }
        if ( PMR_OK != pm_set( pt_sess_obj->t_base.h_pm_service,
                               PM_SET_DEL_RANGE,
                               pv_set_info) )
        {
            return SMR_INTERNAL_ERROR;
        }
        break;

    /*ab_repeat_driver*/
    case SM_SESS_GNRC_SET_TYPE_AB_POSITION:
        {
            if ( PMR_OK != pm_set( pt_sess_obj->t_base.h_pm_service,
                                   PM_SET_AB_POS,
                                   pv_set_info ) )
            {
                return SMR_INTERNAL_ERROR;
            }            
        }
        break;
    case SM_SESS_GNRC_SET_TYPE_SPEED:
        {
            INT32                       i4_ret;           
            SIZE_T                      ui4_get_info_size = sizeof(SM_SESS_GNRC_UOP_CAP_INFO_T);
            SM_SESS_GNRC_UOP_CAP_INFO_T t_uop_cap_info;
			MM_PLAY_MODE_PL_T *prPlayMode=(MM_PLAY_MODE_PL_T *)pv_set_info;
            MM_SPEED_TYPE_T e_speed = prPlayMode->eSpeed;

            if ( ui4_set_info_size < sizeof(MM_SPEED_TYPE_T) )
            {
                return SMR_INV_ARG;
            }
            t_uop_cap_info.e_uop = _SM_SESS_GNRC_UOP_TRICK; 
            t_uop_cap_info.u.t_speed = e_speed;  
            if ( PMR_OK != pm_get( pt_sess_obj->t_base.h_pm_service,
                                   PM_GET_UOP_CAP,
                                   (VOID*)&t_uop_cap_info,
                                   &ui4_get_info_size) )
            {
                return SMR_INTERNAL_ERROR;
            }
            if (!t_uop_cap_info.b_is_allowed)
            {
                return SMR_NOT_ALLOW;        
            }

            if ( e_speed == MM_SPEED_TYPE_PAUSE ||
                 e_speed == MM_SPEED_TYPE_STEP ||
                 e_speed == MM_SPEED_TYPE_STEP_REVERSE )
            {
                i4_ret = SMR_OK;
            }
            else if ( e_speed == MM_SPEED_TYPE_FORWARD_1X )
            {
                i4_ret = sm_sess_gnrc_stc_av_sync_mode( pt_sess_obj , STC_AV_SYNC_MODE_AUDIO_MASTER );
            }
            else if (( e_speed == MM_SPEED_TYPE_FORWARD_2X ) ||
                        (( e_speed >= MM_SPEED_TYPE_FORWARD_1_DIV_32X ) &&
                        ( e_speed <= MM_SPEED_TYPE_FORWARD_1_DIV_2X )))
            {
                i4_ret = sm_sess_gnrc_stc_av_sync_mode( pt_sess_obj , STC_AV_SYNC_MODE_VIDEO_MASTER );
            }          
            else /*speed over 2X*/
            {
                i4_ret = sm_sess_gnrc_stc_av_sync_mode( pt_sess_obj , STC_AV_SYNC_MODE_FREE_RUN );
            }
            if ( i4_ret != SMR_OK )
            {
                SM_GS_DBG_ERR(( 
                    "[SM] SESS, sm_sess_gnrc_stc_av_sync_mode(0x%.8X) error, return %d!\r\n",
                    (UINT32)pt_sess_obj->t_stc_descr.h_comp, i4_ret ));
            
                sm_gs_send_msg_do_event(
                    SM_EVN_SESS_DRV_ERR,
                    h_sess,
                    0 );
                return SMR_INTERNAL_ERROR;
            }
            
            /*if ( pt_sess_obj->e_mm_speed == e_speed )
            {
                break;
            }*/
                
            if ( PMR_OK != pm_set( pt_sess_obj->t_base.h_pm_service,
                                   PM_SET_SPEED,
                                   pv_set_info ) )
            {
                return SMR_INTERNAL_ERROR;
            }            
            pt_sess_obj->e_mm_speed = e_speed;
        }
        break;

    case SM_SESS_GNRC_SET_TYPE_SPEED_WITH_PTS:
        {
            INT32               i4_ret;           
            MM_SPEED_TYPE_T     e_speed = (MM_SPEED_TYPE_T)(UINT32)pv_set_info;

            if ( ui4_set_info_size < sizeof(MM_SPEED_TYPE_T) )
            {
                return SMR_INV_ARG;
            }

            if ( e_speed == MM_SPEED_TYPE_PAUSE ||
                 e_speed == MM_SPEED_TYPE_STEP  ||
                 e_speed == MM_SPEED_TYPE_STEP_REVERSE )
            {
                i4_ret = SMR_OK;
            }
            else if ( e_speed == MM_SPEED_TYPE_FORWARD_1X )
            {
                i4_ret = sm_sess_gnrc_stc_av_sync_mode( pt_sess_obj , STC_AV_SYNC_MODE_AUDIO_MASTER );
            }
            else if (( e_speed == MM_SPEED_TYPE_FORWARD_2X ) ||
                     (( e_speed >= MM_SPEED_TYPE_FORWARD_1_DIV_32X ) &&
                      ( e_speed <= MM_SPEED_TYPE_FORWARD_1_DIV_2X )))
            {
                i4_ret = sm_sess_gnrc_stc_av_sync_mode( pt_sess_obj , STC_AV_SYNC_MODE_VIDEO_MASTER );
            }          
            else /*speed over 2X*/
            {
                i4_ret = sm_sess_gnrc_stc_av_sync_mode( pt_sess_obj , STC_AV_SYNC_MODE_FREE_RUN );
            }
            if ( i4_ret != SMR_OK )
            {
                SM_GS_DBG_ERR(( 
                    "[SM] SESS, sm_sess_gnrc_stc_av_sync_mode(0x%.8X) error, return %d!\r\n",
                    (UINT32)pt_sess_obj->t_stc_descr.h_comp, i4_ret ));
            
                sm_gs_send_msg_do_event(
                    SM_EVN_SESS_DRV_ERR,
                    h_sess,
                    0 );
                return SMR_INTERNAL_ERROR;
            }
            
            /*
            if ( pt_sess_obj->e_mm_speed == e_speed )
            {
                break;
            }
            */
                
            if ( PMR_OK != pm_set( pt_sess_obj->t_base.h_pm_service,
                                   PM_SET_SPEED,
                                   pv_set_info ) )
            {
                return SMR_INTERNAL_ERROR;
            }            
            pt_sess_obj->e_mm_speed = e_speed;
        }
        break;

    case SM_SESS_GNRC_SET_TYPE_STC:
        {
            STC_T t_stc = (STC_T)(UINT32)pv_set_info;
            INT32 i4_ret;

            i4_ret = sm_sess_gnrc_stc_set( pt_sess_obj, t_stc );
            if ( i4_ret != SMR_OK )
            {
                SM_GS_DBG_INFO(( 
                    "[SM] SESS, sm_sess_gnrc_stc_set(0x%.8X) error, return %d!\r\n",
                    (UINT32)pt_sess_obj->t_stc_descr.h_comp, i4_ret ));
            
                sm_gs_send_msg_do_event(
                    SM_EVN_SESS_DRV_ERR,
                    h_sess,
                    0 );
                return SMR_INTERNAL_ERROR;
            }
        }
        break;

    case SM_SESS_GNRC_SET_TYPE_SEEK:
        if (ui4_set_info_size < sizeof(MM_RANGE_POS_INFO_T*))
        {
            return SMR_INV_ARG;
        }

        {
            SIZE_T                      ui4_get_info_size = sizeof(SM_SESS_GNRC_UOP_CAP_INFO_T);
            SM_SESS_GNRC_UOP_CAP_INFO_T t_uop_cap_info;
            
            t_uop_cap_info.e_uop = _SM_SESS_GNRC_UOP_SEEK; 
            if ( PMR_OK != pm_get( pt_sess_obj->t_base.h_pm_service,
                                   PM_GET_UOP_CAP,
                                   (VOID*)&t_uop_cap_info,
                                   &ui4_get_info_size) )
            {
                return SMR_INTERNAL_ERROR;
            }
            if (!t_uop_cap_info.b_is_allowed)
            {
                return SMR_NOT_ALLOW;        
            }
        }
        
        if ( PMR_OK != pm_set( pt_sess_obj->t_base.h_pm_service,
                               PM_SET_SEEK,
                               pv_set_info) )
        {
            return SMR_INTERNAL_ERROR;
        }
        break;

    case SM_SESS_GNRC_SET_TYPE_SEEK_WITH_PTS:
        if (ui4_set_info_size < sizeof(MM_RANGE_POS_INFO_T*))
        {
            return SMR_INV_ARG;
        }
       
        if ( PMR_OK != pm_set( pt_sess_obj->t_base.h_pm_service,
                               PM_SET_SEEK,
                               pv_set_info) )
        {
            return SMR_INTERNAL_ERROR;
        }
        break;

    case SM_SESS_GNRC_SET_TYPE_BLOCK_PSI:
        if (ui4_set_info_size < sizeof(BOOL))
        {
            return SMR_INV_ARG;
        }
        SM_GS_DBG_INFO(("SM_SESS_GNRC_SET_TYPE_BLOCK_PSI(%s)\r\n", pv_set_info ? "true" : "false"));
        if ( PMR_OK != pm_set( pt_sess_obj->t_base.h_pm_service,
                               PM_SET_BLOCK_PSI,
                               pv_set_info) )
        {
            return SMR_INTERNAL_ERROR;
        }
        break;

    case SM_SESS_GNRC_SET_TYPE_REC_BUF_RANGE:
        if (ui4_set_info_size < sizeof(MM_REC_BUF_RANGE_INFO_T*))
        {
            return SMR_INV_ARG;
        }
        if ( PMR_OK != pm_set( pt_sess_obj->t_base.h_pm_service,
                               PM_SET_REC_BUF_RANGE,
                               pv_set_info) )
        {
            return SMR_INTERNAL_ERROR;
        }
        break;

    case SM_SESS_GNRC_SET_TYPE_HDMVC_INFO:
    {
        if (ui4_set_info_size != sizeof(UINT32))
        {
            return SMR_INV_ARG;
        }
        if ( PMR_OK != pm_set( pt_sess_obj->t_base.h_pm_service,
                               PM_SET_HDMVC_INFO,
                               pv_set_info) )
        {
            return SMR_INTERNAL_ERROR;
        }
        break;
    }

#ifdef TIME_SHIFT_SUPPORT
    case SM_SESS_GNRC_SET_TYPE_MONITOR_LBA:
        if (ui4_set_info_size < sizeof(UINT32))
        {
            return SMR_INV_ARG;
        }
        if ( PMR_OK != pm_set( pt_sess_obj->t_base.h_pm_service,
                               PM_SET_MONITOR_LBA,
                               pv_set_info) )
        {
            return SMR_INTERNAL_ERROR;
        }
        break;
#endif /* TIME_SHIFT_SUPPORT */

#ifdef MULTIMEDIA_AUTOTEST_SUPPORT
    case SM_SESS_GNRC_SET_TYPE_MM_AUTOTEST:
    {
        if (ui4_set_info_size < sizeof(MM_AUTOTEST_INFO_T))
        {
            return SMR_INV_ARG;
        }
        if ( PMR_OK != pm_set( pt_sess_obj->t_base.h_pm_service,
                               PM_SET_AUTOTEST,
                               pv_set_info) )
        {
            return SMR_INTERNAL_ERROR;
        }
        break;
    }
#endif

#endif

    default:
        return SMR_NOT_SUPPORT;
    }
    
    return SMR_OK;    
}

/*-----------------------------------------------------------------------------
 * Name: sess_gnrc_sess_get
 *
 * Description: <function description>
 *
 * Inputs:  h_sess             References.
 *          e_get_type         References.
 *          pv_get_info        References.
 *          pui4_get_info_size References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INV_HANDLE         Fail.
 *          SMR_INSUFFICIENT_SIZE  Fail.
 *          SMR_INTERNAL_ERROR     Fail.
 *          SMR_INV_ARG            Fail.
 *          SMR_INV_GET_INFO       Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 sess_gnrc_sess_get( 
    HANDLE_T           h_sess,
    SM_SESS_GNRC_T*    pt_sess_obj,
    SM_SESS_HANDLER_T* pt_sess_hdlr_obj,
    SM_GET_TYPE_T      e_get_type,
    VOID*              pv_get_info,
    SIZE_T*            pui4_get_info_size )
{
    INT32              i4_ret;
    
    if ( !pv_get_info || !pui4_get_info_size )
    {
        return SMR_INV_GET_INFO;    
    }
    
    switch ( e_get_type )
    {
    case SM_SESS_GET_TYPE_STATE:
        if ( *pui4_get_info_size < sizeof( SM_SESS_COND_T ) )
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        *((SM_SESS_COND_T*)(pv_get_info)) = pt_sess_obj->t_base.e_sess_cond;
        *pui4_get_info_size = sizeof( SM_SESS_COND_T );
        break;
    
    case SM_SESS_GET_TYPE_HDLR_NAME:
        if ( *pui4_get_info_size <= 
             x_strlen( pt_sess_hdlr_obj->t_hdlr_desc.s_hdlr_name ) )
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        
        x_strncpy( (CHAR*)(pv_get_info), 
                   pt_sess_hdlr_obj->t_hdlr_desc.s_hdlr_name,
                   x_strlen( pt_sess_hdlr_obj->t_hdlr_desc.s_hdlr_name ) );
                  
        *pui4_get_info_size = 
            x_strlen( pt_sess_hdlr_obj->t_hdlr_desc.s_hdlr_name ) + 1;
        break;
    
    case SM_SESS_GET_TYPE_H_CONNECTION:
        if ( *pui4_get_info_size < sizeof( HANDLE_T ) )
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        *((HANDLE_T*)(pv_get_info)) = pt_sess_obj->t_base.h_connection;
        *pui4_get_info_size = sizeof( HANDLE_T );
        break;
    
    case SM_SESS_GET_TYPE_STRM_NUM:
        hash_lock( pt_sess_obj->t_base.h_strm_ctrl_tbl );

        i4_ret = hash_get_size( pt_sess_obj->t_base.h_strm_ctrl_tbl,
                                (SIZE_T*)pv_get_info );

        hash_unlock( pt_sess_obj->t_base.h_strm_ctrl_tbl );

        if ( i4_ret != HASH_OK )
        {
            return SMR_INTERNAL_ERROR;
        }
        *pui4_get_info_size = sizeof( SIZE_T );
        break;
    
    case SM_SESS_GET_TYPE_STRM_HANDLES:
        {
            SIZE_T     z_buff_size = (*pui4_get_info_size)/sizeof( UINT32 );
            SIZE_T     z_num_strm = 0;

            if ( *pui4_get_info_size < sizeof( UINT32 ) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            
            hash_lock( pt_sess_obj->t_base.h_strm_ctrl_tbl );

            i4_ret = hash_get_size( pt_sess_obj->t_base.h_strm_ctrl_tbl,
                                    (SIZE_T*)&z_num_strm );
            if ( i4_ret != HASH_OK )
            {
                hash_unlock( pt_sess_obj->t_base.h_strm_ctrl_tbl );
                return SMR_INTERNAL_ERROR;
            }
            if ( z_num_strm > z_buff_size )
            {
                hash_unlock( pt_sess_obj->t_base.h_strm_ctrl_tbl );
                return SMR_INSUFFICIENT_SIZE;
            }
            
            x_memset( pv_get_info, 0, *pui4_get_info_size );
            
            i4_ret = hash_parse( pt_sess_obj->t_base.h_strm_ctrl_tbl,
                                 _strm_ctrl_tbl_get_strm_handles_hash_cb,
                                 (VOID*)pv_get_info );
        
            hash_unlock( pt_sess_obj->t_base.h_strm_ctrl_tbl );
        
            if ( i4_ret != HASH_OK )
            {
                SM_GS_DBG_INFO(( 
                    "[SM] SESS(0x%.8x), hash_parse(_strm_ctrl_tbl_get_strm_handles_hash_cb) error, return %d!\r\n",
                    (UINT32)h_sess, i4_ret ));
                return SMR_INTERNAL_ERROR;
            }
        }
        break;

    case SM_SESS_GET_TYPE_DEST_GRP_NAME:
        if ( *pui4_get_info_size <= 
             x_strlen( pt_sess_obj->t_base.s_dest_grp_name ) )
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        
        x_strncpy( (CHAR*)(pv_get_info), 
                   pt_sess_obj->t_base.s_dest_grp_name,
                   x_strlen( pt_sess_obj->t_base.s_dest_grp_name ) );
                  
        *pui4_get_info_size = 
            x_strlen( pt_sess_obj->t_base.s_dest_grp_name ) + 1;
        break;
        
    case SM_SESS_GNRC_GET_TYPE_PCR:
        if ( *pui4_get_info_size < sizeof( HANDLE_T ) )
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        *((HANDLE_T*)(pv_get_info)) = pt_sess_obj->t_pcr_descr.h_comp_clone;
        *pui4_get_info_size = sizeof( HANDLE_T );
        break;
    
    case SM_SESS_GNRC_GET_TYPE_PCR_ID:
        if ( *pui4_get_info_size < sizeof( UINT16 ) )
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        *((UINT16*)(pv_get_info)) = pt_sess_obj->t_pcr_descr.t_info.ui2_comp_id;
        *pui4_get_info_size = sizeof( UINT16 );
        break;
#ifdef MW_SUPPORT_2STC
    case SM_SESS_GNRC_GET_TYPE_STC_ID:
       if ( *pui4_get_info_size < sizeof( UINT16 ) )
       {
           return SMR_INSUFFICIENT_SIZE;
       }
       *((UINT16*)(pv_get_info)) = pt_sess_obj->t_stc_descr.t_info.ui2_comp_id;
       *pui4_get_info_size = sizeof( UINT16 );
        break;
#endif

#ifdef ENABLE_MULTIMEDIA
    case SM_SESS_GNRC_GET_TYPE_UOP_CAP:
        if ( *pui4_get_info_size < sizeof( SM_SESS_GNRC_UOP_CAP_INFO_T ) )
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        
        if ( PMR_OK != pm_get( pt_sess_obj->t_base.h_pm_service,
                               PM_GET_UOP_CAP,
                               pv_get_info,
                               pui4_get_info_size) )
        {
            return SMR_INTERNAL_ERROR;
        }
        break;

    case SM_SESS_GNRC_GET_TYPE_STC:
        if ( *pui4_get_info_size < sizeof( STC_T ) )
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        
        i4_ret = sm_sess_gnrc_stc_get( pt_sess_obj, (STC_T*)pv_get_info);
        if ( i4_ret != SMR_OK )
        {
            SM_GS_DBG_INFO(( 
                "[SM] SESS, sm_sess_gnrc_stc_get(0x%.8X) error, return %d!\r\n",
                (UINT32)pt_sess_obj->t_stc_descr.h_comp, i4_ret ));
        
            sm_gs_send_msg_do_event(
                SM_EVN_SESS_DRV_ERR,
                h_sess,
                0 );
            return SMR_INTERNAL_ERROR;
        }

        *pui4_get_info_size = sizeof( STC_T );
        break;

    case SM_SESS_GNRC_GET_TYPE_STC_SIGNED:
        if ( *pui4_get_info_size < sizeof( STC_T ) )
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        
        i4_ret = sm_sess_gnrc_stc_get_signed( pt_sess_obj, (STC_T*)pv_get_info);
        if ( i4_ret != SMR_OK )
        {
            SM_GS_DBG_INFO(( 
                "[SM] SESS, sm_sess_gnrc_stc_get_signed(0x%.8X) error, return %d!\r\n",
                (UINT32)pt_sess_obj->t_stc_descr.h_comp, i4_ret ));
        
            sm_gs_send_msg_do_event(
                SM_EVN_SESS_DRV_ERR,
                h_sess,
                0 );
            return SMR_INTERNAL_ERROR;
        }

        *pui4_get_info_size = sizeof( STC_T );
        break;

    case SM_SESS_GNRC_GET_TYPE_BLOCK_PSI:
        if (*pui4_get_info_size < sizeof(UINT32))
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        if ( PMR_OK != pm_get( pt_sess_obj->t_base.h_pm_service,
                               PM_GET_BLOCK_PSI,
                               pv_get_info,
                               pui4_get_info_size) )
        {
            return SMR_INTERNAL_ERROR;
        }
        break;
    case SM_SESS_GNRC_GET_TYPE_POST_INFO:
        {   
            SIZE_T                      ui4_get_info_size = sizeof(SM_SESS_GNRC_POST_INFO_T);
    
            if ( *pui4_get_info_size < sizeof(SM_SESS_GNRC_POST_INFO_T) )
            {
                return SMR_INV_ARG;
            }
            if ( PMR_OK != pm_get( pt_sess_obj->t_base.h_pm_service,
                                   PM_GET_POST_INFO,
                                   (VOID*)pv_get_info,
                                   &ui4_get_info_size) )
            {
                return SMR_INTERNAL_ERROR;
            }
        }
        break;
#ifdef TIME_SHIFT_SUPPORT
    case SM_SESS_GNRC_GET_TYPE_CURR_LBA:
        if (*pui4_get_info_size < sizeof(UINT32))
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        if ( PMR_OK != pm_get( pt_sess_obj->t_base.h_pm_service,
                               PM_GET_CURR_LBA,
                               pv_get_info,
                               pui4_get_info_size) )
        {
            return SMR_INTERNAL_ERROR;
        }
        break;

    case SM_SESS_GNRC_GET_TYPE_CURR_TICK_NUM:
        if (*pui4_get_info_size < sizeof(UINT32))
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        if ( PMR_OK != pm_get( pt_sess_obj->t_base.h_pm_service,
                               PM_GET_CURR_TICK_NUM,
                               pv_get_info,
                               pui4_get_info_size) )
        {
            return SMR_INTERNAL_ERROR;
        }
        break;
#endif
#endif

    case SM_SESS_GNRC_GET_TYPE_MEDIA_FMT:
        if ( *pui4_get_info_size < sizeof( MEDIA_FORMAT_T ) )
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        *((MEDIA_FORMAT_T*)(pv_get_info)) = pt_sess_obj->t_media_fmt;
        *pui4_get_info_size = sizeof( MEDIA_FORMAT_T );
        break;

    default:
        return SMR_INV_ARG;
    }
    
    return SMR_OK;    
}

/*-----------------------------------------------------------------------------
                    functions implementation
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: sm_gs_send_msg
 *
 * Description: <function description>
 *
 * Inputs:  pt_msg      References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sm_gs_send_msg( const SM_SESS_GNRC_MSG_T* pt_msg )
{
    INT32   i4_ret;
    UINT32  i4_i = 0;
    
    while( i4_i++ < 10 )
    {
        i4_ret = x_msg_q_send( gh_sess_gnrc_msgq,
                               (VOID*)pt_msg,
                               sizeof( SM_SESS_GNRC_MSG_T ),
                               0 ); /* highest priority */
        if ( i4_ret != OSR_OK )
        {
            HANDLE_T    h_thread = NULL_HANDLE;
            
            i4_ret = x_thread_self( &h_thread );
            if ( i4_ret != OSR_OK || 
                 (i4_ret == OSR_OK && !x_handle_valid(h_thread)) )
            {
                break;
            }

            /* Send message fail, wait 10 ms and retry for 10 times */
            x_thread_delay( 10 );
            continue;
        }
    
        i4_ret = sm_trigger_task( gh_sess_gnrc_hdlr, (VOID*)gh_sess_gnrc_hdlr );
        if ( i4_ret != SMR_OK )
        {
            SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
        }
        
        return;  /* success */
    }

    SM_ABORT( SM_DBG_ABRT_CANNOT_SEND_TO_MSG_Q );
}

/*-----------------------------------------------------------------------------
 * Name: sm_gs_send_msg_do_event
 *
 * Description: <function description>
 *
 * Inputs:  pt_msg      References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sm_gs_send_msg_do_event( 
    UINT32                   ui4_event,
    HANDLE_T                 h_sess,
    UINT32                   ui4_data )
{
    SM_SESS_GNRC_MSG_T  t_sess_msg;
    
    t_sess_msg.e_msg_type = SM_SESS_GNRC_MSG_TYPE_DO_EVENT;
    t_sess_msg.h_sess = h_sess;
    t_sess_msg.t_do_event_msg.ui4_event = ui4_event;
    t_sess_msg.t_do_event_msg.ui4_data = ui4_data;
    
    sm_gs_send_msg( &t_sess_msg );
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_init
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_gnrc_init( VOID )
{
    INT32                    i4_ret;
    SM_SESS_HDLR_FCT_TBL_T   t_sess_hdlr_fct_tbl;
    SM_HDLR_DESC_T           t_hdlr_desc;
    
    t_sess_hdlr_fct_tbl.pf_open_sess = &sm_sess_gnrc_open;
    t_sess_hdlr_fct_tbl.pf_open_stream = &sm_sess_gnrc_strm_open;
    t_sess_hdlr_fct_tbl.pf_close = &sm_sess_gnrc_close;
    t_sess_hdlr_fct_tbl.pf_set = &sm_sess_gnrc_set;
    t_sess_hdlr_fct_tbl.pf_get = &sm_sess_gnrc_get;
    t_sess_hdlr_fct_tbl.pf_strm_nfy = &sm_sess_gnrc_strm_nfy;
    t_sess_hdlr_fct_tbl.pf_task = &sm_sess_gnrc_task;
    
    x_strcpy( t_hdlr_desc.s_hdlr_name, SM_GNRC_SESS_HDLR_NAME );
    t_hdlr_desc.ui4_strm_type_mask = ST_MASK_AUDIO | ST_MASK_VIDEO | ST_MASK_CLOSED_CAPTION;
    
    i4_ret = sm_reg_sess_handler( &t_hdlr_desc,
                                  &t_sess_hdlr_fct_tbl,
                                  FALSE,
                                  NULL,
                                  &gh_sess_gnrc_hdlr );
    if ( i4_ret != SMR_OK || gh_sess_gnrc_hdlr == NULL_HANDLE )
    {
        return SMR_INTERNAL_ERROR;
    }
    
    /* Create message queue for Generic Session Task */
    if ( gh_sess_gnrc_msgq == NULL_HANDLE )
    {
        i4_ret = x_msg_q_create( &gh_sess_gnrc_msgq,
                              SM_GNRC_SESS_HDLR_NAME,  /* use thread name */
                              sizeof( SM_SESS_GNRC_MSG_T ),
                              SM_SESS_GNRC_MSGQ_MAX_SIZE );
        if ( i4_ret != OSR_OK )
        {
            return SMR_INTERNAL_ERROR;
        }
    }    
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_open
 *
 * Description: <function description>
 *
 * Inputs:  h_connection     References.
 *          h_sess_hdlr      References.
 *          pt_sess_data     References.
 *          z_sess_data_size References.
 *          i4_speed         References.
 *          pv_sess_nfy_tag  References.
 *          ph_sess          References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK                   Success.
 *          SMR_INTERNAL_ERROR       Fail.
 *          SMR_OUT_OF_HANDLES       Fail.
 *          SMR_INSUFFICIENT_MEMORY  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_gnrc_open( HANDLE_T              h_connection,
                         HANDLE_T              h_sess_hdlr,
                         const CHAR*           ps_dest_grp_name,
                         VOID*                 pv_sess_data,
                         SIZE_T                z_sess_data_size,
                         INT32                 i4_speed,
                         VOID*                 pv_sess_nfy_tag,
                         HANDLE_T*             ph_sess )
{
    INT32                   i4_ret = 0;
    SM_SESS_GNRC_T*         pt_sess_obj = NULL;
    CM_CTRL_RM_SRC_COMP_T   t_ctrl_rm_src_comp;
    SM_SESS_GNRC_DATA_T*    pt_sess_data = (SM_SESS_GNRC_DATA_T*)pv_sess_data;
    SM_GS_STATE_TAG_T*      pt_state_tag;
    
    pt_state_tag = (SM_GS_STATE_TAG_T*)x_mem_alloc( sizeof(SM_GS_STATE_TAG_T) );
    if ( !pt_state_tag )
    {
        return SMR_INSUFFICIENT_MEMORY;    
    }

    /* create session object */
    pt_sess_obj = (SM_SESS_GNRC_T*)x_mem_alloc( sizeof(SM_SESS_GNRC_T) );
    if ( !pt_sess_obj )
    {
        x_mem_free( pt_state_tag );
        return SMR_INSUFFICIENT_MEMORY;    
    }
    
    x_memset( pt_sess_obj, 0, sizeof(SM_SESS_GNRC_T) );
    
    pt_sess_obj->t_base.h_sess_hdlr = h_sess_hdlr;
    pt_sess_obj->t_base.h_connection = h_connection;
    i4_ret = hash_create( SM_STRM_CTRL_TBL_SIZE,
                          &pt_sess_obj->t_base.h_strm_ctrl_tbl );
    if ( i4_ret != HASH_OK || pt_sess_obj->t_base.h_strm_ctrl_tbl == NULL_HANDLE )
    {
        x_mem_free( pt_sess_obj );
        x_mem_free( pt_state_tag );
        return SMR_INSUFFICIENT_MEMORY;
    }

    pt_sess_obj->t_base.e_sess_cond = SM_SESS_COND_IGNORE;
    pt_sess_obj->t_base.pv_sess_nfy_tag = pv_sess_nfy_tag;
    pt_sess_obj->t_base.i4_speed = i4_speed;
    pt_sess_obj->ui4_lastest_pm_event = SM_EVN_IGNORE;
    pt_sess_obj->ui4_lastest_pcr_event = SM_EVN_IGNORE;
    pt_sess_obj->ui4_lastest_stc_event = SM_EVN_IGNORE;
    pt_sess_obj->ui4_lastest_strm_event = SM_EVN_IGNORE;
    pt_sess_obj->t_base.h_sess = NULL_HANDLE;
    if (MM_IS_TIME_SHIFT_CMPB(pt_sess_data->t_media_fmt))
	{
    	pt_sess_obj->ui2_force_pid = pt_sess_data->ui2_pcr_pid;
	}
	else
	{
		pt_sess_obj->ui2_force_pid = MPEG_2_NULL_PID;
	}

    pt_sess_obj->t_base.h_lock = NULL_HANDLE;
    i4_ret = x_sema_create( &pt_sess_obj->t_base.h_lock,
                            X_SEMA_TYPE_MUTEX,
                            X_SEMA_STATE_UNLOCK );
    if ( i4_ret != OSR_OK || pt_sess_obj->t_base.h_lock == NULL_HANDLE )
    {
        hash_delete( pt_sess_obj->t_base.h_strm_ctrl_tbl, (hash_obj_free_fct)NULL );
        x_mem_free( pt_sess_obj );
        x_mem_free( pt_state_tag );
        return SMR_INTERNAL_ERROR;   
    }
    pt_sess_obj->t_pcr_descr = t_sm_drv_comp_initor;
    pt_sess_obj->t_pcr_descr.t_info.b_shared_comp_id = TRUE;
    pt_sess_obj->t_stc_descr = t_sm_drv_comp_initor;
    pt_sess_obj->t_dmx_descr = t_sm_drv_comp_initor;
    x_strcpy( pt_sess_obj->t_base.s_dest_grp_name, ps_dest_grp_name );

    pt_sess_obj->t_media_fmt.e_media_type = MEDIA_TYPE_UNKNOWN;
    pt_sess_obj->e_mm_sync_mode = SM_SESS_GNRC_MM_SYNC_UNKNOWN;
    pt_sess_obj->t_stc_init = 0;
    pt_sess_obj->e_mm_speed = MM_SPEED_TYPE_FORWARD_1X;
    pt_sess_obj->b_sub_freerun =FALSE;
    if ( pt_sess_data )
    {
        pt_sess_obj->b_disable_av_sync = pt_sess_data->b_disable_av_sync;
#ifdef ENABLE_MULTIMEDIA
        pt_sess_obj->t_media_fmt        = pt_sess_data->t_media_fmt;
        pt_sess_obj->e_priority         = pt_sess_data->e_priority;
        pt_sess_obj->e_mm_sync_mode     = pt_sess_data->e_mm_sync_mode;
        pt_sess_obj->t_stc_init         = pt_sess_data->t_stc_init;
        pt_sess_obj->b_es_with_pts_in   = pt_sess_data->b_es_with_pts_in; 
        pt_sess_obj->b_sub_freerun      = pt_sess_data->b_sub_freerun;
#endif
    }
    else
    {
        pt_sess_obj->b_disable_av_sync = FALSE;
    }
#ifdef MW_TV_AV_BYPASS_SUPPORT
    pt_sess_obj->b_pcr_owner = TRUE;
#endif

    SLIST_INIT(&pt_sess_obj->t_scart_list);
    
    /* get source driver component */
    i4_ret = x_cm_get( pt_sess_obj->t_base.h_connection,
                        CM_CTRL_RM_SRC_COMP,
                        (VOID*)&t_ctrl_rm_src_comp);
    if ( i4_ret != CMR_OK )
    {
        hash_delete( pt_sess_obj->t_base.h_strm_ctrl_tbl, (hash_obj_free_fct)NULL );
        x_mem_free( pt_sess_obj );
        x_mem_free( pt_state_tag );
        return SMR_INTERNAL_ERROR;
    }
    pt_sess_obj->h_source = t_ctrl_rm_src_comp.h_rm_comp;
    
    /* create session handle */
    i4_ret = handle_alloc( SM_HT_SESSION,
                           pt_sess_obj,
                           (VOID*)h_connection,
                           &sess_gnrc_free_fct,
                           ph_sess );
    if ( i4_ret != HR_OK || !ph_sess )
    {
        /* pass NULL as (hash_free_fct), because there is no stream 
           currently */
        hash_delete( pt_sess_obj->t_base.h_strm_ctrl_tbl, (hash_obj_free_fct)NULL );
        x_sema_delete( pt_sess_obj->t_base.h_lock );
        x_mem_free( pt_sess_obj );
        x_mem_free( pt_state_tag );
        return SMR_OUT_OF_HANDLES;
    }
    pt_sess_obj->t_base.h_sess = *ph_sess;
    
    pt_state_tag->h_sess = *ph_sess;
    pt_state_tag->pt_sess_obj = pt_sess_obj;
    i4_ret = sm_state_init( at_gs_state_cond_lst,
                            at_gs_state_descr_lst,
                            at_gs_allowed_pending_event_lst,
                            sm_gs_state_notify_app,
                            (VOID*)pt_state_tag,
                            &pt_sess_obj->t_state );
    if ( i4_ret != SMR_OK )
    {
        handle_free( *ph_sess, FALSE );
        return SMR_INTERNAL_ERROR;   
    }
    
    SM_LOCK( pt_sess_obj->t_base.h_lock )
    
    sm_gs_send_msg_do_event(
        SM_EVN_SESS_APP_OPEN_REQ,
        *ph_sess,
        0 );

    SM_UNLOCK( pt_sess_obj->t_base.h_lock )

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_strm_open
 *
 * Description: <function description>
 *
 * Inputs:  pt_stream_base References.
 *          pt_cmd_sets    References.
 *          b_auto_play    References.
 *          pf_sm_nfy      References.
 *          pv_sm_nfy_tag  References.
 *          ph_stream      References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_SESS_NOT_EXIST  Fail.
 *          SMR_NOT_ALLOW       Fail.
 *          SMR_INV_HANDLE      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_gnrc_strm_open( const SM_STREAM_T*  pt_stream_base,
                              const SM_COMMAND_T* pt_cmd_sets,
                              BOOL                b_auto_play,
                              x_stream_nfy_fct    pf_sm_nfy,
                              VOID*               pv_sm_nfy_tag,
                              HANDLE_T*           ph_stream ) /* OUT */
{
    INT32             i4_ret;
    HANDLE_TYPE_T     t_hdl_type;
    SM_HANDLER_T*     pt_hdlr_obj = NULL;
    SM_SESS_GNRC_T*   pt_sess_obj = NULL;
    
    i4_ret = handle_get_type_obj( pt_stream_base->h_handler,
                                  &t_hdl_type,
                                  (VOID**)&pt_hdlr_obj );
    if ( i4_ret != HR_OK || t_hdl_type != SM_HT_HANDLER )
    {
        return SMR_INV_HANDLE;
    }
    
    pt_sess_obj = (SM_SESS_GNRC_T*)sm_get_sess_obj( pt_stream_base->h_sess );
    if ( !pt_sess_obj )
    {
        return SMR_SESS_NOT_EXIST;
    }

    /* check if session state allows */
    if ( pt_sess_obj->t_base.e_sess_cond == SM_SESS_COND_OPENING ||
         pt_sess_obj->t_base.e_sess_cond == SM_SESS_COND_CLOSING ||
         pt_sess_obj->t_base.e_sess_cond == SM_SESS_COND_CLOSED ||
         pt_sess_obj->t_base.e_sess_cond == SM_SESS_COND_IGNORE )
    {
        return SMR_NOT_ALLOW;
    }
    
    if ((pt_sess_obj->t_base.e_sess_cond == SM_SESS_COND_STARTING ||
         pt_sess_obj->t_base.e_sess_cond == SM_SESS_COND_STARTED)
#ifdef ENABLE_MULTIMEDIA
        && (pt_sess_obj->t_media_fmt.e_media_type == MEDIA_TYPE_UNKNOWN)
#endif
       )
    {
        b_auto_play = TRUE;
    }
        
    i4_ret = pt_hdlr_obj->t_fct_tbl.pf_open( pt_stream_base,
                                             pt_cmd_sets,
                                             b_auto_play,
                                             pf_sm_nfy,
                                             pv_sm_nfy_tag,
                                             ph_stream );   /* OUT */
    if ( i4_ret != SMR_OK || !ph_stream )
    {
        return i4_ret;
    }
    
    i4_ret = sess_gnrc_add_stream( 
        pt_stream_base->h_sess,
        pt_sess_obj,
        *ph_stream );
    if ( i4_ret != SMR_OK )
    {
        pt_stream_base = sm_get_strm_obj( *ph_stream );
        if ( pt_stream_base )
        {
            SM_LOCK( pt_stream_base->h_lock );
            
            pt_hdlr_obj->t_fct_tbl.pf_close( *ph_stream );
            
            SM_UNLOCK( pt_stream_base->h_lock );
        }
        return i4_ret;
    }
        
    return SMR_OK;
}


/* Handler stream close function definition */
/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_close
 *
 * Description: <function description>
 *
 * Inputs:  h_handle References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *          SMR_SESS_NOT_EXIST  Fail.
 *          SMR_NOT_ALLOW       Fail.
 *          SMR_INV_HANDLE      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_gnrc_close( HANDLE_T     h_handle )
{
    INT32             i4_ret = SMR_OK;
    HANDLE_TYPE_T     e_hdl_type;
    VOID*             pv_obj = NULL;

    i4_ret = handle_get_type_obj( h_handle, &e_hdl_type, (VOID**)&pv_obj );    
    if ( i4_ret != HR_OK )
    {
        return SMR_INV_HANDLE;
    }
    
    if ( e_hdl_type == SM_HT_SESSION )
    {
        sm_gs_send_msg_do_event(
            SM_EVN_SESS_APP_CLOSE_REQ,
            h_handle,
            0 );
    }
    else if ( e_hdl_type == SM_HT_STREAM )
    {
        SM_HANDLER_T*     pt_hdlr_obj = NULL;
        SM_STREAM_T*      pt_strm_obj = (SM_STREAM_T*)pv_obj;
        SM_SESS_GNRC_T*   pt_sess_obj = NULL;
        
        pt_sess_obj = (SM_SESS_GNRC_T*)sm_get_sess_obj( pt_strm_obj->h_sess );
        if ( !pt_sess_obj )
        {
            return SMR_SESS_NOT_EXIST;
        }
        
        /* check if session state allows */
        if ( pt_sess_obj->t_base.e_sess_cond == SM_SESS_COND_OPENING ||
             pt_sess_obj->t_base.e_sess_cond == SM_SESS_COND_CLOSING ||
             pt_sess_obj->t_base.e_sess_cond == SM_SESS_COND_CLOSED ||
             pt_sess_obj->t_base.e_sess_cond == SM_SESS_COND_IGNORE )
        {
            return SMR_NOT_ALLOW;
        }
            
        i4_ret = handle_get_type_obj( pt_strm_obj->h_handler,
                                      &e_hdl_type,
                                      (VOID**)&pt_hdlr_obj );
        if ( i4_ret != HR_OK || e_hdl_type != SM_HT_HANDLER )
        {
            return SMR_INV_HANDLE;
        }
        
        i4_ret = pt_hdlr_obj->t_fct_tbl.pf_close( h_handle );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
    }
    else
    {
        return SMR_INV_HANDLE;    
    }
    
    return i4_ret;
}

/* Handler stream set function definition */
/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_set
 *
 * Description: <function description>
 *
 * Inputs:  h_handle       References.
 *          e_set_type     References.
 *          pv_set_info    References.
 *          z_set_info_len References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK           Success.
 *          SMR_NOT_SUPPORT  Fail.
 *          SMR_INV_HANDLE   Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_gnrc_set( HANDLE_T       h_handle,
                        SM_SET_TYPE_T  e_set_type,
                        VOID*          pv_set_info,
                        SIZE_T         z_set_info_len )
{
    INT32             i4_ret;
    HANDLE_TYPE_T     e_hdl_type;
    VOID*             pv_obj = NULL;

    i4_ret = handle_get_type_obj( h_handle, &e_hdl_type, (VOID**)&pv_obj );    
    if ( i4_ret != HR_OK )
    {
        return SMR_INV_HANDLE;
    }
    
    if ( e_hdl_type == SM_HT_SESSION )
    {
        SM_SESS_GNRC_T*    pt_sess_obj = NULL;
    
        pt_sess_obj = (SM_SESS_GNRC_T*)sm_get_sess_obj( h_handle );
        if ( !pt_sess_obj )
        {
            return SMR_INV_HANDLE;
        }
    
        i4_ret = sess_gnrc_sess_set( h_handle,
                                     pt_sess_obj,
                                     e_set_type,
                                     pv_set_info,
                                     z_set_info_len );
        
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;    
        }
    }
    else if ( e_hdl_type == SM_HT_STREAM )
    {
        SM_HANDLER_T*     pt_hdlr_obj = NULL;
        SM_STREAM_T*      pt_strm_obj = (SM_STREAM_T*)pv_obj;

        i4_ret = handle_get_type_obj( pt_strm_obj->h_handler,
                                      &e_hdl_type,
                                      (VOID**)&pt_hdlr_obj );
        if ( i4_ret != HR_OK || e_hdl_type != SM_HT_HANDLER )
        {
            return SMR_INV_HANDLE;
        }
                
        if ( (SM_GET_GRP(e_set_type) != SM_GRP_STRM) &&
             (SM_GET_GRP(e_set_type) < (SM_GRP_LAST_ENTRY<<SM_CODE_BITS)) )
        {
            return SMR_INV_ARG;
        }
        
        i4_ret = pt_hdlr_obj->t_fct_tbl.pf_set( h_handle,
                                                e_set_type,
                                                pv_set_info,
                                                z_set_info_len );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }        
    }
    else
    {
        return SMR_INV_HANDLE;    
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_get
 *
 * Description: <function description>
 *
 * Inputs:  h_handle        References.
 *          e_get_type      References.
 *          pv_get_info     References.
 *          pz_get_info_len References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK           Success.
 *          SMR_INV_ARG      Fail.
 *          SMR_NOT_SUPPORT  Fail.
 *          SMR_INV_HANDLE   Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_gnrc_get( HANDLE_T       h_handle,
                        SM_GET_TYPE_T  e_get_type,
                        VOID*          pv_get_info,      /* IN/OUT */
                        SIZE_T*        pz_get_info_len ) /* IN/OUT */
{
    INT32             i4_ret;
    HANDLE_TYPE_T     e_hdl_type;
    VOID*             pv_obj = NULL;

    i4_ret = handle_get_type_obj( h_handle, &e_hdl_type, (VOID**)&pv_obj );    
    if ( i4_ret != HR_OK )
    {
        return SMR_INV_HANDLE;
    }
    
    if ( e_hdl_type == SM_HT_SESSION )
    {
        SM_SESS_GNRC_T*    pt_sess_obj = NULL;
        SM_SESS_HANDLER_T* pt_sess_hdlr_obj = NULL;
        
        pt_sess_hdlr_obj = (SM_SESS_HANDLER_T*)
                           sm_get_sess_hdlr_obj( h_handle,
                                                (VOID**)&pt_sess_obj );
        if ( !pt_sess_hdlr_obj || !pt_sess_obj )
        {
            return SMR_INV_HANDLE;
        }
        
        i4_ret = sess_gnrc_sess_get( h_handle,
                                     pt_sess_obj,
                                     pt_sess_hdlr_obj,
                                     e_get_type,
                                     pv_get_info,
                                     pz_get_info_len );
        
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;    
        }
    }
    else if ( e_hdl_type == SM_HT_STREAM )
    {
        SM_HANDLER_T*     pt_hdlr_obj = NULL;
        SM_STREAM_T*      pt_strm_obj = (SM_STREAM_T*)pv_obj;
        
        i4_ret = handle_get_type_obj( pt_strm_obj->h_handler,
                                      &e_hdl_type,
                                      (VOID**)&pt_hdlr_obj );
        if ( i4_ret != HR_OK || e_hdl_type != SM_HT_HANDLER )
        {
            return SMR_INV_HANDLE;
        }

        if ( (SM_GET_GRP(e_get_type) != SM_GRP_STRM) &&
             (SM_GET_GRP(e_get_type) < (SM_GRP_LAST_ENTRY<<SM_CODE_BITS)) )
        {
            return SMR_INV_ARG;
        }
        
        i4_ret = pt_hdlr_obj->t_fct_tbl.pf_get( h_handle,
                                             e_get_type,
                                             pv_get_info,
                                             pz_get_info_len );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
    }
    else
    {
        return SMR_INV_HANDLE;    
    }
    
    return SMR_OK;
}

/* Handler stream notify function definition */
/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_strm_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_stream      References.
 *          e_cond_type   References.
 *          pv_sm_nfy_tag References.
 *          e_event       References.
 *          ui4_data      References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_gnrc_strm_nfy( HANDLE_T              h_stream,
                             SM_COND_T             e_cond_type,
                             VOID*                 pv_sm_nfy_tag,
                             SM_EVN_TYPE_T         e_event,
                             UINT32                ui4_data )
{
    SM_SESS_GNRC_MSG_T  t_msg;
         
    t_msg.e_msg_type = SM_SESS_GNRC_MSG_TYPE_HDLR;
    t_msg.t_hdlr_msg.h_stream = h_stream;
    t_msg.t_hdlr_msg.e_strm_cond = e_cond_type;
    t_msg.t_hdlr_msg.pv_tag = pv_sm_nfy_tag;
    t_msg.t_hdlr_msg.e_strm_evn = e_event;
    t_msg.t_hdlr_msg.ui4_data = ui4_data;
    
    sm_gs_send_msg( &t_msg );
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_task
 *
 * Description: <function description>
 *
 * Inputs:  pv_arg References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sm_sess_gnrc_task( VOID*  pv_arg )
{
    INT32                      i4_ret;
    UINT16                     ui2_index;
    SM_SESS_GNRC_T*            pt_sess_obj = (SM_SESS_GNRC_T*)NULL;
    SM_SESS_GNRC_MSG_T         t_msg;
    SIZE_T                     z_msg_size = sizeof(SM_SESS_GNRC_MSG_T);
    
    x_memset( &t_msg, 0, sizeof(SM_SESS_GNRC_MSG_T) );
    
    i4_ret = x_msg_q_receive( &ui2_index,
                              &t_msg,
                              &z_msg_size,
                              &gh_sess_gnrc_msgq,
                              (UINT16)1,
                              X_MSGQ_OPTION_NOWAIT );
    if ( i4_ret == OSR_NO_MSG )
    {
        return;    
    }
    if ( i4_ret != OSR_OK )
    {
        SM_ABORT( SM_DBG_ABRT_CANNOT_RECV_FROM_MSG_Q );
    }
    
    if ( t_msg.e_msg_type == SM_SESS_GNRC_MSG_TYPE_HDLR )
    {
        sess_gnrc_prc_hdlr_msg( t_msg.t_hdlr_msg.h_stream,
                                t_msg.t_hdlr_msg.e_strm_cond,
                                t_msg.t_hdlr_msg.pv_tag,
                                t_msg.t_hdlr_msg.e_strm_evn,
                                t_msg.t_hdlr_msg.ui4_data );
        return;
    }

    pt_sess_obj = (SM_SESS_GNRC_T*)sm_get_sess_obj( t_msg.h_sess );
    if ( !pt_sess_obj )
    {
        SM_GS_DBG_INFO(( 
            "[SM] WARNING, receive message(%d) for a invalid session handle(0x%.8X)\r\n",
            (UINT32)t_msg.e_msg_type, (UINT32)t_msg.h_sess ));
        return;
    }
        
    SM_LOCK_RETURN_ON_FAIL( pt_sess_obj->t_base.h_lock, );
    
    switch( t_msg.e_msg_type )
    {
    case SM_SESS_GNRC_MSG_TYPE_PCR:
        sess_gnrc_prc_pcr_msg( 
            t_msg.h_sess,
            pt_sess_obj,
            &t_msg.t_pcr_msg );
        break;

    case SM_SESS_GNRC_MSG_TYPE_STC:
        sess_gnrc_prc_stc_msg( 
            t_msg.h_sess,
            pt_sess_obj,
            &t_msg.t_stc_msg );
        break;

    case SM_SESS_GNRC_MSG_TYPE_DMX:
        sess_gnrc_prc_dmx_msg( 
            t_msg.h_sess,
            pt_sess_obj,
            &t_msg.t_dmx_msg );
        break;

    case SM_SESS_GNRC_MSG_TYPE_SCDB:
        sess_gnrc_prc_scdb_msg( 
            t_msg.h_sess,
            pt_sess_obj,
            &t_msg.t_scdb_msg );
        break;
    
    case SM_SESS_GNRC_MSG_TYPE_DO_EVENT:
        sess_gnrc_prc_do_event_msg( 
            t_msg.h_sess,
            pt_sess_obj,
            &t_msg.t_do_event_msg );
        break;

    default:
        SM_GS_DBG_ERR(( 
            "[SM] %s:%d: undefined session message (%u)\r\n",
            __FILE__, __LINE__, (UINT32)t_msg.e_msg_type ));
        SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
    }

    SM_UNLOCK( pt_sess_obj->t_base.h_lock );

    if ( pt_sess_obj->t_base.e_sess_cond == SM_SESS_COND_CLOSED )
    {
        if( sm_sess_lock() != SMR_OK )
        {
            SM_ABORT(SM_DBG_ABRT_CANNOT_LOCK_SEMAPHORE);
        }
        if ( sm_stream_lock() != SMR_OK )
        {
            SM_ABORT(SM_DBG_ABRT_CANNOT_LOCK_SEMAPHORE);
        }    
        handle_free( t_msg.h_sess, FALSE );
        sm_stream_unlock();
        sm_sess_unlock();
    }
 
    return;
}

