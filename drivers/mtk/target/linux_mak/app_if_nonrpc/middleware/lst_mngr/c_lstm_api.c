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
 * $RCSfile: c_lstm_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file contains implementation of the lst_mngr's application API's. 
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "lst_mngr/c_lst_mngr.h"
#include "lst_mngr/x_lst_mngr.h"
#include "handle/handle.h"
#include "aee/aee.h"


/*-------------------------------------------------------------------------
                    functions implementations
 --------------------------------------------------------------------------*/

INT32 c_lst_mngr_init_list_id_by_type(UINT16            ui2_lst_id,
                                      LST_MNGR_LST_TYPE_T e_type)
{
    return x_lst_mngr_init_list_id_by_type(ui2_lst_id, e_type);
}

INT32 c_lst_mngr_deinit_list_id_by_type(UINT16            ui2_lst_id,
                                        LST_MNGR_LST_TYPE_T e_type)
{
    return x_lst_mngr_deinit_list_id_by_type(ui2_lst_id, e_type);
}

INT32 c_lst_mngr_init_list_id(UINT16            ui2_lst_id,
                              UINT32            ui4_lst_mask)
{
    return x_lst_mngr_init_list_id(ui2_lst_id, ui4_lst_mask);
}

INT32 c_lst_mngr_deinit_list_id(UINT16            ui2_lst_id,
                                UINT32            ui4_lst_mask,
                                BOOL              b_force)
{
    return x_lst_mngr_deinit_list_id(ui2_lst_id, ui4_lst_mask, b_force);
}

INT32 c_lst_mngr_fs_store(HANDLE_T          h_dir,
                          const CHAR*       ps_lst_path,
                          UINT16            ui2_lst_id,
                          UINT32            ui4_lst_mask)
{
    return x_lst_mngr_fs_store(h_dir, ps_lst_path, ui2_lst_id, ui4_lst_mask);
}

INT32 c_lst_mngr_fs_sync(HANDLE_T          h_dir,
                         const CHAR*       ps_lst_path,
                         UINT16            ui2_lst_id,
                         UINT32            ui4_lst_mask)
{
    return x_lst_mngr_fs_sync(h_dir, ps_lst_path, ui2_lst_id, ui4_lst_mask);
}

INT32 c_lst_mngr_reg_dual_bank(HANDLE_T             h_dir,
                               const CHAR*          ps_main_path,
                               const CHAR*          ps_sub_path,
                               x_lst_mngr_get_fct   pf_get,
                               x_lst_mngr_set_fct   pf_set)
{
    return x_lst_mngr_reg_dual_bank(h_dir,
                                    ps_main_path,
                                    ps_sub_path,
                                    pf_get,
                                    pf_set);
}

INT32 c_lst_mngr_dual_bank_fs_store(UINT16            ui2_lst_id,
                                    UINT32            ui4_lst_mask,
                                    LST_MNGR_DUAL_BANK_AS_MODE_T e_as_mode)
{
    return x_lst_mngr_dual_bank_fs_store(ui2_lst_id,
                                         ui4_lst_mask,
                                         e_as_mode);
}

INT32 c_lst_mngr_dual_bank_fs_sync(UINT16            ui2_lst_id,
                                   UINT32            ui4_lst_mask)
{
    return x_lst_mngr_dual_bank_fs_sync(ui2_lst_id,
                                        ui4_lst_mask);
}

UINT8 c_lst_mngr_get_dual_bank_status(VOID)
{
    return x_lst_mngr_get_dual_bank_status();
}

VOID c_lst_mngr_reset_dual_bank_status(VOID)
{
    x_lst_mngr_reset_dual_bank_status();
}

INT32 c_lst_mngr_reg_default_db(UINT16              ui2_lst_id,
                                UINT32              ui4_lst_mask,
                                HANDLE_T            h_dir,
                                const CHAR*         ps_def_db_path,
                                UINT16              ui2_def_db_lst_id)
{
    return x_lst_mngr_reg_default_db(ui2_lst_id,
                                     ui4_lst_mask,
                                     h_dir,
                                     ps_def_db_path,
                                     ui2_def_db_lst_id);
}

INT32 c_lst_mngr_load_default_db(UINT16            ui2_lst_id,
                                 UINT32            ui4_lst_mask,
                                 BOOL              b_clean_list)
{
    return x_lst_mngr_load_default_db(ui2_lst_id, ui4_lst_mask, b_clean_list);
}

INT32 c_lst_mngr_dual_bank_fs_sync_with_fallback(UINT16  ui2_lst_id,
                                                 UINT32  ui4_lst_mask)
{
    return x_lst_mngr_dual_bank_fs_sync_with_fallback(ui2_lst_id,
                                                      ui4_lst_mask);
}

INT32 c_lst_mngr_swap_svl_rec(HANDLE_T          h_svl,
                              UINT16            ui2_svl_rec_id_A,
                              UINT16            ui2_svl_rec_id_B)
{
    return x_lst_mngr_swap_svl_rec(h_svl, ui2_svl_rec_id_A, ui2_svl_rec_id_B);
}

INT32 c_lst_mngr_copy_svl_rec(HANDLE_T          h_svl,
                              UINT16            ui2_svl_rec_id,
                              UINT16            ui2_new_rec_id,
                              UINT32            ui4_new_channel_id)
{
    return x_lst_mngr_copy_svl_rec(h_svl, ui2_svl_rec_id,
                                   ui2_new_rec_id, ui4_new_channel_id);
}

INT32 c_lst_mngr_clean_svl(HANDLE_T             h_svl,
                           LST_MNGR_CLEAN_T*    pt_clean)
{
    return x_lst_mngr_clean_svl(h_svl, pt_clean);
}

INT32 c_lst_mngr_clean_tsl(HANDLE_T             h_tsl,
                           LST_MNGR_CLEAN_T*    pt_clean)
{
    return x_lst_mngr_clean_tsl(h_tsl, pt_clean);
}

INT32 c_lst_mngr_clean_satl_ch(
        UINT16  ui2_satl_id,
        UINT16  ui2_satl_rec_id,
        BOOL    b_clean_tsl)
{
    return x_lst_mngr_clean_satl_ch(ui2_satl_id, ui2_satl_rec_id, b_clean_tsl);
}
