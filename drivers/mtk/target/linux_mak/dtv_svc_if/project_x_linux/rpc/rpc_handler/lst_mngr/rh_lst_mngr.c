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
 * $RCSfile:  $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $SWAuthor: Jun Zuo  $
 *
 * Description:
 *         This file contains all lst_mngr function implementations.
 *-----------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
                    include files
 ------------------------------------------------------------------------*/
#include "lst_mngr/x_lst_mngr.h"
#include "lst_mngr/lstm_dual_bank.h"
#include "handle/handle.h"

#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"

static INT32
_x_lst_mngr_get_fct_wrapper(
        UINT8*              pui1_value,
        VOID*               pv_tag)
{
    RPC_DECL(1, INT32);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_tag;

    RPC_ARG_INP(ARG_TYPE_REF_UINT8, pui1_value);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_lst_mngr_get_fct", pt_nfy_tag->pv_cb_addr);

    RPC_RETURN(ARG_TYPE_INT32, -1);
}

static INT32
_x_lst_mngr_set_fct_wrapper(
        UINT8               ui1_value,
        VOID*               pv_tag)
{
    RPC_DECL(1, INT32);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_tag;

    RPC_ARG_INP(ARG_TYPE_UINT8,     ui1_value);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_lst_mngr_set_fct", pt_nfy_tag->pv_cb_addr);

    RPC_RETURN(ARG_TYPE_INT32, -1);
}

INT32 _hndlr_x_lst_mngr_init_list_id_by_type(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_lst_mngr_init_list_id_by_type(
            pt_args[0].u.ui2_arg,
            (LST_MNGR_LST_TYPE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_lst_mngr_deinit_list_id_by_type(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_lst_mngr_deinit_list_id_by_type(
            pt_args[0].u.ui2_arg,
            (LST_MNGR_LST_TYPE_T)pt_args[1].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_lst_mngr_init_list_id(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_lst_mngr_init_list_id(
            pt_args[0].u.ui2_arg,
            pt_args[1].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_lst_mngr_deinit_list_id(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_lst_mngr_deinit_list_id(
            pt_args[0].u.ui2_arg,
            pt_args[1].u.ui4_arg,
            pt_args[2].u.b_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_lst_mngr_fs_store(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_lst_mngr_fs_store(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.ps_str,
            pt_args[2].u.ui2_arg,
            pt_args[3].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_lst_mngr_fs_sync(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_lst_mngr_fs_sync(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.ps_str,
            pt_args[2].u.ui2_arg,
            pt_args[3].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_lst_mngr_reg_dual_bank(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    lst_mngr_get_fct    pf_get;
    lst_mngr_set_fct    pf_set;
    RPC_CB_NFY_TAG_T *  pt_get_tag;
    RPC_CB_NFY_TAG_T *  pt_set_tag;
    lst_mngr_get_fct    pf_old_get = NULL;
    lst_mngr_set_fct    pf_old_set = NULL;
    VOID *              pv_old_get_tag = NULL;
    VOID *              pv_old_set_tag = NULL;

    if (ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }

    lstm_get_dual_bank_cb_tag(&pf_old_get, &pf_old_set,
                              &pv_old_get_tag, &pv_old_set_tag);

    if(pt_args[3].u.pv_func == NULL)
    {
        pf_get      = NULL;
        pt_get_tag  = NULL;
    }
    else
    {
        pf_get      = _x_lst_mngr_get_fct_wrapper;
        pt_get_tag  = ri_create_cb_tag(t_rpc_id, &pt_args[3].u.pv_func, 1, NULL);
    }

    if(pt_args[4].u.pv_func == NULL)
    {
        pf_set      = NULL;
        pt_set_tag  = NULL;
    }
    else
    {
        pf_set      = _x_lst_mngr_set_fct_wrapper;
        pt_set_tag  = ri_create_cb_tag(t_rpc_id, &pt_args[4].u.pv_func, 1, NULL);
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = lst_mngr_reg_dual_bank(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.ps_str,
            pt_args[2].u.ps_str,
            pf_get,
            pf_set,
            pt_get_tag,
            pt_set_tag);

    if (pt_return->u.i4_arg == LSTMR_OK && 
        pf_old_get == _x_lst_mngr_get_fct_wrapper &&
        pf_old_set == _x_lst_mngr_set_fct_wrapper &&
        pt_args[0].u.ui4_arg == NULL_HANDLE &&
        pt_args[1].u.ps_str == NULL &&
        pt_args[2].u.ps_str == NULL &&
        pt_args[3].u.pv_func == NULL &&
        pt_args[4].u.pv_func == NULL) /* unregister succeeded */
    {
        if (pv_old_get_tag != NULL)
        {
            ri_free_cb_tag((RPC_CB_NFY_TAG_T *)pv_old_get_tag);
        }
        if (pv_old_set_tag != NULL)
        {
            ri_free_cb_tag((RPC_CB_NFY_TAG_T *)pv_old_set_tag);
        }
    }
    else if (pt_return->u.i4_arg != LSTMR_OK)
    {
        if (pt_get_tag != NULL)
        {
            ri_free_cb_tag(pt_get_tag);
        }
        if (pt_set_tag != NULL)
        {
            ri_free_cb_tag(pt_set_tag);
        }
    }

    return RPCR_OK;
}

INT32 _hndlr_x_lst_mngr_dual_bank_fs_store(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_lst_mngr_dual_bank_fs_store(
            pt_args[0].u.ui2_arg,
            pt_args[1].u.ui4_arg,
            (LST_MNGR_DUAL_BANK_AS_MODE_T)pt_args[2].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_lst_mngr_dual_bank_fs_sync(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_lst_mngr_dual_bank_fs_sync(
            pt_args[0].u.ui2_arg,
            pt_args[1].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_lst_mngr_get_dual_bank_status(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 0)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_UINT8;
    pt_return->u.ui1_arg= x_lst_mngr_get_dual_bank_status();

    return RPCR_OK;
}

INT32 _hndlr_x_lst_mngr_reset_dual_bank_status(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 0)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_VOID;
    x_lst_mngr_reset_dual_bank_status();

    return RPCR_OK;
}

INT32 _hndlr_x_lst_mngr_reg_default_db(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_lst_mngr_reg_default_db(
            pt_args[0].u.ui2_arg,
            pt_args[1].u.ui4_arg,
            pt_args[2].u.ui4_arg,
            pt_args[3].u.ps_str,
            pt_args[4].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_lst_mngr_load_default_db(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_lst_mngr_load_default_db(
            pt_args[0].u.ui2_arg,
            pt_args[1].u.ui4_arg,
            pt_args[2].u.b_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_lst_mngr_dual_bank_fs_sync_with_fallback(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_lst_mngr_dual_bank_fs_sync_with_fallback(
            pt_args[0].u.ui2_arg,
            pt_args[1].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_lst_mngr_swap_svl_rec(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_lst_mngr_swap_svl_rec(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.ui2_arg,
            pt_args[2].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_lst_mngr_copy_svl_rec(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_lst_mngr_copy_svl_rec(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.ui2_arg,
            pt_args[2].u.ui2_arg,
            pt_args[3].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_lst_mngr_clean_svl(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_lst_mngr_clean_svl(
            pt_args[0].u.ui4_arg,
            (LST_MNGR_CLEAN_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_lst_mngr_clean_tsl(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_lst_mngr_clean_tsl(
            pt_args[0].u.ui4_arg,
            (LST_MNGR_CLEAN_T *)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 c_rpc_reg_lst_mngr_op_hndlrs()
{
    RPC_REG_OP_HNDLR(x_lst_mngr_init_list_id_by_type);
    RPC_REG_OP_HNDLR(x_lst_mngr_deinit_list_id_by_type);
    RPC_REG_OP_HNDLR(x_lst_mngr_init_list_id);
    RPC_REG_OP_HNDLR(x_lst_mngr_deinit_list_id);
    RPC_REG_OP_HNDLR(x_lst_mngr_fs_store);
    RPC_REG_OP_HNDLR(x_lst_mngr_fs_sync);
    RPC_REG_OP_HNDLR(x_lst_mngr_reg_dual_bank);
    RPC_REG_OP_HNDLR(x_lst_mngr_dual_bank_fs_store);
    RPC_REG_OP_HNDLR(x_lst_mngr_dual_bank_fs_sync);
    RPC_REG_OP_HNDLR(x_lst_mngr_get_dual_bank_status);
    RPC_REG_OP_HNDLR(x_lst_mngr_reset_dual_bank_status);
    RPC_REG_OP_HNDLR(x_lst_mngr_reg_default_db);
    RPC_REG_OP_HNDLR(x_lst_mngr_load_default_db);
    RPC_REG_OP_HNDLR(x_lst_mngr_dual_bank_fs_sync_with_fallback);
    RPC_REG_OP_HNDLR(x_lst_mngr_swap_svl_rec);
    RPC_REG_OP_HNDLR(x_lst_mngr_copy_svl_rec);
    RPC_REG_OP_HNDLR(x_lst_mngr_clean_svl);
    RPC_REG_OP_HNDLR(x_lst_mngr_clean_tsl);

    return RPCR_OK;
}
