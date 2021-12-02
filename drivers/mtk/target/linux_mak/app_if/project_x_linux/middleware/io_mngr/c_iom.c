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
 * $RCSfile: c_iom.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/4 $
 * $SWAuthor: Alec Lu $
 * $MD5HEX: 53b33c1d8f89b3f01defdea438e4415a $
 *
 * Description:
 *         This is I/O Manager API source file, which are exported to
 *         applications.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "aee/aee.h"
#include "handle/handle.h"
#include "io_mngr/_iom.h"
#include "io_mngr/x_iom.h"

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
 * Name: c_iom_open
 *
 * Description: This API opens a device whose attribute can be input, output,
 *              or both. An application is allowed to do multiple opens to a
 *              device simultaneously.
 *
 * Inputs:  e_dev_type      The device type.
 *          ui2_id          The device id.
 *          ps_name         The device name.
 *          pv_setting      The setting to the device if required.
 *          pv_tag          Private tag value which must be returned in the
 *                          notify function. It can be set to NULL.
 *          pf_nfy          References the caller's notify function.
 *
 * Outputs: ph_dev          A handle referencing the opened device.
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_NOT_INIT           The IO Manager is not initialized.
 *          IOMR_INV_ARG            One or more of the arguments contain
 *                                  invalid data. 
 ----------------------------------------------------------------------------*/
INT32 c_iom_open(IOM_DEV_TYPE_T e_dev_type,
		         UINT16         ui2_id,
                 const CHAR*    ps_name,
                 VOID*          pv_setting,
                 VOID*          pv_tag,
                 x_iom_nfy_fct  pf_nfy,
                 HANDLE_T*      ph_dev)
{
    HANDLE_T    h_aux;
    INT32       i4_return;

    i4_return = aee_grab_handle_resource(AEE_FLAG,
                                         1,
                                         &h_aux);

    if (i4_return == AEER_OK)
    {
        i4_return = x_iom_open(e_dev_type,
		                       ui2_id,
                               ps_name,
                               pv_setting,
                               pv_tag,
                               pf_nfy,
                               ph_dev);

        if (i4_return == IOMR_OK)
        {
            handle_link_to_aux(h_aux, *ph_dev);
        }
        else
        {
            aee_release_handle_resource(1);
        }
    }

    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: c_iom_close
 *
 * Description: This API closes an opened device referenced by h_dev.
 *
 * Inputs:  h_dev   A handle referencing the device.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_NOT_INIT           The IO Manager is not initialized.
 *          IOMR_INV_HANDLE         The handle is invalid.
 ----------------------------------------------------------------------------*/
INT32 c_iom_close(HANDLE_T h_dev)
{
    return (x_iom_close(h_dev));
}


/*-----------------------------------------------------------------------------
 * Name: c_iom_get
 *
 * Description: This API gets some info from a device referenced by h_dev.
 *
 * Inputs:  h_dev               A handle referencing the device.
 *          e_get_type          Specifies the type of information which shall
 *                              be returned in the buffer, referenced by
 *                              argument pv_get_info.
 *          pz_get_info_len     Contains the length of the buffer which is
 *                              referenced by argument pv_get_info.
 *
 * Outputs: pv_get_info         Contains the retrieved information.
 *          pz_get_info_len     Indicates the length of the valid data in the
 *                              buffer referenced by argument pv_get_info.
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_NOT_INIT           The IO Manager is not initialized.
 *          IOMR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          IOMR_INV_HANDLE         The handle is invalid. 
 ----------------------------------------------------------------------------*/
INT32 c_iom_get(HANDLE_T  h_dev,
                IOM_GET_T e_get_type,
                VOID*     pv_get_info,
                SIZE_T*   pz_get_info_len)
{
    return (x_iom_get(h_dev,
                      e_get_type,
                      pv_get_info,  
                      pz_get_info_len));
}


/*-----------------------------------------------------------------------------
 * Name: c_iom_set
 *
 * Description: This API sets some info to a device referenced by h_dev.
 *
 * Inputs:  h_dev           A handle referencing the device.
 *          e_set_type      Specifies the type of information which is
 *                          referenced by argument pv_set_info.
 *          pv_set_info     Contains the set information.
 *          z_set_info_len  Indicates the length of the data in the buffer
 *                          referenced by argument pv_set_info.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_NOT_INIT           The IO Manager is not initialized.
 *          IOMR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          IOMR_INV_HANDLE         The handle is invalid.
 ----------------------------------------------------------------------------*/
INT32 c_iom_set(HANDLE_T    h_dev,
                IOM_SET_T   e_set_type,
                const VOID* pv_set_info,
                SIZE_T      z_set_info_len)
{
    return (x_iom_set(h_dev,
                      e_set_type,
                      pv_set_info,
                      z_set_info_len));
}


/*-----------------------------------------------------------------------------
 * Name: c_iom_output
 *
 * Description: This API sends an event to an output or IO driver.
 *
 * Inputs:  h_dev           A handle referencing the device.
 *          pv_otp_info     The output event information.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_NOT_INIT           The IO Manager is not initialized.
 *          IOMR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          IOMR_DEV_BUSY           Device is busy.
 *          IOMR_INV_HANDLE         The handle is invalid.
 ----------------------------------------------------------------------------*/
INT32 c_iom_output(HANDLE_T h_dev,
                   VOID*    pv_otp_info)
{                   
    return (x_iom_output(h_dev,
                         pv_otp_info));
}


/*-----------------------------------------------------------------------------
 * Name: c_iom_send_evt
 *
 * Description: This API is called in order to generate a pair of button-down
 *              and button-up events.
 *
 * Inputs:  ui4_evt_code    Contains the event code.
 *          ui4_itvl        Contains the interval between button-down and
 *                          button-up events in ms.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_MSGQ_FULL          Message Queue is full.
 ----------------------------------------------------------------------------*/
INT32 c_iom_send_evt(UINT32 ui4_evt_code,
                     UINT32 ui4_itvl)
{
    return (x_iom_send_evt(ui4_evt_code, ui4_itvl));
}


/*-----------------------------------------------------------------------------
 * Name: c_iom_send_evt_single
 *
 * Description: This API is called to generate either a button-down or a
 *              button-up event.
 *
 * Inputs:  ui4_evt_code    Contains the event code.
 *          b_btn_down      Contains the interval between button-down and 
 *                          button-up events in ms.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_NOT_INIT           The IO Manager is not initialized. 
 *          IOMR_MSGQ_FULL          Message Queue is full.
 ----------------------------------------------------------------------------*/
INT32 c_iom_send_evt_single(UINT32 ui4_evt_code,
                            BOOL   b_btn_down)
{
    return (x_iom_send_evt_single(ui4_evt_code, b_btn_down));
}


/*-----------------------------------------------------------------------------
 * Name: c_iom_send_raw_data
 *
 * Description: This API sends a raw data event to IO Manager.
 *
 * Inputs:  ui4_raw_data    Contains the raw data.
 *          ui4_itvl        Contains the interval between button-down and 
 *                          button-up events in ms.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_NOT_INIT           The IO Manager is not initialized. 
 *          IOMR_MSGQ_FULL          Message Queue is full.
 ----------------------------------------------------------------------------*/
INT32 c_iom_send_raw_data(UINT32 ui4_raw_data,
                          UINT32 ui4_itvl)
{
    return (x_iom_send_raw_data(ui4_raw_data, ui4_itvl));
}


/*-----------------------------------------------------------------------------
 * Name: c_iom_convert_raw_data_to_evt_code
 *
 * Description: This API converted IRRC raw data to an event code.
 *
 * Inputs:  h_dev           A handle referencing the device.
 *          pt_data         Contains the raw date to be converted.
 *
 * Outputs: pt_data         Contains the event code after conversion.
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_NOT_INIT           The IO Manager is not initialized. 
 *          IOMR_INV_HANDLE         The handle is invalid.
 *          IOMR_DEV_SET_FAILED     Device get operation is failed. 
 ----------------------------------------------------------------------------*/
INT32 c_iom_convert_raw_data_to_evt_code(HANDLE_T                    h_dev,
                                         IOM_RAW_DATA_TO_EVT_CODE_T* pt_data)
{
    return (x_iom_convert_raw_data_to_evt_code(h_dev, pt_data));
}


/*-----------------------------------------------------------------------------
 * Name: c_iom_convert_raw_bits_to_evt_code
 *
 * Description: This API converted IRRC raw bits to an event code.
 *
 * Inputs:  h_dev           A handle referencing the device.
 *          pt_data         Contains the raw bits to be converted.
 *
 * Outputs: pt_data         Contains the event code after conversion.
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_NOT_INIT           The IO Manager is not initialized. 
 *          IOMR_INV_HANDLE         The handle is invalid.
 *          IOMR_DEV_SET_FAILED     Device get operation is failed. 
 ----------------------------------------------------------------------------*/
INT32 c_iom_convert_raw_bits_to_evt_code(HANDLE_T                    h_dev,
                                         IOM_RAW_BITS_TO_EVT_CODE_T* pt_data)
{
    return (x_iom_convert_raw_bits_to_evt_code(h_dev, pt_data));
}


#if !IOM_REMOVE_APDEMON_IRRC
/*-----------------------------------------------------------------------------
 * Name: c_iom_apdemon_send_evt
 *
 * Description: This API is called by Apdemon in response to received IRRC
 *              events from ATV Manager.
 *
 * Inputs:  ui4_evt_code    The event code.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_NOT_INIT           The IO Manager is not initialized.
 *          IOMR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 ----------------------------------------------------------------------------*/
INT32 c_iom_apdemon_send_evt(UINT32 ui4_evt_code)
{
    return (x_iom_apdemon_send_evt(ui4_evt_code));
}
#endif


/*-----------------------------------------------------------------------------
 * Name: c_iom_set_auto_sleep_timer
 *
 * Description: This API configures the auto sleep timer that will trigger the
 *              system into standby mode if no IRRC key is received during 
 *              that period. Otherwise, the timer will be reset upon an IRRC
 *              key reception.
 *
 * Inputs:  ui4_sleep_time      Contains auto sleep time in seconds.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                     Routine successful.
 *          IOMR_NOT_INIT               The IO Manager is not initialized.
 *          IOMR_INV_ARG                One or more of the arguments contain
 *                                      invalid data.
 *          IOMR_TIMER_ERROR            A timer error occurred.
 ----------------------------------------------------------------------------*/
INT32 c_iom_set_auto_sleep_timer(UINT32 ui4_sleep_time)
{
    return (x_iom_set_auto_sleep_timer(ui4_sleep_time));
}


/*-----------------------------------------------------------------------------
 * Name: c_iom_set_auto_sleep_timer_ex
 *
 * Description: This API configures the auto sleep timer which will set the
 *              system into standby mode when none of IRRC key is received  
 *              during specific period of time. The timer will be restarted if 
 *              any IRRC key is received before expiration. Note auto sleep 
 *              timer contains two types, prompt and sleep timers.
 *
 * Inputs:  pt_auto_sleep_timer     References the auto sleep timer data
 *                                  structure.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                     Routine successful.
 *          IOMR_NOT_INIT               The IO Manager is not initialized.
 *          IOMR_INV_ARG                One or more of the arguments contain
 *                                      invalid data.
 *          IOMR_TIMER_ERROR            A timer error occurred.
 ----------------------------------------------------------------------------*/
INT32 c_iom_set_auto_sleep_timer_ex(AUTO_SLEEP_TIMER_T* pt_auto_sleep_timer)
{
    return (x_iom_set_auto_sleep_timer_ex(pt_auto_sleep_timer));
}

#if IOM_SUPPORT_IRRC_DISABLE_KEYS
/*-----------------------------------------------------------------------------
 * Name: c_iom_set_global_disable_evtcode
 *
 * Description: This API set the disable event code info 
 *
 * Inputs:  pt_disable_evtcode     References the disable event code info
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                     Routine successful.
 *          IOMR_NOT_INIT               The IO Manager is not initialized.
 *          IOMR_INV_ARG                One or more of the arguments contain  
 *                                      invalid data.
 ----------------------------------------------------------------------------*/
INT32   c_iom_set_global_disable_evtcode(DISABLE_EVTCODE_T* pt_disable_evtcode)
{
     return (x_iom_set_global_disable_evtcode(pt_disable_evtcode));
}

/*-----------------------------------------------------------------------------
 * Name: c_iom_get_global_disable_evtcode_cnt
 *
 * Description: This API get  the disable event code count
 *
 * Inputs:  - 
 *
 * Outputs: -
 *
 * Returns:  the count of the disable event code
 ----------------------------------------------------------------------------*/
UINT32  c_iom_get_global_disable_evtcode_cnt()
{
     return (x_iom_get_global_disable_evtcode_cnt());
}

/*-----------------------------------------------------------------------------
 * Name: c_iom_get_global_disable_evtcode
 *
 * Description: This API get  the disable event code information
 *
 * Inputs:  - 
 *
 * Outputs: -  pt_disable_evtcode     References the disable event code info,Please note 
 *                                    the z_evtcode_cnt must be gotten by 
 *                                    c_iom_get_global_disable_evtcode_cnt
 *
  * Returns: IOMR_OK                     Routine successful.
 *          IOMR_NOT_INIT               The IO Manager is not initialized.
 *          IOMR_INV_ARG                One or more of the arguments contain  invalid data.
 ----------------------------------------------------------------------------*/
INT32   c_iom_get_global_disable_evtcode(DISABLE_EVTCODE_T* pt_disable_evtcode)
{
   return (x_iom_get_global_disable_evtcode(pt_disable_evtcode));
}
#endif

#ifdef IOM_CUSTOM_KEY_REPEAT_INTERVAL_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: c_iom_set_custom_key_repeat_interval
 *
 * Description: This API sets the custom key repeat interval configuration.
 *
 * Inputs:  ui4_interval_unit  - Unit of first/other interval in pt_setting
 *                               in milliseconds. Should be multiple of 10.
 *          ui4_custom_key_num - Number of custom key settings in pt_setting
 *          pt_setting         - Array of custom key settings. Each item
 *                               contains (event code, first interval units,
 *                               other interval units).
 *
 * Outputs: -
 *
 * Returns: -
 *        IOMR_OK        - Routine successful.
 *        IOMR_NOT_INIT  - The IO Manager is not initialized.
 *        IOMR_INV_ARG   - One or more of the arguments contain  invalid data.
 *
 ----------------------------------------------------------------------------*/
INT32   c_iom_set_custom_key_repeat_interval(
        UINT32                          ui4_interval_unit,
        UINT32                          ui4_custom_key_num,
        IOM_CUST_KEY_REP_ITVL_ITEM_T*   pt_setting)
{
    return x_iom_set_custom_key_repeat_interval(ui4_interval_unit,
                                                ui4_custom_key_num,
                                                pt_setting);
}
#endif
