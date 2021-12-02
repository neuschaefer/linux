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

#include "df_mngr/x_df_mngr.h"


#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"

#ifdef MW_ISDB_SUPPORT
INT32 _hndlr_x_dfm_nfy_fct(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        VOID *       pv_cb_addr,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }
    /*Maybe not needed*/
    pt_return->e_type = ARG_TYPE_VOID;

    ((x_dfm_nfy_fct)pv_cb_addr)(
            (HANDLE_T)pt_args[0].u.ui4_arg,
            (DFM_COND_T)pt_args[1].u.i4_arg,
            pt_args[2].u.pv_arg,
            pt_args[3].u.ui4_arg);

    return RPCR_OK;
}

INT32 c_dfm_open(const CHAR * ps_dfm_name,
                 HANDLE_T * ph_dfm)
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_REF_STR, ps_dfm_name);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_dfm);

    RPC_DO_OP("x_dfm_open");

    RPC_RETURN(ARG_TYPE_INT32, DFMR_FAIL);
}

INT32 c_dfm_close(HANDLE_T ph_dfm)
{
    RPC_DECL(1, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, ph_dfm);

    RPC_DO_OP("x_dfm_close");

    RPC_RETURN(ARG_TYPE_INT32, DFMR_FAIL);
}

INT32 c_dfm_load_logo(HANDLE_T h_dfm,
                      UINT16 ui2_on_id,
                      UINT16 ui2_svc_id,
                      UINT16 ui2_scan_idx,
                      VOID * pv_tag,
                      x_dfm_nfy_fct pt_nfy,
                      HANDLE_T * ph_logo,
                      DFM_COND_T * pe_cond)
{
    RPC_DECL(8, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_dfm);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_on_id);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_svc_id);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_scan_idx);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pt_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_logo);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_cond);

    RPC_DO_OP("x_dfm_load_logo");

    RPC_RETURN(ARG_TYPE_INT32, DFMR_FAIL);
}

INT32 c_dfm_get_cond(HANDLE_T h_dfm,
                     HANDLE_T h_logo,
                     DFM_COND_T * pe_cond)
{
    RPC_DECL(3, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_dfm);
    RPC_ARG_INP(ARG_TYPE_UINT32, h_logo);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_cond);

    RPC_DO_OP("x_dfm_get_cond");

    RPC_RETURN(ARG_TYPE_INT32, DFMR_FAIL);
}

INT32 c_dfm_get_logo_type(HANDLE_T h_dfm,
                          HANDLE_T h_logo,
                          UINT8 * pui1_logo_types,
                          DFM_COND_T * pe_cond)
{
    RPC_DECL(4, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_dfm);
    RPC_ARG_INP(ARG_TYPE_UINT32, h_logo);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT8, pui1_logo_types);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_cond);

    RPC_DO_OP("x_dfm_get_logo_type");

    RPC_RETURN(ARG_TYPE_INT32, DFMR_FAIL);
}

INT32 c_dfm_get_logo_data(HANDLE_T h_dfm,
                          HANDLE_T h_logo,
                          DFM_LOGO_TYPE_T t_logo_type,
                          WGL_HIMG_TPL_T * pt_img,
                          DFM_COND_T * pe_cond)
{
    RPC_DECL(5, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_dfm);
    RPC_ARG_INP(ARG_TYPE_UINT32, h_logo);
    RPC_ARG_INP(ARG_TYPE_UINT8, t_logo_type);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pt_img); /* TODO: where to get image data from different process? */
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_cond);

    RPC_DO_OP("x_dfm_get_logo_data");

    RPC_RETURN(ARG_TYPE_INT32, DFMR_FAIL);
}

INT32 c_dfm_free_logo(HANDLE_T h_dfm,
                      HANDLE_T h_logo)
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_dfm);
    RPC_ARG_INP(ARG_TYPE_UINT32, h_logo);

    RPC_DO_OP("x_dfm_free_logo");

    RPC_RETURN(ARG_TYPE_INT32, DFMR_FAIL);
}

INT32 c_dfm_change_svc(HANDLE_T h_dfm,
                       UINT16 ui2_onid,
                       UINT16 ui2_service_ID,
                       UINT16 ui2_scan_idx)
{
    RPC_DECL(4, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_dfm);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_onid);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_service_ID);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_scan_idx);

    RPC_DO_OP("x_dfm_change_svc");

    RPC_RETURN(ARG_TYPE_INT32, DFMR_FAIL);
}


INT32 c_rpc_reg_df_mngr_cb_hndlrs()
{
    RPC_REG_CB_HNDLR(x_dfm_nfy_fct);

    return RPCR_OK;
}

#endif
