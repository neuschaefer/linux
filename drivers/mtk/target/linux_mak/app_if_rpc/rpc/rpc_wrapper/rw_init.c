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
#include <time.h>
#include <sys/timeb.h>
#if 0
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

static INT32 rpc_pf_test(VOID * pv_data, SIZE_T z_size)
{
    RPC_DECL(1, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_data, z_size));
    
    RPC_ARG_INP(ARG_TYPE_REF_DESC,   pv_data);

    RPC_DO_OP("rpc_pf_test");

    RPC_RETURN(ARG_TYPE_INT32, 0);	
}

static UINT32 _get_time()
{
    struct timeb tp;
    ftime(&tp);
    return tp.time * 1000 + tp.millitm;
}

INT32 do_rpc_pf_test()
{
    int i;
    SIZE_T z_size;
    VOID * pv_data;
    UINT32 ui4_time;
    INT32 i4_r;

    pv_data = malloc(1000);

    for(z_size = 10; z_size <= 1000; z_size *= 10)
    {
        ui4_time = _get_time();
        for(i = 0; i < 1000; i ++)
        {
            i4_r = rpc_pf_test_api(pv_data);
        }
        printf("%d bytes none-rpc: %d, ret %d\n", z_size, _get_time() - ui4_time, i4_r);
        ui4_time = _get_time();
        for(i = 0; i < 1000; i ++)
        {
            i4_r = rpc_pf_test(pv_data, z_size);
        }
        printf("%d bytes rpc: %d, ret %d\n", z_size, _get_time() - ui4_time, i4_r);
    }
    free(pv_data);
    
}
#endif

/* Function declarations added to resovle compiling warnings. */
extern INT32 c_rpc_reg_svctx_cb_hndlrs();
#ifdef ENABLE_MULTIMEDIA
extern INT32 c_rpc_reg_mm_svctx_cb_hndlrs();
extern INT32 c_rpc_reg_minfo_cb_hndlrs();
extern INT32 c_rpc_reg_midxbuld_cb_hndlrs();
#endif
#ifdef MW_CAP_LOGO_SUPPORT
extern INT32 c_rpc_reg_cap_cb_hndlrs();
#endif
extern INT32 c_rpc_reg_evctx_cb_hndlrs();
extern INT32 c_rpc_reg_flm_cb_hndlrs();
extern INT32 c_rpc_reg_svl_cb_hndlrs();
extern INT32 c_rpc_reg_svl_sort_cmp_hndlrs();
extern INT32 c_rpc_reg_svl_search_cmp_hndlrs();
extern INT32 c_rpc_reg_tsl_cb_hndlrs();
extern INT32 c_rpc_reg_tm_cb_hndlrs();
/*    c_rpc_reg_atsc_si_cb_hndlrs();*/ /* replace with tm cb */
#ifndef NO_NWL_INIT
extern INT32 c_rpc_reg_nwl_cb_hndlrs();
#endif
#ifndef NO_LOL_INIT
extern INT32 c_rpc_reg_lol_cb_hndlrs();
#endif
extern INT32 c_rpc_reg_sm_cb_hndlrs();
extern INT32 c_rpc_reg_sb_cb_hndlrs();
extern INT32 c_rpc_reg_dt_cb_hndlrs();
extern INT32 c_rpc_reg_rrctx_cb_hndlrs();
extern INT32 c_rpc_reg_cm_cb_hndlrs();
extern INT32 c_rpc_reg_dsm_cb_hndlrs();
extern INT32 c_rpc_reg_pcl_cb_hndlrs();
extern INT32 c_rpc_reg_iom_cb_hndlrs();
    
extern INT32 c_rpc_reg_handle_cb_hndlrs();
extern INT32 c_rpc_reg_cecm_cb_hndlrs();
#ifdef MW_DVBS_DEV_ENABLE
#ifndef NO_SATL_INIT
extern INT32 c_rpc_reg_satl_cb_hndlrs();
#endif
#endif

#ifndef NO_LST_MNGR_INIT
extern INT32 c_rpc_reg_lst_mngr_cb_hndlrs();
#endif
#ifdef MW_ISDB_SUPPORT
extern INT32 c_rpc_reg_df_mngr_cb_hndlrs();
#endif

#ifdef ANDROID
extern INT32 c_rpc_reg_brdcst_cb_hndlrs();
extern INT32 c_rpc_reg_input_cb_hndlrs();
extern INT32 c_rpc_reg_scan_cb_hndlrs();
extern INT32 c_rpc_reg_dvbc_scan_cb_hndlrs();
extern INT32 c_rpc_reg_event_cb_hndlrs();
extern INT32 c_rpc_reg_channel_cb_hndlrs();
#endif
#ifdef ANDROID
extern INT32 c_rpc_reg_ci_cb_hndlrs();
extern INT32 c_rpc_reg_acfg_cb_hndlrs(VOID);
#endif
extern INT32 c_rpc_reg_chip_spec_cb_hndlrs();

INT32 c_rpc_init_client()
{    
    c_rpc_init();

   /* do_rpc_pf_test();*/
    
    /* Place CB handler registration here */
    c_rpc_reg_svctx_cb_hndlrs();
#ifdef ENABLE_MULTIMEDIA
    c_rpc_reg_mm_svctx_cb_hndlrs();
    c_rpc_reg_minfo_cb_hndlrs();
    c_rpc_reg_midxbuld_cb_hndlrs();
#endif
#ifdef MW_CAP_LOGO_SUPPORT
    c_rpc_reg_cap_cb_hndlrs();
#endif
    c_rpc_reg_evctx_cb_hndlrs();
    c_rpc_reg_flm_cb_hndlrs();
    c_rpc_reg_svl_cb_hndlrs();
    c_rpc_reg_svl_sort_cmp_hndlrs();
    c_rpc_reg_svl_search_cmp_hndlrs();
    c_rpc_reg_tsl_cb_hndlrs();
    c_rpc_reg_tm_cb_hndlrs();
/*    c_rpc_reg_atsc_si_cb_hndlrs();*/ /* replace with tm cb */
#ifndef NO_NWL_INIT
    c_rpc_reg_nwl_cb_hndlrs();
#endif
#ifndef NO_LOL_INIT
    c_rpc_reg_lol_cb_hndlrs();
#endif
    c_rpc_reg_sm_cb_hndlrs();
    c_rpc_reg_sb_cb_hndlrs();
    c_rpc_reg_dt_cb_hndlrs();
    c_rpc_reg_rrctx_cb_hndlrs();
    c_rpc_reg_cm_cb_hndlrs();
    c_rpc_reg_dsm_cb_hndlrs();
    c_rpc_reg_pcl_cb_hndlrs();
    c_rpc_reg_iom_cb_hndlrs();
    
    c_rpc_reg_handle_cb_hndlrs();
    c_rpc_reg_cecm_cb_hndlrs();
#ifdef MW_DVBS_DEV_ENABLE
#ifndef NO_SATL_INIT
    c_rpc_reg_satl_cb_hndlrs();
#endif
#endif

#ifndef NO_LST_MNGR_INIT
    c_rpc_reg_lst_mngr_cb_hndlrs();
#endif
#ifdef MW_ISDB_SUPPORT
    c_rpc_reg_df_mngr_cb_hndlrs();
#endif

#ifdef ANDROID
    c_rpc_reg_brdcst_cb_hndlrs();
    c_rpc_reg_input_cb_hndlrs();
    c_rpc_reg_scan_cb_hndlrs();
    c_rpc_reg_dvbc_scan_cb_hndlrs();
    c_rpc_reg_event_cb_hndlrs();
    c_rpc_reg_channel_cb_hndlrs();
#endif
#ifdef ANDROID
    c_rpc_reg_ci_cb_hndlrs();
    c_rpc_reg_acfg_cb_hndlrs();
#endif
    c_rpc_reg_chip_spec_cb_hndlrs();
	c_rpc_reg_img_cb_hndlrs();

    return RPCR_OK;
}



