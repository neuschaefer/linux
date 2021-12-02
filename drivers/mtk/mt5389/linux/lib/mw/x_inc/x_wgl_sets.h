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
 * $RCSfile: x_wgl_sets.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: 4b1cf24328c22f9b35c46f0cba295c64 $
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
#ifndef _X_WGL_SETS_H_
#define _X_WGL_SETS_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_gl.h"
#include "u_fe.h"
#include "u_wgl.h"
#include "u_wgl_sets.h"
#include "x_wgl_common.h"


#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
typedef struct _WGL_CUSTOM_INIT_INFO_T
{
    UINT16              ui2_font_handle_grow_num;
} WGL_CUSTOM_INIT_INFO_T;

/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief This function set the system's default background, foreground, and text color for widgets.
 *  @param [in] ui2_color_id		Specify the target.
 *  @param [in] pt_color	Pointer to a GL_COLOR_T structure to contain the target color.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval  WGLR_OK     The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument pt_color_info is NULL or ui2_color_index is invalid
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_set_system_color(
        UINT16                          ui2_color_id,
        const GL_COLOR_T*               pt_color);

/*------------------------------------------------------------------*/
/*! @brief This function set the system's default font information for widgets.
 *  @param [in] ps_font	 Contains font name
 *  @param [in] e_size	 Contains font size
 *  @param [in] e_style	 Contains font style
 *  @param [in] e_cmap	 Contains font encoding type
 *  @note
 *  @see
 *  @return  return error code
 *  @retval WGLR_OK     The routine was successful.
 *  @retval  WGLR_INTERNAL_ERROR    No space to store font info.
 *  @retval  WGLR_OVER_SUPPORT_LIMIT    Font name specified by ps_font is over limit.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_set_system_font(
        const CHAR*                     ps_font, 
        FE_FNT_SIZE                     e_size,
        FE_FNT_STYLE                    e_style,
        FE_CMAP_ENCODING                e_cmap);


/*------------------------------------------------------------------*/
/*! @brief This function returns target image's handle and its information.
 *  @param [in] ui2_image_id	Specify the image id.
 *  @param [in]  pe_segment_type Pointer to a WGL_IMG_ST_T variable to contain the segment type. 
 *  @param [in]   ph_image    Pointer to a WGL_HIMG_TPL_T variable to contain target image's handle.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval  WGLR_OK The routine was successful.
 *  @retval  WGLR_INV_ARG    The argument e_segment_type or ui2_image_id is invalid, pt_image is NULL.
 *  @retval  WGLR_NOT_SUPPORT_RUNTIME_UPDATE    This system image has been initialized. This version doesn't support change system image at runtime .
 *  @retval  WGLR_INTERNAL_ERROR The image handle and image data kept in widget library is invalid
 *  @retval      -
 */
/*------------------------------------------------------------------*/

extern INT32 x_wgl_set_system_image(
        UINT16                          ui2_image_id,
        WGL_IMG_ST_T                    e_segment_type,
        const WGL_IMG_T*                pt_image);

/*------------------------------------------------------------------*/
/*! @brief This function returns the system's default background, foreground, and text color for widgets.
 *  @param [in] ui2_color_id		Specify the target.
  *  @param [in] h_canvas		Specify the target canvas handle.
 *  @param [out] pt_color	Pointer to a GL_COLOR_T structure to receive the target color.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval  WGLR_OK     The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument pt_color_info is NULL or ui2_color_index is invalid
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_get_system_color(
        UINT16                          ui2_color_id,
        HANDLE_T                        h_canvas,
        GL_COLOR_T*                     pt_color);

/*------------------------------------------------------------------*/
/*! @brief This function returns the system's default font information for widgets.
 *  @param [in] None
 *  @param [out] pt_font_info	Pointer to a WGL_FONT_INFO_T to receive the font information.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval WGLR_OK     The routine was successful.
 *  @retval  WGLR_INV_ARG    The argument ph_font is NULL
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_get_system_font(
        WGL_FONT_INFO_T*                pt_font_info);

/*------------------------------------------------------------------*/
/*! @brief This function returns target image's handle and its information.
 *  @param [in] ui2_image_id	Specify the image id.
 *  @param [in] h_canvas		Specify the target canvas handle.
 *  @param [out]  pe_segment_type Pointer to a WGL_IMG_ST_T variable to receive the segment type. About the available segment type. 
 *  @param [out]   pui4_img_width  Pointer to a UINT32 variable to receive target image's width.
 *  @param [out]   pui4_img_height Pointer to a UINT32 variable to receive target image's height.
 *  @param [out]   ph_image    Pointer to a WGL_HIMG_TPL_T variable to receive target image's handle.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval  WGLR_OK The routine was successful.
 *  @retval  WGLR_INV_ARG    The argument ph_image, pe_segment_type, pui4_img_width, or pui4_img_height is NULL or ui2_image_id is invalid.
 *  @retval  WGLR_SYS_RC_NOT_INIT    The target image isn't initialized.
 *  @retval  WGLR_INTERNAL_ERROR The image handle and image data kept in widget library is invalid
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_get_system_image(
        UINT16                          ui2_image_id,
        HANDLE_T                        h_canvas,
        WGL_IMG_ST_T*                   pe_segment_type,
        UINT32*                         pui4_img_width,
        UINT32*                         pui4_img_height,
        WGL_HIMG_TPL_T*                 ph_image);


/*------------------------------------------------------------------*/
/*! @brief This function returns target image's width,height information.
 *  @param [in] h_img_tpl	Specify the image handle.
 *  @param [out]   pui4_height  Pointer to a UINT32 variable to receive target image's width.
 *  @param [out]   pui4_height Pointer to a UINT32 variable to receive target image's height.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval  WGLR_OK The routine was successful.
 *  @retval  WGLR_INV_ARG    The argument h_img_tpl,  pui4_height, or pui4_height is NULL .
 *  @retval  WGLR_INV_HANDLE    h_img_tpl is not valid image handle.
 *  @retval  WGLR_INTERNAL_ERROR Internal error.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_img_tpl_get_size(
        WGL_HIMG_TPL_T                  h_img_tpl,
        UINT32*                         pui4_width,
        UINT32*                         pui4_height);


/*------------------------------------------------------------------*/
/*! @brief This function retrieve widget's ctrl functin table.
 *  @param [in] e_type	Specify the widget type.
 *  @param [out]   ppv_intf_out  Pointer to a WGL_WIDGET_CTRL_INTF_T variable to receive widget's function table.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval  WGLR_OK The routine was successful.
 *  @retval  WGLR_INV_ARG    The argument ppv_intf_out is NULL .
 *  @retval  WGLR_INV_TYPE    e_type is not valid widget type.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 wgl_query_comp_intf (
        HANDLE_TYPE_T                   e_type,
        VOID**                          ppv_intf_out);


/*------------------------------------------------------------------*/
/*! @brief This function initialize widget library related system resource
 *  @param None.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval  WGLR_OK The routine was successful.
 *  @retval     WGLR_OS_ERROR     Get thread, message queue or semphore failed.
 *  @retval     WGLR_INIT_FAILED     init resource failed.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 wgl_init_default_sys_resource(void);


/*------------------------------------------------------------------*/
/*! @brief This function initialize widget library related system resource.
 *  @param [in] pt_custom_info	Specify widget custom init info
 *  @note
 *  @see
 *  @return  return error code
 *  @retval  WGLR_OK The routine was successful.
 *  @retval     WGLR_OS_ERROR     Get thread, message queue or semphore failed.
 *  @retval     WGLR_INIT_FAILED     init resource failed.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 wgl_init_default_sys_resource_ex(
        const WGL_CUSTOM_INIT_INFO_T*   pt_custom_info);

/*------------------------------------------------------------------*/
/*! @brief This API evaluate how many characters could be shown in a box.
 *  @param [in]   h_fnt               Contains a handle of font object.
 *  @param [in]   w2s_str             The string to be evaluated.
 *  @param [in]   i4_max_len          Maximum length of string to check.
  *  @param [in]   b_check_length       if true then maximum check length is smaller one between  i4_max_len and w2s_str's actual length 
  *                                                    if false then maximum check length is i4_max_len
 *  @param [in]   i4_box_w            The widht of required bounding box
 *  @param [in]   w2s_can_break       Array of 'Can Break' characters
 *  @param [in]    w2s_skip            Array of 'Should-Be-Skipped' characters, can be NULL
 *  @param [in]   ui2_num_skip        Number of 'Should-Be-Skipped' characters in w2s_skip
 *  @param [in]   ui2_w_skip          Pixel size of 'Should-Be-Skipped' characters
  *  @param [in]   b_skip_as_break          If skip character should be treated as break character
 *  @param [out]  pw2s_next           Pointer of the next character outside box
 *  @param [out]     pui4_num_ch         How many characters in the box
 *  @param [out]     pi4_width           References the width of the string if the
 *                              routine was successful. In all other cases, the
 *                              value of this argument is undetermined.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval      WGLR_INV_HANDLE     The Font Engine is not initialized yet.
 *  @retval       WGLR_INTERNAL_ERROR The argument h_fnt is an invalid font handle.
 *  @retval      WGLR_INV_ARG        One or more characters in the string is invalid or not in font.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_get_max_char_in_box_ex(
        HFONT_T                         h_fnt,
        const UTF16_T*                  w2s_str,
        INT32                           i4_max_len,
        BOOL                            b_check_length,
        INT32                           i4_box_w,
        const UTF16_T*                  w2s_can_break,
        const UTF16_T*                  w2s_skip,
        UINT16                          ui2_num_skip,
        UINT16                          ui2_w_skip,
        BOOL                            b_skip_as_break,
        UTF16_T**                       pw2s_next,
        UINT32*                         pui4_num_ch,
        INT32*                          pi4_width);

#ifdef __cplusplus
}
#endif

#endif
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_WGL*/
/*----------------------------------------------------------------------------*/

