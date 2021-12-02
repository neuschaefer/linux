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
#include "svl_bldr/_sb_eng.h"
#include "svl_bldr/sb_slctr.h"
#include "svl_bldr/c_sb.h"
#include "svl_bldr/sb_dvb_eng/u_sb_dvb_eng.h"
#include "svl_bldr/sb_dvbc_eng/u_sb_dvbc_eng.h"
#include "svl_bldr/sb_pal_secam_eng/u_sb_pal_secam_eng.h"
#include "svl_bldr/sb_isdb_eng/u_sb_isdb_eng.h"
#include "rpc.h"
#include "ri_common.h"

#include "ri_desc.h"


VOID _x_sb_nfy_fct_wrapper(
      HANDLE_T          h_builder,
      SB_COND_T         e_builder_cond,
      VOID*             pv_nfy_tag)
{
    RPC_DECL_VOID(3);
    
    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_builder);
    RPC_ARG_INP(ARG_TYPE_INT32,    e_builder_cond);        
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);        

    RPC_DO_CB(pt_nfy_tag->t_id, "x_sb_nfy_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID; 
}


static VOID _x_sb_pal_secam_freq_progress_nfy_fct_wrapper (
        SB_PAL_SECAM_FREQ_PROGRESS_NFY_DATA*    pt_nfy_data,
        VOID*                                   pv_nfy_tag)
{
    RPC_DECL_VOID(2);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
    printf("_x_sb_pal_secam_freq_progress_nfy_fct_wrapper \r\n");

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_nfy_data, RPC_DESC_SB_PAL_SECAM_FREQ_PROGRESS_NFY_DATA, NULL));

    RPC_ARG_INP(ARG_TYPE_DESC,      pt_nfy_data);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,  pt_nfy_tag->pv_tag);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_sb_pal_secam_freq_progress_nfy_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID; 
}

static VOID _x_sb_pal_secam_anas_nfy_fct_wrapper (
        SB_PAL_SECAM_ANAS_NFY_DATA* pt_nfy_data,
        VOID*                       pv_nfy_tag)
{
    
    RPC_DECL_VOID(2);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
    printf("_x_sb_pal_secam_anas_nfy_fct_wrapper \r\n");
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_nfy_data, RPC_DESC_SB_PAL_SECAM_ANAS_NFY_DATA, NULL));

    RPC_ARG_INP(ARG_TYPE_DESC,      pt_nfy_data);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,  pt_nfy_tag->pv_tag);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_sb_pal_secam_anas_nfy_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID; 
    
}


static VOID _x_sb_pal_secam_aci_nfy_fct_wrapper (
        SB_PAL_SECAM_ACI_NFY_DATA*  pt_nfy_data,
        VOID*                       pv_nfy_tag)
{
    
    RPC_DECL_VOID(2);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
    printf("_x_sb_pal_secam_aci_nfy_fct_wrapper \r\n");
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_nfy_data, RPC_DESC_SB_PAL_SECAM_ACI_NFY_DATA, NULL));

    RPC_ARG_INP(ARG_TYPE_DESC,      pt_nfy_data);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,  pt_nfy_tag->pv_tag);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_sb_pal_secam_aci_nfy_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID; 
    
}

static VOID _x_sb_pal_secam_scan_map_wrapper(
    SCAN_MAPS_PAL_SECAM_CMD_T     t_cmd,
    ISO_3166_COUNT_T              t_country_code,
    VOID*                         pv_input,
    VOID*                         pv_output,
    SIZE_T*                       pz_output_len,
    VOID*                         pv_nfy_tag)
{
    RPC_DECL_VOID(5);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
    printf("_x_sb_pal_secam_scan_map_fct_wrapper \r\n");

    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_output, *pz_output_len));
    RPC_ARG_INP(ARG_TYPE_UINT32,      t_cmd);
    RPC_ARG_INP(ARG_TYPE_REF_STR,     t_country_code);
    RPC_ARG_INP(ARG_TYPE_VOID,        pv_input);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,    pv_output);
    RPC_ARG_IO(ARG_TYPE_REF_UINT32,   pz_output_len);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_sb_pal_secam_scan_map_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID; 
}

static VOID _x_sb_dvb_scan_map_wrapper(
    SCAN_MAPS_CMD_T               t_cmd,
    ISO_3166_COUNT_T              t_country_code,
    VOID*                         pv_input,
    VOID*                         pv_output,
    SIZE_T*                       pz_output_len,
    VOID*                         pv_nfy_tag)
{
    RPC_DECL_VOID(6);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
    printf("_x_sb_dvb_scan_map_wrapper \r\n");

    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_output, *pz_output_len));
    RPC_ARG_INP(ARG_TYPE_UINT32,      t_cmd);
    RPC_ARG_INP(ARG_TYPE_REF_STR,     t_country_code);

    if ((t_cmd == SCAN_MAPS_CMD_GET_SCAN_IDX_BY_RF_CHANNEL)||
        (t_cmd == SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING_BY_NAME))
    {
        RPC_ARG_INP(ARG_TYPE_REF_STR,        pv_input);
    }
    else if (t_cmd == SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING)
    {
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_input, RPC_DESC_SB_DVB_ON_TS_SVC_ID_T, NULL));   
        RPC_ARG_INP(ARG_TYPE_REF_DESC,        pv_input);
    }
    else
    {
        RPC_ARG_INP(ARG_TYPE_UINT32,        pv_input);
    }
    
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,    pv_output);
    RPC_ARG_IO(ARG_TYPE_REF_UINT32,   pz_output_len);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_sb_dvb_scan_map_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID; 
}

static VOID _x_sb_dvbc_scan_map_wrapper(
    DVBC_SCAN_MAPS_CMD_T            t_cmd,
    ISO_3166_COUNT_T                t_country_code,
    OPERATOR_NAME_T                 t_operator_name,
    VOID*                           pv_input,
    VOID*                           pv_input2,
    VOID*                           pv_output,
    SIZE_T*                         pz_output_len,
    VOID*                           pv_nfy_tag)
{
    RPC_DECL_VOID(7);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
    printf("_x_sb_dvbc_scan_map_wrapper \r\n");

    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_output, *pz_output_len));
    RPC_ARG_INP(ARG_TYPE_UINT32,      t_cmd);
    RPC_ARG_INP(ARG_TYPE_REF_STR,     t_country_code);
    RPC_ARG_INP(ARG_TYPE_UINT32,      t_operator_name);
    RPC_ARG_INP(ARG_TYPE_VOID,        pv_input);

    if (t_cmd == DVBC_SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING_BY_NAME)
    {
        RPC_ARG_INP(ARG_TYPE_REF_STR,        pv_input2);
    }
    else if (t_cmd == DVBC_SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING)
    {
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_input2, RPC_DESC_SB_DVBC_ON_TS_SVC_ID_T, NULL));   
        RPC_ARG_INP(ARG_TYPE_REF_DESC,        pv_input2);
    }
    else
    {
        RPC_ARG_INP(ARG_TYPE_UINT32,        pv_input2);
    }
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,    pv_output);
    RPC_ARG_IO(ARG_TYPE_REF_UINT32,   pz_output_len);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_sb_dvbc_scan_map_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID; 
}


static VOID _x_sb_isdb_scan_map_wrapper(
    SCAN_MAPS_ISDB_CMD_T          t_cmd,
    ISO_3166_COUNT_T              t_country_code,
    VOID*                         pv_input,
    VOID*                         pv_output,
    SIZE_T*                       pz_output_len,
    VOID*                         pv_nfy_tag)
{
    RPC_DECL_VOID(5);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
    printf("_x_sb_isdb_scan_map_wrapper \r\n");

    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_output, *pz_output_len));
    RPC_ARG_INP(ARG_TYPE_UINT32,      t_cmd);
    RPC_ARG_INP(ARG_TYPE_REF_STR,     t_country_code);
    RPC_ARG_INP(ARG_TYPE_VOID,        pv_input);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,    pv_output);
    RPC_ARG_IO(ARG_TYPE_REF_UINT32,   pz_output_len);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_sb_isdb_scan_map_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID; 
}


static VOID _x_sb_scan_nfy_fct_wrapper (
      HANDLE_T          h_builder,
      UINT32            ui4_nfy_reason,
      INT32             i4_nfy_data,
      VOID*             pv_nfy_tag)
{
    RPC_DECL_VOID(4);
    
    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_builder);
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_nfy_reason);        
    RPC_ARG_INP(ARG_TYPE_INT32,    i4_nfy_data);        
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);
    
    RPC_DO_CB(pt_nfy_tag->t_id, "x_sb_scan_nfy_fct", pt_nfy_tag->pv_cb_addr);
    
    
    RPC_RETURN_VOID; 
}

static VOID _x_sb_dvb_svc_update_nfy_fct_wrapper(
        SB_DVB_SVC_UPDATE_NFY_DATA* pt_nfy_data,
        VOID*                       pv_nfy_tag )
{
    /**/
    RPC_DECL_VOID(2);
    
    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_nfy_data, RPC_DESC_SB_DVB_SVC_UPDATE_NFY_DATA, NULL));
    
    RPC_ARG_INP(ARG_TYPE_DESC,   pt_nfy_data);     
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);
    
    RPC_DO_CB(pt_nfy_tag->t_id, "x_sb_svc_update_nfy_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID; 
}

static VOID _x_sb_dvbc_svc_update_nfy_fct_wrapper(
        SB_DVBC_SVC_UPDATE_NFY_DATA* pt_nfy_data,
        VOID*                       pv_nfy_tag )
{
    /**/
    RPC_DECL_VOID(2);
    
    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_nfy_data, RPC_DESC_SB_DVBC_SVC_UPDATE_NFY_DATA, NULL));
    
    RPC_ARG_INP(ARG_TYPE_DESC,   pt_nfy_data);     
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);
    
    RPC_DO_CB(pt_nfy_tag->t_id, "x_sb_dvbc_svc_update_nfy_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID; 
}

static VOID _x_sb_new_svc_nfy_fct_wrapper(
        SB_NEW_SVC_NFY_DATA* 	pt_nfy_data,
        VOID*                   pv_nfy_tag )
{
    /**/
    RPC_DECL_VOID(2);
    
    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_nfy_data, RPC_DESC_SB_NEW_SVC_NFY_DATA, NULL));
    
    RPC_ARG_INP(ARG_TYPE_DESC,   pt_nfy_data);     
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);
    
    RPC_DO_CB(pt_nfy_tag->t_id, "x_sb_new_svc_nfy_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID; 
}

static VOID _x_sb_dvb_nw_change_nfy_fct_wrapper(
        VOID*                   pv_nfy_tag )
{
    /**/
    RPC_DECL_VOID(1);
    
    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
      
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);
    
    RPC_DO_CB(pt_nfy_tag->t_id, "x_sb_nw_change_nfy_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID; 
}

static VOID _x_sb_dvbc_nw_change_nfy_fct_wrapper(
        VOID*                   pv_nfy_tag )
{
    /**/
    RPC_DECL_VOID(1);
    
    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
    
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);
    
    RPC_DO_CB(pt_nfy_tag->t_id, "x_sb_dvbc_nw_change_nfy_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID; 
}

static INT32 _hndlr_x_sb_reg_scan_map(    
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T *      pt_nfy_tag                      = NULL;
    /*RPC_CB_NFY_TAG_T *      pt_nfy_tag_pal_secam_scan_map   = NULL;*/


    VOID*        pf_nfy = NULL;
    
    #if 0
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }
	#endif

    if ( strncmp(pt_args[0].u.ps_str,DVB_ENG_NAME,strlen(DVB_ENG_NAME)) == 0)
    {
        if (pt_args[1].u.pv_func != NULL)
        {
            RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_args[1].u.pv_func, NULL); 
            pf_nfy    = _x_sb_dvb_scan_map_wrapper;
        }
        else
        {
            pf_nfy = NULL;
            pt_nfy_tag = NULL;
        }
    }
    else if ( strncmp(pt_args[0].u.ps_str,DVBC_ENG_NAME,strlen(DVBC_ENG_NAME)) == 0)
    {
        if (pt_args[1].u.pv_func != NULL)
        {
            RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_args[1].u.pv_func, NULL); 
            pf_nfy    = _x_sb_dvbc_scan_map_wrapper;
        }
        else
        {
            pf_nfy      = NULL;
            pt_nfy_tag = NULL;
        }
    }
    else if( strncmp(pt_args[0].u.ps_str,PAL_SECAM_ENG_NAME,strlen(PAL_SECAM_ENG_NAME)) == 0)
    {
        if (pt_args[1].u.pv_func != NULL)
        {
            RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_args[1].u.pv_func, NULL); 
            pf_nfy    = _x_sb_pal_secam_scan_map_wrapper;
        }
        else
        {
            pf_nfy      = NULL;
            pt_nfy_tag = NULL;
        }
    }
    else if( strncmp(pt_args[0].u.ps_str,ISDB_ENG_NAME,strlen(ISDB_ENG_NAME)) == 0)
    {
        if (pt_args[1].u.pv_func != NULL)
        {
            RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_args[1].u.pv_func, NULL); 
            pf_nfy   = _x_sb_isdb_scan_map_wrapper;
        }
        else
        {
            pf_nfy      = NULL;
            pt_nfy_tag = NULL;
        }
    }
    
    
    pt_return->e_type = ARG_TYPE_INT32;
    pt_return->u.i4_arg = 
        x_sb_reg_scan_map(
            pt_args[0].u.ps_str,
            pf_nfy,
            pt_nfy_tag);
    if(pt_return->u.i4_arg == SBR_OK)
    {
        //Do not bind the CB tag, it need free...
    }    

    return RPCR_OK;
}

static INT32 _hndlr_x_sb_open_builder(    
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T *      pt_nfy_tag                      = NULL;
    /*RPC_CB_NFY_TAG_T *      pt_nfy_tag_pal_secam_scan_map   = NULL;*/


    x_sb_nfy_fct        pf_nfy;
    
    
    if(ui4_num_args != 6)
    {
        return RPCR_INV_ARGS;
    } 
    
    if(pt_args[3].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy = _x_sb_nfy_fct_wrapper;
        RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_args[3].u.pv_func, pt_args[2].u.pv_arg);       
    }

    pt_return->e_type = ARG_TYPE_INT32;
    pt_return->u.i4_arg = 
        x_sb_open_builder(
            pt_args[0].u.ps_str,
            pt_args[1].u.pv_desc,
            pt_nfy_tag,
            pf_nfy,
            (HANDLE_T *)pt_args[4].u.pui4_arg,
            (SB_COND_T*)pt_args[5].u.pi4_arg);
    if(pt_return->u.i4_arg == SBR_OK)
    {
        ri_bind_cb_tag(*(HANDLE_T *)pt_args[4].u.pui4_arg, pt_nfy_tag);
    }    

    return RPCR_OK;
}


static INT32 _hndlr_x_sb_close_builder(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    /*RPC_CB_NFY_TAG_T * pt_nfy_tag;*/

    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    } 
    
    pt_return->e_type = ARG_TYPE_INT32;
    pt_return->u.i4_arg = x_sb_close_builder(pt_args[0].u.ui4_arg);

    return RPCR_OK;
}


static INT32 _hndlr_x_sb_start_scan(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T *      pt_nfy_tag                                  = NULL;
    x_sb_scan_nfy_fct       pf_nfy;
    
    RPC_CB_NFY_TAG_T *      pt_nfy_tag_new_svc                          = NULL;
    RPC_CB_NFY_TAG_T *      pt_nfy_tag_svc_update                       = NULL;
    RPC_CB_NFY_TAG_T *      pt_nfy_tag_sb_pal_secam_freq_progress_nfy   = NULL;
    RPC_CB_NFY_TAG_T *      pt_nfy_tag_sb_pal_secam_anas_nfy            = NULL;
    RPC_CB_NFY_TAG_T *      pt_nfy_tag_sb_pal_secam_aci_nfy             = NULL;
    
    HANDLE_T                h_builder                                   = NULL_HANDLE;
    INT32                   i4_ret                                      = 0;
    SB_ENG_CTRL_T*          pt_eng                                      = NULL;
    
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    } 

    h_builder = (HANDLE_T)(pt_args[0].u.pui4_arg);
    if ( h_builder == NULL_HANDLE )
    {
        return RPCR_INV_ARGS;
    }else
    {
        /*printf("_hndlr_x_sb_start_scan h_builder=%d\r\n",(INT32)h_builder);*/
    }

    i4_ret = sb_find_eng_by_handle(h_builder,&pt_eng);
    if ( i4_ret == SBR_INV_HANDLE || pt_eng == NULL)
    {
        printf("_hndlr_x_sb_start_scan sb_find_eng_by_handle fail h_builder=%ld\r\n",(INT32)h_builder);
        return RPCR_INV_ARGS;
    }
    

    if(pt_args[3].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy = _x_sb_scan_nfy_fct_wrapper;
        RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_args[3].u.pv_func, pt_args[2].u.pv_arg);
    }    

    pt_return->e_type = ARG_TYPE_INT32;

    
    if ( strncmp(pt_eng->s_name,DVB_ENG_NAME,strlen(DVB_ENG_NAME)) == 0)
    {
        SB_DVB_SCAN_DATA_T*  pt_scan_data = (SB_DVB_SCAN_DATA_T*)(pt_args[1].u.pv_desc);
        if (pt_scan_data->pf_new_svc_nfy != NULL)
        {
            RPC_CB_TAG(t_rpc_id, pt_nfy_tag_new_svc,
                pt_scan_data->pf_new_svc_nfy, 
                pt_scan_data->pv_new_svc_nfy_tag);
            pt_scan_data->pf_new_svc_nfy = _x_sb_new_svc_nfy_fct_wrapper;
            pt_scan_data->pv_new_svc_nfy_tag = pt_nfy_tag_new_svc;
        }
        else
        {
            pt_nfy_tag_new_svc = NULL;
        }
        if (pt_scan_data->pf_svc_update_nfy != NULL)
        {
            RPC_CB_TAG(t_rpc_id, pt_nfy_tag_svc_update,
                pt_scan_data->pf_svc_update_nfy, 
                pt_scan_data->pv_svc_update_nfy_tag);
            pt_scan_data->pf_svc_update_nfy = _x_sb_dvb_svc_update_nfy_fct_wrapper;
            pt_scan_data->pv_svc_update_nfy_tag = pt_nfy_tag_svc_update;
        }
        else
        {
            pt_nfy_tag_svc_update = NULL;
        }
    }
    else if ( strncmp(pt_eng->s_name,DVBC_ENG_NAME,strlen(DVBC_ENG_NAME)) == 0)
    {
        SB_DVBC_SCAN_DATA_T*  pt_scan_data = (SB_DVBC_SCAN_DATA_T*)(pt_args[1].u.pv_desc);
        if (pt_scan_data->pf_svc_update_nfy != NULL)
        {
            RPC_CB_TAG(t_rpc_id, pt_nfy_tag_svc_update,
                pt_scan_data->pf_svc_update_nfy, 
                pt_scan_data->pv_svc_update_nfy_tag);
            pt_scan_data->pf_svc_update_nfy = _x_sb_dvbc_svc_update_nfy_fct_wrapper;
            pt_scan_data->pv_svc_update_nfy_tag = pt_nfy_tag_svc_update;
        }
        else
        {
            pt_nfy_tag_svc_update = NULL;
        }
    }
    else if( strncmp(pt_eng->s_name,PAL_SECAM_ENG_NAME,strlen(PAL_SECAM_ENG_NAME)) == 0)
    {
        SB_PAL_SECAM_SCAN_DATA_T*  pt_scan_data = (SB_PAL_SECAM_SCAN_DATA_T*)(pt_args[1].u.pv_desc);
        
        printf("_hndlr_x_sb_start_scan Enter PAL Scan=%s\r\n",pt_eng->s_name);
        if (pt_scan_data->pf_freq_nfy != NULL)
        {
            RPC_CB_TAG(t_rpc_id, pt_nfy_tag_sb_pal_secam_freq_progress_nfy, 
                pt_scan_data->pf_freq_nfy, 
                pt_scan_data->pv_freq_nfy_tag);
            
            pt_scan_data->pf_freq_nfy = _x_sb_pal_secam_freq_progress_nfy_fct_wrapper;
            pt_scan_data->pv_freq_nfy_tag = pt_nfy_tag_sb_pal_secam_freq_progress_nfy;
        }
        else
        {
            pt_nfy_tag_sb_pal_secam_freq_progress_nfy = NULL;

        }
        
        if (pt_scan_data->pf_anas_nfy != NULL)
        {
            RPC_CB_TAG(t_rpc_id, pt_nfy_tag_sb_pal_secam_anas_nfy,          
                pt_scan_data->pf_anas_nfy, 
                pt_scan_data->pv_anas_nfy_tag);
            
            pt_scan_data->pf_anas_nfy = _x_sb_pal_secam_anas_nfy_fct_wrapper;
            pt_scan_data->pv_anas_nfy_tag = pt_nfy_tag_sb_pal_secam_anas_nfy;
        }
        else
        {
            pt_nfy_tag_sb_pal_secam_anas_nfy = NULL;

        }
        
        if (pt_scan_data->pf_aci_nfy != NULL)
        {
            RPC_CB_TAG(t_rpc_id, pt_nfy_tag_sb_pal_secam_aci_nfy,
                pt_scan_data->pf_aci_nfy, 
                pt_scan_data->pv_aci_nfy_tag);
            
            pt_scan_data->pf_aci_nfy = _x_sb_pal_secam_aci_nfy_fct_wrapper;
            pt_scan_data->pv_aci_nfy_tag = pt_nfy_tag_sb_pal_secam_aci_nfy;
        }
        else
        {
            pt_nfy_tag_sb_pal_secam_aci_nfy = NULL;
        }
    }

    
    pt_return->u.i4_arg = 
        x_sb_start_scan(
            (HANDLE_T)pt_args[0].u.ui4_arg,
            pt_args[1].u.pv_desc,
            pt_nfy_tag,
            pf_nfy,
            (SB_COND_T*)pt_args[4].u.pi4_arg);
    
    if(pt_return->u.i4_arg == SBR_OK)
    {
        ri_bind_cb_tag((HANDLE_T)pt_args[0].u.ui4_arg, pt_nfy_tag); 
        if ( strncmp(pt_eng->s_name,DVB_ENG_NAME,strlen(DVB_ENG_NAME)) == 0)
        {
            if (pt_nfy_tag_new_svc != NULL)
            {
                ri_bind_cb_tag((HANDLE_T)pt_args[0].u.ui4_arg, pt_nfy_tag_new_svc);
            }
            if (pt_nfy_tag_svc_update != NULL)
            {
                ri_bind_cb_tag((HANDLE_T)pt_args[0].u.ui4_arg, pt_nfy_tag_svc_update);               
            }
        }
        else if( strncmp(pt_eng->s_name,DVBC_ENG_NAME,strlen(DVBC_ENG_NAME)) == 0)
        {
            if (pt_nfy_tag_svc_update != NULL)
            {
                ri_bind_cb_tag((HANDLE_T)pt_args[0].u.ui4_arg, pt_nfy_tag_svc_update);               
            }
        }
        else if( strncmp(pt_eng->s_name,PAL_SECAM_ENG_NAME,strlen(PAL_SECAM_ENG_NAME)) == 0)
        {
            #if 1
            if (pt_nfy_tag_sb_pal_secam_freq_progress_nfy != NULL)
            {
                ri_bind_cb_tag((HANDLE_T)pt_args[0].u.ui4_arg, pt_nfy_tag_sb_pal_secam_freq_progress_nfy);
            }

            if (pt_nfy_tag_sb_pal_secam_anas_nfy != NULL)
            {
                ri_bind_cb_tag((HANDLE_T)pt_args[0].u.ui4_arg, pt_nfy_tag_sb_pal_secam_anas_nfy);
            }

            if (pt_nfy_tag_sb_pal_secam_aci_nfy != NULL)
            {
                ri_bind_cb_tag((HANDLE_T)pt_args[0].u.ui4_arg, pt_nfy_tag_sb_pal_secam_aci_nfy);
            }
            #endif
        }
    }         
    return RPCR_OK;
}

static INT32 _hndlr_x_sb_cancel_scan(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{       

    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }     
    
    pt_return->e_type = ARG_TYPE_INT32;
    pt_return->u.i4_arg = 
        x_sb_cancel_scan(
            pt_args[0].u.ui4_arg);
            
    return RPCR_OK;
}




static INT32 _hndlr_x_sb_get(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
   
	if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }     
    
    pt_return->e_type = ARG_TYPE_INT32;
    pt_return->u.i4_arg = 
        x_sb_get(
                           pt_args[0].u.ui4_arg,
            (SB_KEY_TYPE_T)pt_args[1].u.i4_arg,
                           pt_args[2].u.pv_arg,
                           pt_args[3].u.pz_arg,
                           pt_args[4].u.pv_desc);
    return RPCR_OK;
}




static INT32 _hndlr_x_sb_set(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{   
    
    HANDLE_T                h_builder                                   = NULL_HANDLE;
    INT32                   i4_ret                                      = 0;
    SB_ENG_CTRL_T*          pt_eng                                      = NULL;
    
    RPC_CB_NFY_TAG_T *                  pt_nfy_tag = NULL;
    SB_DVB_NW_CHANGE_NFY_T*             pf_dvb_nfy = NULL;
    SB_DVBC_NW_CHANGE_NFY_T*            pf_dvbc_nfy = NULL;
    
	if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }  
    
    h_builder = (HANDLE_T)(pt_args[0].u.pui4_arg);
    if ( h_builder == NULL_HANDLE )
    {
        return RPCR_INV_ARGS;
    }else
    {
        /*printf("_hndlr_x_sb_start_scan h_builder=%d\r\n",(INT32)h_builder);*/
    }

    i4_ret = sb_find_eng_by_handle(h_builder,&pt_eng);
    if ( i4_ret == SBR_INV_HANDLE || pt_eng == NULL)
    {
        printf("_hndlr_x_sb_set sb_find_eng_by_handle fail h_builder=%ld\r\n",(INT32)h_builder);
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;
    if ( strncmp(pt_eng->s_name,DVB_ENG_NAME,strlen(DVB_ENG_NAME)) == 0)
    {
        if ((SB_KEY_TYPE_T)pt_args[1].u.i4_arg == SB_KEY_TYPE_NW_CHANGE_NFY)
        {
            pf_dvb_nfy = (SB_DVB_NW_CHANGE_NFY_T *)pt_args[4].u.pv_desc;
            if (pf_dvb_nfy->pf_nw_change_nfy == NULL)
            {
                pt_nfy_tag = NULL;
            }
            else
            {
                RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pf_dvb_nfy->pf_nw_change_nfy, pf_dvb_nfy->pv_nw_change_nfy_tag);
                pf_dvb_nfy->pf_nw_change_nfy = _x_sb_dvb_nw_change_nfy_fct_wrapper;
                pf_dvb_nfy->pv_nw_change_nfy_tag = pt_nfy_tag;
            }
        }
    }
    else if ( strncmp(pt_eng->s_name,DVBC_ENG_NAME,strlen(DVBC_ENG_NAME)) == 0)
    {
        if ((SB_KEY_TYPE_T)pt_args[1].u.i4_arg == SB_KEY_TYPE_NW_CHANGE_NFY)
        {
            pf_dvbc_nfy = (SB_DVBC_NW_CHANGE_NFY_T *)pt_args[4].u.pv_desc;
            if (pf_dvbc_nfy->pf_nw_change_nfy == NULL)
            {
                pt_nfy_tag = NULL;
            }
            else
            {
                RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pf_dvbc_nfy->pf_nw_change_nfy, pf_dvbc_nfy->pv_nw_change_nfy_tag);
                pf_dvbc_nfy->pf_nw_change_nfy = _x_sb_dvbc_nw_change_nfy_fct_wrapper;
                pf_dvbc_nfy->pv_nw_change_nfy_tag = pt_nfy_tag;
            }
        }
    }
    else
    {
    }
    
    pt_return->u.i4_arg = x_sb_set(
                                   pt_args[0].u.ui4_arg,
                    (SB_KEY_TYPE_T)pt_args[1].u.i4_arg,
                                   pt_args[2].u.pv_arg,
                                   pt_args[3].u.z_arg,
                                   pt_args[4].u.pv_desc);
    
    if (((SB_KEY_TYPE_T)pt_args[1].u.i4_arg == SB_KEY_TYPE_NW_CHANGE_NFY)&&
        (pt_return->u.i4_arg == SBR_OK))    
    {
        if ((SB_KEY_TYPE_T)pt_args[1].u.i4_arg == SB_KEY_TYPE_NW_CHANGE_NFY)
        {
            /*Todo: check if it can be binded to builder handle.*/
            ri_bind_cb_tag((HANDLE_T)pt_args[0].u.ui4_arg, pt_nfy_tag); 
        }
    }
    return RPCR_OK;
}


INT32 c_rpc_reg_sb_op_hndlrs()
{
    RPC_REG_OP_HNDLR(x_sb_open_builder);    
    RPC_REG_OP_HNDLR(x_sb_close_builder);  
    RPC_REG_OP_HNDLR(x_sb_start_scan);
    RPC_REG_OP_HNDLR(x_sb_cancel_scan);
    RPC_REG_OP_HNDLR(x_sb_get);
    RPC_REG_OP_HNDLR(x_sb_set);
    RPC_REG_OP_HNDLR(x_sb_reg_scan_map);

    return RPCR_OK;
}
