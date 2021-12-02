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
 * $RCSfile: rec_util_cli.c,v $
 * $Revision:
 * $Date:
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_ATSC/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 37ebbb2d76fad2e8a90655e5f2bf2c3a $
 *
 * Description:
 *         This file implements get/set debug level APIs for record utility.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "dbg/dbg.h"
#include "dbg/u_dbg.h"
#include "cli/x_cli.h"
#include "evctx/evctx_atsc_eng/u_evctx_atsc_eng.h"
#include "evctx/evctx_dvb_eng/u_evctx_dvb_eng.h"
#include "file_mngr/x_fm.h"
#include "file_mngr/u_fm.h"
#include "file_mngr/x_fm_mfw.h"
#include "rec_util/_rec_util.h"
#include "rec_util/rec_util_thread.h"
#include "rec_util/rec_util_db.h"
#include "rec_util/rec_util_cli.h"
#include "rec_util/rec_util_utils.h"

 
 /*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL

/*#define CLI_SUPPORT_FAT32*/

static UINT32           ui4_rec_util_cli_sec_sz;
static FM_FS_INFO_T     _t_fs_info;

static INT32 _rec_util_cli_get_dbg_level(INT32 i4_argc, const CHAR**  pps_argv);
static INT32 _rec_util_cli_set_dbg_level(INT32 i4_argc, const CHAR**  pps_argv);
static INT32 _rec_util_cli_generate_dd_cmd(INT32 i4_argc, const CHAR** pps_argv);

/* rec util command table */
static CLI_EXEC_T at_rec_util_cmd_tbl[] =
{
    {
        "gdl",       
        NULL,  
        _rec_util_cli_get_dbg_level,    
        NULL,   
        "rec_util_gdl",    
        CLI_SUPERVISOR
    },
    {
        "sdl",       
        NULL,  
        _rec_util_cli_set_dbg_level,    
        NULL,    
        "rec_util_sdl",    
        CLI_SUPERVISOR
    },
    {
        "dd",       
        NULL,  
        _rec_util_cli_generate_dd_cmd,    
        NULL,    
        "generate_dd_cmd",    
        CLI_SUPERVISOR
    },
    
    END_OF_CLI_CMD_TBL
};

static CLI_EXEC_T at_rec_util_root_cmd_tbl[] =
{
    {"rec_util",     NULL,        NULL,       at_rec_util_cmd_tbl,      "REC_UTIL commands",     CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};


 /*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: rec_util_cli_get_dbg_level
 *
 * Description: This API gets the current setting of rec util debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _rec_util_cli_get_dbg_level(INT32        i4_argc,
                                        const CHAR** pps_argv)
{
    INT32       i4_return;

    i4_return = x_cli_show_dbg_level(rec_util_get_dbg_level());

    return i4_return;
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_cli_set_dbg_level
 *
 * Description: This API sets the debug level of rec util module.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _rec_util_cli_set_dbg_level(INT32        i4_argc,
                                        const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;

    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);

    if (i4_return == CLIR_OK)
    {
        if (rec_util_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_return = CLIR_CMD_EXEC_ERROR;
        }

        i4_return = CLIR_OK;
    }
    else
    {
        i4_return = CLIR_CMD_EXEC_ERROR;
    }

    x_cli_show_dbg_level(rec_util_get_dbg_level());
    
    x_dbg_stmt("[rec_util] set debug level %s\n\r", (i4_return == CLIR_OK) 
               ? "successful" : "failed");
    return i4_return;
}

extern REC_UTIL_DB_HEAD_T* rec_util_get_db_head_list(VOID);

/*internal function: to generize output position and file handle*/
static INT32 _rec_util_cli_dd_get_record_info(REC_UTIL_DB_T*        pt_db,
                                              BOOL                  b_def_valid_range,
                                              MM_BUF_RANGE_INFO_T*  pt_valid_range,
                                              HANDLE_T* ph_cb,      UINT64* pui8_cb,
                                              HANDLE_T* ph_idx,     UINT64* pui8_idx,
                                              HANDLE_T* ph_detail,  UINT64* pui8_detail)
{
    INT32   i4_ret;
    UINT32  ui4_entries;
    SIZE_T  z_cb    = ui4_rec_util_cli_sec_sz;
    SIZE_T  z_idx;
    SIZE_T  z_detail;
    SIZE_T  z_total_size;

    PVR_TICK_INDEX_T            t_start_tick_index;
    PVR_TICK_INDEX_T            t_end_tick_index;
    UINT32                      ui4_copy_sz;

    UINT64                      ui8_start_lba;
    UINT64                      ui8_end_lba;
    UINT64                      ui8_total_lba;
    UINT64                      ui8_record_beg;

    CHAR*                       ps_path     = NULL;
    CHAR*                       ps_query    = NULL;

    if( NULL == pt_db || NULL == pt_valid_range || 
        NULL == ph_cb || NULL == pui8_cb ||
        NULL == ph_idx || NULL == pui8_idx ||
        NULL == ph_detail || NULL == pui8_detail)
    {
        x_dbg_stmt("_rec_util_cli_dd_get_record_info:invalid param.\r\n");
        return CLIR_INV_ARG;
    }
    
    ui4_copy_sz = sizeof(PVR_TICK_INDEX_T);
    i4_ret = rec_util_rque_copy_entry(  pt_db->h_pvr_tick_idx_tbl, 
                                        pt_db->t_valid_range.ui4_start,
                                        (UINT8*)(&t_start_tick_index),
                                        &ui4_copy_sz);

    if(REC_UTILR_RQUE_OK != i4_ret)
    {
        x_dbg_stmt("_rec_util_cli_dd_get_record_info:copy start idx error.\r\n");
        return CLIR_CMD_EXEC_ERROR;
    }

    i4_ret = rec_util_rque_copy_entry(  pt_db->h_pvr_tick_idx_tbl, 
                                        pt_db->t_valid_range.ui4_end,
                                        (UINT8*)(&t_end_tick_index),
                                        &ui4_copy_sz);

    if(REC_UTILR_RQUE_OK != i4_ret)
    {
        x_dbg_stmt("_rec_util_cli_dd_get_record_info:copy end idx error.\r\n");
        return CLIR_CMD_EXEC_ERROR;
    }

    ui8_start_lba = t_start_tick_index.ui4_lba;
    rec_util_div_u64(   ui8_start_lba, 
                        pt_db->t_pvr_tick_cb.ui4_fifo_pkt_num, 
                        &ui8_start_lba);
                        
    
    ui8_end_lba =   t_end_tick_index.ui4_lba  + 
                    PVR_TICK_DETAIL_GET_BLKS(t_end_tick_index.pt_tick_detail);
    rec_util_div_u64(   ui8_end_lba, 
                        pt_db->t_pvr_tick_cb.ui4_fifo_pkt_num, 
                        &ui8_end_lba);      
                        
    if(!b_def_valid_range)
    {
        ui4_entries = pt_db->t_pvr_tick_cb.ui4_entry_num;
    }
    else
    {
        if(pt_valid_range->ui4_end > pt_valid_range->ui4_start)
        {
            ui4_entries = pt_valid_range->ui4_end - pt_valid_range->ui4_start + 1;
        }
        else
        {
            ui4_entries = (pt_valid_range->ui4_end + pt_db->t_pvr_tick_cb.ui4_entry_num)
                            - pt_valid_range->ui4_start + 1;
        }
    }

    do
    {
        /*z_idx           = ui4_entries * sizeof(PVR_TICK_INDEX_T);*/
        z_idx           = (pt_db->t_pvr_tick_cb.ui4_entry_num)*sizeof(PVR_TICK_INDEX_T);
        z_idx           = (SIZE_T)REC_DB_ALIGN_ADDR_FORWARD(z_idx, ui4_rec_util_cli_sec_sz);
        z_detail        = ui4_entries * (sizeof(PVR_TICK_DETAIL_T) + 
                                      2 * sizeof(PVR_PIC_INFO_T) + sizeof(UINT64));
        z_detail        = (SIZE_T)REC_DB_ALIGN_ADDR_FORWARD(z_detail, ui4_rec_util_cli_sec_sz);

        z_total_size    = (z_cb + z_idx + z_detail);
        ui8_total_lba   = ((z_total_size + ui4_rec_util_cli_sec_sz)/
                            REC_UTIL_LOGIC_BLOCK_SIZE) + 1;
                            
        if(FM_TYPE_FAT32 == _t_fs_info.e_type)
        {
            ps_path  = (CHAR*)x_mem_alloc(256);
            x_memset(ps_path, 0, 256);
            ps_query = (CHAR*)pt_db->t_cfg.t_strg_desc.ps_strg_path + 
                        x_strlen(pt_db->t_cfg.t_strg_desc.ps_strg_path) - 1;
            while(1)
            {
                if('/'== (*ps_query))
                {
                    ++ps_query;
                    break;
                }

                if(ps_query == pt_db->t_cfg.t_strg_desc.ps_strg_path)
                {
                    break;
                }
                else
                {
                    --ps_query;
                }
            }

            x_memcpy(ps_path, pt_db->t_cfg.t_strg_desc.ps_strg_path, 
                    (ps_query - pt_db->t_cfg.t_strg_desc.ps_strg_path));
            
            /*set cb*/      
            x_memcpy(ps_path + (ps_query - pt_db->t_cfg.t_strg_desc.ps_strg_path),
                     "\0", 1 );
            x_strcat(ps_path, "mtk.tcb");
            i4_ret = x_fm_mfw_open( FM_ROOT_HANDLE, ps_path, 
                                    FM_OPEN_CREATE | FM_READ_WRITE, 
                                    0666, TRUE, ph_cb);
            if (FMR_OK != i4_ret)
            {
                *ph_cb = NULL_HANDLE;
                x_dbg_stmt(("create cb file fail!\r\n"));
                i4_ret = CLIR_CMD_EXEC_ERROR;
                break;
            }
            *pui8_cb = 0;
            /*set idx*/
            x_memcpy(ps_path + (ps_query - pt_db->t_cfg.t_strg_desc.ps_strg_path),
                     "\0", 1 );
            x_strcat(ps_path, "mtk.tidx");
            i4_ret = x_fm_mfw_open( FM_ROOT_HANDLE, ps_path, 
                                    FM_OPEN_CREATE | FM_READ_WRITE, 
                                    0666, TRUE, ph_idx);
            if (FMR_OK != i4_ret)
            {
                *ph_idx = NULL_HANDLE;
                x_dbg_stmt(("create idx file fail!\r\n"));
                i4_ret = CLIR_CMD_EXEC_ERROR;
                break;
            }
            *pui8_idx= 0;
            /*set detail*/
            x_memcpy(ps_path + (ps_query - pt_db->t_cfg.t_strg_desc.ps_strg_path),
                     "\0", 1 );
            x_strcat(ps_path, "mtk.tdtl");
            i4_ret = x_fm_mfw_open( FM_ROOT_HANDLE, ps_path, 
                                    FM_OPEN_CREATE | FM_READ_WRITE, 
                                    0666, TRUE, ph_detail);
            if (FMR_OK != i4_ret)
            {
                *ph_cb = NULL_HANDLE;
                x_dbg_stmt(("create detail file fail!\r\n"));
                i4_ret = CLIR_CMD_EXEC_ERROR;
                break;
            }
            *pui8_detail= 0;
        }
        else/*by default: RAW DEVICE*/
        {
            if(ui8_end_lba > ui8_start_lba)
            {
                if(ui8_start_lba > ui8_total_lba)
                {
                    ui8_record_beg = pt_db->t_pvr_tick_cb.ui8_fifo_offset;
                }
                else if(pt_db->t_pvr_tick_cb.ui4_fifo_pkt_num - ui8_end_lba > ui8_total_lba)
                {
                    ui8_record_beg = pt_db->t_pvr_tick_cb.ui8_fifo_offset + 
                                    (ui8_end_lba * REC_UTIL_LOGIC_BLOCK_SIZE);
                                    
                    ui8_record_beg = (SIZE_T)REC_DB_ALIGN_ADDR_FORWARD(ui8_record_beg, 
                                                                ui4_rec_util_cli_sec_sz);
                }
                else
                {
                    x_dbg_stmt("_rec_util_cli_dd_get_record_info:get start pos for dump file error.\r\n");
                    return CLIR_CMD_EXEC_ERROR;
                }
            }
            else if(ui8_end_lba < ui8_start_lba)
            {
                if(ui8_start_lba - ui8_end_lba > ui8_total_lba)
                {
                    ui8_record_beg = pt_db->t_pvr_tick_cb.ui8_fifo_offset + 
                                    (ui8_end_lba * REC_UTIL_LOGIC_BLOCK_SIZE);
                                    
                    ui8_record_beg = (SIZE_T)REC_DB_ALIGN_ADDR_FORWARD(ui8_record_beg, 
                                                                ui4_rec_util_cli_sec_sz);
                }
                else
                {
                    x_dbg_stmt("_rec_util_cli_dd_get_record_info:get start pos for dump file error.\r\n");
                    return CLIR_CMD_EXEC_ERROR;
                }
            }
            else
            {
                x_dbg_stmt("_rec_util_cli_dd_get_record_info:get start pos for dump file error.\r\n");
                return CLIR_CMD_EXEC_ERROR;
            }

            *ph_cb      = pt_db->h_file;
            *ph_idx     = pt_db->h_file;
            *ph_detail  = pt_db->h_file;

            *pui8_cb    = ui8_record_beg;
            *pui8_idx   = (*pui8_cb) + ui4_rec_util_cli_sec_sz;
            *pui8_detail= (*pui8_idx) + z_idx;
        }            
    }while(0);

    if(NULL != ps_path)
    {
        x_mem_free(ps_path);
    }
    return  CLIR_OK;

}

static INT32 _rec_util_cli_dd_tick_cb(  
                                        REC_UTIL_DB_T*                  pt_db,
                                        HANDLE_T                        h_handle,
                                        REC_UTIL_CLI_DUMP_TICK_CB_T*    pt_tick_cb,
                                        UINT64                          ui8_start_pos, 
                                        UINT64*                         pui8_end_pos)
{
    INT32               i4_ret = CLIR_OK;
    UINT32              ui4_write_sz;
    REC_UTIL_FM_CACHE_T t_fm_cache;
    
    if( NULL_HANDLE == h_handle || 
        NULL == pt_db || 
        NULL == pt_tick_cb ||
        NULL == pui8_end_pos)
    {
        x_dbg_stmt("_rec_util_cli_dd_tick_cb invalid param!\r\n");
        return CLIR_INV_ARG;
    }

    i4_ret = rec_util_alloc_fm_cache(&t_fm_cache, ui4_rec_util_cli_sec_sz);
    if(CLIR_OK != i4_ret)
    {
        x_dbg_stmt("_rec_util_cli_dd_tick_cb:alloc cache fail!\r\n");
        return CLIR_CMD_EXEC_ERROR;
    }
    /* dump tick cb file: 
    (1) Tick-Period 
    (2) Tick-index table entry number 
    (3) valid range start  
    (4) valid range end */
    do
    {
        i4_ret = x_fm_mfw_lseek(h_handle, 
                            (INT64)ui8_start_pos, FM_SEEK_BGN, 
                            pui8_end_pos);
        if(FMR_OK != i4_ret)
        {
            x_dbg_stmt("_rec_util_cli_dd_tick_cb seek start pos error!\r\n");
            i4_ret = CLIR_CMD_EXEC_ERROR;
            break;
        }

        i4_ret = rec_util_cli_write_with_cache(h_handle, &t_fm_cache,
                                                TRUE, (VOID*)(pt_tick_cb), 
                                                sizeof(REC_UTIL_CLI_DUMP_TICK_CB_T), 
                                                &ui4_write_sz);
        
                            
        if(CLIR_OK != i4_ret || ui4_write_sz != sizeof(REC_UTIL_CLI_DUMP_TICK_CB_T))
        {
            x_dbg_stmt("_rec_util_cli_dd_tick_cb dump tick cb error!\r\n");
            i4_ret = CLIR_CMD_EXEC_ERROR;
            break;
        }

        i4_ret = x_fm_mfw_lseek(h_handle, 
                            0, FM_SEEK_CUR, 
                            pui8_end_pos);
        if(FMR_OK != i4_ret)
        {
            x_dbg_stmt("_rec_util_cli_dd_tick_cb seek start pos error!\r\n");
            i4_ret = CLIR_CMD_EXEC_ERROR;
            break;
        }
    }while(0);

    rec_util_free_fm_cache(&t_fm_cache);
     
    return i4_ret;
}

static BOOL  _rec_util_cli_idx_in_valid_range(
                                            UINT32  ui4_start,
                                            UINT32  ui4_end,
                                            UINT32  ui4_idx,
                                            UINT32  ui4_total)
{
    if(ui4_idx >= ui4_total)
    {
        return FALSE;
    }
    
    if(ui4_start <= ui4_end)
    {
        if(ui4_idx < ui4_start || ui4_idx > ui4_end)
        {
            return FALSE;
        }
    }
    else
    {
        if(ui4_idx < ui4_start && ui4_idx > ui4_end)
        {
            return FALSE;
        }
    }

    return TRUE;
}

static INT32 _rec_util_cli_get_tick_detail_len( PVR_TICK_DETAIL_T*  pt_tick_detail,
                                                UINT32              ui4_flag,
                                                UINT32*             pui4_len)
{
    UINT32 ui4_entry_num;
    if(NULL == pt_tick_detail || NULL == pui4_len)
    {
        x_dbg_stmt("_rec_util_cli_get_tick_detail_len:invalid param!\r\n");
        return CLIR_INV_ARG;
    }

    /*tick ui4_tick_num + ui4_blks_entries + */
    (*pui4_len)  = sizeof(PVR_TICK_DETAIL_T) ;
    /*pic info*/
    ui4_entry_num = PVR_TICK_DETAIL_GET_ENTRIES(pt_tick_detail);
    if(ui4_entry_num > 0)
    {
        (*pui4_len) += (sizeof(PVR_PIC_INFO_T) * ui4_entry_num);
    }
    /*ui8_system_time*/
    if (ui4_flag & REC_UTIL_TICK_DATA_FLAG_TIME_VALID)
    {
        (*pui4_len) += sizeof(TIME_T);  
    }
    return CLIR_OK;
}

static VOID _rec_util_cli_get_dump_info(
                                        HANDLE_T        h_handle,
                                        UINT64          ui8_start_pos,
                                        UINT64          ui8_end_pos,
                                        UINT64*         pui8_sec_cnt,
                                        UINT64*         pui8_sec_skip)
{
    UINT64      ui8_sec_start;
    UINT64      ui8_sec_end;
    UINT64      ui8_remainer;
    
    if( NULL_HANDLE == h_handle ||
        NULL == pui8_sec_cnt ||
        NULL == pui8_sec_skip)
    {
        x_dbg_stmt("_rec_util_cli_get_dump_info:invalid param!\r\n");
        return ;
    }

    ui8_sec_start = rec_util_div_u64(   ui8_start_pos, 
                                        ui4_rec_util_cli_sec_sz, 
                                        &ui8_remainer);
                                        
    ui8_sec_end   = rec_util_div_u64(   ui8_end_pos, 
                                        ui4_rec_util_cli_sec_sz, 
                                        &ui8_remainer);
    if(0 != ui8_remainer)
    {
        ++ui8_sec_end;
    }

    *pui8_sec_cnt   = ui8_sec_end - ui8_sec_start;
    *pui8_sec_skip  = ui8_sec_start;
}
                                        
static INT32 _rec_util_cli_dd_tick_index(  
                                        REC_UTIL_DB_T*          pt_db,
                                        HANDLE_T                h_tick_idx,
                                        HANDLE_T                h_tick_detail,
                                        MM_BUF_RANGE_INFO_T*    pt_valid_range, 
                                        UINT64                  ui8_start_lba,
                                        UINT64                  ui8_tick_idx_start,
                                        UINT64                  ui8_tick_detail_start,
                                        UINT64*                 pui8_tick_idx_end,
                                        UINT64*                 pui8_tick_detail_end,
                                        UINT64*                 pui8_pvr_pre_ofst)
{
    INT32                           i4_ret = CLIR_OK;
    UINT32                          ui4_idx;
    PVR_TICK_INDEX_T                t_tick_index;
    BOOL                            b_in_valid_range;
    UINT32                          ui4_index_sz;
    UINT32                          ui4_detail_sz;
    PVR_TICK_DETAIL_T*              pt_tick_detail;
    UINT32                          ui4_write_sz;
    UINT64                          ui8_max_lba;
    UINT64                          ui8_cur_lba;
    UINT64                          ui8_pre_ofst;
   
    REC_UTIL_FM_CACHE_T             t_mm_idx_cache;
    REC_UTIL_FM_CACHE_T             t_mm_detail_cache;

    UINT64                          ui8_tick_idx_pos;
    UINT64                          ui8_tick_detail_pos;

    if( NULL == pt_db || 
        NULL == pt_valid_range ||
        NULL_HANDLE == h_tick_idx || 
        NULL_HANDLE == h_tick_detail ||
        NULL == pui8_tick_idx_end ||
        NULL == pui8_tick_detail_end||
        NULL == pui8_pvr_pre_ofst)
    {
        x_dbg_stmt("_rec_util_cli_dd_tick_index:invalid param!\r\n");
        return CLIR_INV_ARG;
    }

    i4_ret = rec_util_alloc_fm_cache(&t_mm_idx_cache, ui4_rec_util_cli_sec_sz);
    if(CLIR_OK != i4_ret)
    {
        x_dbg_stmt("_rec_util_cli_dd_tick_cb:alloc cache fail!\r\n");
        return CLIR_CMD_EXEC_ERROR;
    }

    i4_ret = rec_util_alloc_fm_cache(&t_mm_detail_cache, ui4_rec_util_cli_sec_sz);
    if(CLIR_OK != i4_ret)
    {  
        x_dbg_stmt("_rec_util_cli_dd_tick_cb:alloc cache fail!\r\n");
        rec_util_free_fm_cache(&t_mm_idx_cache);
        return CLIR_CMD_EXEC_ERROR;
    }
    
    do
    {
        ui8_max_lba             = pt_db->t_pvr_tick_cb.ui4_fifo_pkt_num;
        if(ui8_start_lba >= ui8_max_lba)
        {
            x_dbg_stmt("_rec_util_cli_dd_tick_index:start lba too large!\r\n");
            i4_ret = CLIR_CMD_EXEC_ERROR;
            break;
        }

        if(FM_TYPE_FAT32 == _t_fs_info.e_type)
        {
            ui8_pre_ofst = (ui8_start_lba*REC_UTIL_LOGIC_BLOCK_SIZE)+
                            pt_db->t_pvr_tick_cb.ui8_fifo_offset;
        }
        else
        {
            rec_util_div_u64(   (ui8_start_lba*REC_UTIL_LOGIC_BLOCK_SIZE)+
                                pt_db->t_pvr_tick_cb.ui8_fifo_offset,   
                                ui4_rec_util_cli_sec_sz, &ui8_pre_ofst);
        }
                            
        *pui8_pvr_pre_ofst = (ui8_pre_ofst);
                            
        *pui8_tick_idx_end      = ui8_tick_idx_start;
        *pui8_tick_detail_end   = ui8_tick_detail_start;
        ui8_tick_idx_pos        = ui8_tick_idx_start;
        ui8_tick_detail_pos     = ui8_tick_detail_start;
        
        for(ui4_idx = 0; ui4_idx < pt_db->t_pvr_tick_cb.ui4_entry_num; ++ ui4_idx)
        {
            b_in_valid_range = _rec_util_cli_idx_in_valid_range(pt_valid_range->ui4_start, 
                                                                pt_valid_range->ui4_end, 
                                                                ui4_idx, 
                                                               pt_db->t_pvr_tick_cb.ui4_entry_num);

            ui4_index_sz = sizeof(PVR_TICK_INDEX_T);

            if(!b_in_valid_range)
            {
                x_memset((VOID*)(&t_tick_index), 0, ui4_index_sz);
            }
            else
            {
                i4_ret = rec_util_rque_copy_entry(  pt_db->h_pvr_tick_idx_tbl, 
                                                ui4_idx, (UINT8*)(&t_tick_index), 
                                                &ui4_index_sz);
                if(REC_UTILR_RQUE_OK != i4_ret)
                {
                    x_dbg_stmt("_rec_util_cli_dd_tick_index:get tick index error!\r\n");
                    i4_ret = CLIR_CMD_EXEC_ERROR;
                    break;
                }
            }

            /*write tick detail*/
            if(b_in_valid_range)
            {
                pt_tick_detail = t_tick_index.pt_tick_detail;
                if(NULL == pt_tick_detail)
                {
                    x_dbg_stmt("_rec_util_cli_dd_tick_index:tick detail null!\r\n");
                    i4_ret = CLIR_CMD_EXEC_ERROR;
                    break;
                }

                i4_ret = _rec_util_cli_get_tick_detail_len( pt_tick_detail, 
                                                            t_tick_index.ui4_flags,
                                                            &ui4_detail_sz);
                if(CLIR_OK != i4_ret)
                {
                    x_dbg_stmt( "_rec_util_cli_dd_tick_index:"
                                "get tick detail len error!\r\n");
                    i4_ret = CLIR_CMD_EXEC_ERROR;
                    break;
                }
                ui4_detail_sz = REC_DB_ALIGN_DATA(ui4_detail_sz);

                /*replace pointer with offset value*/
                t_tick_index.pt_tick_detail = (PVR_TICK_DETAIL_T*)((UINT32)((*pui8_tick_detail_end) - 
                                                                            ui8_tick_detail_start));

                i4_ret = x_fm_mfw_lseek(h_tick_detail, ui8_tick_detail_pos, 
                                    FM_SEEK_BGN, &ui8_tick_detail_pos);
                if(FMR_OK != i4_ret)
                {
                    x_dbg_stmt( "_rec_util_cli_dd_tick_index:"
                                "tick detail seek pos error!\r\n");
                    i4_ret = CLIR_CMD_EXEC_ERROR;
                    break;
                }

                i4_ret = rec_util_cli_write_with_cache(h_tick_detail, &t_mm_detail_cache,
                                                        FALSE, (VOID*)pt_tick_detail,
                                                        ui4_detail_sz, &ui4_write_sz);        
                if(CLIR_OK != i4_ret)
                {
                    x_dbg_stmt( "_rec_util_cli_dd_tick_index:"
                                "tick detail write error!\r\n");
                    i4_ret = CLIR_CMD_EXEC_ERROR;
                    break;      
                }
                (*pui8_tick_detail_end) += ui4_detail_sz;
                ui8_tick_detail_pos += ui4_write_sz;

                /*replace lba with new lba*/
                ui8_cur_lba = t_tick_index.ui4_lba;
                rec_util_div_u64(ui8_cur_lba, ui8_max_lba, &ui8_cur_lba);

                t_tick_index.ui4_lba = (UINT32)((ui8_cur_lba >= ui8_start_lba) ? 
                                       (ui8_cur_lba - ui8_start_lba):
                                       (ui8_cur_lba + ui8_max_lba - ui8_start_lba));      
            }
            
            /*write tick index*/
            i4_ret = x_fm_mfw_lseek(h_tick_idx, ui8_tick_idx_pos, 
                                FM_SEEK_BGN, &ui8_tick_idx_pos);
            if(FMR_OK != i4_ret)
            {
                x_dbg_stmt( "_rec_util_cli_dd_tick_index:"
                            "tick idx seek pos error!\r\n");
                i4_ret = CLIR_CMD_EXEC_ERROR;
                break;
            }
            i4_ret = rec_util_cli_write_with_cache(h_tick_idx, &t_mm_idx_cache,
                                                    FALSE, (VOID*)(&t_tick_index),
                                                    ui4_index_sz, &ui4_write_sz);               
            if(CLIR_OK != i4_ret)
            {
                x_dbg_stmt( "_rec_util_cli_dd_tick_index:"
                            "tick index write error!\r\n");
                i4_ret = CLIR_CMD_EXEC_ERROR;
                break;
            }
            (*pui8_tick_idx_end) += ui4_index_sz;
            ui8_tick_idx_pos += ui4_write_sz;
            
        }

        if(CLIR_OK != i4_ret)
        {
            break;
        }
        /*clear tick detail cache data*/
        i4_ret = x_fm_mfw_lseek(h_tick_detail, ui8_tick_detail_pos, 
                            FM_SEEK_BGN, &ui8_tick_detail_pos);
        if(FMR_OK != i4_ret)
        {
            x_dbg_stmt( "_rec_util_cli_dd_tick_index:"
                        "tick detail seek pos error!\r\n");
            i4_ret = CLIR_CMD_EXEC_ERROR;
            break;
        }

        i4_ret = rec_util_cli_write_with_cache(h_tick_detail, &t_mm_detail_cache,
                                                TRUE, NULL,
                                                0, &ui4_write_sz);        
        if(CLIR_OK != i4_ret)
        {
            x_dbg_stmt( "_rec_util_cli_dd_tick_index:"
                        "tick detail write error!\r\n");
            i4_ret = CLIR_CMD_EXEC_ERROR;
            break;      
        }                

        ui8_tick_detail_pos += ui4_write_sz;
        if(ui8_tick_detail_pos != (*pui8_tick_detail_end))
        {
            x_dbg_stmt( "_rec_util_cli_dd_tick_index:"
                        "tick detail end pos error!\r\n");
            i4_ret = CLIR_CMD_EXEC_ERROR;
            break;      
        }
        /*clear tick idx cache data*/
        i4_ret = x_fm_mfw_lseek(h_tick_idx, ui8_tick_idx_pos, 
                            FM_SEEK_BGN, &ui8_tick_idx_pos);
        if(FMR_OK != i4_ret)
        {
            x_dbg_stmt( "_rec_util_cli_dd_tick_index:"
                        "tick idx seek pos error!\r\n");
            i4_ret = CLIR_CMD_EXEC_ERROR;
            break;
        }
        i4_ret = rec_util_cli_write_with_cache(h_tick_idx, &t_mm_idx_cache,
                                                TRUE, NULL,
                                                0, &ui4_write_sz);               
        if(CLIR_OK != i4_ret)
        {
            x_dbg_stmt( "_rec_util_cli_dd_tick_index:"
                        "tick index write error!\r\n");
            i4_ret = CLIR_CMD_EXEC_ERROR;
            break;
        }
        ui8_tick_idx_pos += ui4_write_sz;
        if(ui8_tick_idx_pos != (*pui8_tick_idx_end))
        {
            x_dbg_stmt( "_rec_util_cli_dd_tick_index:"
                        "tick index end pos error!\r\n");
            i4_ret = CLIR_CMD_EXEC_ERROR;
            break;
        }
        
    }while(0);

    rec_util_free_fm_cache(&t_mm_idx_cache);
    rec_util_free_fm_cache(&t_mm_detail_cache);
    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _rec_util_cli_generate_dd_cmd
 *
 * Description: This API generate dd command which used to dump raw recorded ts data.
 *              in a valid range
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _rec_util_cli_generate_dd_cmd(INT32           i4_argc,
                                           const CHAR**    pps_argv)
{
    INT32                       i4_ret                = CLIR_OK;
    BOOL                        b_def_valid_range     = FALSE;
    HANDLE_T                    h_rec_util;
    REC_UTIL_DB_HEAD_T*         db_head               = NULL;
    REC_UTIL_HDL_LINK_T*        pt_client_head        = NULL;
    REC_UTIL_DB_T*              pt_db             = NULL;
    REC_UTIL_CLIENT_T*          pt_client_obj         = NULL;
    VOID*                       pv_obj                = NULL;
    HANDLE_TYPE_T               e_handle_type;
    
    MM_BUF_RANGE_INFO_T         t_valid_range;
    REC_UTIL_CLI_DUMP_TICK_CB_T t_dump_tick_cb;
    REC_UTIL_SHARE_DATA_T       t_share_data;
    PVR_TICK_INDEX_T            t_start_tick_index;
    PVR_TICK_INDEX_T            t_end_tick_index;

    HANDLE_T                    h_tick_cb;
    HANDLE_T                    h_tick_idx;
    HANDLE_T                    h_tick_detail;
    
    UINT64                      ui8_tick_cb_start;
    UINT64                      ui8_tick_idx_start;
    UINT64                      ui8_tick_detail_start;
    UINT64                      ui8_tick_cb_end;
    UINT64                      ui8_tick_idx_end    = 0;
    UINT64                      ui8_tick_detail_end = 0;
    
    UINT64                      ui8_start_lba;
    UINT64                      ui8_end_lba;
    UINT64                      ui8_max_lba;
    
    UINT64                      ui8_start_file_pos;
    UINT64                      ui8_end_file_pos = 0;

    UINT64                      ui8_skip_blk_cnt;
    UINT64                      ui8_seek_blk_cnt;
    UINT64                      ui8_blk_cnt = 0;

    /*1. Check parameters*/
    if ((0 != i4_argc) && (2 != i4_argc) && (4 != i4_argc))
    {
        x_dbg_stmt("_rec_util_cli_generate_dd_cmd Usage:dd [db_handle] [start_idx end_idx]\r\n");
        return CLIR_INV_ARG;
    }
    
    /*2. If given DB obejct handle*/
    if ((2 == i4_argc) || (4 == i4_argc))
    {
        h_rec_util = (HANDLE_T)x_strtoll(pps_argv[1], NULL, 10);
        i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
        if ((HR_OK != i4_ret) 
            || (RU_HT_CLIENT_HANDLER != e_handle_type)
            || (NULL == pv_obj))
        {   
            x_dbg_stmt("_rec_util_cli_generate_dd_cmd:get handle obj error!\r\n");
            return CLIR_CMD_EXEC_ERROR;
        }
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        db_head = pt_client_obj->pt_rec_db_head;

        if(4 == i4_argc)
        {
            b_def_valid_range = TRUE;
        }
    }
    /*The first db obejct of the rec_util module*/
    else
    {
        db_head = rec_util_get_db_head_list();
        
    }

    if (db_head != NULL)
    {
        pt_db = db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            x_dbg_stmt("_rec_util_cli_generate_dd_cmd:db obj is null\r\n");
            return CLIR_CMD_EXEC_ERROR;
        }
    }
    else
    {
        x_dbg_stmt("_rec_util_cli_generate_dd_cmd:get db head error\r\n");
        return CLIR_CMD_EXEC_ERROR;
    }
    
    pt_client_head = DLIST_HEAD(&(db_head->t_client_handle_list));
    h_rec_util = pt_client_head->h_rec_util;

    ui4_rec_util_cli_sec_sz = pt_db->t_cfg.t_strg_desc.ui4_sec_size;
    /*3. get valid range*/
    t_dump_tick_cb.ui4_magic_num   = REC_UTIL_CLI_DUMP_TICK_CB_MAGIC_NUM;
    t_dump_tick_cb.t_sess_key      = pt_db->t_cfg.t_sess_key;
    t_dump_tick_cb.ui4_tick_period = 500;
    t_dump_tick_cb.ui4_tick_num    = pt_db->t_pvr_tick_cb.ui4_entry_num;
    ui8_seek_blk_cnt               = 0;
    
    if(b_def_valid_range)
    {
        t_dump_tick_cb.ui4_valid_range_start = x_strtoull(pps_argv[2], NULL, 10);
        if(!rec_util_rque_chk_index_in_range(pt_db->h_pvr_tick_idx_tbl,
                                             t_dump_tick_cb.ui4_valid_range_start))
        {
            x_dbg_stmt("_rec_util_cli_generate_dd_cmd:start index not in range!\r\n");
            return CLIR_CMD_EXEC_ERROR;
        }
        
        t_dump_tick_cb.ui4_valid_range_end = x_strtoull(pps_argv[3], NULL, 10);
        if(!rec_util_rque_chk_index_in_range(pt_db->h_pvr_tick_idx_tbl,
                                             t_dump_tick_cb.ui4_valid_range_end))
        {
            x_dbg_stmt("_rec_util_cli_generate_dd_cmd:end index not in range!\r\n");
            return CLIR_CMD_EXEC_ERROR;
        }

        if(t_dump_tick_cb.ui4_valid_range_start == t_dump_tick_cb.ui4_valid_range_end)
        {
            x_dbg_stmt("start range idx equal to end range idx!\r\n");
            return CLIR_CMD_EXEC_ERROR;
        }
                                    
        t_valid_range.ui4_start = t_dump_tick_cb.ui4_valid_range_start;
        t_valid_range.ui4_end   = t_dump_tick_cb.ui4_valid_range_end;        
    }
    else
    {
        i4_ret = rec_util_get_valid_range(h_rec_util, &t_valid_range);
        if (i4_ret != REC_UTILR_OK)
        {
            x_dbg_stmt("_rec_util_cli_generate_dd_cmd:get valid range error(%d)\r\n", i4_ret);
            return CLIR_CMD_EXEC_ERROR;
        }
        
        t_dump_tick_cb.ui4_valid_range_start = t_valid_range.ui4_start;
        t_dump_tick_cb.ui4_valid_range_end   = t_valid_range.ui4_end;
    }
    
    /*4.get share data*/
    i4_ret = rec_util_get_shared_data(h_rec_util, &t_share_data);
    if (i4_ret != REC_UTILR_OK)
    {
        x_dbg_stmt("_rec_util_cli_generate_dd_cmd:get share data error(%d)\r\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }
    
    /*get start tick index*/
    i4_ret = rec_util_get_tick_index(h_rec_util,
                                     t_valid_range.ui4_start,
                                     &t_start_tick_index);
    if (i4_ret != REC_UTILR_OK)
    {
        x_dbg_stmt("_rec_util_cli_generate_dd_cmd:get start tick index error(%d)\r\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }
    ui8_start_lba = t_start_tick_index.ui4_lba;
        
    /*get end tick index and pvr lba range*/
    i4_ret = rec_util_get_tick_index(h_rec_util,
                                     t_valid_range.ui4_end,
                                     &t_end_tick_index);
    if (i4_ret != REC_UTILR_OK)
    {
        x_dbg_stmt("_rec_util_cli_generate_dd_cmd:get end tick index error(%d)\r\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }
    
    if (t_end_tick_index.pt_tick_detail != NULL)
    {
        ui8_end_lba =   t_end_tick_index.ui4_lba  + 
                        PVR_TICK_DETAIL_GET_BLKS(t_end_tick_index.pt_tick_detail);
    }
    else
    {
        x_dbg_stmt("_rec_util_cli_generate_dd_cmd:get end lba error(%d)\r\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    ui8_max_lba = t_share_data.t_tick_cb.ui4_fifo_pkt_num;
    rec_util_div_u64(ui8_start_lba, ui8_max_lba, &ui8_start_lba);
    rec_util_div_u64(ui8_end_lba, ui8_max_lba, &ui8_end_lba);

    i4_ret = x_fm_get_fs_info(  FM_ROOT_HANDLE, pt_db->t_cfg.t_strg_desc.ps_strg_path,
                                &_t_fs_info);
    if(FMR_OK != i4_ret)
    {
        x_dbg_stmt("_rec_util_cli_generate_dd_cmd:get fs info error(%d)!\r\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;   
    }
    /*get dump file record handle and pos*/ 
    i4_ret = _rec_util_cli_dd_get_record_info(pt_db, b_def_valid_range, &t_valid_range, 
                                              &h_tick_cb, &ui8_tick_cb_start, 
                                              &h_tick_idx, &ui8_tick_idx_start,
                                              &h_tick_detail, &ui8_tick_detail_start);
    if(CLIR_OK != i4_ret)
    {
        x_dbg_stmt("_rec_util_cli_generate_dd_cmd:get dump file handle and pos error!\r\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    
    
    /*dump tick idx and detail file*/
    i4_ret = _rec_util_cli_dd_tick_index(   pt_db, h_tick_idx,
                                            h_tick_detail,
                                            &t_valid_range,
                                            ui8_start_lba,
                                            ui8_tick_idx_start,
                                            ui8_tick_detail_start,
                                            &ui8_tick_idx_end,
                                            &ui8_tick_detail_end,
                                            &(t_dump_tick_cb.ui8_pvr_pre_ofst));
    if(CLIR_OK != i4_ret)
    {
        x_dbg_stmt("_rec_util_cli_generate_dd_cmd: dump tick index error!\r\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    /* dump tick cb file*/
    i4_ret = _rec_util_cli_dd_tick_cb(  pt_db, h_tick_cb, 
                                        &t_dump_tick_cb, 
                                        ui8_tick_cb_start,  
                                        &ui8_tick_cb_end);
    if(CLIR_OK != i4_ret)
    {
        x_dbg_stmt("_rec_util_cli_generate_dd_cmd: dump tick cb error!\r\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    /*get dump command*/
    if(FM_TYPE_FAT32 == _t_fs_info.e_type)
    {
        /*close file handle*/
        i4_ret = x_fm_mfw_close(h_tick_cb);
        if(FMR_OK != i4_ret)
        {
            x_dbg_stmt("_rec_util_cli_generate_dd_cmd: close cb handle fail!\n");  
            return CLIR_CMD_EXEC_ERROR;
        }
        
        i4_ret = x_fm_mfw_close(h_tick_idx);
        if(FMR_OK != i4_ret)
        {
            x_dbg_stmt("_rec_util_cli_generate_dd_cmd: close idx handle fail!\n");  
            return CLIR_CMD_EXEC_ERROR;
        }
        
        i4_ret = x_fm_mfw_close(h_tick_detail);
        if(FMR_OK != i4_ret)
        {
            x_dbg_stmt("_rec_util_cli_generate_dd_cmd: close detail handle fail!\n");  
            return CLIR_CMD_EXEC_ERROR;
        }

        x_dbg_stmt( "Please connect the usb disk to pc and copy files.\r\n"
                    "(1) tshift.bin\r\n"
                    "(2) mtk.tcb\r\n"
                    "(3) mtk.tidx\r\n"
                    "(4) mtk.tdtl\r\n"
                    "\r\nTo dump mtk.pvr file:\r\n"
                    "(5) Please copy dd.exe to C:\\pvr_dump\\ folder firstly.\r\n"
                    "(6) Open windows console.\r\n"
                    "(7) Change to C:\\pvr_dump\\ folder under windows console.\r\n"
                    "(8)\r\n");
    }
    else/*by default: RAW DEVICE*/
    {
        x_dbg_stmt( "(1) Please copy dd.exe to C:\\pvr_dump\\ folder firstly.\r\n"
                    "(2) Open windows console.\r\n"
                    "(3) Change to C:\\pvr_dump\\ folder under windows console.\r\n"
                    "(4) \r\n");
                    
        _rec_util_cli_get_dump_info(h_tick_cb, 
                                    ui8_tick_cb_start, 
                                    ui8_tick_cb_end, 
                                    &ui8_blk_cnt, 
                                    &ui8_skip_blk_cnt);

        x_dbg_stmt( "C:\\pvr_dump>dd.exe if=\\\\?\\Device\\Harddisk1\\Partition0"
                    " of=mtk.tcb count=%llu skip=%llu seek=%llu\r\n",
                    ui8_blk_cnt, ui8_skip_blk_cnt, ui8_seek_blk_cnt);  
                           
        _rec_util_cli_get_dump_info(h_tick_idx, 
                                    ui8_tick_idx_start, 
                                    ui8_tick_idx_end, 
                                    &ui8_blk_cnt, 
                                    &ui8_skip_blk_cnt);

        x_dbg_stmt( "C:\\pvr_dump>dd.exe if=\\\\?\\Device\\Harddisk1\\Partition0"
                    " of=mtk.tidx count=%llu skip=%llu seek=%llu\r\n",
                    ui8_blk_cnt, ui8_skip_blk_cnt, ui8_seek_blk_cnt);

        _rec_util_cli_get_dump_info(h_tick_detail, 
                                    ui8_tick_detail_start, 
                                    ui8_tick_detail_end, 
                                    &ui8_blk_cnt, 
                                    &ui8_skip_blk_cnt);

        x_dbg_stmt( "C:\\pvr_dump>dd.exe if=\\\\?\\Device\\Harddisk1\\Partition0"
                    " of=mtk.tdtl count=%llu skip=%llu seek=%llu\r\n",
                    ui8_blk_cnt, ui8_skip_blk_cnt, ui8_seek_blk_cnt);   
    }            
    

    /*dump pvr info*/
    /*TS-FIFO doesn't wrap over*/
    if (ui8_end_lba > ui8_start_lba)
    {
        ui8_start_file_pos = pt_db->t_pvr_tick_cb.ui8_fifo_offset + 
                            ui8_start_lba * REC_UTIL_LOGIC_BLOCK_SIZE;

        ui8_end_file_pos = pt_db->t_pvr_tick_cb.ui8_fifo_offset + 
                            ui8_end_lba * REC_UTIL_LOGIC_BLOCK_SIZE; 

        _rec_util_cli_get_dump_info(pt_db->h_file, 
                                    ui8_start_file_pos, 
                                    ui8_end_file_pos, 
                                    &ui8_blk_cnt, 
                                    &ui8_skip_blk_cnt);

        if(FM_TYPE_FAT32 == _t_fs_info.e_type)
        {
            x_dbg_stmt( "C:\\pvr_dump>dd.exe if=%s" 
                        " of=mtk.pvr count=%llu skip=%llu seek=%llu\r\n",
                        pt_db->t_cfg.t_strg_desc.ps_strg_path,
                        ui8_blk_cnt, ui8_skip_blk_cnt, ui8_seek_blk_cnt); 
        }
        else
        {
            x_dbg_stmt( "C:\\pvr_dump>dd.exe if=\\\\?\\Device\\Harddisk1\\Partition0" 
                        " of=mtk.pvr count=%llu skip=%llu seek=%llu\r\n",
                        ui8_blk_cnt, ui8_skip_blk_cnt, ui8_seek_blk_cnt); 
        }
    }
    /*TS-FIFO wrap over*/
    else if (ui8_end_lba < ui8_start_lba)
    {
        ui8_start_file_pos = pt_db->t_pvr_tick_cb.ui8_fifo_offset + 
                            ui8_start_lba * REC_UTIL_LOGIC_BLOCK_SIZE;

        ui8_end_file_pos = pt_db->t_pvr_tick_cb.ui8_fifo_offset + 
                            ui8_max_lba * REC_UTIL_LOGIC_BLOCK_SIZE; 

        _rec_util_cli_get_dump_info(pt_db->h_file, 
                                    ui8_start_file_pos, 
                                    ui8_end_file_pos, 
                                    &ui8_blk_cnt, 
                                    &ui8_skip_blk_cnt);

        if(FM_TYPE_FAT32 == _t_fs_info.e_type)
        {
            x_dbg_stmt( "C:\\pvr_dump>dd.exe if=%s"
                        " of=mtk.pvr count=%llu skip=%llu seek=%llu\r\n",
                        pt_db->t_cfg.t_strg_desc.ps_strg_path,
                        ui8_blk_cnt, ui8_skip_blk_cnt, ui8_seek_blk_cnt);
        }
        else
        {
            x_dbg_stmt( "C:\\pvr_dump>dd.exe if=\\\\?\\Device\\Harddisk1\\Partition0"
                        " of=mtk.pvr count=%llu skip=%llu seek=%llu\r\n",
                        ui8_blk_cnt, ui8_skip_blk_cnt, ui8_seek_blk_cnt);
        }

        ui8_seek_blk_cnt = ui8_blk_cnt;  

        
        ui8_start_file_pos = pt_db->t_pvr_tick_cb.ui8_fifo_offset;
        
        ui8_end_file_pos = pt_db->t_pvr_tick_cb.ui8_fifo_offset + 
                            ui8_end_lba * REC_UTIL_LOGIC_BLOCK_SIZE; 

        _rec_util_cli_get_dump_info(pt_db->h_file, 
                                    ui8_start_file_pos, 
                                    ui8_end_file_pos, 
                                    &ui8_blk_cnt, 
                                    &ui8_skip_blk_cnt);
        if(FM_TYPE_FAT32 == _t_fs_info.e_type)
        {
            x_dbg_stmt( "C:\\pvr_dump>dd.exe if=%s"
                        " of=mtk.pvr count=%llu skip=%llu seek=%llu\r\n",
                        pt_db->t_cfg.t_strg_desc.ps_strg_path,
                        ui8_blk_cnt, ui8_skip_blk_cnt, ui8_seek_blk_cnt); 
        }
        else
        {
            x_dbg_stmt( "C:\\pvr_dump>dd.exe if=\\\\?\\Device\\Harddisk1\\Partition0"
                        " of=mtk.pvr count=%llu skip=%llu seek=%llu\r\n",
                        ui8_blk_cnt, ui8_skip_blk_cnt, ui8_seek_blk_cnt); 
        }    
    }
    else
    {
        x_dbg_stmt("No valid range\r\n");
        return CLIR_OK;
    }

    if(FM_TYPE_FAT32 == _t_fs_info.e_type)
    {
         x_dbg_stmt("\r\nPlease check and replace"
                   "\"%s\" with a correct input file path",
                   pt_db->t_cfg.t_strg_desc.ps_strg_path);
    }
    else
    {
        x_dbg_stmt("\r\n(5) Please check that the file \"mtk.tcb\" was leaded by 0xAAAAAAAA.\r\n"
                   "If not, please use \"dd.exe --list\" to check and replace"
                   "\"if=\\\\?\\Device\\Harddisk1\\Partition0\" to a correct location");
    }
        
    return i4_ret;
}

#endif

/*-----------------------------------------------------------------------------
 * Name: rec_util_cli_init
 *
 * Description: This API initializes record utility CLI component.
 *
 * Inputs:  ppt_command_array
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK
 *          CLIR_INV_ARG
 ----------------------------------------------------------------------------*/
INT32 rec_util_cli_init(VOID)
{
    #ifdef CLI_LVL_ALL
    INT32           i4_return;

    /* Attach rec util CLI command table to CLI */
    i4_return = x_cli_attach_cmd_tbl(at_rec_util_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_NONE);    
    if (i4_return != CLIR_OK)
    {
        return REC_UTILR_INTERNAL_ERROR;
    }    
    #endif
    
    return REC_UTILR_OK;
}
 
