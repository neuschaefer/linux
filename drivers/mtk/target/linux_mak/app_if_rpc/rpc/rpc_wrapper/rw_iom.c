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
#include "io_mngr/x_iom.h"

#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"


static INT32 _hndlr_x_iom_nfy_fct(
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
    
    ((x_iom_nfy_fct)pv_cb_addr)(
                      pt_args[0].u.pv_arg,
                      pt_args[1].u.ui4_arg,
                      pt_args[2].u.ui4_arg,
                      pt_args[3].u.ui4_arg);
                      
    return RPCR_OK;
}

INT32 c_iom_open(IOM_DEV_TYPE_T e_dev_type,
                    UINT16         ui2_id,
                    const CHAR *   ps_name,
                    VOID*          pv_setting,
                    VOID*          pv_tag,
                    x_iom_nfy_fct  pf_nfy,
                    HANDLE_T*       ph_dev)
{
    if (IOM_DEV_TYPE_IO_UART != e_dev_type)
    {
        return IOMR_INV_ARG;
    }
    
    RPC_DECL(7, INT32); 
      
    RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, ps_name));
    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_setting, sizeof(UART_SETTING_T)));

    RPC_ARG_INP(ARG_TYPE_UINT32,    e_dev_type);
    RPC_ARG_INP(ARG_TYPE_UINT16,    ui2_id);
    RPC_ARG_INP(ARG_TYPE_REF_STR,    ps_name);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,   pv_setting);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_dev);  
    
    RPC_DO_OP("x_iom_open");
    
    RPC_RETURN(ARG_TYPE_INT32, IOMR_ERROR);
}

INT32 c_iom_close(HANDLE_T h_dev)
{
    RPC_DECL(1, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_dev);
      
    RPC_DO_OP("x_iom_close");
    
    RPC_RETURN(ARG_TYPE_INT32, IOMR_ERROR);
}

INT32 c_iom_get(HANDLE_T h_dev,
                  IOM_GET_T e_get_type,
                  VOID*     pv_get_info,
                  SIZE_T*   pv_get_info_len)
{
    if ((IOM_GET_UART_SETTING != e_get_type) &&
        (IOM_GET_UART_OPERATION_MODE != e_get_type))
    {
        return IOMR_INV_ARG;
    }

    RPC_DECL(4, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_dev);
    RPC_ARG_INP(ARG_TYPE_UINT32, e_get_type);

    if (IOM_GET_UART_SETTING == e_get_type)
    {
        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_get_info, *pv_get_info_len));
        RPC_ARG_OUT(ARG_TYPE_REF_DESC, pv_get_info);
    }
    else
    {
        RPC_ARG_OUT (ARG_TYPE_REF_UINT32, pv_get_info);
    }

    RPC_ARG_IO (ARG_TYPE_REF_SIZE_T, pv_get_info_len);

    
    RPC_DO_OP("x_iom_get");
    
    RPC_RETURN(ARG_TYPE_INT32, IOMR_ERROR);
}

INT32 c_iom_set(HANDLE_T      h_dev,
                  IOM_SET_T     e_set_type,
                  const VOID*   pv_set_info,
                  SIZE_T        z_set_info_len)
{
    if ((IOM_SET_UART_SETTING != e_set_type) &&
        (IOM_SET_UART_OPERATION_MODE != e_set_type) &&
        (IOM_SET_UART_MAGIC_CHAR != e_set_type))
    {
        return IOMR_INV_ARG;
    }

    RPC_DECL(4, INT32);

    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_dev);
    RPC_ARG_INP(ARG_TYPE_UINT32, e_set_type);
      
    if (IOM_SET_UART_SETTING == e_set_type)
    {
        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_set_info, z_set_info_len));
        RPC_ARG_INP(ARG_TYPE_REF_DESC,   pv_set_info);
    }
    else if  (IOM_SET_UART_MAGIC_CHAR == e_set_type)
    {
        INT32 i;
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_set_info, RPC_DESC_MAGIC_T, NULL));
        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, ((MAGIC_T *)pv_set_info)->pt_magic_unit, sizeof(MAGIC_UNIT_T) * ((MAGIC_T *)pv_set_info)->ui1_magic_units_count));
        for (i = 0; i < ((MAGIC_T *)pv_set_info)->ui1_magic_units_count; ++i)
        {
            MAGIC_UNIT_T *pt_magic_unit = (((MAGIC_T *)pv_set_info)->pt_magic_unit) + i;
            RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_magic_unit->pui1_magic, pt_magic_unit->z_magic_len));
        }

        RPC_ARG_INP(ARG_TYPE_REF_DESC,   pv_set_info);
    }
    else
    {
        RPC_ARG_INP (ARG_TYPE_REF_UINT32, pv_set_info);
    }

    
    RPC_ARG_INP(ARG_TYPE_SIZE_T, z_set_info_len);
    
    RPC_DO_OP("x_iom_set");
    
    RPC_RETURN(ARG_TYPE_INT32, IOMR_ERROR);
}

INT32 c_iom_output(HANDLE_T   h_dev,
                      VOID*      pv_otp_info)
{
    if (NULL == pv_otp_info || ((IOM_UART_T *)pv_otp_info)->z_len < 1)
    {
        return IOMR_INV_ARG;
    }

    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_otp_info, RPC_DESC_IOM_UART_T, NULL));
    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, ((IOM_UART_T *)pv_otp_info)->pui1_data, ((IOM_UART_T *)pv_otp_info)->z_len));

    RPC_ARG_INP(ARG_TYPE_UINT32, h_dev);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pv_otp_info);
    
    RPC_DO_OP("x_iom_output");
    
    RPC_RETURN(ARG_TYPE_INT32, IOMR_ERROR);
}

INT32 c_rpc_reg_iom_cb_hndlrs()
{
    RPC_REG_CB_HNDLR(x_iom_nfy_fct);    
    return RPCR_OK;
}
