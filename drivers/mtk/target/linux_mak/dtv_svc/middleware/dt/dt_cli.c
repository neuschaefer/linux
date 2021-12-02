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
 * $RCSfile: dt_cli.c,v $
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
#include "dt/u_dt.h"
#include "dt/x_dt.h"
#include "dt/_dt.h"
#include "dt/dt.h"
#include "dt/dt_dbg.h"
#include "os/inc/x_os.h"
#include "inc/u_sys_name.h"


#if defined(CLI_LVL_ALL) || defined(CLI_LVL_GUEST)

/* ----------------------------------------------------------------------
  Typedefs   
------------------------------------------------------------------------- */

/* DateTime module  CLI definitions. */
#ifdef CLI_LVL_ALL
static INT32 cli_get_dbg_level (INT32  i4_argc, const CHAR**  pps_argv);
static INT32 cli_set_dbg_level (INT32  i4_argc, const CHAR**  pps_argv);
#endif

#ifdef CLI_LVL_ALL
static INT32 dt_cli_set_sync_src (INT32  i4_argc, const CHAR**  pps_argv);
#endif

static INT32 dt_cli_get_curr_utc (INT32  i4_argc, const CHAR**  pps_argv);
static INT32 dt_cli_get_user_utc (INT32  i4_argc, const CHAR**  pps_argv);

static INT32 dt_cli_set_user_utc (INT32  i4_argc, const CHAR**  pps_argv);
static INT32 dt_cli_set_sys_cntry_code(INT32 i4_argc, const CHAR** pps_argv);
static INT32 dt_cli_get_sys_cntry_code(INT32 i4_argc, const CHAR** pps_argv);
static INT32 dt_cli_dump_cntry_code(INT32 i4_argc, const CHAR** pps_argv);
static INT32 dt_cli_tz_ctrl(INT32 i4_argc, const CHAR** pps_argv);

static INT32 dt_cli_dst_ctrl(INT32 i4_argc, const CHAR** pps_argv);
static INT32 dt_cli_set_dst(INT32 i4_argc, const CHAR** pps_argv);
static INT32 dt_cli_set_tz(INT32 i4_argc, const CHAR** pps_argv);

#ifdef CLI_LVL_ALL
static INT32 dt_cli_config(INT32 i4_argc, const CHAR** pps_argv);
static INT32 dt_cli_dump_dt_info(INT32 i4_argc, const CHAR** pps_argv);
#endif 

#ifdef DEBUG
static INT32 dt_cli_test(INT32 i4_argc, const CHAR** pps_argv);
#endif 

static CLI_EXEC_T at_cli_dbg_cmd[] =
{
    #ifdef CLI_LVL_ALL
    {"gdl",  NULL, cli_get_dbg_level, NULL,
     CLI_GET_DBG_LVL_HELP_STR,  CLI_SUPERVISOR},
    {"sdl",  NULL, cli_set_dbg_level, NULL,
     CLI_SET_DBG_LVL_HELP_STR,  CLI_SUPERVISOR},
    #endif 
    #ifdef CLI_LVL_ALL
    {"set_sync_src",  NULL, dt_cli_set_sync_src, NULL,
     "Specify how system time should be synchronized. (type 'set_sync_src' to see all the options)",
     CLI_SUPERVISOR},
    #endif
    {"get_brdcst_time",  NULL, dt_cli_get_curr_utc, NULL,
     "Get the current time synchronized with Broadcast stream.",
     CLI_GUEST
    },
    {"get_user_time",  NULL, dt_cli_get_user_utc, NULL,
     "Get the current time set by the user.",
     CLI_GUEST
    },
    {"set_user_time",  NULL, dt_cli_set_user_utc, NULL,
     "Set the user clock time in UTC sec (ex: set_user_time 1162337785).",
     CLI_GUEST
    },
    {"set_cntry_code",  NULL, dt_cli_set_sys_cntry_code, NULL,
     "Set the country code (ex: GBR for United Kingdom).",
     CLI_GUEST
    },
    {"get_cntry_code",  NULL, dt_cli_get_sys_cntry_code, NULL,
     "Get the current country code (ex: GBR for United Kingdom).",
     CLI_GUEST
    },
    {"list_cntry_code",   NULL, dt_cli_dump_cntry_code, NULL,
     "List all the country code/region id data received from the TOT table.",
     CLI_GUEST
    }, 
    {"tz_ctrl",   NULL, dt_cli_tz_ctrl, NULL,
     "Enable/disable timezone offset control.",
     CLI_GUEST
    },
    {"dst_ctrl",   NULL, dt_cli_dst_ctrl, NULL,
     "Enable/disable Day-Light-Saving control.",
     CLI_GUEST
    },
    {"set_tz",   NULL, dt_cli_set_tz, NULL,
     "Set timezone offset.",
     CLI_GUEST
    },
    {"set_dst",   NULL, dt_cli_set_dst, NULL,
     "Set Day-Light-Saving status.",
     CLI_GUEST
    },
    #ifdef CLI_LVL_ALL
    {"set_config",   NULL, dt_cli_config, NULL,
     "Set DT config [DT_USE_DST_AT_CURRENT_TIME]",
     CLI_SUPERVISOR
    },
    {"show",   NULL, dt_cli_dump_dt_info, NULL,
     "Show current DT setting and config",
     CLI_SUPERVISOR
    },
    #endif /* end of  #if(CLI_CMD_LEVEL && CLI_LEVEL_CLI ) */
    #ifdef DEBUG
    {"test",   NULL, dt_cli_test, NULL,
     "Test new clock API",
     CLI_GUEST
    },
    #endif
    END_OF_CLI_CMD_TBL
};

static CLI_EXEC_T at_cli_dbg_tbl [] =
{
    {"dt", NULL, NULL, &(at_cli_dbg_cmd[0]), "Date Time Library Command", CLI_GUEST},
    END_OF_CLI_CMD_TBL
};



/* --------------------------------------------------------------
  Function implementation.
---------------------------------------------------------------- */
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
#ifdef CLI_LVL_ALL
static INT32 cli_get_dbg_level
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;
    
    if ( i4_argc != 1 )
    {
/* #ifdef DEBUG  */
        x_dbg_stmt("Usage: dt.gdl\n");
/* #endif        */
        i4_rc = CLIR_INV_CMD_USAGE;
    }
    else
    {
        i4_rc = x_cli_show_dbg_level(dt_get_dbg_level());
    }
    
    return i4_rc;    
        
}

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
        dt_set_dbg_level(ui2_dbg);
        i4_rc = CLIR_OK;
    }    
    else
    {
/* #ifdef DEBUG    */
        x_dbg_stmt("Usage: dt.sdl <n|e|a|i>\n");
        x_dbg_stmt("where:  n=none, e=error, a=api, i=info. \n");
/* #endif        */
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;   
}
#endif

#ifdef CLI_LVL_ALL
static INT32 dt_cli_set_sync_src
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;
    DT_SYNC_SRC_TYPE_T   t_sync_src_type = DT_SYNC_SRC_NONE ;
    DT_SRC_TYPE_DESC_T   t_src_type_desc = DT_SRC_TYPE_FORMAT_UNKNOWN ;
    const VOID*          pv_src_info = NULL;

    if ( i4_argc == 2 )
    {
        if ( x_strcmp(pps_argv[1],"none") == 0 )
        {
            t_sync_src_type = DT_SYNC_SRC_NONE ;
            t_src_type_desc = DT_SRC_TYPE_FORMAT_UNKNOWN ;
            pv_src_info = NULL;
        }
        
        if ( x_strcmp(pps_argv[1],"atsc_terr") == 0 )
        {
            t_sync_src_type = DT_SYNC_WITH_ATSC_STT ;
            t_src_type_desc = DT_SRC_TYPE_MPEG_2_BRDCST;
            pv_src_info     = SN_TUNER_TER_DIG_0;
        }

        if ( x_strcmp(pps_argv[1],"atsc_cable") == 0 )
        {
            t_sync_src_type = DT_SYNC_WITH_ATSC_STT ;
            t_src_type_desc = DT_SRC_TYPE_MPEG_2_BRDCST;
            pv_src_info     = SN_TUNER_CAB_DIG_0;
        }

        if ( x_strcmp(pps_argv[1],"cable_oob") == 0 )
        {
            t_sync_src_type = DT_SYNC_WITH_SCTE_STT ;
            t_src_type_desc = DT_SRC_TYPE_FORMAT_UNKNOWN;
            pv_src_info     = NULL;
        }

        if ( x_strcmp(pps_argv[1],"dvb_terr_tdt_tot") == 0 )
        {
            t_sync_src_type = DT_SYNC_WITH_DVB_TDT_OR_TOT ;
            t_src_type_desc = DT_SRC_TYPE_MPEG_2_BRDCST;
            pv_src_info     = SN_TUNER_TER_DIG_0;
        }

        if ( x_strcmp(pps_argv[1],"dvb_terr_tot_tdt") == 0 )
        {
            t_sync_src_type = DT_SYNC_WITH_DVB_TOT_OR_TDT ;
            t_src_type_desc = DT_SRC_TYPE_MPEG_2_BRDCST;
            pv_src_info     = SN_TUNER_TER_DIG_0;
        }

        if ( x_strcmp(pps_argv[1],"dvb_cable_tdt_tot") == 0 )
        {
            t_sync_src_type = DT_SYNC_WITH_DVB_TDT_OR_TOT ;
            t_src_type_desc = DT_SRC_TYPE_MPEG_2_BRDCST;
            pv_src_info     = SN_TUNER_CAB_DIG_0;
        }

        if ( x_strcmp(pps_argv[1],"dvb_cable_tot_tdt") == 0 )
        {
            t_sync_src_type = DT_SYNC_WITH_DVB_TOT_OR_TDT ;
            t_src_type_desc = DT_SRC_TYPE_MPEG_2_BRDCST;
            pv_src_info     = SN_TUNER_CAB_DIG_0;
        }

        if ( x_strcmp(pps_argv[1],"dvb_terr_tot_only") == 0 )
        {
            t_sync_src_type = DT_SYNC_WITH_DVB_TOT ;
            t_src_type_desc = DT_SRC_TYPE_MPEG_2_BRDCST;
            pv_src_info     = SN_TUNER_TER_DIG_0;
        }
        
        if ( x_strcmp(pps_argv[1],"dvb_terr_tdt_only") == 0 )
        {
            t_sync_src_type = DT_SYNC_WITH_DVB_TDT ;
            t_src_type_desc = DT_SRC_TYPE_MPEG_2_BRDCST;
            pv_src_info     = SN_TUNER_TER_DIG_0;
        }

        if ( x_strcmp(pps_argv[1],"dvb_cable_tot_only") == 0 )
        {
            t_sync_src_type = DT_SYNC_WITH_DVB_TOT ;
            t_src_type_desc = DT_SRC_TYPE_MPEG_2_BRDCST;
            pv_src_info     = SN_TUNER_CAB_DIG_0;
        }
        
        if ( x_strcmp(pps_argv[1],"dvb_cable_tdt_only") == 0 )
        {
            t_sync_src_type = DT_SYNC_WITH_DVB_TDT ;
            t_src_type_desc = DT_SRC_TYPE_MPEG_2_BRDCST;
            pv_src_info     = SN_TUNER_CAB_DIG_0;
        }

        if ( x_strcmp(pps_argv[1],"teletext") == 0 )
        {
            t_sync_src_type = DT_SYNC_WITH_ANALOG_TELETEXT ;
            t_src_type_desc = DT_SRC_TYPE_VBI_ANA_TER ;
            pv_src_info     = NULL;
            x_dbg_stmt("set sync source to: DT_SYNC_WITH_ANALOG_TELETEXT\n");
        }

        if ( x_strcmp(pps_argv[1],"no_teletext") == 0 )
        {
            t_sync_src_type = DT_SYNC_SRC_ANALOG_NONE ;
            t_src_type_desc = DT_SRC_TYPE_FORMAT_UNKNOWN ;
            pv_src_info     = NULL;
            x_dbg_stmt("remove analog sync source\n");
        }

        

        i4_rc = x_dt_set_sync_src(t_sync_src_type, t_src_type_desc,pv_src_info);
        if ( i4_rc != DTR_OK )
        {
            i4_rc = CLIR_INV_CMD_USAGE;
        }
        else
        {
            i4_rc = CLIR_OK;
        }
    }    
    else
    {
/* #ifdef DEBUG            */
        x_dbg_stmt("Usage: set_sync_src <sync_source>\n");
        x_dbg_stmt("where <sync_source> specifies SI table to "
                   "synchronize the system time with. \n");
        x_dbg_stmt("Example:\n");
        x_dbg_stmt("\tset_sync_src atsc_terr  [sync with ATSC STT table]\n");
        x_dbg_stmt("\tset_sync_src dvb_terr_tot_tdt  [Sync with DVB TOT or TDT table, \n"
                   "\tafter TOT table is received, then only accept TOT table.]\n\n");
        
        x_dbg_stmt("The possible values for the synchronziation source are: \n\n");
        x_dbg_stmt("none:\t\t\tNo sync source (put brdcst system clock in free running).\n");
        x_dbg_stmt("atsc_terr:\t\tSync with terrestrial ATSC STT table.\n");
        
        x_dbg_stmt("atsc_cable:\t\tSync with cable ATSC STT table.\n");
        
        x_dbg_stmt("cable_oob:\t\tSync with cable out-of-band time table.\n");
        
        x_dbg_stmt("dvb_terr_tdt_tot:\tSync with DVB TDT or TOT from terrestrial broadcast,\n"
                   "\t\t\tbut once TDT is received, the system time received from TOT is rejected\n"
                   "\t\t\t(the country code and time zone info will still be processed.)\n");
        
        x_dbg_stmt("dvb_terr_tot_tdt:\tSync with DVB TOT or TDT from terrestrial broadcast,\n"
                   "\t\t\tbut once TOT is received, all TDT are rejected.\n");
        
        x_dbg_stmt("dvb_cable_tdt_tot:\tSync with DVB TDT or TOT from cable broadcast,\n"
                   "\t\t\tbut once TDT is received, the system time received from TOT is rejected\n"
                   "\t\t\t(the country code and time zone info will still be processed.)\n");
        
        x_dbg_stmt("dvb_cable_tot_tdt:\tSync with DVB TOT or TDT from cable broadcast,\n"
                   "\t\t\tbut once TOT is received, all TDT are rejected.\n");
        
        x_dbg_stmt("dvb_terr_tdt_only:\tSync only with TDT from terrestrial broadcast.\n");
        
        x_dbg_stmt("dvb_terr_tot_only:\tSync only with TOT from terrestrial broadcast \n");
        
        x_dbg_stmt("dvb_cable_tdt_only:\tSync only with TDT from cable broadcast.\n");
        
        x_dbg_stmt("dvb_cable_tot_only:\tSync only wiht TOT from cable broadcast.\n");
/* #endif    */
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

#endif

static INT32 dt_cli_get_curr_utc
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;
    DT_COND_T t_dt_cond;
    TIME_T    t_sec;
    DTG_T     t_dtg;
    DTG_T     t_local_dtg;
    
    t_sec = x_dt_get_brdcst_utc(NULL, &t_dt_cond);
    x_dt_utc_sec_to_dtg(t_sec, &t_dtg);
    x_dbg_stmt("current broadcast UTC second: [%lld] :: ", t_sec);
    dt_print_dtg(&t_dtg);

    x_dbg_stmt(":: corresponding local time is: ");
    x_dt_conv_utc_local(&t_dtg, &t_local_dtg);
    dt_print_dtg(&t_local_dtg);

    switch (t_dt_cond)
    {
        case DT_NOT_RUNNING:
        {
            x_dbg_stmt(" sys time cond: DT_NOT_RUNNING. ");
        }
        break;

        case DT_FREE_RUNNING:
        {
            x_dbg_stmt(" sys time cond: DT_FREE_RUNNING. ");
        }
        break;

        case DT_SYNC_RUNNING:
        {
            x_dbg_stmt(" sys time cond: DT_SYNC_RUNNING. ");
        }
        break;

        case DT_SYNC_DISCONT:
        {
            x_dbg_stmt(" sys time cond: DT_SYNC_DISCONT. ");
        }
        break;

        case DT_LTO_UPDATED:
        {
            x_dbg_stmt(" sys time cond: DT_LTO_UPDATED. ");
        }
        break;

        case DT_NO_SYNC_SRC:
        {
            x_dbg_stmt(" sys time cond: DT_NO_SYNC_SRC. ");
        }
        break;

        default:
            break;
    }
    
            
    i4_rc = CLIR_OK;
    
    return i4_rc;  
}

static INT32 dt_cli_get_user_utc
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;
    DT_COND_T t_dt_cond;
    TIME_T    t_sec;
    DTG_T     t_dtg;
    DTG_T     t_local_dtg;
    
    t_sec = x_dt_get_utc(NULL, &t_dt_cond);
    x_dt_utc_sec_to_dtg(t_sec, &t_dtg);
    x_dbg_stmt("current user time in UTC second: [%lld] :: ", t_sec);
    dt_print_dtg(&t_dtg);

    x_dbg_stmt(":: corresponding local time is: ");
    x_dt_conv_utc_local(&t_dtg, &t_local_dtg);
    dt_print_dtg(&t_local_dtg);

    switch (t_dt_cond)
    {
        case DT_NOT_RUNNING:
        {
            x_dbg_stmt(" user time cond: DT_NOT_RUNNING. ");
        }
        break;

        case DT_FREE_RUNNING:
        {
            x_dbg_stmt(" user time cond: DT_FREE_RUNNING. ");
        }
        break;

        case DT_SYNC_RUNNING:
        {
            x_dbg_stmt(" user time cond: DT_SYNC_RUNNING. ");
        }
        break;

        case DT_SYNC_DISCONT:
        {
            x_dbg_stmt(" user time cond: DT_SYNC_DISCONT. ");
        }
        break;

        case DT_LTO_UPDATED:
        {
            x_dbg_stmt(" user time cond: DT_LTO_UPDATED. ");
        }
        break;

        case DT_NO_SYNC_SRC:
        {
            x_dbg_stmt(" user time cond: DT_NO_SYNC_SRC. ");
        }
        break;

        default:
            break;
    }
    
            
    i4_rc = CLIR_OK;
    
    return i4_rc;  
}

static INT32 dt_cli_get_sys_cntry_code
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32              i4_rc;
    ISO_3166_COUNT_T   t_country_code;
    UINT16             ui2_region_id;

    ui2_region_id = 0;
    t_country_code[ISO_3166_COUNT_LEN-1] = '\0';
    
    if ( x_dt_get_sys_count_code (&t_country_code, &ui2_region_id)
         == DTR_OK )
    {
        x_dbg_stmt("Current country code is: [%s] region id is: [%d]\n ",
                   t_country_code, ui2_region_id);
        i4_rc = CLIR_OK;
    }
    else
    {
        x_dbg_stmt("Error: can not get current system country code.\n");
        i4_rc = CLIR_CMD_EXEC_ERROR ;
    }
    return i4_rc;  
}


static INT32 dt_cli_set_user_utc
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    TIME_T            t_utc;
    INT32             i4_rc;
    DTG_T             t_dtg;
    
    if ( i4_argc == 2 )
    {
        t_utc = x_strtoll (pps_argv[1], NULL, 10);
        x_dt_sync_with_brdcst_utc(FALSE);
        x_dt_set_utc(t_utc, 0);
        x_dt_utc_sec_to_dtg(t_utc, &t_dtg);
        x_dbg_stmt("Set user clock to UTC second: [%lld] ",
                   t_utc);
        dt_print_dtg(&t_dtg);
        i4_rc = CLIR_OK;
    }
    else  if ( i4_argc == 1 )
    {
        x_dt_sync_with_brdcst_utc(TRUE);
        x_dbg_stmt("Synchronize user clock to broadcast clock.\n");
        i4_rc = CLIR_OK;
        
    }
    else
    {      
        x_dbg_stmt("Usage: set_country_code <country_code> <region_id>\n");
        x_dbg_stmt("Example:  GBR 0 \n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

static INT32 dt_cli_set_sys_cntry_code
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32             i4_rc;
    ISO_3166_COUNT_T  t_country_code;
    UINT16            ui2_region_id;
    
    if ( i4_argc == 3 )
    {
        x_strncpy(t_country_code, pps_argv[1], ISO_3166_COUNT_LEN-1);
        t_country_code[ISO_3166_COUNT_LEN-1] = '\0';
        
        ui2_region_id = x_strtoll (pps_argv[2], NULL, 10);
        if ( x_dt_set_sys_count_code (t_country_code, ui2_region_id)
             == DTR_OK )
        {
            x_dbg_stmt("Set system country code: [%s] region_id: [%d]\n",
                       t_country_code, ui2_region_id);
            i4_rc = CLIR_OK;
        }
        else
        {
            x_dbg_stmt("Error: can not set country code.\n");
            i4_rc = CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {      
        x_dbg_stmt("Usage: set_country_code <country_code> <region_id>\n");
        x_dbg_stmt("Example:  GBR 0 \n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

static INT32 dt_cli_dump_cntry_code
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32              i4_rc = CLIR_OK;
    INT32              i4_nb_cntry;
    UINT16             ui2_i;
    ISO_3166_COUNT_T   t_country_code;
    UINT16             ui2_region_id;
    TIME_T             t_offset;

    i4_nb_cntry=x_dt_get_num_count_code();

    x_dbg_stmt("Number of country code found: %d\n",i4_nb_cntry);
    
    for ( ui2_i = 0 ; ui2_i < i4_nb_cntry; ui2_i++ )
    {
        ui2_region_id = 0;
        if ( x_dt_get_count_code (ui2_i, &t_country_code, &ui2_region_id, &t_offset)
             == DTR_OK
            )
        {
            x_dbg_stmt("[%d] country code is: [%s] region id is: [%d] local offset: [%lld]\n ",
                   ui2_i, t_country_code, ui2_region_id, t_offset);
            i4_rc = CLIR_OK;
        }
        else
        {
            x_dbg_stmt("Error retriving country code/region id database.\n");
            i4_rc = CLIR_CMD_EXEC_ERROR ;
            break;
        }    
    }
    
    return i4_rc;  
}

static INT32 dt_cli_tz_ctrl(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32             i4_rc;
    BOOL              b_tz;
    
    if ( i4_argc == 2 )
    {
        if ( x_strcmp("on", pps_argv[1]) == 0 )
        {
            b_tz = TRUE;
            x_dbg_stmt("Accept timezone offset received from tables.\n");
        }
        else
        {
            b_tz = FALSE;
            x_dbg_stmt("Ignore timezone offset received from tables.\n");
        }
        x_dt_set_tz_ctrl(b_tz);
        i4_rc = CLIR_OK;
    }
    else
    {
        BOOL     tz_ctrl;
        
        tz_ctrl=x_dt_get_tz_ctrl();
        if ( tz_ctrl == TRUE )
        {
            x_dbg_stmt("Timezone control is enabled, e.g, accept timezone setting from broadcast stream.\n\n");
        }
        else
        {
            x_dbg_stmt("Timezone control is disabled, e.g., ignore timezone setting from broadcast stream.\n\n");
        }
        x_dbg_stmt("Usage: tz_ctrl <on|off>\n");
        x_dbg_stmt("Example: tz_ctrl on  \n");
        x_dbg_stmt("         [Accept timezone offset setting from broadcast tables.\n");
        x_dbg_stmt("          Current setting of timezone offset will be modified by the\n");
        x_dbg_stmt("          broadcast tables.]\n");
        x_dbg_stmt("\n");
        x_dbg_stmt("Example:  tz_ctrl off\n");
        x_dbg_stmt("          [Ignore timezone offset setting from broadcast tables.]\n");
        
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;
}

static INT32 dt_cli_dst_ctrl(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32             i4_rc;
    BOOL              b_dst_ctrl;
    
    if ( i4_argc == 2 )
    {
        if ( x_strcmp("on", pps_argv[1]) == 0 )
        {
            b_dst_ctrl = TRUE;
            x_dbg_stmt("Accept Day-Light-Saving offset received from tables.\n");
        }
        else
        {
            b_dst_ctrl = FALSE;
            x_dbg_stmt("Ignore Day-Light-Saving offset received from tables.\n");
        }
        x_dt_set_dst_ctrl(b_dst_ctrl);
        i4_rc = CLIR_OK;
    }
    else
    {
        BOOL     dst_ctrl;
        
        dst_ctrl=x_dt_get_dst_ctrl();
        if ( dst_ctrl == TRUE )
        {
            x_dbg_stmt("Day-Light-Saving control is enabled, e.g, accept DST setting from broadcast stream.\n\n");
        }
        else
        {
            x_dbg_stmt("Day-Light-Saving control is disabled, e.g., ignore DST setting from broadcast stream.\n\n");
        }
        x_dbg_stmt("Usage: dst_ctrl <on|off> \n");
        x_dbg_stmt("Example:  dst_ctrl on \n");
        x_dbg_stmt("          [Accept Day-Light-Saving offset setting from broadcast tables, \n");
        x_dbg_stmt("           the current Day-Light-Saving status will be modified by the\n");
        x_dbg_stmt("           broadcast tables.]\n");
        x_dbg_stmt("\n");
        x_dbg_stmt("Example:  dst_ctrl off\n");
        x_dbg_stmt("          [Ignore Day-Light-Saving offset setting from broadcast tables.]\n");
        
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;    
}

static INT32 dt_cli_set_tz(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32             i4_rc;
    TIME_T            t_tz_offset;
    
    if ( i4_argc == 2 )
    {
        t_tz_offset = x_strtoll (pps_argv[1], NULL, 10);
        x_dt_set_tz(t_tz_offset);
        x_dbg_stmt("Set current timezone offset: [%lld] seconds\n",
                   t_tz_offset);
        i4_rc = CLIR_OK;
    }
    else
    {
        t_tz_offset = x_dt_get_tz();
        x_dbg_stmt("Current Timezone offset: %lld seconds\n\n",t_tz_offset);
        
        x_dbg_stmt("Usage: set_tz <timezone_offset>\n");
        x_dbg_stmt("Example: set_tz -3600 \n");
        x_dbg_stmt("         [Set timezone offset to -3600 seconds.]\n");
        
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;
}

static INT32 dt_cli_set_dst(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32             i4_rc;
    BOOL              b_dst;
    
    if ( i4_argc == 2 )
    {
        if ( x_strcmp("on", pps_argv[1]) == 0 )
        {
            b_dst = TRUE;
            x_dbg_stmt("Set Day-Light-Saving status to TRUE.\n");
        }
        else
        {
            b_dst = FALSE;
            x_dbg_stmt("Set Day-Light-Saving status to FALSE.\n");
        }
        x_dt_set_dst(b_dst);
        i4_rc = CLIR_OK;
    }
    else
    {
        b_dst = x_dt_get_dst();

        if ( b_dst == TRUE )
        {
            x_dbg_stmt("Current DayLightSaving status: on. (offset due to day-light-saving: %lld seconds)\n\n",
                       dt_get_dst_offset());
        }
        else
        {
            x_dbg_stmt("Current DayLightSaving status: off. \n\n");
        }
        
        x_dbg_stmt("Usage: set_dst <day-light-saving flag>\n");
        x_dbg_stmt("Example:  set_dst on\n");
        x_dbg_stmt("          [Set Day-Light-Saving status to TRUE.]\n");
        x_dbg_stmt("\n");
        x_dbg_stmt("Example:  set_dst off\n");
        x_dbg_stmt("          [Set Day-Light-Saving status to FALSE.]\n");

        
        
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;    
}

#ifdef CLI_LVL_ALL
void dt_show_config_info(void);

static INT32 dt_cli_config(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32             i4_rc;
    
    if ( i4_argc == 2 )
    {
        
        
        if ( x_strcmp("DT_USE_DST_AT_CURRENT_TIME", pps_argv[1]) == 0 )
        {
            x_dbg_stmt("DT_USE_DST_AT_CURRENT_TIME.\n");
            x_dt_set_config( DT_USE_DST_AT_CURRENT_TIME );
        }
        else if ( x_strcmp("DT_USE_DST_AT_SPECIFIED_TIME", pps_argv[1]) == 0 )
        {
            x_dbg_stmt("DT_USE_DST_AT_SPECIFIED_TIME.\n");
            x_dt_set_config( DT_USE_DST_AT_SPECIFIED_TIME );
        }
        else if ( x_strcmp("DT_DVB_MATCH_CNTRY_AND_REG_CODE",pps_argv[1]) == 0 )
        {
            x_dbg_stmt("DT_DVB_MATCH_CNTRY_AND_REG_CODE.\n");
            x_dt_set_config( DT_DVB_MATCH_CNTRY_AND_REG_CODE );
        }
        else if ( x_strcmp("DT_DVB_MATCH_CNTRY_CODE",pps_argv[1]) == 0 )
        {
            x_dbg_stmt("DT_DVB_MATCH_CNTRY_CODE.\n");
            x_dt_set_config(DT_DVB_MATCH_CNTRY_CODE);
        }
        else if ( x_strcmp("DT_DVB_MATCH_ANY_CNTRY_OR_REG_CODE",pps_argv[1]) == 0 )
        {
            x_dbg_stmt("DT_DVB_MATCH_ANY_CNTRY_OR_REG_CODE.\n");
            x_dt_set_config(DT_DVB_MATCH_ANY_CNTRY_OR_REG_CODE);
        }
        
        i4_rc = CLIR_OK;
    }
    else
    {
        x_dbg_stmt("Current configuration setting: \n");
        dt_show_config_info();
        x_dbg_stmt("\n");
        
        x_dbg_stmt("Usage: config <CONFIG PARAM>\n");
        x_dbg_stmt("Example:  \n");
        x_dbg_stmt("\t config DT_USE_DST_AT_CURRENT_TIME \n");
        x_dbg_stmt("\t config DT_USE_DST_AT_SPECIFIED_TIME \n");
        x_dbg_stmt("\t config DT_DVB_MATCH_CNTRY_AND_REG_CODE \n");
        x_dbg_stmt("\t config DT_DVB_MATCH_CNTRY_CODE \n");
        x_dbg_stmt("\t config DT_DVB_MATCH_ANY_CNTRY_OR_REG_CODE \n");
        
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;    
}

static INT32 dt_cli_dump_dt_info(INT32 i4_argc, const CHAR** pps_argv)
{
    x_dbg_stmt("DVB information:\n");
    dt_cli_get_sys_cntry_code(i4_argc,pps_argv);
    dt_cli_dump_cntry_code(i4_argc,pps_argv);
    
    if (  x_dt_get_dst_ctrl() == TRUE )
    {
        x_dbg_stmt("Day-Light-Saving control is enabled, e.g, accept DST setting from broadcast stream.\n\n");
    }
    else
    {
        x_dbg_stmt("Day-Light-Saving control is disabled, e.g., ignore DST setting from broadcast stream.\n\n");
    }

    if ( x_dt_get_dst() == TRUE )
    {
        x_dbg_stmt("Current DayLightSaving status: on. (offset due to day-light-saving: %lld seconds)\n\n",
                   dt_get_dst_offset());
    }
    else
    {
        x_dbg_stmt("Current DayLightSaving status: off. \n\n");
    }

    if ( x_dt_get_tz_ctrl() == TRUE )
    {
        x_dbg_stmt("Timezone control is enabled, e.g, accept timezone setting from broadcast stream.\n\n");
    }
    else
    {
        x_dbg_stmt("Timezone control is disabled, e.g., ignore timezone setting from broadcast stream.\n\n");
    }
    
    x_dbg_stmt("Current Timezone offset: %lld seconds\n\n", x_dt_get_tz());

    x_dbg_stmt("Current configuration setting: \n");
    dt_show_config_info();
    x_dbg_stmt("\n");

    return CLIR_OK;
}

#endif /* end of   #if (CLI_CMD_LEVEL && CLI_LEVEL_CLI) */  

#ifdef DEBUG

static CHAR* cond_code_to_str(DT_COND_T t_dt_cond)
{
    CHAR* s_cond;

    switch (t_dt_cond)
    {
        case DT_NOT_RUNNING:
        {
            s_cond="DT_NOT_RUNNING";
        }
        break;

        case DT_FREE_RUNNING:
        {
            s_cond="DT_FREE_RUNNING";
        }
        break;

        case DT_SYNC_RUNNING:
        {
            s_cond="DT_SYNC_RUNNING";
        }
        break;

        case DT_SYNC_DISCONT:
        {
            s_cond="DT_SYNC_DISCONT";
        }
        break;

        case DT_NO_SYNC_SRC:
        {
            s_cond="DT_NO_SYNC_SRC";
        }
        break;

        case DT_DAY_LGT_SAV_CHANGED:
        {
            s_cond="DT_DAY_LGT_SAV_CHANGED";
        }
        break;

        case DT_TZ_OFFSET_CHANGED:
        {
            s_cond="DT_TZ_OFFSET_CHANGED";
        }
        break;

        default:
        {
            s_cond="unknown";
        }
        break;
    }
    return s_cond;
}

static CHAR* sync_src_type_to_str(DT_SYNC_SRC_TYPE_T t_sync_src_type)
{
    CHAR* s_cond;

    switch (t_sync_src_type)
    {
        case DT_SYNC_SRC_NONE:
        {
            s_cond="";
        }
        break;

        case DT_SYNC_WITH_DVB_TDT:
        {
            s_cond="DT_SYNC_WITH_DVB_TDT";
        }
        break;

        case DT_SYNC_WITH_DVB_TOT:
        {
            s_cond="DT_SYNC_WITH_DVB_TOT";
        }
        break;

        case DT_SYNC_WITH_ATSC_STT:
        {
            s_cond="DT_SYNC_WITH_ATSC_STT";
        }
        break;

        case DT_SYNC_WITH_SCTE_STT:
        {
            s_cond="DT_SYNC_WITH_SCTE_STT";
        }
        break;

        case DT_SYNC_WITH_DVB_TDT_OR_TOT:
        {
            s_cond="DT_SYNC_WITH_DVB_TDT_OR_TOT";
        }
        break;

        case DT_SYNC_WITH_DVB_TOT_OR_TDT:
        {
            s_cond="DT_SYNC_WITH_DVB_TOT_OR_TDT";
        }
        break;

        case DT_SYNC_SRC_ANALOG_NONE:
        {
            s_cond="DT_SYNC_SRC_ANALOG_NONE";
        }
        break;

        case DT_SYNC_WITH_ANALOG_TELETEXT:
        {
            s_cond="DT_SYNC_WITH_ANALOG_TELETEXT";
        }
        break;

        default:
        {
            s_cond="unknown";
        }
        break;
    }
    return s_cond;
}

static CHAR* src_type_desc_to_str(DT_SRC_TYPE_DESC_T t_src_type_desc)
{
    CHAR* s_cond;

    switch (t_src_type_desc)
    {
        case DT_SRC_TYPE_FORMAT_UNKNOWN:
        {
            s_cond="";
        }
        break;

        case DT_SRC_TYPE_MPEG_2_BRDCST:
        {
            s_cond="DT_SRC_TYPE_MPEG_2_BRDCST";
        }
        break;

        case DT_SRC_TYPE_VBI_ANA_TER:
        {
            s_cond="DT_SRC_TYPE_VBI_ANA_TER";
        }
        break;

        case DT_SRC_TYPE_VBI_ANA_CAB:
        {
            s_cond="DT_SRC_TYPE_VBI_ANA_CAB";
        }
        break;

        case DT_SRC_TYPE_VBI_ANA_SAT:
        {
            s_cond="DT_SRC_TYPE_VBI_ANA_SAT";
        }
        break;

        case DT_SRC_TYPE_CONN_HANDLE:
        {
            s_cond="DT_SRC_TYPE_CONN_HANDLE";
        }
        break;

        default:
        {
            s_cond="unknown";
        }
        break;
    }
    return s_cond;
}


static VOID clock_nfy_fct(HANDLE_T  h_hdl,VOID*  pv_tag, DT_COND_T t_dt_cond, TIME_T t_delta)
{
    CHAR* s_cond;
    s_cond = cond_code_to_str(t_dt_cond);   
    x_dbg_stmt("{clock_nfy_fct} handle %d , cond: %s, delta: %lld\n",
               h_hdl, s_cond, t_delta);
    return;
}

static CHAR* utc_cond_str(TIME_T  t_utc, UINT16 ui2_msec, DT_COND_T t_dt_cond)
{
    static CHAR  s_str[48];
    
    x_sprintf(s_str, "UTC: %lld [ms: %d] [%s]\0", t_utc, ui2_msec,cond_code_to_str(t_dt_cond));

    return s_str;
}

static HANDLE_T  new_clock(CHAR* s_name)
{
    INT32             i4_rc;
    UINT16            ui2_milli_sec;
    DT_COND_T         t_dt_cond;
    TIME_T            t_utc;
    

    HANDLE_T   h_clock = NULL_HANDLE;

    i4_rc=x_dt_clock_create(s_name,clock_nfy_fct, NULL, &h_clock);
    if ( i4_rc != DTR_OK )
    {
        x_dbg_stmt("create clock failed: %d\n", i4_rc);
    }
    else
    {
        x_dbg_stmt("clock created success: %d\n", h_clock);
    }
        
    t_utc=x_dt_clock_get_utc(h_clock, &ui2_milli_sec, &t_dt_cond);
    x_dbg_stmt("Get time: %s \n",  utc_cond_str(t_utc,ui2_milli_sec,t_dt_cond));

    t_utc = 946684800;
    ui2_milli_sec = 234;
        
    x_dt_clock_set_utc(h_clock, t_utc,ui2_milli_sec);
    x_dbg_stmt("Set time: %s \n",  utc_cond_str(t_utc,ui2_milli_sec,t_dt_cond));

        
    t_utc=x_dt_clock_get_utc(h_clock, &ui2_milli_sec, &t_dt_cond);
    x_dbg_stmt("Get time: %s \n",  utc_cond_str(t_utc,ui2_milli_sec,t_dt_cond));
    
    return h_clock;
}

static HANDLE_T  new_clock_handle(CHAR* s_name)
{
    INT32             i4_rc;
    HANDLE_T   h_clock = NULL_HANDLE;

    i4_rc=x_dt_clock_get_handle(s_name,clock_nfy_fct, NULL, &h_clock);
    if ( i4_rc != DTR_OK )
    {
        x_dbg_stmt("get clock handle failed: %d\n", i4_rc);
    }
    else
    {
        x_dbg_stmt("clock handle created success: %d\n", h_clock);
    }
    
    return h_clock;
}

static VOID set_sync
(
    HANDLE_T                    h_clock,
    DT_SYNC_SRC_TYPE_T          t_sync_src_type,
    DT_SRC_TYPE_DESC_T          t_src_type_desc,
    const VOID*                 pv_src_info
)
{
    INT32 i4_rc;
    
    x_dbg_stmt("Set sync source: %s  %s\n",
               sync_src_type_to_str(t_sync_src_type),
               src_type_desc_to_str(t_src_type_desc));
    
    i4_rc = x_dt_clock_set_sync_src(h_clock,
                                    t_sync_src_type,
                                    t_src_type_desc,
                                    pv_src_info);

    if ( i4_rc != DTR_OK )
    {
        x_dbg_stmt("Set sync source failed: error code: %d\n", i4_rc);
    }
    else
    {
        x_dbg_stmt("Set sync source success. \n");
    }
}

extern VOID dt_print_utc(TIME_T t_utc_sec);

static INT32 dt_cli_test(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32             i4_rc;
    UINT16            ui2_milli_sec;
    DT_COND_T         t_dt_cond;
    TIME_T            t_utc;
    
    
    if ( i4_argc == 2 )
    {
        i4_rc = CLIR_OK;
    }
    else
    {
        HANDLE_T   h_clock_1 = NULL_HANDLE;
        HANDLE_T   h_clock_2 = NULL_HANDLE;
        HANDLE_T   h_clock_3 = NULL_HANDLE;
        h_clock_1 = new_clock("clock_1");
        h_clock_2 = new_clock("clock_2");
        
            
        {
            DT_SYNC_SRC_TYPE_T          t_sync_src_type;
            DT_SRC_TYPE_DESC_T          t_src_type_desc;
            VOID*                       pv_src_info;

            t_sync_src_type  = DT_SYNC_WITH_DVB_TDT_OR_TOT ;
            t_src_type_desc  =  DT_SRC_TYPE_CONN_HANDLE;
            pv_src_info = NULL;

            set_sync(h_clock_1,t_sync_src_type,t_src_type_desc,pv_src_info);

            t_sync_src_type  = DT_SYNC_WITH_DVB_TDT_OR_TOT ;
            t_src_type_desc  =  DT_SRC_TYPE_MPEG_2_BRDCST;
            pv_src_info = SN_TUNER_TER_DIG_0;

            set_sync(h_clock_2,t_sync_src_type,t_src_type_desc,pv_src_info);
        }

        x_thread_delay(3 * 1000);

        t_utc=x_dt_clock_get_utc(h_clock_1, &ui2_milli_sec, &t_dt_cond);
        x_dbg_stmt("clock_1 Get time: %s \n",  utc_cond_str(t_utc,ui2_milli_sec,t_dt_cond));
        dt_print_utc(t_utc);
        
        t_utc=x_dt_clock_get_utc(h_clock_2, &ui2_milli_sec, &t_dt_cond);
        x_dbg_stmt("clock_2 Get time: %s \n",  utc_cond_str(t_utc,ui2_milli_sec,t_dt_cond));
        dt_print_utc(t_utc);


        h_clock_3 = new_clock_handle("clock_2");

        t_utc=x_dt_clock_get_utc(h_clock_3, &ui2_milli_sec, &t_dt_cond);
        x_dbg_stmt("clock_3 Get time: %s \n",  utc_cond_str(t_utc,ui2_milli_sec,t_dt_cond));
        dt_print_utc(t_utc);


        x_thread_delay(3 * 1000);
        
        x_dbg_stmt("Close clock handle: %d\n", h_clock_1);
        x_handle_free(h_clock_1);

        x_dbg_stmt("Close clock handle: %d\n", h_clock_2);
        x_handle_free(h_clock_2);

        x_dbg_stmt("Close clock handle: %d\n", h_clock_3);
        x_handle_free(h_clock_3);

        i4_rc=CLIR_OK;
    }
    return i4_rc;
}

 #endif /* end  #if (CLI_CMD_LEVEL && CLI_LEVEL_DEBUG) */

INT32 dt_cli_init(VOID)
{
    /* Attach Date Time library CLI command table to CLI */
    if ( x_cli_attach_cmd_tbl(at_cli_dbg_tbl, CLI_CAT_MW, CLI_GRP_NONE)
         != CLIR_OK)
    {
        return DTR_FAIL;
    }
    return DTR_OK;   
}


#endif  /* CLI_LVL_ALL || CLI_LVL_GUEST */
    

    
