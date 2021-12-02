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
 * $RCSfile: drv_ttd.c $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/
#include "general.h"
#include "x_assert.h"
//#include "sv_const.h"
#include "hw_vdoin.h"
#include "hw_ttd.h"
#include "hw_sw.h"
#include "hw_ycproc.h"
#include "drv_ttd.h"
#include "drv_display.h"
#include "nptv_debug.h"
#include "drv_video.h"
#include "drv_tdtv_drvif.h"
#include "panel.h"
#ifdef CC_MT5396
#include "drv_mjc.h"
#include "hw_mjc.h"
#endif

/*----------------------------------------------------------------------------*
 * Global/Static variables
 *----------------------------------------------------------------------------*/
UINT8 g_u1SceneChangedFlag = SV_FALSE;
UINT8 g_u1SceneChangedCounter = 0;
UINT8 g_u1SceneChangedCounterThr = 1;
UINT8 g_u1ParameterChange = 0;
UINT8 g_u1ImageSafetyLevel = 0; // 0: Off, 1: Low, 2: Middle, 3: High
UINT8 g_u1DepthOfField = 16; // 0~32
UINT8 g_u1ProtrudeFromScreen = 16; // 0~32
UINT8 g_u1DistanceToTV = 10; // 2~18
UINT8 g_u1LREyeInverse = 0; // 0, 1
UINT8 g_u1GlobalDepth = 16; // 0~32
UINT8 g_u1LocalDepth = 16; // 0~32
UINT8 g_u1DsprtToDepth = 0; // 0~255 
 
UINT32 g_u4VpHistDistTh = 0;

UINT8 g_u1Input3D = 0; 	// 0:TTD, 1:3D
UINT8 g_u1Ver = VER_5368_ES;
UINT8 g_u1DepthCtrlReady = 0;

TTD_INFO gTTDInfo;

CODE TTD_REGTBL_T TTD_INIT[] =
{
#if defined(CC_MT5396)
    {TTD_00, 0x000001AF, 0x000001FF},
    {TTD_01, 0x00008000, 0x00018161},
    {TTD_02, 0x00438780, 0x83FFFFFF},
    {TTD_03, 0x00000000, 0x3FFF00E5},
    {TTD_04, 0x00000000, 0x0000001F},
    {TTDVP_00, 0x08A6D83C, 0x7FFFFFFF},
    {TTDVP_01, 0x2BC57808, 0xFFFFFFF8},
    {TTDVP_02, 0x4B054010, 0x7FFFFFFF},
    {TTDVP_03, 0x10A003F4, 0x7FFFFFFF},
    {TTDVP_04, 0x000D4820, 0x7FFFFFF0},
    {TTDVP_05, 0x31B0C800, 0x7FFFFFFF},
    {TTDVP_06, 0x06480100, 0x7FFFFFF0},
    {TTDVP_08, 0x2BC2BC04, 0xFFFFFFFF},
    {TTDVP_09, 0x0258A104, 0xFFFFFFFC},
    {TTDVP_10, 0x00000000, 0xFFFFFFFF},
    {TTDVP_11, 0x00000000, 0xFFFFFFFE},
    {TTDVP_12, 0x64000A28, 0xFFFFFFF8},
    {TTDVP_13, 0x4D7F85AD, 0xFFFFFFFF},
    {TTDVP_14, 0x37BE2A10, 0xFFFFFFFC},
    {TTDVP_15, 0x21862100, 0xFFFFFFF0},
    {TTDVP_16, 0x0064EFE0, 0xFFFFFFF0},
    {TTDVP_17, 0x86EC8000, 0xFFFFF000},
    {TTDDG_00, 0x10000000, 0x7F800000},
    {TTDDG_01, 0x00060400, 0x001FFF80},
    {TTDDG_02, 0x00000000, 0xFFF00FFF},
    {TTDDG_03, 0x77F00437, 0xFFF00FFF},
    {TTDDG_04, 0x00000000, 0xFFF00FFF},
    {TTDDG_05, 0x77F00437, 0xFFF00FFF},
    {TTDDG_06, 0x00000000, 0xFFF00FFF},
    {TTDDG_07, 0x07FC0000, 0xFFFFFC00},
    {TTDDG_08, 0x00000022, 0x0000003E},
    {TTDDG_09, 0x00000888, 0x00000FFF},
    {TTDDG_10, 0x00008060, 0x0000FFFF},
    {TTDIR_00, 0x00006025, 0x07FF7F3F},
    {TTDIR_01, 0x000011F1, 0x000071F1},
    {TTDIR_02, 0x0000F000, 0x0000FF13},
    {TTDIR_03, 0x00000000, 0x00000001},
    {TTDIR_04, 0x0080FFFF, 0x00FFFFFF},
#endif
#if defined(CC_MT5368)
    {TTD_00, 0x0000000A, 0xFFFFFFFF},
    {TTD_01, 0x00000006, 0x7FFFFFE7},
    {TTD_02, 0x80438780, 0xE0FFFFFF},
    {TTD_03, 0x00640220, 0xFFFFFFFF},
    {TTD_04, 0x78006400, 0xFFFFFFFF},
    {TTD_05, 0x0000002B, 0xFFFF007F}, // TTD_05[26] TTD_BYPASS = 0
    {TTD_06, 0x00000556, 0xFFFFFFFF},
    {TTDDG_00, 0x10000000, 0xFF800000},
    {TTDDG_01, 0x29060400, 0x7FFFFF80},
    {TTDDG_02, 0x00000000, 0x7FFFFE00},
    {TTDDG_03, 0x07F86E00, 0x7FFFFE00},
    {TTDDG_04, 0x00000000, 0x7FFFFE00},
    {TTDDG_05, 0x77F86FFE, 0x7FFFFFFF},
    {TTDDG_06, 0x00000000, 0x07FF07FF},
    {TTDIR_00, 0x6096BCFE, 0xFFFFFFFF},
    {TTDIR_01, 0x00000064, 0x000007FF},
    {TTDIR_02, 0x000F00F0, 0x03FFFFFF},
    {TTDVP_00, 0x08A6D800, 0x3FFFFFC0},
    {TTDVP_01, 0x2BC57800, 0x7FFFFE00},
    {TTDVP_02, 0x4B054014, 0x7FFFFFFF},
    {TTDVP_03, 0x10A003F4, 0x7FFFFFFF},
    {TTDVP_04, 0x00000000, 0x7FF00000},
    {TTDVP_05, 0x00050000, 0x000FFFFE},
    {TTDVP_06, 0x16820040, 0x7FFFFFFF},
    {TTDVP_08, 0x15E15E00, 0x7FFFFF80},
#endif
#if defined(CC_MT5389)
    {TTD_00, 0x000015AF, 0x007DFFFF},
    {TTD_01, 0x00008000, 0x00018161},
    {TTD_02, 0x00438780, 0x81FFFFFF},
    {TTD_03, 0x00640000, 0x3FFF003D},
    {TTD_04, 0x08800000, 0x1FFDFFFF}, // TTD_04[15] TTD_BYPASS = 0
    {TTD_06, 0x08808000, 0xFFFFFFFF},
    {TTDDG_00, 0x10000000, 0x7F800000},
    {TTDDG_01, 0x00060400, 0x001FFF80},
    {TTDDG_02, 0x00000000, 0xFFF00FFF},
    {TTDDG_03, 0x77F00437, 0xFFF00FFF},
    {TTDDG_04, 0x00000000, 0xFFF00FFF},
    {TTDDG_05, 0x77F00437, 0xFFF00FFF},
    {TTDDG_06, 0x00000000, 0xFFF00FFF},
    {TTDDG_07, 0x07FC0000, 0xFFFFFC00},
    {TTDIR_00, 0x00006025, 0x00037F3F},
    {TTDIR_01, 0x000011F1, 0x000071F1},
    {TTDIR_02, 0x0000F000, 0x01FFFF13},
    {TTDVP_00, 0x08A6D83C, 0x7FFFFFFF},
    {TTDVP_01, 0x2BC57808, 0xFFFFFFF8},
    {TTDVP_02, 0x4B054010, 0x7FFFFFFF},
    {TTDVP_03, 0x10A003F4, 0x7FFFFFFF},
    {TTDVP_04, 0x000D4820, 0x7FFFFFF0},
    {TTDVP_05, 0x31B0C800, 0x7FFFFFFF},
    {TTDVP_06, 0x06480100, 0x7FFFFFF0},
    {TTDVP_08, 0x2BC2BC04, 0xFFFFFFFF},
    {TTDVP_09, 0x0258A104, 0xFFFFFFFC},
    {TTDVP_10, 0x00000000, 0xFFFFFFFE},
    {TTDVP_11, 0x00000000, 0xFFFFFFFC},
    {TTDVP_12, 0x64000A28, 0xFFFFFFF8},
    {TTDVP_13, 0x4D7F85AD, 0xFFFFFFFF},
    {TTDVP_14, 0x37BE2A10, 0xFFFFFFFC},
    {TTDVP_15, 0x21862100, 0xFFFFFFF0},
    {TTDVP_16, 0x0064EFE0, 0xFFFFFFF0},
    {TTDVP_17, 0x86EC8000, 0xFFFFF000},
#endif
    {TTD_REGTBL_END, 0x00000000, 0x00000000}

};

static void vDrvTTDLoadRegTbl(TTD_REGTBL_T * prRegTbl);
static void vDrvTTDCheckTransitionByAPL(void);
static UINT8 u1DrvTTDIsSceneChanged(void);
static void vDrvTTDCheckGlobalModel(void);
static void vDrvTTDCheckLocalInverseMode(void);
static void vDrvTTDSetInverseControl(UINT8 onOff, UINT8 isInverse);
static UINT8 u1DrvTTDGetCurrentSceneMode(void);
static void vDrvTTDSetInputSize(UINT16 u2Width,UINT16 u2Height);
static void vDrvTTDSetDGWidnow(UINT16 u2Width,UINT16 u2Height);
static void vDrvTTDSetDemoWindow(UINT16 u2Width,UINT16 u2Height);
static void vDrvTTDSetLocalInverseThr(UINT16 u2Width,UINT16 u2Height);
static void vDrvTTDSetHistogramBinSize(UINT16 u2Width,UINT16 u2Height);
static void vDrvTTDSetVPSlightMoveRange(UINT16 u2Width,UINT16 u2Height);
static void vDrvTTDSetDisparityGain(UINT8 gain);
static void vDrvTTDSetDepthRangeRatio(UINT8 ratio);
static void vDrvTTDSetConvergenceDepth(UINT8 depth);
static void vDrvTTDSetOcclusionGain(UINT8 gain);
static void vDrvTTDSetVPTile(UINT16 u2Width,UINT16 u2Height);
static void vDrvTTDSetRgnHistogram(UINT16 u2Width,UINT16 u2Height);
static void vDrvTTDSetOutLRDelay(UINT8 u1Delay);
static void vDrvTTDRgnHistogramEnable(UINT8 bEnable);
static UINT8 bDrvTTDIsModeChanged(void);
static void vDrvTTDSetHorizontalParameter(UINT16 u2Width);
static void vDrvTTDSetDIRBDelayOut(UINT16 delayout);
static void vDrvTTDSetLLRRMode(UINT8 mode);
static UINT8 bDrvDisparityToDepth(INT8 i1Dsprt);
static void vDrvTTDSetHistFlatnessCtrlEn(UINT8 u1OnOff);
static void vDrvTTDSetHistFlatness(UINT8 u1Value);
static void vDrvTTDSetGlobalWeight(UINT8 u1Value);
static void vDrvTTDSetSkinAddup(UINT16 u2Value);
static void vDrvTTDSetLocalInverseCtrlEn(UINT8 u1OnOff);
static void vDrvTTDSetSceneCtrlEn(UINT8 u1OnOff);
static void vDrvTTDSetSceneWeight(UINT8 u1Value);
static void vDrvTTDSetBlackEn(UINT8 u1OnOff);
static void vDrvTTDSetBlackNum(UINT8 u1Value);
static void vDrvTTDSetBlackSingleEn(UINT8 u1OnOff);
static void vDrvTTDSetBlackSingleInv(UINT8 u1OnOff);
static void vDrvTTDSetBlackSingleNum(UINT8 u1Value);
static UINT8 u1DrvTTDGetShiftBlack(void);
static UINT8 u1DrvTTDGetShiftBlackByFW(void);
static void vDrvTTDSetDsprtBlackClpEn(UINT8 u1OnOff);
static void vDrvTTDSetDsprtMin(UINT8 u1Value);
static void vDrvTTDSetDsprtMax(UINT8 u1Value);
static void vDrvTTDSetTfuGlobalEn(UINT8 u1OnOff);
static void vDrvTTDSetTfuGlobalOffset(UINT8 u1Value);
static void vDrvTTDSetTfuEn(UINT8 u1OnOff);
static void vDrvTTDSetTfuBase(UINT8 u1Value);
static void vDrvTTDSetColorEnhance(UINT8 u1OnOff);
static void vDrvTTDSet444To422En(UINT8 u1OnOff);
static void vDrvTTDSetTtdEn(UINT8 u1OnOff);
static UINT16 u2DrvTTDGetFnlSceneVpX0(void);
static UINT16 u2DrvTTDGetFnlSceneVpY0(void);
static UINT8 u1DrvTTDGetFnlSceneMode(void);
static UINT8 u1DrvTTDGetHistFlatness(void);
static UINT8 u1DrvTTDGetVertFlipEn(void);
static void vDrvTTDSetHistDistTh(UINT16 u2Value);
static void vDrvTTDSetVpX(UINT16 u2Value);
static void vDrvTTDSetVpY(UINT16 u2Value);
static void vDrvTTDSetSceneMode(UINT8 u1Value);
static UINT8 u1DrvTTDGetDepthRangeRatio(void);
static UINT8 u1DrvTTDGetConvDepth(void);
static UINT32 u4DrvTTDGetImageApl(void);
static UINT32 u4DrvTTDGetCidInvThr(void);
static void vDrvTTDSetTransitionCtrlEn(UINT8 u1OnOff);
static void vDrvTTDSetTransitionStat(UINT8 u1Value);
/*----------------------------------------------------------------------------*
 * Function Members
 *----------------------------------------------------------------------------*/
static void vDrvTTDLoadRegTbl(TTD_REGTBL_T * prRegTbl)
{
    while (prRegTbl->u2Addr != (UINT32)TTD_REGTBL_END)
    {
        vIO32Write4BMsk(prRegTbl->u2Addr, prRegTbl->u4Value, prRegTbl->u4Mask);
        prRegTbl++;
    }
}


void vDrvTTDInit(void)
{
	vDrvTTDLoadRegTbl(TTD_INIT);
	//vIO32WriteFldAlign(0xF005d810, 1, Fld(1,15,AC_MSKB1));

    gTTDInfo.ttdEnabled = SV_FALSE;
    gTTDInfo.ttdModeChangeFlag = SV_FALSE; 
    gTTDInfo.ttdRstCounter = 0;

    vDrvTTDSetFWOnOff(SV_ON);
    vDrvTTDSetDisparityGain(DISPARITY_GAIN_TTD);
	vDrvTTDSetDepthRangeRatio(DEPTH_RANGE_RATIO_TTD);
    vDrvTTDSetOcclusionGain(OCCLUSION_GAIN_TTD);    

	vDrvTTDSetStereoComfortRange(STEREO_COMFORT_FACTOR);

    vDrvTTDSetDIRBDelayOut(DIBR_DELAYOUT_PR);

    vDrvTTDSetConvergenceDepthUI(CONVERGENCE_DEPTH_TTD);
    
#if defined(CC_MT5396) || defined(CC_MT5389)
    vDrvTTDLSVInit(); 
#endif

    vDrvTTDSetPicSize(PANEL_GetPanelWidth(), PANEL_GetPanelHeight());

	// LLRR Mode always Off
    vDrvTTDSetLLRRMode(SV_OFF);

	if (IS_LVDS_DISP_3D_POLARIZED)
	{
		vDrvTTDSetOutLRDelay(1);
	}
	else
	{
		vDrvTTDSetOutLRDelay(1);
	}

	g_u1DsprtToDepth = bDrvDisparityToDepth(0);
}

void vDrvTTDSetMode(UINT8 bMode)
{
#if defined(CC_MT5396)

	g_u1DepthCtrlReady = 1;

	if ((BSP_GetIcVersion() >= IC_VER_5396_AB))
	{
		g_u1Ver = VER_5396_MP;
	}
	else
	{
		g_u1Ver = VER_5396_ES;
	}
	
	if (MJC_2D_TO_FS == bMode)
	{
		vIO32WriteFldAlign(TTD_02, 0, TTD_02_SRC_3D_MODE);
		vIO32WriteFldAlign(TTD_02, 0, TTD_02_SRC_2D_NRPT);
		vIO32WriteFldAlign(TTDIR_00, 0, TTDIR_00_IR_PR_MODE);
		vDrvTTDSet2DInput();
	}
	else if (MJC_FS_TO_FS == bMode)
	{
		vIO32WriteFldAlign(TTD_02, 1, TTD_02_SRC_3D_MODE);
		vIO32WriteFldAlign(TTD_02, 0, TTD_02_SRC_2D_NRPT);
		vIO32WriteFldAlign(TTDIR_00, 0, TTDIR_00_IR_PR_MODE);
		vDrvTTDSet3DInput();
	}
	else if (SCL_2D_TO_FS == bMode) // with MT8283
	{
		vIO32WriteFldAlign(TTD_02, 0, TTD_02_SRC_2D_NRPT);
		vIO32WriteFldAlign(TTD_02, 1, TTD_02_SRC_3D_MODE);
		vIO32WriteFldAlign(TTDIR_00, 0, TTDIR_00_IR_PR_MODE);
		vDrvTTDSet2DInput();
	}
	else
	{
    	LOG(0,"Not Correct Mode for MT5396 !\n");
	}		
#endif

#if defined(CC_MT5368)
	g_u1DepthCtrlReady = 0;
	
	if ((BSP_GetIcVersion() >= IC_VER_5368_AB))
	{
		g_u1Ver = VER_5368_MP;
	}
	else
	{
		g_u1Ver = VER_5368_ES;
	}

	#if 0
	if (BSP_GetIcVersion() >= IC_VER_5368_AB)
	{
		if (SCL_2D_TO_PR == bMode)
		{
			vIO32WriteFldAlign(TTD_02, 1, TTD_02_SRC_2D_NRPT);
			vIO32WriteFldAlign(TTD_02, 0, TTD_02_SRC_3D_MODE);
			vIO32WriteFldAlign(TTDIR_00, 1, TTDIR_00_IR_PR_MODE);
			vDrvTTDSet2DInput();
		}
		else if (SCL_PR_TO_PR == bMode)
		{
			vIO32WriteFldAlign(TTD_02, 1, TTD_02_SRC_2D_NRPT);
			vIO32WriteFldAlign(TTD_02, 1, TTD_02_SRC_3D_MODE);
			vIO32WriteFldAlign(TTDIR_00, 1, TTDIR_00_IR_PR_MODE);
			vDrvTTDSet3DInput();
		}	
		else if (SCL_2D_TO_FS == bMode) // with MT8283
		{
			vIO32WriteFldAlign(TTD_02, 0, TTD_02_SRC_2D_NRPT);
			vIO32WriteFldAlign(TTD_02, 1, TTD_02_SRC_3D_MODE);
			vIO32WriteFldAlign(TTDIR_00, 0, TTDIR_00_IR_PR_MODE);
			vDrvTTDSet2DInput();
		}
		else
		{
    		LOG(0,"Not Correct Mode for MT5368 !\n");
		}
	}
	else
	#endif
	{
		if (SCL_2D_TO_PR == bMode)
		{
			vIO32WriteFldAlign(TTD_01, 1, TTD_01_PR_MODE);
			vDrvTTDSet2DInput();
		}
		else
		{
    		LOG(0,"Not Correct Mode for MT5368 !\n");
		}
	}

#endif

#if defined(CC_MT5389)
	g_u1DepthCtrlReady = 1;

	g_u1Ver = VER_5389;

 		if (SCL_2D_TO_PR == bMode)
		{
			vIO32WriteFldAlign(TTD_00, 1, TTD_00_SRC_2D_NRPT);
			vIO32WriteFldAlign(TTD_02, 0, TTD_02_SRC_3D_MODE);
			vIO32WriteFldAlign(TTDIR_00, 1, TTDIR_00_IR_PR_MODE);
			vDrvTTDSet2DInput();
		}
		else if (SCL_PR_TO_PR == bMode)
		{
			vIO32WriteFldAlign(TTD_00, 1, TTD_00_SRC_2D_NRPT);
			vIO32WriteFldAlign(TTD_02, 1, TTD_02_SRC_3D_MODE);
			vIO32WriteFldAlign(TTDIR_00, 1, TTDIR_00_IR_PR_MODE);
			vDrvTTDSet3DInput();
		}	
		else if (SCL_2D_TO_FS == bMode) // with MT8283
		{
			vIO32WriteFldAlign(TTD_00, 0, TTD_00_SRC_2D_NRPT);
			vIO32WriteFldAlign(TTD_02, 1, TTD_02_SRC_3D_MODE);
			vIO32WriteFldAlign(TTDIR_00, 0, TTDIR_00_IR_PR_MODE);
			vDrvTTDSet2DInput();
		}
		else
		{
    		LOG(0,"Not Correct Mode for MT5389!\n");
		}   
#endif

}

void vDrvTTDSet3DInput(void)
{
	g_u1Input3D = 1;
    g_u1ParameterChange = 1;
	Printf("vDrvTTDSet3DInput, g_u1Input3D = %d, \n", g_u1Input3D);

    vDrvTTDSetSceneCtrlEn(1);
	vDrvTTDSetSceneWeight(0);
	vDrvTTDSetHistFlatnessCtrlEn(1);
	vDrvTTDSetHistFlatness(10);
	vDrvTTDSetGlobalWeight(32);
	vDrvTTDSetLocalWeight(0);
	vDrvTTDSetLocalInverseCtrlEn(1);

	vDrvTTDSetBlackEn(1);
	vDrvTTDSetBlackSingleEn(1);
	vDrvTTDSetDsprtBlackClpEn(0);

	vDrvTTDSetTfuGlobalEn(0);
	vDrvTTDSetTfuEn(0);
	vDrvTTDSetColorEnhance(0);
    
	vDrvTTDSetPrInfo(0);

	vDrvTTDSetFWOnOff(SV_OFF);
	
	if (g_u1DepthCtrlReady)
	{
		vDrvTTDActGlobalShift(SV_OFF);
		vDrvTTDSetFGBGDetect(SV_ON);
		vDrvTTDSet3DDepthCtrl(SV_ON);
	}
	else
	{
		vDrvTTDActGlobalShift(SV_ON);
		vDrvTTDSetFGBGDetect(SV_OFF);
		vDrvTTDSet3DDepthCtrl(SV_OFF);
	}
}

void vDrvTTDSet2DInput(void)
{
	g_u1Input3D = 0;
    g_u1ParameterChange = 1;
	Printf("vDrvTTDSet2DInput, g_u1Input3D = %d, \n", g_u1Input3D);
	
	vDrvTTDSetSceneCtrlEn(1);
	vDrvTTDSetSceneWeight(31);
	vDrvTTDSetHistFlatnessCtrlEn(0);
	vDrvTTDSetHistFlatness(10);
	vDrvTTDSetGlobalWeight(8);
	vDrvTTDSetLocalWeight(24);
	vDrvTTDSetLocalInverseCtrlEn(0);

	vDrvTTDSetBlackEn(1);
	vDrvTTDSetBlackSingleEn(1);
	vDrvTTDSetDsprtBlackClpEn(0);
	
	vDrvTTDSetDepthBias(0);
	vDrvTTDSetConvergenceDepth(CONVERGENCE_DEPTH_TTD);
	vDrvTTDSetDepthRangeRatio(DEPTH_RANGE_RATIO_TTD);
	vDrvTTDSetDisparityGain(DISPARITY_GAIN_TTD);

	vDrvTTDSetTfuGlobalEn(1);
	vDrvTTDSetTfuEn(1);
	vDrvTTDSetColorEnhance(1);

	vDrvTTDSetPrInfo(0);
	
	vDrvTTDSetFWOnOff(SV_ON);
	vDrvTTDSetFGBGDetect(SV_OFF);
	vDrvTTDSet3DDepthCtrl(SV_OFF);
}

void vDrvTTDSetFGBGDetect(UINT8 u1OnOff)
{
	vIO32WriteFldAlign(TTD_FW_REG_00, u1OnOff, TTD_FW_HIST_EN); 
}


void vDrvTTDActGlobalShift(UINT8 u1OnOff)
{
	if(u1OnOff)
	{
		vDrvTTDSetDepthBias(224);
		vDrvTTDSetConvergenceDepth(192);
		vDrvTTDSetDepthRangeRatio(0);
		vDrvTTDSetDisparityGain(0);
	}
	else
	{
		vDrvTTDSetDepthBias(CONVERGENCE_DEPTH_3D);
		vDrvTTDSetConvergenceDepth(CONVERGENCE_DEPTH_3D);
		vDrvTTDSetDepthRangeRatio(DEPTH_RANGE_RATIO_3D);
		vDrvTTDSetDisparityGain(DISPARITY_GAIN_3D);
	}	
}


void vDrvTTDSetEnable(UINT8 bEnable)
{
    LOG(0,"Set TTD to %s\n",(bEnable==SV_TRUE)?"ON":"OFF");

    // if Enable/Disable status is changed, set a flag to do MJC output clk reset in output vsync.
    if(gTTDInfo.ttdEnabled != bEnable)
    {
        gTTDInfo.ttdEnabled = bEnable;
        gTTDInfo.ttdModeChangeFlag = SV_TRUE;
    }
    
    // set TTD enable/disable
    if(bEnable == SV_TRUE)
    {
    	vDrvTTDSet444To422En(1);
    	vDrvTTDSetTtdEn(1);
    }
    else
    {
    	vDrvTTDSet444To422En(0);
        vDrvTTDSetTtdEn(0);
    }	
}


static UINT8 bDrvTTDIsModeChanged(void)
{
    UINT8 ret = SV_FALSE;
    
#if defined(CC_MT5396)
    if(gTTDInfo.ttdModeChangeFlag == SV_TRUE)
    {
        ret = SV_TRUE;
    }
#endif

    return ret;
}

void vDrvTTDCheckRstCtrl(void)
{
    if(bDrvTTDIsModeChanged() == SV_TRUE)    
    {
#if defined(CC_MT5396)
        if(gTTDInfo.ttdRstCounter == 0)
        {
            LOG(6,"MJC Clock Rst\n",gTTDInfo.ttdRstCounter);
            vIO32WriteFldAlign(IO_VIRT + MJC_SYS_CLK_CTRL, 4, MJC_RST_CTRL);
        }
        else if(gTTDInfo.ttdRstCounter > MAX_TTD_RST_COUNTER)        
        {
            vIO32WriteFldAlign(IO_VIRT + MJC_SYS_CLK_CTRL, 0, MJC_RST_CTRL);
            gTTDInfo.ttdModeChangeFlag = SV_FALSE;
            gTTDInfo.ttdRstCounter = 0;
            LOG(6,"Stop MJC Clock Rst\n");
            return;
        }
        gTTDInfo.ttdRstCounter++;
#endif
    }
}

void vDrvTTDSetPicSize(UINT16 u2Width, UINT16 u2Height)
{
    //keep w & h in global
    gTTDInfo.ttdInputWidth = u2Width;
    gTTDInfo.ttdInputHeight = u2Height;
    //set input size
    vDrvTTDSetInputSize(u2Width,u2Height);
    //set dg window
    vDrvTTDSetDGWidnow(u2Width,u2Height);
    //set demo window
    vDrvTTDSetDemoWindow(u2Width,u2Height);
    //set local inverse threshold
    vDrvTTDSetLocalInverseThr(u2Width,u2Height);
    //set histogram bin size
    vDrvTTDSetHistogramBinSize(u2Width,u2Height);

    vDrvTTDSetVPSlightMoveRange(u2Width,u2Height);

    vDrvTTDSetVPTile(u2Width,u2Height);
	vDrvTTDRgnHistogramEnable(SV_ON);
	vDrvTTDSetRgnHistogram(u2Width,u2Height);

	vDrvTTDSetHorizontalParameter(u2Width);
    //set thr for FW scene change
    gTTDInfo.ttdSceneChangeThr = (u2Width / 8) * (u2Height / 8) * 5;
}

static void vDrvTTDSetInputSize(UINT16 u2Width,UINT16 u2Height)
{
    vIO32WriteFldMulti(TTD_02,P_Fld(u2Width, TTD_02_IMG_WIDTH)| P_Fld(u2Height, TTD_02_IMG_HEIGHT));
}
    
static void vDrvTTDSetDGWidnow(UINT16 u2Width,UINT16 u2Height)
{
    vIO32WriteFldMulti(TTDDG_02,P_Fld(0,TTDDG_02_DG_LOCAL_X_STR) | P_Fld(0, TTDDG_02_DG_LOCAL_Y_STR));
    vIO32WriteFldMulti(TTDDG_03,P_Fld(u2Width-1,TTDDG_03_DG_LOCAL_X_END) | P_Fld(u2Height-1, TTDDG_03_DG_LOCAL_Y_END));
}

static void vDrvTTDSetDemoWindow(UINT16 u2Width,UINT16 u2Height)
{
    vIO32WriteFldMulti(TTDDG_04,P_Fld(0,TTDDG_04_DG_DEMO_X_STR) | P_Fld(0, TTDDG_04_DG_DEMO_Y_STR));
    vIO32WriteFldMulti(TTDDG_05,P_Fld(u2Width-1,TTDDG_05_DG_DEMO_X_END) | P_Fld(u2Height-1, TTDDG_05_DG_DEMO_Y_END));
}

static void vDrvTTDSetLocalInverseThr(UINT16 u2Width,UINT16 u2Height)
{
    UINT32 u4Thr;
    //thr for condition to inverse local depth map
    u4Thr = (((UINT32)(u2Width / 8)) * ((UINT32)(u2Height / 8))) * VP_CID_INV_TH;
    vIO32WriteFldAlign(TTDVP_00, u4Thr, TTDVP_00_VP_CID_INV_THR);
}

static void vDrvTTDSetHistogramBinSize(UINT16 u2Width,UINT16 u2Height)
{
    UINT16 binSize;
    //binsize thr for flatness statistic
    binSize = (u2Width / 8) * (u2Height / 8) / 32;    
    vIO32WriteFldAlign(TTDVP_03,binSize,TTDVP_03_HIS_AVG_BINSIZE);
}

static void vDrvTTDSetVPSlightMoveRange(UINT16 u2Width,UINT16 u2Height)
{
    UINT8 u1VpSlightMoveRangeX, u1VpSlightMoveRangeY;
    u1VpSlightMoveRangeX = u2Width/3;
    u1VpSlightMoveRangeY = u2Height/3;
    vIO32WriteFldAlign(TTDVP_05, u1VpSlightMoveRangeX, TTDVP_05_VP_SLIGHT_MOVE_RANGE_X);
    vIO32WriteFldAlign(TTDVP_06, u1VpSlightMoveRangeY, TTDVP_06_VP_SLIGHT_MOVE_RANGE_Y);
}

static void vDrvTTDSetVPTile(UINT16 u2Width,UINT16 u2Height)
{
#if defined(CC_MT5396) || defined(CC_MT5389)
    UINT16 u2SbTileWidth, u2SbTileHeight, u2SbTileRestX, u2SbTileRestY;
    UINT16 u2VpEdgeXLastBlockValid, u2VpEdgeYLastBlockValid;

    u2SbTileWidth  = (u2Width/8)/4;
    u2SbTileHeight = (u2Height/8)/5;
    u2SbTileRestX = (u2Width/8) - (u2Width/8)/4*4;
    u2SbTileRestY = (u2Height/8) - (u2Height/8)/5*5;
    u2VpEdgeXLastBlockValid = 1;
    u2VpEdgeYLastBlockValid = 1;

    vIO32WriteFldAlign(TTDVP_00,u2SbTileWidth,TTDVP_00_SB_TILE_WIDTH);
    vIO32WriteFldAlign(TTDVP_05,u2SbTileHeight,TTDVP_05_SB_TILE_HEIGHT);
    vIO32WriteFldAlign(TTDVP_01,u2SbTileRestX,TTDVP_01_SB_TILE_REST_X);
    vIO32WriteFldAlign(TTDVP_01,u2SbTileRestY,TTDVP_01_SB_TILE_REST_Y);
    vIO32WriteFldAlign(TTDVP_04,u2VpEdgeXLastBlockValid,TTDVP_04_VP_EDGE_X_LAST_BLOCK_VALID);
    vIO32WriteFldAlign(TTDVP_04,u2VpEdgeYLastBlockValid,TTDVP_04_VP_EDGE_Y_LAST_BLOCK_VALID);
#endif
}

static void vDrvTTDSetRgnHistogram(UINT16 u2Width,UINT16 u2Height)
{
#if defined(CC_MT5396) || defined(CC_MT5389)
    UINT8 u1RegionRestY;
    UINT32 u4VpHistDistTh, u4VpLetterBoxHistTh;
    UINT16 u2VpLastTilePxlWeight, u2Remainder;

    u2Remainder = u2Height - ((u2Height >> 4) << 4);
    u1RegionRestY = (u2Remainder > 0) ? 1 : 0;    
    u2VpLastTilePxlWeight = (u2Remainder > 0) ? (16/u2Remainder) : 1;
	vIO32WriteFldAlign(TTDVP_04, u2VpLastTilePxlWeight,TTDVP_04_VP_LAST_TILE_PXL_WEIGHT);
	vIO32WriteFldAlign(TTDVP_01, u1RegionRestY, TTDVP_01_RH_REGION_REST_Y);

	u4VpHistDistTh = VP_HIST_DIST_TH * ((UINT32)u2Width) / 1920;
    vIO32WriteFldAlign(TTDVP_09, u4VpHistDistTh, TTDVP_09_VP_HIST_DIST_TH);
    g_u4VpHistDistTh = u4VpHistDistTh;

	u4VpLetterBoxHistTh = VP_LETTER_BOX_HIST_TH * ((UINT32)u2Width) / 1920;
    vIO32WriteFldAlign(TTDVP_17, u4VpLetterBoxHistTh, TTDVP_17_VP_LETTERBOX_HIST_TH);
#endif
}

static void vDrvTTDSetOutLRDelay(UINT8 u1Delay)
{
#if defined(CC_MT5396) || defined(CC_MT5389)
    vIO32WriteFldAlign(TTD_01,u1Delay,TTD_01_OUT_LR_DLY);
#endif
}

static void vDrvTTDRgnHistogramEnable(UINT8 bEnable)
{
#if defined(CC_MT5396) || defined(CC_MT5389)
    vIO32WriteFldAlign(TTDVP_00, (!bEnable), TTDVP_00_DIS_RGN_HSTGRM);
#endif
}


static void vDrvTTDSetHorizontalParameter(UINT16 u2Width)
{
#if defined(CC_MT5368)
	UINT16 u2WidthAlign8, u2HTotal;
	UINT8  u1FrmRate;
	
	u1FrmRate = bDrvVideoGetRefreshRate(SV_VP_MAIN);

	switch (u1FrmRate)
	{
	case 25:	
	case 50:	
	case 100:	
		u2HTotal = HLEN_TOTAL_50HZ;
		break;
	case 24:	
	case 30:	
	case 48:	
	case 60:	
	case 96:	
	case 120:
	default:
		u2HTotal = HLEN_TOTAL_TYPI;
		break;
	}

	u2WidthAlign8 = ((u2Width+7) >> 3) << 3;
	
	vIO32WriteFldAlign(TTD_04, u2WidthAlign8, TTD_04_PAD_H_ACTIV);
	vIO32WriteFldAlign(TTD_04, u2HTotal - u2WidthAlign8, TTD_04_PAD_H_BLANK);
#endif
}


static void vDrvTTDSetDIRBDelayOut(UINT16 delayout)
{
#if defined(CC_MT5368)
    vIO32WriteFldAlign(TTDIR_01,delayout,TTDIR_01_IR_DIBR_DELAYOUT);
    vIO32WriteFldAlign(TTD_05, delayout*2 + 20,TTD_05_DELAY_CYCLES);
#endif
#if defined(CC_MT5389)
    vIO32WriteFldAlign(TTD_04, 272,TTD_04_DELAY_CYCLES);
#endif
}


static void vDrvTTDSetDisparityGain(UINT8 gain)
{
    vIO32WriteFldAlign(TTDIR_00,gain,TTDIR_00_IR_DISPARITY_GAIN);
}

static void vDrvTTDSetDepthRangeRatio(UINT8 ratio)
{
    vIO32WriteFldAlign(TTDIR_00,ratio,TTDIR_00_IR_DEPTH_RANGE_RATIO);
}


void vDrvTTDSetConvergenceDepthUI(UINT8 depth)
{
    gTTDInfo.ttdConvergenceDepth = depth;
    vDrvTTDSetConvergenceDepth(depth);
    vDrvTTDSetTfuGlobalOffset((depth >> 1));
}

static void vDrvTTDSetConvergenceDepth(UINT8 depth)
{
#if defined(CC_MT5396) || defined(CC_MT5389)
	vIO32WriteFldAlign(TTD_00,depth,TTD_00_CONVERGENCE_DEPTH);
#endif
#if defined(CC_MT5368)
    vIO32WriteFldAlign(TTDIR_00,depth,TTDIR_00_IR_CONVERGENCE_DEPTH);
#endif
}

static void vDrvTTDSetOcclusionGain(UINT8 gain)
{
#if defined(CC_MT5396) || defined(CC_MT5389)
    vIO32WriteFldAlign(TTDIR_01,gain,TTDIR_01_IR_OCCLUSION_GAIN);
#endif
#if defined(CC_MT5368)
    vIO32WriteFldAlign(TTDIR_00,gain,TTDIR_00_IR_OCCLUSION_GAIN);
#endif
}

static void vDrvTTDSetLLRRMode(UINT8 mode)
{
    if(mode == SV_ON)
    {
        vIO32WriteFldAlign(TTD_01,1,TTD_01_LLRR_MODE);
    }
    else
    {
        vIO32WriteFldAlign(TTD_01,0,TTD_01_LLRR_MODE);
    }
}

static void vDrvTTDSetHistFlatnessCtrlEn(UINT8 u1OnOff)
{
	u1OnOff = (u1OnOff > 0) ? 1 : 0;
#if defined(CC_MT5396) || defined(CC_MT5389)
    vIO32WriteFldAlign(TTDVP_09, u1OnOff, TTDVP_09_VP_HIST_FLATNESS_CONTROL_EN);
#endif
#if defined(CC_MT5368)
    vIO32WriteFldAlign(TTD_02, u1OnOff, TTD_02_HIST_FLATNESS_CONTROL_EN);	
#endif
}

static void vDrvTTDSetHistFlatness(UINT8 u1Value)
{
	u1Value = (u1Value > 31) ? 31 : u1Value;
#if defined(CC_MT5396) || defined(CC_MT5389)
	vIO32WriteFldAlign(TTDVP_09, u1Value, TTDVP_09_VP_HIST_FLATNESS); // No Flat mode
#endif
#if defined(CC_MT5368)
    vIO32WriteFldAlign(TTD_00, u1Value, TTD_00_HIST_FLATNESS);
#endif
}

static void vDrvTTDSetGlobalWeight(UINT8 u1Value)
{
	u1Value = (u1Value > 127) ? 127 : u1Value;
#if defined(CC_MT5396) || defined(CC_MT5389)
    vIO32WriteFldAlign(TTDDG_01, u1Value, TTDDG_01_DG_GLOBAL_WEIGHT);
#endif
#if defined(CC_MT5368)
    vIO32WriteFldAlign(TTDDG_01, u1Value, TTDDG_01_DF_GLOBAL_DEPTH_WEIGHT);
#endif
}

void vDrvTTDSetLocalWeight(UINT8 u1Value)
{
	u1Value = (u1Value > 127) ? 127 : u1Value;
#if defined(CC_MT5396) || defined(CC_MT5389)
    vIO32WriteFldAlign(TTDDG_01, u1Value, TTDDG_01_DG_LOCAL_WEIGHT);    
#endif
#if defined(CC_MT5368)
    vIO32WriteFldAlign(TTDDG_01, u1Value, TTDDG_01_DF_LOCAL_DEPTH_WEIGHT);
#endif
}

static void vDrvTTDSetSkinAddup(UINT16 u2Value)
{
	u2Value = (u2Value > 511) ? 511 : u2Value;
#if defined(CC_MT5396) || defined(CC_MT5389)
    vIO32WriteFldAlign(TTDVP_12, u2Value, TTDVP_12_SKIN_ADDUP);
#endif
}

static void vDrvTTDSetLocalInverseCtrlEn(UINT8 u1OnOff)
{
	u1OnOff = (u1OnOff > 0) ? 1 : 0;
#if defined(CC_MT5396) || defined(CC_MT5389)
    vIO32WriteFldAlign(TTDVP_09, u1OnOff, TTDVP_09_VP_LOCAL_INVERSE_CONTROL_EN);
#endif
}

void vDrvTTDSetDepthBias(UINT8 u1Value)
{
    vIO32WriteFldAlign(TTDDG_00, u1Value, TTDDG_00_DG_DEPTH_BIAS);
}

static void vDrvTTDSetSceneCtrlEn(UINT8 u1OnOff)
{
	u1OnOff = (u1OnOff > 0) ? 1 : 0;
#if defined(CC_MT5396) || defined(CC_MT5389)
    vIO32WriteFldAlign(TTDVP_10, u1OnOff, TTDVP_10_VP_SCENE_CONTROL_EN);
#endif 
#if defined(CC_MT5368)
    vIO32WriteFldAlign(TTD_02, u1OnOff, TTD_02_SCENE_CONTROL_EN);
#endif
}

static void vDrvTTDSetSceneWeight(UINT8 u1Value)
{
	u1Value = (u1Value > 31) ? 31 : u1Value;
#if defined(CC_MT5396) || defined(CC_MT5389)
    vIO32WriteFldAlign(TTDVP_10, u1Value, TTDVP_10_VP_SCENE_WEIGHT);
#endif
}

static void vDrvTTDSetBlackEn(UINT8 u1OnOff)
{
	u1OnOff = (u1OnOff > 0) ? 1 : 0;
#if defined(CC_MT5396)
	vIO32WriteFldAlign(TTDIR_00,u1OnOff,TTDIR_00_IR_BLACK_EN);
#endif
#if defined(CC_MT5389)
    vIO32WriteFldAlign(TTDIR_02,u1OnOff,TTDIR_02_IR_BLACK_EN);
#endif
}

static void vDrvTTDSetBlackNum(UINT8 u1Value)
{
#if defined(CC_MT5396)
	vIO32WriteFldAlign(TTDIR_00, u1Value, TTDIR_00_IR_BLACK_NUM);
#endif
#if defined(CC_MT5389)
    vIO32WriteFldAlign(TTDIR_02,u1Value,TTDIR_02_IR_BLACK_NUM);
#endif

}

static void vDrvTTDSetBlackSingleEn(UINT8 u1OnOff)
{
	u1OnOff = (u1OnOff > 0) ? 1 : 0;
#if defined(CC_MT5396)
	if ((BSP_GetIcVersion() >= IC_VER_5396_AB))
	{
		vIO32WriteFldAlign(TTDIR_03, u1OnOff, TTDIR_03_IR_BLACK_SINGLE_EN);
	}	
#endif
}

static void vDrvTTDSetBlackSingleInv(UINT8 u1OnOff)
{
	u1OnOff = (u1OnOff > 0) ? 1 : 0;
#if defined(CC_MT5396)
	if ((BSP_GetIcVersion() >= IC_VER_5396_AB))
	{
		vIO32WriteFldAlign(TTDIR_03, u1OnOff, TTDIR_03_IR_BLACK_SINGLE_INV);
	}	
#endif
}

static void vDrvTTDSetBlackSingleNum(UINT8 u1Value)
{
#if defined(CC_MT5396)
	if ((BSP_GetIcVersion() >= IC_VER_5396_AB))
	{
		vIO32WriteFldAlign(TTDIR_03, u1Value, TTDIR_03_IR_BLACK_SINGLE_NUM);
	}	
#endif
}

static UINT8 u1DrvTTDGetShiftBlack(void)
{
	UINT8 u1ShiftBlack = 0;
#if defined(CC_MT5396)
	if ((BSP_GetIcVersion() >= IC_VER_5396_AB))
	{
    	u1ShiftBlack = IO32ReadFldAlign(STA_TTDIR_00, STA_TTDIR_00_STA_IR_SHIFT_BLACK);
	}	
#endif
#if defined(CC_MT5389)
    u1ShiftBlack = IO32ReadFldAlign(STA_TTDIR_00, STA_TTDIR_00_STA_IR_SHIFT_BLACK);
#endif
	
	return u1ShiftBlack;
}

static UINT8 u1DrvTTDGetShiftBlackByFW(void)
{
	UINT8 u1RangeRatio, u1ConvDepth;
	INT32 i4LDMax, i4LDMin, i4ShiftBlack;

	u1RangeRatio = u1DrvTTDGetDepthRangeRatio();
	u1ConvDepth = u1DrvTTDGetConvDepth();

	i4LDMax = i4DrvDepthToDisparity(255, 127, u1RangeRatio, u1ConvDepth);
	i4LDMin = i4DrvDepthToDisparity(0  , 127, u1RangeRatio, u1ConvDepth);
	
	i4ShiftBlack = (i4LDMax > (-i4LDMin)) ? i4LDMax : (-i4LDMin);
	i4ShiftBlack = (i4ShiftBlack > 255) ? 255 : i4ShiftBlack;

	return (UINT8)i4ShiftBlack;
}

static void vDrvTTDSetDsprtBlackClpEn(UINT8 u1OnOff)
{
	u1OnOff = (u1OnOff > 0) ? 1 : 0;
#if defined(CC_MT5396)
	if ((BSP_GetIcVersion() >= IC_VER_5396_AB))
	{
		vIO32WriteFldAlign(TTDIR_04, u1OnOff, TTDIR_04_IR_DSPRT_BLACK_CLP_EN);
	}	
#endif
}

static void vDrvTTDSetDsprtMin(UINT8 u1Value)
{
#if defined(CC_MT5396)
	if ((BSP_GetIcVersion() >= IC_VER_5396_AB))
	{
		vIO32WriteFldAlign(TTDIR_04, u1Value, TTDIR_04_IR_DISPT_MIN);
	}	
#endif
}

static void vDrvTTDSetDsprtMax(UINT8 u1Value)
{
#if defined(CC_MT5396)
	if ((BSP_GetIcVersion() >= IC_VER_5396_AB))
	{
		vIO32WriteFldAlign(TTDIR_04, u1Value, TTDIR_04_IR_DISPT_MAX);
	}	
#endif
}

static void vDrvTTDSetTfuGlobalEn(UINT8 u1OnOff)
{
	u1OnOff = (u1OnOff > 0) ? 1 : 0;
#if defined(CC_MT5396)
	if ((BSP_GetIcVersion() >= IC_VER_5396_AB))
	{
		vIO32WriteFldAlign(TTDDG_08, u1OnOff, TTDDG_08_DG_TFU_GLOBAL_EN);
	}	
#endif
}

static void vDrvTTDSetTfuGlobalOffset(UINT8 u1Value)
{
#if defined(CC_MT5396)
	if ((BSP_GetIcVersion() >= IC_VER_5396_AB))
	{
		vIO32WriteFldAlign(TTDDG_10, u1Value, TTDDG_10_DG_TFU_GLOBAL_OFFSET);
	}	
#endif
}

static void vDrvTTDSetTfuEn(UINT8 u1OnOff)
{
	u1OnOff = (u1OnOff > 0) ? 1 : 0;
#if defined(CC_MT5396)
	if ((BSP_GetIcVersion() >= IC_VER_5396_AB))
	{
		vIO32WriteFldAlign(TTDIR_03, u1OnOff, TTDIR_03_IR_TFU_EN);
	}	
#endif
}

static void vDrvTTDSetTfuBase(UINT8 u1Value)
{
#if defined(CC_MT5396)
	if ((BSP_GetIcVersion() >= IC_VER_5396_AB))
	{
		vIO32WriteFldAlign(TTDDG_10, u1Value, TTDDG_10_DG_TFU_BASE);
	}	
#endif
}

static void vDrvTTDSetColorEnhance(UINT8 u1OnOff)
{
	u1OnOff = (u1OnOff > 0) ? 1 : 0;

	if (u1OnOff)
	{
		vDrvTTDSetTfuBase(0);
		vDrvTTDSetSkinAddup(200);

	#if defined(CC_MT5396)
		if ((BSP_GetIcVersion() >= IC_VER_5396_AB))
		{
			vDrvTTDSetTfuBase(128);
			vDrvTTDSetSkinAddup(0);
		}
	#endif
	#if defined(CC_MT5368)
		if ((BSP_GetIcVersion() >= IC_VER_5368_AB))
		{
			vDrvTTDSetTfuBase(128);
			vDrvTTDSetSkinAddup(0);
		}
	#endif	
	}
	else
	{
		vDrvTTDSetTfuBase(0);
		vDrvTTDSetSkinAddup(0);
	}
}

static void vDrvTTDSet444To422En(UINT8 u1OnOff)
{
	u1OnOff = (u1OnOff > 0) ? 1 : 0;
#if defined(CC_MT5368)
	vIO32WriteFldAlign(TTD_03, u1OnOff, TTD_03_444_422_444_EN);
	vIO32WriteFldAlign(TTD_03, u1OnOff, TTD_03_444_422_LPF_EN);
	vIO32WriteFldAlign(TTD_03, u1OnOff, TTD_03_444_422_LINE_EXT);
#endif
#if defined(CC_MT5389)
	vIO32WriteFldAlign(TTD_04, u1OnOff, TTD_04_444_422_444_EN);
	vIO32WriteFldAlign(TTD_04, u1OnOff, TTD_04_444_422_LPF_EN);
	vIO32WriteFldAlign(TTD_04, u1OnOff, TTD_04_444_422_LINE_EXT);
#endif
}

static void vDrvTTDSetTtdEn(UINT8 u1OnOff)
{
	u1OnOff = (u1OnOff > 0) ? 1 : 0;
    vIO32WriteFldAlign(TTD_02, u1OnOff, TTD_02_TTD_EN);
}

static UINT16 u2DrvTTDGetFnlSceneVpX0(void)
{
	UINT16 u2FnlSceneVpX0 = 0;
#if defined(CC_MT5396) || defined(CC_MT5389)
    u2FnlSceneVpX0	= IO32ReadFldAlign(STA_TTDVP_18, STA_TTDVP_18_STA_VP_FNL_SCENE_VP_X0);
#endif
#if defined(CC_MT5368)
    u2FnlSceneVpX0	= IO32ReadFldAlign(STA_TTD_00, STA_TTD_00_STA_TTD_VP_X);
#endif
	
	return u2FnlSceneVpX0;
}

static UINT16 u2DrvTTDGetFnlSceneVpY0(void)
{
	UINT16 u2FnlSceneVpY0 = 0;
#if defined(CC_MT5396) || defined(CC_MT5389)
    u2FnlSceneVpY0	= IO32ReadFldAlign(STA_TTDVP_17, STA_TTDVP_17_STA_VP_FNL_SCENE_VP_Y0);
#endif
#if defined(CC_MT5368)
    u2FnlSceneVpY0	= IO32ReadFldAlign(STA_TTD_00, STA_TTD_00_STA_TTD_VP_Y);
#endif
	
	return u2FnlSceneVpY0;
}

static UINT8 u1DrvTTDGetFnlSceneMode(void)
{
	UINT8 u1FnlSceneMode = 0;
#if defined(CC_MT5396) || defined(CC_MT5389)
    u1FnlSceneMode = IO32ReadFldAlign(STA_TTDVP_18, STA_TTDVP_18_STA_VP_FNL_SCENE_MODE0);
#endif
#if defined(CC_MT5368)
    u1FnlSceneMode = IO32ReadFldAlign(STA_TTD_00, STA_TTD_00_STA_TTD_SCENE_MODE);
#endif
	
	return u1FnlSceneMode;
}

static UINT8 u1DrvTTDGetHistFlatness(void)
{
	UINT8 u1HistFlatness = 0;
#if defined(CC_MT5396) || defined(CC_MT5389)
    u1HistFlatness = IO32ReadFldAlign(STA_TTDVP_20, STA_TTDVP_20_STA_TTD_HIST_FLATNESS);
#endif
#if defined(CC_MT5368)
    u1HistFlatness = IO32ReadFldAlign(STA_TTD_00, STA_TTD_00_STA_TTD_HIST_FLATNESS);
#endif
	
	return u1HistFlatness;
}

static UINT8 u1DrvTTDGetVertFlipEn(void)
{
	UINT8 u1VertFlipEn = 0;
    u1VertFlipEn = IO32ReadFldAlign(TTDVP_05, TTDVP_05_VP_VFLIP_EN);
	
	return u1VertFlipEn;
}


static void vDrvTTDSetHistDistTh(UINT16 u2Value)
{
	u2Value = (u2Value > 0x7FFF) ? 0x7FFF : u2Value;
#if defined(CC_MT5396) || defined(CC_MT5389)
    vIO32WriteFldAlign(TTDVP_09, u2Value, TTDVP_09_VP_HIST_DIST_TH);
#endif
}


static void vDrvTTDSetVpX(UINT16 u2Value)
{
	u2Value = (u2Value > 0x07FF) ? 0x07FF : u2Value;
#if defined(CC_MT5396) || defined(CC_MT5389)
    vIO32WriteFldAlign(TTDVP_10, u2Value, TTDVP_10_VP_VP_X);
#endif
#if defined(CC_MT5368)
    vIO32WriteFldAlign(TTD_00, u2Value, TTD_00_VP_X);
#endif
}


static void vDrvTTDSetVpY(UINT16 u2Value)
{
	u2Value = (u2Value > 0x07FF) ? 0x07FF : u2Value;
#if defined(CC_MT5396) || defined(CC_MT5389)
    vIO32WriteFldAlign(TTDVP_10, u2Value, TTDVP_10_VP_VP_Y);
#endif
#if defined(CC_MT5368)
    vIO32WriteFldAlign(TTD_00, u2Value, TTD_00_VP_Y);
#endif
}


static void vDrvTTDSetSceneMode(UINT8 u1Value)
{
	u1Value = (u1Value > 0x0F) ? 0x0F : u1Value;
#if defined(CC_MT5396) || defined(CC_MT5389)
    vIO32WriteFldAlign(TTDVP_10, u1Value, TTDVP_10_VP_SCENE_MODE);
#endif
#if defined(CC_MT5368)
    vIO32WriteFldAlign(TTD_00, u1Value, TTD_00_SCENE_MODE);
#endif
}

static UINT8 u1DrvTTDGetDepthRangeRatio(void)
{
	UINT8 u1DepthRangeRatio = 0;
    u1DepthRangeRatio = IO32ReadFldAlign(TTDIR_00, TTDIR_00_IR_DEPTH_RANGE_RATIO);
	
	return u1DepthRangeRatio;
}

static UINT8 u1DrvTTDGetConvDepth(void)
{
	UINT8 u1ConvDepth = 0;
#if defined(CC_MT5396) || defined(CC_MT5389)
	u1ConvDepth = IO32ReadFldAlign(TTD_00, TTD_00_CONVERGENCE_DEPTH);
#endif
#if defined(CC_MT5368)
	u1ConvDepth = IO32ReadFldAlign(TTDIR_00, TTDIR_00_IR_CONVERGENCE_DEPTH);
#endif
	
	return u1ConvDepth;
}

static UINT32 u4DrvTTDGetImageApl(void)
{
	UINT32 u4ImageApl = 0;
#if defined(CC_MT5396) || defined(CC_MT5389)
    u4ImageApl = IO32ReadFldAlign(STA_TTDVP_20 ,STA_TTDVP_20_STA_TTD_IMAGE_APL); 
#endif
#if defined(CC_MT5368)
    u4ImageApl = IO32ReadFldAlign(STA_TTD_01 ,STA_TTD_01_STA_TTD_IMAGE_AVG); 
#endif
	
	return u4ImageApl;
}

static UINT32 u4DrvTTDGetCidInvThr(void)
{
	UINT32 u4CidInvThr = 0;
	u4CidInvThr = IO32ReadFldAlign(TTDVP_00 ,TTDVP_00_VP_CID_INV_THR);
	
	return u4CidInvThr;
}

static void vDrvTTDSetTransitionCtrlEn(UINT8 u1OnOff)
{
	u1OnOff = (u1OnOff > 0) ? 1 : 0;
    vIO32WriteFldAlign(TTDVP_05, u1OnOff, TTDVP_05_VP_TRANSITION_CTRL_EN);
}

static void vDrvTTDSetTransitionStat(UINT8 u1Value)
{
#if defined(CC_MT5396) || defined(CC_MT5389)
	u1Value = (u1Value > 63) ? 63 : u1Value;
#endif
#if defined(CC_MT5368)
	u1Value = (u1Value > 31) ? 31 : u1Value;
#endif
    vIO32WriteFldAlign(TTDVP_05, u1Value, TTDVP_05_VP_TRANSITION_STAT);
}

void vDrvTTDSetPrInfo(UINT8 u1OnOff)
{
	u1OnOff = (u1OnOff > 0) ? 1 : 0;
#if defined(CC_MT5396) || defined(CC_MT5389)
    vIO32WriteFldAlign(TTDIR_00, u1OnOff, TTDIR_00_IR_PR_INFO);
#endif
#if defined(CC_MT5368)
	if ((BSP_GetIcVersion() >= IC_VER_5368_AB))
	{
    	vIO32WriteFldAlign(TTDIR_00, u1OnOff, TTDIR_00_IR_PR_INFO);
	}	
#endif
}

UINT8 u1DrvTTDGetPrInfo(void)
{
	UINT8 u1PrInfo = 0;
#if defined(CC_MT5396) || defined(CC_MT5389)
	u1PrInfo = IO32ReadFldAlign(TTDIR_00 ,TTDIR_00_IR_PR_INFO);
#endif
#if defined(CC_MT5368)
	if ((BSP_GetIcVersion() >= IC_VER_5368_AB))
	{
		u1PrInfo = IO32ReadFldAlign(TTDIR_00 ,TTDIR_00_IR_PR_INFO);
	}	
#endif
	
	return u1PrInfo;
}

void vDrvTTDSetFWOnOff(UINT8 bOnOff)
{
    gTTDInfo.ttdFwStatus = bOnOff;
    LOG(0,"TTD FW On/Off = %d\n", bOnOff);

    if ((bOnOff == SV_OFF) && (!g_u1Input3D))
    {
    	vDrvTTDSetSceneCtrlEn(0);
    }
}

void vDrvTTDSetQualityISR(void)
{
    if(gTTDInfo.ttdFwStatus == SV_OFF)
    {
        vDrvTTDSetInverseControl(SV_OFF, gTTDInfo.ttdFgLocalInverse);
    }
    else
    {
        vDrvTTDCheckTransitionByAPL();
    }
}


void vDrvTTDProc(void)
{   
	#if defined(CC_MT5396) || defined(CC_MT5389)
    vDrvTTDLSVProc();
    #endif    
}


void vDrvTTDSetStereoComfortRange(UINT8 u1Factor)
{
	UINT32 u4PixelSize, u4MaxDisparity;
	u4PixelSize = 512;//PANEL_Get3DPixelSize();
	u4MaxDisparity = u1Factor * 1000 / u4PixelSize;

	vDrvTTDSetDsprtMax((u4MaxDisparity >> 1));
	vDrvTTDSetDsprtMin((u4MaxDisparity >> 1));
}
 
void vDrvTTDSetLvdsTxGlobalShift(INT8 i1Shift)
{
	UINT8 u1Shift, u1Sign;

	if (i1Shift < 0)
	{
		u1Sign = 1;
		u1Shift = (UINT8)(-i1Shift);
	}
	else
	{
		u1Sign = 0;
		u1Shift = (UINT8)i1Shift;
	}

	vDrvTTDSetPrInfo(u1Sign);

	if (IS_LVDS_DISP_3D_POLARIZED)
	{
		Panel_SetDisplayShift(1, eDisplayShift_Line, u1Shift, u1Shift);
	}
	else if (IS_LVDS_DISP_3D_SHUTTER)
    {
        Panel_SetDisplayShift(1, eDisplayShift_Frame, u1Shift, u1Shift);
    }
}

 /*-----------------------------------------------------------------------
1. Set c_vp_scene_control_en = 1 (enable manual scene control)
2. Set c_dg_global_weight to 32 & c_dg_local_weight to 0.
3. Set c_dg_depth_bias = 224 & c_ttd_convergence_depth = 192
4. Set c_ir_depth_range_ratio to 0
5. Set c_ir_disparity_gain from 0 to 127, should shift 1 pixel at a time
6. Set c_ir_pr_info to 1 -> in oppsite direction compare to step 5
-----------------------------------------------------------------------*/
UINT8 u1DrvTTDSetV3GlobalShift(INT8 i1Shift)
{
#if defined(CC_MT5389)
	UINT8 u1Shift, u1Sign;

	if (i1Shift < 0)
	{
		u1Sign = 1;
		u1Shift = (UINT8)(-i1Shift);
	}
	else
	{
		u1Sign = 0;
		u1Shift = (UINT8)i1Shift;
	}
	
	//vDrvTTDSetPrInfo(u1Sign);
	vDrvTTDSetDisparityGain(u1Shift);
	vDrvTTDSetBlackNum(u1Shift);

	return u1Sign;
#else
	return 0;
#endif
}

 void vDrvTTDSetNewGlobalShift(UINT8 u1Shift)
{
#if defined(CC_MT5396)
	if ((BSP_GetIcVersion() >= IC_VER_5396_AB))
	{
    	vIO32WriteFldAlign(TTDIR_04, u1Shift, TTDIR_04_IR_GLOBAL_SHIFT);
	}
#endif
}

// Image Safety On/Off
void vDrvTTDSetImageSafetyLevel(UINT8 u1Level)
{
    g_u1ImageSafetyLevel = u1Level;
    g_u1ParameterChange = 1;
}


 // Local Depth
void vDrvTTDSetDepthOfField(UINT8 u1Depth)
{
    g_u1DepthOfField = u1Depth;
    g_u1ParameterChange = 1;
}

// Global Depth
void vDrvTTDSetProtrudeFromScreen(UINT8 u1Protrude)
{
    g_u1ProtrudeFromScreen = u1Protrude;
    g_u1ParameterChange = 1;
}

void vDrvTTDSetDistanceToTV(UINT8 u1Distance)
{
    g_u1DistanceToTV = u1Distance;
    g_u1ParameterChange = 1;
}

void vDrvTTDSetLREyeInverse(UINT8 u1OnOff)
{
    g_u1LREyeInverse = u1OnOff;
    g_u1ParameterChange = 1;
}

// Gain = (Depth/32)*(MaxGain*Scale/128 - MinGain) + MinGain 
//      = Depth*(MaxGain*Scale - MinGain*128)/4096 + MinGain
INT32 i4DrvTTDDepthToGain(INT32 i4MaxGain, INT32 i4MinGain, INT32 i4Depth, INT32 i4Scale)
{
	INT32 i4Gain, i4Temp;
	//i4Gain = (i4Depth*i4MaxGain*i4Scale + (1 << 11)) >> 12;

	i4Temp = i4MaxGain*i4Scale - (i4MinGain << 7);


	if (i4Temp > 0)
	{
		i4Gain = ((i4Depth*i4Temp + (1 << 11)) >> 12) + i4MinGain;
	}
	else
	{
		i4Gain = i4MinGain;
	}

	return i4Gain;
}

INT32 i4DrvTTDProtrudeToLRShift(INT32 i4Ptru, INT32 i4Scale)
{
	INT32 i4LRShift;
	i4LRShift = 128 + (((16 - i4Ptru)*2*i4Scale + 64) >> 7);

	return i4LRShift;
}


INT32 i4DrvTTDProtrudeToConvDepth(INT32 i4Ptru, INT32 i4Scale)
{
	INT32 i4ConvDepth;
	i4ConvDepth = 128 + (((16 - i4Ptru)*7*i4Scale + 64) >> 7);

	return i4ConvDepth;
}

INT32 i4DrvTTDCalculateSafetyScale(void)
{
	INT32 i4Safe, i4Depth, i4Ptru, i4Dist;
	INT32 i4PixSize, i4DConvex, i4DConcave, i4DParallax, i4Factor;
	INT32 i4DLimMax, i4DLimMin, i4LDMax, i4LDMin, i4GDMax, i4GDMin, i4TDMax, i4TDMin;
	INT32 i4LRShiftMax, i4LRShiftMin, i4ConvDepthMax, i4ConvDepthMin;
	INT32 i4ScalePos, i4ScaleNeg, i4Scale;

	i4Safe = 0, i4Depth = 16, i4Ptru = 16, i4Dist = 10;
	i4PixSize = 320, i4DConvex = 256, i4DConcave = 256, i4DParallax = 256, i4Factor = 50;
	i4DLimMax = 0, i4DLimMin = 0, i4LDMax = 0, i4LDMin = 0, i4GDMax = 0, i4GDMin = 0, i4TDMax= 0, i4TDMin = 0;
	i4LRShiftMax = 0, i4LRShiftMin = 0, i4ConvDepthMax = 0, i4ConvDepthMin = 0;
	i4ScalePos = 128, i4ScaleNeg = 128, i4Scale = 128;

	if (g_u1ImageSafetyLevel)
	{
		i4Safe = (INT32)g_u1ImageSafetyLevel;
		i4Depth = (INT32)g_u1DepthOfField;
		i4Ptru = (INT32)g_u1ProtrudeFromScreen;
		i4Dist = (INT32)g_u1DistanceToTV;
		i4LRShiftMax = i4DrvTTDProtrudeToLRShift(0 ,128);
		i4LRShiftMin = i4DrvTTDProtrudeToLRShift(32,128);
		i4ConvDepthMax = i4DrvTTDProtrudeToConvDepth(0 , 128);
		i4ConvDepthMin = i4DrvTTDProtrudeToConvDepth(32, 128);

		// (1) Find i4DLimMax, i4DLimMin from Parallax and Convex/Concave 1 degree constraint
		i4PixSize = (INT32)(PANEL_Get3DPixelSize());
		i4Factor = (INT32)STEREO_COMFORT_FACTOR;
		i4DParallax = (i4Factor*1000/i4PixSize) >> 1;
		i4DConvex   = (i4Dist  *5236/i4PixSize) >> 1; // (PI/180)*300*1000 = 5236
		i4DConcave  = (i4Dist  *5236/i4PixSize) >> 1; // (PI/180)*300*1000 = 5236
		
		i4DLimMax = i4DConvex;
		i4DLimMin = (i4DConcave > i4DParallax) ? (-i4DParallax) : (-i4DConcave);
		
		i4DLimMax = (i4DLimMax*(5-i4Safe) + 2) >> 2;
		i4DLimMin = (i4DLimMin*(5-i4Safe) + 2) >> 2;
		
		// (2) Find i4TDMax, i4TDMin
		
		if (g_u1Input3D)
		{
			if (g_u1DepthCtrlReady)
			{
			//#if defined(CC_MT5396)
				i4LDMax = i4DrvDepthToDisparity(255, DISPARITY_GAIN_3D, DEPTH_RANGE_RATIO_3D, CONVERGENCE_DEPTH_3D);
				i4LDMin = i4DrvDepthToDisparity(  0, DISPARITY_GAIN_3D, DEPTH_RANGE_RATIO_3D, CONVERGENCE_DEPTH_3D);
				i4GDMax = (cur_convg > 0) ?  cur_convg : 0;
				i4GDMin = (cur_convg > 0) ?  0 : cur_convg;
				
				i4TDMax = i4LDMax + i4GDMax;
				i4TDMin = i4LDMin + i4GDMin;
			//#endif
			}
			else
			{
				i4TDMax = i4LRShiftMax - 128;
				i4TDMin = i4LRShiftMin - 128; 				
			}
		}
		else
		{
			if ((VER_5368_MP == g_u1Ver) || (VER_5396_MP == g_u1Ver))
			{
				i4LDMax = i4DrvDepthToDisparity(255, 127, (UINT8)DEPTH_RANGE_RATIO_TTD, (UINT8)CONVERGENCE_DEPTH_TTD);
				i4LDMin = i4DrvDepthToDisparity(  0, 127, (UINT8)DEPTH_RANGE_RATIO_TTD, (UINT8)CONVERGENCE_DEPTH_TTD);
				i4GDMax = i4LRShiftMax - 128;
				i4GDMin = i4LRShiftMin - 128; 
				
				i4TDMax = i4LDMax + i4GDMax;
				i4TDMin = i4LDMin + i4GDMin;
			}
			else
			{
				i4TDMax = i4DrvDepthToDisparity(255, 127, (UINT8)DEPTH_RANGE_RATIO_TTD, (UINT8)i4ConvDepthMin);
				i4TDMin = i4DrvDepthToDisparity(  0, 127, (UINT8)DEPTH_RANGE_RATIO_TTD, (UINT8)i4ConvDepthMax);
			}
		}
		

		// (3) Return i4Scale
		i4ScalePos = (i4DLimMax > i4TDMax) ? 128 : (i4DLimMax*128/i4TDMax);
		i4ScaleNeg = (i4DLimMin < i4TDMin) ? 128 : (i4DLimMin*128/i4TDMin);
		
		i4Scale = (i4ScalePos > i4ScaleNeg) ? i4ScaleNeg : i4ScalePos;
	}

	LOG(6, "==== i4DrvTTDCalculateSafetyScale ====%d\n");
	LOG(6, "i4Safe = %d, i4Depth = %d, i4Ptru = %d, i4Dist = %d\n", i4Safe, i4Depth, i4Ptru, i4Dist);
	LOG(6, "i4PixSize = %d, i4DConvex = %d, i4DConcave = %d, i4DParallax = %d, i4Factor = %d\n", i4PixSize, i4DConvex, i4DConcave, i4DParallax, i4Factor);
	LOG(6, "i4DLimMax = %d, i4DLimMin = %d, i4TDMax = %d, i4TDMin = %d\n", i4DLimMax, i4DLimMin, i4TDMax, i4TDMin);
	LOG(6, "i4LDMax = %d, i4LDMin = %d, i4GDMax = %d, i4GDMin = %d\n", i4LDMax, i4LDMin, i4GDMax, i4GDMin);
	LOG(6, "i4LRShiftMax = %d, i4LRShiftMin = %d, i4ConvDepthMax = %d, i4ConvDepthMin = %d\n", i4LRShiftMax, i4LRShiftMin, i4ConvDepthMax, i4ConvDepthMin);
	LOG(6, "i4ScalePos = %d, i4ScaleNeg = %d, i4Scale = %d\n", i4ScalePos, i4ScaleNeg, i4Scale);
	
	return i4Scale;
}

void vDrvTTDUiValueUpdate(INT32 i4Scale)
{
	INT32 i4Depth, i4Ptru;
	INT32 i4DisparityGain, i4ConvergenceDepth, i4GlobalShift;
	INT32 i4ConvgGain, i4EhnGain, i4ConvgPlen;
	INT32 i4ShiftBlack;
	UINT8 u1PrInfo;
	
	i4Depth = (INT32)g_u1DepthOfField;
	i4Ptru = (INT32)g_u1ProtrudeFromScreen;
	i4ConvgGain = 0, i4EhnGain = 0, i4ConvgPlen = 128;
	i4DisparityGain    = (INT32)DISPARITY_GAIN_TTD;
	i4ConvergenceDepth = (INT32)CONVERGENCE_DEPTH_TTD;
	i4GlobalShift = 128;
	i4ShiftBlack = 0;
	u1PrInfo = 0;

	if (g_u1Input3D)
	{	
		if (g_u1DepthCtrlReady)
		{
		//#if defined(CC_MT5396)
			i4DisparityGain    = (INT32)DISPARITY_GAIN_3D;
			i4ConvergenceDepth = (INT32)CONVERGENCE_DEPTH_3D;
			i4ConvgGain = i4DrvTTDDepthToGain(63, 0, i4Depth, i4Scale);
			i4EhnGain   = 0;//i4DrvTTDDepthToGain(31, 0, i4Depth, i4Scale);
			i4ConvgPlen = i4DrvTTDProtrudeToLRShift(i4Ptru ,i4Scale);
		//#endif
		}
		else
		{
			u1PrInfo = u1DrvTTDSetV3GlobalShift((16 - i4Ptru)*2);
			i4ConvergenceDepth = 192;
		}
	}
	else
	{
		i4DisparityGain = i4DrvTTDDepthToGain(127, 0, i4Depth, i4Scale);

		if ((VER_5368_MP == g_u1Ver) || (VER_5396_MP == g_u1Ver))
		{
			i4GlobalShift = i4DrvTTDProtrudeToLRShift(i4Ptru ,i4Scale);
		}
		else
		{
			i4ConvergenceDepth = i4DrvTTDProtrudeToConvDepth(i4Ptru ,128);
		}
	}

	if ((g_u1DepthCtrlReady) || (!g_u1Input3D))
	{
		vDrvTTDSetDisparityGain((UINT8)i4DisparityGain);
	}
	vDrvTTDSetConvergenceDepthUI((UINT8)i4ConvergenceDepth);
	vDrvTTDSetNewGlobalShift((UINT8)i4GlobalShift);

	vIO32WriteFldAlign(TTD_FW_REG_08, (UINT8)i4ConvgPlen, LSV_CONVG_PLEN);
#if defined(CC_MT5396) && defined(TTD_TODO)
    MJC_WRITE_FLD(MJC_TTD_REG_00, (UINT8)i4ConvgGain, TTD_LSV_CONVG_GAIN);
    MJC_WRITE_FLD(MJC_TTD_REG_00, (UINT8)i4EhnGain, TTD_LSV_ENH_GAIN);
#else
	vIO32WriteFldAlign(SW_TTD_REG_00, (UINT8)i4ConvgGain, SW_LSV_CONVG_GAIN);
	vIO32WriteFldAlign(SW_TTD_REG_00, (UINT8)i4EhnGain, SW_LSV_ENH_GAIN);
#endif

	// Set BlackNum
	if ((VER_5368_MP == g_u1Ver) || (VER_5396_MP == g_u1Ver) || (VER_5389 == g_u1Ver))
	{
		i4ShiftBlack = (INT32)(u1DrvTTDGetShiftBlack());
	}
	else
	{
		i4ShiftBlack = (INT32)(u1DrvTTDGetShiftBlackByFW());
	}

	if ((g_u1DepthCtrlReady) || (!g_u1Input3D))
	{
		vDrvTTDSetBlackNum((UINT8)i4ShiftBlack);
	}

	//  Set BlackSingleNum
	if (i4GlobalShift < 128)
	{
		vDrvTTDSetBlackSingleInv(0);
		vDrvTTDSetBlackSingleNum((UINT8)(128 - i4GlobalShift + i4ShiftBlack));
	}
	else
	{
		vDrvTTDSetBlackSingleInv(1);
		vDrvTTDSetBlackSingleNum((UINT8)(i4GlobalShift - 128 + i4ShiftBlack));
	}

	vDrvTTDSetPrInfo(u1PrInfo^g_u1LREyeInverse);
	


	LOG(6, "==== vDrvTTDUiValueUpdate ====%d\n");
	LOG(6, "i4Depth = %d, i4Ptru = %d, i4Scale = %d\n", i4Depth, i4Ptru, i4Scale);
	LOG(6, "i4DisparityGain = %d, i4ConvergenceDepth = %d, i4GlobalShift = %d\n", i4DisparityGain, i4ConvergenceDepth, i4GlobalShift);
	LOG(6, "i4ConvgGain = %d, i4EhnGain = %d, i4ConvgPlen = %d\n", i4ConvgGain, i4EhnGain, i4ConvgPlen);
	LOG(6, "i4ShiftBlack = %d\n", i4ShiftBlack);
}


void vDrvTTDSetUiProc(void)
{	
	INT32 i4Scale;
	
	if (g_u1ParameterChange)
	{
		i4Scale = i4DrvTTDCalculateSafetyScale();
		vDrvTTDUiValueUpdate(i4Scale);
    	Printf("g_u1ImageSafetyLevel = %d\n", g_u1ImageSafetyLevel);
		Printf("g_u1DepthOfField       = %d, \n", g_u1DepthOfField);
		Printf("g_u1ProtrudeFromScreen = %d, \n", g_u1ProtrudeFromScreen);
		Printf("g_u1DistanceToTV       = %d, \n", g_u1DistanceToTV);
		Printf("vDrvTTDUiValueUpdate\n");

		g_u1ParameterChange = 0;
	}
}

/***************************************************************
Original Depth = x, New Depth = y
x =  0~8,  y = 2x
x =  8~16, y = (x-8)+16 = x+8
x = 16~32, y = (x-16)/2+24 = (x+32)/2

Original Distance = x, New Distance = y
x =  2~6,  y = 2(x-2)+2 = 2x-2
x =  6~10, y = (x-6)+10 = x+4
x = 10~18, y = (x-10)/2+14 = (x+18)/2

***************************************************************/
void vDrvTTDCalculateDisparityGain(UINT8 u1Depth, UINT8 u1Distance)
{
	UINT32 u4DisparityGain, u4SafetyGain;

	u4SafetyGain = (UINT32)u1DrvTTDGetSafetyDisparityGain(0, g_u1DistanceToTV, g_u1ProtrudeFromScreen);

	if (g_u1ImageSafetyLevel)
	{
		u4DisparityGain = (((UINT32)u1Depth)*((UINT32)u1Distance))*u4SafetyGain/32/18;
	}
	else
	{
		u4DisparityGain = ((UINT32)u1Depth)*127/32;
	}
	
	u4DisparityGain = (u4DisparityGain > 127) ? 127 : u4DisparityGain;
	vDrvTTDSetDisparityGain(u4DisparityGain);
}
 
void vDrvTTDSetStrength(UINT8 strength)
{
    LOG(6,"Set TTD Strength to [%d]\n",strength);
    vDrvTTDSetDisparityGain(strength==0?0:strength*4-1);
}

// -- Image Safety Implementation Start --//
UINT8 u1DrvTTDGetSafetyDisparityGain(UINT16 u2PixSize, UINT8 u1UiDist, UINT8 u1UiGlobalDepth)
{
	UINT8 u1DsprtGainMax, u1RangeRatio, u1ConvDepth, u1DisparityGainUpperBound;
	UINT32 u4SafetyDisparity;
	INT32 i4LDMax, i4LDMin, i4GDMax, i4GDMin;

	u4SafetyDisparity = u4DrvTTDGetSafetyDisparity(u2PixSize, STEREO_COMFORT_FACTOR);

	u1RangeRatio = u1DrvTTDGetDepthRangeRatio();
	u1ConvDepth = u1DrvTTDGetConvDepth();

	u1DsprtGainMax = (UINT8)(((UINT16)u1UiDist)*127/18);
	i4LDMax = i4DrvDepthToDisparity(255, u1DsprtGainMax, u1RangeRatio, u1ConvDepth);
	i4LDMin = i4DrvDepthToDisparity(0  , u1DsprtGainMax, u1RangeRatio, u1ConvDepth);

	i4GDMax = i4DrvTTDGetGlobalMaxDisparity(u1UiGlobalDepth);
	i4GDMin = i4DrvTTDGetGlobalMinDisparity(u1UiGlobalDepth);

	u1DisparityGainUpperBound = u1DrvTTDGetDisparityGainUpperBound(u4SafetyDisparity, i4LDMax, i4LDMin, i4GDMax, i4GDMin);

	LOG(0,"[Image Safety] u1DisparityGainUpperBound = %d\n",u1DisparityGainUpperBound);

	return u1DisparityGainUpperBound;
}

UINT8 u1DrvTTDGetSafetyLocalDepth(UINT16 u2PixSize, UINT8 u1UiDist, UINT8 u1UiGlobalDepth)
{
	UINT8 u1DsprtGainMax, u1RangeRatio, u1ConvDepth, u1LocalDepthUpperBound;
	UINT32 u4SafetyDisparity;
	INT32 i4LDMax, i4LDMin, i4GDMax, i4GDMin;

	u4SafetyDisparity = u4DrvTTDGetSafetyDisparity(u2PixSize, STEREO_COMFORT_FACTOR);

	u1RangeRatio = u1DrvTTDGetDepthRangeRatio();
	u1ConvDepth = u1DrvTTDGetConvDepth();

	u1DsprtGainMax = (UINT8)(((UINT16)u1UiDist)*127/18);
	i4LDMax = i4DrvDepthToDisparity(255, u1DsprtGainMax, u1RangeRatio, u1ConvDepth);
	i4LDMin = i4DrvDepthToDisparity(0  , u1DsprtGainMax, u1RangeRatio, u1ConvDepth);

	i4GDMax = i4DrvTTDGetGlobalMaxDisparity(u1UiGlobalDepth);
	i4GDMin = i4DrvTTDGetGlobalMinDisparity(u1UiGlobalDepth);

	u1LocalDepthUpperBound = u1DrvTTDGetLocalDepthUpperBound(u4SafetyDisparity, i4LDMax, i4LDMin, i4GDMax, i4GDMin);

	return u1LocalDepthUpperBound;
}

UINT32 u4DrvTTDGetSafetyDisparity(UINT16 u2PixSize, UINT8 u1Factor)
{
	UINT32 u4SafetyDisparity;

	if (u2PixSize == 0)
	{
		u2PixSize = PANEL_Get3DPixelSize();
	}

	// L-Eye shift 1 pixel, R-Eye shift 1 pixel, total = 2 pixel
	u4SafetyDisparity = ((UINT32)u1Factor) * 1000 / ((UINT32)u2PixSize) / 2;

	return u4SafetyDisparity;
}

INT32 i4DrvDepthToDisparity(UINT8 u1Depth, UINT8 u1DsprtGain, UINT8 u1RangeRatio, UINT8 u1ConvDepth)
{
	INT32 i4Tmp1, i4Tmp2, i4Tmp3, i4Tmp4, i4Dsprt;

	i4Tmp1 = (1 << 6) - ((INT32)u1RangeRatio);
	i4Tmp2 = (1 << 14) - ((INT32)u1ConvDepth)*i4Tmp1;
	i4Tmp3 = ((INT32)u1DsprtGain) << 10;
	i4Tmp4 = (1 << 14) - ((INT32)u1Depth)*i4Tmp1;

	i4Dsprt = (((INT32)u1Depth) - ((INT32)u1ConvDepth))*i4Tmp1*i4Tmp3/i4Tmp2*4/i4Tmp4;
	
	return i4Dsprt;
}

INT32 i4DrvTTDGetGlobalMaxDisparity(UINT8 u1UiGlobalDepth)
{
	INT32 i4GDMax = (16 - ((INT32)u1UiGlobalDepth))*2;
	return i4GDMax;
}

INT32 i4DrvTTDGetGlobalMinDisparity(UINT8 u1UiGlobalDepth)
{
	INT32 i4GDMin = (16 - ((INT32)u1UiGlobalDepth))*2;
	return i4GDMin;
}

UINT8 u1DrvTTDGetDisparityGainUpperBound(UINT32 u4SafetyDisparity, INT32 i4LDMax, INT32 i4LDMin, INT32 i4GDMax, INT32 i4GDMin)
{
	INT32 i4DGU, i4SFD;

	i4SFD = (INT32)u4SafetyDisparity;

	if (((i4SFD - i4GDMax) < 0) || ((i4SFD + i4GDMin) < 0))
	{
		i4DGU = 0;
	}
	else
	{
		if (i4LDMax == 0)
		{
			i4DGU = (- i4SFD - i4GDMin)*127/i4LDMin;
		}
		else if (i4LDMin == 0)
		{
			i4DGU = (i4SFD - i4GDMax)*127/i4LDMax;
		}
		else
		{
			i4DGU = MIN((i4SFD - i4GDMax)*127/i4LDMax, (- i4SFD - i4GDMin)*127/i4LDMin);
		}

		i4DGU = (i4DGU > 127) ? 127 : i4DGU;
	}

	return (UINT8)i4DGU;
}

UINT8 u1DrvTTDGetLocalDepthUpperBound(UINT32 u4SafetyDisparity, INT32 i4LDMax, INT32 i4LDMin, INT32 i4GDMax, INT32 i4GDMin)
{
	INT32 i4LDU, i4SFD;

	i4SFD = (INT32)u4SafetyDisparity;

	if (((i4SFD - i4GDMax) < 0) || ((i4SFD + i4GDMin) < 0))
	{
		i4LDU = 0;
	}
	else
	{
		if (i4LDMax == 0)
		{
			i4LDU = (- i4SFD - i4GDMin)*32/i4LDMin;
		}
		else if (i4LDMin == 0)
		{
			i4LDU = (i4SFD - i4GDMax)*32/i4LDMax;
		}
		else
		{
			i4LDU = MIN((i4SFD - i4GDMax)*32/i4LDMax, (- i4SFD - i4GDMin)*32/i4LDMin);
		}

		i4LDU = (i4LDU > 32) ? 32 : i4LDU;
	}

	return (UINT8)i4LDU;
}
// -- Image Safety Implementation End --//


void vDrvTTDSetLRSyncSwitch(UINT8 bEnable)
{
	vIO32WriteFldAlign(TTD_01, bEnable, TTD_01_OUT_LR_INV);
}

void vDrvTTDSetLRDataSwitch(UINT8 bEnable)
{
#if defined(CC_MT5396) || defined(CC_MT5389)
	vIO32WriteFldAlign(TTD_01, bEnable, TTD_01_DIBR_LR_INV);
#endif
}
static UINT8 bDrvDisparityToDepth(INT8 i1Dsprt)
{
	INT32 i4Tmp1, i4Tmp2, i4Tmp3, i4Tmp4;
	INT32 i4ConvDepth, i4DsprtGain, i4RangeRatio, i4Depth;

	// Input restriction
	i1Dsprt = (i1Dsprt > 32) ? 32 : ((i1Dsprt < (-32)) ? (-32) : i1Dsprt);

	i4ConvDepth = 214;
	i4DsprtGain = 60;
	i4RangeRatio = 12;
	
	i4Tmp1 = (1 << 6) - i4RangeRatio;
	i4Tmp2 = (1 << 14) - i4ConvDepth*i4Tmp1;
	i4Tmp3 = i4DsprtGain << 12;
	i4Tmp4 = ((((INT32)i1Dsprt) << 2) - i4DsprtGain)*i4Tmp2 + (i4Tmp3 << 2);
	i4Depth = (((i4Tmp4 << 6)/i4Tmp1) << 6)/(((INT32)i1Dsprt)*i4Tmp2 + i4Tmp3);

	// Output restriction
	i4Depth = (i4Depth > 255) ? 255 : ((i4Depth < 0) ? 0 : i4Depth);
	
	return ((UINT8)i4Depth);
}

static void vDrvTTDCheckTransitionByAPL(void)
{
    static UINT8 bDbgCnt = 0;    
    
    LOG(7,"[%d] Flatness(%d)\n",g_u1SceneChangedCounter, u1DrvTTDGetHistFlatness());

    if ((u1DrvTTDIsSceneChanged() == SV_TRUE)        
        || (IO32ReadFldAlign(TTD_FW_REG_00, TTD_FW_FORCE_SC) && (bDbgCnt >= 150)))
    {
        vDrvTTDCheckGlobalModel();
        vDrvTTDCheckLocalInverseMode();
        bDbgCnt = 0;
    }
    else
    {
        bDbgCnt++;
    }

    vDrvTTDSetInverseControl(SV_ON, gTTDInfo.ttdFgLocalInverse);
}

static UINT8 u1DrvTTDGetCurrentSceneMode(void)
{
    return gTTDInfo.ttdSceneMode;
}

/*  Scene change condition:
 *  1. APL change exceed threshold
 *  2. (Only for Flat Mode)Flatness change exceed Threshold
 */
static UINT8 u1DrvTTDIsSceneChanged(void) 
{
    UINT8 ret = SV_FALSE;
    UINT8 u1CurrentFlatness = 0;
    UINT32 u4ImageAvgDiff = 0;
    //after APL diff > thr, wait 1 vsync and return SV_TRUE, we do this to make sure flatness is stable.
    if(g_u1SceneChangedFlag == SV_TRUE)
    {
        if(g_u1SceneChangedCounter>=g_u1SceneChangedCounterThr)
        {
            g_u1SceneChangedFlag = SV_FALSE;
            g_u1SceneChangedCounter = 0;
            return SV_TRUE;
        }
        else
        {
            g_u1SceneChangedCounter++;
        }
    }
   
    // 1. get current pixel sum
    gTTDInfo.ttdCurrentImageAvg = u4DrvTTDGetImageApl();
    
    // 2. calculate diff with previous one.
    u4ImageAvgDiff = DIFF(gTTDInfo.ttdCurrentImageAvg, gTTDInfo.ttdPreviousImageAvg);
    
    // 3. Scene change Condition 1: APL change exceed threshold 
    if(u4ImageAvgDiff  > gTTDInfo.ttdSceneChangeThr)
    {
        g_u1SceneChangedFlag = SV_TRUE;
        g_u1SceneChangedCounter = 0;
        LOG(7,"Scene Changed:: Image AVG from(%d) to(%d), Diff(%d)\n", 
            gTTDInfo.ttdPreviousImageAvg, gTTDInfo.ttdCurrentImageAvg, u4ImageAvgDiff);
    }
    // 4. set current APL to previous
    gTTDInfo.ttdPreviousImageAvg = gTTDInfo.ttdCurrentImageAvg;

    // 5. Scene change Condition 2: Flatness change exceed threshold
    u1CurrentFlatness = u1DrvTTDGetHistFlatness();

    if(u1DrvTTDGetCurrentSceneMode() == 5 && u1CurrentFlatness > 4)
    {
        LOG(7,"Scene change from Flat mode because of Flatness Change. Current Flatness(%d)\n",u1CurrentFlatness);
        ret = SV_TRUE;
    }

    return ret;
}

static void vDrvTTDCheckGlobalModel(void)
{
    UINT16	u2NewTtdVpX = 0;
    UINT16	u2NewTtdVpY = 0;
    UINT16	u2NewTtdSceneMode = 0;
    UINT8	u1HistFlatness = 0;
    BOOL	u1VertFlipEn = 0;

    // 1. Get VP
    u2NewTtdVpX	= u2DrvTTDGetFnlSceneVpX0();
    u2NewTtdVpY	= u2DrvTTDGetFnlSceneVpY0();
    // 2. Get SceneMode
    u2NewTtdSceneMode = u1DrvTTDGetFnlSceneMode();
    // 3. Get Flatness
    u1HistFlatness = u1DrvTTDGetHistFlatness();
    // 4. Get Vertical Flip setting
    u1VertFlipEn = u1DrvTTDGetVertFlipEn();

    LOG(7, "FLATNESS(%d)\n",u1HistFlatness);
    if(u1HistFlatness <=3)
    {
        u2NewTtdSceneMode = 5;
        vDrvTTDSetHistDistTh(0x7FFF);
    }
    else
	{
        vDrvTTDSetHistDistTh(g_u4VpHistDistTh);
	}

    if(gTTDInfo.ttdSceneMode != u2NewTtdSceneMode)
    {
        LOG(7,"TTD state change from(%d) to(%d)\n", gTTDInfo.ttdSceneMode, u2NewTtdSceneMode);
        gTTDInfo.ttdSceneMode = u2NewTtdSceneMode;
    }
    
    if(gTTDInfo.ttdSceneMode == 5)
    {
        gTTDInfo.ttdVpX = 0;
        gTTDInfo.ttdVpY = gTTDInfo.ttdInputHeight;
        gTTDInfo.ttdDgBias = 0; //32;
        gTTDInfo.ttdGlobalWeight = 0;
        gTTDInfo.ttdLocalWeight = 0;
    }
    else
    {
        gTTDInfo.ttdVpX = u2NewTtdVpX;
        gTTDInfo.ttdVpY = u2NewTtdVpY;
        gTTDInfo.ttdDgBias = 0; //32;
        gTTDInfo.ttdGlobalWeight = 8;
        gTTDInfo.ttdLocalWeight = 24;
   }

    // Write back to control registers
    vDrvTTDSetSceneCtrlEn(1);
    vDrvTTDSetVpX(gTTDInfo.ttdVpX);
    vDrvTTDSetVpY(gTTDInfo.ttdVpY);

    if(gTTDInfo.ttdSceneMode == 5)
    {
        if(u1VertFlipEn == 1)		// SCENE_INV_HORI
        {
        	vDrvTTDSetSceneMode(4);
        }
        else
        {							// SCENE_HORI
        	vDrvTTDSetSceneMode(1);
        }
    }
    else
    {
    	vDrvTTDSetSceneMode(gTTDInfo.ttdSceneMode);
    }
    vDrvTTDSetDepthBias(gTTDInfo.ttdDgBias);
    
    //set convergence depth register
    if(gTTDInfo.ttdSceneMode == 5)
    {
        vDrvTTDSetConvergenceDepth(0);
    }
    else
    {
        vDrvTTDSetConvergenceDepth(gTTDInfo.ttdConvergenceDepth);
    }
    
    vDrvTTDSetGlobalWeight(gTTDInfo.ttdGlobalWeight);
    vDrvTTDSetLocalWeight(gTTDInfo.ttdLocalWeight);

    // Turn off hardware Histogram flatness mechanism
    vDrvTTDSetHistFlatnessCtrlEn(0);
    vDrvTTDSetHistFlatness(10);
}


static void vDrvTTDGetHist(UINT16 Hist[32])
{
    UINT32 i;

    if (Hist == NULL)
    {
        return;
    }
    
    for (i = 0; i < 16; i++)
    {
        Hist[i*2+0] = IO32ReadFldAlign(STA_TTDVP_00+i*4, STA_TTDVP_00_STA_VP_HISTOGRAM_BIN_0);
        Hist[i*2+1] = IO32ReadFldAlign(STA_TTDVP_00+i*4, STA_TTDVP_00_STA_VP_HISTOGRAM_BIN_1);
    }
}

static UINT8 bDrvTTDCheckCIDInverse(void)
{    
    UINT16* Histogram = gTTDInfo.ttdHist;
    UINT32 i, left, right, u4Diversity = 0;
    INT32 max_index = -1, hist_max = 0, max_index_sec = -1, hist_max_sec = 0;
    UINT32 avg_thld = IO32ReadFldAlign(TTD_FW_REG_03, TTD_AVG_THLD);    
    UINT32 hist_thld = IO32ReadFldAlign(TTD_FW_REG_03, TTD_HIST_THLD);
    UINT32 div_thld_h = IO32ReadFldAlign(TTD_FW_REG_03, TTD_DIVER_THLD_H);    
    UINT32 div_thld_l = IO32ReadFldAlign(TTD_FW_REG_03, TTD_DIVER_THLD_L);
    UINT32 avg_cur = gTTDInfo.ttdCurrentImageAvg/((gTTDInfo.ttdInputWidth>>3)*(gTTDInfo.ttdInputHeight>>3));

    vDrvTTDGetHist(Histogram);
    
    for (i =0; i < 32; i++)
    {
        left = (i>0) ? (i-1) : i;
        right = (i<31) ? (i+1) : i;

        if (((Histogram[left]+hist_thld) <= Histogram[i]) && ((Histogram[right]+hist_thld) <= Histogram[i]))
        {    
            if (Histogram[i] > hist_max)
            {                         
                hist_max_sec = hist_max;
                max_index_sec = max_index;
                hist_max = Histogram[i];
                max_index = i;
            }

            if ((Histogram[i] > hist_max_sec) && (i != max_index))
            {            
                hist_max_sec = Histogram[i];
                max_index_sec = i;
            }
        }
        
        if (IO32ReadFldAlign(TTD_FW_REG_00, TTD_FW_HIST_DBG_EN))
        {
            if ((i%8) == 0) Printf( "\nHist[%d] ", i);
            
            Printf( "%5d  ", Histogram[i]);

            if (i == 31)    Printf( "\n ");
        }
    }

    if ((max_index == -1) && (max_index_sec == -1))
    {
        u4Diversity = div_thld_h;   // force to case2 : use APL
        LOG(2, "No Local Max\n");
    }
    else
    {
        // Calculate the diversity between max index and other bins in the histogram
        for (i=0; i<32; i++)
        {
            u4Diversity += DIFF(Histogram[max_index],Histogram[i])*DIFF(max_index, i);
        }
    }

    u4Diversity = u4Diversity>>13;
    
    LOG(2, "Average   %d  Diversity   %d\n", avg_cur, u4Diversity);
	LOG(2, "Max Ind   %5d     %5d \n", max_index, max_index_sec);
    LOG(2, "Max Val   %5d     %5d \n", hist_max, hist_max_sec);

	vIO32WriteFldAlign(TTD_FW_REG_09, avg_cur, TTD_CUR_AVG);       
	vIO32WriteFldAlign(TTD_FW_REG_09, max_index, TTD_MAX_FIR);        
	vIO32WriteFldAlign(TTD_FW_REG_09, max_index_sec, TTD_MAX_SEC);     
	vIO32WriteFldAlign(TTD_FW_REG_09, u4Diversity, TTD_DIVER); 
    
    // try to do the decision according to the following rules
    // Check the diversity
    // 1. diversity is small --> no inverse is required
    // 2. diversity is moderate --> use apl method + checking two indices are separated far apart
    // 3. diveristy is large --> use max_index as the background index + check two indices are slight separated?
    // Finally change the control register to control the inverse status
    if (u4Diversity <= div_thld_l)
    {        
        LOG(2, "CID case 0   \n");
        return SV_FALSE;
    }
    else if (u4Diversity <= div_thld_h)
    {
        if (((max_index_sec != -1) && (ABS(max_index-max_index_sec) >= 10)) 
            || ((max_index != -1) && (max_index_sec == -1)))  // 1. two max 2. only max, no second max
        {            
            LOG(2, "CID case 1  \n");
            return SV_FALSE;
        }
        else
        {            
            LOG(2, "CID case 2  \n");
            return (avg_cur > avg_thld) ? SV_TRUE : SV_FALSE;
        }
    }
    else  // Large diversity
    {
        if ((max_index_sec != -1) && (ABS(max_index-max_index_sec) >= 6))   // 1. two max 
        {            
            LOG(2, "CID case 3  \n");
            return SV_FALSE;
        }
        else
        {            
            LOG(2, "CID case 4  \n");
            return (max_index > (avg_thld>>3)) ? SV_TRUE : SV_FALSE;
        }
    }    
}

static void vDrvTTDCheckLocalInverseMode()
{
    UINT32 u4LocalInverseThr;
    UINT8 fgInverse = SV_FALSE;

	u4LocalInverseThr = u4DrvTTDGetCidInvThr();
    
    //background is bright, should do inverse
    if (gTTDInfo.ttdCurrentImageAvg > u4LocalInverseThr)
    {
        fgInverse = SV_TRUE;
    }
    
    LOG(6, "Average %d    InverseThr %d \n", gTTDInfo.ttdCurrentImageAvg, u4LocalInverseThr);
    
    if (IO32ReadFldAlign(TTD_FW_REG_00, TTD_FW_HIST_EN))
    {
        fgInverse = bDrvTTDCheckCIDInverse();
    }

	vIO32WriteFldAlign(TTD_FW_REG_00, fgInverse, TTD_FW_IS_INVERSE); 
	
    LOG(2, "Final Iverse = %d\n", fgInverse);
    gTTDInfo.ttdFgLocalInverse = fgInverse;
    //vDrvTTDSetInverseControl(SV_ON,fgInverse);
}

#if 0
//INVERSE CONTROL FOR LOCAL DEPTH MAP
static void vDrvTTDSetInverseControl(UINT8 onOff, UINT8 isInverse)
{
    if(onOff==SV_ON)
    {
        vIO32WriteFldAlign(TTDVP_05 ,1, TTDVP_05_VP_TRANSITION_CTRL_EN);
        if(isInverse== SV_TRUE)
        {
            LOG(7,"Set Local Maps to INVERSE\n");
            vIO32WriteFldAlign(TTDVP_05 ,0, TTDVP_05_VP_TRANSITION_STAT);
        }
        else
        {
		#if defined(CC_MT5396)
            vIO32WriteFldAlign(TTDVP_05, 32, TTDVP_05_VP_TRANSITION_STAT);
		#else // MT5368ES
            vIO32WriteFldAlign(TTDVP_05 ,8, TTDVP_05_VP_TRANSITION_STAT);
		#endif
            LOG(7,"Set Local Maps to NON-INVERSE\n");
       }        
    }
    else
    {
        vIO32WriteFldAlign(TTDVP_05 ,0, TTDVP_05_VP_TRANSITION_CTRL_EN); 
    }
}
#else
static void vDrvTTDSetInverseControl(UINT8 onOff, UINT8 isInverse)
{
    static UINT8 bTransStat = 0;
    
    if (isInverse)
    {
        bTransStat = (bTransStat > 0) ? (bTransStat - 1) : 0;
    }
    else
    {
        bTransStat = (bTransStat < TTD_TRANS_STAT_MAX) ? (bTransStat + 1) : TTD_TRANS_STAT_MAX;
    }  
    
    vDrvTTDSetTransitionCtrlEn(onOff);
    vDrvTTDSetTransitionStat(bTransStat);
}    
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////


