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
 * $RCSfile: vdp_image.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdp_image.c
 *  Brief of file vdp_image.c.
 *  Details of file vdp_image.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "srm_drvif.h"
#include "fbm_drvif.h"
#include "vdp_drvif.h"
#include "b2r_drvif.h"
#include "vdp_vsync.h"
#include "vdp_hal.h"
#include "vdp_debug.h"
#include "drvcust_if.h"

#ifdef CC_SCPOS_EN
#include "nptv_if.h"
#include "vdp_display.h"
#include "source_table.h"  // nptv
#endif

//#include "x_hal_926.h" // pibben 5387
#include "x_hal_5381.h"
#include "x_os.h"
#include "x_timer.h"
#include "x_bim.h"
#include "x_assert.h"
LINT_EXT_HEADER_END

LINT_SUPPRESS_BRACE(818)                //Info 818: Pointer parameter 'prInfo' (line 504) could be'

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//#define VDP_IMAGE_DEBUG

#define VDP_IMAGE_MAX_FB    3

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct
{
    UINT32 u4ColorMode;         ///< Color mode of Frame Buffer
    UINT32 u4YAddr;             ///< Y Addr of Frame Buffer
    UINT32 u4CAddr;             ///< C Addr of Frame Buffer
    UINT32 u4Width;  	        ///< Width of Frame Buffer
    UINT32 u4Height; 	        ///< Height of Frame Buffer
    UINT32 u4Pitch; 	        ///< Pitch of Frame Buffer
} VDP_IMAGE_FB_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

//static void _ImageWaitPoolChangeCb(FBM_POOL_TYPE_T ePool);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static VDP_IMAGE_INFO_T _rVdpImageInfo;

static VDP_IMAGE_FB_T _arVdpImageFb[VDP_IMAGE_MAX_FB];

static UINT32 _u4InternalFbNs = 0;

static UINT32 _u4VdpImageMute = 1;

static UINT32 _u4VdpImageConnectPlane = VDP_1;

static UINT32 _u4VdpImageConnected = 0;

static VDP_CONF_T* _prVdpConf[VDP_NS];

static HANDLE_T _hSemaphore = NULL;            // Wait SCPOS Pool Ready

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

#if 0
/**
 * Wait SCPOS Pool Change Call Back
 *
 * @param ePool: Change Pool Type
 * @return void
 */
static void _ImageWaitPoolChangeCb(FBM_POOL_TYPE_T ePool)
{
    //// TODO, maybe not required
    /*
        if (DRVCUST_OptGet(eFbmMemSize) == FBM_MEM_CFG_MT5382x64MB)
        {
            if (ePool == FBM_POOL_TYPE_SCPOS_MAIN)
            {
                VERIFY (x_sema_unlock(_hSemaphore) == OSR_OK);
            }
        }
    */
}
#endif

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

/**
 * Video Image Module : Initialization
 *
 * @param void
 * @return void
 */
void _VDP_Image_Init(void)
{
    static UINT32 _u4ImageInit = 0;

    UINT32 u4VdpIdx;
    FBM_POOL_T* prPool;

    if (_u4ImageInit != 0)
    {
        return;
    }
    _u4ImageInit = 1;

    for (u4VdpIdx = 0; u4VdpIdx < VDP_NS; u4VdpIdx++)
    {
        _prVdpConf[u4VdpIdx] = VDP_GetConf((UCHAR)u4VdpIdx);
    }

    // create semaphore
    VERIFY(x_sema_create(&_hSemaphore, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

    _u4VdpImageMute = 1;
    _u4VdpImageConnectPlane = VDP_1;

    _u4InternalFbNs = 0;
    _rVdpImageInfo.u4ColorMode = 0;
    _rVdpImageInfo.u4Width = 0;
    _rVdpImageInfo.u4Height = 0;
    _rVdpImageInfo.u4FrameBufferNs = 0;
    _rVdpImageInfo.u4PitchAlignement = 0;

    //prPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_B2R);

    //use new fbm config
    prPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_JPEG_OSD1);
    prPool->u4Mode = FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_2FB;

    if (prPool != NULL)
    {
        UINT32 u4FbIdx;
        UINT32 u4FbSize;
        UINT32 u4FbAddr;

        _rVdpImageInfo.u4Width = prPool->u4Width;
        _rVdpImageInfo.u4Height = prPool->u4Height;

        if (prPool->u4Mode & FBM_POOL_MODE_SCPOS_3FB)
        {
            _u4InternalFbNs = 3;
        }
        else if (prPool->u4Mode & FBM_POOL_MODE_SCPOS_2FB)
        {
            _u4InternalFbNs = 2;
        }
        else if (prPool->u4Mode & FBM_POOL_MODE_SCPOS_1FB)
        {
            _u4InternalFbNs = 1;
        }
        else
        {
            _u4InternalFbNs = 0;
        }

        u4FbSize = (_rVdpImageInfo.u4Width * _rVdpImageInfo.u4Height);

        // Handle Alignement
        if ((u4FbSize & (64 - 1)) != 0)
        {
            u4FbSize += (64 - 1);
            u4FbSize &= ~(64 - 1);
        }

        u4FbAddr = prPool->u4Addr;
        for (u4FbIdx = 0; u4FbIdx < _u4InternalFbNs; u4FbIdx++)
        {
            // Handle Alignement
            if ((u4FbAddr & (64 - 1)) != 0)
            {
                u4FbAddr += (64 - 1);
                u4FbAddr &= ~(64 - 1);
            }

            _arVdpImageFb[u4FbIdx].u4YAddr = u4FbAddr;
            u4FbAddr += u4FbSize;

            if (prPool->u4Mode & FBM_POOL_MODE_422_FB)
            {
                _arVdpImageFb[u4FbIdx].u4CAddr = u4FbAddr;
                u4FbAddr += u4FbSize;
            }
            else if (prPool->u4Mode & FBM_POOL_MODE_420_FB)
            {
                _arVdpImageFb[u4FbIdx].u4CAddr = u4FbAddr;
                u4FbAddr += (u4FbSize >> 1);
            }
            else
            {
                ASSERT(0);
            }

            LOG(3, "Image FB(%d) Y(0x%x) C(0x%x)\n",
                u4FbIdx,
                _arVdpImageFb[u4FbIdx].u4YAddr,
                _arVdpImageFb[u4FbIdx].u4CAddr);
        }

        // Hardware Checking (1920 x 1080)
        if (_rVdpImageInfo.u4Width > 1920)
        {
            _rVdpImageInfo.u4Width = 1920;
        }

        if (_rVdpImageInfo.u4Height > 1080)
        {
            _rVdpImageInfo.u4Height = 1080;
        }

        // 5382p
        _rVdpImageInfo.u4ColorMode = (VDP_IMAGE_420_MODE | VDP_IMAGE_422_MODE);
        _rVdpImageInfo.u4PitchAlignement = 8;
        _rVdpImageInfo.u4FrameBufferNs = _u4InternalFbNs;
    }
}

/**
 * Video Image Module : Get information
 *
 * @param void
 * @return pointer of Video Image Module Capability Infomation
 */
VDP_IMAGE_INFO_T* _VDP_Image_GetInfo(void)
{
    return &_rVdpImageInfo;
}

/**
 * Video Image Module : Connect
 *
 * @param u4VdpId: Video plane connected to
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
UINT32 _VDP_Image_Connect(UINT32 u4VdpId)
{
    LINT_SUPPRESS_BRACE(641)        // Warning 641: Converting enum 'VSS_MAJOR_TYPE' to int'

    // Check video plane id
    if (u4VdpId >= VDP_NS)
    {
        return VDP_SET_ERROR;
    }

    LOG(3, "VDP(%d) Image Connect\n", u4VdpId);

    if ((_u4VdpImageConnected != 0) &&
            (_u4VdpImageConnectPlane == u4VdpId))
    {
        // re-connect
        return VDP_SET_OK;
    }

//    FBM_RegCbFunc(FBM_CB_FUNC_POOL_CHANGE, (UINT32)_ImageWaitPoolChangeCb);

    _u4VdpImageConnectPlane = u4VdpId;

    _u4VdpImageConnected = 1;

#ifdef CC_SCPOS_EN
    if (bApiVideoSetVideoSrc((UCHAR)u4VdpId, SV_VS_DT1) == 0)
    {
        return VDP_SET_ERROR;
    }
#endif

    // Pause VSYNC, By Pass DTV Issue
    _VDP_VsyncOnOff((UCHAR)u4VdpId, 0);

    _VDP_HalForceBg((UCHAR)_u4VdpImageConnectPlane, (UCHAR)_u4VdpImageMute);

#ifdef CC_MTAL
    SRM_SetMmMode(SRM_MM_MODE_TYPE_OFF);
#else
    SRM_SetMmMode(SRM_MM_MODE_TYPE_JPEG);
#endif

    // Switch B2R to 1080p mode

    _prVdpConf[_u4VdpImageConnectPlane]->rOutInfo.ucPmxMode = VDP_B2R_MODE_1080P_60;
    _prVdpConf[_u4VdpImageConnectPlane]->rOutInfo.ucPrg = 1;
    _prVdpConf[_u4VdpImageConnectPlane]->rOutInfo.ucFrameRate = 60;
    _prVdpConf[_u4VdpImageConnectPlane]->rOutInfo.u4Width = 1920;
    _prVdpConf[_u4VdpImageConnectPlane]->rOutInfo.u4Height = 1080;
    _prVdpConf[_u4VdpImageConnectPlane]->rOutInfo.u4TotalWidth = 2200;
    _prVdpConf[_u4VdpImageConnectPlane]->rOutInfo.u4TotalHeight = 1125;
    _prVdpConf[_u4VdpImageConnectPlane]->rOutInfo.ucDoubleClk = 0;
    _prVdpConf[_u4VdpImageConnectPlane]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_500;

    _VDP_HalSetOutSize((UCHAR)_u4VdpImageConnectPlane,
                       _prVdpConf[_u4VdpImageConnectPlane]->rOutInfo.u4TotalWidth,
                       _prVdpConf[_u4VdpImageConnectPlane]->rOutInfo.u4TotalHeight,
                       !(_prVdpConf[_u4VdpImageConnectPlane]->rOutInfo.ucPrg),
                       _prVdpConf[_u4VdpImageConnectPlane]->rOutInfo.ucDoubleClk,
                       _prVdpConf[_u4VdpImageConnectPlane]->rOutInfo.u4ClkMode);

    // Init Source Info
    _prVdpConf[_u4VdpImageConnectPlane]->u4SrcLineSize = _prVdpConf[_u4VdpImageConnectPlane]->rOutInfo.u4Width;
    _prVdpConf[_u4VdpImageConnectPlane]->u4SrcWidth = _prVdpConf[_u4VdpImageConnectPlane]->rOutInfo.u4Width;
    _prVdpConf[_u4VdpImageConnectPlane]->u4SrcHeight = _prVdpConf[_u4VdpImageConnectPlane]->rOutInfo.u4Height;
    _prVdpConf[_u4VdpImageConnectPlane]->u4MpegWidth = 1920;
    _prVdpConf[_u4VdpImageConnectPlane]->u4MpegHeight = 1080;

    // Init Pitch
    _VDP_HalSetSrcSize((UCHAR)_u4VdpImageConnectPlane,
                       _prVdpConf[_u4VdpImageConnectPlane]->rOutInfo.u4Width,
                       _prVdpConf[_u4VdpImageConnectPlane]->rOutInfo.u4Height,
                       _prVdpConf[_u4VdpImageConnectPlane]->rOutInfo.u4Width);

    _VDP_StatusNofify((UCHAR)_u4VdpImageConnectPlane, VDP_B2R_START_PLAY);

    //// TODO, maybe not required
    /*
        if (DRVCUST_OptGet(eFbmMemSize) == FBM_MEM_CFG_MT5382x64MB)
        {
            if (x_sema_lock_timeout(_hSemaphore, 1000) != OSR_OK)
            {
                LOG(3, "VDP Image Connect Timeout\n");
            }

            // Add 50 ms for safe
            x_thread_delay(50);
        }

        FBM_RegCbFunc(FBM_CB_FUNC_POOL_CHANGE, 0);
    */

    return VDP_SET_OK;
}

/**
 * Video Image Module : Disconnect
 *
 * @param void
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
UINT32 _VDP_Image_Disconnect(void)
{
    _u4VdpImageConnected = 0;

    // Start VSYNC
    _VDP_VsyncOnOff((UCHAR)_u4VdpImageConnectPlane, 1);

    // Restore Mode
    _VDP_HalSetFormat((UCHAR)_u4VdpImageConnectPlane, 0, 0);

    SRM_SetMmMode(SRM_MM_MODE_TYPE_OFF);

    return VDP_SET_OK;
}

/**
 * Video Image Module : Mute on/off
 *
 * @param u4OnOff: Mute on/off
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
UINT32 _VDP_Image_Mute(UINT32 u4OnOff)
{
    _u4VdpImageMute = u4OnOff;

    _VDP_HalForceBg((UCHAR)_u4VdpImageConnectPlane, (UCHAR)_u4VdpImageMute);

    return VDP_SET_OK;
}

/**
 * Video Image Module : Show Frame Buffer
 *
 * @param prInfo: Frame Buffer Info
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
UINT32 _VDP_Image_Show(VDP_IMAGE_ARG_T* prInfo)
{
    if (prInfo == NULL)
    {
        return VDP_SET_ERROR;
    }

    // Check color mode
    if ((prInfo->u4ColorMode & _rVdpImageInfo.u4ColorMode) == 0)
    {
        return VDP_SET_ERROR;
    }

    // Check height
    if (prInfo->u4Height > _rVdpImageInfo.u4Height)
    {
        return VDP_SET_ERROR;
    }

    // Check width
    if (prInfo->u4Width > _rVdpImageInfo.u4Width)
    {
        return VDP_SET_ERROR;
    }

    // Check width
    if (prInfo->u4Width > prInfo->u4Pitch)
    {
        return VDP_SET_ERROR;
    }

    // Check pitch
    if ((prInfo->u4Pitch & (_rVdpImageInfo.u4PitchAlignement - 1)) != 0)
    {
        return VDP_SET_ERROR;
    }

    // Check frame buffer id
    if (prInfo->u4FrameBufferId > _rVdpImageInfo.u4FrameBufferNs)
    {
        return VDP_SET_ERROR;
    }

    // Check connect
    if (_u4VdpImageConnected == 0)
    {
        return VDP_SET_ERROR;
    }

    // Check video plane id
    if (_u4VdpImageConnectPlane >= VDP_NS)
    {
        return VDP_SET_ERROR;
    }

    _arVdpImageFb[prInfo->u4FrameBufferId].u4Width = prInfo->u4Width;
    _arVdpImageFb[prInfo->u4FrameBufferId].u4Height = prInfo->u4Height;
    _arVdpImageFb[prInfo->u4FrameBufferId].u4Pitch = prInfo->u4Pitch;
    _arVdpImageFb[prInfo->u4FrameBufferId].u4ColorMode = prInfo->u4ColorMode;

    // Addr
    _VDP_HalSetAddr((UCHAR)_u4VdpImageConnectPlane,
                    _arVdpImageFb[prInfo->u4FrameBufferId].u4YAddr,
                    _arVdpImageFb[prInfo->u4FrameBufferId].u4CAddr);

    // Format, 420/422, Linear
    _VDP_HalSetFormat((UCHAR)_u4VdpImageConnectPlane, (_arVdpImageFb[prInfo->u4FrameBufferId].u4ColorMode == VDP_IMAGE_422_MODE), 1);

    // Source Info
    _prVdpConf[_u4VdpImageConnectPlane]->u4SrcLineSize = prInfo->u4Pitch;
    _prVdpConf[_u4VdpImageConnectPlane]->u4SrcWidth = prInfo->u4Width;
    _prVdpConf[_u4VdpImageConnectPlane]->u4SrcHeight = prInfo->u4Height;

    // Update Pitch
    _VDP_HalSetSrcSize((UCHAR)_u4VdpImageConnectPlane,
                       _prVdpConf[_u4VdpImageConnectPlane]->rOutInfo.u4Width,
                       _prVdpConf[_u4VdpImageConnectPlane]->rOutInfo.u4Height,
                       _prVdpConf[_u4VdpImageConnectPlane]->u4SrcLineSize);

    // Rescale
#ifdef CC_SCPOS_EN
#if (defined(CC_MT5368) || defined(CC_MT5396))
    if (bApiVideoSetSrcRegion((UCHAR)_u4VdpImageConnectPlane,
                              (UINT16)(_prVdpConf[_u4VdpImageConnectPlane]->rSrcRegion.u4X),
                              (UINT16)(_prVdpConf[_u4VdpImageConnectPlane]->rSrcRegion.u4Y),
                              (UINT16)(_prVdpConf[_u4VdpImageConnectPlane]->rSrcRegion.u4Width),
                              (UINT16)(_prVdpConf[_u4VdpImageConnectPlane]->rSrcRegion.u4Height)) != 1)
    {
        return VDP_SET_ERROR;
    }
#else
    if (bApiVideoSetSrcPosSize((UCHAR)_u4VdpImageConnectPlane,
                               (UINT16)(_prVdpConf[_u4VdpImageConnectPlane]->rSrcRegion.u4X),
                               (UINT16)(_prVdpConf[_u4VdpImageConnectPlane]->rSrcRegion.u4Y),
                               (UINT16)(_prVdpConf[_u4VdpImageConnectPlane]->rSrcRegion.u4Width),
                               (UINT16)(_prVdpConf[_u4VdpImageConnectPlane]->rSrcRegion.u4Height)) != 1)
    {
        return VDP_SET_ERROR;
    }
#endif
#endif

    return VDP_SET_OK;
}

/**
 * Video Image Module : Get Frame Buffer Address
 *
 * @param u4FrameBufferId: Frame Buffer id
 * @param u4YAddr: pointer to store Y address
 * @param u4CAddr: pointer to store C address
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
UINT32 _VDP_Image_GetFrameBuffer(UINT32 u4FrameBufferId, UINT32* u4YAddr, UINT32* u4CAddr)
{
    if ((u4YAddr == NULL) ||(u4CAddr == NULL))
    {
        return VDP_SET_ERROR;
    }

    // Check frame buffer id
    if (u4FrameBufferId > _rVdpImageInfo.u4FrameBufferNs)
    {
        return VDP_SET_ERROR;
    }

    *u4YAddr = _arVdpImageFb[u4FrameBufferId].u4YAddr;
    *u4CAddr = _arVdpImageFb[u4FrameBufferId].u4CAddr;

    return VDP_SET_OK;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


