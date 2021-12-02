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
 * $RCSfile: sm_bypass_hdlr.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Ivan Wei $
 * $MD5HEX: 5cb9f4f7826f72aa712d5913dfb1ddd4 $
 *
 * Description: 
 *         This file implements the Bypass Stream Handler.
 *---------------------------------------------------------------------------*/

#ifdef MW_TV_AV_BYPASS_SUPPORT

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
 
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_vid_plane.h"
#include "res_mngr/drv/x_tv_enc.h"
#include "res_mngr/drv/x_video_out.h"

#include "scdb/x_scdb_api.h"

#include "strm_mngr/sm_slctr.h"
#include "strm_mngr/sm_state.h"

#include "strm_mngr/bypass_hdlr/_sm_bypass_cmd.h"
#include "strm_mngr/bypass_hdlr/_sm_bypass_comp_vbif.h"
#include "strm_mngr/bypass_hdlr/_sm_bypass_comp_vd.h"
#include "strm_mngr/bypass_hdlr/_sm_bypass_state.h"
#include "strm_mngr/bypass_hdlr/sm_bypass_hdlr.h"
#include "strm_mngr/bypass_hdlr/u_sm_bypass_event.h"

#include "strm_mngr/sess_gnrc_hdlr/x_sm_sess_gnrc.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
 
#define SM_BYPASS_HDLR_MSGQ_MAX_SIZE 256

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
 
HANDLE_T gh_bypass_handler = NULL_HANDLE;
HANDLE_T gh_bypass_hdlr_msgq = NULL_HANDLE;

/* 
 *  Bypass Stream State Machine 
 */
static SM_STATE_DESCR_T   at_bypass_state_descr_lst[] =
{
  /* ui4_state,        b_prcss_pending, b_allow_pending, pf_state_do,                 pf_state_do_exception */
  { SM_COND_IGNORE,   FALSE,           FALSE,           NULL,                        sm_bypass_state_do_ignore_excpt },
  { SM_COND_OPENING,  FALSE,           TRUE,            sm_bypass_state_do_opening,  sm_bypass_state_do_opening_excpt },
  { SM_COND_OPENED,   TRUE,            FALSE,           sm_bypass_state_do_opened,   sm_bypass_state_do_opened_excpt },
  { SM_COND_STARTING, FALSE,           TRUE,            sm_bypass_state_do_starting, sm_bypass_state_do_starting_excpt },
  { SM_COND_STARTED,  TRUE,            FALSE,           sm_bypass_state_do_started,  sm_bypass_state_do_started_excpt },
  { SM_COND_STOPPING, FALSE,           TRUE,            sm_bypass_state_do_stopping, sm_bypass_state_do_stopping_excpt },
  { SM_COND_CLOSING,  FALSE,           TRUE,            sm_bypass_state_do_closing,  sm_bypass_state_do_closing_excpt },
  { SM_COND_CLOSED,   FALSE,           FALSE,           sm_bypass_state_do_closed,   sm_bypass_state_do_closed_excpt },
  { SM_COND_ERROR,    TRUE,            FALSE,           sm_bypass_state_do_error,    sm_bypass_state_do_error_excpt },
  SM_STATE_DESCR_NULL  
};

static UINT32             at_bypass_allowed_pending_event_lst[] =
{
  SM_EVN_BYPASS_APP_PLAY_REQ,
  SM_EVN_BYPASS_APP_STOP_REQ,
  SM_EVN_BYPASS_APP_CLOSE_REQ,
  SM_EVN_BYPASS_SCDB_RECORD_DEL,
  SM_EVN_SESS_APP_PLAY_REQ,
  SM_EVN_SESS_APP_STOP_REQ,
  SM_EVN_BYPASS_RESTART,
  SM_EVN_IGNORE
};

static SM_STATE_COND_T    at_bypass_state_cond_lst[] =
{
  /* ui4_curr_state,   ui4_event,                             ui4_next_state,   ui4_pending_event,           pf_event_do */
  /* ignore state conditions *************************************************/ 
  { SM_COND_IGNORE,   SM_EVN_BYPASS_APP_OPEN_REQ,            SM_COND_OPENING,  SM_EVN_IGNORE,               NULL },
  
  /* opening state conditions ************************************************/ 
  { SM_COND_OPENING,  SM_EVN_BYPASS_DRV_OPENED,              SM_COND_OPENED,   SM_EVN_IGNORE,               NULL },
  { SM_COND_OPENING,  SM_EVN_BYPASS_APP_PLAY_REQ,            SM_COND_OPENING,  SM_EVN_BYPASS_APP_PLAY_REQ,  NULL },
  { SM_COND_OPENING,  SM_EVN_SESS_APP_PLAY_REQ,              SM_COND_OPENING,  SM_EVN_SESS_APP_PLAY_REQ,    NULL },
  { SM_COND_OPENING,  SM_EVN_SESS_APP_STOP_REQ,              SM_COND_OPENING,  SM_EVN_SESS_APP_STOP_REQ,    NULL },   
  { SM_COND_OPENING,  SM_EVN_BYPASS_APP_STOP_REQ,            SM_COND_OPENING,  SM_EVN_BYPASS_APP_STOP_REQ,  NULL },
  { SM_COND_OPENING,  SM_EVN_BYPASS_APP_CLOSE_REQ,           SM_COND_OPENING,  SM_EVN_BYPASS_APP_CLOSE_REQ, NULL },
  { SM_COND_OPENING,  SM_EVN_BYPASS_SCDB_RECORD_DEL,         SM_COND_OPENING,  SM_EVN_BYPASS_APP_CLOSE_REQ, NULL },
  { SM_COND_OPENING,  SM_EVN_BYPASS_DRV_PLAYED,              SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_OPENING,  SM_EVN_BYPASS_DRV_STOPPED,             SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_OPENING,  SM_EVN_BYPASS_DRV_CLOSED,              SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_OPENING,  SM_EVN_BYPASS_DRV_ERR,                 SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_OPENING,  SM_EVN_BYPASS_INTERNAL_ERR,            SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  
  /* opened state conditions *************************************************/ 
  { SM_COND_OPENED,   SM_EVN_BYPASS_APP_PLAY_REQ,            SM_COND_STARTING, SM_EVN_IGNORE,               NULL },
  { SM_COND_OPENED,   SM_EVN_SESS_APP_PLAY_REQ,              SM_COND_STARTING, SM_EVN_IGNORE,               NULL },
  { SM_COND_OPENED,   SM_EVN_BYPASS_APP_CLOSE_REQ,           SM_COND_CLOSING,  SM_EVN_IGNORE,               NULL },
  { SM_COND_OPENED,   SM_EVN_BYPASS_SCDB_RECORD_DEL,         SM_COND_CLOSING,  SM_EVN_IGNORE,               NULL },
  { SM_COND_OPENED,   SM_EVN_BYPASS_DRV_OPENED,              SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_OPENED,   SM_EVN_BYPASS_DRV_PLAYED,              SM_COND_STARTING, SM_EVN_IGNORE,               NULL },
  { SM_COND_OPENED,   SM_EVN_BYPASS_DRV_STOPPED,             SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_OPENED,   SM_EVN_BYPASS_DRV_CLOSED,              SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_OPENED,   SM_EVN_BYPASS_DRV_ERR,                 SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_OPENED,   SM_EVN_BYPASS_INTERNAL_ERR,            SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  
  /* starting state conditions ***********************************************/ 
  { SM_COND_STARTING, SM_EVN_BYPASS_DRV_PLAYED,              SM_COND_STARTED,  SM_EVN_IGNORE,               NULL },
  { SM_COND_STARTING, SM_EVN_BYPASS_APP_STOP_REQ,            SM_COND_STARTING, SM_EVN_BYPASS_APP_STOP_REQ,  NULL },
  { SM_COND_STARTING, SM_EVN_SESS_APP_STOP_REQ,              SM_COND_STARTING, SM_EVN_SESS_APP_STOP_REQ,    NULL },
  { SM_COND_STARTING, SM_EVN_BYPASS_APP_CLOSE_REQ,           SM_COND_STARTING, SM_EVN_BYPASS_APP_CLOSE_REQ, NULL },
  { SM_COND_STARTING, SM_EVN_BYPASS_RESTART,                 SM_COND_STARTING, SM_EVN_BYPASS_RESTART,       NULL },
  { SM_COND_STARTING, SM_EVN_BYPASS_SCDB_RECORD_DEL,         SM_COND_STARTING, SM_EVN_BYPASS_APP_CLOSE_REQ, NULL },
  { SM_COND_STARTING, SM_EVN_BYPASS_DRV_OPENED,              SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_STARTING, SM_EVN_BYPASS_DRV_STOPPED,             SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_STARTING, SM_EVN_BYPASS_DRV_CLOSED,              SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_STARTING, SM_EVN_BYPASS_DRV_ERR,                 SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_STARTING, SM_EVN_BYPASS_INTERNAL_ERR,            SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  
  /* started state conditions ************************************************/ 
  { SM_COND_STARTED,  SM_EVN_BYPASS_APP_STOP_REQ,            SM_COND_STOPPING, SM_EVN_IGNORE,               NULL },
  { SM_COND_STARTED,  SM_EVN_SESS_APP_STOP_REQ,              SM_COND_STOPPING, SM_EVN_IGNORE,               NULL },
  { SM_COND_STARTED,  SM_EVN_BYPASS_APP_CLOSE_REQ,           SM_COND_STOPPING, SM_EVN_BYPASS_APP_CLOSE_REQ, NULL },
  { SM_COND_STARTED,  SM_EVN_BYPASS_RESTART,                 SM_COND_STOPPING, SM_EVN_BYPASS_APP_PLAY_REQ,  NULL },
  { SM_COND_STARTED,  SM_EVN_BYPASS_SCDB_RECORD_DEL,         SM_COND_STOPPING, SM_EVN_BYPASS_APP_CLOSE_REQ, NULL },
  { SM_COND_STARTED,  SM_EVN_BYPASS_DRV_OPENED,              SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_STARTED,  SM_EVN_BYPASS_DRV_PLAYED,              SM_COND_STARTED,  SM_EVN_IGNORE,               NULL },
  { SM_COND_STARTED,  SM_EVN_BYPASS_DRV_STOPPED,             SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_STARTED,  SM_EVN_BYPASS_DRV_CLOSED,              SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_STARTED,  SM_EVN_BYPASS_DRV_ERR,                 SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_STARTED,  SM_EVN_BYPASS_INTERNAL_ERR,            SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  
  /* stopping state conditions ***********************************************/ 
  { SM_COND_STOPPING, SM_EVN_BYPASS_DRV_STOPPED,             SM_COND_OPENED,   SM_EVN_IGNORE,               NULL },
  { SM_COND_STOPPING, SM_EVN_BYPASS_APP_CLOSE_REQ,           SM_COND_STOPPING, SM_EVN_BYPASS_APP_CLOSE_REQ, NULL },
  { SM_COND_STOPPING, SM_EVN_BYPASS_SCDB_RECORD_DEL,         SM_COND_STOPPING, SM_EVN_BYPASS_APP_CLOSE_REQ, NULL },
  { SM_COND_STOPPING, SM_EVN_BYPASS_APP_PLAY_REQ,            SM_COND_STOPPING, SM_EVN_BYPASS_APP_PLAY_REQ,  NULL },
  { SM_COND_STOPPING, SM_EVN_SESS_APP_PLAY_REQ,              SM_COND_STOPPING, SM_EVN_SESS_APP_PLAY_REQ,    NULL },
  { SM_COND_STOPPING, SM_EVN_BYPASS_DRV_OPENED,              SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_STOPPING, SM_EVN_BYPASS_DRV_PLAYED,              SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_STOPPING, SM_EVN_BYPASS_DRV_CLOSED,              SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_STOPPING, SM_EVN_BYPASS_DRV_ERR,                 SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_STOPPING, SM_EVN_BYPASS_INTERNAL_ERR,            SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  
  /* closing state conditions ************************************************/ 
  { SM_COND_CLOSING,  SM_EVN_BYPASS_DRV_CLOSED,              SM_COND_CLOSED,   SM_EVN_IGNORE,               NULL },
  { SM_COND_CLOSING,  SM_EVN_BYPASS_DRV_ERR,                 SM_COND_CLOSING,  SM_EVN_IGNORE,               NULL },
  { SM_COND_CLOSING,  SM_EVN_BYPASS_INTERNAL_ERR,            SM_COND_CLOSING,  SM_EVN_IGNORE,               NULL },

  /* error state conditions ************************************************/ 
  { SM_COND_ERROR,    SM_EVN_BYPASS_APP_CLOSE_REQ,           SM_COND_CLOSING,  SM_EVN_IGNORE,               NULL },
  { SM_COND_ERROR,    SM_EVN_BYPASS_SCDB_RECORD_DEL,         SM_COND_CLOSING,  SM_EVN_IGNORE,               NULL },
  { SM_COND_ERROR,    SM_EVN_BYPASS_APP_PLAY_REQ,            SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_ERROR,    SM_EVN_SESS_APP_PLAY_REQ,              SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_ERROR,    SM_EVN_BYPASS_APP_STOP_REQ,            SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_ERROR,    SM_EVN_SESS_APP_STOP_REQ,              SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_ERROR,    SM_EVN_BYPASS_DRV_OPENED,              SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_ERROR,    SM_EVN_BYPASS_DRV_PLAYED,              SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_ERROR,    SM_EVN_BYPASS_DRV_STOPPED,             SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },
  { SM_COND_ERROR,    SM_EVN_BYPASS_DRV_CLOSED,              SM_COND_ERROR,    SM_EVN_IGNORE,               NULL },

  /* ANY state events ********************************************************/ 
  { SM_STATE_IGNORE,  SM_EVN_CCI_INFORMATION,                SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_cci_events },
  { SM_STATE_IGNORE,  SM_EVN_CA_COND_CLOSED,                 SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_ca_events },
  { SM_STATE_IGNORE,  SM_EVN_CA_COND_UNKNOWN,                SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_ca_events },
  { SM_STATE_IGNORE,  SM_EVN_CA_COND_FREE,                   SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_ca_events },
  { SM_STATE_IGNORE,  SM_EVN_CA_COND_AUTHORIZED,             SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_ca_events },
  { SM_STATE_IGNORE,  SM_EVN_CA_COND_AUTHORIZED_CA_SYS_ID,   SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_ca_events },
  { SM_STATE_IGNORE,  SM_EVN_CA_COND_NOT_AUTHORIZED,         SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_ca_events },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_DEC_OPENED,              SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_DEC_CLOSED,              SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_DEC_CONNECTED,           SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_DEC_DISCONNECTED,        SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_DEC_PLAYED,              SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_DEC_STOPPED,             SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_TVE_OPENED,              SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_TVE_CLOSED,              SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_TVE_CONNECTED,           SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_TVE_DISCONNECTED,        SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_TVE_PLAYED,              SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_TVE_STOPPED,             SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_VOUT_OPENED,             SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_VOUT_CLOSED,             SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_VOUT_CONNECTED,          SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_VOUT_DISCONNECTED,       SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_VOUT_PLAYED,             SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_VOUT_STOPPED,            SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_PM_COND_CLOSED,          SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_PM_COND_PLAYED,          SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_PM_COND_STOPPED,         SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_VIDEO_RESOLUTION_CHG,    SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_res_chg },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_ANA_TV_SIGNAL,           SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_tv_sig },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_ANA_TV_SIGNAL_DETECTING, SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_tv_sig },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_ANA_COLOR_SYSTEM,        SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_color_sys },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_HDCP_CONTENT,            SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_event_nfy },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_MACROVISION,             SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_event_nfy },
  { SM_STATE_IGNORE,  SM_EVN_BYPASS_CODEC_NOT_SUPPORT,       SM_STATE_IGNORE,  SM_EVN_IGNORE,               sm_bypass_state_do_event_nfy },  
  
  /* !!! Don't remove this !!!************************************************/
  SM_STATE_COND_NULL
};

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
#ifdef CI_PLUS_SUPPORT
static INT32 _sm_bypass_hdlr_set_cci_plus_info(
    SM_BYPASS_STREAM_T*    pt_bypass_obj,
    CP_INFO_LIST_T*        pt_cp_info_lst );
#else    
static INT32 _sm_bypass_hdlr_set_cci_info( 
    SM_BYPASS_STREAM_T*    pt_bypass_obj,
    CP_INFO_T*             pt_cp_info );
#endif

/*-----------------------------------------------------------------------------
                    Internal function implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_hdlr_do_msg_do_event
 *
 * Description: <function description>
 *
 * Inputs:  h_stream      References.
 *          pt_bypass_obj References.
 *          pt_msg        References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_bypass_hdlr_do_msg_do_event( 
    HANDLE_T                          h_stream,
    SM_BYPASS_STREAM_T*               pt_bypass_obj,
    SM_BYPASS_HDLR_MSG_DO_EVENT_T*    pt_msg )
{
    sm_state_on_event( &pt_bypass_obj->t_state,
                       pt_msg->ui4_event,
                       (VOID*)pt_bypass_obj,
                       (VOID*)pt_msg->ui4_data,
                       NULL );
}

/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_hdlr_record_rm_nfy_cond
 *
 * Description: <function description>
 *
 * Inputs:  pt_drv_descr  References.
 *          e_old_cond  References.
 *          pt_msg    References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_bypass_hdlr_record_rm_nfy_cond(
    SM_DRV_COMP_DESCR_T*            pt_drv_descr,
    RM_COND_T                       e_old_cond,
    SM_BYPASS_HDLR_MSG_RM_NFY_T*    pt_msg )
{
    if ( pt_drv_descr->t_info.ui1_in_port == ANY_PORT_NUM ||
         pt_msg->ui1_port == ANY_PORT_NUM )
    {
        if ( pt_msg->b_should_update_comp_descr &&
             pt_drv_descr->e_comp_cond != RM_COND_CLOSED )
        {
            pt_drv_descr->e_comp_cond_org = e_old_cond;
            pt_drv_descr->e_comp_cond = pt_msg->e_cond;
        }
    }
    else
    {
        if ( pt_msg->b_should_update_comp_descr &&
             pt_drv_descr->e_comp_cond != RM_COND_CLOSED )
        {
            pt_drv_descr->e_comp_port_cond_org = e_old_cond;
            pt_drv_descr->e_comp_port_cond = pt_msg->e_cond;
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_hdlr_do_msg_rm_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_stream      References.
 *          pt_bypass_obj References.
 *          pt_msg        References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_bypass_hdlr_do_msg_rm_nfy(
    HANDLE_T                        h_stream,
    SM_BYPASS_STREAM_T*             pt_bypass_obj,
    SM_EVN_TYPE_T                   e_event_grp,
    SM_BYPASS_HDLR_MSG_RM_NFY_T*    pt_msg )
{
    SM_DRV_COMP_DESCR_T* pt_drv_descr = NULL; 
    RM_COND_T            e_cond = RM_COND_IGNORE;
    RM_COND_T            e_old_cond = RM_COND_IGNORE;
    UINT32               ui4_latest_event = SM_EVN_IGNORE;
    
    switch( e_event_grp )
    {
    case SM_EVN_BYPASS_GRP_DEC:
        pt_drv_descr = &pt_bypass_obj->t_bypass_comp_grp.t_dec;
        ui4_latest_event = pt_bypass_obj->ui4_lastest_dec_event;
        break;

    case SM_EVN_BYPASS_GRP_TVE:
        pt_drv_descr = &pt_bypass_obj->t_bypass_comp_grp.t_tve;
        ui4_latest_event = pt_bypass_obj->ui4_lastest_tve_event;
        break;

    case SM_EVN_BYPASS_GRP_VOUT:
        pt_drv_descr = &pt_bypass_obj->t_bypass_comp_grp.t_vout;
        ui4_latest_event = pt_bypass_obj->ui4_lastest_vout_event;
        break;

    default:
        SM_BYPASS_DBG_ERR(( "[SM] %s:%d invalid driver event group 0x%.8X!\r\n", 
                            __FILE__, __LINE__, e_event_grp ));
        SM_ABORT( SM_DBG_ABRT_INV_EVENT );
    }

    /* Filter out all set/get operation notify.  */
    e_cond = rm_cond_chg( pt_msg->e_cond, 
                          RM_COND_IGNORE, 
                          COND_GRP_OPERATION );
    if( e_cond != RM_COND_NO_CHG )
    {
        /* check if any set operation for Video Plane */
        switch( e_cond )
        {
        case RM_COND_SET_OPERATION:
        case RM_COND_CLIENT_SET_OPERATION:
            break;
        
        default:
            break;
        }
        
        return;
    }

    /* check if it's a component condition */
    if ( pt_drv_descr->t_info.ui1_in_port == ANY_PORT_NUM ||
         pt_msg->ui1_port == ANY_PORT_NUM )
    {
        if ( pt_msg->b_should_update_comp_descr &&
             pt_drv_descr->e_comp_cond != RM_COND_CLOSED )
        {
            e_old_cond = pt_drv_descr->e_comp_cond;
        }
        else
        {
            e_old_cond = pt_drv_descr->e_comp_cond_org;
        }
    }
    else
    {
        if ( pt_msg->b_should_update_comp_descr &&
             pt_drv_descr->e_comp_cond != RM_COND_CLOSED )
        {
            e_old_cond = pt_drv_descr->e_comp_port_cond;
        }
        else
        {
            e_old_cond = pt_drv_descr->e_comp_port_cond_org;
        }        
    }
    
    /* check condition group COND_GRP_OBJ_STATE */
    e_cond = rm_cond_chg( pt_msg->e_cond, 
                          e_old_cond, 
                          COND_GRP_OBJ_STATE );
    switch( e_cond )
    {
    case RM_COND_CLOSED:
        pt_drv_descr->h_comp = NULL_HANDLE;
        _sm_bypass_hdlr_record_rm_nfy_cond(pt_drv_descr,
                                           e_old_cond,
                                           pt_msg);
        sm_state_on_event( &pt_bypass_obj->t_state,
                           SM_MAKE_DRV_EVN( e_event_grp, SM_EVN_BYPASS_CODE_DRV_CLOSED),
                           (VOID*)pt_bypass_obj,
                           (VOID*)NULL,
                           NULL );
        return;
        
    case RM_COND_OPENED:
        _sm_bypass_hdlr_record_rm_nfy_cond(pt_drv_descr,
                                           e_old_cond,
                                           pt_msg);
        sm_state_on_event( &pt_bypass_obj->t_state,
                           SM_MAKE_DRV_EVN( e_event_grp, SM_EVN_BYPASS_CODE_DRV_OPENED),
                           (VOID*)pt_bypass_obj,
                           (VOID*)NULL,
                           NULL );
        return;
        
    case RM_COND_NO_CHG:
        break;

    default:
        SM_BYPASS_DBG_ERR(( 
            "[SM] BYPASS, unhandled RM object state = %u\r\n", e_cond ));
        break;
    }

    /* check if the connection state is hard wired */
    e_cond = rm_cond_chg( pt_msg->e_cond, 
                          RM_COND_IGNORE, 
                          COND_GRP_CONN_STATE );
    if ( e_cond == RM_COND_HARD_WIRED && 
         (ui4_latest_event&(~SM_EVN_GRP_BIT_MASK)) == SM_EVN_BYPASS_CODE_DRV_OPENED )
    {
        _sm_bypass_hdlr_record_rm_nfy_cond(pt_drv_descr,
                                           e_old_cond,
                                           pt_msg);
        sm_state_on_event( &pt_bypass_obj->t_state,
                           SM_MAKE_DRV_EVN( e_event_grp, SM_EVN_BYPASS_CODE_DRV_CONNECTED),
                           (VOID*)pt_bypass_obj,
                           (VOID*)NULL,
                           NULL );
        return;
    }
    if ( e_cond == RM_COND_HARD_WIRED && 
         (ui4_latest_event&(~SM_EVN_GRP_BIT_MASK)) == SM_EVN_BYPASS_CODE_DRV_CONNECTED )
    {
        _sm_bypass_hdlr_record_rm_nfy_cond(pt_drv_descr,
                                           e_old_cond,
                                           pt_msg);
        sm_state_on_event( &pt_bypass_obj->t_state,
                           SM_MAKE_DRV_EVN( e_event_grp, SM_EVN_BYPASS_CODE_DRV_DISCONNECTED),
                           (VOID*)pt_bypass_obj,
                           (VOID*)NULL,
                           NULL );
        return;
    }
    
    /* check condition group COND_GRP_CONN_STATE */
    e_cond = rm_cond_chg( pt_msg->e_cond, 
                          e_old_cond, 
                          COND_GRP_CONN_STATE );
    switch( e_cond )
    {
    case RM_COND_DISCONNECTED:
        _sm_bypass_hdlr_record_rm_nfy_cond(pt_drv_descr,
                                           e_old_cond,
                                           pt_msg);
        sm_state_on_event( &pt_bypass_obj->t_state,
                           SM_MAKE_DRV_EVN( e_event_grp, SM_EVN_BYPASS_CODE_DRV_DISCONNECTED),
                           (VOID*)pt_bypass_obj,
                           (VOID*)NULL,
                           NULL );
        return;
        
    case RM_COND_CONNECTED:
        if ( pt_bypass_obj->t_bypass_comp_grp.e_grp_status == SM_BYPASS_COMP_GRP_STATUS_OPENED )
        {
            _sm_bypass_hdlr_record_rm_nfy_cond(pt_drv_descr,
                                               e_old_cond,
                                               pt_msg);
            sm_state_on_event( &pt_bypass_obj->t_state,
                               SM_MAKE_DRV_EVN( e_event_grp, SM_EVN_BYPASS_CODE_DRV_CONNECTED),
                               (VOID*)pt_bypass_obj,
                               (VOID*)NULL,
                               NULL );
        }
        return;
    
    case RM_COND_MULTI_PORT:
        break;
            
    case RM_COND_NO_CHG:
        break;
        
    default:
        SM_BYPASS_DBG_ERR(( 
            "[SM] BYPASS, unhandled RM connection state = 0x%.8X\r\n", e_cond ));
        break;
    }
}

#ifdef DEBUG
static CHAR* _sm_bypass_hdlr_get_dec_status_str(
    VID_DEC_DECODE_STATUS_T   e_dec_status )
{
    switch(e_dec_status)
    {
    case VID_DEC_DECODE_UNKNOWN:
        return "VID_DEC_DECODE_UNKNOWN";

    case VID_DEC_DECODE_NO_DATA:
        return "VID_DEC_DECODE_NO_DATA";

    case VID_DEC_DECODE_NORMAL:
        return "VID_DEC_DECODE_NORMAL";

    case VID_DEC_DECODE_ERROR:
        return "VID_DEC_DECODE_ERROR";

    case VID_DEC_DECODE_HD_NOT_SUPPORT:
        return "VID_DEC_DECODE_HD_NOT_SUPPORT";

    case VID_DEC_DECODE_CODEC_NOT_SUPPORT:
        return "VID_DEC_DECODE_CODEC_NOT_SUPPORT";
        
    case VID_DEC_DECODE_CODEC_TYPE_CHG:
        return "VID_DEC_DECODE_CODEC_TYPE_CHG";

    case VID_DEC_DECODE_DATA_RECEIVE:
        return "VID_DEC_DECODE_DATA_RECEIVE";

    case VID_DEC_DECODE_RES_NOT_SUPPORT:
        return "VID_DEC_DECODE_RES_NOT_SUPPORT";

    default:
        return "INVALID_VALUE";
    }
}
#endif /* DEBUG */

/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_hdlr_do_msg_vd_ctrl_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_stream      References.
 *          pt_bypass_obj References.
 *          pt_msg        References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_bypass_hdlr_do_msg_vd_ctrl_nfy( 
    HANDLE_T                                   h_stream,
    SM_BYPASS_STREAM_T*                        pt_bypass_obj,
    const SM_BYPASS_HDLR_MSG_VD_CTRL_NFY_T*    pt_msg )
{
    VID_ENC_T e_nfy_actual_enc;
    
    switch( pt_msg->e_vd_nfy_cond )
    {
    case VID_DEC_COND_ERROR:
        /* to-do, reset video decoder */
        sm_state_on_event( &pt_bypass_obj->t_state,
                           SM_EVN_BYPASS_INTERNAL_ERR,
                           (VOID*)pt_bypass_obj,
                           (VOID*)SMR_DRV_COMP_ERROR,
                           NULL );
        break;
            
    case VID_DEC_COND_CTRL_DONE:
        /* change state and notify sess handler */
        /* had changed for CR: DTV00091838 */
        if ( (pt_bypass_obj->u_dec_ctrl_type.e_vd_ctrl_type == VID_DEC_CTRL_PLAY &&
              (VID_DEC_CTRL_T)pt_msg->ui4_data_1 == VID_DEC_CTRL_PLAY)||
              (pt_bypass_obj->u_dec_ctrl_type.e_vd_ctrl_type == VID_DEC_CTRL_FREEZE &&
              (VID_DEC_CTRL_T)pt_msg->ui4_data_1 == VID_DEC_CTRL_FREEZE))
        {
            sm_state_on_event( &pt_bypass_obj->t_state,
                               SM_EVN_BYPASS_DEC_PLAYED,
                               (VOID*)pt_bypass_obj,
                               (VOID*)NULL,
                               NULL );
        }
        else if ( pt_bypass_obj->u_dec_ctrl_type.e_vd_ctrl_type == VID_DEC_CTRL_STOP &&
                  (VID_DEC_CTRL_T)pt_msg->ui4_data_1 == VID_DEC_CTRL_STOP)
        {
            sm_state_on_event( &pt_bypass_obj->t_state,
                               SM_EVN_BYPASS_DEC_STOPPED,
                               (VOID*)pt_bypass_obj,
                               (VOID*)NULL,
                               NULL );
        }
        else if ( pt_bypass_obj->u_dec_ctrl_type.e_vd_ctrl_type == VID_DEC_CTRL_RESET &&
                  (VID_DEC_CTRL_T)pt_msg->ui4_data_1 == VID_DEC_CTRL_RESET)
        {
            /* not used currently, iolo */
        }
        else if ( (VID_DEC_CTRL_T)pt_msg->ui4_data_1 != VID_DEC_CTRL_RESET &&
                  (VID_DEC_CTRL_T)pt_msg->ui4_data_1 != VID_DEC_CTRL_STOP &&
                  (VID_DEC_CTRL_T)pt_msg->ui4_data_1 != VID_DEC_CTRL_FREEZE &&
                  (VID_DEC_CTRL_T)pt_msg->ui4_data_1 != VID_DEC_CTRL_PLAY &&
                  (VID_DEC_CTRL_T)pt_msg->ui4_data_1 != VID_DEC_CTRL_PLAY_I_FRAME )
        {
            SM_BYPASS_DBG_ERR(( 
                "[SM] BYPASS(0x%.8X), invalid VID_DEC_CTRL_T = 0x%.8X\r\n",
                h_stream, (VID_DEC_CTRL_T)pt_msg->ui4_data_1 ));
            SM_ASSERT(0);
        }
        break;

    case VID_DEC_COND_RESOLUTION_CHG:
        sm_state_on_event( &pt_bypass_obj->t_state,
                           SM_EVN_BYPASS_VIDEO_RESOLUTION_CHG,
                           (VOID*)pt_bypass_obj,
                           (VOID*)NULL,
                           NULL );
        break;

    case VID_DEC_COND_DECODE_STATUS_CHG:
        {
            if ( (VID_DEC_DECODE_STATUS_T)pt_msg->ui4_data_1 < VID_DEC_DECODE_UNKNOWN ||
                 (VID_DEC_DECODE_STATUS_T)pt_msg->ui4_data_1 > VID_DEC_DECODE_RES_NOT_SUPPORT)
            {
                SM_ASSERT(0);
                return;
            }

            SM_BYPASS_DBG_INFO((
                "[SM] BYPASS, VD notify VID_DEC_COND_DECODE_STATUS_CHG (%s)\r\n",
                _sm_bypass_hdlr_get_dec_status_str( (VID_DEC_DECODE_STATUS_T)pt_msg->ui4_data_1 ) ));

            if (((VID_DEC_DECODE_STATUS_T)pt_msg->ui4_data_1) == VID_DEC_DECODE_CODEC_TYPE_CHG)
            {
                sm_bypass_comp_convert_vd_codec_to_scdb_enc( 
                                    (VID_DEC_CODEC_T)pt_msg->ui4_data_2,
                                    &e_nfy_actual_enc );
                if (pt_bypass_obj->e_actual_enc != e_nfy_actual_enc)
                {
                    pt_bypass_obj->e_actual_enc = e_nfy_actual_enc;
                    /* restart video play */
                    sm_bypass_hdlr_send_msg_do_event(
                        pt_bypass_obj->t_base.h_stream,
                        SM_EVN_BYPASS_RESTART,
                        (UINT32)pt_bypass_obj->t_base.i4_speed );

                    SM_BYPASS_DBG_INFO((
                        "[SM] bypass stream(0x%.8X) codec type changed to (%u)!\r\n",
                        (UINT32)pt_bypass_obj->t_base.h_stream,
                        (UINT32)pt_bypass_obj->e_actual_enc ));
                }
            }
        }
        break;

    default:
        SM_BYPASS_DBG_ERR(( 
            "[SM] BYPASS, invalid Video Decoder condition = 0x%.8X\r\n",
            pt_msg->e_vd_nfy_cond ));
        SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_hdlr_drv_color_sys_2_vsh
 *
 * Description: <function description>
 *
 * Inputs:  e_color_sys  References.
 *
 * Outputs: - 
 *
 * Returns: color system.
 *
 ----------------------------------------------------------------------------*/
static COLOR_SYS_T _sm_bypass_hdlr_drv_color_sys_2_vsh(
    TV_DEC_COLOR_SYS_T    e_color_sys )
{
    switch( e_color_sys )
    {
    case TV_DEC_COLOR_SYS_NTSC:
    case TV_DEC_COLOR_SYS_NTSC_443:
        return COLOR_SYS_NTSC;

    case TV_DEC_COLOR_SYS_PAL:
    case TV_DEC_COLOR_SYS_PAL_N:
    case TV_DEC_COLOR_SYS_PAL_M:
    case TV_DEC_COLOR_SYS_PAL_60:
        return COLOR_SYS_PAL;
 
    case TV_DEC_COLOR_SYS_SECAM:
        return COLOR_SYS_SECAM;

    case TV_DEC_COLOR_SYS_UNKNOWN:
    default:
        return COLOR_SYS_UNKNOWN;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_hdlr_get_tv_sig_str
 *
 * Description: <function description>
 *
 * Inputs:  e_color_sys  References.
 *
 * Outputs: - 
 *
 * Returns: string of tv signal if success.
 *          NULL if error.
 *
 ----------------------------------------------------------------------------*/
#ifdef DEBUG
static CHAR* _sm_bypass_hdlr_get_tv_sig_str(
    TV_DEC_TV_SIG_T   e_tv_sig )
{
    switch(e_tv_sig)
    {
    case TV_DEC_TV_SIG_UNKNOWN:
        return "TV_DEC_TV_SIG_UNKNOWN";
        
    case TV_DEC_TV_SIG_NONE:
        return "TV_DEC_TV_SIG_NONE";
        
    case TV_DEC_TV_SIG_STABLE:
        return "TV_DEC_TV_SIG_STABLE";
        
    default:
        return "INVALID_VALUE";
    }
}
#endif /* DEBUG */

/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_hdlr_do_msg_tvd_ctrl_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_stream      References.
 *          pt_bypass_obj References.
 *          pt_msg        References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_bypass_hdlr_do_msg_tvd_ctrl_nfy( 
    HANDLE_T                                 h_stream,
    SM_BYPASS_STREAM_T*                      pt_bypass_obj,
    const SM_BYPASS_HDLR_MSG_TVD_CTRL_NFY_T* pt_msg )
{
    switch( pt_msg->e_tvd_nfy_cond )
    {
    case TV_DEC_COND_ERROR:
        /* to-do, reset TV decoder */
        if ( TV_DEC_SET_TYPE_CTRL == (DRV_SET_TYPE_T)pt_msg->ui4_data_1 )
        {
            sm_state_on_event( &pt_bypass_obj->t_state,
                               SM_EVN_BYPASS_INTERNAL_ERR,
                               (VOID*)pt_bypass_obj,
                               (VOID*)SMR_DRV_COMP_ERROR,
                               NULL );
        }
        else if ( TV_DEC_SET_TYPE_AUTO_CLK_PHS_POS == (DRV_SET_TYPE_T)pt_msg->ui4_data_1 )
        {
        }
        else if ( TV_DEC_SET_TYPE_AUTO_COLOR == (DRV_SET_TYPE_T)pt_msg->ui4_data_1 )
        {
        }
        else
        {
            SM_BYPASS_DBG_ERR(( 
                "[SM] BYPASS, _sm_bypass_hdlr_do_msg_tvd_ctrl_nfy() "
                "invalid (TV_DEC_CTRL_T)ui4_data_1 = 0x%.8X\r\n",
                pt_bypass_obj->u_dec_ctrl_type.e_tvd_ctrl_type ));
            SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
        }
        break;
        
    case TV_DEC_COND_CTRL_DONE:
        /* change state and notify sess handler */
        if ( TV_DEC_SET_TYPE_CTRL == (DRV_SET_TYPE_T)pt_msg->ui4_data_1 )
        {
            if ( pt_bypass_obj->u_dec_ctrl_type.e_tvd_ctrl_type == TV_DEC_CTRL_PLAY )
            {
                sm_state_on_event( &pt_bypass_obj->t_state,
                                   SM_EVN_BYPASS_DEC_PLAYED,
                                   (VOID*)pt_bypass_obj,
                                   (VOID*)NULL,
                                   NULL );
            }
            else if ( pt_bypass_obj->u_dec_ctrl_type.e_tvd_ctrl_type == TV_DEC_CTRL_STOP )
            {
                sm_state_on_event( &pt_bypass_obj->t_state,
                                   SM_EVN_BYPASS_DEC_STOPPED,
                                   (VOID*)pt_bypass_obj,
                                   (VOID*)NULL,
                                   NULL );
            }
            else if ( pt_bypass_obj->u_dec_ctrl_type.e_tvd_ctrl_type == TV_DEC_CTRL_RESET )
            {
                /* not used currently, iolo */
            }
            else 
            {
                SM_BYPASS_DBG_ERR(( 
                    "[SM] BYPASS, _sm_bypass_hdlr_do_msg_tvd_ctrl_nfy() "
                    "invalid TV_DEC_CTRL_T = 0x%.8X\r\n",
                    pt_bypass_obj->u_dec_ctrl_type.e_tvd_ctrl_type ));
                SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
            }
        }
        else if ( TV_DEC_SET_TYPE_AUTO_CLK_PHS_POS == 
                  (DRV_SET_TYPE_T)pt_msg->ui4_data_1 )
        {
        }
        else if ( TV_DEC_SET_TYPE_AUTO_COLOR == 
                  (DRV_SET_TYPE_T)pt_msg->ui4_data_1 )
        {
        }
        else
        {
            SM_BYPASS_DBG_ERR(( 
                "[SM] BYPASS, _sm_bypass_hdlr_do_msg_tvd_ctrl_nfy() "
                "invalid (TV_DEC_CTRL_T)ui4_data_1 = 0x%.8X\r\n",
                pt_bypass_obj->u_dec_ctrl_type.e_tvd_ctrl_type ));
            SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
        }
        break;
                
    case TV_DEC_COND_RESOLUTION_CHG:
        sm_state_on_event( &pt_bypass_obj->t_state,
                           SM_EVN_BYPASS_VIDEO_RESOLUTION_CHG,
                           (VOID*)pt_bypass_obj,
                           (VOID*)NULL,
                           NULL );
        break;

    case TV_DEC_COND_TV_SIGNAL:
        {            
            if ( (TV_DEC_TV_SIG_T)pt_msg->ui4_data_1 != TV_DEC_TV_SIG_UNKNOWN &&
                 (TV_DEC_TV_SIG_T)pt_msg->ui4_data_1 != TV_DEC_TV_SIG_NONE &&
                 (TV_DEC_TV_SIG_T)pt_msg->ui4_data_1 != TV_DEC_TV_SIG_STABLE )
            {
                SM_ASSERT(0);
                return;
            }
            
            SM_BYPASS_DBG_INFO(( 
                "[SM] BYPASS, TVD notify TV_DEC_COND_TV_SIGNAL (%s)\r\n",
                _sm_bypass_hdlr_get_tv_sig_str( (TV_DEC_TV_SIG_T)pt_msg->ui4_data_1 ) ));

            /* tvd will notify TV_DEC_TV_SIG_UNKNOWN while MODE_CHG due to weak signal */
            if ( ((TV_DEC_TV_SIG_T)pt_msg->ui4_data_1) == TV_DEC_TV_SIG_UNKNOWN )
            {
                x_memset( &pt_bypass_obj->t_vid_res, 0, sizeof(pt_bypass_obj->t_vid_res) );
            }
            if ( ((TV_DEC_TV_SIG_T)pt_msg->ui4_data_1) != pt_bypass_obj->e_tv_signal )
            {
                sm_state_on_event( &pt_bypass_obj->t_state,
                                   SM_EVN_BYPASS_ANA_TV_SIGNAL,
                                   (VOID*)pt_bypass_obj,
                                   (VOID*)pt_msg->ui4_data_1,
                                   NULL );
            }
        }
        break;
        
    case TV_DEC_COND_COLOR_SYS:
        {
            COLOR_SYS_T    e_vid_color_sys;
            
            e_vid_color_sys = _sm_bypass_hdlr_drv_color_sys_2_vsh(
                (TV_DEC_COLOR_SYS_T)pt_msg->ui4_data_1 );
            
            if ( e_vid_color_sys != pt_bypass_obj->e_color_sys )
            {
                pt_bypass_obj->e_color_sys = e_vid_color_sys;
                sm_state_on_event( &pt_bypass_obj->t_state,
                                   SM_EVN_BYPASS_ANA_COLOR_SYSTEM,
                                   (VOID*)pt_bypass_obj,
                                   (VOID*)(UINT32)e_vid_color_sys,
                                   NULL );
            }
        }
        break;
        
    case TV_DEC_COND_CONTENT_ADV_INFO:
        break;

    case TV_DEC_COND_TV_SIGNAL_DETECTING:
        sm_state_on_event( &pt_bypass_obj->t_state,
                           SM_EVN_BYPASS_ANA_TV_SIGNAL_DETECTING,
                           (VOID*)pt_bypass_obj,
                           (VOID*)NULL,
                           NULL );
        break;

    case TV_DEC_COND_HDCP_CONTENT:
        {
            SM_BYPASS_DBG_INFO(( 
                "[SM] BYPASS, TVD notify TV_DEC_COND_HDCP_CONTENT (%d)\r\n",
                pt_msg->ui4_data_1 ));
                
            sm_state_on_event( &pt_bypass_obj->t_state,
                               SM_EVN_BYPASS_HDCP_CONTENT,
                               (VOID*)pt_bypass_obj,
                               (VOID*)pt_msg->ui4_data_1,
                               NULL );
        }
        break;

    case TV_DEC_COND_MACROVISION:
        {
            SM_BYPASS_DBG_INFO(( 
                "[SM] BYPASS, TVD notify TV_DEC_COND_MACROVISION (%d)\r\n",
                pt_msg->ui4_data_1 ));
                
            sm_state_on_event( &pt_bypass_obj->t_state,
                               SM_EVN_BYPASS_MACROVISION,
                               (VOID*)pt_bypass_obj,
                               (VOID*)pt_msg->ui4_data_1,
                               NULL );
        }
        break;
    
    default:
        SM_BYPASS_DBG_ERR(( 
            "[SM] BYPASS, invalid TV Decoder condition = 0x%.8X\r\n",
            pt_msg->e_tvd_nfy_cond ));
        SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_hdlr_do_msg_pm_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_stream      References.
 *          pt_bypass_obj References.
 *          pt_msg        References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_bypass_hdlr_do_msg_pm_nfy( 
    HANDLE_T                              h_stream,
    SM_BYPASS_STREAM_T*                   pt_bypass_obj,
    const SM_BYPASS_HDLR_MSG_PM_NFY_T*    pt_msg )
{
    sm_state_on_event( 
        &pt_bypass_obj->t_state,
        pt_msg->ui4_pm_event,
        (VOID*)pt_bypass_obj,
        (VOID*)pt_msg->ui4_detail_code,
        NULL );
}

/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_hdlr_do_msg_cci_msg
 *
 * Description: <function description>
 *
 * Inputs:  h_stream      References.
 *          pt_bypass_obj References.
 *          pt_msg        References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_bypass_hdlr_do_msg_cci_msg(
    HANDLE_T                               h_stream,
    SM_BYPASS_STREAM_T*                    pt_bypass_obj,
    const SM_BYPASS_HDLR_MSG_CCI_NFY_T*    pt_msg )
{
    sm_state_on_event(
        &pt_bypass_obj->t_state,
        SM_EVN_CCI_INFORMATION,
        (VOID*)pt_bypass_obj,
        (VOID*)&pt_msg->t_pm_cp_info_lst,
        NULL );
}

/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_hdlr_do_msg_ca_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_stream      References.
 *          pt_bypass_obj References.
 *          pt_msg        References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_bypass_hdlr_do_msg_ca_nfy(
    HANDLE_T                              h_stream,
    SM_BYPASS_STREAM_T*                   pt_bypass_obj,
    const SM_BYPASS_HDLR_MSG_CA_NFY_T*    pt_msg )
{
    sm_state_on_event(
        &pt_bypass_obj->t_state,
        pt_msg->ui4_ca_event,
        (VOID*)pt_bypass_obj,
        (VOID*)pt_msg->ui4_detail_code,
        NULL );
}

/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_hdlr_do_msg_scdb_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_stream      References.
 *          pt_bypass_obj References.
 *          pt_msg        References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_bypass_hdlr_do_msg_scdb_nfy(
    HANDLE_T                                h_stream,
    SM_BYPASS_STREAM_T*                     pt_bypass_obj,
    const SM_BYPASS_HDLR_MSG_SCDB_NFY_T*    pt_msg )
{
    INT32                i4_ret;
    UINT32               ui4_reason = pt_msg->ui4_reason;
    SCDB_COND_T          e_scdb_cond = pt_msg->e_scdb_cond;
    SCDB_REC_T           t_scdb_rec = {0};
    SM_BYPASS_STATE_TAG_T  t_state_tag;
    
    t_state_tag.pt_bypass_obj = pt_bypass_obj;
    t_state_tag.h_stream = (HANDLE_T)pt_bypass_obj->t_base.h_stream;

#ifdef DEBUG
    const CHAR* as_scdb_vid_fmt[] = 
    {
        "VID_FMT_UNKNOWN", "VID_FMT_HD_4_3", "VID_FMT_HD_16_9",
        "VID_FMT_SD_4_3", "VID_FMT_SD_16_9"
    };
    const CHAR* as_scdb_vid_color_sys[] = 
    {
        "COLOR_SYS_UNKNOWN", "COLOR_SYS_NTSC", "COLOR_SYS_PAL",
        "COLOR_SYS_SECAM"
    };
#endif
    
    
    if ( e_scdb_cond == SCDB_UPDATED )
    {
        STREAM_COMP_ID_T    t_strm_comp_id;
        
        if ( pt_bypass_obj->ui2_force_pid != MPEG_2_NULL_PID )
        {
            /* If the stream is in force PID mode, ignore all SCDB update
               notification */
            return;
        }

        t_strm_comp_id = pt_bypass_obj->t_base.t_strm_comp_id;
        t_strm_comp_id.e_type = ST_VIDEO;
        i4_ret = x_scdb_get_rec( pt_bypass_obj->t_base.h_scdb,
                                 &t_strm_comp_id,
                                 &t_scdb_rec );
        pt_bypass_obj->t_base.t_strm_comp_id = t_strm_comp_id;
        pt_bypass_obj->t_base.t_strm_comp_id.e_type = ST_BYPASS;
        if ( i4_ret != SCDBR_OK || 
             ( pt_bypass_obj->t_base.t_scdb_rec.e_rec_type != t_scdb_rec.e_rec_type ) )
        {
            return;
        }
        
        if ( ui4_reason & SCDB_RECORD_DEL )
        {
            /* As the record is found, my record is NOT deleted. 
               Must be someone else, just ignore the notify. */
        }
        if ( ui4_reason & SCDB_RECORD_MOD &&
             x_memcmp( &t_scdb_rec, &pt_bypass_obj->t_base.t_scdb_rec, sizeof(SCDB_REC_T) ) != 0 )
        {
            if ( t_scdb_rec.e_rec_type == SCDB_REC_TYPE_VIDEO_MPEG )
            {
                /* todo, see what SM could do?? */
                if ( t_scdb_rec.u.t_video_mpeg.ui2_pid != 
                     pt_bypass_obj->t_base.t_scdb_rec.u.t_video_mpeg.ui2_pid )
                {
                    SM_BYPASS_DBG_INFO(( 
                        "[SM] Bypass stream(0x%.8X) PID changed to 0x%.4X!\r\n",
                        (UINT32)h_stream,
                        t_scdb_rec.u.t_video_mpeg.ui2_pid ));
                }
                if ( t_scdb_rec.u.t_video_mpeg.e_vid_fmt != 
                     pt_bypass_obj->t_base.t_scdb_rec.u.t_video_mpeg.e_vid_fmt )
                {
#ifdef DEBUG
                    if ( (UINT32)t_scdb_rec.u.t_video_mpeg.e_vid_fmt <
                         (sizeof(as_scdb_vid_fmt)/sizeof(CHAR*) - 1) )
                    {
                        SM_BYPASS_DBG_INFO(( 
                            "[SM] Bypass stream(0x%.8X) format changed to %s!\r\n",
                            (UINT32)h_stream,
                            as_scdb_vid_fmt[(UINT32)t_scdb_rec.u.t_video_mpeg.e_vid_fmt] ));
                    }
#endif
                }
                if ( t_scdb_rec.u.t_video_mpeg.b_default != 
                     pt_bypass_obj->t_base.t_scdb_rec.u.t_video_mpeg.b_default )
                {
                    SM_BYPASS_DBG_INFO(( 
                        "[SM] Bypass stream(0x%.8X) %s!\r\n",
                        (UINT32)h_stream,
                        ( (t_scdb_rec.u.t_video_mpeg.b_default)?
                          "is now DEFAULT": "no longer DEFAULT" ) ));           
                }
                if ( t_scdb_rec.u.t_video_mpeg.e_enc !=
                     pt_bypass_obj->t_base.t_scdb_rec.u.t_video_mpeg.e_enc )
                {
#if 0    /* Mark this, because upper client can do restart action */ 
                    sm_bypass_hdlr_send_msg_do_event( 
                        pt_bypass_obj->t_base.h_stream,
                        SM_EVN_BYPASS_RESTART,
                        (UINT32)pt_bypass_obj->t_base.i4_speed );
#else                  
                    if ( pt_bypass_obj->t_base.e_condition == SM_COND_STARTED ) 
                    {  
                        sm_bypass_state_notify_app(
                                &t_state_tag,
                                (SM_COND_T)pt_bypass_obj->t_state.ui4_curr_state,
                                SM_EVN_VS_REQ_RESTART,
                                (UINT32)&pt_bypass_obj->t_vid_res );
                    }
#endif
                    SM_BYPASS_DBG_INFO((
                        "[SM] Bypass stream(0x%.8X) encoding changed to (%u)!\r\n",
                        (UINT32)pt_bypass_obj->t_base.h_stream,
                        (UINT32)t_scdb_rec.u.t_video_mpeg.e_enc ));
                }
                /* for 3D video */
                pt_bypass_obj->t_rd3d_info = pt_bypass_obj->t_base.t_scdb_rec.u.t_video_mpeg.t_rd3d;
                pt_bypass_obj->t_mvc_ext = pt_bypass_obj->t_base.t_scdb_rec.u.t_video_mpeg.t_mvc_ext;
            }
            else if ( t_scdb_rec.e_rec_type == SCDB_REC_TYPE_VIDEO_ANALOG )
            {
                if ( t_scdb_rec.u.t_video_analog.e_vid_fmt != 
                     pt_bypass_obj->t_base.t_scdb_rec.u.t_video_analog.e_vid_fmt )
                {
#ifdef DEBUG
                    if ( (UINT32)t_scdb_rec.u.t_video_analog.e_vid_fmt <
                         (sizeof(as_scdb_vid_fmt)/sizeof(CHAR*) - 1) )
                    {
                        SM_BYPASS_DBG_INFO(( 
                            "[SM] Bypass stream(0x%.8X) format changed to %s!\r\n",
                            (UINT32)h_stream,
                            as_scdb_vid_fmt[(UINT32)t_scdb_rec.u.t_video_analog.e_vid_fmt] ));
                    }
#endif
                }
                if ( t_scdb_rec.u.t_video_analog.e_color_sys != 
                     pt_bypass_obj->t_base.t_scdb_rec.u.t_video_analog.e_color_sys )
                {
#ifdef DEBUG
                    if ( ((UINT32)t_scdb_rec.u.t_video_analog.e_color_sys + 1) <
                         (sizeof(as_scdb_vid_color_sys)/sizeof(CHAR*) - 1) )
                    {
                        SM_BYPASS_DBG_INFO(( 
                            "[SM] Bypass stream(0x%.8X) format changed to %s!\r\n",
                            (UINT32)h_stream,
                            as_scdb_vid_color_sys[((UINT32)t_scdb_rec.u.t_video_analog.e_color_sys+1)] ));
                    }
#endif
                }
                if ( t_scdb_rec.u.t_video_analog.b_default != 
                     pt_bypass_obj->t_base.t_scdb_rec.u.t_video_analog.b_default )
                {
                    SM_BYPASS_DBG_INFO(( 
                        "[SM] Bypass stream(0x%.8X) %s!\r\n",
                        (UINT32)h_stream,
                        ( (t_scdb_rec.u.t_video_mpeg.b_default)?
                          "is now DEFAULT": "no longer DEFAULT" ) ));           
                }
            }
        }
        if ( ui4_reason & SCDB_RECORD_ADD )
        {
            /* ignore */
        }
        if ( ui4_reason & SCDB_REASON_UNK )
        {
            /* ignore */
        }
                
        pt_bypass_obj->t_base.t_scdb_rec = t_scdb_rec;
        /* set bypass obj actual enc */
        sm_bypass_comp_vd_get_actual_enc(pt_bypass_obj);
    
    }
    else if ( e_scdb_cond == SCDB_UPDATING )
    {
        /* Ignore */
    }
    else if ( e_scdb_cond == SCDB_CLOSED )
    {
        /* Left it to be process by session layer */
        pt_bypass_obj->t_base.h_scdb = NULL_HANDLE;
    }
    else
    {
        /* improper SCDB state */
        SM_BYPASS_DBG_ERR(( "[SM] %s:%d Improper SCDB state!\r\n", __FILE__, __LINE__ ));
        SM_ASSERT( 0 );
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_hdlr_bs_free_fct
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
static BOOL _sm_bypass_hdlr_bs_free_fct( HANDLE_T       h_handle,
                                         HANDLE_TYPE_T  e_type,
                                         VOID*          pv_obj,
                                         VOID*          pv_tag,
                                         BOOL           b_req_handle )
{
    INT32               i4_ret;
    SM_BYPASS_STREAM_T* pt_bypass_obj = (SM_BYPASS_STREAM_T*)pv_obj;

    if ( b_req_handle || !pt_bypass_obj )
    {
        return FALSE;   
    }
    
    x_sema_delete( pt_bypass_obj->t_base.h_lock );
    pt_bypass_obj->t_base.h_lock = NULL_HANDLE;
    
    if ( pt_bypass_obj->t_base.h_scdb != NULL_HANDLE )
    {
        i4_ret = x_scdb_close( pt_bypass_obj->t_base.h_scdb );
        if ( i4_ret != SCDBR_OK )
        {
            SM_ABORT( SM_DBG_ABRT_CANNOT_CLOSE_SCDB );
        }
        pt_bypass_obj->t_base.h_scdb = NULL_HANDLE;
    }
    
    if ( pt_bypass_obj->t_state.pv_state_nfy_tag )
    {
        x_mem_free( pt_bypass_obj->t_state.pv_state_nfy_tag );
        pt_bypass_obj->t_state.pv_state_nfy_tag = NULL;
    }
    
    x_mem_free( pt_bypass_obj );
    
    return TRUE;
}


#ifdef CI_PLUS_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_hdlr_cci_plus_pm_2_drv
 *
 * Description: <function description>
 *
 * Inputs:  e_pm_cci       Pm defined CCI information.
 *
 * Outputs: - 
 *
 * Returns: TVE defined CCI information.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sm_bypass_hdlr_cci_plus_pm_2_drv( 
    CP_INFO_T*              pt_cp_rct_info,
    CP_INFO_T*              pt_cp_emi_info,
    CP_INFO_T*              pt_cp_aps_info,
    TV_ENC_CCI_DATA_INFO_T* pui4_tve_cci_data_info )
{
    TV_ENC_CCI_DATA_INFO_T  ui4_tve_cci_data = 0;
    
    if ( ( pt_cp_rct_info == NULL ) || (pt_cp_emi_info == NULL) || (pt_cp_aps_info == NULL))
    {
        return SMR_INV_ARG; 
    }
    
    if (pt_cp_rct_info->u.e_rct_ctrl == RCT_OFF)
    {
        if (pt_cp_emi_info->u.e_emi_ctrl == EMI_COPY_NOT_RESTRICTED )
        {
            ui4_tve_cci_data = 0;
        }
        else if (pt_cp_emi_info->u.e_emi_ctrl == EMI_NO_FURTHER_COPY )
        {
            ui4_tve_cci_data |= pt_cp_aps_info->u.e_aps_ctrl;
            ui4_tve_cci_data <<= 2;
            ui4_tve_cci_data += 1;
        }
        else if (pt_cp_emi_info->u.e_emi_ctrl == EMI_ONE_GENERATION_COPY )
        {
            ui4_tve_cci_data |= pt_cp_aps_info->u.e_aps_ctrl;
            ui4_tve_cci_data <<= 2;
            ui4_tve_cci_data += 2;
        }
        else /* (pt_cp_emi_info->u.e_emi_ctrl == EMI_COPY_PROHIBITED ) */
        {
            
            ui4_tve_cci_data |= pt_cp_aps_info->u.e_aps_ctrl;
            ui4_tve_cci_data <<= 2;
            ui4_tve_cci_data += 3;
        }
    }
    else /* pt_cp_rct_info->u.e_rct_ctrl == RCT_ON */ 
    {
        if (pt_cp_emi_info->u.e_emi_ctrl == EMI_COPY_NOT_RESTRICTED )
        {
            ui4_tve_cci_data = 0;
        }
        else
        {
            return SMR_INV_ARG; 
        }
    }

    *pui4_tve_cci_data_info = ui4_tve_cci_data;
    return SMR_OK;
}

#else

/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_hdlr_cci_pm_2_drv
 *
 * Description: <function description>
 *
 * Inputs:  e_pm_cci       Pm defined CCI information.
 *
 * Outputs: - 
 *
 * Returns: TVE defined CCI information.
 *
 ----------------------------------------------------------------------------*/
static TV_ENC_CCI_TYPE_T _sm_bypass_hdlr_cci_pm_2_drv( EMI_TYPE_T   e_pm_cci )
{
    switch( e_pm_cci )
    {
    case EMI_COPY_NOT_RESTRICTED:
        return TV_ENC_CCI_TYPE_NO_RESTRICT;
        
    case EMI_NO_FURTHER_COPY:
        return TV_ENC_CCI_TYPE_NO_FURTHER_COPY;
        
    case EMI_ONE_GENERATION_COPY:
        return TV_ENC_CCI_TYPE_COPY_ONCE;
        
    case EMI_COPY_PROHIBITED:
        return TV_ENC_CCI_TYPE_COPY_NEVER;
        
    default:
        return TV_ENC_CCI_TYPE_NO_RESTRICT;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_hdlr_aps_pm_2_drv
 *
 * Description: <function description>
 *
 * Inputs:  e_pm_aps       PM defined APS information.
 *
 * Outputs: - 
 *
 * Returns: TVE defined APS information.
 *
 ----------------------------------------------------------------------------*/
static TV_ENC_MV_APS_TYPE_T _sm_bypass_hdlr_aps_pm_2_drv( APS_TYPE_T   e_pm_aps )
{
    switch( e_pm_aps )
    {
    case APS_CP_OFF:
        return TV_ENC_MV_APS_TYPE_OFF;
        
    case APS_AGC_ON_SPLIT_BURST_OFF:
        return TV_ENC_MV_APS_TYPE_AGC_ONLY;
        
    case APS_AGC_ON_2_LINE_SPLIT_BURST_ON:
        return TV_ENC_MV_APS_TYPE_AGC_WI_2L_SPLIT_BURST;
        
    case APS_AGC_ON_4_LINE_SPLIT_BURST_OFF:
        return TV_ENC_MV_APS_TYPE_AGC_WI_4L_SPLIT_BURST;
        
    default:
        return TV_ENC_MV_APS_TYPE_OFF;
    }
}
#endif

#ifdef CI_PLUS_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_hdlr_set_cci_plus_info
 *
 * Description: <function description>
 *
 * Inputs:  pt_bypass_obj       References.
 *          pt_cp_info      References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *          SMR_NOT_ALLOW       Fail.
 *          SMR_NOT_SUPPORT     Fail.
 *          SMR_INV_ARG         Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sm_bypass_hdlr_set_cci_plus_info(
    SM_BYPASS_STREAM_T*    pt_bypass_obj,
    CP_INFO_LIST_T*        pt_cp_info_lst )
{
    INT32                   i4_ret;
    UINT8                   ui1_i;
    CP_INFO_T*              pt_cp_rct_info = NULL;
    CP_INFO_T*              pt_cp_emi_info = NULL;
    CP_INFO_T*              pt_cp_aps_info = NULL;
    TV_ENC_CCI_DATA_INFO_T  ui4_tve_cci_data_info = 0;

    UINT32             ui4_drv_set_type;
    VOID*              pv_drv_set_info;
    SIZE_T             z_drv_set_info_size = 0;
    RM_COND_T          e_drv_cond;

    if ( (pt_bypass_obj == NULL) || (pt_cp_info_lst == NULL) )
    {
        return SMR_INV_ARG;
    }

    for( ui1_i = 0; ui1_i < pt_cp_info_lst->ui1_ctrl_size; ui1_i++ )
    {
        if ( ui1_i < CP_TYPE_MAX )
        {
            switch( pt_cp_info_lst->at_cp_ctrl_list[ui1_i].t_cp_type )
            {
            case CP_TYPE_RCT:
                pt_cp_rct_info = &pt_cp_info_lst->at_cp_ctrl_list[ui1_i];
                break;
            case CP_TYPE_EMI:
                pt_cp_emi_info = &pt_cp_info_lst->at_cp_ctrl_list[ui1_i];
                break;
            case CP_TYPE_APS:
                pt_cp_aps_info = &pt_cp_info_lst->at_cp_ctrl_list[ui1_i];
                break;
            case CP_TYPE_CIT:
            case CP_TYPE_RL:
            default:
                break;
            }
        }
        else
        {
            return SMR_INV_ARG;
        }
    }

    if ( (pt_cp_rct_info == NULL) || (pt_cp_emi_info == NULL) || (pt_cp_aps_info == NULL))
    {
        return SMR_INV_ARG;
    }

    i4_ret = _sm_bypass_hdlr_cci_plus_pm_2_drv( pt_cp_rct_info, 
                                                pt_cp_emi_info, 
                                                pt_cp_aps_info, 
                                                &ui4_tve_cci_data_info );
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_ARG;
    }

    ui4_drv_set_type = TV_ENC_SET_TYPE_CCI;
    pv_drv_set_info = &ui4_tve_cci_data_info;
    z_drv_set_info_size = sizeof( TV_ENC_CCI_DATA_INFO_T );

    i4_ret = rm_set( pt_bypass_obj->t_bypass_comp_grp.t_tve.h_comp,
                     FALSE,
                     ANY_PORT_NUM,
                     ui4_drv_set_type,
                     pv_drv_set_info,
                     z_drv_set_info_size,
                     &e_drv_cond );
    if ( i4_ret != RMR_OK )
    {
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

#else

/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_hdlr_set_cci_info
 *
 * Description: <function description>
 *
 * Inputs:  pt_bypass_obj    References.
 *          pt_cp_info       References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *          SMR_NOT_ALLOW       Fail.
 *          SMR_NOT_SUPPORT     Fail.
 *          SMR_INV_ARG         Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sm_bypass_hdlr_set_cci_info( 
    SM_BYPASS_STREAM_T*    pt_bypass_obj,
    CP_INFO_T*             pt_cp_info )
{
    INT32              i4_ret;
    TV_ENC_MV_INFO_T   t_tve_mv_info;
    TV_ENC_CCI_TYPE_T  e_tve_cci_type;
    
    UINT32             ui4_drv_set_type;
    VOID*              pv_drv_set_info;
    SIZE_T             z_drv_set_info_size = 0;
    RM_COND_T          e_drv_cond;
    
    if ( (pt_bypass_obj == NULL) || (pt_cp_info == NULL) )
    {
        return SMR_INV_ARG;
    }
    
    switch( pt_cp_info->t_cp_type )
    {
    case CP_TYPE_EMI:
        e_tve_cci_type = _sm_bypass_hdlr_cci_pm_2_drv( pt_cp_info->u.e_emi_ctrl );
        
        ui4_drv_set_type = TV_ENC_SET_TYPE_CCI;
        pv_drv_set_info = (VOID*)(UINT32)e_tve_cci_type;
        z_drv_set_info_size = sizeof( TV_ENC_CCI_TYPE_T );
        break;
        
    case CP_TYPE_APS:
        t_tve_mv_info.e_mv_type = TV_ENC_MV_TYPE_APS;
        t_tve_mv_info.u.e_aps_type = _sm_bypass_hdlr_aps_pm_2_drv( pt_cp_info->u.e_aps_ctrl );
        
        ui4_drv_set_type = TV_ENC_SET_TYPE_MV;
        pv_drv_set_info = &t_tve_mv_info;
        z_drv_set_info_size = sizeof( TV_ENC_MV_INFO_T );
        break;
        
    case CP_TYPE_CIT:
    default:
        /* Ignore unknown CP types */
        return SMR_OK;
    }

    i4_ret = rm_set( pt_bypass_obj->t_bypass_comp_grp.t_tve.h_comp,
                     FALSE,
                     ANY_PORT_NUM,
                     ui4_drv_set_type,
                     pv_drv_set_info,
                     z_drv_set_info_size,
                     &e_drv_cond );
    if ( i4_ret != RMR_OK )
    {
        return SMR_DRV_COMP_ERROR;    
    }
    
    return SMR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_hdlr_set
 *
 * Description: <function description>
 *
 * Inputs:  h_stream       References.
 *          e_set_type     References.
 *          pv_set_info    References.
 *          z_set_info_len References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK              Success.
 *          SMR_NOT_SUPPORT     Fail.
 *          SMR_INV_ARG         Fail.
 *          SMR_INTERNAL_ERROR  Fail.
 *          SMR_INV_HANDLE      Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sm_bypass_hdlr_set( HANDLE_T         h_stream,
                                  SM_SET_TYPE_T    e_set_type,
                                  VOID*            pv_set_info,
                                  SIZE_T           z_set_info_len )
{
    INT32                  i4_ret = SMR_OK;
    SM_BYPASS_STREAM_T*    pt_bypass_obj = NULL;
    HANDLE_T               h_drv = NULL_HANDLE;
    RM_COND_T*             pe_drv_cond = NULL;
    DRV_SET_TYPE_T         e_drv_set_type = 0;
    VOID*                  pv_drv_set_info = NULL;
    SIZE_T                 z_drv_set_info_len = 0;

    pt_bypass_obj = (SM_BYPASS_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_bypass_obj )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_set_type )
    {
    case SM_BSH_SET_TYPE_VBIF_SRC:
    {
#ifndef DISABLE_VBI_FLTR_SUPPORT
        BSH_VBIF_SRC_INFO_T* pt_vbif_src = (BSH_VBIF_SRC_INFO_T*)pv_set_info;
#endif    

        if ( !IS_MPEG_BYPASS_STRM(pt_bypass_obj) )
        {
            return SMR_NOT_ALLOW;
        }

        if ( pt_bypass_obj->h_vbif != NULL_HANDLE &&
             pt_bypass_obj->e_vbif_cond != VBIF_COND_IGNORE )
        {
            sm_bypass_comp_vbif_close( pt_bypass_obj, TRUE );
        }
        
#ifndef DISABLE_VBI_FLTR_SUPPORT  /* for CR DTV00364349 , temp solution */       
        if ( pt_bypass_obj->e_vbif_cond == VBIF_COND_CLOSED ||
             pt_bypass_obj->e_vbif_cond == VBIF_COND_IGNORE)
        {
            pt_bypass_obj->t_vbif_src = *pt_vbif_src;
            i4_ret = sm_bypass_comp_vbif_open( h_stream, pt_bypass_obj );
            return i4_ret;
        }
        else
        {
            return SMR_INTERNAL_ERROR;
        }
#else
        return SMR_NOT_ALLOW;
#endif
    }
    
    case SM_BSH_SET_TYPE_VBIF_CTRL:
        {
            BSH_VBIF_CTRL_T e_vbif_ctrl = (BSH_VBIF_CTRL_T)(UINT32)pv_set_info;

            if ( !IS_MPEG_BYPASS_STRM(pt_bypass_obj) )
            {
                return SMR_NOT_ALLOW;
            }

            switch ( e_vbif_ctrl )
            {
            case BSH_VBIF_CTRL_STOP:
                i4_ret = sm_bypass_comp_vbif_disable( pt_bypass_obj );
                break;
            case BSH_VBIF_CTRL_START:
                i4_ret = sm_bypass_comp_vbif_enable( pt_bypass_obj );
                break;
            default:
                i4_ret = SMR_INV_SET_INFO;
            }

            return i4_ret;
        }

    case SM_BSH_SET_TYPE_VIDEO_OUT_CTRL:
        {
            BSH_VIDEO_OUT_CTRL_T e_video_out_ctrl = (BSH_VIDEO_OUT_CTRL_T)(UINT32)pv_set_info;
            
            if ( pt_bypass_obj->t_base.e_condition != SM_COND_STARTED )
            {
                return SMR_CAN_NOT_OP_IN_THIS_STATE;
            }

            h_drv = pt_bypass_obj->t_bypass_comp_grp.t_vout.h_comp;
            e_drv_set_type = VIDEO_OUT_SET_TYPE_CTRL;
            switch ( e_video_out_ctrl )
            {
            case BSH_VIDEO_OUT_CTRL_DISABLE:
                pv_drv_set_info = (VOID*)(UINT32)VIDEO_OUT_CTRL_DISABLE;
                break;
            case BSH_VIDEO_OUT_CTRL_ENABLE:
                pv_drv_set_info = (VOID*)(UINT32)VIDEO_OUT_CTRL_ENABLE;
                break;
            default:
                return SMR_INV_SET_INFO;
            }
            z_drv_set_info_len = sizeof(VIDEO_OUT_CTRL_T);
        }
        break;

    case SM_BSH_SET_TYPE_TV_ENC_CTRL:
        {
            BSH_TV_ENC_CTRL_T e_tv_enc_ctrl = (BSH_TV_ENC_CTRL_T)(UINT32)pv_set_info;
            
            if ( pt_bypass_obj->t_base.e_condition != SM_COND_STARTED )
            {
                return SMR_CAN_NOT_OP_IN_THIS_STATE;
            }

            h_drv = pt_bypass_obj->t_bypass_comp_grp.t_tve.h_comp;
            e_drv_set_type = TV_ENC_SET_TYPE_CTRL;
            switch ( e_tv_enc_ctrl )
            {
            case BSH_TV_ENC_CTRL_DISABLE:
                pv_drv_set_info = (VOID*)(UINT32)TV_ENC_CTRL_DISABLE;
                break;
            case BSH_TV_ENC_CTRL_ENABLE:
                pv_drv_set_info = (VOID*)(UINT32)TV_ENC_CTRL_ENABLE;
                break;
            default:
                return SMR_INV_SET_INFO;
            }
            z_drv_set_info_len = sizeof(TV_ENC_CTRL_T);

        }
        break;

    case SM_BSH_SET_TYPE_IGNORE:
    default:
        return SMR_NOT_SUPPORT;
    }

    i4_ret = rm_set( h_drv,
                     FALSE,
                     ANY_PORT_NUM,
                     e_drv_set_type,
                     (VOID*)(pv_drv_set_info),
                     z_drv_set_info_len,
                     pe_drv_cond );
    if ( ( i4_ret != RMR_OK ) && ( i4_ret != RMR_ASYNC_NFY ) )
    {
        return SMR_NOT_ALLOW;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_hdlr_slctr_get
 *
 * Description: <function description>
 *
 * Inputs:  h_stream        References.
 *          e_get_type      References.
 *          pv_get_info     References.
 *          pz_get_info_len References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INV_HANDLE         Fail.
 *          SMR_INSUFFICIENT_SIZE  Fail.
 *          SMR_NOT_SUPPORT        Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 _sm_bypass_hdlr_slctr_get( HANDLE_T         h_stream,
                                 SM_GET_TYPE_T    e_get_type,
                                 VOID*            pv_get_info,
                                 SIZE_T*          pz_get_info_len )
{
    SM_BYPASS_STREAM_T*    pt_bypass_obj = NULL;

    if ( SM_GET_GRP(e_get_type)==SM_GRP_SESS )
    {
        return SMR_NOT_SUPPORT;
    }

    pt_bypass_obj = (SM_BYPASS_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_bypass_obj )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_get_type )
    {
    case SM_STRM_GET_TYPE_STATE:
        if ( *pz_get_info_len < sizeof(pt_bypass_obj->t_base.e_condition) )
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        *((SM_COND_T*)pv_get_info) = pt_bypass_obj->t_base.e_condition;
        *pz_get_info_len = sizeof(pt_bypass_obj->t_base.e_condition);
        break;

    case SM_STRM_GET_TYPE_ID:
        if ( *pz_get_info_len < sizeof( STREAM_COMP_ID_T ) )
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        *((STREAM_COMP_ID_T*)pv_get_info) = pt_bypass_obj->t_base.t_strm_comp_id;
        *pz_get_info_len = sizeof(STREAM_COMP_ID_T);
        break;

    default:
        return SMR_NOT_SUPPORT;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_hdlr_vd_res_2_vs
 *
 * Description: <function description>
 *
 * Inputs:  pt_drv_res   Driver defined resolution.
 *
 * Outputs: pt_vsh_res   SM defined resolution.
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_bypass_hdlr_vd_res_2_vs( 
    const VID_DEC_RESOLUTION_INFO_T*    pt_drv_res,
    VSH_SRC_RESOLUTION_INFO_T*          pt_vsh_res )
{
    pt_vsh_res->ui4_frame_rate = (UINT32)pt_drv_res->ui2_frame_rate;
    pt_vsh_res->ui4_width = (UINT32)pt_drv_res->ui2_width;
    pt_vsh_res->ui4_height = (UINT32)pt_drv_res->ui2_height;
    pt_vsh_res->ui4_aspect_ratio_w = (UINT32)pt_drv_res->u4_aspect_ratio_w;
    pt_vsh_res->ui4_aspect_ratio_h = (UINT32)pt_drv_res->u4_aspect_ratio_h;
    pt_vsh_res->b_is_progressive = pt_drv_res->b_is_progressive;
    pt_vsh_res->b_is_display_aspect_ratio = pt_drv_res->b_is_display_aspect_ratio;
    pt_vsh_res->ui4_ext = 0;
    
    if ( pt_drv_res->e_src_asp_ratio == VID_DEC_SRC_ASPECT_RATIO_4_3 )
    {
        pt_vsh_res->e_src_asp_ratio = VSH_SRC_ASPECT_RATIO_4_3;
    }
    else if ( pt_drv_res->e_src_asp_ratio == VID_DEC_SRC_ASPECT_RATIO_16_9 )
    {
        pt_vsh_res->e_src_asp_ratio = VSH_SRC_ASPECT_RATIO_16_9;
    }
    else if ( pt_drv_res->e_src_asp_ratio == VID_DEC_SRC_ASPECT_RATIO_2_21_1 )
    {
        pt_vsh_res->e_src_asp_ratio = VSH_SRC_ASPECT_RATIO_2_21_1;
    }
    else if ( pt_drv_res->e_src_asp_ratio == VID_DEC_SRC_ASPECT_RATIO_PXL_10_11 )
    {
        pt_vsh_res->e_src_asp_ratio = VSH_SRC_ASPECT_RATIO_PXL_10_11;
    }
    else if ( pt_drv_res->e_src_asp_ratio == VID_DEC_SRC_ASPECT_RATIO_PXL_40_33 )
    {
        pt_vsh_res->e_src_asp_ratio = VSH_SRC_ASPECT_RATIO_PXL_40_33;
    }
    else if ( pt_drv_res->e_src_asp_ratio == VID_DEC_SRC_ASPECT_RATIO_PXL_16_11 )
    {
        pt_vsh_res->e_src_asp_ratio = VSH_SRC_ASPECT_RATIO_PXL_16_11;
    }
    else if ( pt_drv_res->e_src_asp_ratio == VID_DEC_SRC_ASPECT_RATIO_PXL_12_11 )
    {
        pt_vsh_res->e_src_asp_ratio = VSH_SRC_ASPECT_RATIO_PXL_12_11;
    }
    else if ( pt_drv_res->e_src_asp_ratio == VID_DEC_SRC_ASPECT_RATIO_PXL_3_2 )
    {
        pt_vsh_res->e_src_asp_ratio = VSH_SRC_ASPECT_RATIO_PXL_3_2;
    }
    else if ( pt_drv_res->e_src_asp_ratio == VID_DEC_SRC_ASPECT_RATIO_PXL_1_1 )
    {
        pt_vsh_res->e_src_asp_ratio = VSH_SRC_ASPECT_RATIO_PXL_1_1;
    }
    else if ( pt_drv_res->e_src_asp_ratio == VID_DEC_SRC_ASPECT_RATIO_USR )
    {
        pt_vsh_res->e_src_asp_ratio = VSH_SRC_ASPECT_RATIO_USR;
    }
    else
    {
        pt_vsh_res->e_src_asp_ratio = VSH_SRC_ASPECT_RATIO_UNKNOWN;
    }
    
    pt_vsh_res->e_timing_type = VSH_SRC_TIMING_VIDEO;
    pt_vsh_res->e_color_fmt = VSH_SRC_COLOR_FMT_UNKNOWN;
    pt_vsh_res->e_src_color_space = VSH_SRC_COLOR_FMT_UNKNOWN;
#if 0 
    pt_vsh_res->e_scart_pin_8 = VSH_SCART_PIN_8_TYPE_UNKNOWN;
#endif
    pt_vsh_res->ui4_res_idx = 0;
    VSH_SRC_RES_EXT_SET_AFD( pt_vsh_res->ui4_ext, pt_drv_res->ui2_afd );
    x_memset( &pt_vsh_res->t_hdmi_infoframe, 0, sizeof(VSH_HDMI_AVI_INFOFRAME_INFO_T) );

    switch ( pt_drv_res->e_tag3d_type )
    {
    case VID_DEC_TAG3D_2D:
        pt_vsh_res->e_src_tag3d_type = VSH_SRC_TAG3D_2D;
        break;
    case VID_DEC_TAG3D_MVC:
        pt_vsh_res->e_src_tag3d_type = VSH_SRC_TAG3D_MVC;
        break;
    case VID_DEC_TAG3D_FP:
        pt_vsh_res->e_src_tag3d_type = VSH_SRC_TAG3D_FP;
        break;
    case VID_DEC_TAG3D_FS:
        pt_vsh_res->e_src_tag3d_type = VSH_SRC_TAG3D_FS;
        break;
    case VID_DEC_TAG3D_TB:
        pt_vsh_res->e_src_tag3d_type = VSH_SRC_TAG3D_TB;
        break;
    case VID_DEC_TAG3D_SBS:
        pt_vsh_res->e_src_tag3d_type = VSH_SRC_TAG3D_SBS;
        break;
    case VID_DEC_TAG3D_REALD:
        pt_vsh_res->e_src_tag3d_type = VSH_SRC_TAG3D_REALD;
        break;
    case VID_DEC_TAG3D_SENSIO:
        pt_vsh_res->e_src_tag3d_type = VSH_SRC_TAG3D_SENSIO;
        break;
    case VID_DEC_TAG3D_LA:
        pt_vsh_res->e_src_tag3d_type = VSH_SRC_TAG3D_LA;
        break;
    case VID_DEC_TAG3D_TTDO:
        pt_vsh_res->e_src_tag3d_type = VSH_SRC_TAG3D_TTDO;
        break;
    case VID_DEC_TAG3D_NOT_SUPPORT:
    default:
        pt_vsh_res->e_src_tag3d_type = VSH_SRC_TAG3D_NOT_SUPPORT;
        break;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_hdlr_tvd_res_2_vs
 *
 * Description: <function description>
 *
 * Inputs:  pt_drv_res   Driver defined resolution.
 *
 * Outputs: pt_vsh_res   SM defined resolution.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_bypass_hdlr_tvd_res_2_vs( 
    const SM_BYPASS_STREAM_T*          pt_bypass_obj,
    const TV_DEC_RESOLUTION_INFO_T*    pt_drv_res,
    VSH_SRC_RESOLUTION_INFO_T*         pt_vsh_res )
{
    pt_vsh_res->ui4_frame_rate = (UINT32)pt_drv_res->ui2_frame_rate;
    pt_vsh_res->ui4_width = (UINT32)pt_drv_res->ui2_width;
    pt_vsh_res->ui4_height = (UINT32)pt_drv_res->ui2_height;
    pt_vsh_res->b_is_progressive = pt_drv_res->b_is_progressive;
    pt_vsh_res->ui4_ext = 0;
    
    switch( TV_DEC_RES_EXT_GET_HDMI_SCAN_INFO(pt_drv_res->ui4_ext) )
    {
    case TVD_DEC_EXT_HDMI_SCAN_INFO_NONE:
        pt_vsh_res->ui4_ext |= VSH_SRC_RES_EXT_HDMI_SCAN_INFO_NONE;
        break;
        
    case TVD_DEC_EXT_HDMI_SCAN_INFO_OVERSCAN:
        pt_vsh_res->ui4_ext |= VSH_SRC_RES_EXT_HDMI_SCAN_INFO_OVERSCAN;
        break;

    case TVD_DEC_EXT_HDMI_SCAN_INFO_UNDERSCAN:
        pt_vsh_res->ui4_ext |= VSH_SRC_RES_EXT_HDMI_SCAN_INFO_UNDERSCAN;
        break;

    case TVD_DEC_EXT_HDMI_SCAN_INFO_FUTURE:
        pt_vsh_res->ui4_ext |= VSH_SRC_RES_EXT_HDMI_SCAN_INFO_FUTURE;
        break;

    default:
        break;            
    }
    
    switch( TV_DEC_RES_EXT_GET_HDMI_PIC_ASP_RATIO(pt_drv_res->ui4_ext) )
    {
    case TVD_DEC_EXT_HDMI_PIC_ASP_RATIO_NO_DATA:
        VSH_SRC_RES_EXT_SET_HDMI_PIC_ASP_RATIO( pt_vsh_res->ui4_ext, VSH_SRC_RES_EXT_HDMI_PIC_ASP_RATIO_NO_DATA );
        break;

    case TVD_DEC_EXT_HDMI_PIC_ASP_RATIO_4_3:
        VSH_SRC_RES_EXT_SET_HDMI_PIC_ASP_RATIO( pt_vsh_res->ui4_ext, VSH_SRC_RES_EXT_HDMI_PIC_ASP_RATIO_4_3 );
        break;

    case TVD_DEC_EXT_HDMI_PIC_ASP_RATIO_16_9:
        VSH_SRC_RES_EXT_SET_HDMI_PIC_ASP_RATIO( pt_vsh_res->ui4_ext, VSH_SRC_RES_EXT_HDMI_PIC_ASP_RATIO_16_9 );
        break;

    case TVD_DEC_EXT_HDMI_PIC_ASP_RATIO_FUTURE:
        VSH_SRC_RES_EXT_SET_HDMI_PIC_ASP_RATIO( pt_vsh_res->ui4_ext, VSH_SRC_RES_EXT_HDMI_PIC_ASP_RATIO_FUTURE );
        break;

    default:
        break;            
    }

    switch( TV_DEC_RES_EXT_GET_HDMI_AFAR(pt_drv_res->ui4_ext) )
    {
    case TVD_DEC_EXT_HDMI_AFAR_SAME_AS_PIC_ASP_RATIO:
        VSH_SRC_RES_EXT_SET_HDMI_AFAR( pt_vsh_res->ui4_ext, VSH_SRC_RES_EXT_HDMI_AFAR_SAME_AS_PIC_ASP_RATIO );
        break;

    case TVD_DEC_EXT_HDMI_AFAR_4_3_CENTER:
        VSH_SRC_RES_EXT_SET_HDMI_AFAR( pt_vsh_res->ui4_ext, VSH_SRC_RES_EXT_HDMI_AFAR_4_3_CENTER );
        break;

    case TVD_DEC_EXT_HDMI_AFAR_16_9_CENTER:
        VSH_SRC_RES_EXT_SET_HDMI_AFAR( pt_vsh_res->ui4_ext, VSH_SRC_RES_EXT_HDMI_AFAR_16_9_CENTER );
        break;

    case TVD_DEC_EXT_HDMI_AFAR_14_9_CENTER:
        VSH_SRC_RES_EXT_SET_HDMI_AFAR( pt_vsh_res->ui4_ext, VSH_SRC_RES_EXT_HDMI_AFAR_14_9_CENTER );
        break;

    default:
        VSH_SRC_RES_EXT_SET_HDMI_AFAR( pt_vsh_res->ui4_ext, TV_DEC_RES_EXT_GET_HDMI_AFAR(pt_drv_res->ui4_ext) );
        break;            
    }

    /* there is no color system info for component source, so judge resolution */
    /* if ( pt_bypass_obj->e_color_sys == COLOR_SYS_NTSC ) */
    if ( pt_drv_res->ui2_height == 480 )
    {
        VSH_SRC_RES_EXT_SET_NTSC_WSS_ASP_RATIO( pt_vsh_res->ui4_ext, TV_DEC_RES_EXT_GET_NTSC_WSS_ASP_RATIO(pt_drv_res->ui4_ext) );
    }
    else /* if ( pt_bypass_obj->e_color_sys == COLOR_SYS_PAL ||
              pt_bypass_obj->e_color_sys == COLOR_SYS_SECAM ) */
    {
        VSH_SRC_RES_EXT_SET_PAL_SECAM_WSS_ASP_RATIO( pt_vsh_res->ui4_ext, TV_DEC_RES_EXT_GET_PAL_SECAM_WSS_ASP_RATIO(pt_drv_res->ui4_ext) );
    }
    
    if ( pt_drv_res->e_src_asp_ratio == TV_DEC_SRC_ASPECT_RATIO_4_3 )
    {
        pt_vsh_res->e_src_asp_ratio = VSH_SRC_ASPECT_RATIO_4_3;
    }
    else if ( pt_drv_res->e_src_asp_ratio == TV_DEC_SRC_ASPECT_RATIO_16_9 )
    {
        pt_vsh_res->e_src_asp_ratio = VSH_SRC_ASPECT_RATIO_16_9;
    }
    else if ( pt_drv_res->e_src_asp_ratio == TV_DEC_SRC_ASPECT_RATIO_2_21_1 )
    {
        pt_vsh_res->e_src_asp_ratio = VSH_SRC_ASPECT_RATIO_2_21_1;
    }
    else
    {
        pt_vsh_res->e_src_asp_ratio = VSH_SRC_ASPECT_RATIO_UNKNOWN;
    }

    if ( pt_drv_res->e_timing_type == TV_DEC_TIMING_VIDEO )
    {
        pt_vsh_res->e_timing_type = VSH_SRC_TIMING_VIDEO;
    }
    else if ( pt_drv_res->e_timing_type == TV_DEC_TIMING_GRAPHIC )
    {
        pt_vsh_res->e_timing_type = VSH_SRC_TIMING_GRAPHIC;
    }
    else if ( pt_drv_res->e_timing_type == TV_DEC_TIMING_NOT_SUPPORT )
    {
        pt_vsh_res->e_timing_type = VSH_SRC_TIMING_NOT_SUPPORT;
    }
    else
    {
        pt_vsh_res->e_timing_type = VSH_SRC_TIMING_UNKNOWN;
    }

    if ( pt_drv_res->e_color_fmt == TV_DEC_COLOR_FMT_VGA_4_4_4 )
    {
        pt_vsh_res->e_color_fmt = VSH_SRC_COLOR_FMT_VGA_4_4_4;
    }
    else if ( pt_drv_res->e_color_fmt == TV_DEC_COLOR_FMT_YUV_4_2_2 )
    {
        pt_vsh_res->e_color_fmt = VSH_SRC_COLOR_FMT_YUV_4_2_2;
    }
    else
    {
        pt_vsh_res->e_color_fmt = VSH_SRC_COLOR_FMT_UNKNOWN;
    }

    if ( pt_drv_res->e_src_color_space == TV_DEC_COLOR_FMT_VGA_4_4_4 )
    {
        pt_vsh_res->e_src_color_space = VSH_SRC_COLOR_FMT_VGA_4_4_4;
    }
    else if ( pt_drv_res->e_src_color_space == TV_DEC_COLOR_FMT_YUV_4_2_2 )
    {
        pt_vsh_res->e_src_color_space = VSH_SRC_COLOR_FMT_YUV_4_2_2;
    }
    else if ( pt_drv_res->e_src_color_space == TV_DEC_COLOR_FMT_YUV_4_4_4 )
    {
        pt_vsh_res->e_src_color_space = VSH_SRC_COLOR_FMT_YUV_4_4_4;
    }
    else
    {
        pt_vsh_res->e_src_color_space = VSH_SRC_COLOR_FMT_UNKNOWN;
    }

#if 0
    if ( pt_drv_res->e_scart_pin_8 == TV_DEC_SCART_PIN_8_NO_SIGNAL )
    {
        pt_vsh_res->e_scart_pin_8 = VSH_SCART_PIN_8_TYPE_NO_SIGNAL;
    }
    else if ( pt_drv_res->e_scart_pin_8 == TV_DEC_SCART_PIN_8_ASPECT_16_9 )
    {
        pt_vsh_res->e_scart_pin_8 = VSH_SCART_PIN_8_TYPE_ASPECT_16_9;
    }
    else if ( pt_drv_res->e_scart_pin_8 == TV_DEC_SCART_PIN_8_ASPECT_4_3 )
    {
        pt_vsh_res->e_scart_pin_8 = VSH_SCART_PIN_8_TYPE_ASPECT_4_3;
    }
    else
    {
        pt_vsh_res->e_scart_pin_8 = VSH_SCART_PIN_8_TYPE_UNKNOWN;
    }
#endif
    
    pt_vsh_res->ui4_res_idx = pt_drv_res->ui4_res_idx;
    
    pt_vsh_res->t_hdmi_infoframe.b_valid = pt_drv_res->t_hdmi_infoframe.b_valid;
    x_memcpy(pt_vsh_res->t_hdmi_infoframe.aui1_info_frame_data,
             pt_drv_res->t_hdmi_infoframe.aui1_info_frame_data,
             sizeof(UINT8) * VSH_HDMI_INFO_FRAME_LEN);

    switch ( pt_drv_res->e_tag3d_type )
    {
    case TV_DEC_TAG3D_2D:
        pt_vsh_res->e_src_tag3d_type = VSH_SRC_TAG3D_2D;
        break;
    case TV_DEC_TAG3D_MVC:
        pt_vsh_res->e_src_tag3d_type = VSH_SRC_TAG3D_MVC;
        break;
    case TV_DEC_TAG3D_FP:
        pt_vsh_res->e_src_tag3d_type = VSH_SRC_TAG3D_FP;
        break;
    case TV_DEC_TAG3D_FS:
        pt_vsh_res->e_src_tag3d_type = VSH_SRC_TAG3D_FS;
        break;
    case TV_DEC_TAG3D_TB:
        pt_vsh_res->e_src_tag3d_type = VSH_SRC_TAG3D_TB;
        break;
    case TV_DEC_TAG3D_SBS:
        pt_vsh_res->e_src_tag3d_type = VSH_SRC_TAG3D_SBS;
        break;
    case TV_DEC_TAG3D_REALD:
        pt_vsh_res->e_src_tag3d_type = VSH_SRC_TAG3D_REALD;
        break;
    case TV_DEC_TAG3D_SENSIO:
        pt_vsh_res->e_src_tag3d_type = VSH_SRC_TAG3D_SENSIO;
        break;
    case TV_DEC_TAG3D_LA:
        pt_vsh_res->e_src_tag3d_type = VSH_SRC_TAG3D_LA;
        break;
    case TV_DEC_TAG3D_TTDO:
        pt_vsh_res->e_src_tag3d_type = VSH_SRC_TAG3D_TTDO;
        break;
    case TV_DEC_TAG3D_NOT_SUPPORT:
    default:
        pt_vsh_res->e_src_tag3d_type = VSH_SRC_TAG3D_NOT_SUPPORT;
        break;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_hdlr_get
 *
 * Description: <function description>
 *
 * Inputs:  h_stream        References.
 *          e_get_type      References.
 *          pv_get_info     References.
 *          pz_get_info_len References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK           Success.
 *          SMR_INV_HANDLE   Fail.
 *          SMR_NOT_SUPPORT  Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sm_bypass_hdlr_get( HANDLE_T         h_stream,
                                  SM_GET_TYPE_T    e_get_type,
                                  VOID*            pv_get_info,
                                  SIZE_T*          pz_get_info_len )
{
    SM_BYPASS_STREAM_T*    pt_bypass_obj = NULL;
    INT32                  i4_ret;

    pt_bypass_obj = (SM_BYPASS_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_bypass_obj )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_get_type )
    {
    case SM_BSH_GET_TYPE_VIDEO_SRC_RESOLUTION:
        {
            SIZE_T                     z_drv_res_len = sizeof(VID_DEC_RESOLUTION_INFO_T);
            VSH_SRC_RESOLUTION_INFO_T* pt_vsh_res = (VSH_SRC_RESOLUTION_INFO_T*)pv_get_info;
            
            if ( !pt_vsh_res )
            {
                return SMR_INV_GET_INFO;
            }
            if ( !pz_get_info_len || 
                 *pz_get_info_len < sizeof(VSH_SRC_RESOLUTION_INFO_T) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            
            if ( IS_MPEG_BYPASS_STRM(pt_bypass_obj) )
            {
                VID_DEC_RESOLUTION_INFO_T    t_drv_res;
                
                z_drv_res_len = sizeof(VID_DEC_RESOLUTION_INFO_T);
                i4_ret = rm_get( pt_bypass_obj->t_bypass_comp_grp.t_dec.h_comp,
                                 FALSE,
                                 pt_bypass_obj->t_bypass_comp_grp.t_dec.t_info.ui1_in_port,
                                 VID_DEC_GET_TYPE_RESOLUTION,
                                 (VOID*)&t_drv_res,
                                 &z_drv_res_len,
                                 &pt_bypass_obj->t_bypass_comp_grp.t_dec.e_comp_cond );
                if ( i4_ret != RMR_OK )
                {
                    return SMR_NOT_ALLOW;    
                }
                
                _sm_bypass_hdlr_vd_res_2_vs( &t_drv_res, pt_vsh_res );
            }
            else
            {
                TV_DEC_RESOLUTION_INFO_T     t_drv_res;
                
                z_drv_res_len = sizeof(TV_DEC_RESOLUTION_INFO_T);
                i4_ret = rm_get( pt_bypass_obj->t_bypass_comp_grp.t_dec.h_comp,
                                 FALSE,
                                 pt_bypass_obj->t_bypass_comp_grp.t_dec.t_info.ui1_in_port,
                                 TV_DEC_GET_TYPE_RESOLUTION,
                                 (VOID*)&t_drv_res,
                                 &z_drv_res_len,
                                 &pt_bypass_obj->t_bypass_comp_grp.t_dec.e_comp_cond );
                if ( i4_ret != RMR_OK )
                {
                    return SMR_NOT_ALLOW;    
                }
                
                _sm_bypass_hdlr_tvd_res_2_vs( pt_bypass_obj, &t_drv_res, pt_vsh_res );
            }
            
            *pz_get_info_len = sizeof( VSH_SRC_RESOLUTION_INFO_T );
        }
        break;

    case SM_BSH_GET_TYPE_VIDEO_OUT_CTRL:
        {
            BSH_VIDEO_OUT_CTRL_T* pe_bsh_ctrl = (BSH_VIDEO_OUT_CTRL_T*)pv_get_info;
            VIDEO_OUT_CTRL_T      e_drv_ctrl;
            SIZE_T                z_drv_len = sizeof(VIDEO_OUT_CTRL_T);
            
            if ( !pe_bsh_ctrl )
            {
                return SMR_INV_GET_INFO;
            }
            if ( !pz_get_info_len || 
                 *pz_get_info_len < sizeof(BSH_VIDEO_OUT_CTRL_T) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }

            i4_ret = rm_get( pt_bypass_obj->t_bypass_comp_grp.t_vout.h_comp,
                             FALSE,
                             pt_bypass_obj->t_bypass_comp_grp.t_vout.t_info.ui1_in_port,
                             VIDEO_OUT_GET_TYPE_CTRL,
                             (VOID*)&e_drv_ctrl,
                             &z_drv_len,
                             &pt_bypass_obj->t_bypass_comp_grp.t_vout.e_comp_cond );
            if ( i4_ret != RMR_OK )
            {
                return SMR_NOT_ALLOW;    
            }

            *pe_bsh_ctrl = (e_drv_ctrl == VIDEO_OUT_CTRL_DISABLE) ? BSH_VIDEO_OUT_CTRL_DISABLE : BSH_VIDEO_OUT_CTRL_ENABLE;
            *pz_get_info_len = sizeof( BSH_VIDEO_OUT_CTRL_T );

        }
        break;

    case SM_BSH_GET_TYPE_TV_ENC_CTRL:
        {
            BSH_TV_ENC_CTRL_T* pe_bsh_ctrl = (BSH_TV_ENC_CTRL_T*)pv_get_info;
            TV_ENC_CTRL_T      e_drv_ctrl;
            SIZE_T             z_drv_len = sizeof(TV_ENC_CTRL_T);
            
            if ( !pe_bsh_ctrl )
            {
                return SMR_INV_GET_INFO;
            }
            if ( !pz_get_info_len || 
                 *pz_get_info_len < sizeof(BSH_TV_ENC_CTRL_T) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }

            i4_ret = rm_get( pt_bypass_obj->t_bypass_comp_grp.t_tve.h_comp,
                             FALSE,
                             pt_bypass_obj->t_bypass_comp_grp.t_tve.t_info.ui1_in_port,
                             TV_ENC_GET_TYPE_CTRL,
                             (VOID*)&e_drv_ctrl,
                             &z_drv_len,
                             &pt_bypass_obj->t_bypass_comp_grp.t_tve.e_comp_cond );
            if ( i4_ret != RMR_OK )
            {
                return SMR_NOT_ALLOW;    
            }

            *pe_bsh_ctrl = (e_drv_ctrl == TV_ENC_CTRL_DISABLE) ? BSH_TV_ENC_CTRL_DISABLE : BSH_TV_ENC_CTRL_ENABLE;
            *pz_get_info_len = sizeof( BSH_TV_ENC_CTRL_T );

        }
        break;

    case SM_BSH_GET_TYPE_IGNORE:
    default:
        return SMR_NOT_SUPPORT;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
                    Exported function implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: sm_bypass_hdlr_send_msg
 *
 * Description: <function description>
 *
 * Inputs:  pt_msg References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sm_bypass_hdlr_send_msg( SM_BYPASS_HDLR_MSG_T* pt_msg )
{
    INT32                 i4_ret, i4_retry = 0;
    
    if ( !pt_msg )
    {
        return;
    }
    
    if ( !x_handle_valid( pt_msg->h_stream ) )
    {
        return;
    }
    
    pt_msg->ui4_time_stamp = x_os_get_sys_tick();
    
    while ( ++i4_retry <= 10 )
    {
        i4_ret = x_msg_q_send( gh_bypass_hdlr_msgq,
                               pt_msg,
                               sizeof( SM_BYPASS_HDLR_MSG_T ),
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
        
        i4_ret = sm_trigger_task( gh_bypass_handler, (VOID*)gh_bypass_handler );
        if ( i4_ret != SMR_OK )
        {
            SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
        }
        
        /* success */
        return;
    }

    /* send message fail */
    SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
}

/*-----------------------------------------------------------------------------
 * Name: sm_bypass_hdlr_send_msg_do_event
 *
 * Description: <function description>
 *
 * Inputs:  h_stream   References.
 *          ui4_event  References.
 *          ui4_data   References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sm_bypass_hdlr_send_msg_do_event(
    HANDLE_T                   h_stream,
    UINT32                     ui4_event,
    UINT32                     ui4_data )
{
    SM_BYPASS_HDLR_MSG_T  t_msg;
    
    t_msg.h_stream = h_stream;
    t_msg.e_msg_type = SM_BYPASS_HDLR_MSG_DO_EVENT;
    
    t_msg.u.t_msg_do_event.ui4_event = ui4_event;
    t_msg.u.t_msg_do_event.ui4_data = ui4_data;
    
    sm_bypass_hdlr_send_msg( &t_msg );
}

/*-----------------------------------------------------------------------------
 * Name: sm_bypass_hdlr_open_fct
 *
 * Description: Bypass stream handler stream open function.
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
 * Returns: SMR_OK                   Success.
 *          SMR_INSUFFICIENT_MEMORY  Fail.
 *          SMR_OUT_OF_HANDLES       Fail.
 *          SMR_INV_HANDLE           Fail.
 *          SMR_INTERNAL_ERROR       Fail.
 *          SMR_INV_ARG              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_bypass_hdlr_open_fct( 
    const SM_STREAM_T*     pt_stream_base,
    const SM_COMMAND_T*    pt_cmd_sets,
    BOOL                   b_auto_play,
    x_stream_nfy_fct       pf_sm_nfy,
    VOID*                  pv_sm_nfy_tag,
    HANDLE_T*              ph_stream )   /* OUT */
{
    SM_BYPASS_STREAM_T*    pt_bypass_obj = NULL;
    SM_SESSION_T*          pt_sess_obj = NULL;
    HANDLE_T               h_stream = NULL_HANDLE;
    INT32                  i4_ret = SMR_OK;
    SM_BYPASS_STATE_TAG_T* pt_state_tag = NULL;
    SIZE_T                 z_get_size = sizeof(UINT16);
    
    if ( !pt_stream_base || !ph_stream || !pf_sm_nfy )
    {
        return SMR_INV_ARG;
    }
    
    pt_sess_obj = (SM_SESSION_T*)sm_get_sess_obj( pt_stream_base->h_sess );
    if ( !pt_sess_obj )
    {
        return SMR_SESS_NOT_EXIST;
    }
    
    pt_state_tag = (SM_BYPASS_STATE_TAG_T*)x_mem_alloc( sizeof(SM_BYPASS_STATE_TAG_T) );
    if ( !pt_state_tag )
    {
        return SMR_SESS_NOT_EXIST;
    }
    
    pt_bypass_obj = (SM_BYPASS_STREAM_T*) x_mem_alloc( sizeof(SM_BYPASS_STREAM_T) );
    if ( !pt_bypass_obj )
    {
        x_mem_free( pt_state_tag );
        return SMR_INSUFFICIENT_MEMORY;
    }

    x_memset( pt_bypass_obj, 0, sizeof(SM_BYPASS_STREAM_T) );

    pt_bypass_obj->t_base = *pt_stream_base;
    pt_bypass_obj->t_base.t_strm_comp_id.e_type = ST_BYPASS; /* reassign to ST_VIDEO? */
    pt_bypass_obj->pf_sess_hdlr_sm_nfy = pf_sm_nfy;
    pt_bypass_obj->pv_sess_hdlr_sm_nfy_tag = pv_sm_nfy_tag;
    pt_bypass_obj->t_bypass_comp_grp = GT_SM_BYPASS_COMP_GRP_INITOR;
    pt_bypass_obj->ui4_lastest_ca_event = SM_EVN_CA_COND_UNKNOWN;
    pt_bypass_obj->ui4_lastest_pm_event = SM_EVN_IGNORE;
    pt_bypass_obj->ui4_lastest_dec_event = SM_EVN_IGNORE;
    pt_bypass_obj->ui4_lastest_tve_event = SM_EVN_IGNORE;
    pt_bypass_obj->ui4_lastest_vout_event = SM_EVN_IGNORE; 
    pt_bypass_obj->ui4_lastest_ca_detail_code = 0;
    x_memset( &pt_bypass_obj->t_vid_res, 0, sizeof(VSH_SRC_RESOLUTION_INFO_T) );
    pt_bypass_obj->b_pending_res_chg = FALSE;    
    pt_bypass_obj->e_tv_signal = TV_DEC_TV_SIG_UNKNOWN;
    pt_bypass_obj->e_color_sys = COLOR_SYS_UNKNOWN;
    pt_bypass_obj->ui2_force_pid = MPEG_2_NULL_PID;
    pt_bypass_obj->ui1_force_enc = VID_ENC_MPEG_2;
#ifdef SYS_MHP_SUPPORT
    pt_bypass_obj->e_ca_req_lvl = PM_CA_REQUIRE_MANDATORY;
#endif
    pt_bypass_obj->b_not_enable_tve_as_play = FALSE;
    pt_bypass_obj->u_dec_ctrl_type.e_vd_ctrl_type = VID_DEC_CTRL_RESET;

    pt_bypass_obj->h_vbif = NULL_HANDLE;
    pt_bypass_obj->e_vbif_cond = VBIF_COND_IGNORE;
    pt_bypass_obj->t_vbif_src.e_src_type = BSH_VBIF_SRC_TYPE_UNKNOWN;
    pt_bypass_obj->t_vbif_src.ui2_pid = MPEG_2_NULL_PID;
    pt_bypass_obj->b_codec_not_support = TRUE;
    pt_bypass_obj->e_actual_enc = VID_ENC_MPEG_2;
    x_memset( &pt_bypass_obj->t_cp_info_lst, 0, sizeof(CP_INFO_LIST_T) );
    x_memset( &pt_bypass_obj->t_rd3d_info, 0, sizeof(RD3D_REG_DESC_T) );
    x_memset( &pt_bypass_obj->t_mvc_ext, 0, sizeof(MVC_EXTENSION_DESC_T) );

    /* initialize driver descriptors */
    pt_bypass_obj->t_bypass_comp_grp.t_dec.t_info.e_type = DRVT_UNKNOWN;
    pt_bypass_obj->t_bypass_comp_grp.t_tve.t_info.e_type = DRVT_TV_ENC;
    pt_bypass_obj->t_bypass_comp_grp.t_vout.t_info.e_type = DRVT_VIDEO_OUT;

    x_strcpy( pt_bypass_obj->t_bypass_comp_grp.t_dec.t_info.s_logical_name,
              pt_sess_obj->s_dest_grp_name );
    x_strcpy( pt_bypass_obj->t_bypass_comp_grp.t_tve.t_info.s_logical_name,
              pt_sess_obj->s_dest_grp_name );
    x_strcpy( pt_bypass_obj->t_bypass_comp_grp.t_vout.t_info.s_logical_name,
              pt_sess_obj->s_dest_grp_name );

    i4_ret = x_sema_create( &pt_bypass_obj->t_base.h_lock,
                            X_SEMA_TYPE_MUTEX,
                            X_SEMA_STATE_UNLOCK );
    if ( i4_ret != OSR_OK || pt_bypass_obj->t_base.h_lock == NULL_HANDLE )
    {
        x_mem_free( pt_bypass_obj );
        x_mem_free( pt_state_tag );
        return SMR_INTERNAL_ERROR;   
    }
    
    /* create stream handle */
    i4_ret = handle_alloc( SM_HT_STREAM,
                           pt_bypass_obj,
                           (VOID*)pv_sm_nfy_tag,
                           &_sm_bypass_hdlr_bs_free_fct,
                           &h_stream );
    if ( i4_ret != HR_OK || h_stream == NULL_HANDLE )
    {
        x_sema_delete( pt_bypass_obj->t_base.h_lock);
        x_mem_free( pt_bypass_obj );
        x_mem_free( pt_state_tag );
        return SMR_OUT_OF_HANDLES;
    }
    pt_bypass_obj->t_base.h_stream = h_stream;
    *ph_stream = h_stream;

    pt_state_tag->h_stream = h_stream;
    pt_state_tag->pt_bypass_obj = pt_bypass_obj;
    i4_ret = sm_state_init( at_bypass_state_cond_lst,
                            at_bypass_state_descr_lst,
                            at_bypass_allowed_pending_event_lst,
                            sm_bypass_state_notify_app,
                            (VOID*)pt_state_tag,
                            &pt_bypass_obj->t_state );
    if ( i4_ret != SMR_OK )
    {
        handle_free( h_stream, FALSE );
        return SMR_INTERNAL_ERROR;   
    }

    SM_LOCK( pt_bypass_obj->t_base.h_lock );
        
    /* process commands */
    if ( pt_cmd_sets )
    {
        i4_ret = sm_bypass_exec_cmd( h_stream, pt_bypass_obj, pt_cmd_sets);
        if ( i4_ret != SMR_OK )
        {
            SM_UNLOCK( pt_bypass_obj->t_base.h_lock );
            handle_free( h_stream, FALSE );
            return i4_ret;    
        }
    }

    /* Get PCR ID from session. */
    i4_ret = x_sm_get( pt_bypass_obj->t_base.h_sess,
                       SM_SESS_GNRC_GET_TYPE_PCR_ID,
                       &pt_bypass_obj->t_base.ui2_pcr_id,
                       &z_get_size );
    if ( i4_ret != SMR_OK )
    {
        SM_UNLOCK( pt_bypass_obj->t_base.h_lock );
        return SMR_DRV_COMP_ERROR;
    }

    sm_bypass_hdlr_send_msg_do_event(
        h_stream,
        SM_EVN_BYPASS_APP_OPEN_REQ,
        0 );
    
    if ( b_auto_play )
    {
        sm_bypass_hdlr_send_msg_do_event(
            h_stream,
            SM_EVN_BYPASS_APP_PLAY_REQ,
            0 );
    }
    
    SM_UNLOCK( pt_bypass_obj->t_base.h_lock );

    return SMR_OK;

}

/*-----------------------------------------------------------------------------
 * Name: sm_bypass_hdlr_close_fct
 *
 * Description: Bypass stream handler stream close function.
 *
 * Inputs:  h_stream References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *          SMR_INV_HANDLE      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_bypass_hdlr_close_fct( HANDLE_T h_stream )
{
    SM_BYPASS_STREAM_T*    pt_bypass_obj = NULL;

    pt_bypass_obj = (SM_BYPASS_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_bypass_obj )
    {
        return SMR_INV_HANDLE;
    }

    sm_bypass_hdlr_send_msg_do_event(
        h_stream,
        SM_EVN_BYPASS_APP_CLOSE_REQ,
        0 );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_bypass_hdlr_set_fct
 *
 * Description: Bypass stream handler set function.
 *
 * Inputs:  h_stream       References.
 *          e_set_type     References.
 *          pv_set_info    References.
 *          z_set_info_len References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK                        Success.
 *          SMR_INV_ARG                   The e_set_type has invalid set type
 *                                        group.
 *          SMR_INV_HANDLE                h_stream is invalid.
 *          SMR_CAN_NOT_OP_IN_THIS_STATE  Could not process this request in
 *                                        this state.
 *          SMR_NOT_ALLOW                 It's not allowed to set attributes
 *                                        for a Extraction stream.
 *          SMR_NOT_SUPPORT               The e_set_type is not support by
 *                                        Extraction stream handler.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_bypass_hdlr_set_fct( HANDLE_T         h_stream,
                              SM_SET_TYPE_T    e_set_type,
                              VOID*            pv_set_info,
                              SIZE_T           z_set_info_len )
{
    INT32                  i4_ret = SMR_OK;
    SM_BYPASS_STREAM_T*    pt_bypass_obj = NULL;

    pt_bypass_obj = (SM_BYPASS_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_bypass_obj )
    {
        return SMR_INV_HANDLE;
    }

    if ( pt_bypass_obj->t_base.e_condition != SM_COND_IGNORE &&
         pt_bypass_obj->t_base.e_condition != SM_COND_OPENING &&
         pt_bypass_obj->t_base.e_condition != SM_COND_OPENED &&
         pt_bypass_obj->t_base.e_condition != SM_COND_STARTING &&
         pt_bypass_obj->t_base.e_condition != SM_COND_STARTED &&
         pt_bypass_obj->t_base.e_condition != SM_COND_STOPPING )
    {
        return SMR_CAN_NOT_OP_IN_THIS_STATE;
    }

    switch ( SM_GET_GRP(e_set_type) )
    {
    case SM_GRP_STRM:
    case SM_GRP_SESS:
        {
            UINT32    ui4_event;

            if ( pt_bypass_obj->t_base.e_condition == SM_COND_STARTED &&
                 ( e_set_type == SM_SESS_SET_TYPE_PLAY ||
                   e_set_type == SM_STRM_SET_TYPE_PLAY ) )
            {
                return SMR_ALREADY_PLAYED;
            }
            if ( pt_bypass_obj->t_base.e_condition == SM_COND_OPENED &&
                 ( e_set_type == SM_SESS_SET_TYPE_STOP ||
                   e_set_type == SM_STRM_SET_TYPE_STOP ) )
            {
                return SMR_ALREADY_STOPPED;
            }

            if ( e_set_type == SM_SESS_SET_TYPE_PLAY )
            {
                ui4_event = SM_EVN_SESS_APP_PLAY_REQ;
            }
            else if ( e_set_type == SM_SESS_SET_TYPE_STOP )
            {
                ui4_event = SM_EVN_SESS_APP_STOP_REQ;
            }
            else if ( e_set_type == SM_STRM_SET_TYPE_PLAY )
            {
                ui4_event = SM_EVN_BYPASS_APP_PLAY_REQ;
            }
            else if ( e_set_type == SM_STRM_SET_TYPE_STOP )
            {
                ui4_event = SM_EVN_BYPASS_APP_STOP_REQ;
            }
            else if ( e_set_type == SM_STRM_SET_TYPE_CCI_INFO )
            {
#ifdef CI_PLUS_SUPPORT
                CP_INFO_LIST_T* pt_cp_info_lst;
                pt_cp_info_lst = (CP_INFO_LIST_T*)pv_set_info;
                
                if ( !pt_cp_info_lst )
                {
                    return SMR_INV_SET_INFO;
                }
                if ( z_set_info_len != sizeof( CP_INFO_LIST_T ) )
                {
                    return SMR_INSUFFICIENT_SIZE;
                }
                
                return _sm_bypass_hdlr_set_cci_plus_info( pt_bypass_obj, pt_cp_info_lst );
#else
                CP_INFO_T*    pt_cp_info = (CP_INFO_T*)pv_set_info;
                
                if ( !pt_cp_info )
                {
                    return SMR_INV_SET_INFO;
                }
                if ( z_set_info_len != sizeof( CP_INFO_T ) )
                {
                    return SMR_INSUFFICIENT_SIZE;
                }
                
                return _sm_bypass_hdlr_set_cci_info( pt_bypass_obj, pt_cp_info );
#endif                
            }
            else
            {
                return SMR_NOT_SUPPORT;
            }

            sm_bypass_hdlr_send_msg_do_event(
                h_stream,
                ui4_event,
                0 );
        }
        break;


    case SM_GRP_BSH:
        i4_ret = _sm_bypass_hdlr_set( h_stream,
                                      e_set_type,
                                      pv_set_info,
                                      z_set_info_len );
        break;

    default:
        return SMR_INV_ARG;
    }

    return i4_ret;

}

/*-----------------------------------------------------------------------------
 * Name: sm_bypass_hdlr_get_fct
 *
 * Description: Bypass stream handler stream get function definition
 *
 * Inputs:  h_stream        References.
 *          e_get_type      References.
 *          pv_get_info     References.
 *          pz_get_info_len References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK                        Success.
 *          SMR_INV_HANDLE                h_stream is invalid.
 *          SMR_CAN_NOT_OP_IN_THIS_STATE  Could not process this request in
 *                                        current state.
 *          SMR_INV_ARG                   pz_get_info_len is invalid or
 *                                        e_get_type has invalid get type group.
 *          SMR_INV_GET_INFO              pv_get_info is invalid.
 *          SMR_NOT_ALLOW                 It's not allowed to set attributes
 *                                        for a Extraction stream.
 *          SMR_NOT_SUPPORT               The e_get_type is not supported by
 *                                        Extraction stream handler.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_bypass_hdlr_get_fct( HANDLE_T         h_stream,
                              SM_GET_TYPE_T    e_get_type,
                              VOID*            pv_get_info,       /* IN/OUT */
                              SIZE_T*          pz_get_info_len )  /* IN/OUT */
{
    INT32                  i4_ret = SMR_OK;
    SM_BYPASS_STREAM_T*    pt_bypass_obj = NULL;

    if ( !pz_get_info_len )
    {
        return SMR_INV_ARG;
    }
    if ( !pv_get_info )
    {
        return SMR_INV_GET_INFO;
    }

    pt_bypass_obj = (SM_BYPASS_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_bypass_obj )
    {
        return SMR_INV_HANDLE;
    }

    if ( pt_bypass_obj->t_base.e_condition != SM_COND_OPENED &&
         pt_bypass_obj->t_base.e_condition != SM_COND_STARTING &&
         pt_bypass_obj->t_base.e_condition != SM_COND_STARTED &&
         pt_bypass_obj->t_base.e_condition != SM_COND_STOPPING )
    {
        return SMR_CAN_NOT_OP_IN_THIS_STATE;
    }

    switch ( SM_GET_GRP(e_get_type) )
    {
    case SM_GRP_STRM:
    case SM_GRP_SESS:
        i4_ret = _sm_bypass_hdlr_slctr_get( h_stream,
                                            e_get_type,
                                            pv_get_info,
                                            pz_get_info_len );
        break;

    case SM_GRP_BSH:
        i4_ret = _sm_bypass_hdlr_get( h_stream,
                                      e_get_type,
                                      pv_get_info,
                                      pz_get_info_len );
        break;

    default:
        return SMR_INV_ARG;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: sm_bypass_hdlr_task_fct
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
VOID sm_bypass_hdlr_task_fct( VOID* pv_arg )
{
    INT32                   i4_ret = SMR_OK;
    UINT16                  ui2_index;
    HANDLE_TYPE_T           e_hdlr_type = 0;
    SM_BYPASS_STREAM_T*     pt_bypass_obj = NULL;
    SM_BYPASS_HDLR_MSG_T    t_msg;
    SIZE_T                  z_msg_size = sizeof(SM_BYPASS_HDLR_MSG_T);

    x_memset( &t_msg, 0, sizeof(SM_BYPASS_HDLR_MSG_T) );

    i4_ret = x_msg_q_receive( &ui2_index,
                              &t_msg,
                              &z_msg_size,
                              &gh_bypass_hdlr_msgq,
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

    i4_ret = x_handle_get_type( t_msg.h_stream, &e_hdlr_type );
    if ( i4_ret != HR_OK || e_hdlr_type != SM_HT_STREAM )
    {
        return;
    }

    pt_bypass_obj = (SM_BYPASS_STREAM_T*)sm_get_strm_obj( t_msg.h_stream );
    if ( !pt_bypass_obj )
    {
        SM_ABORT( SM_DBG_ABRT_INV_STRM_OBJ );
    }

    SM_LOCK_RETURN_ON_FAIL( pt_bypass_obj->t_base.h_lock, );

    switch( t_msg.e_msg_type )
    {
    case SM_BYPASS_HDLR_MSG_DO_EVENT:
        _sm_bypass_hdlr_do_msg_do_event(
            t_msg.h_stream,
            pt_bypass_obj,
            &t_msg.u.t_msg_do_event );
        break;

    case SM_BYPASS_HDLR_MSG_DEC_RM_NFY:
        _sm_bypass_hdlr_do_msg_rm_nfy(
            t_msg.h_stream,
            pt_bypass_obj,
            SM_EVN_BYPASS_GRP_DEC,
            &t_msg.u.t_msg_rm );
        break;

    case SM_BYPASS_HDLR_MSG_TVE_RM_NFY:
        _sm_bypass_hdlr_do_msg_rm_nfy(
            t_msg.h_stream,
            pt_bypass_obj,
            SM_EVN_BYPASS_GRP_TVE,
            &t_msg.u.t_msg_rm );
        break;

    case SM_BYPASS_HDLR_MSG_VOUT_RM_NFY:
        _sm_bypass_hdlr_do_msg_rm_nfy(
            t_msg.h_stream,
            pt_bypass_obj,
            SM_EVN_BYPASS_GRP_VOUT,
            &t_msg.u.t_msg_rm );
        break;

    case SM_BYPASS_HDLR_MSG_VD_CTRL_NFY:
        _sm_bypass_hdlr_do_msg_vd_ctrl_nfy(
            t_msg.h_stream,
            pt_bypass_obj,
            &t_msg.u.t_msg_vd_ctrl );
        break;

    case SM_BYPASS_HDLR_MSG_TVD_CTRL_NFY:
        _sm_bypass_hdlr_do_msg_tvd_ctrl_nfy(
            t_msg.h_stream,
            pt_bypass_obj,
            &t_msg.u.t_msg_tvd_ctrl );
        break;

    case SM_BYPASS_HDLR_MSG_PM_NFY:
        _sm_bypass_hdlr_do_msg_pm_nfy(
            t_msg.h_stream,
            pt_bypass_obj,
            &t_msg.u.t_msg_pm );
        break;


    case SM_BYPASS_HDLR_MSG_CCI_NFY:
        _sm_bypass_hdlr_do_msg_cci_msg( 
            t_msg.h_stream,
            pt_bypass_obj,
            &t_msg.u.t_msg_cci );
        break;


    case SM_BYPASS_HDLR_MSG_CA_NFY:
        _sm_bypass_hdlr_do_msg_ca_nfy(
            t_msg.h_stream,
            pt_bypass_obj,
            &t_msg.u.t_msg_ca );
        break;

    case SM_BYPASS_HDLR_MSG_SCDB_NFY:
        _sm_bypass_hdlr_do_msg_scdb_nfy(
            t_msg.h_stream,
            pt_bypass_obj,
            &t_msg.u.t_msg_scdb );
        break;

    case SM_BYPASS_HDLR_MSG_TYPE_IGNORE:
    default:
        SM_BYPASS_DBG_ERR((
            "[SM] BYPASS, invalid bypass handler message type = 0x%.8X\r\n",
            t_msg.e_msg_type ));
        SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
    }

    SM_UNLOCK( pt_bypass_obj->t_base.h_lock );
}

/*-----------------------------------------------------------------------------
 * Name: sm_bypass_hdlr_init
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
INT32 sm_bypass_hdlr_init( VOID )
{
    INT32               i4_ret = SMR_OK;
    SM_HDLR_DESC_T      t_hdlr_desc;
    SM_HDLR_FCT_TBL_T   t_fct_tbl;
    
    x_strcpy( t_hdlr_desc.s_hdlr_name, SM_BYPASS_STRM_HDLR_NAME );
    t_hdlr_desc.ui4_strm_type_mask = ST_MASK_BYPASS;
    
    t_fct_tbl.pf_open = &sm_bypass_hdlr_open_fct;
    t_fct_tbl.pf_close = &sm_bypass_hdlr_close_fct;
    t_fct_tbl.pf_set = &sm_bypass_hdlr_set_fct;
    t_fct_tbl.pf_get = &sm_bypass_hdlr_get_fct;
    t_fct_tbl.pf_task = &sm_bypass_hdlr_task_fct;

    /* Create message queue for Bypass Handler Task */
    if ( gh_bypass_hdlr_msgq == NULL_HANDLE )
    {
        i4_ret = x_msg_q_create( &gh_bypass_hdlr_msgq,
                                 SM_BYPASS_STRM_HDLR_NAME,  /* use thread name */
                                 sizeof( SM_BYPASS_HDLR_MSG_T ),
                                 SM_BYPASS_HDLR_MSGQ_MAX_SIZE );
        if ( i4_ret != OSR_OK )
        {
            return SMR_INTERNAL_ERROR;
        }
    }
    
    i4_ret = sm_reg_strm_handler( &t_hdlr_desc,
                                  SM_GNRC_SESS_HDLR_NAME,
                                  &t_fct_tbl,
                                  FALSE,
                                  NULL,
                                  &gh_bypass_handler );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INTERNAL_ERROR;
    }
    
    return SMR_OK;
}

#endif /* MW_TV_AV_BYPASS_SUPPORT */

