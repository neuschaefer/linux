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
// === INCLUDE =========================================================================
#include "general.h"
#include "vdo_misc.h"
#include "drv_lcdim.h"
#include "hw_ospe.h"
#include "panel.h"
#include "drv_meter.h"
#include "hw_sw.h"
#include "drv_nr.h"
#include "x_chip_id.h"
#include "drv_contrast.h"
#include "drv_noisemeter.h"
// === DEFINE ===========================================================================
#define C_FORCE_EN Fld(1,16,AC_MSKB2)//[16:16]

typedef enum
{
    E_LCDIM_BLACK_CONF=0,
    E_LCDIM_NM_MOTION_CONF=1,
	E_LCDIM_ITEM_MAX=2,
} LCDIM_QUE_ITEM;

// === EXTERN FUNCTION ====================================================================
EXTERN UINT32 IS_SupportDimming (VOID);

EXTERN UINT32 _u4BlockNumH, _u4BlockNumV;
EXTERN UINT8 *bLcDimInfo;

// === GLOBAL VARIABLE ====================================================================
UINT32 u4DataQue[2][256];
UINT32 u4QueCounter[2] = {0, 0};

UINT8 bMotionPenaltyEn = SV_FALSE;
UINT8 bBlackPenaltyEn = SV_FALSE;
UINT8 bSensitivityCtrlEn = SV_FALSE;
UINT8 bAdapCurveCtrlEn = SV_FALSE;
UINT8 bFreezeMappingCurve = SV_FALSE;
    
INT32 i4ConfidencePenalty = 0;
UINT8 u1MotionPenaltyStep;
UINT8 u1BlackPenaltyStep;

UINT16 u2LcDimAveWMin;
UINT16 u2LcDimAveWMax;

UINT16 u2LcDimCurWMin;
UINT16 u2LcDimCurWMax;

UINT32 u4MotionConf;
UINT32 u4MotionConfFiltered;

UINT32 u4LcDimBlackThd;
UINT8 u1MotionThd;
UINT8 u1APLThd;

UINT32 u4SensSpeedCtrl = 0;

UINT8 u1QueSizeForBlack = 0x20;
UINT8 u1QueSizeForAve = 0x8;

UINT16 u2LcDimAveW;    
UINT16 u2LcDimCurW;

NR_NOISE_METER_RESULT* stNoiseMeter;

// === BODY ============================================================================
void vDrvSetLcDimOnOffInt(UINT8 bOnOff)
{
    // for eFuse check, hide real register from customer
    if (!IS_SupportDimming())
    {
        bOnOff = SV_OFF;
    }

    // when force full dimming, means local dimming function off
    vRegWriteFldAlign(LOCAL_DIM_00, !bOnOff, C_FORCE_EN);
}

// use a fifo queue as temporal filter
UINT32 u4QueAverage(UINT32 u4Value, UINT8 bItem, UINT32 u4QueSize)
{
	UINT32 bi;
	UINT32 u4Average=0;

    if(u4QueSize == 0)
    {
        u4QueSize = 1;
    }

	if (u4QueCounter[bItem] == 0)
	{
		u4QueCounter[bItem]++;
		u4DataQue[bItem][0] = u4Value;
		return u4Value;
	}

	if (u4QueCounter[bItem] < u4QueSize)
	{
		u4QueCounter[bItem]++;
	}
	else
	{
        u4QueCounter[bItem] = u4QueSize;
	}

	for (bi = u4QueCounter[bItem] - 1; bi > 0 ; bi--)
	{
		u4DataQue[bItem][bi] = u4DataQue[bItem][bi - 1];
	}

	// Insert New Item
	u4DataQue[bItem][0] = u4Value;

	// Average Running Que
	for (bi = 0; bi < u4QueCounter[bItem]; bi++)
	{
		u4Average += u4DataQue[bItem][bi];
	}

	u4Average = u4Average / u4QueCounter[bItem];

	return u4Average;
}

void vDrvClearDataQue(void)
{
    UINT8 i;

    for(i = 0; i < E_LCDIM_ITEM_MAX; i++)
    {
    	u4QueCounter[i] = 0;
    }
}

void vDrvAdaptiveLcDimUpdateParam(void)
{    
    bSensitivityCtrlEn =    IO32ReadFldAlign(ADAP_LCDIM_00, ADAP_LD_SENS_CTRL);
    bAdapCurveCtrlEn =      IO32ReadFldAlign(ADAP_LCDIM_00, ADAP_LD_CUR_CTRL);
    bMotionPenaltyEn =      IO32ReadFldAlign(ADAP_LCDIM_00, ADAP_LCDIM_MOTION_PENALTY_EN);
    bBlackPenaltyEn =       IO32ReadFldAlign(ADAP_LCDIM_00, ADAP_LCDIM_BLACK_PENALTY_EN);
    bFreezeMappingCurve =   IO32ReadFldAlign(ADAP_LCDIM_00, ADAP_LD_CURVE_FREEZE);

    u2LcDimAveWMin  =       IO32ReadFldAlign(ADAP_LCDIM_03, LCDIM_AVE_W_MIN);        
    u2LcDimAveWMax  =       IO32ReadFldAlign(ADAP_LCDIM_03, LCDIM_AVE_W_MAX);    

    u2LcDimCurWMin  =       IO32ReadFldAlign(ADAP_LCDIM_03, LCDIM_CUR_W_MIN);            
    u2LcDimCurWMax  =       IO32ReadFldAlign(ADAP_LCDIM_03, LCDIM_CUR_W_MAX);    

    u1MotionThd     =       IO32ReadFldAlign(ADAP_LCDIM_02, LCDIM_NR_MOTION_THD);        
    u1APLThd        =       IO32ReadFldAlign(ADAP_LCDIM_01, LCDIM_APL_THD);        
    u4LcDimBlackThd =       IO32ReadFldAlign(ADAP_LCDIM_01, LCDIM_BLACK_THD);

    u1MotionPenaltyStep =   IO32ReadFldAlign(ADAP_LCDIM_04, LCDIM_MOTION_PENALTY_STEP);    
    u1BlackPenaltyStep =    IO32ReadFldAlign(ADAP_LCDIM_04, LCDIM_BLACK_PENALTY_STEP);
    
    u1QueSizeForBlack =     IO32ReadFldAlign(ADAP_LCDIM_01, LCDIM_QUE_SIZE_BLACK);
    u1QueSizeForAve =       IO32ReadFldAlign(ADAP_LCDIM_01, LCDIM_QUE_SIZE_AVE);

    // update motion meter status
    stNoiseMeter = vDrvNMGetResult();

    u4MotionConf = stNoiseMeter->u2FinalConfidence;
    
    u4MotionConfFiltered = u4QueAverage(u4MotionConf, 
                                E_LCDIM_NM_MOTION_CONF,
                                u1QueSizeForAve);    

    vIO32WriteFldAlign(ADAP_LCDIM_02, stNoiseMeter->u2FinalConfidence, LCDIM_NR_MOTION_CONF); 
}

void vDrvSetAdaptiveLcDimOnOff(UINT8 bOnOff)
{
    vIO32WriteFldAlign(ADAP_LCDIM_00, bOnOff, ADAP_LD_CUR_CTRL);  
}

UINT8 bDrvGetAdaptiveLcDimOnOff(void)
{
    return IO32ReadFldAlign(ADAP_LCDIM_00, ADAP_LD_CUR_CTRL);
}

UINT32 u4DrvLcDimGetBlackConf(void)
{
    UINT32 i;

    UINT32 u4InfoSize = _u4BlockNumH * _u4BlockNumV;
    UINT32 u4BlackConf = 0;
    UINT32 u4CurBlockAve;
    UINT32 u4Ret;

    for(i = 0; i < u4InfoSize; i++)
    {
        u4CurBlockAve = *(bLcDimInfo + (1*u4InfoSize) + i);
        
        if(u4CurBlockAve < u4LcDimBlackThd)
        {
            u4BlackConf += (u4LcDimBlackThd - u4CurBlockAve);
        }
    }    

    // normalize confidence to 0x80
    u4Ret = (u4BlackConf * 0x80) / (u4InfoSize * u4LcDimBlackThd);

    // motion penalty
    if(bMotionPenaltyEn == SV_TRUE)
    {
        if(u4MotionConfFiltered > u1MotionThd)
        {
            i4ConfidencePenalty -= u1MotionPenaltyStep;
        }
        else
        {
            i4ConfidencePenalty += u1MotionPenaltyStep;
        }
        
        if(bBlackPenaltyEn == SV_TRUE)
        {
            if(bDrvGetAPL() <= u1APLThd)
            {
                i4ConfidencePenalty -= u1BlackPenaltyStep;
            }
        }
        
        i4ConfidencePenalty = MAX(MIN(i4ConfidencePenalty, 0x80), 0);

        if((UINT32)i4ConfidencePenalty >= u4Ret)
        {
            u4Ret = 0;
        }
        else
        {
            u4Ret = u4Ret - (UINT32)i4ConfidencePenalty;
        }        
    }
    else
    {
        i4ConfidencePenalty = 0;
    }

    return u4Ret;
}

void vDrvLcDimCreateAdaptiveCurve(UINT8 *u1LcdimCurveDft, UINT8 *u1LcdimCurveStr, UINT32 u4Confidence)
{
    UINT32 i;

    for(i = 0; i < LCDIM_CURVE_NUM; i++)
    {
        u1LcdimCurveDft[i] = (UINT8)(((u4Confidence * (UINT32)u1LcdimCurveStr[i]) + ((0x80 - u4Confidence) * (UINT32)u1LcdimCurveDft[i])) >> 7);
    }
}

void vDrvAdaptiveSensControl(void)
{
    if(bSensitivityCtrlEn == SV_TRUE)
    {
    	if((u4MotionConf > u1MotionThd) && (bDrvGetAPL() < u1APLThd))
	    {
	        u2LcDimCurW = u2LcDimCurWMin;    
	    }

		if((u4MotionConfFiltered > u1MotionThd) && (bDrvGetAPL() < u1APLThd))
		{
			u2LcDimAveW = MAX(MIN(( u2LcDimAveW - 2 ), u2LcDimAveWMax), u2LcDimAveWMin);
		    u2LcDimCurW = MAX(MIN(( u2LcDimCurW - 2 ), u2LcDimCurWMax), u2LcDimCurWMin);	
		}
		else
		{
        	u2LcDimAveW = MAX(MIN(( u2LcDimAveW + 1 ), u2LcDimAveWMax), u2LcDimAveWMin);

        	if((u4SensSpeedCtrl++ % 2) == 0)
        	{
    		    u2LcDimCurW = MAX(MIN(( u2LcDimCurW + 1 ), u2LcDimCurWMax), u2LcDimCurWMin);	        	
        	}
		}        

		vDrvLcDimSetAveWeight(u2LcDimAveW);
        vDrvLcDimSetTPFParam(u2LcDimCurW);    	
    }
}

void vDrvAdaptiveCurveRemapping(UINT8 *u1LcdimCurveDft, UINT8 *u1LcdimCurveStr)
{
    UINT32 u4BlackConfidence = 0;

    if(bAdapCurveCtrlEn == SV_TRUE)
    {        
        vDrvUpdateLcDimContentInfo(1);

        u4BlackConfidence = u4QueAverage(u4DrvLcDimGetBlackConf(), 
                                         E_LCDIM_BLACK_CONF,
                                         u1QueSizeForBlack);

        vDrvLcDimCreateAdaptiveCurve(u1LcdimCurveDft, u1LcdimCurveStr, u4BlackConfidence);

        vIO32WriteFldAlign(ADAP_LCDIM_04, u4BlackConfidence, LCDIM_BLACK_CONFIDENCE);
    }
}

void vDrvAdaptiveLcDim(UINT8 *u1LcdimCurveDft, UINT8 *u1LcdimCurveStr)
{ 
    vDrvAdaptiveLcDimUpdateParam();

    // Adaptive Sensitivity Control
    vDrvAdaptiveSensControl();

    // Adaptive Curve Remapping
    vDrvAdaptiveCurveRemapping(u1LcdimCurveDft, u1LcdimCurveStr);
}

