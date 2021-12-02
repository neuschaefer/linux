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
 * $RCSfile:$
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $SWAuthor: Jun Zuo  $
 *
 * Description: 
 *    This  file contains implementation of the lst_mngr's application API's. 
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/
#ifndef NO_LST_MNGR_INIT
#include "lst_mngr/x_lst_mngr.h"
#include "handle/handle.h"

#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"

#define LST_MNGRR_FAIL LSTMR_FAIL
/*-------------------------------------------------------------------------
                    functions implementations
 --------------------------------------------------------------------------*/
INT32 c_lst_mngr_init_list_id_by_type(UINT16 ui2_lst_id,
                                      LST_MNGR_LST_TYPE_T e_type)
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_lst_id);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_type);

    RPC_DO_OP("x_lst_mngr_init_list_id_by_type");

    RPC_RETURN(ARG_TYPE_INT32, LST_MNGRR_FAIL);
}

INT32 c_lst_mngr_deinit_list_id_by_type(UINT16 ui2_lst_id,
                                        LST_MNGR_LST_TYPE_T e_type)
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_lst_id);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_type);

    RPC_DO_OP("x_lst_mngr_deinit_list_id_by_type");

    RPC_RETURN(ARG_TYPE_INT32, LST_MNGRR_FAIL);
}

INT32 c_lst_mngr_init_list_id(UINT16 ui2_lst_id,
                              UINT32 ui4_lst_mask)
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_lst_id);
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_lst_mask);

    RPC_DO_OP("x_lst_mngr_init_list_id");

    RPC_RETURN(ARG_TYPE_INT32, LST_MNGRR_FAIL);
}

INT32 c_lst_mngr_deinit_list_id(UINT16 ui2_lst_id,
                                UINT32 ui4_lst_mask,
                                BOOL b_force)
{
    RPC_DECL(3, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_lst_id);
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_lst_mask);
    RPC_ARG_INP(ARG_TYPE_BOOL, b_force);

    RPC_DO_OP("x_lst_mngr_deinit_list_id");

    RPC_RETURN(ARG_TYPE_INT32, LST_MNGRR_FAIL);
}

INT32 c_lst_mngr_fs_store(HANDLE_T h_dir,
                          const CHAR * ps_lst_path,
                          UINT16 ui2_lst_id,
                          UINT32 ui4_lst_mask)
{
    RPC_DECL(4, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_dir);
    RPC_ARG_INP(ARG_TYPE_REF_STR, ps_lst_path);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_lst_id);
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_lst_mask);

    RPC_DO_OP("x_lst_mngr_fs_store");

    RPC_RETURN(ARG_TYPE_INT32, LST_MNGRR_FAIL);
}

INT32 c_lst_mngr_fs_sync(HANDLE_T h_dir,
                         const CHAR * ps_lst_path,
                         UINT16 ui2_lst_id,
                         UINT32 ui4_lst_mask)
{
    RPC_DECL(4, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_dir);
    RPC_ARG_INP(ARG_TYPE_REF_STR, ps_lst_path);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_lst_id);
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_lst_mask);

    RPC_DO_OP("x_lst_mngr_fs_sync");

    RPC_RETURN(ARG_TYPE_INT32, LST_MNGRR_FAIL);
}

INT32 _x_lst_mngr_reg_dual_bank(HANDLE_T h_dir,
                                const CHAR * ps_main_path,
                                const CHAR * ps_sub_path,
                                x_lst_mngr_get_fct pf_get,
                                x_lst_mngr_set_fct pf_set)
{
    RPC_DECL(5, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_dir);
    RPC_ARG_INP(ARG_TYPE_REF_STR, ps_main_path);
    RPC_ARG_INP(ARG_TYPE_REF_STR, ps_sub_path);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_get);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_set);

    RPC_DO_OP("x_lst_mngr_reg_dual_bank");

    RPC_RETURN(ARG_TYPE_INT32, LST_MNGRR_FAIL);
}

INT32 _hndlr_x_lst_mngr_get_fct(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        VOID *       pv_cb_addr,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type = ARG_TYPE_INT32;

    pt_return->u.i4_arg = ((x_lst_mngr_get_fct)pv_cb_addr)(
            pt_args[0].u.pui1_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_lst_mngr_set_fct(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        VOID *       pv_cb_addr,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type = ARG_TYPE_INT32;

    pt_return->u.i4_arg = ((x_lst_mngr_set_fct)pv_cb_addr)(
            pt_args[0].u.ui1_arg);

    return RPCR_OK;
}

INT32 c_lst_mngr_reg_dual_bank(HANDLE_T h_dir,
                               const CHAR * ps_main_path,
                               const CHAR * ps_sub_path,
                               x_lst_mngr_get_fct pf_get,
                               x_lst_mngr_set_fct pf_set)
{
    return _x_lst_mngr_reg_dual_bank(h_dir, ps_main_path, ps_sub_path,
                                     pf_get, pf_set);
}

INT32 c_lst_mngr_dual_bank_fs_store(UINT16 ui2_lst_id,
                                    UINT32 ui4_lst_mask,
                                    LST_MNGR_DUAL_BANK_AS_MODE_T e_as_mode)
{
    RPC_DECL(3, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_lst_id);
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_lst_mask);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_as_mode);

    RPC_DO_OP("x_lst_mngr_dual_bank_fs_store");

    RPC_RETURN(ARG_TYPE_INT32, LST_MNGRR_FAIL);
}

INT32 c_lst_mngr_dual_bank_fs_sync(UINT16 ui2_lst_id,
                                   UINT32 ui4_lst_mask)
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_lst_id);
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_lst_mask);

    RPC_DO_OP("x_lst_mngr_dual_bank_fs_sync");

    RPC_RETURN(ARG_TYPE_INT32, LST_MNGRR_FAIL);
}

UINT8 c_lst_mngr_get_dual_bank_status()
{
    RPC_DECL(0, UINT8);

    RPC_DO_OP("x_lst_mngr_get_dual_bank_status");

    RPC_RETURN(ARG_TYPE_UINT8, LST_MNGRR_FAIL);
}

VOID c_lst_mngr_reset_dual_bank_status()
{
    RPC_DECL_VOID(0);

    RPC_DO_OP("x_lst_mngr_reset_dual_bank_status");

    RPC_RETURN_VOID;
}

INT32 c_lst_mngr_reg_default_db(UINT16 ui2_lst_id,
                                UINT32 ui4_lst_mask,
                                HANDLE_T h_dir,
                                const CHAR * ps_def_db_path,
                                UINT16 ui2_def_db_lst_id)
{
    RPC_DECL(5, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_lst_id);
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_lst_mask);
    RPC_ARG_INP(ARG_TYPE_UINT32, h_dir);
    RPC_ARG_INP(ARG_TYPE_REF_STR, ps_def_db_path);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_def_db_lst_id);

    RPC_DO_OP("x_lst_mngr_reg_default_db");

    RPC_RETURN(ARG_TYPE_INT32, LST_MNGRR_FAIL);
}

INT32 c_lst_mngr_load_default_db(UINT16 ui2_lst_id,
                                 UINT32 ui4_lst_mask,
                                 BOOL   b_clean_list)
{
    RPC_DECL(3, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_lst_id);
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_lst_mask);
    RPC_ARG_INP(ARG_TYPE_BOOL,   b_clean_list);

    RPC_DO_OP("x_lst_mngr_load_default_db");

    RPC_RETURN(ARG_TYPE_INT32, LST_MNGRR_FAIL);
}

INT32 c_lst_mngr_dual_bank_fs_sync_with_fallback(UINT16 ui2_lst_id,
                                                 UINT32 ui4_lst_mask)
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_lst_id);
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_lst_mask);

    RPC_DO_OP("x_lst_mngr_dual_bank_fs_sync_with_fallback");

    RPC_RETURN(ARG_TYPE_INT32, LST_MNGRR_FAIL);
}

INT32 c_lst_mngr_swap_svl_rec(HANDLE_T h_svl,
                              UINT16 ui2_svl_rec_id_A,
                              UINT16 ui2_svl_rec_id_B)
{
    RPC_DECL(3, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_svl);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_svl_rec_id_A);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_svl_rec_id_B);

    RPC_DO_OP("x_lst_mngr_swap_svl_rec");

    RPC_RETURN(ARG_TYPE_INT32, LST_MNGRR_FAIL);
}

INT32 c_lst_mngr_copy_svl_rec(HANDLE_T h_svl,
                              UINT16 ui2_svl_rec_id,
                              UINT16 ui2_new_rec_id,
                              UINT32 ui4_new_channel_id)
{
    RPC_DECL(4, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_svl);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_svl_rec_id);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_new_rec_id);
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_new_channel_id);

    RPC_DO_OP("x_lst_mngr_copy_svl_rec");

    RPC_RETURN(ARG_TYPE_INT32, LST_MNGRR_FAIL);
}

INT32 c_lst_mngr_clean_svl(HANDLE_T h_svl,
                           LST_MNGR_CLEAN_T * pt_clean)
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_clean,  RPC_DESC_LST_MNGR_CLEAN_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32, h_svl);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_clean);

    RPC_DO_OP("x_lst_mngr_clean_svl");

    RPC_RETURN(ARG_TYPE_INT32, LST_MNGRR_FAIL);
}

INT32 c_lst_mngr_clean_tsl(HANDLE_T h_tsl,
                           LST_MNGR_CLEAN_T * pt_clean)
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_clean,  RPC_DESC_LST_MNGR_CLEAN_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32, h_tsl);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_clean);

    RPC_DO_OP("x_lst_mngr_clean_tsl");

    RPC_RETURN(ARG_TYPE_INT32, LST_MNGRR_FAIL);
}

INT32 c_rpc_reg_lst_mngr_cb_hndlrs()
{
    RPC_REG_CB_HNDLR(x_lst_mngr_get_fct);
    RPC_REG_CB_HNDLR(x_lst_mngr_set_fct);

    return RPCR_OK;
}
#endif
