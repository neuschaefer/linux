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
 * $RCSfile: x_nwl.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/20 $
 * $SWAuthor: Pedro Tsai $
 * $MD5HEX: aa3109d410702d35a82d86c6386592c7 $
 *
 * Description:
 *         This header file contains Network list related API definitions,
 *         which are exported to other Middleware components.
 *
 *---------------------------------------------------------------------------*/

#ifndef _X_NWL_H_
#define _X_NWL_H_

#include "u_nwl.h"

extern INT32 x_nwl_create            (const CHAR*      s_nwl_name,
                                      UINT16           ui2_nwl_id,
                                      NWL_MEM_POLICY_T e_mem_policy,
                                      UINT16           ui2_max_nb_rec,
                                      VOID*            pv_tag,
                                      x_nwl_nfy_fct    pf_nfy,
                                      HANDLE_T*        ph_nwl);

extern  INT32 x_nwl_delete           (HANDLE_T    	   h_nwl);

extern  INT32 x_nwl_open             (UINT16           ui2_nwl_id,
                                      VOID*            pv_tag,
                                      x_nwl_nfy_fct    pf_nfy,
                                      HANDLE_T*        ph_nwl);

extern INT32 x_nwl_close             (HANDLE_T         h_nwl);

                                      
extern INT32 x_nwl_clean             (HANDLE_T         h_nwl);

extern INT32 x_nwl_add_rec           (HANDLE_T         h_nwl,
                                      const NWL_REC_T* pt_nwl_rec);

extern INT32 x_nwl_del_rec           (HANDLE_T         h_nwl,
                                      UINT16           ui2_nwl_rec_id);

extern INT32 x_nwl_update_rec        (HANDLE_T         h_nwl,
                                      const NWL_REC_T* pt_nwl_rec,
                                      BOOL             b_must_exist);

extern INT32 x_nwl_qry_rec_id        (HANDLE_T         h_nwl,
                                      NWL_REC_ID_QRY_T e_nwl_qry_option,
                                      UINT16*          pui2_rec_id);
     
extern INT32 x_nwl_lock              (HANDLE_T         h_nwl);

extern INT32 x_nwl_unlock            (HANDLE_T         h_nwl);

extern INT32 x_nwl_read_lock         (HANDLE_T         h_nwl);

extern INT32 x_nwl_read_unlock       (HANDLE_T         h_nwl);

extern INT32 x_nwl_get_rec           (HANDLE_T         h_nwl,
                                      UINT16           ui2_nwl_rec_id,
                                      NWL_REC_T*       pt_nwl_rec,
                                      UINT32*          pui4_ver_id);

extern INT32 x_nwl_get_num_rec       (HANDLE_T            h_nwl,
                                      UINT16*             pui2_num_recs,
                                      UINT32*             pui4_ver_id);

extern INT32 x_nwl_get_rec_by_rec_idx(HANDLE_T      h_nwl,
                                      UINT16        ui2_idx,
                                      NWL_REC_T*    pt_nwl_rec,
                                      UINT32*       pui4_ver_id);

extern INT32 x_nwl_get_rec_by_nw_id  (HANDLE_T         h_nwl,
                                      UINT16           ui2_network_id,
                                      UINT16		   ui2_idx,
                                      NWL_REC_T*       pt_nwl_rec,
                                      UINT32*          pui4_ver_id);

extern INT32 x_nwl_get_num_rec_by_nw_id(HANDLE_T            h_nwl,
                                        UINT16              ui2_network_id,
                                        UINT16*             pui2_num_recs,
                                        UINT32*             pui4_ver_id);

extern INT32 x_nwl_get_rec_by_sat    (HANDLE_T         h_nwl,
                                      UINT16           ui2_satl_id,
                                      UINT16           ui2_satl_rec_id,
                                      UINT16		   ui2_idx,
                                      NWL_REC_T*       pt_nwl_rec,
                                      UINT32*          pui4_ver_id);

extern INT32 x_nwl_get_num_rec_by_sat  (HANDLE_T            h_nwl,
                                        UINT16              ui2_satl_id,
                                        UINT16              ui2_satl_rec_id,
                                        UINT16*             pui2_num_recs,
                                        UINT32*             pui4_ver_id);

extern INT32 x_nwl_rec_init          (NWL_REC_T*        pt_nwl_rec);

/********************************************
   NWL data persistent storage related APIs
*********************************************/

extern INT32 x_nwl_fs_load           (HANDLE_T            h_dir,
                                      const CHAR*         ps_nwl_path,
                                      UINT16              ui2_nwl_id,
                                      const CHAR*	      ps_nwl_name,
                                      VOID*               pv_tag,
                                      x_nwl_nfy_fct       pf_nfy,
                                      HANDLE_T*           ph_nwl);

extern  INT32 x_nwl_fs_store         (HANDLE_T            h_dir,
                                      const CHAR*         ps_nwl_path,
                                      UINT16              ui2_nwl_id);
	

extern  INT32 x_nwl_fs_del           (HANDLE_T            h_dir,
                                      const CHAR*         ps_nwl_path,
                                      UINT16              ui2_nwl_id);



extern  INT32 x_nwl_fs_qry           (HANDLE_T        h_dir,
                                      const CHAR*     ps_nwl_path,
                                      UINT16          ui2_idx,
                                      UINT16*         pui2_nwl_id,
                                      CHAR*           ps_nwl_name);

extern  INT32 x_nwl_fs_sync          (HANDLE_T        h_nwl,
                                      HANDLE_T        h_dir,
                                      const CHAR*     ps_nwl_path,
                                      UINT16          ui2_nwl_id);

extern INT32 x_nwl_init_mem_dump     (NWL_MEM_DUMP_T*        pt_mem_dump);

extern INT32 x_nwl_free_mem_dump     (NWL_MEM_DUMP_T*        pt_mem_dump);

extern INT32 x_nwl_dump_to_mem       (HANDLE_T               h_nwl,
                                      NWL_MEM_DUMP_T*        pt_mem_dump);

extern INT32 x_nwl_load_from_mem     (HANDLE_T               h_nwl,
                                      NWL_MEM_DUMP_T*        pt_mem_dump);

extern INT32 x_nwl_fs_store_given_file_handle
(
    HANDLE_T        h_file,
    UINT16          ui2_nwl_id  
);

extern
INT32 x_nwl_fs_sync_given_file_handle
(
    HANDLE_T		h_nwl,	
    HANDLE_T        h_file,
    UINT16		    ui2_nwl_id
);
/******************************************************
Utility function to get information about Network list
*****************************************************/
INT32 x_nwl_get_info                 (HANDLE_T        h_nwl);


#endif /* _X_NWL_H_ */


