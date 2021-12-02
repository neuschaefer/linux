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
 * $RCSfile: avi_minfo_cli.c,v $
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
#include "inc/x_common.h"
#include "mutil/minfo/u_minfo.h"
#include "mutil/minfo/minfo.h"
#include "mutil/minfo/x_minfo.h"
#include "mutil/minfo/avi_info/avi_minfo.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "cli/x_cli.h"
#include "dbg/x_dbg.h"

#else

#include "x_common.h"
#include "../u_minfo.h"
#include "../minfo.h"
#include "../../mm_util.h"
#include "avi_minfo.h"

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
static INT32 minfo_cli_avi_fopen(INT32 i4_argc, const CHAR** pps_argv);
static INT32 minfo_cli_avi_mopen(INT32 i4_argc, const CHAR** pps_argv);
static INT32 minfo_cli_avi_close(INT32 i4_argc, const CHAR** pps_argv);
static INT32 minfo_cli_avi_getinfo(INT32 i4_argc, const CHAR** pps_argv);

/* mfmtrecg command table */
static CLI_EXEC_T at_avi_minfo_cmd_tbl[] =
{
	{"fopen",     NULL,  minfo_cli_avi_fopen,        NULL,    "minfo_fopen",      CLI_SUPERVISOR},
	{"mopen",     NULL,  minfo_cli_avi_mopen,        NULL,    "minfo_mopen",      CLI_SUPERVISOR},
	{"close",     NULL,  minfo_cli_avi_close,        NULL,    "minfo_close",      CLI_SUPERVISOR},
	{"getinfo",   NULL,  minfo_cli_avi_getinfo,      NULL,    "minfo_getinfo",    CLI_SUPERVISOR},	
    END_OF_CLI_CMD_TBL
};

static HANDLE_T       gh_minfo;

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
                HANDLE_T h_minfo, 
                MINFO_NFY_EVENT_TYPE_T	    e_event,  
                const VOID*	                pv_data1, 
                const VOID*	                pv_data2, 
                const VOID*	                pv_tag)
{    
    x_dbg_stmt("minfo nfy\r\n");
}
#endif
/*-----------------------------------------------------------------------------
 * Name: minfo_cli_avi_fopen
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
static INT32 minfo_cli_avi_fopen(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    MEDIA_FORMAT_T t_format;
    
    t_format.e_media_type = MEDIA_TYPE_UNKNOWN;
    t_format.t_media_subtype.u.e_aud_subtype  = MEDIA_AUD_SUBTYPE_UNKNOWN;
    
    x_minfo_fopen("/mnt/ms/1.mp3", x_minfo_nfy, (VOID*) -1, &t_format, &gh_minfo);
    
    return MINFOR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: minfo_cli_avi_mopen
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
static INT32 minfo_cli_avi_mopen(INT32        i4_argc,
                                    const CHAR** pps_argv)
{

    return MINFOR_OK;

}
#endif

/*-----------------------------------------------------------------------------
 * Name: minfo_cli_avi_close
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
static INT32 minfo_cli_avi_close(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    x_minfo_close(gh_minfo);
    return MINFOR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: minfo_cli_avi_getinfo
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
static INT32 minfo_cli_avi_getinfo(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    /*
    INT32       i4_ret;
    SIZE_T      z_buf_leng;
    MINFO_INFO_PIC_T    t_pic;
    UTF16_T*    pwc_str;
    CHAR*       pc_str; 
    UINT32      ui4_leng;
    */

    return MINFOR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: avi_minfo_cli_init
 *
 * Description: This API initializes midxbuld CLI component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 avi_minfo_cli_init(CLI_EXEC_T **ppt_command_array)
{
#ifdef CLI_LVL_ALL

    *ppt_command_array = at_avi_minfo_cmd_tbl;

#endif
    return 0;
}



