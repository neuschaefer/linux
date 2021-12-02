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
 * $RCSfile: pi_tuner_tdtws810d.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_tdtws810d.c
 *  Panasonic TDTWS810D tuner driver.
 */


#include "tuner_interface_if.h"
#include "pi_def_dvbt.h"
#include "PI_Def.h"
#include "ctrl_bus.h"
#include "pi_dvbt_if.h"  //add by liuqu,20090424
#include "pi_demod_atd.h"
#include "eq_script_tdtws810d.h"
#include "drvcust_if.h"
#include "x_gpio.h"

// *** Release Note ***
// Ver     Date         Description
// 1.00    2009.03.25   file established

#define GPIO_OUTPUT_ON     1
#define GPIO_OUTPUT_OFF     0
#define C_TDTWS810D_VERSION				"TDTWS810D v20090820"

//#define C_TDTWS810D_PLL_POLL_INTERVAL   5
#define C_TDTWS810D_PLL_POLL_INTERVAL   10   // to solve SLT thread UART overflow issue, 090601, Ken
#define C_TDTWS810D_PLL_TIMEOUT         100
#define C_TDTWS810D_PLL_TIMEOUT_CNT     C_TDTWS810D_PLL_TIMEOUT/C_TDTWS810D_PLL_POLL_INTERVAL
#define C_TDTWS810D_I2C_CLK_DIV         0x100
#define C_TDTWS810D_PLL_LOCK_BIT        6

#define C_TDTWS810D_TOP_SET             ((U8)   0x0b) 
//#define C_TDTWS810D_TOP_SET_ANA         ((U8)   0x0A) 
#define C_TDTWS810D_SAW_BW              SAW_BW_8M 

//#define C_TDTWS810D_FREQ_DBOUND_UPPER   858*1000*1000 
#define C_TDTWS810D_FREQ_DBOUND_UPPER   859*1000*1000  ////modified for DVB-C application, Menghau
#define C_TDTWS810D_FREQ_DBOUND_LOWER    48*1000*1000 
#define C_TDTWS810D_FREQ_DBOUND_UPPER_Ana    875*1000*1000
#define C_TDTWS810D_FREQ_DBOUND_LOWER_Ana    45*1000*1000

#define C_TDTWS810D_LO_ADDRESS         ((U16)  0xc2)
#define C_TDTWS810D_IF_FREQUENCY        ((U16)  36167)  /* kHz */
#define C_TDTWS810D_IF_FREQUENCY_ANA    ((U16)  38900)  /* kHz */
#define C_TDTWS810D_IF_FREQUENCY_L1     ((U16)  32900)  /* kHz */
#define C_TDTWS810D_LO_DIVIDER_STEP     ((U32) 166667) /* Hz */
#define C_TDTWS810D_LO_DIVIDER_STEP_ANA ((U16)  50000) /* Hz */

#define C_TDTWS810D_CHANNEL_SCAN_JUMP_SMALL_STEP ((U16) 250)
#define C_TDTWS810D_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP ((U16) 1500)
#define C_TDTWS810D_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN ((U16) 500)


#define C_TDTWS810D_AGC_IF_SLP           1.0
#define C_TDTWS810D_AGC_IF_INT           0.0
#define C_TDTWS810D_AGC_IF_MAX           0.499  // 0.5 will overflow
#define C_TDTWS810D_AGC_IF_MIN          -0.5
#define C_TDTWS810D_AGC_IF_SLP_SGN  (S8)((C_TDTWS810D_AGC_IF_SLP>0)?1:-1) 
#define C_TDTWS810D_AGC_IF_LVL_MAX     0.2265625    //-90dBm  58/256
#define C_TDTWS810D_AGC_IF_LVL_MIN     -0.09765625    //-60dBm        -25/256


#define C_TDTWS810D_MAX_IF_GAIN_SET       -0.12   
#define C_TDTWS810D_MIN_IF_GAIN_SET       -0.38   
#define C_TDTWS810D_MAX_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_TDTWS810D_MIN_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_TDTWS810D_MAX_IF_GAIN_POW       -100    // dBm
#define C_TDTWS810D_MIN_IF_GAIN_POW		-50     // dBm
#define C_TDTWS810D_MAX_RF_GAIN_POW		 C_TDTWS810D_MIN_IF_GAIN_POW
#define C_TDTWS810D_MIN_RF_GAIN_POW		-20     // dBm
#define C_TDTWS810D_POWER_CALIBRATE        26     // dBm
//#define C_TDTWS810D_SIGNAL_LVL_TH          6  //liuqu,20080926,no use for nimtool

#define C_TDTWS810D_AGC_COEF			((U8)   0xBD)
#define C_TDTWS810D_DEMOD_INPUT_POWER	cIF_TARGET_LEVEL_0CCC

// ********************************************* //

#define C_U8_TDTWS810D_AGC_IF_SLP         (S8)(C_TDTWS810D_AGC_IF_SLP *32 )
#define C_U8_TDTWS810D_AGC_IF_INT         (S8)(C_TDTWS810D_AGC_IF_INT *64 )
#define C_U8_TDTWS810D_AGC_IF_MAX         (S8)(C_TDTWS810D_AGC_IF_MAX *256)
#define C_U8_TDTWS810D_AGC_IF_MIN         (S8)(C_TDTWS810D_AGC_IF_MIN *256)
#define C_U8_TDTWS810D_AGC_IF_LVL_MAX        (S8)(C_TDTWS810D_AGC_IF_LVL_MAX *256)
#define C_U8_TDTWS810D_AGC_IF_LVL_MIN         (S8)(C_TDTWS810D_AGC_IF_LVL_MIN *256)

#define C_U8_TDTWS810D_MAX_IF_GAIN_SET	(S8)(C_TDTWS810D_MAX_IF_GAIN_SET *256)
#define C_U8_TDTWS810D_MIN_IF_GAIN_SET	(S8)(C_TDTWS810D_MIN_IF_GAIN_SET *256)
#define C_U8_TDTWS810D_MAX_RF_GAIN_SET	(S8)((C_TDTWS810D_MAX_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_TDTWS810D_MIN_RF_GAIN_SET	(S8)((C_TDTWS810D_MIN_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_TDTWS810D_IF_GAIN_RANGE      (S8)(C_TDTWS810D_MIN_IF_GAIN_POW - C_TDTWS810D_MAX_IF_GAIN_POW)
#define C_U8_TDTWS810D_RF_GAIN_RANGE      (S8)(C_TDTWS810D_MIN_RF_GAIN_POW - C_TDTWS810D_MAX_RF_GAIN_POW)

STATIC UINT8* pTdtws810dEqNormal[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pTdtws810dEqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};


static BOOL fgLNA = 0;  // 0 is LNA off, 1 is LNA on.Add by Jack Mei,2009/10/20


///////////////////////////////////
// ALPS TDTWS810D driver
///////////////////////////////////

CHAR* TDTWS810D_GetSwVer(void)
{
	return ((CHAR*)C_TDTWS810D_VERSION);
}

//Justin TODO, pEUCtx->m_aucPara all necessary????
VOID TDTWS810D_TunerInit(ITUNER_CTX_T *pTCtx)
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);
    
    psTunerCtx->I2cAddress = C_TDTWS810D_LO_ADDRESS;
    psTunerCtx->u2IF_Freq = C_TDTWS810D_IF_FREQUENCY;
    psTunerCtx->u4RF_Freq = 0;
    //Justin TODO
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_DRO|MASK_PATCH_SENS);
    psTunerCtx->fgRFAGC = TRUE;
    // AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x2f; //EC_code_d9930: Change 0x151 value for EU new BPF
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0xDC; //EC_code_d9930: Change 0x158 value for EU new BPF
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B;//EC_code_d9930: Change 0x15f value for EU new BPF

    pEUCtx->m_SAW_BW  = C_TDTWS810D_SAW_BW;
    //pEUCtx->m_Ana_Top = C_TDTWS810D_TOP_SET_ANA;
    pEUCtx->m_s4FreqBoundUpper = C_TDTWS810D_FREQ_DBOUND_UPPER;
    pEUCtx->m_s4FreqBoundLower = C_TDTWS810D_FREQ_DBOUND_LOWER;
    pEUCtx->m_s4AnaFreqBoundUpper = C_TDTWS810D_FREQ_DBOUND_UPPER_Ana;
    pEUCtx->m_s4AnaFreqBoundLower = C_TDTWS810D_FREQ_DBOUND_LOWER_Ana;
    
    pEUCtx->m_Small_Step = C_TDTWS810D_CHANNEL_SCAN_JUMP_SMALL_STEP;
    pEUCtx->m_Middle_Step_Up = C_TDTWS810D_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP;
    pEUCtx->m_Middle_Step_Down = C_TDTWS810D_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN;
    
    // add by JC, 081215, for Signal Level display
    pEUCtx->m_ifAgc_lvl_max = (U8)C_U8_TDTWS810D_AGC_IF_LVL_MAX;
    pEUCtx->m_ifAgc_lvl_min = (U8)C_U8_TDTWS810D_AGC_IF_LVL_MIN;
    
    pEUCtx->m_SigLvTh = 0;//C_TDTWS710D_SIGNAL_LVL_TH;
    pEUCtx->m_SigLvScan = 0;
    
    pEUCtx->m_aucPara[0] = C_TDTWS810D_TOP_SET;
    pEUCtx->m_aucPara[1] = 0;
    
    pEUCtx->m_aucPara[2] = (U8)C_U8_TDTWS810D_AGC_IF_SLP;
    pEUCtx->m_aucPara[3] = (U8)C_U8_TDTWS810D_AGC_IF_INT;
    pEUCtx->m_aucPara[4] = (U8)C_U8_TDTWS810D_AGC_IF_MAX;
    pEUCtx->m_aucPara[5] = (U8)C_U8_TDTWS810D_AGC_IF_MIN;
    
    pEUCtx->m_aucPara[6] = C_TDTWS810D_AGC_COEF;
    pEUCtx->m_aucPara[7] = C_TDTWS810D_DEMOD_INPUT_POWER;
    
    pEUCtx->m_aucPara[ 8] = (U8)C_U8_TDTWS810D_MAX_IF_GAIN_SET;
    pEUCtx->m_aucPara[ 9] = (U8)C_U8_TDTWS810D_MIN_IF_GAIN_SET;
    pEUCtx->m_aucPara[10] = (U8)C_U8_TDTWS810D_MAX_RF_GAIN_SET;
    pEUCtx->m_aucPara[11] = (U8)C_U8_TDTWS810D_MIN_RF_GAIN_SET;
    pEUCtx->m_aucPara[12] = (U8)C_U8_TDTWS810D_IF_GAIN_RANGE;
    pEUCtx->m_aucPara[13] = (U8)C_U8_TDTWS810D_RF_GAIN_RANGE;
    pEUCtx->m_aucPara[14] = C_TDTWS810D_POWER_CALIBRATE;

    pTdtws810dEqNormal[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQTdtws810dBG;
    pTdtws810dEqNormal[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQTdtws810dDK;
    pTdtws810dEqNormal[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQTdtws810dI;
    pTdtws810dEqNormal[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQTdtws810dL;
#ifdef CC_MT5363	
    pTdtws810dEqNormal[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQTdtws810dL1;
#else
    pTdtws810dEqNormal[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQTdtws810dL;
#endif
    pTdtws810dEqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQTdtws810dBGWeak;
    pTdtws810dEqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQTdtws810dDKWeak;
    pTdtws810dEqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQTdtws810dIWeak;
    pTdtws810dEqWeak[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQTdtws810dLWeak;
#ifdef CC_MT5363	
    pTdtws810dEqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQTdtws810dL1Weak;
#else
    pTdtws810dEqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQTdtws810dLWeak;
#endif

 //  ucLastMode = MOD_DVBT;

    mcSHOW_USER_MSG(("Tuner TDTWS810D init done!\n"));
}


static VOID vLNA2PinGpioCtrl(BOOL LNA)
{
  UINT32 GPIONum_LNA_SWITCH = DRVCUST_OptGet(eTunerCtrlLna);
  fgLNA = LNA;

  if (LNA)
        {
            mcSHOW_DBG_MSG(("LNA tuner pin2 (GPIO%d)->1\n", GPIONum_LNA_SWITCH));
            GPIO_SetOut(GPIONum_LNA_SWITCH, GPIO_OUTPUT_ON); // LNA ON           
        }
  else
        {
            mcSHOW_DBG_MSG(("LNA tuner pin2 (GPIO%d)->0\n", GPIONum_LNA_SWITCH));
            GPIO_SetOut(GPIONum_LNA_SWITCH, GPIO_OUTPUT_OFF); // LNA OFF
        }		
}

 STATIC void  TDTWS810D_SetSawBw(ITUNER_CTX_T * pTunerCtx, UINT8 sawbw)
 {
   pTunerCtx->specific_member.eu_ctx.m_SAW_BW = sawbw;

   mcSHOW_DRVAPI_MSG(("TDTWS810D_SetSawBw = %d\n", sawbw));
   
 }
 S16 TDTWS810D_TunerSetFreq(ITUNER_CTX_T *pTCtx, PARAM_SETFREQ_T* param)
{
     U32 Freq = param->Freq; // transfer to video freq
     U8  AutoSearch = param->fgAutoSearch;
     U8  Mode = param->Modulation;
	
	UCHAR uc_data;
	S16 ucTunerLockConfCnt;
	UCHAR TunerS[5];
	U32 Lo;
	U16 Ndivider, CB1, CB2, BB; 
  #ifdef CC_MT5363
     UCHAR u1IfAgc[2];
  #endif
	S16 ii, jj;
	
	SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);
	 pTCtx->u4RF_Freq = param->Freq;   // add by liuqu,20091124,[DTV00143315]
   
    if(( Mode == MOD_DVBT) ||( Mode == MOD_DVBC))//digital reception
    {

	    Lo = Freq + C_TDTWS810D_IF_FREQUENCY; // kHz
	    //Ndivider = (U16) ( ((Lo * 1000) / DTOS403_LO_DIVIDER_STEP) + 0.5 );
	  
	    Ndivider = (U16) ( (Lo*1000+C_TDTWS810D_LO_DIVIDER_STEP/2) / C_TDTWS810D_LO_DIVIDER_STEP);
	    // Conver Freq to unit of MHz
	    Freq = Freq/1000;
	
	    if (Freq < 50 || Freq > 859)
	    {
	        mcSHOW_DBG_MSG(("Out of range for LO!\n"));
	        //return (+1);
	    }
	    
	    if (Freq <= 165)       
		{
			BB = 0x81;
		}
		else if (Freq <= 300)  
		{        
			BB = 0x82;
		}
		else if (Freq <= 449)  
		{        
			BB = 0x86;
		}
		else if (Freq <= 670)  
		{        
			BB = 0x8C;
		}
		else                 
	    {
			BB = 0xCC;
		}

	     CB1 = 0x90;
            CB2 = 0xe1;
			
                                                                                    
	    if (pTunerCtx->m_SAW_BW == SAW_BW_8M)                 //Nov.26.2008 spec
		    BB=BB |0x04; //Saw_BW =8M 
		else
		{
		//  TRACE("kk\n",pTunerCtx->m_SAW_BW);
              //   TRACE("%d\n",BB);
		   BB= BB & 0xFB; //Saw_BW = 7M
              //   TRACE("%d\n",BB);
		}
             //  TRACE("%d\n",BB);
		// TRACE("%d\n",pTunerCtx->m_SAW_BW);


	 //merge TOP setting
	   CB1 = (CB1&0XC7)|((pTunerCtx->m_aucPara[0]&0x07)<<3);  // 0x **xx x***
	   CB2 = (CB2&0XFD)|((pTunerCtx->m_aucPara[0]&0X08)>>2); // 0X **** **x*
	     
            vLNA2PinGpioCtrl(0);	//LNA OFF, Pin2=1
	    mcSHOW_DRVAPI_MSG(("LNA OFF!!\n"));
	    /* write TDA9886 to set RFAGC & SAW_BW */
// move to setIFDemod
/*		TunerS[0] = (UCHAR) 0x00; 
		TunerS[1] = (UCHAR) B_Data; 
		TunerS[2] = (UCHAR) 0x70; 
		TunerS[3] = (UCHAR) 0x09;
	
		//if (RegSetRegisters(0x86, TunerS[0], 3, &TunerS[1]) < 0)	//Set RFAGC control for digital mode
		if (RegSetRegistersTuner(0x86, 4, &TunerS[0]) < 0)
		{
	        //TRACE(("\nTunerSet() failed!\n"));
	        return (-1);
		}
*/	
    }
    else //analog reception
    {
    
	    if( Mode == MOD_SECAM_L1 )
	    {
	    	Lo = Freq + C_TDTWS810D_IF_FREQUENCY_L1; // kHz
	    }
	    else
	    {
	    	Lo = Freq + C_TDTWS810D_IF_FREQUENCY_ANA; // kHz
	    }	
	    Ndivider = (U16) ( (Lo*1000+C_TDTWS810D_LO_DIVIDER_STEP_ANA/2) / C_TDTWS810D_LO_DIVIDER_STEP_ANA);
	    
	    mcSHOW_DBG_MSG(("Freq = %d, Divider ratio = %04X \n",param->Freq, Ndivider));
	    
	    // Conver Freq to unit of MHz
	    Freq = Freq/1000;
	    
	    if (Freq < 45 || Freq > 865)
	    {
	       mcSHOW_DBG_MSG(("Out of range for LO!\n"));
        //  return (+1);    // liuqu,20090910, can not returen 1 for 47.25MHZ channel loss!
	    }
	    
	    if (Freq <= 165)       
		{
			BB = 0x01;
		}
		else if (Freq <= 449)  
		{        
			BB = 0x02;
		}

		//else if (Freq <= 670)  
		//{        
			//BB = 0x88;
		//}
		else                 
	    {
			BB = 0x08;
		}

             // set BW =8M in !PAL BG mode.
		if(Mode!=MOD_PAL_BG)
		{
                   BB|=0x04;
		}
		

            	mcSHOW_DRVAPI_MSG(("Set ATD control RFAGC!\n"));            
	        CB1 = 0xbD;

            CB2 = 0xe3;

	       vLNA2PinGpioCtrl(0);	//LNA OFF, Pin2=1
	       mcSHOW_DRVAPI_MSG(("LNA OFF!!\n"));
	 //merge TOP setting
	//   CB1 = (CB1&0XC7)|((pTunerCtx->m_aucPara[0]&0x07)<<3);  // 0x **xx x***
	//   CB2 = (CB2&0XFD)|((pTunerCtx->m_aucPara[0]&0X08)>>2); // 0X **** **x*
    
    }

	//BB |= 0x04;   //LNA should always turn on,liuqu,20080825
    	
    // LO settings
    TunerS[0] = (UCHAR) 0x7F & (Ndivider >> 8);
    TunerS[1] = (UCHAR) 0xFF & Ndivider;
    TunerS[2] = (UCHAR) CB1;  
    TunerS[3] = (UCHAR) BB;
    TunerS[4] = (UCHAR) CB2; 

    for (jj=0; jj<2; jj++)
    {

//  send this data 
    //if (RegSetRegisters(pTunerCtx->m_Address, TunerS[0], 4, &TunerS[1]) < 0)
	//    if (RegSetRegistersTuner(pTunerCtx->m_Address, 5, &TunerS[0]) < 0)
	 if (ICtrlBus_I2cTunerWrite(C_TDTWS810D_I2C_CLK_DIV, pTCtx->I2cAddress, TunerS, 5))
	    {
	        //TRACE(("\nTunerSet() failed!\n"));
	     //   ucLastMode=Mode;
	        return (-1);
	    }
	
		mcDELAY(10);
		// wait PLL lock
	//#ifndef C_DEMO_PROGRAM
		ucTunerLockConfCnt = 0;
		for (ii=0; ii<20; ii++)
		//for (ii=0; ii<ccMAX_WAIT_TUNER_TIME; ii++)
		{
			//RegGetRegisters(pTunerCtx->m_Address, (pTunerCtx->m_Address) | (0x01), 1, &uc_data);
			// Charles_060425, test I2C
		      //	I2CRead(pTunerCtx->m_Address, 1, &uc_data);
			 ICtrlBus_I2cTunerRead(C_TDTWS810D_I2C_CLK_DIV, pTCtx->I2cAddress, &uc_data, 1);
			if (mcGET_FIELD(uc_data, 0x40, 6))  // if PLL lock
				ucTunerLockConfCnt++;
			else
				ucTunerLockConfCnt = 0;
	                
	                mcDELAY(5);
	                
			if (ucTunerLockConfCnt>ccTUNER_LOCK_CONF_THRESHOLD)
				break;
								
		}
		ucTunerLockConfCnt = ii;
		//mcDELAY(SOFTREG_MT5131_REG_TUNER_WAIT_LOCK_TIME);
	
		//if(ii < ccMAX_WAIT_TUNER_TIME)
		if(ii < 20)
		{
			//SYSTEMTIME  rCurSysTime;
			//GetLocalTime(&rCurSysTime);         // get current time
			mcSHOW_DRVAPI_MSG(("Freq = %d MHz, the %d-th time setting PLL success, cnt = %d\n" , Freq, jj+1, ucTunerLockConfCnt));	
			break;
		}
		else
		{
			mcSHOW_DRVAPI_MSG(("Freq = %d MHz, the %d-th time setting PLL fail\n" , Freq, jj+1));		
		}

	}

   

     #if 1//for driver

/*********************************/
/*****    Digital IF AGC Part          ****/
/*********************************/		 
    if(( Mode == MOD_DVBT) ||( Mode == MOD_DVBC))//|| (Mode >= MOD_PAL_BG && !AutoSearch) )
    {
        /*** Record IfAgc value before LNA on for BestMux selection ***/
        if(AutoSearch)
        {
                 DEMOD_CTX_T    sDvbtDemodCtx;
                 sDvbtDemodCtx.I2cAddress = 0x82;
        
                 mcDELAY(80);
                 /*** Record IfAgc value for channel scan ***/  //Menghau, 080314
                 //Choose reg bank 1
                 DVBT_GetReg(&sDvbtDemodCtx, 0x1DB, u1IfAgc, 2);
                 pTunerCtx->m_SigLvScan = -(((S8)u1IfAgc[1])*C_TDTWS810D_AGC_IF_SLP_SGN);
                 mcSHOW_DRVAPI_MSG(("IfAgc = %d\n", pTunerCtx->m_SigLvScan));                 
        }


/*********************************/
/*****    LNA Control Part           *******/
/*********************************/		
        if(!AutoSearch)
        	mcDELAY(200);	// Ken, 081103, for ADC value stable time
             // I2CRead(pTunerCtx->m_Address, 1, &uc_data);
             ICtrlBus_I2cTunerRead(C_TDTWS810D_I2C_CLK_DIV, pTCtx->I2cAddress, &uc_data, 1);
			 
	       if( (uc_data&0x07) >0x02)	//LG spec 20090917 change,>2 then LNA off
		{
		    vLNA2PinGpioCtrl(1);	//LNA ON, Pin2=1
		    mcSHOW_DRVAPI_MSG(("LNA ON!!\n"));
		}
		else
		{
                 vLNA2PinGpioCtrl(0);	//LNA OFF, Pin2=1
		    mcSHOW_DRVAPI_MSG(("LNA OFF!!\n"));
		}
    }
/***************************************************/

    else if (Mode >= MOD_PAL_BG)
    {
#ifdef LNA_MONITOR       //Add by Jack Mei,2009/10/20,for atd monitor LNA
        
           if (AutoSearch)
    	    {
             vLNA2PinGpioCtrl(1);	//LNA ON, Pin2=1
             mcSHOW_DRVAPI_MSG(("ATV scan, LNA ON(Monitor)!!\n" ));
    	    }
           else
            {

              vLNA2PinGpioCtrl(0);	//LNA OFF, Pin2=1
              
              mcSHOW_DRVAPI_MSG(("Analog LNA OFF(Monitor)!!\n"));
	
            }

#else  
        if (AutoSearch)
    	 {
             vLNA2PinGpioCtrl(1);	//LNA ON, Pin2=1
             mcSHOW_DRVAPI_MSG(("ATV scan, LNA ON!!\n" ));
    	 }
        else
        {
                mcDELAY(200);	// Ken, 081103, for ADC value stable time
             // I2CRead(pTunerCtx->m_Address, 1, &uc_data);
             ICtrlBus_I2cTunerRead(C_TDTWS810D_I2C_CLK_DIV, pTCtx->I2cAddress, &uc_data, 1);
			 
		if( (uc_data&0x07) >0x02)	//LG spec 20090917 change,>2 then LNA off
		{
		    vLNA2PinGpioCtrl(1);	//LNA ON, Pin2=1
		    mcSHOW_DRVAPI_MSG(("LNA ON!!\n"));
		}
		else
		{
                 vLNA2PinGpioCtrl(0);	//LNA OFF, Pin2=1
		   mcSHOW_DRVAPI_MSG(("LNA OFF!!\n"));
		}

        }
#endif
    }
#endif

	pTunerCtx->m_aucPara[1] = TunerS[0];	//send register value to UI, LC 070102
	pTunerCtx->m_aucPara[15] = TunerS[1];
	pTunerCtx->m_aucPara[16] = TunerS[2];
	pTunerCtx->m_aucPara[17] = TunerS[3];
	pTunerCtx->m_aucPara[18] = TunerS[4];
	pTunerCtx->m_aucPara[19] = uc_data;
    
	mcSHOW_DRVAPI_MSG(("CB1 = %d,BB = %d,CB2 = %d\n", CB1,BB,CB2));
    mcDELAY(10);	// add 10ms delay before bypass is turn off, LC 070613
		   
    return (0);
}


INT16 TDTWS810D_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){

    switch(eOperation){
    case itGetVer:
        {
        CHAR ** RetStr = (CHAR **)pInOutVal;
        *RetStr = TDTWS810D_GetSwVer();
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
		 UCHAR uc_data;
		 UINT8 *ptrWord = (UINT8 *)pInOutVal;
		 int ii,jj;
		 int ucTunerLockConfCnt;
	
		 SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);
		 
		 for(ii = 0; ii < 5; ii++)
			{
			   TunerS[ii]=(UCHAR)(*ptrWord);
			   ptrWord++;
			   mcSHOW_DRVERR_MSG(("The register word is: 0x%x\n",TunerS[ii]));
			   // mcDELAY(C_ENV57S_PLL_POLL_INTERVAL);
			}
           mcSHOW_DRVERR_MSG(("The register words are: 0x%x-0x%x-0x%x-0x%x-0x%x\n",TunerS[0],TunerS[1],TunerS[2],TunerS[3],TunerS[4]));
		          
	      for (jj=0; jj<2; jj++)
    {

      //  send this data 
       
	 if (ICtrlBus_I2cTunerWrite(C_TDTWS810D_I2C_CLK_DIV, pTCtx->I2cAddress, TunerS, 5))
	    {
	        
	        return (-1);
	    }
	              pTunerCtx->m_aucPara[1] = (UCHAR)TunerS[0]; //send register value to UI, LC 070102
                  pTunerCtx->m_aucPara[15] = (UCHAR)TunerS[1];
                  pTunerCtx->m_aucPara[16] = (UCHAR)TunerS[2];
                  pTunerCtx->m_aucPara[17] = (UCHAR)TunerS[3];
                  pTunerCtx->m_aucPara[18] = (UCHAR)TunerS[4]; 
		       mcDELAY(10);
	  
		ucTunerLockConfCnt = 0;
		for (ii=0; ii<20; ii++)
		
		{
		
			 ICtrlBus_I2cTunerRead(C_TDTWS810D_I2C_CLK_DIV, pTCtx->I2cAddress, &uc_data, 1);
			if (mcGET_FIELD(uc_data, 0x40, 6))  // if PLL lock
				ucTunerLockConfCnt++;
			else
				ucTunerLockConfCnt = 0;
	                
	                mcDELAY(5);
	                
			if (ucTunerLockConfCnt>ccTUNER_LOCK_CONF_THRESHOLD)
				break;
								
		}
		ucTunerLockConfCnt = ii;
		
		if(ii < 20)
		{
			
			mcSHOW_DRVAPI_MSG(("the %d-th time setting PLL success, cnt = %d\n" ,  jj+1, ucTunerLockConfCnt));	
			break;
		}
		else
		{
			mcSHOW_DRVAPI_MSG((" the %d-th time setting PLL fail\n" ,  jj+1));		
		}

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
		TDTWS810D_SetSawBw(pTCtx,SetVal);
        break;
    case itSetLNA:
		  {
             
             BOOL SetLna = (BOOL)SetVal;    //Add by Jack Mei,2009/10/20,for atd monitor LNA
			 vLNA2PinGpioCtrl(SetLna);
		
	      }
		break;
    case itGetLNA:
		  {   
              *( BOOL *)pInOutVal=fgLNA ;   //Add by Jack Mei,2009/10/20,for atd monitor LNA
			  
		  }
        break;

    case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pTdtws810dEqNormal[SetVal];
        break;
    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pTdtws810dEqWeak[SetVal];
        break;
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = NULL;
        break;
    case itSetIfMin:
	      {

		   UINT8 subSysId = SetVal;
		   UINT8 *ifMinPtr = (UINT8*)pInOutVal;
           mcSHOW_DBG_MSG(("TDTWS810D TOP Setting\n"));
           switch(subSysId ){
                 case MOD_PAL_BG:
                 case MOD_PAL_I:                    
                 case MOD_PAL_DK:
                      *ifMinPtr = 0xDC;
                     break;
                 case MOD_SECAM_L:
                 case MOD_SECAM_L1:
                      *ifMinPtr = 0xE4;
                     break;
                 default:
                     mcSHOW_DBG_MSG(("TOP No change\n"));
                     break;
                }

	      }
			break;
    default:
        return ITUNER_NOT_SUPPORTED;
    }
    return ITUNER_OK;
}
