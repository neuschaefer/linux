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
 * $RCSfile: inp_hdlr_cli.c,v $
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
#include "svctx/svctx.h"
#include "svctx/svctx_dbg.h"
#include "svctx/svctx_cli.h"
#include "svctx/inp_hdlr/inp_svc_req.h"
#include "svctx/util/av_monitor.h"
#include "svctx/inp_hdlr/u_inp_hdlr.h"
#include "dbg/x_dbg_drv.h"

#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static INT32 _inp_hdlr_cli_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _inp_hdlr_cli_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _inp_hdlr_cli_dump_inp_svc_req_data(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _inp_hdlr_cli_dump_avm_data (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _inp_hdlr_cli_ut_set_audio_type (INT32 i4_argc, const CHAR** pps_argv);

#ifdef TIME_MEASUREMENT
static INT32 _inp_hdlr_cli_set_tms_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _inp_hdlr_cli_get_tms_level(INT32 i4_argc, const CHAR** pps_argv);
#endif /* TIME_MEASUREMENT */
/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
static CLI_EXEC_T at_inp_svc_req_cmd_tbl[] =
{
    {   "dump",
	    "d",
	    _inp_hdlr_cli_dump_inp_svc_req_data,
	    NULL,
	    "usage: d using[u]|pooled[p]",
	    CLI_SUPERVISOR
	},
 	END_OF_CLI_CMD_TBL
};
static CLI_EXEC_T at_avm_cmd_tbl[] =
{
    {   "dump",
	    "d",
	    _inp_hdlr_cli_dump_avm_data,
	    NULL,
	    "usage: d using[u]|pooled[p]",
	    CLI_SUPERVISOR
	},
 	END_OF_CLI_CMD_TBL
};
static CLI_EXEC_T at_inp_ut_cmd_tbl[] =
{
    {   "set audio type",
	    "sat",
	    _inp_hdlr_cli_ut_set_audio_type,
	    NULL,
	    "usage: sat a(udio-in)|s(pidf)|h(dmi)|v(ga)|c(vbs)) [main_svctx|sub_svctx]",
	    CLI_SUPERVISOR
	},
 	END_OF_CLI_CMD_TBL
};
static CLI_EXEC_T at_input_hdlr_cmd_tbl[] =
{
    {   "inp_svc_req",
	    "isvc",
	    NULL,
	    at_inp_svc_req_cmd_tbl,
	    "Commands for Input SVC REQ",
	    CLI_SUPERVISOR
	},
	{   "av_monitor",
	    "avm",
	    NULL,
	    at_avm_cmd_tbl,
	    "Commands for AV Monitor",
	    CLI_SUPERVISOR
	},
	{   "unit test",
	    "ut",
	    NULL,
	    at_inp_ut_cmd_tbl,
	    "Commands for unit test",
	    CLI_SUPERVISOR
	},
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
	{   "scdb",
	    "scdb",
	    svctx_cli_dump_scdb_data,
	    NULL,
	    "usage: scdb",
	    CLI_SUPERVISOR
	},
 	{
        CLI_GET_DBG_LVL_STR,
        NULL,
        _inp_hdlr_cli_get_dbg_level,
        NULL,
        CLI_GET_DBG_LVL_HELP_STR,
        CLI_SUPERVISOR
    },
	{
        CLI_SET_DBG_LVL_STR,
        NULL,
        _inp_hdlr_cli_set_dbg_level,
        NULL,
        CLI_SET_DBG_LVL_HELP_STR,
        CLI_SUPERVISOR
    },
#ifdef TIME_MEASUREMENT
    {
        CLI_SET_TMS_LVL_STR,
        NULL,
        _inp_hdlr_cli_set_tms_level,
        NULL,
        CLI_SET_TMS_LVL_HELP_STR,
        CLI_SUPERVISOR
    },
    {
        CLI_GET_TMS_LVL_STR,
        NULL,
        _inp_hdlr_cli_get_tms_level,
        NULL,
        CLI_GET_TMS_LVL_HELP_STR,
        CLI_SUPERVISOR
    },
#endif /* TIME_MEASUREMENT */

    END_OF_CLI_CMD_TBL
};

/* SVL Builder root command table */
static CLI_EXEC_T at_input_hdlr_root_cmd_tbl[] =
{
	{"svctx_inp", NULL, NULL, at_input_hdlr_cmd_tbl, "SVCTX-INP_HDLR commands",   CLI_SUPERVISOR},
	END_OF_CLI_CMD_TBL
};
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
INT32 inp_hdlr_cli_attach_cmd_tbl(VOID)
{
    return (x_cli_attach_cmd_tbl(at_input_hdlr_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_PIPE));
}
/*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: _inp_hdlr_cli_get_dbg_level
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
static INT32 _inp_hdlr_cli_get_dbg_level (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32  i4_ret;

    i4_ret = x_cli_show_dbg_level(svctx_get_dbg_level());

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: _inp_hdlr_cli_set_dbg_level
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
static INT32 _inp_hdlr_cli_set_dbg_level (INT32 i4_argc, const CHAR** pps_argv)
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
 * Name: _inp_hdlr_cli_ut_set_audio_type
 *
 * Description: This API is used to test SVCTX_INP_SET_TYPE_AUDIO_DEVICE
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_cli_ut_set_audio_type (INT32 i4_argc, const CHAR** pps_argv)
{
    HANDLE_T       h_svctx       = NULL_HANDLE;
    CHAR*          ps_svctx_name = "main_svctx";
    SRC_AVC_INFO_T t_audio_info;
    INT32          i4_ret;

    if (i4_argc > 2) {
        ps_svctx_name = (CHAR*)pps_argv[2];
    }
    
    if (x_strcmp("a", pps_argv[1]) == 0) {
        t_audio_info.e_type             = DEV_AVC_AUDIO_INP;
        t_audio_info.t_hint.e_dev_type  = DEV_UNKNOWN;
        t_audio_info.t_hint.e_type      = SRC_AVC_HINT_NONE;
    } else if (x_strcmp("na", pps_argv[1]) == 0) {
        t_audio_info.e_type             = DEV_UNKNOWN;
        t_audio_info.t_hint.e_dev_type  = DEV_UNKNOWN;
        t_audio_info.t_hint.e_type      = SRC_AVC_HINT_NONE;
    } else if (x_strcmp("s", pps_argv[1]) == 0) {
        t_audio_info.e_type             = DEV_AVC_SPDIF;
        t_audio_info.t_hint.e_dev_type  = DEV_UNKNOWN;
        t_audio_info.t_hint.e_type      = SRC_AVC_HINT_NONE;
    } else if (x_strcmp("h", pps_argv[1]) == 0) {
        t_audio_info.e_type             = DEV_AVC_AUDIO_INP;
        t_audio_info.t_hint.e_dev_type  = DEV_AVC_HDMI;
        t_audio_info.t_hint.e_type      = SRC_AVC_HINT_SPECIFIC_DEVICE_FIRST;
    } else if (x_strcmp("nh", pps_argv[1]) == 0) {
        t_audio_info.e_type             = DEV_UNKNOWN;
        t_audio_info.t_hint.e_dev_type  = DEV_AVC_HDMI;
        t_audio_info.t_hint.e_type      = SRC_AVC_HINT_SPECIFIC_DEVICE_FIRST;
    } else if (x_strcmp("v", pps_argv[1]) == 0) {
        t_audio_info.e_type             = DEV_AVC_VGA;
        t_audio_info.t_hint.e_dev_type  = DEV_UNKNOWN;
        t_audio_info.t_hint.e_type      = SRC_AVC_HINT_NONE;
    } else if (x_strcmp("c", pps_argv[1]) == 0) {
        t_audio_info.e_type             = DEV_AVC_AUDIO_INP;
        t_audio_info.t_hint.e_dev_type  = DEV_AVC_COMP_VIDEO;
        t_audio_info.t_hint.e_type      = SRC_AVC_HINT_SPECIFIC_DEVICE_FIRST;
    } else {
        x_dbg_stmt("Invalid audio type!\r\n");
        return CLIR_OK;
    }

    i4_ret = x_svctx_open(ps_svctx_name, &h_svctx);
    if (i4_ret != SVCTXR_OK) {
        x_dbg_stmt("x_svctx_open(%s) failed, i4_ret = %d\r\n", i4_ret, ps_svctx_name);
        return CLIR_OK;
    }
    
    i4_ret = x_svctx_set(
                    h_svctx,
                    SVCTX_INP_SET_TYPE_AUDIO_SRC_INFO,
                    (VOID*)(&t_audio_info),
                    sizeof(SRC_AVC_INFO_T)
                    );

    i4_ret = x_svctx_close(h_svctx);
    if (i4_ret != SVCTXR_OK) {
        x_dbg_stmt("x_svctx_close(main_svctx) failed, i4_ret = %d\r\n", i4_ret);
    }

    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _inp_hdlr_cli_dump_avm_data
 *
 * Description: This API dump Input Svc Req's data.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_cli_dump_avm_data (INT32 i4_argc, const CHAR** pps_argv)
{
    AV_MONITOR_T* pt_using  = NULL;
    AV_MONITOR_T* pt_pooled = NULL;

    if (i4_argc == 1) {
        pt_using  = avm_get_using_head();
        pt_pooled = avm_get_pooled_head();
    } else if (i4_argc == 2) {
        if (x_strcmp("u", pps_argv[1]) == 0  || x_strcmp("using", pps_argv[1]) == 0) {
            pt_using  = avm_get_using_head();
        } else if (x_strcmp("p", pps_argv[1]) == 0  || x_strcmp("pooled", pps_argv[1]) == 0) {
            pt_pooled = avm_get_pooled_head();
        }
    }

    if (pt_using != NULL) {
        x_dbg_stmt("[using av monitors]\r\n");
        for (; pt_using != NULL; pt_using = avm_next(pt_using)) {
            avm_print_self(pt_using);
        }
        x_dbg_stmt("\r\n");
    }

    if (pt_pooled != NULL) {
        x_dbg_stmt("[pooled av monitors]\r\n");
        for (; pt_pooled != NULL; pt_pooled = avm_next(pt_pooled)) {
            x_dbg_stmt("av monitor - 0x%x\r\n", pt_pooled);
        }
        x_dbg_stmt("\r\n");
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _inp_hdlr_cli_dump_inp_svc_req_data
 *
 * Description: This API dump Input Svc Req's data.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK         Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _inp_hdlr_cli_dump_inp_svc_req_data (INT32 i4_argc, const CHAR** pps_argv)
{
    INP_SVC_REQ_T* pt_using  = NULL;
    INP_SVC_REQ_T* pt_pooled = NULL;

    if (i4_argc == 1) {
        pt_using  = inp_svc_req_get_using_head();
        pt_pooled = inp_svc_req_get_pooled_head();
    } else if (i4_argc == 2) {
        if (x_strcmp("u", pps_argv[1]) == 0  || x_strcmp("using", pps_argv[1]) == 0) {
            pt_using  = inp_svc_req_get_using_head();
        } else if (x_strcmp("p", pps_argv[1]) == 0  || x_strcmp("pooled", pps_argv[1]) == 0) {
            pt_pooled = inp_svc_req_get_pooled_head();
        }
    }

    if (pt_using != NULL) {
        x_dbg_stmt("[using inp_svc_reqs]\r\n");
        for (; pt_using != NULL; pt_using = inp_svc_req_next(pt_using)) {
            inp_svc_req_print_self(pt_using);
        }
        x_dbg_stmt("\r\n");
    }

    if (pt_pooled != NULL) {
        x_dbg_stmt("[pooled inp_svc_reqs]\r\n");
        for (; pt_pooled != NULL; pt_pooled = inp_svc_req_next(pt_pooled)) {
            x_dbg_stmt("inp_svc_req - 0x%x\r\n", pt_pooled);
        }
        x_dbg_stmt("\r\n");
    }
    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_hdlr_cli_svctx_nfy_fct
 * Description
 * Input arguments
 *      None
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
VOID _inp_hdlr_cli_svctx_nfy_fct(
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
#ifdef TIME_MEASUREMENT
/*-----------------------------------------------------------------------------
 * Name: _inp_hdlr_cli_set_tms_level
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
static INT32 _inp_hdlr_cli_set_tms_level(INT32 i4_argc, const CHAR** pps_argv)
{
    UINT32 i4_ret;
    UINT16 ui2_tms_level;

    i4_ret = x_cli_parse_tms_level(i4_argc, pps_argv, &ui2_tms_level);
    if ( i4_ret == CLIR_OK ) {
        svctx_set_tms_level(ui2_tms_level);
    }

    x_cli_show_tms_level( svctx_get_tms_level() );

    return CLIR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _inp_hdlr_cli_get_tms_level
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
static INT32 _inp_hdlr_cli_get_tms_level(INT32 i4_argc, const CHAR** pps_argv)
{
    return x_cli_show_tms_level(svctx_get_tms_level());
}
#endif /* TIME_MEASUREMENT */
#else
/*----------------------------------------------------------------------------
 * Name:    svctx_cli_attach_cmd_tbl
 * Description:
 * Input arguments:  -
 * Output arguments: -
 * Returns:
 *----------------------------------------------------------------------------*/
INT32 inp_hdlr_cli_attach_cmd_tbl(VOID)
{
    return CLIR_OK;
}
#endif
