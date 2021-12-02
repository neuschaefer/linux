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
 * $RCSfile: pi_tuner_env5sdf.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_env5sdf.c
 *  Panasonic ENV5SDF tuner driver for TW.
 */


#include "tuner_interface_if.h"
#include "pi_def_dvbt.h"
#include "PI_Def.h"
#include "ctrl_bus.h"
#include "pi_dvbt_if.h"  //add by liuqu,20090424
#include "pi_demod_atd.h"
#include "eq_script_env5sdf.h"


// *** Release Note ***
// Ver     Date         Description
// 1.00    2009.12.14   file established
#define C_ENV5SDF_VERSION				"ENV5SDF v1.00"
#define C_ENV5SDF_PLL_POLL_INTERVAL   10   // to solve SLT thread UART overflow issue, 090601, Ken
#define C_ENV5SDF_PLL_TIMEOUT         100
#define C_ENV5SDF_PLL_TIMEOUT_CNT     C_ENV5SDF_PLL_TIMEOUT/C_ENV5SDF_PLL_POLL_INTERVAL
#define C_ENV5SDF_I2C_CLK_DIV         0x100
#define C_ENV5SDF_PLL_LOCK_BIT        6

#define C_ENV5SDF_TOP_SET             ((U8)   0x02) 
#define C_ENV5SDF_TOP_SET_ANA         ((U8)   0x0A) 
#define C_ENV5SDF_SAW_BW              SAW_BW_6M 


#define C_ENV5SDF_FREQ_DBOUND_UPPER        864*1000*1000      
#define C_ENV5SDF_FREQ_DBOUND_LOWER        54*1000*1000 
#define C_ENV5SDF_FREQ_DBOUND_UPPER_Ana    864*1000*1000
#define C_ENV5SDF_FREQ_DBOUND_LOWER_Ana    54*1000*1000

#define C_ENV5SDF_LO_ADDRESS          ((U16)  0xc2)
#define C_ENV5SDF_IF_FREQUENCY        ((U16)  36167)  /* kHz */
#define C_ENV5SDF_IF_FREQUENCY_ANA    ((U16)  38000)  /* kHz ,NTSC-M*/
#define C_ENV5SDF_LO_DIVIDER_STEP     ((U32)  166667) /* Hz */
#define C_ENV5SDF_LO_DIVIDER_STEP_ANA ((U16)  62500) /* Hz */


#define C_ENV5SDF_CHANNEL_SCAN_JUMP_SMALL_STEP ((U16) 250)
#define C_ENV5SDF_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP ((U16) 1500)
#define C_ENV5SDF_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN ((U16) 500)


#define C_ENV5SDF_AGC_IF_SLP           1.0
#define C_ENV5SDF_AGC_IF_INT           0.0
#define C_ENV5SDF_AGC_IF_MAX           0.499  // 0.5 will overflow
#define C_ENV5SDF_AGC_IF_MIN          -0.5
#define C_ENV5SDF_AGC_IF_SLP_SGN  (S8)((C_ENV5SDF_AGC_IF_SLP>0)?1:-1) 
#define C_ENV5SDF_AGC_IF_LVL_MAX      0.4774    //-90dBm    127/256
#define C_ENV5SDF_AGC_IF_LVL_MIN      0.08203125   //-60dBm  21/256


#define C_ENV5SDF_MAX_IF_GAIN_SET       -0.12   
#define C_ENV5SDF_MIN_IF_GAIN_SET       -0.38   
#define C_ENV5SDF_MAX_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_ENV5SDF_MIN_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_ENV5SDF_MAX_IF_GAIN_POW       -100    // dBm
#define C_ENV5SDF_MIN_IF_GAIN_POW		-50     // dBm
#define C_ENV5SDF_MAX_RF_GAIN_POW		 C_ENV5SDF_MIN_IF_GAIN_POW
#define C_ENV5SDF_MIN_RF_GAIN_POW		-20     // dBm
#define C_ENV5SDF_POWER_CALIBRATE        26     // dBm

#define C_ENV5SDF_AGC_COEF			((U8)   0xBD)
#define C_ENV5SDF_DEMOD_INPUT_POWER	cIF_TARGET_LEVEL_0CCC_0D36

// ********************************************* //

#define C_U8_ENV5SDF_AGC_IF_SLP         (S8)(C_ENV5SDF_AGC_IF_SLP *32 )
#define C_U8_ENV5SDF_AGC_IF_INT         (S8)(C_ENV5SDF_AGC_IF_INT *64 )
#define C_U8_ENV5SDF_AGC_IF_MAX         (S8)(C_ENV5SDF_AGC_IF_MAX *256)
#define C_U8_ENV5SDF_AGC_IF_MIN         (S8)(C_ENV5SDF_AGC_IF_MIN *256)
#define C_U8_ENV5SDF_AGC_IF_LVL_MAX        (S8)(C_ENV5SDF_AGC_IF_LVL_MAX *256)
#define C_U8_ENV5SDF_AGC_IF_LVL_MIN         (S8)(C_ENV5SDF_AGC_IF_LVL_MIN *256)

#define C_U8_ENV5SDF_MAX_IF_GAIN_SET	(S8)(C_ENV5SDF_MAX_IF_GAIN_SET *256)
#define C_U8_ENV5SDF_MIN_IF_GAIN_SET	(S8)(C_ENV5SDF_MIN_IF_GAIN_SET *256)
#define C_U8_ENV5SDF_MAX_RF_GAIN_SET	(S8)((C_ENV5SDF_MAX_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_ENV5SDF_MIN_RF_GAIN_SET	(S8)((C_ENV5SDF_MIN_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_ENV5SDF_IF_GAIN_RANGE      (S8)(C_ENV5SDF_MIN_IF_GAIN_POW - C_ENV5SDF_MAX_IF_GAIN_POW)
#define C_U8_ENV5SDF_RF_GAIN_RANGE      (S8)(C_ENV5SDF_MIN_RF_GAIN_POW - C_ENV5SDF_MAX_RF_GAIN_POW)

STATIC UINT8* pEnv5sdfEqNormal[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pEnv5sdfEqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};


///////////////////////////////////
//  ENV5SDF driver
///////////////////////////////////

CHAR* ENV5SDF_GetSwVer(void)
{
	return ((CHAR*)C_ENV5SDF_VERSION);
}

VOID ENV5SDF_TunerInit(ITUNER_CTX_T *pTCtx)
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);
    
    psTunerCtx->I2cAddress = C_ENV5SDF_LO_ADDRESS;
    psTunerCtx->u2IF_Freq = C_ENV5SDF_IF_FREQUENCY;
    psTunerCtx->u4RF_Freq = 0;
    //Justin TODO
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_DRO|MASK_PATCH_SENS);
    psTunerCtx->fgRFAGC = TRUE;
    // AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x2f;  
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0xFA;
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B;

    pEUCtx->m_SAW_BW  = C_ENV5SDF_SAW_BW;
    pEUCtx->m_Ana_Top = C_ENV5SDF_TOP_SET_ANA;

    pEUCtx->m_s4FreqBoundUpper = C_ENV5SDF_FREQ_DBOUND_UPPER;
    pEUCtx->m_s4FreqBoundLower = C_ENV5SDF_FREQ_DBOUND_LOWER;
    pEUCtx->m_s4AnaFreqBoundUpper = C_ENV5SDF_FREQ_DBOUND_UPPER_Ana;
    pEUCtx->m_s4AnaFreqBoundLower = C_ENV5SDF_FREQ_DBOUND_LOWER_Ana;
    
    pEUCtx->m_Small_Step = C_ENV5SDF_CHANNEL_SCAN_JUMP_SMALL_STEP;
    pEUCtx->m_Middle_Step_Up = C_ENV5SDF_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP;
    pEUCtx->m_Middle_Step_Down = C_ENV5SDF_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN;
    
    // add by JC, 081215, for Signal Level display
    pEUCtx->m_ifAgc_lvl_max = (U8)C_U8_ENV5SDF_AGC_IF_LVL_MAX;
    pEUCtx->m_ifAgc_lvl_min = (U8)C_U8_ENV5SDF_AGC_IF_LVL_MIN;
    
    pEUCtx->m_SigLvTh = 0;
    pEUCtx->m_SigLvScan = 0;
	
    pEUCtx->m_aucPara[0] = C_ENV5SDF_TOP_SET;
    pEUCtx->m_aucPara[1] = 0;
    
    pEUCtx->m_aucPara[2] = (U8)C_U8_ENV5SDF_AGC_IF_SLP;
    pEUCtx->m_aucPara[3] = (U8)C_U8_ENV5SDF_AGC_IF_INT;
    pEUCtx->m_aucPara[4] = (U8)C_U8_ENV5SDF_AGC_IF_MAX;
    pEUCtx->m_aucPara[5] = (U8)C_U8_ENV5SDF_AGC_IF_MIN;
    
    pEUCtx->m_aucPara[6] = C_ENV5SDF_AGC_COEF;
    pEUCtx->m_aucPara[7] = C_ENV5SDF_DEMOD_INPUT_POWER;
    
    pEUCtx->m_aucPara[ 8] = (U8)C_U8_ENV5SDF_MAX_IF_GAIN_SET;
    pEUCtx->m_aucPara[ 9] = (U8)C_U8_ENV5SDF_MIN_IF_GAIN_SET;
    pEUCtx->m_aucPara[10] = (U8)C_U8_ENV5SDF_MAX_RF_GAIN_SET;
    pEUCtx->m_aucPara[11] = (U8)C_U8_ENV5SDF_MIN_RF_GAIN_SET;
    pEUCtx->m_aucPara[12] = (U8)C_U8_ENV5SDF_IF_GAIN_RANGE;
    pEUCtx->m_aucPara[13] = (U8)C_U8_ENV5SDF_RF_GAIN_RANGE;
    pEUCtx->m_aucPara[14] = C_ENV5SDF_POWER_CALIBRATE;

    

     pEnv5sdfEqNormal[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQEnv5sdfMN;
     pEnv5sdfEqWeak[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQEnv5sdfMN;

    mcSHOW_USER_MSG(("Tuner ENV5SDF init done!\n"));
}


 STATIC void  ENV5SDF_SetSawBw(ITUNER_CTX_T * pTunerCtx, UINT8 sawbw)
 {
   pTunerCtx->specific_member.eu_ctx.m_SAW_BW = sawbw;

   mcSHOW_DRVAPI_MSG(("ENV5SDF_SetSawBw = %d\n", sawbw));
   
 }
S16 ENV5SDF_TunerSetFreq(ITUNER_CTX_T *pTCtx, PARAM_SETFREQ_T* param)
{
    U32 Freq = param->Freq; // transfer to video freq
    U8  AutoSearch = param->fgAutoSearch;
    U8  Mode = param->Modulation;

    UCHAR ucPllValue;
    UCHAR TunerS[5];
	UINT8 CP;//Charge pump  
    U32 Lo;
    U16 Ndivider, CB1, CB2, BB;
    U16 AGC;
    S16 ii, jj;
    #ifdef CC_MT5363
     UCHAR u1IfAgc[2];
    #endif
 
    SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);

    pTCtx->u4RF_Freq = Freq;

    if( Mode == MOD_DVBT ) //digital reception
    {

        Lo = Freq + C_ENV5SDF_IF_FREQUENCY;
        Ndivider = (U16)( (Lo*1000+C_ENV5SDF_LO_DIVIDER_STEP/2) / C_ENV5SDF_LO_DIVIDER_STEP  );
        
        mcSHOW_DRVAPI_MSG(("DVBT DigFreq = %d, Divider ratio = %04X \n",Freq, Ndivider));
        
        // Conver Freq to unit of MHz
        Freq = Freq/1000;
	//	Lo   = Lo/1000;
        
        if (Freq < (C_ENV5SDF_FREQ_DBOUND_LOWER/1000/1000) ||
            Freq > (C_ENV5SDF_FREQ_DBOUND_UPPER/1000/1000)  )
        {
            mcSHOW_DRVERR_MSG(("Out of range for LO!\n"));
          //return (+1);
        }
        CB1=0X90;   // TOP=108dBuV, step=166.67KHz
        CB2=0XD0;   // ATC=0
        
        if (Lo < 96250)
            CP = 0x00;
        else if(Lo < 162250)
            CP = 0x01;
        else if(Lo < 186250)
            CP = 0x02;
        else if(Lo < 234250)
            CP = 0x00;
        else if(Lo < 402250)      
            CP = 0x01;
        else if(Lo < 432250)
            CP = 0x02;
        else if(Lo < 456250)
            CP = 0x03;
        else if(Lo < 468250)
            CP = 0x04;		
        else if(Lo < 698250)
            CP = 0x01;
        else if(Lo < 836250)
            CP = 0x02;
        else if(Lo < 884250)
            CP = 0x03;
        else 
            CP = 0x04;      

       /*  BB =   00******         */
	    if (Lo < 186250)
			BB = 0X01;//p5=0,INT2,LIUQU,20100121
		else if (Lo < 468250)    
			BB = 0X02;  //p5=0,INT2,LIUQU,20100121
		else 
			BB = 0X04;  //p5=0,INT2,LIUQU,20100121
	   	
       /*  BB =   ********         */
		BB = BB | ((CP << 6)&0XC0);  //CP1CP0******
	#if 0
        if (pTunerCtx->m_SAW_BW == SAW_BW_8M)
            BB = (BB | 0x08);      //8Mhz
   #endif         
       /*  CB2 =   ********         */
		CB2 = CB2 | ((CP >> 1)&0X02);  //******CP2*
		
        AGC = pTunerCtx->m_aucPara[0];
	    mcSHOW_DRVAPI_MSG(("ENV5SDF Set Tuner PLL AGC = %x\n",AGC));
        CB1 = (((AGC<<3) & 0X38)|(CB1 & 0XC7)) ; //SET TOP LEVEL
    }
    else  //NTSC 
    {    
        Lo = Freq + C_ENV5SDF_IF_FREQUENCY_ANA; // kHz	
        Ndivider = (U16) ( (Lo*1000+C_ENV5SDF_LO_DIVIDER_STEP_ANA/2) / C_ENV5SDF_LO_DIVIDER_STEP_ANA);
        mcSHOW_DBG_MSG(("NTSC Freq = %d, Divider ratio = %04X PLL set freq=%d kHz\n",Freq, Ndivider,(Ndivider*C_ENV5SDF_LO_DIVIDER_STEP_ANA/1000-C_ENV5SDF_IF_FREQUENCY_ANA)));

        if (Freq < C_ENV5SDF_FREQ_DBOUND_LOWER_Ana/1000 || Freq > C_ENV5SDF_FREQ_DBOUND_UPPER_Ana/1000)
        {
            mcSHOW_DRVERR_MSG(("Out of range for LO!\n"));
          //return (+1);
        }

        Freq = Freq/1000;
	//	Lo   = Lo/1000;

        CB1=0XBB;  //RF external control, step=62.5KHz
             //   CB2=0XD4;    //ATC=0,IFDA=1,DISCA=0,CP2=0
             CB2=0XD0;   //liuqu modified,20091229,ATC=0,IFDA=0,DISCA=0,CP2=0

	    //CP = 0X000;
        /*  BB =   ********         */
        if (Lo < 186250)   // low band, VHF low
        {
            BB = 0x11;
        }
        else if (Lo < 468250)  // mid band, VHF high
        {
            BB = 0x12;    
        }
        else   // high band, UHF
        {
            BB = 0x14;
        }

    }

    if (AutoSearch)
       CB2 = (UCHAR) (CB2 | 0x20);  //ATC=1
    for (jj=0; jj<2; jj++)  //retry 2 times while PLL unlock, LC 070702
    {
        // LO settings
        TunerS[0] = (UCHAR) 0x7F & (Ndivider >> 8);
        TunerS[1] = (UCHAR) 0xFF & Ndivider;
        TunerS[2] = (UCHAR) CB1;
        TunerS[3] = (UCHAR) BB;
		TunerS[4] = (UCHAR) CB2; 

      mcSHOW_DRVAPI_MSG(("0x%02X-0x%02X-0x%02X-0x%02X-0x%02X\n", TunerS[0], TunerS[1], TunerS[2], TunerS[3], TunerS[4]));
      
        if (ICtrlBus_I2cTunerWrite(C_ENV5SDF_I2C_CLK_DIV, pTCtx->I2cAddress, TunerS, 5))
        {
            mcSHOW_HW_MSG(("TunerWrite PLL failed!\n"));
            return (+1);
        }

        // wait PLL lock

        for (ii=0; ii<C_ENV5SDF_PLL_TIMEOUT_CNT; ii++)
        {
            if (ICtrlBus_I2cTunerRead(C_ENV5SDF_I2C_CLK_DIV, pTCtx->I2cAddress, &ucPllValue, 1))
            {
                mcSHOW_HW_MSG(("Tuner read failed!\n"));
                return (+1);
            }
            // check if PLL lock
            if (mcCHK_BIT1(ucPllValue, C_ENV5SDF_PLL_LOCK_BIT)) {
                break;
            }
            mcDELAY(C_ENV5SDF_PLL_POLL_INTERVAL);
        }

        if(ii < C_ENV5SDF_PLL_TIMEOUT_CNT) {
             mcSHOW_DBG_MSG(("Freq = %d MHz, the %d-th time setting PLL success\n" , Freq, jj+1));
            break;
          }
        else {
        	    if (Freq < 48)
        	   {	
              mcSHOW_DBG_MSG(("Freq = %d MHz,Frequency not support\n" , Freq));
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
        pTunerCtx->m_SigLvScan = -(((S8)u1IfAgc[1])*C_ENV5SDF_AGC_IF_SLP_SGN);
        mcSHOW_DRVAPI_MSG(("IfAgc = %d\n", pTunerCtx->m_SigLvScan));
        /*** Record IfAgc value before LNA on for BestMux selection ***/
    }  
    #endif 

    pTunerCtx->m_aucPara[1]  = TunerS[0]; //send register value to UI, LC 070102
    pTunerCtx->m_aucPara[15] = TunerS[1];
    pTunerCtx->m_aucPara[16] = TunerS[2];
    pTunerCtx->m_aucPara[17] = TunerS[3];
    pTunerCtx->m_aucPara[18] = TunerS[4];
    pTunerCtx->m_aucPara[19] = ucPllValue;
    
    return 0;
}


INT16 ENV5SDF_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){

    switch(eOperation){
    case itGetVer:
        {
        CHAR ** RetStr = (CHAR **)pInOutVal;
        *RetStr = ENV5SDF_GetSwVer();
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
					 // mcDELAY(C_ENV5SDF_PLL_POLL_INTERVAL);
			}
           mcSHOW_DRVERR_MSG(("The register words are: 0x%x-0x%x-0x%x-0x%x-0x%x\n",TunerS[0],TunerS[1],TunerS[2],TunerS[3],TunerS[4]));
	 for (j=0; j<2; j++)  //retry 2 times while PLL unlock, LC 070702
       {
   
           if (ICtrlBus_I2cTunerWrite(C_ENV5SDF_I2C_CLK_DIV, pTCtx->I2cAddress, TunerS, 5))    //  send this data
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
   
          for (i=0; i<C_ENV5SDF_PLL_TIMEOUT_CNT; i++)
          {
            if (ICtrlBus_I2cTunerRead(C_ENV5SDF_I2C_CLK_DIV, pTCtx->I2cAddress, &ucPllValue, 1))
            {
              
				mcSHOW_HW_MSG(("Tuner read failed!\n"));
                return (+1);
            }
           
            if (mcCHK_BIT1(ucPllValue, C_ENV5SDF_PLL_LOCK_BIT))   // check if PLL lock
				{
                   pTunerCtx->m_aucPara[19] = ucPllValue;
					break;
                }
            mcDELAY(C_ENV5SDF_PLL_POLL_INTERVAL);
          }

          if(i < C_ENV5SDF_PLL_TIMEOUT_CNT) 
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
		ENV5SDF_SetSawBw(pTCtx,SetVal);
        break;
    case itSetLNA:
    case itGetLNA:
        break;

    case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pEnv5sdfEqNormal[SetVal];
        break;
    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pEnv5sdfEqWeak[SetVal];
        break;
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = NULL;
        break;
    default:
        return ITUNER_NOT_SUPPORTED;
    }
    return ITUNER_OK;
}
