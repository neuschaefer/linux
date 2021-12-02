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
 * $RCSfile: osd_plane_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_plane_if.c
 *  osd plane header, incluse osd plane setting
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "osd_hw.h"
#include "osd_drvif.h"
#include "osd_debug.h"
#include "drvcust_if.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_5381.h"
LINT_EXT_HEADER_END

#ifdef CC_MTK_LOADER
#define CRIT_STATE_T    UINT32
#ifdef x_crit_start
#undef x_crit_start
#define x_crit_start()  (0)
#endif
#ifdef x_crit_end
#undef x_crit_end
#define x_crit_end(x)   UNUSED(x)
#endif
#endif /* CC_MTK_LOADER */

#ifndef CC_MTK_LOADER
#ifdef SYS_3D_SUPPORT
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "gfx_if.h"
#endif
#endif

#ifndef CC_MTK_LOADER
#ifdef CC_CONFIG_OSD_SBS
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "gfx_if.h"
#endif
#endif


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// delay to avoid temporal status when switching display mode
#define MUTE_VSYNC 10

//define the DRAM access configuration for OSD plane 3
#define OSD3_ContReqLmt 3
#define OSD3_FifoSize 3
#define OSD3_VacTh 3
#define OSD3_PauseCnt 0xf
#define OSD3_ContReqLmt0 1
#define OSD3_BurstDis 0

#ifndef CC_MTK_LOADER
#ifdef SYS_3D_SUPPORT
#define UI_WIDTH    960
#define UI_HEIGHT    540
#define UI_BPP    4
#endif
#endif

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct _OSD_PLANE_INFO_T
{
    UINT32 u4FirstRegionAddr;
    INT32 fgPlaneEnable;
    INT32 fgGlobeEnable;
    INT32 i4FlipedList;
    INT32 i4FirstRegionId;
} OSD_PLANE_INFO_T;


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static OSD_PLANE_INFO_T _arOsdPlaneInfo[OSD_PLANE_MAX_NUM];
static UINT32 _u4MuteCounter = 0;
static UINT8 _u1FadingCache[OSD_PLANE_MAX_NUM];
static BOOL _fgInit = FALSE;
//#ifdef CC_MT5363
//static BOOL gfgPLASwap=FALSE;
//#endif

#if 0
//For 3D OSD
UINT8 _u13DMode=0;
UINT8 _u1Previous3DMode=0;
UINT32 _u4_3D_Plane;
UINT32 _u4_3D_Region;
UINT32 _au4_3D_L_Addr;
UINT32 _au4_3D_R_Addr;
//UINT32 _au4_3D_L_Addr[2];
//UINT32 _au4_3D_R_Addr[2];
#endif

#ifndef CC_MTK_LOADER
#ifdef SYS_3D_SUPPORT
static UINT32 _u43DPlane = 0;
static UINT32 _e3DMode = 0;
static UINT32 _u4Shift = 0;
static UINT32 _u4BmpWidth = 0;
static UINT32 _u4BmpHeight = 0;
static UINT32 _u4BmpPitch = 0;
static UINT32 _u4DstAddr = 0;
static UINT32 _eColorMode = 0;
static UINT32 _u4SrcAddr = 0;
static UINT32 _u4TmpBuf = 0;
static UINT32 _u4TmpBuf1 = 0;
#endif
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static BOOL _OsdPlaReadyToDisplay(UINT32 u4Plane)
{
    return (_arOsdPlaneInfo[u4Plane].fgGlobeEnable &&
            _arOsdPlaneInfo[u4Plane].u4FirstRegionAddr &&
            _arOsdPlaneInfo[u4Plane].fgPlaneEnable &&
            (OSD_DMA_GetMode() == OSD_OP_NORMAL));
}

static void _UpdatePlaneEnableState(UINT32 u4Plane)
{
    UINT32 u4Value;
    IGNORE_RET(_OSD_PLA_GetEnable(u4Plane, &u4Value));

    if (u4Value == 0)
    {
        // Previously disabled but may need to enable because some condition
        // was satisfied now. For example, the plane flip to a valid region
        if (_OsdPlaReadyToDisplay(u4Plane))
        {
            // all flags on
            IGNORE_RET(_OSD_PLA_SetEnable(u4Plane, (UINT32)TRUE));
        }
    }
    else
    {
        // This function will check global enable(MW), driver enable and
        // valid first region. If one of the three condition is not satisfied,
        // we will force to disable it even previous enable
        if (!_OsdPlaReadyToDisplay(u4Plane))
        {
            // not "all flags on"
            IGNORE_RET(_OSD_PLA_SetEnable(u4Plane, (UINT32)FALSE));
        }
    }
    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(u4Plane));
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void OSD_PLA_Mute(void)
{
    UINT32 u4Plane;
    CRIT_STATE_T rCritState;
    if(_fgInit == TRUE)
    {
        if (_u4MuteCounter == 0)
        {
            _OSD_AlwaysUpdateReg(TRUE);
            for (u4Plane = 0; u4Plane < (INT32)OSD_PLANE_MAX_NUM; u4Plane++)
            {
                IGNORE_RET(OSD_PLA_GetFading(u4Plane, &_u1FadingCache[u4Plane]));
                IGNORE_RET(OSD_PLA_SetFading(u4Plane, 0));
                //IGNORE_RET(OSD_PLA_SetGlobeEnable(u4Plane, FALSE));
            }
            _OSD_AlwaysUpdateReg(FALSE);
        }
        
        rCritState = x_crit_start();
        _u4MuteCounter = MUTE_VSYNC;
        x_crit_end(rCritState);
    }
    LOG(5, "OSD_PLA_Mute\n");
}


void OSD_PLA_Unmute(void)
{
    UINT32 u4Plane;
    CRIT_STATE_T rCritState;

    if (_u4MuteCounter > 0)
    {
        if (_u4MuteCounter == 1)
        {
            for (u4Plane = 0; u4Plane < (INT32)OSD_PLANE_MAX_NUM; u4Plane++)
            {
                IGNORE_RET(OSD_PLA_SetFading(u4Plane, _u1FadingCache[u4Plane]));
                //IGNORE_RET(OSD_PLA_SetGlobeEnable(u4Plane, TRUE));
            }
            LOG(5, "OSD_PLA_Unmute\n");
        }
        rCritState = x_crit_start();
        _u4MuteCounter--;
        x_crit_end(rCritState);
    }
}


INT32 OSD_PLA_SetGlobeEnable(UINT32 u4Plane, BOOL fgGlobeEnable)
{
    OSD_VERIFY_PLANE(u4Plane);
    //#ifdef CC_MT5363
    #if 0
    if(gfgPLASwap)
    {
    	if(u4Plane==OSD_PLANE_2)
    		u4Plane=OSD_PLANE_3;
    	else if(u4Plane==OSD_PLANE_3)
    		u4Plane=OSD_PLANE_2;
    }
    #endif
    _arOsdPlaneInfo[u4Plane].fgGlobeEnable = fgGlobeEnable;
    _UpdatePlaneEnableState(u4Plane);

    return (INT32)OSD_RET_OK;
}


INT32 OSD_PLA_Init(UINT32 u4Plane)
{
    OSD_VERIFY_PLANE(u4Plane);
    IGNORE_RET(OSD_PLA_Reset(u4Plane));

    return (INT32)OSD_RET_OK;
}


INT32 OSD_PLA_Reset(UINT32 u4Plane)
{
    OSD_VERIFY_PLANE(u4Plane);

    //#ifdef CC_MT5363
    #if 0
    if(gfgPLASwap)
    {
    	if(u4Plane==OSD_PLANE_2)
    		u4Plane=OSD_PLANE_3;
    	else if(u4Plane==OSD_PLANE_3)
    		u4Plane=OSD_PLANE_2;
    }
    #endif

    // reset plane's hardware state
    IGNORE_RET(_OSD_PLA_SetReg(u4Plane, NULL));
    IGNORE_RET(_OSD_PLA_SetEnable(u4Plane, 0));
    IGNORE_RET(_OSD_PLA_SetHFilter(u4Plane, 1));
   // IGNORE_RET(_OSD_PLA_SetVFilter(u4Plane, 1));
#ifdef ANDROID
    IGNORE_RET(_OSD_PLA_SetBlending(u4Plane, 0x80));
    IGNORE_RET(_OSD_PLA_SetFading(u4Plane, 0x80));
#else   
    IGNORE_RET(_OSD_PLA_SetBlending(u4Plane, 0xFF));
    IGNORE_RET(_OSD_PLA_SetFading(u4Plane, 0xFF));
#endif    
    // reset plane's software state
    x_memset((void*)&_arOsdPlaneInfo[u4Plane], 0, sizeof(OSD_PLANE_INFO_T));
    
    //set i4FirstRegionId to -1 at initial state
    _arOsdPlaneInfo[u4Plane].i4FirstRegionId=(-1);


    //#ifdef CC_MT5363
    #if 0
    if(gfgPLASwap)
    {
    	if(u4Plane==OSD_PLANE_2)
    		u4Plane=OSD_PLANE_3;
    	else if(u4Plane==OSD_PLANE_3)
    		u4Plane=OSD_PLANE_2;
    }
    #endif

    IGNORE_RET(OSD_PLA_SetGlobeEnable(u4Plane, (INT32)TRUE));
    // global enable is set to true by default
    //_UpdatePlaneEnableState(u4Plane);

    _fgInit = TRUE;
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** enable/disable osd plane, it will also disable scaler when disable plane
 *  @param  u4Plane plane id
 *  @param  fgEnable enable=TRUE/disable=FALSE
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_PLA_Enable(UINT32 u4Plane, BOOL fgEnable)
{
    OSD_VERIFY_PLANE(u4Plane);

    //#ifdef CC_MT5363
    #if 0
    if(gfgPLASwap)
    {
    	if(u4Plane==OSD_PLANE_2)
    		u4Plane=OSD_PLANE_3;
    	else if(u4Plane==OSD_PLANE_3)
    		u4Plane=OSD_PLANE_2;
    }
    #endif
    IGNORE_RET(_OSD_PLA_SetEnable(u4Plane, fgEnable));
    _arOsdPlaneInfo[u4Plane].fgPlaneEnable = fgEnable;
    _UpdatePlaneEnableState(u4Plane);

    // we suppose no abnormal case when using MJC, so don't care about
    // _UpdatePlaneEnableState now.
#if defined(DRV_SUPPORT_EXTMJC) && !defined(CC_MTK_LOADER)
    if (IS_COMPANION_CHIP_ON() && _OsdPlaReadyToDisplay(u4Plane))
    {
        LOG(5, "drv_extmjc_set_osd_enable, fgEnable=%d\n", fgEnable);
        drv_extmjc_set_osd_turn_on();
    }
#endif
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** attach a specific list to osd plane
 *  @param  u4Plane plane id
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_PLA_FlipTo(UINT32 u4Plane, UINT32 u4List)
{
    INT32 i4FirstRegion;
    UINT32 u4FirstRegionAddr;

    OSD_VERIFY_PLANE(u4Plane);
    OSD_VERIFY_RGNLIST(u4List);

    //#ifdef CC_MT5363
    #if 0
    if(gfgPLASwap)
    {
    	if(u4Plane==OSD_PLANE_2)
    		u4Plane=OSD_PLANE_3;
    	else if(u4Plane==OSD_PLANE_3)
    		u4Plane=OSD_PLANE_2;
    }
    #endif
    
    VERIFY((INT32)OSD_RET_OK == OSD_RGN_LIST_GetHead(u4List, &i4FirstRegion));
    OSD_VERIFY_REGION(i4FirstRegion);

    _arOsdPlaneInfo[u4Plane].i4FirstRegionId = i4FirstRegion;

    VERIFY((INT32)OSD_RET_OK ==
           _OSD_RGN_GetAddress((UINT32)i4FirstRegion, &u4FirstRegionAddr));

    if (u4Plane == OSD_PLANE_1)
    {
        x_memcpy((void*)OSD_PLA1_RGN_REG, (void*)u4FirstRegionAddr, sizeof(OSD_RGN_UNION_T));
    }

    VERIFY((INT32)OSD_RET_OK ==
           _OSD_PLA_SetHeaderAddr(u4Plane, u4FirstRegionAddr));

    OSD_WaitVsync();
    
    _arOsdPlaneInfo[u4Plane].u4FirstRegionAddr = u4FirstRegionAddr;
    _UpdatePlaneEnableState(u4Plane);

    _arOsdPlaneInfo[u4Plane].i4FlipedList = (INT32)u4List;

    return (INT32)OSD_RET_OK;
}


INT32 OSD_PLA_FlipToNone(UINT32 u4Plane)
{
    OSD_VERIFY_PLANE(u4Plane);
    //#ifdef CC_MT5363
    #if 0
    if(gfgPLASwap)
    {
    	if(u4Plane==OSD_PLANE_2)
    		u4Plane=OSD_PLANE_3;
    	else if(u4Plane==OSD_PLANE_3)
    		u4Plane=OSD_PLANE_2;
    }
    #endif
    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_SetHeaderAddr(u4Plane, 0));
    _arOsdPlaneInfo[u4Plane].u4FirstRegionAddr = 0;
    _arOsdPlaneInfo[u4Plane].i4FirstRegionId = -1;
	OSD_WaitVsync();
    _UpdatePlaneEnableState(u4Plane);

    return (INT32)OSD_RET_OK;
}


INT32 OSD_PLA_Reflip(UINT32 u4List)
{
    INT32 i4Index;
    UINT32 u4Value;

    for (i4Index = 0; i4Index < (INT32)OSD_PLANE_MAX_NUM; i4Index++)
    {
        if (u4List == (UINT32)_arOsdPlaneInfo[i4Index].i4FlipedList)
        {
            IGNORE_RET(_OSD_PLA_GetEnable((UINT32)i4Index, &u4Value));
            if (u4Value)
            {
                IGNORE_RET(OSD_PLA_FlipTo((UINT32)i4Index, u4List));
            }
        }
    }

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** get osd plane alpha level
 *  @param  u4Plane plane id
 *  @param  pu1BlendLevel aplha level pointer
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_PLA_GetBlendLevel(UINT32 u4Plane, UINT8* pu1BlendLevel)
{
    UINT32 u4Blending;

    if (pu1BlendLevel == NULL)
    {
        return -(INT32)OSD_RET_INV_ARG;
    }
    OSD_VERIFY_PLANE(u4Plane);
    //#ifdef CC_MT5363
    #if 0
    if(gfgPLASwap)
    {
    	if(u4Plane==OSD_PLANE_2)
    		u4Plane=OSD_PLANE_3;
    	else if(u4Plane==OSD_PLANE_3)
    		u4Plane=OSD_PLANE_2;
    }
    #endif
    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_GetBlending(u4Plane, &u4Blending));
    *pu1BlendLevel = (UINT8) u4Blending;

    return (INT32)OSD_RET_OK;
}


INT32 OSD_PLA_GetEnable(UINT32 u4Plane, BOOL* pfgEnable)
{
    UINT32 u4Enable;

    if (pfgEnable == NULL)
    {
        return -(INT32)OSD_RET_INV_ARG;
    }
    OSD_VERIFY_PLANE(u4Plane);
    //#ifdef CC_MT5363
    #if 0
    if(gfgPLASwap)
    {
    	if(u4Plane==OSD_PLANE_2)
    		u4Plane=OSD_PLANE_3;
    	else if(u4Plane==OSD_PLANE_3)
    		u4Plane=OSD_PLANE_2;
    }
    #endif
    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_GetEnable(u4Plane, &u4Enable));
    *pfgEnable = (BOOL) u4Enable;

    return (INT32)OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** set osd plane alpha level
 *  @param  u4Plane plane id
 *  @param  u1BlendLevel aplha level(0~255)
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_PLA_SetBlendLevel(UINT32 u4Plane, UINT8 u1BlendLevel)
{
    OSD_VERIFY_PLANE(u4Plane);
    //#ifdef CC_MT5363
    #if 0
    if(gfgPLASwap)
    {
    	if(u4Plane==OSD_PLANE_2)
    		u4Plane=OSD_PLANE_3;
    	else if(u4Plane==OSD_PLANE_3)
    		u4Plane=OSD_PLANE_2;
    }
    #endif
    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_SetBlending(u4Plane, u1BlendLevel));
    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(u4Plane));
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** get osd plane fading value
 *  @param  u4Plane plane id
 *  @param  pu1Fading fading value pointer
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_PLA_GetFading(UINT32 u4Plane, UINT8* pu1Fading)
{
    UINT32 u4Fading;

    if (pu1Fading == NULL)
    {
        return -(INT32)OSD_RET_INV_ARG;
    }
    OSD_VERIFY_PLANE(u4Plane);
    //#ifdef CC_MT5363
    #if 0
    if(gfgPLASwap)
    {
    	if(u4Plane==OSD_PLANE_2)
    		u4Plane=OSD_PLANE_3;
    	else if(u4Plane==OSD_PLANE_3)
    		u4Plane=OSD_PLANE_2;
    }
    #endif
    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_GetFading(u4Plane, &u4Fading));
    *pu1Fading = (UINT8) u4Fading;

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** set osd plane fading value
 *  @param  u4Plane plane id
 *  @param  u1Fading fading value
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_PLA_SetFading(UINT32 u4Plane, UINT8 u1Fading)
{
    OSD_VERIFY_PLANE(u4Plane);
    //#ifdef CC_MT5363
    #if 0
    if(gfgPLASwap)
    {
    	if(u4Plane==OSD_PLANE_2)
    		u4Plane=OSD_PLANE_3;
    	else if(u4Plane==OSD_PLANE_3)
    		u4Plane=OSD_PLANE_2;
    }
    #endif
    if (_u4MuteCounter > 1)	
    {
        _u1FadingCache[u4Plane] = u1Fading;
    }
    else
    {
    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_SetFading(u4Plane, u1Fading));
    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(u4Plane));
    }
    return (INT32)OSD_RET_OK;
}


INT32 OSD_PLA_SetHFilter(UINT32 u4Plane, BOOL fgEnable)
{
    OSD_VERIFY_PLANE(u4Plane);
    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_SetHFilter(u4Plane, fgEnable));
    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(u4Plane));
    return (INT32)OSD_RET_OK;
}

INT32 OSD_PLA_SetVFilter(UINT32 u4Plane, BOOL fgEnable)
{
/*
    OSD_VERIFY_PLANE(u4Plane);
    //VERIFY((INT32)OSD_RET_OK == _OSD_PLA_SetVFilter(u4Plane, fgEnable));
    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(u4Plane));
    */
    return (INT32)OSD_RET_OK;
}

INT32 OSD_PLA_SetFifo(UINT32 u4Plane, BOOL fgFastReq, UINT8 u1ExVacThr,
                      UINT8 u1VacThr, UINT8 u1FullThr)
{
    UNUSED(u4Plane);
    UNUSED(fgFastReq);
    UNUSED(u1ExVacThr);
    UNUSED(u1VacThr);
    UNUSED(u1FullThr);
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** dump osd debug info
 *  @param  u4Plane osd plane
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_PLA_Dump(UINT32 u4Plane)
{
//#ifdef CC_DEBUG
    OSD_PLA_UNION_T rOsdPlaneReg;
    UINT32 u4Region, u4List;
    INT32 i4Count;

    OSD_VERIFY_PLANE(u4Plane);

    // dump plane
    IGNORE_RET(_OSD_PLA_GetReg(u4Plane, rOsdPlaneReg.au4Reg));
    Printf("OSD PLANE[%d]\n", u4Plane + 1);
    Printf("\tEnable=%d, FirstRegion=0x%08x\n",
           rOsdPlaneReg.rField.fgEn, rOsdPlaneReg.rField.u4HeaderAddr);
    if (!rOsdPlaneReg.rField.u4HeaderAddr)
    {
        return (INT32)OSD_RET_OK;
    }
    /*
    Printf("\tHFilter=%d, VFilter, Fading=%d, Alpha=%d\n",
           rOsdPlaneReg.rField.fgHFilter, rOsdPlaneReg.rField.fgVFilter,
           rOsdPlaneReg.rField.u4Fading, rOsdPlaneReg.rField.u4Blending);
     */
     Printf("\tHFilter=%d, Fading=%d, Alpha=%d\n",
           rOsdPlaneReg.rField.fgHFilter,
           rOsdPlaneReg.rField.u4Fading, rOsdPlaneReg.rField.u4Blending);
    /*
    Printf("\tVacTh4=%d, VacTh16=%d, FifoSize=%d, ContReqLmt=%d\n",
           rOsdPlaneReg.rField.fgVacTh4, rOsdPlaneReg.rField.u4VacTh16,
           rOsdPlaneReg.rField.u4FifoSize, rOsdPlaneReg.rField.u4ContReqLmt);
    */
    //Printf("\tDecEn=%d, AlphaMod=%d\n",
    //       rOsdPlaneReg.rField.fgDecEn, rOsdPlaneReg.rField.fgAlphaMod);

    // dump scaler
    if (u4Plane != (UINT32)OSD_PLANE_1)
    {
        IGNORE_RET(OSD_SC_Dump(OSD_PLANE_TO_SCALER(u4Plane)));
    }

    // dump region
    if (_arOsdPlaneInfo[u4Plane].u4FirstRegionAddr != NULL)
    {
        u4List = (UINT32)_arOsdPlaneInfo[u4Plane].i4FlipedList;
        IGNORE_RET(OSD_RGN_LIST_Get(u4List, (INT32)OSD_RGN_LIST_COUNT,
                                   (UINT32*)&i4Count));
        IGNORE_RET(OSD_RGN_LIST_Get(u4List, (INT32)OSD_RGN_LIST_HEAD,
                                    &u4Region));
        while (i4Count--)
        {
            UINT32 u4Next;
            IGNORE_RET(OSD_RGN_Dump(u4Region));
            IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_NEXT, &u4Next));
            u4Region = u4Next;
        }
    }

//#endif
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** get the first region id of osd plane
 *  @param  u4Plane osd plane
 *  @retval region id, -1 is no region
 */
//-----------------------------------------------------------------------------
INT32 OSD_PLA_GetFirstRegion(UINT32 u4Plane)
{
    OSD_VERIFY_PLANE(u4Plane);
    return _arOsdPlaneInfo[u4Plane].i4FirstRegionId;
}

//-----------------------------------------------------------------------------
/** get the first region id of osd plane
 *  @param  u4Plane osd plane
 *  @retval region id, -1 is no region
 */
//-----------------------------------------------------------------------------
INT32 OSD_PLA_GetFirstRegionList(UINT32 u4Plane)
{
    OSD_VERIFY_PLANE(u4Plane);

    //#ifdef CC_MT5363
    #if 0
    if(gfgPLASwap)
    {
    	if(u4Plane==OSD_PLANE_2)
    		u4Plane=OSD_PLANE_3;
    	else if(u4Plane==OSD_PLANE_3)
    		u4Plane=OSD_PLANE_2;
    }
    #endif
    return _arOsdPlaneInfo[u4Plane].i4FlipedList;
}

//-----------------------------------------------------------------------------
/** get the first region address of osd plane
 *  @param  u4Plane osd plane
 *  @retval region address, 0 is no region
 */
//-----------------------------------------------------------------------------
UINT32 OSD_PLA_GetFirstRegionAddress(UINT32 u4Plane)
{
    if (u4Plane >= (UINT32)OSD_PLANE_MAX_NUM)
    {
        return 0;
    }
    return _arOsdPlaneInfo[u4Plane].u4FirstRegionAddr;
}

void OSD_PLA_SetWorkaround(UINT32 u4Plane, BOOL fgEnable)
{
#if 0
    IC_VERSION_T eVer;
    eVer = BSP_GetIcVersion();

    if ((eVer >= IC_VER_5381_AA) && (eVer < IC_VER_5382P_AA))
    {
        if (u4Plane == (UINT32)OSD_PLANE_1)
        {
            if (_OsdPlaReadyToDisplay(u4Plane))
            {
            	/*
                LOG(7, "5381 workaround, fgEnable=%d\n", fgEnable);
                IO_WRITE32MSK(OSD_BASE, 0x28, 3 << 8, 0xff << 8);
                IO_WRITE32MSK(SCPOS_BASE, 0x5c, 0, 1 << 13);
                */
            }
            else
            {
            	/*
                LOG(7, "5381 workaround, fgEnable=%d\n", fgEnable);
                IO_WRITE32MSK(OSD_BASE, 0x28, 2 << 8, 0xff << 8);
                IO_WRITE32MSK(SCPOS_BASE, 0x5c, 1 << 13, 1 << 13);
                */
            }
            _OSD_UpdateReg();
        }
    }
#endif
}

void OSD_PLA_GetCompressMode(UINT32 u4Plane, UINT32 *pu4DecEnable, UINT32 *pu4AlphaMode)
{
    //IGNORE_RET(_OSD_PLA_GetDecEn(u4Plane, pu4DecEnable));
    //IGNORE_RET(_OSD_PLA_GetAlphaMod(u4Plane, pu4AlphaMode));
}

void OSD_PLA_SetCompressMode(UINT32 u4Plane, UINT32 u4DecEnable, UINT32 u4AlphaMode)
{
/*
    UINT32 u4Region, u4List, u4Tmp;
    INT32 i4Count;

    // plane1 doesn't support compress mode
    if (u4Plane == (UINT32)OSD_PLANE_1)
    {
        return;
    }

    // set alpha mode no matter what compression mode it is
    //IGNORE_RET(_OSD_PLA_SetAlphaMod(u4Plane, u4AlphaMode));

    // check previous status
    //IGNORE_RET(_OSD_PLA_GetDecEn(u4Plane, &u4Tmp));
    if (u4Tmp != u4DecEnable)
    {
        if (_arOsdPlaneInfo[u4Plane].u4FirstRegionAddr != NULL)
        {
            u4List = (UINT32)_arOsdPlaneInfo[u4Plane].i4FlipedList;
            IGNORE_RET(OSD_RGN_LIST_Get(u4List, (INT32)OSD_RGN_LIST_COUNT,
                                       (UINT32*)&i4Count));
            IGNORE_RET(OSD_RGN_LIST_Get(u4List, (INT32)OSD_RGN_LIST_HEAD,
                                        &u4Region));

            while (i4Count--)
            {
                IGNORE_RET(OSD_RGN_Get(u4Region, OSD_RGN_BMP_PITCH, &u4Tmp));
                if (u4DecEnable)
                {
                    // if enable, half the line size, and set little endian
                    IGNORE_RET(OSD_RGN_Set(u4Region, OSD_RGN_BMP_PITCH, u4Tmp >> 1));
                }
                else
                {
                    // restore line size
                    IGNORE_RET(OSD_RGN_Set(u4Region, OSD_RGN_BMP_PITCH, u4Tmp << 1));
                }
                IGNORE_RET(OSD_RGN_SetBigEndian(u4Region, 0));
                IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_NEXT, &u4Region));
            }
        }
        //IGNORE_RET(_OSD_PLA_SetDecEn(u4Plane, u4DecEnable));
    }

    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(u4Plane));
    */
}

//#ifdef CC_MT5363
#if 0
INT32 OSD_PLA_Swap(BOOL fgSwap)
{
	OSD_PLA_UNION_T rOsdPlaneRegTmp1,rOsdPlaneRegTmp2;
	OSD_PLANE_INFO_T rPlaneInfoTmp;

	if(gfgPLASwap==fgSwap)
		return (INT32)OSD_RET_OK;;

	gfgPLASwap=fgSwap;

	_OSD_PLA_GetReg(OSD_PLANE_2, rOsdPlaneRegTmp1.au4Reg);	//get Plane2 reg to tmp1
	_OSD_PLA_GetReg(OSD_PLANE_3, rOsdPlaneRegTmp2.au4Reg);	//get Planer3 reg to tmp2
	_OSD_PLA_SetReg(OSD_PLANE_2, rOsdPlaneRegTmp2.au4Reg);	//Set Plane2 reg with tmp2(old scaler3)
	_OSD_PLA_SetReg(OSD_PLANE_3, rOsdPlaneRegTmp1.au4Reg);	//Set Plane2 reg with tmp1(old scaler2)

	rPlaneInfoTmp=_arOsdPlaneInfo[OSD_PLANE_2];
	_arOsdPlaneInfo[OSD_PLANE_2]=_arOsdPlaneInfo[OSD_PLANE_3];
	_arOsdPlaneInfo[OSD_PLANE_3]=rPlaneInfoTmp;

	_OSD_PLA_UpdateHwReg(OSD_PLANE_2);
	_OSD_PLA_UpdateHwReg(OSD_PLANE_3);

	return (INT32)OSD_RET_OK;
}
#endif

//Add for Set DRAM parameters    	
void OSD_PLA_SetContReqLmt(UINT32 u4Plane, UINT32 u4Value)
{
	IGNORE_RET(_OSD_PLA_SetContReqLmt(u4Plane, u4Value));
}
void OSD_PLA_SetFifoSize(UINT32 u4Plane, UINT32 u4Value)
{
	IGNORE_RET(_OSD_PLA_SetFifoSize(u4Plane, u4Value));
}
void OSD_PLA_SetVacTh(UINT32 u4Plane, UINT32 u4Value)
{
	IGNORE_RET(_OSD_PLA_SetVacTh(u4Plane, u4Value));
}
void OSD_PLA_SetPauseCnt(UINT32 u4Plane, UINT32 u4Value)
{
	IGNORE_RET(_OSD_PLA_SetPauseCnt(u4Plane, u4Value));
}
void OSD_PLA_SetContReqLmt0(UINT32 u4Plane, UINT32 u4Value)
{
	IGNORE_RET(_OSD_PLA_SetContReqLmt0(u4Plane, u4Value));
}

#if 0
//For 3D OSD
INT32 OSD_PLA_Set3DAddress(UINT32 u4Plane, UINT32 u4L_Address, UINT32 u4R_Address)
{
    if( (u4Plane!=OSD_PLANE_2)&&(u4Plane!=OSD_PLANE_3))
    {
    	return -(INT32)OSD_RET_INV_PLANE;
    }
    _u4_3D_Plane = u4Plane;
    _au4_3D_L_Addr=u4L_Address;
    _au4_3D_R_Addr=u4R_Address;
    //_au4_3D_L_Addr[u4Plane-1]=u4L_Address;
    //_au4_3D_R_Addr[u4Plane-1]=u4R_Address;
    _u4_3D_Region = _arOsdPlaneInfo[u4Plane].i4FirstRegionId;

    return (INT32)OSD_RET_OK;
}

INT32 OSD_Vsync_3DUpdateLR(UINT32 u4L_R)
{
	if(_u13DMode==1) // 3D mode
	{
		if(u4L_R==0) // L
			//OSD_RGN_Set(_u4_3D_Region,OSD_RGN_BMP_ADDR,_au4_3D_L_Addr[_u4_3D_Plane]);
			OSD_RGN_Set(_u4_3D_Region,OSD_RGN_BMP_ADDR,_au4_3D_L_Addr);
		else
			//OSD_RGN_Set(_u4_3D_Region,OSD_RGN_BMP_ADDR,_au4_3D_R_Addr[_u4_3D_Plane]);
			OSD_RGN_Set(_u4_3D_Region,OSD_RGN_BMP_ADDR,_au4_3D_R_Addr);
	}
	else
	{
		if(_u1Previous3DMode !=_u13DMode)
			//OSD_RGN_Set(_u4_3D_Region,OSD_RGN_BMP_ADDR,_au4_3D_L_Addr[_u4_3D_Plane]);
			OSD_RGN_Set(_u4_3D_Region,OSD_RGN_BMP_ADDR,_au4_3D_L_Addr);
	}
	
	_u1Previous3DMode =_u13DMode;
    	return (INT32)OSD_RET_OK;
}	

INT32 OSD_Set3DMode(UINT8 u1Mode)
{
    _u13DMode=u1Mode;
    return (INT32)OSD_RET_OK;
}
#endif

INT32 OSD_PLA_Set3DMode(UINT32 u4Plane, UINT8 u1Mode)
{
    if( (u4Plane!=OSD_PLANE_2)&&(u4Plane!=OSD_PLANE_3))
    {
    	return -(INT32)OSD_RET_INV_PLANE;
    }

    _OSD_PLA_SetShutter(u4Plane, u1Mode);

    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(u4Plane));

    return (INT32)OSD_RET_OK;
}


INT32 OSD_PLA_Set3DRegion(UINT32 u4Plane, UINT32 u4LeftRgn, UINT32 u4RightRgn)
{
    UINT32 u4u4LeftRgnAddr;
    UINT32 u4u4RightRgnAddr;
    
    if( (u4Plane!=OSD_PLANE_2)&&(u4Plane!=OSD_PLANE_3))
    {
    	return -(INT32)OSD_RET_INV_PLANE;
    }
    
    OSD_VERIFY_REGION(u4LeftRgn);

    _arOsdPlaneInfo[u4Plane].i4FirstRegionId = u4LeftRgn;

    VERIFY((INT32)OSD_RET_OK ==
           _OSD_RGN_GetAddress((UINT32)u4LeftRgn, &u4u4LeftRgnAddr));

    VERIFY((INT32)OSD_RET_OK ==
           _OSD_PLA_SetHeaderAddr(u4Plane, u4u4LeftRgnAddr));

    VERIFY((INT32)OSD_RET_OK ==
           _OSD_RGN_GetAddress((UINT32)u4RightRgn, &u4u4RightRgnAddr));

    VERIFY((INT32)OSD_RET_OK ==
           _OSD_PLA_SetRightHeaderAddr(u4Plane, u4u4RightRgnAddr));

    //printf("u4u4RightRgnAddr = 0x%x\n",u4u4RightRgnAddr);

    _arOsdPlaneInfo[u4Plane].u4FirstRegionAddr = u4u4LeftRgnAddr;

    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(u4Plane));
    
    return (INT32)OSD_RET_OK;
}


#ifndef CC_MTK_LOADER
#ifdef SYS_3D_SUPPORT
INT32 OSD_PLA_Set3DFlip(UINT32 u4Plane, OSD_3D_MODE_T e3DMode, UINT32 u4Shift, 
					UINT32 u4BmpWidth, UINT32 u4BmpHeight, UINT32 u4BmpPitch,
			        UINT32 u4BmpAddr, UINT32 eColorMode)
{
	RZ_OSD_SCL_PARAM_SET_T rSclParam;
	E_RZ_INOUT_TYPE_T eRzType;

    _u43DPlane = u4Plane;
    _e3DMode = e3DMode;
    _u4Shift = u4Shift;
    _u4BmpWidth = u4BmpWidth;
    _u4BmpHeight = u4BmpHeight;
    _u4BmpPitch = u4BmpPitch;
    _u4DstAddr = u4BmpAddr;
    _eColorMode = eColorMode;

    if (_u4TmpBuf1 == 0)
    {
	    _u4TmpBuf1 =
	       (UINT32)BSP_AllocAlignedDmaMemory((UI_WIDTH*UI_HEIGHT*UI_BPP), (OSD_DMA_ALIGN_SIZE + 1));

        Printf("TmpBuf1 = 0x%x\n", _u4TmpBuf1);
        
        _u4SrcAddr = _u4TmpBuf1;     
    }	
   
    GFX_Lock();
	GFX_SetSrc((UINT8 *)u4BmpAddr, eColorMode, u4BmpPitch);
	GFX_SetDst((UINT8 *)_u4TmpBuf1, eColorMode, u4BmpPitch);
	GFX_BitBlt(0, 0, 0, 0, u4BmpWidth, u4BmpHeight);

	GFX_Flush();
	GFX_Wait();
	GFX_Unlock();

	x_memset(&rSclParam,0,sizeof(RZ_OSD_SCL_PARAM_SET_T));

	if (e3DMode == OSD_3D_SBSHALF)
	{
		rSclParam.u4TgW     = u4BmpWidth >> 1;		
		rSclParam.u4TgH     = u4BmpHeight;	
	}
	else if (e3DMode == OSD_3D_TPANDBT)
	{
		rSclParam.u4TgW     = u4BmpWidth;		
		rSclParam.u4TgH     = u4BmpHeight >> 1;		
	}
				
	rSclParam.u4SrcBufLen   = u4BmpPitch;
	rSclParam.u4SrcW        = u4BmpWidth;
	rSclParam.u4SrcH        = u4BmpHeight;

	rSclParam.u4SrcBase     = u4BmpAddr;
	rSclParam.u4IsSrcDirCM  = 1;
	rSclParam.u4SrcCM       = (eColorMode == OSD_CM_ARGB4444_DIRECT16)?
	                          (UINT32) E_RZ_OSD_DIR_CM_ARGB4444:(UINT32) E_RZ_OSD_DIR_CM_AYCbCr8888;
	rSclParam.u4SrcHOffset  = 0;
	rSclParam.u4SrcVOffset  = 0;

	rSclParam.u4TgBase      = u4BmpAddr;

	rSclParam.u4TgHOffset   = 0;		
	rSclParam.u4TgVOffset   = 0;
	
	rSclParam.u4TgCM        = rSclParam.u4SrcCM;
	rSclParam.u4TgBufLen    = u4BmpPitch;
    
	eRzType =  E_RZ_INOUT_TYPE_OSDMD;

	IMGRZ_Lock();
	IMGRZ_ReInit();
	IMGRZ_SetScaleOpt(eRzType);
	IMGRZ_Scale(&rSclParam);
	IMGRZ_Flush();
	IMGRZ_Wait();
	IMGRZ_Unlock();

	GFX_Lock();
	GFX_SetSrc((UINT8 *)u4BmpAddr, eColorMode, u4BmpPitch);
	GFX_SetDst((UINT8 *)u4BmpAddr, eColorMode, u4BmpPitch);
	GFX_SetColor(0x0);
	GFX_SetBltOpt((UINT32)D_GFXFLAG_NONE, 0x00000000, 0xffffffff);

	if (e3DMode == OSD_3D_SBSHALF)
	{
	    GFX_Fill(rSclParam.u4TgW, 0, rSclParam.u4TgW, rSclParam.u4TgH);	
	    GFX_BitBlt((u4Shift >> 1), 0, rSclParam.u4TgW, 0, (rSclParam.u4TgW - (u4Shift >> 1)),  rSclParam.u4TgH);
	}
	else if (e3DMode == OSD_3D_TPANDBT)
	{
		GFX_Fill(0, rSclParam.u4TgH, rSclParam.u4TgW, rSclParam.u4TgH);
	    GFX_BitBlt(u4Shift, 0, 0, rSclParam.u4TgH, (rSclParam.u4TgW - u4Shift),  rSclParam.u4TgH);	
	}

	GFX_Flush();
	GFX_Wait();
	GFX_Unlock();
    
	return (INT32)OSD_RET_OK;
}



INT32 OSD_PLA_Set3DBlit(UINT32 u4Plane, OSD_3D_MODE_T e3DMode, UINT32 u4Shift, 
						UINT32 u4BmpWidth, UINT32 u4BmpHeight, UINT32 u4BmpPitch,
						UINT32 u4DstAddr, UINT32 eColorMode, UINT32 u4SrcAddr)
{
	RZ_OSD_SCL_PARAM_SET_T rSclParam;
	E_RZ_INOUT_TYPE_T eRzType;

    _u43DPlane = u4Plane;
    _e3DMode = e3DMode;
    _u4Shift = u4Shift;
    _u4BmpWidth = u4BmpWidth;
    _u4BmpHeight = u4BmpHeight;
    _u4BmpPitch = u4BmpPitch;
    _u4SrcAddr = u4SrcAddr;
    _u4DstAddr = u4DstAddr;    
    _eColorMode = eColorMode;   

	x_memset(&rSclParam,0,sizeof(RZ_OSD_SCL_PARAM_SET_T));

    if (_u4TmpBuf == 0)
    {
	    _u4TmpBuf =
	       (UINT32)BSP_AllocAlignedDmaMemory((UI_WIDTH*UI_HEIGHT*UI_BPP), (OSD_DMA_ALIGN_SIZE + 1));
        
        Printf("TmpBuf = 0x%x\n", _u4TmpBuf);        
    }	

	if (e3DMode == OSD_3D_SBSHALF)
	{
		rSclParam.u4TgW         = u4BmpWidth >> 1;		
		rSclParam.u4TgH = u4BmpHeight;	
	}
	else if (e3DMode == OSD_3D_TPANDBT)
	{
		rSclParam.u4TgW     = u4BmpWidth;		
		rSclParam.u4TgH     = u4BmpHeight >> 1;		
	}	
				
	rSclParam.u4SrcBufLen   = u4BmpPitch;
	rSclParam.u4SrcW        = u4BmpWidth;
	rSclParam.u4SrcH        = u4BmpHeight;

	rSclParam.u4SrcBase     = u4SrcAddr;
	rSclParam.u4IsSrcDirCM  = 1;
	rSclParam.u4SrcCM       = (eColorMode == OSD_CM_ARGB4444_DIRECT16)?
	                          (UINT32) E_RZ_OSD_DIR_CM_ARGB4444:(UINT32) E_RZ_OSD_DIR_CM_AYCbCr8888;
	rSclParam.u4SrcHOffset  = 0;
	rSclParam.u4SrcVOffset  = 0;

	rSclParam.u4TgBase      = _u4TmpBuf;


	rSclParam.u4TgHOffset   = 0;		
	rSclParam.u4TgVOffset   = 0;
	
	rSclParam.u4TgCM        = rSclParam.u4SrcCM;
	rSclParam.u4TgBufLen    = u4BmpPitch;
    
	eRzType =  E_RZ_INOUT_TYPE_OSDMD;

	IMGRZ_Lock();
	IMGRZ_ReInit();
	IMGRZ_SetScaleOpt(eRzType);
	IMGRZ_Scale(&rSclParam);
	IMGRZ_Flush();
	IMGRZ_Wait();
	IMGRZ_Unlock();

	GFX_Lock();
	GFX_SetSrc((UINT8 *)_u4TmpBuf, eColorMode, u4BmpPitch);

	GFX_SetDst((UINT8 *)_u4TmpBuf, eColorMode, u4BmpPitch);
	GFX_SetColor(0x0);
	GFX_SetBltOpt((UINT32)D_GFXFLAG_NONE, 0x00000000, 0xffffffff);
		
	if (e3DMode == OSD_3D_SBSHALF)
	{
	    GFX_Fill(rSclParam.u4TgW, 0, rSclParam.u4TgW, rSclParam.u4TgH);	
	    GFX_BitBlt((u4Shift >> 1), 0, rSclParam.u4TgW, 0, (rSclParam.u4TgW - (u4Shift >> 1)),  rSclParam.u4TgH);
	}
	else if (e3DMode == OSD_3D_TPANDBT)
	{
	    GFX_Fill(0, rSclParam.u4TgH, rSclParam.u4TgW, rSclParam.u4TgH);		
	    GFX_BitBlt(u4Shift, 0, 0, rSclParam.u4TgH, (rSclParam.u4TgW - u4Shift),  rSclParam.u4TgH);	
	}
	
	GFX_SetSrc((UINT8 *)_u4TmpBuf, eColorMode, u4BmpPitch);
	GFX_SetDst((UINT8 *)u4DstAddr, eColorMode, u4BmpPitch);
	GFX_BitBlt(0, 0, 0, 0, u4BmpWidth, u4BmpHeight);

	GFX_Flush();
	GFX_Wait();
	GFX_Unlock();
    
	return (INT32)OSD_RET_OK;	

}


INT32 OSD_PLA_Redraw3D(OSD_3D_MODE_T e3DMode)
{

    if ((_u4DstAddr != 0) && (e3DMode != _e3DMode))
    {       
    	if (e3DMode == OSD_3D_NONE)
    	{
    		GFX_Lock();
		GFX_SetSrc((UINT8 *)_u4SrcAddr, _eColorMode, _u4BmpPitch);
		GFX_SetDst((UINT8 *)_u4DstAddr, _eColorMode, _u4BmpPitch);
		GFX_BitBlt(0, 0, 0, 0, _u4BmpWidth, _u4BmpHeight);

		GFX_Flush();
		GFX_Wait();
		GFX_Unlock();		    		
    	}
	else
	{
       OSD_PLA_Set3DBlit(_u43DPlane, e3DMode,  _u4Shift, 
                    _u4BmpWidth,  _u4BmpHeight,  _u4BmpPitch,
                    _u4DstAddr,  _eColorMode,  _u4SrcAddr);         
	}

	_e3DMode = e3DMode;

    }
	return (INT32)OSD_RET_OK;	

}
#endif
#if defined(CC_CONFIG_OSD_SBS)
INT32 OSD_PLA_Set3DDFB(UINT32 u4Plane, UINT32 e3DMode, UINT32 u4Shift, 
						UINT32 u4BmpWidth, UINT32 u4BmpHeight, UINT32 u4BmpPitch,
						UINT32 u4DstAddr, UINT32 eColorMode, UINT32 u4SrcAddr)
{
	RZ_OSD_SCL_PARAM_SET_T rSclParam;
	E_RZ_INOUT_TYPE_T eRzType;

	
	x_memset(&rSclParam,0,sizeof(RZ_OSD_SCL_PARAM_SET_T));
	 
	if (e3DMode == 0)//OSD_3D_SBSHALF
	{
		rSclParam.u4TgW         = u4BmpWidth >> 1;		
		rSclParam.u4TgH = u4BmpHeight;	
	}
	else if (e3DMode == 1) //OSD_3D_TPANDBT
	{
		rSclParam.u4TgW     = u4BmpWidth;		
		rSclParam.u4TgH     = u4BmpHeight >> 1;		
	}	
	else
	{
	  GFX_Lock();
	  GFX_SetSrc((UINT8 *)u4SrcAddr, eColorMode, u4BmpPitch);
	  GFX_SetDst((UINT8 *)u4DstAddr, eColorMode, u4BmpPitch);
	  GFX_BitBlt(0, 0, 0, 0, u4BmpWidth, u4BmpHeight);

	  GFX_Flush();
	  GFX_Wait();
	  GFX_Unlock();
	  return (INT32)OSD_RET_OK;
	}
				
	rSclParam.u4SrcBufLen   = u4BmpPitch;
	rSclParam.u4SrcW        = u4BmpWidth;
	rSclParam.u4SrcH        = u4BmpHeight;

	rSclParam.u4SrcBase     = u4SrcAddr;
	rSclParam.u4IsSrcDirCM  = 1;
	rSclParam.u4SrcCM       = (eColorMode == OSD_CM_ARGB4444_DIRECT16)?
	                          (UINT32) E_RZ_OSD_DIR_CM_ARGB4444:(UINT32) E_RZ_OSD_DIR_CM_AYCbCr8888;
	rSclParam.u4SrcHOffset  = 0;
	rSclParam.u4SrcVOffset  = 0;

	rSclParam.u4TgBase      = u4DstAddr;


	rSclParam.u4TgHOffset   = 0;		
	rSclParam.u4TgVOffset   = 0;
	
	rSclParam.u4TgCM        = rSclParam.u4SrcCM;
	rSclParam.u4TgBufLen    = u4BmpPitch;
    
	eRzType =  E_RZ_INOUT_TYPE_OSDMD;

	IMGRZ_Lock();
	IMGRZ_ReInit();
	IMGRZ_SetScaleOpt(eRzType);
	IMGRZ_Scale(&rSclParam);
	IMGRZ_Flush();
	IMGRZ_Wait();
	IMGRZ_Unlock();

	GFX_Lock();
	GFX_SetSrc((UINT8 *)u4DstAddr, eColorMode, u4BmpPitch);

	GFX_SetDst((UINT8 *)u4DstAddr, eColorMode, u4BmpPitch);
	GFX_SetColor(0x0);
	GFX_SetBltOpt((UINT32)D_GFXFLAG_NONE, 0x00000000, 0xffffffff);
		
	if (e3DMode == 0)// OSD_3D_SBSHALF
	{
	    GFX_Fill(rSclParam.u4TgW, 0, rSclParam.u4TgW, rSclParam.u4TgH);	
	    GFX_BitBlt((u4Shift >> 1), 0, rSclParam.u4TgW, 0, (rSclParam.u4TgW - (u4Shift >> 1)),  rSclParam.u4TgH);
	}
	else //OSD_3D_TPANDBT
	{
	    GFX_Fill(0, rSclParam.u4TgH, rSclParam.u4TgW, rSclParam.u4TgH);		
	    GFX_BitBlt(u4Shift, 0, 0, rSclParam.u4TgH, (rSclParam.u4TgW - u4Shift),  rSclParam.u4TgH);	
	}
	
	GFX_SetSrc((UINT8 *)u4DstAddr, eColorMode, u4BmpPitch);
	GFX_SetDst((UINT8 *)u4DstAddr, eColorMode, u4BmpPitch);
	GFX_BitBlt(0, 0, 0, 0, u4BmpWidth, u4BmpHeight);

	GFX_Flush();
	GFX_Wait();
	GFX_Unlock();
    
	return (INT32)OSD_RET_OK;	

}
#endif

#endif

