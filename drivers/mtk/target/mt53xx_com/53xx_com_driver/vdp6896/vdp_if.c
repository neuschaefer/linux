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
 * $Author: jifeng.fan $
 * $Date: 2012/11/16 $
 * $RCSfile: vdp_if.c,v $
 * $Revision: #10 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdp_if.c
 *  Video Plane driver - public interface
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "vdp_if.h"
#include "vdp_drvif.h"
#include "b2r_drvif.h"
#include "b2r_if.h"
#include "drv_lvds.h"
#include "drv_di.h"
#include "drv_scaler.h"
#include "drv_contrast.h"
#include "tve_if.h"
#include "vdo_misc.h"
#include "vdec_drvif.h"
#include "mute_if.h"
#include "drv_lbox.h"
#include "source_select.h"
#ifdef CC_MT5396
#include "drv_mjc.h"
#include "drv_mjc_if.h"
#endif
#if 0 //def CC_MT5391
#include "drv_video.h"
#endif
//#include "vdp_display.h"

#ifdef DRAM_TEST_DSP_CODE
#include "drv_video.h"
#endif

#include "drv_nr.h"
#include "hw_scpos.h"

#ifdef CC_SRM_ON
#include "srm_drvif.h"
#endif

#ifdef CHANNEL_CHANGE_LOG
#include "x_timer.h"
#endif

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "drvcust_if.h"
#ifdef CC_SCPOS_EN
#include "drv_scaler.h"
#include "drv_scaler_drvif.h"
#include "drv_scpos.h"
#include "drv_display.h"
#include "panel.h"
#include "mute_if.h"
#include "nptv_if.h"
#include "pmx_drvif.h"
#include "source_table.h"
#include "drv_video.h"
#include "hal_ycproc.h"
#include "video_def.h"
#endif
#include "vdp_display.h"
#include "drv_tdc.h"
#ifdef CC_SUPPORT_TVE_OSD_OUT
#include "tve_if.h"
#endif
#include "x_os.h"
#include "x_assert.h"
#define DEFINE_IS_LOG   VDP_IsLog
#include "x_debug.h"
LINT_EXT_HEADER_END

LINT_SUPPRESS_BRACE(717)        // Info 717: do ... while(0);


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

#ifndef CC_CLI
#define CC_CLI
#endif

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//EXTERN UINT32 u4ScpipGetMinSrcRegionWidth(UINT32);
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/**
 * Video plane scaler information
 */
typedef struct
{
    UINT32 u4SrcWidth;
    UINT32 u4SrcHeight;
    UINT32 u4OutWidth;
    UINT32 u4OutHeight;
} VDP_SCALER_INFO_T;

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------
#if 0 // def CC_MT5391
EXTERN UINT16 u2MuteDelay;
#endif
UINT32 u4EnableVideoLoopMute = 0;

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

#ifdef CC_SCPOS_EN
EXTERN void vMpegInit(void);
EXTERN void vMpegInitB2rConf(void);
EXTERN UINT32 vDrvGetCurLvdsMode(void);
EXTERN void vApiVideoNR10bitMode(UINT8 bOnOff);
#endif

#ifdef DIVX_PLUS_CER
#ifdef   CC_B2R_CHAPTER_CHG
EXTERN VOID _VDP_FrcSetChapterEnable(VDP_CHAPTER_T*    prChapter);
#endif
#endif


//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

static void _VdpIfStatus(UCHAR ucVdpId);


#ifdef CC_SRM_ON
static void _VdpSrmDICb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);

static void _VdpSrmDispCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);
static void _VdpSrmTDCCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);
#ifdef CC_MT5396
static void _VdpSrmMJCCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4PDSize, UINT32 u4SramMode);
#endif
#if 0
static void _VdpSrmMuteCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);

static void _VdpSrmScalerWeCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);

static void _VdpSrmNrCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);

static void _VdpSrmMiscCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);
#endif
#endif

#ifdef CC_SCPOS_EN
static void _ScposSetEnable(UCHAR ucVdpId, UCHAR ucEnable);

#endif

static UINT32 _VDP_CheckSourceChangeCallback(UCHAR ucVdpId, VDP_SOURCE_INFO_T rVdpSrcInfo);


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static VDP_CONF_T _arVdpConfDefault[VDP_MAX_NS] =
{
    // video plane 1
    {
        0, 0, B2R_NS, VDP_STATUS_NOSIGNAL,
        VDP_MODE_BG, 0, 0, 0xa, 1024, 0,
        {MAX_ES_NS},
        0,
        //0,
        0,
        0,
        0,
        0,
        0,
        0,
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {PMX_1, VDP_B2R_MODE_UNKNOWN, 0, 60, 0, 0, 720, 480, 858, 525},
        {0, 0, 0, 0},
        VDP_MAX_REGION_WIDTH, VDP_MAX_REGION_HEIGHT,
        768, 720, 480, VDP_UNIT_ASPECT_RATIO, 60, 0, ACT_FMT_DEFAULT,
        0, 0, 0, 1, 0, 0, 0, 0,
        0x000000,
        0, 0, 0, 0,
        (HANDLE_T) NULL
    },

    // video plane 2
    {
        0, 0, B2R_NS, VDP_STATUS_NOSIGNAL,
        VDP_MODE_BG, 0, 0, 0xa, 1024, 0,
        {MAX_ES_NS},
        0,
        //0,
        0,
        0,
        0,
        0,
        0,
        0,
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {PMX_1, VDP_B2R_MODE_UNKNOWN, 0, 60, 0, 0, 720, 480, 858, 525},
        {0, 0, 0, 0},
        VDP_MAX_REGION_WIDTH/2, VDP_MAX_REGION_HEIGHT,
        768, 720, 480, VDP_UNIT_ASPECT_RATIO, 60, 0, ACT_FMT_DEFAULT,
        0, 0, 0, 1, 0, 0, 0, 0,
        0x000000,
        0, 0, 0, 0,
        (HANDLE_T) NULL
    },
};

static UCHAR _VdpInitiated = 0;

static HANDLE_T _hMutex = (HANDLE_T) NULL;            // Thread safe protection

static VDP_CONF_T _arVdpConf[VDP_NS];

VDP_CB_FUNC_T _rVdpCbFunc;

static UINT32 _u4ScalerPixelBased = 0;

static VDP_SCALER_INFO_T _arScalerInfo[VDP_NS];

#ifdef CC_SUPPORT_TVE
UCHAR _ucSameB2rVdp = VDP_NS;
#endif

#if 0 // def CC_MT5391
#if CC_SCPOS_MUTE_RELATCH_WA
UINT32 _u4MjcCbArg1 = 0xffff;
UINT32 _u4MjcCbArg2 = 0xffff;
UINT32 _u4MjcCbArg3 = 0xffff;
UINT32 _u4MjcCbArg4 = 0xffff;
UINT32 _u4MjcCbDelayCnt = 0;
#endif
#endif
UINT32 _u4MjcTVmode = 0xffff;

#ifdef CC_MT5396
UINT32 _u4VdpCmdDelay = 0;
#endif

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define VERIFY_VDP_ID(id)                                   \
          do {                                              \
            if (id >= VDP_NS) { return VDP_SET_ERROR; }     \
          } while (0)

#define VDP_MUTEX_LOCK                  (VERIFY(x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK))

#define VDP_MUTEX_UNLOCK                (VERIFY(x_sema_unlock(_hMutex) == OSR_OK))


#ifdef CC_SRM_ON
/**
 * VDP deinterlacer control call back
 *
 * @param void
 * @return void
 */
static void _VdpSrmDICb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4PDSize, UINT32 u4SramMode)
{
    LOG(3, "[SRM] DI CTRL vdp%d mode=0x%x, pds=%d, sram=%d\n", u4Arg1, u4Arg2, u4PDSize, u4SramMode);

    VDP_MutexLock();
    vDrvDISrmCb(u4Arg1, u4Arg2, u4PDSize, u4SramMode);
    VDP_MutexUnlock();
}

/**
 * VDP scaler control call back
 *
 * @param void
 * @return void
 */
static void _VdpSrmDispCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4PDSize, UINT32 u4SramMode)
{
    LOG(3, "[SRM] DISP CTRL vdp%d mode=0x%x, pds=%d, sram=%d\n", u4Arg1, u4Arg2, u4PDSize, u4SramMode);

    VDP_MutexLock();
    vDrvScpipSrmDispCb(u4Arg1, u4Arg2, u4PDSize, u4SramMode);
    VDP_MutexUnlock();
}

/**
 * VDP TDC control call back
 *
 * @param void
 * @return void
 */
static void _VdpSrmTDCCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4PDSize, UINT32 u4SramMode)
{
    LOG(3, "[SRM] TDC CTRL vdp%d mode=0x%x, pds=%d, sram=%d\n", u4Arg1, u4Arg2, u4PDSize, u4SramMode);

    VDP_MutexLock();
    vDrvTDCSrmCb(u4Arg1, u4Arg2, u4PDSize, u4SramMode);
    VDP_MutexUnlock();
}

#ifdef CC_MT5396
/**
 * VDP MJC control call back
 *
 * @param void
 * @return void
 */
static void _VdpSrmMJCCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4PDSize, UINT32 u4SramMode)
{
    LOG(3, "[SRM] MJC CTRL vdp%d mode=0x%x, pds=%d, sram=%d\n", u4Arg1, u4Arg2, u4PDSize, u4SramMode);

    VDP_MutexLock();
    vDrvMJCSrmCb(u4Arg1, u4Arg2, u4PDSize, u4SramMode);
    VDP_MutexUnlock();
}
#endif

#if 0
/**
 * VDP scaler control call back
 *
 * @param void
 * @return void
 */
static void _VdpSrmMuteCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4)
{
    LOG(3, "[SRM] MUTE CTRL vdp%d 0x%x %d %d\n", u4Arg1, u4Arg2, u4Arg3, u4Arg4);

#ifdef CC_SCPOS_EN
    if (u4Arg2 == SRM_SCPOS_MUTE_ON)
    {
        _vDrvVideoSetMute(MUTE_MODULE_SRM, u4Arg1, FOREVER_MUTE, FALSE);
    }
    else
    if (u4Arg2 == SRM_SCPOS_MUTE_OFF)
    {
        _vDrvVideoSetMute(MUTE_MODULE_SRM, u4Arg1, 0, FALSE);
    }
    else
    {
    }
#endif
}

/**
 * VDP scaler control call back
 *
 * @param void
 * @return void
 */
static void _VdpSrmScalerWeCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4)
{
    LOG(3, "[SRM] SCALER WE CTRL vdp%d 0x%x %d %d\n", u4Arg1, u4Arg2, u4Arg3, u4Arg4);

#ifdef CC_SCPOS_EN
    if (u4Arg2 == SRM_SCPOS_WE_ON)
    {
        if (VDP_ForceWriteOff((UINT8)u4Arg1, 0) == VDP_SET_OK)
        {
        }
    }
    else
    if (u4Arg2 == SRM_SCPOS_WE_OFF)
    {
        if (VDP_ForceWriteOff((UINT8)u4Arg1, 1) == VDP_SET_OK)
        {
        }
    }
    else
    {
    }
#endif
}


/**
 * VDP noise reduction control call back
 *
 * @param void
 * @return void
 */
static void _VdpSrmNrCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4PDSize, UINT32 u4SramMode)
{
    LOG(3, "[SRM] NR vdp%d mode=0x%x, pds=%d, sram=%d\n", u4Arg1, u4Arg2, u4PDSize, u4SramMode);

    VDP_MutexLock();
    vDrvNRSrmCb(u4Arg1, u4Arg2, u4PDSize, u4SramMode);
    VDP_MutexUnlock();
}

/**
 * VDP misc control call back
 *
 * @param void
 * @return void
 */
static void _VdpSrmMiscCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4)
{
    LOG(3, "[SRM] MISC CTRL vdp%d 0x%x %d %d\n", u4Arg1, u4Arg2, u4Arg3, u4Arg4);

    if (u4Arg2 == SRM_MISC_SCALER_CHANGE)
    {
        vScpipReconfig((UCHAR) u4Arg1);
    }
}
#endif
#endif

#ifdef CC_SCPOS_EN
/**
 * VDP video backend enable/disable
 *
 * @param ucVdpId specify the video plane id.
 * @param ucEnable specify backend enable/disable.
 * @return void
 */
static void _ScposSetEnable(UCHAR ucVdpId, UCHAR ucEnable)
{
    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    //_VDP_ScposEnable(ucVdpId, ucEnable);
    if (_fgAutoSearch)  // scan channel case
    {
        vScpipDelayEnable(ucVdpId, ucEnable, 16);//VDP_VD_DELAY_COUNT);
    }
    else
    {
        vScpipDelayEnable(ucVdpId, ucEnable, VDP_VD_DELAY_COUNT);
    }

    vDrvVideoFixedPreScaler(ucVdpId, CC_VDP_FIXED_PRESCALER, 1);

    if (ucEnable)
    {
#ifdef CC_SRM_ON
        SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_OUT_SIZE + (UINT32)ucVdpId)
            , _arVdpConf[ucVdpId].rOutRegion.u4Width
            , _arVdpConf[ucVdpId].rOutRegion.u4Height);
            //printf("add debu log _arVdpConf[ucVdpId].rOutRegion.u4Width %d at %d  %s\n",_arVdpConf[ucVdpId].rOutRegion.u4Width,__LINE__,__FUNCTION__);
#endif

        if (bApiVideoSetDispRegion(ucVdpId,
            _arVdpConf[ucVdpId].rOutRegion.u4X,
            _arVdpConf[ucVdpId].rOutRegion.u4Y,
            _arVdpConf[ucVdpId].rOutRegion.u4Width,
            _arVdpConf[ucVdpId].rOutRegion.u4Height) != 1)
        {
        }
		//printf("add debu log _arVdpConf[ucVdpId].rOutRegion.u4Width %d at %d  %s\n",_arVdpConf[ucVdpId].rOutRegion.u4Width,__LINE__,__FUNCTION__);

        if (bApiVideoSetSrcRegion(ucVdpId,
            _arVdpConf[ucVdpId].rSrcRegion.u4X,
            _arVdpConf[ucVdpId].rSrcRegion.u4Y,
            _arVdpConf[ucVdpId].rSrcRegion.u4Width,
            _arVdpConf[ucVdpId].rSrcRegion.u4Height) != 1)
        {
        }
    }
}
#endif

/**
 * Show video plane status
 *
 * @param ucVdpId specify the video plane id.
 * @return void
 */
static void _VdpIfStatus(UCHAR ucVdpId)
{
    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    LOG(1, "VDP(%d) E(%d) DTV(%d, %d) M(%d) O(%d) ES(%d) Bg(%x)\n",
        ucVdpId,
        _arVdpConf[ucVdpId].ucVdpEnable,
        _arVdpConf[ucVdpId].ucB2rId,
        _arVdpConf[ucVdpId].ucEnable,
        //_arVdpConf[ucVdpId].ucMode,
        //_arVdpConf[ucVdpId].ucDispMode,
        u1ScpipGetDispMode(ucVdpId),
        _arVdpConf[ucVdpId].rOutInfo.ucPmxMode,
        _arVdpConf[ucVdpId].ucInputPort[0],
        _arVdpConf[ucVdpId].u4BgColor);

    LOG(1, "  --SRCR(%d, %d, %d, %d) OUTR(%d %d %d %d) OVERSCAN(%d %d %d %d)\n",
        _arVdpConf[ucVdpId].rSrcRegion.u4X,
        _arVdpConf[ucVdpId].rSrcRegion.u4Y,
        _arVdpConf[ucVdpId].rSrcRegion.u4Width,
        _arVdpConf[ucVdpId].rSrcRegion.u4Height,
        _arVdpConf[ucVdpId].rOutRegion.u4X,
        _arVdpConf[ucVdpId].rOutRegion.u4Y,
        _arVdpConf[ucVdpId].rOutRegion.u4Width,
        _arVdpConf[ucVdpId].rOutRegion.u4Height,
        _arVdpConf[ucVdpId].rOverScan.u4Top,
        _arVdpConf[ucVdpId].rOverScan.u4Bottom,
        _arVdpConf[ucVdpId].rOverScan.u4Left,
        _arVdpConf[ucVdpId].rOverScan.u4Right);
    LOG(1, "  --Src(W:%d H:%d Fr:%d LineSize:%d) FrmCrop(T:%d B:%d) Afd(Afd:%d AR:%d)\n",
        _arVdpConf[ucVdpId].u4SrcWidth,
        _arVdpConf[ucVdpId].u4SrcHeight,
        _arVdpConf[ucVdpId].u4FrameRate,
        _arVdpConf[ucVdpId].u4SrcLineSize,
        _arVdpConf[ucVdpId].u4FrmCropTop,
        _arVdpConf[ucVdpId].u4FrmCropBtm,
        _arVdpConf[ucVdpId].u4Afd,
        _arVdpConf[ucVdpId].u4AspectRatio);
    LOG(1, "  --Total(%d %d) Active(%d %d) Fr(%d) In-Fr(%d) Prg(%d) DoubleClk(%d) ClkMode(%d)\n",
        _arVdpConf[ucVdpId].rOutInfo.u4TotalWidth,
        _arVdpConf[ucVdpId].rOutInfo.u4TotalHeight,
        _arVdpConf[ucVdpId].rOutInfo.u4Width,
        _arVdpConf[ucVdpId].rOutInfo.u4Height,
        _arVdpConf[ucVdpId].rOutInfo.ucFrameRate,
        _arVdpConf[ucVdpId].rOutInfo.ucInputFrameRate,
        _arVdpConf[ucVdpId].rOutInfo.ucPrg,
        _arVdpConf[ucVdpId].rOutInfo.ucDoubleClk,
        _arVdpConf[ucVdpId].rOutInfo.u4ClkMode);

    /*LOG(1, "PAUSE(%d), FREEZE(%d), TRICK(%d)\n",
        _arVdpConf[ucVdpId].ucPauseMm,
        _arVdpConf[ucVdpId].ucFreeze,
        VDP_GetTrickMode());
    */
#ifdef CC_SCPOS_EN
   vScpipPrintCurrentScale(ucVdpId);
#endif
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

/**
 * VDP init function.
 *
 * @param void
 * @return void
 */
void VDP_Init(void)
{
    if (_VdpInitiated == 0)
    {
        UINT32 u4EsId;
        UINT32 u4VdpId;
        #ifdef CC_SRM_ON
        UINT32 u4SRMPause;
        #endif
       // INT32 i4VisIdx;

        _VdpInitiated = 1;

  //      _u4ScalerPixelBased = 0;

        // create semaphore
        VERIFY(x_sema_create(&_hMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);

        FBM_Init();
        B2R_InitVdpConf();

        // clear ES to FBG Table
        for (u4EsId = 0; u4EsId < MAX_ES_NS; u4EsId++)
        {
            B2R_SetEs2Fbg(u4EsId, FBM_FBG_ID_UNKNOWN);
        }

        FBM_RegCbFunc(FBM_CB_FUNC_FBG_CHG_IND, (UINT32)_VdpFbgChgNotify);

        // dirty code to fit _VDP_VsyncInit
        #if defined(CC_MT8223)
        u4VdpId = 0;
        #else
        for (u4VdpId = 0; u4VdpId < VDP_NS; u4VdpId++)
        #endif
        {
            _arVdpConf[u4VdpId].hMutex = _hMutex;

            _arScalerInfo[u4VdpId].u4SrcWidth = 0;
            _arScalerInfo[u4VdpId].u4SrcHeight = 0;
            _arScalerInfo[u4VdpId].u4OutWidth = 0;
            _arScalerInfo[u4VdpId].u4OutHeight = 0;

            B2R_SetImgConnect((UCHAR)u4VdpId, 0);
        }

        VDP_VsyncInit();

        VDP_Image_Init();

        #if defined(CC_MT8223)
        u4VdpId = 0;
        #else
        for (u4VdpId = 0; u4VdpId < VDP_NS; u4VdpId++)
        #endif
        {
            VDP_Reset(u4VdpId);

#ifdef CC_SCPOS_EN
            vMpegInit();
            _vDrvVideoSetMute(MUTE_MODULE_SCPOS_PLA_EN, u4VdpId, FOREVER_MUTE, FALSE);
           // vApiVideoSetBg(u4VdpId, _arVdpConf[u4VdpId].u4BgColor);
            vDrvVideoSetMuteColor(u4VdpId, _arVdpConf[u4VdpId].u4BgColor);
            vScpipDelayEnable(u4VdpId, 0, 0);

            vDrvVideoFixedPreScaler(u4VdpId, CC_VDP_FIXED_PRESCALER, 1);
            vDrvVideoFixedPreScaler(u4VdpId, CC_VDP_FIXED_PRESCALER, 1);
#endif

          // Init Mute
            _vDrvVideoSetMute(MUTE_MODULE_MODECHG, u4VdpId, FOREVER_MUTE, FALSE);
            VDP_VsyncSendCmd(u4VdpId, VDP_CMD_SET_MODE);
        }

#ifdef CC_SRM_ON
        u4SRMPause = SRM_IsPauseMode();

        if (u4SRMPause)
        {
            // SRM is init in PAUSE state by default
        }
        else
        {
            // Pause SRM
           SRM_Pause(); 
        }

        // Reg SRM Callback Function
        SRM_RegisterCB(SRM_BOB_CTRL, _VdpSrmDICb);
        //SRM_RegisterCB(SRM_NR_CTRL, _VdpSrmNrCb);
        SRM_RegisterCB(SRM_DISP_CTRL, _VdpSrmDispCb);
        SRM_RegisterCB(SRM_TDC_CTRL, _VdpSrmTDCCb);
        //SRM_RegisterCB(SRM_MUTE_CTRL, _VdpSrmMuteCb);
        //SRM_RegisterCB(SRM_SCALER_WE_CTRL, _VdpSrmScalerWeCb);
        //SRM_RegisterCB(SRM_MISC_CTRL, _VdpSrmMiscCb);
        #ifdef CC_MT5396
        SRM_RegisterCB(SRM_MJC_CTRL, _VdpSrmMJCCb);
        #endif

        // Init SRM Configuration
        #if defined(CC_MT8223)
        u4VdpId = 0;
        #else
        for (u4VdpId = 0; u4VdpId < VDP_NS; u4VdpId++)
        #endif
        {
			LINT_SUPPRESS_BRACE(641)        // Warning 641: Converting enum 'VSS_MAJOR_TYPE' to int

            SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_ONOFF + u4VdpId), 0, 0);
            SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_SOURCE + u4VdpId), (UINT32) SV_VS_MAX, 0);
            SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_MPEG_SIZE + u4VdpId), 0, 0);
            SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_OUT_SIZE + u4VdpId), 0, 0);
        }
		
		SRM_Resume();	//add by hongjun.chu
#if 0
        if (u4SRMPause)
        {
            // Postpone Resume until NPTV main loop is created
        }
        else
        {
            // Resume SRM
           SRM_Resume();   
        }
#endif

#endif
    }
}

/**
 * VDP stop function.
 *
 * @param void
 * @return void
 */
void VDP_Stop(void)
{
    if (_VdpInitiated == 1)
    {
        _VdpInitiated = 0;

        FBM_RegCbFunc(FBM_CB_FUNC_FBG_CHG_IND, (UINT32)NULL);
        FBM_RegCbFunc(FBM_CB_FUNC_PICSTRUCT_CHG_IND, (UINT32)NULL);

        // delete semaphore
        VERIFY(x_sema_delete(_hMutex) == OSR_OK);
        _hMutex = (HANDLE_T) NULL;
    }
}

/**
 * VDP reset function.
 *
 * @param ucVdpId specify the video plane id.
 * @return void
 */
void VDP_Reset(UCHAR ucVdpId)
{
    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    VDP_MUTEX_LOCK;

    _arVdpConf[ucVdpId] = _arVdpConfDefault[ucVdpId];
    #if SUPPORT_POP
    if(ucVdpId == VDP_2)
    {
        _arVdpConf[ucVdpId].u4MaxOutWidth = 960*VDP_MAX_REGION_WIDTH/wDISPLAY_WIDTH;
    }
    #endif
    _arVdpConf[ucVdpId].hMutex = _hMutex;
    _VDP_VsyncReset(ucVdpId);

    // set software default value to lower layer
    VDP_VsyncSendCmd(ucVdpId, VDP_CMD_SET_MODE);
    VDP_VsyncSendCmd(ucVdpId, VDP_CMD_SET_INPUT);
    VDP_VsyncSendCmd(ucVdpId, VDP_CMD_SET_OUTPUT);
    VDP_VsyncSendCmd(ucVdpId, VDP_CMD_SET_SRCR);
    VDP_VsyncSendCmd(ucVdpId, VDP_CMD_SET_OUTR);
    VDP_VsyncSendCmd(ucVdpId, VDP_CMD_SET_BG);

    VDP_MUTEX_UNLOCK;
}

/**
 * VDP query function, show status in CLI with debug level 1.
 *
 * @param void
 * @return void
 */
void VDP_QueryStatus(void)
{
    UINT32 u4VdpId;

    for (u4VdpId = 0; u4VdpId < VDP_NS; u4VdpId++)
    {
        _VdpIfStatus(u4VdpId);
    }
    //SRM_QueryStatus();
}

/**
  * VDP set video plane enable.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucEnable value 0:disable, 1:enable.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetEnable(UCHAR ucVdpId, UCHAR ucEnable)
{
    VERIFY_VDP_ID(ucVdpId);

    #ifdef CHANNEL_CHANGE_LOG
    HAL_TIME_T dt;
    if (ucEnable != 0)
    {
        HAL_GetTime(&dt);
        LOG(0, " %u.%06u s [AV SYNC] 7 VDP Enable VDP(%d) On(%d)\n", dt.u4Seconds, dt.u4Micros, ucVdpId, ucEnable);
    }
    #endif

    if (_arVdpConf[ucVdpId].ucVdpEnable == ucEnable)
    {
        return VDP_SET_OK;
    }

    VDP_MUTEX_LOCK;

#ifdef CC_SRM_ON
    SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_ONOFF + (UINT32)ucVdpId), (UINT32)ucEnable, 0);
#endif

#ifdef CC_SCPOS_EN
    if (_arVdpConf[ucVdpId].ucVdpEnable != ucEnable)
    {
        // PIP/POP Mode Change
        vDrvVideoFixedPreScaler(VDP_1, CC_VDP_FIXED_PRESCALER, 1);
        #if SUPPORT_POP
        vDrvVideoFixedPreScaler(VDP_2, CC_VDP_FIXED_PRESCALER, 1);
        #endif
    }

    if (ucEnable == 0)
    {
#ifdef CC_SUPPORT_TVE
        if(_ucSameB2rVdp == VDP_NS)
        {
            _VDP_StatusNofify(ucVdpId, VDP_B2R_NO_SIGNAL);
            LOG(3, "Mute VDP(%d)\n", ucVdpId);
        }
#else
        _VDP_StatusNofify(ucVdpId, VDP_B2R_NO_SIGNAL);
        LOG(3, "Mute VDP(%d)\n", ucVdpId);
#endif

#if 0
#ifdef CC_SRM_ON
        SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_MPEG_SIZE + (UINT32)ucVdpId),
            0, 0);
#endif
#endif
    }
    else
    {
        if ((B2R_GetImgConnect(ucVdpId)!= 0) &&
            (_arVdpConf[ucVdpId].ucStatus == VDP_STATUS_NOSIGNAL))
        {
            _VDP_StatusNofify(ucVdpId, VDP_B2R_START_PLAY);
        }
    }
#endif

    _arVdpConf[ucVdpId].ucVdpEnable = ucEnable;
    if (!ucEnable)
    {
    #ifdef CC_SCPOS_EN
        _ScposSetEnable(ucVdpId, ucEnable);
    #endif
    }

    _B2rSetEnable(ucVdpId);

#ifdef CC_SCPOS_EN
    if ((_arVdpConf[ucVdpId].ucMode != VDP_MODE_BG) &&
        (_arVdpConf[ucVdpId].ucVdpEnable))
    {
        // only unmute when vdp is enable,
        // else you will see some garbage frame buffer
            _vDrvVideoSetMute(MUTE_MODULE_API_FORCE, ucVdpId, 0, FALSE);
    }

    // Signal status return to SV_VDO_NOSIGNAL when enable >> disable or disable >> enable
    if (ucEnable)
    {
        _ScposSetEnable(ucVdpId, ucEnable);
    }
#endif

    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;
}

/**
 * VDP query video plane enable/disable status.
 *
 * @param ucVdpId specify the video plane id.
 * @param pucEnable value 0:disable, 1:enable.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_GetEnable(UCHAR ucVdpId, UCHAR* pucEnable)
{
    VERIFY_VDP_ID(ucVdpId);
    VERIFY_NULL(pucEnable);

    *pucEnable = _arVdpConf[ucVdpId].ucVdpEnable;

    return VDP_SET_OK;
}

/**
 * VDP set video plane mode.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucMode VDP_MODE_NORMAL or VDP_MODE_BG.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetMode(UCHAR ucVdpId, UCHAR ucMode)
{
    VERIFY_VDP_ID(ucVdpId);

    if (ucMode >= VDP_MODE_UNKNOWN)
    {
        return VDP_SET_ERROR;
    }
	u4EnableVideoLoopMute = 1;

    if (ucMode == VDP_MODE_DEINT)
    {	// VDP_MODE_DEINT is the same as VDP_MODE_NORMAL
        ucMode = VDP_MODE_NORMAL;
    }

    if (_arVdpConf[ucVdpId].ucMode == ucMode)
    {
        return VDP_SET_OK;
    }

    if (ucMode == VDP_MODE_QV)
    {
        return VDP_SET_ERROR;
    }

    VDP_MUTEX_LOCK;

#ifdef CC_SCPOS_EN
    if (ucMode == VDP_MODE_BG)
    {
            _vDrvVideoSetMute(MUTE_MODULE_API_FORCE, ucVdpId, FOREVER_MUTE, FALSE);
    }
    else if (_arVdpConf[ucVdpId].ucVdpEnable)
    {
        // CR102740, ask SCPOS to mute, DTV only
        // DTV00112202 VGA, DTV00112364 Scart , need to dealy unmute
       // if (_arVdpConf[ucVdpId].ucB2rId < B2R_NS)
        {
       // 5387: DTV00135447   if AP call SetMode(VDP_MODE_NORMAL) too late, it will cause driver flash black mute.
       //     _VDP_SetScposDelayUnMute(ucVdpId, 10, 1);
        }

        // only unmute when vdp is enable,
        // else you will see some garbage frame buffer
       _vDrvVideoSetMute(MUTE_MODULE_API_FORCE, ucVdpId, 15, FALSE);
    }
#endif

    _arVdpConf[ucVdpId].ucMode = ucMode;
    VDP_VsyncSendCmd(ucVdpId, VDP_CMD_SET_MODE);

    //for CR DTV00139325
    //_VdpCheckFbgReady();

    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;
}

/**
 * VDP query video plane mode.
 *
 * @param ucVdpId specify the video plane id.
 * @param pucMode VDP_MODE_NORMAL or VDP_MODE_BG.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_GetMode(UCHAR ucVdpId, UCHAR* pucMode)
{
    VERIFY_VDP_ID(ucVdpId);
    VERIFY_NULL(pucMode);

    *pucMode = _arVdpConf[ucVdpId].ucMode;

    return VDP_SET_OK;
}

/**
 * VDP set video plane with freeze feature.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucFreeze value 0:disable, 1:freeze feature enable.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetFreeze(UCHAR ucVdpId, UCHAR ucFreeze)
{
    UINT32 ret = VDP_SET_OK;
    #ifdef CC_SCPOS_EN
    SRM_FREEZE_MODE_T eFreezeMode = SRM_FREEZE_MODE_SCPOS;
    static SRM_FREEZE_MODE_T aeLastFreezeMode[VDP_NS];
    #endif

    VERIFY_VDP_ID(ucVdpId);
    vDrvAutoContrastFreeze(ucFreeze);

    VDP_MUTEX_LOCK;

    if (_arVdpConf[ucVdpId].ucFreeze != ucFreeze)
    {
        _arVdpConf[ucVdpId].ucFreeze = ucFreeze;

        // do freeze here
        #ifdef CC_SCPOS_EN
        if (ucFreeze != 0)
        {
            eFreezeMode = SRM_GetFreezeMode((UINT32)ucVdpId);
            aeLastFreezeMode[ucVdpId] = eFreezeMode;
        }
        else
        {
            eFreezeMode = aeLastFreezeMode[ucVdpId];
        }

        if (eFreezeMode == SRM_FREEZE_MODE_SCPOS)
        {
            if ((ucFreeze == 0) || (u1ScpipIsVideoFreezable(ucVdpId) == SV_TRUE))
            {
                vScpipSetFreeze(ucVdpId, ucFreeze);
	        }
            else
            {
                // can't not support freeze function
                _arVdpConf[ucVdpId].ucFreeze = 0;   // 0 means video is not in freeze mode
                ret = VDP_SET_ERROR;
            }
        }
        #ifdef CC_MT5396
        else if(eFreezeMode == SRM_FREEZE_MODE_MJC)
        {
        	vDrvMJCSetFrmFreeze(ucFreeze);
			if(VDP_SCPOS_DISPMODE_OFF == u1ScpipGetDispMode(ucVdpId))
		    {
			    vScpipSetFreeze(ucVdpId, ucFreeze);
				LOG(1,"+++++++vScpipSetFreeze(ucVdpId, ucFreeze)\n");
			}
			else if(VDP_SCPOS_PSCAN_DISPMODE == u1ScpipGetDispMode(ucVdpId) ||
                   (VDP_SCPOS_LINESYNC_DISPMODE == u1ScpipGetDispMode(ucVdpId) &&
                    bGetSignalType(ucVdpId) != SV_ST_VGA  && !SRM_IsGameMode(ucVdpId)))
			{
                vDrvDIFreezeOnOff(ucVdpId, ucFreeze);
				
				LOG(1,"+++++++vDrvDIFreezeOnOff(ucVdpId, ucFreeze)\n");
			}
			else if((SV_VD_MPEGHD ==  bGetVideoDecType(ucVdpId)) ||(SV_VD_MPEGSD ==  bGetVideoDecType(ucVdpId)))
		    {
		        VDP_FrcSetFreeze(ucVdpId, ucFreeze);
				
				LOG(1,"+++++++VDP_FrcSetFreeze(ucVdpId, ucFreeze)\n");
			}
        }
        #endif
		else if (eFreezeMode == SRM_FREEZE_MODE_MDDI)
		{
			vDrvDIFreezeOnOff(ucVdpId, ucFreeze);
		}
        else if (eFreezeMode == SRM_FREEZE_MODE_MPEG)
        {
            VDP_FrcSetFreeze(ucVdpId, ucFreeze);
        }
        else
        {
            ASSERT(0);
        }
        #endif
    }

    VDP_MUTEX_UNLOCK;

    LOG(3, "VDP_SetFreeze: path=%d, freeze=%d, mode=%d\n", ucVdpId, ucFreeze, eFreezeMode);

    return ret;
}

/**
 * VDP query video plane with freeze feature.
 *
 * @param ucVdpId specify the video plane id.
 * @param pucFreeze value 0:disable, 1:freeze feature enable.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_GetFreeze(UCHAR ucVdpId, UCHAR* pucFreeze)
{
    VERIFY_VDP_ID(ucVdpId);
    VERIFY_NULL(pucFreeze);

    *pucFreeze = _arVdpConf[ucVdpId].ucFreeze;

    return VDP_SET_OK;
}


/**
 * VDP query video plane with freeze feature.
 *
 * @param ucVdpId specify the video plane id.
 * @param pucFreezable value 0: can't freeze, 1:freeze feature is available.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_GetFreezable(UCHAR ucVdpId, UCHAR* pucFreezable)
{
    VERIFY_VDP_ID(ucVdpId);
    VERIFY_NULL(pucFreezable);

    *pucFreezable = SV_TRUE;

    if (SRM_GetFreezeMode((UINT32)ucVdpId) == SRM_FREEZE_MODE_SCPOS)
    {
        if (u1ScpipIsVideoFreezable((UINT8)ucVdpId) == SV_FALSE)
        {
            *pucFreezable = SV_FALSE;
        }
    }
	else if (SRM_GetFreezeMode((UINT32)ucVdpId) == SRM_FREEZE_MODE_MDDI)
	{
		if (u1DrvDIFreezable((UINT8)ucVdpId) == SV_FALSE)
		{
			*pucFreezable = SV_FALSE;
		}
	}

    return VDP_SET_OK;
}

/**
 * VDP query if overscan is supported.
 *
 * @param ucVdpId specify the video plane id.
 * @return TRUE/FALSE
 */
BOOL VDP_IsSupportOverscan(UCHAR ucVdpId)
{
    // this function return value will be used to determine whether AP is able to
    // set source region, output region, overscan,

    BOOL fgSupported = SV_TRUE;    
    #if defined(CC_SCPOS_3DTV_SUPPORT)    
    #ifdef DRV_USE_EXTERNAL_3D_FRC    
    if(u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF)
    {
        fgSupported = SV_FALSE;
    }
    #endif
    #endif
    
    return fgSupported;
}

/**
 * VDP query if aspect ratio is supported.
 *
 * @param ucVdpId specify the video plane id.
 * @return TRUE/FALSE
 */
BOOL VDP_IsSupportAspectRatio(UCHAR ucVdpId)
{
    // this function return value will be used to determine whether AP is able to
    // enable aspect ratio

    BOOL fgSupported = SV_TRUE;    
    #if 0//defined(CC_SCPOS_3DTV_SUPPORT)    
    #ifdef DRV_USE_EXTERNAL_3D_FRC    
    if(u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF)
    {
        fgSupported = SV_FALSE;
    }
    #endif
    #endif
    SCALER_TDTV_IN_TYPE in;
    in = eScpipGetTdtvInType(ucVdpId);
    if((vScpipIs3DDoneByScaler(ucVdpId)==SV_TRUE) &&(in == SCALER_TDTV_IN_TYPE_LI))
    {
      fgSupported = SV_FALSE;
    }
    LOG(3,"in VDP_IsSupportAspectRatio and fgSupported(%d)\n",fgSupported);
    return fgSupported;
}

/* * VDP set overscan region.
 *
 * @param ucVdpId specify the video plane id.
 * @param rRegion overscan percentage setting.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetOverScan(UCHAR ucVdpId, VDP_OVERSCAN_REGION_T rRegion)
{
    VERIFY_VDP_ID(ucVdpId);

    if (_u4ScalerPixelBased != 0)
    {
        // no overscan feature when scaler is pixel based
        return VDP_SET_OK;
    }

    if(VDP_IsSupportOverscan(ucVdpId)!=1)
    {
	    rRegion.u4Top =0;
	    rRegion.u4Bottom =0;
	    rRegion.u4Left =0; 
	    rRegion.u4Right =0;
    }

    if ((_arVdpConf[ucVdpId].rOverScan.u4Top == rRegion.u4Top) &&
        (_arVdpConf[ucVdpId].rOverScan.u4Bottom == rRegion.u4Bottom) &&
        (_arVdpConf[ucVdpId].rOverScan.u4Left == rRegion.u4Left) &&
        (_arVdpConf[ucVdpId].rOverScan.u4Right == rRegion.u4Right))
    {
        // nothing change

        return VDP_SET_OK;
    }

    _arVdpConf[ucVdpId].rOverScan = rRegion;
    
    VDP_XdataSetOverscan((UINT32)ucVdpId, _arVdpConf[ucVdpId].rOverScan);

#ifdef CC_SCPOS_EN
    if (bApiVideoSetSrcOverScan(ucVdpId,
        _arVdpConf[ucVdpId].rOverScan.u4Top,
        _arVdpConf[ucVdpId].rOverScan.u4Bottom,
        _arVdpConf[ucVdpId].rOverScan.u4Left,
        _arVdpConf[ucVdpId].rOverScan.u4Right) != 1)
    {
        return VDP_SET_ERROR;
    }
#endif

    return VDP_SET_OK;
}

/**
 * VDP query overscan region.
 *
 * @param ucVdpId specify the video plane id.
 * @param prRegion overscan percentage setting.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_GetOverScan(UCHAR ucVdpId, VDP_OVERSCAN_REGION_T* prRegion)
{
    VERIFY_VDP_ID(ucVdpId);
    VERIFY_NULL(prRegion);

    *prRegion = _arVdpConf[ucVdpId].rOverScan;

    return VDP_SET_OK;
}

/**
 * VDP set source region.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucSrcFullRegion value 1:full region and ignore parameter rSrcRegion.
 * @param rSrcRegion source region description.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetSrcRegion(UCHAR ucVdpId, UCHAR ucSrcFullRegion, VDP_REGION_T rSrcRegion)
{
    VERIFY_VDP_ID(ucVdpId);

    VDP_MUTEX_LOCK;

    if (ucSrcFullRegion)
    {
        if (_u4ScalerPixelBased != 0)
        {
            if ((_arScalerInfo[ucVdpId].u4SrcWidth != 0) &&
                (_arScalerInfo[ucVdpId].u4SrcHeight != 0))
            {
                if ((_arVdpConf[ucVdpId].rSrcRegion.u4X == 0) &&
                    (_arVdpConf[ucVdpId].rSrcRegion.u4Y == 0) &&
                    (_arVdpConf[ucVdpId].rSrcRegion.u4Width == _arScalerInfo[ucVdpId].u4SrcWidth) &&
                    (_arVdpConf[ucVdpId].rSrcRegion.u4Height == _arScalerInfo[ucVdpId].u4SrcHeight))
                {
                    // nothing change

                    VDP_MUTEX_UNLOCK;
                    return VDP_SET_OK;
                }

                _arVdpConf[ucVdpId].rSrcRegion.u4X = 0;
                _arVdpConf[ucVdpId].rSrcRegion.u4Y = 0;
                _arVdpConf[ucVdpId].rSrcRegion.u4Width = _arScalerInfo[ucVdpId].u4SrcWidth;
                _arVdpConf[ucVdpId].rSrcRegion.u4Height = _arScalerInfo[ucVdpId].u4SrcHeight;
            }
        }
        else
        {
            if ((_arVdpConf[ucVdpId].rSrcRegion.u4X == 0) &&
                (_arVdpConf[ucVdpId].rSrcRegion.u4Y == 0) &&
                (_arVdpConf[ucVdpId].rSrcRegion.u4Width == VDP_MAX_REGION_WIDTH) &&
                (_arVdpConf[ucVdpId].rSrcRegion.u4Height == VDP_MAX_REGION_HEIGHT))
            {
                // nothing change

                VDP_MUTEX_UNLOCK;
                return VDP_SET_OK;
            }

            _arVdpConf[ucVdpId].rSrcRegion.u4X = 0;
            _arVdpConf[ucVdpId].rSrcRegion.u4Y = 0;
#ifdef CC_SCPOS_EN
            _arVdpConf[ucVdpId].rSrcRegion.u4Width = VDP_MAX_REGION_WIDTH;
            _arVdpConf[ucVdpId].rSrcRegion.u4Height = VDP_MAX_REGION_HEIGHT;
#else
            _arVdpConf[ucVdpId].rSrcRegion.u4Width = _arVdpConf[ucVdpId].u4SrcWidth;
            _arVdpConf[ucVdpId].rSrcRegion.u4Height = _arVdpConf[ucVdpId].u4SrcHeight;
#endif
        }

    }
    else
    {
        if ((_u4ScalerPixelBased == 0) &&
            (_arVdpConf[ucVdpId].rSrcRegion.u4X == rSrcRegion.u4X) &&
            (_arVdpConf[ucVdpId].rSrcRegion.u4Y == rSrcRegion.u4Y) &&
            (_arVdpConf[ucVdpId].rSrcRegion.u4Width == rSrcRegion.u4Width) &&
            (_arVdpConf[ucVdpId].rSrcRegion.u4Height == rSrcRegion.u4Height))
        {
            // nothing change

            VDP_MUTEX_UNLOCK;
            return VDP_SET_OK;
        }

        _arVdpConf[ucVdpId].rSrcRegion = rSrcRegion;
    }

    VDP_XdataSetSrcRegion((UINT32)ucVdpId, _arVdpConf[ucVdpId].rSrcRegion);

#ifdef CC_SCPOS_EN
    if (bApiVideoSetSrcRegion(ucVdpId,
        _arVdpConf[ucVdpId].rSrcRegion.u4X,
        _arVdpConf[ucVdpId].rSrcRegion.u4Y,
        _arVdpConf[ucVdpId].rSrcRegion.u4Width,
        _arVdpConf[ucVdpId].rSrcRegion.u4Height) != 1)
    {
        VDP_MUTEX_UNLOCK;

        return VDP_SET_ERROR;
    }

    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;
#else

    VDP_VsyncSendCmd(ucVdpId, VDP_CMD_SET_SRCR);

    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;
#endif
}

/**
 * VDP query source region.
 *
 * @param ucVdpId specify the video plane id.
 * @param pucSrcFullRegion value 1:full region and ignore parameter rSrcRegion.
 * @param prSrcRegion source region description.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_GetSrcRegion(UCHAR ucVdpId, UCHAR* pucSrcFullRegion, VDP_REGION_T* prSrcRegion)
{
    VERIFY_VDP_ID(ucVdpId);
    VERIFY_NULL(pucSrcFullRegion);
    VERIFY_NULL(prSrcRegion);

    VDP_MUTEX_LOCK;
    *pucSrcFullRegion = 0;
    *prSrcRegion = _arVdpConf[ucVdpId].rSrcRegion;
    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;
}

/**
 * VDP set output region.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucOutFullRegion value 1:full region and ignore parameter rOutRegion.
 * @param rOutRegion output region description.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetOutRegion(UCHAR ucVdpId, UCHAR ucOutFullRegion, VDP_REGION_T rOutRegion)
{
    VERIFY_VDP_ID(ucVdpId);
	//printf("VDP_SetOutRegion() rOutRegion %d, LINE = %d \n", rOutRegion.u4Width,__LINE__);

    VDP_MUTEX_LOCK;
    if (ucOutFullRegion)
    {
    	printf("VDP_SetOutRegion(), LINE = %d \n", __LINE__);
        if (_u4ScalerPixelBased != 0)
        {
        
            if ((_arScalerInfo[ucVdpId].u4OutWidth != 0) &&
                (_arScalerInfo[ucVdpId].u4OutHeight != 0))
            {
                if ((_arVdpConf[ucVdpId].rOutRegion.u4X == 0) &&
                    (_arVdpConf[ucVdpId].rOutRegion.u4Y == 0) &&
                    (_arVdpConf[ucVdpId].rOutRegion.u4Width == _arScalerInfo[ucVdpId].u4OutWidth) &&
                    (_arVdpConf[ucVdpId].rOutRegion.u4Height == _arScalerInfo[ucVdpId].u4OutHeight))
                {
                    // nothing change
					//printf("add debu log _arVdpConf[ucVdpId].rOutRegion.u4Width %d at %d  %s\n",_arVdpConf[ucVdpId].rOutRegion.u4Width,__LINE__,__FUNCTION__);

                    VDP_MUTEX_UNLOCK;
                    return VDP_SET_OK;
                }

                _arVdpConf[ucVdpId].rOutRegion.u4X = 0;
                _arVdpConf[ucVdpId].rOutRegion.u4Y = 0;
                _arVdpConf[ucVdpId].rOutRegion.u4Width = _arScalerInfo[ucVdpId].u4OutWidth;
                _arVdpConf[ucVdpId].rOutRegion.u4Height = _arScalerInfo[ucVdpId].u4OutHeight;
                
				//printf("add debu log _arVdpConf[ucVdpId].rOutRegion.u4Width %d at %d  %s\n",_arVdpConf[ucVdpId].rOutRegion.u4Width,__LINE__,__FUNCTION__);
            }
        }
        else
        {
        
            if ((_arVdpConf[ucVdpId].rOutRegion.u4X == 0) &&
                (_arVdpConf[ucVdpId].rOutRegion.u4Y == 0) &&
                (_arVdpConf[ucVdpId].rOutRegion.u4Width == VDP_MAX_REGION_WIDTH) &&
                (_arVdpConf[ucVdpId].rOutRegion.u4Height == VDP_MAX_REGION_HEIGHT))
            {
                // nothing change

                VDP_MUTEX_UNLOCK;
                return VDP_SET_OK;
            }

            //// TODO, Output region = Panel Size
            _arVdpConf[ucVdpId].rOutRegion.u4X = 0;
            _arVdpConf[ucVdpId].rOutRegion.u4Y = 0;
#ifdef CC_SCPOS_EN
            _arVdpConf[ucVdpId].rOutRegion.u4Width = VDP_MAX_REGION_WIDTH;
            _arVdpConf[ucVdpId].rOutRegion.u4Height = VDP_MAX_REGION_HEIGHT;
#else
            _arVdpConf[ucVdpId].rOutRegion.u4Width = _arVdpConf[ucVdpId].rOutInfo.u4Width;
            _arVdpConf[ucVdpId].rOutRegion.u4Height =_arVdpConf[ucVdpId].rOutInfo.u4Height;
#endif
		//printf("add debu log _arVdpConf[ucVdpId].rOutRegion.u4Width %d at %d  %s\n",_arVdpConf[ucVdpId].rOutRegion.u4Width,__LINE__,__FUNCTION__);

        }
    }
    else
    {
    
        if ((_arVdpConf[ucVdpId].rOutRegion.u4X == rOutRegion.u4X) &&
            (_arVdpConf[ucVdpId].rOutRegion.u4Y == rOutRegion.u4Y) &&
            (_arVdpConf[ucVdpId].rOutRegion.u4Width == rOutRegion.u4Width) &&
            (_arVdpConf[ucVdpId].rOutRegion.u4Height == rOutRegion.u4Height))
        {
            // nothing change

            VDP_MUTEX_UNLOCK;
            return VDP_SET_OK;
        }
		//printf("add debu log _arVdpConf[ucVdpId].rOutRegion.u4Width %d at %d  %s\n",_arVdpConf[ucVdpId].rOutRegion.u4Width,__LINE__,__FUNCTION__);
		if((rOutRegion.u4Width != 0 && rOutRegion.u4Height != 0 )||(ucVdpId == VDP_2))
		{
        	_arVdpConf[ucVdpId].rOutRegion = rOutRegion;
        }
    }       
#if SUPPORT_POP
    if(ucVdpId == VDP_2)
    {
        _arVdpConf[ucVdpId].u4MaxOutWidth = 960*VDP_MAX_REGION_WIDTH/wDISPLAY_WIDTH;
    }
#endif

    if (_u4ScalerPixelBased != 0)
    {
        // u4MaxOutWidth & u4MaxOutHeight is percentage based

        if (_arScalerInfo[ucVdpId].u4OutWidth != 0)
        {
            UINT32 u4MaxOutWidth;

            u4MaxOutWidth = (_arScalerInfo[ucVdpId].u4OutWidth * _arVdpConf[ucVdpId].u4MaxOutWidth) / VDP_MAX_REGION_WIDTH;

            if ((_arVdpConf[ucVdpId].rOutRegion.u4Width > u4MaxOutWidth))
            {
                _arVdpConf[ucVdpId].rOutRegion.u4Width = u4MaxOutWidth;
            }        
        }

        if (_arScalerInfo[ucVdpId].u4OutHeight != 0)
        {
            UINT32 u4MaxOutHeight;

            u4MaxOutHeight = (_arScalerInfo[ucVdpId].u4OutHeight * _arVdpConf[ucVdpId].u4MaxOutHeight) / VDP_MAX_REGION_HEIGHT;

            if ((_arVdpConf[ucVdpId].rOutRegion.u4Height > u4MaxOutHeight))
            {
                _arVdpConf[ucVdpId].rOutRegion.u4Height = u4MaxOutHeight;
            }
        }
    }
    else
    {
    
        if (_arVdpConf[ucVdpId].rOutRegion.u4Width > _arVdpConf[ucVdpId].u4MaxOutWidth)
        {
            _arVdpConf[ucVdpId].rOutRegion.u4Width = _arVdpConf[ucVdpId].u4MaxOutWidth;
        }

        if (_arVdpConf[ucVdpId].rOutRegion.u4Height > _arVdpConf[ucVdpId].u4MaxOutHeight)
        {
            _arVdpConf[ucVdpId].rOutRegion.u4Height = _arVdpConf[ucVdpId].u4MaxOutHeight;
        }
    }

	
    VDP_XdataSetOutRegion((UINT32)ucVdpId, _arVdpConf[ucVdpId].rOutRegion);

#ifdef CC_SRM_ON
    SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_OUT_SIZE + (UINT32)ucVdpId)
        , _arVdpConf[ucVdpId].rOutRegion.u4Width
        , _arVdpConf[ucVdpId].rOutRegion.u4Height);
#endif

#ifdef CC_SCPOS_EN
    if (bApiVideoSetDispRegion(ucVdpId,
        _arVdpConf[ucVdpId].rOutRegion.u4X,
        _arVdpConf[ucVdpId].rOutRegion.u4Y,
        _arVdpConf[ucVdpId].rOutRegion.u4Width,
        _arVdpConf[ucVdpId].rOutRegion.u4Height) != 1)
    {
        VDP_MUTEX_UNLOCK;

        return VDP_SET_ERROR;
    }
    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;
#else

    VDP_VsyncSendCmd(ucVdpId, VDP_CMD_SET_OUTR);

    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;
#endif
}

/**
 * VDP query output region.
 *
 * @param ucVdpId specify the video plane id.
 * @param pucOutFullRegion value 1:full region and ignore parameter rOutRegion.
 * @param prOutRegion output region description.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_GetOutRegion(UCHAR ucVdpId, UCHAR* pucOutFullRegion, VDP_REGION_T* prOutRegion)
{
    VERIFY_VDP_ID(ucVdpId);
    VERIFY_NULL(pucOutFullRegion);
    VERIFY_NULL(prOutRegion);

    VDP_MUTEX_LOCK;
    *pucOutFullRegion = 0;
    *prOutRegion= _arVdpConf[ucVdpId].rOutRegion;
    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;
}


#if 0 // def DRV_SUPPORT_EXTMJC //// RGB to YUV do by LVDS TX Transform
static UINT32 _VdpRgb2Ybr(UINT32 u4RGB)
{
    UINT32 u4Y, u4Cb, u4Cr;
    UINT32 u4R, u4G, u4B;

    u4R = (u4RGB >> 16) & 0xff;
    u4G = (u4RGB >> 8) & 0xff;
    u4B = u4RGB & 0xff;
/*
    u4Y = (16 + (((u4R * 66) + (u4G * 129) + (u4B * 25)) / 255)) & 0xFF;
    u4Cb = ((((128 * 255) + (u4B * 112)) - ((u4R * 38) + (u4G * 74)) ) / 255) & 0xFF;
    u4Cr = ((((128 * 255) + (u4R * 112)) - ((u4G * 94) + (u4B * 18))) / 255) & 0xFF;
*/
    //Sammy provide the following calculation
    u4Y = ((((u4R * 76) + (u4G * 150) + (u4B * 29)) / 255));
       if (u4Y > 255)
           u4Y = 255;
       else
           u4Y = u4Y & 0xFF;

    u4Cb = ((((128 * 255) + (u4B * 128)) - ((u4R * 43) + (u4G * 85)) ) / 255);
       if (u4Cb > 255)
           u4Cb = 255;
       else
           u4Cb = u4Cb & 0xFF;
    u4Cr = ((((128 * 255) + (u4R * 128)) - ((u4G * 107) + (u4B * 21))) / 255);
       if (u4Cr > 255)
           u4Cr = 255;
       else
           u4Cr = u4Cr & 0xFF;

    return ((u4Cr << 16) + (u4Y << 8) + (u4Cb));
}
#endif

UINT32 VDP_SetMuteOnOff(UINT32 u4VdpId, UINT8 fgEnable)
{
    VERIFY_VDP_ID(u4VdpId);

    vDrvVideoSetMuteOnOff((UINT8)u4VdpId, fgEnable);
    vApiVideoAllPQItemOnOff(!fgEnable);
    return VDP_SET_OK;
}

/**
 * VDP set background color.
 *
 * @param ucVdpId specify the video plane id.
 * @param u4BgColor specify the color in RGB color mode, 1 byte per color component.(e.g. Red: 0xFF0000).
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetBg(UCHAR ucVdpId, UINT32 u4BgColor)
{

    VERIFY_VDP_ID(ucVdpId);

    VDP_MUTEX_LOCK;

#ifdef CC_SCPOS_EN
#if 0 //def DRV_SUPPORT_EXTMJC // RGB to YUV do by LVDS TX Transform
    if(IS_COMPANION_CHIP_ON())
    {
        _arVdpConf[ucVdpId].u4BgColor = u4BgColor;

    	if(vDrvGetLvdsColorDomain() == eColorDomainYUV)
    	{
       	 u4BgColor = _VdpRgb2Ybr(u4BgColor);
    	}
    }
    else
#endif
    {
    	if (_arVdpConf[ucVdpId].u4BgColor == u4BgColor)
    	{
        	VDP_MUTEX_UNLOCK;
        	return VDP_SET_OK;
    	}
       _arVdpConf[ucVdpId].u4BgColor = u4BgColor;
    }
#endif


   VDP_MUTEX_UNLOCK;

    VDP_VsyncSendCmd(ucVdpId, VDP_CMD_SET_BG);

#ifdef CC_SCPOS_EN
    //vApiVideoSetBg(ucVdpId, u4BgColor);
    vDrvVideoSetMuteColor(ucVdpId, u4BgColor);
#endif

    return VDP_SET_OK;
}

/**
 * VDP query background color.
 *
 * @param ucVdpId specify the video plane id.
 * @param pu4BgColor the color in RGB color mode, 1 byte per color component.(e.g. Red: 0xFF0000).
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_GetBg(UCHAR ucVdpId, UINT32* pu4BgColor)
{
    VERIFY_VDP_ID(ucVdpId);
    VERIFY_NULL(pu4BgColor);

    *pu4BgColor = _arVdpConf[ucVdpId].u4BgColor;
    return VDP_SET_OK;
}

/**
 * Video plane call back function register
 *
 * @param eFuncType specify the call back function type.
 * @param u4FuncPtr specify the call back function pointer.
 * @return void
 */
void VDP_RegCbFunc(VDP_CB_FUNC_ENUM_T eFuncType, UINT32 u4FuncPtr, UINT32 u4Arg1, UINT32 u4Arg2)
{
    if (eFuncType < VDP_CB_FUNC_NS)
    {
        if (u4FuncPtr == 0)
        {
            _rVdpCbFunc.au4CbFunc[eFuncType] = u4FuncPtr;
            _rVdpCbFunc.au4CbFuncCRC[eFuncType] = u4FuncPtr;
        }
        else
        {
            _rVdpCbFunc.au4CbFunc[eFuncType] = u4FuncPtr;
            _rVdpCbFunc.au4CbFuncCRC[eFuncType] = ~u4FuncPtr;
        }

        if (eFuncType == VDP_CB_FUNC_PTS_IND)
        {
            UINT32 u4EsId;
            UINT32 u4VdpId;
            UINT32 u4PortIdx;
            UINT32 u4CbPts;

            u4EsId = u4Arg1;
            u4CbPts = u4Arg2;

            for (u4VdpId = 0; u4VdpId < VDP_NS; u4VdpId++)
            {
                for (u4PortIdx = 0; u4PortIdx < VDP_MAX_INPORT_NS; u4PortIdx++)
                {
                    if (_arVdpConf[u4VdpId].ucInputPort[u4PortIdx] == (UCHAR) u4EsId)
                    {
                        VDP_FrcSetPtsCb(u4VdpId, u4PortIdx, u4CbPts, u4EsId);
                    }
                }
            }
        }
        else if (eFuncType == VDP_CB_FUNC_LIPSYNC_IND)
        {
            UINT32 u4VdpId;
            UINT32 u4PortIdx;

            for (u4VdpId = 0; u4VdpId < VDP_NS; u4VdpId++)
            {
                for (u4PortIdx = 0; u4PortIdx < VDP_MAX_INPORT_NS; u4PortIdx++)
                {
                    if (_arVdpConf[u4VdpId].ucInputPort[u4PortIdx] == ES0)
                    {
                        VDP_FrcSetLipSyncCb(u4VdpId, u4PortIdx, u4Arg1, u4Arg2);
                    }
                }
            }
        }
        else if((eFuncType == VDP_CB_FUNC_LIPSYNC_OK_TO_NG_IND) ||
                (eFuncType == VDP_CB_FUNC_LIPSYNC_NG_TO_OK_IND))
        {
            UCHAR ucRegType = VDP_LIPSYNC_REG_NG_TO_OK;

            if(eFuncType == VDP_CB_FUNC_LIPSYNC_NG_TO_OK_IND)
            {
                ucRegType = VDP_LIPSYNC_REG_NG_TO_OK;
            }
            else if(eFuncType == VDP_CB_FUNC_LIPSYNC_OK_TO_NG_IND)
            {
                ucRegType = VDP_LIPSYNC_REG_OK_TO_NG;
            }
            if (u4FuncPtr == 0)
            {
                /* Remove callback register */
                VDP_FrcSetLipSyncNGOKCb(ucRegType, 1, u4Arg1, u4Arg2);
            }
            else
            {
                VDP_FrcSetLipSyncNGOKCb(ucRegType, 0, u4Arg1, u4Arg2);
            }
        }        
    }
    else
    {
        ASSERT(0);
    }
}

/**
 * VDP set scaler to pixel based or percentage based.
 *
 * @param u4OnOff = 0 refer to scaler API change to percentage based. (Default)
 * @param u4OnOff = 1 refer to scaler API change to pixel based.
 * @return void
 */
void VDP_SetScalerPixelBased(UINT32 u4OnOff)
{
    UINT8 ucVdpId;

	for(ucVdpId=0;ucVdpId<VDP_NS;ucVdpId++)
    {
        /* patch for translating the out region setting */
        if(_u4ScalerPixelBased == 0 && u4OnOff != 0)
        {
            _arVdpConf[ucVdpId].rOutRegion.u4Width = wDISPLAY_WIDTH*_arVdpConf[ucVdpId].rOutRegion.u4Width/VDP_MAX_REGION_WIDTH;
            _arVdpConf[ucVdpId].rOutRegion.u4Height = wDISPLAY_HEIGHT*_arVdpConf[ucVdpId].rOutRegion.u4Height/VDP_MAX_REGION_HEIGHT;
        }
    }
    _u4ScalerPixelBased = u4OnOff;
}

/**
 * VDP Get scaler to pixel based or percentage based.
 *
 * @param void
 * @return 1 refer to scaler API change to pixel based.
 */
UINT32 VDP_GetScalerPixelBased(void)
{
    return _u4ScalerPixelBased ;
}


/**
 * Get delay time of PQ
 *
 * @return delay time in 90000
 */
UINT32 VDP_GetDelayTime(void)
{
    UINT32 u4PQDelay = 0;
#if 0 // def CC_MT5392B
    u4PQDelay += (UINT32)bDrvMJCGetFrameDelay();
#endif

#if 1 // defined (CC_MT5360B) || defined (CC_MT5392B)
    UCHAR ucVdpId = 0;
    UCHAR ucOutFrameRate = 60;
    if(_arVdpConf[ucVdpId].ucEnable)
    {
        ucVdpId = 0;
    }
    else
    {
        ucVdpId = 1;
    }
    ucOutFrameRate = _VDP_GetOutFrameRate(ucVdpId);
    u4PQDelay += (UINT32)u1DrvDIGetFrameDelay(ucVdpId);
    u4PQDelay += u4ScpipGetFrameDelay(ucVdpId);
    u4PQDelay += (UINT32)u1DrvTDCGetFrameDelay(ucVdpId);
    if(ucOutFrameRate > 0)
    {
        u4PQDelay *= (9000/ucOutFrameRate);
    }
    else
    {
        u4PQDelay *= (9000/60);
    }
    LOG(5, "u4PQDelay = %d\n", u4PQDelay);
#endif
    return u4PQDelay;
}

/**
 * VDP set source size (for module test)
 *
 * @param ucVdpId specify the video plane id.
 * @param u4SrcWidth specify source width size.
 * @param u4SrcHeight specify source height size.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetSrcSize(UCHAR ucVdpId, UINT32 u4SrcWidth, UINT32 u4SrcHeight)
{
    // obsolete
    VERIFY_VDP_ID(ucVdpId);

    VDP_MUTEX_LOCK;

    _arVdpConf[ucVdpId].u4SrcLineSize = u4SrcWidth;
    _arVdpConf[ucVdpId].u4SrcWidth = u4SrcWidth;
    _arVdpConf[ucVdpId].u4SrcHeight = u4SrcHeight;

    VDP_VsyncSendCmd(ucVdpId, VDP_CMD_SET_SRCSIZE);

    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;
}

/**
 * VDP get source size (for module test)
 *
 * @param ucVdpId specify the video plane id.
 * @param pu4SrcLineSize specify source line size pointer.
 * @param pu4SrcWidth specify source width size pointer.
 * @param pu4SrcHeight specify source height size pointer.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_GetSrcSize(UCHAR ucVdpId, UINT32* pu4SrcLineSize, UINT32* pu4SrcWidth, UINT32* pu4SrcHeight)
{
    VERIFY_VDP_ID(ucVdpId);
    VERIFY_NULL(pu4SrcLineSize);
    VERIFY_NULL(pu4SrcWidth);
    VERIFY_NULL(pu4SrcHeight);

    VDP_MUTEX_LOCK;
    *pu4SrcLineSize = _arVdpConf[ucVdpId].u4SrcLineSize;
    *pu4SrcWidth = _arVdpConf[ucVdpId].u4SrcWidth;
    *pu4SrcHeight = _arVdpConf[ucVdpId].u4SrcHeight;
    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;
}

/**
 * VDP get video plane configuration structure
 *
 * @param ucVdpId specify the video plane id.
 * @return video plane configuration structure pointer
 */
VDP_CONF_T* VDP_GetConf(UCHAR ucVdpId)
{
    if (ucVdpId < VDP_NS)
    {
        return &(_arVdpConf[ucVdpId]);
    }
    else
    {
        return NULL;
    }
}

/**
 * VDP get panel type
 */
UINT32 VDP_GetPanelType(void)
{
//#if defined(CC_MT5395) && defined(CC_SCPOS_3DTV_SUPPORT)
#if defined(CC_SCPOS_3DTV_SUPPORT)
    if(IS_LVDS_DISP_3D_POLARIZED) //3D_PR_Panel
    {
        return 1;
    }
    else if (IS_LVDS_DISP_3D_SHUTTER) //3D_Shutter_Panel
    {
        return 2;
    }
    else // 2D_Panel
    {
        return 0;
    }
#else
    return 0;  // 2D_Panel    
#endif
}

/**
 * VDP lock video plane mutex
 *
 * @param void
 * @return void
 */
void VDP_MutexLock(void)
{
    VDP_MUTEX_LOCK;
}

/**
 * VDP unlock video plane mutex
 *
 * @param void
 * @return void
 */
void VDP_MutexUnlock(void)
{
    VDP_MUTEX_UNLOCK;
}

/**
 * VDP get video plane mutex handle
 *
 * @param void
 * @return mutex handle
 */
HANDLE_T VDP_GetMutex(void)
{
    return _hMutex;
}

/**
 * VDP check source change callback function
 *
 * @param ucVdpId specify the video plane id.
 * @param rVdpSrcInfo new source info
 * @return 0: no source/output region change 1: source/output region change
 */
static UINT32 _VDP_CheckSourceChangeCallback(UCHAR ucVdpId, VDP_SOURCE_INFO_T rVdpSrcInfo)
{
    if (ucVdpId >= VDP_NS)
    {
        return 0;
    }

    // vdp_if.c >> vdp_mw.c >> customization
    if (VDP_CHECK_CB_FUNC_VERIFY(_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_SRC_CHG_IND], _rVdpCbFunc.au4CbFuncCRC[VDP_CB_FUNC_SRC_CHG_IND]) ||
    		VDP_CHECK_CB_FUNC_VERIFY(_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_SRC_CHG_IND_2], _rVdpCbFunc.au4CbFuncCRC[VDP_CB_FUNC_SRC_CHG_IND_2]))
    {
    	
    		UINT32 u4Ret1, u4Ret2;
        u4Ret1 = 0xff;
        u4Ret2 = 0xff;
            
        if (_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_SRC_CHG_IND])
        {
            u4Ret1 = ((PFN_VDP_SRC_CHG_NOTIFY)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_SRC_CHG_IND])(ucVdpId, rVdpSrcInfo);
        }

        if (_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_SRC_CHG_IND_2])
        {
            u4Ret2 = ((PFN_VDP_SRC_CHG_NOTIFY)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_SRC_CHG_IND_2])(ucVdpId, rVdpSrcInfo);
        }
        
        if (((0 == u4Ret1) && (0 == u4Ret2)) || 
            ((0 == u4Ret1) && (0xff == u4Ret2)) || 
            ((0xff == u4Ret1) && (0 == u4Ret2)))
        {
            return 0;
        }
                    	
        //if (((PFN_VDP_SRC_CHG_NOTIFY)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_SRC_CHG_IND])(ucVdpId, rVdpSrcInfo))
        {
            //Source/Output Region Updated

            LOG(3, "VDP_CheckSourceChangeCallback Src(%d, %d, %d, %d), Out(%d, %d, %d, %d)\n",
                _arVdpConf[ucVdpId].rSrcRegion.u4X,
                _arVdpConf[ucVdpId].rSrcRegion.u4Y,
                _arVdpConf[ucVdpId].rSrcRegion.u4Width,
                _arVdpConf[ucVdpId].rSrcRegion.u4Height,
                _arVdpConf[ucVdpId].rOutRegion.u4X,
                _arVdpConf[ucVdpId].rOutRegion.u4Y,
                _arVdpConf[ucVdpId].rOutRegion.u4Width,
                _arVdpConf[ucVdpId].rOutRegion.u4Height);

            return 1;
        }
    }
    else
    {
        ASSERT(_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_SRC_CHG_IND] == 0);
    }

    return 0;

}

/*
Scaler display mode not support notify
*/
UINT32 VDP_DispStatusNotify(UINT8 ucVdpId, UINT8 bStatus)
{
    if (ucVdpId >= VDP_NS)
    {
        return 0;
    }

    // vdp_if.c >> vdp_mw.c >> customization
    if (VDP_CHECK_CB_FUNC_VERIFY(_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_DISP_SUPPORT_IND], _rVdpCbFunc.au4CbFuncCRC[VDP_CB_FUNC_DISP_SUPPORT_IND]))
    {
        if (((PFN_VDP_DISP_SUPPORT_NOTIFY)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_DISP_SUPPORT_IND])(ucVdpId, bStatus))
        {
            return 1;
        }
    }
    else
    {
        ASSERT(_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_DISP_SUPPORT_IND] == 0);
    }

    return 0;
}

/*
Scaler display mode not support notify
*/
UINT32 VDP_TDNaviStatusNotify(UINT32 u4VdpId, UINT32 u4Tag)
{
    if (u4VdpId >= VDP_NS)
    {
        return 0;
    }

    // vdp_if.c >> vdp_mw.c >> customization
    if (VDP_CHECK_CB_FUNC_VERIFY(_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_TDNAVI_IND], _rVdpCbFunc.au4CbFuncCRC[VDP_CB_FUNC_TDNAVI_IND]))
    {
        if (((PFN_VDP_NAVI_TAG_NOTIFY)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_TDNAVI_IND])(u4VdpId, u4Tag))
        {
            return 1;
        }
    }
    return 0;
}


#ifdef CC_SCPOS_3DTV_SUPPORT 
#ifdef DRV_SUPPORT_MT8283
extern BOOL fgForceChangeRegion;
#endif
#endif

/**
 * VDP source change notify
 *
 * @param ucVdpId specify the video plane id.
 * @return void
 */
UINT32 VDP_SourceChangeNotify(UINT8 bPath) 
{
   // RPicInfo *prPicInfo;

    UINT32 u4SrcWidth;
    UINT32 u4SrcHeight;
    UINT8 u1Interlace;
    VDP_SOURCE_INFO_T rVdpSrcInfo;
    
    if (bPath >= VDP_NS)
    {
        return 0;
    }

    // Get Source Size
    //u4SrcWidth = wDrvVideoInputValidDataWidth(bPath);
    u4SrcWidth = wApiVideoGetInputWidth(bPath);
    u4SrcHeight = wApiVideoGetInputHeight(bPath);
    u1Interlace = bDrvVideoIsSrcInterlace(bPath);
    
    if (bPath == SV_VP_MAIN)
    {
        LboxSourceChange();
/*
        // Reset LetterBox if Source Change
        _u4LBoxDetectionValid = 0;
        _u4LBoxSourceChange = 1;

        _u4LBoxSrcWidth = u4SrcWidth;
        _u4LBoxSrcHeight = u4SrcHeight;
        _u1LBoxSrcInterrupt = u1LBoxSrcInterrupt;
        */
    }

    rVdpSrcInfo.u4SrcWidth = u4SrcWidth;
    rVdpSrcInfo.u4SrcHeight = u4SrcHeight;
    rVdpSrcInfo.u4OutWidth = wDISPLAY_WIDTH;
    rVdpSrcInfo.u4OutHeight = wDISPLAY_HEIGHT;
    rVdpSrcInfo.u4FrameRate = (UINT32)bDrvVideoGetRefreshRate(bPath);
    rVdpSrcInfo.u4Interlace= (UINT32)u1Interlace;

    // Update Scaler Info
    if (bDrvVideoGetWidthOverSample(bPath))
    {
        _arScalerInfo[bPath].u4SrcWidth = rVdpSrcInfo.u4SrcWidth << 1;
    }
    else
    {
        _arScalerInfo[bPath].u4SrcWidth = rVdpSrcInfo.u4SrcWidth;
    }
    _arScalerInfo[bPath].u4SrcHeight = rVdpSrcInfo.u4SrcHeight;
    _arScalerInfo[bPath].u4OutWidth = rVdpSrcInfo.u4OutWidth;
    _arScalerInfo[bPath].u4OutHeight = rVdpSrcInfo.u4OutHeight;

    if (_VDP_CheckSourceChangeCallback(bPath, rVdpSrcInfo)
    #ifdef CC_SCPOS_3DTV_SUPPORT 
    #ifdef DRV_SUPPORT_MT8283
         || (fgForceChangeRegion && (bPath == SV_VP_MAIN))
    #endif
    #endif
        )    
    {
        
        #ifdef CC_SCPOS_3DTV_SUPPORT         
        #ifdef DRV_SUPPORT_MT8283
                fgForceChangeRegion = FALSE;
        #endif
        #endif
        #ifdef CC_SCPOS_EN
            if (bApiVideoSetSrcRegion(bPath,
                _arVdpConf[bPath].rSrcRegion.u4X,
                _arVdpConf[bPath].rSrcRegion.u4Y,
                _arVdpConf[bPath].rSrcRegion.u4Width,
                _arVdpConf[bPath].rSrcRegion.u4Height) != 1)
            {
            }

            // "Outpur region = 0" = invisible
            if (_arVdpConf[bPath].ucVdpEnable)
            {
                #ifdef CC_SRM_ON
                SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_OUT_SIZE + (UINT32)bPath)
                    , _arVdpConf[bPath].rOutRegion.u4Width
                    , _arVdpConf[bPath].rOutRegion.u4Height);
                #endif

                if (bApiVideoSetDispRegion(bPath,
                    _arVdpConf[bPath].rOutRegion.u4X,
                    _arVdpConf[bPath].rOutRegion.u4Y,
                    _arVdpConf[bPath].rOutRegion.u4Width,
                    _arVdpConf[bPath].rOutRegion.u4Height) != 1)
                {
                }                
            }

            if (_arVdpConf[bPath].ucNoOverScan == 1)
            {
                _arVdpConf[bPath].rOverScan.u4Top = 0;
                _arVdpConf[bPath].rOverScan.u4Bottom = 0;
                _arVdpConf[bPath].rOverScan.u4Left = 0;
                _arVdpConf[bPath].rOverScan.u4Right = 0;
            }

            if (bApiVideoSetSrcOverScan(bPath,
                _arVdpConf[bPath].rOverScan.u4Top,
                _arVdpConf[bPath].rOverScan.u4Bottom,
                _arVdpConf[bPath].rOverScan.u4Left,
                _arVdpConf[bPath].rOverScan.u4Right) != 1)
            {
            }
        #endif
    }
    else
    {
        if (VDP_GetScalerPixelBased() == 0)
        {
            bVideoUpdateSrcRegion(bPath);
        }
        else
        {
            VDP_REGION_T rRegion={0,0,1920,1080};
            VDP_SetSrcRegion(bPath , 1 , rRegion);
        }
    }

    return VDP_SET_OK;
}

/*
Driver Source range change notify
*/
UINT32 VDP_DrvSrcRangeChgNotify(UINT8 bPath, VDP_REGION_T rVdpSrcRegion)
{
    if (bPath >= VDP_NS)
    {
        return 0;
    }
    LOG(0,"------- in VDP_DrvSrcRangeChgNotify--------\n");
    // vdp_if.c >> vdp_mw.c >> customization
    if(VDP_CHECK_CB_FUNC_VERIFY(_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_DRIVER_SRC_RANGE_CHG_IND], _rVdpCbFunc.au4CbFuncCRC[VDP_CB_FUNC_DRIVER_SRC_RANGE_CHG_IND]))
    {
            
        if (_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_DRIVER_SRC_RANGE_CHG_IND])
        {
            
            if(((PFN_VDP_RANGE_CHG_NOTIFY)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_DRIVER_SRC_RANGE_CHG_IND])(bPath, rVdpSrcRegion))
            {
                 LOG(0,"in VDP_DrvSrcRangeChgNotify and (u4X,u4Y,u4Width,u4Height)(%d,%d,%d,%d)\n",rVdpSrcRegion.u4X,rVdpSrcRegion.u4Y,rVdpSrcRegion.u4Width,rVdpSrcRegion.u4Height);
                return 1;
            }
        }
    }
    else
    {
        ASSERT(_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_DRIVER_SRC_RANGE_CHG_IND] == 0);
    }

    return 0;
}
/*
Driver Source range change notify
*/
UINT32 VDP_DrvOutputRangeChgNotify(UINT8 bPath, VDP_REGION_T rVdpOutRegion)
{
    if (bPath >= VDP_NS)
    {
        return 0;
    }
    LOG(0,"------- in VDP_DrvOutputRangeChgNotify--------\n");
    // vdp_if.c >> vdp_mw.c >> customization
    if(VDP_CHECK_CB_FUNC_VERIFY(_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_DRIVER_OUT_RANGE_CHG_IND], _rVdpCbFunc.au4CbFuncCRC[VDP_CB_FUNC_DRIVER_OUT_RANGE_CHG_IND]))
    {   
        if (_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_DRIVER_OUT_RANGE_CHG_IND])
        {
           if(((PFN_VDP_RANGE_CHG_NOTIFY)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_DRIVER_OUT_RANGE_CHG_IND])(bPath, rVdpOutRegion))
            {
                LOG(0,"in VDP_DrvOutputRangeChgNotify and (u4X,u4Y,u4Width,u4Height)(%d,%d,%d,%d)\n",rVdpOutRegion.u4X,rVdpOutRegion.u4Y,rVdpOutRegion.u4Width,rVdpOutRegion.u4Height);
                return 1;
            }
        }
    }
    else
    {
        ASSERT(_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_DRIVER_OUT_RANGE_CHG_IND] == 0);
    }

    return 0;
}
/**
 * VDP set dot-by-dot mode
 *
 * @param ucVdpId specify the video plane id.
 * @param ucEn 0: dot-by-dot mode off, 1:dot-by-dot mode on
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_Set121Map(UCHAR ucVdpId, UCHAR ucEn)
{
    VERIFY_VDP_ID(ucVdpId);

    if (ucVdpId == VDP_1)
    {
#ifdef CC_SCPOS_EN
    vScpipSet121Map(ucVdpId, ucEn);
    VDP_XdataSet121Map(ucVdpId, ucEn);
    #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    SRM_SendEvent(SRM_DRV_SCPOS, SRM_SCPOS_EVENT_Dot_By_Dot, ucVdpId ,(UINT32)ucEn);
    #endif
#else
    UNUSED(ucEn);
#endif
    }

    return VDP_SET_OK;
}

UINT32 VDP_Get121Map(UCHAR ucVdpId)
{
    VERIFY_VDP_ID(ucVdpId);

    return vScpipGet121Map(ucVdpId);
}

UINT32 VDP_Set121MapHPos(UCHAR ucVdpId, UINT16 u2HPos)
{
    VERIFY_VDP_ID(ucVdpId);
    if (ucVdpId == VDP_1)
    {
	vScpipSet121MapHPos(ucVdpId, u2HPos);
    }
    return VDP_SET_OK;
}

/**
 * VDP set letter box detection on/off (for module test)
 *
 * @param void
 * @return void
 */
UINT32 VDP_SetLBoxDetection(UCHAR ucVdpId, UCHAR ucOnOff, UINT8 bFixed)
{
    VERIFY_VDP_ID(ucVdpId);

#ifdef CC_SCPOS_EN
//    vApiVideoLbox(ucVdpId, ucOnOff, bFixed);
    if (ucVdpId == VDP_1)
    {
        LboxSetOnOff(LBOX_ONOFF_AP, ucOnOff, bFixed);
    }
#else
    UNUSED(ucOnOff);
    UNUSED(bFixed);
#endif

    return VDP_SET_OK;
}


/**
 * VDP set letter box detection mode
 *
 * @param void
 * @return void
 */
EXTERN UINT32 _u4StableTime;
EXTERN UINT32 _u4VeryStableTime;
EXTERN UINT32 _u4CaptionStableTime;
//EXTERN UINT32 u4DetectBoundRatio;
UINT32 VDP_LBoxDetectMode(UCHAR ucVdpId, UINT32 u4DetectPeriod, UINT32 Ratio)
{
    VERIFY_VDP_ID(ucVdpId);

    _u4StableTime = u4DetectPeriod;
    _u4VeryStableTime = _u4StableTime * 2;
    _u4CaptionStableTime = _u4StableTime * 3;
    //u4DetectBoundRatio = Ratio;
    UNUSED(Ratio);

    //LOG(1, "Mode Detect Period: %d Detect Bound: %d%\n", _u4StableTime, u4DetectBoundRatio);

    return VDP_SET_OK;
}

/**
 * VDP set nonlinear scaler (for module test)
 *
 * @param ucVdpId specify the video plane id.
 * @param ucNonlinear 0: turn off nonlinear scaler, 1: turn on nonlinear scaler
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetNonlinear(UCHAR ucVdpId, UCHAR ucNonlinear)
{
    VERIFY_VDP_ID(ucVdpId);

    _arVdpConf[ucVdpId].ucNonlinear = ucNonlinear;

#ifdef CC_SCPOS_EN
    VDP_MUTEX_LOCK;
    vScpipSetNonLinearUpScaler(ucVdpId, ucNonlinear);
    VDP_XdataSetNewNonlEn(ucVdpId,(UCHAR) ucNonlinear);
    VDP_MUTEX_UNLOCK;    
#endif

    return VDP_SET_OK;
}

/**
 * VDP get nonlinear scaler status (for module test)
 *
 * @param ucVdpId specify the video plane id.
 * @param pucNonlinear return the status of nonlinear scaler. 0: nonlinear scaler is down, 1: nonlinear scaler is on.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_GetNonlinear(UCHAR ucVdpId, UCHAR* pucNonlinear)
{
    VERIFY_VDP_ID(ucVdpId);
    VERIFY_NULL(pucNonlinear);

    *pucNonlinear = _arVdpConf[ucVdpId].ucNonlinear;

    return VDP_SET_OK;
}

BOOL VDP_IsSupportNonlinear(UCHAR ucVdpId)
{
    BOOL fgSupported = SV_TRUE;
    #if 0//defined(CC_SCPOS_3DTV_SUPPORT)
    if(fgIsScpipTv3dFlgSet(ucVdpId, SCPIP_TV3D_INPUT_IS_3D))
    {
        fgSupported = SV_FALSE;
    }
    #endif
    if((vScpipIs3DDoneByMJC() == SV_TRUE) || (vScpipIs3DDoneByScaler(ucVdpId) == SV_TRUE))
    {
        fgSupported = SV_FALSE;
    }
    return fgSupported;
}

/**
 * VDP force scpos write control off
 *
 * @param u4VdpId specify the video plane id.
 * @param u1OnOff enable/disable force write control off.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_ForceWriteOff(UINT32 u4VdpId, UINT8 u1OnOff)
{
    VERIFY_VDP_ID(u4VdpId);

#ifdef CC_SCPOS_EN
    vScpipForceWriteOff(u4VdpId, u1OnOff);
#else
    UNUSED(u1OnOff);
#endif

    return VDP_SET_OK;
}

UINT32 VDP_SetScposPorch(UINT32 u4VdpId, UINT8 bPorchType, UINT16 wValue)
{
    VERIFY_VDP_ID(u4VdpId);

#ifdef CC_SCPOS_EN
    VDP_MUTEX_LOCK;
    _VDP_ScposSetPorch((UINT8)u4VdpId, bPorchType, wValue);
    VDP_MUTEX_UNLOCK;
#else
    UNUSED(bPorchType);
    UNUSED(wValue);
#endif

    return VDP_SET_OK;
}


/**
 * VDP set game mode on/off
 *
 * @param u4VdpId specify the video plane id.
 * @param u1OnOff enable/disable game mode.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetGameMode(UINT32 u4VdpId, UINT8 u1OnOff)
{
    VERIFY_VDP_ID(u4VdpId);

#if 1
    #ifdef CC_SRM_ON
    SRM_SendEvent(SRM_DRV_SCPOS, SRM_SCPOS_EVENT_GAME_MODE, u4VdpId ,(UINT32)u1OnOff);
    #else
    UNUSED(u1OnOff);
    #endif
#else
#ifdef CC_SCPOS_EN
    vScpipSetGameMode(u4VdpId, u1OnOff);
#else
    UNUSED(u1OnOff);
#endif
#endif
    return VDP_SET_OK;
}

#if defined(CC_DOWN_BW_WITH_MENU)
/**
 * VDP set bandwidth saving mode on/off
 *
 * @param u1OnOff enable/disable bandwidth saving mode.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetDownBWMode(UINT8 u1OnOff)
{
    #ifdef CC_SRM_ON
    SRM_SendEvent(SRM_DRV_SCPOS, SRM_SCPOS_EVENT_DOWN_PQ_MODE, 0,(UINT32)u1OnOff);
    #else
    UNUSED(u1OnOff);
    #endif
    return VDP_SET_OK;
}
#endif


//mtk01140 20071010 3D game mode
/**
 * VDP set game mode on/off
 *
 * @param u4VdpId specify the video plane id.
 * @param u1OnOff enable/disable game mode.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_Set3DGameMode(UINT32 u4VdpId, UINT8 u1OnOff)
{
    VERIFY_VDP_ID(u4VdpId);

#if 0  // 5387
#ifdef CC_SCPOS_EN
	//DI BOB mode control
	#if 1 // def CC_MT5360
	//vDrvDISetBob(u4VdpId, u1OnOff);
	#else
	    #if 0 // def CC_MT5392B
           vDrvDISetBob(u4VdpId, u1OnOff);
	    #else
	vDrvDISetGameMode(u4VdpId, u1OnOff);
	#endif
	#endif
	//control of SCE
	//should bypass DNIE CSC here
//	vApiVideoSCEOnOff(ucVdpId, !bOnOff);

	//control overscan
	//no need?

	//contro scaler 121 mode
	vScpipSet121Map(u4VdpId, u1OnOff);
#else
    UNUSED(u1OnOff);
#endif
#else
    VDP_SetGameMode(u4VdpId, u1OnOff);
#endif
    return VDP_SET_OK;
}

#if 1// no use now
/**
 * VDP set nonlinear scaler scope (for module test)
 *
 * @param ucVdpId specify the video plane id.
 * @param pucNonlinear return the slope of nonlinear scaler.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_GetNonlinearScope(UCHAR ucVdpId, UCHAR* pucNonlinear)
{
    VERIFY_VDP_ID(ucVdpId);
    VERIFY_NULL(pucNonlinear);

    *pucNonlinear = _arVdpConf[ucVdpId].ucNonlinearScope;

    return VDP_SET_OK;
}

#ifdef CC_CLI
/**
 * VDP set nonlinear scaler factor (for module test)
 *
 * @param ucVdpId specify the video plane id.
 * @param ucScpoe slope of nonlinear scaler.
 * @param u4Factor scaling factor of nonlinear scaler in center.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetNonlinearFactor(UCHAR ucVdpId, UCHAR ucScpoe, UINT32 u4Factor)
{
#ifdef CC_SCPOS_EN
    VDP_AUTO_NONLINEAR_PRM_T rAutoNonlinearPrm;

    VERIFY_VDP_ID(ucVdpId);

    rAutoNonlinearPrm.u4EndFactor = u4Factor;
    rAutoNonlinearPrm.u4Adaptive = 0;
    rAutoNonlinearPrm.u4Slope = ucScpoe;
    rAutoNonlinearPrm.u1Mode= VDO_NONLINEAR_AUTO;

//    _VDP_VideoSetNonLinearUpScalerFactor(ucVdpId, ucScpoe, u4Factor);
    VDP_MUTEX_LOCK;
    vScpipSetAutoNonLinear((UINT32)ucVdpId, rAutoNonlinearPrm);
    VDP_MUTEX_UNLOCK;

#else
    VERIFY_VDP_ID(ucVdpId);
#endif

    _arVdpConf[ucVdpId].ucNonlinearScope = ucScpoe;
    _arVdpConf[ucVdpId].u4NonlinarEndFactor = u4Factor;

    return VDP_SET_OK;
}
#endif

/**
 * VDP set nonlinear scaler manually (for module test)
 *
 * @param ucVdpId specify the video plane id.
 * @param rNonlinearPrm nonlinear scaler factors.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetManualNonLinear(UCHAR ucVdpId, VDP_NONLINEAR_PRM_T rNonlinearPrm)
{
    VERIFY_VDP_ID(ucVdpId);

#ifdef CC_SCPOS_EN
    VDP_MUTEX_LOCK;
    vScpipSetManualNonLinear((UINT32)ucVdpId, rNonlinearPrm);
    VDP_MUTEX_UNLOCK;
#else
    UNUSED(ucVdpId);
    UNUSED(rNonlinearPrm);
#endif

    return VDP_SET_OK;
}

/**
 * VDP set nonlinear scaler auto (for module test)
 *
 * @param ucVdpId specify the video plane id.
 * @param rAutoNonlinearPrm structure of nonlinear scaler factors in auto mode.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetAutoNonLinear(UCHAR ucVdpId, VDP_AUTO_NONLINEAR_PRM_T rAutoNonlinearPrm)
{
    VERIFY_VDP_ID(ucVdpId);

    _arVdpConf[ucVdpId].ucNonlinearScope = rAutoNonlinearPrm.u4Slope;
    _arVdpConf[ucVdpId].u4NonlinarEndFactor = rAutoNonlinearPrm.u4EndFactor;

#ifdef CC_SCPOS_EN
    VDP_MUTEX_LOCK;
    vScpipSetAutoNonLinear((UINT32)ucVdpId, rAutoNonlinearPrm);
    VDP_MUTEX_UNLOCK;
#else
    UNUSED(ucVdpId);
    UNUSED(rAutoNonlinearPrm);
#endif

    return VDP_SET_OK;
}


/**
 * VDP set nonlinear scaler (for module test)
 *
 * @param ucVdpId specify the video plane id.
 * @param ucNonlinear 0: turn off nonlinear scaler, 1: turn on nonlinear scaler
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetNewNonlinearOnOff(UCHAR ucVdpId, UCHAR ucOnOff)
{
    VERIFY_VDP_ID(ucVdpId);

    _arVdpConf[ucVdpId].ucNonlinear = ucOnOff;

#ifdef CC_SCPOS_EN
    VDP_MUTEX_LOCK;
    vScpipSetNewNonLinearOnOff(ucVdpId, ucOnOff);
    VDP_XdataSetNewNonlEn(ucVdpId,(UCHAR) ucOnOff);
    VDP_MUTEX_UNLOCK;    
#endif

    return VDP_SET_OK;
}
/**
 * VDP set new nonlinear scaler (for module test)
 *
 * @param ucVdpId specify the video plane id.
 * @param rNewNonlinearPrm structure of nonlinear scaler factors
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetNewNonLinear(UCHAR ucVdpId, VDP_NEW_NONLINEAR_PRM_T rNonlPrm)
{
    VERIFY_VDP_ID(ucVdpId);

    //_arVdpConf[ucVdpId].ucNonlinearScope = rAutoNonlinearPrm.u4Slope;
    //_arVdpConf[ucVdpId].u4NonlinarEndFactor = rAutoNonlinearPrm.u4EndFactor;

    if(ucVdpId >= VDP_2)
    {
        return VDP_SET_ERROR;
    }

    if(rNonlPrm.u4SrcMiddleRatio == 0 || rNonlPrm.u4OutMiddleRatio == 0)
    {
        LOG(1, "VDP_SetNewNonLinear: middle region specified is zero\n");
        //return VDP_SET_ERROR;
    }

#ifdef CC_SCPOS_EN
    VDP_MUTEX_LOCK;
    vScpipSetNewNonLinear((UINT32)ucVdpId, rNonlPrm);
    VDP_XdataSetNewNonlPrm(ucVdpId, rNonlPrm);
    VDP_MUTEX_UNLOCK;
#else
    UNUSED(ucVdpId);
    UNUSED(rNonlPrm);
#endif

    return VDP_SET_OK;
}


/**
 * VDP get nonlinear scaler factor (for module test)
 *
 * @param ucVdpId specify the video plane id.
 * @param pucScpoe return the slope of nonlinear scaler.
 * @param pu4Factor return the scaler factor in center of nonlinear scaler.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_GetNonlinearFactor(UCHAR ucVdpId, UCHAR *pucScpoe, UINT32 *pu4Factor)
{
    VERIFY_VDP_ID(ucVdpId);
    VERIFY_NULL(pucScpoe);
    VERIFY_NULL(pu4Factor);

    *pucScpoe = _arVdpConf[ucVdpId].ucNonlinearScope;
    *pu4Factor = _arVdpConf[ucVdpId].u4NonlinarEndFactor;

    return VDP_SET_OK;
}

#endif// end of "no use now"

/**
 * VDP set nonlinear scaler for customer (for module test)
 *
 * @param ucVdpId specify the video plane id.
 * @param bAuto 1: turn on auto mode of nonlinear scaler. 0: turn of auto mode of nonlinear scaler.
 * @param u4StartFactor the scaler factor in side of nonlinear scaler.
 * @param u4MidPoint middle position adjustment of nonlinear scaler.
 * @param u4StopFactor the scaler factor in center of nonlinear scaler.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetNonLinear_Cust(UCHAR ucVdpId, BOOL bAuto,UINT32 u4StartFactor, UINT32 u4Slope,  UINT32 u4MidPoint, UINT32 u4StopFactor)
{
#ifdef CC_SCPOS_EN
    VERIFY_VDP_ID(ucVdpId);
//    vScpipSetManualNonLinear(ucVdpId,  bAuto, u4StartFactor,  u4Slope,   u4MidPoint, u4StopFactor);

    VDP_MUTEX_LOCK;

    if (bAuto)
    {
        VDP_AUTO_NONLINEAR_PRM_T rAutoNonlinearPrm;

        rAutoNonlinearPrm.u4EndFactor = DRVCUST_OptGet(eFlagScposNonLinearEndPointRatio);
        rAutoNonlinearPrm.u4Adaptive = 0;
        rAutoNonlinearPrm.u4Slope = DRVCUST_OptGet(eFlagScposNonLinearSlope);
        rAutoNonlinearPrm.u1Mode = VDO_NONLINEAR_AUTO;

        vScpipSetAutoNonLinear((UINT32)ucVdpId, rAutoNonlinearPrm) ;
    }
    else
    {
        VDP_NONLINEAR_PRM_T rNonlinearPrm;

        rNonlinearPrm.u4End = u4StopFactor;
        rNonlinearPrm.u4MiddlePoint = u4MidPoint;
        rNonlinearPrm.u4Slope = u4Slope;
        rNonlinearPrm.u4Start = u4StartFactor;

        vScpipSetManualNonLinear((UINT32)ucVdpId, rNonlinearPrm);
    }
    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;
#else
    UNUSED(ucVdpId);
    UNUSED(bAuto);
    UNUSED(u4StartFactor);
    UNUSED(u4Slope);
    UNUSED(u4MidPoint);
    UNUSED(u4StopFactor);
    return VDP_SET_OK;
#endif
}

/**
 * VDP set dynamic scaling (for module test)
 *
 * @param ucVdpId specify the video plane id.
 * @param ucOnOff 0: turn off dynamic scaling, 1: turn on dynamic scaling
 * @param u4ResponseTime response time for dynamic scaling in millisecond.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetDynamiceScaler(UCHAR ucVdpId, UCHAR ucOnOff, UINT32 u4ResponseTime)
{
    VERIFY_VDP_ID(ucVdpId);

#ifdef CC_SCPOS_EN
    vScpipSetDynamicScaler(ucVdpId, ucOnOff, u4ResponseTime);
#else
    UNUSED(ucOnOff);
    UNUSED(u4ResponseTime);
#endif

    return VDP_SET_OK;
}

/**
 * VDP set scaler DRAM
 *
 * @param u4VdpId specify the video plane id.
 * @param u4X x offset to fill DRAM data
 * @param u4Y y offset to fill DRAM data
 * @param u4Width pixel width to fill DRAM data
 * @param u4Height pixel height to fill DRAM data
 * @param u4GBRYCbCr pixel data to fill DRAM
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetScalerDRAM(UINT32 u4VdpId, UINT32 u4X, UINT32 u4Y, UINT32 u4Width, UINT32 u4Height, UINT32 u4GBRYCbCr)
{
    VERIFY_VDP_ID(u4VdpId);

#ifdef CC_SCPOS_EN
    u4ScpipDirectSetDRAM(u4VdpId, u4X, u4Y, u4Width, u4Height, u4GBRYCbCr);
#else
    UNUSED(u4X);
    UNUSED(u4Y);
    UNUSED(u4Width);
    UNUSED(u4Height);
    UNUSED(u4GBRYCbCr);
#endif

    return VDP_SET_OK;
}

UINT32 VDP_SetScalerPattern(UINT32 u4VdpId, UINT32 u4OnOff)
{
    UINT32 au4ColorArray[6] = {0xff00ff, 0xffff00, 0xff0000, 0x00ffff, 0x0000ff, 0x00ff00};

    VERIFY_VDP_ID(u4VdpId);

#ifndef CC_SCPOS_EN
    UNUSED(u4OnOff);
    UNUSED(au4ColorArray);
#else

    // freeze/ unfreeze
    UNUSED(VDP_SetFreeze(u4VdpId, u4OnOff));
    x_thread_delay(100);
    if (u4OnOff)
    {
        UINT32 u4GY, u4BCb, u4RCr;
        UINT32 u4GBRYCbCr;
        UINT32 u4Is444;
        UINT32 u4X, u4Y, u4W, u4H;
        UINT32 u4Width, u4Height;
        UINT32 i;

        // get input source resolution
        if (u4VdpId == SV_VP_MAIN)
        {
            u4Width = _rMPicInfo.wTargetW;
            u4Height = _rMPicInfo.wTargetH;
        }
        else
        {
            u4Width = _rPPicInfo.wTargetW;
            u4Height = _rPPicInfo.wTargetH;
        }
//        u4Width = wApiVideoGetInputWidth(u4VdpId);
//        u4Height = wApiVideoGetInputHeight(u4VdpId);

        // get video format
        u4Is444 = (UINT32)bIsScalerInput444(SV_VP_MAIN);

        u4X = 0;
        u4Y = 0;
        u4W = u4Width/6;
        u4H = u4Height;
        for (i = 0; i < 6; i++)
        {
            u4GY = au4ColorArray[i] >> 16;
            u4BCb = (au4ColorArray[i] & 0xff00) >> 8;
            u4RCr = (au4ColorArray[i] & 0xff);

            if (u4Is444 == 0)
            {  // conver RGB to YCbCr
                UINT32 u4Y, u4Cb, u4Cr;
                u4Y = (16 + (((u4RCr * 66) + (u4GY * 129) + (u4BCb * 25)) / 255)) & 0xFF;
                u4Cb = ((((128 * 255) + (u4BCb * 112)) - ((u4RCr * 38) + (u4GY * 74)) ) / 255) & 0xFF;
                u4Cr = ((((128 * 255) + (u4RCr * 112)) - ((u4GY * 94) + (u4BCb * 18))) / 255) & 0xFF;

                u4GY =  u4Y;
                u4BCb = u4Cb;
                u4RCr = u4Cr;
            }
            u4GBRYCbCr = (u4GY << 16) | (u4BCb << 8) | u4RCr;

            u4ScpipDirectSetDRAM(u4VdpId, u4X, u4Y, u4W, u4H, u4GBRYCbCr);
            u4X += u4W;
        }


    }

#endif

    return VDP_SET_OK;
}

/**
 * VDP Check DisplayMode function.
 *
 * @param ucVdpId specify the video plane id.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_CheckScalerDisplayMode(void)
{
    UINT32 u4Mode;
    u4Mode = SRM_CheckDisplayMode(_bSrcMainNew,
                                  wDrvVideoInputHeight(SV_VP_MAIN)*wDrvVideoInputWidth(SV_VP_MAIN),
                                  bDrvVideoGetRefreshRate(SV_VP_MAIN),
                                  fgApiVideoIsVgaTiming(SV_VP_MAIN),
                                  bDrvVideoIsSrcInterlace(SV_VP_MAIN));
    #if 0
    if (u4Mode & FBM_POOL_MODE_SCPOS_FULL)
    {
        return FALSE;
    }
    return TRUE;
    #else
    if (u4Mode & FBM_POOL_MODE_SCPOS_DISP)
    {
        return TRUE;
    }
    return FALSE;

    #endif
}

/**
 * VDP Check Support DotbyDot
 *
 * @param void
 * @return VDP_TRUE or VDP_FLASE
 */
//#define VDP_720HD_RESOLUTION                (1280 * 720)

UINT32 VDP_IsSupportDotByDot(void)
{
    UINT32 u4InputWidth;

    u4InputWidth = (UINT32)wDrvVideoInputWidth(SV_VP_MAIN);
    if (bDrvVideoGetWidthOverSample(SV_VP_MAIN))
    {
        u4InputWidth = u4InputWidth >> 1;
    }
    if((u1ScpipGetDispMode(SV_VP_MAIN) ==VDP_SCPOS_DISPMODE_AUTO) ||
        (u1ScpipGetDispMode(SV_VP_MAIN) ==VDP_SCPOS_DISPMODE_MANUAL))
        {
            if((u4InputWidth == wDISPLAY_WIDTH) && 
            (wDrvVideoInputHeight(SV_VP_MAIN)== wDISPLAY_HEIGHT))
            {   //if scaler display mode, input size == output size, can support dot by dot
                return SV_TRUE;
            }
            else
            {
                return SV_FALSE;
            }
        }
        else
        {
            if((u4InputWidth <= wDISPLAY_WIDTH) && 
            (wDrvVideoInputHeight(SV_VP_MAIN)<= wDISPLAY_HEIGHT))
            {
                return SV_TRUE;
            }
            else
            {
                return SV_FALSE;
            }
        }
}

UINT32 VDP_SetNoOverScan(UCHAR ucVdpId, UCHAR ucNoOverScan)
{

    VERIFY_VDP_ID(ucVdpId);

    VDP_MUTEX_LOCK;

    _arVdpConf[ucVdpId].ucNoOverScan = ucNoOverScan;

    VDP_MUTEX_UNLOCK;

    return VDP_SET_OK;
}

void VDP_GetActiveImageArea(UINT16 *x, UINT16 *y, UINT16 *w, UINT16 *h)
{
    #if 1 // defined(CC_MT5392B) || defined(CC_MT5360B)
    if(0 == bDrvLboxGetActiveImageArea(x, y, w, h))
    {
        return;
    }
    #endif
    *x = *y = *w = *h = 0;
    return;
}

#ifdef DIVX_PLUS_CER
#ifdef CC_B2R_CHAPTER_CHG
BOOL VDP_SetChpterEnable(VDP_CHAPTER_T*    prChapter)
{
    #ifdef   CC_B2R_CHAPTER_CHG
    _VDP_FrcSetChapterEnable(prChapter);
    return TRUE;
    #else
    return FALSE;
    #endif
}
#endif
#endif


void VDP_Memory_Test(UINT32 u4Color)
{
    #if 0 // def CC_MT5392B
    vScpipSetMuteMask(VDP_1, 1, CC_MUTE_MEMORY_TEST, 0);
    vScpipSetMuteMask(VDP_2, 1, CC_MUTE_MEMORY_TEST, 0);
    #else
    _vDrvVideoSetMute(MUTE_MODULE_MEMORY_TEST, VDP_1, FOREVER_MUTE, FALSE);
    #if SUPPORT_POP
    _vDrvVideoSetMute(MUTE_MODULE_MEMORY_TEST, VDP_2, FOREVER_MUTE, FALSE);
    #endif
    #endif

    if (u4Color) // green
    {
        vScposSetMuteColor(VDP_1, 0x00ff00);
        #if SUPPORT_POP
        vScposSetMuteColor(VDP_2, 0x00ff00);
        #endif
    }
    else // red
    {
        vScposSetMuteColor(VDP_1, 0xff0000);
        #if SUPPORT_POP
        vScposSetMuteColor(VDP_2, 0xff0000);
        #endif
    }
}

static UINT32 _u4PlaBlankedCounter = 0;
static UINT32 _u4PlaBlankedReason = (BLANK_FRAMERATE_CHANGED|BLANK_RESOLUTION_CHANGED);
#define PLANES_BLANK_VSYNC 10

void VDP_SetBlankedReason(UINT32 u4BlankedReason, BOOL fgOnOff)
{
    if(fgOnOff != 0)
    {
        _u4PlaBlankedReason |= u4BlankedReason;
    }
    else
    {
        _u4PlaBlankedReason &= ~u4BlankedReason;
    }
}

void VDP_SetPlanesBlank(UINT32 u4BlankedReason)
{
    CRIT_STATE_T rCritState;
    if (_u4PlaBlankedCounter == 0 &&
        (u4BlankedReason & _u4PlaBlankedReason) != 0)
    {
        rCritState = x_crit_start();
        _u4PlaBlankedCounter = PLANES_BLANK_VSYNC;
        x_crit_end(rCritState);
        PMX_SetAllPlaneHide(TRUE);
    }
    LOG(5, "VDP_SetPlanesBlank\n");

}

void VDP_SetPlanesUnblank()
{
    CRIT_STATE_T rCritState;

    if (_u4PlaBlankedCounter > 0)
    {
        if (_u4PlaBlankedCounter == 1)
        {
            PMX_SetAllPlaneHide(FALSE);
            LOG(5, "VDP_SetPlanesUnblank\n");
        }
        rCritState = x_crit_start();
        _u4PlaBlankedCounter--;
        x_crit_end(rCritState);
    }
}

UINT32 VDP_GetMinSrcRegionHeight(void)
{
	UINT32 ret = 0;
	#if defined(CC_MT5396)
	ret = u4ScpipGetMinSrcRegionHeight(VDP_1);
	#endif
	return ret;
}

UINT32 VDP_GetMinSrcRegionWidth(void)
{
    UINT32 ret = 0;
    ret = u4ScpipGetMinSrcRegionWidth(VDP_1);
    return ret;
}

void VDP_SetDisplayOff(UINT32 u4Path, UINT32 u4OnOff)
{             
    #ifdef CC_SCPOS_EN
    vScpipForceDispOff(u4Path,  u4OnOff);
    #else
    UNUSED(u4Path);
    UNUSED(u4OnOff);    
    #endif
    LOG(5, "VDP_SetDisplayOff Path(%d) OnOff(%d)\n",u4Path,  u4OnOff);
}

#if SUPPORT_DOT_TO_DOT_PATTERN
void VDP_DotToDotPattern(UINT8 ucOnOff,UINT32 u4RGBColor)
{             
    #ifdef CC_SCPOS_EN
    vScpipDotToDotPattern(ucOnOff,  u4RGBColor);
    #else
    UNUSED(ucOnOff);    
    UNUSED(u4RGBColor);
    #endif
    LOG(5, "VDP_DotToDotPattern OnOff(%d) RGBColor(%x)\n", ucOnOff, u4RGBColor);
}
#endif
#if SUPPORT_SBS_CONVERT
void VDP_SBSConVert(UINT8 ucOnOff)
{             
    #ifdef CC_SCPOS_EN
    vScpipSBSConVert(ucOnOff);
    #else
    UNUSED(ucOnOff);    
    #endif
    LOG(5, "SBS output OnOff(%d)\n", ucOnOff);
}
#endif

UINT32 VDP_SetDispmode(UCHAR ucVdpId, UCHAR ucMode)
{
    if((ucVdpId > VDP_1) || (ucMode >= VDP_SCPOS_DISPMODE_UNKNOWN))
    {
        return VDP_SET_ERROR;
    }
    
    VDP_MUTEX_LOCK;
    _VDP_DispModeEnable(ucVdpId, ucMode);
    if (ucMode == VDP_SCPOS_PSCAN_DISPMODE)
    {
        vDrvDISetDebugPSCANDisplay(ucVdpId, SV_ON);
    }
    else
    {
        vDrvDISetDebugPSCANDisplay(ucVdpId, SV_OFF);
    }    
    VDP_XdataSetDispmode(ucVdpId, ucMode);
    VDP_MUTEX_UNLOCK;
    
    return VDP_SET_OK;
}

UINT32 VDP_GetDispmode(UCHAR ucVdpId)
{
    return u1ScpipGetDispMode((UINT8)ucVdpId);
}

UINT32 VDP_SetScaler10BitMode(UINT32 u4VdpId, UINT32 u4Forced10BitMode)
{
    VDP_MUTEX_LOCK;
    _VDP_Set10bitMode(u4VdpId, u4Forced10BitMode);
    VDP_XdataSetScaler10Bit(u4VdpId, u4Forced10BitMode);
    VDP_MUTEX_UNLOCK;
    
    return VDP_SET_OK;
}

UINT32 VDP_GetScaler10BitMode(UINT32 u4VdpId)
{
    return u4ScpipGetForced10BitMode(u4VdpId);
}

UINT32 VDP_UpdateCmdDelay(UINT32 u4VdpId)
{
    #ifdef CC_MT5396
    VDP_MUTEX_LOCK;
    _VDP_UpdateCmdDelay(u4VdpId);
    VDP_MUTEX_UNLOCK;
    #endif
    UNUSED(u4VdpId);    
    
    return VDP_SET_OK;
}

UINT32 _VDP_UpdateCmdDelay(UINT32 u4VdpId)
{
    #ifdef CC_MT5396
    UINT32 u4MJCFrameDelay;
    CRIT_STATE_T rCritState;

    u4MJCFrameDelay = (UINT32)u1DrvMJCGetFrmDelay();
    u4MJCFrameDelay += 2; // scaler delay
    rCritState = x_crit_start();
    _u4VdpCmdDelay = (_u4VdpCmdDelay > u4MJCFrameDelay)? _u4VdpCmdDelay: u4MJCFrameDelay;
    x_crit_end(rCritState);
    
    LOG(3, "Update Vdp(%d) Cmd Delay: %d\n", u4VdpId, _u4VdpCmdDelay);
    #endif
    UNUSED(u4VdpId);    
    
    return VDP_SET_OK;
}

UINT32 VDP_GetCmdDelay(void)
{
    UINT32 u4Delay = 0;
    
    #ifdef CC_MT5396
    {
        CRIT_STATE_T rCritState;
        UINT32 u4FrameRate = (UINT32)vDrvGetLCDFreq();
        VDP_MUTEX_LOCK;

        if (u4FrameRate == 0)
        {
            u4FrameRate = 60;
        }
        rCritState = x_crit_start();
        u4Delay = _u4VdpCmdDelay * (1000/u4FrameRate);
        x_crit_end(rCritState);

        LOG(3, "Vdp Cmd Delay = %d ms\n", u4Delay);

        VDP_MUTEX_UNLOCK;
    }
    #endif

    
    return u4Delay;
}

#if 1 //for capture logo
void VDP_GetDramInf(UINT8 bPath,DRAM_INFO_T* inf)
{
		#ifdef CC_FLIP_MIRROR_SUPPORT
		UINT8 FlipMirrorModule = u4GetFlipMirrorModule(bPath);
		#endif
	
		if(inf == NULL)
			return ;

		if(VDP_SCPOS_DISPMODE_OFF == u1ScpipGetDispMode(bPath))
		{
			vScpipGetDramInf(bPath,(SCALER_INFO_T*)inf);
			inf->scaler_inf.dram_from = 3;
			#ifdef CC_FLIP_MIRROR_SUPPORT
			if(FlipMirrorModule & ( FLIP_BY_SCALER | FLIP_BY_PSCAN | FLIP_BY_B2R ))
			{	
				if(_arScalePrm[bPath].u4MirrorEnable)//bScpipMirrorEnable[bPath]
				{
					inf->scaler_inf.dram_from |= 0x10;
				}
				if(_arScalePrm[bPath].u4FlipEnable)//bScpipFlipEnable[bPath]
				{
					inf->scaler_inf.dram_from |= 0x20;
				}
			}
			#endif
			return ;
		}
		else if(VDP_SCPOS_PSCAN_DISPMODE == u1ScpipGetDispMode(bPath) ||
            (VDP_SCPOS_LINESYNC_DISPMODE == u1ScpipGetDispMode(bPath) &&
            bGetSignalType(bPath) != SV_ST_VGA  && !SRM_IsGameMode(bPath)))
		{
			vDrvDIDispInfoForCapLog(bPath,(DI_DISP_INFO_T*)inf);
			inf->DI_inf.dram_from = 2; //HD exclude DTV from DI
			inf->scaler_inf.dram_from = 2;		
			#ifdef CC_FLIP_MIRROR_SUPPORT
			if(FlipMirrorModule == FLIP_BY_B2R)    //b2r enable flip & mirror at the same time
			{	
				inf->scaler_inf.dram_from |= 0x10;
				inf->scaler_inf.dram_from |= 0x20;
			}
			else
			{
				//todo u1CfgFlipMirror will be define when di data structure is update.
				//inf->scaler_inf.dram_from |= (inf->DI_inf.u1CfgFlipMirror<<4);
			}
			#endif
			return;
		}
		
		else if((SV_VD_MPEGHD ==  bGetVideoDecType(bPath)) ||(SV_VD_MPEGSD ==  bGetVideoDecType(bPath)))
		{
			#ifdef CC_B2R_SKYPE_SUPPORT
            VDP_HalGetB2rDispInfo(bPath,(B2R_DISP_INFO_T *)inf);//(&(inf->B2R_inf));
			#else
			VDP_HalGetB2rDispInfo((B2R_DISP_INFO_T *)inf);//(&(inf->B2R_inf));
			#endif
			inf->B2R_inf.dram_from = 1;
			inf->scaler_inf.dram_from = 1;
			return ;
		}
		else
		{
			inf->B2R_inf.dram_from = 0xF;
			return ;
		}
	
}

#endif
/**
 * VDP set video plane picture mode.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucMode ice_backlight_dimming_low or ice_backlight_dimming_auto.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetBacklightMode(UCHAR ucVdpId, UINT8 ucMode)
{
    VERIFY_VDP_ID(ucVdpId);

    vApiUpdateBacklightMode(ucMode);
    return VDP_SET_OK;
}

/**
 * VDP set HDMI True Black on/off
 *
 * @param u4VdpId specify the video plane id.
 * @param u1OnOff enable/disable HDMI True Black .
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetHDMITrueBlack(UINT32 u4VdpId, UINT8 u1OnOff)
{
    UINT8 u1HDMIRangeMode=u1OnOff ? SV_HDMI_RANGE_FORCE_FULL: SV_HDMI_RANGE_FORCE_LIMIT;

    VERIFY_VDP_ID(u4VdpId);

    vSetHDMIRangeMode(u1HDMIRangeMode);
    vDrvColorTransform((UINT8)u4VdpId);

    return VDP_SET_OK;
}


