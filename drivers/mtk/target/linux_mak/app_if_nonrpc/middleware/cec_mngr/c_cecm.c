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

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
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

    i4_return = aee_grab_handle_resource(AEE_FLAG,
                                         1,
                                         &h_aux);

    if (i4_return == AEER_OK)
    {
        i4_return = x_cecm_open_svc(pf_nfy,
                                    pv_tag,
                                    ph_cecm_svc);

        if (i4_return == CECMR_OK)
        {
            handle_link_to_aux(h_aux, *ph_cecm_svc);
        }
        else
        {
            aee_release_handle_resource(1);
        }
    }

    return i4_return;
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
    return (x_cecm_close_svc(h_cecm_svc));
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
    return (x_cecm_signal_cec_ready(h_cecm_svc,
                                    b_ready));     
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
    return (x_cecm_set_cec_wakeup_ctrl(h_cecm_svc,
                                       pt_wakeup_ctrl));
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
    return (x_cecm_set_cec_fct(h_cecm_svc,
                               t_params));
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
    return (x_cecm_is_act_src_reved(h_cecm_svc, pt_act_src_reved));
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
    return (x_cecm_get_la_by_pa(h_cecm_svc,
                                ui2_pa));
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
    return (x_cecm_get_la_by_pa_ex(h_cecm_svc,
                                   ui2_pa,
                                   pt_la_lst));
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
    return (x_cecm_get_pa_by_la(h_cecm_svc,
                                e_la));
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
    return (x_cecm_get_dev_info_by_la(h_cecm_svc,
                                      e_la,
                                      pt_ctrl,
                                      pt_dev_info));
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
    return (x_cecm_get_comp_id_by_la(h_cecm_svc,
                                     e_la,
                                     pui2_comp_id));
}
/*-----------------------------------------------------------------------------
 * Name: c_cecm_get_comp_id_by_la_ex
 *
 * Description: The API gets component ID by logical address.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *          b_retry         Do retry if device list not avliable
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
    return (x_cecm_get_comp_id_by_la_ex(h_cecm_svc,
                                     e_la,
                                     pui2_comp_id,
                                     b_retry));
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
    return (x_cecm_set_max_tv_la(h_cecm_svc, e_la));
}


/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_support_cec_ver
 *
 * Description: The API set the support CEC version, will used this version response <Get CEC Version>
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_cec_ver       The logical address 
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 ----------------------------------------------------------------------------*/

INT32 c_cecm_set_support_cec_ver(HANDLE_T        h_cecm_svc,
                              CECM_CEC_VER_T   e_cec_ver)
{
    return (x_cecm_set_support_cec_ver(h_cecm_svc, e_cec_ver));
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
    return (x_cecm_find_tv_la_alloc(h_cecm_svc));    
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
    return (x_cecm_find_tv_la_alloc_ex(h_cecm_svc, ui4_itvl));    
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
    return (x_cecm_config_dev_discovery(h_cecm_svc,pt_dev_discovery_config));
}
/*-----------------------------------------------------------------------------
 * Name: c_cecm_get_dev_discovery_config
 *
 * Description: The API get discovery config
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

INT32 c_cecm_get_dev_discovery_config(HANDLE_T        h_cecm_svc,
                                             CECM_DEV_DISCOVERY_CONFIG_T*   pt_dev_discovery_config)

{
    return (x_cecm_get_dev_discovery_config(h_cecm_svc,pt_dev_discovery_config));
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
    return (x_cecm_start_dev_discovery(h_cecm_svc,
                                       ui4_dev_discovery_itvl));
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
    return (x_cecm_start_dev_discovery_ex(h_cecm_svc,
                                          pt_dev_discovery_info));
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
    return (x_cecm_stop_dev_discovery(h_cecm_svc));
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
    return (x_cecm_set_active_source(h_cecm_svc));
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
    return (x_cecm_request_active_source(h_cecm_svc));
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
    return (x_cecm_set_standby(h_cecm_svc,
                               e_la));
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
    return (x_cecm_set_standby_all(h_cecm_svc,
                                   b_sync));
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
    return (x_cecm_set_routing_change(h_cecm_svc,
                                      ui2_orig_comp_id,
                                      ui2_new_comp_id));
}

/*-----------------------------------------------------------------------------
 * Name: c_cecm_set_routing_change_ex
 *
 * Description: Ths API informs all devices on the newwork that the active
 *              route had been changed.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          ui2_orig_pa     Original CEC device PA.
 *          ui2_new_pa     New CEC device PA.
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
INT32 c_cecm_set_routing_change_ex(HANDLE_T h_cecm_svc,
                                    UINT16   ui2_orig_pa,
                                    UINT16   ui2_new_pa)
{
    return (x_cecm_set_routing_change_ex(h_cecm_svc,
                                    ui2_orig_pa,
                                    ui2_new_pa));
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
    return (x_cecm_set_stream_path(h_cecm_svc,
                                   e_la));
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
    return (x_cecm_set_stream_path_ex(h_cecm_svc,
                                      ui2_pa));
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
    return (x_cecm_get_cec_version(h_cecm_svc,
                                   e_la));
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
    return (x_cecm_give_physical_address(h_cecm_svc,
                                         e_la));
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
    return (x_cecm_get_menu_lang(h_cecm_svc,
                                 e_la));
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
    return (x_cecm_set_polling_message(h_cecm_svc,
                                       e_la,
                                       b_sync,
                                       pt_polling_info));
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
    return (x_cecm_report_physical_address(h_cecm_svc,
                                           pt_report_phy_addr));
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
    return (x_cecm_set_menu_lang(h_cecm_svc,
                                 pt_lang));
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
    return (x_cecm_give_deck_status(h_cecm_svc,
                                    e_la,
                                    e_sts_req));
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
    return (x_cecm_set_deck_ctrl(h_cecm_svc,
                                 e_la,
                                 e_deck_ctrl_mod));
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
    return (x_cecm_set_deck_play(h_cecm_svc,
                                 e_la,
                                 e_play_mod));
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
    return (x_cecm_set_osd_name(h_cecm_svc,
                                e_la,
                                paw2_osd_name));    
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
    return (x_cecm_give_osd_name(h_cecm_svc,
                                 e_la));
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
    return (x_cecm_set_menu_request(h_cecm_svc,
                                    e_la,
                                    e_menu_req_type));
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
    return (x_cecm_set_device_vendor_id(h_cecm_svc,
                                        pt_dev_vndr_id,
                                        b_sync));
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
    return (x_cecm_give_device_vendor_id(h_cecm_svc,
                                         e_la,
                                         b_sync,
                                         pt_dev_vndr_id));    
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
    return (x_cecm_set_user_ctrl_pressed(h_cecm_svc,
                                         e_la,
                                         e_usr_ctrl));
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
    return (x_cecm_set_user_ctrl_pressed_ex(h_cecm_svc,
                                         e_la,
                                         pt_usr_ctrl_with_operand));
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
    return (x_cecm_set_user_ctrl_released(h_cecm_svc,
                                          e_la));
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
    return (x_cecm_power_on_dev(h_cecm_svc,
                                e_la));
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
    return (x_cecm_give_device_power_status(h_cecm_svc,
                                            e_la,
                                            b_sync,
                                            pe_pwr_sts));
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
    return (x_cecm_report_power_status(h_cecm_svc,
                                       e_la,
                                       e_pwr_sts));
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
    return (x_cecm_set_system_audio_mode_request(h_cecm_svc,
                                                 pt_sys_aud_ctrl));
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
    return (x_cecm_set_system_audio_mode_request_ex(h_cecm_svc,
                                                    pt_sys_aud_ctrl,
                                                    b_sync,
                                                    pe_sys_aud_sts));
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
    return (x_cecm_give_audio_status(h_cecm_svc,
                                     e_la,
                                     b_sync,
                                     pui1_aud_sts));
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
    return (x_cecm_give_system_audio_mode_status(h_cecm_svc,
                                                 e_la));
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
    return (x_cecm_request_audio_descriptor(h_cecm_svc ,
                                           e_la,
                                           pt_aud_fmt,                                                                                   
                                           b_sync));
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
    return (x_cecm_set_audio_rate(h_cecm_svc,
                                  e_la,
                                  e_aud_rate));
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
    return (x_cecm_clear_timer(h_cecm_svc,
                               e_la,
                               pt_timer_info));
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
    return (x_cecm_set_timer(h_cecm_svc,
                             e_la,
                             pt_timer_info));
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
    return (x_cecm_set_timer_prog_title(h_cecm_svc,
                                        e_la,
                                        pt_prog_title_str));
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
    return (x_cecm_give_tuner_status(h_cecm_svc,
                                     e_la,
                                     e_sts_req));
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
    return (x_cecm_select_service(h_cecm_svc,
                                  e_la,
                                  pt_svc_info));
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
    return (x_cecm_set_tuner_step(h_cecm_svc,
                                  e_la,
                                  e_tuner_step));
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
    return (x_cecm_set_record_on(h_cecm_svc,
                                 e_la,
                                 pt_rec_src));
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
    return (x_cecm_set_record_off(h_cecm_svc,
                                  e_la));
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
    return (x_cecm_set_feature_abort(h_cecm_svc,
                                     e_la,
                                     pt_feature_abort));
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
    return (x_cecm_set_vendor_cmd(h_cecm_svc,
                                  e_la,
                                  pt_vndr_cmd_data,
                                  b_sync));
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
    return (x_cecm_set_vendor_cmd_with_id(h_cecm_svc,
                                          e_la,
                                          pt_vndr_cmd_with_id_data,
                                          b_sync));
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
    return (x_cecm_set_vendor_remote_btn_down(h_cecm_svc,
                                              e_la,
                                              pt_vndr_rc_code_data));
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
    return (x_cecm_set_vendor_remote_btn_up(h_cecm_svc,
                                            e_la));
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
INT32 c_cecm_set_specific_cmd(HANDLE_T              h_cecm_svc,
                            CECM_LOG_ADDR_T       e_la,
                            CECM_OPCODE_T         e_opcode,
                            CECM_SPEC_CMD_DATA_T* pt_spec_cmd_data,
                            BOOL                  b_sync)
{
    return (x_cecm_set_specific_cmd(h_cecm_svc,
                                    e_la,
                                    e_opcode,
                                    pt_spec_cmd_data,
                                    b_sync));
}

/*-----------------------------------------------------------------------------
 * Name: c_cecm_arc_ctrl
 *
 * Description: The API control ARC
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
INT32 c_cecm_arc_ctrl(HANDLE_T                  h_cecm_svc,
                         CECM_ARC_CTRL_T*          pt_arc_ctrl)
{
    return (x_cecm_arc_ctrl(h_cecm_svc,
                            pt_arc_ctrl));
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
    return(x_cecm_request_init_arc(h_cecm_svc, e_la, b_sync));
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
    return (x_cecm_request_terminate_arc(h_cecm_svc, e_la, b_sync));
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
    return (x_cecm_report_init_arc(h_cecm_svc, e_la, b_sync));
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
    return (x_cecm_report_terminate_arc(h_cecm_svc, e_la, b_sync));
}

