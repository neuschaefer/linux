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
 *    This file contains implementation of the NWL's application API's. 
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/
#ifndef NO_NWL_INIT
#include "nwl/c_nwl.h"
#include "nwl/x_nwl.h"
#include "handle/handle.h"

#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"

/*-----------------------------------------------------------------------------
                    functions implementation
 ----------------------------------------------------------------------------*/

static INT32 _hndlr_x_nwl_nfy_fct(
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
    
       ((x_nwl_nfy_fct)pv_cb_addr)(
            (HANDLE_T)pt_args[0].u.ui4_arg,
          (NWL_COND_T)pt_args[1].u.i4_arg,
                      pt_args[2].u.ui4_arg,
                      pt_args[3].u.pv_arg,
                      pt_args[4].u.ui4_arg);
                      
    return RPCR_OK;
}

static INT32 _x_nwl_create(const CHAR*      ps_nwl_name,
                   UINT16           ui2_nwl_id,
                   NWL_MEM_POLICY_T e_mem_policy,
                   UINT16           ui2_max_nb_rec,
                   VOID*            pv_tag,
                   x_nwl_nfy_fct    pf_nfy,
                   HANDLE_T*        ph_nwl)
{
    RPC_DECL(7, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_REF_STR,   ps_nwl_name);
    RPC_ARG_INP(ARG_TYPE_UINT16,     ui2_nwl_id);
    RPC_ARG_INP(ARG_TYPE_UINT8,    e_mem_policy);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_max_nb_rec);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,       pv_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,       pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,     ph_nwl);

    RPC_DO_OP("x_nwl_create");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

static INT32 _x_nwl_open(UINT16          ui2_nwl_id,
                 VOID*           pv_tag,
                 x_nwl_nfy_fct   pf_nfy,
                 HANDLE_T*       ph_nwl)
{
    RPC_DECL(4, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT16,     ui2_nwl_id);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,       pv_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,       pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,     ph_nwl);

    RPC_DO_OP("x_nwl_open");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

static INT32 _x_nwl_fs_load           (HANDLE_T            h_dir,
                               const CHAR*         ps_nwl_path,
                               UINT16              ui2_nwl_id,
                               const CHAR*	       ps_nwl_name,
                               VOID*               pv_tag,
                               x_nwl_nfy_fct       pf_nfy,
                               HANDLE_T*           ph_nwl)
{
    RPC_DECL(7, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_dir);
    RPC_ARG_INP(ARG_TYPE_REF_STR,   ps_nwl_path);
    RPC_ARG_INP(ARG_TYPE_UINT16,     ui2_nwl_id);
    RPC_ARG_INP(ARG_TYPE_REF_STR,   ps_nwl_name);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,       pv_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,       pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,     ph_nwl);

    RPC_DO_OP("x_nwl_fs_load");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

INT32 c_nwl_create (const CHAR*       ps_nwl_name,
                    UINT16            ui2_nwl_id,
                    NWL_MEM_POLICY_T  e_mem_policy,
                    UINT16            ui2_max_nb_rec,
                    VOID*             pv_tag,
                    x_nwl_nfy_fct     pf_nfy,
                    HANDLE_T*         ph_nwl)
{
    INT32     i4_rc;
    i4_rc = _x_nwl_create(ps_nwl_name, ui2_nwl_id, e_mem_policy,
                         ui2_max_nb_rec, pv_tag, pf_nfy, ph_nwl);
    return i4_rc;
}

INT32 c_nwl_delete (HANDLE_T  h_nwl)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,  h_nwl);

    RPC_DO_OP("x_nwl_delete");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

INT32 c_nwl_open (UINT16         ui2_nwl_id,
                  VOID*          pv_tag,
                  x_nwl_nfy_fct  pf_nfy,
                  HANDLE_T*      ph_nwl)
{
    INT32     i4_rc;
    
    i4_rc = _x_nwl_open(ui2_nwl_id, pv_tag, pf_nfy, ph_nwl);

    return i4_rc;
}

INT32 c_nwl_close (HANDLE_T  h_nwl)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,  h_nwl);

    RPC_DO_OP("x_nwl_close");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

INT32 c_nwl_clean (HANDLE_T  h_nwl)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,  h_nwl);

    RPC_DO_OP("x_nwl_clean");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

INT32 c_nwl_add_rec  (HANDLE_T          h_nwl,
                      const NWL_REC_T*  pt_nwl_rec)
{
    RPC_DECL(2, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_nwl_rec,  RPC_DESC_NWL_REC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_nwl);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_nwl_rec);

    RPC_DO_OP("x_nwl_add_rec");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

INT32 c_nwl_del_rec (HANDLE_T  h_nwl,
                     UINT16    ui2_nwl_rec_id)
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,          h_nwl);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_nwl_rec_id);

    RPC_DO_OP("x_nwl_del_rec");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

INT32 c_nwl_update_rec (HANDLE_T          h_nwl,
                        const NWL_REC_T*  pt_nwl_rec,
                        BOOL              b_must_exist)
{
    RPC_DECL(3, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_nwl_rec,  RPC_DESC_NWL_REC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_nwl);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_nwl_rec);
    RPC_ARG_INP(ARG_TYPE_BOOL,   b_must_exist);

    RPC_DO_OP("x_nwl_update_rec");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

INT32 c_nwl_qry_rec_id (HANDLE_T          h_nwl,
                        NWL_REC_ID_QRY_T  e_nwl_qry_option,
                        UINT16*           pui2_rec_id)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_nwl);
    RPC_ARG_INP(ARG_TYPE_UINT8, e_nwl_qry_option);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,   pui2_rec_id);

    RPC_DO_OP("x_nwl_qry_rec_id");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

INT32 c_nwl_lock (HANDLE_T  h_nwl)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,  h_nwl);

    RPC_DO_OP("x_nwl_lock");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

INT32 c_nwl_unlock (HANDLE_T  h_nwl)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,  h_nwl);

    RPC_DO_OP("x_nwl_unlock");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

INT32 c_nwl_read_lock (HANDLE_T  h_nwl)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,  h_nwl);

    RPC_DO_OP("x_nwl_read_lock");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

INT32 c_nwl_read_unlock (HANDLE_T  h_nwl)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,  h_nwl);

    RPC_DO_OP("x_nwl_read_unlock");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

INT32 c_nwl_get_rec (HANDLE_T    h_nwl,
                     UINT16      ui2_nwl_rec_id,
                     NWL_REC_T*  pt_nwl_rec,
                     UINT32*     pui4_ver_id)
{
    RPC_DECL(4, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_nwl_rec,  RPC_DESC_NWL_REC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_nwl);
    RPC_ARG_INP(ARG_TYPE_UINT16,  ui2_nwl_rec_id);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,    pt_nwl_rec);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pui4_ver_id);

    RPC_DO_OP("x_nwl_get_rec");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

INT32 c_nwl_get_num_rec (HANDLE_T  h_nwl,
                         UINT16*   pui2_num_recs,
                         UINT32*   pui4_ver_id)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,             h_nwl);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16, pui2_num_recs);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,   pui4_ver_id);

    RPC_DO_OP("x_nwl_get_num_rec");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

INT32 c_nwl_get_rec_by_nw_id (HANDLE_T    h_nwl,
                              UINT16      ui2_network_id,
                              UINT16      ui2_idx,
                              NWL_REC_T*  pt_nwl_rec,
                              UINT32*     pui4_ver_id)
{
    RPC_DECL(5, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_nwl_rec,  RPC_DESC_NWL_REC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_nwl);
    RPC_ARG_INP(ARG_TYPE_UINT16,  ui2_network_id);
    RPC_ARG_INP(ARG_TYPE_UINT16,         ui2_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,    pt_nwl_rec);
    RPC_ARG_IO(ARG_TYPE_REF_UINT32,  pui4_ver_id);

    RPC_DO_OP("x_nwl_get_rec_by_nw_id");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

INT32 c_nwl_get_num_rec_by_nw_id(HANDLE_T  h_nwl,
                                 UINT16    ui2_network_id,
                                 UINT16*   pui2_num_recs,
                                 UINT32*   pui4_ver_id)
{
    RPC_DECL(4, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,              h_nwl);
    RPC_ARG_INP(ARG_TYPE_UINT16,     ui2_network_id);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_num_recs);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    pui4_ver_id);

    RPC_DO_OP("x_nwl_get_num_rec_by_nw_id");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

INT32 c_nwl_rec_init (NWL_REC_T*  pt_nwl_rec)
{
    RPC_DECL(1, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_nwl_rec,  RPC_DESC_NWL_REC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_REF_DESC,    pt_nwl_rec);

    RPC_DO_OP("x_nwl_rec_init");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

INT32 c_nwl_fs_load (HANDLE_T       h_dir,
                     const CHAR*    ps_nwl_path,
                     UINT16         ui2_nwl_id,
                     const CHAR*    ps_nwl_name,
                     VOID*          pv_tag,
                     x_nwl_nfy_fct  pf_nfy,
                     HANDLE_T*      ph_nwl)
{
    INT32     i4_rc;
    i4_rc = _x_nwl_fs_load(h_dir, ps_nwl_path, ui2_nwl_id,
                          ps_nwl_name, pv_tag, pf_nfy, ph_nwl);
    return i4_rc;
}

INT32 c_nwl_fs_store (HANDLE_T     h_dir,
                      const CHAR*  ps_nwl_path,
                      UINT16       ui2_nwl_id)
{
    RPC_DECL(3, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,        h_dir);
    RPC_ARG_INP(ARG_TYPE_REF_STR, ps_nwl_path);
    RPC_ARG_INP(ARG_TYPE_UINT16,   ui2_nwl_id);

    RPC_DO_OP("x_nwl_fs_store");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

INT32 c_nwl_fs_del (HANDLE_T     h_dir,
                    const CHAR*  ps_nwl_path,
                    UINT16       ui2_nwl_id)
{
    RPC_DECL(3, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,        h_dir);
    RPC_ARG_INP(ARG_TYPE_REF_STR, ps_nwl_path);
    RPC_ARG_INP(ARG_TYPE_UINT16,   ui2_nwl_id);

    RPC_DO_OP("x_nwl_fs_del");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

INT32 c_nwl_fs_qry (HANDLE_T     h_dir,
                    const CHAR*  ps_nwl_path,
                    UINT16       ui2_idx,
                    UINT16*      pui2_nwl_id,
                    CHAR*        ps_nwl_name)
{
    RPC_DECL(5, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,           h_dir);
    RPC_ARG_INP(ARG_TYPE_REF_STR,    ps_nwl_path);
    RPC_ARG_INP(ARG_TYPE_UINT16,         ui2_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16, pui2_nwl_id);
    RPC_ARG_OUT(ARG_TYPE_REF_STR,    ps_nwl_name);

    RPC_DO_OP("x_nwl_fs_qry");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

INT32 c_nwl_fs_sync (HANDLE_T     h_nwl,
                     HANDLE_T     h_dir,
                     const CHAR*  ps_nwl_path,
                     UINT16       ui2_nwl_id)
{
    RPC_DECL(4, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,           h_nwl);
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_dir);
    RPC_ARG_INP(ARG_TYPE_REF_STR,    ps_nwl_path);
    RPC_ARG_INP(ARG_TYPE_UINT16,      ui2_nwl_id);

    RPC_DO_OP("x_nwl_fs_sync");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_INV_ARG);
}

INT32 c_nwl_init_mem_dump(NWL_MEM_DUMP_T * pt_mem_dump)
{
    RPC_DECL(1, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_mem_dump,  RPC_DESC_NWL_MEM_DUMP_T, NULL));

    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_mem_dump);

    RPC_DO_OP("x_nwl_init_mem_dump");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_FAIL);
}

INT32 c_nwl_free_mem_dump(NWL_MEM_DUMP_T * pt_mem_dump)
{
    RPC_DECL(1, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_mem_dump,  RPC_DESC_NWL_MEM_DUMP_T, NULL));

    RPC_ARG_IO(ARG_TYPE_REF_DESC, pt_mem_dump);

    RPC_DO_OP("x_nwl_free_mem_dump");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_FAIL);
}

INT32 c_nwl_dump_to_mem(HANDLE_T h_nwl,
                        NWL_MEM_DUMP_T * pt_mem_dump)
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_mem_dump,  RPC_DESC_NWL_MEM_DUMP_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32, h_nwl);
    RPC_ARG_IO(ARG_TYPE_REF_DESC, pt_mem_dump);

    RPC_DO_OP("x_nwl_dump_to_mem");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_FAIL);
}

INT32 c_nwl_load_from_mem(HANDLE_T h_nwl,
                          NWL_MEM_DUMP_T * pt_mem_dump)
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_mem_dump,  RPC_DESC_NWL_MEM_DUMP_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32, h_nwl);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_mem_dump);

    RPC_DO_OP("x_nwl_load_from_mem");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_FAIL);
}

INT32 c_nwl_get_rec_by_rec_idx(HANDLE_T h_nwl,
                               UINT16 ui2_idx,
                               NWL_REC_T * pt_nwl_rec,
                               UINT32 * pui4_ver_id)
{
    RPC_DECL(4, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_nwl_rec,  RPC_DESC_NWL_REC_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32, h_nwl);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_nwl_rec);
    RPC_ARG_IO(ARG_TYPE_REF_UINT32, pui4_ver_id);

    RPC_DO_OP("x_nwl_get_rec_by_rec_idx");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_FAIL);
}

INT32 c_nwl_get_rec_by_sat(HANDLE_T h_nwl,
                           UINT16 ui2_satl_id,
                           UINT16 ui2_satl_rec_id,
                           UINT16 ui2_idx,
                           NWL_REC_T * pt_nwl_rec,
                           UINT32 * pui4_ver_id)
{
    RPC_DECL(6, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_nwl_rec,  RPC_DESC_NWL_REC_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32, h_nwl);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_satl_id);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_satl_rec_id);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_nwl_rec);
    RPC_ARG_IO(ARG_TYPE_REF_UINT32, pui4_ver_id);

    RPC_DO_OP("x_nwl_get_rec_by_sat");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_FAIL);
}

INT32 c_nwl_get_num_rec_by_sat(HANDLE_T h_nwl,
                               UINT16 ui2_satl_id,
                               UINT16 ui2_satl_rec_id,
                               UINT16 * pui2_num_recs,
                               UINT32 * pui4_ver_id)
{
    RPC_DECL(5, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_nwl);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_satl_id);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_satl_rec_id);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16, pui2_num_recs);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pui4_ver_id);

    RPC_DO_OP("x_nwl_get_num_rec_by_sat");

    RPC_RETURN(ARG_TYPE_INT32, NWLR_FAIL);
}

INT32 c_rpc_reg_nwl_cb_hndlrs()
{
    RPC_REG_CB_HNDLR(x_nwl_nfy_fct);    

    return RPCR_OK;
}
#endif
