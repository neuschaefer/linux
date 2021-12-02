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
 * $RCSfile: drm_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision:  $
 * $SWAuthor:  $
 * $MD5HEX:  $
 *
 * Description:
 *         
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "dbg/x_dbg.h"
#include "cli/x_cli.h"
#include "drm/x_drm.h"
#include "drm/drm_cli.h"
#include "drm/drm_dbg.h"
#include "drm/divx_drm/divx_drm_cli.h"
#include "drm/divx_drm/x_divx_drm.h"
#include "mutil/minfo/x_minfo.h"

#else

#include "x_common.h"
#include "x_os.h"
#include "x_dbg.h"
#include "x_cli.h"
#include "x_drm.h"
#include "drm_cli.h"
#include "drm_dbg.h"
#include "divx_drm/divx_drm_cli.h"
#include "divx_drm/x_divx_drm.h"
#include "x_minfo.h"

#endif

#ifdef CLI_SUPPORT
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define MAX_TEST_ITEM 10

/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static BOOL b_nfy = 0;

static DRM_COMMAND_T at_drm_cmd_set[MAX_TEST_ITEM][3] = 
{
    {{DRM_CMD_CODE_DIVX_DRM_STRD_CHUNK,}, {DRM_CMD_CODE_DIVX_DRM_REGISTRATION_CODE,}, {DRM_CMD_CODE_END,}},
    {{DRM_CMD_CODE_DIVX_DRM_STRD_CHUNK,}, {DRM_CMD_CODE_DIVX_DRM_REGISTRATION_CODE,}, {DRM_CMD_CODE_END,}},
    {{DRM_CMD_CODE_DIVX_DRM_STRD_CHUNK,}, {DRM_CMD_CODE_DIVX_DRM_REGISTRATION_CODE,}, {DRM_CMD_CODE_END,}},
    {{DRM_CMD_CODE_DIVX_DRM_STRD_CHUNK,}, {DRM_CMD_CODE_DIVX_DRM_REGISTRATION_CODE,}, {DRM_CMD_CODE_END,}},
    {{DRM_CMD_CODE_DIVX_DRM_STRD_CHUNK,}, {DRM_CMD_CODE_DIVX_DRM_REGISTRATION_CODE,}, {DRM_CMD_CODE_END,}},
    {{DRM_CMD_CODE_DIVX_DRM_STRD_CHUNK,}, {DRM_CMD_CODE_DIVX_DRM_REGISTRATION_CODE,}, {DRM_CMD_CODE_END,}},
    {{DRM_CMD_CODE_DIVX_DRM_STRD_CHUNK,}, {DRM_CMD_CODE_DIVX_DRM_REGISTRATION_CODE,}, {DRM_CMD_CODE_END,}},
    {{DRM_CMD_CODE_DIVX_DRM_STRD_CHUNK,}, {DRM_CMD_CODE_DIVX_DRM_REGISTRATION_CODE,}, {DRM_CMD_CODE_END,}},
    {{DRM_CMD_CODE_DIVX_DRM_STRD_CHUNK,}, {DRM_CMD_CODE_DIVX_DRM_REGISTRATION_CODE,}, {DRM_CMD_CODE_END,}}
};

static DRM_TEST_COMMAND_T at_drm_test_cmd_tbl[MAX_TEST_ITEM] =
{
    /* owner guard: 91 90 8f */
    {"/mnt/usb/Mass-000/test/V05_HT_user1_type4_key5_serial0_slot0.avi", at_drm_cmd_set[0], DRM_TYPE_DIVX_DRM, 0, 0},
    {"/mnt/usb/Mass-000/test/V201_base.avi", at_drm_cmd_set[1], DRM_TYPE_DIVX_DRM, 0, 0},
    {"/mnt/usb/Mass-000/test/V202_purchase_user1.avi", at_drm_cmd_set[2], DRM_TYPE_DIVX_DRM, 0, 0},
    {"/mnt/usb/Mass-000/test/V203_rental_user1.avi", at_drm_cmd_set[3], DRM_TYPE_DIVX_DRM,  0, 0},
    {"/mnt/usb/Mass-000/test/V204_purchase_user2.avi", at_drm_cmd_set[4], DRM_TYPE_DIVX_DRM,  0, 0},
    {"/mnt/usb/Mass-000/test/V205_base_metadata_2.avi", at_drm_cmd_set[5], DRM_TYPE_DIVX_DRM, 0, 0},
    {"/mnt/usb/Mass-000/test/V212_HD_purchase_user1.divx", at_drm_cmd_set[6], DRM_TYPE_DIVX_DRM, 0, 0},
    {"/mnt/usb/Mass-000/test/V213_HD_rental_user1.divx", at_drm_cmd_set[7], DRM_TYPE_DIVX_DRM, 0, 0},
    {"/mnt/usb/Mass-000/test/V214_HD_purchase_user2.divx", at_drm_cmd_set[8], DRM_TYPE_DIVX_DRM, 0, 0}
};

/* drm cli function declaration */
static INT32 drm_cli_get_dbg_level (INT32 i4_argc, const CHAR** pps_argv);
static INT32 drm_cli_set_dbg_level (INT32 i4_argc, const CHAR** pps_argv);
static INT32 drm_cli_open (INT32 i4_argc, const CHAR** pps_argv);
static INT32 drm_cli_close (INT32 i4_argc, const CHAR** pps_argv);
static INT32 drm_cli_get_info (INT32 i4_argc, const CHAR** pps_argv);
static INT32 drm_cli_set_info (INT32 i4_argc, const CHAR** pps_argv);

/* drm command table */
static CLI_EXEC_T at_drm_cmd_tbl[] =
{
    {CLI_GET_DBG_LVL_STR, NULL, drm_cli_get_dbg_level, NULL, CLI_GET_DBG_LVL_HELP_STR, CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR, NULL, drm_cli_set_dbg_level, NULL, CLI_SET_DBG_LVL_HELP_STR, CLI_SUPERVISOR},
    {"divx", NULL, NULL, NULL, "divx drm commands", CLI_SUPERVISOR},
    {"wm", NULL, NULL, NULL, "wm drm commands", CLI_SUPERVISOR},    
    {"open", NULL, drm_cli_open, NULL, "drm open", CLI_SUPERVISOR},
    {"close", NULL, drm_cli_close, NULL, "drm close", CLI_SUPERVISOR},
    {"getinfo", NULL, drm_cli_get_info, NULL, "drm get info", CLI_SUPERVISOR},
    {"setinfo", NULL, drm_cli_set_info, NULL, "drm set info", CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

/* drm root command table */
static CLI_EXEC_T at_drm_root_cmd_tbl[] =
{
    {"drm", NULL, NULL, at_drm_cmd_tbl, "drm commands", CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: drm_cli_get_dbg_level
 *
 * Description: This API retrives current drm debug level.
 *
 * Inputs: i4_argc  Specity the argument count.
 *         pps_argv  Specity the arguments.
 *
 * Outputs: NONE.
 *
 * Returns: CLIR_OK  Success.
 *          CLIR_NOT_ENABLED  Fail.
 ----------------------------------------------------------------------------*/
static INT32 drm_cli_get_dbg_level (
    INT32 i4_argc,
    const CHAR** pps_argv)
{
    INT32 i4_ret;

    i4_ret = x_cli_show_dbg_level(drm_get_dbg_level());

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: drm_cli_set_dbg_level
 *
 * Description: This API sets current drm debug level.
 *
 * Inputs: i4_argc  Specify the argument count.
 *         pps_argv  Specity the arguments.
 *
 * Outputs: NONE.
 *
 * Returns: CLIR_OK  Success.
 *          CLIR_CMD_EXEC_ERROR  Fail.
 *          CLIR_NOT_ENABLED  Fail.
 ----------------------------------------------------------------------------*/
static INT32 drm_cli_set_dbg_level (
    INT32 i4_argc,
    const CHAR** pps_argv)
{
    INT32 i4_ret;
    UINT16 ui2_dbg_level;

    i4_ret = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);
    if (i4_ret == CLIR_OK)
    {
        drm_set_dbg_level(ui2_dbg_level);
    }
    else
    {
        i4_ret = CLIR_CMD_EXEC_ERROR;
    }

    i4_ret = x_cli_show_dbg_level(drm_get_dbg_level());

    DBG_INFO((_INFO_HEADER"%s: %s\r\n", __FUNCTION__, (i4_ret == CLIR_OK) ? "successful" : "failed"));
    DBG_API((_API_HEADER"%s: %s\r\n", __FUNCTION__, (i4_ret == CLIR_OK) ? "successful" : "failed"));
    DBG_ERROR((_ERROR_HEADER"%s: %s\r\n", __FUNCTION__, (i4_ret == CLIR_OK) ? "successful" : "failed"));    
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _x_minfo_nfy
 *
 * Description: -
 *
 * Inputs: -
 *
 * Outputs: NONE.
 *
 * Returns: NONE.
 ----------------------------------------------------------------------------*/
static VOID _x_minfo_nfy (
    HANDLE_T h_minfo, 
    MINFO_NFY_EVENT_TYPE_T e_event,  
    const VOID* pv_data1, 
    const VOID* pv_data2, 
    const VOID* pv_tag)
{    
    x_dbg_stmt("minfo nfy\r\n");

    b_nfy = 1;
}

/*-----------------------------------------------------------------------------
 * Name: drm_cli_open
 *
 * Description: This API tests drm open.
 *
 * Inputs: i4_argc  Specify the argument count.
 *         pps_argv  Specity the arguments.
 *
 * Outputs: NONE.
 *
 * Returns: CLIR_OK  Success.
 *          CLIR_INV_ARG  Fail.
 *          CLIR_CMD_EXEC_ERROR  Fail.
 ----------------------------------------------------------------------------*/
static INT32 drm_cli_open (
    INT32 i4_argc, 
    const CHAR** pps_argv)
{
    INT32 i4_ret, i4_idx;
    MEDIA_FORMAT_T t_format;
    MINFO_INFO_T t_info;
    SIZE_T z_size;
    char s_registration_code[9] = {0};

    i4_idx = (INT32)x_strtoll(pps_argv[1], NULL, 10);
    if ((i4_idx < 0) || (i4_argc != 2))
    {
        return CLIR_INV_ARG;
    }

    if (at_drm_test_cmd_tbl[i4_idx].h_minfo != 0)
    {
        x_dbg_stmt(" [drm_cli_open]: file already opened\r\n");
        return CLIR_INV_ARG;    
    }
    
    t_format.e_media_type = MEDIA_TYPE_CONTAINER;
    t_format.t_media_subtype.u.e_contnr_subtype  = MEDIA_CONTNR_SUBTYPE_AVI;
    i4_ret = x_minfo_fopen(at_drm_test_cmd_tbl[i4_idx].s_file_path, _x_minfo_nfy, (VOID*)0, &t_format, &(at_drm_test_cmd_tbl[i4_idx].h_minfo));
    if (i4_ret != MINFOR_OK)
    {
        x_dbg_stmt(" [x_minfo_fopen]: MINFO error code = %d\r\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;        
    }

    do {
        x_thread_delay(10);
    } while (b_nfy != 1);
    b_nfy = 0;
    x_thread_delay(1000);    

    z_size = sizeof(MINFO_INFO_T);
    i4_ret = x_minfo_get_info(at_drm_test_cmd_tbl[i4_idx].h_minfo, MINFO_INFO_TYPE_DRM_INFO, 0, (VOID*)&t_info, &z_size);
    if (i4_ret != MINFOR_OK)
    {
        x_dbg_stmt(" [x_minfo_get_info]: MINFO error code = %d\r\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;        
    }

    if (t_info.u.t_drm_info.t_minfo_drm_type == DRM_TYPE_DIVX_DRM)
    {
        at_drm_test_cmd_tbl[i4_idx].pt_drm_cmd_sets[0].u.pv_data = t_info.u.t_drm_info.u.t_minfo_divx_drm_obj.pui1_strd_chunk;
        at_drm_test_cmd_tbl[i4_idx].pt_drm_cmd_sets[1].u.ps_text = s_registration_code;
    }

    if (t_info.u.t_drm_info.t_minfo_drm_type == at_drm_test_cmd_tbl[i4_idx].t_drm_type)
    {
        i4_ret = x_drm_open(at_drm_test_cmd_tbl[i4_idx].pt_drm_cmd_sets, at_drm_test_cmd_tbl[i4_idx].t_drm_type, &(at_drm_test_cmd_tbl[i4_idx].h_drm));
        if (i4_ret != DRMR_OK)
        {
            x_dbg_stmt(" [x_drm_open]: DRM error code = %d\r\n", i4_ret);
            return CLIR_CMD_EXEC_ERROR;        
        }        

        x_dbg_stmt(" [x_drm_open]: DRM s_registration_code = %s\r\n", s_registration_code);
    }
    else
    {
        return CLIR_CMD_EXEC_ERROR;        
    }

    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: drm_cli_close
 *
 * Description: This API tests drm close.
 *
 * Inputs: i4_argc  Specify the argument count.
 *         pps_argv  Specity the arguments.
 *
 * Outputs: NONE.
 *
 * Returns: CLIR_OK  Success.
 *          CLIR_INV_ARG  Fail.
 *          CLIR_CMD_EXEC_ERROR  Fail.
 ----------------------------------------------------------------------------*/
static INT32 drm_cli_close (
    INT32 i4_argc, 
    const CHAR** pps_argv)
{
    INT32 i4_ret, i4_idx;

    i4_idx = (INT32)x_strtoll(pps_argv[1], NULL, 10);
    if ((i4_idx < 0) || (i4_argc != 2))
    {
        return CLIR_INV_ARG;
    }

    i4_ret = x_drm_close (at_drm_test_cmd_tbl[i4_idx].h_drm);
    at_drm_test_cmd_tbl[i4_idx].h_drm = 0;
    if (i4_ret != DRMR_OK)
    {
        x_dbg_stmt(" [x_drm_close]: DRM error code = %d\r\n", i4_ret);
    }

    i4_ret = x_minfo_close(at_drm_test_cmd_tbl[i4_idx].h_minfo);
    at_drm_test_cmd_tbl[i4_idx].h_minfo = 0;
    if (i4_ret != MINFOR_OK)
    {
        x_dbg_stmt(" [x_minfo_close]: MINFO error code = %d\r\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }    

    return CLIR_OK;    
}

/*-----------------------------------------------------------------------------
 * Name: drm_cli_get_info
 *
 * Description: This API tests drm get info.
 *
 * Inputs: i4_argc  Specify the argument count.
 *         pps_argv  Specity the arguments.
 *
 * Outputs: NONE.
 *
 * Returns: CLIR_OK  Success.
 *          CLIR_INV_ARG  Fail.
 *          CLIR_CMD_EXEC_ERROR  Fail.
 ----------------------------------------------------------------------------*/
static INT32 drm_cli_get_info (
    INT32 i4_argc, 
    const CHAR** pps_argv)
{
    INT32 i4_ret, i4_idx;
    DIVX_DRM_UNION_INFO_T t_divx_drm_info;
    UINT8 ui1_i, ui1_j;

    i4_idx = (INT32)x_strtoll(pps_argv[1], NULL, 10);
    if ((i4_idx < 0) || (i4_argc != 2))
    {
        return CLIR_INV_ARG;
    }
    
    x_memset(&t_divx_drm_info, 0, sizeof(DIVX_DRM_UNION_INFO_T));
    i4_ret = x_drm_get_info(at_drm_test_cmd_tbl[i4_idx].h_drm, DRM_GET_TYPE_DIVX_DRM_BASIC_INFO, &t_divx_drm_info, sizeof(DIVX_DRM_UNION_INFO_T));
    if (i4_ret != DRMR_OK)
    {
        x_dbg_stmt(" [x_drm_get_info]: DRM error code = %d\r\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;         
    }
    else
    {
        x_dbg_stmt(" [x_drm_get_info]: DRM ui1_flag = %d\r\n", t_divx_drm_info.pt_divx_drm_basic_info->ui1_flag);
        x_dbg_stmt(" [x_drm_get_info]: DRM ui1_use_count = %d\r\n", t_divx_drm_info.pt_divx_drm_basic_info->ui1_use_count);
        x_dbg_stmt(" [x_drm_get_info]: DRM ui1_use_limit = %d\r\n\r\n", t_divx_drm_info.pt_divx_drm_basic_info->ui1_use_limit);        
    }

    x_memset(&t_divx_drm_info, 0, sizeof(DIVX_DRM_UNION_INFO_T)); 
    i4_ret = x_drm_get_info(at_drm_test_cmd_tbl[i4_idx].h_drm, DRM_GET_TYPE_OUTPUT_SIGNAL_PROTECTION_INFO, &t_divx_drm_info, sizeof(DIVX_DRM_UNION_INFO_T));
    if (i4_ret != DRMR_OK)
    {
        x_dbg_stmt(" [x_drm_get_info]: DRM error code = %d\r\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;        
    }
    else
    {
        x_dbg_stmt(" [x_drm_get_info]: DRM cgmsa_flag = %d\r\n", t_divx_drm_info.pt_drm_output_signal_protection_info->ui1_cgmsa_flag);
        x_dbg_stmt(" [x_drm_get_info]: DRM acptb_flag = %d\r\n", t_divx_drm_info.pt_drm_output_signal_protection_info->ui1_acptb_flag);
        x_dbg_stmt(" [x_drm_get_info]: DRM digital_protection_flag = %d\r\n\r\n", t_divx_drm_info.pt_drm_output_signal_protection_info->ui1_digital_protection_flag);
    }

    x_memset(&t_divx_drm_info, 0, sizeof(DIVX_DRM_UNION_INFO_T));    
    i4_ret = x_drm_get_info(at_drm_test_cmd_tbl[i4_idx].h_drm, DRM_GET_TYPE_DIVX_DRM_CRYPTO_INFO, &t_divx_drm_info, sizeof(DIVX_DRM_UNION_INFO_T));
    if (i4_ret != DRMR_OK)
    {
        x_dbg_stmt(" [x_drm_get_info]: DRM error code = %d\r\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;        
    }
    else
    {
        x_dbg_stmt(" [x_drm_get_info]: DRM ui1_flag = %d\r\n", t_divx_drm_info.pt_divx_drm_crypto_info->ui1_flag);
        x_dbg_stmt(" [x_drm_get_info]: DRM ui1_frame_keys_count = %d\r\n", t_divx_drm_info.pt_divx_drm_crypto_info->ui1_frame_keys_count);
        x_dbg_stmt(" [x_drm_get_info]: DRM ui1_protected_audio_offset = %d\r\n", t_divx_drm_info.pt_divx_drm_crypto_info->ui1_protected_audio_offset);
        x_dbg_stmt(" [x_drm_get_info]: DRM ui1_protected_audio_crypto_size = %d\r\n", t_divx_drm_info.pt_divx_drm_crypto_info->ui1_protected_audio_crypto_size);
        x_dbg_stmt(" [x_drm_get_info]: DRM aui1_frame_keys\r\n");
        for (ui1_i=0; ui1_i<t_divx_drm_info.pt_divx_drm_crypto_info->ui1_frame_keys_count; ui1_i++)
        {
            x_dbg_stmt(" [%3d]", ui1_i);
            for (ui1_j=0; ui1_j<16/*VIDEO_KEY_SIZE_BYTES*/; ui1_j++)
            {
                x_dbg_stmt(" %2x", t_divx_drm_info.pt_divx_drm_crypto_info->aui1_frame_keys[ui1_i][ui1_j]);
            }
            x_dbg_stmt("\r\n");
        }
    }

    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: drm_cli_set_info
 *
 * Description: This API tests drm set info.
 *
 * Inputs: i4_argc  Specify the argument count.
 *         pps_argv  Specity the arguments.
 *
 * Outputs: NONE.
 *
 * Returns: CLIR_OK  Success.
 *          CLIR_INV_ARG  Fail. 
 *          CLIR_CMD_EXEC_ERROR  Fail.
 ----------------------------------------------------------------------------*/
static INT32 drm_cli_set_info (
    INT32 i4_argc, 
    const CHAR** pps_argv)
{
    INT32 i4_ret, i4_idx;

    i4_idx = (INT32)x_strtoll(pps_argv[1], NULL, 10);
    if ((i4_idx < 0) || (i4_argc != 2))
    {
        return CLIR_INV_ARG;
    }

    i4_ret = x_drm_set_info(at_drm_test_cmd_tbl[i4_idx].h_drm, DRM_SET_TYPE_DIVX_DRM_PLAYBACK, NULL, NULL);
    if (i4_ret != DRMR_OK)
    {
        x_dbg_stmt(" [x_drm_set_info]: DRM error code = %d\r\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: drm_cli_init
 *
 * Description: This API initializes the drm CLI component.
 *
 * Inputs: t_drm_type  Specify the drm type.
 *
 * Outputs: NONE.
 *
 * Returns: DRMR_OK  Success.
 *          DRMR_CLI_ERROR  Fail.
 ----------------------------------------------------------------------------*/
INT32 drm_cli_init (
    DRM_TYPE_T t_drm_type)
{
    INT32 i4_ret;
    CLI_EXEC_T* pt_cmd_array;

    /* Attach the drm command array */
    if (DRM_TYPE_DIVX_DRM == t_drm_type)
    {
        divx_drm_cli_init(&pt_cmd_array);
        at_drm_cmd_tbl[2].pt_next_level = pt_cmd_array;
    }

    /* Attach the drm CLI command table to CLI */
    i4_ret = x_cli_attach_cmd_tbl(at_drm_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_NONE);
    if (i4_ret != CLIR_OK)
    {
        return DRMR_CLI_ERROR;
    }

    return DRMR_OK;
}

#else

/*-----------------------------------------------------------------------------
 * Name: drm_cli_init
 *
 * Description: This API initializes the drm CLI component.
 *
 * Inputs: t_drm_type  Specify the drm type.
 *
 * Outputs: NONE.
 *
 * Returns: DRMR_OK  Success.
 *          DRMR_CLI_ERROR  Fail.
 ----------------------------------------------------------------------------*/
INT32 drm_cli_init (
    DRM_TYPE_T t_drm_type)
{
    return DRMR_OK;
}

#endif /* CLI_SUPPORT */

