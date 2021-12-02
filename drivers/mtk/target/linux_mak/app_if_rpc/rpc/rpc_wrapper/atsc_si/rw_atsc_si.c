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
 *         This file contains all ATSC-SI engine c_* version API implementations.
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/
#include "handle/handle.h"
#include "tbl_mngr/atsc_si_eng/x_atsc_si_eng.h"
#include "tbl_mngr/atsc_si_eng/c_atsc_si_eng.h"
#include "tbl_mngr/tm_hndlr.h"

#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"

/*-----------------------------------------------------------------------------
                    functions implementation
 ----------------------------------------------------------------------------*/
/* Replace with tm register cb */
#if 0
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
    
       ((x_tm_nfy_fct)pv_cb_addr)(
            (HANDLE_T)pt_args[0].u.ui4_arg,
       (HANDLE_TYPE_T)pt_args[1].u.ui2_arg,
           (TM_COND_T)pt_args[2].u.i4_arg,
                      pt_args[3].u.pv_arg,
                      pt_args[4].u.ui4_arg);
                      
    return RPCR_OK;
}
#endif

static INT32 _x_atsc_si_txt_loc_vct_sname ( HANDLE_T   h_vct,
                                            UINT16     ui2_idx,
                                            HANDLE_T*  ph_txt)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_vct);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_txt);

    RPC_DO_OP("x_atsc_si_txt_loc_vct_sname");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);    
}

static INT32 _x_atsc_si_txt_loc_rrt_region( HANDLE_T   h_rrt,
                                            HANDLE_T*  ph_txt)
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_rrt);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_txt);
    
    RPC_DO_OP("x_atsc_si_txt_loc_rrt_region");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG); 
}

static INT32 _x_atsc_si_txt_loc_rrt_dim (HANDLE_T   h_rrt,
                                         UINT16     ui2_dim_idx,
                                         HANDLE_T*  ph_txt)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_rrt);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_dim_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_txt);

    RPC_DO_OP("x_atsc_si_txt_loc_rrt_dim");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);     
}

static INT32 _x_atsc_si_txt_loc_rrt_abrv_rating (HANDLE_T   h_rrt,
                                                 UINT16     ui2_dim_idx,
                                                 UINT16     ui2_val_idx,
                                                 HANDLE_T*  ph_txt)
{
    RPC_DECL(4, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_rrt);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_dim_idx);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_val_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_txt);

    RPC_DO_OP("x_atsc_si_txt_loc_rrt_abrv_rating");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);       
}

static INT32 _x_atsc_si_txt_loc_rrt_rating (HANDLE_T   h_rrt,
                                            UINT16     ui2_dim_idx,
                                            UINT16     ui2_val_idx,
                                            HANDLE_T*  ph_txt)
{
    RPC_DECL(4, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_rrt);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_dim_idx);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_val_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_txt);

    RPC_DO_OP("x_atsc_si_txt_loc_rrt_rating");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);      
}

static INT32 _x_atsc_si_txt_loc_eit_title ( HANDLE_T   h_eit,
                                            UINT16     ui2_idx,
                                            HANDLE_T*  ph_txt)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_eit);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_txt);

    RPC_DO_OP("x_atsc_si_txt_loc_eit_title");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);        
}

static INT32 _x_atsc_si_txt_loc_ett_ext_text (HANDLE_T   h_ett,
                                              HANDLE_T*  ph_txt)
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_ett);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_txt);

    RPC_DO_OP("x_atsc_si_txt_loc_ett_ext_text");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);     
}

static INT32 _x_atsc_si_txt_loc_descr (HANDLE_T      h_obj,
                                       const UINT8*  pui1_multi_lang_txt,
                                       HANDLE_T*     ph_txt)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_obj);
    RPC_ARG_INP(ARG_TYPE_REF_UINT8,     pui1_multi_lang_txt);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_txt);

    RPC_DO_OP("x_atsc_si_txt_loc_descr");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);        
}

static INT32 _x_atsc_si_txt_loc_eas_nat_of_act (HANDLE_T     h_eas,
                                                HANDLE_T*    ph_txt)
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_eas);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_txt);

    RPC_DO_OP("x_atsc_si_txt_loc_eas_nat_of_act");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

static INT32 _x_atsc_si_txt_loc_eas_alert ( HANDLE_T  h_eas,
                                            HANDLE_T* ph_txt)
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_eas);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_txt);

    RPC_DO_OP("x_atsc_si_txt_loc_eas_alert");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

static INT32 _x_atsc_si_load_stt (HANDLE_T       h_root,
                                  VOID*          pv_nfy_tag,
                                  x_tm_nfy_fct   pf_nfy,
                                  HANDLE_T*      ph_stt,
                                  TM_COND_T*     pe_cond)
{
    RPC_DECL(5, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_root);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,      pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,      pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_stt);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_load_stt");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);    
}

static INT32 _x_atsc_si_load_mgt_by_ts_id (HANDLE_T       h_root,
                                           UINT16         ui2_ts_id,
                                           TM_SRC_TYPE_T  e_src_type,
                                           VOID*          pv_src_info,
                                           VOID*          pv_nfy_tag,
                                           x_tm_nfy_fct   pf_nfy,
                                           HANDLE_T*      ph_mgt,
                                           TM_COND_T*     pe_cond)
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
            RI_LOG("x_atsc_si_load_mgt_by_ts_id specify unkown or unused type %d, 0x%x\n", e_src_type, pv_src_info);
            break;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32,        h_root);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_ts_id);
    RPC_ARG_INP(ARG_TYPE_UINT8,         e_src_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE,      pv_src_info);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,      pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,      pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_mgt);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_load_mgt_by_ts_id");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG); 
}

static INT32 _x_atsc_si_load_mgt_by_ts_descr (HANDLE_T       h_root,
                                              TS_DESCR_T*    pt_ts_descr,
                                              TM_SRC_TYPE_T  e_src_type,
                                              VOID*          pv_src_info,
                                              VOID*          pv_nfy_tag,
                                              x_tm_nfy_fct   pf_nfy,
                                              HANDLE_T*      ph_mgt,
                                              TM_COND_T*     pe_cond)
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
            RI_LOG("x_atsc_si_load_mgt_by_ts_descr specify unkown or unused type %d, 0x%x\n", e_src_type, pv_src_info);
            break;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32,        h_root);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,      pt_ts_descr);
    RPC_ARG_INP(ARG_TYPE_UINT8,         e_src_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE,      pv_src_info);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,      pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,      pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_mgt);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_load_mgt_by_ts_descr");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);     
}

static INT32 _x_atsc_si_load_cvct (HANDLE_T       h_mgt,
                                   TM_SRC_TYPE_T  e_src_type,
                                   VOID*          pv_src_info,
                                   VOID*          pv_nfy_tag,
                                   x_tm_nfy_fct   pf_nfy,
                                   HANDLE_T*      ph_cvct,
                                   TM_COND_T*     pe_cond)
{
    RPC_DECL(7, INT32);    

    switch (e_src_type)
    {
        case TM_SRC_TYPE_CONN_HANDLE:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 2, ARG_TYPE_UINT32));
            break;
        case TM_SRC_TYPE_MPEG_2_BRDCST:
            RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pv_src_info));
            break;
        case TM_SRC_TYPE_MAP:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 2, ARG_TYPE_UINT32));
        default:
            RI_LOG("x_atsc_si_load_cvct specify unkown or unused type %d, 0x%x\n", e_src_type, pv_src_info);
            break;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32,        h_mgt);
    RPC_ARG_INP(ARG_TYPE_UINT8,         e_src_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE,      pv_src_info);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,      pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,      pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_cvct);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_load_cvct");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);      
}

static INT32 _x_atsc_si_load_tvct (HANDLE_T       h_mgt,
                                   TM_SRC_TYPE_T  e_src_type,
                                   VOID*          pv_src_info,
                                   VOID*          pv_nfy_tag,
                                   x_tm_nfy_fct   pf_nfy,
                                   HANDLE_T*      ph_tvct,
                                   TM_COND_T*     pe_cond)
{
    RPC_DECL(7, INT32);    

    switch (e_src_type)
    {
        case TM_SRC_TYPE_CONN_HANDLE:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 2, ARG_TYPE_UINT32));
            break;
        case TM_SRC_TYPE_MPEG_2_BRDCST:
            RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pv_src_info));
            break;
        case TM_SRC_TYPE_MAP:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 2, ARG_TYPE_UINT32));
        default:
            RI_LOG("x_atsc_si_load_tvct specify unkown or unused type %d, 0x%x\n", e_src_type, pv_src_info);
            break;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32,        h_mgt);
    RPC_ARG_INP(ARG_TYPE_UINT8,         e_src_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE,      pv_src_info);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,      pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,      pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_tvct);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_load_tvct");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);      
}

static INT32 _x_atsc_si_load_rrt (HANDLE_T       h_mgt,
                                  UINT8          ui1_region,
                                  TM_SRC_TYPE_T  e_src_type,
                                  VOID*          pv_src_info,
                                  VOID*          pv_nfy_tag,
                                  x_tm_nfy_fct   pf_nfy,
                                  HANDLE_T*      ph_rrt,
                                  TM_COND_T*     pe_cond)
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
            RI_LOG("x_atsc_si_load_rrt specify unkown or unused type %d, 0x%x\n", e_src_type, pv_src_info);
            break;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32,        h_mgt);
    RPC_ARG_INP(ARG_TYPE_UINT8,         ui1_region);
    RPC_ARG_INP(ARG_TYPE_UINT8,         e_src_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE,      pv_src_info);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,      pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,      pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_rrt);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_load_rrt");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);     
}

static INT32 _x_atsc_si_load_eit_by_idx (HANDLE_T       h_vct,
                                         UINT16         ui2_idx,
                                         TIME_T         t_time,
                                         TM_SRC_TYPE_T  e_src_type,
                                         VOID*          pv_src_info,
                                         VOID*          pv_nfy_tag,
                                         x_tm_nfy_fct   pf_nfy,
                                         HANDLE_T*      ph_eit,
                                         TM_COND_T*     pe_cond)
{
    RPC_DECL(9, INT32);    

    switch (e_src_type)
    {
        case TM_SRC_TYPE_CONN_HANDLE:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 4, ARG_TYPE_UINT32));
            break;
        case TM_SRC_TYPE_MPEG_2_BRDCST:
            RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pv_src_info));
            break;
        case TM_SRC_TYPE_MAP:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 4, ARG_TYPE_UINT32));
        default:
            RI_LOG("x_atsc_si_load_eit_by_idx specify unkown or unused type %d, 0x%x\n", e_src_type, pv_src_info);
            break;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32,        h_vct);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_idx);
    RPC_ARG_INP(ARG_TYPE_INT64,         t_time);
    RPC_ARG_INP(ARG_TYPE_UINT8,         e_src_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE,      pv_src_info);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,      pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,      pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_eit);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_load_eit_by_idx");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);        
}

static INT32 _x_atsc_si_load_eit_by_src_id (HANDLE_T       h_obj,
                                            UINT16         ui2_src_id,
                                            TIME_T         t_time,
                                            TM_SRC_TYPE_T  e_src_type,
                                            VOID*          pv_src_info,
                                            VOID*          pv_nfy_tag,
                                            x_tm_nfy_fct   pf_nfy,
                                            HANDLE_T*      ph_eit,
                                            TM_COND_T*     pe_cond)
{
    RPC_DECL(9, INT32);    

    switch (e_src_type)
    {
        case TM_SRC_TYPE_CONN_HANDLE:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 4, ARG_TYPE_UINT32));
            break;
        case TM_SRC_TYPE_MPEG_2_BRDCST:
            RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pv_src_info));
            break;
        case TM_SRC_TYPE_MAP:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 4, ARG_TYPE_UINT32));
        default:
            RI_LOG("x_atsc_si_load_eit_by_src_id specify unkown or unused type %d, 0x%x\n", e_src_type, pv_src_info);
            break;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32,        h_obj);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_src_id);
    RPC_ARG_INP(ARG_TYPE_INT64,         t_time);
    RPC_ARG_INP(ARG_TYPE_UINT8,         e_src_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE,      pv_src_info);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,      pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,      pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_eit);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_load_eit_by_src_id");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);        
}

static INT32 _x_atsc_si_load_eit_0_by_idx (HANDLE_T       h_vct,
                                           UINT16         ui2_idx,
                                           TM_SRC_TYPE_T  e_src_type,
                                           VOID*          pv_src_info,
                                           VOID*          pv_nfy_tag,
                                           x_tm_nfy_fct   pf_nfy,
                                           HANDLE_T*      ph_eit,
                                           TM_COND_T*     pe_cond)
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
            RI_LOG("x_atsc_si_load_eit_0_by_idx specify unkown or unused type %d, 0x%x\n", e_src_type, pv_src_info);
            break;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32,        h_vct);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_idx);
    RPC_ARG_INP(ARG_TYPE_UINT8,         e_src_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE,      pv_src_info);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,      pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,      pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_eit);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_load_eit_0_by_idx");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);          
}

static INT32 _x_atsc_si_load_eit_0_by_src_id (HANDLE_T       h_obj,
                                              UINT16         ui2_src_id,
                                              TM_SRC_TYPE_T  e_src_type,
                                              VOID*          pv_src_info,
                                              VOID*          pv_nfy_tag,
                                              x_tm_nfy_fct   pf_nfy,
                                              HANDLE_T*      ph_eit,
                                              TM_COND_T*     pe_cond)
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
            RI_LOG("x_atsc_si_load_eit_0_by_src_id specify unkown or unused type %d, 0x%x\n", e_src_type, pv_src_info);
            break;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32,        h_obj);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_src_id);
    RPC_ARG_INP(ARG_TYPE_UINT8,         e_src_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE,      pv_src_info);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,      pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,      pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_eit);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_load_eit_0_by_src_id");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);      
}

static INT32 _x_atsc_si_load_ett_by_idx (HANDLE_T       h_obj,
                                         UINT16         ui2_idx,
                                         TM_SRC_TYPE_T  e_src_type,
                                         VOID*          pv_src_info,
                                         VOID*          pv_nfy_tag,
                                         x_tm_nfy_fct   pf_nfy,
                                         HANDLE_T*      ph_ett,
                                         TM_COND_T*     pe_cond)
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
            RI_LOG("x_atsc_si_load_ett_by_idx specify unkown or unused type %d, 0x%x\n", e_src_type, pv_src_info);
            break;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32,        h_obj);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_idx);
    RPC_ARG_INP(ARG_TYPE_UINT8,         e_src_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE,      pv_src_info);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,      pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,      pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_ett);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_load_ett_by_idx");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);       
}

static INT32 _x_atsc_si_load_ett_by_src_id (HANDLE_T       h_mgt,
                                            UINT16         ui2_src_id,
                                            TM_SRC_TYPE_T  e_src_type,
                                            VOID*          pv_src_info,
                                            VOID*          pv_nfy_tag,
                                            x_tm_nfy_fct   pf_nfy,
                                            HANDLE_T*      ph_ett,
                                            TM_COND_T*     pe_cond)
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
            RI_LOG("x_atsc_si_load_ett_by_src_id specify unkown or unused type %d, 0x%x\n", e_src_type, pv_src_info);
            break;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32,        h_mgt);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_src_id);
    RPC_ARG_INP(ARG_TYPE_UINT8,         e_src_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE,      pv_src_info);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,      pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,      pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_ett);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_load_ett_by_src_id");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);    
}

static INT32 _x_atsc_si_load_ett_by_src_evt_id (HANDLE_T       h_mgt,
                                                UINT16         ui2_src_id,
                                                UINT16         ui2_evt_id,
                                                TIME_T         t_time,
                                                TM_SRC_TYPE_T  e_src_type,
                                                VOID*          pv_src_info,
                                                VOID*          pv_nfy_tag,
                                                x_tm_nfy_fct   pf_nfy,
                                                HANDLE_T*      ph_ett,
                                                TM_COND_T*     pe_cond)
{
    RPC_DECL(10, INT32);    

    switch (e_src_type)
    {
        case TM_SRC_TYPE_CONN_HANDLE:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 5, ARG_TYPE_UINT32));
            break;
        case TM_SRC_TYPE_MPEG_2_BRDCST:
            RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pv_src_info));
            break;
        case TM_SRC_TYPE_MAP:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 5, ARG_TYPE_UINT32));
        default:
            RI_LOG("x_atsc_si_load_ett_by_src_evt_id specify unkown or unused type %d, 0x%x\n", e_src_type, pv_src_info);
            break;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32,        h_mgt);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_src_id);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_evt_id);
    RPC_ARG_INP(ARG_TYPE_INT64,         t_time);
    RPC_ARG_INP(ARG_TYPE_UINT8,         e_src_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE,      pv_src_info);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,      pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,      pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_ett);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_load_ett_by_src_evt_id");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);      
}

static INT32 _x_atsc_si_load_eas (HANDLE_T       h_root,
                                  VOID*          pv_nfy_tag,
                                  x_tm_nfy_fct   pf_nfy,
                                  HANDLE_T*      ph_eas,
                                  TM_COND_T*     pe_cond)
{
    RPC_DECL(5, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_root);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,      pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,      pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_eas);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_load_eas");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_tbl_version
 *
 * Description: This API returns a version number of the given table object.
 *
 * Inputs:  h_obj               References a table object.
 *
 * Outputs: pui1_tbl_version    Contains the table version number.
 *          pe_cond             Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_tbl_version (HANDLE_T    h_obj,
                                 UINT8*      pui1_tbl_version,
                                 TM_COND_T*  pe_cond)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_obj);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT8,     pui1_tbl_version);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_get_tbl_version");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_prot_version
 *
 * Description: This API returns a protocol version number of the given table 
 *              object.
 *
 * Inputs:  h_obj               References a table object.
 *
 * Outputs: pui1_prot_version   Contains the protocol version number.
 *          pe_cond             Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_prot_version (HANDLE_T    h_obj,
                                  UINT8*      pui1_prot_version,
                                  TM_COND_T*  pe_cond)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_obj);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT8,     pui1_prot_version);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_get_prot_version");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_num_mgt_entries
 *
 * Description: This API returns the number of entries of the given MGT table object.
 *
 * Inputs:  h_mgt           References an MGT table object.
 *
 * Outputs: pui2_num_entries    Contains the number of entries.
 *          pe_cond             Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_num_mgt_entries (HANDLE_T             h_mgt,
                                     UINT16*              pui2_num_entries,
                                     TM_COND_T*           pe_cond)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_mgt);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,    pui2_num_entries);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_get_num_mgt_entries");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_mgt_entry
 *
 * Description: This API returns an entry of the given MGT table object.
 *
 * Inputs:  h_mgt           References an MGT table object.
 *          ui2_idx         Specifies the table_type entry by index.
 *
 * Outputs: pt_mgt_info     Contains the MGT entry structure.
 *          pe_cond         Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_INV_GET_INFO      Invalid get operation data.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_mgt_entry (HANDLE_T             h_mgt,
                               UINT16               ui2_idx,
                               ATSC_SI_MGT_INFO_T*  pt_mgt_info,
                               TM_COND_T*           pe_cond)
{
    RPC_DECL(4, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_mgt_info, RPC_DESC_ATSC_SI_MGT_INFO_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_mgt);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,      pt_mgt_info);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_get_mgt_entry");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_vct_ts_id
 *
 * Description: This API returns TS id of the given VCT table object.
 *
 * Inputs:  h_vct           References a VCT table object.
 *
 * Outputs: pui2_ts_id      Contains the transport steam id.
 *          pe_cond         Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_vct_ts_id (HANDLE_T    h_vct,
                               UINT16*     pui2_ts_id,
                               TM_COND_T*  pe_cond)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_vct);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,    pui2_ts_id);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_get_vct_ts_id");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_num_vct_entries
 *
 * Description: This API returns the number of entries of the given VCT table object.
 *
 * Inputs:  h_vct           References a VCT table object.
 *
 * Outputs: pui2_num_entries    Contains the number of entries.
 *          pe_cond             Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_num_vct_entries (HANDLE_T             h_vct,
                                     UINT16*              pui2_num_entries,
                                     TM_COND_T*           pe_cond)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_vct);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,    pui2_num_entries);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_get_num_vct_entries");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_vct_entry
 *
 * Description: This API returns a channel entry of the given VCT table object.
 *
 * Inputs:  h_vct           References a VCT table object.
 *          ui2_idx         Specifies the virtual channel entry by index.
 *
 * Outputs: pt_vct_info     Contains the VCT entry structure.
 *          pe_cond         Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_INV_GET_INFO      Invalid get operation data.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_vct_entry (HANDLE_T             h_vct,
                               UINT16               ui2_idx,
                               ATSC_SI_VCT_INFO_T*  pt_vct_info,
                               TM_COND_T*           pe_cond)
{
    RPC_DECL(4, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_vct_info, RPC_DESC_ATSC_SI_VCT_INFO_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_vct);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,      pt_vct_info);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_get_vct_entry");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_rrt_region
 *
 * Description: This API returns region id of the given RRT table object.
 *
 * Inputs:  h_rrt           References an RRT table object.
 *
 * Outputs: pui1_region     Contains the region id.
 *          pe_cond         Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_rrt_region (HANDLE_T    h_rrt,
                                UINT8*      pui1_region,
                                TM_COND_T*  pe_cond)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_rrt);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT8,     pui1_region);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_get_rrt_region");
 
    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_rrt_entry
 *
 * Description: This API returns a dimension entry of the given RRT table object.
 *
 * Inputs:  h_rrt           References an RRT table object.
 *          ui2_idx         Specifies the rating dimension by index.
 *
 * Outputs: pt_rrt_info     Contains the RRT entry structure.
 *          pe_cond         Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_INV_GET_INFO      Invalid get operation data.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_rrt_entry (HANDLE_T             h_rrt,
                               UINT16               ui2_idx,
                               ATSC_SI_RRT_INFO_T*  pt_rrt_info,
                               TM_COND_T*           pe_cond)
{
    RPC_DECL(4, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_rrt_info, RPC_DESC_ATSC_SI_RRT_INFO_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_rrt);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,      pt_rrt_info);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_get_rrt_entry");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_eit_src_id
 *
 * Description: This API returns source id of the given EIT table object.
 *
 * Inputs:  h_eit           References an EIT table object.
 *
 * Outputs: pui2_src_id     Contains the channel source id.
 *          pe_cond         Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_eit_src_id (HANDLE_T    h_eit,
                                UINT16*     pui2_src_id,
                                TM_COND_T*  pe_cond)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_eit);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,    pui2_src_id);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_get_eit_src_id");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_num_eit_entries
 *
 * Description: This API returns the number of entries of the given EIT table object
 *
 * Inputs:  h_eit           References an EIT table object.
 *
 * Outputs: pui2_num_entries    Contains the number of entries.
 *          pe_cond             Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_num_eit_entries (HANDLE_T             h_eit,
                                     UINT16*              pui2_num_entries,
                                     TM_COND_T*           pe_cond)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_eit);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,    pui2_num_entries);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_get_num_eit_entries");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_eit_entry
 *
 * Description: This API returns an event entry of the given EIT table object.
 *
 * Inputs:  h_eit           References an EIT table object.
 *          ui2_idx         Specifies the event by index.
 *
 * Outputs: pt_eit_info     Contains the EIT entry structure.
 *          pe_cond         Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_INV_GET_INFO      Invalid get operation data.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_eit_entry (HANDLE_T             h_eit,
                               UINT16               ui2_idx,
                               ATSC_SI_EIT_INFO_T*  pt_eit_info,
                               TM_COND_T*           pe_cond)
{
    RPC_DECL(4, INT32);      

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_eit_info, RPC_DESC_ATSC_SI_EIT_INFO_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_eit); 
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,      pt_eit_info);    
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_get_eit_entry");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_ett_etm_id
 *
 * Description: This API returns source id of the given ETT table object.
 *
 * Inputs:  h_ett           References an ETT table object.
 *
 * Outputs: pui4_etm_id     Contains the ETM id.
 *          pe_cond         Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_ett_etm_id (HANDLE_T    h_ett,
                                UINT32*     pui4_etm_id,
                                TM_COND_T*  pe_cond)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_ett);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    pui4_etm_id);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_get_ett_etm_id");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_txt_num_lang
 *
 * Description: This API returns the number of strings with different languages
 *              of the given multi string structure handle.
 *
 * Inputs:  h_txt           References a txt table object.
 *
 * Outputs: pui2_num_lang   Contains the number of strings.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_txt_num_lang (HANDLE_T    h_txt,
                                  UINT16*     pui2_num_lang)
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_txt);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,    pui2_num_lang);

    RPC_DO_OP("x_atsc_si_get_txt_num_lang"); 

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_txt_info
 *
 * Description: This API returns the information about the specified language
 *              of the given multi string structure handle.
 *
 * Inputs:  h_txt           References a txt table object.
 *          ui2_lang_idx    Contains the language index.
 *
 * Outputs: pt_lang_info    Contains the information about the language.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_INV_GET_INFO      Invalid get operation data.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_txt_info (HANDLE_T                  h_txt,
                              UINT16                    ui2_lang_idx,
                              ATSC_SI_TXT_LANG_INFO_T*  pt_lang_info)
{
    RPC_DECL(3, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_lang_info, RPC_DESC_ATSC_SI_RRT_INFO_T, NULL));
    RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_lang_info->t_lang));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_txt);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_lang_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,      pt_lang_info);

    RPC_DO_OP("x_atsc_si_get_txt_info");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_txt_seg_info
 *
 * Description: This API returns the information about the specified segment
 *              of the given multi string structure handle.
 *
 * Inputs:  h_txt               References a txt table object.
 *          ui2_lang_idx        Contains the language index.
 *          ui2_seg_idx         Contains the segment index.
 *
 * Outputs: pt_text_seg_info    Contains the information about the segment.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_INV_GET_INFO      Invalid get operation data.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_txt_seg_info (HANDLE_T                 h_txt,
                                  UINT16                   ui2_lang_idx,
                                  UINT16                   ui2_seg_idx,
                                  ATSC_SI_TXT_SEG_INFO_T*  pt_text_seg_info)
{
    RPC_DECL(4, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_text_seg_info, RPC_DESC_ATSC_SI_RRT_INFO_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_txt);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_lang_idx);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_seg_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,      pt_text_seg_info);

    RPC_DO_OP("x_atsc_si_get_txt_seg_info");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_txt
 *
 * Description: This API returns the text of the specified segment of
 *              the given multi string structure handle.
 *
 * Inputs:  h_txt           References a txt table object.
 *          ui2_lang_idx    Contains the language index.
 *          ui2_seg_idx     Contains the segment index.
 *          pz_txt_len      Contains the size of the buffer ps_txt.
 *
 * Outputs: ps_txt          Contains the segment string.
 *          pz_txt_len      Contains the actual length of ps_txt.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_INV_GET_INFO      Invalid get operation data.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_txt (HANDLE_T  h_txt,
                         UINT16    ui2_lang_idx,
                         UINT16    ui2_seg_idx,
                         CHAR*     ps_txt,
                         SIZE_T*   pz_txt_len)
{
    RPC_DECL(5, INT32);    

    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, ps_txt, *pz_txt_len));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_txt);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_lang_idx);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_seg_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,       ps_txt);
    RPC_ARG_IO(ARG_TYPE_REF_SIZE_T,     pz_txt_len);

    RPC_DO_OP("x_atsc_si_get_txt");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_txt_len
 *
 * Description: This API returns the text of the first segment of
 *              the given multi string structure handle.
 *
 * Inputs:  h_txt           References a txt table object.
 *
 * Outputs: pz_txt_len      Contains the length of the first segment.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_txt_len (HANDLE_T  h_txt,
                             SIZE_T*   pz_txt_len)
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_txt);
    RPC_ARG_OUT(ARG_TYPE_REF_SIZE_T,    pz_txt_len);

    RPC_DO_OP("x_atsc_si_get_txt_len");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_txt_len_by_lang
 *
 * Description: this function gets the length of the text in the requested 
 *              language from the text handle.
 *
 * Inputs:  h_txt               Contains the text handle.
 *          t_lang              Contains the ISO 639 language code.
 *
 * Outputs: pz_event_info_len   Contains the length of the text in the
 *                              requested language if the routine was 
 *                              successfully returned.
 *
 * Returns: TMR_OK              Success.
 *          TMR_INV_ARG         Fail. Invalid arguments.
 *          TMR_INV_GET_INFO    Fail. The requested info was not found.
 *
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_txt_len_by_lang (HANDLE_T           h_txt,
                                     ISO_639_LANG_T     t_lang,
                                     SIZE_T*            pz_event_info_len)
{
    RPC_DECL(3, INT32);    

    RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, t_lang));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_txt);
    RPC_ARG_INP(ARG_TYPE_REF_STR,       t_lang);
    RPC_ARG_OUT(ARG_TYPE_REF_SIZE_T,    pz_event_info_len);

    RPC_DO_OP("x_atsc_si_get_txt_len_by_lang");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_txt_by_lang
 *
 * Description: this function gets the text in the requested language from 
 *              the text handle.
 *
 * Inputs:  h_txt               Contains the text handle.
 *          t_lang              Contains the ISO 639 language code.
 *          pz_event_info_len   Contains the length of the buffer ps_txt.
 *
 * Outputs: pz_event_info_len   Contains the actual length of the text in
 *                              ps_txt if the routine was successfully 
 *                              returned.
 *          ps_txt              Contains the requested text if the routine
 *                              was successfully returned.
 *
 * Returns: TMR_OK              Success.
 *          TMR_INV_ARG         Fail. Invalid arguments.
 *          TMR_INV_GET_INFO    Fail. The requested info was not found.
 *
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_txt_by_lang (HANDLE_T           h_txt,
                                 ISO_639_LANG_T     t_lang,
                                 SIZE_T*            pz_event_info_len,
                                 CHAR*              ps_txt)
{
    RPC_DECL(4, INT32);    

    RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, t_lang));
    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, ps_txt, *pz_event_info_len));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_txt);
    RPC_ARG_INP(ARG_TYPE_REF_STR,       t_lang);
    RPC_ARG_IO(ARG_TYPE_REF_SIZE_T,     pz_event_info_len);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,      ps_txt);

    RPC_DO_OP("x_atsc_si_get_txt_by_lang");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_eas_event_data
 *
 * Description: This API returns the event data of the given EAS table object.
 *
 * Inputs:  h_eas           References an EAS table object.
 *
 * Outputs: pt_eas_info     Contains the EAS entry structure.
 *          pe_cond         Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_INV_GET_INFO      Invalid get operation data.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_eas_event_data (HANDLE_T     h_eas,
                                    EAS_EVENT_T* pt_eas_info,
                                    TM_COND_T*   pe_cond)
{
    RPC_DECL(3, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_eas_info,  RPC_DESC_EAS_EVENT_T, NULL));
    RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_eas_info->s_originator));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_eas);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,      pt_eas_info);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_get_eas_event_data");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_eas_evt_code_len
 *
 * Description: This API returns the event code length of the given EAS table object.
 *
 * Inputs:  h_eas           References an EAS table object.
 *
 * Outputs: pz_evt_code_len Contains the length of event code text.
 *          pe_cond         Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_INV_GET_INFO      Invalid get operation data.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_eas_evt_code_len (HANDLE_T   h_eas,
                                      SIZE_T*    pz_evt_code_len,
                                      TM_COND_T* pe_cond)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,            h_eas);
    RPC_ARG_OUT(ARG_TYPE_REF_SIZE_T,        pz_evt_code_len);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,         pe_cond);

    RPC_DO_OP("x_atsc_si_get_eas_evt_code_len");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_eas_evt_code
 *
 * Description: This API returns the event code text of the given EAS table object.
 *
 * Inputs:  h_eas           References an EAS table object.
 *          pz_evt_code_len Contains the size of the buffer ps_evt_code.
 *
 * Outputs: ps_evt_code     Contains the event code text.
 *          pz_evt_code_len Contains the actual length of event code text.
 *          pe_cond         Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_INV_GET_INFO      Invalid get operation data.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_eas_evt_code (HANDLE_T   h_eas,
                                  CHAR*      ps_evt_code,  
                                  SIZE_T*    pz_evt_code_len,
                                  TM_COND_T* pe_cond)
{
    RPC_DECL(4, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, ps_evt_code, *pz_evt_code_len));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,            h_eas);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,          ps_evt_code);
    RPC_ARG_IO (ARG_TYPE_REF_SIZE_T,        pz_evt_code_len);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,         pe_cond);

    RPC_DO_OP("x_atsc_si_get_eas_evt_code");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_eas_num_loc_entries
 *
 * Description: This API returns the number of location entries of the given 
 *              EAS table object
 *
 * Inputs:  h_eas               References an EAS table object.
 *
 * Outputs: pui2_num_entries    Contains the number of entries.
 *          pe_cond             Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_eas_num_loc_entries (HANDLE_T    h_eas,
                                         UINT16*     pui2_num_entries,
                                         TM_COND_T*  pe_cond)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,            h_eas);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,        pui2_num_entries);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,         pe_cond);

    RPC_DO_OP("x_atsc_si_get_eas_num_loc_entries");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_eas_loc_entry
 *
 * Description: This API returns a location entry of the given EAS table object.
 *
 * Inputs:  h_eas           References an EAS table object.
 *          ui2_idx         Specifies the location by index.
 *
 * Outputs: pt_location     Contains the location entry structure.
 *          pe_cond         Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_INV_GET_INFO      Invalid get operation data.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_eas_loc_entry (HANDLE_T         h_eas,
                                   UINT16           ui2_idx,
                                   EAS_LOC_INFO_T*  pt_location,
                                   TM_COND_T*       pe_cond)
{
    RPC_DECL(4, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_location, RPC_DESC_EAS_LOC_INFO_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_eas);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,      pt_location);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_get_eas_loc_entry");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_eas_num_xcept_entries
 *
 * Description: This API returns the number of exception entries of the given 
 *              EAS table object
 *
 * Inputs:  h_eas               References an EAS table object.
 *
 * Outputs: pui2_num_entries    Contains the number of entries.
 *          pe_cond             Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_eas_num_xcept_entries (HANDLE_T      h_eas,
                                           UINT16*       pui2_num_entries,
                                           TM_COND_T*    pe_cond)

{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,            h_eas);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,        pui2_num_entries);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,         pe_cond);

    RPC_DO_OP("x_atsc_si_get_eas_num_xcept_entries");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_get_eas_xcept_entry
 *
 * Description: This API returns an exception entry of the given EAS table object.
 *
 * Inputs:  h_eas           References an EAS table object.
 *          ui2_idx         Specifies the location by index.
 *
 * Outputs: pt_xcept        Contains the exception entry structure.
 *          pe_cond         Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_INV_GET_INFO      Invalid get operation data.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_get_eas_xcept_entry (HANDLE_T            h_eas,
                                     UINT16              ui2_idx,
                                     EAS_XCEPT_INFO_T*   pt_xcept,
                                     TM_COND_T*          pe_cond)
{
    RPC_DECL(4, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_xcept,  RPC_DESC_EAS_XCEPT_INFO_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_eas);
    RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,      pt_xcept);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pe_cond);

    RPC_DO_OP("x_atsc_si_get_eas_xcept_entry");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_txt_loc_vct_sname
 *
 * Description: This API returns a handle, in order to retrieve the short name of 
 *              a virtual channel from a VCT table object.
 *
 * Inputs:  h_vct       References a TVCT/CVCT table object.
 *          ui2_idx     Contains the index of the required virtual channel.
 *
 * Outputs: ph_txt      Contains the handle which references the virtual channel
 *                      short name.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_INV_GET_INFO      Invalid get operation data.
 *          TMR_OUT_OF_HANDLES    No handles available.
 *          TMR_OUT_OF_RESOURCES  Out of resources.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_txt_loc_vct_sname (HANDLE_T   h_vct,
                                   UINT16     ui2_idx,
                                   HANDLE_T*  ph_txt)
{
    INT32       i4_return;

    i4_return = _x_atsc_si_txt_loc_vct_sname(h_vct, ui2_idx, ph_txt);

    return (i4_return); 
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_txt_loc_rrt_region
 *
 * Description: This API returns a handle, in order to retrieve the name of 
 *              a rating region from an RRT table object.
 *
 * Inputs:  h_rrt       References an RRT table object.
 *
 * Outputs: ph_txt      Contains the handle which references the ratign region
 *                      name.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_OUT_OF_HANDLES    No handles available.
 *          TMR_OUT_OF_RESOURCES  Out of resources.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_txt_loc_rrt_region (HANDLE_T   h_rrt,
                                    HANDLE_T*  ph_txt)
{
    INT32       i4_return;

    i4_return = _x_atsc_si_txt_loc_rrt_region (h_rrt, ph_txt);

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_txt_loc_rrt_dim
 *
 * Description: This API returns a handle, in order to retrieve the dimension 
 *              name of a rating region from an RRT table object.
 *
 * Inputs:  h_rrt       References an RRT table object.
 *          ui2_dim_idx Contains the index of the required rating dimension.
 *
 * Outputs: ph_txt      Contains the handle which references the dimension
 *                      name.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_INV_GET_INFO      Invalid get operation data.
 *          TMR_OUT_OF_HANDLES    No handles available.
 *          TMR_OUT_OF_RESOURCES  Out of resources.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_txt_loc_rrt_dim (HANDLE_T   h_rrt,
                                 UINT16     ui2_dim_idx,
                                 HANDLE_T*  ph_txt)
{
    INT32       i4_return;
    
    i4_return = _x_atsc_si_txt_loc_rrt_dim (h_rrt, ui2_dim_idx, ph_txt);

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_txt_loc_rrt_abrv_rating
 *
 * Description: This API returns a handle, in order to retrieve the abbreviation 
 *              of a rating dimension value from an RRT table object.
 *
 * Inputs:  h_rrt       References an RRT table object.
 *          ui2_dim_idx Contains the index of the required rating dimension.
 *          ui2_val_idx Contains the index of value of the specified dimension.
 *
 * Outputs: ph_txt      Contains the handle which references the abbreviation.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_INV_GET_INFO      Invalid get operation data.
 *          TMR_OUT_OF_HANDLES    No handles available.
 *          TMR_OUT_OF_RESOURCES  Out of resources.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_txt_loc_rrt_abrv_rating (HANDLE_T   h_rrt,
                                         UINT16     ui2_dim_idx,
                                         UINT16     ui2_val_idx,
                                         HANDLE_T*  ph_txt)
{

    INT32       i4_return;
    
    i4_return = _x_atsc_si_txt_loc_rrt_abrv_rating(h_rrt, 
                                                   ui2_dim_idx, 
                                                   ui2_val_idx, 
                                                   ph_txt);

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_txt_loc_rrt_rating
 *
 * Description: This API returns a handle, in order to retrieve the name 
 *              of a rating dimension value from an RRT table object.
 *
 * Inputs:  h_rrt       References an RRT table object.
 *          ui2_dim_idx Contains the index of the required rating dimension.
 *          ui2_val_idx Contains the index of value of the specified dimension.
 *
 * Outputs: ph_txt      Contains the handle which references the value's name.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_INV_GET_INFO      Invalid get operation data.
 *          TMR_OUT_OF_HANDLES    No handles available.
 *          TMR_OUT_OF_RESOURCES  Out of resources.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_txt_loc_rrt_rating (HANDLE_T   h_rrt,
                                    UINT16     ui2_dim_idx,
                                    UINT16     ui2_val_idx,
                                    HANDLE_T*  ph_txt)
{
    INT32       i4_return;
    
    i4_return = _x_atsc_si_txt_loc_rrt_rating(h_rrt, 
                                              ui2_dim_idx, 
                                              ui2_val_idx, 
                                              ph_txt);

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_txt_loc_eit_title
 *
 * Description: This API returns a handle, in order to retrieve the title of 
 *              a program event from an EIT table object.
 *
 * Inputs:  h_eit       References an EIT table object.
 *          ui2_idx     Contains the index of the required program event.
 *
 * Outputs: ph_txt      Contains the handle which references the title.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_INV_GET_INFO      Invalid get operation data.
 *          TMR_OUT_OF_HANDLES    No handles available.
 *          TMR_OUT_OF_RESOURCES  Out of resources.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_txt_loc_eit_title (HANDLE_T   h_eit,
                                   UINT16     ui2_idx,
                                   HANDLE_T*  ph_txt)
{
    INT32       i4_return;

    i4_return = _x_atsc_si_txt_loc_eit_title (h_eit, ui2_idx, ph_txt);

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_txt_loc_ett_ext_text
 *
 * Description: This API returns a handle, in order to retrieve the extended
 *              text message from an ETT table object.
 *
 * Inputs:  h_rrt       References an ETT table object.
 *
 * Outputs: ph_txt      Contains the handle which references the extended text
 *                      message.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_OUT_OF_HANDLES    No handles available.
 *          TMR_OUT_OF_RESOURCES  Out of resources.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_txt_loc_ett_ext_text (HANDLE_T   h_ett,
                                      HANDLE_T*  ph_txt)
{
    INT32       i4_return;

    i4_return = _x_atsc_si_txt_loc_ett_ext_text (h_ett, ph_txt);

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_txt_loc_descr
 *
 * Description: This API returns a handle, in order to retrieve the multiple
 *              string on the given buffer.
 *
 * Inputs:  h_obj       References a table object, which contains the descriptor,
 *                      where the buffer pui1_multi_lang_txt comes from.
 *
 *          pui1_multi_lang_txt
 *                      References a multiple string structure data.
 *
 * Outputs: ph_txt      Contains the handle which references the multiple string
 *                      structure data.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_OUT_OF_HANDLES    No handles available.
 *          TMR_OUT_OF_RESOURCES  Out of resources.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_txt_loc_descr (HANDLE_T      h_obj,
                               const UINT8*  pui1_multi_lang_txt,
                               HANDLE_T*     ph_txt)
{
    INT32       i4_return;

    i4_return = _x_atsc_si_txt_loc_descr (h_obj, pui1_multi_lang_txt, ph_txt);

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_txt_loc_eas_nat_of_act
 *
 * Description: This API returns a handle, in order to retrieve the nature of 
 *              activation from an EAS table object.
 *
 * Inputs:  h_eas       References an EAS table object.
 *
 * Outputs: ph_txt      Contains the handle which references the nature of
 *                      activation.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_OUT_OF_HANDLES    No handles available.
 *          TMR_OUT_OF_RESOURCES  Out of resources.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_txt_loc_eas_nat_of_act (HANDLE_T     h_eas,
                                        HANDLE_T*    ph_txt)
{
    INT32       i4_return;
    
    i4_return = _x_atsc_si_txt_loc_eas_nat_of_act (h_eas, ph_txt);

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_txt_loc_eas_alert
 *
 * Description: This API returns a handle, in order to retrieve the alert
 *              message from an EAS table object.
 *
 * Inputs:  h_eas       References an EAS table object.
 *
 * Outputs: ph_txt      Contains the handle which references the alert message.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_OUT_OF_HANDLES    No handles available.
 *          TMR_OUT_OF_RESOURCES  Out of resources.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_txt_loc_eas_alert (HANDLE_T  h_eas,
                                   HANDLE_T* ph_txt)
{
    INT32       i4_return;

    i4_return = _x_atsc_si_txt_loc_eas_alert (h_eas, ph_txt);

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_load_stt
 *
 * Description: This API loads an STT table.
 *
 * Inputs:  h_root       References the root table object.
 *          pv_nfy_tag   Contains a notify tag value.
 *          pf_nfy       References the notify function.
 *
 * Outputs: ph_stt   Contains a handle which references the STT table.
 *          pe_cond  Contains the condition of the STT table.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_OUT_OF_HANDLES    No handles available.
 *          TMR_OUT_OF_RESOURCES  Out of resources.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_load_stt (HANDLE_T       h_root,
                          VOID*          pv_nfy_tag,
                          x_tm_nfy_fct   pf_nfy,
                          HANDLE_T*      ph_stt,
                          TM_COND_T*     pe_cond)
{
    INT32       i4_return;

    i4_return = _x_atsc_si_load_stt(h_root, 
                                    pv_nfy_tag,
                                    pf_nfy,
                                    ph_stt,
                                    pe_cond);

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_load_mgt_by_ts_id
 *
 * Description: This API loads an MGT table given a transport stream id.
 *
 * Inputs:  h_root       References the root table object.
 *          ui2_ts_id    Contains the MGT's transport stream id.
 *          e_src_type   Specifies the source type for loading the requested
 *                       table.
 *          pv_src_info  Contains additional information describing the source.
 *          pv_nfy_tag   Contains a notify tag value.
 *          pf_nfy       References the notify function.
 *
 * Outputs: ph_mgt   Contains a handle which references the MGT table.
 *          pe_cond  Contains the condition of the MGT table.
 *
 * Returns: TMR_OK                 Routine successful.
 *          TMR_NOT_INIT           The Table Manager is not initialized.
 *          TMR_INV_ARG            One or more of the arguments contain invalid
 *                                 data.
 *          TMR_INV_HANDLE         Invalid handle.
 *          TMR_OUT_OF_HANDLES     No handles available.
 *          TMR_OUT_OF_RESOURCES   Out of resources.
 *          TMR_INV_HANDLE_STATE   Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_load_mgt_by_ts_id (HANDLE_T       h_root,
                                   UINT16         ui2_ts_id,
                                   TM_SRC_TYPE_T  e_src_type,
                                   VOID*          pv_src_info,
                                   VOID*          pv_nfy_tag,
                                   x_tm_nfy_fct   pf_nfy,
                                   HANDLE_T*      ph_mgt,
                                   TM_COND_T*     pe_cond)
{
    INT32       i4_return;

    i4_return = _x_atsc_si_load_mgt_by_ts_id(h_root,
                                             ui2_ts_id,
                                             e_src_type,
                                             pv_src_info,
                                             pv_nfy_tag,
                                             pf_nfy,
                                             ph_mgt,
                                             pe_cond);

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_load_mgt_by_ts_descr
 *
 * Description: This API loads an MGT table given a transport stream description.
 *
 * Inputs:  h_root       References the root table object.
 *          pt_ts_descr  Contains the MGT's transport stream description.
 *          e_src_type   Specifies the source type for loading the requested
 *                       table.
 *          pv_src_info  Contains additional information describing the source.
 *          pv_nfy_tag   Contains a notify tag value.
 *          pf_nfy       References the notify function.
 *
 * Outputs: ph_mgt   Contains a handle which references the MGT table.
 *          pe_cond  Contains the condition of the MGT table.
 *
 * Returns: TMR_OK                 Routine successful.
 *          TMR_NOT_INIT           The Table Manager is not initialized.
 *          TMR_INV_ARG            One or more of the arguments contain invalid
 *                                 data.
 *          TMR_INV_HANDLE         Invalid handle.
 *          TMR_OUT_OF_HANDLES     No handles available.
 *          TMR_OUT_OF_RESOURCES   Out of resources.
 *          TMR_INV_HANDLE_STATE   Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_load_mgt_by_ts_descr (HANDLE_T       h_root,
                                      TS_DESCR_T*    pt_ts_descr,
                                      TM_SRC_TYPE_T  e_src_type,
                                      VOID*          pv_src_info,
                                      VOID*          pv_nfy_tag,
                                      x_tm_nfy_fct   pf_nfy,
                                      HANDLE_T*      ph_mgt,
                                      TM_COND_T*     pe_cond)
{
    INT32       i4_return;

    i4_return = _x_atsc_si_load_mgt_by_ts_descr(h_root,
                                                pt_ts_descr,
                                                e_src_type,
                                                pv_src_info,
                                                pv_nfy_tag,
                                                pf_nfy,
                                                ph_mgt,
                                                pe_cond);

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_load_cvct
 *
 * Description: This API loads a CVCT table given an MGT handle.
 *
 * Inputs:  h_mgt       References the MGT table object.
 *          e_src_type   Specifies the source type for loading the requested
 *                       table.
 *          pv_src_info  Contains additional information describing the source.
 *          pv_nfy_tag   Contains a notify tag value.
 *          pf_nfy       References the notify function.
 *
 * Outputs: ph_cvct   Contains a handle which references the CVCT table.
 *          pe_cond  Contains the condition of the CVCT table.
 *
 * Returns: TMR_OK                 Routine successful.
 *          TMR_NOT_INIT           The Table Manager is not initialized.
 *          TMR_INV_ARG            One or more of the arguments contain invalid
 *                                 data.
 *          TMR_INV_HANDLE         Invalid handle.
 *          TMR_OUT_OF_HANDLES     No handles available.
 *          TMR_OUT_OF_RESOURCES   Out of resources.
 *          TMR_INV_HANDLE_STATE   Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_load_cvct (HANDLE_T       h_mgt,
                           TM_SRC_TYPE_T  e_src_type,
                           VOID*          pv_src_info,
                           VOID*          pv_nfy_tag,
                           x_tm_nfy_fct   pf_nfy,
                           HANDLE_T*      ph_cvct,
                           TM_COND_T*     pe_cond)
{
    INT32       i4_return;

    i4_return = _x_atsc_si_load_cvct(h_mgt,
                                     e_src_type,
                                     pv_src_info,
                                     pv_nfy_tag,
                                     pf_nfy,
                                     ph_cvct,
                                     pe_cond);

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_load_tvct
 *
 * Description: This API loads a TVCT table given an MGT handle.
 *
 * Inputs:  h_mgt       References the MGT table object.
 *          e_src_type   Specifies the source type for loading the requested
 *                       table.
 *          pv_src_info  Contains additional information describing the source.
 *          pv_nfy_tag   Contains a notify tag value.
 *          pf_nfy       References the notify function.
 *
 * Outputs: ph_tvct   Contains a handle which references the TVCT table.
 *          pe_cond  Contains the condition of the TVCT table.
 *
 * Returns: TMR_OK                 Routine successful.
 *          TMR_NOT_INIT           The Table Manager is not initialized.
 *          TMR_INV_ARG            One or more of the arguments contain invalid
 *                                 data.
 *          TMR_INV_HANDLE         Invalid handle.
 *          TMR_OUT_OF_HANDLES     No handles available.
 *          TMR_OUT_OF_RESOURCES   Out of resources.
 *          TMR_INV_HANDLE_STATE   Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_load_tvct (HANDLE_T       h_mgt,
                           TM_SRC_TYPE_T  e_src_type,
                           VOID*          pv_src_info,
                           VOID*          pv_nfy_tag,
                           x_tm_nfy_fct   pf_nfy,
                           HANDLE_T*      ph_tvct,
                           TM_COND_T*     pe_cond)
{
    INT32       i4_return;

    i4_return = _x_atsc_si_load_tvct(h_mgt,
                                     e_src_type,
                                     pv_src_info,
                                     pv_nfy_tag,
                                     pf_nfy,
                                     ph_tvct,
                                     pe_cond);

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_load_rrt
 *
 * Description: This API loads an RRT table given an MGT handle and a region id.
 *
 * Inputs:  h_mgt        References the MGT table object.
 *          ui1_region   Contains the RRT's region id.
 *          e_src_type   Specifies the source type for loading the requested
 *                       table.
 *          pv_src_info  Contains additional information describing the source.
 *          pv_nfy_tag   Contains a notify tag value.
 *          pf_nfy       References the notify function.
 *
 * Outputs: ph_rrt   Contains a handle which references the RRT table.
 *          pe_cond  Contains the condition of the RRT table.
 *
 * Returns: TMR_OK                 Routine successful.
 *          TMR_NOT_INIT           The Table Manager is not initialized.
 *          TMR_INV_ARG            One or more of the arguments contain invalid
 *                                 data.
 *          TMR_INV_HANDLE         Invalid handle.
 *          TMR_OUT_OF_HANDLES     No handles available.
 *          TMR_OUT_OF_RESOURCES   Out of resources.
 *          TMR_INV_HANDLE_STATE   Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_load_rrt (HANDLE_T       h_mgt,
                          UINT8          ui1_region,
                          TM_SRC_TYPE_T  e_src_type,
                          VOID*          pv_src_info,
                          VOID*          pv_nfy_tag,
                          x_tm_nfy_fct   pf_nfy,
                          HANDLE_T*      ph_rrt,
                          TM_COND_T*     pe_cond)

{
    INT32       i4_return;

    i4_return = _x_atsc_si_load_rrt(h_mgt,
                                    ui1_region,
                                    e_src_type,
                                    pv_src_info,
                                    pv_nfy_tag,
                                    pf_nfy,
                                    ph_rrt,
                                    pe_cond);

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_load_eit_by_idx
 *
 * Description: This API loads an EIT table given a TVCT/CVCT handle, an index,
 *              and time.
 *
 * Inputs:  h_vct        References the TVCT/CVCT table object.
 *          ui2_idx      Specifies the channel by index.
 *          t_time       Specifies the time covered by the requested EIT table
 *                       object.
 *          e_src_type   Specifies the source type for loading the requested
 *                       table.
 *          pv_src_info  Contains additional information describing the source.
 *          pv_nfy_tag   Contains a notify tag value.
 *          pf_nfy       References the notify function.
 *
 * Outputs: ph_eit   Contains a handle which references the EIT table.
 *          pe_cond  Contains the condition of the EIT table.
 *
 * Returns: TMR_OK                 Routine successful.
 *          TMR_NOT_INIT           The Table Manager is not initialized.
 *          TMR_INV_ARG            One or more of the arguments contain invalid
 *                                 data.
 *          TMR_INV_HANDLE         Invalid handle.
 *          TMR_OUT_OF_HANDLES     No handles available.
 *          TMR_OUT_OF_RESOURCES   Out of resources.
 *          TMR_INV_HANDLE_STATE   Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_load_eit_by_idx (HANDLE_T       h_vct,
                                 UINT16         ui2_idx,
                                 TIME_T         t_time,
                                 TM_SRC_TYPE_T  e_src_type,
                                 VOID*          pv_src_info,
                                 VOID*          pv_nfy_tag,
                                 x_tm_nfy_fct   pf_nfy,
                                 HANDLE_T*      ph_eit,
                                 TM_COND_T*     pe_cond)
{
    INT32       i4_return;

    i4_return = _x_atsc_si_load_eit_by_idx(h_vct,
                                           ui2_idx,
                                           t_time,
                                           e_src_type,
                                           pv_src_info,
                                           pv_nfy_tag,
                                           pf_nfy,
                                           ph_eit,
                                           pe_cond);


    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_load_eit_by_src_id
 *
 * Description: This API loads an EIT table given an MGT/TVCT/CVCT handle, a 
 *              source id, and time.
 *
 * Inputs:  h_obj        References the MGT, TVCT or CVCT table object.
 *          ui2_src_id   Specifies the channel source id.
 *          t_time       Specifies the time covered by the requested EIT table
 *                       object.
 *          e_src_type   Specifies the source type for loading the requested
 *                       table.
 *          pv_src_info  Contains additional information describing the source.
 *          pv_nfy_tag   Contains a notify tag value.
 *          pf_nfy       References the notify function.
 *
 * Outputs: ph_eit   Contains a handle which references the EIT table.
 *          pe_cond  Contains the condition of the EIT table.
 *
 * Returns: TMR_OK                 Routine successful.
 *          TMR_NOT_INIT           The Table Manager is not initialized.
 *          TMR_INV_ARG            One or more of the arguments contain invalid
 *                                 data.
 *          TMR_INV_HANDLE         Invalid handle.
 *          TMR_OUT_OF_HANDLES     No handles available.
 *          TMR_OUT_OF_RESOURCES   Out of resources.
 *          TMR_INV_HANDLE_STATE   Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_load_eit_by_src_id (HANDLE_T       h_obj,
                                    UINT16         ui2_src_id,
                                    TIME_T         t_time,
                                    TM_SRC_TYPE_T  e_src_type,
                                    VOID*          pv_src_info,
                                    VOID*          pv_nfy_tag,
                                    x_tm_nfy_fct   pf_nfy,
                                    HANDLE_T*      ph_eit,
                                    TM_COND_T*     pe_cond)
{
    INT32       i4_return;
    
    i4_return = _x_atsc_si_load_eit_by_src_id (h_obj,
                                                  ui2_src_id,
                                                  t_time,
                                                  e_src_type,
                                                  pv_src_info,
                                                  pv_nfy_tag,
                                                  pf_nfy,
                                                  ph_eit,
                                                  pe_cond);

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_load_eit_0_by_idx
 *
 * Description: This API loads an EIT-0 table given a TVCT/CVCT handle and an
 *              index.
 *
 * Inputs:  h_vct        References the TVCT/CVCT table object.
 *          ui2_idx      Specifies the channel by index.
 *          e_src_type   Specifies the source type for loading the requested
 *                       table.
 *          pv_src_info  Contains additional information describing the source.
 *          pv_nfy_tag   Contains a notify tag value.
 *          pf_nfy       References the notify function.
 *
 * Outputs: ph_eit   Contains a handle which references the EIT table.
 *          pe_cond  Contains the condition of the EIT table.
 *
 * Returns: TMR_OK                 Routine successful.
 *          TMR_NOT_INIT           The Table Manager is not initialized.
 *          TMR_INV_ARG            One or more of the arguments contain invalid
 *                                 data.
 *          TMR_INV_HANDLE         Invalid handle.
 *          TMR_OUT_OF_HANDLES     No handles available.
 *          TMR_OUT_OF_RESOURCES   Out of resources.
 *          TMR_INV_HANDLE_STATE   Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_load_eit_0_by_idx (HANDLE_T       h_vct,
                                   UINT16         ui2_idx,
                                   TM_SRC_TYPE_T  e_src_type,
                                   VOID*          pv_src_info,
                                   VOID*          pv_nfy_tag,
                                   x_tm_nfy_fct   pf_nfy,
                                   HANDLE_T*      ph_eit,
                                   TM_COND_T*     pe_cond)
{
    INT32       i4_return;

    i4_return = _x_atsc_si_load_eit_0_by_idx(h_vct,
                                             ui2_idx,
                                             e_src_type,
                                             pv_src_info,
                                             pv_nfy_tag,
                                             pf_nfy,
                                             ph_eit,
                                             pe_cond);

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_load_eit_0_by_src_id
 *
 * Description: This API loads an EIT-0 table given an MGT/TVCT/CVCT handle,
 *              and a source id.
 *
 * Inputs:  h_obj        References the MGT, TVCT or CVCT table object.
 *          ui2_src_id   Specifies the channel source id.
 *          e_src_type   Specifies the source type for loading the requested
 *                       table.
 *          pv_src_info  Contains additional information describing the source.
 *          pv_nfy_tag   Contains a notify tag value.
 *          pf_nfy       References the notify function.
 *
 * Outputs: ph_eit   Contains a handle which references the EIT table.
 *          pe_cond  Contains the condition of the EIT table.
 *
 * Returns: TMR_OK                 Routine successful.
 *          TMR_NOT_INIT           The Table Manager is not initialized.
 *          TMR_INV_ARG            One or more of the arguments contain invalid
 *                                 data.
 *          TMR_INV_HANDLE         Invalid handle.
 *          TMR_OUT_OF_HANDLES     No handles available.
 *          TMR_OUT_OF_RESOURCES   Out of resources.
 *          TMR_INV_HANDLE_STATE   Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_load_eit_0_by_src_id (HANDLE_T       h_obj,
                                      UINT16         ui2_src_id,
                                      TM_SRC_TYPE_T  e_src_type,
                                      VOID*          pv_src_info,
                                      VOID*          pv_nfy_tag,
                                      x_tm_nfy_fct   pf_nfy,
                                      HANDLE_T*      ph_eit,
                                      TM_COND_T*     pe_cond)
{
    INT32       i4_return;
    
    i4_return = _x_atsc_si_load_eit_0_by_src_id(h_obj,
                                                ui2_src_id,
                                                e_src_type,
                                                pv_src_info,
                                                pv_nfy_tag,
                                                pf_nfy,
                                                ph_eit,
                                                pe_cond);

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_load_ett_by_idx
 *
 * Description: This API loads an ETT table given a VCT or EIT handle and an index.
 *
 * Inputs:  h_obj        References the VCT or EIT table object.
 *          ui2_idx      Specifies the channel by index.
 *          e_src_type   Specifies the source type for loading the requested
 *                       table.
 *          pv_src_info  Contains additional information describing the source.
 *          pv_nfy_tag   Contains a notify tag value.
 *          pf_nfy       References the notify function.
 *
 * Outputs: ph_ett   Contains a handle which references the ETT table.
 *          pe_cond  Contains the condition of the ETT table.
 *
 * Returns: TMR_OK                 Routine successful.
 *          TMR_NOT_INIT           The Table Manager is not initialized.
 *          TMR_INV_ARG            One or more of the arguments contain invalid
 *                                 data.
 *          TMR_INV_HANDLE         Invalid handle.
 *          TMR_OUT_OF_HANDLES     No handles available.
 *          TMR_OUT_OF_RESOURCES   Out of resources.
 *          TMR_INV_HANDLE_STATE   Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_load_ett_by_idx (HANDLE_T       h_obj,
                                 UINT16         ui2_idx,
                                 TM_SRC_TYPE_T  e_src_type,
                                 VOID*          pv_src_info,
                                 VOID*          pv_nfy_tag,
                                 x_tm_nfy_fct   pf_nfy,
                                 HANDLE_T*      ph_ett,
                                 TM_COND_T*     pe_cond)
{
    INT32       i4_return;

    i4_return = _x_atsc_si_load_ett_by_idx(h_obj,
                                           ui2_idx,
                                           e_src_type,
                                           pv_src_info,
                                           pv_nfy_tag,
                                           pf_nfy,
                                           ph_ett,
                                           pe_cond);


    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_load_ett_by_src_id
 *
 * Description: This API loads a channel ETT table given an MGT handle and a 
 *              source id.
 *
 * Inputs:  h_mgt        References the MGT table object.
 *          ui2_src_id   Specifies the channel source id.
 *          e_src_type   Specifies the source type for loading the requested
 *                       table.
 *          pv_src_info  Contains additional information describing the source.
 *          pv_nfy_tag   Contains a notify tag value.
 *          pf_nfy       References the notify function.
 *
 * Outputs: ph_ett   Contains a handle which references the ETT table.
 *          pe_cond  Contains the condition of the ETT table.
 *
 * Returns: TMR_OK                 Routine successful.
 *          TMR_NOT_INIT           The Table Manager is not initialized.
 *          TMR_INV_ARG            One or more of the arguments contain invalid
 *                                 data.
 *          TMR_INV_HANDLE         Invalid handle.
 *          TMR_OUT_OF_HANDLES     No handles available.
 *          TMR_OUT_OF_RESOURCES   Out of resources.
 *          TMR_INV_HANDLE_STATE   Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_load_ett_by_src_id (HANDLE_T       h_mgt,
                                    UINT16         ui2_src_id,
                                    TM_SRC_TYPE_T  e_src_type,
                                    VOID*          pv_src_info,
                                    VOID*          pv_nfy_tag,
                                    x_tm_nfy_fct   pf_nfy,
                                    HANDLE_T*      ph_ett,
                                    TM_COND_T*     pe_cond)
{
    INT32       i4_return;

    i4_return = _x_atsc_si_load_ett_by_src_id(h_mgt,
                                              ui2_src_id,
                                              e_src_type,
                                              pv_src_info,
                                              pv_nfy_tag,
                                              pf_nfy,
                                              ph_ett,
                                              pe_cond);

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_load_ett_by_src_evt_id
 *
 * Description: This API loads an event ETT table given an MGT handle, a source
 * id, an event id, and time.
 *
 * Inputs:  h_mgt        References the MGT table object.
 *          ui2_src_id   Specifies the channel source id.
 *          ui2_evt_id   Specifies the event id.
 *          t_time       Specifies the time which the event covers.
 *          e_src_type   Specifies the source type for loading the requested
 *                       table.
 *          pv_src_info  Contains additional information describing the source.
 *          pv_nfy_tag   Contains a notify tag value.
 *          pf_nfy       References the notify function.
 *
 * Outputs: ph_ett   Contains a handle which references the ETT table.
 *          pe_cond  Contains the condition of the ETT table.
 *
 * Returns: TMR_OK                 Routine successful.
 *          TMR_NOT_INIT           The Table Manager is not initialized.
 *          TMR_INV_ARG            One or more of the arguments contain invalid
 *                                 data.
 *          TMR_INV_HANDLE         Invalid handle.
 *          TMR_OUT_OF_HANDLES     No handles available.
 *          TMR_OUT_OF_RESOURCES   Out of resources.
 *          TMR_INV_HANDLE_STATE   Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_load_ett_by_src_evt_id (HANDLE_T       h_mgt,
                                        UINT16         ui2_src_id,
                                        UINT16         ui2_evt_id,
                                        TIME_T         t_time,
                                        TM_SRC_TYPE_T  e_src_type,
                                        VOID*          pv_src_info,
                                        VOID*          pv_nfy_tag,
                                        x_tm_nfy_fct   pf_nfy,
                                        HANDLE_T*      ph_ett,
                                        TM_COND_T*     pe_cond)
{
    INT32       i4_return;

    i4_return = _x_atsc_si_load_ett_by_src_evt_id(h_mgt,
                                                  ui2_src_id,
                                                  ui2_evt_id,
                                                  t_time,
                                                  e_src_type,
                                                  pv_src_info,
                                                  pv_nfy_tag,
                                                  pf_nfy,
                                                  ph_ett,
                                                  pe_cond);

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_atsc_si_load_eas
 *
 * Description: This API loads an EAS table given a TM root handle.
 *
 * Inputs:  h_root       References the root table object.
 *          pv_nfy_tag   Contains a notify tag value.
 *          pf_nfy       References the notify function.
 *
 * Outputs: ph_eas   Contains a handle which references the EAS table.
 *          pe_cond  Contains the condition of the EAS table.
 *
 * Returns: TMR_OK                 Routine successful.
 *          TMR_NOT_INIT           The Table Manager is not initialized.
 *          TMR_INV_ARG            One or more of the arguments contain invalid
 *                                 data.
 *          TMR_INV_HANDLE         Invalid handle.
 *          TMR_OUT_OF_HANDLES     No handles available.
 *          TMR_OUT_OF_RESOURCES   Out of resources.
 *          TMR_INV_HANDLE_STATE   Invalid handle state.
 ----------------------------------------------------------------------------*/
INT32 c_atsc_si_load_eas (HANDLE_T       h_root,
                          VOID*          pv_nfy_tag,
                          x_tm_nfy_fct   pf_nfy,
                          HANDLE_T*      ph_eas,
                          TM_COND_T*     pe_cond)
{
    INT32       i4_return;

    i4_return = _x_atsc_si_load_eas(h_root,
                                    pv_nfy_tag,
                                    pf_nfy,
                                    ph_eas,
                                    pe_cond);
    
    return (i4_return);
}

/* Replace with tm register cb */
#if 0
INT32 c_rpc_reg_atsc_si_cb_hndlrs()
{
    RPC_REG_CB_HNDLR(x_tm_nfy_fct);    
}
#endif


