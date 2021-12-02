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
/*! @file x_dlna_media_type.h
*  $RCSfile: $
*  $Revision: #1 $
*  $Date: 2012/04/27 $
*  $Author: dtvbm11 $
*  
*  @par Description:
*         This header file contains DLNA media type exported API.
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


#ifndef X_DLNA_MEDIA_TYPE_H
#define X_DLNA_MEDIA_TYPE_H

#include "u_dlna_media_type.h"
#include "u_dlna_dmp.h"


/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_get_media_type_by_profile_id(CHAR * ps_profile_id, CHAR ** pps_mime_type)
*  @brief  Gets the media type by profile ID
*          - Called while who wants to Get the media type by profile ID
*  @param  [in]ps_profile_id          - The profile id string
*  @param  [out]pps_mime_type         - The mime type string
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval DLNAR_OK     	   - Get type ok. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_get_media_type_by_profile_id
*
* Description: get object media type by profile id
*
* Inputs:  ps_profile_id - the dlna object media type profile id
*
* Outputs: pps_mime_type - object mime type, 
*                this type may include many mime types the space token is ";", 
*                such as the profile "HEAAC_L4", the media type is "audio/mp4;audio/3gpp;audio/vnd.dlna.adts"
*
*
* Returns: DLNA_INVALID_ARGUMENT - the argument is invalid
*          DLNAR_OK               - get profile inof ok 
----------------------------------------------------------------------------*/
extern INT32 x_dlna_get_media_type_by_profile_id(CHAR * ps_profile_id, CHAR ** pps_mime_type);



/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_get_mime_type(  DLNA_DMP_PROFILE_INFO_T * pt_profile_info, 
                                        DLNA_DMP_OBJECT_DRM_TYPE_T e_type, 
                                        CHAR ** pps_mime_type)
*  @brief  Gets the MIME type by profile info and DRM type
*          - Called while who wants to Get the MIME type
*  @param  [in]pt_profile_info        - The profile info
*  @param  [in]e_type                 - The DRM type
*  @param  [in]pps_mime_type          - The mime type string
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval DLNAR_OK     	   - Get type ok. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_get_mime_type
*
* Description: get object media type by profile id
*
* Inputs:  pt_profile_info - the dlna object media type profile
*          e_type          - the object drm type, this type returned by x_dlna_dmp_get_object_drm_type 
*
* Outputs: pps_mime_type - object mime type
*
* Returns: DLNAR_INVALID_ARGUMENT - the argument is invalid
*          DLNAR_OK               - get profile info ok 
----------------------------------------------------------------------------*/
extern INT32 x_dlna_get_mime_type(DLNA_DMP_PROFILE_INFO_T * pt_profile_info, 
                           DLNA_DMP_OBJECT_DRM_TYPE_T e_type, 
                           CHAR ** pps_mime_type);
    


/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_get_media_type_extension(CHAR* ps_protocol, CHAR ** pps_ext_name)
*  @brief  Gets the media type extension
*          - Called while who wants to Get the media type extension
*  @param  [in]ps_protocol           - The protocol string
*  @param  [in]pps_ext_name          - The media type extension string
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval DLNAR_OK     	   - Get type extension ok. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_get_media_type_extension
*
* Description: get object media type by profile string
*
* Inputs:  ps_protocol  - the dlna media protocol info string
*         
* Outputs: pps_ext_name - media extensions name
*
* Returns: DLNAR_OK               - get profile info ok 
----------------------------------------------------------------------------*/                       
extern INT32 x_dlna_get_media_type_extension(CHAR* ps_protocol, CHAR ** pps_ext_name);

#endif

/*! @} */
