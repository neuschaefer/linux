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
 * $RCSfile: sm_video_hdlr.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 5cb9f4f7826f72aa712d5913dfb1ddd4 $
 *
 * Description:
 *         This file implements the Video Stream Handler.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "res_mngr/drv/x_vid_dec.h"
#include "res_mngr/drv/x_vid_plane.h"
#include "res_mngr/drv/x_plane_mxr.h"
#include "res_mngr/drv/x_tv_enc.h"
#include "res_mngr/drv/x_tv_dec.h"
#include "res_mngr/drv/x_avc.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/rm.h"
#include "dbg/x_dbg.h"
#include "dbg/dbg.h"
#include "scdb/x_scdb_api.h"

#include "prc_mngr/u_prc_ca.h"
#include "prc_mngr/prc_mngr.h"

#include "strm_mngr/sm_slctr.h"
#include "strm_mngr/_sm_utils.h"
#include "strm_mngr/sm_drv_map.h"
#include "strm_mngr/sm_state.h"

#include "strm_mngr/sess_gnrc_hdlr/x_sm_sess_gnrc.h"
#include "strm_mngr/video_hdlr/x_sm_video_hdlr.h"
#include "strm_mngr/video_hdlr/sm_video_hdlr.h"

#include "strm_mngr/video_hdlr/_sm_vs_state.h"
#include "strm_mngr/video_hdlr/_sm_video_hdlr.h"
#include "strm_mngr/video_hdlr/_sm_vs_comp_vd.h"
#include "strm_mngr/video_hdlr/_sm_vs_comp_vp.h"
#include "strm_mngr/video_hdlr/_sm_vs_cmd.h"
#include "strm_mngr/video_hdlr/_sm_vs_comp_tvd.h"

#include "strm_mngr/cc_hdlr/u_sm_cc_hdlr.h"

#include "strm_mngr/scc/x_scc.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define SM_VIDEO_HDLR_MSGQ_MAX_SIZE       512
#define SM_VS_RESET_CC_IND_TIMER_DELAY    10000

#define VIDEO_HDLR_CONVERT_TO_DRV_REGION( drv_region, vsh_region )     \
    {                                                                  \
        drv_region.ui4_x = vsh_region.ui4_x;                           \
        drv_region.ui4_y = vsh_region.ui4_y;                           \
        drv_region.ui4_width = vsh_region.ui4_width;                   \
        drv_region.ui4_height = vsh_region.ui4_height;                 \
    }

#define VIDEO_HDLR_CONVERT_TO_VSH_REGION( vsh_region, drv_region )     \
    {                                                                  \
        vsh_region.ui4_x = drv_region.ui4_x;                           \
        vsh_region.ui4_y = drv_region.ui4_y;                           \
        vsh_region.ui4_width = drv_region.ui4_width;                   \
        vsh_region.ui4_height = drv_region.ui4_height;                 \
    }

#define VIDEO_HDLR_CONVERT_TO_DRV_ENHANCE( drv_enhance, vsh_enhance )  \
    {                                                                  \
        if ( vsh_enhance.e_mode == VSH_ENHANCE_MODE_DISABLE )          \
        {                                                              \
            drv_enhance.e_mode = VID_PLA_ENHANCE_DISABLE;              \
        }                                                              \
        else if ( vsh_enhance.e_mode == VSH_ENHANCE_MODE_ENHANCEMENT ) \
        {                                                              \
            drv_enhance.e_mode = VID_PLA_ENHANCE;                      \
        }                                                              \
        else if ( vsh_enhance.e_mode == VSH_ENHANCE_MODE_BLUR )        \
        {                                                              \
            drv_enhance.e_mode = VID_PLA_BLUR;                         \
        }                                                              \
        drv_enhance.ui1_level = vsh_enhance.ui1_level;                 \
    }

#define VIDEO_HDLR_CONVERT_TO_VSH_ENHANCE( vsh_enhance, drv_enhance  ) \
    {                                                                  \
        if ( drv_enhance.e_mode == VID_PLA_ENHANCE_DISABLE )           \
        {                                                              \
            vsh_enhance.e_mode = VSH_ENHANCE_MODE_DISABLE;             \
        }                                                              \
        else if ( drv_enhance.e_mode == VID_PLA_ENHANCE )              \
        {                                                              \
            vsh_enhance.e_mode = VSH_ENHANCE_MODE_ENHANCEMENT;         \
        }                                                              \
        else if ( drv_enhance.e_mode == VID_PLA_BLUR )                 \
        {                                                              \
            vsh_enhance.e_mode = VSH_ENHANCE_MODE_BLUR;                \
        }                                                              \
        vsh_enhance.ui1_level = drv_enhance.ui1_level;                 \
    }


#define SWAP_QV_POS( qv_inp1, qv_inp2 ) \
    {                                   \
        VID_PLA_QV_INP_T    e_temp;     \
        e_temp = qv_inp1;               \
        qv_inp1 = qv_inp2;              \
        qv_inp2 = e_temp;               \
    }

typedef struct _SM_VSH_VBI_PSR_FCT_TBL
{
    SM_VSH_VBI_TYPE_T           e_vbi_type;
    x_sm_vsh_vbi_psr_open_fct   pf_vbi_open;
    x_sm_vsh_vbi_parse_fct      pf_vbi_parse;
    x_sm_vsh_vbi_get_fct        pf_vbi_get;
    x_sm_vsh_vbi_psr_close_fct  pf_vbi_close;
} SM_VSH_VBI_PSR_FCT_TBL;

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
HANDLE_T   gh_video_handler = NULL_HANDLE;
HANDLE_T   gh_video_hdlr_msgq = NULL_HANDLE;

/*
 *  Video Stream State Machine
 */
static SM_STATE_DESCR_T   at_vs_state_descr_lst[] =
{
/*  ui4_state,            b_prcss_pending,    b_allow_pending,  pf_state_do,                pf_state_do_exception */
  { SM_COND_IGNORE,       FALSE,              FALSE,            NULL,                       sm_vs_state_do_ignore_excpt },
  { SM_COND_OPENING,      FALSE,              TRUE,             sm_vs_state_do_opening,     sm_vs_state_do_opening_excpt },
  { SM_COND_OPENED,       TRUE,               FALSE,            sm_vs_state_do_opened,      sm_vs_state_do_opened_excpt },
  { SM_COND_STARTING,     FALSE,              TRUE,             sm_vs_state_do_starting,    sm_vs_state_do_starting_excpt },
  { SM_COND_STARTED,      TRUE,               FALSE,            sm_vs_state_do_started,     sm_vs_state_do_started_excpt },
  { SM_COND_STOPPING,     FALSE,              TRUE,             sm_vs_state_do_stopping,    sm_vs_state_do_stopping_excpt },
  { SM_COND_CLOSING,      FALSE,              TRUE,             sm_vs_state_do_closing,     sm_vs_state_do_closing_excpt },
  { SM_COND_CLOSED,       FALSE,              FALSE,            sm_vs_state_do_closed,      sm_vs_state_do_closed_excpt },
  { SM_COND_ERROR,        TRUE,               FALSE,            sm_vs_state_do_error,       sm_vs_state_do_error_excpt },
  SM_STATE_DESCR_NULL
};

static UINT32             at_vs_allowed_pending_event_lst[] =
{
  SM_EVN_VS_APP_PLAY_REQ,
  SM_EVN_VS_APP_STOP_REQ,
  SM_EVN_VS_APP_CLOSE_REQ,
  SM_EVN_VS_SCDB_RECORD_DEL,
  SM_EVN_SESS_APP_PLAY_REQ,
  SM_EVN_SESS_APP_STOP_REQ,
  SM_EVN_VS_RESTART,
  SM_EVN_IGNORE
};

static SM_STATE_COND_T    at_vs_state_cond_lst[] =
{
  /* ui4_curr_state,   ui4_event,              ui4_next_state,       ui4_pending_event,      pf_event_do */
  /* ignore state conditions *************************************************/
  { SM_COND_IGNORE,   SM_EVN_VS_APP_OPEN_REQ,   SM_COND_OPENING,    SM_EVN_IGNORE,          NULL },

  /* opening state conditions ************************************************/
  { SM_COND_OPENING,  SM_EVN_VS_DRV_OPENED,     SM_COND_OPENED,     SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENING,  SM_EVN_VS_APP_PLAY_REQ,   SM_COND_OPENING,    SM_EVN_VS_APP_PLAY_REQ, NULL },
  { SM_COND_OPENING,  SM_EVN_SESS_APP_PLAY_REQ, SM_COND_OPENING,    SM_EVN_SESS_APP_PLAY_REQ, NULL },
  { SM_COND_OPENING,  SM_EVN_SESS_APP_STOP_REQ, SM_COND_OPENING,    SM_EVN_SESS_APP_STOP_REQ, NULL },    
  { SM_COND_OPENING,  SM_EVN_VS_APP_STOP_REQ,   SM_COND_OPENING,    SM_EVN_VS_APP_STOP_REQ,  NULL },
  { SM_COND_OPENING,  SM_EVN_VS_APP_CLOSE_REQ,  SM_COND_OPENING,    SM_EVN_VS_APP_CLOSE_REQ, NULL },
  { SM_COND_OPENING,  SM_EVN_VS_SCDB_RECORD_DEL,SM_COND_OPENING,    SM_EVN_VS_APP_CLOSE_REQ, NULL },
  { SM_COND_OPENING,  SM_EVN_VS_DRV_PLAYED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENING,  SM_EVN_VS_DRV_STOPPED,    SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENING,  SM_EVN_VS_DRV_CLOSED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENING,  SM_EVN_VS_DRV_ERR,        SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENING,  SM_EVN_VS_DEC_DISCONNECTED,SM_COND_ERROR,     SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENING,  SM_EVN_VS_VP_DISCONNECTED,SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENING,  SM_EVN_VS_PMX_DISCONNECTED,SM_COND_ERROR,     SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENING,  SM_EVN_VS_TVE_DISCONNECTED,SM_COND_ERROR,     SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENING,  SM_EVN_VS_INTERNAL_ERR,   SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },

  /* opened state conditions *************************************************/
  { SM_COND_OPENED,   SM_EVN_VS_APP_PLAY_REQ,   SM_COND_STARTING,   SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENED,   SM_EVN_SESS_APP_PLAY_REQ, SM_COND_STARTING,   SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENED,   SM_EVN_VS_APP_CLOSE_REQ,  SM_COND_CLOSING,    SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENED,   SM_EVN_VS_SCDB_RECORD_DEL,SM_COND_CLOSING,    SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENED,   SM_EVN_VS_DRV_OPENED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENED,   SM_EVN_VS_DRV_PLAYED,     SM_COND_STARTING,   SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENED,   SM_EVN_VS_DRV_STOPPED,    SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENED,   SM_EVN_VS_DRV_CLOSED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENED,   SM_EVN_VS_DRV_ERR,        SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENED,   SM_EVN_VS_DEC_DISCONNECTED,SM_COND_ERROR,     SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENED,   SM_EVN_VS_VP_DISCONNECTED,SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENED,   SM_EVN_VS_PMX_DISCONNECTED,SM_COND_ERROR,     SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENED,   SM_EVN_VS_TVE_DISCONNECTED,SM_COND_ERROR,     SM_EVN_IGNORE,          NULL },
  { SM_COND_OPENED,   SM_EVN_VS_INTERNAL_ERR,   SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },

  /* starting state conditions ***********************************************/
  { SM_COND_STARTING, SM_EVN_VS_DRV_PLAYED,     SM_COND_STARTED,    SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTING, SM_EVN_VS_APP_STOP_REQ,   SM_COND_STARTING,   SM_EVN_VS_APP_STOP_REQ, NULL },
  { SM_COND_STARTING, SM_EVN_SESS_APP_STOP_REQ, SM_COND_STARTING,   SM_EVN_SESS_APP_STOP_REQ, NULL },
  { SM_COND_STARTING, SM_EVN_VS_APP_CLOSE_REQ,  SM_COND_STARTING,   SM_EVN_VS_APP_CLOSE_REQ, NULL },
  { SM_COND_STARTING, SM_EVN_VS_RESTART,        SM_COND_STARTING,   SM_EVN_VS_RESTART,      NULL },
  { SM_COND_STARTING, SM_EVN_VS_SCDB_RECORD_DEL,SM_COND_STARTING,   SM_EVN_VS_APP_CLOSE_REQ, NULL },
  { SM_COND_STARTING, SM_EVN_VS_DRV_OPENED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTING, SM_EVN_VS_DRV_STOPPED,    SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTING, SM_EVN_VS_DRV_CLOSED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTING, SM_EVN_VS_DRV_ERR,        SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTING, SM_EVN_VS_DEC_DISCONNECTED,SM_COND_ERROR,     SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTING, SM_EVN_VS_VP_DISCONNECTED,SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTING, SM_EVN_VS_PMX_DISCONNECTED,SM_COND_ERROR,     SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTING, SM_EVN_VS_TVE_DISCONNECTED,SM_COND_ERROR,     SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTING, SM_EVN_VS_INTERNAL_ERR,   SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },

  /* started state conditions ************************************************/
  { SM_COND_STARTED,  SM_EVN_VS_APP_STOP_REQ,   SM_COND_STOPPING,   SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTED,  SM_EVN_SESS_APP_STOP_REQ, SM_COND_STOPPING,   SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTED,  SM_EVN_VS_APP_CLOSE_REQ,  SM_COND_STOPPING,   SM_EVN_VS_APP_CLOSE_REQ,NULL },
  { SM_COND_STARTED,  SM_EVN_VS_RESTART,        SM_COND_STOPPING,   SM_EVN_VS_APP_PLAY_REQ, NULL },
  { SM_COND_STARTED,  SM_EVN_VS_SCDB_RECORD_DEL,SM_COND_STOPPING,   SM_EVN_VS_APP_CLOSE_REQ,NULL },
  { SM_COND_STARTED,  SM_EVN_VS_DRV_OPENED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTED,  SM_EVN_VS_DRV_PLAYED,     SM_COND_STARTED,    SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTED,  SM_EVN_VS_DRV_STOPPED,    SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTED,  SM_EVN_VS_DRV_CLOSED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTED,  SM_EVN_VS_DRV_ERR,        SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STARTED,  SM_EVN_VS_DEC_DISCONNECTED,SM_COND_STOPPING,  SM_EVN_VS_DEC_DISCONNECTED,NULL },
  { SM_COND_STARTED,  SM_EVN_VS_VP_DISCONNECTED,SM_COND_STOPPING,   SM_EVN_VS_VP_DISCONNECTED,NULL },
  { SM_COND_STARTED,  SM_EVN_VS_PMX_DISCONNECTED,SM_COND_STOPPING,  SM_EVN_VS_PMX_DISCONNECTED,NULL },
  { SM_COND_STARTED,  SM_EVN_VS_TVE_DISCONNECTED,SM_COND_STOPPING,  SM_EVN_VS_TVE_DISCONNECTED,NULL },
  { SM_COND_STARTED,  SM_EVN_VS_INTERNAL_ERR,   SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },

  /* stopping state conditions ***********************************************/
  { SM_COND_STOPPING, SM_EVN_VS_DRV_STOPPED,    SM_COND_OPENED,     SM_EVN_IGNORE,          NULL },
  { SM_COND_STOPPING, SM_EVN_VS_APP_CLOSE_REQ,  SM_COND_STOPPING,   SM_EVN_VS_APP_CLOSE_REQ,NULL },
  { SM_COND_STOPPING, SM_EVN_VS_SCDB_RECORD_DEL,SM_COND_STOPPING,   SM_EVN_VS_APP_CLOSE_REQ,NULL },
  { SM_COND_STOPPING, SM_EVN_VS_APP_PLAY_REQ,   SM_COND_STOPPING,   SM_EVN_VS_APP_PLAY_REQ, NULL },
  { SM_COND_STOPPING, SM_EVN_SESS_APP_PLAY_REQ, SM_COND_STOPPING,   SM_EVN_SESS_APP_PLAY_REQ, NULL },
  { SM_COND_STOPPING, SM_EVN_VS_DRV_OPENED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STOPPING, SM_EVN_VS_DRV_PLAYED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STOPPING, SM_EVN_VS_DRV_CLOSED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STOPPING, SM_EVN_VS_DRV_ERR,        SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_STOPPING, SM_EVN_VS_DEC_DISCONNECTED,SM_COND_STOPPING,  SM_EVN_VS_APP_CLOSE_REQ,NULL },
  { SM_COND_STOPPING, SM_EVN_VS_VP_DISCONNECTED,SM_COND_STOPPING,   SM_EVN_VS_APP_CLOSE_REQ,NULL },
  { SM_COND_STOPPING, SM_EVN_VS_PMX_DISCONNECTED,SM_COND_STOPPING,  SM_EVN_VS_APP_CLOSE_REQ,NULL },
  { SM_COND_STOPPING, SM_EVN_VS_TVE_DISCONNECTED,SM_COND_STOPPING,  SM_EVN_VS_APP_CLOSE_REQ,NULL },
  { SM_COND_STOPPING, SM_EVN_VS_INTERNAL_ERR,   SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },

  /* closing state conditions ************************************************/
  { SM_COND_CLOSING,  SM_EVN_VS_DRV_CLOSED,     SM_COND_CLOSED,     SM_EVN_IGNORE,          NULL },
  { SM_COND_CLOSING,  SM_EVN_VS_DRV_ERR,        SM_COND_CLOSING,    SM_EVN_IGNORE,          NULL },
  { SM_COND_CLOSING,  SM_EVN_VS_INTERNAL_ERR,   SM_COND_CLOSING,    SM_EVN_IGNORE,          NULL },

  /* error state conditions ************************************************/
  { SM_COND_ERROR,    SM_EVN_VS_APP_CLOSE_REQ,  SM_COND_CLOSING,    SM_EVN_IGNORE,          NULL },
  { SM_COND_ERROR,    SM_EVN_VS_SCDB_RECORD_DEL,SM_COND_CLOSING,    SM_EVN_IGNORE,          NULL },
  { SM_COND_ERROR,    SM_EVN_VS_APP_PLAY_REQ,   SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_ERROR,    SM_EVN_SESS_APP_PLAY_REQ, SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_ERROR,    SM_EVN_VS_APP_STOP_REQ,   SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_ERROR,    SM_EVN_SESS_APP_STOP_REQ, SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_ERROR,    SM_EVN_VS_DRV_OPENED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_ERROR,    SM_EVN_VS_DRV_OPENED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_ERROR,    SM_EVN_VS_DRV_PLAYED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_ERROR,    SM_EVN_VS_DRV_STOPPED,    SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },
  { SM_COND_ERROR,    SM_EVN_VS_DRV_CLOSED,     SM_COND_ERROR,      SM_EVN_IGNORE,          NULL },

  /* ANY state events ********************************************************/
  { SM_STATE_IGNORE,  SM_EVN_CCI_INFORMATION,   SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_cci_events },
  { SM_STATE_IGNORE,  SM_EVN_CA_COND_CLOSED,    SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_ca_events },
  { SM_STATE_IGNORE,  SM_EVN_CA_COND_UNKNOWN,   SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_ca_events },
  { SM_STATE_IGNORE,  SM_EVN_CA_COND_FREE,      SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_ca_events },
  { SM_STATE_IGNORE,  SM_EVN_CA_COND_AUTHORIZED,SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_ca_events },
  { SM_STATE_IGNORE,  SM_EVN_CA_COND_AUTHORIZED_CA_SYS_ID, SM_STATE_IGNORE, SM_EVN_IGNORE,  sm_vs_state_do_ca_events },
  { SM_STATE_IGNORE,  SM_EVN_CA_COND_NOT_AUTHORIZED,SM_STATE_IGNORE,SM_EVN_IGNORE,          sm_vs_state_do_ca_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_DEC_OPENED,      SM_STATE_IGNORE,   SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_DEC_CLOSED,      SM_STATE_IGNORE,   SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_DEC_CONNECTED,   SM_STATE_IGNORE,   SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_DEC_DISCONNECTED,SM_STATE_IGNORE,   SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_DEC_PLAYED,      SM_STATE_IGNORE,   SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_DEC_STOPPED,     SM_STATE_IGNORE,   SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_VP_OPENED,      SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_VP_CLOSED,      SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_VP_CONNECTED,   SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_VP_DISCONNECTED,SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_VP_PLAYED,      SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_VP_STOPPED,     SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_PMX_OPENED,     SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_PMX_CLOSED,     SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_PMX_CONNECTED,  SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_PMX_DISCONNECTED,SM_STATE_IGNORE,   SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_PMX_PLAYED,     SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_PMX_STOPPED,    SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_TVE_OPENED,     SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_TVE_CLOSED,     SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_TVE_CONNECTED,  SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_TVE_DISCONNECTED,SM_STATE_IGNORE,   SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_TVE_PLAYED,     SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_TVE_STOPPED,    SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_PM_COND_CLOSED, SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_PM_COND_PLAYED, SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_PM_COND_STOPPED,SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_drv_events },
  { SM_STATE_IGNORE,  SM_EVN_VS_VIDEO_RESOLUTION_CHG, SM_STATE_IGNORE, SM_EVN_IGNORE,       sm_vs_state_do_res_chg },
  { SM_STATE_IGNORE,  SM_EVN_VS_AUTO_CLK_POS_PHS_DONE, SM_STATE_IGNORE, SM_EVN_IGNORE,      sm_vs_state_do_auto_done },
  { SM_STATE_IGNORE,  SM_EVN_VS_AUTO_COLOR_DONE, SM_STATE_IGNORE,   SM_EVN_IGNORE,          sm_vs_state_do_auto_done },
  { SM_STATE_IGNORE,  SM_EVN_VS_ANA_TV_SIGNAL,  SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_tv_sig },
  { SM_STATE_IGNORE,  SM_EVN_VS_ANA_COLOR_SYSTEM,SM_STATE_IGNORE,   SM_EVN_IGNORE,          sm_vs_state_do_color_sys },
  { SM_STATE_IGNORE,  SM_EVN_VS_ANA_CONTENT_ADV_INFO,SM_STATE_IGNORE, SM_EVN_IGNORE,        sm_vs_state_do_cnt_adv },
  { SM_STATE_IGNORE,  SM_EVN_VS_CC_INDICATOR,   SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_cc_ind },
  { SM_STATE_IGNORE,  SM_EVN_VS_ANA_TV_SIGNAL_DETECTING, SM_STATE_IGNORE, SM_EVN_IGNORE,    sm_vs_state_do_tv_sig },
  { SM_STATE_IGNORE,  SM_EVN_VS_DIG_DECODE_STATUS_CHG, SM_STATE_IGNORE, SM_EVN_IGNORE,      sm_vs_state_do_dec_status },
  { SM_STATE_IGNORE,  SM_EVN_VS_SPEED_CHG,      SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_event_nfy },
  { SM_STATE_IGNORE,  SM_EVN_VS_EOS,            SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_event_nfy },
#ifdef DIVX_PLUS_CER 
  { SM_STATE_IGNORE,  SM_EVN_VS_CHAP_DONE,      SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_event_nfy }, 
#endif
  { SM_STATE_IGNORE,  SM_EVN_VS_CODEC_NOT_SUPPORT, SM_STATE_IGNORE, SM_EVN_IGNORE,          sm_vs_state_do_event_nfy },
  { SM_STATE_IGNORE,  SM_EVN_VS_THUMBNAIL_DONE, SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_event_nfy },
  { SM_STATE_IGNORE,  SM_EVN_VS_HDCP_CONTENT,   SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_event_nfy },
  { SM_STATE_IGNORE,  SM_EVN_VS_MACROVISION,    SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_event_nfy },
  { SM_STATE_IGNORE,  SM_EVN_RANGE_AB_DONE,     SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_event_nfy },
  { SM_STATE_IGNORE,  SM_EVN_VS_DISP_FRAME,     SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_event_nfy },
  { SM_STATE_IGNORE,  SM_EVN_VS_BIT_RATE,       SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_event_nfy },
  { SM_STATE_IGNORE,  SM_EVN_VS_VIDEO_PLAY_DONE,SM_STATE_IGNORE,    SM_EVN_IGNORE,          sm_vs_state_do_event_nfy },
  { SM_STATE_IGNORE,  SM_EVN_VS_FRAMERATE_NOT_SUPPORT,SM_STATE_IGNORE,SM_EVN_IGNORE,        sm_vs_state_do_event_nfy },
  { SM_STATE_IGNORE,  SM_EVN_VS_AUD_PROTECT_INFO,SM_STATE_IGNORE,   SM_EVN_IGNORE,          sm_vs_state_do_event_nfy },

  /* !!! Don't remove this !!!************************************************/
  SM_STATE_COND_NULL
};

static SM_VSH_VBI_PSR_FCT_TBL  t_vsh_vbi_psr_fct_tbl =
{
    SM_VSH_VBI_TYPE_UNKNOWN,
    NULL,
    NULL,
    NULL,
    NULL
};

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
#ifdef CI_PLUS_SUPPORT
static INT32 _sm_video_hdlr_set_cci_plus_info(
    SM_VIDEO_STREAM_T*              pt_vs_obj,
    CP_INFO_LIST_T*                 pt_cp_info_list );
#else
static INT32 _sm_video_hdlr_set_cci_info(
    SM_VIDEO_STREAM_T*           pt_vs_obj,
    CP_INFO_T*                   pt_cp_info );
#endif

/*-----------------------------------------------------------------------------
                    Internal function implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_cch_vbi_psr_cb_fct
 *
 * Description: <function description>
 *
 * Inputs:  pv_tag     References.
 *          ui1_char_1 References.
 *          ui2_char_2 References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID _sm_video_hdlr_cch_vbi_psr_cb_fct(
    VOID*        pv_tag,
    UINT8        ui1_char_1,
    UINT8        ui1_char_2 )
{
    HANDLE_T              h_stream = (HANDLE_T)pv_tag;
    SM_VIDEO_HDLR_MSG_T   t_msg;

    if ( !pv_tag )
    {
        SM_VS_DBG_ERR(( "[SM] ERROR! _sm_video_hdlr_cch_vbi_psr_cb_fct() "
                        "with NULL pv_tag\r\n" ));
        return;
    }

    /* Pretend to be a CC data notify from TVD */
    t_msg.h_stream = h_stream;
    t_msg.e_msg_type = SM_VIDEO_HDLR_MSG_TVD_CC_DATA;
    t_msg.u.t_msg_tvd_cc.b_even_field = TRUE;
    t_msg.u.t_msg_tvd_cc.ui1_byte1 = ui1_char_1;
    t_msg.u.t_msg_tvd_cc.ui1_byte2 = ui1_char_2;

    /* send message */
    sm_vs_send_msg( &t_msg );
}

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_do_msg_do_event
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
static VOID _sm_video_hdlr_do_msg_do_event(
    HANDLE_T                       h_stream,
    SM_VIDEO_STREAM_T*             pt_vs_obj,
    SM_VIDEO_HDLR_MSG_DO_EVENT_T*  pt_msg )
{
    sm_state_on_event( &pt_vs_obj->t_state,
                       pt_msg->ui4_event,
                       (VOID*)pt_vs_obj,
                       (VOID*)pt_msg->ui4_data,
                       NULL );
}

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_record_rm_nfy_cond
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
static VOID _sm_video_hdlr_record_rm_nfy_cond(
    SM_DRV_COMP_DESCR_T*        pt_drv_descr,
    RM_COND_T                   e_old_cond,
    SM_VIDEO_HDLR_MSG_RM_NFY_T* pt_msg )
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
 * Name: _sm_video_hdlr_do_msg_rm_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_vs_obj References.
 *          pt_msg    References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_video_hdlr_do_msg_rm_nfy(
    HANDLE_T                    h_stream,
    SM_VIDEO_STREAM_T*          pt_vs_obj,
    SM_EVN_TYPE_T               e_event_grp,
    SM_VIDEO_HDLR_MSG_RM_NFY_T* pt_msg )
{
    SM_DRV_COMP_DESCR_T* pt_drv_descr = NULL;
    RM_COND_T            e_cond = RM_COND_IGNORE;
    RM_COND_T            e_old_cond = RM_COND_IGNORE;
    UINT32               ui4_latest_event = SM_EVN_IGNORE;

    SM_ASSERT( pt_vs_obj );
    SM_ASSERT( pt_msg );

    switch( e_event_grp )
    {
    case SM_EVN_VS_GRP_DEC:
        pt_drv_descr = &pt_vs_obj->t_vs_comp_grp.t_dec;
        ui4_latest_event = pt_vs_obj->ui4_lastest_dec_event;
        break;

    case SM_EVN_VS_GRP_VP:
        pt_drv_descr = &pt_vs_obj->t_vs_comp_grp.t_vp;
        ui4_latest_event = pt_vs_obj->ui4_lastest_vp_event;
        break;

    case SM_EVN_VS_GRP_PMX:
        pt_drv_descr = &pt_vs_obj->t_vs_comp_grp.t_pmx;
        ui4_latest_event = pt_vs_obj->ui4_lastest_pmx_event;
        break;

    case SM_EVN_VS_GRP_TVE:
        pt_drv_descr = &pt_vs_obj->t_vs_comp_grp.t_tve;
        ui4_latest_event = pt_vs_obj->ui4_lastest_tve_event;
        break;

    default:
        SM_VS_DBG_ERR(( "[SM] VS, %s:%d, invalid driver event group 0x%.8X!\r\n",
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
            if ( e_event_grp == SM_EVN_VS_GRP_VP )
            {
                sm_vs_comp_vp_set_op_monitor( pt_vs_obj, pt_msg->ui4_data );
            }
            else if ( e_event_grp == SM_EVN_VS_GRP_DEC &&
                      pt_vs_obj->t_vs_comp_grp.t_dec.t_info.e_type == DRVT_TV_DEC )
            {
                sm_vs_comp_tvd_set_op_monitor( pt_vs_obj, pt_msg->ui4_data );
            }
            break;

        default:
            break;
        }
        
#if 0   /* Don't show the message, because there are too many */ 
        /* Don't show message for Video Decoder, because there are too many for
           Closed Caption. */
        if ( e_event_grp != SM_EVN_VS_GRP_DEC )
        {
            SM_VS_DBG_INFO((
                "[SM] VS, Filter out RM operation condition(0x%.8x) notify for VS event group 0x%.8x.\r\n",
                pt_msg->e_cond, (UINT32)e_event_grp ));
        }
#endif   
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
        _sm_video_hdlr_record_rm_nfy_cond(pt_drv_descr,
                                          e_old_cond,
                                          pt_msg);
        sm_state_on_event( &pt_vs_obj->t_state,
                           SM_MAKE_DRV_EVN( e_event_grp, SM_EVN_CODE_DRV_CLOSED),
                           (VOID*)pt_vs_obj,
                           (VOID*)NULL,
                           NULL );
        return;

    case RM_COND_OPENED:
        _sm_video_hdlr_record_rm_nfy_cond(pt_drv_descr,
                                          e_old_cond,
                                          pt_msg);
        sm_state_on_event( &pt_vs_obj->t_state,
                           SM_MAKE_DRV_EVN( e_event_grp, SM_EVN_CODE_DRV_OPENED),
                           (VOID*)pt_vs_obj,
                           (VOID*)NULL,
                           NULL );
        return;

    case RM_COND_NO_CHG:
        break;

    default:
        SM_VS_DBG_ERR((
            "[SM] VS, unhandled RM object state = %u\r\n", e_cond ));
        break;
    }

    /* check if the connection state is hard wired */
    e_cond = rm_cond_chg( pt_msg->e_cond,
                          RM_COND_IGNORE,
                          COND_GRP_CONN_STATE );
    if ( e_cond == RM_COND_HARD_WIRED &&
         (ui4_latest_event&(~SM_EVN_GRP_BIT_MASK)) == SM_EVN_CODE_DRV_OPENED )
    {
        _sm_video_hdlr_record_rm_nfy_cond(pt_drv_descr,
                                          e_old_cond,
                                          pt_msg);
        sm_state_on_event( &pt_vs_obj->t_state,
                           SM_MAKE_DRV_EVN( e_event_grp, SM_EVN_CODE_DRV_CONNECTED),
                           (VOID*)pt_vs_obj,
                           (VOID*)NULL,
                           NULL );
        return;
    }
    if ( e_cond == RM_COND_HARD_WIRED &&
         (ui4_latest_event&(~SM_EVN_GRP_BIT_MASK)) == SM_EVN_CODE_DRV_CONNECTED )
    {
        _sm_video_hdlr_record_rm_nfy_cond(pt_drv_descr,
                                          e_old_cond,
                                          pt_msg);
        sm_state_on_event( &pt_vs_obj->t_state,
                           SM_MAKE_DRV_EVN( e_event_grp, SM_EVN_CODE_DRV_DISCONNECTED),
                           (VOID*)pt_vs_obj,
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
        _sm_video_hdlr_record_rm_nfy_cond(pt_drv_descr,
                                          e_old_cond,
                                          pt_msg);
        sm_state_on_event( &pt_vs_obj->t_state,
                           SM_MAKE_DRV_EVN( e_event_grp, SM_EVN_CODE_DRV_DISCONNECTED),
                           (VOID*)pt_vs_obj,
                           (VOID*)NULL,
                           NULL );
        return;

    case RM_COND_CONNECTED:
        if ( pt_vs_obj->t_vs_comp_grp.e_grp_status == SM_VS_COMP_GRP_STATUS_OPENED )
        {
            _sm_video_hdlr_record_rm_nfy_cond(pt_drv_descr,
                                              e_old_cond,
                                              pt_msg);
            sm_state_on_event( &pt_vs_obj->t_state,
                               SM_MAKE_DRV_EVN( e_event_grp, SM_EVN_CODE_DRV_CONNECTED),
                               (VOID*)pt_vs_obj,
                               (VOID*)NULL,
                               NULL );
        }
        return;

    case RM_COND_MULTI_PORT:
        break;

    case RM_COND_NO_CHG:
        if( e_event_grp == SM_EVN_VS_GRP_VP )
        {
            /* for handle two video stream in 3D mode*/
            e_cond = rm_cond_chg( pt_msg->e_cond,
                           RM_COND_IGNORE,
                           COND_GRP_CONN_STATE );

            if ( e_cond  == RM_COND_CONNECTED)
            {
                SM_VS_DBG_INFO((
                    "[SM] VS(0x%.8X): RM_COND_NO_CHG, RM_COND_CONNECTED.\r\n", h_stream ));
                
                sm_state_on_event( &pt_vs_obj->t_state,
                               SM_MAKE_DRV_EVN( e_event_grp, SM_EVN_CODE_DRV_CONNECTED),
                               (VOID*)pt_vs_obj,
                               (VOID*)NULL,
                               NULL );
            }
            if ( e_cond  == RM_COND_DISCONNECTED)
            {
                SM_VS_DBG_INFO((
                    "[SM] VS(0x%.8X): RM_COND_NO_CHG, RM_COND_DISCONNECTED.\r\n", h_stream ));
                
                sm_state_on_event( &pt_vs_obj->t_state,
                               SM_MAKE_DRV_EVN( e_event_grp, SM_EVN_CODE_DRV_DISCONNECTED),
                               (VOID*)pt_vs_obj,
                               (VOID*)NULL,
                               NULL );
            }
       }
       break;

    default:
        SM_VS_DBG_ERR((
            "[SM] VS, unhandled RM connection state = 0x%.8X\r\n", e_cond ));
        break;
    }


}

#ifdef DEBUG
static CHAR* _sm_video_hdlr_get_dec_status_str(
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

#ifdef DEBUG
static CHAR* _sm_video_hdlr_get_actual_enc_str(
    VID_ENC_T   e_actual_enc )
{
    switch( e_actual_enc )
    {
    case VID_ENC_UNKNOWN:
        return "VID_ENC_UNKNOWN";

    case VID_ENC_MPEG_1:
        return "VID_ENC_MPEG_1";

    case VID_ENC_MPEG_2:
        return "VID_ENC_MPEG_2";

    case VID_ENC_MPEG_4:
        return "VID_ENC_MPEG_4";

    case VID_ENC_DIVX_311:
        return "VID_ENC_DIVX_311";

    case VID_ENC_DIVX_4:
        return "VID_ENC_DIVX_4";
        
    case VID_ENC_DIVX_5:
        return "VID_ENC_DIVX_5";

    case VID_ENC_XVID:
        return "VID_ENC_XVID";
        
    case VID_ENC_WMV1:
        return "VID_ENC_WMV1";
    
    case VID_ENC_WMV2:
        return "VID_ENC_WMV2";
     
    case VID_ENC_WMV3:
        return "VID_ENC_WMV3";
        
    case VID_ENC_WVC1:
        return "VID_ENC_WVC1";
        
    case VID_ENC_H264:
        return "VID_ENC_H264";
        
    case VID_ENC_H263:
        return "VID_ENC_H263";
        
    case VID_ENC_MJPEG:
        return "VID_ENC_MJPEG";
        
    case VID_ENC_RV8:
        return "VID_ENC_RV8";
        
    case VID_ENC_RV9:
        return "VID_ENC_RV9";
        
    case VID_ENC_SORENSON:
        return "VID_ENC_SORENSON";
        
    case VID_ENC_AVS:
        return "VID_ENC_AVS"; 
     
    case VID_ENC_NV12:
        return "VID_ENC_NV12"; 

    case VID_ENC_VP8:
        return "VID_ENC_VP8"; 

    case VID_ENC_VP6:
        return "VID_ENC_VP6"; 
             
    default:
        return "INVALID_VALUE";
    }
}
#endif /* DEBUG */
 
/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_do_msg_vd_ctrl_nfy
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
static VOID _sm_video_hdlr_do_msg_vd_ctrl_nfy(
    HANDLE_T                                h_stream,
    SM_VIDEO_STREAM_T*                      pt_vs_obj,
    const SM_VIDEO_HDLR_MSG_VD_CTRL_NFY_T*  pt_msg )
{
    VID_ENC_T e_nfy_actual_enc;

    SM_ASSERT( pt_vs_obj );
    SM_ASSERT( pt_msg );
    
    switch( pt_msg->e_vd_nfy_cond )
    {
    case VID_DEC_COND_ERROR:
        /* to-do, reset video decoder */
        sm_state_on_event( &pt_vs_obj->t_state,
                           SM_EVN_VS_INTERNAL_ERR,
                           (VOID*)pt_vs_obj,
                           (VOID*)SMR_DRV_COMP_ERROR,
                           NULL );
        break;

    case VID_DEC_COND_CTRL_DONE:
        /* change state and notify sess handler */ 
        /* had changed for CR: DTV00091838 */
        if ((pt_vs_obj->e_vd_ctrl_type == VID_DEC_CTRL_PLAY &&
              (VID_DEC_CTRL_T)pt_msg->ui4_data_1 == VID_DEC_CTRL_PLAY ) ||
            (pt_vs_obj->e_vd_ctrl_type == VID_DEC_CTRL_FREEZE &&
              (VID_DEC_CTRL_T)pt_msg->ui4_data_1 == VID_DEC_CTRL_FREEZE))
        {
            sm_state_on_event( &pt_vs_obj->t_state,
                               SM_EVN_VS_DEC_PLAYED,
                               (VOID*)pt_vs_obj,
                               (VOID*)NULL,
                               NULL );
        }
        else if ( pt_vs_obj->e_vd_ctrl_type == VID_DEC_CTRL_STOP &&
                  (VID_DEC_CTRL_T)pt_msg->ui4_data_1 == VID_DEC_CTRL_STOP )
        {
            sm_state_on_event( &pt_vs_obj->t_state,
                               SM_EVN_VS_DEC_STOPPED,
                               (VOID*)pt_vs_obj,
                               (VOID*)NULL,
                               NULL );
        }
        else if ( pt_vs_obj->e_vd_ctrl_type == VID_DEC_CTRL_RESET &&
                  (VID_DEC_CTRL_T)pt_msg->ui4_data_1 == VID_DEC_CTRL_RESET )
        {
            /* not used currently, iolo */
        }
        else if ( (VID_DEC_CTRL_T)pt_msg->ui4_data_1 != VID_DEC_CTRL_RESET &&
                  (VID_DEC_CTRL_T)pt_msg->ui4_data_1 != VID_DEC_CTRL_STOP &&
                  (VID_DEC_CTRL_T)pt_msg->ui4_data_1 != VID_DEC_CTRL_FREEZE &&
                  (VID_DEC_CTRL_T)pt_msg->ui4_data_1 != VID_DEC_CTRL_PLAY &&
                  (VID_DEC_CTRL_T)pt_msg->ui4_data_1 != VID_DEC_CTRL_PLAY_I_FRAME )
        {
            SM_VS_DBG_ERR((
                "[SM] VS(0x%.8X), invalid VID_DEC_CTRL_T = 0x%.8X\r\n",
                h_stream, (VID_DEC_CTRL_T)pt_msg->ui4_data_1));
            SM_ASSERT(0);
        }
        break;

    case VID_DEC_COND_RESOLUTION_CHG:
        sm_state_on_event( &pt_vs_obj->t_state,
                           SM_EVN_VS_VIDEO_RESOLUTION_CHG,
                           (VOID*)pt_vs_obj,
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

            SM_VS_DBG_INFO((
                "[SM] VS, VD notify VID_DEC_COND_DECODE_STATUS_CHG (%s).\r\n",
                _sm_video_hdlr_get_dec_status_str( (VID_DEC_DECODE_STATUS_T)pt_msg->ui4_data_1 )));

            if ( ((VID_DEC_DECODE_STATUS_T)pt_msg->ui4_data_1) != pt_vs_obj->e_dec_status )
            {
                if ( ((VID_DEC_DECODE_STATUS_T)pt_msg->ui4_data_1) == VID_DEC_DECODE_CODEC_TYPE_CHG )
                {
                    sm_vs_comp_convert_vd_codec_to_scdb_enc((VID_DEC_CODEC_T)pt_msg->ui4_data_2,
                                                             &e_nfy_actual_enc );
                    if (pt_vs_obj->e_actual_enc != e_nfy_actual_enc)
                    {
                        pt_vs_obj->e_actual_enc = e_nfy_actual_enc;
                        /* restart video play */
                        sm_vs_send_msg_do_event(
                            pt_vs_obj->t_base.h_stream,
                            SM_EVN_VS_RESTART,
                            (UINT32)pt_vs_obj->t_base.i4_speed );

                        SM_VS_DBG_INFO((
                            "[SM] VS(0x%.8X), codec type changed to (%s)!\r\n",
                            (UINT32)pt_vs_obj->t_base.h_stream,
                            _sm_video_hdlr_get_actual_enc_str((VID_ENC_T)pt_vs_obj->e_actual_enc )));
                    }
                }
                else
                {
                    sm_state_on_event( &pt_vs_obj->t_state,
                                       SM_EVN_VS_DIG_DECODE_STATUS_CHG,
                                       (VOID*)pt_vs_obj,
                                       (VOID*)pt_msg->ui4_data_1,
                                       NULL );
                }
            }
        }
        break;

    case VID_DEC_COND_SPEED_CHG:
        sm_state_on_event( &pt_vs_obj->t_state,
                           SM_EVN_VS_SPEED_CHG,
                           (VOID*)pt_vs_obj,
                           (VOID*)pt_msg->ui4_data_2,
                           NULL );
        break;

    case VID_DEC_COND_THUMBNAIL_DONE:
        sm_state_on_event( &pt_vs_obj->t_state,
                           SM_EVN_VS_THUMBNAIL_DONE,
                           (VOID*)pt_vs_obj,
                           (VOID*)NULL,
                           NULL );
        break;
        
    case VID_DEC_COND_BIT_RATE:
        sm_state_on_event( &pt_vs_obj->t_state,
                           SM_EVN_VS_BIT_RATE,
                           (VOID*)pt_vs_obj,
                           (VOID*)pt_msg->ui4_data_1,
                           NULL );
        break;
        
    case VID_DEC_COND_PLAY_DONE:
        sm_state_on_event( &pt_vs_obj->t_state,
                           SM_EVN_VS_VIDEO_PLAY_DONE,
                           (VOID*)pt_vs_obj,
                           (VOID*)NULL,
                           NULL );
        break;

    case VID_DEC_COND_FRAMERATE_NOT_SUPPORT:
        sm_state_on_event( &pt_vs_obj->t_state,
                           SM_EVN_VS_FRAMERATE_NOT_SUPPORT,
                           (VOID*)pt_vs_obj,
                           (VOID*)NULL,
                           NULL );
        break;
    
    default:
        SM_VS_DBG_ERR((
            "[SM] VS, invalid Video Decoder condition = 0x%.8X\r\n",
            pt_msg->e_vd_nfy_cond ));
        SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_do_msg_vp_ctrl_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_vs_obj References.
 *          pt_msg    References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_video_hdlr_do_msg_vp_ctrl_nfy(
    HANDLE_T                                h_stream,
    SM_VIDEO_STREAM_T*                      pt_vs_obj,
    const SM_VIDEO_HDLR_MSG_VP_CTRL_NFY_T*  pt_msg )
{
    SM_ASSERT( pt_vs_obj );
    SM_ASSERT( pt_msg );

    switch( pt_msg->e_vp_nfy_cond )
    {
    case VID_PLA_COND_ERROR:
        /* to-do, reset video plane */
        sm_state_on_event( &pt_vs_obj->t_state,
                           SM_EVN_VS_INTERNAL_ERR,
                           (VOID*)pt_vs_obj,
                           (VOID*)SMR_DRV_COMP_ERROR,
                           NULL );
        break;

    case VID_PLA_COND_EOS:
        sm_vs_send_msg_do_event(
            h_stream,
            SM_EVN_VS_EOS,
            pt_msg->ui4_data_1 );
        break;

    case VID_PLA_COND_RANGE_AB_DONE: 
        sm_vs_send_msg_do_event(
            h_stream,
            SM_EVN_RANGE_AB_DONE,
            pt_msg->ui4_data_1 );
        break;

    case VID_PLA_COND_DISP_MODE:
        if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
        {
            SM_VS_STATE_TAG_T          t_state_tag;
            t_state_tag.h_stream = h_stream;
            t_state_tag.pt_vs_obj = pt_vs_obj;
            if (pt_msg->ui4_data_1 == 0) /* scaler display mode NOT Support */
            {
                x_dbg_stmt("[SM] VS, scaler notify display mode NOT Support!\r\n");
                if (pt_vs_obj->t_vid_res.e_timing_type != VSH_SRC_TIMING_NOT_SUPPORT)
                {
                    pt_vs_obj->t_vid_res.e_timing_type = VSH_SRC_TIMING_NOT_SUPPORT;
                    sm_vs_state_notify_app(
                        &t_state_tag,
                        SM_COND_STARTED,
                        SM_EVN_VS_VIDEO_RESOLUTION_CHG,
                        (UINT32)&pt_vs_obj->t_vid_res );
                    x_dbg_stmt("[SM] VS, video handler simulate notify resolution chg!\r\n");
                }
            }
            else if (pt_msg->ui4_data_1 == 1) /* scaler display mode Support */
            {
                x_dbg_stmt("[SM] VS, scaler notify display mode Support!\r\n");
                if (pt_vs_obj->t_vid_res.e_timing_type == VSH_SRC_TIMING_NOT_SUPPORT)
                {
                    pt_vs_obj->t_vid_res.e_timing_type = VSH_SRC_TIMING_VIDEO;
                    sm_vs_state_notify_app(
                        &t_state_tag,
                        SM_COND_STARTED,
                        SM_EVN_VS_VIDEO_RESOLUTION_CHG,
                        (UINT32)&pt_vs_obj->t_vid_res );
                    x_dbg_stmt("[SM] VS, video handler simulate notify resolution chg!\r\n");
                }
            }
            else
            {
                x_dbg_stmt("[SM] VS, display mode notify invalid value!\r\n");
            }
        }
        break;

    case VID_PLA_COND_DISP_FRAME:
        sm_state_on_event( &pt_vs_obj->t_state,
                            SM_EVN_VS_DISP_FRAME,
                            (VOID*)pt_vs_obj,
                            (VOID*)NULL,
                            NULL );
        break;
 
#ifdef DIVX_PLUS_CER       
    case VID_PLA_COND_CHAP_DONE: 
        sm_vs_send_msg_do_event(
            h_stream,
            SM_EVN_VS_CHAP_DONE,
            pt_msg->ui4_data_1 );
        break;
#endif 
   
    default:
        SM_VS_DBG_ERR((
            "[SM] VS, invalid Video Plane condition = 0x%.8X\r\n",
            pt_msg->e_vp_nfy_cond ));
        SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_get_color_sys_str
 *
 * Description: <function description>
 *
 * Inputs:  e_color_sys  References.
 *
 * Outputs: -
 *
 * Returns: string of color system if success.
 *          NULL if error.
 *
 ----------------------------------------------------------------------------*/
#ifdef DEBUG
static CHAR* _sm_video_hdlr_get_color_sys_str(
    TV_DEC_COLOR_SYS_T   e_color_sys
)
{
    switch( e_color_sys )
    {
    case TV_DEC_COLOR_SYS_UNKNOWN:
        return "TV_DEC_COLOR_SYS_UNKNOWN";

    case TV_DEC_COLOR_SYS_NTSC:
        return "TV_DEC_COLOR_SYS_NTSC";

    case TV_DEC_COLOR_SYS_NTSC_443:
        return "TV_DEC_COLOR_SYS_NTSC_443";

    case TV_DEC_COLOR_SYS_PAL:
        return "TV_DEC_COLOR_SYS_PAL";

    case TV_DEC_COLOR_SYS_PAL_N:
        return "TV_DEC_COLOR_SYS_PAL_N";

    case TV_DEC_COLOR_SYS_PAL_M:
        return "TV_DEC_COLOR_SYS_PAL_M";

    case TV_DEC_COLOR_SYS_PAL_60:
        return "TV_DEC_COLOR_SYS_PAL_60";

    case TV_DEC_COLOR_SYS_SECAM:
        return "TV_DEC_COLOR_SYS_SECAM";

    default:
        return NULL;
    }
}
#endif /* DEBUG */

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_get_color_sys_str
 *
 * Description: <function description>
 *
 * Inputs:  e_color_sys  References.
 *
 * Outputs: -
 *
 * Returns: string of color system if success.
 *          NULL if error.
 *
 ----------------------------------------------------------------------------*/
#ifdef DEBUG
static CHAR* _sm_video_hdlr_get_tv_sig_str(
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
 * Name: _sm_video_hdlr_do_msg_tvd_ctrl_nfy
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
static VOID _sm_video_hdlr_do_msg_tvd_ctrl_nfy(
    HANDLE_T                                h_stream,
    SM_VIDEO_STREAM_T*                      pt_vs_obj,
    const SM_VIDEO_HDLR_MSG_TVD_CTRL_NFY_T* pt_msg )
{
    SM_ASSERT( pt_vs_obj );
    SM_ASSERT( pt_msg );

    switch( pt_msg->e_tvd_nfy_cond )
    {
    case TV_DEC_COND_ERROR:
        /* to-do, reset TV decoder */
        if ( TV_DEC_SET_TYPE_CTRL == (DRV_SET_TYPE_T)pt_msg->ui4_data_1 )
        {
            sm_state_on_event( &pt_vs_obj->t_state,
                               SM_EVN_VS_INTERNAL_ERR,
                               (VOID*)pt_vs_obj,
                               (VOID*)SMR_DRV_COMP_ERROR,
                               NULL );
        }
        else if ( TV_DEC_SET_TYPE_AUTO_CLK_PHS_POS == (DRV_SET_TYPE_T)pt_msg->ui4_data_1 )
        {
            sm_state_on_event( &pt_vs_obj->t_state,
                               SM_EVN_VS_AUTO_CLK_POS_PHS_DONE,
                               (VOID*)pt_vs_obj,
                               (VOID*)FALSE,
                               NULL );
        }
        else if ( TV_DEC_SET_TYPE_AUTO_COLOR == (DRV_SET_TYPE_T)pt_msg->ui4_data_1 )
        {
            sm_state_on_event( &pt_vs_obj->t_state,
                               SM_EVN_VS_AUTO_COLOR_DONE,
                               (VOID*)pt_vs_obj,
                               (VOID*)((UINT32)FALSE),
                               NULL );
        }
        else
        {
            SM_VS_DBG_ERR((
                "[SM] VS, _sm_video_hdlr_do_msg_tvd_ctrl_nfy() "
                "invalid (TV_DEC_CTRL_T)ui4_data_1 = 0x%.8X\r\n",
                pt_vs_obj->e_tvd_ctrl_type ));
            SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
        }
        break;

    case TV_DEC_COND_CTRL_DONE:
        /* change state and notify sess handler */
        if ( TV_DEC_SET_TYPE_CTRL == (DRV_SET_TYPE_T)pt_msg->ui4_data_1 )
        {
            if ( pt_vs_obj->e_tvd_ctrl_type == TV_DEC_CTRL_PLAY )
            {
                sm_state_on_event( &pt_vs_obj->t_state,
                                   SM_EVN_VS_DEC_PLAYED,
                                   (VOID*)pt_vs_obj,
                                   (VOID*)NULL,
                                   NULL );
            }
            else if ( pt_vs_obj->e_tvd_ctrl_type == TV_DEC_CTRL_STOP )
            {
                sm_state_on_event( &pt_vs_obj->t_state,
                                   SM_EVN_VS_DEC_STOPPED,
                                   (VOID*)pt_vs_obj,
                                   (VOID*)NULL,
                                   NULL );
            }
            else if ( pt_vs_obj->e_tvd_ctrl_type == TV_DEC_CTRL_RESET )
            {
                /* not used currently, iolo */
            }
            else
            {
                SM_VS_DBG_ERR((
                    "[SM] VS, _sm_video_hdlr_do_msg_tvd_ctrl_nfy() "
                    "invalid TV_DEC_CTRL_T = 0x%.8X\r\n",
                    pt_vs_obj->e_tvd_ctrl_type ));
                SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
            }
        }
        else if ( TV_DEC_SET_TYPE_AUTO_CLK_PHS_POS == (DRV_SET_TYPE_T)pt_msg->ui4_data_1 )
        {
            sm_state_on_event( &pt_vs_obj->t_state,
                               SM_EVN_VS_AUTO_CLK_POS_PHS_DONE,
                               (VOID*)pt_vs_obj,
                               (VOID*)TRUE,
                               NULL );
        }
        else if ( TV_DEC_SET_TYPE_AUTO_COLOR == (DRV_SET_TYPE_T)pt_msg->ui4_data_1 )
        {
            sm_state_on_event( &pt_vs_obj->t_state,
                               SM_EVN_VS_AUTO_COLOR_DONE,
                               (VOID*)pt_vs_obj,
                               (VOID*)((UINT32)TRUE),
                               NULL );
        }
        else
        {
            SM_VS_DBG_ERR((
                "[SM] VS, _sm_video_hdlr_do_msg_tvd_ctrl_nfy() "
                "invalid (TV_DEC_CTRL_T)ui4_data_1 = 0x%.8X\r\n",
                pt_vs_obj->e_tvd_ctrl_type ));
            SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
        }
        break;

    case TV_DEC_COND_RESOLUTION_CHG:
        sm_state_on_event( &pt_vs_obj->t_state,
                           SM_EVN_VS_VIDEO_RESOLUTION_CHG,
                           (VOID*)pt_vs_obj,
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

            SM_VS_DBG_INFO((
                "[SM] VS, TVD notify TV_DEC_COND_TV_SIGNAL (%s)\r\n",
                _sm_video_hdlr_get_tv_sig_str( (TV_DEC_TV_SIG_T)pt_msg->ui4_data_1 ) ));

            /* tvd will notify TV_DEC_TV_SIG_UNKNOWN while MODE_CHG due to weak signal */
            if ( ((TV_DEC_TV_SIG_T)pt_msg->ui4_data_1) == TV_DEC_TV_SIG_UNKNOWN )
            {
                x_memset( &pt_vs_obj->t_vid_res, 0, sizeof(pt_vs_obj->t_vid_res) );
            }
            if ( ((TV_DEC_TV_SIG_T)pt_msg->ui4_data_1) != pt_vs_obj->e_tv_signal )
            {
                sm_state_on_event( &pt_vs_obj->t_state,
                                   SM_EVN_VS_ANA_TV_SIGNAL,
                                   (VOID*)pt_vs_obj,
                                   (VOID*)pt_msg->ui4_data_1,
                                   NULL );
            }
        }
        break;

    case TV_DEC_COND_COLOR_SYS:
        {
            COLOR_SYS_T  e_vid_color_sys;

#ifdef DEBUG
            CHAR* ps_color_sys_str = _sm_video_hdlr_get_color_sys_str(
                    (TV_DEC_COLOR_SYS_T)pt_msg->ui4_data_1);

            SM_VS_DBG_INFO((
                "[SM] VS, TV_DEC_COND_COLOR_SYS (%s)\r\n",
                ((ps_color_sys_str)? ps_color_sys_str:"") ));
#endif
            e_vid_color_sys = sm_vs_comp_tvd_color_sys_2_vsh(
                (TV_DEC_COLOR_SYS_T)pt_msg->ui4_data_1 );

            if ( e_vid_color_sys != pt_vs_obj->e_color_sys )
            {
                pt_vs_obj->e_color_sys = e_vid_color_sys;
                sm_state_on_event( &pt_vs_obj->t_state,
                                   SM_EVN_VS_ANA_COLOR_SYSTEM,
                                   (VOID*)pt_vs_obj,
                                   (VOID*)(UINT32)e_vid_color_sys,
                                   NULL );
            }
        }
        break;

    case TV_DEC_COND_CONTENT_ADV_INFO:
        {
            TV_DEC_CNT_ADV_INFO_T* pt_drv_cnt_adv =
                (TV_DEC_CNT_ADV_INFO_T*)pt_msg->ui4_data_1;

            if ( pt_drv_cnt_adv &&
                 ( pt_vs_obj->t_cnt_adv.ui1_char_1 != pt_drv_cnt_adv->ui1_char_1 ||
                   pt_vs_obj->t_cnt_adv.ui1_char_2 != pt_drv_cnt_adv->ui1_char_2 ) )
            {
                pt_vs_obj->t_cnt_adv.ui1_char_1 = pt_drv_cnt_adv->ui1_char_1;
                pt_vs_obj->t_cnt_adv.ui1_char_2 = pt_drv_cnt_adv->ui1_char_2;

                sm_state_on_event( &pt_vs_obj->t_state,
                                   SM_EVN_VS_ANA_CONTENT_ADV_INFO,
                                   (VOID*)pt_vs_obj,
                                   (VOID*)&pt_vs_obj->t_cnt_adv,
                                   NULL );
            }
        }
        break;

    case TV_DEC_COND_TV_SIGNAL_DETECTING:
        sm_state_on_event( &pt_vs_obj->t_state,
                           SM_EVN_VS_ANA_TV_SIGNAL_DETECTING,
                           (VOID*)pt_vs_obj,
                           (VOID*)NULL,
                           NULL );
        break;

    case TV_DEC_COND_HDCP_CONTENT:
        {
            SM_VS_DBG_INFO((
                "[SM] VS, TVD notify TV_DEC_COND_HDCP_CONTENT (%d)\r\n",
                pt_msg->ui4_data_1 ));

            sm_state_on_event( &pt_vs_obj->t_state,
                               SM_EVN_VS_HDCP_CONTENT,
                               (VOID*)pt_vs_obj,
                               (VOID*)pt_msg->ui4_data_1,
                               NULL );
        }
        break;

    case TV_DEC_COND_MACROVISION:
        {
            SM_VS_DBG_INFO((
                "[SM] VS, TVD notify TV_DEC_COND_MACROVISION (%d)\r\n",
                pt_msg->ui4_data_1 ));

            sm_state_on_event( &pt_vs_obj->t_state,
                               SM_EVN_VS_MACROVISION,
                               (VOID*)pt_vs_obj,
                               (VOID*)pt_msg->ui4_data_1,
                               NULL );
        }
        break;

    case TV_DEC_COND_AUD_PROTECT_INFO:
        {
            VSH_AUD_PROTECT_INFO_T  e_vsh_aud_protect_info;
            
            SM_VS_DBG_INFO((
                "[SM] VS, TVD notify TV_DEC_COND_AUD_PROTECT_INFO(%d)\r\n",
                pt_msg->ui4_data_1 ));

            e_vsh_aud_protect_info = sm_vs_comp_tvd_aud_protect_info_2_vsh(
                (TV_DEC_AUD_PROTECT_INFO_T)pt_msg->ui4_data_1 );

            sm_state_on_event( &pt_vs_obj->t_state,
                               SM_EVN_VS_AUD_PROTECT_INFO,
                               (VOID*)pt_vs_obj,
                               (VOID*)(UINT32)e_vsh_aud_protect_info,
                               NULL );
        }        
        break;

    default:
        SM_VS_DBG_ERR((
            "[SM] VS, invalid TV Decoder condition = 0x%.8X\r\n",
            pt_msg->e_tvd_nfy_cond ));
        SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_do_msg_pm_nfy
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
static VOID _sm_video_hdlr_do_msg_pm_nfy(
    HANDLE_T                                h_stream,
    SM_VIDEO_STREAM_T*                      pt_vs_obj,
    const SM_VIDEO_HDLR_MSG_PM_NFY_T*       pt_msg )
{
    sm_state_on_event(
        &pt_vs_obj->t_state,
        pt_msg->ui4_pm_event,
        (VOID*)pt_vs_obj,
        (VOID*)pt_msg->ui4_detail_code,
        NULL );
}

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_do_msg_cci_msg
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_vs_obj References.
 *          pt_msg    References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_video_hdlr_do_msg_cci_msg(
    HANDLE_T                                h_stream,
    SM_VIDEO_STREAM_T*                      pt_vs_obj,
    const SM_VIDEO_HDLR_MSG_CCI_NFY_T*      pt_msg )
{
    sm_state_on_event(
        &pt_vs_obj->t_state,
        SM_EVN_CCI_INFORMATION,
        (VOID*)pt_vs_obj,
        (VOID*)&pt_msg->t_pm_cp_info_lst,
        NULL );
}

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_do_msg_ca_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_vs_obj References.
 *          pt_msg    References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_video_hdlr_do_msg_ca_nfy(
    HANDLE_T                                h_stream,
    SM_VIDEO_STREAM_T*                      pt_vs_obj,
    const SM_VIDEO_HDLR_MSG_CA_NFY_T*       pt_msg )
{
    sm_state_on_event(
        &pt_vs_obj->t_state,
        pt_msg->ui4_ca_event,
        (VOID*)pt_vs_obj,
        (VOID*)pt_msg->ui4_detail_code,
        NULL );
}

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_do_msg_scdb_nfy
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
static VOID _sm_video_hdlr_do_msg_scdb_nfy(
    HANDLE_T                                h_stream,
    SM_VIDEO_STREAM_T*                      pt_vs_obj,
    const SM_VIDEO_HDLR_MSG_SCDB_NFY_T*     pt_msg )
{
    INT32                i4_ret;
    UINT32               ui4_reason = pt_msg->ui4_reason;
    SCDB_COND_T          e_scdb_cond = pt_msg->e_scdb_cond;
    SCDB_REC_T           t_scdb_rec = {0};
    SM_VS_STATE_TAG_T    t_state_tag;
    
    t_state_tag.pt_vs_obj = pt_vs_obj;
    t_state_tag.h_stream  = (HANDLE_T)pt_vs_obj->t_base.h_stream;
    
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
        if ( pt_vs_obj->ui2_force_pid != MPEG_2_NULL_PID )
        {
            /* If the stream is in force PID mode, ignore all SCDB update
               notification */
            return;
        }

        i4_ret = x_scdb_get_rec( pt_vs_obj->t_base.h_scdb,
                                 &pt_vs_obj->t_base.t_strm_comp_id,
                                 &t_scdb_rec );
        if ( i4_ret != SCDBR_OK ||
             ( pt_vs_obj->t_base.t_scdb_rec.e_rec_type != t_scdb_rec.e_rec_type ) )
        {
            SM_VS_DBG_ERR((
                        "[SM] VS: %s:%d, Error, x_scdb_get_rec() fail, i4_ret = %d\r\n",
                        __FILE__, __LINE__, i4_ret));
            return;
        }

        if ( ui4_reason & SCDB_RECORD_DEL )
        {
            /* As the record is found, my record is NOT deleted.
               Must be someone else, just ignore the notify. */
        }
        if ( ui4_reason & SCDB_RECORD_MOD &&
             x_memcmp( &t_scdb_rec, &pt_vs_obj->t_base.t_scdb_rec, sizeof(SCDB_REC_T) ) != 0 )
        {
            if ( t_scdb_rec.e_rec_type == SCDB_REC_TYPE_VIDEO_MPEG )
            {
                /* todo, see what SM could do?? */
                if ( t_scdb_rec.u.t_video_mpeg.ui2_pid !=
                     pt_vs_obj->t_base.t_scdb_rec.u.t_video_mpeg.ui2_pid )
                {
                    SM_VS_DBG_INFO((
                        "[SM] Video stream(0x%.8X) PID changed to 0x%.4X!\r\n",
                        (UINT32)h_stream,
                        t_scdb_rec.u.t_video_mpeg.ui2_pid ));
                }
                if ( t_scdb_rec.u.t_video_mpeg.e_vid_fmt !=
                     pt_vs_obj->t_base.t_scdb_rec.u.t_video_mpeg.e_vid_fmt )
                {
#ifdef DEBUG
                    if ( (UINT32)t_scdb_rec.u.t_video_mpeg.e_vid_fmt <
                         (sizeof(as_scdb_vid_fmt)/sizeof(CHAR*) - 1) )
                    {
                        SM_VS_DBG_INFO((
                            "[SM] Video stream(0x%.8X) format changed to %s!\r\n",
                            (UINT32)h_stream,
                            as_scdb_vid_fmt[(UINT32)t_scdb_rec.u.t_video_mpeg.e_vid_fmt] ));
                    }
#endif
                }
                if ( t_scdb_rec.u.t_video_mpeg.b_default !=
                     pt_vs_obj->t_base.t_scdb_rec.u.t_video_mpeg.b_default )
                {
                    SM_VS_DBG_INFO((
                        "[SM] Video stream(0x%.8X) %s!\r\n",
                        (UINT32)h_stream,
                        ( (t_scdb_rec.u.t_video_mpeg.b_default)?
                          "is now DEFAULT": "no longer DEFAULT" ) ));
                }
                if ( t_scdb_rec.u.t_video_mpeg.e_enc !=
                     pt_vs_obj->t_base.t_scdb_rec.u.t_video_mpeg.e_enc )
                {
#if 0   /* Mark this, because upper client can do restart action */ 
                    sm_vs_send_msg_do_event(
                        pt_vs_obj->t_base.h_stream,
                        SM_EVN_VS_RESTART,
                        (UINT32)pt_vs_obj->t_base.i4_speed );
#else 
                    if ( pt_vs_obj->t_base.e_condition == SM_COND_STARTED ) 
                    {  
                        sm_vs_state_notify_app(
                        &t_state_tag,
                        (SM_COND_T)pt_vs_obj->t_state.ui4_curr_state,
                        SM_EVN_VS_REQ_RESTART,
                        (UINT32)&pt_vs_obj->t_vid_res );
                    }
#endif
                    SM_VS_DBG_INFO((
                        "[SM] Video stream(0x%.8X) encoding changed to (%u)!\r\n",
                        (UINT32)pt_vs_obj->t_base.h_stream,
                        (UINT32)t_scdb_rec.u.t_video_mpeg.e_enc ));
                }
                /* for 3D video */
                pt_vs_obj->t_rd3d_info = pt_vs_obj->t_base.t_scdb_rec.u.t_video_mpeg.t_rd3d;
                pt_vs_obj->t_mvc_ext = pt_vs_obj->t_base.t_scdb_rec.u.t_video_mpeg.t_mvc_ext;
            }
            else if ( t_scdb_rec.e_rec_type == SCDB_REC_TYPE_VIDEO_ANALOG )
            {
                if ( t_scdb_rec.u.t_video_analog.e_vid_fmt !=
                     pt_vs_obj->t_base.t_scdb_rec.u.t_video_analog.e_vid_fmt )
                {
#ifdef DEBUG
                    if ( (UINT32)t_scdb_rec.u.t_video_analog.e_vid_fmt <
                         (sizeof(as_scdb_vid_fmt)/sizeof(CHAR*) - 1) )
                    {
                        SM_VS_DBG_INFO((
                            "[SM] Video stream(0x%.8X) format changed to (%s)!\r\n",
                            (UINT32)h_stream,
                            as_scdb_vid_fmt[(UINT32)t_scdb_rec.u.t_video_analog.e_vid_fmt] ));
                    }
#endif
                }
                if ( t_scdb_rec.u.t_video_analog.e_color_sys !=
                     pt_vs_obj->t_base.t_scdb_rec.u.t_video_analog.e_color_sys )
                {
#ifdef DEBUG
                    if ( ((UINT32)t_scdb_rec.u.t_video_analog.e_color_sys + 1) <
                         (sizeof(as_scdb_vid_color_sys)/sizeof(CHAR*) - 1) )
                    {
                        SM_VS_DBG_INFO((
                            "[SM] Video stream(0x%.8X) format changed to (%s)!\r\n",
                            (UINT32)h_stream,
                            as_scdb_vid_color_sys[((UINT32)t_scdb_rec.u.t_video_analog.e_color_sys+1)] ));
                    }
#endif
                }
                if ( t_scdb_rec.u.t_video_analog.b_default !=
                     pt_vs_obj->t_base.t_scdb_rec.u.t_video_analog.b_default )
                {
                    SM_VS_DBG_INFO((
                        "[SM] Video stream(0x%.8X) %s!\r\n",
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

        pt_vs_obj->t_base.t_scdb_rec = t_scdb_rec;
        /* set vs obj actual enc */
        sm_vs_comp_vd_get_actual_enc(pt_vs_obj);
    
    }
    else if ( e_scdb_cond == SCDB_UPDATING )
    {
        /* Ignore */
    }
    else if ( e_scdb_cond == SCDB_CLOSED )
    {
        /* Left it to be process by session layer */
        pt_vs_obj->t_base.h_scdb = NULL_HANDLE;
    }
    else
    {
        /* improper SCDB state */
        SM_VS_DBG_ERR(( "[SM] %s:%d Improper SCDB state!\r\n", __FILE__, __LINE__ ));
        SM_ASSERT( 0 );
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_do_msg_scdb_nfy
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
static VOID _sm_video_hdlr_do_msg_vbi_data_nfy(
    HANDLE_T                                    h_stream,
    SM_VIDEO_STREAM_T*                          pt_vs_obj,
    const SM_VIDEO_HDLR_MSG_VBI_DATA_NFY_T*     pt_msg )
{
    SM_VSH_VBI_INFO_T         t_vsh_vbi_info;

    if ( !pt_vs_obj->t_vbi_nfy.pf_vbi_nfy )
    {
        return;
    }
    if ( pt_msg->t_vbi_psr_id != pt_vs_obj->t_vbi_psr_id )
    {
        return;
    }

    t_vsh_vbi_info.e_vbi_type      = pt_msg->e_vbi_type;
    t_vsh_vbi_info.pv_vbi_data     = (VOID*)pt_msg->aui1_data;
    t_vsh_vbi_info.z_vbi_data_size = pt_msg->z_data_size;

    pt_vs_obj->t_vbi_nfy.pf_vbi_nfy(
        pt_vs_obj->t_vbi_nfy.pv_tag,
        (VOID*)&t_vsh_vbi_info );
}

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_do_msg_cc_ind_timer
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
static VOID _sm_video_hdlr_do_msg_cc_ind_timer(
    HANDLE_T                                    h_stream,
    SM_VIDEO_STREAM_T*                          pt_vs_obj,
    const SM_VIDEO_HDLR_MSG_CC_IND_TIMER_T*     pt_msg_cc_ind )
{
    /* CC indicator timer-out, set it to false and notify APP */
    pt_vs_obj->b_cc_indicator = FALSE;

    sm_vs_send_msg_do_event(
        h_stream,
        SM_EVN_VS_CC_INDICATOR,
        (UINT32)pt_vs_obj->b_cc_indicator );
}

/*-----------------------------------------------------------------------------
 * Name: _sm_cc_hdlr_reset_timer_cb(
 *
 * Description: <function description>
 *
 * Inputs:  pt_tm_handle References.
 *          pv_tag       References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_vs_hdlr_cc_ind_timer_cb(
    HANDLE_T  pt_tm_handle,
    VOID*     pv_tag )
{
    HANDLE_T                 h_stream = (HANDLE_T)pv_tag;
    SM_VIDEO_HDLR_MSG_T      t_msg;

    SM_ASSERT( pv_tag );

    t_msg.h_stream = h_stream;
    t_msg.e_msg_type = SM_VIDEO_HDLR_MSG_CC_IND_TIMER;

    t_msg.u.t_msg_cc_ind.pv_tag = pv_tag;

    sm_vs_send_msg( &t_msg );
}

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_start_cc_ind_timer(
 *
 * Description: <function description>
 *
 * Inputs:  h_stream        References.
 *          pt_vs_obj       References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_video_hdlr_start_cc_ind_timer(
    HANDLE_T                                    h_stream,
    SM_VIDEO_STREAM_T*                          pt_vs_obj )
{
    INT32     i4_ret;

    /* Stop old timer */
    if ( pt_vs_obj->b_is_timer_active )
    {
        /* if timer is ON, stop timer */
        i4_ret = x_timer_stop( pt_vs_obj->h_cc_ind_timer );
        if ( i4_ret != OSR_OK )
        {
            SM_VS_DBG_ERR(( "[SM] VS, %s:%d, x_timer_stop() fail, i4_ret = %d\r\n",
                              __FILE__, __LINE__, i4_ret ));
            SM_ABORT( SM_DBG_ABRT_CANNOT_STOP_TIMER );
        }
        pt_vs_obj->b_is_timer_active = FALSE;
    }

    /* Active new timer */
    i4_ret = x_timer_start( pt_vs_obj->h_cc_ind_timer,
                            SM_VS_RESET_CC_IND_TIMER_DELAY,
                            X_TIMER_FLAG_ONCE,
                            _sm_vs_hdlr_cc_ind_timer_cb,
                            (VOID*)(pt_vs_obj->t_base.h_stream) );
    if ( i4_ret != OSR_OK )
    {
        SM_VS_DBG_ERR((
            "[SM] VS, ERROR! x_timer_start(SM_VS_RESET_CC_IND_TIMER_DELAY) fail, i4_ret = %d\r\n",
            i4_ret ));
        SM_ABORT( SM_DBG_ABRT_CANNOT_START_TIMER );
        return;
    }

    pt_vs_obj->b_is_timer_active = TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_update_cc_indicator(
 *
 * Description: <function description>
 *
 * Inputs:  h_stream        References.
 *          pt_vs_obj       References.
 *          b_cc_indicator  References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_video_hdlr_update_cc_indicator(
    HANDLE_T                                    h_stream,
    SM_VIDEO_STREAM_T*                          pt_vs_obj,
    BOOL                                        b_cc_indicator )
{
    if ( b_cc_indicator )
    {
        _sm_video_hdlr_start_cc_ind_timer(
            h_stream, pt_vs_obj );

        /* Notify Application */
        if ( !pt_vs_obj->b_cc_indicator )
        {
            pt_vs_obj->b_cc_indicator = TRUE;

            sm_vs_send_msg_do_event(
                h_stream,
                SM_EVN_VS_CC_INDICATOR,
                (UINT32)pt_vs_obj->b_cc_indicator );
        }
    }
    else
    {
        /* if CC indicator is FALSE and no timer waiting */
        if ( !pt_vs_obj->b_is_timer_active )
        {
            _sm_video_hdlr_start_cc_ind_timer(
                h_stream, pt_vs_obj );
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_do_msg_scdb_nfy
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
static VOID _sm_video_hdlr_do_msg_tvd_cc_data_nfy(
    HANDLE_T                                    h_stream,
    SM_VIDEO_STREAM_T*                          pt_vs_obj,
    const SM_VIDEO_HDLR_MSG_TVD_CC_DATA_T*      pt_msg )
{
    UINT8   aui1_cc_data[3];
    BOOL    b_is_cc_cmd = FALSE;
    VOID*   pv_tag = NULL;

    aui1_cc_data[0] = (UINT8)pt_msg->b_even_field;
    aui1_cc_data[1] = pt_msg->ui1_byte1;
    aui1_cc_data[2] = pt_msg->ui1_byte2;

    if ( t_vsh_vbi_psr_fct_tbl.pf_vbi_parse )
    {
        if ( t_vsh_vbi_psr_fct_tbl.e_vbi_type == SM_VSH_VBI_TYPE_NTSC )
        {
            pv_tag = &b_is_cc_cmd;
        }

        t_vsh_vbi_psr_fct_tbl.pf_vbi_parse(
            pt_vs_obj->t_vbi_psr_id,
            aui1_cc_data,
            3,
            (VOID*)pv_tag );

        if ( t_vsh_vbi_psr_fct_tbl.e_vbi_type == SM_VSH_VBI_TYPE_NTSC )
        {
            _sm_video_hdlr_update_cc_indicator(
                h_stream,
                pt_vs_obj,
                b_is_cc_cmd );
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_vs_free_fct
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
static BOOL _sm_video_hdlr_vs_free_fct(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle )
{
    INT32                i4_ret;
    SM_VIDEO_STREAM_T*   pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_obj;

    if ( b_req_handle )
    {
        return FALSE;
    }

    x_sema_delete( pt_vs_obj->t_base.h_lock );
    pt_vs_obj->t_base.h_lock = NULL_HANDLE;

    if ( pt_vs_obj->t_base.h_scdb != NULL_HANDLE )
    {
        i4_ret = x_scdb_close( pt_vs_obj->t_base.h_scdb );
        if ( i4_ret != SCDBR_OK )
        {
            SM_ABORT( SM_DBG_ABRT_CANNOT_CLOSE_SCDB );
        }
        pt_vs_obj->t_base.h_scdb = NULL_HANDLE;
    }

    sm_drv_map_lock();

    if ( sm_vs_comp_grp_close( h_handle, &pt_vs_obj->t_vs_comp_grp ) != SMR_OK )
    {
        SM_ABORT( SM_DBG_ABRT_CANNOT_CLOSE_DRIVER_COMP );
    }

    sm_drv_map_unlock();

    if ( pt_vs_obj->t_base.h_pm_comp != NULL_HANDLE )
    {
        i4_ret = pm_del_component( pt_vs_obj->t_base.h_pm_comp );
        if ( i4_ret != SMR_OK )
        {
            SM_ABORT( SM_DBG_ABRT_CANNOT_CLOSE_PM_COMP );
        }
    }

    /* Close VBI parser */
    if ( pt_vs_obj->t_vbi_psr_id != (UINT32)0xFFFFFFFF &&
         t_vsh_vbi_psr_fct_tbl.e_vbi_type != SM_VSH_VBI_TYPE_UNKNOWN )
    {
        BOOL b_success;

        b_success = t_vsh_vbi_psr_fct_tbl.pf_vbi_close(
            pt_vs_obj->t_vbi_psr_id );
        if ( !b_success )
        {
            SM_ASSERT( 0 );
        }
        pt_vs_obj->t_vbi_psr_id = 0xFFFFFFFF;
    }

    if ( pt_vs_obj->t_state.pv_state_nfy_tag )
    {
        x_mem_free( pt_vs_obj->t_state.pv_state_nfy_tag );
    }

    x_timer_stop( pt_vs_obj->h_cc_ind_timer );
    x_timer_delete( pt_vs_obj->h_cc_ind_timer );
    pt_vs_obj->h_cc_ind_timer = NULL_HANDLE;

    x_mem_free( pt_vs_obj );

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_slctr_set
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
static INT32 _sm_video_hdlr_slctr_set(
    HANDLE_T       h_stream,
    SM_SET_TYPE_T  e_set_type,
    VOID*          pv_set_info,
    SIZE_T         z_set_info_len )
{
    INT32                 i4_ret = SMR_OK;
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;

    pt_vs_obj = (SM_VIDEO_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_vs_obj )
    {
        return SMR_INV_HANDLE;
    }

    /* change to xxx-ing states */
    if ( e_set_type == SM_STRM_SET_TYPE_PLAY )
    {
        if ( pt_vs_obj->t_base.e_condition == SM_COND_STARTED )
        {
            return SMR_ALREADY_PLAYED;
        }

        sm_vs_send_msg_do_event(
            h_stream,
            SM_EVN_VS_APP_PLAY_REQ,
            0 );
    }
    else if ( e_set_type == SM_SESS_SET_TYPE_PLAY )
    {
        if ( pt_vs_obj->t_base.e_condition == SM_COND_STARTED )
        {
            return SMR_ALREADY_PLAYED;
        }

        sm_vs_send_msg_do_event(
            h_stream,
            SM_EVN_SESS_APP_PLAY_REQ,
            0 );
    }
    else if ( e_set_type == SM_STRM_SET_TYPE_STOP )
    {
        if ( pt_vs_obj->t_base.e_condition == SM_COND_OPENED )
        {
            return SMR_ALREADY_STOPPED;
        }

        sm_vs_send_msg_do_event(
            h_stream,
            SM_EVN_VS_APP_STOP_REQ,
            0 );
    }
    else if ( e_set_type == SM_SESS_SET_TYPE_STOP )
    {
        if ( pt_vs_obj->t_base.e_condition == SM_COND_OPENED )
        {
            return SMR_ALREADY_STOPPED;
        }

        sm_vs_send_msg_do_event(
            h_stream,
            SM_EVN_SESS_APP_STOP_REQ,
            0 );
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
        
        i4_ret = _sm_video_hdlr_set_cci_plus_info( pt_vs_obj, pt_cp_info_lst );
#else
        CP_INFO_T*     pt_cp_info = (CP_INFO_T*)pv_set_info;

        if ( !pt_cp_info )
        {
            return SMR_INV_SET_INFO;
        }
        if ( z_set_info_len != sizeof( CP_INFO_T ) )
        {
            return SMR_INSUFFICIENT_SIZE;
        }

        i4_ret = _sm_video_hdlr_set_cci_info( pt_vs_obj, pt_cp_info );
#endif        
    }
    else
    {
        i4_ret = SMR_NOT_SUPPORT;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_swap_pla_z_order
 *
 * Description: <function description>
 *
 * Inputs:  pt_pmx           References.
 *          ui4_curr_adj_pla References.
 *          ui1_target_port  References.
 *          pui4_pla_order   References.
 *
 * Outputs: -
 *
 * Returns: SMR_INTERNAL_ERROR  Success.
 *          SMR_OK              Fail.
 *          SMR_NOT_ALLOW       Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sm_video_hdlr_swap_pla_z_order(
    SM_DRV_COMP_DESCR_T*  pt_pmx,
    UINT32               ui4_curr_adj_pla,
    UINT8                ui1_target_port,
    UINT32*              pui4_pla_order )
{
    INT32                             i4_ret;
    SIZE_T                            z_size;
    PLA_MXR_ADJ_PLA_BY_PORT_INFO_T    t_adj_pla_info;

    /* get plane order adjustment */
    z_size = sizeof(PLA_MXR_ADJ_PLA_BY_PORT_INFO_T);
    t_adj_pla_info.ui1_port = ui1_target_port;
    i4_ret = rm_get( pt_pmx->h_comp,
                     FALSE,
                     ANY_PORT_NUM,
                     PLA_MXR_GET_TYPE_ADJ_PLA_BY_PORT,
                     (VOID*)(&t_adj_pla_info),
                     &z_size,
                     &pt_pmx->e_comp_cond );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INTERNAL_ERROR;
    }

    if ( pt_pmx->t_info.ui1_in_port >= MAX_NUM_PORTS ||
         ui1_target_port >= MAX_NUM_PORTS )
    {
        return SMR_INTERNAL_ERROR;
    }
    
    /* Check if target port could adjust to current z-order */
    if ( t_adj_pla_info.ui4_adj_zorder & MAKE_BIT_MASK_32(pui4_pla_order[pt_pmx->t_info.ui1_in_port]) )
    {
        UINT32  ui4_z_tmp = pui4_pla_order[pt_pmx->t_info.ui1_in_port];

        pui4_pla_order[pt_pmx->t_info.ui1_in_port] = pui4_pla_order[ui1_target_port];
        pui4_pla_order[ui1_target_port] = ui4_z_tmp;

        return SMR_OK;
    }

    return SMR_NOT_ALLOW;
}


#ifdef CI_PLUS_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_cci_plus_pm_2_drv
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
static INT32 _sm_video_hdlr_cci_plus_pm_2_drv( 
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
 * Name: _sm_video_hdlr_cci_pm_2_drv
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
static TV_ENC_CCI_TYPE_T _sm_video_hdlr_cci_pm_2_drv( EMI_TYPE_T   e_pm_cci )
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
 * Name: _sm_video_hdlr_aps_pm_2_drv
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
static TV_ENC_MV_APS_TYPE_T _sm_video_hdlr_aps_pm_2_drv( APS_TYPE_T   e_pm_aps )
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

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_vd_res_2_vs
 *
 * Description: <function description>
 *
 * Inputs:  pt_drv_res   Driver defined resolution.
 *
 * Outputs: pt_vsh_res   SM defined resolution.
 *
 * Returns: TVE defined APS information.
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_video_hdlr_vd_res_2_vs(
    const VID_DEC_RESOLUTION_INFO_T*   pt_drv_res,
    VSH_SRC_RESOLUTION_INFO_T*          pt_vsh_res )
{
    pt_vsh_res->ui4_frame_rate = (UINT32)pt_drv_res->ui2_frame_rate;
    pt_vsh_res->ui4_width = (UINT32)pt_drv_res->ui2_width;
    pt_vsh_res->ui4_height = (UINT32)pt_drv_res->ui2_height;
    pt_vsh_res->ui4_aspect_ratio_w = (UINT32)pt_drv_res->u4_aspect_ratio_w; 
    pt_vsh_res->ui4_aspect_ratio_h = (UINT32)pt_drv_res->u4_aspect_ratio_h;
    pt_vsh_res->b_is_progressive = pt_drv_res->b_is_progressive;
    pt_vsh_res->b_overscan_not_applied = pt_drv_res->b_overscan_not_applied;
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
 * Name: _sm_video_hdlr_tvd_res_2_vs
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
static VOID _sm_video_hdlr_tvd_res_2_vs(
    const SM_VIDEO_STREAM_T*           pt_vs_obj,
    const TV_DEC_RESOLUTION_INFO_T*    pt_drv_res,
    VSH_SRC_RESOLUTION_INFO_T*          pt_vsh_res )
{
    pt_vsh_res->ui4_frame_rate = (UINT32)pt_drv_res->ui2_frame_rate;
    pt_vsh_res->ui4_width = (UINT32)pt_drv_res->ui2_width;
    pt_vsh_res->ui4_height = (UINT32)pt_drv_res->ui2_height;
    pt_vsh_res->b_is_progressive = pt_drv_res->b_is_progressive;
    pt_vsh_res->b_overscan_not_applied = FALSE;
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
    /*if ( pt_vs_obj->e_color_sys == COLOR_SYS_NTSC ||
         pt_vs_obj->e_color_sys == COLOR_SYS_NTSC_443 )*/
    if ( pt_drv_res->ui2_height == 480 )
    {
        VSH_SRC_RES_EXT_SET_NTSC_WSS_ASP_RATIO( pt_vsh_res->ui4_ext, TV_DEC_RES_EXT_GET_NTSC_WSS_ASP_RATIO(pt_drv_res->ui4_ext) );
    }
    else
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

#ifdef CI_PLUS_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_set_cci_plus_info
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
static INT32 _sm_video_hdlr_set_cci_plus_info(
    SM_VIDEO_STREAM_T*          pt_vs_obj,
    CP_INFO_LIST_T*             pt_cp_info_lst )
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

    if ( (pt_vs_obj == NULL) || (pt_cp_info_lst == NULL) )
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

    i4_ret = _sm_video_hdlr_cci_plus_pm_2_drv( pt_cp_rct_info, 
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

    if( pt_vs_obj->t_vs_comp_grp.t_tve.h_comp != NULL_HANDLE )
    {
        i4_ret = rm_set( pt_vs_obj->t_vs_comp_grp.t_tve.h_comp,
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
    }
    
    return SMR_OK;
}

#else

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_set_cci_info
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
static INT32 _sm_video_hdlr_set_cci_info(
    SM_VIDEO_STREAM_T*           pt_vs_obj,
    CP_INFO_T*                   pt_cp_info )
{
    INT32              i4_ret;
    TV_ENC_MV_INFO_T   t_tve_mv_info;
    TV_ENC_CCI_TYPE_T  e_tve_cci_type;

    UINT32             ui4_drv_set_type;
    VOID*              pv_drv_set_info;
    SIZE_T             z_drv_set_info_size = 0;
    RM_COND_T          e_drv_cond;

    if ( (pt_vs_obj == NULL) || (pt_cp_info == NULL) )
    {
        return SMR_INV_ARG;
    }

    switch( pt_cp_info->t_cp_type )
    {
    case CP_TYPE_EMI:
        e_tve_cci_type = _sm_video_hdlr_cci_pm_2_drv( pt_cp_info->u.e_emi_ctrl );

        ui4_drv_set_type = TV_ENC_SET_TYPE_CCI;
        pv_drv_set_info = (VOID*)(UINT32)e_tve_cci_type;
        z_drv_set_info_size = sizeof( TV_ENC_CCI_TYPE_T );
        break;

    case CP_TYPE_APS:
        t_tve_mv_info.e_mv_type = TV_ENC_MV_TYPE_APS;
        t_tve_mv_info.u.e_aps_type = _sm_video_hdlr_aps_pm_2_drv( pt_cp_info->u.e_aps_ctrl );

        ui4_drv_set_type = TV_ENC_SET_TYPE_MV;
        pv_drv_set_info = &t_tve_mv_info;
        z_drv_set_info_size = sizeof( TV_ENC_MV_INFO_T );
        break;

    case CP_TYPE_CIT:
    default:
        /* Ignore unknown CP types */
        return SMR_OK;
    }

    if( pt_vs_obj->t_vs_comp_grp.t_tve.h_comp != NULL_HANDLE )
    {
        i4_ret = rm_set( pt_vs_obj->t_vs_comp_grp.t_tve.h_comp,
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
    }
    
    return SMR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_set_pla_order
 *
 * Description: <function description>
 *
 * Inputs:  h_stream        References.
 *          pt_vs_obj       References.
 *          pt_order_info   References.
 *          z_pla_order_len References.
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
static INT32 _sm_video_hdlr_set_pla_order(
    HANDLE_T                     h_stream,
    SM_VIDEO_STREAM_T*           pt_vs_obj,
    VSH_SET_PLANE_ORDER_INFO_T*  pt_order_info,
    SIZE_T                       z_pla_order_len )
{
    INT32                         i4_ret;
    UINT32                        aui4_pla_order[MAX_NUM_PORTS];
    UINT32                        ui4_top, ui4_bottom, ui4_curr;
    UINT32                        ui4_num_pla, ui4_capa;
    UINT8                         ui1_i;
    SIZE_T                        z_size;
    PLA_MXR_ADJ_PLA_BY_ID_TYPE_INFO_T t_adj_pla_info;

    if ( !pt_order_info )
    {
        return SMR_INV_ARG;
    }

    if ( !pt_vs_obj ||
         pt_vs_obj->t_vs_comp_grp.t_pmx.t_info.ui1_in_port >= MAX_NUM_PORTS )
    {
        return SMR_INV_ARG;
    }

    SM_VS_DBG_INFO(( "[SM] VS, e_order_ctrl=%d\r\n", pt_order_info->e_order_ctrl ));

    /* get capability */
    z_size = sizeof( UINT32 );
    i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_pmx.h_comp,
                     FALSE,
                     ANY_PORT_NUM,
                     PLA_MXR_GET_TYPE_CAPABILITY,
                     (VOID*)(&ui4_capa),
                     &z_size,
                     &pt_vs_obj->t_vs_comp_grp.t_pmx.e_comp_cond );
    if ( i4_ret != SMR_OK )
    {
        return SMR_DRV_COMP_ERROR;
    }
    if ( !(ui4_capa & PLA_MXR_CAP_ADJ_PLA) )
    {
        return SMR_NOT_ALLOW;
    }

    /* get current order */
    z_size = sizeof( UINT32 ) * MAX_NUM_PORTS;
    i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_pmx.h_comp,
                     FALSE,
                     ANY_PORT_NUM,
                     PLA_MXR_GET_TYPE_PLA_ORDER,
                     (VOID*)(aui4_pla_order),
                     &z_size,
                     &pt_vs_obj->t_vs_comp_grp.t_pmx.e_comp_cond );
    if ( i4_ret != SMR_OK )
    {
        return SMR_NOT_ALLOW;
    }
    ui4_num_pla = z_size / sizeof(UINT32);
    if ( ui4_num_pla >= MAX_NUM_PORTS )
    {
        return SMR_INTERNAL_ERROR;
    }

    /* get plane order adjustment */
    z_size = sizeof(PLA_MXR_ADJ_PLA_BY_ID_TYPE_INFO_T);
    t_adj_pla_info.e_plane_type = pt_vs_obj->t_vs_comp_grp.t_vp.t_info.e_type;
    t_adj_pla_info.ui2_comp_id = pt_vs_obj->t_vs_comp_grp.t_vp.t_info.ui2_comp_id;
    i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_pmx.h_comp,
                     FALSE,
                     ANY_PORT_NUM,
                     PLA_MXR_GET_TYPE_ADJ_PLA_BY_ID_TYPE,
                     (VOID*)(&t_adj_pla_info),
                     &z_size,
                     &pt_vs_obj->t_vs_comp_grp.t_pmx.e_comp_cond );
    if ( i4_ret != SMR_OK )
    {
        return SMR_NOT_ALLOW;
    }

    /* get top */
    for ( ui4_top = PLA_MXR_Z_ORDER_0; ui4_top <= PLA_MXR_Z_ORDER_31; ui4_top++ )
    {
        if ( t_adj_pla_info.ui4_adj_zorder & MAKE_BIT_MASK_32(ui4_top) )
        {
            break;
        }
    }

    /* get bottom */
    for ( ui4_bottom = PLA_MXR_Z_ORDER_31; ui4_bottom != PLA_MXR_Z_ORDER_0; ui4_bottom-- )
    {
        if ( t_adj_pla_info.ui4_adj_zorder & MAKE_BIT_MASK_32(ui4_bottom) )
        {
            break;
        }
    }

    ui4_curr = aui4_pla_order[pt_vs_obj->t_vs_comp_grp.t_pmx.t_info.ui1_in_port];
    if ( ui4_curr < ui4_top || ui4_curr > ui4_bottom )
    {
        return SMR_NOT_ALLOW;
    }

    switch( pt_order_info->e_order_ctrl )
    {
    case VSH_PLANE_ORDER_CTRL_TOP:
        if ( ui4_top < ui4_curr )
        {
            UINT32 ui4_target_z = ui4_curr;

            while ( ui4_target_z > ui4_top )
            {
                /* find target z-order */
                /* Move up to next adjustable z-order */
                for ( ui1_i = (UINT8)(ui4_curr-1); ui1_i != 0; --ui1_i )
                {
                    if ( t_adj_pla_info.ui4_adj_zorder & MAKE_BIT_MASK_32(ui1_i) )
                    {
                        ui4_target_z = ui1_i;
                        break;
                    }
                }

                if ( ui4_target_z < ui4_curr )
                {
                    /* find target port number */
                    for ( ui1_i = 0; ui1_i < (UINT8)ui4_num_pla; ++ui1_i )
                    {
                        if ( aui4_pla_order[ui1_i] == ui4_target_z )
                        {
                            break;
                        }
                    }
                    if ( ui1_i >= (UINT8)ui4_num_pla )
                    {
                        /* not found, just set current port to the target z-order */
                        aui4_pla_order[pt_vs_obj->t_vs_comp_grp.t_pmx.t_info.ui1_in_port] = ui4_target_z;
                        ui4_curr = ui4_target_z;
                        continue;
                    }

                    i4_ret = _sm_video_hdlr_swap_pla_z_order(
                                               &pt_vs_obj->t_vs_comp_grp.t_pmx,
                                               t_adj_pla_info.ui4_adj_zorder,
                                               (UINT8)ui1_i,
                                               aui4_pla_order );
                    if ( i4_ret != SMR_OK )
                    {
                        /* get current order */
                        z_size = sizeof( UINT32 ) * MAX_NUM_PORTS;
                        i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_pmx.h_comp,
                                         FALSE,
                                         ANY_PORT_NUM,
                                         PLA_MXR_GET_TYPE_PLA_ORDER,
                                         (VOID*)(aui4_pla_order),
                                         &z_size,
                                         &pt_vs_obj->t_vs_comp_grp.t_pmx.e_comp_cond );
                        if ( i4_ret != SMR_OK )
                        {
                            return SMR_NOT_ALLOW;
                        }

                        for ( ui1_i = 0; ui1_i < (UINT8)ui4_num_pla; ++ui1_i )
                        {
                            /* Find the port that is not the current used port and is in the top z-order */
                            if ( ui1_i != pt_vs_obj->t_vs_comp_grp.t_pmx.t_info.ui1_in_port &&
                                 aui4_pla_order[ui1_i] == ui4_top )
                            {
                                break;
                            }
                        }
                        if ( ui1_i >= (UINT8)ui4_num_pla )
                        {
                            /* not found, just set current port to the top z-order */
                            aui4_pla_order[pt_vs_obj->t_vs_comp_grp.t_pmx.t_info.ui1_in_port] = ui4_top;
                            break;
                        }

                        i4_ret = _sm_video_hdlr_swap_pla_z_order(
                                                   &pt_vs_obj->t_vs_comp_grp.t_pmx,
                                                   t_adj_pla_info.ui4_adj_zorder,
                                                   (UINT8)ui1_i,
                                                   aui4_pla_order );
                        if ( i4_ret != SMR_OK )
                        {
                            return i4_ret;
                        }
                        ui4_target_z = ui4_top;
                    }
                    else
                    {
                        ui4_curr = ui4_target_z;
                    }
                }
                else
                {
                    break;
                }
            }
        }
        break;

    case VSH_PLANE_ORDER_CTRL_UP:
        {
            UINT8  ui1_z_idx = 0;
            UINT32 ui4_target_z = ui4_curr;

            /* find target z-order */
            /* Move up to next adjustable z-order */
            for ( ui1_z_idx = (UINT8)(ui4_curr-1); ui1_z_idx != 0; --ui1_z_idx )
            {
                if ( t_adj_pla_info.ui4_adj_zorder & MAKE_BIT_MASK_32(ui1_z_idx) )
                {
                    ui4_target_z = ui1_z_idx;
                    break;
                }
            }
            if ( ui4_target_z == ui4_curr )
            {
                return SMR_OK;
            }

            /* find target port number */
            for ( ui1_i = 0; ui1_i < (UINT8)ui4_num_pla; ++ui1_i )
            {
                if ( aui4_pla_order[ui1_i] == ui4_target_z )
                {
                    break;
                }
            }
            if ( ui1_i >= (UINT8)ui4_num_pla )
            {
                /* not found, just set current port to the target z-order */
                aui4_pla_order[pt_vs_obj->t_vs_comp_grp.t_pmx.t_info.ui1_in_port] = ui4_target_z;
                break;
            }

            i4_ret = _sm_video_hdlr_swap_pla_z_order(
                                       &pt_vs_obj->t_vs_comp_grp.t_pmx,
                                       t_adj_pla_info.ui4_adj_zorder,
                                       (UINT8)ui1_i,
                                       aui4_pla_order );
            if ( i4_ret != SMR_OK )
            {
                return i4_ret;
            }
        }
        break;

    case VSH_PLANE_ORDER_CTRL_DOWN:
        {
            UINT32 ui4_target_z = ui4_curr;

            /* find target z-order */
            /* Move down to next adjustable z-order */
            for ( ui1_i = (UINT8)(ui4_curr+1); ui1_i < (UINT8)ui4_num_pla; ++ui1_i )
            {
                if ( t_adj_pla_info.ui4_adj_zorder & MAKE_BIT_MASK_32(ui1_i) )
                {
                    ui4_target_z = ui1_i;
                    break;
                }
            }
            if ( ui4_target_z == ui4_curr )
            {
                return SMR_OK;
            }

            /* find target port number */
            for ( ui1_i = 0; ui1_i < (UINT8)ui4_num_pla; ++ui1_i )
            {
                if ( aui4_pla_order[ui1_i] == ui4_target_z )
                {
                    break;
                }
            }
            if ( ui1_i >= (UINT8)ui4_num_pla )
            {
                /* not found, just set current port to the target z-order */
                aui4_pla_order[pt_vs_obj->t_vs_comp_grp.t_pmx.t_info.ui1_in_port] = ui4_target_z;
                break;
            }

            i4_ret = _sm_video_hdlr_swap_pla_z_order(
                                       &pt_vs_obj->t_vs_comp_grp.t_pmx,
                                       t_adj_pla_info.ui4_adj_zorder,
                                       (UINT8)ui1_i,
                                       aui4_pla_order );
            if ( i4_ret != SMR_OK )
            {
                return i4_ret;
            }
        }
        break;

    case VSH_PLANE_ORDER_CTRL_SWAP:
        {
            for ( ui1_i = 0; ui1_i < (UINT8)ui4_num_pla; ++ui1_i )
            {
                if ( aui4_pla_order[ui1_i] == pt_order_info->u.ui1_layer )
                {
                   break;
                }
            }
            if ( ui1_i >= (UINT8)ui4_num_pla )
            {
                /* not found, just set current port to the target z-order */
                aui4_pla_order[pt_vs_obj->t_vs_comp_grp.t_pmx.t_info.ui1_in_port] = pt_order_info->u.ui1_layer;
                break;
            }

            i4_ret = _sm_video_hdlr_swap_pla_z_order(
                                       &pt_vs_obj->t_vs_comp_grp.t_pmx,
                                       t_adj_pla_info.ui4_adj_zorder,
                                       (UINT8)ui1_i,
                                       aui4_pla_order );
            if ( i4_ret != SMR_OK )
            {
                return i4_ret;
            }
        }
        break;

    case VSH_PLANE_ORDER_CTRL_BOTTOM:
        if ( ui4_bottom > ui4_curr )
        {
            UINT32 ui4_target_z = ui4_curr;

            while ( ui4_target_z < ui4_bottom )
            {
                /* find target z-order */
                /* Move down to next adjustable z-order */
                for ( ui1_i = (UINT8)(ui4_curr+1); ui1_i < (UINT8)ui4_num_pla; ++ui1_i )
                {
                    if ( t_adj_pla_info.ui4_adj_zorder & MAKE_BIT_MASK_32(ui1_i) )
                    {
                        ui4_target_z = ui1_i;
                        break;
                    }
                }
                if ( ui4_target_z > ui4_curr )
                {
                    /* find target port number */
                    for ( ui1_i = 0; ui1_i < (UINT8)ui4_num_pla; ++ui1_i )
                    {
                        if ( aui4_pla_order[ui1_i] == ui4_target_z )
                        {
                            break;
                        }
                    }
                    if ( ui1_i >= (UINT8)ui4_num_pla )
                    {
                        /* not found, just set current port to the target z-order */
                        aui4_pla_order[pt_vs_obj->t_vs_comp_grp.t_pmx.t_info.ui1_in_port] = ui4_target_z;
                        ui4_curr = ui4_target_z;
                        continue;
                    }

                    i4_ret = _sm_video_hdlr_swap_pla_z_order(
                                               &pt_vs_obj->t_vs_comp_grp.t_pmx,
                                               t_adj_pla_info.ui4_adj_zorder,
                                               (UINT8)ui1_i,
                                               aui4_pla_order );
                    if ( i4_ret != SMR_OK )
                    {
                        /* get current order */
                        z_size = sizeof( UINT32 ) * MAX_NUM_PORTS;
                        i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_pmx.h_comp,
                                         FALSE,
                                         ANY_PORT_NUM,
                                         PLA_MXR_GET_TYPE_PLA_ORDER,
                                         (VOID*)(aui4_pla_order),
                                         &z_size,
                                         &pt_vs_obj->t_vs_comp_grp.t_pmx.e_comp_cond );
                        if ( i4_ret != SMR_OK )
                        {
                            return SMR_NOT_ALLOW;
                        }

                        for ( ui1_i = 0; ui1_i < (UINT8)ui4_num_pla; ++ui1_i )
                        {
                            /* Find the port that is not the current used port and is in the bottom z-order */
                            if ( ui1_i != pt_vs_obj->t_vs_comp_grp.t_pmx.t_info.ui1_in_port &&
                                 aui4_pla_order[ui1_i] == ui4_bottom )
                            {
                                break;
                            }
                        }
                        if ( ui1_i >= (UINT8)ui4_num_pla )
                        {
                            /* not found, just set current port to the bottom z-order */
                            aui4_pla_order[pt_vs_obj->t_vs_comp_grp.t_pmx.t_info.ui1_in_port] = ui4_bottom;
                            break;
                        }

                        i4_ret = _sm_video_hdlr_swap_pla_z_order(
                                                   &pt_vs_obj->t_vs_comp_grp.t_pmx,
                                                   t_adj_pla_info.ui4_adj_zorder,
                                                   (UINT8)ui1_i,
                                                   aui4_pla_order );
                        if ( i4_ret != SMR_OK )
                        {
                            return i4_ret;
                        }
                        ui4_target_z = ui4_bottom;
                    }
                    else
                    {
                        ui4_curr = ui4_target_z;
                    }
                }
                else
                {
                    break;
                }
            }
        }
        break;

    default:
        return SMR_NOT_SUPPORT;
    }

    z_size = sizeof(UINT32) * pt_vs_obj->t_vs_comp_grp.t_pmx.t_info.ui1_num_in_port;
    i4_ret = rm_set( pt_vs_obj->t_vs_comp_grp.t_pmx.h_comp,
                  FALSE,
                  ANY_PORT_NUM,
                  PLA_MXR_SET_TYPE_PLA_ORDER,
                  (VOID*)(aui4_pla_order),
                  z_size,
                  &pt_vs_obj->t_vs_comp_grp.t_pmx.e_comp_cond );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INTERNAL_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_set_qv_ctrl
 *
 * Description: <function description>
 *
 * Inputs:  h_stream        References.
 *          pt_vs_obj       References.
 *          pt_order_info   References.
 *          z_pla_order_len References.
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
static INT32 _sm_video_hdlr_set_qv_ctrl(
    HANDLE_T             h_stream,
    SM_VIDEO_STREAM_T*   pt_vs_obj,
    VSH_QV_CTRL_INFO_T*  pt_order_info,
    SIZE_T               z_pla_order_len )
{
    INT32                         i4_ret;
    UINT32                        ui4_i;
    SIZE_T                        z_size;
    VID_PLA_QV_INFO_T             t_drv_qv;

    if ( !pt_order_info )
    {
        return SMR_INV_ARG;
    }

    x_memset( &t_drv_qv, 0, sizeof( t_drv_qv) );

    /* get plane order adjustment */
    z_size = sizeof(VID_PLA_QV_INFO_T);
    i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_vp.h_comp,
                  FALSE,
                  ANY_PORT_NUM,
                  VID_PLA_GET_TYPE_QV_INP,
                  (VOID*)(&t_drv_qv),
                  &z_size,
                  &pt_vs_obj->t_vs_comp_grp.t_vp.e_comp_cond );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INTERNAL_ERROR;
    }

    switch( pt_order_info->e_ctrl )
    {
    case VSH_QV_CTRL_TYPE_ROTATE:
        {
            if ( pt_order_info->u.e_rotate.e_rotate_type ==
                 VSH_QV_ROTATE_TYPE_CLOCKWISE )
            {
                for ( ui4_i = 0; ui4_i < pt_order_info->u.e_rotate.ui1_steps; ui4_i++ )
                {
                    VID_PLA_QV_INP_T e_temp = t_drv_qv.e_tl;

                    t_drv_qv.e_tl = t_drv_qv.e_bl;
                    t_drv_qv.e_bl = t_drv_qv.e_br;
                    t_drv_qv.e_br = t_drv_qv.e_tr;
                    t_drv_qv.e_tr = e_temp;
                }
            }
            else /* VSH_QV_ROTATE_TYPE_ANTI_CLOCKWISE */
            {
                for ( ui4_i = 0; ui4_i < pt_order_info->u.e_rotate.ui1_steps; ui4_i++ )
                {
                    VID_PLA_QV_INP_T e_temp = t_drv_qv.e_tl;

                    t_drv_qv.e_tl = t_drv_qv.e_tr;
                    t_drv_qv.e_tr = t_drv_qv.e_br;
                    t_drv_qv.e_br = t_drv_qv.e_bl;
                    t_drv_qv.e_bl = e_temp;
                }
            }
        }
        break;

    case VSH_QV_CTRL_TYPE_SWAP:
        {
            VID_PLA_QV_INP_T*   pe_qv_inp_2;
            VID_PLA_QV_INP_T    ae_qv_inp_map[4] = {
                VID_PLA_QV_INP_1, VID_PLA_QV_INP_2, VID_PLA_QV_INP_3,
                VID_PLA_QV_INP_4
            };

            if ( pt_vs_obj->t_vs_comp_grp.t_pmx.t_info.ui1_in_port >= 4 )
            {
                return SMR_NOT_ALLOW;
            }

            if ( pt_order_info->u.e_swap_to == VSH_QV_POSITION_TOP_LEFT )
            {
                pe_qv_inp_2 = &t_drv_qv.e_tl;
            }
            else if ( pt_order_info->u.e_swap_to == VSH_QV_POSITION_TOP_RIGHT )
            {
                pe_qv_inp_2 = &t_drv_qv.e_tr;
            }
            else if ( pt_order_info->u.e_swap_to == VSH_QV_POSITION_BOTTOM_LEFT )
            {
                pe_qv_inp_2 = &t_drv_qv.e_bl;
            }
            else if ( pt_order_info->u.e_swap_to == VSH_QV_POSITION_BOTTOM_RIGHT )
            {
                pe_qv_inp_2 = &t_drv_qv.e_br;
            }
            else
            {
                return SMR_NOT_SUPPORT;
            }

            if ( t_drv_qv.e_tl == ae_qv_inp_map[pt_vs_obj->t_vs_comp_grp.t_pmx.t_info.ui1_in_port] )
            {
                SWAP_QV_POS( t_drv_qv.e_tl, (*pe_qv_inp_2) );
            }
            else if ( t_drv_qv.e_tr == ae_qv_inp_map[pt_vs_obj->t_vs_comp_grp.t_pmx.t_info.ui1_in_port] )
            {
                SWAP_QV_POS( t_drv_qv.e_tr, (*pe_qv_inp_2) );
            }
            else if ( t_drv_qv.e_bl == ae_qv_inp_map[pt_vs_obj->t_vs_comp_grp.t_pmx.t_info.ui1_in_port] )
            {
                SWAP_QV_POS( t_drv_qv.e_bl, (*pe_qv_inp_2) );
            }
            else if ( t_drv_qv.e_br == ae_qv_inp_map[pt_vs_obj->t_vs_comp_grp.t_pmx.t_info.ui1_in_port] )
            {
                SWAP_QV_POS( t_drv_qv.e_br, (*pe_qv_inp_2) );
            }
            else
            {
                return SMR_NOT_SUPPORT;
            }
        }
        break;

    default:
        return SMR_NOT_SUPPORT;
    }

    i4_ret = rm_set( pt_vs_obj->t_vs_comp_grp.t_vp.h_comp,
                  FALSE,
                  ANY_PORT_NUM,
                  VID_PLA_SET_TYPE_QV_INP,
                  (VOID*)(&t_drv_qv),
                  sizeof(t_drv_qv),
                  &pt_vs_obj->t_vs_comp_grp.t_vp.e_comp_cond );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INTERNAL_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_vsh_frame_rate_2_drv
 *
 * Description: <function description>
 *
 * Inputs:  e_frame_rate    References.
 *
 * Outputs: -
 *
 * Returns: VID_DEC_SRC_FRAME_RATE_T.
 *
 ----------------------------------------------------------------------------*/
VID_DEC_SRC_FRAME_RATE_T _sm_video_hdlr_vsh_frame_rate_2_drv(
    VSH_SRC_FRAME_RATE_T    e_frame_rate )
{
    switch ( e_frame_rate )
    {
    case VSH_SRC_FRAME_RATE_23_976:
        return VID_DEC_SRC_FRAME_RATE_23_976;

    case VSH_SRC_FRAME_RATE_24:
        return VID_DEC_SRC_FRAME_RATE_24;

    case VSH_SRC_FRAME_RATE_25:
        return VID_DEC_SRC_FRAME_RATE_25;

    case VSH_SRC_FRAME_RATE_29_97:
        return VID_DEC_SRC_FRAME_RATE_29_97;

    case VSH_SRC_FRAME_RATE_30:
        return VID_DEC_SRC_FRAME_RATE_30;

    case VSH_SRC_FRAME_RATE_50:
        return VID_DEC_SRC_FRAME_RATE_50;

    case VSH_SRC_FRAME_RATE_59_94:
        return VID_DEC_SRC_FRAME_RATE_59_94;

    case VSH_SRC_FRAME_RATE_60:
        return VID_DEC_SRC_FRAME_RATE_60;

    case VSH_SRC_FRAME_RATE_120:
        return VID_DEC_SRC_FRAME_RATE_120;

    case VSH_SRC_FRAME_RATE_1:
        return VID_DEC_SRC_FRAME_RATE_1;

    case VSH_SRC_FRAME_RATE_5:
        return VID_DEC_SRC_FRAME_RATE_5;

    case VSH_SRC_FRAME_RATE_8:
        return VID_DEC_SRC_FRAME_RATE_8;

    case VSH_SRC_FRAME_RATE_10:
        return VID_DEC_SRC_FRAME_RATE_10;

    case VSH_SRC_FRAME_RATE_12:
        return VID_DEC_SRC_FRAME_RATE_12;

    case VSH_SRC_FRAME_RATE_15:
        return VID_DEC_SRC_FRAME_RATE_15;

    case VSH_SRC_FRAME_RATE_2:
        return VID_DEC_SRC_FRAME_RATE_2;

    case VSH_SRC_FRAME_RATE_6:
        return VID_DEC_SRC_FRAME_RATE_6;

    case VSH_SRC_FRAME_RATE_48:
        return VID_DEC_SRC_FRAME_RATE_48;

    case VSH_SRC_FRAME_RATE_70:
        return VID_DEC_SRC_FRAME_RATE_70;

    case VSH_SRC_FRAME_RATE_20:
        return VID_DEC_SRC_FRAME_RATE_20;

    case VSH_SRC_FRAME_RATE_VARIABLE:
        return VID_DEC_SRC_FRAME_RATE_VARIABLE;

    default:
        return VID_DEC_SRC_FRAME_RATE_UNKNOWN;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_set
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
static INT32 _sm_video_hdlr_set(
    HANDLE_T       h_stream,
    SM_SET_TYPE_T  e_set_type,
    VOID*          pv_set_info,
    SIZE_T         z_set_info_len )
{
    INT32                 i4_ret;
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;

    HANDLE_T              h_drv = NULL_HANDLE;
    RM_COND_T*            pe_drv_cond = NULL;
    DRV_SET_TYPE_T        e_drv_set_type = 0;
    VOID*                 pv_drv_set_info = NULL;
    SIZE_T                z_drv_set_info_len = 0;
    VID_PLA_VID_REGION_T  t_drv_region;
    VID_PLA_ENHANCE_T     t_drv_enhance;
    TV_DEC_VGA_TIMING_OPTION_T t_drv_vga_timing_opt;
    VID_DEC_SEQUENCE_INFO_T t_drv_seq_info;
    VID_DEC_START_RENDER_INFO_T t_drv_start_render_info; 
#ifdef DIVX_PLUS_CER
    VID_PLA_VID_CHAP_T    t_drv_chap = {0}; 
#endif

    pt_vs_obj = (SM_VIDEO_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_vs_obj )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_set_type )
    {
    case SM_VSH_SET_TYPE_CAPTURE:
        {
            return SMR_NOT_SUPPORT;
        }
    
    case SM_VSH_SET_TYPE_DISP_REGION:
        {
            VSH_REGION_INFO_T*       pt_region_info = pv_set_info;

            if ( !pv_set_info )
            {
                return SMR_INV_ARG;
            }

            SM_VS_DBG_INFO((
                "[SM] VS, DISP_REGION( x,y,w,h ) = ( %d, %d, %d, %d)\r\n",
                pt_region_info->ui4_x,
                pt_region_info->ui4_y,
                pt_region_info->ui4_width,
                pt_region_info->ui4_height ));

            VIDEO_HDLR_CONVERT_TO_DRV_REGION( t_drv_region, (*pt_region_info) );

            h_drv = pt_vs_obj->t_vs_comp_grp.t_vp.h_comp;
            pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_vp.e_comp_cond;
            e_drv_set_type = VID_PLA_SET_TYPE_DISP_REGION;
            pv_drv_set_info = (VOID*)&t_drv_region;
            z_drv_set_info_len = sizeof( VID_PLA_VID_REGION_T );
        }
        break;

    case SM_VSH_SET_TYPE_VIDEO_REGION:
        {
            VSH_REGION_INFO_T*       pt_region_info = pv_set_info;

            if ( !pv_set_info )
            {
                return SMR_INV_ARG;
            }

            SM_VS_DBG_INFO((
                "[SM] VS, SRC_REGION( x,y,w,h ) = ( %d, %d, %d, %d)\r\n",
                pt_region_info->ui4_x,
                pt_region_info->ui4_y,
                pt_region_info->ui4_width,
                pt_region_info->ui4_height ));

            VIDEO_HDLR_CONVERT_TO_DRV_REGION( t_drv_region, (*pt_region_info) );

            h_drv = pt_vs_obj->t_vs_comp_grp.t_vp.h_comp;
            pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_vp.e_comp_cond;
            e_drv_set_type = VID_PLA_SET_TYPE_SRC_REGION;
            pv_drv_set_info = (VOID*)&t_drv_region;
            z_drv_set_info_len = sizeof( VID_PLA_VID_REGION_T );
        }
        break;

    case SM_VSH_SET_TYPE_ENHANCEMENT:
        {
            VSH_ENCHANCE_INFO_T*  pt_enhance_info = pv_set_info;

            if ( !pv_set_info )
            {
                return SMR_INV_ARG;
            }

            VIDEO_HDLR_CONVERT_TO_DRV_ENHANCE( t_drv_enhance, (*pt_enhance_info) );

            h_drv = pt_vs_obj->t_vs_comp_grp.t_vp.h_comp;
            pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_vp.e_comp_cond;
            e_drv_set_type = VID_PLA_SET_TYPE_ENHANCE;
            pv_drv_set_info = (VOID*)&t_drv_enhance;
            z_drv_set_info_len = sizeof( VID_PLA_ENHANCE_T );
        }
        break;

    case SM_VSH_SET_TYPE_PLANE_ORDER:
        x_scc_disp_pmx_lock();
        i4_ret = _sm_video_hdlr_set_pla_order( h_stream,
                                            pt_vs_obj,
                                            (VSH_SET_PLANE_ORDER_INFO_T*)pv_set_info,
                                            z_set_info_len );
        x_scc_disp_pmx_unlock();
        return i4_ret;

    case SM_VSH_SET_TYPE_BLENDING:
        {
            VSH_BLENDING_INFO_T t_blending = (VSH_BLENDING_INFO_T)((UINT32)pv_set_info);
            VID_PLA_BLENDING_T  t_drv_blending = t_blending;

            h_drv = pt_vs_obj->t_vs_comp_grp.t_vp.h_comp;
            pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_vp.e_comp_cond;
            e_drv_set_type = VID_PLA_SET_TYPE_BLENDING;
            pv_drv_set_info = (VOID*)((UINT32)t_drv_blending);
            z_drv_set_info_len = sizeof( VID_PLA_BLENDING_T );
        }
        break;

    case SM_VSH_SET_TYPE_QV_CTRL:
        i4_ret = _sm_video_hdlr_set_qv_ctrl(
            h_stream,
            pt_vs_obj,
            (VSH_QV_CTRL_INFO_T*)pv_set_info,
            z_set_info_len );
        return i4_ret;

    case SM_VSH_SET_TYPE_AUTO_CLK_PHS_POS:
        {
            if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
            {
                return SMR_NOT_ALLOW;
            }

            h_drv = pt_vs_obj->t_vs_comp_grp.t_dec.h_comp;
            pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond;
            e_drv_set_type = TV_DEC_SET_TYPE_AUTO_CLK_PHS_POS;
            pv_drv_set_info = (VOID*)NULL;
            z_drv_set_info_len = 0;
        }
        break;

    case SM_VSH_SET_TYPE_AUTO_COLOR:
        {
            if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
            {
                return SMR_NOT_ALLOW;
            }

            h_drv = pt_vs_obj->t_vs_comp_grp.t_dec.h_comp;
            pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond;
            e_drv_set_type = TV_DEC_SET_TYPE_AUTO_COLOR;
            pv_drv_set_info = (VOID*)NULL;
            z_drv_set_info_len = 0;
        }
        break;

    case SM_VSH_SET_TYPE_VID_CLOCK:
        {
            if ( z_set_info_len != sizeof( INT16 ) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
            {
                return SMR_NOT_ALLOW;
            }

            h_drv = pt_vs_obj->t_vs_comp_grp.t_dec.h_comp;
            pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond;
            e_drv_set_type = TV_DEC_SET_TYPE_CLOCK;
            pv_drv_set_info = (VOID*)pv_set_info;
            z_drv_set_info_len = sizeof( INT16 );
        }
        break;

    case SM_VSH_SET_TYPE_VID_PHASE:
        {
            if ( z_set_info_len != sizeof( UINT16 ) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
            {
                return SMR_NOT_ALLOW;
            }

            h_drv = pt_vs_obj->t_vs_comp_grp.t_dec.h_comp;
            pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond;
            e_drv_set_type = TV_DEC_SET_TYPE_PHASE;
            pv_drv_set_info = (VOID*)pv_set_info;
            z_drv_set_info_len = sizeof( UINT16 );
        }
        break;

    case SM_VSH_SET_TYPE_VID_H_POS:
        {
            if ( z_set_info_len != sizeof( UINT16 ) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
            {
                return SMR_NOT_ALLOW;
            }

            h_drv = pt_vs_obj->t_vs_comp_grp.t_dec.h_comp;
            pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond;
            e_drv_set_type = TV_DEC_SET_TYPE_H_POSITION;
            pv_drv_set_info = (VOID*)pv_set_info;
            z_drv_set_info_len = sizeof( UINT16 );
        }
        break;

    case SM_VSH_SET_TYPE_VID_V_POS:
        {
            if ( z_set_info_len != sizeof( UINT16 ) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
            {
                return SMR_NOT_ALLOW;
            }

            h_drv = pt_vs_obj->t_vs_comp_grp.t_dec.h_comp;
            pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond;
            e_drv_set_type = TV_DEC_SET_TYPE_V_POSITION;
            pv_drv_set_info = (VOID*)pv_set_info;
            z_drv_set_info_len = sizeof( UINT16 );
        }
        break;

    case SM_VSH_SET_TYPE_VBI_NFY:
        {
            BOOL                b_success;
            SM_VSH_VBI_NFY_T*   pt_vbi_nfy = (SM_VSH_VBI_NFY_T*)pv_set_info;

            if ( z_set_info_len < sizeof( SM_VSH_VBI_NFY_T ) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            if ( t_vsh_vbi_psr_fct_tbl.e_vbi_type == SM_VSH_VBI_TYPE_UNKNOWN )
            {
                return SMR_NOT_SUPPORT;
            }

            SM_VS_DBG_INFO(( "[SM] VS, svctx set VBI notify to %x%.8X\r\n",
                pt_vbi_nfy->pf_vbi_nfy ));

            if ( !pt_vbi_nfy || !pt_vbi_nfy->pf_vbi_nfy )
            {
                if ( pt_vs_obj->t_vbi_nfy.pf_vbi_nfy )
                {
                    SM_ASSERT( t_vsh_vbi_psr_fct_tbl.pf_vbi_close );

                    /* Close VBI parser */
                    b_success = t_vsh_vbi_psr_fct_tbl.pf_vbi_close(
                        pt_vs_obj->t_vbi_psr_id );
                    if ( !b_success )
                    {
                        SM_ASSERT( 0 );
                        pt_vs_obj->t_vbi_psr_id = 0xFFFFFFFF;
                        return SMR_INTERNAL_ERROR;
                    }
                    pt_vs_obj->t_vbi_psr_id = 0xFFFFFFFF;
                }
            }
            else
            {
                if ( !pt_vs_obj->t_vbi_nfy.pf_vbi_nfy )
                {
                    SM_ASSERT( t_vsh_vbi_psr_fct_tbl.pf_vbi_open );

                    /* Open VBI parser */
                    b_success = t_vsh_vbi_psr_fct_tbl.pf_vbi_open(
                        h_stream,
                        &pt_vs_obj->t_vbi_psr_id );
                    if ( !b_success )
                    {
                        SM_ASSERT( 0 );
                        return SMR_INTERNAL_ERROR;
                    }

                    if ( !pt_vs_obj->b_is_timer_active )
                    {
                        _sm_video_hdlr_start_cc_ind_timer(
                            h_stream, pt_vs_obj );
                    }
                }
            }
            if ( pt_vbi_nfy )
            {
                pt_vs_obj->t_vbi_nfy = *pt_vbi_nfy;
            }
        }
        return SMR_OK;

    case SM_VSH_SET_TYPE_VBI_REF_INFO:
        {
            SM_VSH_VBI_REF_INFO_T*   pt_vbi_ref =
                (SM_VSH_VBI_REF_INFO_T*)pv_set_info;
            SM_CCH_NTSC_CC_VBI_PSR_CB_INFO_T t_cch_vbi_cb;

            if ( z_set_info_len < sizeof( SM_VSH_VBI_REF_INFO_T ) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            if ( pt_vbi_ref->e_vbi_ref_type !=
                 SM_VSH_VBI_REF_INFO_TYPE_CC_STRM_HDL )
            {
                return SMR_NOT_SUPPORT;
            }
            if ( !IS_MPEG_VID_STRM(pt_vs_obj) )
            {
                return SMR_OK;
            }

            pt_vs_obj->h_cc_strm = pt_vbi_ref->u.h_cc_strm;

            t_cch_vbi_cb.pv_tag = (VOID*)h_stream;
            t_cch_vbi_cb.pf_vbi_psr_cb = _sm_video_hdlr_cch_vbi_psr_cb_fct;

            i4_ret = x_sm_set( pt_vs_obj->h_cc_strm,
                SM_CCH_SET_TYPE_NTSC_CC_VBI_PSR_CB,
                &t_cch_vbi_cb,
                sizeof(SM_CCH_NTSC_CC_VBI_PSR_CB_INFO_T) );
            if ( i4_ret != SMR_OK )
            {
                SM_VS_DBG_ERR((
                    "[SM] VS, set SM_CCH_SET_TYPE_NTSC_CC_VBI_PSR_CB fail, i4_ret = %d\r\n",
                    i4_ret ));
            }
        }
        return SMR_OK;

    case SM_VSH_SET_TYPE_VGA_TIMING_OPTION:
        {
            VSH_VGA_TIMING_OPTION_T* pt_vga_timing_opt = (VSH_VGA_TIMING_OPTION_T*)pv_set_info;

            if ( !pv_set_info )
            {
                return SMR_INV_ARG;
            }

            t_drv_vga_timing_opt.ui2_opt_index = pt_vga_timing_opt->ui2_opt_index;
            t_drv_vga_timing_opt.b_is_default = pt_vga_timing_opt->b_is_default;

            h_drv = pt_vs_obj->t_vs_comp_grp.t_dec.h_comp;
            pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond;
            e_drv_set_type = TV_DEC_SET_TYPE_VGA_TIMING_OPTION;
            pv_drv_set_info = (VOID*)&t_drv_vga_timing_opt;
            z_drv_set_info_len = sizeof( TV_DEC_VGA_TIMING_OPTION_T );
        }
        break;

    case SM_VSH_SET_TYPE_COLOR_SYS:
        {
            VSH_COLOR_SYS_TYPE_T e_vsh_color_sys = (VSH_COLOR_SYS_TYPE_T)(UINT32)pv_set_info;
            TV_DEC_COLOR_SYS_T e_drv_tvd_color_sys;

            switch( e_vsh_color_sys )
            {
            case VSH_COLOR_SYS_TYPE_UNKNOWN:
                e_drv_tvd_color_sys = TV_DEC_COLOR_SYS_UNKNOWN;
                break;
            case VSH_COLOR_SYS_TYPE_NTSC:
                e_drv_tvd_color_sys = TV_DEC_COLOR_SYS_NTSC;
                break;
            case VSH_COLOR_SYS_TYPE_NTSC_443:
                e_drv_tvd_color_sys = TV_DEC_COLOR_SYS_NTSC_443;
                break;
            case VSH_COLOR_SYS_TYPE_PAL:
                e_drv_tvd_color_sys = TV_DEC_COLOR_SYS_PAL;
                break;
            case VSH_COLOR_SYS_TYPE_PAL_N:
                e_drv_tvd_color_sys = TV_DEC_COLOR_SYS_PAL_N;
                break;
            case VSH_COLOR_SYS_TYPE_PAL_M:
                e_drv_tvd_color_sys = TV_DEC_COLOR_SYS_PAL_M;
                break;
            case VSH_COLOR_SYS_TYPE_PAL_60:
                e_drv_tvd_color_sys = TV_DEC_COLOR_SYS_PAL_60;
                break;
            case VSH_COLOR_SYS_TYPE_SECAM:
                e_drv_tvd_color_sys = TV_DEC_COLOR_SYS_SECAM;
                break;
            default:
                return SMR_INV_SET_INFO;
            }

            h_drv = pt_vs_obj->t_vs_comp_grp.t_dec.h_comp;
            pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond;
            e_drv_set_type = TV_DEC_SET_TYPE_COLOR_SYS;
            pv_drv_set_info = (VOID*)(UINT32)e_drv_tvd_color_sys;
            z_drv_set_info_len = sizeof( TV_DEC_COLOR_SYS_T );
        }
        break;

    case SM_VSH_SET_TYPE_SEQUENCE_INFO:
        {
            VSH_SEQUENCE_INFO_T* pt_vsh_seq_info = (VSH_SEQUENCE_INFO_T*)pv_set_info;
            VID_DEC_SEQUENCE_INFO_T* pt_drv_seq_info = &t_drv_seq_info;

            if ( !IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
            {
                return SMR_NOT_ALLOW;
            }
            if ( !pv_set_info )
            {
                return SMR_INV_ARG;
            }

#if SUPPORT_MULTI_INFO
            x_memset(pt_drv_seq_info, 0, sizeof(VID_DEC_SEQUENCE_INFO_T));
            while(pt_vsh_seq_info)
            {
#endif
                pt_drv_seq_info->b_is_raw = pt_vsh_seq_info->b_is_raw;
                pt_drv_seq_info->b_is_second_raw = pt_vsh_seq_info->b_is_second_raw;
                if ( pt_vsh_seq_info->b_is_raw )
                {
                    pt_drv_seq_info->u.t_seq_raw.pc_buff = pt_vsh_seq_info->u.t_seq_raw.pc_buff;
                    pt_drv_seq_info->u.t_seq_raw.z_buff_size = pt_vsh_seq_info->u.t_seq_raw.z_buff_size;
                    pt_drv_seq_info->u.t_seq_raw.e_frame_rate =
                        _sm_video_hdlr_vsh_frame_rate_2_drv( pt_vsh_seq_info->u.t_seq_raw.e_frame_rate );
                }
                else
                {
                    pt_drv_seq_info->u.t_seq_data.ui4_seq_hdr    = pt_vsh_seq_info->u.t_seq_data.ui4_seq_hdr;
                    pt_drv_seq_info->u.t_seq_data.ui2_seq_hdr_sz = pt_vsh_seq_info->u.t_seq_data.ui2_seq_hdr_sz;
                    pt_drv_seq_info->u.t_seq_data.ui2_width = pt_vsh_seq_info->u.t_seq_data.ui2_width;
                    pt_drv_seq_info->u.t_seq_data.ui2_height = pt_vsh_seq_info->u.t_seq_data.ui2_height;
                    pt_drv_seq_info->u.t_seq_data.ui2_asp_usr_w = pt_vsh_seq_info->u.t_seq_data.ui2_asp_usr_w;
                    pt_drv_seq_info->u.t_seq_data.ui2_asp_usr_h = pt_vsh_seq_info->u.t_seq_data.ui2_asp_usr_h;
                    pt_drv_seq_info->u.t_seq_data.b_chk_framerate = pt_vsh_seq_info->u.t_seq_data.b_chk_framerate;
                    
                    switch ( pt_vsh_seq_info->u.t_seq_data.e_asp_usr_type )
                    {
                    case VSH_ASP_USR_TYPE_SRC:
                        pt_drv_seq_info->u.t_seq_data.e_asp_usr_type = VID_DEC_ASP_USR_TYPE_SRC;
                        break;
                    case VSH_ASP_USR_TYPE_DISP:
                    default:
                        pt_drv_seq_info->u.t_seq_data.e_asp_usr_type = VID_DEC_ASP_USR_TYPE_DISP;
                        break;
                    }                    
                    pt_drv_seq_info->u.t_seq_data.e_frame_rate =
                        _sm_video_hdlr_vsh_frame_rate_2_drv( pt_vsh_seq_info->u.t_seq_data.e_frame_rate );
                    switch ( pt_vsh_seq_info->u.t_seq_data.e_src_asp )
                    {
                    case VSH_SRC_ASPECT_RATIO_4_3:
                        pt_drv_seq_info->u.t_seq_data.e_src_asp = VID_DEC_SRC_ASPECT_RATIO_4_3;
                        break;
                
                    case VSH_SRC_ASPECT_RATIO_16_9:
                        pt_drv_seq_info->u.t_seq_data.e_src_asp = VID_DEC_SRC_ASPECT_RATIO_16_9;
                        break;
                
                    case VSH_SRC_ASPECT_RATIO_2_21_1:
                        pt_drv_seq_info->u.t_seq_data.e_src_asp = VID_DEC_SRC_ASPECT_RATIO_2_21_1;
                        break;
                
                    case VSH_SRC_ASPECT_RATIO_PXL_10_11:
                        pt_drv_seq_info->u.t_seq_data.e_src_asp = VID_DEC_SRC_ASPECT_RATIO_PXL_10_11;
                        break;
                
                    case VSH_SRC_ASPECT_RATIO_PXL_40_33:
                        pt_drv_seq_info->u.t_seq_data.e_src_asp = VID_DEC_SRC_ASPECT_RATIO_PXL_40_33;
                        break;
                
                    case VSH_SRC_ASPECT_RATIO_PXL_16_11:
                        pt_drv_seq_info->u.t_seq_data.e_src_asp = VID_DEC_SRC_ASPECT_RATIO_PXL_16_11;
                        break;
                
                    case VSH_SRC_ASPECT_RATIO_PXL_12_11:
                        pt_drv_seq_info->u.t_seq_data.e_src_asp = VID_DEC_SRC_ASPECT_RATIO_PXL_12_11;
                        break;
                
                    case VSH_SRC_ASPECT_RATIO_PXL_3_2:
                        pt_drv_seq_info->u.t_seq_data.e_src_asp = VID_DEC_SRC_ASPECT_RATIO_PXL_3_2;
                        break;
                
                    case VSH_SRC_ASPECT_RATIO_PXL_1_1:
                        pt_drv_seq_info->u.t_seq_data.e_src_asp = VID_DEC_SRC_ASPECT_RATIO_PXL_1_1;
                        break;
                
                    case VSH_SRC_ASPECT_RATIO_USR:
                        pt_drv_seq_info->u.t_seq_data.e_src_asp = VID_DEC_SRC_ASPECT_RATIO_USR;
                        break;
                        
                    default:
                        pt_drv_seq_info->u.t_seq_data.e_src_asp = VID_DEC_SRC_ASPECT_RATIO_UNKNOWN;
                    }
                }
#if SUPPORT_MULTI_INFO
                pt_vsh_seq_info = pt_vsh_seq_info->pt_next_scale;
                if(pt_vsh_seq_info)
                {
                    pt_drv_seq_info->pt_next_scale = (VID_DEC_SEQUENCE_INFO_T*)
                        x_mem_alloc(sizeof(VID_DEC_SEQUENCE_INFO_T));
                    if(!pt_drv_seq_info->pt_next_scale)
                    {
                        pt_drv_seq_info = t_drv_seq_info.pt_next_scale;
                        while(pt_drv_seq_info)
                        {
                            pv_drv_set_info = pt_drv_seq_info->pt_next_scale;
                            x_mem_free(pt_drv_seq_info);
                            pt_drv_seq_info = pv_drv_set_info;
                        }
                        return SMR_NOT_ALLOW;
                    }
                    pt_drv_seq_info = pt_drv_seq_info->pt_next_scale;
                    x_memset(pt_drv_seq_info, 0, sizeof(VID_DEC_SEQUENCE_INFO_T));
                }
            }
#endif

            h_drv = pt_vs_obj->t_vs_comp_grp.t_dec.h_comp;
            pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond;
            e_drv_set_type = VID_DEC_SET_TYPE_SEQUENCE_INFO;
            pv_drv_set_info = (VOID*)&t_drv_seq_info;
            z_drv_set_info_len = sizeof( VID_DEC_SEQUENCE_INFO_T );
        }
        break;

    case SM_VSH_SET_TYPE_SPEED:
        {
            if ( !IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
            {
                return SMR_NOT_ALLOW;
            }
            if ( !pv_set_info )
            {
                return SMR_INV_ARG;
            }

            h_drv = pt_vs_obj->t_vs_comp_grp.t_dec.h_comp;
            pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond;
            e_drv_set_type = VID_DEC_SET_TYPE_SPEED;
            pv_drv_set_info = (VOID*)pv_set_info;
            z_drv_set_info_len = sizeof( MM_SPEED_TYPE_T );
        }
        break;
    case SM_VSH_SET_TYPE_FLUSH:
    {
            if ( !pv_set_info )
            {
                return SMR_INV_ARG;
            }
            h_drv = pt_vs_obj->t_vs_comp_grp.t_dec.h_comp;
            pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond;
            e_drv_set_type = VID_DEC_SET_TYPE_FLUSH;
            pv_drv_set_info = (VOID*)pv_set_info;
            z_drv_set_info_len = sizeof( BOOL );
        }
        break;
    case SM_VSH_SET_TYPE_START_RENDER_INFO:
       {
            SM_VSH_START_RENDER_INFO_T*  pt_start_render_info = pv_set_info;

            if ( !pv_set_info )
            {
                return SMR_INV_ARG;
            }
            h_drv = pt_vs_obj->t_vs_comp_grp.t_vp.h_comp;
            pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_vp.e_comp_cond;
            e_drv_set_type = VID_PLA_SET_TYPE_ENHANCE;
            t_drv_start_render_info.b_auto_pause = pt_start_render_info->b_auto_pause;
            t_drv_start_render_info.t_pts = pt_start_render_info->t_pts;
            pv_drv_set_info = (VOID*)&t_drv_start_render_info;
            z_drv_set_info_len = sizeof( VID_DEC_START_RENDER_INFO_T );
        }
        break;

#ifdef DIVX_PLUS_CER 
   case SM_VSH_SET_TYPE_CHAPTER: 
        {
            VSH_CHAP_INFO_T*       pt_info = pv_set_info;

            if ( pv_set_info == NULL )
            {
                return SMR_INV_ARG;
            }

            t_drv_chap.b_seek = pt_info->b_seek;
            t_drv_chap.b_first = pt_info->b_first;
            t_drv_chap.ui8_start_time = pt_info->ui8_start_time;
            t_drv_chap.ui8_end_time = pt_info->ui8_end_time;

            h_drv = pt_vs_obj->t_vs_comp_grp.t_vp.h_comp;
            pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_vp.e_comp_cond;
            e_drv_set_type = VID_PLA_SET_TYPE_CHAP_INFO;
            pv_drv_set_info = (VOID*)&t_drv_chap;
            z_drv_set_info_len = sizeof( VID_PLA_VID_CHAP_T );
        }
        break;
#endif

    case SM_VSH_SET_TYPE_VDEC_REPLAY:
        {
            if ( !IS_MPEG_VID_STRM(pt_vs_obj) && !IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
            {
                return SMR_NOT_ALLOW;
            }

            h_drv = pt_vs_obj->t_vs_comp_grp.t_dec.h_comp;
            pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond;
            e_drv_set_type = VID_DEC_SET_TYPE_VDEC_REPLAY;
            pv_drv_set_info = (VOID*)NULL;
            z_drv_set_info_len = 0;
        }
        break;
        
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
#if SUPPORT_MULTI_INFO
    if(e_set_type == SM_VSH_SET_TYPE_SEQUENCE_INFO)
    {
        VID_DEC_SEQUENCE_INFO_T* pt_drv_seq_info = pv_drv_set_info;
        pt_drv_seq_info = pt_drv_seq_info->pt_next_scale;
        while(pt_drv_seq_info)
        {
            pv_drv_set_info = pt_drv_seq_info->pt_next_scale;
            x_mem_free(pt_drv_seq_info);
            pt_drv_seq_info = pv_drv_set_info;
        }
    }
#endif
    if ( i4_ret != RMR_OK && i4_ret != RMR_ASYNC_NFY )
    {
        SM_VS_DBG_ERR(("[SM] VS, _sm_video_hdlr_set() fail, set type( 0x%.8X ), i4_ret = %d\r\n",
                    e_set_type, i4_ret ));
        return SMR_INTERNAL_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_tunl_set
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
 *          SMR_INTERNAL_ERROR  Fail.
 *          SMR_INV_HANDLE      Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sm_video_hdlr_tunl_set(
    HANDLE_T       h_stream,
    SM_SET_TYPE_T  e_set_type,
    VOID*          pv_set_info,
    SIZE_T         z_set_info_len )
{
    INT32                 i4_ret;
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;
    HANDLE_T              h_drv;
    RM_COND_T*            pe_drv_cond;

    pt_vs_obj = (SM_VIDEO_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_vs_obj )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_set_type )
    {
    case SM_GRP_TUNL_DEC:
        h_drv = pt_vs_obj->t_vs_comp_grp.t_dec.h_comp;
        pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond;
        break;

    case SM_GRP_TUNL_VIDEO_PLANE:
        h_drv = pt_vs_obj->t_vs_comp_grp.t_vp.h_comp;
        pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_vp.e_comp_cond;
        break;

    case SM_GRP_TUNL_PLANE_MIXER:
        h_drv = pt_vs_obj->t_vs_comp_grp.t_pmx.h_comp;
        pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_pmx.e_comp_cond;
        break;

    case SM_GRP_TUNL_TV_ENC:
        h_drv = pt_vs_obj->t_vs_comp_grp.t_tve.h_comp;
        pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_tve.e_comp_cond;
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
 * Name: _sm_video_hdlr_slctr_get
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
static INT32 _sm_video_hdlr_slctr_get(
    HANDLE_T       h_stream,
    SM_GET_TYPE_T  e_get_type,
    VOID*          pv_get_info,
    SIZE_T*        pz_get_info_len )
{
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;

    if ( SM_GET_GRP(e_get_type)==SM_GRP_SESS )
    {
        return SMR_NOT_SUPPORT;
    }

    pt_vs_obj = (SM_VIDEO_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_vs_obj )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_get_type )
    {
    case SM_STRM_GET_TYPE_STATE:
        *((SM_COND_T*)pv_get_info) = pt_vs_obj->t_base.e_condition;
        *pz_get_info_len = sizeof(pt_vs_obj->t_base.e_condition);
        break;

    case SM_STRM_GET_TYPE_ID:
        if ( *pz_get_info_len < sizeof( STREAM_COMP_ID_T ) )
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        *((STREAM_COMP_ID_T*)pv_get_info) = pt_vs_obj->t_base.t_strm_comp_id;
        *pz_get_info_len = sizeof(STREAM_COMP_ID_T);
        break;

    case SM_STRM_GET_TYPE_SCDB_REC:
        if ( *pz_get_info_len < sizeof( SCDB_REC_T ) )
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        *((SCDB_REC_T*)pv_get_info) = pt_vs_obj->t_base.t_scdb_rec;
        *pz_get_info_len = sizeof(SCDB_REC_T);
        break;

    default:
        return SMR_NOT_SUPPORT;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_get
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
static INT32 _sm_video_hdlr_get(
    HANDLE_T       h_stream,
    SM_GET_TYPE_T  e_get_type,
    VOID*          pv_get_info,
    SIZE_T*        pz_get_info_len )
{
    INT32                 i4_ret;
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;

    pt_vs_obj = (SM_VIDEO_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_vs_obj )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_get_type )
    {
    case SM_VSH_GET_TYPE_FINAL_DISP_REGION:
    case SM_VSH_GET_TYPE_FINAL_VIDEO_REGION:
    case SM_VSH_GET_TYPE_DISP_REGION:
    case SM_VSH_GET_TYPE_VIDEO_REGION:
        {
            VID_PLA_VID_REGION_T t_drv_region;
            SIZE_T               z_drv_region_len = sizeof(VID_PLA_VID_REGION_T);
            VSH_REGION_INFO_T*   pt_vsh_region = (VSH_REGION_INFO_T*)pv_get_info;
            DRV_GET_TYPE_T       e_drv_get_type;

            if ( !pt_vsh_region || !pz_get_info_len ||
                 *pz_get_info_len < sizeof(VSH_REGION_INFO_T) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }

            if ( e_get_type == SM_VSH_GET_TYPE_DISP_REGION )
            {
                e_drv_get_type = VID_PLA_GET_TYPE_DISP_REGION;
            }
            else if ( e_get_type == SM_VSH_GET_TYPE_VIDEO_REGION )
            {
                e_drv_get_type = VID_PLA_GET_TYPE_SRC_REGION;
            }
            else if ( e_get_type == SM_VSH_GET_TYPE_FINAL_DISP_REGION )
            {
                e_drv_get_type = VID_PLA_GET_TYPE_FINAL_DISP_REGION;
            }
            else if ( e_get_type == SM_VSH_GET_TYPE_FINAL_VIDEO_REGION )
            {
                e_drv_get_type = VID_PLA_GET_TYPE_FINAL_SRC_REGION;
            }
            else
            {
                return SMR_INV_ARG;
            }

            VIDEO_HDLR_CONVERT_TO_DRV_REGION( t_drv_region, (*pt_vsh_region) )

            i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_vp.h_comp,
                             FALSE,
                             pt_vs_obj->t_vs_comp_grp.t_vp.t_info.ui1_in_port,
                             e_drv_get_type,
                             (VOID*)&t_drv_region,
                             &z_drv_region_len,
                             &pt_vs_obj->t_vs_comp_grp.t_vp.e_comp_cond );
            if ( i4_ret != RMR_OK )
            {
                return SMR_NOT_ALLOW;
            }

            VIDEO_HDLR_CONVERT_TO_VSH_REGION( (*pt_vsh_region), t_drv_region )
            *pz_get_info_len = sizeof(VSH_REGION_INFO_T);
        }
        break;

    case SM_VSH_GET_TYPE_ENHANCEMENT:
        {
            VID_PLA_ENHANCE_T    t_drv_enh;
            SIZE_T               z_drv_enh_len = sizeof(VID_PLA_ENHANCE_T);
            VSH_ENCHANCE_INFO_T* pt_vsh_enh = (VSH_ENCHANCE_INFO_T*)pv_get_info;

            if ( !pt_vsh_enh || !pz_get_info_len ||
                 *pz_get_info_len < sizeof(VSH_ENCHANCE_INFO_T) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }

            i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_vp.h_comp,
                             FALSE,
                             pt_vs_obj->t_vs_comp_grp.t_vp.t_info.ui1_in_port,
                             VID_PLA_GET_TYPE_ENHANCE,
                             (VOID*)&t_drv_enh,
                             &z_drv_enh_len,
                             &pt_vs_obj->t_vs_comp_grp.t_vp.e_comp_cond );
            if ( i4_ret != RMR_OK )
            {
                return SMR_NOT_ALLOW;
            }

            VIDEO_HDLR_CONVERT_TO_VSH_ENHANCE( (*pt_vsh_enh), t_drv_enh )
            *pz_get_info_len = sizeof(VSH_ENCHANCE_INFO_T);
        }
        break;

    case SM_VSH_GET_TYPE_PLANE_ORDER:
        {
            UINT32                       aui4_pla_order[32];
            SIZE_T                       z_size = sizeof( UINT32 ) * MAX_NUM_PORTS;
            VSH_GET_PLANE_ORDER_INFO_T*  pt_vsh_pla_order = (VSH_GET_PLANE_ORDER_INFO_T*)pv_get_info;

            if ( !pt_vsh_pla_order || !pz_get_info_len ||
                 *pz_get_info_len < sizeof(VSH_GET_PLANE_ORDER_INFO_T) )
            {
                return SMR_INV_GET_INFO;
            }

            /* get current order */
            i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_pmx.h_comp,
                             FALSE,
                             ANY_PORT_NUM,
                             PLA_MXR_GET_TYPE_PLA_ORDER,
                             (VOID*)(aui4_pla_order),
                             &z_size,
                             &pt_vs_obj->t_vs_comp_grp.t_pmx.e_comp_cond );
            if ( i4_ret != SMR_OK )
            {
                return SMR_NOT_ALLOW;
            }

            pt_vsh_pla_order->ui1_num_layers = (UINT8)(z_size / sizeof(UINT32));

            if ( pt_vs_obj->t_vs_comp_grp.t_pmx.t_info.ui1_in_port >= pt_vsh_pla_order->ui1_num_layers ||
                 pt_vs_obj->t_vs_comp_grp.t_pmx.t_info.ui1_in_port >= 32 )
            {
                return SMR_INTERNAL_ERROR;
            }

            pt_vsh_pla_order->ui1_curr_layer = (UINT8)aui4_pla_order[pt_vs_obj->t_vs_comp_grp.t_pmx.t_info.ui1_in_port];
            *pz_get_info_len = sizeof(VSH_GET_PLANE_ORDER_INFO_T);
        }
        break;

    case SM_VSH_GET_TYPE_BLENDING:
        {
            VID_PLA_BLENDING_T    t_drv_blending;
            SIZE_T                z_size = sizeof( VID_PLA_BLENDING_T );
            VSH_BLENDING_INFO_T*  pt_vsh_blending = (VSH_BLENDING_INFO_T*)pv_get_info;

            if ( !pt_vsh_blending || !pz_get_info_len ||
                 *pz_get_info_len < sizeof(VSH_BLENDING_INFO_T) )
            {
                return SMR_INV_GET_INFO;
            }

            i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_vp.h_comp,
                             FALSE,
                             pt_vs_obj->t_vs_comp_grp.t_vp.t_info.ui1_in_port,
                             VID_PLA_GET_TYPE_BLENDING,
                             (VOID*)&t_drv_blending,
                             &z_size,
                             &pt_vs_obj->t_vs_comp_grp.t_vp.e_comp_cond );
            if ( i4_ret != RMR_OK )
            {
                return SMR_NOT_ALLOW;
            }

            *pt_vsh_blending = t_drv_blending;
            *pz_get_info_len = sizeof(VSH_BLENDING_INFO_T);
        }
        break;

    case SM_VSH_GET_TYPE_QV_POSITION:
        {
            VID_PLA_QV_INP_T          at_qv_inp[4] = { VID_PLA_QV_INP_1,
                                                       VID_PLA_QV_INP_2,
                                                       VID_PLA_QV_INP_3,
                                                       VID_PLA_QV_INP_4 };
            VID_PLA_QV_INFO_T         t_drv_qv;
            SIZE_T                    z_size = sizeof(VID_PLA_QV_INFO_T);
            VSH_QV_POSITION_INFO_T*   pt_vsh_qv = (VSH_QV_POSITION_INFO_T*)pv_get_info;

            if ( !pt_vsh_qv || !pz_get_info_len ||
                 *pz_get_info_len < sizeof(VSH_QV_POSITION_INFO_T) )
            {
                return SMR_INV_GET_INFO;
            }

            if ( pt_vs_obj->t_vs_comp_grp.t_vp.t_info.ui1_in_port >=4 )
            {
                return SMR_INTERNAL_ERROR;
            }

            i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_vp.h_comp,
                             FALSE,
                             pt_vs_obj->t_vs_comp_grp.t_vp.t_info.ui1_in_port,
                             VID_PLA_GET_TYPE_QV_INP,
                             (VOID*)&t_drv_qv,
                             &z_size,
                             &pt_vs_obj->t_vs_comp_grp.t_vp.e_comp_cond );
            if ( i4_ret != RMR_OK )
            {
                return SMR_NOT_ALLOW;
            }

            if ( t_drv_qv.e_tl == at_qv_inp[pt_vs_obj->t_vs_comp_grp.t_vp.t_info.ui1_in_port] )
            {
                *pt_vsh_qv = VSH_QV_POSITION_TOP_LEFT;
            }
            else if ( t_drv_qv.e_tr == at_qv_inp[pt_vs_obj->t_vs_comp_grp.t_vp.t_info.ui1_in_port] )
            {
                *pt_vsh_qv = VSH_QV_POSITION_TOP_RIGHT;
            }
            else if ( t_drv_qv.e_bl == at_qv_inp[pt_vs_obj->t_vs_comp_grp.t_vp.t_info.ui1_in_port] )
            {
                *pt_vsh_qv = VSH_QV_POSITION_BOTTOM_LEFT;
            }
            else if ( t_drv_qv.e_br == at_qv_inp[pt_vs_obj->t_vs_comp_grp.t_vp.t_info.ui1_in_port] )
            {
                *pt_vsh_qv = VSH_QV_POSITION_BOTTOM_RIGHT;
            }
            else
            {
                return SMR_INTERNAL_ERROR;
            }

            *pz_get_info_len = sizeof(VSH_QV_POSITION_INFO_T);
        }
        break;

    case SM_VSH_GET_TYPE_DECODER:
        if ( !pz_get_info_len || *pz_get_info_len < sizeof(HANDLE_T) )
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        *((HANDLE_T*)pv_get_info) = pt_vs_obj->t_vs_comp_grp.t_dec.h_comp_clone;
        *pz_get_info_len = sizeof(HANDLE_T);
        break;

    case SM_VSH_GET_TYPE_VIDEO_PLANE:
        if ( !pz_get_info_len || *pz_get_info_len < sizeof(HANDLE_T) )
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        *((HANDLE_T*)pv_get_info) = pt_vs_obj->t_vs_comp_grp.t_vp.h_comp_clone;
        *pz_get_info_len = sizeof(HANDLE_T);
        break;

    case SM_VSH_GET_TYPE_PLANE_MIXER:
        if ( !pz_get_info_len || *pz_get_info_len < sizeof(HANDLE_T) )
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        *((HANDLE_T*)pv_get_info) = pt_vs_obj->t_vs_comp_grp.t_pmx.h_comp_clone;
        *pz_get_info_len = sizeof(HANDLE_T);
        break;

    case SM_VSH_GET_TYPE_TV_ENCODER:
        if ( !pz_get_info_len || *pz_get_info_len < sizeof(HANDLE_T) )
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        *((HANDLE_T*)pv_get_info) = pt_vs_obj->t_vs_comp_grp.t_tve.h_comp_clone;
        *pz_get_info_len = sizeof(HANDLE_T);
        break;

    case SM_VSH_GET_TYPE_VIDEO_SRC_RESOLUTION:
        {
            SIZE_T                       z_drv_res_len = sizeof(VID_DEC_RESOLUTION_INFO_T);
            VSH_SRC_RESOLUTION_INFO_T*   pt_vsh_res = (VSH_SRC_RESOLUTION_INFO_T*)pv_get_info;

            if ( !pt_vsh_res )
            {
                return SMR_INV_GET_INFO;
            }
            if ( !pz_get_info_len ||
                 *pz_get_info_len < sizeof(VSH_SRC_RESOLUTION_INFO_T) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }

            if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
            {
                VID_DEC_RESOLUTION_INFO_T    t_drv_res;

                z_drv_res_len = sizeof(VID_DEC_RESOLUTION_INFO_T);
                i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                                 FALSE,
                                 pt_vs_obj->t_vs_comp_grp.t_dec.t_info.ui1_in_port,
                                 VID_DEC_GET_TYPE_RESOLUTION,
                                 (VOID*)&t_drv_res,
                                 &z_drv_res_len,
                                 &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond );
                if ( i4_ret != RMR_OK )
                {
                    return SMR_NOT_ALLOW;
                }

                _sm_video_hdlr_vd_res_2_vs( &t_drv_res, pt_vsh_res );
            }
            else
            {
                TV_DEC_RESOLUTION_INFO_T     t_drv_res;

                z_drv_res_len = sizeof(TV_DEC_RESOLUTION_INFO_T);
                i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                                 FALSE,
                                 pt_vs_obj->t_vs_comp_grp.t_dec.t_info.ui1_in_port,
                                 TV_DEC_GET_TYPE_RESOLUTION,
                                 (VOID*)&t_drv_res,
                                 &z_drv_res_len,
                                 &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond );
                if ( i4_ret != RMR_OK )
                {
                    return SMR_NOT_ALLOW;
                }
                else if ( pt_vs_obj->e_color_sys == COLOR_SYS_UNKNOWN )
                {
                    TV_DEC_COLOR_SYS_T e_drv_color_sys;
                    SIZE_T             z_size = sizeof(TV_DEC_COLOR_SYS_T);;

                    i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                        FALSE,
                        0,
                        TV_DEC_GET_TYPE_COLOR_SYS,
                        &e_drv_color_sys,
                        &z_size,
                        &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond );
                    if ( i4_ret != RMR_OK )
                    {
                        SM_VS_DBG_ERR((
                            "[SM] VS: _sm_video_hdlr_get(SM_VSH_GET_TYPE_VIDEO_SRC_RESOLUTION), "
                            "rm_get(TV_DEC_GET_TYPE_COLOR_SYS) fail, i4_ret = %d\r\n",
                            i4_ret ));
                    }
                    else
                    {
                        pt_vs_obj->e_color_sys = sm_vs_comp_tvd_color_sys_2_vsh( e_drv_color_sys );
                    }
                }

                _sm_video_hdlr_tvd_res_2_vs( pt_vs_obj, &t_drv_res, pt_vsh_res );
            }

            *pz_get_info_len = sizeof( VSH_SRC_RESOLUTION_INFO_T );
        }
        break;

    case SM_VSH_GET_TYPE_VID_CLOCK:
        {
            SIZE_T   z_drv_len = sizeof(INT16);
            INT16*   pi2_vsh_clock = (INT16*)pv_get_info;

            if ( !pi2_vsh_clock )
            {
                return SMR_INV_GET_INFO;
            }
            if ( !pz_get_info_len ||
                 *pz_get_info_len < sizeof(INT16) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
            {
                return SMR_NOT_SUPPORT;
            }

            i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                             FALSE,
                             pt_vs_obj->t_vs_comp_grp.t_dec.t_info.ui1_in_port,
                             TV_DEC_GET_TYPE_CLOCK,
                             (VOID*)pi2_vsh_clock,
                             &z_drv_len,
                             &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond );
            if ( i4_ret != RMR_OK )
            {
                return SMR_NOT_ALLOW;
            }

            *pz_get_info_len = z_drv_len;
        }
        break;

    case SM_VSH_GET_TYPE_VID_PHASE:
        {
            SIZE_T   z_drv_len = sizeof(UINT16);
            UINT16*  pui2_vsh_phase = (UINT16*)pv_get_info;

            if ( !pui2_vsh_phase )
            {
                return SMR_INV_GET_INFO;
            }
            if ( !pz_get_info_len ||
                 *pz_get_info_len < sizeof(UINT16) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
            {
                return SMR_NOT_SUPPORT;
            }

            i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                             FALSE,
                             pt_vs_obj->t_vs_comp_grp.t_dec.t_info.ui1_in_port,
                             TV_DEC_GET_TYPE_PHASE,
                             (VOID*)pui2_vsh_phase,
                             &z_drv_len,
                             &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond );
            if ( i4_ret != RMR_OK )
            {
                return SMR_NOT_ALLOW;
            }

            *pz_get_info_len = z_drv_len;
        }
        break;

    case SM_VSH_GET_TYPE_VID_H_POS:
        {
            SIZE_T   z_drv_len = sizeof(UINT16);
            UINT16*  pui2_vsh_h_pos = (UINT16*)pv_get_info;

            if ( !pui2_vsh_h_pos )
            {
                return SMR_INV_GET_INFO;
            }
            if ( !pz_get_info_len ||
                 *pz_get_info_len < sizeof(UINT16) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
            {
                return SMR_NOT_SUPPORT;
            }

            i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                             FALSE,
                             pt_vs_obj->t_vs_comp_grp.t_dec.t_info.ui1_in_port,
                             TV_DEC_GET_TYPE_H_POSITION,
                             (VOID*)pui2_vsh_h_pos,
                             &z_drv_len,
                             &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond );
            if ( i4_ret != RMR_OK )
            {
                return SMR_NOT_ALLOW;
            }

            *pz_get_info_len = z_drv_len;
        }
        break;

    case SM_VSH_GET_TYPE_VID_V_POS:
        {
            SIZE_T   z_drv_len = sizeof(UINT16);
            UINT16*  pui2_vsh_v_pos = (UINT16*)pv_get_info;

            if ( !pui2_vsh_v_pos )
            {
                return SMR_INV_GET_INFO;
            }
            if ( !pz_get_info_len ||
                 *pz_get_info_len < sizeof(UINT16) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
            {
                return SMR_NOT_SUPPORT;
            }

            i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                             FALSE,
                             pt_vs_obj->t_vs_comp_grp.t_dec.t_info.ui1_in_port,
                             TV_DEC_GET_TYPE_V_POSITION,
                             (VOID*)pui2_vsh_v_pos,
                             &z_drv_len,
                             &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond );
            if ( i4_ret != RMR_OK )
            {
                return SMR_NOT_ALLOW;
            }

            *pz_get_info_len = z_drv_len;
        }
        break;

    case SM_VSH_GET_TYPE_VID_MIN_MAX:
        {
            SIZE_T                    z_drv_len = 0;
            VSH_VID_MIN_MAX_INFO_T*   pt_vsh_min_max =
                (VSH_VID_MIN_MAX_INFO_T*)pv_get_info;

            if ( !pt_vsh_min_max )
            {
                return SMR_INV_GET_INFO;
            }
            if ( !pz_get_info_len ||
                 *pz_get_info_len < sizeof(VSH_VID_MIN_MAX_INFO_T) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
            {
                return SMR_NOT_SUPPORT;
            }

            if ( pt_vsh_min_max->e_get_type == SM_VSH_GET_TYPE_VID_CLOCK )
            {
                INT16                     i2_value;

                z_drv_len = sizeof(INT16);
                i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                                 FALSE,
                                 pt_vs_obj->t_vs_comp_grp.t_dec.t_info.ui1_in_port,
                                 TV_DEC_GET_TYPE_CLOCK_MIN,
                                 (VOID*)&i2_value,
                                 &z_drv_len,
                                 &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond );
                if ( i4_ret != RMR_OK )
                {
                    return SMR_NOT_ALLOW;
                }
                pt_vsh_min_max->i4_min_value = (INT32)i2_value;

                z_drv_len = sizeof(INT16);
                i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                                 FALSE,
                                 pt_vs_obj->t_vs_comp_grp.t_dec.t_info.ui1_in_port,
                                 TV_DEC_GET_TYPE_CLOCK_MAX,
                                 (VOID*)&i2_value,
                                 &z_drv_len,
                                 &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond );
                if ( i4_ret != RMR_OK )
                {
                    return SMR_NOT_ALLOW;
                }
                pt_vsh_min_max->i4_max_value = (INT32)i2_value;
            }
            else if ( pt_vsh_min_max->e_get_type == SM_VSH_GET_TYPE_VID_PHASE ||
                      pt_vsh_min_max->e_get_type == SM_VSH_GET_TYPE_VID_H_POS ||
                      pt_vsh_min_max->e_get_type == SM_VSH_GET_TYPE_VID_V_POS )
            {
                TV_DEC_MIN_MAX_INFO_T     t_drv_min_max;

                switch( pt_vsh_min_max->e_get_type )
                {
                case SM_VSH_GET_TYPE_VID_PHASE:
                    t_drv_min_max.e_get_type = TV_DEC_GET_TYPE_PHASE;
                    break;
                case SM_VSH_GET_TYPE_VID_H_POS:
                    t_drv_min_max.e_get_type = TV_DEC_GET_TYPE_H_POSITION;
                    break;
                case SM_VSH_GET_TYPE_VID_V_POS:
                    t_drv_min_max.e_get_type = TV_DEC_GET_TYPE_V_POSITION;
                    break;
                default:
                    return SMR_NOT_SUPPORT;
                }

                z_drv_len = sizeof(TV_DEC_MIN_MAX_INFO_T);
                i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                                 FALSE,
                                 pt_vs_obj->t_vs_comp_grp.t_dec.t_info.ui1_in_port,
                                 TV_DEC_GET_TYPE_MIN_MAX,
                                 (VOID*)&t_drv_min_max,
                                 &z_drv_len,
                                 &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond );
                if ( i4_ret != RMR_OK )
                {
                    return SMR_NOT_ALLOW;
                }

                pt_vsh_min_max->i4_min_value = (INT32)t_drv_min_max.ui4_min_value;
                pt_vsh_min_max->i4_max_value = (INT32)t_drv_min_max.ui4_max_value;
            }
            else if ( pt_vsh_min_max->e_get_type == SM_VSH_GET_TYPE_DISP_REGION ||
                      pt_vsh_min_max->e_get_type == SM_VSH_GET_TYPE_VIDEO_REGION  )
            {
                VID_PLA_MIN_MAX_INFO_T     t_drv_min_max;

                switch( pt_vsh_min_max->e_get_type )
                {
                case SM_VSH_GET_TYPE_DISP_REGION:
                    t_drv_min_max.e_get_type = VID_PLA_GET_TYPE_DISP_REGION;
                    break;
                case SM_VSH_GET_TYPE_VIDEO_REGION:
                    t_drv_min_max.e_get_type = VID_PLA_GET_TYPE_SRC_REGION;
                    break;
                default:
                    return SMR_NOT_SUPPORT;
                }

                z_drv_len = sizeof(VID_PLA_MIN_MAX_INFO_T);
                i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_vp.h_comp,
                                 FALSE,
                                 pt_vs_obj->t_vs_comp_grp.t_vp.t_info.ui1_in_port,
                                 VID_PLA_GET_TYPE_MIN_MAX,
                                 (VOID*)&t_drv_min_max,
                                 &z_drv_len,
                                 &pt_vs_obj->t_vs_comp_grp.t_vp.e_comp_cond );
                if ( i4_ret != RMR_OK )
                {
                    return SMR_NOT_ALLOW;
                }

                pt_vsh_min_max->i4_min_value = (INT32)t_drv_min_max.ui4_min_value;
                pt_vsh_min_max->i4_max_value = (INT32)t_drv_min_max.ui4_max_value;
            }
            else
            {
                return SMR_NOT_SUPPORT;
            }

            *pz_get_info_len = sizeof(VSH_VID_MIN_MAX_INFO_T);
        }
        break;

    case SM_VSH_GET_TYPE_CC_INDICATOR:
        {
            if ( !pv_get_info )
            {
                return SMR_INV_GET_INFO;
            }
            if ( *pz_get_info_len < sizeof(BOOL) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
            {
                return SMR_NOT_SUPPORT;
            }

            *((BOOL*)pv_get_info) = pt_vs_obj->b_cc_indicator;
        }
        break;

    case SM_VSH_GET_TYPE_VBI_DATA:
        {
            BOOL                 b_success = FALSE;
            SM_VSH_VBI_INFO_T*   pt_vbi_info = (SM_VSH_VBI_INFO_T*)pv_get_info;

            if ( !pv_get_info )
            {
                return SMR_INV_GET_INFO;
            }
            if ( *pz_get_info_len < sizeof(SM_VSH_VBI_INFO_T) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }

            if ( pt_vbi_info->e_vbi_type == SM_VSH_VBI_TYPE_NTSC &&
                 pt_vbi_info->pv_vbi_data &&
                 pt_vbi_info->z_vbi_data_size != 0 )
            {
                if ( t_vsh_vbi_psr_fct_tbl.pf_vbi_get )
                {
                    b_success = t_vsh_vbi_psr_fct_tbl.pf_vbi_get(
                        pt_vs_obj->t_vbi_psr_id,
                        pt_vbi_info->pv_vbi_data,
                        &pt_vbi_info->z_vbi_data_size );
                    if ( !b_success )
                    {
                        return SMR_NOT_ALLOW;
                    }
                }
            }
        }
        break;

    case SM_VSH_GET_TYPE_AMBIG_VGA_TIMING:
        {
            TV_DEC_AMBIG_VGA_TIMING_INFO_T t_drv_vga_timing;
            SIZE_T z_drv_vga_timing_len = sizeof(TV_DEC_AMBIG_VGA_TIMING_INFO_T);
            VSH_AMBIG_VGA_TIMING_INFO_T* pt_vsh_vga_timing = (VSH_AMBIG_VGA_TIMING_INFO_T*)pv_get_info;

            if ( !pt_vsh_vga_timing )
            {
                return SMR_INV_GET_INFO;
            }
            if ( !pz_get_info_len ||
                 *pz_get_info_len < sizeof(VSH_AMBIG_VGA_TIMING_INFO_T) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }

            switch( pt_vsh_vga_timing->e_type )
            {
            case VSH_AMBIG_VGA_TIMING_TYPE_NUM_IN_GRP:
                t_drv_vga_timing.e_type = TV_DEC_AMBIG_VGA_TIMING_TYPE_NUM_IN_GRP;
                break;
            case VSH_AMBIG_VGA_TIMING_TYPE_OPTION_IN_GRP:
                t_drv_vga_timing.e_type = TV_DEC_AMBIG_VGA_TIMING_TYPE_OPTION_IN_GRP;
                t_drv_vga_timing.u.t_opt.ui2_opt_index = pt_vsh_vga_timing->u.t_opt.ui2_opt_index;
                break;
            case VSH_AMBIG_VGA_TIMING_TYPE_CURR_OPT_IN_GRP:
                t_drv_vga_timing.e_type = TV_DEC_AMBIG_VGA_TIMING_TYPE_CURR_OPT_IN_GRP;
                break;
            default:
                return SMR_DRV_COMP_ERROR;
            }

            i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                             FALSE,
                             pt_vs_obj->t_vs_comp_grp.t_dec.t_info.ui1_in_port,
                             TV_DEC_GET_TYPE_AMBIG_VGA_TIMING,
                             (VOID*)&t_drv_vga_timing,
                             &z_drv_vga_timing_len,
                             &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond );
            if ( i4_ret != RMR_OK )
            {
                return SMR_NOT_ALLOW;
            }

            switch( t_drv_vga_timing.e_type )
            {
            case TV_DEC_AMBIG_VGA_TIMING_TYPE_NUM_IN_GRP:
                pt_vsh_vga_timing->u.ui2_num = t_drv_vga_timing.u.ui2_num;
                break;
            case TV_DEC_AMBIG_VGA_TIMING_TYPE_OPTION_IN_GRP:
            case TV_DEC_AMBIG_VGA_TIMING_TYPE_CURR_OPT_IN_GRP:
                pt_vsh_vga_timing->u.t_opt.ui2_opt_index = t_drv_vga_timing.u.t_opt.ui2_opt_index;
                pt_vsh_vga_timing->u.t_opt.ui2_ver_frequncy = t_drv_vga_timing.u.t_opt.ui2_ver_frequncy;
                pt_vsh_vga_timing->u.t_opt.ui4_hor_resolution = t_drv_vga_timing.u.t_opt.ui4_hor_resolution;
                pt_vsh_vga_timing->u.t_opt.ui4_ver_resolution = t_drv_vga_timing.u.t_opt.ui4_ver_resolution;
                break;
            default:
                return SMR_DRV_COMP_ERROR;
            }

            *pz_get_info_len = sizeof( VSH_AMBIG_VGA_TIMING_INFO_T );
        }
        break;

    case SM_VSH_GET_TYPE_COLOR_SYS:
        {
            SIZE_T z_drv_res_len = sizeof(TV_DEC_COLOR_SYS_T);
            TV_DEC_COLOR_SYS_T e_drv_tvd_color_sys;
            VSH_COLOR_SYS_TYPE_T* pe_vsh_color_sys = (VSH_COLOR_SYS_TYPE_T*)pv_get_info;

            i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                             FALSE,
                             pt_vs_obj->t_vs_comp_grp.t_dec.t_info.ui1_in_port,
                             TV_DEC_GET_TYPE_COLOR_SYS,
                             (VOID*)&e_drv_tvd_color_sys,
                             &z_drv_res_len,
                             &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond );
            if ( i4_ret != RMR_OK )
            {
                return SMR_NOT_ALLOW;
            }

            switch( e_drv_tvd_color_sys )
            {
            case TV_DEC_COLOR_SYS_UNKNOWN:
                *pe_vsh_color_sys = VSH_COLOR_SYS_TYPE_UNKNOWN;
                break;
            case TV_DEC_COLOR_SYS_NTSC:
                *pe_vsh_color_sys = VSH_COLOR_SYS_TYPE_NTSC;
                break;
            case TV_DEC_COLOR_SYS_NTSC_443:
                *pe_vsh_color_sys = VSH_COLOR_SYS_TYPE_NTSC_443;
                break;
            case TV_DEC_COLOR_SYS_PAL:
                *pe_vsh_color_sys = VSH_COLOR_SYS_TYPE_PAL;
                break;
            case TV_DEC_COLOR_SYS_PAL_N:
                *pe_vsh_color_sys = VSH_COLOR_SYS_TYPE_PAL_N;
                break;
            case TV_DEC_COLOR_SYS_PAL_M:
                *pe_vsh_color_sys = VSH_COLOR_SYS_TYPE_PAL_M;
                break;
            case TV_DEC_COLOR_SYS_PAL_60:
                *pe_vsh_color_sys = VSH_COLOR_SYS_TYPE_PAL_60;
                break;
            case TV_DEC_COLOR_SYS_SECAM:
                *pe_vsh_color_sys = VSH_COLOR_SYS_TYPE_SECAM;
                break;
            default:
                return SMR_DRV_COMP_ERROR;
            }

            *pz_get_info_len = sizeof( VSH_COLOR_SYS_TYPE_T );
        }
        break;

    case SM_VSH_GET_TYPE_REGION_CAPABILITY:
        {
            SIZE_T z_drv_region_cap_len = sizeof(VID_PLA_VID_REGION_CAPABILITY_T);
            VID_PLA_VID_REGION_CAPABILITY_T t_drv_vp_region_cap;
            VSH_REGION_CAPABILITY_INFO_T* pt_vsh_region_cap = (VSH_REGION_CAPABILITY_INFO_T*)pv_get_info;

            if ( pt_vsh_region_cap->e_get_type == SM_VSH_GET_TYPE_DISP_REGION )
            {
                t_drv_vp_region_cap.e_get_type = VID_PLA_GET_TYPE_DISP_REGION;
            }
            else if ( pt_vsh_region_cap->e_get_type == SM_VSH_GET_TYPE_VIDEO_REGION )
            {
                t_drv_vp_region_cap.e_get_type = VID_PLA_GET_TYPE_SRC_REGION;
            }
            else
            {
                return SMR_INV_GET_INFO;
            }

            i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_vp.h_comp,
                             FALSE,
                             pt_vs_obj->t_vs_comp_grp.t_vp.t_info.ui1_in_port,
                             VID_PLA_GET_TYPE_REGION_CAPABILITY,
                             (VOID*)&t_drv_vp_region_cap,
                             &z_drv_region_cap_len,
                             &pt_vs_obj->t_vs_comp_grp.t_vp.e_comp_cond );
            if ( i4_ret != RMR_OK )
            {
                return SMR_NOT_ALLOW;
            }

            pt_vsh_region_cap->b_is_enable = t_drv_vp_region_cap.b_is_enable;
            pt_vsh_region_cap->ui4_x_min= t_drv_vp_region_cap.ui4_x_min;
            pt_vsh_region_cap->ui4_x_max= t_drv_vp_region_cap.ui4_x_max;
            pt_vsh_region_cap->ui4_y_min= t_drv_vp_region_cap.ui4_y_min;
            pt_vsh_region_cap->ui4_y_max= t_drv_vp_region_cap.ui4_y_max;
            pt_vsh_region_cap->ui4_width_min= t_drv_vp_region_cap.ui4_width_min;
            pt_vsh_region_cap->ui4_width_max= t_drv_vp_region_cap.ui4_width_max;
            pt_vsh_region_cap->ui4_height_min= t_drv_vp_region_cap.ui4_height_min;
            pt_vsh_region_cap->ui4_height_max= t_drv_vp_region_cap.ui4_height_max;

            *pz_get_info_len = sizeof( VSH_REGION_CAPABILITY_INFO_T );
        }
        break;

    case SM_VSH_GET_TYPE_DIG_DECODE_STATUS:
        {
            VSH_DIG_DECODE_STATUS_T* pe_vsh_dec_status = (VSH_DIG_DECODE_STATUS_T*)pv_get_info;

            switch ( pt_vs_obj->e_dec_status )
            {
            case VID_DEC_DECODE_NO_DATA:
                *pe_vsh_dec_status = VSH_DIG_DECODE_NO_DATA;
                break;

            case VID_DEC_DECODE_NORMAL:
                *pe_vsh_dec_status = VSH_DIG_DECODE_NORMAL;
                break;

            case VID_DEC_DECODE_ERROR:
                *pe_vsh_dec_status = VSH_DIG_DECODE_ERROR;
                break;

            case VID_DEC_DECODE_HD_NOT_SUPPORT:
                *pe_vsh_dec_status = VSH_DIG_DECODE_HD_NOT_SUPPORT;
                break;

            case VID_DEC_DECODE_CODEC_NOT_SUPPORT:
                *pe_vsh_dec_status = VSH_DIG_DECODE_CODEC_NOT_SUPPORT;
                break;
                
            case VID_DEC_DECODE_DATA_RECEIVE:
                *pe_vsh_dec_status = VSH_DIG_DECODE_DATA_RECEIVE;
                break;

            case VID_DEC_DECODE_RES_NOT_SUPPORT:
                *pe_vsh_dec_status = VSH_DIG_DECODE_RES_NOT_SUPPORT;
                break;

            case VID_DEC_DECODE_UNKNOWN:
            default:
                *pe_vsh_dec_status = VSH_DIG_DECODE_UNKNOWN;
            }

            *pz_get_info_len = sizeof( VSH_DIG_DECODE_STATUS_T );
        }
        break;

    case SM_VSH_GET_TYPE_PICTURE_INFO:
        {
            SIZE_T z_drv_len = sizeof(VID_PLA_PICTURE_INFO_T);
            VID_PLA_PICTURE_INFO_T t_drv_pic_info;
            VSH_PICTURE_INFO_T* pt_vsh_pic_info = (VSH_PICTURE_INFO_T*)pv_get_info;

            if ( !IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
            {
                return SMR_NOT_SUPPORT;
            }
            if ( !pt_vsh_pic_info )
            {
                return SMR_INV_GET_INFO;
            }
            if ( !pz_get_info_len ||
                 *pz_get_info_len < sizeof(VSH_PICTURE_INFO_T) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }

            switch ( pt_vsh_pic_info->e_type )
            {
            case VSH_PIC_INFO_TYPE_PICTURE:
                t_drv_pic_info.e_type = VID_PLA_PIC_INFO_TYPE_PICTURE;
                break;

            case VSH_PIC_INFO_TYPE_GOP:
                t_drv_pic_info.e_type = VID_PLA_PIC_INFO_TYPE_GOP;
                break;

            default:
                return SMR_INV_GET_INFO;
            }

            i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_vp.h_comp,
                             FALSE,
                             pt_vs_obj->t_vs_comp_grp.t_vp.t_info.ui1_in_port,
                             VID_PLA_GET_TYPE_PICTURE_INFO,
                             (VOID*)&t_drv_pic_info,
                             &z_drv_len,
                             &pt_vs_obj->t_vs_comp_grp.t_vp.e_comp_cond );
            if ( i4_ret != RMR_OK )
            {
                return SMR_NOT_ALLOW;
            }

            pt_vsh_pic_info->ui8_offset = t_drv_pic_info.ui8_offset;
            switch ( t_drv_pic_info.e_type )
            {
            case VID_PLA_PIC_INFO_TYPE_PICTURE:
                pt_vsh_pic_info->u.ui8_pts = t_drv_pic_info.u.ui8_pts;
                break;

            case VID_PLA_PIC_INFO_TYPE_GOP:
                {
                    pt_vsh_pic_info->u.t_time_code.ui1_hours = t_drv_pic_info.u.t_time_code.ui1_hours;
                    pt_vsh_pic_info->u.t_time_code.ui1_minutes = t_drv_pic_info.u.t_time_code.ui1_minutes;
                    pt_vsh_pic_info->u.t_time_code.ui1_seconds = t_drv_pic_info.u.t_time_code.ui1_seconds;
                    pt_vsh_pic_info->u.t_time_code.ui1_pictures = t_drv_pic_info.u.t_time_code.ui1_pictures;
                    pt_vsh_pic_info->u.t_time_code.i4_total_dur_adj = t_drv_pic_info.u.t_time_code.i4_total_dur_adj;
                }
                break;

            default:
                return SMR_DRV_COMP_ERROR;
            }

            *pz_get_info_len = sizeof( VSH_PICTURE_INFO_T );
        }
        break;

    case SM_VSH_GET_TYPE_SPEED:
        {
            SIZE_T z_drv_len = sizeof(MM_SPEED_TYPE_T);

            if ( !pv_get_info )
            {
                return SMR_INV_GET_INFO;
            }
            if ( !pz_get_info_len ||
                 *pz_get_info_len < sizeof(MM_SPEED_TYPE_T) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }
            if ( !IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
            {
                return SMR_NOT_SUPPORT;
            }

            i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                             FALSE,
                             pt_vs_obj->t_vs_comp_grp.t_dec.t_info.ui1_in_port,
                             VID_DEC_GET_TYPE_SPEED,
                             (VOID*)pv_get_info,
                             &z_drv_len,
                             &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond );
            if ( i4_ret != RMR_OK )
            {
                return SMR_NOT_ALLOW;
            }

            *pz_get_info_len = z_drv_len;
        }
        break;

    case SM_VSH_GET_TYPE_THUMBNAIL:
        {
            SIZE_T z_drv_len = sizeof(VID_DEC_THUMBNAIL_INFO_T);
            VID_DEC_THUMBNAIL_INFO_T t_drv_thumbnail_info;
            VSH_THUMBNAIL_INFO_T* pt_vsh_thumbnail_info = (VSH_THUMBNAIL_INFO_T*)pv_get_info;

            if ( !IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
            {
                return SMR_NOT_SUPPORT;
            }
            if ( !pt_vsh_thumbnail_info )
            {
                return SMR_INV_GET_INFO;
            }
            if ( !pz_get_info_len ||
                 *pz_get_info_len < sizeof(VSH_THUMBNAIL_INFO_T) )
            {
                return SMR_INSUFFICIENT_SIZE;
            }

            switch ( pt_vsh_thumbnail_info->e_canvas_colormode )
            {
            case VSH_COLORMODE_AYUV_CLUT2:
                t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_AYUV_CLUT2;
                break;
            case VSH_COLORMODE_AYUV_CLUT4:
                t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_AYUV_CLUT4;
                break;
            case VSH_COLORMODE_AYUV_CLUT8:
                t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_AYUV_CLUT8;
                break;
            case VSH_COLORMODE_UYVY_16:
                t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_UYVY_16;
                break;
            case VSH_COLORMODE_YUYV_16:
                t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_YUYV_16;
                break;
            case VSH_COLORMODE_AYUV_D8888:
                t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_AYUV_D8888;
                break;
            case VSH_COLORMODE_ARGB_CLUT2:
                t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_ARGB_CLUT2;
                break;
            case VSH_COLORMODE_ARGB_CLUT4:
                t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_ARGB_CLUT4;
                break;
            case VSH_COLORMODE_ARGB_CLUT8:
                t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_ARGB_CLUT8;
                break;
            case VSH_COLORMODE_RGB_D565:
                t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_RGB_D565;
                break;
            case VSH_COLORMODE_ARGB_D1555:
                t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_ARGB_D1555;
                break;
            case VSH_COLORMODE_ARGB_D4444:
                t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_ARGB_D4444;
                break;
            case VSH_COLORMODE_ARGB_D8888:
                t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_ARGB_D8888;
                break;
            case VSH_COLORMODE_YUV_420_BLK:
                t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_YUV_420_BLK;
                break;
            case VSH_COLORMODE_YUV_420_RS:
                t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_YUV_420_RS;
                break;
            case VSH_COLORMODE_YUV_422_BLK:
                t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_YUV_422_BLK;
                break;
            case VSH_COLORMODE_YUV_422_RS:
                t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_YUV_422_RS;
                break;
            case VSH_COLORMODE_YUV_444_BLK:
                t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_YUV_444_BLK;
                break;
            case VSH_COLORMODE_YUV_444_RS:
                t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_YUV_444_RS;
                break;
            default:
                return SMR_INV_GET_INFO;
            }
            t_drv_thumbnail_info.ui4_canvas_width = pt_vsh_thumbnail_info->ui4_canvas_width;
            t_drv_thumbnail_info.ui4_canvas_height = pt_vsh_thumbnail_info->ui4_canvas_height;
            t_drv_thumbnail_info.ui4_canvas_pitch = pt_vsh_thumbnail_info->ui4_canvas_pitch;
            t_drv_thumbnail_info.ui4_thumbnail_x = pt_vsh_thumbnail_info->ui4_thumbnail_x;
            t_drv_thumbnail_info.ui4_thumbnail_y = pt_vsh_thumbnail_info->ui4_thumbnail_y;
            t_drv_thumbnail_info.ui4_thumbnail_width = pt_vsh_thumbnail_info->ui4_thumbnail_width;
            t_drv_thumbnail_info.ui4_thumbnail_height = pt_vsh_thumbnail_info->ui4_thumbnail_height;
            t_drv_thumbnail_info.pc_canvas_buffer = pt_vsh_thumbnail_info->pc_canvas_buffer;

            i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                             FALSE,
                             pt_vs_obj->t_vs_comp_grp.t_dec.t_info.ui1_in_port,
                             VID_DEC_GET_TYPE_THUMBNAIL,
                             (VOID*)&t_drv_thumbnail_info,
                             &z_drv_len,
                             &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond );
            if ( i4_ret != RMR_OK )
            {
                return SMR_NOT_ALLOW;
            }

            *pz_get_info_len = sizeof( VSH_THUMBNAIL_INFO_T );
        }
        break;

    case SM_VSH_GET_TYPE_HDMI_WITH_XVYCC:
        {
            SIZE_T z_drv_len = sizeof(BOOL);
            BOOL b_drv_hdmi_with_xvycc;
            BOOL* pb_vsh_hdmi_with_xvycc = (BOOL*)pv_get_info;

            i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_vp.h_comp,
                             FALSE,
                             pt_vs_obj->t_vs_comp_grp.t_vp.t_info.ui1_in_port,
                             VID_PLA_GET_TYPE_HDMI_WITH_XVYCC,
                             (VOID*)&b_drv_hdmi_with_xvycc,
                             &z_drv_len,
                             &pt_vs_obj->t_vs_comp_grp.t_vp.e_comp_cond );
            if ( i4_ret != RMR_OK )
            {
                return SMR_NOT_ALLOW;
            }

            *pb_vsh_hdmi_with_xvycc = b_drv_hdmi_with_xvycc;
            *pz_get_info_len = sizeof(BOOL);
        }
        break;

    case SM_VSH_GET_TYPE_IGNORE:
    default:
        return SMR_NOT_SUPPORT;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_video_hdlr_tunl_get
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
 *          SMR_INV_HANDLE      Fail.
 *          SMR_INTERNAL_ERROR  Fail.
 *          SMR_NOT_SUPPORT     Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sm_video_hdlr_tunl_get(
    HANDLE_T       h_stream,
    SM_GET_TYPE_T  e_get_type,
    VOID*          pv_get_info,
    SIZE_T*        pz_get_info_len )
{
    INT32                 i4_ret;
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;
    HANDLE_T              h_drv;
    RM_COND_T*            pe_drv_cond;

    if ( SM_GET_GRP(e_get_type)==SM_GRP_SESS )
    {
        return SMR_NOT_SUPPORT;
    }

    pt_vs_obj = (SM_VIDEO_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_vs_obj )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_get_type )
    {
    case SM_GRP_TUNL_DEC:
        h_drv = pt_vs_obj->t_vs_comp_grp.t_dec.h_comp_clone;
        pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond;
        break;

    case SM_GRP_TUNL_VIDEO_PLANE:
        h_drv = pt_vs_obj->t_vs_comp_grp.t_vp.h_comp_clone;
        pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_vp.e_comp_cond;
        break;

    case SM_GRP_TUNL_PLANE_MIXER:
        h_drv = pt_vs_obj->t_vs_comp_grp.t_pmx.h_comp_clone;
        pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_pmx.e_comp_cond;
        break;

    case SM_GRP_TUNL_TV_ENC:
        h_drv = pt_vs_obj->t_vs_comp_grp.t_tve.h_comp_clone;
        pe_drv_cond = &pt_vs_obj->t_vs_comp_grp.t_tve.e_comp_cond;
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
 * Name: _sm_vs_send_msg
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
VOID sm_vs_send_msg( SM_VIDEO_HDLR_MSG_T* pt_msg )
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
        i4_ret = x_msg_q_send( gh_video_hdlr_msgq,
                               pt_msg,
                               sizeof( SM_VIDEO_HDLR_MSG_T ),
                               0 ); /* highest priority */
        if ( i4_ret != OSR_OK )
        {
            HANDLE_T    h_thread = NULL_HANDLE;

            i4_ret = x_thread_self( &h_thread );
            if ( i4_ret != OSR_OK ||
                 (i4_ret == OSR_OK && !x_handle_valid(h_thread)) )
            {
                x_dbg_stmt("[SM] VS, x_thread_self() error! i4_ret=%d\r\n", i4_ret );
                break;
            }

            /* Send message fail, wait 10 ms and retry for 10 times */
            x_thread_delay( 10 );
            continue;
        }

        i4_ret = sm_trigger_task( gh_video_handler, (VOID*)gh_video_handler );
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
 * Name: sm_vs_send_msg_do_event
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
VOID sm_vs_send_msg_do_event(
    HANDLE_T                   h_stream,
    UINT32                     ui4_event,
    UINT32                     ui4_data )
{
    SM_VIDEO_HDLR_MSG_T  t_msg;

    t_msg.h_stream = h_stream;
    t_msg.e_msg_type = SM_VIDEO_HDLR_MSG_DO_EVENT;

    t_msg.u.t_msg_do_event.ui4_event = ui4_event;
    t_msg.u.t_msg_do_event.ui4_data = ui4_data;

    sm_vs_send_msg( &t_msg );
}

/*-----------------------------------------------------------------------------
 * Name: sm_video_hdlr_open_fct
 *
 * Description: Video stream handler stream open function.
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
INT32 sm_video_hdlr_open_fct( const SM_STREAM_T*  pt_stream_base,
                              const SM_COMMAND_T* pt_cmd_sets,
                              BOOL                b_auto_play,
                              x_stream_nfy_fct    pf_sm_nfy,
                              VOID*               pv_sm_nfy_tag,
                              HANDLE_T*           ph_stream )   /* OUT */
{
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;
    SM_SESSION_T*         pt_sess_obj = NULL;
    HANDLE_T              h_stream = NULL_HANDLE;
    INT32                 i4_ret = SMR_OK;
    SM_VS_STATE_TAG_T*    pt_state_tag = NULL;
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

    pt_state_tag = (SM_VS_STATE_TAG_T*)x_mem_alloc( sizeof(SM_VS_STATE_TAG_T) );
    if ( !pt_state_tag )
    {
        return SMR_SESS_NOT_EXIST;
    }

    pt_vs_obj = (SM_VIDEO_STREAM_T*) x_mem_alloc( sizeof(SM_VIDEO_STREAM_T) );
    if ( !pt_vs_obj )
    {
        x_mem_free( pt_state_tag );
        return SMR_INSUFFICIENT_MEMORY;
    }

    x_memset( pt_vs_obj, 0, sizeof(SM_VIDEO_STREAM_T));

    pt_vs_obj->t_base = *pt_stream_base;
    pt_vs_obj->t_vs_comp_grp = GT_SM_VS_COMP_GRP_INITOR;
    pt_vs_obj->pf_sess_hdlr_sm_nfy = pf_sm_nfy;
    pt_vs_obj->pv_sess_hdlr_sm_nfy_tag = pv_sm_nfy_tag;
    pt_vs_obj->t_base.h_pm_comp = NULL_HANDLE;
    pt_vs_obj->e_vd_ctrl_type = VID_DEC_CTRL_RESET;
    pt_vs_obj->e_tvd_ctrl_type = TV_DEC_CTRL_RESET;
    pt_vs_obj->e_vp_mode = VID_PLA_NORMAL;
    pt_vs_obj->ui4_lastest_ca_event = SM_EVN_CA_COND_UNKNOWN;
    pt_vs_obj->ui4_lastest_pm_event = SM_EVN_IGNORE;
    pt_vs_obj->ui4_lastest_dec_event = SM_EVN_IGNORE;
    pt_vs_obj->ui4_lastest_vp_event = SM_EVN_IGNORE;
    pt_vs_obj->ui4_lastest_pmx_event = SM_EVN_IGNORE;
    pt_vs_obj->ui4_lastest_tve_event = SM_EVN_IGNORE;
    pt_vs_obj->ui4_lastest_ca_detail_code = 0;
    pt_vs_obj->e_tv_signal = TV_DEC_TV_SIG_UNKNOWN;
    pt_vs_obj->e_dec_status = VID_DEC_DECODE_UNKNOWN;
    pt_vs_obj->e_color_sys = COLOR_SYS_UNKNOWN;
    pt_vs_obj->t_vbi_psr_id = 0xFFFFFFFF;
    pt_vs_obj->t_vbi_nfy.pf_vbi_nfy = NULL;
    pt_vs_obj->t_vbi_nfy.pv_tag = NULL;
    pt_vs_obj->b_cc_indicator = FALSE;
    pt_vs_obj->h_cc_ind_timer = NULL_HANDLE;
    pt_vs_obj->b_is_timer_active = FALSE;
    pt_vs_obj->b_pending_res_chg = FALSE;

    x_memset( &pt_vs_obj->t_vid_region, 0, sizeof(VSH_REGION_INFO_T) );
    pt_vs_obj->t_vid_region.ui4_width = VSH_REGION_MAX_WIDTH;
    pt_vs_obj->t_vid_region.ui4_height = VSH_REGION_MAX_HEIGHT;
    x_memset( &pt_vs_obj->t_disp_region, 0, sizeof(VSH_REGION_INFO_T) );
    pt_vs_obj->t_disp_region.ui4_width = VSH_REGION_MAX_WIDTH;
    pt_vs_obj->t_disp_region.ui4_height = VSH_REGION_MAX_HEIGHT;
    x_memset( &pt_vs_obj->t_vid_res, 0, sizeof(VSH_SRC_RESOLUTION_INFO_T) );
    pt_vs_obj->h_cc_strm = NULL_HANDLE;
    pt_vs_obj->ui2_force_pid = MPEG_2_NULL_PID;
    pt_vs_obj->ui1_force_enc = VID_ENC_MPEG_2;
#ifdef SYS_MHP_SUPPORT
    pt_vs_obj->e_ca_req_lvl = PM_CA_REQUIRE_MANDATORY;
#endif
    pt_vs_obj->b_codec_not_support = TRUE;
    pt_vs_obj->e_actual_enc = VID_ENC_MPEG_2;
    x_memset( &pt_vs_obj->t_rd3d_info, 0, sizeof(RD3D_REG_DESC_T));
    x_memset( &pt_vs_obj->t_mvc_ext, 0, sizeof(MVC_EXTENSION_DESC_T));

    x_memset( &pt_vs_obj->t_crypt_info, 0, sizeof(MM_CRYPT_INFO_T) );
    pt_vs_obj->t_crypt_info.u.t_pvr_crypt.e_mode = MM_CRYPT_MODE_INVALID;

    x_memset( &pt_vs_obj->t_cp_info_lst, 0, sizeof(CP_INFO_LIST_T) );

    /* initialize driver descriptors */
    pt_vs_obj->t_vs_comp_grp.t_dec.t_info.e_type = DRVT_UNKNOWN;
    pt_vs_obj->t_vs_comp_grp.t_vp.t_info.e_type = DRVT_VID_PLANE;
    pt_vs_obj->t_vs_comp_grp.t_pmx.t_info.e_type = DRVT_PLA_MXR;
    pt_vs_obj->t_vs_comp_grp.t_tve.t_info.e_type = DRVT_TV_ENC;

    x_strcpy( pt_vs_obj->t_vs_comp_grp.t_dec.t_info.s_logical_name,
              pt_sess_obj->s_dest_grp_name );
    x_strcpy( pt_vs_obj->t_vs_comp_grp.t_vp.t_info.s_logical_name,
              pt_sess_obj->s_dest_grp_name );
    x_strcpy( pt_vs_obj->t_vs_comp_grp.t_pmx.t_info.s_logical_name,
              pt_sess_obj->s_dest_grp_name );
    x_strcpy( pt_vs_obj->t_vs_comp_grp.t_tve.t_info.s_logical_name,
              pt_sess_obj->s_dest_grp_name );

    i4_ret = x_sema_create( &pt_vs_obj->t_base.h_lock,
                            X_SEMA_TYPE_MUTEX,
                            X_SEMA_STATE_UNLOCK );
    if ( i4_ret != OSR_OK || pt_vs_obj->t_base.h_lock == NULL_HANDLE )
    {
        x_mem_free( pt_vs_obj );
        x_mem_free( pt_state_tag );
        return SMR_INTERNAL_ERROR;
    }

    i4_ret = x_timer_create( &pt_vs_obj->h_cc_ind_timer );
    if ( i4_ret != OSR_OK )
    {
        x_sema_delete( pt_vs_obj->t_base.h_lock);
        x_mem_free( pt_vs_obj );
        x_mem_free( pt_state_tag );
        return SMR_INTERNAL_ERROR;
    }

    /* create stream handle */
    i4_ret = handle_alloc( SM_HT_STREAM,
                           pt_vs_obj,
                           (VOID*)pv_sm_nfy_tag,
                           &_sm_video_hdlr_vs_free_fct,
                           &h_stream );
    if ( i4_ret != HR_OK || h_stream==NULL_HANDLE )
    {
        x_timer_delete( pt_vs_obj->h_cc_ind_timer );
        x_sema_delete( pt_vs_obj->t_base.h_lock);
        x_mem_free( pt_vs_obj );
        x_mem_free( pt_state_tag );
        return SMR_OUT_OF_HANDLES;
    }
    pt_vs_obj->t_base.h_stream = h_stream;
    *ph_stream = h_stream;

    pt_state_tag->h_stream = h_stream;
    pt_state_tag->pt_vs_obj = pt_vs_obj;
    i4_ret = sm_state_init( at_vs_state_cond_lst,
                            at_vs_state_descr_lst,
                            at_vs_allowed_pending_event_lst,
                            sm_vs_state_notify_app,
                            (VOID*)pt_state_tag,
                            &pt_vs_obj->t_state );
    if ( i4_ret != SMR_OK )
    {
        handle_free( h_stream, FALSE );
        return SMR_INTERNAL_ERROR;
    }

    SM_LOCK( pt_vs_obj->t_base.h_lock );

    /* process commands */
    if ( pt_cmd_sets )
    {
        i4_ret = sm_vs_exec_cmd( h_stream, pt_vs_obj, pt_cmd_sets);
        if ( i4_ret != SMR_OK )
        {
            SM_UNLOCK( pt_vs_obj->t_base.h_lock );
            handle_free( h_stream, FALSE );
            return i4_ret;
        }
    }

    /* Get PCR ID from session. */
    i4_ret = x_sm_get( pt_vs_obj->t_base.h_sess,
                       SM_SESS_GNRC_GET_TYPE_PCR_ID,
                       &pt_vs_obj->t_base.ui2_pcr_id,
                       &z_get_size );
    if ( i4_ret != SMR_OK )
    {
        SM_UNLOCK( pt_vs_obj->t_base.h_lock );
        return SMR_DRV_COMP_ERROR;
    }

    /* Get media format from session. */
    z_get_size = sizeof(MEDIA_FORMAT_T);
    i4_ret = x_sm_get( pt_vs_obj->t_base.h_sess,
                       SM_SESS_GNRC_GET_TYPE_MEDIA_FMT,
                       &pt_vs_obj->t_media_fmt,
                       &z_get_size );
    if ( i4_ret != SMR_OK )
    {
        SM_UNLOCK( pt_vs_obj->t_base.h_lock );
        return SMR_DRV_COMP_ERROR;
    }

    sm_vs_send_msg_do_event(
        h_stream,
        SM_EVN_VS_APP_OPEN_REQ,
        0 );

    if ( b_auto_play )
    {
        sm_vs_send_msg_do_event(
            h_stream,
            SM_EVN_VS_APP_PLAY_REQ,
            0 );
    }

    SM_UNLOCK( pt_vs_obj->t_base.h_lock );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_video_hdlr_close_fct
 *
 * Description: Video stream handler stream close function.
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
INT32 sm_video_hdlr_close_fct( HANDLE_T     h_stream )
{
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;

    pt_vs_obj = (SM_VIDEO_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_vs_obj )
    {
        return SMR_INV_HANDLE;
    }

    sm_vs_send_msg_do_event(
        h_stream,
        SM_EVN_VS_APP_CLOSE_REQ,
        0 );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_video_hdlr_set_fct
 *
 * Description: Video stream handler set function.
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
 *          SMR_NOT_SUPPORT               Fail.
 *          SMR_INV_ARG                   Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_video_hdlr_set_fct( HANDLE_T       h_stream,
                             SM_SET_TYPE_T  e_set_type,
                             VOID*          pv_set_info,
                             SIZE_T         z_set_info_len )
{
    INT32                 i4_ret = SMR_OK;
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;

    pt_vs_obj = (SM_VIDEO_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_vs_obj )
    {
        return SMR_INV_HANDLE;
    }

    if ( pt_vs_obj->t_base.e_condition != SM_COND_IGNORE &&
         pt_vs_obj->t_base.e_condition != SM_COND_OPENING &&
         pt_vs_obj->t_base.e_condition != SM_COND_OPENED &&
         pt_vs_obj->t_base.e_condition != SM_COND_STARTING &&
         pt_vs_obj->t_base.e_condition != SM_COND_STARTED &&
         pt_vs_obj->t_base.e_condition != SM_COND_STOPPING )
    {
        return SMR_CAN_NOT_OP_IN_THIS_STATE;
    }

    switch ( SM_GET_GRP(e_set_type) )
    {
    case SM_GRP_STRM:
    case SM_GRP_SESS:
        {
            if ( pt_vs_obj->t_base.e_condition == SM_COND_STARTED &&
                 ( e_set_type == SM_SESS_SET_TYPE_PLAY ||
                   e_set_type == SM_STRM_SET_TYPE_PLAY ) )
            {
                return SMR_ALREADY_PLAYED;
            }
            if ( pt_vs_obj->t_base.e_condition == SM_COND_OPENED &&
                 ( e_set_type == SM_SESS_SET_TYPE_STOP ||
                   e_set_type == SM_STRM_SET_TYPE_STOP ) )
            {
                return SMR_ALREADY_STOPPED;
            }

            i4_ret = _sm_video_hdlr_slctr_set(
                h_stream,
                e_set_type,
                pv_set_info,
                z_set_info_len );
        }
        break;

    case SM_GRP_VSH:
        i4_ret = _sm_video_hdlr_set( h_stream,
                                     e_set_type,
                                     pv_set_info,
                                     z_set_info_len );
        break;

    case SM_GRP_TUNL_DEC:
    case SM_GRP_TUNL_VIDEO_PLANE:
    case SM_GRP_TUNL_PLANE_MIXER:
    case SM_GRP_TUNL_TV_ENC:
        i4_ret = _sm_video_hdlr_tunl_set( h_stream,
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
 * Name: sm_video_hdlr_get_fct
 *
 * Description: Video stream handler stream get function definition
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
INT32 sm_video_hdlr_get_fct( HANDLE_T       h_stream,
                             SM_GET_TYPE_T  e_get_type,
                             VOID*          pv_get_info,       /* IN/OUT */
                             SIZE_T*        pz_get_info_len )  /* IN/OUT */
{
    INT32                 i4_ret = SMR_OK;
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;

    if ( !pv_get_info || !pz_get_info_len )
    {
        return SMR_INV_ARG;
    }

    pt_vs_obj = (SM_VIDEO_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_vs_obj )
    {
        return SMR_INV_HANDLE;
    }

    if ( pt_vs_obj->t_base.e_condition != SM_COND_OPENED &&
         pt_vs_obj->t_base.e_condition != SM_COND_STARTING &&
         pt_vs_obj->t_base.e_condition != SM_COND_STARTED &&
         pt_vs_obj->t_base.e_condition != SM_COND_STOPPING )
    {
        return SMR_CAN_NOT_OP_IN_THIS_STATE;
    }

    switch ( SM_GET_GRP(e_get_type) )
    {
    case SM_GRP_STRM:
    case SM_GRP_SESS:
        i4_ret = _sm_video_hdlr_slctr_get( h_stream,
                                           e_get_type,
                                           pv_get_info,
                                           pz_get_info_len );
        break;

    case SM_GRP_VSH:
        i4_ret = _sm_video_hdlr_get( h_stream,
                                     e_get_type,
                                     pv_get_info,
                                     pz_get_info_len );
        break;

    case SM_GRP_TUNL_DEC:
    case SM_GRP_TUNL_VIDEO_PLANE:
    case SM_GRP_TUNL_PLANE_MIXER:
    case SM_GRP_TUNL_TV_ENC:
        i4_ret = _sm_video_hdlr_tunl_get( h_stream,
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
 * Name: sm_video_hdlr_task_fct
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
VOID sm_video_hdlr_task_fct( VOID*  pv_arg )
{
    INT32                      i4_ret = SMR_OK;
    UINT16                     ui2_index;
    HANDLE_TYPE_T              e_hdlr_type = 0;
    SM_VIDEO_STREAM_T*         pt_vs_obj = NULL;
    SM_VIDEO_HDLR_MSG_T        t_msg;
    SIZE_T                     z_msg_size = sizeof(SM_VIDEO_HDLR_MSG_T);

    x_memset( &t_msg, 0, sizeof(SM_VIDEO_HDLR_MSG_T) );

    i4_ret = x_msg_q_receive( &ui2_index,
                              &t_msg,
                              &z_msg_size,
                              &gh_video_hdlr_msgq,
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

    pt_vs_obj = (SM_VIDEO_STREAM_T*)sm_get_strm_obj( t_msg.h_stream );
    if ( !pt_vs_obj )
    {
        SM_ABORT( SM_DBG_ABRT_INV_STRM_OBJ );
    }

    if ( pt_vs_obj->t_base.t_strm_comp_id.e_type != ST_VIDEO )
    {
        return;
    }

    SM_LOCK_RETURN_ON_FAIL( pt_vs_obj->t_base.h_lock, );

    switch( t_msg.e_msg_type )
    {
    case SM_VIDEO_HDLR_MSG_DO_EVENT:
        _sm_video_hdlr_do_msg_do_event(
            t_msg.h_stream,
            pt_vs_obj,
            &t_msg.u.t_msg_do_event );
        break;

    case SM_VIDEO_HDLR_MSG_DEC_RM_NFY:
        _sm_video_hdlr_do_msg_rm_nfy(
            t_msg.h_stream,
            pt_vs_obj,
            SM_EVN_VS_GRP_DEC,
            &t_msg.u.t_msg_rm );
        break;

    case SM_VIDEO_HDLR_MSG_VP_RM_NFY:
        _sm_video_hdlr_do_msg_rm_nfy(
            t_msg.h_stream,
            pt_vs_obj,
            SM_EVN_VS_GRP_VP,
            &t_msg.u.t_msg_rm );
        break;

    case SM_VIDEO_HDLR_MSG_PMX_RM_NFY:
        _sm_video_hdlr_do_msg_rm_nfy(
            t_msg.h_stream,
            pt_vs_obj,
            SM_EVN_VS_GRP_PMX,
            &t_msg.u.t_msg_rm );
        break;

    case SM_VIDEO_HDLR_MSG_TVE_RM_NFY:
        _sm_video_hdlr_do_msg_rm_nfy(
            t_msg.h_stream,
            pt_vs_obj,
            SM_EVN_VS_GRP_TVE,
            &t_msg.u.t_msg_rm );
        break;

    case SM_VIDEO_HDLR_MSG_VD_CTRL_NFY:
        _sm_video_hdlr_do_msg_vd_ctrl_nfy(
            t_msg.h_stream,
            pt_vs_obj,
            &t_msg.u.t_msg_vd_ctrl );
        break;

    case SM_VIDEO_HDLR_MSG_TVD_CTRL_NFY:
        _sm_video_hdlr_do_msg_tvd_ctrl_nfy(
            t_msg.h_stream,
            pt_vs_obj,
            &t_msg.u.t_msg_tvd_ctrl );
        break;

    case SM_VIDEO_HDLR_MSG_VP_CTRL_NFY:
        _sm_video_hdlr_do_msg_vp_ctrl_nfy(
            t_msg.h_stream,
            pt_vs_obj,
            &t_msg.u.t_msg_vp_ctrl );
        break;

    case SM_VIDEO_HDLR_MSG_PM_NFY:
        _sm_video_hdlr_do_msg_pm_nfy(
            t_msg.h_stream,
            pt_vs_obj,
            &t_msg.u.t_msg_pm );
        break;

    case SM_VIDEO_HDLR_MSG_CCI_NFY:
        _sm_video_hdlr_do_msg_cci_msg(
            t_msg.h_stream,
            pt_vs_obj,
            &t_msg.u.t_msg_cci );
        break;

    case SM_VIDEO_HDLR_MSG_CA_NFY:
        _sm_video_hdlr_do_msg_ca_nfy(
            t_msg.h_stream,
            pt_vs_obj,
            &t_msg.u.t_msg_ca );
        break;

    case SM_VIDEO_HDLR_MSG_SCDB_NFY:
        _sm_video_hdlr_do_msg_scdb_nfy(
            t_msg.h_stream,
            pt_vs_obj,
            &t_msg.u.t_msg_scdb );
        break;

    case SM_VIDEO_HDLR_MSG_VBI_DATA_NFY:
        _sm_video_hdlr_do_msg_vbi_data_nfy(
            t_msg.h_stream,
            pt_vs_obj,
            &t_msg.u.t_msg_vbi_data );
        break;

    case SM_VIDEO_HDLR_MSG_TVD_CC_DATA:
        _sm_video_hdlr_do_msg_tvd_cc_data_nfy(
            t_msg.h_stream,
            pt_vs_obj,
            &t_msg.u.t_msg_tvd_cc );
        break;

    case SM_VIDEO_HDLR_MSG_CC_IND_TIMER:
        _sm_video_hdlr_do_msg_cc_ind_timer(
            t_msg.h_stream,
            pt_vs_obj,
            &t_msg.u.t_msg_cc_ind );
        break;

    case SM_VIDEO_HDLR_MSG_TYPE_IGNORE:
    default:
        SM_VS_DBG_ERR((
            "[SM] VS, invalid video handler message type = 0x%.8X\r\n",
            t_msg.e_msg_type ));
        SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
    }

    SM_UNLOCK( pt_vs_obj->t_base.h_lock );
}

/*-----------------------------------------------------------------------------
 * Name: sm_video_hdlr_init
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
INT32 sm_video_hdlr_init( VOID )
{
    INT32               i4_ret = SMR_OK;
    SM_HDLR_DESC_T      t_hdlr_desc;
    SM_HDLR_FCT_TBL_T   t_fct_tbl;

    x_strcpy( t_hdlr_desc.s_hdlr_name, SM_VIDEO_STRM_HDLR_NAME );
    t_hdlr_desc.ui4_strm_type_mask = ST_MASK_VIDEO;

    t_fct_tbl.pf_open = &sm_video_hdlr_open_fct;
    t_fct_tbl.pf_close = &sm_video_hdlr_close_fct;
    t_fct_tbl.pf_set = &sm_video_hdlr_set_fct;
    t_fct_tbl.pf_get = &sm_video_hdlr_get_fct;
    t_fct_tbl.pf_task = &sm_video_hdlr_task_fct;

    /* Create message queue for Video Handler Task */
    if ( gh_video_hdlr_msgq == NULL_HANDLE )
    {
        i4_ret = x_msg_q_create( &gh_video_hdlr_msgq,
                                 SM_VIDEO_STRM_HDLR_NAME,  /* use thread name */
                                 sizeof( SM_VIDEO_HDLR_MSG_T ),
                                 SM_VIDEO_HDLR_MSGQ_MAX_SIZE );
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
                                  &gh_video_handler );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INTERNAL_ERROR;
    }

    return SMR_OK;
}

INT32 x_sm_video_hdlr_reg_vbi_psr(
    SM_VSH_VBI_TYPE_T           e_vbi_type,
    x_sm_vsh_vbi_psr_open_fct   pf_vbi_open,
    x_sm_vsh_vbi_parse_fct      pf_vbi_parse,
    x_sm_vsh_vbi_get_fct        pf_vbi_get,
    x_sm_vsh_vbi_psr_close_fct  pf_vbi_close )
{
    CRIT_STATE_T   t_cs;

    if ( e_vbi_type == SM_VSH_VBI_TYPE_UNKNOWN ||
         !pf_vbi_open || !pf_vbi_parse || !pf_vbi_close )
    {
        return SMR_INV_ARG;
    }
    if ( t_vsh_vbi_psr_fct_tbl.e_vbi_type != SM_VSH_VBI_TYPE_UNKNOWN )
    {
        return SMR_ALREADY_INIT;
    }

    t_cs = x_crit_start();

    t_vsh_vbi_psr_fct_tbl.e_vbi_type = e_vbi_type;
    t_vsh_vbi_psr_fct_tbl.pf_vbi_open = pf_vbi_open;
    t_vsh_vbi_psr_fct_tbl.pf_vbi_parse = pf_vbi_parse;
    t_vsh_vbi_psr_fct_tbl.pf_vbi_get = pf_vbi_get;
    t_vsh_vbi_psr_fct_tbl.pf_vbi_close = pf_vbi_close;

    x_crit_end( t_cs );

    return SMR_OK;
}

VOID x_sm_video_hdlr_nfy_vbi_data(
    HANDLE_T                    h_vs,
    VSH_VBI_PSR_ID_T            t_vbi_psr_id,
    VOID*                       pv_vbi_data,
    SIZE_T                      z_vbi_data_size )
{
    SM_VIDEO_HDLR_MSG_T         t_msg;

    if ( t_vsh_vbi_psr_fct_tbl.e_vbi_type == SM_VSH_VBI_TYPE_UNKNOWN )
    {
        return;
    }

    if ( z_vbi_data_size > MAX_VBI_DATA_SIZE )
    {
        SM_VS_DBG_ERR((
            "[SM] VS, x_sm_video_hdlr_nfy_vbi_info() invalid size = %u > %u\r\n",
            z_vbi_data_size, MAX_VBI_DATA_SIZE ));
        SM_ASSERT(0);
        return;
    }

    t_msg.e_msg_type = SM_VIDEO_HDLR_MSG_VBI_DATA_NFY;
    t_msg.h_stream = h_vs;

    t_msg.u.t_msg_vbi_data.e_vbi_type   = t_vsh_vbi_psr_fct_tbl.e_vbi_type;
    t_msg.u.t_msg_vbi_data.t_vbi_psr_id = t_vbi_psr_id;
    t_msg.u.t_msg_vbi_data.z_data_size  = z_vbi_data_size;
    x_memcpy( t_msg.u.t_msg_vbi_data.aui1_data,
              pv_vbi_data,
              z_vbi_data_size );

    sm_vs_send_msg( &t_msg );

    return;
}
