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
 * $RCSfile: cli_alias.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/5 $
 * $SWAuthor: Clear Case Administrator $
 * $MD5HEX: c27e85e2bc952c5e63b5003c74ad583b $
 *
 * Description:
 *         This program will handle alias issue related to CLI.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "inc/x_common.h"
#include "dbg/x_dbg.h"
#include "cli/_cli.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* Alias element data structure */
typedef struct _ALIAS_ELEM_T
{
    CHAR        s_alias[CLI_MAX_ALIAS_LEN];
    CHAR        s_cmd[CLI_CMD_BUF_SIZE];
}   ALIAS_ELEM_T;

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
static ALIAS_ELEM_T at_cli_alias_tbl[CLI_MAX_ALIAS_NUM];
#endif

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: cli_alias_init
 *
 * Description: This API clears CLI alias table.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_NOT_ENABLED        CLI is not enabled.
 ----------------------------------------------------------------------------*/
INT32 cli_alias_init(VOID)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    UINT32 ui4_idx;

    for (ui4_idx = 0; ui4_idx < CLI_MAX_ALIAS_NUM; ui4_idx++)
    {
        at_cli_alias_tbl[ui4_idx].s_alias[0] = ASCII_NULL;
        at_cli_alias_tbl[ui4_idx].s_cmd[0] = ASCII_NULL;
    }

    return CLIR_OK;
#else
    return CLIR_NOT_ENABLED;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: cli_alias_attach
 *
 * Description: This API adds, deletes, or raplaces an alias element.
 *
 * Inputs:  ps_alias    Contains the alias to attach.
 *          ps_cmd      Contains the command string corresponding to the alias.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_ARG            One or more invalid arguments.
 *          CLIR_ALIAS_TOO_LONG     Alias is too long.
 *          CLIR_CMD_TOO_LONG       CLI command is too long.
 *          CLIR_ALIAS_TBL_FULL     CLI alias table is full.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 *          CLIR_NOT_ENABLED        CLI is not enabled.
 ----------------------------------------------------------------------------*/
INT32 cli_alias_attach(const CHAR* ps_alias,
                       const CHAR* ps_cmd)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    UINT32 ui4_idx;
    UINT32 ui4_alias_len;
    UINT32 ui4_cur_alias_size;
    UINT32 ui4_cmd_len;

    /* Check arguments */
    if ((ps_alias == NULL) ||
        (ps_cmd == NULL))
    {
        return CLIR_INV_ARG;
    }

    ui4_alias_len = x_strlen(ps_alias);
    if (ui4_alias_len >= CLI_MAX_ALIAS_LEN)
    {
        return CLIR_ALIAS_TOO_LONG;
    }

    ui4_cmd_len = x_strlen(ps_cmd);
    if (ui4_cmd_len >= CLI_CMD_BUF_SIZE)
    {
        return CLIR_CMD_TOO_LONG;
    }

    /* Replace or delete existed alias */
    for (ui4_idx = 0; ui4_idx < CLI_MAX_ALIAS_NUM; ui4_idx++)
    {
        if (at_cli_alias_tbl[ui4_idx].s_alias[0] != ASCII_NULL)
        {
            ui4_cur_alias_size = x_strlen(at_cli_alias_tbl[ui4_idx].s_alias);

            if ((ui4_alias_len == ui4_cur_alias_size) &&
                (x_strncmp(ps_alias, at_cli_alias_tbl[ui4_idx].s_alias, ui4_cur_alias_size) == 0))
            {
                if (ui4_cmd_len == 0) /* Delete alias */
                {
                    at_cli_alias_tbl[ui4_idx].s_alias[0] = ASCII_NULL;
                    at_cli_alias_tbl[ui4_idx].s_cmd[0] = ASCII_NULL;
                }
                else /* Replace alias */
                {
                    x_memset((VOID*)(at_cli_alias_tbl[ui4_idx].s_cmd),
                             ASCII_NULL,
                             CLI_CMD_BUF_SIZE);
                    x_strncpy(at_cli_alias_tbl[ui4_idx].s_cmd,
                              ps_cmd,
                              ui4_cmd_len);
                }

                return CLIR_OK;
            }
        }
    }

    /* Add new alias */
    if (ui4_cmd_len == 0)
    {
        return CLIR_OK;
    }

    for (ui4_idx = 0; ui4_idx < CLI_MAX_ALIAS_NUM; ui4_idx++)
    {
        if (at_cli_alias_tbl[ui4_idx].s_alias[0] == ASCII_NULL)
        {
            x_strncpy(at_cli_alias_tbl[ui4_idx].s_alias,
                      ps_alias, ui4_alias_len);
            x_strncpy(at_cli_alias_tbl[ui4_idx].s_cmd,
                      ps_cmd, ui4_cmd_len);

            return CLIR_OK;
        }
    }

    if (ui4_idx == CLI_MAX_ALIAS_NUM)
    {
        return CLIR_ALIAS_TBL_FULL;
    }
    else
    {
        return CLIR_CMD_EXEC_ERROR;
    }
#else
    return CLIR_NOT_ENABLED;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: cli_alias_search
 *
 * Description: This API searches CLI alias table for corresponding command
 *              string.
 *
 * Inputs:  ps_alias    Contains the alias to compare.
 *
 * Outputs: -
 *
 * Returns: NULL        The alias is not defined/somthing error.
 *          Others      The command string corresponds to the alias.
 ----------------------------------------------------------------------------*/
const CHAR* cli_alias_search(const CHAR* ps_alias)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    UINT32 ui4_idx;
    UINT32 ui4_alias_len;
    UINT32 ui4_cur_alias_size;

    /* Check arguments */
    if (ps_alias == NULL)
    {
        return NULL;
    }

    /* Search alias from CLI alias table */
    ui4_alias_len = x_strlen(ps_alias);
    for (ui4_idx = 0; ui4_idx < CLI_MAX_ALIAS_NUM; ui4_idx++)
    {
        if (at_cli_alias_tbl[ui4_idx].s_alias[0] != ASCII_NULL)
        {
            ui4_cur_alias_size = x_strlen(at_cli_alias_tbl[ui4_idx].s_alias);

            if ((ui4_alias_len == ui4_cur_alias_size) &&
                (x_strncmp(ps_alias, at_cli_alias_tbl[ui4_idx].s_alias, ui4_cur_alias_size) == 0))
            {
                return (at_cli_alias_tbl[ui4_idx].s_cmd);
            }
        }
    }

    /* No alias has been found */
    return NULL;
#else
    return NULL;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: cli_alias
 *
 * Description: CLI alias main function.
 *
 * Inputs:  i4_argc     Contains the argument count.
 *          pps_argv    Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_ALIAS_TOO_LONG     Alias is too long.
 *          CLIR_CMD_TOO_LONG       CLI command is too long.
 *          CLIR_NOT_ENABLED        CLI is not enabled.
 ----------------------------------------------------------------------------*/
INT32 cli_alias(INT32        i4_argc,
                const CHAR** pps_argv)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    UINT32  ui4_idx;
    UINT32  ui4_alias_len;
    UINT32  ui4_cmd_len;
    CHAR    s_cmd_buf[CLI_CMD_BUF_SIZE];

    /* alias [alias] [command] */

    /* Check arguments */
    if ((i4_argc == 1) ||
        (pps_argv == NULL) ||
        (pps_argv[1] == NULL))
    {
        for (ui4_idx = 0; ui4_idx < CLI_MAX_ALIAS_NUM; ui4_idx++)
        {
            if (at_cli_alias_tbl[ui4_idx].s_alias[0] != ASCII_NULL)
            {
                x_dbg_stmt("%s:\t%s\n\r", at_cli_alias_tbl[ui4_idx].s_alias, at_cli_alias_tbl[ui4_idx].s_cmd);
            }
        }

        return CLIR_OK;
    }

    /* Check alias length */
    ui4_alias_len = x_strlen(pps_argv[1]);
    if (ui4_alias_len >= CLI_MAX_ALIAS_LEN)
    {
        return CLIR_ALIAS_TOO_LONG;
    }

    /* Attach alias and corresponding command to CLI alias table */
    ui4_cmd_len = 0;
    s_cmd_buf[0] = ASCII_NULL;
    for (ui4_idx = 2; ui4_idx < (UINT32)i4_argc; ui4_idx++)
    {
        ui4_cmd_len += x_strlen(pps_argv[ui4_idx]) + 1;
        if (ui4_cmd_len >= CLI_CMD_BUF_SIZE)
        {
            return CLIR_CMD_TOO_LONG;
        }

        x_strncat(s_cmd_buf,
                  pps_argv[ui4_idx],
                  (x_strlen(pps_argv[ui4_idx]) + 1));
        x_strncat(s_cmd_buf,
                  " ",
                  2);
    }

    return (cli_alias_attach(pps_argv[1], s_cmd_buf));
#else
    return CLIR_NOT_ENABLED;
#endif
}

