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
 * $Date  $
 * $RCSfile: drv_lbox.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#define _DRV_LBOX_C_

#include "general.h"
#include "drv_video.h"
#include "video_def.h"
#include "drv_contrast.h"
#include "drv_common.h"
#include "vdp_drvif.h"
#include "x_timer.h"
#include "nptv_debug.h"
#include "drvcust_if.h"
#include "hw_ycproc.h"
#include "x_assert.h"
#include "drv_lbox.h"
#include "vdp_display.h"

// Note
// (8 Tap) (LetterBox >> Full) 36 -Tap, 10/s = 3.6s
// (8 Tap) (Full >> LetterBox) 29 -Tap, 10/s = 2.8s
// (4 Tap) (LetterBox >> Full) 17 -Tap, 10/s = 1.7s
// (4 Tap) (Full >> LetterBox) 15 -Tap, 10/s = 1.5s

#define LETTER_BOX_DEBUG
#ifndef LETTER_BOX_DEBUG
#undef LOG
#define LOG(lvl, fmt...)
#endif

#define LBOX_IGNORE (0xffffffff)

#define LETTER_BOX_SMALL_BOUND              7      ///< 15%
#define LETTER_BOX_SMALL_BOUND_MIN          8       ///< 8 time detected
#define LETTER_BOX_IIR_VALID_DRIFT_1        2       ///< 2%: for _u4LboxInvalidDraft1
#define LETTER_BOX_IIR_VALID_DRIFT_2        4       ///< 4%: for _u4LboxInvalidDraft2
#define LETTER_BOX_IIR_TAP_FRACTION         3       ///< IIR tap order
#define LETTER_BOX_IIR_TAP                  (1 << LETTER_BOX_IIR_TAP_FRACTION)
#define LETTER_BOX_UP_LOW_BOUND             4       ///< Avoid vsync interference, 4%
#define LETTER_BOX_LEFT_RIGHT_BOUND             15  ///< 15%
#define LETTER_BOX_MAX_BOUND                25      ///< 25%, Note 16:9 >> 4:3, About 12.5% (We set max = 25%, therefore, we can handle 32:9 inside 4:3)
#define LETTER_BOX_PROTECT_BOUND            2       ///< Region = (Up - 2, Low + 2)
#define LETTER_BOX_FAST_RECOVERY_BOUND      10      ///< Fast Recovery From LetterBox to Full
#define LETTER_BOX_FAST_RECOVERY_TAP        4       ///< Fast Recovery From LetterBox to Full
#define LETTER_BOX_BOUND_TIME               2       ///< 2 seconds
//#define LETTER_BOX_MODE_CHANGE_TIME               30

UINT32 _u4StableTime = 15; //10; //2;
UINT32 _u4VeryStableTime = 25; //20; //4;
UINT32 _u4CaptionStableTime = 30; //6;
//UINT32 u4DetectBoundRatio = 5; // 5%

#define LETTER_BOX_FCNT_BOUND               0x10            ///< threahold for HW to report state changed when field counter reaches the bound
//#define LETTER_BOX_Y_THRESHOLD              0x28
#define LETTER_BOX_LOOP_DELAY               100                  ///< SW LBD peroid (ms)   
#define LETTER_BOX_HARDWARE_DELAY           90              ///< LBD HW detection peroid limitation (ms)
#define LETTER_BOX_APATIVED_THD_REFLASH         20      ///< counter to update adaptive luma threshold
#define LETTER_BOX_APATIVED_THD_DARK            16       ///< The threshold count that hist reports the picture is too dark
#define LETTER_BOX_APATIVED_THD_NS              4           ///< number of adaptive luma threshold
#define LETTER_BOX_APATIVED_THD_DEFAULT         0       // 32
#define LETTER_BOX_APATIVED_THD_TOO_DARK        2       // Luma range: 0~31
#define LETTER_BOX_CHROMA_MAX_DELTA             20          ///< delta value for chroma checking
//#if SUPPORT_HORIZONTAL_LBOX
#define LETTER_BOX_MAX_MIN_DIFF_BOUND              10   ///< 10%
#define LETTER_BOX_SUM_DIFF_BOUND              5
#define TONE_DETECT_RANGE 10
#define TONE_DETECT_BOUND 80
//#endif
UINT32 _u4LBoxDetectionInit = 0;                        ///< flag to indicate letter box detetion module is initiated or not
UINT32 _u4LBoxDetectionEnable = 0;                      ///< flag to indicate letter box detection is enabled or not
UINT32 _u4LBoxDetectionValid = 0;                       ///< flag to indicate letter box detection results are valid or not
UINT32 _u4LBoxDetectionFixed = 0;                   ///< Pause SW LBD. Keep all sw parameters fixed.
UINT8 _u1LBoxCutEnable =1;                              ///<  default : auto cut of letter box
UINT32 _u4LBoxSourceChange = 0;
//UINT32 _u4LBoxPictureFormatChange = 0;
static UINT32 _u4LBoxSourceChangeFixed = 0;
static HANDLE_T _hLBoxSemaphore;

UINT32 _u4LBoxSrcWidth = 0;                             ///< input source width
UINT32 _u4LBoxSrcHeight = 0;                            ///< input souce height
UINT8 _u1LBoxSrcInterlace = 0;                          ///< input source is interlace or progressive
VDP_PIC_OVERSCAN_REGION_T _rLBoxOverScan;       ///< original overscan setting

// IIR
static UINT32 _u4LBoxIIRLow = 0;
static UINT32 _u4LBoxIIRUp = 0;
static UINT32 _u4LBoxIIRCount = 0;

// Fast Recovery
static UINT32 _u4LBoxFastRecovery = 0;

// Two Bound for Subtitle Detection
static UINT32 _u4LBoxBound1Low = 0;                 ///< cadidate 1 of downside LBD
static UINT32 _u4LBoxBound1Up = 0;                  ///< cadidate 1 of upside LBD
static UINT32 _u4LBoxBound1UpCounter = 0;       ///< counter for cadidate 1 of upside LBD
static UINT32 _u4LBoxBound1LowCounter = 0;      ///< counter for cadidate 1 of downside LBD
static UINT32 _u4LBoxBound2Low = 0;                 ///< cadidate 2 of downside LBD
static UINT32 _u4LBoxBound2Up = 0;                  ///< cadidate 2 of upside LBD
static UINT32 _u4LBoxBound2UpCounter = 0;       ///< counter for cadidate 2 of upside LBD
static UINT32 _u4LBoxBound2LowCounter = 0;      ///< counter for cadidate 2 of downside LBD

// Bound & Final & Current
static UINT32 _u4LboxInvalidDraft1 = 0;     ///< the difference threshold between current letter box and new deteced letter box to report scalar to update LBD
static UINT32 _u4LboxInvalidDraft2 = 0;     ///< the difference threshold to update letter box candidates
static UINT32 _u4LBoxUpBound = 0;           ///< upper boundary of SW upside LBD
static UINT32 _u4LBoxLowBound = 0;          ///< lower boundary of SW downside LBD
static UINT32 _u4LBoxUpMax = 0;                 ///< lower boundary of SW upside LBD
static UINT32 _u4LBoxLowMax = 0;                ///< upper boundary of SW downside LBD
static UINT32 _u4LBoxUpFinal = 0;               ///< final result of upper bound detection
static UINT32 _u4LBoxLowFinal = 0;              ///< final result of lower bound detection
static UINT32 _u4LBoxUpCurrent = 0;             ///< current reported upside letter box
static UINT32 _u4LBoxLowCurrent = 0;            ///< current reported downside letter box

// External Stable Time
static UINT32 _u4LBoxUpFinalBackup[3];          ///< history final result of upper bound detection
static UINT32 _u4LBoxLowFinalBackup[3];         ///< history final result of lower bound detection
static UINT32 _u4LBoxUpStableBackup[2];
static UINT32 _u4LBoxLowStableBackup[2];

// Adaptive Threshold
static UINT32 _u4LBoxLastTooDark = 0;               ///< Dark Screen, Should NOT enable letter box detection
static UINT32 _u4LBoxTooDark = 0;                   ///< Dark Screen, Should NOT enable letter box detection
static UINT32 _u4LBoxThresholdCount = LETTER_BOX_APATIVED_THD_REFLASH;
static UINT32 _u4LBoxThresholdTooDark = 0;          ///< 95%
static UINT32 _u4LBoxThresholdDark = 0;             ///< 50%
static UINT32 _u4LBoxThresholdFit = 0;              ///< 25%
static UINT32 _u4LBoxThresholdEdge = 0;             ///< 12.5%

static UINT32 _u4LBoxThresholdDarkCount = 0;        ///< the changce that the picture is too dark
static UINT32 _au4LboxThresholdCount[LETTER_BOX_APATIVED_THD_NS] = { 0, 0, 0, 0 };          ///< Score of 4 adaptive threshold of Luma   
const UINT32 _au4LboxThresholdValue[LETTER_BOX_APATIVED_THD_NS] = { 32, 48, 64, 80 };     ///< 4 adaptive threshold of Luma   

// Stable Time
static UINT32 _u4LBoxStableTime;        ///< SW LBD Peroid (sec)
static HAL_TIME_T _rStableTime;
static HAL_TIME_T _rBoundTime;
//static HAL_TIME_T _rStartTime;

//#if SUPPORT_LUMACHROMA_CHECK
//static UINT32 _u4BestLumaThreshold = 32;
//#endif

#if SUPPORT_HORIZONTAL_LBOX
static UINT32 _u4LBoxLeftBound = 0;
static UINT32 _u4LBoxRightBound = 0;
static UINT32 _u4LBoxLeftCurrent = 0;
static UINT32 _u4LBoxRightCurrent = 0;
static UINT32 _u4LBoxBound1Left = 0;
static UINT32 _u4LBoxBound1Right = 0;
static UINT32 _u4LBoxBound1LeftCounter = 0;
static UINT32 _u4LBoxBound1RightCounter = 0;
static UINT32 _u4LBoxBound2Left = 0;
static UINT32 _u4LBoxBound2Right = 0;
static UINT32 _u4LBoxBound2LeftCounter = 0;
static UINT32 _u4LBoxBound2RightCounter = 0;
static UINT32 _u4LBoxLeftFinal = 0;
static UINT32 _u4LBoxRightFinal = 0;
static UINT32 _u4LBoxIIRRight;
static UINT32 _u4LBoxIIRLeft;
static UINT32 _u4LBoxSumDiffThreshold = 0;
static UINT32 _u4LboxInvalidDraft3 = 0;
static UINT32 _u4LboxInvalidDraft4 = 0;
#endif

//-----------------------------------------------------------------------------
// I/O Interface
//-----------------------------------------------------------------------------

static void LboxSetPosSize()
{            
    if(_u1LBoxCutEnable)
    {
        if(bVideoUpdateSrcRegion(SV_VP_MAIN) != SV_SUCCESS)
        {
        }
    }
}

static void vLBoxGetInputSourceResolution(UINT32 *pu4Width, UINT32 *pu4Height)
{
    if (pu4Width)
    {
        *pu4Width = (UINT32)wDrvVideoInputWidth(SV_VP_MAIN);
    }
    if (pu4Height)
    {
        *pu4Height = (UINT32)wDrvVideoInputHeight(SV_VP_MAIN);
    }
}

static void vLBoxGetInputResolution(UINT32 *pu4Width, UINT32 *pu4Height)
{
    if (pu4Width)
    {
        *pu4Width = (UINT32)wDrvVideoGetPreScaleWidth(SV_VP_MAIN);
    }
    if (pu4Height)
    {
        UINT16 u2Height;
        u2Height = wDrvVideoInputHeight(SV_VP_MAIN);
        u2Height = bDrvVideoIsSrcInterlace(SV_VP_MAIN) ? (u2Height>>1): u2Height;
        *pu4Height = (UINT32)u2Height;
    }
}

static void vLBoxGetInputOverscanSetting(VDP_PIC_OVERSCAN_REGION_T *pOverScan)
{
    if (pOverScan)
    {
        *pOverScan = _rMPicInfo.rOverScan;
    }
}

static BOOL bLBOXIsInputInterlace(void)
{
    return bDrvVideoIsSrcInterlace(SV_VP_MAIN);
}

static BOOL bLBOXIsInput444(void)
{
    return bIsScalerInput444(SV_VP_MAIN);
}

static BOOL bLBOXIsInputValid(void)
{
    if ((_rMChannel.bIsChannelOn != 0) && (bDrvVideoSignalStatus(SV_VP_MAIN) == (UINT8) SV_VDO_STABLE))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static BOOL bLBOXIsInputDispmode(void)
{
    UINT8 u1Dispmode;

    u1Dispmode = _VDP_vGetDispModeOnOff(SV_VP_MAIN);
    if ((u1Dispmode != VDP_SCPOS_DISPMODE_AUTO) && (u1Dispmode != VDP_SCPOS_DISPMODE_MANUAL))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

static void vLBoxGetLumaHistogram(UINT16 *au2Hist)
{
    UNUSED(bDrvGetHist(au2Hist));
}

#if SUPPORT_LUMACHROMA_CHECK
static void vLBoxSetToneDetectionRange(UINT32 u4Left, UINT32 u4LeftSeperate, UINT32 u4Right, UINT32 u4RightSeperate)
{
    if (u4Left != LBOX_IGNORE)
    {
        vRegWriteFldAlign(SIDE_LBOX_01, u4Left, C_LEFT_BOUND);
    }
    if (u4Right != LBOX_IGNORE)
    {
        vRegWriteFldAlign(SIDE_LBOX_01, u4Right, C_RIGHT_BOUND);
    }
    if (u4LeftSeperate != LBOX_IGNORE)
    {
        vRegWriteFldAlign(SIDE_LBOX_05, u4LeftSeperate, C_LEFT_TONE_SEPERATE);
    }
    if (u4RightSeperate != LBOX_IGNORE)
    {
        vRegWriteFldAlign(SIDE_LBOX_05, u4RightSeperate, C_RIGHT_TONE_SEPERATE);
    }

}
#endif


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static UINT32 LboxBoundStable(UINT32 u4Input, UINT32 u4Bound, UINT32 u4Drift);
static void LboxDetectionUnlock(void);

static void LboxDetectionUnlock(void)
{
	_u4LBoxDetectionEnable = 1;	
	VERIFY (x_sema_unlock(_hLBoxSemaphore) == OSR_OK);
}

//-----------------------------------------------------------------------------
/** Brief of LboxResetCaptionVideo.
 */
//-----------------------------------------------------------------------------
static void LboxResetCaptionVideo(void)
{
    _u4LBoxStableTime = _u4StableTime;

    _u4LBoxLowFinalBackup[2] = _u4LBoxLowBound;
    _u4LBoxLowFinalBackup[1] = _u4LBoxLowBound;
    _u4LBoxLowFinalBackup[0] = _u4LBoxLowBound;

    _u4LBoxUpFinalBackup[2] = _u4LBoxUpBound;
    _u4LBoxUpFinalBackup[1] = _u4LBoxUpBound;
    _u4LBoxUpFinalBackup[0] = _u4LBoxUpBound;
}

//-----------------------------------------------------------------------------
/** Brief of LboxCheckCaptionVideo.
 */
//-----------------------------------------------------------------------------
static void LboxCheckCaptionVideo(void)
{
    _u4LBoxStableTime = _u4StableTime;

    if ((_u4LBoxLowFinal > _u4LBoxLowFinalBackup[0]) &&
        (_u4LBoxLowFinal > (_u4LBoxLowFinalBackup[0] + _u4LboxInvalidDraft1)))
    {
        if (LboxBoundStable(_u4LBoxLowFinal, _u4LBoxLowFinalBackup[1], _u4LboxInvalidDraft1) != 0)
        {
            if (LboxBoundStable(_u4LBoxLowFinalBackup[0], _u4LBoxLowFinalBackup[2], _u4LboxInvalidDraft1) != 0)
            {
                LOG(4, "Caption Detect Low(%d %d %d %d)\n", 
                    _u4LBoxLowFinal, 
                    _u4LBoxLowFinalBackup[0], 
                    _u4LBoxLowFinalBackup[1],
                    _u4LBoxLowFinalBackup[2]);

                _u4LBoxStableTime = _u4CaptionStableTime;
            }
        }
    }

    if ((_u4LBoxUpFinal < _u4LBoxUpFinalBackup[0]) &&
        ((_u4LBoxUpFinal + _u4LboxInvalidDraft1) < _u4LBoxUpFinalBackup[0] ))
    {
        if (LboxBoundStable(_u4LBoxUpFinal, _u4LBoxUpFinalBackup[1], _u4LboxInvalidDraft1) != 0)
        {
            if (LboxBoundStable(_u4LBoxUpFinalBackup[0], _u4LBoxUpFinalBackup[2], _u4LboxInvalidDraft1) != 0)
            {
                LOG(4, "Caption Detect Up(%d %d %d %d)\n", 
                    _u4LBoxUpFinal, 
                    _u4LBoxUpFinalBackup[0], 
                    _u4LBoxUpFinalBackup[1],
                    _u4LBoxUpFinalBackup[2]);

                _u4LBoxStableTime = _u4CaptionStableTime;
            }
        }
    }

    _u4LBoxLowFinalBackup[2] = _u4LBoxLowFinalBackup[1];
    _u4LBoxLowFinalBackup[1] = _u4LBoxLowFinalBackup[0];
    _u4LBoxLowFinalBackup[0] = _u4LBoxLowFinal;

    _u4LBoxUpFinalBackup[2] = _u4LBoxUpFinalBackup[1];
    _u4LBoxUpFinalBackup[1] = _u4LBoxUpFinalBackup[0];
    _u4LBoxUpFinalBackup[0] = _u4LBoxUpFinal;
}

//-----------------------------------------------------------------------------
/** Brief of LboxResetCaptionVideo.
 */
//-----------------------------------------------------------------------------
static void LboxResetStableVideo(void)
{
    _u4LBoxStableTime = _u4StableTime;

    _u4LBoxLowStableBackup[1] = _u4LBoxLowBound;
    _u4LBoxLowStableBackup[0] = _u4LBoxLowBound;

    _u4LBoxUpStableBackup[1] = _u4LBoxUpBound;
    _u4LBoxUpStableBackup[0] = _u4LBoxUpBound;
}

//-----------------------------------------------------------------------------
/** Brief of LboxCheckCaptionVideo.
 */
//-----------------------------------------------------------------------------
static void LboxCheckStableVideo(void)
{
    if (_u4LBoxStableTime == _u4VeryStableTime)
    {
        _u4LBoxStableTime = _u4StableTime;
    }

    if ((_u4LBoxLowFinal != _u4LBoxLowBound) &&
        (_u4LBoxLowStableBackup[0] != _u4LBoxLowBound) &&
        (_u4LBoxLowStableBackup[1] != _u4LBoxLowBound) &&
        (_u4LBoxUpFinal != _u4LBoxUpBound) &&
        (_u4LBoxUpStableBackup[0] != _u4LBoxUpBound) &&
        (_u4LBoxUpStableBackup[1] != _u4LBoxUpBound))
    {
        if ((LboxBoundStable(_u4LBoxLowFinal, _u4LBoxLowStableBackup[0], _u4LboxInvalidDraft1) != 0) &&
            (LboxBoundStable(_u4LBoxLowFinal, _u4LBoxLowStableBackup[1], _u4LboxInvalidDraft1) != 0) &&
            (LboxBoundStable(_u4LBoxUpFinal, _u4LBoxUpStableBackup[0], _u4LboxInvalidDraft1) != 0) &&
            (LboxBoundStable(_u4LBoxUpFinal, _u4LBoxUpStableBackup[1], _u4LboxInvalidDraft1) != 0))
        {
            LOG(4, "Stable Detect(%d %d %d %d)\n", 
                _u4LBoxLowFinal, 
                _u4LBoxLowStableBackup[0], 
                _u4LBoxUpFinal,
                _u4LBoxUpStableBackup[0]);

            if (_u4LBoxStableTime == _u4StableTime)
            {
                _u4LBoxStableTime = _u4VeryStableTime;
            }
        }
    }

    _u4LBoxLowStableBackup[1] = _u4LBoxLowStableBackup[0];
    _u4LBoxLowStableBackup[0] = _u4LBoxLowFinal;

    _u4LBoxUpStableBackup[1] = _u4LBoxUpStableBackup[0];
    _u4LBoxUpStableBackup[0] = _u4LBoxUpFinal;
}


#if SUPPORT_LUMACHROMA_CHECK
//-----------------------------------------------------------------------------
/** Brief of LboxCheckHorizontalChroma.
 */
//-----------------------------------------------------------------------------
static void LboxCheckHorizontalChroma(void)
{
    #if SUPPORT_HORIZONTAL_LBOX
    UINT32 u4UpBound, u4LowBound;
    UINT32 u4DiffLine, u4ToneThresh;
    UINT32 u4ULeft, u4URight, u4VLeft, u4VRight;
    
    // Set Tone detect bound based on Y detected bound.
    vLBoxSetToneDetectionRange(_u4LBoxLeftFinal - TONE_DETECT_RANGE, _u4LBoxLeftFinal, _u4LBoxRightFinal + TONE_DETECT_RANGE, _u4LBoxRightFinal);

    // wait
    x_thread_delay(LETTER_BOX_HARDWARE_DELAY);

    // Get Up and Low bound
    u4UpBound = RegReadFldAlign(SIDE_LBOX_00, C_UP_BOUND);
    u4LowBound = RegReadFldAlign(SIDE_LBOX_00, C_LOW_BOUND);
    u4DiffLine = u4LowBound - u4UpBound;
    // u4ToneThresh: number of pixels fall in the range of tone * 80%
    u4ToneThresh = ((u4DiffLine * TONE_DETECT_RANGE) >> 4) * TONE_DETECT_BOUND / 100;
    u4ULeft = RegReadFldAlign(SIDE_LBOX_STA_03, STA_U_IN_TONE1_LEFT);
    u4VLeft = RegReadFldAlign(SIDE_LBOX_STA_03, STA_V_IN_TONE1_LEFT);
    u4URight = RegReadFldAlign(SIDE_LBOX_STA_04, STA_U_IN_TONE1_RIGHT);
    u4VRight = RegReadFldAlign(SIDE_LBOX_STA_04, STA_V_IN_TONE1_RIGHT);

    if ((u4ULeft <= u4ToneThresh) && (u4VLeft <= u4ToneThresh))
    {
        // colorful, do not clip
        _u4LBoxLeftFinal = _u4LBoxLeftCurrent;
    }
    
    if ((u4URight <= u4ToneThresh) && (u4VRight <= u4ToneThresh))
    {
        // colorful, do not clip
        _u4LBoxRightFinal = _u4LBoxRightCurrent;
    }
    #endif
}


//-----------------------------------------------------------------------------
/** Brief of LboxCheckVerticalChroma.
 */
//-----------------------------------------------------------------------------
static void LboxCheckVerticalChroma(void)
{
    UINT32 u4DiffWidth, u4ToneThresh;
    UINT32 u4ULeft, u4VLeft;
    UINT32 u4Range, u4Upbound, u4Lowbound;
    
    // Set Tone detect bound based on Y detected bound.
    #if SUPPORT_HORIZONTAL_LBOX
    vLBoxSetToneDetectionRange(_u4LBoxLeftFinal, _u4LBoxRightFinal, _u4LBoxRightFinal, _u4LBoxLeftFinal);
    u4DiffWidth = _u4LBoxRightFinal - _u4LBoxLeftFinal;
    #else
    u4ULeft = (_u4LBoxSrcWidth * _rLBoxOverScan.u4Left) / VDP_MAX_REGION_WIDTH;              // left bound
    u4VLeft = _u4LBoxSrcWidth - ((_u4LBoxSrcWidth * _rLBoxOverScan.u4Right) / VDP_MAX_REGION_WIDTH);     // right bound       
    vLBoxSetToneDetectionRange(u4ULeft, u4VLeft, u4VLeft, u4ULeft);
    u4DiffWidth = u4VLeft - u4ULeft;
    #endif

    /* Upper part */
    if (_u4LBoxUpFinal > TONE_DETECT_RANGE)
    {
        if ((_u4LBoxUpFinal - TONE_DETECT_RANGE) > _u4LBoxUpBound)
        {
            u4Upbound = _u4LBoxUpFinal - TONE_DETECT_RANGE;
        }
        else
        {
            u4Upbound = _u4LBoxUpBound;
        }
    }
    else
    {
        u4Upbound = 0;
    }
    if (_u4LBoxUpFinal < u4Upbound)
    {
        LOG(3, "Error: _u4LBoxUpFinal < u4Upbound\n");
    }
    u4Range = _u4LBoxUpFinal -u4Upbound;
    vRegWriteFldAlign(SIDE_LBOX_00, u4Upbound, C_UP_BOUND);
    vRegWriteFldAlign(SIDE_LBOX_00, _u4LBoxUpFinal, C_LOW_BOUND);
 
    // u4ToneThresh: number of pixels fall in the range of tone * 80%
    u4ToneThresh = ((u4DiffWidth * u4Range) >> 4) * TONE_DETECT_BOUND / 100;
    x_thread_delay(LETTER_BOX_HARDWARE_DELAY);
    u4ULeft = RegReadFldAlign(SIDE_LBOX_STA_03, STA_U_IN_TONE1_LEFT);
    u4VLeft = RegReadFldAlign(SIDE_LBOX_STA_03, STA_V_IN_TONE1_LEFT);

    if ((u4ULeft <= u4ToneThresh) && (u4VLeft <= u4ToneThresh))
    {
        _u4LBoxUpFinal = _u4LBoxUpCurrent;  // colorful, do not clip
    }
    
    /*********************************************************************/
    /* Lower part */
    if ((_u4LBoxLowFinal + TONE_DETECT_RANGE) > _u4LBoxLowFinal)
    {
        u4Lowbound = _u4LBoxLowFinal;
    }
    else
    {
        u4Lowbound = _u4LBoxLowFinal + TONE_DETECT_RANGE;
    }
    vRegWriteFldAlign(SIDE_LBOX_00, _u4LBoxLowFinal, C_UP_BOUND);
    vRegWriteFldAlign(SIDE_LBOX_00, u4Lowbound, C_LOW_BOUND);

    if (u4Lowbound < _u4LBoxLowFinal)
    {
        LOG(3, "Error: u4Lowbound < _u4LBoxLowFinal\n");
    }
    u4Range = u4Lowbound - _u4LBoxLowFinal;
    // u4ToneThresh: number of pixels fall in the range of tone * 80%
    u4ToneThresh = ((u4DiffWidth * u4Range) >> 4) * TONE_DETECT_BOUND / 100;
    // wait
    x_thread_delay(LETTER_BOX_HARDWARE_DELAY);
    u4ULeft = RegReadFldAlign(SIDE_LBOX_STA_03, STA_U_IN_TONE1_LEFT);
    u4VLeft = RegReadFldAlign(SIDE_LBOX_STA_03, STA_V_IN_TONE1_LEFT);

    if ((u4ULeft <= u4ToneThresh) && (u4VLeft <= u4ToneThresh))
    {
        _u4LBoxLowFinal = _u4LBoxLowCurrent;  // colorful, do not clip
    }
}


//-----------------------------------------------------------------------------
/** Brief of LboxCheckLumaChroma.
 */
//-----------------------------------------------------------------------------
static void LboxCheckLumaChroma(void)
{
    if (bLBOXIsInput444() == 0)    
    {
        UINT32 u4LeftBound, u4RightBound;
        
        // Reconfigure Tone detect bound
        // Save original left and right bound for Y detection.
        u4LeftBound = RegReadFldAlign(SIDE_LBOX_01, C_LEFT_BOUND);
        u4RightBound = RegReadFldAlign(SIDE_LBOX_01, C_RIGHT_BOUND);
        
        // Set Tone detect range        
        vRegWriteFldAlign(SIDE_LBOX_02, 128, C_TONE1_U);
        vRegWriteFldAlign(SIDE_LBOX_02, LETTER_BOX_CHROMA_MAX_DELTA, C_TONE1_U_RANGE);
        vRegWriteFldAlign(SIDE_LBOX_02, 128, C_TONE1_V);
        vRegWriteFldAlign(SIDE_LBOX_02, LETTER_BOX_CHROMA_MAX_DELTA, C_TONE1_V_RANGE);

        #if SUPPORT_LUMACHROMA_CHECK
            LboxCheckHorizontalChroma();
            LboxCheckVerticalChroma();
        #endif
     
        // Recover left and right bound for Y detection
        vRegWriteFldAlign(SIDE_LBOX_01, u4LeftBound, C_LEFT_BOUND);
        vRegWriteFldAlign(SIDE_LBOX_01, u4RightBound, C_RIGHT_BOUND);
        
    }
}
#endif


//-----------------------------------------------------------------------------
/** Brief of LboxBoundStable.
 */
//-----------------------------------------------------------------------------
static UINT32 LboxBoundStable(UINT32 u4Input, UINT32 u4Bound, UINT32 u4Drift)
{
    if (u4Input == u4Bound)
    {
        return 1;
    }

    if (u4Input > u4Bound)
    {
        if (u4Input > (u4Bound + u4Drift))
        {
            return 0;
        }
    }
    else
    {
        if (u4Bound > (u4Input + u4Drift))
        {
            return 0;
        }
    }

    return 1;
}

//-----------------------------------------------------------------------------
/** Brief of LboxAdaptiveThreshold.
 */
//-----------------------------------------------------------------------------
static void LboxAdaptiveThreshold(void)
{
    UINT16 au2Hist[LUMA_HIST_LEVEL];
    UINT32 u4Sum;
    UINT32 u4Idx;
    UINT32 u4Best;

    //UNUSED(bDrvGetHist(au2Hist));
    vLBoxGetLumaHistogram(au2Hist);
    
    // update luma threshold
    if (++_u4LBoxThresholdCount >= LETTER_BOX_APATIVED_THD_REFLASH)
    {        
        _u4LBoxThresholdCount = 0;

        // Update Count
        u4Sum = 0;
        for (u4Idx = 0; u4Idx < LUMA_HIST_LEVEL; u4Idx++)
        {
            u4Sum += au2Hist[u4Idx];
        }

        _u4LBoxThresholdTooDark = ((u4Sum * 19) / 20);                              // 95%
        _u4LBoxThresholdDark = (u4Sum * 5) / 8;                                       // 50%
        _u4LBoxThresholdFit = (u4Sum >> 2);                                         // 25%
        _u4LBoxThresholdEdge = (u4Sum >> 3);                                        // 12.5%

        u4Best = 0;
        // Pick the Best Threshold
        for (u4Idx = 1; u4Idx < LETTER_BOX_APATIVED_THD_NS; u4Idx++)
        {
            if (_au4LboxThresholdCount[u4Idx] > _au4LboxThresholdCount[u4Best])
            {
                u4Best = u4Idx;
            }
        }        
        if (_au4LboxThresholdCount[u4Best] == 0)
        {
            u4Best = LETTER_BOX_APATIVED_THD_DEFAULT;
        }

        vRegWriteFldAlign(LBOX_03, LETTER_BOX_FCNT_BOUND, FCNT_BOUND);
        vRegWriteFldAlign(LBOX_01, _au4LboxThresholdValue[u4Best], Y_THRES);
        #if SUPPORT_HORIZONTAL_LBOX
        vRegWriteFldAlign(SIDE_LBOX_01, _au4LboxThresholdValue[u4Best], C_Y_THRES);
        #endif

        LOG(5, "Threshold (%d) (%d %d %d %d) (%d) (%d %d)\n",
            _u4LBoxThresholdDarkCount,
            _au4LboxThresholdCount[0],
            _au4LboxThresholdCount[1],
            _au4LboxThresholdCount[2],
            _au4LboxThresholdCount[3],
            u4Best, u4Sum,  _u4LBoxThresholdFit);

        // Too Dark ?
        _u4LBoxTooDark = 0;
        if (_u4LBoxThresholdDarkCount >= LETTER_BOX_APATIVED_THD_DARK)
        {
            // Histogram reports the picture is too dark more than LETTER_BOX_APATIVED_THD_DARK/LETTER_BOX_APATIVED_THD_REFLASH.
            u4Sum = 0;
            
            for (u4Idx = 1; u4Idx < LETTER_BOX_APATIVED_THD_NS; u4Idx++)
            {
                u4Sum += _au4LboxThresholdCount[u4Idx];
            }

            // make sure it is very dark, only Threshold#1 (32) found
            if (u4Sum == 0)
            {
                _u4LBoxTooDark = 1;
                LOG(4, "Too dark!\n");
            }
        }

        // Clean Threshold Count
        for (u4Idx = 0; u4Idx < LETTER_BOX_APATIVED_THD_NS; u4Idx++)
        {
            _au4LboxThresholdCount[u4Idx] = 0;
        }
        _u4LBoxThresholdDarkCount = 0;
    }

    // update parameters of adaptive threshold
    u4Best = 0;
    u4Sum = 0;
    for (u4Idx = 0; u4Idx < LUMA_HIST_LEVEL; u4Idx++)
    {    
        u4Sum += au2Hist[u4Idx];

        if (u4Sum >= _u4LBoxThresholdTooDark)
        {
            if (u4Idx < LETTER_BOX_APATIVED_THD_TOO_DARK)
            {
                // TooDark Case1: 0~31 > 95%
                ++_u4LBoxThresholdDarkCount;
            }
                
            u4Idx = LUMA_HIST_LEVEL;
        }
        else if ((u4Sum >= _u4LBoxThresholdFit) && (u4Best == 0))
        {
            // 25% detect
        
            // 1st Time Only
            u4Best = 1;
            
            if (u4Idx < LETTER_BOX_APATIVED_THD_NS)  // 0~63 > 25%
            {  
                UINT32 u4Target;
                u4Target = u4Idx;
        
                // 1) Can not distinquish 25% and 50%
                // 2) Force smallest threshold
                if (u4Sum >= _u4LBoxThresholdDark)
                {
                    if (u4Idx == 0)
                    {
                        // TooDark Case2:
                        // Can not distinquish 25% and 50% by force smallest threshold
                        ++_u4LBoxThresholdDarkCount;

                        u4Idx = LUMA_HIST_LEVEL;
                    }

                    if (u4Target != 0)
                    {
                        u4Target--;
                    }
                }

                ++_au4LboxThresholdCount[u4Target];
            }
        }
        else if (((u4Idx + 1) < LUMA_HIST_LEVEL) && ((au2Hist[u4Idx] > (au2Hist[u4Idx + 1] + _u4LBoxThresholdEdge)) && (u4Best == 0)))
        {
            // Edge detected
            // The difference between two luma level is over 12.5%

            // 1st Time Only
            u4Best = 1;

            if (u4Idx < LETTER_BOX_APATIVED_THD_NS) // Edge deteced in 0~63 
            {
                UINT32 u4Target;
                u4Target = u4Idx;

                // Remove??
                // 1) Can not distinquish 25% and 50%
                // 2) Force smallest threshold
                if (u4Sum >= _u4LBoxThresholdDark)
                {
                    if (u4Idx == 0)
                    {
                        // Can not distinquish 25% and 50% by force smallest threshold
                        ++_u4LBoxThresholdDarkCount;

                        u4Idx = LUMA_HIST_LEVEL;

                        LOG(4, "Dark#3 [%d %d %d %d]\n", au2Hist[0], au2Hist[1], au2Hist[2], au2Hist[3]); 
                    }

                    if (u4Target != 0)
                    {
                        u4Target--;
                    }
                }

                ++_au4LboxThresholdCount[u4Target];
            }
        }
    }    
}

//-----------------------------------------------------------------------------
/** Brief of LboxDetectionConfig.
 */
//-----------------------------------------------------------------------------
static void LboxDetectionConfig(void)
{
    UINT32 u4ReConfig;
    UINT32 u4Width, u4Height;
    VDP_PIC_OVERSCAN_REGION_T rOverScan;

    u4ReConfig = 0;

    // Check if Source Change or Picture Format Change
    if ((_u4LBoxSourceChange != 0) ||(_u4LBoxSourceChangeFixed != 0))
    {
        _u4LBoxSourceChange = 0;
        _u4LBoxSourceChangeFixed = 0;
        u4ReConfig = 1;
    }

    vLBoxGetInputResolution(&u4Width, &u4Height);
    vLBoxGetInputOverscanSetting(&rOverScan);
    
    // When change notify happen, bDrvVideoIsSrcInterlace return value NOT valid
    if ((_u1LBoxSrcInterlace != bLBOXIsInputInterlace()) || (_u4LBoxSrcHeight != u4Height))
    {
        LOG(3, "New LBOX source height: %d --> %d\n", _u4LBoxSrcHeight, u4Height);
        _u1LBoxSrcInterlace = bLBOXIsInputInterlace();
        _u4LBoxSrcHeight = u4Height;

        u4ReConfig = 1;
    }

    // input source width changed
    if (_u4LBoxSrcWidth != u4Width)
    {
        LOG(3, "New LBOX source width: %d --> %d\n", _u4LBoxSrcWidth, u4Width);
        _u4LBoxSrcWidth = u4Width;

        u4ReConfig = 1;
    }

    // Check if OverScan Change
    if ((rOverScan.u4Top != _rLBoxOverScan.u4Top) ||
        (rOverScan.u4Bottom != _rLBoxOverScan.u4Bottom) ||
        (rOverScan.u4Left != _rLBoxOverScan.u4Left) ||
        (rOverScan.u4Right != _rLBoxOverScan.u4Right))
    {
        _rLBoxOverScan = rOverScan;

        u4ReConfig = 1;
    }

    if (u4ReConfig != 0)
    {
        // Do Reconfig
        UINT32 u4RightBound;
        UINT32 u4LeftBound;

        // set stable threshold for LBD
        if (_u1LBoxSrcInterlace != 0)
        {
            _u4LboxInvalidDraft1 = ((_u4LBoxSrcHeight * LETTER_BOX_IIR_VALID_DRIFT_1) + 199) / 100;
            _u4LboxInvalidDraft2 = ((_u4LBoxSrcHeight * LETTER_BOX_IIR_VALID_DRIFT_2) + 199) / 100;
        }
        else
        {
            _u4LboxInvalidDraft1 = ((_u4LBoxSrcHeight * LETTER_BOX_IIR_VALID_DRIFT_1) + 99) / 100;
            _u4LboxInvalidDraft2 = ((_u4LBoxSrcHeight * LETTER_BOX_IIR_VALID_DRIFT_2) + 99) / 100;
        }
        
        #if SUPPORT_HORIZONTAL_LBOX
        _u4LboxInvalidDraft3 = ((_u4LBoxSrcWidth * LETTER_BOX_IIR_VALID_DRIFT_1) + 99) / 200;
        _u4LboxInvalidDraft4 = ((_u4LBoxSrcWidth * LETTER_BOX_IIR_VALID_DRIFT_2) + 99) / 200;
        #endif

        // Set detection range for upside LBD: _u4LBoxUpBound ~ _u4LBoxUpMax
        // Set detection range for downside LBD: _u4LBoxLowMax ~ _u4LBoxLowBound
        _u4LBoxUpBound = (_u4LBoxSrcHeight * LETTER_BOX_UP_LOW_BOUND) / 100;
        _u4LBoxLowBound = _u4LBoxSrcHeight - _u4LBoxUpBound;
        _u4LBoxUpMax = (_u4LBoxSrcHeight * LETTER_BOX_MAX_BOUND) / 100;
        _u4LBoxLowMax = (_u4LBoxSrcHeight * (100 -LETTER_BOX_MAX_BOUND)) / 100;

        u4LeftBound = (_u4LBoxSrcWidth * _rLBoxOverScan.u4Left) / VDP_MAX_REGION_WIDTH;        
        u4RightBound = _u4LBoxSrcWidth - ((_u4LBoxSrcWidth * _rLBoxOverScan.u4Right) / VDP_MAX_REGION_WIDTH);        

        _u4LBoxUpCurrent = _u4LBoxUpBound;
        _u4LBoxLowCurrent = _u4LBoxLowBound;

        // Reset Bound#1 and Bound#2
        _u4LBoxBound1Up = _u4LBoxUpBound;
        _u4LBoxBound1Low = _u4LBoxLowBound;
        _u4LBoxBound2Up = _u4LBoxUpBound;
        _u4LBoxBound2Low = _u4LBoxLowBound; 
        _u4LBoxBound1UpCounter = 0;
        _u4LBoxBound1LowCounter = 0;
        _u4LBoxBound2UpCounter = 0;
        _u4LBoxBound2LowCounter = 0;

        if (_u4LBoxDetectionValid == 0)
        {
            _u4LBoxUpFinal = _u4LBoxUpBound;
            _u4LBoxLowFinal = _u4LBoxLowBound;
        }

        LboxResetCaptionVideo();
        LboxResetStableVideo();

        // Update Config Register
        vRegWriteFldAlign(LBOX_00, _u4LBoxUpBound, LBOX_UP_BOUND);
        vRegWriteFldAlign(LBOX_00, _u4LBoxLowBound, LBOX_LOW_BOUND);
        vRegWriteFldAlign(LBOX_01, u4LeftBound, LBOX_LEFT_BOUND);
        vRegWriteFldAlign(LBOX_01, u4RightBound, LBOX_RIGHT_BOUND);
        vRegWrite4B(LBOX_02, 0);
        
        vRegWriteFldAlign(LBOX_00, 0, LBOX_START);
        vRegWriteFldAlign(LBOX_00, 0, LBOX_STATE);
        vRegWriteFldAlign(LBOX_03, 0, FCNT_BOUND);
        vRegWriteFldAlign(LBOX_01, 0, Y_THRES);

        #if SUPPORT_HORIZONTAL_LBOX
        UINT32 u4UpBound;
        UINT32 u4LowBound;
        
        //_u4LBoxLeftBound = (_u4LBoxSrcWidth * LETTER_BOX_LEFT_RIGHT_BOUND) / 100;
        _u4LBoxLeftBound = u4LeftBound;
        _u4LBoxRightBound = _u4LBoxSrcWidth - _u4LBoxLeftBound;
        _u4LBoxSumDiffThreshold = _u4LBoxSrcHeight * LETTER_BOX_SUM_DIFF_BOUND;
        
        u4UpBound = (_u4LBoxSrcHeight * _rLBoxOverScan.u4Top) / VDP_MAX_REGION_HEIGHT;
        u4LowBound = _u4LBoxSrcHeight - ((_u4LBoxSrcHeight * _rLBoxOverScan.u4Bottom) / VDP_MAX_REGION_HEIGHT);

         _u4LBoxLeftCurrent = _u4LBoxLeftBound;
        _u4LBoxRightCurrent = _u4LBoxRightBound;

        // Reset Bound#1 and Bound#2
        _u4LBoxBound1Left = _u4LBoxLeftBound;
        _u4LBoxBound1Right = _u4LBoxRightBound;
        _u4LBoxBound2Left = _u4LBoxLeftBound;
        _u4LBoxBound2Right = _u4LBoxRightBound; 
        _u4LBoxBound1LeftCounter = 0;
        _u4LBoxBound1RightCounter = 0;
        _u4LBoxBound2LeftCounter = 0;
        _u4LBoxBound2RightCounter = 0;

        if (_u4LBoxDetectionValid == 0)
        {
            _u4LBoxLeftFinal = _u4LBoxLeftBound;
            _u4LBoxRightFinal = _u4LBoxRightBound;
        }
        
        // Update Config Register
        vRegWriteFldAlign(SIDE_LBOX_00, u4UpBound, C_UP_BOUND);
        vRegWriteFldAlign(SIDE_LBOX_00, u4LowBound, C_LOW_BOUND);
        vRegWriteFldAlign(SIDE_LBOX_01, _u4LBoxLeftBound, C_LEFT_BOUND);
        vRegWriteFldAlign(SIDE_LBOX_01, _u4LBoxRightBound, C_RIGHT_BOUND);
        vRegWriteFldAlign(SIDE_LBOX_00, 0, C_START);
        vRegWriteFldAlign(SIDE_LBOX_01, 0, C_Y_THRES);
        #endif

        // Reset IIR Filter
        _u4LBoxIIRCount = 0;

        // Reset Adaptive Threshold
        _u4LBoxThresholdCount = LETTER_BOX_APATIVED_THD_REFLASH;
    }
}


//-----------------------------------------------------------------------------
/** Brief of LboxVerticalBoundSelect.
 */
//-----------------------------------------------------------------------------
#if SUPPORT_HORIZONTAL_LBOX    
static void LboxHorizontalBoundSelect(UINT8 fgIsLeftRight, UINT32 u4Bound1Counter, UINT32 u4Bound2Counter, UINT32 u4Bound1, UINT32 u4Bound2, UINT32 u4Final)
{
    UINT32 u4SmallBoundNs;
    u4SmallBoundNs = 0;

    // choose left bound
    if (_u4LBoxFastRecovery != 0)
    {
        // choose left bound
        if ((u4Bound1Counter == 0) && (u4Bound2Counter == 0))
        {
            // do nothing
        }
        else if (u4Bound1Counter == 0)
        {
            // Bound#1 NOT detected! Use Bound#2
            u4Final = u4Bound2;
        }
        else if (u4Bound2Counter == 0)
        {
            // Bound#2 NOT detected! Use Bound#1
            u4Final = u4Bound1;
        }
        else
        {
            // Two Bound detected! Caption/Subtitle Case
            // Pick envelop of Bound#1 & Bound#2
            if (fgIsLeftRight) // Left
            {
                if (u4Bound1 > u4Bound2)
                {
                    u4Final = u4Bound2;
                }
                else
                {
                    u4Final = u4Bound1;
                }
            }
            else
            {
                if (u4Bound1 > u4Bound2)
                {
                    u4Final = u4Bound1;
                }
                else
                {
                    u4Final = u4Bound2;
                }
            }           
        }
    }
    else
    {
        u4SmallBoundNs = (u4Bound1Counter + u4Bound2Counter) / LETTER_BOX_SMALL_BOUND;
    
        if ((u4Bound1Counter == 0) && (u4Bound2Counter == 0))
        {
            // do nothing
        }
        else if ((u4Bound1Counter < u4SmallBoundNs) && (u4Bound2Counter > u4SmallBoundNs))
        {
            // Bound#1 NOT detected! Use Bound#2
           u4Final = u4Bound2;
        }
        else
        if ((u4Bound2Counter < u4SmallBoundNs) && (u4Bound1Counter > u4SmallBoundNs))
        {
            // Bound#2 NOT detected! Use Bound#1            
            u4Final = u4Bound1;
        }
        else
        {
            // Two Bound detected! Caption/Subtitle Case
            // Pick envelop of Bound#1 & Bound#2
            LOG(4, "Two Up Bound detected! Caption/Subtitle Case! B1(%d, %d), B2(%d, %d)\n", u4Bound1, u4Bound1Counter, u4Bound2, u4Bound2Counter);

            if (fgIsLeftRight) // fgIsLeftRight:1 -> Left
            {
                if (u4Bound1 > u4Bound2)
                {
                    u4Final = u4Bound2;
                }
                else
                {
                    u4Final = u4Bound1;
                }
            }
            else // fgIsLeftRight:2 -> Right
            {
                if (u4Bound1 > u4Bound2)
                {
                    u4Final = u4Bound1;
                }
                else
                {
                    u4Final = u4Bound2;
                }
            }            
        }
    }

    
    if (fgIsLeftRight) // Left
    {
        _u4LBoxLeftFinal  = u4Final;
    }
    else // Right
    {
        _u4LBoxRightFinal  = u4Final;
    }
}
#endif

//-----------------------------------------------------------------------------
/** Brief of LboxVerticalBoundSelect.
 */
//-----------------------------------------------------------------------------
static void LboxVerticalBoundSelect(UINT8 fgIsDirect, UINT32 u4Bound1Counter, UINT32 u4Bound2Counter, UINT32 u4Bound1, UINT32 u4Bound2, UINT32 u4Final)
{
    UINT32 u4SmallBoundNs;
    u4SmallBoundNs = 0;
   
    // choose upper bound
    if (_u4LBoxFastRecovery != 0)
    {
        // choose upper bound
        if ((u4Bound1Counter == 0) && (u4Bound2Counter == 0))
        {
            // do nothing
        }
        else if (u4Bound1Counter == 0)
        {
            // Bound#1 NOT detected! Use Bound#2
            u4Final = u4Bound2;
        }
        else if (u4Bound2Counter == 0)
        {
            // Bound#2 NOT detected! Use Bound#1
            u4Final = u4Bound1;
        }
        else
        {
            // Two Bound detected! Caption/Subtitle Case
            // Pick envelop of Bound#1 & Bound#2

            if (u4Bound1 > u4Bound2)
            {
                if (u4Bound1 > u4Bound2)
                {
                    u4Final = u4Bound2;
                }
                else
                {
                    u4Final = u4Bound1;
                }
            }
            else // fgIsDirect:2 -> Low
            {
                if (u4Bound1 > u4Bound2)
                {
                    u4Final = u4Bound1;
                }
                else
                {
                    u4Final = u4Bound2;
                }
            }            
        }
    }
    else
    {
        u4SmallBoundNs = (u4Bound1Counter + u4Bound2Counter) / LETTER_BOX_SMALL_BOUND;
    
        if ((u4Bound1Counter == 0) && (u4Bound2Counter == 0))
        {
            // do nothing
        }
        else if ((u4Bound1Counter < u4SmallBoundNs) && (u4Bound2Counter > u4SmallBoundNs))
        {
            // Bound#1 NOT detected! Use Bound#2            
            u4Final = u4Bound2;
        }
        else if ((u4Bound2Counter < u4SmallBoundNs) && (u4Bound1Counter > u4SmallBoundNs))
        {
            // Bound#2 NOT detected! Use Bound#1            
            u4Final = u4Bound1;
        }
        else
        {
            // Two Bound detected! Caption/Subtitle Case
            // Pick envelop of Bound#1 & Bound#2
            LOG(4, "Two Up Bound detected! Caption/Subtitle Case! B1(%d, %d), B2(%d, %d)\n", u4Bound1, u4Bound1Counter, u4Bound2, u4Bound2Counter);

            if (fgIsDirect) // fgIsDirect:1 -> Up
            {
                if (u4Bound1 > u4Bound2)
                {
                    u4Final = u4Bound2;
                }
                else
                {
                    u4Final = u4Bound1;
                }
            }
            else // fgIsDirect:2 -> Low
            {
                if (u4Bound1 > u4Bound2)
                {
                    u4Final = u4Bound1;
                }
                else
                {
                    u4Final = u4Bound2;
                }
            }            
        }
    }

    if (fgIsDirect) // fgIsDirect:1 -> Up
    {
        _u4LBoxUpFinal  = u4Final;
    }
    else // fgIsDirect:0 -> Low
    {
        _u4LBoxLowFinal  = u4Final;
    }
}


//-----------------------------------------------------------------------------
/** Brief of LboxBoundUpdate.
 */
//-----------------------------------------------------------------------------
static void LboxBoundUpdate(UINT8 fgIsDirect, UINT32 u4Bound1Counter, UINT32 u4Bound2Counter, UINT32 u4Bound1, UINT32 u4Bound2, UINT32 u4Final, UINT32 u4Draft, UINT32 u4New, UINT32 u4Small)
{
    UINT32 u4MatchBound1;
    UINT32 u4MatchBound2;

    // If Low = IIR (Low), it means the current value is stable 
    // If Up = IIR (Up), it means the current value is stable 
    // IIR Stable, Upload Bound

    LOG(4, "_u4Bound1Counter: %d _u4Bound2Counter: %d _u4Bound1: %d _u4Bound2: %d _u4Final: %d\n", u4Bound1Counter, u4Bound2Counter, u4Bound1, u4Bound2, u4Final);

    u4MatchBound1 = 0;
    u4MatchBound2 = 0;
    if (LboxBoundStable(u4New, u4Bound1, u4Draft))
    {
        u4MatchBound1 = 1;
    }

    if (LboxBoundStable(u4New, u4Bound2, u4Draft))
    {
        u4MatchBound2 = 1;
    }

    if (u4MatchBound1 == u4MatchBound2)
    {
        // new Bound

        if (u4MatchBound1 != 0)
        {
            // both match
            UINT32 u4Delta1;
            UINT32 u4Delta2;

            if (u4New > u4Bound1)
            {
                u4Delta1 = (u4New - u4Bound1);
            }
            else
            {
                u4Delta1 = (u4Bound1 - u4New);
            }

            if (u4New > u4Bound2)
            {
                u4Delta2 = (u4New - u4Bound2);
            }
            else
            {
                u4Delta2 = (u4Bound2 - u4New);
            }

            if (u4Delta1 > u4Delta2)
            {
                u4Bound2 = u4New;
            }
            else
            {
                u4Bound1 = u4New;
            }
        }
        else if (u4Bound1Counter > u4Bound2Counter)
        {
            // Update Bound#2
            u4Bound2 = u4New;
            u4Bound2Counter = 0;
        }
        else
        {
            // Update Bound#1
            u4Bound1 = u4New;
            u4Bound1Counter = 0;
        }
    }
    else if (u4MatchBound1 != 0)
    {
        // old Bound #1
        u4Bound1 = u4New;
    }
    else
    {
        // old Bound #2
        u4Bound2 = u4New;
    }

    if (fgIsDirect == 1) // fgIsDirect:1 -> Up
    {
        // Handle Fast Recovery
        if ((u4Bound1 < u4Final) &&
            ((u4Bound1 + LETTER_BOX_FAST_RECOVERY_BOUND) < u4Final) &&
            (u4Bound1Counter > LETTER_BOX_SMALL_BOUND_MIN) && (u4Small == 0) && (u4New == u4Bound1))
        {
            _u4LBoxFastRecovery = 1;
        }

        if ((u4Bound2 < u4Final) &&
            ((u4Bound2 + LETTER_BOX_FAST_RECOVERY_BOUND) < u4Final) &&
            (u4Bound2Counter > LETTER_BOX_SMALL_BOUND_MIN) && (u4Small == 0) && (u4New == u4Bound2))
        {
            _u4LBoxFastRecovery = 1;
        }
        
        _u4LBoxBound1UpCounter = u4Bound1Counter;
        _u4LBoxBound2UpCounter = u4Bound2Counter;
        _u4LBoxBound1Up = u4Bound1;
        _u4LBoxBound2Up = u4Bound2;
//        _u4LBoxUpFinal  = u4Final;
    }
    else if (fgIsDirect == 0) // fgIsDirect:0 -> Low
    {
        // Handle Fast Recovery
        if ((u4Bound1 > u4Final) &&
            ((u4Bound1 + LETTER_BOX_FAST_RECOVERY_BOUND) < u4Final) &&
            (u4Bound1Counter > LETTER_BOX_SMALL_BOUND_MIN) && (u4Small == 0) && (u4New == u4Bound1))
        {
            _u4LBoxFastRecovery = 1;
        }

        if ((u4Bound2 > u4Final) &&
            ((u4Bound2 + LETTER_BOX_FAST_RECOVERY_BOUND) < u4Final) &&
            (u4Bound2Counter > LETTER_BOX_SMALL_BOUND_MIN) && (u4Small == 0) && (u4New == u4Bound2))
        {
            _u4LBoxFastRecovery = 1;
        }
        
        _u4LBoxBound1LowCounter = u4Bound1Counter;
        _u4LBoxBound2LowCounter = u4Bound2Counter;
        _u4LBoxBound1Low = u4Bound1;
        _u4LBoxBound2Low = u4Bound2;
//        _u4LBoxLowFinal  = u4Final;
    } 
    #if SUPPORT_HORIZONTAL_LBOX  
    else if (fgIsDirect == 3) // fgIsDirect:3 -> Left
    {
        // Handle Fast Recovery
        if ((u4Bound1 < u4Final) &&
            ((u4Bound1 + LETTER_BOX_FAST_RECOVERY_BOUND) < u4Final) &&
            (u4Bound1Counter > LETTER_BOX_SMALL_BOUND_MIN) && (u4Small == 0) && (u4New == u4Bound1))
        {
            _u4LBoxFastRecovery = 1;
        }

        if ((u4Bound2 < u4Final) &&
            ((u4Bound2 + LETTER_BOX_FAST_RECOVERY_BOUND) < u4Final) &&
            (u4Bound2Counter > LETTER_BOX_SMALL_BOUND_MIN) && (u4Small == 0) && (u4New == u4Bound2))
        {
            _u4LBoxFastRecovery = 1;
        }
        
        _u4LBoxBound1LeftCounter = u4Bound1Counter;
        _u4LBoxBound2LeftCounter = u4Bound2Counter;
        _u4LBoxBound1Left = u4Bound1;
        _u4LBoxBound2Left = u4Bound2;
 //       _u4LBoxLeftFinal  = u4Final;
    }
    else // fgIsDirect:4 -> Right
    {
        // Handle Fast Recovery
        if ((u4Bound1 > u4Final) &&
            ((u4Bound1 + LETTER_BOX_FAST_RECOVERY_BOUND) < u4Final) &&
            (u4Bound1Counter > LETTER_BOX_SMALL_BOUND_MIN) && (u4Small == 0) && (u4New == u4Bound1))
        {
            _u4LBoxFastRecovery = 1;
        }

        if ((u4Bound2 > u4Final) &&
            ((u4Bound2 + LETTER_BOX_FAST_RECOVERY_BOUND) < u4Final) &&
            (u4Bound2Counter > LETTER_BOX_SMALL_BOUND_MIN) && (u4Small == 0) && (u4New == u4Bound2))
        {
            _u4LBoxFastRecovery = 1;
        }
        
        _u4LBoxBound1RightCounter = u4Bound1Counter;
        _u4LBoxBound2RightCounter = u4Bound2Counter;
        _u4LBoxBound1Right = u4Bound1;
        _u4LBoxBound2Right = u4Bound2;
   //    _u4LBoxRightFinal  = u4Final;
    }
    #endif
}


//-----------------------------------------------------------------------------
/** Brief of LboxUploadCounter.
 */
//-----------------------------------------------------------------------------
static void LboxUploadCounter(UINT8 fgIsDirect, UINT32 u4Bound1Counter, UINT32 u4Bound2Counter, UINT32 u4Bound1, UINT32 u4Bound2, UINT32 u4Draft, UINT32 u4New, UINT32 u4Ori)
{   
    // Upload Counters
    if (u4New == u4Bound1)
    {
        // Bound#1
        u4Bound1Counter++;
    }
    else if (u4Ori == u4Bound1)
    {
        // Bound#1
        u4Bound1Counter++;
    }
    else if (LboxBoundStable(u4New, u4Bound1, u4Draft))
    {
        // Bound#1
        u4Bound1Counter++;
    }
    else if (LboxBoundStable(u4Ori, u4Bound1, u4Draft))
    {
        // Bound#1
        u4Bound1Counter++;
    }

    if (u4New == u4Bound2)
    {
        // Bound#2
        u4Bound2Counter++;
    }
    else if (u4Ori == u4Bound2)
    {
        // Bound#2
        u4Bound2Counter++;
    }
    else if (LboxBoundStable(u4New, u4Bound2, u4Draft))
    {
        // Bound#2
        u4Bound2Counter++;
    }
    else if (LboxBoundStable(u4Ori, u4Bound2, u4Draft))
    {
        // Bound#2
        u4Bound2Counter++;
    }
    
    if (fgIsDirect == 1) // fgIsDirect:1 -> Up
    {
        _u4LBoxBound1UpCounter = u4Bound1Counter;
        _u4LBoxBound2UpCounter = u4Bound2Counter;
    }
    else if (fgIsDirect == 0) // fgIsDirect:0 -> Low
    {
        _u4LBoxBound1LowCounter = u4Bound1Counter;
        _u4LBoxBound2LowCounter = u4Bound2Counter;
    }
    #if SUPPORT_HORIZONTAL_LBOX  
    else if (fgIsDirect == 3) // fgIsDirect:3 -> Left
    {
        _u4LBoxBound1LeftCounter = u4Bound1Counter;
        _u4LBoxBound2LeftCounter = u4Bound2Counter;
    }    
    else // fgIsDirect:4 -> Right
    {
        _u4LBoxBound1RightCounter = u4Bound1Counter;
        _u4LBoxBound2RightCounter = u4Bound2Counter;
    }
    #endif
}

//-----------------------------------------------------------------------------
/** Brief of LboxDetectionGetResult.
 */
//-----------------------------------------------------------------------------
static void LboxDetectionGetResult(void)
{
    UINT32 u4Low;
    UINT32 u4Up;
    UINT32 u4Small;
    #if SUPPORT_HORIZONTAL_LBOX
    UINT32 u4LeftMin;
    UINT32 u4LeftMax;
    UINT32 u4RightMin;
    UINT32 u4RightMax;
    UINT32 u4LeftDiff;
    UINT32 u4RightDiff;
    #endif

    // start
    vRegWriteFldAlign(LBOX_00, 0, LBOX_START);
    vRegWriteFldAlign(LBOX_00, 0, LBOX_STATE);
    vRegWriteFldAlign(LBOX_00, 1, LBOX_START);
    vRegWriteFldAlign(LBOX_00, 1, LBOX_STATE);
    #if SUPPORT_HORIZONTAL_LBOX    
    vRegWriteFldAlign(SIDE_LBOX_00, 1, C_START);
    #endif

    // wait
    x_thread_delay(LETTER_BOX_HARDWARE_DELAY);

    // get result
    u4Low = RegReadFldAlign(LBOX_STA_00, STA_LBOX_Y_LOW_LINE);
    u4Up = RegReadFldAlign(LBOX_STA_00, STA_LBOX_Y_UP_LINE);
    u4Small = RegReadFldAlign(LBOX_STA_00, STA_LBOX_Y_IS_SMALL);

    #if SUPPORT_HORIZONTAL_LBOX    
    u4LeftMin = RegReadFldAlign(SIDE_LBOX_STA_02, STA_BLACK_LEFT_POS_MIN);
    u4LeftMax = RegReadFldAlign(SIDE_LBOX_STA_01, STA_BLACK_LEFT_POS_MAX);
    u4RightMin = RegReadFldAlign(SIDE_LBOX_STA_02, STA_BLACK_RIGHT_POS_MIN);
    u4RightMax = RegReadFldAlign(SIDE_LBOX_STA_01, STA_BLACK_RIGHT_POS_MAX);
    u4LeftDiff = RegReadFldAlign(SIDE_LBOX_STA_00, STA_SUM_BLACK_LEFT_POS_DIFF);
    u4RightDiff = RegReadFldAlign(SIDE_LBOX_STA_00, STA_SUM_BLACK_RIGHT_POS_DIFF);
    #endif

    // Handle Small
    if (u4Small)
    {
        u4Low = _u4LBoxLowBound;
        u4Up = _u4LBoxUpBound;

        LOG(4, "Dark Frame Detected\n");
    }

    // Check Max Bound
    if (u4Up > _u4LBoxUpMax)
    {
        u4Up = _u4LBoxUpMax;
    }

    if (u4Low < _u4LBoxLowMax)
    {
        u4Low = _u4LBoxLowMax;
    }

    // Handle Low/Up value with IIR
    if (_u4LBoxIIRCount == 0)
    {
        _u4LBoxIIRLow = (u4Low << LETTER_BOX_IIR_TAP_FRACTION);
        _u4LBoxIIRUp = (u4Up << LETTER_BOX_IIR_TAP_FRACTION);
        #if SUPPORT_HORIZONTAL_LBOX    
        _u4LBoxIIRRight = (u4RightMax << LETTER_BOX_IIR_TAP_FRACTION);
        _u4LBoxIIRLeft = (u4LeftMin << LETTER_BOX_IIR_TAP_FRACTION);
        #endif    
    }
    else
    {
        _u4LBoxIIRLow -= (_u4LBoxIIRLow >> LETTER_BOX_IIR_TAP_FRACTION);    // 15/16 = 1 - 1/16
        _u4LBoxIIRLow += u4Low;                                             // +1/16

        _u4LBoxIIRUp -= (_u4LBoxIIRUp >> LETTER_BOX_IIR_TAP_FRACTION);      // 15/16 = 1 - 1/16
        _u4LBoxIIRUp += u4Up;                                               // +1/16
        #if SUPPORT_HORIZONTAL_LBOX    
        _u4LBoxIIRRight -= (_u4LBoxIIRRight >> LETTER_BOX_IIR_TAP_FRACTION);    // 15/16 = 1 - 1/16
        _u4LBoxIIRRight += u4RightMax;                                             // +1/16

        _u4LBoxIIRLeft -= (_u4LBoxIIRLeft >> LETTER_BOX_IIR_TAP_FRACTION);      // 15/16 = 1 - 1/16
        _u4LBoxIIRLeft += u4LeftMin;                                               // +1/16
        #endif
    }

    
    if (++_u4LBoxIIRCount <= LETTER_BOX_IIR_TAP)
    {
        // update stable time        
        HAL_GetTime(&_rStableTime);
        _rBoundTime = _rStableTime;
        _u4LBoxBound1UpCounter = 0;
        _u4LBoxBound2UpCounter = 0;
        _u4LBoxBound1LowCounter = 0;
        _u4LBoxBound2LowCounter = 0;
        #if SUPPORT_HORIZONTAL_LBOX  
        _u4LBoxBound1LeftCounter = 0;
        _u4LBoxBound2LeftCounter = 0;
        _u4LBoxBound1RightCounter = 0;
        _u4LBoxBound2RightCounter = 0;
        #endif

        return;
    }
    
    // IIR stable at least 8 taps
    //  (++_u4LBoxIIRCount > LETTER_BOX_IIR_TAP)  
    {
        HAL_TIME_T rCurrentTime;
        HAL_TIME_T rDeltaTime;
        
        UINT32 u4NewLow;
        UINT32 u4NewUp;
        UINT32 u4Update;
        #if SUPPORT_HORIZONTAL_LBOX    
        UINT32 u4NewRight;
        UINT32 u4NewLeft;
        #endif

        _u4LBoxIIRCount = LETTER_BOX_IIR_TAP;

        u4Update = 0;
        u4NewLow = (_u4LBoxIIRLow >> LETTER_BOX_IIR_TAP_FRACTION);
        u4NewUp = (_u4LBoxIIRUp >> LETTER_BOX_IIR_TAP_FRACTION);
        #if SUPPORT_HORIZONTAL_LBOX    
        u4NewRight = (_u4LBoxIIRRight >> LETTER_BOX_IIR_TAP_FRACTION);
        u4NewLeft = (_u4LBoxIIRLeft >> LETTER_BOX_IIR_TAP_FRACTION);
        #endif

        HAL_GetTime(&rCurrentTime);
        HAL_GetDeltaTime(&rDeltaTime, &_rStableTime, &rCurrentTime);

        if ((rDeltaTime.u4Seconds >= _u4LBoxStableTime) || (_u4LBoxFastRecovery != 0))
        {
            // Select upside letter box from cadidate and store the result to _u4LBoxUpFinal
            LboxVerticalBoundSelect(1, _u4LBoxBound1UpCounter, _u4LBoxBound2UpCounter, _u4LBoxBound1Up, _u4LBoxBound2Up, _u4LBoxUpFinal);
            // Select downside letter box from cadidate and store the result to _u4LBoxLowFinal
            LboxVerticalBoundSelect(0, _u4LBoxBound1LowCounter, _u4LBoxBound2LowCounter, _u4LBoxBound1Low, _u4LBoxBound2Low, _u4LBoxLowFinal);            
            
            #if SUPPORT_HORIZONTAL_LBOX    
            LboxHorizontalBoundSelect(1, _u4LBoxBound1LeftCounter, _u4LBoxBound2LeftCounter, _u4LBoxBound1Left, _u4LBoxBound2Left, _u4LBoxLeftFinal);
            LboxHorizontalBoundSelect(0, _u4LBoxBound1RightCounter, _u4LBoxBound2RightCounter, _u4LBoxBound1Right, _u4LBoxBound2Right, _u4LBoxRightFinal);            
            #endif
            _u4LBoxFastRecovery = 0;

            // Content too dark, can not make decision!
            if ((_u4LBoxTooDark != 0) && (_u4LBoxLastTooDark != 0))
            {
                _u4LBoxUpFinal = _u4LBoxUpBound;
                _u4LBoxLowFinal = _u4LBoxLowBound;
                #if SUPPORT_HORIZONTAL_LBOX    
                _u4LBoxLeftFinal = _u4LBoxLeftBound;
                _u4LBoxRightFinal = _u4LBoxRightBound;
                #endif
                LOG(4, "Too Dark!\n");
            }
            _u4LBoxLastTooDark = _u4LBoxTooDark;

            // check if we need to update new letter box position
            LOG(4, "_u4LBoxLowFinal: %d _u4LBoxLowCurrent: %d _u4LboxInvalidDraft1: %d\n", _u4LBoxLowFinal, _u4LBoxLowCurrent, _u4LboxInvalidDraft1);
            if (LboxBoundStable(_u4LBoxLowFinal, _u4LBoxLowCurrent, _u4LboxInvalidDraft1) == 0)
            {
                // If new detected letter box is NOT inside (Current - draft, Current + draft), we need to update
                u4Update = 1;
                LOG(4, "LBOX Low Update\n");
            }

            LOG(4, "_u4LBoxUpFinal: %d _u4LBoxUpCurrent: %d _u4LboxInvalidDraft1: %d\n", _u4LBoxUpFinal, _u4LBoxUpCurrent, _u4LboxInvalidDraft1);
            if (LboxBoundStable(_u4LBoxUpFinal, _u4LBoxUpCurrent, _u4LboxInvalidDraft1) == 0)
            {
                // If New Final NOT inside (Current - 2, Current + 2), we need to update
                u4Update = 1;
                LOG(4, "LBOX Up Update\n");
            }

            #if SUPPORT_HORIZONTAL_LBOX            
            if ((u4LeftDiff < _u4LBoxSumDiffThreshold) && (u4RightDiff < _u4LBoxSumDiffThreshold))
            {
                if (LboxBoundStable(u4LeftMax, u4LeftMin, _u4LboxInvalidDraft3) == 0)
                {
                    u4Update = 1;
                    LOG(4, "LBOX Right Update\n");
                }
                if (LboxBoundStable(u4RightMax, u4RightMin, _u4LboxInvalidDraft3) == 0)
                {
                    u4Update = 1;
                    LOG(4, "LBOX Left Update\n");
                }
                if (LboxBoundStable(_u4LBoxRightFinal, _u4LBoxRightCurrent, _u4LboxInvalidDraft3) == 0)
                {
                    u4Update = 1;
                    LOG(4, "LBOX Right Update\n");
                }
                if (LboxBoundStable(_u4LBoxLeftFinal, _u4LBoxLeftCurrent, _u4LboxInvalidDraft3) == 0)
                {
                    u4Update = 1;
                    LOG(4, "LBOX Left Update\n");
                }
            }
            #endif

            // Check Chroma
            if ((_u4LBoxUpFinal == _u4LBoxUpBound) && (_u4LBoxLowFinal == _u4LBoxLowBound)
                #if SUPPORT_HORIZONTAL_LBOX        
                && (_u4LBoxLeftFinal == _u4LBoxLeftBound) && (_u4LBoxRightFinal == _u4LBoxRightBound)
                #endif                
                )
            {
                // do not need to check
            }
            else
            {
                #if SUPPORT_LUMACHROMA_CHECK
                LboxCheckLumaChroma();
                #endif
            }

            _u4LBoxDetectionValid = 1;

            // Reset Timer and Counter
            _rStableTime = rCurrentTime;
            _u4LBoxBound1UpCounter = 0;
            _u4LBoxBound2UpCounter = 0;
            _u4LBoxBound1LowCounter = 0;
            _u4LBoxBound2LowCounter = 0;
            #if SUPPORT_HORIZONTAL_LBOX     
            _u4LBoxBound1LeftCounter = 0;
            _u4LBoxBound2LeftCounter = 0;
            _u4LBoxBound1RightCounter = 0;
            _u4LBoxBound2RightCounter = 0;
            #endif


            #if SUPPORT_HORIZONTAL_LBOX     
            LOG(4, "TIME (%d.%06d), FINAL(%d %d %d %d) U(%d)\n", _rStableTime.u4Seconds, _rStableTime.u4Micros,
            _u4LBoxUpFinal, _u4LBoxLowFinal, _u4LBoxLeftFinal, _u4LBoxRightFinal, u4Update);
            #else
            LOG(4, "TIME (%d.%06d), FINAL(%d %d) U(%d)\n", _rStableTime.u4Seconds, _rStableTime.u4Micros,
            _u4LBoxUpFinal, _u4LBoxLowFinal, u4Update);
            #endif            

            if (u4Update != 0)
            {
                // Check Caption Video
                LboxCheckCaptionVideo();
            
                // Update New Up/Low
                LboxSetPosSize();
            }
            else
            {
                // Check Stable Video
                LboxCheckStableVideo();
            }
        }

        // Determine stable or NOT
        HAL_GetDeltaTime(&rDeltaTime, &_rBoundTime, &rCurrentTime);
        if ((u4NewLow == u4Low) && (u4NewUp == u4Up) && 
            #if SUPPORT_HORIZONTAL_LBOX  
            (u4NewRight == u4RightMax) && (u4NewLeft == u4LeftMin) &&
            #endif        
            (rDeltaTime.u4Seconds >= LETTER_BOX_BOUND_TIME))            
        {
            // Reset Timer and Counter
            _rBoundTime = rCurrentTime;
            LOG(4, "[TEST] _u4LBoxUpFinal: %d _u4LBoxLowFinal: %d\n", _u4LBoxUpFinal, _u4LBoxLowFinal);

            // update upside letter box candidate
            LboxBoundUpdate(1, _u4LBoxBound1UpCounter, _u4LBoxBound2UpCounter, _u4LBoxBound1Up, _u4LBoxBound2Up, _u4LBoxUpFinal,  _u4LboxInvalidDraft2 , u4NewUp, u4Small);
            // update downside letter box candidate
            LboxBoundUpdate(0, _u4LBoxBound1LowCounter, _u4LBoxBound2LowCounter, _u4LBoxBound1Low, _u4LBoxBound2Low, _u4LBoxLowFinal,  _u4LboxInvalidDraft2 , u4NewLow, u4Small);
            #if SUPPORT_HORIZONTAL_LBOX  
            // update leftside letter box candidate
            LboxBoundUpdate(3, _u4LBoxBound1LeftCounter, _u4LBoxBound2LeftCounter, _u4LBoxBound1Left, _u4LBoxBound2Left, _u4LBoxLeftFinal,  _u4LboxInvalidDraft4 , u4NewLeft, 0);
            // update rightside letter box candidate
            LboxBoundUpdate(2, _u4LBoxBound1RightCounter, _u4LBoxBound2RightCounter, _u4LBoxBound1Right, _u4LBoxBound2Right, _u4LBoxRightFinal,  _u4LboxInvalidDraft4 , u4NewRight, 0);
            #endif

            LOG(5, "BOUND B1(%d, %d, %d, %d), B2(%d, %d, %d, %d)\n",
                _u4LBoxBound1Up, _u4LBoxBound1Low, _u4LBoxBound1UpCounter, _u4LBoxBound1LowCounter,
                _u4LBoxBound2Up, _u4LBoxBound2Low, _u4LBoxBound2UpCounter, _u4LBoxBound2LowCounter);

            #if SUPPORT_HORIZONTAL_LBOX  
            LOG(5, "H_BOUND B1(%d, %d, %d, %d), B2(%d, %d, %d, %d)\n",
                _u4LBoxBound1Left, _u4LBoxBound1Right, _u4LBoxBound1LeftCounter, _u4LBoxBound1RightCounter,
                _u4LBoxBound2Left, _u4LBoxBound2Right, _u4LBoxBound2LeftCounter, _u4LBoxBound2RightCounter);
            #endif
            if (_u4LBoxFastRecovery != 0)
            {
                LOG(4, "Fast Recovery B1(%d, %d), B2(%d, %d), FINAL(%d %d)\n",
                    _u4LBoxBound1Up, _u4LBoxBound1Low, _u4LBoxBound2Up, _u4LBoxBound2Low, _u4LBoxUpFinal, _u4LBoxLowFinal);
            }
        }

        // update counter of upside letter box candidate
        LboxUploadCounter(1, _u4LBoxBound1UpCounter, _u4LBoxBound2UpCounter, _u4LBoxBound1Up, _u4LBoxBound2Up, _u4LboxInvalidDraft2, u4NewUp, u4Up);
        // update counter of downside letter box candidate
        LboxUploadCounter(0, _u4LBoxBound1LowCounter, _u4LBoxBound2LowCounter, _u4LBoxBound1Low, _u4LBoxBound2Low, _u4LboxInvalidDraft2, u4NewLow, u4Low);
        #if SUPPORT_HORIZONTAL_LBOX  
        // update counter of leftside letter box candidate
        LboxUploadCounter(3, _u4LBoxBound1LeftCounter, _u4LBoxBound2LeftCounter, _u4LBoxBound1Left, _u4LBoxBound2Left, _u4LboxInvalidDraft4, u4NewLeft, u4LeftMin);
        // update counter of rightside letter box candidate
        LboxUploadCounter(2, _u4LBoxBound1RightCounter, _u4LBoxBound2RightCounter, _u4LBoxBound1Right, _u4LBoxBound2Right, _u4LboxInvalidDraft4, u4NewRight, u4RightMax);
        #endif

        LOG(6, "NEW(%d %d), IIR(%d %d), B1(%d, %d), B2(%d, %d)\n",
            u4Up, u4Low, u4NewUp, u4NewLow, _u4LBoxBound1Up, _u4LBoxBound1Low, _u4LBoxBound2Up, _u4LBoxBound2Low);
        #if SUPPORT_HORIZONTAL_LBOX  
        LOG(6, "H_NEW(%d %d), IIR(%d %d), B1(%d, %d), B2(%d, %d)\n",
            u4LeftMin, u4RightMax, u4NewLeft, u4NewRight, _u4LBoxBound1Left, _u4LBoxBound1Right, _u4LBoxBound2Left, _u4LBoxBound2Right);
        #endif
    }

}

//-----------------------------------------------------------------------------
/** Brief of LboxDetectionLoop.
 */
//-----------------------------------------------------------------------------
static void LboxDetectionLoop(void *pvArgs)
{
    UNUSED(pvArgs);
    UINT8 u1Dispmode;
    UINT8 u1PreviousDispmode = 0;

    while (1)
    {
        while (_u4LBoxDetectionEnable != 0)
        {
            // delay
            #if (LETTER_BOX_LOOP_DELAY > LETTER_BOX_HARDWARE_DELAY)           
            x_thread_delay(LETTER_BOX_LOOP_DELAY - LETTER_BOX_HARDWARE_DELAY);
            #endif

           //u1Dispmode = _VDP_vGetDispModeOnOff(SV_VP_MAIN);
           u1Dispmode = bLBOXIsInputDispmode();

            // normal LBD procedure
            if (bLBOXIsInputValid() && (_u4LBoxDetectionFixed == 0) && (u1Dispmode == 0))
            {                                   
                // reconfig
                LboxDetectionConfig();

                // Handle Adaptive Threshold
                LboxAdaptiveThreshold();

                // measure
                LboxDetectionGetResult();
            }
            else
            if ((_u4LBoxDetectionFixed != 0) || (u1Dispmode == 1))
            {
                // recover LBD under Dispmode
                if ((_u4LBoxDetectionValid != 0) && (u1PreviousDispmode != u1Dispmode) && (_u4LBoxDetectionFixed == 0))
                {
                        _u4LBoxUpFinal = _u4LBoxUpBound;
                        _u4LBoxLowFinal = _u4LBoxLowBound;
                        #if SUPPORT_HORIZONTAL_LBOX  
                        _u4LBoxLeftFinal = _u4LBoxLeftBound;
                        _u4LBoxRightFinal = _u4LBoxRightBound;
                        #endif            
                        LboxSetPosSize();
                        _u4LBoxDetectionValid = 0;
                        // Reset IIR Filter
                        _u4LBoxIIRCount = 0;
                }

                // handle input source change case
                if ((_u4LBoxSourceChange != 0) ||
                    (_u1LBoxSrcInterlace != bLBOXIsInputInterlace()))
                {
                    // Source Change at Fixed Mode
                    _u4LBoxSourceChangeFixed = 1;
                    _u4LBoxSourceChange = 0;
                    _u1LBoxSrcInterlace = bLBOXIsInputInterlace();

                    // Reset Up/Low Final
                    _u4LBoxUpFinal = _u4LBoxUpBound;
                    _u4LBoxLowFinal = _u4LBoxLowBound;
                    #if SUPPORT_HORIZONTAL_LBOX  
                    _u4LBoxLeftFinal = _u4LBoxLeftBound;
                    _u4LBoxRightFinal = _u4LBoxRightBound;
                    #endif
                    
                    // Reset Bound#1 and Bound#2
                    _u4LBoxBound1Up = _u4LBoxUpBound;
                    _u4LBoxBound1Low = _u4LBoxLowBound;
                    _u4LBoxBound2Up = _u4LBoxUpBound;
                    _u4LBoxBound2Low = _u4LBoxLowBound;
                    _u4LBoxBound1UpCounter = 0;
                    _u4LBoxBound2UpCounter = 0;
                    _u4LBoxBound1LowCounter = 0;
                    _u4LBoxBound2LowCounter = 0;
                    #if SUPPORT_HORIZONTAL_LBOX  
                    _u4LBoxBound1Left = _u4LBoxUpBound;
                    _u4LBoxBound1Right = _u4LBoxLowBound;
                    _u4LBoxBound2Left = _u4LBoxUpBound;
                    _u4LBoxBound2Right = _u4LBoxLowBound;
                    _u4LBoxBound1LeftCounter = 0;
                    _u4LBoxBound2LeftCounter = 0;
                    _u4LBoxBound1RightCounter = 0;
                    _u4LBoxBound2RightCounter = 0;
                    #endif

                    // reset parameters of caption video detection
                    LboxResetCaptionVideo();
                    
                    // reset parameters of stable video detection
                    LboxResetStableVideo();

                    // callback scalar for letter box cutting
                    LboxSetPosSize();
                }
            }

            u1PreviousDispmode = u1Dispmode;
        }

        if (_u4LBoxDetectionValid != 0)
        {
            if (_u4LBoxDetectionFixed == 0)
            {
                // Reset Up/Low Final
                _u4LBoxUpFinal = _u4LBoxUpBound;
                _u4LBoxLowFinal = _u4LBoxLowBound;
                #if SUPPORT_HORIZONTAL_LBOX  
                _u4LBoxLeftFinal = _u4LBoxLeftBound;
                _u4LBoxRightFinal = _u4LBoxRightBound;
                #endif
                // callback scalar for letter box cutting
                LboxSetPosSize();
                _u4LBoxDetectionValid = 0;
            }
        
            // Reset IIR Filter
            _u4LBoxIIRCount = 0;
        }
        
        VERIFY (x_sema_lock(_hLBoxSemaphore, X_SEMA_OPTION_WAIT) == OSR_OK);        
    }    
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void LboxSetOnOff(UINT8 bOnOff, UINT8 bFixed) 
{
    _u4LBoxDetectionFixed = (UINT32) bFixed;

    if (_u4LBoxDetectionInit != 0)
    {
        if (bOnOff != 0)
        {
            if (_u4LBoxDetectionEnable == 0)
            {
                LboxDetectionUnlock();
            }
        }
        else
        {
            _u4LBoxDetectionEnable = 0;
        }
    }
}


UINT8 LboxQueryRegion(UINT32 *pu4LBoxX, UINT32 *pu4LBoxY, UINT32 *pu4Width, UINT32 *pu4Height, UINT32 *pu4HValid, UINT32 *pu4VValid)
{
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4LBoxY;
    UINT32 u4NewHeight;
    #if SUPPORT_HORIZONTAL_LBOX    
    UINT32 u4LBoxX;
    UINT32 u4NewWidth;
    #endif

    if ((pu4LBoxX == NULL) || (pu4LBoxY == NULL) || (pu4Width == NULL) || (pu4Height == NULL) || (pu4HValid == NULL) || (pu4VValid == NULL))
    {
        return (SV_FAIL);
    }
    
    // Get Source Size
    //u4Width = wDrvVideoInputWidth(SV_VP_MAIN);
    //u4Height = wDrvVideoInputHeight(SV_VP_MAIN);
    vLBoxGetInputSourceResolution(&u4Width, &u4Height);
    *pu4HValid = 0;
    *pu4VValid = 0;
    
    #if SUPPORT_HORIZONTAL_LBOX    
    u4LBoxX = 0;
    #endif
    u4LBoxY = 0;
    
    if ((_u4LBoxDetectionEnable != 0) && (_u4LBoxDetectionValid != 0) && (!bLBOXIsInput444()))   // 444 can not use letterbox detection (detect G only!)
    {
            if ((_u4LBoxLowFinal > _u4LBoxUpFinal)
                #if SUPPORT_HORIZONTAL_LBOX    
                 || (_u4LBoxRightFinal > _u4LBoxLeftFinal)
                #endif
                )
            {
                if (_u1LBoxSrcInterlace != 0)
                {
                    u4LBoxY = (_u4LBoxUpFinal << 1);
                    u4NewHeight = ((_u4LBoxLowFinal - _u4LBoxUpFinal) << 1);
                }
                else
                {
                    u4LBoxY = _u4LBoxUpFinal;
                    u4NewHeight = (_u4LBoxLowFinal - _u4LBoxUpFinal);
                }
                *pu4VValid = 1;

                if (u4LBoxY > LETTER_BOX_PROTECT_BOUND)
                {
                    // Handle Up Protect
                    u4LBoxY -= LETTER_BOX_PROTECT_BOUND;
                    if ((u4Height - u4NewHeight) > (LETTER_BOX_PROTECT_BOUND * 2))
                    {
                        // Handle Low Protect
                        u4NewHeight += (LETTER_BOX_PROTECT_BOUND * 2);
                    }
                }
                else if ((u4Height - u4NewHeight) > LETTER_BOX_PROTECT_BOUND)
                {
                    // Handle Low Protect
                    u4NewHeight += LETTER_BOX_PROTECT_BOUND;
                }
                
                #if SUPPORT_HORIZONTAL_LBOX    
                u4LBoxX = _u4LBoxLeftFinal;
                u4NewWidth = (_u4LBoxRightFinal - _u4LBoxLeftFinal);
                //u4Width = u4NewWidth;
                if (!_u4LBoxSrcWidth)
                {
                    return (SV_FAIL);
                }
                // get original input width
                //u4LBoxX = u4LBoxX * (UINT32)wDrvVideoInputWidth(SV_VP_MAIN)/_u4LBoxSrcWidth;
                //u4Width = u4Width * (UINT32)wDrvVideoInputWidth(SV_VP_MAIN)/_u4LBoxSrcWidth;
                u4LBoxX = u4LBoxX * u4Width/_u4LBoxSrcWidth;            
                u4Width = u4NewWidth * u4Width/_u4LBoxSrcWidth;

                *pu4HValid = 1;
                #endif

                u4Height = u4NewHeight;
                #if SUPPORT_HORIZONTAL_LBOX   
                LOG(3, "LBOX (%d %d %d %d)\n", u4LBoxX, u4LBoxY, u4Width, u4Height);
                #else
                LOG(3, "LBOX (%d %d %d)\n", u4LBoxY, u4Width, u4Height);
                #endif
            }

            _u4LBoxUpCurrent = _u4LBoxUpFinal;
            _u4LBoxLowCurrent = _u4LBoxLowFinal;
            #if SUPPORT_HORIZONTAL_LBOX    
            _u4LBoxLeftCurrent = _u4LBoxLeftFinal;
            _u4LBoxRightCurrent = _u4LBoxRightFinal;
            #endif
    }
    else
    {
            _u4LBoxUpCurrent = _u4LBoxUpBound;
            _u4LBoxLowCurrent = _u4LBoxLowBound;
            #if SUPPORT_HORIZONTAL_LBOX    
            _u4LBoxLeftCurrent = _u4LBoxLeftBound;
            _u4LBoxRightCurrent = _u4LBoxRightBound;
            #endif
    }    

    *pu4Width = u4Width;
    *pu4Height = u4Height;
    #if SUPPORT_HORIZONTAL_LBOX    
    *pu4LBoxX = u4LBoxX;
    #endif
    *pu4LBoxY = u4LBoxY;

    return (SV_SUCCESS);
}

void LboxDetectionInit(void)
{
    if (_u4LBoxDetectionInit == 0)
    {
        HANDLE_T hThread1;
        
        _u4LBoxDetectionInit = 1;

        // create semaphore
		if (x_sema_create(&_hLBoxSemaphore, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
		{
			ASSERT(0);
		}
		
        // create thread
        if (x_thread_create(&hThread1, "LBOX", 1024, 100,
            LboxDetectionLoop, 0, NULL) != OSR_OK)
        {
            ASSERT(0);
        }
    }
}


void LboxSourceChange(UINT32 u4SrcWidth, UINT32 u4SrcHeight, UINT8 u1SrcInterlace)
{
        _u4LBoxSrcWidth = u4SrcWidth;
        _u4LBoxSrcHeight = u4SrcHeight;
        _u1LBoxSrcInterlace = u1SrcInterlace;

        // Reset LetterBox if Source Change
        _u4LBoxDetectionValid = 0;
        _u4LBoxSourceChange = 1;
}

UINT8 bDrvLboxGetActiveImageArea(UINT16 *x, UINT16 *y, UINT16 *w, UINT16 *h)
{
    if (_u4LBoxDetectionValid != 0)
    {
        *y = _u4LBoxUpFinal;
        *h = _u4LBoxLowFinal - _u4LBoxUpFinal;
        #if SUPPORT_HORIZONTAL_LBOX  
        *x = _u4LBoxLeftFinal;
        *w = _u4LBoxRightFinal - _u4LBoxLeftFinal;
        #else
        *x = 0; *w = 0;
        #endif
        return 0;
    }
    
    return 1;
}

