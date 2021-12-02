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

#include "svl_bldr/sb_slctr.h"
#include "svl_bldr/c_sb.h"
#include "svl_bldr/sb_atsc_eng/u_sb_atsc_eng.h"
#include "svl_bldr/sb_ntsc_eng/u_sb_ntsc_eng.h"
#include "svl_bldr/sb_cqam_eng/u_sb_cqam_eng.h"
#include "svl_bldr/sb_dvb_eng/u_sb_dvb_eng.h"
#include "svl_bldr/sb_dvbc_eng/u_sb_dvbc_eng.h"
#include "svl_bldr/sb_pal_secam_eng/u_sb_pal_secam_eng.h"
#include "svl_bldr/sb_isdb_eng/u_sb_isdb_eng.h"
#include "rpc.h"
#include "ri_common.h"

#include "ri_desc.h"





static INT32 _x_sb_open_builder(
    const CHAR*         ps_builder_name,
    VOID*               pv_open_data,
    VOID*               pv_nfy_tag,
    x_sb_nfy_fct        pf_nfy,
    HANDLE_T*           ph_builder,
    SB_COND_T*          pe_builder_cond)
{
    RPC_DECL(6, INT32);    

    
    
    if(strcmp(ps_builder_name, "sb_atsc") == 0)
    {
        SB_ATSC_OPEN_DATA_T * pt_atsc_data = pv_open_data;
        
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_open_data, RPC_DESC_SB_ATSC_OPEN_DATA_T, NULL));
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_atsc_data->ps_file_name));
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_atsc_data->ps_svl_name));
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_atsc_data->ps_tuner_name));
    }
    else if(strncmp(ps_builder_name, "sb_ntsc", strlen("sb_ntsc")) == 0)
    {    
        SB_NTSC_OPEN_DATA_T * pt_ntsc_data = pv_open_data;
        /*TMP use SB_ATSC_OPEN_DATA_T*/
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_open_data, RPC_DESC_SB_ATSC_OPEN_DATA_T, NULL));
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_ntsc_data->ps_file_name));
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_ntsc_data->ps_svl_name));
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_ntsc_data->ps_tuner_name));
    }
    else if(strcmp(ps_builder_name, "sb_cqam") == 0)
    {
        SB_CQAM_OPEN_DATA_T * pt_cqam_data = pv_open_data;
        /*TMP use SB_ATSC_OPEN_DATA_T*/
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_open_data, RPC_DESC_SB_CQAM_OPEN_DATA_T, NULL));
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_cqam_data->ps_file_name));
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_cqam_data->ps_svl_name));
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_cqam_data->ps_tuner_name));
    }
    else if(strcmp(ps_builder_name, "sb_dvb") == 0)
    {
        SB_DVB_OPEN_DATA_T * pt_dvb_data = pv_open_data;
        /*TMP use SB_DVB_OPEN_DATA_T*/
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_open_data, RPC_DESC_SB_DVB_OPEN_DATA_T, NULL));
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_dvb_data->ps_file_name));
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_dvb_data->ps_svl_name));
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_dvb_data->ps_tuner_name));
    }
    else if(strcmp(ps_builder_name, "sb_dvbc") == 0)
    {
        SB_DVBC_OPEN_DATA_T * pt_dvbc_data = pv_open_data;
        /*TMP use SB_DVBC_OPEN_DATA_T*/
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_open_data, RPC_DESC_SB_DVBC_OPEN_DATA_T, NULL));
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_dvbc_data->ps_file_name));
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_dvbc_data->ps_svl_name));
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_dvbc_data->ps_tuner_name));
    }
    else if(strcmp(ps_builder_name, PAL_SECAM_ENG_NAME) == 0)
    {
        SB_PAL_SECAM_OPEN_DATA_T*  pt_pal_secam_data = pv_open_data;
        /*TMP use SB_PAL_SECAM_OPEN_DATA_T*/
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_open_data, RPC_DESC_SB_PAL_SECAM_OPEN_DATA_T, NULL));
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_pal_secam_data->ps_file_name));
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_pal_secam_data->ps_svl_name));
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_pal_secam_data->ps_tuner_name));
    }
    else if(strcmp(ps_builder_name, ISDB_ENG_NAME) == 0)
    {
        SB_ISDB_OPEN_DATA_T * pt_isdb_data = pv_open_data;
        /*TMP use SB_ISDB_OPEN_DATA_T*/
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_open_data, RPC_DESC_SB_ISDB_OPEN_DATA_T, NULL));
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_isdb_data->ps_file_name));
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_isdb_data->ps_svl_name));
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_isdb_data->ps_tuner_name));
    }
    else
    {
        RI_LOG("ENG:%s not support\n", ps_builder_name);
        RPC_FAIL;
    }
    /* Other builder engines.TMP not support
    else if(strcmp(ps_builder_name, "sb_dvb"))
    {
    }
    */
    
    
    RPC_ARG_INP(ARG_TYPE_REF_STR,    ps_builder_name);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,   pv_open_data);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_builder);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_builder_cond);

    RPC_DO_OP("x_sb_open_builder");
    
    RI_INF("RW open builder 0x%x, %s\n", *ph_builder, ps_builder_name);

    RPC_RETURN(ARG_TYPE_INT32, SBR_INV_ARG);
	
}




INT32 _hndlr_x_sb_scan_nfy_fct(
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
    
    pt_return->e_type   = ARG_TYPE_VOID;
    
    ((x_sb_scan_nfy_fct)pv_cb_addr)(
        (HANDLE_T)pt_args[0].u.ui4_arg,
                  pt_args[1].u.ui4_arg,
                  pt_args[2].u.i4_arg,
                  pt_args[3].u.pv_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_sb_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }   
    
    pt_return->e_type   = ARG_TYPE_VOID;
    
    ((x_sb_nfy_fct)pv_cb_addr)(
            (HANDLE_T)pt_args[0].u.ui4_arg,
           (SB_COND_T)pt_args[1].u.ui4_arg,
                      pt_args[2].u.pv_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_sb_dvb_svc_update_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }   
    
    pt_return->e_type   = ARG_TYPE_VOID;
    
    ((x_sb_svc_update_nfy_fct)pv_cb_addr)(
        (SB_DVB_SVC_UPDATE_NFY_DATA*)pt_args[0].u.pv_desc,
                                     pt_args[1].u.pv_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_sb_new_svc_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }   
    
    pt_return->e_type   = ARG_TYPE_VOID;
    
    ((x_sb_new_svc_nfy_fct)pv_cb_addr)(
        (SB_NEW_SVC_NFY_DATA*)pt_args[0].u.pv_desc,
                              pt_args[1].u.pv_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_sb_dvb_nw_change_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }   
    
    pt_return->e_type   = ARG_TYPE_VOID;
    
    ((x_sb_nw_change_nfy_fct)pv_cb_addr)(
                       pt_args[0].u.pv_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_sb_dvbc_svc_update_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }   
    
    pt_return->e_type   = ARG_TYPE_VOID;
    
    ((x_sb_dvbc_svc_update_nfy_fct)pv_cb_addr)(
        (SB_DVBC_SVC_UPDATE_NFY_DATA*)pt_args[0].u.pv_desc,
                                     pt_args[1].u.pv_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_sb_dvbc_nw_change_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }   
    
    pt_return->e_type   = ARG_TYPE_VOID;
    
    ((x_sb_dvbc_nw_change_nfy_fct)pv_cb_addr)(
                       pt_args[0].u.pv_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_sb_pal_secam_freq_progress_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }  

    pt_return->e_type   = ARG_TYPE_VOID;

    printf("_hndlr_x_sb_pal_secam_freq_progress_nfy_fct \r\n");
    ((x_sb_freq_progress_nfy_fct)pv_cb_addr)(
        (SB_PAL_SECAM_FREQ_PROGRESS_NFY_DATA*)pt_args[0].u.pv_desc,
                                              pt_args[1].u.pv_arg);
    
    return RPCR_OK;
}

INT32 _hndlr_x_sb_pal_secam_anas_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }  

    pt_return->e_type   = ARG_TYPE_VOID;

    printf("_hndlr_x_sb_pal_secam_anas_nfy_fct \r\n");
    ((x_sb_anas_nfy_fct)pv_cb_addr)(
        (SB_PAL_SECAM_ANAS_NFY_DATA*)pt_args[0].u.pv_desc,
                                     pt_args[1].u.pv_arg);
    
    return RPCR_OK;

}


INT32 _hndlr_x_sb_pal_secam_aci_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }  

    pt_return->e_type   = ARG_TYPE_VOID;

    printf("_hndlr_x_sb_pal_secam_aci_nfy_fct \r\n");
    ((x_sb_aci_nfy_fct)pv_cb_addr)(
      (SB_PAL_SECAM_ACI_NFY_DATA*)pt_args[0].u.pv_desc,
                                  pt_args[1].u.pv_arg);
    
    return RPCR_OK;

}

INT32 _hndlr_x_sb_pal_secam_scan_map_fct(
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

    printf("_hndlr_x_sb_pal_secam_scan_map_fct\r\n");
    #if 1
    ((x_sb_pal_secam_scan_maps_fct)pv_cb_addr)
        (
            pt_args[0].u.ui4_arg,
            pt_args[1].u.ps_str,    //ISO_3166_COUNT_T
            pt_args[2].u.pv_arg,
            pt_args[3].u.pv_arg,
            pt_args[4].u.pz_arg
        );
    #endif
    return RPCR_OK;

}



INT32 _hndlr_x_sb_dvb_scan_map_fct(
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

    printf("_hndlr_x_sb_dvb_scan_map_fct\r\n");
    #if 1
    ((x_sb_dvb_scan_maps_fct)pv_cb_addr)
        (
            pt_args[0].u.ui4_arg,
            pt_args[1].u.ps_str,    //ISO_3166_COUNT_T
            pt_args[2].u.pv_arg,
            pt_args[3].u.pv_arg,
            pt_args[4].u.pz_arg
        );
    #endif
    return RPCR_OK;

}

INT32 _hndlr_x_sb_dvbc_scan_map_fct(
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

    printf("_hndlr_x_sb_dvbc_scan_map_fct\r\n");
    #if 1
    ((x_sb_dvbc_scan_maps_fct)pv_cb_addr)
        (
            pt_args[0].u.ui4_arg,
            pt_args[1].u.ps_str,    //ISO_3166_COUNT_T
            pt_args[2].u.ui4_arg,    //OPeration name
            pt_args[3].u.pv_arg,
            pt_args[4].u.pv_arg,
            pt_args[5].u.pv_arg,
            pt_args[6].u.pz_arg
        );
    #endif
    return RPCR_OK;

}

INT32 _hndlr_x_sb_isdb_scan_map_fct(
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

    printf("_hndlr_x_sb_isdb_scan_map_fct\r\n");
    #if 1
    ((x_sb_isdb_scan_maps_fct)pv_cb_addr)
        (
            pt_args[0].u.ui4_arg,
            pt_args[1].u.ps_str,
            pt_args[2].u.pv_arg,
            pt_args[3].u.pv_arg,
            pt_args[4].u.pz_arg
        );
    #endif
    return RPCR_OK;

}


INT32 c_sb_reg_scan_map(
    const CHAR *        ps_eng_name, 
    VOID*               pv_nfy_fct)
{
    RPC_DECL(2, INT32);

    RI_LOG("RW register scan map with engine name[%s]\n", ps_eng_name);
 
    
    RPC_ARG_INP(ARG_TYPE_REF_CHAR,   ps_eng_name);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pv_nfy_fct);

    RPC_DO_OP("x_sb_reg_scan_map");

    RPC_RETURN(ARG_TYPE_INT32, SBR_INV_ARG);
}

BOOL c_sb_is_inited(VOID)
{
    return SBR_NOT_SUPPORT;
}

INT32 c_sb_open_builder(
    const CHAR*         ps_builder_name,
    VOID*               pv_open_data,
    VOID*               pv_nfy_tag,
    x_sb_nfy_fct        pf_nfy,
    HANDLE_T*           ph_builder,
    SB_COND_T*          pe_builder_cond)
{
    INT32     i4_res;    
    
    i4_res = _x_sb_open_builder(ps_builder_name,
                               pv_open_data,
                               pv_nfy_tag,
                               pf_nfy,
                               ph_builder,
                               pe_builder_cond);

    
    
    return i4_res;
}


INT32 c_sb_close_builder(
    HANDLE_T            h_builder)
{
    RPC_DECL(1, INT32);    

    RPC_ARG_OUT(ARG_TYPE_UINT32, h_builder);

    RPC_DO_OP("x_sb_close_builder");

    RPC_RETURN(ARG_TYPE_INT32, SBR_INV_ARG);
} 


INT32 c_sb_start_scan(
    HANDLE_T            h_builder,
    VOID*               pv_scan_data,
    VOID*               pv_nfy_tag,
    x_sb_scan_nfy_fct   pf_nfy,
    SB_COND_T*          pe_builder_cond)
{
    RPC_DECL(5, INT32);

    RI_LOG("RW start scan 0x%x\n", h_builder);
 
    /*RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_scan_data, RPC_DESC_SB_ATSC_SCAN_DATA_T, NULL));*/
    /*In DVB engine case, it must be pre-process in APP level, 
      because it do not know which svl builder engine by this parameter.*/
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_builder);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,   pv_scan_data);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_builder_cond);

    RPC_DO_OP("x_sb_start_scan");

    RPC_RETURN(ARG_TYPE_INT32, SBR_INV_ARG);
}


INT32 c_sb_cancel_scan(
    HANDLE_T            h_builder)
{    
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_builder);
    
    RPC_DO_OP("x_sb_cancel_scan");

    RPC_RETURN(ARG_TYPE_INT32, SBR_INV_ARG);
}


INT32 c_sb_save_lists(
    HANDLE_T            h_builder,
    const CHAR*         ps_file_name)
{
    return SBR_NOT_SUPPORT;
}


INT32 c_sb_load_lists(
    HANDLE_T            h_builder,
    const CHAR*         ps_file_name)
{
        return SBR_NOT_SUPPORT;
}


INT32 c_sb_clean_lists(
    HANDLE_T            h_builder,
    const CHAR*         ps_file_name)
{
        return SBR_NOT_SUPPORT;
}


INT32 c_sb_get(
    HANDLE_T            h_builder,
    SB_KEY_TYPE_T       e_key_type,
    VOID*               pv_key_info,
    SIZE_T*             pz_key_data_len,
    VOID*               pv_key_data)
{
    RPC_DECL(5, INT32);    

    RI_LOG("RW sb get  0x%x, %d\n", h_builder, e_key_type);
    if(pz_key_data_len != NULL)
    {
         RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_key_data, *pz_key_data_len));
    }  
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_builder);
    RPC_ARG_INP(ARG_TYPE_INT32,      e_key_type);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_key_info);
    RPC_ARG_IO (ARG_TYPE_REF_SIZE_T, pz_key_data_len);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,   pv_key_data);
    
    RPC_DO_OP("x_sb_get");

    RPC_RETURN(ARG_TYPE_INT32, SBR_INV_ARG);
	
}


INT32 c_sb_set(
    HANDLE_T            h_builder,
    SB_KEY_TYPE_T       e_key_type,
    VOID*               pv_key_info,
    SIZE_T              z_key_data_len,
    VOID*               pv_key_data)
{
    RPC_DECL(5, INT32);    
    RI_LOG("RW sb set  0x%x, %d\n", h_builder, e_key_type);
    if(z_key_data_len == 0)
    {
        /* ........ */
        
    }
    else if(   e_key_type == SB_KEY_TYPE_SET_TVD_SNK_NAME )
    {
        /* For string, the caller seems not pass the length including '\0' */
        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_key_data, z_key_data_len + 1));
    } 
    else if (!(e_key_type == SB_KEY_TYPE_RESET_TVD_SNK_NAME
            || e_key_type == SB_KEY_TYPE_FAV_NET
            || e_key_type == SB_KEY_TYPE_NW_CHANGE_NFY /* caller in AP should RPC_ADD_REF_DESC, 
            because that is different struct in DVBC, DVBT and  module */
            ))
    {
        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_key_data, z_key_data_len));
    }    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_builder);
    RPC_ARG_INP(ARG_TYPE_INT32,      e_key_type);
    /* I just found NULL */
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_key_info);
    RPC_ARG_INP(ARG_TYPE_SIZE_T,     z_key_data_len);

    if (e_key_type == SB_KEY_TYPE_RESET_TVD_SNK_NAME
        || e_key_type == SB_KEY_TYPE_FAV_NET)
    {
        RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_key_data);
    }
    else
    {
        RPC_ARG_INP(ARG_TYPE_REF_DESC,   pv_key_data);
    }
    
    RPC_DO_OP("x_sb_set");

    RPC_RETURN(ARG_TYPE_INT32, SBR_INV_ARG);
}


INT32 c_sb_get_cond(
    HANDLE_T            h_builder,
    SB_COND_T*          pe_builder_cond)
{
       return SBR_NOT_SUPPORT;
}

 
UINT16 c_sb_get_dbg_level(VOID)
{
       return SBR_NOT_SUPPORT;
}


BOOL c_sb_set_dbg_level(UINT16 ui2_level)
{
        return SBR_NOT_SUPPORT;
} 



INT32 c_rpc_reg_sb_cb_hndlrs()
{
    RPC_REG_CB_HNDLR(x_sb_nfy_fct);    
    RPC_REG_CB_HNDLR(x_sb_scan_nfy_fct);  
    RPC_REG_CB_HNDLR(x_sb_pal_secam_freq_progress_nfy_fct);  
    RPC_REG_CB_HNDLR(x_sb_pal_secam_anas_nfy_fct);  
    RPC_REG_CB_HNDLR(x_sb_pal_secam_aci_nfy_fct);  
    RPC_REG_CB_HNDLR(x_sb_pal_secam_scan_map_fct);  
    RPC_REG_CB_HNDLR(x_sb_dvb_svc_update_nfy_fct);  
    RPC_REG_CB_HNDLR(x_sb_new_svc_nfy_fct); 
    RPC_REG_CB_HNDLR(x_sb_dvb_nw_change_nfy_fct); 
    RPC_REG_CB_HNDLR(x_sb_dvbc_svc_update_nfy_fct); 
    RPC_REG_CB_HNDLR(x_sb_dvbc_nw_change_nfy_fct);
    /*scan maps function*/
    RPC_REG_CB_HNDLR(x_sb_dvb_scan_map_fct);
    RPC_REG_CB_HNDLR(x_sb_dvbc_scan_map_fct);
    RPC_REG_CB_HNDLR(x_sb_isdb_scan_map_fct);

    return RPCR_OK;
}


