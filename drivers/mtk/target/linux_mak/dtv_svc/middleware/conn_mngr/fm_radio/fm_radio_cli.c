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
 * $RCSfile: fm_radio_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains the FM Radio Connection Handler CLI-related
 *         functions.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "conn_mngr/fm_radio/_fm_radio.h"
#include "cli/x_cli.h"

#ifdef CLI_LVL_ALL

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

static HANDLE_T    g_h_pipe = NULL_HANDLE;
static HANDLE_T    g_h_conn = NULL_HANDLE;

/*-----------------------------------------------------------------------------
 * Name: fm_radio_cli_show_debug_level
 *
 * Description: This API shows the current analog broadcast handler debug
 *              level.
 *
 * Inputs:  i4_argc   Contains the argument count.
 *          pps_argv  Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK    Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 fm_radio_cli_show_debug_level (
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32           i4_ret;

    i4_ret = x_cli_show_dbg_level(fm_radio_get_dbg_level());

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_cli_set_debug_level
 *
 * Description: This API sets the broadcast handler debug level.
 *
 * Inputs:  i4_argc   Contains the argument count.
 *          pps_argv  Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK    Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 fm_radio_cli_set_debug_level (
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32   i4_ret;
    UINT16  ui2_dbg_level;

    i4_ret = x_cli_parse_dbg_level(i4_argc, pps_argv, & ui2_dbg_level);

    if (i4_ret == CLIR_OK)
    {
        i4_ret = fm_radio_set_dbg_level(ui2_dbg_level);

        i4_ret = (i4_ret == CMR_OK) ? CLIR_OK : CLIR_CMD_EXEC_ERROR;
    }
    else
    {
        i4_ret = CLIR_CMD_EXEC_ERROR;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _fm_radio_cli_open_pipe_nfy
 *
 * Description: the (*x_cm_nfy_fct) type callback function to be specified
 *              when calling the API x_cm_open_pipe(...).
 *
 * Inputs:  See (*x_cm_nfy_fct) for details.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _fm_radio_cli_open_pipe_nfy(
    HANDLE_T     h_handle,
    CM_COND_T    e_nfy_cond,
    VOID*        pv_nfy_tag,
    UINT32       ui4_data
)
{
    switch(e_nfy_cond)
    {
        case CM_COND_OPENED:
            x_dbg_stmt(DBG_PREFIX"==> Notified: Pipe opened.\n");
            break;

        case CM_COND_CLOSED:
            x_dbg_stmt(DBG_PREFIX"==> Notified: Pipe closed.\n");
            break;

        default:
            /* do nothing */
            ;
    }

    return;
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_cli_open_pipe
 *
 * Description: This API toggles the "dump SCDB" flag.
 *
 * Inputs:  i4_argc   Contains the argument count.
 *          pps_argv  Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK    Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 fm_radio_cli_open_pipe (
    INT32               i4_argc,
    const CHAR**        pps_argv
)
{
    CM_COND_T           e_cond;
    CM_COMMAND_T        at_cmds[2];

    at_cmds[0].e_code    = CONN_SRC_TYPE;
    at_cmds[0].u.ps_name = FM_RADIO_HANDLER_DEFAULT_NAME;
    at_cmds[1].e_code    = CONN_END;

    if (x_cm_open_pipe(
        at_cmds,
        129,  /* priority */
        0,    /* flag */
        NULL,
        _fm_radio_cli_open_pipe_nfy,
        &g_h_pipe,
        &e_cond) != CMR_OK)
    {
        g_h_pipe = NULL_HANDLE;

        return CLIR_CMD_EXEC_ERROR;
    }
    x_dbg_stmt(DBG_PREFIX"==> Open pipe successfully, handle = %d\n", g_h_pipe);

    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _fm_radio_cli_connect_nfy
 *
 * Description: the (*x_cm_nfy_fct) type callback function to be specified
 *              when calling the API x_cm_connect(...).
 *
 * Inputs:  See (*x_cm_nfy_fct) for details.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _fm_radio_cli_connect_nfy(
    HANDLE_T     h_handle,
    CM_COND_T    e_nfy_cond,
    VOID*        pv_nfy_tag,
    UINT32       ui4_data
)
{
    switch(e_nfy_cond)
    {
        case CM_COND_CONNECTED:
            x_dbg_stmt(DBG_PREFIX"==> Notified: Connected.\n");
            break;

        case CM_COND_DISCONNECTED:
            x_dbg_stmt(DBG_PREFIX"==> Notified: Disconnected.\n");
            break;

        case FM_RADIO_CONN_INFO_DETECT_DONE:
            x_dbg_stmt(DBG_PREFIX"==> Notified: Detected done.\n");
            break;

        default:
            /* do nothing */
            ;
    }

    return;
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_cli_do_connect
 *
 * Description: This API toggles the "dump SCDB" flag.
 *
 * Inputs:  i4_argc   Contains the argument count.
 *          pps_argv  Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK    Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 fm_radio_cli_do_connect (
    INT32               i4_argc,
    const CHAR**        pps_argv
)
{
    CM_COND_T           e_cond;
    CM_COMMAND_T        at_cmds[3]; /* command arrays */

    UINT32              ui4_freq;

    if (i4_argc == 2)
    {
        ui4_freq = (UINT32)x_strtoull(pps_argv[1], NULL, 0);
        x_dbg_stmt(DBG_PREFIX"==> Connecting to freq = %d...\n", ui4_freq);

        at_cmds[0].e_code       = FM_RADIO_CONN_DELIVERY_SYS;
        at_cmds[0].u.ui4_number = TUNER_CONN_TYPE_TER_ANA;
        at_cmds[1].e_code       = FM_RADIO_CONN_FREQUENCY;
        at_cmds[1].u.ui4_number = ui4_freq;
        at_cmds[2].e_code       = CONN_END;

        if (x_cm_connect(
            g_h_pipe,
            at_cmds,
            NULL,
            _fm_radio_cli_connect_nfy,
            &g_h_conn,
            &e_cond) != CMR_OK)
        {
            g_h_conn = NULL_HANDLE;

            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt(DBG_PREFIX"Usage: connect <freq>\n");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_cli_set_detect
 *
 * Description: This API toggles the "dump SCDB" flag.
 *
 * Inputs:  i4_argc   Contains the argument count.
 *          pps_argv  Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK    Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 fm_radio_cli_set_detect (
    INT32               i4_argc,
    const CHAR**        pps_argv
)
{
    INT32               i4_ret;

    i4_ret = x_cm_set(g_h_conn,
                      FM_RADIO_CTRL_SET_DET_CHANNEL,
                      NULL);

    if ((i4_ret != CMR_OK) && (i4_ret != CMR_ASYNC_NFY))
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_cli_do_disconnect
 *
 * Description: This API toggles the "dump SCDB" flag.
 *
 * Inputs:  i4_argc   Contains the argument count.
 *          pps_argv  Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK    Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 fm_radio_cli_do_disconnect (
    INT32               i4_argc,
    const CHAR**        pps_argv
)
{
    CM_COND_T           e_cond;

    x_dbg_stmt(DBG_PREFIX"==> Disconnecting...\n");

    if (x_cm_disconnect(
        g_h_conn,
        &e_cond) != CMR_OK)
    {
        return CLIR_CMD_EXEC_ERROR;
    }
    g_h_conn = NULL_HANDLE;

    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_cli_set_freq
 *
 * Description: This API toggles the "dump SCDB" flag.
 *
 * Inputs:  i4_argc   Contains the argument count.
 *          pps_argv  Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK    Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 fm_radio_cli_set_freq (
    INT32               i4_argc,
    const CHAR**        pps_argv
)
{
    UINT32              ui4_freq;
    INT32               i4_ret;

    if (i4_argc == 2)
    {
        ui4_freq = (UINT32)x_strtoull(pps_argv[1], NULL, 0);
        x_dbg_stmt(DBG_PREFIX"==> Setting freq to %d...\n", ui4_freq);

        i4_ret = x_cm_set(g_h_conn,
                          FM_RADIO_CTRL_SET_TUNE_FREQ,
                          (VOID*)ui4_freq);

        if ((i4_ret != CMR_OK) && (i4_ret != CMR_ASYNC_NFY))
        {
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt(DBG_PREFIX"Usage: connect <freq>\n");
        return CLIR_INV_CMD_USAGE;
    }

    return CLIR_OK;
}


#endif /* CLI_LVL_ALL */

/*-----------------------------------------------------------------------------
 * Name: fm_radio_cli_init
 *
 * Description: This API attaches the analog broadcast handler CLI tables if
 *              CLI_LVL_ALL is defined, otherwise it does nothing.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID fm_radio_cli_init (VOID)
{
#ifdef CLI_LVL_ALL
    static CLI_EXEC_T  at_fm_radio_cli_tbl[] =
    {
        /* get debug level */
        {
            CLI_GET_DBG_LVL_STR,
            NULL,
            fm_radio_cli_show_debug_level,
            NULL,
            CLI_GET_DBG_LVL_HELP_STR,
            CLI_SUPERVISOR
        },
        /* set debug level */
        {
            CLI_SET_DBG_LVL_STR,
            NULL,
            fm_radio_cli_set_debug_level,
            NULL,
            CLI_SET_DBG_LVL_HELP_STR,
            CLI_SUPERVISOR
        },
        /* open a CM pipe */
        {
            "open",
            "o",
            fm_radio_cli_open_pipe,
            NULL,
            "Open a CM pipe",
            CLI_SUPERVISOR
        },
        /* do connect */
        {
            "connect",
            "c",
            fm_radio_cli_do_connect,
            NULL,
            "Do connect (usage: connect <freq>)",
            CLI_SUPERVISOR
        },
        /* set detect channel */
        {
            "detect",
            "d",
            fm_radio_cli_set_detect,
            NULL,
            "Do detect",
            CLI_SUPERVISOR
        },
        /* do disconnect */
        {
            "disconnect",
            "dc",
            fm_radio_cli_do_disconnect,
            NULL,
            "Do disconnect",
            CLI_SUPERVISOR
        },
        /* set frequency */
        {
            "set_freq",
            "sf",
            fm_radio_cli_set_freq,
            NULL,
            "Set frequency (usage: set_freq <freq>)",
            CLI_SUPERVISOR
        },

        END_OF_CLI_CMD_TBL
    };

    static CLI_EXEC_T at_fm_radio_root_cli_tbl[] =
    {
        {
            "fmrdo",
            NULL,
            NULL,
            at_fm_radio_cli_tbl,
            "FM radio handler commands",
            CLI_SUPERVISOR
        },
        END_OF_CLI_CMD_TBL
    };

    static BOOL     b_attached = FALSE;

    CRIT_STATE_T    t_state;
    BOOL            b_attach;

    t_state = x_crit_start();

    b_attach = !b_attached;

    b_attached = TRUE;

    x_crit_end(t_state);

    if (b_attach)
    {
        INT32    i4_ret;

        i4_ret = x_cli_attach_cmd_tbl(at_fm_radio_root_cli_tbl,
                                      CLI_CAT_MW,
                                      CLI_GRP_NONE);

        if (i4_ret != CLIR_OK)
        {
            t_state = x_crit_start();

            b_attached = FALSE;

            x_crit_end(t_state);
        }
    }
#endif /* CLI_LVL_ALL */
}

