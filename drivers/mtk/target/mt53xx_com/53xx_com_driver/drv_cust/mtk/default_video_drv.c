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
#include "drv_nr.h"
#include "video_def.h"
#include "drv_video.h"
#include "api_backlt.h"
#include "drv_contrast.h"
#include "api_eep.h"
#include "eepdef.h"
#include "drv_tvd.h"
#include "vdo_misc.h"
#include "vdp_drvif.h"
#include "drv_scpos.h"
#include "eeprom_if.h"
#include "u_drv_cust.h"
#include "drvcust_if.h"
#include "drv_hdtv.h"
#include "drv_mpeg.h"
#include "x_debug.h"
#include "x_hal_5381.h"
#include "drv_vdoclk.h"
#include "drv_adcset.h"
#include "drv_autocolor.h"
#include "source_select.h"
#include "drv_pq_cust.h"
#include "srm_drvif.h"

static UINT8 gbSmartPicIndex = 0;
extern UINT8 bBackLightCtrl;

enum
{
	DYNAMIC_CONTRAST_RATIO_100PERCENT = 0,
	DYNAMIC_CONTRAST_RATIO_150PERCENT,
	DYNAMIC_CONTRAST_RATIO_200PERCENT,
	DYNAMIC_CONTRAST_END
};


void DRVCUST_AutoBacklightUpdate(AUTO_BACKLIGHT_SETS_INFO_T *aBLight)
{
    UNUSED(aBLight);
    return;
}

INT8 Clarity_value=0;

void DRVCUST_ClarityUpdate(INT8 u1Value)
{
    UNUSED(u1Value);
    return;
}

UINT16 vApiFlashPqGetSceId(UINT8 bIndex)
{
	UNUSED(bIndex);
	return (0);
}

UINT16 vApiFlashPqGetMaxSceId(UINT8 bIndex)
{
	UNUSED(bIndex);
	return (0);
}
void vApiFlashPqSetSceId(UINT8 bIndex)
{
    return;
}

//Color Temp
///////////////////////////////////////////////////////

void DRVCUST_AutoColorTempUpdate(AUTO_COLORTEMP_SETS_INFO_T *aCTInfo)
{

}
//Gamma.
///////////////////////////////////////////////////////
UINT16 vApiFlashPqGetGammaId(UINT8 bIndex)
{
	UNUSED(bIndex);
    return 0;
}

void DRVCUST_SET_GAMMA(void)
{
}

void vApiFlashPqSetGammaId(UINT8 bIndex)
{
    UNUSED(bIndex);
}

///////////////////////////////////////////////////////

UINT16 vApiFlashPqGetQtyId(UINT8 bIndex)
{
    UNUSED(bIndex);

    if (PANEL_GetPanelWidth() <= 1440)
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

void vApiFlashPqSetSmartPicId(UINT8 bIndex)
{
	gbSmartPicIndex = bIndex;
}

UINT8 vApiFlashPqGetSmartPicId(UINT8 bIndex)
{
	UNUSED(bIndex);
	return (gbSmartPicIndex);
}


/*****************************************************************************************/
/******************************  Dynamic Backlight Control      *********************************/
/*****************************************************************************************/

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

structBacklightCurve BacklightCurve;

void vDrvBacklightCtrlInit(void)
{
    bBackLightCtrl = LUMA_BACKLIGHT_CTRL;
    wAutoContrastModifyCount = LUMA_MODIFY_COUNT;
    wAutoContrastLongTime = LUMA_LONG_TIME;
}

void vDrvBacklightPreSet(UINT8 y0, UINT8 x1, UINT8 y1, UINT8 x2, UINT8 y2,
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

void vDrvBacklightReset(UINT16 wResetValue)
{
    wCurValue = wResetValue;
    wPreValue = wResetValue;
}

void vDrvBacklightCtrl(void)
{
    INT32 i4BackLightModify = 0;
    INT32 i4TargetValue = 0;
    UINT8 bLongTimeAPL = 0;

    // Pause running adaptive backlight if video is not ready.
    if ((bBackLightCtrl == SV_TRUE)
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
                vApiSetPanelBright(wCurValue);

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

void DRVCUST_AdaptiveBacklightMode(UINT8 bMode)
{
    bBackLightCtrl = bMode;
}

void DRVCUST_AdaptiveBacklightParam(UINT8 bMode)
{
    switch (bMode)
    {
    case DYNAMIC_CONTRAST_RATIO_150PERCENT:
        vDrvBacklightPreSet(bHalGetAdpBacklight(ADP_BACKLIGHT_MIDDLE_IDX),      // y0
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
    case 3:
        vDrvBacklightPreSet(bHalGetAdpBacklight(ADP_BACKLIGHT_LOW_IDX), // y0
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
    case DYNAMIC_CONTRAST_RATIO_100PERCENT:
        vDrvBacklightPreSet(bHalGetAdpBacklight(ADP_BACKLIGHT_HIGH_IDX),        // y0
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
        break;
    }
}

void DRVCUST_HwInit(void)
{
    return;
}

void DRVCUST_VideoInit(void)
{
    vDrvBacklightCtrlInit();
    return;
}

void DRVCUST_VdoModeChgDone(UINT8 bPath)
{
    UNUSED(bPath);
    return;
}

void DRVCUST_VideoMainloop()
{
    vDrvBacklightCtrl();
}
void DRVCUST_OutVSyncISR()
{
    return;
}

void DRVCUST_VdoInISR()
{
    return;
}

UINT32 DRVCUST_RGBOfstRemap(UINT32 u4Value)
{
    return u4Value<<2;
}

UINT16 DRVCUST_Y2RMap(void)
{
    //HSI SPEC change , Always -16 For video mode / Special Mode , 
    //because of DEMO Mode Support
    return  0;
}

/* @ Auto NR default Initial setting
 *      1: low
 *      2: Middle
 */
UINT8 DRVCUST_AutoNRDefSet(void)
{
    return 2;
}

/* @ Auto NR noise level implement
 * @ return noise level
 */
UINT8 DRVCUST_NoiseLevel(void)
{
    UINT8 bNoiseLevel = NM_INVALIDLEVEL;
    UINT8 bRFNMLevel;

    if((bGetVideoDecType(bDrvNRGetCurrentPath())==SV_VD_TVD3D)&&
        	 (bGetSignalType(bDrvNRGetCurrentPath())==SV_ST_TV))// TVD RF source
    {

        bRFNMLevel = bTvdSignalStrengthLevel(1);   //RF level 0 ~ 80

        if(bRFNMLevel > 75)
        {
            bNoiseLevel = NM_LEVEL1_3;
        }
        else if(bRFNMLevel > 69)
        {
            bNoiseLevel = NM_LEVEL2;
        }
        else if(bRFNMLevel > 59)
        {
            bNoiseLevel = NM_LEVEL3;
        }
        else if(bRFNMLevel > 55)
        {
            bNoiseLevel = NM_LEVEL4;
        }
        else
        {
            bNoiseLevel = NM_LEVEL5;
        }
    }
    else
    {
        bNoiseLevel = vDrvNMLevel();   //content NR, Noise level 0 ~ 4
    }

    return bNoiseLevel;
}

void DRVCUST_PANEL_GAMMA_REMAP(UINT32 u4GammaSel)
{
    UNUSED(u4GammaSel);
}

#define READ_BASE_SCE_TBL(index1,index2) BASE_SCE_TBL[index1*SCE_HUE_SLICE + index2]
void DRVCUST_CalculateSCE(UINT8 bSceLumaGain, UINT8 bSceSatGain, UINT8 bSceHueGain)
{
    UINT8 i,j;
    INT32 i4Tmp;

    LOG(1, "bSceSatGain = %d\n", bSceSatGain);

    for(j = ENUM_S_GAIN1; j < ENUM_S_GAIN3 + 1; ++j)
    {
        for (i = 0; i < 28; i++)
        {
            i4Tmp =(((INT32)(READ_BASE_SCE_TBL(j, i) - 0x80) * bSceSatGain) >> 7) + 0x80;
            i4Tmp = i4Tmp < 0 ? 0 : i4Tmp;
            i4Tmp = i4Tmp > 0xFF ? 0xFF : i4Tmp;
            WRITE_SCE_TBL(j, i, (UINT8)i4Tmp);
        }
    }
}

void DRVCUST_UpdateBacklightState(UINT8 u1State, UINT16 u2Duty)
{
    UNUSED(u1State);
    UNUSED(u2Duty);
}

void DRVCUST_SetBacklightDuty(UINT16 u2Duty)
{
    vApiSetPanelBright(u2Duty);
}


//======================================================================
//              G A M M A   D E L T A 
//======================================================================
#define CTRL_NUMS 4

#define ENABLE_GAMMA_DELTA          0
#define ENABLE_GAMMA_DELTA_DEBUG    0


#if ENABLE_GAMMA_DELTA

INT16 iCtrlP[CTRL_NUMS] = {16, 48, 112, 176};
INT8 iGmDelta[2][CTRL_NUMS] = {{0,0,0,0},{0,0,0,0}};
UINT16 wGmRef[SV_RGB_RGB][256];
UINT16 wGmGolden[SV_RGB_RGB][256];

void vHalGammaDeltaDebug(void)
{
	Printf("\n\nDelta Red : [%3d] = %4d , [%3d] = %4d, [%3d] = %4d, [%3d] = %4d\n",
		iCtrlP[0], iGmDelta[0][0], 
		iCtrlP[1], iGmDelta[0][1],
		iCtrlP[2], iGmDelta[0][2],
		iCtrlP[3], iGmDelta[0][3]);

	Printf("Delta Blue: [%3d] = %4d , [%3d] = %4d, [%3d] = %4d, [%3d] = %4d\n",
		iCtrlP[0], iGmDelta[1][0], 
		iCtrlP[1], iGmDelta[1][1],
		iCtrlP[2], iGmDelta[1][2],
		iCtrlP[3], iGmDelta[1][3]);
}


UINT8 bHalLoadRefGamma(void)
{
#if ENABLE_GAMMA_DELTA_DEBUG 
    UINT32 i, j;

	Printf("\nLoad Reference gamma\n\n");

	for (j = 0; j < 3; j++)
    {
	    for (i = 0; i < 256; i++)
	    {
	        Printf("%03d ", GAMMA_256_MIDDLE[j][i]);
	        if (i % 16 == 15)
	        {
	            Printf("\n");
	        }
	    }
		Printf("\n\n");
	}
#endif

    return SV_TRUE;
}


UINT16 DRVCUST_GetGammaVal(UINT8 bMode, UINT16 wLevel)
{
	if (wLevel < 256)
	{
		return wGmGolden[bMode][wLevel]	
				-wGmGolden[SV_RGB_G][wLevel]%16;
	}
	else
	{
		return 0;
	}
}


UINT16 DRVCUST_GetCompGammaVal(UINT8 bMode, UINT16 wLevel)
{
	if (wLevel < 256)
	{
		return wGmRef[bMode][wLevel]
				-wGmGolden[SV_RGB_G][wLevel]%16;	
	}
	else
	{
		return 0;
	}
}

void vHalUpdateGammaDelta(void)
{
	INT16 i, j, uIndex;
	INT8 iGmDeltaTbl[2][256];
	UINT8 bPreVal[SV_RGB_RGB] = {0, 0, 0};
	UINT16 wMsb[SV_RGB_RGB] = {0, 0, 0};

	for (uIndex = 0; uIndex < 2; uIndex++)
	{	
		for (i = 0; i < 0x100; i++)
		{
			if (i < iCtrlP[0])
			{
				iGmDeltaTbl[uIndex][i] = (iGmDelta[uIndex][0]*i)/iCtrlP[0];
			}
			else if (i < iCtrlP[1])
			{
				iGmDeltaTbl[uIndex][i] = iGmDelta[uIndex][0]
					+(iGmDelta[uIndex][1]-iGmDelta[uIndex][0])*(i-iCtrlP[0])/(iCtrlP[1]-iCtrlP[0]);
			}
			else if (i < iCtrlP[2])
			{
				iGmDeltaTbl[uIndex][i] = iGmDelta[uIndex][1]
					+(iGmDelta[uIndex][2]-iGmDelta[uIndex][1])*(i-iCtrlP[1])/(iCtrlP[2]-iCtrlP[1]);
			}
			else if (i < iCtrlP[3])
			{
				iGmDeltaTbl[uIndex][i] = iGmDelta[uIndex][2]
					+(iGmDelta[uIndex][3]-iGmDelta[uIndex][2])*(i-iCtrlP[2])/(iCtrlP[3]-iCtrlP[2]);
			}
			else
			{
				iGmDeltaTbl[uIndex][i] = iGmDelta[uIndex][3]
					+(0-iGmDelta[uIndex][3])*(i-iCtrlP[3])/(0x100-iCtrlP[3]);
			}
		}
	}
	
#if ENABLE_GAMMA_DELTA_DEBUG
	Printf("\n\n%s Detal Table\n", uIndex?"Blue":"Red");
	for (i = 0; i < 0x100; i++)
	{
		if ((i%16) == 0)
			Printf("\n[%3d] ", i);

		Printf("%3d  %3d", iGmDeltaTbl[0][i], iGmDeltaTbl[1][i]);
	}
#endif

	bHalLoadRefGamma();
	
	// Rebuild Original Golden Gamma curve
	for (i = 0; i < 0x100; i++)
	{
		for (j = 0; j < SV_RGB_RGB; j++)
		{
			if (bPreVal[j] > GAMMA_256_MIDDLE[j][i])
			{
				wMsb[j]++;
			}
		
			bPreVal[j] = GAMMA_256_MIDDLE[j][i];	
			wGmRef[j][i] = wMsb[j]*256 + GAMMA_256_MIDDLE[j][i];
			wGmGolden[j][i] = wGmRef[j][i];
		}
	}

#if ENABLE_GAMMA_DELTA_DEBUG
	Printf("\n\nOriginal Table\n");

	for (j = 0; j < SV_RGB_RGB; j++)
	{
		for (i = 0; i < 0x100; i++)
		{
			if ((i%16) == 0)
				Printf("\n[%3d] ", i);
			
			Printf("%4d ", wGmRef[j][i]);
		}

		Printf("\n\n");
	}
#endif

	Printf("\n\nDelta Index\n");

	// Buil Golden Gamma curve
	for (i = 0; i < 0x100; i++)
	{
        uIndex = (((wGmRef[SV_RGB_G][i]+8)/16) > 255) ? 255 : (UINT8)((wGmRef[SV_RGB_G][i]+8)/16);

		wGmRef[SV_RGB_R][i] += iGmDeltaTbl[0][uIndex];		
		wGmRef[SV_RGB_B][i] += iGmDeltaTbl[1][uIndex]; 

#if ENABLE_GAMMA_DELTA_DEBUG
		if ((i%16) == 0)
			Printf("\n[%3d] ", i);
			
		Printf("%3d ", (wGmRef[SV_RGB_G][i]+8)/16 );
#endif
	}

#if ENABLE_GAMMA_DELTA_DEBUG
	Printf("\n\nReference Table\n");

	for (j = 0; j < SV_RGB_RGB; j++)
	{
		for (i = 0; i < 0x100; i++)
		{
			if ((i%16) == 0)
				Printf("\n[%3d] ", i);
			
			Printf("%4d ", wGmRef[j][i]);
		}

		Printf("\n\n");
	}
#endif
}

void vHalSetCtrlPoint(INT16 iP1, INT16 iP2, INT16 iP3, INT16 iP4)
{
	if ((iCtrlP[0] < iCtrlP[1]) && (iCtrlP[1] < iCtrlP[2]) 
		&& (iCtrlP[2] < iCtrlP[3]) && (iCtrlP[0] != 0) && (iCtrlP[3] != 0x0100))
	{
		iCtrlP[0] = iP1;
		iCtrlP[1] = iP2;
		iCtrlP[2] = iP3;
		iCtrlP[3] = iP4;

		vHalGammaDeltaDebug();
		vHalUpdateGammaDelta();
	}
	else
	{
		Printf("Error control point setting\n");
		return;
	}	
}


void DRVCUST_SetGammaLevel(UINT16 wP1, UINT16 wP2, UINT16 wP3, UINT16 wP4)
{
	vHalSetCtrlPoint(wP1/16, wP2/16, wP3/16, wP4/16);
}


void DRVCUST_SetGammaDelta(UINT8 bMode, INT8 iDelta1, INT8 iDelta2, INT8 iDelta3, INT8 iDelta4)
{
	UINT8 uIndex = (bMode == SV_RGB_R) ? 0 : 1;
	
	iGmDelta[uIndex][0] = iDelta1;
	iGmDelta[uIndex][1] = iDelta2;
	iGmDelta[uIndex][2] = iDelta3;
	iGmDelta[uIndex][3] = iDelta4;
	
	vHalGammaDeltaDebug();
	vHalUpdateGammaDelta();
}

void vHalInitGammaDelta(void)
{
	if (bApiEepromReadByte(EEP_GMMMA_DELTA_DONE) == 0x86)
	{
		iCtrlP[0] = bApiEepromReadByte(EEP_GMMMA_CTRL_1);
		iCtrlP[1] = bApiEepromReadByte(EEP_GMMMA_CTRL_2);
		iCtrlP[2] = bApiEepromReadByte(EEP_GMMMA_CTRL_3);
		iCtrlP[3] = bApiEepromReadByte(EEP_GMMMA_CTRL_4);

		iGmDelta[0][0] = (INT8)bApiEepromReadByte(EEP_GMMMA_DELTA_R1)-0x80;
		iGmDelta[0][1] = (INT8)bApiEepromReadByte(EEP_GMMMA_DELTA_R2)-0x80;
		iGmDelta[0][2] = (INT8)bApiEepromReadByte(EEP_GMMMA_DELTA_R3)-0x80;
		iGmDelta[0][3] = (INT8)bApiEepromReadByte(EEP_GMMMA_DELTA_R4)-0x80;
	
		iGmDelta[1][0] = (INT8)bApiEepromReadByte(EEP_GMMMA_DELTA_B1)-0x80;
		iGmDelta[1][1] = (INT8)bApiEepromReadByte(EEP_GMMMA_DELTA_B2)-0x80;
		iGmDelta[1][2] = (INT8)bApiEepromReadByte(EEP_GMMMA_DELTA_B3)-0x80;
		iGmDelta[1][3] = (INT8)bApiEepromReadByte(EEP_GMMMA_DELTA_B4)-0x80;
	}
	
	vHalGammaDeltaDebug();
	vHalUpdateGammaDelta();	
}


void vHalUpdateGammmaTbl(UINT8* pTblR, UINT8* pTblG, UINT8* pTblB)
{
	INT16 i, j;
	UINT16 wMsb = 0, wGmVal;
	UINT16 wGmValR, wGmValB;
	UINT8 bPreVal = 0;

	for (i = 0; i < 0x100; i++)
	{
		if (bPreVal > pTblG[i])
		{
			wMsb++;
		}
		
		bPreVal = pTblG[i];		
		wGmVal = wMsb*256 + pTblG[i];	

		for (j = 0; j < 0x100; j++)
		{
			if (wGmVal < wGmRef[SV_RGB_G][j])
				break;					
		}

		if (j == 0x100)
		{
			wGmValR = wGmRef[SV_RGB_R][255] + (4096 - wGmRef[SV_RGB_R][255])
				*(wGmVal - wGmRef[SV_RGB_G][255])/(4096 - wGmRef[SV_RGB_G][255]);
			wGmValB = wGmRef[SV_RGB_B][255] + (4096- wGmRef[SV_RGB_B][255])
				*(wGmVal - wGmRef[SV_RGB_G][255])/(4096 - wGmRef[SV_RGB_G][255]);
		}
		else if (j > 0)
		{
			if (wGmRef[SV_RGB_G][j] != wGmRef[SV_RGB_G][j-1])
			{
				wGmValR = wGmRef[SV_RGB_R][j-1] + (wGmRef[SV_RGB_R][j] - wGmRef[SV_RGB_R][j-1])
					*(wGmVal - wGmRef[SV_RGB_G][j-1])/(wGmRef[SV_RGB_G][j] - wGmRef[SV_RGB_G][j-1]);
				wGmValB = wGmRef[SV_RGB_B][j-1] + (wGmRef[SV_RGB_B][j] - wGmRef[SV_RGB_B][j-1])
					*(wGmVal - wGmRef[SV_RGB_G][j-1])/(wGmRef[SV_RGB_G][j] - wGmRef[SV_RGB_G][j-1]);
			}
			else
			{
				wGmValR = wGmRef[SV_RGB_R][j];	
				wGmValB = wGmRef[SV_RGB_B][j];
			}
		}
		else
		{
            wGmValR = wGmRef[SV_RGB_R][0];	
            wGmValB = wGmRef[SV_RGB_B][0];
		}

		pTblR[i] = wGmValR%256;
		pTblB[i] = wGmValB%256;

#if ENABLE_GAMMA_DELTA_DEBUG
		if ((i%4) == 0)
			Printf("\n[%3d] ", i);
			
		Printf("(%4d,%4d,%4d) ", wGmValB, wGmVal, wGmValB);
#endif
	}

#if ENABLE_GAMMA_DELTA_DEBUG
	Printf("\n\nAdjust Red Table\n");
	for (i = 0; i < 0x100; i++)
	{
		if ((i%16) == 0)
			Printf("\n[%3d] ", i);
			
		Printf("%4d ", pTblR[i]);
	}

	Printf("\n\nAdjust Green Table\n");
	for (i = 0; i < 0x100; i++)
	{
		if ((i%16) == 0)
			Printf("\n[%3d] ", i);
			
		Printf("%4d ", pTblG[i]);
	}
	
	Printf("\n\nAdjust Blue Table\n");
	for (i = 0; i < 0x100; i++)
	{
		if ((i%16) == 0)
			Printf("\n[%3d] ", i);
			
		Printf("%4d ", pTblB[i]);
	}
#endif
}
#endif


