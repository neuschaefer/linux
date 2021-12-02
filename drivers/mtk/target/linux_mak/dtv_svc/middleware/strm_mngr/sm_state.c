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
 * $RCSfile: sm_state.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/10 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: d908c026a37c5be69fa4ecb1520bc593 $
 *
 * Description:
 *         This header file contains Stream Manager state machine specific
 *         definitions.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "handle/x_handle.h"
#include "strm_mngr/x_sm.h"
#include "strm_mngr/sm_state.h"
#include "strm_mngr/_sm_utils.h"

#ifdef TIME_MEADUREMENT
#include "time_msrt/x_time_msrt.h"
#endif

#ifdef DEBUG
#include "strm_mngr/audio_hdlr/_sm_audio_hdlr.h"
#include "strm_mngr/video_hdlr/_sm_video_hdlr.h"
#include "strm_mngr/cc_hdlr/u_sm_cc_event.h"
#include "strm_mngr/cc_hdlr/_sm_cc_hdlr.h"
#include "strm_mngr/sess_gnrc_hdlr/sm_sess_gnrc.h"

#include "strm_mngr/sess_rcrd_hdlr/u_sm_sess_rcrd.h"
#include "strm_mngr/rcrd_strm_hdlr/u_sm_rcrd_hdlr.h"

#include "strm_mngr/sess_rcrd_hdlr/_sm_sess_rcrd.h"
#include "strm_mngr/rcrd_strm_hdlr/_sm_rcrd_hdlr.h"
#ifdef MW_TV_AV_BYPASS_SUPPORT
#include "strm_mngr/bypass_hdlr/u_sm_bypass_event.h"
#include "strm_mngr/bypass_hdlr/_sm_bypass_hdlr.h"
#endif /* MW_TV_AV_BYPASS_SUPPORT */
#endif /* DEBUG */

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#ifdef DEBUG
typedef struct _EVN_NAME_PAIR_T
{
    UINT32    ui4_event;
    CHAR*     s_name;
} EVN_NAME_PAIR_T;
#endif /* DEBUG */

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

#ifdef SM_TIME_MEASURE
static UINT32            ui4_tick_res_ms = 0;
#endif /* SM_TIME_MEASURE */

static SM_STATE_COND_T   t_cond_null = SM_STATE_COND_NULL;
static SM_STATE_DESCR_T  t_state_descr_null = SM_STATE_DESCR_NULL;

#ifdef DEBUG
static EVN_NAME_PAIR_T as_event_name[] =
{
    /* Session */
    { SM_EVN_SESS_APP_OPEN_REQ,     "SM_EVN_SESS_APP_OPEN_REQ"},
    { SM_EVN_SESS_APP_PLAY_REQ,     "SM_EVN_SESS_APP_PLAY_REQ"},
    { SM_EVN_SESS_APP_STOP_REQ,     "SM_EVN_SESS_APP_STOP_REQ"},
    { SM_EVN_SESS_APP_CLOSE_REQ,    "SM_EVN_SESS_APP_CLOSE_REQ"},
    { SM_EVN_SESS_DRV_OPENED,       "SM_EVN_SESS_DRV_OPENED"},
    { SM_EVN_SESS_DRV_PLAYED,       "SM_EVN_SESS_DRV_PLAYED"},
    { SM_EVN_SESS_DRV_STOPPED,      "SM_EVN_SESS_DRV_STOPPED"},
    { SM_EVN_SESS_DRV_CLOSED,       "SM_EVN_SESS_DRV_CLOSED"},
    { SM_EVN_SESS_DRV_ERR,          "SM_EVN_SESS_DRV_ERR"},
    { SM_EVN_SESS_SCDB_CLOSED,      "SM_EVN_SESS_SCDB_CLOSED"},
    { SM_EVN_SESS_SCDB_UPDATE,      "SM_EVN_SESS_SCDB_UPDATE"},
    { SM_EVN_SESS_INTERNAL_ERR,     "SM_EVN_SESS_INTERNAL_ERR"},

    { SM_EVN_SESS_PCR_OPENED,       "SM_EVN_SESS_PCR_OPENED"},
    { SM_EVN_SESS_PCR_CLOSED,       "SM_EVN_SESS_PCR_CLOSED"},
    { SM_EVN_SESS_PCR_CONNECTED,    "SM_EVN_SESS_PCR_CONNECTED"},
    { SM_EVN_SESS_PCR_DISCONNECTED, "SM_EVN_SESS_PCR_DISCONNECTED"},
    { SM_EVN_SESS_PCR_PID_UPDATE,   "SM_EVN_SESS_PCR_PID_UPDATE"},
    { SM_EVN_SESS_PM_COND_CLOSED,   "SM_EVN_SESS_PM_COND_CLOSED"},
    { SM_EVN_SESS_PM_COND_PLAYED,   "SM_EVN_SESS_PM_COND_PLAYED"},
    { SM_EVN_SESS_PM_COND_STOPPED,  "SM_EVN_SESS_PM_COND_STOPPED"},
    { SM_EVN_SESS_STRM_ADD,         "SM_EVN_SESS_STRM_ADD"},
    { SM_EVN_SESS_STRM_DEL,         "SM_EVN_SESS_STRM_DEL"},
    { SM_EVN_SESS_STRM_ALL_OPENED,  "SM_EVN_SESS_STRM_ALL_OPENED"},
    { SM_EVN_SESS_STRM_ALL_PLAYED,  "SM_EVN_SESS_STRM_ALL_PLAYED"},
    { SM_EVN_SESS_STRM_ALL_CLOSED,  "SM_EVN_SESS_STRM_ALL_CLOSED"},
    { SM_EVN_SESS_STRM_ERR,         "SM_EVN_SESS_STRM_ERR"},

    /* VS */
    { SM_EVN_VS_APP_OPEN_REQ,       "SM_EVN_VS_APP_OPEN_REQ"},
    { SM_EVN_VS_APP_PLAY_REQ,       "SM_EVN_VS_APP_PLAY_REQ"},
    { SM_EVN_VS_APP_STOP_REQ,       "SM_EVN_VS_APP_STOP_REQ"},
    { SM_EVN_VS_APP_CLOSE_REQ,      "SM_EVN_VS_APP_CLOSE_REQ"},
    { SM_EVN_VS_DRV_OPENED,         "SM_EVN_VS_DRV_OPENED"},
    { SM_EVN_VS_DRV_PLAYED,         "SM_EVN_VS_DRV_PLAYED"},
    { SM_EVN_VS_DRV_STOPPED,        "SM_EVN_VS_DRV_STOPPED"},
    { SM_EVN_VS_DRV_CLOSED,         "SM_EVN_VS_DRV_CLOSED"},
    { SM_EVN_VS_DRV_ERR,            "SM_EVN_VS_DRV_ERR"},
    { SM_EVN_VS_SCDB_RECORD_DEL,    "SM_EVN_VS_SCDB_RECORD_DEL"},
    { SM_EVN_VS_SCDB_RECORD_MOD,    "SM_EVN_VS_SCDB_RECORD_MOD"},
    { SM_EVN_VS_INTERNAL_ERR,       "SM_EVN_VS_INTERNAL_ERR"},
    { SM_EVN_VS_VIDEO_RESOLUTION_CHG,       "SM_EVN_VS_VIDEO_RESOLUTION_CHG"},
    { SM_EVN_VS_AUTO_CLK_POS_PHS_DONE,      "SM_EVN_VS_AUTO_CLK_POS_PHS_DONE"},
    { SM_EVN_VS_AUTO_COLOR_DONE,            "SM_EVN_VS_AUTO_COLOR_DONE"},
    { SM_EVN_VS_ANA_TV_SIGNAL,              "SM_EVN_VS_ANA_TV_SIGNAL"},
    { SM_EVN_VS_ANA_COLOR_SYSTEM,           "SM_EVN_VS_ANA_COLOR_SYSTEM"},
    { SM_EVN_VS_ANA_CONTENT_ADV_INFO,       "SM_EVN_VS_ANA_CONTENT_ADV_INFO"},
    { SM_EVN_VS_CC_INDICATOR,               "SM_EVN_VS_CC_INDICATOR"},
    { SM_EVN_VS_ANA_TV_SIGNAL_DETECTING,    "SM_EVN_VS_ANA_TV_SIGNAL_DETECTING"},
    { SM_EVN_VS_DIG_DECODE_STATUS_CHG,      "SM_EVN_VS_DIG_DECODE_STATUS_CHG"},
    { SM_EVN_VS_SPEED_CHG,                  "SM_EVN_VS_SPEED_CHG"},
    { SM_EVN_VS_EOS,                        "SM_EVN_VS_EOS"},
#ifdef DIVX_PLUS_CER 
    { SM_EVN_VS_CHAP_DONE,                  "SM_EVN_VS_CHAP_DONE"},
#endif 
    { SM_EVN_VS_RESTART,                    "SM_EVN_VS_RESTART"},
    { SM_EVN_VS_CODEC_NOT_SUPPORT,          "SM_EVN_VS_CODEC_NOT_SUPPORT"},
    { SM_EVN_VS_THUMBNAIL_DONE,             "SM_EVN_VS_THUMBNAIL_DONE"},
    { SM_EVN_VS_HDCP_CONTENT,               "SM_EVN_VS_HDCP_CONTENT"},
    { SM_EVN_VS_MACROVISION,                "SM_EVN_VS_MACROVISION"},
    { SM_EVN_VS_DISP_FRAME,                 "SM_EVN_VS_DISP_FRAME"},
    { SM_EVN_VS_BIT_RATE,                   "SM_EVN_VS_BIT_RATE"},
    { SM_EVN_VS_VIDEO_PLAY_DONE,            "SM_EVN_VS_VIDEO_PLAY_DONE"},
    { SM_EVN_VS_FRAMERATE_NOT_SUPPORT,      "SM_EVN_VS_FRAMERATE_NOT_SUPPORT"},
    { SM_EVN_VS_AUD_PROTECT_INFO,           "SM_EVN_VS_AUD_PROTECT_INFO"},

    { SM_EVN_VS_DEC_OPENED,         "SM_EVN_VS_DEC_OPENED"},
    { SM_EVN_VS_DEC_CLOSED,         "SM_EVN_VS_DEC_CLOSED"},
    { SM_EVN_VS_DEC_CONNECTED,      "SM_EVN_VS_DEC_CONNECTED"},
    { SM_EVN_VS_DEC_DISCONNECTED,   "SM_EVN_VS_DEC_DISCONNECTED"},
    { SM_EVN_VS_DEC_PLAYED,         "SM_EVN_VS_DEC_PLAYED"},
    { SM_EVN_VS_DEC_STOPPED,        "SM_EVN_VS_DEC_STOPPED"},

    { SM_EVN_VS_VP_OPENED,          "SM_EVN_VS_VP_OPENED"},
    { SM_EVN_VS_VP_CLOSED,          "SM_EVN_VS_VP_CLOSED"},
    { SM_EVN_VS_VP_CONNECTED,       "SM_EVN_VS_VP_CONNECTED"},
    { SM_EVN_VS_VP_DISCONNECTED,    "SM_EVN_VS_VP_DISCONNECTED"},
    { SM_EVN_VS_VP_PLAYED,          "SM_EVN_VS_VP_PLAYED"},
    { SM_EVN_VS_VP_STOPPED,         "SM_EVN_VS_VP_STOPPED"},

    { SM_EVN_VS_PMX_OPENED,         "SM_EVN_VS_PMX_OPENED"},
    { SM_EVN_VS_PMX_CLOSED,         "SM_EVN_VS_PMX_CLOSED"},
    { SM_EVN_VS_PMX_CONNECTED,      "SM_EVN_VS_PMX_CONNECTED"},
    { SM_EVN_VS_PMX_DISCONNECTED,   "SM_EVN_VS_PMX_DISCONNECTED"},
    { SM_EVN_VS_PMX_PLAYED,         "SM_EVN_VS_PMX_PLAYED"},
    { SM_EVN_VS_PMX_STOPPED,        "SM_EVN_VS_PMX_STOPPED"},

    { SM_EVN_VS_TVE_OPENED,         "SM_EVN_VS_TVE_OPENED"},
    { SM_EVN_VS_TVE_CLOSED,         "SM_EVN_VS_TVE_CLOSED"},
    { SM_EVN_VS_TVE_CONNECTED,      "SM_EVN_VS_TVE_CONNECTED"},
    { SM_EVN_VS_TVE_DISCONNECTED,   "SM_EVN_VS_TVE_DISCONNECTED"},
    { SM_EVN_VS_TVE_PLAYED,         "SM_EVN_VS_TVE_PLAYED"},
    { SM_EVN_VS_TVE_STOPPED,        "SM_EVN_VS_TVE_STOPPED"},

    { SM_EVN_VS_PM_COND_CLOSED,     "SM_EVN_VS_PM_COND_CLOSED"},
    { SM_EVN_VS_PM_COND_PLAYED,     "SM_EVN_VS_PM_COND_PLAYED"},
    { SM_EVN_VS_PM_COND_STOPPED,    "SM_EVN_VS_PM_COND_STOPPED"},
    
    /* AS */
    { SM_EVN_AS_APP_OPEN_REQ,       "SM_EVN_AS_APP_OPEN_REQ"},
    { SM_EVN_AS_APP_PLAY_REQ,       "SM_EVN_AS_APP_PLAY_REQ"},
    { SM_EVN_AS_APP_STOP_REQ,       "SM_EVN_AS_APP_STOP_REQ"},
    { SM_EVN_AS_APP_CLOSE_REQ,      "SM_EVN_AS_APP_CLOSE_REQ"},
    { SM_EVN_AS_DRV_OPENED,         "SM_EVN_AS_DRV_OPENED"},
    { SM_EVN_AS_DRV_PLAYED,         "SM_EVN_AS_DRV_PLAYED"},
    { SM_EVN_AS_DRV_STOPPED,        "SM_EVN_AS_DRV_STOPPED"},
    { SM_EVN_AS_DRV_CLOSED,         "SM_EVN_AS_DRV_CLOSED"},
    { SM_EVN_AS_DRV_ERR,              "SM_EVN_AS_DRV_ERR"},
    { SM_EVN_AS_INTERNAL_ERR,       "SM_EVN_AS_INTERNAL_ERR"},
    { SM_EVN_AS_SCDB_RECORD_DEL,    "SM_EVN_AS_SCDB_RECORD_DEL"},
    { SM_EVN_AS_SCDB_RECORD_MOD,    "SM_EVN_AS_SCDB_RECORD_MOD"},
    { SM_EVN_AS_AUD_INFO_CHG,       "SM_EVN_AS_AUD_INFO_CHG"},
    { SM_EVN_AS_RESTART,            "SM_EVN_AS_RESTART"},
    { SM_EVN_AS_DECODE_STATUS_CHG,  "SM_EVN_AS_DECODE_STATUS_CHG"},
    { SM_EVN_AS_EOS,                "SM_EVN_AS_EOS"},
    { SM_EVN_AS_CODEC_NOT_SUPPORT,  "SM_EVN_AS_CODEC_NOT_SUPPORT"},
    { SM_EVN_AS_SIG_MONITOR_TM,     "SM_EVN_AS_SIG_MONITOR_TM"},
    { SM_EVN_AS_SIG_MONITOR_AUTO_MUTE, "SM_EVN_AS_SIG_MONITOR_AUTO_MUTE"},
    

    { SM_EVN_AS_AD_OPENED,          "SM_EVN_AS_AD_OPENED"},
    { SM_EVN_AS_AD_CLOSED,          "SM_EVN_AS_AD_CLOSED"},
    { SM_EVN_AS_AD_CONNECTED,       "SM_EVN_AS_AD_CONNECTED"},
    { SM_EVN_AS_AD_DISCONNECTED,    "SM_EVN_AS_AD_DISCONNECTED"},
    { SM_EVN_AS_AD_PLAYED,          "SM_EVN_AS_AD_PLAYED"},
    { SM_EVN_AS_AD_STOPPED,         "SM_EVN_AS_AD_STOPPED"},
    { SM_EVN_AS_PM_COND_CLOSED,     "SM_EVN_AS_PM_COND_CLOSED"},
    { SM_EVN_AS_PM_COND_PLAYED,     "SM_EVN_AS_PM_COND_PLAYED"},
    { SM_EVN_AS_PM_COND_STOPPED,    "SM_EVN_AS_PM_COND_STOPPED"},

    /* CA */
    { SM_EVN_CA_COND_CLOSED,        "SM_EVN_CA_COND_CLOSED"},
    { SM_EVN_CA_COND_UNKNOWN,       "SM_EVN_CA_COND_UNKNOWN"},
    { SM_EVN_CA_COND_FREE,          "SM_EVN_CA_COND_FREE"},
    { SM_EVN_CA_COND_AUTHORIZED,    "SM_EVN_CA_COND_AUTHORIZED"},
    { SM_EVN_CA_COND_AUTHORIZED_CA_SYS_ID, "SM_EVN_CA_COND_AUTHORIZED_CA_SYS_ID"},
    { SM_EVN_CA_COND_NOT_AUTHORIZED,"SM_EVN_CA_COND_NOT_AUTHORIZED"},

    /* CCI */
    { SM_EVN_CCI_INFORMATION,       "SM_EVN_CCI_INFORMATION"},

    /* CCS */
    { SM_EVN_CCS_APP_OPEN_REQ,      "SM_EVN_CCS_APP_OPEN_REQ"},
    { SM_EVN_CCS_APP_PLAY_REQ,      "SM_EVN_CCS_APP_PLAY_REQ"},
    { SM_EVN_CCS_APP_STOP_REQ,      "SM_EVN_CCS_APP_STOP_REQ"},
    { SM_EVN_CCS_APP_CLOSE_REQ,     "SM_EVN_CCS_APP_CLOSE_REQ"},
    { SM_EVN_CCS_DRV_OPENED,        "SM_EVN_CCS_DRV_OPENED"},
    { SM_EVN_CCS_DRV_PLAYED,        "SM_EVN_CCS_DRV_PLAYED"},
    { SM_EVN_CCS_DRV_STOPPED,       "SM_EVN_CCS_DRV_STOPPED"},
    { SM_EVN_CCS_DRV_CLOSED,        "SM_EVN_CCS_DRV_CLOSED"},
    { SM_EVN_CCS_DRV_ERR,           "SM_EVN_CCS_DRV_ERR"},
    { SM_EVN_CCS_SCDB_RECORD_DEL,   "SM_EVN_CCS_SCDB_RECORD_DEL"},
    { SM_EVN_CCS_SCDB_RECORD_MOD,   "SM_EVN_CCS_SCDB_RECORD_MOD"},
    { SM_EVN_CCS_INTERNAL_ERR,      "SM_EVN_CCS_INTERNAL_ERR"},

    { SM_EVN_CCS_DEC_OPENED,        "SM_EVN_CCS_DEC_OPENED"},
    { SM_EVN_CCS_DEC_CLOSED,        "SM_EVN_CCS_DEC_CLOSED"},
    { SM_EVN_CCS_DEC_CC_PLAYED,     "SM_EVN_CCS_DEC_CC_PLAYED"},
    { SM_EVN_CCS_DEC_CC_STOPPED,    "SM_EVN_CCS_DEC_CC_STOPPED"},
    { SM_EVN_CCS_TVE_OPENED,        "SM_EVN_CCS_TVE_OPENED"},
    { SM_EVN_CCS_TVE_CLOSED,        "SM_EVN_CCS_TVE_CLOSED"},
    { SM_EVN_CCS_TVE_PLAYED,        "SM_EVN_CCS_TVE_PLAYED"},
    { SM_EVN_CCS_TVE_STOPPED,       "SM_EVN_CCS_TVE_STOPPED"},

#ifdef MW_TV_AV_BYPASS_SUPPORT
    /* BYPASS */
    { SM_EVN_BYPASS_APP_OPEN_REQ,   "SM_EVN_BYPASS_APP_OPEN_REQ"},
    { SM_EVN_BYPASS_APP_PLAY_REQ,   "SM_EVN_BYPASS_APP_PLAY_REQ"},
    { SM_EVN_BYPASS_APP_STOP_REQ,   "SM_EVN_BYPASS_APP_STOP_REQ"},
    { SM_EVN_BYPASS_APP_CLOSE_REQ,  "SM_EVN_BYPASS_APP_CLOSE_REQ"},
    { SM_EVN_BYPASS_DRV_OPENED,     "SM_EVN_BYPASS_DRV_OPENED"},
    { SM_EVN_BYPASS_DRV_PLAYED,     "SM_EVN_BYPASS_DRV_PLAYED"},
    { SM_EVN_BYPASS_DRV_STOPPED,    "SM_EVN_BYPASS_DRV_STOPPED"},
    { SM_EVN_BYPASS_DRV_CLOSED,     "SM_EVN_BYPASS_DRV_CLOSED"},
    { SM_EVN_BYPASS_DRV_ERR,        "SM_EVN_BYPASS_DRV_ERR"},
    { SM_EVN_BYPASS_SCDB_RECORD_DEL, "SM_EVN_BYPASS_SCDB_RECORD_DEL"},
    { SM_EVN_BYPASS_INTERNAL_ERR,   "SM_EVN_BYPASS_INTERNAL_ERR"},
    { SM_EVN_BYPASS_VIDEO_RESOLUTION_CHG,       "SM_EVN_BYPASS_VIDEO_RESOLUTION_CHG"},
    { SM_EVN_BYPASS_ANA_TV_SIGNAL,              "SM_EVN_BYPASS_ANA_TV_SIGNAL"},
    { SM_EVN_BYPASS_ANA_TV_SIGNAL_DETECTING,    "SM_EVN_BYPASS_ANA_TV_SIGNAL_DETECTING"},
    { SM_EVN_BYPASS_ANA_COLOR_SYSTEM,           "SM_EVN_BYPASS_ANA_COLOR_SYSTEM"},
    { SM_EVN_BYPASS_RESTART,                    "SM_EVN_BYPASS_RESTART"},
    { SM_EVN_BYPASS_HDCP_CONTENT,               "SM_EVN_BYPASS_HDCP_CONTENT"},
    { SM_EVN_BYPASS_MACROVISION,                "SM_EVN_BYPASS_MACROVISION"},
    { SM_EVN_BYPASS_CODEC_NOT_SUPPORT,          "SM_EVN_BYPASS_CODEC_NOT_SUPPORT"},

    { SM_EVN_BYPASS_DEC_OPENED,     "SM_EVN_BYPASS_DEC_OPENED"},
    { SM_EVN_BYPASS_DEC_CLOSED,     "SM_EVN_BYPASS_DEC_CLOSED"},
    { SM_EVN_BYPASS_DEC_CONNECTED,  "SM_EVN_BYPASS_DEC_CONNECTED"},
    { SM_EVN_BYPASS_DEC_DISCONNECTED, "SM_EVN_BYPASS_DEC_DISCONNECTED"},
    { SM_EVN_BYPASS_DEC_PLAYED,     "SM_EVN_BYPASS_DEC_PLAYED"},
    { SM_EVN_BYPASS_DEC_STOPPED,    "SM_EVN_BYPASS_DEC_STOPPED"},
    { SM_EVN_BYPASS_TVE_OPENED,     "SM_EVN_BYPASS_TVE_OPENED"},
    { SM_EVN_BYPASS_TVE_CLOSED,     "SM_EVN_BYPASS_TVE_CLOSED"},
    { SM_EVN_BYPASS_TVE_CONNECTED,  "SM_EVN_BYPASS_TVE_CONNECTED"},
    { SM_EVN_BYPASS_TVE_DISCONNECTED, "SM_EVN_BYPASS_TVE_DISCONNECTED"},
    { SM_EVN_BYPASS_TVE_PLAYED,     "SM_EVN_BYPASS_TVE_PLAYED"},
    { SM_EVN_BYPASS_TVE_STOPPED,    "SM_EVN_BYPASS_TVE_STOPPED"},
    { SM_EVN_BYPASS_VOUT_OPENED,    "SM_EVN_BYPASS_VOUT_OPENED"},
    { SM_EVN_BYPASS_VOUT_CLOSED,    "SM_EVN_BYPASS_VOUT_CLOSED"},
    { SM_EVN_BYPASS_VOUT_CONNECTED, "SM_EVN_BYPASS_VOUT_CONNECTED"},
    { SM_EVN_BYPASS_VOUT_DISCONNECTED, "SM_EVN_BYPASS_VOUT_DISCONNECTED"},
    { SM_EVN_BYPASS_VOUT_PLAYED,    "SM_EVN_BYPASS_VOUT_PLAYED"},
    { SM_EVN_BYPASS_VOUT_STOPPED,   "SM_EVN_BYPASS_VOUT_STOPPED"},

    { SM_EVN_BYPASS_PM_COND_CLOSED,  "SM_EVN_BYPASS_PM_COND_CLOSED"},
    { SM_EVN_BYPASS_PM_COND_PLAYED,  "SM_EVN_BYPASS_PM_COND_PLAYED"},
    { SM_EVN_BYPASS_PM_COND_STOPPED, "SM_EVN_BYPASS_PM_COND_STOPPED"},
#endif

#ifdef SYS_RECORD_SUPPORT
    /*Record Session*/
    { SM_EVN_SESS_RCRD_APP_OPEN_REQ,   "SM_EVN_SESS_RCRD_APP_OPEN_REQ"},
    { SM_EVN_SESS_RCRD_APP_PLAY_REQ,   "SM_EVN_SESS_RCRD_APP_PLAY_REQ"},
    { SM_EVN_SESS_RCRD_APP_STOP_REQ,   "SM_EVN_SESS_RCRD_APP_STOP_REQ"},
    { SM_EVN_SESS_RCRD_APP_CLOSE_REQ,  "SM_EVN_SESS_RCRD_APP_CLOSE_REQ"},
    { SM_EVN_SESS_RCRD_ALL_OPENED,     "SM_EVN_SESS_RCRD_ALL_OPENED"},
    { SM_EVN_SESS_RCRD_ALL_PLAYED,     "SM_EVN_SESS_RCRD_ALL_PLAYED"},
    { SM_EVN_SESS_RCRD_ALL_STOPPED,    "SM_EVN_SESS_RCRD_ALL_STOPPED"},
    { SM_EVN_SESS_RCRD_ALL_CLOSED,     "SM_EVN_SESS_RCRD_ALL_CLOSED"},
    { SM_EVN_SESS_RCRD_COMP_ERR,       "SM_EVN_SESS_RCRD_COMP_ERR"},
    { SM_EVN_SESS_RCRD_SCDB_CLOSED,    "SM_EVN_SESS_RCRD_SCDB_CLOSED"},
    { SM_EVN_SESS_RCRD_SCDB_UPDATE,    "SM_EVN_SESS_RCRD_SCDB_UPDATE"},
    { SM_EVN_SESS_RCRD_INTERNAL_ERR,   "SM_EVN_SESS_RCRD_INTERNAL_ERR"},
    { SM_EVN_SESS_RCRD_PVR_CTRL_OPENED,"SM_EVN_SESS_RCRD_PVR_CTRL_OPENED"},
    { SM_EVN_SESS_RCRD_PVR_CTRL_OVERFLOW,"SM_EVN_SESS_RCRD_PVR_CTRL_OPENED"},
    { SM_EVN_SESS_RCRD_PVR_CTRL_FLUSHED, "SM_EVN_SESS_RCRD_PVR_CTRL_OPENED"},
    { SM_EVN_SESS_RCRD_REACH_BOUND,      "SM_EVN_SESS_RCRD_REACH_BOUND"},
    { SM_EVN_SESS_RCRD_STRG_FULL,        "SM_EVN_SESS_RCRD_STRG_FULL"},
    
    { SM_EVN_SESS_RCRD_PVR_OPENED,       "SM_EVN_SESS_RCRD_PVR_OPENED"},
    { SM_EVN_SESS_RCRD_PVR_PLAYED,       "SM_EVN_SESS_RCRD_PVR_PLAYED"},
    { SM_EVN_SESS_RCRD_PVR_STOPPED,      "SM_EVN_SESS_RCRD_PVR_STOPPED"},
    { SM_EVN_SESS_RCRD_PVR_OVERFLOW,     "SM_EVN_SESS_RCRD_PVR_OVERFLOW"},
    { SM_EVN_SESS_RCRD_PVR_FLUSHED,      "SM_EVN_SESS_RCRD_PVR_FLUSHED"},
    { SM_EVN_SESS_RCRD_PVR_ERR,          "SM_EVN_SESS_RCRD_PVR_ERR"},
    { SM_EVN_SESS_RCRD_PVR_CLOSED,       "SM_EVN_SESS_RCRD_PVR_CLOSED"},

    { SM_EVN_SESS_RCRD_PM_COND_OPENED,     "SM_EVN_SESS_RCRD_PM_COND_OPENED"},
    { SM_EVN_SESS_RCRD_PM_COND_PLAYED,     "SM_EVN_SESS_RCRD_PM_COND_PLAYED"},
    { SM_EVN_SESS_RCRD_PM_COND_STOPPED,    "SM_EVN_SESS_RCRD_PM_COND_STOPPED"},
    { SM_EVN_SESS_RCRD_PM_COND_CLOSED,     "SM_EVN_SESS_RCRD_PM_COND_CLOSED"},
    { SM_EVN_SESS_RCRD_PM_COND_ERR,        "SM_EVN_SESS_RCRD_PM_COND_ERR"},
    
    { SM_EVN_SESS_RCRD_RUTI_OPENED,     "SM_EVN_SESS_RCRD_RUTI_OPENED"},
    { SM_EVN_SESS_RCRD_RUTI_READY,      "SM_EVN_SESS_RCRD_RUTI_READY"},
    { SM_EVN_SESS_RCRD_RUTI_TRANSITION, "SM_EVN_SESS_RCRD_RUTI_TRANSITION"},
    { SM_EVN_SESS_RCRD_RUTI_PLAYED,     "SM_EVN_SESS_RCRD_RUTI_PLAYED"},
    { SM_EVN_SESS_RCRD_RUTI_STOPPED,    "SM_EVN_SESS_RCRD_RUTI_STOPPED"},
    { SM_EVN_SESS_RCRD_RUTI_CLOSED,     "SM_EVN_SESS_RCRD_RUTI_CLOSED"},
    { SM_EVN_SESS_RCRD_RUTI_REACH_BOUND,   "SM_EVN_SESS_RCRD_RUTI_REACH_BOUND"},
    { SM_EVN_SESS_RCRD_RUTI_STRG_FULL,     "SM_EVN_SESS_RCRD_RUTI_STRG_FULL"},
    { SM_EVN_SESS_RCRD_RUTI_ERR,        "SM_EVN_SESS_RCRD_RUTI_ERR"},

    { SM_EVN_SESS_RCRD_STRM_ADD,          "SM_EVN_SESS_RCRD_STRM_ADD"},
    { SM_EVN_SESS_RCRD_STRM_DEL,          "SM_EVN_SESS_RCRD_STRM_DEL"},
    { SM_EVN_SESS_RCRD_STRM_OPENED,       "SM_EVN_SESS_RCRD_STRM_OPENED"},
    { SM_EVN_SESS_RCRD_STRM_PLAYED,       "SM_EVN_SESS_RCRD_STRM_PLAYED"},
    { SM_EVN_SESS_RCRD_STRM_CLOSED,       "SM_EVN_SESS_RCRD_STRM_CLOSED"},
    { SM_EVN_SESS_RCRD_STRM_ALL_OPENED,   "SM_EVN_SESS_RCRD_STRM_ALL_OPENED"},
    { SM_EVN_SESS_RCRD_STRM_ALL_PLAYED,   "SM_EVN_SESS_RCRD_STRM_ALL_PLAYED"},
    { SM_EVN_SESS_RCRD_STRM_ALL_CLOSED,   "SM_EVN_SESS_RCRD_STRM_ALL_CLOSED"},
    { SM_EVN_SESS_RCRD_STRM_ERR,          "SM_EVN_SESS_RCRD_STRM_ERR"},

    /*Record Stream*/
    { SM_EVN_RSH_APP_OPEN_REQ,          "SM_EVN_RSH_APP_OPEN_REQ"},
    { SM_EVN_RSH_APP_PLAY_REQ,          "SM_EVN_RSH_APP_PLAY_REQ"},
    { SM_EVN_RSH_APP_STOP_REQ,          "SM_EVN_RSH_APP_STOP_REQ"},
    { SM_EVN_RSH_APP_CLOSE_REQ,         "SM_EVN_RSH_APP_CLOSE_REQ"},
    { SM_EVN_RSH_ALL_OPENED,            "SM_EVN_RSH_ALL_OPENED"},
    { SM_EVN_RSH_ALL_PLAYED,            "SM_EVN_RSH_ALL_PLAYED"},
    { SM_EVN_RSH_ALL_STOPPED,           "SM_EVN_RSH_ALL_STOPPED"},
    { SM_EVN_RSH_ALL_CLOSED,            "SM_EVN_RSH_ALL_CLOSED"},
    { SM_EVN_RSH_COMP_ERR,              "SM_EVN_RSH_COMP_ERR"},
    { SM_EVN_RSH_SCDB_RECORD_DEL,       "SM_EVN_RSH_SCDB_RECORD_DEL"},
    { SM_EVN_RSH_SCDB_RECORD_MOD,       "SM_EVN_RSH_SCDB_RECORD_MOD"},
    { SM_EVN_RSH_SCDB_CLOSED,           "SM_EVN_RSH_SCDB_CLOSED"},
    { SM_EVN_RSH_INTERNAL_ERR,          "SM_EVN_RSH_INTERNAL_ERR"},
    { SM_EVN_RSH_PMT_PID_UPDATED,       "SM_EVN_RSH_PMT_PID_UPDATED"},
    { SM_EVN_RSH_PAT_SWAPED,            "SM_EVN_RSH_PAT_SWAPED"},
    { SM_EVN_RSH_PMT_SWAPED,            "SM_EVN_RSH_PMT_SWAPED"},
    
    /*{ SM_EVN_RSH_PM_COND_OPENED,        "SM_EVN_RSH_PM_COND_OPENED"},*/
    { SM_EVN_RSH_PM_COND_PLAYED,        "SM_EVN_RSH_PM_COND_PLAYED"},
    { SM_EVN_RSH_PM_COND_STOPPED,       "SM_EVN_RSH_PM_COND_STOPPED"},
    { SM_EVN_RSH_PM_COND_CLOSED,        "SM_EVN_RSH_PM_COND_CLOSED"},
    { SM_EVN_RSH_PM_COND_ERR,           "SM_EVN_RSH_PM_COND_ERR"},

    { SM_EVN_RSH_PVR_OPENED,            "SM_EVN_RSH_PVR_OPENED"},
    { SM_EVN_RSH_PVR_PLAYED,            "SM_EVN_RSH_PVR_PLAYED"},
    { SM_EVN_RSH_PVR_STOPPED,           "SM_EVN_RSH_PVR_STOPPED"},
    { SM_EVN_RSH_PVR_CONNECTED,         "SM_EVN_RSH_PVR_CONNECTED"},
    { SM_EVN_RSH_PVR_DISCONNECTED,      "SM_EVN_RSH_PVR_DISCONNECTED"},
    { SM_EVN_RSH_PVR_CLOSED,            "SM_EVN_RSH_PVR_CLOSED"},
    
    { SM_EVN_RSH_RUTI_OPENED,           "SM_EVN_RSH_RUTI_OPENED"},
    { SM_EVN_RSH_RUTI_READY,            "SM_EVN_RSH_RUTI_READY"},
    { SM_EVN_RSH_RUTI_TRANSITION,       "SM_EVN_RSH_RUTI_TRANSITION"},
    { SM_EVN_RSH_RUTI_CLOSED,           "SM_EVN_RSH_RUTI_CLOSED"},
    { SM_EVN_RSH_RUTI_ERR,              "SM_EVN_RSH_RUTI_ERR"},
#endif

    /* !!! Do NOT remove the following line !!! */
    { SM_EVN_IGNORE, NULL }
};
#endif /* DEBUG */

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
-----------------------------------------------------------------------------*/

static SM_STATE_COND_T* _sm_state_get_matched_cond(
    P_SM_STATE_T                pt_state,
    UINT32                      ui4_event )
{
    SM_STATE_COND_T*  pt_cond_iter = pt_state->pt_cond_lst;

    while( x_memcmp( pt_cond_iter, &t_cond_null, sizeof(SM_STATE_COND_T) ) != 0 )
    {
        if ( ( pt_cond_iter->ui4_curr_state == pt_state->ui4_curr_state ||
               pt_cond_iter->ui4_curr_state == SM_STATE_IGNORE ) &&
             pt_cond_iter->ui4_event == ui4_event )
        {
            /* matched */
            return pt_cond_iter;
        }
        ++pt_cond_iter;
    }

    return NULL;
}

static SM_STATE_DESCR_T* _sm_state_get_descr(
    P_SM_STATE_T                pt_state,
    UINT32                      ui4_state )
{
    SM_STATE_DESCR_T* pt_state_iter = pt_state->pt_state_lst;

    while( x_memcmp( pt_state_iter, &t_state_descr_null, sizeof(SM_STATE_DESCR_T) ) != 0 )
    {
        if ( pt_state_iter->ui4_state == ui4_state )
        {
            /* matched */
            return pt_state_iter;
        }
        ++pt_state_iter;
    }

    return NULL;
}

static BOOL _sm_state_is_allowed_pending_event(
    P_SM_STATE_T                pt_state,
    UINT32                      ui4_event )
{
    INT32      i4_i = 0;

    while( pt_state->pui4_allowed_pending_events[i4_i] != SM_EVN_IGNORE )
    {
        if ( pt_state->pui4_allowed_pending_events[i4_i] == ui4_event )
        {
            return TRUE;
        }

        ++i4_i;
    }

    return FALSE;
}

static INT32 _sm_state_add_pending_event(
    P_SM_STATE_T                pt_state,
    UINT32                      ui4_event,
    VOID*                       pv_tag1,
    VOID*                       pv_tag2 )
{
    UINT32 ui4_i = 0;

    for( ui4_i = 0; ui4_i < SM_MAX_NUM_PENDING_EVN; ++ui4_i )
    {
        if ( pt_state->aui4_pending_event[ui4_i] == SM_EVN_IGNORE )
        {
            pt_state->aui4_pending_event[ui4_i] = ui4_event;
            pt_state->apv_pending_event_tag1[ui4_i] = pv_tag1;
            pt_state->apv_pending_event_tag2[ui4_i] = pv_tag2;
            return SMR_PENDING;
        }
    }

    return SMR_NOT_ALLOW;
}

static INT32 _sm_state_remove_pending_event(
    P_SM_STATE_T                pt_state,
    UINT32*                     pui4_event,
    VOID**                      ppv_tag1,
    VOID**                      ppv_tag2 )
{
    if ( pt_state->aui4_pending_event[0] != SM_EVN_IGNORE )
    {
        *pui4_event = pt_state->aui4_pending_event[0];
        *ppv_tag1 = pt_state->apv_pending_event_tag1[0];
        *ppv_tag2 = pt_state->apv_pending_event_tag2[0];

        x_memmove( &pt_state->aui4_pending_event[0],
                   &pt_state->aui4_pending_event[1],
                   sizeof(UINT32) * (SM_MAX_NUM_PENDING_EVN - 1) );
        x_memmove( &pt_state->apv_pending_event_tag1[0],
                   &pt_state->apv_pending_event_tag1[1],
                   sizeof(VOID*) * (SM_MAX_NUM_PENDING_EVN - 1) );
        x_memmove( &pt_state->apv_pending_event_tag2[0],
                   &pt_state->apv_pending_event_tag2[1],
                   sizeof(VOID*) * (SM_MAX_NUM_PENDING_EVN - 1) );

        pt_state->aui4_pending_event[SM_MAX_NUM_PENDING_EVN-1] = SM_EVN_IGNORE;
        pt_state->apv_pending_event_tag1[SM_MAX_NUM_PENDING_EVN-1] = NULL;
        pt_state->apv_pending_event_tag2[SM_MAX_NUM_PENDING_EVN-1] = NULL;

        return SMR_OK;
    }

    *pui4_event = SM_EVN_IGNORE;
    *ppv_tag1 = NULL;
    *ppv_tag2 = NULL;

    return SMR_NOT_ALLOW;
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
CHAR* sm_state_event_to_str( UINT32 ui4_event )
{
#ifdef DEBUG
    EVN_NAME_PAIR_T* pt_evn_name = &as_event_name[0];

    while ( !( pt_evn_name->ui4_event == SM_EVN_IGNORE &&
               pt_evn_name->s_name == NULL ) )
    {
        if ( pt_evn_name->ui4_event == ui4_event )
        {
            return pt_evn_name->s_name;
        }
        ++pt_evn_name;
    }

#endif /* DEBUG */
    return NULL;
}

INT32 sm_state_init(
    const SM_STATE_COND_T*     pt_state_cond_lst,
    const SM_STATE_DESCR_T*    pt_state_lst,
    const UINT32*              pui4_allowed_pending_events,
    sm_state_notify_fct        pf_state_nfy,
    VOID*                      pv_state_nfy_tag,
    P_SM_STATE_T               pt_state )
{
    UINT32   ui4_i;

    if ( !pt_state_cond_lst           ||
         !pt_state_lst                ||
         !pui4_allowed_pending_events ||
         !pt_state )
    {
        return SMR_INV_ARG;
    }

    pt_state->pt_cond_lst = (SM_STATE_COND_T*)pt_state_cond_lst;
    pt_state->pt_state_lst = (SM_STATE_DESCR_T*)pt_state_lst;
    pt_state->pui4_allowed_pending_events = (UINT32*)pui4_allowed_pending_events;
    pt_state->ui4_curr_state = SM_STATE_IGNORE;

    for( ui4_i = 0; ui4_i < SM_MAX_NUM_PENDING_EVN; ++ui4_i )
    {
        pt_state->aui4_pending_event[ui4_i] = SM_EVN_IGNORE;
        pt_state->apv_pending_event_tag1[ui4_i] = NULL;
        pt_state->apv_pending_event_tag2[ui4_i] = NULL;
    }
    pt_state->pf_state_nfy = pf_state_nfy;
    pt_state->pv_state_nfy_tag = pv_state_nfy_tag;

    return SMR_OK;
}

INT32 sm_state_on_event(
    P_SM_STATE_T               pt_state,
    UINT32                     ui4_event,
    VOID*                      pv_tag1,
    VOID*                      pv_tag2,
    UINT32*                    pui4_next_state )
{
    INT32              i4_ret = SMR_OK, i4_ret_2 = SMR_INV_ARG;
    UINT32             ui4_previous_state = 0;
    SM_STATE_COND_T*   pt_cond = NULL;
    SM_STATE_DESCR_T*  pt_state_descr = NULL;

    if ( !pt_state || ui4_event == SM_EVN_IGNORE )
    {
        return SMR_INV_ARG;
    }

#ifdef DEBUG
    {
        const CHAR* ps_name = sm_state_event_to_str(ui4_event);

        if ( ps_name )
        {
            SM_SLCTR_DBG_INFO(( "[SM] EVENT(%s)\r\n", ps_name ));
        }
        else
        {
            SM_SLCTR_DBG_INFO(( "[SM] EVENT(0x%.8X)\r\n", ui4_event ));
        }
    }
#endif /* DEBUG */


    pt_state_descr = _sm_state_get_descr( pt_state, pt_state->ui4_curr_state );
    if ( !pt_state_descr )
    {
        return SMR_CAN_NOT_OP_IN_THIS_STATE;
    }


    pt_cond = _sm_state_get_matched_cond( pt_state, ui4_event );
    if ( !pt_cond )
    {
        if ( pt_state_descr->pf_state_do_exception )
        {
            i4_ret = pt_state_descr->pf_state_do_exception(
                ui4_event,
                pt_state->ui4_curr_state,
                pv_tag1,
                pv_tag2 );

            return i4_ret;
        }

        return SMR_CAN_NOT_OP_IN_THIS_STATE;
    }

    if ( pt_cond->pf_event_do )
    {
        pt_cond->pf_event_do( pt_state->ui4_curr_state,
                              ui4_event,
                              pt_cond->ui4_next_state,
                              pv_tag1,
                              pv_tag2 );
    }

    if ( pt_state->ui4_curr_state != pt_cond->ui4_next_state &&
         pt_cond->ui4_next_state != SM_STATE_IGNORE )
    {
        ui4_previous_state = pt_state->ui4_curr_state;
        pt_state->ui4_curr_state = pt_cond->ui4_next_state;
        if ( pui4_next_state )
        {
            *pui4_next_state = pt_cond->ui4_next_state;
        }

        /* Get new state's descriptor */
        pt_state_descr = _sm_state_get_descr( pt_state, pt_state->ui4_curr_state );
        if ( !pt_state_descr )
        {
            return SMR_CAN_NOT_OP_IN_THIS_STATE;
        }

        if ( pt_state->pf_state_nfy )
        {
            pt_state->pf_state_nfy( pt_state->pv_state_nfy_tag,
                                    pt_state->ui4_curr_state,
                                    ui4_event,
                                    0 );

#ifdef TIME_MEASUREMENT
#endif /* TIME_MEASUREMENT */
        }

        if ( pt_state_descr->pf_state_do )
        {
            pt_state_descr->pf_state_do( ui4_previous_state,
                                         ui4_event,
                                         pt_state->ui4_curr_state,
                                         pv_tag1,
                                         pv_tag2 );
        }
    }

    if ( pt_cond->ui4_pending_event != SM_EVN_IGNORE )
    {
        if ( pt_state_descr->b_allow_pending &&
             _sm_state_is_allowed_pending_event( pt_state, ui4_event ) )
        {
            return _sm_state_add_pending_event(
                        pt_state,
                        pt_cond->ui4_pending_event,
                        pv_tag1,
                        pv_tag2 );
        }
        else
        {
            return SMR_CAN_NOT_OP_IN_THIS_STATE;
        }
    }

    while ( pt_state->aui4_pending_event[0] != SM_EVN_IGNORE &&
            pt_state_descr->b_prcss_pending &&
            pt_cond->ui4_next_state != SM_STATE_IGNORE &&
            i4_ret_2 < SMR_OK )
    {
        UINT32 ui4_pending_event;
        VOID*  pv_pending_event_tag1;
        VOID*  pv_pending_event_tag2;

        i4_ret = _sm_state_remove_pending_event(
            pt_state,
            &ui4_pending_event,
            &pv_pending_event_tag1,
            &pv_pending_event_tag2 );
        if ( i4_ret != SMR_OK )
        {
            break;
        }

#ifdef DEBUG
        {
            const CHAR* ps_name = sm_state_event_to_str(ui4_pending_event);
            INT32 i4_i;

            SM_SLCTR_DBG_INFO(( "0x%.8x {", (UINT32)pt_state ));
            for ( i4_i = 0; i4_i <SM_MAX_NUM_PENDING_EVN; i4_i++ )
            {
                SM_SLCTR_DBG_INFO(( "0x%.8X ", pt_state->aui4_pending_event[i4_i] ));
            }
            SM_SLCTR_DBG_INFO(( "}\r\n" ));

            if ( !ps_name )
            {
                SM_SLCTR_DBG_INFO((
                            "[SM] Process pending EVENT >> 0x%.8X\r\n", ui4_pending_event ));
            }
            else
            {
                SM_SLCTR_DBG_INFO((
                            "[SM] Process pending EVENT >> %s\r\n", ps_name ));
            }
        }
#endif /* DEBUG */

        i4_ret_2 = sm_state_on_event( pt_state,
                                      ui4_pending_event,
                                      pv_pending_event_tag1,
                                      pv_pending_event_tag2,
                                      pui4_next_state );
        i4_ret = i4_ret_2;
    }

    return i4_ret;
}
