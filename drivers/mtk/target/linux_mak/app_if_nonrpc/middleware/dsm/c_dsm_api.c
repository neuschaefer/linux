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
 * $RCSfile: dsm_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file contains implementation of the application level Device Status
 *    Manager (DSM) API's.         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "handle/handle.h"
#include "dsm/x_dsm.h"
#include "aee/aee.h"

/*-------------------------------------------------------------------------
                    functions implementation
 --------------------------------------------------------------------------*/


/*----------------------------------------------------------------------
 * Name: c_dsm_open
 *
 * Description: This function create a handle to the a device interface
 *              group. 
 *
 * Inputs:
 *   t_dev_group    The interface specified is one of the following
 *                  interface:
 *                       DEV_1394, DEV_TUNER, DEV_AVC.
 *
 *   pv_tag         Private tag that will be passed to the client's
 *                  provided notification function. 
 *
 *   pf_nfy_fct     DSM device group notification function.  This function
 *                  is called when a new device is detected or removed on
 *                  that  DSM device group.
 *
 * Output:
 *   ph_dsm:	    Handle to an interface (e.g., a device group)
 *
 * Returns: 
 *   DSMR_OK               DSM handle is returned. 
 *   DSMR_OUT_OF_HANDLE    No more handle available.
 *   DSMR_OUT_OF_MEM       Not enough memory to create the DSM control 
 *                         structure.
 *   DSMR_INV_ARG          'ph_dsm' is NULL or 't_dev_group' is not a valid
 *                         group.
 *   DSMR_AEE_OUT_OF_RESOURCES  Can not allocate AEE resources.
 *   DSMR_AEE_NO_RIGHTS No right for the AEE resources.
 -----------------------------------------------------------------------*/
INT32  c_dsm_open
(
    DEVICE_TYPE_T       t_dev_group,
    const CHAR*         ps_name,
    VOID*               pv_tag,
    dsm_dev_nfy_fct     pf_nfy_fct,
    HANDLE_T*           ph_dsm
) 
{
    HANDLE_T            h_aux;
    INT32               i4_rc;

    i4_rc = aee_grab_handle_resource(AEE_FLAG,
                                     1,
                                     &h_aux);

    switch (i4_rc)
    {
        case AEER_OK:
        {
            i4_rc = x_dsm_open(t_dev_group, ps_name, pv_tag, pf_nfy_fct,
                               ph_dsm) ;

            if (i4_rc == DSMR_OK)
            {
                handle_link_to_aux(h_aux, *ph_dsm);
            }
            else
            {
                aee_release_handle_resource(1);
            }
        }
        break;

        case AEER_OUT_OF_RESOURCES:
        {
            i4_rc = DSMR_AEE_OUT_OF_RESOURCES;
        }
        break;

        default:
        {
            i4_rc = DSMR_AEE_NO_RIGHTS;
        }
        break;
    }
        
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: c_dsm_dev_open
 *
 * Description: This function creates a handle to an instance of source 
 *              device attached to the system.
 *
 * Inputs: 
 *   h_dsm:	           Handle to the DSM interface which this device
 *                     type belongs to.
 *
 *   t_dev_type        Specifies the device name belonging to this
 *                     'h_dsm' device interface .
 *
 *   ps_name           string to pass to the Resource Manager 
 *
 *   ui2_idx           iterate for next instance of the device of the
 *                     specified device type name.
 *
 *   pv_tag            Private tag that will be passed to the client's
 *                     provided notification function. 
 *
 *   pf_dev_nfy_fct    DSM device notification function.  This function
 *                     will be called in the context of the DSM thread to
 *                     notify application of any change in the status of
 *                     the attached device.
 *
 * Outputs:
 *   ph_dsm_dev        handle to an instance of a device.
 *
 * Returns: 
 *   DSMR_OK             DSM handle is returned. 
 *   DSMR_OUT_OF_HANDLE  No more handle available.
 *   DSMR_OUT_OF_MEM     Not enough memory to create the DSM handle structure.
 *   DSMR_DEV_TYPE_NOT_SUPPORTED  The specified DSM name is not found.
 *   DSMR_INV_ARG        'h_dsm' is NULL_HANDLE, 'ph_dsm_dev' is NULL.
 *   DSMR_INV_HANDLE     'h_dsm' is wrong handle for the interface group
 *                       that this device belongs to.
 *   DSMR_AEE_OUT_OF_RESOURCES  Can not allocate AEE resources.
 *   DSMR_AEE_NO_RIGHTS No right for the AEE resources.
 -----------------------------------------------------------------------*/
INT32  c_dsm_dev_open 
(
    HANDLE_T         h_dsm,
    DEVICE_TYPE_T    t_dev_type,
    const CHAR*      ps_name,
    UINT16			 ui2_idx,
    VOID*            pv_tag,
    dsm_dev_nfy_fct  pf_dev_nfy_fct,
    HANDLE_T*        ph_dsm_dev	
) 
{
    HANDLE_T         h_aux;
    INT32            i4_rc;

    i4_rc = aee_grab_handle_resource(AEE_FLAG,
                                     1,
                                     &h_aux);

    switch (i4_rc)
    {
        case AEER_OK:
        {
            i4_rc = x_dsm_dev_open(h_dsm, t_dev_type, ps_name, ui2_idx,
                                   pv_tag, pf_dev_nfy_fct, ph_dsm_dev) ;

            if (i4_rc == DSMR_OK)
            {
                handle_link_to_aux(h_aux, *ph_dsm_dev);
            }
            else
            {
                aee_release_handle_resource(1);
            }
        }
        break;

        case AEER_OUT_OF_RESOURCES:
        {
            i4_rc = DSMR_AEE_OUT_OF_RESOURCES;
        }
        break;

        default:
        {
            i4_rc = DSMR_AEE_NO_RIGHTS;
        }
        break;
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: c_dsm_get
 *
 * Description: Get information on a device group.
 * 
 * Input:
 *
 *    h_dsm:        handle to device group.
 *
 *    t_get_type    specifies the get operation to be performed on
 *                  the device group.  The set type is device group
 *                  specific.  
 *
 *
 *    pv_get_info   optional parameters for the get operation.
 *
 *    pz_size       the size of buffer pointed to by the pv_data. 
 *
 * Output:
 *
 *    pz_size       the number of valid bytes in the buffer pointed
 *                  to by the pv_data. 
 *
 *    pv_data       the data returned by the get operation.
 *
 * Returns: 
 *   DSMR_OK           Information about this device is returned
 *   DSMR_INV_HANDLE   'h_dsm' is a invalid.
 *   
 -----------------------------------------------------------------------*/
INT32  c_dsm_get
(
    HANDLE_T         h_dsm,
    DSM_GET_T  		 t_get_type,
    VOID*            pv_get_info,
    SIZE_T*		     pz_size,
    VOID*            pv_data
)
{ 
    return x_dsm_get(h_dsm,t_get_type,pv_get_info,pz_size,pv_data);     
}

/*----------------------------------------------------------------------
 * Name: c_dsm_dev_get
 *
 * Description: Get information on an instance of a specific device.
 * 
 * Input:
 *
 *    h_dsm_dev     handle to an instance of device.
 *
 *    t_get_type    specifies the get operation to be performed
 *                  on the specified device.  This flag is defined by
 *                  specific device.
 *
 *    pv_get_info   optional parameters for the get operation.
 *
 *    pz_size       yhe size of buffer pointed to by the pv_data. 
 *
 * Output:
 *
 *    pz_size       the number of valid bytes in the buffer pointed
 *                  to by the pv_data. 
 *
 *    pv_data       the data returned by the get operation.
 *
 * Returns: 
 *   DSMR_OK           Data from the specified device is returned. 
 *   DSMR_INV_HANDLE   ' h_dsm_dev' is invalid.
 *   
 -----------------------------------------------------------------------*/
INT32  c_dsm_dev_get 
(
    HANDLE_T         h_dsm_dev,
    DSM_DEV_GET_T    t_get_type,
    VOID*            pv_get_info,
    SIZE_T*		     pz_size,
    VOID*            pv_data
)
{
    return x_dsm_dev_get(h_dsm_dev, t_get_type, pv_get_info, pz_size, pv_data);
}

/*----------------------------------------------------------------------
 * Name: c_dsm_set
 *
 * Description: Perform set operation on a device group.
 *		
 * Input:
 *
 *    h_dsm_dev    handle to a device group.
 *
 *    t_set_type   specify the set command.	 The set type is 
 *                 device group specific.  
 *
 *    pv_set_info  Optional parameter for the set command.
 *                 If the 'h_dsm_dev' is a 1394 device group, then
 *                 'pv_set_info' is directly mapped to the 'pv_set_info'
 *                 information required for the FWM (Firewire) set API
 *                 function.
 *
 *                 If the 'h_dsm_dev' is an AVC device group, then this
 *                 argument corresponds to the 'pv_set_info' argument in
 *                 the *(x_rm_set_fct)() function.				 
 *
 *    z_size:      
 *
 * Returns: 
 *   DSMR_OK         Number of records found.
 *   DSMR_INV_HANDLE Bad 'h_dsm' handle.
 *   DSMR_INV_ARG    'pui2_num_recs' or 'pui4_ver_id' is a NULL pointer.
 *   
 -----------------------------------------------------------------------*/
INT32  c_dsm_set
(
    HANDLE_T         h_dsm_dev,
    DSM_SET_T        t_set_type,
    VOID*            pv_set_info,
    SIZE_T           z_size
)
{
    return x_dsm_set(h_dsm_dev, t_set_type, pv_set_info, z_size);
}

/*----------------------------------------------------------------------
 * Name: c_dsm_dev_set
 *
 * Description: Perform set operation on an instance of a specified device.
 *	
 * Input:
 *
 *    h_dsm_dev     handle to an instance of device.
 *
 *    t_set_type    specify the set command. The set type is
 *                  device specific.
 *    pv_set_info   Optional parameter for the set command.
 *                  If the 'h_dsm_dev' is a 1394 device, then
 *                  'pv_set_info' is directly mapped to the 
 *                  'pv_set_info' information required for the
 *                   FWM (Firewire) set function.
 *
 *                  If the 'h_dsm_dev' is an AVC device, then this
 *                  argument corresponds to the 'pv_set_info' argument
 *                  in the *(x_rm_set_fct)
 *
 *    pv_data       Optional data for the set operation.
 *                  If 'h_dsm_dev' is a 1394 device, then this
 *                  parameter contains addition data for the set o
 *                  peration.
 *                  If 'h_dsm_dev' is an AVC device or Tuner device, 
 *                  then this argument is the size (length in byte) of 
 *                  the buffer referenced by the argument 'pv_set_info'.  
 *
 * Returns: 
 *   DSMR_OK           
 *   DSMR_INV_HANDLE
 -----------------------------------------------------------------------*/
INT32  c_dsm_dev_set
(
    HANDLE_T         h_dsm_dev,
    DSM_DEV_SET_T    t_set_type,
    VOID*            pv_set_info,
    SIZE_T           z_size
)
{
    return x_dsm_dev_set(h_dsm_dev, t_set_type, pv_set_info, z_size);
}

