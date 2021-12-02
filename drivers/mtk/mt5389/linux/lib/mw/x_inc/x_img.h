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
/*! @file x_img.h
 *  $RCSfile: $
 *  $Revision: #4 $
 *  $Date: 2012/08/28 $
 *  $Author: xiuqin.wang $
 *
 *  @par Description:
 *         This file exports all image library API and constant for
 *         other module using.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_IMG
 *  @{
 */
/*----------------------------------------------------------------------------*/

#ifndef _X_IMG_H_
#define _X_IMG_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_handle.h"
#include "u_gl.h"
#include "u_img.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
#if 0
/*------------------------------------------------------------------*/
/*! @brief      x_img_nfy_fct
 *                  This function is the callback function of image library.
 *  @param [in] h_handle    Specifies which handle will be notified.
 *  @param [in] pv_tag      The client data specified by the client when callback 
 *							has been called, the callback function will send it 
 *							again..
 *  @param [in] e_event     It identifies the event type of notification.
 *  @param [in] pv_data     Specified the notification data which is followed 
 *							with the notification event.
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_img_nfy_fct) ( 
	HANDLE_T                h_handle,  
    const VOID*             pv_tag,    
    IMG_NFY_EVENT_TYPE_T    e_event,   
    const VOID*             pv_data);  
#endif
/* This function will open an image file, parse the image header, and return the 
image handle. */
/*------------------------------------------------------------------*/
/*! @brief  x_img_fopen
 *             	This function will open an image file, parse the image header, 
 *				and return the image handle.
 *  @param [in] ps_name         Specifies the file name string which indicates 
 *								the image file location, and it is not case 
 *								sensitive.
 *                                           
 *  @param [in] ui1_open_mode   Specifies the open mode of the open operation 
 *								such as IMG_READ_FILE_MODE.
 *  @param [in] pf_nfy          Specified the callback function to receive the 
 *								notification of image library.
 *  @param [in] pv_tag          The client data specified by the client and the 
 *								callback function will send it again.
 *  @param [out] ph_img      	Specified the image handle which retrieve image 
 *								information about the file.
 *  @return Return the error code.
 *  @retval IMGR_OK                     Success. The image file opening is successful.
 *  @retval IMGR_FILE_OR_DIR_MISSING    FAIL. The file name or the path is not valid.
 *  @retval IMGR_OUT_OF_HANDLE          FAIL. The system runs out of handles.
 *  @retval IMGR_FCT_NOT_SUPPORT        FAIL. This function is not supported.
 *  @retval IMGR_IMG_NOT_SUPPORT        FAIL. This image has no engine to decode.
 *  @retval IMGR_INV_ARG                FAIL. Invalid arguments.
 *  @retval IMGR_OUT_OF_MEMORY          FAIL. The system is out of memory.
 *  @retval IMGR_FILE_ERR               FAIL. File system operation has error.
 *  @retval IMGR_INTERNAL_ERR           FAIL. The image decoder occurs error during 
 *										parsing the header.
 */
/*------------------------------------------------------------------*/
extern INT32 x_img_fopen (  
	const CHAR*     ps_name,        
	UINT8           ui1_open_mode,  
	x_img_nfy_fct   pf_nfy,        
	const VOID*     pv_tag,         
	HANDLE_T*       ph_img);       


/*------------------------------------------------------------------*/
/*! @brief  x_img_mopen
 *              This function will open an image memory block, parse the image 
 *				header, and return the image handle.
 *  @param [in] pv_mem_data     Specified the memory address which contain whole 
 *								image data.
 *  @param [in] z_mem_leng      Specified the length of memory data pv_memory_data.
 *  @param [in] ps_format       Specifies what decoder will be invoked to decode 
 *								the image.
 *  @param [in] pf_nfy          Specified the callback function to receive the 
 *								notification of image library.
 *  @param [in] pv_tag          The client data specified by the client and the 
 *								callback function will send it again.
 *  @param [out] ph_img      	Specified the image handle which retrieve image 
 *								information about the file.
 *  @return Return the error code.
 *  @retval IMGR_OK                     Success. The image file opening is successful.
 *  @retval IMGR_OUT_OF_HANDLE          FAIL. The system runs out of handles.
 *  @retval IMGR_FCT_NOT_SUPPORT        FAIL. This function is not supported.
 *  @retval IMGR_INV_ARG                FAIL. Invalid arguments.
 *  @retval IMGR_IMG_NOT_SUPPORT        FAIL. This image has no engine to decode.
 *  @retval IMGR_OUT_OF_MEMORY          FAIL. The system is out of memory.
 *  @retval IMGR_INTERNAL_ERR           FAIL. The image decoder occurs error during 
 *										parsing the header.
 */
/*------------------------------------------------------------------*/
extern INT32 x_img_mopen ( 
	const VOID*     pv_mem_data, 
	SIZE_T          z_mem_leng,   
	const CHAR*     ps_format,    
	x_img_nfy_fct   pf_nfy,     
	const VOID*     pv_tag,      
	HANDLE_T*       ph_img);      

/*------------------------------------------------------------------*/
/*! @brief  x_img_close
 *             After all image library's operation, call this function to close
 *             the image handle, and free all resource, which is occupied by the 
 *				image handle.
 *  @param [in] h_img   Specified the already opened image handle.
 *  @return Return the error code.
 *  @retval IMGR_OK                 Success. The image file opening is successful.
 *  @retval IMGR_INV_HANDLE         FAIL. The image handle is invalid.
 *  @retval IMGR_FCT_NOT_SUPPORT    FAIL. This function is not supported.
 *  @retval IMGR_INV_ARG            FAIL. Invalid arguments.
 *  @retval IMGR_INTERNAL_ERR       FAIL. semaphore lock is failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_img_close (
	HANDLE_T	h_img);    

/* This function will retrieve the width and height of the primary image or 
thumbnail of the image handle */
/*------------------------------------------------------------------*/
/*! @brief   x_img_get_dimension
 *              This function will retrieve the width and height of the primary 
 *				image or thumbnail of the image handle
 *  @param [in] h_img   Specified the already opened image handle.
 *  @param [in] e_type  Specifies what type of dimension want to get.
 *  @param [out] pz_width    Retrieve the width of the image.
 *  @param [out] pz_height   Retrieve the height of the image.
 *  @return Return the error code.
 *  @retval IMGR_OK                 Success. The API execution is successful.
 *  @retval IMGR_INV_HANDLE         FAIL. The image handle is invalid.
 *  @retval IMGR_INV_ARG            FAIL. The pz_width or pz_height is NULL.
 *  @retval IMGR_FCT_NOT_SUPPORT    FAIL. This function is not supported.
 */
/*------------------------------------------------------------------*/
extern INT32 x_img_get_dimension (	
	HANDLE_T         h_img,      
	IMG_INTRA_TYPE_T e_type,     
	SIZE_T*          pz_width,   
	SIZE_T*          pz_height); 

/* This function can stop these types IMG_STOP_OP_TYPE_T of image operation. */
/*------------------------------------------------------------------*/
/*! @brief      x_img_stop
 *     This function can stop these types IMG_STOP_OP_TYPE_T of image operation.
 *  @param [in] h_img       Specified the image handle that want to stop.
 *  @param [in] e_stop_type Specified the type of stop operation.
 *  @return Return the error code.
 *  @retval IMGR_OK                 Success. The API execution is successful.
 *  @retval IMGR_INV_ARG            FAIL. Invalid arguments.
 *  @retval IMGR_INV_HANDLE         FAIL. The image handle is invalid.
 *  @retval IMGR_FCT_NOT_SUPPORT    FAIL. This function is not supported.
 *  @retval IMGR_INTERNAL_ERR       FAIL. semaphore lock is failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_img_stop (  	
	HANDLE_T            h_img,           
    IMG_STOP_OP_TYPE_T  e_stop_type);    

/* This API will start decoding the primary image or thumbnail to the target 
surface h_dst_surf, and it will not return until decoding image is complete. */
/*------------------------------------------------------------------*/
/*! @brief   x_img_sync_load
 *     This API will start decoding the primary image or thumbnail to the target 
 *	surface h_dst_surf, and it will not return until decoding image is complete.
 *  @param [in] h_img           Specified the image handle that want to decode.
 *  @param [in] e_type          Specified the type of loading source image.
 *  @param [in] pv_type_data    Reserve field for GIF file format, and set it 
 *								NULL now.
 *  @param [in] e_rotate_type   Specified the rotation type of this loading 
 *								operation.
 *  @param [in] ui4_src_x       Specified the x offset value to the origin of 
 *								source rectangle on the primary image. It is no 
 *								use in thumbnail loading.
 *  @param [in] ui4_src_y       Specified the y offset value to the origin of 
 *								source rectangle on the primary image. It is no 
 *								use in thumbnail loading.
 *  @param [in] ui4_src_w       Specified the width of source rectangle on the 
 *								primary image. It is no use in thumbnail loading.
 *  @param [in] ui4_src_h       Specified the height of source rectangle on the 
 *								primary image. It is no use in thumbnail loading.
 *  @param [in] ui4_dst_x       Specified the x offset value to the origin of 
 *								destination rectangle on the target surface.
 *  @param [in] ui4_dst_y       Specified the y offset value to the origin of 
 *								destination rectangle on the target surface.
 *  @param [in] ui4_dst_w       Specified the width of destination rectangle on 
 *								the target surface.
 *  @param [in] ui4_dst_h       Specified the height of destination rectangle on 
 *								the target surface.
 *  @param [in] h_dst_surf      Specified the destination surface, which the 
 *								image will draw on it.
 *  @param [in][out] h_dst_surf     Specified the destination surface, which the 
 *									image will draw on it.
 *  @return Return the error code.
 *  @retval IMGR_OK                     Success. The API execution is successful.
 *  @retval IMGR_INV_ARG                FAIL. Invalid arguments.
 *  @retval IMGR_INV_HANDLE             FAIL. The image handle is invalid.
 *  @retval IMGR_FCT_NOT_SUPPORT        FAIL. This function is not supported.
 *  @retval IMGR_INTERNAL_ERR           FAIL. Semaphore lock is failed or 
 *										decoding error.
 *  @retval IMGR_IMG_NOT_SUPPORT        FAIL. This image does not have thumbnail.
 *  @retval IMGR_FILE_OR_DIR_MISSING    FAIL. The file path is invalid.
 *  @retval IMGR_REENTRY_LOADING        FAIL. Loading operation of image decoder 
 *										is reentrant.
 *  @retval IMGR_OUT_OF_MEMORY          FAIL. The system is out of memory.
 *  @retval IMGR_ABORT_LOADING          FAIL. Loading operation is aborted.
 *  @retval IMGR_FILE_ERR               FAIL. The file system has error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_img_sync_load ( 
	HANDLE_T             h_img,        
	IMG_INTRA_TYPE_T     e_type,       
	IMG_TYPE_DATA_T*     pt_type_data, 
	IMG_ROTATE_OP_TYPE_T e_rotate_type,
	UINT32               ui4_src_x,    
	UINT32               ui4_src_y,   
	UINT32               ui4_src_w,    
	UINT32               ui4_src_h,    
	UINT32               ui4_dst_x,    
	UINT32               ui4_dst_y,    
	UINT32               ui4_dst_w,    
	UINT32               ui4_dst_h,    
	GL_HSURFACE_T        h_dst_surf);   

/* This API will send a message to the image decoding thread and then return to 
caller.  */
/*------------------------------------------------------------------*/
/*! @brief   x_img_async_load
 *               This API will send a message to the image decoding thread and 
 *				then return to caller.
 *  @param [in] h_img           Specified the image handle that want to decode.
 *  @param [in] e_type          Specified the type of loading source image.
 *  @param [in] pv_type_data    Reserve field for GIF file format, and set it 
 *								NULL now.
 *  @param [in] e_rotate_type   Specified the rotation type of this loading 
 *								operation.
 *  @param [in] ui4_src_x       Specified the x offset value to the origin of 
 *								source rectangle on the primary image. It is no 
 *								use in thumbnail loading.
 *  @param [in] ui4_src_y       Specified the y offset value to the origin of 
 *								source rectangle on the primary image. It is no 
 *								use in thumbnail loading.
 *  @param [in] ui4_src_w       Specified the width of source rectangle on the 
 *								primary image. It is no use in thumbnail loading.
 *  @param [in] ui4_src_h       Specified the height of source rectangle on the 
 *								primary image. It is no use in thumbnail loading.
 *  @param [in] ui4_dst_x       Specified the x offset value to the origin of 
 *								destination rectangle on the target surface.
 *  @param [in] ui4_dst_y       Specified the y offset value to the origin of 
 *								destination rectangle on the target surface.
 *  @param [in] ui4_dst_w       Specified the width of destination rectangle on 
 *								the target surface.
 *  @param [in] ui4_dst_h       Specified the height of destination rectangle on 
 *								the target surface.
 *  @param [in] h_dst_surf      Specified the destination surface, which the 
 *								image will draw on it.
 *  @param [in][out] h_dst_surf     Specified the destination surface, which 
 *									the image will draw on it.
 *  @return Return the error code.
 *  @retval IMGR_OK                    Success. The API execution is successful.
 *  @retval IMGR_INV_HANDLE            FAIL. The image handle is invalid.
 */
/*------------------------------------------------------------------*/
extern INT32 x_img_async_load (	
	HANDLE_T             h_img,        
	IMG_INTRA_TYPE_T     e_type,       
	IMG_TYPE_DATA_T*     pt_type_data,
	IMG_ROTATE_OP_TYPE_T e_rotate_type,
	UINT32               ui4_src_x,    
	UINT32               ui4_src_y,      
	UINT32               ui4_src_w,   
	UINT32               ui4_src_h,     
	UINT32               ui4_dst_x,     
	UINT32               ui4_dst_y,  
	UINT32               ui4_dst_w,    
	UINT32               ui4_dst_h,   
	GL_HSURFACE_T        h_dst_surf); 

/* This function will get the length of the IMG_INFO_TYPE_T type information 
about the image and information is except the height and width. */
/*------------------------------------------------------------------*/
/*! @brief   x_img_get_info_len
 *           	This function will get the length of the IMG_INFO_TYPE_T type 
 *				information about the image and information is except the height 
 *				and width.
 *  @param [in] h_img           Specified the image handle to retrieve the 
 *								information length.
 *  @param [in] e_info_type     Specified the type of information that want to 
 *								retrieve the information.
 *  @param [in] pv_type_data    Reserve field for GIF file format, and set it 
 *								NULL now.
 *  @param [out] pui4_len      Retrieve the information length.
 *  @return Return the error code.
 *  @retval IMGR_OK                     Success. The API execution is successful.
 *  @retval IMGR_INV_ARG                FAIL. Invalid arguments.
 *  @retval IMGR_INV_HANDLE             FAIL. The image handle is invalid.
 *  @retval IMGR_FCT_NOT_SUPPORT        FAIL. This function is not supported.
 */
/*------------------------------------------------------------------*/
extern INT32 x_img_get_info_len (  
	HANDLE_T        h_img,         
	IMG_INFO_TYPE_T e_info_type,    
	const VOID*     pv_type_data,  
	UINT32*         pui4_len);     

/* This function will get the IMG_INFO_TYPE_T type information about the image 
and information is except the height and width. */
/*------------------------------------------------------------------*/
/*! @brief  x_img_get_info
 *          This function will get the IMG_INFO_TYPE_T type information about 
 *			the image and information is except the height and width.
 *  @param [in] h_img           Specified the image handle to retrieve the 
 *								information length.
 *  @param [in] e_info_type     Specified the type of information that want to 
 *								retrieve the information.
 *  @param [in] pv_type_data    Reserve field for GIF file format, and set it 
 *								NULL now.
 *  @param [in][out] pv_info    Specified the length of pv_info buffer size./
 *								Specified the really size that the information 
 *								data used.
 *  @param [in][out] pui4_len   Specified the memory address which contains the 
 *								retrieved information./Specified the memory 
 *								which contain the information data.
 *  @return Return the error code.
 *  @retval IMGR_OK                     Success. The API execution is successful.
 *  @retval IMGR_INV_ARG                FAIL. Invalid arguments.
 *  @retval IMGR_INV_HANDLE             FAIL. The image handle is invalid.
 *  @retval IMGR_FCT_NOT_SUPPORT        FAIL. This function is not supported.
 */
/*------------------------------------------------------------------*/
extern INT32 x_img_get_info (	
	HANDLE_T        h_img,        
	IMG_INFO_TYPE_T e_info_type,    
	const VOID*     pv_type_data,    
	VOID*           pv_info,  
	UINT32*         pui4_len);    

/* This function will get the length of each EXIF field. */
/*------------------------------------------------------------------*/
/*! @brief  x_img_get_exif_info_len
 *              This function will get the length of each EXIF field.
 *  @param [in] h_img           Specified the memory address which contains the 
 *								retrieved information. the image handle to 
 *								retrieve the information length.
 *  @param [in] e_info_type     Specified the memory address which contains the 
 *								retrieved information. primary of thumbnail 
 *								information to be gotten.
 *  @param [in] at_info         Specified the memory address which contains the 
 *								retrieved information. the array containing the 
 *								EXIF information.
 *  @param [in] ui4_count       Specified the memory address which contains the 
 *								retrieved information. the count of the pat_info 
 *								array.
 *  @return Return the error code.
 *  @retval IMGR_OK                     Success. The API execution is successful.
 *  @retval IMGR_INV_ARG                FAIL. Invalid arguments.
 *  @retval IMGR_INV_HANDLE             FAIL. The image handle is invalid.
 *  @retval IMGR_FCT_NOT_SUPPORT        FAIL. This function is not supported.
 *  @retval IMGR_NO_EXIF_DATA           No EXIF data.
 */
/*------------------------------------------------------------------*/
extern INT32 x_img_get_exif_info_len(
    HANDLE_T            h_img,
    IMG_INTRA_TYPE_T    e_info_type,
    EXIF_INFO_T         *at_info,
    UINT32              ui4_count);

/* This function will get the data of each EXIF field. */
/*------------------------------------------------------------------*/
/*! @brief   x_img_get_exif_info
 *               This function will get the data of each EXIF field.
 *  @param [in] h_img           Specified the memory address which contains the 
 *								retrieved information. the image handle to 
 *								retrieve the information length.
 *  @param [in] e_info_type     Specified the memory address which contains the 
 *								retrieved information. primary or thumbnail 
 *								information to be gotten.
 *  @param [in] at_info         Specified the memory address which contains the 
 *								retrieved information. the array containing the 
 *								EXIF information.
 *  @param [in] ui4_count       Specified the memory address which contains the 
 *								retrieved information. the count of the pat_info 
 *								array.
 *  @return Return the error code.
 *  @retval IMGR_OK                     Success. The API execution is successful.
 *  @retval IMGR_INV_ARG                FAIL. Invalid arguments.
 *  @retval IMGR_INV_HANDLE             FAIL. The image handle is invalid.
 *  @retval IMGR_FCT_NOT_SUPPORT        FAIL. This function is not supported.
 *  @retval IMGR_NO_EXIF_DATA           No EXIF data.
 */
/*------------------------------------------------------------------*/
extern INT32 x_img_get_exif_info(
    HANDLE_T            h_img,
    IMG_INTRA_TYPE_T    e_info_type,
    EXIF_INFO_T         *at_info,
    UINT32              ui4_count);

/* This function will set the IMG_INFO_TYPE_T type information about the image 
and information is except the height and width. */
/*------------------------------------------------------------------*/
/*! @brief  x_img_set_info
 *             This function will set the IMG_INFO_TYPE_T type information about 
 *				the image and information is except the height and width.
 *  @param [in] h_img           Specified the memory address which contains the 
 *								retrieved information. the image handle to 
 *								retrieve the information length.
 *  @param [in] e_info_type     Specified the memory address which contains the 
 *								retrieved information. the type of information 
 *								that want to retrieve the information.
 *  @param [in] pv_type_data    Reserve field for GIF file format, and set it 
 *								NULL now.
 *  @param [in] pv_info         Specified the memory address which contains the 
 *								retrieved information. the length of pv_info 
 *								buffer size.
 *  @param [in] pui4_len        Specified the memory address which contains the 
 *								retrieved information. the information length.
 *  @return Return the error code.
 *  @retval IMGR_OK                     Success. The API execution is successful.
 *  @retval IMGR_INV_ARG                FAIL. Invalid arguments.
 *  @retval IMGR_INV_HANDLE             FAIL. The image handle is invalid.
 *  @retval IMGR_FCT_NOT_SUPPORT        FAIL. This function is not supported.
 */
/*------------------------------------------------------------------*/
extern INT32 x_img_set_info (  	
	HANDLE_T        h_img,         
	IMG_INFO_TYPE_T e_info_type,   
	const VOID*     pv_type_data,  
	VOID*           pv_info,     
	UINT32          ui4_len);       

/* This function will set the data of each EXIF field. */
/*------------------------------------------------------------------*/
/*! @brief  x_img_set_exif_info
 *              This function will set the data of each EXIF field.
 *  @param [in] h_img           Specified the memory address which contains the 
 *								retrieved information. the image handle to 
 *								retrieve the information length.
 *  @param [in] e_info_type     Specified the memory address which contains the 
 *								retrieved information. primary or thumbnail 
 *								information to be set.
 *  @param [in] at_info         Specified the memory address which contains the 
 *								retrieved information. the array containing the 
 *								EXIF information.
 *  @param [in] ui4_count       Specified the memory address which contains the 
 *								retrieved information. the count of the pat_info 
 *								array.
 *  @return  Return the error code.
 *  @retval IMGR_OK                     Success. The API execution is successful.
 *  @retval IMGR_INV_ARG                FAIL. Invalid arguments.
 *  @retval IMGR_INV_HANDLE             FAIL. The image handle is invalid.
 *  @retval IMGR_FCT_NOT_SUPPORT        FAIL. This function is not supported.
 *  @retval IMGR_NO_EXIF_DATA           No EXIF data.
 */
/*------------------------------------------------------------------*/
extern INT32 x_img_set_exif_info(
    HANDLE_T            h_img,
    IMG_INTRA_TYPE_T    e_info_type,
    EXIF_INFO_T         *at_info,
    UINT32              ui4_count);

/*------------------------------------------------------------------*/
/*! @brief   x_img_get_capability
 *                This function will get the capability of system.
 *  @param [out] pt_caps      Capability of system.
 *  @return Return the error code.
 *  @retval IMGR_OK                 Success. The API execution is successful.
 *  @retval IMGR_INV_ARG            FAIL. Invalid arguments.
 *  @retval IMGR_INTERNAL_ERR       FAIL. Semaphore lock is failed or decoding error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_img_get_capability(
	IMG_CAPS_T	*pt_caps);

/*------------------------------------------------------------------*/
/*! @brief   x_img_get_frame_count
 *                This function will get the frame count from the image
 *  @param [in]h_img         identifies the image handle.
 *  @param [out]pui4_count           frame count in the image.
 *  @return Return the error code.
 *  @retval IMGR_OK                 Success. The API execution is successful.
 *  @retval IMGR_INV_ARG            FAIL. Invalid arguments.
 *  @retval IMGR_INTERNAL_ERR       FAIL. Semaphore lock is failed or decoding error.
 */
/*------------------------------------------------------------------*/

INT32 x_img_get_frame_count(
    HANDLE_T	h_img,
    UINT32		*pui4_count);


/*------------------------------------------------------------------*/
/*! @brief   x_img_get_frame_delay
 *                This function will get the frame delay from the image
 *  @param [in]h_img         identifies the image handle.
 *  @param [in]at_delay         array for store the delay time (ms).
 *  @param [in]ui4_count           count of the array element.
 *  @return Return the error code.
 *  @retval IMGR_OK                 Success. The API execution is successful.
 *  @retval IMGR_INV_ARG            FAIL. Invalid arguments.
 *  @retval IMGR_INTERNAL_ERR       FAIL. Semaphore lock is failed or decoding error.
 */
/*------------------------------------------------------------------*/


INT32 x_img_get_frame_delay(
    HANDLE_T	h_img,
    UINT32		*at_delay,
    UINT32		ui4_count);


/*------------------------------------------------------------------*/
/*! @brief   x_img_get_gif_background_color
 *                This function will get the background from the gif image
 *  @param [in]h_img         identifies the image handle.
 *  @param [out]pt_color         point to the background color .
 *  @return Return the error code.
 *  @retval IMGR_OK                 Success. The API execution is successful.
 *  @retval IMGR_INV_ARG            FAIL. Invalid arguments.
 *  @retval IMGR_INTERNAL_ERR       FAIL. Semaphore lock is failed or decoding error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_img_get_gif_background_color(
    HANDLE_T            h_img,
    GL_COLOR_T          *pt_color);

/*------------------------------------------------------------------*/
/*! @brief   x_img_get_gif_disposal_method
 *           This function will get the frame disposal method  from the image
 *  @param [in]h_img         identifies the image handle.
 *  @param [in]at_disposal         array for store the disposal method.
 *  @param [in]ui4_count           count of the array element.
 *  @return Return the error code.
 *  @retval IMGR_OK                 Success. The API execution is successful.
 *  @retval IMGR_INV_ARG            FAIL. Invalid arguments.
 *  @retval IMGR_INTERNAL_ERR       FAIL. Semaphore lock is failed or decoding error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_img_get_gif_disposal_method(
    HANDLE_T            h_img,
    IMG_GIF_DISPOSAL_T  *at_disposal,
    UINT32              ui4_count);

/*------------------------------------------------------------------*/
/*! @brief   x_img_get_gif_loop_count
 *                This function will get the loop count from the gif image
 *  @param [in]h_img         identifies the image handle.
 *  @param [out]pui2_loop_count         point to the loop count.
 *  @return Return the error code.
 *  @retval IMGR_OK                 Success. The API execution is successful.
 *  @retval IMGR_INV_ARG            FAIL. Invalid arguments.
 *  @retval IMGR_INTERNAL_ERR       FAIL. Semaphore lock is failed or decoding error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_img_get_gif_loop_count(
    HANDLE_T            h_img,
    UINT16              *pui2_loop_count);


/*------------------------------------------------------------------*/
/*! @brief   x_img_get_gif_disposal_method
 *                This function will get the frame rectangle from the image
 *  @param [in]h_img         identifies the image handle.
 *  @param [in]at_frame_rect         array for store the rectangle.
 *  @param [in]ui4_count           count of the array element.
 *  @return Return the error code.
 *  @retval IMGR_OK                 Success. The API execution is successful.
 *  @retval IMGR_INV_ARG            FAIL. Invalid arguments.
 *  @retval IMGR_INTERNAL_ERR       FAIL. Semaphore lock is failed or decoding error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_img_get_gif_frame_rect(
    HANDLE_T            h_img,
    IMG_GIF_FRM_RECT_T  *at_frame_rect,
    UINT32              ui4_count);


/*------------------------------------------------------------------*/
/*! @brief   x_img_open_eng
 *                This function will open the engine
 *  @param [in]ps_eng_name      	identifies sixteen characters engine name, 
 *									and the character is case sensitive.
 *  @param [in]pf_nfy           	specify the notify callback function.
 *  @param [in]pv_tag              	the argument prepared by the user, and it 
 *									will send back to user in (*x_img_nfy_fct).
 *  @param [out]ph_engine           The handle of the engine which want to be opened.
 *  @return Return the error code.
 *  @retval IMGR_OK                 Success. The API execution is successful.
 *  @retval IMGR_INV_ARG            FAIL. Invalid arguments.
 *  @retval IMGR_INTERNAL_ERR       FAIL. Semaphore lock is failed or decoding error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_img_open_eng (
    const CHAR          *ps_eng_name,   
    x_img_nfy_fct       pf_nfy,         
    VOID                *pv_tag,        
    HANDLE_T            h_img);        

/*------------------------------------------------------------------*/
/*! @brief   x_img_eng_sync_load
 *                function will load the image synchronously with the engine
 *  @param [in]h_engine           	identifies the handle of the engine which 
 *									performs decompression operation
 *  @param [in]e_src_type          	specify the source type
 *  @param [in]pv_data             	specifies the memory address which contain 
 *									whole image data.
 *  @param [in]pv_data2            	specifies the memory address which contain 
 *									whole image data.
 *  @param [in]z_mem_leng          	specifies the length of memory data 
 *									pv_memory_data.
 *  @param [in]e_type              	specify the type of loading source image.
 *  @param [in]pv_type_data        	auxiliary field for passing palette... etc. 
 *									data.
 *  @param [in]e_rotate_type       	specify the rotation type of this loading 
 *									operation.
 *  @param [in]ui4_src_x           	specify the x offset value to the origin of 
 *									source rectangle on the primary image. It is 
 *									no use in thumbnail loading.
 *  @param [in]ui4_src_y           	specify the y offset value to the origin of 
 *									source rectangle on the primary image. It is 
 *									no use in thumbnail loading.
 *  @param [in]ui4_src_w           	specify the width of source rectangle on the 
 *									primary image. It is no use in thumbnail 
 *									loading.
 *  @param [in]ui4_src_h           	specify the height of source rectangle on the 
 *									primary image. It is no use in thumbnail loading.
 *  @param [in]ui4_dst_x           	specify the x offset value to the origin of 
 *									destination rectangle on the target surface.
 *  @param [in]ui4_dst_y           	specify the y offset value to the origin of 
 *									destination rectangle on the target surface.
 *  @param [in]ui4_dst_w           	specify the width of destination rectangle 
 *									on the target surface.
 *  @param [in]ui4_dst_h           	specify the height of destination rectangle 
 *									on the target surface.
 *  @param [in]h_dst_surf          	specify the destination surface, which the 
 *									image will draw on it.
 *  @return Return the error code.
 *  @retval IMGR_OK                 Success. The API execution is successful.
 *  @retval IMGR_INV_ARG            FAIL. Invalid arguments.
 *  @retval IMGR_INTERNAL_ERR       FAIL. Semaphore lock is failed or decoding error.
 */
/*------------------------------------------------------------------*/


extern INT32 x_img_eng_sync_load (
    HANDLE_T             h_engine,      
    IMG_SRC_TYPE_T       e_src_type,    
    const VOID           *pv_data,      
    const VOID           *pv_data2,      
    SIZE_T               z_mem_leng,    
    IMG_INTRA_TYPE_T     e_type,        
    const VOID           *pv_type_data, 
    IMG_ROTATE_OP_TYPE_T e_rotate_type, 
    UINT32               ui4_src_x,     
    UINT32               ui4_src_y,    
    UINT32               ui4_src_w,     
    UINT32               ui4_src_h,    
    UINT32               ui4_dst_x,     
    UINT32               ui4_dst_y,     
    UINT32               ui4_dst_w,     
    UINT32               ui4_dst_h,     
    GL_HSURFACE_T        h_dst_surf);   

/*------------------------------------------------------------------*/
/*! @brief   x_img_eng_async_load
 *                function will load the image asynchronously with the engine
 *  @param [in]h_engine            	identifies the handle of the engine which 
 *									performs decompression operation
 *  @param [in]e_src_type          	specify the source type
 *  @param [in]pv_data             	specifies the memory address which contain 
 *									whole image data.
 *  @param [in]pv_data2            	specifies the memory address which contain 
 *									whole image data.
 *  @param [in]z_mem_leng          	specifies the length of memory data 
 *									pv_memory_data.
 *  @param [in]e_type              	specify the type of loading source image.
 *  @param [in]pv_type_data        	auxiliary field for passing palette... etc. 
 *									data.
 *  @param [in]e_rotate_type       	specify the rotation type of this loading 
 *									operation.
 *  @param [in]ui4_src_x           	specify the x offset value to the origin of 
 *									source rectangle on the primary image. It is 
 *									no use in thumbnail loading.
 *  @param [in]ui4_src_y           	specify the y offset value to the origin of 
 *									source rectangle on the primary image. It is 
 *									no use in thumbnail loading.
 *  @param [in]ui4_src_w           	specify the width of source rectangle on the 
 *									primary image. It is no use in thumbnail loading.
 *  @param [in]ui4_src_h           	specify the height of source rectangle on the 
 *									primary image. It is no use in thumbnail loading.
 *  @param [in]ui4_dst_x           	specify the x offset value to the origin of 
 *									destination rectangle on the target surface.
 *  @param [in]ui4_dst_y           	specify the y offset value to the origin of 
 *									destination rectangle on the target surface.
 *  @param [in]ui4_dst_w           	specify the width of destination rectangle 
 *									on the target surface.
 *  @param [in]ui4_dst_h           	specify the height of destination rectangle 
 *									on the target surface.
 *  @param [in]h_dst_surf          	specify the destination surface, which the 
 *									image will draw on it.
 *  @return Return the error code.
 *  @retval IMGR_OK                 Success. The API execution is successful.
 *  @retval IMGR_INV_ARG            FAIL. Invalid arguments.
 *  @retval IMGR_INTERNAL_ERR       FAIL. Semaphore lock is failed or decoding error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_img_eng_async_load (
    HANDLE_T             h_engine,     
    IMG_SRC_TYPE_T       e_src_type,    
    const VOID           *pv_data,      
    const VOID           *pv_data2,    
    SIZE_T               z_mem_leng,   
    IMG_INTRA_TYPE_T     e_type,        
    const VOID           *pv_type_data, 
    IMG_ROTATE_OP_TYPE_T e_rotate_type,  
    UINT32               ui4_src_x,     
    UINT32               ui4_src_y,     
    UINT32               ui4_src_w,    
    UINT32               ui4_src_h,     
    UINT32               ui4_dst_x,      
    UINT32               ui4_dst_y,    
    UINT32               ui4_dst_w,     
    UINT32               ui4_dst_h,     
    GL_HSURFACE_T        h_dst_surf);    

/*------------------------------------------------------------------*/
/*! @brief   x_img_eng_close
 *                This function will close the engine
 *  @param [in]h_engine            	identifies the handle of the engine which 
 *									performs decompression operation
 *  @return Return the error code.
 *  @retval IMGR_OK                 Success. The API execution is successful.
 *  @retval IMGR_INV_ARG            FAIL. Invalid arguments.
 *  @retval IMGR_INTERNAL_ERR       FAIL. Semaphore lock is failed or decoding error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_img_eng_close(
	HANDLE_T	h_engine);     
/*------------------------------------------------------------------*/
/*! @brief  x_img_open_exif
 *              This function will open an JPG file, just parse the EXIF info,
 *              and return the image handle. The main flow to get the exif info 
 *              likes open_exif->get_exif_info_len->get_exif_info->close_exif .
 *  @param [in] pv_src      Specified the memory address which contains the 
 *							retrieved information. the source file path string 
 *							or memory address which contain image data and It 
 *							depend on the second parameter e_type.
 *  @param [in] z_len       Specified the memory address which contains the 
 *							retrieved information. the length of pv_src.
 *  @param [in] e_type      Specifies pv_src is a file path string or a memory 
 *							address which contain image data.
 *  @param [out] ph_img     Specified the memory address which contains the 
 *							retrieved information. the image handle which 
 *							retrieve image information about the file.
 *  @return Return the error code.
 *  @retval IMGR_OK                     Success. The API execution is successful.
 *  @retval IMGR_INV_ARG                FAIL. Invalid arguments.
 *  @retval IMGR_IMG_NOT_SUPPORT        FAIL. This image does not have thumbnail.
 */
/*------------------------------------------------------------------*/

extern INT32 x_img_open_exif(
    IMG_SRC_TYPE_T        e_type,
    const VOID*           pv_src,
    SIZE_T                z_len,
    HANDLE_T*             ph_img);

/*------------------------------------------------------------------*/
/*! @brief  x_img_reparse_exif
 *             This API reparse the orientation.
 *  @param [in] ph_img    	Specified the memory address which contains the 
 *							retrieved information. the image handle which 
 *							retrieve image information about the file.
 *  @return Return the error code.
 *  @retval IMGR_OK                 Success. The image file opening is successful.
 *  @retval IMGR_INV_HANDLE         FAIL. The image handle is invalid.
 *  @retval IMGR_FCT_NOT_SUPPORT    FAIL. This function is not supported.
 *  @retval IMGR_INV_ARG            FAIL. Invalid arguments.
 *  @retval IMGR_INTERNAL_ERR       FAIL. semaphore lock is failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_img_reparse_exif(
	HANDLE_T*      ph_img);
/*------------------------------------------------------------------*/
/*! @brief   x_img_ close_exif
 *               This function will open an JPG file, just parse the EXIF info,
 *              and return the image handle. The main flow to get the exif info 
 *              likes open_exif->get_exif_info_len->get_exif_info->close_exif.
 *  @param [in] ph_img      Specified the memory address which contains the 
 *							retrieved information. the image handle which 
 *							retrieve image information about the file.
 *  @return Return the error code.
 *  @retval IMGR_OK                 Success. The image file opening is successful.
 *  @retval IMGR_INV_HANDLE         FAIL. The image handle is invalid.
 *  @retval IMGR_FCT_NOT_SUPPORT    FAIL. This function is not supported.
 *  @retval IMGR_INV_ARG            FAIL. Invalid arguments.
 *  @retval IMGR_INTERNAL_ERR       FAIL. semaphore lock is failed.
 */
/*------------------------------------------------------------------*/

extern INT32 x_img_close_exif(
	HANDLE_T	h_img);

/*------------------------------------------------------------------*/
/*! @brief   x_img_ capture
 *           This function will encode given data into JPEG or MPEG format file.
 *  @param [in] pt_in      Specified the encoding information.
 *  @param [in] ph_out      Specified the information after encoding.
 *  @return Return the error code.
 *  @retval IMGR_OK                 Success. The data encodes OK.
 *  @retval IMGR_INV_HANDLE         FAIL. The image handle is invalid.
 *  @retval IMGR_FCT_NOT_SUPPORT    FAIL. This function is not supported.
 *  @retval IMGR_INV_ARG            FAIL. Invalid arguments.
 *  @retval IMGR_INTERNAL_ERR       FAIL. semaphore lock is failed.
 *  @retval IMGR_OUT_OF_MEMORY          FAIL. The system is out of memory.
 *  @retval IMGR_ENCODE_FAIL          FAIL. The encoding process has error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_img_capture (
	CAP_INPUT_T		*pt_in, 
	CAP_OUTPUT_T	*pt_out);


/*------------------------------------------------------------------*/
/*! @brief   x_img_ cap_stop
 *               This function will stop encoding process.
 *  @return Return the error code.
 *  @retval IMGR_OK                 Success. Stop OK.
 */
/*------------------------------------------------------------------*/
extern INT32 x_img_cap_stop(VOID);

extern INT32 x_img_force_sw_decode(BOOL b_sw);

extern INT32 x_img_set_fm_function(IMAGE_PULL_FM_FUNC_T * pt_func);

#ifdef __cplusplus
}
#endif

#endif /* _X_IMG_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_IMG*/
/*----------------------------------------------------------------------------*/
