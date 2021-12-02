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
 * $RCSfile: pi_tuner_env57s.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_env57s.c
 *  Panasonic TDFRC151D tuner driver.
 */


#include "tuner_interface_if.h"
#include "PI_Def.h"
#include "ctrl_bus.h"

//#include "pi_dvbt_if.h"  //add by liuqu,20090424
#include "pi_demod_atd.h"
#include "eq_script_tdfrc151d.h"
//#include "pi_def_dtmb.h"


// *** Release Note ***
// Ver     Date         Description
// 1.00    2009.03.25   file established
#define C_TDFRC151D_VERSION				"TDFRC151D 2010-01-01\n 2011-04-09\n v1.0"

//#define C_TDFRC151D_PLL_POLL_INTERVAL   5
#define C_TDFRC151D_PLL_POLL_INTERVAL   20   // to solve SLT thread UART overflow issue, 090601, Ken
#define C_TDFRC151D_PLL_TIMEOUT         300
#define C_TDFRC151D_PLL_TIMEOUT_CNT     C_TDFRC151D_PLL_TIMEOUT/C_TDFRC151D_PLL_POLL_INTERVAL
#define C_TDFRC151D_I2C_CLK_DIV         0x100
#define C_TDFRC151D_PLL_LOCK_BIT        6



#define C_TDFRC151D_TOP_SET             ((U8)   0x02) 
#define C_TDFRC151D_TOP_SET_DVBC        ((U8)   0x04) 
#define C_TDFRC151D_TOP_SET_ANA         ((U8)   0x0A) 
//#define C_TDFRC151D_SAW_BW              SAW_BW_8M 
#define C_TDFRC151D_SAW_BW              2

//#define C_TDFRC151D_FREQ_DBOUND_UPPER   858*1000*1000 
#define C_TDFRC151D_FREQ_DBOUND_UPPER   859*1000*1000      //modified for DVB-C application, Menghau
#define C_TDFRC151D_FREQ_DBOUND_LOWER    48*1000*1000 
#define C_TDFRC151D_FREQ_DBOUND_UPPER_Ana     865*1000*1000
#define C_TDFRC151D_FREQ_DBOUND_LOWER_Ana    44*1000*1000

//#if !fgIssharp2403
#if 0
#define C_TDFRC151D_LO_ADDRESS         ((U16)  0xc2)
#define C_TDFRC151D_IF_FREQUENCY        ((U16)  36167)  /* kHz */
#define C_TDFRC151D_IF_FREQUENCY_ANA    ((U16)  38900)  /* kHz */
#define C_TDFRC151D_IF_FREQUENCY_L1     ((U16)  32900)  /* kHz */
#define C_TDFRC151D_LO_DIVIDER_STEP     ((U32) 166667) /* Hz */
#define C_TDFRC151D_LO_DIVIDER_STEP_ANA ((U16)  62500) /* Hz */
#define C_TDFRC151D_LO_DIVIDER_STEP_DVBC ((U16)  62500) /* Hz */
#else
#define C_TDFRC151D_LO_ADDRESS           ((U16)  0xC2)
#define C_TDFRC151D_IF_FREQUENCY_DTMB          ((U16)  36167)  /* kHz */
#define C_TDFRC151D_IF_FREQUENCY_DVBC         ((U16)  36125)  /* kHz */
#define C_TDFRC151D_IF_FREQUENCY_ANA      ((U16)  38900)  /* kHz */
#define C_TDFRC151D_IF_FREQUENCY_L1     ((U16)  32900)  /* kHz */
#define C_TDFRC151D_LO_DIVIDER_STEP_DTMB       ((U32) 166667)  /* Hz */
#define C_TDFRC151D_LO_DIVIDER_STEP_DVBC       ((U32) 62500)  /* Hz */

#define C_TDFRC151D_LO_DIVIDER_STEP_ANA   ((U16)  50000)  /* Hz */
#endif

#define C_TDFRC151D_CHANNEL_SCAN_JUMP_SMALL_STEP ((U16) 250)
#define C_TDFRC151D_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP ((U16) 1500)
#define C_TDFRC151D_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN ((U16) 500)


#if 0
#define C_TDFRC151D_AGC_IF_SLP           1.0
#define C_TDFRC151D_AGC_IF_INT           0.0
#define C_TDFRC151D_AGC_IF_MAX           0.499  // 0.5 will overflow
#define C_TDFRC151D_AGC_IF_MIN          -0.5
#define C_TDFRC151D_AGC_IF_SLP_SGN  (S8)((C_TDFRC151D_AGC_IF_SLP>0)?1:-1) 
#define C_TDFRC151D_AGC_IF_LVL_MAX      0.4774    //-90dBm    127/256
#define C_TDFRC151D_AGC_IF_LVL_MIN      0.08203125   //-60dBm  21/256


#define C_TDFRC151D_MAX_IF_GAIN_SET       -0.12   
#define C_TDFRC151D_MIN_IF_GAIN_SET       -0.38   
#define C_TDFRC151D_MAX_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_TDFRC151D_MIN_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_TDFRC151D_MAX_IF_GAIN_POW       -100    // dBm
#define C_TDFRC151D_MIN_IF_GAIN_POW		-50     // dBm
#define C_TDFRC151D_MAX_RF_GAIN_POW		 C_TDFRC151D_MIN_IF_GAIN_POW
#define C_TDFRC151D_MIN_RF_GAIN_POW		-20     // dBm
#define C_TDFRC151D_POWER_CALIBRATE        26     // dBm
//#define C_TDFRC151D_SIGNAL_LVL_TH          6  //liuqu,20080926,no use for nimtool

#define C_TDFRC151D_AGC_COEF			((U8)   0xBD)
#define C_TDFRC151D_DEMOD_INPUT_POWER	cIF_TARGET_LEVEL_0CCC_0D36

// ********************************************* //

#define C_U8_TDFRC151D_AGC_IF_SLP         (S8)(C_TDFRC151D_AGC_IF_SLP *32 )
#define C_U8_TDFRC151D_AGC_IF_INT         (S8)(C_TDFRC151D_AGC_IF_INT *64 )
#define C_U8_TDFRC151D_AGC_IF_MAX         (S8)(C_TDFRC151D_AGC_IF_MAX *256)
#define C_U8_TDFRC151D_AGC_IF_MIN         (S8)(C_TDFRC151D_AGC_IF_MIN *256)
#define C_U8_TDFRC151D_AGC_IF_LVL_MAX        (S8)(C_TDFRC151D_AGC_IF_LVL_MAX *256)
#define C_U8_TDFRC151D_AGC_IF_LVL_MIN         (S8)(C_TDFRC151D_AGC_IF_LVL_MIN *256)

#define C_U8_TDFRC151D_MAX_IF_GAIN_SET	(S8)(C_TDFRC151D_MAX_IF_GAIN_SET *256)
#define C_U8_TDFRC151D_MIN_IF_GAIN_SET	(S8)(C_TDFRC151D_MIN_IF_GAIN_SET *256)
#define C_U8_TDFRC151D_MAX_RF_GAIN_SET	(S8)((C_TDFRC151D_MAX_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_TDFRC151D_MIN_RF_GAIN_SET	(S8)((C_TDFRC151D_MIN_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_TDFRC151D_IF_GAIN_RANGE      (S8)(C_TDFRC151D_MIN_IF_GAIN_POW - C_TDFRC151D_MAX_IF_GAIN_POW)
#define C_U8_TDFRC151D_RF_GAIN_RANGE      (S8)(C_TDFRC151D_MIN_RF_GAIN_POW - C_TDFRC151D_MAX_RF_GAIN_POW)
#endif

STATIC UINT8* pTdfrc151dEqNormal[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pTdfrc151dEqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};


///////////////////////////////////
// ALPS TDFRC151D driver
///////////////////////////////////

CHAR* TDFRC151D_GetSwVer(void)
{
	return ((CHAR*)C_TDFRC151D_VERSION);
}

//Justin TODO, pEUCtx->m_aucPara all necessary????
VOID TDFRC151D_TunerInit(ITUNER_CTX_T *pTCtx)
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);
    
    psTunerCtx->I2cAddress = C_TDFRC151D_LO_ADDRESS;
    psTunerCtx->u2IF_Freq = C_TDFRC151D_IF_FREQUENCY_DTMB;
    psTunerCtx->u4RF_Freq = 0;
	psTunerCtx->u2IF_Freq_A = C_TDFRC151D_IF_FREQUENCY_ANA;
    //Justin TODO
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_DRO|MASK_PATCH_SENS);
    psTunerCtx->fgRFAGC = TRUE;
    // AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x2f;  //according to yu ding's mail,20091123
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0xFA;
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B;

    pEUCtx->m_SAW_BW  = C_TDFRC151D_SAW_BW;
    pEUCtx->m_Ana_Top = C_TDFRC151D_TOP_SET_ANA;
    pEUCtx->m_s4FreqBoundUpper = C_TDFRC151D_FREQ_DBOUND_UPPER;
    pEUCtx->m_s4FreqBoundLower = C_TDFRC151D_FREQ_DBOUND_LOWER;
    pEUCtx->m_s4AnaFreqBoundUpper = C_TDFRC151D_FREQ_DBOUND_UPPER_Ana;
    pEUCtx->m_s4AnaFreqBoundLower = C_TDFRC151D_FREQ_DBOUND_LOWER_Ana;
    
    pEUCtx->m_Small_Step = C_TDFRC151D_CHANNEL_SCAN_JUMP_SMALL_STEP;
    pEUCtx->m_Middle_Step_Up = C_TDFRC151D_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP;
    pEUCtx->m_Middle_Step_Down = C_TDFRC151D_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN;
    
    // add by JC, 081215, for Signal Level display
    #if 0
    pEUCtx->m_ifAgc_lvl_max = (U8)C_U8_TDFRC151D_AGC_IF_LVL_MAX;
    pEUCtx->m_ifAgc_lvl_min = (U8)C_U8_TDFRC151D_AGC_IF_LVL_MIN;
    
    pEUCtx->m_SigLvTh = 0;//C_TDTWS710D_SIGNAL_LVL_TH;
    pEUCtx->m_SigLvScan = 0;
    
    pEUCtx->m_aucPara[0] = C_TDFRC151D_TOP_SET;
    pEUCtx->m_aucPara[1] = 0;
    
    pEUCtx->m_aucPara[2] = (U8)C_U8_TDFRC151D_AGC_IF_SLP;
    pEUCtx->m_aucPara[3] = (U8)C_U8_TDFRC151D_AGC_IF_INT;
    pEUCtx->m_aucPara[4] = (U8)C_U8_TDFRC151D_AGC_IF_MAX;
    pEUCtx->m_aucPara[5] = (U8)C_U8_TDFRC151D_AGC_IF_MIN;
    
    pEUCtx->m_aucPara[6] = C_TDFRC151D_AGC_COEF;
    pEUCtx->m_aucPara[7] = C_TDFRC151D_DEMOD_INPUT_POWER;
    
    pEUCtx->m_aucPara[ 8] = (U8)C_U8_TDFRC151D_MAX_IF_GAIN_SET;
    pEUCtx->m_aucPara[ 9] = (U8)C_U8_TDFRC151D_MIN_IF_GAIN_SET;
    pEUCtx->m_aucPara[10] = (U8)C_U8_TDFRC151D_MAX_RF_GAIN_SET;
    pEUCtx->m_aucPara[11] = (U8)C_U8_TDFRC151D_MIN_RF_GAIN_SET;
    pEUCtx->m_aucPara[12] = (U8)C_U8_TDFRC151D_IF_GAIN_RANGE;
    pEUCtx->m_aucPara[13] = (U8)C_U8_TDFRC151D_RF_GAIN_RANGE;
    pEUCtx->m_aucPara[14] = C_TDFRC151D_POWER_CALIBRATE;
	#endif

    pTdfrc151dEqNormal[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQTdfrc151dBG;
    pTdfrc151dEqNormal[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQTdfrc151dDK;
    pTdfrc151dEqNormal[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQTdfrc151dI;
    pTdfrc151dEqNormal[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQTdfrc151dL;
#ifdef CC_MT5363	
    pTdfrc151dEqNormal[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQTdfrc151dL1;
#else
    pTdfrc151dEqNormal[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQTdfrc151dL;
#endif
	pTdfrc151dEqNormal[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]	= EQTdfrc151dM;

    pTdfrc151dEqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQTdfrc151dBGWeak;
    pTdfrc151dEqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQTdfrc151dDKWeak;
    pTdfrc151dEqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQTdfrc151dIWeak;
    pTdfrc151dEqWeak[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQTdfrc151dLWeak;
#ifdef CC_MT5363	
    pTdfrc151dEqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQTdfrc151dL1Weak;
#else
    pTdfrc151dEqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQTdfrc151dLWeak;
#endif

    mcSHOW_USER_MSG(("Tuner TDFRC151D init done!\n"));
}


 STATIC void  TDFRC151D_SetSawBw(ITUNER_CTX_T * pTunerCtx, UINT8 sawbw)
 {
   pTunerCtx->specific_member.eu_ctx.m_SAW_BW = sawbw;

   mcSHOW_DRVAPI_MSG(("TDFRC151D_SetSawBw = %d\n", sawbw));
   
 }
S16 TDFRC151D_TunerSetFreq(ITUNER_CTX_T *pTCtx, PARAM_SETFREQ_T* param)
{


 U32 Freq = param->Freq; // transfer to video freq
   //U8  AutoSearch = param->fgAutoSearch;
   U8  Mode = param->Modulation;

   UCHAR ucPllValue;
   UCHAR TunerS[6];
   U32 Lo;
   U16 Ndivider, CB1, CB2, BB;
   S16 ii, jj;


   //SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);

   pTCtx->u4RF_Freq = Freq;

   
   if( Mode == MOD_DTMB)
	   {
   
		   Lo = Freq + C_TDFRC151D_IF_FREQUENCY_DTMB; // kHz
		   //Ndivider = (U16) ( ((Lo * 1000) / DTOS403_LO_DIVIDER_STEP) + 0.5 );
			 
		   Ndivider = (U16) ( (Lo*1000+C_TDFRC151D_LO_DIVIDER_STEP_DTMB/2) / C_TDFRC151D_LO_DIVIDER_STEP_DTMB);
		   CB1 = 0xa8;
		   CB2 = 0xe1;
   
	   
		   if (Freq < 45000 || Freq > 865000)
		   {
			   mcSHOW_DBG_MSG(("Out of range for LO!\n"));
			   //return (+1);
		   }
		   
		   if (Freq < 147000)		
		   {
			   BB = 0x81;
		   }
		   else if (Freq < 431000)	
		   {		
			   BB = 0x82;
		   }
		   else if (Freq < 700000)	
		   {		
			   BB = 0x88;
		   }
				   
		   else 				
		   {
			   BB = 0xC8;
		   }
   
   

   
   
   
	   }
	   
	   else if( Mode == MOD_DVBC)
	   {
		   Lo = Freq + C_TDFRC151D_IF_FREQUENCY_DVBC; // kHz
		   //Ndivider = (U16) ( ((Lo * 1000) / DTOS403_LO_DIVIDER_STEP) + 0.5 );
		 
		   Ndivider = (U16) ( (Lo*1000+C_TDFRC151D_LO_DIVIDER_STEP_DVBC/2) / C_TDFRC151D_LO_DIVIDER_STEP_DVBC);
		   CB1 = 0xab;
		   CB2 = 0xe1;
		   
		   if (Freq < 45000 || Freq > 865000)
		   {
			  mcSHOW_DBG_MSG(("Out of range for LO!\n"));
				  //return (+1);
		   }
			  
		   if (Freq < 147000)	   
		   {
			  BB = 0x01;
		   }
		   else if (Freq < 431000) 
		   {	   
			  BB = 0x02;
		   }
		   else 			   
		   {
			  BB = 0x08;
		   }
		   
		   

		   
		   
		   
	   }	   
	   else //analog reception
	   {
	   
		   if( Mode == MOD_SECAM_L1 )
		   {
			   Lo = Freq + C_TDFRC151D_IF_FREQUENCY_L1; // kHz
		   }
		   else
		   {
			   Lo = Freq + C_TDFRC151D_IF_FREQUENCY_ANA; // kHz
		   }   
		   Ndivider = (U16) ( (Lo*1000+C_TDFRC151D_LO_DIVIDER_STEP_ANA/2) / C_TDFRC151D_LO_DIVIDER_STEP_ANA);
		   
		   mcSHOW_DBG_MSG(("Freq = %d, Divider ratio = %04X \n",param->Freq, Ndivider));
		   
		   CB1 = 0xbe;
		   
		   CB2 = 0xe1;
   
		   
		   if (Freq < 45000 || Freq > 865000)
		   {
			  mcSHOW_DBG_MSG(("Out of range for LO!\n"));
		   //  return (+1);    // liuqu,20090910, can not returen 1 for 47.25MHZ channel loss!
		   }
		   
			  
		   if (Freq < 147000)	   
		   {
			  BB = 0x05;
		   }
		   else if (Freq < 431000) 
		   {	   
			  BB = 0x06;
		   }
		   else 			   
		   {
			  BB = 0x0C;
		   }
		   
	   
	   }
   
		
   for (jj=0; jj<3; jj++)  //retry 2 times while PLL unlock, LC 070702
   {
	   // LO settings
	   TunerS[0] = (UCHAR) 0x7F & (Ndivider >> 8);
	   TunerS[1] = (UCHAR) 0xFF & Ndivider;
	   TunerS[2] = (UCHAR) CB1;
	   TunerS[3] = (UCHAR) BB;
	   TunerS[4] = (UCHAR) (CB2|0x20);
	   TunerS[5] = (UCHAR) (CB2 | 0x20);  //ATC=1

	 mcSHOW_DRVAPI_MSG(("0x%02X-0x%02X-0x%02X-0x%02X-0x%02X\n", TunerS[0], TunerS[1], TunerS[2], TunerS[3], TunerS[4]));

 //  send this data
	 //if (RegSetRegistersTuner(pTunerCtx->m_Address, 5, &TunerS[0]) < 0)


	   if (ICtrlBus_I2cTunerWrite(C_TDFRC151D_I2C_CLK_DIV, pTCtx->I2cAddress, TunerS, 5))
	   {
		   mcSHOW_HW_MSG(("TunerWrite PLL failed!\n"));
		   return (+1);
	   }

	   mcDELAY_MS(20);

	   // wait PLL lock
   //ucTunerLockConfCnt = 0;
	   for (ii=0; ii<C_TDFRC151D_PLL_TIMEOUT_CNT; ii++)
	   {
		   if (ICtrlBus_I2cTunerRead(C_TDFRC151D_I2C_CLK_DIV, pTCtx->I2cAddress, &ucPllValue, 1))
		   {
			   mcSHOW_HW_MSG(("Tuner read failed!\n"));
			   return (+1);
		   }
		   // check if PLL lock
		   if (mcCHK_BIT1(ucPllValue, C_TDFRC151D_PLL_LOCK_BIT)) {
			   break;
		   }
		   mcDELAY_MS(C_TDFRC151D_PLL_POLL_INTERVAL);
	   }

	   if(ii < C_TDFRC151D_PLL_TIMEOUT_CNT) {
			mcSHOW_DBG_MSG(("Freq = %d MHz, the %d-th time setting PLL success\n" , Freq, jj+1));
		   break;
		 }
	   else {
			   if (Freq < 48)
			  {    
			 mcSHOW_DBG_MSG(("Freq = %d MHz,Frequency not support\n" , Freq));//liuqu,20090828,DHCID00000589
		   }else
		   {
			 mcSHOW_DBG_MSG(("Freq = %d MHz, the %d-th time setting PLL fail\n" , Freq, jj+1));
		   }
	 }
 }
   
   return 0;




}


INT16 TDFRC151D_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){

    switch(eOperation){
    case itGetVer:
        {
        CHAR ** RetStr = (CHAR **)pInOutVal;
        *RetStr = TDFRC151D_GetSwVer();
        }
        break;
    case itSetTop:
	    {
          SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);
          pTunerCtx->m_aucPara[0] = SetVal;
	    }
		break;
		
	case itSetReg:
	 {
        
		 UCHAR TunerS[5];
		 UCHAR ucPllValue;
		 UINT8 *ptrWord = (UINT8 *)pInOutVal;
		 int i,j;
	
		 SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);
		 
		 for(i = 0; i < 5; i++)
			{
					
					  TunerS[i]=(UCHAR)(*ptrWord);
					  ptrWord++;
					  mcSHOW_DRVERR_MSG(("The register word is: 0x%x\n",TunerS[i]));
					 // mcDELAY(C_TDFRC151D_PLL_POLL_INTERVAL);
			}
           mcSHOW_DRVERR_MSG(("The register words are: 0x%x-0x%x-0x%x-0x%x-0x%x\n",TunerS[0],TunerS[1],TunerS[2],TunerS[3],TunerS[4]));
	 for (j=0; j<2; j++)  //retry 2 times while PLL unlock, LC 070702
       {
   
           if (ICtrlBus_I2cTunerWrite(C_TDFRC151D_I2C_CLK_DIV, pTCtx->I2cAddress, TunerS, 5))    //  send this data
            {
             mcSHOW_DRVERR_MSG(("TunerWrite PLL failed!\n"));
			  
               return (+1);
            }
                mcSHOW_DRVERR_MSG(("TunerWrite PLL success!\n"));
		          pTunerCtx->m_aucPara[1]  = (UCHAR)TunerS[0]; //send register value to UI, LC 070102
                  pTunerCtx->m_aucPara[15] = (UCHAR)TunerS[1];
                  pTunerCtx->m_aucPara[16] = (UCHAR)TunerS[2];
                  pTunerCtx->m_aucPara[17] = (UCHAR)TunerS[3];
                  pTunerCtx->m_aucPara[18] = (UCHAR)TunerS[4];  
   
          for (i=0; i<C_TDFRC151D_PLL_TIMEOUT_CNT; i++)
          {
            if (ICtrlBus_I2cTunerRead(C_TDFRC151D_I2C_CLK_DIV, pTCtx->I2cAddress, &ucPllValue, 1))
            {
              
				mcSHOW_HW_MSG(("Tuner read failed!\n"));
                return (+1);
            }
           
            if (mcCHK_BIT1(ucPllValue, C_TDFRC151D_PLL_LOCK_BIT))   // check if PLL lock
				{
                   pTunerCtx->m_aucPara[19] = ucPllValue;
					break;
                }
            mcDELAY_MS(C_TDFRC151D_PLL_POLL_INTERVAL);
          }

          if(i < C_TDFRC151D_PLL_TIMEOUT_CNT) 
			  {
                  mcSHOW_DBG_MSG(("the %d-th time setting PLL success\n" , j+1));
				  
				  break;
          	   }
          else
        	   mcSHOW_DBG_MSG((" the %d-th time setting PLL fail\n" , j+1));
               
		 	}
		 
	  }
	    break;
		
	case itGetReg:
	    {
            
			int i = 0;
	        SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);
			              *((INT8 *) pInOutVal)   = pTunerCtx->m_aucPara[1]; 
                          *((INT8 *) pInOutVal+1) = pTunerCtx->m_aucPara[15];
                          *((INT8 *) pInOutVal+2) = pTunerCtx->m_aucPara[16];
			              *((INT8 *) pInOutVal+3) = pTunerCtx->m_aucPara[17];
                          *((INT8 *) pInOutVal+4) = pTunerCtx->m_aucPara[18];
                for (i = 0; i < 5; i++)
				mcSHOW_DRVERR_MSG(("The register word is: 0x%x\n",*((INT8 *)pInOutVal+i)));
			   
        }
		break;
    case itSetSawBw:
		TDFRC151D_SetSawBw(pTCtx,SetVal);
        break;
    case itSetLNA:
    case itGetLNA:
        break;

    case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pTdfrc151dEqNormal[SetVal];
        break;
    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pTdfrc151dEqWeak[SetVal];
        break;
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = NULL;
        break;
    case itGetAnaFreqBound:
        {
        TUNER_ANA_ATTRIBUTE_T   *psTunerAnaAttribute;

        psTunerAnaAttribute = (TUNER_ANA_ATTRIBUTE_T *) pInOutVal;
        psTunerAnaAttribute->ui4_lower_bound_freq = C_TDFRC151D_FREQ_DBOUND_LOWER_Ana;
        psTunerAnaAttribute->ui4_upper_bound_freq = C_TDFRC151D_FREQ_DBOUND_UPPER_Ana;
        }
        break; 
    case itSetIfMin:
	    *(UINT8 *)pInOutVal=0xE8;//for Qlab chroma on/off test fail 2010/06/07
		break;        
#if 0
	case itSetIfMin:
	    {

          UINT8 subSysId = SetVal;
		  UINT8 *ifMinPtr = (UINT8*)pInOutVal;
			
         mcSHOW_DBG_MSG(("TDFRC151D TOP Setting\n"));
    	 switch(subSysId){
    	    case MOD_PAL_BG:
    	    case MOD_PAL_I:
	    case MOD_PAL_DK:	
	      {
		if ((pTCtx->u4RF_Freq/1000 >= 71) && (pTCtx->u4RF_Freq/1000 <= 80))
			{
		        *ifMinPtr = 0x15;
			}
		else
			{
    	               *ifMinPtr = 0x00; //according to yu ding's mail,20091123
			}
	    	}
    	        break;
    	    case MOD_SECAM_L:
            case MOD_SECAM_L1:
               *ifMinPtr = 0x09;  ////according to yu ding's mail,20091123
    		    break;
    	    default:
    	        mcSHOW_DBG_MSG(("TOP No change\n"));
    	        break;
        }

	    }
	    break;
#endif

    default:
        return ITUNER_NOT_SUPPORTED;
    }
    return ITUNER_OK;
}

