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
#include "evctx/u_evctx.h"
#include "rrctx/x_rrctx.h"
#include "rrctx/u_rrctx.h"
#include "rrctx/rrctx_rrt_eng/u_rrctx_rrt_eng.h"
#include "evctx/evctx_atsc_eng/u_evctx_atsc_eng.h"


#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"



static INT32 _x_rrctx_open_rating(
    RRCTX_RATING_TYPE_T e_rating_type,
    VOID*               pv_param,
    x_rrctx_nfy_fct     pf_nfy,
    VOID*               pv_nfy_tag,
    HANDLE_T*           ph_rating)
{
    RPC_DECL(5, INT32);    
        
    RPC_ARG_INP(ARG_TYPE_INT32,     e_rating_type);
    RPC_ARG_INP(ARG_TYPE_REF_STR,   pv_param);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,  pf_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,  pv_nfy_tag);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,ph_rating);

    RPC_DO_OP("x_rrctx_open_rating");

    RPC_RETURN(ARG_TYPE_INT32, RRCTXR_INV_ARG);
}
INT32 _hndlr_x_rrctx_nfy_fct(
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
    
    /*Maybe not needed*/
    pt_return->e_type = ARG_TYPE_VOID;
    
    ((x_rrctx_nfy_fct)pv_cb_addr)(
            (HANDLE_T)pt_args[0].u.ui4_arg,
                      pt_args[1].u.pv_arg,
                      pt_args[2].u.ui4_arg);

    return RPCR_OK;
}

INT32 c_rrctx_close_rating (
                    HANDLE_T            h_rating)
{
    RPC_DECL(1, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_rating); 
    
    RPC_DO_OP("x_rrctx_close_rating");
    
    RPC_RETURN(ARG_TYPE_INT32, RRCTXR_INV_HANDLE);
}

INT32 c_rrctx_get_rating_info (
                    HANDLE_T            h_rating,
                    RRCTX_KEY_TYPE_T    e_key_type,
                    VOID*               pv_key_param,
                    SIZE_T*             pz_rating_info_len,
                    VOID*               pv_rating_info)
{
    RRCTX_RRT_KEY_PARAM_T * pt_rrt_key_param;
    RPC_DECL(5, INT32);    

    pt_rrt_key_param = (RRCTX_RRT_KEY_PARAM_T *)pv_key_param;

    switch(e_key_type)
    {
        /* Just found RRCTX_KEY_TYPE_ENG_SPECIFIC */
        case RRCTX_KEY_TYPE_ENG_SPECIFIC:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 2, ARG_TYPE_REF_DESC));
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_key_param, RPC_DESC_RRCTX_RRT_KEY_PARAM_T, NULL));
            
            switch(pt_rrt_key_param->e_rrt_key_type)
            {
                case RRCTX_RRT_KEY_TYPE_CRNT_TUNER:
                    RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_rrt_key_param->pv_rrt_key_param));
                    break;				
				case RRCTX_RRT_KEY_TYPE_LVL_ABBR:
                    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_rrt_key_param->pv_rrt_key_param,3));
                    break;
				case RRCTX_RRT_KEY_TYPE_DIM_TEXT:
				case RRCTX_RRT_KEY_TYPE_DIM_GRAD:
				case RRCTX_RRT_KEY_TYPE_LVL_NUM:
				case RRCTX_RRT_KEY_TYPE_LVL_TEXT:
				case RRCTX_RRT_KEY_TYPE_DIM_NUM:
				case RRCTX_RRT_KEY_TYPE_RGN_ID:
                    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_rrt_key_param->pv_rrt_key_param,2));
                    break;
				case RRCTX_RRT_KEY_TYPE_RGN_TEXT:
					RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_rrt_key_param->pv_rrt_key_param,1));
					break;
				default:
                    RPC_CHECK(RPC_ADD_REF_VAL(RPC_DEFAULT_ID, &pt_rrt_key_param->pv_rrt_key_param));
                    break;
            }
            break;
        default:
            RPC_FAIL;
    }

    if((e_key_type == RRCTX_KEY_TYPE_ENG_SPECIFIC) &&
		 ((pt_rrt_key_param->e_rrt_key_type == RRCTX_RRT_KEY_TYPE_DIM_TEXT) ||
		  (pt_rrt_key_param->e_rrt_key_type == RRCTX_RRT_KEY_TYPE_RGN_TEXT) ||
		  (pt_rrt_key_param->e_rrt_key_type == RRCTX_RRT_KEY_TYPE_LVL_TEXT)))
	{
		RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_rating_info, *pz_rating_info_len * 2));
	}
	else
	{
		RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_rating_info, *pz_rating_info_len));
	}
        
    RPC_ARG_INP(ARG_TYPE_UINT32,      h_rating);
    RPC_ARG_INP(ARG_TYPE_INT32  ,     e_key_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE,    pv_key_param);
    RPC_ARG_IO (ARG_TYPE_REF_SIZE_T,  pz_rating_info_len);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,    pv_rating_info);

    RPC_DO_OP("x_rrctx_get_rating_info");

    RPC_RETURN(ARG_TYPE_INT32, RRCTXR_INV_ARG);
}

INT32 c_rrctx_get_rating_info_len (
                    HANDLE_T            h_rating,
                    RRCTX_KEY_TYPE_T    e_key_type,
                    VOID*               pv_key_param,
                    SIZE_T*             pz_rating_info_len)
{
    RRCTX_RRT_KEY_PARAM_T * pt_rrt_key_param;
    RPC_DECL(4, INT32);    

    switch(e_key_type)
    {
        /* Just found RRCTX_KEY_TYPE_ENG_SPECIFIC */
        case RRCTX_KEY_TYPE_ENG_SPECIFIC:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 2, ARG_TYPE_REF_DESC));
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_key_param, RPC_DESC_RRCTX_RRT_KEY_PARAM_T, NULL));

            pt_rrt_key_param = (RRCTX_RRT_KEY_PARAM_T *)pv_key_param;
            switch(pt_rrt_key_param->e_rrt_key_type)
            {
                case RRCTX_RRT_KEY_TYPE_CRNT_TUNER:
                    RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_rrt_key_param->pv_rrt_key_param));
                    break;
                default:
                    RPC_CHECK(RPC_ADD_REF_VAL(RPC_DEFAULT_ID, &pt_rrt_key_param->pv_rrt_key_param));
                    break;
            }
            break;
        default:
            RPC_FAIL;
    }
    
    RPC_ARG_INP(ARG_TYPE_UINT32,      h_rating);
    RPC_ARG_INP(ARG_TYPE_INT32  ,     e_key_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE,    pv_key_param);
    RPC_ARG_IO (ARG_TYPE_REF_SIZE_T,  pz_rating_info_len);

    RPC_DO_OP("x_rrctx_get_rating_info_len");

    RPC_RETURN(ARG_TYPE_INT32, RRCTXR_INV_ARG);
}
INT32 c_rrctx_open_rating (
                    RRCTX_RATING_TYPE_T e_rating_type,
                    VOID*               pv_param,
                    x_rrctx_nfy_fct     pf_nfy,
                    VOID*               pv_nfy_tag,
                    HANDLE_T*           ph_rating)
{
    INT32     i4_res;
    i4_res = _x_rrctx_open_rating ( e_rating_type,
                                   pv_param,
                                   pf_nfy,
                                   pv_nfy_tag,
                                   ph_rating);
    return i4_res;
}

INT32 c_rrctx_rating_enable(
                    HANDLE_T            h_rating,
                    BOOL                b_enable)
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,h_rating); 
    RPC_ARG_INP(ARG_TYPE_BOOL,  b_enable); 
    
    RPC_DO_OP("x_rrctx_rating_enable");
    
    RPC_RETURN(ARG_TYPE_INT32, RRCTXR_INV_ARG);
}


INT32 c_rrctx_rating_exceeds(
                    HANDLE_T            h_rating,
                    VOID*               pt_rating_lst,
                    BOOL*               pb_exceed)
{

    RPC_DECL(3, INT32);

    RPC_CHECK(ri_rrctx_rating_list_add_desc(pt_rating_lst));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_rating); 
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_rating_lst); 
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,  pb_exceed); 

    
    RPC_DO_OP("x_rrctx_rating_exceeds");

    RPC_RETURN(ARG_TYPE_INT32, RRCTXR_INV_ARG);
}


INT32 c_rrctx_rating_get_text(
                    HANDLE_T            h_rating,
                    VOID*               pt_rating_lst,
                    UTF16_T*            w2s_str,
                    SIZE_T              z_len)
{
    RPC_DECL(4, INT32);
    
    /* EVCTX_ATSC_RATING_LIST_T * pt_list; */

    RPC_CHECK(ri_rrctx_rating_list_add_desc(pt_rating_lst));
    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, w2s_str, z_len*2));    
        
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_rating); 
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_rating_lst); 
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  w2s_str); 
    RPC_ARG_INP(ARG_TYPE_SIZE_T,    z_len); 
    
    RPC_DO_OP("x_rrctx_rating_get_text");

    RPC_RETURN(ARG_TYPE_INT32, RRCTXR_INV_ARG);
}

INT32 c_rrctx_rating_get_type(
                    HANDLE_T            h_rating,
                    VOID*               pt_rating_lst, 
                    RRCTX_VCHIP_TYPE_T* pt_rating_type)
{

    RPC_DECL(3, INT32);

    RPC_CHECK(ri_rrctx_rating_list_add_desc(pt_rating_lst));
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_rating_type, RPC_DESC_RRCTX_VCHIP_TYPE_T, NULL));    
        
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_rating); 
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_rating_lst); 
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_rating_type); 
    
    RPC_DO_OP("x_rrctx_rating_get_type");

    RPC_RETURN(ARG_TYPE_INT32, RRCTXR_INV_ARG);
}

INT32 c_rrctx_rating_settings(
                    HANDLE_T            h_rating,
                    VOID*               pv_setting)
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_rating); 
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pv_setting); 
   
    RPC_DO_OP("x_rrctx_rating_settings");

    RPC_RETURN(ARG_TYPE_INT32, RRCTXR_INV_ARG);

}
/* Store Rating Data */
INT32 c_rrctx_rating_storage(
                    HANDLE_T            h_rating,
                    BOOL                b_store)
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,h_rating); 
    RPC_ARG_INP(ARG_TYPE_BOOL,  b_store); 
    
    RPC_DO_OP("x_rrctx_rating_storage");
    
    RPC_RETURN(ARG_TYPE_INT32, RRCTXR_INV_ARG);
}


INT32 c_rpc_reg_rrctx_cb_hndlrs()
{
    RPC_REG_CB_HNDLR(x_rrctx_nfy_fct);    
    return RPCR_OK;
}

INT32 c_rrctx_rating_available(
                    HANDLE_T            h_rating,
                    BOOL*               pb_available)
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_rating); 
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,  pb_available); 
   
    RPC_DO_OP("x_rrctx_rating_available");

    RPC_RETURN(ARG_TYPE_INT32, RRCTXR_INV_ARG);
}

