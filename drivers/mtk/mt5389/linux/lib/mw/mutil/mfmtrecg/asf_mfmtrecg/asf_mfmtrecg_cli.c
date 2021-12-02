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
 * $RCSfile: asf_mfmtrecg_cli.c,v $
 * $Revision:
 * $Date:
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_ATSC/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 37ebbb2d76fad2e8a90655e5f2bf2c3a $
 *
 * Description:
 *         This file implements get/set debug level APIs for media format 
 *         recognizer.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/x_common.h"
#include "inc/x_mm_common.h"
#include "mutil/mfmtrecg/u_mfmtrecg.h"
#include "mutil/mfmtrecg/mfmtrecg.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#include "file_mngr/u_fm.h"
#endif

#include "cli/x_cli.h"
#include "dbg/x_dbg.h"
#include "mutil/mfmtrecg/asf_mfmtrecg/asf_mfmtrecg_cli.h"
#include "mutil/minfo/asf_info/asf_parser_common.h"
#include "asf_mfmtrecg.h"

#else

#include "x_common.h"
#include "x_mm_common.h"
#include "../u_mfmtrecg.h"
#include "../mfmtrecg.h"

#include "../../mm_util.h"

#include "x_cli.h"
#include "x_dbg.h"
#include "asf_mfmtrecg_cli.h"
#include "../../minfo/asf_info/asf_parser_common.h"
#include "asf_mfmtrecg.h"

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
 
#ifdef CLI_LVL_ALL
static INT32 mfmtrecg_cli_asf_fconfirm_fct(INT32 i4_argc, const CHAR** pps_argv);     
static INT32 mfmtrecg_cli_asf_mconfirm_fct(INT32 i4_argc, const CHAR** pps_argv);     

/* mfmtrecg command table */
static CLI_EXEC_T at_asf_mfmtrecg_cmd_tbl[] =
{
    {
        "asf fconfirm" ,                "asff",     
        mfmtrecg_cli_asf_fconfirm_fct,  NULL,       
        "ASF format file check",        CLI_SUPERVISOR
    },
    {
        "asf mconfirm" ,                "asfm",     
        mfmtrecg_cli_asf_mconfirm_fct,  NULL,       
        "ASF format mem check",         CLI_SUPERVISOR
    },
    END_OF_CLI_CMD_TBL
};

#endif
/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * Name: mfmtrecg_cli_asf_fconfirm_fct
 *
 * Description: This API returns the media format for file handle.
 *
 * Inputs: command format : asff  /mnt/usb/Mass-000/file_path
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK
 *          CLIR_INV_ARG  MINFOR_FILE_ERR
            
 ----------------------------------------------------------------------------*/
static INT32 mfmtrecg_cli_asf_fconfirm_fct(INT32 i4_argc, const CHAR** pps_argv)
{

#ifndef __NO_FM__

    CHAR*          ps_file_path;
    HANDLE_T       h_file;
    INT32          i4_ret;
    MEDIA_FORMAT_T t_format;
    
    /*two parameters*/
    if ((2 != i4_argc) || (NULL == pps_argv))
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        DBG_INFO(("usage:asff filepath\r\n"));
        return CLIR_INV_ARG;
    }
    
    /*Open the file */
    ps_file_path = (CHAR*)pps_argv[1];
    if (NULL == ps_file_path)
    {
        DBG_ERROR(("Filepath is NULL!\r\n"));
        DBG_INFO(("usage:asff filepath\r\n"));
        return CLIR_INV_ARG;
    }
    
    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_file_path, FM_READ_ONLY, 0777, 
                       FALSE, &h_file);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("File Open %s Failed(%d)!\r\n", ps_file_path, i4_ret));
        i4_ret =  CLIR_INV_ARG;
    }
    
    /*call asf confirm function*/
    i4_ret = asf_handler_file_confirm_fct(h_file, &t_format);
    if (i4_ret != MFMTRECGR_OK)
    {
        DBG_ERROR(("File could not be recognized, i4_ret=%d\r\n", 
                   i4_ret));
        i4_ret =  CLIR_INV_ARG;
    }
    
    /*print info*/
    if (MFMTRECGR_OK == i4_ret)
    {
        DBG_INFO(("Media Format is ASF!\r\n"));
    }
    else
    {
        DBG_INFO(("Fail to recognize ASF!\r\n"));
    }
    
    /*close the file*/
    x_fm_close(h_file);   

    return i4_ret;
    
#else

    return CLIR_OK;

#endif
}


/*-----------------------------------------------------------------------------
 * Name: mfmtrecg_cli_asf_mconfirm_fct
 *
 * Description: This API returns the media format for memory clip.
 *
 * Inputs: NULL
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK 
 *          CLIR_INV_ARG  CLIR_CMD_EXEC_ERROR
 ----------------------------------------------------------------------------*/
static INT32 mfmtrecg_cli_asf_mconfirm_fct(INT32 i4_argc, const CHAR** pps_argv)
{  

    INT32                i4_ret;
    MEDIA_FORMAT_T       t_format;
    CHAR*                ps_file_path;
    ASF_CLIP_OBJ*        pt_clip_obj;  
    
    /*three parameters*/
    if ((3 != i4_argc) || (NULL == pps_argv))
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        DBG_INFO(("usage:asfm filepath zero_len(0/1)"
                  "(the file will be read to a buffer fully to simulate "
                  "buffer clips)\r\n"));
        return CLIR_INV_ARG;
    }
    
    /*get filepath*/
    ps_file_path = (CHAR*)pps_argv[1];
    if (NULL == ps_file_path)
    {
        DBG_ERROR(("Filepath is NULL!\r\n"));
        DBG_INFO(("usage:asfm filepath zero_len(0/1)"
                  "(the file will be read to a buffer only once to simulate "
                  "buffer clips)\r\n"));
        return CLIR_INV_ARG;
    }
    
    /*get clip object*/
    i4_ret = _cli_asf_get_obj(ps_file_path, &pt_clip_obj);
    if (ASFR_CLIP_OK != i4_ret)
    {
        DBG_ERROR(("Fail to get clip object!\r\n"));
        return CLIR_INV_ARG;
    }

    if (0 == x_strncmp(pps_argv[2], "0", (SIZE_T)1))
    {
        /*call asf confirm function*/
        i4_ret = asf_handler_mem_confirm_fct( (const VOID*)pt_clip_obj, 
                                              0,   
                                              &t_format );
    }
    else
    {
        /*call asf confirm function*/
        i4_ret = asf_handler_mem_confirm_fct( pt_clip_obj->pv_clip_buf, 
                                              (SIZE_T)pt_clip_obj->i8_clip_len,   
                                              &t_format );
    }
    /*print info*/
    if (MFMTRECGR_OK == i4_ret)
    {
        DBG_INFO(("Media Format is ASF!\r\n"));
    }
    else
    {
        DBG_INFO(("Fail to recognize ASF!\r\n"));
    }

    /*free clip object*/
    _cli_asf_free_obj(&pt_clip_obj);
    
    return i4_ret;

}
#endif
/*-----------------------------------------------------------------------------
 * Name: asf_mfmtrecg_cli_init
 *
 * Description: This API initializes mfmtrecg CLI component.
 *
 * Inputs:  ppt_command_array
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK
 *          CLIR_INV_ARG
 ----------------------------------------------------------------------------*/
INT32 asf_mfmtrecg_cli_init(CLI_EXEC_T **ppt_command_array)
{
#ifdef CLI_LVL_ALL

    *ppt_command_array = at_asf_mfmtrecg_cmd_tbl;
    
#endif

return 0;
}



