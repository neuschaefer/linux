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
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "general.h"
#include "drv_video.h"
#include "video_def.h"
#include "drv_common.h"
#include "vdp_drvif.h"
#include "x_timer.h"
#include "nptv_debug.h"
#include "drvcust_if.h"
#include "hw_ycproc.h"
#include "x_assert.h"
#include "drv_lbox.h"
#include "drv_scaler.h"
#include "vdp_display.h"
#include "vdo_misc.h"
#include "drv_meter.h"


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

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
#define LBOX_SIZE_AUTO_CUT 1

#define LBOX_BOUND_MATCH_WEIGHT 		1
#define LBOX_BOUND_MISS_WEIGHT 		0
#define LETTER_BOX_SMALL_BOUND_MIN          (8*LBOX_BOUND_MATCH_WEIGHT)       ///< 8 time detected
#define LETTER_BOX_UP_LOW_BOUND             4       ///< Avoid vsync interference, 4%
#define LETTER_BOX_LEFT_RIGHT_BOUND             15  ///< 15%
#define LETTER_BOX_MAX_BOUND                25      ///< 25%, Note 16:9 >> 4:3, About 12.5% (We set max = 25%, therefore, we can handle 32:9 inside 4:3)
#define LETTER_BOX_PROTECT_BOUND            2       ///< Region = (Up - 2, Low + 2)
#define LETTER_BOX_FAST_RECOVERY_BOUND      10      ///< Fast Recovery From LetterBox to Full
//#define LETTER_BOX_FAST_RECOVERY_TAP        4       ///No use?

#define LBOX_CAPTION_BOUND              10      ///< 10 %
#define LBOX_CAPTION_MONITOR_DEPTH	3

#define LETTER_BOX_IIR_VALID_DRIFT_1        2       ///< 2%: for _u4LboxInvalidDraft1
#define LETTER_BOX_IIR_VALID_DRIFT_2        4       ///< 4%: for _u4LboxInvalidDraft2
#define LETTER_BOX_IIR_TAP_FRACTION         2       ///< IIR tap order
#define LETTER_BOX_IIR_TAP                  (1 << LETTER_BOX_IIR_TAP_FRACTION)

#define LETTER_BOX_BOUND_TIME               2       ///< 2 seconds
#define LETTER_BOX_FCNT_BOUND               0x10            ///< threahold for HW to report state changed when field counter reaches the bound
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

//tt add change for a define
#define LBOX_HISTGRAM_NORMAL_BASE 	10000
#define LBOX_HISTGRAM_TOO_DARK                   9500  // 95%
#define LBOX_HISTGRAM_DARK                      6250// 62.5%
#define LBOX_HISTGRAM_FIT                       2500 //25%
#define LBOX_HISTGRAM_EDGE                      1250  //12.5$

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct
{
	UINT32 u4DetectBound;  //_u4LBoxUpBound,_u4LBoxLowBound,_u4LBoxLeftBound,_u4LBoxRightBound 
	UINT32 u4MaxBound;//_u4LBoxUpMax, _u4LBoxLowMax
	UINT32 u4FinalBound; //_u4LBoxUpFinal,_u4LBoxLowFinal,_u4LBoxLeftFinal,_u4LBoxRightFinal
	UINT32 u4BoundIIR;
	//cadidate
	UINT32 u4Cadidate1;  //_u4LBoxBound1
	UINT32 u4Cadidate2;  //_u4LBoxBound2
	UINT32 u4CadidateCnt1; //_u4LBoxBound1Counter
	UINT32 u4CadidateCnt2;  //_u4LBoxBound2Counter
}LBOX_DIR_DATA_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#if 0
#define _u4LBoxUpBound   (((wDrvVideoInputHeight(SV_VP_MAIN) >> bDrvVideoIsSrcInterlace(SV_VP_MAIN)) * LETTER_BOX_UP_LOW_BOUND) / 100)
#define _u4LBoxUpMax     (((wDrvVideoInputHeight(SV_VP_MAIN) >> bDrvVideoIsSrcInterlace(SV_VP_MAIN)) * LETTER_BOX_MAX_BOUND) / 100)
#define _u4LBoxLowBound  (((wDrvVideoInputHeight(SV_VP_MAIN) >> bDrvVideoIsSrcInterlace(SV_VP_MAIN)) * (100 -LETTER_BOX_UP_LOW_BOUND)) / 100)
#define _u4LBoxLowMax    (((wDrvVideoInputHeight(SV_VP_MAIN) >> bDrvVideoIsSrcInterlace(SV_VP_MAIN)) * (100 -LETTER_BOX_MAX_BOUND)) / 100)
#define _u4LBoxLeftBound (((UINT32)u4ScpipGetPreScaleWidth(SV_VP_MAIN) * _rMPicInfo.rOverScan.u4Left) / VDP_MAX_REGION_WIDTH)
#define _u4LBoxRightBound ((UINT32)u4ScpipGetPreScaleWidth(SV_VP_MAIN) - _u4LBoxLeftBound)
#define _u4LBoxSumDiffThreshold ((wDrvVideoInputHeight(SV_VP_MAIN) >> bDrvVideoIsSrcInterlace(SV_VP_MAIN)) * LETTER_BOX_SUM_DIFF_BOUND)
#endif
//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------
static UINT32 LboxIsBoundStable(UINT32 u4Input, UINT32 u4Bound, UINT32 u4Drift);
static void LboxDetectionUnlock(void);

//-----------------------------------------------------------------------------
// I/O Interface
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static LBOX_DIR_DATA_T _rBoundData[LBOX_IDX_MAX] = 
{
	{0,0,0,0,0,0,0,0},//up
	{0,0,0,0,0,0,0,0},//down
#if SUPPORT_HORIZONTAL_LBOX
	{0,0,0,0,0,0,0,0},//left
	{0,0,0,0,0,0,0,0},//right
#endif
}; 

//tt add the sign of soure change
UINT32 _u4StableTime = 15; //10; //2;
UINT32 _u4VeryStableTime = 25; //20; //4;
UINT32 _u4CaptionStableTime = 30; //6;
// Stable Time
static UINT8 u1LBoxReconfigflg = 0;	// lbox reconfig when input timing change or source region change
static UINT32 _u4LBoxStableTime;        ///< SW LBD Peroid (sec)
static HAL_TIME_T _rStableTime;
static HAL_TIME_T _rBoundTime;

static UINT32 _u4LBoxDetectionInit = 0;                        ///< flag to indicate letter box detetion module is initiated or not
static UINT32 _u4LBoxDetectionEnable = 0;                      ///< flag to indicate letter box detection is enabled or not
static UINT32 _u4LBoxDetectionValid = 0;                       ///< flag to indicate letter box detection results are valid or not
static UINT32 _u4LBoxDetectionFixed = 0;                   ///< Pause SW LBD. Keep all sw parameters fixed.
//static UINT32 _u4LBoxSourceChangeFixed = 0;         //tttttttttt check if it is necessary ,if it is the same as u1LBoxReconfigflg (_u4LBoxSourceChangeFixed) 
static HANDLE_T _hLBoxSemaphore;

// IIR
static UINT32 _u4LBoxIIRCount = 0;

// Fast Recovery
static UINT32 _u4LBoxFastRecovery = 0;

//Caption monitor
static UINT8 rCaptionHistory[2][LBOX_CAPTION_MONITOR_DEPTH];  // for caption moitor, work only for top and down side

// Bound & Final & Current
static UINT32 _u4LboxInvalidDraft1 = 0;     ///< the difference threshold between current letter box and new deteced letter box to report scalar to update LBD
static UINT32 _u4LboxInvalidDraft2 = 0;     ///< the difference threshold to update letter box candidates
#if SUPPORT_HORIZONTAL_LBOX
static UINT32 _u4LboxInvalidDraft3 = 0;
static UINT32 _u4LboxInvalidDraft4 = 0;
static UINT32 _u4LBoxSumDiffThreshold = 0;
#endif

// External Stable Time
static UINT32 _u4LBoxUpFinalBackup[3];          ///< history final result of upper bound detection
static UINT32 _u4LBoxLowFinalBackup[3];         ///< history final result of lower bound detection
static UINT32 _u4LBoxUpStableBackup[2];
static UINT32 _u4LBoxLowStableBackup[2];

// Adaptive Threshold
static UINT32 _u4LBoxTooDark = 0;                   ///< Dark Screen, Should NOT enable letter box detection
static UINT32 _u4LBoxThresholdCount = LETTER_BOX_APATIVED_THD_REFLASH;
static UINT32 LBOX_HISTGRAM_DARKCount = 0;        ///< the changce that the picture is too dark
static UINT32 _au4LboxThresholdCount[LETTER_BOX_APATIVED_THD_NS] = { 0, 0, 0, 0 };          ///< Score of 4 adaptive threshold of Luma   
const UINT32 _au4LboxThresholdValue[LETTER_BOX_APATIVED_THD_NS] = { 32, 48, 64, 80 };     ///< 4 adaptive threshold of Luma   


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static void LboxSetPosSize()
{            
    #if LBOX_SIZE_AUTO_CUT
        if(bVideoUpdateSrcRegion(SV_VP_MAIN) != SV_SUCCESS)
        {
        }
    #endif
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
        *pu4Width = (UINT32)u4ScpipGetPreScaleWidth(SV_VP_MAIN);
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

static BOOL bLBOXIsInputScalerDispmode(void)
{
    UINT8 u1Dispmode;

    u1Dispmode = u1ScpipGetDispMode(SV_VP_MAIN);
    if ((u1Dispmode != VDP_SCPOS_DISPMODE_AUTO) && (u1Dispmode != VDP_SCPOS_DISPMODE_MANUAL))
    {
        return 0;
    }
    else
    {
        return 1;
    }
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

static void LboxDetectionUnlock(void)
{
	_u4LBoxDetectionEnable = 1;	
	VERIFY (x_sema_unlock(_hLBoxSemaphore) == OSR_OK);
}

static void LboxResetCaptionVideo(void)
{
    UINT8 i;
    
    _u4LBoxStableTime = _u4StableTime;

    for(i=0; i<3; i++)
    {
        _u4LBoxLowFinalBackup[i] = _rBoundData[LBOX_IDX_DOWN].u4DetectBound;
        _u4LBoxUpFinalBackup[i] = _rBoundData[LBOX_IDX_UP].u4DetectBound;
    }
}

static void LboxResetStableVideo(void)
{
    _u4LBoxStableTime = _u4StableTime;

    _u4LBoxLowStableBackup[1] = _rBoundData[LBOX_IDX_DOWN].u4DetectBound;
    _u4LBoxLowStableBackup[0] = _rBoundData[LBOX_IDX_DOWN].u4DetectBound;

    _u4LBoxUpStableBackup[1] = _rBoundData[LBOX_IDX_UP].u4DetectBound;
    _u4LBoxUpStableBackup[0] = _rBoundData[LBOX_IDX_UP].u4DetectBound;
}

static void ResetFinaldata(void)
{
    UINT8 i;

    for(i=0; i<LBOX_IDX_MAX; i++)
    {
    	_rBoundData[i].u4FinalBound = _rBoundData[i].u4DetectBound;
    }
}

static void ResetCandidateBound(void)
{
    UINT8 i;
    
    for(i=0; i<LBOX_IDX_MAX; i++)
    {
    	_rBoundData[i].u4Cadidate1 = _rBoundData[i].u4DetectBound;
    	_rBoundData[i].u4Cadidate2 = _rBoundData[i].u4DetectBound;
    }
}

static void ResetCandidateCounter(void)
{
    UINT8 i;
    
    for(i=0; i<LBOX_IDX_MAX; i++)
    {
	_rBoundData[i].u4CadidateCnt1 = 0;
	_rBoundData[i].u4CadidateCnt2 = 0;
    }
}

static void ResetCandidate(void)
{
	ResetCandidateBound();
	ResetCandidateCounter();
}

static void LboxUploadCounter(UINT8 u4DirectIdx, UINT32 u4Bound1Counter, UINT32 u4Bound2Counter, UINT32 u4Bound1, UINT32 u4Bound2, UINT32 u4Draft, UINT32 u4New, UINT32 u4Ori)
{   
	UINT32 match = 0;
	// Upload Counters
    if ((u4Ori == u4Bound1) ||(u4New == u4Bound1))
    {
        // Bound#1
        u4Bound1Counter += LBOX_BOUND_MATCH_WEIGHT;
		match = 1;
    }
    else if(u4Bound1Counter)
    {
    	u4Bound1Counter -= LBOX_BOUND_MISS_WEIGHT;
    }
	
    if ((u4Ori == u4Bound2) ||(u4New == u4Bound2))
    {
        // Bound#2
                u4Bound2Counter += LBOX_BOUND_MATCH_WEIGHT;

		match = 1;
    }
    else if(u4Bound2Counter)
    {
    	u4Bound2Counter -= LBOX_BOUND_MISS_WEIGHT;
    }
	
	if( !match)
	{
	    if (LboxIsBoundStable(u4New, u4Bound1, u4Draft)|| LboxIsBoundStable(u4Ori, u4Bound1, u4Draft))
	    {
	        // Bound#1
	        u4Bound1Counter += LBOX_BOUND_MATCH_WEIGHT;
	    }
	    
	    if (LboxIsBoundStable(u4New, u4Bound2, u4Draft) ||LboxIsBoundStable(u4Ori, u4Bound2, u4Draft))
	    {
	        // Bound#2
	        u4Bound2Counter += LBOX_BOUND_MATCH_WEIGHT;
	    }
	}

    _rBoundData[u4DirectIdx].u4CadidateCnt1 = u4Bound1Counter;
    _rBoundData[u4DirectIdx].u4CadidateCnt2 = u4Bound2Counter;
}


static void LboxBoundUpdate(UINT8 u4DirectIdx, UINT32 u4Bound1Counter, UINT32 u4Bound2Counter, UINT32 u4Bound1, UINT32 u4Bound2, UINT32 u4Final, UINT32 u4Draft, UINT32 u4New, UINT32 u4Small)
{
    UINT32 u4MatchBound1;
    UINT32 u4MatchBound2;

    // If Low = IIR (Low), it means the current value is stable 
    // If Up = IIR (Up), it means the current value is stable 
    // IIR Stable, Upload Bound

    switch (u4DirectIdx)
    {
    	case LBOX_IDX_UP:
              LOG(4, "UP\tBound1:%d\t(%d)\tBound2:%d\t(%d)\t_u4Final:%d\n",  u4Bound1,u4Bound1Counter, u4Bound2, u4Bound2Counter,  u4Final);
    		break;
    	case LBOX_IDX_DOWN:
              LOG(4, "DOWN\tBound1:%d\t(%d)\tBound2:%d\t(%d)\t_u4Final:%d\n",  u4Bound1,u4Bound1Counter, u4Bound2, u4Bound2Counter,  u4Final);
    		break;
    	#if SUPPORT_HORIZONTAL_LBOX  
    	case LBOX_IDX_LEFT:
              LOG(4, "LEFT\tBound1:%d\t(%d)\tBound2:%d\t(%d)\t_u4Final:%d\n",  u4Bound1,u4Bound1Counter, u4Bound2, u4Bound2Counter,  u4Final);
    		break;
    	case LBOX_IDX_RIGHT:
              LOG(4, "RIGHT\tBound1:%d\t(%d)\tBound2:%d\t(%d)\t_u4Final:%d\n",  u4Bound1,u4Bound1Counter, u4Bound2, u4Bound2Counter,  u4Final);
    		break;
    	#endif//SUPPORT_HORIZONTAL_LBOX
	default: //not available
		return;
    }
    
    u4MatchBound1 = 0;
    u4MatchBound2 = 0;
    if (LboxIsBoundStable(u4New, u4Bound1, u4Draft))
    {
        u4MatchBound1 = 1;
    }

    if (LboxIsBoundStable(u4New, u4Bound2, u4Draft))
    {
        u4MatchBound2 = 1;
    }

    if ((u4MatchBound1==1) && (u4MatchBound2==1))// both match
    {
        // new Bound
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
	else if ((u4MatchBound1==0) && (u4MatchBound2==0))// both not match
	{
	    if (u4Bound1Counter > u4Bound2Counter)
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
	else if ((u4MatchBound1==1) && (u4MatchBound2==0))
	{
	    // old Bound #1
	    u4Bound1 = u4New;
	}
	else//if ((u4MatchBound1==0) && (u4MatchBound2==1))
	{
	    // old Bound #2
	    u4Bound2 = u4New;
	}

        // Handle Fast Recovery
        if(u4Small == 0)
        {
        	UINT32 u4FastBound, u4FastCount;

        	if(u4New == u4Bound1)
        	{
        		u4FastBound = u4Bound1;
        		u4FastCount = u4Bound1Counter;
        	}
        	else
        	{
        		u4FastBound = u4Bound2;
        		u4FastCount = u4Bound2Counter;
        	}
        	
	    if((u4DirectIdx==LBOX_IDX_UP)
	    	#if SUPPORT_HORIZONTAL_LBOX  
	    	||(u4DirectIdx==LBOX_IDX_LEFT)
	    	#endif
	    	)
	    {
		        if (((u4FastBound + LETTER_BOX_FAST_RECOVERY_BOUND) < u4Final) &&
		            (u4FastCount > LETTER_BOX_SMALL_BOUND_MIN))
		        {
		            _u4LBoxFastRecovery = 1;
		        }
    		}
	    else // down or right
	    {
		        if (((u4FastBound - LETTER_BOX_FAST_RECOVERY_BOUND) > u4Final) &&
		            (u4FastCount > LETTER_BOX_SMALL_BOUND_MIN))
		        {
		            _u4LBoxFastRecovery = 1;
		        }
	    }
        }
        
        _rBoundData[u4DirectIdx].u4CadidateCnt1 = u4Bound1Counter;
        _rBoundData[u4DirectIdx].u4CadidateCnt2 = u4Bound2Counter;
        _rBoundData[u4DirectIdx].u4Cadidate1 = u4Bound1;
        _rBoundData[u4DirectIdx].u4Cadidate2 = u4Bound2;
//        _rBoundData[u4DirectIdx].u4FinalBound  = u4Final;

}

////-----------------Caption Monitor---------------------
static void ResetCaptionMonitor(void)
{
	UINT8 i,j;
	for(i=0; i<2;i ++)
		for(j=0; j<LBOX_CAPTION_MONITOR_DEPTH;j++)
	{
		rCaptionHistory[i][j]=0;
	}
}

UINT8 u1LboxGetCaptionMonitorResult(UINT8 u4DirectIdx)
{
	UINT8 i;
	
	if(u4DirectIdx >=2)
		return 0;  // work only for top and down side

	for(i=0; i<LBOX_CAPTION_MONITOR_DEPTH; i++)
	{
		if(rCaptionHistory[u4DirectIdx][i])
		{
			LOG(3, "LboxGetCaptionMonitorResult: TRUE (%d, %d, %d)\n", rCaptionHistory[u4DirectIdx][0], rCaptionHistory[u4DirectIdx][1], rCaptionHistory[u4DirectIdx][2]);
			return 1;
		}
	}

	LOG(3, "LboxGetCaptionMonitorResult: FALSE (%d)\n");
	return 0;
}

static void LboxUpdateCaptionMonitor(UINT8 u4DirectIdx, UINT8 u1IsCaption)
{
	static UINT8 u1Idx[2]={0,0};
	
	if(u4DirectIdx < 2)
	{
		LOG(1, "LboxUpdateCaptionMonitor: Dir(%d), Idx(%d), Caption(%d) \n", u4DirectIdx, u1Idx[u4DirectIdx], u1IsCaption);
		rCaptionHistory[u4DirectIdx][u1Idx[u4DirectIdx]]=u1IsCaption;
		u1Idx[u4DirectIdx] = (u1Idx[u4DirectIdx]+1)%LBOX_CAPTION_MONITOR_DEPTH;
	}
}

static void LboxBoundSelect(UINT8 u4DirectIdx, UINT32 u4Bound1Counter, UINT32 u4Bound2Counter, UINT32 u4Bound1, UINT32 u4Bound2, UINT32 *u4Final)
{
	UINT32 u4SmallBoundNs;
	u4SmallBoundNs = 0;
   
	// choose upper bound
	if (_u4LBoxFastRecovery != 0)
	{
		// choose upper bound
		if ((u4Bound1Counter == 0) && (u4Bound2Counter == 0))
		{
			// do nothing  *u4Final = _u4LBoxUpCurrent;
		}
		else if (u4Bound1Counter == 0)
		{
			// Bound#1 NOT detected! Use Bound#2
			*u4Final = u4Bound2;
		}
		else if (u4Bound2Counter == 0)
		{
			// Bound#2 NOT detected! Use Bound#1
			*u4Final = u4Bound1;
		}
		else
		{
			// Two Bound detected! Caption/Subtitle Case
			// Pick envelop of Bound#1 & Bound#2

			if ((u4DirectIdx==LBOX_IDX_UP)
		           #if SUPPORT_HORIZONTAL_LBOX  
    	                        ||(u4DirectIdx==LBOX_IDX_LEFT)
    	                  #endif
    	              )
			{
				if (u4Bound1 > u4Bound2)
				{
					*u4Final = u4Bound2;
				}
				else
				{
					*u4Final = u4Bound1;
				}
			}
			else // u4DirectIdx: Low,right
			{
				if (u4Bound1 > u4Bound2)
				{
					*u4Final = u4Bound1;
				}
				else
				{
					*u4Final = u4Bound2;
				}
			}			 
		}
	}
	else
	{
		u4SmallBoundNs = (u4Bound1Counter + u4Bound2Counter);//mtk70767 : multiplied by 10 ,if one counter is smaller than 10 ,the other is 0 ,there would be error !	
		if ((u4Bound1Counter == 0) && (u4Bound2Counter == 0))
		{
			// do nothing  *u4Final = _u4LBoxUpCurrent;
		}
		else if (((u4Bound1Counter * LBOX_CAPTION_BOUND ) < u4SmallBoundNs) && ((u4Bound2Counter * LBOX_CAPTION_BOUND)> u4SmallBoundNs))
		{
			// Bound#1 NOT detected! Use Bound#2			
			*u4Final = u4Bound2;
			LboxUpdateCaptionMonitor(u4DirectIdx, 0);  // not caption
		}
		else if (((u4Bound2Counter * LBOX_CAPTION_BOUND) < u4SmallBoundNs) && ((u4Bound1Counter * LBOX_CAPTION_BOUND ) > u4SmallBoundNs))
		{
			// Bound#2 NOT detected! Use Bound#1			
			*u4Final = u4Bound1;
			LboxUpdateCaptionMonitor(u4DirectIdx, 0);  // not caption
		}
		else
		{
			LboxUpdateCaptionMonitor(u4DirectIdx, 1);  //  caption
			
			if (u4DirectIdx==LBOX_IDX_UP)
			{
				// Two Bound detected! Caption/Subtitle Case
				// Pick envelop of Bound#1 & Bound#2
				LOG(4, "[LBOX_IDX_UP] Two Up Bound! Caption/Subtitle B1:%d (%d), B2:%d (%d)\n", u4Bound1, u4Bound1Counter, u4Bound2, u4Bound2Counter);
			}
			else if(u4DirectIdx==LBOX_IDX_DOWN)
			{
				// Two Bound detected! Caption/Subtitle Case
				// Pick envelop of Bound#1 & Bound#2
				LOG(4, "[LBOX_IDX_DOWN] Two low Bound ! Caption/Subtitle B1:%d (%d), B2:%d (%d)\n", u4Bound1, u4Bound1Counter, u4Bound2, u4Bound2Counter);
			}
			
			if ((u4DirectIdx==LBOX_IDX_UP) 
		           #if SUPPORT_HORIZONTAL_LBOX  
    	                        ||(u4DirectIdx==LBOX_IDX_LEFT)
    	                  #endif
    	              )
			{
				if (u4Bound1 > u4Bound2)
				{
					*u4Final = u4Bound2;
				}
				else
				{
					*u4Final = u4Bound1;
				}
			}
			else // fgIsDirect:2 -> Low,rigth
			{
				if (u4Bound1 > u4Bound2)
				{
					*u4Final = u4Bound1;
				}
				else
				{
					*u4Final = u4Bound2;
				}
			}			 
		}
	}
}


static UINT32 LboxIsBoundStable(UINT32 u4Input, UINT32 u4Bound, UINT32 u4Drift)
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
#if SUPPORT_LUMACHROMA_CHECK

#if SUPPORT_HORIZONTAL_LBOX
static void LboxCheckHorizontalChroma(UINT32 _u4LBoxLeftCurrent,UINT32 _u4LBoxRightCurrent)
{
    
    UINT32 u4UpBound, u4LowBound;
    UINT32 u4DiffLine, u4ToneThresh;
    UINT32 u4ULeft, u4URight, u4VLeft, u4VRight;
    
    // Set Tone detect bound based on Y detected bound.
    vLBoxSetToneDetectionRange(_rBoundData[LBOX_IDX_LEFT].u4FinalBound - TONE_DETECT_RANGE, 
    _rBoundData[LBOX_IDX_LEFT].u4FinalBound, 
    _rBoundData[LBOX_IDX_RIGHT].u4FinalBound + TONE_DETECT_RANGE, 
    _rBoundData[LBOX_IDX_RIGHT].u4FinalBound);

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
        _rBoundData[LBOX_IDX_LEFT].u4FinalBound = _u4LBoxLeftCurrent;
    }
    
    if ((u4URight <= u4ToneThresh) && (u4VRight <= u4ToneThresh))
    {
        // colorful, do not clip
        _rBoundData[LBOX_IDX_RIGHT].u4FinalBound = _u4LBoxRightCurrent;
    }    
}
#endif


static void LboxCheckVerticalChroma(UINT32 _u4LBoxUpCurrent,UINT32 _u4LBoxLowCurrent)
{
    UINT32 u4DiffWidth, u4ToneThresh;
    UINT32 u4ULeft, u4VLeft;
    UINT32 u4Range, u4Upbound, u4Lowbound;
	//tt add
    #if !SUPPORT_HORIZONTAL_LBOX
    UINT32 _u4LBoxSrcWidth = (UINT32)u4ScpipGetPreScaleWidth(SV_VP_MAIN);	
	VDP_PIC_OVERSCAN_REGION_T _rLBoxOverScan ;
	vLBoxGetInputOverscanSetting(&_rLBoxOverScan);
	#endif
    // Set Tone detect bound based on Y detected bound.
    #if SUPPORT_HORIZONTAL_LBOX
    vLBoxSetToneDetectionRange(_rBoundData[LBOX_IDX_LEFT].u4FinalBound, _rBoundData[LBOX_IDX_RIGHT].u4FinalBound, _rBoundData[LBOX_IDX_RIGHT].u4FinalBound, _rBoundData[LBOX_IDX_LEFT].u4FinalBound);
    u4DiffWidth = _rBoundData[LBOX_IDX_RIGHT].u4FinalBound - _rBoundData[LBOX_IDX_LEFT].u4FinalBound;
    #else
    u4ULeft = (_u4LBoxSrcWidth * _rLBoxOverScan.u4Left) / VDP_MAX_REGION_WIDTH;              // left bound
    u4VLeft = _u4LBoxSrcWidth - ((_u4LBoxSrcWidth * _rLBoxOverScan.u4Right) / VDP_MAX_REGION_WIDTH);     // right bound       
    vLBoxSetToneDetectionRange(u4ULeft, u4VLeft, u4VLeft, u4ULeft);
    u4DiffWidth = u4VLeft - u4ULeft;
    #endif

    /* Upper part */
    if (_rBoundData[LBOX_IDX_UP].u4FinalBound > TONE_DETECT_RANGE)
    {
        if ((_rBoundData[LBOX_IDX_UP].u4FinalBound - TONE_DETECT_RANGE) > _rBoundData[LBOX_IDX_UP].u4DetectBound)
        {
            u4Upbound = _rBoundData[LBOX_IDX_UP].u4FinalBound - TONE_DETECT_RANGE;
        }
        else
        {
            u4Upbound = _rBoundData[LBOX_IDX_UP].u4DetectBound;
        }
    }
    else
    {
        u4Upbound = 0;
    }
    if (_rBoundData[LBOX_IDX_UP].u4FinalBound < u4Upbound)
    {
        LOG(3, "Error: _rBoundData[LBOX_IDX_UP].u4FinalBound < u4Upbound\n");
    }
    u4Range = _rBoundData[LBOX_IDX_UP].u4FinalBound -u4Upbound;
    vRegWriteFldAlign(SIDE_LBOX_00, u4Upbound, C_UP_BOUND);
    vRegWriteFldAlign(SIDE_LBOX_00, _rBoundData[LBOX_IDX_UP].u4FinalBound, C_LOW_BOUND);
 
    // u4ToneThresh: number of pixels fall in the range of tone * 80%
    u4ToneThresh = ((u4DiffWidth * u4Range) >> 4) * TONE_DETECT_BOUND / 100;
    x_thread_delay(LETTER_BOX_HARDWARE_DELAY);
    u4ULeft = RegReadFldAlign(SIDE_LBOX_STA_03, STA_U_IN_TONE1_LEFT);
    u4VLeft = RegReadFldAlign(SIDE_LBOX_STA_03, STA_V_IN_TONE1_LEFT);

    if ((u4ULeft <= u4ToneThresh) && (u4VLeft <= u4ToneThresh))
    {
        _rBoundData[LBOX_IDX_UP].u4FinalBound = _u4LBoxUpCurrent;  // colorful, do not clip
    }
    
    /*********************************************************************/
    /* Lower part */
    if ((_rBoundData[LBOX_IDX_DOWN].u4FinalBound + TONE_DETECT_RANGE) > _rBoundData[LBOX_IDX_DOWN].u4FinalBound)
    {
        u4Lowbound = _rBoundData[LBOX_IDX_DOWN].u4FinalBound;
    }
    else
    {
        u4Lowbound = _rBoundData[LBOX_IDX_DOWN].u4FinalBound + TONE_DETECT_RANGE;
    }
    vRegWriteFldAlign(SIDE_LBOX_00, _rBoundData[LBOX_IDX_DOWN].u4FinalBound, C_UP_BOUND);
    vRegWriteFldAlign(SIDE_LBOX_00, u4Lowbound, C_LOW_BOUND);

    if (u4Lowbound < _rBoundData[LBOX_IDX_DOWN].u4FinalBound)
    {
        LOG(3, "Error: u4Lowbound < _rBoundData[LBOX_IDX_DOWN].u4FinalBound\n");
    }
    u4Range = u4Lowbound - _rBoundData[LBOX_IDX_DOWN].u4FinalBound;
    // u4ToneThresh: number of pixels fall in the range of tone * 80%
    u4ToneThresh = ((u4DiffWidth * u4Range) >> 4) * TONE_DETECT_BOUND / 100;
    // wait
    x_thread_delay(LETTER_BOX_HARDWARE_DELAY);
    u4ULeft = RegReadFldAlign(SIDE_LBOX_STA_03, STA_U_IN_TONE1_LEFT);
    u4VLeft = RegReadFldAlign(SIDE_LBOX_STA_03, STA_V_IN_TONE1_LEFT);

    if ((u4ULeft <= u4ToneThresh) && (u4VLeft <= u4ToneThresh))
    {
        _rBoundData[LBOX_IDX_DOWN].u4FinalBound = _u4LBoxLowCurrent;  // colorful, do not clip
    }
}


static void LboxCheckLumaChroma(UINT32 _u4LBoxUpCurrent,UINT32 _u4LBoxLowCurrent
										#if SUPPORT_HORIZONTAL_LBOX 	   
										,UINT32 _u4LBoxLeftCurrent,UINT32 _u4LBoxRightCurrent
										#endif
										)
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
			#if SUPPORT_HORIZONTAL_LBOX
            LboxCheckHorizontalChroma(_u4LBoxLeftCurrent,_u4LBoxRightCurrent);
			#endif
            LboxCheckVerticalChroma(_u4LBoxUpCurrent,_u4LBoxLowCurrent);
        #endif
     
        // Recover left and right bound for Y detection
        vRegWriteFldAlign(SIDE_LBOX_01, u4LeftBound, C_LEFT_BOUND);
        vRegWriteFldAlign(SIDE_LBOX_01, u4RightBound, C_RIGHT_BOUND);
        
    }
}
#endif


static void LboxCheckCaptionVideo(void)
{
    _u4LBoxStableTime = _u4StableTime;
    if (_rBoundData[LBOX_IDX_DOWN].u4FinalBound > (_u4LBoxLowFinalBackup[0] + _u4LboxInvalidDraft1))
    {
        if (LboxIsBoundStable(_rBoundData[LBOX_IDX_DOWN].u4FinalBound, _u4LBoxLowFinalBackup[1], _u4LboxInvalidDraft1) != 0)
        {
            if (LboxIsBoundStable(_u4LBoxLowFinalBackup[0], _u4LBoxLowFinalBackup[2], _u4LboxInvalidDraft1) != 0)
            {
                LOG(4, "Caption Detect Low(%d %d %d %d)\n", 
                    _rBoundData[LBOX_IDX_DOWN].u4FinalBound, 
                    _u4LBoxLowFinalBackup[0], 
                    _u4LBoxLowFinalBackup[1],
                    _u4LBoxLowFinalBackup[2]);

                _u4LBoxStableTime = _u4CaptionStableTime;
            }
        }
    }

    if ((_rBoundData[LBOX_IDX_UP].u4FinalBound + _u4LboxInvalidDraft1) < _u4LBoxUpFinalBackup[0] )
    {
        if (LboxIsBoundStable(_rBoundData[LBOX_IDX_UP].u4FinalBound, _u4LBoxUpFinalBackup[1], _u4LboxInvalidDraft1) != 0)
        {
            if (LboxIsBoundStable(_u4LBoxUpFinalBackup[0], _u4LBoxUpFinalBackup[2], _u4LboxInvalidDraft1) != 0)
            {
                LOG(4, "Caption Detect Up(%d %d %d %d)\n", 
                    _rBoundData[LBOX_IDX_UP].u4FinalBound, 
                    _u4LBoxUpFinalBackup[0], 
                    _u4LBoxUpFinalBackup[1],
                    _u4LBoxUpFinalBackup[2]);

                _u4LBoxStableTime = _u4CaptionStableTime;
            }
        }
    }

    _u4LBoxLowFinalBackup[2] = _u4LBoxLowFinalBackup[1];
    _u4LBoxLowFinalBackup[1] = _u4LBoxLowFinalBackup[0];
    _u4LBoxLowFinalBackup[0] = _rBoundData[LBOX_IDX_DOWN].u4FinalBound;

    _u4LBoxUpFinalBackup[2] = _u4LBoxUpFinalBackup[1];
    _u4LBoxUpFinalBackup[1] = _u4LBoxUpFinalBackup[0];
    _u4LBoxUpFinalBackup[0] = _rBoundData[LBOX_IDX_UP].u4FinalBound;
}



static void LboxCheckStableVideo(void)
{
    if (_u4LBoxStableTime == _u4VeryStableTime)
    {
        _u4LBoxStableTime = _u4StableTime;
    }

    if ((_rBoundData[LBOX_IDX_DOWN].u4FinalBound != _rBoundData[LBOX_IDX_DOWN].u4DetectBound) &&
        (_u4LBoxLowStableBackup[0] != _rBoundData[LBOX_IDX_DOWN].u4DetectBound) &&
        (_u4LBoxLowStableBackup[1] != _rBoundData[LBOX_IDX_DOWN].u4DetectBound) &&
        (_rBoundData[LBOX_IDX_UP].u4FinalBound != _rBoundData[LBOX_IDX_UP].u4DetectBound) &&
        (_u4LBoxUpStableBackup[0] != _rBoundData[LBOX_IDX_UP].u4DetectBound) &&
        (_u4LBoxUpStableBackup[1] != _rBoundData[LBOX_IDX_UP].u4DetectBound))
    {
        if ((LboxIsBoundStable(_rBoundData[LBOX_IDX_DOWN].u4FinalBound, _u4LBoxLowStableBackup[0], _u4LboxInvalidDraft1) != 0) &&
            (LboxIsBoundStable(_rBoundData[LBOX_IDX_DOWN].u4FinalBound, _u4LBoxLowStableBackup[1], _u4LboxInvalidDraft1) != 0) &&
            (LboxIsBoundStable(_rBoundData[LBOX_IDX_UP].u4FinalBound, _u4LBoxUpStableBackup[0], _u4LboxInvalidDraft1) != 0) &&
            (LboxIsBoundStable(_rBoundData[LBOX_IDX_UP].u4FinalBound, _u4LBoxUpStableBackup[1], _u4LboxInvalidDraft1) != 0))
        {
            LOG(4, "Stable Detect(LowFinal:%d _u4LBoxLowStableBackup:%d UpFinal:%d _u4LBoxUpStableBackup:%d)\n", 
                _rBoundData[LBOX_IDX_DOWN].u4FinalBound, 
                _u4LBoxLowStableBackup[0], 
                _rBoundData[LBOX_IDX_UP].u4FinalBound,
                _u4LBoxUpStableBackup[0]);

            if (_u4LBoxStableTime == _u4StableTime)
            {
                _u4LBoxStableTime = _u4VeryStableTime;
            }
        }
    }

    _u4LBoxLowStableBackup[1] = _u4LBoxLowStableBackup[0];
    _u4LBoxLowStableBackup[0] = _rBoundData[LBOX_IDX_DOWN].u4FinalBound;

    _u4LBoxUpStableBackup[1] = _u4LBoxUpStableBackup[0];
    _u4LBoxUpStableBackup[0] = _rBoundData[LBOX_IDX_UP].u4FinalBound;
}

//****************************************************************************
// reconfig
//****************************************************************************

//-----------------------------------------------------------------------------
/** Brief of LboxReconfig.*/
//reconfig letter box  
//-----------------------------------------------------------------------------

static void LboxReconfig()
{
	
        UINT32 u4RightBound;
        UINT32 u4LeftBound;
	//tt add
	UINT32 _u4LBoxSrcWidth ;	
	UINT32 _u4LBoxSrcHeight ;
	UINT8 _u1LBoxSrcInterlace;
	VDP_PIC_OVERSCAN_REGION_T _rLBoxOverScan ;

	_u1LBoxSrcInterlace =	bLBOXIsInputInterlace();
	vLBoxGetInputOverscanSetting(&_rLBoxOverScan);
	vLBoxGetInputResolution(&_u4LBoxSrcWidth, &_u4LBoxSrcHeight);
	
        // set stable threshold for LBD
        if (_u1LBoxSrcInterlace != 0)
        {
            _u4LboxInvalidDraft1 = ((_u4LBoxSrcHeight * LETTER_BOX_IIR_VALID_DRIFT_1) + 199) / 100;    //how many changed it meanse change
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
        
        _rBoundData[LBOX_IDX_UP].u4DetectBound = (_u4LBoxSrcHeight * LETTER_BOX_UP_LOW_BOUND) / 100;
        _rBoundData[LBOX_IDX_DOWN].u4DetectBound = _u4LBoxSrcHeight - _rBoundData[LBOX_IDX_UP].u4DetectBound;
        _rBoundData[LBOX_IDX_UP].u4MaxBound = (_u4LBoxSrcHeight * LETTER_BOX_MAX_BOUND) / 100;
        _rBoundData[LBOX_IDX_DOWN].u4MaxBound = (_u4LBoxSrcHeight * (100 -LETTER_BOX_MAX_BOUND)) / 100;
        u4LeftBound = (_u4LBoxSrcWidth * _rLBoxOverScan.u4Left) / VDP_MAX_REGION_WIDTH;        
        u4RightBound = _u4LBoxSrcWidth - ((_u4LBoxSrcWidth * _rLBoxOverScan.u4Right) / VDP_MAX_REGION_WIDTH);        
        
        LboxResetCaptionVideo();
        LboxResetStableVideo();

        // Update Config Register
        vRegWriteFldAlign(LBOX_00, _rBoundData[LBOX_IDX_UP].u4DetectBound, LBOX_UP_BOUND);
        vRegWriteFldAlign(LBOX_00, _rBoundData[LBOX_IDX_DOWN].u4DetectBound, LBOX_LOW_BOUND);
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
        
        //_rBoundData[LBOX_IDX_LEFT].u4DetectBound = (_u4LBoxSrcWidth * LETTER_BOX_LEFT_RIGHT_BOUND) / 100;
        _rBoundData[LBOX_IDX_LEFT].u4DetectBound = u4LeftBound;
        _rBoundData[LBOX_IDX_RIGHT].u4DetectBound = _u4LBoxSrcWidth - _rBoundData[LBOX_IDX_LEFT].u4DetectBound;
        _u4LBoxSumDiffThreshold = _u4LBoxSrcHeight * LETTER_BOX_SUM_DIFF_BOUND;       
        u4UpBound = (_u4LBoxSrcHeight * _rLBoxOverScan.u4Top) / VDP_MAX_REGION_HEIGHT;
        u4LowBound = _u4LBoxSrcHeight - ((_u4LBoxSrcHeight * _rLBoxOverScan.u4Bottom) / VDP_MAX_REGION_HEIGHT);
        // Update Config Register
        vRegWriteFldAlign(SIDE_LBOX_00, u4UpBound, C_UP_BOUND);
        vRegWriteFldAlign(SIDE_LBOX_00, u4LowBound, C_LOW_BOUND);
        vRegWriteFldAlign(SIDE_LBOX_01, _rBoundData[LBOX_IDX_LEFT].u4DetectBound, C_LEFT_BOUND);
        vRegWriteFldAlign(SIDE_LBOX_01, _rBoundData[LBOX_IDX_RIGHT].u4DetectBound, C_RIGHT_BOUND);
        vRegWriteFldAlign(SIDE_LBOX_00, 0, C_START);
        vRegWriteFldAlign(SIDE_LBOX_01, 0, C_Y_THRES);
	#endif
		
	ResetFinaldata();
	ResetCandidate();
	ResetCaptionMonitor();
        
        _u4LBoxIIRCount = 0;// Reset IIR Filter
        _u4LBoxThresholdCount = LETTER_BOX_APATIVED_THD_REFLASH;    // Reset Adaptive Threshold
	 u1LBoxReconfigflg = 0;		
        //_u4LBoxSourceChangeFixed = 0;
        LOG(3,"LboxReconfig....Done\n");
}




//****************************************************************************
// config threshold
//****************************************************************************


//-----------------------------------------------------------------------------
/** Brief of LboxUpdateThresholdStatus.*/
//update threshold parameter for config threshold us
//-----------------------------------------------------------------------------

static void LboxUpdateThresholdStatus(UINT16 au2Hist[LUMA_HIST_NUM])
{
	    UINT32 u4Sum = 0;
	    UINT32 u4Best = 0;
	    UINT32 u4Idx;
	    for (u4Idx = 0; u4Idx < LUMA_HIST_NUM; u4Idx++)
	    {    
	        u4Sum += au2Hist[u4Idx];

	        if (u4Sum >= LBOX_HISTGRAM_TOO_DARK)
	        {
	            if (u4Idx < LETTER_BOX_APATIVED_THD_TOO_DARK)
	            {
	                // TooDark Case1: 0~31 > 95%
	                ++LBOX_HISTGRAM_DARKCount;
	            }
	                
	            u4Idx = LUMA_HIST_NUM;
	        }
	        else if ((u4Sum >= LBOX_HISTGRAM_FIT) && (u4Best == 0))
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
	                if (u4Sum >= LBOX_HISTGRAM_DARK)
	                {
	                    if (u4Idx == 0)
	                    {
	                        // TooDark Case2:
	                        // Can not distinquish 25% and 50% by force smallest threshold
	                        ++LBOX_HISTGRAM_DARKCount;

	                        u4Idx = LUMA_HIST_NUM;
	                    }

	                    if (u4Target != 0)
	                    {
	                        u4Target--;
	                    }
	                }

	                ++_au4LboxThresholdCount[u4Target];
	            }
	        }
	        else if (((u4Idx + 1) < LUMA_HIST_NUM) && ((au2Hist[u4Idx] > (au2Hist[u4Idx + 1] + LBOX_HISTGRAM_EDGE)) && (u4Best == 0)))
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
	                if (u4Sum >= LBOX_HISTGRAM_DARK)
	                {
	                    if (u4Idx == 0)
	                    {
	                        // Can not distinquish 25% and 50% by force smallest threshold
	                        ++LBOX_HISTGRAM_DARKCount;

	                        u4Idx = LUMA_HIST_NUM;

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
/** Brief of LboxUpdateThreshold.*/
//update letter box luma threshold 
//-----------------------------------------------------------------------------

static void LboxUpdateThreshold(UINT16 au2Hist[LUMA_HIST_NUM])
{	
	UINT32 u4Sum;
	UINT32 u4Best;
	UINT32 u4Idx;
    if (++_u4LBoxThresholdCount >= LETTER_BOX_APATIVED_THD_REFLASH)
    {        
        _u4LBoxThresholdCount = 0;

        // Update Count
        u4Sum = 0;
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
            LBOX_HISTGRAM_DARKCount,
            _au4LboxThresholdCount[0],
            _au4LboxThresholdCount[1],
            _au4LboxThresholdCount[2],
            _au4LboxThresholdCount[3],
            u4Best, u4Sum,  LBOX_HISTGRAM_FIT);

        // Too Dark ?
        //_u4LBoxTooDark = 0;
        if (LBOX_HISTGRAM_DARKCount >= LETTER_BOX_APATIVED_THD_DARK)
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
                //u4LBoxTooDark = 1;
                _u4LBoxTooDark++; 
                LOG(3, "in threshold Too dark!\n");
            }
			else
			{
				_u4LBoxTooDark = 0;
			}
        }
		else
		{
			_u4LBoxTooDark = 0;
		}

        // Clean Threshold Count
        for (u4Idx = 0; u4Idx < LETTER_BOX_APATIVED_THD_NS; u4Idx++)
        {
            _au4LboxThresholdCount[u4Idx] = 0;
        }
        LBOX_HISTGRAM_DARKCount = 0;
    }
}

//****************************************************************************
// get result
//****************************************************************************

//-----------------------------------------------------------------------------
/** Brief of LboxSWIIR. */
//soft ware IIR for detect result
//-----------------------------------------------------------------------------

#if 0
static UINT8 LboxSWIIR(UINT32 u4Low,UINT32 *u4NewLow,UINT32 u4Up,UINT32 *u4NewUp
						#if SUPPORT_HORIZONTAL_LBOX    
						,UINT32 u4RightMax,UINT32 *u4NewRight,UINT32 u4LeftMin,UINT32 *u4NewLeft
						#endif	  
							)
#endif
static UINT8 LboxSWIIR(UINT32 *rBoundBeforeIIR, UINT32* rBoundAfterIIR)
{
    UINT8 i;	

    if((rBoundBeforeIIR==NULL) || (rBoundAfterIIR==NULL))
    	return FALSE; 
    	
    if (_u4LBoxIIRCount == 0)
    {
    	for(i=0; i<LBOX_IDX_MAX; i++)
    	{
    		_rBoundData[i].u4BoundIIR = (rBoundBeforeIIR[i] << LETTER_BOX_IIR_TAP_FRACTION);
    	}
    }
    else
    {
     	for(i=0; i<LBOX_IDX_MAX; i++)
    	{
        _rBoundData[i].u4BoundIIR -= (_rBoundData[i].u4BoundIIR >> LETTER_BOX_IIR_TAP_FRACTION);    // 15/16 = 1 - 1/16
        _rBoundData[i].u4BoundIIR += rBoundBeforeIIR[i] ;                                             // +1/16
    	}
    }

    // take this out ????????????????????????????    
    // IIR stable at least 8 taps
    if (++_u4LBoxIIRCount <= LETTER_BOX_IIR_TAP)
    {
        // update stable time        
        HAL_GetTime(&_rStableTime);
        _rBoundTime = _rStableTime;
		//tt add
		ResetCandidateCounter();
        return FALSE;
    }
    
    _u4LBoxIIRCount = LETTER_BOX_IIR_TAP;

    for(i=0; i<LBOX_IDX_MAX; i++)
    {
    	rBoundAfterIIR[i] = (_rBoundData[i].u4BoundIIR >> LETTER_BOX_IIR_TAP_FRACTION);
    }
    return TRUE;

}

//-----------------------------------------------------------------------------
/** Brief of LboxUploadBoundCounter. */
//update letter box bound counter 
//-----------------------------------------------------------------------------

static void LboxUpdateBoundCounter(UINT32 *rBoundBeforeIIR, UINT32* rBoundAfterIIR)
{
    UINT8 i;
    UINT32 u4IvalidDraft;
    
    if((rBoundBeforeIIR==NULL) || (rBoundAfterIIR==NULL))
    	return ; 

    for(i=0; i<LBOX_IDX_MAX; i++)
    {
    		if((i==LBOX_IDX_UP) ||(i==LBOX_IDX_DOWN))
		{
			u4IvalidDraft = _u4LboxInvalidDraft2;
		}
		else  // left , right
		{
			u4IvalidDraft = _u4LboxInvalidDraft4;
		}

    	// update counter of letter box candidate
    	LboxUploadCounter(i, _rBoundData[i].u4CadidateCnt1, _rBoundData[i].u4CadidateCnt2, _rBoundData[i].u4Cadidate1, _rBoundData[i].u4Cadidate2, u4IvalidDraft, rBoundAfterIIR[i], rBoundBeforeIIR[i]);
    }
    LOG(6, "NEW(%d %d), IIR(%d %d), B1(%d, %d), B2(%d, %d)\n",
        rBoundBeforeIIR[LBOX_IDX_UP], rBoundBeforeIIR[LBOX_IDX_DOWN], rBoundAfterIIR[LBOX_IDX_UP] , rBoundAfterIIR[LBOX_IDX_DOWN], _rBoundData[LBOX_IDX_UP].u4Cadidate1, _rBoundData[LBOX_IDX_DOWN].u4Cadidate1, _rBoundData[LBOX_IDX_UP].u4Cadidate2, _rBoundData[LBOX_IDX_DOWN].u4Cadidate2);
    #if SUPPORT_HORIZONTAL_LBOX  
    LOG(6, "H_NEW(%d %d), IIR(%d %d), B1(%d, %d), B2(%d, %d)\n",
        rBoundBeforeIIR[LBOX_IDX_LEFT], rBoundBeforeIIR[LBOX_IDX_RIGHT], rBoundAfterIIR[LBOX_IDX_LEFT], rBoundAfterIIR[LBOX_IDX_RIGHT] , _rBoundData[LBOX_IDX_LEFT].u4Cadidate1, _rBoundData[LBOX_IDX_RIGHT].u4Cadidate1, _rBoundData[LBOX_IDX_LEFT].u4Cadidate2, _rBoundData[LBOX_IDX_RIGHT].u4Cadidate2);
    #endif		

}
//-----------------------------------------------------------------------------
/** Brief of LboxUploadBound. */
//update letter box bound 
//-----------------------------------------------------------------------------
static void LboxUpdateAllBound(UINT32 *rNewBound, UINT32 u4Small)
{
	UINT8 i;
	UINT32 u4IvalidDraft;
	
	if(rNewBound==NULL)
		return;

	// Reset Timer and Counter
	//_rBoundTime = rCurrentTime;/////////////////////////////////////////////////take this out 
	LOG(4, "------------ LboxUpdateAllBound() -----------------\n");
	for(i=0; i<LBOX_IDX_MAX; i++)
	{
		if((i==LBOX_IDX_UP) ||(i==LBOX_IDX_DOWN))
		{
			u4IvalidDraft = _u4LboxInvalidDraft2;
		}
		else  // left , right
		{
			u4IvalidDraft = _u4LboxInvalidDraft4;
			u4Small =0;
		}
		LboxBoundUpdate(i, _rBoundData[i].u4CadidateCnt1, _rBoundData[i].u4CadidateCnt2, _rBoundData[i].u4Cadidate1, _rBoundData[i].u4Cadidate2, _rBoundData[i].u4FinalBound,  u4IvalidDraft , rNewBound[i], u4Small);
	}

	LOG(5, "BOUND B1(%d, %d, %d, %d), B2(%d, %d, %d, %d)\n",
		_rBoundData[LBOX_IDX_UP].u4Cadidate1, _rBoundData[LBOX_IDX_DOWN].u4Cadidate1, _rBoundData[LBOX_IDX_UP].u4CadidateCnt1, _rBoundData[LBOX_IDX_DOWN].u4CadidateCnt1,
		_rBoundData[LBOX_IDX_UP].u4Cadidate2, _rBoundData[LBOX_IDX_DOWN].u4Cadidate2, _rBoundData[LBOX_IDX_UP].u4CadidateCnt2, _rBoundData[LBOX_IDX_DOWN].u4CadidateCnt2);

#if SUPPORT_HORIZONTAL_LBOX  
	LOG(5, "H_BOUND B1(%d, %d, %d, %d), B2(%d, %d, %d, %d)\n",
		_rBoundData[LBOX_IDX_LEFT].u4Cadidate1, _rBoundData[LBOX_IDX_RIGHT].u4Cadidate1, _rBoundData[LBOX_IDX_LEFT].u4CadidateCnt1, _rBoundData[LBOX_IDX_RIGHT].u4CadidateCnt1,
		_rBoundData[LBOX_IDX_LEFT].u4Cadidate2, _rBoundData[LBOX_IDX_RIGHT].u4Cadidate2, _rBoundData[LBOX_IDX_LEFT].u4CadidateCnt2, _rBoundData[LBOX_IDX_RIGHT].u4CadidateCnt2);
#endif
	if (_u4LBoxFastRecovery != 0)
	{
		LOG(4, "Fast Recovery B1(%d, %d), B2(%d, %d), FINAL(%d %d)\n",
			_rBoundData[LBOX_IDX_UP].u4Cadidate1, _rBoundData[LBOX_IDX_DOWN].u4Cadidate1, _rBoundData[LBOX_IDX_UP].u4Cadidate2, _rBoundData[LBOX_IDX_DOWN].u4Cadidate2, _rBoundData[LBOX_IDX_UP].u4FinalBound, _rBoundData[LBOX_IDX_DOWN].u4FinalBound);
	}
}

//-----------------------------------------------------------------------------
/** Brief of LboxUploadBound. */
//update final date
//-----------------------------------------------------------------------------
static void LboxTakeAction(UINT8 u1Update)
{
#if SUPPORT_HORIZONTAL_LBOX     
	LOG(4, "TIME (%d.%06d), FINAL(T:%d B:%d L:%d R:%d) Update(%d)\n", _rStableTime.u4Seconds, _rStableTime.u4Micros,
	_rBoundData[LBOX_IDX_UP].u4FinalBound, _rBoundData[LBOX_IDX_DOWN].u4FinalBound, _rBoundData[LBOX_IDX_LEFT].u4FinalBound, _rBoundData[LBOX_IDX_RIGHT].u4FinalBound, u1Update);
#else
	LOG(4, "TIME (%d.%06d), FINAL(T:%d B:%d) Update(%d)\n", _rStableTime.u4Seconds, _rStableTime.u4Micros,
	_rBoundData[LBOX_IDX_UP].u4FinalBound, _rBoundData[LBOX_IDX_DOWN].u4FinalBound, u1Update);
#endif            

	if (u1Update != 0)
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

static UINT8 LboxUpdateFinalData( 
		//#if SUPPORT_HORIZONTAL_LBOX  
		//UINT32 u4LeftDiff,UINT32 u4RightDiff,UINT32 u4LeftMax,UINT32 u4LeftMin,UINT32 u4RightMax,UINT32 u4RightMin
		//#endif
	)
{
	UINT8 i;
	UINT8 u1Update = 0;
	UINT32 _u4LBoxBoundCurrent[LBOX_IDX_MAX];

	#if SUPPORT_HORIZONTAL_LBOX
	UINT32 u4LeftMin;
	UINT32 u4LeftMax;
	UINT32 u4RightMin;
	UINT32 u4RightMax;
	UINT32 u4LeftDiff;
	UINT32 u4RightDiff;
	u4LeftMin = RegReadFldAlign(SIDE_LBOX_STA_02, STA_BLACK_LEFT_POS_MIN);
	u4LeftMax = RegReadFldAlign(SIDE_LBOX_STA_01, STA_BLACK_LEFT_POS_MAX);
	u4RightMin = RegReadFldAlign(SIDE_LBOX_STA_02, STA_BLACK_RIGHT_POS_MIN);
	u4RightMax = RegReadFldAlign(SIDE_LBOX_STA_01, STA_BLACK_RIGHT_POS_MAX);
	u4LeftDiff = RegReadFldAlign(SIDE_LBOX_STA_00, STA_SUM_BLACK_LEFT_POS_DIFF);
	u4RightDiff = RegReadFldAlign(SIDE_LBOX_STA_00, STA_SUM_BLACK_RIGHT_POS_DIFF);
	#endif

       for(i=0 ; i<LBOX_IDX_MAX; i++)
       {
       		#if SUPPORT_HORIZONTAL_LBOX
			if((i>LBOX_IDX_DOWN)&&((u4LeftDiff >= _u4LBoxSumDiffThreshold)||(u4RightDiff >= _u4LBoxSumDiffThreshold)))
			{
				break;
			}
			#endif 
	       	_u4LBoxBoundCurrent[i] = _rBoundData[i].u4FinalBound;
			// Select upside letter box from cadidate and store the result to _rBoundData[LBOX_IDX_UP].u4FinalBound
			LboxBoundSelect(i, _rBoundData[i].u4CadidateCnt1,	_rBoundData[i].u4CadidateCnt2, 
							    _rBoundData[i].u4Cadidate1, _rBoundData[i].u4Cadidate2, &_rBoundData[i].u4FinalBound);
       }

	// Content too dark, can not make decision!
	//tt add
	if(	_u4LBoxTooDark >= 2)
	{
		//tt add
		ResetFinaldata();
		LOG(3, " Too dark!	_u4LBoxTooDark = %d\n",_u4LBoxTooDark);
	}
	//_u4LBoxLastTooDark = _u4LBoxTooDark;

	// check if we need to update new letter box position
	LOG(4, "[LBOX_IDX_DOWN] FinalBound: %d Current: %d InvalidDraft1: %d\n", _rBoundData[LBOX_IDX_DOWN].u4FinalBound, _u4LBoxBoundCurrent[LBOX_IDX_DOWN], _u4LboxInvalidDraft1);
	if (LboxIsBoundStable(_rBoundData[LBOX_IDX_DOWN].u4FinalBound, _u4LBoxBoundCurrent[LBOX_IDX_DOWN], _u4LboxInvalidDraft1) == 0)
	{
		// If new detected letter box is NOT inside (Current - draft, Current + draft), we need to update
		u1Update = 1;
		LOG(3, "LBOX Low Update\n");
	}
	#if 0
	if((_rBoundData[LBOX_IDX_DOWN].u4FinalBound == _rBoundData[LBOX_IDX_DOWN].u4Cadidate1 && _rBoundData[LBOX_IDX_DOWN].u4CadidateCnt2 == 0)||(_rBoundData[LBOX_IDX_DOWN].u4FinalBound == _rBoundData[LBOX_IDX_DOWN].u4Cadidate2 && _rBoundData[LBOX_IDX_DOWN].u4CadidateCnt1 == 0))
	{
		u1Update = 1;
		LOG(0, "tt-------LBOX Low Update check\n");
	}
	#endif
	LOG(4, "[LBOX_IDX_UP] FinalBound: %d ,Current: %d ,InvalidDraft1: %d\n", _rBoundData[LBOX_IDX_UP].u4FinalBound, _u4LBoxBoundCurrent[LBOX_IDX_UP], _u4LboxInvalidDraft1);
	if (LboxIsBoundStable(_rBoundData[LBOX_IDX_UP].u4FinalBound, _u4LBoxBoundCurrent[LBOX_IDX_UP], _u4LboxInvalidDraft1) == 0)
	{
		// If New Final NOT inside (Current - 2, Current + 2), we need to update
		u1Update = 1;
		LOG(3, "LBOX Up Update\n");
	}

#if SUPPORT_HORIZONTAL_LBOX            
	if ((u4LeftDiff < _u4LBoxSumDiffThreshold) && (u4RightDiff < _u4LBoxSumDiffThreshold))
	{
		#if 1    //mtk70767    if always update ,remove this
		if (LboxIsBoundStable(u4LeftMax, u4LeftMin, _u4LboxInvalidDraft3) == 0)
		{
			u1Update = 1;
			LOG(3, "LBOX Right Update\n");
			LOG(3,"u4LeftMax = %d u4LeftMin =%d\n",u4LeftMax,u4LeftMin);
			LOG(3,"_u4LboxInvalidDraft3 = %d\n",_u4LboxInvalidDraft3);
		}
		if (LboxIsBoundStable(u4RightMax, u4RightMin, _u4LboxInvalidDraft3) == 0)
		{
			u1Update = 1;
			LOG(3, "LBOX Left Update\n");
			LOG(3,"u4RightMax = %d,u4RightMin =%d\n",u4RightMax,u4RightMin);
			LOG(3,"_u4LboxInvalidDraft3 = %d\n",_u4LboxInvalidDraft3);
		}
		#endif
		if (LboxIsBoundStable(_rBoundData[LBOX_IDX_RIGHT].u4FinalBound, _u4LBoxBoundCurrent[LBOX_IDX_RIGHT], _u4LboxInvalidDraft3) == 0)
		{
			u1Update = 1;
			LOG(3, "LBOX Right Update\n");
			LOG(3,"_rBoundData[LBOX_IDX_RIGHT].u4FinalBound = %d ,_u4LBoxRightCurrent =%d\n",_rBoundData[LBOX_IDX_RIGHT].u4FinalBound,_u4LBoxBoundCurrent[LBOX_IDX_RIGHT]);
			LOG(3,"_u4LboxInvalidDraft3 = %d\n",_u4LboxInvalidDraft3);
		}
		if (LboxIsBoundStable(_rBoundData[LBOX_IDX_LEFT].u4FinalBound, _u4LBoxBoundCurrent[LBOX_IDX_LEFT], _u4LboxInvalidDraft3) == 0)
		{
			u1Update = 1;
			LOG(3, "LBOX Left Update\n");
			LOG(3,"_rBoundData[LBOX_IDX_LEFT].u4FinalBound = %d,_u4LBoxLeftCurrent = %d\n",_rBoundData[LBOX_IDX_LEFT].u4FinalBound,_u4LBoxBoundCurrent[LBOX_IDX_LEFT]);
			LOG(3,"_u4LboxInvalidDraft3 = %d\n",_u4LboxInvalidDraft3);
		}
	}
#endif

	// Check Chroma
	if ((_rBoundData[LBOX_IDX_UP].u4FinalBound == _rBoundData[LBOX_IDX_UP].u4DetectBound) && (_rBoundData[LBOX_IDX_DOWN].u4FinalBound == _rBoundData[LBOX_IDX_DOWN].u4DetectBound)
    #if SUPPORT_HORIZONTAL_LBOX        
		&& (_rBoundData[LBOX_IDX_LEFT].u4FinalBound == _rBoundData[LBOX_IDX_LEFT].u4DetectBound) && (_rBoundData[LBOX_IDX_RIGHT].u4FinalBound == _rBoundData[LBOX_IDX_RIGHT].u4DetectBound)
    #endif                
		)
	{
		// do not need to check
	}
	else
	{
    #if SUPPORT_LUMACHROMA_CHECK
		LboxCheckLumaChroma(_u4LBoxBoundCurrent[LBOX_IDX_UP],_u4LBoxBoundCurrent[LBOX_IDX_DOWN]
#if SUPPORT_HORIZONTAL_LBOX 	   
		,_u4LBoxBoundCurrent[LBOX_IDX_LEFT],_u4LBoxBoundCurrent[LBOX_IDX_RIGHT]
#endif
		);
    #endif
	}

	_u4LBoxDetectionValid = 1;

	// Reset Timer and Counter
	
	//_rStableTime = rCurrentTime;        ///////////////reflash it when get in this function

	//tt add
	ResetCandidateCounter();
	return u1Update;
}



//-----------------------------------------------------------------------------
/** Brief of LboxAdaptiveThreshold.
 */
//-----------------------------------------------------------------------------
static void LboxAdaptiveThreshold(void)
{
    UINT16 au2Hist[LUMA_HIST_NUM];

    //get luma
    u1DrvGetLumaHist(au2Hist, LBOX_HISTGRAM_NORMAL_BASE);
    
    // update luma threshold
    LboxUpdateThreshold(au2Hist);
    
    // update parameters of adaptive threshold
    LboxUpdateThresholdStatus(au2Hist);
}


static void LboxProcessrResult(UINT32 u4YIsSmall, UINT32 *rBeforeIIR, UINT32* rAfterIIR)
{
    //******************finaldate check******************
    UINT8 u1IsFinalUpdate;
    HAL_TIME_T rCurrentTime;
    HAL_TIME_T rDeltaTime;
    
    HAL_GetTime(&rCurrentTime);    
    HAL_GetDeltaTime(&rDeltaTime, &_rStableTime, &rCurrentTime);
    //*************************************
    if ((rDeltaTime.u4Seconds >= _u4LBoxStableTime) || (_u4LBoxFastRecovery != 0))
    {	
		//reset timer
		_rStableTime = rCurrentTime;
    		//update finaldate
		u1IsFinalUpdate=LboxUpdateFinalData();
		LboxTakeAction(u1IsFinalUpdate);
		_u4LBoxFastRecovery = 0;

		//u1LboxGetCaptionMonitorResult(LBOX_IDX_DOWN);  //Yulia test
	}
    
	//******************update candidate******************    	
    HAL_GetDeltaTime(&rDeltaTime, &_rBoundTime, &rCurrentTime);
	// Determine stable or NOT
    if ((rAfterIIR[LBOX_IDX_UP] == rBeforeIIR[LBOX_IDX_UP]) && (rAfterIIR[LBOX_IDX_DOWN] == rBeforeIIR[LBOX_IDX_DOWN]) && 
        #if SUPPORT_HORIZONTAL_LBOX  
        (rAfterIIR[LBOX_IDX_LEFT] == rBeforeIIR[LBOX_IDX_LEFT]) && (rAfterIIR[LBOX_IDX_RIGHT] == rBeforeIIR[LBOX_IDX_RIGHT]) &&
        #endif        
        (rDeltaTime.u4Seconds >= LETTER_BOX_BOUND_TIME))            
    {
        // Reset Timer 
        _rBoundTime = rCurrentTime;
		
	//updatebound
	LboxUpdateAllBound(rAfterIIR, u4YIsSmall);
    }
		
    //******************update candidate counter******************
    LboxUpdateBoundCounter(rBeforeIIR, rAfterIIR);

}

//-----------------------------------------------------------------------------
/** Brief of LboxDetectionGetResult.
 */
//-----------------------------------------------------------------------------
static UINT8 LboxDetectionGetResult(UINT32 *u4YIsSmall, UINT32 *rBeforeIIR, UINT32* rAfterIIR)
{
    UINT32 u4Up, u4Low, u4Small;
    #if SUPPORT_HORIZONTAL_LBOX
    UINT32 u4LeftMin;
    //UINT32 u4LeftMax;
   // UINT32 u4RightMin;
    UINT32 u4RightMax;
    //UINT32 u4LeftDiff;
    //UINT32 u4RightDiff;
    #endif

    if((rBeforeIIR==NULL) ||(rAfterIIR==NULL))
        return 0;
    
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

    u4Small = RegReadFldAlign(LBOX_STA_00, STA_LBOX_Y_IS_SMALL);
    *u4YIsSmall = u4Small;
    // Handle Small
    if (u4Small)
    {
        u4Low = _rBoundData[LBOX_IDX_DOWN].u4DetectBound;
        u4Up = _rBoundData[LBOX_IDX_UP].u4DetectBound;
        LOG(4, "Dark Frame Detected\n");
    }
    else
    {
    	// get vertical result
    	u4Low= RegReadFldAlign(LBOX_STA_00, STA_LBOX_Y_LOW_LINE);
    	u4Up= RegReadFldAlign(LBOX_STA_00, STA_LBOX_Y_UP_LINE);
    	
	// Check Max Bound
	if (u4Up  > _rBoundData[LBOX_IDX_UP].u4MaxBound)
	{
	    u4Up = _rBoundData[LBOX_IDX_UP].u4MaxBound;
	}

	if (u4Low < _rBoundData[LBOX_IDX_DOWN].u4MaxBound)
	{
	    u4Low  = _rBoundData[LBOX_IDX_DOWN].u4MaxBound;
	}
    }

    #if SUPPORT_HORIZONTAL_LBOX    
    u4LeftMin = RegReadFldAlign(SIDE_LBOX_STA_02, STA_BLACK_LEFT_POS_MIN);
    //u4LeftMax = RegReadFldAlign(SIDE_LBOX_STA_01, STA_BLACK_LEFT_POS_MAX);
    //u4RightMin = RegReadFldAlign(SIDE_LBOX_STA_02, STA_BLACK_RIGHT_POS_MIN);
    u4RightMax = RegReadFldAlign(SIDE_LBOX_STA_01, STA_BLACK_RIGHT_POS_MAX);
    //u4LeftDiff = RegReadFldAlign(SIDE_LBOX_STA_00, STA_SUM_BLACK_LEFT_POS_DIFF);
    //u4RightDiff = RegReadFldAlign(SIDE_LBOX_STA_00, STA_SUM_BLACK_RIGHT_POS_DIFF);
    #endif

    //****************** get soft ware IIR result******************

	rBeforeIIR[LBOX_IDX_DOWN] = u4Low ;
	rBeforeIIR[LBOX_IDX_UP] = u4Up;
	rBeforeIIR[LBOX_IDX_LEFT] = u4LeftMin;
	rBeforeIIR[LBOX_IDX_RIGHT] = u4RightMax;
	
	return LboxSWIIR(rBeforeIIR, rAfterIIR);
    
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

           u1Dispmode = bLBOXIsInputScalerDispmode();

            // normal LBD procedure
            if (bLBOXIsInputValid() && (_u4LBoxDetectionFixed == 0) && (u1Dispmode == 0))
            {            
                UINT32 u4YIsSmall;
                UINT32 rBeforeIIR[LBOX_IDX_MAX], rAfterIIR[LBOX_IDX_MAX];
                // reconfig
                if (u1LBoxReconfigflg != 0)
  		  {
  		      LboxReconfig();
  		  }

                // Handle Adaptive Threshold
                LboxAdaptiveThreshold();

                // measure
                if(LboxDetectionGetResult(&u4YIsSmall, rBeforeIIR, rAfterIIR)==SV_SUCCESS)
                {	
                	//process measured data
			LboxProcessrResult(u4YIsSmall, rBeforeIIR, rAfterIIR);
                }	
            }
            else
            if ((_u4LBoxDetectionFixed != 0) || (u1Dispmode == 1))
            {
                // recover LBD under Dispmode
                if ((_u4LBoxDetectionValid != 0) && (u1PreviousDispmode != u1Dispmode) && (_u4LBoxDetectionFixed == 0))
                {
                		//tt add
			   ResetFinaldata();
                        LboxSetPosSize();
                        _u4LBoxDetectionValid = 0;
                        // Reset IIR Filter
                        _u4LBoxIIRCount = 0;
                }

                // handle input source change case
                
                //tt add                
                if (u1LBoxReconfigflg ==1)
                {
                    // Source Change at Fixed Mode
                    //_u4LBoxSourceChangeFixed = 1;
		      u1LBoxReconfigflg ++;
                    //_u4LBoxSourceChange = 0;
					
					//tt add
                    //remove this ? _u1LBoxSrcInterlace = bLBOXIsInputInterlace();

					//tt add
			ResetFinaldata();   //the final date is right ?
			//tt add
			ResetCandidate();
			ResetCaptionMonitor();
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
            	//tt add
				ResetFinaldata();
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
	
	//tt add
	UINT32 _u4LBoxSrcWidth = (UINT32)u4ScpipGetPreScaleWidth(SV_VP_MAIN);	
	UINT8 _u1LBoxSrcInterlace = bLBOXIsInputInterlace();
	
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
            if ((_rBoundData[LBOX_IDX_DOWN].u4FinalBound > _rBoundData[LBOX_IDX_UP].u4FinalBound)
                #if SUPPORT_HORIZONTAL_LBOX    
                 || (_rBoundData[LBOX_IDX_RIGHT].u4FinalBound > _rBoundData[LBOX_IDX_LEFT].u4FinalBound)
                #endif
                )
            {
                if (_u1LBoxSrcInterlace != 0)
                {
                    u4LBoxY = (_rBoundData[LBOX_IDX_UP].u4FinalBound << 1);
                    u4NewHeight = ((_rBoundData[LBOX_IDX_DOWN].u4FinalBound - _rBoundData[LBOX_IDX_UP].u4FinalBound) << 1);
                }
                else
                {
                    u4LBoxY = _rBoundData[LBOX_IDX_UP].u4FinalBound;
                    u4NewHeight = (_rBoundData[LBOX_IDX_DOWN].u4FinalBound - _rBoundData[LBOX_IDX_UP].u4FinalBound);
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
                u4LBoxX = _rBoundData[LBOX_IDX_LEFT].u4FinalBound;
                u4NewWidth = (_rBoundData[LBOX_IDX_RIGHT].u4FinalBound - _rBoundData[LBOX_IDX_LEFT].u4FinalBound);
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
                LOG(3, "LBOX (X:%d Y:%d Width:%d Height:%d)\n", u4LBoxX, u4LBoxY, u4Width, u4Height);
                #else
                LOG(3, "LBOX (Y:%d Width:%d Height:%d)\n", u4LBoxY, u4Width, u4Height);
                #endif
            }
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
        if (x_thread_create(&hThread1, "LBOX", 4096, 100,
            LboxDetectionLoop, 0, NULL) != OSR_OK)
        {
            ASSERT(0);
        }
    }
}


void LboxSourceChange(void)
{
		u1LBoxReconfigflg = 1;
		_u4LBoxDetectionValid = 0;
}

UINT8 bDrvLboxGetActiveImageArea(UINT16 *x, UINT16 *y, UINT16 *w, UINT16 *h)
{
    if (_u4LBoxDetectionValid != 0)
    {
        *y = _rBoundData[LBOX_IDX_UP].u4FinalBound;
        *h = _rBoundData[LBOX_IDX_DOWN].u4FinalBound - _rBoundData[LBOX_IDX_UP].u4FinalBound;
        #if SUPPORT_HORIZONTAL_LBOX  
        *x = _rBoundData[LBOX_IDX_LEFT].u4FinalBound;
        *w = _rBoundData[LBOX_IDX_RIGHT].u4FinalBound - _rBoundData[LBOX_IDX_LEFT].u4FinalBound;
        #else
        *x = 0; *w = 0;
        #endif
        return 0;
    }
    
    return 1;
}

