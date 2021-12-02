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
 * $RCSfile: c_cecm.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/14 $
 * $SWAuthor: Clear Case Administrator $
 * $MD5HEX: a872122d22ab4b982b54b29e95d0ff55 $
 *
 * Description:
 *         This is CEC Manager API source file, which are exported to
 *         applications.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "inc/common.h"
#include "aee/aee.h"
#include "handle/handle.h"
#include "cec_mngr/x_cecm.h"



#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/

static INT32 _hndlr_x_cecm_nfy_fct(
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
    
      ((x_cecm_nfy_fct)pv_cb_addr)(
                      (CECM_NFY_T*)pt_args[0].u.pv_desc,
                      pt_args[1].u.pv_arg,
                      pt_args[2].u.ui4_arg);
                      
    return RPCR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_open_svc
 *
 * Description: This API is used to open a CECM service.
 *
 * Inputs:  pf_nfy          References the caller's notify function.
 *          pv_tag          Private tag value which must be returned in the
 *                          notify function. It can be set to NULL.
 *
 * Outputs: ph_cecm_svc     A handle referencing the opened service.
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_OUT_OF_RESOURCE       Not enough CECM resource.
 *          CECMR_OUT_OF_HANDLE         No handle available.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_open_svc(x_cecm_nfy_fct pf_nfy,
                      VOID*          pv_tag,
                      HANDLE_T*      ph_cecm_svc)
{
    HANDLE_T    h_aux;
    INT32       i4_return;
    
    RPC_DECL(3, INT32);  

    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_VOID, pv_tag);    
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_cecm_svc);

    i4_return = aee_grab_handle_resource(AEE_FLAG,
                                         1,
                                         &h_aux);
    if (i4_return == AEER_OK)
    {
        RPC_DO_OP("x_cecm_open_svc");
      
        if (i4_return == CECMR_OK)
        {
            handle_link_to_aux(h_aux, *ph_cecm_svc);
        }
        else
        {
            aee_release_handle_resource(1);
        }
    }

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR); 
}

/*-----------------------------------------------------------------------------
 * Name: c_cecm_close_svc
 *
 * Description: This API is used to close a CECM service.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_HANDLE_FREE_ERROR     Handle free error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_close_svc(HANDLE_T h_cecm_svc)
{
    RPC_DECL(1, INT32);  

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cecm_svc);  
    RPC_DO_OP("x_cecm_close_svc");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR); 
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_signal_cec_ready
 *
 * Description: The API is used to switch CEC control between CEC driver and 
 *              uP.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          b_ready         Indicates the current control right (TRUE => CEC
 *                          driver, FALSE => uP).
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DRV_COMP_ERROR        CEC driver operation error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_signal_cec_ready(HANDLE_T h_cecm_svc,
                              BOOL     b_ready)
{
    RPC_DECL(2, INT32);  

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cecm_svc);  
    RPC_ARG_INP(ARG_TYPE_BOOL,   b_ready);    
    RPC_DO_OP("x_cecm_signal_cec_ready");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR); 

}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_cec_wakeup_ctrl
 *
 * Description: This API sets uP CEC wakeup control.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          pt_wakeup_ctrl      Wake-up control info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DRV_COMP_ERROR        CEC driver operation error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_cec_wakeup_ctrl(HANDLE_T             h_cecm_svc,
                                 CECM_WAKE_UP_CTRL_T* pt_wakeup_ctrl)
{

    RPC_DECL(2, INT32);  
  
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_wakeup_ctrl, RPC_DESC_CECM_WAKE_UP_CTRL_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cecm_svc);  
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_wakeup_ctrl);    

    RPC_DO_OP("x_cecm_set_cec_wakeup_ctrl");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);     
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_cec_fct
 *
 * Description: The API enables/disables CEC functions.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          t_paramS        Contains cec_set_fct parameters.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DRV_COMP_ERROR        CEC driver operation error. 
 *          CECMR_MSG_SEND_ERROR        Send message failed. 
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_cec_fct(HANDLE_T                  h_cecm_svc,
                         CECM_SET_CEC_FCT_PARAMS_T t_params)
{

    CECM_SET_CEC_FCT_PARAMS_T* pt_cec_fct_para;
    pt_cec_fct_para = &t_params;
    
    RPC_DECL(2, INT32);  

    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_cec_fct_para, RPC_DESC_CECM_SET_CEC_FCT_PARAMS_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cecm_svc);  
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_cec_fct_para);    

    RPC_DO_OP("x_cecm_set_cec_fct");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);     
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_is_act_src_received
 *
 * Description: The API get if received active source.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DRV_COMP_ERROR        CEC driver operation error. 
 *          CECMR_MSG_SEND_ERROR        Send message failed. 
 ----------------------------------------------------------------------------*/

INT32 c_cecm_is_act_src_reved(HANDLE_T  h_cecm_svc , CECM_ACT_SRC_REVED_T* pt_act_src_reved)
{    
    RPC_DECL(2, INT32);  
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_act_src_reved, RPC_DESC_CECM_ACT_SRC_REVED_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cecm_svc);   
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_act_src_reved);    

    RPC_DO_OP("x_cecm_is_act_src_reved");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);      
}
/*-----------------------------------------------------------------------------
 * Name: c_cecm_get_la_by_pa
 *
 * Description: This API gets the first logical address in the list which is  
 *              mapped to a physical address.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          ui2_pa          A physical address.
 *
 * Outputs: -
 *
 * Returns: CECM_LOG_ADDR_MAX   Failed.
 *          Other               It's logical address.
 ----------------------------------------------------------------------------*/
CECM_LOG_ADDR_T c_cecm_get_la_by_pa(HANDLE_T h_cecm_svc,
                                    UINT16   ui2_pa)
{
    RPC_DECL(2, INT32);   

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cecm_svc);  
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_pa);    

    RPC_DO_OP("x_cecm_get_la_by_pa");

    RPC_RETURN(ARG_TYPE_INT32, CECM_LOG_ADDR_MAX);    

}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_get_la_by_pa_ex
 *
 * Description: This API gets the logical address(es) which is(are) mapped to 
 *              a physical address.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          ui2_pa          A physical address.
 *          pt_la_lst       References a logical address list.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK            Routine successful.
 *          CECMR_INV_ARG       One or more of the arguments contain invalid
 *                              data.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_get_la_by_pa_ex(HANDLE_T            h_cecm_svc,
                             UINT16              ui2_pa,
                             CECM_LOG_ADD_LST_T* pt_la_lst)
{                             

    RPC_DECL(3, INT32);  
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_la_lst, RPC_DESC_CECM_LOG_ADD_LST_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cecm_svc);  
    RPC_ARG_INP(ARG_TYPE_UINT16,   ui2_pa);  
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_la_lst);    

    RPC_DO_OP("x_cecm_get_la_by_pa_ex");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);   

}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_get_pa_by_la
 *
 * Description: This API gets the physical address of a logical address.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECM_LOG_ADDR_MAX   Failed.
 *          Other               It's logical address.
 ----------------------------------------------------------------------------*/
UINT16 c_cecm_get_pa_by_la(HANDLE_T        h_cecm_svc,
                           CECM_LOG_ADDR_T e_la)
{
    RPC_DECL(2, INT32);  

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cecm_svc);  
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_la);   

    RPC_DO_OP("x_cecm_get_pa_by_la");

    RPC_RETURN(ARG_TYPE_UINT16, CECM_INV_PA);      
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_get_dev_info_by_la
 *
 * Description: The API gets device info by logical address.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *          pt_ctrl         References the control info.
 *
 * Outputs: pt_dev_info     References device info.
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_get_dev_info_by_la(HANDLE_T        h_cecm_svc,
                                CECM_LOG_ADDR_T e_la,
                                CECM_CTRL_T*    pt_ctrl,
                                CECM_DEV_T*     pt_dev_info)
{
    RPC_DECL(4, INT32);  
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_ctrl, RPC_DESC_CECM_CTRL_T, NULL));
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_dev_info, RPC_DESC_CECM_DEV_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cecm_svc);  
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_la);  
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_ctrl);     
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_dev_info);    

    RPC_DO_OP("x_cecm_get_dev_info_by_la");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);   

}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_get_comp_id_by_la
 *
 * Description: The API gets component ID by logical address.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: pui2_comp_id    HDMI component ID.
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_get_comp_id_by_la(HANDLE_T        h_cecm_svc,
                               CECM_LOG_ADDR_T e_la,
                               UINT16*         pui2_comp_id)
{

    RPC_DECL(3, INT32);  
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cecm_svc);  
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_la);   
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16, pui2_comp_id);    

    RPC_DO_OP("x_cecm_get_comp_id_by_la");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);   

}

/*-----------------------------------------------------------------------------
 * Name: c_cecm_get_comp_id_by_la_ex
 *
 * Description: The API gets component ID by logical address.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: pui2_comp_id    HDMI component ID.
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_get_comp_id_by_la_ex(HANDLE_T        h_cecm_svc,
                                          CECM_LOG_ADDR_T e_la,
                                          UINT16*         pui2_comp_id,
                                          BOOL  b_retry)

{

    RPC_DECL(4, INT32);  
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cecm_svc);  
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_la);   
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16, pui2_comp_id);   
    RPC_ARG_INP(ARG_TYPE_BOOL,   b_retry);  

    RPC_DO_OP("x_cecm_get_comp_id_by_la_ex");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);   

}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_tv_la
 *
 * Description: The API set the return la when get tv la fail.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            The logical address 
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_max_tv_la(HANDLE_T        h_cecm_svc,
                          CECM_LOG_ADDR_T   e_la)
{  
    RPC_DECL(2, INT32);  
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cecm_svc);  
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_la);    

    RPC_DO_OP("x_cecm_set_max_tv_la");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);       
}
/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_tv_la
 *
 * Description: The API set the return la when get tv la fail.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            The logical address 
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_support_cec_ver(HANDLE_T        h_cecm_svc,
                                  CECM_CEC_VER_T   e_cec_ver)

{  
    RPC_DECL(2, INT32);  
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cecm_svc);  
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_cec_ver);    

    RPC_DO_OP("x_cecm_set_support_cec_ver");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);       
}

/*-----------------------------------------------------------------------------
 * Name: c_cecm_find_tv_la_alloc
 *
 * Description: The API finds the TV logical address allocation.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *
 * Outputs: -
 *
 * Returns: CECM_LOG_ADDR_MAX   Failed.
 *          Other               It's logical address.
 ----------------------------------------------------------------------------*/
CECM_LOG_ADDR_T c_cecm_find_tv_la_alloc(HANDLE_T h_cecm_svc)
{  
    RPC_DECL(1, INT32);  
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cecm_svc);   

    RPC_DO_OP("x_cecm_find_tv_la_alloc");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);         
}

/*-----------------------------------------------------------------------------
 * Name: c_cecm_find_tv_la_alloc_ex
 *
 * Description: The API finds the TV logical address allocation.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          ui4_itvl        Ping la 0 interval in second.
 *
 * Outputs: -
 *
 * Returns: CECM_LOG_ADDR_MAX   Failed.
 *          Other               It's logical address.
 ----------------------------------------------------------------------------*/

CECM_LOG_ADDR_T c_cecm_find_tv_la_alloc_ex(HANDLE_T h_cecm_svc,
                                               UINT32   ui4_itvl)
{
    RPC_DECL(2, INT32);  
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cecm_svc);   
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_itvl);   

    RPC_DO_OP("x_cecm_find_tv_la_alloc_ex");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);        
}

/*-----------------------------------------------------------------------------
 * Name: c_cecm_config_dev_discovery
 *
 * Description: The API config CEC device discovery.
 *
 * Inputs:  h_cecm_svc                  A handle referencing the service.
 *          pt_dev_discovery_config     Device discovery config info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 ----------------------------------------------------------------------------*/

INT32 c_cecm_config_dev_discovery(HANDLE_T        h_cecm_svc,
                                         CECM_DEV_DISCOVERY_CONFIG_T*   pt_dev_discovery_config)

{
    RPC_DECL(2, INT32);  
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_dev_discovery_config, RPC_DESC_CECM_DEV_DISCOVERY_CONFIG_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_dev_discovery_config);    

    RPC_DO_OP("x_cecm_config_dev_discovery");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);   
}
/*-----------------------------------------------------------------------------
 * Name: c_cecm_get_dev_discovery_config
 *
 * Description: The API get discovery config
 *
 * Inputs:  h_cecm_svc                  A handle referencing the service.
 *          
 *
 * Outputs: pt_dev_discovery_config     Device discovery config info.
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 ----------------------------------------------------------------------------*/

INT32 c_cecm_get_dev_discovery_config(HANDLE_T        h_cecm_svc,
                                             CECM_DEV_DISCOVERY_CONFIG_T*   pt_dev_discovery_config)

{

    RPC_DECL(2, INT32);  

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_dev_discovery_config, RPC_DESC_CECM_DEV_DISCOVERY_CONFIG_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cecm_svc);     
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_dev_discovery_config);    

    RPC_DO_OP("x_cecm_get_dev_discovery_config");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);  

}

/*-----------------------------------------------------------------------------
 * Name: c_cecm_start_dev_discovery
 *
 * Description: The API starts CEC device discovery.
 *
 * Inputs:  h_cecm_svc                  A handle referencing the service.
 *          ui4_dev_discovery_itvl      Device discovery interval in second.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_start_dev_discovery(HANDLE_T h_cecm_svc,
                                 UINT32   ui4_dev_discovery_itvl)
{
    RPC_DECL(2, INT32);  

    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_dev_discovery_itvl);    

    RPC_DO_OP("x_cecm_start_dev_discovery");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);     
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_start_dev_discovery_ex
 *
 * Description: The API starts CEC device discovery.
 *
 * Inputs:  h_cecm_svc                  A handle referencing the service.
 *          pt_dev_discovery_info       References a device discovery
 *                                      information.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_start_dev_discovery_ex(HANDLE_T              h_cecm_svc,
                                    CECM_DEV_DISCOVERY_T* pt_dev_discovery_info)
{
    RPC_DECL(2, INT32);  

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_dev_discovery_info, RPC_DESC_CECM_DEV_DISCOVERY_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_dev_discovery_info);    

    RPC_DO_OP("x_cecm_start_dev_discovery_ex");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);  

}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_stop_dev_discovery
 *
 * Description: The API stops CEC device discovery.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_MSG_SEND_ERROR        Send message failed. 
 ----------------------------------------------------------------------------*/
INT32 c_cecm_stop_dev_discovery(HANDLE_T h_cecm_svc)
{
    RPC_DECL(1, INT32);  

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cecm_svc);     

    RPC_DO_OP("x_cecm_stop_dev_discovery");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);  

}
/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_active_source
 *
 * Description: This API is used to response to a <Request Active Source>.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_active_source(HANDLE_T h_cecm_svc)
{
    RPC_DECL(1, INT32);  

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cecm_svc);     

    RPC_DO_OP("x_cecm_set_active_source");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);      
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_request_active_source
 *
 * Description: This API discovers the current active source of the system.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_request_active_source(HANDLE_T h_cecm_svc)
{
    RPC_DECL(1, INT32);  

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cecm_svc);     

    RPC_DO_OP("x_cecm_request_active_source");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);       
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_standby
 *
 * Description: The API switches a device into standby mode.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_OUT_OF_RESOURCE       Not enough CECM resource.
 *          CECMR_OUT_OF_HANDLE         No handle available.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_standby(HANDLE_T        h_cecm_svc,
                         CECM_LOG_ADDR_T e_la)
{
    RPC_DECL(2, INT32);  

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_la);     

    RPC_DO_OP("x_cecm_set_standby");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);      
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_standby_all
 *
 * Description: The API switches all devices into standby mode.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          b_sync          Indicates if this is a synchronous operation.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_standby_all(HANDLE_T h_cecm_svc,
                             BOOL     b_sync)
{
    RPC_DECL(2, INT32);  

    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_BOOL, b_sync);     

    RPC_DO_OP("x_cecm_set_standby_all");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);      
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_routing_change
 *
 * Description: Ths API informs all devices on the newwork that the active
 *              route had been changed.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          ui1_orig_comp_id    Original HDMI component ID.
 *          ui1_new_comp_id     New HDMI component ID.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_routing_change(HANDLE_T h_cecm_svc,
                                UINT16   ui2_orig_comp_id,
                                UINT16   ui2_new_comp_id)
{
    RPC_DECL(3, INT32);  

    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_orig_comp_id); 
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_new_comp_id);  

    RPC_DO_OP("x_cecm_set_routing_change");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);       
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_stream_path
 *
 * Description: This API requests a streaming path from the specified logical
 *              device.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_OUT_OF_RESOURCE       Not enough CECM resource.
 *          CECMR_OUT_OF_HANDLE         No handle available.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_stream_path(HANDLE_T        h_cecm_svc,
                             CECM_LOG_ADDR_T e_la)
{
    RPC_DECL(2, INT32);  

    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la); 

    RPC_DO_OP("x_cecm_set_stream_path");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);      
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_stream_path_ex
 *
 * Description: This API requests a streaming path from the specified logical
 *              device.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          ui2_pa              A physical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_OUT_OF_RESOURCE       Not enough CECM resource.
 *          CECMR_OUT_OF_HANDLE         No handle available.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_stream_path_ex(HANDLE_T h_cecm_svc,
                                UINT16   ui2_pa)
{
    RPC_DECL(2, INT32);  

    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_pa); 

    RPC_DO_OP("x_cecm_set_stream_path_ex");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);       
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_get_cec_version
 *
 * Description: This API enquires which version of CEC the target supports.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_get_cec_version(HANDLE_T        h_cecm_svc,
                             CECM_LOG_ADDR_T e_la)
{
    RPC_DECL(2, INT32);  

    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la); 

    RPC_DO_OP("x_cecm_get_cec_version");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);     
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_give_physical_address
 *
 * Description: This API requests a device to return its physical address.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_give_physical_address(HANDLE_T        h_cecm_svc,
                                   CECM_LOG_ADDR_T e_la)
{

    RPC_DECL(2, INT32);  

    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la); 

    RPC_DO_OP("x_cecm_give_physical_address");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);      
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_get_menu_lang
 *
 * Description: This API discovers the currently set menu language of a device.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_get_menu_lang(HANDLE_T        h_cecm_svc,
                           CECM_LOG_ADDR_T e_la)
{
    RPC_DECL(2, INT32);  

    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la); 

    RPC_DO_OP("x_cecm_get_menu_lang");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);      
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_polling_message
 *
 * Description: This API pings a CEC device.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          b_sync              Indicates if this is a synchronous operation.
 *
 * Outputs: pt_polling_info     References polling info.
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_polling_message(HANDLE_T             h_cecm_svc,
                                 CECM_LOG_ADDR_T      e_la,
                                 BOOL                 b_sync,
                                 CECM_POLLING_INFO_T* pt_polling_info)
{
    RPC_DECL(4, INT32);  
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_polling_info, RPC_DESC_CECM_POLLING_INFO_T, NULL));   
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la); 
    RPC_ARG_INP(ARG_TYPE_BOOL, b_sync); 
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_polling_info); 

    RPC_DO_OP("x_cecm_set_polling_message");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);  
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_report_physical_address
 *
 * Description: This API informs all other devices of the mapping between
 *              physical and logical address.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          pt_report_phy_addr  References the report physical address data
 *                              structure.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_report_physical_address(HANDLE_T                h_cecm_svc,
                                     CECM_REPORT_PHY_ADDR_T* pt_report_phy_addr)
{
    RPC_DECL(2, INT32);  
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_report_phy_addr, RPC_DESC_CECM_REPORT_PHY_ADDR_T, NULL));   
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_report_phy_addr); 

    RPC_DO_OP("x_cecm_report_physical_address");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);      
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_menu_lang
 *
 * Description: This API indicates the menu language.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          pt_lang         References ISO-639 language.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_menu_lang(HANDLE_T        h_cecm_svc,
                           ISO_639_LANG_T* pt_lang)
{
    RPC_DECL(2, INT32);  
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_REF_STR, pt_lang); 

    RPC_DO_OP("x_cecm_set_menu_lang");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);       
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_give_deck_status
 *
 * Description: This API requests the status of a device.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *          e_sts_req       Contains status request.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_give_deck_status(HANDLE_T        h_cecm_svc,
                              CECM_LOG_ADDR_T e_la,
                              CECM_STS_REQ_T  e_sts_req)
{
    RPC_DECL(3, INT32);  
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);   
    RPC_ARG_INP(ARG_TYPE_UINT32, e_sts_req);   
    
    RPC_DO_OP("x_cecm_give_deck_status");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);        
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_deck_ctrl
 *
 * Description: This API is used to control a device's media functions.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          e_deck_ctrl_mod     Contains deck control mode.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_deck_ctrl(HANDLE_T             h_cecm_svc,
                           CECM_LOG_ADDR_T      e_la,
                           CECM_DECK_CTRL_MOD_T e_deck_ctrl_mod)
{
    RPC_DECL(3, INT32);   
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);     
    RPC_ARG_INP(ARG_TYPE_UINT32, e_deck_ctrl_mod);     

    RPC_DO_OP("x_cecm_set_deck_ctrl");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);        
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_deck_play
 *
 * Description: This API is used to control the playback behavior of a source
 *              device.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *          e_play_mod      Contains play mode.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_deck_play(HANDLE_T        h_cecm_svc,
                           CECM_LOG_ADDR_T e_la,
                           CECM_PLAY_MOD_T e_play_mod)
{
    RPC_DECL(3, INT32);   
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);     
    RPC_ARG_INP(ARG_TYPE_UINT32, e_play_mod);     

    RPC_DO_OP("x_cecm_set_deck_play");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);   

}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_osd_name
 *
 * Description: This API sets the preferred OSD name of a device for use in
 *              menus associated with that device.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *          paw2_osd_name   References a OSD name.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_osd_name(HANDLE_T        h_cecm_svc,
                          CECM_LOG_ADDR_T e_la,
                          UTF16_T*        paw2_osd_name)
{  
    RPC_DECL(3, INT32);   
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);     
    RPC_ARG_INP(ARG_TYPE_REF_STR, paw2_osd_name);     

    RPC_DO_OP("x_cecm_set_osd_name");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);     
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_give_osd_name
 *
 * Description: This API requests the preferred OSD name of a device for use in
 *              menus associated with that device.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_give_osd_name(HANDLE_T        h_cecm_svc,
                           CECM_LOG_ADDR_T e_la)
{
    RPC_DECL(2, INT32);   
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);         

    RPC_DO_OP("x_cecm_give_osd_name");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);      
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_menu_request
 *
 * Description: This API is used to request a device to show/remove a menu or
 *              to query if a device is currently showing a menu.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          e_menu_req_type     Contains menu request type.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_menu_request(HANDLE_T              h_cecm_svc,
                              CECM_LOG_ADDR_T       e_la,
                              CECM_MENU_REQ_STATE_T e_menu_req_type)
{
    RPC_DECL(3, INT32);   
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);     
    RPC_ARG_INP(ARG_TYPE_UINT32, e_menu_req_type);         

    RPC_DO_OP("x_cecm_set_menu_request");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);        
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_device_vendor_id
 *
 * Description: This API reports the vendor ID of this device.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          pt_dev_vndr_id  References a device vendor ID.
 *          b_sync          Indicates if this is a synchronous operation.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_device_vendor_id(HANDLE_T            h_cecm_svc,
                                  CECM_DEV_VNDR_ID_T* pt_dev_vndr_id,
                                  BOOL                b_sync)
{
    RPC_DECL(3, INT32);   

    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_dev_vndr_id, CECM_DEV_VNDR_ID_SIZE));        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_dev_vndr_id);     
    RPC_ARG_INP(ARG_TYPE_BOOL, b_sync);         

    RPC_DO_OP("x_cecm_set_device_vendor_id");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);         
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_give_device_vendor_id
 *
 * Description: This API requests the vendor ID from a device.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *          b_sync          Indicates if this is a synchronous operation.
 *
 * Outputs: pt_dev_vndr_id  References a device vendor ID.
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_give_device_vendor_id(HANDLE_T            h_cecm_svc,
                                   CECM_LOG_ADDR_T     e_la,
                                   BOOL                b_sync,
                                   CECM_DEV_VNDR_ID_T* pt_dev_vndr_id)
{  
    RPC_DECL(4, INT32);   

    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_dev_vndr_id, CECM_DEV_VNDR_ID_SIZE));           
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);    
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);     
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_dev_vndr_id);     
    RPC_ARG_INP(ARG_TYPE_BOOL, b_sync);         

    RPC_DO_OP("x_cecm_give_device_vendor_id");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);       
}

                                   
/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_user_ctrl_pressed
 *
 * Description: This API indicates that the user pressed a remote control
 *              button or switched from one remote control button to another.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *          e_usr_ctrl      Contains user control info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_user_ctrl_pressed(HANDLE_T         h_cecm_svc,
                                   CECM_LOG_ADDR_T  e_la,
                                   CECM_USER_CTRL_T e_usr_ctrl)
{
    RPC_DECL(3, INT32);   
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);    
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);     
    RPC_ARG_INP(ARG_TYPE_UINT32, e_usr_ctrl);           

    RPC_DO_OP("x_cecm_set_user_ctrl_pressed");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);     
}

/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_user_ctrl_pressed_ex
 *
 * Description: This API indicates that the user pressed a remote control
 *              button or switched from one remote control button to another,
 *              and can specify additional information.
 *
 * Inputs:  h_cecm_svc                  A handle referencing the service.
 *          e_la                        CEC device logical address.
 *          pt_usr_ctrl_with_operand    Reference of user control info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_user_ctrl_pressed_ex(HANDLE_T         h_cecm_svc,
                                   CECM_LOG_ADDR_T        e_la,
                                   CECM_USER_CTRL_WITH_OPERAND_T*   pt_usr_ctrl_with_operand)
{
    RPC_DECL(3, INT32);  

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_usr_ctrl_with_operand, RPC_DESC_CECM_USER_CTRL_WITH_OPERAND_T, NULL));   
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);     
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_usr_ctrl_with_operand);

    RPC_DO_OP("x_cecm_set_user_ctrl_pressed_ex");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);  

}

/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_user_ctrl_released
 *
 * Description: This API indicates that the user released a remote control
 *              button (the last one indicated by <User Control Pressed>
 *              message).
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_user_ctrl_released(HANDLE_T        h_cecm_svc,
                                    CECM_LOG_ADDR_T e_la)
{
    RPC_DECL(2, INT32);  
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);     

    RPC_DO_OP("x_cecm_set_user_ctrl_released");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);      
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_power_on_dev
 *
 * Description: This API powers on a device.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_power_on_dev(HANDLE_T        h_cecm_svc,
                          CECM_LOG_ADDR_T e_la)
{
    RPC_DECL(2, INT32);  
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);     

    RPC_DO_OP("x_cecm_power_on_dev");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);      
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_give_device_power_status
 *
 * Description: This API is used to determine the current power status of a
 *              target device.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *          b_sync          Indicates if this is a synchronous operation.
 *
 * Outputs: pe_pwr_sts      References power status.
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_give_device_power_status(HANDLE_T        h_cecm_svc,
                                      CECM_LOG_ADDR_T e_la,
                                      BOOL            b_sync,
                                      CECM_PWR_STS_T* pe_pwr_sts)
{
    RPC_DECL(4, INT32);  
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);     
    RPC_ARG_INP(ARG_TYPE_BOOL, b_sync);     
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_pwr_sts);     

    RPC_DO_OP("x_cecm_give_device_power_status");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);      
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_report_power_status
 *
 * Description: This API reports current power status.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *          e_pwr_sts       Contains power status.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_report_power_status(HANDLE_T        h_cecm_svc,
                                 CECM_LOG_ADDR_T e_la,
                                 CECM_PWR_STS_T  e_pwr_sts)
{
    RPC_DECL(3, INT32);  
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);     
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);       
    RPC_ARG_INP(ARG_TYPE_UINT32, e_pwr_sts);     

    RPC_DO_OP("x_cecm_report_power_status");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);       
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_system_audio_mode_request
 *
 * Description: This API requests to use System Audio Mode to the amplifier.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          pt_sys_aud_ctrl     References the system audio control info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_system_audio_mode_request(HANDLE_T             h_cecm_svc,
                                           CECM_SYS_AUD_CTRL_T* pt_sys_aud_ctrl)
{
    RPC_DECL(2, INT32);  

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_sys_aud_ctrl, RPC_DESC_CECM_SYS_AUD_CTRL_T, NULL));   
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);         
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_sys_aud_ctrl);

    RPC_DO_OP("x_cecm_set_system_audio_mode_request");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);      
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_system_audio_mode_request_ex
 *
 * Description: This API requests to use System Audio Mode to the amplifier.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          pt_sys_aud_ctrl     References the system audio control info.
 *          b_sync              Indicates if this is a synchronous operation.
 *
 * Outputs: pe_sys_aud_sts      References system audio status.
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_system_audio_mode_request_ex(HANDLE_T             h_cecm_svc,
                                              CECM_SYS_AUD_CTRL_T* pt_sys_aud_ctrl,
                                              BOOL                 b_sync,
                                              CECM_SYS_AUD_STS_T*  pe_sys_aud_sts)
{
    RPC_DECL(4, INT32);  

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_sys_aud_ctrl, RPC_DESC_CECM_SYS_AUD_CTRL_T, NULL));   
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);    
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_sys_aud_ctrl);
    RPC_ARG_INP(ARG_TYPE_BOOL, b_sync); 
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_sys_aud_sts);  
     
    RPC_DO_OP("x_cecm_set_system_audio_mode_request_ex");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);       
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_give_audio_status
 *
 * Description: This API requests an amplifier to send its volume and mute
 *              status.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          b_sync              Indicates if this is a synchronous operation.
 *
 * Outputs: pui1_aud_sts        References audio status of an amplifier (This
 *                              argument is valid when b_sync = TRUE and is
 *                              ignored if b_sync = FALSE).
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_give_audio_status(HANDLE_T        h_cecm_svc,
                               CECM_LOG_ADDR_T e_la,
                               BOOL            b_sync,
                               UINT8*          pui1_aud_sts)
{
    RPC_DECL(4, INT32);  
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);    
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);
    RPC_ARG_INP(ARG_TYPE_BOOL, b_sync);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT8, pui1_aud_sts); 
     
    RPC_DO_OP("x_cecm_give_audio_status");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);       
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_give_system_audio_mode_status
 *
 * Description: This API requests the status of the System Audio Mode.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_give_system_audio_mode_status(HANDLE_T        h_cecm_svc,
                                           CECM_LOG_ADDR_T e_la)
{
    RPC_DECL(2, INT32); 
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);    
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);  
     
    RPC_DO_OP("x_cecm_give_system_audio_mode_status");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);        
}
/*-----------------------------------------------------------------------------
 * Name: c_cecm_request_audio_descriptor
 *
 * Description: This API requests the audio descriptor the device support
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          pt_aud_fmt          Reference audio format code
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/

INT32 c_cecm_request_audio_descriptor(HANDLE_T h_cecm_svc,
                                             CECM_LOG_ADDR_T e_la,
                                             CECM_AUD_FMT_T*   pt_aud_fmt,
                                             BOOL  b_sync)
{
    RPC_DECL(4, INT32);  

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_aud_fmt, RPC_DESC_CECM_AUD_FMT_T, NULL));   
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);   
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);  
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_aud_fmt);
    RPC_ARG_INP(ARG_TYPE_BOOL, b_sync); 
     
    RPC_DO_OP("x_cecm_request_audio_descriptor");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);       
}

/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_audio_rate
 *
 * Description: This API is used to control audio rate from source device.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          e_aud_rate          Contains the audio rate info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_audio_rate(HANDLE_T        h_cecm_svc,
                            CECM_LOG_ADDR_T e_la,
                            CECM_AUD_RATE_T e_aud_rate)
{

    RPC_DECL(3, INT32);  
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);   
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);  
    RPC_ARG_INP(ARG_TYPE_UINT32, e_aud_rate);
     
    RPC_DO_OP("x_cecm_set_audio_rate");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);      
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_clear_timer
 *
 * Description: This API is used to clear a timer block of a device.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          pt_timer_info       References the timer info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_clear_timer(HANDLE_T           h_cecm_svc,
                         CECM_LOG_ADDR_T    e_la,
                         CECM_TIMER_INFO_T* pt_timer_info)
{
    RPC_DECL(3, INT32);  

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_timer_info, RPC_DESC_CECM_TIMER_INFO_T, NULL));   
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);   
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);  
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_timer_info);
     
    RPC_DO_OP("x_cecm_clear_timer");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);       
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_timer
 *
 * Description: This API is used to set a single timer block of a device.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          pt_timer_info       References the timer info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_timer(HANDLE_T           h_cecm_svc,
                       CECM_LOG_ADDR_T    e_la,
                       CECM_TIMER_INFO_T* pt_timer_info)
{
    RPC_DECL(3, INT32);  

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_timer_info, RPC_DESC_CECM_TIMER_INFO_T, NULL));   
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);   
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);  
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_timer_info);
     
    RPC_DO_OP("x_cecm_set_timer");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);     
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_timer_prog_title
 *
 * Description: This API is used to set the name of a program associated with a
 *              timer block.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          pt_prog_title_str   References a program title string.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_timer_prog_title(HANDLE_T           h_cecm_svc,
                                  CECM_LOG_ADDR_T    e_la,
                                  CECM_PROG_TITLE_T* pt_prog_title_str)
{
    RPC_DECL(3, INT32);  
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);   
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);  
    RPC_ARG_INP(ARG_TYPE_REF_STR, pt_prog_title_str);
     
    RPC_DO_OP("x_cecm_set_timer_prog_title");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);      
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_give_tuner_status
 *
 * Description: This API is used to request the status of a tuner.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          e_sts_req           Contains status request.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_give_tuner_status(HANDLE_T        h_cecm_svc,
                               CECM_LOG_ADDR_T e_la,
                               CECM_STS_REQ_T  e_sts_req)
{
    RPC_DECL(3, INT32);  
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);   
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);  
    RPC_ARG_INP(ARG_TYPE_UINT32, e_sts_req);
     
    RPC_DO_OP("x_cecm_give_tuner_status");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);       
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_select_service
 *
 * Description: This API is used to directly select an analog TV, digital TV,
 *              radio or data broadcast service.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          pt_svc_info         References the tuner service info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error. 
 ----------------------------------------------------------------------------*/
INT32 c_cecm_select_service(HANDLE_T               h_cecm_svc,
                            CECM_LOG_ADDR_T        e_la,
                            CECM_TUNER_SVC_INFO_T* pt_svc_info)
{
    RPC_DECL(3, INT32);  

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_svc_info, RPC_DESC_CECM_TUNER_SVC_INFO_T, NULL)); 

    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);   
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);  
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_svc_info);
     
    RPC_DO_OP("x_cecm_select_service");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);       
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_tuner_step
 *
 * Description: This API is used to tune to next lowest/highest service in a
 *              tuner's service list.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          e_tuner_step        Contains tuner step info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_tuner_step(HANDLE_T          h_cecm_svc,
                            CECM_LOG_ADDR_T   e_la,
                            CECM_TUNER_STEP_T e_tuner_step)
{
    RPC_DECL(3, INT32);  
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);   
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);  
    RPC_ARG_INP(ARG_TYPE_UINT32, e_tuner_step);
     
    RPC_DO_OP("x_cecm_set_tuner_step");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);  

}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_record_on
 *
 * Description: This API attempts to record the specific source.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          pt_rec_src          References the record source info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_record_on(HANDLE_T        h_cecm_svc,
                           CECM_LOG_ADDR_T e_la,
                           CECM_REC_SRC_T* pt_rec_src)
{
    RPC_DECL(3, INT32);  

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_rec_src, RPC_DESC_CECM_REC_SRC_T, NULL));   
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);   
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);  
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_rec_src);
     
    RPC_DO_OP("x_cecm_set_record_on");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);         
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_record_off
 *
 * Description: This API requests a device to stop a recording.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_record_off(HANDLE_T        h_cecm_svc,
                            CECM_LOG_ADDR_T e_la)
{
    RPC_DECL(2, INT32);  
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);   
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);  
     
    RPC_DO_OP("x_cecm_set_record_off");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);     
}

/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_feature_abort
 *
 * Description: This API is used as a response to indicate that the requested
 *              message type is not supported.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          pt_feature_abort    References the feature abort info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_feature_abort(HANDLE_T              h_cecm_svc,
                               CECM_LOG_ADDR_T       e_la,
                               CECM_FEATURE_ABORT_T* pt_feature_abort)
{
    RPC_DECL(3, INT32);  

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_feature_abort, RPC_DESC_CECM_FEATURE_ABORT_T, NULL));   
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);   
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);  
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_feature_abort);
     
    RPC_DO_OP("x_cecm_set_feature_abort");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);        
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_vendor_cmd
 *
 * Description: This API allows vendor specific commands to be sent between
 *              two devices.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          pt_vndr_cmd_data    References vendor specific data.
 *          b_sync              Indicates if this is a synchronous operation.  
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_vendor_cmd(HANDLE_T              h_cecm_svc,
                            CECM_LOG_ADDR_T       e_la,
                            CECM_VNDR_CMD_DATA_T* pt_vndr_cmd_data,
                            BOOL                  b_sync)
{
    RPC_DECL(4, INT32);  

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_vndr_cmd_data, RPC_DESC_CECM_VNDR_CMD_DATA_T, NULL));   
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);   
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);  
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_vndr_cmd_data);
    RPC_ARG_INP(ARG_TYPE_BOOL, b_sync);  
     
    RPC_DO_OP("x_cecm_set_vendor_cmd");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);     
}    


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_vendor_cmd_with_id
 *
 * Description: This API allows vendor specific commands to be sent between
 *              two devices or broadcast.
 *
 * Inputs:  h_cecm_svc                  A handle referencing the service.
 *          e_la                        CEC device logical address.
 *          pt_vndr_cmd_with_id_data    References vendor specific data with 
 *                                      ID.
 *          b_sync                      Indicates if this is a synchronous 
 *                                      operation.  
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_vendor_cmd_with_id(HANDLE_T                      h_cecm_svc,
                                    CECM_LOG_ADDR_T               e_la,
                                    CECM_VNDR_CMD_WITH_ID_DATA_T* pt_vndr_cmd_with_id_data,
                                    BOOL                          b_sync)
{
    RPC_DECL(4, INT32);  

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_vndr_cmd_with_id_data, RPC_DESC_CECM_VNDR_CMD_WITH_ID_DATA_T, NULL));   
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);   
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);  
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_vndr_cmd_with_id_data);
    RPC_ARG_INP(ARG_TYPE_BOOL, b_sync);  
     
    RPC_DO_OP("x_cecm_set_vendor_cmd_with_id");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);      
}    


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_vendor_remote_btn_down
 *
 * Description: This API indicates that a remote control button has been
 *              depressed.
 *
 * Inputs:  h_cecm_svc              A handle referencing the service.
 *          e_la                    CEC device logical address.
 *          pt_vndr_rc_code_data    References vendor specific RC code.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_vendor_remote_btn_down(HANDLE_T                  h_cecm_svc,
                                        CECM_LOG_ADDR_T           e_la,
                                        CECM_VNDR_RC_CODE_DATA_T* pt_vndr_rc_code_data)
{
    RPC_DECL(3, INT32);  

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_vndr_rc_code_data, RPC_DESC_CECM_VNDR_RC_CODE_DATA_T, NULL));   
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);   
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);  
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_vndr_rc_code_data);
     
    RPC_DO_OP("x_cecm_set_vendor_remote_btn_down");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);     
}    


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_vendor_remote_btn_up
 *
 * Description: This API indicates that a remote control button (the last
 *              button pressed indicated by the Vendor Remote Button Down
 *              message) Button has been released.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 c_cecm_set_vendor_remote_btn_up(HANDLE_T        h_cecm_svc,
                                      CECM_LOG_ADDR_T e_la)
{
    RPC_DECL(2, INT32);  
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);   
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);  
     
    RPC_DO_OP("x_cecm_set_vendor_remote_btn_up");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);      
}    

INT32 c_cecm_arc_ctrl(HANDLE_T                  h_cecm_svc,
                         CECM_ARC_CTRL_T*          pt_arc_ctrl)
{
    RPC_DECL(2, INT32);  

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_arc_ctrl, RPC_DESC_CECM_ARC_CTRL_T, NULL));       
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);   
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_arc_ctrl);  
     
    RPC_DO_OP("x_cecm_arc_ctrl");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);  

}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_request_init_arc
 *
 * Description: 
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    
 *   
 ----------------------------------------------------------------------------*/
INT32 c_cecm_request_init_arc(HANDLE_T h_cecm_svc,
                                  CECM_LOG_ADDR_T e_la,
                                  BOOL  b_sync)
{
    
    RPC_DECL(3, INT32);  
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);   
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);  
    RPC_ARG_INP(ARG_TYPE_BOOL, b_sync);
     
    RPC_DO_OP("x_cecm_request_init_arc");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);      
}
/*-----------------------------------------------------------------------------
 * Name: c_cecm_request_terminate_arc
 *
 * Description: 
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    
 *   
 ----------------------------------------------------------------------------*/

INT32 c_cecm_request_terminate_arc(HANDLE_T h_cecm_svc,
                                   CECM_LOG_ADDR_T e_la,
                                   BOOL    b_sync)
{    
    RPC_DECL(3, INT32);  
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);   
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);  
    RPC_ARG_INP(ARG_TYPE_BOOL, b_sync);
     
    RPC_DO_OP("x_cecm_request_terminate_arc");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);     
}
/*-----------------------------------------------------------------------------
 * Name: c_cecm_repoert_init_arc
 *
 * Description: 
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    
 *   
 ----------------------------------------------------------------------------*/

INT32 c_cecm_report_init_arc(HANDLE_T h_cecm_svc,
                              CECM_LOG_ADDR_T e_la,
                              BOOL    b_sync)
{   
    RPC_DECL(3, INT32);  
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);   
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);  
    RPC_ARG_INP(ARG_TYPE_BOOL, b_sync);
     
    RPC_DO_OP("x_cecm_report_init_arc");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);     
}
/*-----------------------------------------------------------------------------
 * Name: c_cecm_report_terminate_arc
 *
 * Description: 
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    
 *   
 ----------------------------------------------------------------------------*/


INT32 c_cecm_report_terminate_arc(HANDLE_T h_cecm_svc,
                                  CECM_LOG_ADDR_T e_la,
                                  BOOL    b_sync)
{
    RPC_DECL(3, INT32);  
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_cecm_svc);   
    RPC_ARG_INP(ARG_TYPE_UINT32, e_la);  
    RPC_ARG_INP(ARG_TYPE_BOOL, b_sync);
     
    RPC_DO_OP("x_cecm_report_terminate_arc");

    RPC_RETURN(ARG_TYPE_INT32, CECMR_ERROR);       
}


INT32 c_rpc_reg_cecm_cb_hndlrs()
{
    RPC_REG_CB_HNDLR(x_cecm_nfy_fct);    

    return RPCR_OK;
}

