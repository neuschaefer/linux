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
#include "handle/handle.h"
#include "tbl_mngr/psi_eng/x_psi_eng.h"

#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"

static NFY_RET_T 
_x_tm_nfy_fct_wrapper(
	HANDLE_T       h_obj,
	HANDLE_TYPE_T  e_obj_type,
	TM_COND_T      e_nfy_cond,
	VOID*          pv_nfy_tag,
	UINT32         ui4_data)
{    
    RPC_DECL(5, INT32);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_obj);
    RPC_ARG_INP(ARG_TYPE_UINT16,  e_obj_type);        
    RPC_ARG_INP(ARG_TYPE_INT32,   e_nfy_cond);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);        
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_data);    

    RPC_DO_CB(pt_nfy_tag->t_id, "x_tm_nfy_fct", pt_nfy_tag->pv_cb_addr);

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);    
}

INT32 _hndlr_x_psi_get_raw_section(
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
    pt_return->u.i4_arg = x_psi_get_raw_section(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.ui1_arg,
            pt_args[2].u.pui1_arg,
            pt_args[3].u.pz_arg,
            (TM_COND_T *)pt_args[4].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_psi_get_svc_entry(
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
    pt_return->u.i4_arg = x_psi_get_svc_entry(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.ui2_arg,
            (PSI_PID_SVC_ID_INFO_T *)pt_args[2].u.pv_desc,
            (TM_COND_T *)pt_args[3].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_psi_get_ts_id(
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
    pt_return->u.i4_arg = x_psi_get_ts_id(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.pui2_arg,
            (TM_COND_T *)pt_args[2].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_psi_get_pcr_pid(
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
    pt_return->u.i4_arg = x_psi_get_pcr_pid(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.pui2_arg,
            (TM_COND_T *)pt_args[2].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_psi_get_stream_entry(
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
    pt_return->u.i4_arg = x_psi_get_stream_entry(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.ui2_arg,
            (PSI_STREAM_INFO_T *)pt_args[2].u.pv_desc,
            (TM_COND_T *)pt_args[3].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_psi_get_svc_id(
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
    pt_return->u.i4_arg = x_psi_get_svc_id(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.pui2_arg,
            (TM_COND_T *)pt_args[2].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_psi_mon_get_num_ts_id(
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
    pt_return->u.i4_arg = x_psi_mon_get_num_ts_id(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.pui2_arg,
            (TM_COND_T *)pt_args[2].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_psi_mon_get_ts_id(
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
    pt_return->u.i4_arg = x_psi_mon_get_ts_id(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.ui2_arg,
            pt_args[2].u.pui2_arg,
            (TM_COND_T *)pt_args[3].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_psi_load_tsdt(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T *  pt_nfy_tag;
    x_tm_nfy_fct        pf_nfy;

    if (ui4_num_args != 7)
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
        pf_nfy = _x_tm_nfy_fct_wrapper;
        RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_args[4].u.pv_func, pt_args[3].u.pv_arg);
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_psi_load_tsdt(
            pt_args[0].u.ui4_arg,
            (TM_SRC_TYPE_T)pt_args[1].u.ui1_arg,
            pt_args[2].u.pv_desc,
            pt_nfy_tag,
            pf_nfy,
            pt_args[5].u.pui4_arg,
            (TM_COND_T *)pt_args[6].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_psi_load_cat(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T *  pt_nfy_tag;
    x_tm_nfy_fct        pf_nfy;

    if (ui4_num_args != 7)
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
        pf_nfy = _x_tm_nfy_fct_wrapper;
        RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_args[4].u.pv_func, pt_args[3].u.pv_arg);
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_psi_load_cat(
            pt_args[0].u.ui4_arg,
            (TM_SRC_TYPE_T)pt_args[1].u.ui1_arg,
            pt_args[2].u.pv_desc,
            pt_nfy_tag,
            pf_nfy,
            pt_args[5].u.pui4_arg,
            (TM_COND_T *)pt_args[6].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_psi_load_pat(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T *  pt_nfy_tag;
    x_tm_nfy_fct        pf_nfy;

    if (ui4_num_args != 7)
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
        pf_nfy = _x_tm_nfy_fct_wrapper;
        RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_args[4].u.pv_func, pt_args[3].u.pv_arg);
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_psi_load_pat(
            pt_args[0].u.ui4_arg,
            (TM_SRC_TYPE_T)pt_args[1].u.ui1_arg,
            pt_args[2].u.pv_desc,
            pt_nfy_tag,
            pf_nfy,
            pt_args[5].u.pui4_arg,
            (TM_COND_T *)pt_args[6].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_psi_load_pat_by_ts_id(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T *  pt_nfy_tag;
    x_tm_nfy_fct        pf_nfy;

    if (ui4_num_args != 8)
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
        pf_nfy = _x_tm_nfy_fct_wrapper;
        RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_args[5].u.pv_func, pt_args[4].u.pv_arg);
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_psi_load_pat_by_ts_id(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.ui2_arg,
            (TM_SRC_TYPE_T)pt_args[2].u.ui1_arg,
            pt_args[3].u.pv_desc,
            pt_nfy_tag,
            pf_nfy,
            pt_args[6].u.pui4_arg,
            (TM_COND_T *)pt_args[7].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_psi_load_pat_by_ts_descr(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T *  pt_nfy_tag;
    x_tm_nfy_fct        pf_nfy;

    if (ui4_num_args != 8)
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
        pf_nfy = _x_tm_nfy_fct_wrapper;
        RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_args[5].u.pv_func, pt_args[4].u.pv_arg);
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_psi_load_pat_by_ts_descr(
            pt_args[0].u.ui4_arg,
            (TS_DESCR_T *)pt_args[1].u.pv_desc,
            (TM_SRC_TYPE_T)pt_args[2].u.ui1_arg,
            pt_args[3].u.pv_desc,
            pt_nfy_tag,
            pf_nfy,
            pt_args[6].u.pui4_arg,
            (TM_COND_T *)pt_args[7].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_psi_load_pmt_by_svc_id(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T *  pt_nfy_tag;
    x_tm_nfy_fct        pf_nfy;

    if (ui4_num_args != 8)
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
        pf_nfy = _x_tm_nfy_fct_wrapper;
        RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_args[5].u.pv_func, pt_args[4].u.pv_arg);
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_psi_load_pmt_by_svc_id(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.ui2_arg,
            (TM_SRC_TYPE_T)pt_args[2].u.ui1_arg,
            pt_args[3].u.pv_desc,
            pt_nfy_tag,
            pf_nfy,
            pt_args[6].u.pui4_arg,
            (TM_COND_T *)pt_args[7].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_psi_load_pmt_by_idx(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T *  pt_nfy_tag;
    x_tm_nfy_fct        pf_nfy;

    if (ui4_num_args != 8)
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
        pf_nfy = _x_tm_nfy_fct_wrapper;
        RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_args[5].u.pv_func, pt_args[4].u.pv_arg);
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_psi_load_pmt_by_idx(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.ui2_arg,
            (TM_SRC_TYPE_T)pt_args[2].u.ui1_arg,
            pt_args[3].u.pv_desc,
            pt_nfy_tag,
            pf_nfy,
            pt_args[6].u.pui4_arg,
            (TM_COND_T *)pt_args[7].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_psi_mon_pat(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T *  pt_nfy_tag;
    x_tm_nfy_fct        pf_nfy;

    if (ui4_num_args != 7)
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
        pf_nfy = _x_tm_nfy_fct_wrapper;
        RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_args[4].u.pv_func, pt_args[3].u.pv_arg);
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_psi_mon_pat(
            pt_args[0].u.ui4_arg,
            (TM_SRC_TYPE_T)pt_args[1].u.ui1_arg,
            pt_args[2].u.pv_desc,
            pt_nfy_tag,
            pf_nfy,
            pt_args[5].u.pui4_arg,
            (TM_COND_T *)pt_args[6].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_psi_set_pat_pmt_info(
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
    pt_return->u.i4_arg = x_psi_set_pat_pmt_info(
            pt_args[0].u.ui4_arg,
            (PSI_PID_SVC_ID_INFO_T *)pt_args[1].u.pv_desc,
            (TM_COND_T *)pt_args[2].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_psi_set_available(
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
    pt_return->u.i4_arg = x_psi_set_available(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.b_arg,
            (TM_COND_T *)pt_args[2].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_psi_set_version_num(
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
    pt_return->u.i4_arg = x_psi_set_version_num(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.ui1_arg,
            (TM_COND_T *)pt_args[2].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_psi_set_pmt_pcr_pid(
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
    pt_return->u.i4_arg = x_psi_set_pmt_pcr_pid(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.ui2_arg,
            (TM_COND_T *)pt_args[2].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_psi_set_pmt_stream_entry(
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
    pt_return->u.i4_arg = x_psi_set_pmt_stream_entry(
            pt_args[0].u.ui4_arg,
            (PSI_STREAM_INFO_T *)pt_args[1].u.pv_desc,
            (TM_COND_T *)pt_args[2].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_psi_set_pmt_stream_descr(
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
    pt_return->u.i4_arg = x_psi_set_pmt_stream_descr(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.pui1_arg,
            pt_args[2].u.z_arg,
            (TM_COND_T *)pt_args[3].u.pui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_psi_set_pmt_main_descr(
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
    pt_return->u.i4_arg = x_psi_set_pmt_main_descr(
            pt_args[0].u.ui4_arg,
            pt_args[1].u.pui1_arg,
            pt_args[2].u.z_arg,
            (TM_COND_T *)pt_args[3].u.pui4_arg);

    return RPCR_OK;
}

INT32 c_rpc_reg_psi_eng_op_hndlrs()
{
    RPC_REG_OP_HNDLR(x_psi_get_raw_section);
    RPC_REG_OP_HNDLR(x_psi_get_svc_entry);
    RPC_REG_OP_HNDLR(x_psi_get_ts_id);
    RPC_REG_OP_HNDLR(x_psi_get_pcr_pid);
    RPC_REG_OP_HNDLR(x_psi_get_stream_entry);
    RPC_REG_OP_HNDLR(x_psi_get_svc_id);
    RPC_REG_OP_HNDLR(x_psi_mon_get_num_ts_id);
    RPC_REG_OP_HNDLR(x_psi_mon_get_ts_id);
    RPC_REG_OP_HNDLR(x_psi_load_tsdt);
    RPC_REG_OP_HNDLR(x_psi_load_cat);
    RPC_REG_OP_HNDLR(x_psi_load_pat);
    RPC_REG_OP_HNDLR(x_psi_load_pat_by_ts_id);
    RPC_REG_OP_HNDLR(x_psi_load_pat_by_ts_descr);
    RPC_REG_OP_HNDLR(x_psi_load_pmt_by_svc_id);
    RPC_REG_OP_HNDLR(x_psi_load_pmt_by_idx);
    RPC_REG_OP_HNDLR(x_psi_mon_pat);
    RPC_REG_OP_HNDLR(x_psi_set_pat_pmt_info);
    RPC_REG_OP_HNDLR(x_psi_set_available);
    RPC_REG_OP_HNDLR(x_psi_set_version_num);
    RPC_REG_OP_HNDLR(x_psi_set_pmt_pcr_pid);
    RPC_REG_OP_HNDLR(x_psi_set_pmt_stream_entry);
    RPC_REG_OP_HNDLR(x_psi_set_pmt_stream_descr);
    RPC_REG_OP_HNDLR(x_psi_set_pmt_main_descr);

    return RPCR_OK;
}
