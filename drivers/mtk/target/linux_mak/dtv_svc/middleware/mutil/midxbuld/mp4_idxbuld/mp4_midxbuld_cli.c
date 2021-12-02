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
 * $RCSfile: mp4_midxbuld_cli.c,v $
 * $Revision:
 * $Date:
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_ATSC/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 37ebbb2d76fad2e8a90655e5f2bf2c3a $
 *
 * Description:
 *         This file implements get/set debug level APIs for media format recognizer.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "../../mm_util.h"

#include "inc/x_common.h"
#include "inc/x_mm_common.h"
#include "mutil/midxbuld/u_midxbuld.h"
#include "mutil/midxbuld/midxbuld.h"
#include "mutil/midxbuld/mp4_idxbuld/mp4_midxbuld.h"
#include "mutil/mm_util_input.h"

#include "mutil/minfo/u_minfo.h"
#include "mutil/minfo/x_minfo.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif

#include "cli/x_cli.h"
#include "dbg/x_dbg.h"
#include "os/inc/x_os.h"

#else

#include "x_common.h"
#include "x_mm_common.h"
#include "../u_midxbuld.h"
#include "../midxbuld.h"
#include "mp4_midxbuld.h"
#include "../../mm_util_input.h"

#include "../../minfo/u_minfo.h"
#include "../../minfo/x_minfo.h"

#include "../../mm_util.h"

#include "x_cli.h"
#include "x_dbg.h"
#include "x_os.h"


#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
#if (ENABLE_MM_CLI_TEST == 1) 
#ifdef CLI_LVL_ALL

static HANDLE_T       gh_minfo;
static INT32 midxbuld_cli_mp4_fopen(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_mp4_mopen(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_mp4_close(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_mp4_start_idx(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_mp4_stop_idx(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_mp4_get_tbl(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_mp4_alloc_range(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_mp4_free_range(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_mp4_fstress(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_mp4_mstress(INT32 i4_argc, const CHAR** pps_argv);
/* mindex builder command table */
static CLI_EXEC_T at_mp4_midxbuld_cmd_tbl[] =
{
    {"fopen",       NULL,  midxbuld_cli_mp4_fopen,        NULL,    "midxbuld_fopen",       CLI_SUPERVISOR},
    {"mopen",       NULL,  midxbuld_cli_mp4_mopen,        NULL,    "midxbuld_mopen",       CLI_SUPERVISOR},
    {"close",       NULL,  midxbuld_cli_mp4_close,        NULL,    "midxbuld_close",       CLI_SUPERVISOR},
    {"start",       NULL,  midxbuld_cli_mp4_start_idx,    NULL,    "midxbuld_start_idx",   CLI_SUPERVISOR},
    {"stop",        NULL,  midxbuld_cli_mp4_stop_idx,     NULL,    "midxbuld_stop_idx",    CLI_SUPERVISOR},
    {"get_tbl",     NULL,  midxbuld_cli_mp4_get_tbl,      NULL,    "midxbuld_get_tbl",     CLI_SUPERVISOR},
    {"alloc_range", NULL,  midxbuld_cli_mp4_alloc_range,  NULL,    "midxbuld_alloc_range", CLI_SUPERVISOR}, 
    {"free_range",  NULL,  midxbuld_cli_mp4_free_range,   NULL,    "midxbuld_free_range",  CLI_SUPERVISOR}, 
    {"fstress",     NULL,  midxbuld_cli_mp4_fstress,      NULL,    "midxbuld_file_stress", CLI_SUPERVISOR}, 
    {"mstress",     NULL,  midxbuld_cli_mp4_mstress,      NULL,    "midxbuld_mem_stress",  CLI_SUPERVISOR}, 
    END_OF_CLI_CMD_TBL
};

static HANDLE_T       gh_midxbuld;
static UINT32         gui4_key_elmt_tbl_id;

/* for test mopen */
static HANDLE_T       gh_file = NULL_HANDLE;
static UINT8*         gpui1_buf = NULL;

#endif
/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 _atoi( const CHAR* ps )
{
    INT32  i4_total;

    i4_total = 0;
    while ( *ps != 0 )
    {
        if ( *ps >= '0' && *ps <= '9' )
        {
            /* accumulate digit */
            i4_total = 10 * i4_total + (*ps - '0');
            ps++; /* get next char */
        }
        else /* not a digit char */
        {
            return (-1);
        }
    }

    return i4_total;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: x_midxbuld_mp4_nfy
 *
 * Description: 
 *
 * Inputs:  
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
BOOL b_event_idx_done = FALSE;

static VOID x_midxbuld_mp4_nfy (
                        HANDLE_T                  h_midxbuld, 
                        MIDXBULD_NFY_EVENT_TYPE_T e_event,  
                        const VOID*               pv_data1, 
                        const VOID*               pv_data2, 
                        const VOID*               pv_tag)
{    
    x_dbg_stmt("x_midxbuld_nfy nfy\r\n");

    switch (e_event)
    {
    case MIDXBULD_NFY_EVENT_INDEXING_DONE:
        x_dbg_stmt("MIDXBULD_NFY_EVENT_INDEXING_DONE %d(0x%.8X)\r\n", 
                   (UINT32)pv_data1, 
                   (UINT32)pv_data1);
        b_event_idx_done = TRUE;
        break;
    case MIDXBULD_NFY_EVENT_INDEXING_ABORT:
        x_dbg_stmt("MIDXBULD_NFY_EVENT_INDEXING_ABORT\r\n");
        break;
    case MIDXBULD_NFY_EVENT_HANDLE_CLOSED:
        x_dbg_stmt("MIDXBULD_NFY_EVENT_HANDLE_CLOSED\r\n");
        break;
    case MIDXBULD_NFY_EVENT_ERROR:
        x_dbg_stmt("MIDXBULD_NFY_EVENT_ERROR\r\n");
        break;
    default:
        break;
    }
}
#endif

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_mp4_fopen
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
#ifdef CLI_LVL_ALL
static INT32 midxbuld_cli_mp4_fopen(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32                   i4_ret;
    MEDIA_FORMAT_T          t_format;
    CHAR*                   ps_file_path;
    MIDXBULD_SOURCE_INFO_T  t_src_info;
    MIDXBULD_KEY_INFO_T     t_key_info;

    if (i4_argc != 2)
    {
        x_dbg_stmt("Usage: fopen FILE...\r\n");
        x_dbg_stmt("FILE List:\r\n");
        x_dbg_stmt("1: 1.mp4\r\n");
        return CLIR_INV_ARG;
    }

    ps_file_path = (CHAR*)pps_argv[1];

    t_format.e_media_type = MEDIA_TYPE_CONTAINER;
    t_format.t_media_subtype.u.e_contnr_subtype  = MEDIA_CONTNR_SUBTYPE_MP4;

    if (*ps_file_path == '1') /* file 1.mp4 */
    {
        ps_file_path = "/mnt/usb/Mass-000/mp4/1.mp4";
        
        t_src_info.ui8_data_offset = 0x0;
        t_src_info.ui8_data_size = 0x0;
        t_src_info.ui8_idx_offset = 0x1C;
        t_src_info.ui8_idx_size = 0x14285;
        t_src_info.ui1_total_strm_num = 2;
        t_src_info.ui4_total_duration = 15915885; /* dwDuration*90000/timescale */
        t_src_info.t_es_map[0].e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
        t_src_info.t_es_map[0].ui4_strm_id = 0xC9;
        t_src_info.t_es_map[0].ui4_scale = 90000;
        t_src_info.t_es_map[0].ui4_rate = 0;
        t_src_info.t_es_map[0].ui4_sample_sz = 0;
        t_src_info.t_es_map[0].ui4_avg_bytes_per_sec = 0;
        t_src_info.t_es_map[1].e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
        t_src_info.t_es_map[1].ui4_strm_id = 0x65;
        t_src_info.t_es_map[1].ui4_scale = 44100;
        t_src_info.t_es_map[1].ui4_rate = 0;
        t_src_info.t_es_map[1].ui4_sample_sz = 0;
        t_src_info.t_es_map[1].ui4_avg_bytes_per_sec = 0;
    
        t_key_info.ui4_strm_id = 0xC9;
        t_key_info.ui4_scale = 90000;
        t_key_info.ui4_rate = 0;
        t_key_info.ui4_sample_sz = 0;
        t_key_info.ui4_avg_bytes_per_sec = 0;
    }

    i4_ret = x_midxbuld_fopen(ps_file_path, 
                              x_midxbuld_mp4_nfy, 
                              (VOID*) -1, 
                              &t_format, 
                              &t_src_info, 
                              &t_key_info, 
                              &gh_midxbuld);
    if (i4_ret != MIDXBULDR_OK)
    {
        x_dbg_stmt("midxbuld_cli_mp4_fopen ERROR!\r\n");
        return CLIR_CMD_EXEC_ERROR;
    }

    gui4_key_elmt_tbl_id = t_key_info.ui4_tbl_id;

    x_dbg_stmt("midxbuld_cli_mp4_fopen OK!\r\n");
    x_dbg_stmt("key index range id: %d(0x%.8X)\r\n", 
               (UINT32)t_key_info.ui4_range_id, 
               (UINT32)t_key_info.ui4_range_id);
    x_dbg_stmt("key index element table 00 id: %d(0x%.8X)\r\n", 
               (UINT32)t_key_info.ui4_tbl_id, 
               (UINT32)t_key_info.ui4_tbl_id);
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_mp4_mopen
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
#ifdef CLI_LVL_ALL
static INT32 midxbuld_cli_mp4_mopen(INT32 i4_argc, const CHAR** pps_argv)
{
#ifndef __NO_FM__
    INT32                   i4_ret;
    MEDIA_FORMAT_T          t_format;
    CHAR*                   ps_file_path;
    FM_FILE_INFO_T          t_fm_info;
    UINT64                  ui8_size;
    UINT64                  ui8_cur_pos;
    UINT32                  ui4_read;
    MIDXBULD_SOURCE_INFO_T  t_src_info;
    MIDXBULD_KEY_INFO_T     t_key_info;
    MEDIA_FEEDER_T          t_feeder;

    
    if (i4_argc != 3)
    {
        x_dbg_stmt("Usage: mopen FILE mem_leng  \r\n");
        return CLIR_INV_ARG;
    }

    ps_file_path = (CHAR*)pps_argv[1];

    t_format.e_media_type = MEDIA_TYPE_CONTAINER;
    t_format.t_media_subtype.u.e_contnr_subtype  = MEDIA_CONTNR_SUBTYPE_MP4;

    if (*ps_file_path == '1') /* file 1.mp4 */
    {
        ps_file_path = "/mnt/usb/Mass-000/1.mp4";
        
        t_src_info.ui8_data_offset = 0x0;
        t_src_info.ui8_data_size = 0x0;
        t_src_info.ui8_idx_offset = 0x1C;
        t_src_info.ui8_idx_size = 0x14285;
        t_src_info.ui1_total_strm_num = 2;
        t_src_info.ui4_total_duration = 15915885; /* dwDuration*90000/timescale */
        t_src_info.t_es_map[0].e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
        t_src_info.t_es_map[0].ui4_strm_id = 0xC9;
        t_src_info.t_es_map[0].ui4_scale = 90000;
        t_src_info.t_es_map[0].ui4_rate = 0;
        t_src_info.t_es_map[0].ui4_sample_sz = 0;
        t_src_info.t_es_map[0].ui4_avg_bytes_per_sec = 0;
        t_src_info.t_es_map[1].e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
        t_src_info.t_es_map[1].ui4_strm_id = 0x65;
        t_src_info.t_es_map[1].ui4_scale = 44100;
        t_src_info.t_es_map[1].ui4_rate = 0;
        t_src_info.t_es_map[1].ui4_sample_sz = 0;
        t_src_info.t_es_map[1].ui4_avg_bytes_per_sec = 0;
    
        t_key_info.ui4_strm_id = 0xC9;
        t_key_info.ui4_scale = 90000;
        t_key_info.ui4_rate = 0;
        t_key_info.ui4_sample_sz = 0;
        t_key_info.ui4_avg_bytes_per_sec = 0;
    }  
    
    /* open file and fill data */
    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_file_path, FM_READ_ONLY, 0777, FALSE, &gh_file);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("open file fail  \r\n");
        return CLIR_CMD_EXEC_ERROR;
    }

    /* set memory length */
    if (0 == x_strncmp(pps_argv[2], "0", (SIZE_T)1))
    {
        t_feeder.h_feeder = gh_file;
        t_feeder.t_feeder_be_fct_tbl.pf_copybytes = cli_feeder_copybytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_get_pos = cli_feeder_getpos_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input1 = cli_feeder_input1bytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input2 = cli_feeder_input2bytes_b_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input3 = cli_feeder_input3bytes_b_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input4 = cli_feeder_input4bytes_b_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_set_pos = cli_feeder_setpos_fct;
        
        i4_ret = x_midxbuld_mopen((VOID*)&t_feeder, 
                                  (SIZE_T)0,
                                  x_midxbuld_mp4_nfy, 
                                  (VOID*) -1, 
                                  &t_format, 
                                  &t_src_info, 
                                  &t_key_info, 
                                  &gh_midxbuld);
    }
    else
    {
        x_fm_get_info_by_handle(gh_file, &t_fm_info);
        ui8_size = t_fm_info.ui8_size;
        gpui1_buf = (UINT8*)_mm_util_mem_alloc(sizeof(UINT8) * ui8_size);
        if(gpui1_buf == NULL)
        {
            x_dbg_stmt("alloc file memory fail  \r\n");
            x_fm_close(gh_file);   
            gh_file = NULL_HANDLE;
            return CLIR_CMD_EXEC_ERROR;
        }
        x_memset(gpui1_buf, 0, ui8_size);
        i4_ret = x_fm_lseek(gh_file, 0, FM_SEEK_BGN, &ui8_cur_pos);
        if(i4_ret != FMR_OK)
        {
            x_dbg_stmt("file seek fail  \r\n");
            x_fm_close(gh_file);   
            _mm_util_mem_free(gpui1_buf);
            return CLIR_CMD_EXEC_ERROR;
        }
        i4_ret = x_fm_read(gh_file, (VOID*)gpui1_buf, ui8_size, &ui4_read);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("read file data fail  \r\n");
            x_fm_close(gh_file);   
            _mm_util_mem_free(gpui1_buf);
            return CLIR_CMD_EXEC_ERROR;
        }
        i4_ret = x_midxbuld_mopen((VOID*)gpui1_buf, 
                                  (SIZE_T)ui8_size,
                                  x_midxbuld_mp4_nfy, 
                                  (VOID*) -1, 
                                  &t_format, 
                                  &t_src_info, 
                                  &t_key_info, 
                                  &gh_midxbuld);
    }

    if (i4_ret != MIDXBULDR_OK)
    {
        x_dbg_stmt("midxbuld_cli_mp4_mopen ERROR!\r\n");
        return CLIR_CMD_EXEC_ERROR;
    }

    gui4_key_elmt_tbl_id = t_key_info.ui4_tbl_id;

    x_dbg_stmt("midxbuld_cli_mp4_mopen OK!\r\n");
    x_dbg_stmt("key index range id: %d(0x%.8X)\r\n", 
               (UINT32)t_key_info.ui4_range_id, 
               (UINT32)t_key_info.ui4_range_id);
    x_dbg_stmt("key index element table 00 id: %d(0x%.8X)\r\n", 
               (UINT32)t_key_info.ui4_tbl_id, 
               (UINT32)t_key_info.ui4_tbl_id);
    return CLIR_OK;
#else
    return CLIR_OK;
#endif
}
#endif

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_mp4_close
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
#ifdef CLI_LVL_ALL
static INT32 midxbuld_cli_mp4_close(INT32 i4_argc, const CHAR** pps_argv)
{
    /* check and free mopen resource */
    if(gh_file != NULL_HANDLE)
    {
        x_dbg_stmt("close file handle for mopen \r\n");
        x_fm_close(gh_file);   
        gh_file = NULL_HANDLE;
    }
    if(gpui1_buf != NULL)
    {
        x_dbg_stmt("free memory for mopen  \r\n");
        _mm_util_mem_free(gpui1_buf);
        gpui1_buf = NULL;
    }

    x_midxbuld_close(gh_midxbuld);
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_mp4_start_idx
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
#ifdef CLI_LVL_ALL
static INT32 midxbuld_cli_mp4_start_idx(INT32 i4_argc, const CHAR** pps_argv)
{
    x_midxbuld_start_indexing(gh_midxbuld);
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_mp4_stop_idx
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
#ifdef CLI_LVL_ALL
static INT32 midxbuld_cli_mp4_stop_idx(INT32 i4_argc, const CHAR** pps_argv)
{
    x_midxbuld_stop_indexing(gh_midxbuld);
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_mp4_get_tbl
 *
 * Description: This API gets the info of idx builder.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 midxbuld_cli_mp4_get_tbl(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32                       i4_ret;
    MIDXBULD_ELMT_TBL_INFO_T    t_tbl_info;
    MIDXBULD_ELMT_STRM_ENTRY_T* pt_idx_entry;
    SIZE_T                      z_size;

    if (i4_argc == 1)
    {
        t_tbl_info.ui4_tbl_id = gui4_key_elmt_tbl_id;
    }
    else if (i4_argc == 2)
    {
        t_tbl_info.ui4_tbl_id = (UINT32)_atoi(pps_argv[1]);
    }
    else
    {
        x_dbg_stmt("Usage: get_info TABLE_ID...\r\n");
        return CLIR_INV_ARG;
    }

    z_size = sizeof(MIDXBULD_ELMT_TBL_INFO_T);
    i4_ret = x_midxbuld_get_info(gh_midxbuld, 
                                 MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL, 
                                 (VOID*)&t_tbl_info, 
                                 &z_size);
    if (i4_ret != MIDXBULDR_OK)
    {
        x_dbg_stmt("midxbuld_cli_mp4_get_tbl ERROR!\r\n");
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("midxbuld_cli_mp4_get_tbl OK!\r\n");
    x_dbg_stmt("ui8_base_offset: 0x%.8X\r\n", (UINT32)t_tbl_info.t_elmt_tbl.ui8_base_offset);
    x_dbg_stmt("ui4_number_of_entry: %d\r\n", t_tbl_info.t_elmt_tbl.ui4_number_of_entry);
    x_dbg_stmt("e_es_type: %d\r\n", t_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.e_es_type);
    x_dbg_stmt("ui4_strm_id: 0x%.8X\r\n", t_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id);
    x_dbg_stmt("ui4_scale: %d\r\n", t_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale);
    x_dbg_stmt("ui4_rate: %d\r\n", t_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.ui4_rate);
    x_dbg_stmt("ui4_sample_sz: %d\r\n", t_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.ui4_sample_sz);
    x_dbg_stmt("ui4_avg_bytes_per_sec: %d\r\n", t_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.ui4_avg_bytes_per_sec);
    x_dbg_stmt("pt_idx_entry: 0x%.8X\r\n", t_tbl_info.t_elmt_tbl.u_fmt.t_es.pt_idx_entry);
    pt_idx_entry = t_tbl_info.t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
    x_dbg_stmt("the first index entry: (0x%.8X, 0x%.8X, 0x%.8X)\r\n", pt_idx_entry->ui4_pts, pt_idx_entry->ui4_relative_offset, pt_idx_entry->ui4_size);
    pt_idx_entry = t_tbl_info.t_elmt_tbl.u_fmt.t_es.pt_idx_entry + t_tbl_info.t_elmt_tbl.ui4_number_of_entry - 1;
    x_dbg_stmt("the last index entry: (0x%.8X, 0x%.8X, 0x%.8X)\r\n", pt_idx_entry->ui4_pts, pt_idx_entry->ui4_relative_offset, pt_idx_entry->ui4_size);
    
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_mp4_alloc_range
 *
 * Description: This API sets the info of idx builder.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 midxbuld_cli_mp4_alloc_range(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32                           i4_ret;
    MIDXBULD_ELMT_TBL_ALLOC_INFO_T  t_tbl_info;
    MIDXBULD_ELMT_TBL_DESCRIB_T     t_describ_00;
    MIDXBULD_ELMT_TBL_DESCRIB_T     t_describ_01;
    SIZE_T                          z_size;

    if (i4_argc != 4)
    {
        x_dbg_stmt("Usage: alloc_range FILE START_PTS END_PTS...\r\n");
        return CLIR_INV_ARG;
    }

    t_tbl_info.e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
    t_tbl_info.u_range.t_es.ui4_start_pts = (UINT32)_atoi(pps_argv[2]);
    t_tbl_info.u_range.t_es.ui4_end_pts = (UINT32)_atoi(pps_argv[3]);
    t_tbl_info.pt_tbl_describ = &t_describ_00;

    if (*(CHAR*)pps_argv[1] == '1') /* file 1.mp4 */
    {
        t_describ_00.u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
        t_describ_00.u_info.t_es.ui4_strm_id = 0xC9;
        t_describ_00.u_info.t_es.ui4_scale = 90000;
        t_describ_00.u_info.t_es.ui4_rate = 0;
        t_describ_00.u_info.t_es.ui4_sample_sz = 0;
        t_describ_00.u_info.t_es.ui4_avg_bytes_per_sec = 0;
        t_describ_00.pt_next = &t_describ_01;
        
        t_describ_01.u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
        t_describ_01.u_info.t_es.ui4_strm_id = 0x65;
        t_describ_01.u_info.t_es.ui4_scale = 44100;
        t_describ_01.u_info.t_es.ui4_rate = 0;
        t_describ_01.u_info.t_es.ui4_sample_sz = 0;
        t_describ_01.u_info.t_es.ui4_avg_bytes_per_sec = 0;
        t_describ_01.pt_next = NULL;
        x_dbg_stmt("File 1.mp4 \r\n");
        x_dbg_stmt("Video last sample pts:0xF2BE30, offset:0xA911AB, size:0x8B \r\n");
        x_dbg_stmt("Audio last sample pts:0xF2D343, offset:0xA914F5, size:0x29 \r\n");
    }

    z_size = sizeof(MIDXBULD_ELMT_TBL_ALLOC_INFO_T);
    i4_ret = x_midxbuld_set_info(gh_midxbuld, 
                                 MIDXBULD_SET_INFO_TYPE_ALLOC_RANGE, 
                                 (VOID*)&t_tbl_info, 
                                 &z_size);
    if (i4_ret != MIDXBULDR_OK)
    {
        x_dbg_stmt("midxbuld_cli_mp4_alloc_range ERROR!\r\n");
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("index range id: %d(0x%.8X)\r\n", 
               (UINT32)t_tbl_info.ui4_range_id, 
               (UINT32)t_tbl_info.ui4_range_id);
    x_dbg_stmt("index element table 00 id: %d(0x%.8X)\r\n", 
               (UINT32)t_describ_00.ui4_tbl_id, 
               (UINT32)t_describ_00.ui4_tbl_id);
    x_dbg_stmt("index element table 01 id: %d(0x%.8X)\r\n", 
               (UINT32)t_describ_01.ui4_tbl_id, 
               (UINT32)t_describ_01.ui4_tbl_id);
    
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_mp4_free_range
 *
 * Description: This API sets the info of idx builder.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 midxbuld_cli_mp4_free_range(INT32 i4_argc, const CHAR** pps_argv)
{
    UINT32  ui4_range_id;
    INT32   i4_ret;
    SIZE_T  z_size;

    if (i4_argc != 2)
    {
        x_dbg_stmt("Usage: free_range RANGE_ID...\r\n");
        return CLIR_INV_ARG;
    }

    ui4_range_id = (UINT32)_atoi(pps_argv[1]);

    z_size = sizeof(UINT32);
    i4_ret = x_midxbuld_set_info(gh_midxbuld, 
                                 MIDXBULD_SET_INFO_TYPE_FREE_RANGE, 
                                 (VOID*)ui4_range_id, 
                                 &z_size);
    if (i4_ret != MIDXBULDR_OK)
    {
        x_dbg_stmt("midxbuld_cli_mp4_free_range ERROR!\r\n");
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("midxbuld_cli_mp4_free_range OK!\r\n");
    
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_mp4_fstress
 *
 * Description: 
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 midxbuld_cli_mp4_fstress(INT32 i4_argc, const CHAR** pps_argv)
{
/*Usage: fstress filename [null(full_range)/build_duration] audio_check_value*/
    INT32                           i4_ret;
    MEDIA_FORMAT_T                  t_format;
    CHAR*                           ps_file_path;
    MIDXBULD_SOURCE_INFO_T          t_src_info;
    MIDXBULD_KEY_INFO_T             t_key_info;
    MIDXBULD_ELMT_TBL_ALLOC_INFO_T  t_tbl_info;
    MIDXBULD_ELMT_TBL_DESCRIB_T     t_describ_00;
    MIDXBULD_ELMT_TBL_DESCRIB_T     t_describ_01;
    MIDXBULD_ELMT_STRM_ENTRY_T*     pt_idx_entry;
    MIDXBULD_ELMT_TBL_INFO_T        t_elmt_tbl_info;
    UINT32                          ui4_range_id;
    SIZE_T                          z_size;
    INT32                           i4_loop_count = 0;
    MINFO_INFO_T       t_minfo_rec;
    SIZE_T             t_size;
    INT32                           i4_build_len;/*second*/
    INT32                           i4_build_cnt = 0;
    UINT32                         ui4_check_value = 0;
    BOOL                            b_need_check_audio_value;
    static UINT32                   ui4_aud_sample_cnt = 0;
    UINT32                          ui4_start_time;
    UINT32                          ui4_end_time;
    UINT32                          ui4_idx_open_time;
    UINT32                          ui4_idx_building_time;

    if (i4_argc < 3)
    {
        x_dbg_stmt("\r\nUsage: fstress filepathname 0(full_range)/build_duration [audio_check_value]\r\n");
        return CLIR_OK;
    }
    else if (i4_argc == 3)
    {
        b_need_check_audio_value = FALSE;
    }
    else
    {
        ui4_check_value = x_strtoll(pps_argv[3], NULL, 10);
        b_need_check_audio_value = TRUE;
    }
    while (1)
    {
        if(i4_loop_count >= 2)
        {
            break;
        }
        else
        {
            i4_build_cnt = 0;
        }
        x_dbg_stmt("\r\n---------- loop:  %d   -----------\r\n", i4_loop_count++);
        t_format.e_media_type = MEDIA_TYPE_CONTAINER;
        t_format.t_media_subtype.u.e_contnr_subtype  = MEDIA_CONTNR_SUBTYPE_MP4;

        ps_file_path = (CHAR*)pps_argv[1];

        /* open file*/    
        i4_ret = x_minfo_fopen(ps_file_path, NULL, NULL, &t_format, &gh_minfo);
        if (MINFOR_OK != i4_ret )
        {
            DBG_ERROR(("Failed to open file(%s)!,i4_ret = %d\r\n", 
                       ps_file_path, i4_ret));
            return CLIR_CMD_EXEC_ERROR;
        }
        DBG_INFO(("minfo open file(%s) successfully!\r\n", ps_file_path));

        /*open for check audio offset value --- start*/
        if(b_need_check_audio_value)
        {
            i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_file_path, FM_READ_ONLY, 0777, FALSE, &gh_file);
            if (i4_ret != FMR_OK)
            {
                x_dbg_stmt("fail to open file \r\n");
                return CLIR_CMD_EXEC_ERROR;
            }
        }
        /*open for check audio offset value --- end*/

        x_thread_delay(2000);

        /*Get minfo idx info*/
        t_size = sizeof(MINFO_INFO_T);
        x_memset(&t_minfo_rec, 0, t_size);
        i4_ret = x_minfo_get_info(gh_minfo,
                              MINFO_INFO_TYPE_IDX_INFO,
                              1,
                              (VOID*)&t_minfo_rec,
                              &t_size);
        t_src_info.ui8_data_offset = t_minfo_rec.u.t_idx_info.ui8_data_offset;
        t_src_info.ui8_data_size = t_minfo_rec.u.t_idx_info.ui8_data_sz;
        t_src_info.ui8_idx_offset = t_minfo_rec.u.t_idx_info.ui8_idx_offset;
        t_src_info.ui8_idx_size = t_minfo_rec.u.t_idx_info.ui8_idx_sz;
        t_src_info.pt_mp4_cmov_info = t_minfo_rec.u.t_idx_info.pt_mp4_cmov_info;
        
        /* get minfo info-generic */
        t_size = sizeof(MINFO_INFO_T);
        x_memset(&t_minfo_rec, 0, t_size);
        i4_ret = x_minfo_get_info(gh_minfo,
                              MINFO_INFO_TYPE_GENERIC,
                              0,/* notice here*/
                              (VOID*)&t_minfo_rec,
                              &t_size);
        t_src_info.ui1_total_strm_num = t_minfo_rec.u.t_general_info.ui1_strm_ns;
        t_src_info.ui4_total_duration = t_minfo_rec.u.t_general_info.ui8_pb_duration; /* dwDuration*90000/timescale */
        
        /* get minfo video info */
        t_size = sizeof(MINFO_INFO_T);
        x_memset(&t_minfo_rec, 0, t_size);
        i4_ret = x_minfo_get_info(gh_minfo,
                              MINFO_INFO_TYPE_VIDEO_ATTR,
                              1,
                              (VOID*)&t_minfo_rec,
                              &t_size);
        t_src_info.t_es_map[0].e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
        t_src_info.t_es_map[0].ui4_strm_id = t_minfo_rec.u.t_stm_attribute.t_stm_id.u.t_mp4_stm_id;
        t_src_info.t_es_map[0].ui4_scale = t_minfo_rec.u.t_stm_attribute.u.t_mp4i_video_stm_attr.ui4_timescale;
        t_src_info.t_es_map[0].ui4_rate = 0;
        t_src_info.t_es_map[0].ui4_sample_sz = 0;
        t_src_info.t_es_map[0].ui4_avg_bytes_per_sec = 0;
        
        /* get minfo audio info */
        t_size = sizeof(MINFO_INFO_T);
        x_memset(&t_minfo_rec, 0, t_size);
        i4_ret = x_minfo_get_info(gh_minfo,
                              MINFO_INFO_TYPE_AUDIO_ATTR,
                              1,
                              (VOID*)&t_minfo_rec,
                              &t_size);
        t_src_info.t_es_map[1].e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
        t_src_info.t_es_map[1].ui4_strm_id = t_minfo_rec.u.t_stm_attribute.t_stm_id.u.t_mp4_stm_id;
        t_src_info.t_es_map[1].ui4_scale = t_minfo_rec.u.t_stm_attribute.u.t_mp4i_audio_stm_attr.ui4_timescale;
        t_src_info.t_es_map[1].ui4_rate = 0;
        t_src_info.t_es_map[1].ui4_sample_sz = 0;
        t_src_info.t_es_map[1].ui4_avg_bytes_per_sec = 0;

        t_key_info.ui4_strm_id = t_src_info.t_es_map[0].ui4_strm_id;
        t_key_info.ui4_scale = t_src_info.t_es_map[0].ui4_scale;
        t_key_info.ui4_rate = t_src_info.t_es_map[0].ui4_rate;
        t_key_info.ui4_sample_sz = t_src_info.t_es_map[0].ui4_sample_sz;
        t_key_info.ui4_avg_bytes_per_sec = 0;

        b_event_idx_done = FALSE;
        ui4_start_time = x_os_get_sys_tick();
        /* open file first */
        i4_ret = x_midxbuld_fopen(ps_file_path, 
                                  x_midxbuld_mp4_nfy, 
                                  (VOID*) -1, 
                                  &t_format, 
                                  &t_src_info, 
                                  &t_key_info, 
                                  &gh_midxbuld);
        /* delay 500ms for open done */
        while (!b_event_idx_done)
        {
            x_thread_delay(10);
        }
        ui4_end_time = x_os_get_sys_tick();
        ui4_idx_open_time = ui4_end_time - ui4_start_time;
        x_dbg_stmt("============ index open time =%d ms ============ \n",ui4_idx_open_time*x_os_get_sys_tick_period ());

        if (i4_ret != MIDXBULDR_OK)
        {
            x_dbg_stmt("midxbuld_cli_mp4_fopen ERROR!\r\n");
            return CLIR_CMD_EXEC_ERROR;
        }

        gui4_key_elmt_tbl_id = t_key_info.ui4_tbl_id;

        x_dbg_stmt("midxbuld_cli_mp4_fopen OK!\r\n");
        x_dbg_stmt("key index range id: %d(0x%.8X)\r\n", 
                   (UINT32)t_key_info.ui4_range_id, 
                   (UINT32)t_key_info.ui4_range_id);
        x_dbg_stmt("key index element table 00 id: %d(0x%.8X)\r\n", 
                   (UINT32)t_key_info.ui4_tbl_id, 
                   (UINT32)t_key_info.ui4_tbl_id);

        x_dbg_stmt("Video Strm ID: %d ,   Audio Strm ID: %d\r\n", 
                   (UINT32)t_src_info.t_es_map[0].ui4_strm_id, 
                   (UINT32)t_src_info.t_es_map[1].ui4_strm_id);
        
        t_describ_00.u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
        t_describ_00.u_info.t_es.ui4_strm_id = t_src_info.t_es_map[0].ui4_strm_id;
        t_describ_00.u_info.t_es.ui4_scale = t_src_info.t_es_map[0].ui4_scale;
        t_describ_00.u_info.t_es.ui4_rate = t_src_info.t_es_map[0].ui4_rate;
        t_describ_00.u_info.t_es.ui4_sample_sz = t_src_info.t_es_map[0].ui4_sample_sz;
        t_describ_00.u_info.t_es.ui4_avg_bytes_per_sec = 0;
        t_describ_00.pt_next = &t_describ_01;

        t_describ_01.u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
        t_describ_01.u_info.t_es.ui4_strm_id = t_src_info.t_es_map[1].ui4_strm_id;
        t_describ_01.u_info.t_es.ui4_scale = t_src_info.t_es_map[1].ui4_scale;
        t_describ_01.u_info.t_es.ui4_rate = t_src_info.t_es_map[1].ui4_rate;
        t_describ_01.u_info.t_es.ui4_sample_sz = t_src_info.t_es_map[1].ui4_sample_sz;
        t_describ_01.u_info.t_es.ui4_avg_bytes_per_sec = 0;
        t_describ_01.pt_next = NULL;
/*start*/
    while(1)
    { 
        /* alloc range */
        t_tbl_info.e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
        i4_build_len = x_strtoll(pps_argv[2], NULL, 10);
        if (i4_build_len == 0)
        {
            t_tbl_info.u_range.t_es.ui4_start_pts = 0;
            t_tbl_info.u_range.t_es.ui4_end_pts = 0xffffffff;
        }
        else
        {
            t_tbl_info.u_range.t_es.ui4_start_pts = i4_build_cnt*i4_build_len*90000;
            t_tbl_info.u_range.t_es.ui4_end_pts = (i4_build_cnt+1)*i4_build_len*90000;
            x_dbg_stmt("\r\n-----section: %d   ----- start pts: %d -----end pts: %d \r\n", 
                    i4_build_cnt,
                   (UINT32)t_tbl_info.u_range.t_es.ui4_start_pts, 
                   (UINT32)t_tbl_info.u_range.t_es.ui4_end_pts);
            i4_build_cnt++;
        }
        t_tbl_info.pt_tbl_describ = &t_describ_00;        

        z_size = sizeof(MIDXBULD_ELMT_TBL_ALLOC_INFO_T);
        i4_ret = x_midxbuld_set_info(gh_midxbuld, 
                                     MIDXBULD_SET_INFO_TYPE_ALLOC_RANGE, 
                                    (VOID*)&t_tbl_info, 
                                    &z_size);
        
        /* delay 500ms for alloc range done */
        /*x_thread_delay(10); */
        
        if (i4_ret != MIDXBULDR_OK)
        {
            x_dbg_stmt("midxbuld_cli_mp4_alloc_range ERROR!\r\n");
            return CLIR_CMD_EXEC_ERROR;
        }

        x_dbg_stmt("index range id: %d(0x%.8X)\r\n", 
                   (UINT32)t_tbl_info.ui4_range_id, 
                   (UINT32)t_tbl_info.ui4_range_id);
        x_dbg_stmt("index element table 00 id: %d(0x%.8X)\r\n", 
                   (UINT32)t_describ_00.ui4_tbl_id, 
                   (UINT32)t_describ_00.ui4_tbl_id);
        x_dbg_stmt("index element table 01 id: %d(0x%.8X)\r\n", 
                   (UINT32)t_describ_01.ui4_tbl_id, 
                   (UINT32)t_describ_01.ui4_tbl_id);

        b_event_idx_done = FALSE;
        /* start indexing */
        ui4_start_time = x_os_get_sys_tick();
        x_midxbuld_start_indexing(gh_midxbuld);
        /* delay 2000ms for indexing done */
        while (!b_event_idx_done)
        {
            x_thread_delay(10);
        }
        ui4_end_time = x_os_get_sys_tick();
        ui4_idx_building_time = ui4_end_time - ui4_start_time;
        x_dbg_stmt("=============== index build time =%d ms =============== \n",ui4_idx_building_time*x_os_get_sys_tick_period ());

        x_dbg_stmt("----x_midxbuld_start_indexing  finished ----\r\n");
        /* get table info */ 
        /* video */
        t_elmt_tbl_info.ui4_tbl_id = t_describ_00.ui4_tbl_id;
        z_size = sizeof(MIDXBULD_ELMT_TBL_INFO_T);
        i4_ret = x_midxbuld_get_info(gh_midxbuld, 
                                     MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL, 
                                     (VOID*)&t_elmt_tbl_info, 
                                     &z_size);
        /* delay 500ms for get info done */
        x_thread_delay(10); 
        if (i4_ret != MIDXBULDR_OK)
        {
            x_dbg_stmt("midxbuld_cli_mp4_get_tbl ERROR!\r\n");
            return CLIR_CMD_EXEC_ERROR;
        }

        x_dbg_stmt("midxbuld_cli_mp4_get_tbl OK!\r\n");
        x_dbg_stmt("ui8_base_offset: 0x%.8X\r\n", (UINT32)t_elmt_tbl_info.t_elmt_tbl.ui8_base_offset);
        x_dbg_stmt("ui4_number_of_entry: %d\r\n", t_elmt_tbl_info.t_elmt_tbl.ui4_number_of_entry);
        x_dbg_stmt("e_es_type: %d\r\n", t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.e_es_type);
        x_dbg_stmt("ui4_strm_id: 0x%.8X\r\n", t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id);
        x_dbg_stmt("ui4_scale: %d\r\n", t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale);
        x_dbg_stmt("pt_idx_entry: 0x%.8X\r\n", t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.pt_idx_entry);
        pt_idx_entry = t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
        x_dbg_stmt("Video first index entry: (0x%.8X, 0x%.8X, 0x%.8X)\r\n", pt_idx_entry->ui4_pts, pt_idx_entry->ui4_relative_offset, pt_idx_entry->ui4_size);
        pt_idx_entry = t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.pt_idx_entry + t_elmt_tbl_info.t_elmt_tbl.ui4_number_of_entry - 1;
        x_dbg_stmt("Video last index entry: (0x%.8X, 0x%.8X, 0x%.8X)\r\n", pt_idx_entry->ui4_pts, pt_idx_entry->ui4_relative_offset, pt_idx_entry->ui4_size);

        /* audio */
        t_elmt_tbl_info.ui4_tbl_id = t_describ_01.ui4_tbl_id;
        z_size = sizeof(MIDXBULD_ELMT_TBL_INFO_T);
        i4_ret = x_midxbuld_get_info(gh_midxbuld, 
                                    MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL, 
                                    (VOID*)&t_elmt_tbl_info, 
                                    &z_size);
        /* delay 500ms for get info done */
        /*x_thread_delay(10); */
        if (i4_ret != MIDXBULDR_OK)
        {
            x_dbg_stmt("midxbuld_cli_mp4_get_tbl ERROR!\r\n");
            return CLIR_CMD_EXEC_ERROR;
        }

        x_dbg_stmt("midxbuld_cli_mp4_get_tbl OK!\r\n");
        x_dbg_stmt("ui8_base_offset: 0x%.8X\r\n", (UINT32)t_elmt_tbl_info.t_elmt_tbl.ui8_base_offset);
        x_dbg_stmt("ui4_number_of_entry: %d\r\n", t_elmt_tbl_info.t_elmt_tbl.ui4_number_of_entry);
        x_dbg_stmt("e_es_type: %d\r\n", t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.e_es_type);
        x_dbg_stmt("ui4_strm_id: 0x%.8X\r\n", t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id);
        x_dbg_stmt("ui4_scale: %d\r\n", t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale);
        x_dbg_stmt("pt_idx_entry: 0x%.8X\r\n", t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.pt_idx_entry);
        pt_idx_entry = t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
        x_dbg_stmt("Audio first index entry: (0x%.8X, 0x%.8X, 0x%.8X)\r\n", pt_idx_entry->ui4_pts, pt_idx_entry->ui4_relative_offset, pt_idx_entry->ui4_size);
        pt_idx_entry = t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.pt_idx_entry + t_elmt_tbl_info.t_elmt_tbl.ui4_number_of_entry - 1;
        x_dbg_stmt("Audio last index entry: (0x%.8X, 0x%.8X, 0x%.8X)\r\n", pt_idx_entry->ui4_pts, pt_idx_entry->ui4_relative_offset, pt_idx_entry->ui4_size);

        /* printf out all audio data */
        if(b_need_check_audio_value)
        {
            UINT8   ui1_buf[4];
            UINT32  ui4_read_size;
            INT64   i8_file_off;
            UINT64  ui8_file_pos;
            UINT32  ui4_temp_i;
            
            x_dbg_stmt("\n===============================================\n");
            pt_idx_entry = t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
            for (ui4_temp_i=0; ui4_temp_i<t_elmt_tbl_info.t_elmt_tbl.ui4_number_of_entry; ui4_temp_i++)
            {            
                ui4_aud_sample_cnt++;
                x_dbg_stmt("Audio [%05d] off=0x%08x size=%d pts=%d ",
                            ui4_aud_sample_cnt,
                            pt_idx_entry->ui4_relative_offset,
                            pt_idx_entry->ui4_size,
                            pt_idx_entry->ui4_pts
                            );
                /* check data */
                i8_file_off = (INT64)(pt_idx_entry->ui4_relative_offset);
                x_fm_lseek(gh_file, i8_file_off, FM_SEEK_BGN, &ui8_file_pos);
                x_fm_read(gh_file,ui1_buf,4,&ui4_read_size);
                if (ui4_read_size == 4)
                {
                    x_dbg_stmt(" data=%02x %02x %02x %02x ", 
                                ui1_buf[0],
                                ui1_buf[1], 
                                ui1_buf[2], 
                                ui1_buf[3]
                                );
                    if (ui1_buf[0] == ui4_check_value)
                    {
                        x_dbg_stmt(" OK \n");
                    }
                    else
                    {
                        x_dbg_stmt(" Error! \n");
                    }
                }
                else
                {
                    x_dbg_stmt("file read error size=%d \n",ui4_read_size);
                    break;
                }

                pt_idx_entry++;
                            
            }
            x_dbg_stmt("\n===============================================\n");
        }
        /* ========================= */
        
        /* free range */
        ui4_range_id = t_tbl_info.ui4_range_id;

        z_size = sizeof(UINT32);
        i4_ret = x_midxbuld_set_info(gh_midxbuld, 
                                    MIDXBULD_SET_INFO_TYPE_FREE_RANGE, 
                                    (VOID*)ui4_range_id, 
                                    &z_size);
        /* delay 200ms for set info done */
        /*x_thread_delay(10); */
        if (i4_ret != MIDXBULDR_OK)
        {
            x_dbg_stmt("midxbuld_cli_mp4_free_range ERROR!\r\n");
            return CLIR_CMD_EXEC_ERROR;
        }
        if(t_tbl_info.u_range.t_es.ui4_end_pts >= t_src_info.ui4_total_duration)
        {
            break;
        }
    }
/*end*/
                
        x_dbg_stmt("midxbuld_cli_mp4_free_range OK!\r\n");

        /* close file */
        x_midxbuld_close(gh_midxbuld);
        if(b_need_check_audio_value)
        {
            x_fm_close(gh_file);
        }
        /* delay 5000ms for one loop */
        x_thread_delay(5000); 
    }
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_mp4_mstress
 *
 * Description: 
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: 
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 midxbuld_cli_mp4_mstress(INT32 i4_argc, const CHAR** pps_argv)
{
#ifndef __NO_FM__
    INT32                           i4_ret;
    MEDIA_FORMAT_T                  t_format;
    CHAR*                           ps_file_path;
    MIDXBULD_SOURCE_INFO_T          t_src_info;
    MIDXBULD_KEY_INFO_T             t_key_info;
    MIDXBULD_ELMT_TBL_ALLOC_INFO_T  t_tbl_info;
    MIDXBULD_ELMT_TBL_DESCRIB_T     t_describ_00;
    MIDXBULD_ELMT_TBL_DESCRIB_T     t_describ_01;
    MIDXBULD_ELMT_STRM_ENTRY_T*     pt_idx_entry;
    MIDXBULD_ELMT_TBL_INFO_T        t_elmt_tbl_info;
    UINT32                          ui4_range_id;
    SIZE_T                          z_size;
    FM_FILE_INFO_T                  t_fm_info;
    UINT64                          ui8_size;
    UINT64                          ui8_cur_pos;
    UINT32                          ui4_read;
    MEDIA_FEEDER_T                  t_feeder;
    INT32                           i4_loop_count;

    if ((i4_argc != 2) || (pps_argv == NULL))
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        DBG_INFO(("usage:mstress mem_length(0/1) \r\n"));
        return CLIR_INV_ARG;
    }

    i4_loop_count = 100000;

    while (i4_loop_count--)

    {
        ps_file_path = "/mnt/usb/Mass-000/1.mp4";
        
        /* open file and fill data */
        i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_file_path, FM_READ_ONLY, 0777, FALSE, &gh_file);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("fail to open file \r\n");
            return CLIR_CMD_EXEC_ERROR;
        }

        /* set memory length */
        if (0 == x_strncmp(pps_argv[1], "0", (SIZE_T)1))
        {
            ui8_size = 0;
            t_feeder.h_feeder = gh_file;
            t_feeder.t_feeder_be_fct_tbl.pf_copybytes = cli_feeder_copybytes_fct;
            t_feeder.t_feeder_be_fct_tbl.pf_get_pos = cli_feeder_getpos_fct;
            t_feeder.t_feeder_be_fct_tbl.pf_input1 = cli_feeder_input1bytes_fct;
            t_feeder.t_feeder_be_fct_tbl.pf_input2 = cli_feeder_input2bytes_b_fct;
            t_feeder.t_feeder_be_fct_tbl.pf_input3 = cli_feeder_input3bytes_b_fct;
            t_feeder.t_feeder_be_fct_tbl.pf_input4 = cli_feeder_input4bytes_b_fct;
            t_feeder.t_feeder_be_fct_tbl.pf_set_pos = cli_feeder_setpos_fct;
        }
        else
        {
            x_fm_get_info_by_handle(gh_file, &t_fm_info);
            ui8_size = t_fm_info.ui8_size;
            gpui1_buf = (UINT8*)_mm_util_mem_alloc(sizeof(UINT8) * ui8_size);
            if(gpui1_buf == NULL)
            {
                x_dbg_stmt("mopen alloc memory fail \r\n");
                x_fm_close(gh_file);   
                gh_file = NULL_HANDLE;
                return CLIR_CMD_EXEC_ERROR;
            }
            x_memset(gpui1_buf, 0, ui8_size);
            i4_ret = x_fm_lseek(gh_file, 0, FM_SEEK_BGN, &ui8_cur_pos);
            if(i4_ret != FMR_OK)
            {
                x_fm_close(gh_file);   
                _mm_util_mem_free(gpui1_buf);
                return CLIR_CMD_EXEC_ERROR;
            }
            i4_ret = x_fm_read(gh_file, (VOID*)gpui1_buf, ui8_size, &ui4_read);
            if (i4_ret != FMR_OK)
            {
                x_fm_close(gh_file);   
                _mm_util_mem_free(gpui1_buf);
                return CLIR_CMD_EXEC_ERROR;
            }
        }
        
        t_format.e_media_type = MEDIA_TYPE_CONTAINER;
        t_format.t_media_subtype.u.e_contnr_subtype  = MEDIA_CONTNR_SUBTYPE_MP4;
        
        t_src_info.ui8_data_offset = 0x0;
        t_src_info.ui8_data_size = 0x0;
        t_src_info.ui8_idx_offset = 0x1C;
        t_src_info.ui8_idx_size = 0x14285;
        t_src_info.ui1_total_strm_num = 2;
        t_src_info.ui4_total_duration = 15915885; /* dwDuration*90000/timescale */
        t_src_info.t_es_map[0].e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
        t_src_info.t_es_map[0].ui4_strm_id = 0xC9;
        t_src_info.t_es_map[0].ui4_scale = 90000;
        t_src_info.t_es_map[0].ui4_rate = 0;
        t_src_info.t_es_map[0].ui4_sample_sz = 0;
        t_src_info.t_es_map[0].ui4_avg_bytes_per_sec = 0;
        t_src_info.t_es_map[1].e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
        t_src_info.t_es_map[1].ui4_strm_id = 0x65;
        t_src_info.t_es_map[1].ui4_scale = 44100;
        t_src_info.t_es_map[1].ui4_rate = 16000;
        t_src_info.t_es_map[1].ui4_sample_sz = 0;
        t_src_info.t_es_map[1].ui4_avg_bytes_per_sec = 0;
    
        t_key_info.ui4_strm_id = 0xC9;
        t_key_info.ui4_scale = 90000;
        t_key_info.ui4_rate = 0;
        t_key_info.ui4_sample_sz = 0;
        t_key_info.ui4_avg_bytes_per_sec = 0;

        /* open file first */
        if(ui8_size == 0)
        {
            i4_ret = x_midxbuld_mopen((VOID*)&t_feeder, 
                                      (SIZE_T)0, 
                                      x_midxbuld_mp4_nfy, 
                                      (VOID*) -1, 
                                      &t_format, 
                                      &t_src_info, 
                                      &t_key_info, 
                                      &gh_midxbuld);
        }
        else
        {
            i4_ret = x_midxbuld_mopen((VOID*)gpui1_buf, 
                                      (SIZE_T)ui8_size, 
                                      x_midxbuld_mp4_nfy, 
                                      (VOID*) -1, 
                                      &t_format, 
                                      &t_src_info, 
                                      &t_key_info, 
                                      &gh_midxbuld);
        }
        /* delay 500ms for open done */
        x_thread_delay(500); 
        if (i4_ret != MIDXBULDR_OK)
        {
            x_dbg_stmt("midxbuld_cli_mp4_mopen ERROR!\r\n");
            return CLIR_CMD_EXEC_ERROR;
        }

        gui4_key_elmt_tbl_id = t_key_info.ui4_tbl_id;

        x_dbg_stmt("midxbuld_cli_mp4_mopen OK!\r\n");
        x_dbg_stmt("key index range id: %d(0x%.8X)\r\n", 
                   (UINT32)t_key_info.ui4_range_id, 
                   (UINT32)t_key_info.ui4_range_id);
        x_dbg_stmt("key index element table 00 id: %d(0x%.8X)\r\n", 
                   (UINT32)t_key_info.ui4_tbl_id, 
                   (UINT32)t_key_info.ui4_tbl_id);

        /* alloc range */
        t_tbl_info.e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
        t_tbl_info.u_range.t_es.ui4_start_pts = 0;
        t_tbl_info.u_range.t_es.ui4_end_pts = 15918000;
        t_tbl_info.pt_tbl_describ = &t_describ_00;

        t_describ_00.u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
        t_describ_00.u_info.t_es.ui4_strm_id = 0xC9;
        t_describ_00.u_info.t_es.ui4_scale = 90000;
        t_describ_00.u_info.t_es.ui4_rate = 0;
        t_describ_00.u_info.t_es.ui4_sample_sz = 0;
        t_describ_00.u_info.t_es.ui4_avg_bytes_per_sec = 0;
        t_describ_00.pt_next = &t_describ_01;
        
        t_describ_01.u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
        t_describ_01.u_info.t_es.ui4_strm_id = 0x65;
        t_describ_01.u_info.t_es.ui4_scale = 44100;
        t_describ_01.u_info.t_es.ui4_rate = 0;
        t_describ_01.u_info.t_es.ui4_sample_sz = 0;
        t_describ_01.u_info.t_es.ui4_avg_bytes_per_sec = 0;
        t_describ_01.pt_next = NULL;
        x_dbg_stmt("File 1.mp4 \r\n");
        x_dbg_stmt("Video last sample pts:0xF2BE30, offset:0xA911AB, size:0x8B \r\n");
        x_dbg_stmt("Audio last sample pts:0xF2D343, offset:0xA914F5, size:0x29 \r\n");

        z_size = sizeof(MIDXBULD_ELMT_TBL_ALLOC_INFO_T);
        i4_ret = x_midxbuld_set_info(gh_midxbuld, 
                                     MIDXBULD_SET_INFO_TYPE_ALLOC_RANGE, 
                                    (VOID*)&t_tbl_info, 
                                    &z_size);
        /* delay 500ms for alloc range done */
        x_thread_delay(500); 
        
        if (i4_ret != MIDXBULDR_OK)
        {
            x_dbg_stmt("midxbuld_cli_mp4_alloc_range ERROR!\r\n");
            return CLIR_CMD_EXEC_ERROR;
        }

        x_dbg_stmt("index range id: %d(0x%.8X)\r\n", 
                   (UINT32)t_tbl_info.ui4_range_id, 
                   (UINT32)t_tbl_info.ui4_range_id);
        x_dbg_stmt("index element table 00 id: %d(0x%.8X)\r\n", 
                   (UINT32)t_describ_00.ui4_tbl_id, 
                   (UINT32)t_describ_00.ui4_tbl_id);
        x_dbg_stmt("index element table 01 id: %d(0x%.8X)\r\n", 
                   (UINT32)t_describ_01.ui4_tbl_id, 
                   (UINT32)t_describ_01.ui4_tbl_id);

        /* start indexing */
        x_midxbuld_start_indexing(gh_midxbuld);
        /* delay 2000ms for indexing done */
        x_thread_delay(2000); 

        /* get table info */ 
        /* video */
        t_elmt_tbl_info.ui4_tbl_id = t_describ_00.ui4_tbl_id;
        z_size = sizeof(MIDXBULD_ELMT_TBL_INFO_T);
        i4_ret = x_midxbuld_get_info(gh_midxbuld, 
                                     MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL, 
                                     (VOID*)&t_elmt_tbl_info, 
                                     &z_size);
        /* delay 500ms for get info done */
        x_thread_delay(500); 
        if (i4_ret != MIDXBULDR_OK)
        {
            x_dbg_stmt("midxbuld_cli_mp4_get_tbl ERROR!\r\n");
            return CLIR_CMD_EXEC_ERROR;
        }

        x_dbg_stmt("midxbuld_cli_mp4_get_tbl OK!\r\n");
        x_dbg_stmt("ui8_base_offset: 0x%.8X\r\n", (UINT32)t_elmt_tbl_info.t_elmt_tbl.ui8_base_offset);
        x_dbg_stmt("ui4_number_of_entry: %d\r\n", t_elmt_tbl_info.t_elmt_tbl.ui4_number_of_entry);
        x_dbg_stmt("e_es_type: %d\r\n", t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.e_es_type);
        x_dbg_stmt("ui4_strm_id: 0x%.8X\r\n", t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id);
        x_dbg_stmt("ui4_scale: %d\r\n", t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale);
        x_dbg_stmt("pt_idx_entry: 0x%.8X\r\n", t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.pt_idx_entry);
        pt_idx_entry = t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
        x_dbg_stmt("Video first index entry: (0x%.8X, 0x%.8X, 0x%.8X)\r\n", pt_idx_entry->ui4_pts, pt_idx_entry->ui4_relative_offset, pt_idx_entry->ui4_size);
        pt_idx_entry = t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.pt_idx_entry + t_elmt_tbl_info.t_elmt_tbl.ui4_number_of_entry - 1;
        x_dbg_stmt("Video last index entry: (0x%.8X, 0x%.8X, 0x%.8X)\r\n", pt_idx_entry->ui4_pts, pt_idx_entry->ui4_relative_offset, pt_idx_entry->ui4_size);

        /* audio */
        t_elmt_tbl_info.ui4_tbl_id = t_describ_01.ui4_tbl_id;
        z_size = sizeof(MIDXBULD_ELMT_TBL_INFO_T);
        i4_ret = x_midxbuld_get_info(gh_midxbuld, 
                                    MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL, 
                                    (VOID*)&t_elmt_tbl_info, 
                                    &z_size);
        /* delay 500ms for get info done */
        x_thread_delay(500); 
        if (i4_ret != MIDXBULDR_OK)
        {
            x_dbg_stmt("midxbuld_cli_mp4_get_tbl ERROR!\r\n");
            return CLIR_CMD_EXEC_ERROR;
        }

        x_dbg_stmt("midxbuld_cli_mp4_get_tbl OK!\r\n");
        x_dbg_stmt("ui8_base_offset: 0x%.8X\r\n", (UINT32)t_elmt_tbl_info.t_elmt_tbl.ui8_base_offset);
        x_dbg_stmt("ui4_number_of_entry: %d\r\n", t_elmt_tbl_info.t_elmt_tbl.ui4_number_of_entry);
        x_dbg_stmt("e_es_type: %d\r\n", t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.e_es_type);
        x_dbg_stmt("ui4_strm_id: 0x%.8X\r\n", t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id);
        x_dbg_stmt("ui4_scale: %d\r\n", t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale);
        x_dbg_stmt("pt_idx_entry: 0x%.8X\r\n", t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.pt_idx_entry);
        pt_idx_entry = t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
        x_dbg_stmt("Audio first index entry: (0x%.8X, 0x%.8X, 0x%.8X)\r\n", pt_idx_entry->ui4_pts, pt_idx_entry->ui4_relative_offset, pt_idx_entry->ui4_size);
        pt_idx_entry = t_elmt_tbl_info.t_elmt_tbl.u_fmt.t_es.pt_idx_entry + t_elmt_tbl_info.t_elmt_tbl.ui4_number_of_entry - 1;
        x_dbg_stmt("Audio last index entry: (0x%.8X, 0x%.8X, 0x%.8X)\r\n", pt_idx_entry->ui4_pts, pt_idx_entry->ui4_relative_offset, pt_idx_entry->ui4_size);

        /* free range */
        ui4_range_id = t_tbl_info.ui4_range_id;

        z_size = sizeof(UINT32);
        i4_ret = x_midxbuld_set_info(gh_midxbuld, 
                                    MIDXBULD_SET_INFO_TYPE_FREE_RANGE, 
                                    (VOID*)ui4_range_id, 
                                    &z_size);
        /* delay 200ms for set info done */
        x_thread_delay(200); 
        if (i4_ret != MIDXBULDR_OK)
        {
            x_dbg_stmt("midxbuld_cli_mp4_free_range ERROR!\r\n");
            return CLIR_CMD_EXEC_ERROR;
        }

        x_dbg_stmt("midxbuld_cli_mp4_free_range OK!\r\n");

        /* check and free mopen resource */
        if(gh_file != NULL_HANDLE)
        {
            x_dbg_stmt("close file handle for mopen \r\n");
            x_fm_close(gh_file);   
            gh_file = NULL_HANDLE;
        }
        if(gpui1_buf != NULL)
        {
            x_dbg_stmt("free memory for mopen  \r\n");
            _mm_util_mem_free(gpui1_buf);
            gpui1_buf = NULL;
        }
        
        /* close file */
        x_midxbuld_close(gh_midxbuld);
        
        /* delay 5000ms for one loop */
        x_thread_delay(5000); 
    }
    return CLIR_OK;
#else
    return CLIR_OK;
#endif
}
#endif

/*-----------------------------------------------------------------------------
 * Name: mp4_midxbuld_cli_init
 *
 * Description: This API initializes midxbuld CLI component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 mp4_midxbuld_cli_init(CLI_EXEC_T** ppt_command_array)
{
#ifdef CLI_LVL_ALL

    *ppt_command_array = at_mp4_midxbuld_cmd_tbl;
    
#endif    

return 0;
}

#else

INT32 mp4_midxbuld_cli_init(CLI_EXEC_T** ppt_command_array)
{
    return 0;
}

#endif/*end of #if (ENABLE_MM_CLI_TEST == 1)*/


