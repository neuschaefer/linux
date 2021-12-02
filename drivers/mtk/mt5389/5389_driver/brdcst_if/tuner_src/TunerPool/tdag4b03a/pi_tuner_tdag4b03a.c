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
*    Justin Wu
*
*************************************************************************/
#include "pi_def_dvbt.h"
//#include "pd_common.h"
#include "pi_dvbt_if.h"  //get demod ctx
//#include "pi_demod.h"
#include "tuner_interface_if.h"
#include "ctrl_bus.h"
#include "PI_Def.h"
// *** specify tuner related parameters here *** //

#define C_TDAG4B03A_VERSION       "TDAG4B03A 2010-01-01\n 2011-04-07\n v1.0"
// *** Release Note ***
// Ver     Date         Description
// 1.00    2006.12.12   file established
// 1.01    2007.02.06 PLL IC change

// *** Memo ***
// 24cc Combo Tuner
#define C_TDAG4B03A_I2C_DIVIDER       ((U16) 270)

#define C_TDAG4B03A_TOP_SET               ((U8)   0x03)
#define C_TDAG4B03A_TOP_SET_ANA         ((U8)   0x0A)
#define C_TDAG4B03A_SAW_BW                SAW_BW_8M

#define C_TDAG4B03A_FREQ_DBOUND_UPPER   858*1000*1000
#define C_TDAG4B03A_FREQ_DBOUND_LOWER    48*1000*1000
#define C_TDAG4B03A_FREQ_DBOUND_UPPER_Ana   858250*1000
#define C_TDAG4B03A_FREQ_DBOUND_LOWER_Ana    42250*1000

#define C_TDAG4B03A_LO_ADDRESS           ((U16)  0xc2)
#define C_TDAG4B03A_IF_FREQUENCY          ((U16)  36166)  /* kHz */
#define C_TDAG4B03A_IF_FREQUENCY_ANA      ((U16)  38900)  /* kHz */
#define C_TDAG4B03A_IF_FREQUENCY_L1       ((U16)  33900)  /* kHz */
#define C_TDAG4B03A_LO_DIVIDER_STEP       ((U32)  166670) /* Hz */
#define C_TDAG4B03A_LO_DIVIDER_STEP_ANA   ((U16)  50000) /* Hz */

#define C_TDAG4B03A_CHANNEL_SCAN_JUMP_SMALL_STEP ((U16) 250)
#define C_TDAG4B03A_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP ((U16) 400)
#define C_TDAG4B03A_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN ((U16) 400)
#define C_TDAG4B03A_AGC_IF_SLP             1
#define C_TDAG4B03A_AGC_IF_INT             0
#define C_TDAG4B03A_AGC_IF_MAX             0.499
#define C_TDAG4B03A_AGC_IF_MIN            -0.5
#define C_TDAG4B03A_AGC_IF_SLP_SGN  (S8)((C_TDAG4B03A_AGC_IF_SLP>0)?1:-1)
//#define C_TDAG4B03A_AGC_IF_LVL_MAX      0.34
//#define C_TDAG4B03A_AGC_IF_LVL_MIN     -0.078

#define C_TDAG4B03A_MAX_IF_GAIN_SET       -0.12
#define C_TDAG4B03A_MIN_IF_GAIN_SET       -0.38
#define C_TDAG4B03A_MAX_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_TDAG4B03A_MIN_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_TDAG4B03A_MAX_IF_GAIN_POW       -100    // dBm
#define C_TDAG4B03A_MIN_IF_GAIN_POW   -50     // dBm
#define C_TDAG4B03A_MAX_RF_GAIN_POW    C_TDAG4B03A_MIN_IF_GAIN_POW
#define C_TDAG4B03A_MIN_RF_GAIN_POW   -20     // dBm
#define C_TDAG4B03A_POWER_CALIBRATE        26     // dBm
#define C_TDAG4B03A_SIGNAL_LVL_TH          4

#define C_TDAG4B03A_AGC_COEF        ((U8)   0xBD)
#define C_TDAG4B03A_DEMOD_INPUT_POWER   cIF_TARGET_LEVEL_0CCC

// ********************************************* //

#define C_U8_TDAG4B03A_AGC_IF_SLP         (S8)(C_TDAG4B03A_AGC_IF_SLP *32 )
#define C_U8_TDAG4B03A_AGC_IF_INT         (S8)(C_TDAG4B03A_AGC_IF_INT *64 )
#define C_U8_TDAG4B03A_AGC_IF_MAX         (S8)(C_TDAG4B03A_AGC_IF_MAX *256)
#define C_U8_TDAG4B03A_AGC_IF_MIN         (S8)(C_TDAG4B03A_AGC_IF_MIN *256)
#define C_U8_TDAG4B03A_AGC_IF_LVL_MAX      0x50 //(S8)(C_TDAG4B03A_AGC_IF_LVL_MAX *256)//-90dBm   mapping UI signal level display 100
#define C_U8_TDAG4B03A_AGC_IF_LVL_MIN      0xE9 //(S8)(C_TDAG4B03A_AGC_IF_LVL_MIN *256)//-60dBm   mapping UI signal level display 0

#define C_U8_TDAG4B03A_MAX_IF_GAIN_SET  (S8)(C_TDAG4B03A_MAX_IF_GAIN_SET *256)
#define C_U8_TDAG4B03A_MIN_IF_GAIN_SET  (S8)(C_TDAG4B03A_MIN_IF_GAIN_SET *256)
#define C_U8_TDAG4B03A_MAX_RF_GAIN_SET  (S8)((C_TDAG4B03A_MAX_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_TDAG4B03A_MIN_RF_GAIN_SET  (S8)((C_TDAG4B03A_MIN_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_TDAG4B03A_IF_GAIN_RANGE      (S8)(C_TDAG4B03A_MIN_IF_GAIN_POW - C_TDAG4B03A_MAX_IF_GAIN_POW)
#define C_U8_TDAG4B03A_RF_GAIN_RANGE      (S8)(C_TDAG4B03A_MIN_RF_GAIN_POW - C_TDAG4B03A_MAX_RF_GAIN_POW)

//******************************************************************************//
VOID TDAG4B03A_TunerInit( ITUNER_CTX_T *pTunerCtx );

STATIC VOID   TDAG4B03A_SetSawBw( ITUNER_CTX_T * pTunerCtx,UINT8 sawbw);

INT16 TDAG4B03A_TunerSetFreq(    ITUNER_CTX_T *pTunerCtx,  PARAM_SETFREQ_T* param );

STATIC CHAR *TDAG4B03A_TunerGetVer(VOID);

STATIC INT8  TDAG4B03A_SetIFDemod( ITUNER_CTX_T * pTunerCtx,U8 SubSysIdx);
//------------------------------------------------------------------------------
/*
 *  TDAG4B03A_Initialize
 *  Tuner initialization.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
VOID TDAG4B03A_TunerInit(
    ITUNER_CTX_T *pTunerCtx
    )
{
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTunerCtx->specific_member.eu_ctx);

  pTunerCtx->I2cAddress = C_TDAG4B03A_LO_ADDRESS;
  pTunerCtx->u2IF_Freq= C_TDAG4B03A_IF_FREQUENCY;
  pEUCtx->m_SAW_BW  = C_TDAG4B03A_SAW_BW;

  pEUCtx->m_s4FreqBoundUpper = C_TDAG4B03A_FREQ_DBOUND_UPPER;
  pEUCtx->m_s4FreqBoundLower = C_TDAG4B03A_FREQ_DBOUND_LOWER;
  pEUCtx->m_s4AnaFreqBoundUpper = C_TDAG4B03A_FREQ_DBOUND_UPPER_Ana;
  pEUCtx->m_s4AnaFreqBoundLower = C_TDAG4B03A_FREQ_DBOUND_LOWER_Ana;

  //add by liuqu,20081111
  pEUCtx->m_Small_Step = C_TDAG4B03A_CHANNEL_SCAN_JUMP_SMALL_STEP;//set small step as 250k
  pEUCtx->m_Middle_Step_Up = C_TDAG4B03A_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP;//set middle step as 1500k,tda9886
  pEUCtx->m_Middle_Step_Down = C_TDAG4B03A_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN;

    // add by JC, 081215, for Signal Level display
  pEUCtx->m_ifAgc_lvl_max = (U8)C_U8_TDAG4B03A_AGC_IF_LVL_MAX;
  pEUCtx->m_ifAgc_lvl_min = (U8)C_U8_TDAG4B03A_AGC_IF_LVL_MIN;
  pEUCtx->m_Ana_Top = C_TDAG4B03A_TOP_SET_ANA;

  pEUCtx->m_aucPara[0] = C_TDAG4B03A_TOP_SET;
  pEUCtx->m_aucPara[1] = 0;

  pEUCtx->m_aucPara[2] = (U8)C_U8_TDAG4B03A_AGC_IF_SLP;
  pEUCtx->m_aucPara[3] = (U8)C_U8_TDAG4B03A_AGC_IF_INT;
  pEUCtx->m_aucPara[4] = (U8)C_U8_TDAG4B03A_AGC_IF_MAX;
  pEUCtx->m_aucPara[5] = (U8)C_U8_TDAG4B03A_AGC_IF_MIN;

  pEUCtx->m_aucPara[6] = C_TDAG4B03A_AGC_COEF;
  pEUCtx->m_aucPara[7] = C_TDAG4B03A_DEMOD_INPUT_POWER;

  pEUCtx->m_aucPara[ 8] = (U8)C_U8_TDAG4B03A_MAX_IF_GAIN_SET;
  pEUCtx->m_aucPara[ 9] = (U8)C_U8_TDAG4B03A_MIN_IF_GAIN_SET;
  pEUCtx->m_aucPara[10] = (U8)C_U8_TDAG4B03A_MAX_RF_GAIN_SET;
  pEUCtx->m_aucPara[11] = (U8)C_U8_TDAG4B03A_MIN_RF_GAIN_SET;
  pEUCtx->m_aucPara[12] = (U8)C_U8_TDAG4B03A_IF_GAIN_RANGE;
  pEUCtx->m_aucPara[13] = (U8)C_U8_TDAG4B03A_RF_GAIN_RANGE;
  pEUCtx->m_aucPara[14] = C_TDAG4B03A_POWER_CALIBRATE;
  pEUCtx->m_aucPara[21] = 0x07;

  pEUCtx->m_SigLvTh = C_TDAG4B03A_SIGNAL_LVL_TH;
  pEUCtx->m_SigLvScan = 0;
    //IFAGC sample array
    //psTunerSpecificCtx->pSignalLvlArray[TUNER_SIGNAL_LVL_ARRAY_VHF][TUNER_SIGNAL_LVL_ARRAY_8VSB]= ;//8VSB VHF band
  mcSHOW_USER_MSG(("Init " C_TDAG4B03A_VERSION "\n"));

  DEMOD_CTX_T    sDvbtDemodCtx;
  sDvbtDemodCtx.I2cAddress = 0x82;
  U8 u1Data = 0;

  DVBT_GetReg(&sDvbtDemodCtx, 0x1A4, &u1Data, 1);
  u1Data |= 0x80;
  DVBT_SetReg(&sDvbtDemodCtx, 0x1A4, &u1Data, 1);

}


INT16  TDAG4B03A_TunerSetFreq(
    ITUNER_CTX_T *pTunerCtx,
    PARAM_SETFREQ_T* param
    )
{
  UCHAR uc_data;
  S16 ucTunerLockConfCnt;
  UCHAR TunerS[12];
  U32 Lo,Freq;
  U16 Ndivider, CB1, CB2, CB3;
  S16 ii, jj;
  UCHAR u1IfAgc[2];
  //static U8 lastMode = MOD_NOT_ATV;
  //static U16 lastAutoSearch = 1;
  UINT8 AutoSearch = param->fgAutoSearch;
  UINT8 Mode = param->Modulation;

  SPECIFIC_MEMBER_EU_CTX* pEUCtx= &( pTunerCtx->specific_member.eu_ctx);


    pTunerCtx->u4RF_Freq = param->Freq;
    Freq = param->Freq;  // for coding convience,liuqu,20090424
    pTunerCtx->I2cAddress = C_TDAG4B03A_LO_ADDRESS;

  if( Mode == MOD_DVBT) //digital reception
    {
      Lo = Freq + C_TDAG4B03A_IF_FREQUENCY; // kHz
      Ndivider = (U16) ( (Lo*1000+C_TDAG4B03A_LO_DIVIDER_STEP) / C_TDAG4B03A_LO_DIVIDER_STEP);
    //Freq = Freq/1000;
    //Freq_temp = Lo/1000;

      if (Freq < (C_TDAG4B03A_FREQ_DBOUND_LOWER/1000) || Freq > (C_TDAG4B03A_FREQ_DBOUND_UPPER/1000))
      {
      mcSHOW_USER_MSG(("return -1\n"));
          return (ITUNER_NOT_OK);
      }

    CB1 = 0x80;
    CB3 = 0XC2;

    CB1 = CB1|((pEUCtx->m_aucPara[0])<<3) ; //  TOP setting 00XXX000

    if (Freq < 125000)
    {
      CB2 = 0x81;
    }
    else if (Freq < 366000)
    {
      CB2 = 0x82;
    }
    else if (Freq < 654000)
    {
      CB2 = 0x8c;
    }
    else if (Freq < 766000)
    {
      CB2 = 0xcc;
    }
    else
    {
      CB2 = 0x0C;
    }



     if (pEUCtx->m_SAW_BW == SAW_BW_8M)
        CB2 =  CB2 | 0x04;
      else
       {
            CB2 =  CB2 & 0xFB;
       }
    }
    else //analog reception
    {
        if(Mode == MOD_SECAM_L1 )
        {
            Lo = Freq + C_TDAG4B03A_IF_FREQUENCY_L1; // kHz
            Ndivider = (U16) ( (Lo*1000+C_TDAG4B03A_LO_DIVIDER_STEP_ANA/2) / C_TDAG4B03A_LO_DIVIDER_STEP_ANA);
            mcSHOW_DBG_MSG(("Freq = %d, Divider ratio = %04X PLL set freq=%d kHz\n",pTunerCtx->u4RF_Freq, Ndivider,(Ndivider*C_TDAG4B03A_LO_DIVIDER_STEP_ANA/1000-C_TDAG4B03A_IF_FREQUENCY_L1)));
            CB3=0XCE;
        }
        else
        {
            Lo = Freq + C_TDAG4B03A_IF_FREQUENCY_ANA; // kHz
            Ndivider = (U16) ( (Lo*1000+C_TDAG4B03A_LO_DIVIDER_STEP_ANA/2) / C_TDAG4B03A_LO_DIVIDER_STEP_ANA);
            mcSHOW_DBG_MSG(("Freq = %d, Divider ratio = %04X PLL set freq=%d kHz\n",pTunerCtx->u4RF_Freq, Ndivider,(Ndivider*C_TDAG4B03A_LO_DIVIDER_STEP_ANA/1000-C_TDAG4B03A_IF_FREQUENCY_ANA)));
            CB3=0XEE;
        }


        if (Freq > (C_TDAG4B03A_FREQ_DBOUND_UPPER_Ana/1000) || Freq < (C_TDAG4B03A_FREQ_DBOUND_LOWER_Ana/1000))
        {
        //    TRACE(("Out of range for LO!\n"));
              mcSHOW_USER_MSG(("return -1\n"));
              return (ITUNER_NOT_OK);
        }

    CB1 = 0x92; //  Lo step 50kHz


      if (Freq < 121750)         //Alps agt Mar_10 yuan
    {
      CB2 = 0x01;
    }
      else if (Freq < 360250)   //Alps sgt Mar_10 yuan
    {
      CB2 = 0x02;
    }
    else
    {
      CB2 = 0x08;
    }
     //  TRACE((" CB2= %X \n",CB2));
    }

    mcSHOW_DRVAPI_MSG(("CB1=%x,CB2=%x,CB3=%x\n",CB1,CB2,CB3));

    // LO settings
    TunerS[0] = (UCHAR) 0x7F & (Ndivider >> 8);
    TunerS[1] = (UCHAR) 0xFF & Ndivider;
    TunerS[2] = (UCHAR) CB1;
    TunerS[3] = (UCHAR) CB2;
    TunerS[4] = (UCHAR) CB3;

    for (jj=0; jj<2; jj++)
    {
      if (ICtrlBus_I2cTunerWrite(C_TDAG4B03A_I2C_DIVIDER,pTunerCtx->I2cAddress,TunerS,5) > 0)
      {
      mcSHOW_USER_MSG(("\nTunerSet() failed!\n"));
      mcSHOW_USER_MSG(("return -1\n"));
          return (ITUNER_NOT_OK);
      }



    ucTunerLockConfCnt = 0;
    // increase timeout due to long TDAG4 PLL lock time (worst >= 184ms)
    for (ii=0; ii < 50; ii++)           //Alps sgt the max lock time is 250ms
    {
      mcDELAY(5);
      ICtrlBus_I2cTunerRead(C_TDAG4B03A_I2C_DIVIDER,pTunerCtx->I2cAddress,  &uc_data,1);
      if (mcGET_FIELD(uc_data, 0x40, 6))  // if PLL lock
          ucTunerLockConfCnt++;
      else
          ucTunerLockConfCnt = 0;

      if (ucTunerLockConfCnt>ccTUNER_LOCK_CONF_THRESHOLD)
        break;

    }
    ucTunerLockConfCnt = ii;

    if(ii < 50)
    {
        if(Mode == MOD_DVBT)
        {
            mcDELAY(700);   // for ALPS tuner RFAGC convergence time
            if(AutoSearch)
                {
                DEMOD_CTX_T    sDvbtDemodCtx;
                sDvbtDemodCtx.I2cAddress = 0x82;


                mcDELAY(80);
                /*** Record IfAgc value for channel scan ***/  //Menghau, 080314
                //Choose reg bank 1
                DVBT_GetReg(&sDvbtDemodCtx, 0x1DB, u1IfAgc, 2);
                pEUCtx->m_SigLvScan = -(((S8)u1IfAgc[1])*C_TDAG4B03A_AGC_IF_SLP_SGN);
                mcSHOW_DRVAPI_MSG(("IfAgc = %d\n", pEUCtx->m_SigLvScan));

                }
         }

         mcSHOW_DBG_MSG(("Freq = %d KHz, the %d-th time setting PLL success, cnt = %d\n" , Freq, jj+1, ucTunerLockConfCnt));
         break;
    }
    else
    {
      mcSHOW_DBG_MSG(("Freq = %d KHz, the %d-th time setting PLL fail\n" , Freq, jj+1));
    }

    }

  //pTunerCtx->m_aucPara[1] = TunerS[0];  //send register value to UI, LC 070102
  //pTunerCtx->m_aucPara[15] = TunerS[1];
  //pTunerCtx->m_aucPara[16] = TunerS[2];
  //pTunerCtx->m_aucPara[17] = TunerS[3];
  //pTunerCtx->m_aucPara[18] = TunerS[4];
  //pTunerCtx->m_aucPara[19] = uc_data;
  mcDELAY(10);  // add 10ms delay before bypass is turn off, LC 070613
  mcSHOW_USER_MSG(("return 0\n"));
    return (ITUNER_OK);
}

//Analog DEMO R10405 settings
STATIC INT8   TDAG4B03A_SetIFDemod(ITUNER_CTX_T * pTunerCtx,U8 SubSysIdx)
{


  UCHAR TunerS[4],M[3],ii;
  UCHAR AFC;
  AFC = 0xFF;
  TunerS[0] = 0x84; // IF IC slave address
  TunerS[3] = 0x84; // IF IC read address
  //memset(M,0,3);

    switch (SubSysIdx)
    {
    case MOD_PAL_BG:
    M[0] = 0x44;
        //  M[1] = 0x40;//ALPS SGT
    M[1] = 0x20;
    M[2] = 0x33;
    mcSHOW_DBG_MSG(("TDAG4B03A_SetIFDemod :BG\n"));
    break;
    case MOD_PAL_DK:
    M[0] = 0x44;
      //  M[1] = 0x40;//ALPS SGT
    M[1] = 0x20;
    M[2] = 0x53;
    mcSHOW_DBG_MSG(("TDAG4B03A_SetIFDemod :DK\n"));
    break;
    case MOD_PAL_I:
    M[0] = 0x44;
      //  M[1] = 0x40;//ALPS SGT
    M[1] = 0x20;
        M[2] = 0x73;
    mcSHOW_DBG_MSG(("TDAG4B03A_SetIFDemod :I\n"));
    break;
  case MOD_SECAM_L:

    M[0] = 0x4c;
      //  M[1] = 0x40;//ALPS SGT
    M[1] = 0x20;
    M[2] = 0xb3;
    mcSHOW_DBG_MSG(("TDAG4B03A_SetIFDemod :L\n"));
    break;
  case MOD_SECAM_L1:

    M[0] = 0x48;
      //  M[1] = 0x40;//ALPS SGT
    M[1] = 0x20;
    M[2] = 0xd3;
    mcSHOW_DBG_MSG(("TDAG4B03A_SetIFDemod :L1\n"));
    break;

  default:

    M[0] = 0x44;
  //  M[1] = 0x40;//ALPS SGT
    M[1] = 0x20;
    M[2] = 0x33;

    break;
  }
  mcDELAY(2);

for(ii =0;ii<3;ii++)   //write data seprately. Slave addr---sub addr---data
   {
        TunerS[1] = (UCHAR)ii;  //sub addr
        TunerS[2] = M[ii] ;
        if (ICtrlBus_I2cTunerWrite(C_TDAG4B03A_I2C_DIVIDER,TunerS[0], &TunerS[1], 2) > 0)
        {
            //     TRACE(("\nTunerSet() failed!\n"));
            return (ITUNER_NOT_OK);
        }
        mcDELAY(10);
   }
  //mcDELAY(70);
   mcDELAY(10);
    // for S-curve log
      ICtrlBus_I2cTunerRead(C_TDAG4B03A_I2C_DIVIDER,TunerS[3], & AFC,1);

      AFC = ((AFC & 0XE0)>>5);
      mcSHOW_DBG_MSG(("AFT = %d\n",AFC));
      //    pTunerCtx->m_aucPara[19] = AFC;
  return (ITUNER_OK);

}
//------------------------------------------------------------------------------------
STATIC VOID   TDAG4B03A_SetSawBw( ITUNER_CTX_T * pTunerCtx,UINT8 sawbw)
{
   pTunerCtx->specific_member.eu_ctx.m_SAW_BW = sawbw ;
}
STATIC CHAR *TDAG4B03A_TunerGetVer(VOID)
{
    return (C_TDAG4B03A_VERSION);
}


//-----------------------------------------------------------------------------
/** TDAG4B03A_TunerOP
 * Set/Get function
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  eOperation       To Get/Set operation id
 *  @retval  ITUNER_OK    Operation Success
 *  @retval  ITUNER_NOT_OK    Operation fail
 *  @retval  ITUNER_NOT_SUPPORTED Operation not supported in  driver
 */
//-----------------------------------------------------------------------------
INT16 TDAG4B03A_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal)
{

    switch (eOperation)
    {
    case itGetVer:
        {
            char ** RetStr=(CHAR **)pInOutVal;
            *RetStr=TDAG4B03A_TunerGetVer();
        }
    break;
    case itSetIfDemod:
        {
             BOOL * Retval = (BOOL *)pInOutVal;
            * Retval = TDAG4B03A_SetIFDemod(pTCtx, SetVal);
        }
    break;
    case itSetSawBw:
        TDAG4B03A_SetSawBw(pTCtx, SetVal);
    break;
    default:
        return ITUNER_NOT_SUPPORTED;
    }
    return ITUNER_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

