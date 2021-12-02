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
 * $RCSfile: midxbuld.h,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/9 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 907020e011d0dbd7ca156869d249b742 $ Asa
 *
 * Description: 
 *         This file exports all media index builder API and constant for 
 *         other module using.
 *---------------------------------------------------------------------------*/

#ifndef _MIDXBULD_H_
#define _MIDXBULD_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/u_common.h"
#include "handle/u_handle.h"
#include "midxbuld_dbg.h"
#include "mutil/mm_util_memory.h"
#include "mutil/mfmtrecg/x_mfmtrecg.h"
#include "mutil/midxbuld/x_midxbuld.h"
#include "util/x_lnk_list.h"
#include "mutil/mm_util_input.h"
#include "cli/u_cli.h"

#else

#include "u_common.h"
#include "u_handle.h"
#include "midxbuld_dbg.h"
#include "x_dbg.h"
#include "../mfmtrecg/x_mfmtrecg.h"
#include "../mm_util_input.h"
#include "x_midxbuld.h"
#include "x_lnk_list.h"
#include "x_mm_common.h"
#include "u_cli.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_HANDLER_NAME_LEN 16

#define HT_MIDXBULD_FILE_TYPE      (HT_GROUP_MIDXBULD)
#define HT_MIDXBULD_MEM_TYPE       (HT_GROUP_MIDXBULD+1)
#define HT_MIDXBULD_RANGE_TYPE     (HT_GROUP_MIDXBULD+2)
#define HT_MIDXBULD_TBL_TYPE       (HT_GROUP_MIDXBULD+3)


typedef enum
{
    MIDXBULD_MSG_NOTIFY_TYPE = 0,
    MIDXBULD_MSG_ASYNC_EXEC_TYPE
}   MIDXBULD_MSG_TYPE_T;

#if 0
typedef UINT32 (*copybytes_fct)   (VOID* pv_midxbuld_obj, VOID* pv_buf, SIZE_T z_buf_leng, SIZE_T z_copy_size);
typedef UINT32 (*input4bytes_fct) (VOID* pv_midxbuld_obj);
typedef UINT32 (*input3bytes_fct) (VOID* pv_midxbuld_obj);
typedef UINT8  (*input1bytes_fct) (VOID* pv_midxbuld_obj);
typedef UINT32 (*setpos_fct)      (VOID* pv_midxbuld_obj, INT64 i8_offset, UINT8 ui1_whence);
typedef UINT64 (*getpos_fct)      (VOID* pv_midxbuld_obj);
#endif

typedef struct _MIDXBULD_SRC_INPUT_FCT_TBL_T
{
    copybytes_fct       pf_copybytes;
    input4bytes_fct     pf_input4;
    input3bytes_fct     pf_input3;
    input1bytes_fct     pf_input1;
    setpos_fct          pf_set_pos;
    getpos_fct          pf_get_pos;
} MIDXBULD_SRC_INPUT_FCT_TBL_T;

/* This function open a element handler for a file */
typedef INT32 (*handler_file_open_fct) (CHAR* ps_path_name, MIDXBULD_KEY_INFO_T* pt_key_info, VOID* pv_obj);

/* This function open a element handler for a memory buffer */
typedef INT32 (*handler_mem_open_fct) (VOID* pv_mem_data, SIZE_T z_mem_leng, MIDXBULD_KEY_INFO_T* pt_key_info, VOID* pv_obj);

/* This function close a element handler */
typedef INT32 (*handler_close_fct) (VOID* pv_obj); 

/* This function parse a element handler */
typedef INT32 (*handler_start_indexing_fct) (VOID* pv_obj, UINT32* pui4_range_id); 

/* This function stop indexing of a element handler */
typedef INT32 (*handler_stop_indexing_fct) (VOID* pv_obj);

/* This function get information from a element handler */
typedef INT32 (*handler_get_info_fct) (VOID* pv_obj, MIDXBULD_GET_INFO_TYPE_T e_info_type, VOID* pv_buf, SIZE_T* pz_buf_leng); 

/* This function set information from a element handler */
typedef INT32 (*handler_set_info_fct) (VOID* pv_obj, MIDXBULD_SET_INFO_TYPE_T e_info_type, VOID* pv_buf, SIZE_T* pz_buf_leng); 

/* This function execute cli cmd */
typedef INT32 (*handler_midxbuld_cli_init_fct) (CLI_EXEC_T **ppt_command_array); 

/* This structure holds the function table for recognizer registration. */
typedef struct _MIDXBULD_HANDLER_FCT_TBL_T
{
    handler_file_open_fct           pf_fopen;
    handler_mem_open_fct            pf_mopen;
    handler_close_fct               pf_close;
    handler_start_indexing_fct      pf_start_indexing;
    handler_stop_indexing_fct       pf_stop_indexing;
    handler_get_info_fct            pf_get_info;
    handler_set_info_fct            pf_set_info;
    handler_midxbuld_cli_init_fct   pf_midxbuld_cli_init;
} MIDXBULD_HANDLER_FCT_TBL_T;

/* This structure holds msg. */
typedef struct _MIDXBULD_NOTIFY_MSG_T
{
    MIDXBULD_NFY_EVENT_TYPE_T   e_event;
    const VOID*                 pv_data1;
    const VOID*                 pv_data2;
} MIDXBULD_NOTIFY_MSG_T;

typedef enum
{
    MIDXBULD_ASYNC_EXEC_CLOSE,
    MIDXBULD_ASYNC_EXEC_START_INDEXING
}   MIDXBULD_ASYNC_EXEC_TYPE_T;

typedef struct _MIDXBULD_ASYNC_EXEC_MSG_T
{
    MIDXBULD_ASYNC_EXEC_TYPE_T    e_async_exec;
} MIDXBULD_ASYNC_EXEC_MSG_T;

typedef struct _MIDXBULD_NOTIFY_ASYNC_EXEC_MSG_T
{
    HANDLE_T                    h_midxbuld;
    MIDXBULD_MSG_TYPE_T         e_msg_type;
    union{
        MIDXBULD_NOTIFY_MSG_T          t_nfy_msg;
        MIDXBULD_ASYNC_EXEC_MSG_T      t_async_exec_msg;
    }u;
} MIDXBULD_NOTIFY_ASYNC_EXEC_MSG_T;

/* This structure holds the obj of index builder info. */
typedef struct _MIDXBULD_OBJ_T
{
    HANDLE_T                        h_src_feeder;
    /* file or memory buffer operation */
    VOID*                           pv_buff_or_file_path_name;
    UINT64                          z_buff_leng;
    HANDLE_T                        h_file;
    INT64                           i8_buff_pos;

    MIDXBULD_HANDLER_FCT_TBL_T      t_handler_fct_tbl; 
    MIDXBULD_SRC_INPUT_FCT_TBL_T    t_input_fct_tbl;
    x_midxbuld_nfy_fct              pf_nfy;
    VOID*                           pv_tag;
    HANDLE_T                        h_midxbuld;
    MEDIA_FORMAT_T                  t_media_format;
    MIDXBULD_SOURCE_INFO_T          t_src_info;
    HANDLE_T                        h_sync_lock;
    BOOL                            b_abort_flag;

    /* handler object */
    VOID*                           pv_handler_obj;

} MIDXBULD_OBJ_T;

/* This structure holds the register table for handler registration. */
typedef struct _MIDXBULD_HANDLER_REG_T
{
    MIDXBULD_HANDLER_FCT_TBL_T                 t_handler_fct_tbl;
    CHAR                                       s_handler_name[MAX_HANDLER_NAME_LEN];
    MEDIA_FORMAT_T                             t_favor_media_format;
    SLIST_ENTRY_T(_MIDXBULD_HANDLER_REG_T)     t_link;
} MIDXBULD_HANDLER_REG_T;
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
extern INT32 midxbuld_match_handler (
                        MEDIA_FORMAT_T*             pt_format, 
                        MIDXBULD_HANDLER_FCT_TBL_T* pt_handler_fct_tbl);
                        
extern INT32 midxbuld_send_notify_msg (MIDXBULD_NOTIFY_ASYNC_EXEC_MSG_T* pt_msg);
                        
extern INT32 midxbuld_init (void);

extern INT32 midxbuld_register_handler (  
                        const CHAR*                       ps_name,    
                        const MEDIA_FORMAT_T*             pt_favor_media_format,
                        const MIDXBULD_HANDLER_FCT_TBL_T* pt_fct_tbl);


#ifdef __cplusplus
}
#endif

#endif /* _MIDXBULD_H_ */
