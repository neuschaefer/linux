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

#ifdef CC_SCPOS_EN
#include "source_table.h"  // nptv
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

#define SRM_DRAM_MONITOR_DELAY                  100        // 100 ms

#define SRM_DRAM_MONITOR_IDLE                   500         // Take a rest between each monitor

#define SRM_DRAM_UTITLIY_EMPTY                  50          // 5%

#define SRM_VDP_NS                              VDP_NS

#define SRM_VDP_SD_RESOLUTION                   (720 * 576)

#define SRM_VDP_720HD_RESOLUTION                (1280 * 720)

#define SRM_VDP_1080HD_RESOLUTION               (1920 * 1080)

#define SRM_DB_RULE_NS                          (16)

#define SRM_MIN_DRAM_CLOCK                      (10000000)      // 1M

#define SRM_DRAM_MEAN_FILTER_TAP                (4)

#define SRM_5372_MAX_BW                         (700)

#define SRM_5372_AVG_BW                         (600)

#define SRM_STABLE_TIMEOUT                      (4000)

#define SRM_SWAP_MUTE_LONG_TIMEOUT              (5000)

#define SRM_SWAP_MUTE_DTV_TIMEOUT               (2000)

#define SRM_SWAP_MUTE_SHORT_TIMEOUT             (100)


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

/** Brief of SRM_OSD_INFO_T
 */
typedef struct
{
    UINT32 u4Enable;
    UINT32 u4Resolution;
    UINT32 u4Bpp;
} SRM_OSD_INFO_T;

/** Brief of SRM_OSD_INFO_T
 */
typedef struct
{
    UINT32 u4Arg1;
    UINT32 u4Arg2;
    UINT32 u4Arg3;
} SRM_VDP_CTRL_PRM_T;

/** Brief of SRM_VDP_CRTL_T
 */
typedef struct
{
    UINT32 u4CtrlMode[SRM_FEATURE_CTRL_NS];
    SRM_VDP_CTRL_PRM_T rPrm[SRM_FEATURE_CTRL_NS];
    SRM_VDP_CTRL_PRM_T rNewPrm[SRM_FEATURE_CTRL_NS];
} SRM_VDP_CRTL_T;

/** Brief of SRM_MSG_T
 */
typedef struct
{
    UINT32 u4SwapMute;
    UINT32 u4MainSouce;
    UINT32 u4PipSouce;
    UINT32 u4DualStable;
    HANDLE_T hTimer;
    HANDLE_T hDtvTimer;
} SRM_SWAP_DETECT_T;

/** Brief of HAL_DRAMC_T
 */
typedef struct
{
    UINT32 u4Bmgp3ag                        : 4;
    UINT32 u4Bmgp2ag                        : 4;
    UINT32 u4Bmgp1ag                        : 4;
    UINT32 fgBmbp1                          : 1;
    UINT32 fgBmbp2                          : 1;
    UINT32 fgBmbp3                          : 1;
    UINT32 u4Reserved                       : 17;

LINT_SUPPRESS_NEXT_EXPRESSION(950)
} __attribute__ ((packed))  HAL_DRAMC_T;

typedef void (*FN_SRN_DB_RULE) (void);

#define SRM_CTRL_MODE_UNKOWN                    0
#define SRM_CTRL_MODE_NONE                      1
#define SRM_CTRL_MODE_DB_RULE                   2
#define SRM_CTRL_MODE_BONDING_RULE              3

#define SRM_ARG_UNKNOWN                         0xFFFFFFFF

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

static HANDLE_T _hMonitor = NULL;          // Hardware protection

static UINT32 _u4DramMaxUtilization = SRM_5372_MAX_BW;

static UINT32 _u4DramAvgUtilization = SRM_5372_AVG_BW;

static UINT32 _u4DramMeasureModule = SRM_DRV_TOTAL;

static SRM_CB_FUNC_T _rSrmCbFunc;

static SRM_VDP_INFO_T _arSrmVdpInfo[SRM_VDP_NS];

static SRM_VDP_CRTL_T _arSrmVdpCtrl[SRM_VDP_NS];

static FN_SRN_DB_RULE _apfSrmDbRule[SRM_DB_RULE_NS];

static UINT32 _u4SrmDbRuleNs;

static FN_SRN_DB_RULE _apfSrmDbRuleNotStable[SRM_DB_RULE_NS];

static UINT32 _u4SrmDbRuleNotStableNs;

static UINT32 _u4SrmPause;

static DRV_DBASE_T* _prDbase = NULL;

static SRM_TV_MODE_T _eApplicationMode = SRM_TV_MODE_TYPE_LAST_VALID_ENTRY;

static SRM_SWAP_DETECT_T _rSwapDetect = {0, 0, 0, 0, 0, 0};

#ifdef CC_SCPOS_EN
static UINT32 _u4StableTimeOut = 0;
#endif

static HANDLE_T _hStableTimer = 0;

static UINT32 _u4VgaMaxSupportPixelRate = 0;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define SRM_UPDATE_BOB(vdp, mode, arg1, arg2, arg3)                     \
    if ((_arSrmVdpCtrl[vdp].u4CtrlMode[SRM_BOB_CTRL] < mode)){          \
        _arSrmVdpCtrl[vdp].u4CtrlMode[SRM_BOB_CTRL] = mode;             \
        _arSrmVdpCtrl[vdp].rNewPrm[SRM_BOB_CTRL].u4Arg1 = arg1;         \
        _arSrmVdpCtrl[vdp].rNewPrm[SRM_BOB_CTRL].u4Arg2 = arg2;         \
        _arSrmVdpCtrl[vdp].rNewPrm[SRM_BOB_CTRL].u4Arg3 = arg3;         \
        LOG(5, "[SRM] BOB DB %d %d %d %d %d\n"                          \
            , vdp, mode, arg1, arg2, arg3);                             \
     }                                                                  \

#define SRM_UPDATE_NR(vdp, mode, arg1, arg2, arg3)                      \
    if ((_arSrmVdpCtrl[vdp].u4CtrlMode[SRM_NR_CTRL] < mode)){           \
        _arSrmVdpCtrl[vdp].u4CtrlMode[SRM_NR_CTRL] = mode;              \
        _arSrmVdpCtrl[vdp].rNewPrm[SRM_NR_CTRL].u4Arg1 = arg1;          \
        _arSrmVdpCtrl[vdp].rNewPrm[SRM_NR_CTRL].u4Arg2 = arg2;          \
        _arSrmVdpCtrl[vdp].rNewPrm[SRM_NR_CTRL].u4Arg3 = arg3;          \
        LOG(5, "[SRM] NR DB %d %d %d %d %d\n"                           \
            , vdp, mode, arg1, arg2, arg3);                             \
     }                                                                  \

#define SRM_UPDATE_DISP(vdp, mode, arg1, arg2, arg3)                    \
    if ((_arSrmVdpCtrl[vdp].u4CtrlMode[SRM_DISP_CTRL] < mode)){         \
        _arSrmVdpCtrl[vdp].u4CtrlMode[SRM_DISP_CTRL] = mode;            \
        _arSrmVdpCtrl[vdp].rNewPrm[SRM_DISP_CTRL].u4Arg1 = arg1;        \
        _arSrmVdpCtrl[vdp].rNewPrm[SRM_DISP_CTRL].u4Arg2 = arg2;        \
        _arSrmVdpCtrl[vdp].rNewPrm[SRM_DISP_CTRL].u4Arg3 = arg3;        \
        LOG(5, "[SRM] DISP DB %d %d %d %d %d\n"                         \
            , vdp, mode, arg1, arg2, arg3);                             \
     }                                                                  \

#define SRM_UPDATE_MUTE(vdp, mode, arg1, arg2, arg3)                    \
    if ((_arSrmVdpCtrl[vdp].u4CtrlMode[SRM_MUTE_CTRL] < mode)){         \
        _arSrmVdpCtrl[vdp].u4CtrlMode[SRM_MUTE_CTRL] = mode;            \
        _arSrmVdpCtrl[vdp].rNewPrm[SRM_MUTE_CTRL].u4Arg1 = arg1;        \
        _arSrmVdpCtrl[vdp].rNewPrm[SRM_MUTE_CTRL].u4Arg2 = arg2;        \
        _arSrmVdpCtrl[vdp].rNewPrm[SRM_MUTE_CTRL].u4Arg3 = arg3;        \
        LOG(5, "[SRM] MUTE DB %d %d %d %d %d\n"                         \
            , vdp, mode, arg1, arg2, arg3);                             \
     }                                                                  \

#define SRM_UPDATE_SCALER_WE(vdp, mode, arg1, arg2, arg3)               \
    if ((_arSrmVdpCtrl[vdp].u4CtrlMode[SRM_SCALER_WE_CTRL] < mode)){    \
        _arSrmVdpCtrl[vdp].u4CtrlMode[SRM_SCALER_WE_CTRL] = mode;       \
        _arSrmVdpCtrl[vdp].rNewPrm[SRM_SCALER_WE_CTRL].u4Arg1 = arg1;   \
        _arSrmVdpCtrl[vdp].rNewPrm[SRM_SCALER_WE_CTRL].u4Arg2 = arg2;   \
        _arSrmVdpCtrl[vdp].rNewPrm[SRM_SCALER_WE_CTRL].u4Arg3 = arg3;   \
        LOG(5, "[SRM] WE DB %d %d %d %d %d\n"                           \
            , vdp, mode, arg1, arg2, arg3);                             \
     }                                                                  \

#define SRM_UPDATE_MPEG_PIP(vdp, mode, arg1, arg2, arg3)                \
    if ((_arSrmVdpCtrl[vdp].u4CtrlMode[SRM_MPEG_PIP_CTRL] < mode)){     \
        _arSrmVdpCtrl[vdp].u4CtrlMode[SRM_MPEG_PIP_CTRL] = mode;        \
        _arSrmVdpCtrl[vdp].rNewPrm[SRM_MPEG_PIP_CTRL].u4Arg1 = arg1;    \
        _arSrmVdpCtrl[vdp].rNewPrm[SRM_MPEG_PIP_CTRL].u4Arg2 = arg2;    \
        _arSrmVdpCtrl[vdp].rNewPrm[SRM_MPEG_PIP_CTRL].u4Arg3 = arg3;    \
        LOG(5, "[SRM] MPEG DB %d %d %d %d %d\n"                         \
            , vdp, mode, arg1, arg2, arg3);                             \
     }                                                                  \

#define SRM_UPDATE_MISC_CTRL(vdp, mode, arg1, arg2, arg3)               \
    if ((_arSrmVdpCtrl[vdp].u4CtrlMode[SRM_MISC_CTRL] < mode)){         \
        _arSrmVdpCtrl[vdp].u4CtrlMode[SRM_MISC_CTRL] = mode;            \
        _arSrmVdpCtrl[vdp].rNewPrm[SRM_MISC_CTRL].u4Arg1 = arg1;        \
        _arSrmVdpCtrl[vdp].rNewPrm[SRM_MISC_CTRL].u4Arg2 = arg2;        \
        _arSrmVdpCtrl[vdp].rNewPrm[SRM_MISC_CTRL].u4Arg3 = arg3;        \
        LOG(5, "[SRM] MISC DB %d %d %d %d %d\n"                         \
            , vdp, mode, arg1, arg2, arg3);                             \
     }                                                                  \

#define SRM_CHECK_CB_FUNC_VERIFY(func, crc)     ((UINT32)func == ~((UINT32)crc))

#define SRM_BYTE3(value) ((value >> 24) & 0xFF)
#define SRM_BYTE2(value) ((value >> 16) & 0xFF)
#define SRM_BYTE1(value) ((value >> 8) & 0xFF)
#define SRM_BYTE0(value) (value & 0xFF)

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

#ifdef CC_SCPOS_EN

INLINE static void _SrmICBondingRule(void);

static void _SrmDataBaseRule1(void);

static void _SrmDataBaseRule2(void);

static void _SrmDataBaseRule3(void);

static void _SrmDataBaseRule4(void);

static void _SrmDataBaseRule5(void);

static void _SrmDataBaseRule6(void);

static void _SrmDataBaseRule7(void);

static void _SrmDataBaseRule8(void);

static void _SrmDataBaseRule9(void);

static void _SrmDataBaseRule10(void);

static void _SrmDataBaseRule21(void);

static void _SrmDataBaseRule22(void);

static void _SrmDataBaseRule23(void);

static void _SrmDataBaseRule24(void);

static void _SrmDataBaseRule25(void);

static void _SrmDataBaseRule30(void);

static void _SrmDataBaseRule40(void);

static void _SrmDataBaseRule50(void);

static void _SrmDataBaseRuleSwap(void);

static void _SrmDataBaseRuleSwapNotStable(void);

static void _SrmStableTimeout(HANDLE_T pt_tm_handle, VOID* pv_tag);

static void _SrmStartStableTimer(void);

static void _SrmDtvSwapTimeout(HANDLE_T pt_tm_handle, VOID* pv_tag);

static void _SrmSwapTimeout(HANDLE_T pt_tm_handle, VOID* pv_tag);

static void _SrmSwapMute(void);

static void _SrmSwapUnMute(void);

static UINT32 _SrmDualVideo(void);

static UINT32 _SrmPipVideo(void);

static UINT32 _SrmPopVideo(void);

static UINT32 _SrmVideoStrictStable(void);

static UINT32 _SrmMainVideoStrictStable(void);

#endif

static UINT32 _SrmVideoStable(void);

static void _SrmDramMonitor(void* pvArg);

static void _SrmEventHandler(void* pvArg);

#ifdef CC_SCPOS_EN

//-----------------------------------------------------------------------------
/** Brief of _SrmICBondingRule.
 */
//-----------------------------------------------------------------------------

LINT_SUPPRESS_NEXT_EXPRESSION(129)
INLINE static void _SrmICBondingRule(void)
{
    // Apply to all model
    //SRM_UPDATE_BOB(VDP_1, SRM_CTRL_MODE_BONDING_RULE, SRM_MDDI_MODE_BOB, 0, 0);
}

//-----------------------------------------------------------------------------
/** Brief of _SrmDataBaseRule21.
 */
//-----------------------------------------------------------------------------

static void _SrmDataBaseRule21(void)
{
    // Rule #21, DTV-HD (1080i/1080p), NR = 2D
    UINT32 u4Mode;
    UINT32 u4CtrlMode;

    u4Mode = SRM_NR_MODE_3D;
    u4CtrlMode = SRM_CTRL_MODE_NONE;

    if ((_arSrmVdpInfo[VDP_1].u4Enable != 0) &&
        (_arSrmVdpInfo[VDP_1].u4Source == (UINT32) VSS_DTV) &&
        (_arSrmVdpInfo[VDP_1].u4MpegResolution > SRM_VDP_720HD_RESOLUTION))
    {
        u4Mode = SRM_NR_MODE_2D;
        u4CtrlMode = SRM_CTRL_MODE_DB_RULE;
    }

    SRM_UPDATE_NR(VDP_1, u4CtrlMode, u4Mode, 0, 0);
}

//-----------------------------------------------------------------------------
/** Brief of _SrmDataBaseRule22.
 */
//-----------------------------------------------------------------------------

static void _SrmDataBaseRule22(void)
{
    // Rule #22, NR VGA Rule
    // 20061017, Request by MF Tien
    // 1. VGA input only: 3DNR turn off
    // 2. VGA(main)+VGA(sub): 3DNR turn off
    UINT32 u4Mode;
    UINT32 u4CtrlMode;

    u4Mode = SRM_NR_MODE_3D;
    u4CtrlMode = SRM_CTRL_MODE_NONE;

    // 1. VGA input only: 3DNR turn off
    if (_arSrmVdpInfo[VDP_1].u4VgaTiming != 0)
    {
        u4Mode = SRM_NR_MODE_2D;
        u4CtrlMode = SRM_CTRL_MODE_DB_RULE;
    }

    SRM_UPDATE_NR(VDP_1, u4CtrlMode, u4Mode, 0, 0);
}

//-----------------------------------------------------------------------------
/** Brief of _SrmDataBaseRule23.
 */
//-----------------------------------------------------------------------------

static void _SrmDataBaseRule23(void)
{
    // Rule #23, Shut off NR when NOT stable
    UINT32 u4Mode;
    UINT32 u4CtrlMode;

    u4Mode = SRM_NR_MODE_3D;
    u4CtrlMode = SRM_CTRL_MODE_NONE;

    if (_SrmVideoStable() == 0)
    {
        u4Mode = SRM_NR_MODE_2D;
        u4CtrlMode = SRM_CTRL_MODE_DB_RULE;
    }

    SRM_UPDATE_NR(VDP_1, u4CtrlMode, u4Mode, 0, 0);
}

//-----------------------------------------------------------------------------
/** Brief of _SrmDataBaseRule25.
 */
//-----------------------------------------------------------------------------

static void _SrmDataBaseRule25(void)
{
    // Rule #25, PIP and Main HD (1080i/1080p) Input, NR = 2D
    UINT32 u4Mode;
    UINT32 u4CtrlMode;

    u4Mode = SRM_NR_MODE_3D;
    u4CtrlMode = SRM_CTRL_MODE_NONE;

    if (_SrmDualVideo())
    {
        if (_arSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION)
        {
            u4Mode = SRM_NR_MODE_2D;
            u4CtrlMode = SRM_CTRL_MODE_DB_RULE;
        }
    }

    SRM_UPDATE_NR(VDP_1, u4CtrlMode, u4Mode, 0, 0);
}

//-----------------------------------------------------------------------------
/** Brief of _SrmDataBaseRule30.
 */
//-----------------------------------------------------------------------------

static void _SrmDataBaseRule30(void)
{
    // Rule #30, DTV pip control
    if (_arSrmVdpInfo[VDP_1].u4Enable != 0)
    {
        if (_arSrmVdpInfo[VDP_1].u4Source == (UINT32) VSS_DTV)
        {
            // DTV at main Video
            SRM_UPDATE_MPEG_PIP(VDP_1, SRM_CTRL_MODE_DB_RULE, SRM_MPEG_MODE_FULL, 0, 0);
        }
        else
        {
            // DTV at pip Video
            SRM_UPDATE_MPEG_PIP(VDP_1, SRM_CTRL_MODE_DB_RULE, SRM_MPEG_MODE_HALF, 0, 0);
        }
    }
}

//-----------------------------------------------------------------------------
/** Brief of _SrmDataBaseRuleSwap.
 */
//-----------------------------------------------------------------------------

static void _SrmDataBaseRuleSwap(void)
{
    if ((_SrmDualVideo()) && (_SrmVideoStrictStable()))
    {
        if (_rSwapDetect.u4SwapMute != 0)
        {
            LOG(3, "SWAP UNMUTE\n");

            VERIFY(x_timer_stop(_rSwapDetect.hTimer) == OSR_OK);

            VERIFY(x_timer_stop(_rSwapDetect.hDtvTimer) == OSR_OK);

            VERIFY(x_timer_start(_rSwapDetect.hTimer,
                SRM_SWAP_MUTE_SHORT_TIMEOUT,
                X_TIMER_FLAG_ONCE,
                _SrmSwapTimeout,
                NULL) == OSR_OK);
        }

        _rSwapDetect.u4SwapMute = 0;
        _rSwapDetect.u4MainSouce = _arSrmVdpInfo[VDP_1].u4Source;
        _rSwapDetect.u4PipSouce = _arSrmVdpInfo[VDP_2].u4Source;
        _rSwapDetect.u4DualStable = 1;

        _arSrmVdpInfo[VDP_1].u4NoSignal = 0;
        _arSrmVdpInfo[VDP_2].u4NoSignal = 0;
    }
    else
    {
        _rSwapDetect.u4SwapMute = 0;
        _rSwapDetect.u4MainSouce = _arSrmVdpInfo[VDP_1].u4Source;
        _rSwapDetect.u4PipSouce = _arSrmVdpInfo[VDP_2].u4Source;
        _rSwapDetect.u4DualStable = 0;

        _arSrmVdpInfo[VDP_1].u4NoSignal = 0;
        _arSrmVdpInfo[VDP_2].u4NoSignal = 0;
    }
}

//-----------------------------------------------------------------------------
/** Brief of _SrmDataBaseRuleSwapNotStable.
 */
//-----------------------------------------------------------------------------

static void _SrmDataBaseRuleSwapNotStable(void)
{
    if (_SrmDualVideo())
    {
        if ((_rSwapDetect.u4SwapMute == 0) &&
            (_rSwapDetect.u4DualStable != 0) &&
            (_arSrmVdpInfo[VDP_1].u4NoSignal == 0) &&
            (_arSrmVdpInfo[VDP_2].u4NoSignal == 0) &&
            (_rSwapDetect.u4MainSouce != (UINT32) SV_VS_MAX) &&
            (_rSwapDetect.u4PipSouce != (UINT32) SV_VS_MAX) &&
            (_rSwapDetect.u4MainSouce == _arSrmVdpInfo[VDP_2].u4Source) &&
            (_rSwapDetect.u4PipSouce == _arSrmVdpInfo[VDP_1].u4Source))
        {
            LOG(3, "SWAP MUTE\n");

            _rSwapDetect.u4SwapMute = 1;
            _rSwapDetect.u4DualStable = 0;

            // Timeout Protection
            VERIFY(x_timer_stop(_rSwapDetect.hTimer) == OSR_OK);

            VERIFY(x_timer_start(_rSwapDetect.hTimer,
                SRM_SWAP_MUTE_LONG_TIMEOUT,
                X_TIMER_FLAG_ONCE,
                _SrmSwapTimeout,
                NULL) == OSR_OK);

            VERIFY(x_timer_stop(_rSwapDetect.hDtvTimer) == OSR_OK);

            if ((_arSrmVdpInfo[VDP_1].u4Source == (UINT32) VSS_DTV) ||
                (_arSrmVdpInfo[VDP_2].u4Source == (UINT32) VSS_DTV))
            {
                VERIFY(x_timer_start(_rSwapDetect.hDtvTimer,
                    SRM_SWAP_MUTE_DTV_TIMEOUT,
                    X_TIMER_FLAG_ONCE,
                    _SrmDtvSwapTimeout,
                    NULL) == OSR_OK);
            }

            _SrmSwapMute();
        }
    }
}

//-----------------------------------------------------------------------------
/** Brief of _SrmStableTimeout.
 */
//-----------------------------------------------------------------------------

static void _SrmStableTimeout(HANDLE_T pt_tm_handle, VOID* pv_tag)
{
    UNUSED(pt_tm_handle);
    UNUSED(pv_tag);

    if (_SrmVideoStrictStable() == 0)
    {
        _u4StableTimeOut = 1;
    }

    LOG(3, "STABLE TIMEOUT (%d)\n", _u4StableTimeOut);

    SRM_SendEvent(SRM_DRV_SCPOS, SRM_SCPOS_EVENT_UNSTABLE, 0, 0);
}

//-----------------------------------------------------------------------------
/** Brief of _SrmStartStableTimer.
 */
//-----------------------------------------------------------------------------

static void _SrmStartStableTimer(void)
{
    if (_SrmDualVideo() != 0)
    {
        VERIFY(x_timer_stop(_hStableTimer) == OSR_OK);

        _u4StableTimeOut = 0;

        LOG(3, "START STABLE TIMER\n");

        VERIFY(x_timer_start(_hStableTimer,
            SRM_STABLE_TIMEOUT,
            X_TIMER_FLAG_ONCE,
            _SrmStableTimeout,
            NULL) == OSR_OK);
    }
}

//-----------------------------------------------------------------------------
/** Brief of _SrmSwapTimeout.
 */
//-----------------------------------------------------------------------------

static void _SrmSwapTimeout(HANDLE_T pt_tm_handle, VOID* pv_tag)
{
    UNUSED(pt_tm_handle);
    UNUSED(pv_tag);

    _rSwapDetect.u4SwapMute = 0;

    LOG(3, "SWAP TIMEOUT\n");

    SRM_SendEvent(SRM_DRV_SCPOS, SRM_SCPOS_EVENT_UNMUTE, 0, 0);
}

//-----------------------------------------------------------------------------
/** Brief of _SrmDtvSwapTimeout.
 */
//-----------------------------------------------------------------------------

static void _SrmDtvSwapTimeout(HANDLE_T pt_tm_handle, VOID* pv_tag)
{
    UINT32 u4Idx;
    UINT32 u4DtvReady;

    UNUSED(pt_tm_handle);
    UNUSED(pv_tag);

    u4DtvReady = 0;

    for (u4Idx = VDP_1; u4Idx <= VDP_2; u4Idx++)
    {
        if ((_arSrmVdpInfo[u4Idx].u4Source == (UINT32) VSS_DTV) &&
            (_arSrmVdpInfo[u4Idx].u4MpegResolution != 0))
        {
            u4DtvReady = 1;
        }
    }

    LOG(3, "DTV SWAP TIMEOUT\n");

    if (u4DtvReady == 0)
    {
        VERIFY(x_timer_stop(_rSwapDetect.hTimer) == OSR_OK);

        _SrmSwapTimeout(pt_tm_handle, pv_tag);
    }
}

//-----------------------------------------------------------------------------
/** Brief of _SrmSwapMute.
 */
//-----------------------------------------------------------------------------
static void _SrmSwapMute(void)
{
    if (SRM_CHECK_CB_FUNC_VERIFY(_rSrmCbFunc.au4CbFunc[SRM_MUTE_CTRL], _rSrmCbFunc.au4CbFuncCRC[SRM_MUTE_CTRL]))
    {
        ((FN_SRN_CALLBACK)_rSrmCbFunc.au4CbFunc[SRM_MUTE_CTRL])(VDP_1, SRM_SCPOS_MUTE_ON, 0, 0);
        ((FN_SRN_CALLBACK)_rSrmCbFunc.au4CbFunc[SRM_MUTE_CTRL])(VDP_2, SRM_SCPOS_MUTE_ON, 0, 0);
    }
    else
    {
        ASSERT(_rSrmCbFunc.au4CbFunc[SRM_MUTE_CTRL] == 0);
    }
}

//-----------------------------------------------------------------------------
/** Brief of _SrmSwapUnMute.
 */
//-----------------------------------------------------------------------------
static void _SrmSwapUnMute(void)
{
    if (SRM_CHECK_CB_FUNC_VERIFY(_rSrmCbFunc.au4CbFunc[SRM_MUTE_CTRL], _rSrmCbFunc.au4CbFuncCRC[SRM_MUTE_CTRL]))
    {
        ((FN_SRN_CALLBACK)_rSrmCbFunc.au4CbFunc[SRM_MUTE_CTRL])(VDP_1, SRM_SCPOS_MUTE_OFF, 0, 0);
        ((FN_SRN_CALLBACK)_rSrmCbFunc.au4CbFunc[SRM_MUTE_CTRL])(VDP_2, SRM_SCPOS_MUTE_OFF, 0, 0);
    }
    else
    {
        ASSERT(_rSrmCbFunc.au4CbFunc[SRM_MUTE_CTRL] == 0);
    }
}

//-----------------------------------------------------------------------------
/** Brief of _SrmDualVideo.
 */
//-----------------------------------------------------------------------------
static UINT32 _SrmDualVideo(void)
{
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

        return 0;
    }
    else
    {
        if (_eApplicationMode == SRM_TV_MODE_TYPE_NORMAL)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
}

//-----------------------------------------------------------------------------
/** Brief of _SrmPipVideo.
 */
//-----------------------------------------------------------------------------
static UINT32 _SrmPipVideo(void)
{
    if (_eApplicationMode == SRM_TV_MODE_TYPE_LAST_VALID_ENTRY)
    {
        if (_SrmDualVideo() != 0)
        {
            if ((_arSrmVdpInfo[VDP_1].u4OutputWidth + _arSrmVdpInfo[VDP_2].u4OutputWidth) > 1000)
            {
                return 1;
            }
        }

        return 0;
    }
    else
    {
        if (_eApplicationMode == SRM_TV_MODE_TYPE_PIP)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

//-----------------------------------------------------------------------------
/** Brief of _SrmPopVideo.
 */
//-----------------------------------------------------------------------------
static UINT32 _SrmPopVideo(void)
{
    if (_eApplicationMode == SRM_TV_MODE_TYPE_LAST_VALID_ENTRY)
    {
        if (_arSrmVdpInfo[VDP_1].u4OutputHeight != 1000)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if (_eApplicationMode == SRM_TV_MODE_TYPE_POP)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

//-----------------------------------------------------------------------------
/** Brief of _SrmVideoStrictStable.
 */
//-----------------------------------------------------------------------------
static UINT32 _SrmVideoStrictStable(void)
{
#ifdef CC_SCPOS_EN
    // not include stable time out

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

    if (u4DualVideo == 0)
    {
        // Single Video

        if (_arSrmVdpInfo[VDP_1].u4Resolution == 0)
        {
            u4Stable = 0;
        }

        if (_arSrmVdpInfo[VDP_1].u4Status == 0)
        {
            u4Stable = 0;
        }

        if (_arSrmVdpInfo[VDP_1].u4Enable == 0)
        {
            u4Stable = 0;
        }

        if (_arSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_MAX)
        {
            u4Stable = 0;
        }
        else
        if (_arSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV)
        {
            if (_arSrmVdpInfo[VDP_1].u4MpegResolution == 0)
            {
                u4Stable = 0;
            }
        }
    }
    else
    {
        UINT32 u4VdpId;

        // Dual Video

        for (u4VdpId = VDP_1; u4VdpId <= VDP_2; u4VdpId++)
        {
            if (_arSrmVdpInfo[u4VdpId].u4Resolution == 0)
            {
                u4Stable = 0;
            }

            if (_arSrmVdpInfo[u4VdpId].u4Status == 0)
            {
                u4Stable = 0;
            }

            if (_arSrmVdpInfo[u4VdpId].u4Enable == 0)
            {
                u4Stable = 0;
            }

            if (_arSrmVdpInfo[u4VdpId].u4Source == (UINT32)VSS_MAX)
            {
                u4Stable = 0;
            }
            else
            if (_arSrmVdpInfo[u4VdpId].u4Source == (UINT32)VSS_DTV)
            {
                if (_arSrmVdpInfo[u4VdpId].u4MpegResolution == 0)
                {
                    u4Stable = 0;
                }
            }
        }
    }

    return u4Stable;
#else
    return 1;
#endif
}

//-----------------------------------------------------------------------------
/** Brief of _SrmMainVideoStrictStable.
 */
//-----------------------------------------------------------------------------
static UINT32 _SrmMainVideoStrictStable(void)
{
#ifdef CC_SCPOS_EN
    UINT32 u4Stable;

    u4Stable = 1;

    if (_arSrmVdpInfo[VDP_1].u4Resolution == 0)
    {
        u4Stable = 0;
    }

    if (_arSrmVdpInfo[VDP_1].u4Status == 0)
    {
        u4Stable = 0;
    }

    if (_arSrmVdpInfo[VDP_1].u4Enable == 0)
    {
        u4Stable = 0;
    }

    if (_arSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_MAX)
    {
        u4Stable = 0;
    }
    else
    if (_arSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV)
    {
        if (_arSrmVdpInfo[VDP_1].u4MpegResolution == 0)
        {
            u4Stable = 0;
        }
    }

    return u4Stable;
#else
    return 1;
#endif
}

#endif


//-----------------------------------------------------------------------------
/** Brief of _SrmVideoStable.
 */
//-----------------------------------------------------------------------------
static UINT32 _SrmVideoStable(void)
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

    if (u4DualVideo == 0)
    {
        return _SrmVideoStrictStable();
    }
    else
    {
        u4Stable = _SrmVideoStrictStable();

        if (_u4StableTimeOut != 0)
        {
            u4Stable = 1;
        }
    }

    return u4Stable;
#else
    return 1;
#endif
}

//-----------------------------------------------------------------------------
/** Brief of _SrmDramMonitor.
 */
//-----------------------------------------------------------------------------

static void _SrmDramMonitor(void* pvArg)
{
    UNUSED(pvArg);

    while (1)
    {
#if 1
        UINT32 u4Result;
        UINT32 u4DelayClock;
        UINT64 u8Result;

        // NOTE: u4DelayClock should less than 0xffffffff
        u4DelayClock = 0x04000000;

        u4Result = SRM_DramMonitor(_u4DramMeasureModule, u4DelayClock, 2000);

        u8Result = (UINT64)u4Result;
        u8Result *= 1000;
        u8Result /= u4DelayClock;

        u4Result = (UINT32) u8Result;

        SRM_SendEvent(SRM_DRV_DRAM, SRM_DRV_TOTAL, u4Result, 0);
#else
        UNUSED(_u4DramMeasureModule);
#endif

        // Take a rest between each monitor
        x_thread_delay(SRM_DRAM_MONITOR_IDLE);
    }
}

//-----------------------------------------------------------------------------
/** Brief of _SrmEventHandler.
 */
//-----------------------------------------------------------------------------

static void _SrmEventHandler(void* pvArg)
{
    UNUSED(pvArg);

    while (1)
    {
        UINT32 u4EnterDataBase;
        UINT32 u4NoMessage;
        UINT16 u2MsgQIdx;
        SIZE_T zMsgSize;
        SRM_MSG_T rMsg;

        zMsgSize = sizeof(SRM_MSG_T);

        u4EnterDataBase = 0;
        VERIFY(x_msg_q_receive(&u2MsgQIdx, (void *) (&rMsg), &zMsgSize,
            &(_hSrmMsgQueue), 1, X_MSGQ_OPTION_WAIT) == OSR_OK);

        ASSERT(zMsgSize == sizeof(SRM_MSG_T));

        // handle all command first, then handle data base
        do
        {
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
            {
#ifdef CC_SCPOS_EN
                UINT32 u4StartStableTimer;
#endif

                LOG(5, "[SRM] Event %d 0x%0x %d %d\n",
                    rMsg.u4Arg1, rMsg.u4Arg2, rMsg.u4Arg3, rMsg.u4Arg4);

#ifdef CC_SCPOS_EN
                if (rMsg.u4Arg1 == SRM_DRV_SCPOS)
                {
                    UINT32 u4VdpId;

                    u4VdpId = rMsg.u4Arg2 & SRM_SCPOS_EVENT_MASK;

                    u4StartStableTimer = 0;

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

                        if (_arSrmVdpInfo[u4VdpId].u4Enable != rMsg.u4Arg3)
                        {
                            u4StartStableTimer = 1;
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

                        if (_arSrmVdpInfo[u4VdpId].u4SourceId != rMsg.u4Arg3)
                        {
                            u4StartStableTimer = 1;
                        }

                        _arSrmVdpInfo[u4VdpId].u4Source = u4SourceType;
                        _arSrmVdpInfo[u4VdpId].u4SourceId = rMsg.u4Arg3;

                        u4EnterDataBase = 1;
                    }
                    else
                    if (rMsg.u4Arg2 & SRM_SCPOS_EVENT_MPEG_SIZE)
                    {
                        if ((_arSrmVdpInfo[u4VdpId].u4MpegResolution != rMsg.u4Arg3) &&
                            (rMsg.u4Arg3 == 0))
                        {
                            u4StartStableTimer = 1;
                        }

                        _arSrmVdpInfo[u4VdpId].u4MpegResolution = rMsg.u4Arg3;

                        u4EnterDataBase = 1;
                    }
                    else
                    if (rMsg.u4Arg2 & SRM_SCPOS_EVENT_SIZE)
                    {
                        if ((_arSrmVdpInfo[u4VdpId].u4Resolution != rMsg.u4Arg3) &&
                            (rMsg.u4Arg3 == 0))
                        {
                            u4StartStableTimer = 1;
                        }

                        _arSrmVdpInfo[u4VdpId].u4Resolution = rMsg.u4Arg3;
                        _arSrmVdpInfo[u4VdpId].u4Interlace = rMsg.u4Arg4;

                        u4EnterDataBase = 1;
                    }
                    else
                    if (rMsg.u4Arg2 & SRM_SCPOS_EVENT_OUT_SIZE)
                    {
                        _arSrmVdpInfo[u4VdpId].u4OutputWidth = rMsg.u4Arg3;
                        _arSrmVdpInfo[u4VdpId].u4OutputHeight = rMsg.u4Arg4;

                        u4EnterDataBase = 1;
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

                        if (_arSrmVdpInfo[u4VdpId].u4Status != rMsg.u4Arg3)
                        {
                            u4StartStableTimer = 1;
                        }

                        _arSrmVdpInfo[u4VdpId].u4Status = rMsg.u4Arg3;
                        _arSrmVdpInfo[u4VdpId].u4VgaTiming = BYTE0(rMsg.u4Arg4);
                        _arSrmVdpInfo[u4VdpId].u4InputFrameRate = BYTE1(rMsg.u4Arg4);
                        _arSrmVdpInfo[u4VdpId].u4ColorFormat444 = BYTE2(rMsg.u4Arg4);

                        u4EnterDataBase = 1;
                    }
                    else
                    if (rMsg.u4Arg2 & SRM_SCPOS_EVENT_UNMUTE)
                    {
                        _SrmSwapUnMute();
                    }
                    else
                    if (rMsg.u4Arg2 & SRM_SCPOS_EVENT_UNSTABLE)
                    {
                        u4EnterDataBase = 1;
                    }

                    if (u4StartStableTimer !=0)
                    {
                        _SrmStartStableTimer();
                    }
                }
#endif
            }

            if (x_msg_q_receive(&u2MsgQIdx, (void *) (&rMsg), &zMsgSize,
                &(_hSrmMsgQueue), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
            {
                u4NoMessage = 0;
            }
            else
            {
                u4NoMessage = 1;
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

                if (_SrmVideoStable() != 0)
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
                            _pfSrmDbRule[u4RuleIdx]();
                        }
                    }

                    // Apply Bounding Rule
                    _SrmICBondingRule();

                    // Post Handle DB Rule
                    for (u4RuleIdx = VDP_1; u4RuleIdx <= VDP_2; u4RuleIdx++)
                    {
                        UINT32 u4Feature;

                        for (u4Feature = 0; u4Feature < SRM_FEATURE_CTRL_NS; u4Feature++)
                        {
                            if (_arSrmVdpCtrl[u4RuleIdx].u4CtrlMode[u4Feature] != SRM_CTRL_MODE_UNKOWN)
                            {
                                if (SRM_CHECK_CB_FUNC_VERIFY(_rSrmCbFunc.au4CbFunc[u4Feature], _rSrmCbFunc.au4CbFuncCRC[u4Feature]))
                                {
                                    if ((_arSrmVdpCtrl[u4RuleIdx].rNewPrm[u4Feature].u4Arg1 != _arSrmVdpCtrl[u4RuleIdx].rPrm[u4Feature].u4Arg1) ||
                                        (_arSrmVdpCtrl[u4RuleIdx].rNewPrm[u4Feature].u4Arg2 != _arSrmVdpCtrl[u4RuleIdx].rPrm[u4Feature].u4Arg2) ||
                                        (_arSrmVdpCtrl[u4RuleIdx].rNewPrm[u4Feature].u4Arg3 != _arSrmVdpCtrl[u4RuleIdx].rPrm[u4Feature].u4Arg3))
                                    {
                                        _arSrmVdpCtrl[u4RuleIdx].rPrm[u4Feature] = _arSrmVdpCtrl[u4RuleIdx].rNewPrm[u4Feature];

                                        ((FN_SRN_CALLBACK)_rSrmCbFunc.au4CbFunc[u4Feature])(u4RuleIdx,
                                            _arSrmVdpCtrl[u4RuleIdx].rPrm[u4Feature].u4Arg1,
                                            _arSrmVdpCtrl[u4RuleIdx].rPrm[u4Feature].u4Arg2,
                                            _arSrmVdpCtrl[u4RuleIdx].rPrm[u4Feature].u4Arg3);
                                    }
                                    else
                                    if ( (rMsg.u4Arg1 == SRM_DRV_SCPOS) &&
                                        (rMsg.u4Arg2 & SRM_SCPOS_EVENT_ONOFF))
                                    {
                                        if (u4Feature == SRM_NR_CTRL)
                                        {
                                            // trigger NR again when SCPOS ON/OFF

                                            ((FN_SRN_CALLBACK)_rSrmCbFunc.au4CbFunc[u4Feature])(u4RuleIdx,
                                                _arSrmVdpCtrl[u4RuleIdx].rPrm[u4Feature].u4Arg1,
                                                _arSrmVdpCtrl[u4RuleIdx].rPrm[u4Feature].u4Arg2,
                                                _arSrmVdpCtrl[u4RuleIdx].rPrm[u4Feature].u4Arg3);
                                        }
                                        else
                                        if (u4Feature == SRM_BOB_CTRL)
                                        {
                                            // trigger SRM_DRV_MDDI again when SCPOS ON/OFF

                                            ((FN_SRN_CALLBACK)_rSrmCbFunc.au4CbFunc[u4Feature])(u4RuleIdx,
                                                _arSrmVdpCtrl[u4RuleIdx].rPrm[u4Feature].u4Arg1,
                                                _arSrmVdpCtrl[u4RuleIdx].rPrm[u4Feature].u4Arg2,
                                                _arSrmVdpCtrl[u4RuleIdx].rPrm[u4Feature].u4Arg3);
                                        }
                                    }
                                }
                                else
                                {
                                    ASSERT(_rSrmCbFunc.au4CbFunc[u4Feature] == 0);
                                }
                            }
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

void SRM_Init(void)
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

        VERIFY(x_timer_create(&_rSwapDetect.hTimer) == OSR_OK);
        VERIFY(x_timer_create(&_rSwapDetect.hDtvTimer) == OSR_OK);
        VERIFY(x_timer_create(&_hStableTimer) == OSR_OK);

#ifdef CC_SRM_PAUSE
        _u4SrmPause = 1;
#else
        _u4SrmPause = 0;
#endif

        _u4VgaMaxSupportPixelRate = DRVCUST_OptGet(eVGAMaxRateForDispmode);

        // reset variable
        for (u4Idx = 0; u4Idx < SRM_FEATURE_CTRL_NS; u4Idx++)
        {
            _rSrmCbFunc.au4CbFunc[u4Idx] = NULL;
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

        for (u4Idx = 0; u4Idx < SRM_VDP_NS; u4Idx++)
        {
            _arSrmVdpInfo[u4Idx].u4Enable = 0;
            _arSrmVdpInfo[u4Idx].u4MpegResolution = 0;
            _arSrmVdpInfo[u4Idx].u4Resolution = 0;
        }

        // NR Rule #20~#29
        // Rule #21, DTV-HD (1080i/1080p), NR = 2D
        // Rule #22, NR VGA Rule
        // Rule #23, Shut off NR when NOT stable
        // Rule #25, PIP and HD (1080i/1080p) Input, NR = 2D
        // Rule #30, DTV pip control

#ifdef CC_SCPOS_EN
        // Remove Warning
        _apfSrmDbRule[0] = _SrmDataBaseRule21;
        _apfSrmDbRule[0] = _SrmDataBaseRule22;
        _apfSrmDbRule[0] = _SrmDataBaseRule23;
        _apfSrmDbRule[0] = _SrmDataBaseRule25;
        _apfSrmDbRule[0] = _SrmDataBaseRule30;
        _apfSrmDbRule[0] = _SrmDataBaseRuleSwap;
        _apfSrmDbRule[0] = _SrmDataBaseRuleSwapNotStable;

        // Remove Warning
        if (_SrmPipVideo() == 0)
        {
        }
        if (_SrmPopVideo() == 0)
        {
        }

        // prepare rule database while not stable
        _u4SrmDbRuleNotStableNs = 0;

        if (DRVCUST_OptGet(eFbmMemSize) == FBM_MEM_CFG_MT5391x128MB)
        {
            _apfSrmDbRule[0] = _SrmDataBaseRule21;
            _apfSrmDbRule[1] = _SrmDataBaseRule22;
            _apfSrmDbRule[2] = _SrmDataBaseRule23;
            _apfSrmDbRule[3] = _SrmDataBaseRule25;
            _apfSrmDbRule[4] = _SrmDataBaseRule30;
            _u4SrmDbRuleNs = 5;

            _apfSrmDbRuleNotStable[0] = _SrmDataBaseRule23;
            _apfSrmDbRuleNotStable[1] = _SrmDataBaseRule30;
            _u4SrmDbRuleNotStableNs = 2;
        }
        else
        {
            _u4SrmDbRuleNotStableNs = 0;
            _u4SrmDbRuleNs = 0;
        }
#else
        _u4SrmDbRuleNotStableNs = 0;
        _u4SrmDbRuleNs = 0;
#endif

        // set empty rule to NUL
        ASSERT(_u4SrmDbRuleNs <= SRM_DB_RULE_NS);
        for (u4Idx = _u4SrmDbRuleNs; u4Idx < SRM_DB_RULE_NS; u4Idx++)
        {
            _apfSrmDbRule[u4Idx] = 0;
        }

        // set empty rule to NUL
        ASSERT(_u4SrmDbRuleNotStableNs <= SRM_DB_RULE_NS);
        for (u4Idx = _u4SrmDbRuleNotStableNs; u4Idx < SRM_DB_RULE_NS; u4Idx++)
        {
            _apfSrmDbRuleNotStable[u4Idx] = 0;
        }

        // set max dram utilization, according to IC Model (5371/72/73) & Dram Model (DDR1*2/DDR1*4/DDR2*2)
        _u4DramMaxUtilization = SRM_5372_MAX_BW;
        _u4DramAvgUtilization = SRM_5372_AVG_BW;

        // create semaphore
        VERIFY(x_sema_create(&_hMonitor, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);

        // create thread
        if (x_thread_create(&hThread, "DRAM_M", 1024, (LOG_PRINT_PRIORITY - 1),
            _SrmDramMonitor, 0, NULL) != OSR_OK)
        {
            ASSERT(0);
        }

        // create message Q
        VERIFY(x_msg_q_create(&_hSrmMsgQueue, "SRM_Q", sizeof(SRM_MSG_T), 32) == OSR_OK);

        // create thread
        if (x_thread_create(&hThread, "SRM", 1024, (MLVDO_THREAD_PRIORITY - 1),
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

void SRM_Pause(void)
{
    _u4SrmPause = 1;
}

//-----------------------------------------------------------------------------
/** Brief of SRM_Resume.
 */
//-----------------------------------------------------------------------------

void SRM_Resume(void)
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

void SRM_SendEvent(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4)
{
    SRM_MSG_T rMsg;

    rMsg.u4Arg1 = u4Arg1;
    rMsg.u4Arg2 = u4Arg2;
    rMsg.u4Arg3 = u4Arg3;
    rMsg.u4Arg4 = u4Arg4;

    VERIFY (x_msg_q_send(_hSrmMsgQueue, (void *) (&rMsg), sizeof(SRM_MSG_T), 255) == OSR_OK);
}

//-----------------------------------------------------------------------------
/** Brief of SRM_RegisterCB.
 */
//-----------------------------------------------------------------------------

void SRM_RegisterCB(UINT32 u4Mod, FN_SRN_CALLBACK pfCbFun)
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

//-----------------------------------------------------------------------------
/** Brief of SRM_DramMonitor.
 */
//-----------------------------------------------------------------------------

UINT32 SRM_DramMonitor(UINT32 u4Mod, UINT32 u4DelayClock, UINT32 u4MonitorDelay)
{
    UINT32 u4AgendId;
    UINT32 u4GroupId;
    UINT32 u4Result;
    UINT32 u4Total;

    volatile HAL_DRAMC_T* _prDramcReg = (HAL_DRAMC_T*) (DRAM_BASE + 0x080);
    HAL_DRAMC_T rDramcReg;

    VERIFY(x_sema_lock(_hMonitor, X_SEMA_OPTION_WAIT) == OSR_OK);

    u4Total = 0;

    switch (u4Mod)
    {
        case SRM_DRV_TOTAL:
        u4Total = 1;
        u4GroupId = 1;
        u4AgendId = 1;
        break;

        case SRM_DRV_MPEG:
        u4AgendId = 1;
        u4GroupId = 2;
        break;

        case SRM_DRV_NR:
        u4AgendId = 2;
        u4GroupId = 1;
        break;

        case SRM_DRV_MDDI:
        u4AgendId = 4;
        u4GroupId = 1;
        break;

        case SRM_DRV_SCPOS:
        u4AgendId = 7;
        u4GroupId = 1;
        break;

        case SRM_DRV_OSD:
        u4AgendId = 3;
        u4GroupId = 1;
        break;

        case SRM_DRV_AUD:
        u4AgendId = 0;
        u4GroupId = 1;
        break;

        case SRM_DRV_DMX:
        u4AgendId = 1;
        u4GroupId = 1;
        break;

        case SRM_DRV_USB:
        u4AgendId = 0;
        u4GroupId = 2;
        break;

        case SRM_DRV_B2R:
        u4AgendId = 5;
        u4GroupId = 1;
        break;

        case SRM_DRV_CPU:
        u4AgendId = 6;
        u4GroupId = 1;
        break;

        case SRM_DRV_GFX:
        u4AgendId = 0;
        u4GroupId = 3;
        break;

        case SRM_DRV_JPEG:
        u4AgendId = 2;
        u4GroupId = 2;
        break;

        case SRM_DRV_PSR:
        u4AgendId = 3;
        u4GroupId = 2;
        break;

        case SRM_DRV_DSP:
        u4AgendId = 4;
        u4GroupId = 2;
        break;

        case SRM_DRV_TCM:
        u4AgendId = 3;
        u4GroupId = 3;
        break;

        case SRM_DRV_IDETEST:
        u4AgendId = 4;
        u4GroupId = 3;
        break;

        case SRM_DRV_UART:
        u4AgendId = 5;
        u4GroupId = 3;
        break;

        case SRM_DRV_FCI:
        u4AgendId = 5;
        u4GroupId = 2;
        break;

        case SRM_DRV_MCBAK:
        u4AgendId = 8;
        u4GroupId = 1;
        break;

        case SRM_DRV_USB2:
        u4AgendId = 1;
        u4GroupId = 3;
        break;

        default:
        u4Total = 1;
        u4GroupId = 1;
        u4AgendId = 1;
        break;
    }

    if (u4GroupId != 1)
    {
        u4AgendId = (u4AgendId) ^ (u4AgendId >> 1);
    }

    IO_REG32(DRAM_BASE, 0x084) = u4DelayClock;

    rDramcReg = *_prDramcReg;
    rDramcReg.fgBmbp1 = 0;
    rDramcReg.fgBmbp2 = 0;
    rDramcReg.fgBmbp3 = 0;
    if (u4Total != 0)
    {
        // All agent, including group 1, 2, and 3
        rDramcReg.u4Bmgp1ag = 0xF;
    }
    else
    if (u4GroupId == 1)
    {
        rDramcReg.u4Bmgp1ag = u4AgendId;
    }
    else
    if (u4GroupId == 2)
    {
        rDramcReg.u4Bmgp2ag = u4AgendId;
    }
    else
    if (u4GroupId == 3)
    {
        rDramcReg.u4Bmgp3ag = u4AgendId;
    }
    *_prDramcReg = rDramcReg;
    if (u4GroupId == 1)
    {
        rDramcReg.fgBmbp1 = 1;
    }
    else
    if (u4GroupId == 2)
    {
        rDramcReg.fgBmbp2 = 1;
    }
    else
    if (u4GroupId == 3)
    {
        rDramcReg.fgBmbp3 = 1;
    }

    *_prDramcReg = rDramcReg;

    x_thread_delay(u4MonitorDelay);

    u4Result = 0;
    if (u4GroupId == 1)
    {
        u4Result = IO_REG32(DRAM_BASE, 0x3C8);
    }
    else
    if (u4GroupId == 2)
    {
        u4Result = IO_REG32(DRAM_BASE, 0x3CC);
    }
    else
    if (u4GroupId == 3)
    {
        u4Result = IO_REG32(DRAM_BASE, 0x3D0);
    }

    VERIFY(x_sema_unlock(_hMonitor) == OSR_OK);

    return u4Result;
}

//-----------------------------------------------------------------------------
/** Brief of SRM_GetTvMode.
 */
//-----------------------------------------------------------------------------

SRM_TV_MODE_T SRM_GetTvMode(void)
{
    if (_eApplicationMode == SRM_TV_MODE_TYPE_LAST_VALID_ENTRY)
    {
        if ((_arSrmVdpInfo[VDP_1].u4Enable != 0) &&
            (_arSrmVdpInfo[VDP_1].u4OutputWidth != 0) &&
            (_arSrmVdpInfo[VDP_1].u4OutputHeight != 0) &&
            (_arSrmVdpInfo[VDP_2].u4Enable != 0) &&
            (_arSrmVdpInfo[VDP_2].u4OutputWidth != 0) &&
            (_arSrmVdpInfo[VDP_2].u4OutputHeight != 0))
        {
            if ((_arSrmVdpInfo[VDP_1].u4OutputWidth + _arSrmVdpInfo[VDP_2].u4OutputWidth) > 1000)
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

//-----------------------------------------------------------------------------
/** Brief of SRM_SetTvMode.
 */
//-----------------------------------------------------------------------------

void SRM_SetTvMode(SRM_TV_MODE_T eMode)
{
    UINT32 u4Trigger;

    u4Trigger = 0;
    if (_eApplicationMode != eMode)
    {
        u4Trigger = 1;
    }

    _eApplicationMode = eMode;

    // trigger SRM to handle
    if (u4Trigger != 0)
    {
        SRM_SendEvent(SRM_DRV_NS, 0, 0, 0);
    }

    LOG(5, "[SRM] Event (TvMod) %d\n",
        eMode);
}

//-----------------------------------------------------------------------------
/** Brief of SRM_GetFreezeMode.
 */
//-----------------------------------------------------------------------------
SRM_FREEZE_MODE_T SRM_GetFreezeMode(UINT32 u4VdpId)
{
#ifdef CC_SCPOS_EN
    if ((u4VdpId == VDP_1) && (_arSrmVdpInfo[VDP_1].u4Source == (UINT32) VSS_DTV))
    {
        if (_arSrmVdpCtrl[VDP_1].rPrm[SRM_DISP_CTRL].u4Arg1 == SRM_SCPOS_MODE_OFF)
        {
            return SRM_FREEZE_MODE_MPEG;
        }
    }
#endif

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

//-----------------------------------------------------------------------------
/** Brief of SRM_QueryStatus.
 */
//-----------------------------------------------------------------------------
void SRM_QueryStatus(void)
{
    UINT32 u4VdpId;

    for (u4VdpId = 0; u4VdpId < VDP_NS; u4VdpId++)
    {
        LOG(3, "SRM(%d) E(%d) S(%d) 444(%d) VGA(%d) RES(%d, %d) I(%d) FR(%d)\n",
            u4VdpId,
            _arSrmVdpInfo[u4VdpId].u4Enable,
            _arSrmVdpInfo[u4VdpId].u4Status,
            _arSrmVdpInfo[u4VdpId].u4ColorFormat444,
            _arSrmVdpInfo[u4VdpId].u4VgaTiming,
            _arSrmVdpInfo[u4VdpId].u4Resolution,
            _arSrmVdpInfo[u4VdpId].u4MpegResolution,
            _arSrmVdpInfo[u4VdpId].u4Interlace,
            _arSrmVdpInfo[u4VdpId].u4InputFrameRate);

        LOG(3, "SRM(%d) CTRL BOB(%d) NR(%d, %d) DISP(%d) MUTE(%d) WE(%d) MPEG(%d)\n",
            u4VdpId,
            _arSrmVdpCtrl[u4VdpId].rPrm[SRM_BOB_CTRL].u4Arg1,
            _arSrmVdpCtrl[u4VdpId].rPrm[SRM_NR_CTRL].u4Arg1,
            _arSrmVdpCtrl[u4VdpId].rPrm[SRM_NR_CTRL].u4Arg2,
            _arSrmVdpCtrl[u4VdpId].rPrm[SRM_DISP_CTRL].u4Arg1,
            _arSrmVdpCtrl[u4VdpId].rPrm[SRM_MUTE_CTRL].u4Arg1,
            _arSrmVdpCtrl[u4VdpId].rPrm[SRM_SCALER_WE_CTRL].u4Arg1,
            _arSrmVdpCtrl[u4VdpId].rPrm[SRM_MPEG_PIP_CTRL].u4Arg1);
    }

    //if (IS_MT5391())
    {
        LOG(3, "MT5391\n");
    }


    //if (DRVCUST_OptGet(eFbmMemSize) == FBM_MEM_CFG_MT5391x128MB)
    {
        LOG(3, "FBM_MEM_CFG_MT5391x128MB Model\n");
    }
}

