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
 * $RCSfile: avi_midxbuld_cli.c,v $
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

#include "inc/x_common.h"
#include "mutil/midxbuld/u_midxbuld.h"
#include "mutil/midxbuld/midxbuld.h"
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "cli/x_cli.h"
#include "dbg/x_dbg.h"

#else

#include "x_common.h"
#include "../u_midxbuld.h"
#include "../midxbuld.h"
#include "../../mm_util.h"
#include "x_cli.h"
#include "x_dbg.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
 
#ifdef CLI_LVL_ALL
static INT32 midxbuld_cli_avi_fopen(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_avi_mopen(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_avi_close(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_avi_start_idx(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_avi_stop_idx(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_avi_get_tbl(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_avi_alloc_range(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_avi_free_range(INT32 i4_argc, const CHAR** pps_argv);
/* mfmtrecg command table */
static CLI_EXEC_T at_avi_midxbuld_cmd_tbl[] =
{
    {"fopen",       NULL,  midxbuld_cli_avi_fopen,        NULL,    "midxbuld_fopen",       CLI_SUPERVISOR},
    {"mopen",       NULL,  midxbuld_cli_avi_mopen,        NULL,    "midxbuld_mopen",       CLI_SUPERVISOR},
    {"close",       NULL,  midxbuld_cli_avi_close,        NULL,    "midxbuld_close",       CLI_SUPERVISOR},
    {"start",       NULL,  midxbuld_cli_avi_start_idx,    NULL,    "midxbuld_start_idx",   CLI_SUPERVISOR},
    {"stop",        NULL,  midxbuld_cli_avi_stop_idx,     NULL,    "midxbuld_stop_idx",    CLI_SUPERVISOR},
    {"get_tbl",     NULL,  midxbuld_cli_avi_get_tbl,      NULL,    "midxbuld_get_tbl",     CLI_SUPERVISOR},
    {"alloc_range", NULL,  midxbuld_cli_avi_alloc_range,  NULL,    "midxbuld_alloc_range", CLI_SUPERVISOR}, 
    {"free_range",  NULL,  midxbuld_cli_avi_free_range,   NULL,    "midxbuld_free_range",  CLI_SUPERVISOR}, 
    END_OF_CLI_CMD_TBL
};

static HANDLE_T       gh_midxbuld;
static UINT32         gui4_key_elmt_tbl_id;
static BOOL           b_cbr;

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
 * Name: x_midxbuld_avi_nfy
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
static VOID x_midxbuld_avi_nfy (
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
        x_dbg_stmt("MIDXBULD_NFY_EVENT_INDEXING_DONE %d(0x%.8X)\r\n", (UINT32)pv_data1, (UINT32)pv_data1);
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
 * Name: midxbuld_cli_avi_fopen
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
static INT32 midxbuld_cli_avi_fopen(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;
    MEDIA_FORMAT_T t_format;
    CHAR* ps_file_path;
    MIDXBULD_SOURCE_INFO_T t_src_info;
    MIDXBULD_KEY_INFO_T t_key_info;

    if (i4_argc != 2)
    {
        x_dbg_stmt("Usage: fopen FILE...\r\n");
        return CLIR_INV_ARG;
    }

    ps_file_path = (CHAR*)pps_argv[1];

    t_format.e_media_type = MEDIA_TYPE_CONTAINER;
    t_format.t_media_subtype.u.e_contnr_subtype  = MEDIA_CONTNR_SUBTYPE_AVI;

    if (*ps_file_path == 'c') /* CBR */
    {
        ps_file_path = "/mnt/usb/Mass-000/a05_divx311fast_640x304_870kbps_mp3_44khz_cbr128.avi";
        
        t_src_info.ui8_data_offset = 0x2808;
        t_src_info.ui8_data_size = 0xF7A990;
        t_src_info.ui8_idx_offset = 0xF7D198;
        t_src_info.ui8_idx_size = 0x17590;
        t_src_info.ui1_total_strm_num = 2;
        t_src_info.ui4_total_duration = 11238739; /* dwLength*(dwScale/dwRate)*90000 or dwLength/dwAvgBytesPerSec*90000 */
        t_src_info.t_es_map[0].e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
        t_src_info.t_es_map[0].ui4_strm_id = 0x63643030;
        t_src_info.t_es_map[0].ui4_scale = 125;
        t_src_info.t_es_map[0].ui4_rate = 2997;
        t_src_info.t_es_map[0].ui4_sample_sz = 0;
        t_src_info.t_es_map[0].ui4_avg_bytes_per_sec = 0;
        t_src_info.t_es_map[1].e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
        t_src_info.t_es_map[1].ui4_strm_id = 0x62773130;
        t_src_info.t_es_map[1].ui4_scale = 1;
        t_src_info.t_es_map[1].ui4_rate = 16000;
        t_src_info.t_es_map[1].ui4_sample_sz = 1;
        t_src_info.t_es_map[1].ui4_avg_bytes_per_sec = 16000;
    
        t_key_info.ui4_strm_id = 0x63643030;
        t_key_info.ui4_scale = 125;
        t_key_info.ui4_rate = 2997;
        t_key_info.ui4_sample_sz = 0;
        t_key_info.ui4_avg_bytes_per_sec = 0;

        b_cbr = TRUE;
    }
    else /* VBR */
    {
        ps_file_path = "/mnt/usb/Mass-000/a06_divx311fast_640x304_870kbps_mp3_44khz_vbr128.avi";
        
        t_src_info.ui8_data_offset = 0x2808;
        t_src_info.ui8_data_size = 0xF7C7500;
        t_src_info.ui8_idx_offset = 0xF7EF58;
        t_src_info.ui8_idx_size = 0x1E5D0;
        t_src_info.ui1_total_strm_num = 2;
        t_src_info.ui4_total_duration = 11238739; /* dwLength*(dwScale/dwRate)*90000 */
        t_src_info.t_es_map[0].e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
        t_src_info.t_es_map[0].ui4_strm_id = 0x63643030;
        t_src_info.t_es_map[0].ui4_scale = 125;
        t_src_info.t_es_map[0].ui4_rate = 2997;
        t_src_info.t_es_map[0].ui4_sample_sz = 0;
        t_src_info.t_es_map[0].ui4_avg_bytes_per_sec = 0;
        t_src_info.t_es_map[1].e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
        t_src_info.t_es_map[1].ui4_strm_id = 0x62773130;
        t_src_info.t_es_map[1].ui4_scale = 1152;
        t_src_info.t_es_map[1].ui4_rate = 44100;
        t_src_info.t_es_map[1].ui4_sample_sz = 0;
        t_src_info.t_es_map[1].ui4_avg_bytes_per_sec = 15948;
    
        t_key_info.ui4_strm_id = 0x63643030;
        t_key_info.ui4_scale = 125;
        t_key_info.ui4_rate = 2997;
        t_key_info.ui4_sample_sz = 0;
        t_key_info.ui4_avg_bytes_per_sec = 0;

        b_cbr = FALSE;
    }

    i4_ret = x_midxbuld_fopen(ps_file_path, x_midxbuld_avi_nfy, (VOID*) -1, &t_format, &t_src_info, &t_key_info, &gh_midxbuld);
    if (i4_ret != MIDXBULDR_OK)
    {
        x_dbg_stmt("midxbuld_cli_avi_fopen ERROR!\r\n");
        return CLIR_CMD_EXEC_ERROR;
    }

    gui4_key_elmt_tbl_id = t_key_info.ui4_tbl_id;

    x_dbg_stmt("midxbuld_cli_avi_fopen OK!\r\n");
    x_dbg_stmt("key index range id: %d(0x%.8X)\r\n", (UINT32)t_key_info.ui4_range_id, (UINT32)t_key_info.ui4_range_id);
    x_dbg_stmt("key index element table 00 id: %d(0x%.8X)\r\n", (UINT32)t_key_info.ui4_tbl_id, (UINT32)t_key_info.ui4_tbl_id);
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_avi_mopen
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
static INT32 midxbuld_cli_avi_mopen(INT32 i4_argc, const CHAR** pps_argv)
{
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_avi_close
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
static INT32 midxbuld_cli_avi_close(INT32 i4_argc, const CHAR** pps_argv)
{
    x_midxbuld_close(gh_midxbuld);
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_avi_start_idx
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
static INT32 midxbuld_cli_avi_start_idx(INT32 i4_argc, const CHAR** pps_argv)
{
    x_midxbuld_start_indexing(gh_midxbuld);
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_avi_stop_idx
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
static INT32 midxbuld_cli_avi_stop_idx(INT32 i4_argc, const CHAR** pps_argv)
{
    x_midxbuld_stop_indexing(gh_midxbuld);
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_avi_get_tbl
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
static INT32 midxbuld_cli_avi_get_tbl(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;
    MIDXBULD_ELMT_TBL_INFO_T t_tbl_info;
    SIZE_T z_size;
    MIDXBULD_ELMT_STRM_ENTRY_T* pt_idx_entry;

    MIDXBULD_KEY_TBL_INFO_T  t_key_info;

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
    i4_ret = x_midxbuld_get_info(gh_midxbuld, MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL, (VOID*)&t_tbl_info, &z_size);
    if (i4_ret != MIDXBULDR_OK)
    {
        x_dbg_stmt("midxbuld_cli_avi_get_tbl ERROR!\r\n");
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("midxbuld_cli_avi_get_tbl OK!\r\n");
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

    t_key_info.ui4_key_tbl_id = gui4_key_elmt_tbl_id;
    i4_ret = x_midxbuld_get_info(gh_midxbuld, 
                                 MIDXBULD_GET_INFO_TYPE_KEY_IDX_TBL_LIST, 
                                 (VOID*)&t_key_info, 
                                 &z_size);
    if (i4_ret != MIDXBULDR_OK)
    {
        x_dbg_stmt("midxbuld_cli_avi_get_key_tbl ERROR!\r\n");
        return CLIR_CMD_EXEC_ERROR;
    }
    
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_avi_alloc_range
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
static INT32 midxbuld_cli_avi_alloc_range(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;
    MIDXBULD_ELMT_TBL_ALLOC_INFO_T t_tbl_info;
    MIDXBULD_ELMT_TBL_DESCRIB_T t_describ_00;
    MIDXBULD_ELMT_TBL_DESCRIB_T t_describ_01;
    SIZE_T z_size;

    if (i4_argc != 3)
    {
        x_dbg_stmt("Usage: alloc_tbl START_PTS END_PTS...\r\n");
        return CLIR_INV_ARG;
    }

    t_tbl_info.e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
    t_tbl_info.u_range.t_es.ui4_start_pts = (UINT32)_atoi(pps_argv[1]);;
    t_tbl_info.u_range.t_es.ui4_end_pts = (UINT32)_atoi(pps_argv[2]);;
    t_tbl_info.pt_tbl_describ = &t_describ_00;

    if (b_cbr)
    {
        t_describ_00.u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
        t_describ_00.u_info.t_es.ui4_strm_id = 0x63643030;
        t_describ_00.u_info.t_es.ui4_scale = 125;
        t_describ_00.u_info.t_es.ui4_rate = 2997;
        t_describ_00.u_info.t_es.ui4_sample_sz = 0;
        t_describ_00.u_info.t_es.ui4_avg_bytes_per_sec = 0;
        t_describ_00.pt_next = &t_describ_01;
        
        t_describ_01.u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
        t_describ_01.u_info.t_es.ui4_strm_id = 0x62773130;
        t_describ_01.u_info.t_es.ui4_scale = 1;
        t_describ_01.u_info.t_es.ui4_rate = 16000;
        t_describ_01.u_info.t_es.ui4_sample_sz = 1;
        t_describ_01.u_info.t_es.ui4_avg_bytes_per_sec = 16000;
        t_describ_01.pt_next = NULL;
    }
    else
    {
        t_describ_00.u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
        t_describ_00.u_info.t_es.ui4_strm_id = 0x63643030;
        t_describ_00.u_info.t_es.ui4_scale = 125;
        t_describ_00.u_info.t_es.ui4_rate = 2997;
        t_describ_00.u_info.t_es.ui4_sample_sz = 0;
        t_describ_00.u_info.t_es.ui4_avg_bytes_per_sec = 0;
        t_describ_00.pt_next = &t_describ_01;
        
        t_describ_01.u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
        t_describ_01.u_info.t_es.ui4_strm_id = 0x62773130;
        t_describ_01.u_info.t_es.ui4_scale = 1152;
        t_describ_01.u_info.t_es.ui4_rate = 44100;
        t_describ_01.u_info.t_es.ui4_sample_sz = 0;
        t_describ_01.u_info.t_es.ui4_avg_bytes_per_sec = 15948;
        t_describ_01.pt_next = NULL;
    }

    z_size = sizeof(MIDXBULD_ELMT_TBL_ALLOC_INFO_T);
    i4_ret = x_midxbuld_set_info(gh_midxbuld, MIDXBULD_SET_INFO_TYPE_ALLOC_RANGE, (VOID*)&t_tbl_info, &z_size);
    if (i4_ret != MIDXBULDR_OK)
    {
        x_dbg_stmt("midxbuld_cli_avi_alloc_range ERROR!\r\n");
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("index range id: %d(0x%.8X)\r\n", (UINT32)t_tbl_info.ui4_range_id, (UINT32)t_tbl_info.ui4_range_id);
    x_dbg_stmt("index element table 00 id: %d(0x%.8X)\r\n", (UINT32)t_describ_00.ui4_tbl_id, (UINT32)t_describ_00.ui4_tbl_id);
    if (t_describ_00.pt_next != NULL)
    {
        x_dbg_stmt("index element table 01 id: %d(0x%.8X)\r\n", (UINT32)t_describ_01.ui4_tbl_id, (UINT32)t_describ_01.ui4_tbl_id);
    }
    
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_avi_free_range
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
static INT32 midxbuld_cli_avi_free_range(INT32 i4_argc, const CHAR** pps_argv)
{
    UINT32 ui4_range_id;
    INT32 i4_ret;
    SIZE_T z_size;

    if (i4_argc != 2)
    {
        x_dbg_stmt("Usage: free_range RANGE_ID...\r\n");
        return CLIR_INV_ARG;
    }

    ui4_range_id = (UINT32)_atoi(pps_argv[1]);

    z_size = sizeof(UINT32);
    i4_ret = x_midxbuld_set_info(gh_midxbuld, MIDXBULD_SET_INFO_TYPE_FREE_RANGE, (VOID*)ui4_range_id, &z_size);
    if (i4_ret != MIDXBULDR_OK)
    {
        x_dbg_stmt("midxbuld_cli_avi_free_range ERROR!\r\n");
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("midxbuld_cli_avi_free_range OK!\r\n");
    
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: avi_midxbuld_cli_init
 *
 * Description: This API initializes midxbuld CLI component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 avi_midxbuld_cli_init(CLI_EXEC_T **ppt_command_array)
{
#ifdef CLI_LVL_ALL

    *ppt_command_array = at_avi_midxbuld_cmd_tbl;
#endif
    return 0;
}

