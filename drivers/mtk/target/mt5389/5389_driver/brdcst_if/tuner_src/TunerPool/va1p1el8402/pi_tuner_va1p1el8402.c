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
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2008, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: pi_tuner_VA1P1EL8402.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_VA1P1EL8402.c
 *  Tuner control for sharp Tune VA1P1EL8402.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "tuner_interface_if.h"
#include "pi_def_dvbt.h"
#include "PI_Def.h"
#include "ctrl_bus.h"
#include "x_gpio.h"
#include "pi_dvbt_if.h" 
#include "fe_tuner_common_if.h"
#include "pi_demod_atd.h"
#include "drvcust_if.h"
#include "eq_script_va1p1el8402.h"
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define C_VA1P1EL8402_VERSION                "SHARP va1p1el8402 2010-01-01\n 2011-05-31\n v1.0"

#define C_VA1P1EL8402_BASE                  ((UINT16) 0xC2)  
#define C_VA1P1EL8402_IF_FREQUENCY          ((UINT16) 36167)  /* kHz */
#define C_VA1P1EL8402_IF_FREQUENCY_C          ((UINT16) 36125)  /* kHz */
#define C_VA1P1EL8402_IF_FREQUENCY_LO_STEP  ((UINT16) 900)    /* 100Hz */
#define C_VA1P1EL8402_IF_FREQUENCY_ANA      ((UINT16) 38900)  /* kHz */
#define C_VA1P1EL8402_IF_FREQUENCY_L1       ((UINT16) 33900)  /* kHz */
#define C_VA1P1EL8402_IF_FREQUENCY_I        ((UINT16) 39500)  /* kHz */
#define C_VA1P1EL8402_LO_DIVIDER_STEP       ((UINT32) 166670) /* Hz */
#define C_VA1P1EL8402_LO_DIVIDER_STEP_ANA   ((UINT16) 50000)  /* Hz */
#define C_VA1P1EL8402_LO_DIVIDER_STEP_C     ((UINT16) 62500)  /* Hz */

// DVB-T+DVB-C D-only Tuner
#define ccTUNER_LOCK_CONF_THRESHOLD     0
#define cALC_ADC_COMM_VOLT              1.2
#define cALC_ADC_INPUT_SWING            2  //Volt
#define cALC_ADC_BIAS                   (cALC_ADC_COMM_VOLT - cALC_ADC_INPUT_SWING/2)

#define C_VA1P1EL8402_TOP_SET_T              ((U8)   0x04)  //103dBuV
#define C_VA1P1EL8402_TOP_SET_C              ((U8)   0x05)  //100dBuV
//#define C_VA1P1EL8402_TOP_SET                ((U8)   0x06)  //external AGC
//#define C_VA1P1EL8402_TOP_SET_ANA           ((U8)   0x03)  //106dBuV
//#define C_VA1P1EL8402_TOP_SET               ((U8)   0x06)  //EXTERNAL
#define C_VA1P1EL8402_TOP_SET_ANA            ((U8)   0x06)  //EXTERNAL
#define C_VA1P1EL8402_SAW_BW                 SAW_BW_8M 

#define C_VA1P1EL8402_AGC_IF_SLP             0.225
#define C_VA1P1EL8402_AGC_IF_INT             0
#define C_VA1P1EL8402_AGC_IF_MAX             0.499
#define C_VA1P1EL8402_AGC_IF_MIN             -0.5

#define C_VA1P1EL8402_MAX_IF_GAIN_SET        -0.12   
#define C_VA1P1EL8402_MIN_IF_GAIN_SET        -0.38   
#define C_VA1P1EL8402_MAX_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_VA1P1EL8402_MIN_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_VA1P1EL8402_MAX_IF_GAIN_POW        -100    // dBm
#define C_VA1P1EL8402_MIN_IF_GAIN_POW        -50     // dBm
#define C_VA1P1EL8402_MAX_RF_GAIN_POW        C_VA1P1EL8402_MIN_IF_GAIN_POW
#define C_VA1P1EL8402_MIN_RF_GAIN_POW        -20     // dBm
#define C_VA1P1EL8402_POWER_CALIBRATE        26     // dBm

#define C_VA1P1EL8402_AGC_COEF               ((U8)   0xBD)
#define C_VA1P1EL8402_DEMOD_INPUT_POWER      cIF_TARGET_LEVEL_0CCC

#define C_VA1P1EL8402_I2C_DIVIDER       ((U16) 0x100)
// ********************************************* //

#define C_U8_VA1P1EL8402_AGC_IF_SLP          (S8)(C_VA1P1EL8402_AGC_IF_SLP *32 )
#define C_U8_VA1P1EL8402_AGC_IF_INT          (S8)(C_VA1P1EL8402_AGC_IF_INT *64 )
#define C_U8_VA1P1EL8402_AGC_IF_MAX          (S8)(C_VA1P1EL8402_AGC_IF_MAX *256)
#define C_U8_VA1P1EL8402_AGC_IF_MIN          (S8)(C_VA1P1EL8402_AGC_IF_MIN *256)

#define C_U8_VA1P1EL8402_MAX_IF_GAIN_SET     (S8)(C_VA1P1EL8402_MAX_IF_GAIN_SET *256)
#define C_U8_VA1P1EL8402_MIN_IF_GAIN_SET     (S8)(C_VA1P1EL8402_MIN_IF_GAIN_SET *256)
#define C_U8_VA1P1EL8402_MAX_RF_GAIN_SET     (S8)((C_VA1P1EL8402_MAX_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_VA1P1EL8402_MIN_RF_GAIN_SET     (S8)((C_VA1P1EL8402_MIN_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_VA1P1EL8402_IF_GAIN_RANGE       (S8)(C_VA1P1EL8402_MIN_IF_GAIN_POW - C_VA1P1EL8402_MAX_IF_GAIN_POW)
#define C_U8_VA1P1EL8402_RF_GAIN_RANGE       (S8)(C_VA1P1EL8402_MIN_RF_GAIN_POW - C_VA1P1EL8402_MAX_RF_GAIN_POW)

#define C_VA1P1EL8402_FREQ_DBOUND_UPPER   858*1000*1000     //modified for DVB-C application, Menghau, 091020
#define C_VA1P1EL8402_FREQ_DBOUND_LOWER    51*1000*1000
#define C_VA1P1EL8402_FREQ_DBOUND_UPPER_Ana   865*1000*1000
#define C_VA1P1EL8402_FREQ_DBOUND_LOWER_Ana    44*1000*1000
#define C_VA1P1EL8402_CHANNEL_SCAN_JUMP_SMALL_STEP ((U16) 250)
#define C_VA1P1EL8402_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP ((U16) 1500)
#define C_VA1P1EL8402_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN ((U16) 500)
#define C_U8_VA1P1EL8402_AGC_IF_LVL_MAX     0x12//18  //no signal
#define C_U8_VA1P1EL8402_AGC_IF_LVL_MIN     0x08 //8  //-70dbm input

STATIC UINT8* pVA1P1EL8402EqNormal[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pVA1P1EL8402EqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};

extern S16 VA1P1EL8402_TunerSetFreq(ITUNER_CTX_T *pTCtx, PARAM_SETFREQ_T* param);


//Tuner releated ATD register
STATIC ATD_TUNER_REL_REG_T arrAtdRelRegPAL[]={
	{0x2d4,0x03},//
	{0x2d5,0xfb},//
	{0x2d6,0xfb},//
	{0x2d7,0xed},//
	{0x2d8,0xeb},//
	{0x2d9,0xeb},//
	{0x2da,0x7f},//
	{0x2db,0x79},//
	{0x2dc,0x80},//
	{0x2dd,0x40},//
	{0x2de,0x40},//
	{0x245,0xdb},//
	//{0x2ca,0x99},//
	{0x0,0x0}//end flag
 };
STATIC ATD_TUNER_REL_REG_T arrAtdRelRegSECAM[]={
	{0x2d4,0x03},//
	{0x2d5,0xfb},//
	{0x2d6,0xfb},//
	{0x2d7,0xed},//
	{0x2d8,0xeb},//
	{0x2d9,0xeb},//
	{0x2da,0x7f},//
	{0x2db,0x79},//
	{0x2dc,0x80},//
	{0x2dd,0x40},//
	{0x2de,0x40},//
	{0x245,0xdb},//
	//{0x2ca,0x99},//
	{0x0,0x0}//end flag

 };

//----------------------------------------------------------------------------- 
/** VA1P1EL8402_TunerInit
 *  Tuner initialzation for VA1P1EL8402.
 *  @param  pTunerCtx       Pointer of pi tuner driver context.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void VA1P1EL8402_TunerInit(ITUNER_CTX_T *pTCtx)
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);

    psTunerCtx->I2cAddress = C_VA1P1EL8402_BASE;
    psTunerCtx->u2IF_Freq = C_VA1P1EL8402_IF_FREQUENCY;
    psTunerCtx->u2IF_Freq_A = C_VA1P1EL8402_IF_FREQUENCY_ANA; //EC_code_dA402: Add for MT5365 SAWLess ATD IF Freq. Info. 
    psTunerCtx->u4RF_Freq = 0;
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_SENS|MASK_PATCH_CCI);

	psTunerCtx->fgRFAGC = TRUE;//for RFAGC legacy
    // AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x2f; //EC_code_d9930: Change 0x151 value for EU new BPF
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0xe3; //EC_code_d9930: Change 0x158 value for EU new BPF
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B; //EC_code_d9930: Change 0x15F value for EU new BPF

    pEUCtx->m_SAW_BW  = C_VA1P1EL8402_SAW_BW;
    //pEUCtx->m_Ana_Top = C_VA1P1EL8402_TOP_SET_ANA;
    pEUCtx->m_s4FreqBoundUpper = C_VA1P1EL8402_FREQ_DBOUND_UPPER;
    pEUCtx->m_s4FreqBoundLower = C_VA1P1EL8402_FREQ_DBOUND_LOWER;
    pEUCtx->m_s4AnaFreqBoundUpper = C_VA1P1EL8402_FREQ_DBOUND_UPPER_Ana;
    pEUCtx->m_s4AnaFreqBoundLower = C_VA1P1EL8402_FREQ_DBOUND_LOWER_Ana;
    
    pEUCtx->m_Small_Step = C_VA1P1EL8402_CHANNEL_SCAN_JUMP_SMALL_STEP;
    pEUCtx->m_Middle_Step_Up = C_VA1P1EL8402_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP;
    pEUCtx->m_Middle_Step_Down = C_VA1P1EL8402_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN;
    
    // add by JC, 081215, for Signal Level display
    pEUCtx->m_ifAgc_lvl_max = (U8)C_U8_VA1P1EL8402_AGC_IF_LVL_MAX;
    pEUCtx->m_ifAgc_lvl_min = (U8)C_U8_VA1P1EL8402_AGC_IF_LVL_MIN;

    pEUCtx->m_SigLvTh = 0;//C_TDTWS710D_SIGNAL_LVL_TH;
    pEUCtx->m_SigLvScan = 0;

    pEUCtx->m_aucPara[0] = C_VA1P1EL8402_TOP_SET_T;
    pEUCtx->m_aucPara[1] = 0;

    pEUCtx->m_aucPara[2] = (U8)C_U8_VA1P1EL8402_AGC_IF_SLP;
    pEUCtx->m_aucPara[3] = (U8)C_U8_VA1P1EL8402_AGC_IF_INT;
    pEUCtx->m_aucPara[4] = (U8)C_U8_VA1P1EL8402_AGC_IF_MAX;
    pEUCtx->m_aucPara[5] = (U8)C_U8_VA1P1EL8402_AGC_IF_MIN;

    pEUCtx->m_aucPara[6] = C_VA1P1EL8402_AGC_COEF;
    pEUCtx->m_aucPara[7] = C_VA1P1EL8402_DEMOD_INPUT_POWER;

    pEUCtx->m_aucPara[ 8] = (U8)C_U8_VA1P1EL8402_MAX_IF_GAIN_SET;
    pEUCtx->m_aucPara[ 9] = (U8)C_U8_VA1P1EL8402_MIN_IF_GAIN_SET;
    pEUCtx->m_aucPara[10] = (U8)C_U8_VA1P1EL8402_MAX_RF_GAIN_SET;
    pEUCtx->m_aucPara[11] = (U8)C_U8_VA1P1EL8402_MIN_RF_GAIN_SET;
    pEUCtx->m_aucPara[12] = (U8)C_U8_VA1P1EL8402_IF_GAIN_RANGE;
    pEUCtx->m_aucPara[13] = (U8)C_U8_VA1P1EL8402_RF_GAIN_RANGE;
    pEUCtx->m_aucPara[14] = C_VA1P1EL8402_POWER_CALIBRATE;

    pVA1P1EL8402EqNormal[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQVA1P1EL8402BG;
    pVA1P1EL8402EqNormal[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQVA1P1EL8402DK;
    pVA1P1EL8402EqNormal[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQVA1P1EL8402I;
    pVA1P1EL8402EqNormal[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQVA1P1EL8402L;
    pVA1P1EL8402EqNormal[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQVA1P1EL8402L1; 
    pVA1P1EL8402EqNormal[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN] = EQVA1P1EL8402M;
    
    pVA1P1EL8402EqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQVA1P1EL8402BG;//EQVA1P1EL8402BG;
    pVA1P1EL8402EqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQVA1P1EL8402DK;//EQVA1P1EL8402DK;
    pVA1P1EL8402EqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQVA1P1EL8402I;//EQVA1P1EL8402I;
    pVA1P1EL8402EqWeak[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQVA1P1EL8402L;//EQVA1P1EL8402L;
    pVA1P1EL8402EqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQVA1P1EL8402L1;//EQVA1P1EL8402L1;
    pVA1P1EL8402EqWeak[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN] = EQVA1P1EL8402M;//EQVA1P1EL8402M;

    mcSHOW_USER_MSG(("Tuner VA1P1EL8402 init done!\n"));     
}


//----------------------------------------------------------------------------- 
/** VA1P1EL8402_TunerSetFreq
 *  Set Tuner PLL for VA1P1EL8402 to tune RF frequency.
 *  @param  pTCtx     Pointer of pi tuner driver context.
 *  @param  freq        RF center frequency in KHz.
 *  @param  mode                    
 *  @param  autoSearch  
 *  @param  step            
 *  @retval 
 */
//-----------------------------------------------------------------------------
INT16 VA1P1EL8402_TunerSetFreq(
    ITUNER_CTX_T *pTunerCtx,
    PARAM_SETFREQ_T* param
  )
{
    UCHAR uc_data;
    UCHAR TunerS[5];
    S16 ucTunerLockConfCnt;
    UINT32 Lo;
    UINT16  Ndivider, CB1, CB2, CB3; 
    S16 ii, jj;
//    UINT8 autoSearch=param->fgAutoSearch;;
    UINT8 Mode = param->Modulation;
    UINT32 Freq = param->Freq;

    mcSHOW_DBG_MSG(("VA1P1EL8402_TunerSetFreq\n"));
    
    SPECIFIC_MEMBER_EU_CTX* pEUCtx= &( pTunerCtx->specific_member.eu_ctx);
    //GPIO_AGC_SWITCH = DRVCUST_OptGet(eTunerCtrlAna);

    pTunerCtx->u4RF_Freq = Freq;
    pTunerCtx->I2cAddress = C_VA1P1EL8402_BASE;

      if( Mode == MOD_DVBT)//digital reception
    {

	    Lo = Freq + C_VA1P1EL8402_IF_FREQUENCY; // kHz
	    //Ndivider = (U16) ( ((Lo * 1000) / DTOS403_LO_DIVIDER_STEP) + 0.5 );
	  
	    Ndivider = (U16) ( (Lo*1000+C_VA1P1EL8402_LO_DIVIDER_STEP/2) / C_VA1P1EL8402_LO_DIVIDER_STEP);
		
		// Conver Freq to unit of MHz
			   Freq = Freq/1000;
	
	    if (Freq < 48 || Freq > 866)
	    {
	        mcSHOW_DBG_MSG(("Out of range for LO!\n"));
	        //return (+1);
	    }
	    
	     if (Freq < 146)	   
		{
			CB2 = 0x91;
		}
		else if (Freq < 430)  
		{		 
			CB2 = 0x92;
		}
	
		else	 			 
		{
			CB2 = 0x94;
		}
	 	CB1 = 0x80;
		CB3 = 0xd1;
		
		if (pEUCtx->m_SAW_BW == SAW_BW_7M)
			{
			CB2=(CB2 &0xef);
			mcSHOW_USER_MSG(("DVBT set SAW 7M!\n"));
			}
		
			
			                                                                                	    	   
    }
	else if ( Mode == MOD_DVBC)
		  {
		    Lo = Freq + C_VA1P1EL8402_IF_FREQUENCY_C; // kHz
		   //Ndivider = (U16) ( ((Lo * 1000) / DTOS403_LO_DIVIDER_STEP) + 0.5 );
		 
		    Ndivider = (U16) ( (Lo*1000+C_VA1P1EL8402_LO_DIVIDER_STEP_C/2) / C_VA1P1EL8402_LO_DIVIDER_STEP_C);

		    // Conver Freq to unit of MHz
	          Freq = Freq/1000;

		    if (Freq < 48|| Freq > 866)
		  {
			mcSHOW_USER_MSG(("Out of range for LO!\n"));
		   // return (+1);
		  }
			
		   if (Freq <= 146)	   
		  {
			CB2= 0x51;
		  }
		  else if (Freq <= 430)  
		  {		 
			CB2 = 0x52;
		  }
		  else				 
		  {
			CB2 = 0x54;
		  }
	 	   CB1 = 0x9B;
		   CB3 = 0xd1;			
	
	    }		
    else //analog reception
        {
    
	    if (Mode == MOD_SECAM_L1 )
	    {
	    	Lo = Freq + C_VA1P1EL8402_IF_FREQUENCY_L1; // kHz
	    }
	    else
	    {
	    	Lo = Freq + C_VA1P1EL8402_IF_FREQUENCY_ANA; // kHz
	    }	
	    Ndivider = (U16) ( (Lo*1000+C_VA1P1EL8402_LO_DIVIDER_STEP_ANA/2) / C_VA1P1EL8402_LO_DIVIDER_STEP_ANA);
	    
	    mcSHOW_DBG_MSG(("Freq = %d, Divider ratio = %04X \n",param->Freq, Ndivider));
	    
		pTunerCtx->u2IF_Freq_A = Lo - Freq;//chuanjin,for secam L1 to atd
	    // Conver Freq to unit of MHz
	    Freq = Freq/1000;
	    
	    if (Freq < 45 || Freq > 865)
	    {
	       mcSHOW_DBG_MSG(("Out of range for LO!\n"));
        //  return (+1);    // liuqu,20090910, can not returen 1 for 47.25MHZ channel loss!
	    }
	    
	    if (Freq <= 146)       
		{
			CB2 = 0x11;
		}
		else if (Freq <= 430)  
		{        
			CB2 = 0x12;
		}
		else                 
	    {
			CB2 = 0x14;
		}

         
	      CB1 = 0xBF;
          CB3 = 0xd1;

    }

    // LO settings
    // LO settings
    TunerS[0] = (UCHAR) 0x7F & (Ndivider >> 8);
    TunerS[1] = (UCHAR) 0xFF & Ndivider;
    TunerS[2] = (UCHAR) CB1;  
    TunerS[3] = (UCHAR) CB2;
    TunerS[4] = (UCHAR) CB3; 

    //pTunerCtx->u4LO_Freq = Lo;

    for (jj=0; jj<2; jj++)
    {        
        if (ICtrlBus_I2cTunerWrite(C_VA1P1EL8402_I2C_DIVIDER,pTunerCtx->I2cAddress, TunerS,5) > 0)
        {
            return (-1);
        }

	mcDELAY(10); 
	
        ucTunerLockConfCnt = 0;
        for (ii=0; ii<20; ii++)
        {
            mcDELAY_MS(5);
            ICtrlBus_I2cTunerRead(C_VA1P1EL8402_I2C_DIVIDER,pTunerCtx->I2cAddress, &uc_data,1);
			mcSHOW_DBG_MSG(("uc_data=0x%02x\n",uc_data));

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
            mcSHOW_DBG_MSG(("Freq = %d MHz, the %d-th time setting PLL success, cnt = %d\n" , Freq, jj+1, ucTunerLockConfCnt)); 
            break;
        }
        else
        {
            mcSHOW_DBG_MSG(("Freq = %d MHz, the %d-th time setting PLL fail\n" , Freq, jj+1));
        }
    }

    mcSHOW_DBG_MSG(("[%s]-LO%02X: %02X-%02X-%02X-%02X-%02X\n",(CHAR *)C_VA1P1EL8402_VERSION ,pTunerCtx->I2cAddress, TunerS[0], TunerS[1], TunerS[2], TunerS[3],TunerS[4]));


    pEUCtx->m_aucPara[1]  = TunerS[0];   //send register value to UI, LC 070102
    pEUCtx->m_aucPara[15] = TunerS[1];
    pEUCtx->m_aucPara[16] = TunerS[2];
    pEUCtx->m_aucPara[17] = TunerS[3];
    pEUCtx->m_aucPara[18] = TunerS[4];

    return (0);
}


 



//----------------------------------------------------------------------------- 
/** VA1P1EL8402_TunerGetStatus
 *  Set Tuner PLL staus for VA1P1EL8402
 *  @param  
 *  @retval tuner model name
 */
//-----------------------------------------------------------------------------
char *VA1P1EL8402_TunerGetVer(void)
{
    return ((CHAR*)C_VA1P1EL8402_VERSION);
}
//----------------------------------------------------------------------------- 
/** VA1P1EL8402_SetSawBw
 *  Set Tuner PLL staus for VA1P1EL8402
 *  @param  
 *  @retval tuner Saw BandWidth
 */
//-----------------------------------------------------------------------------
void VA1P1EL8402_SetSawBw(ITUNER_CTX_T* pTunerCtx, UINT8 sawbw)
{
    pTunerCtx->specific_member.eu_ctx.m_SAW_BW = sawbw;
	mcSHOW_DRVAPI_MSG(("VA1P1EL8402_SetSawBw = %d\n", sawbw));
}
//----------------------------------------------------------------------------- 
/** VA1P1EL8402_SetSSICondition
 *  Set SSI Condition for VA1P1EL8402
 *  @param  
 *  @retval void
 */
//-----------------------------------------------------------------------------
void VA1P1EL8402_SetSSICondition(ITUNER_CTX_T* pTunerCtx, VOID * pInOutVal)
{
    pTunerCtx->sSSIPara.i2Mod = *((INT16*) pInOutVal);
    pTunerCtx->sSSIPara.i2CR = *((INT16*) pInOutVal+1);
    pTunerCtx->sSSIPara.i2Ssi_RF= *((INT16 *) pInOutVal+2);
    pTunerCtx->sSSIPara.i2Ssi_IF= *((INT16 *) pInOutVal+3);
    pTunerCtx->sSSIPara.i2Ssi_BB= *((INT16 *) pInOutVal+4);
}

//----------------------------------------------------------------------------- 
/*
 *  VA1P1EL8402_GetSSI
 *  calculate Signal Strength Indicator.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   
 */
//-----------------------------------------------------------------------------
UINT32 VA1P1EL8402_GetSSI(ITUNER_CTX_T* pTunerCtx) 
{
    INT16 Power_Ref[] =
    {
        -93,
        -91,
        -90,
        -89,
        -88,
        -87,
        -85,
        -84,
        -83,
        -82,
        -82,
        -80,
        -78,
        -77,
        -76,
    };
    INT32 i4Power_ref = 0;
    INT32 i4Power_rec = 0;
    INT32 i4Power_rel = 0;
    INT16 i2Ssi_RF = 0;
    INT16 i2Ssi_IF = 0;
    INT16 i2Ssi_BB = 0;
    UINT32 u4SSI_Indicator = 0;
    SPECIFIC_MEMBER_EU_CTX * pCtx= &(pTunerCtx->specific_member.eu_ctx);

    // initial level for best mux, Ken, 100607
    pCtx->m_SigLvScan = 0;
    
    /* Get Power Ref Value */  
    mcSHOW_DBG_MSG(("DVBT: Mod=%d, Code Rate=%d, RFAGC=%d, IFAGC=%d, BBAGC=%d\n",pTunerCtx->sSSIPara.i2Mod,pTunerCtx->sSSIPara.i2CR,
	                                                              pTunerCtx->sSSIPara.i2Ssi_RF,pTunerCtx->sSSIPara.i2Ssi_IF,pTunerCtx->sSSIPara.i2Ssi_BB));

    /* Get Pre Value */    
    i2Ssi_RF = pTunerCtx->sSSIPara.i2Ssi_RF;    
    i2Ssi_IF = pTunerCtx->sSSIPara.i2Ssi_IF;    
    i2Ssi_BB = pTunerCtx->sSSIPara.i2Ssi_BB;	
    /* Get Power_Rec Value */
    if(i2Ssi_RF <= 115)
    {
        i4Power_rec = -5*(i2Ssi_RF-115)/12 -10; 
    }
    else if((i2Ssi_RF > 115) && (i2Ssi_RF <= 135))
    {
        i4Power_rec = -5*(i2Ssi_RF-135)/11 - 20;
    }
    else if((i2Ssi_RF > 135) && (i2Ssi_RF <= 199))
    {
        i4Power_rec = -5*(i2Ssi_RF-199)/41 - 30;
    }
	else if((i2Ssi_RF > 199) && (i2Ssi_RF <= 318))
    {
        i4Power_rec = -5*(i2Ssi_RF-318)/63 - 40;
    }
	else if((i2Ssi_RF > 318) && (i2Ssi_RF <= 384))
    {
        i4Power_rec = -5*(i2Ssi_RF-384)/32 - 50;
    }
	else if((i2Ssi_RF > 384) && (i2Ssi_RF <= 423))
    {
        i4Power_rec = -5*(i2Ssi_RF-423)/39 - 55;
    }
//    else if((i2Ssi_RF >= 540) && (i2Ssi_IF >= 8) && (i2Ssi_IF < 18))
    else if((i2Ssi_RF > 423) && (i2Ssi_IF < 18))
    {
        i4Power_rec = -5*(i2Ssi_IF-18)/4 - 65;
    }
    else if((i2Ssi_RF >= 515) && (i2Ssi_IF >= 18) && (i2Ssi_BB > 52) && (i2Ssi_BB <= 240))
    {
//        i4Power_rec = 5*(i2Ssi_BB-52)/37 - 75;
        i4Power_rec = 5*(i2Ssi_BB-52)/43 - 75;
    }
    else if((i2Ssi_RF >= 515) && (i2Ssi_IF >= 18) && (i2Ssi_BB > 21) && (i2Ssi_BB <= 52))
    {
        i4Power_rec = 5*(i2Ssi_BB-21)/11 - 85;
    }
    else if((i2Ssi_RF >= 515) && (i2Ssi_IF >= 18) && (i2Ssi_BB > 0) && (i2Ssi_BB <= 21))
    {
        i4Power_rec = 5*(i2Ssi_BB-16)/5 - 90;
    }
    else
    {
        i4Power_rec = -120;
    }

	/*Get Ref Value-Start*/
	if(pTunerCtx->sSSIPara.i2Mod==DVBT_Qam_Mode_Unknow)
	{
		mcSHOW_DBG_MSG1(("DVBT: QAM mode unknown; Power_rec=%d, Power_ref=%d, Power_rel=%d\n",i4Power_rec,i4Power_ref,i4Power_rel));
		return u4SSI_Indicator;
	}
	else
	{
		i4Power_ref = Power_Ref[(pTunerCtx->sSSIPara.i2Mod)*DVBT_CR_Mode_Size + pTunerCtx->sSSIPara.i2CR];
	}


    
    /* Get Power Rel Value */
    i4Power_rel = i4Power_rec - i4Power_ref;
	mcSHOW_DBG_MSG(("DVBT: Power_rec=%d, Power_ref=%d, Power_rel=%d\n",i4Power_rec,i4Power_ref,i4Power_rel));
    /* Cal SSI_Indicator Value */
    if(i4Power_rel < -15)
    {
        u4SSI_Indicator = 0;
    }
    else if((i4Power_rel >= -15) && (i4Power_rel < 0))
    {
        u4SSI_Indicator = 2*(i4Power_rel+15)/3;
    }
    else if((i4Power_rel >= 0) && (i4Power_rel < 20))
    {
        u4SSI_Indicator = 4*i4Power_rel+10;
    }
    else if((i4Power_rel >= 20) && (i4Power_rel < 35))
    {
        u4SSI_Indicator = 2*(i4Power_rel-20)/3+90;
    }
    else if(i4Power_rel >= 35)
    {
        u4SSI_Indicator = 100;
    }

    // update signal level for best mux, Ken, 100607
    pCtx->m_SigLvScan = (S8) u4SSI_Indicator;
    
    return u4SSI_Indicator;
}

//----------------------------------------------------------------------------- 
/** VA1P1EL8402_TunerOP
 *  Set Tuner PLL staus for VA1P1EL8402
 *  @param  
 *  @retval tuner OP
 */
//-----------------------------------------------------------------------------
INT16 VA1P1EL8402_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal)
{
    switch (eOperation)
    {
    case itGetVer:
        {
            CHAR ** RetStr=(CHAR **)pInOutVal;
          *RetStr=VA1P1EL8402_TunerGetVer();
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
                if (ICtrlBus_I2cTunerWrite(C_VA1P1EL8402_BASE,pTCtx->I2cAddress, TunerS,5) > 0)
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
                ICtrlBus_I2cTunerRead(C_VA1P1EL8402_BASE,pTCtx->I2cAddress, &uc_data,1);
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
        VA1P1EL8402_SetSawBw(pTCtx, SetVal);
        break;
    case itSetLNA:
        break;
    case itSetIfMin:
        {

        UINT8 subSysId = SetVal;
        UINT8 *ifMinPtr = (UINT8*)pInOutVal;
            
        mcSHOW_DBG_MSG(("VA1P1EL8402 TOP Setting\n"));
        switch(subSysId)
            {
            case MOD_SECAM_L:
            case MOD_SECAM_L1:
                *ifMinPtr = 0x00;
                break;
            default:
                *ifMinPtr = 0x40;
                break;
            }
        }
        break;

    case itGetTHC:
        {

        UINT8 subSysId = SetVal;
        UINT8 *ifThcPtr = (UINT8*)pInOutVal;
            
        mcSHOW_DBG_MSG(("VA1P1EL8402 TOP Setting\n"));
        switch(subSysId){
            case MOD_SECAM_L:
            case MOD_SECAM_L1:
                *ifThcPtr = 0x20;
                break;
            default:
                *ifThcPtr = 0x1A;
                break;
        }
        }
        break;

    case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pVA1P1EL8402EqNormal[SetVal];
        break;
    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pVA1P1EL8402EqWeak[SetVal];
        break;
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = NULL;
        break; 

    case itSetSSICond:
        VA1P1EL8402_SetSSICondition(pTCtx, pInOutVal);
        break;
    case itGetSSIIndex:
        *((UINT32*)pInOutVal) = VA1P1EL8402_GetSSI(pTCtx);
        break;  

    case itGetAnaFreqBound:
        {
        TUNER_ANA_ATTRIBUTE_T   *psTunerAnaAttribute;

        psTunerAnaAttribute = (TUNER_ANA_ATTRIBUTE_T *) pInOutVal;
        psTunerAnaAttribute->ui4_lower_bound_freq = C_VA1P1EL8402_FREQ_DBOUND_LOWER_Ana;
        psTunerAnaAttribute->ui4_upper_bound_freq = C_VA1P1EL8402_FREQ_DBOUND_UPPER_Ana;
        }
        break;
	case itGetTunerRelReg://Get ATD Tuner Rel register setting
				{
					UINT8 subSysId = SetVal;
					switch(subSysId)
						{
						case MOD_SECAM_L:
						case MOD_SECAM_L1:
							*(ATD_TUNER_REL_REG_T **)pInOutVal = arrAtdRelRegSECAM;
							break;
						default:
							*(ATD_TUNER_REL_REG_T **)pInOutVal= arrAtdRelRegPAL;
							break;
						}
				}
			break;

    default:
        return ITUNER_NOT_SUPPORTED;
    }

    return ITUNER_OK;
}

