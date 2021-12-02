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
 * $RCSfile: x_gfx.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains type definitions common to the whole
 *         Middleware.
 *---------------------------------------------------------------------------*/

#ifndef _X_GFX_H_
#define _X_GFX_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "u_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* get types of OSD plane driver component */
#define OSD_GET_TYPE_PLANE_CAPS             \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  1))

#define OSD_GET_TYPE_PLANE_BLEND_LEVEL      \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  2))

#define OSD_GET_TYPE_RGNLIST_CREATE         \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  3))

#define OSD_GET_TYPE_REGION_CREATE          \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  4))

#define OSD_GET_TYPE_REGION_INFO            \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  5))

#define OSD_GET_TYPE_BITMAP_CREATE          \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  6))

#define OSD_GET_TYPE_BITMAP_LOCK            \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  7))

#define OSD_GET_TYPE_BITMAP_PALETTE         \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  8))

#define OSD_GET_TYPE_PALETTE_CREATE         \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  9))

#define OSD_GET_TYPE_PALETTE_ENTRIES        \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 10))

#define OSD_GET_TYPE_PALETTE_PARAMS         \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 11))

#define OSD_GET_TYPE_PLANE_ADV_SCALER       \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 12))

#define OSD_GET_TYPE_PLANE_ADV_SCALER_FILTER_CNT    \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 13))

#define OSD_GET_TYPE_PLANE_NO_ALPHA_AREA    \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 14))

#define OSD_GET_TYPE_PLANE_FADING_RATIO     \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 15))

#define OSD_GET_TYPE_PLANE_POS              \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 16))

#define OSD_GET_TYPE_BITMAP_CREATE_FROM_FBM \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 17))

#define OSD_GET_TYPE_FBM_BITMAP_INFO \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 18))


/* set types of OSD plane driver component */
#define OSD_SET_TYPE_PLANE_ENABLE           \
   ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  1)) | RM_SET_TYPE_ARG_NO_REF)

#define OSD_SET_TYPE_PLANE_FLIP             \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  2))

#define OSD_SET_TYPE_PLANE_BLEND_LEVEL      \
   ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  3)) | RM_SET_TYPE_ARG_NO_REF)

#define OSD_SET_TYPE_RGNLIST_DELETE         \
   ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  4)) | RM_SET_TYPE_ARG_NO_REF)

#define OSD_SET_TYPE_RGNLIST_DETACH_ALL     \
   ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  5)) | RM_SET_TYPE_ARG_NO_REF)

#define OSD_SET_TYPE_REGION_DELETE          \
   ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  6)) | RM_SET_TYPE_ARG_NO_REF)

#define OSD_SET_TYPE_REGION_ATTACH          \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  7))

#define OSD_SET_TYPE_REGION_DETACH          \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  8))

#define OSD_SET_TYPE_REGION_ATTR            \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  9))

#define OSD_SET_TYPE_BITMAP_DELETE          \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 10))

#define OSD_SET_TYPE_BITMAP_UNLOCK          \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 11))

#define OSD_SET_TYPE_BITMAP_PALETTE         \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 12))

#define OSD_SET_TYPE_PALETTE_DELETE         \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 13))

#define OSD_SET_TYPE_PALETTE_ENTRIES        \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 14))

#define OSD_SET_TYPE_PALETTE_PARAMS         \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 15))

#define OSD_SET_TYPE_PLANE_ADV_SCALER       \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 16))

#define OSD_SET_TYPE_PLANE_NO_ALPHA_AREA    \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 17))

#define OSD_SET_TYPE_PLANE_FADING_RATIO     \
   ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 18)) | RM_SET_TYPE_ARG_NO_REF)

#define OSD_SET_TYPE_PLANE_POS              \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 19))

#define OSD_SET_TYPE_BITMAP_DELETE_FROM_FBM \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 20))

#define OSD_SET_TYPE_BITMAP_UPDATE          \
   ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 21)) | RM_SET_TYPE_ARG_NO_REF)

#define OSD_SET_TYPE_BITMAP_RECONFIG        \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 22))

#define OSD_SET_TYPE_3D_OSD                 \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 23))

/* get types of graphics driver component */
#define GFX_GET_TYPE_GPU_CAPS               \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  1))

#define GFX_GET_TYPE_GPU_CAPS_EX            \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  2))

/* set types of graphics driver component */
#define GFX_SET_TYPE_FILL_RECT              \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  1))

#define GFX_SET_TYPE_DRAW_LINE_H            \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  2))

#define GFX_SET_TYPE_DRAW_LINE_V            \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  3))

#define GFX_SET_TYPE_EXT_ALPHA_MUL          \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  4))

#define GFX_SET_TYPE_SELF_ALPHA_MUL         \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  5))

#define GFX_SET_TYPE_SELF_ALPHA_DIV         \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  6))

#define GFX_SET_TYPE_ALPHA_BITBLT           \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  7))

#define GFX_SET_TYPE_COLOR_BITBLT           \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  8))

#define GFX_SET_TYPE_BITBLT                 \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  9))

#define GFX_SET_TYPE_STRETCH_BITBLT         \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 10))

#define GFX_SET_TYPE_TRANSPARENT_BITBLT     \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 11))

#define GFX_SET_TYPE_TRANSPARENT_FILL       \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 12))

#define GFX_SET_TYPE_ALPHA_BLENDING         \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 13))

#define GFX_SET_TYPE_ALPHA_COMPOSITION      \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 14))

#define GFX_SET_TYPE_FLUSH_OPQUEUE          \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 15))

#define GFX_SET_TYPE_NOTIFY_FCT             \
   ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 16)) | RM_SET_TYPE_ARG_CALLBACK)

#define GFX_SET_TYPE_ALPHAMAP_BITBLT        \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 17))

#define GFX_SET_TYPE_ADV_STRETCH_BITBLT     \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 18))

#define GFX_SET_TYPE_YCBCR_TO_RGB           \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 19))

#define GFX_SET_TYPE_ROTATE_90              \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 20))

#define GFX_SET_TYPE_PALETTE_BITBLT         \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 21))

#define GFX_SET_TYPE_ROP_BITBLT             \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 22))

#define GFX_SET_TYPE_JPG_Y_SCALE            \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 23))

#define GFX_SET_TYPE_JPG_CB_SCALE           \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 24))

#define GFX_SET_TYPE_JPG_CR_SCALE           \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 25))

#define GFX_SET_TYPE_REPLACE_COLOR          \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 26))

#define GFX_SET_TYPE_COMPRESSED_BITBLT      \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 27))
    
#define GFX_SET_TYPE_OSD_WAIT_VDP      \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 28))

#define GFX_SET_TYPE_OPBITBLT      \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 29)) 

#define GFX_SET_TYPE_VGFX_SCALER      \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 30)) 

/* enumerations */
typedef enum _GFX_COLORMODE_T
{
    GFX_COLORMODE_AYCbCr_CLUT2  = 0,
    GFX_COLORMODE_AYCbCr_CLUT4  = 1,
    GFX_COLORMODE_AYCbCr_CLUT8  = 2,
    GFX_COLORMODE_CbYCrY_16     = 3,
    GFX_COLORMODE_YCbYCr_16     = 4,
    GFX_COLORMODE_AYCbCr_D8888  = 5,
    GFX_COLORMODE_ARGB_CLUT2    = 6,
    GFX_COLORMODE_ARGB_CLUT4    = 7,
    GFX_COLORMODE_ARGB_CLUT8    = 8,
    GFX_COLORMODE_RGB_D565      = 9,
    GFX_COLORMODE_ARGB_D1555    = 10,
    GFX_COLORMODE_ARGB_D4444    = 11,
    GFX_COLORMODE_ARGB_D8888    = 12,
    GFX_COLORMDOE_YUV_420_BLK   = 13,
    GFX_COLORMODE_YUV_420_RS    = 14,
    GFX_COLORMDOE_YUV_422_BLK   = 15,
    GFX_COLORMODE_YUV_422_RS    = 16,
    GFX_COLORMDOE_YUV_444_BLK   = 17,
    GFX_COLORMODE_YUV_444_RS    = 18
} GFX_COLORMODE_T;

typedef enum _GFX_YCBCR_FORMAT_T
{
    GFX_YCBCR_420_MB  = 0,
    GFX_YCBCR_420_LNR = 1,
    GFX_YCBCR_422_LNR = 2       /* MT538x only */
} GFX_YCBCR_FORMAT_T;

/* constants */
#define GFX_CAP_AYCbCr_CLUT2    ((UINT32) 1 << (UINT32)GFX_COLORMODE_AYCbCr_CLUT2)
#define GFX_CAP_AYCbCr_CLUT4    ((UINT32) 1 << (UINT32)GFX_COLORMODE_AYCbCr_CLUT4)
#define GFX_CAP_AYCbCr_CLUT8    ((UINT32) 1 << (UINT32)GFX_COLORMODE_AYCbCr_CLUT8)
#define GFX_CAP_CbYCrY_16       ((UINT32) 1 << (UINT32)GFX_COLORMODE_CbYCrY_16   )
#define GFX_CAP_YCbYCr_16       ((UINT32) 1 << (UINT32)GFX_COLORMODE_YCbYCr_16   )
#define GFX_CAP_ARGB_CLUT2      ((UINT32) 1 << (UINT32)GFX_COLORMODE_ARGB_CLUT2  )
#define GFX_CAP_ARGB_CLUT4      ((UINT32) 1 << (UINT32)GFX_COLORMODE_ARGB_CLUT4  )
#define GFX_CAP_ARGB_CLUT8      ((UINT32) 1 << (UINT32)GFX_COLORMODE_ARGB_CLUT8  )
#define GFX_CAP_RGB_D565        ((UINT32) 1 << (UINT32)GFX_COLORMODE_RGB_D565    )
#define GFX_CAP_ARGB_D1555      ((UINT32) 1 << (UINT32)GFX_COLORMODE_ARGB_D1555  )
#define GFX_CAP_ARGB_D4444      ((UINT32) 1 << (UINT32)GFX_COLORMODE_ARGB_D4444  )
#define GFX_CAP_ARGB_D8888      ((UINT32) 1 << (UINT32)GFX_COLORMODE_ARGB_D8888  )
#define GFX_CAP_AYCbCr_D8888    ((UINT32) 1 << (UINT32)GFX_COLORMODE_AYCbCr_D8888)

/* alpha blending levels */
#define GFX_BLEND_TRANSPARENT   ((UINT8)   0)
#define GFX_BLEND_OPAQUE        ((UINT8) 255)

#define GFX_MAX_SUBPLANE        ((UINT32)  4)

#define GFX_CLUT_ALPHA_0        ((UINT8) 0)
#define GFX_CLUT_ALPHA_1_0      ((UINT8) 1)
#define GFX_CLUT_ALPHA_2_0      ((UINT8) 2)
#define GFX_CLUT_ALPHA_3_0      ((UINT8) 3)
#define GFX_CLUT_ALPHA_4_1      ((UINT8) 4)
#define GFX_CLUT_ALPHA_5_2      ((UINT8) 5)
#define GFX_CLUT_ALPHA_6_3      ((UINT8) 6)
#define GFX_CLUT_ALPHA_7_4      ((UINT8) 7)

/* palette entry color order */
#define GFX_CLUT_HIBYTE         ((UINT8) 3)
#define GFX_CLUT_MHBYTE         ((UINT8) 2)
#define GFX_CLUT_MLBYTE         ((UINT8) 1)
#define GFX_CLUT_LOBYTE         ((UINT8) 0)

#if 0
#define SELECT_A                ((UINT8) 3)
#define SELECT_R                ((UINT8) 2)
#define SELECT_G                ((UINT8) 1)
#define SELECT_B                ((UINT8) 0)

#define GFX_COLOR_PALETTE(a, r, g, b)       ((((UINT32)(a) & 0xff) << (SELECT_A * 8)) | (((UINT32)(r) & 0xff) << (SELECT_R * 8)) | (((UINT32)(g) & 0xff) << (SELECT_G * 8)) | (((UINT32)(b) & 0xff) << (SELECT_B * 8)))
#endif

#define GFX_COLOR_RGB565(r, g, b)           (((((UINT16) (r)) & 0xf8) << 8) | ((((UINT16) (g)) & 0xfc) << 3) | ((((UINT16) (b)) & 0xf8) >> 3))
#if 0
#define GFX_COLOR_ARGB1555(a, r, g, b)      (((((UINT32) (a)) & 0x80) << 8) | ((((UINT32) (r)) & 0xf8) << 7) | ((((UINT32) (g)) & 0xf8) << 2) | ((((UINT32) (b)) & 0xf8) >> 3))
#else
#define GFX_COLOR_ARGB1555(a, r, g, b)      (((((UINT16) (a)) > 0) ? (UINT16) 0x8000 : 0) | ((((UINT16) (r)) & 0xf8) << 7) | ((((UINT16) (g)) & 0xf8) << 2) | ((((UINT16) (b)) & 0xf8) >> 3))
#endif
#define GFX_COLOR_ARGB4444(a, r, g, b)      (((((UINT16) (a)) & 0xf0) << 8) | ((((UINT16) (r)) & 0xf0) << 4) | (((UINT16) (g)) & 0xf0) | ((((UINT16) (b)) & 0xf0) >> 4))
#define GFX_COLOR_ARGB8888(a, r, g, b)      (((((UINT32) (a)) & 0xff) << 24) | ((((UINT32) (r)) & 0xff) << 16) | ((((UINT32) (g)) & 0xff) << 8) | (((UINT32) (b)) & 0xff))

#define GFX_FIX_FRACTION_BITS       12
#define GFX_INTTOFIX(i)             (GFX_FP_T) ((UINT32) (i) << GFX_FIX_FRACTION_BITS)
#define GFX_FIXTOINT(f)             (UINT32) ((f) >> GFX_FIX_FRACTION_BITS)

/* region attr flags */
#define GFX_RGN_ATTR_NULL               ((UINT32)      0)
#define GFX_RGN_ATTR_SHRINK_X           ((UINT32) 0x0001)
#define GFX_RGN_ATTR_SHRINK_Y           ((UINT32) 0x0002)
#define GFX_RGN_ATTR_BLEND_OPTION       ((UINT32) 0x0004)
#define GFX_RGN_ATTR_BLEND_LEVEL        ((UINT32) 0x0008)
#define GFX_RGN_ATTR_COLORKEY_ENABLE    ((UINT32) 0x0010)
#define GFX_RGN_ATTR_COLORKEY           ((UINT32) 0x0020)
#define GFX_RGN_ATTR_WIDTH              ((UINT32) 0x0040)
#define GFX_RGN_ATTR_HEIGHT             ((UINT32) 0x0080)
#define GFX_RGN_ATTR_SURF_OFFSET_X      ((UINT32) 0x0100)
#define GFX_RGN_ATTR_SURF_OFFSET_Y      ((UINT32) 0x0200)
#define GFX_RGN_ATTR_COMPRESSION        ((UINT32) 0x0400)
#define GFX_RGN_ATTR_ALL                ((UINT32) 0xffffffff)

/* alpha blending option */
#define GFX_BLENDING_OPT_NONE       ((UINT8) 0)
#define GFX_BLENDING_OPT_PIXEL      ((UINT8) 1)
#define GFX_BLENDING_OPT_REGION     ((UINT8) 2)
#define GFX_BLENDING_OPT_PLANE      ((UINT8) 3)

/* 2D graphics operation */
#define GFX_OP_FILLRECT             ((UINT32)0x000001)
#define GFX_OP_DRAWHLINE            ((UINT32)0x000002)
#define GFX_OP_DRAWVLINE            ((UINT32)0x000004)
#define GFX_OP_BITBLT               ((UINT32)0x000008)
#define GFX_OP_TRANSPARENT_BITBLT   ((UINT32)0x000010)
#define GFX_OP_TRANSPARENT_FILL     ((UINT32)0x000020)
#define GFX_OP_ALPHA_BLENDING       ((UINT32)0x000040)
#define GFX_OP_ALPHA_COMPOSITION    ((UINT32)0x000080)
#define GFX_OP_ALPHAMAP_BITBLT      ((UINT32)0x000100)
#define GFX_OP_EXT_ALPHA_MUL        ((UINT32)0x000200)
#define GFX_OP_SELF_ALPHA_MUL       ((UINT32)0x000400)
#define GFX_OP_SELF_ALPHA_DIV       ((UINT32)0x000800)
#define GFX_OP_ALPHA_BITBLT         ((UINT32)0x001000)
#define GFX_OP_COLOR_BITBLT         ((UINT32)0x002000)
#define GFX_OP_STRETCH_BITBLT       ((UINT32)0x004000)
#define GFX_OP_ADV_STRETCH_BITBLT   ((UINT32)0x008000)
#define GFX_OP_YCBCR_TO_RGB         ((UINT32)0x010000)
#define GFX_OP_ROTATE_90            ((UINT32)0x020000)
#define GFX_OP_PALETTE_BITBLT       ((UINT32)0x040000)
#define GFX_OP_ROP_BITBLT           ((UINT32)0x080000)
#define GFX_OP_JPG_SCALE            ((UINT32)0x100000)

/* used for GFX_GET_TYPE_GPU_CAPS_EX */
#define GFX_CAP_SCALING_LIMIT       ((UINT32) 0)

/* scaler function enabling flags */
#define GFX_SCALER_FUNC_SCALING         ((UINT16) 0x01)
#define GFX_SCALER_FUNC_FILTERING       ((UINT16) 0x02)

/* scaler setting flags */
#define GFX_SCALER_FUNC_ENABLE          ((UINT16) 0x01)
#define GFX_SCALER_SRC_SIZE             ((UINT16) 0x02)
#define GFX_SCALER_DST_SIZE             ((UINT16) 0x04)
#define GFX_SCALER_BUF_ARRANGEMENT      ((UINT16) 0x08)
#define GFX_SCALER_COLOR_MODE           ((UINT16) 0x10)
#define GFX_SCALER_FILTER_AS_DEFAULT    ((UINT16) 0x20)
#define GFX_SCALER_FILTER_AS_SPECIFIED  ((UINT16) 0x40)
#define GFX_SCALER_SPECIFIED_REGION     ((UINT16) 0x80)

/* scaler line buffer arrangement */
#define GFX_SCALER_BUF_SHARED           ((UINT8) 0)
#define GFX_SCALER_BUF_EXCLUSIVE        ((UINT8) 1)

/* scaler color mode setting */
#define GFX_SCALER_CM_16BPP             ((UINT8) 0)
#define GFX_SCALER_CM_32BPP             ((UINT8) 1)

/* extra capabilities */
#define GFX_CAPS_ADV_SCALER             ((UINT32) 0x01)
#define GFX_CAPS_NO_ALPHA_AREA          ((UINT32) 0x02)
#define GFX_CAPS_FADING_RATIO           ((UINT32) 0x04)
#define GFX_CAPS_PLANE_REPOS            ((UINT32) 0x08)
#define GFX_CAPS_SCART_OUT              ((UINT32) 0x10)

/* callback condition */
typedef UINT32 GFX_COND_T;

#define GFX_COND_IDLE_IN_ISR        ((GFX_COND_T) 0)
#define GFX_COND_IDLE_IN_THREAD     ((GFX_COND_T) 1)

/* callback status */
#define GFX_EXEC_OK                 ((UINT32) 0)
#define GFX_EXEC_FAIL               ((UINT32) 1)

/* Z order */
#define GFX_ZORDER_TOPMOST          ((UINT32) 0)

/* enumerations */
typedef enum
{
    GFX_CLEAR = 0,
    GFX_DST_IN,
    GFX_DST_OUT,
    GFX_DST_OVER,
    GFX_SRC,
    GFX_SRC_IN,
    GFX_SRC_OUT,
    GFX_SRC_OVER
} GFX_PD_RULE_T;

typedef enum
{
    GFX_CLOCKWISE   = 0,
    GFX_C_CLOCKWISE = 1
} GFX_ROTATE_FLAG_T;

typedef enum
{
    GFX_LOSSLESS        = 0,
    GFX_LOSSY_LOW       = 1,
    GFX_LOSSY_MEDIAN    = 2,
    GFX_LOSSY_HIGH      = 3
} GFX_COMPRESSION_RATIO_T;

/* typedefs */
typedef struct
{
    UINT32 ui4_unused;
} *GFX_HRGNLIST_T;

typedef struct
{
    UINT32 ui4_unused;
} *GFX_HREGION_T;

typedef struct
{
    UINT32 ui4_unused;
} *GFX_HBITMAP_T;

typedef struct
{
    UINT32 ui4_unused;
} *GFX_HPALETTE_T;

typedef enum
{
    GFX_PALETTE_MSB = 0,
    GFX_PALETTE_LSB = 1
} GFX_BYTE_ALIGNED_T;

typedef enum
{
    GFX_ROP_NOT_SRC = 0,
    GFX_ROP_NOT_DST,
    GFX_ROP_SRC_XOR_DST,
    GFX_ROP_SRC_XNOR_DST,
    GFX_ROP_SRC_AND_DST,
    GFX_ROP_NOT_SRC_AND_DST,
    GFX_ROP_SRC_AND_NOT_DST,
    GFX_ROP_NOT_SRC_AND_NOT_DST,
    GFX_ROP_SRC_OR_DST,
    GFX_ROP_NOT_SRC_OR_DST,
    GFX_ROP_SRC_OR_NOT_DST,
    GFX_ROP_NOT_SRC_OR_NOT_DST
} GFX_ROP_TYPE_T;

typedef enum
{
    GFX_STRETCH_UNKNOWN = 0,
    GFX_STRETCH_PHOTO   = 1,
    GFX_STRETCH_UI      = 2
} GFX_STRETCH_OPTION;

typedef enum _GFX_3D_OSD_MODE_T
{
    GFX_3D_OSD_UNKNOWN = 0,
    GFX_3D_OSD_BLIT    = 1,
    GFX_3D_OSD_FLIP    = 2
} GFX_3D_OSD_MODE_T;

typedef struct _FBM_SURFACE_INFO_T
{
    UINT8       ui1_cnt;
    UINT32      ui4_mem_size;
} FBM_SURFACE_INFO_T;

typedef UINT16 GFX_FP_T;
typedef UINT32 GFX_COLOR_PACKED_T;

typedef INT32 (*GFX_NFY_FCT_T) (GFX_COND_T e_cond, const VOID* pv_tag, UINT32 ui4_data);

/* structures */
#define GFX_PALETTE(x)          struct                                          \
                                {                                               \
                                    UINT16              ui2_size;               \
                                    UINT8               ui1_alpha_bit_select;   \
                                    UINT8               ui1_alpha_select;       \
                                    UINT8               ui1_YR_select;          \
                                    UINT8               ui1_UG_select;          \
                                    UINT8               ui1_VB_select;          \
                                    GFX_COLOR_PACKED_T  at_clut_entry[x];       \
                                }

typedef GFX_PALETTE(1)          GFX_PALETTE1_T;
typedef GFX_PALETTE(2)          GFX_PALETTE2_T;
typedef GFX_PALETTE(4)          GFX_PALETTE4_T;
typedef GFX_PALETTE(16)         GFX_PALETTE16_T;
typedef GFX_PALETTE(256)        GFX_PALETTE256_T;
typedef GFX_PALETTE1_T*         GFX_PALETTE_PTR;

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/

typedef struct _GFX_PALETTE_HEADER_T
{
    GFX_COLORMODE_T e_color_mode;
    UINT16          ui2_size;
    UINT8           ui1_alpha_bit_select;
    UINT8           ui1_alpha_select;
    UINT8           ui1_YR_select;
    UINT8           ui1_UG_select;
    UINT8           ui1_VB_select;
} GFX_PALETTE_HEADER_T;

typedef struct _GFX_BITMAP_INIT_T
{
    GFX_COLORMODE_T     e_colormode;
    UINT32              ui4_bmp_w;
    UINT32              ui4_bmp_h;
    GFX_HPALETTE_T      h_palette;
    GFX_HBITMAP_T       h_new_bitmap;
    SIZE_T              z_new_bitmap_size;
} GFX_BITMAP_INIT_T;

typedef struct _GFX_PALETTE_INIT_T
{
    GFX_PALETTE_PTR     pt_palette;
    GFX_HPALETTE_T      h_new_palette;
    UINT32              ui4_id;
} GFX_PALETTE_INIT_T;

typedef struct _GFX_REGION_INIT_T
{
    UINT32              ui4_out_w;
    UINT32              ui4_out_h;

    GFX_HBITMAP_T       h_bitmap;

    UINT8               ui1_blend_option;
    UINT8               ui1_blend_level;

    BOOL                b_enable_colorkey;
    GFX_COLOR_PACKED_T  ui4_colorkey;

    GFX_FP_T            t_shrink_x;
    GFX_FP_T            t_shrink_y;

    GFX_HREGION_T       h_new_region;

    BOOL                b_enable_decompression;
    BOOL                b_line_separate;
    UINT8               ui1_compression_level;
    UINT8               ui1_extra_line_size;
} GFX_REGION_INIT_T;

typedef struct _GFX_REGION_INFO_T
{
    GFX_HREGION_T   h_region;

    INT32           i4_out_x;
    INT32           i4_out_y;
    UINT32          ui4_out_w;
    UINT32          ui4_out_h;

    GFX_FP_T        t_shrink_x;
    GFX_FP_T        t_shrink_y;

    UINT8           ui1_blend_option;
    UINT8           ui1_blend_level;

    BOOL            b_colorkey;
    UINT32          ui4_colorkey;

    GFX_HBITMAP_T   h_bitmap;

    GFX_COLORMODE_T e_colormode;
    UINT32          ui4_bmp_w;
    UINT32          ui4_bmp_h;
}   GFX_REGION_INFO_T;

typedef struct _GFX_BITMAP_LOCK_INFO_T
{
    GFX_HBITMAP_T       h_bitmap;

    GFX_COLORMODE_T     e_colormode;
    SIZE_T              z_subplane;

    struct _SUB_PLANE_INFO_T {
        void*       pv_bits;
        UINT32      ui4_width;
        UINT32      ui4_height;
        UINT32      ui4_pitch;
    } at_subplane[GFX_MAX_SUBPLANE];
} GFX_BITMAP_LOCK_INFO_T;

typedef struct _GFX_REGION_POS_T
{
    GFX_HREGION_T   h_region;
    GFX_HRGNLIST_T  h_rgnlist;
    INT32           i4_out_x;
    INT32           i4_out_y;
} GFX_REGION_POS_T;

typedef struct _GFX_BITMAP_PALETTE_T
{
    GFX_HBITMAP_T   h_bitmap;
    GFX_HPALETTE_T  h_palette;
} GFX_BITMAP_PALETTE_T;

typedef struct _GFX_PALETTE_PARAMS_T
{
    GFX_HPALETTE_T  h_palette;
    UINT8           ui1_alpha_bit_select;
    UINT8           ui1_alpha_select;
    UINT8           ui1_YR_select;
    UINT8           ui1_UG_select;
    UINT8           ui1_VB_select;
} GFX_PALETTE_PARAMS_T;

typedef struct _GFX_PALETTE_ENTRIES_T
{
    GFX_HPALETTE_T      h_palette;
    UINT16              ui2_start;
    UINT16              ui2_count;
    GFX_COLOR_PACKED_T* pui4_entries;
} GFX_PALETTE_ENTRIES_T;

typedef struct _GFX_PLANE_CAPS_T
{
    UINT32              ui4_cm_caps;
    UINT32              ui4_func_caps;
#if 0
    UINT32              ui4_display_mask;
    INT32               i4_left;
    INT32               i4_top;
    INT32               i4_right;
    INT32               i4_bottom;
    UINT32              ui4_max_w;
    UINT32              ui4_max_h;

    UINT32              ui4_min_w;
    UINT32              ui4_min_h;
#endif
} GFX_PLANE_CAPS_T;

typedef struct _GFX_REGION_DETACH_T
{
    GFX_HREGION_T   h_region;
    GFX_HRGNLIST_T  h_rgnlist;
} GFX_REGION_DETACH_T;

typedef struct _GFX_REGION_ATTR_T
{
    GFX_HREGION_T   h_region;

    UINT32          ui4_flag;

    UINT32          ui4_width;
    UINT32          ui4_height;

    GFX_FP_T        t_shrink_x;
    GFX_FP_T        t_shrink_y;

    UINT8           ui1_blend_option;
    UINT8           ui1_blend_level;

    BOOL            b_colorkey;
    UINT32          ui4_colorkey;

    UINT32          ui4_surf_offset_x;
    UINT32          ui4_surf_offset_y;

    BOOL            b_enable_decompression;
    UINT8           ui1_compression_level;
} GFX_REGION_ATTR_T;

typedef struct _GFX_ATTACH_LIST_T
{
    GFX_HRGNLIST_T  h_rgnlist;
    UINT16          ui2_count;
    GFX_HREGION_T   ah_region[1];
} GFX_ATTACH_LIST_T;

typedef struct _GFX_POS_T
{
    INT32       i4_x;
    INT32       i4_y;
} GFX_POS_T;

/* as GFXSC_VERT_SEG_T */
typedef struct _GFX_SEG_INFO_T
{
    UINT32      ui4_src_pos;
    UINT32      ui4_dst_pos;

    UINT32      ui4_src_size;
    UINT32      ui4_dst_size;

    UINT32      ui4_reserved1;
    UINT32      ui4_reserved2;
} GFX_SEG_INFO_T;

typedef struct _GFX_ADV_SCALER_PARAM_T
{
    UINT16          ui2_flag;
    UINT16          ui2_func_enable;
    UINT32          ui4_src_w;
    UINT32          ui4_src_h;
    UINT32          ui4_dst_w;
    UINT32          ui4_dst_h;
    UINT8           ui1_buf_arrange;
    UINT8           ui1_color_mode;
    UINT8           ui1_filter_cnt;
    INT16           ai2_filter[1];
    GFX_HRGNLIST_T  h_gfx_rgnlist;
} GFX_ADV_SCALER_PARAM_T;

typedef struct _GFX_NO_ALPHA_AREA_T
{
    BOOL        b_enable;
    INT32       i4_x;
    INT32       i4_y;
    UINT32      ui4_w;
    UINT32      ui4_h;
} GFX_NO_ALPHA_AREA_T;

typedef struct _GFX_GPU_CAPS_T
{
    UINT32          ui4_gfx_op;
    UINT32          ui4_reserved;
} GFX_GPU_CAPS_T;

/* used for GFX_GET_TYPE_GPU_CAPS_EX */
typedef struct _GFX_GPU_CAPS_EX_T
{
    UINT32          ui4_type;

    union {
        /* used for GFX_CAP_SCALING_LIMIT */
        struct {
            UINT32  ui4_max_width;
            UINT32  ui4_min_width;
            UINT32  ui4_max_height;
            UINT32  ui4_min_height;
        } t_scaling_limit;
    } u;
} GFX_GPU_CAPS_EX_T;

typedef struct _GFX_FILL_T
{
    VOID*           pv_dst;
    INT32           i4_dst_x;
    INT32           i4_dst_y;
    UINT32          ui4_dst_pitch;
    GFX_COLORMODE_T e_dst_cm;

    UINT32          ui4_width;
    UINT32          ui4_height;
    UINT32          ui4_color;
} GFX_FILL_T;

#define COMMON_BITBLT_FIELDS        \
    VOID*           pv_src;         \
    INT32           i4_src_x;       \
    INT32           i4_src_y;       \
    UINT32          ui4_src_pitch;  \
    GFX_COLORMODE_T e_src_cm;       \
                                    \
    VOID*           pv_dst;         \
    INT32           i4_dst_x;       \
    INT32           i4_dst_y;       \
    UINT32          ui4_dst_pitch;  \
    GFX_COLORMODE_T e_dst_cm;       \
                                    \
    UINT32          ui4_width;      \
    UINT32          ui4_height;

typedef struct _GFX_BITBLT_BASE_T
{
    COMMON_BITBLT_FIELDS
} GFX_BITBLT_BASE_T;

typedef struct _GFX_BITBLT_T
{
    COMMON_BITBLT_FIELDS

    UINT8       ui1_alpha;
} GFX_BITBLT_T;

typedef struct _GFX_OPBITBLT_T
{
    COMMON_BITBLT_FIELDS

    UINT8       ui1_alpha;
	UINT32      u4Switch;
} GFX_OPBITBLT_T;


typedef GFX_BITBLT_T GFX_ALPHA_MUL_T;

typedef GFX_BITBLT_BASE_T GFX_ALPHAMAP_BITBLT_T;

typedef struct _GFX_STRETCH_BITBLT_T
{
    COMMON_BITBLT_FIELDS

    UINT32      ui4_dst_width;
    UINT32      ui4_dst_height;

    /* only used for GFX_SET_TYPE_ADV_STRETCH_BITBLT */
    GFX_STRETCH_OPTION  e_option;
} GFX_STRETCH_BITBLT_T;

typedef struct _GFX_TRANSPARENT_BITBLT_T
{
    COMMON_BITBLT_FIELDS

#if 0
    UINT8       ui1_alpha;
#endif

    BOOL        b_inverse_masking;
    UINT32      ui4_color_space_min;
    UINT32      ui4_color_space_max;
} GFX_TRANSPARENT_BITBLT_T;

typedef struct _GFX_TRANSPARENT_FILL_T
{
    COMMON_BITBLT_FIELDS

#if 0
    UINT8       ui1_alpha;
#endif

    BOOL        b_inverse_masking;
    UINT32      ui4_color_space_min;
    UINT32      ui4_color_space_max;

    UINT32      ui4_fill_color;
} GFX_TRANSPARENT_FILL_T;

typedef GFX_BITBLT_T GFX_ALPHA_BLEND_T;

typedef struct _GFX_ALPHA_COMPOSITION_T
{
    COMMON_BITBLT_FIELDS

    UINT8           ui1_alpha;

    GFX_PD_RULE_T   e_rule;

    BOOL            b_rect_src_option;
    UINT32          ui4_color;
} GFX_ALPHA_COMPOSITION_T;

typedef struct _GFX_ROTATE_T
{
    COMMON_BITBLT_FIELDS

    GFX_ROTATE_FLAG_T e_direction;
} GFX_ROTATE_T;

typedef struct _GFX_YCBCR_TO_RGB
{
    COMMON_BITBLT_FIELDS

    VOID                *pv_cbcr;
    UINT32              ui4_cbcr_pitch;
    UINT8               ui1_alpha;
    GFX_YCBCR_FORMAT_T  e_ycbcr_format;
} GFX_YCBCR_TO_RGB_T;

typedef struct _GFX_PLTBLT_T
{
    COMMON_BITBLT_FIELDS

    GFX_BYTE_ALIGNED_T  e_byte_aligned;
    GFX_HPALETTE_T      h_palette;
} GFX_PLTBLT_T;

typedef struct _GFX_ROP_BITBLT_T
{
    COMMON_BITBLT_FIELDS

    GFX_ROP_TYPE_T      e_rop_type;
} GFX_ROP_BITBLT_T;

typedef struct _GFX_JPG_SCALE_T
{
    COMMON_BITBLT_FIELDS

    UINT32          ui4_dst_width;
    UINT32          ui4_dst_height;

    BOOL            b_do_segment;
    UINT32          ui4_vert_seg_length;
    UINT32          ui4_loop;

    /* output */
    UINT32          ui4_segment_count;
    GFX_SEG_INFO_T  *pt_vertical_info;
} GFX_JPG_SCALE_T;

typedef struct _GFX_REPLACE_COLOR_T
{
    COMMON_BITBLT_FIELDS

    UINT8           ui1_alpha;

    UINT32          *pui4_original_color;
    UINT32          *pui4_new_color;
    UINT8           ui1_color_num;
} GFX_REPLACE_COLOR_T;

typedef struct _GFX_COMPRESSED_BITBLT_T
{
    COMMON_BITBLT_FIELDS

    GFX_COMPRESSION_RATIO_T     e_ratio;
    BOOL                        b_line_separate;
    UINT32                      ui4_buffer_height;
} GFX_COMPRESSED_BITBLT_T;

typedef struct _GFX_3D_OSD_DATA_T
{
    COMMON_BITBLT_FIELDS

    GFX_3D_OSD_MODE_T   e_mode;
    UINT32              ui4_bmp_width;
    UINT32              ui4_bmp_height;
} GFX_3D_OSD_DATA_T;

#ifdef __cplusplus
}
#endif

#endif
