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
 * $RCSfile: dmx_util_cli.c,v $
 * $Revision:
 * $Date:
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_ATSC/8 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 36c0b5ff3e590d92a1efcdb6b1e45528 $
 *
 * Description:
 *         This file implements CLI command table for dmx utility library.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "inc/common.h"
#include "os/inc/x_os.h"
#include "dbg/x_dbg.h"
#include "cli/x_cli.h"
#include "handle/handle.h"
#include "dmx_util/x_dmx_util.h"
#include "res_mngr/rm.h"

#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define PATTERN_INIT_ARRAY              {0, 0, 0, 0, 0, 0, 0, 0}
#define DMX_UT_CLI_SECT_BUF_SIZE        (256)

static INT32 _dmx_ut_cli_pf_alloc (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _dmx_ut_cli_pf_free (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _dmx_ut_cli_sf_alloc (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _dmx_ut_cli_sf_set (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _dmx_ut_cli_sf_enable (INT32 i4_argc, const CHAR** pps_argv);
static INT32 _dmx_ut_cli_sf_free (INT32 i4_argc, const CHAR** pps_argv);

static HANDLE_T    _g_h_pid_filter = NULL_HANDLE;
static UINT8       _g_aui1_buf[DMX_UT_CLI_SECT_BUF_SIZE];

static INT32 _dmx_ut_cli_get_dbg_level (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32  i4_ret;

    i4_ret = x_cli_show_dbg_level(dmx_ut_get_dbg_level());

    return i4_ret;
}

static INT32 _dmx_ut_cli_set_dbg_level (INT32 i4_argc, const CHAR** pps_argv)
{
    UINT16 ui2_dbg_level;
    INT32  i4_ret;

    i4_ret = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);

    if (i4_ret == CLIR_OK){
        dmx_ut_set_dbg_level(ui2_dbg_level);
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/* dmx_util library command table */
static CLI_EXEC_T at_dmx_ut_cmd_tbl[] =
{
    {CLI_GET_DBG_LVL_STR,   NULL,   _dmx_ut_cli_get_dbg_level,  NULL,   CLI_GET_DBG_LVL_HELP_STR,   CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR,   NULL,   _dmx_ut_cli_set_dbg_level,  NULL,   CLI_SET_DBG_LVL_HELP_STR,   CLI_SUPERVISOR},
    {"pf_alloc",            NULL,   _dmx_ut_cli_pf_alloc,       NULL,   "allocate pid filter",      CLI_SUPERVISOR},
    {"pf_free",             NULL,   _dmx_ut_cli_pf_free,        NULL,   "free pid filter",          CLI_SUPERVISOR},
    {"sf_alloc",            NULL,   _dmx_ut_cli_sf_alloc,       NULL,   "allocate section filter",  CLI_SUPERVISOR},
    {"sf_set",              NULL,   _dmx_ut_cli_sf_set,         NULL,   "set section filter",       CLI_SUPERVISOR},
    {"sf_enable",           NULL,   _dmx_ut_cli_sf_enable,      NULL,   "enable/disable section filter",  CLI_SUPERVISOR},
    {"sf_free",             NULL,   _dmx_ut_cli_sf_free,        NULL,   "free section filter",      CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};


/* IO Manager root command table */
static CLI_EXEC_T at_dmx_ut_root_cmd_tbl[] =
{
    {"dmx_ut",     NULL,        NULL,       at_dmx_ut_cmd_tbl,      "Demux Utility library commands",   CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

static HANDLE_T _find_handle_by_pid(MPEG_2_PID_T    t_pid)
{
    /*TODO: should support mutliple pid search*/
    return _g_h_pid_filter;
}

static BOOL dmx_ut_cli_pid_nfy(DMX_UT_COND_T    e_nfy_cond,
                               VOID*            pv_src_tag,
                               SIZE_T           z_size,
                               UINT32           ui4_nfy_data)
{
    INT32       i4_ret;
    HANDLE_T    h_pid_filter = (HANDLE_T)ui4_nfy_data;
    UINT8       ui1_i;
    SIZE_T      z_read_size;
    
    switch (e_nfy_cond)
    {
    case DMX_UT_COND_DATA_REC:
        z_read_size = z_size>DMX_UT_CLI_SECT_BUF_SIZE?DMX_UT_CLI_SECT_BUF_SIZE:z_size;
        
        i4_ret = x_dmx_ut_sec_data_read(h_pid_filter,
                               pv_src_tag,
                               0,
                               z_read_size,
                               &(_g_aui1_buf[0]));
        if (i4_ret == DMXR_UT_OK)
        {
            x_dmx_ut_sec_data_release(h_pid_filter,
                                      pv_src_tag,
                                      z_read_size);
        
            x_dbg_stmt("!!section data received(%d bytes)!!\n\n", z_size);
            
            /* print section data */
            for (ui1_i = 0; ui1_i < z_read_size; ui1_i++)
            {
                x_dbg_stmt("0x%02X ", _g_aui1_buf[ui1_i]);
                if ((ui1_i+1)%16==0)
                {
                    x_dbg_stmt("\n");    
                }
            }
            x_dbg_stmt("\n\n");
            
        }
        else
        {
            x_dbg_stmt("!!section read data fail(%d)!!\n", i4_ret);
        }
        break;

    case DMX_UT_COND_DATA_OVERFLOW:
        x_dbg_stmt("!!section data overflow!!\n\n");
        break;

    default:
        x_dbg_stmt("!!unknow condition!!\n\n");
        break;
    }
    return TRUE;
}



/*-----------------------------------------------------------------------------
 * Name: _dmx_ut_cli_pf_alloc
 *
 * Description:
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _dmx_ut_cli_pf_alloc (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32          i4_ret;
    MPEG_2_PID_T   t_pid;
    SIZE_T         z_buf_size;
    HANDLE_T       h_pid_filter;
    
    if(i4_argc != 3)
    {
        x_dbg_stmt("Usage: pf_alloc [PID] [Buffer Size]\n");
        return CLIR_INV_CMD_USAGE;
    }

    t_pid       = (MPEG_2_PID_T) x_strtoull(pps_argv[1], NULL, 0);
    z_buf_size  = (SIZE_T) x_strtoull(pps_argv[2], NULL, 0);

    i4_ret = x_dmx_ut_pid_filter_alloc(t_pid,
                                       z_buf_size, 
                                       &h_pid_filter);
    if (i4_ret != DMXR_UT_OK)
    {
        x_dbg_stmt("x_dmx_ut_pid_filter_alloc error(%d)\n", i4_ret);
        return i4_ret;
    }

    i4_ret = x_dmx_ut_pid_filter_set_nfy(h_pid_filter,
                                         dmx_ut_cli_pid_nfy,
                                         (UINT32)h_pid_filter);
    if (i4_ret != DMXR_UT_OK)
    {
        x_dbg_stmt("x_dmx_ut_pid_filter_alloc error(%d)\n", i4_ret);
        return i4_ret;
    }

    /* TODO: should not use global database */                                         
    _g_h_pid_filter = h_pid_filter;
    
    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _dmx_ut_cli_pf_free
 *
 * Description:
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _dmx_ut_cli_pf_free (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32           i4_ret;
    MPEG_2_PID_T    t_pid;
    HANDLE_T        h_pid_filter = NULL_HANDLE;
    
    if(i4_argc != 2)
    {
        x_dbg_stmt("Usage: pf_free [PID]\n");
        return CLIR_INV_CMD_USAGE;
    }

    t_pid       = (MPEG_2_PID_T) x_strtoull(pps_argv[1], NULL, 0);

    h_pid_filter = _find_handle_by_pid(t_pid);
    if (h_pid_filter != NULL_HANDLE)
    {
        i4_ret = x_dmx_ut_pid_filter_free(_g_h_pid_filter);
        if (i4_ret != DMXR_UT_OK)
        {
            x_dbg_stmt("x_dmx_ut_pid_filter_free pid[%d] error(%d)\n", t_pid, i4_ret);
            return i4_ret;
        }
    }
    else
    {
        x_dbg_stmt("can't find the pid filter for pid[%d]\n", t_pid);
    }
    
    return DMXR_UT_OK;
}



/*-----------------------------------------------------------------------------
 * Name: _dmx_ut_cli_sf_alloc
 *
 * Description:
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _dmx_ut_cli_sf_alloc (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32               i4_ret;
    MPEG_2_PID_T        t_pid;
    HANDLE_T            h_pid_filter = NULL_HANDLE;
    HANDLE_T            h_sec_filter = NULL_HANDLE;
    
    if(i4_argc != 2)
    {
        x_dbg_stmt("Usage: sf_alloc [PID]\n");
        return CLIR_INV_CMD_USAGE;
    }

    t_pid       = (MPEG_2_PID_T) x_strtoull(pps_argv[1], NULL, 0);
    h_pid_filter = _find_handle_by_pid(t_pid);
    if (h_pid_filter != NULL_HANDLE)
    {
        i4_ret = x_dmx_ut_sec_filter_alloc(h_pid_filter, &h_sec_filter);
        if (i4_ret != DMXR_UT_OK)
        {
            x_dbg_stmt("_dmx_ut_cli_sf_alloc error(%d)\n", i4_ret);
            return i4_ret;
        }
        else
        {
            x_dbg_stmt("set section filter ok, handle = %d(0x%08X)\n", h_sec_filter, h_sec_filter);
        }
    }
    else
    {
        x_dbg_stmt("can't find the pid filter for pid[%d], error code\n", t_pid);
    }
   
    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _dmx_ut_cli_sf_set
 *
 * Description: 
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _dmx_ut_cli_sf_set (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32               i4_ret;
    UINT8               ui1_tid;
    HANDLE_T            h_sec_filter = NULL_HANDLE;
    DMX_UT_SF_INFO_T    t_dmx_ut_sf_info;
    UINT8               aui1_pattern_mask[8] = PATTERN_INIT_ARRAY;
    
    if(i4_argc != 3)
    {
        x_dbg_stmt("Usage: pf_alloc [Section Filter handle] [Table Id]\n");
        return CLIR_INV_CMD_USAGE;
    }

    x_memset(&t_dmx_ut_sf_info, 0, sizeof(t_dmx_ut_sf_info));

    h_sec_filter    = (HANDLE_T) x_strtoull(pps_argv[1], NULL, 0);
    ui1_tid         = (UINT8) x_strtoull(pps_argv[2], NULL, 0);

    t_dmx_ut_sf_info.ui1_tbl_id_pattern = ui1_tid; 
    t_dmx_ut_sf_info.ui1_tbl_id_mask = 0xff;    
    t_dmx_ut_sf_info.ui1_pattern_offset = 0;
    t_dmx_ut_sf_info.ui1_pattern_offset = 0;
    t_dmx_ut_sf_info.pui1_pos_pattern = &(aui1_pattern_mask[0]);  
    t_dmx_ut_sf_info.pui1_pos_mask = &(aui1_pattern_mask[0]);
    t_dmx_ut_sf_info.pui1_neg_pattern = NULL;  
    t_dmx_ut_sf_info.pui1_neg_mask = NULL;
    t_dmx_ut_sf_info.b_check_crc = TRUE;

    i4_ret = x_dmx_ut_sec_filter_set(h_sec_filter, &t_dmx_ut_sf_info);
    if (i4_ret != DMXR_UT_OK)
    {
        x_dbg_stmt("x_dmx_ut_sec_filter_set error(%d)\n", i4_ret);
        return i4_ret;
    }
    else
    {
        x_dbg_stmt("set x_dmx_ut_sec_filter_set\n");
    }
   
    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _dmx_ut_cli_sf_enable
 *
 * Description: 
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _dmx_ut_cli_sf_enable (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_ret;
    HANDLE_T    h_sec_filter = NULL_HANDLE;
    BOOL        b_enable;
    
    if(i4_argc != 3)
    {
        x_dbg_stmt("Usage: pf_alloc [PID] [Table ID]\n");
        return CLIR_INV_CMD_USAGE;
    }

    h_sec_filter    = (HANDLE_T) x_strtoull(pps_argv[1], NULL, 0);
    b_enable        = (BOOL) x_strtoull(pps_argv[2], NULL, 0);

    i4_ret = x_dmx_ut_sec_filter_ctrl(h_sec_filter, b_enable);
    if (i4_ret != DMXR_UT_OK)
    {
        x_dbg_stmt("x_dmx_ut_sec_filter_enable error(%d)\n", i4_ret);
        return i4_ret;
    }
    else
    {
        x_dbg_stmt("%s section filter ok\n", b_enable?"enable":"disable");
    }
   
    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _dmx_ut_cli_sf_free
 *
 * Description: 
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _dmx_ut_cli_sf_free (INT32 i4_argc, const CHAR** pps_argv)
{
    INT32       i4_ret;
    HANDLE_T    h_sec_filter = NULL_HANDLE;
    
    if(i4_argc != 2)
    {
        x_dbg_stmt("Usage: pf_alloc [PID] [Table ID]\n");
        return CLIR_INV_CMD_USAGE;
    }

    h_sec_filter    = (HANDLE_T) x_strtoull(pps_argv[1], NULL, 0);

    i4_ret = x_dmx_ut_sec_filter_free(h_sec_filter);
    if (i4_ret != DMXR_UT_OK)
    {
        x_dbg_stmt("x_dmx_ut_sec_filter_free error(%d)\n", i4_ret);
        return i4_ret;
    }
    else
    {
        x_dbg_stmt("%s x_dmx_ut_sec_filter_free\n");
    }
   
    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: dmx_ut_cli_init
 *
 * Description: 
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 dmx_ut_cli_init(VOID)
{
    INT32       i4_return;

    /* Attach compression library CLI command table to CLI */
    i4_return = x_cli_attach_cmd_tbl(at_dmx_ut_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_NONE);

    if (i4_return != CLIR_OK)
    {
        return DMXR_UT_CLI_ERR;
    }

    return DMXR_UT_OK;
}



#endif /* CLI_LVL_ALL */

