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
#include "stdio.h"

#include "inc/u_common.h"
#include "os/inc/x_os.h"
#include "rpc.h"
#include "ri_common.h"

INT32 rpc_pf_test_api(VOID * pv_data)
{
    int i;
    int j;
    int s = 0;
    for(i = 0; i < 100; i ++)
    {
        for(j = 0; j < 1000; j ++)
        {
            s += i * j/1315;
        }
    }
    return s;
}

INT32 _hndlr_rpc_pf_test(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = rpc_pf_test_api(pt_args[0].u.pv_desc);
    return RPCR_OK;
}

/*
 * Start the server when init.
 */
extern INT32 c_rpc_reg_handle_op_hndlrs();
extern INT32 c_rpc_reg_svctx_op_hndlrs();
#ifdef ENABLE_MULTIMEDIA
extern INT32 c_rpc_reg_mm_svctx_op_hndlrs();
extern INT32 c_rpc_reg_mfmtrecg_op_hndlrs();
extern INT32 c_rpc_reg_minfo_op_hndlrs();
extern INT32 c_rpc_reg_midxbuld_op_hndlrs();
#ifndef ANDROID
extern INT32 c_rpc_reg_img_op_hndlrs();
#endif
#endif
#ifdef MW_CAP_LOGO_SUPPORT
extern INT32 c_rpc_reg_cap_op_hndlrs();
#endif
extern INT32 c_rpc_reg_evctx_op_hndlrs();
extern INT32 c_rpc_reg_flm_op_hndlrs();
extern INT32 c_rpc_reg_svl_op_hndlrs();
extern INT32 c_rpc_reg_tsl_op_hndlrs();
#ifdef RPC_ATSC_SI 
extern INT32 c_rpc_reg_atsc_si_op_hndlrs();
#endif

#ifndef NO_NWL_INIT
extern INT32 c_rpc_reg_nwl_op_hndlrs();
#endif
#ifndef NO_LOL_INIT
extern INT32 c_rpc_reg_lol_op_hndlrs();
#endif

extern INT32 c_rpc_reg_sm_op_hndlrs();
extern INT32 c_rpc_reg_sb_op_hndlrs();
extern INT32 c_rpc_reg_dt_op_hndlrs();
extern INT32 c_rpc_reg_pcl_op_hndlrs();
extern INT32 c_rpc_reg_iom_op_hndlrs();
#ifdef ANDROID
extern INT32 c_rpc_reg_acfg_op_hndlrs();
extern INT32 c_rpc_reg_brdcst_op_hndlrs();
extern INT32 c_rpc_reg_channel_op_hndlrs();
extern INT32 c_rpc_reg_input_op_hndlrs();
extern INT32 c_rpc_reg_scan_op_hndlrs();
extern INT32 c_rpc_reg_dvbc_scan_op_hndlrs();
extern INT32 c_rpc_reg_event_op_hndlrs();


#endif
extern INT32 c_rpc_reg_rrctx_op_hndlrs();
extern INT32 c_rpc_reg_tm_op_hndlrs();
extern INT32 c_rpc_reg_cm_op_hndlrs();
extern INT32 c_rpc_reg_dsm_op_hndlrs();
extern INT32 c_rpc_reg_cecm_op_hndlrs();
#ifdef MW_DVBS_DEV_ENABLE
#ifndef NO_SATL_INIT
extern INT32 c_rpc_reg_satl_op_hndlrs();
#endif
#endif
#ifndef NO_LST_MNGR_INIT
extern INT32 c_rpc_reg_lst_mngr_op_hndlrs();
#endif
extern INT32 c_rpc_reg_rm_op_hndlrs();
#ifdef MW_ISDB_SUPPORT
extern INT32 c_rpc_reg_df_mngr_op_hndlrs();
#endif
extern INT32 c_rpc_reg_psi_eng_op_hndlrs();
#ifdef ANDROID
extern INT32 c_rpc_reg_ci_op_hndlrs();
#endif
extern INT32 c_rpc_reg_chip_spec_op_hndlrs();

INT32 c_rpc_init_server()
{   
    c_rpc_init();

    
    /*RPC_REG_OP_HNDLR(rpc_pf_test);*/
    
    /* ... */
    c_rpc_reg_handle_op_hndlrs();     
    c_rpc_reg_svctx_op_hndlrs();
#ifdef ENABLE_MULTIMEDIA
    c_rpc_reg_mm_svctx_op_hndlrs();
    c_rpc_reg_mfmtrecg_op_hndlrs();
    c_rpc_reg_minfo_op_hndlrs();
    c_rpc_reg_midxbuld_op_hndlrs();
#ifndef ANDROID
    c_rpc_reg_img_op_hndlrs();
#endif
#endif
#ifdef MW_CAP_LOGO_SUPPORT
    c_rpc_reg_cap_op_hndlrs();
#endif
    c_rpc_reg_evctx_op_hndlrs();
    c_rpc_reg_flm_op_hndlrs();
    c_rpc_reg_svl_op_hndlrs();
    c_rpc_reg_tsl_op_hndlrs();
#ifdef RPC_ATSC_SI 
    c_rpc_reg_atsc_si_op_hndlrs();
#endif
#ifndef NO_NWL_INIT
    c_rpc_reg_nwl_op_hndlrs();
#endif
#ifndef NO_LOL_INIT
    c_rpc_reg_lol_op_hndlrs();
#endif
    c_rpc_reg_sm_op_hndlrs();
    c_rpc_reg_sb_op_hndlrs();
    c_rpc_reg_dt_op_hndlrs();
    c_rpc_reg_pcl_op_hndlrs();
    c_rpc_reg_iom_op_hndlrs();
#ifdef ANDROID    
    c_rpc_reg_acfg_op_hndlrs();
    c_rpc_reg_brdcst_op_hndlrs();
    c_rpc_reg_channel_op_hndlrs();
    c_rpc_reg_input_op_hndlrs();    
    c_rpc_reg_scan_op_hndlrs();    
    c_rpc_reg_dvbc_scan_op_hndlrs();
    c_rpc_reg_event_op_hndlrs();
#endif
    c_rpc_reg_rrctx_op_hndlrs();
    c_rpc_reg_tm_op_hndlrs();
    c_rpc_reg_cm_op_hndlrs();
    c_rpc_reg_dsm_op_hndlrs();
    c_rpc_reg_cecm_op_hndlrs();
#ifdef MW_DVBS_DEV_ENABLE
#ifndef NO_SATL_INIT
    c_rpc_reg_satl_op_hndlrs();
#endif
#endif
#ifndef NO_LST_MNGR_INIT
    c_rpc_reg_lst_mngr_op_hndlrs();
#endif
    c_rpc_reg_rm_op_hndlrs();
#ifdef MW_ISDB_SUPPORT
    c_rpc_reg_df_mngr_op_hndlrs();
#endif
    c_rpc_reg_psi_eng_op_hndlrs();
#ifdef ANDROID
    c_rpc_reg_ci_op_hndlrs();
#endif
    c_rpc_reg_chip_spec_op_hndlrs();

    return rpc_open_server("DTVService");
}
