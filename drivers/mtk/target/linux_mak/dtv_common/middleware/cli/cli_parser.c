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
 * $RCSfile: cli_parser.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/18 $
 * $SWAuthor: Clear Case Administrator $
 * $MD5HEX: b794305807e8df685f62004bee4502b0 $
 *
 * Description:
 *         This program will handle string parsing of user's input to CLI
 *         console.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "inc/x_common.h"
#include "dbg/dbg.h"
#include "dbg/x_dbg.h"
#include "start/x_version.h"
#include "cli/kbir/kbir.h"
#include "cli/x_cli.h"
#include "cli/_cli.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define HELP_STR_POS            ((UINT32)    24)
#define MAX_STR_LEN             ((UINT32)   128)

#define IS_PRINTABLE(c)         ((((UINT8)(c) > ASCII_NULL) && ((UINT8)(c) < ASCII_KEY_SPACE)) ? 0 : 1)
#define IS_SPACE(c)             (((c) == ' ' || (c) == '\n' || (c) == '\t' || (c) == '\r' || (c) == '\a') ? 1 : 0)
#define IS_DOT(c)               (((UINT8)(c) == ASCII_KEY_DOT) ? 1 : 0)
#define IS_ROOT(c)              (((UINT8)(c) == ASCII_KEY_ROOT) ? 1 : 0)

/* Definition of group info structure */
typedef struct _CLI_GRP_T
{
    CLI_EXEC_T*     pt_cmd_tbl;
    UINT64          aui8_grp_msk;
}   CLI_GRP_T;

/* Definition of category-to-category string structure */
typedef struct _CLI_CAT_N_STR_T
{
    UINT8       aui8_cat;
    CHAR*       ps_cat_str;
}   CLI_CAT_N_STR_T;

/* Definition of group mask-to-group string structure */
typedef struct _CLI_GRP_N_STR_T
{
    UINT64      aui8_grp_msk;
    CHAR*       ps_grp_str;
}   CLI_GRP_N_STR_T;

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
static CLI_EXEC_T*              apt_cli_array[CLI_CAT_MAX][CLI_MAX_CMD_TBL_NUM];
static UINT32                   aui4_cli_tbl_cnt[CLI_CAT_MAX];
static CLI_GRP_T                at_cli_mod_grp_info[CLI_CAT_MAX][CLI_MAX_CMD_TBL_NUM];
static CHAR                     as_argv[CLI_MAX_ARG_NUM][CLI_MAX_ARG_LEN];
static CHAR                     as_alias_argv[CLI_MAX_ARG_NUM][CLI_MAX_ARG_LEN];
static CLI_EXEC_T*              apt_dir_link[CLI_MAX_CMD_TBL_LEVEL];
static UINT32                   ui4_dir_link_idx;
static CLI_EXEC_T*              pt_cur_cmd_tbl;
static BOOL                     b_is_cmd_tbl;
static CHAR*                    ps_cli_prompt_str;

static CLI_ACCESS_RIGHT_T       e_access_right = CLI_ACCESS_RIGHT;


/* Access right */
static CHAR* ps_access_right[] =
{
    CLI_AR_SUPERVISOR_STR,
    CLI_AR_ADMIN_STR,
    CLI_AR_GUEST_STR
};

/* Category */
static CLI_CAT_N_STR_T at_cat_n_str[] =
{
    {CLI_CAT_ROOT,      CLI_CAT_ROOT_STR     },
    {CLI_CAT_BASIC,     CLI_CAT_BASIC_STR    },
    {CLI_CAT_APP,       CLI_CAT_APP_STR      },
    {CLI_CAT_MW,        CLI_CAT_MW_STR       },
    {CLI_CAT_DRV,       CLI_CAT_DRV_STR      },
    {CLI_CAT_TEST,      CLI_CAT_TEST_STR     },
    {CLI_CAT_MTK_TOOL,  CLI_CAT_MTK_TOOL_STR }        
};

/* Group */
static CLI_GRP_N_STR_T at_grp_n_str[] =
{
    {CLI_GRP_NONE,      CLI_NONE_STR     },
    {CLI_GRP_PIPE,      CLI_GRP_PIPE_STR },
    {CLI_GRP_GUI,       CLI_GRP_GUI_STR  },
    {CLI_GRP_EPG,       CLI_GRP_EPG_STR  },
    {CLI_GRP_DRV,       CLI_GRP_DRV_STR  }
};

static INT32 _cli_cmd_change_dir(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cli_cmd_list_cmd(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cli_cmd_repeat(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cli_cmd_set_access_right(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cli_cmd_get_access_right(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cli_cmd_set_grp_dbg_lvl(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cli_cmd_set_drv_grp_dbg_lvl(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cli_cmd_get_grp_vs_mod(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cli_cmd_get_mod_vs_grp(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cli_cmd_set_ctrl_stmt(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cli_cmd_show_ver_info(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _cli_cmd_set_log_info(INT32 i4_argc, const CHAR** pps_argv);

/* Mandatory command table */
static CLI_EXEC_T at_mandatory_cmd_tbl[] =
{
     {"cd",                  NULL,                   _cli_cmd_change_dir,            NULL,      "Change directory",                         CLI_HIDDEN},
     {"ls",                  NULL,                   _cli_cmd_list_cmd,              NULL,      "List commands",                            CLI_HIDDEN},
     {"do",                  NULL,                   _cli_cmd_repeat,                NULL,      "Repeat command",                           CLI_HIDDEN},
     {CLI_ALIAS_CMD_STR,     CLI_ALIAS_CMD_ABBR_STR, cli_alias,                      NULL,      "Show, add or remove alias",                CLI_HIDDEN},
     {"su",                  NULL,                   _cli_cmd_set_access_right,      NULL,      "Change access right",                      CLI_HIDDEN},
     {"whoami",              NULL,                   _cli_cmd_get_access_right,      NULL,      "Get access right",                         CLI_HIDDEN},
#ifndef ANDROID
     {"kbir",                NULL,                   kbir_main,                      NULL,      "Start keyboard IR key generator",          CLI_HIDDEN},
#endif
     /* For debug level control */
     {"sgdl",                NULL,                   _cli_cmd_set_grp_dbg_lvl,       NULL,      "Set non-driver module group debug level",  CLI_HIDDEN},
     {"gd_l",                NULL,                   _cli_cmd_set_drv_grp_dbg_lvl,   NULL,      "Set driver module group debug level",        CLI_HIDDEN},
     {"group",               NULL,                   _cli_cmd_get_grp_vs_mod,        NULL,      "Get group vs module info",                    CLI_HIDDEN},
     {"module",              NULL,                   _cli_cmd_get_mod_vs_grp,        NULL,      "Get module vs group info",                    CLI_HIDDEN},
     {"scs",                 NULL,                   _cli_cmd_set_ctrl_stmt,         NULL,      "Enable/disable debug control statement",   CLI_HIDDEN},
     {"ver",                 NULL,                   _cli_cmd_show_ver_info,         NULL,      "Show system version information",          CLI_HIDDEN},
     {"log",                 NULL,                   _cli_cmd_set_log_info,          NULL,      "Let module specify debug info message",     CLI_HIDDEN},
     END_OF_CLI_CMD_TBL
};

/* Basic command table */
static CLI_EXEC_T at_basic_cmd_tbl[] =
{
    {CLI_CAT_BASIC_STR,     CLI_CAT_BASIC_ABBR_STR,     NULL,       NULL,       "Basic",            CLI_GUEST},
    END_OF_CLI_CMD_TBL
};

/* Application command table */
static CLI_EXEC_T at_app_cmd_tbl[] =
{
    {CLI_CAT_APP_STR,       NULL,                       NULL,       NULL,       "Application",      CLI_GUEST},
    END_OF_CLI_CMD_TBL
};

/* Middleware command table */
static CLI_EXEC_T at_mw_cmd_tbl[] =
{
    {CLI_CAT_MW_STR,        NULL,                       NULL,       NULL,       "Middleware",       CLI_GUEST},
    END_OF_CLI_CMD_TBL
};

/* Driver command table */
static CLI_EXEC_T at_drv_cmd_tbl[] =
{
    {CLI_CAT_DRV_STR,       NULL,                       NULL,       NULL,       "Driver",           CLI_GUEST},
    END_OF_CLI_CMD_TBL
};

/* Test command table */
static CLI_EXEC_T at_test_cmd_tbl[] =
{
    {CLI_CAT_TEST_STR,     CLI_CAT_TEST_ABBR_STR,       NULL,       NULL,       "Test",             CLI_GUEST},
    END_OF_CLI_CMD_TBL
};

/* MTK tool command table */
static CLI_EXEC_T at_mtk_tool_cmd_tbl[] =
{
    {CLI_CAT_MTK_TOOL_STR,  CLI_CAT_MTK_TOOL_ABBR_STR,  NULL,       NULL,       "MTK tool",         CLI_GUEST},
    END_OF_CLI_CMD_TBL
};

#endif

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
static BOOL _is_cmd_tbl_array(CLI_EXEC_T* pt_tbl);

static VOID _get_cmd_tbl_array(CLI_EXEC_T*   pt_tbl,
                               BOOL          b_skip_manda_tbl,
                               UINT32*       pui4_tbl_cnt,
                               CLI_EXEC_T*** pppt_cmd_tbl_array);

static BOOL _format_help_str(CHAR*       ps_dst,
                             UINT32      ui4_dst_len,
                             const CHAR* ps_cmd_str,
                             const CHAR* ps_cmd_abbr_str,
                             const CHAR* ps_cmd_help_str);

static VOID _show_help(CLI_EXEC_T* pt_tbl);

static INT32 _find_cmd_argv(const CHAR*  ps_cmd,
                            UINT32       ui4_argv_num,
                            UINT32       ui4_argv_len,
                            CHAR* const* pps_argv);

static CLI_EXEC_T* _search_tbl(const CHAR* ps_cmd,
                               CLI_EXEC_T* pt_tbl);

static INT32 _parse_cmd(INT32        i4_argc,
                        const CHAR** pps_argv,
                        CLI_EXEC_T*  pt_tbl);

static VOID _generate_prompt(VOID);

/*-----------------------------------------------------------------------------
 * Name: _get_cmd_tbl_array
 *
 * Description: The API checks if input command table links to a command table
 *              array.
 *
 * Inputs:  pt_tbl      The command table to check.
 *
 * Outputs: -
 *
 * Returns: TRUE        This table links to a command table array.
 *          FALSE       This table doea not link to a command table array.
 ----------------------------------------------------------------------------*/
static BOOL _is_cmd_tbl_array(CLI_EXEC_T* pt_tbl)
{
    if ((pt_tbl == NULL) ||
        (x_strcmp(pt_tbl->ps_cmd_str, CLI_CAT_BASIC_STR) == 0 ) ||
        (x_strcmp(pt_tbl->ps_cmd_str, CLI_CAT_BASIC_ABBR_STR) == 0 ) ||
        (x_strcmp(pt_tbl->ps_cmd_str, CLI_CAT_APP_STR) == 0) ||
        (x_strcmp(pt_tbl->ps_cmd_str, CLI_CAT_MW_STR) == 0) ||
        (x_strcmp(pt_tbl->ps_cmd_str, CLI_CAT_DRV_STR) == 0) ||
        (x_strcmp(pt_tbl->ps_cmd_str, CLI_CAT_TEST_STR) == 0) ||
        (x_strcmp(pt_tbl->ps_cmd_str, CLI_CAT_MTK_TOOL_STR) == 0 ) ||
        (x_strcmp(pt_tbl->ps_cmd_str, CLI_CAT_MTK_TOOL_ABBR_STR) == 0 ))

    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*-----------------------------------------------------------------------------
 * Name: _get_cmd_tbl_array
 *
 * Description: The API returns current available command table array and its
 *              element count.
 *
 * Inputs:  pt_tbl                  The command table to reference.
 *          b_skip_manda_tbl        Indicates if skipping mandatory command
 *                                  table is required.
 *
 * Outputs: pui4_tbl_cnt            The command table element count.
 *          pppt_cmd_tbl_array      The command table array.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _get_cmd_tbl_array(CLI_EXEC_T*   pt_tbl,
                               BOOL          b_skip_manda_tbl,
                               UINT32*       pui4_tbl_cnt,
                               CLI_EXEC_T*** pppt_cmd_tbl_array)
{
    if (pt_tbl == NULL) /* Root */
    {
        if (b_skip_manda_tbl)
        {
            *pui4_tbl_cnt = aui4_cli_tbl_cnt[CLI_CAT_ROOT] - 1;
            *pppt_cmd_tbl_array = &(apt_cli_array[CLI_CAT_ROOT][CLI_MANDA_CMD_TBL_IDX + 1]);
        }
        else
        {
            *pui4_tbl_cnt = aui4_cli_tbl_cnt[CLI_CAT_ROOT];
            *pppt_cmd_tbl_array = &(apt_cli_array[CLI_CAT_ROOT][0]);
        }
    }
    else
    {
        if ((x_strcmp(pt_tbl->ps_cmd_str, CLI_CAT_BASIC_STR) == 0) ||
            (x_strcmp(pt_tbl->ps_cmd_str, CLI_CAT_BASIC_ABBR_STR) == 0))
        {
            *pui4_tbl_cnt = aui4_cli_tbl_cnt[CLI_CAT_BASIC];
            *pppt_cmd_tbl_array = &(apt_cli_array[CLI_CAT_BASIC][0]);
        }
        else if (x_strcmp(pt_tbl->ps_cmd_str, CLI_CAT_APP_STR) == 0)
        {
            *pui4_tbl_cnt = aui4_cli_tbl_cnt[CLI_CAT_APP];
            *pppt_cmd_tbl_array = &(apt_cli_array[CLI_CAT_APP][0]);
        }
        else if (x_strcmp(pt_tbl->ps_cmd_str, CLI_CAT_MW_STR) == 0)
        {
            *pui4_tbl_cnt = aui4_cli_tbl_cnt[CLI_CAT_MW];
            *pppt_cmd_tbl_array = &(apt_cli_array[CLI_CAT_MW][0]);
        }
        else if (x_strcmp(pt_tbl->ps_cmd_str, CLI_CAT_DRV_STR) == 0)
        {
            *pui4_tbl_cnt = aui4_cli_tbl_cnt[CLI_CAT_DRV];
            *pppt_cmd_tbl_array = &(apt_cli_array[CLI_CAT_DRV][0]);
        }
        else if (x_strcmp(pt_tbl->ps_cmd_str, CLI_CAT_TEST_STR) == 0)
        {
            *pui4_tbl_cnt = aui4_cli_tbl_cnt[CLI_CAT_TEST];
            *pppt_cmd_tbl_array = &(apt_cli_array[CLI_CAT_TEST][0]);
        }
        else if ((x_strcmp(pt_tbl->ps_cmd_str, CLI_CAT_MTK_TOOL_STR) == 0) ||
                 (x_strcmp(pt_tbl->ps_cmd_str, CLI_CAT_MTK_TOOL_ABBR_STR) == 0))
        {
            *pui4_tbl_cnt = aui4_cli_tbl_cnt[CLI_CAT_MTK_TOOL];
            *pppt_cmd_tbl_array = &(apt_cli_array[CLI_CAT_MTK_TOOL][0]);
        }               
        else
        {
            *pui4_tbl_cnt = 0;
            *pppt_cmd_tbl_array = NULL;
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: _format_help_str
 *
 * Description: The API formats help string for display.
 *
 * Inputs:  ui4_dst_len         Destination buffer size.
 *          ps_cmd_str          Command string.
 *          ps_cmd_abbr_str     Command abbreviation string.
 *          ps_cmd_help_str     Command description.
 *
 * Outputs: ps_dst              Destination buffer.
 *
 * Returns: TRUE                Routine successful.
 *          FALSE               Routine failed.
 ----------------------------------------------------------------------------*/
static BOOL _format_help_str(CHAR*       ps_dst,
                             UINT32      ui4_dst_len,
                             const CHAR* ps_cmd_str,
                             const CHAR* ps_cmd_abbr_str,
                             const CHAR* ps_cmd_help_str)
{
    UINT32      ui4_cmd_str_len;
    UINT32      ui4_cmd_abbr_str_len;
    UINT32      ui4_help_str_len;
    UINT32      ui4_idx;

    /* Check arguments */
    if (ps_cmd_str == NULL)
    {
        return FALSE;
    }
   
    x_memset(ps_dst, '\0', ui4_dst_len);

    /* Format => xxx(xxx):        xxxxxxxx */
    ui4_cmd_str_len = x_strlen(ps_cmd_str);
    ui4_cmd_abbr_str_len = x_strlen(ps_cmd_abbr_str) + 2; /* (xxx) */
    ui4_help_str_len = x_strlen(ps_cmd_help_str);

    /* Check if destination buffer size is big enough */
    if (ui4_cmd_str_len + ui4_cmd_abbr_str_len + ui4_help_str_len + 1 > ui4_dst_len)
    {
        return FALSE;
    }

    /* Format command and command abbreviation strings */
    if (ps_cmd_abbr_str != NULL)
    {
        x_sprintf(ps_dst, "%s(%s):", ps_cmd_str, ps_cmd_abbr_str);
    }
    else
    {
        x_sprintf(ps_dst, "%s:", ps_cmd_str);
    }

    /* Copy command help string */
    if (ps_cmd_help_str != NULL)
    {
        for (ui4_idx = x_strlen(ps_dst); ui4_idx < HELP_STR_POS; ui4_idx++)
        {
            ps_dst[ui4_idx] = ' ';
        }
        
        if (ui4_dst_len > HELP_STR_POS + 1)
        {   /*DTV0062626 The help str length + HELP_STR_POS overflow the dst_len*/
            x_strncpy(ps_dst + HELP_STR_POS, ps_cmd_help_str, ui4_dst_len - HELP_STR_POS - 1);
        }
    }

    return TRUE;
}


/*-----------------------------------------------------------------------------
 * Name: _show_help
 *
 * Description: This API shows CLI command table help strings.
 *
 * Inputs:  pt_tbl      The command table.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _show_help(CLI_EXEC_T* pt_tbl)
{
    UINT32          ui4_cmd_idx = 0;
    UINT32          ui4_tbl_cnt;
    CHAR            s_help[MAX_STR_LEN];
    CLI_EXEC_T*     pt_cmd_tbl;
    CLI_EXEC_T**    ppt_cmd_tbl_array;
    UINT32          ui4_idx;

    if (b_is_cmd_tbl) /* Commad table */
    {
        if (pt_tbl == NULL)
        {
            return;
        }

        x_dbg_stmt("[Help]\n\r");

        while (pt_tbl[ui4_cmd_idx].ps_cmd_help_str != NULL)
        {
            if (_format_help_str(s_help,
                                 MAX_STR_LEN,
                                 pt_tbl[ui4_cmd_idx].ps_cmd_str,
                                 pt_tbl[ui4_cmd_idx].ps_cmd_abbr_str,
                                 pt_tbl[ui4_cmd_idx].ps_cmd_help_str) == TRUE)
            {
                if (pt_tbl[ui4_cmd_idx].e_access_right >= e_access_right &&
                    pt_tbl[ui4_cmd_idx].e_access_right != CLI_HIDDEN)
                {
                    x_dbg_stmt("%s\n\r", s_help);
                }
            }

            ui4_cmd_idx++;
        }
    }
    else /* Command table array */
    {
        _get_cmd_tbl_array(pt_tbl, FALSE, &ui4_tbl_cnt, &ppt_cmd_tbl_array);

        if (ui4_tbl_cnt > 0)
        {
            x_dbg_stmt("[Help]\n\r");
        }

        for (ui4_idx = 0; ui4_idx < ui4_tbl_cnt; ui4_idx++)
        {
            ui4_cmd_idx = 0;
            pt_cmd_tbl = ppt_cmd_tbl_array[ui4_idx];

            while ((pt_cmd_tbl[ui4_cmd_idx].ps_cmd_str != NULL) ||
                   (pt_cmd_tbl[ui4_cmd_idx].ps_cmd_abbr_str != NULL) ||
                   (pt_cmd_tbl[ui4_cmd_idx].ps_cmd_help_str != NULL))
            {
                if (_format_help_str(s_help,
                                     MAX_STR_LEN,
                                     pt_cmd_tbl[ui4_cmd_idx].ps_cmd_str,
                                     pt_cmd_tbl[ui4_cmd_idx].ps_cmd_abbr_str,
                                     pt_cmd_tbl[ui4_cmd_idx].ps_cmd_help_str) == TRUE)
                {
                    if (pt_cmd_tbl[ui4_cmd_idx].e_access_right >= e_access_right &&
                        pt_cmd_tbl[ui4_cmd_idx].e_access_right != CLI_HIDDEN)
                    {
                        x_dbg_stmt("%s\n\r", s_help);
                    }
                }

                ui4_cmd_idx++;
            }
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: _find_cmd_argv
 *
 * Description: The API finds all the arguments of a command string and stores
 *              them into argument buffer.
 *
 * Inputs:  ps_cmd              Contains the command to be parse.
 *          ui4_argv_num        The maximum argument number that argument
 *                              buffer supports.
 *          ui4_argv_len        The maximum length per argument.
 *
 * Outputs: pps_argv            Points to argument buffer.
 *
 * Returns: Number of arguments found.
 ----------------------------------------------------------------------------*/
static INT32 _find_cmd_argv(const CHAR*  ps_cmd,
                            UINT32       ui4_argv_num,
                            UINT32       ui4_argv_len,
                            CHAR* const* pps_argv)
{
    INT32       i4_argc = 0;
    CHAR*       ps_argv;
    CHAR        c_char;
    UINT32      ui4_cmd_idx;
    UINT8       ui1_quote_state; /* 0: no quote found
                                    1: first single quote (') found
                                    2: first double quota (") found  */

    /* Check arguments */
    if ((ps_cmd == NULL) ||
        (ui4_argv_num == 0) ||
        (ui4_argv_len == 0) ||
        (pps_argv == NULL))
    {
        return 0;
    }

    /* Start finding arguments of a command */
    c_char = *ps_cmd;
    while (((UINT8)c_char != ASCII_NULL) &&
           ((UINT8)c_char != ASCII_KEY_PUNCH))
    {
        ps_argv = pps_argv[i4_argc];

        /* Search the first non-space and printable character */
        while (!IS_PRINTABLE(c_char) || IS_SPACE(c_char))
        {
            c_char = *(++ps_cmd);
        }

        /* Grab an argument */
        ui4_cmd_idx = 0;
        ui1_quote_state = 0;
        while (IS_PRINTABLE(c_char) &&
               ((ui1_quote_state == 0 && !IS_SPACE(c_char)) ||
                (ui1_quote_state == 1 && (UINT8)c_char != ASCII_KEY_SGL_QUOTE) ||
                (ui1_quote_state == 2 && (UINT8)c_char != ASCII_KEY_DBL_QUOTE)))
        {
            if (c_char == ASCII_NULL ||             /* End of string */
                ui4_cmd_idx >= (ui4_argv_len - 1))    /* Exceed maximum argument length */
            {
                *ps_argv = ASCII_NULL;
                break;
            }

            /* Take care of quote issue */
            if ((ui1_quote_state != 2) &&
                ((UINT8)c_char == ASCII_KEY_SGL_QUOTE))
            {
                if (ui1_quote_state == 0)
                {
                    ui1_quote_state = 1;
                }
                else if (ui1_quote_state == 1)
                {
                    ui1_quote_state = 0;
                }
                c_char = *(++ps_cmd);
                ui4_cmd_idx++;
                continue;
            }
            else if ((ui1_quote_state != 1) &&
                     ((UINT8)c_char == ASCII_KEY_DBL_QUOTE))
            {
                if (ui1_quote_state == 0)
                {
                    ui1_quote_state = 2;
                }
                else if (ui1_quote_state == 2)
                {
                    ui1_quote_state = 0;
                }
                c_char = *(++ps_cmd);
                ui4_cmd_idx++;
                continue;
            }

            /* Copy the character into argument buffer */
            *ps_argv = c_char;
            ps_argv++;
            c_char = *(++ps_cmd);
            ui4_cmd_idx++;
        }

        if (((UINT8)c_char == ASCII_KEY_SGL_QUOTE) ||
            ((UINT8)c_char == ASCII_KEY_DBL_QUOTE))
        {
            c_char = *(++ps_cmd);
            ui4_cmd_idx++;
        }

        if (ui4_cmd_idx > 0)
        {
            *ps_argv = ASCII_NULL;
            i4_argc++;
        }

        if ((UINT32)i4_argc >= ui4_argv_num)
        {
            break;
        }
    }

    return i4_argc;
}


/*-----------------------------------------------------------------------------
 * Name: _search_tbl
 *
 * Description: This API finds out a command table corresponding to the
 *              command.
 *
 * Inputs:  ps_cmd      The command to search.
 *          pt_tbl      The command table for search.
 *
 * Outputs: -
 *
 * Returns: NULL        No matched command table found.
 *          Other       The address of matched command table.
 ----------------------------------------------------------------------------*/
static CLI_EXEC_T* _search_tbl(const CHAR* ps_cmd,
                               CLI_EXEC_T* pt_tbl)
{
    const CHAR*     ps_dir_str;
    UINT32          ui4_dir_str_len;
    UINT32          ui4_cmd_len;
    UINT32          ui4_cmd_abbr_len;
    UINT32          ui4_cmd_idx;
    BOOL            b_found = FALSE;

    /* Check arguments */
    if ((ps_cmd == NULL) ||
        (pt_tbl == NULL))
    {
        return NULL;
    }

    ui4_dir_str_len = 0;
    ps_dir_str = ps_cmd;
    while ((!IS_DOT(*ps_dir_str)) &&
           (*ps_dir_str != ASCII_NULL))
    {
        ui4_dir_str_len++;
        ps_dir_str++;
    }

    /* Search commmad from current command table */
    ui4_cmd_idx = 0;
    while (pt_tbl[ui4_cmd_idx].ps_cmd_str != NULL)
    {
        /* Compare command string */
        ui4_cmd_len = x_strlen(pt_tbl[ui4_cmd_idx].ps_cmd_str);
        if ((ui4_dir_str_len == ui4_cmd_len) &&
            (x_strncmp(pt_tbl[ui4_cmd_idx].ps_cmd_str, ps_cmd, ui4_dir_str_len) == 0))
        {
            b_found = TRUE;
            break;
        }

        /* Compare command abbreviation string */
        if (pt_tbl[ui4_cmd_idx].ps_cmd_abbr_str != NULL)
        {
            ui4_cmd_abbr_len = x_strlen(pt_tbl[ui4_cmd_idx].ps_cmd_abbr_str);

            if ((ui4_dir_str_len == ui4_cmd_abbr_len) &&
                (x_strncmp(pt_tbl[ui4_cmd_idx].ps_cmd_abbr_str, ps_cmd, ui4_dir_str_len) == 0))
            {
                b_found = TRUE;
                break;
            }
        }

        ui4_cmd_idx++;
    }

    /* Return the matched command structure */
    if (b_found)
    {
        return &pt_tbl[ui4_cmd_idx];
    }

    return NULL;
}


/*-----------------------------------------------------------------------------
 * Name: _parse_cmd
 *
 * Description: This API parses and executes a command.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *          pt_tbl          The command table to start parsing.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_INV_ARG            One or more invalid arguments.
 *          CLIR_INV_CMD_TBL        Invalid CLI command table format.
 *          CLIR_CMD_NOT_FOUND      CLI command not found.
 *          CLIR_DIR_NOT_FOUND      CLI directory not found.
 ----------------------------------------------------------------------------*/
static INT32 _parse_cmd(INT32        i4_argc,
                        const CHAR** pps_argv,
                        CLI_EXEC_T*  pt_tbl)
{
    UINT32          ui4_idx;
    const CHAR*     ps_dir_str;
    CLI_EXEC_T*     pt_cmd_tbl = NULL;
    UINT32          ui4_tbl_cnt;
    CLI_EXEC_T**    ppt_cmd_tbl_array;
    BOOL            b_cmd_tbl_orig;

    /* Check arguments */
    if (pps_argv == NULL)
    {
        return CLIR_INV_ARG;
    }

    ps_dir_str = pps_argv[0];
    while ((!IS_DOT(*ps_dir_str)) &&
           (*ps_dir_str != ASCII_NULL))
    {
        ps_dir_str++;
    }

    if (_is_cmd_tbl_array(pt_tbl) == TRUE)
    {
        _get_cmd_tbl_array(pt_tbl, FALSE, &ui4_tbl_cnt, &ppt_cmd_tbl_array);

        for (ui4_idx = 0; ui4_idx < ui4_tbl_cnt; ui4_idx++)
        {
            pt_cmd_tbl = _search_tbl(pps_argv[0], ppt_cmd_tbl_array[ui4_idx]);

            if (pt_cmd_tbl != NULL)
            {
                break;
            }
        }
    }
    else
    {
        pt_cmd_tbl = _search_tbl(pps_argv[0], pt_tbl);
    }

    /* Execute command */
    if ((pt_cmd_tbl != NULL) &&
        (pt_cmd_tbl->e_access_right >= e_access_right))
    {
         if ((pt_cmd_tbl->pt_next_level == NULL) &&
             (pt_cmd_tbl->pf_exec_fct == NULL))
         {
            if (_is_cmd_tbl_array(pt_cmd_tbl) != TRUE)
            {
                return CLIR_INV_CMD_TBL;
            }
         }

        if (pt_cmd_tbl->pf_exec_fct != NULL)
        {
            if ((pt_cmd_tbl->pt_next_level == NULL) ||
                (!IS_DOT(*ps_dir_str) && (i4_argc > 1))) /* Entry can be a directory or a command */
            {
                return (pt_cmd_tbl->pf_exec_fct(i4_argc,
                                                pps_argv));
            }
        }

         if (pt_cmd_tbl->pt_next_level != NULL)
        {
            if (*ps_dir_str == ASCII_NULL)
            {
                /* Show CLI help if command contains only directory or module name */
                if (i4_argc == 1)
                {
                    if (_is_cmd_tbl_array(pt_cmd_tbl) == TRUE)
                    {
                        _show_help(pt_cmd_tbl);
                    }
                    else
                    {
                        b_cmd_tbl_orig = b_is_cmd_tbl;
                        b_is_cmd_tbl = TRUE;
                        _show_help(pt_cmd_tbl->pt_next_level);
                        b_is_cmd_tbl = b_cmd_tbl_orig;
                    }

                    return CLIR_INV_CMD_USAGE;
                }
                else
                {
                    return CLIR_CMD_NOT_FOUND;
                }
            }

            pps_argv[0] = ++ps_dir_str; /* Skip '.' character */

            /* Go to next level */
            if (_is_cmd_tbl_array(pt_cmd_tbl) == TRUE)
            {
                return (_parse_cmd(i4_argc, pps_argv, pt_cmd_tbl));
            }
            else
            {
                return (_parse_cmd(i4_argc, pps_argv, pt_cmd_tbl->pt_next_level));
            }
        }
    }

    return CLIR_CMD_NOT_FOUND;
}


/*-----------------------------------------------------------------------------
 * Name: _generate_prompt
 *
 * Description: This API generates CLI prompt string according to current
 *              directory level.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _generate_prompt(VOID)
{
    UINT32      ui4_total_len;
    UINT32      ui4_dir_str_len;
    CHAR*       ps_dir_str;
    UINT32      ui4_idx;

    ps_cli_prompt_str = cli_get_prompt_str_buf();
    ui4_total_len = sizeof(CLI_PROMPT_STR) + 1;
    ps_cli_prompt_str[0] = ASCII_NULL;
    x_strncat(ps_cli_prompt_str, CLI_PROMPT_STR, sizeof(CLI_PROMPT_STR));

    for (ui4_idx = 1; ui4_idx <= ui4_dir_link_idx; ui4_idx++)
    {
        if (apt_dir_link[ui4_idx]->ps_cmd_abbr_str != NULL)
        {
            ps_dir_str = apt_dir_link[ui4_idx]->ps_cmd_abbr_str;
        }
        else
        {
            ps_dir_str = apt_dir_link[ui4_idx]->ps_cmd_str;
        }

        ui4_dir_str_len = x_strlen(ps_dir_str);

        if (ui4_total_len + ui4_dir_str_len > CLI_CMD_BUF_SIZE)
        {
            x_strncat(ps_cli_prompt_str, ">", 1);
            return;
        }

        x_strncat(ps_cli_prompt_str, ".", 1);
        x_strncat(ps_cli_prompt_str, ps_dir_str, (ui4_dir_str_len + 1));
        ui4_total_len += (ui4_dir_str_len + 1); /* .xxx */
    }

    x_strncat(ps_cli_prompt_str, ">", 1);
}


/*-----------------------------------------------------------------------------
 * Name: _cli_cmd_change_dir
 *
 * Description: This API changes current command level.
 *
 * Inputs:  i4_argc     Number of arguments.
 *          pps_argv    Points to the argument array.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _cli_cmd_change_dir(INT32        i4_argc,
                                 const CHAR** pps_argv)
{
    const CHAR*     ps_dir_str;
    const CHAR*     ps_cur_dir;
    CLI_EXEC_T*     pt_tbl;
    UINT32          ui4_tbl_cnt;
    CLI_EXEC_T**    ppt_cmd_tbl_array;
    UINT32          ui4_idx;

    /* cd [directory path] */

    /* Check arguments */
    if ((i4_argc != 2) ||
        (pps_argv == NULL) ||
        (pps_argv[1] == NULL))
    {
        _show_help(pt_cur_cmd_tbl);
        return CLIR_INV_CMD_USAGE;
    }

    ps_dir_str = pps_argv[1];
    while (*ps_dir_str != ASCII_NULL)
    {
        if (IS_DOT(*ps_dir_str))
        {
            ps_dir_str++;
            if (IS_DOT(*ps_dir_str)) /* cd .. */
            {
                ps_dir_str++;
                if (ui4_dir_link_idx > 0)
                {
                    ui4_dir_link_idx--;

                    if (ui4_dir_link_idx == 0)
                    {
                        pt_cur_cmd_tbl = NULL;
                        b_is_cmd_tbl = FALSE;
                    }
                    else
                    {
                        /* Check if currect directory points to a command
                           table or command table array */
                        pt_tbl = apt_dir_link[ui4_dir_link_idx];
                        if (_is_cmd_tbl_array(pt_tbl) == TRUE)
                        {
                            pt_cur_cmd_tbl = pt_tbl;
                            b_is_cmd_tbl = FALSE;
                        }
                        else
                        {
                            pt_cur_cmd_tbl = pt_tbl->pt_next_level;
                            b_is_cmd_tbl = TRUE;
                           }
                    }
                    _generate_prompt();
                }
            }
            else
            {
                return CLIR_DIR_NOT_FOUND;
            }
         }
        else
        {
            if (IS_ROOT(*ps_dir_str)) /* cd / */
            {
                ps_dir_str++;
                ui4_dir_link_idx = 0;
                pt_cur_cmd_tbl = NULL;
                b_is_cmd_tbl = FALSE;
                _generate_prompt();
            }
            else /* cd xxx.xxx.xxx... */
            {
                ps_cur_dir = ps_dir_str;
                if (b_is_cmd_tbl) /* Command table */
                {
                    pt_tbl = _search_tbl(ps_cur_dir, pt_cur_cmd_tbl);
                }
                else /* Command table array */
                {
                    pt_tbl = NULL;
                    _get_cmd_tbl_array(pt_cur_cmd_tbl, TRUE, &ui4_tbl_cnt, &ppt_cmd_tbl_array);

                    for (ui4_idx = 0; ui4_idx < ui4_tbl_cnt; ui4_idx++)
                    {
                        pt_tbl = _search_tbl(ps_cur_dir, ppt_cmd_tbl_array[ui4_idx]);

                        if (pt_tbl != NULL)
                        {
                            break;
                        }
                    }
                }

                if ((pt_tbl != NULL) &&
                    ((b_is_cmd_tbl == TRUE && pt_tbl->pt_next_level != NULL) || b_is_cmd_tbl == FALSE) &&
                    (pt_tbl->e_access_right >= e_access_right))
                {
                    if (ui4_dir_link_idx < (CLI_MAX_CMD_TBL_LEVEL - 1))
                    {
                        /* Print out current directory path */
                        ui4_dir_link_idx++;
                        apt_dir_link[ui4_dir_link_idx] = pt_tbl;
                        _generate_prompt();

                        /* Check if currect directory points to a command
                           table or command table array */
                        if (_is_cmd_tbl_array(pt_tbl) == TRUE)
                        {
                            pt_cur_cmd_tbl = pt_tbl;
                            b_is_cmd_tbl = FALSE;
                        }
                        else
                        {
                            pt_cur_cmd_tbl = pt_tbl->pt_next_level;
                            b_is_cmd_tbl = TRUE;
                           }
                    }
                    else
                    {
                        /* There is something wrong => change directory back to root */
                        ui4_dir_link_idx = 0;
                        pt_cur_cmd_tbl = NULL;
                        b_is_cmd_tbl = FALSE;
                        _generate_prompt();
                    }
                }
                else
                {
                    return CLIR_DIR_NOT_FOUND;
                }

                /* Proceed next level directory path */
                while (!IS_DOT(*ps_dir_str) && (*ps_dir_str != ASCII_NULL))
                {
                    ps_dir_str++;
                }

                /* Skip '.' character */
                if (IS_DOT(*ps_dir_str))
                {
                    ps_dir_str++;
                }
            }
        }
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cli_cmd_list_cmd
 *
 * Description: This API list commands at current command level.
 *
 * Inputs:  i4_argc     Number of arguments.
 *          pps_argv    Points to the argument array.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _cli_cmd_list_cmd(INT32        i4_argc,
                               const CHAR** pps_argv)
{
    /* ls */

    _show_help(pt_cur_cmd_tbl);
    
    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cli_cmd_repeat
 *
 * Description: This API executes a CLI command for a specific loop.
 *
 * Inputs:  i4_argc     Number of arguments.
 *          pps_argv    Points to the argument array.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _cli_cmd_repeat(INT32        i4_argc,
                             const CHAR** pps_argv)
{
    INT32       i4_return;
    HANDLE_T*   ph_cli_ctrl_b_msgq;
    UINT16      ui2_idx;
    UINT8       ui1_data;
    SIZE_T      z_size;
    UINT32      ui4_idx;
    UINT32      ui4_cmd_str_len;
    UINT32      ui4_loop = 0;
    UINT32      ui4_delay = 0;
    BOOL        b_loop_found = FALSE;
    BOOL        b_delay_found = FALSE;
    CHAR        s_cmd_buf[CLI_CMD_BUF_SIZE];

    /* do [-d] [loop] [cmd] */

    /* Check arguments */
    if ((i4_argc < 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("do [-d] [loop] [cmd]\teg: do -d 1000 10 read 0x200 0x10\n\r");
        x_dbg_stmt("  [-d]: Delay between two executions (ms)\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    /* Get the CLI message queue handle */
    ph_cli_ctrl_b_msgq = cli_get_ctrl_b_msgq_hdl();

    /* Get the real command for loop run */
    ui4_cmd_str_len = 0;
    s_cmd_buf[0] = ASCII_NULL;
    for (ui4_idx = 1; ui4_idx < (UINT32) i4_argc; ui4_idx++)
    {
        /* Check if "-d" is specified */
        if (!b_delay_found &&
            (x_strlen(pps_argv[ui4_idx]) == 2) &&
            (x_strncmp(pps_argv[ui4_idx], "-d", 2) == 0))
        {
            ui4_delay = (UINT32)(x_strtoull(pps_argv[ui4_idx + 1], NULL, 0));
            ui4_idx = ui4_idx + 1;
            b_delay_found = TRUE;
            continue;
        }
        else if (!b_loop_found) /* Check loop count */
        {
            ui4_loop = (UINT32)(x_strtoull(pps_argv[ui4_idx], NULL, 0));
            b_loop_found = TRUE;
            continue;
        }

        /* Otherwise, it is treated as partial command string */
        ui4_cmd_str_len += x_strlen(pps_argv[ui4_idx]) + 1;

        if (ui4_cmd_str_len >= CLI_CMD_BUF_SIZE)
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

    /* Execute the command */
    for (ui4_idx = 0; ui4_idx < ui4_loop; ui4_idx++)
    {
        if (ui4_idx > 0)
        {
            x_dbg_stmt("\n\r");
        }

        x_dbg_stmt("[Loop %d of %d]\n\r", ui4_idx + 1, ui4_loop);

        i4_return = cli_parser(s_cmd_buf);
        if (i4_return != CLIR_OK)
        {
            return i4_return;
        }

        z_size = sizeof(UINT8);
        i4_return = x_msg_q_receive(&ui2_idx,
                                    &ui1_data,
                                    &z_size,
                                    ph_cli_ctrl_b_msgq,
                                    1,
                                    X_MSGQ_OPTION_NOWAIT);

        if ((i4_return == OSR_OK) &&
            (ui1_data == ASCII_KEY_CTRL_B))
        {
            x_dbg_stmt("Interrupt!!!\n\r");
            return CLIR_OK;
        }

        if (b_delay_found &&
            (ui4_idx < ui4_loop - 1))
        {
            x_thread_delay(ui4_delay);
        }
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cli_cmd_set_access_right
 *
 * Description: This API changes system access right.
 *
 * Inputs:  i4_argc     Number of arguments.
 *          pps_argv    Points to the argument array.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _cli_cmd_set_access_right(INT32        i4_argc,
                                       const CHAR** pps_argv)
{
    UINT32  ui4_idx = 0;
    UINT32  ui4_cur_access_right_str_len;
    UINT32  ui4_new_access_right_str_len;

    /* su [access right] */

    /* Check arguments */
    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("su [access right]\teg: su admin\n\r");
        x_dbg_stmt("  [access right]: ");
        for (ui4_idx = 0; ui4_idx < CLI_HIDDEN; ui4_idx++)
        {
            x_dbg_stmt("%s", ps_access_right[ui4_idx]);
            if (ui4_idx < (UINT32)(CLI_HIDDEN - 1))
            {
                x_dbg_stmt("/");
            }
        }
        x_dbg_stmt("\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    /* Check new input access right */
    if (x_strncmp(pps_argv[1], ps_access_right[e_access_right],
                  x_strlen(ps_access_right[e_access_right])) == 0)
    {
        x_dbg_stmt("Same access right!\n\r");
    }
    else
    {
        ui4_new_access_right_str_len = x_strlen(pps_argv[1]);

        for (ui4_idx = 0; ui4_idx < CLI_HIDDEN; ui4_idx++)
        {
            ui4_cur_access_right_str_len = x_strlen(ps_access_right[ui4_idx]);

            if ((ui4_new_access_right_str_len == ui4_cur_access_right_str_len) &&
                (x_strncmp(pps_argv[1], ps_access_right[ui4_idx], ui4_cur_access_right_str_len) == 0))
            {
                if (x_cli_passwd(CLI_PASSWD_TWO_DIGITS_SUM_DEC) == CLIR_OK)
                {
                    e_access_right = (CLI_ACCESS_RIGHT_T)(ui4_idx);
                    x_dbg_stmt("\n\rAccess right change successful!\n\r");
                }
                break;
            }
        }
    }

    if (ui4_idx == CLI_HIDDEN)
    {
        x_dbg_stmt("su [access right]\teg: su admin\n\r");
        x_dbg_stmt("  [access right]: ");
        for (ui4_idx = 0; ui4_idx < CLI_HIDDEN; ui4_idx++)
        {
            x_dbg_stmt("%s", ps_access_right[ui4_idx]);
            if (ui4_idx < (UINT32)(CLI_HIDDEN - 1))
            {
                x_dbg_stmt("/");
            }
        }
        x_dbg_stmt("\n\r");
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cli_cmd_get_access_right
 *
 * Description: This API gets current system access right.
 *
 * Inputs:  i4_argc     Number of arguments.
 *          pps_argv    Points to the argument array.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _cli_cmd_get_access_right(INT32        i4_argc,
                                       const CHAR** pps_argv)
{
    /* whoami */

    /* Check arguments */
    if ((i4_argc != 1) ||
        (pps_argv == NULL))
    {
        _show_help(pt_cur_cmd_tbl);
        return CLIR_OK;
    }

    x_dbg_stmt("%s\n\r", ps_access_right[e_access_right]);
    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cli_cmd_set_grp_dbg_lvl
 *
 * Description: This API sets non-driver module group debug level.
 *
 * Inputs:  i4_argc     Number of arguments.
 *          pps_argv    Points to the argument array.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _cli_cmd_set_grp_dbg_lvl(INT32        i4_argc,
                                      const CHAR** pps_argv)
{
    const CHAR*     ps_dbg_level;
    CHAR*           ps_sdl_argv[2];
    CHAR            as_sdl_argv[2][CLI_MAX_ARG_LEN];
    UINT32          ui4_max_grp_num = 0;
    CLI_EXEC_T*     pt_cmd_tbl;
    CLI_EXEC_T*     pt_sdl_cmd_tbl;
    UINT64          aui8_grp_msk;
    UINT32          ui4_i;
    UINT32          ui4_j;
    UINT32          ui4_k;
    UINT32          ui4_l;
    BOOL            b_invalid_arg = FALSE;

    /* sgdl [group] [level] / sgdl all [level] */

    /* Check arguments */
    if ((i4_argc != 3) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("sgdl [group] [level] / sgdl all [level]\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    ps_dbg_level = pps_argv[2];

    if (((x_strchr(ps_dbg_level, 'n') != NULL) || (x_strchr(ps_dbg_level, 'N') != NULL)) &&
        (x_strlen(ps_dbg_level) > 1))
    {
        x_dbg_stmt("sgdl [group] [level] / sgdl all\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    /* Init sdl argument buffer */
    for (ui4_i = 0; ui4_i < 2; ui4_i++)
    {
        ps_sdl_argv[ui4_i] = as_sdl_argv[ui4_i];
        as_sdl_argv[ui4_i][0] = 0;
    }

    x_strcpy(ps_sdl_argv[0], CLI_SET_DBG_LVL_STR);

    /* Convert the debug level */
    while (*ps_dbg_level != 0)
    {
        switch (*ps_dbg_level)
        {
            case 'e':
            case 'E':
                x_strcat(ps_sdl_argv[1], "e");
                break;

            case 'a':
            case 'A':
                x_strcat(ps_sdl_argv[1], "a");
                break;

            case 'i':
            case 'I':
                x_strcat(ps_sdl_argv[1], "i");
                break;

            case 'n':
            case 'N':
                x_strcat(ps_sdl_argv[1], "n");
                break;

            default:
                b_invalid_arg = TRUE;
                break;
        }

        ps_dbg_level++;
    }

    if (b_invalid_arg)
    {
        return CLIR_INV_ARG;
    }

    if (x_strcmp(CLI_ALL_STR, pps_argv[1]) == 0) /* All */
    {
        for (ui4_i = CLI_CAT_ROOT; ui4_i < CLI_CAT_MAX; ui4_i++) /* Category */
        {
            if ((ui4_i == CLI_CAT_DRV) ||   /* Skip drv and test categories */
                (ui4_i == CLI_CAT_TEST))
            {
                continue;
            }

            for (ui4_j = (ui4_i == CLI_CAT_ROOT ? CLI_CAT_MAX : 0); ui4_j < CLI_MAX_CMD_TBL_NUM; ui4_j++)
            {
                pt_cmd_tbl = at_cli_mod_grp_info[ui4_i][ui4_j].pt_cmd_tbl;
                if (pt_cmd_tbl == NULL)
                {
                    continue;
                }

                ui4_k = 0;
                while (pt_cmd_tbl[ui4_k].ps_cmd_str != NULL) /* Module */
                {
                    if (pt_cmd_tbl[ui4_k].pt_next_level != NULL)
                    {
                        ui4_l = 0;
                        pt_sdl_cmd_tbl = pt_cmd_tbl[ui4_k].pt_next_level;
                        while (pt_sdl_cmd_tbl[ui4_l].ps_cmd_str != NULL) /* "sdl" command table */
                        {
                            if (x_strcmp(pt_sdl_cmd_tbl[ui4_l].ps_cmd_str, CLI_SET_DBG_LVL_STR) == 0 &&
                                pt_sdl_cmd_tbl[ui4_l].pf_exec_fct != NULL)
                            {
                                if (pt_sdl_cmd_tbl[ui4_l].pf_exec_fct(2, (const CHAR**)&ps_sdl_argv) != CLIR_OK)
                                {
                                    x_dbg_stmt("Set %s debug level failed\n\r", pt_cmd_tbl[ui4_k].ps_cmd_str);
                                }
                                break;
                            }

                            ui4_l++;
                        }
                    }

                    ui4_k++;
                }
            }
        }
    }
    else /* Specific groups */
    {
        /* Calculate the maximum group number */
        for (ui4_i = 1; ui4_i <= sizeof(CLI_GRP_MAX); ui4_i++)
        {
            ui4_max_grp_num++;

            if ((CLI_GRP_MAX >> ui4_i) == 0)
            {
                break;
            }
        }

        /* Retrieve group mask */
        aui8_grp_msk = CLI_GRP_NONE;
        for (ui4_i = 0; ui4_i < ui4_max_grp_num; ui4_i++)
        {
            if (x_strcmp(pps_argv[1], at_grp_n_str[ui4_i].ps_grp_str) == 0)
            {
                aui8_grp_msk = at_grp_n_str[ui4_i].aui8_grp_msk;
                break;
            }
        }

        if (ui4_i == ui4_max_grp_num)
        {
            x_dbg_stmt("sgdl [group] [level] / sgdl all\n\r");
            return CLIR_INV_CMD_USAGE;
        }

        for (ui4_i = CLI_CAT_ROOT; ui4_i < CLI_CAT_MAX; ui4_i++) /* Category */
        {
            if ((ui4_i == CLI_CAT_DRV) ||   /* Skip drv and test categories */
                (ui4_i == CLI_CAT_TEST))
            {
                continue;
            }

            for (ui4_j = (ui4_i == CLI_CAT_ROOT ? CLI_CAT_MAX : 0); ui4_j < CLI_MAX_CMD_TBL_NUM; ui4_j++)
            {
                pt_cmd_tbl = at_cli_mod_grp_info[ui4_i][ui4_j].pt_cmd_tbl;
                if (pt_cmd_tbl == NULL)
                {
                    break;
                }

                if (((at_cli_mod_grp_info[ui4_i][ui4_j].aui8_grp_msk & aui8_grp_msk) > 0) ||
                    (aui8_grp_msk == (at_cli_mod_grp_info[ui4_i][ui4_j].aui8_grp_msk)))
                {
                    ui4_k = 0;
                    while (pt_cmd_tbl[ui4_k].ps_cmd_str != NULL) /* Module */
                    {
                        if (pt_cmd_tbl[ui4_k].pt_next_level != NULL)
                        {
                            ui4_l = 0;
                            pt_sdl_cmd_tbl = pt_cmd_tbl[ui4_k].pt_next_level;
                            while (pt_sdl_cmd_tbl[ui4_l].ps_cmd_str != NULL) /* "sdl" command table */
                            {
                                if (x_strcmp(pt_sdl_cmd_tbl[ui4_l].ps_cmd_str, CLI_SET_DBG_LVL_STR) == 0 &&
                                    pt_sdl_cmd_tbl[ui4_l].pf_exec_fct != NULL)
                                {
                                    if (pt_sdl_cmd_tbl[ui4_l].pf_exec_fct(2, (const CHAR**)&ps_sdl_argv) != CLIR_OK)
                                    {
                                        x_dbg_stmt("Set %s debug level failed\n\r", pt_cmd_tbl[ui4_k].ps_cmd_str);
                                    }
                                    break;
                                }

                                ui4_l++;
                            }
                        }

                        ui4_k++;
                    }
                }
            }
        }
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cli_cmd_set_drv_grp_dbg_lvl
 *
 * Description: This API sets driver module group debug level.
 *
 * Inputs:  i4_argc     Number of arguments.
 *          pps_argv    Points to the argument array.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _cli_cmd_set_drv_grp_dbg_lvl(INT32           i4_argc,
                                          const CHAR**    pps_argv)
{
    INT64           i8_dbg_level;
    CHAR*           ps_d_l_argv[2];
    CHAR            as_d_l_argv[2][CLI_MAX_ARG_LEN];
    CLI_EXEC_T*     pt_cmd_tbl;
    CLI_EXEC_T*     pt_d_l_cmd_tbl;
    UINT32          ui4_i;
    UINT32          ui4_j;
    UINT32          ui4_k;

    /* gd_l [level] */

    /* Check arguments */
    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("gd_l [level]\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    /* Init d_l argument buffer */
    for (ui4_i = 0; ui4_i < 2; ui4_i++)
    {
        ps_d_l_argv[ui4_i] = as_d_l_argv[ui4_i];
        as_d_l_argv[ui4_i][0] = 0;
    }

    x_strcpy(ps_d_l_argv[0], CLI_D_L_STR);

    /* Convert the debug level */
    i8_dbg_level = (INT64)x_strtoull(pps_argv[1], NULL, 0);
    if ((i8_dbg_level >= 0) && (i8_dbg_level <= 11))
    {
        x_strcpy(ps_d_l_argv[1], pps_argv[1]);
    }
    else
    {
        return CLIR_INV_ARG;
    }

    for (ui4_i = 0; ui4_i < CLI_MAX_CMD_TBL_NUM; ui4_i++) /* Drv category */
    {
        pt_cmd_tbl = at_cli_mod_grp_info[CLI_CAT_DRV][ui4_i].pt_cmd_tbl;
        if (pt_cmd_tbl == NULL)
        {
            continue;
        }

        ui4_j = 0;
        while (pt_cmd_tbl[ui4_j].ps_cmd_str != NULL) /* Module */
        {
            if (pt_cmd_tbl[ui4_j].pt_next_level != NULL)
            {
                ui4_k = 0;
                pt_d_l_cmd_tbl = pt_cmd_tbl[ui4_j].pt_next_level;
                while (pt_d_l_cmd_tbl[ui4_k].ps_cmd_str != NULL) /* "d_l" command table */
                {
                    if (x_strcmp(pt_d_l_cmd_tbl[ui4_k].ps_cmd_str, CLI_DEBUG_LEVEL_STR) == 0 &&
                        pt_d_l_cmd_tbl[ui4_k].pf_exec_fct != NULL)
                    {
                        if (pt_d_l_cmd_tbl[ui4_k].pf_exec_fct(2, (const CHAR**)&ps_d_l_argv) != CLIR_OK)
                        {
                            x_dbg_stmt("Set %s debug level failed\n\r", pt_cmd_tbl[ui4_j].ps_cmd_str);
                        }
                        break;
                    }

                    ui4_k++;
                }
            }

            ui4_j++;
        }
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cli_cmd_get_grp_vs_mod
 *
 * Description: This API gets the information of what groups contain what
 *              modules.
 *
 * Inputs:  i4_argc     Number of arguments.
 *          pps_argv    Points to the argument array.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _cli_cmd_get_grp_vs_mod(INT32        i4_argc,
                                     const CHAR** pps_argv)
{
    UINT32          ui4_max_grp_num = 0;
    UINT64          aui8_grp_msk;
    CLI_EXEC_T*     pt_cmd_tbl;
    UINT32          ui4_grp;
    UINT32          ui4_i;
    UINT32          ui4_j;
    UINT32          ui4_k;
    BOOL            b_found = FALSE;

    /* group(g) [group] / group(g) all */

    /* Check arguments */
    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("group(g) [group] / group(g) all\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    /* Calculate the maximum group number */
    for (ui4_grp = 1; ui4_grp <= sizeof(CLI_GRP_MAX); ui4_grp++)
    {
        ui4_max_grp_num++;
        if ((CLI_GRP_MAX >> ui4_grp) == 0)
        {
            break;
        }
    }

    if (x_strcmp(CLI_ALL_STR, pps_argv[1]) == 0) /* All */
    {
        for (ui4_grp = 0; ui4_grp < ui4_max_grp_num; ui4_grp++)
        {
            x_dbg_stmt("------ %s ------\n\r", at_grp_n_str[ui4_grp].ps_grp_str); /* Group */
            aui8_grp_msk = at_grp_n_str[ui4_grp].aui8_grp_msk;
            for (ui4_i = CLI_CAT_ROOT; ui4_i < CLI_CAT_MAX; ui4_i++)
            {
                x_dbg_stmt("[%s] ", at_cat_n_str[ui4_i].ps_cat_str); /* Category */
                for (ui4_j = (ui4_i == CLI_CAT_ROOT ? CLI_CAT_MAX : 0); ui4_j < CLI_MAX_CMD_TBL_NUM; ui4_j++)
                {
                    pt_cmd_tbl = at_cli_mod_grp_info[ui4_i][ui4_j].pt_cmd_tbl;
                    if (pt_cmd_tbl == NULL)
                    {
                        break;
                    }

                    if (((at_cli_mod_grp_info[ui4_i][ui4_j].aui8_grp_msk & aui8_grp_msk) > 0) ||
                        (aui8_grp_msk == (at_cli_mod_grp_info[ui4_i][ui4_j].aui8_grp_msk)))
                    {
                        ui4_k = 0;
                        while (pt_cmd_tbl[ui4_k].ps_cmd_str != NULL)
                        {
                            x_dbg_stmt("%s ", pt_cmd_tbl[ui4_k++].ps_cmd_str); /* Module */
                        }
                    }
                }
                x_dbg_stmt("\n\r");
            }

            if (ui4_grp < ui4_max_grp_num - 1)
            {
                x_dbg_stmt("\n\r");
            }
        }
    }
    else /* Specific groups */
    {
        for (ui4_grp = 0; ui4_grp < ui4_max_grp_num; ui4_grp++)
        {
            if (x_strcmp(at_grp_n_str[ui4_grp].ps_grp_str, pps_argv[1]) == 0)
            {
                b_found = TRUE;
                x_dbg_stmt("------ %s ------\n\r", at_grp_n_str[ui4_grp].ps_grp_str); /* Group */
                aui8_grp_msk = at_grp_n_str[ui4_grp].aui8_grp_msk;
                for (ui4_i = CLI_CAT_ROOT; ui4_i < CLI_CAT_MAX; ui4_i++)
                {
                    x_dbg_stmt("[%s] ", at_cat_n_str[ui4_i].ps_cat_str); /* Category */
                    for (ui4_j = (ui4_i == CLI_CAT_ROOT ? CLI_CAT_MAX : 0); ui4_j < CLI_MAX_CMD_TBL_NUM; ui4_j++)
                    {
                        pt_cmd_tbl = at_cli_mod_grp_info[ui4_i][ui4_j].pt_cmd_tbl;
                        if (pt_cmd_tbl == NULL)
                        {
                            break;
                        }

                        if (((at_cli_mod_grp_info[ui4_i][ui4_j].aui8_grp_msk & aui8_grp_msk) > 0) ||
                            (aui8_grp_msk == (at_cli_mod_grp_info[ui4_i][ui4_j].aui8_grp_msk)))
                        {
                            ui4_k = 0;
                            while (pt_cmd_tbl[ui4_k].ps_cmd_str != NULL)
                            {
                                x_dbg_stmt("%s ", pt_cmd_tbl[ui4_k++].ps_cmd_str); /* Module */
                            }
                        }
                    }
                    x_dbg_stmt("\n\r");
                }
            }
        }

        if (!b_found)
        {
            x_dbg_stmt("group(g) [group] / group(g) all\n\r");
            return CLIR_INV_CMD_USAGE;
        }
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cli_cmd_get_mod_vs_grp
 *
 * Description: This API gets the information of what modules belong to what
 *              groups.
 *
 * Inputs:  i4_argc     Number of arguments.
 *          pps_argv    Points to the argument array.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _cli_cmd_get_mod_vs_grp(INT32        i4_argc,
                                     const CHAR** pps_argv)
{
    UINT32          ui4_max_grp_num = 0;
    UINT32          ui4_grp;
    CLI_EXEC_T*     pt_cmd_tbl;
    UINT32          ui4_i;
    UINT32          ui4_j;
    UINT32          ui4_k;
    BOOL            b_found = FALSE;

    /* module(m) [category] [module] / module(m) all */

    /* Check arguments */
    if ((((i4_argc != 2) || x_strcmp(CLI_ALL_STR, pps_argv[1]) != 0) &&
        (i4_argc != 3)) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("module(m) [category] [module] / module(m) all\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    /* Calculate the maximum group number */
    for (ui4_grp = 1; ui4_grp <= sizeof(CLI_GRP_MAX); ui4_grp++)
    {
        ui4_max_grp_num++;
        if ((CLI_GRP_MAX >> ui4_grp) == 0)
        {
            break;
        }
    }

    if (x_strcmp(CLI_ALL_STR, pps_argv[1]) == 0) /* All */
    {
        for (ui4_i = (UINT32)CLI_CAT_ROOT; ui4_i < (UINT32)CLI_CAT_MAX; ui4_i++)
        {
            x_dbg_stmt("------ %s ------\n\r", at_cat_n_str[ui4_i].ps_cat_str); /* Category */
            for (ui4_j = (ui4_i == CLI_CAT_ROOT ? CLI_CAT_MAX : 0); ui4_j < CLI_MAX_CMD_TBL_NUM; ui4_j++)
            {
                pt_cmd_tbl = at_cli_mod_grp_info[ui4_i][ui4_j].pt_cmd_tbl;
                if (pt_cmd_tbl == NULL)
                {
                    continue;
                }

                ui4_k = 0;
                while (pt_cmd_tbl[ui4_k].ps_cmd_str != NULL)
                {
                    x_dbg_stmt("%-10s", pt_cmd_tbl[ui4_k].ps_cmd_str); /* Module */

                    if (at_cli_mod_grp_info[ui4_i][ui4_j].aui8_grp_msk == CLI_GRP_NONE)
                    {
                        x_dbg_stmt("%s\n\r", CLI_NONE_STR); /* Group */
                    }
                    else
                    {
                        for (ui4_grp = 0; ui4_grp < ui4_max_grp_num; ui4_grp++)
                        {
                            if ((at_cli_mod_grp_info[ui4_i][ui4_j].aui8_grp_msk &
                                at_grp_n_str[ui4_grp].aui8_grp_msk) > 0)
                            {
                                x_dbg_stmt("%s ", at_grp_n_str[ui4_grp].ps_grp_str); /* Group */
                            }
                        }
                        x_dbg_stmt("\n\r");
                    }
                    ui4_k++;
                }
            }

            if (ui4_i < (UINT32)(CLI_CAT_MAX - 1))
            {
                x_dbg_stmt("\n\r");
            }
        }
    }
    else /* Specific modules */
    {
        for (ui4_i = CLI_CAT_ROOT; ui4_i < CLI_CAT_MAX; ui4_i++)
        {
            if (x_strcmp(at_cat_n_str[ui4_i].ps_cat_str, pps_argv[1]) == 0)
            {
                x_dbg_stmt("------ %s ------\n\r", at_cat_n_str[ui4_i].ps_cat_str); /* Category */
                for (ui4_j = (ui4_i == CLI_CAT_ROOT ? CLI_CAT_MAX : 0); ui4_j < CLI_MAX_CMD_TBL_NUM; ui4_j++)
                {
                    pt_cmd_tbl = at_cli_mod_grp_info[ui4_i][ui4_j].pt_cmd_tbl;
                    if (pt_cmd_tbl == NULL)
                    {
                        break;
                    }

                    ui4_k = 0;
                    while (pt_cmd_tbl[ui4_k].ps_cmd_str != NULL)
                    {
                        if (x_strcmp(pt_cmd_tbl[ui4_k].ps_cmd_str, pps_argv[2]) == 0)
                        {
                            b_found = TRUE;
                            x_dbg_stmt("%-10s", pt_cmd_tbl[ui4_k].ps_cmd_str); /* Module */

                            if (at_cli_mod_grp_info[ui4_i][ui4_j].aui8_grp_msk == CLI_GRP_NONE)
                            {
                                x_dbg_stmt("%s\n\r", CLI_NONE_STR); /* Group */
                            }
                            else
                            {
                                for (ui4_grp = 0; ui4_grp < ui4_max_grp_num; ui4_grp++)
                                {
                                    if ((at_cli_mod_grp_info[ui4_i][ui4_j].aui8_grp_msk &
                                        at_grp_n_str[ui4_grp].aui8_grp_msk) > 0)
                                    {
                                        x_dbg_stmt("%s ", at_grp_n_str[ui4_grp].ps_grp_str); /* Group */
                                    }
                                }
                            }

                            x_dbg_stmt("\n\r");
                            return CLIR_OK;
                        }
                        ui4_k++;
                    }
                }
            }
        }

        if (!b_found)
        {
            x_dbg_stmt("module(m) [category] [module] / module(m) all\n\r");
            return CLIR_INV_CMD_USAGE;
        }
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cli_cmd_set_ctrl_stmt
 *
 * Description: This API enables or disables statement output via
 *              "[xc]_dbg_ctrl_stmt" of Debug library.
 *
 * Inputs:  i4_argc     Number of arguments.
 *          pps_argv    Points to the argument array.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _cli_cmd_set_ctrl_stmt(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    /* cs [ctrl] */

    /* Check arguments */
    if ((i4_argc != 2) ||
        (pps_argv == NULL))
    {
        x_dbg_stmt("cs [ctrl]\n\r");
        x_dbg_stmt("  [ctrl]: 0=OFF, 1=ON");
        return CLIR_INV_CMD_USAGE;
    }

    if (x_strcmp("0", pps_argv[1]) == 0)
    {
        dbg_set_ctrl_stmt(FALSE);
        x_dbg_stmt("Ctrl stmt output = OFF\n\r");
    }
    else if (x_strcmp("1", pps_argv[1]) == 0)
    {
        dbg_set_ctrl_stmt(TRUE);
        x_dbg_stmt("Ctrl stmt output = ON\n\r");
    }
    else
    {
        return CLIR_INV_ARG;
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cli_cmd_show_ver_info
 *
 * Description: This API shows the current system version information.
 *
 * Inputs:  i4_argc     Number of arguments.
 *          pps_argv    Points to the argument array.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _cli_cmd_show_ver_info(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    /* ver */

    x_dbg_stmt("------------------------------------------------------\n\r");
    x_dbg_stmt(" Customer = %s\n\r", x_sys_get_customer());
    x_dbg_stmt(" Build name = %s\n\r", x_sys_get_build_name());
    x_dbg_stmt(" Module name = %s\n\r", x_sys_get_model_name());
    x_dbg_stmt(" Version = %s\n\r", x_sys_get_version());
    x_dbg_stmt(" Serial number = %s\n\r", x_sys_get_serial_number());        
    x_dbg_stmt("------------------------------------------------------\n\r");        

    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _cli_cmd_set_log_info
 *
 * Description: This API let module specify debug info message,if specfy related flags,then debug
 * info will print more msg,eg:time,file name,line num.....
 *
 * Inputs:  i4_argc     Number of arguments.
 *          pps_argv    Points to the argument array.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 		CLIR_MODULE_UNREGIST   module is not regist
 ----------------------------------------------------------------------------*/

static INT32 _cli_cmd_set_log_info(INT32 i4_argc, const CHAR** pps_argv)
{

	/* Check arguments */
    if ((i4_argc < 2) ||
        (pps_argv == NULL))
	
    {
        cli_show_log_prompt();
        return CLIR_INV_CMD_USAGE;
    }

	return cli_get_module_fuc(i4_argc, pps_argv);	
}


#endif


/*-----------------------------------------------------------------------------
 * Name: cli_parser_list_mandatory_tbl_cmd
 *
 * Description: This API lists all commands from mandatory command table.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_NOT_ENABLED        CLI is not enabled.
 ----------------------------------------------------------------------------*/
INT32 cli_parser_list_mandatory_tbl_cmd(VOID)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    UINT32          ui4_cmd_idx = 0;
    CHAR            s_help[MAX_STR_LEN];

    x_dbg_stmt("\n\r[Help]\n\r");

    while (at_mandatory_cmd_tbl[ui4_cmd_idx].ps_cmd_help_str != NULL)
    {
        if (_format_help_str(s_help,
                             MAX_STR_LEN,
                             at_mandatory_cmd_tbl[ui4_cmd_idx].ps_cmd_str,
                             at_mandatory_cmd_tbl[ui4_cmd_idx].ps_cmd_abbr_str,
                             at_mandatory_cmd_tbl[ui4_cmd_idx].ps_cmd_help_str) == TRUE)
        {
            x_dbg_stmt("%s\n\r", s_help);
        }

        ui4_cmd_idx++;
    }
            
    _generate_prompt();
                
    return CLIR_OK;
#else
    return CLIR_NOT_ENABLED;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: cli_parser_attach_cmd_tbl
 *
 * Description: This API attaches a command table to another command table.
 *
 * Inputs:  pt_tbl          The command table to be attached.
 *          e_category      The category that the command table belongs to.
 *          ui8_group_mask  The group(s) that the command table belongs to.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_ARG            One or more invalid arguments.
 *          CLIR_CMD_TBL_FULL       Command table is full.
 *          CLIR_GROUP_TBL_FULL     Module-to-group table is full.
 *          CLIR_NOT_ENABLED        CLI is not enabled.
 ----------------------------------------------------------------------------*/
INT32 cli_parser_attach_cmd_tbl(CLI_EXEC_T* pt_tbl,
                                CLI_CAT_T   e_category,
                                UINT64      ui8_group_mask)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    UINT32          ui4_idx;
    UINT32*         pui4_tbl_cnt;
    CLI_EXEC_T*     pt_cmd_tbl = NULL;
    CLI_EXEC_T**    ppt_cmd_tbl_array;
    CLI_GRP_T*      pt_mod_grp_info;
    CLI_EXEC_T*     pt_tbl_tmp_a = NULL;
    CLI_EXEC_T*     pt_tbl_tmp_b = NULL;
    UINT64          ui8_group_mask_tmp_a = 0;
    UINT64          ui8_group_mask_tmp_b = 0;
    BOOL            b_replace_cmd_tbl = FALSE;
    BOOL            b_sort_cmd_tbl = FALSE;
    BOOL            b_swap = FALSE;

    /* Check arguments */
    if ((pt_tbl == NULL) ||
        (pt_tbl->ps_cmd_str == NULL) ||
        (e_category >= CLI_CAT_MAX))
    {
        return CLIR_INV_ARG;
    }

    /* Attach the command table to requested category */
    switch (e_category)
    {
        case CLI_CAT_ROOT:
            pui4_tbl_cnt = &aui4_cli_tbl_cnt[CLI_CAT_ROOT];
            ppt_cmd_tbl_array = &(apt_cli_array[CLI_CAT_ROOT][0]);
            pt_mod_grp_info = &(at_cli_mod_grp_info[CLI_CAT_ROOT][0]);
            break;

        case CLI_CAT_BASIC:
            pui4_tbl_cnt = &aui4_cli_tbl_cnt[CLI_CAT_BASIC];
            pt_cmd_tbl = at_basic_cmd_tbl;
            ppt_cmd_tbl_array = &(apt_cli_array[CLI_CAT_BASIC][0]);
            pt_mod_grp_info = &(at_cli_mod_grp_info[CLI_CAT_BASIC][0]);
            break;

        case CLI_CAT_APP:
            pui4_tbl_cnt = &aui4_cli_tbl_cnt[CLI_CAT_APP];
            pt_cmd_tbl = at_app_cmd_tbl;
            ppt_cmd_tbl_array = &(apt_cli_array[CLI_CAT_APP][0]);
            pt_mod_grp_info = &(at_cli_mod_grp_info[CLI_CAT_APP][0]);
            break;

        case CLI_CAT_MW:
            pui4_tbl_cnt = &aui4_cli_tbl_cnt[CLI_CAT_MW];
            pt_cmd_tbl = at_mw_cmd_tbl;
            ppt_cmd_tbl_array = &(apt_cli_array[CLI_CAT_MW][0]);
            pt_mod_grp_info = &(at_cli_mod_grp_info[CLI_CAT_MW][0]);
            break;

        case CLI_CAT_DRV:
            pui4_tbl_cnt = &aui4_cli_tbl_cnt[CLI_CAT_DRV];
            pt_cmd_tbl = at_drv_cmd_tbl;
            ppt_cmd_tbl_array = &(apt_cli_array[CLI_CAT_DRV][0]);
            pt_mod_grp_info = &(at_cli_mod_grp_info[CLI_CAT_DRV][0]);
            break;

        case CLI_CAT_TEST:
            pui4_tbl_cnt = &aui4_cli_tbl_cnt[CLI_CAT_TEST];
            pt_cmd_tbl = at_test_cmd_tbl;
            ppt_cmd_tbl_array = &(apt_cli_array[CLI_CAT_TEST][0]);
            pt_mod_grp_info = &(at_cli_mod_grp_info[CLI_CAT_TEST][0]);
            break;
            
        case CLI_CAT_MTK_TOOL:
            pui4_tbl_cnt = &aui4_cli_tbl_cnt[CLI_CAT_MTK_TOOL];
            pt_cmd_tbl = at_mtk_tool_cmd_tbl;
            ppt_cmd_tbl_array = &(apt_cli_array[CLI_CAT_MTK_TOOL][0]);
            pt_mod_grp_info = &(at_cli_mod_grp_info[CLI_CAT_MTK_TOOL][0]);
            break;
            
        default:
            return CLIR_INV_ARG;
    }

    /* Check if buffer size is ok */
    if (*pui4_tbl_cnt >= CLI_MAX_CMD_TBL_NUM)
    {
        return CLIR_CMD_TBL_FULL;
    }

    /* Check if command table sorting is required */
    if (*pui4_tbl_cnt > 0 && e_category > CLI_CAT_ROOT)
    {
        b_sort_cmd_tbl = TRUE;
    }

    /* If the command table is duplicated, replace existed commad table with new one */
    for (ui4_idx = 0; ui4_idx < *pui4_tbl_cnt; ui4_idx++)
    {
        if ((x_strcmp(pt_tbl->ps_cmd_str, ppt_cmd_tbl_array[ui4_idx]->ps_cmd_str) == 0) &&
            (x_strcmp(pt_tbl->ps_cmd_abbr_str, ppt_cmd_tbl_array[ui4_idx]->ps_cmd_abbr_str) == 0))
        {
            b_replace_cmd_tbl = TRUE;
            break;
        }
    }

    if (b_replace_cmd_tbl)
    {
        ppt_cmd_tbl_array[ui4_idx] = pt_tbl;
        pt_mod_grp_info[ui4_idx].pt_cmd_tbl = pt_tbl;
        pt_mod_grp_info[ui4_idx].aui8_grp_msk = ui8_group_mask;

        if (*pui4_tbl_cnt == 0 && e_category > CLI_CAT_ROOT)
        {
            pt_cmd_tbl[0].pt_next_level = pt_tbl;
        }
    }
    else
    {
        ui4_idx = *pui4_tbl_cnt;
        (*pui4_tbl_cnt)++;

        if (b_sort_cmd_tbl)
        {
            /* Start sortig */
            for (ui4_idx = 0; ui4_idx < *pui4_tbl_cnt; ui4_idx++)
            {
                if (!b_swap && ui4_idx < *pui4_tbl_cnt - 1)
                {
                    if (x_strcmp(pt_tbl->ps_cmd_str, ppt_cmd_tbl_array[ui4_idx]->ps_cmd_str) == -1)
                    {
                        b_swap = TRUE;
                        pt_tbl_tmp_b = ppt_cmd_tbl_array[ui4_idx];
                        ui8_group_mask_tmp_b = pt_mod_grp_info[ui4_idx].aui8_grp_msk;

                        ppt_cmd_tbl_array[ui4_idx] = pt_tbl;
                        pt_mod_grp_info[ui4_idx].pt_cmd_tbl = pt_tbl;
                        pt_mod_grp_info[ui4_idx].aui8_grp_msk = ui8_group_mask;

                        if (ui4_idx == 0 && e_category > CLI_CAT_ROOT)
                        {
                            pt_cmd_tbl[0].pt_next_level = pt_tbl;
                        }
                    }
                }
                else
                {
                    if (b_swap)
                    {
                        /* Do command table swap */
                        pt_tbl_tmp_a = ppt_cmd_tbl_array[ui4_idx];
                        ui8_group_mask_tmp_a = pt_mod_grp_info[ui4_idx].aui8_grp_msk;
                        ppt_cmd_tbl_array[ui4_idx] = pt_tbl_tmp_b;
                        pt_mod_grp_info[ui4_idx].pt_cmd_tbl = pt_tbl_tmp_b;
                        pt_mod_grp_info[ui4_idx].aui8_grp_msk = ui8_group_mask_tmp_b;
                        pt_tbl_tmp_b = pt_tbl_tmp_a;
                        ui8_group_mask_tmp_b = ui8_group_mask_tmp_a;
                    }
                    else
                    {
                        ppt_cmd_tbl_array[ui4_idx] = pt_tbl;
                        pt_mod_grp_info[ui4_idx].pt_cmd_tbl = pt_tbl;
                        pt_mod_grp_info[ui4_idx].aui8_grp_msk = ui8_group_mask;
                    }
                }
            }
        }
        else
        {
            ppt_cmd_tbl_array[ui4_idx] = pt_tbl;
            pt_mod_grp_info[ui4_idx].pt_cmd_tbl = pt_tbl;
            pt_mod_grp_info[ui4_idx].aui8_grp_msk = ui8_group_mask;

            if (ui4_idx == 0 && e_category > CLI_CAT_ROOT)
            {
                pt_cmd_tbl[0].pt_next_level = pt_tbl;
            }
        }
    }

    return CLIR_OK;
#else
    return CLIR_NOT_ENABLED;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: cli_parser_clear_cmd_tbl
 *
 * Description: This API removes all command tables from CLI root then
 *              attaches default ones.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_ARG            One or more invalid arguments.
 *          CLIR_CMD_TBL_FULL       Command table is full.
 *          CLIR_GROUP_TBL_FULL     Module-to-group table is full.
 *          CLIR_NOT_ENABLED        CLI is not enabled.
 ----------------------------------------------------------------------------*/
INT32 cli_parser_clear_cmd_tbl(VOID)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    INT32       i4_return;
    UINT32      ui4_i;
    UINT32      ui4_j;

    /* Remove all command tables attached to CLI root */
    for (ui4_i = CLI_CAT_ROOT; ui4_i < CLI_CAT_MAX; ui4_i++)
    {
        for (ui4_j = 0; ui4_j < CLI_MAX_CMD_TBL_NUM; ui4_j++)
        {
            apt_cli_array[ui4_i][ui4_j] = NULL;
            aui4_cli_tbl_cnt[ui4_i] = 0;
            at_cli_mod_grp_info[ui4_i][ui4_j].pt_cmd_tbl = NULL;
            at_cli_mod_grp_info[ui4_i][ui4_j].aui8_grp_msk = CLI_GRP_NONE;
        }
    }

    /* Attach default command tables to CLI root */
    at_basic_cmd_tbl[0].pt_next_level = NULL;
    at_app_cmd_tbl[0].pt_next_level = NULL;
    at_mw_cmd_tbl[0].pt_next_level = NULL;
    at_drv_cmd_tbl[0].pt_next_level = NULL;
    at_test_cmd_tbl[0].pt_next_level = NULL;
    at_mtk_tool_cmd_tbl[0].pt_next_level = NULL;        

    /* Mandatory command table */
    i4_return = cli_parser_attach_cmd_tbl(at_mandatory_cmd_tbl, CLI_CAT_ROOT, CLI_GRP_NONE);
    if (i4_return != CLIR_OK)
    {
        return i4_return;
    }

    /* Basic command table */
    i4_return = cli_parser_attach_cmd_tbl(at_basic_cmd_tbl, CLI_CAT_ROOT, CLI_GRP_NONE);
    if (i4_return != CLIR_OK)
    {
        return i4_return;
    }

    /* Application command table */
    i4_return = cli_parser_attach_cmd_tbl(at_app_cmd_tbl, CLI_CAT_ROOT, CLI_GRP_NONE);
    if (i4_return != CLIR_OK)
    {
        return i4_return;
    }

    /* Middleware command table */
    i4_return = cli_parser_attach_cmd_tbl(at_mw_cmd_tbl, CLI_CAT_ROOT, CLI_GRP_NONE);
    if (i4_return != CLIR_OK)
    {
        return i4_return;
    }

    /* Driver command table */
    i4_return = cli_parser_attach_cmd_tbl(at_drv_cmd_tbl, CLI_CAT_ROOT, CLI_GRP_NONE);
    if (i4_return != CLIR_OK)
    {
        return i4_return;
    }

    /* Test command table */
    i4_return = cli_parser_attach_cmd_tbl(at_test_cmd_tbl, CLI_CAT_ROOT, CLI_GRP_NONE);
    if (i4_return != CLIR_OK)
    {
        return i4_return;
    }

    /* MTK tool command table */
    i4_return = cli_parser_attach_cmd_tbl(at_mtk_tool_cmd_tbl, CLI_CAT_ROOT, CLI_GRP_NONE);
    if (i4_return != CLIR_OK)
    {
        return i4_return;
    }
    
    /* Init internal variables */
    ui4_dir_link_idx = 0;
    apt_dir_link[0] = NULL;
    pt_cur_cmd_tbl = NULL;
    b_is_cmd_tbl = FALSE;

    /* Set up prompt string */
    ps_cli_prompt_str = cli_get_prompt_str_buf();
    ps_cli_prompt_str[0] = ASCII_NULL;
    x_strncat(ps_cli_prompt_str, CLI_PROMPT_STR, sizeof(CLI_PROMPT_STR));
    x_strncat(ps_cli_prompt_str, ">", 1);

    return CLIR_OK;
#else
    return CLIR_NOT_ENABLED;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: cli_parser
 *
 * Description: This API parses a CLI command and performs corresponding
 *              operation.
 *
 * Inputs:  ps_cmd      Contain the command to parse.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_ARG            One or more invalid arguments.
 *          CLIR_UNKNOWN_CMD        Unknown CLI command.
 *          CLIR_CMD_NOT_FOUND      CLI command not found.
 *          CLIR_DIR_NOT_FOUND      CLI directory not found.
 *          CLIR_NOT_ENABLED        CLI is not enabled.
 ----------------------------------------------------------------------------*/
INT32 cli_parser(const CHAR* ps_cmd)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    INT32           i4_return = CLIR_OK;
    INT32           i4_argc;
    UINT32          ui4_inp_cmd_len;
    UINT32          ui4_cmd_len;
    UINT32          ui4_cmd_abbr_len;
    UINT32          ui4_argc;
    UINT32          ui4_alias_argc;
    UINT32          ui4_idx;
    UINT32          ui4_alias_idx;
    CHAR*           ps_argv[CLI_MAX_ARG_NUM];
    CHAR*           ps_alias_argv[CLI_MAX_ARG_NUM];
    const CHAR*     ps_alias_str;

    /* Check arguments */
    if (ps_cmd == NULL)
    {
        return CLIR_INV_ARG;
    }

    /* Init argument buffer */
    for (ui4_idx = 0; ui4_idx < CLI_MAX_ARG_NUM; ui4_idx++)
    {
        ps_argv[ui4_idx] = as_argv[ui4_idx];
        as_argv[ui4_idx][0] = 0;
    }

    /* Get all the arguments of the command */
    i4_argc = _find_cmd_argv(ps_cmd,
                             CLI_MAX_ARG_NUM,
                             CLI_MAX_ARG_LEN,
                             ps_argv);

    ui4_argc = (UINT32)i4_argc;

    if (ui4_argc > 0)
    {
        /* Alias handling: replace alias with corresponding string */
        ui4_inp_cmd_len = x_strlen(ps_argv[0]);
        ui4_cmd_len = x_strlen(CLI_ALIAS_CMD_STR);
        ui4_cmd_abbr_len = x_strlen(CLI_ALIAS_CMD_ABBR_STR);

        if (!(((ui4_inp_cmd_len == ui4_cmd_len) && (x_strncmp(ps_argv[0], CLI_ALIAS_CMD_STR, ui4_inp_cmd_len) == 0)) ||
            ((ui4_inp_cmd_len == ui4_cmd_abbr_len) && (x_strncmp(ps_argv[0], CLI_ALIAS_CMD_ABBR_STR, ui4_inp_cmd_len) == 0))))
        {
            ui4_idx = 0;
            while (ui4_idx < ui4_argc)
            {
                /* Retrieve string corresponding to the alias */
                ps_alias_str = cli_alias_search(ps_argv[ui4_idx]);

                if (ps_alias_str)
                {
                    /* Fill current alias arguments in to alias argument buffer */
                    for (ui4_alias_idx = 0; ui4_alias_idx < CLI_MAX_ARG_NUM; ui4_alias_idx++)
                    {
                        ps_alias_argv[ui4_alias_idx] = as_alias_argv[ui4_alias_idx];
                    }

                    ui4_alias_argc = (UINT32)_find_cmd_argv(ps_alias_str,
                                                            CLI_MAX_ARG_NUM,
                                                            CLI_MAX_ARG_LEN,
                                                            ps_alias_argv);

                    /* Too many arguments after alias parsing */
                    if (ui4_alias_argc > (CLI_MAX_ARG_NUM - (ui4_argc + 1)))
                    {
                        ui4_alias_argc = CLI_MAX_ARG_NUM - (ui4_argc + 1);
                    }

                    /* Copy string arguments after alias backward */
                    for (ui4_alias_idx = (ui4_argc - 1); ui4_alias_idx > ui4_idx; ui4_alias_idx--)
                    {
                        x_strncpy(ps_argv[ui4_alias_idx + (ui4_alias_argc - 1)],
                                  ps_argv[ui4_alias_idx],
                                  CLI_MAX_ARG_LEN);
                    }

                    /* Copy alias string arguemnts */
                    for (ui4_alias_idx = 0; ui4_alias_idx < ui4_alias_argc; ui4_alias_idx++)
                    {
                        x_strncpy(ps_argv[ui4_idx + ui4_alias_idx],
                                  ps_alias_argv[ui4_alias_idx],
                                  CLI_MAX_ARG_LEN);
                    }

                    ui4_argc += (ui4_alias_argc - 1);
                    ui4_idx--;
                 }

                ui4_idx++;
            }
        }

        i4_argc = (INT32)ui4_argc;

        /* Command parsing: mandatory table */
        i4_return = _parse_cmd(i4_argc,
                               (const CHAR**)ps_argv,
                               apt_cli_array[CLI_CAT_ROOT][CLI_MANDA_CMD_TBL_IDX]);

        if (i4_return != CLIR_CMD_NOT_FOUND)
        {
            return i4_return;
        }
        else
        {
            /* Command parsing: other command tables */
            if (i4_argc > 0)
            {
                i4_return = _parse_cmd(i4_argc,
                                       (const CHAR**)ps_argv,
                                       pt_cur_cmd_tbl);

                if (i4_return != CLIR_CMD_NOT_FOUND)
                {
                    return i4_return;
                }
            }
        }
    }

    if ((i4_argc > 0) && (i4_return == CLIR_CMD_NOT_FOUND))
    {
        return i4_return;
    }
    else
    {
        /* Display CLI help */
        _show_help(pt_cur_cmd_tbl);
        return CLIR_OK;
    }
#else
    return CLIR_NOT_ENABLED;
#endif
}

INT32 cli_parser_cmd(const CHAR* ps_cmd, CHAR* const* pps_args)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)

	INT32           i4_argc;

	if(ps_cmd == NULL ||
		pps_args == NULL)
	{
		return 0;
	}
	
	/* Get all the arguments of the command */
	i4_argc = _find_cmd_argv(ps_cmd,
							 CLI_MAX_ARG_NUM,
							 CLI_MAX_ARG_LEN,
							 pps_args);
	return i4_argc;
#else
    return 0;
#endif	
}

INT32 cli_set_access_right(CLI_ACCESS_RIGHT_T e_right)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    e_access_right = e_right;
	return CLIR_OK;
#else
    return CLIR_NOT_ENABLED;
#endif
}

INT32 cli_show_log_prompt(VOID)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
	x_dbg_stmt("log <module_name> <level> <ts> <file> <func> <line> <custom>\n\r");
	x_dbg_stmt("ex: log abc 0 +t -d +f -l -c\n\r");
	return CLIR_OK;
#else
    return CLIR_NOT_ENABLED;
#endif

}


