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
 * $RCSfile: wgl_progress_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_ATSC/3 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: a52d38c809a873b50922a8c51d2af345 $
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
#include "wgl/progress/wgl_progress_cli.h"


/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/


 /*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
 
static INT32 _wgl_pg_cli_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv); 
static INT32 _wgl_pg_cli_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv);

#endif
/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static UINT16 ui2_pg_dbg_level = DBG_INIT_LEVEL_MW_WGL_MNGR;

/*ProgressBar sub module command table */
static CLI_EXEC_T _at_wgl_pg_cmd_tbl[] = 
{
    {CLI_GET_DBG_LVL_STR,   NULL,       _wgl_pg_cli_get_dbg_level,     NULL,       CLI_GET_DBG_LVL_HELP_STR,           CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR,   NULL,       _wgl_pg_cli_set_dbg_level,     NULL,       CLI_SET_DBG_LVL_HELP_STR,           CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

/* Widget Manager root command table */
static CLI_EXEC_T _t_wgl_pg_root_cmd_tbl =
{
    "progress",     NULL,        NULL,      _at_wgl_pg_cmd_tbl,      "ProgressBar commands",   CLI_SUPERVISOR
};
#endif
/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/ 
/*******************************************************************************
 * Name
 *          wgl_pg_cli_dbg_level_get
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/                    
#ifdef CLI_LVL_ALL
UINT16 wgl_pg_cli_dbg_level_get()
{
    UINT16 ui2_mode = wgl_sets_cli_mode_get();
    
    switch (ui2_mode)
    {
    case WGL_SETS_CLI_MODE_SETS_CTL:
        return ui2_pg_dbg_level;
        
    case WGL_SETS_CLI_MODE_MGR_MASTER:
        return (ui2_pg_dbg_level & wgl_get_dbg_level());
        
    case WGL_SETS_CLI_MODE_BOTH:
        return (ui2_pg_dbg_level | wgl_get_dbg_level());
        
    case WGL_SETS_CLI_MODE_MGR_CTL:
    default:
        return wgl_get_dbg_level();
    }
}
#endif

/*******************************************************************************
 * Name
 *          wgl_pg_cli_dbg_level_set
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
VOID wgl_pg_cli_dbg_level_set(UINT16 ui2_new_level)
{
#ifdef CLI_LVL_ALL
    ui2_pg_dbg_level = ui2_new_level;
#endif
}

/*******************************************************************************
 * Name
 *          wgl_pg_cli_init
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_pg_cli_init()
{
#ifdef CLI_LVL_ALL 
    INT32       i4_ret;    
    
    /* Attach ProgressBar's CLI command table to Widget Manager's CLI table*/
    i4_ret = wgl_cli_reg_sub_table(&_t_wgl_pg_root_cmd_tbl);
    
    if (WGLR_OK != i4_ret)
    {
        return WGLR_INIT_FAILED;
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
 * Name: _wgl_pg_cli_get_dbg_level
 *
 * Description: 
 *
 * Inputs:  -
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 _wgl_pg_cli_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_return;
        
    if (i4_argc != 1)
    {
        x_dbg_stmt("{WGL(PG)} progress.gdl\n\r");
        return CLIR_OK;
    }    
    
    i4_return = x_cli_show_dbg_level(wgl_pg_cli_dbg_level_get());
    
    return (i4_return);
}
#endif

/*-----------------------------------------------------------------------------
 * Name: _wgl_pg_cli_set_dbg_level
 *
 * Description: 
 *
 * Inputs:  -
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 _wgl_pg_cli_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;     
        
    /* Check arguments */
    if ((i4_argc != 2) || (pps_argv == NULL))
    {
        x_dbg_stmt("{WGL(PG)} progress.sdl [level]\n\r");
        x_dbg_stmt("{WGL(PG)}   [level]: n=none/e=error, a=api, i=info");
        return CLIR_OK;
    }    
    
    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);
    
    if (i4_return == CLIR_OK)
    {
        wgl_pg_cli_dbg_level_set(ui2_dbg_level);
    }
    else
    {
        x_dbg_stmt("{WGL(PG)} progress.sdl [level]\n\r");
        x_dbg_stmt("{WGL(PG)}   [level]: n=none/e=error, a=api, i=info");
        i4_return = CLIR_OK;
    }
    
    return (i4_return);
}
#endif

