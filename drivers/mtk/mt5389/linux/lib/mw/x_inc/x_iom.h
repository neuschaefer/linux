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
/*----------------------------------------------------------------------------*/
/*! @file x_iom.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains IO Manager related definitions, which
 *         are exported.
 */
/*----------------------------------------------------------------------------*/

#ifndef _X_IOM_H_
#define _X_IOM_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_iom.h"
#include "u_handle.h"

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*! @brief This API opens a device whose attribute can be input, output,  or both. 
  *              An application is allowed to do multiple opens to a   device simultaneously.
 *
 *  @param [in] e_dev_type  - The device type.
 *  @param [in] ui2_id          - The device id.
 *  @param [in] ps_name      - The device name.
 *  @param [in] pv_setting    - The setting to the device if required.
 *  @param [in] pv_tag         - Private tag value which must be returned in the  notify
 *                                          function. It can be set to NULL.
 *  @param [in] pf_nfy          - References the caller's notify function.
 *
 *  @param [out] ph_dev       - A handle referencing the opened device.
 *
 *  @note
 *  @see
 *
 *  @return Return the error code.
 *  @retval  IOMR_OK            - Routine successful.
 *  @retval  IOMR_NOT_INIT  - The IO Manager is not initialized.
 *  @retval  IOMR_INV_ARG   - One or more of the arguments contain  invalid data. 
 */
/*------------------------------------------------------------------*/
extern INT32 x_iom_open(IOM_DEV_TYPE_T e_dev_type,
                        UINT16         ui2_id,
                        const CHAR*    ps_name,
                        VOID*          pv_setting,
                        VOID*          pv_tag,
                        x_iom_nfy_fct  pf_nfy,
                        HANDLE_T*      ph_dev);

/*------------------------------------------------------------------*/
/*! @brief  This API closes an opened device referenced by h_dev.
 *
 *  @param [in]  h_dev  - A handle referencing the device.
 *
 *  @param [out] None.
 *
 *  @note
 *  @see
 *
 *  @return Return the error code.
 *  @retval  IOMR_OK                 -  Routine successful.
 *  @retval  IOMR_NOT_INIT       - The IO Manager is not initialized.
 *  @retval  IOMR_INV_HANDLE  - The handle is invalid.
 */
/*------------------------------------------------------------------*/
extern INT32 x_iom_close(HANDLE_T h_dev);

/*------------------------------------------------------------------*/
/*! @brief  This API gets some info from a device referenced by h_dev.
 *
 *  @param [in]  h_dev              - A handle referencing the device.
 *  @param [in]  e_get_type       - Specifies the type of information which shall
 *                                                be returned in the buffer, referenced by
 *                                                argument pv_get_info.
 *  @param [in] pz_get_info_len  - Contains the length of the buffer which is
 *                                                 referenced by argument pv_get_info.
 *
 *
 *  @param [out] pv_get_info    - Contains the retrieved information.
 *
 *  @note
 *  @see
 *
 *  @return Return the error code.
 *  @retval  IOMR_OK                -  Routine successful.
 *  @retval  IOMR_NOT_INIT      - The IO Manager is not initialized.
 *  @retval  IOMR_INV_ARG       - One or more of the arguments contain  invalid data.
 *  @retval  IOMR_INV_HANDLE  - The handle is invalid. 
 */
/*------------------------------------------------------------------*/
extern INT32 x_iom_get(HANDLE_T  h_dev,
                       IOM_GET_T e_get_type,
                       VOID*     pv_get_info,
                       SIZE_T*   pz_get_info_len);

/*------------------------------------------------------------------*/
/*! @brief This API sets some info to a device referenced by h_dev.
 *
 *  @param [in] h_dev             - A handle referencing the device.
 *  @param [in] e_set_type      - Specifies the type of information which is referenced
 *                                             by argument pv_set_info.
 *  @param [in]  pv_set_info     - Contains the set information.
 *  @param [in] z_set_info_len  - Indicates the length of the data in the buffer
 *                                               referenced by argument pv_set_info.
 *
 *  @param [out] None.
 *
 *  @note
 *  @see
 *
 *  @return Return the error code.
 *  @retval  IOMR_OK                - Routine successful.
 *  @retval  IOMR_NOT_INIT      - The IO Manager is not initialized.
 *  @retval  IOMR_INV_ARG       - One or more of the arguments contain invalid data.
 *  @retval  IOMR_INV_HANDLE  - The handle is invalid.
 */
/*------------------------------------------------------------------*/
extern INT32 x_iom_set(HANDLE_T    h_dev,
                       IOM_SET_T   e_set_type,
                       const VOID* pv_set_info,
                       SIZE_T      z_set_info_len);

/*------------------------------------------------------------------*/
/*! @brief  This API sends an event to an output or IO driver.
 *
 *  @param [in]  h_dev         - A handle referencing the device.
 *  @param [in] pv_otp_info  - The output event information.
 *
 *  @param [out] None.
 *
 *  @note
 *  @see
 *
 *  @return Return the error code.
 *  @retval  IOMR_OK               -  Routine successful.
 *  @retval  IOMR_NOT_INIT     - The IO Manager is not initialized.
 *  @retval  IOMR_INV_ARG      - One or more of the arguments contain  invalid data.
 *  @retval  IOMR_DEV_BUSY    - Device is busy.
 *  @retval  IOMR_INV_HANDLE - The handle is invalid.
 */
/*------------------------------------------------------------------*/
extern INT32 x_iom_output(HANDLE_T h_dev,
                          VOID*    pv_otp_info);

/*------------------------------------------------------------------*/
/*! @brief This API is called in order to generate a pair of button-down
 *              and button-up events.
 *
 *  @param [in] ui4_evt_code  - Contains the event code.
 *  @param [in] ui4_itvl           - Contains the interval between button-down and
 *                                             button-up events in ms.
 *
 *  @param [out] None.
 *
 *  @note
 *  @see
 *
 *  @return Return the error code.
 *  @retval  IOMR_OK             - Routine successful.
 *  @retval IOMR_MSGQ_FULL - Message Queue is full.
 */
/*------------------------------------------------------------------*/
extern INT32 x_iom_send_evt(UINT32 ui4_evt_code,
                            UINT32 ui4_itvl);

/*------------------------------------------------------------------*/
/*! @brief This API is called to generate either a button-down or a  button-up event.
 *
 *  @param [in] ui4_evt_code  - Contains the event code.
 *  @param [in] b_btn_down    - Contains the interval between button-down and 
 *                                              button-up events in ms.
 *
 *  @param [out] None.
 *
 *  @note
 *  @see
 *
 *  @return Return the error code.
 *  @retval  IOMR_OK               - Routine successful.
 *  @retval  IOMR_NOT_INIT     - The IO Manager is not initialized. 
 *  @retval  IOMR_MSGQ_FULL  - Message Queue is full.
 */
/*------------------------------------------------------------------*/
extern INT32 x_iom_send_evt_single(UINT32 ui4_evt_code,
                                   BOOL   b_btn_down);
                                   

/*------------------------------------------------------------------*/
/*! @brief This API sends a raw data event to IO Manager.
 *
 *  @param [in] ui4_raw_data  - Contains the raw data.
 *  @param [in] ui4_itvl           - Contains the interval between button-down and 
 *                                              button-up events in ms.
 *
 *  @param [out] None.
 *
 *  @note
 *  @see
 *
 *  @return Return the error code.
 *  @retval  IOMR_OK               - Routine successful.
 *  @retval  IOMR_NOT_INIT     - The IO Manager is not initialized. 
 *  @retval  IOMR_MSGQ_FULL  - Message Queue is full.
 */
/*------------------------------------------------------------------*/
extern INT32 x_iom_send_raw_data(UINT32 ui4_raw_data,
                                 UINT32 ui4_itvl);
                                 
/*------------------------------------------------------------------*/
/*! @brief This API converted IRRC raw data to an event code.
 *
 *  @param [in] h_dev       - A handle referencing the device.
 *  @param [in] pt_data     - Contains the raw date to be converted.
 *
 *  @param [out] pt_data   - Contains the event code after conversion.
 *
 *  @note
 *  @see
 *
 *  @return Return the error code.
 *  @retval  IOMR_OK                       - Routine successful.
 *  @retval  IOMR_NOT_INIT             - The IO Manager is not initialized. 
 *  @retval  IOMR_INV_HANDLE        - The handle is invalid.
 *  @retval  IOMR_DEV_SET_FAILED - Device get operation is failed. 
 */
/*------------------------------------------------------------------*/
extern INT32 x_iom_convert_raw_data_to_evt_code(HANDLE_T                    h_dev,
                                                IOM_RAW_DATA_TO_EVT_CODE_T* pt_data);

/*------------------------------------------------------------------*/
/*! @brief This API converted IRRC raw bits to an event code.
 *
 *  @param [in]  h_dev     - A handle referencing the device.
 *  @param [in]  pt_data   - Contains the raw bits to be converted.
 *
 *  @param [out] pt_data  - Contains the event code after conversion.
 *
 *  @note
 *  @see
 *
 *  @return Return the error code.
 *  @retval IOMR_OK                        - Routine successful.
 *  @retval IOMR_NOT_INIT              - The IO Manager is not initialized. 
 *  @retval IOMR_INV_HANDLE         - The handle is invalid.
 *  @retval IOMR_DEV_SET_FAILED  - Device get operation is failed. 
 */
/*------------------------------------------------------------------*/
extern INT32 x_iom_convert_raw_bits_to_evt_code(HANDLE_T                    h_dev,
                                                IOM_RAW_BITS_TO_EVT_CODE_T* pt_data);
                                         
#if !IOM_REMOVE_APDEMON_IRRC
/*------------------------------------------------------------------*/
/*! @brief  This API is called by Apdemon in response to received IRRC
 *               events from ATV Manager.
 *
 *  @param [in] ui4_evt_code  - The event code.
 *
 *  @param [out] None.
 *
 *  @note
 *  @see
 *
 *  @return Return the error code.
 *  @retval  IOMR_OK           -  Routine successful.
 *  @retval  IOMR_NOT_INIT - The IO Manager is not initialized.
 *  @retval  IOMR_INV_ARG  - One or more of the arguments contain invalid data.
 */
/*------------------------------------------------------------------*/
extern INT32 x_iom_apdemon_send_evt(UINT32 ui4_evt_code);
#endif

/*------------------------------------------------------------------*/
/*! @brief  This API configures the auto sleep timer that will trigger the
 *               system into standby mode if no IRRC key is received during 
 *               that period. Otherwise, the timer will be reset upon an IRRC
 *               key reception.
 *
 *  @param [in] ui4_sleep_time  - Contains auto sleep time in seconds.
 *
 *  @param [out] None.
 *
 *  @note
 *  @see
 *
 *  @return Return the error code.
 *  @retval IOMR_OK                   - Routine successful.
 *  @retval IOMR_NOT_INIT         - The IO Manager is not initialized.
 *  @retval IOMR_INV_ARG          - One or more of the arguments contain invalid data.
 *  @retval IOMR_TIMER_ERROR   - A timer error occurred.
 */
/*------------------------------------------------------------------*/
extern INT32 x_iom_set_auto_sleep_timer(UINT32 ui4_sleep_time);

/*------------------------------------------------------------------*/
/*! @brief  This API configures the auto sleep timer which will set the
 *               system into standby mode when none of IRRC key is received  
 *               during specific period of time. The timer will be restarted if 
 *               any IRRC key is received before expiration. Note auto sleep 
 *               timer contains two types, prompt and sleep timers.
 *
 *  @param [in] pt_auto_sleep_timer  - References the auto sleep timer data
 *                                                       structure.
 *
 *  @param [out] None.
 *
 *  @note
 *  @see
 *
 *  @return
 *  @retval  IOMR_OK                   -  Routine successful.
 *  @retval  IOMR_NOT_INIT         - The IO Manager is not initialized.
 *  @retval  IOMR_INV_ARG          - One or more of the arguments contain  invalid data.
 *  @retval  IOMR_TIMER_ERROR  - A timer error occurred.
 */
/*------------------------------------------------------------------*/

extern INT32 x_iom_set_auto_sleep_timer_ex(AUTO_SLEEP_TIMER_T* pt_auto_sleep_timer);

#if IOM_SUPPORT_IRRC_DISABLE_KEYS

/*------------------------------------------------------------------*/
/*! @brief  This API set the disable event code info 
 *
 *  @param [in] pt_disable_evtcode  - References the disable event code info
 *
 *  @param [out]
 *
 *  @note
 *  @see
 *
 *  @return  Return the error code.
 *  @retval  IOMR_OK           -  Routine successful.
 *  @retval  IOMR_NOT_INIT  - The IO Manager is not initialized.
 *  @retval  IOMR_INV_ARG   - One or more of the arguments contain invalid data.
 */
/*------------------------------------------------------------------*/
extern INT32   x_iom_set_global_disable_evtcode(DISABLE_EVTCODE_T* pt_disable_evtcode);

/*------------------------------------------------------------------*/
/*! @brief  This API get  the disable event code count
 *
 *  @param [in] None.
 *
 *  @param [out] None.
 *
 *  @note
 *  @see
 *
 *  @return Return the count of the global disable event code.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern UINT32  x_iom_get_global_disable_evtcode_cnt(VOID);

/*------------------------------------------------------------------*/
/*! @brief  This API get  the disable event code information.
 *
 *  @param [in] None.
 *
 *  @param [out] pt_disable_evtcode  - References the disable event code info,Please note 
 *                                                       the z_evtcode_cnt must be gotten by 
 *                                                       c_iom_get_global_disable_evtcode_cnt
 *
 *  @note
 *  @see
 *
 *  @return  Return the error code.
 *  @retval  IOMR_OK            - Routine successful.
 *  @retval  IOMR_NOT_INIT  - The IO Manager is not initialized.
 *  @retval  IOMR_INV_ARG   - One or more of the arguments contain  invalid data.
 */
/*------------------------------------------------------------------*/
extern INT32   x_iom_get_global_disable_evtcode(DISABLE_EVTCODE_T* pt_disable_evtcode);
#endif

#ifdef IOM_CUSTOM_KEY_REPEAT_INTERVAL_SUPPORT
/*------------------------------------------------------------------*/
/*! @brief  This API sets the custom key repeat interval configuration.
 *  @param [in] ui4_interval_unit  - Unit of first/other interval in pt_setting
 *                                   in milliseconds. Should be multiple of 10.
 *  @param [in] ui4_custom_key_num - Number of custom key settings in pt_setting
 *  @param [in] pt_setting         - Array of custom key settings. Each item
 *                                   contains (event code, first interval units,
 *                                   other interval units).
 *  @return  Return the error code.
 *  @retval  IOMR_OK            - Routine successful.
 *  @retval  IOMR_NOT_INIT  - The IO Manager is not initialized.
 *  @retval  IOMR_INV_ARG   - One or more of the arguments contain  invalid data.
 */
/*------------------------------------------------------------------*/
extern INT32   x_iom_set_custom_key_repeat_interval(
        UINT32                          ui4_interval_unit,
        UINT32                          ui4_custom_key_num,
        IOM_CUST_KEY_REP_ITVL_ITEM_T*   pt_setting);
#endif

#endif /* _X_IOM_H_ */

