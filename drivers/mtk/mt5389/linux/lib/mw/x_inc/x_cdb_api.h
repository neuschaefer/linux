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
 * $RCSfile: x_cdb_api.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains core database related API definitions,
 *         which are exported to other Middleware components.
 *         
 *---------------------------------------------------------------------------*/

#ifndef _X_CDB_API_H_
#define _X_CDB_API_H_

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/
#include "x_common.h"
#include "handle.h"
#include "u_cdb.h"
/*-------------------------------------------------------------------------
                    functions declarations
 --------------------------------------------------------------------------*/

extern INT32 x_db_create
(
    UINT16            ui2_max_record,
    DB_MEM_POLICY_T   e_memory_policy,
    CHAR*             s_db_name,
    UINT8             ui1_count,
    DB_FIELD_INFO_T   at_field_info[],
    UINT8			  ui1_key_field_count,
    DB_FIELD_T        aui4_key_field_types[],
    VOID*             pv_tag,
    db_notify_fct     pf_nfy,
    HANDLE_T*         ph_db_obj
);


extern INT32 x_db_delete
(
    HANDLE_T          h_client,
    CHAR*             s_db_name
);

extern INT32 x_db_clean
(
    HANDLE_T          h_client
);

extern INT32 x_db_get_handle_by_name
(
    CHAR*             s_db_name,
    VOID*             pv_tag,
    db_notify_fct     pf_nfy,
    HANDLE_T*         ph_client
);

extern INT32 x_db_read_rec
(
    HANDLE_T          h_client,
    UINT8		      ui1_keyfld_count,
    const DB_FIELD_T  aui4_keyfld_types[],
    const VOID*       apv_keyfld_addrs[],
    const DB_SEARCH_T at_keyfld_search_opts[],
    UINT8             ui1_count,
    const DB_FIELD_T  aui4_fld_type_selected[],
    VOID*             apv_fld_value_buffer[],
    UINT16            aui2_buffer_len[],
    UINT16            ui2_record_index,
    UINT32*           pui4_db_version
);

extern INT32 x_db_update_rec
(
    HANDLE_T          h_client,
    const VOID*       apv_key_field_values[],
    UINT8             ui1_fld_count,
    const DB_FIELD_T  aui4_fld_type_selected[],
    const VOID*       apv_fld_buffer[],
    const UINT16      aui2_buffer_len[]
);

extern INT32 x_db_write_rec
(
    HANDLE_T          h_client,
    const VOID*       apv_key_field_values[],
    const VOID*       apv_field_values[],
    const UINT16      aui2_buffer_len[],
    DB_CTRL_FLAG_T    ui1_write_mode
);

extern INT32 x_db_delete_rec
(
    HANDLE_T          h_client,
    UINT8		      ui1_key_field_count,
    const DB_FIELD_T  aui4_field_types[],  
    const VOID*       apv_field_values[],     
    UINT16		      ui2_record_index 
);

extern INT32 x_db_write_lock
(
    HANDLE_T h_client
);

extern INT32 x_db_write_unlock
(
    HANDLE_T h_client
);

extern INT32 x_db_read_lock
(
    HANDLE_T h_client
);

extern INT32 x_db_read_unlock
(
    HANDLE_T h_client
);

#endif /* _X_CDB_API_H_ */
