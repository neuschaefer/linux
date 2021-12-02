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
 * $RCSfile: prc_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_ATSC/8 $
 * $SWAuthor: Felisa Hsiao $
 * $MD5HEX: 10766088a799ef3f29e158e43c4b93e4 $
 *
 * Description: 
 *         This file contains PM_ESDB and PM_SVDB which are the control data for 
 *         whole PM module.
 * History:
 *                
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "prc_mngr/prc_mngr.h"
#include "prc_mngr/prc_common.h"
#include "prc_mngr/prc_db.h"
#include "prc_mngr/prc_lock.h"
#include "prc_mngr/prc_util.h"
#include "prc_mngr/prc_debug.h"
#include "prc_mngr/prc_ca.h"
#include "cli/x_cli.h"

#if (PM_ENABLE_FILTER_CTRL)
#include "prc_mngr/prc_filter.h"
#include "prc_mngr/prc_filter_db.h"
#include "svctx/svctx.h"
#include "svctx/ch_hdlr/ch_svc_req.h"
#include "svctx/util/player.h"
#endif

#if (PM_ENABLE_MULTIMEDIA)
#include "conn_mngr/u_cm.h"
#include "conn_mngr/x_cm.h"
#include "conn_mngr/brdcst/u_brdcst.h"
#include "conn_mngr/cm_playback/u_playback_handler.h"
#endif  /* (PM_ENABLE_MULTIMEDIA) */


#ifdef CLI_LVL_ALL

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
BOOL g_fg_SCRS_debug = FALSE;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static INT32 pmi_cli_resource               ( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_service_info           ( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_esnode_info            ( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_ctrl_scrs_log          ( INT32 i4_argc, const CHAR** pps_argv);
#if PM_ENABLE_CA
static INT32 pmi_cli_ca_info                ( INT32 i4_argc, const CHAR** pps_argv);
#endif
static INT32 pmi_cli_get_dbg_level          ( INT32 i4_argc, const CHAR** pps_argv);  
static INT32 pmi_cli_set_dbg_level          ( INT32 i4_argc, const CHAR** pps_argv);
                                   
#if PM_ENABLE_SCRAMBLE_DETECTION                                    
static INT32 pmi_cli_dmx_scramble_nfy       ( INT32 i4_argc, const CHAR** pps_argv);
extern BOOL  pmi_scramble_status_nfy_fct(VOID*         pv_nfy_tag,
                                        DEMUX_COND_T  e_nfy_cond,
                                        UINT32        ui4_data_1,
                                        UINT32        ui4_data_2);                     
#endif              

#if (PM_ENABLE_FILTER_CTRL)
static INT32 pmi_cli_pidfilter_update       ( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_pidctrl_resource       ( INT32 i4_argc, const CHAR** pps_argv);
#endif                                                                                                             
 
#if (PM_ENABLE_MULTIMEDIA)
static INT32 pmi_cli_test_init              ( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_set_test_mode          ( INT32 i4_argc, const CHAR** pps_argv);
            
static INT32 pmi_cli_open_svc               ( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_play_svc               ( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_stop_svc               ( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_close_svc              ( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_get_svc_cond           ( INT32 i4_argc, const CHAR** pps_argv);
        
static INT32 pmi_cli_add_comp               ( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_play_comp              ( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_stop_comp              ( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_del_comp               ( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_get_comp_cond          ( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_get_comp_info          ( INT32 i4_argc, const CHAR** pps_argv);
        
static INT32 pmi_cli_set_cmd                ( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_get_cmd                ( INT32 i4_argc, const CHAR** pps_argv);
        
#if 1        
    #define PM_CLI_ENABLE_UT    1
#else        
    #define PM_CLI_ENABLE_UT    0
#endif

#if 1        
    #define PM_CLI_DETAIL_LOG   1
#else        
    #define PM_CLI_DETAIL_LOG   0
#endif                                                                                                             
 
#if (PM_CLI_ENABLE_UT)
static INT32 pmi_cli_abn_loop_add_comp      ( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_abn_stop_svc_after_open( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_abn_play_svc_no_comp   ( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_abn_play_deleted_comp  ( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_abn_play_svc_no_opened ( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_abn_play_svc_deleted   ( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_abn_play_comp_wrong_pid( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_abn_play_comp_no_cond  ( INT32 i4_argc, const CHAR** pps_argv);
static INT32 pmi_cli_stress_test_for_res    ( INT32 i4_argc, const CHAR** pps_argv);
#endif  /* (PM_CLI_ENABLE_UT) */
#endif  /* (PM_ENABLE_MULTIMEDIA) */

 
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
#if (PM_ENABLE_MULTIMEDIA)
#if (PM_CLI_ENABLE_UT)
static CLI_EXEC_T at_pm_abn_cmd_tbl[] = 
{
    {
        "lac",      "lac",                                  pmi_cli_abn_loop_add_comp,
        NULL,       "loop add component to SVC",            CLI_SUPERVISOR
    },
    {
        "iss",      "iss",                                  pmi_cli_abn_stop_svc_after_open,
        NULL,       "immediately stop SVC after open",      CLI_SUPERVISOR
    },
    {
        "psnc",     "psnc",                                 pmi_cli_abn_play_svc_no_comp,
        NULL,       "play SVC without component",           CLI_SUPERVISOR
    },
    {
        "pcad",     "pcad",                                 pmi_cli_abn_play_deleted_comp,
        NULL,       "play component after deleting it",     CLI_SUPERVISOR
    },
    {
        "psno",     "psno",                                 pmi_cli_abn_play_svc_no_opened,
        NULL,       "directly play SVC without open",       CLI_SUPERVISOR
    },
    {
        "pssd",     "pssd",                                 pmi_cli_abn_play_svc_deleted,
        NULL,       "play SVC after deleting it",           CLI_SUPERVISOR
    },
    {
        "pcwp",     "pcwp",                                 pmi_cli_abn_play_comp_wrong_pid,
        NULL,       "open SVC/add comp with wrong PID",     CLI_SUPERVISOR
    },
    {
        "pcnp",     "pcnp",                                 pmi_cli_abn_play_comp_no_cond,
        NULL,       "open SVC/add comp without condition",  CLI_SUPERVISOR
    },
    {
        "stfr",     "stfr",                                 pmi_cli_stress_test_for_res,
        NULL,       "stress test for resource used in PM",  CLI_SUPERVISOR
    },

    END_OF_CLI_CMD_TBL
};
#endif  /* (PM_CLI_ENABLE_UT) */
#endif  /* (PM_ENABLE_MULTIMEDIA) */

/* Stream Manager command table */
static CLI_EXEC_T at_pm_cmd_tbl[] = 
{
    {"resource",    "res",      pmi_cli_resource,           NULL,   "Resource statistics",                  CLI_SUPERVISOR},
    {"service",     "sv",       pmi_cli_service_info,       NULL,   "Get service information",              CLI_SUPERVISOR},
    {"comp",        "comp",     pmi_cli_esnode_info,        NULL,   "Get component info[component index]",  CLI_SUPERVISOR},
    {"scrs",        "sc",       pmi_cli_ctrl_scrs_log,      NULL,   "control SCRS log[1:enable/0:disable]", CLI_SUPERVISOR},
#if PM_ENABLE_CA
    {"casys",       "cs",       pmi_cli_ca_info,            NULL,   "Get get CA system information",        CLI_SUPERVISOR},
#endif
#if PM_ENABLE_SCRAMBLE_DETECTION
    {"scramblenfy", "scnfy",    pmi_cli_dmx_scramble_nfy,   NULL,   "Simulate demux scramble status notify $1: tag  $2: 1: Scrambled 0: Free",  CLI_SUPERVISOR}, 
#endif    

#if (PM_ENABLE_FILTER_CTRL)    
    {"pidctrlres",  "ctrlres",  pmi_cli_pidctrl_resource,   NULL,   "PID filter ctrl Resource statistics",  CLI_SUPERVISOR},
    {"updatees",    "update",   pmi_cli_pidfilter_update,   NULL,   "Simulate demux update $1: ESNODE",     CLI_SUPERVISOR}, 
#endif
    {CLI_GET_DBG_LVL_STR, NULL, pmi_cli_get_dbg_level,      NULL,   CLI_GET_DBG_LVL_HELP_STR,               CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR, NULL, pmi_cli_set_dbg_level,      NULL,   CLI_SET_DBG_LVL_HELP_STR,               CLI_SUPERVISOR},    
        
#if (PM_ENABLE_MULTIMEDIA)
    {
        "testinit", "ti",                           pmi_cli_test_init,
        NULL,       "prepare CM handle for test",   CLI_SUPERVISOR
    },
    {
        "setmode",  "stm",                          pmi_cli_set_test_mode,
        NULL,       "set test mode",                CLI_SUPERVISOR
    },
    {
        "opensvc",  "osvc",                         pmi_cli_open_svc,
        NULL,       "open PM service",              CLI_SUPERVISOR
    },
    {
        "playsvc",  "psvc",                         pmi_cli_play_svc, 
        NULL,       "play PM service",              CLI_SUPERVISOR
    },
    {
        "stopsvc",  "ssvc",                         pmi_cli_stop_svc, 
        NULL,       "stop PM service",              CLI_SUPERVISOR
    },
    {
        "closesvc", "csvc",                         pmi_cli_close_svc, 
        NULL,       "close PM service",             CLI_SUPERVISOR
    },
    {
        "getsvcc",  "gsvcc",                        pmi_cli_get_svc_cond, 
        NULL,       "get PM service condition",     CLI_SUPERVISOR
    },
    {
        "addcomp",  "acom",                         pmi_cli_add_comp, 
        NULL,       "add PM component",             CLI_SUPERVISOR
    },
    {
        "playcomp", "pcom",                         pmi_cli_play_comp, 
        NULL,       "play PM component",            CLI_SUPERVISOR
    },
    {
        "stopcomp", "scom",                         pmi_cli_stop_comp, 
        NULL,       "stop PM component",            CLI_SUPERVISOR
    },
    {
        "delcomp",  "dcom",                         pmi_cli_del_comp, 
        NULL,       "delete PM component",          CLI_SUPERVISOR
    },
    {
        "getcompc", "gcomc",                        pmi_cli_get_comp_cond, 
        NULL,       "get PM component condition",   CLI_SUPERVISOR
    },
    {
        "getcompi", "gcomi",                        pmi_cli_get_comp_info, 
        NULL,       "get PM component information", CLI_SUPERVISOR
    },
    {
        "setcmd",   "scmd",                         pmi_cli_set_cmd, 
        NULL,       "set MultiMedia command to PM", CLI_SUPERVISOR
    },
    {
        "getcmd",   "gcmd",                         pmi_cli_get_cmd,
        NULL,       "get MultiMedia command to PM", CLI_SUPERVISOR
    },
    #if (PM_CLI_ENABLE_UT)
    {
        "abn",              NULL,                   NULL,
        at_pm_abn_cmd_tbl,  "PM abnormal test",     CLI_SUPERVISOR
    },
    #endif  /* (PM_CLI_ENABLE_UT) */
#endif  /* (PM_ENABLE_MULTIMEDIA) */ 
    
    END_OF_CLI_CMD_TBL
};


/* Stream Manager root command table */
static CLI_EXEC_T at_pm_root_cmd_tbl[] =
{
    {
        "pm",
        NULL,
        NULL,
        at_pm_cmd_tbl,
        "PM command",
        CLI_SUPERVISOR
    },
    END_OF_CLI_CMD_TBL
};

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: pmi_cli_init
 *
 * Description: Initialize CLI command. 
 *
 * Inputs:  -      
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/ 
INT32 pmi_cli_init( VOID )
{
    INT32   i4_ret;
    
    i4_ret = x_cli_attach_cmd_tbl( at_pm_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_PIPE);
    if ( i4_ret != CLIR_OK && i4_ret != CLIR_NOT_INIT )
    {
        return PMR_FAILED;
    }
    
    return PMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: pmi_cli_resource
 *
 * Description: PM internal resource statistics. 
 *
 * Inputs:  -      
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/ 
static INT32 pmi_cli_resource(INT32          i4_argc, 
                              const CHAR**   pps_argv)
{
    /* Lock */
    pmi_lock_db_wr();
    
    pmi_db_resource_statistics_4cli();
    
    /* Unlock */
    pmi_unlock_db();
    
    return 0;
}
/*-----------------------------------------------------------------------------
 * Name: pmi_cli_service_info
 *
 * Description: Dump all servicee information. 
 *
 * Inputs:  -      
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/ 
INT32 pmi_cli_service_info(INT32          i4_argc, 
                           const CHAR**   pps_argv)
{
    /* Lock */
    pmi_lock_db_wr();
    
    pmi_service_dump_4cli();    
    
    /* Unlock */
    pmi_unlock_db();    
    
    return 0;
}
/*-----------------------------------------------------------------------------
 * Name: pmi_cli_esnode_info
 *
 * Description: Dump the specified ES component information. 
 *
 * Inputs:  -      
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/ 
INT32 pmi_cli_esnode_info( INT32 i4_argc, const CHAR**   pps_argv )
{
    UINT16  ui2_es_idx;

    if (i4_argc != 2)
    {        
        return CLIR_INV_CMD_USAGE;
    }    
    
    ui2_es_idx = x_strtoll(pps_argv[1], NULL, 10);
        
    /* Lock */
    pmi_lock_db_wr();

    pmi_component_dump_4cli( ui2_es_idx );
    
    /* Unlock */
    pmi_unlock_db();    
    
    return 0;
}


INT32 pmi_cli_ctrl_scrs_log( INT32 i4_argc, const CHAR**   pps_argv )
{
    UINT16  ui2_flag;

    if (i4_argc != 2)
    {
        x_dbg_stmt( "please input [sc 1] or [sc 0]\n" );
        return CLIR_INV_CMD_USAGE;
    }
    
    ui2_flag = x_strtoll(pps_argv[1], NULL, 10);

    if (1 == ui2_flag)
    {
        g_fg_SCRS_debug = TRUE;
    }

    if (0 == ui2_flag)
    {
        g_fg_SCRS_debug = FALSE;
    }
    
    return 0;
}


#if PM_ENABLE_CA
INT32 pmi_cli_ca_info( INT32 i4_argc, const CHAR**   pps_argv )
{
    pmi_lock_db_wr();

    pmi_ca_sys_dump_4cli();

    pmi_unlock_db();

    return 0;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: pmi_cli_get_dbg_level
 *
 * Description: This API gets the current setting of Process Manager debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 pmi_cli_get_dbg_level(INT32        i4_argc, 
                                    const CHAR** pps_argv)
{
    INT32       i4_return;
    
    /* iom.gdl */
    
    if (i4_argc != 1)
    {
        x_dbg_stmt("pm.gdl\n\r");
        return CLIR_INV_CMD_USAGE;
    }    
        
    i4_return = x_cli_show_dbg_level(pmi_dbg_get_level());

    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: pmi_cli_set_dbg_level
 *
 * Description: This API sets the debug level of Process Manager.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage. 
 ----------------------------------------------------------------------------*/
static INT32 pmi_cli_set_dbg_level(INT32        i4_argc, 
                                   const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;     
    
    /* iom.sdl [level] */
    
    /* Check arguments */
    if ((i4_argc != 2) || 
        (pps_argv == NULL))
    {
        x_dbg_stmt("pm.sdl [level]\n\r");
        x_dbg_stmt("  [level]: n=none/e=error, a=api, i=info\n\r");
        return CLIR_INV_CMD_USAGE;
    }    
        
    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);
    
    if (i4_return == CLIR_OK)
    {
        if (pmi_dbg_set_level(ui2_dbg_level) == FALSE)
        {
            i4_return = CLIR_CMD_EXEC_ERROR;  
        }
        else
        {
            i4_return = x_cli_show_dbg_level(pmi_dbg_get_level());
        }
    }
    else
    {
        x_dbg_stmt("pm.sdl [level]\n\r");
        x_dbg_stmt("  [level]: n=none/e=error, a=api, i=info\n\r");
        return CLIR_INV_CMD_USAGE;
    }

    return i4_return;
}
#if PM_ENABLE_SCRAMBLE_DETECTION        
/*-----------------------------------------------------------------------------
 * Name: pmi_cli_dmx_scramble_nfy
 *
 * Description: This API is used to simulate demux scramble status notification.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage. 
 ----------------------------------------------------------------------------*/                            
static INT32 pmi_cli_dmx_scramble_nfy(INT32        i4_argc, 
                                      const CHAR** pps_argv)
{
    INT32   pt_esdb;
    INT32   status;
    
    if (i4_argc != 3)
    {        
        return CLIR_INV_CMD_USAGE;
    }    
    
    pt_esdb = x_strtoll(pps_argv[1], NULL, 10);
    status = x_strtoll(pps_argv[2], NULL, 10);
    
    if (status == 1)
    {
        pmi_scramble_status_nfy_fct((VOID*) pt_esdb,
                                    DEMUX_COND_SCRS,
                                    DEMUX_SCRS_SCRAMBLED,
                                    0);
    }
    else
    {
        pmi_scramble_status_nfy_fct((VOID*) pt_esdb,
                                    DEMUX_COND_SCRS,
                                    DEMUX_SCRS_CLEAR,
                                    0);
    }
    
    return CLIR_OK;
}                                          
#endif 

#if (PM_ENABLE_FILTER_CTRL)
/*-----------------------------------------------------------------------------
 * Name: pmi_cli_pidfilter_update
 *
 * Description: This API is used to simulate demux PID update.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage. 
 ----------------------------------------------------------------------------*/                            
static INT32 pmi_cli_pidfilter_update(INT32        i4_argc, 
                                      const CHAR** pps_argv)
{
    INT32   pt_esdb;
    
    if (i4_argc != 2)
    {        
        return CLIR_INV_CMD_USAGE;
    }    
    
    pt_esdb = x_strtoll(pps_argv[1], NULL, 10);
    
    pmi_filter_update((PM_COMPONENT_INFO_T*) pt_esdb);
    
    return CLIR_OK;
}   

/*-----------------------------------------------------------------------------
 * Name: pmi_cli_pidctrl_resource
 *
 * Description: PM PID filter control internal resource statistics. 
 *
 * Inputs:  -      
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/ 
static INT32 pmi_cli_pidctrl_resource(INT32          i4_argc, 
                                      const CHAR**   pps_argv)
{
    /* Lock */
    pmi_lock_db_wr();
    
    pmi_filter_db_resource_statistics_4cli();
    
    /* Unlock */
    pmi_unlock_db();
    
    return 0;
}            
#endif                           


#if (PM_ENABLE_MULTIMEDIA)
HANDLE_T    g_h_pm_cli_conn = NULL_HANDLE;
HANDLE_T    g_h_pm_cli_svc  = NULL_HANDLE;
HANDLE_T    g_h_pm_cli_comp = NULL_HANDLE;
HANDLE_T    g_h_pm_cli_mmp_svc  = NULL_HANDLE;
BOOL        g_fg_test_mode  = TRUE;/* FALSE: manual mode; TRUE: auto mode */


static INT32 pmi_cli_test_init      ( INT32 i4_argc, const CHAR** pps_argv)
{
    if(i4_argc != 2)
    {
        x_dbg_stmt("command [cm_handle]\n");
        x_dbg_stmt("    please input the CM handle\n");
        return 0;
    }

    g_h_pm_cli_conn = (UINT32)x_strtoull(pps_argv[1], NULL, 10);
    
    return 0;
}


static INT32 pmi_cli_set_test_mode  ( INT32 i4_argc, const CHAR** pps_argv)
{
    INT32   i4_parameter_mode = 0;
    
    if(i4_argc != 2)
    {
        x_dbg_stmt("command [parameter_mode]\n");
        x_dbg_stmt("    parameter_mode: 0:manual parameter,1:auto parameter\n");
        return 0;
    }

    i4_parameter_mode = (UINT32)x_strtoull(pps_argv[1], NULL, 10);

    if (0 == i4_parameter_mode)
    {
        g_fg_test_mode = FALSE;
        x_dbg_stmt("It is manual mode now, you must input the parameter yourself\n");
    }
    else if (1 == i4_parameter_mode)
    {
        x_dbg_stmt("It is auto mode now, it will input parameter for you\n");
        g_fg_test_mode = TRUE;
    }
    else
    {
        x_dbg_stmt("command [parameter_mode]\n");
        x_dbg_stmt("    parameter_mode: 0:manual parameter,1:auto parameter\n");
        return 0;
    }
    
    return 0;
}


#if (PM_ENABLE_RECORD)
static VOID pm_cli_filter_nfy_fct( 
        HANDLE_T                h_component,
        PM_COND_T               e_nfy_cond,                            
        UINT32                  ui4_detail_code,
        VOID*                   pv_nfy_tag )
{
#if (PM_CLI_DETAIL_LOG)
    x_dbg_stmt( 
        "%s, component handle(0x%x), component index(%d)\n",
        __func__,
        h_component,
        (UINT32)pv_nfy_tag );

    switch (e_nfy_cond)
    {
        case PM_COND_CLOSED:
            x_dbg_stmt( "   new condition is %s\n", "PM_COND_CLOSED" );
            break;
        case PM_COND_OPENING:
            x_dbg_stmt( "   new condition is %s\n", "PM_COND_OPENING" );
            break;
        case PM_COND_CLOSING:
            x_dbg_stmt( "   new condition is %s\n", "PM_COND_CLOSING" );
            break;
        case PM_COND_PLAYING:
            x_dbg_stmt( "   new condition is %s\n", "PM_COND_PLAYING" );
            break;
        case PM_COND_PLAYED:
            x_dbg_stmt( "   new condition is %s\n", "PM_COND_PLAYED" );
            break;
        case PM_COND_STOPPING:
            x_dbg_stmt( "   new condition is %s\n", "PM_COND_STOPPING" );
            break;
        case PM_COND_STOPPED:
            x_dbg_stmt( "   new condition is %s\n", "PM_COND_STOPPED" );
            break;
        default :
            break;
    }

    x_dbg_stmt( "   detail  code  is 0x%x\n", ui4_detail_code );
#endif
}


static INT32 pmi_cli_build_record_comp_list_info(
        PM_RECORDING_ES_TYPE_T  e_es_type,
        BOOL                    fg_is_empty,
        UINT8                   ui1_comp_num,
        VOID**                  ppv_rec_info )
{
    PM_RECORDING_INFO_T*    pt_rec_info;
    PM_REC_ES_REQUEST_T*    pt_comp_list;
    UINT8                   i;

    pt_rec_info = (PM_RECORDING_INFO_T*)x_mem_alloc( sizeof(PM_RECORDING_INFO_T) );
    if (NULL == pt_rec_info)
    {
        x_dbg_stmt( "request memory FAIL(line:%d)\n", __LINE__ );
        return -1;
    }

    pt_rec_info->ui4_list_size     = ui1_comp_num;
    pt_rec_info->pt_component_list = (PM_REC_ES_REQUEST_T*)x_mem_alloc( ui1_comp_num * sizeof(PM_REC_ES_REQUEST_T) );
    if (NULL == pt_rec_info->pt_component_list)
    {
        x_dbg_stmt( "request memory FAIL(line:%d)\n", __LINE__ );
        return -1;
    }

    pt_comp_list = pt_rec_info->pt_component_list;
    
    switch ( e_es_type )
    {
        case PM_RECORDING_ES_WI_SETTING:
            pt_comp_list[0].e_es_info_type = e_es_type;
            pt_comp_list[0].u_es_info.t_stream_setting.t_pid  = 0x12;
            pt_comp_list[0].t_sink_comp_info.e_type  = DRVT_DEMUX_REC;
            pt_comp_list[0].t_sink_comp_info.ui2_id  = 1;
            pt_comp_list[0].t_filter_nfy_info.pf_nfy = pm_cli_filter_nfy_fct;
            pt_comp_list[0].t_filter_nfy_info.pv_tag = (VOID*)(UINT32)0;

            for (i = 0; i < ui1_comp_num; i++)
            {
                pt_comp_list[i] = pt_comp_list[0];
            }

            if (2 == ui1_comp_num)
            {
                pt_comp_list[1].u_es_info.t_stream_comp_id.e_type = ST_AUDIO;
                pt_comp_list[0].u_es_info.t_stream_setting.t_pid  = 0x34;
                pt_comp_list[1].t_filter_nfy_info.pv_tag = (VOID*)(UINT32)1;
            }
            break;
            
        case PM_RECORDING_ES_WO_SETTING:
            pt_comp_list[0].e_es_info_type = e_es_type;
            pt_comp_list[0].u_es_info.t_stream_comp_id.e_type = ST_VIDEO;
            pt_comp_list[0].u_es_info.t_stream_comp_id.pv_stream_tag = 0;
            pt_comp_list[0].t_sink_comp_info.e_type  = DRVT_DEMUX_REC;
            pt_comp_list[0].t_sink_comp_info.ui2_id  = 1;
            pt_comp_list[0].t_filter_nfy_info.pf_nfy = pm_cli_filter_nfy_fct;
            pt_comp_list[0].t_filter_nfy_info.pv_tag = (VOID*)(UINT32)0;

            for (i = 0; i < ui1_comp_num; i++)
            {
                pt_comp_list[i] = pt_comp_list[0];
            }

            if (2 == ui1_comp_num)
            {
                pt_comp_list[1].u_es_info.t_stream_comp_id.e_type = ST_AUDIO;
                pt_comp_list[1].t_filter_nfy_info.pv_tag = (VOID*)(UINT32)1;
            }
            
            break;
        case PM_RECORDING_ES_FILTER_ALLOCATED:
            break;
        default:
            break;
    }
    
    *ppv_rec_info = pt_rec_info;

    if (TRUE == fg_is_empty)
    {
        x_memset( pt_rec_info->pt_component_list, 0, sizeof(ui1_comp_num * sizeof(PM_REC_ES_REQUEST_T)) );
    }
    
    return 0;
}


#if (PM_CLI_DETAIL_LOG) 
static VOID  pmi_cli_print_comp_info( 
        PM_REC_ES_REQUEST_T*    pt_comp_list )
{
    UINT32                  ui4_comp_idx;
    
    ui4_comp_idx = (UINT32)pt_comp_list[0].t_filter_nfy_info.pv_tag;
    x_dbg_stmt( "the (%d) component return value:\n", ui4_comp_idx );
    x_dbg_stmt( "   comp handle is (0x%x)\n", pt_comp_list[0].h_component );
    switch (pt_comp_list[0].t_filter_nfy_info.e_cond)
    {
        case PM_COND_CLOSED:
            x_dbg_stmt( "   filter condition is %s\n", "PM_COND_CLOSED" );
            break;
        case PM_COND_OPENING:
            x_dbg_stmt( "   filter condition is %s\n", "PM_COND_OPENING" );
            break;
        case PM_COND_CLOSING:
            x_dbg_stmt( "   filter condition is %s\n", "PM_COND_CLOSING" );
            break;
        case PM_COND_PLAYING:
            x_dbg_stmt( "   filter condition is %s\n", "PM_COND_PLAYING" );
            break;
        case PM_COND_PLAYED:
            x_dbg_stmt( "   filter condition is %s\n", "PM_COND_PLAYED" );
            break;
        case PM_COND_STOPPING:
            x_dbg_stmt( "   filter condition is %s\n", "PM_COND_STOPPING" );
            break;
        case PM_COND_STOPPED:
            x_dbg_stmt( "   filter condition is %s\n", "PM_COND_STOPPED" );
            break;
        default :
            break;
    }
}
#endif


static INT32 pmi_cli_free_record_comp_list_info(
        VOID*                   pv_rec_info,
        UINT8                   ui1_comp_num )
{
    PM_RECORDING_INFO_T*    pt_rec_info  = (PM_RECORDING_INFO_T*)pv_rec_info;
#if (PM_CLI_DETAIL_LOG)    
    PM_REC_ES_REQUEST_T*    pt_comp_list = pt_rec_info->pt_component_list;
    UINT8                   i;

    for (i = 0; i < ui1_comp_num; i++)
    {
        pmi_cli_print_comp_info( pt_comp_list+i );
    }
#endif

    x_mem_free( pt_rec_info->pt_component_list );
    x_mem_free( pt_rec_info );
    
    return 0;
}
#endif  /* (PM_ENABLE_RECORD) */


static INT32 pmi_cli_open_svc       ( INT32 i4_argc, const CHAR** pps_argv)
{
    INT32                   i4_ret;
    VOID*                   pv_pm_info = NULL;    
    UINT8                   ui1_es_info_type;
    UINT8                   ui1_process_type;    
    HANDLE_T                h_connection;
    PM_PROCESSING_TYPE_T    e_prc_type; 
    BOOL                    fg_is_ok;
    const   UINT8           ui1_comp_num = 2;

    if (FALSE == g_fg_test_mode)
    {
        if(i4_argc != 4)//_ch_hdlr_get_cm_cmds
        {
            x_dbg_stmt("command [h_conn] [prc_type] [es_type]\n");
            x_dbg_stmt("    prc_type: 0:PM_PRESENT,1:PM_DETECTION,2:PM_RECORDING,3:PM_MULTIMEDIA,\n");
            x_dbg_stmt("    es_type : 0:WI_SETTING,1:WO_SETTING,2:FILTER_ALLOCATED, 3:EMPTY, 4:ZERO\n");
            return 0;
        }

        h_connection     = (UINT32)x_strtoull(pps_argv[1], NULL, 10);
        ui1_process_type = (UINT32)x_strtoull(pps_argv[2], NULL, 10);
        ui1_es_info_type = (UINT32)x_strtoull(pps_argv[3], NULL, 10);
    }
    
    else
    {
        h_connection     = g_h_pm_cli_conn;
#if (PM_CLI_DETAIL_LOG)
        x_dbg_stmt( "***************************(%s)(%d)\n", __func__, h_connection );
#endif
        ui1_process_type = 2;
        ui1_es_info_type = 3;
    }

    fg_is_ok = FALSE;

    switch( ui1_process_type )
    {
        case 0:
            e_prc_type = PM_PRESENT;
            x_dbg_stmt( "ui1_es_info_type(%d), ui1_comp_num(%d)\n", ui1_es_info_type, ui1_comp_num );
            break;
        case 1:
            e_prc_type = PM_DETECTION;
            break;
        #if (PM_ENABLE_RECORD)
        case 2:
            e_prc_type = PM_RECORDING;
            switch( ui1_es_info_type )
            {
                case 0:
                    if (0 == pmi_cli_build_record_comp_list_info(
                                PM_RECORDING_ES_WI_SETTING,
                                FALSE,
                                ui1_comp_num,
                                &pv_pm_info ))
                    {
                        fg_is_ok = TRUE;
                    }
                    break;
                    
                case 1:
                    if (0 == pmi_cli_build_record_comp_list_info(
                                PM_RECORDING_ES_WO_SETTING,
                                FALSE,
                                ui1_comp_num,
                                &pv_pm_info ))
                    {
                        fg_is_ok = TRUE;
                    }
                    break;
                    
                case 2:
                    if (0 == pmi_cli_build_record_comp_list_info(
                                PM_RECORDING_ES_FILTER_ALLOCATED,
                                FALSE,
                                ui1_comp_num,
                                &pv_pm_info ))
                    {
                        fg_is_ok = TRUE;
                    }
                    break;

                case 3:
                    pv_pm_info = NULL;
                    fg_is_ok = TRUE;
                    break;
                case 4:
                    if (0 == pmi_cli_build_record_comp_list_info(
                            PM_RECORDING_ES_FILTER_ALLOCATED,
                            TRUE,
                            ui1_comp_num,
                            &pv_pm_info ))
                    {
                        fg_is_ok = TRUE;
                    }
                    break;
                    
                default:
                    break;
            }
            break;
        #endif
        case 3:
            e_prc_type = PM_MULTIMEDIA;
            break;
        default:
            break;
    }

    if (!fg_is_ok)
    {
        x_dbg_stmt( "command [h_conn] [prc_type] [es_type]\n");
        x_dbg_stmt( "    prc_type: 0:PM_PRESENT,1:PM_DETECTION,2:PM_RECORDING,3:PM_MULTIMEDIA,\n");
        x_dbg_stmt( "    es_type : 0:WI_SETTING,1:WO_SETTING,  2:FILTER_ALLOCATED\n");
        x_dbg_stmt( "not support PM_PRESENT, PM_DETECTION & PM_MULTIMEDIA, now\n" );
        return 0;
    }

    i4_ret = pm_open_service(
        h_connection,   
        e_prc_type,                   
        pv_pm_info,                    
        &g_h_pm_cli_svc );
    if (PMR_OK == i4_ret)
    {
#if (PM_CLI_DETAIL_LOG)
        x_dbg_stmt( "PM service handle is (0x%x)\n", g_h_pm_cli_svc );
#endif
    }
    else
    {
        x_dbg_stmt( "PM open service FAIL(0x%x)!\n", i4_ret );
    }

    switch( e_prc_type )
    {
        case PM_PRESENT:
            break;
        case PM_DETECTION:
            break;
        #if (PM_ENABLE_RECORD)
        case PM_RECORDING:
            if (NULL != pv_pm_info)
            {
                pmi_cli_free_record_comp_list_info( pv_pm_info, ui1_comp_num );
            }
            break;
        #endif
        case PM_MULTIMEDIA:
            break;
        default :
            break;
    }
    
    return 0;
}


static INT32 pmi_cli_play_svc       ( INT32 i4_argc, const CHAR** pps_argv)
{
    HANDLE_T    h_service;
    INT32       i4_speed;
    INT32       i4_ret;

    if (FALSE == g_fg_test_mode)    
    {
        if(i4_argc != 3)
        {
            x_dbg_stmt("command [h_service] [i4_speed]\n");
            return 0;
        }

        h_service = (UINT32)x_strtoull(pps_argv[1], NULL, 10);
        i4_speed  = (INT32)x_strtoull (pps_argv[2], NULL, 10);
    }
    else
    {
        h_service = g_h_pm_cli_svc;
        i4_speed  = 1;
    }

    i4_ret = pm_play_service( h_service, i4_speed);
    if (PMR_OK != i4_ret)
    {
        x_dbg_stmt( "%s FAIL(ReturnValue == 0x%x)\n", __func__, i4_ret );
    }
    else
    {
        x_dbg_stmt( "%s OK\n", __func__ );
    }
    
    return 0;
}


static INT32 pmi_cli_stop_svc       ( INT32 i4_argc, const CHAR** pps_argv)
{
    HANDLE_T    h_service;
    INT32       i4_ret;

    if (FALSE == g_fg_test_mode)
    {
        if(i4_argc != 2)
        {
            x_dbg_stmt("command [h_service]\n");
            return 0;
        }

        h_service = (UINT32)x_strtoull(pps_argv[1], NULL, 10);
    }
    else    
    {
        h_service = g_h_pm_cli_svc;
    }

    i4_ret = pm_stop_service( h_service );
    if (PMR_OK != i4_ret)
    {
        x_dbg_stmt( "%s FAIL(ReturnValue == 0x%x)\n", __func__, i4_ret );
    }
    else
    {
        x_dbg_stmt( "%s OK\n", __func__ );
    }
    
    return 0;
}


static INT32 pmi_cli_close_svc      ( INT32 i4_argc, const CHAR** pps_argv)
{
    HANDLE_T    h_service;
    INT32       i4_ret;

    if (FALSE == g_fg_test_mode)
    {
        if(i4_argc != 2)
        {
            x_dbg_stmt("command [h_service]\n");
            return 0;
        }

        h_service = (UINT32)x_strtoull(pps_argv[1], NULL, 10);
    }
    else
    {
        h_service = g_h_pm_cli_svc;
    }

    i4_ret = pm_close_service( h_service );
    if (PMR_OK != i4_ret)
    {
        x_dbg_stmt( "%s FAIL(ReturnValue == 0x%x)\n", __func__, i4_ret );
    }
    else
    {
        x_dbg_stmt( "%s OK\n", __func__ );
    }
    
    return 0;
}


static INT32 pmi_cli_get_svc_cond   ( INT32 i4_argc, const CHAR** pps_argv)
{
    HANDLE_T            h_service;
    PM_SERVICE_COND_T   e_cond;
    INT32               i4_speed;
    INT32               i4_ret;

    if (FALSE == g_fg_test_mode)
    {
        if(i4_argc != 2)
        {
            x_dbg_stmt("command [h_service]\n");
            return 0;
        }

        h_service = (UINT32)x_strtoull(pps_argv[1], NULL, 10);
    }
    else
    {
        h_service = g_h_pm_cli_svc;
    }

    i4_ret = pm_get_service_cond( h_service, &e_cond, &i4_speed );
    if (PMR_OK != i4_ret)
    {
        x_dbg_stmt( "%s FAIL(ReturnValue == 0x%x)\n", __func__, i4_ret );
    }
    else
    {
        x_dbg_stmt( "%s OK\n", __func__ );
        
        switch (e_cond)
        {
            case PM_COND_CLOSE:
                x_dbg_stmt( "   condition is PM_COND_CLOSE\n" );
                break;
            case PM_COND_OPEN:
                x_dbg_stmt( "   condition is PM_COND_OPEN\n" );
                break;
            case PM_COND_PLAY:
                x_dbg_stmt( "   condition is PM_COND_PLAY\n" );
                break;
            case PM_COND_STOP:
                x_dbg_stmt( "   condition is PM_COND_STOP\n" );
                break;
            default:
                break;
        }

        x_dbg_stmt( "   i4_speed is (%d)\n", i4_speed );
    }
    
    return 0;
}


static INT32 pmi_cli_add_comp       ( INT32 i4_argc, const CHAR** pps_argv)
{
#if (PM_ENABLE_RECORD)
    HANDLE_T                h_service;
    INT32                   i4_ret;
    BOOL                    fg_is_ok;
    UINT8                   ui1_comp_num = 1;
    VOID*                   pv_rec_info;
    PM_RECORDING_INFO_T*    pt_rec_info;
    PM_RECORDING_INFO_T     t_rec_info;
    UINT8                   ui1_es_info_type;

    if (FALSE == g_fg_test_mode)
    {
        if(i4_argc != 3)
        {
            x_dbg_stmt("command [h_service] [es_type]\n");
            x_dbg_stmt("    es_type : 0:WI_SETTING,1:WO_SETTING,2:FILTER_ALLOCATED, 3:UNKNOWN\n");
            return 0;
        }

        h_service        = (UINT32)x_strtoull(pps_argv[1], NULL, 10);
        ui1_es_info_type = (UINT32)x_strtoull(pps_argv[2], NULL, 10);
    }
    else
    {
        h_service        = g_h_pm_cli_svc;
        ui1_es_info_type = 0;
    }

    fg_is_ok = FALSE;

    switch( ui1_es_info_type )
        {
            case 0:
                x_dbg_stmt( "add WI_SETTING type component\n" );
                if (0 == pmi_cli_build_record_comp_list_info(
                            PM_RECORDING_ES_WI_SETTING,
                            FALSE,
                            ui1_comp_num,
                            &pv_rec_info ))
                {
                    fg_is_ok = TRUE;
                }
                break;
                
            case 1:
                if (0 == pmi_cli_build_record_comp_list_info(
                            PM_RECORDING_ES_WO_SETTING,
                            FALSE,
                            ui1_comp_num,
                            &pv_rec_info ))
                {
                    fg_is_ok = TRUE;
                }
                break;
                
            case 2:
                if (0 == pmi_cli_build_record_comp_list_info(
                            PM_RECORDING_ES_FILTER_ALLOCATED,
                            FALSE,
                            ui1_comp_num,
                            &pv_rec_info ))
                {
                    fg_is_ok = TRUE;
                }
                break;
            case 3:
                fg_is_ok = TRUE;
                t_rec_info.pt_component_list = NULL;
                pv_rec_info = &t_rec_info;
                break;
            case 4:
                if (0 == pmi_cli_build_record_comp_list_info(
                            PM_RECORDING_ES_FILTER_ALLOCATED,
                            TRUE,
                            ui1_comp_num,
                            &pv_rec_info ))
                {
                    fg_is_ok = TRUE;
                }
                break;

            default:
                break;
        }

    if (!fg_is_ok)
    {
        x_dbg_stmt("command [h_service] [es_type]\n");
        x_dbg_stmt("    es_type : 0:WI_SETTING,1:WO_SETTING,2:FILTER_ALLOCATED\n");
        return 0;
    }

    pt_rec_info = (PM_RECORDING_INFO_T*)pv_rec_info;

    i4_ret = pm_add_component( h_service, pt_rec_info->pt_component_list );
    if (PMR_OK != i4_ret)
    {
        x_dbg_stmt( "%s FAIL(ReturnValue == 0x%x)\n", __func__, i4_ret );
    }
    else
    {
#if (PM_CLI_DETAIL_LOG)
        x_dbg_stmt( 
            "%s OK(component handle == 0x%x)\n", 
            __func__,
            pt_rec_info->pt_component_list->h_component );
#endif

        g_h_pm_cli_comp = pt_rec_info->pt_component_list->h_component;
        
        pmi_cli_free_record_comp_list_info( pt_rec_info, ui1_comp_num );
    }
#endif  /* (PM_ENABLE_RECORD) */
    
    return 0;
}


static INT32 pmi_cli_play_comp      ( INT32 i4_argc, const CHAR** pps_argv)
{
    HANDLE_T    h_comp;
    INT32       i4_ret;

    if (FALSE == g_fg_test_mode)
    {
        if(i4_argc != 2)
        {
            x_dbg_stmt("command [h_component]\n");
            return 0;
        }

        h_comp = (UINT32)x_strtoull(pps_argv[1], NULL, 10);
    }
    else
    {
        h_comp = g_h_pm_cli_comp;
    }

    i4_ret = pm_play_component( h_comp );
    if (PMR_OK != i4_ret)
    {
        x_dbg_stmt( "%s FAIL(ReturnValue == 0x%x)\n", __func__, i4_ret );
    }
    else
    {
        x_dbg_stmt( "%s OK\n", __func__ );
    }
    
    return 0;
}

static INT32 pmi_cli_stop_comp      ( INT32 i4_argc, const CHAR** pps_argv)
{
    HANDLE_T    h_comp;
    INT32       i4_ret;

    if (FALSE == g_fg_test_mode)
    {
        if(i4_argc != 2)
        {
            x_dbg_stmt("command [h_component]\n");
            return 0;
        }

        h_comp = (UINT32)x_strtoull(pps_argv[1], NULL, 10);
    }
    else
    {
        h_comp = g_h_pm_cli_comp;
    }

    i4_ret = pm_stop_component( h_comp );
    if (PMR_OK != i4_ret)
    {
        x_dbg_stmt( "%s FAIL(ReturnValue == 0x%x)\n", __func__, i4_ret );
    }
    else
    {
        x_dbg_stmt( "%s OK\n", __func__ );
    }
    
    return 0;
}


static INT32 pmi_cli_del_comp       ( INT32 i4_argc, const CHAR** pps_argv)
{
    HANDLE_T    h_comp;
    INT32       i4_ret;

    if (FALSE == g_fg_test_mode)
    {
        if(i4_argc != 2)
        {
            x_dbg_stmt("command [h_component]\n");
            return 0;
        }

        h_comp = (UINT32)x_strtoull(pps_argv[1], NULL, 10);
    }
    else
    {
        h_comp = g_h_pm_cli_comp;
    }

    i4_ret = pm_del_component( h_comp );
    if (PMR_OK != i4_ret)
    {
        x_dbg_stmt( "%s FAIL(ReturnValue == 0x%x)\n", __func__, i4_ret );
    }
    else
    {
#if (PM_CLI_DETAIL_LOG)
        x_dbg_stmt( "%s OK\n", __func__ );
#endif
    }
    
    return 0;
}


static INT32 pmi_cli_get_comp_cond  ( INT32 i4_argc, const CHAR** pps_argv)
{
    HANDLE_T    h_comp;
    INT32       i4_ret;
    PM_COND_T   e_cond;
    UINT32      ui4_data;

    if (FALSE == g_fg_test_mode)
    {
        if(i4_argc != 2)
        {
            x_dbg_stmt("command [h_component]\n");
            return 0;
        }

        h_comp = (UINT32)x_strtoull(pps_argv[1], NULL, 10);
    }
    else
    {
        h_comp = g_h_pm_cli_comp;
    }

    i4_ret = pm_get_component_cond( h_comp, &e_cond, &ui4_data );
    if (PMR_OK != i4_ret)
    {
        x_dbg_stmt( "%s FAIL(ReturnValue == 0x%x)\n", __func__, i4_ret );
    }
    else
    {
        x_dbg_stmt( "%s OK\n", __func__ );

        switch (e_cond)
        {
            case PM_COND_CLOSED:
                x_dbg_stmt( "   condition is PM_COND_CLOSED\n" );
                break;
            case PM_COND_OPENING:
                x_dbg_stmt( "   condition is PM_COND_OPENING\n" );
                break;
            case PM_COND_CLOSING:
                x_dbg_stmt( "   condition is PM_COND_CLOSING\n" );
                break;
            case PM_COND_PLAYING:
                x_dbg_stmt( "   condition is PM_COND_PLAYING\n" );
                break;
            case PM_COND_PLAYED:
                x_dbg_stmt( "   condition is PM_COND_PLAYED\n" );
                break;
            case PM_COND_STOPPING:
                x_dbg_stmt( "   condition is PM_COND_STOPPING\n" );
                break;
            case PM_COND_STOPPED:
                x_dbg_stmt( "   condition is PM_COND_STOPPED\n" );
                break;
            default:
                x_dbg_stmt( "   condition is unknown\n" );
                break;
        }

        x_dbg_stmt( "   ui4_data is (0x%x)\n", ui4_data );
    }
    
    return 0;
}


static INT32 pmi_cli_get_comp_info  ( INT32 i4_argc, const CHAR** pps_argv)
{
    HANDLE_T    h_comp;
    INT32       i4_ret;
    HANDLE_T    h_real_obj;

    if (FALSE == g_fg_test_mode)
    {
        if(i4_argc != 2)
        {
            x_dbg_stmt("command [h_component]\n");
            return 0;
        }

        h_comp = (UINT32)x_strtoull(pps_argv[1], NULL, 10);
    }
    else
    {
        h_comp = g_h_pm_cli_comp;
    }

    i4_ret = pm_get_component_info( h_comp, &h_real_obj );
    if (PMR_OK != i4_ret)
    {
        x_dbg_stmt( "%s FAIL(ReturnValue == 0x%x)\n", __func__, i4_ret );
    }
    else
    {
        x_dbg_stmt( "%s OK\n", __func__ );
        x_dbg_stmt( "   h_real_obj is (0x%x)\n", h_real_obj );
    }
    
    return 0;
}


typedef union   _PM_CMD_INFO_T
{
    MM_RANGE_INFO_T         t_add_range;
    UINT32                  ui4_del_range;
    MM_REC_BUF_RANGE_INFO_T t_rec_range_info;
    MM_SPEED_TYPE_T         t_mm_speed_type;
    MM_RANGE_POS_INFO_T     t_mm_pos_info;
    BOOL                    fg_block_psi;
}   PM_CMD_INFO_T;

typedef union   _PM_CMD_GET_INFO_T
{
    UINT32                  ui4_block_psi;
}   PM_GET_INFO_T;

static INT32 pmi_cli_set_cmd        ( INT32 i4_argc, const CHAR** pps_argv)
{
    HANDLE_T        h_comp_svc;/* component or service handle */
    UINT32          ui4_comd_idx;
    PM_SET_TYPE_T   t_cmd = PM_SET_UNKNOWN;
    INT32           i4_ret;
    PM_CMD_INFO_T   t_cmd_info;
    VOID*           pt_info = NULL;

    if(i4_argc != 3)
    {
        x_dbg_stmt("command [h_component/h_service] [cmd_idx]\n");
        x_dbg_stmt("    cmd_idx: (1)PM_SET_ADD_RANGE\n");
        x_dbg_stmt("    cmd_idx: (2)PM_SET_DEL_RANGE\n");
        x_dbg_stmt("    cmd_idx: (3)PM_SET_UPDATE_VALID_RANGE\n");
        x_dbg_stmt("    cmd_idx: (4)PM_SET_SPEED\n");
        x_dbg_stmt("    cmd_idx: (5)PM_SET_SEEK\n");
        x_dbg_stmt("    cmd_idx: (6)PM_SET_BLOCK_PSI\n"); 
        x_dbg_stmt("    cmd_idx: (7)PM_SET_LAST\n");
        return 0;
    }

    h_comp_svc   = (UINT32)x_strtoull(pps_argv[1], NULL, 10);
    ui4_comd_idx = (UINT32)x_strtoull(pps_argv[2], NULL, 10);

    switch (ui4_comd_idx)
    {
        case 1:
            t_cmd   = PM_SET_ADD_RANGE;
            pt_info = &t_cmd_info.t_add_range;
            break;
        case 2:
            t_cmd   = PM_SET_DEL_RANGE;
            pt_info = &t_cmd_info.ui4_del_range;
            break;
        case 3:
#if (PM_ENABLE_TIME_SHIFT)
            t_cmd   = PM_SET_REC_BUF_RANGE;
            pt_info = &t_cmd_info.t_rec_range_info;
            t_cmd_info.t_rec_range_info.t_safe.ui4_start  = 0x12;
            t_cmd_info.t_rec_range_info.t_safe.ui4_end    = 0x34;
            t_cmd_info.t_rec_range_info.t_valid.ui4_start = 0x56;
            t_cmd_info.t_rec_range_info.t_valid.ui4_end   = 0x78;
#endif
            break;
        case 4:
            t_cmd   = PM_SET_SPEED;
            pt_info = &t_cmd_info.t_mm_speed_type;
            break;
        case 5:
            t_cmd   = PM_SET_SEEK;
            pt_info = &t_cmd_info.t_mm_pos_info;
            break;
        case 6:
            t_cmd   = PM_SET_BLOCK_PSI;
            pt_info = &t_cmd_info.fg_block_psi;
            break;
        case 7:
            t_cmd   = PM_SET_LAST;
            pt_info = NULL;
            break;
        default :
            break;
    }

    i4_ret = pm_set( h_comp_svc, t_cmd, pt_info );
    if (PMR_OK != i4_ret)
    {
        x_dbg_stmt( "%s FAIL(ReturnValue == 0x%x)\n", __func__, i4_ret );
    }
    else
    {
        x_dbg_stmt( "%s OK\n", __func__ );
    }
    
    return 0;
}

static INT32 pmi_cli_get_cmd        ( INT32 i4_argc, const CHAR** pps_argv)
{
    HANDLE_T        h_comp_svc;/* component or service handle */
    UINT32          ui4_comd_idx;
    PM_GET_TYPE_T   t_cmd = PM_GET_UNKNOWN;
    INT32           i4_ret;
    PM_GET_INFO_T   t_get_info;
    VOID*           pt_info = NULL;
    SIZE_T          t_get_info_length;

    if(i4_argc != 3)
    {
        x_dbg_stmt("command [h_component/h_service] [cmd_idx]\n");
        x_dbg_stmt("    cmd_idx: (1)PM_GET_BLOCK_PSI\n");
        return 0;
    }

    h_comp_svc   = (UINT32)x_strtoull(pps_argv[1], NULL, 10);
    ui4_comd_idx = (UINT32)x_strtoull(pps_argv[2], NULL, 10);

    switch (ui4_comd_idx)
    {
        case 1:
            t_cmd   = PM_GET_BLOCK_PSI;
            pt_info = &t_get_info.ui4_block_psi;
            t_get_info_length = sizeof(UINT32);
            break;
        
        default :
            break;
    }

    i4_ret = pm_get ( h_comp_svc, t_cmd, pt_info, &t_get_info_length );
    if (PMR_OK != i4_ret)
    {
        x_dbg_stmt( "%s FAIL(ReturnValue == 0x%x)\n", __func__, i4_ret );
    }
    else
    {
        x_dbg_stmt( "%s OK\n", __func__ );
    }
    
    return 0;
}


#if (PM_CLI_ENABLE_UT)
static INT32 pmi_cli_abn_loop_add_comp      ( INT32 i4_argc, const CHAR** pps_argv)
{
    UINT32      u4_comp_num_insvc = 100;
    UINT32      i;
    
    CHAR        ac_h_connection[20];
    CHAR        ac_process_type[10];
    CHAR        ac_es_info_type[10];
    INT32       i4_para_argc = 4;
    CHAR*       aps_para_argv[4];

    HANDLE_T    ah_comp[100];
    
    x_sprintf( ac_h_connection, "%d\0", g_h_pm_cli_conn );
    x_sprintf( ac_process_type, "%d\0", 2 );
    x_sprintf( ac_es_info_type, "%d\0", 3 );

    aps_para_argv[1] = ac_h_connection;
    aps_para_argv[2] = ac_process_type;
    aps_para_argv[3] = ac_es_info_type;

    pmi_cli_open_svc( i4_para_argc, (const CHAR**)(const CHAR**)aps_para_argv );
    
    {
        CHAR*   pc_h_svc = ac_h_connection;

        x_sprintf( pc_h_svc,        "%d\0", g_h_pm_cli_svc );
        x_sprintf( ac_es_info_type, "%d\0", 0 );

        aps_para_argv[1] = pc_h_svc;
        aps_para_argv[2] = ac_es_info_type;

        for (i = 0; i < u4_comp_num_insvc; i++)
        {
            pmi_cli_add_comp( 3, (const CHAR**)(const CHAR**)aps_para_argv );
            ah_comp[i] = g_h_pm_cli_comp;
        }
    }

    /* free resource */
    {
        for (i = 0; i < u4_comp_num_insvc; i++)
        {
            pm_del_component( ah_comp[i] );
        }

        pm_close_service( g_h_pm_cli_svc );
    }
    
    return 0;
}


static INT32 pmi_cli_abn_stop_svc_after_open( INT32 i4_argc, const CHAR** pps_argv)
{
    CHAR    ac_h_connection[20];
    CHAR    ac_process_type[10];
    CHAR    ac_es_info_type[10];
    INT32   i4_para_argc = 4;
    CHAR*   aps_para_argv[4];
    
    x_sprintf( ac_h_connection, "%d\0", g_h_pm_cli_conn );
    x_sprintf( ac_process_type, "%d\0", 2 );
    x_sprintf( ac_es_info_type, "%d\0", 0 );

    aps_para_argv[1] = ac_h_connection;
    aps_para_argv[2] = ac_process_type;
    aps_para_argv[3] = ac_es_info_type;

    pmi_cli_open_svc( i4_para_argc, (const CHAR**)(const CHAR**)aps_para_argv );

    {
        CHAR*   pc_h_svc = ac_h_connection;

        x_sprintf( pc_h_svc, "%d\0", g_h_pm_cli_svc );
        aps_para_argv[1] = pc_h_svc;
        
        pmi_cli_stop_svc( 2, (const CHAR**)(const CHAR**)aps_para_argv );
    }
    
    return 0;
}


static INT32 pmi_cli_abn_play_svc_no_comp   ( INT32 i4_argc, const CHAR** pps_argv)
{
    CHAR    ac_h_connection[20];
    CHAR    ac_process_type[10];
    CHAR    ac_es_info_type[10];
    INT32   i4_para_argc = 4;
    CHAR*   aps_para_argv[4];
    
    x_sprintf( ac_h_connection, "%d\0", g_h_pm_cli_conn );
    x_sprintf( ac_process_type, "%d\0", 2 );
    x_sprintf( ac_es_info_type, "%d\0", 3 );

    aps_para_argv[1] = ac_h_connection;
    aps_para_argv[2] = ac_process_type;
    aps_para_argv[3] = ac_es_info_type;

    pmi_cli_open_svc( i4_para_argc, (const CHAR**)(const CHAR**)aps_para_argv );

    {
        CHAR*   pc_h_svc    = ac_h_connection;
        CHAR*   pc_i4_speed = ac_process_type;

        x_sprintf( pc_h_svc,    "%d\0", g_h_pm_cli_svc );
        x_sprintf( pc_i4_speed, "%d\0", 1 );
        aps_para_argv[1] = pc_h_svc;
        aps_para_argv[2] = pc_i4_speed;
        
        pmi_cli_play_svc( 3, (const CHAR**)(const CHAR**)aps_para_argv );
    }

    /* free resource */
    {
        pm_close_service( g_h_pm_cli_svc );
    }

    
    return 0;
}


static INT32 pmi_cli_abn_play_deleted_comp  ( INT32 i4_argc, const CHAR** pps_argv)
{
    CHAR    ac_h_connection[20];
    CHAR    ac_process_type[10];
    CHAR    ac_es_info_type[10];
    INT32   i4_para_argc = 4;
    CHAR*   aps_para_argv[4];
    
    x_sprintf( ac_h_connection, "%d\0", g_h_pm_cli_conn );
    x_sprintf( ac_process_type, "%d\0", 2 );
    x_sprintf( ac_es_info_type, "%d\0", 3 );

    aps_para_argv[1] = ac_h_connection;
    aps_para_argv[2] = ac_process_type;
    aps_para_argv[3] = ac_es_info_type;

    pmi_cli_open_svc( i4_para_argc, (const CHAR**)aps_para_argv );

    {
        CHAR*   pc_h_svc = ac_h_connection;

        x_sprintf( pc_h_svc,        "%d\0", g_h_pm_cli_svc );
        x_sprintf( ac_es_info_type, "%d\0", 0 );

        aps_para_argv[1] = pc_h_svc;
        aps_para_argv[2] = ac_es_info_type;

        pmi_cli_add_comp( 3, (const CHAR**)aps_para_argv );

        {
            CHAR*   pc_h_comp = pc_h_svc;

            x_sprintf( pc_h_comp, "%d\0", g_h_pm_cli_comp );
            aps_para_argv[1] = pc_h_comp;

            pmi_cli_del_comp ( 2, (const CHAR**)aps_para_argv );
            pmi_cli_play_comp( 2, (const CHAR**)aps_para_argv );
        }
    }

    /* free resource */
    {
        pm_close_service( g_h_pm_cli_svc );
    }
    
    return 0;
}


static INT32 pmi_cli_abn_play_svc_no_opened ( INT32 i4_argc, const CHAR** pps_argv)
{
    CHAR*   aps_para_argv[4];
    CHAR    ac_h_connection[20];
    CHAR    ac_process_type[10];
    CHAR*   pc_h_svc    = ac_h_connection;
    CHAR*   pc_i4_speed = ac_process_type;

    x_sprintf( pc_h_svc,    "%d\0", 0x1234 );
    x_sprintf( pc_i4_speed, "%d\0", 1 );
    aps_para_argv[1] = pc_h_svc;
    aps_para_argv[2] = pc_i4_speed;
    
    pmi_cli_play_svc( 3, (const CHAR**)(const CHAR**)aps_para_argv );
    
    return 0;
}


static INT32 pmi_cli_abn_play_svc_deleted   ( INT32 i4_argc, const CHAR** pps_argv)
{
    CHAR    ac_h_connection[20];
    CHAR    ac_process_type[10];
    CHAR    ac_es_info_type[10];
    INT32   i4_para_argc = 4;
    CHAR*   aps_para_argv[4];
    
    x_sprintf( ac_h_connection, "%d\0", g_h_pm_cli_conn );
    x_sprintf( ac_process_type, "%d\0", 2 );
    x_sprintf( ac_es_info_type, "%d\0", 0 );

    aps_para_argv[1] = ac_h_connection;
    aps_para_argv[2] = ac_process_type;
    aps_para_argv[3] = ac_es_info_type;

    pmi_cli_open_svc( i4_para_argc, (const CHAR**)(const CHAR**)aps_para_argv );

    {
        CHAR*   pc_h_svc    = ac_h_connection;
        CHAR*   pc_i4_speed = ac_process_type;

        x_sprintf( pc_h_svc,    "%d\0", g_h_pm_cli_svc );
        x_sprintf( pc_i4_speed, "%d\0", 1 );
        aps_para_argv[1] = pc_h_svc;
        aps_para_argv[2] = pc_i4_speed;

        pmi_cli_close_svc( 2, (const CHAR**)(const CHAR**)aps_para_argv );        
        pmi_cli_play_svc( 3, (const CHAR**)(const CHAR**)aps_para_argv );
    }
    
    return 0;
}


static INT32 pmi_cli_abn_play_comp_wrong_pid( INT32 i4_argc, const CHAR** pps_argv)
{
    CHAR    ac_h_connection[20];
    CHAR    ac_process_type[10];
    CHAR    ac_es_info_type[10];
    INT32   i4_para_argc = 4;
    CHAR*   aps_para_argv[4];
    
    x_sprintf( ac_h_connection, "%d\0", g_h_pm_cli_conn );
    x_sprintf( ac_process_type, "%d\0", 2 );
    x_sprintf( ac_es_info_type, "%d\0", 0 );

    aps_para_argv[1] = ac_h_connection;
    aps_para_argv[2] = ac_process_type;
    aps_para_argv[3] = ac_es_info_type;

    pmi_cli_open_svc( i4_para_argc, (const CHAR**)(const CHAR**)aps_para_argv );

    /* free resource */
    {
        pm_close_service( g_h_pm_cli_svc );
    }

    
    return 0;
}


static INT32 pmi_cli_abn_play_comp_no_cond  ( INT32 i4_argc, const CHAR** pps_argv)
{
    CHAR    ac_h_connection[20];
    CHAR    ac_process_type[10];
    CHAR    ac_es_info_type[10];
    INT32   i4_para_argc = 4;
    CHAR*   aps_para_argv[4];
    
    x_sprintf( ac_h_connection, "%d\0", g_h_pm_cli_conn );
    x_sprintf( ac_process_type, "%d\0", 2 );
    x_sprintf( ac_es_info_type, "%d\0", 3 );

    aps_para_argv[1] = ac_h_connection;
    aps_para_argv[2] = ac_process_type;
    aps_para_argv[3] = ac_es_info_type;

    pmi_cli_open_svc( i4_para_argc, (const CHAR**)(const CHAR**)aps_para_argv );

    {
        CHAR*   pc_h_svc = ac_h_connection;

        x_sprintf( pc_h_svc,        "%d\0", g_h_pm_cli_svc );
        x_sprintf( ac_es_info_type, "%d\0", 3 );

        aps_para_argv[1] = pc_h_svc;
        aps_para_argv[2] = ac_es_info_type;

        pmi_cli_add_comp( 3, (const CHAR**)(const CHAR**)aps_para_argv );

        {
            CHAR*   pc_h_comp = pc_h_svc;

            x_sprintf( pc_h_comp, "%d\0", g_h_pm_cli_svc );
            aps_para_argv[1] = pc_h_comp;
            
            pmi_cli_play_comp( 2, (const CHAR**)aps_para_argv );
            pmi_cli_del_comp ( 2, (const CHAR**)aps_para_argv );
        }
    }

    {
        CHAR*   pc_h_svc = ac_h_connection;

        x_sprintf( pc_h_svc,        "%d\0", g_h_pm_cli_svc );
        x_sprintf( ac_es_info_type, "%d\0", 4 );

        aps_para_argv[1] = pc_h_svc;
        aps_para_argv[2] = ac_es_info_type;

        pmi_cli_add_comp( 3, (const CHAR**)(const CHAR**)aps_para_argv );

        {
            CHAR*   pc_h_comp = pc_h_svc;

            x_sprintf( pc_h_comp, "%d\0", g_h_pm_cli_svc );
            aps_para_argv[1] = pc_h_comp;
            
            pmi_cli_play_comp( 2, (const CHAR**)aps_para_argv );
            pmi_cli_del_comp ( 2, (const CHAR**)aps_para_argv );
        }
    }

    /* free resource */
    {
        pm_close_service( g_h_pm_cli_svc );
    }

    
    return 0;
}


static INT32 pmi_cli_stress_test_unit( )
{
    CHAR    ac_h_connection[20];
    CHAR    ac_process_type[10];
    CHAR    ac_es_info_type[10];
    INT32   i4_para_argc = 4;
    CHAR*   aps_para_argv[4];
    
    x_sprintf( ac_h_connection, "%d\0", g_h_pm_cli_conn );
    x_sprintf( ac_process_type, "%d\0", 2 );
    x_sprintf( ac_es_info_type, "%d\0", 0 );

    aps_para_argv[1] = ac_h_connection;
    aps_para_argv[2] = ac_process_type;
    aps_para_argv[3] = ac_es_info_type;

    /* open a recording service node */
    /* the service have two components, after be created */
    pmi_cli_open_svc( i4_para_argc, (const CHAR**)(const CHAR**)aps_para_argv );
    x_thread_delay( 500 );

    /* add a component */
    {
        CHAR*   pc_h_svc = ac_h_connection;

        x_sprintf( pc_h_svc,        "%d\0", g_h_pm_cli_svc );
        x_sprintf( ac_es_info_type, "%d\0", 0 );

        aps_para_argv[1] = pc_h_svc;
        aps_para_argv[2] = ac_es_info_type;

        pmi_cli_add_comp( 3, (const CHAR**)aps_para_argv );
        x_thread_delay( 500 );
    }

    /* delete a component */
    {
        CHAR*   pc_h_comp = ac_h_connection;

        x_sprintf( pc_h_comp, "%d\0", g_h_pm_cli_comp );
        aps_para_argv[1] = pc_h_comp;

        pmi_cli_del_comp ( 2, (const CHAR**)aps_para_argv );
        x_thread_delay( 500 );
    }

    /* add a component again */
    {
        CHAR*   pc_h_svc = ac_h_connection;

        x_sprintf( pc_h_svc,        "%d\0", g_h_pm_cli_svc );
        x_sprintf( ac_es_info_type, "%d\0", 0 );

        aps_para_argv[1] = pc_h_svc;
        aps_para_argv[2] = ac_es_info_type;

        pmi_cli_add_comp( 3, (const CHAR**)aps_para_argv );
        x_thread_delay( 500 );
    }

    /* play */
    pm_play_service( g_h_pm_cli_svc, 1 );
    x_thread_delay( 500 );

    /* stop */
    pm_stop_service( g_h_pm_cli_svc );
    x_thread_delay( 500 );

    /* play */
    pm_play_service( g_h_pm_cli_svc, 1 );
    x_thread_delay( 500 );

    /* closed the service */
    pm_close_service( g_h_pm_cli_svc );
    x_thread_delay( 500 );
    
    return 0;
}

static INT32 pmi_cli_stress_test_for_res    ( INT32 i4_argc, const CHAR** pps_argv)
{
    HANDLE_T    h_cm_handle;
    UINT32      ui4_times;
    UINT32      i;
    
    if(i4_argc != 3)
    {
        x_dbg_stmt("command [h_cm_handle] [ui4_times]\n");
        return 0;
    }

    /* mw.handle.s */
    /* mw.os.sp */

    x_dbg_stmt( "%s:begin\n", __func__ );

    h_cm_handle = (UINT32)x_strtoull(pps_argv[1], NULL, 10);
    ui4_times   = (UINT32)x_strtoull(pps_argv[2], NULL, 10);

    g_h_pm_cli_conn = h_cm_handle;

    for (i = 0; i < ui4_times; i++)
    {
        x_thread_delay( 500 );
        pmi_cli_stress_test_unit();
    }

    x_dbg_stmt( "%s:end\n", __func__ );

    /* mw.handle.s */
    /* mw.os.sp */

    return 0;
}
#endif  /* (PM_CLI_ENABLE_UT) */
#endif  /* (PM_ENABLE_MULTIMEDIA) */


#endif /* CLI_LVL_ALL */





