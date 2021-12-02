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
 * $RCSfile: sb_cli.c,v $
 * $Revision: #1 $ 
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/14 $
 * $SWAuthor: Alec Lu $
 * $MD5HEX: 73b7ae919c00c7ddd3f9bedfd3a77f13 $
 *
 * Description: 
 *         This file implements CLI command table for SVL Builder.
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
#include "svl/x_svl.h"
#include "tsl/x_tsl.h"
#ifndef NO_NWL_INIT
#include "nwl/x_nwl.h"
#endif
#include "svl_bldr/x_sb.h"
#include "svl_bldr/sb_slctr.h"
#include "svl_bldr/sb_atsc_eng/u_sb_atsc_eng.h"
#include "res_mngr/drv/u_tuner.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
#define MAX_APP_REC         32 
#define MIN_SCAN_IDX        2
#define MAX_SCAN_IDX        69
#define MAX_STR_SIZE        128
#define SVL_ID              ((UINT16)   0x1)
#define TSL_ID              SVL_ID
#define NWL_ID              SVL_ID

typedef struct _SB_APP_REC
{
    UINT32      ui4_tag;
    HANDLE_T    h_builder;
} SB_APP_REC_T;
#endif  /* CLI_LVL_ALL */

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static SB_APP_REC_T         at_app_rec[MAX_APP_REC];
static UINT32               ui4_app_count;
static CHAR                 sz_svl_name[MAX_STR_SIZE];
static CHAR                 sz_file_name[MAX_STR_SIZE];
static CHAR                 sz_tuner_name[MAX_STR_SIZE];
static UINT32               ui4_scan_start_idx;
static UINT32               ui4_scan_stop_idx;
static UINT32               ui4_cur_scan_idx;
static UINT32               ui4_ch_num_found;
static UINT8                aui1_scan_result[MAX_SCAN_IDX];

static SB_ATSC_OPEN_DATA_T  t_atsc_open_data = 
{
    SVL_ID,
    sz_svl_name,
    sz_file_name,
    sz_tuner_name,        
    FALSE,
}; 

static SB_ATSC_SCAN_DATA_T  t_atsc_scan_data = 
{
    SB_ATSC_SCAN_TYPE_UNKNOWN,
    0
};

static INT32 _sb_cli_dump_svl(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _sb_cli_dump_tsl(INT32 i4_argc, const CHAR** pps_argv);
#ifndef NO_NWL_INIT
static INT32 _sb_cli_dump_nwl(INT32 i4_argc, const CHAR** pps_argv);
#endif
static INT32 _sb_cli_open_builder(INT32 i4_argc, const CHAR** pps_argv); 
static INT32 _sb_cli_close_builder(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _sb_cli_start_scan(INT32 i4_argc, const CHAR** pps_argv);   
static INT32 _sb_cli_cancel_scan(INT32 i4_argc, const CHAR** pps_argv);  
static INT32 _sb_cli_save_lists(INT32 i4_argc, const CHAR** pps_argv);   
static INT32 _sb_cli_load_lists(INT32 i4_argc, const CHAR** pps_argv);   
static INT32 _sb_cli_clean_lists(INT32 i4_argc, const CHAR** pps_argv);  
static INT32 _sb_cli_get_cond(INT32 i4_argc, const CHAR** pps_argv);     
static INT32 _sb_cli_is_inited(INT32 i4_argc, const CHAR** pps_argv);    
static INT32 _sb_cli_list_app_rec(INT32 i4_argc, const CHAR** pps_argv);    
static INT32 _sb_cli_list_scan_rslt(INT32 i4_argc, const CHAR** pps_argv);   
static INT32 _sb_cli_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv); 
static INT32 _sb_cli_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv);

/* SVL Builder command table */
static CLI_EXEC_T at_sb_cli_tbl[] = 
{
    {"dumpsvl",             "ds",       _sb_cli_dump_svl,           NULL,           "Dump the SVL",                         CLI_SUPERVISOR},
    {"dumptsl",             "dt",       _sb_cli_dump_tsl,           NULL,           "Dump the TSL",                         CLI_SUPERVISOR},
#ifndef NO_NWL_INIT
    {"dumpnwl",             "dn",       _sb_cli_dump_nwl,           NULL,           "Dump the NWL",                         CLI_SUPERVISOR},
#endif
 	{"openb",               "ob",       _sb_cli_open_builder,       NULL,           "Open a builder",                       CLI_SUPERVISOR},
	{"closeb",              "cb",       _sb_cli_close_builder,      NULL,           "Close a builder",                      CLI_SUPERVISOR},
	{"starts",              "ss",       _sb_cli_start_scan,         NULL,           "Start a scan",                         CLI_SUPERVISOR},  
    {"cancels",             "cs",       _sb_cli_cancel_scan,        NULL,           "Cancle a scan",                        CLI_SUPERVISOR},  	
    {"savel",               "sl",       _sb_cli_save_lists,         NULL,           "Save SVL & TSL",                       CLI_SUPERVISOR},
	{"loadl",               "ll",       _sb_cli_load_lists,         NULL,           "Load SVL & TSL",                       CLI_SUPERVISOR},
 	{"cleanl",              "cl",       _sb_cli_clean_lists,        NULL,           "Clean SVL & TSL",                      CLI_SUPERVISOR},
    {"getc",                "gc",       _sb_cli_get_cond,           NULL,           "Get builder condition",                CLI_SUPERVISOR},
    {"isinit",              "ii",       _sb_cli_is_inited,          NULL,           "Check SB init status",                 CLI_SUPERVISOR},
    {"lista",               "la",       _sb_cli_list_app_rec,       NULL,           "List app record",                      CLI_SUPERVISOR},    
    {"lists",               "ls",       _sb_cli_list_scan_rslt,     NULL,           "List scan result",                     CLI_SUPERVISOR},        
 	{CLI_GET_DBG_LVL_STR,   NULL,       _sb_cli_get_dbg_level,      NULL,           CLI_GET_DBG_LVL_HELP_STR,               CLI_SUPERVISOR},
	{CLI_SET_DBG_LVL_STR,   NULL,       _sb_cli_set_dbg_level,      NULL,           CLI_SET_DBG_LVL_HELP_STR,               CLI_SUPERVISOR},    
    END_OF_CLI_CMD_TBL,
    END_OF_CLI_CMD_TBL,
    END_OF_CLI_CMD_TBL,
    END_OF_CLI_CMD_TBL,
    /* Last entry is used as an ending indicator */	
    END_OF_CLI_CMD_TBL
};

/* SVL Builder root command table */
static CLI_EXEC_T at_sb_root_cmd_tbl[] =
{
	{"sb",     NULL,        NULL,       at_sb_cli_tbl,      "SVL Builder commands",     CLI_SUPERVISOR},
	END_OF_CLI_CMD_TBL
};

static const SIZE_T z_cmd_tbl    = sizeof(at_sb_cli_tbl)/sizeof(CLI_EXEC_T)-1;
static SIZE_T       z_last_entry = 16;
#endif  /* CLI_LVL_ALL */

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * Name: _sb_nfy_fct
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _sb_nfy_fct(HANDLE_T    h_builder,
                        SB_COND_T   e_builder_cond,
                        VOID*       pv_nfy_tag)                        
{
    switch (e_builder_cond)
    {
        case SB_COND_AVAIL:
            x_dbg_stmt(_SB_INFO"cond = avail, nfy_tag = %d\n\r", *(UINT32*)pv_nfy_tag);
            break;
            
        case SB_COND_UNCONFIG:
            x_dbg_stmt(_SB_INFO"cond = unconfig, nfy_tag = %d\n\r", *(UINT32*)pv_nfy_tag);
            break;
            
        case SB_COND_INITING:
            x_dbg_stmt(_SB_INFO"cond = initing, nfy_tag = %d\n\r", *(UINT32*)pv_nfy_tag);
            break;
            
        case SB_COND_BUILDING:
            x_dbg_stmt(_SB_INFO"cond = building, nfy_tag = %d\n\r", *(UINT32*)pv_nfy_tag);
            break;     
            
        case SB_COND_UPDATE:
            x_dbg_stmt(_SB_INFO"cond = update, nfy_tag = %d\n\r", *(UINT32*)pv_nfy_tag);
            break; 
            
        case SB_COND_CLOSED:
            x_dbg_stmt(_SB_INFO"cond = closed, nfy_tag = %d\n\r", *(UINT32*)pv_nfy_tag);
            break;   
            
        default:        
            x_dbg_stmt(_SB_INFO"cond = unknown\n\r");
    }
}


/*-----------------------------------------------------------------------------
 * Name: _sb_scan_nfy_fct
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _sb_scan_nfy_fct(HANDLE_T    h_builder,
                             UINT32      ui4_nfy_reason,
                             INT32       i4_nfy_data,
                             VOID*       pv_nfy_tag)                          
{
    BOOL b_reason = FALSE;
    
    x_dbg_stmt(_SB_INFO"nfy_reason = ");
    
    if (ui4_nfy_reason & SB_REASON_COMPLETE)
    {
        if (b_reason)
        {
            x_dbg_stmt(" | ");
        }
                
        x_dbg_stmt("complete");
        b_reason = TRUE;
    }
    else if (ui4_nfy_reason & SB_REASON_PROGRESS)
    {
        if (b_reason)
        {
            x_dbg_stmt(" | ");
        }
                
        x_dbg_stmt("progress");
        b_reason = TRUE;
    }
    else if (ui4_nfy_reason & SB_REASON_CANCEL)
    {
        if (b_reason)
        {
            x_dbg_stmt(" | ");
        }
                
        x_dbg_stmt("cancel");
        b_reason = TRUE;
    }
    else if (ui4_nfy_reason & SB_REASON_ABORT)
    {
        if (b_reason)
        {
            x_dbg_stmt(" | ");
        }
                
        x_dbg_stmt("abort");
        b_reason = TRUE;
    }

    x_dbg_stmt(", nfy_tag = %d\n\r", *(UINT32*)pv_nfy_tag);
    
    if ((ui4_nfy_reason & SB_REASON_PROGRESS) ||
        (ui4_nfy_reason & SB_REASON_COMPLETE))
    {
        if (t_atsc_scan_data.e_scan_type != SB_ATSC_SCAN_TYPE_QUICK_MODE)
        {
            aui1_scan_result[ui4_cur_scan_idx++] = (UINT8)(i4_nfy_data);
        }            
        ui4_ch_num_found += (i4_nfy_data & 0xFFFF);  
        
        x_dbg_stmt(_SB_INFO"scan pogress = %d (total: %d) channels found!\r\n",
                   (i4_nfy_data & 0xFFFF), ui4_ch_num_found);
    }
}


/*-----------------------------------------------------------------------------
 * Name: _sb_cli_dump_svl
 *
 * Description: This API dumps the SVL.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _sb_cli_dump_svl(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_return;
    HANDLE_T    h_svl = NULL_HANDLE;
    UINT32      ui4_ver_id = SVL_NULL_VER_ID;
    UINT16      ui2_idx = 0;
    UINT16      ui2_svl_idx = 0;
    UINT16      ui2_up_limit = 2;
    SVL_REC_T   t_svl_rec;
    BOOL        b_detail = FALSE;

    /* sb.ds */

    /* Check arguments */
    if ((i4_argc > 2) && (pps_argv != NULL))
	{
        if (!x_strcmp(pps_argv[1], "help")  ||
            !x_strcmp(pps_argv[1], "h")     ||
            !x_strcmp(pps_argv[1], "?")     ||
            !x_strcmp(pps_argv[1], "-h"))
        {
            x_dbg_stmt(_SB_INFO"sb.ds [SVL no.] [-d|--detail]\n\r");
            return CLIR_INV_CMD_USAGE;
        }
    }
    
    if ((i4_argc > 1) && (pps_argv != NULL))
    {
        ui2_svl_idx = (UINT16)x_strtoll(pps_argv[1], NULL, 10);
        if (ui2_svl_idx > 0)
        {
            ui2_up_limit = ui2_svl_idx;
            ui2_svl_idx--;
        }
        else if (!x_strcmp(pps_argv[1], "-d")  ||
                 !x_strcmp(pps_argv[1], "--detail"))
        {
            b_detail = TRUE;
        }
        else
        {
            x_dbg_stmt(_SB_INFO"sb.ds [SVL no.] [-d|--detail]\n\r");
            return CLIR_INV_CMD_USAGE;
        }

        if ((i4_argc > 2) && (pps_argv != NULL))
        {
            if (!x_strcmp(pps_argv[2], "-d")  ||
                !x_strcmp(pps_argv[2], "--detail"))
            {
                b_detail = TRUE;
            }
        }
    }

    for (; ui2_svl_idx < ui2_up_limit; ui2_svl_idx++)
    {
        x_dbg_stmt(_SB_INFO"Opening SVL %u...\n", SVL_ID + ui2_svl_idx);
        i4_return = x_svl_open(SVL_ID + ui2_svl_idx, NULL, NULL, &h_svl);
    
        if (i4_return == SVLR_OK)
        {
            ui2_idx = 0;
            do
            {
                i4_return = x_svl_get_rec_by_brdcst_type(h_svl,
                                                         BRDCST_TYPE_UNKNOWN,
                                                         0xffffffff,
                                                         ui2_idx,
                                                         &t_svl_rec,
                                                         &ui4_ver_id);
                if (i4_return == SVLR_OK)
                {
                    x_dbg_stmt(_SB_INFO"%s%3d%s%2d [idx%2d] [%7s] [rec id%3d] [RF%3d] [prg_id 0x%04x] [svc_type 0x%x] [brdcst_type 0x%x][mask 0x%08X][opt mask 0x%08X]\n\r",
                               (t_svl_rec.uheader.t_rec_hdr.ui2_tsl_rec_id & 0x8000)?
                                    "A": ((t_svl_rec.uheader.t_rec_hdr.ui2_tsl_rec_id & 0x4000)? "X":"D"),
                               SB_ATSC_GET_MAJOR_CHANNEL_NUM(t_svl_rec.uheader.t_rec_hdr.ui4_channel_id),
                               SB_ATSC_IS_ONE_PART_CHANNEL_ID(t_svl_rec.uheader.t_rec_hdr.ui4_channel_id)? " ":"-",
                               (t_svl_rec.uheader.t_rec_hdr.e_brdcst_type == BRDCST_TYPE_DVB) ? 
                                 SB_DVB_GET_LCN(t_svl_rec.uheader.t_rec_hdr.ui4_channel_id) :
                               SB_ATSC_GET_MINOR_CHANNEL_NUM(t_svl_rec.uheader.t_rec_hdr.ui4_channel_id),
                               SB_ATSC_GET_CHANNEL_INDEX(t_svl_rec.uheader.t_rec_hdr.ui4_channel_id),
                               t_svl_rec.uheader.t_rec_hdr.ac_name,
                               t_svl_rec.ui2_svl_rec_id,
                               t_svl_rec.uheader.t_rec_hdr.ui2_tsl_rec_id & 0x3fff,
                               t_svl_rec.uheader.t_rec_hdr.ui2_prog_id,
                               t_svl_rec.uheader.t_rec_hdr.e_serv_type,
                               t_svl_rec.uheader.t_rec_hdr.e_brdcst_type,
                               t_svl_rec.uheader.t_rec_hdr.ui4_nw_mask,
                               t_svl_rec.uheader.t_rec_hdr.ui4_option_mask);
                    if (b_detail)
                    {
                        x_dbg_stmt(_SB_INFO"[source id %d]\n\r",
                                   t_svl_rec.u_data.t_atsc.ui2_src_id);
                    }
                }
                else if (i4_return == SVLR_MODIFIED)
                {
                    ui2_idx = 0;
                    ui4_ver_id = SVL_NULL_VER_ID;
                    x_dbg_stmt(_SB_INFO"SVL has been modified during the dumping process.  Re-dumping...\n\n\r");
                    continue;
                }
                
                ui2_idx++;
            } while (i4_return == SVLR_OK);
    
            x_svl_close(h_svl);
            h_svl = NULL_HANDLE;
        }
        else
        {
            x_dbg_stmt(_SB_INFO"Failed to open SVL %u. [return %d]\n\r",
                       SVL_ID + ui2_svl_idx,
                       i4_return);
        }
        x_dbg_stmt("\n");
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _sb_cli_dump_tsl
 *
 * Description: This API dumps the SVL.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _sb_cli_dump_tsl(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_return;
    HANDLE_T    h_tsl = NULL_HANDLE;
    UINT32      ui4_ver_id = TSL_NULL_VER_ID;
    UINT16      ui2_idx = 0;
    UINT16      ui2_tsl_idx = 0;
    UINT16      ui2_up_limit = 2;
    UINT16      ui2_tsl_type_idx = 0;
    UINT16      ui2_type_up_limit = 3;
    TSL_REC_T   t_tsl_rec;
    BOOL        b_detail = FALSE;

    /* sb.dt */

    /* Check arguments */
	if ((i4_argc > 2) && (pps_argv != NULL))
	{
        if (!x_strcmp(pps_argv[1], "help")  ||
            !x_strcmp(pps_argv[1], "h")     ||
            !x_strcmp(pps_argv[1], "?")     ||
            !x_strcmp(pps_argv[1], "-h"))
        {
            x_dbg_stmt(_SB_INFO"sb.dt [TSL no.] [-d|--detail]\n\r");
            return CLIR_INV_CMD_USAGE;
        }
    }
    
    if ((i4_argc > 1) && (pps_argv != NULL))
    {
        ui2_tsl_idx = (UINT16)x_strtoll(pps_argv[1], NULL, 10);
        if (ui2_tsl_idx > 0)
        {
            ui2_up_limit = ui2_tsl_idx;
            ui2_tsl_idx--;
        }
        else if (!x_strcmp(pps_argv[1], "-d")  ||
                 !x_strcmp(pps_argv[1], "--detail"))
        {
            b_detail = TRUE;
        }
        else
        {
            x_dbg_stmt(_SB_INFO"sb.dt [TSL no.] [-d|--detail]\n\r");
            return CLIR_INV_CMD_USAGE;
        }

        if ((i4_argc > 2) && (pps_argv != NULL))
        {
            if (!x_strcmp(pps_argv[2], "-d")  ||
                !x_strcmp(pps_argv[2], "--detail"))
            {
                b_detail = TRUE;
            }
        }
    }

    for (; ui2_tsl_idx < ui2_up_limit; ui2_tsl_idx++)
    {
        x_dbg_stmt(_SB_INFO"Opening TSL %u...\n", TSL_ID + ui2_tsl_idx);

        i4_return = x_tsl_open(TSL_ID + ui2_tsl_idx, NULL, NULL, &h_tsl);
        if (i4_return == TSLR_OK)
        {
            for (ui2_tsl_type_idx = 0; ui2_tsl_type_idx < ui2_type_up_limit; ui2_tsl_type_idx++)
            {
                for (ui2_idx = 2; ui2_idx <= 512; ui2_idx++)
                {
                    UINT16 ui2_tsl_rec_id = 0;

                    if (ui2_tsl_type_idx == 0)
                    {
                        ui2_tsl_rec_id = SB_MAKE_DIG_TSL_REC_ID(ui2_idx);
                    }
                    else if (ui2_tsl_type_idx == 1)
                    {
                        ui2_tsl_rec_id = SB_MAKE_AUX_DIG_TSL_REC_ID(ui2_idx);
                    }
                    else if (ui2_tsl_type_idx == 2)
                    {
                        ui2_tsl_rec_id = SB_MAKE_ANA_TSL_REC_ID(ui2_idx);
                    }

                    ui4_ver_id = TSL_NULL_VER_ID;
                    i4_return = x_tsl_get_rec(h_tsl,
                                              ui2_tsl_rec_id,
                                              &t_tsl_rec,
                                              &ui4_ver_id);
                    if (i4_return == TSLR_REC_NOT_FOUND)
                    {
                        continue;
                    }
                    else if (i4_return != TSLR_OK)
                    {
                        x_dbg_stmt(_SB_INFO"Failed to get the rec [%d] of TSL. [return %d]\n\r", 
                                   ui2_idx, i4_return);
                        if (i4_return == TSLR_INV_HANDLE)
                        {
                            break;
                        }
                        else
                        {
                            continue;
                        }
                    }
        
                    x_dbg_stmt(_SB_INFO"[RF%3u/rec id 0x%04X], TS_ID = [0x%04X], ON_ID = [0x%04X], Freq = [%lu][Brdcst type 0x%x][medium 0x%x]\n\r",
                               (t_tsl_rec.ui2_tsl_rec_id & 0x3fff),
                               t_tsl_rec.ui2_tsl_rec_id,
                               t_tsl_rec.uheader.t_desc.ui2_ts_id,
                               t_tsl_rec.uheader.t_desc.ui2_on_id,
                               t_tsl_rec.udata.t_ter_dig.ui4_freq,
                               t_tsl_rec.uheader.t_desc.e_bcst_type,
                               t_tsl_rec.uheader.t_desc.e_bcst_medium);
                    if (b_detail)
                    {
                        CHAR s_mod[16];
    
                        switch (t_tsl_rec.uheader.t_desc.e_bcst_medium)
                        {
                        case BRDCST_MEDIUM_DIG_CABLE:
                            if (t_tsl_rec.udata.t_cab_dig.e_mod == MOD_QAM_64)
                            {
                                x_sprintf(s_mod, "QAM 64");
                            }
                            else if (t_tsl_rec.udata.t_cab_dig.e_mod == MOD_QAM_256)
                            {
                                x_sprintf(s_mod, "QAM 256");
                            }
                            else
                            {
                                x_sprintf(s_mod, "Mod 0x08X",
                                          t_tsl_rec.udata.t_cab_dig.e_mod);
                            }
    
                            x_dbg_stmt(_SB_INFO"Modulation [%s] Sym Rate [%u] Inner\n\r",
                                       s_mod,
                                       t_tsl_rec.udata.t_cab_dig.ui4_sym_rate,
                                       t_tsl_rec.udata.t_cab_dig.e_fec_inner);
                            break;
    
                        default:
                            break;
                        }
                    }
                }
            }
    
            x_tsl_close(h_tsl);
            h_tsl = NULL_HANDLE;
        }
        else
        {
            x_dbg_stmt(_SB_INFO"Failed to open TSL %u. [return %d]\n\r",
                       TSL_ID + ui2_tsl_idx,
                       i4_return);
        }
        x_dbg_stmt("\n");
    }

    return CLIR_OK;
}

#ifndef NO_NWL_INIT
/*-----------------------------------------------------------------------------
 * Name: _sb_cli_dump_nwl
 *
 * Description: This API dumps the NWL.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _sb_cli_dump_nwl(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_return;
    HANDLE_T    h_nwl = NULL_HANDLE;
    UINT32      ui4_ver_id = SVL_NULL_VER_ID;
    UINT16      ui2_idx = 0;
    UINT16      ui2_nwl_idx = 0;
    UINT16      ui2_up_limit = 2;
    NWL_REC_T   t_nwl_rec;
    BOOL        b_detail = FALSE;
    UINT16      ui2_num_rec = 0;

    /* sb.dn */

    /* Check arguments */
    if ((i4_argc > 2) && (pps_argv != NULL))
	{
        if (!x_strcmp(pps_argv[1], "help")  ||
            !x_strcmp(pps_argv[1], "h")     ||
            !x_strcmp(pps_argv[1], "?")     ||
            !x_strcmp(pps_argv[1], "-h"))
        {
            x_dbg_stmt(_SB_INFO"sb.dn [NWL no.] [-d|--detail]\n\r");
            return CLIR_INV_CMD_USAGE;
        }
    }
    
    if ((i4_argc > 1) && (pps_argv != NULL))
    {
        ui2_nwl_idx = x_strtoll(pps_argv[1], NULL, 10);
        if (ui2_nwl_idx > 0)
        {
            ui2_nwl_idx--;
            ui2_up_limit = ui2_nwl_idx + 1;
        }
        else if (!x_strcmp(pps_argv[1], "-d")  ||
                 !x_strcmp(pps_argv[1], "--detail"))
        {
            b_detail = TRUE;
        }
        else
        {
            x_dbg_stmt(_SB_INFO"sb.dn [SVL no.] [-d|--detail]\n\r");
            return CLIR_INV_CMD_USAGE;
        }

        if ((i4_argc > 2) && (pps_argv != NULL))
        {
            if (!x_strcmp(pps_argv[2], "-d")  ||
                !x_strcmp(pps_argv[2], "--detail"))
            {
                b_detail = TRUE;
            }
        }
    }

    for (; ui2_nwl_idx < ui2_up_limit; ui2_nwl_idx++)
    {
        x_dbg_stmt(_SB_INFO"Opening NWL %u...\n", NWL_ID + ui2_nwl_idx);
        i4_return = x_nwl_open(NWL_ID + ui2_nwl_idx, NULL, NULL, &h_nwl);
    
        if (i4_return == SVLR_OK)
        {
            if (x_nwl_get_num_rec(h_nwl,
                                   &ui2_num_rec,
                                   &ui4_ver_id) == NWLR_OK)
            {
                for (ui2_idx = 0; ui2_idx < ui2_num_rec; ui2_idx++)
                {
                    if (x_nwl_get_rec(h_nwl, 
                                    ui2_idx + 1,
                                    &t_nwl_rec,
                                    &ui4_ver_id) == NWLR_OK)
                    {  
                        x_dbg_stmt(_SB_INFO" NWL rec_id %2d, nw_id=0x%05x, nit_ver=%2d, net_name=%15s, mask=0x%08x\n\r",
                                   t_nwl_rec.ui2_nwl_rec_id,
                                   t_nwl_rec.ui2_nw_id,
                                   t_nwl_rec.ui1_nit_ver,
                                   t_nwl_rec.s_nw_name,
                                   t_nwl_rec.ui4_nw_mask);                        
                    }
                }
            }
            if (b_detail)
            {
                x_dbg_stmt(_SB_INFO"detail...\n\r");
            }            
            x_svl_close(h_nwl);
            h_nwl = NULL_HANDLE;
        }
        else
        {
            x_dbg_stmt(_SB_INFO"Failed to open NWL %u. [return %d]\n\r",
                       SVL_ID + ui2_nwl_idx,
                       i4_return);
        }
        x_dbg_stmt("\n");
    }

    return CLIR_OK;
}
#endif


/*-----------------------------------------------------------------------------
 * Name: _sb_cli_open_builder
 *
 * Description: This API opens an builder.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _sb_cli_open_builder(INT32        i4_argc, 
                                  const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT32      i;
    SB_COND_T   e_cond;

    /* sb.ob [eng name] [svl name] [file name] [use orig] */

    /* Check arguments */
	if ((i4_argc != 5) || 
	    (pps_argv == NULL))
	{
        x_dbg_stmt(_SB_INFO"sb.ob [eng name] [svl name] [file name] [use orig]\n\r");
        return CLIR_INV_CMD_USAGE;
    }
    
    if (x_strcmp("null", pps_argv[2]) == 0)
    {
        t_atsc_open_data.ps_svl_name = NULL;
    }
    else
    {
        x_strcpy(t_atsc_open_data.ps_svl_name, pps_argv[2]);
    }
    
    if (x_strcmp("null", pps_argv[3]) == 0)
    {
        t_atsc_open_data.ps_file_name = NULL;
    }    
    else
    {
        x_strcpy(t_atsc_open_data.ps_file_name, pps_argv[3]);
    }
            
    t_atsc_open_data.b_use_orig = x_strtoull(pps_argv[4], NULL, 0) ? TRUE : FALSE;
    
    x_strcpy(t_atsc_open_data.ps_tuner_name, SN_MAIN_TUNER_TER_DIG);

    for (i = 0; i < MAX_APP_REC; i++)
    {
        if (at_app_rec[i].h_builder == NULL_HANDLE)
        {
            break;
        }
    }
    
    i4_return = SBR_INTERNAL_ERROR;
    if (i < MAX_APP_REC)
    {
        i4_return = x_sb_open_builder(pps_argv[1],
                                      (VOID*)(&t_atsc_open_data),
                                      (VOID*)(&at_app_rec[i].ui4_tag),
                                      _sb_nfy_fct,
                                      &at_app_rec[i].h_builder,
                                      &e_cond);
                                      
        if (i4_return == SBR_OK)
        {
            ui4_app_count++;
        }                                      
    }
              
    if (i4_return == SBR_OK)
    {
        i4_return = CLIR_OK;
    }
    else
    {
        i4_return = CLIR_CMD_EXEC_ERROR;
    }
                      
    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: _sb_cli_close_builder
 *
 * Description: This API closes a builder.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _sb_cli_close_builder(INT32        i4_argc, 
                                   const CHAR** pps_argv)
{
    INT32   i4_return;
    UINT32  ui4_app;    

    /* sb.cb [app index] */

    /* Check arguments */
	if ((i4_argc != 2) || 
	    (pps_argv == NULL))
	{
        x_dbg_stmt(_SB_INFO"sb.ob [app index]\n\r");
        return CLIR_INV_CMD_USAGE;
    }
    
    ui4_app = (UINT32)(x_strtoull(pps_argv[1], NULL, 0));

    if (ui4_app >= ui4_app_count)
    {
        x_dbg_stmt(_SB_INFO"invalid application index!");
        return CLIR_INV_CMD_USAGE;
    }
        
    i4_return = x_sb_close_builder(at_app_rec[ui4_app].h_builder);
    
    if (i4_return == SBR_OK)
    {
        at_app_rec[ui4_app].h_builder = NULL_HANDLE;
        ui4_app_count--;
        i4_return = CLIR_OK;
    }
    else
    {
        i4_return = CLIR_CMD_EXEC_ERROR;
    }
                                  
    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: _sb_cli_start_scan
 *
 * Description: This API triggers a channel scan.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _sb_cli_start_scan(INT32        i4_argc, 
                                const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT8       ui1_idx;
    UINT32      ui4_app;
    SB_COND_T   e_cond; 
    BOOL        b_inv_arg = FALSE;      

    /* sb.ss [app index] [scan type] [start idx] [stop idx] */

    /* Check arguments */
	if ((i4_argc < 3) || 
	    (pps_argv == NULL))
	{
        x_dbg_stmt(_SB_INFO"sb.ss [app index] [scan type] [start idx] [stop idx] \n\r");
        x_dbg_stmt(_SB_INFO"  [scan type]: full/quick/single/range\n\r");
        return CLIR_INV_CMD_USAGE;
    }
    
    ui4_scan_start_idx = MIN_SCAN_IDX;
    ui4_scan_stop_idx = MAX_SCAN_IDX;
    ui4_app = (UINT32)(x_strtoull(pps_argv[1], NULL, 0));   

    if (ui4_app >= ui4_app_count)
    {
        x_dbg_stmt(_SB_INFO"invalid application index!");
        return CLIR_INV_CMD_USAGE;
    }
        
    if (x_strncmp("full", pps_argv[2], sizeof(pps_argv[2])) == 0)
    {
        if (i4_argc > 3)
        {
            ui4_scan_start_idx = (UINT32)(x_strtoull(pps_argv[3], NULL, 0));
        }
        
        t_atsc_scan_data.e_scan_type = SB_ATSC_SCAN_TYPE_FULL_MODE; 
    }
    else if (x_strncmp("quick", pps_argv[2], sizeof(pps_argv[2])) == 0)
    {
        if (i4_argc > 3)
        {
            ui4_scan_start_idx = (UINT32)(x_strtoull(pps_argv[3], NULL, 0));
        }
                
        t_atsc_scan_data.e_scan_type = SB_ATSC_SCAN_TYPE_QUICK_MODE; 
    }
    else if (x_strncmp("single", pps_argv[2], sizeof(pps_argv[2])) == 0)
    {
        if (i4_argc < 4)
        {
            b_inv_arg = TRUE;
        }
        else
        {
            ui4_scan_start_idx = ui4_scan_stop_idx = (UINT32)(x_strtoull(pps_argv[3], NULL, 0));
            t_atsc_scan_data.e_scan_type = SB_ATSC_SCAN_TYPE_SINGLE_RF_CHANNEL; 
        }            
    }    
    else if (x_strncmp("range", pps_argv[2], sizeof(pps_argv[2])) == 0)
    {
        if (i4_argc < 5)
        {
            b_inv_arg = TRUE;
        }
        else
        {
            ui4_scan_start_idx = (UINT32)(x_strtoull(pps_argv[3], NULL, 0));
            ui4_scan_stop_idx = (UINT32)(x_strtoull(pps_argv[4], NULL, 0));
            t_atsc_scan_data.e_scan_type = SB_ATSC_SCAN_TYPE_RANGE_RF_CHANNEL; 
        }            
    }
    else
    {
        x_dbg_stmt(_SB_INFO"invalid scan type!");
        return CLIR_OK;
    }    
    
    if (b_inv_arg)
    {
        x_dbg_stmt(_SB_INFO"sb.ss [app index] [scan type] [start idx] [stop idx] \n\r");
        x_dbg_stmt(_SB_INFO"  [scan type]: full/quick/single/range\n\r");
        return CLIR_INV_CMD_USAGE;
    }
    else
    {    
        t_atsc_scan_data.ui4_scan_info = (ui4_scan_start_idx << 16) |
                                          ui4_scan_stop_idx; 
        
        ui4_ch_num_found = 0;
        
        ui4_cur_scan_idx = ui4_scan_start_idx;
        
        for (ui1_idx = 0; ui1_idx < MAX_SCAN_IDX; ui1_idx++)
        {
            aui1_scan_result[ui1_idx] = 0;
        }
            
        i4_return = x_sb_start_scan(at_app_rec[ui4_app].h_builder,
                                    &t_atsc_scan_data,
                                    (VOID*)(&at_app_rec[ui4_app].ui4_tag),
                                    _sb_scan_nfy_fct,
                                    &e_cond);
    }                                    

    if (i4_return == SBR_OK)
    {
        i4_return = CLIR_OK;
    }
    else
    {
        i4_return = CLIR_CMD_EXEC_ERROR;
    }
    
    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: _sb_cli_cancel_scan
 *
 * Description: This API cancels a channel scan.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _sb_cli_cancel_scan(INT32        i4_argc, 
                                 const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT32      ui4_app;

    /* sb.cs [app index] */

    /* Check arguments */
	if ((i4_argc != 2) || 
	    (pps_argv == NULL))
	{
        x_dbg_stmt(_SB_INFO"sb.cs [app index]\n\r");
        return CLIR_INV_CMD_USAGE;
    }
    
    ui4_app = (UINT32)(x_strtoull(pps_argv[1], NULL, 0));   

    if (ui4_app >= ui4_app_count)
    {
        x_dbg_stmt(_SB_INFO"invalid application index!");
        return CLIR_INV_CMD_USAGE;
    }
    
    i4_return = x_sb_cancel_scan(at_app_rec[ui4_app].h_builder);

    if (i4_return == SBR_OK)
    {
        i4_return = CLIR_OK;
    }
    else
    {
        i4_return = CLIR_CMD_EXEC_ERROR;
    }
    
    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: _sb_cli_save_lists
 *
 * Description: This API saves TSL and SVL records into NVRAM.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _sb_cli_save_lists(INT32        i4_argc, 
                                const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT32      ui4_app;

    /* sb.sl [app index] [file name] */

    /* Check arguments */
	if ((i4_argc != 3) || 
	    (pps_argv == NULL))
	{
        x_dbg_stmt(_SB_INFO"sb.cs [app index] [file name]\n\r");
        return CLIR_INV_CMD_USAGE;
    }
    
    ui4_app = (UINT32)(x_strtoull(pps_argv[1], NULL, 0));  

    if (ui4_app >= ui4_app_count)
    {
        x_dbg_stmt(_SB_INFO"invalid application index!");
        return CLIR_INV_CMD_USAGE;
    }
    
    i4_return = x_sb_save_lists(at_app_rec[ui4_app].h_builder,
                                pps_argv[2]);

    if (i4_return == SBR_OK)
    {
        i4_return = CLIR_OK;
    }
    else
    {
        i4_return = CLIR_CMD_EXEC_ERROR;
    }
    
    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: _sb_cli_load_lists
 *
 * Description: This API loads TSL and SVL records from NVRAM.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _sb_cli_load_lists(INT32        i4_argc, 
                                const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT32      ui4_app;

    /* sb.ll [app index] [file name] */

    /* Check arguments */
	if ((i4_argc != 3) || 
	    (pps_argv == NULL))
	{
        x_dbg_stmt(_SB_INFO"sb.ll [app index] [file name]\n\r");
        return CLIR_INV_CMD_USAGE;
    }
    
    ui4_app = (UINT32)(x_strtoull(pps_argv[1], NULL, 0));

    if (ui4_app >= ui4_app_count)
    {
        x_dbg_stmt(_SB_INFO"invalid application index!");
        return CLIR_INV_CMD_USAGE;
    }
    
    i4_return = x_sb_load_lists(at_app_rec[ui4_app].h_builder,
                                pps_argv[2]);

    if (i4_return == SBR_OK)
    {
        i4_return = CLIR_OK;
    }
    else
    {
        i4_return = CLIR_CMD_EXEC_ERROR;
    }
    
    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: _sb_cli_clean_lists
 *
 * Description: This API cleans TSL and SVL records from NVRAM.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _sb_cli_clean_lists(INT32        i4_argc, 
                                 const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT32      ui4_app;

    /* sb.cl [app index] [file name] */

    /* Check arguments */
	if ((i4_argc != 3) || 
	    (pps_argv == NULL))
	{
        x_dbg_stmt(_SB_INFO"sb.cl [app index] [file name]\n\r");
        return CLIR_INV_CMD_USAGE;
    }
    
    ui4_app = (UINT32)(x_strtoull(pps_argv[1], NULL, 0));

    if (ui4_app >= ui4_app_count)
    {
        x_dbg_stmt(_SB_INFO"invalid application index!");
        return CLIR_INV_CMD_USAGE;
    }
    
    i4_return = x_sb_clean_lists(at_app_rec[ui4_app].h_builder,
                                 pps_argv[2]);

    if (i4_return == SBR_OK)
    {
        i4_return = CLIR_OK;
    }
    else
    {
        i4_return = CLIR_CMD_EXEC_ERROR;
    }
    
    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: _sb_cli_get_cond
 *
 * Description: This API gets current condition of a builder.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _sb_cli_get_cond(INT32        i4_argc, 
                              const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT32      ui4_app;
    SB_COND_T   e_cond;       

    /* sb.gc [app index] */

    /* Check arguments */
	if ((i4_argc != 2) || 
	    (pps_argv == NULL))
	{
        x_dbg_stmt(_SB_INFO"sb.gc [app index]\n\r");
        return CLIR_INV_CMD_USAGE;
    }
    
    ui4_app = (UINT32)(x_strtoull(pps_argv[1], NULL, 0));
        
    i4_return = x_sb_get_cond(at_app_rec[ui4_app].h_builder,
                              &e_cond);
                              
    if (i4_return == SBR_OK)
    {
        switch (e_cond)
        {
            case SB_COND_AVAIL:
                x_dbg_stmt(_SB_INFO"available\n\r");
                break;
            
            case SB_COND_UNCONFIG:
                x_dbg_stmt(_SB_INFO"unconfigured\n\r");
                break;
                
            case SB_COND_INITING:
                x_dbg_stmt(_SB_INFO"initializing\n\r");
                break;
                
            case SB_COND_BUILDING:
                x_dbg_stmt(_SB_INFO"building\n\r");
                break;
                
            case SB_COND_UPDATE:
                x_dbg_stmt(_SB_INFO"update\n\r");
                break;
                
            case SB_COND_CLOSED:
                x_dbg_stmt(_SB_INFO"closed\n\r");
                break;
                
            default:
                break;                
        }
    }

    if (i4_return == SBR_OK)
    {
        i4_return = CLIR_OK;
    }
    else
    {
        i4_return = CLIR_CMD_EXEC_ERROR;
    }
        
    return i4_return;
}

/*-----------------------------------------------------------------------------
 * Name: _sb_cli_is_inited
 *
 * Description: This API checks if SVL Builder is initialized.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _sb_cli_is_inited(INT32        i4_argc, 
                               const CHAR** pps_argv)
{
    /* sb.ii */

    /* Check arguments */
	if (i4_argc != 1)
	{
		x_dbg_stmt(_SB_INFO"sb.ii\n\r");
		return CLIR_INV_CMD_USAGE;
	}    
	     
    x_dbg_stmt(_SB_INFO"SVL Builder %s been initialized\n\r", 
               x_sb_is_inited() ? "has" : "has not");
               
    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _sb_cli_list_app_rec
 *
 * Description: This API lists applications that are using SVL Builder.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _sb_cli_list_app_rec(INT32        i4_argc, 
                                  const CHAR** pps_argv)
{
    UINT32  i;
    UINT32  ui4_app_found = 0;

    /* sb.la */
      
    /* Check arguments */
	if (i4_argc != 1)
	{
		x_dbg_stmt(_SB_INFO"sb.la\n\r");
		return CLIR_INV_CMD_USAGE;
	}    
	    
    x_dbg_stmt(_SB_INFO"total application count: %d\n\r", ui4_app_count);
    for (i = 0; i < MAX_APP_REC; i++)
    {
        if (at_app_rec[i].h_builder != NULL_HANDLE)
        {
            x_dbg_stmt(_SB_INFO"[%02d] handle = 0x%08x, nfy_tag = %d\n\r",
                       ui4_app_found++,  
                       at_app_rec[i].h_builder,
                       at_app_rec[i].ui4_tag);
        }           
          
        if (ui4_app_found == ui4_app_count)
        {
            break;
        }
    }

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _sb_cli_list_scan_rslt
 *
 * Description: This API lists channel scan result.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _sb_cli_list_scan_rslt(INT32        i4_argc, 
                                    const CHAR** pps_argv)
{
    UINT32  i;

    /* sb.ls */ 
    
    /* Check arguments */
	if (i4_argc != 1)
	{
		x_dbg_stmt(_SB_INFO"sb.ls\n\r");
		return CLIR_INV_CMD_USAGE;
	}    
	
    x_dbg_stmt(_SB_INFO"total channels found: %d\n\r", ui4_ch_num_found);
    if (t_atsc_scan_data.e_scan_type != SB_ATSC_SCAN_TYPE_QUICK_MODE)
    {
        x_dbg_stmt(_SB_INFO"index\tNum of ch\n\r");
        for (i = ui4_scan_start_idx; i <= ui4_scan_stop_idx; i++)
        {
            x_dbg_stmt(_SB_INFO"%02d\t%d\n\r", i, aui1_scan_result[i]);
        }
    }

    return SBR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _sb_cli_get_dbg_level
 *
 * Description: This API gets the current setting of SVL Builder debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _sb_cli_get_dbg_level(INT32        i4_argc, 
                                   const CHAR** pps_argv)
{
    INT32       i4_return;
    
    /* sb.gdl */
    
    /* Check arguments */
	if (i4_argc != 1)
	{
		x_dbg_stmt(_SB_INFO"sb.gdl\n\r");
		return CLIR_INV_CMD_USAGE;
	}    
	    
    i4_return = x_cli_show_dbg_level(x_sb_get_dbg_level());

    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: _sb_cli_set_dbg_level
 *
 * Description: This API sets the debug level of SVL Builder.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _sb_cli_set_dbg_level(INT32        i4_argc, 
                                   const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;     
    
    /* sb.sdl [level] */
    
    /* Check arguments */
	if ((i4_argc != 2) || 
	    (pps_argv == NULL))
	{
		x_dbg_stmt(_SB_INFO"sb.sdl [level]\n\r");
		x_dbg_stmt(_SB_INFO"  [level]: n=none/e=error, a=api, i=info");
		return CLIR_INV_CMD_USAGE;
	}    
	    
    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);
    
    if (i4_return == CLIR_OK)
    {
        if (x_sb_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_return = CLIR_CMD_EXEC_ERROR;  
        }
        else
        {
            i4_return = x_cli_show_dbg_level(x_sb_get_dbg_level());
        }
    }
    else
    {
		x_dbg_stmt(_SB_INFO"sb.sdl [level]\n\r");
		x_dbg_stmt(_SB_INFO"  [level]: n=none/e=error, a=api, i=info");
		i4_return = CLIR_INV_CMD_USAGE;
    }

    return i4_return;
}
#endif  /* CLI_LVL_ALL */


/*-----------------------------------------------------------------------------
 * Name: sb_cli_init
 *
 * Description: This API initializes SVL Builder CLI component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: SBR_OK                     Routine successful.
 ----------------------------------------------------------------------------*/
INT32 sb_cli_init(VOID)
{
#ifdef CLI_LVL_ALL
    INT32   i4_return;
    UINT32  i;
    
    ui4_app_count = 0;
    for (i = 0; i < MAX_APP_REC; i++)
    {
        at_app_rec[i].ui4_tag = i;
        at_app_rec[i].h_builder = NULL_HANDLE;
    }

    /* Attach SVL Builder CLI command table to CLI */
    i4_return = x_cli_attach_cmd_tbl(at_sb_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_NONE);
    
    if (i4_return != CLIR_OK)
    {
        return SBR_INTERNAL_ERROR;
    }
#endif   

    return SBR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sb_cli_reg_sub_table
 *
 * Description: This API hooks a CLI command table into SVL Builder CLI 
 *              component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: SBR_OK              Routine successful.
 *          SBR_INV_ARG         Invalid arguments.
 *          SBR_OUT_OF_MEM      Buffer is not enough.    
 ----------------------------------------------------------------------------*/
INT32 sb_cli_reg_sub_table(const CLI_EXEC_T* pt_exec)
{
#ifdef CLI_LVL_ALL    
    if (!pt_exec)
    {
        return SBR_INV_ARG;
    }

    if (z_last_entry >= z_cmd_tbl)
    {
        return SBR_OUT_OF_MEM;
    }

    at_sb_cli_tbl[z_last_entry++] = *pt_exec;
#endif
     
    return SBR_OK;
}

