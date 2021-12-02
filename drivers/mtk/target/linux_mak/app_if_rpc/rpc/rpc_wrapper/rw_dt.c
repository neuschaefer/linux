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

INT32 _x_dt_reg_nfy_fct
(
    x_dt_nfy_fct   pf_nfy,
    VOID*          pv_tag,
    HANDLE_T*      ph_dt
)
{
    RPC_DECL(3, INT32);

    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_tag);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_dt);

    RPC_DO_OP("x_dt_reg_nfy_fct");

    RPC_RETURN(ARG_TYPE_INT32, DTR_FAIL);
}

INT32 _hndlr_x_dt_nfy_fct(
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

    ((x_dt_nfy_fct)pv_cb_addr)(
            (HANDLE_T)pt_args[0].u.ui4_arg,
                      pt_args[1].u.pv_arg,
           (DT_COND_T)pt_args[2].u.ui4_arg,
              (TIME_T)pt_args[3].u.i8_arg);

    return RPCR_OK;
}

INT32 c_dt_add(const DTG_T* pt_dtg_old, TIME_T t_sec, DTG_T* pt_dtg_new)
{
    RPC_DECL(3, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_dtg_old, RPC_DESC_DTG_T, NULL));
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_dtg_new, RPC_DESC_DTG_T, NULL));

    RPC_ARG_INP(ARG_TYPE_REF_DESC,  pt_dtg_old);
    RPC_ARG_INP(ARG_TYPE_INT64,     t_sec);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_dtg_new);

    RPC_DO_OP("x_dt_add");

    RPC_RETURN(ARG_TYPE_INT32, DTR_INV_ARG);
}

INT32 c_dt_conv_utc_local(const DTG_T* pt_input_dtg,  DTG_T* pt_output_dtg)
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_input_dtg, RPC_DESC_DTG_T, NULL));
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_output_dtg, RPC_DESC_DTG_T, NULL));

    RPC_ARG_INP(ARG_TYPE_REF_DESC,  pt_input_dtg);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_output_dtg);

    RPC_DO_OP("x_dt_conv_utc_local");

    RPC_RETURN(ARG_TYPE_INT32, DTR_INV_ARG);
}

TIME_T c_dt_diff(const DTG_T* pt_dtg_from, const DTG_T*  pt_dtg_to)
{
    RPC_DECL(2, TIME_T);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_dtg_from, RPC_DESC_DTG_T, NULL));
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_dtg_to, RPC_DESC_DTG_T, NULL));

    RPC_ARG_INP(ARG_TYPE_REF_DESC,  pt_dtg_from);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,  pt_dtg_to);

    RPC_DO_OP("x_dt_diff");

    RPC_RETURN(ARG_TYPE_INT64, NULL_TIME);
}


TIME_T c_dt_dtg_to_sec(const DTG_T* pt_dtg)
{
    RPC_DECL(1, TIME_T);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_dtg, RPC_DESC_DTG_T, NULL));

    RPC_ARG_INP(ARG_TYPE_REF_DESC,  pt_dtg);

    RPC_DO_OP("x_dt_dtg_to_sec");

    RPC_RETURN(ARG_TYPE_INT64, NULL_TIME);
}

TIME_T c_dt_get_brdcst_utc
(
    UINT16*       pui2_milli_sec,
    DT_COND_T*    pt_dt_cond
)
{
    RPC_DECL(2, TIME_T);

    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,    pui2_milli_sec);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pt_dt_cond);

    RPC_DO_OP("x_dt_get_brdcst_utc");

    RPC_RETURN(ARG_TYPE_INT64, NULL_TIME);
}

TIME_T c_dt_get_utc(UINT16*  pui2_milli_sec, DT_COND_T* pt_dt_cond )
{
    RPC_DECL(2, TIME_T);

    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,    pui2_milli_sec);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pt_dt_cond);

    RPC_DO_OP("x_dt_get_utc");

    RPC_RETURN(ARG_TYPE_INT64, NULL_TIME);
}

BOOL c_dt_leap_yr(TIME_T t_yr)
{
    RPC_DECL(1, BOOL);

    RPC_ARG_INP(ARG_TYPE_INT64, t_yr);

    RPC_DO_OP("x_dt_leap_yr");

    RPC_RETURN(ARG_TYPE_BOOL, FALSE);
}

INT32 c_dt_reg_nfy_fct
(
    x_dt_nfy_fct   pf_nfy,
    VOID*          pv_tag,
    HANDLE_T*      ph_dt
)
{
    INT32     i4_rc;

    i4_rc = _x_dt_reg_nfy_fct(pf_nfy, pv_tag, ph_dt);

    return i4_rc;
}


VOID c_dt_set_dst(BOOL b_dls)
{
    RPC_DECL_VOID(1);

    RPC_ARG_INP(ARG_TYPE_BOOL, b_dls);

    RPC_DO_OP("x_dt_set_dst");

    RPC_RETURN_VOID;
}

VOID c_dt_set_dst_ctrl(BOOL b_dls)
{
    RPC_DECL_VOID(1);

    RPC_ARG_INP(ARG_TYPE_BOOL, b_dls);

    RPC_DO_OP("x_dt_set_dst_ctrl");

    RPC_RETURN_VOID;
}

INT32 c_dt_set_sync_src(DT_SYNC_SRC_TYPE_T t_sync_src_type,
                        DT_SRC_TYPE_DESC_T t_src_desc_type,
                        const VOID* pv_src_info)
{
    RPC_DECL(3, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,    t_sync_src_type);
    RPC_ARG_INP(ARG_TYPE_UINT32,    t_src_desc_type);
    RPC_ARG_INP(ARG_TYPE_REF_STR,   pv_src_info);

    RPC_DO_OP("x_dt_set_sync_src");

    RPC_RETURN(ARG_TYPE_INT32, DTR_FAIL);
}

VOID c_dt_set_tz(TIME_T t_tz_offset)
{
    RPC_DECL_VOID(1);

    RPC_ARG_INP(ARG_TYPE_INT64, t_tz_offset);

    RPC_DO_OP("x_dt_set_tz");

    RPC_RETURN_VOID;
}

INT32 c_dt_set_utc(TIME_T t_sec, UINT16 ui2_milli_sec)
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_INT64,     t_sec);
    RPC_ARG_INP(ARG_TYPE_UINT16,    ui2_milli_sec);

    RPC_DO_OP("x_dt_set_utc");

    RPC_RETURN(ARG_TYPE_INT32, DTR_FAIL);
}

INT32 c_dt_sync_with_brdcst_utc(BOOL b_sync_flag)
{
    RPC_DECL(1, INT32);

    RPC_ARG_INP(ARG_TYPE_BOOL, b_sync_flag);

    RPC_DO_OP("x_dt_sync_with_brdcst_utc");

    RPC_RETURN(ARG_TYPE_INT32, DTR_FAIL);
}

INT32 c_dt_utc_sec_to_dtg(TIME_T t_utc_sec, DTG_T* pt_dtg)
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_dtg, RPC_DESC_DTG_T, NULL));

    RPC_ARG_INP(ARG_TYPE_INT64,     t_utc_sec);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_dtg);

    RPC_DO_OP("x_dt_utc_sec_to_dtg");

    RPC_RETURN(ARG_TYPE_INT32, DTR_INV_ARG);
}

INT32 c_dt_utc_sec_to_loc_dtg(TIME_T t_utc_sec, DTG_T* pt_dtg)
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_dtg, RPC_DESC_DTG_T, NULL));

    RPC_ARG_INP(ARG_TYPE_INT64,     t_utc_sec);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_dtg);

    RPC_DO_OP("x_dt_utc_sec_to_loc_dtg");

    RPC_RETURN(ARG_TYPE_INT32, DTR_INV_ARG);
}

#if 1 /*Yan CLI*/
TIME_T c_dt_get_tz(VOID)
{
    RPC_DECL(0, TIME_T);

    RPC_DO_OP("x_dt_get_tz");

    RPC_RETURN(ARG_TYPE_INT64, DTR_INV_ARG);
}

BOOL c_dt_get_dst(VOID)
{
    RPC_DECL(0, BOOL);

    RPC_DO_OP("x_dt_get_dst");

    RPC_RETURN(ARG_TYPE_BOOL, DTR_INV_ARG);
}

BOOL c_dt_get_tz_ctrl(VOID)
{
    RPC_DECL(0, BOOL);

    RPC_DO_OP("x_dt_get_tz_ctrl");

    RPC_RETURN(ARG_TYPE_BOOL, DTR_INV_ARG);
}

BOOL c_dt_get_dst_ctrl(VOID)
{
    RPC_DECL(0, BOOL);

    RPC_DO_OP("x_dt_get_dst_ctrl");

    RPC_RETURN(ARG_TYPE_BOOL, DTR_INV_ARG);
}
#endif

INT32 c_dt_set_sys_count_code (ISO_3166_COUNT_T  t_count_code,
                               UINT16            ui2_region_id)
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_REF_STR,   t_count_code);
    RPC_ARG_INP(ARG_TYPE_UINT16,    ui2_region_id);

    RPC_DO_OP("x_dt_set_sys_count_code");

    RPC_RETURN(ARG_TYPE_INT32, DTR_FAIL);
}

INT32 c_dt_get_sys_count_code (ISO_3166_COUNT_T*  pt_count_code,
                               UINT16*            pui2_region_id)
{
    RPC_DECL(2, INT32);

    RPC_ARG_OUT(ARG_TYPE_REF_STR,   pt_count_code);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,pui2_region_id);

    RPC_DO_OP("x_dt_get_sys_count_code");

    RPC_RETURN(ARG_TYPE_INT32, DTR_INV_ARG);
}

INT32 c_dt_get_count_code (UINT16             ui2_idx,
                           ISO_3166_COUNT_T*  pt_count_code,
                           UINT16*            pui2_region_id,
                           TIME_T*            pt_tz_offset)
{
    RPC_DECL(4, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT16,    ui2_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_STR,   pt_count_code);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,pui2_region_id);
    RPC_ARG_OUT(ARG_TYPE_REF_INT64, pt_tz_offset);

    RPC_DO_OP("x_dt_get_count_code");

    RPC_RETURN(ARG_TYPE_INT32, DTR_INV_ARG);
}

UINT16 c_dt_get_num_count_code (VOID)
{
    RPC_DECL(0, UINT16);

    RPC_DO_OP("x_dt_get_num_count_code");

    RPC_RETURN(ARG_TYPE_UINT16, DTR_INV_ARG);
}

TIME_T c_dt_get_gps(INT32* pi4_gps_leap_sec, UINT16* pui2_milli_sec)
{
    RPC_DECL(2, TIME_T);

    RPC_ARG_OUT(ARG_TYPE_REF_INT32,     pi4_gps_leap_sec);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,    pui2_milli_sec);

    RPC_DO_OP("x_dt_get_gps");

    RPC_RETURN(ARG_TYPE_INT64, NULL_TIME);
}

TIME_T c_dt_gps_sec_to_utc_sec(TIME_T   t_gps_sec)
{
    RPC_DECL(1, TIME_T);

    RPC_ARG_INP(ARG_TYPE_INT64,    t_gps_sec);

    RPC_DO_OP("x_dt_gps_sec_to_utc_sec");

    RPC_RETURN(ARG_TYPE_INT64, NULL_TIME);
}

INT32 c_dt_bcd_to_sec(const UINT8* pui1_data,   TIME_T* pt_sec)
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pui1_data, 3));

    RPC_ARG_INP(ARG_TYPE_REF_DESC,  pui1_data);
    RPC_ARG_OUT(ARG_TYPE_REF_INT64, pt_sec);

    RPC_DO_OP("x_dt_bcd_to_sec");

    RPC_RETURN(ARG_TYPE_INT32, DTR_INV_ARG);
}

INT32 c_dt_mjd_bcd_to_dtg(const UINT8* pui1_data,   DTG_T* pt_dtg)
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pui1_data, 3));
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_dtg, RPC_DESC_DTG_T, NULL));

    RPC_ARG_INP(ARG_TYPE_REF_DESC,  pui1_data);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_dtg);

    RPC_DO_OP("x_dt_mjd_bcd_to_dtg");

    RPC_RETURN(ARG_TYPE_INT32, DTR_INV_ARG);
}

INT32 c_dt_mjd_to_dtg(UINT32 ui4_mjd, DTG_T*  pt_dtg)
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_dtg, RPC_DESC_DTG_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,    ui4_mjd);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_dtg);

    RPC_DO_OP("x_dt_mjd_to_dtg");

    RPC_RETURN(ARG_TYPE_INT32, DTR_INV_ARG);
}

VOID c_dt_set_tz_ctrl(BOOL b_tz)
{
    RPC_DECL_VOID(1);

    RPC_ARG_INP(ARG_TYPE_BOOL, b_tz);

    RPC_DO_OP("x_dt_set_tz_ctrl");

    RPC_RETURN_VOID;
}

INT32 c_dt_check_input_time(BOOL b_enable)
{
    RPC_DECL(1, INT32);

    RPC_ARG_INP(ARG_TYPE_BOOL, b_enable);

    RPC_DO_OP("x_dt_check_input_time");

    RPC_RETURN(ARG_TYPE_INT32, DTR_FAIL);
}

INT32 c_dt_config_check_input_time(DT_CHECK_TIME_CONFIG  e_set_parm,
                                   VOID*                 pv_value)
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,     e_set_parm);

    switch(e_set_parm)
    {
        case DT_SET_CONSECUTIVE_VAL_TO_CHK:
        case DT_SET_TIME_WINDOW_ADJUSTMENT:
            RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    pv_value);
            break;

        case DT_REJECT_BRDCST_TIME_BEFORE_THIS_DATE:
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_value, RPC_DESC_DTG_T, NULL));
            RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pv_value);
            break;

        default:
            RPC_ARG_OUT(ARG_TYPE_REF_VOID,    pv_value);
            break;
    }

    RPC_DO_OP("x_dt_config_check_input_time");

    RPC_RETURN(ARG_TYPE_INT32, DTR_FAIL);
}

INT32 c_dt_set_config(UINT32  ui4_flag)
{
    RPC_DECL(1, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_flag);

    RPC_DO_OP("x_dt_set_config");

    RPC_RETURN(ARG_TYPE_INT32, DTR_FAIL);
}

TIME_T c_dt_get_ds_change()
{
    RPC_DECL(0, TIME_T);

    RPC_DO_OP("x_dt_get_ds_change");

    RPC_RETURN(ARG_TYPE_INT64, DTR_FAIL);
}

VOID c_dt_set_ds_change(TIME_T t_ds_change)
{
    RPC_DECL_VOID(1);

    RPC_ARG_INP(ARG_TYPE_INT64, t_ds_change);

    RPC_DO_OP("x_dt_set_ds_change");

    RPC_RETURN_VOID;
}

TIME_T c_dt_get_ds_offset()
{
    RPC_DECL(0, TIME_T);

    RPC_DO_OP("x_dt_get_ds_offset");

    RPC_RETURN(ARG_TYPE_INT64, DTR_FAIL);
}

VOID c_dt_set_ds_offset(TIME_T t_ds_offset)
{
    RPC_DECL_VOID(1);

    RPC_ARG_INP(ARG_TYPE_INT64, t_ds_offset);

    RPC_DO_OP("x_dt_set_ds_offset");

    RPC_RETURN_VOID;
}

INT32 c_dt_dtg_to_mjd(const DTG_T * pt_dtg,
                      UINT16 * pui2_mjd)
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_dtg, RPC_DESC_DTG_T, NULL));

    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_dtg);
    RPC_ARG_INP(ARG_TYPE_REF_UINT16, pui2_mjd);

    RPC_DO_OP("x_dt_dtg_to_mjd");

    RPC_RETURN(ARG_TYPE_INT32, DTR_FAIL);
}

INT32 c_dt_dtg_to_mjd_bcd(const DTG_T * pt_dtg,
                          UINT16 * pui2_mjd,
                          UINT16 * pui2_hr_min,
                          UINT8 * pui1_sec)
{
    RPC_DECL(4, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_dtg, RPC_DESC_DTG_T, NULL));

    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_dtg);
    RPC_ARG_INP(ARG_TYPE_REF_UINT16, pui2_mjd);
    RPC_ARG_INP(ARG_TYPE_REF_UINT16, pui2_hr_min);
    RPC_ARG_INP(ARG_TYPE_REF_UINT8, pui1_sec);

    RPC_DO_OP("x_dt_dtg_to_mjd_bcd");

    RPC_RETURN(ARG_TYPE_INT32, DTR_FAIL);
}

UINT8 c_dt_get_last_sync_tbl_id()
{
    RPC_DECL(0, UINT8);

    RPC_DO_OP("x_dt_get_last_sync_tbl_id");

    RPC_RETURN(ARG_TYPE_UINT8, DTR_FAIL);
}

INT32 c_rpc_reg_dt_cb_hndlrs()
{
    RPC_REG_CB_HNDLR(x_dt_nfy_fct);

    return RPCR_OK;
}




