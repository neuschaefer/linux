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
 * $RCSfile: ch_hdlr_cli.c,v $
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
#include "os/inc/x_os.h"
#include "graphic/x_gl.h"

#include "inc/common.h"
#include "inc/u_sys_name.h"
#include "svctx/svctx.h"
#include "svctx/svctx_dbg.h"
#include "svctx/util/player.h"
#include "svctx/ch_hdlr/ch_svc_req.h"
#include "svctx/svctx_cli.h"
#include "dbg/x_dbg_drv.h"

#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static INT32 _ch_hdlr_cli_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _ch_hdlr_cli_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _ch_hdlr_cli_dump_ch_svc_req_data(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _ch_hdlr_cli_dump_strm_req_data(INT32 i4_argc, const CHAR** pps_argv);
#ifndef ANDROID 
static INT32 _ch_hdlr_cli_select_subtitle(INT32 i4_argc, const CHAR** pps_argv);
#endif
#ifdef TIME_MEASUREMENT
static INT32 _ch_hdlr_cli_set_tms_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _ch_hdlr_cli_get_tms_level(INT32 i4_argc, const CHAR** pps_argv);
#endif /* TIME_MEASUREMENT */
/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
static CLI_EXEC_T at_ch_svc_req_cmd_tbl[] =
{
    {   "dump",
        "d",
        _ch_hdlr_cli_dump_ch_svc_req_data,
        NULL,
        "usage: d using[u]|pooled[p]",
        CLI_GUEST
    },
    END_OF_CLI_CMD_TBL
};
static CLI_EXEC_T at_strm_req_cmd_tbl[] =
{
    {   "dump",
        "d",
        _ch_hdlr_cli_dump_strm_req_data,
        NULL,
        "usage: d using[u]|pooled[p]",
        CLI_GUEST
    },
 #ifndef ANDROID 
    {   "sbtl",
        "s",
        _ch_hdlr_cli_select_subtitle,
        NULL,
        "usage: s [639_lang_code, if empty = stop] [svctx name]",
        CLI_SUPERVISOR
    },
 #endif
     END_OF_CLI_CMD_TBL
};

static CLI_EXEC_T at_ch_hdlr_cmd_tbl[] =
{
    {   "ch_svc_req",
        "csvc",
        NULL,
        at_ch_svc_req_cmd_tbl,
        "Commands for Channel SVC REQ",
        CLI_GUEST
    },
    {   "ch_strm_req",
        "cstrm",
        NULL,
        at_strm_req_cmd_tbl,
        "Commands for Channel Strm REQ",
        CLI_GUEST
    },
    {   "scdb",
        "scdb",
        svctx_cli_dump_scdb_data,
        NULL,
        "usage: scdb",
        CLI_GUEST
    },
     {
        CLI_GET_DBG_LVL_STR,
        NULL,
        _ch_hdlr_cli_get_dbg_level,
        NULL,
        CLI_GET_DBG_LVL_HELP_STR,
        CLI_GUEST
    },
    {
        CLI_SET_DBG_LVL_STR,
        NULL,
        _ch_hdlr_cli_set_dbg_level,
        NULL,
        CLI_SET_DBG_LVL_HELP_STR,
        CLI_GUEST
    },
#ifdef CLI_LVL_ALL
    {   "stop",
        "st",
        svctx_cli_sync_stop_svctx,
        NULL,
        "usage: st",
        CLI_SUPERVISOR
    },
    {   "close",
        "cl",
        svctx_cli_close_svctx,
        NULL,
        "usage: cl",
        CLI_SUPERVISOR
    },    
#ifdef TIME_MEASUREMENT
    {
        CLI_SET_TMS_LVL_STR,
        NULL,
        _ch_hdlr_cli_set_tms_level,
        NULL,
        CLI_SET_TMS_LVL_HELP_STR,
        CLI_SUPERVISOR
    },
    {
        CLI_GET_TMS_LVL_STR,
        NULL,
        _ch_hdlr_cli_get_tms_level,
        NULL,
        CLI_GET_TMS_LVL_HELP_STR,
        CLI_SUPERVISOR
    },
#endif /* TIME_MEASUREMENT */
#endif
    END_OF_CLI_CMD_TBL
};

/* Service Context root command table */
static CLI_EXEC_T at_ch_hdlr_root_cmd_tbl[] =
{
    {"svctx_ch", NULL, NULL, at_ch_hdlr_cmd_tbl, "SVCTX-CH_HDLR commands",   CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

static HANDLE_T h_g_crnt_svctx = NULL_HANDLE;
/*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_cli_open_svctx
 * Description
 * Input arguments
 *      None
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static BOOL _ch_hdlr_cli_open_svctx(
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
 * Name: _ch_hdlr_cli_get_dbg_level
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
static INT32 _ch_hdlr_cli_get_dbg_level (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32  i4_ret;

    i4_ret = x_cli_show_dbg_level(svctx_get_dbg_level());

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: _ch_hdlr_cli_set_dbg_level
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
static INT32 _ch_hdlr_cli_set_dbg_level (INT32 i4_argc, const CHAR** pps_argv)
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
 * Name: _ch_hdlr_cli_dump_ch_svc_req_data
 *
 * Description: This API dump Channel Svc Req's data.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_cli_dump_ch_svc_req_data (INT32 i4_argc, const CHAR** pps_argv)
{
    CH_SVC_REQ_T* pt_using  = NULL;
    CH_SVC_REQ_T* pt_pooled = NULL;

    if (i4_argc == 1) {
        pt_using  = ch_svc_req_get_using_head();
        pt_pooled = ch_svc_req_get_pooled_head();
    } else if (i4_argc == 2) {
        if (x_strcmp("u", pps_argv[1]) == 0  || x_strcmp("using", pps_argv[1]) == 0) {
            pt_using  = ch_svc_req_get_using_head();
        } else if (x_strcmp("p", pps_argv[1]) == 0  || x_strcmp("pooled", pps_argv[1]) == 0) {
            pt_pooled = ch_svc_req_get_pooled_head();
        }
    }

    if (pt_using != NULL) {
        x_dbg_stmt("[using ch_svc_reqs]\r\n");
        for (; pt_using != NULL; pt_using = ch_svc_req_next(pt_using)) {
            ch_svc_req_print_self(pt_using);
        }
        x_dbg_stmt("\r\n");
    }

    if (pt_pooled != NULL) {
        x_dbg_stmt("[pooled ch_svc_reqs]\r\n");
        for (; pt_pooled != NULL; pt_pooled = ch_svc_req_next(pt_pooled)) {
            x_dbg_stmt("ch_svc_req - 0x%x\r\n", pt_pooled);
        }
        x_dbg_stmt("\r\n");
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _ch_hdlr_cli_dump_strm_req_data
 *
 * Description: This API dump Channel Svc Req's data.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_cli_dump_strm_req_data (INT32 i4_argc, const CHAR** pps_argv)
{
    STRM_REQ_T* pt_using  = NULL;
    STRM_REQ_T* pt_pooled = NULL;

    if (i4_argc == 1) {
        pt_using  = ch_strm_req_get_using_head();
        pt_pooled = ch_strm_req_get_pooled_head();
    } else if (i4_argc == 2) {
        if (x_strcmp("u", pps_argv[1]) == 0  || x_strcmp("using", pps_argv[1]) == 0) {
            pt_using  = ch_strm_req_get_using_head();
        } else if (x_strcmp("p", pps_argv[1]) == 0  || x_strcmp("pooled", pps_argv[1]) == 0) {
            pt_pooled = ch_strm_req_get_pooled_head();
        }
    }

    if (pt_using != NULL) {
        x_dbg_stmt("[using ch_strm_reqs]\r\n");
        for (; pt_using != NULL; pt_using = ch_strm_req_next(pt_using)) {
            ch_strm_req_print_self(pt_using);
        }
        x_dbg_stmt("\r\n");
    }

    if (pt_pooled != NULL) {
        x_dbg_stmt("[pooled ch_strm_reqs]\r\n");
        for (; pt_pooled != NULL; pt_pooled = ch_strm_req_next(pt_pooled)) {
            x_dbg_stmt("ch_strm_req - 0x%x\r\n", pt_pooled);
        }
        x_dbg_stmt("\r\n");
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_hdlr_cli_svctx_nfy_fct
 * Description
 * Input arguments
 *      None
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
VOID _ch_hdlr_cli_svctx_nfy_fct(
                    HANDLE_T                        h_svctx,
                    SVCTX_COND_T                    e_nfy_cond,
                    SVCTX_NTFY_CODE_T               e_code,
                    STREAM_TYPE_T                   e_stream_type,
                    VOID*                           pv_nfy_tag
                    )
{
#ifdef CLI_LVL_ALL
    x_dbg_stmt("%s() - cond=%s, code=%s, strm=%s \r\n",
                    __func__,
                    s_g_state_strings[e_nfy_cond],
                    s_g_svctx_ntfy_codes[e_code],
                    s_g_strm_types[e_stream_type]
                    );
#endif
}
#ifndef ANDROID 
/*-----------------------------------------------------------------------------
 * Name: _ch_hdlr_cli_create_osd_plane
 *
 * Description: This API is used to create a OSD plane
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static BOOL _ch_hdlr_cli_create_osd_plane(
                    UINT16                  ui2_osd_plan_id,
                    HANDLE_T*               ph_g_osd_plane
                    )
{
    static HANDLE_T h_g_screen = NULL_HANDLE;
    CHAR*  pt_screen_name      = SN_PRES_MAIN_DISPLAY;
    INT32  i4_ret;

    if (x_handle_valid(h_g_screen) == TRUE) {
        x_gl_obj_free(h_g_screen);
        h_g_screen = NULL_HANDLE;
    }

    i4_ret = x_gl_screen_open(
                    pt_screen_name,
                    NULL,
                    NULL,
                    &h_g_screen
                    );
    if (GLR_OK != i4_ret) {
        return FALSE;
    }

    if (x_handle_valid(*ph_g_osd_plane) == TRUE) {
        x_gl_obj_free(*ph_g_osd_plane);
        *ph_g_osd_plane = NULL_HANDLE;
    }

    i4_ret = x_gl_plane_open(
                    h_g_screen,
                    ui2_osd_plan_id,
                    ph_g_osd_plane
                    );
    if (GLR_OK != i4_ret) {
        return FALSE;
    }

    return TRUE;
}
/*-----------------------------------------------------------------------------
 * Name: _ch_hdlr_cli_select_subtitle
 *
 * Description: This API is used to select subtitle by ISO639 code. For example,
 *              eng
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _ch_hdlr_cli_select_subtitle (INT32 i4_argc, const CHAR** pps_argv)
{
    static HANDLE_T h_g_osd_plane = NULL_HANDLE;
    UINT16          ui2_osd_plane_id = 0;
    INT32           i4_ret;

    if (i4_argc == 1) {
        if (x_handle_valid(h_g_crnt_svctx) == TRUE) {
            i4_ret = x_svctx_stop_stream(
                        h_g_crnt_svctx,
                        ST_SUBTITLE
                        );
            if (i4_ret == SVCTXR_OK) {
                x_dbg_stmt("Stoping subtitle...\r\n");
            } else {
                x_dbg_stmt("No subtitle to stop\r\n");
            }
        }
        return CLIR_OK;
    }

    if (_ch_hdlr_cli_open_svctx(i4_argc > 2 ? (CHAR*)pps_argv[2] : NULL, &h_g_crnt_svctx) == FALSE) {
        x_dbg_stmt("create service contex failed!\r\n");
        return CLIR_OK;
    }

    if (_ch_hdlr_cli_create_osd_plane(ui2_osd_plane_id, &h_g_osd_plane) == FALSE) {
        x_dbg_stmt("create OSD plane(%d) failed!\r\n", ui2_osd_plane_id);
        return CLIR_OK;
    }

    i4_ret = x_svctx_select_subtitle_stream(
                    h_g_crnt_svctx,
                    h_g_osd_plane,
                    (const ISO_639_LANG_T*)pps_argv[1],
                    _ch_hdlr_cli_svctx_nfy_fct,
                    NULL
                    );

    if (i4_ret == SVCTXR_OK) {
        x_dbg_stmt("Selecting subtitle(%s)...\r\n", pps_argv[1]);
    } else {
        x_dbg_stmt("No subtitle(%s)\r\n", pps_argv[1]);
    }

    return CLIR_OK;
}
#endif
#ifdef TIME_MEASUREMENT
/*-----------------------------------------------------------------------------
 * Name: _ch_hdlr_cli_set_tms_level
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
static INT32 _ch_hdlr_cli_set_tms_level(INT32 i4_argc, const CHAR** pps_argv)
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
 * Name: _ch_hdlr_cli_get_tms_level
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
static INT32 _ch_hdlr_cli_get_tms_level(INT32 i4_argc, const CHAR** pps_argv)
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
INT32 ch_hdlr_cli_attach_cmd_tbl(VOID)
{
    return (x_cli_attach_cmd_tbl(at_ch_hdlr_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_PIPE));
}
#else
/*----------------------------------------------------------------------------
 * Name:    svctx_cli_attach_cmd_tbl
 * Description:
 * Input arguments:  -
 * Output arguments: -
 * Returns:
 *----------------------------------------------------------------------------*/
INT32 ch_hdlr_cli_attach_cmd_tbl(VOID)
{
    return CLIR_OK;
}
#endif
