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
 * $RCSfile: c_cecm.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/14 $
 * $SWAuthor: Clear Case Administrator $
 * $MD5HEX: a872122d22ab4b982b54b29e95d0ff55 $
 *
 * Description:
 *         This is CEC Manager API source file, which are exported to
 *         applications.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "inc/common.h"
#include "aee/aee.h"
#include "handle/handle.h"
#include "cec_mngr/x_cecm.h"

#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"



static VOID _x_cecm_nfy_fct_wrapper(CECM_NFY_T* pt_nfy_data,
                               VOID*       pv_tag,
                               UINT32      ui4_data)
    
{    
    RPC_DECL_VOID(3);
    
    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_tag;
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_nfy_data, RPC_DESC_CECM_NFY_T, NULL));
    RPC_ARG_INP(ARG_TYPE_REF_DESC,   pt_nfy_data);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);                      
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_data);    

    RPC_DO_CB(pt_nfy_tag->t_id, "x_cecm_nfy_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID;    
}

static INT32 _hndlr_x_cecm_open_svc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T *  pt_nfy_tag;
    x_cecm_nfy_fct      pf_nfy;

    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }

    if(pt_args[0].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = _x_cecm_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, &pt_args[0].u.pv_func, 1, pt_args[1].u.pv_arg);
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_open_svc(pf_nfy, 
                                          pt_nfy_tag, 
                                          (HANDLE_T *)pt_args[2].u.pui4_arg);
    
    if(pt_return->u.i4_arg == CECMR_OK)
    {
        ri_bind_cb_tag(*(HANDLE_T *)pt_args[2].u.pui4_arg, pt_nfy_tag);
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

static INT32 _hndlr_x_cecm_close_svc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_close_svc((HANDLE_T)pt_args[0].u.ui4_arg);
    
    return RPCR_OK;   

}

static INT32 _hndlr_x_cecm_signal_cec_ready(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_signal_cec_ready((HANDLE_T)pt_args[0].u.ui4_arg,
                                                   pt_args[1].u.b_arg);
    
    return RPCR_OK;       
}

static INT32 _hndlr_x_cecm_set_cec_wakeup_ctrl(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_cec_wakeup_ctrl((HANDLE_T)pt_args[0].u.ui4_arg,
                                                   (CECM_WAKE_UP_CTRL_T*)pt_args[1].u.pv_desc);
    
    return RPCR_OK;    

}

static INT32 _hndlr_x_cecm_set_cec_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_cec_fct((HANDLE_T)pt_args[0].u.ui4_arg,
                                             *(CECM_SET_CEC_FCT_PARAMS_T*)pt_args[1].u.pv_desc);
    
    return RPCR_OK; 

}

static INT32 _hndlr_x_cecm_is_act_src_reved(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_is_act_src_reved((HANDLE_T)pt_args[0].u.ui4_arg,
                                                 (CECM_ACT_SRC_REVED_T*)pt_args[1].u.pv_desc);
    
    return RPCR_OK; 

}

static INT32 _hndlr_x_cecm_get_la_by_pa(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECM_LOG_ADDR_MAX;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_get_la_by_pa((HANDLE_T)pt_args[0].u.ui4_arg,
                                               pt_args[1].u.ui2_arg);
    
    return RPCR_OK; 

}

static INT32 _hndlr_x_cecm_get_la_by_pa_ex(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_get_la_by_pa_ex((HANDLE_T)pt_args[0].u.ui4_arg,
                                                  pt_args[1].u.ui2_arg,
                                                  (CECM_LOG_ADD_LST_T*)pt_args[2].u.pv_desc);
    
    return RPCR_OK; 

}

static INT32 _hndlr_x_cecm_get_pa_by_la(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_UINT16; 
        pt_return->u.i4_arg = CECM_INV_PA;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_UINT16;    
    pt_return->u.ui2_arg = x_cecm_get_pa_by_la((HANDLE_T)pt_args[0].u.ui4_arg,
                                              (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg);
    
    return RPCR_OK; 

}

static INT32 _hndlr_x_cecm_get_dev_info_by_la(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_get_dev_info_by_la((HANDLE_T)pt_args[0].u.ui4_arg,
                                              (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                              (CECM_CTRL_T*)pt_args[2].u.pv_desc,
                                              (CECM_DEV_T*)pt_args[3].u.pv_desc);
    
    return RPCR_OK; 

}


static INT32 _hndlr_x_cecm_get_comp_id_by_la(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_get_comp_id_by_la((HANDLE_T)pt_args[0].u.ui4_arg,
                                              (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               pt_args[2].u.pui2_arg);
    return RPCR_OK; 

}
static INT32 _hndlr_x_cecm_get_comp_id_by_la_ex(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_get_comp_id_by_la_ex((HANDLE_T)pt_args[0].u.ui4_arg,
                                              (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               pt_args[2].u.pui2_arg,
                                               pt_args[3].u.b_arg);
    return RPCR_OK; 

}



static INT32 _hndlr_x_cecm_set_max_tv_la(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_max_tv_la((HANDLE_T)pt_args[0].u.ui4_arg,
                                              (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg);
    
    return RPCR_OK; 

}

static INT32 _hndlr_x_cecm_set_support_cec_ver(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_support_cec_ver((HANDLE_T)pt_args[0].u.ui4_arg,
                                                     (CECM_CEC_VER_T)pt_args[1].u.ui4_arg);
    
    return RPCR_OK; 

}


static INT32 _hndlr_x_cecm_find_tv_la_alloc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_find_tv_la_alloc((HANDLE_T)pt_args[0].u.ui4_arg);
    
    return RPCR_OK; 

}

static INT32 _hndlr_x_cecm_find_tv_la_alloc_ex(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_find_tv_la_alloc_ex((HANDLE_T)pt_args[0].u.ui4_arg, pt_args[1].u.ui4_arg);
    
    return RPCR_OK; 

}


static INT32 _hndlr_x_cecm_config_dev_discovery(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
     
    pt_return->e_type = ARG_TYPE_INT32;    
    
    pt_return->u.i4_arg = x_cecm_config_dev_discovery((HANDLE_T)pt_args[0].u.ui4_arg,                                          
                                              (CECM_DEV_DISCOVERY_CONFIG_T*)pt_args[1].u.pv_desc);   
    return RPCR_OK; 

}

static INT32 _hndlr_x_cecm_get_dev_discovery_config(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_get_dev_discovery_config((HANDLE_T)pt_args[0].u.ui4_arg,                                          
                                              (CECM_DEV_DISCOVERY_CONFIG_T*)pt_args[1].u.pv_desc);
    return RPCR_OK; 

}

static INT32 _hndlr_x_cecm_start_dev_discovery(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_start_dev_discovery((HANDLE_T)pt_args[0].u.ui4_arg,
                                                      pt_args[1].u.ui4_arg);
    return RPCR_OK; 

}

static INT32 _hndlr_x_cecm_start_dev_discovery_ex(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_start_dev_discovery_ex((HANDLE_T)pt_args[0].u.ui4_arg,
                                                     (CECM_DEV_DISCOVERY_T*)pt_args[1].u.pv_desc);
    return RPCR_OK; 

}

static INT32 _hndlr_x_cecm_stop_dev_discovery(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_stop_dev_discovery((HANDLE_T)pt_args[0].u.ui4_arg);
    return RPCR_OK; 

}

static INT32 _hndlr_x_cecm_set_active_source(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_active_source((HANDLE_T)pt_args[0].u.ui4_arg);
    return RPCR_OK; 

}

static INT32 _hndlr_x_cecm_request_active_source(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_request_active_source((HANDLE_T)pt_args[0].u.ui4_arg);
    return RPCR_OK; 

}

static INT32 _hndlr_x_cecm_set_standby(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_standby((HANDLE_T)pt_args[0].u.ui4_arg,
                                             (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg);
    return RPCR_OK; 

}

static INT32 _hndlr_x_cecm_set_standby_all(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_standby_all((HANDLE_T)pt_args[0].u.ui4_arg,
                                                  pt_args[1].u.b_arg);
    return RPCR_OK; 

}


static INT32 _hndlr_x_cecm_set_routing_change(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_routing_change((HANDLE_T)pt_args[0].u.ui4_arg,
                                                     pt_args[1].u.ui2_arg,
                                                     pt_args[2].u.ui2_arg);
    return RPCR_OK; 

}
static INT32 _hndlr_x_cecm_set_stream_path(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_stream_path((HANDLE_T)pt_args[0].u.ui4_arg,
                                                 (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg);
    return RPCR_OK; 

}

static INT32 _hndlr_x_cecm_set_stream_path_ex(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_stream_path_ex((HANDLE_T)pt_args[0].u.ui4_arg,
                                                     pt_args[1].u.ui2_arg);
    return RPCR_OK; 

}

static INT32 _hndlr_x_cecm_get_cec_version(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_get_cec_version((HANDLE_T)pt_args[0].u.ui4_arg,
                                                 (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg);
    return RPCR_OK; 

}

static INT32 _hndlr_x_cecm_give_physical_address(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_give_physical_address((HANDLE_T)pt_args[0].u.ui4_arg,
                                                       (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg);
    return RPCR_OK; 

}

static INT32 _hndlr_x_cecm_get_menu_lang(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_get_menu_lang((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg);
    return RPCR_OK; 

}

static INT32 _hndlr_x_cecm_set_polling_message(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_polling_message((HANDLE_T)pt_args[0].u.ui4_arg,
                                                     (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                                     pt_args[2].u.b_arg,
                                                     (CECM_POLLING_INFO_T*)pt_args[3].u.pv_desc);
    return RPCR_OK; 

}

static INT32 _hndlr_x_cecm_report_physical_address(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_report_physical_address((HANDLE_T)pt_args[0].u.ui4_arg,
                                                         (CECM_REPORT_PHY_ADDR_T*)pt_args[1].u.pv_desc);
    return RPCR_OK; 

}


static INT32 _hndlr_x_cecm_set_menu_lang(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_menu_lang((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (ISO_639_LANG_T*)pt_args[1].u.ps_str);
    return RPCR_OK; 

}


static INT32 _hndlr_x_cecm_give_deck_status(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_give_deck_status((HANDLE_T)pt_args[0].u.ui4_arg,
                                                  (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                                  (CECM_STS_REQ_T)pt_args[2].u.ui4_arg);
    return RPCR_OK; 

}

static INT32 _hndlr_x_cecm_set_deck_ctrl(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_deck_ctrl((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               (CECM_DECK_CTRL_MOD_T)pt_args[2].u.ui4_arg);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_set_deck_play(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_deck_play((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               (CECM_PLAY_MOD_T)pt_args[2].u.ui4_arg);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_set_osd_name(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_osd_name((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               (UTF16_T*)pt_args[2].u.ps_str);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_give_osd_name(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_give_osd_name((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_set_menu_request(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_menu_request((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               (CECM_MENU_REQ_STATE_T)pt_args[2].u.ui4_arg);
    return RPCR_OK;

}


static INT32 _hndlr_x_cecm_set_device_vendor_id(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_device_vendor_id((HANDLE_T)pt_args[0].u.ui4_arg,
                                                      (CECM_DEV_VNDR_ID_T*)pt_args[1].u.pui1_arg,
                                                      pt_args[2].u.b_arg);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_give_device_vendor_id(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_give_device_vendor_id((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               pt_args[2].u.b_arg,
                                               (CECM_DEV_VNDR_ID_T*)pt_args[3].u.pui1_arg);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_set_user_ctrl_pressed(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_user_ctrl_pressed((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               (CECM_USER_CTRL_T)pt_args[2].u.ui4_arg);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_set_user_ctrl_pressed_ex(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_user_ctrl_pressed_ex((HANDLE_T)pt_args[0].u.ui4_arg,
                                                          (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                                          (CECM_USER_CTRL_WITH_OPERAND_T*)pt_args[2].u.pv_desc);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_set_user_ctrl_released(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_user_ctrl_released((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg);
    return RPCR_OK;

}


static INT32 _hndlr_x_cecm_power_on_dev(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_power_on_dev((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_give_device_power_status(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_give_device_power_status((HANDLE_T)pt_args[0].u.ui4_arg,
                                                 (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                                  pt_args[2].u.b_arg,
                                                 (CECM_PWR_STS_T*)pt_args[3].u.pv_desc);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_report_power_status(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_report_power_status((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               (CECM_PWR_STS_T)pt_args[2].u.ui4_arg);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_set_system_audio_mode_request(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_system_audio_mode_request((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_SYS_AUD_CTRL_T*)pt_args[1].u.pv_desc);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_set_system_audio_mode_request_ex(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_system_audio_mode_request_ex((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_SYS_AUD_CTRL_T*)pt_args[1].u.ui4_arg,
                                                pt_args[2].u.b_arg,
                                                (CECM_SYS_AUD_STS_T*)pt_args[3].u.ui4_arg);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_give_audio_status(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_give_audio_status((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               pt_args[2].u.b_arg,
                                               pt_args[3].u.pui1_arg);
    return RPCR_OK;

}


static INT32 _hndlr_x_cecm_give_system_audio_mode_status(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_give_system_audio_mode_status((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_request_audio_descriptor(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_request_audio_descriptor((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               (CECM_AUD_FMT_T*)pt_args[2].u.pv_desc,
                                               pt_args[3].u.b_arg);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_set_audio_rate(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_audio_rate((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               (CECM_AUD_RATE_T)pt_args[2].u.ui4_arg);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_clear_timer(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_clear_timer((HANDLE_T)pt_args[0].u.ui4_arg,
                                             (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                             (CECM_TIMER_INFO_T*)pt_args[2].u.pv_desc);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_set_timer(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_timer((HANDLE_T)pt_args[0].u.ui4_arg,
                                           (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                           (CECM_TIMER_INFO_T*) pt_args[2].u.pv_desc);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_set_timer_prog_title(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_timer_prog_title((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               (CECM_PROG_TITLE_T*)pt_args[2].u.ps_str);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_give_tuner_status(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_give_tuner_status((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               (CECM_STS_REQ_T)pt_args[2].u.ui4_arg);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_select_service(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_select_service((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               (CECM_TUNER_SVC_INFO_T*)pt_args[2].u.pv_desc);
    return RPCR_OK;
}

static INT32 _hndlr_x_cecm_set_tuner_step(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_tuner_step((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               (CECM_TUNER_STEP_T)pt_args[2].u.ui4_arg);
    return RPCR_OK;
}

static INT32 _hndlr_x_cecm_set_record_on(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_record_on((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               (CECM_REC_SRC_T*)pt_args[2].u.pv_desc);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_set_record_off(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_record_off((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_set_feature_abort(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_feature_abort((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               (CECM_FEATURE_ABORT_T*)pt_args[2].u.pv_desc);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_set_vendor_cmd(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_vendor_cmd((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               (CECM_VNDR_CMD_DATA_T*)pt_args[2].u.pv_desc,
                                               pt_args[3].u.b_arg);
    return RPCR_OK;
  
}

static INT32 _hndlr_x_cecm_set_vendor_cmd_with_id(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_vendor_cmd_with_id((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               (CECM_VNDR_CMD_WITH_ID_DATA_T*)pt_args[2].u.pv_desc,
                                               pt_args[3].u.b_arg);
    return RPCR_OK;
}

static INT32 _hndlr_x_cecm_set_vendor_remote_btn_down(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_vendor_remote_btn_down((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               (CECM_VNDR_RC_CODE_DATA_T*)pt_args[2].u.pv_desc);
    return RPCR_OK;

}


static INT32 _hndlr_x_cecm_set_vendor_remote_btn_up(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_set_vendor_remote_btn_up((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg);
    return RPCR_OK;

}
static INT32 _hndlr_x_cecm_arc_ctrl(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_arc_ctrl((HANDLE_T)pt_args[0].u.ui4_arg,
                                          (CECM_ARC_CTRL_T*)pt_args[2].u.pv_desc);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_request_init_arc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_request_init_arc((HANDLE_T)pt_args[0].u.ui4_arg,
                                                  (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                                   pt_args[2].u.b_arg);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_request_terminate_arc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_request_terminate_arc((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                                pt_args[2].u.b_arg);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_report_init_arc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_report_init_arc((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               pt_args[2].u.b_arg);
    return RPCR_OK;

}

static INT32 _hndlr_x_cecm_report_terminate_arc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        pt_return->e_type = ARG_TYPE_INT32; 
        pt_return->u.i4_arg = CECMR_INV_ARG;
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_cecm_report_terminate_arc((HANDLE_T)pt_args[0].u.ui4_arg,
                                               (CECM_LOG_ADDR_T)pt_args[1].u.ui4_arg,
                                               pt_args[2].u.b_arg);
    return RPCR_OK;

}

INT32 c_rpc_reg_cecm_op_hndlrs()
{

    RPC_REG_OP_HNDLR(x_cecm_open_svc);
    RPC_REG_OP_HNDLR(x_cecm_close_svc);
    RPC_REG_OP_HNDLR(x_cecm_signal_cec_ready);
    RPC_REG_OP_HNDLR(x_cecm_set_cec_wakeup_ctrl);
    RPC_REG_OP_HNDLR(x_cecm_set_cec_fct);
    RPC_REG_OP_HNDLR(x_cecm_is_act_src_reved);
    RPC_REG_OP_HNDLR(x_cecm_get_la_by_pa);
    RPC_REG_OP_HNDLR(x_cecm_get_la_by_pa_ex);
    RPC_REG_OP_HNDLR(x_cecm_get_pa_by_la);
    RPC_REG_OP_HNDLR(x_cecm_get_dev_info_by_la);
    RPC_REG_OP_HNDLR(x_cecm_get_comp_id_by_la);
    RPC_REG_OP_HNDLR(x_cecm_get_comp_id_by_la_ex);
    RPC_REG_OP_HNDLR(x_cecm_set_max_tv_la);
    RPC_REG_OP_HNDLR(x_cecm_set_support_cec_ver);
    RPC_REG_OP_HNDLR(x_cecm_find_tv_la_alloc);
    RPC_REG_OP_HNDLR(x_cecm_find_tv_la_alloc_ex);
    RPC_REG_OP_HNDLR(x_cecm_config_dev_discovery);
    RPC_REG_OP_HNDLR(x_cecm_get_dev_discovery_config);
    RPC_REG_OP_HNDLR(x_cecm_start_dev_discovery_ex);
    RPC_REG_OP_HNDLR(x_cecm_start_dev_discovery);
    RPC_REG_OP_HNDLR(x_cecm_stop_dev_discovery);
    RPC_REG_OP_HNDLR(x_cecm_set_active_source);
    RPC_REG_OP_HNDLR(x_cecm_request_active_source);
    RPC_REG_OP_HNDLR(x_cecm_set_standby);
    RPC_REG_OP_HNDLR(x_cecm_set_standby_all);
    RPC_REG_OP_HNDLR(x_cecm_start_dev_discovery);
    RPC_REG_OP_HNDLR(x_cecm_set_routing_change);
    RPC_REG_OP_HNDLR(x_cecm_set_stream_path);
    RPC_REG_OP_HNDLR(x_cecm_set_stream_path_ex);
    RPC_REG_OP_HNDLR(x_cecm_start_dev_discovery);
    RPC_REG_OP_HNDLR(x_cecm_get_cec_version);
    RPC_REG_OP_HNDLR(x_cecm_give_physical_address);
    RPC_REG_OP_HNDLR(x_cecm_get_menu_lang);
    RPC_REG_OP_HNDLR(x_cecm_set_polling_message);
    RPC_REG_OP_HNDLR(x_cecm_report_physical_address);
    RPC_REG_OP_HNDLR(x_cecm_set_menu_lang);
    RPC_REG_OP_HNDLR(x_cecm_give_deck_status);
    RPC_REG_OP_HNDLR(x_cecm_set_deck_ctrl);
    RPC_REG_OP_HNDLR(x_cecm_set_deck_play);
    RPC_REG_OP_HNDLR(x_cecm_set_osd_name);
    RPC_REG_OP_HNDLR(x_cecm_give_osd_name);
    RPC_REG_OP_HNDLR(x_cecm_set_menu_request);
    RPC_REG_OP_HNDLR(x_cecm_set_deck_play);
    RPC_REG_OP_HNDLR(x_cecm_set_device_vendor_id);
    RPC_REG_OP_HNDLR(x_cecm_give_device_vendor_id);
    RPC_REG_OP_HNDLR(x_cecm_set_user_ctrl_pressed);
    RPC_REG_OP_HNDLR(x_cecm_set_user_ctrl_pressed_ex);
    RPC_REG_OP_HNDLR(x_cecm_set_user_ctrl_released);
    RPC_REG_OP_HNDLR(x_cecm_set_deck_play);
    RPC_REG_OP_HNDLR(x_cecm_power_on_dev);
    RPC_REG_OP_HNDLR(x_cecm_give_device_power_status);
    RPC_REG_OP_HNDLR(x_cecm_report_power_status);
    RPC_REG_OP_HNDLR(x_cecm_set_system_audio_mode_request);
    RPC_REG_OP_HNDLR(x_cecm_set_system_audio_mode_request_ex);
    RPC_REG_OP_HNDLR(x_cecm_give_audio_status);
    RPC_REG_OP_HNDLR(x_cecm_give_system_audio_mode_status);
    RPC_REG_OP_HNDLR(x_cecm_request_audio_descriptor);
    RPC_REG_OP_HNDLR(x_cecm_set_audio_rate);
    RPC_REG_OP_HNDLR(x_cecm_clear_timer);
    RPC_REG_OP_HNDLR(x_cecm_set_timer);
    RPC_REG_OP_HNDLR(x_cecm_set_timer_prog_title);
    RPC_REG_OP_HNDLR(x_cecm_give_tuner_status);
    RPC_REG_OP_HNDLR(x_cecm_select_service);
    RPC_REG_OP_HNDLR(x_cecm_set_tuner_step);
    RPC_REG_OP_HNDLR(x_cecm_set_record_on);
    RPC_REG_OP_HNDLR(x_cecm_set_record_off);
    RPC_REG_OP_HNDLR(x_cecm_set_feature_abort);
    RPC_REG_OP_HNDLR(x_cecm_set_vendor_cmd);
    RPC_REG_OP_HNDLR(x_cecm_set_vendor_cmd_with_id);
    RPC_REG_OP_HNDLR(x_cecm_set_vendor_remote_btn_down);
    RPC_REG_OP_HNDLR(x_cecm_set_feature_abort);
    RPC_REG_OP_HNDLR(x_cecm_set_vendor_remote_btn_up);
    RPC_REG_OP_HNDLR(x_cecm_arc_ctrl);
    RPC_REG_OP_HNDLR(x_cecm_request_init_arc);
    RPC_REG_OP_HNDLR(x_cecm_request_terminate_arc);
    RPC_REG_OP_HNDLR(x_cecm_report_init_arc);
    RPC_REG_OP_HNDLR(x_cecm_report_terminate_arc);
    
    return RPCR_OK;

}



