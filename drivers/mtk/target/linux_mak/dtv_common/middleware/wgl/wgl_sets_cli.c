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
 * $RCSfile: wgl_sets_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_ATSC/3 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: 76adf0c00cbc6bbac0dce09701922ff6 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "dbg/x_dbg.h"
#include "dbg/def_dbg_level_mw.h"
#include "cli/x_cli.h"
#include "wgl/wgl.h"
#include "wgl/_wgl_cli.h"
#include "wgl/wgl_sets_cli.h"


/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL 

#define WGL_SETS_CLI_CMD_NUMBER              4
#define WGL_SETS_CLI_GET_DBG_LVL_STR         "wb_gdl"
#define WGL_SETS_CLI_SET_DBG_LVL_STR         "wb_sdl"
#define WGL_SETS_CLI_GET_DBG_LVL_HELP_STR    "Get Widget-Sets Basic Functions debug level"
#define WGL_SETS_CLI_SET_DBG_LVL_HELP_STR    "Set Widget-Sets Basic Functions debug level"
#define WGL_SETS_CLI_GET_DBG_MOD_STR         "ws_gmd"
#define WGL_SETS_CLI_SET_DBG_MOD_STR         "ws_smd"
#define WGL_SETS_CLI_GET_DBG_MOD_HELP_STR    "Get Widget-Sets debug mode"
#define WGL_SETS_CLI_SET_DBG_MOD_HELP_STR    "Set Widget-Sets debug mode"

#endif
 /*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL 

static INT32 _wgl_sets_cli_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv); 
static INT32 _wgl_sets_cli_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _wgl_sets_cli_get_mode(INT32 i4_argc, const CHAR** pps_argv); 
static INT32 _wgl_sets_cli_set_mode(INT32 i4_argc, const CHAR** pps_argv);

#endif
/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL 
static UINT16 ui2_wgl_sets_dbg_level = DBG_INIT_LEVEL_MW_WGL_MNGR;
static UINT16 ui2_wgl_sets_mode = WGL_SETS_CLI_MODE_MGR_MASTER;
#endif

#ifdef CLI_LVL_ALL
/* Widget Manager root command table */
static CLI_EXEC_T _t_wgl_sets_cmd_tbl[WGL_SETS_CLI_CMD_NUMBER] =
{
    {WGL_SETS_CLI_GET_DBG_LVL_STR,     NULL,        _wgl_sets_cli_get_dbg_level,      NULL,      WGL_SETS_CLI_GET_DBG_LVL_HELP_STR,   CLI_SUPERVISOR},
    {WGL_SETS_CLI_SET_DBG_LVL_STR,     NULL,        _wgl_sets_cli_set_dbg_level,      NULL,      WGL_SETS_CLI_SET_DBG_LVL_HELP_STR,   CLI_SUPERVISOR},
    {WGL_SETS_CLI_GET_DBG_MOD_STR,     NULL,        _wgl_sets_cli_get_mode,           NULL,      WGL_SETS_CLI_GET_DBG_MOD_HELP_STR,   CLI_SUPERVISOR},
    {WGL_SETS_CLI_SET_DBG_MOD_STR,     NULL,        _wgl_sets_cli_set_mode,           NULL,      WGL_SETS_CLI_SET_DBG_MOD_HELP_STR,   CLI_SUPERVISOR}
};

#endif
/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/ 
/*******************************************************************************
 * Name
 *          wgl_sets_cli_dbg_level_get
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
#ifdef CLI_LVL_ALL 
UINT16 wgl_sets_cli_dbg_level_get()
{
    UINT16 ui2_mode = wgl_sets_cli_mode_get();

    switch (ui2_mode)
    {
    case WGL_SETS_CLI_MODE_SETS_CTL:
        return ui2_wgl_sets_dbg_level;

    case WGL_SETS_CLI_MODE_MGR_MASTER:
        return (ui2_wgl_sets_dbg_level & wgl_get_dbg_level());

    case WGL_SETS_CLI_MODE_BOTH:
        return (ui2_wgl_sets_dbg_level | wgl_get_dbg_level());
    
    case WGL_SETS_CLI_MODE_MGR_CTL:
    default:
        return wgl_get_dbg_level();
    }
}
#endif

/*******************************************************************************
 * Name
 *          wgl_sets_cli_dbg_level_set
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
#ifdef CLI_LVL_ALL
VOID wgl_sets_cli_dbg_level_set(UINT16 ui2_new_level)
{
    ui2_wgl_sets_dbg_level = ui2_new_level;
}
#endif

/*******************************************************************************
 * Name
 *          wgl_sets_cli_mode_get
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
UINT16 wgl_sets_cli_mode_get()
{
#ifdef CLI_LVL_ALL
    return ui2_wgl_sets_mode;
#else
    return 0;
#endif
}

/*******************************************************************************
 * Name
 *          wgl_sets_cli_mode_set
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
VOID wgl_sets_cli_mode_set(UINT16 ui2_new_mode)
{
#ifdef CLI_LVL_ALL
    switch (ui2_new_mode)
    {
    case WGL_SETS_CLI_MODE_MGR_CTL:
    case WGL_SETS_CLI_MODE_SETS_CTL:
    case WGL_SETS_CLI_MODE_MGR_MASTER:
    case WGL_SETS_CLI_MODE_BOTH:
        ui2_wgl_sets_mode = ui2_new_mode;
        break;

    default:
        ui2_wgl_sets_mode = WGL_SETS_CLI_MODE_MGR_CTL;
        break;
    }
#endif
}

/*******************************************************************************
 * Name
 *          wgl_sets_cli_init
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_sets_cli_init()
{
#ifdef CLI_LVL_ALL 
    INT32       i4_ret, i;    
    
    /* Attach Basic's CLI command table to Widget Manager's CLI table*/
    for(i = 0; i < WGL_SETS_CLI_CMD_NUMBER; i++)
    {
        i4_ret = wgl_cli_reg_sub_table(&_t_wgl_sets_cmd_tbl[i]);
        if (WGLR_OK != i4_ret)
        {
            return WGLR_INIT_FAILED;
        }
    }
    
    return WGLR_OK;   
#else    
    return WGLR_OK;
#endif      
}

/*-----------------------------------------------------------------------------
 * private VIEW methods implementations
 *---------------------------------------------------------------------------*/ 
/*-----------------------------------------------------------------------------
 * Name: _wgl_sets_cli_get_dbg_level
 *
 * Description: 
 *
 * Inputs:  -
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 _wgl_sets_cli_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_return;
        
    if (i4_argc != 1)
    {
        x_dbg_stmt("{WGL(SETS)} wb_gdl\n\r");
        return CLIR_OK;
    }    
    
    i4_return = x_cli_show_dbg_level(wgl_sets_cli_dbg_level_get());
    
    return (i4_return);
}
#endif

/*-----------------------------------------------------------------------------
 * Name: _wgl_sets_cli_set_dbg_level
 *
 * Description: 
 *
 * Inputs:  -
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 _wgl_sets_cli_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;     
        
    /* Check arguments */
    if ((i4_argc != 2) || (pps_argv == NULL))
    {
        x_dbg_stmt("{WGL(SETS)} wb_sdl [level]\n\r");
        x_dbg_stmt("{WGL(SETS)}   [level]: n=none/e=error, a=api, i=info");
        return CLIR_OK;
    }    
    
    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);
    
    if (i4_return == CLIR_OK)
    {
        wgl_sets_cli_dbg_level_set(ui2_dbg_level);
    }
    else
    {
        x_dbg_stmt("{WGL(SETS)} wb_sdl [level]\n\r");
        x_dbg_stmt("{WGL(SETS)}   [level]: n=none/e=error, a=api, i=info");
        i4_return = CLIR_OK;
    }
    
    return (i4_return);
}
#endif

/*-----------------------------------------------------------------------------
 * Name: _wgl_sets_cli_get_mode
 *
 * Description: 
 *
 * Inputs:  -
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 _wgl_sets_cli_get_mode(INT32 i4_argc, const CHAR** pps_argv)
{
    UINT16      ui2_mode;
    
    if (i4_argc != 1)
    {
        x_dbg_stmt("{WGL(SETS)} ws_gmd\n\r");
        return CLIR_OK;
    }    
    
    ui2_mode = wgl_sets_cli_mode_get();

    x_dbg_stmt("Debug mode = ");
    
    switch (ui2_mode)
    {
    case WGL_SETS_CLI_MODE_SETS_CTL:
        {
            x_dbg_stmt("s");
        }
        break;
        
    case WGL_SETS_CLI_MODE_MGR_MASTER:
        {
            x_dbg_stmt("a");
        }
        break;
        
    case WGL_SETS_CLI_MODE_BOTH:
        {
            x_dbg_stmt("b");
        }
        break;
        
    case WGL_SETS_CLI_MODE_MGR_CTL:
    default:
        {
            x_dbg_stmt("m");
        }
        break;
    }

    x_dbg_stmt("\n\r");
    
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: _wgl_sets_cli_set_mode
 *
 * Description: 
 *
 * Inputs:  -
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 _wgl_sets_cli_set_mode(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_ret = CLIR_OK;
    UINT16      ui2_mode;
    const CHAR* ps_mode;
    
    /* Check arguments */
    if ((i4_argc != 2) || (pps_argv == NULL))
    {
        x_dbg_stmt("{WGL(SETS)} ws_smd [mode]\n\r");
        x_dbg_stmt("              [mode]: \n\r");
        x_dbg_stmt("                      m = mgr's debug level is used for all widget components\n\r");
        x_dbg_stmt("                      s = individual component's debug level is used.\n\r");
        x_dbg_stmt("                      a = the AND result of mgr's and individual component's debug level is used.\n\r");
        x_dbg_stmt("                      b = the OR result of mgr's and individual component's debug levels are used.\n\r");
        
        return CLIR_OK;
    }    
    
    ps_mode = pps_argv[1];
    
    switch (*ps_mode)
    {
    case 'm':
    case 'M':
        ui2_mode = WGL_SETS_CLI_MODE_MGR_CTL;
        break;
        
    case 's':
    case 'S':
        ui2_mode = WGL_SETS_CLI_MODE_SETS_CTL;
        break;
        
    case 'a':
    case 'A':      
        ui2_mode = WGL_SETS_CLI_MODE_MGR_MASTER;         
        break;

    case 'b':
    case 'B':      
        ui2_mode = WGL_SETS_CLI_MODE_BOTH;         
        break;
        
    default:
        i4_ret = CLIR_INV_ARG;
        break;
    }
    
    if (CLIR_OK == i4_ret)
    {
        wgl_sets_cli_mode_set(ui2_mode);
    }
    else
    {
        x_dbg_stmt("{WGL(SETS)} ws_smd [mode]\n\r");
        x_dbg_stmt("              [mode]: \n\r");
        x_dbg_stmt("                      m = mgr's debug level is used for all widget components\n\r");
        x_dbg_stmt("                      s = individual component's debug level is used.\n\r");
        x_dbg_stmt("                      a = individual component's debug level in mgr's debug level is used.\n\r");
        x_dbg_stmt("                      b = both mgr's and individual component's debug levels are used.\n\r");
        i4_ret = CLIR_OK;
    }
    
    return (i4_ret);
}
#endif
