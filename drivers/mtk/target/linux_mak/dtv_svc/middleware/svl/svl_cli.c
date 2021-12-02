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
 * $RCSfile: svl_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains functions and internal variables to support
 *         CLI control of the SVL module.
 *
 *---------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "dbg/x_dbg.h"
#include "cli/x_cli.h"
#include "svl/svl_dbg.h"
#include "svl/u_svl.h"
#include "os/inc/x_os.h"
#include "svl/_svl.h"
#include "util/fs_raw.h"
#include "inc/u_sys_name.h"
#include "svl/_svl.h"

#ifndef SN_CH_LIST_1
#define SN_CH_LIST_1 SN_FLASH_2     /* default partition for channel list 1 */
#endif

#ifndef SN_CH_LIST_2
#define SN_CH_LIST_2 SN_FLASH_4     /* default partition for channel list 2 */
#endif


#if defined(CLI_LVL_ALL) || defined(CLI_LVL_GUEST) || defined(CLI_STRESS_MODE)

#if !defined(CLI_LVL_ALL) && defined(CLI_LVL_GUEST) && !defined(CLI_STRESS_MODE)
#define CLI_STRESS_MODE
#endif
/* ----------------------------------------------------------------------
  Typedefs   
------------------------------------------------------------------------- */

/* DateTime module  CLI definitions. */

#ifndef CLI_STRESS_MODE
static INT32 cli_get_dbg_level (INT32  i4_argc, const CHAR**  pps_argv);
static INT32 cli_set_dbg_level (INT32  i4_argc, const CHAR**  pps_argv);
static INT32 cli_dump_svl_rec  (INT32  i4_argc, const CHAR**  pps_argv);
static INT32 cli_svl_info      (INT32  i4_argc, const CHAR** pps_argv);
static INT32 cli_svl_save_to_file(INT32 i4_argc,const CHAR** pps_argv);
static INT32 cli_svl_del_from_file(INT32 i4_argc, const CHAR** pps_argv);
#endif /* ! CLI_STRESS_MODE */
static INT32 cli_svl_filesys_info(INT32 i4_argc, const CHAR** pps_argv);
static INT32 cli_svl_dup_filesys(INT32 i4_argc, const CHAR** pps_argv);
static INT32 cli_svl_dup_filesys_auto(INT32 i4_argc, const CHAR** pps_argv);
#ifndef CLI_STRESS_MODE
static INT32 cli_svl_set_customer_data(INT32 i4_argc, const CHAR** pps_argv);
static INT32 cli_svl_get_customer_data(INT32 i4_argc, const CHAR** pps_argv);
static INT32 cli_svl_sync_data_with_file(INT32 i4_argc, const CHAR** pps_argv);
static INT32 cli_svl_reinit_fsys(INT32 i4_argc, const CHAR** pps_argv);
static INT32 cli_svl_dump_to_mem(INT32 i4_argc, const CHAR** pps_argv);
static INT32 cli_svl_load_from_mem(INT32 i4_argc, const CHAR** pps_argv);
static INT32 cli_svl_enable_log(INT32 i4_argc, const CHAR** pps_argv);
#endif /* ! CLI_STRESS_MODE */

static CLI_EXEC_T at_cli_dbg_cmd[] =
{
#ifndef CLI_STRESS_MODE
    {"gdl",  NULL, cli_get_dbg_level, NULL,
     CLI_GET_DBG_LVL_HELP_STR,  CLI_SUPERVISOR},
    {"sdl",  NULL, cli_set_dbg_level, NULL,
     CLI_SET_DBG_LVL_HELP_STR,  CLI_SUPERVISOR},
    {"dump",  NULL, cli_dump_svl_rec, NULL,
     "[dump SVL_ID] display records from a SVL handle", CLI_SUPERVISOR},
    {"info",  NULL, cli_svl_info, NULL,
     "[info SVL_ID] info (record counts, record size, etc)", CLI_SUPERVISOR},
    {"save",  NULL, cli_svl_save_to_file, NULL,
     "[save SVL_ID DIR_HANDLE DIR_PATH] write SVL object to filesystem.", CLI_SUPERVISOR},
    {"sync_with_file",  NULL, cli_svl_sync_data_with_file, NULL,
     "[sync_with_file SVL_ID DIR_HANDLE DIR_PATH] sync specifiedSVL object with data from filesystem.", CLI_SUPERVISOR},
    {"delete",  NULL, cli_svl_del_from_file, NULL,
     "[delete SVL_ID DIR_HANDLE DIR_PATH] delete SVL object from filesystem.", CLI_SUPERVISOR},
#endif /* ! CLI_STRESS_MODE */
    {"fsysinfo",  NULL, cli_svl_filesys_info, NULL,
     "[fsysinfo] print out FS_RAW filesystem.", CLI_GUEST},
    {"fsysdup",  NULL, cli_svl_dup_filesys, NULL,
     "[fsysdup] duplicate FS_RAW filesystem to another file.", CLI_GUEST},
    {"dup",  NULL, cli_svl_dup_filesys_auto, NULL,
     "[dup] auto duplicate FS_RAW filesystem to USB disk.", CLI_GUEST},
#ifndef CLI_STRESS_MODE
    {"set_customer_data", NULL, cli_svl_set_customer_data, NULL,
     "[set_customer_data SVL_ID SVL_REC_ID customer_data] write customer data to specified SVL record.", CLI_SUPERVISOR},
    {"get_customer_data", NULL, cli_svl_get_customer_data, NULL,
     "[get_customer_data SVL_ID SVL_REC_ID] read customer data from specified SVL record.", CLI_SUPERVISOR},
    {"memdump", NULL, cli_svl_dump_to_mem, NULL,
     "[memdump SVL_ID] dump SVL database to memory and print out memory address.", CLI_SUPERVISOR},
    {"memload", NULL, cli_svl_load_from_mem, NULL,
     "[memload SVL_ID mem_addr] load SVL database from memory.", CLI_SUPERVISOR},
    {"get_customer_data", NULL, cli_svl_get_customer_data, NULL,
     "[get_customer_data SVL_ID SVL_REC_ID] read customer data from specified SVL record.", CLI_SUPERVISOR},
    {"reinit_fs", NULL, cli_svl_reinit_fsys, NULL,
     "[reinit_fs DIR_HANDLE DIR_PATH] reinit (clean) the filesyst.", CLI_SUPERVISOR},
    {"log", NULL, cli_svl_enable_log, NULL,
     "log <API> <Rec dump> , enable/disable API log and dump log", CLI_SUPERVISOR},
#endif /* ! CLI_STRESS_MODE */
    END_OF_CLI_CMD_TBL
};

static CLI_EXEC_T at_cli_dbg_tbl [] =
{
    {"svl", NULL, NULL, &(at_cli_dbg_cmd[0]), "SVL Command", CLI_GUEST},
    END_OF_CLI_CMD_TBL
};



/* --------------------------------------------------------------
  Function implementation.
---------------------------------------------------------------- */

#ifndef CLI_STRESS_MODE

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
        x_dbg_stmt("{SVL} svl.gdl\n");
        i4_rc = CLIR_INV_CMD_USAGE;
    }
    else
    {
        i4_rc = x_cli_show_dbg_level(svl_get_dbg_level());
    }
    
    return i4_rc;    
        
}

/*-------------------------------------------------------------------------
 * Name: cli_set_dbg_level
 *
 * Description: This API sets the debug level of SVL module.
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
        svl_set_dbg_level(ui2_dbg);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{SVL} svl.sdl <n|e|a|i>\n");
        x_dbg_stmt("{SVL} where:  n=none, e=error, a=api, i=info. \n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;   
}



/*-------------------------------------------------------------------------
 * Name: cli_dump_svl_id_and_name
 *
 * Description: This API prints out the name and ID of all available
 *    SVL objects.
 *
 * Inputs: 
 *    VOID.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_dump_svl_id_and_name
(
    VOID
)
{
    INT32     i4_rc;
    
    UINT16    ui2_idx;
    UINT16    ui2_svl_id;
    CHAR      s_svl_name[MAX_SVL_NAME_LEN + 1];

    /* show the number of SVL ID available. */
    ui2_idx = 0;
    while (ui2_idx < svl_get_max_obj_num())
    {
        if ( svl_get_id_and_name_by_idx(ui2_idx, s_svl_name, &ui2_svl_id)
             == SVLR_OK )
        {
            if ( ui2_svl_id != SVL_NULL_ID )
            {
                /* print out the name and id of SVL object. */
                x_dbg_stmt("(%d) SVL ID: %d  SVL_NAME: %s \n",
                           ui2_idx, ui2_svl_id, s_svl_name);
            }
        }
        else
        {
            //break;
        }
        ui2_idx++;
    }

    i4_rc=CLIR_OK;
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_dump_svl_rec
 *
 * Description: This API prints out the SVL records for the specified
 *    SVL handle.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_dump_svl_rec
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;
    BRDCST_TYPE_T e_brdcst_type = BRDCST_TYPE_UNKNOWN;

    if ( (i4_argc == 2) || (i4_argc == 3))
    {
        UINT16   ui2_svl_id;

        ui2_svl_id = (HANDLE_T) x_strtoll(pps_argv[1],NULL, 10);

        if(i4_argc == 3)
        {
            if( x_strcasecmp(pps_argv[2], "a") == 0)
            {
                e_brdcst_type = BRDCST_TYPE_ANALOG;
            }
            else if( x_strcasecmp(pps_argv[2], "d") == 0)
            {
                e_brdcst_type = BRDCST_TYPE_DVB;
            }
            else
            {
                e_brdcst_type = BRDCST_TYPE_UNKNOWN;
            }
        }
        
        svl_dump_rec(ui2_svl_id, e_brdcst_type);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{SVL} dump   <svl_id> [brdcst_type]\n");
        x_dbg_stmt("{SVL} where: svl_id specifies the SVL to dump records. \n");
        x_dbg_stmt("{SVL} where: brdcst_type specifies broadcast type. \"a\" for ANALOG ; \"d\" for DVB \n");
        x_dbg_stmt("{SVL} eg: \n");
        x_dbg_stmt("{SVL} \"dump 1\"  dump all records in SVL 1\n");
        x_dbg_stmt("{SVL} \"dump 1 a\"  dump all ANALOG records in SVL 1\n");
        x_dbg_stmt("{SVL} \"dump 1 d\"  dump all DVB records in SVL 1\n");
        cli_dump_svl_id_and_name();
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_svl_info
 *
 * Description: This API prints out the information about
 *    the specified SVL handle.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_svl_info
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 2 )
    {
        UINT16   ui2_svl_id;

        ui2_svl_id = (HANDLE_T) x_strtoll(pps_argv[1],NULL, 10);
        
        svl_print_info(ui2_svl_id);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{SVL} info   <svl_id>\n");
        x_dbg_stmt("{SVL} where: svl_id specifies the SVL to dump records. \n");
        cli_dump_svl_id_and_name();
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_svl_save_to_file
 *
 * Description: This API write out the SVL object to filesystem.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_svl_save_to_file
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 4 )
    {
        UINT16      ui2_svl_id;
        HANDLE_T    h_dir;
        
        ui2_svl_id  = (UINT16) x_strtoll(pps_argv[1],NULL, 10);
        h_dir       = (HANDLE_T) x_strtoll(pps_argv[2],NULL, 10);
        
        svl_print_info(ui2_svl_id);
        x_svl_fs_store(h_dir, pps_argv[3], ui2_svl_id);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{SVL} save   svl_id dir_handle dir_path\n");
        x_dbg_stmt("{SVL} where: svl_id specifies the SVL object to save, dir_handle is handle ot directory, dir_path is a string specifying the directory under the directory handle. \n");
        x_dbg_stmt("{SVL} example: save 1 0 /dev/nor_2 \n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_svl_del_from_file
 *
 * Description: This API deletes the SVL object to filesystem.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_svl_del_from_file
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 4 )
    {
        UINT16      ui2_svl_id;
        HANDLE_T    h_dir;

        ui2_svl_id  = (UINT16) x_strtoll(pps_argv[1],NULL, 10);
        h_dir       = (HANDLE_T) x_strtoll(pps_argv[2],NULL, 10);
        x_svl_fs_del(h_dir, pps_argv[3], ui2_svl_id);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{SVL} delete  <svl_id dir_handle dir_path\n");
        x_dbg_stmt("{SVL} where: svl_id specifies the SVL object to delete, dir_handle is handle ot directory, dir_path is a string specifying the directory under the directory handle. \n");
        x_dbg_stmt("{SVL} example: delete 1 0 /dev/nor_2 \n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

#endif /* ! CLI_STRESS_MODE */


/*-------------------------------------------------------------------------
 * Name: cli_svl_filesys_info
 *
 * Description: This API displays filesystem information.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_svl_filesys_info(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32     i4_rc;

    if ( i4_argc == 3 )
    {
        HANDLE_T    h_dir;
        
        h_dir       = (HANDLE_T) x_strtoll(pps_argv[1],NULL, 10);
        fs_raw_get_finfo(h_dir,pps_argv[2]);
        i4_rc = CLIR_OK;
    }    
    else
    {
         x_dbg_stmt("{SVL} fsysinfo < dir_handle dir_path > \n");
         x_dbg_stmt("{SVL} where: dir_handle is handle ot directory, dir_path is a string specifying the directory under the directory handle. \n");
         x_dbg_stmt("{SVL} example: fsysinfo 0 /dev/nor_2 \n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

extern INT32 fs_raw_copy ( HANDLE_T         h_dir,
                           const CHAR*      ps_fpath,
                           HANDLE_T         h_dir_dst,
                           const CHAR*      ps_fpath_dst
                         );
/*-------------------------------------------------------------------------
 * Name: cli_svl_dup_filesys
 *
 * Description: This API displays filesystem information.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_svl_dup_filesys(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32     i4_rc;

    if ( i4_argc == 3 )
    {
        HANDLE_T    h_dir = NULL_HANDLE;
        
        i4_rc = fs_raw_copy(h_dir, pps_argv[1], h_dir, pps_argv[2]);
    }    
    else
    {
         x_dbg_stmt("{SVL} fsysdup < dir_path_1 dir_path_2 > \n");
         x_dbg_stmt("{SVL} example: fsysinfo /dev/nor_2 /mnt/usb/Mass-000/abc \n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_svl_dup_filesys_auto
 *
 * Description: This API displays filesystem information.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_svl_dup_filesys_auto(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32     i4_rc;

    /* the path is hard-coded */
#if 1
    x_dbg_stmt("Dumping SVL/TSL main RAW filesystem...\r\n");
    i4_rc = fs_raw_copy(NULL_HANDLE, SN_DEV_PATH "/" SN_CH_LIST_1,
                        NULL_HANDLE, "/mnt/usb/Mass-000/svltsl_main.bin");
    x_dbg_stmt("Dumping SVL/TSL sub RAW filesystem...\r\n");
    i4_rc = fs_raw_copy(NULL_HANDLE, SN_DEV_PATH "/" SN_CH_LIST_2,
                        NULL_HANDLE, "/mnt/usb/Mass-000/svltsl_sub.bin");
#endif
    
    return i4_rc;  
}


#ifndef CLI_STRESS_MODE

/*-------------------------------------------------------------------------
 * Name: cli_svl_set_customer_data
 *
 * Description: This API write out customer data to a specified SVL record.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_svl_set_customer_data
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 4 )
    {
        UINT16                 ui2_svl_id;
        UINT16                 ui2_svl_rec_id;
        HANDLE_T               h_svl;
        SVL_CLIENT_DATA_T      t_client_data;
        
        ui2_svl_id        = (UINT16) x_strtoll(pps_argv[1],NULL, 10);
        ui2_svl_rec_id    = (UINT16) x_strtoll(pps_argv[2],NULL, 10);

        if ( x_svl_open(ui2_svl_id, NULL, NULL, &h_svl) == SVLR_OK )
        {
            t_client_data.pui1_data_buf = (UINT8*) pps_argv[3];
            t_client_data.ui2_data_tag = x_strlen(pps_argv[3]);
            t_client_data.ui1_buf_len  = x_strlen(pps_argv[3]) + 1 ;
            t_client_data.ui1_data_len = x_strlen(pps_argv[3]) + 1 ;

            if ( x_svl_lock(h_svl) == SVLR_OK )
            {
                if ( x_svl_set_client_data(h_svl,ui2_svl_rec_id, &t_client_data)
                     == SVLR_OK )
                {
                    x_dbg_stmt("{SVL} customer data saved.\n");
                }
                else
                {
                    x_dbg_stmt("{SVL} Error: can not write customer data.\n");
                }
                x_svl_unlock(h_svl);
            }
            else
            {
                x_dbg_stmt("{SVL} Error: can not obtain write lock.\n");
            }
            x_svl_close(h_svl);
        }
        else
        {
            x_dbg_stmt("{SVL} Error: can not open SVL handle. \n");
        }
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{SVL} set_customer_data   <svl_id svl_rec_id customer_data>\n");
        x_dbg_stmt("{SVL} where: svl_id specifies the SVL object to save, svl_rec_id specify the record to set the customer data, customer_data (e.g. some string) to be written to specified SVL record. \n");
        x_dbg_stmt("{SVL} example: set_customer_data 1 10 ABC_SPORT \n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_svl_get_customer_data
 *
 * Description: This API reads customer data from a specified SVL record.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_svl_get_customer_data
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 3 )
    {
        UINT16                 ui2_svl_id;
        UINT16                 ui2_svl_rec_id;
        HANDLE_T               h_svl;
        SVL_CLIENT_DATA_T      t_client_data;
        CHAR                   c_buf[96];
        UINT32                 ui4_ver_id;
        
        ui2_svl_id        = (UINT16) x_strtoll(pps_argv[1],NULL, 10);
        ui2_svl_rec_id    = (UINT16) x_strtoll(pps_argv[2],NULL, 10);

        if ( x_svl_open(ui2_svl_id, NULL, NULL, &h_svl) == SVLR_OK )
        {
            t_client_data.pui1_data_buf = (UINT8*)c_buf;
            t_client_data.ui1_buf_len  = sizeof(c_buf) ;
            t_client_data.ui1_data_len = 0;

            if ( x_svl_get_client_data(h_svl,ui2_svl_rec_id, &t_client_data, &ui4_ver_id)
                 == SVLR_OK )
            {
                x_dbg_stmt("{SVL} customer data read success: data len [%d], "
                           "private tag value [%d] data content [%s] \n",
                           t_client_data.ui1_data_len,
                           t_client_data.ui2_data_tag,
                           t_client_data.pui1_data_buf);
            }
            else
            {
                x_dbg_stmt("{SVL} Error: can not read customer data from SVL record [%d]\n",
                           ui2_svl_rec_id);
            }
            x_svl_close(h_svl);
        }
        else
        {
            x_dbg_stmt("{SVL} Error: can not open SVL handle. \n");
        }
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{SVL} get_customer_data   <svl_id svl_rec_id>\n");
        x_dbg_stmt("{SVL} where: svl_id specifies the SVL object to save, svl_rec_id specify the record to set the customer data. \n");
        x_dbg_stmt("{SVL} example: get_customer_data 1 10\n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}


/*-------------------------------------------------------------------------
 * Name: cli_svl_sync_data_with_file
 *
 * Description: This API reads customer data from a specified SVL record.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_svl_sync_data_with_file
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 4 )
    {
        UINT16                 ui2_svl_id;
        HANDLE_T               h_svl;
        HANDLE_T               h_dir;
        
        ui2_svl_id  = (UINT16) x_strtoll(pps_argv[1],NULL, 10);
        h_dir       = (HANDLE_T) x_strtoll(pps_argv[2],NULL, 10);

        if ( x_svl_open(ui2_svl_id, NULL, NULL, &h_svl) == SVLR_OK )
        {
            if ( x_svl_fs_sync(h_svl, h_dir, pps_argv[3], ui2_svl_id) == SVLR_OK )
            {
                x_dbg_stmt("{SVL} SVL (ID: %d) synced with data from file.\n",ui2_svl_id);
            }
            else
            {
                x_dbg_stmt("{SVL} Error: can not sync with SVL data from file. \n");
            }
            x_svl_close(h_svl);
        }
        else
        {
            x_dbg_stmt("{SVL} Error: can not sync with SVL data from file. \n");
        }
            
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{SVL} sync_with_file svl_id dir_handle dir_path\n");
        
        x_dbg_stmt("{SVL} where: svl_id specifies the SVL object to sync, dir_handle is handle to a directory, dir_path is a string specifying the directory under the directory handle.\n");
        
        x_dbg_stmt("{SVL} example: sync_with_file 1 0 /dev/nor_2 \n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_svl_reinit_fsys
 *
 * Description: This API reads customer data from a specified SVL record.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_svl_reinit_fsys(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32     i4_rc;

    if ( i4_argc == 3 )
    {
        HANDLE_T    h_dir;
        
        h_dir       = (HANDLE_T) x_strtoll(pps_argv[1],NULL, 10);
        fs_raw_init(h_dir,pps_argv[2]);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{SVL} reinit_fs  <dir_handle dir_path\n");
        x_dbg_stmt("{SVL} This command cleans the filesystem where SVL data are stored.\n");
        x_dbg_stmt("Where: dir_handle is handle ot directory, dir_path is a string specifying the directory under the directory handle. \n");
        x_dbg_stmt("{SVL} example: reinit_fs 0 /dev/nor_2 \n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_svl_dump_to_mem
 *
 * Description: This API dumps SVL to memory and prints the address.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_svl_dump_to_mem
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 2 )
    {
        UINT16                 ui2_svl_id;
        HANDLE_T               h_svl;
        SVL_MEM_DUMP_T*        pt_mem_dump = NULL;
        
        ui2_svl_id        = (UINT16) x_strtoll(pps_argv[1],NULL, 10);

        if ( x_svl_open(ui2_svl_id, NULL, NULL, &h_svl) == SVLR_OK )
        {
            pt_mem_dump = (SVL_MEM_DUMP_T *) x_mem_alloc(sizeof(SVL_MEM_DUMP_T));

            if ( pt_mem_dump != NULL )
            {
                x_svl_init_mem_dump(pt_mem_dump);
                if ( x_svl_dump_to_mem(h_svl, pt_mem_dump)
                     == SVLR_OK )
                {
                    x_dbg_stmt("{SVL} memory dumped to 0x%x.\n", pt_mem_dump);
                }
                else
                {
                    x_dbg_stmt("{SVL} Error: failed to dump SVL.\n");
                    x_mem_free(pt_mem_dump);
                }
            }
            else
            {
                x_dbg_stmt("{SVL} Error: failed to allocate memory.\n");
            }
            x_svl_close(h_svl);
        }
        else
        {
            x_dbg_stmt("{SVL} Error: can not open SVL handle. \n");
        }
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{SVL} memdump   <svl_id>\n");
        x_dbg_stmt("{SVL} where: svl_id specifies the SVL object to save.\n");
        x_dbg_stmt("{SVL} example: memdump 1\n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_svl_load_from_mem
 *
 * Description: This API loads SVL from memory.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_svl_load_from_mem
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 3 )
    {
        UINT16                 ui2_svl_id;
        HANDLE_T               h_svl;
        SVL_MEM_DUMP_T*        pt_mem_dump = NULL;
        UINT32                 ui4_tmp;
        
        ui2_svl_id        = (UINT16) x_strtoll(pps_argv[1],NULL, 10);
        ui4_tmp           = (UINT32) x_strtoll(pps_argv[2],NULL, 0);
        pt_mem_dump       = (SVL_MEM_DUMP_T *) ui4_tmp;

        if ( x_svl_open(ui2_svl_id, NULL, NULL, &h_svl) == SVLR_OK )
        {
            if ( pt_mem_dump != NULL )
            {
                if ( x_svl_load_from_mem(h_svl, pt_mem_dump)
                     == SVLR_OK )
                {
                    x_dbg_stmt("{SVL} load okay.\n");
                    x_svl_free_mem_dump(pt_mem_dump);
                    x_mem_free(pt_mem_dump);
                }
                else
                {
                    x_dbg_stmt("{SVL} Error: failed to load SVL data.\n");
                }
            }
            else
            {
                x_dbg_stmt("{SVL} Error: invalid memory address 0x%x.\n", pt_mem_dump);
            }
            x_svl_close(h_svl);
        }
        else
        {
            x_dbg_stmt("{SVL} Error: can not open SVL handle. \n");
        }
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{SVL} memload   <svl_id mem_addr>\n");
        x_dbg_stmt("{SVL} where: svl_id specifies the SVL object to load, mem_addr specifies the memory address of a previous memory dump.\n");
        x_dbg_stmt("{SVL} example: memload 1 0x12345\n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}




static INT32 cli_svl_enable_log(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32     i4_rc;
    
    if ( i4_argc == 3 )
    {
        if(x_strcmp(pps_argv[1] , "1") == 0)
        {
            svl_enable_api_log(TRUE);
        }
        else if(x_strcmp(pps_argv[1] , "0") == 0)
        {
            svl_enable_api_log(FALSE);
        }

        if(x_strcmp(pps_argv[2] , "1") == 0)
        {
            svl_enable_rec_dmp(TRUE);
        }
        else if(x_strcmp(pps_argv[2] , "0") == 0)
        {
            svl_enable_rec_dmp(FALSE);
        }

        i4_rc = CLIR_OK;
    }
    else
    {
        x_dbg_stmt("{SVL} log <API> <DUMP>\n");
        x_dbg_stmt("{SVL} where: \'API\' used to enable or disable API log , \'DUMP\' for svl rec dump\n");
        x_dbg_stmt("{SVL} example: log 1 0\n");

        i4_rc=CLIR_INV_CMD_USAGE;
    }

    return i4_rc;  
}

#endif /* ! CLI_STRESS_MODE */

/*-------------------------------------------------------------------------
 * Name: svl_cli_init
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
INT32 svl_cli_init(VOID)
{
    /* Attach SVL CLI command table to CLI */
    if ( x_cli_attach_cmd_tbl(at_cli_dbg_tbl, CLI_CAT_MW, CLI_GRP_NONE)
         != CLIR_OK)
    {
        return SVLR_FAIL;
    }
    
    return SVLR_OK;   
}

#endif  /* CLI_LVL_ALL || CLI_LVL_GUEST || CLI_STRESS_MODE */
    

    
