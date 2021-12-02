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
 * $RCSfile: svctx_dbg.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/4 $
 * $SWAuthor: Weider Chang $
 * $MD5HEX: 14da3451bc05a6efe3530391331bb355 $
 *
 * Description:
 *---------------------------------------------------------------------------*/
#include "inc/u_common.h"
#include "dbg/u_dbg.h"
#include "dbg/def_dbg_level_mw.h"
#include "time_msrt/x_time_msrt.h"

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
CHAR* s_g_brdcst_types[] = {
            "BRDCST_TYPE_UNKNOWN",
            "BRDCST_TYPE_ANALOG",
            "BRDCST_TYPE_DVB",
            "BRDCST_TYPE_ATSC",
            "BRDCST_TYPE_SCTE",
            "BRDCST_TYPE_ISDB",
            "BRDCST_TYPE_FMRDO",
            "BRDCST_TYPE_DTMB"
            };
CHAR* s_g_state_strings[] = {
            "POOLED",
            "READY",
            "PENDING",
            "PRESENTING",
            "PENDING_SELECT_SAME_REQ",
            "PRESENTING_SELECT_SAME_REQ",
            "LAST_VALID_ENTRY",
            "INVALID",
            "---"   /* ST_UNKNOWN = -1 */
            };
CHAR* s_g_strm_types[] = {
            "AUDIO",
            "VIDEO",
            "CC",
            "ISDB_CC",
            "ISDB_SI",
            "TTX",
            "SBTL",
            "DATA",
            "BYPASS",
            "MM_SBTL",
            "RCRD_STRM"
            };
CHAR* s_g_strm_req_types[] = {
            "SCDB",
            "PID"
            };
CHAR* s_g_svctx_ntfy_codes[] = {
            /* service */
            "NORMAL",
            "SVCTX_THREAD_KICK_OFF",
            "DELIVERY_SYSTEM_CHANGE",
            "SIGNAL_DETECTING",
            "SIGNAL_LOCKED",
            "SIGNAL_LOSS",
            "INTERRUPTED",
            "SERVICE_CHANGING",
            "OWN_SERVICE_CHANGING",
            "SERVICE_BLOCKED",
            "SIGNAL_LOSS_AS_BLOCKED",
            "SIGNAL_LOCKED_AS_BLOCKED",
            "SERVICE_UNBLOCKED",
            "STOPPED",
            "IN_STOPPING",

            /* current event */
            "CRNT_EVN_START_QUERY",
            "CRNT_EVN_NOT_READY",
            "CRNT_EVN_READY",

            /* service type */
            "NO_AUDIO_VIDEO_SVC",
            "AUDIO_ONLY_SVC",
            "VIDEO_ONLY_SVC",
            "AUDIO_VIDEO_SVC",
            "SCRAMBLED_AUDIO_VIDEO_SVC",
            "SCRAMBLED_AUDIO_CLEAR_VIDEO_SVC",
            "SCRAMBLED_AUDIO_NO_VIDEO_SVC",
            "SCRAMBLED_VIDEO_CLEAR_AUDIO_SVC",
            "SCRAMBLED_VIDEO_NO_AUDIO_SVC",
            "AUDIO_ONLY_STRM",
            "VIDEO_ONLY_STRM",

            /* cc availability */
            "WITH_CC_AVIL_INDICATOR",
            "NO_CC_AVIL_INDICATOR",
            "EXISTED_CC_IND_CHG",

            /* stream */
            "NO_ANY_STREAM",
            "NO_STREAM",
            "STREAM_STOPPED",
            "STREAM_STARTED",
            "VIDEO_FMT_UPDATE",
            "AUDIO_FMT_UPDATE",
            "VIDEO_AUTO_CLK_POS_PHS_DONE",
            "VIDEO_AUTO_COLOR_DONE",
            "VIDEO_ANA_COLOR_SYSTEM",
            "STREAM_STOPPED_AS_BLOCKED",
            "STREAM_STARTED_AS_BLOCKED",
            "VIDEO_FMT_UPDATE_AS_BLOCKED",
            "AUDIO_DMUTE_AMUTE",
            "AUDIO_DUNMUTE_AMUTE",
            "AUDIO_DUNMUTE_AUNMUTE",
            "AUDIO_DMUTE_AUNMUTE",

            "AUDIO_AUTO_MUTE",
            "AUDIO_AUTO_UNMUTE",
            "VIDEO_AUTO_MUTE",
            "VIDEO_AUTO_UNMUTE",
            "VIDEO_PLAY_DONE",
            /* SCDB (PMT) */
            "SCDB_ADD",
            "SCDB_DEL",
            "SCDB_MODIFY",

            /* underneath middlewares' error */
            "NO_RESOURCES",
            "INTERNAL_ERROR",

            /* CA/CP */
            "CA",
            "CP",

            /* tuner singal notify for PAL/SECAM */
            "TUNER_SIGNAL_LOCKED",
            "TUNER_SIGNAL_LOSS",

            /* scart bypass - only sent when analog channel for EU */
            "CH_SRC_FROM_TUNER",
            "CH_SRC_FROM_SCART",

            /* single RF scan required */
            "RF_SCAN_REQUIRED",

            /* unfreeze by SVCTX*/
            "UNFREEZE",

            /* for analog whatever system detected */
            "TV_SYS_DETECTED",
            "AUD_SYS_DETECTED",

            /* for Riks TV */
            "START_ALTER_FREQ_TUNE",
            "END_ALTER_FREQ_TUNE",
            "START_FIRST_TUNE",
            "END_FIRST_TUNE",
            "START_FREQ_LIST_UPDATED_CHK",
            "END_FREQ_LIST_UPDATED_CHK",

            /*Multimedia relative events*/
            "MEDIA_LOST",
            "MEDIA_PRE_PROCESSING",
            "MEDIA_PRE_PROCESSED",
            "MEDIA_PB_RANGE_ALLOC",
            "MEDIA_ERROR",
            "MEDIA_CHAPTER_UPDATE",
            "MEDIA_TITLE_UPDATE",
            "MEDIA_DURATION_UPDATE",
            "MEDIA_SPEED_UPDATE",
            "MEDIA_AB_REPEAT_DONE",
            "MEDIA_UOP_INVALID",
            "MEDIA_SEEK_READY",
            "MEDIA_SEEK_ERROR",
            "MEDIA_REPEAT_DONE",
            "MEDIA_MENU_PRESNT",
            "MEDIA_EOS",
            "MEDIA_AS_CODEC_NOT_SUPPORT",
            "MEDIA_VS_CODEC_NOT_SUPPORT",
            "MEDIA_THUMBNAIL_DONE",
            "MEDIA_HD_NOT_SUPPORT",
            "MEDIA_DRM_NOT_SUPPORT",
            "MEDIA_SUBTITLE_FMT_UPDATE",
            "REACH_VALID_RANGE_START",
            "REACH_VALID_RANGE_END",
            "MEDIA_BAD_TICK_Y_TO_N",
            "MEDIA_BAD_TICK_N_TO_Y",
            "MEDIA_FILE_NOT_SUPPORT",
            "MEDIA_RESOLUTION_NOT_SUPPORT",
            "MEDIA_FRAMERATE_NOT_SUPPORT",
            "MEDIA_NO_CONTENT",
            "MEDIA_RANGE_AB_DONE",            
            "MEDIA_VIDEO_PRESENT",
            "MEDIA_VIDEO_BIG_RATE",
            "MEDIA_REPLAY",
            "MEDIA_URI_FILE_OPENED",
            "MEDIA_DESC_ALLOCED",
            /* input source relative events */
            "INPUT_SIGNAL_UNKNOWN",

            /* Copy protection */
            "HDCP_DETECT",
            "HDCP_NOT_DETECT",
            "MACROVISION_DETECT",
            "MACROVISION_NOT_DETECT",

            /* ISDB */
#ifdef MW_ISDB_SUPPORT
            "EWS_ON",
            "EWS_OFF",
            "BRDCSTING_SVC",
            "NON_BRDCSTING_SVC",
#endif

            /* record */
#ifdef SYS_RECORD_SUPPORT    
            "REC_STARTED",
            "REC_STOPPED",
            "REC_PAUSED",
            "REC_REQ_ES_LIST",
            "REC_ES_AUTH_UPDATED",
            "REC_REACH_BOUND",
            "REC_NOT_AUTHORIZED",
            "REC_BLOCKED",
            "REC_STRG_FULL",
            "REC_BITRATE_TOO_HIGH",
            "REC_NOT_SUPPORTED",
            "REC_STRG_ERROR",
            "REC_INTERNAL_ERROR",
            "REC_BAD_TICK",
#endif
            "CHAPTER_CHANGED",
            "CONNECTION_ANA_FREQ",
            "ANA_AUTO_DETECTED_AUD_SYS",
            "BUFFER_READY",
            "TIME_UPDATED",
            "UNDERFLOW",
            "EOF",
            "STREAM_OPENED",
            "VIDEO_NUM_READY",

            "DRM_NONE",
            "DRM_UNKNOWN_ERR",
            "DRM_LIC_EXPIRATION_ERR",
            "DRM_SRVDATA_ACCESS_FAIL",
            "DRM_INV_BUSINESS_TKN",
            "DRM_CERT_EXPIRED",
            "DRM_LIC_NOT_ALLOW",
            "DRM_SRV_END",
            "DRM_SRV_UNAVAIL",
            "DRM_SRV_BUSY",
            "DRM_DEV_NOT_REG",
            "DRM_CONTENT_EXHAUSTED",
            "DRM_UNKNOWN_SRV_ERR",
            "DRM_INV_USR_AGENT",
            "DRM_UNSUPPORTED_VER",
            "DRM_INV_CONTENT_TYPE",
            "DRM_INV_SRV_TKN",
            "DRM_INV_ASSET_ID",
            "DRM_INIT_ERROR",
            "DRM_LICENSE_EXPIRED_ERROR",
            "DRM_LICENSE_ACQUISITION_ERROR",
            "DRM_LICENSE_SERVER_CONNECTION_ERROR",
            "DRM_TOKEN_ACQUISITION_ERROR",
            "DRM_CAD_ACQUISITION_ERROR"
            };
            
UINT8 ui1_g_ntfy_code_num = sizeof (s_g_svctx_ntfy_codes) / sizeof (CHAR *);
            
CHAR* s_g_snk_oper_types[] = {
            "SNK_OPER_TYPE_NONE",
            "SNK_OPER_TYPE_OPEN",
            "SNK_OPER_TYPE_PLAY",
            "SNK_OPER_TYPE_STOP",
            "SNK_OPER_TYPE_CLOSE"
            };
CHAR* s_g_src_oper_types[] = {
            "SRC_OPER_TYPE_NONE",
            "SRC_OPER_TYPE_CONNECT",
            "SRC_OPER_TYPE_DISCONNECT"
            };
CHAR* s_g_sess_conds[] = {
            "SM_SESS_COND_IGNORE",
            "SM_SESS_COND_OPENING",
            "SM_SESS_COND_OPENED",
            "SM_SESS_COND_STARTING",
            "SM_SESS_COND_STARTED",
            "SM_SESS_COND_STOPPING",
            "SM_SESS_COND_CLOSING",
            "SM_SESS_COND_CLOSED",
            "SM_SESS_COND_ERROR"
            };
CHAR* s_g_strm_conds[] = {
            "SM_COND_IGNORE",
            "SM_COND_OPENING",
            "SM_COND_OPENED",
            "SM_COND_STARTING",
            "SM_COND_STARTED",
            "SM_COND_STOPPING",
            "SM_COND_CLOSING",
            "SM_COND_CLOSED",
            "SM_COND_ERROR"
            };
CHAR* s_g_cm_conds[] = {
            "CM_COND_CLOSED",
            "CM_COND_OPENING",
            "CM_COND_OPENED",
            "CM_COND_CLOSING",
            "CM_COND_DISCONNECTED",
            "CM_COND_CONNECTING",
            "CM_COND_CONNECTED",
            "CM_COND_DISCONNECTING",
            "CM_COND_CONNECTED_TRANS",
            "CM_COND_CONNECTION_INFO"
            };
CHAR* s_g_scdb_conds[] = {
            "",
            "SCDB_UPDATING",
            "SCDB_UPDATED",
            "SCDB_CLOSED",
            "SCDB_UNKNOWN"
            };
CHAR* s_g_svc_block_conds[] = {
            "EXCEED_RATING",
            "USER_BLOCK",
            "PASSED",
            "PASSED_AND_NO_RATING_SET"
            };
CHAR* s_g_evctx_conds[] = {
            "EVCTX_COND_UNAVAIL",
            "EVCTX_COND_TRANSITION",
            "EVCTX_COND_AVAIL",
            "EVCTX_COND_ACTIVE",
            "EVCTX_COND_EXPIRED",
            "EVCTX_COND_UPDATED"
            };
CHAR* s_g_vsh_plane_order_ctrl[] = {
            "TOP",
            "UP",
            "DOWN",
            "SWAP",
            "BOTTOM"
            };
CHAR* s_g_cc_types[] = {
            "NTSC_LINE21",
            "DTVCC"
            };
CHAR* s_g_ntsc_cc_types[] = {
            "CC1",
            "CC2",
            "CC3",
            "CC4",
            "T1",
            "T2",
            "T3",
            "T4"
            };
CHAR* s_g_src_types[] = {
            "BRDCST_SVL",
            "BRDCST_FREQ",
            "?",
            "?",
            "AVC"
            };
CHAR* s_g_freq_info_types[] = {
            "DIG_TER",
            "DIG_CAB",
            "DIG_SAT",
            "ANA_TER",
            "ANA_CAB",
            "ANA_SAT"
            };
CHAR* s_g_player_ntfy_codes[] = {
            "PIPE_OPENED",
            "PIPE_CLOSED",
            "CONN_CONNECTED",
            "CONN_DISCONNECTED",
            "CONN_INFO_NTFY",
            "CONN_ANA_FREQ",
            "SCDB_UPDATED",
            "SESS_OPENED",
            "SESS_STARTED",
            "SESS_CLOSING",
            "SESS_STOPPING",
            "SCDB_OPENED",
#ifdef MW_TV_AV_BYPASS_SUPPORT
            "QUERY_ACTION",
            "CHECK_QUEUED_REQ",
            "CONN_FAKE_DISCONNECTED",
            "CONN_FAKE_CONNECTED",
#endif
            "REQ_SEND_MSG"
            };
CHAR* s_g_avc_comp_types[] = {
            "AVC_UNKNOWN",
            "AVC_COMP_VIDEO",   /* (DEV_AVC  |  0x00000001) */
            "AVC_S_VIDEO",
            "AVC_Y_PB_PR",
            "AVC_VGA",
            "AVC_SCART",
            "AVC_DVI",
            "AVC_HDMI",
            "AVC_AUDIO_INP",
            "AVC_SPDIF",
            "AVC_COMBI",
            "AVC_RESERVED"
            };
CHAR* s_g_tuner_types[] = {
            "TUNER",
            "TUNER_SAT_DIG",        /*(DEV_TUNER |  0x00000001)*/
            "TUNER_CAB_DIG",
            "TUNER_TER_DIG",
            "TUNER_SAT_ANA",
            "TUNER_CAB_ANA",
            "TUNER_TER_ANA",
            "TUNER_CAB_DIG_OOB_TX",
            "TUNER_CAB_DIG_OOB_RX",
            "UNKNOWN"
            };
CHAR* s_g_color_systems[] = {
            "NTSC",
            "PAL",
            "SECAM",
            "NTSC_443",
            "PAL_M",
            "PAL_N",
            "PAL_60"
            };
CHAR* s_g_aspect_ratios[] = {
            "?",
            "4:3",
            "16:9",
            "2.21:1"
            };
CHAR* s_g_audio_fmts[] = {
            "UNKNOWN",
            "MPEG",
            "AC3",
            "PCM",
            "MP3",
            "AAC",
            "DTS",
            "WMA",
            "RA",
            "HDCD",
            "MLP",
            "MTS",
            "TV_SYS"
            };
CHAR* s_g_audio_channels[] = {
            "UNKNOWN",
            "MONO",
            "MONO_SUB",
            "DUAL_MONO",
            "STEREO",
            "STEREO_SUB",
            "STEREO_DOLBY_SURROUND",
            "SURROUND_2CH",
            "SURROUND",
            "3_0",
            "4_0",
            "5_1",
            "7_1",
            "OTHERS"
            };
CHAR* s_g_signal_statuses[] = {
            "UNKNOWN",
            "LOCKED",
            "LOSS",
            "DETECTING"
            };
CHAR* s_g_vid_modes[] = {
            "NORMAL",
            "BLANK",
            "QUAD_VIDEO",
            "DEINT",
            "FREEZE"
            };
CHAR* s_g_aud_mtss[] = {
            "MTS_UNKNOWN",
            "MTS_MONO",
            "MTS_STEREO",
            "MTS_SUB_LANG"
            };
CHAR* s_g_av_statuses[] = {
            "AUDIO_ONLY",
            "VIDEO_ONLY",
            "AUDIO_VIDEO",
            "NO_AUDIO_VIDEO",
            "SCRAMBLED_AUDIO_VIDEO",
            "SCRAMBLED_AUDIO_CLEAR_VIDEO",
            "SCRAMBLED_AUDIO_NO_VIDEO",
            "SCRAMBLED_VIDEO_CLEAR_AUDIO",
            "SCRAMBLED_VIDEO_NO_AUDIO",
            "UNKNOWN"
            };
CHAR* s_g_scart_mode[] = {
            "UNKNOWN",
            "S_VIDEO",
            "COMPOSITE",
            "RGB",
            "MIX",
            "AUTO"
            };
CHAR* s_g_timing_types[] = {
            "UNKNOWN",
            "VIDEO",
            "GRAPHIC",
            "NOT_SUPPORT"
            };
CHAR* s_g_avc_hints[] = {
            "NONE",
            "SPECIFIC_DEVICE_ONLY",
            "SPECIFIC_DEVICE_FIRST",
            "BETTER_QUALITY_DEVICE_FIRST"
            };
CHAR* s_g_ch_decode_types[] = {
            "DISABLE",
            "MANUAL",
            "AUTO"
            };
CHAR* s_g_tuner_out_types[] = {
            "IGONRE",
            "DISABLE",
            "ENABLE"
            };
CHAR* s_g_audio_types[] = {
            "UNDEFINED",
            "CLEAN",
            "HEARING_IMPAIRED",
            "VISUAL_IMPAIRED",
            "RESERVED"
            };

CHAR* s_g_scrn_statuses[] = {
            "SINGLE",
            "MULTIPLE_FULL",
            "MULTIPLE_PART",
            "UNKNOWN"
            };
CHAR* s_g_scrn_modes[] = {
            "UNKNOWN",
            "NORMAL",
            "LETTERBOX",
            "PAN_SCAN",
            "SER_DEFINED",
            "NON_LINEAR_ZOOM",
            "DOT_BY_DOT",
            "USTOM_DEF_0",
            "CUSTOM_DEF_1",
            "CUSTOM_DEF_2",
            "CUSTOM_DEF_3",
            "CUSTOM_DEF_4",
            "CUSTOM_DEF_5",
            "CUSTOM_DEF_6",
            "CUSTOM_DEF_7",
            "NLZ_CUSTOM_DEF_0",
            "NLZ_CUSTOM_DEF_1",
            "NLZ_CUSTOM_DEF_2",
            "NLZ_CUSTOM_DEF_3"
            };
CHAR* s_g_block_rules[] = {
            "INP_SIGNAL_LOSS_CH_PROG",
            "INP_CH_SIGNAL_LOSS_PROG",
            "INP_CH_PROG_SIGNAL_LOSS"
            };
CHAR* s_g_auto_clr_conds[] = {
            "UNKNOWN",
            "SUCCESS",
            "FAILED"
            };
CHAR* s_g_auto_auto_clk_pos_phs_conds[] = {
            "UNKNOWN",
            "SUCCESS",
            "FAILED"
            };
CHAR* s_g_detection_modes[] = {
            "BY_SIGNAL_STATUS",
            "BY_CABLE_STATUS",
            "ASH_AUDIO_DECODE_UNKNOWN" /* -1 */
            };
CHAR* s_audio_decode_statuses[] = {
            "ASH_AUDIO_DECODE_NO_DATA",
            "ASH_AUDIO_DECODE_NORMAL",
            "ASH_AUDIO_DECODE_ERROR",
            "ASH_AUDIO_DECODE_NOT_SUPPORT",
            "ASH_AUDIO_DECODE_FMT_CONFLICT",
            "VSH_DIG_DECODE_UNKNOWN" /* -1 */
            };
CHAR* s_video_decode_statuses[] = {
            "VSH_DIG_DECODE_NO_DATA",
            "VSH_DIG_DECODE_NORMAL",
            "VSH_DIG_DECODE_ERROR",
            "VSH_DIG_DECODE_HD_NOT_SUPPORT",
            "VSH_DIG_DECODE_CODEC_NOT_SUPPORT"
            };
CHAR* s_g_strm_pending_statuses[] = {
            "STRM_PENDING_COND_NONE",
            "STRM_PENDING_COND_SELECT_STRM",
            "STRM_PENDING_COND_STOP_STRM"
            };
#ifdef MW_TV_AV_BYPASS_SUPPORT
CHAR* s_g_stream[] = {
    "AUDIO",
    "VIDEO",
    "CLOSED_CAPTION",
    "ISDB_CAPTION",
    "ISDB_TEXT",
    "TELETEXT",
    "SUBTITLE",
    "DATA",
    "BYPASS",
    "MM_SUBTITLE",
    "RCRD_STRM"
};

CHAR* s_g_player_action[] = {
    "PLAYER_ACTION_NONE",
    "PLAYER_ACTION_PLAY",
    "PLAYER_ACTION_STOP",
    "PLAYER_ACTION_PAUSE",
    "PLAYER_ACTION_PIPE_OPEN",
    "PLAYER_ACTION_PIPE_CLOSE",
    "PLAYER_ACTION_CONN_OPEN",
    "PLAYER_ACTION_CONN_CLOSE",
    "PLAYER_ACTION_SESS_OPEN",
    "PLAYER_ACTION_SESS_PLAY",
    "PLAYER_ACTION_SESS_CLOSE",
    "PLAYER_ACTION_RECONNECT"
};

CHAR* s_g_player_types[] = {
    "PLAYER_TYPE_CH_SVC",
    "PLAYER_TYPE_INP_SVC_VIDEO",
    "PLAYER_TYPE_INP_SVC_AUDIO"
};

CHAR* s_g_mgr_svc_select_types[] = {
    "SVC_SELECT_NONE",
    "SVC_SELECT_TV",
    "SVC_SELECT_AV",
    "SVC_SELECT_MM"
};

CHAR* s_g_mgr_op_types[] = {
    "OP_TYPE_NONE",
    "OP_TYPE_OPEN",
    "OP_TYPE_CLOSE"
};

CHAR* s_g_mgr_var_types[] = {
    "VAR_TYPE_SERVICE",
    "VAR_TYPE_PLAYER",
    "VAR_TYPE_STREAM"
};
#endif
CHAR* s_g_log_channel_change[] ={
"START_SELECT_SVC",
"REQUEST_CONNECT",
"RECEIVED_CONNECT",
"GOT_TV_SIGNAL",
"RECEIVED_RESOLUTION_CHG",
"SET_SVCTX_UNMUTE",
"SET_AP_UNMUTE"
};

CHAR* s_g_svctx_name[]= {
    "main_svctx",
    "sub_svctx",
    "tv_bypass_svctx",
    "mn_bypass_svctx"
};

UINT8  ui1_ch_xng_stage_num = sizeof (s_g_log_channel_change) / sizeof (CHAR *);
UINT32 g_ui4_ch_xng_stages_Log[28]; /* ui1_ch_xng_stage_num * 4 */
#endif

static UINT16  ui2_g_svctx_dbg_level  = DBG_LEVEL_NONE;
static UINT16  ui2_g_svctx_tms_level  = TMS_LEVEL_NONE;
/*-----------------------------------------------------------------------------
 * private methods declaration
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: svctx_get_dbg_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/
UINT16 svctx_get_dbg_level(VOID)
{
    return ui2_g_svctx_dbg_level;
}
/*-----------------------------------------------------------------------------
 * Name: svctx_set_dbg_level
 *
 * Description: This API set the debug level.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/
VOID svctx_set_dbg_level(
                    UINT16                      ui2_db_level
                    )
{
    ui2_g_svctx_dbg_level = ui2_db_level;
}
/*-----------------------------------------------------------------------------
 * Name: svctx_get_tms_level
 *
 * Description: This API returns the current setting of time measurement level.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/
UINT16 svctx_get_tms_level(VOID)
{
    return ui2_g_svctx_tms_level;
}
/*-----------------------------------------------------------------------------
 * Name: svctx_set_tms_level
 *
 * Description: This API set the time measurement level.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/
VOID svctx_set_tms_level(
                    UINT16                      ui2_tms_level
                    )
{
    ui2_g_svctx_tms_level = ui2_tms_level;
}
