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
 * $RCSfile: svctx_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_ATSC/5 $
 * $SWAuthor: Weider Chang $
 * $MD5HEX: 631271d39cbc47b5a9231693a0fbf6ae $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "cli/x_cli.h"
#include "os/inc/x_os.h"

#include "inc/common.h"
#include "inc/u_sys_name.h"
#include "conn_mngr/analog_brdcst/u_analog_brdcst.h"
#include "svctx/svctx.h"
#include "svctx/svctx_dbg.h"
#include "svctx/svctx_mgr.h"
#include "svctx/util/player.h"
#include "svctx/util/rating_chker.h"
#include "svctx/util/crnt_evn.h"
#include "svctx/util/strm_fmt_chker.h"
#include "svctx/svctx_cli.h"
#include "svctx/svctx_sm_broker.h"
#include "dbg/x_dbg_drv.h"

#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static INT32 _svctx_cli_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_ctrl_strm_select(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_dump_player_data(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_dump_strm_fmt_chker_data(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_dump_rating_chker_data(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_dump_crnt_evn_data(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_dump_video_data (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_dump_svctx_data (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_dump_svc_req_data (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_dump_av_profile_data (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_get_event_capable (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_stop_svc(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_dump_active_link_data (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_set_clock (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_set_phase (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_set_h_pos (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_set_v_pos (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_auto_clk_phs_pos (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_auto_color (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_tv_video_stream_broker (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_inp_video_stream_broker (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_open_scdb (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_open_stream (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_stop_stream (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_get_signal_lvl (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_set_scart_bypass (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_select_mpeg_stream (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_set_ntfy_trace (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_set_func_trace (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_set_player_ntfy_trace (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_set_strm_msg_trace (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_video_region_trace (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_test_video_display(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_stream_component_trace (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_select_ad(INT32 i4_argc, const CHAR** pps_argv);

extern UINT8 ui1_ch_xng_stages_Log[24];
static INT32 _svctx_cli_set_channel_change_trace(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_channel_change_time_dump(INT32 i4_argc, const CHAR** pps_argv);

#ifdef TIME_MEASUREMENT
static INT32 _svctx_cli_set_tms_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _svctx_cli_get_tms_level(INT32 i4_argc, const CHAR** pps_argv);
#endif /* TIME_MEASUREMENT */
/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL 
static CLI_EXEC_T at_svc_type_cmd_tbl[] =
{
    {
     "ctrl stream select",
     "css",
     _svctx_cli_ctrl_strm_select,
     NULL,
     "usage: css av|a|v y/n",
     CLI_SUPERVISOR
 },
 END_OF_CLI_CMD_TBL
};
#endif

static CLI_EXEC_T at_player_cmd_tbl[] =
{
    {   
        "dump",
        "d",
        _svctx_cli_dump_player_data,
        NULL,
        "usage: d using[u]|pooled[p]",
        CLI_GUEST
    },
    {   
        "ntfy trace",
        "nt",
        _svctx_cli_set_player_ntfy_trace,
        NULL,
        "usage: nt y|n",
        CLI_GUEST
    },
    END_OF_CLI_CMD_TBL
};

#ifdef CLI_LVL_ALL
static CLI_EXEC_T at_video_cmd_tbl[] =
{
    {   
        "dump",
        "d",
        _svctx_cli_dump_video_data,
        NULL,
        "usage: d",
        CLI_SUPERVISOR
    },
    {   
        "clock",
        "cl",
        _svctx_cli_set_clock,
        NULL,
        "usage: cl #value [svctx name]",
        CLI_SUPERVISOR
    },
    {   
        "phase",
        "ph",
        _svctx_cli_set_phase,
        NULL,
        "usage: ph #value [svctx name]",
        CLI_SUPERVISOR
    },
    {   
        "h_pos",
        "hp",
        _svctx_cli_set_h_pos,
        NULL,
        "usage: hp #value [svctx name]",
        CLI_SUPERVISOR
    },
    {   
        "v_pos",
        "vp",
        _svctx_cli_set_v_pos,
        NULL,
        "usage: vp #value [svctx name]",
        CLI_SUPERVISOR
    },
    {   
        "auto_clk_phs_pos",
        "acpp",
        _svctx_cli_auto_clk_phs_pos,
        NULL,
        "usage: acpp [svctx name]",
        CLI_SUPERVISOR
    },
    {   
        "auto_color",
        "ac",
        _svctx_cli_auto_color,
        NULL,
        "usage: ac [svctx name]",
        CLI_SUPERVISOR
    },
    {  
        "tv_video_broker",
        "tvb",
        _svctx_cli_tv_video_stream_broker,
        NULL,
        "usage: tvb SVC_LST_ID SVL_REC_ID",
        CLI_SUPERVISOR
    },
    {   "inp_video_broker",
        "ivb",
        _svctx_cli_inp_video_stream_broker,
        NULL,
        "usage: ivb src_name",
        CLI_SUPERVISOR
    },
     END_OF_CLI_CMD_TBL
};

static CLI_EXEC_T at_unit_test_cmd_tbl[] =
{
    {   
        "open_scdb",
        "oscdb",
        _svctx_cli_open_scdb,
        NULL,
        "usage: oscdb",
        CLI_SUPERVISOR
    },
    {   
        "open_stream",
        "ostrm",
        _svctx_cli_open_stream,
        NULL,
        "usage: ostrm",
        CLI_SUPERVISOR
    },
    {   
        "stop_stream",
        "sstrm",
        _svctx_cli_stop_stream,
        NULL,
        "usage: sstrm s(sync)|a(async) a(audio)|v(video) [svctx name]",
        CLI_SUPERVISOR
    },
    {   
        "open_mpeg",
        "ompeg",
        _svctx_cli_select_mpeg_stream,
        NULL,
        "usage: ompeg a(audio)|v(video)|s(subtitle) pid [svctx name]",
        CLI_SUPERVISOR
    },
    {   
        "signal level",
        "sl",
        _svctx_cli_get_signal_lvl,
        NULL,
        "usage: sl",
        CLI_SUPERVISOR
    },
    {   
        "scart bypass",
        "sb",
        _svctx_cli_set_scart_bypass,
        NULL,
        "usage: sb",
        CLI_SUPERVISOR
    },
    {   
        "select ad",
        "sad",
        _svctx_cli_select_ad,
        NULL,
        "usage: sad",
        CLI_SUPERVISOR
    },
    END_OF_CLI_CMD_TBL
};
#endif

static CLI_EXEC_T at_scdb_cmd_tbl[] =
{
    {   "dump",
    "d",
    svctx_cli_dump_scdb_data,
    NULL,
    "usage: d",
    CLI_GUEST
},
 END_OF_CLI_CMD_TBL
};

#ifdef CLI_LVL_ALL
static CLI_EXEC_T at_strm_fmt_chker_cmd_tbl[] =
{
    {   
        "dump",
        "d",
        _svctx_cli_dump_strm_fmt_chker_data,
        NULL,
        "usage: d using[u]|pooled[p]",
        CLI_SUPERVISOR
    },
    END_OF_CLI_CMD_TBL
};

static CLI_EXEC_T at_rating_chker_cmd_tbl[] =
{
    {   
        "dump",
        "d",
        _svctx_cli_dump_rating_chker_data,
        NULL,
        "usage: d using[u]|pooled[p]",
        CLI_SUPERVISOR
    },
    END_OF_CLI_CMD_TBL
};

static CLI_EXEC_T at_crnt_evn_cmd_tbl[] =
{
    {   
        "dump",
        "d",
        _svctx_cli_dump_crnt_evn_data,
        NULL,
        "usage: d using[u]|pooled[p]",
        CLI_SUPERVISOR
    },
    END_OF_CLI_CMD_TBL
};
#endif

static CLI_EXEC_T at_svctx_cmd_tbl[] =
{
    {
        CLI_GET_DBG_LVL_STR,
        NULL,
        _svctx_cli_get_dbg_level,
        NULL,
        CLI_GET_DBG_LVL_HELP_STR,
        CLI_GUEST
    },
    {
        CLI_SET_DBG_LVL_STR,
        NULL,
        _svctx_cli_set_dbg_level,
        NULL,
        CLI_SET_DBG_LVL_HELP_STR,
        CLI_GUEST
    },
    {   "dump",
        "d",
        _svctx_cli_dump_svctx_data,
        NULL,
        "usage: d using[u]|pooled[p]",
        CLI_GUEST
    },
    {   "dumpsvcreq",
        "ds",
        _svctx_cli_dump_svc_req_data,
        NULL,
        "usage: ds 0|1|...",
        CLI_GUEST
    },
    {   "activelink",
        "al",
        _svctx_cli_dump_active_link_data,
        NULL,
        "usage: al now|next",
        CLI_GUEST
    },
    {   "ntfy trace",
        "nt",
        _svctx_cli_set_ntfy_trace,
        NULL,
        "usage: nt y|n",
        CLI_GUEST
    },
    {   "func trace",
        "ft",
        _svctx_cli_set_func_trace,
        NULL,
        "usage: ft y|n",
        CLI_GUEST
    },
    {   "strm msg trace",
        "smt",
        _svctx_cli_set_strm_msg_trace,
        NULL,
        "usage: smt y|n",
        CLI_GUEST
    },
    {   "video region trace",
        "vrt",
        _svctx_cli_video_region_trace,
        NULL,
        "usage: vrt y|n",
        CLI_GUEST
    },
    {   "test video display",
        "vd",
        _svctx_cli_test_video_display,
        NULL,
        "usage: vd idx (v)edio/(d)isplay r g b",
        CLI_GUEST
    },
    {   "stream component trace",
        "sct",
        _svctx_cli_stream_component_trace,
        NULL,
        "usage: sct y|n",
        CLI_GUEST
    },
    {   "CH change time trace",
        "cct",
        _svctx_cli_set_channel_change_trace,
        NULL,
        "usage: cct y|n",
        CLI_GUEST
    },
    {   "cH change time dump",
        "cctd",
        _svctx_cli_channel_change_time_dump,
        NULL,
        "usage: cctd",
        CLI_GUEST
    },
    {   "player",
        "p",
        NULL,
        at_player_cmd_tbl,
        "Commands for Player",
        CLI_GUEST
    },
    {   "scdb",
        "scdb",
        NULL,
        at_scdb_cmd_tbl,
        "Commands for SCDB",
        CLI_GUEST
    },
    
#ifdef CLI_LVL_ALL
    {   
        "svc type",
        "st",
        NULL,
        at_svc_type_cmd_tbl,
        "Commands for Service Type",
        CLI_SUPERVISOR
    },    
    {   "av profile",
        "av",
        _svctx_cli_dump_av_profile_data,
        NULL,
        "usage: av",
        CLI_SUPERVISOR
    },    
    {   "get event capability",
        "gec",
        _svctx_cli_get_event_capable,
        NULL,
        "usage: gec",
        CLI_SUPERVISOR
    },        
    {   "Stop SVCTX",
        "stop",
        _svctx_cli_stop_svc,
        NULL,
        "usage: stop svctx_name(ex: stop main_svctx)",
        CLI_SUPERVISOR
    },      
    
    {   "strm fmt chker",
        "sfc",
        NULL,
        at_strm_fmt_chker_cmd_tbl,
        "Commands for Strm FMT chker",
        CLI_SUPERVISOR
    },
    {   "rating chker",
        "rc",
        NULL,
        at_rating_chker_cmd_tbl,
        "Commands for Rating Chker",
        CLI_SUPERVISOR
    },
    {   "crnt event",
        "ce",
        NULL,
        at_crnt_evn_cmd_tbl,
        "Commands for Crnt Event",
        CLI_SUPERVISOR
    },
    {   "video",
        "v",
        NULL,
        at_video_cmd_tbl,
        "Commands for Video",
        CLI_SUPERVISOR
    },
    {   "unit test",
        "ut",
        NULL,
        at_unit_test_cmd_tbl,
        "Commands for Unit Test",
        CLI_SUPERVISOR
    },
#ifdef TIME_MEASUREMENT
    {
        CLI_SET_TMS_LVL_STR,
        NULL,
        _svctx_cli_set_tms_level,
        NULL,
        CLI_SET_TMS_LVL_HELP_STR,
        CLI_SUPERVISOR
    },
    {
        CLI_GET_TMS_LVL_STR,
        NULL,
        _svctx_cli_get_tms_level,
        NULL,
        CLI_GET_TMS_LVL_HELP_STR,
        CLI_SUPERVISOR
    },
#endif /* TIME_MEASUREMENT */
#endif

    END_OF_CLI_CMD_TBL
};

/* Service Context root command table */
static CLI_EXEC_T at_svctx_root_cmd_tbl[] =
{
    {
        "svctx", 
        NULL, 
        NULL, 
        at_svctx_cmd_tbl, 
        "Service Context commands",
        CLI_GUEST
    },
    END_OF_CLI_CMD_TBL
};

static UINT32   ui4_g_css_strm_status       = 0;
static HANDLE_T h_g_main_svctx              = NULL_HANDLE;
static BOOL     b_g_svc_nfy_trace           = TRUE;
static BOOL     b_g_svc_func_trace          = FALSE;
static BOOL     b_g_svc_player_nfy_trace    = FALSE;
static BOOL     b_g_strm_msg_trace          = FALSE;
static BOOL     b_g_video_region_trace      = FALSE;
static CHAR     s_g_video_region_trace_name[SVCTX_NAME_MAX_SIZE+1];

static CHAR     s_g_svc_nfy_trace_name[SVCTX_NAME_MAX_SIZE+1];
static CHAR     s_g_svc_player_nfy_trace_name[SVCTX_SNK_NAME_MAX_SIZE+1];
static CHAR     s_g_strm_msg_trace_name[SVCTX_NAME_MAX_SIZE+1];

static UINT32   ui4_g_stream_component_idx_mask = 0;

static BOOL     b_g_log_channel_change_time  = TRUE;
static CHAR     s_g_channel_change_trace_name[SVCTX_NAME_MAX_SIZE+1];
/*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_get_dbg_level
 *
 * Description: This API gets the current debug level of Service Context.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_get_dbg_level (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32  i4_ret;

    i4_ret = x_cli_show_dbg_level(svctx_get_dbg_level());

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_set_dbg_level
 *
 * Description: This API sets the debug level of Service Context.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_set_dbg_level (INT32 i4_argc, const CHAR** pps_argv)
{
    UINT16 ui2_dbg_level;
    INT32  i4_ret;

    i4_ret = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);

    if (i4_ret == CLIR_OK){
        svctx_set_dbg_level(ui2_dbg_level);
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_ctrl_strm_select
 *
 * Description: This API control the audio/video stream select.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_ctrl_strm_select (INT32 i4_argc, const CHAR** pps_argv)
{
    BOOL b_disable = TRUE;

    if (i4_argc == 1) {
        ui4_g_css_strm_status = 0;
        x_dbg_stmt("Reset stream select control status.\r\n");
        return CLIR_OK;
    }

    if (i4_argc != 3) {
        x_dbg_stmt("Illegal command syntax!\r\n");
        return CLIR_OK;
    }

    if (x_strcmp("y", pps_argv[2]) == 0 || x_strcmp("Y", pps_argv[2]) == 0) {
        b_disable = TRUE;
    } else if (x_strcmp("n", pps_argv[2]) == 0 || x_strcmp("N", pps_argv[2]) == 0) {
        b_disable = FALSE;
    } else {
        x_dbg_stmt("Illegal command syntax!\r\n");
    }

    if (x_strcmp("a", pps_argv[1]) == 0
            || x_strcmp("av", pps_argv[1]) == 0
            || x_strcmp("va", pps_argv[1]) == 0) {
        if (b_disable == TRUE) {
            ui4_g_css_strm_status |= MAKE_BIT_MASK_32(ST_AUDIO);
            x_dbg_stmt("Disable Audio select\r\n");
        } else {
            ui4_g_css_strm_status &= ~MAKE_BIT_MASK_32(ST_AUDIO);
            x_dbg_stmt("Enable Audio select\r\n");
        }
    }

    if (x_strcmp("v", pps_argv[1]) == 0
            || x_strcmp("av", pps_argv[1]) == 0
            || x_strcmp("va", pps_argv[1]) == 0) {
        if (b_disable == TRUE) {
            ui4_g_css_strm_status |= MAKE_BIT_MASK_32(ST_VIDEO);
            x_dbg_stmt("Disable Video select\r\n");
        } else {
            ui4_g_css_strm_status &= ~MAKE_BIT_MASK_32(ST_VIDEO);
            x_dbg_stmt("Enable Video select\r\n");
        }
    }

    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_dump_av_profile_data
 *
 * Description: This API dump AV profile table
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_dump_av_profile_data (INT32 i4_argc, const CHAR** pps_argv)
{
    svctx_print_av_profile_table();
    return CLIR_OK;
}

static INT32 _svctx_cli_get_event_capable(INT32 i4_argc, const CHAR** pps_argv)
{
  SVCTX_CAPABILITY_T  t_capablility;
    
  if(i4_argc != 2){
    x_dbg_stmt("Invalid parameter.\r\n");
    x_dbg_stmt("Usage: gec event_key");    
     return CLIR_OK;
  }
  
  if (x_svctx_open("main_svctx", &h_g_main_svctx) != SVCTXR_OK) {
      x_dbg_stmt("x_svctx_open main_svctx failed\r\n");
      return CLIR_OK;
  }
  x_svctx_get_event_capability(h_g_main_svctx, (EVCTX_KEY_TYPE_T)x_strtoll(pps_argv[1], NULL, 0), &t_capablility);
  
  if(t_capablility == SVCTX_CAPABILITY_YES){
      x_dbg_stmt("has this event capablity\n");
  }else if(t_capablility == SVCTX_CAPABILITY_NO){
      x_dbg_stmt("has no this event capablity\n");
  }

  x_svctx_close(h_g_main_svctx);
  
  return CLIR_OK;
}


static INT32 _svctx_cli_stop_svc(INT32 i4_argc, const CHAR** pps_argv)
{
    HANDLE_T             h_svctx = NULL_HANDLE;
    CHAR*                ps_svctx_name = "main_svctx";
    INT32                i4_ret = CLIR_OK;
    if (i4_argc > 1) {
         ps_svctx_name = (CHAR*)pps_argv[1];
    }
    
    i4_ret = x_svctx_open(ps_svctx_name, &h_svctx);
    if (i4_ret != SVCTXR_OK) {
         x_dbg_stmt("x_svctx_open(%s) failed, i4_ret = %d\r\n", i4_ret, ps_svctx_name);
         return CLIR_OK;
    }   

    x_svctx_sync_stop_svc(h_svctx, DEFAULT_STOP_SVC_TIMEOUT);
    x_svctx_close(h_svctx);
   
   return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_dump_svctx_data
 *
 * Description: This API dump Player's data.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_dump_svctx_data (INT32 i4_argc, const CHAR** pps_argv)
{
    SVCTX_T* pt_using  = NULL;
    SVCTX_T* pt_pooled = NULL;
    CHAR*    pc_end_ptr;
    INT32    i4_idx = -1;

    if (i4_argc == 1) {
        pt_using  = svctx_get_using_head();
        pt_pooled = svctx_get_pooled_head();
    } else if (i4_argc == 2) {
        if (x_strcmp("u", pps_argv[1]) == 0  || x_strcmp("using", pps_argv[1]) == 0) {
            pt_using  = svctx_get_using_head();
        } else if (x_strcmp("p", pps_argv[1]) == 0  || x_strcmp("pooled", pps_argv[1]) == 0) {
            pt_pooled = svctx_get_pooled_head();
        } else {
            i4_idx = (INT32) x_strtoll(pps_argv[1], &pc_end_ptr, 0);
            if (pc_end_ptr != NULL && *pc_end_ptr != '\0')
            {
                x_dbg_stmt("Invalid parameter.\r\n");
                return CLIR_INV_CMD_USAGE;
            }
        }
    }

    if (pt_using != NULL) {
        x_dbg_stmt("[using svctxs]\r\n");
        for (; pt_using != NULL; pt_using = svctx_next(pt_using)) {
            svctx_print_self(pt_using);
        }
        x_dbg_stmt("\r\n");
    }

    if (pt_pooled != NULL) {
        x_dbg_stmt("[pooled svctxs]\r\n");
        for (; pt_pooled != NULL; pt_pooled = svctx_next(pt_pooled)) {
            x_dbg_stmt("svctxs - 0x%x\r\n", pt_pooled);
        }
        x_dbg_stmt("\r\n");
    }

    if (i4_argc == 1)
    {
        pt_using  = svctx_get_using_head();
        if (pt_using == NULL) {
            return CLIR_OK;
        }
        x_dbg_stmt("CLI command output control:\r\n");
        for (i4_idx = 0; pt_using != NULL; pt_using = svctx_next(pt_using)) {
            x_dbg_stmt("dump %d - %s\r\n", i4_idx, pt_using->s_name);
            i4_idx++;
        }
    } else if (i4_argc == 2 && i4_idx >= 0) {
        pt_using  = svctx_get_using_head();
        for (; i4_idx > 0 && pt_using != NULL; pt_using = svctx_next(pt_using)) {
            i4_idx--;
        }
        if (i4_idx == 0) {
            svctx_print_self(pt_using);
        } else {
            x_dbg_stmt("Invalid parameter.\r\n");
            return CLIR_INV_CMD_USAGE;
        }
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_dump_svc_req_data
 *
 * Description: This API dump service request's data
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_dump_svc_req_data (INT32 i4_argc, const CHAR** pps_argv)
{
    SVCTX_T* pt_using  = NULL;
    CHAR*    pc_end_ptr;
    INT32    i4_idx = -1;

    if (i4_argc == 1) {
        pt_using  = svctx_get_using_head();
    } else if (i4_argc == 2) {
        i4_idx = (INT32) x_strtoll(pps_argv[1], &pc_end_ptr, 0);
        if (pc_end_ptr != NULL && *pc_end_ptr != '\0')
        {
            x_dbg_stmt("Invalid parameter.\r\n");
            return CLIR_INV_CMD_USAGE;
        }
    }

    if (pt_using != NULL) {
        x_dbg_stmt("[using svctxs]\r\n");
        for (; pt_using != NULL; pt_using = svctx_next(pt_using)) {
            x_dbg_stmt("++++++++++++++++++++++++++++++++++++ %s running service request\r\n", pt_using->s_name);
            svctx_print_svc_req(pt_using->pt_running_svc_req);
            x_dbg_stmt("++++++++++++++++++++++++++++++++++++ %s pending service request\r\n", pt_using->s_name);
            svctx_print_svc_req(pt_using->pt_pending_svc_req);
        }
        x_dbg_stmt("\r\n");
    }

    if (i4_argc == 1)
    {
        pt_using  = svctx_get_using_head();
        if (pt_using == NULL) {
            return CLIR_OK;
        }
        x_dbg_stmt("CLI command output control:\r\n");
        for (i4_idx = 0; pt_using != NULL; pt_using = svctx_next(pt_using)) {
            x_dbg_stmt("dumpsvcreq %d - %s\r\n", i4_idx, pt_using->s_name);
            i4_idx++;
        }
    } else if (i4_argc == 2 && i4_idx >= 0) {
        pt_using  = svctx_get_using_head();
        for (; i4_idx > 0 && pt_using != NULL; pt_using = svctx_next(pt_using)) {
            i4_idx--;
        }
        if (i4_idx == 0) {
            x_dbg_stmt("++++++++++++++++++++++++++++++++++++ %s running service request\r\n", pt_using->s_name);
            svctx_print_svc_req(pt_using->pt_running_svc_req);
            x_dbg_stmt("++++++++++++++++++++++++++++++++++++ %s pending service request\r\n", pt_using->s_name);
            svctx_print_svc_req(pt_using->pt_pending_svc_req);
        } else {
            x_dbg_stmt("Invalid parameter.\r\n");
            return CLIR_INV_CMD_USAGE;
        }
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_dump_active_link_data
 *
 * Description: This API dump svctx manager active link's data
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_dump_active_link_data (INT32 i4_argc, const CHAR** pps_argv)
{
#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (i4_argc == 1) {
        svctx_mgr_print_active_link(ACTIVE_LINK_NOW);
        svctx_mgr_print_active_link(ACTIVE_LINK_NEXT);
    } else if (i4_argc == 2) {
        if (x_strcmp("now", pps_argv[1]) == 0) {
            svctx_mgr_print_active_link(ACTIVE_LINK_NOW);
        } else if (x_strcmp("next", pps_argv[1]) == 0) {
            svctx_mgr_print_active_link(ACTIVE_LINK_NEXT);
        } else {
            x_dbg_stmt("Invalid parameter.\r\n");
            return CLIR_INV_CMD_USAGE;
        }
    }
#endif
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_dump_strm_fmt_chker_data
 *
 * Description: This API dump Rating Chker's data.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_dump_strm_fmt_chker_data (INT32 i4_argc, const CHAR** pps_argv)
{
    STRM_FMT_CHKER_T* pt_using  = NULL;
    STRM_FMT_CHKER_T* pt_pooled = NULL;

    if (i4_argc == 1) {
        pt_using  = strm_fmt_chker_get_using_head();
        pt_pooled = strm_fmt_chker_get_pooled_head();
    } else if (i4_argc == 2) {
        if (x_strcmp("u", pps_argv[1]) == 0  || x_strcmp("using", pps_argv[1]) == 0) {
            pt_using  = strm_fmt_chker_get_using_head();
        } else if (x_strcmp("p", pps_argv[1]) == 0  || x_strcmp("pooled", pps_argv[1]) == 0) {
            pt_pooled = strm_fmt_chker_get_pooled_head();
        }
    }

    if (pt_using != NULL) {
        x_dbg_stmt("[using rating chkers]\r\n");
        for (; pt_using != NULL; pt_using = strm_fmt_chker_next(pt_using)) {
            strm_fmt_chker_print_self(pt_using);
        }
        x_dbg_stmt("\r\n");
    }

    if (pt_pooled != NULL) {
        x_dbg_stmt("[pooled rating chkers]\r\n");
        for (; pt_pooled != NULL; pt_pooled = strm_fmt_chker_next(pt_pooled)) {
            x_dbg_stmt("strm_fmt_chker - 0x%x\r\n", pt_pooled);
        }
        x_dbg_stmt("\r\n");
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_dump_player_data
 *
 * Description: This API dump Player's data.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_dump_player_data (INT32 i4_argc, const CHAR** pps_argv)
{
    PLAYER_T* pt_using  = NULL;
    PLAYER_T* pt_pooled = NULL;

    if (i4_argc == 1) {
        pt_using  = player_get_using_head();
        pt_pooled = player_get_pooled_head();
    } else if (i4_argc == 2) {
        if (x_strcmp("u", pps_argv[1]) == 0  || x_strcmp("using", pps_argv[1]) == 0) {
            pt_using  = player_get_using_head();
        } else if (x_strcmp("p", pps_argv[1]) == 0  || x_strcmp("pooled", pps_argv[1]) == 0) {
            pt_pooled = player_get_pooled_head();
        }
    }

    if (pt_using != NULL) {
        x_dbg_stmt("[using players]\r\n");
        for (; pt_using != NULL; pt_using = player_next(pt_using)) {
            player_print_self(pt_using);
        }
        x_dbg_stmt("\r\n");
    }

    if (pt_pooled != NULL) {
        x_dbg_stmt("[pooled players]\r\n");
        for (; pt_pooled != NULL; pt_pooled = player_next(pt_pooled)) {
            x_dbg_stmt("player - 0x%x\r\n", pt_pooled);
        }
        x_dbg_stmt("\r\n");
    }
    return CLIR_OK;
}
/*------------------------------------------------------------------------------
 * Name
 *      _svctx_cli_scdb_filter
 * Description
 * Input arguments
 *      pt_this
 * Output arguments
 *      None
 * Returns
 *      None
 *----------------------------------------------------------------------------*/
static BOOL _svctx_cli_scdb_filter(
                    UINT16                      ui2_num_recs,     /* in  */
                    UINT16                      ui2_idx,          /* in  */
                    const STREAM_COMP_ID_T*     pt_comp_id,       /* in  */
                    const SCDB_REC_T*           pt_scdb_rec,      /* in  */
                    VOID*                       pv_fltr_tag       /* in  */
                    )
{
    SCDB_REC_T* pt_using_scdb_rec = (SCDB_REC_T*)pv_fltr_tag;

    if (ui2_num_recs == ui2_idx) {
        return TRUE;
    }

    switch(pt_comp_id->e_type) {
    case ST_AUDIO:
        switch(pt_scdb_rec->e_rec_type) {
        case SCDB_REC_TYPE_AUDIO_MPEG:
            if (pt_using_scdb_rec != NULL
                    && pt_using_scdb_rec->u.t_audio_mpeg.ui2_pid == pt_scdb_rec->u.t_audio_mpeg.ui2_pid) {
                x_dbg_stmt("->");
            } else {
                x_dbg_stmt("  ");
            }
            x_dbg_stmt("Audio[%d]  -PID=%d, Fmt=%d, Lang=%s, def=%s, enc=%d, comp_tag=%d, pv_strm_tag=%d, type=%s, layer=%d, prf_lvl=%d, mix_type=%d, e_class=%d,pmx_index=%d\r\n",
                            ui2_idx,
                            pt_scdb_rec->u.t_audio_mpeg.ui2_pid,
                            pt_scdb_rec->u.t_audio_mpeg.e_aud_fmt,
                            pt_scdb_rec->u.t_audio_mpeg.s_lang,
                            (pt_scdb_rec->u.t_audio_mpeg.b_default == TRUE ? "true" : "false"),
                            pt_scdb_rec->u.t_audio_mpeg.e_enc,
                            pt_scdb_rec->u.t_audio_mpeg.ui1_component_tag,
                            (CHAR*)pt_comp_id->pv_stream_tag,
                            s_g_audio_types[pt_scdb_rec->u.t_audio_mpeg.e_type],
                            pt_scdb_rec->u.t_audio_mpeg.e_layer,
                            pt_scdb_rec->u.t_audio_mpeg.e_prof_lvl,
                            pt_scdb_rec->u.t_audio_mpeg.e_mix_type,
                            pt_scdb_rec->u.t_audio_mpeg.e_class,
                            pt_scdb_rec->u.t_audio_mpeg.ui2_pmt_index
                            );
            break;
        case SCDB_REC_TYPE_AUDIO_ANALOG:
            x_dbg_stmt("->");
            x_dbg_stmt("Audio[%d]  -Fmt=%d, def=%s, enc=%d, pv_strm_tag=%d\r\n",
                            ui2_idx,
                            pt_scdb_rec->u.t_audio_analog.e_aud_fmt,
                            (pt_scdb_rec->u.t_audio_analog.b_default == TRUE ? "true" : "false"),
                            pt_scdb_rec->u.t_audio_analog.e_enc,
                            (CHAR*)pt_comp_id->pv_stream_tag
                            );
            break;
        default:
            x_dbg_stmt("Unknown Audio e_rec_type(%d) Comp[%d]\r\n",
                        pt_scdb_rec->e_rec_type, ui2_idx);
            break;
        }
        break;
    case ST_VIDEO:
        switch(pt_scdb_rec->e_rec_type) {
        case SCDB_REC_TYPE_VIDEO_MPEG:
            if (pt_using_scdb_rec != NULL
                    && pt_using_scdb_rec->u.t_video_mpeg.ui2_pid == pt_scdb_rec->u.t_video_mpeg.ui2_pid) {
                x_dbg_stmt("->");
            } else {
                x_dbg_stmt("  ");
            }
             x_dbg_stmt("Video[%d]  -PID=%d, Fmt=%d, def=%s, comp_tag=%d, pv_strm_tag=%d, pmt_idx=%d\r\n",
                            ui2_idx,
                            pt_scdb_rec->u.t_video_mpeg.ui2_pid,
                            pt_scdb_rec->u.t_video_mpeg.e_vid_fmt,
                            (pt_scdb_rec->u.t_video_mpeg.b_default == TRUE ? "true" : "false"),
                            pt_scdb_rec->u.t_video_mpeg.ui1_component_tag,
                            (CHAR*)pt_comp_id->pv_stream_tag,
                            pt_scdb_rec->u.t_video_mpeg.ui2_pmt_index
                            );
            break;
        case SCDB_REC_TYPE_VIDEO_ANALOG:
            x_dbg_stmt("->");
            x_dbg_stmt("Video[%d]  -Fmt=%d, def=%s, color_sys=%d, pv_strm_tag=%d\r\n",
                            ui2_idx,
                            pt_scdb_rec->u.t_video_analog.e_vid_fmt,
                            (pt_scdb_rec->u.t_video_analog.b_default == TRUE ? "true" : "false"),
                            pt_scdb_rec->u.t_video_analog.e_color_sys,
                            (CHAR*)pt_comp_id->pv_stream_tag
                            );
            break;
        default:
            x_dbg_stmt("Unknown Video e_rec_type(%d) Comp[%d]\r\n",
                        pt_scdb_rec->e_rec_type, ui2_idx);
            break;
        }
        break;
    case ST_CLOSED_CAPTION:
        switch(pt_scdb_rec->u.t_atsc_cc.e_type) {
        case CC_ATSC_LINE_21:
            if (pt_using_scdb_rec != NULL
                    && pt_using_scdb_rec->u.t_atsc_cc.e_type == pt_scdb_rec->u.t_atsc_cc.e_type
                    && pt_using_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_line_21.ui1_cc_id == pt_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_line_21.ui1_cc_id) {
                x_dbg_stmt("->");
            } else {
                x_dbg_stmt("  ");
            }

            x_dbg_stmt("NTSC CC[%d]-Lang=%s, def=%s, pv_strm_tag=%d, easy_reader=%s, wide_aspect=%s, cc_id=%d, fake=%s\r\n",
                        ui2_idx,
                        pt_scdb_rec->u.t_atsc_cc.s_lang,
                        (pt_scdb_rec->u.t_atsc_cc.b_default == TRUE ? "true" : "false"),
                        (CHAR*)pt_comp_id->pv_stream_tag,
                        (pt_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_line_21.b_easy_reader == TRUE ? "true" : "false"),
                        (pt_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_line_21.b_wide_aspect == TRUE ? "true" : "false"),
                        pt_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_line_21.ui1_cc_id,
                        (pt_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_line_21.b_det_in_descr == TRUE ? "false" : "true")
                        );
            break;
        case CC_ATSC_DTV:
            if (pt_using_scdb_rec != NULL
                    && pt_using_scdb_rec->u.t_atsc_cc.e_type == pt_scdb_rec->u.t_atsc_cc.e_type
                    && pt_using_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_dtv.ui1_caption_svc_num == pt_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_dtv.ui1_caption_svc_num) {
                x_dbg_stmt("->");
            } else {
                x_dbg_stmt("  ");
            }
            x_dbg_stmt("DTVCC[%d]  -lang=%s, def=%s, pv_strm_tag=%d, easy_reader=%s, wide_aspect=%s, svc_#=%d\r\n",
                        ui2_idx,
                        pt_scdb_rec->u.t_atsc_cc.s_lang,
                        (pt_scdb_rec->u.t_atsc_cc.b_default == TRUE ? "true" : "false"),
                        (CHAR*)pt_comp_id->pv_stream_tag,
                        (pt_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_dtv.b_easy_reader == TRUE ? "true" : "false"),
                        (pt_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_dtv.b_wide_aspect == TRUE ? "true" : "false"),
                        pt_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_dtv.ui1_caption_svc_num
                        );
            break;
        default:
            x_dbg_stmt("Unknown CC type(%d) Comp[%d]\r\n",
                        pt_scdb_rec->u.t_atsc_cc.e_type, ui2_idx);
            break;
        }
        break;
    case ST_TELETEXT:
        if (pt_using_scdb_rec != NULL
                && pt_using_scdb_rec->u.t_dvb_tltxt.ui2_pid == pt_scdb_rec->u.t_dvb_tltxt.ui2_pid) {
            x_dbg_stmt("->");
        } else {
            x_dbg_stmt("  ");
        }
        x_dbg_stmt("TTX[%d]  -PID=%d, b_page_info=%s, e_type=%d, lang=%s, ui1_mag_num=%d, ui1_page_num=%d, def=%s\r\n",
                        ui2_idx,
                        pt_scdb_rec->u.t_dvb_tltxt.ui2_pid,
                        (pt_scdb_rec->u.t_dvb_tltxt.b_page_info == TRUE ? "true" : "false"),
                        pt_scdb_rec->u.t_dvb_tltxt.e_type,
                        pt_scdb_rec->u.t_dvb_tltxt.s_lang,
                        pt_scdb_rec->u.t_dvb_tltxt.ui1_mag_num,
                        pt_scdb_rec->u.t_dvb_tltxt.ui1_page_num,
                        (pt_scdb_rec->u.t_dvb_tltxt.b_default ==TRUE ? "true" : "false")
                        );
        break;
    case ST_SUBTITLE:
        if (pt_using_scdb_rec != NULL
                && pt_using_scdb_rec->u.t_dvb_sbtl.ui2_pid == pt_scdb_rec->u.t_dvb_sbtl.ui2_pid) {
            x_dbg_stmt("->");
        } else {
            x_dbg_stmt("  ");
        }
        x_dbg_stmt("Subtitle[%d]  -PID=%d, comp_pg_id=%d, anci_pg_id=%d, , lang=%s, sbtl_type=%d, pv_strm_tag=%d\r\n",
                        ui2_idx,
                        pt_scdb_rec->u.t_dvb_sbtl.ui2_pid,
                        pt_scdb_rec->u.t_dvb_sbtl.ui2_comp_pg_id,
                        pt_scdb_rec->u.t_dvb_sbtl.ui2_anci_pg_id,
                        pt_scdb_rec->u.t_dvb_sbtl.s_lang,
                        pt_scdb_rec->u.t_dvb_sbtl.e_sbtl_type,
                        (CHAR*)pt_comp_id->pv_stream_tag
                        );
        break;
    default:
        x_dbg_stmt("Unknown(type=%d) Comp[%d] - don't support\r\n", pt_comp_id->e_type, ui2_idx);
        break;
    }
    return FALSE;
}
#ifdef TIME_MEASUREMENT
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_set_tms_level
 *
 * Description: This API sets the time measurement level of Service Context.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_set_tms_level(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32  i4_ret;
    UINT16 ui2_tms_level;

    i4_ret = x_cli_parse_tms_level(i4_argc, pps_argv, &ui2_tms_level);
    if ( i4_ret == CLIR_OK ) {
        svctx_set_tms_level(ui2_tms_level);
    }

    x_cli_show_tms_level( svctx_get_tms_level() );

    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_get_tms_level
 *
 * Description: This API gets the current time measurement level of Service Context.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_get_tms_level(INT32 i4_argc, const CHAR** pps_argv)
{
    return x_cli_show_tms_level(svctx_get_tms_level());
}
#endif /* TIME_MEASUREMENT */
/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Name:    svctx_cli_attach_cmd_tbl
 * Description:
 * Input arguments:  -
 * Output arguments: -
 * Returns:
 *----------------------------------------------------------------------------*/
INT32 svctx_cli_attach_cmd_tbl(VOID)
{
    return (x_cli_attach_cmd_tbl(at_svctx_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_PIPE));
}
/*----------------------------------------------------------------------------
 * Name:    svctx_cli_enable_strm_select
 * Description:
 * Input arguments:  -
 * Output arguments: -
 * Returns:
 *----------------------------------------------------------------------------*/
BOOL svctx_cli_enable_strm_select(
                    STREAM_TYPE_T           e_type
                    )
{
    if ((ui4_g_css_strm_status & MAKE_BIT_MASK_32(e_type)) > 0) {
        return FALSE;
    } else {
        return TRUE;
    }
}
/*------------------------------------------------------------------------------
 * Name
 *      svctx_cli_dump_scdb_data
 * Description
 * Input arguments
 *      pt_this
 * Output arguments
 *      None
 * Returns
 *      None
 *----------------------------------------------------------------------------*/
INT32 svctx_cli_dump_scdb_data(INT32 i4_argc, const CHAR** pps_argv)
{
    static STREAM_TYPE_T e_strm_types[] = {ST_VIDEO, ST_AUDIO, ST_CLOSED_CAPTION, ST_SUBTITLE, ST_TELETEXT};
    static UINT32        ui4_types_size = sizeof(e_strm_types) / sizeof(STREAM_TYPE_T);
    HANDLE_T             h_svctx = NULL_HANDLE;
    UINT32               ui4_idx = 0;
    STREAM_COMP_ID_T     t_comp_id;
    UINT16               ui2_num_recs[] = {0, 0, 0, 0, 0};
    INT32                i4_ret;
    CHAR*                ps_svctx_name = "main_svctx";

    if (i4_argc > 1) {
        ps_svctx_name = (CHAR*)pps_argv[1];
    }

    i4_ret = x_svctx_open(ps_svctx_name, &h_svctx);
    if (i4_ret != SVCTXR_OK) {
        x_dbg_stmt("x_svctx_open(%s) failed, i4_ret = %d\r\n", i4_ret, ps_svctx_name);
        return CLIR_OK;
    }

    x_dbg_stmt("- SCDB records ----------------------------- (%s)\r\n", ps_svctx_name);

    for (ui4_idx = 0; ui4_idx < ui4_types_size; ui4_idx++) {
        SCDB_REC_T  t_scdb_rec;
        SCDB_REC_T* pt_scdb_rec;

        i4_ret = x_svctx_get_scdb_num_recs(
                    h_svctx,
                    e_strm_types[ui4_idx],
                    &ui2_num_recs[ui4_idx]
                    );

        i4_ret = x_svctx_get_using_scdb_rec(
                    h_svctx,
                    e_strm_types[ui4_idx],
                    &t_scdb_rec
                    );

        if (i4_ret == SVCTXR_OK) {
            pt_scdb_rec = &t_scdb_rec;
        } else {
            pt_scdb_rec = NULL;
        }

        x_svctx_get_stream_comp_id(
                    h_svctx,
                    e_strm_types[ui4_idx],
                    _svctx_cli_scdb_filter,
                    pt_scdb_rec,
                    &t_comp_id,
                    NULL
                    );
    }

    if ((ui2_num_recs[0] + ui2_num_recs[1] + ui2_num_recs[2]) > 0) {
        x_dbg_stmt("--------------------------------------------\r\n");
    }

    x_dbg_stmt("- Total %d video    streams\r\n", ui2_num_recs[0]);
    x_dbg_stmt("- Total %d audio    streams\r\n", ui2_num_recs[1]);
    x_dbg_stmt("- Total %d cc       streams\r\n", ui2_num_recs[2]);
    x_dbg_stmt("- Total %d subtitle streams\r\n", ui2_num_recs[3]);
    x_dbg_stmt("- Total %d teltex   streams\r\n", ui2_num_recs[4]);

    i4_ret = x_svctx_close(h_svctx);
    if (i4_ret != SVCTXR_OK) {
        x_dbg_stmt("x_svctx_close(main_svctx) failed, i4_ret = %d\r\n", i4_ret);
    }

    return CLIR_OK;
}
/*----------------------------------------------------------------------------
 * Name:    _atoi
 *
 * Description: Convert a number string to a number.
 *
 * Inputs:  ps     null ('\0') terminated character string.
 *
 *
 * Outputs: -
 *
 * Returns: sum of the number string.
 *
 ----------------------------------------------------------------------------*/
static INT32 _atoi( const CHAR* ps )
{
    INT32  i4_total;

    i4_total = 0;
    while ( *ps != 0 )
    {
        if ( *ps >= '0' && *ps <= '9' )
        {
            /* accumulate digit */
            i4_total = 10 * i4_total + (*ps - '0');
            ps++; /* get next char */
        }
        else /* not a digit char */
        {
            return (-1);
        }
    }

    return i4_total;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_cli_open_svctx
 * Description
 * Input arguments
 *      None
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static BOOL _svctx_cli_open_svctx(
                    CHAR*                       ps_name,
                    HANDLE_T*                   ph_svctx
                    )
{
    INT32 i4_ret;

    if (x_strlen(ps_name) == 0) {
        ps_name = "main_svctx";
    }

    if (x_handle_valid(*ph_svctx) == TRUE) {
        x_svctx_close(*ph_svctx);
    }

    i4_ret = x_svctx_open(ps_name, ph_svctx);
    if (i4_ret != SVCTXR_OK) {
        x_dbg_stmt("x_svctx_open(%s) failed, i4_ret = %d\r\n", ps_name, i4_ret);
        return FALSE;
    }

    return TRUE;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_dump_video_data
 *
 * Description: -
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_dump_video_data (INT32 i4_argc, const CHAR** pps_argv)
{
    INT16  i2_clock  = SM_VSH_VID_COLCK_UNKNOWN;
    UINT16 ui2_phase = SM_VSH_VID_PHASE_UNKNOWN;
    UINT16 ui2_h_pos = SM_VSH_VID_H_POS_UNKNOWN;
    UINT16 ui2_v_pos = SM_VSH_VID_V_POS_UNKNOWN;
    SIZE_T z_size    = 0;

    if (_svctx_cli_open_svctx(i4_argc > 1 ? (CHAR*)pps_argv[1] : NULL, &h_g_main_svctx) == FALSE) {
        x_dbg_stmt("create service contex failed!\r\n");
        return CLIR_OK;
    }

    /* clock */
    z_size = sizeof(i2_clock);
    x_svctx_get_stream_attr(
                    h_g_main_svctx,
                    ST_VIDEO,
                    SM_VSH_GET_TYPE_VID_CLOCK,
                    &i2_clock,
                    &z_size
                    );
    if (i2_clock == SM_VSH_VID_COLCK_UNKNOWN) {
        x_dbg_stmt("clock = unknow\r\n");
    } else {
        x_dbg_stmt("clock = %d\r\n", i2_clock);
    }

    /* phase */
    z_size = sizeof(ui2_phase);
    x_svctx_get_stream_attr(
                    h_g_main_svctx,
                    ST_VIDEO,
                    SM_VSH_GET_TYPE_VID_PHASE,
                    &ui2_phase,
                    &z_size
                    );
    if (ui2_phase == SM_VSH_VID_PHASE_UNKNOWN) {
        x_dbg_stmt("phase = unknow\r\n");
    } else {
        x_dbg_stmt("phase = %d\r\n", ui2_phase);
    }

    /* h pos */
    z_size = sizeof(ui2_h_pos);
    x_svctx_get_stream_attr(
                    h_g_main_svctx,
                    ST_VIDEO,
                    SM_VSH_GET_TYPE_VID_H_POS,
                    &ui2_h_pos,
                    &z_size
                    );
    if (ui2_phase == SM_VSH_VID_H_POS_UNKNOWN) {
        x_dbg_stmt("h_pos = unknow\r\n");
    } else {
        x_dbg_stmt("h_pos = %d\r\n", ui2_h_pos);
    }

    z_size = sizeof(ui2_v_pos);
    x_svctx_get_stream_attr(
                    h_g_main_svctx,
                    ST_VIDEO,
                    SM_VSH_GET_TYPE_VID_V_POS,
                    &ui2_v_pos,
                    &z_size
                    );
    if (ui2_phase == SM_VSH_VID_V_POS_UNKNOWN) {
        x_dbg_stmt("v_pos = unknow\r\n");
    } else {
        x_dbg_stmt("v_pos = %d\r\n", ui2_v_pos);
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_set_clock
 *
 * Description: -
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_set_clock (INT32 i4_argc, const CHAR** pps_argv)
{
    INT16 i2_clock;
    INT32 i4_ret;

    if (i4_argc == 1) {
        x_dbg_stmt("Syntax Error!\r\n");
        return CLIR_OK;
    }

    if (_svctx_cli_open_svctx(i4_argc > 2 ? (CHAR*)pps_argv[2] : NULL, &h_g_main_svctx) == FALSE) {
        x_dbg_stmt("create service contex failed!\r\n");
        return CLIR_OK;
    }

    i2_clock = (INT16)_atoi(pps_argv[1]);

    x_dbg_stmt("set clock to %d\r\n", i2_clock);

    i4_ret = x_svctx_set_stream_attr(
                    h_g_main_svctx,
                    ST_VIDEO,
                    SM_VSH_SET_TYPE_VID_CLOCK,
                    (VOID*)(UINT32)i2_clock,
                    sizeof(INT16)
                    );
    if (i4_ret != SVCTXR_OK) {
        x_dbg_stmt("x_svctx_set_stream_attr(SM_VSH_SET_TYPE_VID_CLOCK) failed, i4_ret=%d\r\n", i4_ret);
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_set_phase
 *
 * Description: -
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_set_phase (INT32 i4_argc, const CHAR** pps_argv)
{
    UINT16 ui2_phase;
    INT32  i4_ret;

    if (i4_argc == 1) {
        x_dbg_stmt("Syntax Error!\r\n");
        return CLIR_OK;
    }

    if (_svctx_cli_open_svctx(i4_argc > 1 ? (CHAR*)pps_argv[2] : NULL, &h_g_main_svctx) == FALSE) {
        x_dbg_stmt("create service contex failed!\r\n");
        return CLIR_OK;
    }

    ui2_phase = (UINT16)_atoi(pps_argv[1]);

    x_dbg_stmt("set phase to %d\r\n", ui2_phase);

    i4_ret = x_svctx_set_stream_attr(
                    h_g_main_svctx,
                    ST_VIDEO,
                    SM_VSH_SET_TYPE_VID_PHASE,
                    (VOID*)(UINT32)ui2_phase,
                    sizeof(UINT16)
                    );
    if (i4_ret != SVCTXR_OK) {
        x_dbg_stmt("x_svctx_set_stream_attr(SM_VSH_SET_TYPE_VID_PHASE) failed, i4_ret=%d\r\n", i4_ret);
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_set_h_pos
 *
 * Description: -
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_set_h_pos (INT32 i4_argc, const CHAR** pps_argv)
{
    UINT16 ui2_h_pos;
    INT32  i4_ret;

    if (i4_argc == 1) {
        x_dbg_stmt("Syntax Error!\r\n");
        return CLIR_OK;
    }

    if (_svctx_cli_open_svctx(i4_argc > 1 ? (CHAR*)pps_argv[2] : NULL, &h_g_main_svctx) == FALSE) {
        x_dbg_stmt("create service contex failed!\r\n");
        return CLIR_OK;
    }

    ui2_h_pos = (UINT16)_atoi(pps_argv[1]);

    x_dbg_stmt("set h_pos to %d\r\n", ui2_h_pos);

    i4_ret = x_svctx_set_stream_attr(
                    h_g_main_svctx,
                    ST_VIDEO,
                    SM_VSH_SET_TYPE_VID_H_POS,
                    (VOID*)(UINT32)ui2_h_pos,
                    sizeof(UINT16)
                    );
    if (i4_ret != SVCTXR_OK) {
        x_dbg_stmt("x_svctx_set_stream_attr(SM_VSH_SET_TYPE_VID_H_POS) failed, i4_ret=%d\r\n", i4_ret);
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_set_v_pos
 *
 * Description: -
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_set_v_pos (INT32 i4_argc, const CHAR** pps_argv)
{
    UINT16 ui2_v_pos;
    INT32  i4_ret;

    if (i4_argc == 1) {
        x_dbg_stmt("Syntax Error!\r\n");
        return CLIR_OK;
    }

    if (_svctx_cli_open_svctx(i4_argc > 1 ? (CHAR*)pps_argv[2] : NULL, &h_g_main_svctx) == FALSE) {
        x_dbg_stmt("create service contex failed!\r\n");
        return CLIR_OK;
    }

    ui2_v_pos = (UINT16)_atoi(pps_argv[1]);

    x_dbg_stmt("set v_pos to %d\r\n", ui2_v_pos);

    i4_ret = x_svctx_set_stream_attr(
                    h_g_main_svctx,
                    ST_VIDEO,
                    SM_VSH_SET_TYPE_VID_V_POS,
                    (VOID*)(UINT32)ui2_v_pos,
                    sizeof(UINT16)
                    );
    if (i4_ret != SVCTXR_OK) {
        x_dbg_stmt("x_svctx_set_stream_attr(SM_VSH_SET_TYPE_VID_V_POS) failed, i4_ret=%d\r\n", i4_ret);
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_auto_clk_phs_pos
 *
 * Description: -
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_auto_clk_phs_pos (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;

    if (_svctx_cli_open_svctx(i4_argc > 0 ? (CHAR*)pps_argv[1] : NULL, &h_g_main_svctx) == FALSE) {
        x_dbg_stmt("create service contex failed!\r\n");
        return CLIR_OK;
    }

    x_dbg_stmt("auto clock/phase/position\r\n");

    i4_ret = x_svctx_set_stream_attr(
                    h_g_main_svctx,
                    ST_VIDEO,
                    SM_VSH_SET_TYPE_AUTO_CLK_PHS_POS,
                    NULL,
                    0
                    );
    if (i4_ret != SVCTXR_OK) {
        x_dbg_stmt("x_svctx_set_stream_attr(SM_VSH_SET_TYPE_AUTO_CLK_PHS_POS) failed, i4_ret=%d\r\n", i4_ret);
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_auto_color
 *
 * Description: -
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_auto_color (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;

    if (_svctx_cli_open_svctx(i4_argc > 0 ? (CHAR*)pps_argv[1] : NULL, &h_g_main_svctx) == FALSE) {
        x_dbg_stmt("create service contex failed!\r\n");
        return CLIR_OK;
    }

    x_dbg_stmt("auto color\r\n");

    i4_ret = x_svctx_set_stream_attr(
                    h_g_main_svctx,
                    ST_VIDEO,
                    SM_VSH_SET_TYPE_AUTO_COLOR,
                    NULL,
                    0
                    );
    if (i4_ret != SVCTXR_OK) {
        x_dbg_stmt("x_svctx_set_stream_attr(SM_VSH_SET_TYPE_AUTO_COLOR) failed, i4_ret=%d\r\n", i4_ret);
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_get_signal_lvl
 *
 * Description: -
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_get_signal_lvl (INT32 i4_argc, const CHAR** pps_argv)
{
    INT8  ui1_signal_level;
    INT32 i4_ret;

    if (_svctx_cli_open_svctx(i4_argc > 0 ? (CHAR*)pps_argv[1] : NULL, &h_g_main_svctx) == FALSE) {
        x_dbg_stmt("create service contex failed!\r\n");
        return CLIR_OK;
    }

    i4_ret = x_svctx_get_conn_attr(
                    h_g_main_svctx,
                    ST_VIDEO,
                    BRDCST_CTRL_GET_SIGNAL_LEVEL,
                    &ui1_signal_level
                    );
    x_dbg_stmt("x_svctx_get_conn_attr(BRDCST_CTRL_GET_SIGNAL_LEVEL) i4_ret=%d", i4_ret);
    if (i4_ret == SVCTXR_OK) {
        x_dbg_stmt(", ui1_signal_level=%d", ui1_signal_level);
    }
    x_dbg_stmt("\r\n");
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_set_scart_bypass
 *
 * Description: -
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_set_scart_bypass (INT32 i4_argc, const CHAR** pps_argv)
{
    ANALOG_BRDCST_CTRL_SCART_BYPASS_T t_bypass;
    INT32 i4_ret;

    if (_svctx_cli_open_svctx(i4_argc > 0 ? (CHAR*)pps_argv[1] : NULL, &h_g_main_svctx) == FALSE) {
        x_dbg_stmt("create service contex failed!\r\n");
        return CLIR_OK;
    }

    t_bypass.b_bypass = TRUE;

    i4_ret = x_svctx_set_conn_attr(
                    h_g_main_svctx,
                    ST_VIDEO,
                    ANALOG_BRDCST_CTRL_SET_SCART_BYPASS,
                    &t_bypass
                    );
    x_dbg_stmt("x_svctx_set_conn_attr(ANALOG_BRDCST_CTRL_SET_SCART_BYPASS) i4_ret=%d\r\n", i4_ret);
    return CLIR_OK;
}

#if 1
static VOID _svctx_cli_print_scdb_rec(SCDB_REC_T* pt_scdb_rec)
{
    if (!pt_scdb_rec) {
        x_dbg_stmt("\r\n%s()NULL_PTR!!!\r\n", __func__);
    }

    x_dbg_stmt(" -PID=%d, Fmt=%d, Lang=%s, def=%s, enc=%d, comp_tag=%d,  type=%s, layer=%d, prf_lvl=%d\r\n",
                    pt_scdb_rec->u.t_audio_mpeg.ui2_pid,
                    pt_scdb_rec->u.t_audio_mpeg.e_aud_fmt,
                    pt_scdb_rec->u.t_audio_mpeg.s_lang,
                    (pt_scdb_rec->u.t_audio_mpeg.b_default == TRUE ? "true" : "false"),
                    pt_scdb_rec->u.t_audio_mpeg.e_enc,
                    pt_scdb_rec->u.t_audio_mpeg.ui1_component_tag,
                    s_g_audio_types[pt_scdb_rec->u.t_audio_mpeg.e_type],
                    pt_scdb_rec->u.t_audio_mpeg.e_layer,
                    pt_scdb_rec->u.t_audio_mpeg.e_prof_lvl
                    );
    return;
}

static INT32 _svctx_cli_select_ad(INT32 i4_argc, const CHAR** pps_argv)
{
    UINT16          ui2_ad_num      = 0;
    SCDB_REC_T*     pt_scdb_recs    = NULL;
    SCDB_REC_T      t_scdb_rec      = {0};
    SIZE_T          z_get_info_size = 0;
    UINT32          ui4_ret         = CLIR_OK;

    CHAR*           pc_end_ptr      = NULL;
    INT32           i4_idx          = -1;

    if (_svctx_cli_open_svctx("main_svctx", &h_g_main_svctx) == FALSE) {
        x_dbg_stmt("create service contex failed!\r\n");
        return CLIR_OK;
    }

    z_get_info_size = sizeof(ui2_ad_num);
    x_svctx_get(h_g_main_svctx,
                SVCTX_COMMON_GET_AD_RECS_NUM,
                &ui2_ad_num,
                &z_get_info_size);
    x_dbg_stmt("\r\nAD recs number = %d\r\n", ui2_ad_num);


    if (0 < ui2_ad_num){
        z_get_info_size = sizeof(SCDB_REC_T)*ui2_ad_num;
        pt_scdb_recs = (SCDB_REC_T*)x_mem_alloc(z_get_info_size);
        if (!pt_scdb_recs) {
            x_dbg_stmt("alloc mem failure!\r\n");
            return CLIR_OK;
        }
    }

    do {
        ui4_ret = x_svctx_get(h_g_main_svctx,
                    SVCTX_COMMON_GET_AD_RECS,
                    (VOID*)pt_scdb_recs,
                    &z_get_info_size);
        if (SVCTXR_OK != ui4_ret) {
            x_dbg_stmt("\r\n=====Get ad recs failure! Return value = 0x%x = %d\r\n", ui4_ret, ui4_ret);
            ui4_ret = CLIR_OK;
            break;
        }
        
        x_dbg_stmt("\r\n====Available AD record(s):\r\n");
        for (i4_idx = 0; i4_idx < ui2_ad_num; i4_idx ++)
        {
            _svctx_cli_print_scdb_rec(pt_scdb_recs + i4_idx);
        }

        if (i4_argc < 2){
            ui4_ret = CLIR_OK;
            break;
        }

        i4_idx = (INT32) x_strtoll(pps_argv[1], &pc_end_ptr, 0);
        if (pc_end_ptr != NULL && *pc_end_ptr != '\0')
        {
            x_dbg_stmt("Invalid parameter.\r\n");
            ui4_ret =  CLIR_INV_CMD_USAGE;
            break;
        }

        if ((UINT16)i4_idx > ui2_ad_num) {
            x_dbg_stmt("Invalid parameter.\r\n");
            ui4_ret =  CLIR_INV_CMD_USAGE;
            break;
        }

        if (!pt_scdb_recs) {
            break;
        }

        x_dbg_stmt("\r\nSet ad now...\r\n");
        ui4_ret = x_svctx_set_ad_state_ex(
                        h_g_main_svctx,
                        pt_scdb_recs + (i4_idx - 1));
        if (SVCTXR_OK != ui4_ret) {
            x_dbg_stmt("\r\n=====Set ad failure! Return value = 0x%x = %d\r\n", ui4_ret, ui4_ret);
            ui4_ret = CLIR_OK;
            break;
        }


        z_get_info_size = sizeof(SCDB_REC_T);
        ui4_ret = x_svctx_get(
                        h_g_main_svctx,
                        SVCTX_COMMON_GET_AD_USING_REC,
                        (VOID*)&t_scdb_rec,
                        &z_get_info_size);
        if (SVCTXR_OK != ui4_ret) {
            x_dbg_stmt("\r\n======Get using ad rec failure! Return value = 0x%x = %d\r\n", ui4_ret, ui4_ret);
            ui4_ret = CLIR_OK;
            break;
        }
        x_dbg_stmt("\r\n====Current using AD record:\r\n");
        _svctx_cli_print_scdb_rec(&t_scdb_rec);

    }while(0);
    
    if (!pt_scdb_recs) {
        x_mem_free(pt_scdb_recs);
    }
    

    return ui4_ret;
}
#endif


/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_open_scdb
 *
 * Description: -
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_open_scdb (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32    i4_ret;
    HANDLE_T h_scdb;

    if (_svctx_cli_open_svctx(i4_argc > 0 ? (CHAR*)pps_argv[1] : NULL, &h_g_main_svctx) == FALSE) {
        x_dbg_stmt("create service contex failed!\r\n");
        return CLIR_OK;
    }

    i4_ret = x_svctx_open_scdb(
                    h_g_main_svctx,
                    ST_VIDEO,
                    &h_scdb,
                    NULL,
                    NULL
                    );
    x_dbg_stmt("x_svctx_open_scdb() i4_ret=%d\r\n", i4_ret);
    if (i4_ret == SVCTXR_OK) {
        i4_ret = x_svctx_close_scdb(h_scdb);
        x_dbg_stmt("x_svctx_close_scdb() i4_ret=%d\r\n", i4_ret);
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_select_mpeg_stream_nfy
 *
 * Description:
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _svctx_cli_select_mpeg_stream_nfy(
                    HANDLE_T                    h_svctx,
                    SVCTX_COND_T                e_nfy_cond,
                    SVCTX_NTFY_CODE_T           e_code,
                    STREAM_TYPE_T               e_stream_type,
                    VOID*                       pv_nfy_tag
                    )
{
    x_dbg_stmt("<%s state=\"%s\" stream_type=\"%s\" \tcode=\"SVCTX_NTFY_CODE_%s\"/>\r\n",
                    __func__,
                    s_g_state_strings[e_nfy_cond],
                    s_g_strm_types[e_stream_type],
                    s_g_svctx_ntfy_codes[e_code]
                    );
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_select_mpeg_stream
 *
 * Description: usage: sstrm a(audio)|v(video)|s(subtitle) pid [svctx name]
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_select_mpeg_stream (INT32 i4_argc, const CHAR** pps_argv)
{
    STREAM_TYPE_T e_strm_type;
    INT32         i4_ret;

    if (i4_argc < 3) {
        x_dbg_stmt("Syntax Error!\r\n");
        return CLIR_OK;
    }

    if (_svctx_cli_open_svctx(i4_argc > 3 ? (CHAR*)pps_argv[3] : NULL, &h_g_main_svctx) == FALSE) {
        x_dbg_stmt("create service contex failed!\r\n");
        return CLIR_OK;
    }

    if (x_strcmp("a", pps_argv[1]) == 0) {
        e_strm_type = ST_AUDIO;
    } else if (x_strcmp("v", pps_argv[1]) == 0) {
        e_strm_type = ST_VIDEO;
    } else if (x_strcmp("s", pps_argv[1]) == 0) {
        e_strm_type = ST_SUBTITLE;
    } else {
        x_dbg_stmt("Syntax Error! Stream type not support \r\n");
        return CLIR_OK;
    }

    i4_ret = x_svctx_select_mpeg_stream_by_pid(
                    h_g_main_svctx,
                    e_strm_type,
                    (MPEG_2_PID_T)_atoi(pps_argv[2]),
                    _svctx_cli_select_mpeg_stream_nfy,
                    NULL
                    );
    if (i4_ret != SVCTXR_OK) {
        x_dbg_stmt("x_svctx_select_mpeg_stream_by_pid() failed, i4_ret=%d\r\n", i4_ret);
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_open_stream
 *
 * Description: -
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_open_stream (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32    i4_ret;
    HANDLE_T h_stream;

    if (_svctx_cli_open_svctx(i4_argc > 0 ? (CHAR*)pps_argv[1] : NULL, &h_g_main_svctx) == FALSE) {
        x_dbg_stmt("create service contex failed!\r\n");
        return CLIR_OK;
    }

    i4_ret = x_svctx_open_stream(
                    h_g_main_svctx,
                    ST_VIDEO,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    TRUE,
                    &h_stream
                    );
    x_dbg_stmt("x_svctx_open_stream() i4_ret=%d\r\n", i4_ret);
    if (i4_ret == SVCTXR_OK) {
        i4_ret = x_svctx_close_stream(h_stream);
        x_dbg_stmt("x_svctx_close_stream() i4_ret=%d\r\n", i4_ret);
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_stop_stream
 *
 * Description: usage: sstrm s(sync)|a(async) a(audio)|v(video) [svctx name]
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_stop_stream (INT32 i4_argc, const CHAR** pps_argv)
{
    STREAM_TYPE_T e_strm_type;

    if (i4_argc < 3) {
        x_dbg_stmt("Syntax Error!\r\n");
        return CLIR_OK;
    }

    if (_svctx_cli_open_svctx(i4_argc > 3 ? (CHAR*)pps_argv[3] : NULL, &h_g_main_svctx) == FALSE) {
        x_dbg_stmt("create service contex failed!\r\n");
        return CLIR_OK;
    }

    if (x_strcmp("a", pps_argv[2]) == 0) {
        e_strm_type = ST_AUDIO;
    } else if (x_strcmp("v", pps_argv[2]) == 0) {
        e_strm_type = ST_VIDEO;
    } else if (x_strcmp("s", pps_argv[2]) == 0) {
        e_strm_type = ST_SUBTITLE;
    } else if (x_strcmp("c", pps_argv[2]) == 0) {
        e_strm_type = ST_CLOSED_CAPTION;
    } else {
        x_dbg_stmt("Syntax Error! Stream type not support \r\n");
        return CLIR_OK;
    }

    if (x_strcmp("a", pps_argv[1]) == 0) {
        x_svctx_stop_stream(h_g_main_svctx, e_strm_type);
    } else if (x_strcmp("s", pps_argv[1]) == 0) {
        x_svctx_sync_stop_stream(h_g_main_svctx, e_strm_type, DEFAULT_STOP_STRM_TIMEOUT);
    } else {
        x_dbg_stmt("Syntax Error!\r\n");
    }

    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_vbi_nfy
 *
 * Description: -
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_vbi_nfy (
                    VOID*                           pv_nfy_tag,
                    SM_VSH_VBI_INFO_T*              pt_vbi
                    )
{
    x_dbg_stmt("%s(), pv_nfy_tag=%d\r\n", __func__, pv_nfy_tag);

    return 0;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_vbi_broker_nfy
 *
 * Description: -
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _svctx_cli_vbi_broker_nfy (
                    HANDLE_T                        h_vbi_broker,
                    SVCTX_SM_BROKER_COND_T          e_cond,
                    VOID*                           pv_tag,
                    UINT32                          ui4_data
                    )
{
    x_dbg_stmt("%s(), h_vbi_broker=%d, e_cond=%d, pv_tag=%d\r\n",
                    __func__, h_vbi_broker, e_cond, (UINT32)pv_tag);
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_tv_video_stream_broker
 *
 * Description: -
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_tv_video_stream_broker (INT32 i4_argc, const CHAR** pps_argv)
{
    SVCTX_SM_BROKER_CMD_T   t_cmds[5];
    SM_VSH_VBI_NFY_T        t_vbi_nfy;
    HANDLE_T                h_sm_broker;
    SVCTX_SM_BROKER_COND_T  e_cond;
    INT32                   i4_ret;

    if (i4_argc < 3) {
        return CLIR_OK;
    }

    t_cmds[0].e_code          = SVCTX_SM_BROKER_CMD_CODE_DEVICE_TYPE;
    t_cmds[0].u.e_device_type = DEV_TUNER;
    t_cmds[1].e_code          = SVCTX_SM_BROKER_CMD_CODE_SRC_NAME;
    t_cmds[1].u.ps_text       = SN_TUNER_GRP_0;
    t_cmds[2].e_code          = SVCTX_SM_BROKER_CMD_CODE_SVC_LST_ID;
    t_cmds[2].u.ui2_data      = (UINT16)_atoi(pps_argv[1]);
    t_cmds[3].e_code          = SVCTX_SM_BROKER_CMD_CODE_SVL_REC_ID;
    t_cmds[3].u.ui2_data      = (UINT16)_atoi(pps_argv[2]);
    t_cmds[4].e_code          = SVCTX_SM_BROKER_CMD_CODE_END;

    t_vbi_nfy.pf_vbi_nfy = _svctx_cli_vbi_nfy;
    t_vbi_nfy.pv_tag     = (VOID*)2000;

    i4_ret = x_svctx_vbi_broker_open (
                    t_cmds,
                    &t_vbi_nfy,
                    _svctx_cli_vbi_broker_nfy,
                    (VOID*)2000,
                    &h_sm_broker,
                    &e_cond
                    );

    if (i4_ret != SVCTXR_OK) {
        x_dbg_stmt("x_svctx_vbi_broker_open() failed, i4_ret=%d\r\n", i4_ret);
    } else {
        x_dbg_stmt("h_sm_broker=%d, e_cond=%d\r\n", h_sm_broker, e_cond);
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_inp_video_stream_broker
 *
 * Description: -
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_inp_video_stream_broker (INT32 i4_argc, const CHAR** pps_argv)
{
    SVCTX_SM_BROKER_CMD_T   t_cmds[3];
    SM_VSH_VBI_NFY_T        t_vbi_nfy;
    HANDLE_T                h_sm_broker;
    SVCTX_SM_BROKER_COND_T  e_cond;
    INT32                   i4_ret;

    if (i4_argc < 2) {
        return CLIR_OK;
    }

    t_cmds[0].e_code          = SVCTX_SM_BROKER_CMD_CODE_DEVICE_TYPE;
    t_cmds[0].u.e_device_type = DEV_AVC;
    t_cmds[1].e_code          = SVCTX_SM_BROKER_CMD_CODE_SRC_NAME;
    t_cmds[1].u.ps_text       = (CHAR*)pps_argv[1];
    t_cmds[2].e_code          = SVCTX_SM_BROKER_CMD_CODE_END;

    t_vbi_nfy.pf_vbi_nfy = _svctx_cli_vbi_nfy;
    t_vbi_nfy.pv_tag     = (VOID*)3000;

    i4_ret = x_svctx_vbi_broker_open (
                    t_cmds,
                    &t_vbi_nfy,
                    _svctx_cli_vbi_broker_nfy,
                    (VOID*)3000,
                    &h_sm_broker,
                    &e_cond
                    );

    if (i4_ret != SVCTXR_OK) {
        x_dbg_stmt("x_svctx_vbi_broker_open() failed, i4_ret=%d\r\n", i4_ret);
    } else {
        x_dbg_stmt("x_svctx_sm_broker_open() successfully\r\n");
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_dump_rating_chker_data
 *
 * Description: This API dump Rating Chker's data.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_dump_rating_chker_data (INT32 i4_argc, const CHAR** pps_argv)
{
    RATING_CHKER_T* pt_using  = NULL;
    RATING_CHKER_T* pt_pooled = NULL;

    if (i4_argc == 1) {
        pt_using  = rating_chker_get_using_head();
        pt_pooled = rating_chker_get_pooled_head();
    } else if (i4_argc == 2) {
        if (x_strcmp("u", pps_argv[1]) == 0  || x_strcmp("using", pps_argv[1]) == 0) {
            pt_using  = rating_chker_get_using_head();
        } else if (x_strcmp("p", pps_argv[1]) == 0  || x_strcmp("pooled", pps_argv[1]) == 0) {
            pt_pooled = rating_chker_get_pooled_head();
        }
    }

    if (pt_using != NULL) {
        x_dbg_stmt("[using rating chkers]\r\n");
        for (; pt_using != NULL; pt_using = rating_chker_next(pt_using)) {
            rating_chker_print_self(pt_using);
        }
        x_dbg_stmt("\r\n");
    }

    if (pt_pooled != NULL) {
        x_dbg_stmt("[pooled rating chkers]\r\n");
        for (; pt_pooled != NULL; pt_pooled = rating_chker_next(pt_pooled)) {
            x_dbg_stmt("rating_chker - 0x%x\r\n", pt_pooled);
        }
        x_dbg_stmt("\r\n");
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_dump_crnt_evn_data
 *
 * Description: This API dump Rating Chker's data.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_dump_crnt_evn_data (INT32 i4_argc, const CHAR** pps_argv)
{
    CRNT_EVENT_T* pt_using  = NULL;
    CRNT_EVENT_T* pt_pooled = NULL;

    if (i4_argc == 1) {
        pt_using  = crnt_evn_get_using_head();
        pt_pooled = crnt_evn_get_pooled_head();
    } else if (i4_argc == 2) {
        if (x_strcmp("u", pps_argv[1]) == 0  || x_strcmp("using", pps_argv[1]) == 0) {
            pt_using  = crnt_evn_get_using_head();
        } else if (x_strcmp("p", pps_argv[1]) == 0  || x_strcmp("pooled", pps_argv[1]) == 0) {
            pt_pooled = crnt_evn_get_pooled_head();
        }
    }

    if (pt_using != NULL) {
        x_dbg_stmt("[using crnt_evns]\r\n");
        for (; pt_using != NULL; pt_using = crnt_evn_next(pt_using)) {
            crnt_evn_print_self(pt_using);
        }
        x_dbg_stmt("\r\n");
    }

    if (pt_pooled != NULL) {
        x_dbg_stmt("[pooled crnt_evns]\r\n");
        for (; pt_pooled != NULL; pt_pooled = crnt_evn_next(pt_pooled)) {
            x_dbg_stmt("crnt_evn - 0x%x\r\n", pt_pooled);
        }
        x_dbg_stmt("\r\n");
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: svctx_cli_sync_stop_svctx
 *
 * Description: This API dump Player's data.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
INT32 svctx_cli_sync_stop_svctx (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;

    x_dbg_stmt("%s()\r\n", __func__);

    if (x_handle_valid(h_g_main_svctx) == FALSE) {
        i4_ret = x_svctx_open("main_svctx", &h_g_main_svctx);
        if (i4_ret != SVCTXR_OK) {
            x_dbg_stmt("x_svctx_open(main_svctx) failed, i4_ret = %d\r\n", i4_ret);
            return CLIR_OK;
        }
    }

    i4_ret = x_svctx_sync_stop_svc(h_g_main_svctx, 0);
    if (i4_ret != SVCTXR_OK) {
        x_dbg_stmt("x_svctx_sync_stop_svc(main_svctx) failed, i4_ret = %d\r\n", i4_ret);
    }

    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: svctx_cli_close_svctx
 *
 * Description: This API dump Player's data.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
INT32 svctx_cli_close_svctx (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;

    x_dbg_stmt("%s()\r\n", __func__);

    if (x_handle_valid(h_g_main_svctx) == TRUE) {
        i4_ret = x_svctx_close(h_g_main_svctx);
        if (i4_ret != SVCTXR_OK) {
            x_dbg_stmt("x_svctx_close(main_svctx) failed, i4_ret = %d\r\n", i4_ret);
        }
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_set_ntfy_trace
 *
 * Description: usage: nt y|n
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_set_ntfy_trace (INT32 i4_argc, const CHAR** pps_argv)
{
    if (i4_argc > 1) {
        if (x_strcmp("y", pps_argv[1]) == 0 || x_strcmp("Y", pps_argv[1]) == 0) {
            b_g_svc_nfy_trace = TRUE;
            x_strcpy(s_g_svc_nfy_trace_name, "all");
            if (i4_argc > 2)
            {
                if (x_strcmp("main", pps_argv[2]) == 0) {
                    x_strcpy(s_g_svc_nfy_trace_name, "main_svctx");
                }
                else if (x_strcmp("sub", pps_argv[2]) == 0) {
                    x_strcpy(s_g_svc_nfy_trace_name, "sub_svctx");
                }
                else if (x_strcmp("tv_bypass", pps_argv[2]) == 0) {
                    x_strcpy(s_g_svc_nfy_trace_name, "tv_bypass_svctx");
                }
                else if (x_strcmp("mn_bypass", pps_argv[2]) == 0) {
                    x_strcpy(s_g_svc_nfy_trace_name, "mn_bypass_svctx");
                }
            }
        } else if (x_strcmp("n", pps_argv[1]) == 0 || x_strcmp("N", pps_argv[1]) == 0) {
            b_g_svc_nfy_trace = FALSE;
        } else {
            x_dbg_stmt("svctx.nt [ctrl] [svc_name]\n\r");
            x_dbg_stmt("  [ctrl]: y or n\n\r");
            x_dbg_stmt("  [svc_name]: all, main, sub, tv_bypass, mn_bypass\n\r");
            x_dbg_stmt("              (only valid when [ctrl] is enabled)\n\r");
            return CLIR_OK;
        }
        x_dbg_stmt("Svctx notification trace is %s ", (b_g_svc_nfy_trace ? "enabled" : "disabled"));
        if (b_g_svc_nfy_trace)
        {
            x_dbg_stmt("for %s\n\r", s_g_svc_nfy_trace_name);
        }
        else
        {
            x_dbg_stmt("\n\r");
        }
    } else {
        x_dbg_stmt("svctx.nt [ctrl] [svc_name]\n\r");
        x_dbg_stmt("  [ctrl]: y or n\n\r");
        x_dbg_stmt("  [svc_name]: all, main, sub, tv_bypass, mn_bypass\n\r");
        x_dbg_stmt("              (only valid when [ctrl] is enabled)\n\r");
        return CLIR_OK;
    }

    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_set_func_trace
 *
 * Description: usage: nt y|n
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_set_func_trace (INT32 i4_argc, const CHAR** pps_argv)
{
    if (i4_argc > 1) {
        if (x_strcmp("y", pps_argv[1]) == 0 || x_strcmp("Y", pps_argv[1]) == 0) {
            b_g_svc_func_trace = TRUE;
        } else if (x_strcmp("n", pps_argv[1]) == 0 || x_strcmp("N", pps_argv[1]) == 0) {
            b_g_svc_func_trace = FALSE;
        } else {
            x_dbg_stmt("Syntax Error! must be y or n\r\n");
            return CLIR_OK;
        }
    }

    x_dbg_stmt("function call trace is %s\r\n", (b_g_svc_func_trace ? "enabled" : "disabled"));
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_set_player_ntfy_trace
 *
 * Description: usage: nt y|n
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_set_player_ntfy_trace (INT32 i4_argc, const CHAR** pps_argv)
{
    if (i4_argc > 1) {
        if (x_strcmp("y", pps_argv[1]) == 0 || x_strcmp("Y", pps_argv[1]) == 0) {
            b_g_svc_player_nfy_trace = TRUE;
            x_strcpy(s_g_svc_player_nfy_trace_name, "all");
            if (i4_argc > 2)
            {
                if (x_strcmp("main", pps_argv[2]) == 0) {
                    x_strcpy(s_g_svc_player_nfy_trace_name, "snk_MainDisp");
                }
                else if (x_strcmp("sub", pps_argv[2]) == 0) {
                    x_strcpy(s_g_svc_player_nfy_trace_name, "snk_SubDisp");
                }
                else if (x_strcmp("tv_bypass", pps_argv[2]) == 0) {
                    x_strcpy(s_g_svc_player_nfy_trace_name, "snk_TvByPass");
                }
                else if (x_strcmp("mn_bypass", pps_argv[2]) == 0) {
                    x_strcpy(s_g_svc_player_nfy_trace_name, "snk_MntrByPass");
                }
            }
        } else if (x_strcmp("n", pps_argv[1]) == 0 || x_strcmp("N", pps_argv[1]) == 0) {
            b_g_svc_player_nfy_trace = FALSE;
        } else {
            x_dbg_stmt("svctx.player.nt [ctrl] [svc_name]\n\r");
            x_dbg_stmt("  [ctrl]: y or n\n\r");
            x_dbg_stmt("  [svc_name]: all, main, sub, tv_bypass, mn_bypass\n\r");
            x_dbg_stmt("              (only valid when [ctrl] is enabled)\n\r");
            return CLIR_OK;
        }
        x_dbg_stmt("Svctx player notification trace is %s ", (b_g_svc_player_nfy_trace ? "enabled" : "disabled"));
        if (b_g_svc_player_nfy_trace)
        {
            x_dbg_stmt("for %s\n\r", s_g_svc_player_nfy_trace_name);
        }
        else
        {
            x_dbg_stmt("\n\r");
        }
    } else {
        x_dbg_stmt("svctx.nt [ctrl] [svc_name]\n\r");
        x_dbg_stmt("  [ctrl]: y or n\n\r");
        x_dbg_stmt("  [svc_name]: all, main, sub, tv_bypass, mn_bypass\n\r");
        x_dbg_stmt("              (only valid when [ctrl] is enabled)\n\r");
        return CLIR_OK;
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _svctx_cli_set_strm_msg_trace
 *
 * Description: usage: nt y|n
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _svctx_cli_set_strm_msg_trace (INT32 i4_argc, const CHAR** pps_argv)
{
    if (i4_argc > 1) {
        if (x_strcmp("y", pps_argv[1]) == 0 || x_strcmp("Y", pps_argv[1]) == 0) {
            b_g_strm_msg_trace = TRUE;
            x_strcpy(s_g_strm_msg_trace_name, "all");
            if (i4_argc > 2)
            {
                if (x_strcmp("main", pps_argv[2]) == 0) {
                    x_strcpy(s_g_strm_msg_trace_name, "main_svctx");
                }
                else if (x_strcmp("sub", pps_argv[2]) == 0) {
                    x_strcpy(s_g_strm_msg_trace_name, "sub_svctx");
                }
                else if (x_strcmp("tv_bypass", pps_argv[2]) == 0) {
                    x_strcpy(s_g_strm_msg_trace_name, "tv_bypass_svctx");
                }
                else if (x_strcmp("mn_bypass", pps_argv[2]) == 0) {
                    x_strcpy(s_g_strm_msg_trace_name, "mn_bypass_svctx");
                }
            }
        } else if (x_strcmp("n", pps_argv[1]) == 0 || x_strcmp("N", pps_argv[1]) == 0) {
            b_g_strm_msg_trace = FALSE;
        } else {
            x_dbg_stmt("svctx.smt [ctrl] [svc_name]\n\r");
            x_dbg_stmt("  [ctrl]: y or n\n\r");
            x_dbg_stmt("  [svc_name]: all, main, sub, tv_bypass, mn_bypass\n\r");
            x_dbg_stmt("              (only valid when [ctrl] is enabled)\n\r");
            return CLIR_OK;
        }
        x_dbg_stmt("Stream message notification trace is %s ", (b_g_strm_msg_trace ? "enabled" : "disabled"));
        if (b_g_strm_msg_trace)
        {
            x_dbg_stmt("for %s\n\r", s_g_strm_msg_trace_name);
        }
        else
        {
            x_dbg_stmt("\n\r");
        }
    } else {
        x_dbg_stmt("svctx.smt [ctrl] [svc_name]\n\r");
        x_dbg_stmt("  [ctrl]: y or n\n\r");
        x_dbg_stmt("  [svc_name]: all, main, sub, tv_bypass, mn_bypass\n\r");
        x_dbg_stmt("              (only valid when [ctrl] is enabled)\n\r");
        return CLIR_OK;
    }

    return CLIR_OK;
}

static INT32 _svctx_cli_video_region_trace (INT32 i4_argc, const CHAR** pps_argv)
{
    if (i4_argc > 1) {
        if (x_strcmp("y", pps_argv[1]) == 0 || x_strcmp("Y", pps_argv[1]) == 0) {
            b_g_video_region_trace = TRUE;
            x_strcpy(s_g_video_region_trace_name, "all");
            if (i4_argc > 2)
            {
                if (x_strcmp("main", pps_argv[2]) == 0) {
                    x_strcpy(s_g_video_region_trace_name, "main_svctx");
                }
                else if (x_strcmp("sub", pps_argv[2]) == 0) {
                    x_strcpy(s_g_video_region_trace_name, "sub_svctx");
                }
                else if (x_strcmp("tv_bypass", pps_argv[2]) == 0) {
                    x_strcpy(s_g_video_region_trace_name, "tv_bypass_svctx");
                }
                else if (x_strcmp("mn_bypass", pps_argv[2]) == 0) {
                    x_strcpy(s_g_video_region_trace_name, "mn_bypass_svctx");
                }
            }
        } else if (x_strcmp("n", pps_argv[1]) == 0 || x_strcmp("N", pps_argv[1]) == 0) {
            b_g_video_region_trace = FALSE;
        } else {
            x_dbg_stmt("svctx.vrt [ctrl] [svc_name]\n\r");
            x_dbg_stmt("  [ctrl]: y or n\n\r");
            x_dbg_stmt("  [svc_name]: all, main, sub, tv_bypass, mn_bypass\n\r");
            x_dbg_stmt("              (only valid when [ctrl] is enabled)\n\r");
            return CLIR_OK;
        }
        x_dbg_stmt("video region trace is %s ", (b_g_video_region_trace ? "enabled" : "disabled"));
        if (b_g_video_region_trace)
        {
            x_dbg_stmt("for %s\n\r", s_g_video_region_trace_name);
        }
        else
        {
            x_dbg_stmt("\n\r");
        }
    } else {
        x_dbg_stmt("svctx.vrt [ctrl] [svc_name]\n\r");
        x_dbg_stmt("  [ctrl]: y or n\n\r");
        x_dbg_stmt("  [svc_name]: all, main, sub, tv_bypass, mn_bypass\n\r");
        x_dbg_stmt("              (only valid when [ctrl] is enabled)\n\r");
        return CLIR_OK;
    }

    return CLIR_OK;
}

static INT32 _svctx_cli_test_video_display(INT32 i4_argc, const CHAR** pps_argv)
{
    SVCTX_T* pt_using  = NULL;
    INT32    i4_idx = 0;
    INT32    i4_ret;

    if (i4_argc == 1) {
        pt_using  = svctx_get_using_head();
        for (; pt_using != NULL; pt_using = svctx_next(pt_using)) {
            x_dbg_stmt("%d - %s\r\n", i4_idx++, pt_using->s_name);
        }
        
    } else if (i4_argc == 6) {
        INT32 i4_pos = _atoi(pps_argv[1]);
        
        pt_using  = svctx_get_using_head();
        for (; pt_using != NULL; pt_using = svctx_next(pt_using)) {
            if (i4_idx++ == i4_pos)
            {
                if (x_strcmp("d", pps_argv[2]) == 0) {
                    SCC_BG_COLOR_T t_bg;
                    t_bg.ui1_red    = _atoi(pps_argv[3]);
                    t_bg.ui1_green  = _atoi(pps_argv[4]);
                    t_bg.ui1_blue   = _atoi(pps_argv[5]);
                    i4_ret = x_scc_disp_set_bg(pt_using->h_display_scc_comp, &t_bg);
                    if (i4_ret != SMR_OK)
                    {
                        x_dbg_stmt("call x_scc_disp_set_bg() ret = %d\r\n", i4_ret);
                    }
                }
                else if (x_strcmp("v", pps_argv[2]) == 0) {
                    SCC_BG_COLOR_T t_bg;
                    t_bg.ui1_red    = _atoi(pps_argv[3]);
                    t_bg.ui1_green  = _atoi(pps_argv[4]);
                    t_bg.ui1_blue   = _atoi(pps_argv[5]);
                    i4_ret = x_scc_vid_set_bg(pt_using->h_video_scc_comp, &t_bg);
                    if (i4_ret != SMR_OK)
                    {
                        x_dbg_stmt("call x_scc_vid_set_bg() ret = %d\r\n", i4_ret);
                    }                    
                }
                break;
            }
        }
    }
    else
    {
        x_dbg_stmt("vd idx (v)edio/(d)isplay r g b\r\n");
    }

    return CLIR_OK;    
}

static INT32 _svctx_cli_stream_component_trace (INT32 i4_argc, const CHAR** pps_argv)
{
    do
    {
        if (i4_argc > 2) {
            if (x_strcmp("=", pps_argv[1]) == 0)
            {
                ui4_g_stream_component_idx_mask = 0;
            }
            else
            {
                if (i4_argc < 3)
                {
                    break;
                }
            
                if (x_strcmp("+", pps_argv[1]) == 0)
                {
                    ui4_g_stream_component_idx_mask |= MAKE_BIT_MASK_32(_atoi(pps_argv[2]));
                }
                else if (x_strcmp("-", pps_argv[1]) == 0)
                {
                    ui4_g_stream_component_idx_mask &= ~MAKE_BIT_MASK_32(_atoi(pps_argv[2]));
                }
                else
                {
                    break;
                }
            }
            
            x_dbg_stmt("stream component idx mask is 0x%x\r\n", ui4_g_stream_component_idx_mask);
            return CLIR_OK;
        }
    }while(0);

    x_dbg_stmt("svctx.sct [ctrl] [stream_component_idx]\n\r");
    x_dbg_stmt("  [ctrl]: + (add a component idx) / -(remove a component mask) / =(diabled trace, no third parameter)\r\n");
    x_dbg_stmt("  [stream_component_idx]: 0-audio, 1-video and so on.\r\n");

    return CLIR_OK;
}

static INT32 _svctx_cli_set_channel_change_trace(INT32 i4_argc, const CHAR** pps_argv)
{
       if (i4_argc > 1) {
        if (x_strcmp("y", pps_argv[1]) == 0 || x_strcmp("Y", pps_argv[1]) == 0) {
            b_g_log_channel_change_time = TRUE;
            x_strcpy(s_g_channel_change_trace_name, "all");
            if (i4_argc > 2)
            {
                if (x_strcmp("main", pps_argv[2]) == 0) {
                    x_strcpy(s_g_strm_msg_trace_name, "main_svctx");
                }
                else if (x_strcmp("sub", pps_argv[2]) == 0) {
                    x_strcpy(s_g_channel_change_trace_name, "sub_svctx");
                }
                else if (x_strcmp("tv_bypass", pps_argv[2]) == 0) {
                    x_strcpy(s_g_channel_change_trace_name, "tv_bypass_svctx");
                }
                else if (x_strcmp("mn_bypass", pps_argv[2]) == 0) {
                    x_strcpy(s_g_channel_change_trace_name, "mn_bypass_svctx");
                }
            }
        } else if (x_strcmp("n", pps_argv[1]) == 0 || x_strcmp("N", pps_argv[1]) == 0) {
            b_g_log_channel_change_time = FALSE;
        } else {
            x_dbg_stmt("svctx.cct [ctrl] [svc_name]\n\r");
            x_dbg_stmt("  [ctrl]: y or n\n\r");
            x_dbg_stmt("  [svc_name]: all, main, sub, tv_bypass, mn_bypass\n\r");
            x_dbg_stmt("              (only valid when [ctrl] is enabled)\n\r");
            return CLIR_OK;
        }
        x_dbg_stmt("Channel change time trace is %s ", (b_g_log_channel_change_time ? "enabled" : "disabled"));
        if (b_g_log_channel_change_time)
        {
            x_dbg_stmt("for %s\n\r", s_g_channel_change_trace_name);
        }
        else
        {
            x_dbg_stmt("\n\r");
        }
    } else {
        x_dbg_stmt("svctx.cct [ctrl] [svc_name]\n\r");
        x_dbg_stmt("  [ctrl]: y or n\n\r");
        x_dbg_stmt("  [svc_name]: all, main, sub, tv_bypass, mn_bypass\n\r");
        x_dbg_stmt("              (only valid when [ctrl] is enabled)\n\r");
        return CLIR_OK;
    }

    return CLIR_OK;
}

/*----------------------------------------------------------------------------
 * Name:    svctx_cli_print_nfy
 * Description:
 * Input arguments:  -
 * Output arguments: -
 * Returns:
 *----------------------------------------------------------------------------*/
VOID svctx_cli_print_nfy(
                    SVCTX_T*                        pt_svctx,
                    SVCTX_NTFY_CODE_T               e_code,
                    STREAM_TYPE_T                   e_strm_type
                    )
{
    BOOL    b_print_nfy = FALSE;

    if (b_g_svc_nfy_trace == TRUE) {
        if ((x_strcmp("all", s_g_svc_nfy_trace_name) == 0) ||
            (x_strcmp(pt_svctx->s_name, s_g_svc_nfy_trace_name) == 0)) {
            b_print_nfy = TRUE;
        }

        if (b_print_nfy)
        {
            if (e_strm_type == ST_UNKNOWN) {
                x_dbg_stmt("<NTFY svctx=\"%s\" \tstate=\"%s\" \tcode=\"SVCTX_NTFY_CODE_%s\" (ms=%d)>\r\n",
                        pt_svctx->s_name,
                        s_g_state_strings[pt_svctx->e_state],
                        s_g_svctx_ntfy_codes[e_code],
                        x_os_get_sys_tick()*x_os_get_sys_tick_period()
                        );
            } else {
                x_dbg_stmt("<NTFY svctx=\"%s\" \tstate=\"%s\" \tcode=\"(%s) SVCTX_NTFY_CODE_%s\" (ms=%d)>\r\n",
                        pt_svctx->s_name,
                        s_g_state_strings[pt_svctx->e_state],
                        s_g_strm_types[e_strm_type],
                        s_g_svctx_ntfy_codes[e_code],
                        x_os_get_sys_tick()*x_os_get_sys_tick_period()
                        );
            }
        }
    }
}
/*----------------------------------------------------------------------------
 * Name:    svctx_cli_print_func_call
 * Description:
 * Input arguments:  -
 * Output arguments: -
 * Returns:
 *----------------------------------------------------------------------------*/
VOID svctx_cli_print_func_call(
                    const CHAR*                     ps_func_name,
                    STREAM_TYPE_T                   e_strm_type
                    )
{
    if (b_g_svc_func_trace == TRUE) {
        if (e_strm_type == ST_UNKNOWN) {
            x_dbg_stmt("<FUNC \tms=\"%d\" name=\"%s\"/>\r\n",
                        x_os_get_sys_tick()*x_os_get_sys_tick_period(),
                        ps_func_name
                        );
        } else {
            x_dbg_stmt("<FUNC stream=\"%s\" \tms=\"%d\" name=\"%s\"/>\r\n",
                        s_g_strm_types[e_strm_type],
                        x_os_get_sys_tick()*x_os_get_sys_tick_period(),
                        ps_func_name
                        );
        }
    }
}
/*----------------------------------------------------------------------------
 * Name:    svctx_cli_print_player_nfy
 * Description:
 * Input arguments:  -
 * Output arguments: -
 * Returns:
 *----------------------------------------------------------------------------*/
VOID svctx_cli_print_player_nfy(
                    PLAYER_T*       pt_player,
                    SVCTX_MSG_T*    pt_msg
                    )
{
    BOOL    b_print_nfy = FALSE;

    if (b_g_svc_player_nfy_trace == TRUE) {
        if ((x_strcmp("all", s_g_svc_player_nfy_trace_name) == 0) ||
            (x_strcmp(pt_player->s_snk_grp_name, s_g_svc_player_nfy_trace_name) == 0)) {
            b_print_nfy = TRUE;
        }

        if (b_print_nfy)
        {
            switch(pt_msg->ui1_msg_id)
            {
                case _PLAYER_MSG_PIPE_NTFY:
                    x_dbg_stmt("<NTFY player=\"%s\" \t(PIPE) cond=\"%s\" \toper_type=\"%s\">\r\n",
                        pt_player->s_snk_grp_name,
                        s_g_cm_conds[pt_msg->u.t_pipe.e_cond],
                        s_g_src_oper_types[pt_player->e_src_oper_type]
                        );
                    break;
                case _PLAYER_MSG_CONN_NTFY:
                    x_dbg_stmt("<NTFY player=\"%s\" \t(CONN) cond=\"%s\" \toper_type=\"%s\" \tdata=%d>\r\n",
                        pt_player->s_snk_grp_name,
                        s_g_cm_conds[pt_msg->u.t_conn.e_cond],
                        s_g_src_oper_types[pt_player->e_src_oper_type],
                        pt_msg->u.t_conn.ui4_data
                        );
                    break;
                case _PLAYER_MSG_SCDB_NTFY:
                    x_dbg_stmt("<NTFY player=\"%s\" \t(SCDB) cond=\"%s\" \toper_type=\"%s\">\r\n",
                        pt_player->s_snk_grp_name,
                        s_g_scdb_conds[pt_msg->u.t_scdb.e_cond],
                        s_g_src_oper_types[pt_player->e_src_oper_type]
                        );
                    break;
                case _PLAYER_MSG_SESS_NTFY:
                    x_dbg_stmt("<NTFY player=\"%s\" \t(SESS) cond=\"%s\" \toper_type=\"%s\">\r\n",
                        pt_player->s_snk_grp_name,
                        s_g_sess_conds[pt_msg->u.t_sess.e_cond],
                        s_g_snk_oper_types[pt_player->e_snk_oper_type]
                        );
                    break;
                default:
                    return;
            }
        }
    }
}
/*----------------------------------------------------------------------------
 * Name:    svctx_cli_print_strm_msg
 * Description:
 * Input arguments:  -
 * Output arguments: -
 * Returns:
 *----------------------------------------------------------------------------*/
VOID svctx_cli_print_strm_msg(
                    SVCTX_T*                        pt_svctx,
                    SVCTX_MSG_T*                    pt_msg
                    )
{
    BOOL    b_print_nfy = FALSE;

    if (b_g_strm_msg_trace == TRUE) {
        if ((x_strcmp("all", s_g_strm_msg_trace_name) == 0) ||
            (x_strcmp(pt_svctx->s_name, s_g_strm_msg_trace_name) == 0)) {
            b_print_nfy = TRUE;
        }

        if (b_print_nfy)
        {
            SVCTX_STRM_MSG_T*   pt_strm_msg = &pt_msg->u.t_strm;
            x_dbg_stmt("<NTFY svctx=\"%s\" hdlr=\"%s\"\t(STRM) handle=0x%x\tcond=\"%s\" \tevent=0x%x>\r\n",
                pt_svctx->s_name,
                pt_svctx->pt_running_svc_req->pt_hdlr->s_name,
                pt_strm_msg->h_obj,
                s_g_strm_conds[pt_strm_msg->e_cond],
                pt_strm_msg->e_event
                );
        }
    }
}

VOID svctx_cli_print_video_region(
                    SVCTX_T*                        pt_svctx,
                    SCC_VID_MODE_T                  e_vid_mode,
                    const CHAR*                     s_file,
                    UINT32                          ui4_line
                    )
{
    BOOL    b_print_nfy = FALSE;

    if (b_g_video_region_trace == TRUE) {
        if ((x_strcmp("all", s_g_video_region_trace_name) == 0) ||
            (x_strcmp(pt_svctx->s_name, s_g_video_region_trace_name) == 0)) {
            b_print_nfy = TRUE;
        }

        if (b_print_nfy)
        {
            VSH_REGION_INFO_T* pt_vr = &(pt_svctx->t_disp_region);
            x_dbg_stmt("[SVCTX=%s][file=%s][line=%d][VIDEO REGION: x=%d,y=%d,w=%d,y=%d][VIDEO MODE=%d] - time=%d\r\n",
                pt_svctx->s_name,
                s_file,
                ui4_line,
                pt_vr->ui4_x,
                pt_vr->ui4_y,
                pt_vr->ui4_width,
                pt_vr->ui4_height,
                e_vid_mode,
                x_os_get_sys_tick()*x_os_get_sys_tick_period()
                );
        }
    }
}

VOID svctx_cli_print_stream_component_idx(
                    SVCTX_T*                        pt_svctx,
                    UINT32                          ui4_type,
                    UINT32                          ui4_stream_type,
                    UINT32                          ui4_stream_handle,
                    UINT32                          ui4_event,
                    UINT32                          ui4_cond,
                    const CHAR*                     s_file,
                    UINT32                          ui4_line
                    )
{
    if ((ui4_g_stream_component_idx_mask & MAKE_BIT_MASK_32(ui4_stream_type)) > 0)
    {
        if (ui4_type == 0) /* open */
        {
            x_dbg_stmt("[%s][%s][%d] open(idx=%d, handle=0x%x), time=%d\r\n", pt_svctx->s_name, s_file, ui4_line, ui4_stream_type, ui4_stream_handle, x_os_get_sys_tick()*x_os_get_sys_tick_period());
        }
        else if (ui4_type == 1) /* close */
        {
            x_dbg_stmt("[%s][%s][%d] close(idx=%d, handle=0x%x), time=%d\r\n", pt_svctx->s_name, s_file, ui4_line, ui4_stream_type, ui4_stream_handle, x_os_get_sys_tick()*x_os_get_sys_tick_period());
        }
        else /* notify */
        {
            x_dbg_stmt("[%s][%s][%d] sm(idx=%d, handle=0x%x), notify(0x%x) cond(0x%x), time=%d\r\n", pt_svctx->s_name, s_file, ui4_line, ui4_stream_type, ui4_stream_handle, ui4_event, ui4_cond, x_os_get_sys_tick()*x_os_get_sys_tick_period());
        }
    }
}

static INT8 _svctx_cli_get_svctx_idx(CHAR* ps_name)
{

  INT8  i1_idx = 0;
 /*  4: sizeof (s_g_svctx_name) / sizeof (CHAR *); */

  for(i1_idx = 0; i1_idx < 4; i1_idx ++){
       if(x_strcmp(s_g_svctx_name[i1_idx], ps_name) == 0){
           return i1_idx;  
       }
  }
  
   x_dbg_stmt("[SVCTX_CLI] Not Get SVCTX=%s, at Line = %d\n", ps_name, __LINE__);
   return -1;
}
VOID svctx_cli_print_channel_change_log(
                    SVCTX_T*                        pt_svctx,
                    LOG_CH_XNG_STAGE_T              e_ch_xng_stage
                    )
{
    BOOL    b_print_nfy = FALSE;

    if(pt_svctx == NULL || e_ch_xng_stage > ui1_ch_xng_stage_num){
        x_dbg_stmt("<SVCTX_CLI> Invaild Parameter\n");
        
        return;
     }
    if (b_g_log_channel_change_time == TRUE) {
        if ((x_strcmp("all", s_g_channel_change_trace_name) == 0) ||
            (x_strcmp(pt_svctx->s_name, s_g_channel_change_trace_name) == 0)) {
            b_print_nfy = TRUE;
        }


        if (b_print_nfy)
        {
          UINT32 u4_time = x_os_get_sys_tick()*x_os_get_sys_tick_period();
          
          INT8 i1_idx = _svctx_cli_get_svctx_idx(pt_svctx->s_name);
          if(i1_idx < 0){
              x_dbg_stmt("<SVCTX_CLI> Invaild Parameter\n");
              return;
          }

          g_ui4_ch_xng_stages_Log[i1_idx * ui1_ch_xng_stage_num + e_ch_xng_stage] = u4_time;
            
          x_dbg_stmt("<####SVCTX_CH_XNG_LOG:svctx=%s stage =%s  ms = %d>\r\n",
                pt_svctx->s_name,
                s_g_log_channel_change[e_ch_xng_stage],
                u4_time          
                );
          if(e_ch_xng_stage == CH_XNG_STAGE_SVCTX_SET_UNMUTE ||
             e_ch_xng_stage == CH_XNG_STAGE_AP_SET_UNMUTE){
             x_dbg_stmt("<####SVCTX_CH_XNG_LOG: e_default_vid_mode = %d,"
                        "b_enable_video_mute_supervisor = %d,"
                        "b_able_set_pmx_bg = %d,"
                        "t_video_mute_b_enable = %d\n>",
                  pt_svctx->e_default_vid_mode,
                  pt_svctx->b_enable_video_mute_supervisor,
                  pt_svctx->b_able_set_pmx_bg,
                  pt_svctx->t_video_mute.b_enable);
             if(pt_svctx->e_default_vid_mode == 1){
                 g_ui4_ch_xng_stages_Log[i1_idx * ui1_ch_xng_stage_num + e_ch_xng_stage] = 0;
             }
          }
          else if (e_ch_xng_stage == CH_XNG_STAGE_RECEIVED_RESOLUTION_CHG){
              x_dbg_stmt("<####SVCTX_CH_XNG_LOG: timing_type = %d\n", pt_svctx->t_video_info.e_timing_type);
          }
            
             
        }
    }
}

VOID svctx_cli_clean_channel_change_log(
                    VOID
                    )
{
    if (b_g_log_channel_change_time == TRUE){
       x_memset(g_ui4_ch_xng_stages_Log, 0, sizeof(g_ui4_ch_xng_stages_Log)/sizeof(UINT32));
    }

    return;
        
}

static INT32 _svctx_cli_channel_change_time_dump(INT32 i4_argc, const CHAR** pps_argv)
{
    if (i4_argc > 1) {
        INT8   i1_idx = -1;
        UINT32 ui4_times = 0;
        UINT32 ui4_total_time = 0;
        UINT8  ui1_idx_offset = i1_idx * ui1_ch_xng_stage_num;
        
         if(x_strcmp("main", pps_argv[1]) == 0) {
             i1_idx = _svctx_cli_get_svctx_idx("main_svctx");
         }
         else if(x_strcmp("sub", pps_argv[1]) == 0) {
             i1_idx = _svctx_cli_get_svctx_idx("sub_svctx");
         }
         else if (x_strcmp("tv_bypass", pps_argv[1]) == 0) {
             i1_idx = _svctx_cli_get_svctx_idx("tv_bypass_svctx");
         }
         else if (x_strcmp("mn_bypass", pps_argv[1]) == 0) {
             i1_idx = _svctx_cli_get_svctx_idx("mn_bypass_svctx");
         }

        if(i1_idx < 0){
             x_dbg_stmt("<SVCTX_CLI> Invaild Parameter\n");
             return CLIR_INV_ARG;
        } 

        ui1_idx_offset = i1_idx * ui1_ch_xng_stage_num;
        if(g_ui4_ch_xng_stages_Log[ui1_idx_offset + CH_XNG_STAGE_AP_SET_UNMUTE] <
           g_ui4_ch_xng_stages_Log[ui1_idx_offset + CH_XNG_STAGE_SVCTX_SET_UNMUTE]){           

            x_dbg_stmt("AP unmute First!!!!, Total time is AP unmute - start select svc\n");
            ui4_total_time = g_ui4_ch_xng_stages_Log[ui1_idx_offset + CH_XNG_STAGE_AP_SET_UNMUTE] - 
                            g_ui4_ch_xng_stages_Log[ui1_idx_offset + CH_XNG_STAGE_START_SELECT_SVC];
        }
        else{
             x_dbg_stmt("SVCTX unmute First!!!!, Total time is SVCTX unmute - start select svc\n");
             ui4_total_time = g_ui4_ch_xng_stages_Log[ui1_idx_offset + CH_XNG_STAGE_SVCTX_SET_UNMUTE] - 
                            g_ui4_ch_xng_stages_Log[ui1_idx_offset + CH_XNG_STAGE_START_SELECT_SVC];
        }
        
        if(ui4_total_time == 0){
            x_dbg_stmt("Total time is 0!!!!!!!!!!!!!!!!!!!!\n");
            return CLIR_OK;
        }
        x_dbg_stmt("<++++++++ SVCTX = %s CHANNEL CHANGE TIME LOG: Total = %d ++++++++>\n",
                   pps_argv[2],
                   ui4_total_time); 
        
        x_dbg_stmt("Items         Time Cost     Pencent\n");
        
        ui4_times = g_ui4_ch_xng_stages_Log[ui1_idx_offset + CH_XNG_STAGE_REQUEST_CONNECT] -
                    g_ui4_ch_xng_stages_Log[ui1_idx_offset + CH_XNG_STAGE_START_SELECT_SVC];        
        x_dbg_stmt("Kick off         %d           %d\n", ui4_times,  (ui4_times * 100)/ui4_total_time);

        ui4_times = g_ui4_ch_xng_stages_Log[ui1_idx_offset + CH_XNG_STAGE_RECEIVED_CONNECTED] - 
                    g_ui4_ch_xng_stages_Log[ui1_idx_offset + CH_XNG_STAGE_REQUEST_CONNECT];        
        x_dbg_stmt("Connected        %d           %d\n", ui4_times,  (ui4_times * 100)/ui4_total_time);

        ui4_times = g_ui4_ch_xng_stages_Log[ui1_idx_offset + CH_XNG_STAGE_GOT_TV_SIGNAL] - 
                    g_ui4_ch_xng_stages_Log[ui1_idx_offset + CH_XNG_STAGE_RECEIVED_CONNECTED];        
        x_dbg_stmt("Got TV Sigal     %d           %d\n", ui4_times,  (ui4_times * 100)/ui4_total_time);

        ui4_times = g_ui4_ch_xng_stages_Log[ui1_idx_offset + CH_XNG_STAGE_RECEIVED_RESOLUTION_CHG] -
                    g_ui4_ch_xng_stages_Log[ui1_idx_offset + CH_XNG_STAGE_GOT_TV_SIGNAL];        
        x_dbg_stmt("Video Resolution  %d          %d\n", ui4_times,  (ui4_times * 100)/ui4_total_time);

        ui4_times = g_ui4_ch_xng_stages_Log[ui1_idx_offset + CH_XNG_STAGE_SVCTX_SET_UNMUTE] -
                    g_ui4_ch_xng_stages_Log[ui1_idx_offset + CH_XNG_STAGE_RECEIVED_RESOLUTION_CHG];        
        x_dbg_stmt("SVCTX Unmute      %d          %d\n", ui4_times,  (ui4_times * 100)/ui4_total_time);

        ui4_times = g_ui4_ch_xng_stages_Log[ui1_idx_offset + CH_XNG_STAGE_AP_SET_UNMUTE] -
                    g_ui4_ch_xng_stages_Log[ui1_idx_offset + CH_XNG_STAGE_GOT_TV_SIGNAL];        
        x_dbg_stmt("AP Unmute         %d          %d\n", ui4_times,  (ui4_times * 100)/ui4_total_time);
    }
    else{
        x_dbg_stmt("svctx.cctd [svc_name]\n\r");
        x_dbg_stmt("  [svc_name]: main, sub, tv_bypass, mn_bypass\n\r");
        return CLIR_OK;
    }

    return CLIR_OK;
}
#endif
