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
 *    This  file contains implementation of the TSL's application API's. 
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/


#include "res_mngr/u_rm.h"
#include "res_mngr/x_rm.h"
#include "res_mngr/drv/u_drv_cust.h"
#include "res_mngr/u_rm_dev_types.h"

#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"

#define RM_RPC_DBG 0

INT32 c_rm_set_comp (DRV_TYPE_T     e_type,
                     UINT16             ui2_id,
                     BOOL               b_sel_out_port,
                     UINT8              ui1_port,
                     DRV_SET_TYPE_T     e_set_type,
                     const VOID*        pv_set_info,
                     SIZE_T             z_set_info_len)
{
#if RM_RPC_DBG    
    printf("[MD:c_rm_set_comp]");
#endif    
    if (e_type == DRVT_CUST_DRV)
    {    
        RPC_DECL(7, INT32);    
        DRV_CUST_OPERATION_INFO_T * pt_op_info = (DRV_CUST_OPERATION_INFO_T *)pv_set_info;
        if(pv_set_info)
        {    
            /*RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_get_info, *pz_get_info_len));*/
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_set_info, RPC_DESC_DRV_CUST_OPERATION_INFO_T, NULL));
            
            if (pt_op_info->e_op_type == DRV_CUSTOM_OPERATION_TYPE_MISC_SET)
            {
                  if ( pt_op_info->u.t_misc_set_info.e_misc_type == DRV_CUSTOM_MISC_TYPE_CEC_ENABLE_SET)
                  {
                      RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, &(pt_op_info->u), RPC_DESC_DRV_CUST_OPERATION_INFO_T_u, "u.t_misc_set_info"));
                         
                      RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_op_info->u.t_misc_set_info.pv_set_info, sizeof(UINT8*)));
                  }
                  else if ( pt_op_info->u.t_misc_set_info.e_misc_type == DRV_CUSTOM_MISC_TYPE_CEC_ONE_TOUCH_PLAY_EN_SET)
                  {
                      RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, &(pt_op_info->u), RPC_DESC_DRV_CUST_OPERATION_INFO_T_u, "u.t_misc_set_info"));
                         
                      RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_op_info->u.t_misc_set_info.pv_set_info, sizeof(UINT8*)));
                  }
            }
        }
        else
        {
            RPC_FAIL;
        }    
                    
        RPC_ARG_INP(ARG_TYPE_UINT16,        e_type);    
        RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_id);
        RPC_ARG_INP(ARG_TYPE_BOOL,          b_sel_out_port);
        RPC_ARG_INP(ARG_TYPE_UINT8,         ui1_port);    
        RPC_ARG_INP(ARG_TYPE_UINT32,        e_set_type);
        RPC_ARG_INP(ARG_TYPE_REF_DESC,      pv_set_info);
        RPC_ARG_INP(ARG_TYPE_SIZE_T,        z_set_info_len);
#if RM_RPC_DBG
     printf("[MD:ResMgr] c_rm_set_comp:drv_type=%d e_set_type=%d, size:%d\n\n",
           (DRV_TYPE_T)e_type,  (DRV_SET_TYPE_T)e_set_type,
            z_set_info_len);    
#endif    
        RPC_DO_OP("x_rm_set_comp");
    
        RPC_RETURN(ARG_TYPE_INT32, RMR_INV_ARG);  
    }
    return 0;
}

INT32 c_rm_get_comp (DRV_TYPE_T      e_type,
                     UINT16          ui2_id,
                     BOOL            b_sel_out_port,
                     UINT8           ui1_port,
                     DRV_GET_TYPE_T  e_get_type,
                     VOID*           pv_get_info,
                     SIZE_T*         pz_get_info_len)
{

#if RM_RPC_DBG    
    printf("[MD:c_rm_get_comp]");
#endif    
    if (e_type == DRVT_CUST_DRV)
    {    
        RPC_DECL(7, INT32);    
        DRV_CUST_OPERATION_INFO_T * pt_op_info = (DRV_CUST_OPERATION_INFO_T *)pv_get_info;
        if(pz_get_info_len)
        {    
            /*RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_get_info, *pz_get_info_len));*/
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_get_info, RPC_DESC_DRV_CUST_OPERATION_INFO_T, NULL));
            
            if (pt_op_info->e_op_type == DRV_CUSTOM_OPERATION_TYPE_AUD_GET)
            {
                  if ( pt_op_info->u.t_aud_get_info.e_aud_type == DRV_CUSTOM_AUD_TYPE_SPECTRUM_INFO)
                  {
                      RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, &(pt_op_info->u), RPC_DESC_DRV_CUST_OPERATION_INFO_T_u, "u.t_aud_get_info"));
                         
                      RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_op_info->u.t_aud_get_info.pv_get_info, RPC_DESC_AUD_TYPE_SPECTRUM_INFO_T, NULL));
                      RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_op_info->u.t_aud_get_info.pz_size, sizeof(SIZE_T*)));
                      /*RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_op_info->u.t_aud_get_info.pv_get_info, *(pt_op_info->u.t_aud_get_info.pz_size)));*/
                  }
            }
            else if (pt_op_info->e_op_type == DRV_CUSTOM_OPERATION_TYPE_MISC_GET)
            {
                  if ( pt_op_info->u.t_misc_get_info.e_misc_type == DRV_CUSTOM_MISC_TYPE_CEC_ENABLE_GET)
                  {
                      RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, &(pt_op_info->u), RPC_DESC_DRV_CUST_OPERATION_INFO_T_u, "u.t_misc_get_info"));
                         
                      RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_op_info->u.t_misc_get_info.pv_get_info, sizeof(UINT8*)));
                      RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_op_info->u.t_misc_get_info.pz_size, sizeof(SIZE_T*)));
                      /*RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_op_info->u.t_aud_get_info.pv_get_info, *(pt_op_info->u.t_aud_get_info.pz_size)));*/
                  }
                  else if ( pt_op_info->u.t_misc_get_info.e_misc_type == DRV_CUSTOM_MISC_TYPE_CEC_ONE_TOUCH_PLAY_EN_GET)
                  {
                      RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, &(pt_op_info->u), RPC_DESC_DRV_CUST_OPERATION_INFO_T_u, "u.t_misc_get_info"));
                         
                      RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_op_info->u.t_misc_get_info.pv_get_info, sizeof(UINT8*)));
                      RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_op_info->u.t_misc_get_info.pz_size, sizeof(SIZE_T*)));
                      /*RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_op_info->u.t_aud_get_info.pv_get_info, *(pt_op_info->u.t_aud_get_info.pz_size)));*/
                  }
            }
        }
        else
        {
            RPC_FAIL;
        }    
                    
        RPC_ARG_INP(ARG_TYPE_UINT16,        e_type);    
        RPC_ARG_INP(ARG_TYPE_UINT16,        ui2_id);
        RPC_ARG_INP(ARG_TYPE_BOOL,          b_sel_out_port);
        RPC_ARG_INP(ARG_TYPE_UINT8,         ui1_port);    
        RPC_ARG_INP(ARG_TYPE_UINT32,        e_get_type);
        RPC_ARG_OUT(ARG_TYPE_REF_DESC,      pv_get_info);
        RPC_ARG_IO (ARG_TYPE_REF_SIZE_T,    pz_get_info_len);
#if RM_RPC_DBG
     printf("[MD:ResMgr] c_rm_get_comp:drv_type=%d e_get_type=%d, size:%d\n\n",
           (DRV_TYPE_T)e_type,  (DRV_GET_TYPE_T)e_get_type,
            pz_get_info_len);    
#endif    
        RPC_DO_OP("x_rm_get_comp");
    
        RPC_RETURN(ARG_TYPE_INT32, RMR_INV_ARG);  
    }
    return 0;
}

