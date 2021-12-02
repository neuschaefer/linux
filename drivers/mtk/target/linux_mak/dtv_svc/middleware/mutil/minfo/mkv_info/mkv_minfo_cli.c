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
 * $RCSfile: mkv_minfo_cli.c,v $
 * $Revision:
 * $Date:
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_ATSC/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 37ebbb2d76fad2e8a90655e5f2bf2c3a $
 *
 * Description:
 *         This file implements get/set debug level APIs for media info.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__
#include "os/inc/x_os.h"
#include "inc/x_common.h"
#include "inc/x_mm_common.h"
#include "mutil/minfo/u_minfo.h"
#include "mutil/minfo/minfo.h"
#include "mutil/minfo/x_minfo.h"
#include "mutil/minfo/mkv_info/mkv_minfo.h"
#include "mutil/mm_util.h"
#include "mkv_parser.h"
#include "mutil/mm_util_input.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "cli/x_cli.h"
#include "dbg/x_dbg.h"
#include "dbg/u_dbg.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "x_mm_common.h"
#include "../u_minfo.h"
#include "../minfo.h"
#include "../../mm_util_input.h"

#include "../../mm_util.h"
#include "mkv_minfo.h"
#include "mkv_parser.h"

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
static INT32 minfo_cli_mkv_fopen(INT32 i4_argc, const CHAR** pps_argv);
static INT32 minfo_cli_mkv_mopen(INT32 i4_argc, const CHAR** pps_argv);
static INT32 minfo_cli_mkv_close(INT32 i4_argc, const CHAR** pps_argv);
static INT32 minfo_cli_mkv_getnum(INT32 i4_argc, const CHAR** pps_argv);
static INT32 minfo_cli_mkv_getinfo(INT32 i4_argc, const CHAR** pps_argv);
static INT32 minfo_cli_mkv_auto_test(INT32 i4_argc, const CHAR** pps_argv);
static INT32 minfo_cli_mkv_stress_test(INT32 i4_argc, const CHAR** pps_argv);

/* mfmtrecg command table */
static CLI_EXEC_T at_mkv_minfo_cmd_tbl[] =
{
    {"fopen",       NULL,  minfo_cli_mkv_fopen,        NULL,    "minfo_fopen",      CLI_SUPERVISOR},
    {"mopen",       NULL,  minfo_cli_mkv_mopen,        NULL,    "minfo_mopen",      CLI_SUPERVISOR},
    {"close",       NULL,  minfo_cli_mkv_close,        NULL,    "minfo_close",      CLI_SUPERVISOR},
    {"GetRecNum",   NULL,  minfo_cli_mkv_getnum,       NULL,    "minfo_getnum",     CLI_SUPERVISOR},
    {"GetRecInfo",  NULL,  minfo_cli_mkv_getinfo,      NULL,    "minfo_getinfo",    CLI_SUPERVISOR},
    {"AutoTest",    NULL,  minfo_cli_mkv_auto_test,    NULL,    "minfo_autotest",   CLI_SUPERVISOR},  
    {"StressTest",  NULL,  minfo_cli_mkv_stress_test,  NULL,    "minfo_stresstest", CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

CHAR* g_at_mkv_aud_encode_info[] =
{
    "MINFO_INFO_AUD_ENC_UNKNOWN",
    "MINFO_INFO_AUD_ENC_AC3",
    "MINFO_INFO_AUD_ENC_MPEG_1",         
    "MINFO_INFO_AUD_ENC_MPEG_2",         
    "MINFO_INFO_AUD_ENC_PCM",         
    "MINFO_INFO_AUD_ENC_TV_SYS",         
    "MINFO_INFO_AUD_ENC_DTS",         
    "MINFO_INFO_AUD_ENC_AAC",        
    "MINFO_INFO_AUD_ENC_EU_CANAL_PLUS",         
    "MINFO_INFO_AUD_ENC_WMA_V1",         
    "MINFO_INFO_AUD_ENC_WMA_V2",         
    "MINFO_INFO_AUD_ENC_WMA_V3",         
    "MINFO_INFO_AUD_ENC_LPCM"         
};

CHAR* g_at_mkv_vid_encode_info[] =
{
    "MINFO_INFO_VID_ENC_UNKNOWN",         
    "MINFO_INFO_VID_ENC_MPEG_1",         
    "MINFO_INFO_VID_ENC_MPEG_2",         
    "MINFO_INFO_VID_ENC_MPEG_4",         
    "MINFO_INFO_VID_ENC_DIVX_311",         
    "MINFO_INFO_VID_ENC_DIVX_4",         
    "MINFO_INFO_VID_ENC_DIVX_5",         
    "MINFO_INFO_VID_ENC_XVID",         
    "MINFO_INFO_VID_ENC_WMV1",         
    "MINFO_INFO_VID_ENC_WMV2",         
    "MINFO_INFO_VID_ENC_WMV3",         
    "MINFO_INFO_VID_ENC_WVC1",         
    "MINFO_INFO_VID_ENC_H264",         
    "MINFO_INFO_VID_ENC_H263",         
    "MINFO_INFO_VID_ENC_MJPG"         
};

CHAR* g_at_mkv_sp_type_info[] =
{
    "MINFO_INFO_SUBTITLE_TYPE_UNKOWN",
    "MINFO_INFO_MKV_SUBTITLE_TYPE_TEXT_UTF8",
    "MINFO_INFO_MKV_SUBTITLE_TYPE_TEXT_SSA",
    "MINFO_INFO_MKV_SUBTITLE_TYPE_TEXT_USF",
    "MINFO_INFO_MKV_SUBTITLE_TYPE_VOBSUB",
    "MINFO_INFO_MKV_SUBTITLE_TYPE_IMG_BMP",
    "MINFO_INFO_MKV_SUBTITLE_TYPE_KATE"
};

static HANDLE_T   gh_minfo_mkv;
static BOOL       b_parsing_done;
static BOOL       b_abort;

#endif
/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: x_mkv_minfo_nfy
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
static VOID x_mkv_minfo_nfy (
                HANDLE_T h_minfo, 
                MINFO_NFY_EVENT_TYPE_T     e_event,  
                const VOID*                pv_data1, 
                const VOID*                pv_data2, 
                const VOID*                pv_tag)
{    
    CHAR *apsz_parse_Info[] = { 
                                "PARSING DONE", 
                                "HANDLE CLOSED",   
                                "ERROR"
                              };
    
    if (e_event == MINFO_NFY_EVENT_PARSING_DONE)
    {
        b_parsing_done = TRUE;
    }
    else if (e_event == MINFO_NFY_EVENT_ERROR)
    {
        b_abort = TRUE;
    }
    x_dbg_stmt("minfo_cli_asf_nfy_fct:Event type=%s\r\n", 
              apsz_parse_Info[e_event]);
    return;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: minfo_cli_mkv_fopen
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
static INT32 minfo_cli_mkv_fopen(INT32        i4_argc,
                                 const CHAR** pps_argv)
{
    INT32           i4_ret;
    MEDIA_FORMAT_T  t_format;
    CHAR*           ps_file_path = (CHAR*)pps_argv[1];
    
    if(i4_argc >2 ||i4_argc <2 )
    {
        x_dbg_stmt("Invalid argument!\r\n");
        return CLIR_INV_ARG;
    }
    
    t_format.e_media_type = MEDIA_TYPE_UNKNOWN;
    t_format.t_media_subtype.u.e_aud_subtype  = MEDIA_AUD_SUBTYPE_UNKNOWN;
    b_parsing_done = FALSE;
    b_abort = FALSE;
    i4_ret = x_minfo_fopen(ps_file_path, x_mkv_minfo_nfy, (VOID*) -1, &t_format, &gh_minfo_mkv);
    if (MINFOR_OK != i4_ret )
    {
        DBG_ERROR(("Failed to open file(%s)!,i4_ret=%d\r\n", 
                   ps_file_path, 
                   i4_ret));
        return CLIR_CMD_EXEC_ERROR;
    }
    

    x_dbg_stmt("Open file(%s) successfully!\r\n", ps_file_path);
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: minfo_cli_mkv_mopen
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
static INT32 minfo_cli_mkv_mopen(INT32        i4_argc,
                                 const CHAR** pps_argv)
{
    HANDLE_T            h_file;
    INT32               i4_ret;
    CHAR*               ps_file_path;
    MEDIA_FORMAT_T      t_format;
    MEDIA_FEEDER_T      t_feeder;
    UINT8*              pui1_buf;
    UINT32              ui4_len;
    UINT64              ui8_len_seek;
    UINT32              ui4_read;
    
    x_memset(&t_format, 0, sizeof(MEDIA_FORMAT_T));
    /*parameters check, 2 parameters*/
    if ((3 != i4_argc) || (NULL == pps_argv))
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        x_dbg_stmt("usage:fopen filename zero_len(0/1)\r\n");
        return CLIR_INV_ARG;
    }

    /*get filename*/
    if (NULL == pps_argv[1])
    {
        DBG_ERROR(("usage:fopen filename zero_len(0/1)\r\n"));
        return CLIR_INV_ARG;
    }
    else
    {
        ps_file_path = (CHAR*)pps_argv[1];
    }
    i4_ret = x_fm_open(FM_ROOT_HANDLE, 
                       ps_file_path, 
                       FM_READ_ONLY, 
                       0777, 
                       FALSE, 
                       &h_file);
    if(i4_ret != FMR_OK)
    {
        x_dbg_stmt("File Open Failed! \r\n");
        return CLIR_INV_ARG;
    }
    if (0 == x_strncmp(pps_argv[2], "0", (SIZE_T)1))
    {
        t_feeder.h_feeder = h_file;
        t_feeder.t_feeder_le_fct_tbl.pf_copybytes = cli_feeder_copybytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_get_pos = cli_feeder_getpos_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_input1 = cli_feeder_input1bytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_input2 = cli_feeder_input2bytes_l_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_input3 = cli_feeder_input3bytes_l_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_input4 = cli_feeder_input4bytes_l_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_set_pos = cli_feeder_setpos_fct;
        i4_ret = x_minfo_mopen ((VOID* )&t_feeder,
                                0, 
                                x_mkv_minfo_nfy,        
                                NULL,        
                                &t_format,
                                &gh_minfo_mkv);
    }
    else
    {
        ui4_len = 2048;
        if (FMR_OK != i4_ret)
        {
            x_dbg_stmt("x_fm_lseek fail!\n");   
            dbg_abort(DBG_MOD_QA);
        }
        pui1_buf =(UINT8 *)x_mem_alloc(ui4_len);
        if (NULL == pui1_buf)
        {
            x_dbg_stmt("malloc memory fail!!\n");
            dbg_abort(DBG_MOD_QA);
        }
        i4_ret = x_fm_lseek(h_file,0,FM_SEEK_BGN,&ui8_len_seek);
        if (FMR_OK != i4_ret)
        {
            x_dbg_stmt("x_fm_lseek fail!\n");   
            dbg_abort(DBG_MOD_QA);
        }
        i4_ret = x_fm_read(h_file, (VOID *)pui1_buf, ui4_len, &ui4_read);
        if (FMR_OK != i4_ret)
        {
            x_dbg_stmt("x_fm_read fail!\n");   
            dbg_abort(DBG_MOD_QA);
        }
        x_dbg_stmt("ui4_read =%u\n",ui4_read);
        x_dbg_stmt("ui8_len_low =%u\n",ui4_len);
        i4_ret =  x_minfo_mopen ((VOID *) pui1_buf,
                                 ui4_len,
                                  x_mkv_minfo_nfy,        
                                  NULL,        
                                  &t_format,
                                  &gh_minfo_mkv);
        x_mem_free(pui1_buf);
    }
    if (MINFOR_OK != i4_ret )
    {
        DBG_ERROR(("Failed to open file(%s)!,i4_ret=%d\r\n", 
                   ps_file_path, i4_ret));
    }
    else
    {
        x_dbg_stmt("Open file(%s) successfully!\r\n", ps_file_path);
    }
    return i4_ret;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: minfo_cli_mkv_close
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
static INT32 minfo_cli_mkv_close(INT32        i4_argc,
                                 const CHAR** pps_argv)
{
    INT32 i4_ret;
    
    i4_ret = x_minfo_close(gh_minfo_mkv);
    if (MINFOR_OK != i4_ret)
    {
        DBG_ERROR(("minfo_cli_mkv_close error=%d\r\n", i4_ret));
        return CLIR_CMD_EXEC_ERROR;
    }
    x_dbg_stmt("minfo_cli_mkv_close successfully!\r\n");
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: minfo_cli_mkv_getnum
 *
 * Description: get minfo records number
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK           Routine successful.
 *          other non values  Routine unsuccessful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 minfo_cli_mkv_getnum(INT32        i4_argc,
                                  const CHAR** pps_argv)
{ 
    MINFO_INFO_TYPE_T  e_info_type;
    UINT16             ui2_num_recs;
    INT32              i4_ret;
    CHAR               c_type;

    /*parameters check*/
    if ((2 != i4_argc) || (NULL == pps_argv))
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        x_dbg_stmt("usage:getnum A/V/G\r\n");
        x_dbg_stmt("      A:Audio; V:Video; G:Generic; I:Index\r\n");
        return CLIR_INV_ARG;
    }

    if (NULL == pps_argv[1])
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        return CLIR_INV_ARG;
    }
    
    /*get minfo type*/
    c_type = pps_argv[1][0];
    switch (c_type)
    {
        case 'V':
            e_info_type = MINFO_INFO_TYPE_VIDEO_ATTR;
            break;
        case 'A':
            e_info_type = MINFO_INFO_TYPE_AUDIO_ATTR;
            break;
        case 'S':
            e_info_type = MINFO_INFO_TYPE_SP_ATTR;
            break;
        case 'G':
            e_info_type = MINFO_INFO_TYPE_GENERIC;
            break;
        case 'I':
            e_info_type =  MINFO_INFO_TYPE_IDX_INFO;
            break;
        case 'M':
            e_info_type = MINFO_INFO_TYPE_META_DATA;
            break;
        default:
            e_info_type = MINFO_INFO_TYPE_UNKNOWN;
            break;
    }

    /*get minfo number*/
    i4_ret = x_minfo_get_num_recs( gh_minfo_mkv,
                                   e_info_type,
                                   &ui2_num_recs);
    if (MINFOR_OK == i4_ret)
    {
        x_dbg_stmt("getnum steam_type=%d successfully num=%u\r\n", 
                  e_info_type,
                  ui2_num_recs);
    }
    else
    {
        x_dbg_stmt("getnum stream_type=%d unsuccessfully(%d)(A/V/G)\r\n", 
                  e_info_type,
                  i4_ret);
    }
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: _mkv_get_encode_str
 *
 * Description: get stream encode string
 *
 * Inputs:  ui1_encode_id speifies codec id
 *             ui1_strm_type specifies stream type
 *
 * Outputs: CHAR* encode string
 *
 * Returns: NULL
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
CHAR* _mkv_get_encode_str(UINT8 ui1_encode_id, UINT8 ui1_strm_type)
{
    CHAR* ps_info = NULL;
    
    switch (ui1_strm_type)
    {
    case 0:
        /*video encode*/
        if (ui1_encode_id <= 14)
        {
            ps_info = g_at_mkv_vid_encode_info[ui1_encode_id];
        }
        break;
    case 1:
        /*audio encode*/
        if (ui1_encode_id <= 12)
        {
            ps_info = g_at_mkv_aud_encode_info[ui1_encode_id];
        }
        break;
    case 2:
        /*subtitle type*/
        if (ui1_encode_id)
        {
            ui1_encode_id -= 3;
        }
        if (ui1_encode_id <= 6)
        {
            ps_info = g_at_mkv_sp_type_info[ui1_encode_id];
        }
        break;
    default:
        DBG_ERROR(("_mkv_get_encode_str wrong strm type!\r\n"));
        break;
    }
    return ps_info;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: _mkv_log_minfo_rec
 *
 * Description: log minfo rec info
 *
 * Inputs:  t_minfo_rec speifies minfo rec objet
 *
 * Outputs: NULL
 *
 * Returns: NULL
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
VOID  _mkv_log_minfo_rec(MINFO_INFO_T* pt_minfo_rec)
{
    MINFO_INFO_TYPE_T            e_info_type;
    MINFO_TYPE_GENERIC_T*        pt_general_info;  
    MINFO_MKV_AUDIO_STM_ATRBT_T* pt_mkv_audio_stm_attr;
    MINFO_MKV_VIDEO_STM_ATRBT_T* pt_mkv_video_stm_attr;
    MINFO_MKV_SUBTITLE_STM_ATRBT_T* pt_mkv_sp_stm_attr;
    MINFO_TYPE_IDX_INFO_T*       pt_idx_info; 
    MINFO_TYPE_META_DATA_T*      pt_meta_data;
    UINT32                       ui4_strm_id;
    UINT32                       ui4_i;
    CHAR                         ac_string[100];
    
    if (NULL == pt_minfo_rec)
    {
        x_dbg_stmt("_mkv_log_minfo_rec:NULL \r\n");
        return ;
    }
    
    e_info_type = pt_minfo_rec->e_info_type;
    switch (e_info_type)
    {
        case MINFO_INFO_TYPE_GENERIC:
            pt_general_info = &(pt_minfo_rec->u.t_general_info);
            x_dbg_stmt("Gneric:duration=%llu; "
                      "media_sz=%llu; strm_ns=%u; timecode_scale=%llu\r\n",
                      _mm_div64((pt_general_info->ui8_pb_duration * 1000000),
                                (pt_general_info->ui8_timecode_scale * 90),
                                NULL),
                      pt_general_info->ui8_media_sz,
                      pt_general_info->ui1_strm_ns,
                      pt_general_info->ui8_timecode_scale);
            x_dbg_stmt("\r\n");
            break;
        case MINFO_INFO_TYPE_VIDEO_ATTR:
            ui4_strm_id = pt_minfo_rec->u.t_stm_attribute.t_stm_id.u.t_mkv_stm_id;
            pt_mkv_video_stm_attr = &(pt_minfo_rec->u.t_stm_attribute.u.t_mkv_video_stm_attr);
            x_dbg_stmt("Video:stream_id=%u; enc=%s; codec_type=%u; height=%d; width=%d;\r\n"
                      "      default=%d; scale=%u, rate=%u, timecode_scale.n=%d; timecode_scale.d=%d;\r\n"
                      "      codec_info_size=%u:\r\n",
                      ui4_strm_id,
                      _mkv_get_encode_str((UINT8)pt_mkv_video_stm_attr->e_enc, 0),
                      pt_mkv_video_stm_attr->e_codec_type,
                      pt_mkv_video_stm_attr->i4_height,
                      pt_mkv_video_stm_attr->i4_width,
                      pt_mkv_video_stm_attr->b_default,
                      /*pt_mkv_video_stm_attr->ui8_duration,*/
                      pt_mkv_video_stm_attr->ui4_scale,
                      pt_mkv_video_stm_attr->ui4_rate,
                      pt_mkv_video_stm_attr->t_track_timecode_scale.ui4_float_numerator,
                      pt_mkv_video_stm_attr->t_track_timecode_scale.ui4_float_denominator,
                      /*pt_mkv_video_stm_attr->ui4_length,
                      pt_mkv_video_stm_attr->ui4_sample_size,*/
                      pt_mkv_video_stm_attr->ui4_codec_info_size);
            
            for (ui4_i = 0; 
                 ui4_i < pt_mkv_video_stm_attr->ui4_codec_info_size; 
                 ++ui4_i)
            {
                x_dbg_stmt("%d  ", pt_mkv_video_stm_attr->pui1_codec_info[ui4_i]);
            }
            if (pt_mkv_video_stm_attr->ui4_codec_info_size)
            {
                x_dbg_stmt("\r\n");
            }
            x_dbg_stmt("\r\n");
             
            break;
        case MINFO_INFO_TYPE_AUDIO_ATTR:
            ui4_strm_id = pt_minfo_rec->u.t_stm_attribute.t_stm_id.u.t_mkv_stm_id;
            pt_mkv_audio_stm_attr = &(pt_minfo_rec->u.t_stm_attribute.u.t_mkv_audio_stm_attr);
            x_dbg_stmt("Audio:stream_id=%u; enc=%s; layer=%d; pcm_align=%d; "
                      "default=%d; avg_bytes_sec=%u;\r\n"
                      "      bits_per_sample=%d; block_align=%d; "
                      "channels=%d; samples_per_sec=%d; lang=%s;\r\n"
                      "      scale=%u; rate=%u; timecode_scale.n=%d; timecode_scale.d=%d; encode_opts=%u;\r\n"
                      "      codec info size=%u:\r\n",
                      ui4_strm_id,
                      _mkv_get_encode_str((UINT8)pt_mkv_audio_stm_attr->e_enc, 1),
                      pt_mkv_audio_stm_attr->e_layer,
                      pt_mkv_audio_stm_attr->e_align,
                      pt_mkv_audio_stm_attr->b_default,
                      pt_mkv_audio_stm_attr->ui4_avg_bytes_sec,
                      pt_mkv_audio_stm_attr->i2_bits_per_sample,
                      pt_mkv_audio_stm_attr->i2_block_align,
                      pt_mkv_audio_stm_attr->i2_channels,
                      pt_mkv_audio_stm_attr->i4_samples_per_sec,
                      (CHAR*)&(pt_mkv_audio_stm_attr->s_lang[0]),
                      /*pt_mkv_audio_stm_attr->ui4_length,
                      pt_mkv_audio_stm_attr->ui4_sample_size,*/
                      pt_mkv_audio_stm_attr->ui4_scale,
                      pt_mkv_audio_stm_attr->ui4_rate,
                      pt_mkv_audio_stm_attr->t_track_timecode_scale.ui4_float_numerator,
                      pt_mkv_audio_stm_attr->t_track_timecode_scale.ui4_float_denominator,
                      pt_mkv_audio_stm_attr->ui2_encode_opts,
                      /*pt_mkv_audio_stm_attr->ui8_duration,*/
                      pt_mkv_audio_stm_attr->ui4_codec_info_size);
            
            for (ui4_i = 0; 
                 ui4_i < pt_mkv_audio_stm_attr->ui4_codec_info_size; 
                 ++ui4_i)
            {
                x_dbg_stmt("%d  ", pt_mkv_audio_stm_attr->pui1_codec_info[ui4_i]);
            }
            if (pt_mkv_audio_stm_attr->ui4_codec_info_size)
            {
                x_dbg_stmt("\r\n");
            }
            x_dbg_stmt("\r\n");
            
            break;
        case MINFO_INFO_TYPE_SP_ATTR:
            ui4_strm_id = pt_minfo_rec->u.t_stm_attribute.t_stm_id.u.t_mkv_stm_id;
            pt_mkv_sp_stm_attr = &(pt_minfo_rec->u.t_stm_attribute.u.t_mkv_sp_stm_attr);
            x_dbg_stmt("Subtitle:stream_id=%u; subtitle_type=%s; default=%d;\r\n"
                      "         scale=%u; rate=%u; timecode_scale.n=%d; timecode_scale.d=%d; lang=%s;\r\n"
                      "         content_encodings_size=%d; codec info size=%u:\r\n",
                      ui4_strm_id,
                      _mkv_get_encode_str((UINT8)pt_mkv_sp_stm_attr->e_subtitle_type, 2),
                      pt_mkv_sp_stm_attr->b_default,
                      pt_mkv_sp_stm_attr->ui4_scale,
                      pt_mkv_sp_stm_attr->ui4_rate,
                      pt_mkv_sp_stm_attr->t_track_timecode_scale.ui4_float_numerator,
                      pt_mkv_sp_stm_attr->t_track_timecode_scale.ui4_float_denominator,
                      (CHAR*)&(pt_mkv_sp_stm_attr->s_lang[0]),
                      pt_mkv_sp_stm_attr->ui4_content_encodings_size,
                      pt_mkv_sp_stm_attr->ui4_codec_info_size);
            
            /*for (ui4_i = 0; 
                 ui4_i < pt_mkv_sp_stm_attr->ui4_codec_info_size; 
                 ++ui4_i)
            {
                x_dbg_stmt(("%d  ", pt_mkv_sp_stm_attr->pui1_codec_info[ui4_i]));
            }
            if (pt_mkv_sp_stm_attr->ui4_codec_info_size)
            {
                x_dbg_stmt(("\r\n"));
            }*/
            x_dbg_stmt("\r\n");
            
            break;
        case MINFO_INFO_TYPE_IDX_INFO:
            pt_idx_info = &(pt_minfo_rec->u.t_idx_info);
            x_dbg_stmt("Index Info:ui4_idx_type=%d; "
                      "data_offset=%llu; data_sz=%llu; "
                      "idx_offset=%llu; idx_sz=%llu\r\n"
                      "           seg_data_offset=%llu, seg_timecode_scale=%llu\r\n",
                      pt_idx_info->ui4_idx_type,
                      pt_idx_info->ui8_data_offset,
                      pt_idx_info->ui8_data_sz,
                      pt_idx_info->ui8_idx_offset,
                      pt_idx_info->ui8_idx_sz,
                      pt_idx_info->ui8_segment_data_offset,
                      pt_idx_info->ui8_seg_timecode_scale);
            x_dbg_stmt("\r\n");
            break;
        case MINFO_INFO_TYPE_META_DATA:
            pt_meta_data = &(pt_minfo_rec->u.t_meta_data);
            x_uc_w2s_to_ps((const UTF16_T*)pt_meta_data->pwc_str_title,
                            &ac_string[0],
                            x_uc_w2s_strlen((const UTF16_T*)pt_meta_data->pwc_str_title));
            x_dbg_stmt("Meta Data Info:pwc_str_title=%s;\r\n", ac_string);
            x_memset(ac_string, 0, 100);
            x_uc_w2s_to_ps((const UTF16_T*)pt_meta_data->pwc_str_director,
                            &ac_string[0],
                            x_uc_w2s_strlen((const UTF16_T*)pt_meta_data->pwc_str_director));
            x_dbg_stmt("               pwc_str_director=%s;\r\n", ac_string); 
            x_memset(ac_string, 0, 100);
            x_uc_w2s_to_ps((const UTF16_T*)pt_meta_data->pwc_str_copyright,
                            &ac_string[0],
                            x_uc_w2s_strlen((const UTF16_T*)pt_meta_data->pwc_str_copyright));
            x_dbg_stmt("               pwc_str_copyright=%s;\r\n", ac_string);
            x_memset(ac_string, 0, 100);
            x_uc_w2s_to_ps((const UTF16_T*)pt_meta_data->pwc_str_genre,
                            &ac_string[0],
                            x_uc_w2s_strlen((const UTF16_T*)pt_meta_data->pwc_str_genre));
            x_dbg_stmt("               pwc_str_genre=%s;\r\n", ac_string);
            x_dbg_stmt("               b_cr_date_exist=%d; "
                      "t_date:year=%d, month=%d, day=%d, %d:%d:%d, week=%d;\r\n"
                      "               t_frame_rate:rate=%u, scale=%u; "
                      "duration=%llu, bitrate=%u\r\n",
                      pt_meta_data->b_cr_date_exist,
                      pt_meta_data->t_date.ui2_year,
                      pt_meta_data->t_date.ui1_month,
                      pt_meta_data->t_date.ui1_day,
                      pt_meta_data->t_date.ui1_hour,
                      pt_meta_data->t_date.ui1_min,
                      pt_meta_data->t_date.ui1_sec,
                      pt_meta_data->t_date.ui1_week,
                      pt_meta_data->t_frame_rate.ui4_rate,
                      pt_meta_data->t_frame_rate.ui4_scale,
                      pt_meta_data->ui8_pb_duration,
                      pt_meta_data->ui4_bitrate);
            x_dbg_stmt("\r\n");
            break;
        default:
            x_dbg_stmt("_mkv_log_minfo_rec:unknown\r\n");
            x_dbg_stmt("\r\n");
            break;
            
    }
    
    return ;
}
#endif   /*if define CLI_LVL_ALL*/

/*-----------------------------------------------------------------------------
 * Name: minfo_cli_mkv_getinfo
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
static INT32 minfo_cli_mkv_getinfo(INT32        i4_argc,
                                   const CHAR** pps_argv)
{
    MINFO_INFO_TYPE_T  e_info_type;
    INT32              i4_ret;
    CHAR               c_type;
    UINT16             ui2_index;
    MINFO_INFO_T       t_minfo_rec;
    SIZE_T             t_size;

    /*parameters check*/
    if ((3 != i4_argc) || (NULL == pps_argv))
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        x_dbg_stmt("usage:getinfo A/V/G count\r\n");
        x_dbg_stmt("      A:Audio; V:Video; G:Generic; I:Index\r\n");
        return CLIR_INV_ARG;
    }
    if (NULL == pps_argv[1])
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        return CLIR_INV_ARG;
    }
    /*get minfo type*/
    c_type = pps_argv[1][0];
    switch (c_type)
    {
        case 'V':
            e_info_type = MINFO_INFO_TYPE_VIDEO_ATTR;
            break;
        case 'A':
            e_info_type = MINFO_INFO_TYPE_AUDIO_ATTR;
            break;
        case 'S':
            e_info_type = MINFO_INFO_TYPE_SP_ATTR;
            break;
        case 'G':
            e_info_type = MINFO_INFO_TYPE_GENERIC;
            break;
        case 'I':
            e_info_type =  MINFO_INFO_TYPE_IDX_INFO;
            break;
        case 'M':
            e_info_type = MINFO_INFO_TYPE_META_DATA;
            break;
        default:
            e_info_type = MINFO_INFO_TYPE_UNKNOWN;
            break;
    }
    /*init 0*/
    t_size = sizeof(MINFO_INFO_T);
    x_memset(&t_minfo_rec, 0, t_size);
    /*get count */
    ui2_index = (UINT16)x_strtoull(pps_argv[2], NULL, 10);
    i4_ret = x_minfo_get_info ( gh_minfo_mkv,    
                                e_info_type,
                                ui2_index,          
                                (VOID*)&t_minfo_rec,       
                                &t_size);
    if (MINFOR_OK != i4_ret)
    {
        DBG_ERROR(("mkv_minfo_get_info return %d\r\n", i4_ret));
        return MINFOR_INTERNAL_ERR;
    }
    _mkv_log_minfo_rec(&t_minfo_rec);
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: minfo_cli_mkv_auto_test
 *
 * Description:  mkv minfo module auto test command. This function will loop
 *               vocating minfo fopen/getnum/getinfo/close for different
 *               type file. 
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK           Routine successful.
 *          other non values  Routine unsuccessful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 minfo_cli_mkv_auto_test(INT32        i4_argc,
                                     const CHAR** pps_argv)
{  
    INT32           i4_ret;
    CHAR*           ppc_argv[4];
    UINT16          ui2_number;
    MINFO_INFO_T    t_minfo_rec;
    SIZE_T          t_size;
    UINT32          ui4_idx;
    CHAR*           ps_file_path;
    
    /*parameters check*/
    if ((2 != i4_argc) || (NULL == pps_argv))
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        x_dbg_stmt("usage:mstress count\r\n");
        return CLIR_INV_ARG;
    }
    ps_file_path = (CHAR*)pps_argv[1];
    /*fopen*/
    if (NULL == ps_file_path)
    {
        DBG_ERROR(("file name null error!\r\n"));
        return CLIR_INV_ARG;
    }
    
    ppc_argv[0] = "fopen";
    ppc_argv[1] = ps_file_path;
    i4_ret = minfo_cli_mkv_fopen(2, (const CHAR**)ppc_argv);
    if (MINFOR_OK != i4_ret)
    {
        DBG_ERROR(("mkv open error=%d\r\n", i4_ret));
        return CLIR_INV_ARG;
    }

    while ((!b_parsing_done) && (!b_abort))
    {
        x_dbg_stmt("now mkv parsing isn't complete, so wait!\r\n");
        x_thread_delay(10);
    }
    
    x_dbg_stmt("Begin File: %s\r\n", ps_file_path);
    
    /*get num and getinfo*/
    /*1) get generic num should be 1*/
    i4_ret = x_minfo_get_num_recs( gh_minfo_mkv,
                                   MINFO_INFO_TYPE_GENERIC,
                                   &ui2_number);
    if (MINFOR_OK == i4_ret)
    {
        if (1 != ui2_number)
        {
            x_dbg_stmt("getnum Generic unsuccessfully num=%u\r\n", 
                      ui2_number);
        }
        else
        {
            x_dbg_stmt("getnum Generic successfully\r\n");
        }
    }
    else
    {
        x_dbg_stmt("getnum Generic unsuccessfully(%d)\r\n", 
                  i4_ret);
    }
    t_size = sizeof(MINFO_INFO_T);
    x_memset(&t_minfo_rec, 0, t_size);
    i4_ret = x_minfo_get_info( gh_minfo_mkv,    
                               MINFO_INFO_TYPE_GENERIC,
                               1,          
                               (VOID*)&t_minfo_rec,       
                               &t_size);
    if (MINFOR_OK != i4_ret)
    {
        DBG_ERROR(("minfo_cli_mkv_auto_test return %d "
                   "for generic \r\n", 
                   i4_ret));
        return MINFOR_INTERNAL_ERR;
    }
    /*show obtainted info*/
    x_dbg_stmt("Genric Info:\r\n");
    _mkv_log_minfo_rec(&t_minfo_rec);
    

    /*2) get Video num and info*/
    i4_ret = x_minfo_get_num_recs( gh_minfo_mkv,
                                   MINFO_INFO_TYPE_VIDEO_ATTR,
                                   &ui2_number);
    if (MINFOR_OK == i4_ret)
    {
        x_dbg_stmt("getnum Video successfully num=%u\r\n", 
                  ui2_number);
    }
    else
    {
        x_dbg_stmt("getnum Video unsuccessfully(%d)\r\n", 
                  i4_ret);
    }
    if (!ui2_number)
    {
        x_dbg_stmt("The file not contain video stream!\r\n");
        x_dbg_stmt("\r\n");
    }
    /*get info*/
    for (ui4_idx = 1 ; ui4_idx <= ui2_number; ++ui4_idx)
    {
        t_size = sizeof(MINFO_INFO_T);
        x_memset(&t_minfo_rec, 0, t_size);
        i4_ret = x_minfo_get_info( gh_minfo_mkv,    
                                   MINFO_INFO_TYPE_VIDEO_ATTR,
                                   (UINT16)ui4_idx,          
                                   (VOID*)&t_minfo_rec,       
                                   &t_size);
        if (MINFOR_OK != i4_ret)
        {
            DBG_ERROR(("minfo_cli_mkv_auto_test return %d "
                       "for video\r\n", 
                       i4_ret));
            return MINFOR_INTERNAL_ERR;
        }
        /*show obtainted info*/
        x_dbg_stmt("Video Count[%d] Info:\r\n", ui4_idx);
        _mkv_log_minfo_rec(&t_minfo_rec);
    }
    /*3)get audio num and info*/
    i4_ret = x_minfo_get_num_recs( gh_minfo_mkv,
                                   MINFO_INFO_TYPE_AUDIO_ATTR,
                                   &ui2_number);
    if (MINFOR_OK == i4_ret)
    {
        x_dbg_stmt("getnum Audio successfully num=%u\r\n", 
                  ui2_number);
    }
    else
    {
        x_dbg_stmt("getnum Audio unsuccessfully(%d)\r\n", 
                  i4_ret);
    }
    if (!ui2_number)
    {
        x_dbg_stmt("The file not contain audio stream!\r\n");
        x_dbg_stmt("\r\n");
    }
    /*get info*/
    for (ui4_idx = 1 ; ui4_idx <= ui2_number; ++ui4_idx)
    {
        t_size = sizeof(MINFO_INFO_T);
        x_memset(&t_minfo_rec, 0, t_size);
        i4_ret = x_minfo_get_info( gh_minfo_mkv,    
                                   MINFO_INFO_TYPE_AUDIO_ATTR,
                                   (UINT16)ui4_idx,          
                                   (VOID*)&t_minfo_rec,       
                                   &t_size);
        if (MINFOR_OK != i4_ret)
        {
            DBG_ERROR(("minfo_cli_mkv_auto_test return %d "
                       "for audio\r\n", 
                       i4_ret));
            return MINFOR_INTERNAL_ERR;
        }
        /*show obtainted info*/
        x_dbg_stmt("Audio Count[%d] Info:\r\n", ui4_idx);
        _mkv_log_minfo_rec(&t_minfo_rec);
    }
    
    /*4)get subtitle num and info*/
    i4_ret = x_minfo_get_num_recs( gh_minfo_mkv,
                                   MINFO_INFO_TYPE_SP_ATTR,
                                   &ui2_number);
    if (MINFOR_OK == i4_ret)
    {
        x_dbg_stmt("getnum Subtitle successfully num=%u\r\n", 
                  ui2_number);
    }
    else
    {
        x_dbg_stmt("getnum Subtitle unsuccessfully(%d)\r\n", 
                  i4_ret);
    }
    if (!ui2_number)
    {
        x_dbg_stmt("The file not contain subtitle stream!\r\n");
        x_dbg_stmt("\r\n");
    }
    /*get info*/
    for (ui4_idx = 1 ; ui4_idx <= ui2_number; ++ui4_idx)
    {
        t_size = sizeof(MINFO_INFO_T);
        x_memset(&t_minfo_rec, 0, t_size);
        i4_ret = x_minfo_get_info( gh_minfo_mkv,    
                                   MINFO_INFO_TYPE_SP_ATTR,
                                   (UINT16)ui4_idx,          
                                   (VOID*)&t_minfo_rec,       
                                   &t_size);
        if (MINFOR_OK != i4_ret)
        {
            DBG_ERROR(("minfo_cli_mkv_auto_test return %d "
                       "for subtitle\r\n", 
                       i4_ret));
            return MINFOR_INTERNAL_ERR;
        }
        /*show obtainted info*/
        x_dbg_stmt("Subtitle Count[%d] Info:\r\n", ui4_idx);
        _mkv_log_minfo_rec(&t_minfo_rec);
    }

    /*5)get index info*/
    i4_ret = x_minfo_get_num_recs( gh_minfo_mkv,
                                   MINFO_INFO_TYPE_IDX_INFO,
                                   &ui2_number);
    if (MINFOR_OK == i4_ret)
    {
        if (1 != ui2_number)
        {
            x_dbg_stmt("getnum Index Info unsuccessfully num=%u\r\n", 
                      ui2_number);
        }
        else
        {
            x_dbg_stmt("getnum Index Info successfully\r\n");
        }
    }
    else
    {
        x_dbg_stmt("getnum Index Info unsuccessfully(%d)\r\n", 
                  i4_ret);
    }
    t_size = sizeof(MINFO_INFO_T);
    x_memset(&t_minfo_rec, 0, t_size);
    i4_ret = x_minfo_get_info( gh_minfo_mkv,    
                               MINFO_INFO_TYPE_IDX_INFO,
                               1,          
                               (VOID*)&t_minfo_rec,       
                               &t_size);
    if (MINFOR_OK != i4_ret)
    {
        DBG_ERROR(("minfo_cli_mkv_auto_test return %d "
                   "for Index Info\r\n", 
                   i4_ret));
        return MINFOR_INTERNAL_ERR;
    }
    /*show obtainted info*/
    x_dbg_stmt("Index Info:\r\n");
    _mkv_log_minfo_rec(&t_minfo_rec);
    
    /*6)get meta data info*/
    i4_ret = x_minfo_get_num_recs( gh_minfo_mkv,
                                   MINFO_INFO_TYPE_META_DATA,
                                   &ui2_number);
    if (MINFOR_OK == i4_ret)
    {
        if (1 != ui2_number)
        {
            x_dbg_stmt("getnum Meta Data unsuccessfully num=%u\r\n", 
                      ui2_number);
        }
        else
        {
            x_dbg_stmt("getnum Meta Data successfully\r\n");
        }
    }
    else
    {
        x_dbg_stmt("getnum Meta Data unsuccessfully(%d)\r\n", 
                  i4_ret);
    }
    t_size = sizeof(MINFO_INFO_T);
    x_memset(&t_minfo_rec, 0, t_size);
    i4_ret = x_minfo_get_info( gh_minfo_mkv,    
                               MINFO_INFO_TYPE_META_DATA,
                               1,          
                               (VOID*)&t_minfo_rec,       
                               &t_size);
    if (MINFOR_OK != i4_ret)
    {
        DBG_ERROR(("minfo_cli_mkv_auto_test return %d "
                   "for Meta data Info\r\n", 
                   i4_ret));
        return MINFOR_INTERNAL_ERR;
    }
    /*show MetaData info*/
    x_dbg_stmt("Meta Data Info:\r\n");
    _mkv_log_minfo_rec(&t_minfo_rec);
    /*close*/
    i4_ret = x_minfo_close(gh_minfo_mkv);
    if (MINFOR_OK != i4_ret)
    {
        DBG_ERROR(("mkv closed error=%d\r\n", i4_ret));
        return CLIR_CMD_EXEC_ERROR;
    }
    x_dbg_stmt("End File: %s\r\n", ps_file_path);
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: minfo_cli_mkv_stress_test
 *
 * Description:  mkv minfo module stress test command.  
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK           Routine successful.
 *          other non values  Routine unsuccessful.
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 minfo_cli_mkv_stress_test(INT32        i4_argc,
                                       const CHAR** pps_argv)
{  
    UINT32     ui4_loop_count;
    BOOL       b_dead_loop;
    CHAR       ac_file_path[50];
    CHAR*      ppc_argv[2];
    INT32      i4_ret;
    
    /*parameters check*/
    if ((2 != i4_argc) || (NULL == pps_argv))
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        x_dbg_stmt("usage:mstress count\r\n");
        return CLIR_INV_ARG;
    }

    /*************************Do***********************************/
   
    /*get loop count*/
    ui4_loop_count = (UINT32)x_strtoull(pps_argv[1], NULL, 10);
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
        x_sprintf(ac_file_path, "/mnt/usb/Mass-000/mkv/0034_DX50_MP3_001000.mkv");
        ppc_argv[0] = "AutoTest";
        ppc_argv[1] = ac_file_path;
        i4_ret = minfo_cli_mkv_auto_test(2, (const CHAR**)ppc_argv);
        if (i4_ret != CLIR_OK)
        {
            DBG_ERROR(("stress test: minfo_cli_mkv_auto_test error!\r\n"));
            return CLIR_CMD_EXEC_ERROR;
        }
        /*loop control*/
        --ui4_loop_count;
        if (TRUE != b_dead_loop)
        {
            if (0 == ui4_loop_count)
            {
                break;
            }
        }
    }
     
    return CLIR_OK;   
}
#endif

/*-----------------------------------------------------------------------------
 * Name: mkv_minfo_cli_init
 *
 * Description: This API initializes midxbuld CLI component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 mkv_minfo_cli_init(CLI_EXEC_T **ppt_command_array)
{
#ifdef CLI_LVL_ALL

    *ppt_command_array = at_mkv_minfo_cmd_tbl;

#endif
    return 0;
}



