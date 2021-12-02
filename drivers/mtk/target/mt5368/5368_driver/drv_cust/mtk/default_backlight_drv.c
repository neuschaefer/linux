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
#include "general.h"
#include "hw_vdoin.h"
#include "util.h"
#include "pe_if.h"
#include "drvcust_if.h"

#define LUMA_BACKLIGHT_CTRL		SV_FALSE	// backlight control initialize: 0: OFF, 1:ON
#define LUMA_MODIFY_COUNT		0x0060	    // 0x1000 = 4096
#define LUMA_LONG_TIME			0x0020	    // 0x0040 = 64,  statistic 64 frame APL

// Global Parameter For Adaptive Backlight Control
UINT8 bBackLightCtrl;
UINT16 wAutoContrastModifyCount;
UINT16 wAutoContrastLongTime;

UINT32 dwSumAPL;
UINT16 wFrameCount;
UINT16 wDoModifyCount;
UINT16 wModifyInterval;

UINT16 wPreValue;
UINT16 wCurValue;
UINT8 bModifyPos;

enum
{
	DYNAMIC_CONTRAST_RATIO_100PERCENT = 0,
	DYNAMIC_CONTRAST_RATIO_150PERCENT,
	DYNAMIC_CONTRAST_RATIO_200PERCENT,
	DYNAMIC_CONTRAST_END
};


typedef struct structBacklightCurve
{
	UINT8 x1;
	UINT8 x2;
	UINT8 x3;
	UINT8 x4;
	UINT8 y0;
	UINT8 y1;
	UINT8 y2;
	UINT8 y3;
	UINT8 y4;
	INT16 m0;
	INT16 m1;
	INT16 m2;
	INT16 m3;
	INT16 m4;
} structBacklightCurve;

structBacklightCurve BacklightCurve;
/*************************************************************************************/
//                  Customized Internal Adaptive Backlight Driver
/*************************************************************************************/
void _AdaptiveBacklightSetCurve(UINT8 y0, UINT8 x1, UINT8 y1, UINT8 x2, UINT8 y2,
                         UINT8 x3, UINT8 y3, UINT8 x4, UINT8 y4, UINT8 y5)
{
    // initialize
    wFrameCount = wAutoContrastLongTime;
    dwSumAPL = 0;
    wDoModifyCount = wAutoContrastLongTime;

    // boundery protection
    if (x1 > x2)
    {
        x1 = x2;
    }
    if (x2 > x3)
    {
        x2 = x3;
    }
    if (x3 > x4)
    {
        x3 = x4;
    }

    // The segment from ( x0, y0 ) --> ( x1, y1 )
    if (x1 > 0)
    {
        BacklightCurve.m0 =
            (INT16) ((((INT32) y1 - (INT32) y0) << 7) / (INT32) x1);
    }
    else
    {
        BacklightCurve.m0 = 0;
    }

    // The segment from ( x1, y1 ) --> ( x2, y2 )
    if (x2 > x1)
    {
        BacklightCurve.m1 =
            (INT16) ((((INT32) y2 - (INT32) y1) << 7) / ((INT32) x2 -
                                                         (INT32) x1));
    }
    else
    {
        BacklightCurve.m1 = 0;
    }

    // The segment from ( x2, y2 ) --> ( x3, y3 )
    if (x3 > x2)
    {
        BacklightCurve.m2 =
            (INT16) ((((INT32) y3 - (INT32) y2) << 7) / ((INT32) x3 -
                                                         (INT32) x2));
    }
    else
    {
        BacklightCurve.m2 = 0;
    }

    // The segment from ( x3, y3 ) --> ( x4, y4 )
    if (x4 > x3)
    {
        BacklightCurve.m3 =
            (INT16) ((((INT32) y4 - (INT32) y3) << 7) / ((INT32) x4 -
                                                         (INT32) x3));
    }
    else
    {
        BacklightCurve.m3 = 0;
    }

    // The segment from ( x4, y4 ) --> ( x5, y5 )
    if (x4 < 255)
    {
        BacklightCurve.m4 =
            (INT16) ((((INT32) y5 - (INT32) y4) << 7) / ((INT32) 255 -
                                                         (INT32) x4));
    }
    else
    {
        BacklightCurve.m4 = 0;
    }

    BacklightCurve.x1 = x1;
    BacklightCurve.x2 = x2;
    BacklightCurve.x3 = x3;
    BacklightCurve.x4 = x4;
    BacklightCurve.y0 = y0;
    BacklightCurve.y1 = y1;
    BacklightCurve.y2 = y2;
    BacklightCurve.y3 = y3;
    BacklightCurve.y4 = y4;
}
/*************************************************************************************/
//            Customized Adaptive Backlight Driver (Internal Interfaces)
/*************************************************************************************/
void _DRVCUST_DBLProc(void)
{
    INT32 i4BackLightModify = 0;
    INT32 i4TargetValue = 0;
    UINT8 bLongTimeAPL = 0;

    // Pause running adaptive backlight if video is not ready.
    if ((bBackLightCtrl != SV_FALSE)
        && (bDrvVideoSignalStatus(SV_VP_MAIN) == SV_VDO_STABLE) &&  (SRM_IsEpgMode() == FALSE))
    {
        if ((bGetVideoDecType(SV_VP_MAIN) == SV_VD_DVI)
            && (_bDviTiming == MODE_WAIT))
        {
            return;
        }

        if (wFrameCount != 0)
        {
            dwSumAPL += (UINT32) bDrvGetAPL();

            // do modify back light job
            if ((wFrameCount == wDoModifyCount) && (bModifyPos < 2))
            {
                if ((bModifyPos == 1) && (wPreValue < 255))
                {
                    wCurValue = wPreValue + 1;
                    i4BackLightModify -= 1;
                }

                if ((bModifyPos == 0) && (wPreValue > 0))
                {
                    wCurValue = wPreValue - 1;
                    i4BackLightModify += 1;
                }

                //LOG(0, "Dimmer:%d\n", wCurValue);

                // set back light duty
                // vApiSetPanelBright(wCurValue);
                DRVCUST_SetBacklightDuty(VDP_ARG_BACKLT_DBL, wCurValue);
                // recursive
                wPreValue = wCurValue;

                // proceesing finish !
                if (i4BackLightModify == 0)
                {
                    bModifyPos = 2;
                }

                // Protect
                if (wDoModifyCount < wModifyInterval)
                {
                    bModifyPos = 2;
                }
                else
                {
                    wDoModifyCount -= wModifyInterval;
                }
            }

            wFrameCount--;
        }
        else
        {
            // Calculate bLongTimeAPL
            bLongTimeAPL =
                (UINT8) (dwSumAPL / (UINT32) wAutoContrastLongTime);

            // Reset to calculate next dwSumAPL
            wFrameCount = wAutoContrastLongTime;
            wDoModifyCount = wAutoContrastLongTime;
            dwSumAPL = 0;

            // Calculate i4TargetValue from bLongTimeAPL
            if (bLongTimeAPL < BacklightCurve.x1)
            {
                i4TargetValue =
                    ((((long) bLongTimeAPL -
                       0) * (long) BacklightCurve.m0) >> 7) +
                    (long) BacklightCurve.y0;
            }
            else if (bLongTimeAPL < BacklightCurve.x2)
            {
                i4TargetValue =
                    ((((long) bLongTimeAPL -
                       (long) BacklightCurve.x1) *
                      (long) BacklightCurve.m1) >> 7) +
                    (long) BacklightCurve.y1;
            }
            else if (bLongTimeAPL < BacklightCurve.x3)
            {
                i4TargetValue =
                    ((((long) bLongTimeAPL -
                       (long) BacklightCurve.x2) *
                      (long) BacklightCurve.m2) >> 7) +
                    (long) BacklightCurve.y2;
            }
            else if (bLongTimeAPL < BacklightCurve.x4)
            {
                i4TargetValue =
                    ((((long) bLongTimeAPL -
                       (long) BacklightCurve.x3) *
                      (long) BacklightCurve.m3) >> 7) +
                    (long) BacklightCurve.y3;
            }
            else
            {
                i4TargetValue =
                    ((((long) bLongTimeAPL -
                       (long) BacklightCurve.x4) *
                      (long) BacklightCurve.m4) >> 7) +
                    (long) BacklightCurve.y4;
            }

            // Calculate i4BackLightModify
            i4BackLightModify += (i4TargetValue - (long) wPreValue);

            // Calculate wModifyCount
            if (i4BackLightModify > 0)
            {
                bModifyPos = 1;
                wModifyInterval =
                    (UINT16) ((INT32) wAutoContrastModifyCount /
                              i4BackLightModify);
            }
            else if (i4BackLightModify < 0)
            {
                bModifyPos = 0;
                wModifyInterval =
                    (UINT16) ((INT32) wAutoContrastModifyCount /
                              (-i4BackLightModify));
            }
            else
            {
                bModifyPos = 2;
                wModifyInterval = 1;
            }

            // avoid wModifyInterval = 0
            if (wModifyInterval < 1)
            {
                wModifyInterval = 1;
            }
        }
    }
}

void _DRVCUST_LightSensorProc(void)
{
// to be defined by customer
}

/*************************************************************************************/
//            Customized Adaptive Backlight Driver (External Interfaces)
/*************************************************************************************/

void DRVCUST_AdaptiveBacklightInit(void)
{
    // ------------------------------------------------------------------------------------------------------------
    // this function defines the initialization of backlight algorithm
    // for example : initialize IIR strength, select default mapping table, defines backlight lower/upper bound, etc
    // you could also set initialzed backlight value in this function, in case no value in initialization
    // ------------------------------------------------------------------------------------------------------------    
    bBackLightCtrl = LUMA_BACKLIGHT_CTRL;
    wAutoContrastModifyCount = LUMA_MODIFY_COUNT;
    wAutoContrastLongTime = LUMA_LONG_TIME;

  	DRVCUST_SetBacklightDuty(VDP_ARG_BACKLT_DBL, 0xFF);
}

void DRVCUST_AdaptiveBacklightProc(void)
{
    // ------------------------------------------------------------------------------------------------------------
    // this function defines customized algorithm for backlight behavior, which may contain
    // dynamic backlight, light sensor control, etc
    //
    // following listed function are frequently used in these algorithms, you could also find them
    // in drv_meter.c :
    // 
    // UINT8 bDrvGetAPL(void) : returns average picture level
    // UINT8 u1DrvGetLumaHist(UINT16 waHistCur[LUMA_HIST_NUM], UINT16 u2NormBase) : get (normalized) luma histogram
    // UINT8 bDrvGetOutputLumaMax(UINT8 bPath) : returns picture luma maximum value in output stage
    //
    // you should set your algorithm result through DRVCUST_SetBacklightDuty(UINT8 u1RefIndex, UINT16 u2Duty)
    // that you can manipulate final backlight result that may controlled by several backlight algorithms
    //
    // sample code example : 
    //
    // UINT8 u1CurrentAPL = bDrvGetAPL();
    // DRVCUST_SetBacklightDuty(VDP_ARG_BACKLT_DBL, u1CurrentAPL);  // which directly maps backlight from APL        
    // ------------------------------------------------------------------------------------------------------------
    
    _DRVCUST_DBLProc();
    _DRVCUST_LightSensorProc();
}

void DRVCUST_AdaptiveBacklightParam(UINT8 bMode)
{
    // ------------------------------------------------------------------------------------------------------------
    // this function defines the behavior when UI parameter changed
    // for example : UI have 3 level (low (0) / mid (1) / high (2))
    // when level changed, backlight mapping changed according to pre-defined behavior
    // ------------------------------------------------------------------------------------------------------------
    bBackLightCtrl = bMode;

    switch (bMode)
    {
    case DYNAMIC_CONTRAST_RATIO_150PERCENT:
        _AdaptiveBacklightSetCurve(bHalGetAdpBacklight(ADP_BACKLIGHT_MIDDLE_IDX),      // y0
                            DRVCUST_OptGet(eAdpBacklightAplDark),       // x1
                            bHalGetAdpBacklight(ADP_BACKLIGHT_MIDDLE_IDX),      // y1
                            DRVCUST_OptGet(eAdpBacklightAplDark),       // x2
                            bHalGetAdpBacklight(ADP_BACKLIGHT_MIDDLE_IDX),      // y2
                            DRVCUST_OptGet(eAdpBacklightAplBright),     // x3
                            bHalGetAdpBacklight(ADP_BACKLIGHT_HIGH_IDX),        // y3
                            DRVCUST_OptGet(eAdpBacklightAplBright),     // x4
                            bHalGetAdpBacklight(ADP_BACKLIGHT_HIGH_IDX),        // y4
                            bHalGetAdpBacklight(ADP_BACKLIGHT_HIGH_IDX) // y5
            );
        break;
    case DYNAMIC_CONTRAST_RATIO_200PERCENT:
        _AdaptiveBacklightSetCurve(bHalGetAdpBacklight(ADP_BACKLIGHT_LOW_IDX), // y0
                            DRVCUST_OptGet(eAdpBacklightAplDark),       // x1
                            bHalGetAdpBacklight(ADP_BACKLIGHT_LOW_IDX), // y1
                            DRVCUST_OptGet(eAdpBacklightAplDark),       // x2
                            bHalGetAdpBacklight(ADP_BACKLIGHT_LOW_IDX), // y2
                            DRVCUST_OptGet(eAdpBacklightAplBright),     // x3
                            bHalGetAdpBacklight(ADP_BACKLIGHT_HIGH_IDX),        // y3
                            DRVCUST_OptGet(eAdpBacklightAplBright),     // x4
                            bHalGetAdpBacklight(ADP_BACKLIGHT_HIGH_IDX),        // y4
                            bHalGetAdpBacklight(ADP_BACKLIGHT_HIGH_IDX) // y5
            );
        break;
    default:
    case DYNAMIC_CONTRAST_RATIO_100PERCENT: // Turn Off Dynamic Backlight
        _AdaptiveBacklightSetCurve(bHalGetAdpBacklight(ADP_BACKLIGHT_HIGH_IDX),        // y0
                            DRVCUST_OptGet(eAdpBacklightAplDark),       // x1
                            bHalGetAdpBacklight(ADP_BACKLIGHT_HIGH_IDX),        // y1
                            DRVCUST_OptGet(eAdpBacklightAplDark),       // x2
                            bHalGetAdpBacklight(ADP_BACKLIGHT_HIGH_IDX),        // y2
                            DRVCUST_OptGet(eAdpBacklightAplBright),     // x3
                            bHalGetAdpBacklight(ADP_BACKLIGHT_HIGH_IDX),        // y3
                            DRVCUST_OptGet(eAdpBacklightAplBright),     // x4
                            bHalGetAdpBacklight(ADP_BACKLIGHT_HIGH_IDX),        // y4
                            bHalGetAdpBacklight(ADP_BACKLIGHT_HIGH_IDX) // y5
            );
        DRVCUST_SetBacklightDuty(VDP_ARG_BACKLT_DBL, 0xFF);        
        break;
    }
}

void DRVCUST_SetBacklightDuty(UINT8 u1RefIndex, UINT16 u2Duty)
{
    // ------------------------------------------------------------------------------------------------------------    
    // Customized backlight interface
    // Could be defined as multiplication of several result :
    // such as (1) adaptive backlight * (2) UI value * (3) light sensor value
    // ------------------------------------------------------------------------------------------------------------    
    static UINT16 u2BacklightSrc[VDP_ARG_BACKLT_MAX];
    UINT16 u2BacklightDuty;

    u2BacklightSrc[u1RefIndex] = u2Duty;

    switch(u1RefIndex)
    {
        case VDP_ARG_BACKLT_UI:
        case VDP_ARG_BACKLT_DBL:
            u2BacklightDuty = (u2BacklightSrc[VDP_ARG_BACKLT_UI] * u2BacklightSrc[VDP_ARG_BACKLT_DBL]) >> 8;
            if(bIsPanelOn() == SV_ON)
            {            
            	vApiSetPanelBright(u2BacklightDuty);
            }
            break;
        case VDP_ARG_BACKLT_LIGHT_SNSR:
            // behavior to be defined by customer
            break;
        case VDP_ARG_BACKLT_OPC:
            // behavior to be defined by customer
            break;            
        case VDP_ARG_BACKLT_SYSTEM_SET:
            u2BacklightDuty = u2BacklightSrc[VDP_ARG_BACKLT_SYSTEM_SET];
            if(bIsPanelOn() == SV_ON)
            {
            	vApiSetPanelBright(u2BacklightDuty);
            }
            break;            
            
    }        
}

/**************************************************************************/
//         D_INTERFACE Adaptive Backlight Param Update
/**************************************************************************/
void DRVCUST_AutoBacklightUpdate(AUTO_BACKLIGHT_SETS_INFO_T *aBLight)
{
    UNUSED(aBLight);
    return;
}

/**************************************************************************/
//         Customized Backlight Driver For Loader Boot Sequence
/**************************************************************************/
void DRVCUST_UpdateBacklightState(UINT8 u1State, UINT16 u2Duty)
{
    UNUSED(u1State);
    UNUSED(u2Duty);
}
