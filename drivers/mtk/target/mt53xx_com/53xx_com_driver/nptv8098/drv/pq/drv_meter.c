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
#include "vdo_misc.h"
#include "x_assert.h"
#include "nptv_debug.h"
#include "drv_scaler_drvif.h"
#include "video_def.h"
#include "drv_scaler.h"
#include "drv_lbox.h"



// === HW INCLUDE =============================================================================

// === DEFINE =============================================================================
// ADL Debug Mode define
#define	METER_DBG_WINDOW_INFO	    (1<<0)
#define	METER_DBG_3D_INFO	    (1<<1)



#define METER_LOG_CHECK(x)    ((bMeterDbgCur==bMeterDbgCount) && (bMeterDbgIndex & x))
#define METER_LOG_CHECK2(x)   (bMeterDbgIndex & x)

// === FUNCTION PROTOTYPE =================================================================

// === EXTERN FUNCTION ====================================================================

// === GLOBAL VARIABLE ====================================================================
UINT8 u1WindowLBoxEnable = 0;
UINT8 u1SatWinInit = 0;
UINT8 bIsSrcLRflg = 0;
UINT8 bHistWindowLRflg = 0;
UINT8 bHistWindowTBflg = 0;


// === EXTERN VARIABLE ====================================================================
extern UINT8 _u1LBoxFinalUpdate;	

// === STATIC VARIABLE ====================================================================
static UINT16 bMeterDbgIndex;
static UINT8 bMeterDbgCount;
static UINT8 bMeterDbgCur;

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
    UINT32 HistAcc[LUMA_HIST_NUM_WEIGHT];

    if (bDim > LUMA_HIST_NUM_WEIGHT)
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
UINT8 bDrvGetAPL(void)
{
    UINT32 u1OffsetLR = (u43DGetOutputLR() == 0) ? 0 : METER_OFFSET_LR;
    
    return IO32ReadFldAlign(METER_INFO_00+u1OffsetLR, METER_APL_VAL);
}

void vDrvGetAPLLR(UINT8* bAPLL, UINT8* bAPLR)
{   
    *bAPLL = IO32ReadFldAlign(METER_INFO_00, METER_APL_VAL);
    if(u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF)    
    {
        *bAPLR = IO32ReadFldAlign(METER_INFO_00+METER_OFFSET_LR, METER_APL_VAL);
    }
    else
    {
        *bAPLR = 0;
    }
}

UINT8 u1DrvGetLumaHist(UINT16 waHistCur[LUMA_HIST_NUM_WEIGHT], UINT16 u2NormBase)
{	    
	UINT8 bi;
	UINT32 u1OffsetLR = (u43DGetOutputLR() == 0) ? 0 : METER_OFFSET_LR;
	
	for (bi = 0; bi < LUMA_HIST_NUM_WEIGHT; bi++)
	{	
		waHistCur[bi] = u2IO32Read2B(LUMA_HIST_START+u1OffsetLR+2*bi);
	}	

	if (u2NormBase)
	{
	    return bHistogramNormalization(waHistCur, LUMA_HIST_NUM_WEIGHT, u2NormBase);
	}

	return SV_TRUE;
}

void vDrvGetLumaHistLR(UINT16 waHistL[LUMA_HIST_NUM_WEIGHT], UINT16 waHistR[LUMA_HIST_NUM_WEIGHT], UINT16 u2NormBase)
{
    UINT8 bi;
    
    if(u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF)    
    {
    	for (bi = 0; bi < LUMA_HIST_NUM_WEIGHT; bi++)
    	{	
    		waHistL[bi] = u2IO32Read2B(LUMA_HIST_START+2*bi);
    		waHistR[bi] = u2IO32Read2B(LUMA_HIST_START+METER_OFFSET_LR+2*bi);
    	}
    }
    else
    {
    	for (bi = 0; bi < LUMA_HIST_NUM_WEIGHT; bi++)
    	{	
    		waHistL[bi] = u2IO32Read2B(LUMA_HIST_START+2*bi);            
    		waHistR[bi] = 0;
    	}	
    }
    
	if (u2NormBase)
	{
	    bHistogramNormalization(waHistL, LUMA_HIST_NUM_WEIGHT, u2NormBase);
	    bHistogramNormalization(waHistR, LUMA_HIST_NUM_WEIGHT, u2NormBase);   
	}
}

void vDrvGetInputLumaMaxLR(UINT8* bMaxL, UINT8* bMaxR)
{
    if(u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF)    
    {
        *bMaxL = IO32ReadFldAlign(METER_INFO_00, METER_LUMA_MAX);
    	*bMaxR = IO32ReadFldAlign(METER_INFO_00+METER_OFFSET_LR, METER_LUMA_MAX);
    }
    else
    {
        *bMaxL = IO32ReadFldAlign(METER_INFO_00, METER_LUMA_MAX);
    	*bMaxR = 0;
    }
}

void vDrvGetInputLumaMinLR(UINT8* bMinL, UINT8* bMinR)
{
    if(u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF)  
    {
        *bMinL = IO32ReadFldAlign(METER_INFO_00, METER_LUMA_MIN);
    	*bMinR = IO32ReadFldAlign(METER_INFO_00+METER_OFFSET_LR, METER_LUMA_MIN);
    }
    else
    {
        *bMinL = IO32ReadFldAlign(METER_INFO_00, METER_LUMA_MIN);
    	*bMinR = 0;
    }
}

// ====================================================================================================
// Output Stage Luma Statistics
// ====================================================================================================
UINT8 bDrvGetOutputLumaMax(UINT8 bPath)
{
    #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    return 0;
    #else
    if (bPath == SV_VP_MAIN)
    {
        return RegReadFldAlign(OUTSTG_RO_79, RO_79_M_YMAX);
    }
    else
    {
        return RegReadFldAlign(OUTSTG_RO_79, RO_79_P_YMAX);
    }
    #endif
}

UINT8 bDrvGetOutputLumaMin(UINT8 bPath)
{
    #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    return 0;
    #else
    if (bPath == SV_VP_MAIN)
    {
        return RegReadFldAlign(OUTSTG_RO_79, RO_79_M_YMIN);
    }
    else
    {
        return RegReadFldAlign(OUTSTG_RO_79, RO_79_P_YMIN);
    }
    #endif
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

void vDrvGetSatHistWindow(UINT16 * u2Window)
{
	if (u2Window == NULL)
	{
		return;
	}
    u2Window[0] = RegReadFldAlign(SAT_HIST_X_CFG_MAIN, SAT_WIN_X_START);
    u2Window[1] = RegReadFldAlign(SAT_HIST_Y_CFG_MAIN, SAT_WIN_Y_START);
    u2Window[2] = RegReadFldAlign(SAT_HIST_X_CFG_MAIN, SAT_WIN_X_END);
    u2Window[3] = RegReadFldAlign(SAT_HIST_Y_CFG_MAIN, SAT_WIN_Y_END);
}

void vDrvSatHistWinReconfig(void)
{
    u1SatWinInit = 0;
}

extern UINT32 _u4LBoxDetectionEnable;
extern UINT32 _u4LBoxDetectionValid;

static void vDrvSatHistWinCalculate(UINT16 *u2Window)
{
    UINT32 u4LBoxXStart = 0;
    UINT32 u4LBoxYStart = 0;
    UINT32 u4LBoxWidth = 0;
    UINT32 u4LBoxHeight = 0;
    UINT32 u4LBoxXEnd = 0;
    UINT32 u4LBoxYEnd = 0;    
    static UINT16 u2WindowRecord[4];
    RDispInfo* dispInfo;
	dispInfo = getDispInfo(SV_VP_MAIN);

  
    if(u1SatWinInit == 0)
    {
		u2WindowRecord[0] = 16;
		u2WindowRecord[1] = 16;
		u2WindowRecord[2] = dispInfo->wWidth - 16;
		u2WindowRecord[3] = dispInfo->wHeight - 16;
		u1SatWinInit = 1;
    }

    if((u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_OFF) || (u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_FS))
    {
        if(u1WindowLBoxEnable)
        {
            LboxSetOnOff(LBOX_ONOFF_ADAPTIVE_LUMA, 1, 0);
            if(u1LBoxCalculateDispRegion(&u4LBoxXStart, &u4LBoxYStart, &u4LBoxWidth, &u4LBoxHeight) && (_u1LBoxFinalUpdate != 0))
            {
                u4LBoxXEnd = ((u4LBoxXStart + u4LBoxWidth) < (dispInfo->wWidth) ? (u4LBoxXStart + u4LBoxWidth) : (dispInfo->wWidth));// -16;    >dispInfo->wXOffset + dispInfo->wWidth = dispInfo->wXOffset + dispInfo->wWidth
                u4LBoxYEnd = ((u4LBoxYStart + u4LBoxHeight) < (dispInfo->wHeight) ?(u4LBoxYStart + u4LBoxHeight) : (dispInfo->wHeight));//-16;
        
                if((u4LBoxXEnd > u4LBoxXStart) && (u4LBoxYEnd > u4LBoxYStart))  //valid region
                {
                    u2WindowRecord[0] = u4LBoxXStart + 16;
                    u2WindowRecord[1] = u4LBoxYStart + 16;
                    u2WindowRecord[2] = u4LBoxXEnd - 16;
                    u2WindowRecord[3] = u4LBoxYEnd - 16;
                }
                else
                {
                    u2WindowRecord[0] = 16;
                    u2WindowRecord[1] = 16;
                    u2WindowRecord[2] = dispInfo->wWidth - 16;
                    u2WindowRecord[3] = dispInfo->wHeight - 16;
                }  
            }        
            else
            {
                u2WindowRecord[0] = 16;
                u2WindowRecord[1] = 16;
                u2WindowRecord[2] = dispInfo->wWidth - 16;
                u2WindowRecord[3] = dispInfo->wHeight - 16;
            }  
        }
        else
        {
            LboxSetOnOff(LBOX_ONOFF_ADAPTIVE_LUMA, 0, 0);
            u2WindowRecord[0] = 16;
            u2WindowRecord[1] = 16;
            u2WindowRecord[2] = dispInfo->wWidth - 16;
            u2WindowRecord[3] = dispInfo->wHeight - 16;
        }

    }
#ifdef CC_MT5396    
    else if(u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_SBS)
    {
        if(bHistWindowLRflg == 0) //left
        {
            u2WindowRecord[0] = 16;
            u2WindowRecord[1] = 16;
            u2WindowRecord[2] = dispInfo->wWidth / 2 - 16;
            u2WindowRecord[3] = dispInfo->wHeight - 16;
            bHistWindowLRflg = 1;
        }
        else    //right
        {
            u2WindowRecord[0] = dispInfo->wWidth / 2 + 16;
            u2WindowRecord[1] = 16;
            u2WindowRecord[2] = dispInfo->wWidth - 16;
            u2WindowRecord[3] = dispInfo->wHeight - 16;
            bHistWindowLRflg = 0;
        }
        
    }
    else if(u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_TB)
    {
        if(bHistWindowTBflg == 0) //top
        {
            u2WindowRecord[0] = 16;
            u2WindowRecord[1] = 16;
            u2WindowRecord[2] = dispInfo->wWidth - 16;
            u2WindowRecord[3] = dispInfo->wHeight / 2 - 16;
            bHistWindowTBflg = 1;
        }
        else    //bottom
        {
            u2WindowRecord[0] = 16;
            u2WindowRecord[1] = dispInfo->wHeight / 2 + 16;
            u2WindowRecord[2] = dispInfo->wWidth - 16;
            u2WindowRecord[3] = dispInfo->wHeight - 16;
            bHistWindowTBflg = 0;
        }
    }
#endif    
    else
    {
        u2WindowRecord[0] = 16;
        u2WindowRecord[1] = 16;
        u2WindowRecord[2] = dispInfo->wWidth - 16;
        u2WindowRecord[3] = dispInfo->wHeight - 16;
    }


    u2Window[0] = u2WindowRecord[0];
    u2Window[1] = u2WindowRecord[1];
    u2Window[2] = u2WindowRecord[2];
    u2Window[3] = u2WindowRecord[3];

    if(METER_LOG_CHECK(METER_DBG_3D_INFO))
    {  
        LOG(0, "Current 3D Format is: ");

        if(u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_OFF)
        {
            LOG(0, "3D off \n");
        }
        else if(u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_FS)
        {
            LOG(0, "FS \n");
        }
        else if(u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_SBS)
        {
            LOG(0, "SBS \n");
            LOG(0, "Current frame is %d \n", bHistWindowLRflg);
        }
        else if(u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_TB)
        {
            LOG(0, "TB \n");
            LOG(0, "Current frame is %d \n", bHistWindowTBflg);            
        }
        else
        {
            LOG(0, "Others \n");
        }
    }      
    if(METER_LOG_CHECK(METER_DBG_WINDOW_INFO))
    {        
    	LOG(0, "X_START = %d, Y_START = %d X_END = %d ,Y_END = %d \n", u2Window[0], u2Window[1], u2Window[2], u2Window[3]);
    }        
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

static void vDrvSatHistWinUpdate(void)
{
	UINT16 wActiveWin[4];

	vDrvSatHistWinCalculate(wActiveWin);
	vDrvSetSatHistWindow(wActiveWin);
}

/**
 * @notice function called in InputVsync
 */
void vDrvGetLumaInfoHW(void)
{
	UINT32 i;
    UINT32 dwHcount = (UINT32)RegReadFldAlign(SAT_HIST_X_CFG_MAIN, SAT_WIN_X_END) - (UINT32)RegReadFldAlign(SAT_HIST_X_CFG_MAIN, SAT_WIN_X_START);
    UINT32 dwVcount = (UINT32)RegReadFldAlign(SAT_HIST_Y_CFG_MAIN, SAT_WIN_Y_END) - (UINT32)RegReadFldAlign(SAT_HIST_Y_CFG_MAIN, SAT_WIN_Y_START);
	UINT32 dwPixCnt = dwHcount*dwVcount;
	UINT32 dwLumaSum = 0;
	UINT32 dwSatSum = 0;
	UINT16 bYMin = 0;
	UINT16 bYMax = 0;
    static UINT8 bLRInfoPre = 0;
    UINT8 u1OffsetLR = 0;

    if((u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_OFF) || (u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_FS))
    {
        u1OffsetLR = (u43DGetOutputLR() == 0) ? 0 : METER_OFFSET_LR;
    }
#ifdef CC_MT5396    
    else if(u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_SBS)
    {
        u1OffsetLR = (bHistWindowLRflg == 0) ? 0 : METER_OFFSET_LR;
    }
    else if(u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_TB)
    {
        u1OffsetLR = (bHistWindowTBflg == 0) ? 0 : METER_OFFSET_LR;
    }
#endif    
//	vDrvSatHistInit();
    u1WindowLBoxEnable = IO32ReadFldAlign(METER_INFO_01, METER_LUMA_LBOX);

	vDrvSatHistWinUpdate();

	dwLumaSum = u4RegRead4B(LUMA_SUM_MAIN);    
	dwSatSum = u4RegRead4B(SAT_SUM_MAIN);
	bYMin = RegReadFldAlign(LUMA_MIN_MAX_MAIN, SCE_LUMA_MIN); //10b
	bYMax = RegReadFldAlign(LUMA_MIN_MAX_MAIN, SCE_LUMA_MAX); //10b

    if(bLRInfoPre != (UINT8)(u43DGetOutputLR()))
    {
        bIsSrcLRflg = 1;
    }
    else
    {
        bIsSrcLRflg = 0; //not FS content
    }
    bLRInfoPre = (UINT8)(u43DGetOutputLR());

	vIO32WriteFldAlign(METER_INFO_00+u1OffsetLR, (dwPixCnt) ? (dwLumaSum/dwPixCnt/4) : 0, METER_APL_VAL);    
	vIO32WriteFldAlign(METER_INFO_00+u1OffsetLR, (dwPixCnt) ? (dwSatSum/dwPixCnt/4) : 0, METER_ASL_VAL);
	vIO32WriteFldAlign(METER_INFO_00+u1OffsetLR, bYMin>>2, METER_LUMA_MIN);    //10b->8b
	vIO32WriteFldAlign(METER_INFO_00+u1OffsetLR, bYMax>>2, METER_LUMA_MAX);	//10b->8b
	vIO32WriteFldAlign(METER_INFO_01+u1OffsetLR, dwLumaSum, METER_LUMA_SUM);    
	vIO32WriteFldAlign(METER_INFO_0E+u1OffsetLR, dwSatSum, METER_SAT_SUM);
	vIO32WriteFldAlign(METER_INFO_02+u1OffsetLR, dwPixCnt, METER_LUMA_PXL_CNT);  
	vIO32WriteFldAlign(METER_INFO_01, bIsSrcLRflg, METER_LUMA_LR);  

	for (i = 0; i < LUMA_HIST_NUM_WEIGHT; i++)
	{
		vIO32Write2B(LUMA_HIST_START+u1OffsetLR+2*i, u4RegRead4B(LUMA_HIST_00+(i*4))>>7);	
	}
}

UINT8 u1DrvGetASL(void)
{
    UINT32 u1OffsetLR = (u43DGetOutputLR() == 0) ? 0 : METER_OFFSET_LR;
    
    return IO32ReadFldAlign(METER_INFO_00+u1OffsetLR, METER_ASL_VAL);
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

UINT8 u1DrvGetHueHist(UINT16 u2aHist[HUE_HIST_NUM], UINT32 *u4PixNumNorm, UINT16 u2NormBase)
{
	UINT8 i;
	UINT32 u1OffsetLR = (u43DGetOutputLR() == 0) ? 0 : METER_OFFSET_LR;
	UINT32 u4PixCnt = 0;
    UINT32 u4TotalCnt = 0;
    UINT16 u2ActiveWin[4];

    vDrvGetSatHistWindow(u2ActiveWin);
    u4TotalCnt = (u2ActiveWin[2] - u2ActiveWin[0])*(u2ActiveWin[3] - u2ActiveWin[1]);
	
	for (i=0; i<HUE_HIST_NUM; i++)
	{
		u2aHist[i] = u2IO32Read2B(METER_INFO_03+u1OffsetLR+2*i);
        u4PixCnt += u2aHist[i];
	}
	
	if (u2NormBase)
	{
        *u4PixNumNorm = u4PixCnt*u2NormBase/(u4TotalCnt>>6);
		return bHistogramNormalization(u2aHist, HUE_HIST_NUM, u2NormBase);
	}
    else
    {
        *u4PixNumNorm = u4PixCnt;
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
    // avoid overflow when normalization = 4096
    vIO32WriteFldAlign(METER_INFO_0B+u1OffsetLR, 
        RegReadFldAlign(TWO_D_W1_RESULT, W1_RESULT)>>2, METER_TWOD_WIN_00);   
    vIO32WriteFldAlign(METER_INFO_0C+u1OffsetLR, 
        RegReadFldAlign(TWO_D_W2_RESULT, W2_RESULT)>>2, METER_TWOD_WIN_01);   
    vIO32WriteFldAlign(METER_INFO_0D+u1OffsetLR, 
        RegReadFldAlign(TWO_D_W3_RESULT, W3_RESULT)>>2, METER_TWOD_WIN_02);    
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
        u4WinPxlCnt = IO32ReadFldAlign(METER_INFO_0B+u1OffsetLR+bWinSel*4, METER_TWOD_WIN_00);
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
        u2WinL[0] = (IO32ReadFldAlign(METER_INFO_0B, METER_TWOD_WIN_00)*u2NormBase)/u4TotalPxlCnt;
        u2WinL[1] = (IO32ReadFldAlign(METER_INFO_0C, METER_TWOD_WIN_01)*u2NormBase)/u4TotalPxlCnt;
        u2WinL[2] = (IO32ReadFldAlign(METER_INFO_0D, METER_TWOD_WIN_02)*u2NormBase)/u4TotalPxlCnt;
        u2WinR[0] = (IO32ReadFldAlign(METER_INFO_0B+METER_OFFSET_LR, METER_TWOD_WIN_00)*u2NormBase)/u4TotalPxlCnt;
        u2WinR[1] = (IO32ReadFldAlign(METER_INFO_0C+METER_OFFSET_LR, METER_TWOD_WIN_01)*u2NormBase)/u4TotalPxlCnt;
        u2WinR[2] = (IO32ReadFldAlign(METER_INFO_0D+METER_OFFSET_LR, METER_TWOD_WIN_02)*u2NormBase)/u4TotalPxlCnt;
    }
}

// ====================================================================================================
// Output Stage RGB Histogram 
// ====================================================================================================
static void vDrvSetRGBMaxWindow(UINT16* u2Window)
{
	if (u2Window == NULL)
	{
		return;
	}

	vRegWriteFldMulti(MAXMETER_00,
	                  P_Fld(u2Window[0], C_MAX_METER_X_START) |
	                  P_Fld(u2Window[2], C_MAX_METER_X_END));
	vRegWriteFldMulti(MAXMETER_01,
	                  P_Fld(u2Window[1], C_MAX_METER_Y_START) |
	                  P_Fld(u2Window[3], C_MAX_METER_Y_END));
}

static void vDrvRGBMaxInit(void)
{
	UINT16 wActiveWin[4];
	
	wActiveWin[0] = 16;
	wActiveWin[1] = 16;
	wActiveWin[2] = PANEL_GetPanelWidth()-16;
	wActiveWin[3] = PANEL_GetPanelHeight()-16;

	vDrvSetRGBMaxWindow(wActiveWin);
}

UINT8 u1DrvGetRGBHist(UINT16 u2aHist[RGB_MAX_HIST_NUM], UINT16 u2NormBase)
{
    UINT32 i;
    
    for (i = 0; i< RGB_MAX_HIST_NUM; i++)
    {
        u2aHist[i] = RegReadFldAlign(MAXMETER_02 + (i * 4), CNT00)>>5;
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
void vDrvMeterInit(void)
{
    vDrvSatHistInit();
    vDrvHueHistInit();
    vDrvRGBMaxInit();
    vDrvSetSatHue2DWinInit();
}

void vDrvMeterDebug(void)
{   
    bMeterDbgIndex       = IO32ReadFldAlign(METER_DEBUG, METER_DEBUG_INDEX);    
    bMeterDbgCount       = IO32ReadFldAlign(METER_DEBUG, METER_DEBUG_COUNT);
    bMeterDbgCur         = (bMeterDbgCur<bMeterDbgCount) ? (bMeterDbgCur+1) : 0;     
}

void vDrvMeterProc(void)
{   
    if (IO32ReadFldAlign(ADAPTIVE_REG, ADAPTIVE_GETINFO) == 0)
    {
        return;
    }

    // copy data to software register
    vDrvGetHueHistHW();
    vDrvGetSatHistHW();
    vDrvGetSatHue2DWindowHW();    
    vDrvMeterDebug();
}


