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
#include "pcl/x_pcl.h"

#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"

static INT32 _hndlr_x_pcl_nfy_fct(
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
    
      ((x_pcl_nfy_fct)pv_cb_addr)(
            (HANDLE_T)pt_args[0].u.ui4_arg,
                      pt_args[1].u.pv_arg,
          (PCL_COND_T)pt_args[2].u.ui4_arg,
                      pt_args[3].u.ui4_arg);
                      
    return RPCR_OK;
}

INT32 c_pcl_get_wakeup_capability(UINT32* pui4_wakeup_cap,
                                  UINT32* pui4_wakeup_ctrl_cap)
{
    RPC_DECL(2, INT32);
      
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,  pui4_wakeup_cap);  
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,  pui4_wakeup_ctrl_cap);  
    
    RPC_DO_OP("x_pcl_get_wakeup_capability");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_GET_FAILED);
}

INT32 c_pcl_get_wakeup_setup(UINT32* pui4_wakeup_setup)
{
    RPC_DECL(1, INT32);
      
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,  pui4_wakeup_setup);  
    
    RPC_DO_OP("x_pcl_get_wakeup_setup");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_GET_FAILED);
}

INT32 c_pcl_set_wakeup_setup(UINT32 ui4_wakeup_setup)
{
    RPC_DECL(1, INT32);
      
    RPC_ARG_INP(ARG_TYPE_UINT32,  ui4_wakeup_setup);  
    
    RPC_DO_OP("x_pcl_set_wakeup_setup");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_SET_FAILED);
}

INT32 c_pcl_set_wakeup_time(TIME_T t_time)
{
    RPC_DECL(1, INT32);
      
    RPC_ARG_INP(ARG_TYPE_INT64,  t_time);  
    
    RPC_DO_OP("x_pcl_set_wakeup_time");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_SET_FAILED);
}

INT32 c_pcl_get_wakeup_time(TIME_T* pt_time)
{
    RPC_DECL(1, INT32);
      
    RPC_ARG_OUT(ARG_TYPE_REF_INT64, pt_time);  
    
    RPC_DO_OP("x_pcl_get_wakeup_time");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_GET_FAILED);
}

INT32 c_pcl_get_power_down_time(TIME_T* pt_time)
{
    RPC_DECL(1, INT32);
      
    RPC_ARG_OUT(ARG_TYPE_REF_INT64, pt_time);  
    
    RPC_DO_OP("x_pcl_get_power_down_time");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_NOT_INIT);
}

INT32 c_pcl_set_power_down_time(TIME_T t_time)
{
    RPC_DECL(1, INT32);
      
    RPC_ARG_INP(ARG_TYPE_INT64,  t_time);  
    
    RPC_DO_OP("x_pcl_set_power_down_time");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_TIMER_ERROR);
}

INT32 c_pcl_get_misc_ctrl(UINT32* pui4_ctrl)
{
    RPC_DECL(1, INT32);
      
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,  pui4_ctrl);  
    
    RPC_DO_OP("x_pcl_get_misc_ctrl");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_GET_FAILED);
}

INT32 c_pcl_set_misc_ctrl(UINT32 ui4_ctrl)
{
    RPC_DECL(1, INT32);
      
    RPC_ARG_INP(ARG_TYPE_UINT32,  ui4_ctrl);  
    
    RPC_DO_OP("x_pcl_set_misc_ctrl");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_SET_FAILED);
}

INT32 c_pcl_get_wakeup_reason(PCL_WAKE_UP_REASON_T* pe_wakeup_reason,
                              VOID*                 pv_data)
{
    RPC_DECL(2, INT32);
      
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,  pe_wakeup_reason);  
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,  pv_data);  
    
    RPC_DO_OP("x_pcl_get_wakeup_reason");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_GET_FAILED);
}

INT32 c_pcl_set_wakeup_reason(PCL_WAKE_UP_REASON_T e_wakeup_reason,
                              VOID*                pv_data)
{
    RPC_DECL(2, INT32);
      
    RPC_ARG_INP(ARG_TYPE_UINT32, e_wakeup_reason);  
    RPC_ARG_INP(ARG_TYPE_REF_UINT32, pv_data);  
    
    RPC_DO_OP("x_pcl_set_wakeup_reason");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_SET_FAILED);
}

INT32 c_pcl_get_misc_status(UINT32* pui4_status)
{
    RPC_DECL(1, INT32);
       
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pui4_status);  
    
    RPC_DO_OP("x_pcl_get_misc_status");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_GET_FAILED);
}

INT32 c_pcl_get_power_down_reason(PCL_POWER_DOWN_REASON_T* pe_power_down_reason,
                                  VOID*                    pv_data)
{
    RPC_DECL(2, INT32);
         
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_power_down_reason);  
    RPC_ARG_OUT(ARG_TYPE_REF_VOID, pv_data);  
    
    RPC_DO_OP("x_pcl_get_power_down_reason");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_GET_FAILED);
}

INT32 c_pcl_set_power_down_reason(PCL_POWER_DOWN_REASON_T e_power_down_reason,
                                  VOID*                    pv_data)
{
    RPC_DECL(2, INT32);
      
    RPC_ARG_INP(ARG_TYPE_UINT32, e_power_down_reason);  
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_data);
    
    RPC_DO_OP("x_pcl_set_power_down_reason");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_SET_FAILED);
}

INT32 c_pcl_set_power_down(VOID)
{
    RPC_DECL(0, INT32);
      
    RPC_DO_OP("x_pcl_set_power_down");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_SET_FAILED);
}

INT32 c_pcl_set_power_state(PCL_PWR_STATE_CTRL_T e_pwr_state_ctrl)
{
    RPC_DECL(1, INT32);
      
    RPC_ARG_INP(ARG_TYPE_UINT32, e_pwr_state_ctrl);  
    
    RPC_DO_OP("x_pcl_set_power_state");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_SET_FAILED);
}

INT32 c_pcl_set_reboot(VOID)
{
    RPC_DECL(0, INT32);
      
    RPC_DO_OP("x_pcl_set_reboot");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_SET_FAILED);
}

INT32 c_pcl_get_gpio(UINT32             ui4_gpio_id,
                     PCL_GPIO_MODE_T    e_gpio_mode,
                     PCL_GPIO_STATUS_T* pe_gpio_status)
{
    RPC_DECL(3, INT32);
      
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_gpio_id);  
    RPC_ARG_INP(ARG_TYPE_UINT32, e_gpio_mode); 
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_gpio_status); 
    
    RPC_DO_OP("x_pcl_get_gpio");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_GET_FAILED);
}

INT32 c_pcl_set_gpio(UINT32            ui4_gpio_id,
                     PCL_GPIO_STATUS_T e_gpio_status)
{
    RPC_DECL(2, INT32);
      
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_gpio_id);  
    RPC_ARG_INP(ARG_TYPE_UINT32, e_gpio_status); 
    
    RPC_DO_OP("x_pcl_set_gpio");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_SET_FAILED);
}

INT32 c_pcl_get_gpio_monitor_ctrl(PCL_GPIO_MONITOR_CTRL_T* pt_gpio_monitor_ctrl)
{
    RPC_DECL(1, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_gpio_monitor_ctrl, RPC_DESC_PCL_GPIO_MONITOR_CTRL_T, NULL));
      
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_gpio_monitor_ctrl);  
    
    RPC_DO_OP("x_pcl_get_gpio_monitor_ctrl");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_GET_FAILED);
}

INT32 c_pcl_set_gpio_monitor_ctrl(PCL_GPIO_MONITOR_CTRL_T* pt_gpio_monitor_ctrl)
{
    RPC_DECL(1, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_gpio_monitor_ctrl, RPC_DESC_PCL_GPIO_MONITOR_CTRL_T, NULL));
      
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_gpio_monitor_ctrl);  
    
    RPC_DO_OP("x_pcl_set_gpio_monitor_ctrl");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_SET_FAILED);
}

INT32 c_pcl_get_gpio_monitor_ctrl_ex(PCL_GPIO_MONITOR_CTRL_EX_T* pt_gpio_monitor_ctrl_ex)
{
    RPC_DECL(1, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_gpio_monitor_ctrl_ex, RPC_DESC_PCL_GPIO_MONITOR_CTRL_EX_T, NULL));
      
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_gpio_monitor_ctrl_ex);  
    
    RPC_DO_OP("x_pcl_get_gpio_monitor_ctrl_ex");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_GET_FAILED);

}

INT32 c_pcl_set_gpio_monitor_ctrl_ex(PCL_GPIO_MONITOR_CTRL_EX_T* pt_gpio_monitor_ctrl_ex)
{
    RPC_DECL(1, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_gpio_monitor_ctrl_ex, RPC_DESC_PCL_GPIO_MONITOR_CTRL_EX_T, NULL));
      
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_gpio_monitor_ctrl_ex);  
    
    RPC_DO_OP("x_pcl_set_gpio_monitor_ctrl_ex");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_SET_FAILED);
}

INT32 c_pcl_get_sif(UINT32 ui4_sif_addr,
                    UINT32 ui4_sif_offset,
                    UINT8* pui1_sif_data)
{
    RPC_DECL(3, INT32);
      
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_sif_addr);  
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_sif_offset); 
    RPC_ARG_OUT(ARG_TYPE_REF_UINT8, pui1_sif_data);
    
    RPC_DO_OP("x_pcl_get_sif");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_GET_FAILED);
}

INT32 c_pcl_set_sif(UINT32 ui4_sif_addr,
                    UINT32 ui4_sif_offset,
                    UINT8  ui1_sif_data)
{
    RPC_DECL(3, INT32);
      
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_sif_addr);  
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_sif_offset); 
    RPC_ARG_INP(ARG_TYPE_UINT8, ui1_sif_data);
    
    RPC_DO_OP("x_pcl_set_sif");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_SET_FAILED);
}

INT32 c_pcl_get_sif_advanced(PCL_SIF_INFO_T* pt_sif_info)
{
    RPC_DECL(1, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_sif_info, RPC_DESC_PCL_SIF_INFO_T, NULL));
    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_sif_info->pui1_data, pt_sif_info->ui1_data_len));
      
    RPC_ARG_IO(ARG_TYPE_REF_DESC, pt_sif_info);  
    
    RPC_DO_OP("x_pcl_get_sif_advanced");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_GET_FAILED);
}

INT32 c_pcl_set_sif_advanced(PCL_SIF_INFO_T* pt_sif_info)
{
    RPC_DECL(1, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_sif_info, RPC_DESC_PCL_SIF_INFO_T, NULL));
    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_sif_info->pui1_data, pt_sif_info->ui1_data_len));
      
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_sif_info);  
    
    RPC_DO_OP("x_pcl_set_sif_advanced");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_SET_FAILED);
}

INT32 c_pcl_get_sif_by_gpio(PCL_SIF_INFO_BY_GPIO_T* pt_sif_info_by_gpio)
{
    RPC_DECL(1, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_sif_info_by_gpio, RPC_DESC_PCL_SIF_INFO_BY_GPIO_T, NULL));
    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_sif_info_by_gpio->pui1_data, pt_sif_info_by_gpio->ui1_data_len));
      
    RPC_ARG_IO(ARG_TYPE_REF_DESC, pt_sif_info_by_gpio);  
    
    RPC_DO_OP("x_pcl_get_sif_by_gpio");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_GET_FAILED);
}

INT32 c_pcl_set_sif_by_gpio(PCL_SIF_INFO_BY_GPIO_T* pt_sif_info_by_gpio)
{
    RPC_DECL(1, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_sif_info_by_gpio, RPC_DESC_PCL_SIF_INFO_BY_GPIO_T, NULL));
    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_sif_info_by_gpio->pui1_data, pt_sif_info_by_gpio->ui1_data_len));
      
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_sif_info_by_gpio);  
    
    RPC_DO_OP("x_pcl_set_sif_by_gpio");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_SET_FAILED);
}

INT32 c_pcl_get_upgrade_ctrl(UINT32* pui4_ctrl)
{
    RPC_DECL(1, INT32);
      
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pui4_ctrl);
    
    RPC_DO_OP("x_pcl_get_upgrade_ctrl");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_GET_FAILED);
}

INT32 c_pcl_set_upgrade_ctrl(UINT32 ui4_ctrl)
{
    RPC_DECL(1, INT32);
      
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_ctrl);
    
    RPC_DO_OP("x_pcl_set_upgrade_ctrl");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_SET_FAILED);
}

INT32 c_pcl_get_servo_ad(PCL_SERVO_AD_INFO_T* pt_servo_ad_info)
{
    RPC_DECL(1, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_servo_ad_info, RPC_DESC_PCL_SERVO_AD_INFO_T, NULL));
      
    RPC_ARG_IO(ARG_TYPE_REF_DESC, pt_servo_ad_info);  
    
    RPC_DO_OP("x_pcl_get_servo_ad");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_GET_FAILED);
}

INT32 _x_pcl_reg_nfy_fct(VOID*         pv_tag,
                        x_pcl_nfy_fct pf_nfy,
                        HANDLE_T*     ph_nfy)
{
    RPC_DECL(3, INT32);
  
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_tag); 
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_nfy);
    
    RPC_DO_OP("x_pcl_reg_nfy_fct");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_HANDLE_LIB_ERROR);
}
INT32 c_pcl_reg_nfy_fct(VOID*         pv_tag,
                        x_pcl_nfy_fct pf_nfy,
                        HANDLE_T*     ph_nfy)
{
    return _x_pcl_reg_nfy_fct(pv_tag, pf_nfy, ph_nfy);
}

INT32 c_pcl_dereg_nfy_fct(HANDLE_T h_nfy)
{
    RPC_DECL(1, INT32);
  
    RPC_ARG_INP(ARG_TYPE_UINT32, h_nfy);
    
    RPC_DO_OP("x_pcl_dereg_nfy_fct");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_HANDLE_LIB_ERROR);
}

INT32 c_pcl_add_wakeup_event(PCL_WAKE_UP_CTRL_T* pt_wakeup_ctrl,
                             HANDLE_T*           ph_wakeup_evt)
{
    RPC_DECL(2, INT32);
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_wakeup_ctrl, RPC_DESC_PCL_WAKE_UP_CTRL_T, NULL));

    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_wakeup_ctrl);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_wakeup_evt);
    
    RPC_DO_OP("x_pcl_add_wakeup_event");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_HANDLE_LIB_ERROR);
}

INT32 c_pcl_remove_wakeup_event(HANDLE_T h_wakeup_evt)
{
    RPC_DECL(1, INT32);
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_wakeup_evt);
    
    RPC_DO_OP("x_pcl_remove_wakeup_event");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_HANDLE_LIB_ERROR);
}

INT32 c_pcl_get_wakeup_event(HANDLE_T            h_wakeup_evt,
                             PCL_WAKE_UP_CTRL_T* pt_wakeup_ctrl)
{
    RPC_DECL(2, INT32);
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_wakeup_ctrl, RPC_DESC_PCL_WAKE_UP_CTRL_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32, h_wakeup_evt);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_wakeup_ctrl);
    
    RPC_DO_OP("x_pcl_get_wakeup_event");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_GET_FAILED);
}

INT32 c_pcl_set_wakeup_event(HANDLE_T            h_wakeup_evt,
                             PCL_WAKE_UP_CTRL_T* pt_wakeup_ctrl)
{
    RPC_DECL(2, INT32);
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_wakeup_ctrl, RPC_DESC_PCL_WAKE_UP_CTRL_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32, h_wakeup_evt);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_wakeup_ctrl);
    
    RPC_DO_OP("x_pcl_set_wakeup_event");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_DEV_SET_FAILED);
}

INT32 c_pcl_get_res_count(PCL_RES_TYPE_T e_res_type,
                          UINT16*        pui2_count)
{
    RPC_DECL(2, INT32);
    
    RPC_ARG_INP(ARG_TYPE_UINT32, e_res_type);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16, pui2_count);
    
    RPC_DO_OP("x_pcl_get_res_count");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_INV_ARG);
}

INT32 c_pcl_set_res_count(PCL_RES_TYPE_T e_res_type,
                          BOOL           b_increase)
{
    RPC_DECL(2, INT32);
    
    RPC_ARG_INP(ARG_TYPE_UINT32, e_res_type);
    RPC_ARG_INP(ARG_TYPE_BOOL, b_increase);
    
    RPC_DO_OP("x_pcl_set_res_count");
    
    RPC_RETURN(ARG_TYPE_INT32, PCLR_INV_ARG);
}

INT32 c_pcl_get_dual_bank_index(PCL_DUAL_BANK_TYPE_T e_dual_bank,
                                UINT8 * pui1_index)
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, e_dual_bank);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT8, pui1_index);

    RPC_DO_OP("x_pcl_get_dual_bank_index");

    RPC_RETURN(ARG_TYPE_INT32, PCLR_INV_ARG);
}

INT32 c_pcl_set_dual_bank_index(PCL_DUAL_BANK_TYPE_T e_dual_bank,
                                UINT8 ui1_index)
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, e_dual_bank);
    RPC_ARG_INP(ARG_TYPE_UINT8, ui1_index);

    RPC_DO_OP("x_pcl_set_dual_bank_index");

    RPC_RETURN(ARG_TYPE_INT32, PCLR_INV_ARG);
}

INT32 c_pcl_toggle_dual_bank_index(PCL_DUAL_BANK_TYPE_T e_dual_bank)
{
    RPC_DECL(1, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, e_dual_bank);

    RPC_DO_OP("x_pcl_toggle_dual_bank_index");

    RPC_RETURN(ARG_TYPE_INT32, PCLR_INV_ARG);
}

INT32 c_rpc_reg_pcl_cb_hndlrs()
{
    RPC_REG_CB_HNDLR(x_pcl_nfy_fct);    
    return RPCR_OK;
}
