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
 *         This file contains all TSL
 *         function implementations.
 *-----------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
                    include files
 ------------------------------------------------------------------------*/
#include "tsl/x_tsl.h"
#include "handle/handle.h"

#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"

static VOID 
_x_tsl_nfy_fct_wrapper(
    HANDLE_T            h_tsl,
    TSL_COND_T          e_cond,
    UINT32              ui4_reason,
    VOID*               pv_tag,
    UINT32              ui4_data)
{    
    RPC_DECL_VOID(5);
    
    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_tag;
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_tsl);
    RPC_ARG_INP(ARG_TYPE_INT32,    e_cond);        
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_reason);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);        
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_data);    

    RPC_DO_CB(pt_nfy_tag->t_id, "x_tsl_nfy_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID;    
}

INT32 _hndlr_x_tsl_create(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T *  pt_nfy_tag;
    x_tsl_nfy_fct       pf_nfy;
    
    if(ui4_num_args != 7)
    {
        return RPCR_INV_ARGS;
    }   
    
    if(pt_args[5].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = _x_tsl_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, &pt_args[5].u.pv_func, 1, pt_args[4].u.pv_arg);
    }
   
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_create(
                           pt_args[0].u.ps_str,
                           pt_args[1].u.ui2_arg,
         (TSL_MEM_POLICY_T)pt_args[2].u.ui1_arg,
                           pt_args[3].u.ui2_arg,
                           pt_nfy_tag,
                           pf_nfy,
               (HANDLE_T *)pt_args[6].u.pui4_arg);

    if(pt_return->u.i4_arg == TSLR_OK)
    {
        ri_bind_cb_tag(*(HANDLE_T *)pt_args[6].u.pui4_arg, pt_nfy_tag);
    }
    else if (pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_delete(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_delete(
               (HANDLE_T )pt_args[0].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_clean(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_clean(
               (HANDLE_T )pt_args[0].u.ui4_arg);

    return RPCR_OK;
}
/*
INT32 _hndlr_x_tsl_set_src_name(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_set_src_name(
               (HANDLE_T )pt_args[0].u.ui4_arg,
                          pt_args[1].u.ps_str);

    return RPCR_OK;
}
*/

INT32 _hndlr_x_tsl_add_rec(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_add_rec(
               (HANDLE_T )pt_args[0].u.ui4_arg,
       (const TSL_REC_T* )pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_del_rec(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_del_rec(
               (HANDLE_T )pt_args[0].u.ui4_arg,
                          pt_args[1].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_update_rec(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_update_rec(
               (HANDLE_T )pt_args[0].u.ui4_arg,
        (const TSL_REC_T*)pt_args[1].u.pv_desc,
                          pt_args[2].u.b_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_qry_rec_id(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_qry_rec_id(
               (HANDLE_T )pt_args[0].u.ui4_arg,
        (TSL_REC_ID_QRY_T)pt_args[1].u.ui1_arg,
                          pt_args[2].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_lock(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_lock(
               (HANDLE_T )pt_args[0].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_unlock(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_unlock(
               (HANDLE_T )pt_args[0].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_read_lock(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_read_lock(
               (HANDLE_T )pt_args[0].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_read_unlock(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_read_unlock(
               (HANDLE_T )pt_args[0].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_fs_load(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T * pt_nfy_tag;
    x_tsl_nfy_fct       pf_nfy;
    
    if(ui4_num_args != 7)
    {
        return RPCR_INV_ARGS;
    }   
    
    if(pt_args[5].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = _x_tsl_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, &pt_args[5].u.pv_func, 1, pt_args[4].u.pv_arg);
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_fs_load(
                 (HANDLE_T)pt_args[0].u.ui4_arg,
                           pt_args[1].u.ps_str,
                           pt_args[2].u.ui2_arg,
                           pt_args[3].u.ps_str,
                           pt_nfy_tag,
                           pf_nfy,
               (HANDLE_T *)pt_args[6].u.pui4_arg);

    if(pt_return->u.i4_arg == TSLR_OK)
    {
        ri_bind_cb_tag(*(HANDLE_T *)pt_args[6].u.pui4_arg, pt_nfy_tag);
    }
    else if (pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }
    
    return RPCR_OK;
}

INT32 _hndlr_x_tsl_fs_store(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_fs_store(
		        (HANDLE_T)pt_args[0].u.ui4_arg,
		                  pt_args[1].u.ps_str,
		                  pt_args[2].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_fs_del(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_fs_del(
               	  (HANDLE_T)pt_args[0].u.ui4_arg,
               	            pt_args[1].u.ps_str,
               	            pt_args[2].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_fs_qry(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_fs_qry(
               	  (HANDLE_T)pt_args[0].u.ui4_arg,
               	            pt_args[1].u.ps_str,
               	            pt_args[2].u.ui2_arg,
               	            pt_args[3].u.pui2_arg,
               	            pt_args[4].u.ps_str);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_fs_sync(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_fs_sync(
               	  (HANDLE_T)pt_args[0].u.ui4_arg,
               	  (HANDLE_T)pt_args[1].u.ui4_arg,
               	            pt_args[2].u.ps_str,
               	            pt_args[3].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_open(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T * pt_nfy_tag;
    x_tsl_nfy_fct       pf_nfy;
    
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }   
    
    if(pt_args[2].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = _x_tsl_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, &pt_args[2].u.pv_func, 1, pt_args[1].u.pv_arg);
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_open(
                           pt_args[0].u.ui2_arg,
                           pt_nfy_tag,
                           pf_nfy,
               (HANDLE_T *)pt_args[3].u.pui4_arg);

    if(pt_return->u.i4_arg == TSLR_OK)
    {
        ri_bind_cb_tag(*(HANDLE_T *)pt_args[3].u.pui4_arg, pt_nfy_tag);
    }
    else if (pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }
    
    return RPCR_OK;
}

INT32 _hndlr_x_tsl_open_by_name(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T * pt_nfy_tag;
    x_tsl_nfy_fct       pf_nfy;
    
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }   
    
    if(pt_args[2].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = _x_tsl_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, &pt_args[2].u.pv_func, 1, pt_args[1].u.pv_arg);
    }    
    
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_open_by_name(
                           pt_args[0].u.ps_str,
                           pt_nfy_tag,
                           pf_nfy,
               (HANDLE_T *)pt_args[3].u.pui4_arg);

    if(pt_return->u.i4_arg == TSLR_OK)
    {
        ri_bind_cb_tag(*(HANDLE_T *)pt_args[3].u.pui4_arg, pt_nfy_tag);
    }
    else if (pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_close(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_close(
               (HANDLE_T )pt_args[0].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_get_handle_info(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_get_handle_info(
               	  (HANDLE_T)pt_args[0].u.ui4_arg,
               	            pt_args[1].u.pui2_arg,
                            pt_args[2].u.ps_str);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_get_rec(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_get_rec(
		          (HANDLE_T)pt_args[0].u.ui4_arg,
		                    pt_args[1].u.ui2_arg,
		        (TSL_REC_T*)pt_args[2].u.pv_desc,
		                    pt_args[3].u.pui4_arg);

    return RPCR_OK;
}
/*
INT32 _hndlr_x_tsl_get_src_name(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_get_src_name(
		          (HANDLE_T)pt_args[0].u.ui4_arg,
		                    pt_args[1].u.ps_str);

    return RPCR_OK;
}
*/

INT32 _hndlr_x_tsl_get_num_recs(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_get_num_recs(
		          (HANDLE_T)pt_args[0].u.ui4_arg,
		                    pt_args[1].u.pui2_arg,
		                    pt_args[2].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_rec_init(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_rec_init(
		        (TSL_REC_T*)pt_args[0].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_set_cursor(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_set_cursor(
		          (HANDLE_T)pt_args[0].u.ui4_arg,
		     (TSL_ITERATE_T)pt_args[1].u.ui1_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_read_cursor(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_read_cursor(
               	  (HANDLE_T)pt_args[0].u.ui4_arg,
               	(TSL_REC_T*)pt_args[1].u.pv_desc,
               	            pt_args[2].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_sort_rec(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }   

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_sort_rec(
               	  (HANDLE_T)pt_args[0].u.ui4_arg,
               	            pt_args[1].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_init_mem_dump(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_init_mem_dump(
            (TSL_MEM_DUMP_T*)pt_args[0].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_free_mem_dump(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_free_mem_dump(
            (TSL_MEM_DUMP_T*)pt_args[0].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_dump_to_mem(
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
    pt_return->u.i4_arg = x_tsl_dump_to_mem(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
            (TSL_MEM_DUMP_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_load_from_mem(
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
    pt_return->u.i4_arg = x_tsl_load_from_mem(
                   (HANDLE_T)pt_args[0].u.ui4_arg,
            (TSL_MEM_DUMP_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_get_rec_by_rec_idx(
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
    pt_return->u.i4_arg = x_tsl_get_rec_by_rec_idx(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.ui2_arg,
            pt_args[2].u.pv_desc,
            pt_args[3].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_get_num_rec_by_sat(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 6)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_get_num_rec_by_sat(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.ui4_arg,
            pt_args[2].u.ui2_arg,
            pt_args[3].u.ui2_arg,
            pt_args[4].u.pui2_arg,
            pt_args[5].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_tsl_get_rec_by_sat(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 7)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_tsl_get_rec_by_sat(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.ui4_arg,
            pt_args[2].u.ui2_arg,
            pt_args[3].u.ui2_arg,
            pt_args[4].u.ui2_arg,
            pt_args[5].u.pv_desc,
            pt_args[6].u.pui4_arg);

    return RPCR_OK;
}

INT32 c_rpc_reg_tsl_op_hndlrs()
{
    RPC_REG_OP_HNDLR(x_tsl_create);
    RPC_REG_OP_HNDLR(x_tsl_delete);
    RPC_REG_OP_HNDLR(x_tsl_clean);
    RPC_REG_OP_HNDLR(x_tsl_add_rec);
    RPC_REG_OP_HNDLR(x_tsl_del_rec);
    RPC_REG_OP_HNDLR(x_tsl_update_rec);
    RPC_REG_OP_HNDLR(x_tsl_qry_rec_id);
    RPC_REG_OP_HNDLR(x_tsl_lock);
    RPC_REG_OP_HNDLR(x_tsl_unlock);
    RPC_REG_OP_HNDLR(x_tsl_read_lock);
    RPC_REG_OP_HNDLR(x_tsl_read_unlock);
    RPC_REG_OP_HNDLR(x_tsl_fs_load);
    RPC_REG_OP_HNDLR(x_tsl_fs_store);
    RPC_REG_OP_HNDLR(x_tsl_fs_del);
    RPC_REG_OP_HNDLR(x_tsl_fs_qry);
    RPC_REG_OP_HNDLR(x_tsl_fs_sync);
    RPC_REG_OP_HNDLR(x_tsl_open);
    RPC_REG_OP_HNDLR(x_tsl_open_by_name);
    RPC_REG_OP_HNDLR(x_tsl_close);
    RPC_REG_OP_HNDLR(x_tsl_get_handle_info);
    RPC_REG_OP_HNDLR(x_tsl_get_rec);
    RPC_REG_OP_HNDLR(x_tsl_get_num_recs);
    RPC_REG_OP_HNDLR(x_tsl_rec_init);
    RPC_REG_OP_HNDLR(x_tsl_set_cursor);
    RPC_REG_OP_HNDLR(x_tsl_read_cursor);
    RPC_REG_OP_HNDLR(x_tsl_sort_rec);
    RPC_REG_OP_HNDLR(x_tsl_init_mem_dump);
    RPC_REG_OP_HNDLR(x_tsl_free_mem_dump);
    RPC_REG_OP_HNDLR(x_tsl_dump_to_mem);
    RPC_REG_OP_HNDLR(x_tsl_load_from_mem);
    RPC_REG_OP_HNDLR(x_tsl_get_rec_by_rec_idx);
    RPC_REG_OP_HNDLR(x_tsl_get_num_rec_by_sat);
    RPC_REG_OP_HNDLR(x_tsl_get_rec_by_sat);

    return RPCR_OK;
}


