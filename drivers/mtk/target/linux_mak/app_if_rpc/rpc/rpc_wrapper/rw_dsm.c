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
#include "inc/x_common.h"
#include "dsm/x_dsm.h"
#include "strm_mngr/u_sm.h"

#include "rpc.h"
#include "ri_common.h"

#include "ri_desc.h"

static INT32 _hndlr_dsm_dev_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }   
    
    pt_return->e_type   = ARG_TYPE_VOID;

    ((dsm_dev_nfy_fct)pv_cb_addr)(
                    (HANDLE_T      )pt_args[0].u.ui4_arg,
                    (DSM_COND_DEV_T)pt_args[1].u.ui4_arg,
                                    pt_args[2].u.pv_arg,
                                    pt_args[3].u.ui4_arg,
                                    pt_args[4].u.ui4_arg);

    return RPCR_OK;
}

INT32  c_dsm_open
(
    DEVICE_TYPE_T       t_dev_group,    /* in */
    const CHAR*         ps_name,           /* in */
    VOID*               pv_tag,                 /* in */
    dsm_dev_nfy_fct     pf_nfy_fct,        /* in */
    HANDLE_T*           ph_dsm             /* out */
) 
{
    RPC_DECL(5, INT32);    


    RPC_ARG_INP(ARG_TYPE_UINT32,     t_dev_group);
    RPC_ARG_INP(ARG_TYPE_REF_STR,    ps_name);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,    pv_tag);    
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_nfy_fct);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_dsm);
    
    RPC_DO_OP("x_dsm_open");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);    
}

INT32  c_dsm_dev_open 
(
    HANDLE_T         h_dsm,
    DEVICE_TYPE_T    t_dev_type,
    const CHAR*      ps_name,
    UINT16			 ui2_idx,
    VOID*            pv_tag,
    dsm_dev_nfy_fct  pf_dev_nfy_fct,
    HANDLE_T*        ph_dsm_dev	
) 
{
    RPC_DECL(7, INT32);    


    RPC_ARG_INP(ARG_TYPE_UINT32,     h_dsm);
    RPC_ARG_INP(ARG_TYPE_UINT32,     t_dev_type);
    RPC_ARG_INP(ARG_TYPE_REF_STR,    ps_name);
    RPC_ARG_INP(ARG_TYPE_UINT16,    ui2_idx);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,    pv_tag);    
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_dev_nfy_fct);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_dsm_dev);
    
    RPC_DO_OP("x_dsm_dev_open");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);    
}

INT32  c_dsm_get
(
    HANDLE_T         h_dsm,
    DSM_GET_T  		 t_get_type,
    VOID*            pv_get_info,
    SIZE_T*		     pz_size,
    VOID*            pv_data            /* in/out */
)
{
    RPC_DECL(5, INT32);    

    if(pz_size != NULL)
    {
        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pz_size, *pz_size));
    }
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_dsm);
    RPC_ARG_INP(ARG_TYPE_UINT32,     t_get_type);
    RPC_ARG_INP (ARG_TYPE_REF_VOID,   pv_get_info);    
    RPC_ARG_IO (ARG_TYPE_REF_SIZE_T, pz_size);
    RPC_ARG_IO (ARG_TYPE_REF_DESC,   pv_data);    
    
    RPC_DO_OP("x_dsm_get");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);   
}


INT32  c_dsm_dev_get 
(
    HANDLE_T         h_dsm_dev,
    DSM_DEV_GET_T    t_get_type,
    VOID*            pv_get_info,
    SIZE_T*		     pz_size,
    VOID*            pv_data
)
{
    RPC_DECL(5, INT32);    
   
    if(pz_size != NULL)
    {
        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pz_size, *pz_size));
    }
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_dsm_dev);
    RPC_ARG_INP(ARG_TYPE_UINT32,     t_get_type);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_get_info);    
    RPC_ARG_IO (ARG_TYPE_REF_SIZE_T, pz_size);
    RPC_ARG_IO (ARG_TYPE_REF_DESC,   pv_data);    
    
    RPC_DO_OP("x_dsm_dev_get");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);   
}

INT32  c_dsm_set
(
    HANDLE_T         h_dsm_dev,
    DSM_SET_T        t_set_type,
    VOID*            pv_set_info,
    SIZE_T           z_size
)
{
    RPC_DECL(4, INT32);
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_dsm_dev);
    RPC_ARG_INP(ARG_TYPE_UINT32,     t_set_type);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_set_info);    
    RPC_ARG_INP(ARG_TYPE_SIZE_T,     z_size);
    
    RPC_DO_OP("x_dsm_set");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);   
}



INT32  c_dsm_dev_set
(
    HANDLE_T         h_dsm_dev,
    DSM_DEV_SET_T    t_set_type,
    VOID*            pv_set_info,
    SIZE_T           z_size
)
{
    RPC_DECL(4, INT32);
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_dsm_dev);
    RPC_ARG_INP(ARG_TYPE_UINT32,     t_set_type);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_set_info);    
    RPC_ARG_INP(ARG_TYPE_SIZE_T,     z_size);
    
    RPC_DO_OP("x_dsm_dev_set");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  

}
INT32 c_rpc_reg_dsm_cb_hndlrs()
{
 
    RPC_REG_CB_HNDLR(dsm_dev_nfy_fct); 
    
    return RPCR_OK;
}


