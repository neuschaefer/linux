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
 * $RCSfile: c_cli_input.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/4 $
 * $SWAuthor: Alec Lu $
 * $MD5HEX: e6e42ffefefe41c633e95ec138eae58a $
 *
 * Description:
 *         The program will handle UART input to CLI console and is exported
 *         to applications.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "inc/common.h"
#include "cli/_cli.h"
#include "cli/cli.h"
#include "cli/x_cli.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: c_cli_attach_cmd_tbl
 *
 * Description: This API attaches a command table to CLI root.
 *
 * Inputs:  pt_tbl          The command table to be attached.
 *          e_category      The category that the command table belongs to.
 *          ui8_group_mask  The group(s) that the command table belongs to.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_NOT_INIT           The CLI has not been initialized.
 *          CLIR_INV_ARG            One or more invalid arguments.
 *          CLIR_CMD_TBL_FULL       Command table is full.
 ----------------------------------------------------------------------------*/
INT32 c_cli_attach_cmd_tbl(CLI_EXEC_T* pt_tbl,
                           CLI_CAT_T   e_category,
                           UINT64      ui8_group_mask)
{
    return (x_cli_attach_cmd_tbl(pt_tbl, e_category, ui8_group_mask));
}


/*-----------------------------------------------------------------------------
 * Name: c_cli_attach_alias
 *
 * Description: This API adds, deletes, or raplaces an alias element from alias
 *              table.
 *
 * Inputs:  ps_alias    Contains the alias to attach.
 *          ps_cmd      Contains the command string corresponding to the alias.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_NOT_INIT           The CLI has not been initialized.
 *          CLIR_INV_ARG            One or more invalid arguments.
 *          CLIR_ALIAS_TOO_LONG     Alias is too long.
 *          CLIR_CMD_TOO_LONG       CLI command is too long.
 *          CLIR_ALIAS_TBL_FULL     CLI alias table is full.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
INT32 c_cli_attach_alias(const CHAR* ps_alias,
                         const CHAR* ps_cmd)
{
    return (x_cli_attach_alias(ps_alias,
                               ps_cmd));
}


/*-----------------------------------------------------------------------------
 * Name: c_cli_parser
 *
 * Description: This API parses CLI command and performs corresponding
 *              operation.
 *
 * Inputs:  ps_cmd      Contain the command to parse.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_NOT_INIT           The CLI has not been initialized.
 *          CLIR_INV_ARG            One or more invalid arguments.
 *          CLIR_UNKNOWN_CMD        Unknown CLI command.
 ----------------------------------------------------------------------------*/
INT32 c_cli_parser(const CHAR* ps_cmd)
{
    return (x_cli_parser(ps_cmd));
}


/*-----------------------------------------------------------------------------
 * Name: c_cli_parser_arg
 *
 * Description: This API parses CLI command and performs corresponding
 *              operation. The variable-length argument lists are supported.
 *
 * Inputs:  ps_cmd   Contain the format.
 *          ...      Variable argument list.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK             Routine successful.
 *          CLIR_NOT_INIT       The CLI has not been initialized.
 *          CLIR_INV_ARG        One or more invalid arguments.
 *          CLIR_UNKNOWN_CMD    Unknown CLI command.
 ----------------------------------------------------------------------------*/
INT32 c_cli_parser_arg(const CHAR* ps_cmd, ...)
{
    CHAR        s_buf[CLI_CMD_BUF_SIZE];
    VA_LIST     t_ap;

    if (!cli_is_inited())
    {
        return CLIR_NOT_INIT;
    }

    VA_START(t_ap, ps_cmd);
    x_vsprintf(s_buf, ps_cmd, t_ap);
    VA_END(t_ap);

    return (cli_parser(s_buf));
}


/*-----------------------------------------------------------------------------
 * Name: c_cli_passwd
 *
 * Description: This API generates a password according to current system
 *              date and time.
 *
 * Inputs:  t_password          The password calculation method.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_CMD_EXEC_ERROR     Input password is incorrect.
 ----------------------------------------------------------------------------*/
INT32 c_cli_passwd(CLI_PASSWD_T t_password)
{
    return (x_cli_passwd(t_password));
}


/*-----------------------------------------------------------------------------
 * Name: c_cli_get_char_timeout
 *
 * Description: This API waits for the first available character within
 *              specific time period.
 *
 * Inputs:  ui4_time    The waiting time period in ms.
 *
 * Outputs: -
 *
 * Returns: ASCII_NULL      The UART queue is empty.
 *          Others          The first available character from UART queue.
 ----------------------------------------------------------------------------*/
CHAR c_cli_get_char_timeout(UINT32 ui4_time)
{
    return (x_cli_get_char_timeout(ui4_time));
}


/*-----------------------------------------------------------------------------
 * Name: c_cli_parse_dbg_level
 *
 * Description: This API parses a CLI command for debug level.
 *
 * Inputs:  i4_argc         Number of arguments.
 *          pps_argv        Points to the argument array.
 *
 * Outputs: pui2_dbg_lvl    The value of debug level after parsing.
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_CMD_EXEC_ERROR     Input password is incorrect.
 ----------------------------------------------------------------------------*/
INT32 c_cli_parse_dbg_level(INT32        i4_argc,
                            const CHAR** pps_argv,
                            UINT16*      pui2_dbg_level)
{
    return (x_cli_parse_dbg_level(i4_argc, pps_argv, pui2_dbg_level));
}


/*-----------------------------------------------------------------------------
 * Name: c_cli_show_dbg_level
 *
 * Description: This API display debug level in text format.
 *
 * Inputs:  ui2_dbg_lvl          The value of debug level to display.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
INT32 c_cli_show_dbg_level(UINT16 ui2_dbg_level)
{
    return (x_cli_show_dbg_level(ui2_dbg_level));
}

/*-----------------------------------------------------------------------------
 * Name: c_cli_parse_tms_level
 *
 * Description: This API parses a CLI command for time measurement level.
 *
 * Inputs:  i4_argc         Number of arguments.
 *          pps_argv        Points to the argument array.
 *
 * Outputs: pui2_tms_lvl    The value of time measurement level after parsing.
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_NOT_INIT           The CLI has not been initialized.
 *          CLIR_CMD_EXEC_ERROR     Input password is incorrect.
 *          CLIR_NOT_ENABLED        CLI is not enabled.
 ----------------------------------------------------------------------------*/
INT32 c_cli_parse_tms_level(INT32        i4_argc,
                            const CHAR** pps_argv,
                            UINT16*      pui2_tms_level)
{
    return x_cli_parse_tms_level(i4_argc, pps_argv, pui2_tms_level);
}

/*-----------------------------------------------------------------------------
 * Name: c_cli_show_tms_level
 *
 * Description: This API display time measurement level in text format.
 *
 * Inputs:  ui2_tms_lvl          The value of time measurement level to
 *                               display.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_NOT_INIT           The CLI has not been initialized.
 *          CLIR_NOT_ENABLED        CLI is not enabled.
 ----------------------------------------------------------------------------*/
INT32 c_cli_show_tms_level(UINT16 ui2_tms_level)
{
    return x_cli_show_tms_level(ui2_tms_level);
}


/*-----------------------------------------------------------------------------
 * Name: c_cli_ui_send_hotkeys
 *
 * Description: This API is used to trigger the hotkey callback functions.
 *
 * Inputs:  pui4_keys           Contains a hotkey array.
 *          ui2_key_len         The length of the hotkey array.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_NOT_INIT           The CLI sub-module has not been initialized.
 *          CLIR_INV_ARG            Invalid arguments.
 *          CLIR_NOT_ENABLED        CLI sub-module is not enabled.
 ----------------------------------------------------------------------------*/
INT32 c_cli_ui_send_hotkeys (UINT32* pui4_keys,
                             UINT16  ui2_key_len)
{
    return x_cli_ui_send_hotkeys(pui4_keys, ui2_key_len);
}

