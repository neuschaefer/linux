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
 * $RCSfile: x_wgl_rect.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/2 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: b8d132a151518058e76116fd8f8c48e1 $
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
#ifndef _X_WGL_RECT_H_
#define _X_WGL_RECT_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_wgl_rect.h"

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
/*! @brief This function sets the rectangle with specified values. 
 *  @param [in]  i4_left The x coordinate of the upper-left corner of the rectangle. 
  *  @param [in] i4_top  The y coordinate of the upper-left corner of the rectangle. 
 *  @param [in]  i4_right    The x coordinate of the lower-right corner of the rectangle. 
 *  @param [in]  i4_bottom   The y coordinate of the lower-right corner of the rectangle.
 *  @param [out] pt_rect	Pointer to the rectangle.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval  WGLR_OK     The routine was successful.
  *  @retval WGLR_INV_ARG    The argument pt_rect is NULL.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_rect_set(
        GL_RECT_T*                      pt_rect,
        INT32                           i4_left,
        INT32                           i4_top,
        INT32                           i4_right,
        INT32                           i4_bottom);


/*------------------------------------------------------------------*/
/*! @brief This function copies the coordinates of the source rectangle pointed to by pt_src_rect to the destination rectangle pointed to by pt_dest_rect.
 *  @param [in] pt_src_rect	Pointer to the source rectangle.
 *  @param [out] pt_dest _rect	Pointer to the destination rectangle. 
 *  @note
 *  @see
 *  @return  return error code
 *  @retval  WGLR_OK     The routine was successful.
  *  @retval WGLR_INV_ARG    The argument pt_src_rect or pt_dest_rect is NULL.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_rect_copy(
        GL_RECT_T*                      pt_dest_rect,
        const GL_RECT_T*                pt_src_rect);


/*------------------------------------------------------------------*/
/*! @brief This function increases or decreases the width and height of the specified rectangle pt_rect. This function adds cx units to the right of the rectangle and cy units to the bottom. Positive values increases the width and height, and negative values decreases them.
 *  @param [in]  i4_cx   The value inflating the width of the rectangle.
 *  @param [in]  i4_cy   The value inflating the height of the rectangle.
 *  @param [out] pt_rect	Pointer to the rectangle.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval  WGLR_OK     The routine was successful.
  *  @retval WGLR_INV_ARG    The argument pt_rect is NULL.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_rect_inflate(
        GL_RECT_T*                      pt_rect,
        INT32                           i4_cx,
        INT32                           i4_cy);


/*------------------------------------------------------------------*/
/*! @brief This function calculates the intersection of two source rectangles (pt_src_rect1 and pt_src_rect2) and places the coordinates of the intersection rectangle into the destination rectangle pointed to by pt_intersect_rect. If the source rectangles do not intersect, and empty rectangle (in which all coordinates are set to zero) is placed into the destination rectangle.
 *  @param [in]  pt_src_rect1    Pointer to the first source rectangle.
 *  @param [in]  pt_src_rect2    Pointer to the second source rectangle.
 *  @param [out] pt_intersect_rect	Pointer to the destination rectangle.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval  WGLR_OK     The routine was successful.
 *  @retval WGLR_INV_ARG    The argument pt_dest_rect, pt_src_rect1, or pt_src_rect2 is NULL.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_rect_intersect(
        GL_RECT_T*                      pt_intersect_rect,
        const GL_RECT_T*                pt_src_rect1,
        const GL_RECT_T*                pt_src_rect2);


/*------------------------------------------------------------------*/
/*! @brief This function calculates the union of two source rectangles (pt_src_rect1 and pt_src_rect2) and places the coordinates of the union rectangle into the destination rectangle pointed to by pt_union_rect.
 *  @param [in]  pt_src_rect1    Pointer to the first source rectangle.
  *  @param [in] pt_src_rect2    Pointer to the second source rectangle.
 *  @param [out] pt_union _rect	Pointer to the destination rectangle.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval  WGLR_OK     The routine was successful.
  *  @retval  WGLR_INV_ARG    The argument pt_union_rect, pt_src_rect1, or pt_src_rect2 is NULL.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_rect_union(
        GL_RECT_T*                      pt_union_rect,
        const GL_RECT_T*                pt_src_rect1,
        const GL_RECT_T*                pt_src_rect2);


/*------------------------------------------------------------------*/
/*! @brief This function moves the specified rectangle by the specified offsets. if i4_x is a negative value, the rectangle would be moved to the left, and if i4_y is a negative value, it would be moved up.
 *  @param [in]  i4_x    The x offset.
  *  @param [in]  i4_y    The y offset.
 *  @param [out] pt_rect	Pointer to the rectangle.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval  WGLR_OK     The routine was successful.
 *  @retval WGLR_INV_ARG    The argument pt_rect is NULL.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_rect_offset(
        GL_RECT_T*                      pt_rect,
        INT32                           i4_x,
        INT32                           i4_y);

/*------------------------------------------------------------------*/
/*! @brief inset the specified rectange by the specified inset
 *  @param [in]  i4_left left inset 
 *  @param [in]  i4_top top inset 
 *  @param [in]  i4_right right inset 
 *  @param [in]  i4_bottom bottom inset 
 *  @param [out] [out] pt_rect	Pointer to the rectangle.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK     The routine was successful.
 *  @retval  WGLR_INV_ARG    The argument pt_rect is NULL.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_rect_inset(
        GL_RECT_T*                      pt_rect,
        INT32                           i4_left,
        INT32                           i4_top,
        INT32                           i4_right,
        INT32                           i4_bottom);

/*------------------------------------------------------------------*/
/*! @brief This function empties the rectangle pointed to by pt_rect. After this function, all coordinates of the rectangle would be zero.
 *  @param [in] None.
 *  @param [out] pt_rect	Pointer to the rectangle.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK     The routine was successful.
 *  @retval  WGLR_INV_ARG    The argument pt_rect is NULL.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_rect_empty(
        GL_RECT_T*                      pt_rect);


/*------------------------------------------------------------------*/
/*! @brief This function  normalizes the rectangle. After this function,  the left would be less than the right, and the top would be less than the bottom.
 *  @param [in] None
 *  @param [out]  pt_rect	Pointer to the rectangle.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval  WGLR_OK     The routine was successful.
  *  @retval WGLR_INV_ARG    The argument pt_rect is NULL.
 *  @retval      -
 */
/*------------------------------------------------------------------*/

extern INT32 x_wgl_rect_normalize(
        GL_RECT_T*                      pt_rect);


/*------------------------------------------------------------------*/
/*! @brief This function determines whether the two specified rectangles (pt_rect1 and pt_rect2) are equal by comparing the coordinates of the upper-left and lower-right corners.
 *  @param [in]   pt_rect1    Pointer to the source rectangle.
 *  @param [in]   pt_rect2    Pointer to the source rectangle.
 *  @param [out]  pb_is_equal	Pointer to a BOOL variable to receive the result.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK     The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument pt_rect1, pt_rect2, or pb_is_equal is NULL
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_rect_equal(
        BOOL*                           pb_is_equal,
        const GL_RECT_T*                pt_rect1,
        const GL_RECT_T*                pt_rect2);


/*------------------------------------------------------------------*/
/*! @brief This function determines whether the specified rectangle pt_rect is empty. An empty rectangle is one that has no area; that is, the coordinates of the right side is equal to the coordinate of the left side, or the coordinates of the bottom side is equal to the coordinate of the top side.
 *  @param [in] pt_rect	Pointer to the source rectangle.
 *  @param [out]  pb_is_empty	Pointer to a BOOL variable to receive the result.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval  WGLR_OK     The routine was successful.
 *  @retval  WGLR_INV_ARG    The argument pt_rect or pb_is_empty is NULL.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_rect_is_empty(
        BOOL*                           pb_is_empty,
        const GL_RECT_T*                pt_rect);


/*------------------------------------------------------------------*/
/*! @brief This function determines whether two rectangles (pt_rect1 and pt_rect2) intersect.
 *  @param [in]   pt_rect1    Pointer to the first rectangle.
 *  @param [in]   pt_rect2    Pointer to the second rectangle.
 *  @param [out] pb_is_cross	Pointer to a BOOL variable to receive the result.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK     The routine was successful.
 *  @retval  WGLR_INV_ARG    The argument pt_rect1, pt_rect2, or pb_is_cross is NULL.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_rect_is_cross(
        BOOL*                           pb_is_intersected,
        const GL_RECT_T*                pt_rect1,
        const GL_RECT_T*                pt_rect2);


/*------------------------------------------------------------------*/
/*! @brief This function determines whether pt_rect1 includes pt_rect2.
 *  @param [in]   pt_rect1    Pointer to the first rectangle.
 *  @param [in] pt_rect2    Pointer to the second rectangle.
 *  @param [out]  pb_is_included	Pointer to a BOOL variable to receive the result.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK     The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument pt_rect1, pt_rect2, or pb_is_included is NULL.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_rect_is_included(
        BOOL*                           pb_is_included,
        const GL_RECT_T*                pt_rect1,
        const GL_RECT_T*                pt_rect2);


/*------------------------------------------------------------------*/
/*! @brief This function determines whether pt_point is in pt_rect.
 *  @param [in]   pt_rect Pointer to the rectangle.
 *  @param [in]   pt_point    Pointer to the point.
 *  @param [out]  pb_is_included	Pointer to a BOOL variable to receive the result.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK     The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument pt_rect, pt_point, or pb_is_included is NULL.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_rect_is_point_included (
        BOOL*                           pb_is_included,
        const GL_RECT_T*                pt_rect,
        const GL_POINT_T*               pt_point);


#ifdef __cplusplus
}
#endif

#endif
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_WGL*/
/*----------------------------------------------------------------------------*/

