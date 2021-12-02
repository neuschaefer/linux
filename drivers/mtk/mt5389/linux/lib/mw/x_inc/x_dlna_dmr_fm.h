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
/*! @file x_dlna_dmr_fm.h
*  $RCSfile: $
*  $Revision: #1 $
*  $Date: 2012/04/27 $
*  $Author: dtvbm11 $
*  
*  @par Description:
*         This header file contains DLNA DMR file manager exported API.
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


#ifndef X_DLNA_DMR_FM_H
#define X_DLNA_DMR_FM_H

#include "x_fm.h"
#include "u_dlna_dmr.h"

/**     
 * Transport layer API
 */
/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmr_fm_open(CHAR * ps_uri, CHAR * ps_protocol, INT32 *pi4_fd)
*  @brief  Opens a URI looks like a local file
*          - Called while who wants to get a object in DMS
*  @param  [in]ps_uri            - The object URI
*  @param  [in]ps_protocol       - The object protocol info
*  @param  [out]pi4_fd           - The file handle 
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval FMR_OK          - The routine was successful. 
*  @retval FMR_FAIL        - Open object failed.
*  @retval FMR_DEVICE_ERROR       - The DMS error 
*  @retval FMR_NOT_ENOUGH_SPACE   - The memory not enough
*/
/*----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmr_fm_open(CHAR * ps_uri, CHAR * ps_protocol, INT32 *pi4_fd);



/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmr_fm_close(INT32 i4_fd)
*  @brief  Closes the object file handle
*          - Called while who wants to close the file handle
*  @param  [in]i4_fd           - The file handle 
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval FMR_OK          - The routine was successful. 
*/
/*----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmr_fm_close(INT32 i4_fd);


/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmr_fm_read(INT32 i4_fd, VOID *pv_data, UINT32 ui4_count, UINT32 *ui4_read)
*  @brief  Reads the object
*          - Called while who wants to get a object data in DMS
*  @param  [in]i4_fd           - The file handle 
*  @param  [in][out]pv_data    - The data buffer
*  @param  [in]ui4_count       - The buffer size 
*  @param  [out]i4_read        - The reading data size
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval FMR_OK                 - The routine was successful. 
*  @retval FMR_FAIL               - Open object failed.
*  @retval FMR_BUSY               - The DMS too busy to respond
*  @retval FMR_DEVICE_ERROR       - The DMS error 
*  @retval FMR_NOT_ENOUGH_SPACE   - The memory not enough
*  @retval FMR_EOF                - The file has already read to end
*  @retval FMR_NOT_INIT           - The handle may be closed
*  @retval FMR_CMD_TIMEOUT        - Read time out
*  @retval FMR_INVAL              - DRM failed  
*/
/*----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmr_fm_read(INT32 i4_fd, VOID *pv_data, UINT32 ui4_count, UINT32 *ui4_read);



/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmr_fm_read_async(INT32 i4_fd, VOID *pv_data, UINT32 ui4_count, UINT8 ui1_pri, x_fm_async_fct pf_nfy_fct, VOID *pv_tag, INT32 *pi4_req)
*  @brief  Reads the object asynchronism
*          - Called while who wants to get a object data in DMS
*  @param  [in]i4_fd           - The file handle 
*  @param  [in][out]pv_data    - The data buffer
*  @param  [in]ui4_count       - The buffer size 
*  @param  [in]ui1_pri         - The read priority 
*  @param  [in]pf_nfy_fct      - The callback function
*  @param  [in]pv_tag          - the callback tag 
*  @param  [out]pi4_req        - The request handle
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval FMR_OK                 - The routine was successful. 
*  @retval FMR_FAIL               - Open object failed.
*  @retval FMR_BUSY               - The DMS too busy to respond
*  @retval FMR_DEVICE_ERROR       - The DMS error 
*  @retval FMR_NOT_ENOUGH_SPACE   - The memory not enough
*  @retval FMR_EOF                - The file has already read to end
*  @retval FMR_NOT_INIT           - The handle may be closed
*  @retval FMR_CMD_TIMEOUT        - Read time out
*  @retval FMR_INVAL              - DRM failed  
*/
/*----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmr_fm_read_async(INT32 i4_fd, VOID *pv_data, UINT32 ui4_count, UINT8 ui1_pri, x_fm_async_fct pf_nfy_fct, VOID *pv_tag, INT32 *pi4_req);



/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmr_fm_abort_async(INT32 i4_fd)
*  @brief  Aborts the read action asynchronism
*          - Called while who wants to abort the read action
*  @param  [in]i4_fd           - The file request handle 
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval FMR_OK                 - The routine was successful. 
*/
/*----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmr_fm_abort_async(INT32 i4_fd);

/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmr_fm_seek(INT32 i4_fd, INT64 i8_offset, UINT8 ui1_whence, UINT64 *pui8_cur_pos)
*  @brief  Seeks file position
*          - Called while who wants to seek object file
*  @param  [in]i4_fd               - The file handle 
*  @param  [in]i8_offset           - The offset
*  @param  [in]ui1_whence          - The whence    
*  @param  [out]pui8_cur_pos       - The current position 
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval FMR_OK                 - The routine was successful.
*/
/*----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmr_fm_seek(INT32 i4_fd, INT64 i8_offset, UINT8 ui1_whence, UINT64 *pui8_cur_pos);


/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmr_fm_eof(INT32 i4_fd)
*  @brief  Checks the file is read to end.
*          - Called while who wants to check the file handle
*  @param  [in]i4_fd           - The file handle 
*  
*  @note   
*  @see      
*  @return Return the value                
*  @retval 0                 - The file is not EOF.
*  @retval 1                 - The file is EOF 
*/
/*----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmr_fm_eof(INT32 i4_fd);


/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmr_fm_file_size(INT32 i4_fd, UINT64 *pui8_size)
*  @brief  Gets the file size
*          - Called while who wants to get the file size
*  @param  [in]i4_fd               - The file handle    
*  @param  [out]pui8_size          - The file size 
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval FMR_OK                 - The routine was successful.
*/
/*----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmr_fm_file_size(INT32 i4_fd, UINT64 *pui8_size);


/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmr_fm_get_seek_type(INT32 i4_fd, DLNA_SEEK_TYPE_T * pe_type)
*  @brief  Gets the file seek type
*          - Called while who wants to get the file seek type
*  @param  [in]i4_fd               - The file handle    
*  @param  [out]pe_type            - The file seek type 
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval FMR_OK                 - The routine was successful.
*/
/*----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmr_fm_get_seek_type(INT32 i4_fd, DLNA_SEEK_TYPE_T * pe_type);


/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmr_fm_set_wmdrm_session_id(INT32 i4_fd, UINT8 * pui1_session_id, UINT32 ui4_length)
*  @brief  Sets the object WMDRM session id
*          - Called while who wants to set object WMDRM session id
*  @param  [in]i4_fd            - The file handle
*  @param  [in]pui1_session_id  - The WMDRM session id data
*  @param  [in]ui4_length       - the id data length
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval DLNAR_OK          - The routine was successful. 
*/
/*----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmr_fm_set_wmdrm_session_id(INT32 i4_fd, UINT8 * pui1_session_id, UINT32 ui4_length);


/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmr_fm_get_drm_type(INT32 i4_fd, DLNA_DRM_TYPE_T * pe_type)
*  @brief  Gets the object DRM type
*          - Called while who wants to get the device DRM type
*  @param  [in]i4_fd            - The file handle
*  @param  [out]pe_type         - The device DRM type
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval DLNAR_OK          - The routine was successful. 
*/
/*----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmr_fm_get_drm_type(INT32 i4_fd, DLNA_DRM_TYPE_T *pe_type);

#endif

/*! @} */

