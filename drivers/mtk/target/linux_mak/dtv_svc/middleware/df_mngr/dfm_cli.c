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
 * $RCSfile: dfm_cli.c,v $
 * $Revision: #1 $ 
 * $Date: 2012/04/27 $ 
 * $Author: dtvbm11 $
 * $CCRevision$
 * $SWAuthor$
 * $MD5HEX$
 *
 * Description: 
 *         This file implements CLI command table for Download function manager.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "handle/x_handle.h"
#include "dbg/x_dbg.h"
#include "inc/x_sys_name.h"
#include "cli/x_cli.h"

#include "df_mngr.h"
#include "dfm_cli.h"
#include "dfm_dbg.h"
#include "u_df_mngr.h"
#include "x_df_mngr.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#ifdef  CLI_LVL_ALL

static HANDLE_T         t_dfm;

#endif  /* CLI_LVL_ALL */

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
#ifdef  CLI_LVL_ALL

static INT32 _dfm_cli_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _dfm_cli_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _dfm_cli_dump_rec_data  (INT32  i4_argc, const CHAR**  pps_argv);
static INT32 _dfm_cli_dump_rec_req  (INT32  i4_argc, const CHAR**  pps_argv);
static INT32 _dfm_cli_test_logo(INT32 i4_argc, const CHAR**  pps_argv);

/* DFM command table */
static CLI_EXEC_T at_dfm_cli_tbl[] = 
{
    {CLI_GET_DBG_LVL_STR, NULL, _dfm_cli_get_dbg_level, NULL, 
    CLI_GET_DBG_LVL_HELP_STR,            CLI_SUPERVISOR},

    {CLI_SET_DBG_LVL_STR,NULL,_dfm_cli_set_dbg_level,NULL,
    CLI_SET_DBG_LVL_HELP_STR,            CLI_SUPERVISOR},

    {"dumpdata",  NULL, _dfm_cli_dump_rec_data, NULL,
     "[dump ] display records from current DFM", CLI_SUPERVISOR},
     
    {"dumpreq",  NULL, _dfm_cli_dump_rec_req, NULL,
     "[dump ] display request from current DFM", CLI_SUPERVISOR},

    {"logo",  NULL, _dfm_cli_test_logo, NULL,
     "[logo ] set logo information and load logo data", CLI_SUPERVISOR},

    
    END_OF_CLI_CMD_TBL
};

/* DFM root command table */
static CLI_EXEC_T at_dfm_root_cmd_tbl[] =
{
    {"dfm",  NULL,   NULL,  at_dfm_cli_tbl,  "Download Function Manager commands",     CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

#endif  /* CLI_LVL_ALL */

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/

#ifdef  CLI_LVL_ALL

/*-----------------------------------------------------------------------------
 * Name: _dfm_cli_get_dbg_level
 *
 * Description: This API gets the current setting of DFM debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _dfm_cli_get_dbg_level(INT32        i4_argc, 
                                    const CHAR**    pps_argv)
{
    INT32       i4_return;
    
    
    /* Check arguments */
    if (i4_argc != 1)
    {
        x_dbg_stmt(_DFM_INFO"dfm.gdl\n\r");
        return CLIR_OK;
    }    
        
    i4_return = x_cli_show_dbg_level(dfm_get_dbg_level());

    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: _dfm_cli_set_dbg_level
 *
 * Description: This API sets the debug level of DF Manager.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _dfm_cli_set_dbg_level(INT32        i4_argc,
                                    const CHAR**    pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;     
    
    
    /* Check arguments */
    if ((i4_argc != 2) || 
        (pps_argv == NULL))
    {
        x_dbg_stmt(_DFM_INFO"dfm.sdl [level]\n\r");
        x_dbg_stmt(_DFM_INFO"  [level]: n=none/e=error, a=api, i=info\n\r");
        return CLIR_OK;
    }    
        
    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);
    
    if (i4_return == CLIR_OK)
    {
        if (dfm_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_return = CLIR_CMD_EXEC_ERROR;  
        }
    }
    else
    {
        x_dbg_stmt(_DFM_INFO"dfm.sdl [level]\n\r");
        x_dbg_stmt(_DFM_INFO"  [level]: n=none/e=error, a=api, i=info");
        i4_return = CLIR_OK;
    }

    x_dbg_stmt(_DFM_INFO"set debug level %s\n\r", (i4_return == CLIR_OK) ? "successful" : "failed");
    return i4_return;
}

/*Dump download function rec*/
static INT32 _dfm_cli_dump_rec_data(INT32  i4_argc, const CHAR**  pps_argv)
{
    INT32       i4_return;
    
    /* Check arguments */
    if ((i4_argc != 1) || 
        (pps_argv == NULL))
    {
        x_dbg_stmt(_DFM_INFO"dfm.dumpdata \n\r");
        return CLIR_OK;
    }

    dfm_dump_rec();
    
    i4_return = CLIR_OK;
    return i4_return;
}

/*Dump download function rec*/
static INT32 _dfm_cli_dump_rec_req(INT32  i4_argc, const CHAR**  pps_argv)
{
    INT32       i4_return;
    
    /* Check arguments */
    if ((i4_argc != 1) || 
        (pps_argv == NULL))
    {
        x_dbg_stmt(_DFM_INFO"dfm.dumqreq \n\r");
        return CLIR_OK;
    }

    dfm_dump_rec_req();
    
    i4_return = CLIR_OK;
    return i4_return;
}

VOID dfm_logo_nfy (HANDLE_T       h_logo,
                   DFM_COND_T     e_nfy_cond,
                   VOID*          pv_nfy_tag,
                   UINT32         ui4_data)
{
    if ((e_nfy_cond == DFM_COND_AVAILABLE)||
        (e_nfy_cond == DFM_COND_UPDATE))
    {
        x_dbg_stmt(_DFM_INFO"load logo OK! \n\r");
    }
    else
    {
        x_dbg_stmt(_DFM_INFO"load logo fail! \n\r");        
    }
    
    return ;
}


static INT32 _dfm_cli_test_logo(INT32 i4_argc, const CHAR**  pps_argv)
{
    INT32       i4_return;
    
    UINT16      ui2_on_id  =  /*0x7FE5;*/0x7D70;
    UINT16      ui2_svc_id = /*0x428;*/0xA000;
    UINT16      ui2_scan_id = /*0x428;*/14;
    
    HANDLE_T    h_logo;
    DFM_COND_T  e_dfm_cond;
    
    /* Check arguments */
    if ((i4_argc != 1) || 
        (pps_argv == NULL))
    {
        x_dbg_stmt(_DFM_INFO"dfm.logo \n\r");
        return CLIR_OK;
    }
    
    i4_return = x_dfm_open("dfm", &t_dfm);
    if (i4_return != DFMR_OK)
    {
        x_dbg_stmt(_DFM_INFO"open fail! \n\r");
        return CLIR_OK;
    }
    
    i4_return = x_dfm_load_logo(t_dfm,
                    ui2_on_id,
                    ui2_svc_id,
                    ui2_scan_id,
                    NULL,
                    dfm_logo_nfy,
                    &h_logo,
                    &e_dfm_cond);
    if (i4_return != DFMR_OK)
    {
        x_dbg_stmt(_DFM_INFO"load logo fail! \n\r");
        return CLIR_OK;
    }
    
    if ((e_dfm_cond == DFM_COND_AVAILABLE)||
        (e_dfm_cond == DFM_COND_UPDATE))
    {
        x_dbg_stmt(_DFM_INFO"load logo OK! \n\r");
        return CLIR_OK;
    }

    return CLIR_OK;
    
}
#endif

/*-----------------------------------------------------------------------------
 * Name: dfm_cli_init
 *
 * Description: This API initializes DFM engine CLI component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                     Routine successful.
 *          IOMR_CLI_ERROR              CLI operation is failed.
 ----------------------------------------------------------------------------*/
INT32 dfm_cli_init(VOID)
{
#ifdef CLI_LVL_ALL 
    INT32   i4_return;

    /* Attach DFM engine CLI command table to CLI */
    i4_return = x_cli_attach_cmd_tbl(at_dfm_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_NONE);
    
    if (i4_return != CLIR_OK)
    {
        return DFMR_NOT_INIT;
    }

#endif  /* CLI_LVL_ALL */
    return DFMR_OK;
}

