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
 * $RCSfile: ogg_minfo_cli.c,v $
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
#include "mutil/minfo/ogg_info/ogg_minfo.h"
#include "mutil/minfo/ogg_info/x_minfo_ogg.h"
#include "mutil/mm_util_input.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif

#include "cli/x_cli.h"
#include "dbg/x_dbg.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "x_mm_common.h"
#include "../u_minfo.h"
#include "../minfo.h"
#include "../x_minfo.h"
#include "ogg_minfo.h"
#include "x_minfo_ogg.h"
#include "../../mm_util_input.h"

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
static INT32 minfo_cli_ogg_fopen(INT32 i4_argc, const CHAR** pps_argv);
static INT32 minfo_cli_ogg_mopen(INT32 i4_argc, const CHAR** pps_argv);
static INT32 minfo_cli_ogg_close(INT32 i4_argc, const CHAR** pps_argv);
static INT32 minfo_cli_ogg_getnum(INT32 i4_argc, const CHAR** pps_argv);
static INT32 minfo_cli_ogg_getinfo(INT32 i4_argc, const CHAR** pps_argv);
static INT32 minfo_cli_ogg_fstress(INT32 i4_argc, const CHAR** pps_argv);
static INT32 minfo_cli_ogg_mstress(INT32 i4_argc, const CHAR** pps_argv);

/* mfmtrecg command table */
static CLI_EXEC_T at_ogg_minfo_cmd_tbl[] =
{
    {"fopen",   NULL,  minfo_cli_ogg_fopen,     NULL,    "minfo_fopen",     CLI_SUPERVISOR},
    {"mopen",   NULL,  minfo_cli_ogg_mopen,     NULL,    "minfo_mopen",     CLI_SUPERVISOR},
    {"close",   NULL,  minfo_cli_ogg_close,     NULL,    "minfo_close",     CLI_SUPERVISOR},
    {"getnum",  NULL,  minfo_cli_ogg_getnum,    NULL,    "minfo_getnum",    CLI_SUPERVISOR},
    {"getinfo", NULL,  minfo_cli_ogg_getinfo,   NULL,    "minfo_getinfo",   CLI_SUPERVISOR},
    {"fstress", NULL,  minfo_cli_ogg_fstress,   NULL,    "minfo_fstress",   CLI_SUPERVISOR},
    {"mstress", NULL,  minfo_cli_ogg_mstress,   NULL,    "minfo_mstress",   CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

static HANDLE_T       gh_minfo;

/* for test mopen */
static HANDLE_T       gh_file = NULL_HANDLE;
static UINT8*         gpui1_buf = NULL;

#endif
/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: x_minfo_nfy
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
static VOID x_minfo_nfy (
                HANDLE_T                    h_minfo, 
                MINFO_NFY_EVENT_TYPE_T      e_event,  
                const VOID*                 pv_data1, 
                const VOID*                 pv_data2, 
                const VOID*                 pv_tag)
{    
    x_dbg_stmt("minfo nfy\r\n");
}
#endif
/*-----------------------------------------------------------------------------
 * Name: minfo_cli_ogg_fopen
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
static INT32 minfo_cli_ogg_fopen(INT32        i4_argc,
                                 const CHAR** pps_argv)
{

#ifndef __NO_FM__

    INT32           i4_ret;
    MEDIA_FORMAT_T  t_format;
    CHAR*           ps_file_path;
    
    if ((i4_argc != 3) || (pps_argv == NULL))
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        x_dbg_stmt("usage:fopen filename formatflag(0/1)\r\n");
        return CLIR_INV_ARG;
    }
    /* check file path */
    ps_file_path = (CHAR*)pps_argv[1];
    if(ps_file_path == NULL)
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        x_dbg_stmt("usage:fopen filename formatflag(0/1)\r\n");
        return CLIR_INV_ARG;
    }
    /*get format info*/
    if (0 == x_strncmp(pps_argv[2], "1", (SIZE_T)1))
    {
        t_format.e_media_type = MEDIA_TYPE_CONTAINER;
        t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_OGG;
    }
    else
    {
        t_format.e_media_type = MEDIA_TYPE_UNKNOWN;
        t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_UNKNOWN;
    }
    
    i4_ret = x_minfo_fopen(ps_file_path, x_minfo_nfy, NULL, &t_format, &gh_minfo);
    if (MINFOR_OK != i4_ret )
    {
        DBG_ERROR(("Failed to open file(%s)!,i4_ret = %d\r\n", 
                   ps_file_path, i4_ret));
        return CLIR_CMD_EXEC_ERROR;
    }


    x_dbg_stmt("minfo open file(%s) successfully!\r\n", ps_file_path);
    return CLIR_OK;
    
#else

    return CLIR_OK;

#endif
}
#endif

/*-----------------------------------------------------------------------------
 * Name: minfo_cli_ogg_mopen
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
static INT32 minfo_cli_ogg_mopen(INT32        i4_argc,
                                 const CHAR** pps_argv)
{
#ifndef __NO_FM__
    INT32           i4_ret;
    MEDIA_FORMAT_T  t_format;
    CHAR*           ps_file_path;
    UINT64          ui8_size;
    UINT64          ui8_cur_pos;
    UINT32          ui4_read;
    MEDIA_FEEDER_T  t_feeder;
    FM_FILE_INFO_T  t_fm_info;
    
    if ((i4_argc != 4) || (pps_argv == NULL))
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        x_dbg_stmt("usage:mopen filename formatflag(0/1) mem_length \r\n");
        return CLIR_INV_ARG;
    }
    /* check file path */
    ps_file_path = (CHAR*)pps_argv[1];
    if(ps_file_path == NULL)
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        x_dbg_stmt("usage:fopen filename formatflag(0/1)\r\n");
        return CLIR_INV_ARG;
    }
    /*get format info*/
    if (0 == x_strncmp(pps_argv[2], "1", (SIZE_T)1))
    {
        t_format.e_media_type = MEDIA_TYPE_CONTAINER;
        t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_OGG;
    }
    else
    {
        t_format.e_media_type = MEDIA_TYPE_UNKNOWN;
        t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_UNKNOWN;
    }
    
    /* open file and fill data */
    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_file_path, FM_READ_ONLY, 0777, FALSE, &gh_file);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("fail to open file \r\n");
        return CLIR_CMD_EXEC_ERROR;
    }

    if (0 == x_strncmp(pps_argv[3], "0", (SIZE_T)1))
    {
        t_feeder.h_feeder = gh_file;
        t_feeder.t_feeder_be_fct_tbl.pf_copybytes = cli_feeder_copybytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_get_pos = cli_feeder_getpos_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input1 = cli_feeder_input1bytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input2 = cli_feeder_input2bytes_b_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input3 = cli_feeder_input3bytes_b_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input4 = cli_feeder_input4bytes_b_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_set_pos = cli_feeder_setpos_fct;
        
        i4_ret = x_minfo_mopen((VOID*)&t_feeder, 
                               (SIZE_T)0, 
                               x_minfo_nfy, 
                               NULL, 
                               &t_format, 
                               &gh_minfo);
    }
    else
    {
        /* set memory length */
        x_fm_get_info_by_handle(gh_file, &t_fm_info);
        if (t_fm_info.ui8_size > 100000)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
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
        i4_ret = x_minfo_mopen((VOID*)gpui1_buf, 
                               (SIZE_T)ui8_size, 
                               x_minfo_nfy, 
                               NULL, 
                               &t_format, 
                               &gh_minfo);
    }
    if (MINFOR_OK != i4_ret )
    {
        _mm_util_mem_free(gpui1_buf);
        DBG_ERROR(("Failed to mopen ,i4_ret = %d\r\n", i4_ret));
        return CLIR_CMD_EXEC_ERROR;
    }
    
    x_dbg_stmt("minfo mopen successfully!\r\n");
    return CLIR_OK;
#else
    return CLIR_OK;
#endif
}
#endif

/*-----------------------------------------------------------------------------
 * Name: minfo_cli_ogg_close
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
static INT32 minfo_cli_ogg_close(INT32        i4_argc,
                                 const CHAR** pps_argv)
{
    INT32 i4_ret;

    /* check and free mopen resource */
    x_dbg_stmt("minfo_cli_ogg_close \r\n");
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

    i4_ret = x_minfo_close(gh_minfo);
    if (MINFOR_OK != i4_ret)
    {
        DBG_ERROR(("ogg closed error=%d\r\n", i4_ret));
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("ogg closed successfully!\r\n");
    return CLIR_OK;
}
#endif
/*-----------------------------------------------------------------------------
 * Name: minfo_cli_ogg_getnum
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
static INT32 minfo_cli_ogg_getnum(INT32        i4_argc,
                                  const CHAR** pps_argv)
{
    MINFO_INFO_TYPE_T  e_info_type;
    UINT16             ui2_num_recs;
    INT32              i4_ret;
    CHAR               c_type;

    /*parameters check*/
    if ((i4_argc != 2) || (pps_argv == NULL))
    {
        DBG_ERROR(("Invalid argument! \r\n"));
        x_dbg_stmt("usage:getnum A/V/G/I \r\n");
        x_dbg_stmt("     A:Audio; V:Video; G:Generic; I:Idx builder \r\n");
        return CLIR_INV_ARG;
    }


    if (pps_argv[1] == NULL)
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
        case 'G':
            e_info_type = MINFO_INFO_TYPE_GENERIC;
            break;
        case 'I':
            e_info_type =  MINFO_INFO_TYPE_IDX_INFO;
            break;
        default:
            e_info_type = MINFO_INFO_TYPE_UNKNOWN;
            break;
    }

    /*get minfo number*/
    i4_ret = x_minfo_get_num_recs( gh_minfo,
                                   e_info_type,
                                   &ui2_num_recs);
    
    if (i4_ret != MINFOR_OK)
    {
        x_dbg_stmt("ogg minfo getnum FAIL!! \r\n");
    }
    else
    {
        switch (e_info_type)
        {
        case MINFO_INFO_TYPE_VIDEO_ATTR:
            x_dbg_stmt("get video num = %u \r\n", ui2_num_recs);
            break;
        case MINFO_INFO_TYPE_AUDIO_ATTR:
            x_dbg_stmt("get audio num = %u \r\n", ui2_num_recs);
            break;
        case MINFO_INFO_TYPE_GENERIC:
            x_dbg_stmt("get generic num = %u \r\n", ui2_num_recs);
            break;
        case MINFO_INFO_TYPE_IDX_INFO:
            x_dbg_stmt("get idx_builder num = %u \r\n", ui2_num_recs);
            break;
        default:
            x_dbg_stmt("get num unknown ");
            break;
        }
        x_dbg_stmt("ogg minfo getnum OK \r\n");
    }
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: minfo_cli_ogg_getinfo
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
static INT32 minfo_cli_ogg_getinfo(INT32        i4_argc,
                                   const CHAR** pps_argv)
{
    MINFO_INFO_TYPE_T  e_info_type;
    INT32              i4_ret;
    CHAR               c_type;
    UINT16             ui2_index;
    MINFO_INFO_T       t_minfo_rec;
    SIZE_T             t_size;
    
    /*parameters check*/
    if ((i4_argc != 3) || (pps_argv == NULL))
    {
        DBG_ERROR(("Invalid argument! \r\n"));
        x_dbg_stmt("usage:getinfo A/V/G/I index \r\n");
        x_dbg_stmt("      A:Audio; V:Video; G:Generic; I:Idx builder \r\n");
        return CLIR_INV_ARG;
    }


    if (pps_argv[1] == NULL)
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
        case 'G':
            e_info_type = MINFO_INFO_TYPE_GENERIC;
            break;
        case 'I':
            e_info_type =  MINFO_INFO_TYPE_IDX_INFO;
            break;
        case 'R':
            e_info_type =  MINFO_INFO_TYPE_DRM_INFO;
            break;
        case 'M':
            e_info_type =  MINFO_INFO_TYPE_META_DATA;
            break;
        case 'S':
            e_info_type =  MINFO_INFO_TYPE_SP_ATTR;
            break;
        default:
            e_info_type = MINFO_INFO_TYPE_UNKNOWN;
            break;
    }

    /*get index */
    ui2_index = x_strtoll(pps_argv[2], NULL, 10);
    
    t_size = sizeof(MINFO_INFO_T);
    x_memset(&t_minfo_rec, 0, t_size);

    /* get minfo info */
    i4_ret = x_minfo_get_info(gh_minfo,
                              e_info_type,
                              ui2_index,
                              (VOID*)&t_minfo_rec,
                              &t_size);
    
    if (i4_ret != MINFOR_OK)
    {
        x_dbg_stmt("ogg minfo getinfo FAIL!! \r\n");
    }
    else
    {
        switch (e_info_type)
        {
        case MINFO_INFO_TYPE_VIDEO_ATTR:
            x_dbg_stmt("info type =         0x%.16x\r\n", 
                      t_minfo_rec.e_info_type);
            x_dbg_stmt("stream index =      0x%.16x\r\n", 
                      t_minfo_rec.ui1_rec_idx);
            x_dbg_stmt("is codec support =  0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.b_is_codec_support);
            x_dbg_stmt("e_enc =             0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.e_enc);
            x_dbg_stmt("steam number =      0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui2_strm_num);
            x_dbg_stmt("preroll =           0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui4_preroll);
            x_dbg_stmt("duration =          0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui4_duration);
            x_dbg_stmt("max_bit_rate =      0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui4_max_bit_rate);
            x_dbg_stmt("avg_bit_rate =      0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui4_avg_bit_rate);
            x_dbg_stmt("max_packet_size =   0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui4_max_packet_size);
            x_dbg_stmt("avg_packet_size =   0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui4_avg_packet_size);
            x_dbg_stmt("start_time =        0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui4_start_time);
            x_dbg_stmt("codec_id =          0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui4_codec_id);
            x_dbg_stmt("frm_width =         0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui2_frm_width);
            x_dbg_stmt("frm_height =        0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui2_frm_height);
            x_dbg_stmt("bit_count =         0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui2_bit_count);
            x_dbg_stmt("pad_width =         0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui2_pad_width);
            x_dbg_stmt("pad_height =        0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui2_pad_height);
            x_dbg_stmt("frm_rate =          0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui4_frm_rate);
            x_dbg_stmt("spo_extra_flags =   0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui4_spo_extra_flags);
            x_dbg_stmt("version =           0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui4_version);
            x_dbg_stmt("specific =          0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui4_specific);
            x_dbg_stmt("ecc_mask =          0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui1_ecc_mask);
            break;
            
        case MINFO_INFO_TYPE_AUDIO_ATTR:
            x_dbg_stmt("info type =         0x%.16x\r\n", 
                      t_minfo_rec.e_info_type);
            x_dbg_stmt("stream index =      0x%.16x\r\n", 
                      t_minfo_rec.ui1_rec_idx);
            x_dbg_stmt("is codec support =  0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.b_is_codec_support);
            x_dbg_stmt("e_enc =             0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.e_enc);
            x_dbg_stmt("steam number =      0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui2_strm_num);
            x_dbg_stmt("preroll =           0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui4_preroll);
            x_dbg_stmt("duration =          0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui4_duration);
            x_dbg_stmt("max_bit_rate =      0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui4_max_bit_rate);
            x_dbg_stmt("avg_bit_rate =      0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui4_avg_bit_rate);
            x_dbg_stmt("max_packet_size =   0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui4_max_packet_size);
            x_dbg_stmt("avg_packet_size =   0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui4_avg_packet_size);
            x_dbg_stmt("start_time =        0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui4_start_time);
            x_dbg_stmt("version =           0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui2_version);
            x_dbg_stmt("revision =          0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui2_revision);
            x_dbg_stmt("hdr_bytes =         0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui2_hdr_bytes);
            x_dbg_stmt("flavor_idx =        0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui2_flavor_idx);
            x_dbg_stmt("granularity =       0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui4_granularity);
            x_dbg_stmt("total_bytes =       0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui4_total_bytes);
            x_dbg_stmt("bytes_per_min =     0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui4_bytes_per_min);
            x_dbg_stmt("bytes_per_min2 =    0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui4_bytes_per_min2);
            x_dbg_stmt("intlv_factor =      0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui2_intlv_factor);
            x_dbg_stmt("intlv_blk_sz =      0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui2_intlv_blk_sz);
            x_dbg_stmt("codec_frm_sz =      0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui2_codec_frm_sz);
            x_dbg_stmt("user_data =         0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui4_user_data);
            x_dbg_stmt("user_data =         0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui4_user_data);
            x_dbg_stmt("sample_rate =       0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui4_sample_rate);
            x_dbg_stmt("act_sample_rate =   0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui4_act_sample_rate);
            x_dbg_stmt("sample_sz =         0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui2_sample_sz);
            x_dbg_stmt("num_channel =       0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui2_num_channel);
            x_dbg_stmt("intlvr_id =         0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui4_intlvr_id);
            x_dbg_stmt("codec_id =          0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui4_codec_id);
            x_dbg_stmt("intlv_flag =        0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui1_intlv_flag);
            x_dbg_stmt("can_copy_flag =     0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui1_can_copy_flag);
            x_dbg_stmt("strm_type =         0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui1_strm_type);
            x_dbg_stmt("has_intlv_ptrn =    0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui1_has_intlv_ptrn);
            x_dbg_stmt("opaque_data_sz =    0x%.16x\r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui4_opaque_data_sz);
            break;
            
        case MINFO_INFO_TYPE_GENERIC:
            x_dbg_stmt("info type =         0x%.16x\r\n", 
                      t_minfo_rec.e_info_type);
            x_dbg_stmt("stream index =      0x%.16x\r\n", 
                      t_minfo_rec.ui1_rec_idx);
            x_dbg_stmt("duration =          0x%.16x\r\n", 
                      t_minfo_rec.u.t_general_info.ui8_pb_duration);
            x_dbg_stmt("size =              0x%.16x\r\n", 
                      t_minfo_rec.u.t_general_info.ui8_media_sz);
            x_dbg_stmt("stream numbers =    0x%.16x\r\n", 
                      t_minfo_rec.u.t_general_info.ui1_strm_ns);
            break;
            
        case MINFO_INFO_TYPE_IDX_INFO:
            x_dbg_stmt("info type =         0x%.16x\r\n", 
                      t_minfo_rec.e_info_type);
            x_dbg_stmt("stream index =      0x%.16x\r\n", 
                      t_minfo_rec.ui1_rec_idx);
            x_dbg_stmt("idx_type =          0x%.16x\r\n", 
                      t_minfo_rec.u.t_idx_info.ui4_idx_type);
            x_dbg_stmt("index offset =      0x%.16x\r\n", 
                       t_minfo_rec.u.t_idx_info.ui8_idx_offset);
            x_dbg_stmt("index size =        0x%.16x\r\n", 
                       t_minfo_rec.u.t_idx_info.ui8_idx_sz);
            x_dbg_stmt("data offset =       0x%.16x\r\n", 
                       t_minfo_rec.u.t_idx_info.ui8_data_offset);
            x_dbg_stmt("data size =         0x%.16x\r\n", 
                       t_minfo_rec.u.t_idx_info.ui8_data_sz);
            break;
            
        case MINFO_INFO_TYPE_DRM_INFO:
            x_dbg_stmt("info type =         0x%.16x\r\n", 
                      t_minfo_rec.u.t_drm_info.t_minfo_drm_type);
            break;
            
        case MINFO_INFO_TYPE_META_DATA:
            x_dbg_stmt("title_len =         0x%.16x\r\n", 
                      t_minfo_rec.u.t_meta_data.z_title_len);
            x_dbg_stmt("director_len =      0x%.16x\r\n", 
                      t_minfo_rec.u.t_meta_data.z_director_len);
            x_dbg_stmt("genre_len =         0x%.16x\r\n", 
                      t_minfo_rec.u.t_meta_data.z_genre_len);
            x_dbg_stmt("b_cr_date_exist =   0x%.16x\r\n", 
                      t_minfo_rec.u.t_meta_data.b_cr_date_exist);
            x_dbg_stmt("frame_rate =        0x%.16x\r\n", 
                      t_minfo_rec.u.t_meta_data.t_frame_rate.ui4_rate);
            x_dbg_stmt("frame_rate_scale =  0x%.16x\r\n", 
                      t_minfo_rec.u.t_meta_data.t_frame_rate.ui4_scale);
            x_dbg_stmt("pb_duration =       0x%.16x\r\n", 
                      t_minfo_rec.u.t_meta_data.ui8_pb_duration);
            x_dbg_stmt("bitrate =           0x%.16x\r\n", 
                      t_minfo_rec.u.t_meta_data.ui4_bitrate);
            break;
            
        case MINFO_INFO_TYPE_SP_ATTR:
            break;
        default:
            x_dbg_stmt("get info unknown type ");
            break;
        }
        x_dbg_stmt("ogg minfo get info OK \r\n");
    }
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: minfo_cli_rm_fstress
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
static INT32 minfo_cli_ogg_fstress(INT32 i4_argc, const CHAR** pps_argv)
{

#ifndef __NO_FM__

    INT32           i4_ret;
    UINT16          ui2_num_recs;
    MEDIA_FORMAT_T  t_format;
    CHAR*           ps_file_path;
    MINFO_INFO_T    t_minfo_rec;
    SIZE_T          t_size;
    INT32           i4_loop_count;

    if ((i4_argc != 2) || (pps_argv == NULL))
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        x_dbg_stmt("usage: fstress filename  \r\n");
        return CLIR_INV_ARG;
    }
    /* check file path */
    ps_file_path = (CHAR*)pps_argv[1];
    if(ps_file_path == NULL)
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        x_dbg_stmt("usage: fstress filename  \r\n");
        return CLIR_INV_ARG;
    }
    
    /* set format info */
    t_format.e_media_type = MEDIA_TYPE_UNKNOWN;
    t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_UNKNOWN;

    i4_loop_count = 100000;

    while (i4_loop_count--)
    {
        /* open file */
        i4_ret = x_minfo_fopen(ps_file_path, x_minfo_nfy, NULL, &t_format, &gh_minfo);
        if (MINFOR_OK != i4_ret )
        {
            DBG_ERROR(("Failed to open file(%s)!,i4_ret = %d\r\n", 
                        ps_file_path, i4_ret));
            return CLIR_CMD_EXEC_ERROR;
        }
        x_dbg_stmt("minfo open file(%s) successfully!\r\n", ps_file_path);
        /* delay 2000ms for parser done */
        x_thread_delay(2000); 

        /* Get num recs */
        i4_ret = x_minfo_get_num_recs(gh_minfo,
                                      MINFO_INFO_TYPE_VIDEO_ATTR,
                                      &ui2_num_recs);
        if (i4_ret != MINFOR_OK)
        {
            x_dbg_stmt("ogg minfo get number FAIL!! \r\n");
            x_minfo_close(gh_minfo);
            x_dbg_stmt("ogg closed file! \r\n");
            continue;
        }
        x_dbg_stmt("get video num = %u \r\n", ui2_num_recs);
    
        x_minfo_get_num_recs(gh_minfo,
                             MINFO_INFO_TYPE_AUDIO_ATTR,
                             &ui2_num_recs);
        x_dbg_stmt("get audio num = %u \r\n", ui2_num_recs);

        x_minfo_get_num_recs(gh_minfo,
                             MINFO_INFO_TYPE_GENERIC,
                             &ui2_num_recs);
        x_dbg_stmt("get generic num = %u \r\n", ui2_num_recs);

        x_minfo_get_num_recs(gh_minfo,
                             MINFO_INFO_TYPE_IDX_INFO,
                             &ui2_num_recs);
        x_dbg_stmt("get idx_builder num = %u \r\n", ui2_num_recs);

        /* Get info */
        t_size = sizeof(MINFO_INFO_T);
        x_memset(&t_minfo_rec, 0, t_size);

        /* get minfo info */
        /* video info */
        i4_ret = x_minfo_get_info(gh_minfo,
                                  MINFO_INFO_TYPE_VIDEO_ATTR,
                                  1,
                                  (VOID*)&t_minfo_rec,
                                  &t_size);
        if (i4_ret != MINFOR_OK)
        {
            x_dbg_stmt("ogg minfo getinfo FAIL!! \r\n");
            x_minfo_close(gh_minfo);
            x_dbg_stmt("ogg closed file! \r\n");
            continue;
        }
        else
        {
            x_dbg_stmt("get video info steam id = %u \r\n", 
                      t_minfo_rec.u.t_stm_attribute.t_stm_id);
            x_dbg_stmt("get video info Width = %u \r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui2_pad_width);
            x_dbg_stmt("get video info Width = %u \r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui2_pad_height);
        }
        /* audio info */
        i4_ret = x_minfo_get_info(gh_minfo,
                                  MINFO_INFO_TYPE_AUDIO_ATTR,
                                  0,
                                  (VOID*)&t_minfo_rec,
                                  &t_size);
        x_dbg_stmt("get audio info steam id = %u \r\n", 
                  t_minfo_rec.u.t_stm_attribute.t_stm_id);
        x_dbg_stmt("get audio info sample rate = %u \r\n", 
                  t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui4_sample_rate);
        x_dbg_stmt("get audio info sample size = %u \r\n", 
                  t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui2_sample_sz);
        /* generic info */
        i4_ret = x_minfo_get_info(gh_minfo,
                                  MINFO_INFO_TYPE_GENERIC,
                                  1,
                                  (VOID*)&t_minfo_rec,
                                  &t_size);
        x_dbg_stmt("get general info duration = %llu \r\n", 
                  t_minfo_rec.u.t_general_info.ui8_pb_duration);
        /* index builder info */
        i4_ret = x_minfo_get_info(gh_minfo,
                                  MINFO_INFO_TYPE_IDX_INFO,
                                  1,
                                  (VOID*)&t_minfo_rec,
                                  &t_size);
        x_dbg_stmt("get idx_builder index offset = 0x%llx \r\n", 
                   t_minfo_rec.u.t_idx_info.ui8_idx_offset);
        x_dbg_stmt("get idx_builder index size = 0x%llx \r\n", 
                   t_minfo_rec.u.t_idx_info.ui8_idx_sz);
        x_dbg_stmt("get idx_builder data offset = 0x%llx \r\n", 
                   t_minfo_rec.u.t_idx_info.ui8_data_offset);
        x_dbg_stmt("get idx_builder data size = 0x%llx \r\n", 
                   t_minfo_rec.u.t_idx_info.ui8_data_sz);

        /* close file */
        i4_ret = x_minfo_close(gh_minfo);
        if (MINFOR_OK != i4_ret)
        {
            DBG_ERROR(("ogg closed error=%d \r\n", i4_ret));
            return CLIR_CMD_EXEC_ERROR;
        }

        x_dbg_stmt("ogg closed successfully! \r\n");
        
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
 * Name: minfo_cli_rm_mstress
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
static INT32 minfo_cli_ogg_mstress(INT32 i4_argc, const CHAR** pps_argv)
{    

#ifndef __NO_FM__

    INT32           i4_ret;
    UINT16          ui2_num_recs;
    MEDIA_FORMAT_T  t_format;
    CHAR*           ps_file_path;
    MINFO_INFO_T    t_minfo_rec;
    SIZE_T          t_size;
    FM_FILE_INFO_T  t_fm_info;
    UINT64          ui8_size;
    UINT64          ui8_cur_pos;
    UINT32          ui4_read;
    MEDIA_FEEDER_T  t_feeder;
    INT32            i4_loop_count;

    if ((i4_argc != 3) || (pps_argv == NULL))
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        x_dbg_stmt("usage: mstress filename mem_length(0/1) \r\n");
        return CLIR_INV_ARG;
    }
    /* check file path */
    ps_file_path = (CHAR*)pps_argv[1];
    if(ps_file_path == NULL)
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        x_dbg_stmt("usage: mstress filename mem_length(0/1) \r\n");
        return CLIR_INV_ARG;
    }
    
    /* set format info */
    t_format.e_media_type = MEDIA_TYPE_UNKNOWN;
    t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_UNKNOWN;

    i4_loop_count = 100000;

    while (i4_loop_count--)

    {
        /* open file and fill data */
        i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_file_path, FM_READ_ONLY, 0777, FALSE, &gh_file);
        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("fail to open file \r\n");
            return CLIR_CMD_EXEC_ERROR;
        }

        /* set memory length */
        if (0 == x_strncmp(pps_argv[2], "0", (SIZE_T)1))
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
        
        /* open file */
        if(ui8_size == 0)
        {
            i4_ret = x_minfo_mopen((VOID*)&t_feeder, 
                                   (SIZE_T)0, 
                                   x_minfo_nfy, 
                                   NULL, 
                                   &t_format, 
                                   &gh_minfo);
        }
        else
        {
            i4_ret = x_minfo_mopen((VOID*)gpui1_buf, 
                                   (SIZE_T)ui8_size, 
                                   x_minfo_nfy, 
                                   NULL, 
                                   &t_format, 
                                   &gh_minfo);
        }
        if (MINFOR_OK != i4_ret )
        {
            DBG_ERROR(("Failed to open file(%s)!,i4_ret = %d\r\n", 
                        ps_file_path, i4_ret));
            return CLIR_CMD_EXEC_ERROR;
        }
        x_dbg_stmt("minfo open file(%s) successfully!\r\n", ps_file_path);
        /* delay 2000ms for parser done */
        x_thread_delay(2000); 

        /* Get num recs */
        i4_ret = x_minfo_get_num_recs(gh_minfo,
                                      MINFO_INFO_TYPE_VIDEO_ATTR,
                                      &ui2_num_recs);
        if (i4_ret != MINFOR_OK)
        {
            x_dbg_stmt("ogg minfo get number FAIL!! \r\n");
            x_minfo_close(gh_minfo);
            x_dbg_stmt("ogg closed file! \r\n");
            continue;
        }
        x_dbg_stmt("get video num = %u \r\n", ui2_num_recs);
    
        x_minfo_get_num_recs(gh_minfo,
                             MINFO_INFO_TYPE_AUDIO_ATTR,
                             &ui2_num_recs);
        x_dbg_stmt("get audio num = %u \r\n", ui2_num_recs);

        x_minfo_get_num_recs(gh_minfo,
                             MINFO_INFO_TYPE_GENERIC,
                             &ui2_num_recs);
        x_dbg_stmt("get generic num = %u \r\n", ui2_num_recs);

        x_minfo_get_num_recs(gh_minfo,
                             MINFO_INFO_TYPE_IDX_INFO,
                             &ui2_num_recs);
        x_dbg_stmt("get idx_builder num = %u \r\n", ui2_num_recs);

        /* Get info */
        t_size = sizeof(MINFO_INFO_T);
        x_memset(&t_minfo_rec, 0, t_size);

        /* get minfo info */
        /* video info */
        i4_ret = x_minfo_get_info(gh_minfo,
                                  MINFO_INFO_TYPE_VIDEO_ATTR,
                                  1,
                                  (VOID*)&t_minfo_rec,
                                  &t_size);
        if (i4_ret != MINFOR_OK)
        {
            x_dbg_stmt("ogg minfo getinfo FAIL!! \r\n");
            x_minfo_close(gh_minfo);
            x_dbg_stmt("ogg closed file! \r\n");
            continue;
        }
        else
        {
            x_dbg_stmt("get video info steam id = %u \r\n", 
                      t_minfo_rec.u.t_stm_attribute.t_stm_id);
            x_dbg_stmt("get video info Width = %u \r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui2_pad_width);
            x_dbg_stmt("get video info Width = %u \r\n", 
                      t_minfo_rec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui2_pad_height);
        }
        /* audio info */
        i4_ret = x_minfo_get_info(gh_minfo,
                                  MINFO_INFO_TYPE_AUDIO_ATTR,
                                  0,
                                  (VOID*)&t_minfo_rec,
                                  &t_size);
        x_dbg_stmt("get audio info steam id = %u \r\n", 
                  t_minfo_rec.u.t_stm_attribute.t_stm_id);
        x_dbg_stmt("get audio info sample rate = %u \r\n", 
                  t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui4_sample_rate);
        x_dbg_stmt("get audio info sample size = %u \r\n", 
                  t_minfo_rec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui2_sample_sz);
        /* generic info */
        i4_ret = x_minfo_get_info(gh_minfo,
                                  MINFO_INFO_TYPE_GENERIC,
                                  1,
                                  (VOID*)&t_minfo_rec,
                                  &t_size);
        x_dbg_stmt("get general info duration = %llu \r\n", 
                  t_minfo_rec.u.t_general_info.ui8_pb_duration);
        /* index builder info */
        i4_ret = x_minfo_get_info(gh_minfo,
                                  MINFO_INFO_TYPE_IDX_INFO,
                                  1,
                                  (VOID*)&t_minfo_rec,
                                  &t_size);
        x_dbg_stmt("get idx_builder index offset = 0x%llx \r\n", 
                   t_minfo_rec.u.t_idx_info.ui8_idx_offset);
        x_dbg_stmt("get idx_builder index size = 0x%llx \r\n", 
                   t_minfo_rec.u.t_idx_info.ui8_idx_sz);
        x_dbg_stmt("get idx_builder data offset = 0x%llx \r\n", 
                   t_minfo_rec.u.t_idx_info.ui8_data_offset);
        x_dbg_stmt("get idx_builder data size = 0x%llx \r\n", 
                   t_minfo_rec.u.t_idx_info.ui8_data_sz);

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
        i4_ret = x_minfo_close(gh_minfo);
        if (MINFOR_OK != i4_ret)
        {
            DBG_ERROR(("ogg closed error=%d \r\n", i4_ret));
            return CLIR_CMD_EXEC_ERROR;
        }

        x_dbg_stmt("ogg closed successfully! \r\n");
        
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
 * Name: ogg_minfo_cli_init
 *
 * Description: This API initializes midxbuld CLI component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 ogg_minfo_cli_init(CLI_EXEC_T** ppt_command_array)
{
#ifdef CLI_LVL_ALL

    *ppt_command_array = at_ogg_minfo_cmd_tbl;
#endif
    return 0;
}



