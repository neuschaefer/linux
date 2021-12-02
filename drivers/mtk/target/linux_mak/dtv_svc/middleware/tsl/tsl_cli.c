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
 * $RCSfile: tsl_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains functions and internal variables to support
 *         CLI control of the TSL module.
 *
 *---------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "dbg/x_dbg.h"
#include "cli/x_cli.h"
#include "tsl/tsl_dbg.h"
#include "tsl/u_tsl.h"
#include "os/inc/x_os.h"
#include "tsl/_tsl.h"
#include "svl/x_svl.h"

#ifdef CLI_LVL_ALL

/* ----------------------------------------------------------------------
  Typedefs   
------------------------------------------------------------------------- */

/* DateTime module  CLI definitions. */

static INT32 cli_get_dbg_level (INT32  i4_argc, const CHAR**  pps_argv);
static INT32 cli_set_dbg_level (INT32  i4_argc, const CHAR**  pps_argv);

static INT32 cli_tsl_info      (INT32  i4_argc, const CHAR** pps_argv);
static INT32 cli_tsl_iterate_rec(INT32 i4_argc, const CHAR** pps_argv);
static INT32 cli_tsl_freq_info(INT32  i4_argc,const CHAR** pps_argv);
static INT32 cli_tsl_update_rec(INT32  i4_argc,const CHAR** pps_argv);
static INT32 cli_tsl_delete_rec(INT32  i4_argc,const CHAR** pps_argv);

static INT32 cli_tsl_save_to_file(INT32  i4_argc, const CHAR** pps_argv);
static INT32 cli_tsl_sync_data_with_file(INT32 i4_argc,const CHAR** pps_argv);

static CLI_EXEC_T at_cli_dbg_cmd[] =
{
    {"gdl",  NULL, cli_get_dbg_level, NULL,
     CLI_GET_DBG_LVL_HELP_STR,  CLI_SUPERVISOR},
    {"sdl",  NULL, cli_set_dbg_level, NULL,
     CLI_SET_DBG_LVL_HELP_STR,  CLI_SUPERVISOR},
    {"info",  NULL, cli_tsl_info, NULL,
     "[info TSL_ID] info (print out record counts, record size, etc)", CLI_SUPERVISOR},
    {"dump",  NULL, cli_tsl_iterate_rec, NULL,
     "[dump TSL_ID] dump [print out TSL records information]", CLI_SUPERVISOR},
    {"freq_info",  NULL, cli_tsl_freq_info, NULL,
     "[freq_info tsl_id freq] (get service record info for specified freq)", CLI_SUPERVISOR},
    {"update_rec",  NULL, cli_tsl_update_rec, NULL,
     "[update_rec tsl_id tsl_rec_id new_nwl_id new_nwl_rec_id] (update record with new NW values)", CLI_SUPERVISOR},
    {"delete_rec",  NULL, cli_tsl_delete_rec, NULL,
     "[delete_rec tsl_id tsl_rec_id ] (delete a record from TSL object)", CLI_SUPERVISOR},
    {"save_to_file",  NULL, cli_tsl_save_to_file, NULL,
     "[save_to_file tsl_id dir_handle file_path] (save TSL object to filesystem.)", CLI_SUPERVISOR},
    {"sync_with_file",  NULL, cli_tsl_sync_data_with_file, NULL,
     "[sync_with_file TSL_ID DIR_HANDLE DIR_PATH] sync specified TSL object with data from filesystem.", CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

static CLI_EXEC_T at_cli_dbg_tbl [] =
{
    {"tsl", NULL, NULL, &(at_cli_dbg_cmd[0]), "TSL Command", CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};



/* --------------------------------------------------------------
  Function implementation.
---------------------------------------------------------------- */
/*-------------------------------------------------------------------------
 * Name: cli_get_dbg_level
 *
 * Description: This API gets the debug level for the TSL module.
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
        x_dbg_stmt("{TSL} tsl.gdl\n");
        i4_rc = CLIR_INV_CMD_USAGE;
    }
    else
    {
        i4_rc = x_cli_show_dbg_level(tsl_get_dbg_level());
    }
    
    return i4_rc;    
        
}

/*-------------------------------------------------------------------------
 * Name: cli_set_dbg_level
 *
 * Description: This API sets the debug level for the TSL module.
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
        tsl_set_dbg_level(ui2_dbg);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{TSL} tsl.sdl <n|e|a|i>\n");
        x_dbg_stmt("{TSL} where:  n=none, e=error, a=api, i=info. \n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;   
}

/*-------------------------------------------------------------------------
 * Name: cli_dump_tsl_id_and_name
 *
 * Description: This API prints out the name and ID of all available
 *    TSL objects.
 *
 * Inputs: 
 *    VOID.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_dump_tsl_id_and_name
(
    VOID
)
{
    INT32     i4_rc;
    
    UINT16    ui2_idx;
    UINT16    ui2_tsl_id;
    CHAR      s_tsl_name[MAX_TSL_NAME_LEN + 1];

    /* show the number of TSL ID available. */
    ui2_idx = 0;
    while (1)
    {
        if ( tsl_get_id_and_name_by_idx(ui2_idx, s_tsl_name, &ui2_tsl_id)
             == TSLR_OK )
        {
            if ( ui2_tsl_id != TSL_NULL_ID )
            {
                /* print out the name and id of TSL object. */
                x_dbg_stmt("(%d) TSL ID: %d  TSL_NAME: %s \n",
                           ui2_idx, ui2_tsl_id, s_tsl_name);
            }
        }
        else
        {
            break;
        }
        ui2_idx++;
    }

    i4_rc=CLIR_OK;
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_tsl_info
 *
 * Description: This API prints out the information about
 *    the specified TSL handle.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_tsl_info
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 2 )
    {
        UINT16   ui2_tsl_id;

        ui2_tsl_id = (HANDLE_T) x_strtoll(pps_argv[1],NULL, 10);
        
        tsl_print_info(ui2_tsl_id);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{TSL} info   <tsl_id>\n");
        x_dbg_stmt("{TSL} where: tsl_id specifies the TSL to dump records. \n");
        cli_dump_tsl_id_and_name();
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_tsl_iterate_rec
 *
 * Description: This API prints out the information about
 *    the specified TSL handle.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_tsl_iterate_rec
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 2 )
    {
        UINT16   ui2_tsl_id;

        ui2_tsl_id = (HANDLE_T) x_strtoll(pps_argv[1],NULL, 10);
        
        tsl_print_all_rec(ui2_tsl_id);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{TSL} info   <tsl_id>\n");
        x_dbg_stmt("{TSL} where: tsl_id specifies the TSL to dump records. \n");
        cli_dump_tsl_id_and_name();
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}



/*-------------------------------------------------------------------------
 * Name: cli_tsl_freq_info
 *
 * Description: This API prints out the service information about
 *    the specified freq
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
extern INT32   tsl_find_tsl_rec_id_by_freq
(
    UINT16             ui2_tsl_id,
    UINT32             ui4_freq,
    BRDCST_TYPE_T      e_brdcst_type,
    BRDCST_MEDIUM_T    e_brdcst_medium,
    UINT16*            pui2_tsl_rec_id
    
);

INT32   svl_find_svc_rec_by_tsl_info
(
    UINT16       ui2_svl_id,
    UINT16       ui2_tsl_id,
    UINT16       ui2_tsl_rec_id,
    SVL_REC_T*   pt_svl_rec
);


static INT32 cli_tsl_freq_info
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 3 )
    {
        UINT16             ui2_tsl_id;
        UINT16             ui2_svl_id;
        UINT32             ui4_freq;
        UINT16             ui2_tsl_rec_id;
        BRDCST_TYPE_T      e_brdcst_type;
        BRDCST_MEDIUM_T    e_brdcst_medium;
        SVL_REC_T          t_svl_rec;
        
        
        e_brdcst_type = BRDCST_TYPE_DVB;
        e_brdcst_medium = BRDCST_MEDIUM_DIG_TERRESTRIAL;
        
        ui2_tsl_id = (HANDLE_T) x_strtoll(pps_argv[1],NULL, 10);
        ui4_freq   = (UINT32)  x_strtoll(pps_argv[2],NULL, 10);

        tsl_find_tsl_rec_id_by_freq(ui2_tsl_id,
                                    ui4_freq,
                                    e_brdcst_type,
                                    e_brdcst_medium,
                                    &ui2_tsl_rec_id);

        ui2_svl_id=1;
        svl_find_svc_rec_by_tsl_info(ui2_svl_id,
                                     ui2_tsl_id,
                                     ui2_tsl_rec_id,
                                     &t_svl_rec);
        
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{TSL} freq_info   <tsl_id> <freq>\n");
        x_dbg_stmt("{TSL} where: tsl_id specifies the TSL, freq specifies freq (kh). \n");
        cli_dump_tsl_id_and_name();
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_tsl_update_rec
 *
 * Description: This API updates a specified TSL record with new NWL data.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
INT32 cli_tsl_update_rec
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 5 )
    {
        UINT16   ui2_tsl_id;
        UINT16   ui2_tsl_rec_id;
        UINT16   ui2_nwl_id;
        UINT16   ui2_nwl_rec_id;

        ui2_tsl_id = (HANDLE_T) x_strtoll(pps_argv[1],NULL, 10);
        ui2_tsl_rec_id = (HANDLE_T) x_strtoll(pps_argv[2],NULL, 10);
        ui2_nwl_id = (HANDLE_T) x_strtoll(pps_argv[3],NULL, 10);
        ui2_nwl_rec_id = (HANDLE_T) x_strtoll(pps_argv[4],NULL, 10);


        tsl_modify_rec_value(ui2_tsl_id,
                             ui2_tsl_rec_id,
                             ui2_nwl_id,
                             ui2_nwl_rec_id);
        
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{TSL} update_rec  <tsl_id tsl_rec_id new_nwl_id_value new_nwl_rec_id_value>\n");
        x_dbg_stmt("{TSL} where: tsl_id specifies the TSL object, and tsl_rec_id is the record to update.\n");
        cli_dump_tsl_id_and_name();
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_tsl_delete_rec
 *
 * Description: This API deletes a specified TSL record.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
INT32 cli_tsl_delete_rec
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 3 )
    {
        UINT16   ui2_tsl_id;
        UINT16   ui2_tsl_rec_id;
        
        ui2_tsl_id = (HANDLE_T) x_strtoll(pps_argv[1],NULL, 10);
        ui2_tsl_rec_id = (HANDLE_T) x_strtoll(pps_argv[2],NULL, 10);

        tsl_delete_rec(ui2_tsl_id,
                       ui2_tsl_rec_id);
        
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{TSL} update_rec  <tsl_id tsl_rec_id new_nwl_id_value new_nwl_rec_id_value>\n");
        x_dbg_stmt("{TSL} where: tsl_id specifies the TSL object, and tsl_rec_id is the record to update.\n");
        cli_dump_tsl_id_and_name();
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}


/*-------------------------------------------------------------------------
 * Name: cli_tsl_save_to_file
 *
 * Description: This API write out the TSL object to filesystem.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_tsl_save_to_file
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 4 )
    {
        UINT16      ui2_tsl_id;
        HANDLE_T    h_dir;
        
        ui2_tsl_id  = (UINT16) x_strtoll(pps_argv[1],NULL, 10);
        h_dir       = (HANDLE_T) x_strtoll(pps_argv[2],NULL, 10);
        
        tsl_print_info(ui2_tsl_id);
        x_tsl_fs_store(h_dir, pps_argv[3], ui2_tsl_id);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{TSL} save   tsl_id dir_handle dir_path\n");
        x_dbg_stmt("{TSL} where: tsl_id specifies the TSL object to save, dir_handle is handle ot directory, dir_path is a string specifying the directory under the directory handle. \n");
        x_dbg_stmt("{TSL} example: save 1 0 /dev/nor_2 \n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_tsl_sync_data_with_file
 *
 * Description: This API reads customer data from a specified TSL record.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_tsl_sync_data_with_file
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 4 )
    {
        UINT16                 ui2_tsl_id;
        HANDLE_T               h_tsl;
        HANDLE_T               h_dir;
        const CHAR*            s_file_path;
        
        ui2_tsl_id  = (UINT16) x_strtoll(pps_argv[1],NULL, 10);
        h_dir       = (HANDLE_T) x_strtoll(pps_argv[2],NULL, 10);
        s_file_path = pps_argv[3];
        

        if ( x_tsl_open(ui2_tsl_id, NULL, NULL, &h_tsl) == TSLR_OK )
        {
            if ( x_tsl_fs_sync(h_tsl, h_dir, s_file_path, ui2_tsl_id) == TSLR_OK )
            {
                x_dbg_stmt("{TSL} TSL (ID: %d) synced with data from file.\n",ui2_tsl_id);
            }
            else
            {
                x_dbg_stmt("{TSL} Error: can not sync with TSL data from file. \n");
            }
            x_tsl_close(h_tsl);
        }
        else
        {
            x_dbg_stmt("{TSL} Error: can not sync with TSL data from file. \n");
        }
            
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{TSL} sync_with_file tsl_id dir_handle dir_path\n");
        
        x_dbg_stmt("{TSL} where: tsl_id specifies the TSL object to sync, dir_handle is handle to a directory, dir_path is a string specifying the directory under the directory handle.\n");
        
        x_dbg_stmt("{TSL} example: sync_with_file 1 0 /dev/nor_2 \n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: tsl_cli_init
 *
 * Description: This API initializes and registers the CLI function with
 *    the Middleware
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
INT32 tsl_cli_init(VOID)
{
    /* Attach TSL CLI command table to CLI */
    if ( x_cli_attach_cmd_tbl(at_cli_dbg_tbl, CLI_CAT_MW, CLI_GRP_NONE)
         != CLIR_OK)
    {
        return TSLR_FAIL;
    }
    
    return TSLR_OK;   
}




#endif  /* CLI_LVL_ALL */
    

    
