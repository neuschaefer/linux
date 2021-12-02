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
 * $RCSfile: x_mdb.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/1 $
 * $SWAuthor: Yuchien Chen $
 * $MD5HEX: 34076dbf72c1e756a741024f89fac47f $
 *
 * Description: 
 *         This file contains File Manager exported API's.
 *---------------------------------------------------------------------------*/
 
#ifndef _X_MDB_H_
#define _X_MDB_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#ifdef _WIN32_
#include "x_os.h"
#else
#include "x_os.h"
#endif
#include "x_common.h"
#include "common.h"
#include "u_mtp.h"
#include "u_mdb.h"
#include "u_dm.h"
#include "x_dm.h"

/*-----------------------------------------------------------------------------
                    external functions declarations
 ----------------------------------------------------------------------------*/  
extern INT32 x_mdb_init(VOID);

/*******************
  Virtual Tree Define related API
 *******************/ 
extern INT32 x_mdb_start_define_tree(VOID** ppv_tree);

extern INT32 x_mdb_config_category(VOID*               pv_tree,
                                   UINT8               ui1_category_idx,
                                   UINT32              ui4_max_item,
                                   UINT32              ui4_format_num,
                                   UINT16*             aui2_formats);

extern INT32 x_mdb_add_tree_node(VOID*               pv_tree,
                                 MDB_LAYOUT_NODE_T*  pt_node, 
                                 MDB_NODE_HND_T      t_parent,
                                 MDB_NODE_HND_T*     pt_new_node);
                            
extern INT32 x_mdb_finish_define_tree(VOID*    pv_tree);

extern INT32 x_mdb_virtual_tree_free(VOID*    pv_tree);

/*******************
  MTP database builder related API
 *******************/    
extern INT32 x_mdb_open(DM_DEV_STAT_T*  pt_cfg,
                        HANDLE_T*       ph_mdb);

extern INT32 x_mdb_register_async(HANDLE_T              h_mdb,
                                  x_mdb_async_fct       pf_async);

extern INT32 x_mdb_deregister_async(HANDLE_T            h_mdb,
                                    x_mdb_async_fct     pf_async);

extern INT32 x_mdb_config(HANDLE_T          h_mdb,
                          MDB_CONFIG_T*     pt_cfg);

extern INT32 x_mdb_close(HANDLE_T h_mdb);

extern INT32 x_mdb_status(HANDLE_T         h_mdb,
                          MDB_STATUS_T*    pe_status);

extern INT32 x_mdb_start(HANDLE_T     h_mdb);

extern INT32 x_mdb_pause(HANDLE_T     h_mdb);

extern INT32 x_mdb_resume(HANDLE_T    h_mdb);

extern INT32 x_mdb_percentage(HANDLE_T    h_mdb,
                              UINT8*      pui1_percentage);

extern INT32 x_mdb_query(HANDLE_T           h_mdb,
                         MDB_ITEM_HND_T*    at_item, 
                         UINT32             ui4_offset,
                         UINT32             ui4_query_num,
                         UINT32*            pui4_cnt, 
                         MDB_ITEM_INFO_T*   pt_item_info_list);

extern INT32 x_mdb_obj_prop_size(HANDLE_T           h_mdb,
                                 MDB_OBJ_HND_T      t_obj, 
                                 UINT16             ui2_prop_code,
                                 UINT32*            pui4_size);

extern INT32 x_mdb_obj_prop_value(HANDLE_T          h_mdb,
                                  MDB_OBJ_HND_T     t_obj, 
                                  UINT16            ui2_prop_code,
                                  UINT16*           pui2_data_type,
                                  UINT8*            pui1_buf);

extern INT32 x_mdb_obj_detail_info(HANDLE_T                 h_mdb,
                                   MDB_OBJ_HND_T            t_obj, 
                                   MDB_ITEM_DETAIL_INFO_T*  pt_info);


extern INT32 x_mdb_device_info(HANDLE_T             h_mdb,
                               MTP_DEVICE_INFO_T**  ppt_mdb_device_info);

extern INT32 x_mdb_storage_ids(HANDLE_T     h_mdb,
                               UINT32*      pui4_storage_num,
                               UINT32**     ppui4_storage_ids);

extern INT32 x_mdb_storage_info(HANDLE_T                h_mdb,
                                UINT32                  ui4_storage_id,
                                MTP_STORAGE_INFO_T**    ppt_mdb_storage_info);

#endif /* _X_MDB_H_ */


