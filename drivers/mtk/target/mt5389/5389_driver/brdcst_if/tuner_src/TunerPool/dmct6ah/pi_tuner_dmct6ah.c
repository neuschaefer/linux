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

/** @file pi_tuner_DMCT6AH.c
 *  Panasonic DMCT6AH tuner driver.
 */

#include "tuner_interface_if.h"
#include "pi_def_dvbt.h"
#include "PI_Def.h"
#include "ctrl_bus.h"
#include "x_gpio.h"
#include "pi_dvbt_if.h" 
#include "fe_tuner_common_if.h"
#include "pi_demod_atd.h"
#include "drvcust_if.h"

#include "eq_script_dmct6ah.h"

#include "x_gpio.h"   // to set the GPIO control



// *** Release Note ***
// Ver     Date         Description
// 1.00    2009.03.25   file established
#define C_DMCT6AH_VERSION				"DMCT6AH v1.02-2011822_DVBCIFAGC-ATVifagc_EQ2"

//#define C_DMCT6AH_PLL_POLL_INTERVAL   5
#define C_DMCT6AH_PLL_POLL_INTERVAL   20   // to solve SLT thread UART overflow issue, 090601, Ken
#define C_DMCT6AH_PLL_TIMEOUT         120
#define C_DMCT6AH_PLL_TIMEOUT_CNT     C_DMCT6AH_PLL_TIMEOUT/C_DMCT6AH_PLL_POLL_INTERVAL
#define C_DMCT6AH_I2C_CLK_DIV         0x100
#define C_DMCT6AH_PLL_LOCK_BIT        6



#define C_DMCT6AH_TOP_SET             ((U8)   0x03) 
#define C_DMCT6AH_TOP_SET_DVBC        ((U8)   0x03) 
#define C_DMCT6AH_TOP_SET_ANA         ((U8)   0x03) //chuanjin change  0x06
//#define C_DMCT6AH_SAW_BW              SAW_BW_8M 
#define C_DMCT6AH_SAW_BW              2
#define C_DMCT6AH_LNA_SWITCH              1


//#define C_DMCT6AH_FREQ_DBOUND_UPPER   858*1000*1000 
#define C_DMCT6AH_FREQ_DBOUND_UPPER   859*1000*1000      //modified for DVB-C application, Menghau
#define C_DMCT6AH_FREQ_DBOUND_LOWER    48*1000*1000 
#define C_DMCT6AH_FREQ_DBOUND_UPPER_Ana     863250000
#define C_DMCT6AH_FREQ_DBOUND_LOWER_Ana    49750000


#define C_DMCT6AH_LO_ADDRESS           ((U16)  0xC0)
#define C_DMCT6AH_IF_FREQUENCY          ((U16)  36000)  /* kHz */
#define C_DMCT6AH_IF_FREQUENCY_ANA      ((U16)  38900)  /* kHz */
#define C_DMCT6AH_LO_DIVIDER_STEP      ((U32) 166667)  /* Hz */
#define C_DMCT6AH_LO_DIVIDER_STEP_ANA     ((U32) 62500)  /* Hz */


#define C_DMCT6AH_CHANNEL_SCAN_JUMP_SMALL_STEP ((U16) 250)
#define C_DMCT6AH_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP ((U16) 1500)
#define C_DMCT6AH_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN ((U16) 500)


//------------------
#define C_DMCH6AH_AGC_IF_SLP             0.5
#define C_DMCH6AH_AGC_IF_INT             0
#define C_DMCH6AH_AGC_IF_MAX             0.10
#define C_DMCH6AH_AGC_IF_MIN             -0.5

#define C_DMCH6AH_MAX_IF_GAIN_SET        -0.12   
#define C_DMCH6AH_MIN_IF_GAIN_SET        -0.38   
#define C_DMCH6AH_MAX_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_DMCH6AH_MIN_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_DMCH6AH_MAX_IF_GAIN_POW        -100    // dBm
#define C_DMCH6AH_MIN_IF_GAIN_POW        -50     // dBm
#define C_DMCH6AH_MAX_RF_GAIN_POW        C_DMCH6AH_MIN_IF_GAIN_POW
#define C_DMCH6AH_MIN_RF_GAIN_POW        -20     // dBm
#define C_DMCH6AH_POWER_CALIBRATE        26     // dBm

#define C_DMCH6AH_AGC_COEF               ((U8)   0xBD)
#define C_DMCH6AH_DEMOD_INPUT_POWER      cIF_TARGET_LEVEL_0CCC

#define C_DMCH6AH_I2C_DIVIDER       ((U16) 0x100)
// ********************************************* //

#define C_U8_DMCH6AH_AGC_IF_SLP          (S8)(C_DMCH6AH_AGC_IF_SLP *32 )
#define C_U8_DMCH6AH_AGC_IF_INT          (S8)(C_DMCH6AH_AGC_IF_INT *64 )
#define C_U8_DMCH6AH_AGC_IF_MAX          (S8)(C_DMCH6AH_AGC_IF_MAX *256)
#define C_U8_DMCH6AH_AGC_IF_MIN          (S8)(C_DMCH6AH_AGC_IF_MIN *256)

#define C_U8_DMCH6AH_MAX_IF_GAIN_SET     (S8)(C_DMCH6AH_MAX_IF_GAIN_SET *256)
#define C_U8_DMCH6AH_MIN_IF_GAIN_SET     (S8)(C_DMCH6AH_MIN_IF_GAIN_SET *256)
#define C_U8_DMCH6AH_MAX_RF_GAIN_SET     (S8)((C_DMCH6AH_MAX_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_DMCH6AH_MIN_RF_GAIN_SET     (S8)((C_DMCH6AH_MIN_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_DMCH6AH_IF_GAIN_RANGE       (S8)(C_DMCH6AH_MIN_IF_GAIN_POW - C_DMCH6AH_MAX_IF_GAIN_POW)
#define C_U8_DMCH6AH_RF_GAIN_RANGE       (S8)(C_DMCH6AH_MIN_RF_GAIN_POW - C_DMCH6AH_MAX_RF_GAIN_POW)

#define C_U8_DMCH6AH_AGC_IF_LVL_MAX     0x12//18  //no signal
#define C_U8_DMCH6AH_AGC_IF_LVL_MIN     0x08 //8  //-70dbm input


STATIC UINT8* pDMCT6AHEqNormal[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pDMCT6AHEqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};

STATIC UCHAR TunerS[5];
STATIC UINT8 SetLNA=0;
STATIC UINT8 LNAValue=0;
#if defined(CC_MT5396) || defined(CC_MT5389) 
STATIC ATD_TUNER_REL_REG_T arrAtdRelReg[]={
 {0x7d4,0x0c},//PRA target level[11:4] upper boundary
 {0x7d5,0xec},
 {0x7d6,0xec},//IF_BIAS1
 {0x7d7,0xe5},//IF_BIAS2
 {0x7d8,0xdc},//IF_SLOPE1
 {0x7d9,0xdc},//IF_SLOPE2
 {0x7da,0x7f},
 {0x7db,0xf0},  //chuanjin change
 {0x7dc,0x80},
 {0x7dd,0x80},
 {0x7de,0x80},
 {0x0,0x0}//end flag
 };
#else
STATIC ATD_TUNER_REL_REG_T arrAtdRelReg[]={
 {0x2d4,0x0c},//PRA target level[11:4] upper boundary
 {0x2d5,0xec},
 {0x2d6,0xec},//IF_BIAS1
 {0x2d7,0xe5},//IF_BIAS2
 {0x2d8,0xdc},//IF_SLOPE1
 {0x2d9,0xdc},//IF_SLOPE2
 {0x2da,0x7f},
 {0x2db,0xf0},  //chuanjin change
 {0x2dc,0x80},
 {0x2dd,0x80},
 {0x2de,0x80},
 {0x0,0x0}//end flag
 };
#endif 
///////////////////////////////////
// ALPS DMCT6AH driver
///////////////////////////////////

CHAR* DMCT6AH_GetSwVer(void)
{
	return ((CHAR*)C_DMCT6AH_VERSION);
}

static VOID vAGCPinGpioCtrl(BOOL fgDigital)
{
  UINT32 GPIONum_LNA_SWITCH = 51 ;

  if (fgDigital)
        {
            mcSHOW_DBG_MSG(("-LNA tuner pin2 (GPIO%d)->1\n", GPIONum_LNA_SWITCH));
           GPIO_SetOut(GPIONum_LNA_SWITCH, 1); // Digital set AGC pin as 1        
        }
  else
        {
            mcSHOW_DBG_MSG(("-LNA tuner pin2 (GPIO%d)->0\n", GPIONum_LNA_SWITCH));
            GPIO_SetOut(GPIONum_LNA_SWITCH, 0); // Analog set AGC pin as 0
        }		
}


//Justin TODO, pEUCtx->m_aucPara all necessary????
VOID DMCT6AH_TunerInit(ITUNER_CTX_T *pTCtx)
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);
    
    psTunerCtx->I2cAddress = C_DMCT6AH_LO_ADDRESS;
    psTunerCtx->u2IF_Freq = C_DMCT6AH_IF_FREQUENCY;
    psTunerCtx->u4RF_Freq = 0;
	psTunerCtx->u2IF_Freq_A = C_DMCT6AH_IF_FREQUENCY_ANA;
    //Justin TODO
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_DRO|MASK_PATCH_SENS);
    psTunerCtx->fgRFAGC = TRUE;
    // AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x2f;  //according to yu ding's mail,20091123
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B;

    pEUCtx->m_SAW_BW  = C_DMCT6AH_SAW_BW;
    pEUCtx->m_Ana_Top = C_DMCT6AH_TOP_SET_ANA;
    pEUCtx->m_s4FreqBoundUpper = C_DMCT6AH_FREQ_DBOUND_UPPER;
    pEUCtx->m_s4FreqBoundLower = C_DMCT6AH_FREQ_DBOUND_LOWER;
    pEUCtx->m_s4AnaFreqBoundUpper = C_DMCT6AH_FREQ_DBOUND_UPPER_Ana;
    pEUCtx->m_s4AnaFreqBoundLower = C_DMCT6AH_FREQ_DBOUND_LOWER_Ana;
    
    pEUCtx->m_Small_Step = C_DMCT6AH_CHANNEL_SCAN_JUMP_SMALL_STEP;
    pEUCtx->m_Middle_Step_Up = C_DMCT6AH_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP;
    pEUCtx->m_Middle_Step_Down = C_DMCT6AH_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN;


// add by JC, 081215, for Signal Level display
    pEUCtx->m_ifAgc_lvl_max = (U8)C_U8_DMCH6AH_AGC_IF_LVL_MAX;
    pEUCtx->m_ifAgc_lvl_min = (U8)C_U8_DMCH6AH_AGC_IF_LVL_MIN;

    pEUCtx->m_SigLvTh = 0;//C_TDTWS710D_SIGNAL_LVL_TH;
    pEUCtx->m_SigLvScan = 0;

    pEUCtx->m_aucPara[0] = C_DMCT6AH_TOP_SET;
    pEUCtx->m_aucPara[31] = C_DMCT6AH_TOP_SET_ANA ;
    pEUCtx->m_aucPara[1] = C_DMCT6AH_LNA_SWITCH;

    pEUCtx->m_aucPara[2] = (U8)C_U8_DMCH6AH_AGC_IF_SLP;
    pEUCtx->m_aucPara[3] = (U8)C_U8_DMCH6AH_AGC_IF_INT;
    pEUCtx->m_aucPara[4] = (U8)C_U8_DMCH6AH_AGC_IF_MAX;
    pEUCtx->m_aucPara[5] = (U8)C_U8_DMCH6AH_AGC_IF_MIN;

    pEUCtx->m_aucPara[6] = C_DMCH6AH_AGC_COEF;
    pEUCtx->m_aucPara[7] = C_DMCH6AH_DEMOD_INPUT_POWER;

    pEUCtx->m_aucPara[ 8] = (U8)C_U8_DMCH6AH_MAX_IF_GAIN_SET;
    pEUCtx->m_aucPara[ 9] = (U8)C_U8_DMCH6AH_MIN_IF_GAIN_SET;
    pEUCtx->m_aucPara[10] = (U8)C_U8_DMCH6AH_MAX_RF_GAIN_SET;
    pEUCtx->m_aucPara[11] = (U8)C_U8_DMCH6AH_MIN_RF_GAIN_SET;
    pEUCtx->m_aucPara[12] = (U8)C_U8_DMCH6AH_IF_GAIN_RANGE;
    pEUCtx->m_aucPara[13] = (U8)C_U8_DMCH6AH_RF_GAIN_RANGE;
    pEUCtx->m_aucPara[14] = C_DMCH6AH_POWER_CALIBRATE;


    pDMCT6AHEqNormal[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQDMCT6AHBG;
    pDMCT6AHEqNormal[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQDMCT6AHDK;
    pDMCT6AHEqNormal[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQDMCT6AHI;
    pDMCT6AHEqNormal[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQDMCT6AHL;
    pDMCT6AHEqNormal[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]   = EQDMCT6AHM;

#ifdef CC_MT5363	
    pDMCT6AHEqNormal[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQDMCT6AHL1;
#else
    pDMCT6AHEqNormal[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQDMCT6AHL;
#endif
    pDMCT6AHEqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQDMCT6AHBGWeak;
    pDMCT6AHEqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQDMCT6AHDKWeak;
    pDMCT6AHEqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQDMCT6AHIWeak;
    pDMCT6AHEqWeak[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQDMCT6AHLWeak;
#ifdef CC_MT5363	
    pDMCT6AHEqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQDMCT6AHL1Weak;
#else
    pDMCT6AHEqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQDMCT6AHLWeak;
#endif

    mcSHOW_USER_MSG(("Tuner DMCT6AH init done!\n"));
}
S16 DMCT6AH_TunerSetFreq(ITUNER_CTX_T *pTCtx, PARAM_SETFREQ_T* param)
{
    U32 Freq = param->Freq; // transfer to video freq
    //U8  AutoSearch = param->fgAutoSearch;
    U8  Mode = param->Modulation;

    UCHAR ucPllValue;
    //UCHAR TunerS[5];
  //  U32 Lo;
    U16 Ndivider, CB1, BB, CB3;
    S16 ii;
    U32 u4Lo;

    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);

    pTCtx->u4RF_Freq = Freq;

	         CB1= 0x80;
		  BB=0x00;//AISL 0 P5 0
	         CB3=0xC0; 
			 
    if ((Mode == MOD_DTMB) ||(Mode == MOD_DVBT))//DTMB  reception
    {
        vAGCPinGpioCtrl(1);
		
	u4Lo=Freq+C_DMCT6AH_IF_FREQUENCY;
 //       Ndivider = (U16)((u4Lo*6)/1000);//Stepsize 1000/6KHz
 
        Ndivider = (U16)( (u4Lo*1000+C_DMCT6AH_LO_DIVIDER_STEP/2) / C_DMCT6AH_LO_DIVIDER_STEP);
 
        // Conver Freq to unit of MHz
        u4Lo/=1000;//convert to MHz
		
        if (Freq < (C_DMCT6AH_FREQ_DBOUND_LOWER/1000) ||
            Freq > (C_DMCT6AH_FREQ_DBOUND_UPPER/1000))
        {
            mcSHOW_DRVERR_MSG(("Out of range for LO!\n"));
          //return (+1);
        }
		
     //   CB1|=0x20;//TOP 105 Stepsize 1/6MHz
     //merge TOP setting!!
          CB1 &= 0XC7;
          CB1 |= (pEUCtx->m_aucPara[0]&0X07)<<3;
	   CB3 &=0XFD;
	   CB3 |= (pEUCtx->m_aucPara[0] & 0X08) >>2;
	  
    }
    else if (Mode == MOD_DVBC ) 
	{
            vAGCPinGpioCtrl(1);
			
		u4Lo=Freq+C_DMCT6AH_IF_FREQUENCY;
   //     Ndivider = (U16)((u4Lo*6)/1000);//Stepsize 1000/6KHz

         Ndivider = (U16)( (u4Lo*1000+C_DMCT6AH_LO_DIVIDER_STEP/2) / C_DMCT6AH_LO_DIVIDER_STEP);
   
        // Conver Freq to unit of MHz
        u4Lo/=1000;//convert to MHz
        
        if (Freq < (C_DMCT6AH_FREQ_DBOUND_LOWER/1000) ||
            Freq > (C_DMCT6AH_FREQ_DBOUND_UPPER/1000)  )
        {
            mcSHOW_DRVERR_MSG(("Out of range for LO!\n"));
          //return (+1);
        }
       CB1|=0x20;//TOP 105 Stepsize 1/6MHz

	   CB1 &= 0XC7;
          CB1 |= (pEUCtx->m_aucPara[0]&0X07)<<3;
	   CB3 &=0XFD;
	   CB3 |= (pEUCtx->m_aucPara[0] & 0X08) >>2;
	  
    }
	else//ATV
    {
          vAGCPinGpioCtrl(0);
		  
		u4Lo=Freq+C_DMCT6AH_IF_FREQUENCY_ANA;
 //       Ndivider = (U16)((u4Lo*64)/1000);//Stepsize 1000/64KHz

         Ndivider = (U16)( (u4Lo*1000+C_DMCT6AH_LO_DIVIDER_STEP_ANA/2) / C_DMCT6AH_LO_DIVIDER_STEP_ANA);
 
       // Conver Freq to unit of MHz
        u4Lo/=1000;//convert to MHz
		
        if (Freq < (C_DMCT6AH_FREQ_DBOUND_LOWER_Ana/1000) ||
            Freq > (C_DMCT6AH_FREQ_DBOUND_UPPER_Ana/1000))
        {
            mcSHOW_DRVERR_MSG(("Out of range for LO!\n"));
          //return (+1);
        }

		
            //  CB1|=0x38;//RF_AGC disable 
		CB1|=0x03;//Stepsize 62.5KHz	

	   CB1 &= 0XC7;
          CB1 |= (pEUCtx->m_aucPara[31]&0X07)<<3;
	   CB3 &=0XFD;
	   CB3 |= (pEUCtx->m_aucPara[31] & 0X08) >>2;
	   
//	   CB1 |= (C_DMCT6AH_TOP_SET_ANA&0X07)<<3;
//	   CB3 &=0XFD;
//	   CB3 |= (C_DMCT6AH_TOP_SET_ANA & 0X08) >>2;     // tuner top 117
    }
			//CP1,CP0
        if(u4Lo<148)
        {	
         BB|=0x00;		
        }
		else if(u4Lo<196)
		{
		 BB|=0x40;
		}
		else if(u4Lo<366)
		{
		  BB|=0x00;
		}
		else if(u4Lo<484)
		{
		  BB|=0x80;
		}
		else if(u4Lo<646)
		{
		  BB|=0x40;
		}
		else if(u4Lo<790)
		{
		  BB|=0x80;
		}
		else
		{
		  BB|=0xC0;
		}
		
		//DVBC CP1 CP2 0 0
		if (Mode == MOD_DVBC)
        {
            BB&=0x3F;     			
        }
		
		//BS4~BS1
		if(Freq<148000)//VHF-L
		{
		 BB|=0x01;		 
		}
		else if(Freq<431000)//VHF-H
		{
		BB|=0x02;
		}
		else//UHF
		{
		BB|=0x08;
		}
		
		BB&=0xfb;
		
			
        if(SetLNA==1)                         //set LNA 
		{BB|= (LNAValue&0X01)<<2; 

        mcSHOW_DBG_MSG(("SET LNA  =%d\n",LNAValue));}
		else
		{                                    //recommended digital on;analog off
			if (Mode <= MOD_ANA_TYPE_BEGIN)    
             BB|=0x04;
		     else
		     BB|=0x00;
		}
		
        // LO settings
        TunerS[0] = (UCHAR) 0x7F & (Ndivider >> 8);
        TunerS[1] = (UCHAR) 0xFF & Ndivider;
        TunerS[2] = (UCHAR) CB1;
        TunerS[3] = (UCHAR) BB;
        TunerS[4] = (UCHAR) CB3;

  //  send this data

        if (ICtrlBus_I2cTunerWrite(C_DMCT6AH_I2C_CLK_DIV, pTCtx->I2cAddress, TunerS, 5))
        {
            mcSHOW_HW_MSG(("TunerWrite PLL failed!\n"));
            return (+1);
        }
		mcDELAY_MS(50);

        // wait PLL lock
    //ucTunerLockConfCnt = 0;
        for (ii=0; ii<C_DMCT6AH_PLL_TIMEOUT_CNT; ii++)
        {
            if (ICtrlBus_I2cTunerRead(C_DMCT6AH_I2C_CLK_DIV, pTCtx->I2cAddress, &ucPllValue, 1))
            {
                mcSHOW_HW_MSG(("Tuner read failed!\n"));
                return (+1);
            }
            // check if PLL lock
            if (mcCHK_BIT1(ucPllValue, C_DMCT6AH_PLL_LOCK_BIT)) {
                break;
            }
            mcDELAY_MS(C_DMCT6AH_PLL_POLL_INTERVAL);
        }

        if(ii < C_DMCT6AH_PLL_TIMEOUT_CNT)
		{
             mcSHOW_DBG_MSG(("Freq = %d kHz, the %d-th time setting PLL success\n" , Freq, ii+1));
          }
        else 
		{
        	if (Freq < 48)
        	   {	
              mcSHOW_DBG_MSG(("Freq = %d kHz,Frequency not support\n" , Freq));//liuqu,20090828,DHCID00000589
            }else
          	{
              mcSHOW_DBG_MSG(("Freq = %d kHz, the %d-th time setting PLL fail\n" , Freq, ii+1));
            }
        }
    mcSHOW_DBG_MSG(("LO_%02X:0x%02X-0x%02X-0x%02X-0x%02X-0x%02X\n" , pTCtx->I2cAddress, TunerS[0],TunerS[1],TunerS[2],TunerS[3],TunerS[4]));
    return 0;
}

INT16 DMCT6AH_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){

    switch(eOperation){
    case itGetVer:
        {
        CHAR ** RetStr = (CHAR **)pInOutVal;
        *RetStr = DMCT6AH_GetSwVer();
        }
        break;
    case itGetEqScriptNormal: 
			{	UINT8 subSysId = SetVal;
				
				if(subSysId==(MOD_PAL_DK - MOD_ANA_TYPE_BEGIN))
					{
					if(pTCtx->u4RF_Freq >400000)
						{*(UINT8**)pInOutVal =EQDMCT6AHDK_400M;
					mcSHOW_DBG_MSG(("PALDK 400M EQ\n"));
					
					}
					else if ((pTCtx->u4RF_Freq >175000) &&(pTCtx->u4RF_Freq<195000) )
						{*(UINT8**)pInOutVal =EQDMCT6AHDK;
					mcSHOW_DBG_MSG(("PALDK  EQ\n"));
						}
					else
						{*(UINT8**)pInOutVal =EQDMCT6AHDK_112M;
					mcSHOW_DBG_MSG(("PALDK 112M EQ\n"));
						}
					}
				else
        {*(UINT8**)pInOutVal = pDMCT6AHEqNormal[SetVal];
				mcSHOW_DBG_MSG(("other mode EQ\n"));
				}
    	}
        break;
    case itGetEqScriptWeak: 
		{
			UINT8 subSysId = SetVal;
				if(subSysId==(MOD_PAL_DK - MOD_ANA_TYPE_BEGIN))
					{
					if(pTCtx->u4RF_Freq >400000)
						{*(UINT8**)pInOutVal =EQDMCT6AHDKWeak_400M;
						mcSHOW_DBG_MSG(("PALDK 400M EQ\n"));}
					else if ((pTCtx->u4RF_Freq >175000) &&(pTCtx->u4RF_Freq<195000) )
						{*(UINT8**)pInOutVal =EQDMCT6AHDKWeak;
						mcSHOW_DBG_MSG(("PALDK  EQ\n"));
						}
					else
						{*(UINT8**)pInOutVal =EQDMCT6AHDKWeak_112M;
						mcSHOW_DBG_MSG(("PALDK 112M EQ\n"));}
					}
				else	
        *(UINT8**)pInOutVal = pDMCT6AHEqWeak[SetVal];
    	}
        break;
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = NULL;
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
                if (ICtrlBus_I2cTunerWrite(0x100,pTCtx->I2cAddress, TunerS,5) > 0)
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
                ICtrlBus_I2cTunerRead(0x100,pTCtx->I2cAddress, &uc_data,1);
                if (mcGET_FIELD(uc_data, 0x40, 6))  // if PLL lock
                  ucTunerLockConfCnt++;
                else
                  ucTunerLockConfCnt = 0;
                if (ucTunerLockConfCnt>40)
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
     case itSetTop:
    {
          SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);
		  
	    if (SetVal & 0x80)
            	{
	        pTunerCtx->m_aucPara[31] = (SetVal & 0x0f);
	        mcSHOW_DBG_MSG(("Analog TOP = 0x%2x\n",pTunerCtx->m_aucPara[1])); 
            	}
	  else
	  	{
               pTunerCtx->m_aucPara[0] = SetVal;		  
	        mcSHOW_DBG_MSG(("Digital TOP = 0x%2x\n",pTunerCtx->m_aucPara[0]));
	  	}
    }	
    //}	
     break;	 
    case itSetIfMin:
        {

        UINT8 subSysId = SetVal;
        UINT8 *ifMinPtr = (UINT8*)pInOutVal;
            
        mcSHOW_DBG_MSG(("DMCT6AH TOP Setting\n"));
        switch(subSysId)
            {
            case MOD_SECAM_L:
            case MOD_SECAM_L1:
                *ifMinPtr = 0x80;
                break;
            default:
                *ifMinPtr = 0x80;
                break;
            }
        }
        break;	
	case itSetLNAUI:
			{
	//	SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);
	  LNAValue= SetVal; 
	   SetLNA=1;
	   TunerS[3]&=0xfb;
	   TunerS[3]|= (LNAValue&0X01)<<2; //set LNA 
	   ICtrlBus_I2cTunerWrite(C_DMCT6AH_I2C_CLK_DIV, pTCtx->I2cAddress, TunerS, 5);
	   mcSHOW_DBG_MSG(("set LNA = 0x%2x\n",LNAValue));
			}
		break;
	case itGetTunerRelReg://Get ATD Tuner Rel register setting
	  {
	  *(ATD_TUNER_REL_REG_T **)pInOutVal=arrAtdRelReg;
	  }
	break;
    default:
        return ITUNER_NOT_SUPPORTED;
    }
    return ITUNER_OK;
}
