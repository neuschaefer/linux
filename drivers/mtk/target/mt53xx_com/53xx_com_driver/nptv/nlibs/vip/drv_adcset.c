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
 * $Date: 2012/04/27 $
 * $RCSfile: drv_adcset.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#define _ADCSET_C_

#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#include "general.h"
#include "hw_vdoin.h"
#include "drv_adcset.h"
#include "drv_vdoclk.h"
#include "drv_tdc.h"
#include "hw_tvd.h"
#include "api_eep.h"
#include "eepdef.h"
#include "source_table.h"
#include "source_select.h"
//#include "x_pdwnc.h"
#include "video_def.h"
#include "hw_hdtv.h"
#include "debug.h"
#include "hw_async.h"
#include "util.h"
#include "x_printf.h"
#ifndef CC_COPLAT_MT82
#include "x_os.h"
#endif
#include "drv_async.h"
#include "x_timer.h"
#include "nptv_debug.h"
#include "x_typedef.h"
#include "x_hal_5381.h"
#if SUPPORT_SCART
#include "drv_scart.h"
#endif
#include "hw_dvi.h"
#include "drvcust_if.h"
#include "drv_autocolor.h"   
#ifndef CC_COPLAT_MT82
#include "eeprom_if.h"
#endif
#include "hw_ckgen.h" 
#include "x_ckgen.h"
#include "hw_pdwnc.h"
#include "drv_hdtv.h"

#ifdef CC_COPLAT_MT82
// cyj.todo.20090115
BOOL fgApiEepromWriteByte(UINT16 wAddr, UINT8 bData)
{
        return TRUE;
}
#endif

UINT8 bINT_BST_DET;
UINT8 _bPGAGainOffsetHistory[4];
UINT8 _bEFUSE_AUTOCOLOR_READY;

static struct 
{
	UINT8 AP_SliceLevelManuSet;
	UINT8 AP_SVTL_ManuValue;
	UINT8 AP_SVTH_ManuValue;
       UINT8 AP_SVTLH_ManuValue;
} rSlicerConfig={0,0,0,0};

extern UINT8 code bVGADefaultGainTABLE_100[4][3]; 
extern UINT8 code bVGADefaultOffsetTABLE_100[4][3];
extern UINT8 code bVGADefaultGainTABLE_120[4][3];
extern UINT8 code bVGADefaultOffsetTABLE_120[4][3];
extern UINT8 code bVGADefaultGainTABLE_75_100[4][3];
extern UINT8 code bVGADefaultOffsetTABLE_75_100[4][3];
extern UINT16 code wOnChipColorGainTable[4][3];
extern UINT16 code wOnChipColorGainTable_Temp[4][3];
extern INT8 code bEFuseCompensation[4][3];
extern void vDrvGeneralAutoColorGainSetToNextType(UINT16* ColorGainTablePtr, UINT8 bOrgType, UINT8 bNextType);
extern UINT8 bDvMeasureDataSel(void);
//////////////////////////////////////////////////////////////////////

// Firmware Flow Description
//////////////////////////////////////////////////////////////////////
// vDrvVideoHwInit
// => vDrvADCDefaultSetting
//	=> vDrvTVADCDefaultSetting()
//	=> vDrvHDTVADCDefaultSetting()
//
// vApiVideoInit
// => vDrvADCOffsetCal
//	=> vDrvCvbsAdcCal
//		=> vDrvTVADC0CH1ClampOnBottomOffsetCal();
//		=> vDrvTVADC0CH0ClampOnBlankOffsetCal();
//		=> vDrvAAFCalibration();
//			=> vDrvCalibrationEnable
//			=> vDrvCalibrationDisable
//			=> vDrvInputOffsetCal
//			=> wDrvOutputOffsetCal
//			=> vDrvDFE
//	=> vDrvHDTVADCOffsetCal
//
// bApiVideoMainSubSrc
// => Bypass Control
// 	vDrvBypPD
//	vDrvAvByp
//	vDrvSVdoByp
// => AD Setting
//	vDrvSetTVADC
//	vDrvAvMux (source_table.h)
//	vDrvCvbsAdcPD
//	vDrvSetHDTVADC
//	vDrvSetHDTVMux
//	vDrvAllHDADCPow
// => SCART Canal Plus

//////////////////////////////////////////////////////////////////////
// Private Function
//////////////////////////////////////////////////////////////////////

// => vDrvADCDefaultSetting
/**
 * @brief Cvbs ADC Setting
 *
 * @param bVDOINPUT bVDOINPUT is the input types, like CVBS\SVDO
 */
void vDrvTVADCDefaultSetting(void)
{     
       //Set MUX between Gain and PGA
	//vRegWriteFldAlign(VSRC_01, 0x00, VSRC_MUX6);
	//vRegWriteFldAlign(VSRC_01, 0x00, VSRC_MUX5);
	//vRegWriteFldAlign(VSRC_01, 0x00, VSRC_MUX1);
	//vRegWriteFldAlign(VSRC_01, 0x00, VSRC_MUX0);

	vRegWrite4BMsk(VFE_E0, 0x61001f09, 0xffffff3f);  //initial MIX0 gain = 1.33X
	vRegWrite4BMsk(VFE_E1, 0x556c1f00, 0xff6003ff);  //0xff7f7fff
	vRegWrite4BMsk(VFE_E2, 0x0000000c, 0x007fffff);
	vRegWrite4BMsk(VFE_E3, 0x0000333c, 0x000077ff);  //0x1f1f77ff
	vRegWrite4BMsk(VFE_E4, 0x00200000, 0x1fffffff);//AV1
	vRegWrite4BMsk(VFE_E5, 0x43000555, 0x77ff7fff);// RG_MIX0_10U = 100b must match 1.33X to let bapass > 2V
	vRegWrite4BMsk(VFE_E6, 0x550f0f01, 0xff1f1f7f);
	vRegWrite4BMsk(VFE_E7, 0x00006100, 0xff3fffff);//change clamping current to 30uA
//	vRegWrite4BMsk(VFE_E7, 0x00000100, 0xff3fffff);//Robin's suggestion
	vRegWrite4BMsk(VFE_E8, 0x81600000, 0xffe00000);
       vRegWriteFldAlign(VFE_E5, 0x01, RG_MIX0_BEN);   // Command Mode = 1/3 * Vdd = 1V
       vRegWriteFldAlign(VFE_E5, 0x01, RG_MIX1_BEN);   // Command Mode = 1/3 * Vdd = 1V
       //CVBS CLK Select
       ////vRegWriteFldAlign(PLL_REG21, 1, RG_SYS_CVBS_SEL); //CVBS SEL setting : 108MHz this is 8222

       //SWAP ChA/ChB ; the same with MT537x @0609_2007
       vRegWriteFldAlign(VSRC_07, 0x01, VSRC_INV_AIDX); //[SWAP AIDX] could be "00" or "11" in MT5381
       vRegWriteFldAlign(VSRC_07, 0x01, VSRC_SWAP_AD01); //in MT537x, [SWAP AIDX] = "01" and maybe "10"

       //TVD_CLK and TVD_CLK54
       //vRegWrite1BMsk(REG_OCLK, 0xc0, TVD_CLK_SEL); //reversed index;the same with MT537x and MT538x
       //vRegWrite1BMsk(REG_OCLK, 0x20, TVD_CK54_REV); //reversed;the same with MT537x and MT538x
       vIO32WriteFldAlign(CKGEN_AFECFG4, 1, FLD_RG_SYS_CVBS_SEL);//kal 108MHz
       vIO32WriteFldAlign(CKGEN_VPCLK_STOP, 0, FLD_TVD_CK54_STOP);//kal 
       vIO32WriteFldAlign(CKGEN_VPCLK_STOP, 0, FLD_TVD3D_STOP);//kal 
       vIO32WriteFldMulti(CKGEN_VPCLK_STOP,P_Fld(0,FLD_PIP_CLK_STOP)|P_Fld(0,FLD_MAIN_CLK_STOP));
}

//////////////////////////////////////////////////////////////////////
// Public Function
//////////////////////////////////////////////////////////////////////
// => vDrvVideoHwInit
/**
 * @brief ADC Default Setting
 *
 * @param None
 */
void vDrvADCDefaultSetting(void)
{
	vDrvTVADCDefaultSetting();
	vDrvHDTVADCDefaultSetting();
}

// => vApiVideoInit
/**
 * @brief ADC Offset calibration
 *
 * @param None
 */
void vDrvADCOffsetCal(void)
{
   //UINT8 bEEPROMDATA0,bLoopCnt;
	//CVBS ADC Calibration
	vDrvCvbsAdcCal();
	
  vRegWriteFldAlign(HDFE_02, 0x00, AD1_OFFSET);  //set digital offset =1 for CH 1
  vRegWriteFldAlign(HDFE_02, 0x00, AD2_OFFSET);	 //set digital offset =1 for CH 2 
  vRegWriteFldAlign(HDFE_00, 0x00, AD3_OFFSET);  //set digital offset =1 for CH 3
  
}

// => vDrvSetTVADC
/**
 * @brief SVDO enable
 *
 * @param None
 */
void vDrvSVdoEnable(void)
{
	// Enable SVDO
	vRegWriteFldAlign(VSRC_07, 1, VSRC_SVID);
	vRegWriteFldAlign(SECAM_01, 0, DOT_REDUCE);
#if defined(CC_SUPPORT_TVE)||defined(CC_SUPPORT_TVE_82xx)
	//for SCART out
       vRegWriteFldAlign(SVF_03, 0x01, SVF_YCMIX_SEL);
       vRegWriteFldAlign(SVF_03, 0x01, SVF_MOFC_EN); // for SECAM HW modification
#endif
}

// => vDrvSetTVADC
/**
 * @brief SVDO Disable
 *
 * @param None
 */
void vDrvSVdoDisable(void)
{
	vRegWriteFldAlign(VSRC_07, 0, VSRC_SVID);
	vRegWriteFldAlign(SECAM_01, 1, DOT_REDUCE);
#if defined(CC_SUPPORT_TVE)||defined(CC_SUPPORT_TVE_82xx)
	//for SCART out
       vRegWriteFldAlign(SVF_03, 0x00, SVF_YCMIX_SEL);
       vRegWriteFldAlign(SVF_03, 0x00, SVF_MOFC_EN); 
#endif
}

// => bApiVideoMainSubSrc
/**
 * @brief AV/SVDO BYPASS Power down
 * @param None
 */
void vDrvBypPD(void)
{
       vRegWriteFldAlign(VFE_E0, 1, RG_MIX0_PWD);//power down mixer
       vRegWriteFldAlign(VFE_E0, 0, RG_MIX0_INPUT);
       vRegWriteFldAlign(VFE_E0, 1, RG_MIX1_PWD);//power down mixer
       vRegWriteFldAlign(VFE_E0, 0, RG_MIX1_INPUT);
}
// => bApiVideoMainSubSrc
/**
 * @brief AV Byopass enable
 * @param None
 */
void vDrvAvByp(void)
{
       //// 1st set mixer used, 2nd mixer .......wait
       vRegWriteFldAlign(VFE_E0, 1, RG_MIX0_INPUT);
       vRegWriteFldAlign(VFE_E5, 2, RG_MIX0_SHORT);   //short chB
       vRegWriteFldAlign(VFE_E0, 0, RG_MIX0_PWD); // Power on  Mixer0
       vRegWriteFldAlign(VFE_E0, 2, RG_MIX0_GAIN);   //MIX0 gain = 1.33X

       #if (BYPASS_MIXER1_TO_SLICER)
       vRegWriteFldAlign(VFE_E0, 1, RG_MIX0_PWD);   // Power down    Mixer 0
       vRegWriteFldAlign(VFE_E0, 0, RG_MIX0_INPUT);
       vRegWriteFldAlign(VFE_E0, 0x00, RG_MIX1_PWD);   // Power On    Mixer 1
       vRegWriteFldAlign(VFE_E0, 0x01, RG_MIX1_INPUT); // Mixer1 Source : A input
       vRegWriteFldAlign(VFE_E5, 2, RG_MIX1_SHORT);   //short chB
       vRegWriteFldAlign(VFE_E0, 0x1, RG_MIX1_GAIN);  // Mixer1 Gain : 1.25x to avoid I/P offset and the ofset make sync bottom loer??? why   
       vRegWriteFldAlign(VFE_E5, 0x01, RG_MIX1_BEN);   // Command Mode = 1/3 * Vdd = 1V
       vRegWriteFldAlign(VFE_E5, 0x0,RG_MIX1_10U);//for small CVBS I/P level
       // Scart RGB --> YC Mixer1 -->Sync Slicer
       vRegWriteFldAlign(VFE_E7, 0x00, RG_CVBS2SYNCA_EN);   
       vRegWriteFldAlign(VFE_E7, 0x01, RG_CVBS2SYNCB_EN);    

       vRegWriteFldAlign(VFE_E0, 0x01, RG_MIX0_PWD);   // Power down Mixer 0
       vRegWriteFldAlign(VFE_E0, 0x00, RG_MIX0_INPUT); // Mixer0 Source : No input
       
	#else	
       // Scart RGB --> YC Mixer0 -->Sync Slicer
       vRegWriteFldAlign(VFE_E7, 0x01, RG_CVBS2SYNCA_EN);
       vRegWriteFldAlign(VFE_E7, 0x00, RG_CVBS2SYNCB_EN);  
       #endif
	}

void vDrvSVdoByp(void)
{
       //// 1st set mixer used, 2nd mixer .......wait

       #if (BYPASS_MIXER1_TO_SLICER)
       vRegWriteFldAlign(VFE_E0, 3, RG_MIX1_INPUT);
       vRegWriteFldAlign(VFE_E5, 0, RG_MIX1_SHORT);   //short chB
       vRegWriteFldAlign(VFE_E0, 0, RG_MIX1_PWD);//power down mixer
       vRegWriteFldAlign(VFE_E0, 2, RG_MIX1_GAIN);    //MIX0 gain = 1.33X

       vRegWriteFldAlign(VFE_E0, 0x01, RG_MIX0_PWD);   // Power down Mixer 0
       vRegWriteFldAlign(VFE_E0, 0x00, RG_MIX0_INPUT); // Mixer0 Source : No input
       #else   
       vRegWriteFldAlign(VFE_E0, 3, RG_MIX0_INPUT);
       vRegWriteFldAlign(VFE_E5, 0, RG_MIX0_SHORT);   //short chB
       vRegWriteFldAlign(VFE_E0, 0, RG_MIX0_PWD);//power down mixer
       vRegWriteFldAlign(VFE_E0, 2, RG_MIX0_GAIN);    //MIX0 gain = 1.33X
       #endif
}

static struct 
{
	UINT8 AP_SrcMuxCtrl;
	UINT8 AP_SrcMuxVal;
	UINT8 AP_GainCtrl;
	UINT8 AP_GainVal;
} rChBConfig={0,0,0,0};

/**
 * @brief bDrvSetTVDCHBGain(void)
 * @param  void
 * @retval UINT8 manual analog gain code
 */
void vDrvSetTVDCHBSrcMux(UINT8 value)
{
	rChBConfig.AP_SrcMuxCtrl=1;
	rChBConfig.AP_SrcMuxVal=value;
	vRegWriteFldAlign(VSRC_01, value, VSRC_MUX1);	//Set ChB Gain code from HDTV_04
	if(rChBConfig.AP_GainCtrl)
	{
		vRegWriteFldAlign(HDTV_04, rChBConfig.AP_GainVal, HDTV_AGC);	//Set ChB gain="10001"==>1.496x;0418_007
	}
}

/**
 * @brief bDrvSetTVDCHBGain(void)
 * @param  void
 * @retval UINT8 manual analog gain code
 */
void vDrvSetTVDCHBGain(UINT8 value)
{
	rChBConfig.AP_GainCtrl=1;
	rChBConfig.AP_GainVal=value;
	if(rChBConfig.AP_SrcMuxCtrl)
	{
		vRegWriteFldAlign(HDTV_04, value, HDTV_AGC);	//Set ChB gain="10001"==>1.496x;0418_007
	}
}

// => bApiVideoMainSubSrc
/**
 * @brief Cvbs ADC Setting
 *
 * @param bVDOINPUT bVDOINPUT is the input types, like CVBS\SVDO
 */
void vDrvSetTVADC(UINT8 vsrc)
{
	
	switch (VSS_MAJOR(vsrc))
	{
		case VSS_ATD:
			/* enable internal ATV demod setting here */
			//Printf(">>>>>>enter set VSS_ATD\n");
			vTvd3dATDInit(SV_ON);
			vDrvCvbsVfePD();
                     vIO32WriteFldMulti(CKGEN_VPCLK_CFG,P_Fld(2,FLD_TVD_CLK_SEL)|P_Fld(1,FLD_ADC_C54M_SEL)|P_Fld(0,FLD_TVD_CK54_SEL));
                    
			vRegWriteFldAlign(DFE_03, 0x0c, AGC2_MODE);
			vDrvSVdoDisable();							//SVDO Disable
			vRegWriteFldAlign(VSRC_07, SV_ON, ATD_ENABLE);	//set ATD path for TVD
			break;

		case VSS_ATV:
			vTvd3dATDInit(SV_OFF);
			vDrvCvbsVfePWON();
			vRegWriteFldAlign(VSRC_01, 2, VSRC_MUX1);	//Set ChB Gain code from HDTV_04
			vRegWriteFldAlign(HDTV_04, 0x11, HDTV_AGC);	//Set ChB gain="10001"==>1.496x;0418_007

                     vRegWrite1BMsk(VFE_E0_2, 0x00 , 0x10 );//set chB RG_CALCTB = 0 
                     vRegWrite1BMsk(VFE_E1_1, 0x01 , 0x01 );//set chB RG_VAGSELB = 1 
                     vRegWrite1BMsk(VFE_E1_0, 0x00 , 0x50 );//set chB RG_CALPGAB = 0, RG_C_SELB = 0. 
                     vRegWrite1BMsk(VFE_E0_0, 0x20 , 0x20 );//set chB RG_CLAMP_SOURCEB = 0x20 
                     vRegWrite1BMsk(VFE_E0_1, 0x40 , 0x40 );//set chB RG_SINKB_EN = 1 
                     vRegWrite1BMsk(VFE_E0_2, 0x01 , 0x01 );//set chB RG_SOURCEB_EN = 1
                     vRegWrite1BMsk(VFE_E1_2, 0x00 , 0x20 );//set chB RG_UPDNB = 0 
                     vRegWrite1BMsk(VFE_E2_0, 0x00 , 0x15 );//set chB RG_SHIFTB_PWD = 0, RG_SHIFT2B_PWD = 0, RG_BP_PGABUFB = 0. 
                     
			// Printf(">>>>>>enter set ATV ADC\n");
			/*************************/
			//setting of ADC, ADC clock and TVD clock;0523_2007
                     vRegWriteFldAlign(VFE_E8, 0, RG_CVBS_ADC_PWD);
                     //SWAP ChA/ChB
	              vRegWriteFldAlign(VSRC_07, 0x01, VSRC_INV_AIDX);	//[SWAP AIDX] could be "00" or "11"
	              vRegWriteFldAlign(VSRC_07, 0x01, VSRC_SWAP_AD01); //SWAP ChA and ChB, for 5381 , by hua
                     vIO32WriteFldMulti(CKGEN_VPCLK_CFG,P_Fld(2,FLD_TVD_CLK_SEL)|P_Fld(0,FLD_ADC_C54M_SEL)|
                           P_Fld(0,FLD_TVD_CK54_SEL)); 

			vRegWriteFldAlign(DFE_03, 0x08, AGC2_MODE);
			/*************************/

			vDrvSVdoDisable();							//SVDO Disable
			//vDrvAvByp();								//Set CVBS Bypass
			vRegWriteFldAlign(VSRC_07, SV_OFF, ATD_ENABLE);	//disable ATD path for TVD
			break;

		case VSS_CVBS:
			//P_C0~P_C3, Composite signal, set Ch B PGA Manual gain to minimal to prevent signal out of range
			vTvd3dATDInit(SV_OFF);
			vDrvCvbsVfePWON();
			//vRegWriteFldAlign(VFE_20, 2, RG_GMUXSEL);////mt5382p, but 5391 no ,ground mux selection 
			vRegWriteFldAlign(VSRC_01, 2, VSRC_MUX1);	//Set ChB Gain code from HDTV_04
                     if((_bMainICIn == P_FB0) || (_bSubICIn == P_FB0) || (_bMainICIn == P_FB1) || (_bSubICIn == P_FB1)) //set gain of CHB from TVD when SCART
			{
                            vRegWriteFldAlign(VSRC_01, 0, VSRC_MUX1);	//Set ChB Gain code from TVD
				//vRegWriteFldAlign(HDTV_04, 0x0, HDTV_AGC);	//gain=1.0x
				//LogS("Enter P_FB");
			}
			else
			{
				//vRegWriteFldAlign(HDTV_04, 0x1F, HDTV_AGC);	//gain=1.5x
				vRegWriteFldAlign(HDTV_04, 0x11, HDTV_AGC);	//Set ChB gain="10001"==>1.496x;0418_007
			}
			//vRegWriteFldAlign(HDTV_04, 0x11, HDTV_AGC);	//Set ChB gain="10001"==>1.496x;0418_007

                     vRegWrite1BMsk(VFE_E0_2, 0x00 , 0x10 );//set chB RG_CALCTB = 0 
                     vRegWrite1BMsk(VFE_E1_1, 0x01 , 0x01 );//set chB RG_VAGSELB = 1 
                     vRegWrite1BMsk(VFE_E1_0, 0x00 , 0x50 );//set chB RG_CALPGAB = 0, RG_C_SELB = 0. 
                     vRegWrite1BMsk(VFE_E0_0, 0x20 , 0x20 );//set chB RG_CLAMP_SOURCEB = 0x20 
                     vRegWrite1BMsk(VFE_E0_1, 0x40 , 0x40 );//set chB RG_SINKB_EN = 1 
                     vRegWrite1BMsk(VFE_E0_2, 0x01 , 0x01 );//set chB RG_SOURCEB_EN = 1
                     vRegWrite1BMsk(VFE_E1_2, 0x00 , 0x20 );//set chB RG_UPDNB = 0 
                     vRegWrite1BMsk(VFE_E2_0, 0x00 , 0x15 );//set chB RG_SHIFTB_PWD = 0, RG_SHIFT2B_PWD = 0, RG_BP_PGABUFB = 0. 

                     #if SUPPORT_SCART
                     if (((_bMainICIn == P_FB0) || (_bSubICIn == P_FB0) || (_bMainICIn == P_FB1) || (_bSubICIn == P_FB1))  && (bINT_BST_DET == 1)) //internal path
			{
                           #if HDTV_BST_DET //TVD bst detector 
                                 vRegWrite1BMsk(VFE_E0_1, 0x00 , 0x40 );//set chB RG_SINKB_EN = 0
                                 vRegWrite1BMsk(VFE_E0_2, 0x00 , 0x01 );//set chB RG_SOURCE_EN = 0
                                 //LogS("bINT_BST_DET==1 and HDTV_BST at P_FB");
                           #else //HDTV bst detector 
                                 vRegWrite1BMsk(VFE_E0_1, 0x00 , 0x40 );//set chB RG_SINKB_EN = 0
                                 vRegWrite1BMsk(VFE_E0_2, 0x00 , 0x01 );//set chB RG_SOURCE_EN = 0
                                 vRegWriteFldAlign(VFE_E2, 1,  RG_SHIFTB_PWD);
                                 //LogS("bINT_BST_DET==1 and TVD_BST at P_FB");
                           #endif
                     }
                     #endif
                     
			// Printf(">>>>>>enter set cvbs ADC\n");
			/*************************/
			//setting of ADC, ADC clock and TVD clock;0523_2007
                     vRegWriteFldAlign(VFE_E8, 0, RG_CVBS_ADC_PWD);

                     //SWAP ChA/ChB
	              vRegWriteFldAlign(VSRC_07, 0x01, VSRC_INV_AIDX);	//[SWAP AIDX] could be "00" or "11"
	              vRegWriteFldAlign(VSRC_07, 0x01, VSRC_SWAP_AD01); //SWAP ChA and ChB, for 5381 , by hua
                     vIO32WriteFldMulti(CKGEN_VPCLK_CFG,P_Fld(2,FLD_TVD_CLK_SEL)|P_Fld(0,FLD_ADC_C54M_SEL)|
                            P_Fld(0,FLD_TVD_CK54_SEL)); 

			vRegWriteFldAlign(DFE_03, 0x08, AGC2_MODE);
			/*************************/

			vDrvSVdoDisable();							//SVDO Disable
			//vDrvAvByp();								//Set CVBS Bypass
			vRegWriteFldAlign(VSRC_07, SV_OFF, ATD_ENABLE);	//disable ATD path for TVD
			break;

		case VSS_SV:
			//P_S0~P_S2, S-Video
			vTvd3dATDInit(SV_OFF);
			vDrvCvbsVfePWON();
                     if((_bMainICIn==P_FB0)||(_bSubICIn==P_FB0)) //kal cheng for MT5335 SCART Y-C
                     {
                         vRegWriteFldAlign(VSRC_01, 2, VSRC_MUX1);  //select AGC0B ~ from HDTV
                         vRegWriteFldAlign(HDTV_04, 8, HDTV_AGC); //Set PGA Manual gain 
                     }
                     else
                     {
		    	    vRegWriteFldAlign(VSRC_01, 0, VSRC_MUX1);	//set AGC from TVD3D for chroma;0418_2007
			    //vRegWriteFldAlign(HDTV_04, 8, HDTV_AGC);	//Set PGA Manual gain = 1.5 to prevent chroma cutoff
                     }

                     vRegWrite1BMsk(VFE_E0_2, 0x00 , 0x14 );//set chB RG_CALCTB = 0, RG_CTCLAMP_CTMB = 0. 
                     vRegWrite1BMsk(VFE_E1_3, 0x05 , 0x0f );//set chB RG_CLAMP_SINKB = 5 
                     vRegWrite1BMsk(VFE_E1_1, 0x1c , 0x7d );//set chB RG_CALCLAMPB = , RG_VAGSELB = 0. 
                     vRegWrite1BMsk(VFE_E1_0, 0x14 , 0x55 );//set chB RG_CALPGAB = 0, RG_C_SELB = 1, RG_HPCLAMPB = 1, RG_CLAMPC_TSTB = 0. // 0x14->0x10->0x18(analog always clamping)   
                     vRegWrite1BMsk(VFE_E0_0, 0x09 , 0x3f );//set chB RG_CLAMP_SOURCEB = 9 
                     vRegWrite1BMsk(VFE_E0_1, 0x00 , 0x40 );//set chB RG_CLAMP_SINKB_EN = 0 
                     vRegWrite1BMsk(VFE_E0_2, 0x00 , 0x01 );//set chB RG_CLAMP_SOURCEB_EN = 0 
                     vRegWrite1BMsk(VFE_E1_2, 0x00 , 0x20 );//set chB RG_UPDNB = 0 
                     vRegWrite1BMsk(VFE_E2_0, 0x14 , 0x55 );//set chB RG_BUFB_PWD = 0, RG_SHIFTB_PWD = 1, RG_SHIFT2B_PWD = 1, RG_BP_PGABUFB = 1 // 0x15->0x14
                     vRegWrite1BMsk(VFE_E2_1, 0x00 , 0x35 );//set chB RG_AAFB_PWD = 0, RG_PGABUFNB_PWD = 0, RG_PGABUFPB_PWD = 0                 // 0x00->0x30->0x00
                     vRegWrite1BMsk(VFE_E2_2, 0x00 , 0x29 );//set chB RG_CTCLAMPB_PWD = 0, RG_CTBUFB_PWD = 0, RG_PGAB_PWD = 0 
                     vRegWrite1BMsk(VFE_E3_0, 0x04 , 0x15 );//set chB RG_VCOM_SEL_PGAB = 0, RG_VCOM_SEL_FLTB = 1, RG_CTINB = 0 
                     vRegWrite1BMsk(VFE_E3_2, 0x00 , 0x1f );//set chB RG_OFFCURB = 0x00 
                     
			//Printf(">>>>>>enter set SV ADC\n");
			/*************************/
			//setting of ADC, ADC clock and TVD clock;0523_2007
                     vRegWriteFldAlign(VFE_E8, 0, RG_CVBS_ADC_PWD);

                     //SWAP ChA/ChB
	              vRegWriteFldAlign(VSRC_07, 0x01, VSRC_INV_AIDX);	//[SWAP AIDX] could be "00" or "11"
	              vRegWriteFldAlign(VSRC_07, 0x01, VSRC_SWAP_AD01); //SWAP ChA and ChB, for 5381 , by hua
                     vIO32WriteFldMulti(CKGEN_VPCLK_CFG,P_Fld(2,FLD_TVD_CLK_SEL)|P_Fld(0,FLD_ADC_C54M_SEL)|
                            P_Fld(0,FLD_TVD_CK54_SEL)); 
                    

			vRegWriteFldAlign(DFE_03, 0x08, AGC2_MODE);
			/*************************/

			vDrvSVdoEnable();							//Enable SVDO
			//vDrvSVdoByp();								//Enable SVDO bypass
			vRegWriteFldAlign(VSRC_07, SV_OFF, ATD_ENABLE);	//disable ATD path for TVD
			break;

		case VSS_SCART:
			//SCART Setting
			vTvd3dATDInit(SV_OFF);
			vDrvCvbsVfePWON();
			/*************************/
			//setting of ADC, ADC clock and TVD clock;0523_2007
			//Clock setting
                     //vRegWrite4BMsk(VFE_27, 0xaaf040a0, 0xffffffff); //mt5382p

                     vRegWrite1BMsk(VFE_E0_2, 0x00 , 0x10 );//set chB RG_CALCTB = 0 
                     vRegWrite1BMsk(VFE_E1_1, 0x01 , 0x01 );//set chB RG_VAGSELB = 1 
                     vRegWrite1BMsk(VFE_E1_0, 0x00 , 0x50 );//set chB RG_CALPGAB = 0, RG_C_SELB = 0. 
                     vRegWrite1BMsk(VFE_E0_0, 0x20 , 0x20 );//set chB RG_CLAMP_SOURCEB = 0x20 
                     vRegWrite1BMsk(VFE_E0_1, 0x00 , 0x40 );//set chB RG_SINKB_EN = 0
                     vRegWrite1BMsk(VFE_E0_2, 0x00 , 0x01 );//set chB RG_SOURCE_EN = 0 
                     vRegWrite1BMsk(VFE_E1_2, 0x00 , 0x20 );//set chB RG_UPDNB = 0 
                     vRegWrite1BMsk(VFE_E2_0, 0x00 , 0x15 );//set chB RG_SHIFTB_PWD = 0, RG_SHIFT2B_PWD = 0, RG_BP_PGABUFB = 0. 
                     vIO32WriteFldMulti(CKGEN_VPCLK_CFG,P_Fld(2,FLD_TVD_CLK_SEL)|P_Fld(0,FLD_ADC_C54M_SEL)|
                             P_Fld(0,FLD_TVD_CK54_SEL));

                     vRegWriteFldAlign(DFE_03, 0x08, AGC2_MODE);
			/*************************/

			vRegWriteFldAlign(VSRC_07, SV_OFF, ATD_ENABLE);	//disable ATD path for TVD
			break;

		default:
			assert(0);
	}
}

#if CLI_CVBS
void vDrvCVBSPGACheck(void)
{//FLUKE white pattern video 50%
  UINT8 i;
  UINT16 wPY_LV;
  
  vRegWriteFldAlign(DFE_03, 0x0E, AGC2_MODE); // A/D code fix
  vRegWriteFldAlign(DFE_03, 0x07, AGC2_PK_MODE); // no peak limit

  for(i=0;i<=0x78;i+=8)
  {
     vRegWriteFldAlign(DFE_07, i, AGC2_MANUAL_ACODE); // set A code
     vUtDelay1ms(100);
     wPY_LV = RegReadFldAlign(DFE_STA_03, PY_LV);
     //Printf("CVBS PGA Check acode=%2x, wPY_LV = %4d\n", i, wPY_LV );
     Printf("CVBS PGA Check acode= %2x, wPY_LV = %3d\n", i, wPY_LV );

  }

  vRegWriteFldAlign(DFE_07, 0x7f, AGC2_MANUAL_ACODE); // set A code
  vUtDelay1ms(1);
  wPY_LV = RegReadFldAlign(DFE_STA_03, PY_LV);
  //Printf("CVBS PGA Check acode=7f, wPY_LV = %4d\n", wPY_LV);
  Printf("CVBS PGA Check acode= 7f, wPY_LV = %3d\n", wPY_LV);

  vRegWriteFldAlign(DFE_03, 0x08, AGC2_MODE);
  vRegWriteFldAlign(DFE_03, 0x0F, AGC2_PK_MODE);

}

void vDrvTargetBlankCheck(void)
{//FLUKE white pattern pure AV

  UINT16 wTargetBlank;
  UINT16 wBlank_LV;
  
  for(wTargetBlank=0;wTargetBlank<=1023;wTargetBlank++)
  {
     vRegWriteFldAlign(DFE_0A, wTargetBlank, CLAMP_TARGET_BLANK_LV); // set target blank
     vUtDelay1ms(50);
     wBlank_LV = RegReadFldAlign(DFE_STA_01, BLANK_LV);
     //Printf("TBCheck wTB= %4d,wBL= %4d\n", wTargetBlank, wBlank_LV );
     Printf("TBCheck wTB= %4d ,wBL= %4d\n", wTargetBlank, wBlank_LV );
  }
   wBlank_LV = 256;
   vRegWriteFldAlign(DFE_0A, wBlank_LV, CLAMP_TARGET_BLANK_LV);
}

void vDrvIOOffsetBlankCheck(void)
{//FLUKE white pattern pure AV

  UINT8 bCALCLAMP,bOFFCUR;

  for(bCALCLAMP=0;bCALCLAMP<=0x0f;bCALCLAMP++)
     for(bOFFCUR=0;bOFFCUR<=0x1f;bOFFCUR++)
     {
        vRegWriteFldAlign(VFE_E1, bCALCLAMP, RG_CALCLAMPA); 
        vRegWriteFldAlign(VFE_E3, bOFFCUR, RG_OFFCURA);
        vUtDelay1ms(50);
        //Printf("IOOBCheck I= %2d, J= %2d, wBL= %4d\n", bCALCLAMP,bOFFCUR, RegReadFldAlign(DFE_STA_01, BLANK_LV) );
        Printf("IOOBCheck I= %2d ,J= %2d ,wBL= %4d\n", bCALCLAMP,bOFFCUR, RegReadFldAlign(DFE_STA_01, BLANK_LV) );
     }
        vRegWriteFldAlign(VFE_E1, 0x07, RG_CALCLAMPA); //default value
        vRegWriteFldAlign(VFE_E3, 0x00, RG_OFFCURA);     //default value
}

void vDrvPeakLimitCheck(UINT8 PYC)
{//FLUKE white pattern pure AV

  UINT16 PYLIM;
  UINT16 PYCLIM;
  UINT8 BYCLIM;

  switch(PYC)
  {
    case 1://Peak Y
               vRegWriteFldAlign(DFE_03, 0x0C, AGC2_PK_MODE);
               for(PYLIM=0x0f; PYLIM<0x1000;PYLIM+=16)
                    {
                        vRegWriteFldAlign(DFE_04, PYLIM, AGC2_PY_LIM);
                        vUtDelay1ms(50);
                        //Printf("PKCheck PYLIM= %4d, PYLV= %4d\n", PYLIM, RegReadFldAlign(DFE_STA_03, PY_LV));
                        Printf("PKCheck PYLIM= %4d ,PYLV= %4d\n", PYLIM, RegReadFldAlign(DFE_STA_03, PY_LV));
                   }
               vRegWriteFldAlign(DFE_04, 0x890, AGC2_PY_LIM);//recovery
    break;


    case 2://Peak YC
               vRegWriteFldAlign(DFE_03, 0x0a, AGC2_PK_MODE);
               for(PYCLIM=0x00; PYCLIM<0x1000;PYCLIM+=16)
                    {
                        vRegWriteFldAlign(DFE_04, PYCLIM, AGC2_PYC_LIM);
                        vUtDelay1ms(50);
                        //Printf("PKCheck PYCLIM= %4d, PYCLV= %4d\n", PYCLIM, RegReadFldAlign(DFE_STA_03, PYC_LV));
                        Printf("PKCheck PYCLIM= %4d ,PYCLV= %4d\n", PYCLIM, RegReadFldAlign(DFE_STA_03, PYC_LV));
                   }
               vRegWriteFldAlign(DFE_04, 0xF80, AGC2_PYC_LIM);//recovery
    break;


    case 3://Bottom YC
               vRegWriteFldAlign(DFE_03, 0x09, AGC2_PK_MODE);
               for(BYCLIM=0x00; BYCLIM<8;BYCLIM++)
                    {
                        vRegWriteFldAlign(DFE_04, BYCLIM, AGC2_BYC_LIM);
                        vUtDelay1ms(50);
						//Printf("PKCheck BYCLIM= %4d, BYCLV= %4d\n", BYCLIM, RegReadFldAlign(DFE_STA_02, BYC_LV));
                        Printf("PKCheck BYCLIM= %4d ,BYCLV= %4d\n", BYCLIM, RegReadFldAlign(DFE_STA_02, BYC_LV));
                   }
               vRegWriteFldAlign(DFE_04, 0x05, AGC2_BYC_LIM);//recovery
    break;


    default:
    break;

}
   vRegWriteFldAlign(DFE_03, 0x0F, AGC2_PK_MODE);//recovery

}

void vDrvSVFTest(void)
{//plug AV and SV
  UINT8 bVpres2, bVpres3;
  
  //接上AV1, SV1端子,並送CVBS signal
  //0x20022790 Bit[0] = 1 (SV.SY = SY0?ChB)
  vRegWriteFldAlign(VFE_E4,(RegReadFldAlign(VFE_E4, RG_AISELR)|0x001), RG_AISELR);

  //0x20022780 Bit[16] = 1(CT mode)
  vRegWriteFldAlign(VFE_E0,1, RG_CLAMP_SOURCEB_EN);

  //0x20022784 Bit[21] = 0(CT mode)
  vRegWriteFldAlign(VFE_E1,0, RG_UPDNB);

  //Set  0x 20022638 Bit[31] = "1"
  vRegWriteFldAlign(SVF_00,1, SVF_EN);

  //Set  0x20022638 Bit[30] = "0" [Vpres 2]
  vRegWriteFldAlign(SVF_00,0, SVF_VPRES_SEL);
  vUtDelay1ms(500);

  //Read 0x20022080 Bit[13] ==> 需為"1"
  bVpres2 = RegReadFldAlign(STA_CDET_00, VPRES_SVF);

  //Set  0x20022638 Bit[30] = "1" [Vpres 3]
  vRegWriteFldAlign(SVF_00,1, SVF_VPRES_SEL);
  vUtDelay1ms(500);

  //Read：0x20022080 Bit[13] ==> 需為"1"
  bVpres3 = RegReadFldAlign(STA_CDET_00, VPRES_SVF);

  Printf("vDrvSVFTest bVpres2 = %x, bVpres3 = %x\n", bVpres2, bVpres3);

}
#endif

/**
 * @brief Set SIF ADC for tuner bypass
 *
 * @param None
 */
#ifdef CC_MT5363
void vDrvSetSIFADC(void)
{
//SIF ADC
    vRegWriteFldAlign(VFE_E8, SV_OFF, RG_REFGEN_PWD);//disable power down
    
    vRegWriteFldAlign(VFE_E9, SV_OFF, RG_PGABUFNC_PWD);
    vRegWriteFldAlign(VFE_E9, SV_OFF, RG_PGABUFPC_PWD);
    vRegWriteFldAlign(VFE_E9, SV_OFF, RG_BUFC_PWD);
    vRegWriteFldAlign(VFE_E9, SV_OFF, RG_SHIFTC_PWD);
    vRegWriteFldAlign(VFE_E9, SV_OFF, RG_SHIFT2C_PWD);
    vRegWriteFldAlign(VFE_E9, SV_ON, RG_CLAMP_SOURCEC_EN);
    vRegWriteFldAlign(VFE_E9, SV_ON, RG_CLAMP_SINKC_EN);
    vRegWriteFldAlign(VFE_E9, SV_ON, RG_UPDNC);//clamp on ~0V

    vRegWriteFldAlign(VFE_EA, SV_OFF, RG_40U_PGAOFFC);
    vRegWriteFldAlign(VFE_EA, 0x1f, RG_OFFCURC);
    vRegWriteFldAlign(VFE_EA, SV_OFF, RG_CTCLAMPC_PWD);
    vRegWriteFldAlign(VFE_EA, SV_OFF, RG_CTBUFC_PWD);
    vRegWriteFldAlign(VFE_EA, SV_OFF, RG_PGAC_PWD);
    vRegWriteFldAlign(VFE_EA, SV_OFF, RG_AAFC_PWD);

    vRegWriteFldAlign(VFE_EB, 0xe, RG_PGAGAINC);

    vRegWriteFldAlign(VFE_EB, 0xe, RG_PGAGAINC);

    IO_WRITE32MSK(AUDIO_BASE, 0x22c, 0x00200000, 0x00220000); //bit17=0, bit21=1             

    vRegWriteFldAlign(VFE_E4,(RegReadFldAlign(VFE_E4, RG_AISEL_9_8)|0x1), RG_AISEL_9_8);

//SIF CLK 54M setting 
    IO_WRITE32(CKGEN_BASE, 0x130, 0x00099403); 
    IO_WRITE32(CKGEN_BASE, 0x10c, 0x198c0210); 
    IO_WRITE32(CKGEN_BASE, 0x110, 0xc10f0004); 
    IO_WRITE32(CKGEN_BASE, 0x114, 0x00003016); //54MHz; 00003001//27Mhz
}
#endif

// => bApiVideoMainSubSrc
/**
 * @brief power down Cvbs ADC
 *
 * @param None
 */
void vDrvCvbsAdcPD(void)
{
#if PD_ADC_ENABLE
#ifndef __MODEL_slt__
     vRegWriteFldAlign(VFE_E8, 1, RG_REFGEN_PWD);
#endif
     vRegWriteFldAlign(VFE_E8, 1, RG_CVBS_ADC_PWD);
#endif
}

// => bApiVideoMainSubSrc
/**
 * @brief power on Cvbs ADC
 *
 * @param None
 */
void vDrvCvbsAdcPWON(void)
{
#if PD_ADC_ENABLE
#ifndef __MODEL_slt__
     vRegWriteFldAlign(VFE_E8, 0, RG_REFGEN_PWD);
#endif
     vRegWriteFldAlign(VFE_E8, 0, RG_CVBS_ADC_PWD);
#endif
}

// => bApiVideoMainSubSrc
/**
 * @brief power down Cvbs ADC
 *
 * @param None
 */
void vDrvCvbsVfePD(void)
{
#if PD_ADC_ENABLE
     vRegWriteFldAlign(VFE_E0, 1, RG_MIX0_PWD);
     vRegWriteFldAlign(VFE_E0, 1, RG_MIX1_PWD);
     vRegWriteFldAlign(VFE_E0, 0, RG_MIX0_INPUT);
     vRegWriteFldAlign(VFE_E0, 0, RG_MIX1_INPUT);
     vRegWriteFldAlign(VFE_E2, 1, RG_INMUX_PWD);
     vRegWriteFldAlign(VFE_E2, 1, RG_PGAA_PWD);
     vRegWriteFldAlign(VFE_E2, 1, RG_PGABUFPA_PWD);
     vRegWriteFldAlign(VFE_E2, 1, RG_PGABUFNA_PWD);
     vRegWriteFldAlign(VFE_E2, 1, RG_AAFA_PWD);
     vRegWriteFldAlign(VFE_E2, 1, RG_CTCLAMPA_PWD);
     vRegWriteFldAlign(VFE_E2, 1, RG_BUFA_PWD);
     vRegWriteFldAlign(VFE_E2, 1, RG_CTBUFA_PWD);
     vRegWriteFldAlign(VFE_E2, 1, RG_PGAB_PWD);
     vRegWriteFldAlign(VFE_E2, 1, RG_PGABUFPB_PWD);
     vRegWriteFldAlign(VFE_E2, 1, RG_PGABUFNB_PWD);
     vRegWriteFldAlign(VFE_E2, 1, RG_AAFB_PWD);
     vRegWriteFldAlign(VFE_E2, 1, RG_CTCLAMPB_PWD);
     vRegWriteFldAlign(VFE_E2, 1, RG_BUFB_PWD);
     vRegWriteFldAlign(VFE_E2, 1, RG_CTBUFB_PWD);
#ifndef __MODEL_slt__     
     vRegWriteFldAlign(VFE_E8, 1, RG_REFGEN_PWD);
#endif     
     vRegWriteFldAlign(VFE_E8, 1, RG_CVBS_ADC_PWD);
     vIO32WriteFldAlign(CKGEN_VPCLK_STOP, 1, FLD_TVD3D_STOP); 
#endif
}

// => bApiVideoMainSubSrc
/**
 * @brief power on Cvbs ADC
 *
 * @param None
 */
void vDrvCvbsVfePWON(void)
{
#if PD_ADC_ENABLE
   #if (BYPASS_MIXER1_TO_SLICER)
     vRegWriteFldAlign(VFE_E0, 0, RG_MIX1_PWD);
   #else
     vRegWriteFldAlign(VFE_E0, 0, RG_MIX0_PWD);
   #endif
     vRegWriteFldAlign(VFE_E2, 0, RG_INMUX_PWD);
     vRegWriteFldAlign(VFE_E2, 0, RG_PGAA_PWD);
     vRegWriteFldAlign(VFE_E2, 0, RG_PGABUFPA_PWD);
     vRegWriteFldAlign(VFE_E2, 0, RG_PGABUFNA_PWD);
     vRegWriteFldAlign(VFE_E2, 0, RG_AAFA_PWD);
     vRegWriteFldAlign(VFE_E2, 0, RG_CTCLAMPA_PWD);
     vRegWriteFldAlign(VFE_E2, 0, RG_BUFA_PWD);
     vRegWriteFldAlign(VFE_E2, 0, RG_CTBUFA_PWD);
     vRegWriteFldAlign(VFE_E2, 0, RG_PGAB_PWD);
     vRegWriteFldAlign(VFE_E2, 0, RG_PGABUFPB_PWD);
     vRegWriteFldAlign(VFE_E2, 0, RG_PGABUFNB_PWD);
     vRegWriteFldAlign(VFE_E2, 0, RG_AAFB_PWD);
     vRegWriteFldAlign(VFE_E2, 0, RG_CTCLAMPB_PWD);
     vRegWriteFldAlign(VFE_E2, 0, RG_BUFB_PWD);
     vRegWriteFldAlign(VFE_E2, 0, RG_CTBUFB_PWD);
#ifndef __MODEL_slt__
     vRegWriteFldAlign(VFE_E8, 0, RG_REFGEN_PWD);
#endif
     vRegWriteFldAlign(VFE_E8, 0, RG_CVBS_ADC_PWD);
     vIO32WriteFldAlign(CKGEN_VPCLK_STOP, 0, FLD_TVD3D_STOP); 
#endif
}


//////////////////////////////////////////////////////////////////////
// Private Function
//////////////////////////////////////////////////////////////////////

// => vDrvADCDefaultSetting		=>5371, OK, defaulting setting by ping
/**
 * @brief The 3 ADCs for HDTV/VGA default setting
 *
 * necessary default settings of high speed ADCs
 */
void vDrvHDTVADCDefaultSetting(void)
{
	UINT8 bCh;
	UINT8 bTmpvalue,bTmpResult;

#if EEPROM_READY
      _bEEPROM_ready = 1;
#else
      _bEEPROM_ready = 0;
#endif

     if (DRVCUST_OptGet(eEFuseAutocolor))
     {     
            bTmpResult=0;
            for(bCh=0; bCh<3; bCh++)
            {
                 bTmpvalue=((IO32ReadFldAlign(0x20008674, AC_FULLDW))>>(8*bCh));
                 if((bTmpvalue>=0x3A) && (bTmpvalue<=0x66))
                 {
                      bTmpResult |= (0x01 << bCh);
                 }
            }
            _bEFUSE_AUTOCOLOR_READY = ((bTmpResult & 0x07) != 0x07) ? 0 : 1;
     }
     else
     {
          _bEFUSE_AUTOCOLOR_READY=0;
     }

     if(bApiEepromReadByte(EEP_ADC_AUTOCOLOR_OVER_RULE_EFUSE)==0xAA)
     {
        _bEFUSE_AUTOCOLOR_READY=0;
     }

#if EnableSDDSCalibration
     _bSDDSCaliState = 0;
#endif

     //vIO32WriteFldMulti(VGACFG0,P_Fld(0,FLD_RG_VMUX_PWD)|P_Fld(1,FLD_RG_DESPIKE)|P_Fld(1,FLD_RG_RGB_EN));  //5360 Michael checked
     vIO32WriteFldMulti(VGACFG0,P_Fld(0,FLD_RG_VMUX_PWD)|P_Fld(1,FLD_RG_DESPIKE));  //5388 Kal checked
	   vIO32WriteFldAlign(VGACFG2, 1, FLD_RG_AUTOBIAS_EN);
	   vIO32WriteFldAlign(SYNCFG2,  0, FLD_RG_SYNC_DESPK_EN);  //5360 Michael checked
	   vIO32WriteFldAlign(SYNCFG2, 0, FLD_RG_SOGY_SORS_PWD);//power on
 	   vIO32WriteFldAlign(SYNCFG2, 0, FLD_RG_SOGY_SINK_PWD);//power on

       vRegWriteFldAlign(ASYNC_0E, 2, AS_C_MAXMIN_FILTER);
       vRegWriteFldAlign(VFE_00, 1, SELF_EN);  
       vRegWriteFldAlign(VFE_00, 0, BG_PWD); 
       vUtDelay1ms(2);

	for (bCh=0; bCh<3; bCh++)
	{
		vRegWriteFldAlign(VFE_01+16*bCh, 0x00000A0A, VFE_01_All);   //VFE_01, VFE_05, VFE_09 //20090323 37.5uA->62.5uA for SAG
		vRegWriteFldAlign(VFE_02+16*bCh, 0x40000802, VFE_02_All);   //VFE_02, VFE_06, VFE_0A //0x40000804 reduce PGA bias current from 50uA to 25uA
		vRegWriteFldAlign(VFE_04+16*bCh, 0x00410000, VFE_04_All);	//VFE_04, VFE_08, VFE_0C // enable AG Buffer for MT5387_88 and set AG 600mV(recommended)
	}
       vIO32WriteFldAlign(SYNCFG3,0xB330,FLD_RG_SYNC_REV);	
       vRegWriteFldAlign(VFE_03,0x40306503, VFE_03_All); // MT5387 Adjust ADC refp/n 0x40006503
       vRegWriteFldAlign(VFE_07,0x00302503, VFE_07_All); // MT5387 Adjust ADC refp/n 0x00002503  //0x00000503  ADC bias current 50uA
       vRegWriteFldAlign(VFE_0B,0x00302503, VFE_0B_All); // MT5387 Adjust ADC refp/n 0x00002503 //0x00000503

       vUtDelay1ms(2);
       vRegWriteFldAlign(VFE_00, 0, SELF_EN);
       vRegWriteFldAlign(VFE_00,0x12001800 , VFE_00_All);  //[SA7_Kal] 20090324 Band gap=710mv for MT5387_88 
#if 1	
// Slove VGA ADC no data
	vRegWriteFldAlign(VFE_03, 0, CLR_1);vUtDelay1ms(1);
	vRegWriteFldAlign(VFE_03, 1, CLR_1);vUtDelay1ms(1);
	vRegWriteFldAlign(VFE_03, 0, CLR_1);vUtDelay1ms(1);
// Slove VGA ADC no data
#endif


	vRegWriteFldAlign(HDFE_00, 0x07, CLAMP_MID_EN);

    vDrvClkInit(); //init for adcpll clk 
    
    for(bCh=0; bCh<4; bCh++ )   // to clear the control variables of loading PGA gain and offset initial values.
    {
        _bPGAGainOffsetHistory[bCh]=0;
        _bOffsetCaliDone[bCh]=0;
    }
}

//CKGEN_WRITE32(0x150, (CKGEN_READ32(0x150) & 0xFFFFFFEF));// power on ADCPLL2
//vIO32WriteFldAlign(CKGEN_ADCPLLCFG0, 0, FLD_ADCPLL2_PWD); // 5360 NEED TO CHECK

//-- Michael : ------------------------------------------------------------------------------------------ 
//--    1. Set the default gain and offset value to EEPROM if the system never do any gain and offset calibration --------
//--    2. These values can be used for vDrvSetHDTVADC()                                      --------
//--    3. We can save 4 EEPROM location and simplify the program flow of  the vDrvSetHDTVADC --------
//--    4. The 4 EEPROM locations are                                                         --------
//--        EEP_VIDEO_AUTO_COLOR_VGA_DONE                                                     --------
//--        EEP_VIDEO_AUTO_COLOR_HDTV_DONE                                                    --------
//--        EEP_VIDEO_AUTO_COLOR_SCART_DONE                                                   --------
//--        EEP_VIDEO_AUTO_COLOR_VGA_COMP_DONE                                                --------
//----------------------------------------------------------------------------------------------------

PRIVATE void vDrvEFUSEAutoColorGainSetToNextType(UINT8 bOrgType, UINT8 bNextType)
{
   UINT8 bOrder,bGain;
   UINT16 wDen,wNum;
   UINT32 dGain;

   for(bOrder=0; bOrder<3; bOrder++)
   {
      dGain = _bAutoColorGain[bOrgType][bOrder];
      wNum  = wOnChipColorGainTable[bOrgType][bOrder];
      wDen  = wOnChipColorGainTable[bNextType][bOrder];
      dGain = ((0xff + dGain)* wDen) - (0xff * wNum);
      bGain = (dGain + (wNum>>1)) /wNum ;  //round off
      _bAutoColorGain[bNextType][bOrder] = bGain;
  }
}


void vSetPGAGainOffsetInitVal(UINT8 bType)
{
UINT8 bCh,i,j;
#if EEPROM_READY
     #if EEPROM_USAGE_debug
           HAL_TIME_T rHDTVADCTime;
           HAL_GetTime(&rHDTVADCTime);
           Printf("vDrvHDTVADCDefaultSetting start time=%d  %d\n",rHDTVADCTime.u4Seconds, rHDTVADCTime.u4Micros);   
    #endif
#endif 

 if(_bPGAGainOffsetHistory[bType]==0) //This means the gain and offset were never given values.
 {
     Printf("To set the PGA gain and offset \n");
    _bPGAGainOffsetHistory[bType]=1;

    if (_bEFUSE_AUTOCOLOR_READY==1)//kalcheng   eEFuseAutocolor
    {
             _bAutoColorHistory[bType] = SV_TRUE;
             //////////////////////////////  1. Read Offset From code table   ////////////////////////////////////////////
             for(bCh=0; bCh<3; bCh++)
             {
                  if(DRVCUST_OptGet(eVFE_HW_075ohm))
                  {
                          _bAutoColorOffset[bType][bCh]= bVGADefaultOffsetTABLE_75_100[bType][bCh];
                  }
                  else
                  {
                      if(DRVCUST_OptGet(eYPBPR_ADC_SUPPORT_120))
                      {
                              _bAutoColorOffset[bType][bCh]= bVGADefaultOffsetTABLE_120[bType][bCh];
                      }
                      else
                      {
                              _bAutoColorOffset[bType][bCh]= bVGADefaultOffsetTABLE_100[bType][bCh];
                      }
                 }
             }
             //////////////////////////////  2. Read Gain From E Fuse   ///////////////////////////////////////////////

             for(bCh=0; bCh<3; bCh++)
             {  //compel Read VGA E Fuse address 0x20008674
                 _bAutoColorGain[1][bCh] = ((IO32ReadFldAlign(0x20008674, AC_FULLDW))>>(8*bCh));//+bEFuseCompensation[bCh];
                 _bAutoColorGain[3][bCh] = _bAutoColorGain[1][bCh]; //kaltst add
             }
             /////////////////////////////   3. Transfer to  YPbPr, SCART, VGA_YPbPr  ///////////////////////////////////
    
             if(DRVCUST_OptGet(eVFE_HW_SPECIAL_TARGET))
             {
                 for(i=0;i<4;i++)
   	               for(j=0;j<3;j++)
                      {
                            wOnChipColorGainTable_Temp[i][j] = wOnChipColorGainTable[i][j];
   	               }
                       vDrvGeneralAutoColorGainSetToNextType((UINT16 *)wOnChipColorGainTable_Temp,INT_VGA_COMPOENT,INT_VGA);//_bType 3 maps to 1
                       vDrvGeneralAutoColorGainSetToNextType((UINT16 *)wOnChipColorGainTable_Temp,INT_VGA,INT_HDTV);//_bType 1 maps to 0
                       vDrvGeneralAutoColorGainSetToNextType((UINT16 *)wOnChipColorGainTable_Temp,INT_VGA,INT_SCART);//_bType 1 maps to 2
             }
             else
             {
                vDrvEFUSEAutoColorGainSetToNextType(INT_VGA,INT_HDTV);
                vDrvEFUSEAutoColorGainSetToNextType(INT_VGA,INT_SCART);
                vDrvEFUSEAutoColorGainSetToNextType(INT_VGA,INT_VGA_COMPOENT);
             }
             //////////////////////////////  4. add compensation gain ///////////////////////////////////////////////
             //for CLI command n.vga.ebgain 0 or 1 read; kal add
             for(i=0;i<4;i++)
   	           for(j=0;j<3;j++)
               {
                 _bAutoColorGain[i][j] = _bAutoColorGain[i][j] + bEFuseCompensation[i][j];
                 _bAutoColorGain_for_EFuse[i][j] = _bAutoColorGain[i][j];
   	           }
             /////////////////////////////   5. Check Embedded gain  ///////////////////////////////////

             #ifdef __MODEL_slt__
			
		       if(((IO32ReadFldAlign(0x20008674, AC_FULLDW))& 0x00FFFFFF)==0x00000000)
		       { 
		         vRegWriteFldAlign(OUTSTG_OS_29, 1, OS_29_PATGEN_EN);
		   	     Printf("==== E Fuse Auto color fail(empty) !!!!  ===\n");
		       }
             #endif
    }
    else if (_bEEPROM_ready==1)
    {
    #if EEPROM_USAGE_debug
         Printf("=== EEPROM is READY, _bEEPROM_ready=%u\n",_bEEPROM_ready );
    #endif
        if (bApiEepromReadByte(EEP_VIDEO_AUTO_COLOR_HDTV_DONE+bType) != SV_TRUE) //never do auto color
        {
             _bAutoColorHistory[bType] = SV_FALSE;
             #if EEPROM_USAGE_debug
             Printf("=== EEPROM is not done, Type=%u, _bAutoColorHistory=%u\n",bType,_bAutoColorHistory[bType] );
             #endif
             for(bCh=0; bCh<3; bCh++)
             {
                  if(DRVCUST_OptGet(eVFE_HW_075ohm))
                  {
                          UNUSED(fgApiEepromWriteByte((EEP_VIDEO_AUTO_COLOR_START + (bType*3) + bCh), bVGADefaultGainTABLE_75_100[bType][bCh]));
                          UNUSED(fgApiEepromWriteByte((EEP_ADC_OFFSET_START + (bType*3) + bCh), bVGADefaultOffsetTABLE_75_100[bType][bCh]));
                          _bAutoColorGain[bType][bCh] = bVGADefaultGainTABLE_75_100[bType][bCh];
                          _bAutoColorOffset[bType][bCh]= bVGADefaultOffsetTABLE_75_100[bType][bCh];
                  }
                  else
                  {
                      if(DRVCUST_OptGet(eYPBPR_ADC_SUPPORT_120))
                      {
                              UNUSED(fgApiEepromWriteByte((EEP_VIDEO_AUTO_COLOR_START + (bType*3) + bCh), bVGADefaultGainTABLE_120[bType][bCh]));
                              UNUSED(fgApiEepromWriteByte((EEP_ADC_OFFSET_START + (bType*3) + bCh), bVGADefaultOffsetTABLE_120[bType][bCh]));
                              _bAutoColorGain[bType][bCh] = bVGADefaultGainTABLE_120[bType][bCh];
                              _bAutoColorOffset[bType][bCh]= bVGADefaultOffsetTABLE_120[bType][bCh];
                      }
                      else
                      {
                              UNUSED(fgApiEepromWriteByte((EEP_VIDEO_AUTO_COLOR_START + (bType*3) + bCh), bVGADefaultGainTABLE_100[bType][bCh]));
                              UNUSED(fgApiEepromWriteByte((EEP_ADC_OFFSET_START + (bType*3) + bCh), bVGADefaultOffsetTABLE_100[bType][bCh]));
                              _bAutoColorGain[bType][bCh] = bVGADefaultGainTABLE_100[bType][bCh];
                              _bAutoColorOffset[bType][bCh]= bVGADefaultOffsetTABLE_100[bType][bCh];
                      }
                 }
             }
        }
        else
        {
             _bAutoColorHistory[bType] = SV_TRUE;
             #if EEPROM_USAGE_debug 
             Printf("=== EEPROM is done, Type=%u, _bAutoColorHistory=%u\n",bType,_bAutoColorHistory[bType] );
             #endif
             for(bCh=0; bCh<3; bCh++)
             {
                    _bAutoColorGain[bType][bCh]= bApiEepromReadByte((EEP_VIDEO_AUTO_COLOR_START + (bType*3) + bCh));
                    _bAutoColorOffset[bType][bCh]= bApiEepromReadByte((EEP_ADC_OFFSET_START + (bType*3) + bCh));
             }
        }
    }
    else
    {
    #if EEPROM_USAGE_debug    
    Printf("=== EEPROM is NOT READY, _bEEPROM_ready=%u\n",_bEEPROM_ready );
    #endif
        for(bCh=0; bCh<3; bCh++)
        {
            if(DRVCUST_OptGet(eVFE_HW_075ohm))
            {
                   _bAutoColorGain[bType][bCh]    = bVGADefaultGainTABLE_75_100[bType][bCh];
                   _bAutoColorOffset[bType][bCh]  = bVGADefaultOffsetTABLE_75_100[bType][bCh];        
            }
            else
            {
                  if(DRVCUST_OptGet(eYPBPR_ADC_SUPPORT_120))
                  {
                   _bAutoColorGain[bType][bCh]    = bVGADefaultGainTABLE_120[bType][bCh];
                   _bAutoColorOffset[bType][bCh]  = bVGADefaultOffsetTABLE_120[bType][bCh];        
                  }
                  else
                  {
                   _bAutoColorGain[bType][bCh]    = bVGADefaultGainTABLE_100[bType][bCh];
                   _bAutoColorOffset[bType][bCh]  = bVGADefaultOffsetTABLE_100[bType][bCh];        
                  }
            }
       }
         _bAutoColorHistory[bType] = SV_FALSE;
    }         

  #if EEPROM_READY
       #if EEPROM_USAGE_debug

       HAL_GetTime(&rHDTVADCTime);
       Printf("vDrvHDTVADCDefaultSetting end time=%d  %d\n",rHDTVADCTime.u4Seconds, rHDTVADCTime.u4Micros);   

       UINT8 bTmpEEPROMdata[3];
       for(bCh=0; bCh<3; bCh++)
       {
              bTmpEEPROMdata[bCh] = bApiEepromReadByte((EEP_VIDEO_AUTO_COLOR_START + (bType*3) + bCh));
       }
       Printf("after Type=%u  ,Gain1  =0x%x ,Gain2  =0x%x ,Gain3  =0x%x \n",bType,bTmpEEPROMdata[0],bTmpEEPROMdata[1],bTmpEEPROMdata[2]);
       #endif
  #endif
  }
  else
  {
  #if EEPROM_USAGE_debug
       Printf("The PGA gain and offset were given\n");
  #endif
  }
  #if EEPROM_READY
    #if EEPROM_USAGE_debug       
       Printf("Type   =%u, autocolor ready=%u\n",bType,_bAutoColorHistory[bType]);
       Printf("Gain1  =0x%x ,Gain2  =0x%x ,Gain3  =0x%x \n",_bAutoColorGain[bType][0],_bAutoColorGain[bType][1],_bAutoColorGain[bType][2]);
       Printf("Offset1=0x%x ,Offset2=0x%x ,Offset3=0x%x \n",_bAutoColorOffset[bType][0],_bAutoColorOffset[bType][1],_bAutoColorOffset[bType][2]);
    #endif
  #endif
  
}

//////////////////////////////////////////////////////////////////////
// Public Function
//////////////////////////////////////////////////////////////////////


// => bApiVideoMainSubSrc
/**
 * @brief Set the HDTV ADCs setting by different input types
 *
 * Set the HDTV ADCs by different input types, like set the clamp methods, correct PGA offset and gain settings
 * @param bVDOINPUT bVDOINPUT is the input types, like VGA/SCART/HDTV
 */
void vDrvSetHDTVADC(UINT8 bVDOINPUT)
{
  UINT8 bInputType,bLoopCnt;	
	
       vIO32WriteFldMulti(CKGEN_VPCLK_STOP,P_Fld(0,FLD_PIP_CLK_STOP)|P_Fld(0,FLD_MAIN_CLK_STOP)|P_Fld(0,FLD_VGA_CLK_STOP)|P_Fld(0,FLD_HDTV_CLK_STOP));// 
       #ifdef CC_MT5363
       vIO32WriteFldAlign(SYNCFG0, 0xE8884F58,FLD_RG_SYNCFG0_All);
       #else
       vIO32WriteFldAlign(SYNCFG0, 0xE8884E58,FLD_RG_SYNCFG0_All);  // Clamp level 0xE8884C58 // 0xE8884D58
       #endif
       vIO32WriteFldAlign(SYNCFG1, 0x69C47044,FLD_RG_SYNCFG1_All);  // add sync path clamp current 0x69C47044 // add LPF for SYNC Path 0x69C15044 // 0x88C15044 // 0x88c4c044
       #ifndef CC_VGA_HVSYNC_SUPPORT_LOWLEVEL_2P5V // for  Dell Optiplex 745 2.62*0.6=1.572 to slicer
       vIO32WriteFldMulti(SYNCFG2, P_Fld(1,FLD_RG_VSYNC_EN)|P_Fld(8,FLD_RG_SYNC_VLSEL)|P_Fld(8,FLD_RG_SYNC_VHSEL)|P_Fld(0,FLD_RG_SYNC_TSTSEL)|P_Fld(0,FLD_RG_SYNC_DESPK_EN)
          |P_Fld(0,FLD_RG_SYNC_PWD)|P_Fld(1,FLD_RG_SYNC_TST_PWD)|P_Fld(1,FLD_RG_FB_PWD));  // [SA7_Nick] 080520 revised, power down despike  // vIO32WriteFldAlign(SYNCFG2, 0x88801011,SYNCFG2_All);  //5360 Michael checked
       #else
       vIO32WriteFldMulti(SYNCFG2, P_Fld(1,FLD_RG_VSYNC_EN)|P_Fld(4,FLD_RG_SYNC_VLSEL)|P_Fld(4,FLD_RG_SYNC_VHSEL)|P_Fld(0,FLD_RG_SYNC_TSTSEL)|P_Fld(0,FLD_RG_SYNC_DESPK_EN)
          |P_Fld(0,FLD_RG_SYNC_PWD)|P_Fld(1,FLD_RG_SYNC_TST_PWD)|P_Fld(1,FLD_RG_FB_PWD));  // [SA7_Nick] 080520 revised, power down despike  // vIO32WriteFldAlign(SYNCFG2, 0x88801011,SYNCFG2_All);  //5360 Michael checked
       #endif
       vIO32WriteFldAlign(VGACFG2, 0x2   , FLD_RG_SOGY_SINK_2_0);  // FLD_RG_SOGY_SINK=0.625 uA = 10*0.0625
       vDrvVoDDDSRST(1);


  //Set clamp control mode
  for(bLoopCnt=0; bLoopCnt<3; bLoopCnt++)
  {
    vRegWriteFldAlign(VFE_02+(bLoopCnt<<4), 0, DCLAMP_EN_1);      //disable digital clamp of CH 1/2/3
    vRegWriteFldAlign(VFE_01+(bLoopCnt<<4), 0, CLAMP_SOURCE_OFF_1);
    vRegWriteFldAlign(VFE_01+(bLoopCnt<<4), 0, CLAMP_SINK_OFF_1); 	
  }
  
  vRegWriteFldAlign(HDFE_02, 0x00, AD1_OFFSET);  //set digital offset =1 for CH 1
  vRegWriteFldAlign(HDFE_02, 0x00, AD2_OFFSET);	 //set digital offset =1 for CH 2 
  vRegWriteFldAlign(HDFE_00, 0x00, AD3_OFFSET);  //set digital offset =1 for CH 3

    // 1. Enable Mapping of Channel 1
    vRegWriteFldAlign(HDFE_00, 0x0F, IDX1_COMP_EN); 	//[SA7_Michael] enable mapping table for CH1,2,3

    // 2. Set the Mapping Gain fot ADC1~3
    vRegWriteFldAlign(HDFE_01, 0x8000, AD1_GAIN);
    vRegWriteFldAlign(HDFE_01, 0x8000, AD2_GAIN);	
    vRegWriteFldAlign(HDFE_02, 0x8000, AD3_GAIN);

  switch (bVDOINPUT)
  {
    case P_YP0:   //HDTV
    case P_YP1:
      vRegWriteFldAlign(VFE_01, 0, CLAMP_MIDDLE_1);
      vRegWriteFldAlign(VFE_05, 1, CLAMP_MIDDLE_2);
      vRegWriteFldAlign(VFE_09, 1, CLAMP_MIDDLE_3);		
      bInputType = 0;
      break;
    case P_VGA:
      vRegWriteFldAlign(VFE_01, 0, CLAMP_MIDDLE_1);
	  vRegWriteFldAlign(VFE_05, 0, CLAMP_MIDDLE_2);
      vRegWriteFldAlign(VFE_09, 0, CLAMP_MIDDLE_3);
	  bInputType = 1;
      break;

    case P_FB0:   //SCART0
    case P_FB1:   //SCART1
      vRegWriteFldAlign(VFE_01, 0, CLAMP_MIDDLE_1);
      vRegWriteFldAlign(VFE_05, 0, CLAMP_MIDDLE_2);
      vRegWriteFldAlign(VFE_09, 0, CLAMP_MIDDLE_3);
	  bInputType = 2;
      break;

    case P_VGACOMP:
      vRegWriteFldAlign(VFE_01, 1, CLAMP_MIDDLE_1);
      vRegWriteFldAlign(VFE_05, 0, CLAMP_MIDDLE_2);
      vRegWriteFldAlign(VFE_09, 1, CLAMP_MIDDLE_3);
      bInputType = 3;
      break;    

    default:
      bInputType= 0xFF;
      break;
  }

  //Load PGA gain and offset

  if(bInputType != 0xFF)
  {
      if(bInputType==INT_VGA)
      {
         vSetPGAGainOffsetInitVal(INT_SCART);  //We use VGA signal to simulate SCART signal to do auto color. So we have to set the related variables.
      }
      vSetPGAGainOffsetInitVal(bInputType);
      for(bLoopCnt=0; bLoopCnt<3; bLoopCnt++)
      {
          vRegWriteFldAlign(VFE_02+(0x10*bLoopCnt), _bAutoColorGain[bInputType][bLoopCnt], PGAGAIN_1);      //PGAGAIN_1,PGAGAIN_2,PGAGAIN_3
          vRegWriteFldAlign(VFE_02+(0x10*bLoopCnt), _bAutoColorOffset[bInputType][bLoopCnt], PGAOFFSET_1);  //PGAOFFSET_1,PGAOFFSET_2,PGAOFFSET_3
      }
       vDrvHDTVMeasureSetting(UPDATE_PER_1_FIELD);
       vDrvHDTVClampMethodDefaultSetting(bInputType);
  }

	//Set RGB front clamp-control and LPF
	if ((bVDOINPUT == (UINT8)P_FB0)||(bVDOINPUT == (UINT8)P_FB1))
	{
		vRegWriteFldAlign(VSRC_01, CLAMP_OF_TVD3D, VSRC_MUX9);
		vRegWriteFldAlign(VSRC_01, CLAMP_OF_TVD3D, VSRC_MUX8);
		vRegWriteFldAlign(VSRC_01, CLAMP_OF_TVD3D, VSRC_MUX7);
	}
	else
	{
		vRegWriteFldAlign(VSRC_01, CLAMP_OF_HDTV, VSRC_MUX9);
		vRegWriteFldAlign(VSRC_01, CLAMP_OF_HDTV, VSRC_MUX8);
		vRegWriteFldAlign(VSRC_01, CLAMP_OF_HDTV, VSRC_MUX7);
	}
}

// => bApiVideoMainSubSrc
/**
 * @brief Set the HDTV ADCs input muxes
 *
 * Set the HDTV ADCs input muxes by different input types
 * @param bVDOINPUT bVDOINPUT is the input types, like VGA/SCART/HDTV
 */
 
void vDrvSetHDTVMux(UINT8 bVDOINPUT)
{
//	UINT8 bCh;
#if 1	
// Slove VGA ADC no data
	vRegWriteFldAlign(VFE_03, 0, CLR_1);vUtDelay1ms(1);
	vRegWriteFldAlign(VFE_03, 1, CLR_1);vUtDelay1ms(1);
	vRegWriteFldAlign(VFE_03, 0, CLR_1);vUtDelay1ms(1);
// Slove VGA ADC no data
#endif


	switch (bVDOINPUT)
	{	
		case P_YP1:
		//case P_FB0:
		//case P_FB1:
			vIO32WriteFldAlign(VGACFG0, VGAMUX_HDTV1_EN, FLD_RGBHDTV01_EN);	//Set RGB Mux YPbPb1
			vIO32WriteFldAlign(SYNCFG2, 2, FLD_SOY1_SOY0_EN);	                            //Enable SOY1, disable SOY0
        		vIO32WriteFldAlign(SYNCFG2, 0, FLD_RG_SOG_EN);	                                   //disable  SOG
			break;
		case P_YP0:
			vIO32WriteFldAlign(VGACFG0, VGAMUX_HDTV0_EN, FLD_RGBHDTV01_EN);	//Set RGB Mux YPbPb0			
			vIO32WriteFldAlign(SYNCFG2, 1, FLD_SOY1_SOY0_EN);	                            //Enable SOY0, disable SOY1
        		vIO32WriteFldAlign(SYNCFG2, 0, FLD_RG_SOG_EN);	                                   //disable  SOG
			break;
		case P_VGA:
		case P_VGACOMP:	
			vIO32WriteFldAlign(VGACFG0, VGAMUX_RGB_EN, FLD_RGBHDTV01_EN);		//Set RGB Mux RGB			
			vIO32WriteFldAlign(SYNCFG2, 0, FLD_SOY1_SOY0_EN);	                            //Disable SOY0, disable SOY1
			vIO32WriteFldAlign(SYNCFG2, 1, FLD_RG_SOG_EN);	                                   //Enable SOG
			break;
		default:
			assert(0);
	}

}

//to control the SOY0, SOY1, SOGY ; 1:enable, 0:disable
void vDrvSOY0EN(UINT8 bEn) 
{
  if (bEn == 1) //Enable SOY0
  {
      vIO32WriteFldAlign(SYNCFG2, 1, FLD_RG_SOY0_EN);	                            
  }
  else //Disable SOY1
  {      
	vIO32WriteFldAlign(SYNCFG2, 0, FLD_RG_SOY0_EN);	 
  }
}

void vDrvSOY1EN(UINT8 bEn) 
{
  if (bEn == 1) //Enable SOY1
  {
      vIO32WriteFldAlign(SYNCFG2, 1, FLD_RG_SOY1_EN);	                          
  }
  else //Disable SOY1
  {
      vIO32WriteFldAlign(SYNCFG2, 0, FLD_RG_SOY1_EN);	                           
  }
}

void vDrvSOGEN(UINT8 bEn) 
{
  if (bEn == 1) //Enable SOG
  {
      vIO32WriteFldAlign(SYNCFG2, 1, FLD_RG_SOG_EN);	                          
  }
  else //Disable SOG
  {
      vIO32WriteFldAlign(SYNCFG2, 0, FLD_RG_SOG_EN);	                          
  }
}


// => bApiVideoMainSubSrc
/**
 * @brief Set all the HDTV ADCs on / off
 *
 * Set all the HDTV ADCs on / off
 * @param bPow bPow is the on / off selection (SV_TRUE, SV_FALSE)
 */
void vDrvAllHDADCPow(UINT8 bPow)
{
	UINT8 bCh;
	if (bPow == SV_TRUE)//Power on
	{
#if PD_ADC_ENABLE
              vRegWriteFldAlign(VFE_00, 1, SELF_EN);  
		vRegWriteFldAlign(VFE_00,0,BG_PWD);
              vUtDelay1ms(2);
		for (bCh=0; bCh<3; bCh++)
		{
               vRegWriteFldAlign(VFE_01+(bCh<<4), 0, CLAMP_1_PWD);
               vRegWriteFldAlign(VFE_01+(bCh<<4), 0, CLAMP_SOURCE_OFF_1);		
		 vRegWriteFldAlign(VFE_01+(bCh<<4), 0, CLAMP_SINK_OFF_1);		
		 vRegWriteFldAlign(VFE_01+(bCh<<4), 0, PGA_1_PWD);		
		 vRegWriteFldAlign(VFE_03+(bCh<<4), 0, ADC_1_PWD);	
		 vRegWriteFldAlign(VFE_03+(bCh<<4), 0, ADCCK_1_PWD);
		}
              vUtDelay1ms(2);
              vRegWriteFldAlign(VFE_00, 0, SELF_EN);  
		
#endif 
    vRegWriteFldAlign(HDTV_03, 1, HDTV_EN);
    vDrvAllADCPLLPow(SV_TRUE);
	}
	else if (bPow == SV_FALSE)//Power down
	{
#if PD_ADC_ENABLE
		vRegWriteFldAlign(VFE_00,1,BG_PWD);
		for (bCh=0; bCh<3; bCh++)
		{
		 vRegWriteFldAlign(VFE_01,1,CLAMP_1_PWD);
		 vRegWriteFldAlign(VFE_01+(bCh<<4),1,CLAMP_SOURCE_OFF_1);	   
		 vRegWriteFldAlign(VFE_01+(bCh<<4),1,CLAMP_SINK_OFF_1); 	   
 		 vRegWriteFldAlign(VFE_01+(bCh<<4),1,PGA_1_PWD);	   
		 vRegWriteFldAlign(VFE_03+(bCh<<4),1,ADC_1_PWD);			   
		 //vRegWriteFldAlign(VFE_03+(bCh<<4), 1, ADCCK_1_PWD);
		}
#endif 
    vRegWriteFldAlign(HDTV_03, 0, HDTV_EN);
    vDrvAllADCPLLPow(SV_FALSE);    
	}
}

// => _PdwncSetupVGA
/**
 * @brief Set all the vDrvSYNCSlicerWakeup on / off
 *
 * Set all the vDrvSYNCSlicerWakeup on / off
 * @param bPow bPow is the on / off selection (SV_TRUE, SV_FALSE)
 */
void vDrvSYNCSlicerWakeup(UINT8 bPow)
{
	if (bPow == SV_TRUE) // for Wakeup setting
	{
	 vIO32WriteFldMulti(SYNCFG0, P_Fld(0,FLD_RG_SYNC_BG_PWD)|P_Fld(1,FLD_RG_HSYNC_EN));
     vIO32WriteFldMulti(SYNCFG2, P_Fld(1,FLD_RG_VSYNC_EN)|P_Fld(1,FLD_RG_SOG_EN)|P_Fld(0,FLD_RG_SOGY_SORS_PWD)|P_Fld(0,FLD_RG_SOGY_SINK_PWD));
     Printf("[SA7]for Wakeup setting \n");
	}
	else if (bPow == SV_FALSE)//Power down setting
	{
     vIO32WriteFldMulti(SYNCFG0, P_Fld(1,FLD_RG_SYNC_BG_PWD)|P_Fld(0,FLD_RG_HSYNC_EN));
     vIO32WriteFldMulti(SYNCFG2, P_Fld(0,FLD_RG_VSYNC_EN)|P_Fld(1,FLD_RG_SOG_EN)|P_Fld(1,FLD_RG_SOGY_SORS_PWD)|P_Fld(1,FLD_RG_SOGY_SINK_PWD));
     Printf("[SA7]for Power down setting \n");     
	}
}



//New function for manual gain/offset adjustment

/**
 * @brief  save RGB_offset to EEPROM
 *
 * @param None
 */
void vCust_R_Offset_Update(UINT8 bOffset)
{
UINT8 bIntputType,bType;
    bIntputType   = bGetICInputType(SV_VP_MAIN);        //MC20080417_1 ***
    bType  = bDrvOnChipGetVFESignalType(bIntputType);  //to get Signal Type
    if(bType!=P_FA)
    {
       _bAutoColorOffset[bType][2] = bOffset;
    if((bType==INT_SCART) && (fgIsCLKLock()))
    {
	UNUSED(fgApiEepromWriteByte(EEP_ADC_OFFSET_SCART_CLAMP2 ,bOffset));
    }
    else
    {
	if ( fgIsMainVga()||fgIsPipVga() )
	{
		UNUSED(fgApiEepromWriteByte(EEP_ADC_OFFSET_VGA_CLAMP2,bOffset));
	}
	else if ( fgIsMainYPbPr()||fgIsPipYPbPr() )
	{
		UNUSED(fgApiEepromWriteByte(EEP_ADC_OFFSET_HDTV_CLAMP2,bOffset));
	}
	//UNUSED(fgApiEepromWriteByte((fgIsMainVga()||fgIsPipVga()) ?  EEP_VIDEO_AUTO_COLOR_VGA_DONE : EEP_VIDEO_AUTO_COLOR_HDTV_DONE, SV_TRUE));
}
}
}
void vCust_G_Offset_Update(UINT8 bOffset)
{
UINT8 bIntputType,bType;
    bIntputType   = bGetICInputType(SV_VP_MAIN);        //MC20080417_1 ***
    bType  = bDrvOnChipGetVFESignalType(bIntputType);  //to get Signal Type
    if(bType!=P_FA)
    {
        _bAutoColorOffset[bType][1] = bOffset;
    if((bType==INT_SCART) && (fgIsCLKLock()))
    {
	UNUSED(fgApiEepromWriteByte(EEP_ADC_OFFSET_SCART_CLAMP1 ,bOffset));
    }
    else
    {
	if ( fgIsMainVga()||fgIsPipVga() )
	{
		UNUSED(fgApiEepromWriteByte(EEP_ADC_OFFSET_VGA_CLAMP1,bOffset));
	}
	else if ( fgIsMainYPbPr()||fgIsPipYPbPr() )
	{
		UNUSED(fgApiEepromWriteByte(EEP_ADC_OFFSET_HDTV_CLAMP1,bOffset));
	}
	//UNUSED(fgApiEepromWriteByte((fgIsMainVga()||fgIsPipVga()) ?       EEP_VIDEO_AUTO_COLOR_VGA_DONE : EEP_VIDEO_AUTO_COLOR_HDTV_DONE, SV_TRUE));
}
}
}

void vCust_B_Offset_Update(UINT8 bOffset)
{
UINT8 bIntputType,bType;
    bIntputType   = bGetICInputType(SV_VP_MAIN);        //MC20080417_1 ***
    bType  = bDrvOnChipGetVFESignalType(bIntputType);  //to get Signal Type
   if(bType!=P_FA)
    {
        _bAutoColorOffset[bType][0] = bOffset;
    if((bType==INT_SCART) && (fgIsCLKLock()))
    {
	UNUSED(fgApiEepromWriteByte(EEP_ADC_OFFSET_SCART_CLAMP0 ,bOffset));
    }
    else
    {
	if ( fgIsMainVga()||fgIsPipVga() )
	{
		UNUSED(fgApiEepromWriteByte(EEP_ADC_OFFSET_VGA_CLAMP0,bOffset));
	}
	else if ( fgIsMainYPbPr()||fgIsPipYPbPr() )
	{
		UNUSED(fgApiEepromWriteByte(EEP_ADC_OFFSET_HDTV_CLAMP0,bOffset));
	}
	//UNUSED(fgApiEepromWriteByte((fgIsMainVga()||fgIsPipVga()) ?       EEP_VIDEO_AUTO_COLOR_VGA_DONE : EEP_VIDEO_AUTO_COLOR_HDTV_DONE, SV_TRUE));
}
}
}



/**
 * @brief  save RGB_Gain to EEPROM
 *
 * @param None
 */
void vCust_R_Gain_Update(UINT8 bGain)
{
UINT8 bIntputType,bType;
    bIntputType   = bGetICInputType(SV_VP_MAIN);        
    bType  = bDrvOnChipGetVFESignalType(bIntputType);  //to get Signal Type
    if(bType!=P_FA)
    {
         _bAutoColorGain[bType][2]=bGain;
    if((bType==INT_SCART) && (fgIsCLKLock()))
    {
	UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_SCART_GAIN2 ,bGain));
    }
    else
    {
	if ( fgIsMainVga()||fgIsPipVga() )
	{
		UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_VGA_GAIN2 ,bGain));
	}
	else if ( fgIsMainYPbPr()||fgIsPipYPbPr() )
	{
		     UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_HDTV_GAIN2,bGain));
	     }
	     //UNUSED(fgApiEepromWriteByte((fgIsMainVga()||fgIsPipVga()) ?        EEP_VIDEO_AUTO_COLOR_VGA_DONE : EEP_VIDEO_AUTO_COLOR_HDTV_DONE, SV_TRUE));
	}
    }
}

void vCust_G_Gain_Update(UINT8 bGain)
{
UINT8 bIntputType,bType;
    bIntputType   = bGetICInputType(SV_VP_MAIN);        
    bType  = bDrvOnChipGetVFESignalType(bIntputType);  //to get Signal Type
    if(bType!=P_FA)
    {
         _bAutoColorGain[bType][1]=bGain;
    if((bType==INT_SCART) && (fgIsCLKLock()))
    {
	UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_SCART_GAIN1 ,bGain));
    }
    else
    {
	if ( fgIsMainVga()||fgIsPipVga() )
	{
		UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_VGA_GAIN1 ,bGain));
	}
	else if ( fgIsMainYPbPr()||fgIsPipYPbPr() )
	{
		     UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_HDTV_GAIN1,bGain));
	     }
	     //UNUSED(fgApiEepromWriteByte((fgIsMainVga()||fgIsPipVga()) ?        EEP_VIDEO_AUTO_COLOR_VGA_DONE : EEP_VIDEO_AUTO_COLOR_HDTV_DONE, SV_TRUE));
	}
    }
}

void vCust_B_Gain_Update(UINT8 bGain)
{
UINT8 bIntputType,bType;
    bIntputType   = bGetICInputType(SV_VP_MAIN);        
    bType  = bDrvOnChipGetVFESignalType(bIntputType);  //to get Signal Type
    if(bType!=P_FA)
    {
         _bAutoColorGain[bType][0]=bGain;
         if((bType==INT_SCART) && (fgIsCLKLock()))
         {
	    UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_SCART_GAIN0 ,bGain));
         }
         else
         {
	    if ( fgIsMainVga()||fgIsPipVga() )
	    {
		UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_VGA_GAIN0 ,bGain));
	    }
	    else if ( fgIsMainYPbPr()||fgIsPipYPbPr() )
	    {
		UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_HDTV_GAIN0,bGain));
	    }
	    //UNUSED(fgApiEepromWriteByte((fgIsMainVga()||fgIsPipVga()) ?        EEP_VIDEO_AUTO_COLOR_VGA_DONE : EEP_VIDEO_AUTO_COLOR_HDTV_DONE, SV_TRUE));
        }
    }
}

void vCust_EBGain_OverRule_Update(UINT8 bNum) //kaloverrule
{
  UNUSED(fgApiEepromWriteByte(EEP_ADC_AUTOCOLOR_OVER_RULE_EFUSE, bNum));
}

UINT8 vCust_EBGain_OverRule_Read(void) //kaloverrule   
{
	return (UINT8)bApiEepromReadByte(EEP_ADC_AUTOCOLOR_OVER_RULE_EFUSE);
}


//SOGY slicer Vth setting by customer
void vCust_SOGY_VTH_Set(UINT32 dwLevel)
{
      dwLevel = (dwLevel << 24);   
      vIO32WriteFldAlign(SYNCFG1, dwLevel,FLD_RG_SOGY_VHSEL);
}
//SOGY slicer Vtl setting by customer
void vCust_SOGY_VTL_Set(UINT32 dwLevel)
{
     dwLevel= (dwLevel << 28);
     vIO32WriteFldAlign(SYNCFG1, dwLevel,FLD_RG_SOGY_VLSEL);
}

void MTAL_SOGY_VTH_Set(UINT8 bLevel)
{
	rSlicerConfig.AP_SliceLevelManuSet=1;
	if(bLevel>0x0f)
	{
	   bLevel = 0x0f;
	}
	rSlicerConfig.AP_SVTH_ManuValue=bLevel;
	rSlicerConfig.AP_SVTLH_ManuValue=(bLevel) |(rSlicerConfig.AP_SVTL_ManuValue<<4);
    vIO32WriteFldAlign(SYNCFG1,bLevel,FLD_RG_SOGY_VHSEL);
}

void MTAL_SOGY_VTL_Set(UINT8 bLevel)
{
	rSlicerConfig.AP_SliceLevelManuSet=1;
	if(bLevel>0x0f)
	{
	   bLevel = 0x0f;
	}
	rSlicerConfig.AP_SVTL_ManuValue=bLevel;
	rSlicerConfig.AP_SVTLH_ManuValue=(bLevel<<4) |(rSlicerConfig.AP_SVTH_ManuValue);
    vIO32WriteFldAlign(SYNCFG1,bLevel,FLD_RG_SOGY_VLSEL);
}


/**
 * @brief  Read RGB_offset
 *
 * @param None
 */
UINT8 vCust_R_Offset_Read(void)
{
	return (UINT8)(RegReadFldAlign(VFE_0A, PGAOFFSET_3));
}

UINT8 vCust_G_Offset_Read(void)
{
	return (UINT8)(RegReadFldAlign(VFE_06, PGAOFFSET_2));
}

UINT8 vCust_B_Offset_Read(void)
{
	return (UINT8)(RegReadFldAlign(VFE_02, PGAOFFSET_1));
}

/**
 * @brief  Read RGB_Gain
 *
 * @param None
 */
UINT8 vCust_R_Gain_Read(void)
{
	return (UINT8)(RegReadFldAlign(VFE_0A, PGAGAIN_3));
}

UINT8 vCust_G_Gain_Read(void)
{
	return (UINT8)(RegReadFldAlign(VFE_06, PGAGAIN_2));
}

UINT8 vCust_B_Gain_Read(void)
{
	return (UINT8)(RegReadFldAlign(VFE_02, PGAGAIN_1));
}

void vCust_AutoColor_EEPROM_Enable(UINT8 bEnable)
{
    if(bEnable==0)
    {
      _bEEPROM_ready=0;
    }
    else
    {
      _bEEPROM_ready=1;
    }
}

UINT8 vCust_B_MAX_Read(void)
{
	if ( fgIsMainVga()||fgIsPipVga() )
	{
	  return (UINT8)(bApiEepromReadByte(EEP_ADC_VGA_MAX_CH0));
	}
	else if(fgIsMainYPbPr() || fgIsPipYPbPr())
	{
	  return (UINT8)(bApiEepromReadByte(EEP_ADC_HDTV_MAX_CH0));
	}
       return 0; 
}

UINT8 vCust_G_MAX_Read(void)
{
	if ( fgIsMainVga()||fgIsPipVga() )
	{
	  return (UINT8)(bApiEepromReadByte(EEP_ADC_VGA_MAX_CH1));
	}
	else if(fgIsMainYPbPr() || fgIsPipYPbPr())
	{
	  return (UINT8)(bApiEepromReadByte(EEP_ADC_HDTV_MAX_CH1));
	}
       return 0; 
}

UINT8 vCust_R_MAX_Read(void)
{
	if ( fgIsMainVga()||fgIsPipVga() )
	{
	  return (UINT8)(bApiEepromReadByte(EEP_ADC_VGA_MAX_CH2));
	}
	else
	{
	  return (UINT8)(bApiEepromReadByte(EEP_ADC_HDTV_MAX_CH2));
	}
}

//Mike modify ADC setting 20080704
void vCust_R_MAX_Update(UINT8 value)
{
	if ( fgIsMainVga()||fgIsPipVga() )
	{
	  UNUSED(fgApiEepromWriteByte(EEP_ADC_VGA_MAX_CH2,value));
	}
	else if(fgIsMainYPbPr() || fgIsPipYPbPr())
	{
	  UNUSED(fgApiEepromWriteByte(EEP_ADC_HDTV_MAX_CH2,value));
	}
}

void vCust_G_MAX_Update(UINT8 value)
{
	if ( fgIsMainVga()||fgIsPipVga() )
	{
	  UNUSED(fgApiEepromWriteByte(EEP_ADC_VGA_MAX_CH1,value));
	}
	else if(fgIsMainYPbPr() || fgIsPipYPbPr())
	{
	  UNUSED(fgApiEepromWriteByte(EEP_ADC_HDTV_MAX_CH1,value));
	}
}

void vCust_B_MAX_Update(UINT8 value)
{
	if ( fgIsMainVga()||fgIsPipVga() )
	{
	  UNUSED(fgApiEepromWriteByte(EEP_ADC_VGA_MAX_CH0,value));
	}
	else
	{
	  UNUSED(fgApiEepromWriteByte(EEP_ADC_HDTV_MAX_CH0,value));
	}
}

UINT16 vCust_B_Blank_Read(void)
{
  UINT32 dwSumADCRawData;
  UINT16 wData_v1v2;
  UINT8 bLoopCnt;
  dwSumADCRawData = 0;
  if(bDvMeasureDataSel()==1)
  {
    dwSumADCRawData = bDrvGetBlankVal(0);   //to read the blank level of Y, Pb, Pr respective
    wData_v1v2 = (dwSumADCRawData)<<3; // 10 bits to 13 bits = 8bit sum 32 times               
  }
  else
  {
    for(bLoopCnt = 0; bLoopCnt<32; bLoopCnt++)
    {
      dwSumADCRawData += bDrvGetBlankVal(0);   //to read the blank level of Y, Pb, Pr respective
	  vUtDelay2us(50);
    }
      wData_v1v2 = (dwSumADCRawData+2)>>2; // 10 bits to 8 bits // +8) >> 4; // 12 bits to 8 bits
  }
      wData_v1v2 = wData_v1v2 >>3;  //10 BIT resolution

      return (wData_v1v2);
}

UINT16 vCust_G_Blank_Read(void)
{
  UINT32 dwSumADCRawData;
  UINT16 wData_v1v2;
  UINT8 bLoopCnt;
  dwSumADCRawData = 0;
  if(bDvMeasureDataSel()==1)
  {
    dwSumADCRawData = bDrvGetBlankVal(1);   //to read the blank level of Y, Pb, Pr respective
    wData_v1v2 = (dwSumADCRawData)<<3; // 10 bits to 13 bits = 8bit sum 32 times               
  }
  else
  {
    for(bLoopCnt = 0; bLoopCnt<32; bLoopCnt++)
    {
      dwSumADCRawData += bDrvGetBlankVal(1);   //to read the blank level of Y, Pb, Pr respective
	  vUtDelay2us(50);
    }
      wData_v1v2 = (dwSumADCRawData+2)>>2; // 10 bits to 8 bits // +8) >> 4; // 12 bits to 8 bits
  }
      wData_v1v2 = wData_v1v2 >>3;  //10 BIT resolution

      return (wData_v1v2);
}

UINT16 vCust_R_Blank_Read(void)
{
  UINT32 dwSumADCRawData;
  UINT16 wData_v1v2;
  UINT8 bLoopCnt;
  dwSumADCRawData = 0;
  if(bDvMeasureDataSel()==1)
  {
    dwSumADCRawData = bDrvGetBlankVal(2);   //to read the blank level of Y, Pb, Pr respective
    wData_v1v2 = (dwSumADCRawData)<<3; // 10 bits to 13 bits = 8bit sum 32 times               
  }
  else
  {
    for(bLoopCnt = 0; bLoopCnt<32; bLoopCnt++)
    {
      dwSumADCRawData += bDrvGetBlankVal(2);   //to read the blank level of Y, Pb, Pr respective
	  vUtDelay2us(50);
    }
      wData_v1v2 = (dwSumADCRawData+2)>>2; // 10 bits to 8 bits // +8) >> 4; // 12 bits to 8 bits
  }
      wData_v1v2 = wData_v1v2 >>3;  //10 BIT resolution

      return (wData_v1v2);
}

UINT16 vCust_B_Max_Read(void)
{
      return bDrvOnChipGetADCMaxMinValue(0,AS_PHASE_MAX_SEL);
}

UINT16 vCust_G_Max_Read(void)
{
      return bDrvOnChipGetADCMaxMinValue(1,AS_PHASE_MAX_SEL);;
}

UINT16 vCust_R_Max_Read(void)
{
      return bDrvOnChipGetADCMaxMinValue(2,AS_PHASE_MAX_SEL);;
}

void vCust_clear_eeprom(UINT8 bValue)
{
  int i; 
  for(i=EEP_ADC_START; i<(EEP_VIDEO_AUTO_COLOR_END+1); i++)
  {
    UNUSED(fgApiEepromWriteByte(i ,bValue));
  }
}


#ifdef CC_COPLAT_MT82
typedef UINT32  HANDLE_T;
#define OSR_OK                    ((INT32)   0)
#endif

INT32 AD_AcquireControl(void)
{
	return OSR_OK;
}

INT32 AD_ReleaseControl(void)
{
	return OSR_OK;
}

UINT16 AD_QueryInputSource(void)
{
	// return format:  (PIP_IPS<<8)|MAIN_IPS
	//  *	current only return MAIN path for 5381
	//  *	going to support 5382p
	// 

	switch (VSS_MAJOR(_bSrcMainNew))
	{
		case VSS_ATV:
			return IPS_ATV;
		case VSS_ATD:
			return IPS_ATD;
		case VSS_CVBS:
		case VSS_SV:
		case VSS_SCART:
			return IPS_CVBS_A_B; //use 2 channel
		case VSS_DTV:
			return IPS_DTV;
		case VSS_DTD:
			return IPS_DTD;
		default:
			return IPS_DTV;
	}

}

#if defined(CC_MT5387) && !defined(CC_MT5363)
/**
 * @brief  Set RESISTOR between BUFFER and COM
 *
 * @param Sigal Type
 */

void vPGA_RESISTOR_BUFFER_COM(UINT8 bType)
{
  if (BSP_GetIcVersion() <= IC_VER_5387_AA) // Before ECO
  {
  if (bType==SYNCONGREEN)  	
  {
   vRegWriteFldAlign(VFE_04, RESISTOR_BUFFER_COM_1_0K, RESISTOR_BUFFER_COM_1);
  }
  else
  {
   vRegWriteFldAlign(VFE_04, RESISTOR_BUFFER_COM_1_50K, RESISTOR_BUFFER_COM_1);    
  }
  }
}
#endif

#if 1//SUPPORT_SCART
#define SCART_SOGY_BW 0		//0~3
#define SCART_SOGY_RGAIN 3	//0~3   // [SD3/SA7/nick] 20080407 revised MT5391 SCART RGB By External Path  // 1
#define SCART_FB_HIGH_THR 1	//0~7
#define SCART_FB_LOW_THR 1	//0~7
#define SCART_SOY_HIGH_THR 8//0~15
#define SCART_SOY_LOW_THR 8	//0~15

/*	Scart Setting for drv_scart.c		*/
// =>
/**
 * @brief Set SCART Video front end
 * Set SCART VFE
 * @param void
 */
void vScartVfeConnect(void)
{

       UINT32 ret_FB = 0;
       UINT32 ret_SCART1_SC = 0;
       UINT32 ret_SCART2_SC = 0;
       
       bINT_BST_DET = RegReadFldAlign(VFE_E4,Fld(1, 25, AC_MSKB3));
       vIO32WriteFldAlign(SYNCFG0, 1   , FLD_RG_CVBS_EN);//location changed???
	vIO32WriteFldAlign(SYNCFG2, 0   , FLD_RG_FB_PWD);
       vRegWriteFldAlign(VFE_E7, 0x01, RG_CVBS2SYNCA_EN); 

	//vRegWriteFldAlign(VFE_E7, 0x01, RG_CINRB_EN);//remove  

 #ifndef SCART_CLK_SOURCE_CVBS
	vIO32WriteFldAlign(SYNCFG1, 0x0   , FLD_RG_SOGY_SINK_5_3);
	vIO32WriteFldAlign(VGACFG2, 0x0   , FLD_RG_SOGY_SINK_2_0);	
	vIO32WriteFldAlign(SYNCFG1, 0x0   , FLD_RG_SOGY_SOURCE);
    vIO32WriteFldAlign(SYNCFG1, 0xc   , FLD_RG_SOGY_VHSEL);//for small I/P CVBS level//need to rename???
	vIO32WriteFldAlign(SYNCFG1, 0xb  , FLD_RG_SOGY_VLSEL);//for small I/P CVBS level
	vIO32WriteFldAlign(SYNCFG1, 0x1   , FLD_RG_SOGY_RGAIN);// 0x01 6x 0x00 3.5x gain = 3x 0x1
	//vIO32WriteFldAlign(SYNCFG1, 0x5  , SOGY_CLAMP);//remove, only for MT8226
	vIO32WriteFldAlign(SYNCFG1, 0x3  , FLD_RG_SOGY_BW);//BW = "11" in SCART RGB for Fluke multiburst

	vIO32WriteFldAlign(SYNCFG2, 1   , FLD_RG_SOGY_SINK_PWD);
	vIO32WriteFldAlign(SYNCFG2, 1   , FLD_RG_SOGY_SORS_PWD);
	vIO32WriteFldAlign(SYNCFG2, 0   , FLD_RG_SOY0_EN);//disable SOY0, need to check MT8222 and MT8226???
	vIO32WriteFldAlign(SYNCFG2, 0   , FLD_RG_SOY1_EN);//disable SOY1
	vIO32WriteFldAlign(SYNCFG2, 0   , FLD_RG_SOG_EN);//disable SOG
#else
    vRegWriteFldMulti(ASYNC_08, P_Fld(0,AS_MASK_SLICE_EN)|P_Fld(1,AS_VMASK1_OFF)|P_Fld(1,AS_VMASK2_OFF)|P_Fld(1,AS_VMASK3_OFF));
    vIO32WriteFldAlign(CKGEN_AFECFG7, 0x1, FLD_RG_ADCPLL_H_CVBS_SEL);
#endif	
        vIO32WriteFldAlign(SYNCFG3, 0   , FLD_RG_SCART_EN);//default disable FB

      //select FB
        if((_bMainICIn == P_FB0) || (_bSubICIn == P_FB0))//SCART1
        {
	    ret_FB = (UINT32)DRVCUST_OptGet(eSCART1FBInSelect);
        }
        else if((_bMainICIn == P_FB1) || (_bSubICIn == P_FB1))//SCART2
        {
	    ret_FB = (UINT32)DRVCUST_OptGet(eSCART2FBInSelect);
        }
       if(ret_FB == SCART_FB_IN_SOY0)
       {
           Printf("FB from SOY0\n");
           vIO32WriteFldAlign(SYNCFG3, 0x04, FLD_RG_SCART_EN);
       }
       else if(ret_FB == SCART_FB_IN_SOY1)
       {
           Printf("FB from SOY1\n");
           vIO32WriteFldAlign(SYNCFG3, 0x02, FLD_RG_SCART_EN);
       }
       else if(ret_FB == SCART_FB_IN_SOG)
       {
           Printf("FB from SOG\n");
           vIO32WriteFldAlign(SYNCFG3, 0x01, FLD_RG_SCART_EN);
       }    
	//vIO32WriteFldAlign(VGACFG2, 0x01, FLD_SC1_SCART_EN);//remove
	vRegWriteFldMulti(SVF_02, P_Fld(1,SVFBST_HDTV)|P_Fld(1,SVFBST_FIR));//enable HDTV burst detector
	vRegWriteFldAlign(HDTV_05, 0xB0, BIW_END);//hdtv burst detector window, check with MT8222
	vRegWriteFldAlign(HDTV_05, 0x50, BIW_START);
	vRegWriteFldAlign(SYS_00, 1, DATA_SYNC_INV); // For MT5387 TVD change to 54MHz issue
	vRegWriteFldAlign(SYS_00, 1, SUB_DATA_SYNC_INV); // For MT5363 Sub PIP TVD change to 54MHz issue
	
	//This setting needs to be customized.  BS will check further.
	/********************************************/
	/**/vRegWriteFldAlign(HDTV_05, 0x2, BIW_AD_SEL); /**/
	/********************************************/

	if(bINT_BST_DET==1) // Internal Path, set from source_table.c
	{            
             vRegWriteFldAlign(VFE_E7, 0x01, RG_CINRB_EN);//connect SC from VGA VFE to CVBS VFE in CVBS part             
             if((_bMainICIn == P_FB0) || (_bSubICIn == P_FB0))//SCART1
             	{
#if defined(CC_MT5387) && !defined(CC_MT5363)             	
#if SCART_SOG_WA
                if (BSP_GetIcVersion() <= IC_VER_5387_AA) // Before ECO
        		 vIO32WriteFldAlign(SYNCFG2, 1, FLD_RG_SOG_EN);	  //enable SOG
#endif
#endif
                  ret_SCART1_SC = (UINT32)DRVCUST_OptGet(eScart1IntScCh);
                  if(ret_SCART1_SC == SCART_SC_FROM_CH1)//SC from CH1
                  {
                       vIO32WriteFldAlign(VGACFG2, 0x01, FLD_RG_SC1_SCART_EN);
                  }
                  else if(ret_SCART1_SC == SCART_SC_FROM_CH3)//SC from CH1
                  {
                       vIO32WriteFldAlign(VGACFG2, 0x01, FLD_RG_SC2_SCART_EN);
                  }
              }
             else if((_bMainICIn == P_FB1) || (_bSubICIn == P_FB1))//SCART2
             {
                 ret_SCART2_SC = (UINT32)DRVCUST_OptGet(eScart2IntScCh);
                 if(ret_SCART2_SC == SCART_SC_FROM_CH1)//SC from CH1
                  {
                       vIO32WriteFldAlign(VGACFG2, 0x01, FLD_RG_SC1_SCART_EN);
                  }
                  else if(ret_SCART2_SC == SCART_SC_FROM_CH3)//SC from CH3
                  {
                       vIO32WriteFldAlign(VGACFG2, 0x01, FLD_RG_SC2_SCART_EN);
                  }
             }
             #if HDTV_BST_DET
                   vRegWriteFldMulti(SVF_02, P_Fld(1,SVFBST_HDTV)|P_Fld(1,SVFBST_FIR));//enable HDTV burst detector
			vRegWriteFldAlign(SVF_03, 0x4C, SVF_BSTDET_TH);
             #else
                   vRegWriteFldMulti(SVF_02, P_Fld(0,SVFBST_HDTV)|P_Fld(0,SVFBST_FIR));//disable HDTV burst detector
                   #if !TVD_NEW_BST_DET_TH
				   vRegWriteFldAlign(SVF_03, 0x8, SVF_BSTDET_TH);
				   #endif
             #endif    

	}
	else
	{
          vRegWriteFldMulti(SVF_02, P_Fld(0,SVFBST_HDTV)|P_Fld(0,SVFBST_FIR));//disable HDTV burst detector
          #if !TVD_NEW_BST_DET_TH
	    vRegWriteFldAlign(SVF_03, 0x8, SVF_BSTDET_TH);
		  #endif
	}
	vDrvAvByp();
}

// =>
/**
 * @brief Disconnect SCART Video front end setting
 * Set SCART
 * @param void
 */
void vScartVfeDisconnect(void)
{ 
#ifndef SCART_CLK_SOURCE_CVBS
    vIO32WriteFldAlign(SYNCFG1, 0x5   , FLD_RG_SOGY_SINK_5_3);
	vIO32WriteFldAlign(VGACFG2, 0x2   , FLD_RG_SOGY_SINK_2_0);
	vIO32WriteFldAlign(SYNCFG1, 0xc   , FLD_RG_SOGY_SOURCE);

//penggang 20090827 mark. for CR 	DTV00140033. eg:Change sub source will cause main ypbpr slicer modify here and 
//ypbpr image position move into right lightly.																																	
//	vIO32WriteFldAlign(SYNCFG1, 0x8   , FLD_RG_SOGY_VHSEL); // 0x9 for Macrovision disc set 0x8 for YPbPr
//	vIO32WriteFldAlign(SYNCFG1, 0x5   , FLD_RG_SOGY_VLSEL); // 0x6 for Macrovision disc set 0x8 for YPbPr
	vIO32WriteFldAlign(SYNCFG1, 0x3   , FLD_RG_SOGY_BW);    // 0x01 60.8MHz 0x0 905MHz

	vIO32WriteFldAlign(SYNCFG2, 0   , FLD_RG_SOGY_SINK_PWD);
	vIO32WriteFldAlign(SYNCFG2, 0   , FLD_RG_SOGY_SORS_PWD);
	vIO32WriteFldAlign(SYNCFG1, 0x1   , FLD_RG_SOGY_RGAIN);  // 0x3
	vIO32WriteFldAlign(SYNCFG0, 0   , FLD_RG_CVBS_EN);
	vIO32WriteFldAlign(SYNCFG2, 1   , FLD_RG_FB_PWD);
	vIO32WriteFldAlign(SYNCFG3, 0x00, FLD_RG_SCART_EN);
	Printf("Disconnect FB at VFE disconnect\n");
    vRegWriteFldAlign(VFE_E7, 0x00, RG_CVBS2SYNCA_EN);//disconnect CVBS to slicer for RGB sync
#endif
	vIO32WriteFldAlign(VGACFG2, 0x00, FLD_RG_SC1_SCART_EN);
	vIO32WriteFldAlign(VGACFG2, 0x00, FLD_RG_SC2_SCART_EN);

	vRegWriteFldAlign(VFE_E7, 0x00, RG_CINRB_EN);//disconnect C in R at CHB
    vRegWriteFldMulti(SVF_02, P_Fld(0,SVFBST_HDTV)|P_Fld(0,SVFBST_FIR));//disable HDTV burst detector
    #if !TVD_NEW_BST_DET_TH
    vRegWriteFldAlign(SVF_03, 0x8, SVF_BSTDET_TH);
	#endif
	vRegWriteFldAlign(SYS_00, 0, DATA_SYNC_INV); // For MT5387 TVD change to 54MHz issue    
	vRegWriteFldAlign(SYS_00, 0, SUB_DATA_SYNC_INV); // For MT5363 Sub PIP TVD change to 54MHz issue
	vDrvBypPD();
}


// =>
/**
 * @brief Set Scart Mix mode
 * Set SCART
 * @param void
 */
void vScartMix(void)
{
   #if HDTV_BST_DET
      UINT8 bCh;
   #endif
	if(bINT_BST_DET==1) //Internal Path
	{
	       //LogS("bINT_BST_DET == 1 at Mix");
		#if HDTV_BST_DET //HDTV Bst Detector --> need to enable VGA clamping
      		vRegWriteFldMulti(SVF_02, P_Fld(1,SVFBST_HDTV)|P_Fld(1,SVFBST_FIR));
      		vRegWriteFldAlign(SVF_03, 0x4C, SVF_BSTDET_TH);
	       // VGA clamp power on
		for (bCh=0; bCh<3; bCh++)
		{
			vRegWriteFldAlign(VFE_01+(bCh<<4), 0, CLAMP_SOURCE_OFF_1);
			vRegWriteFldAlign(VFE_01+(bCh<<4), 0, CLAMP_SINK_OFF_1);
		}
		vRegWriteFldAlign(HDTV_03, 1, HDTV_EN);
     	      //LogS("bINT_BST_DET==1 and HDTV_BST at Mix");
	      #else //TVD Bst Detector
		       vRegWriteFldMulti(SVF_02, P_Fld(0,SVFBST_HDTV)|P_Fld(0,SVFBST_FIR));
		  	   #if !TVD_NEW_BST_DET_TH
		       vRegWriteFldAlign(SVF_03, 0x8, SVF_BSTDET_TH);
			   #endif
		       //LogS("bINT_BST_DET==1 and TVD_BST at Mix");
	      #endif
	}
	else //TVD Bst Detector & External Path
	{
	       vRegWriteFldMulti(SVF_02, P_Fld(0,SVFBST_HDTV)|P_Fld(0,SVFBST_FIR));
		   #if !TVD_NEW_BST_DET_TH
	       vRegWriteFldAlign(SVF_03, 0x8, SVF_BSTDET_TH);
		   #endif
	       //LogS("bINT_BST_DET==0 at Mix");
	}
      //TVD ch1 clamp on bottom (clamping off); s-first mode
	vDrvSetTVADC(SV_VS_CVBS1);
	//mtk01140 20090902 remove to prevent call too early will get wrong DRAM address
        //vDrvTDCOnOff(SV_ON);
}



// =>
/**
 * @brief Set SCART SVDO
 * Set SCART
 * @param void
 */
void vScartSvConnect(void)
{
    #if HDTV_BST_DET
	UINT8 bCh;
    #endif	   
     	if(bINT_BST_DET==1) // Internal Path --> need to disable VGA clamping when SV
	{
	     //LogS("bINT_BST_DET==1 at SV");
		#if HDTV_BST_DET //HDTV Bst Detector
	for (bCh=0; bCh<3; bCh++)
	{
		vRegWriteFldAlign(VFE_01+(bCh<<4), 1, CLAMP_SOURCE_OFF_1);
		vRegWriteFldAlign(VFE_01+(bCh<<4), 1, CLAMP_SINK_OFF_1);
	}
    vRegWriteFldAlign(HDTV_03, 1, HDTV_EN);	
      #endif
	} 
     	//LogS("bINT_BST_DET == 0 or 1 at SV");
       vRegWriteFldMulti(SVF_02, P_Fld(0,SVFBST_HDTV)|P_Fld(0,SVFBST_FIR));
		#if !TVD_NEW_BST_DET_TH
       vRegWriteFldAlign(SVF_03, 0x8, SVF_BSTDET_TH);
		#endif
	//TVD ch1 clamp on middle; s-video mode
	vDrvSetTVADC(SV_VS_S1);
	//fix mixed mode to SV , tdc on problem.
//mtk01140 20090902 3D COMB Callbak Test
#ifndef TDC_CALLBACK_CONTROL_BY_FBM	
	vDrvTDCOnOff(SV_OFF);
#endif //TDC_CALLBACK_CONTROL_BY_FBM	

}

#endif


/********************************************************/
/***************  Adaptive Slicer  **********************/
/********************************************************/

#if ADAPTIVE_SLICER_TSET

static HAL_TIME_T cur_time[20];
UINT16 wAS_CV_totalMeasure(void);
UINT16 wASV_MONtotalMeasure(void);

/**
 * @brief void vResetVLen(void)
 * Reset Vlen counter when losing V sync for VGA
 * @param	None
 * @retval	None
 */
void vResetVLenSP0(void)
{
	vRegWriteFldAlign(ASYNC_02, 0x1, AS_HLEN_VLEN_RESET);   // Set HVLEN_RESET
	vUtDelay2us(10);
	vRegWriteFldAlign(ASYNC_02, 0x0, AS_HLEN_VLEN_RESET);  // Disable HVLEN_RESET
}

/**
 * @brief void vResetVLen(void)
 * Reset Vlen counter when losing V sync for VGA
 * @param	None
 * @retval	None
 */
void vResetVLenSP2(void)
{
	vRegWriteFldAlign(ASYNC_02, 0x1, AS_HLEN_VLEN_RESET_SP2);   // Set HVLEN_RESET in SP2
	vUtDelay2us(10);
	vRegWriteFldAlign(ASYNC_02, 0x0, AS_HLEN_VLEN_RESET_SP2);   // Disable HVLEN_RESET in SP2
}

/**
 * @brief Return Monitor Slicer  V-Length
 * @param void
 */
UINT16 wASV_MONtotalMeasure(void)
{
	UINT16 measure;
    measure = RegReadFldAlign(STA_SYNC2_01, AS2_V_LEN_S);//MT5360B has the same address with MT5360A
	return (measure);
}

/**
 * @brief Return composite sync V-Length in Main Slicer
 *
 * @param void
 */
UINT16 wAS_CV_totalMeasure(void)
{
	UINT16 measure;
	measure = RegReadFldAlign(STA_SYNC0_00, AS_V_LEN_S);
	return (measure);
}

int VFErateXX(UINT8 low,UINT8 high,UINT16 vlen,UINT16 target)
{
	/* 272 & 271 */
	INT16 min,max;
	if ((vlen<200)||(vlen>2000))
		return 0;

//	if((low==8)&&(high==8))
//		Printf("8888888888888:vlen=%d target=%d\n",vlen,target);
	
	if (bHdtvInterlace())
	{
		min=target-1;
		max=target+1;
	}
	else
	{
		min=target;
		max=target;
	}
	if (min<0)
	{
		min=0;
	}
	if (max>2000)
	{
		max=2000;
	}
	if ((vlen>=min)&&(vlen<=max))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


UINT8 VFEverC_slicer[16*16]={
                                0x00,
                            };

void vVFESLT_slicer_test_verC(void)
{
	int testok=1;
	int time_esplase;
	int i,j;

	for (i=0;i<16;i++)
		for (j=0;j<16;j++)
			VFEverC_slicer[16*i+j]=(i<<4)|j;

	LOG(1,"#SLT:[  ]  [#00] [#01] [#02] [#03] [#04] [#05] [#06] [#07] [#08] [#09] [#10] [#11] [#12] [#13] [#14] [#15]\n");
	for (i=0;i<sizeof(VFEverC_slicer);i++)
	{
		UINT16 r_vlen,target_len;
		HAL_GetTime(&cur_time[0]);

		if ((i%16)==0)
			LOG(1,"#SLT:[%2d] ",i/16);	
		vIO32WriteFldMulti(SYNCFG1, P_Fld((VFEverC_slicer[i]>>3), FLD_RG_SOGY_VLSEL)|P_Fld((VFEverC_slicer[i]), FLD_RG_SOGY_VHSEL));

		vResetVLenSP0();
		do
		{
			HAL_GetTime(&cur_time[1]);
			time_esplase=((cur_time[1].u4Seconds-cur_time[0].u4Seconds)*1000000)+
			             (cur_time[1].u4Micros-cur_time[0].u4Micros);
		}
		while ( (time_esplase<50000));
		HAL_GetTime(&cur_time[0]);
		target_len=wAS_CV_totalMeasure();
		testok=1;
		do
		{
			r_vlen=wAS_CV_totalMeasure();
			testok=VFErateXX(VFEverC_slicer[i]>>4,VFEverC_slicer[i]&0xf,r_vlen,target_len);
			HAL_GetTime(&cur_time[1]);
			time_esplase=((cur_time[1].u4Seconds-cur_time[0].u4Seconds)*1000000)+
			             (cur_time[1].u4Micros-cur_time[0].u4Micros);
		}
		while ( (time_esplase<200000)&&(testok));
		if (testok)
			//LOG(3," [%x@%x]",VFEverC_slicer[i]>>4,VFEverC_slicer[i]&0xf);
			LOG(1," [%3x]",r_vlen);
		else
			LOG(1," /%3x/",r_vlen);

		if ((i%16)==15)
			LOG(1,"\n");
	}

	//*******************************************************************
#if 1
	//LOG(3,"#SLT: ---%d---\n",bSLT_port);
	LOG(1,"#SLT:[  ]  [#00] [#01] [#02] [#03] [#04] [#05] [#06] [#07] [#08] [#09] [#10] [#11] [#12] [#13] [#14] [#15]\n");
	for (i=0;i<sizeof(VFEverC_slicer);i++)
	{
		UINT16 r_vlen,target_len;
		HAL_GetTime(&cur_time[0]);
		if ((i%16)==0)
			LOG(1,"#SLT:[%2d] ",i/16);
		vIO32WriteFldMulti(SYNCFG3, P_Fld((VFEverC_slicer[i]>>3), FLD_RG_MON_VLSEL)|P_Fld((VFEverC_slicer[i]), FLD_RG_MON_VHSEL));

		vResetVLenSP2();
		do
		{
			HAL_GetTime(&cur_time[1]);
			time_esplase=((cur_time[1].u4Seconds-cur_time[0].u4Seconds)*1000000)+
			             (cur_time[1].u4Micros-cur_time[0].u4Micros);
		}
		while ( (time_esplase<50000));
		HAL_GetTime(&cur_time[0]);
		target_len=wASV_MONtotalMeasure();
		testok=1;
		do
		{
			r_vlen=wASV_MONtotalMeasure();
			testok=VFErateXX(VFEverC_slicer[i]>>4,VFEverC_slicer[i]&0xf,r_vlen,target_len);
			HAL_GetTime(&cur_time[1]);
			time_esplase=((cur_time[1].u4Seconds-cur_time[0].u4Seconds)*1000000)+
			             (cur_time[1].u4Micros-cur_time[0].u4Micros);
		}
		while ( (time_esplase<200000)&&(testok));
		if (testok)
			LOG(1," [%3x]",r_vlen);
		else
			LOG(1," /%3x/",r_vlen);

		if ((i%16)==15)
			LOG(1,"\n");
	}
#endif
	//Change PGA setting
	vIO32WriteFldMulti(SYNCFG1, P_Fld(8, FLD_RG_SOGY_VLSEL)|P_Fld(8, FLD_RG_SOGY_VHSEL));

}
#endif

void vQuerySlicer(void)
{
  #ifndef CC_UP8032_ATV
    Printf("slicer Low/High %d/%d\n", IO32ReadFldAlign(SYNCFG1,  FLD_RG_SOGY_VLSEL), IO32ReadFldAlign(SYNCFG1,  FLD_RG_SOGY_VHSEL));
  #endif  

}

#if ADAPTIVE_SLICER_ENABLE
UINT8 check_quaity_state;
#define SLICER_LV_LIST_NUM 5 // 4
#if ADAPTIVE_MONITOR_SLICER_MEASURE
PRIVATE UINT8 code  MonSlicerSetting[] ={0x14, 0x14, 0x9C, 0x9C, 0x36}; // {0x9c, 0x14, 0x9C, 0x9C, 0x36};
#endif

PRIVATE void vSetSlicer(UINT8 pair)
{
	if(bHdtvOpt05_AdaptiveSlicer)
	{
		vIO32WriteFldAlign(SYNCFG1, pair, FLD_RG_SOGY_VLHSEL);//MT8222 set 0x8 for YPbPr	
		LOG(5, "set slicer Low/High 0x%x\n", pair);
    #if ADAPTIVE_MONITOR_SLICER_MEASURE
    vSetMONSlicer_Matrix();
    #endif 		
	}
}

#if ADAPTIVE_MONITOR_SLICER_MEASURE
void vSetMONSlicer_Matrix(void)
{
 if (_bCurSlicerIdx>=SLICER_LV_LIST_NUM)
 {
  return;
 }
 vSetMONSlicer(MonSlicerSetting[_bCurSlicerIdx]);
}
#endif


UINT8 _bCurSlicerIdx=0 ;
#if ADAPTIVE_MONITOR_SLICER_MEASURE_ONLINE
UINT8 _bCurSlicerIdx_best=0 ;
#endif

void vSetDefaultSlicer(void)
{
    //Printf("set slicer in DefaultSlicer\n");
    #if (defined(ADAPTIVE_SLICER_DEFAULT_SETTING) || defined(ADAPTIVE_MONITOR_SLICER_MEASURE))
    _bCurSlicerIdx = 0;
    #endif
    #if ADAPTIVE_MONITOR_SLICER_MEASURE_ONLINE
    _bCurSlicerIdx_best= 0;    
    #endif
    vSetSlicer(0x69);  // 0x88
}

PRIVATE void check_quaity(UINT16 vlen,UINT8 *trylist, UINT8 trynr, UINT32 ms_delay, UINT32 ms_delay2, UINT8 h_prot)
{
static	UINT8 i;
static	INT32 bestdiff;
static	HAL_TIME_T t_init;
static	UINT8 bestone;
static      INT32 ai4DiffScore[SLICER_LV_LIST_NUM];
static      UINT8 bestlevel;
              UINT8 bTmpCnt;
		INT32 diff;
		HAL_TIME_T t_end;
		UINT32 time_esplase;

	if((!vlen)||(!trynr))
	{
		if(trylist)		
			vSetSlicer(trylist[0]);
		check_quaity_state=10;
		return;
	}

	ms_delay=ms_delay*1000;
	ms_delay2=ms_delay2*1000;
	switch(check_quaity_state)
	{
		case 0:
			i=0;
			bestone=trylist[0];
			bestdiff=0xffff;
			check_quaity_state++;
			break;
		case 1:
                     //Printf("set slicer in Fine tune\n");
			vSetSlicer(trylist[i]);
			vResetVLen();
			HAL_GetTime(&t_init);
			check_quaity_state++;
			break;
		case 2:
			HAL_GetTime(&t_end);
			time_esplase=((t_end.u4Seconds-t_init.u4Seconds)*1000000)+
			(t_end.u4Micros-t_init.u4Micros);
			if(time_esplase>=ms_delay)
			{
				HAL_GetTime(&t_init);
				check_quaity_state++;
			}
			break;
		case 3:
			HAL_GetTime(&t_end);
			diff=vlen-wASVtotalMeasure();
			if(diff<0) diff=-diff;
			
			ai4DiffScore[i] = diff;
			//Printf( "Slicer[%d] Diff = %x\n", i, diff);
			if(diff>=2) 
			{
				if(diff<bestdiff)
				{
					bestone=trylist[i];
					bestdiff=diff;
					bestlevel=i;
				}
				i++;
				if(i<trynr)
				{
					check_quaity_state=1;
				}
				else
				{
					check_quaity_state=4;
				}
				break;
			}
			time_esplase=((t_end.u4Seconds-t_init.u4Seconds)*1000000)+
				(t_end.u4Micros-t_init.u4Micros);
			if(time_esplase>=ms_delay2)
			{
				bestone=trylist[i];
				bestlevel=i;
            #if ADAPTIVE_MONITOR_SLICER_MEASURE_ONLINE
                _bCurSlicerIdx_best=bestlevel;
            #endif   				
				check_quaity_state=4;
			}
			break;
		case 4:
                     //Printf( "Final_at_case 4_Slicer[%d], level = 0x%x,  Diff = %x\n", i, trylist[i],  ai4DiffScore[i]);
                     //Printf( "best_before_center_Slicer[%d], level = 0x%x,  Diff = %x\n", bestlevel, trylist[bestlevel],  ai4DiffScore[bestlevel]);
			for(bTmpCnt=0;bTmpCnt<i;bTmpCnt++)
			{
			    if(ai4DiffScore[bestlevel] >= ai4DiffScore[bTmpCnt])
			    {
			        /* bTmpCnt is better or equal to the bestone */
			        if(DIFF(bestlevel, trynr/2) > DIFF(bTmpCnt, trynr/2))
			        {
			            /* bTmpCnt is near center of the list */
			            bestlevel = bTmpCnt;
			            bestone = trylist[bTmpCnt];
			            //Printf("Center_Slicer = 0x%x\n", bestone);
			        }
			    }
			}
                     //Printf("final choose bestone in fine tune = 0x%x\n", bestone);
			vSetSlicer(bestone);
			
			check_quaity_state=10;
			break;			
	}
		
}

typedef struct slicerSetting_s
{
        UINT8 bCoarse;
        UINT8 abVReduce[4];
}slicerSetting_t;
    
 
PRIVATE slicerSetting_t code rSlicerSetting[] =
{   
        0x69, {0x69,0x7A,0x8B,0x9B}, // 0x58, {0x58,0x69,0x79,0x89}, // 0xa8, {0x98,0xa8,0xb9,0xc9},
        0x9C, {0x9C,0xAD,0xBE,0xCF}, // 0x79, {0x69,0x79,0x89,0x9A}, // 0xb9, {0xa8,0xb9,0xc9,0xca},
        0x36, {0x36,0x47,0x58,0x69},
        0x14, {0x14,0x15,0x26,0x37}, // 0x04, {0x04,0x15,0x26,0x37},  // 480i HV Reduce
        0xBE, {0xBE,0xCE,0xDE,0xCF}, // 0xCF, {0xCF,0xDF,0xCE,0xDE},  // 720p60 V Reduce
};

#if defined(CC_MT5387) && !defined(CC_MT5363)
PRIVATE slicerSetting_t code rSlicerSetting1[] =
{   
        0x69, {0x7A,0x8B,0x9B,0x58}, // 0x58, {0x58,0x69,0x79,0x89}, // 0xa8, {0x98,0xa8,0xb9,0xc9},
        0x8A, {0x9B,0xAB,0xAC,0x7A}, // 0x79, {0x69,0x79,0x89,0x9A}, // 0xb9, {0xa8,0xb9,0xc9,0xca},
        0x36, {0x36,0x47,0x58,0x69}, 
        0x02, {0x02,0x13,0x24,0x35}, // 0x89, {0x79,0x89,0x9A,0xAA}  // 0x77, {0x77,0x88,0x98,0xa8},         
        0xDF, {0xDF,0xCE,0xBD,0xAC}, // 0x9A, {0x89,0x9A,0xAA,0xAB} // 0x98, {0x88,0x98,0xa8,0xb9},            
};
#endif
//backup level
/* 
    0x88, {0x78,0x88,0x98,0xa8},
    0x98, {0x88,0x98,0xa8,0xb9},
    0xa8, {0x98,0xa8,0xb9,0xc9},
    0xb9, {0xa8,0xb9,0xc9,0xca},
*/
/*
PRIVATE UINT8 SlicerCoarse[]={0x88,0x98,0xA8,0xB9};
PRIVATE UINT8 SlicerVReduce[][]={{0x88,0x98,0xA8,0xB9},
	                                                                               {0x88,0x98,0xA8,0xB9},
	                                                                               {0x88,0x98,0xA8,0xB9},
	                                                                               {0x88,0x98,0xA8,0xB9}
                                                                                 };*/
void  vSliceQuality536x(void)
{

	UINT8 *list;

	#if defined(CC_MT5387) && !defined(CC_MT5363)
    if (BSP_GetIcVersion() <= IC_VER_5387_AA) // Before ECO	
    list = rSlicerSetting1[(_bCurSlicerIdx)%(sizeof(rSlicerSetting)/sizeof(slicerSetting_t))].abVReduce;
    else
    list = rSlicerSetting[(_bCurSlicerIdx)%(sizeof(rSlicerSetting)/sizeof(slicerSetting_t))].abVReduce;    
    #else
    list = rSlicerSetting[(_bCurSlicerIdx)%(sizeof(rSlicerSetting)/sizeof(slicerSetting_t))].abVReduce;
    #endif
    
	  switch(_bHdtvTiming)
	  {
		case MODE_525I:
		case MODE_525I_OVERSAMPLE:
				check_quaity(263,list,4,40,360,1);
				break;
		case MODE_625I:
		case MODE_625I_OVERSAMPLE:
				check_quaity(313,list,4,50,440,1);
				break;
		case MODE_480P:
		case MODE_480P_OVERSAMPLE:
				check_quaity(525,list,4,40,360,1);
				break;
		case MODE_576P:
		case MODE_576P_OVERSAMPLE:	
				check_quaity(625,list,4,50,440,1);
				break;
		case MODE_720p_50:
				check_quaity(750,list,4,50,440,0);//0x2f3
				break;
		case MODE_720p_60:	
				check_quaity(750,list,4,40,360,0);//0x2f3
				break;
		case MODE_1080i_50:
				check_quaity(563,list,4,50,440,0);//0x243
				break;		
		case MODE_1080i:		
				check_quaity(563,list,4,40,360,0);//0x243
				break;
	  	case MODE_1080p_50:
		  		check_quaity(1125,list,4,50,440,0);//0x46a
				break;
	  	case MODE_1080p_60:
		  		check_quaity(1125,list,4,40,360,0);//0x46a
				break;
	  	default:
			/*	MODE_1080i_48,         
				MODE_1080p_24,   
				MODE_1080p_25,  
				MODE_1080p_30,  */
		  	check_quaity(0,list,4,50,110,0);
			break;

		case MODE_NOSIGNAL:
		case MODE_NODISPLAY:
		case MODE_NOSUPPORT:
		case MODE_WAIT:
			return;
	}
}


HAL_TIME_T rPreSliceTime, rDeltaSliceTime, _rCurSliceTime;
void vResetSliceTimer(void)
{
    if(bHdtvOpt05_AdaptiveSlicer)
    {
         HAL_GetTime(&rPreSliceTime);
         //Printf("set slicer in ResetSliceTimer\n");
         //vSetDefaultSlicer();
         /* default level index */
         //_bCurSlicerIdx = 0;//reset slicer level to index 0 to match default setting
    }     
}

void vNextSlicer(UINT16 wTimeout)
{
    if(bHdtvOpt05_AdaptiveSlicer)
    {
    	UINT32 u4MilliSec;
    	
        HAL_GetTime(&_rCurSliceTime);
        HAL_GetDeltaTime(&rDeltaSliceTime, &_rCurSliceTime, &rPreSliceTime);
        u4MilliSec = (rDeltaSliceTime.u4Seconds * 1000) + (rDeltaSliceTime.u4Micros / 1000);
        if(u4MilliSec >= wTimeout)
        {     
            HAL_GetTime(&rPreSliceTime);
            _bCurSlicerIdx++;
        #if ADAPTIVE_MONITOR_SLICER_MEASURE   
            if (_bCurSlicerIdx>=5)
            {
             _bCurSlicerIdx=0;
            }
        #endif
            //Printf("set slicer in Coarse tune\n");
 	        #if defined(CC_MT5387) && !defined(CC_MT5363)
            if (BSP_GetIcVersion() <= IC_VER_5387_AA) // Before ECO            
            vSetSlicer(rSlicerSetting1[(_bCurSlicerIdx)%(sizeof(rSlicerSetting)/sizeof(slicerSetting_t))].bCoarse);
            else
            vSetSlicer(rSlicerSetting[(_bCurSlicerIdx)%(sizeof(rSlicerSetting)/sizeof(slicerSetting_t))].bCoarse);            
            #else
            vSetSlicer(rSlicerSetting[(_bCurSlicerIdx)%(sizeof(rSlicerSetting)/sizeof(slicerSetting_t))].bCoarse);
            #endif
            #if ADAPTIVE_MONITOR_SLICER_MEASURE
            vSetMONSlicer(MonSlicerSetting[_bCurSlicerIdx]);
            #endif             
        }    
    }    
}


#endif

#if ADAPTIVE_MONITOR_SLICER_MEASURE
void vSetMONSlicer(UINT8 pair)
{
	if(bHdtvOpt05_AdaptiveSlicer)
	{
     vIO32WriteFldAlign(SYNCFG3, pair, FLD_RG_MON_VLHSEL);
     DBG_Printf(DBG_MCHG,"[SA7] set MON slicer Low/High = 0x%x \n",pair);    
    }
}

#if ADAPTIVE_MONITOR_SLICER_MEASURE_DEBUG
UINT8 bReadMONSlicer(void)
{
    return(IO32ReadFldAlign(SYNCFG3, FLD_RG_MON_VLHSEL));
}

UINT8 bReadNewSlicer(void)
{
    return(IO32ReadFldAlign(SYNCFG1, FLD_RG_SOGY_VLHSEL));
}
#endif
#endif

//--------- To get the auto color gain and offset
/**
 * @brief  Read  3 channel Gain of VGA  from EEPROM
 *
 * @param None
 */ 
UINT8 vCust_EEPROM_VGA_B_Gain_Read(void)
{
UINT8 bGain;
      bGain = bApiEepromReadByte(EEP_VIDEO_AUTO_COLOR_VGA_GAIN0);
	return (bGain);
}
UINT8 vCust_EEPROM_VGA_G_Gain_Read(void)
{
UINT8 bGain;
      bGain = bApiEepromReadByte(EEP_VIDEO_AUTO_COLOR_VGA_GAIN1);
	return (bGain);
}
UINT8 vCust_EEPROM_VGA_R_Gain_Read(void)
{
UINT8 bGain;
      bGain = bApiEepromReadByte(EEP_VIDEO_AUTO_COLOR_VGA_GAIN2);
	return (bGain);
}
UINT8 vCust_EEPROM_VGA_B_Offset_Read(void)
{
UINT8 bOffset;
      bOffset = bApiEepromReadByte(EEP_ADC_OFFSET_VGA_CLAMP0);
	return (bOffset);
}
UINT8 vCust_EEPROM_VGA_G_Offset_Read(void)
{
UINT8 bOffset;
      bOffset = bApiEepromReadByte(EEP_ADC_OFFSET_VGA_CLAMP1);
	return (bOffset);
}
UINT8 vCust_EEPROM_VGA_R_Offset_Read(void)
{
UINT8 bOffset;
      bOffset = bApiEepromReadByte(EEP_ADC_OFFSET_VGA_CLAMP2);
	return (bOffset);
}


/**
 * @brief  Read  3 channel Gain of COMPONENT  from EEPROM
 *
 * @param None
 */ 
UINT8 vCust_EEPROM_CMP_Y_Gain_Read(void)
{
UINT8 bGain;
      bGain = bApiEepromReadByte(EEP_VIDEO_AUTO_COLOR_HDTV_GAIN0);
	return (bGain);
}
UINT8 vCust_EEPROM_CMP_PB_Gain_Read(void)
{
UINT8 bGain;
      bGain = bApiEepromReadByte(EEP_VIDEO_AUTO_COLOR_HDTV_GAIN1);
	return (bGain);
}
UINT8 vCust_EEPROM_CMP_PR_Gain_Read(void)
{
UINT8 bGain;
      bGain = bApiEepromReadByte(EEP_VIDEO_AUTO_COLOR_HDTV_GAIN2);
	return (bGain);
}

UINT8 vCust_EEPROM_CMP_Y_Offset_Read(void)
{
UINT8 bOffset;
      bOffset = bApiEepromReadByte(EEP_ADC_OFFSET_HDTV_CLAMP0);
	return (bOffset);
}
UINT8 vCust_EEPROM_CMP_PB_Offset_Read(void)
{
UINT8 bOffset;
      bOffset = bApiEepromReadByte(EEP_ADC_OFFSET_HDTV_CLAMP1);
	return (bOffset);
}
UINT8 vCust_EEPROM_CMP_PR_Offset_Read(void)
{
UINT8 bOffset;
      bOffset = bApiEepromReadByte(EEP_ADC_OFFSET_HDTV_CLAMP2);
	return (bOffset);
}


/**
 * @brief  Read  RGB Gain of SCART from EEPROM
 *
 * @param None
 */ 
 UINT8 vCust_EEPROM_SCART_B_Gain_Read(void)
{
UINT8 bGain;
      bGain = bApiEepromReadByte(EEP_VIDEO_AUTO_COLOR_SCART_GAIN0);
	return (bGain);
}
UINT8 vCust_EEPROM_SCART_G_Gain_Read(void)
{
UINT8 bGain;
      bGain = bApiEepromReadByte(EEP_VIDEO_AUTO_COLOR_SCART_GAIN1);
	return (bGain);
}
UINT8 vCust_EEPROM_SCART_R_Gain_Read(void)
{
UINT8 bGain;
      bGain = bApiEepromReadByte(EEP_VIDEO_AUTO_COLOR_SCART_GAIN2);
	return (bGain);
}
UINT8 vCust_EEPROM_SCART_B_Offset_Read(void)
{
UINT8 bOffset;
      bOffset = bApiEepromReadByte(EEP_ADC_OFFSET_SCART_CLAMP0);
	return (bOffset);
}
UINT8 vCust_EEPROM_SCART_G_Offset_Read(void)
{
UINT8 bOffset;
      bOffset = bApiEepromReadByte(EEP_ADC_OFFSET_SCART_CLAMP1);
	return (bOffset);
}
UINT8 vCust_EEPROM_SCART_R_Offset_Read(void)
{
UINT8 bOffset;
      bOffset = bApiEepromReadByte(EEP_ADC_OFFSET_SCART_CLAMP2);
	return (bOffset);
}


/**
 * @brief  Read  auto color status for VGA, HDTV, and SCART
 *
 * @param None
 */ 
UINT8 vCust_EEPROM_CMP_STATUS(void)
{
UINT8 bStatus;
      bStatus = bApiEepromReadByte(EEP_VIDEO_AUTO_COLOR_HDTV_DONE);
	return (bStatus);
}
UINT8 vCust_EEPROM_VGA_STATUS(void)
{
UINT8 bStatus;
      bStatus = bApiEepromReadByte(EEP_VIDEO_AUTO_COLOR_VGA_DONE);
	return (bStatus);
}
UINT8 vCust_EEPROM_SCART_STATUS(void)
{
UINT8 bStatus;
      bStatus = bApiEepromReadByte(EEP_VIDEO_AUTO_COLOR_SCART_DONE);
	return (bStatus);
}
void vPGA_LPF_BW(UINT8 bw)
{
      switch(bw)
      {
          case 0: vIO32WriteFldAlign(VGACFG2, 0x00, FLD_RG_PGA_ADC_LPF);//no setting any LPF	
                       vIO32WriteFldAlign(VGACFG2, 0x00, FLD_RG_INPUT_CAP);	 
                       break;

          case 1: vIO32WriteFldAlign(VGACFG2, 0x07, FLD_RG_PGA_ADC_LPF);//pixel clock <=108MHz
                       vIO32WriteFldAlign(VGACFG2, 0x01, FLD_RG_INPUT_CAP);	
                       break;

          case 2: vIO32WriteFldAlign(VGACFG2, 0x04, FLD_RG_PGA_ADC_LPF);//pixel clock >108MHz	
                       vIO32WriteFldAlign(VGACFG2, 0x00, FLD_RG_INPUT_CAP);	
                       break;

          default:
                       break;
      }
}

//===================================================================
//  VFE GPIO Debug Monitor
//===================================================================
#if VFE_GPIODEBUG
extern INT32 GPIO_Enable(INT32 i4GpioNum, INT32 *pfgSet);
extern INT32 GPIO_Output(INT32 i4GpioNum, INT32 *pfgSet);

void GPIO_H(UINT8 u1GpioNumIdx)
{
 INT32 i4Val, i4Set;

 ASSERT(u1GpioNumIdx<VFE_GPIONum);

 i4Val = 0;
 GPIO_Enable(VFE_GPIOPINTbl[u1GpioNumIdx], &i4Val); // Change to input mode first.
 i4Set = 1;
 GPIO_Output(VFE_GPIOPINTbl[u1GpioNumIdx], &i4Set);
 i4Val = 1;
 GPIO_Enable(VFE_GPIOPINTbl[u1GpioNumIdx], &i4Val); // Change to output mode.
}

void GPIO_L(UINT8 u1GpioNumIdx)
{
 INT32 i4Val, i4Set;

 ASSERT(u1GpioNumIdx<VFE_GPIONum);

 i4Val = 0;
 GPIO_Enable(VFE_GPIOPINTbl[u1GpioNumIdx], &i4Val); // Change to input mode first.
 i4Set = 0;
 GPIO_Output(VFE_GPIOPINTbl[u1GpioNumIdx], &i4Set);
 i4Val = 1;
 GPIO_Enable(VFE_GPIOPINTbl[u1GpioNumIdx], &i4Val); // Change to output mode.
}
#endif /* VFE_GPIODEBUG */

