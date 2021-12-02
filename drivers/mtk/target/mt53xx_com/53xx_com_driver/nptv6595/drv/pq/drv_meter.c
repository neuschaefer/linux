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

// === INCLUDE =============================================================================
#include "general.h"
#include "drv_meter.h"
#include "drv_ycproc.h"
#include "hw_ospe.h"
#include "hw_scpos.h"
#include "hw_sw.h"
#include "hw_ycproc.h"
#include "x_assert.h"
#include "nptv_debug.h"
#include "vdo_misc.h"

// === HW INCLUDE =============================================================================

// === DEFINE =============================================================================

// === FUNCTION PROTOTYPE =================================================================

// === EXTERN FUNCTION ====================================================================

// === GLOBAL VARIABLE ====================================================================

// === EXTERN VARIABLE ====================================================================

// === STATIC VARIABLE ====================================================================
static UINT8 const CONTENT_METER_TH[8] = { 6, 12, 13, 17, 21, 22, 26, 30};
static UINT8 bContMeterCnt;
static UINT8 bContMeterIIR;
static UINT8 bIsHD = SV_FALSE;
// === BODY ===============================================================================

// ====================================================================================================
// Common utility functions
// ====================================================================================================
/**
 * @brief Normalize the input histogram.
 * @param waHistCur : array to get the histogram
 * @param wBase : Normalizing base
 * @param bDim : Dim of the input array
 */
UINT8 bHistogramNormalization(UINT16 waHistCur[], UINT8 bDim, UINT16 wBase)
{
	UINT32 i, dwHistSum=0, dwDiv;
    UINT32 HistAcc[LUMA_HIST_NUM];

    if (bDim > LUMA_HIST_NUM)
    {
        //Printf("bHistogramNormalization error dim %d\n", bDim);
        return SV_FALSE;
    }
    
	for (i = 0; i < bDim; i++)
	{
	    dwHistSum += waHistCur[i];
	    HistAcc[i] = dwHistSum;
	}

	if (dwHistSum != 0)
	{
        dwDiv = (0x10000*wBase+dwHistSum/2)/dwHistSum;

        HistAcc[0] = (HistAcc[0]*dwDiv+0x8000)>>16;
		waHistCur[0] = (UINT16)HistAcc[0];
	    
		for (i = 1; i < bDim; i++)
		{
			HistAcc[i] = (HistAcc[i]*dwDiv+0x8000)>>16;
			waHistCur[i] = HistAcc[i] - HistAcc[i-1];
		}

		return SV_TRUE;
	}
	else
	{
		return SV_FALSE;
	}	
}

// ====================================================================================================
// Luma Histogram
// ====================================================================================================
/**
 * @notice function called in InputVsync
 */
void vDrvGetLumaInfoHW(void)
{
	UINT32 i;
	UINT32 dwPixCnt = RegReadFldAlign(METRIC_STA_18, METRIC_PCNTALL);
	UINT32 dwLumaSum = RegReadFldAlign(METRIC_STA_11, METRIC_YSUM);
    UINT8 bYMin = RegReadFldAlign(METRIC_STA_12, METRIC_YMIN);
    UINT8 bYMax = RegReadFldAlign(METRIC_STA_12, METRIC_YMAX);
    UINT32 u1OffsetLR = (u43DGetInputLR() == 0) ? 0 : METER_OFFSET_LR;
    
	vIO32WriteFldAlign(METER_INFO_00+u1OffsetLR, (dwPixCnt) ? (dwLumaSum/dwPixCnt) : 0, APL_VAL);
	vIO32WriteFldAlign(METER_INFO_00+u1OffsetLR, bYMin, LUMA_MIN);
	vIO32WriteFldAlign(METER_INFO_00+u1OffsetLR, bYMax, LUMA_MAX);  
	vIO32WriteFldAlign(METER_INFO_01+u1OffsetLR, dwLumaSum, LUMA_SUM);
	vIO32WriteFldAlign(METER_INFO_02+u1OffsetLR, dwPixCnt, LUMA_PXL_CNT);  
	
	//HW default design is >> 4, and Register setting >> 1 
	vRegWriteFldAlign(METRIC_00, 0, SEE_NEXT_16BIN);
	for (i = 0; i < 8; i++)
	{
		vIO32Write4B(LUMA_HIST_START+u1OffsetLR+4*i, u4RegRead4B(METRIC_STA_00+(i*4))); 	
	}

	vRegWriteFldAlign(METRIC_00, 1, SEE_NEXT_16BIN);
	for (i = 0; i < 8; i++)
	{
		vIO32Write4B(LUMA_HIST_START+u1OffsetLR+4*(i+8), u4RegRead4B(METRIC_STA_00+(i*4)));
	}
}	

UINT8 bDrvGetAPL(void)
{
    UINT32 u1OffsetLR = (u43DGetInputLR() == 0) ? 0 : METER_OFFSET_LR;
    
    return IO32ReadFldAlign(METER_INFO_00+u1OffsetLR, APL_VAL);
}

void vDrvGetAPLLR(UINT8* bAPLL, UINT8* bAPLR)
{   
    *bAPLL = IO32ReadFldAlign(METER_INFO_00, APL_VAL);
    *bAPLR = IO32ReadFldAlign(METER_INFO_00+METER_OFFSET_LR, APL_VAL);
}

/**
 * @brief Get the Luma histogram.
 * @param waHistCur : array to get the histogram
 * @param wBase : Normalizing base
 */
UINT8 u1DrvGetLumaHist(UINT16 waHistCur[LUMA_HIST_NUM], UINT16 u2NormBase)
{	    
	UINT8 bi;
	UINT32 u1OffsetLR = (u43DGetInputLR() == 0) ? 0 : METER_OFFSET_LR;
	
	for (bi = 0; bi < LUMA_HIST_NUM; bi++)
	{	
		waHistCur[bi] = u2IO32Read2B(LUMA_HIST_START+u1OffsetLR+2*bi);
	}	

	if (u2NormBase)
	{
	    return bHistogramNormalization(waHistCur, LUMA_HIST_NUM, u2NormBase);
	}

	return SV_TRUE;
}

void vDrvGetLumaHistLR(UINT16 waHistL[LUMA_HIST_NUM], UINT16 waHistR[LUMA_HIST_NUM], UINT16 u2NormBase)
{
    UINT8 bi;
    
	for (bi = 0; bi < LUMA_HIST_NUM; bi++)
	{	
		waHistL[bi] = u2IO32Read2B(LUMA_HIST_START+2*bi);
		waHistR[bi] = u2IO32Read2B(LUMA_HIST_START+METER_OFFSET_LR+2*bi);
	}	

	if (u2NormBase)
	{
	    bHistogramNormalization(waHistL, LUMA_HIST_NUM, u2NormBase);
	    bHistogramNormalization(waHistR, LUMA_HIST_NUM, u2NormBase);   
	}
}

void vDrvGetInputLumaMaxLR(UINT8* bMaxL, UINT8* bMaxR)
{
    *bMaxL = IO32ReadFldAlign(METER_INFO_00, LUMA_MAX);
	*bMaxR = IO32ReadFldAlign(METER_INFO_00+METER_OFFSET_LR, LUMA_MAX);
}

void vDrvGetInputLumaMinLR(UINT8* bMinL, UINT8* bMinR)
{
    *bMinL = IO32ReadFldAlign(METER_INFO_00, LUMA_MIN);
	*bMinR = IO32ReadFldAlign(METER_INFO_00+METER_OFFSET_LR, LUMA_MIN);
}

// ====================================================================================================
// Output Stage Luma Statistics
// ====================================================================================================
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

// ====================================================================================================
// SCE Histogram 
// ====================================================================================================
void vDrvSetSatHistWindow(UINT16 * u2Window)
{
	if (u2Window == NULL)
	{
		return;
	}

	vRegWriteFldMulti(SAT_HIST_X_CFG_MAIN,
	                  P_Fld(u2Window[0], SAT_WIN_X_START) |
	                  P_Fld(u2Window[2], SAT_WIN_X_END));
	vRegWriteFldMulti(SAT_HIST_Y_CFG_MAIN,
	                  P_Fld(u2Window[1], SAT_WIN_Y_START) |
	                  P_Fld(u2Window[3], SAT_WIN_Y_END));
}

void vDrvSetSatHistBoundary(UINT8* bBoundary)
{
	if (bBoundary == NULL)
	{
		return;
	}

	vRegWriteFldMulti(SAT_HIST_CFG_MAIN,
	                  P_Fld(bBoundary[0], SAT_BOUND_1) |
	                  P_Fld(bBoundary[1], SAT_BOUND_2) |
	                  P_Fld(bBoundary[2], SAT_BOUND_3) |
	                  P_Fld(bBoundary[3], SAT_BOUND_4));
	vRegWriteFldMulti(SAT_HIST_CFG_MAIN_2,
	                  P_Fld(bBoundary[4], SAT_BOUND_5) |
	                  P_Fld(bBoundary[5], SAT_BOUND_6) |
	                  P_Fld(bBoundary[6], SAT_BOUND_7));
}

static void vDrvSatHistInit(void)
{
	UINT16 wActiveWin[4];
    UINT8 u1SatHistBoundary[SAT_HIST_NUM-1] = {1, 4, 9, 16, 25, 36, 49};

	wActiveWin[0] = 16;
	wActiveWin[1] = 16;
	wActiveWin[2] = PANEL_GetPanelWidth()-16;
	wActiveWin[3] = PANEL_GetPanelHeight()-16;

	vDrvSetSatHistWindow(wActiveWin);
	vDrvSetSatHistBoundary(u1SatHistBoundary);	
}

UINT8 u1DrvGetASL(void)
{
    UINT32 u1OffsetLR = (u43DGetOutputLR() == 0) ? 0 : METER_OFFSET_LR;
    
    return IO32ReadFldAlign(METER_INFO_00+u1OffsetLR, ASL_VAL);
}

void vDrvGetSatHistHW(void)
{
	UINT8 i;
	UINT32 u1OffsetLR = (u43DGetOutputLR() == 0) ? 0 : METER_OFFSET_LR;
	
	for (i=0; i<SAT_HIST_NUM/2; i++)
	{
		vIO32Write4B(METER_INFO_07+u1OffsetLR+4*i, u4RegRead4B(SAT_HIST_1_0_MAIN+(i*4))); 	
	}
}

/**
 * @brief Get the chroma histogram from SCE.
 * @param u2aHist : Array to get histogram.
 * @param u2NormBase : Normalization base
 */
UINT8 u1DrvGetSatHist(UINT16 u2aHist[SAT_HIST_NUM], UINT16 u2NormBase)
{
	UINT8 i;
	UINT32 u1OffsetLR = (u43DGetOutputLR() == 0) ? 0 : METER_OFFSET_LR;
	
	for (i=0; i<SAT_HIST_NUM; i++)
	{
		u2aHist[i] = u2IO32Read2B(METER_INFO_07+u1OffsetLR+2*i);
	}

	if (u2NormBase)
	{
		return bHistogramNormalization(u2aHist, SAT_HIST_NUM, u2NormBase);
	}

	return SV_TRUE;
}

void vDrvGetSatHistLR(UINT16 u2HistL[SAT_HIST_NUM], UINT16 u2HistR[SAT_HIST_NUM], UINT16 u2NormBase)
{
    UINT8 bi;
    
	for (bi = 0; bi < SAT_HIST_NUM; bi++)
	{	
		u2HistL[bi] = u2IO32Read2B(METER_INFO_07+2*bi);
		u2HistR[bi] = u2IO32Read2B(METER_INFO_07+METER_OFFSET_LR+2*bi);
	}	
    
    bHistogramNormalization(u2HistL, SAT_HIST_NUM, u2NormBase);
	bHistogramNormalization(u2HistR, SAT_HIST_NUM, u2NormBase);
}

void vDrvSetHueHistBoundary(UINT8* bBoundary)
{
	if (bBoundary == NULL)
	{
		return;
	}

	vRegWriteFldMulti(HUE_HIST_CFG_MAIN,
	                  P_Fld(bBoundary[0], HUE_BOUND_0) |
	                  P_Fld(bBoundary[1], HUE_BOUND_1) |
	                  P_Fld(bBoundary[2], HUE_BOUND_2) |
	                  P_Fld(bBoundary[3], HUE_BOUND_3));
	vRegWriteFldMulti(HUE_HIST_CFG_MAIN_1,
	                  P_Fld(bBoundary[4], HUE_BOUND_4) |
	                  P_Fld(bBoundary[5], HUE_BOUND_5) |
	                  P_Fld(bBoundary[6], HUE_BOUND_6) |
	                  P_Fld(bBoundary[7], HUE_BOUND_7));
}

static void vDrvHueHistInit(void)
{
    // M=56, R=108, Skin=126, Y=174, G=244, C=291, B=353,  (0~360)
    // M=40, R= 77, Skin=89,  Y=124, G=174, C=207, B=251,  (0~256)
    UINT8 u1HueHistBoundary[HUE_HIST_NUM] = {18, 59, 83, 95, 101, 149, 191, 229};

    vDrvSetHueHistBoundary(u1HueHistBoundary);
}

void vDrvGetHueHistHW(void)
{
	UINT8 i;
	UINT32 u1OffsetLR = (u43DGetOutputLR() == 0) ? 0 : METER_OFFSET_LR;
	
	for (i=0; i<HUE_HIST_NUM/2; i++)
	{
		vIO32Write4B(METER_INFO_03+u1OffsetLR+4*i, u4RegRead4B(HUE_HIST_1_0_MAIN+(i*4))); 	
	}
}

/**
 * @brief Get the HUE histogram from SCE.
 * @param u2aHist : Array to get histogram.
 * @param u2NormBase : Normalization base
 */
UINT8 u1DrvGetHueHist(UINT16 u2aHist[HUE_HIST_NUM], UINT16 u2NormBase)
{
	UINT8 i;
	UINT32 u1OffsetLR = (u43DGetOutputLR() == 0) ? 0 : METER_OFFSET_LR;
	
	for (i=0; i<HUE_HIST_NUM; i++)
	{
		u2aHist[i] = u2IO32Read2B(METER_INFO_03+u1OffsetLR+2*i);
	}
	
	if (u2NormBase)
	{
		return bHistogramNormalization(u2aHist, HUE_HIST_NUM, u2NormBase);
	}

    return SV_TRUE;
}

void vDrvGetHueHistLR(UINT16 u2HistL[HUE_HIST_NUM], UINT16 u2HistR[HUE_HIST_NUM], UINT16 u2NormBase)
{
    UINT8 bi;
    
	for (bi = 0; bi < HUE_HIST_NUM; bi++)
	{	
		u2HistL[bi] = u2IO32Read2B(METER_INFO_03+2*bi);
		u2HistR[bi] = u2IO32Read2B(METER_INFO_03+METER_OFFSET_LR+2*bi);
	}	

    if (u2NormBase)
    {
        bHistogramNormalization(u2HistL, HUE_HIST_NUM, u2NormBase);
	    bHistogramNormalization(u2HistR, HUE_HIST_NUM, u2NormBase);
    }
}

// ====================================================================================================
// Saturation / Hue 2D Window  
// ====================================================================================================
void vDrvSetSatHue2DWindowBound(UINT8 bWinSel, UINT8 bSatUpper, UINT8 bSatLower, UINT8 bHueUpper, UINT8 bHueLower)
{
    if (bWinSel < SAT_HUE_2DWIN_NUM) // support 3 window
    {
    	vRegWriteFldMulti(TWO_D_WINDOW_1 + bWinSel*4,
	                  P_Fld(bSatUpper, W1_SAT_UPPER) |
	                  P_Fld(bSatLower, W1_SAT_LOWER) |
	                  P_Fld(bHueUpper, W1_HUE_UPPER) |
	                  P_Fld(bHueLower, W1_HUE_LOWER));
    }
    else
    {
        Printf("Support 3 Window Only\n");
    }
}

static void vDrvSetSatHue2DWinInit(void)
{
    UINT32 i;
    UINT8 bWin[SAT_HUE_2DWIN_NUM][4] = {{   40,  5,  100,   78},     
                                        {   32,  8,   95,   83},
                                        {  255,  0,  192,  128}};

    for (i = 0; i < SAT_HUE_2DWIN_NUM; i++)
    {
        vDrvSetSatHue2DWindowBound(i, bWin[i][0], bWin[i][1], bWin[i][2], bWin[i][3]);
    }
}


void vDrvGetSatHue2DWindowHW(void)
{   
    UINT32 u1OffsetLR = (u43DGetOutputLR() == 0) ? 0 : METER_OFFSET_LR;
    
    vIO32WriteFldAlign(METER_INFO_0B+u1OffsetLR, 
        RegReadFldAlign(TWO_D_W1_RESULT, W1_RESULT)>>2, TWOD_WIN_00);   
    vIO32WriteFldAlign(METER_INFO_0C+u1OffsetLR, 
        RegReadFldAlign(TWO_D_W2_RESULT, W2_RESULT)>>2, TWOD_WIN_01);   
    vIO32WriteFldAlign(METER_INFO_0D+u1OffsetLR, 
        RegReadFldAlign(TWO_D_W3_RESULT, W3_RESULT)>>2, TWOD_WIN_02);    
}

UINT32 u4DrvGetTotalPixelCount(void)
{
    UINT32 u4W = RegReadFldAlign(SAT_HIST_X_CFG_MAIN, SAT_WIN_X_END) 
                 - RegReadFldAlign(SAT_HIST_X_CFG_MAIN, SAT_WIN_X_START);
    UINT32 u4H = RegReadFldAlign(SAT_HIST_Y_CFG_MAIN, SAT_WIN_Y_END) 
                 - RegReadFldAlign(SAT_HIST_Y_CFG_MAIN, SAT_WIN_Y_START);
    
    return (u4W*u4H);    
}

UINT32 u4DrvGetSatHue2DWindowResult(UINT8 bWinSel, UINT16 u2NormBase)
{   
    UINT32 u4WinPxlCnt = 0;   
    UINT32 u1OffsetLR = (u43DGetOutputLR() == 0) ? 0 : METER_OFFSET_LR;
    UINT32 u4TotalPxlCnt = u4DrvGetTotalPixelCount() >> 2;
   
    if (bWinSel < SAT_HUE_2DWIN_NUM) // support 3 window
    {    
        u4WinPxlCnt = IO32ReadFldAlign(METER_INFO_0B+u1OffsetLR+bWinSel*4, TWOD_WIN_00);
    }
    else
    {
        return 0;
    }

    if (u2NormBase != 0)
	{
	    if (u4TotalPxlCnt != 0)    
	    {
            return (u4WinPxlCnt*u2NormBase)/u4TotalPxlCnt;
	    }
	    else
	    {
            return 0;
	    }
    }
    else
    {
        return u4WinPxlCnt;
    }
}

void vDrvGet2DWindowLR(UINT16 u2WinL[3], UINT16 u2WinR[3], UINT16 u2NormBase)
{
    UINT32 u4TotalPxlCnt = u4DrvGetTotalPixelCount() >> 2;
    
    if ((u4TotalPxlCnt != 0) && (u2NormBase != 0))
    { 
        u2WinL[0] = (IO32ReadFldAlign(METER_INFO_0B, TWOD_WIN_00)*u2NormBase)/u4TotalPxlCnt;
        u2WinL[1] = (IO32ReadFldAlign(METER_INFO_0C, TWOD_WIN_01)*u2NormBase)/u4TotalPxlCnt;
        u2WinL[2] = (IO32ReadFldAlign(METER_INFO_0D, TWOD_WIN_02)*u2NormBase)/u4TotalPxlCnt;
        u2WinR[0] = (IO32ReadFldAlign(METER_INFO_0B+METER_OFFSET_LR, TWOD_WIN_00)*u2NormBase)/u4TotalPxlCnt;
        u2WinR[1] = (IO32ReadFldAlign(METER_INFO_0C+METER_OFFSET_LR, TWOD_WIN_01)*u2NormBase)/u4TotalPxlCnt;
        u2WinR[2] = (IO32ReadFldAlign(METER_INFO_0D+METER_OFFSET_LR, TWOD_WIN_02)*u2NormBase)/u4TotalPxlCnt;
    }
}

// ====================================================================================================
// Output Stage RGB Histogram 
// ====================================================================================================
void vDrvSetRGBMaxBoundary(UINT8 * bBoundary)
{
#if !defined(CC_MT5395)    
	UINT8 i = 0;

	if (bBoundary == NULL)
	{
		return;
	}

	for (i = 0; i < RGB_MAX_HIST_NUM-1; i++)
	{
		vRegWrite1B(OS_36+i, bBoundary[i]);
	}
#endif	
}

static void vDrvSetRGBMaxWindow(UINT16* u2Window)
{
	if (u2Window == NULL)
	{
		return;
	}

	vRegWriteFldMulti(RGB_HIST_BOUND_H,
	                  P_Fld(u2Window[0], MAX_METER_HSTART) |
	                  P_Fld(u2Window[2], MAX_METER_HEND));
	vRegWriteFldMulti(RGB_HIST_BOUND_V,
	                  P_Fld(u2Window[1], MAX_METER_VSTART) |
	                  P_Fld(u2Window[3], MAX_METER_VEND));
}

static void vDrvRGBMaxInit(void)
{
	UINT16 wActiveWin[4];
	
	wActiveWin[0] = 16;
	wActiveWin[1] = 16;
	wActiveWin[2] = PANEL_GetPanelWidth()-16;
	wActiveWin[3] = PANEL_GetPanelHeight()-16;

	vDrvSetRGBMaxWindow(wActiveWin);

#if !defined(CC_MT5395)
    UINT8 bRgbHistBoundary[RGB_MAX_HIST_NUM-1] =
	{
        8	,   16	,   24	,   32	,   40	,   48	,   56	,   64	,
        72	,   80	,   88	,   96	,   104	,   112	,   120	,   128	,
        136	,   144	,   152	,   160	,   168	,   176	,   184	,   192	,
        200	,   208	,   216	,   224	,   232	,   240	,   248
	};

	vDrvSetRGBMaxBoundary(bRgbHistBoundary);
#endif // 5395 don't need to initialize boundary
}

UINT8 u1DrvGetRGBHist(UINT16 u2aHist[RGB_MAX_HIST_NUM], UINT16 u2NormBase)
{
    UINT32 i;
    
    for (i = 0; i< RGB_MAX_HIST_NUM; i++)
    {
        u2aHist[i] = RegReadFldAlign(RGB_HIST_CNT_00 + (i * 4), CNT00)>>5;
    }

    if (u2NormBase)
	{
		return bHistogramNormalization(u2aHist, RGB_MAX_HIST_NUM, u2NormBase);
	}

    return SV_TRUE;
}


// ====================================================================================================
// CONTENT METER
// ====================================================================================================
void vDrvContMeterOnOff(UINT8 bOnOff)
{
    vRegWriteFldAlign(CONTENT_METER_00, bOnOff, CONTMETER_EN);
}

void vDrvContMeterInit(void)
{
    vDrvContMeterOnOff(SV_OFF);

    bContMeterIIR = 20;
    vRegWriteFldMulti(CONTENT_METER_13,
                      P_Fld(250, CONTMETER_HCNTSTART) |
                      P_Fld(PANEL_GetPanelWidth() - 250, CONTMETER_HCNTEND));
    vRegWriteFldMulti(CONTENT_METER_14,
                      P_Fld(100, CONTMETER_VCNTSTART) |
                      P_Fld(PANEL_GetPanelHeight() - 100, CONTMETER_VCNTEND));
}

void vDrvContMeterProc(void)
{
    UINT8 i;
	UINT16 wDiff, wContMeterTh[8];
   	UINT32 dwContMeterHis[9], dwDiffSum[3];

    if ((bContMeterCnt % 30 != 0)
        || (RegReadFldAlign(CONTENT_METER_00, CONTMETER_EN) == 0))
	{
	    bContMeterCnt++;
		return;
	}

	wDiff = RegReadFldAlign(CONTENT_METER_12, CONTMETER_MAX)
	        - RegReadFldAlign(CONTENT_METER_12, CONTMETER_MIN);

	for (i = 0; i < 8 ; i ++)
	{
		wContMeterTh[i] = wDiff * CONTENT_METER_TH[i] >> 5;
	}

	vRegWriteFldMulti(CONTENT_METER_00,
	                  P_Fld(wContMeterTh[2], CONTMETER_THRE02) |
	                  P_Fld(wContMeterTh[1], CONTMETER_THRE01) |
	                  P_Fld(wContMeterTh[0], CONTMETER_THRE00));
	vRegWriteFldMulti(CONTENT_METER_01,
	                  P_Fld(wContMeterTh[5], CONTMETER_THRE05) |
	                  P_Fld(wContMeterTh[4], CONTMETER_THRE04) |
	                  P_Fld(wContMeterTh[3], CONTMETER_THRE03));
	vRegWriteFldMulti(CONTENT_METER_02,
	                  P_Fld(wContMeterTh[7], CONTMETER_THRE07) |
	                  P_Fld(wContMeterTh[6], CONTMETER_THRE06));

    for (i = 0; i < 9 ; i ++)
    {
    	dwContMeterHis[i] = RegReadFldAlign(CONTENT_METER_03+4*i, CONTMETER_CNT00);
    }
    
	dwDiffSum[0] = 0;
	dwDiffSum[1] = 0;

	for (i = 2; i < 9 ; i ++)
	{
		dwDiffSum[0] += dwContMeterHis[i];
	}

	for (i = 5; i < 9 ; i ++)
	{
		dwDiffSum[1] += dwContMeterHis[i];
	}

	if (dwDiffSum[0] >= 15)
	{
		if (bContMeterIIR < 60)
		{
			bContMeterIIR ++;
			if (bContMeterIIR >= 45)
			{
				bIsHD = SV_TRUE;
			}
		}
	}
	else
	{
		if (bContMeterIIR != 0)
		{
			bContMeterIIR --;
			if (bContMeterIIR <= 15)
			{
				bIsHD = SV_FALSE;
			}
		}
	}

	// Status Log
	LOG(3,"=======================\n");
	LOG(3,"dwDiffSum[0] = %d\n", dwDiffSum[0]);
	LOG(3,"dwDiffSum[1] = %d\n", dwDiffSum[1]);

	for (i = 0; i < 9; i ++)
	{
		LOG(3,"CONTMETER_HIS[%d] = %d\n", i, dwContMeterHis[i]);
	}

	LOG(3,"bContMeterIIR = %d\n", bContMeterIIR);
	LOG(3,"HD = %d\n", bIsHD);
}

UINT8 vDrvContMeterIsHD(void)
{
	return (bIsHD);
}


// ====================================================================================================
// CONTENT METER
// ====================================================================================================
void vDrvMeterInit(void)
{    
    vDrvSatHistInit();
    vDrvHueHistInit();
    vDrvRGBMaxInit();
    vDrvSetSatHue2DWinInit();
    vDrvContMeterInit();
}

void vDrvMeterProc(void)
{
    UINT32 i, u4SatSum = 0;
    UINT16 u2aSatHist[SAT_HIST_NUM];
    UINT32 u1OffsetLR = (u43DGetOutputLR() == 0) ? 0 : METER_OFFSET_LR;
    
    if (IO32ReadFldAlign(ADAPTIVE_REG, ADAPTIVE_GETINFO) == 0)
    {
        return;
    }

    // copy data to software register
    vDrvGetHueHistHW();
    vDrvGetSatHistHW();
    vDrvGetSatHue2DWindowHW();
    
    // calculate the Average saturation level
    u1DrvGetSatHist(u2aSatHist, 255);

    for (i = 0; i < SAT_HIST_NUM; i++)
    {
        u4SatSum += (u2aSatHist[i] * (i+1) * (i+1));
    }
    vIO32WriteFldAlign(METER_INFO_00+u1OffsetLR, u4SatSum>>6, ASL_VAL);

    vIO32WriteFldAlign(METER_INFO_0E+u1OffsetLR, u4DrvGetSatHue2DWindowResult(0, 255), TWOD_WIN_NORM_00);
    vIO32WriteFldAlign(METER_INFO_0E+u1OffsetLR, u4DrvGetSatHue2DWindowResult(1, 255), TWOD_WIN_NORM_01);
    vIO32WriteFldAlign(METER_INFO_0E+u1OffsetLR, u4DrvGetSatHue2DWindowResult(2, 255), TWOD_WIN_NORM_02);
    
    vDrvContMeterProc();
}

