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
 * $RCSfile: rwl_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains functions and internal variables to support
 *         CLI control of the DT library.
 *
 *---------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "dbg/x_dbg.h"
#include "cli/x_cli.h"
#include "os/inc/x_os.h"
#include "rwlock/u_rwlock.h"
#include "rwlock/rwl_dbg.h"

#ifdef CLI_LVL_ALL

/* ----------------------------------------------------------------------
  Typedefs   
------------------------------------------------------------------------- */

/* DateTime module  CLI definitions. */

VOID  rwl_info(HANDLE_T h_rw_lock);

static INT32 cli_get_dbg_level (INT32  i4_argc, const CHAR**  pps_argv);
static INT32 cli_set_dbg_level (INT32  i4_argc, const CHAR**  pps_argv);
static INT32 rwl_show_owner_info (INT32  i4_argc, const CHAR**  pps_argv);

static CLI_EXEC_T at_cli_dbg_cmd[] =
{
    {"gdl",  NULL, cli_get_dbg_level, NULL,
     CLI_GET_DBG_LVL_HELP_STR,  CLI_SUPERVISOR},
    {"sdl",  NULL, cli_set_dbg_level, NULL,
     CLI_SET_DBG_LVL_HELP_STR,  CLI_SUPERVISOR},
    {"info",  NULL, rwl_show_owner_info, NULL,
     "Show status of a ReadWrite lock.",
     CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

static CLI_EXEC_T at_cli_dbg_tbl [] =
{
    {"rwl", NULL, NULL, &(at_cli_dbg_cmd[0]), "ReadWrite Lock Command", CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};



/* --------------------------------------------------------------
  Function implementation.
---------------------------------------------------------------- */

static  BOOL   rwl_cli_inited = FALSE;

INT32 rwl_cli_init(VOID)
{
    if ( rwl_cli_inited == FALSE )
    {
        /* Attach ReadWrite Lock library CLI command table to CLI engine. */
        if ( x_cli_attach_cmd_tbl(at_cli_dbg_tbl, CLI_CAT_MW, CLI_GRP_NONE)
             != CLIR_OK)
        {
            return RWLR_FAIL;
        }
        rwl_cli_inited = TRUE ;
    }
    return RWLR_OK;   
}


/*-------------------------------------------------------------------------
 * Name: cli_get_dbg_level
 *
 * Description: This API gets the current setting of DT module debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_get_dbg_level
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;
    
    if ( i4_argc != 1 )
    {
#ifdef DEBUG            
        x_dbg_stmt("Usage: rwl.gdl\n");
#endif          
        i4_rc = CLIR_INV_CMD_USAGE;
    }
    else
    {
        i4_rc = x_cli_show_dbg_level(rwl_get_dbg_level());
    }
    
    return i4_rc;
}

/*-------------------------------------------------------------------------
 * Name: cli_set_dbg_level
 *
 * Description: This API sets the current setting of DT module debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_set_dbg_level
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;
    UINT16    ui2_dbg;
    
    if ( i4_argc == 2             &&
         x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg) == CLIR_OK
        )
    {
        rwl_set_dbg_level(ui2_dbg);
        i4_rc = CLIR_OK;
    }    
    else
    {
#ifdef DEBUG            
        x_dbg_stmt("Usage: rwl.sdl <n|e|a|i>\n");
        x_dbg_stmt("where:  n=none, e=error, a=api, i=info. \n");
#endif        
        i4_rc=CLIR_INV_CMD_USAGE;
    }    
    return i4_rc;   
}

static INT32 rwl_show_owner_info
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32             i4_rc;
    HANDLE_T          h_rwl = NULL_HANDLE;
    
    if ( i4_argc == 2 )
    {
        h_rwl=x_strtoll(pps_argv[1], NULL, 10);
        rwl_info(h_rwl);
        i4_rc = CLIR_OK;
    }
    else
    {
#ifdef DEBUG        
        x_dbg_stmt("Usage: info <read-write-lock-handle> \n");
#endif        
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    return i4_rc;  
}

#endif  /* CLI_LVL_ALL */
    

    
