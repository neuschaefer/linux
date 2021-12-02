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
 * $RCSfile: ps_mfmtrecg_cli.c,v $
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
#include "mutil/mfmtrecg/u_mfmtrecg.h"
#include "mutil/mfmtrecg/mfmtrecg.h"
#include "mutil/mfmtrecg/ps_mfmtrecg/ps_mfmtrecg.h"
#include "mutil/mm_util_memory.h"
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "cli/x_cli.h"
#include "dbg/x_dbg.h"

#else

#include "x_common.h"
#include "../u_mfmtrecg.h"
#include "../mfmtrecg.h"
#include "ps_mfmtrecg.h"
#include "../../mm_util_memory.h"
#include "../../mm_util.h"
#include "x_cli.h"
#include "x_dbg.h"

#endif


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define TEST_BUF_SIZE   (100*1024)
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

#ifdef CLI_LVL_ALL
static INT32 mfmtrecg_cli_ps_fconfirm_fct(INT32 i4_argc, const CHAR** pps_argv);     
static INT32 mfmtrecg_cli_ps_mconfirm_fct(INT32 i4_argc, const CHAR** pps_argv);     

/* mfmtrecg command table */
static CLI_EXEC_T at_ps_mfmtrecg_cmd_tbl[] =
{
    {
        "ps fconfirm" ,       
        "psf",     
        mfmtrecg_cli_ps_fconfirm_fct,  
        NULL,       
        "PS format file check",                   
        CLI_SUPERVISOR
    },
    {
        "ps mconfirm" ,       
        "psm",     
        mfmtrecg_cli_ps_mconfirm_fct,  
        NULL,       
        "TS format mem check",                    
        CLI_SUPERVISOR
    },
    END_OF_CLI_CMD_TBL
};

#endif
/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: mfmtrecg_cli_ps_fconfirm_fct
 *
 * Description: This API returns the media format for file handle.
 *
 * Inputs: -command format : tsf  /mnt/usb/Mass-000/file_path
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL 
static INT32 mfmtrecg_cli_ps_fconfirm_fct(INT32 i4_argc, const CHAR** pps_argv)
{

#ifndef __NO_FM__

    HANDLE_T            h_file;
    INT32               i4_ret;
    MEDIA_FORMAT_T      t_format;
    CHAR*               ps_file_path;

    if(i4_argc >2 ||i4_argc <2 )
    {
        x_dbg_stmt("Invalid argument!\r\n");
        return CLIR_INV_ARG;
    }
    
    ps_file_path = (CHAR*)pps_argv[1];
    
    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_file_path, FM_READ_ONLY, 0777, FALSE, &h_file);
    
    do
    {
        if(i4_ret != FMR_OK)
        {
            x_dbg_stmt("File Open Failed!\r\n");
            i4_ret =  CLIR_OK;
            break;
        }
        i4_ret = ps_handler_file_confirm_fct(h_file, &t_format);
        if(i4_ret != MFMTRECGR_OK)
        {
            x_dbg_stmt("File could not be recognized, i4_ret=%d\r\n", i4_ret);
            i4_ret =  CLIR_OK;
            break;
        }
        x_dbg_stmt("Media Format is PS!\r\n");
    } while(0);
    
    x_fm_close(h_file);   

#endif    
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: mfmtrecg_cli_ps_mconfirm_fct
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
static INT32 mfmtrecg_cli_ps_mconfirm_fct(INT32 i4_argc, const CHAR** pps_argv)
{

#ifndef __NO_FM__

    HANDLE_T            h_file;
    INT32               i4_ret;
    UINT32              ui4_read;
    UINT8*              pui1_buf;
    MEDIA_FORMAT_T      t_format;
    CHAR*               ps_file_path;

    if(i4_argc >2 ||i4_argc <2 )
    {
        x_dbg_stmt("Invalid argument!\r\n");
        return CLIR_INV_ARG;
    }

    ps_file_path = (CHAR*)pps_argv[1];
    pui1_buf=NULL;
    do
    {
        i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_file_path, FM_READ_ONLY, 0777, FALSE, &h_file);
        if(i4_ret != FMR_OK)
        {
            x_dbg_stmt("File Open Failed!\r\n");
            i4_ret =  CLIR_OK;
            break;
        }

        pui1_buf = (UINT8 *)_mm_util_mem_alloc(TEST_BUF_SIZE);
        if (NULL == pui1_buf)
        {
            i4_ret =  CLIR_OK;
            break;
        }
        
        i4_ret = x_fm_read(h_file, pui1_buf, TEST_BUF_SIZE, &ui4_read);
        if (i4_ret != FMR_OK)
        {
            i4_ret = CLIR_OK;
            break;
        }

        i4_ret = ps_handler_mem_confirm_fct(pui1_buf, ui4_read, &t_format);
        if(i4_ret != MFMTRECGR_OK)
        {
            x_dbg_stmt("File could not be recognized, i4_ret=%d\r\n", i4_ret);
            i4_ret =  CLIR_OK;
            break;
        }
        x_dbg_stmt("Media Format is PS!\r\n");
    } while(0);

    if(pui1_buf)
    {
        _mm_util_mem_free(pui1_buf);
    }
    x_fm_close(h_file);
#endif    
    return CLIR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: ps_mfmtrecg_cli_init
 *
 * Description: This API initializes mfmtrecg CLI component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 ps_mfmtrecg_cli_init(CLI_EXEC_T **ppt_command_array)
{
#ifdef CLI_LVL_ALL

    *ppt_command_array = at_ps_mfmtrecg_cmd_tbl;
#endif
    return 0;
}



