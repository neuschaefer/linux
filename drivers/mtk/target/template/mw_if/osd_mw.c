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
 * $RCSfile: osd_mw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_mw.c
 *  This file defines the interfaces for middleware to access OSD driver
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "osd_drvif.h"
#define DEFINE_IS_LOG OSD_IsLog
#include "x_debug.h"

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "fbm_drvif.h"
#include "x_chip_id.h"
#include "x_common.h"
#include "x_rm.h"
#include "x_rm_dev_types.h"
#include "x_os.h"
#include "x_gfx.h"
#include "x_plane_mxr.h"
#include "x_sys_name.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "drvcust_if.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define BUFFER_EMPTY 0
#define BUFFER_FREE 1
#define BUFFER_ALLOC 2


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct _OSD_PALETTE_HANDLE_T
{
	void* pvFreePoint;
	void* pvPalette;
	GFX_PALETTE256_T rPalette;
	GFX_PALETTE_INIT_T rInit;
	GFX_PALETTE_PARAMS_T rParams;
} OSD_PALETTE_HANDLE_T;

typedef struct _OSD_BITMAP_HANDLE_T
{
	void* pvFreePoint;
	GFX_BITMAP_INIT_T rInit;
	GFX_BITMAP_LOCK_INFO_T rLockInfo;
	GFX_BITMAP_PALETTE_T rPalette;
	UINT32 u4BitsPerPixel;
} OSD_BITMAP_HANDLE_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define GET_OSDDRV_HANDLE(XHANDLE) ((UINT32)(XHANDLE) - 1)
#define GET_OSDMW_HANDLE(XHANDLE, XTYPE) ((XTYPE)(((UINT32)XHANDLE) + 1))

#define GET_COMMAND(FUNC)                                                      \
    do                                                                         \
    {                                                                          \
        if ((INT32)(FUNC) == (INT32)OSD_RET_OK)                                \
        {                                                                      \
            return RMR_OK;                                                     \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            return RMR_DRV_GET_FAILED;                                         \
        }                                                                      \
    } while (0)

#define SET_COMMAND(FUNC)                                                      \
    do                                                                         \
    {                                                                          \
        if ((INT32)(FUNC) == (INT32)OSD_RET_OK)                                \
        {                                                                      \
            return RMR_OK;                                                     \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            return RMR_DRV_SET_FAILED;                                         \
        }                                                                      \
    } while (0)

#define RET_ON_FAIL(FUNC)                                                      \
    do                                                                         \
    {                                                                          \
        INT32 i4Ret;                                                           \
        if ((i4Ret = (INT32)(FUNC)) != (INT32)OSD_RET_OK)                      \
        {                                                                      \
            return i4Ret;                                                      \
        }                                                                      \
    } while (0)

#define RET_ON_NO_SPACE(SPACE, SIZE)                                           \
    do                                                                         \
    {                                                                          \
        if ((SPACE) == NULL)                                                   \
        {                                                                      \
            return RMR_DRV_INV_GET_INFO;                                       \
        }                                                                      \
        else if (*(SPACE) > (SIZE))                                            \
        {                                                                      \
            *(SPACE) = (SIZE);                                                 \
        }                                                                      \
        else if (*(SPACE) < (SIZE))                                            \
        {                                                                      \
            *(SPACE) = (SIZE);                                                 \
            return RMR_DRV_NOT_ENOUGH_SPACE;                                   \
        }                                                                      \
    } while (0)

#define RET_ON_NULL(PT)                                                        \
    do                                                                         \
    {                                                                          \
        if ((void*)(PT) == NULL)                                               \
        {                                                                      \
            return -(INT32)OSD_RET_INV_ARG;                                    \
        }                                                                      \
    } while (0)

#define RET_ON_NULL_GET(PT)                                                    \
    do                                                                         \
    {                                                                          \
        if ((void*)(PT) == NULL)                                               \
        {                                                                      \
            return RMR_DRV_INV_GET_INFO;                                       \
        }                                                                      \
    } while (0)

#define RET_ON_NULL_SET(PT)                                                    \
    do                                                                         \
    {                                                                          \
        if ((void*)(PT) == NULL)                                               \
        {                                                                      \
            return RMR_DRV_INV_SET_INFO;                                       \
        }                                                                      \
    } while (0)

#define INIT_BUFFER(IDX)                                                       \
    do                                                                         \
    {                                                                          \
        prPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_JPEG_OSD##IDX);          \
        if (prPool->u4Size > 0)                                                \
        {                                                                      \
            _au4FbmBufferAddr[(IDX)-1] = prPool->u4Addr;                       \
            _au4FbmBufferSize[(IDX)-1] = prPool->u4Size;                       \
            _u4BufferNumber++;                                                 \
        }                                                                      \
    }                                                                          \
    while (0)

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

//extern INT32 _OSD_BASE_GetPreMulTarget(UINT32 *pu4Value);
extern INT32 MW_OSD_Init(void);
extern void MW_OsdConnect(INT32 hOsdPlane, INT32 fgEnable);
extern void OSD_Uninit(void);


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static BOOL _fgInitOK = FALSE;
static const GFX_PLANE_CAPS_T arMwPlaCaps[] =
{
    {
        /* ui4_cm_caps */
        (GFX_CAP_AYCbCr_CLUT2 |
        GFX_CAP_AYCbCr_CLUT4 |
        GFX_CAP_AYCbCr_CLUT8 |
        GFX_CAP_CbYCrY_16 |
        GFX_CAP_YCbYCr_16 |
        GFX_CAP_ARGB_CLUT2 |
        GFX_CAP_ARGB_CLUT4 |
        GFX_CAP_ARGB_CLUT8 |
        GFX_CAP_RGB_D565 |
        GFX_CAP_ARGB_D1555 |
        GFX_CAP_ARGB_D4444 |
        GFX_CAP_ARGB_D8888 |
        GFX_CAP_AYCbCr_D8888),
        /* ui4_func_caps */
        (GFX_CAPS_FADING_RATIO |
        GFX_CAPS_PLANE_REPOS)
    },
    {
        /* ui4_cm_caps */
        (GFX_CAP_AYCbCr_CLUT2 |
        GFX_CAP_AYCbCr_CLUT4 |
        GFX_CAP_AYCbCr_CLUT8 |
        GFX_CAP_CbYCrY_16 |
        GFX_CAP_YCbYCr_16 |
        GFX_CAP_ARGB_CLUT2 |
        GFX_CAP_ARGB_CLUT4 |
        GFX_CAP_ARGB_CLUT8 |
        GFX_CAP_RGB_D565 |
        GFX_CAP_ARGB_D1555 |
        GFX_CAP_ARGB_D4444 |
        GFX_CAP_ARGB_D8888 |
        GFX_CAP_AYCbCr_D8888),
        /* ui4_func_caps */
        (GFX_CAPS_ADV_SCALER |
        GFX_CAPS_FADING_RATIO |
        GFX_CAPS_PLANE_REPOS)
    },
    {
        /* ui4_cm_caps */
        (GFX_CAP_AYCbCr_CLUT2 |
        GFX_CAP_AYCbCr_CLUT4 |
        GFX_CAP_AYCbCr_CLUT8 |
        GFX_CAP_CbYCrY_16 |
        GFX_CAP_YCbYCr_16 |
        GFX_CAP_ARGB_CLUT2 |
        GFX_CAP_ARGB_CLUT4 |
        GFX_CAP_ARGB_CLUT8 |
        GFX_CAP_RGB_D565 |
        GFX_CAP_ARGB_D1555 |
        GFX_CAP_ARGB_D4444 |
        GFX_CAP_ARGB_D8888 |
        GFX_CAP_AYCbCr_D8888),
        /* ui4_func_caps */
        (GFX_CAPS_ADV_SCALER |
        GFX_CAPS_FADING_RATIO |
        GFX_CAPS_PLANE_REPOS)
    }
};
static UINT32 _au4FbmBufferAddr[4];
static UINT32 _au4FbmBufferSize[4];
static UINT8 _au1FbmBufferStstus[4]; // 0:free, 1:allocated
static UINT32 _u4BufferNumber = 0;


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static UINT32 _OsdGetDrvColorMode(GFX_COLORMODE_T eMwColorMode)
{
    switch (eMwColorMode)
    {
    case GFX_COLORMODE_AYCbCr_CLUT2:
        return (UINT32)OSD_CM_YCBCR_CLUT2;

    case GFX_COLORMODE_AYCbCr_CLUT4:
        return (UINT32)OSD_CM_YCBCR_CLUT4;

    case GFX_COLORMODE_AYCbCr_CLUT8:
        return (UINT32)OSD_CM_YCBCR_CLUT8;

    case GFX_COLORMODE_CbYCrY_16:
        return (UINT32)OSD_CM_CBYCRY422_DIRECT16;

    case GFX_COLORMODE_YCbYCr_16:
        return (UINT32)OSD_CM_YCBYCR422_DIRECT16;

    case GFX_COLORMODE_AYCbCr_D8888:
        return (UINT32)OSD_CM_AYCBCR8888_DIRECT32;

    case GFX_COLORMODE_ARGB_CLUT2:
        return (UINT32)OSD_CM_RGB_CLUT2;

    case GFX_COLORMODE_ARGB_CLUT4:
        return (UINT32)OSD_CM_RGB_CLUT4;

    case GFX_COLORMODE_ARGB_CLUT8:
        return (UINT32)OSD_CM_RGB_CLUT8;

    case GFX_COLORMODE_RGB_D565:
        return (UINT32)OSD_CM_RGB565_DIRECT16;

    case GFX_COLORMODE_ARGB_D1555:
        return (UINT32)OSD_CM_ARGB1555_DIRECT16;

    case GFX_COLORMODE_ARGB_D4444:
        return (UINT32)OSD_CM_ARGB4444_DIRECT16;

    case GFX_COLORMODE_ARGB_D8888:
    default:
        return (UINT32)OSD_CM_ARGB8888_DIRECT32;
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static UINT32 _OsdGetMwColorMode(UINT32 u4DrvColorMode)
{
    switch (u4DrvColorMode)
    {
    case (UINT32)OSD_CM_YCBCR_CLUT2:
        return (UINT32)GFX_COLORMODE_AYCbCr_CLUT2;

    case (UINT32)OSD_CM_YCBCR_CLUT4:
        return (UINT32)GFX_COLORMODE_AYCbCr_CLUT4;

    case (UINT32)OSD_CM_YCBCR_CLUT8:
        return (UINT32)GFX_COLORMODE_AYCbCr_CLUT8;

    case (UINT32)OSD_CM_CBYCRY422_DIRECT16:
        return (UINT32)GFX_COLORMODE_CbYCrY_16;

    case (UINT32)OSD_CM_YCBYCR422_DIRECT16:
        return (UINT32)GFX_COLORMODE_YCbYCr_16;

    case (UINT32)OSD_CM_AYCBCR8888_DIRECT32:
        return (UINT32)GFX_COLORMODE_AYCbCr_D8888;

    case (UINT32)OSD_CM_RGB_CLUT2:
        return (UINT32)GFX_COLORMODE_ARGB_CLUT2;

    case (UINT32)OSD_CM_RGB_CLUT4:
        return (UINT32)GFX_COLORMODE_ARGB_CLUT4;

    case (UINT32)OSD_CM_RGB_CLUT8:
        return (UINT32)GFX_COLORMODE_ARGB_CLUT8;

    case (UINT32)OSD_CM_RGB565_DIRECT16:
        return (UINT32)GFX_COLORMODE_RGB_D565;

    case (UINT32)OSD_CM_ARGB1555_DIRECT16:
        return (UINT32)GFX_COLORMODE_ARGB_D1555;

    case (UINT32)OSD_CM_ARGB4444_DIRECT16:
        return (UINT32)GFX_COLORMODE_ARGB_D4444;

    case (UINT32)OSD_CM_ARGB8888_DIRECT32:
    default:
        return (UINT32)GFX_COLORMODE_ARGB_D8888;
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static UINT32 _OsdGetBitsPerPixel(UINT32 u4DrvColorMode)
{
	switch (u4DrvColorMode)
	{
	case OSD_CM_YCBCR_CLUT2:
	case OSD_CM_RGB_CLUT2:
		return 2;

	case OSD_CM_YCBCR_CLUT4:
	case OSD_CM_RGB_CLUT4:
		return 4;

	case OSD_CM_YCBCR_CLUT8:
	case OSD_CM_RGB_CLUT8:
		return 8;

	case OSD_CM_CBYCRY422_DIRECT16:
	case OSD_CM_YCBYCR422_DIRECT16:
	case OSD_CM_RGB565_DIRECT16:
	case OSD_CM_ARGB1555_DIRECT16:
	case OSD_CM_ARGB4444_DIRECT16:
		return 16;

	case OSD_CM_AYCBCR8888_DIRECT32:
	case OSD_CM_ARGB8888_DIRECT32:
	default:
		return 32;
	}
}


static void _OsdInitFbmBuffer(void)
{
    FBM_POOL_T* prPool;

    x_memset(_au4FbmBufferAddr, 0, sizeof(_au4FbmBufferAddr));
    x_memset(_au4FbmBufferSize, 0, sizeof(_au4FbmBufferSize));
    x_memset(_au1FbmBufferStstus, BUFFER_EMPTY, sizeof(_au1FbmBufferStstus));

    INIT_BUFFER(1);
    INIT_BUFFER(2);
    INIT_BUFFER(3);
    INIT_BUFFER(4);

    x_memset(_au1FbmBufferStstus, BUFFER_FREE, _u4BufferNumber);
}


static UINT32 _OsdAllocateFbmBuffer(UINT32* pu4BufferSize)
{
    UINT32 i;

    for (i = 0; i < _u4BufferNumber; i++)
    {
        if (_au1FbmBufferStstus[i] == BUFFER_FREE)
        {
            _au1FbmBufferStstus[i] = BUFFER_ALLOC;
            *pu4BufferSize = _au4FbmBufferSize[i];
            return _au4FbmBufferAddr[i];
        }
    }

    return NULL;
}


static void _OsdFreeFbmBuffer(void* pvBufferAddr)
{
    UINT32 i;

    for (i = 0; i < _u4BufferNumber; i++)
    {
        if (_au4FbmBufferAddr[i] == (UINT32)pvBufferAddr)
        {
            _au1FbmBufferStstus[i] = BUFFER_FREE;
            break;
        }
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdGetPlaneCaps(UINT32 u4Plane, GFX_PLANE_CAPS_T* prCaps)
{
    x_memcpy(prCaps, &arMwPlaCaps[u4Plane], sizeof(GFX_PLANE_CAPS_T));

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdCreateRgnList(GFX_HRGNLIST_T* prRgnList)
{
    UINT32 u4RgnList;
    RET_ON_NULL(prRgnList);

    RET_ON_FAIL(OSD_RGN_LIST_Create(&u4RgnList));
    *prRgnList = GET_OSDMW_HANDLE(u4RgnList, GFX_HRGNLIST_T);
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdCreateRegion(GFX_REGION_INIT_T* prRegionInit)
{
    UINT32 u4Rgn;

    OSD_BITMAP_HANDLE_T *prBmp;
    OSD_PALETTE_HANDLE_T *prPal;
    UINT32 u4PalAddr;
    UINT32 u4NewPalette;
    UINT32 eColorMode;

    RET_ON_NULL(prRegionInit);
    RET_ON_NULL(prRegionInit->h_bitmap);

    prBmp = (OSD_BITMAP_HANDLE_T*)(void*)prRegionInit->h_bitmap;
    eColorMode = _OsdGetDrvColorMode(prBmp->rInit.e_colormode);

    RET_ON_FAIL(OSD_RGN_Create(&u4Rgn, prBmp->rInit.ui4_bmp_w,
                               prBmp->rInit.ui4_bmp_h,
                               prBmp->rLockInfo.at_subplane[0].pv_bits,
                               eColorMode,
                               prBmp->rLockInfo.at_subplane[0].ui4_pitch,
                               0, 0, prRegionInit->ui4_out_w,
                               prRegionInit->ui4_out_h));

    if ((OSD_RGN_Set(u4Rgn, (INT32)OSD_RGN_STEP_H,
                    (UINT32)prRegionInit->t_shrink_x) != (INT32)OSD_RET_OK) ||
        (OSD_RGN_Set(u4Rgn, (INT32)OSD_RGN_STEP_V,
                    (UINT32)prRegionInit->t_shrink_y) != (INT32)OSD_RET_OK) ||
        (OSD_RGN_Set(u4Rgn, (INT32)OSD_RGN_MIX_SEL,
                    (UINT32)prRegionInit->ui1_blend_option) != (INT32)OSD_RET_OK) ||
        (OSD_RGN_Set(u4Rgn, (INT32)OSD_RGN_ALPHA,
                    (UINT32)prRegionInit->ui1_blend_level) != (INT32)OSD_RET_OK) ||
        (OSD_RGN_Set(u4Rgn, (INT32)OSD_RGN_COLOR_KEY_EN,
                    (UINT32)prRegionInit->b_enable_colorkey) != (INT32)OSD_RET_OK) ||
        (OSD_RGN_Set(u4Rgn, (INT32)OSD_RGN_COLOR_KEY,
                    (UINT32)prRegionInit->ui4_colorkey) != (INT32)OSD_RET_OK))
    {
        IGNORE_RET(OSD_RGN_Delete(u4Rgn));
        return RMR_DRV_SET_FAILED;
    }

    prRegionInit->h_new_region = GET_OSDMW_HANDLE(u4Rgn, GFX_HREGION_T);

    if ((eColorMode <= (UINT32)OSD_CM_YCBCR_CLUT8) ||
        ((eColorMode >= (UINT32)OSD_CM_RGB_CLUT2) &&
         (eColorMode <= (UINT32)OSD_CM_RGB_CLUT8)))
    {
        // change palette or set default palette
        if (prBmp == (OSD_BITMAP_HANDLE_T*)(void*)prBmp->rPalette.h_bitmap)
        {
            // change palette
            prPal = (OSD_PALETTE_HANDLE_T*)(void*)prBmp->rPalette.h_palette;
            u4PalAddr = (UINT32)prPal->pvPalette;
            u4NewPalette = 1;
        }
        else
        {
            // use previous palette
            u4PalAddr = 0;
            u4NewPalette = 0;
        }
        IGNORE_RET(OSD_RGN_Set(u4Rgn, (INT32)OSD_RGN_PAL_LOAD, u4NewPalette));
        IGNORE_RET(OSD_RGN_Set(u4Rgn, (INT32)OSD_RGN_PAL_ADDR, u4PalAddr));
    }

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdGetRegionInfo(GFX_REGION_INFO_T* prRegionInfo)
{
    UINT32 u4Rgn, u4Value;

    RET_ON_NULL(prRegionInfo);
    RET_ON_NULL(prRegionInfo->h_region);

    u4Rgn = GET_OSDDRV_HANDLE(prRegionInfo->h_region);

    IGNORE_RET(OSD_RGN_Get(u4Rgn, (INT32)OSD_RGN_POS_X, &u4Value));
    prRegionInfo->i4_out_x = (INT32)u4Value;
    IGNORE_RET(OSD_RGN_Get(u4Rgn, (INT32)OSD_RGN_POS_Y, &u4Value));
    prRegionInfo->i4_out_y = (INT32)u4Value;

    IGNORE_RET(OSD_RGN_Get(u4Rgn, (INT32)OSD_RGN_OUT_W, &u4Value));
    prRegionInfo->ui4_out_w = u4Value;
    IGNORE_RET(OSD_RGN_Get(u4Rgn, (INT32)OSD_RGN_OUT_H, &u4Value));
    prRegionInfo->ui4_out_h = u4Value;

    IGNORE_RET(OSD_RGN_Get(u4Rgn, (INT32)OSD_RGN_STEP_H, &u4Value));
    prRegionInfo->t_shrink_x = (GFX_FP_T)u4Value;
    IGNORE_RET(OSD_RGN_Get(u4Rgn, (INT32)OSD_RGN_STEP_V, &u4Value));
    prRegionInfo->t_shrink_y = (GFX_FP_T)u4Value;

    IGNORE_RET(OSD_RGN_Get(u4Rgn, (INT32)OSD_RGN_MIX_SEL, &u4Value));
    prRegionInfo->ui1_blend_option = u4Value;
    IGNORE_RET(OSD_RGN_Get(u4Rgn, (INT32)OSD_RGN_ALPHA, &u4Value));
    prRegionInfo->ui1_blend_level = u4Value;

    IGNORE_RET(OSD_RGN_Get(u4Rgn, (INT32)OSD_RGN_COLOR_KEY_EN, &u4Value));
    prRegionInfo->b_colorkey = u4Value;
    IGNORE_RET(OSD_RGN_Get(u4Rgn, (INT32)OSD_RGN_COLOR_KEY, &u4Value));
    prRegionInfo->ui4_colorkey = u4Value;

    IGNORE_RET(OSD_RGN_Get(u4Rgn, (INT32)OSD_RGN_COLORMODE, &u4Value));
    prRegionInfo->e_colormode = (GFX_COLORMODE_T)_OsdGetMwColorMode(u4Value);

    IGNORE_RET(OSD_RGN_Get(u4Rgn, (INT32)OSD_RGN_BMP_W, &u4Value));
    prRegionInfo->ui4_bmp_w = u4Value;
    IGNORE_RET(OSD_RGN_Get(u4Rgn, (INT32)OSD_RGN_BMP_H, &u4Value));
    prRegionInfo->ui4_bmp_h = u4Value;

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdFlipPlane(UINT32 u4Plane, GFX_HRGNLIST_T rRgnList)
{
    if (rRgnList)
    {
        RET_ON_FAIL(OSD_PLA_FlipTo(u4Plane, GET_OSDDRV_HANDLE(rRgnList)));
    }
    else
    {
        RET_ON_FAIL(OSD_PLA_FlipToNone(u4Plane));
    }

    RET_ON_FAIL(OSD_PLA_Enable(u4Plane, TRUE));

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdAttachRegion(const GFX_REGION_POS_T* prRgnPos)
{
    UINT32 u4RgnList, u4Rgn;

    // get driver handle
    RET_ON_NULL(prRgnPos);
    RET_ON_NULL(prRgnPos->h_rgnlist);
    RET_ON_NULL(prRgnPos->h_region);

    u4RgnList = GET_OSDDRV_HANDLE(prRgnPos->h_rgnlist);
    u4Rgn = GET_OSDDRV_HANDLE(prRgnPos->h_region);
    // insert into list
    RET_ON_FAIL(OSD_RGN_Set(u4Rgn, (INT32)OSD_RGN_POS_X, (UINT32)prRgnPos->i4_out_x));
    RET_ON_FAIL(OSD_RGN_Set(u4Rgn, (INT32)OSD_RGN_POS_Y, (UINT32)prRgnPos->i4_out_y));
    RET_ON_FAIL(OSD_RGN_Insert(u4Rgn, u4RgnList));

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdDetachRegion(const GFX_REGION_DETACH_T* prRgnDetach)
{
    UINT32 u4RgnList, u4Rgn;
    // get driver handle

    RET_ON_NULL(prRgnDetach);
    RET_ON_NULL(prRgnDetach->h_rgnlist);
    RET_ON_NULL(prRgnDetach->h_region);

    u4RgnList = GET_OSDDRV_HANDLE(prRgnDetach->h_rgnlist);
    u4Rgn = GET_OSDDRV_HANDLE(prRgnDetach->h_region);
    RET_ON_FAIL(OSD_RGN_Detach(u4Rgn, u4RgnList));

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdSetRegionAttr(const GFX_REGION_ATTR_T* prRgnAttr)
{
    UINT32 u4Rgn;

    RET_ON_NULL(prRgnAttr);
    RET_ON_NULL(prRgnAttr->h_region);

    u4Rgn = GET_OSDDRV_HANDLE(prRgnAttr->h_region);

    if (prRgnAttr->ui4_flag & GFX_RGN_ATTR_SHRINK_X)
    {
        RET_ON_FAIL(OSD_RGN_Set(u4Rgn, (INT32)OSD_RGN_STEP_H, prRgnAttr->t_shrink_x));
    }
    if (prRgnAttr->ui4_flag & GFX_RGN_ATTR_SHRINK_Y)
    {
        RET_ON_FAIL(OSD_RGN_Set(u4Rgn, (INT32)OSD_RGN_STEP_V, prRgnAttr->t_shrink_y));
    }
    if (prRgnAttr->ui4_flag & GFX_RGN_ATTR_BLEND_OPTION)
    {
        RET_ON_FAIL(OSD_RGN_Set(u4Rgn, (INT32)OSD_RGN_MIX_SEL,
                    prRgnAttr->ui1_blend_option));
    }
    if (prRgnAttr->ui4_flag & GFX_RGN_ATTR_BLEND_LEVEL)
    {
        RET_ON_FAIL(OSD_RGN_Set(u4Rgn, (INT32)OSD_RGN_ALPHA,
                    prRgnAttr->ui1_blend_level));
    }
    if (prRgnAttr->ui4_flag & GFX_RGN_ATTR_COLORKEY_ENABLE)
    {
        RET_ON_FAIL(OSD_RGN_Set(u4Rgn, (INT32)OSD_RGN_COLOR_KEY_EN,
                    prRgnAttr->b_colorkey));
    }
    if (prRgnAttr->ui4_flag & GFX_RGN_ATTR_COLORKEY)
    {
        RET_ON_FAIL(OSD_RGN_Set(u4Rgn, (INT32)OSD_RGN_COLOR_KEY,
                    prRgnAttr->ui4_colorkey));
    }
    if (prRgnAttr->ui4_flag & GFX_RGN_ATTR_WIDTH)
    {
        RET_ON_FAIL(OSD_RGN_Set(u4Rgn, (INT32)OSD_RGN_OUT_W, prRgnAttr->ui4_width));
    }
    if (prRgnAttr->ui4_flag & GFX_RGN_ATTR_HEIGHT)
    {
        RET_ON_FAIL(OSD_RGN_Set(u4Rgn, (INT32)OSD_RGN_OUT_H, prRgnAttr->ui4_height));
    }
    if (prRgnAttr->ui4_flag & GFX_RGN_ATTR_SURF_OFFSET_X)
    {
        RET_ON_FAIL(OSD_RGN_Set(u4Rgn, (INT32)OSD_RGN_CLIP_H, prRgnAttr->ui4_surf_offset_x));
    }
    if (prRgnAttr->ui4_flag & GFX_RGN_ATTR_SURF_OFFSET_Y)
    {
        RET_ON_FAIL(OSD_RGN_Set(u4Rgn, (INT32)OSD_RGN_CLIP_V, prRgnAttr->ui4_surf_offset_y));
    }

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
// Static functions, bitmap related API
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdCreateBitmap(GFX_BITMAP_INIT_T* prBmpInit, BOOL fgTakeFbmBuffer)
{
    OSD_BITMAP_HANDLE_T *prBmp;
    UINT32 u4bpp; // bits per pixel
    UINT32 u4Pitch;
    UINT32 u4Lines;

    RET_ON_NULL(prBmpInit);

    prBmp = x_mem_alloc(sizeof(OSD_BITMAP_HANDLE_T));
    if (prBmp == NULL)
    {
        return -(INT32)OSD_RET_OUT_OF_MEM;
    }

    // create bitmap bits area
    u4bpp = _OsdGetBitsPerPixel(_OsdGetDrvColorMode(prBmpInit->e_colormode));

    // bytes, aligned on 128 bits
    u4Pitch = (((u4bpp * prBmpInit->ui4_bmp_w) + (15 * 8)) >> 3) & (~0xf);
    // luis@2008/1/30, remove even line protection
    //u4Lines = (prBmpInit->ui4_bmp_h + 1) & (~1);
    u4Lines = prBmpInit->ui4_bmp_h;

    // build init info
    x_memset(prBmp, 0, sizeof(OSD_BITMAP_HANDLE_T));

    if (fgTakeFbmBuffer)
    {
        prBmp->pvFreePoint =
        (void*)_OsdAllocateFbmBuffer((UINT32*)&prBmpInit->z_new_bitmap_size);
    }
    else
    {
        prBmp->pvFreePoint =
        (void*)x_mem_alloc((u4Pitch * u4Lines) + 32 + OSD_DMA_ALIGN_SIZE + 32);
        prBmpInit->z_new_bitmap_size = u4Pitch * u4Lines;
    }

    if (prBmp->pvFreePoint == NULL)
    {
        return -(INT32)OSD_RET_OUT_OF_MEM;
    }

    if (fgTakeFbmBuffer)
    {
        x_memset(prBmp->pvFreePoint, 0, u4Pitch * u4Lines);
    }
    else
    {
        x_memset(prBmp->pvFreePoint, 0, (u4Pitch * u4Lines) + 32 + OSD_DMA_ALIGN_SIZE + 32);
    }
    HalFlushDCache();

    x_memcpy(&prBmp->rInit, prBmpInit, sizeof(GFX_BITMAP_INIT_T));
    prBmp->rInit.h_new_bitmap = (GFX_HBITMAP_T)(void*)prBmp;

    // build lock info
    prBmp->rLockInfo.e_colormode = prBmpInit->e_colormode;
    prBmp->rLockInfo.z_subplane = 1;
    prBmp->rLockInfo.at_subplane[0].ui4_width = prBmpInit->ui4_bmp_w;
    prBmp->rLockInfo.at_subplane[0].ui4_height = prBmpInit->ui4_bmp_h;
    prBmp->rLockInfo.at_subplane[0].ui4_pitch = u4Pitch;

    if (fgTakeFbmBuffer)
    {
        prBmp->rLockInfo.at_subplane[0].pv_bits = prBmp->pvFreePoint;
    }
    else
    {
        prBmp->rLockInfo.at_subplane[0].pv_bits =
        (void*)(((UINT32)prBmp->pvFreePoint + 32 + OSD_DMA_ALIGN_SIZE) & (~OSD_DMA_ALIGN_SIZE));
    }

    // skip first 32 bytes, 128 bits aligned
    prBmp->rLockInfo.h_bitmap = (GFX_HBITMAP_T)(void*)prBmp;

    //indexed color mode
    prBmp->rPalette.h_bitmap = (GFX_HBITMAP_T)(void*)prBmp;
    prBmp->rPalette.h_palette = prBmp->rInit.h_palette;
    prBmp->u4BitsPerPixel = u4bpp;

    prBmpInit->h_new_bitmap = prBmp->rInit.h_new_bitmap;
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdDeleteBitmap(GFX_HBITMAP_T rBmp, BOOL fgTakeFbmBuffer)
{
    OSD_BITMAP_HANDLE_T *prBmp;

    RET_ON_NULL(rBmp);

    prBmp = (OSD_BITMAP_HANDLE_T*)(void*)rBmp;
    if (prBmp->pvFreePoint)
    {
        if (fgTakeFbmBuffer)
        {
            _OsdFreeFbmBuffer(prBmp->pvFreePoint);
        }
        else
        {
            x_mem_free(prBmp->pvFreePoint);
        }
        x_mem_free(prBmp);
    }
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdLockBitmap(GFX_BITMAP_LOCK_INFO_T* prLockInfo)
{
    OSD_BITMAP_HANDLE_T *prBmp;

    RET_ON_NULL(prLockInfo);
    RET_ON_NULL(prLockInfo->h_bitmap);

    prBmp = (OSD_BITMAP_HANDLE_T*)(void*)prLockInfo->h_bitmap;

    if (prBmp->rLockInfo.h_bitmap != (GFX_HBITMAP_T)(void*)prBmp)
    {
        return -(INT32)OSD_RET_ERR_INTERNAL;
    }

    x_memcpy(prLockInfo, &prBmp->rLockInfo, sizeof(GFX_BITMAP_LOCK_INFO_T));

    HalFlushDCache();
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdUnlockBitmap(GFX_HBITMAP_T rBitmap)
{
    OSD_BITMAP_HANDLE_T *prBmp;

    RET_ON_NULL(rBitmap);

    prBmp = (OSD_BITMAP_HANDLE_T*)(void*)rBitmap;

    if (prBmp->rLockInfo.h_bitmap != rBitmap)
    {
        return -(INT32)OSD_RET_ERR_INTERNAL;
    }

    HalFlushDCache();

    // do nothing?
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdGetBitmapPalette(GFX_BITMAP_PALETTE_T* prBmpPal)
{
    OSD_BITMAP_HANDLE_T *prBmp;

    RET_ON_NULL(prBmpPal);
    RET_ON_NULL(prBmpPal->h_bitmap);

    prBmp = (OSD_BITMAP_HANDLE_T*)(void*)prBmpPal->h_bitmap;

    if (prBmp->rPalette.h_bitmap != (GFX_HBITMAP_T)(void*)prBmp)
    {
        return -(INT32)OSD_RET_ERR_INTERNAL;
    }

    if (prBmp->u4BitsPerPixel > 8)
    {
        // osd case3 - bitmap in direct color mode can not set/get palette
        return -(INT32)OSD_RET_INV_ARG;
    }

    prBmpPal->h_palette = prBmp->rPalette.h_palette;

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdSetBitmapPalette(const GFX_BITMAP_PALETTE_T* prBmpPal)
{
    OSD_BITMAP_HANDLE_T *prBmp;

    RET_ON_NULL(prBmpPal);
    RET_ON_NULL(prBmpPal->h_bitmap);

    prBmp = (OSD_BITMAP_HANDLE_T*)(void*)prBmpPal->h_bitmap;

    if (prBmp->u4BitsPerPixel > 8)
    {
        // osd case3 - bitmap in direct color mode can not set/get palette
        return -(INT32)OSD_RET_INV_ARG;
    }

    if (prBmp->rPalette.h_bitmap != (GFX_HBITMAP_T)(void*)prBmp)
    {
        return -(INT32)OSD_RET_ERR_INTERNAL;
    }

    x_memcpy(&prBmp->rPalette, prBmpPal, sizeof(GFX_BITMAP_PALETTE_T));

    if (prBmp->rPalette.h_palette)
    {
        // here we do have a real palette
        OSD_PALETTE_HANDLE_T *prPal;
        prPal = (OSD_PALETTE_HANDLE_T*)(void*)prBmp->rPalette.h_palette;
        if (prPal->rParams.h_palette != (GFX_HPALETTE_T)(void*)prPal)
        {
            return -(INT32)OSD_RET_ERR_INTERNAL;
        }
        // do nothing?
    }

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
// Static functions, palette related API
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdCreatePalette(GFX_PALETTE_INIT_T* prPalInit)
{
    OSD_PALETTE_HANDLE_T *prPal;

    RET_ON_NULL(prPalInit);

    // create instance
    prPal = (OSD_PALETTE_HANDLE_T*)x_mem_alloc(sizeof(OSD_PALETTE_HANDLE_T));
    if (prPal == NULL)
    {
        return -(INT32)OSD_RET_OUT_OF_MEM;
    }

    // duplicate init structure and palette
    x_memcpy(&prPal->rInit, prPalInit, sizeof(GFX_PALETTE_INIT_T));
    prPal->rInit.h_new_palette = (GFX_HPALETTE_T)(void*)prPal;
    x_memcpy(&prPal->rPalette, prPalInit->pt_palette, sizeof(GFX_PALETTE1_T));
    prPal->rInit.pt_palette = (GFX_PALETTE_PTR)(void*)&prPal->rPalette;

    x_memcpy(prPal->rPalette.at_clut_entry,
             prPalInit->pt_palette->at_clut_entry,
             256 * sizeof(GFX_COLOR_PACKED_T));

    // initial params
    prPal->rParams.h_palette = (GFX_HPALETTE_T)(void*)prPal;
    prPal->rParams.ui1_alpha_bit_select =
    prPal->rPalette.ui1_alpha_bit_select;
    prPal->rParams.ui1_alpha_select = prPal->rPalette.ui1_alpha_select;
    prPal->rParams.ui1_YR_select = prPal->rPalette.ui1_YR_select;
    prPal->rParams.ui1_UG_select = prPal->rPalette.ui1_UG_select;
    prPal->rParams.ui1_VB_select = prPal->rPalette.ui1_VB_select;
    // prepare aligned palette memory
    prPal->pvFreePoint = (void*)x_mem_alloc((256 * 4) + 32 + 32 + OSD_DMA_ALIGN_SIZE);
    if (prPal->pvFreePoint == NULL)
    {
        x_mem_free(prPal);
        return (INT32)OSD_RET_OUT_OF_MEM;
    }
    prPal->pvPalette =
    (void*)(((UINT32)(prPal->pvFreePoint) + 32 + OSD_DMA_ALIGN_SIZE) & (~OSD_DMA_ALIGN_SIZE));

    x_memcpy((void*)prPal->pvPalette, &prPal->rPalette.at_clut_entry, 256 * 4);
    HalFlushDCache();
    // write back palette handle
    prPalInit->h_new_palette = (GFX_HPALETTE_T)(void*)prPal;

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdDeletePalette(GFX_HPALETTE_T rPal)
{
    OSD_PALETTE_HANDLE_T *prPal;

    RET_ON_NULL(rPal);

    prPal = (OSD_PALETTE_HANDLE_T*)(void*)rPal;

    if (prPal->rParams.h_palette != rPal)
    {
        return -(INT32)OSD_RET_ERR_INTERNAL;
    }

    if (prPal->pvFreePoint == NULL)
    {
        return -(INT32)OSD_RET_ERR_INTERNAL;
    }

    x_mem_free(prPal->pvFreePoint);
    x_mem_free(prPal);

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdGetPaletteParams(GFX_PALETTE_PARAMS_T* prPalParams)
{
    OSD_PALETTE_HANDLE_T *prPal;

    RET_ON_NULL(prPalParams);
    RET_ON_NULL(prPalParams->h_palette);

    prPal = (OSD_PALETTE_HANDLE_T*)(void*)prPalParams->h_palette;

    if (prPal->rParams.h_palette != prPalParams->h_palette)
    {
        return -(INT32)OSD_RET_ERR_INTERNAL;
    }

    x_memcpy(prPalParams, &prPal->rParams, sizeof(GFX_PALETTE_PARAMS_T));
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdSetPaletteParams(const GFX_PALETTE_PARAMS_T* prPalParams)
{
    OSD_PALETTE_HANDLE_T *prPal;

    RET_ON_NULL(prPalParams);
    RET_ON_NULL(prPalParams->h_palette);

    prPal = (OSD_PALETTE_HANDLE_T*)(void*)prPalParams->h_palette;

    if (prPal->rParams.h_palette != prPalParams->h_palette)
    {
        return -(INT32)OSD_RET_ERR_INTERNAL;
    }

    x_memcpy(&prPal->rParams, prPalParams, sizeof(GFX_PALETTE_PARAMS_T));
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdGetPaletteEntries(const GFX_PALETTE_ENTRIES_T* prPalEntries)
{
    OSD_PALETTE_HANDLE_T *prPal;
    UINT32 *pu4Entries;
    UINT32 u4Bytes;

    RET_ON_NULL(prPalEntries);
    RET_ON_NULL(prPalEntries->h_palette);
    RET_ON_NULL(prPalEntries->pui4_entries);

    prPal = (OSD_PALETTE_HANDLE_T*)(void*)prPalEntries->h_palette;

    if (prPal->rParams.h_palette != prPalEntries->h_palette)
    {
        return -(INT32)OSD_RET_ERR_INTERNAL;
    }

    pu4Entries = &((UINT32*)prPal->pvPalette)[prPalEntries->ui2_start];
    u4Bytes = prPalEntries->ui2_count * 4;
    x_memcpy(prPalEntries->pui4_entries, pu4Entries, u4Bytes);
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdSetPaletteEntries(const GFX_PALETTE_ENTRIES_T* prPalEntries)
{
    OSD_PALETTE_HANDLE_T *prPal;
    UINT32 *pu4Entries;
    UINT32 u4Bytes;

    RET_ON_NULL(prPalEntries);
    RET_ON_NULL(prPalEntries->h_palette);

    prPal = (OSD_PALETTE_HANDLE_T*)(void*)prPalEntries->h_palette;

    if (prPal->rParams.h_palette != prPalEntries->h_palette)
    {
        return -(INT32)OSD_RET_ERR_INTERNAL;
    }

    if (prPalEntries->pui4_entries == NULL)
    {
        return -(INT32)OSD_RET_ERR_INTERNAL;
    }

    //pu4Entries = (UINT32*)NONCACHE((UINT32)(&((UINT32*)prPal->pvPalette)[prPalEntries->ui2_start]));
    pu4Entries = (UINT32*)(&((UINT32*)prPal->pvPalette)[prPalEntries->ui2_start]);
    u4Bytes = prPalEntries->ui2_count * 4;
    x_memcpy(pu4Entries, prPalEntries->pui4_entries, u4Bytes);
    HalFlushDCache();
    pu4Entries = &prPal->rPalette.at_clut_entry[prPalEntries->ui2_start];
    x_memcpy(pu4Entries, prPalEntries->pui4_entries, u4Bytes);

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
// Static functions, 5371 add-in API
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdGetScalerInfo(UINT32 u4Plane, GFX_ADV_SCALER_PARAM_T* prParam)
{
    UINT32 u4Scaler;
    UINT32 u4Enable;
    UINT32 u4Is16Bpp;
    INT16 ai2LpfParam[5];

    RET_ON_NULL(prParam);

    OSD_VERIFY_PLANE(u4Plane);
    u4Scaler = OSD_PLANE_TO_SCALER(u4Plane);
    OSD_VERIFY_SCALER(u4Scaler);

    prParam->ui2_func_enable = 0;

    RET_ON_FAIL(OSD_SC_GetScalerInfo(u4Scaler, &u4Enable,
                                     &prParam->ui4_src_w, &prParam->ui4_src_h,
                                     &prParam->ui4_dst_w, &prParam->ui4_dst_h,
                                     &u4Is16Bpp));
    if (u4Is16Bpp)
    {
        prParam->ui1_color_mode = GFX_SCALER_CM_16BPP;
    }
    else
    {
        prParam->ui1_color_mode = GFX_SCALER_CM_32BPP;
    }

    if (u4Enable)
    {
        prParam->ui2_func_enable = GFX_SCALER_FUNC_SCALING;
    }

    RET_ON_FAIL(OSD_SC_GetLpfInfo(u4Scaler, &u4Enable, &ai2LpfParam[0],
                                  &ai2LpfParam[1], &ai2LpfParam[2],
                                  &ai2LpfParam[3], &ai2LpfParam[4]));
    prParam->ui1_filter_cnt = OSD_LPF_PARAM_NUM;
    x_memcpy(prParam->ai2_filter, ai2LpfParam, sizeof(ai2LpfParam));

    if (u4Enable)
    {
        prParam->ui2_func_enable |= GFX_SCALER_FUNC_FILTERING;
    }

    prParam->ui2_flag = (UINT16)GFX_SCALER_FUNC_ENABLE | GFX_SCALER_SRC_SIZE |
                        GFX_SCALER_DST_SIZE | GFX_SCALER_COLOR_MODE |
                        GFX_SCALER_FILTER_AS_DEFAULT |
                        GFX_SCALER_FILTER_AS_SPECIFIED;

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdSetScalerInfo(UINT32 u4Plane, const GFX_ADV_SCALER_PARAM_T* prParam)
{
    UINT32 u4Scaler;
    UINT32 u4SrcW;
    UINT32 u4SrcH;
    UINT32 u4DstW;
    UINT32 u4DstH;
    UINT32 u4Enable;
    UINT32 u4Is16Bpp;
    INT16 ai2LpfParam[5];

    RET_ON_NULL(prParam);

    OSD_VERIFY_PLANE(u4Plane);
    u4Scaler = OSD_PLANE_TO_SCALER(u4Plane);
    OSD_VERIFY_SCALER(u4Scaler);

    // configure scaler
    RET_ON_FAIL(OSD_SC_GetScalerInfo(u4Scaler, &u4Enable, &u4SrcW, &u4SrcH,
                                     &u4DstW, &u4DstH, &u4Is16Bpp));
    (void)u4Is16Bpp;
    if (prParam->ui2_flag & GFX_SCALER_FUNC_ENABLE)
    {
        u4Enable =( prParam->ui2_func_enable & GFX_SCALER_FUNC_SCALING) > 0;
    }
    if (prParam->ui2_flag & GFX_SCALER_SRC_SIZE)
    {
        u4SrcW = prParam->ui4_src_w;
        u4SrcH = prParam->ui4_src_h;
    }
    if (prParam->ui2_flag & GFX_SCALER_DST_SIZE)
    {
        u4DstW = prParam->ui4_dst_w;
        u4DstH = prParam->ui4_dst_h;
    }
    if (OSD_SC_CheckCapability(u4SrcW, u4SrcH, u4DstH) == (INT32)OSD_RET_OK)
    {
        RET_ON_FAIL(OSD_SC_Scale(u4Scaler, u4Enable, u4SrcW, u4SrcH, u4DstW, u4DstH));
    }
    else
    {
        return -(INT32)OSD_RET_INV_ARG;
    }

    // set color format
    if (prParam->ui2_flag & GFX_SCALER_COLOR_MODE)
    {
        // no use after 538x
    }

    // configure low pass filter
    RET_ON_FAIL(OSD_SC_GetLpfInfo(u4Scaler, &u4Enable, &ai2LpfParam[0],
                                  &ai2LpfParam[1], &ai2LpfParam[2],
                                  &ai2LpfParam[3], &ai2LpfParam[4]));
    if (prParam->ui2_flag & GFX_SCALER_FUNC_ENABLE)
    {
        u4Enable = (prParam->ui2_func_enable & GFX_SCALER_FUNC_FILTERING) > 0;
    }

    // just don't care this case
    if ((prParam->ui2_flag & GFX_SCALER_FILTER_AS_DEFAULT) &&
        (prParam->ui2_flag & GFX_SCALER_FILTER_AS_SPECIFIED))
    {
        return (INT32)OSD_RET_OK;
    }

    if (prParam->ui2_flag & GFX_SCALER_FILTER_AS_DEFAULT)
    {
        // update enable and use default coefficient
        return OSD_SC_SetLpf(u4Scaler, u4Enable);
    }
    else if (prParam->ui2_flag & GFX_SCALER_FILTER_AS_SPECIFIED)
    {
        // update enable and use specified coefficient
        if (prParam->ui1_filter_cnt < OSD_LPF_PARAM_NUM)
        {
            return -(INT32)OSD_RET_INV_ARG;
        }
        // copy prParam->ai2_filter to ai2LpfParam for preventing warning
        x_memcpy(ai2LpfParam, prParam->ai2_filter, sizeof(ai2LpfParam));
        return OSD_SC_SetLpfInfo(u4Scaler, u4Enable, ai2LpfParam[0],
                                 ai2LpfParam[1], ai2LpfParam[2],
                                 ai2LpfParam[3], ai2LpfParam[4]);
    }
    else
    {
        // update enable and use previous coefficient
        return OSD_SC_SetLpfInfo(u4Scaler, u4Enable, ai2LpfParam[0],
                                 ai2LpfParam[1], ai2LpfParam[2],
                                 ai2LpfParam[3], ai2LpfParam[4]);
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdGetLpfParamCnt(UINT32 u4Plane, UINT16* pu1Cnt)
{
    RET_ON_NULL(pu1Cnt);

    OSD_VERIFY_PLANE(OSD_PLANE_TO_SCALER(u4Plane));
    *pu1Cnt = OSD_LPF_PARAM_NUM;

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
// Static functions, Getter and Setter
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdGetCommand(DRV_COMP_ID_T* prCompId, DRV_GET_TYPE_T rGetType,
                            VOID* pvGetInfo, SIZE_T* pzGetInfoLen)
{
    UINT32 u4Plane;

    RET_ON_NULL(prCompId);
    u4Plane = (UINT32)prCompId->ui2_id;
    UNUSED(prCompId);

    UNUSED(OSD_Init());

    switch (rGetType)
    {
    case OSD_GET_TYPE_PLANE_CAPS:
        RET_ON_NO_SPACE(pzGetInfoLen, sizeof(GFX_PLANE_CAPS_T));
        RET_ON_NULL_GET(pvGetInfo);
        GET_COMMAND(_OsdGetPlaneCaps(u4Plane, (GFX_PLANE_CAPS_T*)pvGetInfo));

    case OSD_GET_TYPE_PLANE_BLEND_LEVEL:
        RET_ON_NO_SPACE(pzGetInfoLen, sizeof(UINT8));
        RET_ON_NULL_GET(pvGetInfo);
        GET_COMMAND(OSD_PLA_GetBlendLevel(u4Plane, (UINT8*)pvGetInfo));

    case OSD_GET_TYPE_RGNLIST_CREATE:
        RET_ON_NO_SPACE(pzGetInfoLen, sizeof(GFX_HRGNLIST_T));
        RET_ON_NULL_GET(pvGetInfo);
        GET_COMMAND(_OsdCreateRgnList((GFX_HRGNLIST_T*)pvGetInfo));

    case OSD_GET_TYPE_REGION_CREATE:
        RET_ON_NO_SPACE(pzGetInfoLen, sizeof(GFX_REGION_INIT_T));
        RET_ON_NULL_GET(pvGetInfo);
        GET_COMMAND(_OsdCreateRegion((GFX_REGION_INIT_T*)pvGetInfo));

    case OSD_GET_TYPE_REGION_INFO:
        RET_ON_NO_SPACE(pzGetInfoLen, sizeof(GFX_REGION_INFO_T));
        RET_ON_NULL_GET(pvGetInfo);
        GET_COMMAND(_OsdGetRegionInfo((GFX_REGION_INFO_T*)pvGetInfo));

    case OSD_GET_TYPE_BITMAP_CREATE:
        RET_ON_NO_SPACE(pzGetInfoLen, sizeof(GFX_BITMAP_INIT_T));
        RET_ON_NULL_GET(pvGetInfo);
        GET_COMMAND(_OsdCreateBitmap((GFX_BITMAP_INIT_T*)pvGetInfo, FALSE));

    // luis@2007/6/7
    case OSD_GET_TYPE_BITMAP_CREATE_FROM_FBM:
        RET_ON_NO_SPACE(pzGetInfoLen, sizeof(GFX_BITMAP_INIT_T));
        RET_ON_NULL_GET(pvGetInfo);
        GET_COMMAND(_OsdCreateBitmap((GFX_BITMAP_INIT_T*)pvGetInfo, TRUE));

    case OSD_GET_TYPE_BITMAP_LOCK:
        RET_ON_NO_SPACE(pzGetInfoLen, sizeof(GFX_BITMAP_LOCK_INFO_T));
        RET_ON_NULL_GET(pvGetInfo);
        GET_COMMAND(_OsdLockBitmap((GFX_BITMAP_LOCK_INFO_T*)pvGetInfo));

    case OSD_GET_TYPE_BITMAP_PALETTE:
        RET_ON_NO_SPACE(pzGetInfoLen, sizeof(GFX_BITMAP_PALETTE_T));
        RET_ON_NULL_GET(pvGetInfo);
        GET_COMMAND(_OsdGetBitmapPalette((GFX_BITMAP_PALETTE_T*)pvGetInfo));

    case OSD_GET_TYPE_PALETTE_CREATE:
        RET_ON_NO_SPACE(pzGetInfoLen, sizeof(GFX_PALETTE_INIT_T));
        RET_ON_NULL_GET(pvGetInfo);
        GET_COMMAND(_OsdCreatePalette((GFX_PALETTE_INIT_T*)pvGetInfo));

    case OSD_GET_TYPE_PALETTE_ENTRIES:
        RET_ON_NO_SPACE(pzGetInfoLen, sizeof(GFX_PALETTE_ENTRIES_T));
        RET_ON_NULL_GET(pvGetInfo);
        GET_COMMAND(_OsdGetPaletteEntries((GFX_PALETTE_ENTRIES_T*)pvGetInfo));

    case OSD_GET_TYPE_PALETTE_PARAMS:
        RET_ON_NO_SPACE(pzGetInfoLen, sizeof(GFX_PALETTE_PARAMS_T));
        RET_ON_NULL_GET(pvGetInfo);
        GET_COMMAND(_OsdGetPaletteParams((GFX_PALETTE_PARAMS_T*)pvGetInfo));

    case OSD_GET_TYPE_PLANE_ADV_SCALER:
        RET_ON_NO_SPACE(pzGetInfoLen, sizeof(GFX_ADV_SCALER_PARAM_T));
        RET_ON_NULL_GET(pvGetInfo);
        GET_COMMAND(_OsdGetScalerInfo(u4Plane,
                    (GFX_ADV_SCALER_PARAM_T*)pvGetInfo));

    case OSD_GET_TYPE_PLANE_ADV_SCALER_FILTER_CNT:
        RET_ON_NO_SPACE(pzGetInfoLen, sizeof(UINT16));
        RET_ON_NULL_GET(pvGetInfo);
        GET_COMMAND(_OsdGetLpfParamCnt(u4Plane, (UINT16*)pvGetInfo));

    case OSD_GET_TYPE_PLANE_FADING_RATIO:
        RET_ON_NO_SPACE(pzGetInfoLen, sizeof(UINT8));
        RET_ON_NULL_GET(pvGetInfo);
        GET_COMMAND(OSD_PLA_GetFading(u4Plane, (UINT8*)pvGetInfo));

    case OSD_GET_TYPE_PLANE_POS:
        RET_ON_NO_SPACE(pzGetInfoLen, sizeof(GFX_POS_T));
        RET_ON_NULL_GET(pvGetInfo);
        GET_COMMAND(OSD_BASE_GetOsdPosition(u4Plane,
                    (UINT32*)&((GFX_POS_T*)pvGetInfo)->i4_x,
                    (UINT32*)&((GFX_POS_T*)pvGetInfo)->i4_y));

    default:
        return RMR_DRV_INV_GET_INFO;
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdSetCommand(DRV_COMP_ID_T* prCompId, DRV_SET_TYPE_T rSetType,
                            const VOID* pvSetInfo, SIZE_T zSetInfoLen)
{
    UINT32 u4Plane;

    RET_ON_NULL(prCompId);
    u4Plane = (UINT32)prCompId->ui2_id;
    UNUSED(prCompId);

    UNUSED(OSD_Init());

    if ((rSetType != OSD_SET_TYPE_PLANE_FLIP) && (zSetInfoLen == 0))
    {
        return RMR_DRV_INV_SET_INFO;
    }

    switch (rSetType)
    {
    case OSD_SET_TYPE_PLANE_ENABLE:
        SET_COMMAND(OSD_PLA_Enable(u4Plane, (INT32)pvSetInfo));

    case OSD_SET_TYPE_PLANE_FLIP:
        SET_COMMAND(_OsdFlipPlane(u4Plane, (GFX_HRGNLIST_T)pvSetInfo));

    case OSD_SET_TYPE_PLANE_BLEND_LEVEL:
        SET_COMMAND(OSD_PLA_SetBlendLevel(u4Plane, (UINT32)pvSetInfo));

    case OSD_SET_TYPE_RGNLIST_DELETE:
        RET_ON_NULL_SET(pvSetInfo);
        SET_COMMAND(OSD_RGN_LIST_Delete(GET_OSDDRV_HANDLE(pvSetInfo)));

    case OSD_SET_TYPE_RGNLIST_DETACH_ALL:
        RET_ON_NULL_SET(pvSetInfo);
        SET_COMMAND(OSD_RGN_LIST_DetachAll(GET_OSDDRV_HANDLE(pvSetInfo)));

    case OSD_SET_TYPE_REGION_DELETE:
        RET_ON_NULL_SET(pvSetInfo);
        SET_COMMAND(OSD_RGN_Delete(GET_OSDDRV_HANDLE(pvSetInfo)));

    case OSD_SET_TYPE_REGION_ATTACH:
        SET_COMMAND(_OsdAttachRegion((GFX_REGION_POS_T*)pvSetInfo));

    case OSD_SET_TYPE_REGION_DETACH:
        SET_COMMAND(_OsdDetachRegion((GFX_REGION_DETACH_T*)pvSetInfo));

    case OSD_SET_TYPE_REGION_ATTR:
        SET_COMMAND(_OsdSetRegionAttr((GFX_REGION_ATTR_T*)pvSetInfo));

    case OSD_SET_TYPE_BITMAP_DELETE:
        SET_COMMAND(_OsdDeleteBitmap((GFX_HBITMAP_T)pvSetInfo, FALSE));

    case OSD_SET_TYPE_BITMAP_DELETE_FROM_FBM:
        SET_COMMAND(_OsdDeleteBitmap((GFX_HBITMAP_T)pvSetInfo, TRUE));

    case OSD_SET_TYPE_BITMAP_UNLOCK:
        SET_COMMAND(_OsdUnlockBitmap((GFX_HBITMAP_T)pvSetInfo));

    case OSD_SET_TYPE_BITMAP_PALETTE:
        SET_COMMAND(_OsdSetBitmapPalette((GFX_BITMAP_PALETTE_T*)pvSetInfo));

    case OSD_SET_TYPE_PALETTE_DELETE:
        SET_COMMAND(_OsdDeletePalette((GFX_HPALETTE_T)pvSetInfo));

    case OSD_SET_TYPE_PALETTE_ENTRIES:
        SET_COMMAND(_OsdSetPaletteEntries((GFX_PALETTE_ENTRIES_T*)pvSetInfo));

    case OSD_SET_TYPE_PALETTE_PARAMS:
        SET_COMMAND(_OsdSetPaletteParams((GFX_PALETTE_PARAMS_T*)pvSetInfo));

    case OSD_SET_TYPE_PLANE_ADV_SCALER:
        RET_ON_NULL_SET(pvSetInfo);
        SET_COMMAND(_OsdSetScalerInfo(u4Plane,
                    (GFX_ADV_SCALER_PARAM_T*)pvSetInfo));

    case OSD_SET_TYPE_PLANE_FADING_RATIO:
        SET_COMMAND(OSD_PLA_SetFading(u4Plane, (UINT32)pvSetInfo));

    case OSD_SET_TYPE_PLANE_POS:
        RET_ON_NULL_SET(pvSetInfo);
        SET_COMMAND(OSD_BASE_SetOsdPosition(u4Plane,
                    (UINT32)((GFX_POS_T*)pvSetInfo)->i4_x,
                    (UINT32)((GFX_POS_T*)pvSetInfo)->i4_y));

    default:
        return RMR_DRV_INV_SET_INFO;
    }
}


//-----------------------------------------------------------------------------
// public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
INT32 MW_OSD_Init(void)
{
    UINT32 i;
    DRV_COMP_FCT_TBL_T rFctTbl;
    DRV_COMP_REG_T rCompReg;
    //CHAR szDriverName[128];
    INT32 i4Ret;

    if (_fgInitOK)
    {
        return 0;
    }

    if (!DRVCUST_InitGet(eLoaderLogoNumber))
    {
        if (OSD_Init() != (INT32)OSD_RET_OK)
        {
            return -1;
        }
    }

    // register driver components
    rFctTbl.pf_rm_connect = NULL;
    rFctTbl.pf_rm_disconnect = NULL;
    rFctTbl.pf_rm_get = _OsdGetCommand;
    rFctTbl.pf_rm_set = _OsdSetCommand;

    // register OSD plane driver component
    for (i = 0; i < (INT32)OSD_PLANE_MAX_NUM; i++)
    {
        rCompReg.e_type = DRVT_OSD_PLANE;
        rCompReg.e_id_type = ID_TYPE_IND;
        rCompReg.u.t_ind.ui2_id = (UINT16)i;
        rCompReg.u.t_ind.pv_tag = NULL;
        rCompReg.u.t_ind.ui1_port = ALL_PORTS;

        i4Ret = x_rm_reg_comp(&rCompReg, 1, 1, NULL,
                              DRV_FLAG_SINGLE_CONN_ON_OUTPUT,
                              &rFctTbl, NULL, 0);
        if (i4Ret != RMR_OK)
        {
            return i4Ret;
        }
    }

    // register connection list
    //_OsdMakeConnList();

    _OsdInitFbmBuffer();

    _fgInitOK = TRUE;
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
void MW_OsdConnect(INT32 hOsdPlane, INT32 fgEnable)
{
	IGNORE_RET(OSD_PLA_SetGlobeEnable((UINT32)hOsdPlane, fgEnable));
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
void OSD_Uninit(void)
{
    if (_fgInitOK)
    {
        _fgInitOK = FALSE;
    }
}

