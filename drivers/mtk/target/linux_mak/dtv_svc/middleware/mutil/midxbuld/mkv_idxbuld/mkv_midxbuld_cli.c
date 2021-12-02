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
 * $RCSfile: mkv_midxbuld_cli.c,v $
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
#include "inc/x_mm_common.h"
#include "mutil/midxbuld/u_midxbuld.h"
#include "mutil/midxbuld/midxbuld.h"
#include "mutil/midxbuld/x_midxbuld.h"
#include "mutil/mm_common/rfc_ebml/rfc_ebml.h"
#include "mutil/mm_util_input.h"
#include "mutil/mm_util_memory.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif

#include "cli/x_cli.h"
#include "dbg/x_dbg.h"
#include "dbg/u_dbg.h"

#else

#include "x_common.h"
#include "x_mm_common.h"
#include "../u_midxbuld.h"
#include "../midxbuld.h"
#include "../x_midxbuld.h"
#include "../../mm_common/rfc_ebml/rfc_ebml.h"
#include "../../mm_util_input.h"
#include "../../mm_util_memory.h"

#include "../../mm_util.h"

#include "x_cli.h"
#include "x_dbg.h"
#include "u_dbg.h"

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
 
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
 
#ifdef CLI_LVL_ALL
static VOID x_midxbuld_mkv_nfy (
                                HANDLE_T                  h_midxbuld, 
                                MIDXBULD_NFY_EVENT_TYPE_T e_event,  
                                const VOID*               pv_data1, 
                                const VOID*               pv_data2, 
                                const VOID*               pv_tag);
static INT32 midxbuld_cli_mkv_fopen(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_mkv_mopen(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_mkv_close(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_mkv_start_idx(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_mkv_stop_idx(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_mkv_get_tbl(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_mkv_alloc_range(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_mkv_free_range(INT32 i4_argc, const CHAR** pps_argv);
static INT32 midxbuld_cli_mkv_stress(INT32 i4_argc, const CHAR** pps_argv);


/* midxbuld command table */
static CLI_EXEC_T at_mkv_midxbuld_cmd_tbl[] =
{
    {
        "fopen",       NULL,  
        midxbuld_cli_mkv_fopen,        
        NULL,    
        "midxbuld_fopen",       
        CLI_SUPERVISOR
    },
    {
        "mopen",       NULL,  
        midxbuld_cli_mkv_mopen,        
        NULL,    
        "midxbuld_mopen",       
        CLI_SUPERVISOR
    },
    {
        "close",       NULL,  
        midxbuld_cli_mkv_close,        
        NULL,    
        "midxbuld_close",       
        CLI_SUPERVISOR
    },
    {
        "start",       NULL,  
        midxbuld_cli_mkv_start_idx,    
        NULL,    
        "midxbuld_start_idx",   
        CLI_SUPERVISOR
    },
    {
        "stop",        NULL,  
        midxbuld_cli_mkv_stop_idx,     
        NULL,    
        "midxbuld_stop_idx",    
        CLI_SUPERVISOR
    },
    {
        "get_tbl",     NULL,  
        midxbuld_cli_mkv_get_tbl,      
        NULL,    
        "midxbuld_get_tbl",     
        CLI_SUPERVISOR
    },
    {
        "alloc_range", NULL,  
        midxbuld_cli_mkv_alloc_range,  
        NULL,    
        "midxbuld_alloc_range", 
        CLI_SUPERVISOR
    }, 
    {
        "free_range",  NULL,  
        midxbuld_cli_mkv_free_range,   
        NULL,    
        "midxbuld_free_range",  
        CLI_SUPERVISOR
    }, 
    {
        "istress",       NULL,  
        midxbuld_cli_mkv_stress,        
        NULL,    
        "midxbuld_stress",       
        CLI_SUPERVISOR
    },
    END_OF_CLI_CMD_TBL
};

static HANDLE_T       gh_midxbuld;
static UINT32         gui4_key_elmt_tbl_id;
static MEDIA_FEEDER_T gt_clip_obj;  
static VOID*          gpv_clip_buf = NULL;
static UINT64         gi8_clip_len = 0;
static UINT64         ui8_seg_timecode_scale;
static BOOL           b_build_done;
static BOOL           b_build_abort;
#endif
/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL

/*-----------------------------------------------------------------------------
 * Name: x_midxbuld_mkv_nfy
 *
 * Description: 
 *
 * Inputs:  
 *            HANDLE_T                  h_midxbuld, 
 *            MIDXBULD_NFY_EVENT_TYPE_T e_event,  
 *            const VOID*               pv_data1, 
 *            const VOID*               pv_data2, 
 *            const VOID*               pv_tag
 *
 * Outputs: NULL
 *
 * Returns: NULL
 ----------------------------------------------------------------------------*/
static VOID x_midxbuld_mkv_nfy (
                        HANDLE_T                  h_midxbuld, 
                        MIDXBULD_NFY_EVENT_TYPE_T e_event,  
                        const VOID*               pv_data1, 
                        const VOID*               pv_data2, 
                        const VOID*               pv_tag)
{    

    switch (e_event)
    {
        case MIDXBULD_NFY_EVENT_INDEXING_DONE:
            x_dbg_stmt("MIDXBULD_NFY_EVENT_INDEXING_DONE %d(0x%.8X)\r\n", 
                      (UINT32)pv_data1, 
                      (UINT32)pv_data1);
            b_build_done = TRUE;
            break;
        case MIDXBULD_NFY_EVENT_INDEXING_ABORT:
            x_dbg_stmt("MIDXBULD_NFY_EVENT_INDEXING_ABORT\r\n");
            b_build_abort = TRUE;
            break;
        case MIDXBULD_NFY_EVENT_HANDLE_CLOSED:
            x_dbg_stmt("MIDXBULD_NFY_EVENT_HANDLE_CLOSED\r\n");
            b_build_abort = TRUE;
            break;
        case MIDXBULD_NFY_EVENT_ERROR:
            x_dbg_stmt("MIDXBULD_NFY_EVENT_ERROR\r\n");
            b_build_abort = TRUE;
            break;
        default:
            break;
    }
    return;
}

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_mkv_fopen
 *
 * Description: 
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 midxbuld_cli_mkv_fopen(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32                  i4_ret;
    MEDIA_FORMAT_T         t_format;
    CHAR*                  psz_file_path;
    MIDXBULD_SOURCE_INFO_T t_src_info;
    MIDXBULD_KEY_INFO_T    t_key_info;
    UINT32                 ui4_i;

    if ((NULL == pps_argv) || (NULL == *pps_argv))
    {
        DBG_ERROR(("Invalid Parameters\r\n"));
        return CLIR_INV_ARG;
    }
    
    if (0 == x_strncmp(pps_argv[1], "?", (SIZE_T)1))
    {
        DBG_ERROR(("Usage: fopen filepath  segment_data_offset segment_tm_scale\r\n"
                   "cue_offset     cue_size\r\n"
                   "total_stream   duration\r\n"
                   "stream_type_1  stream_id_1  "
                   "time_scale_1   time_rate_1  is_key_1(1/0)\r\n"
                   "stream_type_2  stream_id_2  "
                   "time_scale_2   time_rate_2  is_key_2(1/0)\r\n"
                   "...    ...\r\n"));
        return CLIR_OK;
        
    }
    
    /*at least 13 parameters*/
    if (i4_argc < 13)
    {
        DBG_ERROR(("Usage: fopen filepath  segment_data_offset segment_tm_scale\r\n"
                   "cue_offset     cue_size\r\n"
                   "total_stream   duration\r\n"
                   "stream_type_1  stream_id_1  "
                   "time_scale_1   time_rate_1  is_key_1(1/0)\r\n"
                   "stream_type_2  stream_id_2  "
                   "time_scale_2   time_rate_2  is_key_2(1/0)\r\n"
                   "...    ...\r\n"));
        return CLIR_INV_ARG;
    }
    
    /*get filepath*/
    psz_file_path = (CHAR*)pps_argv[1];
    if (NULL == psz_file_path)
    {
        return CLIR_INV_ARG;
    }
    b_build_done = FALSE;
    b_build_abort = FALSE;
    /*get data offset*/
    t_src_info.ui8_seg_data_offset = x_strtoull(pps_argv[2], NULL, 10);
    /*get data size*/
    t_src_info.ui8_seg_tm_scale = x_strtoull(pps_argv[3], NULL, 10);
    ui8_seg_timecode_scale = t_src_info.ui8_seg_tm_scale;
    /*get index offset*/
    t_src_info.ui8_idx_offset = x_strtoull(pps_argv[4], NULL, 10);
    /*get index size*/
    t_src_info.ui8_idx_size = x_strtoull(pps_argv[5], NULL, 10);
    /*get total streams*/
    t_src_info.ui1_total_strm_num = (UINT8)x_strtoull(pps_argv[6], NULL, 10);
    /*total duration*/
    t_src_info.ui4_total_duration = (UINT32)x_strtoull(pps_argv[7], NULL, 10);
    /*packet size*/
    t_src_info.ui4_packet_sz = 0;

    /*set stream info 0*/
    x_memset(t_src_info.t_es_map, 
             0, 
             sizeof(MIDXBULD_ELMT_STRM_TBL_INFO_T) 
              * MIDXBULD_ELMT_TBL_MAX);
    
    /*for the 7th place is stream info.*/
    for (ui4_i = 0; ui4_i < t_src_info.ui1_total_strm_num; ++ui4_i)
    {
        if (ui4_i >= MIDXBULD_ELMT_TBL_MAX)
        {
            DBG_ERROR(("midxbuld_cli_mkv_fopen: idx error!\r\n"));
            return CLIR_CMD_EXEC_ERROR;
        }
        /*stream type*/
        if ((0 == x_strncmp(pps_argv[8 + 5 * ui4_i], "v", (SIZE_T)1))
            || (0 == x_strncmp(pps_argv[8 + 5 * ui4_i], "V", (SIZE_T)1)))
        {
            t_src_info.t_es_map[ui4_i].e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
        }
        /*consider audio*/
        else if ((0 == x_strncmp(pps_argv[8 + 5 * ui4_i], "a", (SIZE_T)1))
                  || (0 == x_strncmp(pps_argv[8 + 5 * ui4_i], "A", (SIZE_T)1)))
        {
            t_src_info.t_es_map[ui4_i].e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
        }
        else
        {
            t_src_info.t_es_map[ui4_i].e_es_type = 
                              MIDXBULD_ELEM_STRM_TYPE_SUBTITLE;
        }
        /*get stream id*/
        t_src_info.t_es_map[ui4_i].ui4_strm_id = 
                               (UINT32)x_strtoull(pps_argv[8 + 5 * ui4_i + 1], NULL, 10);
        /*time scale*/
        t_src_info.t_es_map[ui4_i].ui4_scale = 
                               (UINT32)x_strtoull(pps_argv[8 + 5 * ui4_i + 2], NULL, 10);
        /*time ui4_rate*/
        t_src_info.t_es_map[ui4_i].ui4_rate = 
                               (UINT32)x_strtoull(pps_argv[8 + 5 * ui4_i + 3], NULL, 10);
        /*is key */
        if (0 == x_strncmp(pps_argv[8 + 5 * ui4_i + 4], "1", (SIZE_T)1))
        {
            t_key_info.ui4_strm_id = t_src_info.t_es_map[ui4_i].ui4_strm_id;
            t_key_info.ui4_scale = t_src_info.t_es_map[ui4_i].ui4_scale;
            t_key_info.ui4_rate = t_src_info.t_es_map[ui4_i].ui4_rate;
        }
        /*others set 0*/
        t_src_info.t_es_map[ui4_i].ui4_sample_sz = 0;
        t_src_info.t_es_map[ui4_i].ui4_avg_bytes_per_sec = 0;
          
    }

    /*other fields set 0*/
    t_key_info.ui4_sample_sz = 0;
    t_key_info.ui4_avg_bytes_per_sec = 0;
    t_key_info.ui4_range_id = 0xff;
    t_key_info.ui4_tbl_id = 0xff;

    /*set format type*/
    t_format.e_media_type = MEDIA_TYPE_CONTAINER;
    t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_MATROSKA;


    /*call fopen*/
    i4_ret = x_midxbuld_fopen(psz_file_path, 
                              x_midxbuld_mkv_nfy, 
                              (VOID*) -1, 
                              &t_format, 
                              &t_src_info, 
                              &t_key_info, 
                              &gh_midxbuld);
    if (i4_ret != MIDXBULDR_OK)
    {
        x_dbg_stmt("midxbuld_cli_mkv_fopen: ERROR!\r\n");
        return CLIR_CMD_EXEC_ERROR;
    }

    gui4_key_elmt_tbl_id = t_key_info.ui4_tbl_id;

    x_dbg_stmt("midxbuld_cli_mkv_fopen: OK!\r\n");
    x_dbg_stmt("key index range id: %d(0x%.8X)\r\n", 
             (UINT32)t_key_info.ui4_range_id, 
             (UINT32)t_key_info.ui4_range_id);
    x_dbg_stmt("key index element table 00 id: %d(0x%.8X)\r\n", 
             (UINT32)t_key_info.ui4_tbl_id, 
             (UINT32)t_key_info.ui4_tbl_id);
    
    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_mkv_mopen
 *
 * Description: 
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 midxbuld_cli_mkv_mopen(INT32 i4_argc, const CHAR** pps_argv)
{
#ifndef __NO_FM__
    INT32                  i4_ret;
    MEDIA_FORMAT_T         t_format;
    CHAR*                  psz_file_path;
    MIDXBULD_SOURCE_INFO_T t_src_info;
    MIDXBULD_KEY_INFO_T    t_key_info;
    UINT32                 ui4_i;
    BOOL                   b_zero_len;
    FM_FILE_INFO_T         t_fm_info;
    UINT64                 ui8_cur_pos;    
    UINT32                 ui4_clip_len;
    UINT32                 ui4_read;      

    if ((NULL == pps_argv) || (NULL == *pps_argv))
    {
        DBG_ERROR(("Invalid Parameters\r\n"));
        return CLIR_INV_ARG;
    }
    
    if (0 == x_strncmp(pps_argv[1], "?", (SIZE_T)1))
    {
        DBG_ERROR(("Usage: fopen filepath  zero_len(0/1) segment_data_offset segment_tm_scale\r\n"
                   "cue_offset     cue_size\r\n"
                   "total_stream   duration\r\n"
                   "stream_type_1  stream_id_1  "
                   "time_scale_1   time_rate_1  is_key_1(1/0)\r\n"
                   "stream_type_2  stream_id_2  "
                   "time_scale_2   time_rate_2  is_key_2(1/0)\r\n"
                   "...    ...\r\n"));
        return CLIR_OK;
        
    }
    
    /*at least 14 parameters*/
    if (i4_argc < 14)
    {
        DBG_ERROR(("Usage: fopen filepath  zero_len(0/1) segment_data_offset segment_tm_scale\r\n"
                   "cue_offset     cue_size\r\n"
                   "total_stream   duration\r\n"
                   "stream_type_1  stream_id_1  "
                   "time_scale_1   time_rate_1  is_key_1(1/0)\r\n"
                   "stream_type_2  stream_id_2  "
                   "time_scale_2   time_rate_2  is_key_2(1/0)\r\n"
                   "...    ...\r\n"));
        return CLIR_INV_ARG;
    }
    
    /*get filepath*/
    psz_file_path = (CHAR*)pps_argv[1];
    if (NULL == psz_file_path)
    {
        return CLIR_INV_ARG;
    }
    
    /*get zero length flag*/
    if (0 == x_strncmp(pps_argv[2], "0", (SIZE_T)1))
    {
        b_zero_len = TRUE;
    }
    else
    {
        b_zero_len = FALSE;
    }
    b_build_abort = FALSE;
    b_build_done = FALSE;
    /*get data offset*/
    t_src_info.ui8_seg_data_offset = x_strtoull(pps_argv[3], NULL, 10);
    /*get data size*/
    t_src_info.ui8_seg_tm_scale = x_strtoull(pps_argv[4], NULL, 10);
    ui8_seg_timecode_scale = t_src_info.ui8_seg_tm_scale;
    /*get index offset*/
    t_src_info.ui8_idx_offset = x_strtoull(pps_argv[5], NULL, 10);
    /*get index size*/
    t_src_info.ui8_idx_size = x_strtoull(pps_argv[6], NULL, 10);
    /*get total streams*/
    t_src_info.ui1_total_strm_num = (UINT8)x_strtoull(pps_argv[7], NULL, 10);
    /*total duration*/
    t_src_info.ui4_total_duration = (UINT32)x_strtoull(pps_argv[8], NULL, 10);
    /*packet size*/
    t_src_info.ui4_packet_sz = 0;

    /*set stream info 0*/
    x_memset(t_src_info.t_es_map, 
             0, 
             sizeof(MIDXBULD_ELMT_STRM_TBL_INFO_T) 
              * MIDXBULD_ELMT_TBL_MAX);
    
    /*for the 9th place is stream info.*/
    for (ui4_i = 0; ui4_i < t_src_info.ui1_total_strm_num; ++ui4_i)
    {
        if (ui4_i >= MIDXBULD_ELMT_TBL_MAX)
        {
            DBG_ERROR(("midxbuld_cli_mkv_mopen: idx error!\r\n"));
            return CLIR_CMD_EXEC_ERROR;
        }
        /*stream type*/
        if ((0 == x_strncmp(pps_argv[9 + 5 * ui4_i], "v", (SIZE_T)1))
            || (0 == x_strncmp(pps_argv[9 + 5 * ui4_i], "V", (SIZE_T)1)))
        {
            t_src_info.t_es_map[ui4_i].e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
        }
        /*consider audio*/
        else if ((0 == x_strncmp(pps_argv[9 + 5 * ui4_i], "a", (SIZE_T)1))
                  || (0 == x_strncmp(pps_argv[9 + 5 * ui4_i], "A", (SIZE_T)1)))
        {
            t_src_info.t_es_map[ui4_i].e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
        }
        else
        {
            t_src_info.t_es_map[ui4_i].e_es_type = 
                              MIDXBULD_ELEM_STRM_TYPE_SUBTITLE;
        }
        /*get stream id*/
        t_src_info.t_es_map[ui4_i].ui4_strm_id = 
                               (UINT32)x_strtoull(pps_argv[9 + 5 * ui4_i + 1], NULL, 10);
        /*is key ?*/
        /*time scale*/
        t_src_info.t_es_map[ui4_i].ui4_scale = 
                               (UINT32)x_strtoull(pps_argv[9 + 5 * ui4_i + 2], NULL, 10);
        /*time ui4_rate*/
        t_src_info.t_es_map[ui4_i].ui4_rate = 
                               (UINT32)x_strtoull(pps_argv[9 + 5 * ui4_i + 3], NULL, 10);
        /*is key */
        if (0 == x_strncmp(pps_argv[9 + 5 * ui4_i + 4], "1", (SIZE_T)1))
        {
            t_key_info.ui4_strm_id = t_src_info.t_es_map[ui4_i].ui4_strm_id;
            t_key_info.ui4_scale = t_src_info.t_es_map[ui4_i].ui4_scale;
            t_key_info.ui4_rate = t_src_info.t_es_map[ui4_i].ui4_rate;
        }
        /*others set 0*/
        t_src_info.t_es_map[ui4_i].ui4_sample_sz = 0;
        t_src_info.t_es_map[ui4_i].ui4_avg_bytes_per_sec = 0;
          
    }

    /*other fields set 0*/
    t_key_info.ui4_sample_sz = 0;
    t_key_info.ui4_avg_bytes_per_sec = 0;
    t_key_info.ui4_range_id = 0xff;
    t_key_info.ui4_tbl_id = 0xff;

    /*set format type*/
    t_format.e_media_type = MEDIA_TYPE_CONTAINER;
    t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_MATROSKA;

    /*call fopen*/
    /*set clip object*/
    x_memset(&gt_clip_obj, 0, sizeof(gt_clip_obj));
    i4_ret = x_fm_open(FM_ROOT_HANDLE, 
                       psz_file_path,
                       FM_READ_ONLY, 
                       0777, FALSE, 
                       &(gt_clip_obj.h_feeder));
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("midxbuld_cli_mkv_mopen:open file ERROR!!!\r\n"));
        return i4_ret;
    }
    
    if (TRUE == b_zero_len)
    {
        gt_clip_obj.t_feeder_le_fct_tbl.pf_copybytes = cli_feeder_copybytes_fct;
        gt_clip_obj.t_feeder_le_fct_tbl.pf_set_pos = cli_feeder_setpos_fct;
        gt_clip_obj.t_feeder_le_fct_tbl.pf_get_pos = cli_feeder_getpos_fct;
        gt_clip_obj.t_feeder_le_fct_tbl.pf_input1 = cli_feeder_input1bytes_fct;
        gt_clip_obj.t_feeder_le_fct_tbl.pf_input2 = cli_feeder_input2bytes_b_fct;
        gt_clip_obj.t_feeder_le_fct_tbl.pf_input3 = cli_feeder_input3bytes_b_fct;
        gt_clip_obj.t_feeder_le_fct_tbl.pf_input4 = cli_feeder_input4bytes_b_fct;
        
        i4_ret = x_midxbuld_mopen((VOID*)&gt_clip_obj, 
                                  0,
                                  x_midxbuld_mkv_nfy, 
                                  (VOID*) -1, 
                                  &t_format, 
                                  &t_src_info, 
                                  &t_key_info, 
                                  &gh_midxbuld);
    }
    else
    {
        x_fm_get_info_by_handle(gt_clip_obj.h_feeder, &t_fm_info);
        ui4_clip_len = (SIZE_T)t_fm_info.ui8_size;
        if (ui4_clip_len < 2*1024)
        {
            if ((NULL != gpv_clip_buf) && ((UINT64)ui4_clip_len < gi8_clip_len))
            {
                /*nothin*/
            }
            else
            {
                if (NULL != gpv_clip_buf)
                {
                    _mm_util_mem_free_with_tag(gpv_clip_buf, MM_MEM_MIDX_TAG);
                }
                gpv_clip_buf = _mm_util_mem_alloc_with_tag(ui4_clip_len, 
                                                           MM_MEM_MIDX_TAG);
                if ( NULL == gpv_clip_buf )
                {
                    DBG_ERROR(("midxbuld_cli_mkv_mopen: alloc NULL\r\n"));
                    return MIDXBULDR_OUT_OF_MEMORY;
                }
                gi8_clip_len = ui4_clip_len;
            }
            i4_ret = x_fm_lseek(gt_clip_obj.h_feeder, 
                                0, FM_SEEK_BGN, &ui8_cur_pos);
            if (FMR_OK != i4_ret)
            {
                DBG_ERROR(("midxbuld_cli_mkv_mopen:x_fm_lseek error(%d)\r\n", 
                           i4_ret));
                return MIDXBULDR_FILE_OR_DIR_MISSING;
            }
            
            i4_ret = x_fm_read(gt_clip_obj.h_feeder, 
                               gpv_clip_buf, ui4_clip_len, 
                              &ui4_read);
            if (i4_ret != FMR_OK)
            {
                DBG_ERROR(("asf_handler_file_confirm_fct: x_fm_read "
                           "failed(%d)\r\n", 
                           i4_ret));
                return MFMTRECGR_INTERNAL_ERR;
            }
        }
        else
        {
            x_dbg_stmt("midxbuld_cli_mkv_mopen: file is too big !!!\r\n");
            return CLIR_CMD_EXEC_ERROR;
        }
        i4_ret = x_midxbuld_mopen(gpv_clip_buf, 
                                  ui4_clip_len,
                                  x_midxbuld_mkv_nfy, 
                                  (VOID*) -1, 
                                  &t_format, 
                                  &t_src_info, 
                                  &t_key_info, 
                                  &gh_midxbuld);
    }

    if (i4_ret != MIDXBULDR_OK)
    {
        x_dbg_stmt("midxbuld_cli_mkv_mopen: ERROR!!!\r\n");
        return CLIR_CMD_EXEC_ERROR;
    }

    gui4_key_elmt_tbl_id = t_key_info.ui4_tbl_id;

    x_dbg_stmt("midxbuld_cli_mkv_mopen: OK!\r\n");
    x_dbg_stmt("key index range id: %d(0x%.8X)\r\n", 
             (UINT32)t_key_info.ui4_range_id, 
             (UINT32)t_key_info.ui4_range_id);
    x_dbg_stmt("key index element table 00 id: %d(0x%.8X)\r\n", 
             (UINT32)t_key_info.ui4_tbl_id, 
             (UINT32)t_key_info.ui4_tbl_id);

    return CLIR_OK;
#else
        return CLIR_CMD_EXEC_ERROR;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_mkv_close
 *
 * Description: 
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 midxbuld_cli_mkv_close(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32 i4_ret;
    
    i4_ret = x_midxbuld_close(gh_midxbuld);
#ifndef __NO_FM__        
        x_fm_close(gt_clip_obj .h_feeder);
#endif
    if (NULL != gpv_clip_buf)
    {
        _mm_util_mem_free_with_tag(gpv_clip_buf, MM_MEM_MIDX_TAG);
        gpv_clip_buf = NULL;
    }
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_mkv_start_idx
 *
 * Description: 
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 midxbuld_cli_mkv_start_idx(INT32 i4_argc, const CHAR** pps_argv)
{
    x_midxbuld_start_indexing(gh_midxbuld);
    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_mkv_stop_idx
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
static INT32 midxbuld_cli_mkv_stop_idx(INT32 i4_argc, const CHAR** pps_argv)
{
    x_midxbuld_stop_indexing(gh_midxbuld);
    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_mkv_get_tbl
 *
 * Description: This API gets the info of idx builder.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 midxbuld_cli_mkv_get_tbl(INT32 i4_argc, const CHAR** pps_argv)
{
    INT32                       i4_ret;
    MIDXBULD_ELMT_TBL_INFO_T    t_tbl_info;
    SIZE_T                      z_size;
    MIDXBULD_ELMT_STRM_ENTRY_T* pt_idx_entry;
    UINT32                      ui4_idx;

    if (!i4_argc)
    {
        DBG_ERROR(("Usage: get_info TABLE_ID\r\n"));
        return CLIR_INV_ARG;
    }
    if (i4_argc == 1)
    {
        t_tbl_info.ui4_tbl_id = gui4_key_elmt_tbl_id;
    }
    else if (i4_argc == 2)
    {
        t_tbl_info.ui4_tbl_id = (UINT32)x_strtoull(pps_argv[1], NULL, 10);
    }
    else
    {
        x_dbg_stmt("Usage: get_info TABLE_ID\r\n");
        return CLIR_INV_ARG;
    }

    z_size = sizeof(MIDXBULD_ELMT_TBL_INFO_T);
    i4_ret = x_midxbuld_get_info(gh_midxbuld, 
                                 MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL, 
                                (VOID*)&t_tbl_info, &z_size);
    if (MIDXBULDR_OK != i4_ret)
    {
        DBG_ERROR(("midxbuld_cli_mkv_get_tbl ERROR!\r\n"));
        return CLIR_CMD_EXEC_ERROR;
    }

    /*print info*/
    x_dbg_stmt("midxbuld_cli_mkv_get_tbl OK!\r\n");
    x_dbg_stmt("ui8_base_offset: 0x%.8X\r\n", 
             (UINT32)t_tbl_info.t_elmt_tbl.ui8_base_offset);
    x_dbg_stmt("ui4_number_of_entry: %u\r\n", 
              t_tbl_info.t_elmt_tbl.ui4_number_of_entry);
    x_dbg_stmt("e_es_type: %d\r\n", 
              t_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.e_es_type);
    x_dbg_stmt("ui4_strm_id: 0x%.8X\r\n", 
              t_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id);
    x_dbg_stmt("ui4_scale: %u\r\n", 
              t_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale);
    x_dbg_stmt("ui4_rate: %u\r\n", 
              t_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.ui4_rate);
    x_dbg_stmt("ui4_sample_sz: %u\r\n", 
              t_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.ui4_sample_sz);
    x_dbg_stmt("ui4_avg_bytes_per_sec: %u\r\n", 
              t_tbl_info.t_elmt_tbl.u_fmt.t_es.t_info.ui4_avg_bytes_per_sec);
    x_dbg_stmt("pt_idx_entry: 0x%.8X\r\n\r\n\r\n", 
              t_tbl_info.t_elmt_tbl.u_fmt.t_es.pt_idx_entry);

    if (!ui8_seg_timecode_scale)
    {
        x_dbg_stmt("ui8_seg_timecode_scale is 0!\r\n");
        return CLIR_INV_ARG;
    }
    
    for (ui4_idx = 0 ; ui4_idx < t_tbl_info.t_elmt_tbl.ui4_number_of_entry; ++ui4_idx)
    {
        pt_idx_entry = t_tbl_info.t_elmt_tbl.u_fmt.t_es.pt_idx_entry 
                       + ui4_idx;
        
        x_dbg_stmt("Index entry %.3d    (%.10d,      %.10d,      %.10d)\r\n", 
                  ui4_idx, 
                  (UINT32)_mm_div64(((UINT64)pt_idx_entry->ui4_pts * 100000),
                                    (ui8_seg_timecode_scale * 9),
                                    NULL),
                  pt_idx_entry->ui4_relative_offset, 
                  pt_idx_entry->ui4_size);
    }
    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_mkv_alloc_range
 *
 * Description: This API sets the info of idx builder.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 midxbuld_cli_mkv_alloc_range(INT32 i4_argc, const CHAR** pps_argv)
{
    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_mkv_free_range
 *
 * Description: This API sets the info of idx builder.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 midxbuld_cli_mkv_free_range(INT32 i4_argc, const CHAR** pps_argv)
{
    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: midxbuld_cli_mkv_stress
 *
 * Description: 
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 midxbuld_cli_mkv_stress(INT32 i4_argc, const CHAR** pps_argv)
{
#if 1
    INT32 i4_ret;
    i4_ret = midxbuld_cli_mkv_fopen(i4_argc, pps_argv);
    if (i4_ret != CLIR_OK)
    {
        x_dbg_stmt("midxbuld_cli_mkv_stress: idxbuld open error!\r\n");
        return CLIR_CMD_EXEC_ERROR;
    }
    while ((!b_build_done) && (!b_build_abort))
    {
        x_dbg_stmt("now mkv idxbuild isn't complete, so wait!\r\n");
        x_thread_delay(10);
    }
    i4_ret = midxbuld_cli_mkv_get_tbl(1, NULL);
    if (i4_ret != CLIR_OK)
    {
        x_dbg_stmt("midxbuld_cli_mkv_stress: get table error!\r\n");
        return CLIR_CMD_EXEC_ERROR;
    }
    i4_ret = midxbuld_cli_mkv_close(1, NULL);
    if (i4_ret != CLIR_OK)
    {
        x_dbg_stmt("midxbuld_cli_mkv_stress: idxbuld close error!\r\n");
        return CLIR_CMD_EXEC_ERROR;
    }
    return i4_ret;
#else
    UINT32     ui4_loop_count;
    BOOL       b_dead_loop;
    /*INT32      i4_ret;*/
    
    /*parameters check*/
    if ((3 != i4_argc) || (NULL == pps_argv))
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        x_dbg_stmt("usage:mstress f/m(fopen/mopen) count\r\n");
        return CLIR_INV_ARG;
    }

    /*************************Do***********************************/
   
    /*get loop count*/
    ui4_loop_count = x_strtoll(pps_argv[2], NULL, 10);
    /*if 0 set dead loop*/
    if (0 == ui4_loop_count)
    {
        b_dead_loop = TRUE;
    }
    else
    {
        b_dead_loop = FALSE;
    }

    while (1)
    {
        /*To do ...*/


        /*loop control*/
        --ui4_loop_count;
        if (TRUE != b_dead_loop)
        {
            if (0 == ui4_loop_count)
            {
                break;
            }
        }
        else
        {
            
        }
    }
     
    return CLIR_OK;   
#endif
}
#endif

/*-----------------------------------------------------------------------------
 * Name: mkv_midxbuld_cli_init
 *
 * Description: This API initializes midxbuld CLI component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 mkv_midxbuld_cli_init(CLI_EXEC_T **ppt_command_array)
{
#ifdef CLI_LVL_ALL

    *ppt_command_array = at_mkv_midxbuld_cmd_tbl;
    
#endif

return 0;

}



