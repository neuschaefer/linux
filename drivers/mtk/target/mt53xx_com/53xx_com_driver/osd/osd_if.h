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
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: osd_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_if.h
 *  This header file declares public function prototypes of OSD.
 */

#ifndef OSD_IF_H
#define OSD_IF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/** OSD plane enum.
 */
typedef enum
{
    OSD_PLANE_1,
    OSD_PLANE_2,
    #ifdef CC_MT5363
    OSD_PLANE_3,
    #endif
    OSD_PLANE_MAX_NUM
} OSD_PLANE_T;


/** OSD scaler enum.
 */
typedef enum
{
    OSD_SCALER_2,
    #ifdef CC_MT5363
    OSD_SCALER_3,
    #endif
    OSD_SCALER_MAX_NUM
} OSD_SCALER_T;


/** OSD function return code.
 */
typedef enum
{
    OSD_RET_OK,
    OSD_RET_INV_ARG,
    OSD_RET_OUT_OF_MEM,
    OSD_RET_OUT_OF_REGION,
    OSD_RET_OUT_OF_LIST,
    OSD_RET_UNINIT,
    OSD_RET_INV_PLANE,
    OSD_RET_INV_SCALER,
    OSD_RET_INV_REGION,
    OSD_RET_INV_BITMAP,
    OSD_RET_INV_LIST,
    OSD_RET_INV_DISPLAY_MODE,
    OSD_RET_REGION_COLLISION,
    OSD_RET_ERR_INTERNAL,
    OSD_RET_UNDEF_ERR
} OSD_RET_CODE_T;

/** OSD color mode.
 */
typedef enum
{
    OSD_CM_YCBCR_CLUT2,
    OSD_CM_YCBCR_CLUT4,
    OSD_CM_YCBCR_CLUT8,
    OSD_CM_RESERVED_0,
    OSD_CM_CBYCRY422_DIRECT16,
    OSD_CM_YCBYCR422_DIRECT16,
    OSD_CM_AYCBCR8888_DIRECT32,
    OSD_CM_RESERVED_1,
    OSD_CM_RGB_CLUT2,
    OSD_CM_RGB_CLUT4,
    OSD_CM_RGB_CLUT8,
    OSD_CM_RGB565_DIRECT16,
    OSD_CM_ARGB1555_DIRECT16,
    OSD_CM_ARGB4444_DIRECT16,
    OSD_CM_ARGB8888_DIRECT32,
    OSD_CM_ALPHA_BIT
} OSD_COLOR_MODE_T;

/** OSD blending mode.
 */
typedef enum
{
    OSD_BM_NONE,
    OSD_BM_PIXEL,
    OSD_BM_REGION,
    OSD_BM_PLANE
} OSD_BLEND_MODE_T;

/** OSD region cmd.
 */
typedef enum
{
    OSD_RGN_ALLOC,
    OSD_RGN_PREV,
    OSD_RGN_NEXT,
    OSD_RGN_FLAGS,
    OSD_RGN_POS_X,
    OSD_RGN_POS_Y,
    OSD_RGN_BMP_W,
    OSD_RGN_BMP_H,
    OSD_RGN_DISP_W,
    OSD_RGN_DISP_H,
    OSD_RGN_OUT_W,
    OSD_RGN_OUT_H,
    OSD_RGN_COLORMODE,
    OSD_RGN_ALPHA,
    OSD_RGN_BMP_ADDR,
    OSD_RGN_BMP_PITCH,
    OSD_RGN_CLIP_V,
    OSD_RGN_CLIP_H,
    OSD_RGN_V_FLIP,
    OSD_RGN_H_MIRROR,
    OSD_RGN_PAL_LOAD,
    OSD_RGN_PAL_ADDR,
    OSD_RGN_PAL_LEN,
    OSD_RGN_STEP_V,
    OSD_RGN_STEP_H,
    OSD_RGN_COLOR_KEY,
    OSD_RGN_COLOR_KEY_EN,
    OSD_RGN_MIX_SEL,
    OSD_RGN_BIG_ENDIAN,
    OSD_RGN_ALPHA_SEL,
    OSD_RGN_YR_SEL,
    OSD_RGN_UG_SEL,
    OSD_RGN_VB_SEL,
    OSD_RGN_NEXT_EN,
    OSD_RGN_NEXT_HDR_ADDR,
    OSD_RGN_FIFO_EX
} OSD_RGN_CMD_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/** translate osd plane id to osd scaler id */
#define OSD_PLANE_TO_SCALER(X) ((X)-1)

/** translate osd sclaer id to osd plane id */
#define OSD_SCALER_TO_PLANE(X) ((X)+1)

/** input color mode, width and get coresspoding pitch size */
#define OSD_GET_PITCH_SIZE(CM, WIDTH, SIZE) \
    do \
    { \
        switch ((CM)) \
        { \
        case OSD_CM_CBYCRY422_DIRECT16: \
        case OSD_CM_YCBYCR422_DIRECT16: \
        case OSD_CM_RGB565_DIRECT16: \
        case OSD_CM_ARGB1555_DIRECT16: \
        case OSD_CM_ARGB4444_DIRECT16: \
            (SIZE) = (WIDTH) * 2; \
            break; \
        case OSD_CM_AYCBCR8888_DIRECT32: \
        case OSD_CM_ARGB8888_DIRECT32: \
            (SIZE) = (WIDTH) * 4; \
            break; \
        case OSD_CM_YCBCR_CLUT8: \
        case OSD_CM_RGB_CLUT8: \
            (SIZE) = (WIDTH); \
            break; \
        case OSD_CM_YCBCR_CLUT4: \
        case OSD_CM_RGB_CLUT4: \
            (SIZE) = (WIDTH) >> 1; \
            break; \
        case OSD_CM_YCBCR_CLUT2: \
        case OSD_CM_RGB_CLUT2: \
            (SIZE) = (WIDTH) >> 2; \
            break; \
        default: \
            ASSERT(0); \
        } \
    } while (0)


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/** reset osd driver and hardware status
 *  @param  void
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_Reset(void);

/** set osd plane position
 *  @param  u4Plane plane id
 *  @param  u4X x position
 *  @param  u4Y y position
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_BASE_SetOsdPosition(UINT32 u4Plane, UINT32 u4X, UINT32 u4Y);

/** get osd plane position
 *  @param  u4Plane plane id
 *  @param  pu4X x position pointer
 *  @param  pu4Y y position pointer
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_BASE_GetOsdPosition(UINT32 u4Plane, UINT32* pu4X,
                                     UINT32* pu4Y);

/** enable/disable osd plane, it will also disable scaler when disable plane
 *  @param  u4Plane plane id
 *  @param  fgEnable enable=TRUE/disable=FALSE
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_PLA_Enable(UINT32 u4Plane, BOOL fgEnable);

/** attach a specific list to osd plane
 *  @param  u4Plane plane id
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_PLA_FlipTo(UINT32 u4Plane, UINT32 u4RgnList);

/** get osd plane alpha level
 *  @param  u4Plane plane id
 *  @param  pu1BlendLevel aplha level pointer
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_PLA_GetBlendLevel(UINT32 u4Plane, UINT8* pu1BlendLevel);

/** set osd plane alpha level
 *  @param  u4Plane plane id
 *  @param  u1BlendLevel aplha level(0~255)
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_PLA_SetBlendLevel(UINT32 u4Plane, UINT8 u1BlendLevel);

/** get osd plane fading value
 *  @param  u4Plane plane id
 *  @param  pu1Fading fading value pointer
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_PLA_GetFading(UINT32 u4Plane, UINT8* pu1Fading);

/** set osd plane fading value
 *  @param  u4Plane plane id
 *  @param  u1Fading fading value
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_PLA_SetFading(UINT32 u4Plane, UINT8 u1Fading);

/** dump osd debug info
 *  @param  u4Plane osd plane
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_PLA_Dump(UINT32 u4Plane);

/** get the first region id of osd plane
 *  @param  u4Plane osd plane
 *  @retval region id, -1 is no region
 */
EXTERN INT32 OSD_PLA_GetFirstRegion(UINT32 u4Plane);

/** get the first region address of osd plane
 *  @param  u4Plane osd plane
 *  @retval region address, 0 is no region
 */
EXTERN UINT32 OSD_PLA_GetFirstRegionAddress(UINT32 u4Plane);


/** create an osd list
 *  @param pu4RgnList list id pointer
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_RGN_LIST_Create(UINT32 * pu4RgnList);

/** delete an osd list
 *  @param  u4RgnList list id
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_RGN_LIST_Delete(UINT32 u4RgnList);

/** get first region id of list
 *  @param  u4RgnList list id
 *  @param  pi4HeadRegion region id pointer
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_RGN_LIST_GetHead(UINT32 u4RgnList, INT32 * phHeadRegion);

/** get osd region attribute
 *  @param  u4Region region id
 *  @param  i4Cmd attribute name, refer to OSD_RGN_CMD_T
 *  @param  pu4Value attribute value pointer
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_RGN_Get(UINT32 u4Region, INT32 i4Cmd, UINT32 * pu4Value);

/** set osd region attribute
 *  @param  u4Region region id
 *  @param  i4Cmd attribute name, refer to OSD_RGN_CMD_T
 *  @param  u4Value attribute value
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_RGN_Set(UINT32 u4Region, INT32 i4Cmd, UINT32 u4Value);

/** create an osd region
 *  @param  pu4Region region id pointer
 *  @param  u4BmpWidth source bitmap width
 *  @param  u4BmpHeight source bitmap height
 *  @param  pvBitmap source bitmap address, MUST align with 16bytes
 *  @param  eColorMode, region color mode, refer to OSD_COLOR_MODE_T
 *  @param  u4BmpPitch, bytes per line, MUST align with 16bytes
 *  @param  u4DispX, region x position
 *  @param  u4DispY, region y position
 *  @param  u4DispW, output width
 *  @param  u4DispH, output height

 *  @retval OSD_RET_OK success
 *  @retval OSD_RET_INV_ARG invalid argument
 */
EXTERN INT32 OSD_RGN_Create(UINT32 * pu4Region, UINT32 u4BmpWidth,
                            UINT32 u4BmpHeight, void * pvBitmap,
                            UINT32 eColorMode, UINT32 u4BmpPitch,
                            UINT32 u4DispX, UINT32 u4DispY, UINT32 u4DispW,
                            UINT32 u4DispH);

/** delete an osd region
 *  @param  u4Region region id
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_RGN_Delete(UINT32 u4Region);

/** attach region to list
 *  @param  u4Region region id
 *  @param  u4RgnList list id
 *  @retval OSD_RET_OK success
 *  @retval OSD_RET_INV_LIST invalid list
 */
EXTERN INT32 OSD_RGN_Insert(UINT32 u4Region, UINT32 u4RgnList);

/** dettach region from list
 *  @param  u4Region region id
 *  @param  u4RgnList list id
 *  @retval OSD_RET_OK success
 *  @retval OSD_RET_INV_REGION invalid region
 */
EXTERN INT32 OSD_RGN_Detach(UINT32 u4Region, UINT32 u4RgnList);

/** cinfugure osd scaler
 *  @param  u4Scaler scaler id(only OSD_SCALER_2)
 *  @param  fgEnable enable=TRUE/disable=FALSE
 *  @param  u4SrcWidth osd input width
 *  @param  u4SrcHeight osd input height
 *  @param  u4DstWidth osd output width
 *  @param  u4DstHeight osd output height
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_SC_Scale(UINT32 u4Scaler, UINT32 u4Enable,
                          UINT32 u4SrcWidth, UINT32 u4SrcHeight,
                          UINT32 u4DstWidth, UINT32 u4DstHeight);

/** get osd scaler infomation
 *  @param  u4Scaler scaler id(only OSD_SCALER_2)
 *  @param  pu4Enable enable pointer
 *  @param  pu4SrcW osd input width pointer
 *  @param  pu4SrcH osd input height pointer
 *  @param  pu4DstW osd output width pointer
 *  @param  pu4DstH osd output height pointer
 *  @param  pu4Is16Bpp don't care
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_SC_GetScalerInfo(UINT32 u4Scaler, UINT32* pu4Enable,
                                  UINT32* pu4SrcW, UINT32* pu4SrcH,
                                  UINT32* pu4DstW, UINT32* pu4DstH,
                                  UINT32* pu4Is16Bpp);
                                  
/** enable low pass filter
 *  @param  u4Scaler scaler id(only OSD_SCALER_2)
 *  @param  fgEnable enable=TRUE/disable=FALSE
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_SC_SetLpf(UINT32 u4Scaler, UINT32 u4Enable);

EXTERN INT32 OSD_SetTVEOut(BOOL fgEnable);
EXTERN INT32 OSD_SetPowerDwn(BOOL fgEnable);
EXTERN INT32 OSD_ShowLogo(void);

#endif //OSD_IF_H
