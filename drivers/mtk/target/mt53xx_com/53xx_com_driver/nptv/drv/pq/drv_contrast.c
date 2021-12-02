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

/*************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: drv_contrast.c $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *
 *
 * Author:
 * -------
 *
 *
 * Last changed:
 * -------------
 * $Author: dtvbm11 $
 *
 * $Modtime: 05/12/01 8:18p $
 *
 * $Revision: #1 $
*************************************************************************/

#define _DRV_CONTRAST_C_

#include "general.h"
#include "pe_if.h"
#include "drv_contrast.h"
#include "video_timing.h"
#include "api_backlt.h"
#include "nptv_debug.h"
#include "x_util.h"
#include "drv_ycproc.h"
#include "drv_scaler.h"
#include "drv_video.h"
#include "srm_drvif.h"
#include "x_timer.h"

// support software register
#define SUPPORT_SW_REG 1

UINT16 wHist32Rec[32];

// 0.0 global information
UINT8 bAPL;
// 0.1 adaptive luma parameters : adaptive luma
UINT8 bAdapLumaGain;
UINT8 bAdapLumaOffset;
UINT8 bAdapLumaLimit;
UINT8 bMaxDiffThreshold;
UINT8 bTotalDiffThreshold;
UINT8 bALOnOff;
UINT8 bBWSOnOff;
UINT8 bBSOnOff;
UINT8 bBSLevel;
UINT8 bBSGain;
UINT8 bBSOffset;
UINT8 bBSRatio;
UINT8 bBSLimit;
UINT8 bWSOnOff;
UINT8 bWSLevel;
UINT8 bWSGain;
UINT8 bWSOffset;
UINT8 bWSRatio;
UINT8 bWSLimit;

// 0.2 adaptive luma parameters : bws middle point control
UINT8 bBWSKeepMidOnOff;
UINT8 bBWSKeepMidX;
UINT16 wBWSKeepMidY;

// 0.3 adaptive luma parameters : y gamma
UINT8 bForceYGamma;

// 0.4 adaptive luma parameters : dynamic black stretch
UINT8 bDynamicBSRange;
UINT8 bDynamicBSOnOff;
UINT8 bDynamicBSDbg;
UINT8 bDBSPxlThd1;
UINT8 bDBSPxlThd2;
UINT8 bDBSDistThdH;
UINT8 bDBSDistThdL;

UINT8 bDBSIntpMethod;

UINT8 bDBSLmtBtm;
UINT8 bDBSLmtTop;

UINT8 bDBSGainBtm;
UINT8 bDBSGainTop;

UINT8 bDBSOfstBtm;
UINT8 bDBSOfstTop;

// color tone
UINT8 u1BSColorToneOnOff;

UINT8 u1RRatioOffset; 
UINT8 u1GRatioOffset;
UINT8 u1BRatioOffset;  

UINT8 u1ASLRemapRLo;
UINT8 u1ASLRemapRHi;
UINT8 u1ASLRemapGLo;
UINT8 u1ASLRemapGHi;
UINT8 u1ASLRemapBLo;
UINT8 u1ASLRemapBHi;

UINT8 u1ASLRemapWDecay;

UINT8 fgCurveComputeOn;
UINT8 bCurveFreeze;
UINT8 bSceneChange;
UINT8 bLumaMax;
UINT8 bLumaMin;
UINT32 dwTotalSumOS;

UINT8 bYGammaDecayL;
UINT8 bYGammaDecayH;

UINT8 bDarkSceneBrit;

//----------------- Histogram Info
volatile UINT8 bTriggerHistogram = SV_FALSE;

//----------------- OPC
UINT8 bDBLMaxNoiseThreshold;
UINT8 bDBLMaxDiffThreshold; // next 0xd0a5
UINT8 bDBLTotalDiffThreshold; // next 0xd0a6
UINT8 bDBLNormlizedHistPrec; // next 0xd0a7
UINT8 bAlternateTurn;
UINT8 fgDynamicBacklightOn;
UINT8 bDBLUpdate;
UINT8 bDBLSceneChange;
UINT8 fgDBLOnToOFF;

extern UINT8 bUiPWM;


UINT16 wReadHist32Cur(UINT8 wAddr)
{
    return u2IO32Read2B(LUMA_HIST_START+2*wAddr);
}

void vWriteHist32Cur(UINT8 wAddr, UINT16 wData)
{
    vIO32Write2B(LUMA_HIST_START+2*wAddr, wData);
}

UINT16 wReadHist32Rec(UINT8 wAddr)
{
    return wHist32Rec[wAddr];
}

void vWriteHist32Rec(UINT8 wAddr, UINT16 wData)
{
    wHist32Rec[wAddr] = wData;
}

void vDrvSetYGammaCoeff(UINT8 bmode)
{
    if (bmode > 9)
    {
        LOG(0, "Not support such Y Gamma Coeff\n");
        bForceYGamma = 0;
    }
    else
    {
#if 0    
        LOG(0, "case 1 = GAMMA_950_TABLE\n");
    	LOG(0, "case 2 = GAMMA_925_TABLE\n");
    	LOG(0, "case 3 = GAMMA_900_TABLE\n");
    	LOG(0, "case 4 = GAMMA_875_TABLE\n");
    	LOG(0, "case 5 = GAMMA_850_TABLE\n");
    	LOG(0, "case 6 = GAMMA_825_TABLE\n");
    	LOG(0, "case 7 = GAMMA_105_TABLE\n");
    	LOG(0, "case 8 = GAMMA_110_TABLE\n");
    	LOG(0, "case 9 = GAMMA_115_TABLE\n");    
#endif    	
        bForceYGamma = bmode;
    }
}

UINT8 vDrvGetYGammaCoeff(void)
{
    LOG(0, "Current Gamma Table is case %d\n",(int)bForceYGamma);
    LOG(0, "case 0 = GAMMA_LINEAR\n");    
    LOG(0, "case 1 = GAMMA_950_TABLE\n");
	LOG(0, "case 2 = GAMMA_925_TABLE\n");
	LOG(0, "case 3 = GAMMA_900_TABLE\n");
	LOG(0, "case 4 = GAMMA_875_TABLE\n");
	LOG(0, "case 5 = GAMMA_850_TABLE\n");
	LOG(0, "case 6 = GAMMA_825_TABLE\n");
	LOG(0, "case 7 = GAMMA_105_TABLE\n");
	LOG(0, "case 8 = GAMMA_110_TABLE\n");
	LOG(0, "case 9 = GAMMA_115_TABLE\n");    
    return bForceYGamma;
}

static void vParamConstrain(void)
{
	bAdapLumaGain = (bAdapLumaGain > 0x80) ? 0x80 : bAdapLumaGain;
	bAdapLumaOffset = (bAdapLumaOffset > 16) ? 16 : bAdapLumaOffset;

	bBSLevel = (bBSLevel > 8) ? 8 : bBSLevel;
	bBSGain = (bBSGain > 0x80) ? 0x80 : bBSGain;

	bWSLevel = (bWSLevel > 8) ? 8 : bWSLevel;
	bWSGain = (bWSGain > 0x80) ? 0x80 : bWSGain;

	bDynamicBSRange = (bDynamicBSRange < 4) ? 4 : bDynamicBSRange;

	if(bDBSDistThdH <= bDBSDistThdL)
	{
        bDBSDistThdH = bDBSDistThdL + 1;    
	}

	if(bDBSPxlThd2 <= bDBSPxlThd1)
	{
        bDBSPxlThd2 = bDBSPxlThd1 + 1;    
	} 
}

void vDrvAdaptiveLumaSet(void)
{
#if SUPPORT_SW_REG // use usb dram protocol
    vIO32WriteFldAlign(ADAP_LUMA_00, bBSGain, SWREG_BWS_BLACK_GAIN);
    vIO32WriteFldAlign(ADAP_LUMA_00, bBSOffset, SWREG_BWS_BLACK_OFFSET);
    vIO32WriteFldAlign(ADAP_LUMA_00, bBSRatio, SWREG_BWS_BLACK_RATIO);
    vIO32WriteFldAlign(ADAP_LUMA_00, bBSLimit, SWREG_BWS_BLACK_LIMIT);

    vIO32WriteFldAlign(ADAP_LUMA_01, bWSGain, SWREG_BWS_WHITE_GAIN);
    vIO32WriteFldAlign(ADAP_LUMA_01, bWSOffset, SWREG_BWS_WHITE_OFFSET);
    vIO32WriteFldAlign(ADAP_LUMA_01, bWSRatio, SWREG_BWS_WHITE_RATIO);
    vIO32WriteFldAlign(ADAP_LUMA_01, bWSLimit, SWREG_BWS_WHITE_LIMIT);

    vIO32WriteFldAlign(ADAP_LUMA_02, bDBSPxlThd1, SWREG_DYNBS_PXL_THD1);
    vIO32WriteFldAlign(ADAP_LUMA_02, bDBSPxlThd2, SWREG_DYNBS_PXL_THD2);    
    vIO32WriteFldAlign(ADAP_LUMA_02, bDBSDistThdL, SWREG_DYNBS_DIST_THDL);
    vIO32WriteFldAlign(ADAP_LUMA_02, bDBSDistThdH, SWREG_DYNBS_DIST_THDH);
     
    vIO32WriteFldAlign(ADAP_LUMA_03, bAdapLumaGain, SWREG_AL_GAIN);  
    vIO32WriteFldAlign(ADAP_LUMA_03, bAdapLumaOffset, SWREG_AL_OFFSET);  
    vIO32WriteFldAlign(ADAP_LUMA_03, bAdapLumaLimit, SWREG_AL_LIMIT);  

    vIO32WriteFldAlign(ADAP_LUMA_04, bALOnOff, SWREG_AL_ON_1_OFF_0);  
    vIO32WriteFldAlign(ADAP_LUMA_04, bBWSOnOff, SWREG_BWS_ON_1_OFF_0);  
    vIO32WriteFldAlign(ADAP_LUMA_04, bBSOnOff, SWREG_BWS_BLACK_ON_1_OFF_0);  
    vIO32WriteFldAlign(ADAP_LUMA_04, bWSOnOff, SWREG_BWS_WHITE_ON_1_OFF_0);  
    vIO32WriteFldAlign(ADAP_LUMA_04, bDynamicBSOnOff, SWREG_DYNBS_ONOFF);  
    vIO32WriteFldAlign(ADAP_LUMA_04, bDynamicBSDbg, SWREG_DYNBS_DBG);  
    vIO32WriteFldAlign(ADAP_LUMA_04, bBSLevel, SWREG_BWS_BLACK_LEVEL);  
    vIO32WriteFldAlign(ADAP_LUMA_04, bWSLevel, SWREG_BWS_WHITE_LEVEL);  
    vIO32WriteFldAlign(ADAP_LUMA_04, bDynamicBSRange, SWREG_DYNBS_RANGE);

    vIO32WriteFldAlign(ADAP_LUMA_05, bDBSLmtBtm, SWREG_DYNBS_LMT_BTM);  
    vIO32WriteFldAlign(ADAP_LUMA_05, bDBSLmtTop, SWREG_DYNBS_LMT_TOP);  
    vIO32WriteFldAlign(ADAP_LUMA_05, bDBSGainBtm, SWREG_DYNBS_GAIN_BTM);  
    vIO32WriteFldAlign(ADAP_LUMA_05, bDBSGainTop, SWREG_DYNBS_GAIN_TOP);  
    vIO32WriteFldAlign(ADAP_LUMA_06, bDBSOfstBtm, SWREG_DYNBS_OFST_BTM);  
    vIO32WriteFldAlign(ADAP_LUMA_06, bDBSOfstTop, SWREG_DYNBS_OFST_TOP);

    vIO32WriteFldAlign(ADAP_LUMA_0D, bBWSKeepMidOnOff, SWREG_BWS_KEEP_MID_ON_OFF); 
    vIO32WriteFldAlign(ADAP_LUMA_0D, bBWSKeepMidX, SWREG_BWS_KEEP_MID_X); 
    vIO32WriteFldAlign(ADAP_LUMA_0D, wBWSKeepMidY, SWREG_BWS_KEEP_MID_Y); 

    vIO32WriteFldAlign(ADAP_LUMA_0C, bYGammaDecayH, Y_GAMMA_DECAY_H);  
    vIO32WriteFldAlign(ADAP_LUMA_0C, bYGammaDecayL, Y_GAMMA_DECAY_L);  

    vIO32WriteFldAlign(ADAP_LUMA_0C, bDarkSceneBrit, Y_BRIGHT);  

    vIO32WriteFldAlign(ADAP_LUMA_08, u1RRatioOffset, BS_COLOR_KR);
    vIO32WriteFldAlign(ADAP_LUMA_08, u1GRatioOffset, BS_COLOR_KG);
    vIO32WriteFldAlign(ADAP_LUMA_08, u1BRatioOffset, BS_COLOR_KB);    
    vIO32WriteFldAlign(ADAP_LUMA_09, u1ASLRemapRLo, BS_ASL_REMAP_R_LO);
    vIO32WriteFldAlign(ADAP_LUMA_09, u1ASLRemapRHi, BS_ASL_REMAP_R_HI);
    vIO32WriteFldAlign(ADAP_LUMA_09, u1ASLRemapGLo, BS_ASL_REMAP_G_LO);
    vIO32WriteFldAlign(ADAP_LUMA_09, u1ASLRemapGHi, BS_ASL_REMAP_G_HI);
    vIO32WriteFldAlign(ADAP_LUMA_0A, u1ASLRemapBLo, BS_ASL_REMAP_B_LO);
    vIO32WriteFldAlign(ADAP_LUMA_0A, u1ASLRemapBHi, BS_ASL_REMAP_B_HI); 
    vIO32WriteFldAlign(ADAP_LUMA_04, u1BSColorToneOnOff, SWREG_COLOR_BS_ON_1_OFF_0);     

    vIO32WriteFldAlign(ADAP_LUMA_0A, u1ASLRemapWDecay, BS_ASL_REMAP_W_DECAY); 

  
    // OPC initialization
    vDrvDBLInit();
#else
    vRegWriteFldAlign(AL_PRAM_01, bBSGain, HWREG_BWS_BLACK_GAIN);
    vRegWriteFldAlign(AL_PRAM_01, bBSOffset, HWREG_BWS_BLACK_OFFSET);
    vRegWriteFldAlign(AL_PRAM_01, bBSRatio, HWREG_BWS_BLACK_RATIO);
    vRegWriteFldAlign(AL_PRAM_01, bBSLimit, HWREG_BWS_BLACK_LIMIT);

    vRegWriteFldAlign(AL_PRAM_02, bWSGain, HWREG_BWS_WHITE_GAIN);
    vRegWriteFldAlign(AL_PRAM_02, bWSOffset, HWREG_BWS_WHITE_OFFSET);
    vRegWriteFldAlign(AL_PRAM_02, bWSRatio, HWREG_BWS_WHITE_RATIO);
    vRegWriteFldAlign(AL_PRAM_02, bWSLimit, HWREG_BWS_WHITE_LIMIT);

    vRegWriteFldAlign(AL_PRAM_03, bDBSPxlThd1, HWREG_DYNBS_PXL_THD1);
    vRegWriteFldAlign(AL_PRAM_03, bDBSPxlThd2, HWREG_DYNBS_PXL_THD2);    
    vRegWriteFldAlign(AL_PRAM_03, bDBSDistThdL, HWREG_DYNBS_DIST_THDL);
    vRegWriteFldAlign(AL_PRAM_03, bDBSDistThdH, HWREG_DYNBS_DIST_THDH);
     
    vRegWriteFldAlign(AL_PRAM_04, bAdapLumaGain, HWREG_AL_GAIN);  
    vRegWriteFldAlign(AL_PRAM_04, bAdapLumaOffset, HWREG_AL_OFFSET);  
    vRegWriteFldAlign(AL_PRAM_04, bAdapLumaLimit, HWREG_AL_LIMIT);  
    
    vRegWriteFldAlign(AL_PRAM_05, bBWSOnOff, HWREG_BWS_ON_1_OFF_0);  
    vRegWriteFldAlign(AL_PRAM_05, bBSOnOff, HWREG_BWS_BLACK_ON_1_OFF_0);  
    vRegWriteFldAlign(AL_PRAM_05, bWSOnOff, HWREG_BWS_WHITE_ON_1_OFF_0);  
    vRegWriteFldAlign(AL_PRAM_05, bDynamicBSOnOff, HWREG_DYNBS_ONOFF);  
    vRegWriteFldAlign(AL_PRAM_05, bDynamicBSDbg, HWREG_DYNBS_DBG);  
    vRegWriteFldAlign(AL_PRAM_05, bBSLevel, HWREG_BWS_BLACK_LEVEL);  
    vRegWriteFldAlign(AL_PRAM_05, bWSLevel, HWREG_BWS_WHITE_LEVEL);  
    vRegWriteFldAlign(AL_PRAM_05, bDynamicBSRange, HWREG_DYNBS_RANGE);

    vRegWriteFldAlign(DUMMY_OS_7B, bDBSLmtBtm, HWREG_DYNBS_LMT_BTM);  
    vRegWriteFldAlign(DUMMY_OS_7B, bDBSLmtTop, HWREG_DYNBS_LMT_TOP);  
    vRegWriteFldAlign(DUMMY_OS_7B, bDBSGainBtm, HWREG_DYNBS_GAIN_BTM);  
    vRegWriteFldAlign(DUMMY_OS_7B, bDBSGainTop, HWREG_DYNBS_GAIN_TOP);  
    vRegWriteFldAlign(DUMMY_OS_7E, bDBSOfstBtm, HWREG_DYNBS_OFST_BTM);  
    vRegWriteFldAlign(DUMMY_OS_7E, bDBSOfstTop, HWREG_DYNBS_OFST_TOP);
#endif//SUPPORT_DRAM_REG
}

void vDrvAdaptiveLumaGet(void)
{
#if SUPPORT_SW_REG // use usb dram protocol    
    bAdapLumaGain   =   IO32ReadFldAlign(ADAP_LUMA_03, SWREG_AL_GAIN);
    bAdapLumaOffset =   IO32ReadFldAlign(ADAP_LUMA_03, SWREG_AL_OFFSET);
    bAdapLumaLimit  =   IO32ReadFldAlign(ADAP_LUMA_03, SWREG_AL_LIMIT);
    bBSLevel =          IO32ReadFldAlign(ADAP_LUMA_04, SWREG_BWS_BLACK_LEVEL);
    bWSLevel =          IO32ReadFldAlign(ADAP_LUMA_04, SWREG_BWS_WHITE_LEVEL);  

    bBSGain =   IO32ReadFldAlign(ADAP_LUMA_00, SWREG_BWS_BLACK_GAIN);
    bBSOffset = IO32ReadFldAlign(ADAP_LUMA_00, SWREG_BWS_BLACK_OFFSET);
    bBSRatio =  IO32ReadFldAlign(ADAP_LUMA_00, SWREG_BWS_BLACK_RATIO);
    bBSLimit =  IO32ReadFldAlign(ADAP_LUMA_00, SWREG_BWS_BLACK_LIMIT);

    bWSGain =   IO32ReadFldAlign(ADAP_LUMA_01, SWREG_BWS_WHITE_GAIN);
    bWSOffset = IO32ReadFldAlign(ADAP_LUMA_01, SWREG_BWS_WHITE_OFFSET);
    bWSRatio =  IO32ReadFldAlign(ADAP_LUMA_01, SWREG_BWS_WHITE_RATIO);
    bWSLimit =  IO32ReadFldAlign(ADAP_LUMA_01, SWREG_BWS_WHITE_LIMIT);    

    bDBSPxlThd1 =   IO32ReadFldAlign(ADAP_LUMA_02, SWREG_DYNBS_PXL_THD1);  
    bDBSPxlThd2 =   IO32ReadFldAlign(ADAP_LUMA_02, SWREG_DYNBS_PXL_THD2);  
    bDBSDistThdL =  IO32ReadFldAlign(ADAP_LUMA_02, SWREG_DYNBS_DIST_THDL);  
    bDBSDistThdH =  IO32ReadFldAlign(ADAP_LUMA_02, SWREG_DYNBS_DIST_THDH);  

    bDynamicBSRange =   IO32ReadFldAlign(ADAP_LUMA_04, SWREG_DYNBS_RANGE);  
    
    bDBSLmtBtm =    IO32ReadFldAlign(ADAP_LUMA_05, SWREG_DYNBS_LMT_BTM);
    bDBSLmtTop =    IO32ReadFldAlign(ADAP_LUMA_05, SWREG_DYNBS_LMT_TOP);

    bDBSGainBtm =   IO32ReadFldAlign(ADAP_LUMA_05, SWREG_DYNBS_GAIN_BTM);
    bDBSGainTop =   IO32ReadFldAlign(ADAP_LUMA_05, SWREG_DYNBS_GAIN_TOP);

    bDBSOfstBtm =   IO32ReadFldAlign(ADAP_LUMA_06, SWREG_DYNBS_OFST_BTM);
    bDBSOfstTop =   IO32ReadFldAlign(ADAP_LUMA_06, SWREG_DYNBS_OFST_TOP);

    bBWSKeepMidOnOff = IO32ReadFldAlign(ADAP_LUMA_0D, SWREG_BWS_KEEP_MID_ON_OFF);
    bBWSKeepMidX = IO32ReadFldAlign(ADAP_LUMA_0D, SWREG_BWS_KEEP_MID_X);
    wBWSKeepMidY = IO32ReadFldAlign(ADAP_LUMA_0D, SWREG_BWS_KEEP_MID_Y);

    bDynamicBSOnOff  =  IO32ReadFldAlign(ADAP_LUMA_04, SWREG_DYNBS_ONOFF);
    bALOnOff =         IO32ReadFldAlign(ADAP_LUMA_04, SWREG_AL_ON_1_OFF_0);
    bBWSOnOff =         IO32ReadFldAlign(ADAP_LUMA_04, SWREG_BWS_ON_1_OFF_0);
    bBSOnOff =          IO32ReadFldAlign(ADAP_LUMA_04, SWREG_BWS_BLACK_ON_1_OFF_0);
    bWSOnOff =          IO32ReadFldAlign(ADAP_LUMA_04, SWREG_BWS_WHITE_ON_1_OFF_0);
    u1BSColorToneOnOff = IO32ReadFldAlign(ADAP_LUMA_04, SWREG_COLOR_BS_ON_1_OFF_0); 
    bDynamicBSDbg   =   IO32ReadFldAlign(ADAP_LUMA_04, SWREG_DYNBS_DBG);


    bYGammaDecayH    =  IO32ReadFldAlign(ADAP_LUMA_0C, Y_GAMMA_DECAY_H);
    bYGammaDecayL    =  IO32ReadFldAlign(ADAP_LUMA_0C, Y_GAMMA_DECAY_L);
    bDarkSceneBrit   =  IO32ReadFldAlign(ADAP_LUMA_0C, Y_BRIGHT);

    u1RRatioOffset = IO32ReadFldAlign(ADAP_LUMA_08, BS_COLOR_KR);
    u1GRatioOffset = IO32ReadFldAlign(ADAP_LUMA_08, BS_COLOR_KG);
    u1BRatioOffset = IO32ReadFldAlign(ADAP_LUMA_08, BS_COLOR_KB);    
    
    u1ASLRemapRLo = IO32ReadFldAlign(ADAP_LUMA_09, BS_ASL_REMAP_R_LO);
    u1ASLRemapRHi = IO32ReadFldAlign(ADAP_LUMA_09, BS_ASL_REMAP_R_HI);
    u1ASLRemapGLo = IO32ReadFldAlign(ADAP_LUMA_09, BS_ASL_REMAP_G_LO);
    u1ASLRemapGHi = IO32ReadFldAlign(ADAP_LUMA_09, BS_ASL_REMAP_G_HI);
    u1ASLRemapBLo = IO32ReadFldAlign(ADAP_LUMA_0A, BS_ASL_REMAP_B_LO);
    u1ASLRemapBHi = IO32ReadFldAlign(ADAP_LUMA_0A, BS_ASL_REMAP_B_HI);    

    u1ASLRemapWDecay = IO32ReadFldAlign(ADAP_LUMA_0A, BS_ASL_REMAP_W_DECAY);       
#else
    bAdapLumaGain   =   RegReadFldAlign(AL_PRAM_04, HWREG_AL_GAIN);
    bAdapLumaOffset =   RegReadFldAlign(AL_PRAM_04, HWREG_AL_OFFSET);
    bAdapLumaLimit  =   RegReadFldAlign(AL_PRAM_04, HWREG_AL_LIMIT);
    bBSLevel =          RegReadFldAlign(AL_PRAM_05, HWREG_BWS_BLACK_LEVEL);
    bWSLevel =          RegReadFldAlign(AL_PRAM_05, HWREG_BWS_WHITE_LEVEL);  

    bBSGain =   RegReadFldAlign(AL_PRAM_01, HWREG_BWS_BLACK_GAIN);
    bBSOffset = RegReadFldAlign(AL_PRAM_01, HWREG_BWS_BLACK_OFFSET);
    bBSRatio =  RegReadFldAlign(AL_PRAM_01, HWREG_BWS_BLACK_RATIO);
    bBSLimit =  RegReadFldAlign(AL_PRAM_01, HWREG_BWS_BLACK_LIMIT);

    bWSGain =   RegReadFldAlign(AL_PRAM_02, HWREG_BWS_WHITE_GAIN);
    bWSOffset = RegReadFldAlign(AL_PRAM_02, HWREG_BWS_WHITE_OFFSET);
    bWSRatio =  RegReadFldAlign(AL_PRAM_02, HWREG_BWS_WHITE_RATIO);
    bWSLimit =  RegReadFldAlign(AL_PRAM_02, HWREG_BWS_WHITE_LIMIT);    

    bDBSPxlThd1 =   RegReadFldAlign(AL_PRAM_03, HWREG_DYNBS_PXL_THD1);  
    bDBSPxlThd2 =   RegReadFldAlign(AL_PRAM_03, HWREG_DYNBS_PXL_THD2);  
    bDBSDistThdL =  RegReadFldAlign(AL_PRAM_03, HWREG_DYNBS_DIST_THDL);  
    bDBSDistThdH =  RegReadFldAlign(AL_PRAM_03, HWREG_DYNBS_DIST_THDH);  

    bDynamicBSRange =   RegReadFldAlign(AL_PRAM_05, HWREG_DYNBS_RANGE);  
    
    bDBSLmtBtm =    RegReadFldAlign(DUMMY_OS_7B, HWREG_DYNBS_LMT_BTM);
    bDBSLmtTop =    RegReadFldAlign(DUMMY_OS_7B, HWREG_DYNBS_LMT_TOP);

    bDBSGainBtm =   RegReadFldAlign(DUMMY_OS_7B, HWREG_DYNBS_GAIN_BTM);
    bDBSGainTop =   RegReadFldAlign(DUMMY_OS_7B, HWREG_DYNBS_GAIN_TOP);

    bDBSOfstBtm =   RegReadFldAlign(DUMMY_OS_7E, HWREG_DYNBS_OFST_BTM);
    bDBSOfstTop =   RegReadFldAlign(DUMMY_OS_7E, HWREG_DYNBS_OFST_TOP);

    bDynamicBSOnOff  =  RegReadFldAlign(AL_PRAM_05, HWREG_DYNBS_ONOFF);
    bBWSOnOff =         RegReadFldAlign(AL_PRAM_05, HWREG_BWS_ON_1_OFF_0);
    bBSOnOff =          RegReadFldAlign(AL_PRAM_05, HWREG_BWS_BLACK_ON_1_OFF_0);
    bWSOnOff =          RegReadFldAlign(AL_PRAM_05, HWREG_BWS_WHITE_ON_1_OFF_0);
    bDynamicBSDbg   =   RegReadFldAlign(AL_PRAM_05, HWREG_DYNBS_DBG);
#endif //#ifndef CC_UP8032_ATV
    vParamConstrain();
}

static UINT8 bReadQtable(UINT16 bIndex)
{
    return QUALITY_TBL[bIndex];
}

void vDrvHistInit(void)
{
    vRegWriteFldAlign(METRIC_00, 1, BIN32);
    vRegWriteFldAlign(METRIC_00, 1, FRAMEBASE);
}

void vDrvHistTrigger(void)
{
    // notifying histogram trigger in next input vsync
    // see vDrvTriggerHistogramInVsync()
    bTriggerHistogram = SV_TRUE;
}

void vDrvGetHistogramInfoInVsync()
{ 
	if (!fgIsAdaptiveFlgSet(FLG_ADAPTIVE_LUMA))
	{
		return;
	}

    if(bDrvIsHistReady())
    {
        vDrvGetHistInfo();

        vRegWriteFldAlign(METRIC_00, 0, METRIC_ENA);
        vRegWriteFldAlign(METRIC_00, 1, METRIC_ENA);
    }    
}

void vDrvTriggerHistogramInVsync()
{
    if( bTriggerHistogram )
    {
        vRegWriteFldAlign(METRIC_00, 0, METRIC_ENA);
        vRegWriteFldAlign(METRIC_00, 1, METRIC_ENA);
        
        bTriggerHistogram = SV_FALSE;
    }   
}

UINT8 bDrvIsHistReady(void)
{
    return (RegReadFldAlign(METRIC_STA_11, METRIC_RDY) ? SV_TRUE : SV_FALSE);
}

void vDrvGetHistInfo(void)
{
    UINT8 i;
    UINT32 dwPixCnt = RegReadFldAlign(METRIC_STA_18, METRIC_PCNTALL);
    UINT32 dwLumaSum = RegReadFldAlign(METRIC_STA_11, METRIC_YSUM);

    bLumaMax = RegReadFldAlign(METRIC_STA_12, METRIC_YMAX);
    bLumaMin = RegReadFldAlign(METRIC_STA_12, METRIC_YMIN);

    bAPL = (dwPixCnt) ? (dwLumaSum/dwPixCnt) : 0;   

    vIO32WriteFldAlign(ADAP_LUMA_0E, dwLumaSum, AL_Y_SUM);
    vIO32WriteFldAlign(ADAP_LUMA_0F, dwPixCnt, AL_PXL_CNT);    

    vRegWriteFldAlign(METRIC_00, 0, SEE_NEXT_16BIN);
    for (i = 0; i < LUMA_HIST_LEVEL; i++)
    {
        vWriteHist32Cur(i, u2RegRead2B(METRIC_STA_00 + (i * 2)));
    }

    vRegWriteFldAlign(METRIC_00, 1, SEE_NEXT_16BIN);
    for (i = 0; i < LUMA_HIST_LEVEL; i++)
    {
        vWriteHist32Cur((i + LUMA_HIST_LEVEL),
                        u2RegRead2B(METRIC_STA_00 + (i * 2)));
    }
}

void vDrvContrastDefault(void)
{
    UINT8 bi;

    // Note : Due to Fetch in VSync, can't use vRegWrite2B
    for (bi = 0; bi < LUMA_HIST_LEVEL; bi++)
    {
        vRegWrite4B(Y_FTN_1_0_MAIN + 4 * bi, 0x40 * bi + 0x400000 * bi + 0x200000);
    }

    vRegWrite2B(Y_FTN_32_MAIN, 0x3FF);
}

void vDrvSetLumaCurve(const UINT16 * waLArray)
{
    UINT8 bi;

    // Note : Due to Fetch in VSync, can't use vRegWrite2B
    for (bi = 0; bi < LUMA_HIST_LEVEL; bi++)
    {
        vRegWrite4B(Y_FTN_1_0_MAIN + 4 * bi,
            (UINT32)waLArray[2 * bi] + (((UINT32)waLArray[2 * bi + 1]) << 16));
    }

    vRegWrite4B(Y_FTN_32_MAIN, waLArray[LUMA_HIST_NUM]);
}

void vDrvReadLumaCurve(UINT16 * waLArray)
{
    UINT8 bi;

    for (bi = 0; bi < LUMA_HIST_LEVEL; bi++)
    {
        waLArray[2 * bi] = RegReadFldAlign(Y_FTN_1_0_MAIN + 4 * bi, Y_FTN_0);
        waLArray[2 * bi + 1] = RegReadFldAlign(Y_FTN_1_0_MAIN + 4 * bi, Y_FTN_1);        
    }

    waLArray[LUMA_HIST_NUM] = RegReadFldAlign(Y_FTN_32_MAIN, Y_FTN_0);

}

void vDrvSetAutoContrastWindow(void)
{
    UINT16 wWidth, wHeight;

    wWidth = wDrvVideoInputWidth(SV_VP_MAIN) - 8;

    if (bDrvVideoIsSrcInterlace(SV_VP_MAIN))
        wHeight = wDrvVideoInputHeight(SV_VP_MAIN) / 2 - 4;
    else
        wHeight = wDrvVideoInputHeight(SV_VP_MAIN) - 8;

    vRegWriteFldAlign(METRIC_01, 8, HSTART);
    vRegWriteFldAlign(METRIC_00, 8, VMASK_RISE);

    vRegWriteFldAlign(METRIC_01, wWidth, HEND);
    vRegWriteFldAlign(METRIC_00, wHeight, VMASK_FALL);
    vDrvHistTrigger();
}

UINT8 bDrvGetAPL(void)
{
    return bAPL;
}

UINT8 bDrvGetLumaMax(void)
{
    return bLumaMax;
}

UINT8 bDrvGetLumaMin(void)
{
    return bLumaMin;
}

UINT8 bDrvGetHist(UINT16 * wHist)
{
    UINT8 i;

    if (wHist == NULL)
    {
        return SV_FALSE;
    }

    for (i = 0; i < LUMA_HIST_LEVEL; i++)
    {
        wHist[i] = wReadHist32Cur((i * 2)) + wReadHist32Cur(((i * 2) + 1));
    }

    return SV_TRUE;
}

UINT8 bDrvGetHist32(UINT16 * wHist)
{
    UINT8 i;

    if (wHist == NULL)
    {
        return SV_FALSE;
    }

    for (i = 0; i < LUMA_HIST_NUM; i++)
    {
        wHist[i] = wReadHist32Cur(i);
    }

    return SV_TRUE;
}

UINT8 bDrvGetNormHist(UINT16 * wHist)
{
    UINT8 i;
    UINT32 dwTotal = 0;

    if (wHist == NULL)
    {
        return SV_FALSE;
    }

    for (i = 0; i < LUMA_HIST_NUM; i++)
    {
        dwTotal += wReadHist32Cur(i);
    }

    for (i = 0; i < LUMA_HIST_LEVEL; i++)
    {
        // Use UINT64 to avoid data overflow when full HD resolution.
        if (dwTotal == 0)
        {
            wHist[i] = 0;
        }
        else
        {
            wHist[i] =
                (UINT16) u8Div6432((UINT64)
                          (wReadHist32Cur((i * 2)) + wReadHist32Cur(((i * 2) + 1))) * 10000,
                           dwTotal, NULL);
        }
    }

    return SV_TRUE;
}

UINT8 bDrvGetNormHist32(UINT16 * wHist)
{
    UINT8 i;
    UINT32 dwTotal = 0;

    if (wHist == NULL)
    {
        return SV_FALSE;
    }

    for (i = 0; i < LUMA_HIST_NUM; i++)
    {
        dwTotal += wReadHist32Cur(i);
    }

    for (i = 0; i < LUMA_HIST_NUM; i++)
    {
        // User UINT64 to avoid data overflow when full HD resolution.
        if (dwTotal == 0)
        {
            wHist[i] = 0;
        }
        else
        {
        #ifdef THE_3RD_PARTY_SW_SUPPORT
            wHist[i] =
                (UINT16) u8Div6432( (UINT64)wReadHist32Cur(i) * 65535, dwTotal, NULL);
            #else
            wHist[i] =
                (UINT16) u8Div6432( (UINT64)wReadHist32Cur(i) * 10000, dwTotal, NULL);
        #endif

        }
    }

    return SV_TRUE;
}



UINT8 bDrvGetOutputAPL(UINT8 bPath)
{
#if 0 // TBD    
    UINT32 dwOutPicWidth = wDrvGetOutputHTotal(); // not correct ??
    UINT32 dwOutPicHeight = wDrvGetOutputVTotal(); // not correct ??
    UINT32 dwSumY;

    if (bPath == SV_VP_MAIN)
    {
        dwSumY = RegReadFldAlign(OUTSTG_RO_77, RO_77_M_YSUM);
    }
    else
    {
        dwSumY = RegReadFldAlign(OUTSTG_RO_78, RO_78_P_YSUM);
    }

    if ((dwOutPicWidth != 0) && (dwOutPicHeight != 0))
    {
        dwTotalSumOS = ((dwSumY / dwOutPicWidth) / dwOutPicHeight);
    }
#endif

    return dwTotalSumOS;
}

UINT8 bDrvGetOutputLumaMax(UINT8 bPath)
{
    if (bPath == SV_VP_MAIN)
    {
        return RegReadFldAlign(OUTSTG_RO_79, RO_79_M_YMAX);
    }
    else
    {
        return RegReadFldAlign(OUTSTG_RO_79, RO_79_P_YMAX);
    }
}


UINT8 bDrvGetOutputLumaMin(UINT8 bPath)
{
    if (bPath == SV_VP_MAIN)
    {
        return RegReadFldAlign(OUTSTG_RO_79, RO_79_M_YMIN);
    }
    else
    {
        return RegReadFldAlign(OUTSTG_RO_79, RO_79_P_YMIN);
    }
}


static void vDrvALInit(void)
{
	bAdapLumaGain = AL_GAIN;
	bAdapLumaOffset = AL_OFFSET;
	bAdapLumaLimit = AL_LIMIT;

	bMaxDiffThreshold = MAX_DIFF_THRESHOLD;
	bTotalDiffThreshold = TOTAL_DIFF_THRESHOLD;
    bALOnOff = SV_TRUE;
}

static void vDrvBWSInit(void)
{
    bBWSOnOff = SV_FALSE;
    bBSOnOff = BLACK_STRETCH_ENABLE;
    bWSOnOff = WHITE_STRETCH_ENABLE;

    bBSLevel = BLACK_STRETCH_LEVEL;
    bBSGain = BLACK_STRETCH_GAIN;
    bBSOffset = BLACK_STRETCH_OFFSET;
    bBSRatio = BLACK_STRETCH_RATIO;
    bBSLimit = BLACK_STRETCH_LIMIT;

    bWSLevel = WHITE_STRETCH_LEVEL;
    bWSGain = WHITE_STRETCH_GAIN;
    bWSOffset = WHITE_STRETCH_OFFSET;
    bWSRatio = WHITE_STRETCH_RATIO;
    bWSLimit = WHITE_STRETCH_LIMIT;
}

void vDrvAutoContrastInit(void)
{
	fgCurveComputeOn = SV_FALSE;
	bCurveFreeze = SV_FALSE;

	vDrvALInit();
	vDrvAutoContrastInit_int();
	vDrvBWSInit();
	vDrvHistInit();
	fgCurveComputeOn = SV_TRUE;
}

void vDrvAutoContrastFreeze(UINT8 bFreeze)
{
    bCurveFreeze = bFreeze;
}

void vDrvAutoContrastOn(UINT8 bPath)
{
    if (bPath == SV_VP_PIP)
    {
        return;
    }

    vDrvHistTrigger();
    fgCurveComputeOn = SV_TRUE;
    bSceneChange = SV_TRUE;
}

void vDrvAutoContrastOff(UINT8 bPath)
{
    if (bPath == SV_VP_PIP)
    {
        return;
    }

    vDrvHistTrigger();
    fgCurveComputeOn = SV_FALSE;
    vDrvContrastDefault();
}

void vDrvAutoContrast(void)
{
	if (!fgIsAdaptiveFlgSet(FLG_ADAPTIVE_LUMA))
	{
		return;
	}
   	
    vDrvAdaptiveLumaGet();	   

	if (fgCurveComputeOn == SV_TRUE)
	{
		vDrvContrastProc_int();

		if (bCurveFreeze == SV_FALSE)
		{
			vDrvContrastUpdate();
		}
	}		
}

void vDrvAutoContrastSetDefault(void)
{
    bALOnOff = bReadQtable(QUALITY_AL_ON_1_OFF_0);
	bAdapLumaGain = bReadQtable(QUALITY_AL_GAIN);
	bAdapLumaOffset = bReadQtable(QUALITY_AL_OFFSET);
	bAdapLumaLimit = bReadQtable(QUALITY_AL_LIMIT);

	bMaxDiffThreshold = bReadQtable(QUALITY_AL_SCENE_CHANGE_MAX_THD);
	bTotalDiffThreshold = bReadQtable(QUALITY_AL_SCENE_CHANGE_TOTAL_THD);

	bBWSOnOff = bReadQtable(QUALITY_BWS_ON_1_OFF_0);
	bBSOnOff = bReadQtable(QUALITY_BWS_BLACK_ON_1_OFF_0);
	bWSOnOff = bReadQtable(QUALITY_BWS_WHITE_ON_1_OFF_0);

	bBSLevel = bReadQtable(QUALITY_BWS_BLACK_LEVEL);
	bBSGain = bReadQtable(QUALITY_BWS_BLACK_GAIN);
	bBSOffset = bReadQtable(QUALITY_BWS_BLACK_OFFSET);
	bBSRatio = bReadQtable(QUALITY_BWS_BLACK_RATIO);
	bBSLimit = bReadQtable(QUALITY_BWS_BLACK_LIMIT);

	bWSLevel = bReadQtable(QUALITY_BWS_WHITE_LEVEL);
	bWSGain = bReadQtable(QUALITY_BWS_WHITE_GAIN);
	bWSOffset = bReadQtable(QUALITY_BWS_WHITE_OFFSET);
	bWSRatio = bReadQtable(QUALITY_BWS_WHITE_RATIO);
	bWSLimit = bReadQtable(QUALITY_BWS_WHITE_LIMIT);

	bDynamicBSRange = bReadQtable(QUALITY_DYNBS_RANGE);
	bDynamicBSOnOff = bReadQtable(QUALITY_DYNBS_ONOFF);
	bDynamicBSDbg = bReadQtable(QUALITY_DYNBS_DBG);
	bDBSPxlThd1 = bReadQtable(QUALITY_DYNBS_PXL_THD1);
	bDBSPxlThd2 = bReadQtable(QUALITY_DYNBS_PXL_THD2);
	bDBSDistThdH = bReadQtable(QUALITY_DYNBS_DIST_THDH);
	bDBSDistThdL = bReadQtable(QUALITY_DYNBS_DIST_THDL);

    bDBSLmtBtm = bReadQtable(QUALITY_DYNBS_LIMIT_BTM);
    bDBSLmtTop = bReadQtable(QUALITY_DYNBS_LIMIT_TOP);

    bDBSGainBtm = bReadQtable(QUALITY_DYNBS_GAIN_BTM);
    bDBSGainTop = bReadQtable(QUALITY_DYNBS_GAIN_TOP);

    bDBSOfstBtm = bReadQtable(QUALITY_DYNBS_OFFSET_BTM);
    bDBSOfstTop = bReadQtable(QUALITY_DYNBS_OFFSET_TOP);

	bBWSKeepMidOnOff = bReadQtable(QUALITY_BWS_KEEP_MID_ONOFF);
	bBWSKeepMidX = bReadQtable(QUALITY_BWS_KEEP_MID_X_LVL);
	wBWSKeepMidY = ((UINT16)bReadQtable(QUALITY_BWS_KEEP_MID_Y_POS)) << 2;

	u1BSColorToneOnOff = bReadQtable(QUALITY_TONE_PROT_ON_1_OFF_0); 

    u1RRatioOffset = bReadQtable(QUALITY_TONE_PROT_COLOR_KR); 
    u1GRatioOffset = bReadQtable(QUALITY_TONE_PROT_COLOR_KG);
    u1BRatioOffset = bReadQtable(QUALITY_TONE_PROT_COLOR_KB);  

    u1ASLRemapRLo = bReadQtable(QUALITY_TONE_PROT_ASL_REMAP_R_LO);
    u1ASLRemapRHi = bReadQtable(QUALITY_TONE_PROT_ASL_REMAP_R_HI);
    u1ASLRemapGLo = bReadQtable(QUALITY_TONE_PROT_ASL_REMAP_G_LO);
    u1ASLRemapGHi = bReadQtable(QUALITY_TONE_PROT_ASL_REMAP_G_HI);
    u1ASLRemapBLo = bReadQtable(QUALITY_TONE_PROT_ASL_REMAP_B_LO);
    u1ASLRemapBHi = bReadQtable(QUALITY_TONE_PROT_ASL_REMAP_B_HI);	

    u1ASLRemapWDecay = bReadQtable(QUALITY_TONE_PROT_ASL_REMAP_W_DECAY);	


    bYGammaDecayH    =  bReadQtable(QUALITY_YGAMMA_DECAY_HI);
    bYGammaDecayL    =  bReadQtable(QUALITY_YGAMMA_DECAY_LO);
    bDarkSceneBrit   =  bReadQtable(QUALITY_YGAMMA_DECAY_BRIGHT);


    u1BSColorToneOnOff = bReadQtable(QUALITY_TONE_PROT_ON_1_OFF_0);	

	vParamConstrain();
    vDrvAdaptiveLumaSet();
    
}

void vDrvAutoContrastSetALOff(UINT8 bPath)
{

    if (bPath == SV_VP_PIP)
    {
        return;
    }
    
	bAdapLumaGain   = 0x0;
	bAdapLumaOffset = 0x0;
	bWSGain         = 0x80;
	bWSOffset       = 0x0;
	bWSLimit        = 0x0;

    vDrvAdaptiveLumaSet();    
}

void vDrvAutoContrastSetBSOff(UINT8 bPath)
{

    if (bPath == SV_VP_PIP)
    {
        return;
    }

	bBSGain = 0x80;
	bBSOffset = 0x0;
	bBSLimit =0x0;
	bDynamicBSOnOff = 0x0;

    vDrvAdaptiveLumaSet();
    
	bSceneChange = SV_TRUE;
}


void vDrvVideoSoftBWSOnOff(UINT8 bOnOff)
{
    bBWSOnOff = bOnOff;
    bSceneChange = SV_TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////
// RGB Max Meter
UINT32 dwRGBHist16[32];

UINT32 dwReadRGBHist16(UINT8 wAddr)
{
    return dwRGBHist16[wAddr];
}

void vWriteRGBHist16(UINT8 wAddr, UINT32 wData)
{
    dwRGBHist16[wAddr] = wData;
}

void vDrvDBLInit(void)
{
    bDBLMaxDiffThreshold = DBL_MAX_DIFF_THRESHOLD;
    bDBLTotalDiffThreshold = DBL_TOTAL_DIFF_THRESHOLD;
    bDBLMaxNoiseThreshold = DBL_MAX_NOISE_THRESHOLD;
    bDBLNormlizedHistPrec = DBL_NORMALIZED_HIST_PREC;

    vRegWriteFldAlign(DBL_CFG2, DYNAMIC_BACKLIGHT_ON, DBL_ENABLE);
    fgDynamicBacklightOn = DYNAMIC_BACKLIGHT_ON;



    vIO32WriteFldAlign(OPC_0A, 995, OPC_MAX_CONTENT_TH);
    vIO32WriteFldAlign(OPC_01, 0, OPC_ON_OFF);
    vIO32WriteFldAlign(OPC_01, 0, OPC_CONTROL_ON_OFF);    
    vIO32WriteFldAlign(OPC_0E, 128, RUN_QUE_SIZE);    
    
}

void vDrvGetNormHist32(UINT32 * wHist)
{
    UINT8 bi;
    for (bi = 0; bi < RGB_HIST_NUM; bi++)
    {
        wHist[bi] = (UINT32)RegReadFldAlign(OS_40 + (bi * 4), CNT00);
    }
}

void vDrvGetRGBHistInfo(void){
    UINT8 bi;
    for (bi = 0; bi < RGB_HIST_NUM; bi++)
    {
        vWriteRGBHist16(bi, (UINT32)RegReadFldAlign(OS_40 + (bi * 4), CNT00));
    }
}


