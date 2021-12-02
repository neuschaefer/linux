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
/*! @file u_gl.h 
 *  $RCSfile: $
 *  $Revision: #2 $
 *  $Date: 2012/06/26 $
 *  $Author: xiaokuan.shi $
 *  
 *  @par Description:
 *         This header file contains Graphics Library specific definitions,
 *         which are exported.
 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_GL  Graphic library
 *
 *  @ingroup groupMW
 *  @brief Provides constant,type definitions and function of GL. GL is a middleware component that is responsible for handling GFX and OSD related operations
 *
 *  %MW Application API would be used by TV Application, lots of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_GL_H_
#define _U_GL_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_sys_name.h"
#include "u_handle.h"
#include "u_fe.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* handle type of root component */
typedef HANDLE_T        GL_HOBJ_T;          /**< GL object handle */

/* handle type of disply objects */
typedef HANDLE_T        GL_HSCREEN_T;       /**< GL screen handle */
typedef HANDLE_T        GL_HPLANE_T;        /**< GL plane handle */
typedef HANDLE_T        GL_HVDP_T;          /**< GL video plane handle */
typedef HANDLE_T        GL_HVIEW_T;         /**< GL view handle */

/* handle type of composite graphics objects */
typedef HANDLE_T        GL_HGRAPHICS_T;     /**< GL graphics handle */

/* handle type of graphics objects */
typedef HANDLE_T        GL_HSURFACE_T;      /**< GL surface handle */

/* handle type of auxiliary objects */
typedef HANDLE_T        GL_HCLIPPER_T;      /**< GL clipper handle */
typedef HANDLE_T        GL_HPALETTE_T;      /**< GL palette handle */

typedef INT32           GL_FIX32_T;         /**< GL 32-bit fixed point type */
typedef UINT16          GL_FIX16_T;         /**< GL 16-bit fixed point type */

#define GL_F32_FRAC     12                  /**< number of bits for GL_FIX32_T in the fractional part */
#define GL_F32_INTR     19                  /**< number of bits for GL_FIX32_T in the integral part */

#define GL_F16_FRAC     12                  /**< number of bits for GL_FIX16_T in the fractional part */
#define GL_F16_INTR     4                   /**< number of bits for GL_FIX16_T in the integral part */

typedef UINT32          GL_STYLE_T;         /**< GL shape style */

/* return values */
#define GLR_NEED_FLUSH                  ((INT32)    1)    /**< notify this operation needs to be flushed to take effect */

#define GLR_OK                          ((INT32)    0)    /**< an okay return value */

#define GLR_INV_ARG                     ((INT32) -  1)    /**< invalid argument is passed */
#define GLR_INV_HANDLE                  ((INT32) -  2)    /**< invalid handle is passed */
#define GLR_INCOMPATIBLE_OBJ            ((INT32) -  3)    /**< incompatible GL object is passed */
#define GLR_NO_MEMORY                   ((INT32) -  4)    /**< no memory to perform this operation */
#define GLR_SURFACE_BUSY                ((INT32) -  5)    /**< this surface is currently being used */
#define GLR_SURFACE_LOCKED              ((INT32) -  6)    /**< this surface is currently being locked */
#define GLR_SURFACE_NOT_LOCKED          ((INT32) -  7)    /**< this surface is not locked */
#define GLR_SURFACE_ALREADY_ATTACHED    ((INT32) -  8)    /**< there is already view and viewport attached on this surface */
#define GLR_NO_PALETTE_ATTACHED         ((INT32) -  9)    /**< no palette is attached on this surface */
#define GLR_NO_CLIPPER_ATTACHED         ((INT32) - 10)    /**< no clipper is attached on this surface */
#define GLR_INCOMPATIBLE_SURFACE        ((INT32) - 11)    /**< input surface(s) is/are not compatible */
#define GLR_NOT_INIT                    ((INT32) - 12)    /**< GL is not initialized */
#define GLR_FLIP_FAILED                 ((INT32) - 13)    /**< the flip operation is failed */
#define GLR_HDL_LIB_ERROR               ((INT32) - 14)    /**< there is error from Handle library */
#define GLR_OUT_OF_HANDLES              ((INT32) - 15)    /**< no handle can be allocated for this operation */
#define GLR_DRV_ERROR                   ((INT32) - 16)    /**< driver error */
#define GLR_PLANE_NOT_LINKED            ((INT32) - 17)    /**< this plane is not linked to a screen */
#define GLR_PLANE_ALREADY_LINKED        ((INT32) - 18)    /**< this plane is already linked to a screen */
#define GLR_OUT_OF_RESOURCE             ((INT32) - 19)    /**< out of resource */
#define GLR_UNLINKABLE                  ((INT32) - 20)    /**< this plane is not able to be linked to a screen */
#define GLR_FE_ERROR                    ((INT32) - 21)    /**< there is error from Font library */
#define GLR_NO_IMP                      ((INT32) - 22)    /**< not implemented yet */
#define GLR_UNSUPPORTED                 ((INT32) - 23)    /**< this operation is not supported */
#define GLR_VDP_ALREADY_LINKED          ((INT32) - 24)    /**< this video plane is already linked */
#define GLR_INV_COLORMODE               ((INT32) - 25)    /**< the input color mode is invalid */

#define GLR_NO_RESOURCE_AVAIL           ((INT32) -100)    /**< no resuorce available */
#define GLR_ALREADY_INIT                ((INT32) -101)    /**< GL has been initialized */
#define GLR_INIT_FAILED                 ((INT32) -102)    /**< GL initialization is failed */

#define GL_SUCCEEDED(x)                 ((x) == GLR_OK)    /**< check if this GL operation is succeeded */
#define GL_FAILED(x)                    ((x) <  GLR_OK)    /**< check if this GL operation is failed */

/* blend option */
#define GL_BLEND_OPT_NONE               ((UINT8) 0)        /**< set the alpha blending mode off */
#define GL_BLEND_OPT_PIXEL              ((UINT8) 1)        /**< use per-pixel alpha blending mode */
#define GL_BLEND_OPT_SURFACE            ((UINT8) 2)        /**< use per-surface alpha blending mode */
#define GL_BLEND_OPT_PLANE              ((UINT8) 3)        /**< use per-plane alpha blending mode */

/* reorder type */
#define GL_REORDER_ABSOLUTE             ((UINT32) 0)       /**< use absolute order for plane reordering */
#define GL_REORDER_RELATIVE             ((UINT32) 1)       /**< use relative order for plane reordering */

/* rendering flags */
#define GL_NOWAIT                       ((UINT32) 0x00000001)       /**< indicate this operation would not wait if the surface is locked */
#define GL_SYNC                         ((UINT32) 0x00000002)       /**< indicate this operation is synchronous */
#define GL_BLT2FRONT                    ((UINT32) 0x08000000)       /**< used for LINUX platform. indicate this operation is blitting contents to the front buffer */

/* shape style */
#define GL_STYLE_OUTLINED               ((GL_STYLE_T) 0x00000001)   /**< render with outline only */
#define GL_STYLE_FILLED                 ((GL_STYLE_T) 0x00000002)   /**< render with color filled */
#define GL_STYLE_SHADOWED               ((GL_STYLE_T) 0x00000004)   /**< render with shadow */
#define GL_STYLE_MASK                   ((GL_STYLE_T) 0x00000007)   /**< shape style mask */

/* display flags for surface */
#define GL_DISP_OPT_POSITION            ((UINT8) 0x01)              /**< indicate position would be set */
#define GL_DISP_OPT_COLORKEY            ((UINT8) 0x02)              /**< indicate colorkey would be set */
#define GL_DISP_OPT_BLEND_OPTION        ((UINT8) 0x04)              /**< indicate blending option would be set */
#define GL_DISP_OPT_ALPHA               ((UINT8) 0x08)              /**< indicate alpha value would be set */
#define GL_DISP_OPT_RESIZE              ((UINT8) 0x10)              /**< indicate resize option would be set */
#define GL_DISP_OPT_SURF_OFFSET         ((UINT8) 0x20)              /**< indicate surface position offset would be set */
#define GL_DISP_OPT_OSD_COMPRESSION     ((UINT8) 0x40)              /**< indicate OSD compression mode would be set */
#define GL_DISP_OPT_ALL                 ((UINT8) 0xff)              /**< indicate all configurable option would be set */

/* shrink setting */
#define GL_SHRINK_SURFACE               ((UINT8) 0)                 /**< the size would be the same as the associated surface */
#define GL_SHRINK_AUTOFIT               ((UINT8) 1)                 /**< use the user specified size with all the surface content fit into it */
#define GL_SHRINK_MANUAL                ((UINT8) 2)                 /**< use manual shrinking setting */

/* transformation type */
#define GL_TRANSFORM_GLOBAL             ((UINT32) 0)                /**< global transformation */
#define GL_TRANSFORM_LOCAL              ((UINT32) 1)                /**< local transformation */

#define GL_BLT_FLIP                     ((UINT32) 0x10000)          /**< indicate the rotation operation comes with content flipping */

#define GL_BLT_ROTATE_MASK              ((UINT32) 0xffff)           /**< mask for rotation operation */
#define GL_BLT_ROTATE_NONE              ((UINT32)      0)           /**< no rotation */
#define GL_BLT_ROTATE_CW90              ((UINT32)     90)           /**< clockwise 90 degree rotation */
#define GL_BLT_ROTATE_CW180             ((UINT32)    180)           /**< clockwise 180 degree rotation */
#define GL_BLT_ROTATE_CW270             ((UINT32)    270)           /**< clockwise 270 degree rotation */

/* maximum number of sub-planes */
#define GL_MAX_SUBPLANE                     ((UINT32) 4)        /**< define the max buffer numbers which can be associated to a surface */

/* alpha-setting operation code */
#define BLT_EX_OPT_ALPHAVAL_COLORMAP_REP    ((UINT8) 0)         /**< use new alpha value to replace alpha data on the color map */
#define BLT_EX_OPT_ALPHAVAL_COLORMAP_MUL    ((UINT8) 1)         /**< multiply new alpha value into the color mamp */
#define BLT_EX_OPT_ALPHAMAP_COLORVAL        ((UINT8) 2)         /**< use alpha map and color value to construct the result */
#define BLT_EX_OPT_ALPHAMAP_COLORMAP        ((UINT8) 3)         /**< use alpha map and color map to construct the result */

/* GPU limitation query */
#define GL_GPU_SCALING_LIMIT                ((UINT32) 0)        /**< GPU capability bit for querying the scaling limit */

#define GL_MAX_TTX_STR_LEN                  (40)                /**< max length of a teletext string */

/* color mode */
/*------------------------------------------------------------------*/
/*! @enum   GL_COLORMODE_T
 *  @brief  specify the color mode
 *  @code
 *  typedef enum _GL_COLORMODE_T
 *  {
 *      GL_COLORMODE_AYUV_CLUT2     = 0,
 *      GL_COLORMODE_AYUV_CLUT4     = 1,
 *      GL_COLORMODE_AYUV_CLUT8     = 2,
 *      GL_COLORMODE_UYVY_16        = 3,
 *      GL_COLORMODE_YUYV_16        = 4,
 *      GL_COLORMODE_AYUV_D8888     = 5,
 *      GL_COLORMODE_ARGB_CLUT2     = 6,
 *      GL_COLORMODE_ARGB_CLUT4     = 7,
 *      GL_COLORMODE_ARGB_CLUT8     = 8,
 *      GL_COLORMODE_RGB_D565       = 9,
 *      GL_COLORMODE_ARGB_D1555     = 10,
 *      GL_COLORMODE_ARGB_D4444     = 11,
 *      GL_COLORMODE_ARGB_D8888     = 12,
 *      GL_COLORMODE_YUV_420_BLK    = 13,
 *      GL_COLORMODE_YUV_420_RS     = 14,
 *      GL_COLORMODE_YUV_422_BLK    = 15,
 *      GL_COLORMODE_YUV_422_RS     = 16,
 *      GL_COLORMODE_YUV_444_BLK    = 17,
 *      GL_COLORMODE_YUV_444_RS     = 18
 *  } GL_COLORMODE_T;
 *  @endcode
 *  @li@c  GL_COLORMODE_AYUV_CLUT2                           -  2-bit AYUV color lookup table
 *  @li@c  GL_COLORMODE_AYUV_CLUT4                           -  4-bit AYUV color lookup table
 *  @li@c  GL_COLORMODE_AYUV_CLUT8                           -  8-bit AYUV color lookup table
 *  @li@c  GL_COLORMODE_UYVY_16                              - 16-bit UYUV
 *  @li@c  GL_COLORMODE_YUYV_16                              - 16-bit YUYV
 *  @li@c  GL_COLORMODE_AYUV_D8888                           - 32-bit AYUV
 *  @li@c  GL_COLORMODE_ARGB_CLUT2                           -  2-bit ARGB color lookup table
 *  @li@c  GL_COLORMODE_ARGB_CLUT4                           -  4-bit ARGB color lookup table
 *  @li@c  GL_COLORMODE_ARGB_CLUT8                           -  8-bit ARGB color lookup table
 *  @li@c  GL_COLORMODE_RGB_D565                             - 16-bit  RGB (565)
 *  @li@c  GL_COLORMODE_ARGB_D1555                           - 16-bit ARGB (1555)
 *  @li@c  GL_COLORMODE_ARGB_D4444                           - 16-bit ARGB (4444)
 *  @li@c  GL_COLORMODE_ARGB_D8888                           - 32-bit ARGB (8888)
 *  @li@c  GL_COLORMODE_YUV_420_BLK                          - YUV 4:2:0 block mode
 *  @li@c  GL_COLORMODE_YUV_420_RS                           - YUV 4:2:0 raster-scan mode
 *  @li@c  GL_COLORMODE_YUV_422_BLK                          - YUV 4:2:2 block mode
 *  @li@c  GL_COLORMODE_YUV_422_RS                           - YUV 4:2:2 raster-scan mode
 *  @li@c  GL_COLORMODE_YUV_444_BLK                          - YUV 4:4:4 block mode
 *  @li@c  GL_COLORMODE_YUV_444_RS                           - YUV 4:4:4 raster-scan mode
 */
/*------------------------------------------------------------------*/
typedef enum _GL_COLORMODE_T
{
    GL_COLORMODE_AYUV_CLUT2     = 0,
    GL_COLORMODE_AYUV_CLUT4     = 1,
    GL_COLORMODE_AYUV_CLUT8     = 2,
    GL_COLORMODE_UYVY_16        = 3,
    GL_COLORMODE_YUYV_16        = 4,
    GL_COLORMODE_AYUV_D8888     = 5,
    GL_COLORMODE_ARGB_CLUT2     = 6,
    GL_COLORMODE_ARGB_CLUT4     = 7,
    GL_COLORMODE_ARGB_CLUT8     = 8,
    GL_COLORMODE_RGB_D565       = 9,
    GL_COLORMODE_ARGB_D1555     = 10,
    GL_COLORMODE_ARGB_D4444     = 11,
    GL_COLORMODE_ARGB_D8888     = 12,
    GL_COLORMODE_YUV_420_BLK    = 13,
    GL_COLORMODE_YUV_420_RS     = 14,
    GL_COLORMODE_YUV_422_BLK    = 15,
    GL_COLORMODE_YUV_422_RS     = 16,
    GL_COLORMODE_YUV_444_BLK    = 17,
    GL_COLORMODE_YUV_444_RS     = 18
} GL_COLORMODE_T;

/*------------------------------------------------------------------*/
/*! @enum   GL_BYTE_ALIGNMENT_T
 *  @brief  bytes alignment format
 *  @code
 *  typedef enum _GL_BYTE_ALIGNMENT_T
 *  {
 *      GL_ALIGNMENT_MSB = 0,
 *      GL_ALIGNMENT_LSB = 1
 *  } GL_BYTE_ALIGNMENT_T;
 *  @endcode
 *  @li@c  GL_ALIGNMENT_MSB                                  - MSB alignment
 *  @li@c  GL_ALIGNMENT_LSB                                  - LSB alignment
 */
/*------------------------------------------------------------------*/
typedef enum _GL_BYTE_ALIGNMENT_T
{
    GL_ALIGNMENT_MSB = 0,
    GL_ALIGNMENT_LSB = 1
} GL_BYTE_ALIGNMENT_T;

/*------------------------------------------------------------------*/
/*! @enum   GL_YCBCR_MODE_T
 *  @brief  specify the YCbCr format
 *  @code
 *  typedef enum _GL_YCBCR_MODE_T
 *  {
 *      GL_YCBCR_420_MB  = 0,
 *      GL_YCBCR_420_LNR = 1,
 *      GL_YCBCR_422_LNR = 2        
 *  } GL_YCBCR_MODE_T;
 *  @endcode
 *  @li@c  GL_YCBCR_420_MB                                   - 4:2:0 macro-block mode
 *  @li@c  GL_YCBCR_420_LNR                                  - 4:2:0 linear mode
 *  @li@c  GL_YCBCR_422_LNR                                  - 4:2:2 linear mode
 */
/*------------------------------------------------------------------*/
typedef enum _GL_YCBCR_MODE_T
{
    GL_YCBCR_420_MB  = 0,
    GL_YCBCR_420_LNR = 1,
    GL_YCBCR_422_LNR = 2        /* MT538x only */
} GL_YCBCR_MODE_T;

/*------------------------------------------------------------------*/
/*! @enum   GL_JPEG_MODE_T
 *  @brief  specify the JPEG YUV mode
 *  @code
 *  typedef enum _GL_JPEG_MODE_T
 *  {
 *      GL_JPEG_444     = 0,
 *      GL_JPEG_422_H   = 1,
 *      GL_JPEG_422_V   = 2,
 *      GL_JPEG_411     = 3
 *  } GL_JPEG_MODE_T;
 *  @endcode
 *  @li@c  GL_JPEG_444                                       - 4:4:4 JPEG
 *  @li@c  GL_JPEG_422_H                                     - 4:2:2 horizontal JPEG
 *  @li@c  GL_JPEG_422_V                                     - 4:2:2 vertical JPEG
 *  @li@c  GL_JPEG_411                                       - 4:1:1 JPEG
 */
/*------------------------------------------------------------------*/
typedef enum _GL_JPEG_MODE_T
{
    GL_JPEG_444     = 0,
    GL_JPEG_422_H   = 1,
    GL_JPEG_422_V   = 2,
    GL_JPEG_411     = 3
} GL_JPEG_MODE_T;

/*------------------------------------------------------------------*/
/*! @enum   GL_ROP_TYPE_T
 *  @brief  specify the ROP type
 *  @code
 *  typedef enum _GL_ROP_TYPE_T
 *  {
 *      GL_ROP_NOT_SRC = 0,
 *      GL_ROP_NOT_DST,
 *      GL_ROP_SRC_XOR_DST,
 *      GL_ROP_SRC_XNOR_DST,
 *      GL_ROP_SRC_AND_DST,
 *      GL_ROP_NOT_SRC_AND_DST,
 *      GL_ROP_SRC_AND_NOT_DST,
 *      GL_ROP_NOT_SRC_AND_NOT_DST,
 *      GL_ROP_SRC_OR_DST,
 *      GL_ROP_NOT_SRC_OR_DST,
 *      GL_ROP_SRC_OR_NOT_DST,
 *      GL_ROP_NOT_SRC_OR_NOT_DST
 *  } GL_ROP_TYPE_T;
 *  @endcode
 *  @li@c  GL_ROP_NOT_SRC                                    - out := !src
 *  @li@c  GL_ROP_NOT_DST                                    - out := !dst
 *  @li@c  GL_ROP_SRC_XOR_DST                                - out := src ^ dst
 *  @li@c  GL_ROP_SRC_XNOR_DST                               - out := src XNOR dst
 *  @li@c  GL_ROP_SRC_AND_DST                                - out := src & dst
 *  @li@c  GL_ROP_NOT_SRC_AND_DST                            - out := !src & dst
 *  @li@c  GL_ROP_SRC_AND_NOT_DST                            - out := src & !dst
 *  @li@c  GL_ROP_NOT_SRC_AND_NOT_DST                        - out := !src & !dst
 *  @li@c  GL_ROP_SRC_OR_DST                                 - out := src | dst
 *  @li@c  GL_ROP_NOT_SRC_OR_DST                             - out := !src | dst
 *  @li@c  GL_ROP_SRC_OR_NOT_DST                             - out := src | !dst
 *  @li@c  GL_ROP_NOT_SRC_OR_NOT_DST                         - out := !src | !dst
 */
/*------------------------------------------------------------------*/
typedef enum _GL_ROP_TYPE_T
{
    GL_ROP_NOT_SRC = 0,
    GL_ROP_NOT_DST,
    GL_ROP_SRC_XOR_DST,
    GL_ROP_SRC_XNOR_DST,
    GL_ROP_SRC_AND_DST,
    GL_ROP_NOT_SRC_AND_DST,
    GL_ROP_SRC_AND_NOT_DST,
    GL_ROP_NOT_SRC_AND_NOT_DST,
    GL_ROP_SRC_OR_DST,
    GL_ROP_NOT_SRC_OR_DST,
    GL_ROP_SRC_OR_NOT_DST,
    GL_ROP_NOT_SRC_OR_NOT_DST
} GL_ROP_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum   GL_DRAW_STR_MODE_T
 *  @brief  specify the string drawing mode
 *  @code
 *  typedef enum _GL_DRAW_STR_MODE_T
 *  {
 *      GL_DRAW_STR_BITBLT,
 *      GL_DRAW_STR_COMPOSE,
 *      GL_DRAW_STR_DEFAULT
 *  } GL_DRAW_STR_MODE_T;
 *  @endcode
 *  @li@c  GL_DRAW_STR_BITBLT                                - draw with bitblt
 *  @li@c  GL_DRAW_STR_COMPOSE                               - draw with composition
 *  @li@c  GL_DRAW_STR_DEFAULT                               - draw with GL default behavior
 */
/*------------------------------------------------------------------*/
typedef enum _GL_DRAW_STR_MODE_T
{
    GL_DRAW_STR_BITBLT,
    GL_DRAW_STR_COMPOSE,
    GL_DRAW_STR_DEFAULT
} GL_DRAW_STR_MODE_T;

/* character orientation */
/*------------------------------------------------------------------*/
/*! @enum   GL_CHAR_ORIENT_T
 *  @brief  set character orientation
 *  @code
 *  typedef enum
 *  {
 *      GL_CHAR_ORIENT_CW,
 *      GL_CHAR_ORIENT_CCW
 *  } GL_CHAR_ORIENT_T;
 *  @endcode
 *  @li@c  GL_CHAR_ORIENT_CW                                 - clockwise orientation
 *  @li@c  GL_CHAR_ORIENT_CCW                                - counter-clockwise orientation
 */
/*------------------------------------------------------------------*/
typedef enum
{
    GL_CHAR_ORIENT_CW,
    GL_CHAR_ORIENT_CCW
} GL_CHAR_ORIENT_T;

/*------------------------------------------------------------------*/
/*! @enum   GL_STRETCH_OPTION
 *  @brief  set the option for stretching algorithm
 *  @code
 *  typedef enum
 *  {
 *      GL_STRETCH_UNKNOWN = 0,
 *      GL_STRETCH_PHOTO   = 1,
 *      GL_STRETCH_UI      = 2
 *  } GL_STRETCH_OPTION;
 *  @endcode
 *  @li@c  GL_STRETCH_UNKNOWN                                - not used option
 *  @li@c  GL_STRETCH_PHOTO                                  - algorithm for photo (natural image)
 *  @li@c  GL_STRETCH_UI                                     - algorithm for UI (sharp-edge image)
 */
/*------------------------------------------------------------------*/
typedef enum
{
    GL_STRETCH_UNKNOWN = 0,
    GL_STRETCH_PHOTO   = 1,
    GL_STRETCH_UI      = 2
} GL_STRETCH_OPTION;

/*------------------------------------------------------------------*/
/*! @enum   GL_COMPRESSION_RATIO_T
 *  @brief  set the OSD compression ratio
 *  @code
 *  typedef enum
 *  {
 *      GL_LOSSLESS        = 0,
 *      GL_LOSSY_LOW       = 1,
 *      GL_LOSSY_MEDIAN    = 2,
 *      GL_LOSSY_HIGH      = 3
 *  } GL_COMPRESSION_RATIO_T;
 *  @endcode
 *  @li@c  GL_LOSSLESS                                       - lossless compression
 *  @li@c  GL_LOSSY_LOW                                      - lossy with low compression
 *  @li@c  GL_LOSSY_MEDIAN                                   - lossy with median compression
 *  @li@c  GL_LOSSY_HIGH                                     - lossy with high compression
 */
/*------------------------------------------------------------------*/
typedef enum
{
    GL_LOSSLESS        = 0,
    GL_LOSSY_LOW       = 1,
    GL_LOSSY_MEDIAN    = 2,
    GL_LOSSY_HIGH      = 3
} GL_COMPRESSION_RATIO_T;

#define GL_GET_COLORMODE_MASK(m)    (MAKE_BIT_MASK_32(((INT32) ((GL_COLORMODE_T) (m)))))    /**< used to create color mode mask */

/* constants */
#define GL_CMCAP_AYUV_CLUT2     GL_GET_COLORMODE_MASK(GL_COLORMODE_AYUV_CLUT2)    /**< AYUV_CLUT2 color mode mask */
#define GL_CMCAP_AYUV_CLUT4     GL_GET_COLORMODE_MASK(GL_COLORMODE_AYUV_CLUT4)    /**< AYUV_CLUT4 color mode mask */
#define GL_CMCAP_AYUV_CLUT8     GL_GET_COLORMODE_MASK(GL_COLORMODE_AYUV_CLUT8)    /**< AYUV_CLUT8 color mode mask */
#define GL_CMCAP_UYVY_16        GL_GET_COLORMODE_MASK(GL_COLORMODE_UYVY_16)       /**< UYUV_16    color mode mask */
#define GL_CMCAP_YUYV_16        GL_GET_COLORMODE_MASK(GL_COLORMODE_YUYV_16)       /**< YUYV_16    color mode mask */
#define GL_CMCAP_AYUV_D8888     GL_GET_COLORMODE_MASK(GL_COLORMODE_AYUV_D8888)    /**< AYUV_D8888 color mode mask */
#define GL_CMCAP_ARGB_CLUT2     GL_GET_COLORMODE_MASK(GL_COLORMODE_ARGB_CLUT2)    /**< ARGB_CLUT2 color mode mask */
#define GL_CMCAP_ARGB_CLUT4     GL_GET_COLORMODE_MASK(GL_COLORMODE_ARGB_CLUT4)    /**< ARGB_CLUT4 color mode mask */
#define GL_CMCAP_ARGB_CLUT8     GL_GET_COLORMODE_MASK(GL_COLORMODE_ARGB_CLUT8)    /**< ARGB_CLUT8 color mode mask */
#define GL_CMCAP_RGB_D565       GL_GET_COLORMODE_MASK(GL_COLORMODE_RGB_D565)      /**< RGB_565    color mode mask */
#define GL_CMCAP_ARGB_D1555     GL_GET_COLORMODE_MASK(GL_COLORMODE_ARGB_D1555)    /**< ARGB_1555  color mode mask */
#define GL_CMCAP_ARGB_D4444     GL_GET_COLORMODE_MASK(GL_COLORMODE_ARGB_D4444)    /**< ARGB_4444  color mode mask */
#define GL_CMCAP_ARGB_D8888     GL_GET_COLORMODE_MASK(GL_COLORMODE_ARGB_D8888)    /**< ARGB_8888  color mode mask */
#define GL_CMCAP_ALL            ((UINT32) 0xffffffff)                             /**< all        color mode mask */

/* color composition macro */
#define GL_COLOR_RGB565(r, g, b)        (((((UINT32) (r)) & 0xf8) << 8) | ((((UINT32) (g)) & 0xfc) << 3) | ((((UINT32) (b)) & 0xf8) >> 3))  /**< compose an RGB_565 value */
#if 0
#define GL_COLOR_ARGB1555(a, r, g, b)   (((((UINT32) (a)) & 0x80) << 8) | ((((UINT32) (r)) & 0xf8) << 7) | ((((UINT32) (g)) & 0xf8) << 2) | ((((UINT32) (b)) & 0xf8) >> 3))    /**<        */
#else
#define GL_COLOR_ARGB1555(a, r, g, b)   (((((UINT32) (a)) > 0) ? (UINT32) 0x8000 : 0) | ((((UINT32) (r)) & 0xf8) << 7) | ((((UINT32) (g)) & 0xf8) << 2) | ((((UINT32) (b)) & 0xf8) >> 3))   /**< compose an ARGB_1555 value */
#endif
#define GL_COLOR_ARGB4444(a, r, g, b)   (((((UINT32) (a)) & 0xf0) << 8) | ((((UINT32) (r)) & 0xf0) << 4) | (((UINT32) (g)) & 0xf0) | ((((UINT32) (b)) & 0xf0) >> 4))    /**< compose an ARGB_4444 value */
#define GL_COLOR_ARGB8888(a, r, g, b)   (((((UINT32) (a)) & 0xff) << 24) | ((((UINT32) (r)) & 0xff) << 16) | ((((UINT32) (g)) & 0xff) << 8) | (((UINT32) (b)) & 0xff))  /**< compose an ARGB_8888 value */

#define GL_IS_PALETTE_MODE(m)                                   \
    ((((GL_COLORMODE_T) (m)) == GL_COLORMODE_ARGB_CLUT8) ||    \
     (((GL_COLORMODE_T) (m)) == GL_COLORMODE_ARGB_CLUT4) ||    \
     (((GL_COLORMODE_T) (m)) == GL_COLORMODE_ARGB_CLUT2) ||    \
     (((GL_COLORMODE_T) (m)) == GL_COLORMODE_AYUV_CLUT8) ||    \
     (((GL_COLORMODE_T) (m)) == GL_COLORMODE_AYUV_CLUT4) ||    \
     (((GL_COLORMODE_T) (m)) == GL_COLORMODE_AYUV_CLUT2))    /**< check if this color mode is palette mode */

#define GL_IS_COLORMODE_WITH_ALPHA(m)                         \
    ((((GL_COLORMODE_T) (m)) != GL_COLORMODE_RGB_D565) &&    \
     (((GL_COLORMODE_T) (m)) != GL_COLORMODE_UYVY_16)  &&    \
     (((GL_COLORMODE_T) (m)) != GL_COLORMODE_YUYV_16))    /**< check if this color mode supports alpha channel */

/* composition rule */
/*------------------------------------------------------------------*/
/*! @enum   GL_COMPOSE_RULE_T
 *  @brief  specify the composition mode
 *  @code
 *  typedef enum
 *  {
 *      GL_CLEAR = 0,
 *      GL_DST_IN,
 *      GL_DST_OUT,
 *      GL_DST_OVER,
 *      GL_SRC,
 *      GL_SRC_IN,
 *      GL_SRC_OUT,
 *      GL_SRC_OVER
 *  } GL_COMPOSE_RULE_T;
 *  @endcode
 *  @li@c  GL_CLEAR                                          - clear
 *  @li@c  GL_DST_IN                                         - destination in
 *  @li@c  GL_DST_OUT                                        - destination out
 *  @li@c  GL_DST_OVER                                       - destination over
 *  @li@c  GL_SRC                                            - source
 *  @li@c  GL_SRC_IN                                         - source in
 *  @li@c  GL_SRC_OUT                                        - source out
 *  @li@c  GL_SRC_OVER                                       - source over
 */
/*------------------------------------------------------------------*/
typedef enum
{
    GL_CLEAR = 0,
    GL_DST_IN,
    GL_DST_OUT,
    GL_DST_OVER,
    GL_SRC,
    GL_SRC_IN,
    GL_SRC_OUT,
    GL_SRC_OVER
} GL_COMPOSE_RULE_T;

typedef UINT32 GL_COND_T;    /**< screen condition notification type */

#define GL_COND_DISPLAY_CHANGED     ((GL_COND_T)  1)    /**< resolution change */
#define GL_COND_PLANE_REORDERED     ((GL_COND_T)  2)    /**< plane reordered */
#define GL_COND_VIEW_DEACTIVATED    ((GL_COND_T)  3)    /**< view deactivation */

/*------------------------------------------------------------------*/
/*! @brief  notification function when there is screen state change
 *  @param [in]     h_obj       - screen handle
 *  @param [in]     e_nfy_cond  - screen condition
 *  @param [in]     pv_nfy_tag  - user assigned tag
 *  @param [in]     ui4_data    - composite screen resolution
 *  @note
 *  @see
 *  @return         meaningless
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_gl_nfy_fct)(HANDLE_T     h_obj,
                               GL_COND_T    e_nfy_cond,
                               VOID*        pv_nfy_tag,
                               UINT32       ui4_data);

#ifndef MIN
#define MIN(x, y)           (((x) < (y)) ? (x) : (y))    /**< macro for finding minimum */
#endif

#ifndef MAX
#define MAX(x, y)           (((x) > (y)) ? (x) : (y))    /**< macro for finding maximum */
#endif

#ifndef ABS
#define ABS(x)              (((x) > 0) ? (x) : (-(x)))    /**< macro for getting absolute value */
#endif

#define RECT_EMPTY(rect) \
            (((rect)->i4_right <= (rect)->i4_left) || ((rect)->i4_bottom <= (rect)->i4_top))    /**< check if this rectangle is empty */

#define RECT_W(rect)    ((rect)->i4_right - (rect)->i4_left)    /**< get rectangle width */
#define RECT_H(rect)    ((rect)->i4_bottom - (rect)->i4_top)    /**< get rectangle height */

#define SET_RECT(rect, l, t, r, b)  \
    do {                            \
        (rect)->i4_left     = (l);  \
        (rect)->i4_top      = (t);  \
        (rect)->i4_right    = (r);  \
        (rect)->i4_bottom   = (b);  \
    } while (0)    /**< setup a rectangle by coordinates */

#define SET_RECT_BY_SIZE(rc, l, t, w, h)        \
    do {                                \
        (rc)->i4_left   = (l);          \
        (rc)->i4_top    = (t);          \
        (rc)->i4_right  = (l) + (w);    \
        (rc)->i4_bottom = (t) + (h);    \
    } while (0)    /**< setup a rectangle by size */
    
#define WITHIN_RECT(rect, x, y)                                 \
    ((!RECT_EMPTY(rect)) &&                                     \
     (((x) >= (rect)->i4_left) && ((x) < (rect)->i4_right)) &&  \
     (((y) >= (rect)->i4_top)  && ((y) < (rect)->i4_bottom)))    /**< check if a point is within a specified rectangle */
     
#define MOVE_RECT(rect, x, y)       \
    do {                            \
        (rect)->i4_left     += (x); \
        (rect)->i4_top      += (y); \
        (rect)->i4_right    += (x); \
        (rect)->i4_bottom   += (y); \
    } while (0)    /**< move the position of a rectangle by (x, y) */

#define EXPAND_RECT(rect, x)        \
    do {                            \
        (rect)->i4_left     -= (x); \
        (rect)->i4_top      -= (x); \
        (rect)->i4_right    += (x); \
        (rect)->i4_bottom   += (x); \
    } while (0)    /**< expand the size of a rectangle x pixels */

#define SHRINK_RECT(rect, x)        EXPAND_RECT(rect, -(x))    /**< shrink the size of a rectangle x pixels */
    
#define INTERSECT_RECT(rc_out, rc1, rc2)                                \
    do {                                                                \
        (rc_out)->i4_left   = MAX((rc1)->i4_left, (rc2)->i4_left);      \
        (rc_out)->i4_right  = MIN((rc1)->i4_right, (rc2)->i4_right);    \
        (rc_out)->i4_top    = MAX((rc1)->i4_top, (rc2)->i4_top);        \
        (rc_out)->i4_bottom = MIN((rc1)->i4_bottom, (rc2)->i4_bottom);  \
    } while (0)    /**< intersect two rectangles as rc_out */

#define UNION_RECT(rc_out, rc1, rc2)                                    \
    do {                                                                \
        (rc_out)->i4_left   = MIN((rc1)->i4_left, (rc2)->i4_left);      \
        (rc_out)->i4_right  = MAX((rc1)->i4_right, (rc2)->i4_right);    \
        (rc_out)->i4_top    = MIN((rc1)->i4_top, (rc2)->i4_top);        \
        (rc_out)->i4_bottom = MAX((rc1)->i4_bottom, (rc2)->i4_bottom);  \
    } while (0)    /**< union two rectangles as rc_out */

#define RECT_OVERLAPPED(rc1, rc2)                                                       \
    ((!RECT_EMPTY(rc1)) &&                                                              \
     (!RECT_EMPTY(rc2)) &&                                                              \
     (((((rc1)->i4_left >= (rc2)->i4_left) && ((rc1)->i4_left < (rc2)->i4_right))  ||   \
       (((rc1)->i4_right > (rc2)->i4_left) && ((rc1)->i4_right <= (rc2)->i4_right))) && \
      ((((rc1)->i4_top >= (rc2)->i4_top) && ((rc1)->i4_top < (rc2)->i4_bottom))     ||  \
       (((rc1)->i4_bottom > (rc2)->i4_top) && ((rc1)->i4_bottom <= (rc2)->i4_bottom)))))    /**< check if two rectangles overlap */

#define RECT_INCLUDED(rc_in, rc_out)                        \
    (((rc_out)->i4_left <= (rc_in)->i4_left) &&             \
     ((rc_out)->i4_top <= (rc_in)->i4_top) &&               \
     ((rc_out)->i4_right >= (rc_in)->i4_right) &&           \
     ((rc_out)->i4_bottom >= (rc_in)->i4_bottom))    /**< check if rc_out includes rc_in */
    
#define SET_COLOR(color, ca, cr, cg, cb)                                \
    do {                                                                \
        (color)->a      = (ca);                                         \
        (color)->u1.r   = (cr);                                         \
        (color)->u2.g   = (cg);                                         \
        (color)->u3.b   = (cb);                                         \
    } while (0)    /**< set a GL_COLOR_T structure by inputting ARGB directly */

/* plane functionality flags */
#define GL_CAPS_ADV_SCALER              ((UINT32) 0x01)    /**< flag to indicate if there is an advanced scaler on this OSD plane */
#define GL_CAPS_NO_ALPHA_AREA           ((UINT32) 0x02)    /**< flag to indicate if there is no alpha area on this OSD plane */
#define GL_CAPS_FADING_RATIO            ((UINT32) 0x04)    /**< flag to indicate if this OSD plane supports fading ratio */
#define GL_CAPS_PLANE_REPOS             ((UINT32) 0x08)    /**< flag to indicate if this OSD plane supports reposition */
#define GL_CAPS_SCART_OUT               ((UINT32) 0x10)    /**< flag to indicate if this OSD plane supports SCART out */

/* scaler buffer arrangement */
#define GL_SCALER_BUF_SHARED            ((UINT8) 0)    /**< use shared scaler buffer */
#define GL_SCALER_BUF_EXCLUSIVE         ((UINT8) 1)    /**< use exclusive scaler buffer */

/* scaler color mode setting */
#define GL_SCALER_CM_16BPP              ((UINT8) 0)    /**< set the scaler color mode to 16bpp */
#define GL_SCALER_CM_32BPP              ((UINT8) 1)    /**< set the scaler color mode to 32bpp */

typedef UINT32                          GL_TTX_STR_SIZE_T;    /**< specify the teletext string size */

#define GL_TTX_STR_NORMAL               (0x00)    /**< normal TTX size */
#define GL_TTX_STR_DBL_WIDTH            (0x01)    /**< double width TTX size */
#define GL_TTX_STR_DBL_HEIGHT           (0x02)    /**< double height TTX size */
#define GL_TTX_STR_DBL_SIZE             (GL_TTX_STR_DBL_WIDTH | GL_TTX_STR_DBL_HEIGHT)    /**< double width & height TTX size */
#define GL_TTX_STR_UPPER                (0x10)    /**< only render the upper half of this string */
#define GL_TTX_STR_LOWER                (0x20)    /**< only render the lower half of this string */

#if 1
#define GLPMX_MAIN 0 // main video
#define GLPMX_PIP  1 // pip video
#define GLPMX_OSD1 3 // osd1
#define GLPMX_OSD2 4 // osd2


#define GLPMX_ORDER1 0 // top plane
#define GLPMX_ORDER2 1
#define GLPMX_ORDER3 2
#define GLPMX_ORDER4 3 // bottom plane

#endif

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
#if 0
/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _GL_SCREEN_CAPS_T
 *  {
 *      GL_SCREEN_RESOLUTION_CAP_T  ui4_resolution_caps;
 *  } GL_SCREEN_CAPS_T;
 *  @endcode
 *  @li@c  ui4_resolution_caps                               -
 */
/*------------------------------------------------------------------*/
typedef struct _GL_SCREEN_CAPS_T
{
    GL_SCREEN_RESOLUTION_CAP_T  ui4_resolution_caps;
} GL_SCREEN_CAPS_T;
#endif
/*------------------------------------------------------------------*/
/*! @struct GL_SCREEN_STATUS_T
 *  @brief  used for querying the screen status
 *  @code
 *  typedef struct _GL_SCREEN_STATUS_T
 *  {
 *      UINT32  ui2_width;
 *      UINT32  ui2_height;
 *      BOOL    b_progressive;
 *  } GL_SCREEN_STATUS_T;
 *  @endcode
 *  @li@c  ui2_width                                         - screen width
 *  @li@c  ui2_height                                        - screen height
 *  @li@c  b_progressive                                     - if this screen is in progressive mode
 */
/*------------------------------------------------------------------*/
typedef struct _GL_SCREEN_STATUS_T
{
    UINT32  ui2_width;
    UINT32  ui2_height;
    BOOL    b_progressive;
} GL_SCREEN_STATUS_T;

/*------------------------------------------------------------------*/
/*! @struct GL_PLANE_CAPS_T
 *  @brief  describe the plane capability
 *  @code
 *  typedef struct _GL_PLANE_CAPS_T
 *  {
 *      UINT32  ui4_cm_caps;
 *      UINT32  ui4_z_mask;
 *      UINT32  ui4_func_caps;
 *  } GL_PLANE_CAPS_T;
 *  @endcode
 *  @li@c  ui4_cm_caps                                       - capability bits for supported color modes
 *  @li@c  ui4_z_mask                                        - supported z-order
 *  @li@c  ui4_func_caps                                     - functional capability
 */
/*------------------------------------------------------------------*/
typedef struct _GL_PLANE_CAPS_T
{
    UINT32  ui4_cm_caps;
    UINT32  ui4_z_mask;
    UINT32  ui4_func_caps;
} GL_PLANE_CAPS_T;

/*------------------------------------------------------------------*/
/*! @struct GL_PLANE_CFG_T
 *  @brief  OSD scaler configuration
 *  @code
 *  typedef struct _GL_PLANE_CFG_T
 *  {
 *      UINT8   ui1_sc_buf;
 *      UINT8   ui1_sc_cm;
 *      BOOL    b_filtering;
 *  } GL_PLANE_CFG_T;
 *  @endcode
 *  @li@c  ui1_sc_buf                                        - scaler buffer usage mode (share/exclusive)
 *  @li@c  ui1_sc_cm                                         - color mode for scaler buffer
 *  @li@c  b_filtering                                       - apply filtering or not
 */
/*------------------------------------------------------------------*/
typedef struct _GL_PLANE_CFG_T
{
    UINT8   ui1_sc_buf;
    UINT8   ui1_sc_cm;
    BOOL    b_filtering;
} GL_PLANE_CFG_T;

/*------------------------------------------------------------------*/
/*! @struct GL_PLANE_STATUS_T
 *  @brief  used to query the plane status
 *  @code
 *  typedef struct _GL_PLANE_STATUS_T
 *  {
 *      CHAR                s_screen_name[SYS_NAME_LEN + 1];
 *      UINT8               ui1_z;
 *      UINT8               ui1_alpha;
 *      GL_PLANE_CFG_T      t_pln_cfg;
 *  } GL_PLANE_STATUS_T;
 *  @endcode
 *  @li@c  s_screen_name[SYS_NAME_LEN + 1]                   - associated screen name
 *  @li@c  ui1_z                                             - z order
 *  @li@c  ui1_alpha                                         - alpha value
 *  @li@c  t_pln_cfg                                         - data of GL_PLANE_CFG_T
 */
/*------------------------------------------------------------------*/
typedef struct _GL_PLANE_STATUS_T
{
    CHAR                s_screen_name[SYS_NAME_LEN + 1];
    UINT8               ui1_z;
    UINT8               ui1_alpha;
    GL_PLANE_CFG_T      t_pln_cfg;
} GL_PLANE_STATUS_T;

/*------------------------------------------------------------------*/
/*! @struct GL_POINT_T
 *  @brief  describe the position of a point
 *  @code
 *  typedef struct _GL_POINT_T
 *  {
 *      INT32   i4_x;
 *      INT32   i4_y;
 *  } GL_POINT_T;
 *  @endcode
 *  @li@c  i4_x                                              - x coordinate
 *  @li@c  i4_y                                              - y coordinate
 */
/*------------------------------------------------------------------*/
typedef struct _GL_POINT_T
{
    INT32   i4_x;
    INT32   i4_y;
} GL_POINT_T;

/*------------------------------------------------------------------*/
/*! @struct GL_SIZE_T
 *  @brief  describe a rectangle size
 *  @code
 *  typedef struct _GL_SIZE_T
 *  {
 *      UINT32  ui4_width;
 *      UINT32  ui4_height;
 *  } GL_SIZE_T;
 *  @endcode
 *  @li@c  ui4_width                                         - width of this rectangle
 *  @li@c  ui4_height                                        - height of this rectangle
 */
/*------------------------------------------------------------------*/
typedef struct _GL_SIZE_T
{
    UINT32  ui4_width;
    UINT32  ui4_height;
} GL_SIZE_T;

/*------------------------------------------------------------------*/
/*! @struct GL_RECT_T
 *  @brief  describe a rectangle
 *  @code
 *  typedef struct _GL_RECT_T
 *  {
 *      INT32   i4_left;
 *      INT32   i4_top;
 *      INT32   i4_right;
 *      INT32   i4_bottom;
 *  } GL_RECT_T;
 *  @endcode
 *  @li@c  i4_left                                           - left coordinate of this rectangle
 *  @li@c  i4_top                                            - top coordinate of this rectangle
 *  @li@c  i4_right                                          - right coordinate (exclusive) of this rectangle
 *  @li@c  i4_bottom                                         - bottom coordinate (exclusive) of this rectangle
 */
/*------------------------------------------------------------------*/
typedef struct _GL_RECT_T
{
    INT32   i4_left;
    INT32   i4_top;
    INT32   i4_right;
    INT32   i4_bottom;
} GL_RECT_T;

/*------------------------------------------------------------------*/
/*! @struct GL_MATRIX2D_T
 *  @brief  describe a 2x3 matrix
 *  @code
 *  typedef struct _GL_MATRIX2D_T
 *  {
 *      GL_FIX32_T  fx_m00;
 *      GL_FIX32_T  fx_m01;
 *      GL_FIX32_T  fx_m02;
 *      GL_FIX32_T  fx_m10;
 *      GL_FIX32_T  fx_m11;
 *      GL_FIX32_T  fx_m12;
 *  } GL_MATRIX2D_T;
 *  @endcode
 *  @li@c  fx_m00                                            - element of m00
 *  @li@c  fx_m01                                            - element of m01
 *  @li@c  fx_m02                                            - element of m02
 *  @li@c  fx_m10                                            - element of m10
 *  @li@c  fx_m11                                            - element of m11
 *  @li@c  fx_m12                                            - element of m12
 */
/*------------------------------------------------------------------*/
typedef struct _GL_MATRIX2D_T
{
    GL_FIX32_T  fx_m00;
    GL_FIX32_T  fx_m01;
    GL_FIX32_T  fx_m02;
    GL_FIX32_T  fx_m10;
    GL_FIX32_T  fx_m11;
    GL_FIX32_T  fx_m12;
} GL_MATRIX2D_T;

/*------------------------------------------------------------------*/
/*! @struct GL_COLOR_T
 *  @brief  describe a RGB/YUV/CLUT color
 *  @code
 *  typedef struct _GL_COLOR_T
 *  {
 *      UINT8   a;
 *      union {
 *          UINT8   r;
 *          UINT8   y;
 *      } u1;
 *      union {
 *          UINT8   g;
 *          UINT8   u;
 *      } u2;
 *      union {
 *          UINT8   b;
 *          UINT8   v;
 *          UINT8   index;
 *      } u3;
 *  } GL_COLOR_T;
 *  @endcode
 *  @li@c  a                                                 - alpha channel
 *  @li@c  r                                                 - r channel of RGB
 *  @li@c  y                                                 - y channel of YUV
 *  @li@c  g                                                 - g channel of RGB
 *  @li@c  u                                                 - u channel of YUV
 *  @li@c  b                                                 - b channel of RGB
 *  @li@c  v                                                 - v channel of YUV
 *  @li@c  index                                             - index of the color lookup table
 */
/*------------------------------------------------------------------*/
typedef struct _GL_COLOR_T
{
    UINT8   a;

    union {
        UINT8   r;
        UINT8   y;
    } u1;

    union {
        UINT8   g;
        UINT8   u;
    } u2;
    
    union {
        UINT8   b;
        UINT8   v;
        UINT8   index;
    } u3;
} GL_COLOR_T;

/*------------------------------------------------------------------*/
/*! @struct GL_DISPLAY_OPT_T
 *  @brief  configuration used for setup a viewport
 *  @code
 *  typedef struct _GL_DISPLAY_OPT_T
 *  {
 *      UINT8               e_flag;
 *      UINT8               e_blend_option;
 *      UINT8               ui1_alpha;
 *      GL_COLOR_T          t_colorkey;
 *      GL_POINT_T          t_pos;
 *      GL_SIZE_T           t_size;
 *      UINT8               e_shrink_opt;
 *      GL_FIX16_T          fx_shrink_x;
 *      GL_FIX16_T          fx_shrink_y;
 *      UINT32              ui4_surf_offset_x;
 *      UINT32              ui4_surf_offset_y;
 *      BOOL                b_decompression;
 *      BOOL                b_line_separate;
 *      UINT8               ui1_compression_ratio;
 *      UINT8               ui1_extra_line_size;
 *  } GL_DISPLAY_OPT_T;
 *  @endcode
 *  @li@c  e_flag                                            - describe what options would be set with this structure
 *  @li@c  e_blend_option                                    - specify the blend option
 *  @li@c  ui1_alpha                                         - specify the alpha value
 *  @li@c  t_colorkey                                        - specify the colorkey
 *  @li@c  t_pos                                             - specify the position
 *  @li@c  t_size                                            - specify the size
 *  @li@c  e_shrink_opt                                      - specify the shrinking option
 *  @li@c  fx_shrink_x                                       - specify the shrinking ratio in x direction
 *  @li@c  fx_shrink_y                                       - specify the shrinking ratio in y direction
 *  @li@c  ui4_surf_offset_x                                 - specify the x-direction offset of a surface
 *  @li@c  ui4_surf_offset_y                                 - specify the y-direction offset of a surface
 *  @li@c  b_decompression                                   - specify the OSD compression mode
 *  @li@c  b_line_separate                                   - specify OSD compression with line separate or not
 *  @li@c  ui1_compression_ratio                             - specify the OSD compression ratio
 *  @li@c  ui1_extra_line_size                               - specify the extra buffer line size for OSD compression
 */
/*------------------------------------------------------------------*/
typedef struct _GL_DISPLAY_OPT_T
{
    UINT8               e_flag;
    UINT8               e_blend_option;
    UINT8               ui1_alpha;
    GL_COLOR_T          t_colorkey;
    GL_POINT_T          t_pos;
    GL_SIZE_T           t_size;
    UINT8               e_shrink_opt;
    GL_FIX16_T          fx_shrink_x;
    GL_FIX16_T          fx_shrink_y;
    UINT32              ui4_surf_offset_x;
    UINT32              ui4_surf_offset_y;
    BOOL                b_decompression;
    BOOL                b_line_separate;
    UINT8               ui1_compression_ratio;
    UINT8               ui1_extra_line_size;
} GL_DISPLAY_OPT_T;

/*------------------------------------------------------------------*/
/*! @struct GL_SURFACE_DESC_T
 *  @brief  description for creating a surface
 *  @code
 *  typedef struct _GL_SURFACE_DESC_T
 *  {
 *      UINT32          ui4_width;
 *      UINT32          ui4_height;
 *      GL_COLORMODE_T  e_clrmode;
 *      GL_HPALETTE_T   h_palette;
 *      GL_HCLIPPER_T   h_clipper;
 *  } GL_SURFACE_DESC_T;
 *  @endcode
 *  @li@c  ui4_width                                         - width of a surface
 *  @li@c  ui4_height                                        - height of a surface
 *  @li@c  e_clrmode                                         - color mode of a surface
 *  @li@c  h_palette                                         - palette handle of a surface
 *  @li@c  h_clipper                                         - clipper handle of a surface
 */
/*------------------------------------------------------------------*/
typedef struct _GL_SURFACE_DESC_T
{
    UINT32          ui4_width;
    UINT32          ui4_height;
    GL_COLORMODE_T  e_clrmode;
    GL_HPALETTE_T   h_palette;
    GL_HCLIPPER_T   h_clipper;
} GL_SURFACE_DESC_T;

/*------------------------------------------------------------------*/
/*! @struct GL_SURF_INFO_T
 *  @brief  used for querying the information of a surface
 *  @code
 *  typedef struct _GL_SURF_INFO_T
 *  {
 *      GL_COLORMODE_T  e_clrmode;
 *      UINT32          ui4_width;
 *      UINT32          ui4_height;
 *      HANDLE_T        h_signal;
 *  } GL_SURF_INFO_T;
 *  @endcode
 *  @li@c  e_clrmode                                         - color mode of this surface
 *  @li@c  ui4_width                                         - width of this surface
 *  @li@c  ui4_height                                        - height of this surface
 *  @li@c  h_signal                                          - signal handle of this surface
 */
/*------------------------------------------------------------------*/
typedef struct _GL_SURF_INFO_T
{
    GL_COLORMODE_T  e_clrmode;
    UINT32          ui4_width;
    UINT32          ui4_height;
    HANDLE_T        h_signal;
} GL_SURF_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct GL_LOCK_INFO_T
 *  @brief  information got while locking a surface
 *  @code
 *  typedef struct _GL_LOCK_INFO_T
 *  {
 *      GL_COLORMODE_T  e_clrmode;
 *      SIZE_T          z_subplane;
 *      struct _SUB_PLANE_LOCK_INFO_T {
 *          void*           pv_bits;
 *          UINT32          ui4_width;
 *          UINT32          ui4_height;
 *          UINT32          ui4_pitch;
 *      } at_subplane[GL_MAX_SUBPLANE];
 *  } GL_LOCK_INFO_T;
 *  @endcode
 *  @li@c  e_clrmode                                         - color mode of this surface
 *  @li@c  z_subplane                                        - subplane (buffer) number of this surface
 *  @li@c  pv_bits                                           - address of a subplane
 *  @li@c  ui4_width                                         - width of a subplane in pixels
 *  @li@c  ui4_height                                        - height of a subplane in pixels
 *  @li@c  ui4_pitch                                         - pitch of a subplane in bytes
 */
/*------------------------------------------------------------------*/
typedef struct _GL_LOCK_INFO_T
{
    GL_COLORMODE_T  e_clrmode;
    SIZE_T          z_subplane;

    struct _SUB_PLANE_LOCK_INFO_T {
        void*           pv_bits;
        UINT32          ui4_width;
        UINT32          ui4_height;
        UINT32          ui4_pitch;
    } at_subplane[GL_MAX_SUBPLANE];
} GL_LOCK_INFO_T;

/*------------------------------------------------------------------*/
/*! @enum   GL_CORNER_T
 *  @brief  describe the position of a corner
 *  @code
 *  typedef enum _GL_CORNER_T
 *  {
 *      CORNER_RIGHT_BOTTOM,
 *      CORNER_LEFT_BOTTOM,
 *      CORNER_LEFT_TOP,
 *      CORNER_RIGHT_TOP,
 *      CORNER_MAX
 *  } GL_CORNER_T;
 *  @endcode
 *  @li@c  CORNER_RIGHT_BOTTOM                               - the right-bottom corner
 *  @li@c  CORNER_LEFT_BOTTOM                                - the left-bottom corner
 *  @li@c  CORNER_LEFT_TOP                                   - the left-top corner
 *  @li@c  CORNER_RIGHT_TOP                                  - the right-top corner
 *  @li@c  CORNER_MAX                                        - used for enumeration counting
 */
/*------------------------------------------------------------------*/
typedef enum _GL_CORNER_T
{
    CORNER_RIGHT_BOTTOM,
    CORNER_LEFT_BOTTOM,
    CORNER_LEFT_TOP,
    CORNER_RIGHT_TOP,
    CORNER_MAX
} GL_CORNER_T;

#define GL_ARC_IDX(rb, lb, lt, rt)  \
    ((((UINT8) rb & 3) << (CORNER_RIGHT_BOTTOM << 1)) | \
     (((UINT8) lb & 3) << (CORNER_LEFT_BOTTOM << 1)) | \
     (((UINT8) lt & 3) << (CORNER_LEFT_TOP << 1)) | \
     (((UINT8) rt & 3) << (CORNER_RIGHT_TOP << 1)))    /**< macro for recording the four corner index */
     
     
/*------------------------------------------------------------------*/
/*! @struct GL_ARC_DESC_T
 *  @brief  contains the information of an arc
 *  @code
 *  typedef struct _GL_ARC_DESC_T
 *  {
 *      GL_SIZE_T   at_arc[CORNER_MAX];
 *      UINT8       ui1_arc_idx;
 *  } GL_ARC_DESC_T;
 *  @endcode
 *  @li@c  at_arc[CORNER_MAX]                                - contains the size information of four corners
 *  @li@c  ui1_arc_idx                                       - contains the corner index used to get the size information
 */
/*------------------------------------------------------------------*/
typedef struct _GL_ARC_DESC_T
{
    GL_SIZE_T   at_arc[CORNER_MAX];
    UINT8       ui1_arc_idx;
} GL_ARC_DESC_T;

/*------------------------------------------------------------------*/
/*! @struct GL_GPU_CAPS_T
 *  @brief  information describing the capability of the GPU
 *  @code
 *  typedef struct _GL_GPU_CAPS_T
 *  {
 *      UINT32      ui4_type;
 *      union {
 *          struct {
 *              UINT32  ui4_max_width;
 *              UINT32  ui4_min_width;
 *              UINT32  ui4_max_height;
 *              UINT32  ui4_min_height;
 *          } t_scaling_limit;
 *      } u;
 *  } GL_GPU_CAPS_T;
 *  @endcode
 *  @li@c  ui4_type                                          - flag to indicate which capabilities are queried
 *  @li@c  ui4_max_width                                     - max width supported for scaling
 *  @li@c  ui4_min_width                                     - min width supported for scaling
 *  @li@c  ui4_max_height                                    - max height supported for scaling
 *  @li@c  ui4_min_height                                    - min height supported for scaling
 */
/*------------------------------------------------------------------*/
typedef struct _GL_GPU_CAPS_T
{
    UINT32      ui4_type;

    union {
        /* used for GL_GPU_SCALING_LIMIT */
        struct {
            UINT32  ui4_max_width;
            UINT32  ui4_min_width;
            UINT32  ui4_max_height;
            UINT32  ui4_min_height;
        } t_scaling_limit;
    } u;
} GL_GPU_CAPS_T;

typedef union _GL_BLT_EX_T
{
    UINT8           ui1_alpha;
    GL_COLOR_T      t_color;
    GL_HSURFACE_T   h_alphamap;
} GL_BLT_EX_T;

/*------------------------------------------------------------------*/
/*! @enum   DRCS_PROFILE
 *  @brief  describe the teletext version for rendering DRCS
 *  @code
 *  typedef enum
 *  {
 *      DRCS_TTX_25,
 *      DRCS_TTX_35
 *  } DRCS_PROFILE;
 *  @endcode
 *  @li@c  DRCS_TTX_25                                       - teletext 2.5
 *  @li@c  DRCS_TTX_35                                       - teletext 3.5
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRCS_TTX_25,
    DRCS_TTX_35
} DRCS_PROFILE;

/*------------------------------------------------------------------*/
/*! @enum   GL_DRV_UPDATE_TYPE_T
 *  @brief  describe the information type for sending to driver while updating
 *  @code
 *  typedef enum
 *  {
 *      GL_DRV_UPDATE_NORMAL,
 *      GL_DRV_UPDATE_3D
 *  } GL_DRV_UPDATE_TYPE_T;
 *  @endcode
 *  @li@c  GL_DRV_UPDATE_NORMAL                                 - normal update
 *  @li@c  GL_DRV_UPDATE_3D                                     - 3D update
 */
/*------------------------------------------------------------------*/
typedef enum
{
    GL_DRV_UPDATE_NORMAL,
    GL_DRV_UPDATE_3D_BLIT,
    GL_DRV_UPDATE_3D_FLIP
} GL_DRV_UPDATE_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct GL_DRV_UPDATE_INFO_T
 *  @brief  describe the information for sending to driver while updating
 *  @code
 *  typedef struct
 *  {
 *      GL_DRV_UPDATE_TYPE_T    e_mode;
 *      GL_HSURFACE_T           h_surf;
 *  } GL_DRV_UPDATE_INFO_T;
 *  @endcode
 *  @li@c  e_mode                                           - update mode
 *  @li@c  h_surf                                           - surface to be passed
 */
/*------------------------------------------------------------------*/
typedef struct
{
    GL_DRV_UPDATE_TYPE_T    e_mode;
    GL_HSURFACE_T           h_src_surf;
    GL_HSURFACE_T           h_dst_surf;
    INT32                   i4_x;
    INT32                   i4_y;
    UINT32                  ui4_width;
    UINT32                  ui4_height;
    UINT32                  ui4_surf_width;
    UINT32                  ui4_surf_height;
} GL_DRV_UPDATE_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct GL_DRCS_INFO_T
 *  @brief  describe information related to a DRCS string
 *  @code
 *  typedef struct
 *  {
 *      DRCS_PROFILE    e_profile;
 *  } GL_DRCS_INFO_T;
 *  @endcode
 *  @li@c  e_profile                                         - teletext version
 */
/*------------------------------------------------------------------*/
typedef struct
{
    DRCS_PROFILE    e_profile;
} GL_DRCS_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct GL_TTX_COLOR_T
 *  @brief  describe the foreground and background colors for rendering teletext a string
 *  @code
 *  typedef struct
 *  {
 *      GL_COLOR_T      t_fg_clr;
 *      GL_COLOR_T      t_bg_clr;
 *  } GL_TTX_COLOR_T;
 *  @endcode
 *  @li@c  t_fg_clr                                          - foreground color
 *  @li@c  t_bg_clr                                          - background color
 */
/*------------------------------------------------------------------*/
typedef struct
{
    GL_COLOR_T      t_fg_clr;
    GL_COLOR_T      t_bg_clr;
} GL_TTX_COLOR_T;

/*------------------------------------------------------------------*/
/*! @struct GL_ISDB_CHAR_COLOR_T
 *  @brief  describe the colors for rendering isdb a string
 *  @code
 *  typedef struct
 *  {
 *      GL_COLOR_T      t_fg_clr;
 *      GL_COLOR_T      t_bg_clr;
 *      GL_COLOR_T      t_half_fg_color;
 *      GL_COLOR_T      t_half_bg_color;
 *  } GL_ISDB_CHAR_COLOR_T;
 *  @endcode
 *  @li@c  t_fg_clr                                          - foreground color
 *  @li@c  t_bg_clr                                          - background color
 *  @li@c  t_half_fg_color                                   - half foreground color
 *  @li@c  t_half_bg_color                                   - half background color
 */
/*------------------------------------------------------------------*/
typedef struct _GL_ISDB_CHAR_COLOR_T
{
    GL_COLOR_T      t_fg_color;     /* foreground color */
    GL_COLOR_T      t_bg_color;     /* background color */
    GL_COLOR_T      t_half_fg_color;/* half foreground color */
    GL_COLOR_T      t_half_bg_color;/* half background color */
}   GL_ISDB_CHAR_COLOR_T;

/*------------------------------------------------------------------*/
/*! @struct GL_ISDB_GRID_T
 *  @brief  describe the position and size of a isdb character block
 *  @code
 *  typedef struct
 *  {
 *      UINT16          ui2_grid_h;
 *      UINT16          ui2_grid_v;
 *      UINT16          ui2_char_h;
 *      UINT16          ui2_char_v;
 *      UINT16          ui2_l_space_h;
 *      UINT16          ui2_t_space_v;
 *  } GL_ISDB_GRID_T;
 *  @endcode
 *  @li@c  ui2_grid_h                                        - character block width
 *  @li@c  ui2_grid_v                                        - character block height
 *  @li@c  ui2_char_h                                        - character width
 *  @li@c  ui2_char_v                                        - character height
 *  @li@c  ui2_l_space_h                                     - the horizontal space in the left of character
 *  @li@c  ui2_t_space_v                                     - the vertical space in the top of character
 */
/*------------------------------------------------------------------*/
typedef struct _GL_ISDB_GRID_T
{
    UINT16          ui2_grid_h;
    UINT16          ui2_grid_v;
    UINT16          ui2_char_h;
    UINT16          ui2_char_v;
    UINT16          ui2_l_space_h;
    UINT16          ui2_t_space_v;
}   GL_ISDB_GRID_T;


#endif

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_GL*/
/*----------------------------------------------------------------------------*/
