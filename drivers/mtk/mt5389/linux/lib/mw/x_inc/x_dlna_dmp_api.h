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
/*! @file x_dlna_dmp_api.h 
*  $RCSfile: $
*  $Revision: #1 $
*  $Date: 2012/04/27 $
*  $Author: dtvbm11 $
*  
*  @par Description:
*         This header file contains DLNA DMP exported API.
*/
/*----------------------------------------------------------------------------*/

/*!
* @addtogroup  groupMW_INET_DLNA
* @brief       Provide an external API about DLNA.
* %DLNA API would be used by user who wants to play media stream in local network  
* 
* @see 
*
* @{
* */



#ifndef X_DLNA_DMP_API_H
#define X_DLNA_DMP_API_H

#include "u_dlna_dmp.h"
#include "x_dlna_dmp_fm.h"

/**
 * Utility
 */

/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmp_start(x_dlna_device_nfy pf_nfy, VOID * pv_tag)
*  @brief  Starts the DMP
*          - Called while who wants to start DMP
*  @param  [in]pf_nfy          - The callback while device found or left
*  @param  [in]pv_tag          - The callback tag
*  
*  @note   
*  @see    x_dlna_dmp_stop  
*  @return Return the error code                
*  @retval DLNAR_DMP_OK     	- start dmp core ok. 
*/
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_start
*
* Description: The API to start dmp kernel
*
* Inputs:  pf_nfy - device change notify
*          pv_tag - notify function tag 
*
* Outputs: -
*
* Returns: DLNAR_DMP_OK - start dmp core ok
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmp_start(x_dlna_device_nfy pf_nfy, VOID * pv_tag);


/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmp_suspend(VOID)
*  @brief  Suspends the DMP
*          - Called while who wants to suspend DMP which has already started
*  @param  
*  
*  @note   
*  @see    x_dlna_dmp_resume  
*  @return Return the error code                
*  @retval DLNAR_DMP_OK     	- start dmp core ok.
*  @retval DLNAR_DMP_NOT_START  - dmp core not starting.
*/
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_suspend
*
* Description: Suspend t_local_dmp event manager thread
*
* Inputs:  
*
* Outputs: -
*
* Returns: DLNAR_DMP_NOT_START - dmp core not starting.
*          DLNAR_DMP_OK        - suspend dmp core ok.  
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmp_suspend(VOID);


/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmp_resume(VOID)
*  @brief  Resumes the DMP
*          - Called while who wants to resume DMP which has been suspended
*  @param  
*  
*  @note   
*  @see    x_dlna_dmp_suspend  
*  @return Return the error code                
*  @retval DLNAR_DMP_OK     	   - resume dmp core ok. 
*  @retval DLNAR_DMP_ALREADY_START - the dmp core already started.
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_resume
*
* Description: Wake event manager thread
*
* Inputs:  -
*
* Outputs: -
*
* Returns: DLNAR_DMP_ALREADY_START - the dmp core already started
*          DLNAR_DMP_OK            - start ok 
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmp_resume(VOID);

/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmp_stop(VOID)
*  @brief  Stops the DMP
*          - Called while who wants to stop DMP
*  @param  
*  
*  @note   
*  @see    x_dlna_dmp_start  
*  @return Return the error code                
*  @retval DLNAR_DMP_OK     	- stop dmp core ok. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_stop
*
* Description: The API to close t_local_dmp kernel
*
* Inputs:  -
*
* Outputs: -
*
* Returns: DLNAR_DMP_OK - close dmp ok
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmp_stop(VOID);

/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmp_get_device(DLNA_DMP_DEVICE_INFO_LIST_T * pt_list)
*  @brief  Gets the device list in local network
*          - Called while who wants to get all device in DMP client
*  @param  [out]pt_list      - The device list
*  
*  @note   
*  @see    x_dlna_dmp_stop  
*  @return Return the error code                
*  @retval DLNAR_DMP_OK     	- The routine was successful. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_get_device
*
* Description: Get device list from dmp kernel
*
* Inputs:  -
*
* Outputs: pt_list - the output storage
*
* Returns: DLNAR_DMP_OK - get device list ok
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmp_get_device(DLNA_DMP_DEVICE_INFO_LIST_T * pt_list);


/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmp_search_device(VOID)
*  @brief  Searches the new device in local network
*          - Called while who wants to search new device
*  @param  
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval DLNAR_DMP_OK     	- search ok.
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_search_device
*
* Description: search device in current network 
*
* Inputs:  -
*
* Outputs: -
*
* Returns: DLNAR_DMP_OK - search ok
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmp_search_device(VOID);

/*----------------------------------------------------------------------------*/
/*! @fn    BOOL x_dlna_dmp_check_device_sort_capability(DLNA_DMP_DEVICE_T t_device)
*  @brief  Checks the device has sort capability or not
*          - Called while who wants to know the device sort capability
*  @param  [in]t_device         - The device handle
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval TRUE                 - has sort capability.
*  @retval FALSE                - no sort capability.
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_check_device_sort_capability
*
* Description: The entry point to query device ability
*
* Inputs:  t_device - the device to query ability
*
* Outputs: -
*
* Returns: 
*          TRUE          - has sort capability
*          FALSE         - no sort capability
----------------------------------------------------------------------------*/
extern BOOL x_dlna_dmp_check_device_sort_capability(DLNA_DMP_DEVICE_T t_device);


/*----------------------------------------------------------------------------*/
/*! @fn    BOOL x_dlna_dmp_check_device_search_capability(DLNA_DMP_DEVICE_T t_device)
*  @brief  Checks the device has the search capability or not
*          - Called while who wants to know the device search capability
*  @param  [in]t_device         - The device handle
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval TRUE                 - has search capability.
*  @retval FALSE                - no search capability.
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_check_device_search_capability
*
* Description: The entry point to check device ability
*
* Inputs:  t_device - device to query ability
*
* Outputs: pb_search - the device report ability
* 
* Returns: TRUE  - has search capability
*          FALSE - has not search capability
----------------------------------------------------------------------------*/
extern BOOL x_dlna_dmp_check_device_search_capability(DLNA_DMP_DEVICE_T t_device);


/*----------------------------------------------------------------------------*/
/*! @fn    BOOL x_dlna_dmp_device_has_given_search_capability(DLNA_DMP_DEVICE_T t_device, CHAR * ps_prop)
*  @brief  Checks the device has the given search capability or not
*          - Called while who wants to know the device has given search capability
*  @param  [in]t_device         - The device handle
*  @param  [in]ps_prop          - The given upnp capability 
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval TRUE                 - has given search capability.
*  @retval FALSE                - no given search capability.
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_device_has_given_search_capability
*
* Description: The entry point to check device ability
*
* Inputs:  t_device - device to query ability
*          ps_prop  - upnp prop
*
* Outputs: None - the device report ability
* 
* Returns: TRUE  - has search capability
*          FALSE - has not search capability
----------------------------------------------------------------------------*/
extern BOOL x_dlna_dmp_device_has_given_search_capability(DLNA_DMP_DEVICE_T t_device, CHAR * ps_prop);



/*----------------------------------------------------------------------------*/
/*! @fn    BOOL x_dlna_dmp_device_has_given_sort_capability(DLNA_DMP_DEVICE_T t_device, CHAR * ps_prop)
*  @brief  Checks the device has the given sort capability or not
*          - Called while who wants to know the device has given sort capability
*  @param  [in]t_device         - The device handle
*  @param  [in]ps_prop          - The given upnp capability 
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval TRUE                 - has given sort capability.
*  @retval FALSE                - no given sort capability.
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_device_has_given_sort_capability
*
* Description: The entry point to query device ability
*
* Inputs:  t_device - the device to query ability
*          ps_prop  - upnp prop
*
* Outputs: -
*
* Returns: 
*          TRUE          - has given sort capability
*          FALSE         - no given sort capability
----------------------------------------------------------------------------*/
extern BOOL x_dlna_dmp_device_has_given_sort_capability(DLNA_DMP_DEVICE_T t_device, CHAR * ps_prop);


/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmp_get_device_name(DLNA_DMP_DEVICE_T t_device, CHAR ** pps_name)
*  @brief  Gets the device name
*          - Called while who wants to get the device name
*  @param  [in]t_device         - The device handle
*  @param  [out]pps_name        - The device name
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval DLNAR_DMP_OK          - The routine was successful. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_get_device_name
*
* Description: Get device name
*
* Inputs:  t_device - device to check type
*
* Outputs: pps_name - the device name
*
* Returns: DLNAR_DMP_OK - get name ok
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmp_get_device_name(DLNA_DMP_DEVICE_T t_device, CHAR ** pps_name);



/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmp_get_device_type(DLNA_DMP_DEVICE_T t_device, DLNA_DEVICE_TYPE_T * pe_type)
*  @brief  Gets the device type
*          - Called while who wants to get the device type
*  @param  [in]t_device         - The device handle
*  @param  [out]pe_type         - The device type
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval DLNAR_DMP_OK          - The routine was successful. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_get_device_type
*
* Description: The function to get device type
*
* Inputs:  t_device - the object to get
*
* Outputs: pe_type - the device type
*
* Returns: DLNAR_DMP_OK - get device type ok
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmp_get_device_type(DLNA_DMP_DEVICE_T t_device, DLNA_DEVICE_TYPE_T * pe_type);


/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmp_get_object_drm_type(DLNA_DMP_OBJECT_T t_object, DLNA_DMP_OBJECT_DRM_TYPE_T * pe_type)
*  @brief  Gets the object DRM type
*          - Called while who wants to get the device DRM type
*  @param  [in]t_device         - The device handle
*  @param  [out]pe_type         - The device DRM type
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval DLNAR_DMP_OK          - The routine was successful. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_get_object_drm_type
*
* Description: get object drm type
*
* Inputs:  t_object - the dlna object
*
* Outputs: pe_type - the object drm type
*
* Returns: DLNAR_DMP_OK - get drm type ok
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmp_get_object_drm_type(DLNA_DMP_OBJECT_T t_object, DLNA_DMP_OBJECT_DRM_TYPE_T * pe_type);


/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmp_get_object_profile_info(DLNA_DMP_OBJECT_T t_object, DLNA_DMP_PROFILE_INFO_T ** ppt_mime)
*  @brief  Gets the object profile info
*          - Called while who wants to get the object profile info
*  @param  [in]t_object         - The object handle
*  @param  [out]ppt_mime        - The object profile info
*  
*  @note   
*  @see    x_dlna_dmp_free_object_profile_info  
*  @return Return the error code                
*  @retval DLNAR_DMP_OK          - The routine was successful. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_get_object_profile_info
*
* Description: get object prop profile info
*
* Inputs:  t_object - the dlna object
*
* Outputs: ppt_mime - profile info
*
* Returns: DLNAR_DMP_OBJECT_NOT_EXIST - the object not exist
*          DLNAR_DMP_OK               - get profile inof ok 
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmp_get_object_profile_info(DLNA_DMP_OBJECT_T t_object, DLNA_DMP_PROFILE_INFO_T ** ppt_mime);



/*----------------------------------------------------------------------------*/
/*! @fn    BOOL x_dlna_dmp_check_object_type(DLNA_DMP_OBJECT_T t_object, DLNA_DMP_OBJECT_TYPE_T e_type)
*  @brief  Checks the object is given type or not
*          - Called while who wants to check the object type
*  @param  [in]t_object         - The object handle
*  @param  [in]e_type           - The object type
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval TRUE                 - has given sort capability.
*  @retval FALSE                - no given sort capability. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_check_object_type
*
* Description: check the object type
*
* Inputs:  t_object - the dlna object
*          t_type   - the object drm type
*
* Outputs: -  
*
* Returns: TRUE              if the object type is t_type
*          FALSE             check type is not e_type
----------------------------------------------------------------------------*/
extern BOOL x_dlna_dmp_check_object_type(DLNA_DMP_OBJECT_T t_object, DLNA_DMP_OBJECT_TYPE_T e_type);


/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmp_get_object_prop_info(DLNA_DMP_OBJECT_T t_object, DLNA_DMP_OBJECT_PROP_T e_info, CHAR ** pps_info)
*  @brief  Gets the object property info
*          - Called while who wants to get the object prop info
*  @param  [in]t_object         - The object handle
*  @param  [in]e_info           - The object prop info
*  @param  [out]pps_info        - The object prop info string
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval DLNAR_DMP_OK          - The routine was successful.
*  @retval DLNAR_DMP_INVALID_ARGUMENT - invalid argument.
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_get_object_prop_info
*
* Description: get object prop info
*
* Inputs:  t_object - the dlna object
*          e_info   - which object prop info do you want   
*
* Outputs: pps_info - the object prop info
*
* Returns: DLNAR_DMP_INVALID_ARGUMENT - invalid argument
*          DLNAR_DMP_OK               - get prop info ok 
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmp_get_object_prop_info(DLNA_DMP_OBJECT_T t_object, DLNA_DMP_OBJECT_PROP_T e_info, CHAR ** pps_info);



/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmp_free_object_profile_info(DLNA_DMP_PROFILE_INFO_T ** ppt_mime)
*  @brief  Frees the object profile info
*          - Called while who wants to free the object profile info
*  @param  [in]ppt_mime        - The object profile info
*  
*  @note   
*  @see    x_dlna_dmp_get_object_profile_info  
*  @return Return the error code                
*  @retval DLNAR_DMP_OK          - The routine was successful. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_free_object_profile_info
*
* Description: get object prop profile info
*
* Inputs:  ppt_mime - profile info
*
* Outputs: -
*
* Returns: DLNAR_DMP_OK      - get profile inof ok 
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmp_free_object_profile_info(DLNA_DMP_PROFILE_INFO_T ** ppt_mime);



/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmp_cancel_object(DLNA_DMP_DEVICE_T t_dms, CHAR * ps_object_id, HANDLE_T h_handle)
*  @brief  Cancels the object
*          - Called while who wants to cancel object which action by browse or search operation
*  @param  [in]t_dms            - The DMS handle
*  @param  [in]ps_object_id     - The object id string
*  @param  [in]h_handle         - The action handle
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval DLNAR_DMP_OK          - The routine was successful. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_cancel_object
*
* Description: Cancel dmp action
*
* Inputs:  t_object, the dlna object to cancel
*
* Outputs: -
*
* Returns: DLNAR_DMP_OK - cancel object action ok
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmp_cancel_object(DLNA_DMP_DEVICE_T t_dms,                                         
                                      CHAR * ps_object_id, 
                                      HANDLE_T h_handle);

/*----------------------------------------------------------------------------*/
/*! @fn    INT32  x_dlna_dmp_search_object( DLNA_DMP_DEVICE_T t_device, 
                                            CHAR * ps_container_id,
                                            INT32      i4_start_index, 
                                            INT32      i4_request_cnt, 
                                            CHAR * ps_criteria,
                                            CHAR * ps_filter,
                                            CHAR * ps_sort_crt, 
                                            DLNA_DMP_OBJECT_INFO_T ** ppt_list)
*  @brief  Searches the object
*          - Called while who wants to search object
*  @param  [in]t_device         - The DMS handle
*  @param  [in]ps_container_id  - The object id string
*  @param  [in]i4_start_index   - The start index
*  @param  [in]i4_request_cnt   - The request object count
*  @param  [in]ps_criteria      - The criteria info string which using to match
*  @param  [in]ps_filter        - The filter which using to filter result 
*  @param  [in]ps_sort_crt      - The sort criteria which using to sort the result
*  @param  [out]ppt_list        - The object info
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval DLNAR_DMP_OK                  - The routine was successful. 
*  @retval DLNAR_DMP_NOT_ENOUGH_RESOURCE - The memory allocation failed.
*  @retval DLNAR_DMP_SEARCH_FAILED         - Search failed.
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_search_object
* Description: Search objects including items and containers from
*              given containers within a specified device.
* Inputs:  
*          t_device             - Search in this device.
*          ps_object_id         - identify the container in which search is processed.
*          i4_start_index       - start index
*          i4_request_cnt       - request object count  
*          ps_criteria          - Match string.
*          ps_filter            - Filter for filtering searched results.
*          ps_sort_crt          - Sorting searched results.
* Outputs: 
*           ppt_list            - holding searched objects.
* Returns:
*           DLNAR_DMP_OK          - Successful.
*           DLNAR_DMP_NOT_ENOUGH_RESOURCE - memory allocation failed.
*           DLNAR_DMP_SEARCH_FAILED         - Search failed.
----------------------------------------------------------------------------*/
extern INT32  x_dlna_dmp_search_object( DLNA_DMP_DEVICE_T t_device, 
                                 CHAR * ps_container_id,
                                 INT32      i4_start_index, 
                                 INT32      i4_request_cnt, 
                                 CHAR * ps_criteria,
                                 CHAR * ps_filter,
                                 CHAR * ps_sort_crt, 
                                 DLNA_DMP_OBJECT_INFO_T ** ppt_list);



/*----------------------------------------------------------------------------*/
/*! @fn    INT32  x_dlna_dmp_search_object_async(   DLNA_DMP_DEVICE_T t_device, 
                                                    CHAR * ps_container_id, 
                                                    INT32      i4_start_index, 
                                                    INT32      i4_request_cnt, 
                                                    CHAR * ps_criteria,
                                                    CHAR * ps_filter,
                                                    CHAR * ps_sort_crt,
                                                    x_dlna_dmp_nfy pf_nfy, 
                                                    VOID * pv_tag,
                                                    HANDLE_T * ph_handle)
*  @brief  Searches the object asynchronism
*          - Called while who wants to search object asynchronism
*  @param  [in]t_device         - The DMS handle
*  @param  [in]ps_container_id  - The object id string
*  @param  [in]i4_start_index   - The start index
*  @param  [in]i4_request_cnt   - The request object count
*  @param  [in]ps_criteria      - The criteria info string which using to match
*  @param  [in]ps_filter        - The filter which using to filter result 
*  @param  [in]ps_sort_crt      - The sort criteria which using to sort the result
*  @param  [in]pf_nfy           - The callback function
*  @param  [in]pv_tag           - the callback tag  
*  @param  [out]ph_handle       - The object info
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval DLNAR_DMP_OK                  - The routine was successful. 
*  @retval DLNAR_DMP_NOT_ENOUGH_RESOURCE - The memory allocation failed.
*  @retval DLNAR_DMP_SEARCH_FAILED         - Search failed.
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_search_object_async
* Description: Search objects including items and containers from
*              given containers within a specified device.
* Inputs:  
*          t_device             - Search in this device.
*          ps_object_id         - identify the container in which search is processed.
*          i4_start_index       - start index
*          i4_request_cnt       - request object count  
*          pf_search_nfy        - Callback function when something has happened.
*          ps_criteria          - Match string.
*          ps_filter            - Filter for filtering searched results.
*          ps_sort_crt          - Soring searched results.
*          ph_handle            - use handle to cancel search action  
* Outputs: 
*           -
* Returns:
*           DLNAR_DMP_OK : Successful.
*           DLNAR_DMP_NOT_ENOUGH_RESOURCE - memory allocation failed.
*           DLNAR_DMP_SEARCH_FAILED         - Search failed.
---------------------------------------------------------------------------*/
extern INT32  x_dlna_dmp_search_object_async( DLNA_DMP_DEVICE_T t_device, 
                                       CHAR * ps_container_id, 
                                       INT32      i4_start_index, 
                                       INT32      i4_request_cnt, 
                                       CHAR * ps_criteria,
                                       CHAR * ps_filter,
                                       CHAR * ps_sort_crt,
                                       x_dlna_dmp_nfy pf_nfy, 
                                       VOID * pv_tag,
                                       HANDLE_T * ph_handle);




/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmp_browse_object(  DLNA_DMP_DEVICE_T t_dms,
                                            CHAR * ps_object_id,
                                            DLNA_DMP_BROWSE_FLAG_T e_flag,
                                            INT32      i4_start_index, 
                                            INT32      i4_request_cnt,
                                            CHAR * ps_filter,
                                            CHAR * ps_sort_crt,                               
                                            DLNA_DMP_OBJECT_INFO_T ** ppt_list)
*  @brief  Browses the object
*          - Called while who wants to browse DLNA container or object
*  @param  [in]t_dms            - The DMS handle
*  @param  [in]ps_object_id     - The object id string
*  @param  [in]e_flag           - The browse flag
*  @param  [in]i4_start_index   - The start index
*  @param  [in]i4_request_cnt   - The request object count
*  @param  [in]ps_filter        - The filter which using to filter result 
*  @param  [in]ps_sort_crt      - The sort criteria which using to sort the result
*  @param  [out]ppt_list        - The object info
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval DLNAR_DMP_OK                  - The routine was successful. 
*  @retval DLNAR_DMP_NOT_ENOUGH_RESOURCE - The memory allocation failed.
*  @retval DLNAR_DMP_BROWSE_FAILED       - Browse failed.
*  @retval DLNAR_DMP_OBJECT_NOT_EXIST    - The dms or container not exist. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_browse_object
*
* Description: browse dlna container or object synchronized
*
* Inputs:  t_dms                        - the dms
*          ps_object_id                 - the container id
*          e_flag                       - browse flag
*          i4_start_index               - start index
*          i4_request_cnt               - request object count  
*          ps_sort_crt                  - the sort crt
*          ps_filter                    - Filter for filtering searched results.
*
* Outputs: ppt_list - the object list in current container
*
* Returns: DLNAR_DMP_OBJECT_NOT_EXIST    - the dms or container not exist
*          DLNAR_DMP_NOT_ENOUGH_RESOURCE - not enough resource to browse
*          DLNAR_DMP_BROWSE_FAILED       - browse fail 
*          DLNAR_DMP_OK                  - browse ok
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmp_browse_object(DLNA_DMP_DEVICE_T t_dms,
                                  CHAR * ps_object_id,
                                  DLNA_DMP_BROWSE_FLAG_T e_flag,
                                  INT32      i4_start_index, 
                                  INT32      i4_request_cnt,
                                  CHAR * ps_filter,
                                  CHAR * ps_sort_crt,                               
                                  DLNA_DMP_OBJECT_INFO_T ** ppt_list);


/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmp_browse_object_async(DLNA_DMP_DEVICE_T t_dms,                                         
                                                CHAR * ps_object_id, 
                                                DLNA_DMP_BROWSE_FLAG_T e_flag,
                                                INT32      i4_start_index, 
                                                INT32      i4_request_cnt,
                                                CHAR * ps_filter,
                                                CHAR * ps_sort_crt,                                        
                                                x_dlna_dmp_nfy pf_nfy, 
                                                VOID * pv_tag,
                                                HANDLE_T * ph_handle)
*  @brief  Browses the object asynchronism
*          - Called while who wants to browse container or object asynchronism
*  @param  [in]t_dms            - The DMS handle
*  @param  [in]ps_object_id     - The object id string
*  @param  [in]e_flag           - The browse flag
*  @param  [in]i4_start_index   - The start index
*  @param  [in]i4_request_cnt   - The request object count
*  @param  [in]ps_filter        - The filter which using to filter result 
*  @param  [in]ps_sort_crt      - The sort criteria which using to sort the result
*  @param  [in]pf_nfy           - The callback function
*  @param  [in]pv_tag           - the callback tag  
*  @param  [out]ph_handle       - The object info
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval DLNAR_DMP_OK                  - The routine was successful. 
*  @retval DLNAR_DMP_NOT_ENOUGH_RESOURCE - The memory allocation failed.
*  @retval DLNAR_DMP_BROWSE_FAILED       - Browse failed.
*  @retval DLNAR_DMP_OBJECT_NOT_EXIST    - The dms or container not exist. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_browse_object_async
*
* Description: browse dlna object or container asynchronized
*
* Inputs:  t_dms                        - the dms
*          ps_object_id                 - the container id
*          e_flag                       - browse flag
*          i4_start_index               - start index
*          i4_request_cnt               - request object count  
*          ps_filter                    - Filter for filtering searched results.
*          ps_sort_crt                  - the sort crt
*          pf_nfy                       - the callback function
*          pv_tag                       - the callback tag 
*          ph_handle                    - use handle to cancel search action 
*
* Outputs: -
*
* Returns: DLNAR_DMP_OBJECT_NOT_EXIST    - the dms or container not exist
*          DLNAR_DMP_NOT_ENOUGH_RESOURCE - not enough resource to browse
*          DLNAR_DMP_OK - browse ok
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmp_browse_object_async(DLNA_DMP_DEVICE_T t_dms,                                         
                                        CHAR * ps_object_id, 
                                        DLNA_DMP_BROWSE_FLAG_T e_flag,
                                        INT32      i4_start_index, 
                                        INT32      i4_request_cnt,
                                        CHAR * ps_filter,
                                        CHAR * ps_sort_crt,                                        
                                        x_dlna_dmp_nfy pf_nfy, 
                                        VOID * pv_tag,
                                        HANDLE_T * ph_handle);

/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmp_close_object(DLNA_DMP_OBJECT_INFO_T ** ppt_object)
*  @brief  Closes the object 
*          - Called while who wants to close object
*  @param  [in]ppt_object         - The object info
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval DLNAR_DMP_OK          - The routine was successful. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_close_object
*
* Description: close the dlna container or object
*
* Inputs:  ppt_object - the object container
*
* Outputs: -
*
* Returns: DLNAR_DMP_OK - close container ok
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmp_close_object(DLNA_DMP_OBJECT_INFO_T ** ppt_object);


/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmp_free_object(DLNA_DMP_OBJECT_T t_object)
*  @brief  Frees the object
*          - Called while who wants to free object
*  @param  [in]t_object         - The object handle
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval DLNAR_DMP_OK          - The routine was successful. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_free_object
*
* Description: free one object in object list in object info which return by browse/search 
*
* Inputs:  t_object - the object
*
* Outputs: -
*
* Returns: DLNAR_DMP_OK - close container ok
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmp_free_object(DLNA_DMP_OBJECT_T t_object);



/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmp_get_wire_info(DLNA_DMP_WIRE_INFO_T * pt_wire)
*  @brief  Gets device wire info
*          - Called while who wants to get device wire info (upnp client)
*  @param  [in]pt_wire         - The object wire info
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval DLNAR_DMP_OK          - The routine was successful. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_get_wire_info
*
* Description: get device wire info (upnp client)
*
* Inputs:  ppt_object - the object container
*
* Outputs: -
*
* Returns: DLNAR_DMP_OK - close container ok
*          DLNAR_DMP_NOT_SUPPORT - not support
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmp_get_wire_info(DLNA_DMP_WIRE_INFO_T * pt_wire);


/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmp_set_object_wmdrm_session_id(DLNA_DMP_OBJECT_T t_object, UINT8 * pui1_session_id, UINT32 ui4_length)
*  @brief  Sets the object WMDRM session id
*          - Called while who wants to set object WMDRM session id
*  @param  [in]t_object         - The object info
*  @param  [in]pui1_session_id  - The WMDRM session id data
*  @param  [in]ui4_length       - the id data length
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval DLNAR_DMP_OK          - The routine was successful. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmp_set_object_wmdrm_session_id
*
* Description: set wmdrm session id to object item 
*
* Inputs:  t_object - the object item
*          pui1_session_id - session id
*          ui4_length - session id length
*
* Outputs: -
*
* Returns: DLNAR_DMP_OK - set object session id ok
*          DLNAR_DMP_NOT_SUPPORT - not support
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmp_set_object_wmdrm_session_id(DLNA_DMP_OBJECT_T t_object, UINT8 * pui1_session_id, UINT32 ui4_length);

#endif

/*! @} */

