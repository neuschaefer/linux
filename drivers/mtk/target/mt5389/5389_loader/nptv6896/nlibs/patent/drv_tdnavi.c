/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: drv_mjc_int.c $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/

#include "x_assert.h"
#include "hw_tdnavi.h"
#include "drv_tdnavi.h"
#include "vdo_misc.h"
#include "sv_const.h"
#include "vdp_if.h"
#include "mute_if.h"

#ifdef CC_MT5396
#include "drv_mjc.h"
#endif

/*----------------------------------------------------------------------------*
 * Global/Static variables
 *----------------------------------------------------------------------------*/

//#define TD_NAVI_DETECT_FRAME_NUM	30 // 1~255
//#define TD_NAVI_2D_TO_3D_D_TH		218 // 1~255
//#define TD_NAVI_3D_TO_2D_D_TH		160 // 1~255

UINT8 g_u1TdNaviDetInFrmNum = 5;
UINT8 g_u1In3DSimilarityRatio = 218;
UINT8 g_u1In3DSimilarThr= 5;
UINT8 g_u1In3DDecisionType= 1;
UINT8 g_u1VldDivInVldThr= 13; // 0~255 means (0~255)/16


UINT8 g_u1TdNaviDetOutFrmNum = 3;
#if SUPPORT_FW_FRAME_DET_TYPE
UINT8 g_u1Out3DSimilarityRatio = 218;//160; // RealD Test Pattern = 136
UINT8 g_u1Out3DSimilarThr = 5;//10;
UINT8 g_u1Out3DDecisionType = 1;// 2;
#else
UINT8 g_u1Out3DSimilarityRatio = 160; // RealD Test Pattern = 136
UINT8 g_u1Out3DSimilarThr = 10;
UINT8 g_u1Out3DDecisionType = 2;
#endif

#if SUPPORT_FW_FRAME_DET_TYPE
UINT8 g_u1SbsTabLut[9] = {0, 24, 32, 48, 72, 88, 112, 144, 176};
#endif	

UINT8 g_u1TdNaviDetNew = 0;
UINT8 g_u1TdNaviDetCurr = 0;
UINT8 g_u1TdNaviDet2DCnt = 0;
UINT8 g_u1TdNaviDetSbsCnt = 0;
UINT8 g_u1TdNaviDetTabCnt = 0;

UINT8 g_u1TdNaviAdaptOnOff = 1;
UINT8 g_u1TdNaviPrint = 10;
UINT8 g_u1TdNaviTrigger = 0;

UINT8 g_u1RealDInDiffThr[2] = {75, 75};
UINT8 g_u1RealDOutDiffThr[2] = {55, 55};
UINT8 g_u1RealDInFrmThr[2] = {15, 15};
UINT8 g_u1RealDOutFrmThr[2] = {3, 3};
UINT8 g_u1RealDInFrmCnt[2] = {0, 0};
UINT8 g_u1RealDOutFrmCnt[2] = {0, 0};
UINT16 g_u2RealDInQuaThr[2] = {1800, 1800}; // depend on Input Width 
UINT16 g_u2RealDOutQuaThr[2] = {1800, 1800}; // depend on Input Width
UINT16 g_u2RealDInQuaThrRatio = 200; // 200 means 200/256
UINT16 g_u2RealDOutQuaThrRatio = 200; // 200 means 200/256

UINT8 g_u1RealDDetCurr[2] = {0, 0};
UINT8 g_u1RealDAdaptEn[2] = {1, 1};

UINT16 g_u2RealDPrint = 100;
UINT8 g_u1RealDTrigger = 0;

#if SUPPORT_FW_FRAME_DET_TYPE
UINT8 g_u1FrameDet3dHiTh = 96;//160;
UINT8 g_u1FrameDet3dLoTh = 96;
UINT8 g_u1FrameDet2dHiTh = 128;//255;
UINT8 g_u1FrameDet2dLoTh = 128;//255;
UINT8 g_u1FrameDet3dTh = 96;//160;
UINT8 g_u1FrameDet2dTh = 128;
#endif

TD22D_INFO gMain3DTo2DInfo, gPip3DTo2DInfo;

UINT8 fgFldInv[VDP_NS];

CODE TD_NAVI_REGTBL_T TD_NAVI_INIT[] =
{
    {TDNVAI_00, 0x1550C003, 0x7FFFFFFF},
    {TDNVAI_01, 0x4004DA0A, 0xFFFFFFFF},
    {TDNVAI_02, 0x03000043, 0x070000FF},
    {TDNVAI_03, 0x21004000, 0x7FFFFFFF},
    {TDNVAI_04, 0x08060000, 0x3FFFFFFF},
    {TDNVAI_REGTBL_END, 0x00000000, 0x00000000}
};

CODE REALD_REGTBL_T REALD_INIT[] =
{
    {PRE_REALD_MAIN_00, 0x18618618, 0xFFFFFFFF},
    {PRE_REALD_MAIN_01, 0x61861868, 0xFFFFFFFF},
    {PRE_REALD_MAIN_02, 0x61861861, 0xFFFFFFFF},
    {PRE_REALD_MAIN_03, 0x86186100, 0xFFFFFFC0},
    {PRE_REALD_MAIN_04, 0x374B0040, 0xFFFFC171},
    {PRE_REALD_PIP_00, 0x18618618, 0xFFFFFFFF},
    {PRE_REALD_PIP_01, 0x61861868, 0xFFFFFFFF},
    {PRE_REALD_PIP_02, 0x61861861, 0xFFFFFFFF},
    {PRE_REALD_PIP_03, 0x86186100, 0xFFFFFFC0},
    {PRE_REALD_PIP_04, 0x374B0040, 0xFFFFC171},
    {REALD_REGTBL_END, 0x00000000, 0x00000000}
};

CODE TD_NAVI_REGTBL_T TD22D_INIT[] =
{
    {PRE_3D22D_MAIN_00, 0x78043800, 0xFFFFFFF7},
    {PRE_3D22D_MAIN_01, 0x78043800, 0xFFFFFF1A},
    {PRE_3D22D_PIP_00,  0x78043800, 0xFFFFFFF7},
    {PRE_3D22D_PIP_01,  0x78043800, 0xFFFFFF1A},
    {TD22D_REGTBL_END, 0x00000000, 0x00000000}
};


/*----------------------------------------------------------------------------*
 * Function Members
 *----------------------------------------------------------------------------*/
static void vDrvTdNaviModeChangeInt(void);
static void vDrvTdNaviMainloopInt(void);
static UINT8 u1DrvTdNaviGetFwFrameDetType(void);
static void vDrvTdNaviStaConvert(void);
static void vDrvTdNaviGetAdaptSetting(void);
static UINT8 u1DrvRealDModeChangeInt(UINT8 u1VdpId);
static UINT8 u1DrvRealDMainloopInt(UINT8 u1VdpId);


void vDrvTdNaviLoadRegTbl(TD_NAVI_REGTBL_T * prRegTbl)
{
    while (prRegTbl->u2Addr != (UINT32)TDNVAI_REGTBL_END)
    {
        vRegWrite4BMsk(prRegTbl->u2Addr, prRegTbl->u4Value, prRegTbl->u4Mask);
        prRegTbl++;
    }
}


void vDrvTdNaviSetPicSize(void)
{
	UINT16 u2Width, u2Height, u2RestX, u2RestY, u2BlkXMulBlkY;
    UINT8  u1BlkSizeX, u1BlkSizeY, u1IsIntr;
    
    u1IsIntr = bDrvVideoIsSrcInterlace(SV_VP_MAIN);	
    u2Width  = wDrvVideoInputWidth(SV_VP_MAIN);
    u2Height = wDrvVideoInputHeight(SV_VP_MAIN);
    
	u2Height = u1IsIntr ? (u2Height >> 1) : u2Height;
	
	u1BlkSizeX = (UINT8)(u2Width/30);
	u1BlkSizeY = (UINT8)(u2Height >> 4);
	u2RestX = (u2Width  >> 1) - (((UINT16)u1BlkSizeX) * 15);
	u2RestY = (u2Height >> 1) - (((UINT16)u1BlkSizeY) << 3);
	u2BlkXMulBlkY = ((UINT16)u1BlkSizeX) * ((UINT16)u1BlkSizeY);
	
	vRegWriteFldAlign(TDNVAI_01, u1BlkSizeX, TDNVAI_01_BLK_SIZE_X);
	vRegWriteFldAlign(TDNVAI_02, u1BlkSizeY, TDNVAI_02_BLK_SIZE_Y);
	vRegWriteFldAlign(TDNVAI_03, u2RestX, TDNVAI_03_REST_X);
	vRegWriteFldAlign(TDNVAI_03, u2RestY, TDNVAI_03_REST_Y);
	vRegWriteFldAlign(TDNVAI_00, u2BlkXMulBlkY, TDNVAI_00_BLK_X_MUL_BLK_Y);
}

UINT8 u1DrvTdNaviModeChange(void)
{
#if defined(CC_MT5396)

	if ((u1DrvGetMJC50TO120ModeChangeFlg() == SV_TRUE) && (u1DrvMJCIsSupport50To120()))
	{
		vDrvSetMJC50TO120ModeChangeFlg(SV_OFF);
	}
	else
	{
	    vDrvTdNaviModeChangeInt();
	}
#endif
#if defined(CC_MT5389)
    vDrvTdNaviModeChangeInt();
#endif
#if defined(CC_MT5368)
	if ((BSP_GetIcVersion() >= IC_VER_5368_AB))
	{
		vDrvTdNaviModeChangeInt();
	}
#endif	
	
	return 1;
}

static void vDrvTdNaviModeChangeInt(void)
{
#if SUPPORT_FW_FRAME_DET_TYPE
    vIO32WriteFldAlign(TDNAVI_FW_16, g_u1FrameDet3dHiTh  , FRM_DET_3D_HI_TH);  
    vIO32WriteFldAlign(TDNAVI_FW_16, g_u1FrameDet3dLoTh  , FRM_DET_3D_LO_TH);  
    vIO32WriteFldAlign(TDNAVI_FW_16, g_u1FrameDet2dHiTh  , FRM_DET_2D_HI_TH);  
    vIO32WriteFldAlign(TDNAVI_FW_16, g_u1FrameDet2dLoTh  , FRM_DET_2D_LO_TH);  
	// Current 2D, detect 3D
    g_u1FrameDet3dTh =IO32ReadFldAlign(TDNAVI_FW_16, FRM_DET_3D_HI_TH); 
    g_u1FrameDet2dTh =IO32ReadFldAlign(TDNAVI_FW_16, FRM_DET_2D_HI_TH);  	
#endif
	g_u1TdNaviDetNew = TD_NAVI_DETECT_2D;
	g_u1TdNaviDetCurr = TD_NAVI_DETECT_2D;
	g_u1TdNaviDet2DCnt = 0;
	g_u1TdNaviDetSbsCnt = 0;
	g_u1TdNaviDetTabCnt = 0;
	vDrvTdNaviSetSimilarityRatio(g_u1In3DSimilarityRatio);
	vDrvTdNaviSetSimilarityThr(g_u1In3DSimilarThr);
	vDrvTdNaviSetDecisionType(g_u1In3DDecisionType);
	vDrvTdNaviSetPicSize();
}
	

void vDrvTdNaviInit(void)
{
	vIO32WriteFldAlign(TDNAVI_FW_17, g_u1VldDivInVldThr, VLD_DIV_INVLD_THR); 
	vIO32WriteFldAlign(TDNAVI_FW_18, g_u1In3DSimilarityRatio, IN_3D_SIMILARITY_RATIO); 
	vIO32WriteFldAlign(TDNAVI_FW_18, g_u1In3DSimilarThr, IN_3D_SIMILAR_THR); 
	vIO32WriteFldAlign(TDNAVI_FW_18, g_u1In3DDecisionType, IN_3D_DECISION_TYPE); 
	vIO32WriteFldAlign(TDNAVI_FW_18, g_u1Out3DSimilarityRatio, OUT_3D_SIMILARITY_RATIO); 
	vIO32WriteFldAlign(TDNAVI_FW_18, g_u1Out3DSimilarThr, OUT_3D_SIMILAR_THR); 
	vIO32WriteFldAlign(TDNAVI_FW_18, g_u1Out3DDecisionType, OUT_3D_DECISION_TYPE); 

#if SUPPORT_FW_FRAME_DET_TYPE
	vIO32WriteFldAlign(TDNAVI_FW_19, g_u1SbsTabLut[0], SBS_TAB_LUT_0); 
	vIO32WriteFldAlign(TDNAVI_FW_19, g_u1SbsTabLut[1], SBS_TAB_LUT_1); 
	vIO32WriteFldAlign(TDNAVI_FW_19, g_u1SbsTabLut[2], SBS_TAB_LUT_2); 
	vIO32WriteFldAlign(TDNAVI_FW_19, g_u1SbsTabLut[3], SBS_TAB_LUT_3); 
	vIO32WriteFldAlign(TDNAVI_FW_20, g_u1SbsTabLut[4], SBS_TAB_LUT_4); 
	vIO32WriteFldAlign(TDNAVI_FW_20, g_u1SbsTabLut[5], SBS_TAB_LUT_5); 
	vIO32WriteFldAlign(TDNAVI_FW_20, g_u1SbsTabLut[6], SBS_TAB_LUT_6); 
	vIO32WriteFldAlign(TDNAVI_FW_20, g_u1SbsTabLut[7], SBS_TAB_LUT_7);
	vIO32WriteFldAlign(TDNAVI_FW_21, g_u1SbsTabLut[8], SBS_TAB_LUT_8); 
#endif	
#if defined(CC_MT5396) || defined(CC_MT5389)
    vDrvTdNaviLoadRegTbl(TD_NAVI_INIT);
    u1DrvTdNaviModeChange();
    vDrvTdNaviFWDetectionInit();
#endif
#if defined(CC_MT5368)
	if ((BSP_GetIcVersion() >= IC_VER_5368_AB))
	{
    vDrvTdNaviLoadRegTbl(TD_NAVI_INIT);
    u1DrvTdNaviModeChange();
    vDrvTdNaviFWDetectionInit();
}
#endif	
}

void vDrvTdNaviMainloop(void)
{
#if defined(CC_MT5396) || defined(CC_MT5389)
    vDrvTdNaviMainloopInt();
#endif
#if defined(CC_MT5368)
	if ((BSP_GetIcVersion() >= IC_VER_5368_AB))
	{
    	vDrvTdNaviMainloopInt();
	}
#endif	
}

static void vDrvTdNaviMainloopInt(void)
{
	UINT8 u1StaSbsCnt, u1StaTabCnt, u1StaSbsVldBlk, u1StaTabVldBlk;

	vDrvTdNaviStaConvert();
    vDrvTdNaviFWDetectionProc();

	// Get SW Adaptive Setting
	vDrvTdNaviGetAdaptSetting();
	
	u1StaSbsCnt = vDrvTdNaviGetSbsCnt();
	u1StaTabCnt = vDrvTdNaviGetTabCnt();
	u1StaSbsVldBlk = vDrvTdNaviGetSbsVldBlk();
	u1StaTabVldBlk = vDrvTdNaviGetTabVldBlk();
	g_u1TdNaviDetNew = u1DrvTdNaviGetFwFrameDetType();
	
    if (TD_NAVI_DETECT_SBS == g_u1TdNaviDetCurr)
	{
		if (g_u1TdNaviAdaptOnOff)
		{
			// Current 3D, detect 2D
		#if SUPPORT_FW_FRAME_DET_TYPE
		    g_u1FrameDet3dTh =IO32ReadFldAlign(TDNAVI_FW_16, FRM_DET_3D_LO_TH); 
		    g_u1FrameDet2dTh =IO32ReadFldAlign(TDNAVI_FW_16, FRM_DET_2D_LO_TH);  	
		#endif
			vDrvTdNaviSetSimilarityRatio(g_u1Out3DSimilarityRatio);
			vDrvTdNaviSetSimilarityThr(g_u1Out3DSimilarThr);
			vDrvTdNaviSetDecisionType(g_u1Out3DDecisionType);
		}

		// Ink Magenta
		vRegWriteFldAlign(TDNVAI_04, 128, TDNVAI_04_FW_DBG_INK_Y);
		vRegWriteFldAlign(TDNVAI_04, 255, TDNVAI_04_FW_DBG_INK_C);		

		if ((TD_NAVI_DETECT_2D == g_u1TdNaviDetNew) || (TD_NAVI_DETECT_TAB == g_u1TdNaviDetNew))
		{
			g_u1TdNaviDet2DCnt++;

			if(g_u1TdNaviDet2DCnt > g_u1TdNaviDetOutFrmNum)
			{
			g_u1TdNaviDetCurr = TD_NAVI_DETECT_2D;
				g_u1TdNaviDet2DCnt = 0;
			}
		}
		else // SBS, TOO_FEW, AMBIGUO, NEIGHBOR
		{
			if (g_u1TdNaviDet2DCnt > 0)
			{
				g_u1TdNaviDet2DCnt--;
			}
		}
	}
	else if(TD_NAVI_DETECT_TAB == g_u1TdNaviDetCurr)
	{
		if (g_u1TdNaviAdaptOnOff)
		{
			// Current 3D, detect 2D
		#if SUPPORT_FW_FRAME_DET_TYPE
		    g_u1FrameDet3dTh =IO32ReadFldAlign(TDNAVI_FW_16, FRM_DET_3D_LO_TH); 
		    g_u1FrameDet2dTh =IO32ReadFldAlign(TDNAVI_FW_16, FRM_DET_2D_LO_TH);  	
		#endif
			vDrvTdNaviSetSimilarityRatio(g_u1Out3DSimilarityRatio);
			vDrvTdNaviSetSimilarityThr(g_u1Out3DSimilarThr);
			vDrvTdNaviSetDecisionType(g_u1Out3DDecisionType);
		}

		// Ink Green
		vRegWriteFldAlign(TDNVAI_04, 128, TDNVAI_04_FW_DBG_INK_Y);
		vRegWriteFldAlign(TDNVAI_04, 0, TDNVAI_04_FW_DBG_INK_C);		

		if ((TD_NAVI_DETECT_2D == g_u1TdNaviDetNew) || (TD_NAVI_DETECT_SBS == g_u1TdNaviDetNew))
		{
			g_u1TdNaviDet2DCnt++;

			if(g_u1TdNaviDet2DCnt > g_u1TdNaviDetOutFrmNum)
			{
			g_u1TdNaviDetCurr = TD_NAVI_DETECT_2D;
				g_u1TdNaviDet2DCnt = 0;
			}
		}
		else // TB, TOO_FEW, AMBIGUO, NEIGHBOR
		{
			if (g_u1TdNaviDet2DCnt > 0)
			{
				g_u1TdNaviDet2DCnt--;
			}
		}
	}
    else if(TD_NAVI_DETECT_2D!=eDrvSwNaviDetectedFmtQuery())
    {    
        vRegWriteFldAlign(TDNVAI_04, 0xFF, TDNVAI_04_FW_DBG_INK_Y);
        vRegWriteFldAlign(TDNVAI_04, 0xC0, TDNVAI_04_FW_DBG_INK_C);            
    }
    else //(TD_NAVI_DETECT_2D == g_u1TdNaviDetCurr)
	{
		if (g_u1TdNaviAdaptOnOff)
		{
			// Current 2D, detect 3D
		#if SUPPORT_FW_FRAME_DET_TYPE
		    g_u1FrameDet3dTh =IO32ReadFldAlign(TDNAVI_FW_16, FRM_DET_3D_HI_TH); 
		    g_u1FrameDet2dTh =IO32ReadFldAlign(TDNAVI_FW_16, FRM_DET_2D_HI_TH);  	
		#endif
			vDrvTdNaviSetSimilarityRatio(g_u1In3DSimilarityRatio);
			vDrvTdNaviSetSimilarityThr(g_u1In3DSimilarThr);
			vDrvTdNaviSetDecisionType(g_u1In3DDecisionType);
		}
		
		// Ink Black
		vRegWriteFldAlign(TDNVAI_04, 0, TDNVAI_04_FW_DBG_INK_Y);
		vRegWriteFldAlign(TDNVAI_04, 128, TDNVAI_04_FW_DBG_INK_C);		
		
		if (TD_NAVI_DETECT_SBS == g_u1TdNaviDetNew)
		{
			g_u1TdNaviDetSbsCnt++;

			if (g_u1TdNaviDetTabCnt > 0)
			{
				g_u1TdNaviDetTabCnt--;
			}

			if(g_u1TdNaviDetSbsCnt > g_u1TdNaviDetInFrmNum)
			{
				g_u1TdNaviDetCurr = TD_NAVI_DETECT_SBS;
				g_u1TdNaviDetSbsCnt = 0;
				g_u1TdNaviDetTabCnt = 0;
			}
		}
		else if (TD_NAVI_DETECT_TAB == g_u1TdNaviDetNew)
		{
			g_u1TdNaviDetTabCnt++;

			if (g_u1TdNaviDetSbsCnt > 0)
			{
				g_u1TdNaviDetSbsCnt--;
			}

			if(g_u1TdNaviDetTabCnt > g_u1TdNaviDetInFrmNum)
			{
				g_u1TdNaviDetCurr = TD_NAVI_DETECT_TAB;
				g_u1TdNaviDetSbsCnt = 0;
				g_u1TdNaviDetTabCnt = 0;
			}
		}
		else // 2D or unknown
		{
			if (g_u1TdNaviDetSbsCnt > 0)
			{
				g_u1TdNaviDetSbsCnt--;
			}

			if (g_u1TdNaviDetTabCnt > 0)
		{
				g_u1TdNaviDetTabCnt--;
			}
		}
	}


	if (g_u1TdNaviTrigger && (g_u1TdNaviPrint > 0))
	{
		LOG(0, "u1StaSbsCnt = %d\n", u1StaSbsCnt);
		LOG(0, "u1StaTabCnt = %d\n", u1StaTabCnt);
		LOG(0, "u1StaSbsVldBlk = %d\n", u1StaSbsVldBlk);
		LOG(0, "u1StaTabVldBlk = %d\n", u1StaTabVldBlk);
		LOG(0, "u1StaSbsRatio = %d/256\n", (((UINT16)u1StaSbsCnt) << 8)/u1StaSbsVldBlk);
		LOG(0, "u1StaTabRatio = %d/256\n", (((UINT16)u1StaTabCnt) << 8)/u1StaTabVldBlk);
		g_u1TdNaviPrint--;
	}
}

static UINT8 u1DrvTdNaviGetFwFrameDetType(void)
{
	UINT8 u1FwFrameDetType;
#if SUPPORT_FW_FRAME_DET_TYPE
	INT32 i4Det2dTh;//, i4Det3dTh;
	INT32 i4SbsPer, i4TabPer, i4SbsCnt, i4TabCnt, i4SbsVld, i4TabVld, i4VldBlkTh;
	INT32 i4SbsInVld, i4TabInVld, i4SbsInVldThr, i4TabInVldThr;
	INT32 i4AmbigTh, i4CntDiff, i4VldBlkMin, i4VldBlkMax;
	INT32 i4SbsNbrCnt, i4TabNbrCnt, i4NbrCntTh, i4MinNbrCnt;
	INT32 i4Lut[9], i4Idx, i4Gain, i4Comp;
	UINT8 u1IsSbs, u1IsTab, u1Is2D;
	
	i4Det2dTh = (INT32)g_u1FrameDet2dTh;
	//i4Det3dTh = (INT32)g_u1FrameDet3dTh;
	//UNUSED(i4Det3dTh);
	
	i4SbsCnt = (INT32)(RegReadFldAlign(TDNVAI_STA1, TDNVAI_STA1_STA_SBS_CNT));
	i4TabCnt = (INT32)(RegReadFldAlign(TDNVAI_STA1, TDNVAI_STA1_STA_TAB_CNT));
	i4SbsVld = (INT32)(RegReadFldAlign(TDNVAI_STA0, TDNVAI_STA0_STA_SBS_VLD_BLK));
	i4TabVld = (INT32)(RegReadFldAlign(TDNVAI_STA0, TDNVAI_STA0_STA_TAB_VLD_BLK));
	i4SbsInVld = (INT32)(RegReadFldAlign(TDNVAI_STA1, TDNVAI_STA1_STA_INVLD_SBS));
	i4TabInVld = (INT32)(RegReadFldAlign(TDNVAI_STA1, TDNVAI_STA1_STA_INVLD_TAB));

	i4VldBlkTh = (INT32)(RegReadFldAlign(TDNVAI_01, TDNVAI_01_VALID_BLK_THR));
	i4AmbigTh = (INT32)(RegReadFldAlign(TDNVAI_01, TDNVAI_01_AMBIG_THR));

	i4SbsNbrCnt = (INT32)(RegReadFldAlign(TDNVAI_STA2, TDNVAI_STA2_STA_SBS_NEIGHBOR_DIFF_CNT));
	i4TabNbrCnt = (INT32)(RegReadFldAlign(TDNVAI_STA2, TDNVAI_STA2_STA_TAB_NEIGHBOR_DIFF_CNT));
	i4NbrCntTh = (INT32)(RegReadFldAlign(TDNVAI_04, TDNVAI_04_NEIGHBOR_CNT_THR));

	i4Lut[0] = (INT32)(IO32ReadFldAlign(TDNAVI_FW_19, SBS_TAB_LUT_0));
	i4Lut[1] = (INT32)(IO32ReadFldAlign(TDNAVI_FW_19, SBS_TAB_LUT_1));
	i4Lut[2] = (INT32)(IO32ReadFldAlign(TDNAVI_FW_19, SBS_TAB_LUT_2));
	i4Lut[3] = (INT32)(IO32ReadFldAlign(TDNAVI_FW_19, SBS_TAB_LUT_3));
	i4Lut[4] = (INT32)(IO32ReadFldAlign(TDNAVI_FW_20, SBS_TAB_LUT_4));
	i4Lut[5] = (INT32)(IO32ReadFldAlign(TDNAVI_FW_20, SBS_TAB_LUT_5));
	i4Lut[6] = (INT32)(IO32ReadFldAlign(TDNAVI_FW_20, SBS_TAB_LUT_6));
	i4Lut[7] = (INT32)(IO32ReadFldAlign(TDNAVI_FW_20, SBS_TAB_LUT_7));
	i4Lut[8] = (INT32)(IO32ReadFldAlign(TDNAVI_FW_21, SBS_TAB_LUT_8));


	i4CntDiff = (i4SbsCnt > i4TabCnt) ? (i4SbsCnt - i4TabCnt) : (i4TabCnt - i4SbsCnt);
	i4VldBlkMin = (i4TabVld < i4SbsVld) ? i4TabVld : i4SbsVld;
	i4VldBlkMax = (i4SbsVld > i4TabVld) ? i4SbsVld : i4TabVld;

	i4MinNbrCnt = (i4SbsNbrCnt > i4TabNbrCnt) ? i4TabNbrCnt : i4SbsNbrCnt;

	i4SbsPer = (i4SbsVld > 0) ? ((i4SbsCnt << 8)/i4SbsVld) : 0;
	i4TabPer = (i4TabVld > 0) ? ((i4TabCnt << 8)/i4TabVld) : 0;
		
	i4SbsInVldThr = i4SbsInVld*((INT32)g_u1VldDivInVldThr);
	i4TabInVldThr = i4TabInVld*((INT32)g_u1VldDivInVldThr);
	
	// check IsSBS ?
	if (i4SbsPer < 32)
	{
		u1IsSbs = 0;
	}
	else if (i4SbsPer >= 256)
	{
		u1IsSbs = (i4Lut[8] < i4TabPer) ? 0 : 1;
	}
	else
	{
		i4Idx  = i4SbsPer >> 5;
		i4Gain = i4SbsPer & 0x1F;
		i4Comp = i4Lut[i4Idx] + ((i4Gain*(i4Lut[i4Idx+1]-i4Lut[i4Idx]) + 16) >> 5);

		u1IsSbs = (i4Comp < i4TabPer) ? 0 : 1;		
	}
	
	// check IsTab ?
	if (i4TabPer < 32)
	{
		u1IsTab = 0;
	}
	else if (i4TabPer >= 256)
	{
		u1IsTab = (i4Lut[8] < i4SbsPer) ? 0 : 1;
	}	
	else
	{
		i4Idx  = i4TabPer >> 5;
		i4Gain = i4TabPer & 0x1F;
		i4Comp = i4Lut[i4Idx] + ((i4Gain*(i4Lut[i4Idx+1]-i4Lut[i4Idx]) + 16) >> 5);

		u1IsTab = (i4Comp < i4SbsPer) ? 0 : 1;		
	}
	
	// check Is2D ?
	u1Is2D = (i4SbsPer + i4TabPer < i4Det2dTh) ? 1 : 0;
	
	
	if (((i4SbsVld << 4) < i4SbsInVldThr) || ((i4SbsVld << 4) < i4TabInVldThr))
	{
		u1FwFrameDetType = 8; // INVLD
	}
	else if ((!u1IsSbs) && (!u1IsTab) && (!u1Is2D))
	{
		u1FwFrameDetType = 4; // UNKNOWN;
	}
	else if (i4VldBlkMax < i4VldBlkTh)
	{
		u1FwFrameDetType = 5; // TOO_FEW
	}
	else if (u1Is2D)
	{
		u1FwFrameDetType = TD_NAVI_DETECT_2D;
	}
	else if (i4CntDiff*i4AmbigTh < i4VldBlkMin)
	{
		u1FwFrameDetType = 6; // AMBIGUO
	}
	else if (i4MinNbrCnt < i4NbrCntTh)
	{
		u1FwFrameDetType = 7; // NEIGHBO
	}
	else if (u1IsSbs)
	{
		u1FwFrameDetType = TD_NAVI_DETECT_SBS;
	}
	else // if (u1IsTab)
	{
		u1FwFrameDetType = TD_NAVI_DETECT_TAB;
	}

	if (IO32ReadFldAlign(TDNAVI_FW_17, STATUS_PRINT_EN))
	{
		Printf("SbsCnt = %3d, SbsVld = %3d, SbsPer = %3d, TabCnt = %3d, TabVld = %3d, TabPer = %3d, VldBlkMax = %3d, VldBlkMin = %3d, CntDiff = %3d, MinNbrCnt = %3d\n", i4SbsCnt, i4SbsVld, i4SbsPer, i4TabCnt, i4TabVld, i4TabPer, i4VldBlkMax, i4VldBlkMin, i4CntDiff, i4MinNbrCnt);
	}

#else
	u1FwFrameDetType = vDrvTdNaviGetNewDetType();
#endif	
	vIO32WriteFldAlign(TDNAVI_STATUS_16, u1FwFrameDetType  , STA_FW_DET_TYPE);
	return u1FwFrameDetType;
}

static void vDrvTdNaviStaConvert(void)
{
	UINT32 u4SbsCnt, u4TabCnt, u4SbsVld, u4TabVld;
	UINT32 u4SbsPer, u4TabPer, u4CntDiff, u4VldMin;
	UINT32 u4VldMax, u4Ambiguo;
	UINT32 u4SbsNbrCnt, u4TabNbrCnt, u4MinNbrCnt;
	
	u4SbsCnt = (UINT32)(RegReadFldAlign(TDNVAI_STA1, TDNVAI_STA1_STA_SBS_CNT));
	u4TabCnt = (UINT32)(RegReadFldAlign(TDNVAI_STA1, TDNVAI_STA1_STA_TAB_CNT));
	u4SbsVld = (UINT32)(RegReadFldAlign(TDNVAI_STA0, TDNVAI_STA0_STA_SBS_VLD_BLK));
	u4TabVld = (UINT32)(RegReadFldAlign(TDNVAI_STA0, TDNVAI_STA0_STA_TAB_VLD_BLK));
	u4SbsNbrCnt = (UINT32)(RegReadFldAlign(TDNVAI_STA2, TDNVAI_STA2_STA_SBS_NEIGHBOR_DIFF_CNT));
	u4TabNbrCnt = (UINT32)(RegReadFldAlign(TDNVAI_STA2, TDNVAI_STA2_STA_TAB_NEIGHBOR_DIFF_CNT));

	u4VldMax = (u4SbsVld > u4TabVld) ? u4SbsVld : u4TabVld;
	u4VldMin = (u4SbsVld < u4TabVld) ? u4SbsVld : u4TabVld;
	u4CntDiff = (u4SbsCnt > u4TabCnt) ? (u4SbsCnt - u4TabCnt) : (u4TabCnt - u4SbsCnt);

	// If u4CntDiff = 0, AMBIGUO always valid
	// If u4VldMin  = 0, AMBIGUO always invalid
	u4Ambiguo = (u4CntDiff > 0) ? (u4VldMin/u4CntDiff) : 256;
	u4Ambiguo = (u4Ambiguo > 256) ? 256 : u4Ambiguo;

	u4MinNbrCnt = (u4SbsNbrCnt < u4TabNbrCnt) ? u4SbsNbrCnt : u4TabNbrCnt;

	u4SbsPer = (u4SbsVld > 0) ? ((u4SbsCnt << 8)/u4SbsVld) : 0;
	u4TabPer = (u4TabVld > 0) ? ((u4TabCnt << 8)/u4TabVld) : 0;
	
	vIO32WriteFldAlign(TDNAVI_STATUS_16, (UINT8)u4Ambiguo, STA_AMBIGUO); 
	vIO32WriteFldAlign(TDNAVI_STATUS_16, (UINT8)u4VldMax, STA_TOO_FEW); 
	vIO32WriteFldAlign(TDNAVI_STATUS_17, (UINT16)u4SbsPer, STA_SBS_RATE); 
	vIO32WriteFldAlign(TDNAVI_STATUS_17, (UINT16)u4TabPer, STA_TAB_RATE); 
	vIO32WriteFldAlign(TDNAVI_STATUS_18, (UINT16)u4MinNbrCnt, STA_NEIGHBOR); 
}


static void vDrvTdNaviGetAdaptSetting(void)
{
    g_u1VldDivInVldThr       = IO32ReadFldAlign(TDNAVI_FW_17, VLD_DIV_INVLD_THR); 
    g_u1In3DSimilarityRatio  = IO32ReadFldAlign(TDNAVI_FW_18, IN_3D_SIMILARITY_RATIO); 
    g_u1In3DSimilarThr       = IO32ReadFldAlign(TDNAVI_FW_18, IN_3D_SIMILAR_THR); 
    g_u1In3DDecisionType     = IO32ReadFldAlign(TDNAVI_FW_18, IN_3D_DECISION_TYPE); 
    g_u1Out3DSimilarityRatio = IO32ReadFldAlign(TDNAVI_FW_18, OUT_3D_SIMILARITY_RATIO); 
    g_u1Out3DSimilarThr      = IO32ReadFldAlign(TDNAVI_FW_18, OUT_3D_SIMILAR_THR); 
    g_u1Out3DDecisionType    = IO32ReadFldAlign(TDNAVI_FW_18, OUT_3D_DECISION_TYPE); 
}

UINT8 vDrvTdNaviGetSbsCnt(void)
{
	UINT8 u1SbsCnt = RegReadFldAlign(TDNVAI_STA1, TDNVAI_STA1_STA_SBS_CNT);
	return u1SbsCnt;
}

UINT8 vDrvTdNaviGetTabCnt(void)
{
	UINT8 u1TabCnt = RegReadFldAlign(TDNVAI_STA1, TDNVAI_STA1_STA_TAB_CNT);
	return u1TabCnt;
}

UINT8 vDrvTdNaviGetSbsVldBlk(void)
{
	UINT8 u1SbsVldBlk = RegReadFldAlign(TDNVAI_STA0, TDNVAI_STA0_STA_SBS_VLD_BLK);
	return u1SbsVldBlk;
}

UINT8 vDrvTdNaviGetTabVldBlk(void)
{
	UINT8 u1TabVldBlk = RegReadFldAlign(TDNVAI_STA0, TDNVAI_STA0_STA_TAB_VLD_BLK);
	return u1TabVldBlk;
}

UINT8 vDrvTdNaviGetNewDetType(void)
{
	INT32 i4SbsVld, i4TabVld, i4SbsInVld, i4TabInVld, i4SbsInVldThr, i4TabInVldThr;
	UINT8 u1DetType = RegReadFldAlign(TDNVAI_STA2, TDNVAI_STA2_STA_DET_TYPE);
	
	i4SbsVld = (INT32)(RegReadFldAlign(TDNVAI_STA0, TDNVAI_STA0_STA_SBS_VLD_BLK));
	i4TabVld = (INT32)(RegReadFldAlign(TDNVAI_STA0, TDNVAI_STA0_STA_TAB_VLD_BLK));
	i4SbsInVld = (INT32)(RegReadFldAlign(TDNVAI_STA1, TDNVAI_STA1_STA_INVLD_SBS));
	i4TabInVld = (INT32)(RegReadFldAlign(TDNVAI_STA1, TDNVAI_STA1_STA_INVLD_TAB));
	i4SbsInVldThr = i4SbsInVld*((INT32)g_u1VldDivInVldThr);
	i4TabInVldThr = i4TabInVld*((INT32)g_u1VldDivInVldThr);

	if (((i4SbsVld << 4) < i4SbsInVldThr) || ((i4SbsVld << 4) < i4TabInVldThr))
	{
		u1DetType = 8; // AMBIGUO
	}
	
	return u1DetType;
}

UINT8 vDrvTdNaviGetCurrDetType(void)
{
	if (g_u1TdNaviDetCurr != TD_NAVI_DETECT_2D)
	{
		return g_u1TdNaviDetCurr;
	}
	else
	{
		return eDrvSwNaviDetectedFmtQuery();
	}
}

void vDrvTdNaviSetSimilarityRatio(UINT8 u1Ratio)
{
	vRegWriteFldAlign(TDNVAI_01, u1Ratio, TDNVAI_01_D_TH);
}

void vDrvTdNaviSetSimilarityThr(UINT8 u1Thr)
{
	vRegWriteFldAlign(TDNVAI_00, u1Thr, TDNVAI_00_SIMILAR_THR);
}

void vDrvTdNaviSetDecisionType(UINT8 u1Type)
{
	vRegWriteFldAlign(TDNVAI_03, u1Type, TDNVAI_03_DECISION_TYPE);
}

void vDrvTdNaviSetDbgType(UINT8 u1Type)
{
	vRegWriteFldAlign(TDNVAI_03, u1Type, TDNVAI_03_TDNAVI_DBG);
	vRegWriteFldAlign(TDNVAI_03, (u1Type == 1), TDNVAI_03_FW_DBG);
}

// RealD Tag
void vDrvRealDLoadRegTbl(REALD_REGTBL_T * prRegTbl)
{
    while (prRegTbl->u2Addr != (UINT32)REALD_REGTBL_END)
    {
        vRegWrite4BMsk(prRegTbl->u2Addr, prRegTbl->u4Value, prRegTbl->u4Mask);
        prRegTbl++;
    }
}


void vDrvRealDSetSourceMode(UINT8 u1VdpId)
{
    UINT8 u1RealDSrcMode, u1SrcTiming;
    UINT32 u4Width;
    
    u1SrcTiming = bDrvVideoGetTiming(u1VdpId);

    switch (u1SrcTiming)
    {
    case MODE_720p_24:
    case MODE_720p_25:    
    case MODE_720p_30:        
    case MODE_720p_50:
    case MODE_720p_60:
    	u1RealDSrcMode = 0;
		break;
    case MODE_1080i_48:
    case MODE_1080i_50:
    case MODE_1080i:
    	u1RealDSrcMode = 1;
		break;
    case MODE_1080p_24:
    case MODE_1080p_25:
    case MODE_1080p_30:
    case MODE_1080p_50:
    case MODE_1080p_60:
    	u1RealDSrcMode = 2;
        break;
    default:
    	u1RealDSrcMode = 3;
        break;
    
    }

	// Check 2 line, 60 segment/line, each segment delete left/right 5 pixels
    u4Width = wDrvVideoInputWidth(u1VdpId);
    g_u2RealDInQuaThr[u1VdpId]  = ((u4Width-600)*2*((UINT32)g_u2RealDInQuaThrRatio)) >> 8;
   	g_u2RealDOutQuaThr[u1VdpId] = ((u4Width-600)*2*((UINT32)g_u2RealDOutQuaThrRatio)) >> 8;


	if (u1VdpId == SV_VP_MAIN)
	{
		vRegWriteFldAlign(PRE_REALD_MAIN_03, u1RealDSrcMode, PRE_REALD_MAIN_03_R_UIINSRCMODE_M);
	}
	else if (u1VdpId == SV_VP_PIP)
	{
		vRegWriteFldAlign(PRE_REALD_PIP_03, u1RealDSrcMode, PRE_REALD_PIP_03_R_UIINSRCMODE_P);
	}
}

UINT8 u1DrvRealDModeChange(UINT8 u1VdpId)
{
#if defined(CC_MT5396) || defined(CC_MT5389)
    u1DrvRealDModeChangeInt(u1VdpId);
#endif
#if defined(CC_MT5368)
	if ((BSP_GetIcVersion() >= IC_VER_5368_AB))
	{
    	u1DrvRealDModeChangeInt(u1VdpId);
	}
#endif

	return 1;
}

static UINT8 u1DrvRealDModeChangeInt(UINT8 u1VdpId)
{
	if (u1VdpId >= SV_VP_NA)
	{
		return 0;
	}

	g_u1RealDInFrmCnt[u1VdpId] = 0;
	g_u1RealDOutFrmCnt[u1VdpId] = 0;
	g_u1RealDDetCurr[u1VdpId] = REALD_DETECT_2D;
	vDrvRealDSetDetectMode(u1VdpId, REALD_DETECT_2D);
	vDrvRealDSetSourceMode(u1VdpId);

	vDrvRealDSetInDiffThr(u1VdpId, g_u1RealDInDiffThr[u1VdpId]);
	vDrvRealDSetOutDiffThr(u1VdpId, g_u1RealDOutDiffThr[u1VdpId]);
	
	return 1;
}

void vDrvRealDInit(void)
{
	#if defined(CC_MT5396)
		if ((BSP_GetIcVersion() >= IC_VER_5396_AB))
		{
    vDrvRealDLoadRegTbl(REALD_INIT);
}
	#endif
	#if defined(CC_MT5368)
		if ((BSP_GetIcVersion() >= IC_VER_5368_AB))
		{
    		vDrvRealDLoadRegTbl(REALD_INIT);
		}
	#endif
	#if defined(CC_MT5389)
    	vDrvRealDLoadRegTbl(REALD_INIT);
	#endif
}

void vDrvRealDMainloop(UINT8 u1VdpId)
{
	#if defined(CC_MT5396)
		if ((BSP_GetIcVersion() >= IC_VER_5396_AB))
		{
    		u1DrvRealDMainloopInt(u1VdpId);
		}
	#endif
	#if defined(CC_MT5368)
		if ((BSP_GetIcVersion() >= IC_VER_5368_AB))
		{
    		u1DrvRealDMainloopInt(u1VdpId);
		}
	#endif
	#if defined(CC_MT5389)
    	u1DrvRealDMainloopInt(u1VdpId);
	#endif
}

static UINT8 u1DrvRealDMainloopInt(UINT8 u1VdpId)
{
	UINT16 u2QuaSta;
	UINT8 u1DetectMode;
	u2QuaSta = u2DrvRealDGetQuaSta(u1VdpId);
	u1DetectMode = u1DrvRealDGetDetectMode(u1VdpId);

	if (u1VdpId >= SV_VP_NA)
	{
		return 0;
	}

	if ((!u1VdpId) && g_u1RealDTrigger && (g_u2RealDPrint > 0))
	{
		LOG(0, "u1DetectMode = %d, u2QuaSta = %d\n", u1DetectMode, u2QuaSta);
		g_u2RealDPrint--;
	}


	if (g_u1RealDAdaptEn[u1VdpId])
	{
		if (REALD_DETECT_2D == g_u1RealDDetCurr[u1VdpId])
		{
			if (u2QuaSta > g_u2RealDInQuaThr[u1VdpId])
			{
				g_u1RealDInFrmCnt[u1VdpId]++;
			}
			else if (u2QuaSta < g_u2RealDInQuaThr[u1VdpId])
			{
				if (g_u1RealDInFrmCnt[u1VdpId] > 0)
				{
					g_u1RealDInFrmCnt[u1VdpId]--;
				}
			}

			if (g_u1RealDInFrmCnt[u1VdpId] > g_u1RealDInFrmThr[u1VdpId])  // Status == RealD
			{
				g_u1RealDInFrmCnt[u1VdpId] = 0;
				g_u1RealDDetCurr[u1VdpId] = REALD_DETECT_RD;
				vDrvRealDSetDetectMode(u1VdpId, REALD_DETECT_RD);
			}
		}
		else if (REALD_DETECT_RD == g_u1RealDDetCurr[u1VdpId])
		{
			if (u2QuaSta > g_u2RealDOutQuaThr[u1VdpId])
			{
				g_u1RealDOutFrmCnt[u1VdpId]++;
			}
			else if (u2QuaSta < g_u2RealDOutQuaThr[u1VdpId])
			{
				if (g_u1RealDOutFrmCnt[u1VdpId] > 0)
				{
					g_u1RealDOutFrmCnt[u1VdpId]--;
				}
			}

			if (g_u1RealDOutFrmCnt[u1VdpId] > g_u1RealDOutFrmThr[u1VdpId]) // Status == 2D
			{
				g_u1RealDOutFrmCnt[u1VdpId] = 0;
				g_u1RealDDetCurr[u1VdpId] = REALD_DETECT_2D;
				vDrvRealDSetDetectMode(u1VdpId, REALD_DETECT_2D);
			}
		}
	}

	return 1;
}


void vDrvRealDSetDetectMode(UINT8 u1VdpId, UINT8 u1DetMode)
{
	if (u1VdpId == SV_VP_MAIN)
	{
		vRegWriteFldAlign(PRE_REALD_MAIN_04, u1DetMode, PRE_REALD_MAIN_04_R_BIN2D3DMODE_M);
	}
	else if (u1VdpId == SV_VP_PIP)
	{
		vRegWriteFldAlign(PRE_REALD_PIP_04, u1DetMode, PRE_REALD_PIP_04_R_BIN2D3DMODE_P);
	}	
}


UINT8 u1DrvRealDGetDetectMode(UINT8 u1VdpId)
{
	UINT8 u1DetMode = 0;
	
	if (u1VdpId == SV_VP_MAIN)
	{
		u1DetMode = RegReadFldAlign(PRE_REALD_MAIN_04, PRE_REALD_MAIN_04_R_BIN2D3DMODE_M);
	}
	else if (u1VdpId == SV_VP_PIP)
	{
		u1DetMode = RegReadFldAlign(PRE_REALD_PIP_04, PRE_REALD_PIP_04_R_BIN2D3DMODE_P);
	}	

	return u1DetMode;
}

void vDrvRealDSetInDiffThr(UINT8 u1VdpId, UINT8 u1InThr)
{
	if (u1VdpId == SV_VP_MAIN)
	{
		vRegWriteFldAlign(PRE_REALD_MAIN_04, u1InThr, PRE_REALD_MAIN_04_R_UIQUITHL_M);
	}
	else if (u1VdpId == SV_VP_PIP)
	{
		vRegWriteFldAlign(PRE_REALD_PIP_04, u1InThr, PRE_REALD_PIP_04_R_UIQUITHL_P);
	}	
}

void vDrvRealDSetOutDiffThr(UINT8 u1VdpId, UINT8 u1OutThr)
{
	if (u1VdpId == SV_VP_MAIN)
	{
		vRegWriteFldAlign(PRE_REALD_MAIN_04, u1OutThr, PRE_REALD_MAIN_04_R_UIDEQUITHL_M);
	}
	else if (u1VdpId == SV_VP_PIP)
	{
		vRegWriteFldAlign(PRE_REALD_PIP_04, u1OutThr, PRE_REALD_PIP_04_R_UIDEQUITHL_P);
	}	
}


UINT8 u1DrvRealDGetCurrDetType(UINT8 u1VdpId)
{
	return g_u1RealDDetCurr[u1VdpId];
}

void u1DrvRealDMaskOnOff(UINT8 u1VdpId, UINT8 u1OnOff)
{
	if (u1VdpId == SV_VP_MAIN)
	{
		vRegWriteFldAlign(PRE_REALD_MAIN_04, u1OnOff, PRE_REALD_MAIN_04_R_UIMASKENABLE_M);
	}
	else if (u1VdpId == SV_VP_PIP)
	{
		vRegWriteFldAlign(PRE_REALD_PIP_04, u1OnOff, PRE_REALD_PIP_04_R_UIMASKENABLE_P);
	}		
}


UINT16 u2DrvRealDGetQuaSta(UINT8 u1VdpId)
{
	UINT16 u2QuaSta = 0;
	
	if (u1VdpId == SV_VP_MAIN)
	{
		u2QuaSta = RegReadFldAlign(STA_REALD_MAIN_00, STA_REALD_MAIN_00_FRAME_QUA_STA_M);
	}
	else if (u1VdpId == SV_VP_PIP)
	{
		u2QuaSta = RegReadFldAlign(STA_REALD_PIP_00, STA_REALD_PIP_00_FRAME_QUA_STA_P);
	}	

	return u2QuaSta;
}

void u2DrvRealDQuery(void)
{
	UINT8 u1VdpId;

    for (u1VdpId = 0; u1VdpId < 2; u1VdpId++)
    {    
        printf("===================== VDP:%d========================\n",u1VdpId);   
        Printf("g_u1RealDInDiffThr ...... %d\n", g_u1RealDInDiffThr[u1VdpId]);
        Printf("g_u1RealDOutDiffThr ..... %d\n", g_u1RealDOutDiffThr[u1VdpId]);
        Printf("g_u1RealDInFrmThr ....... %d\n", g_u1RealDInFrmThr[u1VdpId]);
        Printf("g_u1RealDOutFrmThr ...... %d\n", g_u1RealDOutFrmThr[u1VdpId]);
        Printf("g_u2RealDInQuaThr ....... %d\n", g_u2RealDInQuaThr[u1VdpId]);
        Printf("g_u2RealDOutQuaThr ...... %d\n", g_u2RealDOutQuaThr[u1VdpId]);
        Printf("g_u2RealDInQuaThrRatio .. %d\n", g_u2RealDInQuaThrRatio);
        Printf("g_u2RealDOutQuaThrRatio . %d\n", g_u2RealDOutQuaThrRatio);
    }
}


// 3D-to-2D
void vDrvTD22DLoadRegTbl(TD22D_REGTBL_T * prRegTbl)
{
    while (prRegTbl->u2Addr != (UINT32)TD22D_REGTBL_END)
    {
        vRegWrite4BMsk(prRegTbl->u2Addr, prRegTbl->u4Value, prRegTbl->u4Mask);
        prRegTbl++;
    }
}


UINT8 u1DrvTD22DSetMode(UINT8 u1VdpId, UINT8 u1TD22DEn, UINT8 u1ModeSel, UINT8 u1MaskSel)
{
	UINT16 u2Width, u2Height;
    UINT8  u1IsIntr;
    
    u1IsIntr = bDrvVideoIsSrcInterlace(u1VdpId);	
    u2Width  = wDrvVideoInputWidth(u1VdpId);
    u2Height = wDrvVideoInputHeight(u1VdpId);
    
	u2Height = u1IsIntr ? (u2Height >> 1) : u2Height;

	// 3D-to-2D Workaround for MT5396ES & MT5368ES
	#if defined(CC_MT5396)
	if (BSP_GetIcVersion() < IC_VER_5396_AB)
	{
		if (u1ModeSel == TD22D_MODE_SIDE_BY_SIDE)
		{
			u1MaskSel = TD22D_SBS_RIGHT;
		}

		if (u1ModeSel == TD22D_MODE_TOP_AND_BOTTOM)
		{
			u1MaskSel = TD22D_TB_TOP;
		}

		if (u1ModeSel == TD22D_MODE_FRAME_SEQUENTIAL)
		{
			u1MaskSel = TD22D_FS_RIGHT;
		}		
	}
	#endif
	
	#if defined(CC_MT5368)
	if (BSP_GetIcVersion() < IC_VER_5368_AB)
	{
		if (u1ModeSel == TD22D_MODE_SIDE_BY_SIDE)
		{
			u1MaskSel = TD22D_SBS_RIGHT;
		}

		if (u1ModeSel == TD22D_MODE_TOP_AND_BOTTOM)
		{
			u1MaskSel = TD22D_TB_TOP;
		}

		if (u1ModeSel == TD22D_MODE_FRAME_SEQUENTIAL)
		{
			u1MaskSel = TD22D_FS_RIGHT;
		}		
	}
	#endif
		
	if (u1VdpId == SV_VP_MAIN)
	{
		vRegWriteFldAlign(PRE_3D22D_MAIN_00, 0, PRE_3D22D_MAIN_00_R_B3D22D_EN_M); // before setting 3D-to-2D parameter, disable 3D-to-2D 
		vRegWriteFldAlign(PRE_3D22D_MAIN_00, u2Width, PRE_3D22D_MAIN_00_R_UIIMGWIDTH_M);
		vRegWriteFldAlign(PRE_3D22D_MAIN_00, u2Height, PRE_3D22D_MAIN_00_R_UIIMGHEIGHT_M);
		vRegWriteFldAlign(PRE_3D22D_MAIN_00, u1ModeSel, PRE_3D22D_MAIN_00_R_UI3DMODESEL_M);
		vRegWriteFldAlign(PRE_3D22D_MAIN_00, u1MaskSel, PRE_3D22D_MAIN_00_R_BMASKSEL_M);
		vRegWriteFldAlign(PRE_3D22D_MAIN_00, u1TD22DEn, PRE_3D22D_MAIN_00_R_B3D22D_EN_M); // after setting 3D-to-2D parameter, enable 3D-to-2D 

        if(gMain3DTo2DInfo.u1TD22DEn != u1TD22DEn)
        {
            _vDrvVideoSetMute(MUTE_MODULE_SCPOS_TV3D, u1VdpId, 6, TRUE);
        }
        
		gMain3DTo2DInfo.u2Width = u2Width;
		gMain3DTo2DInfo.u2Height = u2Height;
		gMain3DTo2DInfo.u1IsIntr = u1IsIntr;
		gMain3DTo2DInfo.u1TD22DEn = u1TD22DEn;
		gMain3DTo2DInfo.u1ModeSel = u1ModeSel;
		gMain3DTo2DInfo.u1MaskSel = u1MaskSel;       
	}
	else
	{
		vRegWriteFldAlign(PRE_3D22D_PIP_00, 0, PRE_3D22D_PIP_00_R_B3D22D_EN_P); // before setting 3D-to-2D parameter, disable 3D-to-2D 
		vRegWriteFldAlign(PRE_3D22D_PIP_00, u2Width, PRE_3D22D_PIP_00_R_UIIMGWIDTH_P);
		vRegWriteFldAlign(PRE_3D22D_PIP_00, u2Height, PRE_3D22D_PIP_00_R_UIIMGHEIGHT_P);
		vRegWriteFldAlign(PRE_3D22D_PIP_00, u1ModeSel, PRE_3D22D_PIP_00_R_UI3DMODESEL_P);
		vRegWriteFldAlign(PRE_3D22D_PIP_00, u1MaskSel, PRE_3D22D_PIP_00_R_BMASKSEL_P);
		vRegWriteFldAlign(PRE_3D22D_PIP_00, u1TD22DEn, PRE_3D22D_PIP_00_R_B3D22D_EN_P); // after setting 3D-to-2D parameter, enable 3D-to-2D 
        
		gPip3DTo2DInfo.u2Width = u2Width;
		gPip3DTo2DInfo.u2Height = u2Height;
		gPip3DTo2DInfo.u1IsIntr = u1IsIntr;
		gPip3DTo2DInfo.u1TD22DEn = u1TD22DEn;
		gPip3DTo2DInfo.u1ModeSel = u1ModeSel;
		gPip3DTo2DInfo.u1MaskSel = u1MaskSel;
	}		

    if((u1ModeSel == TD22D_MODE_TOP_AND_BOTTOM)&&(u1MaskSel == TD22D_TB_TOP) && (SV_ON == u1TD22DEn))                                        
    {
        fgFldInv[u1VdpId]=SV_ON;
    }
    else
    {
        fgFldInv[u1VdpId]=SV_OFF;
    }
		
	return 1;
}

UINT32 vDrvTDIsFldInv(UINT32 u1VdpId)
{
    return fgFldInv[u1VdpId];
}

void vDrvTD22DInit(void)
{
    vDrvTdNaviLoadRegTbl(TD22D_INIT);
    u1DrvTD22DSetMode(SV_VP_MAIN, SV_OFF, TD22D_MODE_FRAME_SEQUENTIAL, TD22D_FS_LEFT);
    u1DrvTD22DSetMode(SV_VP_PIP, SV_OFF, TD22D_MODE_FRAME_SEQUENTIAL, TD22D_FS_LEFT);
}

