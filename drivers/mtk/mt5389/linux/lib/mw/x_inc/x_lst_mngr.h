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
 * $RCSfile: x_lst_mngr.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/20 $
 * $SWAuthor: Pedro Tsai $
 * $MD5HEX: aa3109d410702d35a82d86c6386592c7 $
 *
 * Description:
 *         This header file contains Large Object list related API definitions,
 *         which are exported to other Middleware components.
 *
 *---------------------------------------------------------------------------*/

#ifndef _X_LST_MNGR_H_
#define _X_LST_MNGR_H_

#include "u_lst_mngr.h"

extern INT32 x_lst_mngr_init_list_id_by_type(UINT16            ui2_lst_id,
                                             LST_MNGR_LST_TYPE_T e_type);

extern INT32 x_lst_mngr_deinit_list_id_by_type(UINT16            ui2_lst_id,
                                               LST_MNGR_LST_TYPE_T e_type);

extern INT32 x_lst_mngr_init_list_id(UINT16            ui2_lst_id,
                                     UINT32            ui4_lst_mask);

extern INT32 x_lst_mngr_deinit_list_id(UINT16            ui2_lst_id,
                                       UINT32            ui4_lst_mask,
                                       BOOL              b_force);

extern INT32 x_lst_mngr_fs_store(HANDLE_T          h_dir,
                                 const CHAR*       ps_lst_path,
                                 UINT16            ui2_lst_id,
                                 UINT32            ui4_lst_mask);

extern INT32 x_lst_mngr_fs_sync(HANDLE_T          h_dir,
                                const CHAR*       ps_lst_path,
                                UINT16            ui2_lst_id,
                                UINT32            ui4_lst_mask);

extern INT32 x_lst_mngr_reg_dual_bank(HANDLE_T             h_dir,
                                      const CHAR*          ps_main_path,
                                      const CHAR*          ps_sub_path,
                                      x_lst_mngr_get_fct   pf_get,
                                      x_lst_mngr_set_fct   pf_set);
#ifdef LINUX_TURNKEY_SOLUTION
extern INT32 lst_mngr_reg_dual_bank(HANDLE_T             h_dir,
                                    const CHAR*          ps_main_path,
                                    const CHAR*          ps_sub_path,
                                    lst_mngr_get_fct     pf_get,
                                    lst_mngr_set_fct     pf_set,
                                    VOID*                pv_get_tag,
                                    VOID*                pv_set_tag);
#endif

extern INT32 x_lst_mngr_dual_bank_fs_store(UINT16            ui2_lst_id,
                                           UINT32            ui4_lst_mask,
                                           LST_MNGR_DUAL_BANK_AS_MODE_T e_as_mode);

extern INT32 x_lst_mngr_dual_bank_fs_sync(UINT16            ui2_lst_id,
                                          UINT32            ui4_lst_mask);

extern UINT8 x_lst_mngr_get_dual_bank_status(VOID);

extern VOID  x_lst_mngr_reset_dual_bank_status(VOID);

extern INT32 x_lst_mngr_reg_default_db(UINT16              ui2_lst_id,
                                       UINT32              ui4_lst_mask,
                                       HANDLE_T            h_dir,
                                       const CHAR*         ps_def_db_path,
                                       UINT16              ui2_def_db_lst_id);

extern INT32 x_lst_mngr_load_default_db(UINT16            ui2_lst_id,
                                        UINT32            ui4_lst_mask,
                                        BOOL              b_clean_list);

extern INT32 x_lst_mngr_dual_bank_fs_sync_with_fallback(UINT16  ui2_lst_id,
                                                        UINT32  ui4_lst_mask);

extern INT32 x_lst_mngr_swap_svl_rec(HANDLE_T          h_svl,
                                     UINT16            ui2_svl_rec_id_A,
                                     UINT16            ui2_svl_rec_id_B);

extern INT32 x_lst_mngr_copy_svl_rec(HANDLE_T          h_svl,
                                     UINT16            ui2_svl_rec_id,
                                     UINT16            ui2_new_rec_id,
                                     UINT32            ui4_new_channel_id);

extern INT32 x_lst_mngr_clean_svl(HANDLE_T             h_svl,
                                  LST_MNGR_CLEAN_T*    pt_clean);

extern INT32 x_lst_mngr_clean_tsl(HANDLE_T             h_tsl,
                                  LST_MNGR_CLEAN_T*    pt_clean);

extern INT32 x_lst_mngr_clean_satl_ch(
              UINT16  ui2_satl_id,
              UINT16  ui2_satl_rec_id,
              BOOL    b_clean_tsl);

#endif /* _X_LST_MNGR_H_ */


