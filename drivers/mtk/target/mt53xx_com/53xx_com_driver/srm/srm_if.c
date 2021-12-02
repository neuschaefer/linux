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
 * $RCSfile: srm_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file srm_if.c
 *  Brief of file srm_if.c.
 *  Details of file srm_if.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "srm_drvif.h"
#include "vdp_drvif.h"
#include "fbm_drvif.h"
#include "srm_debug.h"
#include "drvcust_if.h"
#include "panel.h"

#ifdef CC_SCPOS_EN
#include "source_table.h"  // nptv
#endif

#ifdef CC_MT5396
#include "drv_mjc.h"
#endif

#include "drv_dbase.h"
#include "drv_common.h"
#include "x_ckgen.h"
#include "x_chip_id.h"
#include "x_hal_5381.h"
#include "x_os.h"
#include "x_assert.h"
LINT_EXT_HEADER_END

LINT_SUPPRESS_BRACE(818)        // Info 818: Pointer parameter 'xxx' (line 965) could be declared as pointing to const [MISRA Rule 81]
LINT_SUPPRESS_BRACE(641)        // Warning 641: Converting enum 'VSS_MAJOR_TYPE' to int' (LH's Issue)
LINT_SUPPRESS_BRACE(506)        // Warning 506: Constant value Boolean [MTK Rule 6.1.3]

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define SRM_VDP_NS                              VDP_NS

#define SRM_DB_RULE_NS                          (16)

#define SRM_DRAM_MEAN_FILTER_TAP                (4)

#define SRM_5372_MAX_BW                         (700)

#define SRM_5372_AVG_BW                         (600)

#define SRM_ARG_UNKNOWN                         0xFFFFFFFF

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/**
 * Video plane call back function pool
 */
typedef struct
{
    UINT32 au4CbFunc[SRM_FEATURE_CTRL_NS];
    UINT32 au4CbFuncCRC[SRM_FEATURE_CTRL_NS];
} SRM_CB_FUNC_T;

/** Brief of SRM_MSG_T
 */
typedef struct
{
    UINT32 u4Arg1;
    UINT32 u4Arg2;
    UINT32 u4Arg3;
    UINT32 u4Arg4;
} SRM_MSG_T;

typedef struct
{
    UINT32 u4Trigger;
    UINT32 u4Resolution;
    UINT32 u4Size;
} SRM_MM_MODE_SIZE_INFO_T;

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static UINT32 _u4SrmInit = 0;

static HANDLE_T _hSrmMsgQueue = 0;

//static UINT32 _u4DramMaxUtilization = SRM_5372_MAX_BW;

//static UINT32 _u4DramAvgUtilization = SRM_5372_AVG_BW;

static SRM_CB_FUNC_T _rSrmCbFunc;

static SRM_VDP_INFO_T _arSrmVdpInfo[SRM_VDP_NS];

static SRM_VDP_CRTL_T _arSrmVdpCtrl[SRM_VDP_NS];

static FN_SRN_DB_RULE _apfSrmDbRule[SRM_DB_RULE_NS];

static UINT32 _u4SrmDbRuleNs = 0;

static FN_SRN_DB_RULE _apfSrmDbRuleNotStable[SRM_DB_RULE_NS];

static UINT32 _u4SrmDbRuleNotStableNs = 0;

static UINT32 _u4SrmPause;

static DRV_DBASE_T* _prDbase = NULL;

SRM_TV_MODE_T _eApplicationMode = SRM_TV_MODE_TYPE_LAST_VALID_ENTRY;

static SRM_TV_MODE_T _eCurrentTvMode = SRM_TV_MODE_TYPE_NORMAL;

static UINT32 _u4VgaMaxSupportPixelRate = 0;

static SRM_MM_MODE_T _eMmMode = SRM_MM_MODE_TYPE_OFF;

static FN_SRN_CALLBACK _apfStableCbFunc[SRM_STABLE_CB_NS];
static UINT32 _u4StableCBNs = 0;

static UINT32 _u4EnterGameMode[2] = {0, 0};
static UINT32 _u4EnterScanMode[2] = {0, 0};
#if defined(CC_DOWN_BW_WITH_MENU)
static UINT32 _u4EnterDownBWMode = 0;
#endif
#if defined(CC_DTV_HD_DOT_BY_DOT)
static UINT32 _u4EnterZoomMode = 0;
#endif
static SRM_MM_MODE_SIZE_INFO_T _MMBufferInfo[2];

static UINT8 _u1ForceNptvCB[2] = {0, 0};
UINT32 _u1FBMmode =1;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define SRM_CHECK_CB_FUNC_VERIFY(func, crc)     ((UINT32)func == ~((UINT32)crc))

#define SRM_BYTE3(value) ((value >> 24) & 0xFF)
#define SRM_BYTE2(value) ((value >> 16) & 0xFF)
#define SRM_BYTE1(value) ((value >> 8) & 0xFF)
#define SRM_BYTE0(value) (value & 0xFF)

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static UINT32 _SrmGetMaxVdpWidth(void)
{
    if (VDP_GetScalerPixelBased())
    {
        return PANEL_GetPanelWidth();
    }
    else
    {
        return VDP_MAX_REGION_WIDTH;
    }
}

static UINT32 _SrmGetMaxVdpHeight(void)
{
    if (VDP_GetScalerPixelBased())
    {
        return PANEL_GetPanelHeight();
    }
    else
    {
        return VDP_MAX_REGION_HEIGHT;
    }
}

//-----------------------------------------------------------------------------
/** Brief of _SrmICBondingRule.
 */
//-----------------------------------------------------------------------------

LINT_SUPPRESS_NEXT_EXPRESSION(129)
INLINE static VOID _SrmICBondingRule(VOID)
{
#if 0 // def CC_MT5360
    if (IS_Only2DDISupport())
    {
        _arSrmVdpCtrl[VDP_1].rNewPrm[SRM_BOB_CTRL].u4Arg1 = FBM_POOL_MODE_MDDI_BOB;
    }
#endif
}

//-----------------------------------------------------------------------------
/** Brief of _SrmEventHandler.
 */
//-----------------------------------------------------------------------------

#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368)|| defined(CC_MT5396)||defined(CC_MT5389)
SRM_CLI_CRTL_T _arSrmCliCtrl[SRM_VDP_NS];

static void SRM_initSrmCli(void)
{
    UINT32 u4Feature;
    UINT32 u4VdpId;

    for (u4VdpId=0; u4VdpId<SRM_VDP_NS; u4VdpId++)
    {
        for (u4Feature = 0; u4Feature < SRM_FEATURE_CTRL_NS; u4Feature++)
        {
            _arSrmCliCtrl[u4VdpId].u4Valid[u4Feature] = 0;

            _arSrmCliCtrl[u4VdpId].u4Value[u4Feature].u4Arg1 = 0;
            //_arSrmCliCtrl[u4VdpId].u4Value[u4Feature].u4Arg2 = 0;
            //_arSrmCliCtrl[u4VdpId].u4Value[u4Feature].u4Arg3 = 0;

            _arSrmCliCtrl[u4VdpId].u4Mask[u4Feature].u4Arg1 = 0;
            //_arSrmCliCtrl[u4VdpId].u4Mask[u4Feature].u4Arg2 = 0;
            //_arSrmCliCtrl[u4VdpId].u4Mask[u4Feature].u4Arg3 = 0;
        }
    }
}

#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5389) 
static void SRM_UpdateCliRule(void)
{
    UINT32 u4Feature;
    UINT32 u4VdpId;
    UINT32 u4OrigVal, u4NewVal, u4Value, u4Mask;

    UNUSED(SRM_UpdateCliRule);
    for (u4VdpId=0; u4VdpId<SRM_VDP_NS; u4VdpId++)
    {
        for (u4Feature = 0; u4Feature < SRM_FEATURE_CTRL_NS; u4Feature++)
        {
            if (_arSrmCliCtrl[u4VdpId].u4Valid[u4Feature] != 0)
            {
                u4Value = _arSrmCliCtrl[u4VdpId].u4Value[u4Feature].u4Arg1;
                u4Mask = _arSrmCliCtrl[u4VdpId].u4Mask[u4Feature].u4Arg1;
                u4Value &= u4Mask;

                u4OrigVal = _arSrmVdpCtrl[u4VdpId].rNewPrm[u4Feature].u4Arg1;
                u4OrigVal &= ~u4Mask;
                u4NewVal = u4OrigVal | u4Value;

                _arSrmVdpCtrl[u4VdpId].rNewPrm[u4Feature].u4Arg1 = u4NewVal;

                // only for u4Arg1 now
            }
        }
    }
}
#endif

void SRM_TriggerCliSrm(UINT32 u4VdpId)
{
    UINT32 u4OnOff = SRM_IsGameMode(u4VdpId);
    SRM_SendEvent(SRM_DRV_SCPOS, SRM_SCPOS_EVENT_GAME_MODE, u4VdpId ,(UINT32)!u4OnOff);
    SRM_SendEvent(SRM_DRV_SCPOS, SRM_SCPOS_EVENT_GAME_MODE, u4VdpId ,(UINT32)u4OnOff);
}

void SRM_CliSetDispMode(UINT32 u4VdpId, UINT32 u4Mode)
{
    _arSrmCliCtrl[u4VdpId].u4Mask[SRM_BOB_CTRL].u4Arg1 |= FBM_POOL_MODE_MDDI_DISP;
    _arSrmCliCtrl[u4VdpId].u4Mask[SRM_DISP_CTRL].u4Arg1 |= FBM_POOL_MODE_SCPOS_PSCAN_DISP|FBM_POOL_MODE_SCPOS_FULL|FBM_POOL_MODE_SCPOS_DISP;
    _arSrmCliCtrl[u4VdpId].u4Value[SRM_BOB_CTRL].u4Arg1 &= ~(_arSrmCliCtrl[u4VdpId].u4Mask[SRM_BOB_CTRL].u4Arg1);
    _arSrmCliCtrl[u4VdpId].u4Value[SRM_DISP_CTRL].u4Arg1 &= ~(_arSrmCliCtrl[u4VdpId].u4Mask[SRM_DISP_CTRL].u4Arg1);
    if (u4Mode == VDP_SCPOS_DISPMODE_OFF)
    {
        _arSrmCliCtrl[u4VdpId].u4Value[SRM_DISP_CTRL].u4Arg1 |= FBM_POOL_MODE_SCPOS_FULL;
    }
    else if (u4Mode == VDP_SCPOS_PSCAN_DISPMODE)
    {
        _arSrmCliCtrl[u4VdpId].u4Value[SRM_DISP_CTRL].u4Arg1 |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
        _arSrmCliCtrl[u4VdpId].u4Value[SRM_BOB_CTRL].u4Arg1 |= FBM_POOL_MODE_MDDI_DISP;
    }
    else if ((u4Mode == VDP_SCPOS_DISPMODE_AUTO) || (u4Mode == VDP_SCPOS_DISPMODE_MANUAL))
    {
        _arSrmCliCtrl[u4VdpId].u4Value[SRM_DISP_CTRL].u4Arg1 |= FBM_POOL_MODE_SCPOS_DISP;
    }
    _arSrmCliCtrl[u4VdpId].u4Valid[SRM_BOB_CTRL] = 1;
    _arSrmCliCtrl[u4VdpId].u4Valid[SRM_DISP_CTRL] = 1;
}

void SRM_ClearCliSetting(void)
{
    SRM_initSrmCli();
}
#endif

static VOID _SrmEventHandler(VOID* pvArg)
{
    UNUSED(pvArg);

    while (1)
    {
        UINT32 u4EnterDataBase = 0;
        UINT32 u4NoMessage = 0;
        UINT32 u4EventNumber = 0;
        UINT16 u2MsgQIdx;
        SIZE_T zMsgSize;
        SRM_MSG_T rMsg;
        SRM_TV_MODE_T _eNewTvMode;

        zMsgSize = sizeof(SRM_MSG_T);

        VERIFY(x_msg_q_receive(&u2MsgQIdx, (VOID *) (&rMsg), &zMsgSize,
            &(_hSrmMsgQueue), 1, X_MSGQ_OPTION_WAIT) == OSR_OK);

        ASSERT(zMsgSize == sizeof(SRM_MSG_T));

        // handle all command first, then handle data base
        do
        {
            #if 0
            if (rMsg.u4Arg1 == SRM_DRV_DRAM)
            {
                // handle dram measure event

    //            LOG(1, "[SRM] Measure %d %d %d %d\n",
    //                rMsg.u4Arg1, rMsg.u4Arg2, rMsg.u4Arg3, rMsg.u4Arg4);

                if (rMsg.u4Arg2 == SRM_DRV_TOTAL)
                {
                    // Total dram utilization

                    if (rMsg.u4Arg3 >= _u4DramMaxUtilization)
                    {
                        // shut off something ?
                    }
                    else
                    if (rMsg.u4Arg3 < _u4DramAvgUtilization)
                    {
                        // trun on something ?
                    }
                    else
                    {
                        // release lock ?
                    }

                    if (_prDbase->rDram.u4Count != 0)
                    {
                        UINT64 u8Avg;

                        _prDbase->rDram.u4Count++;

                        if (rMsg.u4Arg3 < _prDbase->rDram.u4MinBw)
                        {
                            _prDbase->rDram.u4MinBw = rMsg.u4Arg3;
                        }

                        if (rMsg.u4Arg3 > _prDbase->rDram.u4MaxBw)
                        {
                            _prDbase->rDram.u4MaxBw = rMsg.u4Arg3;
                        }

                        u8Avg = (UINT64) _prDbase->rDram.u4AvgBw;
                        u8Avg <<= 32;
                        u8Avg += (UINT64) _prDbase->rDram.u4AvgBwFraction;

                        u8Avg -= (u8Avg >> SRM_DRAM_MEAN_FILTER_TAP);
                        u8Avg +=  ((UINT64) rMsg.u4Arg3) << (32 - SRM_DRAM_MEAN_FILTER_TAP);

                        _prDbase->rDram.u4AvgBwFraction = (UINT32) (u8Avg & 0xFFFFFFFF);
                        _prDbase->rDram.u4AvgBw = (UINT32) ((u8Avg >> 32) & 0xFFFFFFFF);
                    }
                    else
                    {
                        _prDbase->rDram.u4Count = 1;
                        _prDbase->rDram.u4MinBw = rMsg.u4Arg3;
                        _prDbase->rDram.u4MaxBw = rMsg.u4Arg3;
                        _prDbase->rDram.u4AvgBw = rMsg.u4Arg3;
                        _prDbase->rDram.u4AvgBwFraction = 0;
                    }
                }
                else
                {
                    // Partial dram utilization
                }
            }
            else
            #endif
            {
                u4EventNumber++;
                LOG(6, "[SRM] Event(%d) drv%d 0x%0x %d %d\n",
                    u4EventNumber, rMsg.u4Arg1, rMsg.u4Arg2, rMsg.u4Arg3, rMsg.u4Arg4);

#ifdef CC_SCPOS_EN
                if (rMsg.u4Arg1 == SRM_DRV_SCPOS)
                {
                    UINT32 u4VdpId;

                    u4VdpId = rMsg.u4Arg2 & SRM_SCPOS_EVENT_MASK;
                    if (u4VdpId >= SRM_VDP_NS)
                    {
                        ASSERT(TRUE);
                        continue;
                    }

                    if (rMsg.u4Arg2 & SRM_SCPOS_EVENT_ONOFF)
                    {
                        if (rMsg.u4Arg3 == 0)
                        {
                            // Reset Source Related Parameter
                            _arSrmVdpInfo[u4VdpId].u4Resolution = 0;
                            _arSrmVdpInfo[u4VdpId].u4Interlace = 0;
                            _arSrmVdpInfo[u4VdpId].u4MpegResolution = 0;

    //                        _arSrmVdpInfo[u4VdpId].u4Status = 0;
    //                        _arSrmVdpInfo[u4VdpId].u4VgaTiming = 0;
                        }

                        _arSrmVdpInfo[u4VdpId].u4Enable = rMsg.u4Arg3;

                        u4EnterDataBase = 1;
                    }
                    else
                    if (rMsg.u4Arg2 & SRM_SCPOS_EVENT_SOURCE)
                    {
                        UINT32 u4SourceType;

                        u4SourceType = VSS_MAJOR(rMsg.u4Arg3);

                        if (_arSrmVdpInfo[u4VdpId].u4Source != u4SourceType)
                        {
                            // Reset Source Related Parameter
                            _arSrmVdpInfo[u4VdpId].u4Resolution = 0;
                            _arSrmVdpInfo[u4VdpId].u4Interlace = 0;
                            _arSrmVdpInfo[u4VdpId].u4MpegResolution = 0;

    //                        _arSrmVdpInfo[u4VdpId].u4Status = 0;
    //                        _arSrmVdpInfo[u4VdpId].u4VgaTiming = 0;
                        }

                        _arSrmVdpInfo[u4VdpId].u4Source = u4SourceType;
                        _arSrmVdpInfo[u4VdpId].u4SourceId = rMsg.u4Arg3;

                        u4EnterDataBase = 1;
                    }
                    else
                    if (rMsg.u4Arg2 & SRM_SCPOS_EVENT_MPEG_SIZE)
                    {
                        _arSrmVdpInfo[u4VdpId].u4MpegResolution = rMsg.u4Arg3;
                        #if defined(CC_DYNAMIC_MPEG_SIZE)
                        _MMBufferInfo[u4VdpId].u4Resolution = rMsg.u4Arg3;
                        _MMBufferInfo[u4VdpId].u4Size = rMsg.u4Arg4;
                        _MMBufferInfo[u4VdpId].u4Trigger = 1;
                        #endif
                        u4EnterDataBase = 1;
                    }
                    else
                    if (rMsg.u4Arg2 & SRM_SCPOS_EVENT_SIZE)
                    {
                        _arSrmVdpInfo[u4VdpId].u4Resolution = rMsg.u4Arg3;
                        _arSrmVdpInfo[u4VdpId].u4Interlace = rMsg.u4Arg4;

                        // u4EnterDataBase = 1;
                        // assume an EVENT, SRM_SCPOS_EVENT_STATUS, will follow SRM_SCPOS_EVENT_SIZE
                    }
					if (rMsg.u4Arg2 & SRM_SCPOS_EVENT_WIDTH)
                    {
                        _arSrmVdpInfo[u4VdpId].u4InputWidth= rMsg.u4Arg3;
                        _arSrmVdpInfo[u4VdpId].u4InputHeight= rMsg.u4Arg4;

                        // u4EnterDataBase = 1;
                        // assume an EVENT, SRM_SCPOS_EVENT_STATUS, will follow SRM_SCPOS_EVENT_SIZE
                    }
                    else
                    if (rMsg.u4Arg2 & SRM_SCPOS_EVENT_OUT_SIZE)
                    {
                        _arSrmVdpInfo[u4VdpId].u4OutputWidth = rMsg.u4Arg3;
                        _arSrmVdpInfo[u4VdpId].u4OutputHeight = rMsg.u4Arg4;

                        u4EnterDataBase = 1;
                    }
                    else
                    if (rMsg.u4Arg2 & SRM_SCPOS_EVENT_SRC_RATIO)
                    {
                        _arSrmVdpInfo[u4VdpId].u4InputSrcWidthRatio = rMsg.u4Arg3;
                        _arSrmVdpInfo[u4VdpId].u4InputSrcHeightRadio = rMsg.u4Arg4;

                        #if defined(CC_DTV_HD_DOT_BY_DOT)
                        u4EnterDataBase = 1;
                        #endif
                    }
                    else
                    if (rMsg.u4Arg2 & SRM_SCPOS_EVENT_STATUS)
                    {
                        // fixed bug, CR103413, CR103873, Swap Issue
                        // Enable = ON, Status 1 >> 0 (Plug Off Case)
                        if ((_arSrmVdpInfo[u4VdpId].u4Status != 0) &&
                            (rMsg.u4Arg3 == 0) &&
                            (_arSrmVdpInfo[u4VdpId].u4Enable != 0))
                        {
                            _arSrmVdpInfo[u4VdpId].u4NoSignal = 1;
                        }

                        _arSrmVdpInfo[u4VdpId].u4Status = rMsg.u4Arg3;
                        _arSrmVdpInfo[u4VdpId].u4VgaTiming = BYTE0(rMsg.u4Arg4);
                        _arSrmVdpInfo[u4VdpId].u4InputFrameRate = BYTE1(rMsg.u4Arg4);
                        _arSrmVdpInfo[u4VdpId].u4ColorFormat444 = BYTE2(rMsg.u4Arg4);

                        u4EnterDataBase = 1;
                    }
                    else
                    if (rMsg.u4Arg2 & SRM_SCPOS_EVENT_UNSTABLE)
                    {
                        u4EnterDataBase = 1;
                    }
                    else
                    if (rMsg.u4Arg2 & SRM_SCPOS_EVENT_GAME_MODE)
                    {
                        _u4EnterGameMode[rMsg.u4Arg3] = rMsg.u4Arg4;
                        u4EnterDataBase = 1;
                    }
                    else
                    if (rMsg.u4Arg2 & SRM_SCPOS_EVENT_SCAN_MODE)
                    {
                        _u4EnterScanMode[rMsg.u4Arg3] = rMsg.u4Arg4;
                        u4EnterDataBase = 1;
                    }
                    #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
                    else
                    if(rMsg.u4Arg2 & SRM_SCPOS_EVENT_Dot_By_Dot)
                    {
                        u4EnterDataBase = 1;
                    }
                    #endif
                    #if defined(CC_DOWN_BW_WITH_MENU)
                    else
                    if (rMsg.u4Arg2 & SRM_SCPOS_EVENT_DOWN_PQ_MODE)
                    {
                        _u4EnterDownBWMode = rMsg.u4Arg4;
                        u4EnterDataBase = 1;
                    }
                    #endif
                    #if defined(CC_DTV_HD_DOT_BY_DOT)
                    else
                    if (rMsg.u4Arg2 & SRM_SCPOS_EVENT_ZOOM_MODE)
                    {
                        if ((rMsg.u4Arg3 == TRUE) && (rMsg.u4Arg3 == TRUE))
                        {
                            // dot-by-dot mode
                            _u4EnterZoomMode = 1;
                            u4EnterDataBase = 1;

                        }
                        else // if ((rMsg.u4Arg3 == SV_TRUE) && (rMsg.u4Arg3 == SV_TRUE))
                        {
                            // zoom mode
                            _u4EnterZoomMode = 2;
                            u4EnterDataBase = 1;
                        }
                    }
                    #endif
                }
#endif
            }

            if (x_msg_q_receive(&u2MsgQIdx, (VOID *) (&rMsg), &zMsgSize,
                &(_hSrmMsgQueue), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
            {
                u4NoMessage = 0;
            }
            else
            {
                u4NoMessage = 1;
                u4EventNumber = 0;
            }
        } while(u4NoMessage == 0);

        if (u4EnterDataBase != 0)
        {
            // handle rule database
            if (_u4SrmPause == 0)
            {
                FN_SRN_DB_RULE* _pfSrmDbRule;
                UINT32 u4SrmDbRuleNs;
                UINT32 u4RuleIdx;
                BOOL fgUpdateRule[2] = {FALSE, FALSE};
                UINT8 u1Audpath;

                #if 0
                if (SRM_IsVideoStable() != 0)
                {
                    // Handle Rule During Stable
                    u4SrmDbRuleNs = _u4SrmDbRuleNs;
                    _pfSrmDbRule = _apfSrmDbRule;
                }
                else
                {
                    // Handle Rule During NOT Stable
                    u4SrmDbRuleNs = _u4SrmDbRuleNotStableNs;
                    _pfSrmDbRule = _apfSrmDbRuleNotStable;
                }
                #else
                // Handle Rule During Stable and NOT Stable
                u4SrmDbRuleNs = _u4SrmDbRuleNs;
                _pfSrmDbRule = _apfSrmDbRule;
                #endif

                if (u4SrmDbRuleNs != 0)
                {
                    // Pre Handle DB Rule
                    for (u4RuleIdx = VDP_1; u4RuleIdx <= VDP_2; u4RuleIdx++)
                    {
                        UINT32 u4Feature;

                        for (u4Feature = 0; u4Feature < SRM_FEATURE_CTRL_NS; u4Feature++)
                        {
                            if (_rSrmCbFunc.au4CbFunc[u4Feature] != 0)
                            {
                                _arSrmVdpCtrl[u4RuleIdx].u4CtrlMode[u4Feature] = SRM_CTRL_MODE_UNKOWN;
                                _arSrmVdpCtrl[u4RuleIdx].rNewPrm[u4Feature] = _arSrmVdpCtrl[u4RuleIdx].rPrm[u4Feature];
                            }
                        }
                    }

                    // Apply DB Rule
                    for (u4RuleIdx = 0; u4RuleIdx < u4SrmDbRuleNs; u4RuleIdx++)
                    {
                        if (_pfSrmDbRule[u4RuleIdx] != 0)
                        {
                            UINT32 u4Mode;
                            u4Mode = _pfSrmDbRule[u4RuleIdx](&_arSrmVdpInfo[0], &_arSrmVdpCtrl[0]);
                            UNUSED(u4Mode);
                        }
                    }

                    #if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5389) 
                    SRM_UpdateCliRule();
                    #endif

                    // Apply Bounding Rule
                    _SrmICBondingRule();

                    _eNewTvMode = SRM_GetTvMode();

                    // force all video path modules(scaler,pscan,TDC) to call back when one of whose mode is changed
                    for (u4RuleIdx = VDP_1; u4RuleIdx <= VDP_2; u4RuleIdx++)
                    {
                    		if ((_arSrmVdpCtrl[u4RuleIdx].rNewPrm[SRM_BOB_CTRL].u4Arg1 != _arSrmVdpCtrl[u4RuleIdx].rPrm[SRM_BOB_CTRL].u4Arg1) ||
                                (_arSrmVdpCtrl[u4RuleIdx].rNewPrm[SRM_BOB_CTRL].u4Arg2 != _arSrmVdpCtrl[u4RuleIdx].rPrm[SRM_BOB_CTRL].u4Arg2) ||
                                (_arSrmVdpCtrl[u4RuleIdx].rNewPrm[SRM_BOB_CTRL].u4Arg3 != _arSrmVdpCtrl[u4RuleIdx].rPrm[SRM_BOB_CTRL].u4Arg3) ||
                                (_arSrmVdpCtrl[u4RuleIdx].rNewPrm[SRM_DISP_CTRL].u4Arg1 != _arSrmVdpCtrl[u4RuleIdx].rPrm[SRM_DISP_CTRL].u4Arg1) ||
                                (_arSrmVdpCtrl[u4RuleIdx].rNewPrm[SRM_DISP_CTRL].u4Arg2 != _arSrmVdpCtrl[u4RuleIdx].rPrm[SRM_DISP_CTRL].u4Arg2) ||
                                (_arSrmVdpCtrl[u4RuleIdx].rNewPrm[SRM_DISP_CTRL].u4Arg3 != _arSrmVdpCtrl[u4RuleIdx].rPrm[SRM_DISP_CTRL].u4Arg3) ||
                                (_arSrmVdpCtrl[u4RuleIdx].rNewPrm[SRM_TDC_CTRL].u4Arg1 != _arSrmVdpCtrl[u4RuleIdx].rPrm[SRM_TDC_CTRL].u4Arg1) ||
                                (_arSrmVdpCtrl[u4RuleIdx].rNewPrm[SRM_TDC_CTRL].u4Arg2 != _arSrmVdpCtrl[u4RuleIdx].rPrm[SRM_TDC_CTRL].u4Arg2) ||
                                (_arSrmVdpCtrl[u4RuleIdx].rNewPrm[SRM_TDC_CTRL].u4Arg3 != _arSrmVdpCtrl[u4RuleIdx].rPrm[SRM_TDC_CTRL].u4Arg3)
                               )
                            {
                            	_u1ForceNptvCB[u4RuleIdx]=1;
                    		}
                    }

                    // Post Handle DB Rule
                    for (u4RuleIdx = VDP_1; u4RuleIdx <= VDP_2; u4RuleIdx++)
                    {
                        UINT32 u4Feature;

                        for (u4Feature = 0; u4Feature < SRM_FEATURE_CTRL_NS; u4Feature++)
                        {
                            if (_arSrmVdpCtrl[u4RuleIdx].rNewPrm[u4Feature].u4Arg1 != SRM_ARG_UNKNOWN)
                            {
                                if (SRM_CHECK_CB_FUNC_VERIFY(_rSrmCbFunc.au4CbFunc[u4Feature], _rSrmCbFunc.au4CbFuncCRC[u4Feature]))
                                {
                                    if ((_arSrmVdpCtrl[u4RuleIdx].rNewPrm[u4Feature].u4Arg1 != _arSrmVdpCtrl[u4RuleIdx].rPrm[u4Feature].u4Arg1) ||
                                        (_arSrmVdpCtrl[u4RuleIdx].rNewPrm[u4Feature].u4Arg2 != _arSrmVdpCtrl[u4RuleIdx].rPrm[u4Feature].u4Arg2) ||
                                        (_arSrmVdpCtrl[u4RuleIdx].rNewPrm[u4Feature].u4Arg3 != _arSrmVdpCtrl[u4RuleIdx].rPrm[u4Feature].u4Arg3) ||
                                        (_eCurrentTvMode != _eNewTvMode) ||
                                        ( (_u1ForceNptvCB[u4RuleIdx]==1) && ((u4Feature==SRM_BOB_CTRL)||(u4Feature==SRM_DISP_CTRL)||(u4Feature==SRM_TDC_CTRL)) )
                                        #if 0
                                        || ((u4Feature == SRM_MMBUFCHG_CTRL) && (SRM_ISMMBufChg(u4RuleIdx)))
                                        #endif
                                       )
                                    {
                                        _arSrmVdpCtrl[u4RuleIdx].rPrm[u4Feature] = _arSrmVdpCtrl[u4RuleIdx].rNewPrm[u4Feature];

                                        ((FN_SRN_CALLBACK)_rSrmCbFunc.au4CbFunc[u4Feature])(u4RuleIdx,
                                            _arSrmVdpCtrl[u4RuleIdx].rPrm[u4Feature].u4Arg1,
                                            _arSrmVdpCtrl[u4RuleIdx].rPrm[u4Feature].u4Arg2,
                                            _arSrmVdpCtrl[u4RuleIdx].rPrm[u4Feature].u4Arg3);

                                        #if 0
                                        // rule change
                                        fgUpdateRule[u4RuleIdx] = TRUE;
                                        #endif
                                    }
                                }
                                else
                                {
                                    ASSERT(_rSrmCbFunc.au4CbFunc[u4Feature] == 0);
                                }
                            }
                        }
			   _u1ForceNptvCB[u4RuleIdx]=0;                        
                    }

                    _eCurrentTvMode = _eNewTvMode;
                }

                // Handle stable callback function
                #if 0
                u1Audpath = AUD_GetPIPVideoPath(); // audio focus window
                #else
                u1Audpath = 0;
                #endif
                if (fgUpdateRule[u1Audpath])
                {
                    UINT32 u4CbIdx = 0;
                    UINT8 u1Stable = 0;
                    for (; u4CbIdx < _u4StableCBNs; u4CbIdx++)
                    {
                        if (_apfStableCbFunc[u4CbIdx] != NULL)
                        {
                            if (u1Audpath == 0) // main path
                            {
                                u1Stable = (SRM_IsMainVideoStrictStable() && (_arSrmVdpInfo[VDP_1].u4Status == 1));
                            }
                            else // sub path
                            {
                                u1Stable = (SRM_IsSubVideoStrictStable() && (_arSrmVdpInfo[VDP_2].u4Status == 1));
                            }

                            if (u1Stable)
                            {
                                _apfStableCbFunc[u4CbIdx](0, 0, 0, 0);
                            }
                            #if 0
                            else
                            {
                                _apfStableCbFunc[u4CbIdx](0, 0, 0, 1);
                            }
                            #endif
                        }
                    }
                }
            }
        }
    }
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief of SRM_Init.
 */
//-----------------------------------------------------------------------------

VOID SRM_Init(VOID)
{
    if (_u4SrmInit == 0)
    {
        UINT32 u4Idx;
        HANDLE_T hThread;
        CRIT_STATE_T _rCritState;

        _prDbase = DBS_Lock(&_rCritState);
        ASSERT(_prDbase);
        VERIFY(DBS_Unlock(_prDbase, _rCritState));

        _u4SrmInit = 1;

#ifdef CC_SRM_PAUSE
        _u4SrmPause = 1;
#else
        _u4SrmPause = 0;
#endif

        _u4VgaMaxSupportPixelRate = DRVCUST_OptGet(eVGAMaxRateForDispmode);

        // reset variable
        for (u4Idx = 0; u4Idx < SRM_FEATURE_CTRL_NS; u4Idx++)
        {
            _rSrmCbFunc.au4CbFunc[u4Idx] = (UINT32)NULL;
        }

        for (u4Idx = 0; u4Idx < SRM_VDP_NS; u4Idx++)
        {
            UINT32 u4Module;

            for (u4Module = 0; u4Module < SRM_FEATURE_CTRL_NS; u4Module++)
            {
                _arSrmVdpCtrl[u4Idx].u4CtrlMode[u4Module] = SRM_CTRL_MODE_UNKOWN;

                _arSrmVdpCtrl[u4Idx].rPrm[u4Module].u4Arg1 = SRM_ARG_UNKNOWN;
                _arSrmVdpCtrl[u4Idx].rPrm[u4Module].u4Arg2 = SRM_ARG_UNKNOWN;
                _arSrmVdpCtrl[u4Idx].rPrm[u4Module].u4Arg3 = SRM_ARG_UNKNOWN;
            }
        }
        
        #if defined(CC_MT5365) || defined(CC_MT5395)
        SRM_initSrmCli();
        #endif

        for (u4Idx = 0; u4Idx < SRM_VDP_NS; u4Idx++)
        {
            _arSrmVdpInfo[u4Idx].u4Enable = 0;
            _arSrmVdpInfo[u4Idx].u4MpegResolution = 0;
            _arSrmVdpInfo[u4Idx].u4Resolution = 0;
        }

#ifdef CC_SCPOS_EN
        SRM_LoadRule((FN_SRN_DB_RULE**)&_apfSrmDbRule, &_u4SrmDbRuleNs,
                     (FN_SRN_DB_RULE**)&_apfSrmDbRuleNotStable, &_u4SrmDbRuleNotStableNs);
#else
        _u4SrmDbRuleNotStableNs = 0;
        _u4SrmDbRuleNs = 0;
#endif

        // set empty rule to NULL
        ASSERT(_u4SrmDbRuleNs <= SRM_DB_RULE_NS);
        for (u4Idx = _u4SrmDbRuleNs; u4Idx < SRM_DB_RULE_NS; u4Idx++)
        {
            _apfSrmDbRule[u4Idx] = 0;
        }

        // set empty rule to NULL
        ASSERT(_u4SrmDbRuleNotStableNs <= SRM_DB_RULE_NS);
        for (u4Idx = _u4SrmDbRuleNotStableNs; u4Idx < SRM_DB_RULE_NS; u4Idx++)
        {
            _apfSrmDbRuleNotStable[u4Idx] = 0;
        }

        // set max dram utilization, according to IC Model (5371/72/73) & Dram Model (DDR1*2/DDR1*4/DDR2*2)
        //_u4DramMaxUtilization = SRM_5372_MAX_BW;
        //_u4DramAvgUtilization = SRM_5372_AVG_BW;

        SRM_DramInit();

        // create message Q
        VERIFY(x_msg_q_create(&_hSrmMsgQueue, "SRM_Q", sizeof(SRM_MSG_T), 64) == OSR_OK);

        // create thread
        if (x_thread_create(&hThread, "SRM", 2048, (MLVDO_THREAD_PRIORITY - 1),
            _SrmEventHandler, 0, NULL) != OSR_OK)
        {
            ASSERT(0);
        }
    }
}

//-----------------------------------------------------------------------------
/** Brief of SRM_Pause.
 */
//-----------------------------------------------------------------------------

VOID SRM_Pause(VOID)
{
    _u4SrmPause = 1;
}

//-----------------------------------------------------------------------------
/** Brief of SRM_Resume.
 */
//-----------------------------------------------------------------------------

VOID SRM_Resume(VOID)
{
#ifdef CC_SRM_PAUSE
    _u4SrmPause = 1;
#else
    _u4SrmPause = 0;
#endif
}

//-----------------------------------------------------------------------------
/** Brief of SRM_SendEvent.
 */
//-----------------------------------------------------------------------------

VOID SRM_SendEvent(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4)
{
    SRM_MSG_T rMsg;

    rMsg.u4Arg1 = u4Arg1;
    rMsg.u4Arg2 = u4Arg2;
    rMsg.u4Arg3 = u4Arg3;
    rMsg.u4Arg4 = u4Arg4;

    VERIFY (x_msg_q_send(_hSrmMsgQueue, (VOID *) (&rMsg), sizeof(SRM_MSG_T), 255) == OSR_OK);
}

//-----------------------------------------------------------------------------
/** Brief of SRM_RegisterCB.
 */
//-----------------------------------------------------------------------------

VOID SRM_RegisterCB(UINT32 u4Mod, FN_SRN_CALLBACK pfCbFun)
{
    UINT32 u4FuncPtr;
    if (u4Mod >= SRM_FEATURE_CTRL_NS)
    {
        return;
    }

    u4FuncPtr = (UINT32)pfCbFun;

    if (u4FuncPtr == 0)
    {
        _rSrmCbFunc.au4CbFunc[u4Mod] = u4FuncPtr;
        _rSrmCbFunc.au4CbFuncCRC[u4Mod] = u4FuncPtr;
    }
    else
    {
        _rSrmCbFunc.au4CbFunc[u4Mod] = u4FuncPtr;
        _rSrmCbFunc.au4CbFuncCRC[u4Mod] = ~u4FuncPtr;
    }
}

VOID SRM_RegisterStableCB(FN_SRN_CALLBACK pfCbFun)
{
    if (_u4StableCBNs < SRM_STABLE_CB_NS)
    {
        _apfStableCbFunc[_u4StableCBNs] = pfCbFun;
        _u4StableCBNs++;
    }
    else
    {
        LOG(1, "[SRM] no room for additional callback\n");
    }
}

//-----------------------------------------------------------------------------
/** Brief of SRM_GetTvMode.
 */
//-----------------------------------------------------------------------------

UINT8 _u1tvmode_scart = 0;
SRM_TV_MODE_T SRM_GetTvMode(VOID)
{
    if ((_eApplicationMode == SRM_TV_MODE_TYPE_NORMAL) && (_u1tvmode_scart == 1))
    {
        return SRM_TV_MODE_TYPE_PIP;
    }
    if (_eApplicationMode == SRM_TV_MODE_TYPE_LAST_VALID_ENTRY)
    {
        if ((_arSrmVdpInfo[VDP_1].u4Enable != 0) &&
            (_arSrmVdpInfo[VDP_1].u4OutputWidth != 0) &&
            (_arSrmVdpInfo[VDP_1].u4OutputHeight != 0) &&
            (_arSrmVdpInfo[VDP_2].u4Enable != 0) &&
            (_arSrmVdpInfo[VDP_2].u4OutputWidth != 0) &&
            (_arSrmVdpInfo[VDP_2].u4OutputHeight != 0))
        {
            if ((_arSrmVdpInfo[VDP_1].u4OutputWidth + _arSrmVdpInfo[VDP_2].u4OutputWidth) > _SrmGetMaxVdpWidth())
            {
                return SRM_TV_MODE_TYPE_PIP;
            }

            return SRM_TV_MODE_TYPE_POP;
        }

        return SRM_TV_MODE_TYPE_NORMAL;
    }
    else
    {
        return _eApplicationMode;
    }
}

VOID SRM_SetTvMode(SRM_TV_MODE_T eMode)
{

    if (eMode == SRM_TV_MODE_TYPE_SCART_OUT_ON)
    {
        _u1tvmode_scart = 1;
    }
    else if (eMode == SRM_TV_MODE_TYPE_SCART_OUT_OFF)
    {
        _u1tvmode_scart = 0;
    }
    else
    {
        _eApplicationMode = eMode;
    }
    LOG(5, "[SRM] Event (TvMod) %d\n", eMode);
}

SRM_MM_MODE_T SRM_GetMmMode(VOID)
{
    return _eMmMode;
}

VOID SRM_SetMmMode(SRM_MM_MODE_T eMode)
{
    _eMmMode = eMode;
    LOG(5, "[SRM] Event (MmMod) %d\n", eMode);
}

//-----------------------------------------------------------------------------
/** Brief of SRM_GetFreezeMode.
 */
//-----------------------------------------------------------------------------
SRM_FREEZE_MODE_T SRM_GetFreezeMode(UINT32 u4VdpId)
{
    if (u4VdpId == VDP_1)
    {
        #ifdef CC_MT5396
        if (u1DrvMJCIsOnOff() && (((SRM_IsPipVideo() && (_eApplicationMode == SRM_TV_MODE_TYPE_PIP)) || SRM_IsPopVideo()) == 0))
        {
            return SRM_FREEZE_MODE_MJC;
        }
        #endif
        
        if (_arSrmVdpInfo[VDP_1].u4Source == (UINT32) VSS_DTV)
        {
            // if signal is still un-stable, use mpeg freeze
            if ((_arSrmVdpCtrl[VDP_1].rPrm[SRM_DISP_CTRL].u4Arg1 & FBM_POOL_MODE_SCPOS_DISP) ||
                (_arSrmVdpInfo[VDP_1].u4Status == 0))
            {
                return SRM_FREEZE_MODE_MPEG;
            }
        }

        if (_arSrmVdpCtrl[VDP_1].rPrm[SRM_DISP_CTRL].u4Arg1 & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
        {
            return SRM_FREEZE_MODE_MDDI;
        }
    }

    return SRM_FREEZE_MODE_SCPOS;
}

//-----------------------------------------------------------------------------
/** Brief of SRM_GetVdpInfo.
 */
//-----------------------------------------------------------------------------
SRM_VDP_INFO_T* SRM_GetVdpInfo(UINT32 u4VdpId)
{
    if (u4VdpId < SRM_VDP_NS)
    {
        return &_arSrmVdpInfo[u4VdpId];
    }

    return (SRM_VDP_INFO_T*) NULL;
}

//-----------------------------------------------------------------------------
/** Brief of SRM_GetVdpInfo.
 */
//-----------------------------------------------------------------------------
SRM_VDP_CRTL_T* SRM_GetVdpCtrl(UINT32 u4VdpId)
{
    if (u4VdpId < SRM_VDP_NS)
    {
        return &_arSrmVdpCtrl[u4VdpId];
    }

    return (SRM_VDP_CRTL_T*) NULL;
}

//-----------------------------------------------------------------------------
/** Brief of SRM_CheckVGABandwidth.
 */
//-----------------------------------------------------------------------------
UINT32 SRM_CheckVGABandwidth(UINT32 u4ActiveWidth, UINT32 u4ActiveHeight, UINT32 u4ReflashRate)
{
    UINT32 u4Rate;

    u4Rate = (u4ActiveWidth * u4ActiveHeight) * u4ReflashRate;

    if (u4Rate >= _u4VgaMaxSupportPixelRate)
    {
        return SRM_VGA_BANDWIDTH_FAIL;
    }

    return SRM_VGA_BANDWIDTH_OK;
}

UINT32 SRM_CheckVdpEnable(UINT32 u4VdpId)
{
	return _arSrmVdpInfo[u4VdpId].u4Enable;
}

//-----------------------------------------------------------------------------
/** Brief of SRM_QueryStatus.
 */
//-----------------------------------------------------------------------------
VOID SRM_QueryStatus(VOID)
{
    UINT32 u4VdpId;

    for (u4VdpId = 0; u4VdpId < VDP_NS; u4VdpId++)
    {
        LOG(1, "SRM(%d) E(%d) S(%d) 444(%d) VGA(%d) RES(%d, %d) W(%d) H(%d) I(%d) FR(%d) PIP(%d) POP(%d) Flip(%d)\n",
            u4VdpId,
            _arSrmVdpInfo[u4VdpId].u4Enable,
            _arSrmVdpInfo[u4VdpId].u4Status,
            _arSrmVdpInfo[u4VdpId].u4ColorFormat444,
            _arSrmVdpInfo[u4VdpId].u4VgaTiming,
            _arSrmVdpInfo[u4VdpId].u4Resolution,
            _arSrmVdpInfo[u4VdpId].u4MpegResolution,
            _arSrmVdpInfo[u4VdpId].u4InputWidth,
            _arSrmVdpInfo[u4VdpId].u4InputHeight,
            _arSrmVdpInfo[u4VdpId].u4Interlace,
            _arSrmVdpInfo[u4VdpId].u4InputFrameRate,
            SRM_IsPipVideo(), SRM_IsPopVideo(),
            u4QueryFlipModule(u4VdpId));

        LOG(1, "SRM(%d) CTRL MJC(0x%x) BOB(0x%x) NR(0x%x) DISP(0x%x) MUTE(0x%x) WE(0x%x) MPEG(0x%x)\n",
            u4VdpId,
            _arSrmVdpCtrl[u4VdpId].rPrm[SRM_MJC_CTRL].u4Arg1,
            _arSrmVdpCtrl[u4VdpId].rPrm[SRM_BOB_CTRL].u4Arg1,
            _arSrmVdpCtrl[u4VdpId].rPrm[SRM_NR_CTRL].u4Arg1,
            _arSrmVdpCtrl[u4VdpId].rPrm[SRM_DISP_CTRL].u4Arg1,
            _arSrmVdpCtrl[u4VdpId].rPrm[SRM_MUTE_CTRL].u4Arg1,
            _arSrmVdpCtrl[u4VdpId].rPrm[SRM_SCALER_WE_CTRL].u4Arg1,
            _arSrmVdpCtrl[u4VdpId].rPrm[SRM_MPEG_PIP_CTRL].u4Arg1);
    }

    for (u4VdpId = 0; u4VdpId < VDP_NS; u4VdpId++)
    {
        LOG(3, "SRM(%d) DISP flag:\n", u4VdpId);
        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_DISP_CTRL].u4Arg1 & FBM_POOL_MODE_8BIT)
        {
            LOG(3, "FBM_POOL_MODE_8BIT\n");
        }
        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_DISP_CTRL].u4Arg1 & FBM_POOL_MODE_10BIT)
        {
            LOG(3, "FBM_POOL_MODE_10BIT\n");
        }
        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_DISP_CTRL].u4Arg1 & FBM_POOL_MODE_UNSTABLE)
        {
            LOG(3, "FBM_POOL_MODE_UNSTABLE\n");
        }
        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_DISP_CTRL].u4Arg1 & FBM_POOL_MODE_SCPOS_DISP)
        {
            LOG(3, "FBM_POOL_MODE_SCPOS_DISP\n");
        }
        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_DISP_CTRL].u4Arg1 & FBM_POOL_MODE_SCPOS_FULL)
        {
            LOG(3, "FBM_POOL_MODE_SCPOS_FULL\n");
        }
        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_DISP_CTRL].u4Arg1 & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
        {
            LOG(3, "FBM_POOL_MODE_SCPOS_PSCAN_DISP\n");
        }
        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_DISP_CTRL].u4Arg1 & FBM_POOL_MODE_SCPOS_MJC_DISP)
        {
            LOG(3, "FBM_POOL_MODE_SCPOS_MJC_DISP\n");
        }
        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_DISP_CTRL].u4Arg1 & FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR)
        {
            LOG(3, "FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR\n");
        }
		if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_DISP_CTRL].u4Arg1 & FBM_POOL_MODE_SCPOS_LINESYNC_DISP)
        {
            LOG(3, "FBM_POOL_MODE_SCPOS_LINESYNC_DISP\n");
        }
		
        LOG(3, "SRM(%d) BOB flag:\n", u4VdpId);
        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_BOB_CTRL].u4Arg1 & FBM_POOL_MODE_8BIT)
        {
            LOG(3, "FBM_POOL_MODE_8BIT\n");
        }
        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_BOB_CTRL].u4Arg1 & FBM_POOL_MODE_10BIT)
        {
            LOG(3, "FBM_POOL_MODE_10BIT\n");
        }
        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_BOB_CTRL].u4Arg1 & FBM_POOL_MODE_MDDI_DISP)
        {
            LOG(3, "FBM_POOL_MODE_MDDI_DISP\n");
        }
        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_BOB_CTRL].u4Arg1 & FBM_POOL_MODE_MDDI_BOB)
        {
            LOG(3, "FBM_POOL_MODE_MDDI_BOB\n");
        }
        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_BOB_CTRL].u4Arg1 & FBM_POOL_MODE_MDDI_FULL)
        {
            LOG(3, "FBM_POOL_MODE_MDDI_FULL\n");
        }
        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_BOB_CTRL].u4Arg1 & FBM_POOL_MODE_MDDI_CBOB)
        {
            LOG(3, "FBM_POOL_MODE_MDDI_CBOB\n");
        }
        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_BOB_CTRL].u4Arg1 & FBM_POOL_MODE_MDDI_NR_Y_C)
        {
            LOG(3, "FBM_POOL_MODE_MDDI_NR_Y_C\n");
        }
        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_BOB_CTRL].u4Arg1 & FBM_POOL_MODE_MDDI_NR_Y_ONLY)
        {
            LOG(3, "FBM_POOL_MODE_MDDI_NR_Y_ONLY\n");
        }
        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_BOB_CTRL].u4Arg1 & FBM_POOL_MODE_MDDI_NR_OFF)
        {
            LOG(3, "FBM_POOL_MODE_MDDI_NR_OFF\n");
        }
    }

    LOG(1, "(_eApplicationMode: %d)(_u1tvmode_scart: %d)(_eMmMode: %d)(_eCurrentTvMode: %d) (_u1FBMmode: %d)\n", _eApplicationMode, _u1tvmode_scart, _eMmMode, _eCurrentTvMode,
		      _u1FBMmode);

    LOG(9, "0x%08x = SRM_SCPOS_EVENT_ONOFF\n", SRM_SCPOS_EVENT_ONOFF);
    LOG(9, "0x%08x = SRM_SCPOS_EVENT_SOURCE\n", SRM_SCPOS_EVENT_SOURCE);
    LOG(9, "0x%08x = SRM_SCPOS_EVENT_MPEG_SIZE\n", SRM_SCPOS_EVENT_MPEG_SIZE);
    LOG(9, "0x%08x = SRM_SCPOS_EVENT_SIZE\n", SRM_SCPOS_EVENT_SIZE);
    LOG(9, "0x%08x = SRM_SCPOS_EVENT_OUT_SIZE\n", SRM_SCPOS_EVENT_OUT_SIZE);
    LOG(9, "0x%08x = SRM_SCPOS_EVENT_STATUS\n", SRM_SCPOS_EVENT_STATUS);
    LOG(9, "0x%08x = SRM_SCPOS_EVENT_UNSTABLE\n", SRM_SCPOS_EVENT_UNSTABLE);
}

#if 1

#define MD(x) ((x)/1024/1024)
#define MP(x) ((x)/1024*100/1024%100)

typedef struct
{
    CHAR *strName;
    UINT32 u4Addr;
    UINT32 u4Size;
}Mem;


void LogMem(Mem *M)
{
#if 1//ndef CC_MEM_TOOL
    LOG(1, "\t%8s \tAddr: %3d.%02dM End: %3d.%02dM Size: %3d.%02dM (0x%08X~0x%08X,0x%08X)\n", M->strName,
        MD(M->u4Addr), MP(M->u4Addr),
        MD(M->u4Addr + M->u4Size), MP(M->u4Addr + M->u4Size),
        MD(M->u4Size), MP(M->u4Size),
        M->u4Addr, M->u4Addr + M->u4Size, M->u4Size);
#else
    LOG(1, "%8s\t0x%08X\t0x%08X\n",M->strName,M->u4Addr,M->u4Size);
#endif
        
}

void vMemShow(Mem* M, UINT32 u4Len, UINT32 u4SortType)
{
    UINT32 i, j;
    UINT32 u4tmp;
    CHAR* strtmp;

    if (u4SortType == 1)
    {
        for (i = 0;i < u4Len; i++)
        {
            for (j = i + 1;j < u4Len;j++)
            {

                if (M[i].u4Addr > M[j].u4Addr)
                {
                    u4tmp = M[i].u4Addr;
                    M[i].u4Addr = M[j].u4Addr;
                    M[j].u4Addr = u4tmp;

                    u4tmp = M[i].u4Size;
                    M[i].u4Size = M[j].u4Size;
                    M[j].u4Size = u4tmp;

                    strtmp = M[i].strName;
                    M[i].strName = M[j].strName;
                    M[j].strName = strtmp;
                }

            }
        }
    }


    for (i = 0;i < u4Len; i++)
    {
        LogMem(M + i);
    }

}

void SRM_TStatus(void)
{
    Mem M[10];
    UINT32 n;

    if (1)
    {

        n = 0;
        M[n].strName = "Kernel";
        M[n].u4Addr = 0;
        M[n].u4Size = (DRVCUST_OptGet(eLinuxKernelMemSize));
        n ++;

        M[n].strName = "DirectFB";
        M[n].u4Addr = M[n-1].u4Addr + M[n-1].u4Size;
        M[n].u4Size = (DRVCUST_OptGet(eDirectFBMemSize)) + (DRVCUST_OptGet(eFBMemSize));
        n ++;

        M[n].strName = "FBM";
        M[n].u4Addr = DRVCUST_OptGet(eFbmMemAddr);
        M[n].u4Size = DRVCUST_OptGet(eFbmMemSize);
        n ++;

        vMemShow(M, n, 1);

        n = 0;
        M[n].strName = "Total";
        M[n].u4Addr = 0;
        M[n].u4Size = (DRVCUST_OptGet(eTotalMemSize));
        n ++;

        vMemShow(M, n, 0);    

    }
}



void SRM_CStatus(void)//ctc
{
    UINT32 u4VdpId = 0;

    FBM_POOL_T* prFbmPool;

    Mem M[20];
    UINT32 n;
    
    n = 0;
    for (u4VdpId = 0;u4VdpId <= ((_eApplicationMode == SRM_TV_MODE_TYPE_PIP || _eApplicationMode == SRM_TV_MODE_TYPE_POP) ? 1 : 0); u4VdpId++)
    {
        if (_arSrmVdpInfo[u4VdpId].u4Source == VSS_DTV)
        {
            prFbmPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_OD, NULL);
            M[n].strName = "OD";
            M[n].u4Addr = (prFbmPool->u4Addr);
            M[n].u4Size = (prFbmPool->u4Size);
            if (M[n].u4Size > 0) n ++;
        }

        if (_arSrmVdpInfo[u4VdpId].u4Source == VSS_DTV || SRM_GetMmMode() == SRM_MM_MODE_TYPE_VIDEO)
        {
            prFbmPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_DMX);
            M[n].strName = "DMX";
            M[n].u4Addr = (prFbmPool->u4Addr);
            M[n].u4Size = (prFbmPool->u4Size);
            if (M[n].u4Size > 0) n ++;
        }

        if (_arSrmVdpInfo[u4VdpId].u4Source == VSS_ATV)
        {
            prFbmPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_VBI, NULL);
            M[n].strName = "VBI";
            M[n].u4Addr = (prFbmPool->u4Addr);
            M[n].u4Size = (prFbmPool->u4Size);
            if (M[n].u4Size > 0) n ++;
        }

        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_BOB_CTRL].u4Arg1 & (FBM_POOL_MODE_MDDI_FULL | FBM_POOL_MODE_MDDI_CBOB)
                || (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_DISP_CTRL].u4Arg1 & (FBM_POOL_MODE_SCPOS_PSCAN_DISP)))
        {
            prFbmPool = FBM_GetPoolInfoAuto((UCHAR)(u4VdpId == 0 ? FBM_POOL_TYPE_MDDI_MAIN : FBM_POOL_TYPE_MDDI_PIP), NULL);
            M[n].strName = u4VdpId == 0 ? "MDDI(M)" : "MDDI(P)";
            M[n].u4Addr = (prFbmPool->u4Addr);
            M[n].u4Size = (prFbmPool->u4Size);
            if (M[n].u4Size > 0) n ++;

        }

        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_DISP_CTRL].u4Arg1 & (FBM_POOL_MODE_SCPOS_FULL))
        {
            prFbmPool = FBM_GetPoolInfoAuto((UCHAR)(u4VdpId == 0 ? FBM_POOL_TYPE_SCPOS_MAIN : FBM_POOL_TYPE_SCPOS_PIP), NULL);
            M[n].strName = u4VdpId == 0 ? "SCPOS(M)" : "SCPOS(P)";
            M[n].u4Addr = (prFbmPool->u4Addr);
            M[n].u4Size = (prFbmPool->u4Size);
            if (M[n].u4Size > 0) n ++;
        }
    }

    if (SRM_GetMmMode() != SRM_MM_MODE_TYPE_OFF)
    {
        prFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_FEEDER_MMP);
        M[n].strName = "Feeder(MMP)";
        M[n].u4Addr = (prFbmPool->u4Addr);
        M[n].u4Size = (prFbmPool->u4Size);
        if (M[n].u4Size > 0) n ++;

    }

    if (SRM_GetMmMode() == SRM_MM_MODE_TYPE_JPEG)
    {
        prFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_JPEG);
        M[n].strName = "JPEG";
        M[n].u4Addr = (prFbmPool->u4Addr);
        M[n].u4Size = (prFbmPool->u4Size);
        if (M[n].u4Size > 0) n ++;

        prFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_JPEG_OSD1);
        M[n].strName = "JPEG(OSD1))";
        M[n].u4Addr = (prFbmPool->u4Addr);
        M[n].u4Size = (prFbmPool->u4Size);
        if (M[n].u4Size > 0) n ++;

        prFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_JPEG_OSD2);
        M[n].strName = "JPEG(OSD2)";
        M[n].u4Addr = (prFbmPool->u4Addr);
        M[n].u4Size = (prFbmPool->u4Size);
        if (M[n].u4Size > 0) n ++;

        prFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_JPEG_OSD3);
        M[n].strName = "JPEG(OSD3))";
        M[n].u4Addr = (prFbmPool->u4Addr);
        M[n].u4Size = (prFbmPool->u4Size);
        if (M[n].u4Size > 0) n ++;

        prFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_JPEG_OSD4);
        M[n].strName = "JPEG(OSD4)";
        M[n].u4Addr = (prFbmPool->u4Addr);
        M[n].u4Size = (prFbmPool->u4Size);
        if (M[n].u4Size > 0) n ++;

    }

    if (SRM_GetMmMode() == SRM_MM_MODE_TYPE_VIDEO)
    {
        prFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG);
        M[n].strName = "MPEG";
        M[n].u4Addr = (prFbmPool->u4Addr);
        M[n].u4Size = (prFbmPool->u4Size);
        if (M[n].u4Size > 0) n ++;
    }

    if (1)
    {
        prFbmPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_DSP);
        M[n].strName = "DSP";
        M[n].u4Addr = (prFbmPool->u4Addr);
        M[n].u4Size = (prFbmPool->u4Size);
        if (M[n].u4Size > 0) n ++;
    }

    vMemShow(M, n, 1);
    
    n = 0;
    M[n].strName = "FBM";
    M[n].u4Addr = DRVCUST_OptGet(eFbmMemAddr);
    M[n].u4Size = DRVCUST_OptGet(eFbmMemSize);
    n ++;

    vMemShow(M, n, 0);
    
#if 0
    for (u4VdpId = 0; u4VdpId < VDP_NS; u4VdpId++)
    {
        LOG(1, "SRM(%d) E(%d) S(%d) 444(%d) VGA(%d) RES(%d, %d) W(%d) H(%d) I(%d) FR(%d) PIP(%d) POP(%d) Flip(%d)\n",
            u4VdpId,
            _arSrmVdpInfo[u4VdpId].u4Enable,
            _arSrmVdpInfo[u4VdpId].u4Status,
            _arSrmVdpInfo[u4VdpId].u4ColorFormat444,
            _arSrmVdpInfo[u4VdpId].u4VgaTiming,
            _arSrmVdpInfo[u4VdpId].u4Resolution,
            _arSrmVdpInfo[u4VdpId].u4MpegResolution,
            _arSrmVdpInfo[u4VdpId].u4InputWidth,
            _arSrmVdpInfo[u4VdpId].u4InputHeight,
            _arSrmVdpInfo[u4VdpId].u4Interlace,
            _arSrmVdpInfo[u4VdpId].u4InputFrameRate,
            SRM_IsPipVideo(), SRM_IsPopVideo(),
            u4QueryFlipModule(u4VdpId));

        LOG(1, "SRM(%d) CTRL MJC(0x%x) BOB(0x%x) NR(0x%x) DISP(0x%x) MUTE(0x%x) WE(0x%x) MPEG(0x%x)\n",
            u4VdpId,
            _arSrmVdpCtrl[u4VdpId].rPrm[SRM_MJC_CTRL].u4Arg1,
            _arSrmVdpCtrl[u4VdpId].rPrm[SRM_BOB_CTRL].u4Arg1,
            _arSrmVdpCtrl[u4VdpId].rPrm[SRM_NR_CTRL].u4Arg1,
            _arSrmVdpCtrl[u4VdpId].rPrm[SRM_DISP_CTRL].u4Arg1,
            _arSrmVdpCtrl[u4VdpId].rPrm[SRM_MUTE_CTRL].u4Arg1,
            _arSrmVdpCtrl[u4VdpId].rPrm[SRM_SCALER_WE_CTRL].u4Arg1,
            _arSrmVdpCtrl[u4VdpId].rPrm[SRM_MPEG_PIP_CTRL].u4Arg1);
    }
#endif

#if 0
    for (u4VdpId = 0; u4VdpId < VDP_NS; u4VdpId++)
    {
        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_DISP_CTRL].u4Arg1 & FBM_POOL_MODE_UNSTABLE)
        {
            //LOG(3, "FBM_POOL_MODE_UNSTABLE\n");
            break;
        }
        
        LOG(1,"C\t%s\tSCPOS\t", (u4VdpId == VDP_1 ? "MAIN":"SUB"));
        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_DISP_CTRL].u4Arg1 & FBM_POOL_MODE_8BIT)
        {
            LOG(1, "8BIT\t");
        }
        else if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_DISP_CTRL].u4Arg1 & FBM_POOL_MODE_10BIT)
        {
            LOG(1, "10BIT\t");
        }
        else
        {
            LOG(1, "?BIT\t");
        }
        

        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_DISP_CTRL].u4Arg1 & FBM_POOL_MODE_SCPOS_DISP)
        {
            LOG(1, "DISP\t");
        }
        else if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_DISP_CTRL].u4Arg1 & FBM_POOL_MODE_SCPOS_FULL)
        {
            LOG(1, "FULL\t");
        }
        else if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_DISP_CTRL].u4Arg1 & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
        {
            LOG(1, "PSCAN_DISP\t");
        }
        else if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_DISP_CTRL].u4Arg1 & FBM_POOL_MODE_SCPOS_MJC_DISP)
        {
            LOG(1, "MJC_DISP\t");
        }
        else if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_DISP_CTRL].u4Arg1 & FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR)
        {
            LOG(1, "BYPASS_PSCANNR\t");
        }
        else
        {
            LOG(1, "?\t");
        }
        LOG(1,"\n");

        LOG(1, "C\t%s\tMDDI\t", (u4VdpId == VDP_1 ? "MAIN":"SUB"));
        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_BOB_CTRL].u4Arg1 & FBM_POOL_MODE_8BIT)
        {
            LOG(1, "8BIT\t");
        }
        else if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_BOB_CTRL].u4Arg1 & FBM_POOL_MODE_10BIT)
        {
            LOG(1, "10BIT\t");
        }
        else
        {
            LOG(1, "?BIT\t");
        }
//        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_BOB_CTRL].u4Arg1 & FBM_POOL_MODE_MDDI_DISP)
//        {
//            LOG(1, "DISP\t");
//        }
        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_BOB_CTRL].u4Arg1 & FBM_POOL_MODE_MDDI_BOB)
        {
            LOG(1, "BOB\t");
        }
        else if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_BOB_CTRL].u4Arg1 & FBM_POOL_MODE_MDDI_CBOB)
        {
            LOG(1, "CBOB\t");
        }
        else if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_BOB_CTRL].u4Arg1 & FBM_POOL_MODE_MDDI_FULL)
        {
            LOG(1, "FULL\t");
        }
        else
        {
            LOG(1, "?\t");
        }

        if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_BOB_CTRL].u4Arg1 & FBM_POOL_MODE_MDDI_NR_Y_C)
        {
            LOG(1, "NR_Y_C\t");
        }
        else if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_BOB_CTRL].u4Arg1 & FBM_POOL_MODE_MDDI_NR_Y_ONLY)
        {
            LOG(1, "NR_Y_ONLY\t");
        }
        else if (_arSrmVdpCtrl[u4VdpId].rPrm[SRM_BOB_CTRL].u4Arg1 & FBM_POOL_MODE_MDDI_NR_OFF)
        {
            LOG(1, "NR_OFF\t");
        }
        else
        {
            LOG(1,"?\t");
        }
        LOG(1,"\n");
    }
#endif

}

#endif



UINT32 SRM_IsEpgMode(VOID)
{
    if ((_arSrmVdpInfo[VDP_1].u4Enable != 0) &&
        (_arSrmVdpInfo[VDP_1].u4OutputWidth < _SrmGetMaxVdpWidth()/3) &&
        (_arSrmVdpInfo[VDP_1].u4OutputHeight < _SrmGetMaxVdpHeight()/3))
    {
        return 1;
    }

    return 0;
}
 
UINT32 SRM_IsMainVerticalFull(VOID) // for mt5396 scaler line-sync display mode
{
    if ((_arSrmVdpInfo[VDP_1].u4Enable != 0) &&
        ((_arSrmVdpInfo[VDP_1].u4OutputHeight < _SrmGetMaxVdpHeight()) 
        || (VDP_Get121Map(VDP_1) && (_arSrmVdpInfo[VDP_1].u4InputHeight < PANEL_GetPanelHeight()))))
    {
        return 1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of SRM_IsDualVideo.
 */
//-----------------------------------------------------------------------------
UINT32 SRM_IsDualVideo(VOID)
{
    if ((_eApplicationMode == SRM_TV_MODE_TYPE_NORMAL) && (_u1tvmode_scart == 1))
    {
        return 1;
    }

    if (_eApplicationMode == SRM_TV_MODE_TYPE_LAST_VALID_ENTRY)
    {
        if ((_arSrmVdpInfo[VDP_1].u4Enable != 0) &&
            (_arSrmVdpInfo[VDP_1].u4OutputWidth != 0) &&
            (_arSrmVdpInfo[VDP_1].u4OutputHeight != 0) &&
            (_arSrmVdpInfo[VDP_2].u4Enable != 0) &&
            (_arSrmVdpInfo[VDP_2].u4OutputWidth != 0) &&
            (_arSrmVdpInfo[VDP_2].u4OutputHeight != 0))
        {
            return 1;
        }
    }
    else if (_eApplicationMode != SRM_TV_MODE_TYPE_NORMAL)
    {
        return 1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of SRM_IsPipVideo.
 */
//-----------------------------------------------------------------------------
UINT32 SRM_IsPipVideo(VOID)
{
    if ((_eApplicationMode == SRM_TV_MODE_TYPE_NORMAL) && (_u1tvmode_scart == 1))
    {
        return 1;
    }
    if (_eApplicationMode == SRM_TV_MODE_TYPE_LAST_VALID_ENTRY)
    {
        if (SRM_IsDualVideo())
        {
            if ((_arSrmVdpInfo[VDP_1].u4OutputWidth + _arSrmVdpInfo[VDP_2].u4OutputWidth) > _SrmGetMaxVdpWidth())
            {
                return 1;
            }
        }
    }
    else if (_eApplicationMode == SRM_TV_MODE_TYPE_PIP)
    {
        return 1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of SRM_IsPopVideo.
 */
//-----------------------------------------------------------------------------
UINT32 SRM_IsPopVideo(VOID)
{
    if ((_eApplicationMode == SRM_TV_MODE_TYPE_NORMAL) && (_u1tvmode_scart == 1))
    {
        return 0;
    }

    if (_eApplicationMode == SRM_TV_MODE_TYPE_LAST_VALID_ENTRY)
    {
        if (SRM_IsDualVideo())
        {
            if ((_arSrmVdpInfo[VDP_1].u4OutputWidth + _arSrmVdpInfo[VDP_2].u4OutputWidth) <= _SrmGetMaxVdpWidth())
            {
                return 1;
            }
        }
    }
    else if (_eApplicationMode == SRM_TV_MODE_TYPE_POP)
    {
        return 1;
    }

    return 0;
}

UINT32 SRM_IsGameMode(UINT32 u4VdpId)
{
    return _u4EnterGameMode[u4VdpId];
}

UINT32 SRM_IsScanMode(UINT32 u4VdpId)
{
    // extend the definition of scan mode to atv input source
    return (_u4EnterScanMode[u4VdpId] || (_arSrmVdpInfo[u4VdpId].u4Source == (UINT32) VSS_ATV) || (_arSrmVdpInfo[u4VdpId].u4Source == (UINT32) VSS_ATD));
}

#if defined(CC_DOWN_BW_WITH_MENU)
UINT32 SRM_IsDownBWMode(void)
{
    return _u4EnterDownBWMode;
}
#endif

UINT32 SRM_IsPauseMode(void)
{
    return _u4SrmPause;
}

#if defined(CC_DTV_HD_DOT_BY_DOT)
UINT32 SRM_IsForce2Zoom(void)
{
    return (_u4EnterZoomMode == 2);
}

UINT32 SRM_IsForce2DotByDot(void)
{
    return (_u4EnterZoomMode == 1);
}

void SRM_ClrZoomDBDChg(void)
{
    _u4EnterZoomMode = 0;
}
#endif
void SRM_SetCurrentFBMMode(UINT32 u4FBMMode)
{
   _u1FBMmode = u4FBMMode;
}
UINT32 SRM_GetCurrFBMMode(void)
{
  UINT32 u4Mode=0;
   if(_u1FBMmode == 0)
	u4Mode = FBM_MMMODE;
   if(_u1FBMmode == 1)
   	u4Mode = FBM_TVMODE;
   if(_u1FBMmode == 2)
	u4Mode = FBM_NETMODE;
   return u4Mode;
}

UINT32 SRM_GetMMSizeMode(UINT32 u4VdpId)
{
    return _MMBufferInfo[u4VdpId].u4Size;
}
UINT32 SRM_ISMMBufChg(UINT32 u4VdpId)
{
    return _MMBufferInfo[u4VdpId].u4Trigger;
}
VOID SRM_CLRMMBufChg(UINT32 u4VdpId)
{
    _MMBufferInfo[u4VdpId].u4Trigger = 0;
}

//-----------------------------------------------------------------------------
/** Brief of SRM_IsVideoStrictStable.
 */
//-----------------------------------------------------------------------------
UINT32 SRM_IsVideoStrictStable(VOID)
{
#ifdef CC_SCPOS_EN
    UINT32 u4Stable;
    UINT32 u4DualVideo;

    u4Stable = 1;

    if (_eApplicationMode == SRM_TV_MODE_TYPE_LAST_VALID_ENTRY)
    {
        // When Special VGA, Source = VGA, however, Enable =  0
//        if ((_arSrmVdpInfo[VDP_2].u4Enable == 0) &&
//          (_arSrmVdpInfo[VDP_2].u4Source == (UINT32)SV_VS_MAX))
        if (_arSrmVdpInfo[VDP_2].u4Enable == 0)
        {
            u4DualVideo = 0;
        }
        else
        {
            u4DualVideo = 1;
        }
    }
    else
    {
        if (_eApplicationMode == SRM_TV_MODE_TYPE_NORMAL)
        {
            u4DualVideo = 0;
        }
        else
        {
            u4DualVideo = 1;
        }
    }
    if ((_eApplicationMode == SRM_TV_MODE_TYPE_NORMAL) && (_u1tvmode_scart == 1))
    {
        u4DualVideo = 1;
    }

    if (u4DualVideo == 0)
    {
        // Single Video

        u4Stable = SRM_IsMainVideoStrictStable();
    }
    else
    {
        if (SRM_IsMainVideoStrictStable() == 0)
        {
            u4Stable = 0;
        }

        if (SRM_IsSubVideoStrictStable() == 0)
        {
            u4Stable = 0;
        }
    }

    return u4Stable;
#else
    return 1;
#endif
}

//-----------------------------------------------------------------------------
/** Brief of SRM_IsMainVideoStrictStable.
 */
//-----------------------------------------------------------------------------
UINT32 SRM_IsMainVideoStrictStable(VOID)
{
    return SRM_CheckStable(&_arSrmVdpInfo[VDP_1]);
}

UINT32 SRM_IsSubVideoStrictStable(VOID)
{
    return SRM_CheckStable(&_arSrmVdpInfo[VDP_2]);
}

//-----------------------------------------------------------------------------
/** Brief of SRM_IsVideoStable.
 */
//-----------------------------------------------------------------------------
UINT32 SRM_IsVideoStable(VOID)
{
#ifdef CC_SCPOS_EN
    UINT32 u4Stable;
    UINT32 u4DualVideo;

    u4Stable = 1;

    if (_eApplicationMode == SRM_TV_MODE_TYPE_LAST_VALID_ENTRY)
    {
        // When Special VGA, Source = VGA, however, Enable =  0
//        if ((_arSrmVdpInfo[VDP_2].u4Enable == 0) &&
//          (_arSrmVdpInfo[VDP_2].u4Source == (UINT32)SV_VS_MAX))
        if (_arSrmVdpInfo[VDP_2].u4Enable == 0)
        {
            u4DualVideo = 0;
        }
        else
        {
            u4DualVideo = 1;
        }
    }
    else
    {
        if (_eApplicationMode == SRM_TV_MODE_TYPE_NORMAL)
        {
            u4DualVideo = 0;
        }
        else
        {
            u4DualVideo = 1;
        }
    }
    if ((_eApplicationMode == SRM_TV_MODE_TYPE_NORMAL) && (_u1tvmode_scart == 1))
    {
        u4DualVideo = 1;
    }

    if (u4DualVideo == 0)
    {
        return SRM_IsVideoStrictStable();
    }
    else
    {
        u4Stable = 1;
    }

    return u4Stable;
#else
    return 1;
#endif
}

UINT32 SRM_CheckStable(SRM_VDP_INFO_T* prSrmVdpInfo)
{
    UINT32 u4Stable;

    u4Stable = 1;

    if (prSrmVdpInfo->u4Resolution == 0)
    {
        u4Stable = 0;
    }

    if (prSrmVdpInfo->u4Status == 0)
    {
        u4Stable = 0;
    }

    if (prSrmVdpInfo->u4Enable == 0)
    {
        u4Stable = 0;
    }

    if (prSrmVdpInfo->u4Source == (UINT32)VSS_MAX)
    {
        u4Stable = 0;
    }
/*
    else
    if (prSrmVdpInfo->u4Source == (UINT32)VSS_DTV)
    {
        if (prSrmVdpInfo->u4MpegResolution == 0)
        {
            u4Stable = 0;
        }
    }
*/
    return u4Stable;
}

UINT32 SRM_Is3DModel(VOID)
{
	return 1;
}

#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
UINT32 SRM_GetScposMode(UINT32 u4VdpId)
{
    return _arSrmVdpCtrl[u4VdpId].rNewPrm[SRM_DISP_CTRL].u4Arg1;
}
UINT32 SRM_GetScposHPDSize(UINT32 u4VdpId)
{
    return _arSrmVdpCtrl[u4VdpId].rNewPrm[SRM_DISP_CTRL].u4Arg2;
}
UINT32 SRM_GetMddiMode(UINT32 u4VdpId)
{
    return _arSrmVdpCtrl[u4VdpId].rNewPrm[SRM_BOB_CTRL].u4Arg1;
}
UINT32 SRM_GetTDCMode(UINT32 u4VdpId)
{
    return _arSrmVdpCtrl[u4VdpId].rNewPrm[SRM_TDC_CTRL].u4Arg1;
}
#endif
