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
 * $RCSfile: drv_if_osd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd.c
 *  Driver interface: OSD part implementation
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "osd_if.h"
#include "pmx_if.h"
#include "x_drv_if.h"
#include "x_assert.h"

#define DEFINE_IS_LOG OSD_IsLog
#include "x_debug.h"


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define RET_ON_FAIL(FUNC)                                                      \
    do                                                                         \
    {                                                                          \
        if ((FUNC) != OSD_RET_OK)                                              \
        {                                                                      \
            return DRV_E_FAIL;                                                 \
        }                                                                      \
    } while (0)

#define RET_ON_NULL(VAR)                                                       \
    do                                                                         \
    {                                                                          \
        if ((VAR) == NULL)                                                     \
        {                                                                      \
            return DRV_E_FAIL;                                                 \
        }                                                                      \
    } while (0)


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static UINT32 _u4List[2];


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief :
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static OSD_COLOR_MODE_T _OsdGetColorMode(DRV_COLOR_MODE_T rColorMode)
{
    switch (rColorMode)
    {
        case DRV_CM_YBR_2: return OSD_CM_YCBCR_CLUT2;
        case DRV_CM_YBR_4: return OSD_CM_YCBCR_CLUT4;
        case DRV_CM_YBR_8: return OSD_CM_YCBCR_CLUT8;
        case DRV_CM_BRY_422: return OSD_CM_CBYCRY422_DIRECT16;
        case DRV_CM_YBR_422: return OSD_CM_YCBYCR422_DIRECT16;
        case DRV_CM_AYBR_8888: return OSD_CM_AYCBCR8888_DIRECT32;
        case DRV_CM_RGB_2: return OSD_CM_RGB_CLUT2;
        case DRV_CM_RGB_4: return OSD_CM_RGB_CLUT4;
        case DRV_CM_RGB_8: return OSD_CM_RGB_CLUT8;
        case DRV_CM_RGB_565: return OSD_CM_RGB565_DIRECT16;
        case DRV_CM_ARGB_1555: return OSD_CM_ARGB1555_DIRECT16;
        case DRV_CM_ARGB_4444: return OSD_CM_ARGB4444_DIRECT16;
        case DRV_CM_ARGB_8888: return OSD_CM_ARGB8888_DIRECT32;
        default: ASSERT(0);
    }
    return OSD_CM_ARGB8888_DIRECT32;
}

static BOOL _OsdIsPaletteMode(DRV_COLOR_MODE_T rColorMode)
{
    switch (rColorMode)
    {
        case DRV_CM_YBR_2:
        case DRV_CM_YBR_4:
        case DRV_CM_YBR_8:
        case DRV_CM_RGB_2:
        case DRV_CM_RGB_4:
        case DRV_CM_RGB_8:
            return TRUE;
        default:
            return FALSE;
    }
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief : Init OSD hardware and create a region
 *  @param
  *  @return
 */
//-----------------------------------------------------------------------------

DRV_ERR_CODE_T DRV_OSD_Init(void)
{
    RET_ON_FAIL(OSD_Init());
    RET_ON_FAIL(OSD_RGN_LIST_Create(&_u4List[DRV_OSD_PLANE_1]));
    RET_ON_FAIL(OSD_RGN_LIST_Create(&_u4List[DRV_OSD_PLANE_2]));

    return DRV_E_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

DRV_ERR_CODE_T DRV_OSD_Enable(UINT8 u1PlaID, BOOL fgEnable)
{
    RET_ON_FAIL(OSD_PLA_Enable(u1PlaID, fgEnable));

    return DRV_E_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

DRV_ERR_CODE_T DRV_OSD_RGN_Create(DRV_OSD_RGN_T* prOsd, UINT32* pu4RgnID)
{
    UINT32 u4Region;
    OSD_COLOR_MODE_T rColorMode;

    RET_ON_NULL(prOsd);

    rColorMode = _OsdGetColorMode(prOsd->rColorMode);
    RET_ON_FAIL(OSD_RGN_Create(&u4Region, prOsd->u4Width, prOsd->u4Height,
                               (void*)prOsd->pu1FrameBaseAddr,
                               (UINT32)rColorMode, prOsd->u4Pitch,
                               prOsd->u4Left, prOsd->u4Top,
                               prOsd->u4Width, prOsd->u4Height));

    RET_ON_FAIL(OSD_RGN_Insert(u4Region, _u4List[prOsd->u1PlaID]));
    RET_ON_FAIL(OSD_RGN_Set(u4Region, OSD_RGN_MIX_SEL, OSD_BM_REGION));

    if(_OsdIsPaletteMode(prOsd->rColorMode))
    {
        VERIFY(prOsd->pu4Palette != NULL);
        RET_ON_FAIL(OSD_RGN_Set(u4Region, OSD_RGN_PAL_ADDR,
                                (UINT32)prOsd->pu4Palette));
    }

    if(prOsd->u4ScaleWidth != 0 && prOsd->u4ScaleHeight != 0)
    {
        RET_ON_FAIL(OSD_RGN_Set(u4Region, OSD_RGN_DISP_W, prOsd->u4Width));
        RET_ON_FAIL(OSD_RGN_Set(u4Region, OSD_RGN_DISP_H, prOsd->u4Height));
    }

    RET_ON_FAIL(OSD_PLA_FlipTo(prOsd->u1PlaID, _u4List[prOsd->u1PlaID]));

    *pu4RgnID = u4Region;

    return DRV_E_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

DRV_ERR_CODE_T DRV_OSD_RGN_Delete(UINT8 u1PlaID, UINT32 u4RgnID)
{
    RET_ON_FAIL(OSD_RGN_Detach(u4RgnID, _u4List[u1PlaID]));
    RET_ON_FAIL(OSD_RGN_Delete(u4RgnID));
    return DRV_E_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

DRV_ERR_CODE_T DRV_OSD_SetPlaneAlpha(UINT8 u1PlaID, UINT8 u1Alpha)
{
    RET_ON_FAIL(OSD_PLA_SetBlendLevel(u1PlaID, u1Alpha));
    return DRV_E_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

DRV_ERR_CODE_T DRV_OSD_SetRegionAlpha(UINT32 u4RgnID, UINT8 u1Alpha)
{
    RET_ON_FAIL(OSD_RGN_Set(u4RgnID, OSD_RGN_ALPHA, u1Alpha));
    return DRV_E_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

DRV_ERR_CODE_T DRV_OSD_SetColorkey(UINT32 u4RgnID, BOOL fgEnable,
                                   UINT32 u4Color)
{
    RET_ON_FAIL(OSD_RGN_Set(u4RgnID, OSD_RGN_COLOR_KEY_EN, fgEnable));
    RET_ON_FAIL(OSD_RGN_Set(u4RgnID, OSD_RGN_COLOR_KEY, u4Color));
    return DRV_E_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

DRV_ERR_CODE_T DRV_OSD_SetPlaneOrder(UINT8 u1OrderType)
{
/*
    if (u1OrderType == 1)
    {
        RET_ON_FAIL(OSD_BASE_SetTopPlane(FALSE));
    }
    else if (u1OrderType == 2)
    {
        RET_ON_FAIL(OSD_BASE_SetTopPlane(TRUE));
    }
    else
    {
        return DRV_E_INV_ARG;
    }
*/
    return DRV_E_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

DRV_OSD_RGN_SetAlphaMode(UINT32 u4RgnID, DRV_OSD_BLEND_MODE_T eMode)
{
    if (eMode == DRV_OSD_BM_PIXEL)
    {
        RET_ON_FAIL(OSD_RGN_Set(u4RgnID, OSD_RGN_MIX_SEL, OSD_BM_PIXEL));
    }
    else if (eMode == DRV_OSD_BM_PLANE)
    {
        RET_ON_FAIL(OSD_RGN_Set(u4RgnID, OSD_RGN_MIX_SEL, OSD_BM_PLANE));
    }
    else
    {
        return DRV_E_INV_ARG;
    }
    return DRV_E_OK;
}
