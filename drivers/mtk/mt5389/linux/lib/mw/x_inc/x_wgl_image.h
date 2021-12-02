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
 * $RCSfile: x_wgl_image.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/3 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: c52c49b300505ba29a52b153d361bace $
 *
 * Description: 
 *         This header file contains public interfaces, constant and type 
 *         definitions of the Widget Library.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_WGL   Widget Library
 *
 *  @ingroup groupMW
 *  @brief Provides constant,type definitions and function of the widget library.
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/ 
#ifndef _X_WGL_IMAGE_H_
#define _X_WGL_IMAGE_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle.h"
#include "u_wgl.h"
#include "u_gl.h"
#include "u_wgl_sets.h"
#include "u_wgl_cvsst.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief This function creates an image template for assigned image.
 *  @param [in]   e_segment_type  Segment type of the assigned image. 
 *  @param [in]   pt_image    Pointer to a WGL_IMG_T structure that contains a image data. 
 *  @param [out] ph_img_tpl	Pointer to a WGL_HIMG_TPL_T variable to receive the image template handle.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval    WGLR_OK The routine was successful.
 *  @retval    WGLR_INV_ARG    The followed table lists the cases that may cause this error:
 *                 pt_image:   NULL
 *                 ph_img_tpl: NULL
 *                ui2_width of pt_image:  0
 *                 ui2_lines of pt_image:  0
 *                  pui1_image_data of pt_image: NULL
 *                  ui4_image_size of pt_image:  0
 *  @retval WGLR_NOT_SUPPORT_FORMAT The followed table lists the cases that may cause this error:
 *                  ui1_bits_per_pixel of pt_image: Not 16
 *                  ui2_table_columns of pt_image:  Not 0
 *                  pt_table_data of pt_image:  Not NULL
 *                  ui1_image_type of pt_image: WGL_IMG_TYPE_JPG or other undefined type
 *  @retval  WGLR_INTERNAL_ERROR The image handle and image data kept in widget library is invalid
 *  @retval  WGLR_OUT_OF_MEMORY  There isn't sufficient memory.
 *  @retval  WGLR_OUT_OF_HANDLES There isn't enough handles or the caller's  handle resource is exhausted.
 *  @retval  WGLR_SYS_RC_CREATE_FAIL Fail to create surface for this image template.
 *  @retval  WGLR_INTERNAL_ERROR Fail to get the surface information for this image template.
 *  @retval  WGLR_LOCK_ERROR Fail to lock the surface for some internal operation.
 *  @retval  WGLR_UNLOCK_ERROR   Fail to unlock the surface to end this routine.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_img_tpl_create(
        WGL_IMG_ST_T                    e_segment_type,
        const WGL_IMG_T*                pt_image,
        WGL_HIMG_TPL_T*                 ph_img_tpl);

/*------------------------------------------------------------------*/
/*! @brief This function creates an image template for assigned image. If the image is compressed, this function willd decompress it.
 *  @param [in]   e_segment_type  Segment type of the assigned image. 
 *  @param [in]   pt_image    Pointer to a WGL_IMG_T structure that contains a image data. 
 *  @param [out] ph_img_tpl	Pointer to a WGL_HIMG_TPL_T variable to receive the image template handle.
 *  @return  return error code
 *  @retval    WGLR_OK The routine was successful.
 *  @retval    WGLR_INV_ARG    The followed table lists the cases that may cause this error:
 *                 pt_image:   NULL
 *                 ph_img_tpl: NULL
 *                ui2_width of pt_image:  0
 *                 ui2_lines of pt_image:  0
 *                  pui1_image_data of pt_image: NULL
 *                  ui4_image_size of pt_image:  0
 *  @retval WGLR_NOT_SUPPORT_FORMAT The followed table lists the cases that may cause this error:
 *                  ui1_bits_per_pixel of pt_image: Not 16
 *                  ui2_table_columns of pt_image:  Not 0
 *                  pt_table_data of pt_image:  Not NULL
 *                  ui1_image_type of pt_image: WGL_IMG_TYPE_JPG or other undefined type
 *  @retval  WGLR_INTERNAL_ERROR The image handle and image data kept in widget library is invalid
 *  @retval  WGLR_OUT_OF_MEMORY  There isn't sufficient memory.
 *  @retval  WGLR_OUT_OF_HANDLES There isn't enough handles or the caller's  handle resource is exhausted.
 *  @retval  WGLR_SYS_RC_CREATE_FAIL Fail to create surface for this image template.
 *  @retval  WGLR_INTERNAL_ERROR Fail to get the surface information for this image template.
 *  @retval  WGLR_LOCK_ERROR Fail to lock the surface for some internal operation.
 *  @retval  WGLR_UNLOCK_ERROR   Fail to unlock the surface to end this routine.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_img_tpl_create_ex(
        WGL_IMG_ST_T                    e_segment_type,
        const WGL_IMG_T*                pt_image,
        WGL_HIMG_TPL_T*                 ph_img_tpl);

/*------------------------------------------------------------------*/
/*! @brief  This function destroys the input image template.
 *  @param [in]  h_img_tpl	Handle of a image template.
 *  @param [out]  None
 *  @note
 *  @see
 *  @return  return error code
 *  @retval    WGLR_OK The routine was successful.
 *  @retval    WGLR_INV_ARG    The h_img_tpl is NULL or invalid.
 *  @retval     WGLR_INTERNAL_ERROR The internal object for the image template is NULL or fail to free this object.
 *  @retval     WGLR_SYS_RC_CANNOT_FREE The image template is provided and maintained by the system. 
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_img_tpl_destroy(
        WGL_HIMG_TPL_T                  h_img_tpl);


/*------------------------------------------------------------------*/
/*! @brief This API is to create surface by input image template.
  *  @param [in]  h_img_tpl	Handle of a image template.
 *  @param [in]   pe_type    Segment type of the assigned image. 
 *  @param [out] ph_surface Pointer to a surface handle created according to h_img_tpl .
 *  @note
 *  @see
 *  @return  return error code
 *  @retval    WGLR_OK The routine was successful.
 *  @retval    WGLR_INV_ARG    The followed table lists the cases that may cause this error:
 *                 h_img_tpl:   NULL
 *                 pe_type: NULL
 *                  ph_surface: NULL
 *  @retval  WGLR_INTERNAL_ERROR The image handle and image data kept in widget library is invalid
 *  @retval  WGLR_OUT_OF_MEMORY  There isn't sufficient memory.
 *  @retval  WGLR_OUT_OF_HANDLES There isn't enough handles or the caller's  handle resource is exhausted.
 *  @retval  WGLR_SYS_RC_CREATE_FAIL Fail to create surface for this image template.
 *  @retval  WGLR_INTERNAL_ERROR Fail to get the surface information for this image template.
 *  @retval  WGLR_LOCK_ERROR Fail to lock the surface for some internal operation.
 *  @retval  WGLR_UNLOCK_ERROR   Fail to unlock the surface to end this routine.
 *  @retval   WGLR_IMG_DRAW_FAIL   -Dram the surface data fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_img_tpl_image_data_clone(
        WGL_HIMG_TPL_T                      h_img_tpl,
        WGL_IMG_ST_T*                       pe_type,
        GL_HSURFACE_T*                      ph_surface);

/*------------------------------------------------------------------*/
/*! @brief This API is to create an image template by surface information.
  *  @param [in]   e_segment_type  Segment type of the assigned image. 
 *  @param [in]   h_src_surf    Source surface handle to create image template
 *  @param [out] ph_img_tpl	Pointer to a WGL_HIMG_TPL_T variable to receive the image template handle.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval    WGLR_OK The routine was successful.
 *  @retval    WGLR_INV_ARG    The followed table lists the cases that may cause this error:
 *                 h_src_surf:   NULL
 *                 ph_img_tpl: NULL
 *  @retval     WGLR_INTERNAL_ERROR Some internal calls return error.
 *  @retval   WGLR_IMG_DRAW_FAIL   -Dram the surface data fail.
 *  @retval  WGLR_OUT_OF_HANDLES There isn't enough handles or the caller's  handle resource is exhausted.
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_img_tpl_create_by_surf(
        WGL_IMG_ST_T                    e_segment_type,
        GL_HSURFACE_T                   h_src_surf,
        WGL_HIMG_TPL_T*                 ph_img_tpl);

/*------------------------------------------------------------------*/
/*! @brief This API renders the widget which is located in h_graphics with input WGL_HIMG_TPL_T value. Note that the region is specified in the local coordinates of h_widget.
 *  @param [in]    h_img_tpl             Contains the image handle used to paint on the widget.
 *  @param [in]  pt_output_rect         Contains the rectangle of the widget.
 *  @param [in]  pt_invalid_rect        Contains the invalid rectangle of the widget.
 *  @param [in]  h_graphics              Contains the pointer to the graphic context that can be used to paint the widget background.
 *  @param [in]  pt_cvsst                Contains the canvas status info of the widget.
 *  @param [in]  e_paint_method     Contains the paint mode
 *  @param [out]  None
 *  @note
 *  @see
 *  @return  return error code
 *  @retval    WGLR_OK The routine was successful. 
*  @retval    WGLR_INV_ARG    One of input arguments is NULL.
*  @retval   WGLR_NOT_SUPPORT_FORMAT The image format is not supported.
*  @retval   WGLR_INTERNAL_ERROR  Some internal calls return error.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_img_tpl_paint(
        WGL_HIMG_TPL_T             h_img_tpl,
        const GL_RECT_T*           pt_output_rect,
        const GL_RECT_T*           pt_invalid_rect,
        GL_HGRAPHICS_T             h_graphics,
        const WGL_CANVAS_STATUS_T  *pt_cvsst,
        WGL_TPL_PAINT_METHOD_T     e_paint_method);

/*------------------------------------------------------------------*/
/*! @brief This API is to update image region by surface information.
 *  @param [in] h_dest_img_tpl          -The destignation image handle.
 *  @param [in] h_src_surf                 -The source surface handle.
 *  @param [out] None
 *  @note
 *  @see
 *  @return  return error code
 *  @retval    WGLR_OK The routine was successful. 
*  @retval    WGLR_INV_ARG    -One of input arguments is NULL.
*  @retval   WGLR_INTERNAL_ERROR    -The image handle is invalid.
*  @retval   WGLR_INV_CMD    -if this image template is in the compressed format, it can not be updated by a surface..
*  @retval   WGLR_IMG_DRAW_FAIL   -Dram the surface data fail.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_img_tpl_update_by_surf(
        WGL_HIMG_TPL_T                  h_dest_img_tpl,
        GL_HSURFACE_T                   h_src_surf);

/*------------------------------------------------------------------*/
/*! @brief This function returns target image template's width and height.
 *  @param [in]  h_img_tpl	Handle of a image template.
 *  @param [out]   pui4_width  Pointer to a UINT32 variable to receive target template's width.
 *  @param [out]   pui4_height Pointer to a UINT32 variable to receive target template's height.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument h_img_tpl, pui4_width, or pui4_height is NULL.
 *  @retval   WGLR_INV_HANDLE The h_img_tpl is invalid handle.
 *  @retval   WGLR_INTERNAL_ERROR The image handle and image data kept in widget library is invalid
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_img_tpl_get_size(
        WGL_HIMG_TPL_T              h_img_tpl,
        UINT32*                     pui4_width,
        UINT32*                     pui4_height);


/*------------------------------------------------------------------*/
/*! @brief This function is called by user to clear compressed image data buffer cache
 *               which is used to optimize animation performance 
 *  @param 
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_img_cache_flush(VOID);
extern INT32 x_wgl_img_tpl_create_surface(
        const WGL_HIMG_TPL_T*                  ph_img_tpl,
        UINT16                                 ui2_img_nbs);

extern INT32 x_wgl_img_tpl_destroy_surface(
        const WGL_HIMG_TPL_T*                  ph_img_tpl,
        UINT16                                 ui2_img_nbs);



extern INT32 x_wgl_img_tpl_destroy_surface(
        const WGL_HIMG_TPL_T*                  ph_img_tpl,
        UINT16                                 ui2_img_nbs);


extern INT32 x_wgl_set_img_decode_function(WGL_IMG_DECODE_FUNCTION_TABLE_T* pt_dec_table);

extern INT32 x_wgl_set_invert_height(WGL_HIMG_TPL_T                          h_img_tpl,
                                     UINT16                                  ui2_invert_lines);
extern INT32 x_wgl_set_image_decode_sw_force(BOOL fg_sw);

#ifdef __cplusplus
}
#endif

#endif
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_WGL*/
/*----------------------------------------------------------------------------*/

