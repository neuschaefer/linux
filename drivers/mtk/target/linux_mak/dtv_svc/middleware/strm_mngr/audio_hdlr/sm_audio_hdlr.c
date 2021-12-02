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
 * $RCSfile: sm_audio_hdlr.c,v $
 * $Revision: #2 $
 * $Date: 2012/08/27 $
 * $Author: zhenlu.liu $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 7463ff582d72f7a86d0d073941d29a7c $
 *
 * Description: 
 *         This file implements the Audio Stream Handler.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "res_mngr/drv/x_aud_dec.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/rm.h"
#include "dbg/x_dbg.h"
#include "dbg/dbg.h"

#include "prc_mngr/prc_mngr.h"
#include "scdb/x_scdb_api.h"
#include "strm_mngr/sm_slctr.h"
#include "strm_mngr/_sm_utils.h"
#include "strm_mngr/sm_drv_comp.h"
#include "strm_mngr/sm_drv_map.h"
#include "strm_mngr/sm_state.h"

#include "strm_mngr/sess_gnrc_hdlr/x_sm_sess_gnrc.h"
#include "strm_mngr/audio_hdlr/x_sm_audio_hdlr.h"
#include "strm_mngr/audio_hdlr/sm_audio_hdlr.h"
#include "strm_mngr/audio_hdlr/_sm_audio_hdlr.h"
#include "strm_mngr/audio_hdlr/_sm_as_state.h"
#include "strm_mngr/audio_hdlr/_sm_as_ad.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define SM_AUDIO_HDLR_MSGQ_MAX_SIZE    256
#define AS_COMP_NAME_MAX_LEN           16



/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static HANDLE_T   gh_audio_handler = NULL_HANDLE;
static HANDLE_T   gh_audio_hdlr_msgq = NULL_HANDLE;

/* 
 *  Audio Stream State Machine 
 */
static SM_STATE_DESCR_T   at_as_state_descr_lst[] =
{
/*  ui4_state,            b_prcss_pending,    b_allow_pending,  pf_state_do,                pf_state_do_exception */
  { SM_COND_IGNORE,       FALSE,              FALSE,            NULL,                       sm_as_state_do_ignore_excpt },
  { SM_COND_OPENING,      FALSE,              TRUE,             sm_as_state_do_opening,     sm_as_state_do_opening_excpt },
  { SM_COND_OPENED,       TRUE,               FALSE,            sm_as_state_do_opened,      sm_as_state_do_opened_excpt },
  { SM_COND_STARTING,     FALSE,              TRUE,             sm_as_state_do_starting,    sm_as_state_do_starting_excpt },
  { SM_COND_STARTED,      TRUE,               FALSE,            sm_as_state_do_started,     sm_as_state_do_started_excpt },
  { SM_COND_STOPPING,     FALSE,              TRUE,             sm_as_state_do_stopping,    sm_as_state_do_stopping_excpt },
  { SM_COND_CLOSING,      FALSE,              TRUE,             sm_as_state_do_closing,     sm_as_state_do_closing_excpt },
  { SM_COND_CLOSED,       FALSE,              FALSE,            sm_as_state_do_closed,      sm_as_state_do_closed_excpt },
  { SM_COND_ERROR,        TRUE,               FALSE,            sm_as_state_do_error,       sm_as_state_do_error_excpt },
  SM_STATE_DESCR_NULL  
};

static UINT32             at_as_allowed_pending_event_lst[] =
{
  SM_EVN_AS_APP_PLAY_REQ,
  SM_EVN_AS_APP_STOP_REQ,
  SM_EVN_AS_APP_CLOSE_REQ,
  SM_EVN_AS_SCDB_RECORD_DEL,
  SM_EVN_SESS_APP_PLAY_REQ,
  SM_EVN_SESS_APP_STOP_REQ,
  SM_EVN_AS_RESTART,
  SM_EVN_IGNORE
};

static SM_STATE_COND_T    at_as_state_cond_lst[] =
{
  /* ui4_curr_state,  ui4_event,                ui4_next_state,     ui4_pending_event,      pf_event_do */
  /* ignore state conditions *************************************************/ 
  { SM_COND_IGNORE,   SM_EVN_AS_APP_OPEN_REQ,   SM_COND_OPENING,    SM_EVN_IGNORE,          NULL },
  
  /* opening state conditions ************************************************/ 
  { SM_COND_OPENING,  SM_EVN_AS_DRV_OPENED,     SM_COND_OPENED,     SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENING,  SM_EVN_AS_APP_PLAY_REQ,   SM_COND_OPENING,    SM_EVN_AS_APP_PLAY_REQ, NULL },
  { SM_COND_OPENING,  SM_EVN_SESS_APP_PLAY_REQ, SM_COND_OPENING,    SM_EVN_SESS_APP_PLAY_REQ, NULL },
  { SM_COND_OPENING,  SM_EVN_SESS_APP_STOP_REQ, SM_COND_OPENING,    SM_EVN_SESS_APP_STOP_REQ, NULL },  
  { SM_COND_OPENING,  SM_EVN_AS_APP_STOP_REQ,   SM_COND_OPENING,    SM_EVN_AS_APP_STOP_REQ,  NULL },
  { SM_COND_OPENING,  SM_EVN_AS_APP_CLOSE_REQ,  SM_COND_OPENING,    SM_EVN_AS_APP_CLOSE_REQ, NULL },
  { SM_COND_OPENING,  SM_EVN_AS_SCDB_RECORD_DEL,SM_COND_OPENING,    SM_EVN_AS_APP_CLOSE_REQ, NULL },
  { SM_COND_OPENING,  SM_EVN_AS_DRV_PLAYED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENING,  SM_EVN_AS_DRV_STOPPED,    SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENING,  SM_EVN_AS_DRV_CLOSED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENING,  SM_EVN_AS_DRV_ERR,        SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENING,  SM_EVN_AS_INTERNAL_ERR,   SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  
  /* opened state conditions *************************************************/ 
  { SM_COND_OPENED,   SM_EVN_AS_APP_PLAY_REQ,   SM_COND_STARTING,   SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENED,   SM_EVN_SESS_APP_PLAY_REQ, SM_COND_STARTING,   SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENED,   SM_EVN_AS_APP_CLOSE_REQ,  SM_COND_CLOSING,    SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENED,   SM_EVN_AS_SCDB_RECORD_DEL,SM_COND_CLOSING,    SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENED,   SM_EVN_AS_DRV_OPENED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENED,   SM_EVN_AS_DRV_PLAYED,     SM_COND_STARTING,   SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENED,   SM_EVN_AS_DRV_STOPPED,    SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENED,   SM_EVN_AS_DRV_CLOSED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENED,   SM_EVN_AS_DRV_ERR,        SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENED,   SM_EVN_AS_INTERNAL_ERR,   SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  
  /* starting state conditions ***********************************************/ 
  { SM_COND_STARTING, SM_EVN_AS_DRV_PLAYED,     SM_COND_STARTED,    SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTING, SM_EVN_AS_APP_STOP_REQ,   SM_COND_STARTING,   SM_EVN_AS_APP_STOP_REQ, NULL },
  { SM_COND_STARTING, SM_EVN_SESS_APP_STOP_REQ, SM_COND_STARTING,   SM_EVN_SESS_APP_STOP_REQ, NULL },
  { SM_COND_STARTING, SM_EVN_AS_APP_CLOSE_REQ,  SM_COND_STARTING,   SM_EVN_AS_APP_CLOSE_REQ, NULL },
  { SM_COND_STARTING, SM_EVN_AS_RESTART,        SM_COND_STARTING,   SM_EVN_AS_RESTART,      NULL },
  { SM_COND_STARTING, SM_EVN_AS_SCDB_RECORD_DEL,SM_COND_STARTING,   SM_EVN_AS_APP_CLOSE_REQ, NULL },
  { SM_COND_STARTING, SM_EVN_AS_DRV_OPENED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTING, SM_EVN_AS_DRV_STOPPED,    SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTING, SM_EVN_AS_DRV_CLOSED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTING, SM_EVN_AS_DRV_ERR,        SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTING, SM_EVN_AS_INTERNAL_ERR,   SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  
  /* started state conditions ************************************************/ 
  { SM_COND_STARTED,  SM_EVN_AS_APP_STOP_REQ,   SM_COND_STOPPING,   SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTED,  SM_EVN_SESS_APP_STOP_REQ, SM_COND_STOPPING,   SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTED,  SM_EVN_AS_APP_CLOSE_REQ,  SM_COND_STOPPING,   SM_EVN_AS_APP_CLOSE_REQ,NULL },
  { SM_COND_STARTED,  SM_EVN_AS_RESTART,        SM_COND_STOPPING,   SM_EVN_AS_APP_PLAY_REQ, NULL },
  { SM_COND_STARTED,  SM_EVN_AS_SCDB_RECORD_DEL,SM_COND_STOPPING,   SM_EVN_AS_APP_CLOSE_REQ,NULL },
  { SM_COND_STARTED,  SM_EVN_AS_DRV_OPENED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTED,  SM_EVN_AS_DRV_PLAYED,     SM_COND_STARTED,    SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTED,  SM_EVN_AS_DRV_STOPPED,    SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTED,  SM_EVN_AS_DRV_CLOSED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTED,  SM_EVN_AS_DRV_ERR,        SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTED,  SM_EVN_AS_INTERNAL_ERR,   SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  
  /* stopping state conditions ***********************************************/ 
  { SM_COND_STOPPING, SM_EVN_AS_DRV_STOPPED,    SM_COND_OPENED,     SM_EVN_IGNORE,          NULL },
  { SM_COND_STOPPING, SM_EVN_AS_APP_CLOSE_REQ,  SM_COND_STOPPING,   SM_EVN_AS_APP_CLOSE_REQ,NULL },
  { SM_COND_STOPPING, SM_EVN_AS_SCDB_RECORD_DEL,SM_COND_STOPPING,   SM_EVN_AS_APP_CLOSE_REQ,NULL },
  { SM_COND_STOPPING, SM_EVN_AS_APP_PLAY_REQ,   SM_COND_STOPPING,   SM_EVN_AS_APP_PLAY_REQ, NULL },
  { SM_COND_STOPPING, SM_EVN_SESS_APP_PLAY_REQ, SM_COND_STOPPING,   SM_EVN_SESS_APP_PLAY_REQ, NULL },
  { SM_COND_STOPPING, SM_EVN_AS_DRV_OPENED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STOPPING, SM_EVN_AS_DRV_PLAYED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STOPPING, SM_EVN_AS_DRV_CLOSED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STOPPING, SM_EVN_AS_DRV_ERR,        SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STOPPING, SM_EVN_AS_INTERNAL_ERR,   SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  
  /* closing state conditions ************************************************/ 
  { SM_COND_CLOSING,  SM_EVN_AS_DRV_CLOSED,     SM_COND_CLOSED,     SM_EVN_IGNORE,          NULL },
  { SM_COND_CLOSING,  SM_EVN_AS_DRV_ERR,        SM_COND_CLOSING,    SM_EVN_IGNORE,          NULL },
  { SM_COND_CLOSING,  SM_EVN_AS_INTERNAL_ERR,   SM_COND_CLOSING,    SM_EVN_IGNORE,          NULL },

  /* error state conditions ************************************************/ 
  { SM_COND_ERROR,    SM_EVN_AS_APP_CLOSE_REQ,  SM_COND_CLOSING,    SM_EVN_IGNORE,          NULL },
  { SM_COND_ERROR,    SM_EVN_AS_SCDB_RECORD_DEL,SM_COND_CLOSING,    SM_EVN_IGNORE,          NULL },
  { SM_COND_ERROR,    SM_EVN_AS_APP_PLAY_REQ,   SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_ERROR,    SM_EVN_SESS_APP_PLAY_REQ, SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_ERROR,    SM_EVN_AS_APP_STOP_REQ,   SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_ERROR,    SM_EVN_SESS_APP_STOP_REQ, SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_ERROR,    SM_EVN_AS_DRV_OPENED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_ERROR,    SM_EVN_AS_DRV_OPENED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_ERROR,    SM_EVN_AS_DRV_PLAYED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_ERROR,    SM_EVN_AS_DRV_STOPPED,    SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_ERROR,    SM_EVN_AS_DRV_CLOSED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },

  /* ANY state events ********************************************************/ 
  { SM_STATE_IGNORE,  SM_EVN_CCI_INFORMATION,   SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_as_state_do_cci_events },
  { SM_STATE_IGNORE,  SM_EVN_CA_COND_CLOSED,    SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_as_state_do_ca_events },
  { SM_STATE_IGNORE,  SM_EVN_CA_COND_UNKNOWN,   SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_as_state_do_ca_events },
  { SM_STATE_IGNORE,  SM_EVN_CA_COND_FREE,      SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_as_state_do_ca_events },
  { SM_STATE_IGNORE,  SM_EVN_CA_COND_AUTHORIZED,SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_as_state_do_ca_events },
  { SM_STATE_IGNORE,  SM_EVN_CA_COND_NOT_AUTHORIZED,SM_STATE_IGNORE,SM_EVN_IGNORE,          sm_as_state_do_ca_events },
  { SM_STATE_IGNORE,  SM_EVN_AS_AD_OPENED,      SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_as_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_AS_AD_CLOSED,      SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_as_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_AS_AD_CONNECTED,   SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_as_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_AS_AD_DISCONNECTED,SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_as_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_AS_AD_PLAYED,      SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_as_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_AS_AD_STOPPED,     SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_as_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_AS_AD_PAUSED,      SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_as_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_AS_PM_COND_CLOSED, SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_as_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_AS_PM_COND_PLAYED, SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_as_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_AS_PM_COND_STOPPED,SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_as_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_AS_AUD_INFO_CHG,   SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_as_state_do_aud_info_chg },
  { SM_STATE_IGNORE,  SM_EVN_AS_DECODE_STATUS_CHG,SM_STATE_IGNORE,  SM_EVN_IGNORE,          sm_as_state_do_aud_decode_status_chg },
#ifdef SM_ASH_NO_SIG_AUTO_MUTE    
   { SM_STATE_IGNORE,  SM_EVN_AS_SIG_MONITOR_TM, SM_STATE_IGNORE,    SM_EVN_IGNORE,         sm_as_state_do_signal_check },
#endif
  { SM_STATE_IGNORE,  SM_EVN_AS_TV_AUD_SYS_DETECTED,SM_STATE_IGNORE,  SM_EVN_IGNORE,    sm_as_state_do_tv_aud_sys },
#ifdef ENABLE_MULTIMEDIA        
  { SM_STATE_IGNORE,  SM_EVN_AS_EOS,            SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_as_state_do_event_nfy },
  { SM_STATE_IGNORE,  SM_EVN_AS_CODEC_NOT_SUPPORT,  SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_as_state_do_event_nfy },
#endif
  /* !!! Don't remove this !!!************************************************/
  SM_STATE_COND_NULL
};


/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal function implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_ash_ch_to_drv_ch
 *
 * Description: <function description>
 *
 * Inputs:  e_ash_ch     References.
 *
 * Outputs: - 
 *
 * Returns: AUD_DEC_LS_T
 *
 ----------------------------------------------------------------------------*/
static AUD_DEC_LS_T audio_hdlr_ash_ch_to_drv_ch( ASH_CHANNEL_T  e_ash_ch ) 
{
    AUD_DEC_LS_T e_drv_ch = AUD_DEC_LS_FRONT_LEFT;
    
    if ( e_ash_ch == ASH_CHANNEL_FRONT_LEFT )
    {
        e_drv_ch = AUD_DEC_LS_FRONT_LEFT;        
    }
    else if ( e_ash_ch == ASH_CHANNEL_FRONT_RIGHT )
    {
        e_drv_ch = AUD_DEC_LS_FRONT_RIGHT;
    }
    else if ( e_ash_ch == ASH_CHANNEL_REAR_LEFT )
    {
        e_drv_ch = AUD_DEC_LS_REAR_LEFT;
    }
    else if ( e_ash_ch == ASH_CHANNEL_REAR_RIGHT )
    {
        e_drv_ch = AUD_DEC_LS_REAR_RIGHT;
    }
    else if ( e_ash_ch == ASH_CHANNEL_CENTER )
    {
        e_drv_ch = AUD_DEC_LS_CENTER;
    }
    else if ( e_ash_ch == ASH_CHANNEL_SUB_WOOFER )
    {
        e_drv_ch = AUD_DEC_LS_SUB_WOOFER;
    }
    
    return e_drv_ch;
}

static AUD_DEC_DDC_ID_T ash_mix_id_2_drv( 
    ASH_AUD_DEC_DDC_ID_T e_ash_mix_id )
{
    switch( e_ash_mix_id )
    {
    case ASH_AUD_DEC_DDC_ID_UNKNOWN:
        return AUD_DEC_DDC_ID_UNKNOWN;

    case ASH_AUD_DEC_DDC_ID_1:
        return AUD_DEC_DDC_ID_1;

    case ASH_AUD_DEC_DDC_ID_2:
        return AUD_DEC_DDC_ID_2;

    case ASH_AUD_DEC_DDC_ID_3:
        return AUD_DEC_DDC_ID_3;


    default:
        return AUD_DEC_DDC_ID_UNKNOWN;
    }
}


/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_drv_ch_to_ash_ch
 *
 * Description: <function description>
 *
 * Inputs:  e_drv_ch     References.
 *
 * Outputs: - 
 *
 * Returns: ASH_CHANNEL_T
 *
 ----------------------------------------------------------------------------*/
static ASH_CHANNEL_T audio_hdlr_drv_ch_to_ash_ch( AUD_DEC_LS_T e_drv_ch ) 
{
    ASH_CHANNEL_T  e_ash_ch = ASH_CHANNEL_ALL;
    
    if ( e_drv_ch == AUD_DEC_LS_FRONT_LEFT )
    {
        e_ash_ch = ASH_CHANNEL_FRONT_LEFT;        
    }
    else if ( e_drv_ch == AUD_DEC_LS_FRONT_RIGHT )
    {
        e_ash_ch = ASH_CHANNEL_FRONT_RIGHT;
    }
    else if ( e_drv_ch == AUD_DEC_LS_REAR_LEFT )
    {
        e_ash_ch = ASH_CHANNEL_REAR_LEFT;
    }
    else if ( e_drv_ch == AUD_DEC_LS_REAR_RIGHT )
    {
        e_ash_ch = ASH_CHANNEL_REAR_RIGHT;
    }
    else if ( e_drv_ch == AUD_DEC_LS_CENTER )
    {
        e_ash_ch = ASH_CHANNEL_CENTER;
    }
    else if ( e_drv_ch == AUD_DEC_LS_SUB_WOOFER )
    {
        e_ash_ch = ASH_CHANNEL_SUB_WOOFER;
    }
    
    return e_ash_ch;
}
static AUD_DEC_DOWNMIX_MODE_T _audio_hdlr_ash_down_mix_to_drv( ASH_AUD_DOWNMIX_MODE_T  e_down_mix_mode ) 
{
    AUD_DEC_DOWNMIX_MODE_T  e_drv_down_mix_mode = AUD_DEC_DM_OFF;
    
    if ( e_down_mix_mode == ASH_AUD_DOWNMIX_MODE_OFF )
    {
        e_drv_down_mix_mode = AUD_DEC_DM_OFF;        
    }
    else if ( e_down_mix_mode == ASH_AUD_DOWNMIX_MODE_LT_RT )
    {
        e_drv_down_mix_mode = AUD_DEC_DM_LT_RT;
    }
    else if ( e_down_mix_mode == ASH_AUD_DOWNMIX_MODE_STEREO )
    {
        e_drv_down_mix_mode = AUD_DEC_DM_STEREO;
    }
    else if ( e_down_mix_mode == ASH_AUD_DOWNMIX_MODE_VIR_SURR )
    {
        e_drv_down_mix_mode = AUD_DEC_DM_VIR_SURR;
    }
    else if ( e_down_mix_mode == ASH_AUD_DOWNMIX_MODE_MONO )
    {
        e_drv_down_mix_mode = AUD_DEC_DM_MONO;
    }
    else if ( e_down_mix_mode == ASH_AUD_DOWNMIX_MODE_DUAL1 )
    {
        e_drv_down_mix_mode = AUD_DEC_DM_DUAL1;
    }
    else if ( e_down_mix_mode == ASH_AUD_DOWNMIX_MODE_DUAL2 )
    {
        e_drv_down_mix_mode = AUD_DEC_DM_DUAL2;
    }
    else if ( e_down_mix_mode == ASH_AUD_DOWNMIX_MODE_DUAL_MIX )
    {
        e_drv_down_mix_mode = AUD_DEC_DM_DUAL_MIX;
    }
    else if ( e_down_mix_mode == ASH_AUD_DOWNMIX_MODE_3_STEREO )
    {
        e_drv_down_mix_mode = AUD_DEC_DM_3_STEREO;
    }
    
    return e_drv_down_mix_mode;
}

static ASH_AUD_DOWNMIX_MODE_T _audio_hdlr_drv_down_mix_to_ash(  AUD_DEC_DOWNMIX_MODE_T e_drv_down_mix_mode ) 
{
    ASH_AUD_DOWNMIX_MODE_T  e_down_mix_mode = ASH_AUD_DOWNMIX_MODE_OFF;
    
    if ( e_drv_down_mix_mode == AUD_DEC_DM_OFF )
    {
        e_down_mix_mode = ASH_AUD_DOWNMIX_MODE_OFF;        
    }
    else if ( e_drv_down_mix_mode == AUD_DEC_DM_LT_RT )
    {
        e_down_mix_mode = ASH_AUD_DOWNMIX_MODE_LT_RT;
    }
    else if ( e_drv_down_mix_mode == AUD_DEC_DM_STEREO )
    {
        e_down_mix_mode = ASH_AUD_DOWNMIX_MODE_STEREO;
    }
    else if ( e_drv_down_mix_mode == AUD_DEC_DM_VIR_SURR )
    {
        e_down_mix_mode = ASH_AUD_DOWNMIX_MODE_VIR_SURR;
    }
    else if ( e_drv_down_mix_mode == AUD_DEC_DM_MONO )
    {
        e_down_mix_mode = ASH_AUD_DOWNMIX_MODE_MONO;
    }
    else if ( e_drv_down_mix_mode == AUD_DEC_DM_DUAL1 )
    {
        e_down_mix_mode = ASH_AUD_DOWNMIX_MODE_DUAL1;
    }
    else if ( e_drv_down_mix_mode == AUD_DEC_DM_DUAL2 )
    {
        e_down_mix_mode = ASH_AUD_DOWNMIX_MODE_DUAL2;
    }
    else if ( e_drv_down_mix_mode == AUD_DEC_DM_DUAL_MIX )
    {
        e_down_mix_mode = ASH_AUD_DOWNMIX_MODE_DUAL_MIX;
    }
    else if ( e_drv_down_mix_mode == AUD_DEC_DM_3_STEREO )
    {
        e_down_mix_mode = ASH_AUD_DOWNMIX_MODE_3_STEREO;
    }
    
    return e_down_mix_mode;
}

#ifdef ENABLE_MULTIMEDIA        
/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_drv_ash_spd_to_drv_spd
 *
 * Description: <function description>
 *
 * Inputs:  e_ash_speed     References.
 *
 * Outputs: - 
 *
 * Returns: AUD_DEC_DSP_SPEED_MODE_T
 *
 ----------------------------------------------------------------------------*/
static AUD_DEC_DSP_SPEED_MODE_T audio_hdlr_drv_ash_spd_to_drv_spd( MM_SPEED_TYPE_T e_ash_speed ) 
{
    AUD_DEC_DSP_SPEED_MODE_T e_drv_speed;
    if( e_ash_speed  == MM_SPEED_TYPE_FORWARD_1X )        
    {
        e_drv_speed = AUD_DEC_DSP_SPEED_0;
    }    
    if ( e_ash_speed == MM_SPEED_TYPE_FORWARD_2X )
    {
        e_drv_speed = AUD_DEC_DSP_SPEED_2;        
    }
    else if ( e_ash_speed == MM_SPEED_TYPE_FORWARD_4X )
    {
        e_drv_speed = AUD_DEC_DSP_SPEED_4;
    }
    else if ( e_ash_speed == MM_SPEED_TYPE_FORWARD_8X )
    {
        e_drv_speed = AUD_DEC_DSP_SPEED_8;
    }
    else if ( e_ash_speed == MM_SPEED_TYPE_FORWARD_16X )
    {
        e_drv_speed = AUD_DEC_DSP_SPEED_16;
    }
    else if ( e_ash_speed == MM_SPEED_TYPE_FORWARD_32X )
    {
        e_drv_speed = AUD_DEC_DSP_SPEED_32;
    }
    else
    {   /*normal speed by default*/ 
        e_drv_speed = AUD_DEC_DSP_SPEED_0;
    }
    return e_drv_speed;
}

static AUD_DEC_FMT_T audio_hdlr_ash_aud_fmt_2_drv( 
    ASH_AUDIO_FMT_T e_ash_aud_fmt )
{
    switch( e_ash_aud_fmt )
    {
    case ASH_AUDIO_FMT_UNKNOWN:
        return AUD_DEC_FMT_UNKNOWN;

    case ASH_AUDIO_FMT_MPEG:
        return AUD_DEC_FMT_MPEG;

    case ASH_AUDIO_FMT_AC3:
        return AUD_DEC_FMT_AC3;

    case ASH_AUDIO_FMT_PCM:
        return AUD_DEC_FMT_PCM;

    case ASH_AUDIO_FMT_MP3:
        return AUD_DEC_FMT_MP3;

    case ASH_AUDIO_FMT_AAC:
        return AUD_DEC_FMT_AAC;

    case ASH_AUDIO_FMT_DTS:
        return AUD_DEC_FMT_DTS;

    case ASH_AUDIO_FMT_WMA:
        return AUD_DEC_FMT_WMA;

    case ASH_AUDIO_FMT_WMAPRO:
        return AUD_DEC_FMT_WMAPRO;

    case ASH_AUDIO_FMT_RA:
        return AUD_DEC_FMT_RA;

    case ASH_AUDIO_FMT_HDCD:
        return AUD_DEC_FMT_HDCD;

    case ASH_AUDIO_FMT_MLP:
        return AUD_DEC_FMT_MLP;

    case ASH_AUDIO_FMT_MTS:
        return AUD_DEC_FMT_MTS;

    case ASH_AUDIO_FMT_EU_CANAL_PLUS:
        return AUD_DEC_FMT_EU_CANAL_PLUS;

    case ASH_AUDIO_FMT_TV_SYS:
        return AUD_DEC_FMT_TV_SYS;

    case ASH_AUDIO_FMT_COOK:        
        return AUD_DEC_FMT_COOK;

    case ASH_AUDIO_FMT_VORBIS:
        return AUD_DEC_FMT_VORBIS;

    case ASH_AUDIO_FMT_HE_AAC:        
        return AUD_DEC_FMT_HE_AAC;
        
    case ASH_AUDIO_FMT_HE_AAC_V2:        
        return AUD_DEC_FMT_HE_AAC_V2;

    case ASH_AUDIO_FMT_AMR:        
        return AUD_DEC_FMT_AMR;

    case ASH_AUDIO_FMT_AWB:        
        return AUD_DEC_FMT_AWB;

    default:
        return AUD_DEC_FMT_UNKNOWN;
    }
}

/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_drv_ash_codec_info_to_drv_codec_info
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: AUD_DEC_DSP_SPEED_MODE_T
 *
 ----------------------------------------------------------------------------*/
static VOID audio_hdlr_drv_ash_codec_info_to_drv_codec_info( 
    ASH_AUDIO_CODEC_SPEC_INFO_T*  pt_ash_codec_info,
    AUD_DEC_CODEC_SPEC_INFO_T*    pt_drv_codec_info )
{
   if (pt_ash_codec_info->e_fmt == ASH_AUDIO_FMT_WMA ||
       pt_ash_codec_info->e_fmt == ASH_AUDIO_FMT_WMAPRO)
   {
       pt_drv_codec_info->u.t_wma_info.ui2_hdr_src = pt_ash_codec_info->u.t_wma_info.ui2_hdr_src;     
       pt_drv_codec_info->u.t_wma_info.ui4_pkt_sz = pt_ash_codec_info->u.t_wma_info.ui4_block_sz;        
       pt_drv_codec_info->u.t_wma_info.ui2_num_ch = pt_ash_codec_info->u.t_wma_info.ui2_num_ch;        
       pt_drv_codec_info->u.t_wma_info.ui4_sample_per_sec = pt_ash_codec_info->u.t_wma_info.ui4_sample_per_sec; 
       pt_drv_codec_info->u.t_wma_info.ui4_bits_per_sec = pt_ash_codec_info->u.t_wma_info.ui4_bits_per_sec;   
       pt_drv_codec_info->u.t_wma_info.ui4_block_sz = pt_ash_codec_info->u.t_wma_info.ui4_block_sz;       
       pt_drv_codec_info->u.t_wma_info.ui2_encoder_opt = pt_ash_codec_info->u.t_wma_info.ui2_encoder_opt;           
       pt_drv_codec_info->u.t_wma_info.ui4_channel_mask = pt_ash_codec_info->u.t_wma_info.ui4_channel_mask;
       pt_drv_codec_info->u.t_wma_info.ui2_valid_bits   = pt_ash_codec_info->u.t_wma_info.ui2_valid_bits;
       pt_drv_codec_info->u.t_wma_info.ui2_adv_option   = pt_ash_codec_info->u.t_wma_info.ui2_adv_option;
       pt_drv_codec_info->u.t_wma_info.ui4_adv_option2  = pt_ash_codec_info->u.t_wma_info.ui4_adv_option2;
       if (pt_ash_codec_info->e_fmt == ASH_AUDIO_FMT_WMA)
       {
           pt_drv_codec_info->e_fmt = AUD_DEC_FMT_WMA ;
       }
       else if (pt_ash_codec_info->e_fmt == ASH_AUDIO_FMT_WMAPRO)
       {
           pt_drv_codec_info->e_fmt = AUD_DEC_FMT_WMAPRO;
       }
   }else if (pt_ash_codec_info->e_fmt == ASH_AUDIO_FMT_AAC)
   {
       pt_drv_codec_info->u.t_aac_info.ui4_num_ch = pt_ash_codec_info->u.t_aac_info.ui4_num_ch;     
       pt_drv_codec_info->u.t_aac_info.ui4_sample_rate = pt_ash_codec_info->u.t_aac_info.ui4_sample_rate;     
       pt_drv_codec_info->u.t_aac_info.ui4_aac_profile = pt_ash_codec_info->u.t_aac_info.ui4_aac_profile;  
       pt_drv_codec_info->u.t_aac_info.b_frame_length = pt_ash_codec_info->u.t_aac_info.b_frame_length;
       pt_drv_codec_info->e_fmt = AUD_DEC_FMT_AAC ;
   }else if (pt_ash_codec_info->e_fmt == ASH_AUDIO_FMT_PCM)
   {
       pt_drv_codec_info->u.t_pcm_info.ui4_num_ch = pt_ash_codec_info->u.t_pcm_info.ui4_num_ch;     
       pt_drv_codec_info->u.t_pcm_info.ui4_sample_rate = pt_ash_codec_info->u.t_pcm_info.ui4_sample_rate;        
       pt_drv_codec_info->u.t_pcm_info.ui2_block_align = pt_ash_codec_info->u.t_pcm_info.ui2_block_align;
       pt_drv_codec_info->u.t_pcm_info.ui2_bits_per_sample = pt_ash_codec_info->u.t_pcm_info.ui2_bits_per_sample;
       pt_drv_codec_info->u.t_pcm_info.e_pcm_type = (AUD_DEC_PCM_TYPE_T)pt_ash_codec_info->u.t_pcm_info.e_pcm_type; 
       pt_drv_codec_info->u.t_pcm_info.b_big_endian = pt_ash_codec_info->u.t_pcm_info.b_big_endian;
       pt_drv_codec_info->e_fmt = AUD_DEC_FMT_PCM ;
       pt_drv_codec_info->u.t_pcm_info.b_info_not_specific = pt_ash_codec_info->u.t_pcm_info.b_info_not_specific;
       pt_drv_codec_info->u.t_pcm_info.b_signed = pt_ash_codec_info->u.t_pcm_info.b_signed;
   }else if (pt_ash_codec_info->e_fmt == ASH_AUDIO_FMT_COOK)
   {
       pt_drv_codec_info->u.t_cook_info.ui2_codec_frm_sz = pt_ash_codec_info->u.t_cook_info.ui2_codec_frm_sz;
       pt_drv_codec_info->u.t_cook_info.ui2_flavor_idx = pt_ash_codec_info->u.t_cook_info.ui2_flavor_idx;
       pt_drv_codec_info->u.t_cook_info.ui2_intlv_blk_sz = pt_ash_codec_info->u.t_cook_info.ui2_intlv_blk_sz;
       pt_drv_codec_info->u.t_cook_info.ui2_intlv_factor = pt_ash_codec_info->u.t_cook_info.ui2_intlv_factor;
       pt_drv_codec_info->u.t_cook_info.ui2_num_channel = pt_ash_codec_info->u.t_cook_info.ui2_num_channel;
       pt_drv_codec_info->u.t_cook_info.ui4_sample_rate = pt_ash_codec_info->u.t_cook_info.ui4_sample_rate;
       pt_drv_codec_info->u.t_cook_info.pv_codec_opaque_data = pt_ash_codec_info->u.t_cook_info.pv_codec_opaque_data;
       pt_drv_codec_info->e_fmt = AUD_DEC_FMT_COOK;
   }
   else if (pt_ash_codec_info->e_fmt == ASH_AUDIO_FMT_VORBIS)
   {
       pt_drv_codec_info->u.t_vorbis_info.ui4_vorbis_version= pt_ash_codec_info->u.t_vorbis_info.ui4_vorbis_version;
       pt_drv_codec_info->u.t_vorbis_info.ui1_audio_channel = pt_ash_codec_info->u.t_vorbis_info.ui1_audio_channel;
       pt_drv_codec_info->u.t_vorbis_info.ui4_audio_sample_rate = pt_ash_codec_info->u.t_vorbis_info.ui4_audio_sample_rate;
       pt_drv_codec_info->u.t_vorbis_info.ui4_bitrate_maximun= pt_ash_codec_info->u.t_vorbis_info.ui4_bitrate_maximun;
       pt_drv_codec_info->u.t_vorbis_info.ui4_bitrate_norminal = pt_ash_codec_info->u.t_vorbis_info.ui4_bitrate_norminal;
       pt_drv_codec_info->u.t_vorbis_info.ui4_bitrate_minimun = pt_ash_codec_info->u.t_vorbis_info.ui4_bitrate_minimun ;
       pt_drv_codec_info->u.t_vorbis_info.blocksize = pt_ash_codec_info->u.t_vorbis_info.blocksize;
	   pt_drv_codec_info->u.t_vorbis_info.frameflag = pt_ash_codec_info->u.t_vorbis_info.frameflag;
       pt_drv_codec_info->e_fmt = AUD_DEC_FMT_VORBIS;
   }
   else if (pt_ash_codec_info->e_fmt == ASH_AUDIO_FMT_AC3)
   {
       pt_drv_codec_info->u.t_ac3_info.ui1_endian = pt_ash_codec_info->u.t_ac3_info.ui1_endian;
       pt_drv_codec_info->e_fmt = AUD_DEC_FMT_AC3;
   }
   else if (pt_ash_codec_info->e_fmt == ASH_AUDIO_FMT_DTS)
   {
       pt_drv_codec_info->e_fmt = AUD_DEC_FMT_DTS;
   }
   else if (pt_ash_codec_info->e_fmt == ASH_AUDIO_FMT_AMR)
   {
       pt_drv_codec_info->e_fmt = AUD_DEC_FMT_AMR;
   }
   else if (pt_ash_codec_info->e_fmt == ASH_AUDIO_FMT_AWB)
   {
       pt_drv_codec_info->e_fmt = AUD_DEC_FMT_AWB;
   }
   else
   {
       pt_drv_codec_info->e_fmt = audio_hdlr_ash_aud_fmt_2_drv(pt_ash_codec_info->e_fmt);      
   }
}
#endif
/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_sync_mode_to_drv_sync_mode
 *
 * Description: <function description>
 *
 * Inputs:  e_ash_sync_mode     References.
 *
 * Outputs: - 
 *
 * Returns: AUD_DEC_AV_SYNC_INFO_T
 *
 ----------------------------------------------------------------------------*/
static AUD_DEC_AV_SYNC_INFO_T audio_hdlr_sync_mode_to_drv_sync_mode( ASH_AV_SYNC_INFO_T e_ash_sync_mode ) 
{
    AUD_DEC_AV_SYNC_INFO_T  e_drv_sync_mode;
    if( e_ash_sync_mode  == ASH_AV_SYNC_FREE_RUN )        
    {
        e_drv_sync_mode = AUD_DEC_AV_SYNC_FREE_RUN;
    }    
    else if ( e_ash_sync_mode == ASH_AV_SYNC_AUD_SLAVE )
    {
        e_drv_sync_mode = AUD_DEC_AV_SYNC_AUD_SLAVE;        
    }
    else if ( e_ash_sync_mode == ASH_AV_SYNC_AUD_MASTER )
    {
        e_drv_sync_mode = AUD_DEC_AV_SYNC_AUD_MASTER;
    }
    else
    {   /*AUD_DEC_AV_SYNC_AUD_SLAVE by default*/ 
        e_drv_sync_mode = AUD_DEC_AV_SYNC_AUD_SLAVE;
    }
    return e_drv_sync_mode;
}

/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_as_free_fct
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
 * Returns: FALSE               Success.
 *          SMR_INV_HANDLE      Fail.
 *          SMR_INTERNAL_ERROR  Fail.
 *          TRUE                Fail.
 *
 ----------------------------------------------------------------------------*/
static BOOL audio_hdlr_as_free_fct( 
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle)
{
    INT32                i4_ret;
    SM_AUDIO_STREAM_T*   pt_as_obj = (SM_AUDIO_STREAM_T*)pv_obj;
    
    if ( b_req_handle )
    {
        return FALSE;    
    }
    
    x_sema_delete( pt_as_obj->t_base.h_lock );
    pt_as_obj->t_base.h_lock = NULL_HANDLE;
    
    if ( pt_as_obj->t_base.h_scdb != NULL_HANDLE )
    {
        i4_ret = x_scdb_close( pt_as_obj->t_base.h_scdb );
        if ( i4_ret != SCDBR_OK )
        {
            SM_AS_DBG_INFO((
                "[SM] AS: x_scdb_close(0x%.8x) error return %d\r\n",
                (UINT32)pt_as_obj->t_base.h_scdb, i4_ret ));
        }
        pt_as_obj->t_base.h_scdb = NULL_HANDLE;
    }

    if ( pt_as_obj->t_ad_descr.h_comp != NULL_HANDLE )
    {
        i4_ret = sm_drv_comp_close( &pt_as_obj->t_ad_descr );
        if ( i4_ret != SMR_OK )
        {
            SM_AS_DBG_INFO((
                "[SM] AS: sm_drv_comp_close(0x%.8x) error return %d\r\n",
                (UINT32)pt_as_obj->t_base.h_pm_comp, i4_ret ));
        }
        pt_as_obj->t_ad_descr.h_comp = NULL_HANDLE;
    }
    
    if ( pt_as_obj->t_base.h_pm_comp != NULL_HANDLE )
    {
        i4_ret = pm_del_component( pt_as_obj->t_base.h_pm_comp );
        if ( i4_ret != PMR_OK )
        {
            SM_AS_DBG_INFO((
                "[SM] AS: pm_del_component(0x%.8x) error return %d\r\n",
                (UINT32)pt_as_obj->t_base.h_pm_comp, i4_ret ));
        }
        pt_as_obj->t_base.h_pm_comp = NULL_HANDLE;
    }
    
    if ( pt_as_obj->t_state.pv_state_nfy_tag )
    {
        x_mem_free( pt_as_obj->t_state.pv_state_nfy_tag );
    }
    
    x_mem_free( pt_as_obj );
    
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_as_prc_sub_cmd
 *
 * Description: <function description>
 *
 * Inputs:  ppt_cmd_sets References.
 *          pt_comp_desc References.
 *
 * Outputs: - 
 *
 * Returns: SMR_INTERNAL_ERROR  Success.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 audio_hdlr_as_prc_sub_cmd( 
    SM_COMMAND_T**        ppt_cmd_sets,
    SM_DRV_COMP_DESCR_T*  pt_comp_desc )
{
    INT32                 i = 0, i4_ret;
    SM_COMMAND_T*         pt_cmd_sets = *ppt_cmd_sets;
    SM_COMMAND_T*         pt_iter = pt_cmd_sets;

    while ( pt_iter->e_code != SM_CMD_CODE_END )
    {
        if ( (pt_iter->e_code & SM_CMD_CODE_PRIMARY_BIT_FLAG) )
        {
            *ppt_cmd_sets = pt_iter;
            break;
        }
        
        switch ( pt_iter->e_code )
        {
        case SM_AS_CMD_CODE_SET_LOGICAL_DESC:
            if ( x_strlen( pt_iter->u.ps_text) > AS_COMP_NAME_MAX_LEN )
            {
                return SMR_INV_COMMAND;    
            }
            x_strcpy( pt_comp_desc->t_info.s_logical_name, pt_iter->u.ps_text );
            break;
            
        case SM_AS_CMD_CODE_SET_HANDLE:
            pt_comp_desc->h_comp = sm_drv_hdl_clone_get_hdl( 
                                              pt_iter->u.h_obj );
            if ( pt_comp_desc->h_comp == NULL_HANDLE )
            {
                return SMR_INV_COMMAND;
            }
            break;
            
        case SM_AS_CMD_CODE_SET_COMP_ID:
            pt_comp_desc->t_info.ui2_comp_id = pt_iter->u.ui2_data;
            break;
            
        case SM_AS_CMD_CODE_SET_IN_PORT:
            pt_comp_desc->t_info.ui1_in_port = pt_iter->u.ui1_data;
            break;
            
        default:
            return SMR_INV_COMMAND;
        }
        
        pt_iter = &pt_cmd_sets[++i];
    }

    /* over write all other parameters if handle is setted */
    if ( pt_comp_desc->h_comp != NULL_HANDLE )
    {
        i4_ret = sm_drv_comp_get_info( pt_comp_desc );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        
        pt_comp_desc->h_comp = NULL_HANDLE;
    }
    
    *ppt_cmd_sets = pt_iter;
    
    return SMR_OK;    
}

/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_do_msg_do_event
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_as_obj References.
 *          pt_msg    References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID audio_hdlr_do_msg_do_event( 
    HANDLE_T                       h_stream,
    SM_AUDIO_STREAM_T*             pt_as_obj,
    SM_AUDIO_HDLR_MSG_DO_EVENT_T*  pt_msg )
{
    sm_state_on_event( &pt_as_obj->t_state,
                       pt_msg->ui4_event,
                       (VOID*)pt_as_obj,
                       (VOID*)pt_msg->ui4_data,
                       NULL );
}

/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_do_msg_rm_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_as_obj References.
 *          pt_msg    References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID audio_hdlr_do_msg_rm_nfy( 
    HANDLE_T                    h_stream,
    SM_AUDIO_STREAM_T*          pt_as_obj,
    SM_AUDIO_HDLR_MSG_RM_NFY_T* pt_msg )
{
    RM_COND_T            e_cond = RM_COND_IGNORE;
    RM_COND_T            e_old_cond = RM_COND_IGNORE;
    
    /* Filter out all set/get operation notify.  */
    e_cond = rm_cond_chg( pt_msg->e_ad_cond, 
                          RM_COND_IGNORE, 
                          COND_GRP_OPERATION );
    if( e_cond != RM_COND_NO_CHG )
    {   
        #if 0
        SM_AS_DBG_INFO(( 
            "[SM] AS, Filter out RM operation condition(0x%.8x) notify.\r\n",
            e_cond ));
        #endif
        return;
    }
    
    if ( pt_msg->b_should_update_comp_descr &&
         pt_as_obj->t_ad_descr.e_comp_cond != RM_COND_CLOSED )
    {
        e_old_cond = pt_as_obj->t_ad_descr.e_comp_cond;
        pt_as_obj->t_ad_descr.e_comp_cond_org = e_old_cond;
        pt_as_obj->t_ad_descr.e_comp_cond = pt_msg->e_ad_cond;
    }
    else
    {
        e_old_cond = pt_as_obj->t_ad_descr.e_comp_cond_org;
    }
        
    /* check condition group COND_GRP_OBJ_STATE */
    e_cond = rm_cond_chg( pt_msg->e_ad_cond, 
                          e_old_cond, 
                          COND_GRP_OBJ_STATE );
    switch( e_cond )
    {
    case RM_COND_CLOSED:
        sm_state_on_event( &pt_as_obj->t_state,
                           SM_EVN_AS_AD_CLOSED,
                           (VOID*)pt_as_obj,
                           (VOID*)pt_msg->ui4_data,
                           NULL );
        return;
        
    case RM_COND_OPENED:
        sm_state_on_event( &pt_as_obj->t_state,
                           SM_EVN_AS_AD_OPENED,
                           (VOID*)pt_as_obj,
                           (VOID*)pt_msg->ui4_data,
                           NULL );
        
        /* check if audio decoder has opened by other module,
           Vic 2006/12/12 */
        /* check condition group COND_GRP_CONN_STATE */
        e_cond = rm_cond_chg( pt_msg->e_ad_cond, 
                              e_old_cond, 
                              COND_GRP_CONN_STATE );
        if ((RM_COND_CONNECTED == e_cond) ||
            (RM_COND_HARD_WIRED == e_cond))
        {
            sm_state_on_event( &pt_as_obj->t_state,
                               SM_EVN_AS_AD_CONNECTED,
                               (VOID*)pt_as_obj,
                               (VOID*)pt_msg->ui4_data,
                               NULL );
        }
        return;
        
    case RM_COND_NO_CHG:
        break;

    default:
        SM_AS_DBG_ERR(( 
            "[SM] AUD_DEC unhandled RM object state = %u\r\n",
            e_cond ));
        break;
    }

    /* check condition group COND_GRP_CONN_STATE */
    e_cond = rm_cond_chg( pt_msg->e_ad_cond, 
                          e_old_cond, 
                          COND_GRP_CONN_STATE );
    switch( e_cond )
    {
    case RM_COND_DISCONNECTED:
        sm_state_on_event( &pt_as_obj->t_state,
                           SM_EVN_AS_AD_DISCONNECTED,
                           (VOID*)pt_as_obj,
                           (VOID*)pt_msg->ui4_data,
                           NULL );
        return;
        
    case RM_COND_CONNECTED:
    case RM_COND_HARD_WIRED:
        sm_state_on_event( &pt_as_obj->t_state,
                           SM_EVN_AS_AD_CONNECTED,
                           (VOID*)pt_as_obj,
                           (VOID*)pt_msg->ui4_data,
                           NULL );
        return;
    
    case RM_COND_NO_CHG:
        break;
        
    default:
        SM_AS_DBG_ERR((
            "[SM] AUD_DEC unhandled RM connection state = %u\r\n",
            e_cond ));
        break;
    }
}

static ASH_AUDIO_FMT_T audio_hdlr_drv_aud_fmt_2_ash( 
    AUD_DEC_FMT_T e_drv_aud_fmt )
{
    switch( e_drv_aud_fmt )
    {
    case AUD_DEC_FMT_UNKNOWN:
        return ASH_AUDIO_FMT_UNKNOWN;

    case AUD_DEC_FMT_MPEG:
        return ASH_AUDIO_FMT_MPEG;

    case AUD_DEC_FMT_AC3:
        return ASH_AUDIO_FMT_AC3;

    case AUD_DEC_FMT_PCM:
        return ASH_AUDIO_FMT_PCM;

    case AUD_DEC_FMT_MP3:
        return ASH_AUDIO_FMT_MP3;

    case AUD_DEC_FMT_AAC:
        return ASH_AUDIO_FMT_AAC;

    case AUD_DEC_FMT_DTS:
        return ASH_AUDIO_FMT_DTS;

    case AUD_DEC_FMT_WMA:
        return ASH_AUDIO_FMT_WMA;

    case AUD_DEC_FMT_WMAPRO:
        return ASH_AUDIO_FMT_WMAPRO;

    case AUD_DEC_FMT_RA:
        return ASH_AUDIO_FMT_RA;

    case AUD_DEC_FMT_HDCD:
        return ASH_AUDIO_FMT_HDCD;

    case AUD_DEC_FMT_MLP:
        return ASH_AUDIO_FMT_MLP;

    case AUD_DEC_FMT_MTS:
        return ASH_AUDIO_FMT_MTS;

    case AUD_DEC_FMT_EU_CANAL_PLUS:
        return ASH_AUDIO_FMT_EU_CANAL_PLUS;

    case AUD_DEC_FMT_TV_SYS:
        return ASH_AUDIO_FMT_TV_SYS;

    case AUD_DEC_FMT_COOK:        
        return ASH_AUDIO_FMT_COOK;

    case AUD_DEC_FMT_VORBIS:
        return ASH_AUDIO_FMT_VORBIS;

    case AUD_DEC_FMT_HE_AAC:        
        return ASH_AUDIO_FMT_HE_AAC;
        
    case AUD_DEC_FMT_AMR:        
        return ASH_AUDIO_FMT_AMR;
        
    case AUD_DEC_FMT_AWB:        
        return ASH_AUDIO_FMT_AWB;
        
    case AUD_DEC_FMT_HE_AAC_V2:        
        return ASH_AUDIO_FMT_HE_AAC_V2;

    default:
        return ASH_AUDIO_FMT_UNKNOWN;
    }
}

static ASH_CHANNELS_T audio_hdlr_drv_aud_type_2_ash(
    AUD_DEC_AUD_TYPE_T e_drv_aud_type )
{
    switch( e_drv_aud_type )
    {
    case AUD_DEC_TYPE_MONO:
        return ASH_CHANNELS_MONO;

    case AUD_DEC_TYPE_MONO_SUB:
        return ASH_CHANNELS_MONO_SUB;

    case AUD_DEC_TYPE_DUAL_MONO:
        return ASH_CHANNELS_DUAL_MONO;

    case AUD_DEC_TYPE_STEREO:
        return ASH_CHANNELS_STEREO;

    case AUD_DEC_TYPE_STEREO_SUB:
        return ASH_CHANNELS_STEREO_SUB;

    case AUD_DEC_TYPE_STEREO_DOLBY_SURROUND:
        return ASH_CHANNELS_STEREO_DOLBY_SURROUND;

    case AUD_DEC_TYPE_SURROUND_2CH:
        return ASH_CHANNELS_SURROUND_2CH;

    case AUD_DEC_TYPE_SURROUND:
        return ASH_CHANNELS_SURROUND;

    case AUD_DEC_TYPE_3_0:
        return ASH_CHANNELS_3_0;

    case AUD_DEC_TYPE_4_0:
        return ASH_CHANNELS_4_0;

    case AUD_DEC_TYPE_5_1:
        return ASH_CHANNELS_5_1;

    case AUD_DEC_TYPE_7_1:
        return ASH_CHANNELS_7_1;

    case AUD_DEC_TYPE_FM_MONO_NICAM_MONO:
        return ASH_CHANNELS_FM_MONO_NICAM_MONO;

        
    case AUD_DEC_TYPE_FM_MONO_NICAM_STEREO:
        return ASH_CHANNELS_FM_MONO_NICAM_STEREO;

        
    case AUD_DEC_TYPE_FM_MONO_NICAM_DUAL: 
        return ASH_CHANNELS_FM_MONO_NICAM_DUAL;
            
    case AUD_DEC_TYPE_5_0:
        return ASH_CHANNELS_5_0;
        
    case AUD_DEC_TYPE_JOINT_STEREO: 
        return ASH_CHANNELS_STEREO;

    case AUD_DEC_TYPE_OTHERS:
        return ASH_CHANNELS_OTHERS;

    case AUD_DEC_TYPE_FMRDO_MONO:
        return ASH_CHANNELS_FMRDO_MONO;
        
    case AUD_DEC_TYPE_FMRDO_STEREO:
        return ASH_CHANNELS_FMRDO_STEREO;
        
    case AUD_DEC_TYPE_MONO_LFE:
        return ASH_CHANNELS_MONO_LFE;
        
    case AUD_DEC_TYPE_DUAL_MONO_LFE: 
        return ASH_CHANNELS_DUAL_MONO_LFE;
            
    case AUD_DEC_TYPE_STEREO_LFE:
        return ASH_CHANNELS_STEREO_LFE;
        
    case AUD_DEC_TYPE_SURROUND_2CH_LFE: 
        return ASH_CHANNELS_SURROUND_2CH_LFE;
    
    case AUD_DEC_TYPE_SURROUND_LFE:
        return ASH_CHANNELS_SURROUND_LFE;
    
    case AUD_DEC_TYPE_3_1:
        return ASH_CHANNELS_3_1;
        
    case AUD_DEC_TYPE_4_1:
        return ASH_CHANNELS_4_1;
        
        
    default:
        return ASH_CHANNELS_UNKNOWN;
    }
}

static ASH_DECODE_TYPE_T audio_hdlr_drv_aud_decoder_type_2_ash(
    AUD_DEC_DECODE_TYPE_T e_drv_decoder_type )
{
    switch( e_drv_decoder_type )
    {
    case AUD_DEC_DECODE_TYPE_UNKNOWN:
        return ASH_DECODE_TYPE_UNKNOWN;

    case AUD_DEC_DECODE_TYPE_AC3:
        return ASH_DECODE_TYPE_AC3;

    case AUD_DEC_DECODE_TYPE_EAC3:
        return ASH_DECODE_TYPE_EAC3;
       
        
    default:
        return ASH_DECODE_TYPE_UNKNOWN;
    }
}

/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_do_msg_ad_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_as_obj References.
 *          pt_msg    References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID audio_hdlr_do_msg_ad_nfy( 
    HANDLE_T                     h_stream,
    SM_AUDIO_STREAM_T*           pt_as_obj,
    SM_AUDIO_HDLR_MSG_AD_NFY_T*  pt_msg )
{
    switch( pt_msg->e_ad_nfy_cond )
    {
    case AUD_DEC_COND_ERROR:
        SM_AS_DBG_ERR(( "[SM] %s:%d AUD_DEC_COND_ERROR,stream handler(0x%.8X), Audio Decoder control type 0x%.8X!ui4_data_1=%u, ui4_data_2=%u!\r\n",
                        __FILE__, __LINE__,(UINT32)h_stream, pt_as_obj->e_ad_ctrl_type,
                        pt_msg->ui4_data_1, pt_msg->ui4_data_2  ));
        break;
        
    case AUD_DEC_COND_CTRL_DONE:
        /* change state and notify sess handler */
        if ( (AUD_DEC_CTRL_T)pt_msg->ui4_data_1 == AUD_DEC_CTRL_PLAY ||
             (AUD_DEC_CTRL_T)pt_msg->ui4_data_1 == AUD_DEC_CTRL_DETECT_TV_AUD_SYS_N_PLAY ||
             (AUD_DEC_CTRL_T)pt_msg->ui4_data_1 == AUD_DEC_CTRL_PLAY_SYNC )
        {   
            SM_AS_DBG_INFO(( "[SM] %s:%d, audio decoder notify play ctrl done:stream handler(0x%.8X). AUD_DEC_CTRL_T = 0x%.8X\r\n", 
                        __FILE__, __LINE__, (UINT32)h_stream, (UINT32)pt_msg->ui4_data_1  ));
            sm_state_on_event( &pt_as_obj->t_state,
                               SM_EVN_AS_AD_PLAYED,
                               (VOID*)pt_as_obj,
                               (VOID*)pt_msg->ui4_data_2,
                               NULL );
        }
        else if ( (AUD_DEC_CTRL_T)pt_msg->ui4_data_1 == AUD_DEC_CTRL_STOP )
        {   
            x_dbg_stmt( "[SM] %s:%d, audio decoder notify stop ctrl done:stream handler(0x%.8X). AUD_DEC_CTRL_T = 0x%.8X\r\n", 
                        __FILE__, __LINE__, (UINT32)h_stream, (UINT32)pt_msg->ui4_data_1 );
            sm_state_on_event( &pt_as_obj->t_state,
                               SM_EVN_AS_AD_STOPPED,
                               (VOID*)pt_as_obj,
                               (VOID*)pt_msg->ui4_data_2,
                               NULL );
        }
        else if ( (AUD_DEC_CTRL_T)pt_msg->ui4_data_1 == AUD_DEC_CTRL_PAUSE )
        {   
            SM_AS_DBG_INFO(( "[SM] %s:%d, audio decoder notify pause ctrl done:stream handler(0x%.8X). AUD_DEC_CTRL_T = 0x%.8X\r\n", 
                        __FILE__, __LINE__, (UINT32)h_stream, (UINT32)pt_msg->ui4_data_1  ));
            /*for CR326264 do timeshift and unplug cable .mm set pause then plug cable mm set stop and play.
                       so pause and stop make sm audio into error when sm set stop and driver notify pause done */
            #if 0
            sm_state_on_event( &pt_as_obj->t_state,
                               SM_EVN_AS_AD_PAUSED,
                               (VOID*)pt_as_obj,
                               (VOID*)pt_msg->ui4_data_2,
                               NULL );
            #endif
        }
        else if ( (AUD_DEC_CTRL_T)pt_msg->ui4_data_1 == AUD_DEC_CTRL_FLUSH )
        {
            /* to-do, not used currently */
        }
        else if ( (AUD_DEC_CTRL_T)pt_msg->ui4_data_1 == AUD_DEC_CTRL_RESET )
        {
            /* to-do, not used currently */
        }
#ifdef ENABLE_MULTIMEDIA        
        else if ( (AUD_DEC_CTRL_T)pt_msg->ui4_data_1 == AUD_DEC_CTRL_RESUME )
        {
            /*in case of Mutimedia Playback*/
            SM_AS_DBG_INFO(( "[SM] %s:%d, audio decoder notify resume ctrl done:stream handler(0x%.8X). AUD_DEC_CTRL_T = 0x%.8X\r\n", 
                        __FILE__, __LINE__, (UINT32)h_stream, (UINT32)pt_msg->ui4_data_1  ));
        }
#endif
        else 
        {
            SM_AS_DBG_ERR(( "[SM] %s:%d :stream handler(0x%.8X).invalid Audio Decoder control type 0x%.8X!\r\n",
                            __FILE__, __LINE__, (UINT32)h_stream, (AUD_DEC_CTRL_T)pt_msg->ui4_data_1  ));
            /*in release in case other module mistake into sm audio . it will abort  in debug */ 
            #if 0
            SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
            #endif
            SM_ASSERT( 0 );
        }
        break;
        
    case AUD_DEC_COND_MEM_BUFFER_DONE:
        break;
        
    case AUD_DEC_COND_FLUSH_DONE:
        break;
        
    case AUD_DEC_COND_FEED_ME_ON:
        break;
        
    case AUD_DEC_COND_FEED_ME_OFF:
        break;
        
    case AUD_DEC_COND_AUD_INFO_CHG:
        pt_as_obj->t_aud_info.e_aud_fmt = 
            audio_hdlr_drv_aud_fmt_2_ash(pt_msg->u.t_aud_info.e_aud_fmt);
        pt_as_obj->t_aud_info.e_channels =
            audio_hdlr_drv_aud_type_2_ash(pt_msg->u.t_aud_info.e_aud_type);
        pt_as_obj->t_aud_info.ui4_sample_rate = 
            pt_msg->u.t_aud_info.ui4_sample_rate;
        pt_as_obj->t_aud_info.ui4_data_rate =
            pt_msg->u.t_aud_info.ui4_data_rate;
        pt_as_obj->t_aud_info.ui1_bit_depth =
            pt_msg->u.t_aud_info.ui1_bit_depth;
		pt_as_obj->t_aud_info.e_dec_type = audio_hdlr_drv_aud_decoder_type_2_ash(pt_msg->u.t_aud_info.e_dec_type);

        /* Here is temp patch for the behavior that aud drv use 
           AUD_DEC_FMT_AC3 ramcode type to support EAC3
           it should be removed in future.*/ 
	if ( pt_as_obj->t_aud_info.e_aud_fmt == ASH_AUDIO_FMT_AC3 )
	{
		if ( pt_as_obj->t_aud_info.e_dec_type == ASH_DECODE_TYPE_EAC3 )
		{
			pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_enc = AUD_ENC_E_AC3;
			pt_as_obj->t_aud_info.e_aud_fmt = ASH_AUDIO_FMT_E_AC3;
		}
		 else if ( pt_as_obj->t_aud_info.e_dec_type == ASH_DECODE_TYPE_AC3 )
		{
			pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_enc = AUD_ENC_AC3;
			pt_as_obj->t_aud_info.e_aud_fmt = ASH_AUDIO_FMT_AC3;
		}
	}


        SM_AS_DBG_INFO(( "[SM] %s:%d, :stream handler(0x%.8X).Audio FMT change to %d\r\n", 
                        __FILE__, __LINE__, (UINT32)h_stream,(UINT32)pt_msg->e_ad_nfy_cond  ));

        sm_state_on_event( &pt_as_obj->t_state,
                           SM_EVN_AS_AUD_INFO_CHG,
                           (VOID*)pt_as_obj,
                           (VOID*)&pt_as_obj->t_aud_info,
                           NULL );
        break;
        
    case AUD_DEC_COND_TV_AUD_SYS_DETECTED:
        SM_AS_DBG_INFO(( "[SM] %s:%d, audio decoder notify AUD_DEC_COND_TV_AUD_SYS_DETECTED ctrl done:stream handler(0x%.8X). AUD_DEC_CTRL_T = 0x%.8X\r\n", 
                        __FILE__, __LINE__, (UINT32)h_stream, (UINT32)pt_msg->ui4_data_1  ));
        /* TV system detected */
        pt_as_obj->t_aud_sys_info.ui4_tv_sys = (pt_msg->ui4_data_1);
        pt_as_obj->t_aud_sys_info.ui4_aud_sys= (pt_msg->ui4_data_2);
        sm_state_on_event( &pt_as_obj->t_state,
                           SM_EVN_AS_TV_AUD_SYS_DETECTED,
                           (VOID*)pt_as_obj,
                           (VOID*)&pt_as_obj->t_aud_sys_info,
                           NULL );
        break;
        
    case AUD_DEC_COND_AUD_DECODE_STATUS_CHG:
        if(pt_msg->ui4_data_1 == pt_as_obj->e_aud_decode_status)
        {
            break;
        }
        pt_as_obj->e_aud_decode_status =(AUD_DEC_DECODE_STATUS_T)(pt_msg->ui4_data_1);
        sm_state_on_event( &pt_as_obj->t_state,
                           SM_EVN_AS_DECODE_STATUS_CHG,
                           (VOID*)pt_as_obj,
                           (VOID*)pt_msg->ui4_data_1,
                           NULL );
        break;
#ifdef ENABLE_MULTIMEDIA        
    case AUD_DEC_COND_MM_EOS:
        sm_as_send_msg_do_event(
               h_stream,
               SM_EVN_AS_EOS,
               (UINT32)0 );
        break;
#endif
    /*ignore the message which is not belong to audio handler*/
    case AUD_DEC_COND_AUD_CLIP_DONE:
    case AUD_DEC_COND_FM_RADIO_DET_DONE:
    case AUD_DEC_COND_PROTECT_INFO:
    case AUD_DEC_COND_UPLOAD_INFO_CHG:
        break;
    default:
        /* improper Audio Decoder notify */
        SM_AS_DBG_ERR(( "[SM] %s:%d.:stream handler(0x%.8X).Audio Decoder notify 0x%.8X!\r\n", 
                        __FILE__, __LINE__, (UINT32)h_stream, pt_msg->e_ad_nfy_cond  ));
        SM_ASSERT( 0 );
    }
}

/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_do_msg_pm_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_as_obj References.
 *          pt_msg    References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID audio_hdlr_do_msg_pm_nfy( 
    HANDLE_T                     h_stream,
    SM_AUDIO_STREAM_T*           pt_as_obj,
    SM_AUDIO_HDLR_MSG_PM_NFY_T*  pt_msg )
{
    sm_state_on_event( &pt_as_obj->t_state,
                       pt_msg->ui4_pm_event,
                       (VOID*)pt_as_obj,
                       (VOID*)pt_msg->ui4_detail_code,
                       NULL );
}

/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_do_msg_cci_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_as_obj References.
 *          pt_msg    References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID audio_hdlr_do_msg_cci_nfy( 
    HANDLE_T                     h_stream,
    SM_AUDIO_STREAM_T*           pt_as_obj,
    SM_AUDIO_HDLR_MSG_CCI_NFY_T* pt_msg )
{
    sm_state_on_event( &pt_as_obj->t_state,
                       SM_EVN_CCI_INFORMATION,
                       (VOID*)pt_as_obj,
                       (VOID*)&pt_msg->t_pm_cp_info_lst,
                       NULL );
}

/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_do_msg_ca_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_as_obj References.
 *          pt_msg    References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID audio_hdlr_do_msg_ca_nfy( 
    HANDLE_T                     h_stream,
    SM_AUDIO_STREAM_T*           pt_as_obj,
    SM_AUDIO_HDLR_MSG_CA_NFY_T*  pt_msg )
{
    sm_state_on_event( &pt_as_obj->t_state,
                       pt_msg->ui4_ca_event,
                       (VOID*)pt_as_obj,
                       (VOID*)pt_msg->ui4_detail_code,
                       NULL );
}

/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_prc_scdb_rec_modified
 *
 * Description: <function description>
 *
 * Inputs:  pt_as_obj        References.
 *          pt_new_scdb_rec  References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID audio_hdlr_prc_scdb_rec_modified( 
    SM_AUDIO_STREAM_T*             pt_as_obj,
    const SCDB_REC_T*              pt_new_scdb_rec )
{
    BOOL b_should_restart = FALSE;
    SM_AS_STATE_TAG_T     t_state_tag;
    t_state_tag.pt_as_obj = pt_as_obj;
    t_state_tag.h_stream = (HANDLE_T)pt_as_obj->t_base.h_stream;
#ifdef DEBUG
    const CHAR* as_scdb_aud_fmt[] = 
    {
        "AUD_FMT_UNKNOWN", "AUD_FMT_MONO", 
        "AUD_FMT_DUAL_MONO", "AUD_FMT_STEREO",
        "AUD_FMT_DOLBY", "AUD_FMT_TYPE_5_1"
        ,"AUD_FMT_SUBSTEAM"
    };
#endif /* DEBUG */

    /* todo, see what SM could do?? */
    if ( IS_MPEG_AUD_STRM( pt_as_obj ) || IS_MULTIMEDIA_AUD_STRM( pt_as_obj ) )
    {
        if ( pt_new_scdb_rec->u.t_audio_mpeg.ui2_pid != 
             pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.ui2_pid )
        {
            SM_AS_DBG_INFO((
                "[SM] Audio stream(0x%.8X) PID changed to 0x%.4X!\r\n",
                (UINT32)pt_as_obj->t_base.h_stream,
                pt_new_scdb_rec->u.t_audio_mpeg.ui2_pid ));
        }
        if ( pt_new_scdb_rec->u.t_audio_mpeg.e_aud_fmt != 
             pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_aud_fmt )
        {
#ifdef DEBUG
            if ( (UINT32)pt_new_scdb_rec->u.t_audio_mpeg.e_aud_fmt <
                 (sizeof(as_scdb_aud_fmt)/sizeof(CHAR*) - 1) )
            {
                SM_AS_DBG_INFO((
                    "[SM] Audio stream(0x%.8X) format changed to %s!\r\n",
                    (UINT32)pt_as_obj->t_base.h_stream,
                    as_scdb_aud_fmt[(UINT32)pt_new_scdb_rec->u.t_audio_mpeg.e_aud_fmt] ));
            }
            else
            {
                SM_AS_DBG_ERR((
                    "[SM] ERROR Audio stream(0x%.8X) format changed to %d!\r\n",
                    (UINT32)pt_as_obj->t_base.h_stream,
                    (UINT32)pt_new_scdb_rec->u.t_audio_mpeg.e_aud_fmt ));
            }
#endif /* DEBUG */
        }
        if ( pt_new_scdb_rec->u.t_audio_mpeg.e_enc != 
             pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_enc )
        {
            /* sm_as_ad_set_encoding_type( pt_as_obj ); */
            
            AUD_DEC_FMT_INFO_T    t_old_aud_fmt = { AUD_DEC_FMT_AC3, NULL };
            AUD_DEC_FMT_INFO_T    t_new_aud_fmt = { AUD_DEC_FMT_AC3, NULL };
            
            SM_AS_DBG_INFO((
                "[SM] %s:%d, Audio stream(0x%.8X) encoding changed to (%u)  !\r\n",
                __FILE__, __LINE__,
                (UINT32)pt_as_obj->t_base.h_stream,
                (UINT32)pt_new_scdb_rec->u.t_audio_mpeg.e_enc ));
            
            sm_as_aud_fmt_conv_scdb2drv(pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_enc,
                                                             pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_layer,
                                                             &t_old_aud_fmt  );
            
            sm_as_aud_fmt_conv_scdb2drv(pt_new_scdb_rec->u.t_audio_mpeg.e_enc,
                                                             pt_new_scdb_rec->u.t_audio_mpeg.e_layer,
                                                             &t_new_aud_fmt  );
            if( t_old_aud_fmt.e_fmt != t_new_aud_fmt.e_fmt)
            {
                b_should_restart = TRUE;
            }
        }
        if ( pt_new_scdb_rec->u.t_audio_mpeg.e_layer != 
             pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_layer )
        {
            AUD_DEC_FMT_INFO_T    t_old_aud_fmt = { AUD_DEC_FMT_AC3, NULL };
            AUD_DEC_FMT_INFO_T    t_new_aud_fmt = { AUD_DEC_FMT_AC3, NULL };
           sm_as_aud_fmt_conv_scdb2drv(pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_enc,
                                                            pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_layer,
                                                            &t_old_aud_fmt  );
           
           sm_as_aud_fmt_conv_scdb2drv(pt_new_scdb_rec->u.t_audio_mpeg.e_enc,
                                                            pt_new_scdb_rec->u.t_audio_mpeg.e_layer,
                                                            &t_new_aud_fmt  );
            
            SM_AS_DBG_INFO((
                "[SM] %s:%d, Audio stream(0x%.8X) layer changed to (%u)!\r\n",
                __FILE__, __LINE__,
                (UINT32)pt_as_obj->t_base.h_stream,
                (UINT32)pt_new_scdb_rec->u.t_audio_mpeg.e_layer ));
            if( t_old_aud_fmt.e_fmt != t_new_aud_fmt.e_fmt)
            {
                b_should_restart = TRUE;
            }
        }
        if ( x_strcmp( pt_new_scdb_rec->u.t_audio_mpeg.s_lang,
                       pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.s_lang )
             != 0 )
        {
            SM_AS_DBG_INFO((
                "[SM] %s:%d, Audio stream(0x%.8X) language changed to %s!\r\n",
                __FILE__, __LINE__,
                (UINT32)pt_as_obj->t_base.h_stream,
                pt_new_scdb_rec->u.t_audio_mpeg.s_lang ));
        }
        if ( pt_new_scdb_rec->u.t_audio_mpeg.b_default != 
             pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.b_default )
        {
            SM_AS_DBG_INFO((
                "[SM] %s:%d, Audio stream(0x%.8X) %s!\r\n",
                __FILE__, __LINE__,
                (UINT32)pt_as_obj->t_base.h_stream,
                ( (pt_new_scdb_rec->u.t_audio_mpeg.b_default)?
                  "is now DEFAULT": "no longer DEFAULT" ) ));
        }
        if ( b_should_restart )
        {
            //let upper to do action restart
            #if 0
            sm_as_send_msg_do_event(
                pt_as_obj->t_base.h_stream,
                SM_EVN_AS_RESTART,
                (UINT32)pt_as_obj->t_base.i4_speed );
            #endif
                if ( pt_as_obj->t_base.e_condition == SM_COND_STARTED ) 
                {  
                        sm_as_state_notify_app(
                    &t_state_tag,
                    (SM_COND_T)pt_as_obj->t_state.ui4_curr_state,
                    SM_EVN_AS_REQ_RESTART,
                    (UINT32)&pt_as_obj->t_aud_info );
                }
                
        }
    }
    else if ( IS_DIGITAL_AUD_STRM( pt_as_obj ) )
    {
        if ( pt_new_scdb_rec->u.t_audio_digital.e_aud_fmt != 
             pt_as_obj->t_base.t_scdb_rec.u.t_audio_digital.e_aud_fmt )
        {
#ifdef DEBUG
            if ( (UINT32)pt_new_scdb_rec->u.t_audio_digital.e_aud_fmt <
                 (sizeof(as_scdb_aud_fmt)/sizeof(CHAR*) - 1) )
            {
                SM_AS_DBG_INFO((
                    "[SM] %s:%d, Audio stream(0x%.8X) format changed to %s!\r\n",
                    __FILE__, __LINE__,
                    (UINT32)pt_as_obj->t_base.h_stream,
                    as_scdb_aud_fmt[(UINT32)pt_new_scdb_rec->u.t_audio_digital.e_aud_fmt] ));
            }
            else
            {
                SM_AS_DBG_ERR((
                    "[SM] %s:%d, ERROR Audio stream(0x%.8X) format changed to %d!\r\n",
                    __FILE__, __LINE__,
                    (UINT32)pt_as_obj->t_base.h_stream,
                    (UINT32)pt_new_scdb_rec->u.t_audio_digital.e_aud_fmt ));
            }
#endif /* DEBUG */
            b_should_restart = TRUE;
        }
        if ( pt_new_scdb_rec->u.t_audio_digital.e_enc != 
             pt_as_obj->t_base.t_scdb_rec.u.t_audio_digital.e_enc )
        {
            /* sm_as_ad_set_encoding_type( pt_as_obj ); */
            SM_AS_DBG_INFO((
                "[SM] %s:%d, Audio stream(0x%.8X) encoding changed to (%u)!\r\n",
                __FILE__, __LINE__,
                (UINT32)pt_as_obj->t_base.h_stream,
                (UINT32)pt_new_scdb_rec->u.t_audio_digital.e_enc ));
            b_should_restart = TRUE;
        }
        if ( pt_new_scdb_rec->u.t_audio_digital.e_layer != 
             pt_as_obj->t_base.t_scdb_rec.u.t_audio_digital.e_layer )
        {
            SM_AS_DBG_INFO((
                "[SM] %s:%d, Audio stream(0x%.8X) layer changed to (%u)!\r\n",
                __FILE__, __LINE__,
                (UINT32)pt_as_obj->t_base.h_stream,
                (UINT32)pt_new_scdb_rec->u.t_audio_digital.e_layer ));
            b_should_restart = TRUE;
        }

        if ( b_should_restart )
        {   
            #if 0
            sm_as_send_msg_do_event(
                pt_as_obj->t_base.h_stream,
                SM_EVN_AS_RESTART,
                (UINT32)pt_as_obj->t_base.i4_speed );
            #endif
                if ( pt_as_obj->t_base.e_condition == SM_COND_STARTED ) 
                {  
                        sm_as_state_notify_app(
                    &t_state_tag,
                    (SM_COND_T)pt_as_obj->t_state.ui4_curr_state,
                    SM_EVN_AS_REQ_RESTART,
                    (UINT32)&pt_as_obj->t_aud_info );
                }
        }
    }
    else /* Analog audio */
    {
        if ( pt_new_scdb_rec->u.t_audio_mpeg.e_aud_fmt != 
             pt_as_obj->t_base.t_scdb_rec.u.t_audio_analog.e_aud_fmt )
        {
#ifdef DEBUG
            if ( (UINT32)pt_new_scdb_rec->u.t_audio_analog.e_aud_fmt <
                 (sizeof(as_scdb_aud_fmt)/sizeof(CHAR*) - 1) )
            {
                SM_AS_DBG_INFO((
                    "[SM] %s:%d, Audio stream(0x%.8X) format changed to %s!\r\n",
                    __FILE__, __LINE__,
                    (UINT32)pt_as_obj->t_base.h_stream,
                    as_scdb_aud_fmt[(UINT32)pt_new_scdb_rec->u.t_audio_analog.e_aud_fmt] ));
            }
            else
            {
                SM_AS_DBG_ERR((
                    "[SM] %s:%d,ERROR Audio stream(0x%.8X) format changed to %d!\r\n",
                    __FILE__, __LINE__,
                    (UINT32)pt_as_obj->t_base.h_stream,
                    (UINT32)pt_new_scdb_rec->u.t_audio_analog.e_aud_fmt ));
            }
#endif /* DEBUG */
        }

        if ( pt_new_scdb_rec->u.t_audio_analog.b_default != 
             pt_as_obj->t_base.t_scdb_rec.u.t_audio_analog.b_default )
        {
            SM_AS_DBG_INFO((
                "[SM] %s:%d, Audio stream(0x%.8X) %s!\r\n",
                __FILE__, __LINE__,
                (UINT32)pt_as_obj->t_base.h_stream,
                ( (pt_new_scdb_rec->u.t_audio_analog.b_default)?
                  "is now DEFAULT": "no longer DEFAULT" ) ));
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_do_msg_scdb_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_as_obj References.
 *          pt_msg    References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID audio_hdlr_do_msg_scdb_nfy( 
    HANDLE_T                       h_stream,
    SM_AUDIO_STREAM_T*             pt_as_obj,
    SM_AUDIO_HDLR_MSG_SCDB_NFY_T*  pt_msg )
{
    INT32                i4_ret;
    UINT32               ui4_reason = pt_msg->ui4_reason;
    SCDB_COND_T          e_scdb_cond = pt_msg->e_scdb_cond;
    SCDB_REC_T           t_scdb_rec;
    t_scdb_rec.e_rec_type=SCDB_REC_TYPE_UNKNOWN;

    if ( e_scdb_cond == SCDB_UPDATED )
    {
        if ( pt_as_obj->ui2_force_pid != MPEG_2_NULL_PID )
        {
            /* If the stream is in force PID mode, ignore all SCDB update
               notification */
            return;
        }

        i4_ret = x_scdb_get_rec( pt_as_obj->t_base.h_scdb,
                                 &pt_as_obj->t_base.t_strm_comp_id,
                                 &t_scdb_rec );
        if ( i4_ret != SCDBR_OK ||
             ( t_scdb_rec.e_rec_type != pt_as_obj->t_base.t_scdb_rec.e_rec_type ) )
        {
            return;
        }

        if ( ui4_reason & SCDB_RECORD_DEL )
        {
            /* As the record is found, my record is NOT deleted. 
               Must be someone else, just ignore the notify. */
        }
        if ( ui4_reason & SCDB_RECORD_MOD &&
             x_memcmp( &t_scdb_rec, &pt_as_obj->t_base.t_scdb_rec, sizeof(SCDB_REC_T) ) != 0 )
        {
            audio_hdlr_prc_scdb_rec_modified( pt_as_obj, &t_scdb_rec );
        }
        if ( ui4_reason & SCDB_RECORD_ADD )
        {
            
        }
        if ( ui4_reason & SCDB_REASON_UNK )
        {
            
        }
        
        pt_as_obj->t_base.t_scdb_rec = t_scdb_rec;
    }
    else if ( e_scdb_cond == SCDB_UPDATING )
    {
        /* Ignore */
    }
    else if ( e_scdb_cond == SCDB_CLOSED )
    {
        /* Left it to be process by session layer */
        pt_as_obj->t_base.h_scdb = NULL_HANDLE;
    }
    else
    {
        /* improper SCDB state */
        SM_AS_DBG_ERR(( "[SM] %s:%d Improper SCDB state!\r\n", __FILE__, __LINE__  ));
        SM_ASSERT( 0 );
    }
}

/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_as_prc_prm_cmd_open_ad
 *
 * Description: <function description>
 *
 * Inputs:  h_stream     References.
 *          pt_as_obj    References.
 *          ppt_cmd_sets References.
 *
 * Outputs: - 
 *
 * Returns: SMR_INTERNAL_ERROR  Success.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 audio_hdlr_as_prc_prm_cmd_open_ad( 
    HANDLE_T            h_stream,
    SM_AUDIO_STREAM_T*  pt_as_obj,
    SM_COMMAND_T**      ppt_cmd_sets )
{
    INT32                 i = 0, i4_ret;
    SM_COMMAND_T*         pt_cmd_sets = *ppt_cmd_sets;
    SM_COMMAND_T*         pt_iter = pt_cmd_sets;
    
    /* process primary command */
    
    /* process subsequence commands */
    pt_iter = &pt_cmd_sets[++i];

    i4_ret = audio_hdlr_as_prc_sub_cmd( &pt_iter,
                                        &pt_as_obj->t_ad_descr );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }
    *ppt_cmd_sets = pt_iter;
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_exec_cmd
 *
 * Description: <function description>
 *
 * Inputs:  h_stream    References.
 *          pt_as_obj   References.
 *          pt_cmd_sets References.
 *
 * Outputs: - 
 *
 * Returns: SMR_INTERNAL_ERROR  Success.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 audio_hdlr_exec_cmd( 
    HANDLE_T            h_stream,
    SM_AUDIO_STREAM_T*  pt_as_obj,
    const SM_COMMAND_T* pt_cmd_sets )
{
    SM_COMMAND_T* pt_iter = (SM_COMMAND_T*)pt_cmd_sets;
    INT32         i4_ret;
    
    while ( pt_iter->e_code != SM_CMD_CODE_END )
    {
        if ( !(pt_iter->e_code & SM_CMD_CODE_PRIMARY_BIT_FLAG) )
        {
            return SMR_INV_COMMAND;
        }
        
        switch ( pt_iter->e_code )
        {
        case SM_AS_CMD_CODE_SET_AUDIO_DECODER:
            i4_ret = audio_hdlr_as_prc_prm_cmd_open_ad( 
                h_stream, 
                pt_as_obj, 
                &pt_iter );
            break;

        case SM_AS_CMD_CODE_SET_FORCE_PID:
            if ( pt_as_obj->t_base.t_strm_comp_id.e_type != ST_AUDIO )
            {
                return SMR_INV_COMMAND;
            }
            pt_as_obj->ui2_force_pid = pt_iter->u.ui2_data;
            i4_ret = SMR_OK;
            pt_iter = &pt_iter[1];
            break;

        case SM_AS_CMD_CODE_SET_FORCE_SYNC_MODE:
            if ( pt_as_obj->t_base.t_strm_comp_id.e_type != ST_AUDIO )
            {
                return SMR_INV_COMMAND;
            }
            pt_as_obj->e_force_av_sync = audio_hdlr_sync_mode_to_drv_sync_mode((ASH_AV_SYNC_INFO_T)(pt_iter->u.ui1_data));
            i4_ret = SMR_OK;
            pt_iter = &pt_iter[1];
            break;
        case SM_AS_CMD_CODE_SET_FORCE_CODEC:
            {
                if ( pt_as_obj->t_base.t_strm_comp_id.e_type != ST_AUDIO )
                {
                    return SMR_INV_COMMAND;
                }
                pt_as_obj->ui1_force_enc = pt_iter->u.ui1_data;
                i4_ret = SMR_OK;
                pt_iter = &pt_iter[1];
            }
            break;
        case SM_AS_CMD_CODE_SET_AD_MODE:
            if ( pt_as_obj->t_base.t_strm_comp_id.e_type != ST_AUDIO )
            {
                return SMR_INV_COMMAND;
            }
            pt_as_obj->b_ad_mode = pt_iter->u.b_boolean;
            i4_ret = SMR_OK;
            pt_iter = &pt_iter[1];
            break;

            		case SM_AS_CMD_CODE_SET_MS10_MIX_FLAG:
			{
				if ( pt_as_obj->t_base.t_strm_comp_id.e_type != ST_AUDIO )
				{
					return SMR_INV_COMMAND;
				}
				pt_as_obj->b_ms10_mix_flag= pt_iter->u.b_boolean;
				i4_ret = SMR_OK;
				pt_iter = &pt_iter[1];
			}
			break;

		case SM_AS_CMD_CODE_SET_MS10_MIX_ID:
			{
				if ( pt_as_obj->t_base.t_strm_comp_id.e_type != ST_AUDIO )
				{
					return SMR_INV_COMMAND;
				}
				pt_as_obj->ui1_ms10_mix_id= pt_iter->u.ui1_data;
				i4_ret = SMR_OK;
				pt_iter = &pt_iter[1];
			}
			break;
		case SM_AS_CMD_CODE_SET_FORCE_DOWNMIX_MODE:
			{
            	if ( pt_as_obj->t_base.t_strm_comp_id.e_type != ST_AUDIO )
            	{
                	return SMR_INV_COMMAND;
            	}
            	pt_as_obj->e_force_downmix_mode = (AUD_DEC_DOWNMIX_MODE_T)(pt_iter->u.ui1_data);
            	i4_ret = SMR_OK;
            	pt_iter = &pt_iter[1];
			}
            break;

#ifdef SYS_MHP_SUPPORT
         case SM_AS_CMD_CODE_SET_FORCE_NO_CA_REQ:
         {
            if ( pt_as_obj->t_base.t_strm_comp_id.e_type != ST_AUDIO )
            {
                return SMR_INV_COMMAND;
            }
            pt_as_obj->e_ca_req_lvl = PM_CA_REQUIRE_NO;
            i4_ret = SMR_OK;
            pt_iter = &pt_iter[1];
         }
            break;
#endif

        case SM_AS_CMD_CODE_SET_MM_CRYPT_INFO:
            {
                MM_CRYPT_INFO_T* pt_crypt_info = (MM_CRYPT_INFO_T*)pt_iter->u.pv_data;
                
                if ( !pt_crypt_info )
                {
                    return SMR_INV_COMMAND;
                }
                pt_as_obj->t_crypt_info = *pt_crypt_info;
                i4_ret = SMR_OK;
                pt_iter = &pt_iter[1];
            }
            break;

        default:
            return SMR_INV_COMMAND;
        }
        
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;    
        }
    }
    
    return SMR_OK;    
}

#ifndef CI_PLUS_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: _sm_audio_hdlr_set_cci_info
 *
 * Description: <function description>
 *
 * Inputs:  pt_vs_obj       References.
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
static INT32 _sm_audio_hdlr_set_cci_info( 
    SM_AUDIO_STREAM_T*           pt_as_obj,
    CP_INFO_T*                   pt_cp_info )
{
    INT32                    i4_ret;
    AUD_DEC_COPY_PROTECT_T   e_ad_cp_info;
    RM_COND_T                e_drv_cond;
    UINT32                   ui4_out_port = 0;
    SIZE_T                   z_get_size = 0;
    
    if ( pt_cp_info->t_cp_type != CP_TYPE_EMI )
    {
        /* Ignore unknown CP types */
        return SMR_OK;
    }
    
    switch( pt_cp_info->u.e_emi_ctrl )
    {
    case EMI_COPY_NOT_RESTRICTED:
        e_ad_cp_info = AUD_DEC_COPY_PROTECT_NO_RESTRICT;
        break;

    case EMI_NO_FURTHER_COPY:
        e_ad_cp_info = AUD_DEC_COPY_PROTECT_NEVER_COPY;
        break;

    case EMI_ONE_GENERATION_COPY:
        e_ad_cp_info = AUD_DEC_COPY_PROTECT_COPY_ONCE;
        break;

    case EMI_COPY_PROHIBITED:
        /* Turn OFF SPDIF, if it is copy prohibit */
        z_get_size = sizeof( UINT32 );
        i4_ret = rm_get( pt_as_obj->t_ad_descr.h_comp,
                         FALSE,
                         ANY_PORT_NUM,
                         AUD_DEC_GET_TYPE_OUT_PORT,
                         (VOID*)&ui4_out_port,
                         &z_get_size,
                         &e_drv_cond );
        if ( i4_ret != RMR_OK )
        {
            return SMR_DRV_COMP_ERROR;    
        }
        
        ui4_out_port &= (~AUD_DEC_OUT_PORT_FLAG_SPDIF);
        i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
                         FALSE,
                         ANY_PORT_NUM,
                         AUD_DEC_SET_TYPE_OUT_PORT,
                         (VOID*)(UINT32)ui4_out_port,
                         sizeof( UINT32 ),
                         &e_drv_cond );
        if ( i4_ret != RMR_OK )
        {
            return SMR_DRV_COMP_ERROR;    
        }
        return SMR_OK;
        
    default:
        /* Ignore unknown CP types */
        return SMR_INV_SET_INFO;
    }
    
    i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
                     FALSE,
                     ANY_PORT_NUM,
                     AUD_DEC_SET_TYPE_COPY_PROTECT,
                     (VOID*)(UINT32)e_ad_cp_info,
                     sizeof( AUD_DEC_COPY_PROTECT_T ),
                     &e_drv_cond );
    if ( i4_ret != RMR_OK )
    {
        return SMR_DRV_COMP_ERROR;    
    }
    
    return SMR_OK;
}
#else
/*-----------------------------------------------------------------------------
 * Name: _sm_audio_hdlr_set_cci_plus_info
 *
 * Description: <function description>
 *
 * Inputs:  pt_vs_obj       References.
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
static INT32 _sm_audio_hdlr_set_cci_plus_info( 
    SM_AUDIO_STREAM_T*           pt_as_obj,
    CP_INFO_LIST_T*                   pt_cp_info_lst )
{
    INT32                    i4_ret;
    AUD_DEC_COPY_PROTECT_INFO_T   t_ad_cp_info ;
    RM_COND_T                e_drv_cond;
    INT32                        i4_start    = 0;
    INT32                        i4_end      = 0;
    CP_INFO_T*               pt_info     = NULL;
    CP_INFO_T*             pt_rct_tmp = NULL ;
    CP_INFO_T*             pt_emi_tmp = NULL;

    if ( (pt_as_obj == NULL) || (pt_cp_info_lst == NULL) )
    {
        return SMR_INV_ARG;
    }

    i4_end = pt_cp_info_lst->ui1_ctrl_size;
    while(i4_start < i4_end)
    {
        if( i4_start<CP_TYPE_MAX)
        {
            pt_info = pt_cp_info_lst->at_cp_ctrl_list+i4_start;
            
            switch(pt_info->t_cp_type)
            {
                case CP_TYPE_RCT:
                    {
                        pt_rct_tmp = pt_info;
                    }
                    break;
            
                case CP_TYPE_EMI:
                    {
                        pt_emi_tmp = pt_info;
                    }
                    break;
                case CP_TYPE_APS:
                case CP_TYPE_CIT:
                case CP_TYPE_RL:
                case CP_TYPE_UNKNOWN:
                case CP_TYPE_NOTUSED:
                default:
                    break;
            }
        }
        else
        {
            SM_AS_DBG_INFO(( 
                "[SM] AS, ERROR!  ui1_ctrl_size outside the bounds) \r\n" ));
            return SMR_INV_ARG;
        }
        
        i4_start++;
    }
    if( (pt_rct_tmp == NULL ) ||(pt_emi_tmp == NULL) )
    { 
        return SMR_INV_ARG;
    }

    if( pt_rct_tmp->u.e_rct_ctrl == RCT_OFF )
    {
        if(pt_emi_tmp->u.e_emi_ctrl == EMI_COPY_NOT_RESTRICTED )
        {
            t_ad_cp_info.t_protect_info = AUD_DEC_COPY_PROTECT_NO_RESTRICT;
        }
        else if (pt_emi_tmp->u.e_emi_ctrl == EMI_NO_FURTHER_COPY)
        {
            t_ad_cp_info.t_protect_info = AUD_DEC_COPY_NO_FURTHER_COPY;
        }
        else if (pt_emi_tmp->u.e_emi_ctrl == EMI_ONE_GENERATION_COPY)
        {
            t_ad_cp_info.t_protect_info = AUD_DEC_COPY_PROTECT_COPY_ONCE;
        }
        else /*pt_emi_tmp->u.e_emi_ctrl == EMI_COPY_PROHIBITED*/
        {
            t_ad_cp_info.t_protect_info = AUD_DEC_COPY_PROTECT_NEVER_COPY;
        }
    }
    else /*pt_rct_tmp->u.e_rct_ctrl == RCT_ON */ 
    {
        if(pt_emi_tmp->u.e_emi_ctrl == EMI_COPY_NOT_RESTRICTED )
        {
            t_ad_cp_info.t_protect_info = AUD_DEC_COPY_PROTECT_NO_RESTRICT;
        }
    }
    t_ad_cp_info.t_mode = AUD_DEC_CP_MODE_CI;
        
    i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
                     FALSE,
                     ANY_PORT_NUM,
                     AUD_DEC_SET_TYPE_COPY_PROTECT,
                     (VOID*)&t_ad_cp_info,
                     sizeof( AUD_DEC_COPY_PROTECT_INFO_T ),
                     &e_drv_cond );
    if ( i4_ret != RMR_OK )
    {
        return SMR_DRV_COMP_ERROR;    
    }
    
    return SMR_OK;
}
#endif
/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_slctr_set
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
 * Returns: SMR_OK               Success.
 *          SMR_INV_HANDLE       Fail.
 *          SMR_ALREADY_PLAYED   Fail.
 *          SMR_INTERNAL_ERROR   Fail.
 *          SMR_ALREADY_STOPPED  Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 audio_hdlr_slctr_set( 
    HANDLE_T       h_stream,
    SM_SET_TYPE_T  e_set_type,
    VOID*          pv_set_info,
    SIZE_T         z_set_info_len )
{
    INT32                 i4_ret = SMR_OK;
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;
    
    pt_as_obj = (SM_AUDIO_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_as_obj )
    {
        return SMR_INV_HANDLE;
    }
    
    /* change to xxx-ing states */
    if ( e_set_type == SM_STRM_SET_TYPE_PLAY )
    {
        sm_as_send_msg_do_event( 
            h_stream,
            SM_EVN_AS_APP_PLAY_REQ,
            (UINT32)pt_as_obj->t_base.i4_speed );
    }
    else if ( e_set_type == SM_SESS_SET_TYPE_PLAY )
    {
        sm_as_send_msg_do_event( 
            h_stream,
            SM_EVN_SESS_APP_PLAY_REQ,
            (UINT32)pv_set_info );
    }
    else if ( e_set_type == SM_STRM_SET_TYPE_STOP )
    {
        sm_as_send_msg_do_event( 
            h_stream,
            SM_EVN_AS_APP_STOP_REQ,
            0 );
    }
    else if ( e_set_type == SM_SESS_SET_TYPE_STOP )
    {
        sm_as_send_msg_do_event( 
            h_stream,
            SM_EVN_SESS_APP_STOP_REQ,
            0 );
    }
    else if ( e_set_type == SM_STRM_SET_TYPE_CCI_INFO )
    {
       #ifndef CI_PLUS_SUPPORT
        CP_INFO_T*     pt_cp_info = (CP_INFO_T*)pv_set_info;
        
        if ( !pt_cp_info )
        {
            return SMR_INV_SET_INFO;
        }
        if ( z_set_info_len != sizeof( CP_INFO_T ) )
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        
        i4_ret = _sm_audio_hdlr_set_cci_info( pt_as_obj, pt_cp_info );        
        #else
        CP_INFO_LIST_T*     pt_cp_info_lst = (CP_INFO_LIST_T*)pv_set_info;
        
        if ( !pt_cp_info_lst )
        {
            return SMR_INV_SET_INFO;
        }
        if ( z_set_info_len != sizeof( CP_INFO_LIST_T ) )
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        
        i4_ret = _sm_audio_hdlr_set_cci_plus_info( pt_as_obj, pt_cp_info_lst );  
        #endif
    }
    else
    {
        return SMR_NOT_SUPPORT;
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_ash_set
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
 *          SMR_INV_HANDLE      Fail.
 *          SMR_NOT_SUPPORT     Fail.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 audio_hdlr_ash_set( 
    HANDLE_T       h_stream,
    SM_SET_TYPE_T  e_set_type,
    VOID*          pv_set_info,
    SIZE_T         z_set_info_len )
{
    INT32                 i4_ret;
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;
    AUD_DEC_VOLUME_INFO_T t_drv_info;
#ifdef ENABLE_MULTIMEDIA           
    AUD_DEC_CODEC_SPEC_INFO_T t_codec_info ; 
#endif
    HANDLE_T              h_drv;
    RM_COND_T*            pe_drv_cond;
    DRV_SET_TYPE_T        e_drv_set_type;
    VOID*                 pv_drv_set_info;
    SIZE_T                z_drv_set_info_len;
	AUD_DEC_DDC_INFO_T	  t_drv_ddc_info;

    pt_as_obj = (SM_AUDIO_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_as_obj )
    {
        return SMR_INV_HANDLE;
    }

    if ( pt_as_obj->t_base.e_condition != SM_COND_OPENED &&
         pt_as_obj->t_base.e_condition != SM_COND_STARTING &&
         pt_as_obj->t_base.e_condition != SM_COND_STARTED &&
         pt_as_obj->t_base.e_condition != SM_COND_STOPPING )
    {
        return SMR_CAN_NOT_OP_IN_THIS_STATE;    
    }
    
    switch( e_set_type )
    {
    case SM_ASH_SET_TYPE_MUTE:
        {    
            h_drv = pt_as_obj->t_ad_descr.h_comp;
            pe_drv_cond = &pt_as_obj->t_ad_descr.e_comp_cond;
            e_drv_set_type = AUD_DEC_SET_TYPE_MUTE;
            pv_drv_set_info = (VOID*)AUD_DEC_MUTE_ON;
            z_drv_set_info_len = sizeof( AUD_DEC_MUTE_TYPE_T );
        }
        break;
        
    case SM_ASH_SET_TYPE_UNMUTE:
        {    
            h_drv = pt_as_obj->t_ad_descr.h_comp;
            pe_drv_cond = &pt_as_obj->t_ad_descr.e_comp_cond;
            e_drv_set_type = AUD_DEC_SET_TYPE_MUTE;
            pv_drv_set_info = (VOID*)AUD_DEC_MUTE_OFF;
            z_drv_set_info_len = sizeof( AUD_DEC_MUTE_TYPE_T );
        }
        break;
	case SM_ASH_SET_TYPE_MS10_MIX_ID:
		{	 
			t_drv_ddc_info.eIndex = AUD_DEC_DDC_ID;
			t_drv_ddc_info.eAssocId = ash_mix_id_2_drv(*( (ASH_AUD_DEC_DDC_ID_T*)pv_set_info) );
			
			h_drv = pt_as_obj->t_ad_descr.h_comp;
			pe_drv_cond = &pt_as_obj->t_ad_descr.e_comp_cond;
			e_drv_set_type = AUD_DEC_SET_TYPE_DDC_MODE;
			pv_drv_set_info = (VOID*)&t_drv_ddc_info;
			z_drv_set_info_len = sizeof( AUD_DEC_DDC_INFO_T );
		}
		break;
        
    case SM_ASH_SET_TYPE_VOLUME:
        {    
            ASH_VOLUME_INFO_T* pt_ash_info = (ASH_VOLUME_INFO_T*)pv_set_info;
            
            /* The MAX value of volumn is 100 */
            if ( pt_ash_info->ui1_volumn > 100 )
            {
                pt_ash_info->ui1_volumn = 100;
            }
            
            if ( pt_ash_info->e_channel == ASH_CHANNEL_ALL )
            {
                t_drv_info.e_vol_type = AUD_DEC_ALL_CH;
                t_drv_info.u.ui1_level = pt_ash_info->ui1_volumn;
            }
            else
            {
                t_drv_info.e_vol_type = AUD_DEC_INDIVIDUAL_CH;
                /* CAUTION !!! dangerous mapping here, iolo */
                t_drv_info.u.t_ch_vol.e_ls = audio_hdlr_ash_ch_to_drv_ch( pt_ash_info->e_channel );
                t_drv_info.u.t_ch_vol.ui1_level = pt_ash_info->ui1_volumn;
            }
            
            h_drv = pt_as_obj->t_ad_descr.h_comp;
            pe_drv_cond = &pt_as_obj->t_ad_descr.e_comp_cond;
            e_drv_set_type = AUD_DEC_SET_TYPE_VOLUME;
            pv_drv_set_info = (VOID*)&t_drv_info;
            z_drv_set_info_len = sizeof( AUD_DEC_VOLUME_INFO_T );
        }
        break;
#ifdef ENABLE_MULTIMEDIA  
    case SM_ASH_SET_TYPE_AUDIO_ONLY_INFO:
        {    
            h_drv = pt_as_obj->t_ad_descr.h_comp;
            pe_drv_cond = &pt_as_obj->t_ad_descr.e_comp_cond;
            e_drv_set_type = AUD_DEC_SET_TYPE_AUDIO_ONLY_INFO;
            pv_drv_set_info = (VOID*)pv_set_info;
            z_drv_set_info_len = sizeof( BOOL );
        }
        break;

    case SM_ASH_SET_TYPE_SPEED:
        {
            if ( !IS_MPEG_AUD_STRM(pt_as_obj)&&
                 !IS_MULTIMEDIA_AUD_STRM(pt_as_obj))
            {
                return SMR_NOT_ALLOW;
            }
            if ( !pv_set_info )
            {
                return SMR_INV_ARG;    
            }            
            h_drv = pt_as_obj->t_ad_descr.h_comp;
            pe_drv_cond = &pt_as_obj->t_ad_descr.e_comp_cond;
            e_drv_set_type = AUD_DEC_SET_TYPE_DSP_SPEED;
            pv_drv_set_info = (VOID*)pv_set_info;
            pv_drv_set_info =(VOID*)audio_hdlr_drv_ash_spd_to_drv_spd((MM_SPEED_TYPE_T)((UINT32)pv_set_info));
            z_drv_set_info_len = sizeof( AUD_DEC_DSP_SPEED_MODE_T );
        }
        break;                
    case SM_ASH_SET_TYPE_PAUSE:
       {   
           if ( pt_as_obj->t_base.e_condition != SM_COND_STARTED)
           {
              return SMR_CAN_NOT_OP_IN_THIS_STATE;    
           }
           pt_as_obj->e_ad_ctrl_type = AUD_DEC_CTRL_PAUSE;
           h_drv = pt_as_obj->t_ad_descr.h_comp;
           pe_drv_cond = &pt_as_obj->t_ad_descr.e_comp_cond;
           e_drv_set_type = AUD_DEC_SET_TYPE_CTRL;
           pv_drv_set_info = (VOID*)AUD_DEC_CTRL_PAUSE;
           z_drv_set_info_len = sizeof( AUD_DEC_CTRL_T );
       }
    break;    
    case SM_ASH_SET_TYPE_RESUME:
       {
           pt_as_obj->e_ad_ctrl_type = AUD_DEC_CTRL_RESUME;
           h_drv = pt_as_obj->t_ad_descr.h_comp;
           pe_drv_cond = &pt_as_obj->t_ad_descr.e_comp_cond;
           e_drv_set_type = AUD_DEC_SET_TYPE_CTRL;
           pv_drv_set_info = (VOID*)AUD_DEC_CTRL_RESUME;
           z_drv_set_info_len = sizeof( AUD_DEC_CTRL_T );
        }
     break;        
    case SM_ASH_SET_TYPE_CODEC_SPEC_INFO:
        {          
           pv_drv_set_info = (VOID*)&t_codec_info;
           h_drv = pt_as_obj->t_ad_descr.h_comp;
           pe_drv_cond = &pt_as_obj->t_ad_descr.e_comp_cond;
           e_drv_set_type = AUD_DEC_SET_TYPE_CODEC_SPEC_INFO;
           audio_hdlr_drv_ash_codec_info_to_drv_codec_info(
                             (ASH_AUDIO_CODEC_SPEC_INFO_T*)pv_set_info,
                             (AUD_DEC_CODEC_SPEC_INFO_T*)pv_drv_set_info);
           z_drv_set_info_len = sizeof( AUD_DEC_CODEC_SPEC_INFO_T);
           pt_as_obj->t_aud_codec_spec_info.e_fmt =  audio_hdlr_ash_aud_fmt_2_drv(((ASH_AUDIO_CODEC_SPEC_INFO_T*)pv_set_info)->e_fmt);
        }
        break;
#endif   
    case SM_ASH_SET_TYPE_DOWN_MIX_MODE:
        {    
            h_drv = pt_as_obj->t_ad_descr.h_comp;
            pe_drv_cond = &pt_as_obj->t_ad_descr.e_comp_cond;
            e_drv_set_type = AUD_DEC_SET_TYPE_DOWNMIX;
            pv_drv_set_info = (VOID*)(UINT32)_audio_hdlr_ash_down_mix_to_drv((ASH_AUD_DOWNMIX_MODE_T)((UINT32)pv_set_info));
            pt_as_obj->e_force_downmix_mode = _audio_hdlr_ash_down_mix_to_drv((ASH_AUD_DOWNMIX_MODE_T)((UINT32)pv_set_info));
            x_dbg_stmt("\n %s,%d, set e_force_downmix_mode= %d \n",__func__,__LINE__,pt_as_obj->e_force_downmix_mode);
            z_drv_set_info_len = sizeof( AUD_DEC_DOWNMIX_MODE_T );
        }
        break;

    default:
        return SMR_NOT_SUPPORT;
    }

    i4_ret = rm_set( h_drv,
                  FALSE,
                  ANY_PORT_NUM,
                  e_drv_set_type,
                  pv_drv_set_info,
                  z_drv_set_info_len,
                  pe_drv_cond );
    if ( i4_ret != RMR_OK && i4_ret != RMR_ASYNC_NFY)
    {
        return SMR_DRV_COMP_ERROR;    
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_tunl_set
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
 *          SMR_INV_HANDLE      Fail.
 *          SMR_NOT_ALLOW       Fail.
 *          SMR_NOT_SUPPORT     Fail.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 audio_hdlr_tunl_set( 
    HANDLE_T       h_stream,
    SM_SET_TYPE_T  e_set_type,
    VOID*          pv_set_info,
    SIZE_T         z_set_info_len )
{
    INT32                 i4_ret;
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;
    HANDLE_T              h_drv;
    RM_COND_T*            pe_drv_cond;
    
    pt_as_obj = (SM_AUDIO_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_as_obj )
    {
        return SMR_INV_HANDLE;
    }

    if ( pt_as_obj->t_base.e_condition != SM_COND_OPENED &&
         pt_as_obj->t_base.e_condition != SM_COND_STARTING &&
         pt_as_obj->t_base.e_condition != SM_COND_STARTED &&
         pt_as_obj->t_base.e_condition != SM_COND_STOPPING )
    {
        return SMR_CAN_NOT_OP_IN_THIS_STATE;    
    }
        
    switch( e_set_type )
    {
    case SM_GRP_TUNL_AUDIO_DEC:
        if ( SM_GET_CODE( e_set_type ) == AUD_DEC_SET_TYPE_CTRL )
        {
            return SMR_NOT_ALLOW;    
        }
        h_drv = pt_as_obj->t_ad_descr.h_comp;
        pe_drv_cond = &pt_as_obj->t_ad_descr.e_comp_cond;
        break;
        
    default:
        return SMR_NOT_SUPPORT;
    }
    
    i4_ret = rm_set( h_drv,
                  FALSE,
                  ANY_PORT_NUM,
                  (e_set_type & SM_CODE_MASK),
                  pv_set_info,
                  z_set_info_len,
                  pe_drv_cond );
    if ( i4_ret != RMR_OK )
    {
        return SMR_INTERNAL_ERROR;    
    }
    
    return SMR_OK;    
}

/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_slctr_get
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
 *          SMR_NOT_SUPPORT        Fail.
 *          SMR_INV_HANDLE         Fail.
 *          SMR_INSUFFICIENT_SIZE  Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 audio_hdlr_slctr_get( 
    HANDLE_T       h_stream,
    SM_GET_TYPE_T  e_get_type,
    VOID*          pv_get_info,
    SIZE_T*        pz_get_info_len )
{
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;
    
    if ( SM_GET_GRP(e_get_type)==SM_GRP_SESS )
    {
        return SMR_NOT_SUPPORT;    
    }
    
    pt_as_obj = (SM_AUDIO_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_as_obj )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_get_type )
    {
    case SM_STRM_GET_TYPE_STATE:
        *((SM_COND_T*)pv_get_info) = pt_as_obj->t_base.e_condition;
        *pz_get_info_len = sizeof(pt_as_obj->t_base.e_condition);
        break;
        
    case SM_STRM_GET_TYPE_ID:
        if ( *pz_get_info_len < sizeof( STREAM_COMP_ID_T ) )
        {
            return SMR_INSUFFICIENT_SIZE;    
        }
        *((STREAM_COMP_ID_T*)pv_get_info) = pt_as_obj->t_base.t_strm_comp_id;
        *pz_get_info_len = sizeof(STREAM_COMP_ID_T);
        break;

    case SM_STRM_GET_TYPE_SCDB_REC:
        if ( *pz_get_info_len < sizeof( SCDB_REC_T ) )
        {
            return SMR_INSUFFICIENT_SIZE;    
        }
        *((SCDB_REC_T*)pv_get_info) = pt_as_obj->t_base.t_scdb_rec;
        *pz_get_info_len = sizeof(SCDB_REC_T);
        break;
    
    default:
        return SMR_NOT_SUPPORT;
    }

    return SMR_OK;    
}

/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_ash_get
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
 * Returns: SMR_OK              Success.
 *          SMR_NOT_SUPPORT     Fail.
 *          SMR_INV_HANDLE      Fail.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 audio_hdlr_ash_get( 
    HANDLE_T       h_stream,
    SM_GET_TYPE_T  e_get_type,
    VOID*          pv_get_info,
    SIZE_T*        pz_get_info_len )
{
    INT32                 i4_ret;
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;

    HANDLE_T              h_drv;
    RM_COND_T*            pe_drv_cond;
    DRV_GET_TYPE_T        e_drv_get_type;
    VOID*                 pv_drv_get_info;
    SIZE_T                z_drv_get_info_len;

    AUD_DEC_MUTE_TYPE_T   e_mute_drv_info;
    AUD_DEC_VOLUME_INFO_T t_volume_drv_info;
    AUD_DEC_AUD_INFO_T    t_drv_aud_info;
    UINT64                          ui8_audio_pts;
	AUD_DEC_DOWNMIX_MODE_T       e_aud_dec_dm_mode;
        UINT32                ui4_afifo_sz;

    if ( SM_GET_GRP(e_get_type)==SM_GRP_SESS )
    {
        return SMR_NOT_SUPPORT;    
    }
    
    pt_as_obj = (SM_AUDIO_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_as_obj )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_get_type )
    {
    case SM_ASH_GET_TYPE_MUTE:
        {
            h_drv = pt_as_obj->t_ad_descr.h_comp;
            pe_drv_cond = &pt_as_obj->t_ad_descr.e_comp_cond;
            e_drv_get_type = AUD_DEC_GET_TYPE_MUTE;
            pv_drv_get_info = (VOID*)&e_mute_drv_info;
            z_drv_get_info_len = sizeof( AUD_DEC_MUTE_TYPE_T );
        }
        break;
        
    case SM_ASH_GET_TYPE_VOLUME:
        {
            ASH_VOLUME_INFO_T* pt_ash_info = (ASH_VOLUME_INFO_T*)pv_get_info;

            if ( *(pz_get_info_len) < sizeof(ASH_VOLUME_INFO_T) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }

            h_drv = pt_as_obj->t_ad_descr.h_comp;
            pe_drv_cond = &pt_as_obj->t_ad_descr.e_comp_cond;
            e_drv_get_type = AUD_DEC_GET_TYPE_VOLUME;
            
            if ( pt_ash_info->e_channel == ASH_CHANNEL_ALL )
            {
                t_volume_drv_info.e_vol_type = AUD_DEC_ALL_CH;
            }
            else
            {
                t_volume_drv_info.e_vol_type = AUD_DEC_INDIVIDUAL_CH;
                t_volume_drv_info.u.t_ch_vol.e_out_port = AUD_DEC_OUT_PORT_5_1_CH;
                t_volume_drv_info.u.t_ch_vol.e_ls = 
                    audio_hdlr_ash_ch_to_drv_ch(pt_ash_info->e_channel);
            }
            pv_drv_get_info = (VOID*)&t_volume_drv_info;
            z_drv_get_info_len = sizeof( AUD_DEC_VOLUME_INFO_T );
        }
        break;
    
    case SM_ASH_GET_TYPE_AUDIO_DEC:
        {
            if ( *(pz_get_info_len) < sizeof(HANDLE_T) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            
            *(HANDLE_T*)(pv_get_info) = pt_as_obj->t_ad_descr.h_comp_clone;
        }
        return SMR_OK;
        
    case SM_ASH_GET_TYPE_AUDIO_INFO:
        {
            if ( *(pz_get_info_len) < sizeof(ASH_AUDIO_INFO_T) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            
            h_drv = pt_as_obj->t_ad_descr.h_comp;
            pe_drv_cond = &pt_as_obj->t_ad_descr.e_comp_cond;
            e_drv_get_type = AUD_DEC_GET_TYPE_AUD_INFO;
            pv_drv_get_info = (VOID*)&t_drv_aud_info;
            z_drv_get_info_len = sizeof( AUD_DEC_AUD_INFO_T );
        }
        break;
    case SM_ASH_GET_TYPE_AUDIO_PTS:
        {
            h_drv = pt_as_obj->t_ad_descr.h_comp;
            pe_drv_cond = &pt_as_obj->t_ad_descr.e_comp_cond;
            e_drv_get_type = AUD_DEC_GET_TYPE_AUDIO_PTS;
            pv_drv_get_info = (VOID*)&ui8_audio_pts;
            z_drv_get_info_len = sizeof( UINT64 );
        }
        break;
	case SM_ASH_GET_TYPE_DOWN_MIX_MODE:
        {
            h_drv = pt_as_obj->t_ad_descr.h_comp;
            pe_drv_cond = &pt_as_obj->t_ad_descr.e_comp_cond;
            e_drv_get_type = AUD_DEC_GET_TYPE_DOWNMIX;
            pv_drv_get_info = (VOID*)&e_aud_dec_dm_mode;
            z_drv_get_info_len = sizeof( AUD_DEC_DOWNMIX_MODE_T );
        }
        break;

    case SM_ASH_GET_TYPE_AFIFO_AMOUNT:
        {
            if ( *(pz_get_info_len) < sizeof(UINT32) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            
            h_drv = pt_as_obj->t_ad_descr.h_comp;
            pe_drv_cond = &pt_as_obj->t_ad_descr.e_comp_cond;
            e_drv_get_type = AUD_DEC_GET_TYPE_AFIFO_AMOUNT;
            pv_drv_get_info = (VOID*)&ui4_afifo_sz;
            z_drv_get_info_len = sizeof( UINT32 );
        }
        break;

    default:
        return SMR_NOT_SUPPORT;
    }
    
    i4_ret = rm_get( h_drv,
                     FALSE,
                     ANY_PORT_NUM,
                     e_drv_get_type,
                     pv_drv_get_info,
                     &z_drv_get_info_len,
                     pe_drv_cond );
    if ( i4_ret != RMR_OK )
    {
        return SMR_DRV_COMP_ERROR;    
    }

    switch( e_get_type )
    {
    case SM_ASH_GET_TYPE_MUTE:
        {
            AUD_DEC_MUTE_TYPE_T e_mute = *((AUD_DEC_MUTE_TYPE_T*)pv_drv_get_info);
            
            if ( *pz_get_info_len < sizeof(BOOL) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            
            if ( e_mute == AUD_DEC_MUTE_OFF )
            {
                *((BOOL*)pv_get_info) = FALSE;
            }
            else
            {
                *((BOOL*)pv_get_info) = TRUE;
            }
            *pz_get_info_len = sizeof(BOOL);
        }
        break;
        
    case SM_ASH_GET_TYPE_VOLUME:
        {    
            AUD_DEC_VOLUME_INFO_T* pt_drv_vol = pv_drv_get_info;
            ASH_VOLUME_INFO_T*     pt_ash_info = pv_get_info;

            if ( z_drv_get_info_len < sizeof(AUD_DEC_VOLUME_INFO_T) )
            {
                return SMR_INTERNAL_ERROR;
            }
            
            if ( pt_drv_vol->e_vol_type == AUD_DEC_ALL_CH )
            {
                pt_ash_info->e_channel = ASH_CHANNEL_ALL;
                pt_ash_info->ui1_volumn = pt_drv_vol->u.ui1_level;
            }
            else
            {
                pt_ash_info->e_channel = audio_hdlr_drv_ch_to_ash_ch( pt_drv_vol->u.t_ch_vol.e_ls );
                pt_ash_info->ui1_volumn = pt_drv_vol->u.t_ch_vol.ui1_level;
            }
            *pz_get_info_len = sizeof(ASH_VOLUME_INFO_T);
        }
        break;

    case SM_ASH_GET_TYPE_AUDIO_INFO:
        {
            ASH_AUDIO_INFO_T* pt_ash_aud_info = (ASH_AUDIO_INFO_T*)pv_get_info;

            if ( z_drv_get_info_len < sizeof(ASH_AUDIO_INFO_T) )
            {
                return SMR_INTERNAL_ERROR;
            }

            pt_as_obj->t_aud_info.e_aud_fmt = 
                audio_hdlr_drv_aud_fmt_2_ash(t_drv_aud_info.e_aud_fmt);
            pt_as_obj->t_aud_info.e_channels =
                audio_hdlr_drv_aud_type_2_ash(t_drv_aud_info.e_aud_type);
            pt_as_obj->t_aud_info.ui4_sample_rate = 
                t_drv_aud_info.ui4_sample_rate;
            pt_as_obj->t_aud_info.ui4_data_rate =
                t_drv_aud_info.ui4_data_rate;
            pt_as_obj->t_aud_info.ui1_bit_depth =
                t_drv_aud_info.ui1_bit_depth;
            
            /* Here is temp patch for the behavior that aud drv use 
               AUD_DEC_FMT_AC3 ramcode type to support EAC3
               it should be removed in future.*/ 
            if (pt_as_obj->t_aud_info.e_aud_fmt == ASH_AUDIO_FMT_AC3 && 
                pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_enc == AUD_ENC_E_AC3)
            {
                pt_as_obj->t_aud_info.e_aud_fmt = ASH_AUDIO_FMT_E_AC3;
            }
            
            *pt_ash_aud_info = pt_as_obj->t_aud_info;
            *pz_get_info_len = sizeof(ASH_AUDIO_INFO_T);
        }
        break;
    case SM_ASH_GET_TYPE_AUDIO_PTS:
        {
            UINT64  ui8_audio_pts_info = *((UINT64*)pv_drv_get_info);
            
            if ( *pz_get_info_len < sizeof(UINT64) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            
            *((UINT64*)pv_get_info) = ui8_audio_pts_info;
            *pz_get_info_len = sizeof(UINT64);
        }
        break;
	case SM_ASH_GET_TYPE_DOWN_MIX_MODE:
        {    
            ASH_AUD_DOWNMIX_MODE_T e_ash_down_mix_mode = _audio_hdlr_drv_down_mix_to_ash(*((AUD_DEC_DOWNMIX_MODE_T*)pv_drv_get_info));
            *((ASH_AUD_DOWNMIX_MODE_T*)pv_get_info)       =  e_ash_down_mix_mode;
            *pz_get_info_len = sizeof(ASH_AUD_DOWNMIX_MODE_T);
        }
        break;
    case SM_ASH_GET_TYPE_AFIFO_AMOUNT:
        {
            *((UINT64*)pv_get_info) = ui4_afifo_sz;
            *pz_get_info_len = sizeof(UINT32);
        }
        break;            
    default:
        return SMR_NOT_SUPPORT;
    }
    
    return SMR_OK;    
}

/*-----------------------------------------------------------------------------
 * Name: audio_hdlr_tunl_get
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
 * Returns: SMR_OK              Success.
 *          SMR_NOT_SUPPORT     Fail.
 *          SMR_INV_HANDLE      Fail.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 audio_hdlr_tunl_get( 
    HANDLE_T       h_stream,
    SM_GET_TYPE_T  e_get_type,
    VOID*          pv_get_info,
    SIZE_T*        pz_get_info_len )
{
    INT32                 i4_ret;
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;
    HANDLE_T              h_drv;
    RM_COND_T*            pe_drv_cond;
    
    if ( SM_GET_GRP(e_get_type)==SM_GRP_SESS )
    {
        return SMR_NOT_SUPPORT;    
    }
    
    pt_as_obj = (SM_AUDIO_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_as_obj )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_get_type )
    {
    case SM_GRP_TUNL_AUDIO_DEC:
        h_drv = pt_as_obj->t_ad_descr.h_comp;
        pe_drv_cond = &pt_as_obj->t_ad_descr.e_comp_cond;
        break;
        
    default:
        return SMR_INTERNAL_ERROR;
    }

    i4_ret = rm_get( h_drv,
                  FALSE,
                  ANY_PORT_NUM,
                  (e_get_type & SM_CODE_MASK),
                  pv_get_info,
                  pz_get_info_len,
                  pe_drv_cond );
    if ( i4_ret != RMR_OK )
    {
        return SMR_INTERNAL_ERROR;    
    }
    
    return SMR_OK;    
}

/*-----------------------------------------------------------------------------
                    Exported function implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: sm_as_send_msg
 *
 * Description: <function description>
 *
 * Inputs:  e_msg_type References.
 *          h_stream   References.
 *          pv_data    References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sm_as_send_msg( const SM_AUDIO_HDLR_MSG_T*     pt_msg )
{
    INT32                   i4_ret;
    INT32                   i4_cnt = 0;
    
    while( i4_cnt < 20 )
    {
        i4_ret = x_msg_q_send( gh_audio_hdlr_msgq,
                               pt_msg,
                               sizeof( SM_AUDIO_HDLR_MSG_T ),
                               0 ); /* highest priority */
        if ( i4_ret != OSR_OK )
        {
            HANDLE_T    h_thread = NULL_HANDLE;
            
            i4_ret = x_thread_self( &h_thread );
            if ( i4_ret != OSR_OK || !x_handle_valid( h_thread ) )
            {
                break;
            }
            
            ++i4_cnt;
            x_thread_delay( 20 );
            continue;
        }
        
        i4_ret = sm_trigger_task( gh_audio_handler, (VOID*)gh_audio_handler );
        if ( i4_ret != SMR_OK )
        {
            SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
        }
        
        return;
    }
    
    SM_ABORT( SM_DBG_ABRT_CANNOT_SEND_TO_MSG_Q );
}

/*-----------------------------------------------------------------------------
 * Name: sm_as_send_msg_do_event
 *
 * Description: <function description>
 *
 * Inputs:  ui4_event References.
 *          h_stream  References.
 *          ui4_data  References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sm_as_send_msg_do_event( 
    HANDLE_T                 h_stream,
    UINT32                   ui4_event,
    UINT32                   ui4_data )
{
    SM_AUDIO_HDLR_MSG_T            t_msg;
    
    t_msg.e_msg_type = SM_AUDIO_HDLR_MSG_DO_EVENT;
    t_msg.h_stream = h_stream;
    
    t_msg.u.t_msg_do_evn.ui4_event = ui4_event;
    t_msg.u.t_msg_do_evn.ui4_data = ui4_data;
    
    sm_as_send_msg( &t_msg );
}

/*-----------------------------------------------------------------------------
 * Name: sm_audio_hdlr_open_fct
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
 * Returns: SMR_OK                   Success.
 *          SMR_INSUFFICIENT_MEMORY  Fail.
 *          SMR_OUT_OF_HANDLES       Fail.
 *          SMR_INV_HANDLE           Fail.
 *          SMR_INTERNAL_ERROR       Fail.
 *          SMR_INV_ARG              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_audio_hdlr_open_fct( const SM_STREAM_T*  pt_stream_base,
                              const SM_COMMAND_T* pt_cmd_sets,
                              BOOL                b_auto_play,
                              x_stream_nfy_fct    pf_sm_nfy,
                              VOID*               pv_sm_nfy_tag,
                              HANDLE_T*           ph_stream )   /* OUT */
{
    SM_AS_STATE_TAG_T*    pt_as_state_tag = NULL;
    SM_SESSION_T*         pt_sess_obj = NULL;
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;
    HANDLE_T              h_stream = NULL_HANDLE;
    INT32                 i4_ret;
    SIZE_T                z_get_size = sizeof(UINT16);

    if ( !pt_stream_base || !ph_stream || !pf_sm_nfy )
    {
        return SMR_INV_ARG;
    }
    
    pt_sess_obj = (SM_SESSION_T*)sm_get_sess_obj( pt_stream_base->h_sess );
    if ( !pt_sess_obj )
    {
        return SMR_SESS_NOT_EXIST;
    }

    pt_as_state_tag = (SM_AS_STATE_TAG_T*) x_mem_alloc( sizeof(SM_AS_STATE_TAG_T) );
    if ( !pt_as_state_tag )
    {
        return SMR_INSUFFICIENT_MEMORY;
    }
    
    pt_as_obj = (SM_AUDIO_STREAM_T*) x_mem_alloc( sizeof(SM_AUDIO_STREAM_T) );
    if ( !pt_as_obj )
    {
        x_mem_free(pt_as_state_tag);
        return SMR_INSUFFICIENT_MEMORY;
    }
    
    pt_as_obj->t_base = *pt_stream_base;
    pt_as_obj->t_ad_descr = t_sm_drv_comp_initor;
    pt_as_obj->pf_sess_hdlr_sm_nfy = pf_sm_nfy;
    pt_as_obj->pv_sess_hdlr_sm_nfy_tag = pv_sm_nfy_tag;
    pt_as_obj->t_base.h_pm_comp = NULL_HANDLE;
    pt_as_obj->ui4_lastest_ca_detail_code = 0;
    pt_as_obj->ui4_lastest_ca_event = SM_EVN_IGNORE;
    pt_as_obj->ui4_lastest_pm_event = SM_EVN_IGNORE;
    pt_as_obj->ui4_lastest_ad_event = SM_EVN_IGNORE;
    pt_as_obj->e_ad_ctrl_type = AUD_DEC_CTRL_RESET;
    pt_as_obj->ui4_close_cnt = 0;
    pt_as_obj->ui2_force_pid = MPEG_2_NULL_PID;
    pt_as_obj->ui1_force_enc = AUD_ENC_MPEG_2;
#ifdef SYS_MHP_SUPPORT
    pt_as_obj->e_ca_req_lvl = PM_CA_REQUIRE_MANDATORY;
#endif
    pt_as_obj->e_feed_mod = FEED_MODE_UNKNOWN;
    pt_as_obj->e_av_sync = AUD_DEC_AV_SYNC_AUD_SLAVE;
#ifdef SM_ASH_NO_SIG_AUTO_MUTE
    pt_as_obj->i1_sig_err_count = 0;
    pt_as_obj->h_tm_sig_detect = NULL_HANDLE;
#endif
    pt_as_obj->e_force_av_sync = AUD_DEC_AV_SYNC_NULL;
	pt_as_obj->e_force_downmix_mode = AUD_DEC_DM_NULL; 
    pt_as_obj->e_aud_decode_status = AUD_DEC_DECODE_UNKNOWN;
    pt_as_obj->t_aud_info.e_aud_fmt = ASH_AUDIO_FMT_UNKNOWN;
    pt_as_obj->t_aud_info.e_channels = ASH_CHANNELS_UNKNOWN;
	pt_as_obj->t_aud_info.e_dec_type = ASH_DECODE_TYPE_UNKNOWN;
    x_strcpy( pt_as_obj->t_ad_descr.t_info.s_logical_name,
              pt_sess_obj->s_dest_grp_name );

    x_memset( &pt_as_obj->t_crypt_info, 0, sizeof(MM_CRYPT_INFO_T) );
    pt_as_obj->t_crypt_info.u.t_pvr_crypt.e_mode = MM_CRYPT_MODE_INVALID;

    x_memset( &pt_as_obj->t_cp_info_lst, 0, sizeof(CP_INFO_LIST_T) );
    pt_as_obj->b_had_auto_mute = FALSE;

    pt_as_obj->t_enc   = AUD_ENC_UNKNOWN;
    pt_as_obj->t_layer = AUD_LAYER_UNKNOWN;
    pt_as_obj->b_ad_mode = FALSE;
    pt_as_obj->b_ms10_mix_flag = FALSE;
    pt_as_obj->ui1_ms10_mix_id = AUD_DEC_DDC_ID_UNKNOWN;
	pt_as_obj->b_downmix_recover =FALSE;
	pt_as_obj->e_downmix_mode = SCC_AUD_DOWNMIX_MODE_OFF;

    i4_ret = x_sema_create( &pt_as_obj->t_base.h_lock,
                            X_SEMA_TYPE_MUTEX,
                            X_SEMA_STATE_UNLOCK );
    if ( i4_ret != OSR_OK || pt_as_obj->t_base.h_lock == NULL_HANDLE )
    {
        x_mem_free( pt_as_obj );
        x_mem_free(pt_as_state_tag);
        return SMR_INTERNAL_ERROR;   
    }
    
    /* create stream handle */
    i4_ret = handle_alloc( SM_HT_STREAM,
                           pt_as_obj,
                           (VOID*)pv_sm_nfy_tag,
                           &audio_hdlr_as_free_fct,
                           &h_stream );
    if ( i4_ret != HR_OK || h_stream==NULL_HANDLE )
    {
        SM_UNLOCK( pt_as_obj->t_base.h_lock );
        x_sema_delete( pt_as_obj->t_base.h_lock);
        x_mem_free( pt_as_obj );
        x_mem_free(pt_as_state_tag);
        return SMR_OUT_OF_HANDLES;
    }
    pt_as_obj->t_base.h_stream = *ph_stream = h_stream;
    
    pt_as_state_tag->h_stream = h_stream;
    pt_as_state_tag->pt_as_obj = pt_as_obj;
    i4_ret = sm_state_init( at_as_state_cond_lst,
                            at_as_state_descr_lst,
                            at_as_allowed_pending_event_lst,
                            sm_as_state_notify_app,
                            (VOID*)pt_as_state_tag,
                            &pt_as_obj->t_state );
    if ( i4_ret != SMR_OK )
    {
        handle_free( h_stream, FALSE );
        return SMR_INTERNAL_ERROR;   
    }

    SM_LOCK( pt_as_obj->t_base.h_lock );
    
    /* process commands */
    if ( pt_cmd_sets )
    {
        i4_ret = audio_hdlr_exec_cmd( h_stream, pt_as_obj, pt_cmd_sets);
        if ( i4_ret != SMR_OK )
        {
            SM_UNLOCK( pt_as_obj->t_base.h_lock );
            handle_free( h_stream, FALSE );
            return i4_ret;    
        }
    }
    
    /* First get PCR for Audio Decoder */
    i4_ret = x_sm_get( pt_as_obj->t_base.h_sess,
                       SM_SESS_GNRC_GET_TYPE_PCR_ID,
                       &pt_as_obj->t_base.ui2_pcr_id,
                       &z_get_size );
    if ( i4_ret != SMR_OK )
    {
       SM_AS_DBG_INFO(( "[SM] Audio Stream Get PCR ID failed %d",i4_ret ));
    }
#ifdef MW_SUPPORT_2STC
    /*get STC id for Audio Decoder */
    i4_ret = x_sm_get( pt_as_obj->t_base.h_sess,
                       SM_SESS_GNRC_GET_TYPE_STC_ID,
                       &pt_as_obj->t_base.ui2_stc_id,
                       &z_get_size );
    if ( i4_ret != SMR_OK )
    {
       SM_AS_DBG_INFO(( "[SM] Audio Stream Get STC ID failed %d",i4_ret ));
    }
#endif
    /* Get media format from session. */
    z_get_size = sizeof(MEDIA_FORMAT_T);
    i4_ret = x_sm_get( pt_as_obj->t_base.h_sess,
                       SM_SESS_GNRC_GET_TYPE_MEDIA_FMT,
                       &pt_as_obj->t_media_fmt,
                       &z_get_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_DRV_COMP_ERROR;
    }

    sm_as_send_msg_do_event( 
        h_stream,
        SM_EVN_AS_APP_OPEN_REQ,
        0 );

    if ( b_auto_play )
    {
        sm_as_send_msg_do_event(
            h_stream,
            SM_EVN_AS_APP_PLAY_REQ,
            (UINT32)pt_as_obj->t_base.i4_speed );
    }
    
    SM_UNLOCK( pt_as_obj->t_base.h_lock );
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_audio_hdlr_close_fct
 *
 * Description: <function description>
 *
 * Inputs:  h_stream References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INV_HANDLE      Fail.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_audio_hdlr_close_fct( HANDLE_T     h_stream )
{
    INT32                 i4_ret;
    HANDLE_TYPE_T         e_hdlr_type;
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;

    i4_ret = x_handle_get_type( h_stream, &e_hdlr_type );    
    if ( i4_ret != HR_OK || e_hdlr_type != SM_HT_STREAM )
    {
        return SMR_INV_HANDLE;
    }
    
    pt_as_obj = (SM_AUDIO_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_as_obj )
    {
        return SMR_INV_HANDLE;
    }

    if ( pt_as_obj->ui4_close_cnt != 0 )
    {
        return SMR_CAN_NOT_OP_IN_THIS_STATE;
    }
    ++pt_as_obj->ui4_close_cnt;
    
    /* change to closing state */
    sm_as_send_msg_do_event( 
        h_stream,
        SM_EVN_AS_APP_CLOSE_REQ,
        0 );
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_audio_hdlr_set_fct
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
 * Returns: SMR_OK                        Success.
 *          SMR_INV_HANDLE                Fail.
 *          SMR_CAN_NOT_OP_IN_THIS_STATE  Fail.
 *          SMR_INV_ARG                   Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_audio_hdlr_set_fct( HANDLE_T       h_stream,
                             SM_SET_TYPE_T  e_set_type,
                             VOID*          pv_set_info,
                             SIZE_T         z_set_info_len )
{
    INT32                 i4_ret;
    HANDLE_TYPE_T         e_hdlr_type;
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;

    i4_ret = x_handle_get_type( h_stream, &e_hdlr_type );    
    if ( i4_ret != HR_OK || e_hdlr_type != SM_HT_STREAM )
    {
        return SMR_INV_HANDLE;
    }
    
    pt_as_obj = (SM_AUDIO_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_as_obj )
    {
        return SMR_INV_HANDLE;
    }

    if ( pt_as_obj->t_base.e_condition != SM_COND_IGNORE &&
         pt_as_obj->t_base.e_condition != SM_COND_OPENING &&
         pt_as_obj->t_base.e_condition != SM_COND_OPENED &&
         pt_as_obj->t_base.e_condition != SM_COND_STARTING &&
         pt_as_obj->t_base.e_condition != SM_COND_STARTED &&
         pt_as_obj->t_base.e_condition != SM_COND_STOPPING )
    {
        return SMR_CAN_NOT_OP_IN_THIS_STATE;    
    }

    switch ( SM_GET_GRP(e_set_type) )
    {
    case SM_GRP_STRM:
    case SM_GRP_SESS:
        i4_ret = audio_hdlr_slctr_set( h_stream,
                                       e_set_type,
                                       pv_set_info,
                                       z_set_info_len );
        break;
        
    case SM_GRP_ASH:
        i4_ret = audio_hdlr_ash_set( h_stream,
                                     e_set_type,
                                     pv_set_info,
                                     z_set_info_len );
        break;
        
    case SM_GRP_TUNL_AUDIO_DEC:
        i4_ret = audio_hdlr_tunl_set( h_stream,
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
 * Name: sm_audio_hdlr_get_fct
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
 * Returns: SMR_OK                        Success.
 *          SMR_INV_HANDLE                Fail.
 *          SMR_CAN_NOT_OP_IN_THIS_STATE  Fail.
 *          SMR_INV_ARG                   Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_audio_hdlr_get_fct( HANDLE_T       h_stream,
                             SM_GET_TYPE_T  e_get_type,
                             VOID*          pv_get_info,       /* IN/OUT */
                             SIZE_T*        pz_get_info_len )  /* IN/OUT */
{
    INT32                 i4_ret;
    HANDLE_TYPE_T         e_hdlr_type;
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;

    if ( !pv_get_info || !pz_get_info_len )
    {
        return SMR_INV_ARG;    
    }

    i4_ret = x_handle_get_type( h_stream, &e_hdlr_type );    
    if ( i4_ret != HR_OK || e_hdlr_type != SM_HT_STREAM )
    {
        return SMR_INV_HANDLE;
    }
    
    pt_as_obj = (SM_AUDIO_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_as_obj )
    {
        return SMR_INV_HANDLE;
    }

    if ( pt_as_obj->t_base.e_condition != SM_COND_OPENED &&
         pt_as_obj->t_base.e_condition != SM_COND_STARTING &&
         pt_as_obj->t_base.e_condition != SM_COND_STARTED &&
         pt_as_obj->t_base.e_condition != SM_COND_STOPPING )
    {
        return SMR_CAN_NOT_OP_IN_THIS_STATE;    
    }

    switch ( SM_GET_GRP(e_get_type) )
    {
    case SM_GRP_STRM:
    case SM_GRP_SESS:
        i4_ret = audio_hdlr_slctr_get( h_stream,
                                       e_get_type,
                                       pv_get_info,
                                       pz_get_info_len );
        break;
        
    case SM_GRP_ASH:
        i4_ret = audio_hdlr_ash_get( h_stream,
                                     e_get_type,
                                     pv_get_info,
                                     pz_get_info_len );
        break;
        
    case SM_GRP_TUNL_AUDIO_DEC:
        i4_ret = audio_hdlr_tunl_get( h_stream,
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
 * Name: sm_audio_hdlr_task_fct
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
VOID sm_audio_hdlr_task_fct( VOID*  pv_arg )
{
    INT32                      i4_ret = SMR_OK;
    UINT16                     ui2_index;
    SM_AUDIO_STREAM_T*         pt_as_obj = NULL;
    SM_AUDIO_HDLR_MSG_T        t_msg;
    SIZE_T                     z_msg_size = sizeof(SM_AUDIO_HDLR_MSG_T);
    
    i4_ret = x_msg_q_receive( &ui2_index,
                              &t_msg,
                              &z_msg_size,
                              &gh_audio_hdlr_msgq,
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
    
    pt_as_obj = (SM_AUDIO_STREAM_T*)sm_get_strm_obj( t_msg.h_stream );
    if ( !pt_as_obj )
    {
        return;
    }
    
    if ( pt_as_obj->t_base.t_strm_comp_id.e_type != ST_AUDIO )
    {
        return;
    }

    SM_LOCK_RETURN_ON_FAIL( pt_as_obj->t_base.h_lock, );
    
    switch( t_msg.e_msg_type )
    {
    case SM_AUDIO_HDLR_MSG_RM_NFY:
        audio_hdlr_do_msg_rm_nfy( t_msg.h_stream, pt_as_obj, &t_msg.u.t_msg_rm );
        break;
    
    case SM_AUDIO_HDLR_MSG_DO_EVENT:
        audio_hdlr_do_msg_do_event( t_msg.h_stream, pt_as_obj, &t_msg.u.t_msg_do_evn );
        break;

    case SM_AUDIO_HDLR_MSG_AD_NFY:
        audio_hdlr_do_msg_ad_nfy( t_msg.h_stream, pt_as_obj, &t_msg.u.t_msg_ad );
        break;

    case SM_AUDIO_HDLR_MSG_PM_NFY:
        audio_hdlr_do_msg_pm_nfy( t_msg.h_stream, pt_as_obj, &t_msg.u.t_msg_pm );
        break;
        
    case SM_AUDIO_HDLR_MSG_CCI_NFY:
        audio_hdlr_do_msg_cci_nfy( t_msg.h_stream, pt_as_obj, &t_msg.u.t_msg_cci );
        break;

    case SM_AUDIO_HDLR_MSG_CA_NFY:
        audio_hdlr_do_msg_ca_nfy( t_msg.h_stream, pt_as_obj, &t_msg.u.t_msg_ca );
        break;
        
    case SM_AUDIO_HDLR_MSG_SCDB_NFY:
        audio_hdlr_do_msg_scdb_nfy( t_msg.h_stream, pt_as_obj, &t_msg.u.t_msg_scdb );
        break;
        
    default:
        SM_AS_DBG_ERR(( "[SM] %s:%d invalid message type 0x%.8X!\r\n", 
                        __FILE__, __LINE__, t_msg.e_msg_type  ));
        SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
    }
    
    SM_UNLOCK( pt_as_obj->t_base.h_lock )
}

/*-----------------------------------------------------------------------------
 * Name: sm_audio_hdlr_init
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
INT32 sm_audio_hdlr_init( VOID )
{
    INT32               i4_ret;
    SM_HDLR_DESC_T      t_hdlr_desc;
    SM_HDLR_FCT_TBL_T   t_fct_tbl;
    
    x_strcpy( t_hdlr_desc.s_hdlr_name, SM_AUDIO_STRM_HDLR_NAME );
    t_hdlr_desc.ui4_strm_type_mask = ST_MASK_AUDIO;
    
    t_fct_tbl.pf_open = &sm_audio_hdlr_open_fct;
    t_fct_tbl.pf_close = &sm_audio_hdlr_close_fct;
    t_fct_tbl.pf_set = &sm_audio_hdlr_set_fct;
    t_fct_tbl.pf_get = &sm_audio_hdlr_get_fct;
    t_fct_tbl.pf_task = &sm_audio_hdlr_task_fct;

    /* Create message queue for Audio Handler Task */
    if ( gh_audio_hdlr_msgq == NULL_HANDLE )
    {
        i4_ret = x_msg_q_create( &gh_audio_hdlr_msgq,
                                 SM_AUDIO_STRM_HDLR_NAME,  /* use thread name */
                                 sizeof( SM_AUDIO_HDLR_MSG_T ),
                                 SM_AUDIO_HDLR_MSGQ_MAX_SIZE );
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
                                  &gh_audio_handler );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INTERNAL_ERROR;
    }

    return SMR_OK;
}
#ifdef SM_ASH_NO_SIG_AUTO_MUTE
INT32 x_sm_ash_auto_mute_para_init(SM_AS_AUTO_MUTE_PARA_T* pt_auto_mute_pare)
{

    if(pt_auto_mute_pare->i1_error_rate_bound > 100 ||
        pt_auto_mute_pare->i1_error_rate_bound <0  ||
        pt_auto_mute_pare->i1_lost_rate_low_bound > 100 || 
        pt_auto_mute_pare->i1_lost_rate_low_bound < 0 ||
        pt_auto_mute_pare->i1_lost_rate_up_bound > 100 ||
        pt_auto_mute_pare->i1_lost_rate_up_bound < 0 ||
        pt_auto_mute_pare->i1_react_freq <= 0
    )
    {
        return SMR_INTERNAL_ERROR;
    }
    else
    {
         sm_set_as_force_mute_parm(pt_auto_mute_pare->i1_lost_rate_up_bound,
                                    pt_auto_mute_pare->i1_lost_rate_low_bound,
                                    pt_auto_mute_pare->i1_error_rate_bound,
                                    pt_auto_mute_pare->i1_react_freq);
         return SMR_OK;
    }
}
#endif
