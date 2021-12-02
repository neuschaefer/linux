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
 * $RCSfile: aee_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains the AEE CLI-related functions.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "aee/_aee.h"
#include "cli/x_cli.h"
#include "dbg/x_dbg.h"
#include "dbg/u_dbg.h"

#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * Name: aee_cli_show_debug_level
 *
 * Description: This API shows the current AEE debug level.
 *
 * Inputs:  i4_argc   Contains the argument count.
 *          pps_argv  Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK    Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 aee_cli_show_debug_level (INT32         i4_argc,
                                       const CHAR**  pps_argv)
{
    INT32  i4_res;
    
    i4_res = x_cli_show_dbg_level(aee_get_dbg_level());
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: aee_cli_set_debug_level
 *
 * Description: This API sets the AEE debug level.
 *
 * Inputs:  i4_argc   Contains the argument count.
 *          pps_argv  Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK    Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 aee_cli_set_debug_level (INT32         i4_argc,
                                      const CHAR**  pps_argv)
{
    INT32   i4_res;
    UINT16  ui2_dbg_level;
    
    i4_res = x_cli_parse_dbg_level(i4_argc, pps_argv, & ui2_dbg_level);
    
    if (i4_res == CLIR_OK)
    {
        i4_res = aee_set_dbg_level(ui2_dbg_level);
        
        i4_res = (i4_res == AEER_OK) ? CLIR_OK : CLIR_CMD_EXEC_ERROR;
    }
    else
    {
        i4_res = CLIR_CMD_EXEC_ERROR;
    }
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: aee_cli_list_aees
 *
 * Description: This API lists the AEEs.
 *
 * Inputs:  i4_argc   Contains the argument count.
 *          pps_argv  Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK    Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 aee_cli_list_aees (INT32         i4_argc,
                                const CHAR**  pps_argv)
{
    if (i4_argc == 1)
    {
        aee_list_all_aees_info();
    }
    else
    {
        INT32  i4_i;
        
        for (i4_i = 1; i4_i < i4_argc; i4_i++)
        {
            AEE_T*  pt_aee;
            
            pt_aee = aee_find_by_name(pps_argv[i4_i]);
            
            if (pt_aee != NULL)
            {
                aee_list_aee_info(pt_aee);
            }
            else
            {
                x_dbg_stmt(DBG_PREFIX"\"%s\" does not exist\n",
                           pps_argv[i4_i]);
            }
        }
    }
    
    return CLIR_OK;
}
#endif /* CLI_LVL_ALL */

/*-----------------------------------------------------------------------------
 * Name: aee_cli_init
 *
 * Description: This API attaches the AEE CLI tables if CLI_LVL_ALL is defined,
 *              otherwise it does nothing.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID aee_cli_init (VOID)
{
#ifdef CLI_LVL_ALL
    static CLI_EXEC_T  at_aee_cli_tbl[] = {
        /* get debug level */
        {
            CLI_GET_DBG_LVL_STR,
            NULL,
            aee_cli_show_debug_level,
            NULL,
            CLI_GET_DBG_LVL_HELP_STR,
            CLI_SUPERVISOR
        },
        /* set debug level */
        {
            CLI_SET_DBG_LVL_STR,
            NULL,
            aee_cli_set_debug_level,
            NULL,
            CLI_SET_DBG_LVL_HELP_STR,
            CLI_SUPERVISOR
        },
        /* show all AEEs */
        {
            "list",
            "l",
            aee_cli_list_aees,
            NULL,
            "List all AEEs",
            CLI_SUPERVISOR
        },
        END_OF_CLI_CMD_TBL
    };
    
    static CLI_EXEC_T at_aee_root_cli_tbl[] = {
        {
            "aee",
            NULL,
            NULL,
            at_aee_cli_tbl,
            "AEE commands",
            CLI_SUPERVISOR
        },
        END_OF_CLI_CMD_TBL
    };
    
    static BOOL  b_attached = FALSE;
    
    CRIT_STATE_T  t_state;
    BOOL          b_attach;
    
    t_state = x_crit_start();
    
    b_attach = ! b_attached;
    
    b_attached = TRUE;
    
    x_crit_end(t_state);
    
    if (b_attach)
    {
        INT32  i4_res;
        
        i4_res = x_cli_attach_cmd_tbl(at_aee_root_cli_tbl,
                                      CLI_CAT_MW,
                                      CLI_GRP_NONE);
        
        if (i4_res != CLIR_OK)
        {
            t_state = x_crit_start();
            
            b_attached = FALSE;
            
            x_crit_end(t_state);
        }
    }
#endif /* CLI_LVL_ALL */
}
