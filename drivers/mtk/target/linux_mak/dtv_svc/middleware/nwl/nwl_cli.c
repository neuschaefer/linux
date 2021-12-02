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
 * $RCSfile: nwl_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains functions and internal variables to support
 *         CLI control of the NWL module.
 *
 *---------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "dbg/x_dbg.h"
#include "cli/x_cli.h"
#include "os/inc/x_os.h"

#include "nwl/_nwl.h"

#ifdef CLI_LVL_ALL

/* ----------------------------------------------------------------------
  Typedefs   
------------------------------------------------------------------------- */

/* DateTime module  CLI definitions. */

static INT32 cli_get_dbg_level (INT32  i4_argc, const CHAR**  pps_argv);
static INT32 cli_set_dbg_level (INT32  i4_argc, const CHAR**  pps_argv);
static INT32 cli_dump_nwl_rec  (INT32  i4_argc, const CHAR**  pps_argv);
static INT32 cli_nwl_info      (INT32  i4_argc, const CHAR** pps_argv);
static INT32 cli_nwl_save_to_file(INT32 i4_argc,const CHAR** pps_argv);
static INT32 cli_nwl_del_from_file(INT32 i4_argc, const CHAR** pps_argv);
static INT32 cli_nwl_sync_data_with_file(INT32 i4_argc, const CHAR** pps_argv);
static INT32 cli_nwl_create_test_data(INT32  i4_argc,const CHAR**  pps_argv);
static INT32 cli_nwl_delete_object(INT32  i4_argc,const CHAR**  pps_argv);

static CLI_EXEC_T at_cli_dbg_cmd[] =
{
    {"gdl",  NULL, cli_get_dbg_level, NULL,
     CLI_GET_DBG_LVL_HELP_STR,  CLI_SUPERVISOR},
    {"sdl",  NULL, cli_set_dbg_level, NULL,
     CLI_SET_DBG_LVL_HELP_STR,  CLI_SUPERVISOR},
    {"dump",  NULL, cli_dump_nwl_rec, NULL,
     "[dump NWL_ID] display records from a NWL handle", CLI_SUPERVISOR},
    {"info",  NULL, cli_nwl_info, NULL,
     "[info NWL_ID] info (record counts, record size, etc)", CLI_SUPERVISOR},
    {"save",  NULL, cli_nwl_save_to_file, NULL,
     "[save NWL_ID DIR_HANDLE DIR_PATH] write NWL object to filesystem.", CLI_SUPERVISOR},
    {"sync_with_file",  NULL, cli_nwl_sync_data_with_file, NULL,
     "[sync_with_file NWL_ID DIR_HANDLE DIR_PATH] sync specified NWL object with data from filesystem.", CLI_SUPERVISOR},
    {"delete",  NULL, cli_nwl_del_from_file, NULL,
     "[delete NWL_ID DIR_HANDLE DIR_PATH] delete NWL object from filesystem.", CLI_SUPERVISOR},
    {"test_data",  NULL, cli_nwl_create_test_data, NULL,
     "[test_data  NWL_ID] create NWL object in memory and fill it with test data.", CLI_SUPERVISOR},
    {"delete_obj",  NULL, cli_nwl_delete_object, NULL,
     "[delete_obj  NWL_ID] delete a NWL object in memory.", CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

static CLI_EXEC_T at_cli_dbg_tbl [] =
{
    {"nwl", NULL, NULL, &(at_cli_dbg_cmd[0]), "NWL Command", CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};



/* --------------------------------------------------------------
  Function implementation.
---------------------------------------------------------------- */
/*-------------------------------------------------------------------------
 * Name: cli_get_dbg_level
 *
 * Description: This API gets the debug level for the NWL module.
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
        x_dbg_stmt("{NWL} nwl.gdl\n");
        i4_rc = CLIR_INV_CMD_USAGE;
    }
    else
    {
        i4_rc = x_cli_show_dbg_level(nwl_get_dbg_level());
    }
    
    return i4_rc;    
        
}

/*-------------------------------------------------------------------------
 * Name: cli_set_dbg_level
 *
 * Description: This API sets the debug level for the NWL module.
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
        nwl_set_dbg_level(ui2_dbg);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{NWL} nwl.sdl <n|e|a|i>\n");
        x_dbg_stmt("{NWL} where:  n=none, e=error, a=api, i=info. \n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;   
}

/*-------------------------------------------------------------------------
 * Name: cli_dump_nwl_id_and_name
 *
 * Description: This API prints out the name and ID of all available
 *    NWL objects.
 *
 * Inputs: 
 *    VOID.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_dump_nwl_id_and_name
(
    VOID
)
{
    INT32     i4_rc;
    
    UINT16    ui2_idx;
    UINT16    ui2_nwl_id;
    CHAR      s_nwl_name[MAX_NWL_NAME_LEN + 1];

    /* show the number of NWL ID available. */
    ui2_idx = 0;
    while (1)
    {
        if ( nwl_get_id_and_name_by_idx(ui2_idx, s_nwl_name, &ui2_nwl_id)
             == NWLR_OK )
        {
            if ( ui2_nwl_id != NWL_NULL_ID )
            {
                /* print out the name and id of NWL object. */
                x_dbg_stmt("(%d) NWL ID: %d  NWL_NAME: %s \n",
                           ui2_idx, ui2_nwl_id, s_nwl_name);
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
 * Name: cli_dump_nwl_rec
 *
 * Description: This API prints out the NWL records for the specified
 *    NWL handle.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_dump_nwl_rec
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 2 )
    {
        UINT16   ui2_nwl_id;

        ui2_nwl_id = (HANDLE_T) x_strtoll(pps_argv[1],NULL, 10);
        
        nwl_dump_rec(ui2_nwl_id);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{NWL} dump   <nwl_id>\n");
        x_dbg_stmt("{NWL} where: nwl_id specifies the NWL to dump records. \n");
        cli_dump_nwl_id_and_name();
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}


/*-------------------------------------------------------------------------
 * Name: cli_nwl_info
 *
 * Description: This API prints out the information about
 *    the specified NWL handle.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_nwl_info
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 2 )
    {
        UINT16   ui2_nwl_id;

        ui2_nwl_id = (HANDLE_T) x_strtoll(pps_argv[1],NULL, 10);
        
        nwl_print_info(ui2_nwl_id);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{NWL} info   <nwl_id>\n");
        x_dbg_stmt("{NWL} where: nwl_id specifies the NWL to dump records. \n");
        cli_dump_nwl_id_and_name();
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_nwl_save_to_file
 *
 * Description: This API write out the NWL object to filesystem.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_nwl_save_to_file
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 4 )
    {
        UINT16      ui2_nwl_id;
        HANDLE_T    h_dir;
        
        ui2_nwl_id  = (UINT16) x_strtoll(pps_argv[1],NULL, 10);
        h_dir       = (HANDLE_T) x_strtoll(pps_argv[2],NULL, 10);
        
        nwl_print_info(ui2_nwl_id);
        x_nwl_fs_store(h_dir, pps_argv[3], ui2_nwl_id);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{NWL} save   nwl_id dir_handle dir_path\n");
        x_dbg_stmt("{NWL} where: nwl_id specifies the NWL object to save, dir_handle is handle ot directory, dir_path is a string specifying the directory under the directory handle. \n");
        x_dbg_stmt("{NWL} example: save 1 0 /dev/nor_2 \n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_nwl_del_from_file
 *
 * Description: This API deletes the NWL object to filesystem.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_nwl_del_from_file
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 4 )
    {
        UINT16      ui2_nwl_id;
        HANDLE_T    h_dir;

        ui2_nwl_id  = (UINT16) x_strtoll(pps_argv[1],NULL, 10);
        h_dir       = (HANDLE_T) x_strtoll(pps_argv[2],NULL, 10);
        x_nwl_fs_del(h_dir, pps_argv[3], ui2_nwl_id);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{NWL} delete  <nwl_id dir_handle dir_path\n");
        x_dbg_stmt("{NWL} where: nwl_id specifies the NWL object to delete, dir_handle is handle ot directory, dir_path is a string specifying the directory under the directory handle. \n");
        x_dbg_stmt("{NWL} example: delete 1 0 /dev/nor_2 \n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}


/*-------------------------------------------------------------------------
 * Name: cli_nwl_sync_data_with_file
 *
 * Description: This API reads customer data from a specified NWL record.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_nwl_sync_data_with_file
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 4 )
    {
        UINT16                 ui2_nwl_id;
        HANDLE_T               h_nwl;
        HANDLE_T               h_dir;
        
        ui2_nwl_id  = (UINT16) x_strtoll(pps_argv[1],NULL, 10);
        h_dir       = (HANDLE_T) x_strtoll(pps_argv[2],NULL, 10);

        if ( x_nwl_open(ui2_nwl_id, NULL, NULL, &h_nwl) == NWLR_OK )
        {
            if ( x_nwl_fs_sync(h_nwl, h_dir, pps_argv[3], ui2_nwl_id) == NWLR_OK )
            {
                x_dbg_stmt("{NWL} NWL (ID: %d) synced with data from file.\n",ui2_nwl_id);
            }
            else
            {
                x_dbg_stmt("{NWL} Error: can not sync with NWL data from file. \n");
            }
            x_nwl_close(h_nwl);
        }
        else
        {
            x_dbg_stmt("{NWL} Error: can not sync with NWL data from file. \n");
        }
            
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{NWL} sync_with_file nwl_id dir_handle dir_path\n");
        
        x_dbg_stmt("{NWL} where: nwl_id specifies the NWL object to sync, dir_handle is handle to a directory, dir_path is a string specifying the directory under the directory handle.\n");
        
        x_dbg_stmt("{NWL} example: sync_with_file 1 0 /dev/nor_2 \n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}



/*-------------------------------------------------------------------------
 * Name: cli_nwl_create_test_data
 *
 * Description: This API create a NWL and write test data to the object.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_nwl_create_test_data
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 2 )
    {
        UINT16   ui2_nwl_id;

        ui2_nwl_id = (HANDLE_T) x_strtoll(pps_argv[1],NULL, 10);
        nwl_create_test_data(ui2_nwl_id );
        
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{NWL} test_data   <nwl_id>\n");
        x_dbg_stmt("{NWL} where: nwl_id specifies the NWL to create. \n");
        cli_dump_nwl_id_and_name();
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_nwl_delete_object
 *
 * Description: This API deletes a NWL object.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_nwl_delete_object
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;
    HANDLE_T  h_nwl;
    
    if ( i4_argc == 2 )
    {
        UINT16   ui2_nwl_id;

        ui2_nwl_id = (HANDLE_T) x_strtoll(pps_argv[1],NULL, 10);

        if ( x_nwl_open(ui2_nwl_id, NULL, NULL, &h_nwl) == NWLR_OK )
        {
            i4_rc = x_nwl_delete(h_nwl);

            if ( i4_rc == NWLR_OK )
            {
                x_dbg_stmt("NWL object (%d) deleted.\n", ui2_nwl_id);
            }
            else
            {
                if ( i4_rc == NWLR_INV_HANDLE  )
                {
                    x_dbg_stmt("Invalid handle.\n");
                }
                else
                {
                    x_dbg_stmt("Error, can not delete NWL object (%d)\n",
                               ui2_nwl_id);
                }
            }
        }
        else
        {
            x_dbg_stmt("Can not open a handle to delete NWL object.\n");
        }
        
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{NWL} delete_nwl_obj   <nwl_id>\n");
        x_dbg_stmt("{NWL} where: nwl_id specifies the NWL to delete from the memory. \n");
        cli_dump_nwl_id_and_name();
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: nwl_cli_init
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
INT32 nwl_cli_init(VOID)
{
    /* Attach NWL CLI command table to CLI */
    if ( x_cli_attach_cmd_tbl(at_cli_dbg_tbl, CLI_CAT_MW, CLI_GRP_NONE)
         != CLIR_OK)
    {
        return NWLR_FAIL;
    }
    
    return NWLR_OK;   
}




#endif  /* CLI_LVL_ALL */
    

    
