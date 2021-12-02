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
 * $RCSfile: lstm_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains functions and internal variables to support
 *         CLI control of the lst_mngr module.
 *
 *---------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "dbg/x_dbg.h"
#include "cli/x_cli.h"
#include "os/inc/x_os.h"

#include "lst_mngr/_lstm.h"
#include "lst_mngr/x_lst_mngr.h"
#include "util/fs_raw.h"
#include "lst_mngr/lstm_dual_bank.h"

#if defined(CLI_LVL_ALL) || defined(CLI_LVL_GUEST)

/* ----------------------------------------------------------------------
  Typedefs   
------------------------------------------------------------------------- */

/* DateTime module  CLI definitions. */

#ifdef CLI_LVL_ALL
static INT32 cli_get_dbg_level (INT32  i4_argc, const CHAR**  pps_argv);
static INT32 cli_set_dbg_level (INT32  i4_argc, const CHAR**  pps_argv);
static INT32 cli_lstm_init_lst_id(INT32 i4_argc,const CHAR** pps_argv);
static INT32 cli_lstm_deinit_lst_id(INT32 i4_argc,const CHAR** pps_argv);
#endif
static INT32 cli_lstm_show_info(INT32 i4_argc,const CHAR** pps_argv);
static INT32 cli_lstm_save_to_file(INT32 i4_argc,const CHAR** pps_argv);
static INT32 cli_lstm_sync_data_with_file(INT32 i4_argc, const CHAR** pps_argv);

static INT32 cli_lstm_filesys_info(INT32 i4_argc, const CHAR** pps_argv);
static INT32 cli_lstm_init_filesys(INT32 i4_argc, const CHAR** pps_argv);
static INT32 cli_lstm_dup_filesys(INT32 i4_argc, const CHAR** pps_argv);

#ifdef CLI_LVL_ALL
static INT32 cli_lstm_reg_dual_bank(INT32 i4_argc, const CHAR** pps_argv);
static INT32 cli_lstm_reg_def_db(INT32 i4_argc, const CHAR** pps_argv);

static INT32 cli_lstm_dual_bank_save(INT32 i4_argc,const CHAR** pps_argv);
static INT32 cli_lstm_dual_bank_load(INT32 i4_argc,const CHAR** pps_argv);
#endif
static INT32 cli_lstm_dual_bank_reset(INT32 i4_argc,const CHAR** pps_argv);
static INT32 cli_lstm_load_default_db(INT32 i4_argc,const CHAR** pps_argv);
static INT32 cli_lstm_dual_bank_load_fb(INT32 i4_argc,const CHAR** pps_argv);
static INT32 cli_lstm_clean_satl_ch(INT32 i4_argc, const CHAR** pps_argv);

static CLI_EXEC_T at_cli_dbg_cmd[] =
{
#ifdef CLI_LVL_ALL
    {"gdl",  NULL, cli_get_dbg_level, NULL,
     CLI_GET_DBG_LVL_HELP_STR,  CLI_SUPERVISOR},
    {"sdl",  NULL, cli_set_dbg_level, NULL,
     CLI_SET_DBG_LVL_HELP_STR,  CLI_SUPERVISOR},
    {"init",  NULL, cli_lstm_init_lst_id, NULL,
     "[save LST_MASK LST_ID] initialize list IDs.", CLI_SUPERVISOR},
    {"deinit",  NULL, cli_lstm_deinit_lst_id, NULL,
     "[sync LST_MASK LST_ID] deinitialize list IDs filesystem.", CLI_SUPERVISOR},
#endif
    {"info",  NULL, cli_lstm_show_info, NULL,
     "[info LST_ID] show List Manager configuration status" ,  CLI_GUEST},
    {"save",  NULL, cli_lstm_save_to_file, NULL,
     "[save LST_MASK LST_ID DIR_HANDLE DIR_PATH] write list objects specified to filesystem.", CLI_GUEST},
    {"sync",  NULL, cli_lstm_sync_data_with_file, NULL,
     "[sync LST_MASK LST_ID DIR_HANDLE DIR_PATH] sync specified list objects with data from filesystem.", CLI_GUEST},
    {"fsinfo",  NULL, cli_lstm_filesys_info, NULL,
     "[fsinfo] print out FS_RAW filesystem.", CLI_GUEST},
    {"fsinit",  NULL, cli_lstm_init_filesys, NULL,
     "[fsinit DIR_HANDLE DIR_PATH] initialize FS_RAW filesystem.", CLI_GUEST},
    {"fsdup",  NULL, cli_lstm_dup_filesys, NULL,
     "[fsdup DIR_HANDLE DIR_PATH DIR_HANDLE DIR_PATH] duplicate FS_RAW filesystem to another file.", CLI_GUEST},
#ifdef CLI_LVL_ALL
    {"regdual",  NULL, cli_lstm_reg_dual_bank, NULL,
     "[regdual DIR_HANDLE MAIN_PATH SUB_PATH] register dual bank path with fake get/set function for dual bank status.", CLI_SUPERVISOR},
    {"regdefdb",  NULL, cli_lstm_reg_def_db, NULL,
     "[regdefdb LST_MASK LST_ID DIR_HANDLE DIR_PATH LST_ID] register default database path & list ID for specified list.", CLI_SUPERVISOR},
    {"dualsave",  NULL, cli_lstm_dual_bank_save, NULL,
     "[dualsave LST_MASK LST_ID AS_MODE] write list objects specified to filesystem according to dual bank status.", CLI_SUPERVISOR},
    {"dualload",  NULL, cli_lstm_dual_bank_load, NULL,
     "[dualload LST_MASK LST_ID] sync specified list objects with data from filesystem according to dual bank status.", CLI_SUPERVISOR},
#endif
    {"dualreset",  NULL, cli_lstm_dual_bank_reset, NULL,
     "[dualreset] reset dual bank status. Dual bank status will be toggled for the next store" ,  CLI_GUEST},
    {"defdbload",  NULL, cli_lstm_load_default_db, NULL,
     "[defdbload LST_MASK LST_ID] load specified list objects with data from default database.", CLI_GUEST},
    {"dualloadfb",  NULL, cli_lstm_dual_bank_load_fb, NULL,
     "[dualload LST_MASK LST_ID] sync specified list objects with data from filesystem according to dual bank status, and if loading from dual bank fails, load from default database.", CLI_GUEST},
    {"cleansatlch",  NULL, cli_lstm_clean_satl_ch, NULL,
     "[cleansatlch SATL_ID SATL_REC_ID TRUE_2_CLEAN_TSL] clean specified sattellite's children reords.", CLI_GUEST},
    END_OF_CLI_CMD_TBL
};

static CLI_EXEC_T at_cli_dbg_tbl [] =
{
    {"lstm", NULL, NULL, &(at_cli_dbg_cmd[0]), "lst_mngr Command", CLI_GUEST},
    END_OF_CLI_CMD_TBL
};



/* --------------------------------------------------------------
  Function implementation.
---------------------------------------------------------------- */
#ifdef CLI_LVL_ALL
/*-------------------------------------------------------------------------
 * Name: cli_get_dbg_level
 *
 * Description: This API gets the debug level for the lst_mngr module.
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
        x_dbg_stmt("{LSTM} lstm.gdl\r\n");
        i4_rc = CLIR_INV_CMD_USAGE;
    }
    else
    {
        i4_rc = x_cli_show_dbg_level(lstm_get_dbg_level());
    }
    
    return i4_rc;    
        
}

/*-------------------------------------------------------------------------
 * Name: cli_set_dbg_level
 *
 * Description: This API sets the debug level for the lst_mngr module.
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
        lstm_set_dbg_level(ui2_dbg);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{LSTM} lstm.sdl <n|e|a|i>\r\n");
        x_dbg_stmt("{LSTM} where:  n=none, e=error, a=api, i=info. \r\n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;   
}

/*-------------------------------------------------------------------------
 * Name: cli_lstm_init_lst_id
 *
 * Description: This API initializes specified list types and ID's 
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_lstm_init_lst_id
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 3 )
    {
        UINT32      ui4_lst_mask;
        UINT16      ui2_lst_id;
        
        ui4_lst_mask= (UINT32) x_strtoll(pps_argv[1],NULL, 0);
        ui2_lst_id  = (UINT16) x_strtoll(pps_argv[2],NULL, 10);
        
        x_lst_mngr_init_list_id(ui2_lst_id, ui4_lst_mask);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{LSTM} init   lst_mask lst_id\r\n");
        x_dbg_stmt("{LSTM} where: lst_mask specifies the bit mask of the lists to init, lst_id specifies the list objects to init.\r\n");
        x_dbg_stmt("{LSTM} example: init 0xf 1\r\n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_lstm_deinit_lst_id
 *
 * Description: This API deinitializes specified list types and ID's 
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_lstm_deinit_lst_id
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 3 )
    {
        UINT32      ui4_lst_mask;
        UINT16      ui2_lst_id;
        
        ui4_lst_mask= (UINT32) x_strtoll(pps_argv[1],NULL, 0);
        ui2_lst_id  = (UINT16) x_strtoll(pps_argv[2],NULL, 10);
        
        x_lst_mngr_deinit_list_id(ui2_lst_id, ui4_lst_mask, TRUE);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{LSTM} init   lst_mask lst_id\r\n");
        x_dbg_stmt("{LSTM} where: lst_mask specifies the bit mask of the lists to init, lst_id specifies the list objects to init.\r\n");
        x_dbg_stmt("{LSTM} example: init 0xf 1\r\n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}
#endif

/*-------------------------------------------------------------------------
 * Name: cli_lstm_show_info
 *
 * Description: This API shows information on List Manager config status
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_lstm_show_info
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 2 )
    {
        UINT16   ui2_lst_id;

        ui2_lst_id = (UINT16) x_strtoll(pps_argv[1],NULL, 10);
        
        lstm_print_info(ui2_lst_id);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{LSTM} info   <lst_id>\n");
        x_dbg_stmt("{LSTM} where: lst_id specifies the SVL to dump records. \n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_lstm_save_to_file
 *
 * Description: This API write out the list objects to filesystem.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_lstm_save_to_file
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 5 )
    {
        UINT32      ui4_lst_mask;
        UINT16      ui2_lst_id;
        HANDLE_T    h_dir;
        INT32       i4_tick;
        
        ui4_lst_mask= (UINT32) x_strtoll(pps_argv[1],NULL, 0);
        ui2_lst_id  = (UINT16) x_strtoll(pps_argv[2],NULL, 10);
        h_dir       = (HANDLE_T) x_strtoll(pps_argv[3],NULL, 10);
        
        i4_tick = (INT32) x_os_get_sys_tick();
        x_lst_mngr_fs_store(h_dir, pps_argv[4], ui2_lst_id, ui4_lst_mask);
        i4_tick = (INT32) x_os_get_sys_tick() - i4_tick;
        x_dbg_stmt("{LSTM} save costs %d ms\r\n", i4_tick * x_os_get_sys_tick_period());
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{LSTM} save   lst_mask lst_id dir_handle dir_path\r\n");
        x_dbg_stmt("{LSTM} where: lst_mask specifies the bit mask of the lists to save, lst_id specifies the list objects to save, dir_handle is handle of directory, dir_path is a string specifying the directory under the directory handle. \r\n");
        x_dbg_stmt("{LSTM} example: save 0xf 1 0 /dev/nor_2 \r\n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_lstm_sync_data_with_file
 *
 * Description: This API reads customer data from specified lists.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_lstm_sync_data_with_file
(
    INT32           i4_argc,
    const CHAR**    pps_argv
)
{
    INT32     i4_rc;

    if ( i4_argc == 5 )
    {
        UINT32      ui4_lst_mask;
        UINT16      ui2_lst_id;
        HANDLE_T    h_dir;
        
        ui4_lst_mask= (UINT32) x_strtoll(pps_argv[1],NULL, 0);
        ui2_lst_id  = (UINT16) x_strtoll(pps_argv[2],NULL, 10);
        h_dir       = (HANDLE_T) x_strtoll(pps_argv[3],NULL, 10);
        
        x_lst_mngr_fs_sync(h_dir, pps_argv[4], ui2_lst_id, ui4_lst_mask);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{LSTM} sync   lst_mask lst_id dir_handle dir_path\r\n");
        x_dbg_stmt("{LSTM} where: lst_mask specifies the bit mask of the lists to sync, lst_id specifies the list objects to sync, dir_handle is handle of directory, dir_path is a string specifying the directory under the directory handle. \r\n");
        x_dbg_stmt("{LSTM} example: save 0xf 1 0 /dev/nor_2 \r\n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_lstm_filesys_info
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
static INT32 cli_lstm_filesys_info(INT32 i4_argc, const CHAR** pps_argv)
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
         x_dbg_stmt("{LSTM} fsinfo < dir_handle dir_path > \n");
         x_dbg_stmt("{LSTM} where: dir_handle is handle of directory, dir_path is a string specifying the directory under the directory handle. \n");
         x_dbg_stmt("{LSTM} example: fsinfo 0 /dev/nor_2 \n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_lstm_init_filesys
 *
 * Description: This API initializes filesystem.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_lstm_init_filesys(INT32 i4_argc, const CHAR** pps_argv)
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
         x_dbg_stmt("{LSTM} fsinit < dir_handle dir_path > \n");
         x_dbg_stmt("{LSTM} where: dir_handle is handle of directory, dir_path is a string specifying the directory under the directory handle. \n");
         x_dbg_stmt("{LSTM} example: fsinit 0 /dev/nor_2 \n");
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
 * Name: cli_lstm_dup_filesys
 *
 * Description: This API duplicates filesystem to another place.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_lstm_dup_filesys(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32     i4_rc;

    if ( i4_argc == 3 )
    {
        HANDLE_T    h_dir = NULL_HANDLE;
        
        i4_rc = fs_raw_copy(h_dir, pps_argv[1], h_dir, pps_argv[2]);
    }    
    else
    {
         x_dbg_stmt("{LSTM} fsdup < dir_path_1 dir_path_2 > \n");
         x_dbg_stmt("{LSTM} example: fsdup /dev/nor_2 /mnt/usb/Mass-000/abc \n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

#ifdef CLI_LVL_ALL
static UINT8 ui1_g_dual_bank_status = DUAL_BANK_SVL_TSL_MAIN;
INT32 _get_dual_bank(UINT8 *pui1_val)
{
    if (pui1_val == NULL)
    {
        return LSTMR_INV_ARG;
    }
    *pui1_val = ui1_g_dual_bank_status;
    return LSTMR_OK;
}
INT32 _set_dual_bank(UINT8 ui1_val)
{
    ui1_g_dual_bank_status = ui1_val;
    return LSTMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: cli_lstm_reg_dual_bank
 *
 * Description: This API registers dual bank main/sub path with dummy
 *              set/get functions.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_lstm_reg_dual_bank(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32     i4_rc;

    if ( i4_argc == 4 )
    {
        HANDLE_T    h_dir = NULL_HANDLE;
        
        h_dir = (HANDLE_T) x_strtoll(pps_argv[1],NULL, 10);
        i4_rc = x_lst_mngr_reg_dual_bank(h_dir, pps_argv[2], pps_argv[3],
                                         _get_dual_bank, _set_dual_bank);
        if (i4_rc != LSTMR_OK)
        {
            x_dbg_stmt("{LSTM} failed with ret code %d\r\n", i4_rc);
            i4_rc = CLIR_CMD_EXEC_ERROR;
        }
    }    
    else
    {
        x_dbg_stmt("{LSTM} regdual < dir_handle main_path sub_path> \n");
        x_dbg_stmt("{LSTM} example: regdual 0 /dev/nor_2 /dev/nor_4 \n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_lstm_reg_def_db
 *
 * Description: This API registers default database path & list ID for
 *              specified lists
 *              set/get functions.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_lstm_reg_def_db(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32     i4_rc;

    if ( i4_argc == 6 )
    {
        HANDLE_T    h_dir = NULL_HANDLE;
        UINT16      ui2_lst_id;
        UINT16      ui2_def_db_lst_id;
        UINT32      ui4_lst_mask;
        
        ui4_lst_mask= (UINT32) x_strtoll(pps_argv[1],NULL, 0);
        ui2_lst_id  = (UINT16) x_strtoll(pps_argv[2],NULL, 10);
        h_dir       = (HANDLE_T) x_strtoll(pps_argv[3],NULL, 10);
        ui2_def_db_lst_id   = (UINT16) x_strtoll(pps_argv[5],NULL, 10);
        i4_rc = x_lst_mngr_reg_default_db(ui2_lst_id, ui4_lst_mask, h_dir,
                                          pps_argv[4], ui2_def_db_lst_id);
        if (i4_rc != LSTMR_OK)
        {
            x_dbg_stmt("{LSTM} failed with ret code %d\r\n", i4_rc);
            i4_rc = CLIR_CMD_EXEC_ERROR;
        }
    }    
    else
    {
        x_dbg_stmt("{LSTM} regdefdb < lst_mask lst_id def_db_dir_handle def_db_dir_path def_db_lst_id> \n");
        x_dbg_stmt("{LSTM} example: regdual 0xf 1 0 /dev/nor_2 9 \n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_lstm_dual_bank_save
 *
 * Description: This API write out the list objects to filesystem according
 *              to dual bank status.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_lstm_dual_bank_save(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32     i4_rc;

    if ( i4_argc == 4 )
    {
        UINT32      ui4_lst_mask;
        UINT16      ui2_lst_id;
        INT32       i4_tick;
        LST_MNGR_DUAL_BANK_AS_MODE_T e_as_mode;
        
        ui4_lst_mask= (UINT32) x_strtoll(pps_argv[1],NULL, 0);
        ui2_lst_id  = (UINT16) x_strtoll(pps_argv[2],NULL, 10);
        e_as_mode   = (LST_MNGR_DUAL_BANK_AS_MODE_T) x_strtoll(pps_argv[3],NULL, 10);
        
        i4_tick = (INT32) x_os_get_sys_tick();
        x_lst_mngr_dual_bank_fs_store(ui2_lst_id, ui4_lst_mask, e_as_mode);
        i4_tick = (INT32) x_os_get_sys_tick() - i4_tick;
        x_dbg_stmt("{LSTM} save costs %d ms\r\n", i4_tick * x_os_get_sys_tick_period());
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{LSTM} dualsave < lst_mask lst_id as_mode > \r\n");
        x_dbg_stmt("{LSTM} where: lst_mask specifies the bit mask of the lists to save, lst_id specifies the list objects to save, as_mode: 0 for get dual bank status from NVM, 1 for main/sub both, 2 for main, 3 for sub. \r\n");
        x_dbg_stmt("{LSTM} example: dualsave 0xf 1 0 \r\n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

/*-------------------------------------------------------------------------
 * Name: cli_lstm_dual_bank_load
 *
 * Description: This API reads customer data from specified lists according
 *              to dual bank status.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_lstm_dual_bank_load(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32     i4_rc;

    if ( i4_argc == 3 )
    {
        UINT32      ui4_lst_mask;
        UINT16      ui2_lst_id;
        
        ui4_lst_mask= (UINT32) x_strtoll(pps_argv[1],NULL, 0);
        ui2_lst_id  = (UINT16) x_strtoll(pps_argv[2],NULL, 10);
        
        x_lst_mngr_dual_bank_fs_sync(ui2_lst_id, ui4_lst_mask);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{LSTM} dualload < lst_mask lst_id > \r\n");
        x_dbg_stmt("{LSTM} where: lst_mask specifies the bit mask of the lists to load, lst_id specifies the list objects to load\r\n");
        x_dbg_stmt("{LSTM} example: dualload 0xf 1 \r\n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}
#endif

/*-------------------------------------------------------------------------
 * Name: cli_lstm_dual_bank_reset
 *
 * Description: This API resets dual bank status.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_lstm_dual_bank_reset(INT32 i4_argc, const CHAR** pps_argv)
{
    x_lst_mngr_reset_dual_bank_status();
    return CLIR_OK;
}

/*-------------------------------------------------------------------------
 * Name: cli_lstm_load_default_db
 *
 * Description: This API reads customer data from specified lists according
 *              to dual bank status, with fallback to default database.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_lstm_load_default_db(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32     i4_rc;

    if ( i4_argc == 4 )
    {
        UINT32      ui4_lst_mask;
        UINT16      ui2_lst_id;
        BOOL        b_clean_list;
        
        ui4_lst_mask= (UINT32) x_strtoll(pps_argv[1],NULL, 0);
        ui2_lst_id  = (UINT16) x_strtoll(pps_argv[2],NULL, 10);
        b_clean_list  = (BOOL) x_strtoll(pps_argv[3],NULL, 10);
        
        x_lst_mngr_load_default_db(ui2_lst_id, ui4_lst_mask, b_clean_list);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{LSTM} defdbload < lst_mask lst_id b_clean > \r\n");
        x_dbg_stmt("{LSTM} where: lst_mask specifies the bit mask of the lists to load, lst_id specifies the list objects to load, b_clean (0/1) indicates whether to clean list if load fail\r\n");
        x_dbg_stmt("{LSTM} example: defdbload 0xf 1 1 \r\n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}
/*-------------------------------------------------------------------------
 * Name: cli_lstm_dual_bank_load_fb
 *
 * Description: This API reads customer data from specified lists according
 *              to dual bank status, with fallback to default database.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_lstm_dual_bank_load_fb(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32     i4_rc;

    if ( i4_argc == 3 )
    {
        UINT32      ui4_lst_mask;
        UINT16      ui2_lst_id;
        
        ui4_lst_mask= (UINT32) x_strtoll(pps_argv[1],NULL, 0);
        ui2_lst_id  = (UINT16) x_strtoll(pps_argv[2],NULL, 10);
        
        x_lst_mngr_dual_bank_fs_sync_with_fallback(ui2_lst_id, ui4_lst_mask);
        i4_rc = CLIR_OK;
    }    
    else
    {
        x_dbg_stmt("{LSTM} dualloadfb < lst_mask lst_id > \r\n");
        x_dbg_stmt("{LSTM} where: lst_mask specifies the bit mask of the lists to load, lst_id specifies the list objects to load\r\n");
        x_dbg_stmt("{LSTM} example: dualloadfb 0xf 1 \r\n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
    
    return i4_rc;  
}

static INT32 cli_lstm_dual_bank_load_fb(INT32 i4_argc,const CHAR** pps_argv);
/*-------------------------------------------------------------------------
 * Name: lst_mngr_cli_init
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
INT32 lst_mngr_cli_init(VOID)
{
    /* Attach lst_mngr CLI command table to CLI */
    if ( x_cli_attach_cmd_tbl(at_cli_dbg_tbl, CLI_CAT_MW, CLI_GRP_NONE)
         != CLIR_OK)
    {
        return LSTMR_FAIL;
    }
    
    return LSTMR_OK;   
}


/*-------------------------------------------------------------------------
 * Name: cli_lstm_clean_satl_ch
 *
 * Description: This API clean the specified satellite's children
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
static INT32 cli_lstm_clean_satl_ch(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32     i4_rc = LSTMR_OK;

#ifdef MW_DVBS_DEV_ENABLE
    if ( i4_argc == 4 )
    {
        UINT16      ui2_satl_id;
        UINT16      ui2_satl_rec_id;
        BOOL        b_clean_tsl;
        
        ui2_satl_id     = (UINT32) x_strtoll(pps_argv[1],NULL, 10);
        ui2_satl_rec_id = (UINT16) x_strtoll(pps_argv[2],NULL, 10);
        b_clean_tsl     = (BOOL)   x_strtoll(pps_argv[3],NULL, 10);
        
        i4_rc = x_lst_mngr_clean_satl_ch(
                    ui2_satl_id, 
                    ui2_satl_rec_id, 
                    b_clean_tsl);
    }    
    else
    {
        x_dbg_stmt("{LSTM} cleansatlch satl_id satl_rec_id b_clean_tsl \r\n");
        x_dbg_stmt("{LSTM} where: satl_id specifies satl list id\r\n");
        x_dbg_stmt("              satl_rec_id specifies the satl record id\r\n");
        x_dbg_stmt("              b_clean_tsl indicates whether to clean related TSL\r\n");
        x_dbg_stmt("{LSTM} example: cleansatlch 3 1 0\r\n");
        i4_rc=CLIR_INV_CMD_USAGE;
    }
#endif     
    return i4_rc;  
}


#endif  /* CLI_LVL_ALL || CLI_LVL_GUEST */
