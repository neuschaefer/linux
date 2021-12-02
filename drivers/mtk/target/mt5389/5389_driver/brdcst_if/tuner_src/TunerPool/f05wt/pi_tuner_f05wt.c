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
 * $RCSfile: pi_tuner_f05wt.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_f05wt.c
 *  TCL  F05WT tuner driver.
 */


#include "tuner_interface_if.h"
#include "pi_def_dvbt.h"
#include "PI_Def.h"
#include "ctrl_bus.h"
#include "pi_dvbt_if.h"  //add by liuqu,20090424
#include "pi_demod_atd.h"
#include "eq_script_f05wt.h"


// *** Release Note ***
// Ver     Date         Description
// 1.00    2009.03.25   file established
#define C_F05WT_VERSION				"F05WT 2010-01-01\n 2011-04-07\n v1.0"
#define C_F05WT_PLL_POLL_INTERVAL   10   // to solve SLT thread UART overflow issue, 090601, Ken
#define C_F05WT_PLL_TIMEOUT         100
#define C_F05WT_PLL_TIMEOUT_CNT     C_F05WT_PLL_TIMEOUT/C_F05WT_PLL_POLL_INTERVAL
#define C_F05WT_I2C_CLK_DIV         0x100
#define C_F05WT_PLL_LOCK_BIT        6

#define C_F05WT_TOP_SET             ((U8)   0x00) 
//#define C_F05WT_TOP_SET_DVBC        ((U8)   0x04) 
#define C_F05WT_TOP_SET_ANA         ((U8)   0x06) 
#define C_F05WT_SAW_BW              SAW_BW_8M 

//#define C_F05WT_FREQ_DBOUND_UPPER   858*1000*1000 
#define C_F05WT_FREQ_DBOUND_UPPER   859*1000*1000   
#define C_F05WT_FREQ_DBOUND_LOWER    48*1000*1000 
#define C_F05WT_FREQ_DBOUND_UPPER_Ana     865*1000*1000
#define C_F05WT_FREQ_DBOUND_LOWER_Ana    45*1000*1000

#define C_F05WT_LO_ADDRESS         ((U16)  0xC2)
#define C_F05WT_IF_FREQUENCY        ((U16)  36167)  /* kHz */
#define C_F05WT_IF_FREQUENCY_ANA    ((U16)  38000)  /* kHz */
#define C_F05WT_IF_FREQUENCY_L1     ((U16)  32900)  /* kHz */
#define C_F05WT_LO_DIVIDER_STEP     ((U32)  62500) /* Hz */
#define C_F05WT_LO_DIVIDER_STEP_ANA ((U16)  62500) /* Hz */


#define C_F05WT_CHANNEL_SCAN_JUMP_SMALL_STEP ((U16) 250)
#define C_F05WT_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP ((U16) 1500)
#define C_F05WT_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN ((U16) 500)


#define C_F05WT_AGC_IF_SLP           1.0
#define C_F05WT_AGC_IF_INT           0.0
#define C_F05WT_AGC_IF_MAX           0.499  // 0.5 will overflow
#define C_F05WT_AGC_IF_MIN          -0.5
#define C_F05WT_AGC_IF_SLP_SGN  (S8)((C_F05WT_AGC_IF_SLP>0)?1:-1) 
#define C_F05WT_AGC_IF_LVL_MAX      0.4    
#define C_F05WT_AGC_IF_LVL_MIN      0.5   


#define C_F05WT_MAX_IF_GAIN_SET       -0.12   
#define C_F05WT_MIN_IF_GAIN_SET       -0.38   
#define C_F05WT_MAX_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_F05WT_MIN_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_F05WT_MAX_IF_GAIN_POW       -100    // dBm
#define C_F05WT_MIN_IF_GAIN_POW		-50       // dBm
#define C_F05WT_MAX_RF_GAIN_POW		 C_F05WT_MIN_IF_GAIN_POW
#define C_F05WT_MIN_RF_GAIN_POW		-20     // dBm
#define C_F05WT_POWER_CALIBRATE        26     // dBm

#define C_F05WT_AGC_COEF			((U8)   0xBD)
#define C_F05WT_DEMOD_INPUT_POWER	cIF_TARGET_LEVEL_0CCC

// ********************************************* //

#define C_U8_F05WT_AGC_IF_SLP         (S8)(C_F05WT_AGC_IF_SLP *32 )
#define C_U8_F05WT_AGC_IF_INT         (S8)(C_F05WT_AGC_IF_INT *64 )
#define C_U8_F05WT_AGC_IF_MAX         (S8)(C_F05WT_AGC_IF_MAX *256)
#define C_U8_F05WT_AGC_IF_MIN         (S8)(C_F05WT_AGC_IF_MIN *256)
#define C_U8_F05WT_AGC_IF_LVL_MAX      (U8)(C_F05WT_AGC_IF_LVL_MAX *256)
#define C_U8_F05WT_AGC_IF_LVL_MIN      (U8)(C_F05WT_AGC_IF_LVL_MIN *256)

#define C_U8_F05WT_MAX_IF_GAIN_SET	(S8)(C_F05WT_MAX_IF_GAIN_SET *256)
#define C_U8_F05WT_MIN_IF_GAIN_SET	(S8)(C_F05WT_MIN_IF_GAIN_SET *256)
#define C_U8_F05WT_MAX_RF_GAIN_SET	(S8)((C_F05WT_MAX_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_F05WT_MIN_RF_GAIN_SET	(S8)((C_F05WT_MIN_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_F05WT_IF_GAIN_RANGE      (S8)(C_F05WT_MIN_IF_GAIN_POW - C_F05WT_MAX_IF_GAIN_POW)
#define C_U8_F05WT_RF_GAIN_RANGE      (S8)(C_F05WT_MIN_RF_GAIN_POW - C_F05WT_MAX_RF_GAIN_POW)

STATIC UINT8* pF05wtEqNormal[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pF05wtEqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
S16 F05WT_SetIFDemod(ITUNER_CTX_T *pTCtx, U8 SubSysIdx);


///////////////////////////////////
// TCL  F05WT driver
///////////////////////////////////

CHAR* F05WT_GetSwVer(void)
{
	return ((CHAR*)C_F05WT_VERSION);
}

VOID F05WT_TunerInit(ITUNER_CTX_T *pTCtx)
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);
    
    psTunerCtx->I2cAddress = C_F05WT_LO_ADDRESS;
    psTunerCtx->u2IF_Freq = C_F05WT_IF_FREQUENCY;
	psTunerCtx->u2IF_Freq_A=C_F05WT_IF_FREQUENCY_ANA;
    psTunerCtx->u4RF_Freq = 0;
    //Justin TODO
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_DRO|MASK_PATCH_SENS);
    psTunerCtx->fgRFAGC = TRUE;
    // AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x2F;
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0xDE;
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B;

    pEUCtx->m_SAW_BW  = C_F05WT_SAW_BW;
    pEUCtx->m_Ana_Top = C_F05WT_TOP_SET_ANA;
    pEUCtx->m_s4FreqBoundUpper = C_F05WT_FREQ_DBOUND_UPPER;
    pEUCtx->m_s4FreqBoundLower = C_F05WT_FREQ_DBOUND_LOWER;
    pEUCtx->m_s4AnaFreqBoundUpper = C_F05WT_FREQ_DBOUND_UPPER_Ana;
    pEUCtx->m_s4AnaFreqBoundLower = C_F05WT_FREQ_DBOUND_LOWER_Ana;
    
    pEUCtx->m_Small_Step = C_F05WT_CHANNEL_SCAN_JUMP_SMALL_STEP;
    pEUCtx->m_Middle_Step_Up = C_F05WT_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP;
    pEUCtx->m_Middle_Step_Down = C_F05WT_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN;
    
    // add by JC, 081215, for Signal Level display
    pEUCtx->m_ifAgc_lvl_max = (U8)C_U8_F05WT_AGC_IF_LVL_MAX;
    pEUCtx->m_ifAgc_lvl_min = (U8)C_U8_F05WT_AGC_IF_LVL_MIN;
    
    pEUCtx->m_SigLvTh = 0;//C_TDTWS710D_SIGNAL_LVL_TH;
    pEUCtx->m_SigLvScan = 0;
    
    pEUCtx->m_aucPara[0] = C_F05WT_TOP_SET;
    pEUCtx->m_aucPara[1] = 0;
    
    pEUCtx->m_aucPara[2] = (U8)C_U8_F05WT_AGC_IF_SLP;
    pEUCtx->m_aucPara[3] = (U8)C_U8_F05WT_AGC_IF_INT;
    pEUCtx->m_aucPara[4] = (U8)C_U8_F05WT_AGC_IF_MAX;
    pEUCtx->m_aucPara[5] = (U8)C_U8_F05WT_AGC_IF_MIN;
    
    pEUCtx->m_aucPara[6] = C_F05WT_AGC_COEF;
    pEUCtx->m_aucPara[7] = C_F05WT_DEMOD_INPUT_POWER;
    
    pEUCtx->m_aucPara[ 8] = (U8)C_U8_F05WT_MAX_IF_GAIN_SET;
    pEUCtx->m_aucPara[ 9] = (U8)C_U8_F05WT_MIN_IF_GAIN_SET;
    pEUCtx->m_aucPara[10] = (U8)C_U8_F05WT_MAX_RF_GAIN_SET;
    pEUCtx->m_aucPara[11] = (U8)C_U8_F05WT_MIN_RF_GAIN_SET;
    pEUCtx->m_aucPara[12] = (U8)C_U8_F05WT_IF_GAIN_RANGE;
    pEUCtx->m_aucPara[13] = (U8)C_U8_F05WT_RF_GAIN_RANGE;
    pEUCtx->m_aucPara[14] = C_F05WT_POWER_CALIBRATE;
    pEUCtx->m_aucPara[21] = 0x07;

    //pF05wtEqNormal[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQF05wtBG;
    pF05wtEqNormal[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQF05wtDK;
    pF05wtEqNormal[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQF05wtI;
  // pF05wtEqNormal[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQF05wtL;
//#ifdef CC_MT5363	
   // pF05wtEqNormal[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQF05wtL;
//#else
   // pF05wtEqNormal[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQF05wtL;
//#endif
   // pF05wtEqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQF05wtBG;
    pF05wtEqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQF05wtDK;
    pF05wtEqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQF05wtI;
    //pF05wtEqWeak[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQF05wtL;
//#ifdef CC_MT5363	
  //  pF05wtEqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQF05wtL;
//#else
   //pF05wtEqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQF05wtL;
//#endif

    mcSHOW_USER_MSG(("Tuner F05WT init done!\n"));
}


S16 F05WT_TunerSetFreq(ITUNER_CTX_T *pTCtx, PARAM_SETFREQ_T* param)
{
    U32 Freq = param->Freq; // transfer to video freq
    U8  Mode = param->Modulation;

    UCHAR uc_data;
    UCHAR TunerS[4];
    U32 Lo;
    U16 Ndivider, CB, PB;
    //U16 AGC;
    S16 ii, jj;
    #ifdef CC_MT5363
     U8  AutoSearch = param->fgAutoSearch;
     UCHAR u1IfAgc[2];
    #endif

    SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);

    pTCtx->u4RF_Freq = Freq;

    if( Mode == MOD_DVBT ) //digital reception
    {

        Lo = Freq + C_F05WT_IF_FREQUENCY;
        Ndivider = (U16)( (Lo*1000+C_F05WT_LO_DIVIDER_STEP/2) / C_F05WT_LO_DIVIDER_STEP  );     
        mcSHOW_DRVAPI_MSG(("DVBT DigFreq = %d, Divider ratio = %04X \n",Freq, Ndivider));
        
        // Conver Freq to unit of MHz
        Freq = Freq/1000;
        
        if (Freq < (C_F05WT_FREQ_DBOUND_LOWER/1000/1000) ||
            Freq > (C_F05WT_FREQ_DBOUND_UPPER/1000/1000)  )
        {
            mcSHOW_DRVERR_MSG(("Out of range for LO!\n"));
         
        }
		
		CB = 0XCE;     //CP = 280uA
       		
	    if (Freq < 151)   
	    {
	       PB = 0x01;   
	    }
	    else if (Freq < 431)  
	    {
	       PB = 0x02;     
	    }
	    else   
          PB = 0x08;                           
			
    }
    else
    {
        if( Mode == MOD_SECAM_L1 )
        {
            Lo = Freq + C_F05WT_IF_FREQUENCY_L1; // kHz
            Ndivider = (U16) ( (Lo*1000+C_F05WT_LO_DIVIDER_STEP_ANA/2) / C_F05WT_LO_DIVIDER_STEP_ANA);
            mcSHOW_DBG_MSG(("Freq = %d, Divider ratio = %04X PLL set freq=%d kHz\n",Freq, Ndivider,(Ndivider*C_F05WT_LO_DIVIDER_STEP_ANA/1000-C_F05WT_IF_FREQUENCY_L1)));
        }
        else
        {
            Lo = Freq + pTCtx->u2IF_Freq_A;
			Ndivider = (U16) ( (Lo*1000+C_F05WT_LO_DIVIDER_STEP_ANA/2) / C_F05WT_LO_DIVIDER_STEP_ANA);
            mcSHOW_DBG_MSG(("Freq = %d, Divider ratio = %04X PLL set freq=%d kHz\n",Freq, Ndivider,(Ndivider*C_F05WT_LO_DIVIDER_STEP_ANA/1000-pTCtx->u2IF_Freq_A)));
        }

        if (Freq < C_F05WT_FREQ_DBOUND_LOWER_Ana/1000 || Freq > C_F05WT_FREQ_DBOUND_UPPER_Ana/1000)
        {
            mcSHOW_DRVERR_MSG(("Out of range for LO!\n"));        
        }

        Freq = Freq/1000;

		CB = 0xCE; //CP = 280uA
	    
	    if (Freq < 151)   
	    {
	       PB = 0x01;    
	    }
        else if (Freq < 427)  
	    {
	       PB = 0x02;     
	    }
	    else   
        {
           PB = 0x08;
	    }
		
    }
	  
    for (jj=0; jj<2; jj++)  //retry 2 times while PLL unlock, LC 070702
    {
         // LO settings
        TunerS[0] = (UCHAR) 0x7F & (Ndivider >> 8);
	    TunerS[1] = (UCHAR) 0xFF & Ndivider;
	    TunerS[2] = (UCHAR) CB;  
	    TunerS[3] = (UCHAR) PB;
		   
        if (ICtrlBus_I2cTunerWrite(C_F05WT_I2C_CLK_DIV, pTCtx->I2cAddress, TunerS, 4))
        {
            mcSHOW_HW_MSG(("TunerWrite PLL failed!\n"));
            return (+1);
        }
		mcSHOW_DRVAPI_MSG(("0x%02X-0x%02X-0x%02X-0x%02X\n", TunerS[0], TunerS[1], TunerS[2], TunerS[3]));

        // wait PLL lock
    //ucTunerLockConfCnt = 0;
        for (ii=0; ii<C_F05WT_PLL_TIMEOUT_CNT; ii++)
        {
            if (ICtrlBus_I2cTunerRead(C_F05WT_I2C_CLK_DIV, pTCtx->I2cAddress,  &uc_data, 1))
            {
                mcSHOW_HW_MSG(("Tuner read failed!\n"));
                return (+1);
            }
            // check if PLL lock
            if (mcCHK_BIT1(uc_data, C_F05WT_PLL_LOCK_BIT)) {
                break;
            }
            mcDELAY(C_F05WT_PLL_POLL_INTERVAL);
        }

        if(ii < C_F05WT_PLL_TIMEOUT_CNT) {
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
#ifdef CC_MT5363
    if( (Mode ==MOD_DVBT ) && AutoSearch)
    {
        DEMOD_CTX_T    sDvbtDemodCtx;
        sDvbtDemodCtx.I2cAddress = 0x82;
    
        mcDELAY(80);
         /*** Record IfAgc value for channel scan ***/  //Menghau, 080314
        DVBT_GetReg(&sDvbtDemodCtx, 0x1DB, u1IfAgc, 2);
        pTunerCtx->m_SigLvScan = -(((S8)u1IfAgc[1])*C_F05WT_AGC_IF_SLP_SGN);
        mcSHOW_DRVAPI_MSG(("IfAgc = %d\n", pTunerCtx->m_SigLvScan));
        /*** Record IfAgc value before LNA on for BestMux selection ***/
    }  
#endif 

    pTunerCtx->m_aucPara[1]  = TunerS[0]; //send register value to UI, LC 070102
    pTunerCtx->m_aucPara[15] = TunerS[1];
    pTunerCtx->m_aucPara[16] = TunerS[2];
    pTunerCtx->m_aucPara[17] = TunerS[3];
	pTunerCtx->m_aucPara[18] = TunerS[3];
    pTunerCtx->m_aucPara[19] = uc_data;
    
    return 0;
}


INT16 F05WT_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){

    switch(eOperation){
    case itGetVer:
        {
        CHAR ** RetStr = (CHAR **)pInOutVal;
        *RetStr = F05WT_GetSwVer();
        }
        break;
    case itSetTop:
	    {
          SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);
          pTunerCtx->m_aucPara[0] = SetVal;
	    }
		break;

#if 0
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
					 // mcDELAY(C_F05WT_PLL_POLL_INTERVAL);
			}
           mcSHOW_DRVERR_MSG(("The register words are: 0x%x-0x%x-0x%x-0x%x-0x%x\n",TunerS[0],TunerS[1],TunerS[2],TunerS[3],TunerS[4]));
	 for (j=0; j<2; j++)  //retry 2 times while PLL unlock, LC 070702
       {
   
           if (ICtrlBus_I2cTunerWrite(C_F05WT_I2C_CLK_DIV, pTCtx->I2cAddress, TunerS, 5))    //  send this data
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
   
          for (i=0; i<C_F05WT_PLL_TIMEOUT_CNT; i++)
          {
            if (ICtrlBus_I2cTunerRead(C_F05WT_I2C_CLK_DIV, pTCtx->I2cAddress, &ucPllValue, 1))
            {
              
				mcSHOW_HW_MSG(("Tuner read failed!\n"));
                return (+1);
            }
           
            if (mcCHK_BIT1(ucPllValue, C_F05WT_PLL_LOCK_BIT))   // check if PLL lock
				{
                   pTunerCtx->m_aucPara[19] = ucPllValue;
					break;
                }
            mcDELAY(C_F05WT_PLL_POLL_INTERVAL);
          }

          if(i < C_F05WT_PLL_TIMEOUT_CNT) 
			  {
                  mcSHOW_DBG_MSG(("the %d-th time setting PLL success\n" , j+1));
				  
				  break;
          	   }
          else
        	   mcSHOW_DBG_MSG((" the %d-th time setting PLL fail\n" , j+1));
               
		 	}
		 
	  }
	    break;
#endif

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
        break;
    case itSetLNA:
    case itGetLNA:
        break;

    case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pF05wtEqNormal[SetVal];
        break;
    case itGetEqScriptWeak: 
       *(UINT8**)pInOutVal = pF05wtEqWeak[SetVal];
        break;
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = NULL;
        break;

	case itSetIfMin:
	    {

          UINT8 subSysId = SetVal;
		  UINT8 *ifMinPtr = (UINT8*)pInOutVal;
			
         mcSHOW_DBG_MSG(("F05WT TOP Setting\n"));
    	 switch(subSysId){
    	    case MOD_PAL_BG:
    	    case MOD_PAL_I:
			case MOD_PAL_DK:
    	      *ifMinPtr = 0xE5;
    	        break;
    	    case MOD_SECAM_L:
            case MOD_SECAM_L1:
            *ifMinPtr = 0xE5;
    		    break;
    	    default:
    	        mcSHOW_DBG_MSG(("TOP No change\n"));
    	        break;
        }

	    }
	    break;
	case itSetIfDemod:
        {
             BOOL * Retval = (BOOL *)pInOutVal;
            * Retval =F05WT_SetIFDemod(pTCtx, SetVal);
        }
    break;

    default:
        return ITUNER_NOT_SUPPORTED;
    }
    return ITUNER_OK;
}

S16  F05WT_SetIFDemod(ITUNER_CTX_T *pTCtx, U8 SubSysIdx)
{  // SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);

   // INT32 i4GPIOValue = 1;
    UCHAR TunerS[5];

	
	TunerS[0] = 0x86;	// IF IC slave address
	TunerS[1] = 0x00;	// IF IC sub address
	
	switch (SubSysIdx)
	{
	case MOD_PAL_BG:
		TunerS[2] = 0x56;	// B-Data
		TunerS[3] = 0x70;	// C-Data
		if(pTCtx->u2IF_Freq_A==38000)
		{
		  TunerS[4] = 0x0d;	// E-Data	
		}
		if(pTCtx->u2IF_Freq_A==38900)
		{
		  TunerS[4] = 0x09;	// E-Data	 
		}
		//i4GPIOValue =0;
		//GPIO_Output(22, &i4GPIOValue);
		mcSHOW_DRVAPI_MSG(("Set MOD_PAL_BG=%x\n",MOD_PAL_BG));
		break;
	case MOD_PAL_DK:
		TunerS[2] = 0x56;	// B-Data
		TunerS[3] = 0x70;	// C-Data
		if(pTCtx->u2IF_Freq_A==38000)
		{
		TunerS[4] = 0x0f;	// E-Data
		}
		if(pTCtx->u2IF_Freq_A==38900)
		{
		TunerS[4] = 0x0b;	// E-Data
		}
		//i4GPIOValue =0;
		//GPIO_Output(22, &i4GPIOValue);
		break;
	case MOD_PAL_I:
		TunerS[2] = 0x56;	// B-Data
		TunerS[3] = 0x70;	// C-Data
		if(pTCtx->u2IF_Freq_A==38000)
		{
		 TunerS[4] = 0x0e;	// E-Data
		}
		if(pTCtx->u2IF_Freq_A==38900)
		{
		  TunerS[4] = 0x0a;	// E-Data
		}
		//i4GPIOValue =0;
		//GPIO_Output(22, &i4GPIOValue);
		break;		
	case MOD_NTSC_M:
		TunerS[2] = 0x16;	// B-Data
		TunerS[3] = 0x30;	// C-Data
		if(pTCtx->u2IF_Freq_A==38000)
		{
		 TunerS[4] = 0x0c;	// E-Data
		}
		if(pTCtx->u2IF_Freq_A==38900)
		{
		  TunerS[4] = 0x80;	// E-Data
		}
		//i4GPIOValue =1;
       // GPIO_Output(22, &i4GPIOValue);
		break;
	case MOD_SECAM_L:
	    TunerS[2] = 0x46;	// B-Data
		TunerS[3] = 0x70;	// C-Data
		TunerS[4] = 0x0b;	// E-Data
		break;
	default:
		TunerS[2] = 0x16;	// B-Data
		TunerS[3] = 0x30;	// C-Data
		TunerS[4] = 0x0c;	// E-Data
		//i4GPIOValue =0;
      //  GPIO_Output(22, &i4GPIOValue);
		break;
	}

  

//set 9886 TOP
 //if (!(pTCtx->m_Ana_Top & 0x80)) // if for external ATD AGC control
	//{ // set TOP value to ATD
    //    TunerS[3] &= 0xE0;
    //    TunerS[3] |= (pEUCtx->m_Ana_Top & 0x1F);
	//}
	
	mcDELAY(2);
	//if (RegSetRegisters(TunerS[0], TunerS[1], 3, &TunerS[2]) < 0)
   // if (ICtrlBus_I2cTunerWrite(TunerS[0],&TunerS[1],4) < 0)
   
   //if (ICtrlBus_I2cTunerWrite(u2Tuners,TunerS,4) < 0)


   if (ICtrlBus_I2cTunerWrite(C_F05WT_I2C_CLK_DIV, TunerS[0], TunerS+1, 4)==1)
   
    {
        mcSHOW_DRVAPI_MSG(("\nTunerSet() failed!\n"));
        return (-1);
    }
	
  
  mcSHOW_DRVAPI_MSG(("0x%02X-0x%02X-0x%02X\n",TunerS[2], TunerS[3],TunerS[4]));
	mcDELAY(10);

	return (0);
}
