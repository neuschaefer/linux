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
 * $RCSfile: sm_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description:
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "cli/x_cli.h"
#include "os/inc/x_os.h"
#include "strm_mngr/_sm_utils.h"
#include "strm_mngr/sm_cli.h"
#include "strm_mngr/_sm_sess_tbl.h"
#include "strm_mngr/sm_slctr.h"
#include "strm_mngr/scc/x_scc.h"
#include "strm_mngr/x_sm.h"
#include "scdb/x_scdb_api.h"
#ifdef MW_TV_AV_BYPASS_SUPPORT
#include "strm_mngr/bypass_hdlr/u_sm_bypass_hdlr.h"
#include "strm_mngr/sess_gnrc_hdlr/x_sm_sess_gnrc.h"
#include "strm_mngr/bypass_hdlr/_sm_bypass_hdlr.h"
#endif
#include "strm_mngr/sess_rcrd_hdlr/sm_sess_rcrd.h"
#include "strm_mngr/rcrd_strm_hdlr/_sm_rcrd_hdlr.h"
#include "strm_mngr/video_hdlr/_sm_video_hdlr.h"
#include "dbg/u_dbg.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#ifdef CLI_LVL_ALL

typedef struct
{
   CHAR*    s_trc_lvl_str;
   UINT8    ui1_trc_lvl;
} SM_CLI_TRC_LVL_STR_PAIR_T;

typedef struct
{
   CHAR*    s_trc_mdl_str;
   UINT32   ui4_trc_mdl;
} SM_CLI_TRC_MDL_STR_PAIR_T;

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
static VOID _sm_cli_trace_mdl_enable_help( VOID );

static VOID _sm_cli_trace_mdl_disable_help( VOID );

static BOOL _sm_cli_get_trace_mdl_by_str(
    const CHAR*    s_trc_mdl_str,
    UINT32*        pui4_module );

static INT32 _sm_cli_set_trace_level(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_get_trace_level(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_trace_mdl_enable(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_trace_mdl_disable(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_dump_trace_logs(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_set_ntfy_trace(
    INT32          i4_argc, 
    const CHAR**   pps_argv);

static INT32 _sm_cli_dump_resource(
    INT32          i4_argc,
    const CHAR**   pps_argv);

#ifdef MW_TV_AV_BYPASS_SUPPORT
static INT32 _sm_cli_open_bypass_hdlr(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_close_bypass_hdlr(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_play_bypass_hdlr(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_stop_bypass_hdlr(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_bypass_mode(
    INT32          i4_argc,
    const CHAR**   pps_argv);
#endif
static INT32 _sm_cli_scc_api_test(
    INT32          i4_argc,
    const CHAR**   pps_argv);
static INT32 _sm_cli_plane_mute(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_vp_enhance(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_vp_blur(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_vp_overscan(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_disp_reslution(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_get_slctr_max_msg_num(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_dump_scdb(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_obsvr_test(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_scc_set_vid_cmd(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_scc_get_vid_cmd(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_scc_set_aud_cmd(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_scc_get_aud_cmd(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_scc_set_disp_cmd(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_scc_get_disp_cmd(
    INT32          i4_argc,
    const CHAR**   pps_argv);

#ifdef TIME_MEASUREMENT
static INT32 _sm_cli_set_tms_level(
    INT32          i4_argc,
    const CHAR**   pps_argv);

static INT32 _sm_cli_get_tms_level(
    INT32          i4_argc,
    const CHAR**   pps_argv);

#endif /* TIME_MEASUREMENT */

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/* scc video command table */
static CLI_EXEC_T at_scc_vid_cmd_tbl[] =
{
    {
        "set",
        "s",
        _sm_cli_scc_set_vid_cmd,
        NULL,
        "set function_name parameter ...",
        CLI_SUPERVISOR
    },
    {
        "get",
        "g",
        _sm_cli_scc_get_vid_cmd,
        NULL,
        "get function_name",
        CLI_SUPERVISOR
    },
    
    END_OF_CLI_CMD_TBL
};

/* scc audio command table */
static CLI_EXEC_T at_scc_aud_cmd_tbl[] =
{
    {
        "set",
        "s",
        _sm_cli_scc_set_aud_cmd,
        NULL,
        "set function_name parameter ...",
        CLI_SUPERVISOR
    },
    {
        "get",
        "g",
        _sm_cli_scc_get_aud_cmd,
        NULL,
        "get function_name",
        CLI_SUPERVISOR
    },
      
    END_OF_CLI_CMD_TBL
};

/* scc disp command table */
static CLI_EXEC_T at_scc_disp_cmd_tbl[] =
{
    {
        "set",
        "s",
        _sm_cli_scc_set_disp_cmd,
        NULL,
        "set function_name parameter ...",
        CLI_SUPERVISOR
    },
    {
        "get",
        "g",
        _sm_cli_scc_get_disp_cmd,
        NULL,
        "get function_name",
        CLI_SUPERVISOR
    },
    
    END_OF_CLI_CMD_TBL
};

/* scc command table */
static CLI_EXEC_T at_scc_cmd_tbl[] =
{
    {
        "video",
        "vid",
        NULL,
        at_scc_vid_cmd_tbl,
        "Commands for Video API Test",
        CLI_SUPERVISOR
    },
    {
        "audio",
        "aud",
        NULL,
        at_scc_aud_cmd_tbl,
        "Commands for Audio API Test",
        CLI_SUPERVISOR
    },
    {
        "disp",
        "disp",
        NULL,
        at_scc_disp_cmd_tbl,
        "Commands for Disp API Test",
        CLI_SUPERVISOR
    },
    
    END_OF_CLI_CMD_TBL
};

/* Stream Manager command table */
static CLI_EXEC_T at_sm_cmd_tbl[] =
{
    {
        "sdl",
        "sdl",
        _sm_cli_set_trace_level,
        NULL,
        CLI_SET_DBG_LVL_HELP_STR,
        CLI_SUPERVISOR
    },
    {
        "gdl",
        "gdl",
        _sm_cli_get_trace_level,
        NULL,
        CLI_GET_DBG_LVL_HELP_STR,
        CLI_SUPERVISOR
    },
    {
        "trc_mdl_enable",
        "te",
        _sm_cli_trace_mdl_enable,
        NULL,
        "Enable trace module(all, slctr, gss, vs, as, ccs, rss,bypass, rsh),ex: te ccs",
        CLI_SUPERVISOR
    },
    {
        "trc_mdl_disable",
        "td",
        _sm_cli_trace_mdl_disable,
        NULL,
        "Disable trace module(all, slctr, gss, vs, as, ccs, rss,bypass, rsh),ex: te ccs",
        CLI_SUPERVISOR
    },
    {
        "trc_log_dump",
        "ld",
        _sm_cli_dump_trace_logs,
        NULL,
        "Dump last 32 trace logs",
        CLI_SUPERVISOR
    },
    {   "ntfy trace",
        "nt",
        _sm_cli_set_ntfy_trace,
        NULL,
        "usage: nt y|n",
        CLI_SUPERVISOR
    },
    {
        "res",
        "res",
        _sm_cli_dump_resource,
        NULL,
        "Dump resource info",
        CLI_SUPERVISOR
    },
#ifdef MW_TV_AV_BYPASS_SUPPORT
    {
        "bopen",
        "bypass_open",
        _sm_cli_open_bypass_hdlr,
        NULL,
        "Open bypass handler",
        CLI_SUPERVISOR
    },
    {
        "bclose",
        "bypass_close",
        _sm_cli_close_bypass_hdlr,
        NULL,
        "Close bypass handler",
        CLI_SUPERVISOR
    },
    {
        "bplay",
        "bypass_play",
        _sm_cli_play_bypass_hdlr,
        NULL,
        "Play bypass stream",
        CLI_SUPERVISOR
    },
    {
        "bstop",
        "bypass_stop",
        _sm_cli_stop_bypass_hdlr,
        NULL,
        "Stop bypass stream",
        CLI_SUPERVISOR
    },
    {
        "bmode",
        "bypass_mode",
        _sm_cli_bypass_mode,
        NULL,
        "Set bypass stream mode",
        CLI_SUPERVISOR
    },
#endif
    {
        "mute",
        "plane_mute",
        _sm_cli_plane_mute,
        NULL,
        "mute/unmute video plane",
        CLI_SUPERVISOR
    },
    {
        "enh",
        "enhance",
        _sm_cli_vp_enhance,
        NULL,
        "enhance the video",
        CLI_SUPERVISOR
    },
    {
        "blr",
        "blur",
        _sm_cli_vp_blur,
        NULL,
        "blur the video",
        CLI_SUPERVISOR
    },
    {
        "osc",
        "overscan",
        _sm_cli_vp_overscan,
        NULL,
        "Overscan of video plane",
        CLI_SUPERVISOR
    },
    {
        "rsl",
        "reslution",
        _sm_cli_disp_reslution,
        NULL,
        "Set disply resolution",
        CLI_SUPERVISOR
    },
    {
        "mm",
        "maxmsg",
        _sm_cli_get_slctr_max_msg_num,
        NULL,
        "Get Selector Maximum message number used",
        CLI_SUPERVISOR
    },
    {
        "scdb",
        "scdb",
        _sm_cli_dump_scdb,
        NULL,
        "Dump SCDB records of sessions",
        CLI_SUPERVISOR
    },
    {
        "obsvr",
        "obsvr",
        _sm_cli_obsvr_test,
        NULL,
        "Add a dummy observer to dump stream/session state",
        CLI_SUPERVISOR
    },

    {
        "sat",
        "SCC API test",
        _sm_cli_scc_api_test,
        NULL,
        "test scc API ",
        CLI_SUPERVISOR
    },
    {
        "scc",
        "scc",
        NULL,
        at_scc_cmd_tbl,
        "Commands for SCC API Test",
        CLI_SUPERVISOR
    },
#ifdef TIME_MEASUREMENT
    {
        CLI_SET_TMS_LVL_STR,
        NULL,
        _sm_cli_set_tms_level,
        NULL,
        CLI_SET_TMS_LVL_HELP_STR,
        CLI_SUPERVISOR
    },
    {
        CLI_GET_TMS_LVL_STR,
        NULL,
        _sm_cli_get_tms_level,
        NULL,
        CLI_GET_TMS_LVL_HELP_STR,
        CLI_SUPERVISOR
    },
#endif /* TIME_MEASUREMENT */
    END_OF_CLI_CMD_TBL
};


/* Stream Manager root command table */
static CLI_EXEC_T at_sm_root_cmd_tbl[] =
{
    {
        "sm",
        NULL,
        NULL,
        at_sm_cmd_tbl,
        "Stream Manager command",
        CLI_SUPERVISOR
    },
    END_OF_CLI_CMD_TBL
};

static SM_CLI_TRC_MDL_STR_PAIR_T       a_trc_mdl_str_pair[] =
{
    { "all",     SM_DBG_MOD_ALL },
    { "slctr",   SM_DBG_MOD_SLCTR },
    { "gss",     SM_DBG_MOD_GSS },
    { "rss",     SM_DBG_MOD_RSS },
    { "1394ss",  SM_DBG_MOD_1394SS },
    { "vs",      SM_DBG_MOD_VS },
    { "as",      SM_DBG_MOD_AS },
    { "ccs",     SM_DBG_MOD_CCS },
#ifdef MW_TV_AV_BYPASS_SUPPORT
    { "bypass",  SM_DBG_MOD_BYPASS },
#endif
    { "rsh",     SM_DBG_MOD_RSH },
    { NULL, 0 }
};

static CHAR*   as_sess_cond_str[] =
{
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

static CHAR*   as_strm_cond_str[] =
{
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

#define STRM_TYPE_MAX_NUM      ((UINT32) 32)

static CHAR*   as_strm_type[STRM_TYPE_MAX_NUM] =
{
    "ST_AUDIO",
    "ST_VIDEO",
    "ST_CLOSED_CAPTION",
    "ST_ISDB_CAPTION",
    "ST_ISDB_TEXT",
    "ST_TELETEXT",
    "ST_SUBTITLE",
    "ST_DATA",
    "ST_BYPASS",
    "ST_MM_SUBTITLE",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", ""
};

#define VID_COMP_MASK_EVENT     ((UINT32)0x0000000F)

static CHAR*   as_vid_comp_dec_event[] =
{
    "DEC_OPENED",
    "DEC_CLOSED",
    "DEC_CONNECTED",
    "DEC_DISCONNECTED",
    "DEC_PLAYED",
    "DEC_STOPPED"
};

static CHAR*   as_vid_comp_vp_event[] =
{
    "VP_OPENED",
    "VP_CLOSED",
    "VP_CONNECTED",
    "VP_DISCONNECTED",
    "VP_PLAYED",
    "VP_STOPPED"
};

static CHAR*   as_vid_comp_pmx_event[] =
{
    "PMX_OPENED",
    "PMX_CLOSED",
    "PMX_CONNECTED",
    "PMX_DISCONNECTED",
    "PMX_PLAYED",
    "PMX_STOPPED"
};

static CHAR*   as_vid_comp_tve_event[] =
{
    "TVE_OPENED",
    "TVE_CLOSED",
    "TVE_CONNECTED",
    "TVE_DISCONNECTED",
    "TVE_PLAYED",
    "TVE_STOPPED"
};

static CHAR*   as_vid_comp_pm_event[] =
{
    "PM_CLOSED",
    "PM_PLAYED",
    "PM_STOPPED"
};

#ifdef MW_TV_AV_BYPASS_SUPPORT
static CHAR*   as_vid_comp_vout_event[] =
{
    "VOUT_OPENED",
    "VOUT_CLOSED",
    "VOUT_CONNECTED",
    "VOUT_DISCONNECTED",
    "VOUT_PLAYED",
    "VOUT_STOPPED"
};
#endif

#define         SM_NAME_MAX_SIZE           (16)
static BOOL     b_g_sm_nfy_trace           = FALSE;
static CHAR     s_g_sm_nfy_trace_name[SM_NAME_MAX_SIZE+1];


/*-----------------------------------------------------------------------------
                    Internal functions implementations
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
 * Name: _dump_scdb_as
 *
 * Description: <function description>
 *
 * Inputs:  pt_scdb_rec         References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _dump_scdb_as( const SCDB_REC_T* pt_scdb_rec, UINT16 ui2_idx )
{
    const CHAR*         as_aud_fmt[] =
    {
        "AUD_FMT_UNKNOWN",
        "AUD_FMT_MONO",
        "AUD_FMT_DUAL_MONO",
        "AUD_FMT_STEREO",
        "AUD_FMT_DOLBY",
        "AUD_FMT_TYPE_5_1"
        ,"AUD_FMT_SUBSTEAM"

    };

    if ( pt_scdb_rec->e_rec_type == SCDB_REC_TYPE_AUDIO_MPEG )
    {
        if ( (UINT32)pt_scdb_rec->u.t_audio_mpeg.e_aud_fmt <
             (sizeof(as_aud_fmt)/sizeof(CHAR*) - 1) )
        {
            x_dbg_stmt(
                "    AS#%u, PID(%u), FMT(%s), ENC(%d), LAYER(%d), LANG(%s), %s.\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_audio_mpeg.ui2_pid,
                as_aud_fmt[pt_scdb_rec->u.t_audio_mpeg.e_aud_fmt],
                (INT32)pt_scdb_rec->u.t_audio_mpeg.e_enc,
                (INT32)pt_scdb_rec->u.t_audio_mpeg.e_layer,
                pt_scdb_rec->u.t_audio_mpeg.s_lang,
                ((pt_scdb_rec->u.t_audio_mpeg.b_default)?"DFT":"") );
        }
        else
        {
            x_dbg_stmt(
                "    AS#%u ERROR, PID(%u), FMT(%d), ENC(%d), LAYER(%d), LANG(%s), %s.\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_audio_mpeg.ui2_pid,
                pt_scdb_rec->u.t_audio_mpeg.e_aud_fmt,
                (INT32)pt_scdb_rec->u.t_audio_mpeg.e_enc,
                (INT32)pt_scdb_rec->u.t_audio_mpeg.e_layer,
                pt_scdb_rec->u.t_audio_mpeg.s_lang,
                ((pt_scdb_rec->u.t_audio_mpeg.b_default)?"DFT":"") );
        }
    }
    else if ( pt_scdb_rec->e_rec_type == SCDB_REC_TYPE_AUDIO_ANALOG )
    {
        if ( (UINT32)pt_scdb_rec->u.t_audio_analog.e_aud_fmt <
             (sizeof(as_aud_fmt)/sizeof(CHAR*) - 1) )
        {
            x_dbg_stmt(
                "    AS#%u, TV_SYS(0x%.8x), AUD_SYS(0x%.8x), FMT(%s), ENC(%u), %s.!\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_audio_analog.ui4_tv_sys,
                pt_scdb_rec->u.t_audio_analog.ui4_audio_sys,
                as_aud_fmt[(UINT32)pt_scdb_rec->u.t_audio_analog.e_aud_fmt],
                (UINT32)pt_scdb_rec->u.t_audio_analog.e_enc,
                ((pt_scdb_rec->u.t_audio_analog.b_default)?"DFT":"") );
        }
        else
        {
            x_dbg_stmt(
                "    AS#%u ERROR, TV_SYS(0x%.8x), AUD_SYS(0x%.8x), FMT(%d), ENC(%u), %s.!\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_audio_analog.ui4_tv_sys,
                pt_scdb_rec->u.t_audio_analog.ui4_audio_sys,
                (UINT32)pt_scdb_rec->u.t_audio_analog.e_aud_fmt,
                (UINT32)pt_scdb_rec->u.t_audio_analog.e_enc,
                ((pt_scdb_rec->u.t_audio_analog.b_default)?"DFT":"") );
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: _dump_scdb_vs
 *
 * Description: <function description>
 *
 * Inputs:  pt_scdb_rec         References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _dump_scdb_vs( const SCDB_REC_T* pt_scdb_rec, UINT16 ui2_idx )
{
    const CHAR*         as_vid_fmt[] =
    {
        "VID_FMT_UNKNOWN",
        "VID_FMT_HD_4_3",
        "VID_FMT_HD_16_9",
        "VID_FMT_SD_4_3",
        "VID_FMT_SD_16_9"
    };

    const CHAR*         as_vid_color_sys[] =
    {
        "COLOR_SYS_UNKNOWN",
        "COLOR_SYS_NTSC",
        "COLOR_SYS_PAL",
        "COLOR_SYS_SECAM",
        "COLOR_SYS_NTSC_443",
        "COLOR_SYS_PAL_M",
        "COLOR_SYS_PAL_N",
        "COLOR_SYS_PAL_60"
    };

    const CHAR*         as_vid_enc[] =
    {
        "VID_ENC_UNKNOWN",
        "VID_ENC_MPEG_1",
        "VID_ENC_MPEG_2",
        "VID_ENC_MPEG_4",
        "VID_ENC_DIVX_311",
        "VID_ENC_DIVX_4",
        "VID_ENC_DIVX_5",
        "VID_ENC_XVID",
        "VID_ENC_WMV1",
        "VID_ENC_WMV2",
        "VID_ENC_WMV3",
        "VID_ENC_WVC1",
        "VID_ENC_H264",
        "VID_ENC_H263",
        "VID_ENC_MJPEG",
        "VID_ENC_RV8",
        "VID_ENC_RV9",
        "VID_ENC_SORENSON",
        "VID_ENC_AVS",
        "VID_ENC_NV12",
        "VID_ENC_VP8",
        "VID_ENC_VP6"
    };

    if ( pt_scdb_rec->e_rec_type == SCDB_REC_TYPE_VIDEO_MPEG )
    {
        if ( (UINT32)pt_scdb_rec->u.t_video_mpeg.e_vid_fmt <
             (sizeof(as_vid_fmt)/sizeof(CHAR*) - 1) &&
             (UINT32)pt_scdb_rec->u.t_video_mpeg.e_enc <
             (sizeof(as_vid_enc)/sizeof(CHAR*) - 1) )
        {
            x_dbg_stmt(
                "    VS#%u, PID(%u), ENC(%s), FMT(%s), %s.\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_video_mpeg.ui2_pid,
                as_vid_enc[pt_scdb_rec->u.t_video_mpeg.e_enc],
                as_vid_fmt[pt_scdb_rec->u.t_video_mpeg.e_vid_fmt],
                ((pt_scdb_rec->u.t_video_mpeg.b_default)?"DFT":"") );
        }
        else
        {
            x_dbg_stmt(
                "    VS#%d ERROR, PID(%u), ENC(%d), FMT(%d), %s.\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_video_mpeg.ui2_pid,
                pt_scdb_rec->u.t_video_mpeg.e_enc,
                pt_scdb_rec->u.t_video_mpeg.e_vid_fmt,
                ((pt_scdb_rec->u.t_video_mpeg.b_default)?"DFT":"") );
        }
    }
    if ( pt_scdb_rec->e_rec_type == SCDB_REC_TYPE_VIDEO_ANALOG )
    {
        if ( (UINT32)pt_scdb_rec->u.t_video_analog.e_vid_fmt <
             (sizeof(as_vid_fmt)/sizeof(CHAR*) - 1) &&
             (UINT32)pt_scdb_rec->u.t_video_analog.e_color_sys + 1 <
             (sizeof(as_vid_color_sys)/sizeof(CHAR*) - 1) )
        {
            x_dbg_stmt(
                "    VS#%d, CLR_SYS(%u), FMT(%s), %s, %s.\r\n",
                ui2_idx,
                as_vid_color_sys[pt_scdb_rec->u.t_video_analog.e_color_sys + 1],
                as_vid_fmt[pt_scdb_rec->u.t_video_analog.e_vid_fmt],
                ((pt_scdb_rec->u.t_video_analog.b_default)?"DFT":""),
                ((pt_scdb_rec->u.t_video_analog.b_detected)?"DETECTED":"") );
        }
        else
        {
            x_dbg_stmt(
                "    VS#%d ERROR, CLR_SYS(%u), FMT(%u), %s, %s.\r\n",
                ui2_idx,
                (UINT32)pt_scdb_rec->u.t_video_analog.e_color_sys,
                (UINT32)pt_scdb_rec->u.t_video_analog.e_vid_fmt,
                ((pt_scdb_rec->u.t_video_analog.b_default)?"DFT":""),
                ((pt_scdb_rec->u.t_video_analog.b_detected)?"DETECTED":"") );
        }
    }
    if( pt_scdb_rec->e_rec_type == SCDB_REC_TYPE_VIDEO_AVI)
    {
        if ( (UINT32)pt_scdb_rec->u.t_video_avi.e_enc <
             (sizeof(as_vid_enc)/sizeof(CHAR*) - 1) )
        {
            x_dbg_stmt(
                "    VS#%u, stream_id(%u), ENC(%s).\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_video_avi.ui4_stream_id,
                as_vid_enc[pt_scdb_rec->u.t_video_avi.e_enc] );
        }
        else
        {
            x_dbg_stmt(
                "    VS#%d ERROR, stream_id(%u), ENC(%s).\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_video_avi.ui4_stream_id,
                pt_scdb_rec->u.t_video_avi.e_enc );
        }
    }
    if( pt_scdb_rec->e_rec_type == SCDB_REC_TYPE_VIDEO_MP4 )
    {
        if ( (UINT32)pt_scdb_rec->u.t_video_mp4.e_enc <
             (sizeof(as_vid_enc)/sizeof(CHAR*) - 1) )
        {
            x_dbg_stmt(
                "    VS#%u, stream_id(%u), ENC(%s).\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_video_mp4.ui4_stream_id,
                as_vid_enc[pt_scdb_rec->u.t_video_mp4.e_enc] );
        }
        else
        {
            x_dbg_stmt(
                "    VS#%d ERROR, stream_id(%u), ENC(%s).\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_video_mp4.ui4_stream_id,
                pt_scdb_rec->u.t_video_mp4.e_enc );
        }
    }
    if( pt_scdb_rec->e_rec_type == SCDB_REC_TYPE_VIDEO_ASF)
    {
        if ( (UINT32)pt_scdb_rec->u.t_video_asf.e_enc <
             (sizeof(as_vid_enc)/sizeof(CHAR*) - 1) )
        {
            x_dbg_stmt(
                "    VS#%u, stream_id(%u), ENC(%s).\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_video_asf.ui4_stream_id,
                as_vid_enc[pt_scdb_rec->u.t_video_asf.e_enc] );
        }
        else
        {
            x_dbg_stmt(
                "    VS#%d ERROR, stream_id(%u), ENC(%s).\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_video_asf.ui4_stream_id,
                pt_scdb_rec->u.t_video_asf.e_enc );
        }
    }
    if( pt_scdb_rec->e_rec_type == SCDB_REC_TYPE_VIDEO_MKV)
    {
        if ( (UINT32)pt_scdb_rec->u.t_video_mkv.e_enc <
             (sizeof(as_vid_enc)/sizeof(CHAR*) - 1) )
        {
            x_dbg_stmt(
                "    VS#%u, stream_id(%u), ENC(%s).\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_video_mkv.ui4_stream_id,
                as_vid_enc[pt_scdb_rec->u.t_video_mkv.e_enc] );
        }
        else
        {
            x_dbg_stmt(
                "    VS#%d ERROR, stream_id(%u), ENC(%s).\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_video_mkv.ui4_stream_id,
                pt_scdb_rec->u.t_video_mkv.e_enc );
        }
    }
    if( pt_scdb_rec->e_rec_type == SCDB_REC_TYPE_VIDEO_PS)
    {
        if ( (UINT32)pt_scdb_rec->u.t_video_ps.e_enc <
             (sizeof(as_vid_enc)/sizeof(CHAR*) - 1) )
        {
            x_dbg_stmt(
                "    VS#%u, stream_id(%u), ENC(%s).\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_video_ps.ui1_stream_id,
                as_vid_enc[pt_scdb_rec->u.t_video_ps.e_enc] );
        }
        else
        {
            x_dbg_stmt(
                "    VS#%d ERROR, stream_id(%u), ENC(%s).\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_video_ps.ui1_stream_id,
                pt_scdb_rec->u.t_video_ps.e_enc );
        }
    }
    if( pt_scdb_rec->e_rec_type == SCDB_REC_TYPE_VIDEO_RM)
    {
        if ( (UINT32)pt_scdb_rec->u.t_video_rm.e_enc <
             (sizeof(as_vid_enc)/sizeof(CHAR*) - 1) )
        {
            x_dbg_stmt(
                "    VS#%u, stream_id(%u), ENC(%s).\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_video_rm.ui4_stream_id,
                as_vid_enc[pt_scdb_rec->u.t_video_rm.e_enc] );
        }
        else
        {
            x_dbg_stmt(
                "    VS#%d ERROR, stream_id(%u), ENC(%s).\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_video_rm.ui4_stream_id,
                pt_scdb_rec->u.t_video_rm.e_enc );
        }    
    }
    if( pt_scdb_rec->e_rec_type == SCDB_REC_TYPE_VIDEO_FLV)
    {
        if ( (UINT32)pt_scdb_rec->u.t_video_flv.e_enc <
             (sizeof(as_vid_enc)/sizeof(CHAR*) - 1) )
        {
            x_dbg_stmt(
                "    VS#%u, stream_id(%u), ENC(%s).\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_video_flv.ui4_stream_id,
                as_vid_enc[pt_scdb_rec->u.t_video_flv.e_enc] );
        }
        else
        {
            x_dbg_stmt(
                "    VS#%d ERROR, stream_id(%u), ENC(%s).\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_video_flv.ui4_stream_id,
                pt_scdb_rec->u.t_video_flv.e_enc );
        }    
    }
    if( pt_scdb_rec->e_rec_type == SCDB_REC_TYPE_VIDEO_ES)
    {
        if ( (UINT32)pt_scdb_rec->u.t_video_es.e_enc <
             (sizeof(as_vid_enc)/sizeof(CHAR*) - 1) )
        {
            x_dbg_stmt(
                "    VS#%u, stream_id(%u), ENC(%s).\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_video_es.ui4_stream_id,
                as_vid_enc[pt_scdb_rec->u.t_video_es.e_enc] );
        }
        else
        {
            x_dbg_stmt(
                "    VS#%d ERROR, stream_id(%u), ENC(%s).\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_video_es.ui4_stream_id,
                pt_scdb_rec->u.t_video_es.e_enc );
        }  
    }
    if( pt_scdb_rec->e_rec_type == SCDB_REC_TYPE_VIDEO_OGG)
    {
        if ( (UINT32)pt_scdb_rec->u.t_video_ogg.e_enc <
             (sizeof(as_vid_enc)/sizeof(CHAR*) - 1) )
        {
            x_dbg_stmt(
                "    VS#%u, stream_id(%u), ENC(%s).\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_video_ogg.ui4_stream_id,
                as_vid_enc[pt_scdb_rec->u.t_video_ogg.e_enc] );
        }
        else
        {
            x_dbg_stmt(
                "    VS#%d ERROR, stream_id(%u), ENC(%s).\r\n",
                ui2_idx,
                pt_scdb_rec->u.t_video_ogg.ui4_stream_id,
                pt_scdb_rec->u.t_video_ogg.e_enc );
        }  
    }

}

/*-----------------------------------------------------------------------------
 * Name: _dump_scdb_ccs
 *
 * Description: <function description>
 *
 * Inputs:  pt_scdb_rec         References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _dump_scdb_ccs( const SCDB_REC_T* pt_scdb_rec, UINT16 ui2_idx )
{
    const CHAR*         as_cc_type[] =
    {
        "LINE_21_CC",
        "DTV_CC"
    };

    if ( (UINT32)pt_scdb_rec->u.t_atsc_cc.e_type <
         (sizeof(as_cc_type)/sizeof(CHAR*) - 1) )
    {
        x_dbg_stmt(
            "    CC#%u, CC_TYPE(%s), LANG(%s), %s.\r\n",
            ui2_idx,
            as_cc_type[pt_scdb_rec->u.t_atsc_cc.e_type],
            pt_scdb_rec->u.t_atsc_cc.s_lang,
            ((pt_scdb_rec->u.t_atsc_cc.b_default)?"DFT":"") );
    }
    else
    {
        x_dbg_stmt(
            "    CC#%u ERROR, CC_TYPE(%d), LANG(%s), %s.\r\n",
            ui2_idx,
            pt_scdb_rec->u.t_atsc_cc.e_type,
            pt_scdb_rec->u.t_atsc_cc.s_lang,
            ((pt_scdb_rec->u.t_atsc_cc.b_default)?"DFT":"") );
    }
    if ( pt_scdb_rec->u.t_atsc_cc.e_type == 0 )
    {
        x_dbg_stmt(
            "    \tCC-ID(%u), %s, %s.\r\n",
            (pt_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_line_21.ui1_cc_id),
            ((pt_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_line_21.b_easy_reader)?"EASY_READER":""),
            ((pt_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_line_21.b_wide_aspect)?"16:9":"4:3") );
    }
    else
    {
        x_dbg_stmt(
            "    \tservice number(%d), %s, %s.\r\n",
            pt_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_dtv.ui1_caption_svc_num,
            ((pt_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_dtv.b_easy_reader)?"EASY_READER":""),
            ((pt_scdb_rec->u.t_atsc_cc.udata.t_cc_atsc_dtv.b_wide_aspect)?"16:9":"4:3") );
    }
}

/*-----------------------------------------------------------------------------
 * Name: _dump_scdb
 *
 * Description: <function description>
 *
 * Inputs:  h_scdb         References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _dump_scdb(
    HANDLE_T         h_scdb )
{
    INT32               i4_ret;
    UINT16              ui2_num_recs = 0;
    UINT16              ui2_idx = 0;
    STREAM_COMP_ID_T    t_stream_descr;
    SCDB_REC_T          t_scdb_rec;
    UINT32              ui4_db_version;
    STREAM_TYPE_T       e_strm_type = ST_UNKNOWN;

    x_dbg_stmt( "SCDB Handle = 0x%.8X\r\n", h_scdb );
    for( e_strm_type = ST_AUDIO; e_strm_type <= ST_DATA; e_strm_type++ )
    {
        i4_ret = x_scdb_get_num_recs(
            h_scdb,
            e_strm_type,
            &ui2_num_recs,
            &ui4_db_version );
        if ( i4_ret != SCDBR_OK )
        {
            x_dbg_stmt( "[SM] x_scdb_get_num_recs() fail return %d\r\n", i4_ret );
            return;
        }
        if ( ui2_num_recs == 0 )
        {
            continue;
        }
        x_dbg_stmt( "  Stream Type = %s\r\n",
            as_strm_type[((UINT32)e_strm_type-(UINT32)ST_AUDIO)] );

        for( ui2_idx = 0; ui2_idx < ui2_num_recs; ++ui2_idx )
        {
            i4_ret = x_scdb_get_rec_by_idx(
                    h_scdb,
                    e_strm_type,
                    ui2_idx,
                    &t_stream_descr,
                    &t_scdb_rec,
                    &ui4_db_version );
            if ( i4_ret != SCDBR_OK )
            {
                x_dbg_stmt( "[SM] x_scdb_get_rec_by_idx() fail return %d\r\n", i4_ret );
                return;
            }

            switch( e_strm_type )
            {
            case ST_AUDIO:
                _dump_scdb_as( &t_scdb_rec, ui2_idx );
                break;

            case ST_VIDEO:
                _dump_scdb_vs( &t_scdb_rec, ui2_idx );
                break;

            case ST_CLOSED_CAPTION:
                _dump_scdb_ccs( &t_scdb_rec, ui2_idx );
                break;

            default:
                /* ignore */
                break;
            }
        }
    }
}

static VOID _sm_cli_trace_mdl_enable_help( VOID )
{
    x_dbg_stmt( "Usage:\r\n" );
    x_dbg_stmt( "te [all|slctr|gss|vs|as|ccs|rss|bypass|rsh]        ;Enable trace message for the module\r\n" );
    x_dbg_stmt( "te                                  ;Taggle between ALL enable/disable\r\n" );
}

static VOID _sm_cli_trace_mdl_disable_help( VOID )
{
    x_dbg_stmt( "Usage:\r\n" );
    x_dbg_stmt( "td [all|slctr|gss|vs|as|ccs|rss|bypass|rsh]        ;Enable trace message for the module\r\n" );
    x_dbg_stmt( "td                                  ;Taggle between ALL enable/disable\r\n" );
}

static BOOL _sm_cli_get_trace_mdl_by_str(
    const CHAR* s_trc_mdl_str,
    UINT32*     pui4_module )
{
    INT32   i4_i = 0;

    if ( !s_trc_mdl_str || !pui4_module )
    {
        return FALSE;
    }

    while( a_trc_mdl_str_pair[i4_i].s_trc_mdl_str != NULL ||
           a_trc_mdl_str_pair[i4_i].ui4_trc_mdl != 0 )
    {
        if ( x_strcmp( s_trc_mdl_str, a_trc_mdl_str_pair[i4_i].s_trc_mdl_str ) == 0 )
        {
            *pui4_module = a_trc_mdl_str_pair[i4_i].ui4_trc_mdl;
            return TRUE;
        }
        ++i4_i;
    }

    return FALSE;
}

static INT32 _sm_cli_set_trace_level(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    UINT16 ui2_dbg_level;

    if (x_cli_parse_dbg_level (i4_argc, pps_argv, &ui2_dbg_level) == CLIR_OK)
    {
        sm_dbg_set_trace_level( ui2_dbg_level );
    }

    x_cli_show_dbg_level ( sm_get_trace_level() );
    return (CLIR_OK);
}

static INT32 _sm_cli_get_trace_level(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    return x_cli_show_dbg_level ( sm_get_trace_level() );
}

static VOID _show_module_mask_str( UINT32 ui4_mdl_mask )
{
    if ( ui4_mdl_mask == SM_DBG_MOD_NONE )
    {
        x_dbg_stmt( "Module mask = NONE\r\n" );
    }
    else if ( ui4_mdl_mask == SM_DBG_MOD_ALL )
    {
        x_dbg_stmt( "Module mask = ALL\r\n" );
    }
    else
    {
        x_dbg_stmt( "Module mask =" );

        if ( ui4_mdl_mask&SM_DBG_MOD_SLCTR )
        {
            x_dbg_stmt( " slctr" );
        }
        if ( ui4_mdl_mask&SM_DBG_MOD_GSS )
        {
            x_dbg_stmt( " gss" );
        }
        if ( ui4_mdl_mask&SM_DBG_MOD_RSS )
        {
            x_dbg_stmt( " rss" );
        }
        if ( ui4_mdl_mask&SM_DBG_MOD_VS )
        {
            x_dbg_stmt( " vs" );
        }
        if ( ui4_mdl_mask&SM_DBG_MOD_AS )
        {
            x_dbg_stmt( " as" );
        }
        if ( ui4_mdl_mask&SM_DBG_MOD_CCS )
        {
            x_dbg_stmt( " ccs" );
        }
#ifdef MW_TV_AV_BYPASS_SUPPORT
        if ( ui4_mdl_mask&SM_DBG_MOD_BYPASS)
        {
            x_dbg_stmt( " bypass" );
        }
#endif
        if ( ui4_mdl_mask&SM_DBG_MOD_RSH )
        {
            x_dbg_stmt( " rsh" );
        }
        x_dbg_stmt( "\r\n" );
    }
}

static INT32 _sm_cli_trace_mdl_enable(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    UINT32  ui4_module_mask;
    UINT32  ui4_module;

    if ( i4_argc > 2 || i4_argc<=0 )
    {
        _sm_cli_trace_mdl_enable_help();
        return -1;
    }

    sm_dbg_get_trace_info( NULL, &ui4_module_mask );
    if ( i4_argc == 1 )
    {
        if ( ui4_module_mask == SM_DBG_MOD_NONE )
        {
            sm_dbg_set_module_mask(SM_DBG_MOD_ALL);
            x_dbg_stmt( "OK! Enable all trace module.\r\n" );
        }
        else
        {
            sm_dbg_set_module_mask(SM_DBG_MOD_NONE);
            x_dbg_stmt( "OK! Disable all trace module.\r\n" );
        }
        return 0;
    }

    if ( !pps_argv || !pps_argv[1] )
    {
        _sm_cli_trace_mdl_enable_help();
        return -1;
    }

    if ( !_sm_cli_get_trace_mdl_by_str( pps_argv[1], &ui4_module ) )
    {
        _sm_cli_trace_mdl_enable_help();
        return -1;
    }

    sm_dbg_set_module_mask( (ui4_module_mask|ui4_module) );

    x_dbg_stmt( "OK! Enable trace module [%s].\r\n", pps_argv[1] );
    _show_module_mask_str( (ui4_module_mask|ui4_module) );

    return 0;
}

static INT32 _sm_cli_trace_mdl_disable(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    UINT32  ui4_module_mask;
    UINT32  ui4_module;

    if ( i4_argc > 2 || i4_argc<=0 )
    {
        _sm_cli_trace_mdl_disable_help();
        return -1;
    }

    sm_dbg_get_trace_info( NULL, &ui4_module_mask );
    if ( i4_argc == 1 )
    {
        if ( ui4_module_mask == SM_DBG_MOD_NONE )
        {
            sm_dbg_set_module_mask(SM_DBG_MOD_ALL);
            x_dbg_stmt( "OK! Enable all trace module.\r\n" );
        }
        else
        {
            sm_dbg_set_module_mask(SM_DBG_MOD_NONE);
            x_dbg_stmt( "OK! Disable all trace module.\r\n" );
        }
        return 0;
    }

    if ( !pps_argv || !pps_argv[1] )
    {
        _sm_cli_trace_mdl_disable_help();
        return -1;
    }

    if ( !_sm_cli_get_trace_mdl_by_str( pps_argv[1], &ui4_module ) )
    {
        _sm_cli_trace_mdl_disable_help();
        return -1;
    }

    sm_dbg_set_module_mask( (ui4_module_mask&(~ui4_module)) );

    x_dbg_stmt( "OK! Disable trace module [%s].\r\n", pps_argv[1] );
    _show_module_mask_str( (ui4_module_mask&(~ui4_module)) );

    return 0;
}

static INT32 _sm_cli_dump_trace_logs(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    x_dbg_stmt( ">>>>>>>>\r\n" );
    sm_dbg_dump_records();
    x_dbg_stmt( "<<<<<<<<\r\n" );
    x_dbg_stmt( "OK! Dump trace logs is done.\r\n" );
    return 0;
}

static INT32 _sm_cli_dump_vid_comp_event( HANDLE_T h_strm )
{
    SM_VIDEO_STREAM_T*  pt_vs_obj;

    pt_vs_obj = (SM_VIDEO_STREAM_T*)sm_get_strm_obj( h_strm );
    if( pt_vs_obj == NULL )
    {
        return SMR_INV_HANDLE;
    }

    if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
    {
        if( pt_vs_obj->t_vs_comp_grp.t_tve.h_comp == NULL_HANDLE )
        {
            x_dbg_stmt("\t\tCOMP_EVENT: dec(%s), vp(%s), pmx(%s), pm(%s)\r\n",
                        as_vid_comp_dec_event[ pt_vs_obj->ui4_lastest_dec_event & VID_COMP_MASK_EVENT ],
                        as_vid_comp_vp_event[ pt_vs_obj->ui4_lastest_vp_event & VID_COMP_MASK_EVENT ],
                        as_vid_comp_pmx_event[ pt_vs_obj->ui4_lastest_pmx_event & VID_COMP_MASK_EVENT],
                        as_vid_comp_pm_event[ pt_vs_obj->ui4_lastest_pm_event & VID_COMP_MASK_EVENT ] );
        }
        else
        {
            x_dbg_stmt("\t\tCOMP_EVENT: dec(%s), vp(%s), pmx(%s), tve(%s), pm(%s)\r\n",
                        as_vid_comp_dec_event[ pt_vs_obj->ui4_lastest_dec_event & VID_COMP_MASK_EVENT ],
                        as_vid_comp_vp_event[ pt_vs_obj->ui4_lastest_vp_event & VID_COMP_MASK_EVENT ],
                        as_vid_comp_pmx_event[ pt_vs_obj->ui4_lastest_pmx_event & VID_COMP_MASK_EVENT],
                        as_vid_comp_tve_event[ pt_vs_obj->ui4_lastest_tve_event & VID_COMP_MASK_EVENT ],
                        as_vid_comp_pm_event[ pt_vs_obj->ui4_lastest_pm_event & VID_COMP_MASK_EVENT ] );
        }
    }
    else
    {
        if( pt_vs_obj->t_vs_comp_grp.t_tve.h_comp == NULL_HANDLE )
        {
            x_dbg_stmt("\t\tCOMP_EVENT: dec(%s), vp(%s), pmx(%s)\r\n",
                        as_vid_comp_dec_event[ pt_vs_obj->ui4_lastest_dec_event & VID_COMP_MASK_EVENT ],
                        as_vid_comp_vp_event[ pt_vs_obj->ui4_lastest_vp_event & VID_COMP_MASK_EVENT ],
                        as_vid_comp_pmx_event[ pt_vs_obj->ui4_lastest_pmx_event & VID_COMP_MASK_EVENT] );
        }
        else
        {
            x_dbg_stmt("\t\tCOMP_EVENT: dec(%s), vp(%s), pmx(%s), tve(%s)\r\n",
                        as_vid_comp_dec_event[ pt_vs_obj->ui4_lastest_dec_event & VID_COMP_MASK_EVENT ],
                        as_vid_comp_vp_event[ pt_vs_obj->ui4_lastest_vp_event & VID_COMP_MASK_EVENT ],
                        as_vid_comp_pmx_event[ pt_vs_obj->ui4_lastest_pmx_event & VID_COMP_MASK_EVENT],
                        as_vid_comp_tve_event[ pt_vs_obj->ui4_lastest_tve_event & VID_COMP_MASK_EVENT ] );
        }
    }

    return SMR_OK;
}

#ifdef MW_TV_AV_BYPASS_SUPPORT
static INT32 _sm_cli_dump_bypass_comp_event( HANDLE_T h_strm )
{
    SM_BYPASS_STREAM_T*  pt_bypass_obj;

    pt_bypass_obj = (SM_BYPASS_STREAM_T*)sm_get_strm_obj( h_strm );
    if( pt_bypass_obj == NULL )
    {
        return SMR_INV_HANDLE;
    }

    if ( IS_MPEG_BYPASS_STRM(pt_bypass_obj))
    {
         x_dbg_stmt("\t\tCOMP_EVENT: dec(%s), vout(%s), tve(%s), pm(%s)\r\n",
                     as_vid_comp_dec_event[ pt_bypass_obj->ui4_lastest_dec_event & VID_COMP_MASK_EVENT ],
                     as_vid_comp_vout_event[ pt_bypass_obj->ui4_lastest_vout_event & VID_COMP_MASK_EVENT ],
                     as_vid_comp_tve_event[ pt_bypass_obj->ui4_lastest_tve_event & VID_COMP_MASK_EVENT ],
                     as_vid_comp_pm_event[ pt_bypass_obj->ui4_lastest_pm_event & VID_COMP_MASK_EVENT ] );
    }
    else
    {
 
         x_dbg_stmt("\t\tCOMP_EVENT: dec(%s), vout(%s), tve(%s)\r\n",
                     as_vid_comp_dec_event[ pt_bypass_obj->ui4_lastest_dec_event & VID_COMP_MASK_EVENT ],
                     as_vid_comp_vout_event[ pt_bypass_obj->ui4_lastest_vout_event & VID_COMP_MASK_EVENT ],
                     as_vid_comp_tve_event[ pt_bypass_obj->ui4_lastest_tve_event & VID_COMP_MASK_EVENT ] );
    }

    return SMR_OK;
}
#endif

static INT32 _sm_cli_dump_strm( HANDLE_T h_strm )
{
    INT32               i4_ret;
    SIZE_T              z_get_size = 0;
    SM_COND_T           e_cond = SM_COND_IGNORE;
    STREAM_COMP_ID_T    t_strm_id;

    x_dbg_stmt( "Stream(0x%.8X): ", (UINT32)h_strm );

    z_get_size = sizeof( SM_COND_T );
    i4_ret = x_sm_get( h_strm,
                       SM_STRM_GET_TYPE_STATE,
                       (VOID*)&e_cond,
                       &z_get_size );
    if ( i4_ret == SMR_OK )
    {
        if ( e_cond <= SM_SESS_COND_ERROR )
        {
            x_dbg_stmt( "COND(%s)\r\n", as_strm_cond_str[e_cond] );
        }
        else
        {
            x_dbg_stmt( "COND(%d)\r\n", e_cond );
        }
    }
    else
    {
        x_dbg_stmt( "Error! x_sm_get(SM_STRM_GET_TYPE_STATE) fail, return %d.\r\n",
                    i4_ret );
        return 0;
    }

    z_get_size = sizeof(STREAM_COMP_ID_T);
    i4_ret = x_sm_get( h_strm,
                       SM_STRM_GET_TYPE_ID,
                       (VOID*)&t_strm_id,
                       &z_get_size );
    if ( i4_ret != SMR_OK )
    {
        x_dbg_stmt( "Error! x_sm_get(SM_STRM_GET_TYPE_ID) fail, return %d.\r\n",
                    i4_ret );
        return 0;
    }
    
    if ( t_strm_id.e_type == ST_VIDEO )
    {
        VSH_SRC_RESOLUTION_INFO_T t_src_res;

        z_get_size = sizeof(VSH_SRC_RESOLUTION_INFO_T);
        i4_ret = x_sm_get( h_strm,
                           SM_VSH_GET_TYPE_VIDEO_SRC_RESOLUTION,
                           (VOID*)&t_src_res,
                           &z_get_size );
        if ( i4_ret == SMR_OK )
        {
            x_dbg_stmt( "\t\tSTRM_COMP_ID( e_type=%s, pv_stream_tag=0x%.8x, fr=%u, w*h=%u*%u,%c, ar=%u, tag3d=%u )\r\n",
                        as_strm_type[(UINT32)t_strm_id.e_type],
                        (UINT32)t_strm_id.pv_stream_tag,
                        t_src_res.ui4_frame_rate,
                        t_src_res.ui4_width,
                        t_src_res.ui4_height,
                        t_src_res.b_is_progressive?'P':'I',
                        t_src_res.e_src_asp_ratio,
                        t_src_res.e_src_tag3d_type );
        }
        else
        {
            x_dbg_stmt( "\t\tSTRM_COMP_ID( e_type=%s, pv_stream_tag=0x%.8x)\r\n",
                        as_strm_type[(UINT32)t_strm_id.e_type],
                        (UINT32)t_strm_id.pv_stream_tag );
        }

        i4_ret = _sm_cli_dump_vid_comp_event( h_strm );
        if ( i4_ret != SMR_OK )
        {
            x_dbg_stmt( "Error! _sm_cli_dump_vid_comp_event(0x%.8X) fail, return %d !\r\n",
                         h_strm, i4_ret );
            return 0;
        }

    }
    else if ( t_strm_id.e_type >= ST_AUDIO && t_strm_id.e_type <= ST_RCRD_STRM )
    {
        x_dbg_stmt( "\t\tSTRM_COMP_ID( e_type=%s, pv_stream_tag=0x%.8x)\r\n",
                    as_strm_type[(UINT32)t_strm_id.e_type],
                    (UINT32)t_strm_id.pv_stream_tag );
        
#ifdef MW_TV_AV_BYPASS_SUPPORT
        if( t_strm_id.e_type == ST_BYPASS ) 
        {

            i4_ret = _sm_cli_dump_bypass_comp_event( h_strm );
            if ( i4_ret != SMR_OK )
            {
                x_dbg_stmt( "Error! _sm_cli_dump_bypass_comp_event(0x%.8X) fail, return %d !\r\n",
                             h_strm, i4_ret );
                return 0;
            }
        }
#endif
    }
    else
    {
        x_dbg_stmt( "\t\tSTRM_COMP_ID( e_type=%d, pv_stream_tag=0x%.8x)\r\n",
                    (INT32)t_strm_id.e_type,
                    (UINT32)t_strm_id.pv_stream_tag );
    }

    return 0;
}

static INT32 _sm_cli_dump_sess(
    HANDLE_T h_sess )
{
    INT32               i4_ret;
    SIZE_T              z_i = 0, z_get_size = 0;
    SIZE_T              z_num_strm = 0;
    HANDLE_T*           ph_strm_tbl = NULL;
    SM_SESS_COND_T      e_cond = SM_SESS_COND_IGNORE;
    CHAR                s_sess_hdlr_name[SM_HDLR_NAME_MAX_SIZE+1]= {0};
    CHAR                s_dest_grp_name[RM_NAME_LEN+1];
    HANDLE_T            h_conn = NULL_HANDLE;

    x_dbg_stmt( "Session(0x%.8X)(%u): ", (UINT32)h_sess, (UINT32)h_sess );

    z_get_size = sizeof(SM_SESS_COND_T);
    i4_ret = x_sm_get( h_sess,
                       SM_SESS_GET_TYPE_STATE,
                       (VOID*)&e_cond,
                       &z_get_size );
    if ( i4_ret == SMR_OK )
    {
        if ( e_cond <= SM_SESS_COND_ERROR )
        {
            x_dbg_stmt( "COND(%s), ", as_sess_cond_str[e_cond] );
        }
        else
        {
            x_dbg_stmt( "COND(%d), ", e_cond );
        }
    }

    z_get_size = SM_HDLR_NAME_MAX_SIZE + 1;
    i4_ret = x_sm_get( h_sess,
                       SM_SESS_GET_TYPE_HDLR_NAME,
                       (VOID*)s_sess_hdlr_name,
                       &z_get_size );
    if ( i4_ret == SMR_OK )
    {
        x_dbg_stmt( "HDLR(%s), ", s_sess_hdlr_name );
    }

    z_get_size = RM_NAME_LEN + 1;
    i4_ret = x_sm_get( h_sess,
                       SM_SESS_GET_TYPE_DEST_GRP_NAME,
                       (VOID*)s_dest_grp_name,
                       &z_get_size );
    if ( i4_ret == SMR_OK )
    {
        x_dbg_stmt( "DEST_GRP(%s), ", s_dest_grp_name );
    }

    z_get_size = sizeof( HANDLE_T );
    i4_ret = x_sm_get( h_sess,
                       SM_SESS_GET_TYPE_H_CONNECTION,
                       (VOID*)&h_conn,
                       &z_get_size );
    if ( i4_ret == SMR_OK )
    {
        x_dbg_stmt( "CONN(0x%.8X)\r\n", (UINT32)h_conn );
    }

    z_get_size = sizeof(SIZE_T);
    i4_ret = x_sm_get( h_sess,
                       SM_SESS_GET_TYPE_STRM_NUM,
                       (VOID*)&z_num_strm,
                       &z_get_size );
    if ( i4_ret == SMR_OK && z_num_strm > 0 )
    {
        x_dbg_stmt( "\tNumber of stream: %u\r\n", z_num_strm );

        ph_strm_tbl = x_mem_alloc( sizeof(HANDLE_T) * z_num_strm );
        if ( !ph_strm_tbl )
        {
            x_dbg_stmt( "Error! Not enought memory!\r\n" );
            return 0;
        }

        z_get_size = sizeof(HANDLE_T) * z_num_strm;
        i4_ret = x_sm_get( h_sess,
                           SM_SESS_GET_TYPE_STRM_HANDLES,
                           (VOID*)ph_strm_tbl,
                           &z_get_size );
        if ( i4_ret != SMR_OK )
        {
            x_mem_free( ph_strm_tbl );
            x_dbg_stmt( "Error! x_sm_get(SM_SESS_GET_TYPE_STRM_HANDLES) fail, return %d !\r\n",
                        i4_ret );
            return 0;
        }

        for ( z_i = 0; z_i < z_num_strm; ++z_i )
        {
            x_dbg_stmt( "\t#%2u ", z_i);
            i4_ret = _sm_cli_dump_strm( ph_strm_tbl[z_i] );
            if ( i4_ret != 0 )
            {
                x_dbg_stmt( "Error! _sm_cli_dump_strm(0x%.8X) fail, return %d !\r\n",
                            (UINT32)ph_strm_tbl[z_i], i4_ret );
            }
        }
    }

    x_mem_free( ph_strm_tbl );

    return 0;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_cli_set_ntfy_trace
 *
 * Description: usage: nt y|n
 * Inputs:  -
 * Outputs: -
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sm_cli_set_ntfy_trace (INT32 i4_argc, const CHAR** pps_argv)
{
    if (i4_argc > 1) {
        if (x_strcmp("y", pps_argv[1]) == 0 || x_strcmp("Y", pps_argv[1]) == 0) {
            b_g_sm_nfy_trace = TRUE;
            x_strcpy(s_g_sm_nfy_trace_name, "all");
            if (i4_argc > 2)
            {
                if (x_strcmp("vs", pps_argv[2]) == 0) {
                    x_strcpy(s_g_sm_nfy_trace_name, "VS");
                }
                else if (x_strcmp("as", pps_argv[2]) == 0) {
                    x_strcpy(s_g_sm_nfy_trace_name, "AS");
                }
                else if (x_strcmp("bypass", pps_argv[2]) == 0) {
                    x_strcpy(s_g_sm_nfy_trace_name, "BYPASS");
                }
                else if (x_strcmp("gss", pps_argv[2]) == 0) {
                    x_strcpy(s_g_sm_nfy_trace_name, "GSS");
                }
            }
        } else if (x_strcmp("n", pps_argv[1]) == 0 || x_strcmp("N", pps_argv[1]) == 0) {
            b_g_sm_nfy_trace = FALSE;
        } else {
            x_dbg_stmt("sm.nt [ctrl] [sm_name]\n\r");
            x_dbg_stmt("  [ctrl]: y or n\n\r");
            x_dbg_stmt("  [sm_name]: vs, bypass, as, gss, \n\r");
            x_dbg_stmt("              (only valid when [ctrl] is enabled)\n\r");
            return CLIR_OK;
        }
        x_dbg_stmt("Stream Manager notification trace is %s ", (b_g_sm_nfy_trace ? "enabled" : "disabled"));
        if (b_g_sm_nfy_trace)
        {
            x_dbg_stmt("for %s\n\r", s_g_sm_nfy_trace_name);
        }
        else
        {
            x_dbg_stmt("\n\r");
        }
    } else {
        x_dbg_stmt("sm.nt [ctrl] [sm_name]\n\r");
        x_dbg_stmt("  [ctrl]: y or n\n\r");
        x_dbg_stmt("  [sm_name]: vs, bypass, as, gss, \n\r");
        x_dbg_stmt("              (only valid when [ctrl] is enabled)\n\r");
        return CLIR_OK;
    }

    return CLIR_OK;
}

static INT32 _sm_cli_dump_resource(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    INT32               i4_ret;
    SIZE_T              z_i = 0;
    SIZE_T              z_num_sess = 0, z_get_size;
    HANDLE_T*           ph_sess_tbl = NULL;

    sm_sess_tbl_lock();

    z_num_sess = sm_sess_tbl_get_size();

    x_dbg_stmt( ">>>>>>>>\r\n" );
    x_dbg_stmt( "Number of session: %u\r\n", z_num_sess );

    if ( z_num_sess > 0 )
    {
        ph_sess_tbl = x_mem_alloc( z_num_sess * sizeof(HANDLE_T) );
        if ( !ph_sess_tbl )
        {
            sm_sess_tbl_unlock();
            x_dbg_stmt( "Error! Not enought memory!\r\n" );
            return 0;
        }

        z_get_size = z_num_sess * sizeof(HANDLE_T);
        i4_ret = sm_sess_tbl_get_sess_tbl( ph_sess_tbl, &z_get_size );
        if ( i4_ret != SMR_OK )
        {
            x_mem_free( ph_sess_tbl );
            sm_sess_tbl_unlock();
            x_dbg_stmt( "Error! sm_sess_tbl_get_sess_tbl() fail, return %d !\r\n",
                        i4_ret );
            return 0;
        }

        for ( z_i = 0; z_i < z_num_sess; ++z_i )
        {
            x_dbg_stmt( "#%2u ", z_i);

            i4_ret = _sm_cli_dump_sess( ph_sess_tbl[z_i] );
            if ( i4_ret != 0 )
            {
                x_dbg_stmt( "Error! _sm_cli_dump_sess(0x%.8X) fail return %d!\r\n",
                            (UINT32)ph_sess_tbl[z_i], i4_ret );
            }
        }
    }

    x_dbg_stmt( "<<<<<<<<\r\n" );

    if ( ph_sess_tbl )
    {
        x_mem_free( ph_sess_tbl );
    }
    sm_sess_tbl_unlock();

    return 0;
}

#ifdef MW_TV_AV_BYPASS_SUPPORT
static INT32 _sm_cli_strm_nfy_fct(
                    HANDLE_T                    h_stream,
                    SM_COND_T                   e_nfy_cond,
                    VOID*                       pv_nfy_tag,
                    SM_EVN_TYPE_T               e_event,
                    UINT32                      ui4_data
                    )
{
    x_dbg_stmt( "_sm_cli_strm_nfy_fct( %u, %u, %u, %u, %u )\r\n",
                (UINT32)h_stream, (UINT32)e_nfy_cond, (UINT32)pv_nfy_tag, (UINT32)e_event, ui4_data );
    x_dbg_stmt( "notify event type is 0x%.8X\r\n", (UINT32)e_event );

    switch(e_nfy_cond) {
    case SM_COND_OPENED:
        x_dbg_stmt( "SM_COND_OPENED.\r\n" );
        break;
    case SM_COND_STARTED:
        x_dbg_stmt( "SM_COND_STARTED.\r\n" );
        break;
    case SM_COND_CLOSED:
        x_dbg_stmt( "SM_COND_CLOSED.\r\n" );
        break;
    case SM_COND_ERROR:
        x_dbg_stmt( "SM_COND_ERROR.\r\n" );
        break;
    default:
        break;
    }

    return SMR_OK;
}

static INT32 _sm_cli_open_bypass_hdlr(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    SM_COMMAND_T        t_sm_cmd[6];
    INT32               i4_ret;
    STREAM_COMP_ID_T    t_strm_comp_id;
    BOOL                b_auto_play = FALSE;
    HANDLE_T            h_sess = NULL_HANDLE;
    HANDLE_T            h_stream = NULL_HANDLE;
    SIZE_T              z_i = 0, z_get_size = 0;
    SIZE_T              z_num_strm = 0;
    HANDLE_T*           ph_strm_tbl = NULL;
    UINT8               ui1_idx = 0;
    BSH_VBIF_SRC_INFO_T t_vbif_src_info;

    if ( i4_argc == 2 )
    {
        h_sess = (HANDLE_T)_atoi(pps_argv[1]);
    }
    else if ( i4_argc == 4 )
    {
        INT32 i4_arg;

        h_sess = (HANDLE_T)_atoi(pps_argv[1]);

        i4_arg = _atoi(pps_argv[2]);
        if (i4_arg == 0)
        {
            t_vbif_src_info.e_src_type = BSH_VBIF_SRC_TYPE_UNKNOWN;
        }
        else if (i4_arg == 1)
        {
            t_vbif_src_info.e_src_type = BSH_VBIF_SRC_TYPE_DIG_VBI_ES;
        }
        else if (i4_arg == 2)
        {
            t_vbif_src_info.e_src_type = BSH_VBIF_SRC_TYPE_DIG_TTX_ES;
        }
        else
        {
            return CLIR_INV_ARG;
        }

        t_vbif_src_info.ui2_pid = (UINT16)_atoi(pps_argv[3]);
    }
    else
    {
        x_dbg_stmt("Usage: bypass_open [session handle] [vbif source type] [vbif source pid]\r\n");
        return CLIR_INV_ARG;
    }

    z_get_size = sizeof(SIZE_T);
    i4_ret = x_sm_get( h_sess,
                       SM_SESS_GET_TYPE_STRM_NUM,
                       (VOID*)&z_num_strm,
                       &z_get_size );
    if ( i4_ret == SMR_OK && z_num_strm > 0 )
    {
        ph_strm_tbl = x_mem_alloc( sizeof(HANDLE_T) * z_num_strm );
        if ( !ph_strm_tbl )
        {
            x_dbg_stmt( "Error! not enought memory!\r\n" );
            return 0;
        }

        z_get_size = sizeof(HANDLE_T) * z_num_strm;
        i4_ret = x_sm_get( h_sess,
                           SM_SESS_GET_TYPE_STRM_HANDLES,
                           (VOID*)ph_strm_tbl,
                           &z_get_size );
        if ( i4_ret != SMR_OK )
        {
            x_mem_free( ph_strm_tbl );
            x_dbg_stmt( "Error! x_sm_get(SM_SESS_GET_TYPE_STRM_HANDLES) fail return %d!\r\n",
                        i4_ret );
            return 0;
        }

        for ( z_i = 0; z_i < z_num_strm; ++z_i )
        {
            z_get_size = sizeof(STREAM_COMP_ID_T);
            i4_ret = x_sm_get( ph_strm_tbl[z_i],
                               SM_STRM_GET_TYPE_ID,
                               (VOID*)&t_strm_comp_id,
                               &z_get_size );
            if ( i4_ret != SMR_OK )
            {
                x_dbg_stmt( "Error! x_sm_get(SM_STRM_GET_TYPE_ID) fail return %d\r\n",
                            i4_ret );
                x_mem_free( ph_strm_tbl );
                return 0;
            }
            if (t_strm_comp_id.e_type == ST_VIDEO)
            {
                break;
            }
        }
        x_mem_free( ph_strm_tbl );
        if (t_strm_comp_id.e_type != ST_VIDEO)
        {
            x_dbg_stmt( "Error! video stream handler doesn't exist!\r\n" );
            return 0;
        }
    }

    t_strm_comp_id.e_type = ST_BYPASS;

    t_sm_cmd[ui1_idx].e_code      = SM_BYPASS_CMD_CODE_SET_DEST_GRP_NAME;
    t_sm_cmd[ui1_idx++].u.ps_text = SN_PRES_MONITOR_BYPASS;
    if ( i4_argc == 4 )
    {
        t_sm_cmd[ui1_idx].e_code      = SM_BYPASS_CMD_CODE_SET_VBIF_SRC;
        t_sm_cmd[ui1_idx++].u.pv_data = &t_vbif_src_info;
    }
    t_sm_cmd[ui1_idx].e_code      = SM_CMD_CODE_END;

    i4_ret = x_sm_open_stream(
                h_sess,
                &t_strm_comp_id,
                t_sm_cmd,
                _sm_cli_strm_nfy_fct,
                (VOID*)NULL,
                b_auto_play,
                &h_stream
                );
    if (i4_ret != SMR_OK && i4_ret != SMR_PENDING) {
        x_dbg_stmt( "Error! x_sm_open_stream fail return %d\r\n", i4_ret );
        return 0;
    }

    x_dbg_stmt( "Bypass Stream Handle (%u) open successfully!",
                (UINT32)h_stream );
    return 0;
}

static INT32 _sm_cli_close_bypass_hdlr(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    HANDLE_T h_stream = NULL_HANDLE;
    INT32    i4_ret;

    if ( i4_argc == 2 )
    {
        h_stream = (HANDLE_T)_atoi(pps_argv[1]);
        i4_ret = x_sm_close_stream( h_stream );
        if (i4_ret != SMR_OK && i4_ret != SMR_PENDING) {
            x_dbg_stmt( "Error! x_sm_close_stream fail return %d\r\n", i4_ret );
            return 0;
        }
    }
    else
    {
        x_dbg_stmt("Usage: bypass_close [bypass handle]\r\n");
        return CLIR_INV_ARG;
    }

    x_dbg_stmt( "Bypass Stream Handle (%u) close successfully!", (UINT32)h_stream );
    return 0;
}

static INT32 _sm_cli_play_bypass_hdlr(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    HANDLE_T h_stream = NULL_HANDLE;
    INT32    i4_ret;

    if ( i4_argc == 2 )
    {
        h_stream = (HANDLE_T)_atoi(pps_argv[1]);
        i4_ret = x_sm_play_stream( h_stream );
        if (i4_ret != SMR_OK && i4_ret != SMR_PENDING) {
            x_dbg_stmt( "Error! x_sm_play_stream fail return %d\r\n", i4_ret );
            return 0;
        }
    }
    else
    {
        x_dbg_stmt("Usage: bypass_play [bypass handle]\r\n");
        return CLIR_INV_ARG;
    }

    x_dbg_stmt( "Bypass Stream Handle (%u) play successfully!", (UINT32)h_stream );
    return 0;
}

static INT32 _sm_cli_stop_bypass_hdlr(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    HANDLE_T h_stream = NULL_HANDLE;
    INT32    i4_ret;

    if ( i4_argc == 2 )
    {
        h_stream = (HANDLE_T)_atoi(pps_argv[1]);
        i4_ret = x_sm_stop_stream( h_stream );
        if (i4_ret != SMR_OK && i4_ret != SMR_PENDING) {
            x_dbg_stmt( "Error! x_sm_stop_stream fail return %d\r\n", i4_ret );
            return 0;
        }
    }
    else
    {
        x_dbg_stmt("Usage: bypass_stop [bypass handle]\r\n");
        return CLIR_INV_ARG;
    }

    x_dbg_stmt( "Bypass Stream Handle (%u) stop successfully!", (UINT32)h_stream );
    return 0;
}

static INT32 _sm_cli_bypass_mode(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    SM_COMMAND_T            at_scc_cmds [32];

    if ( !pps_argv[1] )
    {
        x_dbg_stmt( "Command syntax error!!\r\n" );
        x_dbg_stmt( "ech  <0-255>\r\n" );
        return -1;
    }

    at_scc_cmds [0].e_code     = SCC_CMD_CODE_GRP_TYPE;
    at_scc_cmds [0].u.ui4_data = SCC_CMD_TYPE_DISP;

    at_scc_cmds [1].e_code    = SCC_CMD_CODE_NAME;
    at_scc_cmds [1].u.ps_text = SN_PRES_TV_BYPASS;

    at_scc_cmds [2].e_code     = SCC_CMD_CODE_DISP_BYPASS_MODE;
    at_scc_cmds [2].u.ui4_data = (UINT32) x_strtoll( pps_argv[1], NULL, 10);

    at_scc_cmds [3].e_code = SM_CMD_CODE_END;

    if (x_scc_comp_grp_set (&(at_scc_cmds [0])) != SMR_OK)
    {
        return -1;
    }

    x_dbg_stmt( ">>>>>>>>\r\n" );
    x_dbg_stmt( "Bypass Mode is set to %u\r\n", at_scc_cmds [2].u.ui4_data );
    x_dbg_stmt( "<<<<<<<<\r\n" );

    return 0;
}
#endif /* MW_TV_AV_BYPASS_SUPPORT */

static HANDLE_T h_g_scc        = NULL_HANDLE;

static INT32 _sm_cli_plane_mute(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    SCC_VID_SUPER_BLANK_T t_blank;

    t_blank.t_bg_color.ui1_red = 128;
    t_blank.t_bg_color.ui1_green = 128;
    t_blank.t_bg_color.ui1_blue = 128;
    t_blank.t_disp_region.ui4_x = 0;
    t_blank.t_disp_region.ui4_y = 0;
    t_blank.t_disp_region.ui4_width = 1000;
    t_blank.t_disp_region.ui4_height = 1000;

    if ( i4_argc == 2 )
    {
        t_blank.b_enable = (_atoi(pps_argv[1]) == 0) ? FALSE : TRUE;
    }
    else if ( i4_argc == 5 )
    {
        t_blank.b_enable = (_atoi(pps_argv[1]) == 0) ? FALSE : TRUE;
        t_blank.t_bg_color.ui1_red = (UINT8)_atoi(pps_argv[2]);
        t_blank.t_bg_color.ui1_green = (UINT8)_atoi(pps_argv[3]);
        t_blank.t_bg_color.ui1_blue = (UINT8)_atoi(pps_argv[4]);
    }
    else if ( i4_argc == 9 )
    {
        t_blank.b_enable = (_atoi(pps_argv[1]) == 0) ? FALSE : TRUE;
        t_blank.t_bg_color.ui1_red = (UINT8)_atoi(pps_argv[2]);
        t_blank.t_bg_color.ui1_green = (UINT8)_atoi(pps_argv[3]);
        t_blank.t_bg_color.ui1_blue = (UINT8)_atoi(pps_argv[4]);
        t_blank.t_disp_region.ui4_x = (UINT32)_atoi(pps_argv[5]);
        t_blank.t_disp_region.ui4_y = (UINT32)_atoi(pps_argv[6]);
        t_blank.t_disp_region.ui4_width = (UINT32)_atoi(pps_argv[7]);
        t_blank.t_disp_region.ui4_height = (UINT32)_atoi(pps_argv[8]);
    }
    else
    {
        x_dbg_stmt( "Command syntax error!!\r\n" );
        x_dbg_stmt( "mute <0/1> <red> <green> <blue>\r\n" );
        return -1;
    }

    if ( t_blank.b_enable )
    {
        SM_COMMAND_T at_scc_cmds [32];

        if ( h_g_scc != NULL_HANDLE )
        {
            t_blank.b_enable = FALSE;
            if ( x_scc_vid_set_super_blank(h_g_scc, &t_blank) != SMR_OK )
            {
                x_dbg_stmt( "x_scc_vid_set_super_blank() error!\r\n" );
                x_scc_comp_close( h_g_scc );
                h_g_scc = NULL_HANDLE;
                return -1;
            }
            t_blank.b_enable = TRUE;
            x_scc_comp_close( h_g_scc );
            h_g_scc = NULL_HANDLE;
        }

        at_scc_cmds [0].e_code     = SCC_CMD_CODE_GRP_TYPE;
        at_scc_cmds [0].u.ui4_data = SCC_CMD_TYPE_VIDEO;

        at_scc_cmds [1].e_code     = SCC_CMD_CODE_NAME;
        at_scc_cmds [1].u.ps_text  = SN_PRES_MAIN_DISPLAY;

        at_scc_cmds [2].e_code     = SM_CMD_CODE_END;
        if (x_scc_comp_open (&(at_scc_cmds [0]), &h_g_scc) != SMR_OK)
        {
            x_dbg_stmt( "x_scc_comp_open() error!\r\n" );
            return -1;
        }
    }

    if ( x_scc_vid_set_super_blank(h_g_scc, &t_blank) != SMR_OK )
    {
        x_dbg_stmt( "x_scc_vid_set_super_blank() error!\r\n" );
        x_scc_comp_close( h_g_scc );
        h_g_scc = NULL_HANDLE;
        return -1;
    }

    if ( !t_blank.b_enable )
    {
        x_scc_comp_close( h_g_scc );
        h_g_scc = NULL_HANDLE;
    }

    x_dbg_stmt( ">>>>>>>>\r\n" );
    if ( t_blank.b_enable )
    {
        x_dbg_stmt( "Mute video plane\r\n" );
    }
    else
    {
        x_dbg_stmt( "Unmute video plane\r\n" );
    }
    x_dbg_stmt( "<<<<<<<<\r\n" );

    return 0;
}

static INT32 _sm_cli_vp_enhance(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    SM_COMMAND_T            at_scc_cmds [32];

    if ( !pps_argv[1] )
    {
        x_dbg_stmt( "Command syntax error!!\r\n" );
        x_dbg_stmt( "ech  <0-255>\r\n" );
        return -1;
    }

    at_scc_cmds [0].e_code     = SCC_CMD_CODE_GRP_TYPE;
    at_scc_cmds [0].u.ui4_data = SCC_CMD_TYPE_VIDEO;

    at_scc_cmds [1].e_code    = SCC_CMD_CODE_NAME;
    at_scc_cmds [1].u.ps_text = SN_PRES_MAIN_DISPLAY;

    at_scc_cmds [2].e_code     = SCC_CMD_CODE_VIDEO_ENHANCE;
    at_scc_cmds [2].u.ui1_data = (UINT32) x_strtoll( pps_argv[1], NULL, 10);

    at_scc_cmds [3].e_code = SM_CMD_CODE_END;

    if (x_scc_comp_grp_set (&(at_scc_cmds [0])) != SMR_OK)
    {
        return -1;
    }

    x_dbg_stmt( ">>>>>>>>\r\n" );
    x_dbg_stmt( "Enhance is set to %u\r\n", at_scc_cmds [2].u.ui1_data );
    x_dbg_stmt( "<<<<<<<<\r\n" );

    return 0;
}

static INT32 _sm_cli_vp_blur(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    SM_COMMAND_T            at_scc_cmds [32];

    if ( !pps_argv[1] )
    {
        x_dbg_stmt( "Command syntax error!!\r\n" );
        x_dbg_stmt( "blr  <0-255>\r\n" );
        return -1;
    }

    at_scc_cmds [0].e_code     = SCC_CMD_CODE_GRP_TYPE;
    at_scc_cmds [0].u.ui4_data = SCC_CMD_TYPE_VIDEO;

    at_scc_cmds [1].e_code    = SCC_CMD_CODE_NAME;
    at_scc_cmds [1].u.ps_text = SN_PRES_MAIN_DISPLAY;

    at_scc_cmds [2].e_code     = SCC_CMD_CODE_VIDEO_BLUR;
    at_scc_cmds [2].u.ui1_data = (UINT32) x_strtoll( pps_argv[1], NULL, 10);

    at_scc_cmds [3].e_code = SM_CMD_CODE_END;

    if (x_scc_comp_grp_set (&(at_scc_cmds [0])) != SMR_OK)
    {
        return -1;
    }

    x_dbg_stmt( ">>>>>>>>\r\n" );
    x_dbg_stmt( "Blur is set to %u\r\n", at_scc_cmds [2].u.ui1_data );
    x_dbg_stmt( "<<<<<<<<\r\n" );

    return 0;
}

static INT32 _show_vp_overscan( VOID )
{
    SM_COMMAND_T                at_scc_cmds [32];
    SCC_VID_OVER_SCAN_CLIPPER_T t_vid_clipper;
    HANDLE_T                    h_scc = NULL_HANDLE;

    at_scc_cmds [0].e_code     = SCC_CMD_CODE_GRP_TYPE;
    at_scc_cmds [0].u.ui4_data = SCC_CMD_TYPE_VIDEO;

    at_scc_cmds [1].e_code     = SCC_CMD_CODE_NAME;
    at_scc_cmds [1].u.ps_text  = SN_PRES_MAIN_DISPLAY;

    at_scc_cmds [2].e_code     = SM_CMD_CODE_END;

    if (x_scc_comp_open (&(at_scc_cmds [0]), &h_scc) != SMR_OK)
    {
        x_dbg_stmt( "x_scc_comp_open() error!\r\n" );
        return -1;
    }

    if ( x_scc_vid_get_overscan_clipper(h_scc, &t_vid_clipper) != SMR_OK )
    {
        x_dbg_stmt( "x_scc_vid_get_overscan_clipper() error!\r\n" );
        return -1;
    }

    x_scc_comp_close( h_scc );

    x_dbg_stmt( ">>>>>>>>\r\n" );
    x_dbg_stmt( "MAIN overscan is (T,B,L,R)=(%u,%u,%u,%u)\r\n",
                t_vid_clipper.ui4_top,
                t_vid_clipper.ui4_bottom,
                t_vid_clipper.ui4_left,
                t_vid_clipper.ui4_right );
    x_dbg_stmt( "<<<<<<<<\r\n" );


    /* SUB */
    at_scc_cmds [0].e_code     = SCC_CMD_CODE_GRP_TYPE;
    at_scc_cmds [0].u.ui4_data = SCC_CMD_TYPE_VIDEO;

    at_scc_cmds [1].e_code     = SCC_CMD_CODE_NAME;
    at_scc_cmds [1].u.ps_text  = SN_PRES_SUB_DISPLAY;

    at_scc_cmds [2].e_code     = SM_CMD_CODE_END;

    if (x_scc_comp_open (&(at_scc_cmds [0]), &h_scc) != SMR_OK)
    {
        x_dbg_stmt( "x_scc_comp_open() error!\r\n" );
        return -1;
    }

    if ( x_scc_vid_get_overscan_clipper(h_scc, &t_vid_clipper) != SMR_OK )
    {
        x_dbg_stmt( "x_scc_vid_get_overscan_clipper() error!\r\n" );
        return -1;
    }

    x_scc_comp_close( h_scc );

    x_dbg_stmt( ">>>>>>>>\r\n" );
    x_dbg_stmt( "SUB overscan is (T,B,L,R)=(%u,%u,%u,%u)\r\n",
                t_vid_clipper.ui4_top,
                t_vid_clipper.ui4_bottom,
                t_vid_clipper.ui4_left,
                t_vid_clipper.ui4_right );
    x_dbg_stmt( "<<<<<<<<\r\n" );

    return 0;
}

static INT32 _sm_cli_vp_overscan(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    SM_COMMAND_T                at_scc_cmds [32];
    SCC_VID_OVER_SCAN_CLIPPER_T t_vid_clipper;

    if ( i4_argc == 1 )
    {
        _show_vp_overscan();
    }

    if ( i4_argc != 6 ||
         ( !pps_argv[1] || (pps_argv[1][0] != 'm' && pps_argv[1][0] != 's') ) ||
         !pps_argv[2] || !pps_argv[3] || !pps_argv[4] || !pps_argv[5] )
    {
        x_dbg_stmt( "Command syntax is,\r\n" );
        x_dbg_stmt( "\tosc  <m|s> top bottom left right\r\n\r\n" );
        x_dbg_stmt( " <m|s>   m:MAIN, s:SUB\r\n" );
        x_dbg_stmt( " top     Top overscan in 0.1 percent unit\r\n" );
        x_dbg_stmt( " bottom  Bottom overscan in 0.1 percent unit\r\n" );
        x_dbg_stmt( " left    Left overscan in 0.1 percent unit\r\n" );
        x_dbg_stmt( " right   Right overscan in 0.1 percent unit\r\n" );
        return -1;
    }

    at_scc_cmds [0].e_code     = SCC_CMD_CODE_GRP_TYPE;
    at_scc_cmds [0].u.ui4_data = SCC_CMD_TYPE_VIDEO;

    at_scc_cmds [1].e_code     = SCC_CMD_CODE_NAME;
    at_scc_cmds [1].u.ps_text  = ( (pps_argv[1][0]=='m')?
                                   SN_PRES_MAIN_DISPLAY:
                                   SN_PRES_SUB_DISPLAY );

    t_vid_clipper.ui4_top      = (UINT32) x_strtoll( pps_argv[2], NULL, 10);
    t_vid_clipper.ui4_bottom   = (UINT32) x_strtoll( pps_argv[3], NULL, 10);
    t_vid_clipper.ui4_left     = (UINT32) x_strtoll( pps_argv[4], NULL, 10);
    t_vid_clipper.ui4_right    = (UINT32) x_strtoll( pps_argv[5], NULL, 10);
    at_scc_cmds [2].e_code     = SCC_CMD_CODE_VIDEO_OVERSCAN_CLIPPER;
    at_scc_cmds [2].u.pv_data  = (VOID*) &t_vid_clipper;

    at_scc_cmds [3].e_code     = SM_CMD_CODE_END;

    if (x_scc_comp_grp_set (&(at_scc_cmds [0])) != SMR_OK)
    {
        x_dbg_stmt( "x_scc_comp_grp_set() error!\r\n" );
        return -1;
    }

    x_dbg_stmt( ">>>>>>>>\r\n" );
    x_dbg_stmt( "%s overscan is set to (T,B,L,R)=(%u,%u,%u,%u)\r\n",
                ( (pps_argv[1][0]=='m')?"MAIN":"SUB" ),
                t_vid_clipper.ui4_top,
                t_vid_clipper.ui4_bottom,
                t_vid_clipper.ui4_left,
                t_vid_clipper.ui4_right );
    x_dbg_stmt( "<<<<<<<<\r\n" );

    return 0;
}

static INT32 _sm_cli_disp_reslution(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    SCC_DISP_RESOLUTION_T   t_reslution;
    SM_COMMAND_T            at_scc_cmds [32];

    if ( !pps_argv[1] || !pps_argv[2] )
    {
        x_dbg_stmt( "Command syntax error!!\r\n" );
        x_dbg_stmt( "rsl  [width] [height] [is_prograsive]\r\n" );
        x_dbg_stmt( "\twidth         720, 1280, or 1920\r\n" );
        x_dbg_stmt( "\theight        480, 720, or 1080\r\n" );
        x_dbg_stmt( "\tis_prograsive  P(p) or I(i)\r\n" );
        return -1;
    }

    t_reslution.ui2_width = x_strtoll( pps_argv[1], NULL, 10);
    t_reslution.ui2_height = x_strtoll( pps_argv[2], NULL, 10);
    if ( !pps_argv[3] )
    {
        t_reslution.b_is_progressive = FALSE;
    }
    else
    {
        if ( pps_argv[3][0]=='p' || pps_argv[3][0]=='P' )
        {
            t_reslution.b_is_progressive = TRUE;
        }
        else if ( pps_argv[3][0]=='i' || pps_argv[3][0]=='I' )
        {
            t_reslution.b_is_progressive = FALSE;
        }
        else
        {
            x_dbg_stmt( "Invalid command!\r\n" );
            return -1;
        }
    }

    at_scc_cmds [0].e_code     = SCC_CMD_CODE_GRP_TYPE;
    at_scc_cmds [0].u.ui4_data = SCC_CMD_TYPE_DISP;

    at_scc_cmds [1].e_code    = SCC_CMD_CODE_NAME;
    at_scc_cmds [1].u.ps_text = SN_PRES_MAIN_DISPLAY;

    at_scc_cmds [2].e_code     = SCC_CMD_CODE_DISP_RESOLUTION;
    at_scc_cmds [2].u.pv_data = &t_reslution;

    at_scc_cmds [3].e_code = SM_CMD_CODE_END;

    if (x_scc_comp_grp_set (&(at_scc_cmds [0])) != SMR_OK)
    {
        x_dbg_stmt( ">>>>>>>>\r\n" );
        x_dbg_stmt( "Set display resolution fail!\r\n" );
        x_dbg_stmt( "<<<<<<<<\r\n" );
        return -1;
    }

    x_dbg_stmt( ">>>>>>>>\r\n" );
    x_dbg_stmt( "Display resolution is set to %u x %u %s\r\n",
                t_reslution.ui2_width,
                t_reslution.ui2_height,
                (t_reslution.b_is_progressive)?"P":"I" );
    x_dbg_stmt( "<<<<<<<<\r\n" );

    return 0;
}

static INT32 _sm_cli_get_slctr_max_msg_num(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    extern UINT16 gui2_slctr_max_num_msgs;

    x_dbg_stmt( ">>>>>>>>\r\n" );
    x_dbg_stmt( "Selector MAX message number is %u\r\n", gui2_slctr_max_num_msgs );
    x_dbg_stmt( "<<<<<<<<\r\n" );

    return 0;
}

static INT32 _sm_cli_dump_scdb(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    INT32               i4_ret;
    SIZE_T              z_i = 0;
    SIZE_T              z_num_sess = 0, z_get_size;
    HANDLE_T*           ph_sess_tbl = NULL;

    sm_sess_tbl_lock();

    z_num_sess = sm_sess_tbl_get_size();

    x_dbg_stmt( ">>>>>>>>\r\n" );
    x_dbg_stmt( "Number of session: %u\r\n", z_num_sess );

    if ( z_num_sess > 0 )
    {
        ph_sess_tbl = x_mem_alloc( z_num_sess * sizeof(HANDLE_T) );
        if ( !ph_sess_tbl )
        {
            sm_sess_tbl_unlock();
            x_dbg_stmt( "Error! not enought memory!\r\n" );
            return 0;
        }

        z_get_size = z_num_sess * sizeof(HANDLE_T);
        i4_ret = sm_sess_tbl_get_sess_tbl( ph_sess_tbl, &z_get_size );
        if ( i4_ret != SMR_OK )
        {
            x_mem_free( ph_sess_tbl );
            sm_sess_tbl_unlock();
            x_dbg_stmt( "Error! sm_sess_tbl_get_sess_tbl() fail return %d!\r\n",
                        i4_ret );
            return 0;
        }

        for ( z_i = 0; z_i < z_num_sess; ++z_i )
        {
            SM_SESSION_T*           pt_sess_obj = NULL;
            SM_SESS_HANDLER_T*      pt_sess_hdlr_obj = NULL;

            x_dbg_stmt( "Session = 0x%.8X, ", (UINT32)ph_sess_tbl[z_i] );

            pt_sess_hdlr_obj = sm_get_sess_hdlr_obj(
                ph_sess_tbl[z_i],
                (VOID**)&pt_sess_obj );
            if ( !pt_sess_hdlr_obj || !pt_sess_obj )
            {
                return SMR_INV_HANDLE;
            }

            if ( pt_sess_obj->h_scdb != NULL_HANDLE )
            {
                _dump_scdb( pt_sess_obj->h_scdb );
            }
        }
    }

    x_dbg_stmt( "\r\n<<<<<<<<\r\n" );

    if ( ph_sess_tbl )
    {
        x_mem_free( ph_sess_tbl );
    }
    sm_sess_tbl_unlock();

    return 0;
}

static INT32 _sm_obsvr_test_sess_nfy(
    HANDLE_T          h_obsvr,
    SM_SESS_TYPE_T    e_sess_type,
    HANDLE_T          h_sess,
    SM_COND_T         e_cond,
    VOID*             pv_tag,
    SM_EVN_TYPE_T     e_event,
    UINT32            ui4_data )
{
    x_dbg_stmt( "[SM] sm_obsvr_test_sess_nfy(\r\n"
                "\th_obsvr     = 0x%.8x\r\n"
                "\te_sess_type = 0x%.8x\r\n"
                "\th_sess      = 0x%.8x\r\n"
                "\te_cond      = 0x%.8x\r\n"
                "\thpv_tag     = 0x%.8x\r\n"
                "\te_event     = 0x%.8x\r\n"
                "\tui4_data    = 0x%.8x\r\n",
                (UINT32)h_obsvr,
                (UINT32)e_sess_type,
                (UINT32)h_sess,
                (UINT32)e_cond,
                (UINT32)pv_tag,
                (UINT32)e_event,
                (UINT32)ui4_data );

    return SMR_OK;
}

static INT32 _sm_obsvr_test_strm_nfy(
    HANDLE_T          h_obsvr,
    STREAM_TYPE_T     e_strm_type,
    HANDLE_T          h_steam,
    SM_COND_T         e_cond,
    VOID*             pv_tag,
    SM_EVN_TYPE_T     e_event,
    UINT32            ui4_data )
{
    x_dbg_stmt( "[SM] sm_obsvr_test_strm_nfy(\r\n"
                "\th_obsvr     = 0x%.8x\r\n"
                "\te_strm_type = 0x%.8x\r\n"
                "\th_steam     = 0x%.8x\r\n"
                "\te_cond      = 0x%.8x\r\n"
                "\thpv_tag     = 0x%.8x\r\n"
                "\te_event     = 0x%.8x\r\n"
                "\tui4_data    = 0x%.8x\r\n",
                (UINT32)h_obsvr,
                (UINT32)e_strm_type,
                (UINT32)h_steam,
                (UINT32)e_cond,
                (UINT32)pv_tag,
                (UINT32)e_event,
                (UINT32)ui4_data );

    return SMR_OK;
}

INT32 _sm_cli_obsvr_test(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    INT32               i4_ret;
    SM_OBSVR_FILTER_T   t_fltr;
    HANDLE_T            h_obsvr;
    UINT32              aui4_events[2] = {SM_OBSVR_EVN_FLTER_ANY, SM_EVN_IGNORE};

    t_fltr.ui4_obj_type_fltr = SM_OBSVR_OBJ_TYPE_FILTER_ANY;
    t_fltr.ui4_sess_type_fltr = SM_OBSVR_SESS_TYPE_FILTER_ANY;
    t_fltr.ui4_strm_type_fltr = SM_OBSVR_STRM_TYPE_FILTER_ANY;
    t_fltr.ui4_sess_state_fltr = SM_OBSVR_STATE_FLTER_ANY;
    t_fltr.ui4_strm_state_fltr = SM_OBSVR_STATE_FLTER_ANY;
    t_fltr.pui4_event_fltr = aui4_events;

    i4_ret = x_sm_register_obsvr(
        &t_fltr,
        _sm_obsvr_test_sess_nfy,
        (VOID*)0xadadadad,
        _sm_obsvr_test_strm_nfy,
        (VOID*)0xedededed,
        &h_obsvr );
    if ( i4_ret != SMR_OK )
    {
        x_dbg_stmt( "[SM] x_sm_register_obsvr() return error %d\r\n", i4_ret );
        return 0;
    }

    return 0;
}

static INT32 _sm_cli_scc_api_test(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    SM_COMMAND_T            at_scc_cmds [32];
    
    if ((i4_argc !=5) || ( !pps_argv[1] || !pps_argv[2]|| !pps_argv[3] || !pps_argv[4])) 
    {
        x_dbg_stmt( "Command syntax error!!\r\n" );
        x_dbg_stmt( "Please input group type[0~2] 0:video 1:audio 2:display, group name, command number[48~136] and command data.\r\n" );
        return -1;
    }

    at_scc_cmds [0].e_code     = SCC_CMD_CODE_GRP_TYPE;
    at_scc_cmds [0].u.ui4_data = (UINT32) x_strtoll( pps_argv[1], NULL, 10);;

    at_scc_cmds [1].e_code    = SCC_CMD_CODE_NAME;
    at_scc_cmds [1].u.ps_text = (CHAR*)pps_argv[2];

    at_scc_cmds [2].e_code     = (UINT32) x_strtoll( pps_argv[3], NULL, 10);
    at_scc_cmds [2].u.ui4_data = (UINT32) x_strtoll( pps_argv[4], NULL, 10);

    at_scc_cmds [3].e_code = SM_CMD_CODE_END;

    if (x_scc_comp_grp_set (&(at_scc_cmds [0])) != SMR_OK)
    {
        return -1;
    }
    else
    {
        
        x_dbg_stmt( ">>>>>>>>\r\n" );
        x_dbg_stmt( "Group type= [%d],Group name = [%s],command number = [%d],command data = [0x%.8x].\r\n", at_scc_cmds [0].u.ui4_data,at_scc_cmds [1].u.ps_text,at_scc_cmds [2].e_code,at_scc_cmds [2].u.ui4_data);
        x_dbg_stmt( "scc API test success!\r\n");
        x_dbg_stmt( "<<<<<<<<\r\n" );
    }
    return 0;
}

static HANDLE_T h_g_scc_vid = NULL_HANDLE;
static HANDLE_T h_g_scc_aud = NULL_HANDLE;
static HANDLE_T h_g_scc_disp = NULL_HANDLE;

static INT32 _sm_cli_scc_set_vid_cmd(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    INT32       i4_ret = SMR_OK;
    INT32       i4_i;
    CHAR *      ps_vid_set[20] = {
                "mode(m) n(0~4)",
                
                NULL
                };
 
    if ( i4_argc <3 )
    {
        x_dbg_stmt("Please check the usage:\r\n");  

        for( i4_i = 0; ps_vid_set[i4_i] != NULL && i4_i < 20; i4_i++ )
        {
            x_dbg_stmt("set(s) %s\r\n", ps_vid_set[i4_i]);
        }        
        return SMR_INV_ARG;
    }

    if ( x_strcmp( pps_argv[0],"set" )!= 0 && x_strcmp( pps_argv[0],"s" )!= 0  )
    {
        x_dbg_stmt("Command error! Please use \"set\" or \"s\".\r\n");
        return SMR_INV_COMMAND;
    }

    if ( h_g_scc_vid == NULL_HANDLE )
    {
        SM_COMMAND_T            at_scc_cmds[3];
        
        at_scc_cmds [0].e_code     = SCC_CMD_CODE_GRP_TYPE;
        at_scc_cmds [0].u.ui4_data = SCC_CMD_TYPE_VIDEO;

        at_scc_cmds [1].e_code     = SCC_CMD_CODE_NAME;
        at_scc_cmds [1].u.ps_text  = SN_PRES_MAIN_DISPLAY;

        at_scc_cmds [2].e_code     = SM_CMD_CODE_END;
        
        if ( x_scc_comp_open (&(at_scc_cmds [0]), &h_g_scc_vid) != SMR_OK )
        {
            x_dbg_stmt( "x_scc_comp_open() error!\r\n" );
            return SMR_DRV_COMP_NOT_FOUND;
        }
    }

    x_dbg_stmt("============ Video Set Command Starting ======================\r\n");

    /* set mode */
    if ( x_strcmp(pps_argv[1],"mode") == 0 || x_strcmp(pps_argv[1],"m") == 0)
    {
        SCC_VID_MODE_T    e_mode;
        e_mode = (SCC_VID_MODE_T)_atoi( pps_argv[2]);
        i4_ret = x_scc_vid_set_mode( h_g_scc_vid, e_mode );

        if(i4_ret !=  SMR_OK )
        {
            x_dbg_stmt("set mode (%d) fail!\r\n", e_mode );
            i4_ret = SMR_INTERNAL_ERROR;
        }
        else {
            x_dbg_stmt("set mode (%d) OK!\r\n", e_mode );
        }
    }

    x_dbg_stmt("============ Video Set Command End ============================\r\n");

    if( i4_ret != SMR_OK )
    {
            x_scc_comp_close( h_g_scc_vid );
            h_g_scc_vid = NULL_HANDLE;
    }

    return i4_ret;
}

static INT32 _sm_cli_scc_get_vid_cmd(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    INT32       i4_ret = SMR_OK;
    INT32       i4_i;
    CHAR *      ps_vid_get[20] = {
                
                NULL
        };

    if( i4_argc <2 )
    {
        x_dbg_stmt("Please check the usage:\r\n");  

        for( i4_i = 0; ps_vid_get[i4_i] != NULL && i4_i < 20; i4_i++ )
        {
            x_dbg_stmt("get(g) %s\r\n", ps_vid_get[i4_i] );
        }
        return SMR_INV_ARG;
    }

    if( x_strcmp( pps_argv[0],"get" )!= 0 && x_strcmp( pps_argv[0],"g" )!= 0  )
    {
        x_dbg_stmt("Command error! Pleaseuse \"get\" or \"g\".\r\n");
        return SMR_INV_COMMAND;
    }

    if ( h_g_scc_vid == NULL_HANDLE )
    {
        SM_COMMAND_T            at_scc_cmds[3];
        
        at_scc_cmds [0].e_code     = SCC_CMD_CODE_GRP_TYPE;
        at_scc_cmds [0].u.ui4_data = SCC_CMD_TYPE_VIDEO;

        at_scc_cmds [1].e_code     = SCC_CMD_CODE_NAME;
        at_scc_cmds [1].u.ps_text  = SN_PRES_MAIN_DISPLAY;

        at_scc_cmds [2].e_code     = SM_CMD_CODE_END;
        
        if (x_scc_comp_open( &(at_scc_cmds [0]), &h_g_scc_vid )!= SMR_OK)
        {
            x_dbg_stmt( "x_scc_comp_open() error!\r\n" );
            return SMR_DRV_COMP_NOT_FOUND;
        }
    }
    x_dbg_stmt("============ Video Get Command Starting ======================\r\n");

    /* Add new API test here */

    x_dbg_stmt("============ Video Get Command End ============================\r\n");

    if( i4_ret != SMR_OK )
    {
            x_scc_comp_close( h_g_scc_vid );
            h_g_scc_vid = NULL_HANDLE;
    }

    return i4_ret;
}

static INT32 _sm_cli_scc_set_aud_cmd(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    INT32       i4_ret = SMR_OK;
    INT32       i4_i;
    CHAR *      ps_aud_set[20] = {
                  
                NULL
                };
    if( i4_argc <3 )
    {
        x_dbg_stmt("Please check the usage:\r\n");  

        for( i4_i = 0; ps_aud_set[i4_i] != NULL && i4_i < 20; i4_i++ )
        {
            x_dbg_stmt("set(s) %s\r\n", ps_aud_set[i4_i] );
        }
        return SMR_INV_ARG;
    }

    if( x_strcmp( pps_argv[0], "set" )!= 0 && x_strcmp( pps_argv[0], "s" )!= 0  )
    {
        x_dbg_stmt("Command error! Pleaseuse \"set\" or \"s\".\r\n");
        return SMR_INV_COMMAND;
    }

    if ( h_g_scc_aud == NULL_HANDLE )
    {
        SM_COMMAND_T            at_scc_cmds[3];
        
        at_scc_cmds [0].e_code     = SCC_CMD_CODE_GRP_TYPE;
        at_scc_cmds [0].u.ui4_data = SCC_CMD_TYPE_AUDIO;

        at_scc_cmds [1].e_code     = SCC_CMD_CODE_NAME;
        at_scc_cmds [1].u.ps_text  = SN_PRES_MAIN_DISPLAY;

        at_scc_cmds [2].e_code     = SM_CMD_CODE_END;
        
        if (x_scc_comp_open( &(at_scc_cmds [0]), &h_g_scc_aud )!= SMR_OK)
        {
            x_dbg_stmt( "x_scc_comp_open() error!\r\n" );
            return SMR_DRV_COMP_NOT_FOUND;
        }
    }

    x_dbg_stmt("\r\n============ Audio Set Command Starting ======================\r\n");

    /* Add new API test here */

    x_dbg_stmt("\r\n============ Audio Set Command End ============================\r\n");
    
    if( i4_ret != SMR_OK )
    {
            x_scc_comp_close( h_g_scc_aud );
            h_g_scc_aud = NULL_HANDLE;
    }

    return i4_ret;
}

static INT32 _sm_cli_scc_get_aud_cmd(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    INT32       i4_ret = SMR_OK;
    INT32       i4_i;
    CHAR *      ps_aud_get[20] = {

                NULL
                };

    if( i4_argc <2 )
    {
        x_dbg_stmt("Please check the usage:\r\n");  

        for( i4_i = 0; ps_aud_get[i4_i] != NULL && i4_i < 20; i4_i++ )
        {
            x_dbg_stmt("get(g) %s\r\n", ps_aud_get[i4_i]);
        }
        return SMR_INV_ARG;
    }

    if( x_strcmp( pps_argv[0], "get" )!= 0 && x_strcmp( pps_argv[0], "g" )!= 0  )
    {
        x_dbg_stmt("Command error! Pleaseuse \"get\" or \"g\".\r\n");
        return SMR_INV_COMMAND;
    }

    if ( h_g_scc_aud == NULL_HANDLE )
    {
        SM_COMMAND_T            at_scc_cmds[3];
        
        at_scc_cmds [0].e_code     = SCC_CMD_CODE_GRP_TYPE;
        at_scc_cmds [0].u.ui4_data = SCC_CMD_TYPE_AUDIO;

        at_scc_cmds [1].e_code     = SCC_CMD_CODE_NAME;
        at_scc_cmds [1].u.ps_text  = SN_PRES_MAIN_DISPLAY;

        at_scc_cmds [2].e_code     = SM_CMD_CODE_END;
        
        if (x_scc_comp_open( &(at_scc_cmds [0]), &h_g_scc_aud )!= SMR_OK)
        {
            x_dbg_stmt("x_scc_comp_open() error!\r\n");
            return SMR_DRV_COMP_NOT_FOUND;
        }
    }

    x_dbg_stmt("\r\n============ Audio Get Command Starting ======================\r\n");

    /* Add new API test here */
    
    x_dbg_stmt("\r\n============ Audio Get Command End ============================\r\n");
    
    if( i4_ret != SMR_OK )
    {
            x_scc_comp_close( h_g_scc_aud );
            h_g_scc_aud = NULL_HANDLE;
    }

    return i4_ret;
}

static INT32 _sm_cli_scc_set_disp_cmd(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    INT32       i4_ret = SMR_OK;
    INT32       i4_i;
    CHAR *      ps_disp_set[20] = {

                NULL
                };
    
    if(i4_argc < 3)
    {
        x_dbg_stmt("Please check the usage:\r\n");  

        for( i4_i = 0; ps_disp_set[i4_i] != NULL && i4_i < 20; i4_i++ )
        {
            x_dbg_stmt("set(s) %s\r\n", ps_disp_set[i4_i] );
        }
        return SMR_INV_ARG;
    }

    if( x_strcmp( pps_argv[0],"set" )!= 0 && x_strcmp(pps_argv[0],"s" )!= 0  )
    {
        x_dbg_stmt("Command error! Pleaseuse \"set\" or \"s\".\r\n");        
        return SMR_INV_COMMAND;
    }

    if ( h_g_scc_disp == NULL_HANDLE )
    {
        SM_COMMAND_T            at_scc_cmds[3];
        
        at_scc_cmds [0].e_code     = SCC_CMD_CODE_GRP_TYPE;
        at_scc_cmds [0].u.ui4_data = SCC_CMD_TYPE_DISP;

        at_scc_cmds [1].e_code     = SCC_CMD_CODE_NAME;
        at_scc_cmds [1].u.ps_text  = SN_PRES_MAIN_DISPLAY;

        at_scc_cmds [2].e_code     = SM_CMD_CODE_END;
        
        if (x_scc_comp_open(&(at_scc_cmds [0]), &h_g_scc_disp )!= SMR_OK)
        {
            x_dbg_stmt( "x_scc_comp_open() error!\r\n" );
            return SMR_DRV_COMP_NOT_FOUND;
        }
    }

    x_dbg_stmt("\r\n============ Disp Set Command Starting =======================\r\n");

    /* Add new API test here */

    x_dbg_stmt("\r\n============ Disp Set Command End ============================\r\n");
    
    if( i4_ret != SMR_OK )
    {
            x_scc_comp_close( h_g_scc_disp );
            h_g_scc_disp = NULL_HANDLE;
    }

    return i4_ret;
}

static INT32 _sm_cli_scc_get_disp_cmd(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    INT32       i4_ret = SMR_OK;
    INT32       i4_i;
    CHAR *      ps_disp_get[20] = {

                NULL
                };

    if( i4_argc <2 )
    {
        x_dbg_stmt("Please check the usage:\r\n");  

        for( i4_i = 0; ps_disp_get[i4_i] != NULL && i4_i < 20; i4_i++ )
        {
            x_dbg_stmt("get(g) %s\r\n", ps_disp_get[i4_i] );
        }
        return SMR_INV_ARG;
    }

    if( x_strcmp(pps_argv[0], "get" )!= 0 && x_strcmp(pps_argv[0],"g" ) != 0  )
    {
        x_dbg_stmt("Command error! Pleaseuse \"set\" or \"s\".\r\n");
        return SMR_INV_COMMAND;
    }

    if ( h_g_scc_disp == NULL_HANDLE )
    {
        SM_COMMAND_T            at_scc_cmds[3];
        
        at_scc_cmds [0].e_code     = SCC_CMD_CODE_GRP_TYPE;
        at_scc_cmds [0].u.ui4_data = SCC_CMD_TYPE_DISP;

        at_scc_cmds [1].e_code     = SCC_CMD_CODE_NAME;
        at_scc_cmds [1].u.ps_text  = SN_PRES_MAIN_DISPLAY;

        at_scc_cmds [2].e_code     = SM_CMD_CODE_END;
        
        if (x_scc_comp_open( &(at_scc_cmds [0]), &h_g_scc_disp )!= SMR_OK)
        {
            x_dbg_stmt( "x_scc_comp_open() error!\r\n" );
            return SMR_DRV_COMP_NOT_FOUND;
        }
    }

    x_dbg_stmt("\r\n============ Disp Set Command Starting ======================\r\n");

    /* Add new API test here */

    x_dbg_stmt("\r\n============ Disp Set Command End ============================\r\n");

    if( i4_ret != SMR_OK )
    {
            x_scc_comp_close( h_g_scc_disp );
            h_g_scc_disp = NULL_HANDLE;
    }

    return i4_ret;
}

#ifdef TIME_MEASUREMENT
static INT32 _sm_cli_set_tms_level(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    UINT32   i4_ret;
    UINT16   ui2_tms_level;

    i4_ret = x_cli_parse_tms_level(
        i4_argc,
        pps_argv,
        &ui2_tms_level );
    if ( i4_ret == CLIR_OK )
    {
        sm_set_tms_level(ui2_tms_level);
    }

    x_cli_show_tms_level( sm_get_tms_level() );

    return CLIR_OK;
}

static INT32 _sm_cli_get_tms_level(
    INT32          i4_argc,
    const CHAR**   pps_argv)
{
    return x_cli_show_tms_level( sm_get_tms_level() );
}
#endif /* TIME_MEASUREMENT */

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
INT32 sm_cli_init( VOID )
{
    INT32   i4_ret;

    i4_ret = x_cli_attach_cmd_tbl( at_sm_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_PIPE );
    if ( i4_ret != CLIR_OK && i4_ret != CLIR_NOT_INIT )
    {
        return SMR_INTERNAL_ERROR;
    }

    return SMR_OK;
}

/*----------------------------------------------------------------------------
 * Name:    sm_cli_print_nfy
 * Description:
 * Input arguments:  -
 * Output arguments: -
 * Returns:
 *----------------------------------------------------------------------------*/
VOID sm_cli_print_nfy(
                    CHAR*                       ps_type,          
                    HANDLE_T                    h_stream,
                    SM_COND_T                   e_cond,
                    UINT32                      ui4_event,
                    UINT32                      ui4_data )
{
    BOOL    b_print_nfy = FALSE;

    if (b_g_sm_nfy_trace == TRUE) {
        if ((x_strcmp("all", s_g_sm_nfy_trace_name) == 0) ||
            (x_strcmp(ps_type, s_g_sm_nfy_trace_name) == 0)) {
            b_print_nfy = TRUE;
        }

        if (b_print_nfy)
        {
            const CHAR* ps_state = sm_state_event_to_str(ui4_event);

            if ( ps_state )
            {
                x_dbg_stmt(
                     "[SM] %s, (0x%.8X), %s, %s, DATA(0x%.8X)\r\n",
                    ps_type,
                    (UINT32)h_stream,
                    sm_strm_cond_2_str(e_cond),
                    ps_state,
                    (UINT32)ui4_data );
            }
            else
            {
                x_dbg_stmt(
                     "[SM] %s, (0x%.8X), %s, EVN(0x%.8X), DATA(0x%.8X)\r\n",
                    ps_type,
                    (UINT32)h_stream,
                    sm_strm_cond_2_str(e_cond),
                    (UINT32)ui4_event,
                    (UINT32)ui4_data );
            }
        }
    }
}

/*----------------------------------------------------------------------------
 * Name:    sm_cli_auto
 * Description: auto sm cli when power on
 * Input arguments:  -
 * Output arguments: -
 * Returns:
 *----------------------------------------------------------------------------*/
VOID sm_cli_auto(VOID)
{
    sm_dbg_set_trace_level( DBG_LEVEL_ERROR | DBG_LEVEL_INFO );
    sm_set_dbg_module_mask( SM_DBG_MOD_VS | SM_DBG_MOD_AS );

#if 0
    b_g_sm_nfy_trace = TRUE;
    x_strcpy( s_g_sm_nfy_trace_name, "VS" );
#endif
}

#endif /* CLI_LVL_ALL */

