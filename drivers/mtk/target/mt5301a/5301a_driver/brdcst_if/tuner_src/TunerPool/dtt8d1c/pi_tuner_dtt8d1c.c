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
*
* Description:
* ------------
*
* Author:
* -------
*    Mei rui ,12.09
*
*************************************************************************/
//PK
//#include "pi_core.h"


#include "tuner_interface_if.h"
#include "pi_def_dvbt.h"
#include "PI_Def.h"
#include "ctrl_bus.h"
#include "x_gpio.h"
#include "pi_dvbt_if.h" 
#include "fe_tuner_common_if.h"
#include "pi_demod_atd.h"
#include "eq_script_dtt8d1c.h"
#include "drvcust_if.h"
// *** specify tuner related parameters here *** //

#define C_DTT8D1C_VERSION        "DTT8D1C v1.00"
// *** Release Note ***
// Ver     Date         Description
// 1.00    2009.04.14   file established

//#define GPIO_AGC_SWITCH     201
//#define GPIO_AnaAGC      0
//#define GPIO_DigiAGC     1
#define C_IFMIN                       0xc6
#define C_DTT8D1C_I2C_DIVIDER       ((U16) 270)

#define C_DTT8D1C_TOP_SET             ((U8)0x20) 
#define C_DTT8D1C_TOP_SET_ANA         ((U8)0x60)

#define C_DTT8D1C_SAW_BW              SAW_BW_8M

#define C_DTT8D1C_FREQ_DBOUND_UPPER   858*1000*1000   
#define C_DTT8D1C_FREQ_DBOUND_LOWER    48*1000*1000
#define C_DTT8D1C_FREQ_DBOUND_UPPER_Ana   858*1000*1000
#define C_DTT8D1C_FREQ_DBOUND_LOWER_Ana    48*1000*1000

#define C_DTT8D1C_LO_ADDRESS         ((U16)  0xc2)
#define C_DTT8D1C_IF_FREQUENCY        ((U16)  36167)  /* kHz */
#define C_DTT8D1C_IF_FREQUENCY_ANA    ((U16)  38900)  /* kHz */
#define C_DTT8D1C_IF_FREQUENCY_L1     ((U16)  32900)  /* kHz */
#define C_DTT8D1C_LO_DIVIDER_STEP     ((U32) 166667) /* Hz */
#define C_DTT8D1C_LO_DIVIDER_STEP_ANA ((U16)  62500) /* Hz */

#define C_DTT8D1C_CHANNEL_SCAN_JUMP_SMALL_STEP ((U16) 250)
#define C_DTT8D1C_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP ((U16) 1500)
#define C_DTT8D1C_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN ((U16) 500)


#define C_DTT8D1C_AGC_IF_SLP           1.0
#define C_DTT8D1C_AGC_IF_INT           0.0
#define C_DTT8D1C_AGC_IF_MAX           0.499  // 0.5 will overflow
#define C_DTT8D1C_AGC_IF_MIN          -0.5
#define C_DTT8D1C_AGC_IF_LVL_MAX       0.14453125  //-90dBm   37/256
#define C_DTT8D1C_AGC_IF_LVL_MIN      -0.140625   //-60dBm    -36/256
#define C_DTT8D1C_AGC_IF_SLP_SGN  (S8)((C_DTT8D1C_AGC_IF_SLP>0)?1:-1)

#define C_DTT8D1C_MAX_IF_GAIN_SET      -0.12
#define C_DTT8D1C_MIN_IF_GAIN_SET      -0.38
#define C_DTT8D1C_MAX_RF_GAIN_SET       cALC_ADC_BIAS *2   // Volt
#define C_DTT8D1C_MIN_RF_GAIN_SET       cALC_ADC_BIAS *2   // Volt
#define C_DTT8D1C_MAX_IF_GAIN_POW      -100    // dBm
#define C_DTT8D1C_MIN_IF_GAIN_POW  -50     // dBm
#define C_DTT8D1C_MAX_RF_GAIN_POW   C_DTT8D1C_MIN_IF_GAIN_POW
#define C_DTT8D1C_MIN_RF_GAIN_POW  -20     // dBm
#define C_DTT8D1C_POWER_CALIBRATE       26     // dBm

#define C_DTT8D1C_AGC_COEF   ((U8)   0x9B)
#define C_DTT8D1C_DEMOD_INPUT_POWER  cIF_TARGET_LEVEL_0CCC

// ********************************************* //

#define C_U8_DTT8D1C_AGC_IF_SLP         (S8)(C_DTT8D1C_AGC_IF_SLP *32 )
#define C_U8_DTT8D1C_AGC_IF_INT         (S8)(C_DTT8D1C_AGC_IF_INT *64 )
#define C_U8_DTT8D1C_AGC_IF_MAX         (S8)(C_DTT8D1C_AGC_IF_MAX *256)
#define C_U8_DTT8D1C_AGC_IF_MIN         (S8)(C_DTT8D1C_AGC_IF_MIN *256)
#define C_U8_DTT8D1C_AGC_IF_LVL_MAX     (S8)(C_DTT8D1C_AGC_IF_LVL_MAX *256)  
#define C_U8_DTT8D1C_AGC_IF_LVL_MIN     (S8)(C_DTT8D1C_AGC_IF_LVL_MIN *256)  

#define C_U8_DTT8D1C_MAX_IF_GAIN_SET (S8)(C_DTT8D1C_MAX_IF_GAIN_SET *256)
#define C_U8_DTT8D1C_MIN_IF_GAIN_SET (S8)(C_DTT8D1C_MIN_IF_GAIN_SET *256)
#define C_U8_DTT8D1C_MAX_RF_GAIN_SET (S8)((C_DTT8D1C_MAX_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_DTT8D1C_MIN_RF_GAIN_SET (S8)((C_DTT8D1C_MIN_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_DTT8D1C_IF_GAIN_RANGE      (S8)(C_DTT8D1C_MIN_IF_GAIN_POW - C_DTT8D1C_MAX_IF_GAIN_POW)
#define C_U8_DTT8D1C_RF_GAIN_RANGE      (S8)(C_DTT8D1C_MIN_RF_GAIN_POW - C_DTT8D1C_MAX_RF_GAIN_POW)

STATIC UINT8* pDtt8d1cEqNormal[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pDtt8d1cEqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
//STATIC UINT8  u1PreMode = 0xFF;
//----------------------------------------------------------------------------------------------
 void  DTT8D1C_TunerInit(ITUNER_CTX_T* pTCtx);

 STATIC void  DTT8D1C_SetSawBw(ITUNER_CTX_T * pTunerCtx, UINT8 sawbw)
 {
   pTunerCtx->specific_member.eu_ctx.m_SAW_BW = sawbw;
 }

  //---------------------------------------------------------------------------------------------
 STATIC CHAR* DTT8D1C_TunerGetVer(void)
 {
  return ((CHAR*)C_DTT8D1C_VERSION);
}

FUNC_EXPORT void DTT8D1C_TunerInit(ITUNER_CTX_T *pTCtx)
{

    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);
    
    psTunerCtx->I2cAddress = C_DTT8D1C_LO_ADDRESS;
    psTunerCtx->u2IF_Freq = C_DTT8D1C_IF_FREQUENCY;
    psTunerCtx->u4RF_Freq = 0;
    //Justin TODO
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_DRO|MASK_PATCH_SENS);

    // AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x2f; //EC_code_d9930: Change 0x151 value for EU new BPF
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
   // psTunerCtx->sAtdAgcPara.AgcIfMin  = 0xe3; //EC_code_d9930: Change 0x158 value for EU new BPF
    psTunerCtx->sAtdAgcPara.AgcIfMin  = C_IFMIN;
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B; //EC_code_d9930: Change 0x15F value for EU new BPF

    pEUCtx->m_SAW_BW  = C_DTT8D1C_SAW_BW;
    pEUCtx->m_Ana_Top = C_DTT8D1C_TOP_SET_ANA;
    pEUCtx->m_s4FreqBoundUpper = C_DTT8D1C_FREQ_DBOUND_UPPER;
    pEUCtx->m_s4FreqBoundLower = C_DTT8D1C_FREQ_DBOUND_LOWER;
    pEUCtx->m_s4AnaFreqBoundUpper = C_DTT8D1C_FREQ_DBOUND_UPPER_Ana;
    pEUCtx->m_s4AnaFreqBoundLower = C_DTT8D1C_FREQ_DBOUND_LOWER_Ana;
    
    pEUCtx->m_Small_Step = C_DTT8D1C_CHANNEL_SCAN_JUMP_SMALL_STEP;
    pEUCtx->m_Middle_Step_Up = C_DTT8D1C_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP;
    pEUCtx->m_Middle_Step_Down = C_DTT8D1C_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN;
    
    // add by JC, 081215, for Signal Level display
    pEUCtx->m_ifAgc_lvl_max = (U8)C_U8_DTT8D1C_AGC_IF_LVL_MAX;
    pEUCtx->m_ifAgc_lvl_min = (U8)C_U8_DTT8D1C_AGC_IF_LVL_MIN;
    
    pEUCtx->m_SigLvTh = 0;//C_TDTWS710D_SIGNAL_LVL_TH;
    pEUCtx->m_SigLvScan = 0;
    
    pEUCtx->m_aucPara[0] = C_DTT8D1C_TOP_SET;
    pEUCtx->m_aucPara[1] = 0;
    
    pEUCtx->m_aucPara[2] = (U8)C_U8_DTT8D1C_AGC_IF_SLP;
    pEUCtx->m_aucPara[3] = (U8)C_U8_DTT8D1C_AGC_IF_INT;
    pEUCtx->m_aucPara[4] = (U8)C_U8_DTT8D1C_AGC_IF_MAX;
    pEUCtx->m_aucPara[5] = (U8)C_U8_DTT8D1C_AGC_IF_MIN;
    
    pEUCtx->m_aucPara[6] = C_DTT8D1C_AGC_COEF;
    pEUCtx->m_aucPara[7] = C_DTT8D1C_DEMOD_INPUT_POWER;
    
    pEUCtx->m_aucPara[ 8] = (U8)C_U8_DTT8D1C_MAX_IF_GAIN_SET;
    pEUCtx->m_aucPara[ 9] = (U8)C_U8_DTT8D1C_MIN_IF_GAIN_SET;
    pEUCtx->m_aucPara[10] = (U8)C_U8_DTT8D1C_MAX_RF_GAIN_SET;
    pEUCtx->m_aucPara[11] = (U8)C_U8_DTT8D1C_MIN_RF_GAIN_SET;
    pEUCtx->m_aucPara[12] = (U8)C_U8_DTT8D1C_IF_GAIN_RANGE;
    pEUCtx->m_aucPara[13] = (U8)C_U8_DTT8D1C_RF_GAIN_RANGE;
    pEUCtx->m_aucPara[14] = C_DTT8D1C_POWER_CALIBRATE;

    pDtt8d1cEqNormal[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQDtt8d1cBG;
    pDtt8d1cEqNormal[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQDtt8d1cDK;
    pDtt8d1cEqNormal[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQDtt8d1cI;
    pDtt8d1cEqNormal[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQDtt8d1cL;
    pDtt8d1cEqNormal[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQDtt8d1cL1;
	/*
    pDtt8d1cEqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQDtt8d1cBGWeak;
    pDtt8d1cEqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQDtt8d1cDKWeak;
    pDtt8d1cEqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQDtt8d1cIWeak;
    pDtt8d1cEqWeak[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQDtt8d1cLWeak;
    pDtt8d1cEqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQDtt8d1cLWeak;
	*/
    pDtt8d1cEqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQDtt8d1cBG;
    pDtt8d1cEqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQDtt8d1cDK;
    pDtt8d1cEqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQDtt8d1cI;
    pDtt8d1cEqWeak[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQDtt8d1cL;
    pDtt8d1cEqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQDtt8d1cL1;

    mcSHOW_USER_MSG(("Tuner DTT8D1C init done!\n"));

}


 INT16 DTT8D1C_TunerSetFreq(
    ITUNER_CTX_T *pTunerCtx,
    PARAM_SETFREQ_T* param
  )
{
  UCHAR uc_data;
  S16 ucTunerLockConfCnt;
  UCHAR TunerS[5];
  U32 Lo,Freq;
  U16 Ndivider, CB1,CB2,BB,AB;
  S16 ii, jj;
  UCHAR u1IfAgc[2];
  UINT8 AutoSearch = param->fgAutoSearch;
  UINT8 Mode = param->Modulation;


    SPECIFIC_MEMBER_EU_CTX* pEUCtx= &( pTunerCtx->specific_member.eu_ctx);

    pTunerCtx->u4RF_Freq = param->Freq;
    Freq = param->Freq;  
    pTunerCtx->I2cAddress = C_DTT8D1C_LO_ADDRESS;


     if( Mode == MOD_DVBT) //digital reception
    {
	Lo = Freq + C_DTT8D1C_IF_FREQUENCY;
	Ndivider = (U16)( (Lo*1000+C_DTT8D1C_LO_DIVIDER_STEP/2) / C_DTT8D1C_LO_DIVIDER_STEP );
        
        mcSHOW_DRVAPI_MSG(("Freq = %d, Divider ratio = %04X \n",pTunerCtx->u4RF_Freq, Ndivider));
        
        // Conver Freq to unit of MHz
        Freq = Freq/1000;
        Lo = Lo/1000;
        
        if (Freq < (C_DTT8D1C_FREQ_DBOUND_LOWER/1000/1000) || 
	        Freq > (C_DTT8D1C_FREQ_DBOUND_UPPER/1000/1000)  )
	    {
	        mcSHOW_DRVAPI_MSG(("Out of range for LO!\n"));
	        //return (+1);
	    }
    	
        //Band Selection
        if (Freq < 160)   // low band, VHF low
        {
           BB = 0x01;
        }
        else if (Freq < 446)  // mid band, VHF high
        {
           BB = 0x02;
        }
        else   // high band, UHF
        {
           BB = 0x04;
        }
        
		
		if (Lo < 180)
        {
            CB1 = 0xBC;  // Low CP, 166.67KHz step
        }
        else if (Lo < 197)
        {
            CB1 = 0xF4;  // Medium CP, 166.67KHz step
        }
        else if (Lo < 366)
        {
            CB1 = 0xBC;  // Low CP, 166.67KHz step
        }
        else if (Lo < 484)
        {
            CB1 = 0xF4;  // Medium CP, 166.67KHz step
        }   
        else if (Lo < 646)
        {
            CB1 = 0xBC;  // Low CP, 166.67KHz step
        }
        else
        {
            CB1 = 0xF4;  // High CP, 166.67KHz step
        }
        
        if (pEUCtx->m_SAW_BW == SAW_BW_8M)
	    BB = (BB | 0x08); 
	
        AB = (pEUCtx->m_aucPara[0] & 0x70); // 0xxx0000
	    CB2 = ((CB1 & 0xDF) | 0x18);
	
	  if(!AutoSearch) 
      {
      AB= AB & 0x7F; // set ATC = 1'b0 
	  mcSHOW_DRVAPI_MSG(("atc=0\n"));
      }
    }
	
    else //analog reception
    
	{	   
		   if( Mode == MOD_SECAM_L1 )
		   {
			   Lo = Freq + C_DTT8D1C_IF_FREQUENCY_L1; // kHz
			   Ndivider = (U16) ( (Lo*1000+C_DTT8D1C_LO_DIVIDER_STEP_ANA/2) / C_DTT8D1C_LO_DIVIDER_STEP_ANA);
			   mcSHOW_DBG_MSG(("Freq = %d, Divider ratio = %04X PLL set freq=%d kHz\n",pTunerCtx->u4RF_Freq, Ndivider,(Ndivider*C_DTT8D1C_LO_DIVIDER_STEP_ANA/1000-C_DTT8D1C_IF_FREQUENCY_L1)));
		   }
		   else
		   {
			   Lo = Freq + C_DTT8D1C_IF_FREQUENCY_ANA; // kHz
			   Ndivider = (U16) ( (Lo*1000+C_DTT8D1C_LO_DIVIDER_STEP_ANA/2) / C_DTT8D1C_LO_DIVIDER_STEP_ANA);
			   mcSHOW_DBG_MSG(("Freq = %d, Divider ratio = %04X PLL set freq=%d kHz\n",pTunerCtx->u4RF_Freq, Ndivider,(Ndivider*C_DTT8D1C_LO_DIVIDER_STEP_ANA/1000-C_DTT8D1C_IF_FREQUENCY_ANA)));
		   }
		   
		   // Conver Freq to unit of MHz
		   Freq = Freq/1000;
	
		   if (Freq < 45 || Freq > 874)
		   {
			   mcSHOW_DBG_MSG(("Out of range for LO!\n"));
			   //return (+1);
		   }
		   
		   //Band Selection // SAW default 8MHz
		   if (Freq < 160)	 // low band, VHF low
		   {
			   //BB = 0x01;
			   BB = 0x09;
		   }
		   else if (Freq < 446)  // mid band, VHF high
		   {
			   //BB = 0x02;
			   BB = 0x0A;
		   }
		   else   // high band, UHF
		   {
			   //BB = 0x04;
			   BB = 0x0C;
		   }   
	   
	       if(Mode == MOD_PAL_BG)  // Fix SAW at 7MHz for PAL-B/G
		      BB = (BB & 0xF7);
	 
		   if (Lo < 110000)
		   {
			   CB1 = 0xb6;	// 50uA, 62.25KHz step
		   }
		   else if (Lo < 180000)
		   {
			   CB1 = 0xBE;	// Low CP,125uA, 62.25KHz step
		   }
		   else if (Lo < 197000)
		   {
			   CB1 = 0xF6;	// Medium CP250uA, 62.25KHz step
		   }
		   else if (Lo < 366000)
		   {
			   CB1 = 0xBE;	// Low CP,125uA 62.25KHz step
		   }
		   else if (Lo < 484000)   
		   {
			   CB1 = 0xf6;	// Medium CP,250uA, 62.25KHz step
		   }   
		   else if (Lo < 646000)//533-685		 
		   {
			   CB1 = 0xf6;	// 250uA, 62.25KHz step
		   }
		else if (Lo < 830000)
		{
			  CB1 = 0xf6;	 //250uA
		}
		   else
		   {
			  CB1 = 0XFE; //High cp
		}
		
	   AB = 0x60;  // External RFAGC control
		   //CB1 = 0xF6;  // Medium CP, 62.5KHz step
	   CB2 = ((CB1 & 0xDF) | 0x18);
}
    
    // LO settings
      
		 
    for (jj=0; jj<2; jj++)
    {

        TunerS[0] = (UCHAR) 0x7F & (Ndivider >> 8);
        TunerS[1] = (UCHAR) 0xFF & Ndivider;
        TunerS[2] = (UCHAR) CB1;  
        TunerS[3] = (UCHAR) BB;

		pEUCtx->m_aucPara[1] =  (UCHAR)TunerS[0]; //send register value to UI, LC 070102
        pEUCtx->m_aucPara[15] = (UCHAR)TunerS[1];
        pEUCtx->m_aucPara[16] = (UCHAR)TunerS[2];
        pEUCtx->m_aucPara[17] = (UCHAR)TunerS[3];

	    if (ICtrlBus_I2cTunerWrite(C_DTT8D1C_I2C_DIVIDER,pTunerCtx->I2cAddress, TunerS,4) > 0)
        {
          mcSHOW_DRVAPI_MSG(("\nTunerSet() failed!\n"));
		  return (-1);
        }

	   TunerS[0] = (UCHAR) 0x7F & (Ndivider >> 8);
       TunerS[1] = (UCHAR) 0xFF & Ndivider;
       TunerS[2] = (UCHAR) CB2;                     // Set T[2:0] = {011}, CP must be 0
       TunerS[3] = (UCHAR) AB ;             // Set ATC = 1'b1

	   pEUCtx->m_aucPara[18] = (UCHAR)TunerS[2];
       pEUCtx->m_aucPara[19] = (UCHAR)TunerS[3];
      
	   if (ICtrlBus_I2cTunerWrite(C_DTT8D1C_I2C_DIVIDER,pTunerCtx->I2cAddress, TunerS,4) > 0)
       {
          mcSHOW_DRVAPI_MSG(("\nTunerSet() failed!\n"));
		  return (-1);
       }
	   
      ucTunerLockConfCnt = 0;
      for (ii=0; ii<20; ii++)
      {
         mcDELAY(5);// wait PLL lock
         ICtrlBus_I2cTunerRead(C_DTT8D1C_I2C_DIVIDER,pTunerCtx->I2cAddress, &uc_data,1);
         if (mcGET_FIELD(uc_data, 0x40, 6))  // if PLL lock
           ucTunerLockConfCnt++;
         else
          ucTunerLockConfCnt = 0;
         if (ucTunerLockConfCnt>ccTUNER_LOCK_CONF_THRESHOLD)
           break;
       }
        ucTunerLockConfCnt = ii;
      if(ii < 20)
      {
         mcSHOW_DRVAPI_MSG(("Freq = %d MHz, the %d-th time setting PLL success, cnt = %d\n" , Freq, jj+1, ucTunerLockConfCnt));
         break;
      }
      else
      {
        mcSHOW_DRVAPI_MSG(("Freq = %d MHz, the %d-th time setting PLL fail\n" , Freq, jj+1));
      }

  }
  
    if( (Mode ==MOD_DVBT ) && AutoSearch)
    {
      DEMOD_CTX_T    sDvbtDemodCtx;
      sDvbtDemodCtx.I2cAddress = 0x82;
      mcDELAY(80);
     /*** Record IfAgc value for channel scan ***/  //Menghau, 080314
      DVBT_GetReg(&sDvbtDemodCtx, 0x1DB, u1IfAgc, 2);
      pEUCtx->m_SigLvScan = -(((S8)u1IfAgc[1])*C_DTT8D1C_AGC_IF_SLP_SGN);
      mcSHOW_DRVAPI_MSG(("IfAgc = %d\n", pEUCtx->m_SigLvScan));
    /*** Record IfAgc value before LNA on for BestMux selection ***/
    }
    return (0);
}

//---------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------
INT16 DTT8D1C_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal)
{

    switch (eOperation)
    {
    case itGetVer:
                 {
                    CHAR ** RetStr=(CHAR **)pInOutVal;
                    *RetStr=DTT8D1C_TunerGetVer();
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
			   //mcSHOW_DRVERR_MSG(("The register word is: 0x%x\n",TunerS[ii]));
			   // mcDELAY(C_ENV57S_PLL_POLL_INTERVAL);
			}
           mcSHOW_DRVERR_MSG(("The register words are: 0x%x-0x%x-0x%x-0x%x-0x%x\n",TunerS[0],TunerS[1],TunerS[2],TunerS[3],TunerS[4]));
		
	      
		  for (jj=0; jj<2; jj++)
			  {
				if (ICtrlBus_I2cTunerWrite(C_DTT8D1C_I2C_DIVIDER,pTCtx->I2cAddress, TunerS,5) > 0)
				{
					return (-1);
				}

				  pTunerCtx->m_aucPara[1] = (UCHAR)TunerS[0]; //send register value to UI, LC 070102
                  pTunerCtx->m_aucPara[15] = (UCHAR)TunerS[1];
                  pTunerCtx->m_aucPara[16] = (UCHAR)TunerS[2];
                  pTunerCtx->m_aucPara[17] = (UCHAR)TunerS[3];
                  pTunerCtx->m_aucPara[18] = (UCHAR)TunerS[4]; 
			     ucTunerLockConfCnt = 0;
			  for (ii=0; ii<20; ii++)
			  {
			  mcDELAY(5);// wait PLL lock
				ICtrlBus_I2cTunerRead(C_DTT8D1C_I2C_DIVIDER,pTCtx->I2cAddress, &uc_data,1);
				if (mcGET_FIELD(uc_data, 0x40, 6))	// if PLL lock
				  ucTunerLockConfCnt++;
				else
				  ucTunerLockConfCnt = 0;
				if (ucTunerLockConfCnt>ccTUNER_LOCK_CONF_THRESHOLD)
				  break;
			  }
			  ucTunerLockConfCnt = ii;
			  if(ii < 20)
			  {
				mcSHOW_DRVAPI_MSG(("the %d-th time setting PLL success, cnt = %d\n" , jj+1, ucTunerLockConfCnt));
				break;
			  }
			  else
			  {
				mcSHOW_DRVAPI_MSG(("the %d-th time setting PLL fail\n" , jj+1));
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
    case itSetIfDemod:
    break;
    case itSetSawBw:
                 DTT8D1C_SetSawBw(pTCtx, SetVal);
    break;
    case itSetLNA:
    break;

	case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pDtt8d1cEqNormal[SetVal];
        break;
    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pDtt8d1cEqWeak[SetVal];
        break;
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = NULL;
        break;

    case itSetIfMin:
		 {
            UINT8 subSysId = SetVal;
			UINT8 *ifMinPtr = (UINT8*)pInOutVal;

			mcSHOW_DBG_MSG(("NuTune_DTT8D1C TOP Setting\n"));
            switch(subSysId){
                case MOD_PAL_BG:
                case MOD_PAL_I:                    
                case MOD_PAL_DK:
                   // *ifMinPtr = 0xE3;
					 *ifMinPtr = C_IFMIN;
                    break;
                case MOD_SECAM_L:
                case MOD_SECAM_L1:
                     //*ifMinPtr = 0xF3;
					  *ifMinPtr = C_IFMIN;
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
