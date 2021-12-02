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
*    YuanLiu,Apr14.09
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
#include "eq_script_ft2607.h"
#include "drvcust_if.h"
// *** specify tuner related parameters here *** //

#define C_FT2607_VERSION        "FT2607 2010-01-01\n 2011-04-07\n v1.0"
// *** Release Note ***
// Ver     Date         Description
// 1.00    2009.04.14   file established

//#define GPIO_AGC_SWITCH     201
#define GPIO_AnaAGC      0
#define GPIO_DigiAGC     1

#define C_FT2607_I2C_DIVIDER       ((U16) 270)

#define C_FT2607_TOP_SET             ((U8)   0x00) //SPEC IS 0X01,VENDOR SGT 0X00
//#define C_FT2607_TOP_SET_ANA         ((U8)   0x60)
#define C_FT2607_SAW_BW              SAW_BW_8M

//#define C_FT2607_FREQ_DBOUND_UPPER   858*1000*1000
#define C_FT2607_FREQ_DBOUND_UPPER   859*1000*1000     //modified for DVB-C application, Menghau, 091020
#define C_FT2607_FREQ_DBOUND_LOWER    45*1000*1000
#define C_FT2607_FREQ_DBOUND_UPPER_Ana   875*1000*1000
#define C_FT2607_FREQ_DBOUND_LOWER_Ana    45*1000*1000

#define C_FT2607_LO_ADDRESS         ((U16)  0xc2)
#define C_FT2607_IF_FREQUENCY        ((U16)  36167)  /* kHz */
#define C_FT2607_IF_FREQUENCY_DVBC        ((U16)  36167)  /* kHz */   //Add by liuqu,20090915
#define C_FT2607_IF_FREQUENCY_ANA    ((U16)  38900)  /* kHz */
#define C_FT2607_IF_FREQUENCY_L1     ((U16)  32900)  /* kHz */
#define C_FT2607_LO_DIVIDER_STEP     ((U32) 166667) /* Hz */
#define C_FT2607_LO_DIVIDER_STEP_DVBC    ((U32) 50000) /* Hz */
#define C_FT2607_LO_DIVIDER_STEP_ANA ((U16)  62500) /* Hz */

#define C_FT2607_CHANNEL_SCAN_JUMP_SMALL_STEP ((U16) 250)
#define C_FT2607_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP ((U16) 1500)
#define C_FT2607_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN ((U16) 500)


#define C_FT2607_AGC_IF_SLP           1.0
#define C_FT2607_AGC_IF_INT           0.0
#define C_FT2607_AGC_IF_MAX           0.499  // 0.5 will overflow
#define C_FT2607_AGC_IF_MIN          -0.5
#define C_FT2607_AGC_IF_LVL_MAX       0.14453125  //-90dBm   37/256
#define C_FT2607_AGC_IF_LVL_MIN      -0.140625   //-60dBm    -36/256
#define C_FT2607_AGC_IF_SLP_SGN  (S8)((C_FT2607_AGC_IF_SLP>0)?1:-1)

#define C_FT2607_MAX_IF_GAIN_SET      -0.12
#define C_FT2607_MIN_IF_GAIN_SET      -0.38
#define C_FT2607_MAX_RF_GAIN_SET       cALC_ADC_BIAS *2   // Volt
#define C_FT2607_MIN_RF_GAIN_SET       cALC_ADC_BIAS *2   // Volt
#define C_FT2607_MAX_IF_GAIN_POW      -100    // dBm
#define C_FT2607_MIN_IF_GAIN_POW  -50     // dBm
#define C_FT2607_MAX_RF_GAIN_POW   C_FT2607_MIN_IF_GAIN_POW
#define C_FT2607_MIN_RF_GAIN_POW  -20     // dBm
#define C_FT2607_POWER_CALIBRATE       26     // dBm

#define C_FT2607_AGC_COEF   ((U8)   0x9B)
#define C_FT2607_DEMOD_INPUT_POWER  cIF_TARGET_LEVEL_0CCC

#define C_FT2607_DVBC_EARLY_BREAK_IF_AGC_THRESHOLD   170   //ealarge
// ********************************************* //

#define C_U8_FT2607_AGC_IF_SLP         (S8)(C_FT2607_AGC_IF_SLP *32 )
#define C_U8_FT2607_AGC_IF_INT         (S8)(C_FT2607_AGC_IF_INT *64 )
#define C_U8_FT2607_AGC_IF_MAX         (S8)(C_FT2607_AGC_IF_MAX *256)
#define C_U8_FT2607_AGC_IF_MIN         (S8)(C_FT2607_AGC_IF_MIN *256)
#define C_U8_FT2607_AGC_IF_LVL_MAX     0x7F//(S8)(C_FT2607_AGC_IF_LVL_MAX *256)  //no signal
#define C_U8_FT2607_AGC_IF_LVL_MIN     0xEB //(S8)(C_FT2607_AGC_IF_LVL_MIN *256)  //-70dbm input

#define C_U8_FT2607_MAX_IF_GAIN_SET (S8)(C_FT2607_MAX_IF_GAIN_SET *256)
#define C_U8_FT2607_MIN_IF_GAIN_SET (S8)(C_FT2607_MIN_IF_GAIN_SET *256)
#define C_U8_FT2607_MAX_RF_GAIN_SET (S8)((C_FT2607_MAX_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_FT2607_MIN_RF_GAIN_SET (S8)((C_FT2607_MIN_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_FT2607_IF_GAIN_RANGE      (S8)(C_FT2607_MIN_IF_GAIN_POW - C_FT2607_MAX_IF_GAIN_POW)
#define C_U8_FT2607_RF_GAIN_RANGE      (S8)(C_FT2607_MIN_RF_GAIN_POW - C_FT2607_MAX_RF_GAIN_POW)

STATIC UINT8* pFt2607EqNormal[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pFt2607EqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8  u1PreMode = 0xFF;
//----------------------------------------------------------------------------------------------
 void  FT2607_TunerInit(ITUNER_CTX_T* pTCtx);

 STATIC void  FT2607_SetSawBw(ITUNER_CTX_T * pTunerCtx, UINT8 sawbw)
 {
   pTunerCtx->specific_member.eu_ctx.m_SAW_BW = sawbw;
 }

  //---------------------------------------------------------------------------------------------
 STATIC CHAR* FT2607_TunerGetVer(void)
 {
  return ((CHAR*)C_FT2607_VERSION);
}

FUNC_EXPORT void FT2607_TunerInit(ITUNER_CTX_T *pTCtx)
{

ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);
    
    psTunerCtx->I2cAddress = C_FT2607_LO_ADDRESS;
    psTunerCtx->u2IF_Freq = C_FT2607_IF_FREQUENCY;
    psTunerCtx->u4RF_Freq = 0;
    //Justin TODO
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_DRO|MASK_PATCH_SENS);

    // AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x2f; //EC_code_d9930: Change 0x151 value for EU new BPF
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0xe3; //EC_code_d9930: Change 0x158 value for EU new BPF
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B; //EC_code_d9930: Change 0x15F value for EU new BPF

    pEUCtx->m_SAW_BW  = C_FT2607_SAW_BW;
    //pEUCtx->m_Ana_Top = C_FT2607_TOP_SET_ANA;
    pEUCtx->m_s4FreqBoundUpper = C_FT2607_FREQ_DBOUND_UPPER;
    pEUCtx->m_s4FreqBoundLower = C_FT2607_FREQ_DBOUND_LOWER;
    pEUCtx->m_s4AnaFreqBoundUpper = C_FT2607_FREQ_DBOUND_UPPER_Ana;
    pEUCtx->m_s4AnaFreqBoundLower = C_FT2607_FREQ_DBOUND_LOWER_Ana;
    
    pEUCtx->m_Small_Step = C_FT2607_CHANNEL_SCAN_JUMP_SMALL_STEP;
    pEUCtx->m_Middle_Step_Up = C_FT2607_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP;
    pEUCtx->m_Middle_Step_Down = C_FT2607_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN;
    
    // add by JC, 081215, for Signal Level display
    pEUCtx->m_ifAgc_lvl_max = (U8)C_U8_FT2607_AGC_IF_LVL_MAX;
    pEUCtx->m_ifAgc_lvl_min = (U8)C_U8_FT2607_AGC_IF_LVL_MIN;
    
    pEUCtx->m_SigLvTh = 0;//C_TDTWS710D_SIGNAL_LVL_TH;
    pEUCtx->m_SigLvScan = 0;
    
    pEUCtx->m_aucPara[0] = C_FT2607_TOP_SET;
    pEUCtx->m_aucPara[1] = 0;
    
    pEUCtx->m_aucPara[2] = (U8)C_U8_FT2607_AGC_IF_SLP;
    pEUCtx->m_aucPara[3] = (U8)C_U8_FT2607_AGC_IF_INT;
    pEUCtx->m_aucPara[4] = (U8)C_U8_FT2607_AGC_IF_MAX;
    pEUCtx->m_aucPara[5] = (U8)C_U8_FT2607_AGC_IF_MIN;
    
    pEUCtx->m_aucPara[6] = C_FT2607_AGC_COEF;
    pEUCtx->m_aucPara[7] = C_FT2607_DEMOD_INPUT_POWER;
    
    pEUCtx->m_aucPara[ 8] = (U8)C_U8_FT2607_MAX_IF_GAIN_SET;
    pEUCtx->m_aucPara[ 9] = (U8)C_U8_FT2607_MIN_IF_GAIN_SET;
    pEUCtx->m_aucPara[10] = (U8)C_U8_FT2607_MAX_RF_GAIN_SET;
    pEUCtx->m_aucPara[11] = (U8)C_U8_FT2607_MIN_RF_GAIN_SET;
    pEUCtx->m_aucPara[12] = (U8)C_U8_FT2607_IF_GAIN_RANGE;
    pEUCtx->m_aucPara[13] = (U8)C_U8_FT2607_RF_GAIN_RANGE;
    pEUCtx->m_aucPara[14] = C_FT2607_POWER_CALIBRATE;

    pFt2607EqNormal[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQFt2607BG;
    pFt2607EqNormal[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQFt2607DK;
    pFt2607EqNormal[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQFt2607I;
    pFt2607EqNormal[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQFt2607L;
    pFt2607EqNormal[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQFt2607L;
	/*
    pFt2607EqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQFt2607BGWeak;
    pFt2607EqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQFt2607DKWeak;
    pFt2607EqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQFt2607IWeak;
    pFt2607EqWeak[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQFt2607LWeak;
    pFt2607EqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQFt2607LWeak;
	*/
    pFt2607EqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQFt2607BG;
    pFt2607EqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQFt2607DK;
    pFt2607EqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQFt2607I;
    pFt2607EqWeak[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQFt2607L;
    pFt2607EqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQFt2607L;

    mcSHOW_USER_MSG(("Tuner FT2607 init done!\n"));

}


 INT16 FT2607_TunerSetFreq(
    ITUNER_CTX_T *pTunerCtx,
    PARAM_SETFREQ_T* param
  )
{
  UCHAR uc_data;
  S16 ucTunerLockConfCnt;
  UCHAR TunerS[5];
  U32 Lo,Freq;
  U16 Ndivider, CB11, CB2, CB12;
  S16 ii, jj;
  UCHAR u1IfAgc[2];
  UINT8 AutoSearch = param->fgAutoSearch;
  UINT8 Mode = param->Modulation;
  UINT32 GPIO_AGC_SWITCH;


  SPECIFIC_MEMBER_EU_CTX* pEUCtx= &( pTunerCtx->specific_member.eu_ctx);

  GPIO_AGC_SWITCH = DRVCUST_OptGet(eTunerCtrlAna);
 



    pTunerCtx->u4RF_Freq = param->Freq;
    Freq = param->Freq;  // for coding convience,liuqu,20090424
    pTunerCtx->I2cAddress = C_FT2607_LO_ADDRESS;


    if( Mode == MOD_DVBT) //digital reception
    {
      mcSHOW_DBG_MSG(("Switch to Digi AGC (GPIO%d)->1\n", GPIO_AGC_SWITCH));  //Yuan,22.Apr,09
      GPIO_SetOut(GPIO_AGC_SWITCH, GPIO_DigiAGC); //  Digital AGC control
      CB11=0XCA;
      Lo = Freq + C_FT2607_IF_FREQUENCY; // kHz
      Ndivider = (U16) ( (Lo*1000+C_FT2607_LO_DIVIDER_STEP/2) / C_FT2607_LO_DIVIDER_STEP);
      Freq = Freq/1000;
//	 Lo=Lo/1000;

      if (Freq < 40 || Freq > 872)
      {
        mcSHOW_HW_MSG(("Out of range for LO!\n"));
       //return (+1);
      }

      if (Lo <= 126000)
    {
      CB2 = 0x41;
    }
    else if (Lo <= 150000)
    {
      CB2 = 0x61;
    }
    else if (Lo <= 175000)
    {
      CB2 = 0x81;
    }
    else if (Lo <= 193000)
    {
      CB2 = 0xC1;
    }
    else if (Lo <= 336000)
    {
      CB2 = 0x42;
    }
    else if (Lo <= 393000)
    {
      CB2 = 0x62;
    }
    else if (Lo <= 425000)
    {
      CB2 = 0x82;
    }
    else if (Lo <= 456000)
    {
      CB2 = 0xA2;
    }
    else if (Lo <= 480000)
    {
      CB2 = 0xC2;
    }
    else if (Lo <= 691000)
    {
      CB2 = 0x64;
    }
    else if (Lo <= 811000)
    {
      CB2 = 0x84;
    }
    else if (Lo <= 859000)
    {
      CB2 = 0xA4;
    }
    else
    {
      CB2 = 0xE4;
    }
      //CB12=0X89;
     //merge TOP setting
      CB12=0X81;  //ATC=0 is normal 
      CB12 = (CB12&0XF8)|(( pEUCtx->m_aucPara[0]&0x07));  // 0x ****XXXX
     //TRACE("pTunerCtx->m_aucPara[0]=%d\n",pTunerCtx->m_aucPara[0]);

    }
	else if (Mode == MOD_DVBC)
     {
      mcSHOW_DBG_MSG(("Switch to Digi AGC (GPIO%d)->1\n", GPIO_AGC_SWITCH));  //Yuan,22.Apr,09
      GPIO_SetOut(GPIO_AGC_SWITCH, GPIO_DigiAGC); //  Digital AGC control
      CB11=0XCB;  //step size = 50K
      Lo = Freq + C_FT2607_IF_FREQUENCY_DVBC; // kHz
      Ndivider = (U16) ( (Lo*1000+C_FT2607_LO_DIVIDER_STEP_DVBC/2) / C_FT2607_LO_DIVIDER_STEP_DVBC);
      Freq = Freq/1000;
	// Lo=Lo/1000;

      if (Freq < 40 || Freq > 872)
      {
          mcSHOW_HW_MSG(("Out of range for LO!\n"));
      // return (+1);
      }

      if (Lo <= 131000)
    {
      CB2 = 0x01;
    }
    else if (Lo <= 162000)
    {
      CB2 = 0x21;
    }
    else if (Lo <= 172000)
    {
      CB2 = 0x41;
    }
    else if (Lo <= 182000)
    {
      CB2 = 0x61;
    }
    else if (Lo <= 190000)
    {
      CB2 = 0x81;
    }
    else if (Lo <= 196150)
    {
      CB2 = 0xA1;
    }
    else if (Lo <= 320000)
    {
      CB2 = 0x02;
    }
    else if (Lo <= 377000)
    {
      CB2 = 0x22;
    }
    else if (Lo <= 417000)
    {
      CB2 = 0x42;
    }
    else if (Lo <= 443000)
    {
      CB2 = 0x62;
    }
	else if (Lo <= 464000)
    {
      CB2 = 0x82;
    }
	else if (Lo <= 482150)
    {
      CB2 = 0xA2;
    }
	else if (Lo <= 518000)
    {
      CB2 = 0x04;
    }
	else if (Lo <= 689000)
    {
      CB2 = 0x24;
    }
	else if (Lo <= 772000)
    {
      CB2 = 0x44;
    }
	else if (Lo <= 828000)
    {
      CB2 = 0x64;
    }
	else if (Lo <= 865000)
    {
      CB2 = 0x84;
    }
    else
    {
      CB2 = 0xA4;
    }
	
      CB12=0X83;
    }

    else //analog reception
    {
       mcSHOW_DBG_MSG(("Switch to Ana AGC (GPIO%d)->0\n", GPIO_AGC_SWITCH));   //Yuan,22.Apr,09
       GPIO_SetOut(GPIO_AGC_SWITCH, GPIO_AnaAGC); //analog AGC control

       CB11=0XC8;  //step size = 62.5K
       if( Mode == MOD_SECAM_L1 )
      {
        Lo = Freq + C_FT2607_IF_FREQUENCY_L1; // kHz
        Ndivider = (U16) ( (Lo*1000+C_FT2607_LO_DIVIDER_STEP_ANA/2) / C_FT2607_LO_DIVIDER_STEP_ANA);

        mcSHOW_DBG_MSG(("Freq = %d, Divider ratio = %04X PLL set freq=%d kHz\n",pTunerCtx->u4RF_Freq, Ndivider));
        mcSHOW_DBG_MSG(("PLL set freq=%d kHz\n",(Ndivider*C_FT2607_LO_DIVIDER_STEP_ANA/1000-C_FT2607_IF_FREQUENCY_L1)));
      }
      else
      {
        Lo = Freq + C_FT2607_IF_FREQUENCY_ANA; // kHz
        Ndivider = (U16) ( (Lo*1000+C_FT2607_LO_DIVIDER_STEP_ANA/2) / C_FT2607_LO_DIVIDER_STEP_ANA);
        mcSHOW_DBG_MSG(("Freq = %d, Divider ratio = %04X,PLL set freq=%d kHz\n",pTunerCtx->u4RF_Freq, Ndivider,(Ndivider*C_FT2607_LO_DIVIDER_STEP_ANA/1000-C_FT2607_IF_FREQUENCY_ANA)));

      }

      // Conver Freq to unit of MHz
     Freq= Freq/1000;
//	 Lo=Lo/1000;
    if (Freq < 40 || Freq> 872)
      {
          mcSHOW_HW_MSG(("Out of range for LO!\n"));
         // return (+1);
      }

      if (Lo <= 169000)
    {
      CB2 = 0x01;
    }
    else if (Lo <= 179000)
    {
      CB2 = 0x21;
    }
    else if (Lo <= 189000)
    {
      CB2 = 0x41;
    }
    else if (Lo <= 196150)
    {
      CB2 = 0x61;
    }
    else if (Lo <= 406000)
    {
      CB2 = 0x02;
    }
    else if (Lo <= 438000)
    {
      CB2 = 0x22;
    }
    else if (Lo <= 461000)
    {
      CB2 = 0x42;
    }
    else if (Lo <= 482150)
    {
      CB2 = 0x62;
    }
    else if (Lo <= 750000)
    {
      CB2 = 0x04;
    }
    else if (Lo <= 817000)
    {
      CB2 = 0x24;
    }
    else if (Lo <= 862000)
    {
      CB2 = 0x44;
    }
    else if (Lo <= 882000)
    {
      CB2 = 0x64;
    }
	 else if (Lo <= 895000)
    {
      CB2 = 0x84;
    }
    else
    {
      CB2 = 0xA4;
    }
      CB12=0X86;      //ATC_AL2_AL1_AL0 = 0110   EXTERNAL AGC CONTROL
      //merge TOP setting
     //CB12 = (CB12&0XF0)|((pTunerCtx->m_aucPara[19]&0x0F));  // 0x ****XXXX

    }

    // LO settings
    TunerS[0] = (UCHAR) 0x7F & (Ndivider >> 8);
    TunerS[1] = (UCHAR) 0xFF & Ndivider;
    TunerS[2] = (UCHAR) CB11;
    TunerS[3] = (UCHAR) CB2;
    TunerS[4] = (UCHAR) CB12|0x08;

    for (jj=0; jj<2; jj++)
    {
      if (ICtrlBus_I2cTunerWrite(C_FT2607_I2C_DIVIDER,pTunerCtx->I2cAddress, TunerS,5) > 0)
      {
          return (-1);
      }
    ucTunerLockConfCnt = 0;
    for (ii=0; ii<20; ii++)
    {
    mcDELAY(5);// wait PLL lock
      ICtrlBus_I2cTunerRead(C_FT2607_I2C_DIVIDER,pTunerCtx->I2cAddress, &uc_data,1);
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
                  pEUCtx->m_aucPara[1] =  (UCHAR)TunerS[0]; //send register value to UI, LC 070102
                  pEUCtx->m_aucPara[15] = (UCHAR)TunerS[1];
                  pEUCtx->m_aucPara[16] = (UCHAR)TunerS[2];
                  pEUCtx->m_aucPara[17] = (UCHAR)TunerS[3];
                  pEUCtx->m_aucPara[18] = (UCHAR)TunerS[4]; 
    if ((u1PreMode != MOD_DVBC) && (u1PreMode != MOD_DVBT) && ((Mode == MOD_DVBC) || (Mode == MOD_DVBT))) {
	if (!AutoSearch)
	{
        mcDELAY_MS(1000);  //Delay 1000ms after switching to internal RFAGC to wait RFAGC stable
        mcSHOW_DBG_MSG(("channel change :Wait 1000ms after switching to internal RFAGC\n"));
	}	
        else
        {
	 mcDELAY_MS(3000);		
        mcSHOW_DBG_MSG(("auto search :Wait 3000ms after switching to internal RFAGC\n"));
        }	
    }

    u1PreMode = Mode;

// set ATC Bit to 0
    if (!AutoSearch)
    {
        TunerS[4] = (UCHAR) CB12; // set ATC = 1'b0
        if (ICtrlBus_I2cTunerWrite(C_FT2607_I2C_DIVIDER, pTunerCtx->I2cAddress, TunerS, 5))
        {
            mcSHOW_DRVERR_MSG(("TunerWrite PLL failed!\n"));
            return (+1);
        }
    }

    if( (Mode ==MOD_DVBT ) && AutoSearch)
    {
    DEMOD_CTX_T    sDvbtDemodCtx;
    sDvbtDemodCtx.I2cAddress = 0x82;
    mcDELAY(80);
     /*** Record IfAgc value for channel scan ***/  //Menghau, 080314
    DVBT_GetReg(&sDvbtDemodCtx, 0x1DB, u1IfAgc, 2);
    pEUCtx->m_SigLvScan = -(((S8)u1IfAgc[1])*C_FT2607_AGC_IF_SLP_SGN);
    mcSHOW_DRVAPI_MSG(("IfAgc = %d\n", pEUCtx->m_SigLvScan));
    /*** Record IfAgc value before LNA on for BestMux selection ***/
    }
    return (0);
}

//---------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------
INT16 FT2607_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal)
{

    switch (eOperation)
    {
    case itGetVer:
                 {
                    CHAR ** RetStr=(CHAR **)pInOutVal;
                    *RetStr=FT2607_TunerGetVer();
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
				if (ICtrlBus_I2cTunerWrite(C_FT2607_I2C_DIVIDER,pTCtx->I2cAddress, TunerS,5) > 0)
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
				ICtrlBus_I2cTunerRead(C_FT2607_I2C_DIVIDER,pTCtx->I2cAddress, &uc_data,1);
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
                 FT2607_SetSawBw(pTCtx, SetVal);
    break;
    case itSetLNA:
    break;

	case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pFt2607EqNormal[SetVal];
        break;
    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pFt2607EqWeak[SetVal];
        break;
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = NULL;
        break;

    case itSetIfMin:
		 {
            UINT8 subSysId = SetVal;
			UINT8 *ifMinPtr = (UINT8*)pInOutVal;

			mcSHOW_DBG_MSG(("NuTune_FT2607 TOP Setting\n"));
            switch(subSysId){
                case MOD_PAL_BG:
			*ifMinPtr = 0xea;   // liuqu,20100306,rui update
			break;
                case MOD_PAL_I:                    
                case MOD_PAL_DK:
                    *ifMinPtr = 0xef;   // liuqu,20100306 udate
                    break;
                case MOD_SECAM_L:
                case MOD_SECAM_L1:
                     *ifMinPtr = 0xF3;
                    break;
                default:
                    mcSHOW_DBG_MSG(("TOP No change\n"));
                    break;
            }

	     }
         break;
    case itIFAGCTreshold:
	{
        *(UINT8 *) pInOutVal = C_FT2607_DVBC_EARLY_BREAK_IF_AGC_THRESHOLD;
    	}
	break;
    default:
    return ITUNER_NOT_SUPPORTED;
    }
    return ITUNER_OK;
}
