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
#ifndef CC_MTK_LOADER
#include "drv_contrast.h"
#include "nptv_debug.h"
#include "hw_sw.h"
#include "drv_meter.h"
#include "x_typedef.h"
#include "drv_pq_cust.h"

// === DEFINE =============================================================================
#define AL_LOW_START    (64	 >> 3)//<< AL_HIST_SFT)
#define AL_LOW_END      (256 >> 3)//<< AL_HIST_SFT)
#define	AL_HIGH_START   (256 >> 3)//<< AL_HIST_SFT)
#define	AL_HIGH_END     (416 >> 3)//<< AL_HIST_SFT)
#define	AL_HIST_STEP	(32  >> 5)//<< AL_HIST_SFT)

#define	AL_DBG_ADAPGAIN	    (1<<0)
#define	AL_DBG_BWS	        (1<<1)
#define	AL_DBG_DYN_BS	    (1<<2)
#define	AL_DBG_YGAMMA	    (1<<3)
#define	AL_DBG_SCENE_CHANGE	(1<<6)
#define	AL_DBG_FINAL	    (1<<7)

#define ADL_LOG_CHECK(x)    ((bDbgCur==bDbgCount) && (bDbgIndex & x))
#define ADL_LOG_CHECK2(x)   (bDbgIndex & x)

// from 0 to 32
#define	AL_LOW_BIN      9
#define	AL_HIGH_BIN     9
#define AbsDiff(a, b)   (((a)>(b))?((a)-(b)):((b)-(a)))

// === FUNCTION PROTOTYPE =================================================================
// === EXTERN FUNCTION ====================================================================
// === GLOBAL VARIABLE ====================================================================
UINT8 bALOnOff = 1;
UINT8 bAdapLumaGain = 0x40;
UINT8 bAdapLumaOffset = 0x08;
UINT8 bAdapLumaLimit = 0xC0;

UINT8 bMaxDiffThreshold = 0x0A;
UINT8 bTotalDiffThreshold = 0x30;

UINT8 bBSOnOff = 1;
UINT8 bBSLevel = 4;
UINT8 bBSGain = 0x40;
UINT8 bBSOffset = 0x10;
UINT8 bBSRatio = 0x60;
UINT8 bBSLimit = 0x08;

UINT8 bWSOnOff = 1;
UINT8 bWSLevel = 4;
UINT8 bWSGain = 0x60;
UINT8 bWSOffset = 0x08;
UINT8 bWSRatio = 0x40 ;
UINT8 bWSLimit = 0x00;

UINT8 bBWSKeepMidOnOff;
UINT8 bBWSKeepMidX;
UINT16 wBWSKeepMidY;

UINT8 bDynamicBSRange;
UINT8 bDynamicBSOnOff;
UINT8 bDBSPxlThd1;
UINT8 bDBSPxlThd2;
UINT8 bDBSDistThdH;
UINT8 bDBSDistThdL;

UINT8 bDBSLmtBtm;
UINT8 bDBSLmtTop;
UINT8 bDBSGainBtm;
UINT8 bDBSGainTop;
UINT8 bDBSOfstBtm;
UINT8 bDBSOfstTop;

UINT8 bYGammaDecayL;
UINT8 bYGammaDecayH;
UINT8 bYGammaSel;
UINT8 bDarkSceneBrit;

UINT8 bDBSGain;
UINT8 bDBSOffset;
UINT8 bDBSLimit;

UINT8 bYGammaUI = 0;
UINT8 bForceScenceChange = 0;
UINT8 bADLSceneChange = SV_FALSE;
//////////////////////////////////////////////////////////////////////////////////////////
static UINT8 bDbgIndex;
static UINT8 bDbgCount;
static UINT8 bDbgCur;

static UINT32 bCurGainL;
static UINT32 bCurGainM;
static UINT32 bCurGainH;
static UINT32 bValueL;
static UINT32 bValueH;

static UINT32 u4LumaAPL;
static UINT32 u4LumaAPLFullRange;
static UINT32 dwDistribution;
static UINT32 dwDarkPxlCount;

static UINT16 waTargetCurve[LUMA_CURVE_NUM];
static UINT16 waDynCurve[LUMA_CURVE_NUM];

static void _vDrvADLBuildDefault(UINT16 wCurve[LUMA_CURVE_NUM])
{
	UINT8 bi;

	for (bi = 0; bi < LUMA_CURVE_NUM-1; bi++)
	{
		wCurve[bi] = (UINT16) bi << 5;
	}

	wCurve[LUMA_CURVE_NUM-1] = 1023;
}

static void _vDrvADLParamConstrain(void)
{
	bAdapLumaGain = (bAdapLumaGain > 0x80) ? 0x80 : bAdapLumaGain;
	bAdapLumaOffset = (bAdapLumaOffset > 16) ? 16 : bAdapLumaOffset;

	bBSLevel = (bBSLevel > 8) ? 8 : bBSLevel;
	bBSGain = (bBSGain > 0x80) ? 0x80 : bBSGain;

	bWSLevel = (bWSLevel > 8) ? 8 : bWSLevel;
	bWSGain = (bWSGain > 0x80) ? 0x80 : bWSGain;

	bDynamicBSRange = (bDynamicBSRange < 4) ? 4 : bDynamicBSRange;
    bDBSDistThdH = (bDBSDistThdH <= bDBSDistThdL) ? (bDBSDistThdL + 1) : bDBSDistThdH;    
    bDBSPxlThd2 = (bDBSPxlThd2 <= bDBSPxlThd1) ? (bDBSPxlThd1 + 1) : bDBSPxlThd2;    
}

static void _vDrvADLGetAdaptiveLumaParam(void)
{
    bDbgIndex = IO32ReadFldAlign(ADAP_LUMA_08, ADL_DEBUG_INDEX);    
    bDbgCount = IO32ReadFldAlign(ADAP_LUMA_08, ADL_DEBUG_COUNT);
    bDbgCur = (bDbgCur<bDbgCount) ? (bDbgCur+1) : 0; 
    
    bALOnOff = IO32ReadFldAlign(ADAP_LUMA_04, SWREG_AL_ON_1_OFF_0);
    bBSOnOff = IO32ReadFldAlign(ADAP_LUMA_04, SWREG_BWS_BLACK_ON_1_OFF_0);
    bWSOnOff = IO32ReadFldAlign(ADAP_LUMA_04, SWREG_BWS_WHITE_ON_1_OFF_0);
    
    bAdapLumaGain = IO32ReadFldAlign(ADAP_LUMA_03, SWREG_AL_GAIN);
    bAdapLumaOffset = IO32ReadFldAlign(ADAP_LUMA_03, SWREG_AL_OFFSET);
    bAdapLumaLimit = IO32ReadFldAlign(ADAP_LUMA_03, SWREG_AL_LIMIT);
    
    bBSLevel = IO32ReadFldAlign(ADAP_LUMA_04, SWREG_BWS_BLACK_LEVEL);
    bBSGain = IO32ReadFldAlign(ADAP_LUMA_00, SWREG_BWS_BLACK_GAIN);
    bBSOffset = IO32ReadFldAlign(ADAP_LUMA_00, SWREG_BWS_BLACK_OFFSET);
    bBSRatio = IO32ReadFldAlign(ADAP_LUMA_00, SWREG_BWS_BLACK_RATIO);
    bBSLimit = IO32ReadFldAlign(ADAP_LUMA_00, SWREG_BWS_BLACK_LIMIT);

    bWSLevel = IO32ReadFldAlign(ADAP_LUMA_04, SWREG_BWS_WHITE_LEVEL);  
    bWSGain = IO32ReadFldAlign(ADAP_LUMA_01, SWREG_BWS_WHITE_GAIN);
    bWSOffset = IO32ReadFldAlign(ADAP_LUMA_01, SWREG_BWS_WHITE_OFFSET);
    bWSRatio = IO32ReadFldAlign(ADAP_LUMA_01, SWREG_BWS_WHITE_RATIO);
    bWSLimit = IO32ReadFldAlign(ADAP_LUMA_01, SWREG_BWS_WHITE_LIMIT);    

    bDynamicBSOnOff = IO32ReadFldAlign(ADAP_LUMA_04, SWREG_DYNBS_ONOFF);
    bDynamicBSRange = IO32ReadFldAlign(ADAP_LUMA_04, SWREG_DYNBS_RANGE);
    bDBSPxlThd1 = IO32ReadFldAlign(ADAP_LUMA_02, SWREG_DYNBS_PXL_THD1);  
    bDBSPxlThd2 = IO32ReadFldAlign(ADAP_LUMA_02, SWREG_DYNBS_PXL_THD2);  
    bDBSDistThdL = IO32ReadFldAlign(ADAP_LUMA_02, SWREG_DYNBS_DIST_THDL);  
    bDBSDistThdH = IO32ReadFldAlign(ADAP_LUMA_02, SWREG_DYNBS_DIST_THDH);      
    bDBSLmtBtm = IO32ReadFldAlign(ADAP_LUMA_05, SWREG_DYNBS_LMT_BTM);
    bDBSLmtTop = IO32ReadFldAlign(ADAP_LUMA_05, SWREG_DYNBS_LMT_TOP);
    bDBSGainBtm = IO32ReadFldAlign(ADAP_LUMA_05, SWREG_DYNBS_GAIN_BTM);
    bDBSGainTop = IO32ReadFldAlign(ADAP_LUMA_05, SWREG_DYNBS_GAIN_TOP);
    bDBSOfstBtm = IO32ReadFldAlign(ADAP_LUMA_06, SWREG_DYNBS_OFST_BTM);
    bDBSOfstTop = IO32ReadFldAlign(ADAP_LUMA_06, SWREG_DYNBS_OFST_TOP);

    bYGammaUI = IO32ReadFldAlign(ADAP_LUMA_09, ADL_UI_YGAMMA); 
    bYGammaSel = IO32ReadFldAlign(ADAP_LUMA_0C, Y_GAMMA_SEL);        
    bYGammaDecayH = IO32ReadFldAlign(ADAP_LUMA_0C, Y_GAMMA_DECAY_H);
    bYGammaDecayL = IO32ReadFldAlign(ADAP_LUMA_0C, Y_GAMMA_DECAY_L);
    bDarkSceneBrit = IO32ReadFldAlign(ADAP_LUMA_0C, Y_BRIGHT);

    bBWSKeepMidOnOff = IO32ReadFldAlign(ADAP_LUMA_0D, SWREG_BWS_KEEP_MID_ON_OFF);
    bBWSKeepMidX = IO32ReadFldAlign(ADAP_LUMA_0D, SWREG_BWS_KEEP_MID_X);
    wBWSKeepMidY = (IO32ReadFldAlign(ADAP_LUMA_0D, SWREG_BWS_KEEP_MID_Y)<<2);

    _vDrvADLParamConstrain();
}

static BOOL _bDrvADLDetectSceneChanged(const UINT16 wHistCur[LUMA_HIST_NUM])
{
	UINT32 i, u4MaxDiff = 0, u4TotalDiff = 0, u4Diff = 0, u4AplDiff;
    static UINT32 u4PreAPL;    
    static UINT16 waHistRec[LUMA_HIST_NUM]; 

    u4AplDiff = AbsDiff(u4PreAPL, u4LumaAPL);
       
	for (i = 0; i < LUMA_HIST_NUM; i++)
	{
		u4Diff = AbsDiff(wHistCur[i], waHistRec[i]);
		u4MaxDiff = (u4MaxDiff < u4Diff) ? u4Diff : u4MaxDiff;
		u4TotalDiff += u4Diff;
	}

    // Save previous data
	u4PreAPL = u4LumaAPL; 
	x_memcpy(waHistRec, wHistCur, LUMA_HIST_NUM*sizeof(UINT16));		    

    if (bForceScenceChange)
    {        
        if (ADL_LOG_CHECK2(AL_DBG_SCENE_CHANGE))
        {
            LOG(3, "Force Scene Changed\n");
        }
        
        bForceScenceChange = 0;
		return SV_TRUE;
    }
	else if ((u4MaxDiff > bMaxDiffThreshold) && ((u4TotalDiff >> 2) > bTotalDiffThreshold) && (u4AplDiff > 10))
	{           
        if (ADL_LOG_CHECK2(AL_DBG_SCENE_CHANGE))
        {
            LOG(3, "Scene Changed : MaxDiff = %d, TotalDiff = %d, APLDdiff %d\n", 
                u4MaxDiff, u4TotalDiff, u4AplDiff);
        }
		return SV_TRUE;
	}
	else
	{
		return SV_FALSE;
	}
}

static void _vDrvGetDistribution(const UINT16 * waHistCur)
{
	UINT8 bi;

	for (bi = 0, dwDistribution = 0; bi < LUMA_HIST_NUM; bi++)
	{
		dwDistribution += AbsDiff(bi * 8, u4LumaAPL) * waHistCur[bi];
	}

	// shift 8 bits to make distribution a 8 bits value
	dwDistribution = dwDistribution >> 8;
	vIO32WriteFldAlign(ADAP_LUMA_06, dwDistribution, SWREG_AL_DIST);
}

static void _vDrvGetDrkArea(const UINT16 waHistCur[LUMA_HIST_NUM])
{
	UINT8 bi;
    static UINT32 bPreDarkPixlCnt = 0;

	if (bDynamicBSRange < 32)
	{
		for (bi = 0, dwDarkPxlCount = 0; bi <= bDynamicBSRange; bi++)
		{
			dwDarkPxlCount += waHistCur[bi];
		}
	}

	// quality table use 8 bit resolution, but histogram was normalized to 512,
	// so we need to shift 1 bit.
	dwDarkPxlCount >>= 1;

	if (!bADLSceneChange)
	{
		dwDarkPxlCount = (bPreDarkPixlCnt*7 + dwDarkPxlCount) >> 3;
	}

	bPreDarkPixlCnt = dwDarkPxlCount;
	vIO32WriteFldAlign(ADAP_LUMA_06, dwDarkPxlCount, SWREG_AL_DRK_PXL);
}

static void _vDynamicBlackStretch(const UINT16 waHistCur[LUMA_HIST_NUM])
{    
    UINT8 bDBSLimitStep, bDBSGainStep, bDBSOffsetStep;
    
	bDBSOffset = bBSOffset;
	bDBSGain = bBSGain;

	_vDrvGetDistribution(waHistCur);
	_vDrvGetDrkArea(waHistCur);
    
	if (bDynamicBSOnOff)
	{
		// 1. Dynamic BSLimit
		if (dwDarkPxlCount > bDBSPxlThd2)
		{
			if(bDBSLmtTop > 0x80)
			{
				bDBSLimit = bBSLimit + (bDBSLmtTop - 0x80);
			}
			else
			{
				bDBSLimit = (bBSLimit > (0x80 - bDBSLmtTop)) ? bBSLimit - (0x80 - bDBSLmtTop) : 0;
			}
		}
		else if (dwDarkPxlCount > bDBSPxlThd1)
		{
			// interpolate between top / bottom
			if(bDBSPxlThd2 == bDBSPxlThd1)
			{
    			bDBSLimitStep =  bDBSLmtBtm;    			
            }
            else
            {            
			    bDBSLimitStep = (((bDBSLmtTop - bDBSLmtBtm)*(dwDarkPxlCount - bDBSPxlThd1))/(bDBSPxlThd2 - bDBSPxlThd1)) + bDBSLmtBtm;
            }

			if (bDBSLimitStep > 0x80)
			{
				bDBSLimit = bBSLimit + (bDBSLimitStep - 0x80);
			}
			else
			{
				bDBSLimit = (bBSLimit > (0x80 - bDBSLimitStep) ) ? bBSLimit - (0x80 - bDBSLimitStep) : 0;
			}
		}
		else
		{
			if (bDBSLmtBtm > 0x80)
			{
				bDBSLimit = bBSLimit + (bDBSLmtBtm - 0x80);
			}
			else
			{
				bDBSLimit = (bBSLimit > (0x80 - bDBSLmtBtm) ) ? bBSLimit - (0x80 - bDBSLmtBtm) : 0;
			}
		}

		// 2. Dynamic BSGain / BSOffset
		if (dwDistribution > bDBSDistThdH)
		{
			if (bDBSOfstBtm > 0x80)
			{
				bDBSOffset = bBSOffset + (bDBSOfstBtm - 0x80);
			}
			else
			{
				bDBSOffset = (bBSOffset > (0x80 - bDBSOfstBtm) ) ? bBSOffset - (0x80 - bDBSOfstBtm) : 0;
			}

			bDBSGain = bBSGain + (bDBSGainTop - 0x80);
		}
		else if (dwDistribution < bDBSDistThdL)
		{
			bDBSOffset = bBSOffset + (bDBSOfstTop - 0x80);

			if (bDBSGainBtm > 0x80)
			{
				bDBSGain = bBSGain + (bDBSGainBtm - 0x80);
			}
			else
			{
				bDBSGain = (bBSGain > (0x80 - bDBSGainBtm) ) ? bBSGain - (0x80 - bDBSGainBtm) : 0;
			}
		}
		else
		{
		    if(bDBSDistThdH == bDBSDistThdL)
		    {
    			bDBSOffsetStep =  bDBSOfstBtm;
    			bDBSGainStep   =  bDBSGainBtm;	
            }
            else
            {            
    			bDBSOffsetStep = (((bDBSOfstTop - bDBSOfstBtm)*(dwDistribution - bDBSDistThdL))/(bDBSDistThdH - bDBSDistThdL)) + bDBSOfstBtm;
    			bDBSGainStep = (((bDBSGainTop - bDBSGainBtm)*(dwDistribution- bDBSDistThdL))/(bDBSDistThdH - bDBSDistThdL)) + bDBSGainBtm;
            }

			// interpolate dynamic bs offset
			if (bDBSOffsetStep > 0x80)
			{
				bDBSOffset = bBSOffset+ (bDBSOffsetStep - 0x80);
			}
			else
			{
				bDBSOffset = (bBSOffset > (0x80 - bDBSOffsetStep) ) ? bBSOffset - (0x80 - bDBSOffsetStep) : 0;
			}

			// interpolate dynamic bs gain
			if (bDBSGainStep > 0x80)
			{
				bDBSGain = bBSGain + (bDBSGainStep - 0x80);
			}
			else
			{
				bDBSGain = (bBSGain > (0x80 - bDBSGainStep) ) ? bBSGain - (0x80 - bDBSGainStep) : 0;
			}
		}
	}
    
    if (ADL_LOG_CHECK(AL_DBG_DYN_BS))
    {        
    	LOG(3, "DarkPxl = %d, Distribution = %d\n", dwDarkPxlCount, dwDistribution);        
    	LOG(3, "BSGain =  %d, BSOffset =  %d, BSLimit =  %d\n", bBSGain, bBSOffset, bBSLimit);
    	LOG(3, "DBSGain = %d, DBSOffset = %d, DBSLimit = %d\n", bDBSGain, bDBSOffset, bDBSLimit);
    }
}
// ---------------------------------------------------------------------------------
static void _vDrvADLGetApdaptiveGain(const UINT16 wHistCur[LUMA_HIST_NUM])
{
	UINT32  bi;
	UINT32  bCountL = 0, bCountM = 0, bCountH = 0;
	UINT32  bDiffCur;
	UINT32  bDynGain = (bAdapLumaGain * (255 - u4LumaAPLFullRange)) >> 8;
	UINT32  bGainL = (16 + bAdapLumaOffset) * bDynGain; // need >>7 
	UINT32  bGainM = (8 + bAdapLumaOffset) * bDynGain; 
	UINT32  bGainH = bGainL;
        
	bGainL = (bGainL > (16<<7)) ? (16<<7) : bGainL;
    
	for (bValueL = AL_LOW_START; bValueL < AL_LOW_END; bValueL += AL_HIST_STEP)
	{
		bCountL = 0;

		for (bi = 2; bi < LUMA_HIST_NUM-2; bi++)
		{
			if (wHistCur[bi] < bValueL)
			{
				bCountL++;
			}
		}

		if (bCountL >= AL_LOW_BIN)
		{
			break;
		}
	}

	for (bValueH = AL_HIGH_START; bValueH < AL_HIGH_END; bValueH += AL_HIST_STEP)
	{
		bCountH = 0;

		for (bi = 2; bi < LUMA_HIST_NUM-2; bi++)
		{
			if (wHistCur[bi] >= bValueH)
			{
				bCountH++;
			}
		}

		if (bCountH <= AL_HIGH_BIN)
		{
			break;
		}
	}

	bCountM = LUMA_HIST_NUM - 4 - (bCountH + bCountL);
    
    if (ADL_LOG_CHECK(AL_DBG_ADAPGAIN))
    {
    	LOG(3, "DynGain %d  ValueL %d  ValueH %d \n", bDynGain, bValueL, bValueH);
    }
    
	bDiffCur = AL_LOW_END - bValueL;

	bCurGainL = bGainL;
	bCurGainM = (bGainM * bDiffCur) / (AL_LOW_END - AL_LOW_START);
	bCurGainH = (bGainH * bDiffCur) / (AL_LOW_END - AL_LOW_START);
    
    if (ADL_LOG_CHECK(AL_DBG_ADAPGAIN))
    {
	    LOG(3, "CurGainL %d  CurGainM %d  CurGainH %d \n", bCurGainL, bCurGainM, bCurGainH);
    }
    
	if (bCountL != 0) 
	{
		bCurGainL = ((bCurGainH * bCountH) + (bCurGainM * bCountM) +
		            (bCountL >> 1)) / bCountL;

		if (bCurGainL > bGainL)
		{
            bCurGainM = ((bGainL * bCurGainM) + (bCurGainL >> 1)) / bCurGainL;
            bCurGainL = bGainL;
            
            if ((bGainL * bCountL) > (bCurGainM * bCountM))
            {
                bCurGainH = (bCountH == 0) ? 0 
                            : ((bGainL * bCountL) - (bCurGainM * bCountM) + (bCountH >> 1)) / bCountH ;                    
            }
            else
            {
                bCurGainH = 0;
            }
            
		}
	}
	else
	{
		bCurGainL = 0;
		bCurGainM = 0;
		bCurGainH = 0;
	}
    
    if (ADL_LOG_CHECK(AL_DBG_ADAPGAIN))
    {
	    LOG(3, "CurGainL %d  CurGainM %d  CurGainH  %d \n", bCurGainL, bCurGainM, bCurGainH);         
    	LOG(3, "CountL   %d  CountM   %d  CountH    %d \n", bCountL, bCountM, bCountH);
	    LOG(3, "APL  %d ==> Neg %d   Pos %d \n", u4LumaAPL, bCurGainL*bCountL, bCurGainM*bCountM+bCurGainH*bCountH);
    }
}

// ---------------------------------------------------------------------------------
static void _vDrvADLBuildDynCurve(const UINT16 waHistCur[LUMA_HIST_NUM], UINT16 waDynCurve[LUMA_CURVE_NUM])
{
	UINT32 bi, bGainH, bGainM, bGainL;
    UINT32 u4AccCurve[LUMA_HIST_NUM-4], u4Sum = 0;

	if (waHistCur == NULL)
	{
		return;
	}      
    
	_vDrvADLBuildDefault(waDynCurve);     
	_vDrvADLGetApdaptiveGain(waHistCur);

    if (bALOnOff == SV_TRUE)
    {
        bGainL = (32<<7) - bCurGainL;
        bGainM = (32<<7) + bCurGainM;
        bGainH = (32<<7) + bCurGainH;
	    
        for (bi = 0; bi < (LUMA_HIST_NUM-4); bi++)
	    {
            if (waHistCur[bi+2] < bValueL)
		    {
                u4Sum += bGainL;
		    }
            else if (waHistCur[bi+2] >= bValueH)
		    {
                u4Sum += bGainH;		    
		    }
		    else
		    {
                u4Sum += bGainM;		    
            }
            
            u4AccCurve[bi] = u4Sum;            
        }                
        
        for (bi = 3; bi < (LUMA_HIST_NUM - 2); bi++)
	    {
	        waDynCurve[bi] = 0x40+(u4AccCurve[bi-3]>>7);
	    }
    }
}

static UINT32 _bDrvADLGetBSGain(UINT32 u4APL)
{
    if (u4APL < 0x40)
	{
		return ((bBSRatio * u4APL) / 0x40);
	}
	else if (u4APL < bAdapLumaLimit)
	{
		return (((0xFF - bBSRatio) * (u4APL - 0x40)) / (bAdapLumaLimit - 0x40) + bBSRatio);
	}
	else
	{
		return 0xFF;
	}
}

static UINT32 _bDrvADLGetWSGain(UINT32 u4APL)
{
	if (u4APL < 192)
	{
		return (((255 - bWSRatio) * u4APL) / 192);
	}
	else
	{
		return (((bWSRatio * (u4APL - 192)) >> 6) + (255 - bWSRatio));
	}
}

static UINT16* _u2DrvGetYGamma(UINT8 bIndex)
{    
	switch (bIndex)
	{
        case 1:
        	return GAMMA_950_TABLE;
        case 2:
        	return GAMMA_925_TABLE;
        case 3:
        	return GAMMA_900_TABLE;
        case 4:
        	return GAMMA_875_TABLE;
        case 5:
        	return GAMMA_850_TABLE;
        case 6:
        	return GAMMA_825_TABLE;
        case 7:
            return GAMMA_105_TABLE;
        case 8:
        	return GAMMA_110_TABLE;
        case 9:
        	return GAMMA_115_TABLE;
        default:
            return GAMMA_100_TABLE;        
	}
}

static void _vDrvApplyYGamma(UINT16* waFinalCurve)
{
    UINT32 i,u4Index,u4IndexNxt;	
    UINT32 u4Tmp;
    const UINT16* b2PQYGammaSelTbl;//Y Gamma Select By Quality Item
    const UINT16* b2UIYGammaSelTbl;//Y Gamma Select By Men UI
    UINT16 u2CURRENT_Y_GAMMA_TABLE[257];
    UINT8 bDecay;
    UINT8 bDarkSceneBrightRaise;//Raising the Brightness for Dark Scene
    UINT32 u4RemapedAPL;
    UINT16 u2RemapIndex;
    
    b2PQYGammaSelTbl = _u2DrvGetYGamma(bYGammaSel);

    //Boundary Protection
    bYGammaDecayH = MIN(bYGammaDecayH, 0x80);
    bYGammaDecayL = MIN(bYGammaDecayL, 0x80);

    u4RemapedAPL = u4LumaAPLFullRange;
    if(u4RemapedAPL < 0x40)
    {        
        bDecay = u4RemapedAPL * (0x80 - bYGammaDecayL) / 0x40 + bYGammaDecayL;
        bDarkSceneBrightRaise = (0x40 - u4RemapedAPL) * bDarkSceneBrit / 0x40;        
    }
    else if(u4RemapedAPL > 0xC0)
    {    
        bDecay = (0x100-u4RemapedAPL) * (0x80 - bYGammaDecayH) /(0x100-0xC0) + bYGammaDecayH;
        bDarkSceneBrightRaise = 0;
    }
    else
    {
        bDecay = 0x80;
        bDarkSceneBrightRaise = 0;
    }

    if (ADL_LOG_CHECK(AL_DBG_YGAMMA))
    {        
        LOG(3, "Y Gamma : UI Gamma  %d  PQ   %d \n", bYGammaUI, bYGammaSel);
        LOG(3, "APL %d   Decay %d  BrightRise %d \n", u4LumaAPLFullRange, bDecay, bDarkSceneBrightRaise);
    }

    b2UIYGammaSelTbl = _u2DrvGetYGamma(bYGammaUI);
 
    for (i=0; i<257; ++i)
    {      
        u4Tmp = MIN(((INT32)(b2PQYGammaSelTbl[i])-(INT32)i*4) * bDecay / 0x80 + i*4 + bDarkSceneBrightRaise, 0x400);

        //Original Range Already do Boundary Protection
        u4Index = u4Tmp >> 2;
        u4IndexNxt = u4Index + 1;
        
        u2CURRENT_Y_GAMMA_TABLE[i] = (UINT16)((b2UIYGammaSelTbl[u4Index]*(4-(u4Tmp&3))
                                    +b2UIYGammaSelTbl[u4IndexNxt]*(u4Tmp&3)+2)>>2);
    }

	for (i = 3; i < LUMA_CURVE_NUM-3; i++)
	{
        u2RemapIndex = MIN((waFinalCurve[i]>>2), 0x100);
        waFinalCurve[i] = MIN(u2CURRENT_Y_GAMMA_TABLE[u2RemapIndex], 0x3FF);
	}
}

static void _vDrvADLBuildTargetCurve(const UINT16 waHistCur[LUMA_HIST_NUM], UINT16 waTargetCurve[LUMA_CURVE_NUM])
{
	UINT32 bi;
	UINT32 wDiff;
	UINT32 bStep;
	UINT32 bBsGainMap, bWsGainMap;
	UINT32 bWSAdjustGain, bBSAdjustGain;
	UINT32 bWSAdjustOffset, bBSAdjustOffset;
	UINT32 bBSGainApplied = bBSGain;
	UINT32 bBSOffsetApplied = bBSOffset;
	UINT32 bBSLimitApplied = bBSLimit;	
    UINT32 bEnd;

	_vDrvADLBuildDefault(waTargetCurve);
	_vDynamicBlackStretch(waHistCur);
    
	if (bDynamicBSOnOff)
	{
		bBSGainApplied = bDBSGain;
		bBSOffsetApplied = bDBSOffset;
		bBSLimitApplied = bDBSLimit;
	}

	bBsGainMap = _bDrvADLGetBSGain(u4LumaAPLFullRange);
	bWsGainMap = _bDrvADLGetWSGain(u4LumaAPLFullRange);

	// BSGain
	wDiff = AbsDiff(bBSGainApplied, 0x80 + bBSLimitApplied);
	bBSAdjustGain = bBSGainApplied + ((wDiff * (0xFF - bBsGainMap)) >> 8);

	// WS Gain
	wDiff = AbsDiff(bWSGain, 0x80 + bWSLimit);
	bWSAdjustGain = bWSGain + ((wDiff * bWsGainMap) >> 8);

	bBSAdjustOffset = (bBSOffsetApplied * bBsGainMap) >> 8;
	bWSAdjustOffset = (bWSOffset * (0xFF - bWsGainMap)) >> 8;

    if (ADL_LOG_CHECK(AL_DBG_BWS))
    {
    	LOG(3, "APL %d  bBsGainMap %d  bWsGainMap %d\n", u4LumaAPLFullRange, bBsGainMap, bWsGainMap);
    	LOG(3, "BS Gain %d  Offset %d  \n", bBSAdjustGain, bBSAdjustOffset);
    	LOG(3, "WS Gain %d  Offset %d  \n", bWSAdjustGain, bWSAdjustOffset);
    }

	if (bBSOnOff == SV_TRUE)
	{
        bEnd = MIN(bBSLevel, 8);        
		for (bi = 0; bi < bEnd; bi++)
		{
			bStep = (bBSAdjustGain >> 2);
			if (bStep > bBSAdjustOffset)
			{
				bStep -= bBSAdjustOffset;
				bBSAdjustOffset = 0;
			}
			else
			{
				bBSAdjustOffset -= bStep;
				bStep = 0;
			}

			waTargetCurve[bi+3] = waTargetCurve[bi+2] + bStep;

            if (ADL_LOG_CHECK(AL_DBG_BWS))
            {
                LOG(3, "BS[%d] =  %d  \n", (bi+3), waTargetCurve[bi+3]);
            }
		}
	}

	if (bWSOnOff == SV_TRUE)
	{
	    bEnd = MIN(bWSLevel, 8);
		for (bi = 0; bi < bEnd; bi++)
		{
			bStep = (bWSAdjustGain >> 2);
			if (bStep > bWSAdjustOffset)
			{
				bStep -= bWSAdjustOffset;
				bWSAdjustOffset = 0;
			}
			else
			{
				bWSAdjustOffset -= bStep;
				bStep = 0;
			}

			assert(bi <= 29);
			waTargetCurve[29-bi] = waTargetCurve[30-bi] - bStep;
           
            if (ADL_LOG_CHECK(AL_DBG_BWS))
            {
                LOG(3, "WS[%d] =  %d  \n", (29-bi), waTargetCurve[29-bi]);
            }
		}
	}

    UINT16 wBDiff, wWDiff;

	// Keep BWS Mid Point
	if (bBWSKeepMidOnOff)
	{
	    bi = MIN((2 + bBSLevel), (LUMA_CURVE_NUM-1));
		wBDiff = (wBWSKeepMidY - waTargetCurve[bi]) / (bBWSKeepMidX - bBSLevel);        
        bEnd = MIN((bBWSKeepMidX + 2), (LUMA_CURVE_NUM-1));//Fix klock warning
        
		for (bi = (bBSLevel + 3); bi < bEnd ; bi++)
		{
			waTargetCurve[bi] = waTargetCurve[bi - 1] + wBDiff;
		}

        bEnd = MIN((30 - bWSLevel), 30);//Fix klock warning
		wWDiff = (waTargetCurve[bEnd] - wBWSKeepMidY) / (28 - (bWSLevel + bBWSKeepMidX));
		
		for (bi = bBWSKeepMidX + 3; bi < bEnd; bi++)
		{
			waTargetCurve[bi] = waTargetCurve[bi - 1] + wWDiff;
		}
	}
	else
	{
        bi = MIN((2 + bBSLevel), (LUMA_CURVE_NUM-1));
        bEnd = MIN((30 - bWSLevel), 30);//Fix klock warning		  
        
        wDiff = ((waTargetCurve[bEnd] - waTargetCurve[bi]) + ((28 - (bWSLevel + bBSLevel)) / 2))
            / (28 - (bWSLevel + bBSLevel));

    	for (bi = (bBSLevel + 3); bi < bEnd; bi++)
    	{
    		waTargetCurve[bi] = waTargetCurve[bi - 1] + wDiff;
    	}
	}
}

// ---------------------------------------------------------------------------------
static void _vDrvADLBuildLumaCurve(const UINT16 waDynCurve[LUMA_CURVE_NUM],
                            const UINT16 waTargetCurve[LUMA_CURVE_NUM], 
                            UINT16 waTCurve[LUMA_CURVE_NUM],
                            UINT8 SceneChanged)
{
	UINT8 bi, bIndex;
    // previous luma curve : 10+4 bit to prevent quantization error 
    static UINT16 waTCurvePre[LUMA_CURVE_NUM];  

    _vDrvADLBuildDefault(waTCurve);

	for (bi = 3; bi < 30; bi++)
	{
		bIndex = waTargetCurve[bi] >> 5;
        bIndex = MIN(bIndex, (LUMA_CURVE_NUM-2));
        
		waTCurve[bi] = ((waDynCurve[bIndex + 1] * (waTargetCurve[bi] - (bIndex << 5))) +
		     (waDynCurve[bIndex] * (((bIndex + 1) << 5) - waTargetCurve[bi])) + 16) >> 5;
	}

	_vDrvApplyYGamma(waTCurve);

	if (SceneChanged != SV_TRUE)
	{
		for (bi = 3; bi < 30; bi++)
		{
			waTCurvePre[bi] = (((UINT32)waTCurvePre[bi] * 15 + 8) >> 4) + waTCurve[bi];
            waTCurve[bi] = (waTCurvePre[bi] + 8) >> 4;
		}
	}
    else
    {
        for (bi = 3; bi < 30; bi++)
		{
			waTCurvePre[bi] = waTCurve[bi] << 4;
		}
    }
   
    if (ADL_LOG_CHECK(AL_DBG_FINAL))
    {                
        LOG(3, "Dynamic   BWS    Final \n");
		for (bi = 0; bi < LUMA_CURVE_NUM; bi++)
		{
			LOG(3, "  %04d    %04d    %04d \n", waDynCurve[bi], waTargetCurve[bi], waTCurve[bi]);
		}        
    }
}

// ---------------------------------------------------------------------------------
//	Main Loop
// ---------------------------------------------------------------------------------
void vDrvADLProc_int(const UINT16 waHist[LUMA_HIST_NUM], UINT32 u4APL, UINT16 waTCurve[LUMA_CURVE_NUM])
{   
    _vDrvADLGetAdaptiveLumaParam();	  

    u4LumaAPL = u4APL;
    u4LumaAPLFullRange = (u4APL < 0x10) ? 0 : (MIN((u4APL-0x10)*0x12A/0x100, 0xFF));

	bADLSceneChange = _bDrvADLDetectSceneChanged(waHist);

	_vDrvADLBuildDynCurve(waHist, waDynCurve);
	_vDrvADLBuildTargetCurve(waHist, waTargetCurve);
	_vDrvADLBuildLumaCurve(waDynCurve, waTargetCurve, waTCurve, bADLSceneChange);
}

#endif //CC_MTK_LOADER
