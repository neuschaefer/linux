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
#include "conn_mngr/c_cm.h"
#include "conn_mngr/x_cm.h"
#include "handle/handle.h"

#include "rpc.h"
#include "ri_common.h"

#include "ri_desc.h"

VOID _x_cm_nfy_fct_wrapper (
    HANDLE_T  h_handle,
    CM_COND_T e_nfy_cond,
    VOID*     pv_nfy_tag,
    UINT32    ui4_data)
{
    RPC_DECL_VOID(4);
    
    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_handle);
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_nfy_cond);        
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);        
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_data);    

    RPC_DO_CB(pt_nfy_tag->t_id, "x_cm_nfy_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID;    
}


INT32 _hndlr_x_cm_open_pipe(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    x_cm_nfy_fct pf_nfy;
    RPC_CB_NFY_TAG_T * pt_nfy_tag;
    
    if(ui4_num_args != 7)
    {
        return RPCR_INV_ARGS;
    }      

    if(pt_args[4].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = _x_cm_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, &pt_args[4].u.pv_func, 1, pt_args[3].u.pv_arg);
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_cm_open_pipe(
                        (const CM_COMMAND_T*)pt_args[0].u.pv_desc,
                        (UINT8              )pt_args[1].u.b_arg,
                        (UINT32             )pt_args[2].u.ui4_arg,
                        (VOID *             )pt_nfy_tag,
                        pf_nfy,
                        (HANDLE_T*          )pt_args[5].u.pui4_arg,
                        (CM_COND_T*         )pt_args[6].u.pui4_arg);
    if( pt_return->u.i4_arg == CMR_OK)
    {
        ri_bind_cb_tag(*(HANDLE_T *)pt_args[5].u.pui4_arg, pt_nfy_tag);
    }
	else
	{
	    if(pt_nfy_tag != NULL)
        {
            ri_free_cb_tag(pt_nfy_tag);
        }   
	}

    return RPCR_OK;
}


INT32 _hndlr_x_cm_close_pipe(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{    
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_cm_close_pipe(
                          (HANDLE_T   )pt_args[0].u.ui4_arg,
                          (CM_COND_T *)pt_args[1].u.pui4_arg);    
    return RPCR_OK;
}

INT32 _hndlr_x_cm_connect(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{    
    x_cm_nfy_fct pf_nfy;
    RPC_CB_NFY_TAG_T * pt_nfy_tag;
    
    if(ui4_num_args != 6)
    {
        return RPCR_INV_ARGS;
    }      

    if(pt_args[4].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = _x_cm_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, &pt_args[3].u.pv_func, 1, pt_args[2].u.pv_arg);
    }
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_cm_connect(
                            (UINT32             )pt_args[0].u.ui4_arg,                        
                            (const CM_COMMAND_T*)pt_args[1].u.pv_desc,
                            (VOID *             )pt_nfy_tag,
                            pf_nfy,
                            (HANDLE_T*          )pt_args[4].u.pui4_arg,
                            (CM_COND_T*         )pt_args[5].u.pui4_arg);
                        
    if( pt_return->u.i4_arg == CMR_OK)
    {
        ri_bind_cb_tag(*(HANDLE_T *)pt_args[4].u.pui4_arg, pt_nfy_tag);
    }
	else
	{	
	    if(pt_nfy_tag != NULL)
        {
            ri_free_cb_tag(pt_nfy_tag);
        }   
	}

    return RPCR_OK;
}


INT32 _hndlr_x_cm_disconnect(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{    
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_cm_disconnect((HANDLE_T   )pt_args[0].u.ui4_arg,
                                          (CM_COND_T *)pt_args[1].u.pui4_arg);    
    return RPCR_OK;
}

INT32 _hndlr_x_cm_set(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{    
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }      
    
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_cm_set((HANDLE_T      )pt_args[0].u.ui4_arg,
                                   (CM_CTRL_TYPE_T)pt_args[1].u.ui4_arg,
                                                   pt_args[2].u.pv_desc);    
    return RPCR_OK;
}

INT32 _hndlr_x_cm_get(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{    
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }      
    
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_cm_get((HANDLE_T      )pt_args[0].u.ui4_arg,
                                   (CM_CTRL_TYPE_T)pt_args[1].u.ui4_arg,
                                                   pt_args[2].u.pv_desc);    
    return RPCR_OK;
}


INT32 _hndlr_x_cm_get_cond(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{    
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }      
    
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_cm_get_cond((HANDLE_T   )pt_args[0].u.ui4_arg,
                                        (CM_COND_T *)pt_args[1].u.pui4_arg);    
    return RPCR_OK;
}

INT32 c_rpc_reg_cm_op_hndlrs()
{
    RPC_REG_OP_HNDLR(x_cm_open_pipe);
    RPC_REG_OP_HNDLR(x_cm_close_pipe);
    RPC_REG_OP_HNDLR(x_cm_connect);
    RPC_REG_OP_HNDLR(x_cm_disconnect);
    RPC_REG_OP_HNDLR(x_cm_set);
    RPC_REG_OP_HNDLR(x_cm_get);
    RPC_REG_OP_HNDLR(x_cm_get_cond);
    return RPCR_OK;
}

