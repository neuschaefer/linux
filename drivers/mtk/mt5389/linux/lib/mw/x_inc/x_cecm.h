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
/*! @file x_cecm.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains CEC Manager related definitions, which
 *         are exported. 
 */
/*----------------------------------------------------------------------------*/

#ifndef _X_CECM_H_
#define _X_CECM_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "x_common.h"
#include "u_handle.h"
#include "u_cecm.h"

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_CECM
 *  @{ 
 */
/*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
 /*------------------------------------------------------------------*/
/*! @brief         This API initializes the CEC Manager.
 *  @param [in]    pt_cecm_thread_descr     - References a thread descriptor structure.
 *  @return        Return the error code
 *  @retval        CECMR_OK                - Routine successful
 *  @retval        CECMR_ALREADY_INIT      - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_ERROR             - One or more of the arguments contain invalid data. 
 *  @retval        CECMR_OUT_OF_HANDLE     - No handle available.
 *  @retval        CECMR_DRV_COMP_ERROR    - CEC driver operation error.
 *  @retval        CECMR_CLI_ERROR         - CLI operation is failed. 
 */ 
/*------------------------------------------------------------------*/
extern INT32 x_cecm_init(THREAD_DESCR_T* pt_cecm_thread_descr);

/*------------------------------------------------------------------*/
/*! @brief         This API is used to open a CECM service.
 *  @param [in]    pf_nfy         - References the caller's notify function 
 *  @param [in]    pv_tag         - Private tag value which must be returned in the notify function. It can be set to Null
 *  @param [out]   ph_cecm_svc    - A handle referencing the opened service.        
 *  @see           x_cecm_close_svc
 *  @return        Return the error code
 *  @retval        CECMR_OK                    - Routine successful
 *  @retval        CECMR_NOT_INIT              - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG               - One or more of the arguments contain invalid data.
 *  @retval        CECMR_OUT_OF_RESOURCE       - Not enough CECM resource.
 *  @retval        CECMR_OUT_OF_HANDLE         - No handle available.
 */
/*------------------------------------------------------------------*/
extern INT32 x_cecm_open_svc(x_cecm_nfy_fct pf_nfy,
                             VOID*          pv_tag,
                             HANDLE_T*      ph_cecm_svc);

/*------------------------------------------------------------------*/
/*! @brief         This API is used to close a CECM service.
 *  @param [in]    h_cecm_svc      A handle referencing the service.
 *  @see           x_cecm_open_svc
 *  @return        Return the error code
 *  @retval        CECMR_OK                    - Routine successful.
 *  @retval        CECMR_NOT_INIT              - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG               - One or more of the arguments contain invalid data.
 *  @retval        CECMR_HANDLE_FREE_ERROR     - Handle free error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_cecm_close_svc(HANDLE_T h_cecm_svc);

/*------------------------------------------------------------------*/
/*! @brief          The API get if received active source already.
 *  @param [in]     h_cecm_svc                   -A handle referencing the service.
 *  @param [out]    pt_act_src_reved             -Active source information.
 *  @return         Return the error code
 *  @retval         CECMR_OK                    -Routine successful.
 *  @retval         CECMR_INV_ARG               -One or more of the arguments contain
 *  @retval         CECMR_INV_ARG               -CECMR_CEC_FCT_DISABLED      CEC function is disabled. 
 */
/*------------------------------------------------------------------*/
extern INT32 x_cecm_is_act_src_reved(HANDLE_T h_cecm_svc , CECM_ACT_SRC_REVED_T* pt_act_src_reved);

/* General Feature ***********************************************************/
/*------------------------------------------------------------------*/
/*! @brief         The API is used to switch CEC control between CEC driver and uP.
 *  @param [in]    h_cecm_svc  -A handle referencing the service.
 *  @param [in]    b_ready     -Indicates the current control right (TRUE => CEC driver, FALSE => uP).           
 *  @return        Return the error code
 *  @retval        CECMR_OK                    - Routine successful
 *  @retval        CECMR_NOT_INIT              - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG               - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED      - CEC function is disabled.
 *  @retval        CECMR_DRV_COMP_ERROR        - CEC driver operation error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_cecm_signal_cec_ready(HANDLE_T h_cecm_svc,
                                     BOOL     b_ready);

/*------------------------------------------------------------------*/
/*! @brief         This API sets uP CEC wakeup control.   
 *  @param [in]    h_cecm_svc      - A handle referencing the service.
 *  @param [in]    pt_wakeup_ctrl  - Wake-up control info.
 *  @return        Return the error code
 *  @retval        CECMR_OK                    - Routine successful
 *  @retval        CECMR_NOT_INIT              - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG               - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED      - CEC function is disabled.
 *  @retval        CECMR_DRV_COMP_ERROR        - CEC driver operation error.

 */
/*------------------------------------------------------------------*/
extern INT32 x_cecm_set_cec_wakeup_ctrl(HANDLE_T             h_cecm_svc,
                                        CECM_WAKE_UP_CTRL_T* pt_wakeup_ctrl);

/*------------------------------------------------------------------*/
/*! @brief         The API enables/disables CEC functions.
 *  @param [in]    h_cecm_svc      - A handle referencing the service.
 *  @param [in]    t_params        - Contains cec_set_fct parameters.
 *  @return        Return the error code
 *  @retval        CECMR_OK                    - Routine successful
 *  @retval        CECMR_NOT_INIT              - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG               - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED      - CEC function is disabled.
 *  @retval        CECMR_DRV_COMP_ERROR        - CEC driver operation error.
 *  @retval        CECMR_MSG_SEND_ERROR        - Send message failed. 
 */
/*------------------------------------------------------------------*/
extern INT32 x_cecm_set_cec_fct(HANDLE_T                  h_cecm_svc,
                                CECM_SET_CEC_FCT_PARAMS_T t_params);

/*------------------------------------------------------------------*/
/*! @brief          This API gets the first logical address in the list which is  
 *                  mapped to a physical address.
 *  @param [in]     h_cecm_svc      A handle referencing the service.
 *  @param [in]     ui2_pa          A physical address.
 *  @return         Return the error code
 *  @retval         CECM_LOG_ADDR_MAX    -Failed
 *  @retval         Other                -Device's logical address.
 */
/*------------------------------------------------------------------*/
extern CECM_LOG_ADDR_T x_cecm_get_la_by_pa(HANDLE_T h_cecm_svc,
                                           UINT16   ui2_pa);

/*------------------------------------------------------------------*/
/*! @brief         This API gets the logical address(es) which is(are) mapped to a physical address.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    ui2_pa         - A physical address.
 *  @param [in]    pt_la_lst      - References a logical address list.
 *  @return        Return the error code
 *  @retval        CECMR_OK       - Routine successful.
 *  @retval        CECMR_INV_ARG  - One or more of the arguments contain invalid data.
 */
/*------------------------------------------------------------------*/
extern INT32 x_cecm_get_la_by_pa_ex(HANDLE_T            h_cecm_svc,
                                    UINT16              ui2_pa,
                                    CECM_LOG_ADD_LST_T* pt_la_lst);                                   

/*------------------------------------------------------------------*/
/*! @brief         This API gets the physical address of a logical address.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @return        Return the error code
 *  @retval        CECM_LOG_ADDR_MAX            - Failed
 *  @retval        Other                        - It's logical address.
 */
/*------------------------------------------------------------------*/
extern UINT16 x_cecm_get_pa_by_la(HANDLE_T        h_cecm_svc,
                                  CECM_LOG_ADDR_T e_la);

/*------------------------------------------------------------------*/
/*! @brief         The API gets device info by logical address.
 *  @param [in]    h_cecm_svc      - A handle referencing the service.
 *  @param [in]    e_la            - CEC device logical address.
 *  @param [in]    pt_ctrl         - References the control info.
 *  @param [out]   pt_dev_info     - References device info.
 *  @return        Return the error code
 *  @retval        CECMR_OK                    - Routine successful
 *  @retval        CECMR_NOT_INIT              - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG               - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED      - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL          - Device is not available.
 */
/*------------------------------------------------------------------*/
extern INT32 x_cecm_get_dev_info_by_la(HANDLE_T        h_cecm_svc,
                                       CECM_LOG_ADDR_T e_la,
                                       CECM_CTRL_T*    pt_ctrl,
                                       CECM_DEV_T*     pt_dev_info);

/*------------------------------------------------------------------*/
/*! @brief         The API gets component ID by logical address.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @param [out]   pui2_comp_id   - HDMI component ID.
 *  @return        Return the error code
 *  @retval        CECMR_OK                    - Routine successful
 *  @retval        CECMR_NOT_INIT              - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG               - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED      - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL          - Device is not available.

 */
/*------------------------------------------------------------------*/
extern INT32 x_cecm_get_comp_id_by_la(HANDLE_T        h_cecm_svc,
                                      CECM_LOG_ADDR_T e_la,
                                      UINT16*         pui2_comp_id);
 /*------------------------------------------------------------------*/
/*! @brief         The API gets component ID by logical address.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @param [in]    b_retry        - Do retry if device list not available.
 *  @param [out]   pui2_comp_id   - HDMI component ID.
 *  @return        Return the error code
 *  @retval        CECMR_OK                    - Routine successful
 *  @retval        CECMR_NOT_INIT              - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG               - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED      - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL          - Device is not available.

 */
/*------------------------------------------------------------------*/
extern INT32 x_cecm_get_comp_id_by_la_ex(HANDLE_T        h_cecm_svc,
                                          CECM_LOG_ADDR_T e_la,
                                          UINT16*         pui2_comp_id,
                                          BOOL b_retry);

/*------------------------------------------------------------------*/
/*! @brief         The API finds the TV logical address allocation.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @return        Return the error code 
 *  @retval        CECM_LOG_ADDR_MAX  - Failed.
 *  @retval        Other              - TV's logical address.
 */
/*------------------------------------------------------------------*/
extern CECM_LOG_ADDR_T x_cecm_find_tv_la_alloc(HANDLE_T h_cecm_svc);
/*------------------------------------------------------------------*/
/*! @brief         The API finds the TV logical address allocation and set the interval
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    ui4_itvl       - Polling LA0 interval
 *  @return        Return the error code 
 *  @retval        CECM_LOG_ADDR_MAX  - Failed.
 *  @retval        Other              - TV's logical address.
 */
/*------------------------------------------------------------------*/

extern CECM_LOG_ADDR_T x_cecm_find_tv_la_alloc_ex(HANDLE_T h_cecm_svc, UINT32 ui4_itvl);

/*------------------------------------------------------------------*/
/*! @brief         The API set the TV should take which logical address 
 *                 when the logical address 0 and 14 is used by other deive
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - Logical address
 *  @return        Return the error code
 *  @retval        CECMR_OK                    - Routine successful
 *  @retval        CECMR_NOT_INIT              - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG               - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED      - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL          - Device is not available.

 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_max_tv_la(HANDLE_T        h_cecm_svc,
                          CECM_LOG_ADDR_T   e_la);


/*------------------------------------------------------------------*/
/*! @brief         The API set support CEC version
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_cec_ver      - Logical address
 *  @return        Return the error code
 *  @retval        CECMR_OK                    - Routine successful
 *  @retval        CECMR_NOT_INIT              - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG               - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED      - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL          - Device is not available.

 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_support_cec_ver(HANDLE_T        h_cecm_svc,
                                        CECM_CEC_VER_T   e_cec_ver);

/*------------------------------------------------------------------*/
/*! @brief         The API config the device discovery information
 *  @param [in]    h_cecm_svc                - A handle referencing the service.
 *  @param [in]    pt_dev_discovery_config   - Reference of Device discovery config info.
 *  @see           x_cecm_get_dev_discovery_config
 *  @return        Return the error code 
 *  @retval        CECMR_OK                    Routine successful.
 *  @retval        CECMR_NOT_INIT              The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG               One or more of the arguments contain
 *  @retval        CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 */
/*------------------------------------------------------------------*/
extern INT32 x_cecm_config_dev_discovery(HANDLE_T        h_cecm_svc,
                                                CECM_DEV_DISCOVERY_CONFIG_T*   pt_dev_discovery_config);
/*------------------------------------------------------------------*/
/*! @brief         The API get the device discovery config information
 *  @param [in]    h_cecm_svc                - A handle referencing the service.
 *  @param [out]   pt_dev_discovery_config   - Reference of Device discovery config info.
 *  @see           x_cecm_config_dev_discovery
 *  @return        Return the error code 
 *  @retval        CECMR_OK                    Routine successful.
 *  @retval        CECMR_NOT_INIT              The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG               One or more of the arguments contain
 *  @retval        CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_get_dev_discovery_config(HANDLE_T        h_cecm_svc,
                                             CECM_DEV_DISCOVERY_CONFIG_T*   pt_dev_discovery_config);
/*------------------------------------------------------------------*/
/*! @brief         The API starts CEC device discovery.
 *  @param [in]    h_cecm_svc                  - A handle referencing the service.
 *  @param [in]    ui4_dev_discovery_itvl      - Device discovery interval in second.
 *  @see           x_cecm_stop_dev_discovery
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_cecm_start_dev_discovery(HANDLE_T h_cecm_svc,
                                        UINT32   ui4_dev_discovery_itvl);

/*------------------------------------------------------------------*/
/*! @brief         The API starts CEC device discovery.
 *  @param [in]    h_cecm_svc                 - A handle referencing the service.
 *  @param [in]    pt_dev_discovery_info      - References a device discovery
 *  @see           x_cecm_stop_dev_discovery
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_cecm_start_dev_discovery_ex(HANDLE_T              h_cecm_svc,
                                           CECM_DEV_DISCOVERY_T* pt_dev_discovery_info);

/*------------------------------------------------------------------*/
/*! @brief         The API stops CEC device discovery
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @see           x_cecm_start_dev_discovery
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_cecm_stop_dev_discovery(HANDLE_T h_cecm_svc);

/* One Touch Play Feature ****************************************************/
/*------------------------------------------------------------------*/
/*! @brief         This API is used to response the message Request Active Source.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @see           x_cecm_request_active_source
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_cecm_set_active_source(HANDLE_T h_cecm_svc);

/* Standby Feature ***********************************************************/
/*------------------------------------------------------------------*/
/*! @brief         The API switches a device into standby mode.
 *  @param [in]    h_cecm_svc      A handle referencing the service.
 *  @param [in]    e_la            CEC device logical address.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_OUT_OF_RESOURCE   - Not enough CECM resource.
 *  @retval        CECMR_OUT_OF_HANDLE     - No handle available.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_standby(HANDLE_T        h_cecm_svc,
                                CECM_LOG_ADDR_T e_la);

/*------------------------------------------------------------------*/
/*! @brief         The API switches all devices into standby mode.
 *  @param [in]    h_cecm_svc      - A handle referencing the service.
 *  @param [in]    b_sync          - Indicates if this is a synchronous operation.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_standby_all(HANDLE_T h_cecm_svc,
                                    BOOL     b_sync);

/* Routing Control Feature ***************************************************/
/*------------------------------------------------------------------*/
/*! @brief         This API discovers the current active source of the system.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @see           x_cecm_set_active_source
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_cecm_request_active_source(HANDLE_T h_cecm_svc);

/*------------------------------------------------------------------*/
/*! @brief         This API informs all devices on the newwork that the active
 *                 route had been changed.
 *  @param [in]    h_cecm_svc           - A handle referencing the service.
 *  @param [in]    ui1_orig_comp_id     - Original HDMI component ID.
 *  @param [in]    ui1_new_comp_id      - New HDMI component ID.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_routing_change(HANDLE_T h_cecm_svc,
                                       UINT16   ui2_orig_comp_id,
                                       UINT16   ui2_new_comp_id);
extern INT32 x_cecm_set_routing_change_ex(HANDLE_T h_cecm_svc,
                                        UINT16   ui2_orig_pa,
                                        UINT16   ui2_new_pa);

/*------------------------------------------------------------------*/
/*! @brief         This API requests a streaming path from the specified logical
 *                 device.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_OUT_OF_RESOURCE   - Not enough CECM resource.
 *  @retval        CECMR_OUT_OF_HANDLE     - No handle available.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_stream_path(HANDLE_T        h_cecm_svc,
                                    CECM_LOG_ADDR_T e_la);
                                    
/*------------------------------------------------------------------*/
/*! @brief         This API requests a streaming path from the specified logical
 *                 device.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    ui2_pa         - A physical address.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_OUT_OF_RESOURCE   - Not enough CECM resource.
 *  @retval        CECMR_OUT_OF_HANDLE     - No handle available.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_stream_path_ex(HANDLE_T h_cecm_svc,
                                       UINT16   ui2_pa);                                  

/* One Touch Record Feature **************************************************/
/*------------------------------------------------------------------*/
/*! @brief         This API attempts to record the specific source.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @param [in]    pt_rec_src     - References the record source info.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_cecm_set_record_on(HANDLE_T        h_cecm_svc,
                                  CECM_LOG_ADDR_T e_la,
                                  CECM_REC_SRC_T* pt_rec_src);

/*------------------------------------------------------------------*/
/*! @brief         This API requests a device to stop a recording.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_record_off(HANDLE_T        h_cecm_svc,
                                   CECM_LOG_ADDR_T e_la);

/* Timer Programming Feature *************************************************/
/*------------------------------------------------------------------*/
/*! @brief         This API is used to clear a timer block of a device.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @param [in]    pt_timer_info  - References the timer info.
 *  @see           x_cecm_set_timer
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_clear_timer(HANDLE_T           h_cecm_svc,
                                CECM_LOG_ADDR_T    e_la,
                                CECM_TIMER_INFO_T* pt_timer_info);

/*------------------------------------------------------------------*/
/*! @brief         This API is used to set a single timer block of a device.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @param [in]    pt_timer_info  - References the timer info.
 *  @see           x_cecm_clear_timer
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_timer(HANDLE_T           h_cecm_svc,
                              CECM_LOG_ADDR_T    e_la,
                              CECM_TIMER_INFO_T* pt_timer_info);

/*------------------------------------------------------------------*/
/*! @brief         This API is used to set the name of a program associated with a timer block.
 *  @param [in]    h_cecm_svc            - A handle referencing the service.
 *  @param [in]    e_la                  - CEC device logical address.
 *  @param [in]    pt_prog_title_str     - References a program title string.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_timer_prog_title(HANDLE_T           h_cecm_svc,
                                         CECM_LOG_ADDR_T    e_la,
                                         CECM_PROG_TITLE_T* pt_prog_title_str);

/* System Information Feature ************************************************/
/*------------------------------------------------------------------*/
/*! @brief         This API enquires which version of CEC the target supports.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_get_cec_version(HANDLE_T        h_cecm_svc,
                                    CECM_LOG_ADDR_T e_la);

/*------------------------------------------------------------------*/
/*! @brief         This API requests a device to return its physical address.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @see           x_cecm_report_physical_address
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_give_physical_address(HANDLE_T        h_cecm_svc,
                                          CECM_LOG_ADDR_T e_la);

/*------------------------------------------------------------------*/
/*! @brief         This API pings a CEC device.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @param [in]    b_sync         - Indicates if this is a synchronous operation.
 *  @param [out]   pt_polling_info  - References polling info.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_cecm_set_polling_message(HANDLE_T             h_cecm_svc,
                                        CECM_LOG_ADDR_T      e_la,
                                        BOOL                 b_sync,
                                        CECM_POLLING_INFO_T* pt_polling_info);

/*------------------------------------------------------------------*/
/*! @brief         This API informs all other devices of the mapping between
 *                 physical and logical address.
 *  @param [in]    h_cecm_svc             - A handle referencing the service.
 *  @param [in]    pt_report_phy_addr     - References the report physical address data structure
 *  @see           x_cecm_give_physical_address
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_report_physical_address(HANDLE_T                h_cecm_svc,
                                            CECM_REPORT_PHY_ADDR_T* pt_report_phy_addr);

/*------------------------------------------------------------------*/
/*! @brief         This API discovers the currently set menu language of a device.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @see           x_cecm_set_menu_lang
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_get_menu_lang(HANDLE_T        h_cecm_svc,
                                  CECM_LOG_ADDR_T e_la);

/*------------------------------------------------------------------*/
/*! @brief         This API indicates the menu language.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    pt_lang        - References ISO-639 language.
 *  @see           x_cecm_get_menu_lang
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_menu_lang(HANDLE_T        h_cecm_svc,
                                  ISO_639_LANG_T* pt_lang);

/* Deck Control Feature ******************************************************/
/*------------------------------------------------------------------*/
/*! @brief         This API requests the status of a device.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - Device logical address
 *  @param [in]    e_sts_req      - Contains status request.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_give_deck_status(HANDLE_T        h_cecm_svc,
                                     CECM_LOG_ADDR_T e_la,
                                     CECM_STS_REQ_T  e_sts_req);

/*------------------------------------------------------------------*/
/*! @brief         This API is used to control a device's media functions.
 *  @param [in]    h_cecm_svc           - A handle referencing the service.
 *  @param [in]    e_la                 - Logical address.
 *  @param [in]    e_deck_ctrl_mod      - Contains deck control mode.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - CEC function is disabled. 
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_deck_ctrl(HANDLE_T             h_cecm_svc,
                                  CECM_LOG_ADDR_T      e_la,
                                  CECM_DECK_CTRL_MOD_T e_deck_ctrl_mod);

/*------------------------------------------------------------------*/
/*! @brief         This API is used to control the playback behavior of a source
 *                 device.
 *  @param [in]    h_cecm_svc          - A handle referencing the service.
 *  @param [in]    e_la                - CEC device logical address.
 *  @param [in]    e_play_mod          - Contains play mode.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_cecm_set_deck_play(HANDLE_T        h_cecm_svc,
                                  CECM_LOG_ADDR_T e_la,
                                  CECM_PLAY_MOD_T e_play_mod);

/* Tuner Control Feature *****************************************************/
/*------------------------------------------------------------------*/
/*! @brief         This API is used to request the status of a tuner.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @param [in]    e_sts_req      - Contains status request.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_give_tuner_status(HANDLE_T        h_cecm_svc,
                                      CECM_LOG_ADDR_T e_la,
                                      CECM_STS_REQ_T  e_sts_req);

/*------------------------------------------------------------------*/
/*! @brief         This API is used to directly select an analog TV, digital TV,
 *                 radio or data broadcast service.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @param [in]    pt_svc_info    - References the tuner service info.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_select_service(HANDLE_T               h_cecm_svc,
                                   CECM_LOG_ADDR_T        e_la,
                                   CECM_TUNER_SVC_INFO_T* pt_svc_info);

/*------------------------------------------------------------------*/
/*! @brief         This API is used to tune to next lowest/highest service in a
 *                 tuner's service list.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @param [in]    e_tuner_step   - Contains tuner step info.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_tuner_step(HANDLE_T          h_cecm_svc,
                                   CECM_LOG_ADDR_T   e_la,
                                   CECM_TUNER_STEP_T e_tuner_step);

/* Vendor Specific Commands Feature ******************************************/
/*------------------------------------------------------------------*/
/*! @brief         This API reports the vendor ID of this device.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    pt_dev_vndr_id - References a device vendor ID.
 *  @param [in]    b_sync         - Indicates if this is a synchronous operation.
 *  @see           x_cecm_give_device_vendor_id
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - CEC function is disabled. 
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_device_vendor_id(HANDLE_T     h_cecm_svc,
                                  CECM_DEV_VNDR_ID_T* pt_dev_vndr_id,
                                  BOOL                b_sync);

/*------------------------------------------------------------------*/
/*! @brief         This API requests the vendor ID from a device.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @param [in]    b_sync         - Indicates if this is a synchronous operation.
 *  @see           x_cecm_set_device_vendor_id
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_give_device_vendor_id(HANDLE_T            h_cecm_svc,
                                          CECM_LOG_ADDR_T     e_la,
                                          BOOL                b_sync,
                                          CECM_DEV_VNDR_ID_T* pt_dev_vndr_id);
                                   
/*------------------------------------------------------------------*/
/*! @brief         This API allows vendor specific commands to be sent between
 *                 two devices.
*  @param [in]    h_cecm_svc           - A handle referencing the service.
*  @param [in]    e_la                 - CEC device logical address.
*  @param [in]    pt_vndr_cmd_data     - References vendor specific data.
*  @param [in]    b_sync               - Indicates if this is a synchronous operation.  
*  @see           x_cecm_set_vendor_cmd_with_id
*  @return        Return the error code 
*  @retval        CECMR_OK                - Routine successful.
*  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
*  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
*  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
*  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
*  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
*  @retval        CECMR_ERROR             - Internal error.
*/
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_vendor_cmd(HANDLE_T              h_cecm_svc,
                                   CECM_LOG_ADDR_T       e_la,
                                   CECM_VNDR_CMD_DATA_T* pt_vndr_cmd_data,
                                   BOOL                  b_sync);



/*------------------------------------------------------------------*/
/*! @brief         This API allows vendor specific commands to be sent between
 *                 two devices.
*  @param [in]    h_cecm_svc           - A handle referencing the service.
*  @param [in]    e_la                 - CEC device logical address.
*  @param [in]    e_opcode                 - CEC vendor specific opcode.
*  @param [in]    pt_spec_cmd_data     - References vendor specific data.
*  @param [in]    b_sync               - Indicates if this is a synchronous operation.  
*  @see           x_cecm_set_vendor_cmd_with_id
*  @return        Return the error code 
*  @retval        CECMR_OK                - Routine successful.
*  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
*  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
*  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
*  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
*  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
*  @retval        CECMR_ERROR             - Internal error.
*/
/*------------------------------------------------------------------*/
extern INT32 x_cecm_set_specific_cmd(HANDLE_T              h_cecm_svc,
                            CECM_LOG_ADDR_T       e_la,
                            CECM_OPCODE_T         e_opcode,
                            CECM_SPEC_CMD_DATA_T* pt_spec_cmd_data,
                            BOOL                  b_sync);

/*------------------------------------------------------------------*/
/*! @brief         This API allows vendor specific commands to be sent between
 *                 two devices or broadcast.
 *  @param [in]    h_cecm_svc                   - A handle referencing the service.
 *  @param [in]    e_la                         - CEC device logical address.
 *  @param [in]    pt_vndr_cmd_with_id_data     - References vendor specific data with  ID.
 *  @param [in]    b_sync                       - Indicates if this is a synchronous operation.  
 *  @see           x_cecm_set_vendor_cmd
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_vendor_cmd_with_id(HANDLE_T                      h_cecm_svc,
                                           CECM_LOG_ADDR_T               e_la,
                                           CECM_VNDR_CMD_WITH_ID_DATA_T* pt_vndr_cmd_with_id_data,
                                           BOOL                          b_sync);

/*------------------------------------------------------------------*/
/*! @brief         This API indicates that a remote control button has been depressed.
 *  @param [in]    h_cecm_svc               - A handle referencing the service.
 *  @param [in]    e_la                     - CEC device logical address.
 *  @param [in]    pt_vndr_rc_code_data     - References vendor specific RC code.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_vendor_remote_btn_down(HANDLE_T                  h_cecm_svc,
                                               CECM_LOG_ADDR_T           e_la,
                                               CECM_VNDR_RC_CODE_DATA_T* pt_vndr_rc_code_data);

/*------------------------------------------------------------------*/
/*! @brief         This API indicates that a remote control button (the last
 *                 button pressed indicated by the Vendor Remote Button Down
 *                 message) Button has been released.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_vendor_remote_btn_up(HANDLE_T        h_cecm_svc,
                                             CECM_LOG_ADDR_T e_la);

/* Device OSD Transfer Feature ***********************************************/
/*------------------------------------------------------------------*/
/*! @brief         This API sets the preferred OSD name of a device for use in
 *                 menus associated with that device.
 *  @param [in]    h_cecm_svc        - A handle referencing the service.
 *  @param [in]    e_la              - CEC device logical address.
 *  @param [in]    paw2_osd_name     - References a OSD name.
 *  @see           x_cecm_give_osd_name
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_osd_name(HANDLE_T        h_cecm_svc,
                                 CECM_LOG_ADDR_T e_la,
                                 UTF16_T*        paw2_osd_name);
                          
/*------------------------------------------------------------------*/
/*! @brief         This API requests the preferred OSD name of a device for use in
 *                 menus associated with that device.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @see           x_cecm_set_osd_name
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_give_osd_name(HANDLE_T        h_cecm_svc,
                                  CECM_LOG_ADDR_T e_la);

/* Device Menu Control Feature ***********************************************/
/*------------------------------------------------------------------*/
/*! @brief         This API is used to request a device to show/remove a menu or
 *                 to query if a device is currently showing a menu.
 *  @param [in]    h_cecm_svc          - A handle referencing the service.
 *  @param [in]    e_la                - CEC device logical address.
 *  @param [in]    e_menu_req_type     - Contains menu request type.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_menu_request(HANDLE_T              h_cecm_svc,
                                     CECM_LOG_ADDR_T       e_la,
                                     CECM_MENU_REQ_STATE_T e_menu_req_type);

/* Remote Control Passthrough Feature ****************************************/
/*------------------------------------------------------------------*/
/*! @brief         This API indicates that the user pressed a remote control
 *                 button or switched from one remote control button to another.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @param [in]    e_usr_ctrl     - Contains user control info.
 *  @see           x_cecm_set_user_ctrl_released
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/


extern INT32 x_cecm_set_user_ctrl_pressed(HANDLE_T         h_cecm_svc,
                                          CECM_LOG_ADDR_T  e_la,
                                          CECM_USER_CTRL_T e_usr_ctrl);

/*------------------------------------------------------------------*/
/*! @brief         This API indicates that the user pressed a remote control
 *                 button or switched from one remote control button to another,
 *                 and can specify operand
 *  @param [in]    h_cecm_svc                   - A handle referencing the service.
 *  @param [in]    e_la                         - CEC device logical address.
 *  @param [in]    pt_usr_ctrl_with_operand     - User ctrl with operand info
 *  @see           x_cecm_set_user_ctrl_released
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_user_ctrl_pressed_ex(HANDLE_T         h_cecm_svc,
                                   CECM_LOG_ADDR_T  e_la,
                                   CECM_USER_CTRL_WITH_OPERAND_T* pt_usr_ctrl_operand_info);

/*------------------------------------------------------------------*/
/*! @brief         This API indicates that the user release a remote control
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @see           x_cecm_set_user_ctrl_pressed
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_user_ctrl_released(HANDLE_T        h_cecm_svc,
                                           CECM_LOG_ADDR_T e_la);

/* Power Status Feature ******************************************************/
/*------------------------------------------------------------------*/
/*! @brief         This API powers on a device.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_cecm_power_on_dev(HANDLE_T        h_cecm_svc,
                                 CECM_LOG_ADDR_T e_la);

/*------------------------------------------------------------------*/
/*! @brief         This API is used to determine the current power status of a
 *                 target device.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @param [in]    b_sync         - Indicates if this is a synchronous operation.
 *  @see           x_cecm_report_power_status
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_give_device_power_status(HANDLE_T        h_cecm_svc,
                                             CECM_LOG_ADDR_T e_la,
                                             BOOL            b_sync,
                                             CECM_PWR_STS_T* pe_pwr_sts);

/*------------------------------------------------------------------*/
/*! @brief         This API reports current power status.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @param [in]    e_pwr_sts      - Contains power status.
 *  @see           x_cecm_give_device_power_status
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_report_power_status(HANDLE_T        h_cecm_svc,
                                        CECM_LOG_ADDR_T e_la,
                                        CECM_PWR_STS_T  e_pwr_sts);

/* General Protocol Feature **************************************************/
/*------------------------------------------------------------------*/
/*! @brief         This API is used as a response to indicate that the requested
 *                 message type is not supported.
 *  @param [in]    h_cecm_svc           - A handle referencing the service.
 *  @param [in]    e_la                 - CEC device logical address.
 *  @param [in]    pt_feature_abort     - References the feature abort info.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_feature_abort(HANDLE_T              h_cecm_svc,
                                      CECM_LOG_ADDR_T       e_la,
                                      CECM_FEATURE_ABORT_T* pt_feature_abort);

/* System Audio Control Feature **********************************************/
/*------------------------------------------------------------------*/
/*! @brief         This API requests to use System Audio Mode to the amplifier.
 *  @param [in]    h_cecm_svc          - A handle referencing the service.
 *  @param [in]    pt_sys_aud_ctrl     - References the system audio control info.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_system_audio_mode_request(HANDLE_T             h_cecm_svc,
                                                  CECM_SYS_AUD_CTRL_T* pt_sys_aud_ctrl);

/*------------------------------------------------------------------*/
/*! @brief         This API requests to use System Audio Mode to the amplifier.
 *  @param [in]    h_cecm_svc          - A handle referencing the service.
 *  @param [in]    pt_sys_aud_ctrl     - References the system audio control info..
 *  @param [in]    b_sync              - Indicates if this is a synchronous operation.
 *  @param [out]   pe_sys_aud_sts      - References system audio status.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_set_system_audio_mode_request_ex(HANDLE_T             h_cecm_svc,
                                                     CECM_SYS_AUD_CTRL_T* pt_sys_aud_ctrl,
                                                     BOOL                 b_sync,
                                                     CECM_SYS_AUD_STS_T*  pe_sys_aud_sts);

/*------------------------------------------------------------------*/
/*! @brief         This API requests an amplifier to send its volume and mute
 *                 status.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @param [in]    b_sync         - Indicates if this is a synchronous operation.
 *  @param [out]   pui1_aud_sts   - References audio status of an amplifier (This
 *                                  argument is valid when b_sync = TRUE and is
 *                                  ignored if b_sync = FALSE).
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - CEC function is disabled. 
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_give_audio_status(HANDLE_T        h_cecm_svc,
                                      CECM_LOG_ADDR_T e_la,
                                      BOOL            b_sync,
                                      UINT8*          pui1_aud_sts);

/*------------------------------------------------------------------*/
/*! @brief         This API requests the status of the System Audio Mode.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_give_system_audio_mode_status(HANDLE_T        h_cecm_svc,
                                                  CECM_LOG_ADDR_T e_la);

/*------------------------------------------------------------------*/
/*! @brief         This API requests the audio format which the device support
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @param [in]    pt_aud_fmt     - Reference the audio format code
 *  @param [in]    b_sync         - Indicates if this is a synchronous operation.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_cecm_request_audio_descriptor(HANDLE_T h_cecm_svc,
                                             CECM_LOG_ADDR_T e_la,
                                             CECM_AUD_FMT_T*   pt_aud_fmt,
                                             BOOL  b_sync);

/* Audio Rate Control Feature ************************************************/
/*------------------------------------------------------------------*/
/*! @brief         This API is used to control audio rate from source device.
 *  @param [in]    h_cecm_svc     - A handle referencing the service.
 *  @param [in]    e_la           - CEC device logical address.
 *  @param [in]    e_aud_rate     - Contains the audio rate info.
 *  @return        Return the error code 
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_cecm_set_audio_rate(HANDLE_T        h_cecm_svc,
                                   CECM_LOG_ADDR_T e_la,
                                   CECM_AUD_RATE_T e_aud_rate);
extern INT32 x_cecm_arc_ctrl(HANDLE_T                  h_cecm_svc,
                                CECM_ARC_CTRL_T*          pt_arc_ctrl);
/* Audio Return Channel Feature***********************************************/
/*------------------------------------------------------------------*/
/*! @brief            This API is used to request initial audio return channel  
 *  @param [in]       h_cecm_svc    - A handle referencing the service. 
 *  @param [in]       e_la          - CEC device logical address.
 *  @param [in]       b_sync         - Indicates if this is a synchronous operation.
 *  @return        Return the error code  
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error. 
 */
/*------------------------------------------------------------------*/
extern INT32 x_cecm_request_init_arc(HANDLE_T h_cecm_svc,
                                     CECM_LOG_ADDR_T  e_la,
                                     BOOL   b_sync);
/*------------------------------------------------------------------*/
/*! @brief            This API is used to request terminate audio return channel  
 *  @param [in]       h_cecm_svc    - A handle referencing the service. 
 *  @param [in]       e_la          - CEC device logical address.
 *  @param [in]       b_sync         - Indicates if this is a synchronous operation.
 *  @return        Return the error code  
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error. 
 */
/*------------------------------------------------------------------*/


extern INT32 x_cecm_request_terminate_arc(HANDLE_T h_cecm_svc, 
                                          CECM_LOG_ADDR_T e_la,
                                          BOOL    b_sync);
/*------------------------------------------------------------------*/
/*! @brief            This API is used to report initial audio return channel  
 *  @param [in]       h_cecm_svc    - A handle referencing the service. 
 *  @param [in]       e_la          - CEC device logical address.
 *  @param [in]       b_sync         - Indicates if this is a synchronous operation.
 *  @return        Return the error code  
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error. 
 */
/*------------------------------------------------------------------*/


extern INT32 x_cecm_report_init_arc(HANDLE_T h_cecm_svc, 
                                    CECM_LOG_ADDR_T  e_la,
                                    BOOL    b_sync);
/*------------------------------------------------------------------*/
/*! @brief            This API is used to report terminate audio return channel  
 *  @param [in]       h_cecm_svc    - A handle referencing the service. 
 *  @param [in]       e_la          - CEC device logical address.
 *  @param [in]       b_sync         - Indicates if this is a synchronous operation.
 *  @return        Return the error code  
 *  @retval        CECMR_OK                - Routine successful.
 *  @retval        CECMR_NOT_INIT          - The CEC Manager is not initialized.
 *  @retval        CECMR_INV_ARG           - One or more of the arguments contain invalid data.
 *  @retval        CECMR_CEC_FCT_DISABLED  - CEC function is disabled.
 *  @retval        CECMR_DEV_NOT_AVIL      - Device is not available.
 *  @retval        CECMR_MSG_SEND_ERROR    - Send message failed.
 *  @retval        CECMR_ERROR             - Internal error. 
 */
/*------------------------------------------------------------------*/


extern INT32 x_cecm_report_terminate_arc(HANDLE_T h_cecm_svc,
                                         CECM_LOG_ADDR_T e_la,
                                         BOOL    b_sync);
                                     

/*----------------------------------------------------------------------------*/
/** @} */                                             
/*----------------------------------------------------------------------------*/

#endif /* _X_CECM_H_ */

