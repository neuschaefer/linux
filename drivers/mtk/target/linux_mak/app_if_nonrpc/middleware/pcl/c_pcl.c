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
 * $RCSfile: c_pcl.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: Alec Lu $
 * $MD5HEX: 53b33c1d8f89b3f01defdea438e4415a $
 *
 * Description:
 *         This is Power Control Library API source file, which are exported
 *         to applications.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "handle/x_handle.h"
#include "pcl/x_pcl.h"

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
 * Name: c_pcl_get_wakeup_capability
 *
 * Description: This API returns the wake-up capability info that the system
 *              can support and clients can control.
 *
 * Inputs:  -
 *
 * Outputs: pui4_wakeup_cap         References the bit-mask of all wake-up
 *                                  capabilities that the system can support.
 *          pui4_wakeup_ctrl_cap    References the bit-mask of the wake-up
 *                                  capabilities that clients can control.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_get_wakeup_capability(UINT32* pui4_wakeup_cap,
                                  UINT32* pui4_wakeup_ctrl_cap)
{
    return (x_pcl_get_wakeup_capability(pui4_wakeup_cap,
                                        pui4_wakeup_ctrl_cap));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_get_wakeup_setup
 *
 * Description: This API gets the current wake-up setup.
 *
 * Inputs:  -
 *
 * Outputs: pui4_wakeup_setup	    References the wake-up setup in bit-mask
 *                                  fashion.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_get_wakeup_setup(UINT32* pui4_wakeup_setup)
{
    return (x_pcl_get_wakeup_setup(pui4_wakeup_setup));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_set_wakeup_setup
 *
 * Description: This API sets the wake-up setup.
 *              Note that PCL_WAKE_UP_REASON_IRRC and PCL_WAKE_UP_REASON_FP
 *              are always set and cannot be disabled by clients.
 *
 * Inputs:  ui4_wakeup_setup        Contains the wake-up setup in bit-mask
 *                                  fashion.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_set_wakeup_setup(UINT32 ui4_wakeup_setup)
{
    return (x_pcl_set_wakeup_setup(ui4_wakeup_setup));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_get_wakeup_time
 *
 * Description: This API gets the wake-up time configured by
 *              x_pcl_set_wakeup_time().
 *
 * Inputs:  -
 *
 * Outputs: pt_time         References the wake-up time (UTC time count as
 *                          number of seconds since 00:00:00Z January 1, 1970).
 *                          If the wake-up time was never set, NULL_TIME will
 *                          be returned.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_ERROR              A Power Control Library error occurred.
 *          PCLR_HANDLE_LIB_ERROR   A handle library error occurred.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_get_wakeup_time(TIME_T* pt_time)
{
    return (x_pcl_get_wakeup_time(pt_time));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_set_wakeup_time
 *
 * Description: This API sets a wake-up time.
 *
 * Inputs:  t_time          Contains the wake-up time (UTC time count as
 *                          number of seconds since 00:00:00Z January 1, 1970).
 *                          If a client would like to cancel the wake-up time,
 *                          it simply calls this API with ui4_time set to
 *                          NULL_TIME.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_MEM_ALLOC_FAILED   Memory allocation is failed.
 *          PCLR_HANDLE_LIB_ERROR   A handle library error occurred.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_set_wakeup_time(TIME_T t_time)
{
    return (x_pcl_set_wakeup_time(t_time));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_get_power_down_time
 *
 * Description: This API gets the current power-down time.
 *
 * Inputs:  -
 *
 * Outputs: pt_time         References the power-down time (UTC time count as
 *                          number of seconds since 00:00:00Z January 1, 1970).
 *                          If the wake-up time was never set, NULL_TIME will
 *                          be returned.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_get_power_down_time(TIME_T* pt_time)
{
    return (x_pcl_get_power_down_time(pt_time));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_set_power_down_time
 *
 * Description: This API sets the power-down time. If the time is already
 *              passed, this API will reject with PCLR_INV_ARG.
 *
 * Inputs:  t_time          Contains the power-down time (UTC time count as
 *                          number of seconds since 00:00:00Z January 1, 1970).
 *                          If a client would like to cancel the power-down
 *                          time, it simply calls this API with ui4_time set
 *                          to NULL_TIME.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_TIMER_ERROR        A timer error occurred.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_set_power_down_time(TIME_T t_time)
{
    return (x_pcl_set_power_down_time(t_time));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_get_misc_ctrl
 *
 * Description: This API gets the status of miscellaneous controls.
 *
 * Inputs:  -
 *
 * Outputs: pui4_ctrl       References the bit-mask of miscellaneous controls.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_get_misc_ctrl(UINT32* pui4_ctrl)
{
    return (x_pcl_get_misc_ctrl(pui4_ctrl));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_set_misc_ctrl
 *
 * Description: This API sets the miscellaneous controls.
 *
 * Inputs:  ui4_ctrl        Contains the bit-mask of miscellaneous controls.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_set_misc_ctrl(UINT32 ui4_ctrl)
{
    return (x_pcl_set_misc_ctrl(ui4_ctrl));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_get_wakeup_reason
 *
 * Description: This API gets the reason of current wake-up.
 *
 * Inputs:  -
 *
 * Outputs: pe_wakeup_reason        References the wake-up reason.
 *          pv_data                 References the additional information of
 *                                  wakeup reason.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_get_wakeup_reason(PCL_WAKE_UP_REASON_T* pe_wakeup_reason,
                              VOID*                 pv_data)
{
    return (x_pcl_get_wakeup_reason(pe_wakeup_reason,
                                    pv_data));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_set_wakeup_reason
 *
 * Description: This API sets the reason of current wake-up.
 *
 * Inputs:  e_wakeup_reason         Contains the wake-up reason.
 *          pv_data                 References the additional information of
 *                                  wakeup reason.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_SET_DEV_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_set_wakeup_reason(PCL_WAKE_UP_REASON_T e_wakeup_reason,
                              VOID*                pv_data)
{
    return x_pcl_set_wakeup_reason(e_wakeup_reason,
                                   pv_data);
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_get_misc_status
 *
 * Description: This API gets the Power Control driver miscellaneous status.
 *
 * Inputs:  -
 *
 * Outputs: pui4_status       References the bit-mask of miscellaneous status.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_get_misc_status(UINT32* pui4_status)
{
    return (x_pcl_get_misc_status(pui4_status));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_get_power_down_reason
 *
 * Description: This API gets the power-down reason.
 *
 * Inputs:  -
 *
 * Outputs: pe_power_down_reason    References the power-down reason.
 *
 *          pv_data                 References the additional information of
 *                                  power-down reason.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_get_power_down_reason(PCL_POWER_DOWN_REASON_T* pe_power_down_reason,
                                  VOID*                    pv_data)
{
    return (x_pcl_get_power_down_reason(pe_power_down_reason,
                                        pv_data));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_set_power_down_reason
 *
 * Description: This API sets the power-down reason.
 *
 * Inputs:  e_power_down_reason     Contains the power-down reason.
 *          pv_data               - References the additional information of
 *                                       wakeup reason.
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_set_power_down_reason(PCL_POWER_DOWN_REASON_T e_power_down_reason,
                                  VOID*                   pv_data)
{
    return (x_pcl_set_power_down_reason(e_power_down_reason, pv_data));
}

/*-----------------------------------------------------------------------------
 * Name: c_pcl_set_power_down
 *
 * Description: This API sets a wake-up time into driver if available and then
 *              turns the system into power-down mode.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_set_power_down(VOID)
{
    return (x_pcl_set_power_down());
}

/*-----------------------------------------------------------------------------
 * Name: c_pcl_get_power_state
 *
 * Description: This API gets the system power state.
 *
 * Inputs:  None        .
 *
 * Outputs: pe_pwr_state_ctrl -     Contains power state setting
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_get_power_state(PCL_PWR_STATE_CTRL_T* pe_pwr_state_ctrl)
{
    return (x_pcl_get_power_state(pe_pwr_state_ctrl));
}

/*-----------------------------------------------------------------------------
 * Name: c_pcl_set_power_state
 *
 * Description: This API changes the system power state.
 *
 * Inputs:  e_pwr_state_ctrl        Contains power state setting.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_set_power_state(PCL_PWR_STATE_CTRL_T e_pwr_state_ctrl)
{
    return (x_pcl_set_power_state(e_pwr_state_ctrl));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_set_reboot
 *
 * Description: This API reboots the system.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_set_reboot(VOID)
{
    return (x_pcl_set_reboot());
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_get_gpio
 *
 * Description: This API gets the status of specific GPIO pin (for either input
 *              or output mode).
 *
 * Inputs:  ui4_gpio_id             Contains the GPIO ID.
 *          e_gpio_mode             Contains the GPIO mode.
 *
 * Outputs: pe_gpio_status          References the GPIO status.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_get_gpio(UINT32             ui4_gpio_id,
                     PCL_GPIO_MODE_T    e_gpio_mode,
                     PCL_GPIO_STATUS_T* pe_gpio_status)
{
    return (x_pcl_get_gpio(ui4_gpio_id, e_gpio_mode, pe_gpio_status));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_set_gpio
 *
 * Description: This API changes the status of specific GPIO pin output (output
 *              mode only).
 *
 * Inputs:  ui4_gpio_id         Contains the GPIO ID.
 *          e_gpio_status       Contains the GPIO status.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_set_gpio(UINT32            ui4_gpio_id,
                     PCL_GPIO_STATUS_T e_gpio_status)
{
    return (x_pcl_set_gpio(ui4_gpio_id, e_gpio_status));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_get_gpio_monitor_ctrl
 *
 * Description: This API gets GPIO/OPCTRL monitoring control.
 *
 * Inputs:  -
 *
 * Outputs: pt_gpio_monitor_ctrl    References GPIO monitoring control info.
 *                                  [55:0] => GPIO 0~55 monitoring control.
 *                                  [63:56] => OPCTRL 0~7 monitoring control.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_get_gpio_monitor_ctrl(PCL_GPIO_MONITOR_CTRL_T* pt_gpio_monitor_ctrl)
{
    return (x_pcl_get_gpio_monitor_ctrl(pt_gpio_monitor_ctrl));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_set_gpio_monitor_ctrl
 *
 * Description: This API sets GPIO/OPCTRL monitoring control.
 *
 * Inputs:  pt_gpio_monitor_ctrl    References GPIO monitoring control info.
 *                                  [55:0] => GPIO 0~55 monitoring control.
 *                                  [63:56] => OPCTRL 0~7 monitoring control.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_set_gpio_monitor_ctrl(PCL_GPIO_MONITOR_CTRL_T* pt_gpio_monitor_ctrl)
{
    return (x_pcl_set_gpio_monitor_ctrl(pt_gpio_monitor_ctrl));
}
/*-----------------------------------------------------------------------------
 * Name: c_pcl_get_gpio_monitor_ctrl_ex
 *
 * Description: This API gets GPIO/OPCTRL monitoring control.
 *
 * Inputs:  -
 *
 * Outputs: pt_gpio_monitor_ctrl_ex    References GPIO monitoring control info.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_get_gpio_monitor_ctrl_ex(PCL_GPIO_MONITOR_CTRL_EX_T* pt_gpio_monitor_ctrl_ex)
{
    return (x_pcl_get_gpio_monitor_ctrl_ex(pt_gpio_monitor_ctrl_ex));
}

/*-----------------------------------------------------------------------------
 * Name: c_pcl_set_gpio_monitor_ctrl_ex
 *
 * Description: This API sets GPIO/OPCTRL monitoring control.
 *
 * Inputs:  pt_gpio_monitor_ctrl_ex    References GPIO monitoring control info.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_set_gpio_monitor_ctrl_ex(PCL_GPIO_MONITOR_CTRL_EX_T* pt_gpio_monitor_ctrl_ex)
{
    return (x_pcl_set_gpio_monitor_ctrl_ex(pt_gpio_monitor_ctrl_ex));
}

/*-----------------------------------------------------------------------------
 * Name: c_pcl_get_sif
 *
 * Description: This API gets the data from a SIF address.
 *
 * Inputs:  ui4_sif_addr            Contains the SIF address.
 *          ui4_sif_offset          Contains the SIF offset.
 *
 * Outputs: pui1_sif_data	        References the SIF data.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_get_sif(UINT32 ui4_sif_addr,
                    UINT32 ui4_sif_offset,
                    UINT8* pui1_sif_data)
{
    return (x_pcl_get_sif(ui4_sif_addr, ui4_sif_offset, pui1_sif_data));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_set_sif
 *
 * Description: This API sets the data to a SIF address.
 *
 * Inputs:  ui4_sif_addr            Contains the SIF address.
 *          ui4_sif_offset          Contains the SIF offset.
 *          ui1_sif_data	        Contains the SIF data.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_set_sif(UINT32 ui4_sif_addr,
                    UINT32 ui4_sif_offset,
                    UINT8  ui1_sif_data)
{
    return (x_pcl_set_sif(ui4_sif_addr, ui4_sif_offset, ui1_sif_data));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_get_sif_advanced
 *
 * Description: This API gets the data from a SIF address (advanced operation).
 *
 * Inputs:  pt_sif_info             References the SIF info.
 *
 * Outputs: pt_sif_info             References the SIF info.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_get_sif_advanced(PCL_SIF_INFO_T* pt_sif_info)
{
    return (x_pcl_get_sif_advanced(pt_sif_info));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_set_sif_advanced
 *
 * Description: This API sets the data to a SIF address (advanced operation).
 *
 * Inputs:  pt_sif_info             References the SIF info.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_set_sif_advanced(PCL_SIF_INFO_T* pt_sif_info)
{
    return (x_pcl_set_sif_advanced(pt_sif_info));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_get_sif_by_gpio
 *
 * Description: This API gets the data of specific SIF address via GPIO pins.
 *
 * Inputs:  pt_sif_info_by_gpio     References the SIF info.
 *
 * Outputs: pt_sif_info_by_gpio     References the SIF info.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_get_sif_by_gpio(PCL_SIF_INFO_BY_GPIO_T* pt_sif_info_by_gpio)
{
    return (x_pcl_get_sif_by_gpio(pt_sif_info_by_gpio));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_set_sif_by_gpio
 *
 * Description: This API sets the data to specific SIF address via GPIO pins.
 *
 * Inputs:  pt_sif_info_by_gpio     References the SIF info.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_set_sif_by_gpio(PCL_SIF_INFO_BY_GPIO_T* pt_sif_info_by_gpio)
{
    return (x_pcl_set_sif_by_gpio(pt_sif_info_by_gpio));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_get_upgrade_ctrl
 *
 * Description: This API gets the status of upgrade controls.
 *
 * Inputs:  -
 *
 * Outputs: pui4_ctrl       References the bit-mask of upgrade controls.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_get_upgrade_ctrl(UINT32* pui4_ctrl)
{
    return (x_pcl_get_upgrade_ctrl(pui4_ctrl));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_set_upgrade_ctrl
 *
 * Description: This API sets the upgrade controls.
 *
 * Inputs:  ui4_ctrl        Contains the bit-mask of upgrade controls.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_set_upgrade_ctrl(UINT32 ui4_ctrl)
{
    return (x_pcl_set_upgrade_ctrl(ui4_ctrl));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_get_servo_ad
 *
 * Description: This API gets the status of specific servo AD.
 *
 * Inputs:  pt_servo_ad_info        References the servo AD info.
 *
 * Outputs: pt_servo_ad_info        References the servo AD info.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_get_servo_ad(PCL_SERVO_AD_INFO_T* pt_servo_ad_info)
{
    return (x_pcl_get_servo_ad(pt_servo_ad_info));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_reg_nfy_fct
 *
 * Description: This API registers a callback function with the Power Control
 *              Library in order to receive condition changes.
 *
 * Inputs:  pv_tag          Private tag value which must be returned in the
 *                          notify function. It can be set to NULL.
 *          pf_nfy          References the caller's notify function.
 *
 * Outputs: ph_nfy          A handle referencing a notification record.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_MEM_ALLOC_FAILED   Memory allocation is failed.
 *            PCLR_HANDLE_LIB_ERROR   A handle library error occurred.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_reg_nfy_fct(VOID*         pv_tag,
                        x_pcl_nfy_fct pf_nfy,
                        HANDLE_T*     ph_nfy)
{
    return (x_pcl_reg_nfy_fct(pv_tag,
                              pf_nfy,
                              ph_nfy));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_dereg_nfy_fct
 *
 * Description: This API de-registers a callback function from the Power
 *              Control Library.
 *
 * Inputs:  h_nfy       A handle referencing a notification record.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HANDLE_LIB_ERROR   A handle library error occurred.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_dereg_nfy_fct(HANDLE_T h_nfy)
{
	return (x_pcl_dereg_nfy_fct(h_nfy));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_add_wakeup_event
 *
 * Description: This API adds a wake-up event into wake-up list. Note that the
 *              list is sorted by wake-up time.
 *
 * Inputs:  pt_wakeup_ctrl      References wake-up control info.
 *
 * Outputs: ph_wakeup_evt       A handle referencing the newly added wake-up
 *                              event.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_MEM_ALLOC_FAILED   Memory allocation is failed.
 *          PCLR_HANDLE_LIB_ERROR   A handle library error occurred.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_add_wakeup_event(PCL_WAKE_UP_CTRL_T* pt_wakeup_ctrl,
                             HANDLE_T*           ph_wakeup_evt)
{
    return (x_pcl_add_wakeup_event(pt_wakeup_ctrl,
                                   ph_wakeup_evt));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_remove_wakeup_event
 *
 * Description: This API removes a wake-up event from wake-up list.
 *
 * Inputs:  h_wakeup_evt        A handle referencing a wake-up event.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HANDLE_LIB_ERROR   A handle library error occurred.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_remove_wakeup_event(HANDLE_T h_wakeup_evt)
{
    return (x_pcl_remove_wakeup_event(h_wakeup_evt));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_get_wakeup_event
 *
 * Description: This API obtains the wake-up control info of a wake-up event.
 *
 * Inputs:  h_wakeup_evt        A handle referencing a wake-up event.
 *
 * Outputs: pt_wakeup_ctrl      References wake-up control info.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 *          PCLR_HANDLE_LIB_ERROR   A handle library error occurred.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_get_wakeup_event(HANDLE_T            h_wakeup_evt,
                             PCL_WAKE_UP_CTRL_T* pt_wakeup_ctrl)
{
    return (x_pcl_get_wakeup_event(h_wakeup_evt,
                                   pt_wakeup_ctrl));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_set_wakeup_event
 *
 * Description: This API changes the control info of a existing wake-up event
 *              in wake-up list.
 *
 * Inputs:  h_wakeup_evt        A handle referencing a wake-up event.
 *          pt_wakeup_ctrl      References wake-up control info.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_ERROR              Operation error.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 *          PCLR_HANDLE_LIB_ERROR   A handle library error occurred.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_set_wakeup_event(HANDLE_T            h_wakeup_evt,
                             PCL_WAKE_UP_CTRL_T* pt_wakeup_ctrl)
{
    return (x_pcl_set_wakeup_event(h_wakeup_evt,
                                   pt_wakeup_ctrl));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_get_res_count
 *
 * Description: This API obtains the count of a specific resource.
 *
 * Inputs:  e_res_type          Contains the resource type.
 *
 * Outputs: pui2_count          References a count of the resource.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_get_res_count(PCL_RES_TYPE_T e_res_type,
                          UINT16*        pui2_count)
{
    return (x_pcl_get_res_count(e_res_type,
                                pui2_count));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_set_res_count
 *
 * Description: This API increase the count of a specific resource.
 *
 * Inputs:  e_res_type          Contains the resource type.
 *          b_increase          Indicates the counter should be increased or
 *                              decreased by one.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_HANDLE_LIB_ERROR   A handle library error occurred.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_set_res_count(PCL_RES_TYPE_T e_res_type,
                          BOOL           b_increase)
{
    return (x_pcl_set_res_count(e_res_type,
                                b_increase));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_get_dual_bank_index
 *
 * Description: This API gets the dual bank index for the specified dual bank
 *              type.
 *
 * Inputs:  e_dual_bank             Type of the dual bank index.
 *
 * Outputs: pui1_index              References the dual bank index (0/1).
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_get_dual_bank_index(PCL_DUAL_BANK_TYPE_T    e_dual_bank,
                                UINT8 *                 pui1_index)
{
    return (x_pcl_get_dual_bank_index(e_dual_bank, pui1_index));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_set_dual_bank_index
 *
 * Description: This API sets the dual bank index for the specified dual bank
 *              type.
 *
 * Inputs:  e_dual_bank             Type of the dual bank index.
 *          ui1_index               Dual bank index value (0/1).
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_set_dual_bank_index(PCL_DUAL_BANK_TYPE_T    e_dual_bank,
                                UINT8                   ui1_index)
{
    return (x_pcl_set_dual_bank_index(e_dual_bank, ui1_index));
}


/*-----------------------------------------------------------------------------
 * Name: c_pcl_toggle_dual_bank_index
 *
 * Description: This API toggles the dual bank index for the specified dual bank
 *              type, changing from 0 to 1 or 1 to 0.
 *
 * Inputs:  e_dual_bank             Type of the dual bank index.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_toggle_dual_bank_index(PCL_DUAL_BANK_TYPE_T    e_dual_bank)
{
    return (x_pcl_toggle_dual_bank_index(e_dual_bank));
}

#ifdef MW_FAST_INIT
/*-----------------------------------------------------------------------------
 * Name: c_pcl_get_boot_type
 *
  * Description: This API gets the system boot type.
 *
 * Inputs:  None             
 *
 * Outputs: pe_boot_type            References the boot type which must be one of 
 *                                  (PCL_MISC_CTRL_FAST_BOOT, PCL_MISC_CTRL_NORMAL_BOOT_WITH_SUSPEND,
 *                                  PCL_MISC_CTRL_NORMAL_BOOT_WITHOUT_SUSPEND).
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_get_boot_type(PCL_BOOT_TYPE_T    *pe_boot_type)
{
    return (x_pcl_get_boot_type(pe_boot_type));
}

/*-----------------------------------------------------------------------------
 * Name: c_pcl_set_boot_type
 *
 * Description: This API sets the system boot type.
 *
 * Inputs:  e_boot_type             References the boot type. Only one of 
 *                                  (PCL_MISC_CTRL_FAST_BOOT, PCL_MISC_CTRL_NORMAL_BOOT_WITH_SUSPEND,
 *                                  PCL_MISC_CTRL_NORMAL_BOOT_WITHOUT_SUSPEND) can be set.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_set_boot_type(PCL_BOOT_TYPE_T    e_boot_type)
{
    return (x_pcl_set_boot_type(e_boot_type));
}

/*-----------------------------------------------------------------------------
 * Name: c_pcl_set_power_suspend
 *
 * Description: This API sets the power suspend.
 *
 * Inputs:       
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 c_pcl_set_power_suspend(VOID)
{
    return (x_pcl_set_power_suspend());
}
#endif
