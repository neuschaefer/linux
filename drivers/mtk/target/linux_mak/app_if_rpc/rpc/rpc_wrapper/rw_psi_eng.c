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
#include "tbl_mngr/psi_eng/x_psi_eng.h"
#include "tbl_mngr/psi_eng/c_psi_eng.h"
#include "tbl_mngr/tm_hndlr.h"

#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"

#ifdef PSI_ENG_NEED_REG_TM_NFY_CB
static INT32 _hndlr_x_tm_nfy_fct(
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
    
    pt_return->e_type   = ARG_TYPE_INT32;
    
    pt_return->u.i4_arg = ((x_tm_nfy_fct)pv_cb_addr)(
            (HANDLE_T)pt_args[0].u.ui4_arg,
       (HANDLE_TYPE_T)pt_args[1].u.ui2_arg,
           (TM_COND_T)pt_args[2].u.i4_arg,
                      pt_args[3].u.pv_arg,
                      pt_args[4].u.ui4_arg);
                      
    return RPCR_OK;
}
#endif

INT32 c_psi_get_raw_section(HANDLE_T h_tbl,
                            UINT8 ui1_sec_num,
                            UINT8 * pui1_buffer,
                            SIZE_T * pz_len,
                            TM_COND_T * pe_cond)
{
    RPC_DECL(5, INT32);

    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pui1_buffer, *pz_len));
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_tbl);
    RPC_ARG_INP(ARG_TYPE_UINT8, ui1_sec_num);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pui1_buffer);
    RPC_ARG_IO(ARG_TYPE_REF_SIZE_T, pz_len);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_get_raw_section");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

INT32 c_psi_get_svc_entry(HANDLE_T h_pat,
                          UINT16 ui2_idx,
                          PSI_PID_SVC_ID_INFO_T * pt_pid_svc_info,
                          TM_COND_T * pe_cond)
{
    RPC_DECL(4, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_pid_svc_info, RPC_DESC_PSI_PID_SVC_ID_INFO_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32, h_pat);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_pid_svc_info);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_get_svc_entry");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

INT32 c_psi_get_ts_id(HANDLE_T h_pat,
                      UINT16 * pui2_ts_id,
                      TM_COND_T * pe_cond)
{
    RPC_DECL(3, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_pat);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16, pui2_ts_id);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_get_ts_id");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

INT32 c_psi_get_pcr_pid(HANDLE_T h_pmt,
                        MPEG_2_PID_T * pt_pcr_pid,
                        TM_COND_T * pe_cond)
{
    RPC_DECL(3, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_pmt);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16, pt_pcr_pid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_get_pcr_pid");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

INT32 c_psi_get_stream_entry(HANDLE_T h_pmt,
                             UINT16 ui2_idx,
                             PSI_STREAM_INFO_T * pt_stream_entry,
                             TM_COND_T * pe_cond)
{
    RPC_DECL(4, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_stream_entry, RPC_DESC_PSI_STREAM_INFO_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32, h_pmt);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_stream_entry);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_get_stream_entry");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

INT32 c_psi_get_svc_id(HANDLE_T h_pmt,
                       UINT16 * pui2_svc_id,
                       TM_COND_T * pe_cond)
{
    RPC_DECL(3, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_pmt);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16, pui2_svc_id);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_get_svc_id");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

INT32 c_psi_mon_get_num_ts_id(HANDLE_T h_mon_pat,
                              UINT16 * pui2_num_ts_id,
                              TM_COND_T * pe_cond)
{
    RPC_DECL(3, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_mon_pat);
    RPC_ARG_INP(ARG_TYPE_REF_UINT16, pui2_num_ts_id);
    RPC_ARG_INP(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_mon_get_num_ts_id");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

INT32 c_psi_mon_get_ts_id(HANDLE_T h_mon_pat,
                          UINT16 ui2_idx,
                          UINT16 * pui2_ts_id,
                          TM_COND_T * pe_cond)
{
    RPC_DECL(4, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_mon_pat);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16, pui2_ts_id);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_mon_get_ts_id");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

static INT32 _x_psi_load_tsdt(HANDLE_T h_root,
                              TM_SRC_TYPE_T e_src_type,
                              VOID * pv_src_info,
                              VOID * pv_nfy_tag,
                              x_tm_nfy_fct pf_nfy,
                              HANDLE_T * ph_tsdt,
                              TM_COND_T * pe_cond)
{
    RPC_DECL(7, INT32);

    switch (e_src_type)
    {
        case TM_SRC_TYPE_CONN_HANDLE:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 3, ARG_TYPE_UINT32));
            break;
        case TM_SRC_TYPE_MPEG_2_BRDCST:
            RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pv_src_info));
            break;
        case TM_SRC_TYPE_MAP:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 3, ARG_TYPE_UINT32));
        default:
            RI_LOG("x_psi_load_tsdt specify unkown or unused type %d, 0x%x\n", e_src_type, pv_src_info);
            break;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32, h_root);
    RPC_ARG_INP(ARG_TYPE_UINT8, e_src_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE, pv_src_info);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_tsdt);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_load_tsdt");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

INT32 c_psi_load_tsdt(HANDLE_T h_root,
                      TM_SRC_TYPE_T e_src_type,
                      VOID * pv_src_info,
                      VOID * pv_nfy_tag,
                      x_tm_nfy_fct pf_nfy,
                      HANDLE_T * ph_tsdt,
                      TM_COND_T * pe_cond)
{
    return _x_psi_load_tsdt(h_root, e_src_type, pv_src_info,
                            pv_nfy_tag, pf_nfy, ph_tsdt, pe_cond);
}

static INT32 _x_psi_load_cat(HANDLE_T h_root,
                             TM_SRC_TYPE_T e_src_type,
                             VOID * pv_src_info,
                             VOID * pv_nfy_tag,
                             x_tm_nfy_fct pf_nfy,
                             HANDLE_T * ph_cat,
                             TM_COND_T * pe_cond)
{
    RPC_DECL(7, INT32);

    switch (e_src_type)
    {
        case TM_SRC_TYPE_CONN_HANDLE:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 3, ARG_TYPE_UINT32));
            break;
        case TM_SRC_TYPE_MPEG_2_BRDCST:
            RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pv_src_info));
            break;
        case TM_SRC_TYPE_MAP:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 3, ARG_TYPE_UINT32));
        default:
            RI_LOG("x_psi_load_tsdt specify unkown or unused type %d, 0x%x\n", e_src_type, pv_src_info);
            break;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32, h_root);
    RPC_ARG_INP(ARG_TYPE_UINT8, e_src_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE, pv_src_info);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_cat);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_load_cat");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

INT32 c_psi_load_cat(HANDLE_T h_root,
                     TM_SRC_TYPE_T e_src_type,
                     VOID * pv_src_info,
                     VOID * pv_nfy_tag,
                     x_tm_nfy_fct pf_nfy,
                     HANDLE_T * ph_cat,
                     TM_COND_T * pe_cond)
{
    return _x_psi_load_cat(h_root, e_src_type, pv_src_info,
                           pv_nfy_tag, pf_nfy, ph_cat, pe_cond);
}

static INT32 _x_psi_load_pat(HANDLE_T h_root,
                             TM_SRC_TYPE_T e_src_type,
                             VOID * pv_src_info,
                             VOID * pv_nfy_tag,
                             x_tm_nfy_fct pf_nfy,
                             HANDLE_T * ph_pat,
                             TM_COND_T * pe_cond)
{
    RPC_DECL(7, INT32);

    switch (e_src_type)
    {
        case TM_SRC_TYPE_CONN_HANDLE:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 3, ARG_TYPE_UINT32));
            break;
        case TM_SRC_TYPE_MPEG_2_BRDCST:
            RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pv_src_info));
            break;
        case TM_SRC_TYPE_MAP:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 3, ARG_TYPE_UINT32));
        default:
            RI_LOG("x_psi_load_tsdt specify unkown or unused type %d, 0x%x\n", e_src_type, pv_src_info);
            break;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32, h_root);
    RPC_ARG_INP(ARG_TYPE_UINT8, e_src_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE, pv_src_info);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_pat);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_load_pat");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

INT32 c_psi_load_pat(HANDLE_T h_root,
                     TM_SRC_TYPE_T e_src_type,
                     VOID * pv_src_info,
                     VOID * pv_nfy_tag,
                     x_tm_nfy_fct pf_nfy,
                     HANDLE_T * ph_pat,
                     TM_COND_T * pe_cond)
{
    return _x_psi_load_pat(h_root, e_src_type, pv_src_info,
                           pv_nfy_tag, pf_nfy, ph_pat, pe_cond);
}

static INT32 _x_psi_load_pat_by_ts_id(HANDLE_T h_root,
                                      UINT16 ui2_ts_id,
                                      TM_SRC_TYPE_T e_src_type,
                                      VOID * pv_src_info,
                                      VOID * pv_nfy_tag,
                                      x_tm_nfy_fct pf_nfy,
                                      HANDLE_T * ph_pat,
                                      TM_COND_T * pe_cond)
{
    RPC_DECL(8, INT32);

    switch (e_src_type)
    {
        case TM_SRC_TYPE_CONN_HANDLE:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 3, ARG_TYPE_UINT32));
            break;
        case TM_SRC_TYPE_MPEG_2_BRDCST:
            RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pv_src_info));
            break;
        case TM_SRC_TYPE_MAP:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 3, ARG_TYPE_UINT32));
        default:
            RI_LOG("x_psi_load_tsdt specify unkown or unused type %d, 0x%x\n", e_src_type, pv_src_info);
            break;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32, h_root);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_ts_id);
    RPC_ARG_INP(ARG_TYPE_UINT8, e_src_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE, pv_src_info);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_pat);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_load_pat_by_ts_id");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

INT32 c_psi_load_pat_by_ts_id(HANDLE_T h_root,
                              UINT16 ui2_ts_id,
                              TM_SRC_TYPE_T e_src_type,
                              VOID * pv_src_info,
                              VOID * pv_nfy_tag,
                              x_tm_nfy_fct pf_nfy,
                              HANDLE_T * ph_pat,
                              TM_COND_T * pe_cond)
{
    return _x_psi_load_pat_by_ts_id(h_root, ui2_ts_id, e_src_type, pv_src_info,
                                    pv_nfy_tag, pf_nfy, ph_pat, pe_cond);
}

static INT32 _x_psi_load_pat_by_ts_descr(HANDLE_T h_root,
                                 TS_DESCR_T * pt_ts_descr,
                                 TM_SRC_TYPE_T e_src_type,
                                 VOID * pv_src_info,
                                 VOID * pv_nfy_tag,
                                 x_tm_nfy_fct pf_nfy,
                                 HANDLE_T * ph_pat,
                                 TM_COND_T * pe_cond)
{
    RPC_DECL(8, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_ts_descr, RPC_DESC_TS_DESCR_T, NULL));

    switch (e_src_type)
    {
        case TM_SRC_TYPE_CONN_HANDLE:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 3, ARG_TYPE_UINT32));
            break;
        case TM_SRC_TYPE_MPEG_2_BRDCST:
            RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pv_src_info));
            break;
        case TM_SRC_TYPE_MAP:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 3, ARG_TYPE_UINT32));
        default:
            RI_LOG("x_psi_load_tsdt specify unkown or unused type %d, 0x%x\n", e_src_type, pv_src_info);
            break;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32, h_root);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_ts_descr);
    RPC_ARG_INP(ARG_TYPE_UINT8, e_src_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE, pv_src_info);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_pat);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_load_pat_by_ts_descr");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

INT32 c_psi_load_pat_by_ts_descr(HANDLE_T h_root,
                                 TS_DESCR_T * pt_ts_descr,
                                 TM_SRC_TYPE_T e_src_type,
                                 VOID * pv_src_info,
                                 VOID * pv_nfy_tag,
                                 x_tm_nfy_fct pf_nfy,
                                 HANDLE_T * ph_pat,
                                 TM_COND_T * pe_cond)
{
    return _x_psi_load_pat_by_ts_descr(h_root, pt_ts_descr, e_src_type,
                                       pv_src_info, pv_nfy_tag, pf_nfy,
                                       ph_pat, pe_cond);
}

static INT32 _x_psi_load_pmt_by_svc_id(HANDLE_T h_pat,
                                       UINT16 ui2_svc_id,
                                       TM_SRC_TYPE_T e_src_type,
                                       VOID * pv_src_info,
                                       VOID * pv_nfy_tag,
                                       x_tm_nfy_fct pf_nfy,
                                       HANDLE_T * ph_pmt,
                                       TM_COND_T * pe_cond)
{
    RPC_DECL(8, INT32);

    switch (e_src_type)
    {
        case TM_SRC_TYPE_CONN_HANDLE:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 3, ARG_TYPE_UINT32));
            break;
        case TM_SRC_TYPE_MPEG_2_BRDCST:
            RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pv_src_info));
            break;
        case TM_SRC_TYPE_MAP:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 3, ARG_TYPE_UINT32));
        default:
            RI_LOG("x_psi_load_tsdt specify unkown or unused type %d, 0x%x\n", e_src_type, pv_src_info);
            break;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32, h_pat);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_svc_id);
    RPC_ARG_INP(ARG_TYPE_UINT8, e_src_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE, pv_src_info);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_pmt);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_load_pmt_by_svc_id");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

INT32 c_psi_load_pmt_by_svc_id(HANDLE_T h_pat,
                               UINT16 ui2_svc_id,
                               TM_SRC_TYPE_T e_src_type,
                               VOID * pv_src_info,
                               VOID * pv_nfy_tag,
                               x_tm_nfy_fct pf_nfy,
                               HANDLE_T * ph_pmt,
                               TM_COND_T * pe_cond)
{
    return _x_psi_load_pmt_by_svc_id(h_pat, ui2_svc_id, e_src_type, pv_src_info,
                                     pv_nfy_tag, pf_nfy, ph_pmt, pe_cond);
}

static INT32 _x_psi_load_pmt_by_idx(HANDLE_T h_pat,
                                    UINT16 ui2_idx,
                                    TM_SRC_TYPE_T e_src_type,
                                    VOID * pv_src_info,
                                    VOID * pv_nfy_tag,
                                    x_tm_nfy_fct pf_nfy,
                                    HANDLE_T * ph_pmt,
                                    TM_COND_T * pe_cond)
{
    RPC_DECL(8, INT32);

    switch (e_src_type)
    {
        case TM_SRC_TYPE_CONN_HANDLE:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 3, ARG_TYPE_UINT32));
            break;
        case TM_SRC_TYPE_MPEG_2_BRDCST:
            RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pv_src_info));
            break;
        case TM_SRC_TYPE_MAP:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 3, ARG_TYPE_UINT32));
        default:
            RI_LOG("x_psi_load_tsdt specify unkown or unused type %d, 0x%x\n", e_src_type, pv_src_info);
            break;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32, h_pat);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_idx);
    RPC_ARG_INP(ARG_TYPE_UINT8, e_src_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE, pv_src_info);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_pmt);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_load_pmt_by_idx");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

INT32 c_psi_load_pmt_by_idx(HANDLE_T h_pat,
                            UINT16 ui2_idx,
                            TM_SRC_TYPE_T e_src_type,
                            VOID * pv_src_info,
                            VOID * pv_nfy_tag,
                            x_tm_nfy_fct pf_nfy,
                            HANDLE_T * ph_pmt,
                            TM_COND_T * pe_cond)
{
    return _x_psi_load_pmt_by_idx(h_pat, ui2_idx, e_src_type, pv_src_info,
                                  pv_nfy_tag, pf_nfy, ph_pmt, pe_cond);
}

static INT32 _x_psi_mon_pat(HANDLE_T h_root,
                            TM_SRC_TYPE_T e_src_type,
                            VOID * pv_src_info,
                            VOID * pv_nfy_tag,
                            x_tm_nfy_fct pf_nfy,
                            HANDLE_T * ph_mon_pat,
                            TM_COND_T * pe_cond)
{
    RPC_DECL(7, INT32);

    switch (e_src_type)
    {
        case TM_SRC_TYPE_CONN_HANDLE:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 3, ARG_TYPE_UINT32));
            break;
        case TM_SRC_TYPE_MPEG_2_BRDCST:
            RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pv_src_info));
            break;
        case TM_SRC_TYPE_MAP:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 3, ARG_TYPE_UINT32));
        default:
            RI_LOG("x_psi_load_tsdt specify unkown or unused type %d, 0x%x\n", e_src_type, pv_src_info);
            break;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32, h_root);
    RPC_ARG_INP(ARG_TYPE_UINT8, e_src_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE, pv_src_info);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_mon_pat);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_mon_pat");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

INT32 c_psi_mon_pat(HANDLE_T h_root,
                    TM_SRC_TYPE_T e_src_type,
                    VOID * pv_src_info,
                    VOID * pv_nfy_tag,
                    x_tm_nfy_fct pf_nfy,
                    HANDLE_T * ph_mon_pat,
                    TM_COND_T * pe_cond)
{
    return _x_psi_mon_pat(h_root, e_src_type, pv_src_info,
                          pv_nfy_tag, pf_nfy, ph_mon_pat, pe_cond);
}

INT32 c_psi_set_pat_pmt_info(HANDLE_T h_pat,
                             PSI_PID_SVC_ID_INFO_T * pt_pid_svc_info,
                             TM_COND_T * pe_cond)
{
    RPC_DECL(3, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_pid_svc_info, RPC_DESC_PSI_PID_SVC_ID_INFO_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32, h_pat);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_pid_svc_info);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_set_pat_pmt_info");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

INT32 c_psi_set_available(HANDLE_T h_tbl,
                          BOOL b_available,
                          TM_COND_T * pe_cond)
{
    RPC_DECL(3, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_tbl);
    RPC_ARG_INP(ARG_TYPE_BOOL, b_available);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_set_available");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

INT32 c_psi_set_version_num(HANDLE_T h_tbl,
                            UINT8 ui1_version,
                            TM_COND_T * pe_cond)
{
    RPC_DECL(3, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_tbl);
    RPC_ARG_INP(ARG_TYPE_UINT8, ui1_version);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_set_version_num");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

INT32 c_psi_set_pmt_pcr_pid(HANDLE_T h_pmt,
                            MPEG_2_PID_T t_pid,
                            TM_COND_T * pe_cond)
{
    RPC_DECL(3, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_pmt);
    RPC_ARG_INP(ARG_TYPE_UINT16, t_pid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_set_pmt_pcr_pid");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

INT32 c_psi_set_pmt_stream_entry(HANDLE_T h_pmt,
                                 PSI_STREAM_INFO_T * pt_stream_info,
                                 TM_COND_T * pe_cond)
{
    RPC_DECL(3, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_stream_info, RPC_DESC_PSI_STREAM_INFO_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32, h_pmt);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_stream_info);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_set_pmt_stream_entry");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

INT32 c_psi_set_pmt_stream_descr(HANDLE_T h_pmt,
                                 UINT8 * pui1_descr,
                                 SIZE_T z_descr_len,
                                 TM_COND_T * pe_cond)
{
    RPC_DECL(4, INT32);

    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pui1_descr, z_descr_len));

    RPC_ARG_INP(ARG_TYPE_UINT32, h_pmt);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pui1_descr);
    RPC_ARG_INP(ARG_TYPE_SIZE_T, z_descr_len);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_set_pmt_stream_descr");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

INT32 c_psi_set_pmt_main_descr(HANDLE_T h_pmt,
                               UINT8 * pui1_descr,
                               SIZE_T z_descr_len,
                               TM_COND_T * pe_cond)
{
    RPC_DECL(4, INT32);

    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pui1_descr, z_descr_len));

    RPC_ARG_INP(ARG_TYPE_UINT32, h_pmt);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pui1_descr);
    RPC_ARG_INP(ARG_TYPE_SIZE_T, z_descr_len);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_psi_set_pmt_main_descr");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

INT32 c_rpc_reg_psi_eng_cb_hndlrs()
{
#ifdef PSI_ENG_NEED_REG_TM_NFY_CB
    RPC_REG_CB_HNDLR(x_tm_nfy_fct);    
#endif

    return RPCR_OK;
}
