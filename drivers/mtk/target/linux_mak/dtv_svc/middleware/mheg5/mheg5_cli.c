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
 * $RCSfile: mheg5_cli.c,v $
 * $Revision:
 * $Date:
 * $Author: dtvbm11 $
 * $CCRevision:  $
 * $SWAuthor: Vincent Hsu $
 * $MD5HEX: 37ebbb2d76fad2e8a90655e5f2bf2c3a $
 *
 * Description:
 *         This file implements get/set debug level APIs for MHEG5.
 *---------------------------------------------------------------------------*/
#include "inc/u_common.h"
#include "dbg/x_dbg.h"
#include "dbg/u_dbg.h"
#include "cli/u_cli.h"
#include "cli/x_cli.h"
#include "dbg/def_dbg_level_mw.h"
#include "mheg5/OceanBlue/glue/strm_ctrl/sct.h"
#include "mheg5/OceanBlue/glue/gui_render/gfx_rnd.h"
#ifdef MHEG5_IC_SUPPORT
#include "mheg5/OceanBlue/glue/ic/mheg5_ic.h"
#endif
#include "mheg5/OceanBlue/glue/ssf/mheg5_section.h"
#ifdef CI_PLUS_SUPPORT
#include "mheg5/OceanBlue/glue/ci/mheg5_ci.h"
#endif
#include "mheg5/OceanBlue/glue/misc/mheg5_misc.h"
#include "mheg5/OceanBlue/glue/gui_render/grd.h"
#include "graphic/x_gl.h"
#include "handle/x_handle.h"

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32  _mheg5_cli_dump               (INT32 i4_argc, const CHAR** pps_argv);
static INT32  _mheg5_cli_set_dbg_level      (INT32 i4_argc, const CHAR** pps_argv);
static INT32  _mheg5_cli_get_dbg_level      (INT32 i4_argc, const CHAR** pps_argv);
#ifdef MHEG5_IC_SUPPORT
static INT32  _mheg5_cli_ic_set_dbg_level   (INT32 i4_argc, const CHAR** pps_argv);
static INT32  _mheg5_cli_ic_get_dbg_level   (INT32 i4_argc, const CHAR** pps_argv);
#endif
static INT32  _mheg5_cli_sct_set_dbg_level  (INT32 i4_argc, const CHAR** pps_argv);
static INT32  _mheg5_cli_sct_get_dbg_level  (INT32 i4_argc, const CHAR** pps_argv);
static INT32  _mheg5_cli_ssf_set_dbg_level  (INT32 i4_argc, const CHAR** pps_argv);
static INT32  _mheg5_cli_ssf_get_dbg_level  (INT32 i4_argc, const CHAR** pps_argv);
#ifdef CI_PLUS_SUPPORT
static INT32  _mheg5_cli_ci_set_dbg_level   (INT32 i4_argc, const CHAR** pps_argv);
static INT32  _mheg5_cli_ci_get_dbg_level   (INT32 i4_argc, const CHAR** pps_argv);
#endif
static INT32  _mheg5_cli_misc_set_dbg_level (INT32 i4_argc, const CHAR** pps_argv);
static INT32  _mheg5_cli_misc_get_dbg_level (INT32 i4_argc, const CHAR** pps_argv);
static INT32  _mheg5_cli_gui_set_dbg_level (INT32 i4_argc, const CHAR** pps_argv);
static INT32  _mheg5_cli_gui_get_dbg_level (INT32 i4_argc, const CHAR** pps_argv);

static INT32  _mheg5_cli_dump_rendering_surface (INT32 i4_argc, const CHAR** pps_argv);

static UINT16 _mheg5_get_dbg_level      (VOID);
static BOOL   _mheg5_set_dbg_level      (UINT16 ui2_level);

/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
static UINT16           _ui2_mheg5_dbg_level = DBG_INIT_LEVEL_MW_MHEG5;
#ifdef MHEG5_IC_SUPPORT
static CLI_EXEC_T at_ic_cmd_tbl[] =
{
    {CLI_GET_DBG_LVL_STR,   NULL,   _mheg5_cli_ic_get_dbg_level,        NULL,           CLI_GET_DBG_LVL_HELP_STR,               CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR,   NULL,   _mheg5_cli_ic_set_dbg_level,        NULL,           CLI_SET_DBG_LVL_HELP_STR,               CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};
#endif

static CLI_EXEC_T at_sct_cmd_tbl[] =
{
    {CLI_GET_DBG_LVL_STR,   NULL,   _mheg5_cli_sct_get_dbg_level,        NULL,           CLI_GET_DBG_LVL_HELP_STR,               CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR,   NULL,   _mheg5_cli_sct_set_dbg_level,        NULL,           CLI_SET_DBG_LVL_HELP_STR,               CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

static CLI_EXEC_T at_ssf_cmd_tbl[] =
{
    {CLI_GET_DBG_LVL_STR,   NULL,   _mheg5_cli_ssf_get_dbg_level,        NULL,           CLI_GET_DBG_LVL_HELP_STR,               CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR,   NULL,   _mheg5_cli_ssf_set_dbg_level,        NULL,           CLI_SET_DBG_LVL_HELP_STR,               CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

#ifdef CI_PLUS_SUPPORT
static CLI_EXEC_T at_ci_cmd_tbl[] =
{
    {CLI_GET_DBG_LVL_STR,   NULL,   _mheg5_cli_ci_get_dbg_level,        NULL,           CLI_GET_DBG_LVL_HELP_STR,               CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR,   NULL,   _mheg5_cli_ci_set_dbg_level,        NULL,           CLI_SET_DBG_LVL_HELP_STR,               CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};
#endif

static CLI_EXEC_T at_misc_cmd_tbl[] =
{
    {CLI_GET_DBG_LVL_STR,   NULL,   _mheg5_cli_misc_get_dbg_level,        NULL,           CLI_GET_DBG_LVL_HELP_STR,               CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR,   NULL,   _mheg5_cli_misc_set_dbg_level,        NULL,           CLI_SET_DBG_LVL_HELP_STR,               CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

static CLI_EXEC_T at_gui_cmd_tbl[] =
{
    {CLI_GET_DBG_LVL_STR,   NULL,   _mheg5_cli_gui_get_dbg_level,        NULL,           CLI_GET_DBG_LVL_HELP_STR,               CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR,   NULL,   _mheg5_cli_gui_set_dbg_level,        NULL,           CLI_SET_DBG_LVL_HELP_STR,               CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

/* mheg5 command table */
static CLI_EXEC_T at_mheg5_cmd_tbl[] =
{
#ifdef MHEG5_IC_SUPPORT
    {"ic",                  NULL,   NULL,                               at_ic_cmd_tbl,  "IC commands",                          CLI_GUEST},
#endif
    {"sct",                 NULL,   NULL,                               at_sct_cmd_tbl, "Stream relate commands",               CLI_GUEST},
    {"ssf",                 NULL,   NULL,                               at_ssf_cmd_tbl, "Section commands",                     CLI_GUEST},
#ifdef CI_PLUS_SUPPORT
    {"ci",                  NULL,   NULL,                               at_ci_cmd_tbl,  "CI commands",                          CLI_GUEST},
#endif
    {"misc",                NULL,   NULL,                               at_misc_cmd_tbl,"Other commands(state,key)",            CLI_GUEST},
    {"gui",                 NULL,   NULL,                               at_gui_cmd_tbl, "Other commands(grd,osd)",            CLI_GUEST},
    {CLI_GET_DBG_LVL_STR,   NULL,   _mheg5_cli_get_dbg_level,           NULL,           CLI_GET_DBG_LVL_HELP_STR,               CLI_SUPERVISOR},
	{CLI_SET_DBG_LVL_STR,   NULL,   _mheg5_cli_set_dbg_level,           NULL,           CLI_SET_DBG_LVL_HELP_STR,               CLI_SUPERVISOR},
    {"d",                   NULL,   _mheg5_cli_dump,                    NULL,           "Dump internal data",                   CLI_SUPERVISOR},
    {"drs",                 NULL,   _mheg5_cli_dump_rendering_surface,  NULL,           "Dump internal MHEG5 rendering data",   CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

/* mheg5 root command table */
static CLI_EXEC_T at_mheg5_root_cmd_tbl[] =
{
	{"mheg5",   NULL,   NULL,   at_mheg5_cmd_tbl,   "MHEG-5 commands",  CLI_SUPERVISOR},
	END_OF_CLI_CMD_TBL
};
#endif

/*-----------------------------------------------------------------------------
 * exported methods implementations
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/
 
INT32 mheg5_cli_init(VOID)
{
#ifdef CLI_LVL_ALL
    INT32       i4_ret;

    i4_ret = x_cli_attach_cmd_tbl(at_mheg5_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_NONE);

    if (i4_ret == CLIR_OK)
    {
        return MHEG5R_OK;
    }
    else
    {
        return MHEG5R_INIT_FAIL;
    }
#else
    return MHEG5R_INIT_FAIL;
#endif
}

#ifdef CLI_LVL_ALL
static INT32 _mheg5_cli_dump(INT32 i4_argc, const CHAR** pps_argv)
{
    sct_show_data();
    
    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _mheg5_cli_get_dbg_level
 *
 * Description: This API gets the current setting of mheg5 debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _mheg5_cli_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_return;

    i4_return = x_cli_show_dbg_level(_mheg5_get_dbg_level());

    return i4_return;
}

/*-----------------------------------------------------------------------------
 * Name: _mheg5_cli_set_dbg_level
 *
 * Description: This API sets the debug level of mheg5.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _mheg5_cli_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;

    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);

    if (i4_return == CLIR_OK)
    {
        if (_mheg5_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_return = CLIR_CMD_EXEC_ERROR;
        }

        i4_return = CLIR_OK;
    }
    else
    {
        i4_return = CLIR_CMD_EXEC_ERROR;
    }

    x_cli_show_dbg_level(_mheg5_get_dbg_level());
    return i4_return;
}

/*-----------------------------------------------------------------------------
 * Name: _mheg5_get_dbg_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/
static UINT16 _mheg5_get_dbg_level(VOID)
{
    return _ui2_mheg5_dbg_level;
}

/*-----------------------------------------------------------------------------
 * Name: img_set_dbg_level
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs: ui2_level            The new setting of debug level.
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 ----------------------------------------------------------------------------*/
static BOOL _mheg5_set_dbg_level(UINT16 ui2_level)
{

    _ui2_mheg5_dbg_level = ui2_level;
    return TRUE;

}

#ifdef MHEG5_IC_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: _mheg5_cli_ic_get_dbg_level
 *
 * Description: This API gets the current setting of mheg5 debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _mheg5_cli_ic_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_return;

    i4_return = x_cli_show_dbg_level(mheg5_ic_get_dbg_level());

    return i4_return;
}

/*-----------------------------------------------------------------------------
 * Name: _mheg5_cli_ic_set_dbg_level
 *
 * Description: This API sets the debug level of mheg5.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _mheg5_cli_ic_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;

    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);

    if (i4_return == CLIR_OK)
    {
        if (mheg5_ic_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_return = CLIR_CMD_EXEC_ERROR;
        }

        i4_return = CLIR_OK;
    }
    else
    {
        i4_return = CLIR_CMD_EXEC_ERROR;
    }

    x_cli_show_dbg_level(mheg5_ic_get_dbg_level());
    return i4_return;
}

#endif

/*-----------------------------------------------------------------------------
 * Name: _mheg5_cli_sct_get_dbg_level
 *
 * Description: This API gets the current setting of mheg5 debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _mheg5_cli_sct_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_return;

    i4_return = x_cli_show_dbg_level(mheg5_sct_get_dbg_level());

    return i4_return;
}

/*-----------------------------------------------------------------------------
 * Name: _mheg5_cli_sct_set_dbg_level
 *
 * Description: This API sets the debug level of mheg5.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _mheg5_cli_sct_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;

    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);

    if (i4_return == CLIR_OK)
    {
        if (mheg5_sct_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_return = CLIR_CMD_EXEC_ERROR;
        }

        i4_return = CLIR_OK;
    }
    else
    {
        i4_return = CLIR_CMD_EXEC_ERROR;
    }

    x_cli_show_dbg_level(mheg5_sct_get_dbg_level());
    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: _mheg5_cli_ssf_get_dbg_level
 *
 * Description: This API gets the current setting of mheg5 debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _mheg5_cli_ssf_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_return;

    i4_return = x_cli_show_dbg_level(mheg5_ssf_get_dbg_level());

    return i4_return;
}

/*-----------------------------------------------------------------------------
 * Name: _mheg5_cli_ssf_set_dbg_level
 *
 * Description: This API sets the debug level of mheg5.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _mheg5_cli_ssf_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;

    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);

    if (i4_return == CLIR_OK)
    {
        if (mheg5_ssf_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_return = CLIR_CMD_EXEC_ERROR;
        }

        i4_return = CLIR_OK;
    }
    else
    {
        i4_return = CLIR_CMD_EXEC_ERROR;
    }

    x_cli_show_dbg_level(mheg5_ssf_get_dbg_level());
    return i4_return;
}

#ifdef CI_PLUS_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: _mheg5_cli_ci_get_dbg_level
 *
 * Description: This API gets the current setting of mheg5 debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _mheg5_cli_ci_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_return;

    i4_return = x_cli_show_dbg_level(mheg5_ci_get_dbg_level());

    return i4_return;
}

/*-----------------------------------------------------------------------------
 * Name: _mheg5_cli_ci_set_dbg_level
 *
 * Description: This API sets the debug level of mheg5.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _mheg5_cli_ci_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;

    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);

    if (i4_return == CLIR_OK)
    {
        if (mheg5_ci_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_return = CLIR_CMD_EXEC_ERROR;
        }

        i4_return = CLIR_OK;
    }
    else
    {
        i4_return = CLIR_CMD_EXEC_ERROR;
    }

    x_cli_show_dbg_level(mheg5_ci_get_dbg_level());
    return i4_return;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: _mheg5_cli_misc_get_dbg_level
 *
 * Description: This API gets the current setting of mheg5 debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _mheg5_cli_misc_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_return;

    i4_return = x_cli_show_dbg_level(mheg5_misc_get_dbg_level());

    return i4_return;
}

/*-----------------------------------------------------------------------------
 * Name: _mheg5_cli_misc_set_dbg_level
 *
 * Description: This API sets the debug level of mheg5.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _mheg5_cli_misc_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;

    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);

    if (i4_return == CLIR_OK)
    {
        if (mheg5_misc_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_return = CLIR_CMD_EXEC_ERROR;
        }

        i4_return = CLIR_OK;
    }
    else
    {
        i4_return = CLIR_CMD_EXEC_ERROR;
    }

    x_cli_show_dbg_level(mheg5_misc_get_dbg_level());
    return i4_return;
}

/*-----------------------------------------------------------------------------
 * Name: _mheg5_cli_gui_get_dbg_level
 *
 * Description: This API gets the current setting of mheg5 debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _mheg5_cli_gui_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_return;

    i4_return = x_cli_show_dbg_level(mheg5_gui_get_dbg_level());

    return i4_return;
}

/*-----------------------------------------------------------------------------
 * Name: _mheg5_cli_gui_set_dbg_level
 *
 * Description: This API sets the debug level of mheg5.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _mheg5_cli_gui_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;

    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);

    if (i4_return == CLIR_OK)
    {
        if (mheg5_gui_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_return = CLIR_CMD_EXEC_ERROR;
        }

        i4_return = CLIR_OK;
    }
    else
    {
        i4_return = CLIR_CMD_EXEC_ERROR;
    }

    x_cli_show_dbg_level(mheg5_gui_get_dbg_level());
    return i4_return;
}



/*-----------------------------------------------------------------------------
 * Name: _mheg5_cli_dump_rendering_surface
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs: ui2_level            The new setting of debug level.
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _mheg5_cli_dump_rendering_surface(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;
    GL_LOCK_INFO_T  t_lock_info;

    if (x_handle_valid(g_t_surf.h_surf)) 
    {
        /* lock surface */
        i4_ret = x_gl_surface_lock(g_t_surf.h_surf, NULL, GL_SYNC, &t_lock_info);
        if (i4_ret != GLR_OK)
        {
            x_dbg_stmt("[MHEG5-OSD] %s #%d, lock surface error(%d)\n",__FUNCTION__, __LINE__, i4_ret);
            return CLIR_CMD_EXEC_ERROR;
        }
        

		x_dbg_stmt("[SRC INFO]\n\td.save.binary c:/mheg5_rnd_surf.dmp %p--%p\n\t",
			t_lock_info.at_subplane[0].pv_bits,
			(VOID*)((UINT32)(t_lock_info.at_subplane[0].pv_bits) + (t_lock_info.at_subplane[0].ui4_pitch) * (t_lock_info.at_subplane[0].ui4_height)));
        
        x_dbg_stmt("raw2png mheg5_rnd_surf.dmp %s %lu %lu %lu\n", 
			(t_lock_info.e_clrmode == GL_COLORMODE_ARGB_D4444) ? "4444" : "8888",
			t_lock_info.at_subplane[0].ui4_width,
			t_lock_info.at_subplane[0].ui4_height,
			t_lock_info.at_subplane[0].ui4_pitch);
		
        x_gl_surface_unlock(g_t_surf.h_surf, NULL);
    }

    if (x_handle_valid(g_t_surf.h_dest_graphics)) 
	{
		HANDLE_T h_dest_surf = NULL_HANDLE;

		i4_ret = x_gl_get_surface_on_graphice(g_t_surf.h_dest_graphics, &h_dest_surf);
		if (i4_ret == GLR_OK && x_handle_valid(h_dest_surf)) 
		{
			/* lock surface */
			i4_ret = x_gl_surface_lock(h_dest_surf, NULL, GL_SYNC, &t_lock_info);
			if (i4_ret != GLR_OK)
			{
				x_dbg_stmt("[MHEG5-OSD] %s #%d, lock surface error(%d)\n",__FUNCTION__, __LINE__, i4_ret);
				return CLIR_CMD_EXEC_ERROR;
			}
			
		
			x_dbg_stmt("[DEST INFO]\n\td.save.binary c:/dest_osd_surf.dmp %p--%p\n\t",
				t_lock_info.at_subplane[0].pv_bits,
				(VOID*)((UINT32)(t_lock_info.at_subplane[0].pv_bits) + (t_lock_info.at_subplane[0].ui4_pitch) * (t_lock_info.at_subplane[0].ui4_height)));
			
		    x_dbg_stmt("raw2png dest_osd_surf.dmp %s %lu %lu %lu\n", 
				(t_lock_info.e_clrmode == GL_COLORMODE_ARGB_D4444) ? "4444" : "8888",
				t_lock_info.at_subplane[0].ui4_width,
				t_lock_info.at_subplane[0].ui4_height,
				t_lock_info.at_subplane[0].ui4_pitch);
			
			x_gl_surface_unlock(h_dest_surf, NULL);
						
		}
	}

	return CLIR_OK;

}



#endif /* CLI_SUPPORT */

