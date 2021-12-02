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
/*! @file u_wgl_rect.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains public interfaces, constant and type 
 *         definitions of the Widget Library.
 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_WGL
 *  @{ 
 */
/*----------------------------------------------------------------------------*/
 
#ifndef _U_WGL_RECT_H_
#define _U_WGL_RECT_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_gl.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define WGL_RECT_GET_WIDTH(pt_rect)                                                                     \
        (((pt_rect)->i4_right > (pt_rect)->i4_left) ?                                                   \
                                                ((pt_rect)->i4_right - (pt_rect)->i4_left) :            \
                                                ((pt_rect)->i4_left - (pt_rect)->i4_right))    /**<   get rectangle's width,  
                                                                                                                                                                                pt_rect is GL_RECT_T pointer,can not be NULL   */

#define WGL_RECT_GET_HEIGHT(pt_rect)                                                                    \
        (((pt_rect)->i4_bottom > (pt_rect)->i4_top) ?                                                   \
                                                ((pt_rect)->i4_bottom - (pt_rect)->i4_top) :            \
                                                ((pt_rect)->i4_top - (pt_rect)->i4_bottom))     /**<   get rectangle's height, 
                                                                                                                                                                                pt_rect is GL_RECT_T pointer,can not be NULL   */

#define WGL_RECT_SET(pt_rect, left, top, right, bottom)                                         \
        {                                                                                       \
                (pt_rect)->i4_left = ((left) < (right)) ? (left) : (right);                     \
                (pt_rect)->i4_right = ((left) < (right)) ? (right) : (left);                    \
                (pt_rect)->i4_top = ((top) < (bottom)) ? (top) : (bottom);                      \
                (pt_rect)->i4_bottom = ((top) < (bottom)) ? (bottom) : (top);                   \
        }    /**<   set rectangle's value,  
                                pt_rect is GL_RECT_T pointer, can not be NULL ,
                                left, top, right, bottom is INT32 value  */

#define WGL_RECT_SET_BY_SIZE(pt_rect, left, top, width, height)                                 \
        {                                                                                       \
                (pt_rect)->i4_left = ((width) > 0) ? (left) : ((left) + (width));               \
                (pt_rect)->i4_right = ((width) > 0) ? ((left) + (width)) : (left);              \
                (pt_rect)->i4_top = ((height) > 0) ? (top) : ((top) + (height));                \
                (pt_rect)->i4_bottom = ((height) > 0) ? ((top) + (height)) : (top);             \
        }    /**<        */

#define WGL_RECT_COPY(pt_dest_rect, pt_src_rect)                                                \
        {                                                                                       \
                (pt_dest_rect)->i4_left = (pt_src_rect)->i4_left;                               \
                (pt_dest_rect)->i4_right = (pt_src_rect)->i4_right;                             \
                (pt_dest_rect)->i4_top = (pt_src_rect)->i4_top;                                 \
                (pt_dest_rect)->i4_bottom = (pt_src_rect)->i4_bottom;                           \
        }    /**<        */

#define WGL_RECT_INFLATE(pt_rect, i4_cx, i4_cy)                                                 \
        {                                                                                       \
                (pt_rect)->i4_right += i4_cx;                                                   \
                (pt_rect)->i4_bottom += i4_cy;                                                  \
        }    /**<        */

#define WGL_RECT_OFFSET(pt_rect, i4_x, i4_y)                                                    \
        {                                                                                       \
                (pt_rect)->i4_left += i4_x;                                                     \
                (pt_rect)->i4_right += i4_x;                                                    \
                (pt_rect)->i4_top += i4_y;                                                      \
                (pt_rect)->i4_bottom += i4_y;                                                   \
        }    /**<        */

#define WGL_RECT_INSET(pt_rect,i4_l,i4_t,i4_r,i4_b)                                             \
        {                                                                                       \
                (pt_rect)->i4_left += i4_l;                                                     \
                (pt_rect)->i4_right -= i4_r;                                                    \
                (pt_rect)->i4_top += i4_t;                                                      \
                (pt_rect)->i4_bottom -= i4_b;                                                   \
        }    /**<        */

#define WGL_RECT_INSET_W_CHECK(pt_rect,i4_l,i4_t,i4_r,i4_b)                                     \
        {                                                                                       \
                (pt_rect)->i4_left += i4_l;                                                     \
                (pt_rect)->i4_right -= i4_r;                                                    \
                (pt_rect)->i4_top += i4_t;                                                      \
                (pt_rect)->i4_bottom -= i4_b;                                                   \
                (pt_rect)->i4_right = (((pt_rect)->i4_left) >= ((pt_rect)->i4_right)) ? ((pt_rect)->i4_left) : ((pt_rect)->i4_right);  \
                (pt_rect)->i4_bottom = (((pt_rect)->i4_top) >= ((pt_rect)->i4_bottom)) ? ((pt_rect)->i4_top) : ((pt_rect)->i4_bottom); \
        }    /**<        */


#define WGL_RECT_EMPTY(pt_rect)                                                                 \
        {                                                                                       \
                (pt_rect)->i4_left = 0;                                                         \
                (pt_rect)->i4_right = 0;                                                        \
                (pt_rect)->i4_top = 0;                                                          \
                (pt_rect)->i4_bottom = 0;                                                       \
        }    /**<        */
        
#define WGL_RECT_NORMALIZE(pt_rect)                                                             \
        {                                                                                       \
            INT32   i4_temp;                                                                    \
                                                                                                \
            if((pt_rect)->i4_left > (pt_rect)->i4_right)                                        \
            {                                                                                   \
                i4_temp = (pt_rect)->i4_left;                                                   \
                (pt_rect)->i4_left = (pt_rect)->i4_right;                                       \
                (pt_rect)->i4_right = i4_temp;                                                  \
            }                                                                                   \
                                                                                                \
            if((pt_rect)->i4_top > (pt_rect)->i4_bottom)                                        \
            {                                                                                   \
                i4_temp = (pt_rect)->i4_top;                                                    \
                (pt_rect)->i4_top = (pt_rect)->i4_bottom;                                       \
                (pt_rect)->i4_bottom = i4_temp;                                                 \
            }                                                                                   \
        }    /**<        */

#define WGL_RECT_IS_EQUAL(pt_rect1, pt_rect2)                                                   \
        ( ((pt_rect1)->i4_left == (pt_rect2)->i4_left)                                          \
                && ((pt_rect1)->i4_right == (pt_rect2)->i4_right)                               \
                && ((pt_rect1)->i4_top == (pt_rect2)->i4_top)                                   \
                && ((pt_rect1)->i4_bottom == (pt_rect2)->i4_bottom))    /**<        */
                
#define WGL_RECT_IS_EMPTY(pt_rect)                                                              \
        ( ((pt_rect)->i4_left == (pt_rect)->i4_right)                                           \
                || ((pt_rect)->i4_top == (pt_rect)->i4_bottom) )    /**<        */

#define WGL_RECT_IS_INCLUDED(pt_container, pt_rect)                                             \
        ( !(((pt_rect)->i4_left < (pt_container)->i4_left)                                      \
                || ((pt_rect)->i4_right > (pt_container)->i4_right)                             \
                || ((pt_rect)->i4_top < (pt_container)->i4_top)                                 \
                || ((pt_rect)->i4_bottom > (pt_container)->i4_bottom)) )    /**<        */

#define WGL_RECT_IS_PT_INCLUDED(pt_rect_container, pt_point)                            \
        ( ((pt_point)->i4_x >= (pt_rect_container)->i4_left)                            \
                && ((pt_point)->i4_x <= (pt_rect_container)->i4_right)                  \
                && ((pt_point)->i4_y >= (pt_rect_container)->i4_top)                    \
                && ((pt_point)->i4_y <= (pt_rect_container)->i4_bottom) )    /**<        */

#define WGL_RECT_VT_IS_OVERLAP(pt_rect1, pt_rect2)                                             \
        ( !(((pt_rect1)->i4_top > (pt_rect2)->i4_bottom)                                           \
                || ((pt_rect1)->i4_bottom < (pt_rect2)->i4_top)) )    /**<        */

#define WGL_INTERSECT_RECT(rc_out, rc1, rc2)                                \
    do {                                                                \
        (rc_out)->i4_left   = MAX((rc1)->i4_left, (rc2)->i4_left);      \
        (rc_out)->i4_right  = MIN((rc1)->i4_right, (rc2)->i4_right);    \
        (rc_out)->i4_top    = MAX((rc1)->i4_top, (rc2)->i4_top);        \
        (rc_out)->i4_bottom = MIN((rc1)->i4_bottom, (rc2)->i4_bottom);  \
    } while (0)    /**< intersect two rectangles as rc_out */


#define WGL_RECT_OVERLAPPED(rc1, rc2)                                                       \
        ((!RECT_EMPTY(rc1)) &&                                                              \
         (!RECT_EMPTY(rc2)) &&                                                              \
         (((((rc1)->i4_left >= (rc2)->i4_left) && ((rc1)->i4_left < (rc2)->i4_right))  ||   \
           (((rc1)->i4_right > (rc2)->i4_left) && ((rc1)->i4_right <= (rc2)->i4_right))) && \
          ((((rc1)->i4_top >= (rc2)->i4_top) && ((rc1)->i4_top < (rc2)->i4_bottom))     ||  \
           (((rc1)->i4_bottom > (rc2)->i4_top) && ((rc1)->i4_bottom <= (rc2)->i4_bottom)))))    /**< check if two rectangles overlap */


/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_WGL*/
/*----------------------------------------------------------------------------*/


