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
 * $RCSfile: mp4_mfmtrecg_cli.c,v $
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

#include "mutil/mm_util.h"

#include "os/inc/x_os.h"
#include "inc/x_common.h"
#include "inc/x_mm_common.h"
#include "mutil/mfmtrecg/u_mfmtrecg.h"
#include "mutil/mfmtrecg/mfmtrecg.h"
#include "mutil/mfmtrecg/mp4_mfmtrecg/mp4_mfmtrecg.h"
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
#include "../u_mfmtrecg.h"
#include "../mfmtrecg.h"
#include "mp4_mfmtrecg.h"
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
#if (ENABLE_MM_CLI_TEST == 1) 
#ifdef CLI_LVL_ALL
static INT32 mfmtrecg_cli_mp4_fconfirm_fct(INT32 i4_argc, const CHAR** pps_argv);     
static INT32 mfmtrecg_cli_mp4_mconfirm_fct(INT32 i4_argc, const CHAR** pps_argv);     

/* mfmtrecg command table */
static CLI_EXEC_T at_mp4_mfmtrecg_cmd_tbl[] =
{
    {"mp4 fconfirm" ,   "mp4f",                     mfmtrecg_cli_mp4_fconfirm_fct,  
     NULL,              "MP4 format file check",    CLI_SUPERVISOR},
    {"mp4 mconfirm" ,   "mp4m",                     mfmtrecg_cli_mp4_mconfirm_fct,  
     NULL,              "MP4 format mem check",     CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

#endif
/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: mfmtrecg_cli_mp4_fconfirm_fct
 *
 * Description: This API returns the media format for file handle.
 *
 * Inputs: -command format : mp4f  /mnt/usb/Mass-000/file_path
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 mfmtrecg_cli_mp4_fconfirm_fct(INT32 i4_argc, const CHAR** pps_argv)
{

#ifndef __NO_FM__

    HANDLE_T            h_file;
    INT32               i4_ret = CLIR_OK;
    MEDIA_FORMAT_T      t_format;
    CHAR*               ps_file_path;
    
    if ((i4_argc != 2) || (pps_argv == NULL))
    {
        x_dbg_stmt("Invalid argument! \r\n");
        DBG_INFO(("Usage: mp4f filepath \r\n"));
        return CLIR_INV_ARG;
    }
    ps_file_path = (CHAR*)pps_argv[1];
    i4_ret = x_fm_open(FM_ROOT_HANDLE, 
                       ps_file_path, 
                       FM_READ_ONLY, 
                       0777, 
                       FALSE, 
                       &h_file);
    do
    {
        if(i4_ret != FMR_OK)
        {
            x_dbg_stmt("File Open Failed!\r\n");
            i4_ret =  CLIR_INV_ARG;
            break;
        }
        i4_ret = mp4_handler_file_confirm_fct(h_file, &t_format);
        if(i4_ret != MFMTRECGR_OK)
        {
            x_dbg_stmt("File could not be recognized, i4_ret=%d\r\n", i4_ret);
            i4_ret =  CLIR_INV_ARG;
            break;
        }
        x_dbg_stmt("Media Format is MP4!\r\n");
    }while(0);
    x_fm_close(h_file);   
    return i4_ret;

#else
    return CLIR_OK;
#endif

}
#endif
/*-----------------------------------------------------------------------------
 * Name: mfmtrecg_cli_mp4_fconfirm_fct
 *
 * Description: This API returns the media format for memory clip.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
static INT32 mfmtrecg_cli_mp4_mconfirm_fct(INT32 i4_argc, const CHAR** pps_argv)
{  

#ifndef __NO_FM__

    HANDLE_T            h_file;
    INT32               i4_ret = CLIR_OK;
    MEDIA_FORMAT_T      t_format;
    CHAR*               ps_file_path;
    FM_FILE_INFO_T      t_fm_info;
    UINT8*              pui1_buf;
    UINT64              ui8_size;
    UINT64              ui8_cur_pos;
    UINT32              ui4_read;
    MEDIA_FEEDER_T      t_feeder;
    
    if ((i4_argc != 3) || (pps_argv == NULL))
    {
        x_dbg_stmt("Invalid argument! \r\n");
        DBG_INFO(("Usage: mp4m filepath mem_length(0/1) \r\n"));
        return CLIR_INV_ARG;
    }
    ps_file_path = (CHAR*)pps_argv[1];

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
        t_feeder.t_feeder_be_fct_tbl.pf_copybytes = cli_feeder_copybytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_get_pos = cli_feeder_getpos_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input1 = cli_feeder_input1bytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input2 = cli_feeder_input2bytes_b_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input3 = cli_feeder_input3bytes_b_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input4 = cli_feeder_input4bytes_b_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_set_pos = cli_feeder_setpos_fct;
        
        i4_ret = mp4_handler_mem_confirm_fct((VOID*)&t_feeder, 
                                             (SIZE_T)0, 
                                             &t_format);
    }
    else
    {
        /* set memory length */
        x_fm_get_info_by_handle(h_file, &t_fm_info);
        ui8_size = t_fm_info.ui8_size;
        pui1_buf = (UINT8*)_mm_util_mem_alloc(sizeof(UINT8) * ui8_size);
        if(pui1_buf == NULL)
        {
            x_dbg_stmt("mopen alloc memory fail \r\n");
            x_fm_close(h_file);   
            h_file = NULL_HANDLE;
            return CLIR_CMD_EXEC_ERROR;
        }
        x_memset(pui1_buf, 0, ui8_size);
        i4_ret = x_fm_lseek(h_file, 0, FM_SEEK_BGN, &ui8_cur_pos);
        if(i4_ret != FMR_OK)
        {
            x_fm_close(h_file);   
            _mm_util_mem_free(pui1_buf);
            return CLIR_CMD_EXEC_ERROR;
        }
        i4_ret = x_fm_read(h_file, (VOID*)pui1_buf, ui8_size, &ui4_read);
        if (i4_ret != FMR_OK)
        {
            x_fm_close(h_file);   
            _mm_util_mem_free(pui1_buf);
            return CLIR_CMD_EXEC_ERROR;
        }
        i4_ret = mp4_handler_mem_confirm_fct((VOID*)pui1_buf, 
                                             (SIZE_T)ui8_size, 
                                             &t_format);
        _mm_util_mem_free(pui1_buf);
    }
    
    do
    {
        if(i4_ret != MFMTRECGR_OK)
        {
            x_dbg_stmt("File could not be recognized, i4_ret=%d\r\n", i4_ret);
            i4_ret =  CLIR_CMD_EXEC_ERROR;
            break;
        }
        x_dbg_stmt("Media Format is MP4!\r\n");
    }while(0);
    x_fm_close(h_file); 
    return i4_ret;

#else
     return CLIR_OK;
#endif

}

#endif

/*-----------------------------------------------------------------------------
 * Name: mp4_mfmtrecg_cli_init
 *
 * Description: This API initializes mfmtrecg CLI component.
 *
 * Inputs:  ppt_command_array
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 mp4_mfmtrecg_cli_init(CLI_EXEC_T** ppt_command_array)
{
#ifdef CLI_LVL_ALL

    *ppt_command_array = at_mp4_mfmtrecg_cmd_tbl;
   
#endif

return 0;
}

#else

INT32 mp4_mfmtrecg_cli_init(CLI_EXEC_T** ppt_command_array)
{
    return 0;
}
#endif/*end of #if (ENABLE_MM_CLI_TEST == 1)*/


