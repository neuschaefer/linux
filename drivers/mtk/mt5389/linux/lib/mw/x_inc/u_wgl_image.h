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
/*! @file u_wgl_image.h 
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
 
#ifndef _U_WGL_IMAGE_H_
#define _U_WGL_IMAGE_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_wgl.h"
#include "u_gl.h"
#include "u_wgl_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @enum WGL_TPL_PAINT_METHOD_T
 *  @brief
 *  @code
 *  typedef enum _WGL_TPL_PAINT_METHOD_T
 *  {
 *      WGL_TPL_PAINT_BITBLT = 0,
 *      WGL_TPL_PAINT_COMPOSE
 *  } WGL_TPL_PAINT_METHOD_T;
 *  @endcode
 *  @li@c  WGL_TPL_PAINT_BITBLT                              -Specify the source surface pixel data will replace the destination surface pixel data at the same position.
 *  @li@c  WGL_TPL_PAINT_COMPOSE                             -Specify the source surface pixel data will do alpha composition with the destination surface pixel data at the same position.
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_TPL_PAINT_METHOD_T
{
    WGL_TPL_PAINT_BITBLT = 0,
    WGL_TPL_PAINT_COMPOSE
} WGL_TPL_PAINT_METHOD_T;

#define WGL_IMG_TYPE_ALIGN_MASK                 ((UINT32) 0x80)    /**< image data is four bytes aligned       */
#define WGL_IMG_TYPE_COMPRESSED_MASK            ((UINT32) 0x40)    /**< image is compressed       */
#define WGL_IMG_TYPE_ZIPPED_MASK                ((UINT32) 0x20)    /**<  image is zipped      */

/*Image Type*/
#define WGL_IMG_TYPE_1555                       ((UINT32) 0x01)    /**< Indicate every pixel in WGL_IMAGE_T occupies 16 bits, and 1 bit for alpha, 5 bits for R, 5 bits for G, and 5 bits for B. Besides, it's image data is linear without 4 bytes alignment.     */
#define WGL_IMG_TYPE_1555_ALIGN                 (WGL_IMG_TYPE_1555            | WGL_IMG_TYPE_ALIGN_MASK)    /**< It is the same to WGL_IMG_TYPE_1555 except the image data is 4 bytes alignment. */
#define WGL_IMG_TYPE_1555_ALIGN_ZIPPED          (WGL_IMG_TYPE_1555_ALIGN      | WGL_IMG_TYPE_ZIPPED_MASK)    /**<   image's color mode is ARGB(1555) ,data is four bytes aligned and image is zipped     */
#define WGL_IMG_TYPE_1555_ALIGN_COMPRESSED      (WGL_IMG_TYPE_1555_ALIGN      | WGL_IMG_TYPE_COMPRESSED_MASK)    /**<    image's color mode is ARGB(1555)  ,data is four bytes aligned and image is compressed     */

#define WGL_IMG_TYPE_4444                       ((UINT32) 0x03)    /**<Indicate every pixel in WGL_IMAGE_T occupies 16 bits, and 4 bit for alpha, 4 bits for R, 4 bits for G, and 4 bits for B. Besides, it's image data is linear without 4 bytes alignment.         */
#define WGL_IMG_TYPE_4444_ALIGN                 (WGL_IMG_TYPE_4444            | WGL_IMG_TYPE_ALIGN_MASK)    /**<  		It is the same to WGL_IMG_TYPE_4444 except the image data is 4 bytes alignment.   */
#define WGL_IMG_TYPE_4444_ALIGN_ZIPPED          (WGL_IMG_TYPE_4444_ALIGN      | WGL_IMG_TYPE_ZIPPED_MASK)    /**<  image's color mode is ARGB(4444) ,data is four bytes aligned and image is zipped    */
#define WGL_IMG_TYPE_4444_ALIGN_COMPRESSED      (WGL_IMG_TYPE_4444_ALIGN      | WGL_IMG_TYPE_COMPRESSED_MASK)    /**<  image's color mode is ARGB(4444)  ,data is four bytes aligned and image is compressed     */

#define WGL_IMG_TYPE_CLUT8                      ((UINT32) 0x04)    /**<  image's color mode is CLUT8        */
#define WGL_IMG_TYPE_CLUT8_ALIGN                (WGL_IMG_TYPE_CLUT8           | WGL_IMG_TYPE_ALIGN_MASK)    /**<      image's color mode is CLUT8  and data is four bytes aligned   */
#define WGL_IMG_TYPE_CLUT8_ALIGN_ZIPPED         (WGL_IMG_TYPE_CLUT8_ALIGN     | WGL_IMG_TYPE_ZIPPED_MASK)    /**<      image's color mode is CLUT8 ,data is four bytes aligned and image is zipped    */
#define WGL_IMG_TYPE_CLUT8_ALIGN_COMPRESSED     (WGL_IMG_TYPE_CLUT8_ALIGN     | WGL_IMG_TYPE_COMPRESSED_MASK)    /**<   image's color mode is CLUT8  ,data is four bytes aligned and image is compressed       */

#define WGL_IMG_TYPE_YUV_CLUT8                  ((UINT32) 0x05)    /**<image's color mode is YUV CLUT8        */
#define WGL_IMG_TYPE_YUV_CLUT8_ALIGN            (WGL_IMG_TYPE_YUV_CLUT8       | WGL_IMG_TYPE_ALIGN_MASK)    /**< image's color mode is YUV CLUT8  and data is four bytes aligned     */
#define WGL_IMG_TYPE_YUV_CLUT8_ALIGN_ZIPPED     (WGL_IMG_TYPE_YUV_CLUT8_ALIGN | WGL_IMG_TYPE_ZIPPED_MASK)    /**< image's color mode is YUV CLUT8 ,data is four bytes aligned and image is zipped         */
#define WGL_IMG_TYPE_YUV_CLUT8_ALIGN_COMPRESSED (WGL_IMG_TYPE_YUV_CLUT8_ALIGN | WGL_IMG_TYPE_COMPRESSED_MASK)    /**< image's color mode is YUV CLUT8,data is four bytes aligned and image is compressed          */

#define WGL_IMG_TYPE_8888                       ((UINT32) 0x06)    /**< image's color mode is ARGB(8888)         */
#define WGL_IMG_TYPE_8888_ALIGN                 (WGL_IMG_TYPE_8888            | WGL_IMG_TYPE_ALIGN_MASK)    /**<   image's color mode is ARGB(8888)  and data is four bytes aligned      */
#define WGL_IMG_TYPE_8888_ALIGN_ZIPPED          (WGL_IMG_TYPE_8888_ALIGN      | WGL_IMG_TYPE_ZIPPED_MASK)    /**< image's color mode is ARGB(8888) ,data is four bytes aligned and image is zipped        */
#define WGL_IMG_TYPE_8888_ALIGN_COMPRESSED      (WGL_IMG_TYPE_8888_ALIGN      | WGL_IMG_TYPE_COMPRESSED_MASK)    /**<   image's color mode is ARGB(8888) ,data is four bytes aligned and image is compressed      */

#define WGL_IMG_TYPE_STRETCH_MASK               ((UINT32) 0x01000000)  /**<  Indicate this image should stretch when repaint */


#define WGL_IMG_TYPE_JPG                        ((UINT32) 0x10000000)    /**<  Indicate the buffer in WGL_IMAGE_T is JPG format      */
#define WGL_IMG_TYPE_JPG_4444                   (WGL_IMG_TYPE_4444 | WGL_IMG_TYPE_JPG)    /**<  Indicate the buffer in WGL_IMAGE_T is JPG format      */

#define WGL_IMG_TYPE_PNG                        ((UINT32) 0x20000000)    /**<  Indicate the buffer in WGL_IMAGE_T is JPG format      */
#define WGL_IMG_TYPE_PNG_4444                   (WGL_IMG_TYPE_4444 | WGL_IMG_TYPE_PNG)    /**<  Indicate the buffer in WGL_IMAGE_T is JPG format      */

#define WGL_IMG_TYPE_BMP                        ((UINT32) 0x40000000)    /**<  Indicate the buffer in WGL_IMAGE_T is JPG format      */
#define WGL_IMG_TYPE_BMP_4444                   (WGL_IMG_TYPE_4444 | WGL_IMG_TYPE_BMP)     /**<  Indicate the buffer in WGL_IMAGE_T is JPG format      */

#define WGL_IMG_TYPE_GIF                        ((UINT32) 0x80000000)    /**<  Indicate the buffer in WGL_IMAGE_T is JPG format      */
#define WGL_IMG_TYPE_GIF_4444                   (WGL_IMG_TYPE_4444 | WGL_IMG_TYPE_GIF)     /**<  Indicate the buffer in WGL_IMAGE_T is JPG format      */

#define WGL_IMG_SEGMENT_OFFSET                  (8)    /**< image's segment  is offset       */
#define WGL_IMG_SEGMENT_MASK                    ((UINT32)(0xFF << WGL_IMG_SEGMENT_OFFSET))    /**< image's segment  is mask        */

#define WGL_IMG_SEGMENT_TYPE(e_seg_type)        ((UINT32)(e_seg_type << WGL_IMG_SEGMENT_OFFSET))    /**<  image's segment type      */

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
/*Image Structure*/
/*------------------------------------------------------------------*/
/*! @struct WGL_IMG_T
 *  @brief image related data info
 *  @code
 *  typedef struct _WGL_IMG_T
 *  {
 *          UINT16                          ui2_width;
 *          UINT16                          ui2_lines;
 *          UINT8                           ui1_bits_per_pixel;
 *          UINT32                          ui4_image_type;
 *          UINT16                          ui2_table_columns;
 *          GL_COLOR_T*                     pt_table_data;
 *          UINT32                          ui4_image_size;
 *          UINT8*                          pui1_image_data;
 *  } WGL_IMG_T;
 *  @endcode
 *  @li@c  ui2_width                                         -Specify image's width in pixel.
 *  @li@c  ui2_lines                                         -Specify image's line number.
 *  @li@c  ui1_bits_per_pixel                                -Specify the bit number for each pixel. It must be 16 now.
 *  @li@c  ui4_image_type                                    -Specify the type of the image. It cannot be WGL_IMG_TYPE_JPG now.
 *  @li@c  ui2_table_columns                                 -Specify the count of color index table. It must be 0 now
 *  @li@c  pt_table_data                                     -Specify the color table. It must be NULL now.
 *  @li@c  ui4_image_size                                    -Specify the buffer size pointed by pui1_image_data in bytes.
 *  @li@c  pui1_image_data                                   -Pointer to the image buffer.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_IMG_T
{
        UINT16                          ui2_width;
        UINT16                          ui2_lines;
        UINT8                           ui1_bits_per_pixel;
        UINT32                          ui4_image_type;
        UINT16                          ui2_table_columns;
        GL_COLOR_T*                     pt_table_data;
        UINT32                          ui4_image_size;
        UINT8*                          pui1_image_data;
} WGL_IMG_T;


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


