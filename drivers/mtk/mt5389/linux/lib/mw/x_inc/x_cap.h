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
 * $RCSfile: x_cap.h,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/9 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 907020e011d0dbd7ca156869d249b742 $ Asa
 *
 * Description: 
 *         This file exports all image library API and constant for 
 *         other module using.
 *---------------------------------------------------------------------------*/

#ifndef _X_CAP_H_
#define _X_CAP_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_handle.h"
#include "u_cap.h"
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
/*! @brief      x_cap_nfy_fct
 *              This function is the callback function of capture library.
 *  @param [in] h_handle     Specifies which handle will be notified. 
 *  @param [in] pv_tag       The client data specified by the client when callback has been called, the callback function will send it again.. 
 *  @param [in] e_event      It identifies the event type of notification. 
 *  @param [in] pv_data      Specified the notification data which is followed with the notification event. 
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_cap_nfy_fct) (
                                HANDLE_T                h_handle,  /* (I) Specifies which handle will be notified. */
                                const VOID*             pv_tag,    /* (I) The client data specified by the client when callback has been called, the callback function will send it again.. */
                                CAP_EVENT_TYPE_T        e_event,   /* (I) It identifies the event type of notification. */
                                const VOID*             pv_data);  /* (I) specify the notification data which is followed with the notification event. */

/*------------------------------------------------------------------*/
/*! @brief      x_cap_cust_fct
 *              This function is the function of capture library.
 *  @param [in] ui4_source   Specifies the source which will be captured. 
 *  @param [in] e_event      It identifies the event type of notification. 
 *  @param [in] pv_data      Specified the notification data which is followed with the notification event. 
 */
/*------------------------------------------------------------------*/
typedef BOOL (*x_cap_cust_fct) (
                                CAP_SRC_TYPE_T          e_source,  /* (I) Specifies the source which will be captured. */
                                CAP_EVENT_TYPE_T        e_event,     /* (I) It identifies the event type of notification. */
                                const VOID*             pv_data);    /* (I) specify the notification data which is followed with the notification event. */
#endif

/*------------------------------------------------------------------*/
/*! @brief  x_cap_open
 *             This function will freeze the screen , and return the capture handle. 
 *  @param [in] e_source             Specifies the source which will be captured.
 *  @param [in] pf_nfy               Specified the callback function to receive the notification of image library.
 *  @param [in] pv_nfy_tag           The client data specified by the client and the callback function will send it again.
 *  @param [in] pf_cust              Specified the function to customize user's operation when creating handle.
 *  @param [in] pv_cust_tag          The client data specified by the client and the callback function will send it again.
 *  @param [out] ph_cap              Specified the capture handle which retrieve capture information about the source.
 *  @return Return the error code.
 *  @retval CAPR_OK                  Success. The capture open operation is successful. 
 */
/*------------------------------------------------------------------*/
extern INT32 x_cap_open (
                            CAP_SRC_TYPE_T  e_source,        /* (I) specifies the open mode of the open operation*/ 
                            x_cap_nfy_fct   pf_nfy,          /* (I) specify the callback function to receive the notification of image library. */ 
                            const VOID*     pv_nfy_tag,      /* (I) The client data specified by the client and the callback function will send it again. */ 
                            x_cap_cust_fct  pf_cust,         /* (I) Specified the function to customize user's operation when creating handle. */ 
                            const VOID*     pv_cust_tag,     /* (I) The client data specified by the client and the callback function will send it again. */ 
                            HANDLE_T*       ph_cap);         /* (O) Specified the capture handle which retrieve capture information about the source. */

                   
/*------------------------------------------------------------------*/
/*! @brief  c_cap_capture
 *              This function will open an image memory block, parse the image header, and return the image handle.
 *  @param [in] h_cap               Specifies which handle will do the operation.
 *  @param [in] t_cap_info          Specifies the output image detail information.
 *  @return Return the error code.
 *  @retval CAPR_OK                 Success. The capture operation is successful. 
 */
/*------------------------------------------------------------------*/
extern INT32 x_cap_capture ( 
                           HANDLE_T                     h_cap,           /* (I) Specifies which handle will do the operation. */ 
                           const CAP_CAPTURE_INFO_T*    pt_cap_info);     /* (I) Specifies the output image detail information. */ 
                    


/*------------------------------------------------------------------*/
/*! @brief  x_cap_get_capture_data
 *             After the capture operation done, save the image to the specified path.
 *  @param [in]  h_cap               Specified the capture operation handle.
 *  @param [out] pc_buf              Specified the buffer to be output.
 *  @param [out] pui4_len            Specified the length to be output.
 *  @return Return the error code.
 *  @retval CAPR_OK                 Success. The image file opening is successful. 
 *  @retval CAPR_INV_HANDLE         FAIL. The image handle is invalid.
 *  @retval CAPR_FCT_NOT_SUPPORT    FAIL. This function is not supported.
 *  @retval CAPR_INV_ARG            FAIL. Invalid arguments.
 *  @retval CAPR_INTERNAL_ERR       FAIL. Semaphore lock is failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_cap_get_capture_data (
                              HANDLE_T      h_cap,     /* (I) Specified the capture operation handle. */ 
                              UCHAR**       pc_buf,    /* (I) Specified the buffer to save. */ 
                              UINT32*       pui4_len);  /* (I) Specified the buffer length to save. */ 


/*------------------------------------------------------------------*/
/*! @brief  x_cap_save
 *             After the capture operation done, save the image to the specified path.
 *  @param [in] h_cap               Specified the capture operation handle.
 *  @param [in] ps_path             Specified the path to save.
 *  @return Return the error code.
 *  @retval CAPR_OK                 Success. The image file opening is successful. 
 *  @retval CAPR_INV_HANDLE         FAIL. The image handle is invalid.
 *  @retval CAPR_FCT_NOT_SUPPORT    FAIL. This function is not supported.
 *  @retval CAPR_INV_ARG            FAIL. Invalid arguments.
 *  @retval CAPR_INTERNAL_ERR       FAIL. Semaphore lock is failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_cap_save (HANDLE_T                       h_cap,      /* (I) Specified the capture operation handle. */ 
                         const CAP_LOGO_SAVE_INFO_T*    pt_save_info);   /* (I) Specified the path to save. */ 


/*------------------------------------------------------------------*/
/*! @brief   x_cap_close
 *               This function close the capture handle and free the resource.
 *  @param [in] h_cap               Specified the already opened image handle.
 *  @return Return the error code.
 *  @retval CAPR_OK                 Success. The API execution is successful. 
 *  @retval CAPR_INV_HANDLE         FAIL. The capture handle is invalid.
 *  @retval CAPR_INV_ARG            FAIL. The pathe is NULL.
 *  @retval CAPR_FCT_NOT_SUPPORT    FAIL. This function is not supported.
 */
/*------------------------------------------------------------------*/
extern INT32 x_cap_close (HANDLE_T          h_cap); /* (O) retrieve the height of the image. */ 

                                                     
/*------------------------------------------------------------------*/
/*! @brief      x_cap_sync_stop_capture
 *                  This function sync stops the capture operation.
 *  @param [in] h_cap       Specified the capture handle that want to stop.
 *  @return Return the error code.
 *  @retval CAPR_OK                 Success. The API execution is successful. 
 *  @retval CAPR_INV_ARG            FAIL. Invalid arguments.
 *  @retval CAPR_INV_HANDLE         FAIL. The capture handle is invalid.
 *  @retval CAPR_FCT_NOT_SUPPORT    FAIL. This function is not supported.
 *  @retval CAPR_INTERNAL_ERR       FAIL. Semaphore lock is failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_cap_sync_stop_capture (  HANDLE_T     h_cap);           /* (I) specify the capture handle that want to stop. */ 


                        
/*------------------------------------------------------------------*/
/*! @brief      x_cap_sync_stop_save
 *                  This function can stop these types IMG_STOP_OP_TYPE_T of image operation.
 *  @param [in] h_cap               Specified the image handle that want to stop.
 *  @return Return the error code.
 *  @retval CAPR_OK                 Success. The API execution is successful. 
 *  @retval CAPR_INV_ARG            FAIL. Invalid arguments.
 *  @retval CAPR_INV_HANDLE         FAIL. The capture handle is invalid.
 *  @retval CAPR_FCT_NOT_SUPPORT    FAIL. This function is not supported.
 *  @retval CAPR_INTERNAL_ERR       FAIL. Semaphore lock is failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_cap_sync_stop_save (  HANDLE_T     h_cap);           /* (I) Specify the capture handle that want to stop. */ 


/* This API will start decoding the primary image or thumbnail to the target surface h_dst_surf, and it will not return until decoding image is complete. */          
/*------------------------------------------------------------------*/
/*! @brief   x_cap_async_stop_capture
 *               This API will async stop capture operation.
 *  @param [in] h_cap                   Specified the capture handle that want to async stop.
 *  @return Return the error code.
 *  @retval CAPR_OK                     Success. The API execution is successful. 
 */
/*------------------------------------------------------------------*/
extern INT32 x_cap_async_stop_capture ( HANDLE_T             h_cap);        /* (I) specify the capture handle that want to async stop. */ 

                        
/*------------------------------------------------------------------*/
/*! @brief   x_cap_async_stop_save
 *               This API will async stop saving operation.
 *  @param [in] h_cap           Specified the image handle that want to decode.
 *  @return Return the error code.
 *  @retval CAPR_OK                     Success. The API execution is successful. 
 */
/*------------------------------------------------------------------*/
extern INT32 x_cap_async_stop_save ( HANDLE_T             h_cap);        /* (I) Specify the image handle that want to aysnc stop saving. */ 
                            
                            
/*------------------------------------------------------------------*/
/*! @brief   x_cap_set_boot_logo
 *               This API will select specified picture captured as bootup logo.
 *  @param [in] ps_path                 Specified the image path name that want to set as bootup logo.
 *  @return Return the error code.
 *  @retval CAPR_OK                     Success. The API execution is successful. 
 */
/*------------------------------------------------------------------*/
extern INT32 x_cap_select_as_bootlogo (const CAP_LOGO_SELECT_INFO_T* pt_select_info);        /* (I) Specified the image path name that want to set as bootup logo. */ 


/*------------------------------------------------------------------*/
/*! @brief   x_cap_query_capabilty
 *               This API will select specified picture captured as bootup logo.
 *  @param [in] pt_cap_info                 Specified the address to store the logo info.
 *  @return Return the error code.
 *  @retval CAPR_OK                     Success. The API execution is successful. 
 */
/*------------------------------------------------------------------*/
extern INT32 x_cap_query_capabilty(CAP_CAPABILITY_INFO_T*  pt_cap_info); /* (I) specify the already opened image handle. */ 


extern INT32 x_cap_init (void);

extern INT32 x_cap_do_capturing(HANDLE_T   h_cap);
extern INT32 x_cap_do_saving(HANDLE_T   h_cap);

#ifdef __cplusplus
}
#endif

#endif /* _X_CAP_H_ */


