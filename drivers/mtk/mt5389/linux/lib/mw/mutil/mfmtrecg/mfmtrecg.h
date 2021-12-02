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
 * $RCSfile: mfmtrecg.h,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/9 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 907020e011d0dbd7ca156869d249b742 $ Asa
 *
 * Description: 
 *         This file exports all media format recognizer API and constant for 
 *         other module using.
 *---------------------------------------------------------------------------*/

#ifndef _MFMTRECG_H_
#define _MFMTRECG_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/u_common.h"
#include "handle/u_handle.h"
#include "u_mfmtrecg.h"
#include "util/x_lnk_list.h"
#include "mutil/mm_util_memory.h"
#include "mfmtrecg_dbg.h"
#include "cli/u_cli.h"

#else

#include "u_common.h"
#include "u_handle.h"
#include "u_mfmtrecg.h"
#include "x_lnk_list.h"
#include "mfmtrecg_dbg.h"
#include "u_cli.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_HANDLER_NAME_LEN 16
#define MAX_EXTENSION_NAME_LEN 16
#define FILE_EXTENSION_END_CODE "ext_list_end"



typedef struct _MFMREC_FILE_EXT_T
{  
    CHAR*  ps_file_ext;
    SLIST_ENTRY_T(_MFMREC_FILE_EXT_T) t_file_ext_list; 
} MFMREC_FILE_EXT_T;



/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/* This function is the comfirm function of a file  */
typedef INT32 (*handler_file_confirm_fct) ( HANDLE_T h_file, MEDIA_FORMAT_T* pt_format);

/* This function is the comfirm function of a memory buffer  */
typedef INT32 (*handler_mem_confirm_fct) (const VOID* pv_mem_data, SIZE_T z_mem_leng, MEDIA_FORMAT_T* pt_format);

/* This function execute cli cmd */
typedef INT32 (*handler_mfmtrecg_cli_int_fct) (CLI_EXEC_T **ppt_command_array); 

/* This structure holds the function table for recognizer registration. */
typedef struct _MFMTRECG_HANDLER_FCT_TBL_T
{
    handler_file_confirm_fct    pf_fconfirm;
    handler_mem_confirm_fct     pf_mconfirm;
    handler_mfmtrecg_cli_int_fct    pf_mfmtrecg_cli_init;
} MFMTRECG_HANDLER_FCT_TBL_T;
 
/* This structure holds the register table for recognizer registration. */
typedef struct _MFMTRECG_HANDLER_REG_T
{
    MFMTRECG_HANDLER_FCT_TBL_T                  t_handler_fct_tbl;
    CHAR                                                           s_handler_name[MAX_HANDLER_NAME_LEN];
    SLIST_T(_MFMREC_FILE_EXT_T)                     t_favor_list;                                        
    SLIST_ENTRY_T(_MFMTRECG_HANDLER_REG_T)      t_link;
} MFMTRECG_HANDLER_REG_T;

/* This function is a selector's API. This function is called by system initial procedure when system boot-up. register a handler to the selector */
extern INT32 mfmtrecg_register_handler (  
                            const CHAR*                          ps_name,        /* (I) specifies sixteen characters of a handler name, and the character is case sensitive.  */
                            const CHAR**                        ps_favor,       /* (I) specify the favor file extension string list, for example "mp3"  */
                            const MFMTRECG_HANDLER_FCT_TBL_T*    pt_fct_tbl);    /* (I) specify the function table of the engine. */
                            
/* This function is a selector's API. This function is called by system initial procedure when system boot-up to init media format recognizer module. */
extern INT32 mfmtrecg_init (void);

/* this function will ask favor handler which match extension and favor */
extern INT32 mfmtrecg_ask_handler_in_favor_by_file(
                            HANDLE_T                    h_file,         /* (I) specify the handle of the file */
                            const CHAR*                 ps_extension,   /* (I) specify extension of the file */
                            MEDIA_FORMAT_T*             pt_format);     /* (O) retrieve the format of the file */

/* this function will ask all handler */
extern INT32 mfmtrecg_ask_all_handler_by_file(
                            HANDLE_T                    h_file,         /* (I) specify the handle of the file */
                            const CHAR*                 ps_extension,   /* (I) specify extension of the file */
                            MEDIA_FORMAT_T*             pt_format);     /* (O) retrieve the format of the file */

/* this function will ask all handler that what format the memory buffer is */
extern INT32 mfmtrecg_ask_all_handler_by_mem(
                            const VOID*                 pv_mem_data,    /* (I) specify the address of memory buffer which want to retrieve the format. */
                            SIZE_T                      z_mem_leng,     /* (I) specify the length of memory buffer which want to retrieve the format.  */
                            MEDIA_FORMAT_T*             pt_format);     /* (O) retrieve the format of the file */

extern INT32 mfmtrecg_ask_handler_in_favor_by_mem_with_fmt(
                        const VOID*       pv_mem_data, 
                        SIZE_T            z_mem_leng,   
                        MEDIA_FORMAT_T*   pt_format); 

#ifdef __cplusplus
}
#endif

#endif /* _MFMTRECG_H_ */
