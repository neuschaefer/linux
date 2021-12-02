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

#include "dt/x_dt.h"


#include "rpc.h"
#include "ri_common.h"

#include "ri_desc.h"

static VOID
_x_dt_nfy_fct_wrapper(
    HANDLE_T     h_hdl,
    VOID*        pv_tag,
    DT_COND_T    t_dt_cond,
    TIME_T       t_delta)
{
    RPC_DECL_VOID(4);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_tag;

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_hdl);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);
    RPC_ARG_INP(ARG_TYPE_INT32,    t_dt_cond);
    RPC_ARG_INP(ARG_TYPE_INT64,    t_delta);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_dt_nfy_fct", pt_nfy_tag->pv_cb_addr);

    RPC_RETURN_VOID;
}

INT32 _hndlr_x_dt_add(
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
    pt_return->u.i4_arg = x_dt_add(
                  (DTG_T*)pt_args[0].u.pv_desc,
                  (TIME_T)pt_args[1].u.i8_arg,
                  (DTG_T*)pt_args[2].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_conv_utc_local(
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
    pt_return->u.i4_arg = x_dt_conv_utc_local(
                  (DTG_T*)pt_args[0].u.pv_desc,
                  (DTG_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_diff(
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
    pt_return->e_type   = ARG_TYPE_INT64;
    pt_return->u.i8_arg = x_dt_diff(
                  (DTG_T*)pt_args[0].u.pv_desc,
                  (DTG_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_dtg_to_sec(
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
    pt_return->e_type   = ARG_TYPE_INT64;
    pt_return->u.i8_arg = x_dt_dtg_to_sec(
                  (DTG_T*)pt_args[0].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_get_brdcst_utc(
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
    pt_return->e_type   = ARG_TYPE_INT64;
    pt_return->u.i8_arg = x_dt_get_brdcst_utc(
                 (UINT16*)pt_args[0].u.pui2_arg,
              (DT_COND_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_get_utc(
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
    pt_return->e_type   = ARG_TYPE_INT64;
    pt_return->u.i8_arg = x_dt_get_utc(
                 (UINT16*)pt_args[0].u.pui2_arg,
              (DT_COND_T*)pt_args[1].u.pi4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_leap_yr(
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
    pt_return->e_type   = ARG_TYPE_BOOL;
    pt_return->u.b_arg = x_dt_leap_yr(
                  (TIME_T)pt_args[0].u.i8_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_reg_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T *  pt_nfy_tag;
    x_dt_nfy_fct        pf_nfy;

    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }

    if(pt_args[0].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy = _x_dt_nfy_fct_wrapper;
        RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_args[0].u.pv_func, pt_args[1].u.pv_arg);
    }

    pt_return->e_type   = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_dt_reg_nfy_fct(pf_nfy,
                                           pt_nfy_tag,
                                           (HANDLE_T *)pt_args[2].u.pui4_arg);

    if(pt_return->u.i4_arg == DTR_OK)
    {
        ri_bind_cb_tag(*(HANDLE_T *)pt_args[2].u.pui4_arg, pt_nfy_tag);
    }
    else if (pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }
    return RPCR_OK;
}

INT32 _hndlr_x_dt_set_dst(
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
    pt_return->e_type   = ARG_TYPE_VOID;
    x_dt_set_dst(pt_args[0].u.b_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_set_dst_ctrl(
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
    pt_return->e_type   = ARG_TYPE_VOID;
    x_dt_set_dst_ctrl(pt_args[0].u.b_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_set_sync_src(
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
    pt_return->u.i4_arg = x_dt_set_sync_src(
      (DT_SYNC_SRC_TYPE_T)pt_args[0].u.ui4_arg,
      (DT_SYNC_SRC_TYPE_T)pt_args[1].u.ui4_arg,
                   (VOID*)pt_args[2].u.ps_str);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_set_tz(
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
    pt_return->e_type   = ARG_TYPE_VOID;
    x_dt_set_tz((TIME_T)pt_args[0].u.i8_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_set_utc(
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
    pt_return->u.i4_arg = x_dt_set_utc(
                  (TIME_T)pt_args[0].u.i8_arg,
                  (UINT16)pt_args[1].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_sync_with_brdcst_utc(
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
    pt_return->u.i4_arg = x_dt_sync_with_brdcst_utc(
                    (BOOL)pt_args[0].u.b_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_utc_sec_to_dtg(
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
    pt_return->u.i4_arg = x_dt_utc_sec_to_dtg(
                  (TIME_T)pt_args[0].u.i8_arg,
                  (DTG_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_utc_sec_to_loc_dtg(
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

    pt_return->u.i4_arg = x_dt_utc_sec_to_loc_dtg(
                  (TIME_T)pt_args[0].u.i8_arg,
                  (DTG_T*)pt_args[1].u.pv_desc);
    return RPCR_OK;
}

#if 1 /*Yan CLI*/
INT32 _hndlr_x_dt_get_tz(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 0)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_INT64;

    pt_return->u.i8_arg = x_dt_get_tz();
    return RPCR_OK;
}

INT32 _hndlr_x_dt_get_dst(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 0)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_BOOL;

    pt_return->u.b_arg = x_dt_get_dst();
    return RPCR_OK;
}

INT32 _hndlr_x_dt_get_tz_ctrl(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 0)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_BOOL;

    pt_return->u.b_arg = x_dt_get_tz_ctrl();
    return RPCR_OK;
}

INT32 _hndlr_x_dt_get_dst_ctrl(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 0)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type   = ARG_TYPE_BOOL;

    pt_return->u.b_arg = x_dt_get_dst_ctrl();
    return RPCR_OK;
}
#endif

INT32 _hndlr_x_dt_set_sys_count_code(
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
    pt_return->u.i4_arg = x_dt_set_sys_count_code(
            pt_args[0].u.ps_str,
            pt_args[1].u.ui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_get_sys_count_code(
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
    pt_return->u.i4_arg = x_dt_get_sys_count_code(
            (ISO_3166_COUNT_T*)(pt_args[0].u.ps_str),
            pt_args[1].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_get_count_code(
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
    pt_return->u.i4_arg = x_dt_get_count_code(
                pt_args[0].u.ui2_arg,
                (ISO_3166_COUNT_T*)(pt_args[1].u.ps_str),
                pt_args[2].u.pui2_arg,
       (TIME_T*)pt_args[3].u.pi8_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_get_num_count_code(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 0)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_UINT16;
    pt_return->u.ui2_arg= x_dt_get_num_count_code();

    return RPCR_OK;
}

INT32 _hndlr_x_dt_get_gps(
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

    pt_return->e_type   = ARG_TYPE_INT64;
    pt_return->u.i8_arg = x_dt_get_gps(
            pt_args[0].u.pi4_arg,
            pt_args[1].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_gps_sec_to_utc_sec(
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

    pt_return->e_type   = ARG_TYPE_INT64;
    pt_return->u.i8_arg = x_dt_gps_sec_to_utc_sec(
            (TIME_T)pt_args[0].u.i8_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_bcd_to_sec(
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
    pt_return->u.i4_arg = x_dt_bcd_to_sec(
                pt_args[0].u.pui1_arg,
       (TIME_T*)pt_args[1].u.pi8_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_mjd_bcd_to_dtg(
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
    pt_return->u.i4_arg = x_dt_mjd_bcd_to_dtg(
                pt_args[0].u.pui1_arg,
        (DTG_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_mjd_to_dtg(
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
    pt_return->u.i4_arg = x_dt_mjd_to_dtg(
                pt_args[0].u.ui4_arg,
        (DTG_T*)pt_args[1].u.pv_desc);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_set_tz_ctrl(
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

    pt_return->e_type   = ARG_TYPE_VOID;
    x_dt_set_tz_ctrl(pt_args[0].u.b_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_check_input_time(
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
    pt_return->u.i4_arg = x_dt_check_input_time(pt_args[0].u.b_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_config_check_input_time(
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
    pt_return->u.i4_arg = x_dt_config_check_input_time(
            (DT_CHECK_TIME_CONFIG)pt_args[0].u.ui4_arg,
                                  pt_args[1].u.pv_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_set_config(
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
    pt_return->u.i4_arg = x_dt_set_config(pt_args[0].u.ui4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_get_ds_change(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 0)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT64;
    pt_return->u.i8_arg = x_dt_get_ds_change();

    return RPCR_OK;
}

INT32 _hndlr_x_dt_set_ds_change(
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

    pt_return->e_type   = ARG_TYPE_VOID;
    x_dt_set_ds_change(pt_args[0].u.i8_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_get_ds_offset(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 0)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_INT64;
    pt_return->u.i8_arg = x_dt_get_ds_offset();

    return RPCR_OK;
}

INT32 _hndlr_x_dt_set_ds_offset(
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

    pt_return->e_type   = ARG_TYPE_VOID;
    x_dt_set_ds_offset(pt_args[0].u.i8_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_dtg_to_mjd(
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
    pt_return->u.i4_arg = x_dt_dtg_to_mjd(
            (DTG_T *)pt_args[0].u.pv_desc,
                     pt_args[1].u.pui2_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_dtg_to_mjd_bcd(
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
    pt_return->u.i4_arg = x_dt_dtg_to_mjd_bcd(
            (DTG_T *)pt_args[0].u.pv_desc,
                     pt_args[1].u.pui2_arg,
                     pt_args[2].u.pui2_arg,
                     pt_args[3].u.pui1_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_dt_get_last_sync_tbl_id(
        RPC_ID_T     t_rpc_id,
        const CHAR*  ps_cb_type,
        UINT32       ui4_num_args,
        ARG_DESC_T*  pt_args,
        ARG_DESC_T*  pt_return)
{
    if (ui4_num_args != 0)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type   = ARG_TYPE_UINT8;
    pt_return->u.ui1_arg= x_dt_get_last_sync_tbl_id();

    return RPCR_OK;
}



INT32 c_rpc_reg_dt_op_hndlrs()
{
    RPC_REG_OP_HNDLR(x_dt_add);
    RPC_REG_OP_HNDLR(x_dt_conv_utc_local);
    RPC_REG_OP_HNDLR(x_dt_diff);
    RPC_REG_OP_HNDLR(x_dt_dtg_to_sec);
    RPC_REG_OP_HNDLR(x_dt_get_brdcst_utc);
    RPC_REG_OP_HNDLR(x_dt_get_utc);
    RPC_REG_OP_HNDLR(x_dt_leap_yr);
    RPC_REG_OP_HNDLR(x_dt_reg_nfy_fct);
    RPC_REG_OP_HNDLR(x_dt_set_dst);
    RPC_REG_OP_HNDLR(x_dt_set_dst_ctrl);
    RPC_REG_OP_HNDLR(x_dt_set_sync_src);
    RPC_REG_OP_HNDLR(x_dt_set_tz);
    RPC_REG_OP_HNDLR(x_dt_set_utc);
    RPC_REG_OP_HNDLR(x_dt_sync_with_brdcst_utc);
    RPC_REG_OP_HNDLR(x_dt_utc_sec_to_dtg);
    RPC_REG_OP_HNDLR(x_dt_utc_sec_to_loc_dtg);
    RPC_REG_OP_HNDLR(x_dt_get_tz);
    RPC_REG_OP_HNDLR(x_dt_get_dst);
    RPC_REG_OP_HNDLR(x_dt_get_tz_ctrl);
    RPC_REG_OP_HNDLR(x_dt_get_dst_ctrl);
    RPC_REG_OP_HNDLR(x_dt_set_sys_count_code);
    RPC_REG_OP_HNDLR(x_dt_get_sys_count_code);
    RPC_REG_OP_HNDLR(x_dt_get_count_code);
    RPC_REG_OP_HNDLR(x_dt_get_num_count_code);
    RPC_REG_OP_HNDLR(x_dt_get_gps);
    RPC_REG_OP_HNDLR(x_dt_gps_sec_to_utc_sec);
    RPC_REG_OP_HNDLR(x_dt_bcd_to_sec);
    RPC_REG_OP_HNDLR(x_dt_mjd_bcd_to_dtg);
    RPC_REG_OP_HNDLR(x_dt_mjd_to_dtg);
    RPC_REG_OP_HNDLR(x_dt_set_tz_ctrl);
    RPC_REG_OP_HNDLR(x_dt_check_input_time);
    RPC_REG_OP_HNDLR(x_dt_config_check_input_time);
    RPC_REG_OP_HNDLR(x_dt_set_config);
    RPC_REG_OP_HNDLR(x_dt_get_ds_change);
    RPC_REG_OP_HNDLR(x_dt_set_ds_change);
    RPC_REG_OP_HNDLR(x_dt_get_ds_offset);
    RPC_REG_OP_HNDLR(x_dt_set_ds_offset);
    RPC_REG_OP_HNDLR(x_dt_dtg_to_mjd);
    RPC_REG_OP_HNDLR(x_dt_dtg_to_mjd_bcd);
    RPC_REG_OP_HNDLR(x_dt_get_last_sync_tbl_id);

    return RPCR_OK;
}
