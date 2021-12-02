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
/*! @file x_dsm.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *    This header file contains Device Status Manager (DSM) API definitions,
 *    which are exported to other Middleware components and applications.
 *         
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_DSM
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _X_DSM_H_
#define _X_DSM_H_

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "u_dsm.h"

/*-------------------------------------------------------------------------
                    functions declarations
 --------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*!@brief   This function create a handle to the a device interface group. 
 *  @param [in]  t_dev_group  The interface specified is one of the following
 *                                       interface:
 *                                       DEV_1394, DEV_TUNER, DEV_AVC.
 *  @param [in]  pv_tag          Private tag that will be passed to the client's
 *                                       provided notification function. 
 *  @param [in]  pf_nfy_fct     DSM device group notification function.  This function
 *                                       is called when a new device is detected or removed on
 *                                       that  DSM device group.
 *  @param [out]  ph_dsm      Handle to an interface (e.g., a device group)
 *  @note
 *  @see  x_dsm_open
 *  @return  Return the error code
 *  @retval  DSMR_OK                       DSM handle is returned. 
 *  @retval  DSMR_OUT_OF_HANDLE    No more handle available.
 *  @retval  DSMR_OUT_OF_MEM        Not enough memory to create the DSM control structure.
 *  @retval  DSMR_INV_ARG               'ph_dsm' is NULL or 't_dev_group' is not a valid group.
 *  @retval  DSMR_AEE_OUT_OF_RESOURCES  Can not allocate AEE resources.
 *  @retval  DSMR_AEE_NO_RIGHTS     No right for the AEE resources.
 */
/*------------------------------------------------------------------*/
extern  INT32  x_dsm_open
(
	DEVICE_TYPE_T    t_dev_group,
    const CHAR*      ps_name,
    VOID*            pv_tag,
    dsm_dev_nfy_fct  pf_nfy_fct,
    HANDLE_T*		 ph_dsm
);

/*------------------------------------------------------------------*/
/*!@brief   Get information on a device group. 
 *  @param [in]  h_dsm_dev:         handle to device group.
 *  @param [in]  t_get_type    specifies the get operation to be performed on
 *                                       the device group.  The set type is device group specific.  
 *  @param [in]  pv_get_info   optional parameters for the get operation.
 *  @param [in]  pz_size         the size of buffer pointed to by the pv_data. 
 *  @param [out] pz_size       the number of valid bytes in the buffer pointed
 *                                      to by the pv_data. 
 *  @param [out] pv_get_data       the data returned by the get operation.
 *  @note
 *  @see   x_dsm_get
 *  @return  Return the error code
 *  @retval  DSMR_OK           Information about this device is returned
 *  @retval  DSMR_INV_HANDLE   'h_dsm' is a invalid.
 */
/*------------------------------------------------------------------*/
extern  INT32  x_dsm_get
(
    HANDLE_T         h_dsm_dev,
    DSM_GET_T	     t_get_type,
    VOID*            pv_get_info,
    SIZE_T*	         pz_size,
    VOID*            pv_get_data
);

/*------------------------------------------------------------------*/
/*!@brief   Perform set operation on a device group. 
 *  @param [in]  h_dsm_dev    handle to a device group.
 *  @param [in]  t_set_type   specify the set command.	 The set type is 
 *                                      device group specific.  
 *  @param [in]  pv_set_info  Optional parameter for the set command.
 *                    If the 'h_dsm_dev' is a 1394 device group, then
 *                     'pv_set_info' is directly mapped to the 'pv_set_info'
 *                     information required for the FWM (Firewire) set API
 *                    function.
 *
 *                    If the 'h_dsm_dev' is an AVC device group, then this
 *                    argument corresponds to the 'pv_set_info' argument in
 *                    the *(x_rm_set_fct)() function.	
 *  @param [in]  pz_size         the size of buffer pointed to by the pv_set_data. 
 *  @note
 *  @see     x_dsm_set
 *  @return  Return the error code
 *  @retval  DSMR_OK         Number of records found.
 *  @retval  DSMR_INV_HANDLE Bad 'h_dsm' handle.
 *  @retval  DSMR_INV_ARG    'pui2_num_recs' or 'pui4_ver_id' is a NULL pointer.
 */
/*------------------------------------------------------------------*/
extern  INT32  x_dsm_set
(
    HANDLE_T         h_dsm_dev,
    DSM_SET_T        t_set_type,
    VOID*            pv_set_info,
    SIZE_T           z_size
);

/*------------------------------------------------------------------*/
/*!@brief   This function creates a handle to an instance of source device attached to the system.
 *  @param [in]  h_dsm:	    Handle to the DSM interface which this device type belongs to.
 *  @param [in]  t_dev_type    Specifies the device name belonging to this 'h_dsm' device interface .
 *  @param [in]  ps_name        string to pass to the Resource Manager 
 *  @param [in]  ui2_idx           iterate for next instance of the device of the specified device type name.
 *  @param [in]  pv_tag           Private tag that will be passed to the client's provided notification function. 
 *  @param [in]  pf_dev_nfy_fct    DSM device notification function.  This function
 *                                           will be called in the context of the DSM thread to
 *                                           notify application of any change in the status of
 *                                           the attached device.
 *  @param [out]  ph_dsm_dev        handle to an instance of a device.
 *  @note
 *  @see     x_dsm_dev_open
 *  @return  Return the error code
 *  @retval  DSMR_OK             DSM handle is returned. 
 *  @retval  DSMR_OUT_OF_HANDLE    No more handle available.
 *  @retval  DSMR_OUT_OF_MEM        Not enough memory to create the DSM control structure.
 *  @retval  DSMR_INV_ARG               'h_dsm' is NULL_HANDLE, 'ph_dsm_dev' is NULL.
 *  @retval  DSMR_INV_HANDLE     'h_dsm' is wrong handle for the interface group
 *                                             that this device belongs to.
 *  @retval  DSMR_AEE_OUT_OF_RESOURCES  Can not allocate AEE resources.
 *  @retval  DSMR_AEE_NO_RIGHTS No right for the AEE resources.
 */
/*------------------------------------------------------------------*/

extern  INT32  x_dsm_dev_open
(
    HANDLE_T         h_dsm,
    DEVICE_TYPE_T    e_dev_type,
    const CHAR*      ps_name,
    UINT16	         ui2_idx,
    VOID*            pv_tag,
    dsm_dev_nfy_fct  pf_dev_nfy_fct,
    HANDLE_T*        ph_dsm_dev
);

/*------------------------------------------------------------------*/
/*!@brief   Get information on an instance of a specific device.
 *  @param [in]  h_dsm_dev     handle to an instance of device.
 *  @param [in]  t_get_type    specifies the get operation to be performed
 *                                       on the specified device.  This flag is defined by
 *                                       specific device.
 *  @param [in]  pv_get_info   optional parameters for the get operation.
 *  @param [in]  pz_size         the size of buffer pointed to by the pv_data. 
 *  @param [out] pz_size       the number of valid bytes in the buffer pointed
 *                                      to by the pv_data. 
 *  @param [out] pz_size       the number of valid bytes in the buffer pointed
 *                                      to by the pv_data. 
 *  @param [out] pz_sizepv_data       the data returned by the get operation.
 *  @note
 *  @see      x_dsm_dev_get
 *  @return  Return the error code
 *  @retval  DSMR_OK           Data from the specified device is returned. 
 *  @retval  DSMR_INV_HANDLE   ' h_dsm_dev' is invalid.
 */
/*------------------------------------------------------------------*/

extern  INT32  x_dsm_dev_get
(
    HANDLE_T         h_dsm_dev,
    DSM_DEV_GET_T    t_get_type,
    VOID*            pv_get_info,
    SIZE_T*	         pz_size,
    VOID*            pv_get_data
);

/*------------------------------------------------------------------*/
/*!@brief   Perform set operation on an instance of a specified device.
 *  @param [in]  h_dsm_dev     handle to an instance of device.
 *  @param [in]  t_set_type    specify the set command. The set type is
 *                                       device specific.
 *  @param [in]  pv_set_info   Optional parameter for the set command.
 *                  If the 'h_dsm_dev' is a 1394 device, then
 *                  'pv_set_info' is directly mapped to the 
 *                  'pv_set_info' information required for the
 *                   FWM (Firewire) set function.
 *
 *                  If the 'h_dsm_dev' is an AVC device, then this
 *                  argument corresponds to the 'pv_set_info' argument
 *                  in the *(x_rm_set_fct)
 *  @param [in]  pz_size         the size of buffer pointed to by the pv_set_info. 
 *  @note
 *  @see    x_dsm_dev_set
 *  @return  Return the error code
 *  @retval  DSMR_OK         Number of records found.
 *  @retval  DSMR_INV_HANDLE Bad 'h_dsm' handle.
 */
/*------------------------------------------------------------------*/

extern  INT32  x_dsm_dev_set
(
    HANDLE_T         h_dsm_dev,
    DSM_DEV_SET_T    t_set_type,
    VOID*            pv_set_info,
    SIZE_T           z_size
);

/*------------------------------------------------------------------*/
/*! @brief  This function not use currently
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_dsm_stop
(
    VOID
);

#endif /* _X_DSM_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */                                              /* end of groupMW_DSM */
/*----------------------------------------------------------------------------*/

