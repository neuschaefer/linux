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
 * $RCSfile: pi_tuner_tdtws710d.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_tdtws710d.c
 *  Tuner control for SS tdtws710d
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "demod_tuner_interface.h"
//#include "pd_i2capi.h"
#include "tuner_interface_if.h"
#include "ctrl_bus.h"
#include "PI_Def.h"
#include "fe_tuner_common_if.h"
#include "pi_def_dvbt.h" //add by liuqu,20090424
#include "pi_dvbt_if.h"  //add by liuqu,20090424
//#include "pd_tuner_type.h"

#define C_TDTWS710D_VERSION				"TDTWS710D v1.00(mt5363)"
// *** Release Note ***
// Ver     Date         Description
// 1.00    2006.12.12   file established
// 1.01	   2007.02.06	PLL IC change

// *** Memo ***
// 24cc Combo Tuner

#define C_TDTWS710D_I2C_DIVIDER       ((U16) 270)

#define C_TDTWS710D_TOP_SET               ((U8)   0x0a)   //changed by liuqu,20080826 
#define C_TDTWS710D_SAW_BW                SAW_BW_8M 

#define C_TDTWS710D_FREQ_DBOUND_UPPER   858*1000*1000
#define C_TDTWS710D_FREQ_DBOUND_LOWER    48*1000*1000
#define C_TDTWS710D_FREQ_DBOUND_UPPER_Ana   867*1000*1000
#define C_TDTWS710D_FREQ_DBOUND_LOWER_Ana    45*1000*1000

#define C_TDTWS710D_LO_ADDRRESS           ((U16)  0xc2)
#define C_TDTWS710D_IF_ADDRRESS            ((U16)  0x86)
#define C_TDTWS710D_IF_FREQUENCY          ((U16)  36167)  /* kHz */
#define C_TDTWS710D_IF_FREQUENCY_ANA      ((U16)  38900)  /* kHz */
#define C_TDTWS710D_IF_FREQUENCY_L1       ((U16)  33900)  /* kHz */
#define C_TDTWS710D_LO_DIVIDER_STEP       ((U32)  166670) /* Hz */
#define C_TDTWS710D_LO_DIVIDER_STEP_ANA   ((U16)  50000) /* Hz */

#define C_TDTWS710D_CHANNEL_SCAN_JUMP_SMALL_STEP ((U16) 250)
#define C_TDTWS710D_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP ((U16) 800)
#define C_TDTWS710D_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN ((U16) 250)

#define C_TDTWS710D_LNA_SWITCH_THRESHOLD 10
#define C_TDTWS710D_AGC_IF_SLP             1
#define C_TDTWS710D_AGC_IF_INT             0
#define C_TDTWS710D_AGC_IF_MAX             0.499
#define C_TDTWS710D_AGC_IF_MIN            -0.5
#define C_TDTWS710D_AGC_IF_SLP_SGN  (S8)((C_TDTWS710D_AGC_IF_SLP>0)?1:-1) 
#define C_TDTWS710D_AGC_IF_LVL_MAX      0.499
#define C_TDTWS710D_AGC_IF_LVL_MIN     -0.5


#define C_TDTWS710D_MAX_IF_GAIN_SET       -0.12   
#define C_TDTWS710D_MIN_IF_GAIN_SET       -0.38   
#define C_TDTWS710D_MAX_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_TDTWS710D_MIN_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_TDTWS710D_MAX_IF_GAIN_POW       -100    // dBm
#define C_TDTWS710D_MIN_IF_GAIN_POW		-50     // dBm
#define C_TDTWS710D_MAX_RF_GAIN_POW		 C_TDTWS710D_MIN_IF_GAIN_POW
#define C_TDTWS710D_MIN_RF_GAIN_POW		-20     // dBm
#define C_TDTWS710D_POWER_CALIBRATE        26     // dBm
#define C_TDTWS710D_SIGNAL_LVL_TH             4

#define C_TDTWS710D_AGC_COEF				((U8)   0xBD)

#define AGC_IF_POWER_LEVEL__6dBm  0  //add by hichi ??

#define C_TDTWS710D_DEMOD_INPUT_POWER		AGC_IF_POWER_LEVEL__6dBm

// ********************************************* //

#define C_U8_TDTWS710D_AGC_IF_SLP         (S8)(C_TDTWS710D_AGC_IF_SLP *32 )
#define C_U8_TDTWS710D_AGC_IF_INT         (S8)(C_TDTWS710D_AGC_IF_INT *64 )
#define C_U8_TDTWS710D_AGC_IF_MAX         (S8)(C_TDTWS710D_AGC_IF_MAX *256)
#define C_U8_TDTWS710D_AGC_IF_MIN         (S8)(C_TDTWS710D_AGC_IF_MIN *256)
#define C_U8_TDTWS710D_AGC_IF_LVL_MAX (S8)(C_TDTWS710D_AGC_IF_LVL_MAX *256)
#define C_U8_TDTWS710D_AGC_IF_LVL_MIN  (S8)(C_TDTWS710D_AGC_IF_LVL_MIN *256)

#define C_U8_TDTWS710D_MAX_IF_GAIN_SET	(S8)(C_TDTWS710D_MAX_IF_GAIN_SET *256)
#define C_U8_TDTWS710D_MIN_IF_GAIN_SET	(S8)(C_TDTWS710D_MIN_IF_GAIN_SET *256)
#define C_U8_TDTWS710D_MAX_RF_GAIN_SET	(S8)((C_TDTWS710D_MAX_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_TDTWS710D_MIN_RF_GAIN_SET	(S8)((C_TDTWS710D_MIN_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_TDTWS710D_IF_GAIN_RANGE      (S8)(C_TDTWS710D_MIN_IF_GAIN_POW - C_TDTWS710D_MAX_IF_GAIN_POW)
#define C_U8_TDTWS710D_RF_GAIN_RANGE      (S8)(C_TDTWS710D_MIN_RF_GAIN_POW - C_TDTWS710D_MAX_RF_GAIN_POW)

/*************************************
variable
*************************************/

STATIC BOOL fgTDTWS710DLNA;


/*************************************
functions
*************************************/

VOID TDTWS710D_TunerInit( ITUNER_CTX_T *pTunerCtx );

INT16 TDTWS710D_TunerSetFreq(    ITUNER_CTX_T *pTunerCtx,  PARAM_SETFREQ_T* param );

STATIC CHAR *TDTWS710D_TunerGetVer(VOID);

STATIC INT8  TDTWS710D_SetIFDemod( ITUNER_CTX_T * pTunerCtx,U8 SubSysIdx);

//----------------------------------------------------------------------------- 
/** TDTWS710D_TunerSetFreq
 *  Set Tuner PLL forTDTWS710D/TDTWS710D to tune RF frequency.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param 	param    Pointer of structure of parameters
 *  @retval  0 success
                    1  fail   out of range 
                   -1 fail  I2C error 
 */
//-----------------------------------------------------------------------------

VOID TDTWS710D_TunerInit(
    ITUNER_CTX_T *pTunerCtx
    )
{
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTunerCtx->specific_member.eu_ctx);
	   
	pTunerCtx->I2cAddress = C_TDTWS710D_LO_ADDRRESS;
	pTunerCtx->u2IF_Freq= C_TDTWS710D_IF_FREQUENCY;
	pEUCtx->m_SAW_BW  = C_TDTWS710D_SAW_BW;

	pEUCtx->m_s4FreqBoundUpper = C_TDTWS710D_FREQ_DBOUND_UPPER;
	pEUCtx->m_s4FreqBoundLower = C_TDTWS710D_FREQ_DBOUND_LOWER;
  pEUCtx->m_s4AnaFreqBoundUpper = C_TDTWS710D_FREQ_DBOUND_UPPER_Ana;
  pEUCtx->m_s4AnaFreqBoundLower = C_TDTWS710D_FREQ_DBOUND_LOWER_Ana;

	//add by liuqu,20081111
	pEUCtx->m_Small_Step = C_TDTWS710D_CHANNEL_SCAN_JUMP_SMALL_STEP;//set small step as 250k
	pEUCtx->m_Middle_Step_Up = C_TDTWS710D_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP;//set middle step as 1500k,tda9886
    pEUCtx->m_Middle_Step_Down = C_TDTWS710D_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN;
    
    // add by JC, 081215, for Signal Level display
    pEUCtx->m_ifAgc_lvl_max = (U8)C_U8_TDTWS710D_AGC_IF_LVL_MAX;
    pEUCtx->m_ifAgc_lvl_min = (U8)C_U8_TDTWS710D_AGC_IF_LVL_MIN;
	
	pEUCtx->m_aucPara[0] = C_TDTWS710D_TOP_SET;
	pEUCtx->m_aucPara[1] = 0;

	pEUCtx->m_aucPara[2] = (U8)C_U8_TDTWS710D_AGC_IF_SLP;
	pEUCtx->m_aucPara[3] = (U8)C_U8_TDTWS710D_AGC_IF_INT;
	pEUCtx->m_aucPara[4] = (U8)C_U8_TDTWS710D_AGC_IF_MAX;
	pEUCtx->m_aucPara[5] = (U8)C_U8_TDTWS710D_AGC_IF_MIN;

	pEUCtx->m_aucPara[6] = C_TDTWS710D_AGC_COEF;
	pEUCtx->m_aucPara[7] = C_TDTWS710D_DEMOD_INPUT_POWER;

	pEUCtx->m_aucPara[ 8] = (U8)C_U8_TDTWS710D_MAX_IF_GAIN_SET;
	pEUCtx->m_aucPara[ 9] = (U8)C_U8_TDTWS710D_MIN_IF_GAIN_SET;
	pEUCtx->m_aucPara[10] = (U8)C_U8_TDTWS710D_MAX_RF_GAIN_SET;
	pEUCtx->m_aucPara[11] = (U8)C_U8_TDTWS710D_MIN_RF_GAIN_SET;
	pEUCtx->m_aucPara[12] = (U8)C_U8_TDTWS710D_IF_GAIN_RANGE;
	pEUCtx->m_aucPara[13] = (U8)C_U8_TDTWS710D_RF_GAIN_RANGE;
	pEUCtx->m_aucPara[14] = C_TDTWS710D_POWER_CALIBRATE;
	pEUCtx->m_aucPara[21] = 0x07;

	pEUCtx->m_SigLvTh = C_TDTWS710D_SIGNAL_LVL_TH;
    pEUCtx->m_SigLvScan = 0;
    //IFAGC sample array
    //psTunerSpecificCtx->pSignalLvlArray[TUNER_SIGNAL_LVL_ARRAY_VHF][TUNER_SIGNAL_LVL_ARRAY_8VSB]= ;//8VSB VHF band
    
    mcSHOW_USER_MSG(("Init " C_TDTWS710D_VERSION "\n"));
}


//----------------------------------------------------------------------------- 
/** TDTWS710D_TunerSetFreq
 *  Set Tuner PLL forTDTWS710D/TDTWS710D to tune RF frequency.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param 	param    Pointer of structure of parameters
 *  @retval  0 success
                    1  fail   out of range 
                   -1 fail  I2C error 
 */
//-----------------------------------------------------------------------------
INT16 TDTWS710D_TunerSetFreq(
    ITUNER_CTX_T *pTunerCtx,
    PARAM_SETFREQ_T* param
    )
{
	UCHAR uc_data;
	S16 ucTunerLockConfCnt;
	UCHAR TunerS[5];
	U32 Lo,Freq;
	U16 Ndivider, CB1, CB2, BB; 
	S16 ii, jj;
	UCHAR u1IfAgc[2];
	static U8 lastMode = MOD_DVBT;
	static U16 lastAutoSearch = 1;
	UINT8 AutoSearch = param->fgAutoSearch;
	UINT8 Mode = param->Modulation;

	SPECIFIC_MEMBER_EU_CTX* pEUCtx= &( pTunerCtx->specific_member.eu_ctx);
       

    pTunerCtx->u4RF_Freq = param->Freq;
    Freq = param->Freq;  // for coding convience,liuqu,20090424
    pTunerCtx->I2cAddress = C_TDTWS710D_LO_ADDRRESS;

    if(Mode== MOD_DVBT) //digital reception
    {

	    Lo = Freq + C_TDTWS710D_IF_FREQUENCY; // kHz
	    //Ndivider = (U16) ( ((Lo * 1000) / DTOS403_LO_DIVIDER_STEP) + 0.5 );
	  
	    Ndivider = (U16) ( (Lo*1000+C_TDTWS710D_LO_DIVIDER_STEP/2) / C_TDTWS710D_LO_DIVIDER_STEP);
	    // Conver Freq to unit of MHz
	    Freq = Freq/1000;
	
	    if (Freq < (C_TDTWS710D_FREQ_DBOUND_LOWER/1000/1000) || 
	        Freq > (C_TDTWS710D_FREQ_DBOUND_UPPER/1000/1000)  )
	    {
	        mcSHOW_DRVAPI_MSG(("Out of range for LO!\n"));
	        //return (+1);
	    }
	    
	    if (Freq <= 165)       //liuqu,20090317 change!
		{
			BB = 0x81;
		}
		else if (Freq <= 449)  
		{        
			BB = 0x82;
		}
		else if (Freq <= 670)  
		{        
			BB = 0x88;
		}
		else                 
	    {
			BB = 0xC8;
		}

	    CB1 = 0x90;
           CB2 = 0xe2;   // ATC = 1 for digital

	 //merge TOP setting
	    CB1 = (CB1&0XC7)|((pEUCtx->m_aucPara[0]&0x07)<<3);  // 0x **xx x***
	    CB2 = (CB2&0XFD)|((pEUCtx->m_aucPara[0]&0X08)>>2); // 0X **** **x*
	     
    }
    else //analog reception
    {

     if( Mode == MOD_SECAM_L1 )
	    {
	    	Lo = Freq + C_TDTWS710D_IF_FREQUENCY_L1; // kHz
	    	Ndivider = (U16) ( (Lo*1000+C_TDTWS710D_LO_DIVIDER_STEP_ANA/2) / C_TDTWS710D_LO_DIVIDER_STEP_ANA);
            mcSHOW_DBG_MSG(("Freq = %d, Divider ratio = %04X PLL set freq=%d kHz\n", pTunerCtx->u4RF_Freq, Ndivider,(Ndivider*C_TDTWS710D_LO_DIVIDER_STEP_ANA/1000-C_TDTWS710D_IF_FREQUENCY_L1)));
	    }
	    else
	    {
	    	Lo = Freq + C_TDTWS710D_IF_FREQUENCY_ANA; // kHz
	    	Ndivider = (U16) ( (Lo*1000+C_TDTWS710D_LO_DIVIDER_STEP_ANA/2) / C_TDTWS710D_LO_DIVIDER_STEP_ANA);
            mcSHOW_DBG_MSG(("Freq = %d, Divider ratio = %04X PLL set freq=%d kHz\n", pTunerCtx->u4RF_Freq, Ndivider,(Ndivider*C_TDTWS710D_LO_DIVIDER_STEP_ANA/1000-C_TDTWS710D_IF_FREQUENCY_ANA)));

	    }
	    
	    // Conver Freq to unit of MHz
	    Freq = Freq/1000;
	    
	    if (Freq < (C_TDTWS710D_FREQ_DBOUND_LOWER_Ana/1000/1000) || 
	        Freq > (C_TDTWS710D_FREQ_DBOUND_UPPER_Ana/1000/1000)  )
	    {
	        mcSHOW_DRVAPI_MSG(("Out of range for LO!\n"));
	        //return (+1);
	    }
	    
	    if (Freq <= 165)       
		{
			BB = 0x01;
		}
		else if (Freq <= 449)  
		{        
			BB = 0x02;
		}
		else                 
	    {
			BB = 0x08;
		}

	         CB1 = 0xb5;   // 20090317,LG vendor requires to change to TOP =94dBuv fisrt! TOP FROM 111->110 (BIT 5,4,3) bd ->b5 (liuqu)
                CB2 = 0xe2;	//ATC =1 ,20090317,LG vendor requires

    
    }

//20081028,LNA flow has been changed ,according to LG vendor's new spec 20081022
    if (( Mode >= MOD_PAL_BG) && (AutoSearch))
       BB |= 0x04;   //LNA should always turn on,liuqu,20080825
    	

    ////when change to dvbt mode,need to set TOP to BG first! liuqu,20081029
    //if(( Mode == MOD_DVBT) && (!AutoSearch))
    if (Mode == MOD_DVBT)
    {          	
       TunerS[0] = 0x86;	  
       TunerS[1] = (UCHAR)0x02;  //sub addr
       TunerS[2] = 0x44 ;   //follow PAL BG TOP setting
 			 
//       if (RegSetRegistersTuner(TunerS[0], 2, &TunerS[1]) < 0)
         if (ICtrlBus_I2cTunerWrite(C_TDTWS710D_I2C_DIVIDER, C_TDTWS710D_IF_ADDRRESS, &TunerS[1] , 2)>0)
       {
             mcSHOW_USER_MSG(("\nTunerSet() failed!\n"));
             return (-1);
       }	                
    }
    // for D->A, AGC is disabled @ Digital mode, need to re-set again in analog mode
    else if (( Mode >= MOD_PAL_BG) && (!(AutoSearch)))  // analog mode
    {
       if ((lastMode == MOD_DVBT) && (!lastAutoSearch))   // D->A channel change, 
       {
           TunerS[0] = 0x86;	  
           TunerS[1] = (UCHAR)0x02;  //sub addr
           TunerS[2] = 0x44;   //follow PAL BG TOP setting
 			 
//           if (RegSetRegistersTuner(TunerS[0], 2, &TunerS[1]) < 0)
	    if(ICtrlBus_I2cTunerWrite(C_TDTWS710D_I2C_DIVIDER, C_TDTWS710D_IF_ADDRRESS, &TunerS[1] , 2)>0) 	
           {
               mcSHOW_USER_MSG(("\nTunerSet() failed!\n"));
               return (-1);
           }	
       }
    }
              
    // LO settings
    TunerS[0] = (UCHAR) 0x7F & (Ndivider >> 8);
    TunerS[1] = (UCHAR) 0xFF & Ndivider;
    TunerS[2] = (UCHAR) CB1;  
    TunerS[3] = (UCHAR) BB;
	TunerS[4] = (UCHAR) CB2; 

    for (jj=0; jj<2; jj++)
    {

//  send this data 
	 //   if (RegSetRegistersTuner( pTunerCtx->I2cAddress, 5, &TunerS[0]) < 0)
	    if(ICtrlBus_I2cTunerWrite(C_TDTWS710D_I2C_DIVIDER, pTunerCtx->I2cAddress, TunerS , 5)>0)  
	    {
	        //TRACE(("\nTunerSet() failed!\n"));
	        return (-1);
	    }
	
	//	mcDELAY(10);   //marked by liuqu,for scan time improvement,20080918
		// wait PLL lock
	//#ifndef C_DEMO_PROGRAM
		ucTunerLockConfCnt = 0;
		for (ii=0; ii<20; ii++)
		//for (ii=0; ii<ccMAX_WAIT_TUNER_TIME; ii++)
		{
			//RegGetRegisters( pTunerCtx->I2cAddress, ( pTunerCtx->I2cAddress) | (0x01), 1, &uc_data);
			// Charles_060425, test I2C
			//I2CRead( pTunerCtx->I2cAddress, 1, &uc_data);
			ICtrlBus_I2cTunerRead(C_TDTWS710D_I2C_DIVIDER, pTunerCtx->I2cAddress, &uc_data, 1);
			if (mcGET_FIELD(uc_data, 0x40, 6))  // if PLL lock
				ucTunerLockConfCnt++;
			else
				ucTunerLockConfCnt = 0;
	                
	         //       mcDELAY(10);  //marked by liuqu,for scan time improvement,20080918
	                
			if (ucTunerLockConfCnt>ccTUNER_LOCK_CONF_THRESHOLD)
				break;
			mcDELAY(5);
								
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
#ifdef EU_AUTO_ATP			
			gATPFailBitMsk|= 0X01;  //PLL set fail!
#endif			
		}

	}

      if(( Mode == MOD_DVBT) && (!AutoSearch))
      	   mcDELAY(100);     //delay 100ms when channel change 
      if( Mode == MOD_DVBT) //digital reception
         TDTWS710D_SetIFDemod(pTunerCtx, Mode);
      
//#else
//	mcDELAY(200);
//#endif
	
    if( (Mode == MOD_DVBT) )//|| (Mode >= MOD_PAL_BG && !AutoSearch) )
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
		  if ((((S8)u1IfAgc[1]) == 0xd0) &&(C_TDTWS710D_AGC_IF_SLP))
		  {
                       pEUCtx->m_SigLvScan = 0x7f;  //make (-(-128)=127)
		  }
		  else
		  {
                 pEUCtx->m_SigLvScan = -(((S8)u1IfAgc[1])*C_TDTWS710D_AGC_IF_SLP_SGN);
		  }	 
                 mcSHOW_DRVAPI_MSG(("IfAgc = %d\n", pEUCtx->m_SigLvScan));
#ifdef EU_AUTO_ATP
               if (gATP_TEST_ITEM_BIT & 0x20)
               {
                 mcSHOW_DRVAPI_MSG(("##Start\n##IFAGC = %d\n##End\n",pEUCtx->m_SigLvScan));
               }
#endif
        }
		
        if(!AutoSearch)
        	mcDELAY(200);	// Ken, 081103, for ADC value stable time
      //  I2CRead( pTunerCtx->I2cAddress, 1, &uc_data);
           ICtrlBus_I2cTunerRead(C_TDTWS710D_I2C_DIVIDER, pTunerCtx->I2cAddress, &uc_data, 1);
		if( (uc_data&0x07) >0x01)	//ADC value is 100,011,010, LNA should turn ON
		{
		    TunerS[3] = TunerS[3] | 0x04;	//LNA ON, BS3=1
		    fgTDTWS710DLNA = TRUE ;
		    mcSHOW_DRVAPI_MSG(("LNA ON!!\n"));
#ifdef EU_AUTO_ATP
                if (gATP_TEST_ITEM_BIT & 0x40)
               {
                 mcSHOW_DRVAPI_MSG(("##Start\n##DLNA = 1\n##End\n"));
               }
#endif
		}
		else
		{
            TunerS[3] = TunerS[3] & 0xFB;	//LNA OFF, BS3=0
              fgTDTWS710DLNA = FALSE ;
		    mcSHOW_DRVAPI_MSG(("LNA OFF!!\n"));
#ifdef EU_AUTO_ATP
                if (gATP_TEST_ITEM_BIT & 0x40)
               {
                 mcSHOW_DRVAPI_MSG(("##Start\n##DLNA = 0\n##End\n"));
               }
#endif			
		}
    }
    //else if((Mode >= MOD_PAL_BG) && (!AutoSearch))
    else if ( Mode >= MOD_PAL_BG)
    {
        
        if (AutoSearch)
    	 {
             TunerS[3] = TunerS[3] | 0x04;	//LNA ON, BS3=1
             fgTDTWS710DLNA = TRUE ;
             mcSHOW_DRVAPI_MSG(("ATV scan, LNA ON!!\n" ));
    	 }
        else
        {
           //if ((lastMode == MOD_DVBT) && (!lastAutoSearch))
           //   mcDELAY(400);
           //else
              mcDELAY(200);
           //I2CRead( pTunerCtx->I2cAddress, 1, &uc_data);
           ICtrlBus_I2cTunerRead(C_TDTWS710D_I2C_DIVIDER, pTunerCtx->I2cAddress, &uc_data, 1);
           if( (uc_data&0x07) >0x01)	//ADC value is 100.011,010, LNA should turn ON
           {
              TunerS[3] = TunerS[3] | 0x04;	//LNA ON, BS3=1
              fgTDTWS710DLNA = TRUE ;
              mcSHOW_DRVAPI_MSG(("LNA ON!!\n" ));
#ifdef EU_AUTO_ATP
                if (gATP_TEST_ITEM_BIT & 0x80)
               {
                 mcSHOW_DRVAPI_MSG(("##Start\n##ALNA = 1\n##End\n"));
               }
#endif			  
           }
           else
           {
              TunerS[3] = TunerS[3] & 0xFB;	//LNA OFF, BS3=0
              fgTDTWS710DLNA = FALSE ;
              mcSHOW_DRVAPI_MSG(("LNA OFF!!\n"));
#ifdef EU_AUTO_ATP
                  if (gATP_TEST_ITEM_BIT & 0x80)
               {
                 mcSHOW_DRVAPI_MSG(("##Start\n##ALNA = 0\n##End\n"));
               }
#endif			  
           }	
        }
    }
    
    if ((Mode == MOD_DVBT) && (!AutoSearch))
    	TunerS[4] = TunerS[4] & 0xDF;   // ATC = 0

    if( Mode >= MOD_PAL_BG)
    	{
       TunerS[2] = 0xbd ; //liuqu,20090317,from LG vendor. disable MOPLL AGC 
       TunerS[4] = TunerS[4] & 0xDF;   // ATC = 0
       mcSHOW_DRVAPI_MSG(("20090317 new flow from LG vendor\n"));
	    mcDELAY(50);
    	}	
       
    //if (RegSetRegisters( pTunerCtx->I2cAddress, TunerS[0], 4, &TunerS[1]) < 0)
    if(ICtrlBus_I2cTunerWrite(C_TDTWS710D_I2C_DIVIDER, pTunerCtx->I2cAddress, TunerS , 5)>0) 
    {
        //TRACE(("\nTunerSet() failed!\n"));
        return (-1);
    }
    

	mcDELAY_MS(10);   // LG recommend, Ken, 081112
	
 //marked by liuqu,for scan time improvement,20080918  
 //   mcDELAY(10);	// add 10ms delay before bypass is turn off, LC 070613

/*******************************************************
    Record the latest TunerS code.
*******************************************************/
 //    x_memcpy(TunerPll,TunerS,sizeof(TunerS));

	pEUCtx->m_aucPara[15] = TunerS[0];
	pEUCtx->m_aucPara[16] = TunerS[1];
	pEUCtx->m_aucPara[17] = TunerS[2];
	pEUCtx->m_aucPara[18] = TunerS[3];
	pEUCtx->m_aucPara[19] = TunerS[4];
	

    lastMode = Mode;
    lastAutoSearch = AutoSearch;
    
    return (0);
}

//----------------------------------------------------------------------------- 
/** TDTWS710D_TunerGetVer
 *  Set Tuner PLL staus forTDTWS710D/TDTWS710D
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval tuner model name
 */
//-----------------------------------------------------------------------------
STATIC CHAR *TDTWS710D_TunerGetVer(VOID)
{
    return (C_TDTWS710D_VERSION);
}


//----------------------------------------------------------------------------- 
/** TDTWS710D_SetIFDemod
 *  TDTWS710D_SetIFDemod.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param 	param    Pointer of structure of parameters
 *  @retval  0 success
                    1  fail   out of range 
                   -1 fail  I2C error 
 */
//-----------------------------------------------------------------------------
STATIC INT8   TDTWS710D_SetIFDemod( ITUNER_CTX_T * pTunerCtx,U8 SubSysIdx)
{
    UCHAR TunerS[7],B,C,E,F,G,H,J,K,L,Data[9];
    SPECIFIC_MEMBER_EU_CTX* pEUCtx= &( pTunerCtx->specific_member.eu_ctx);	
	
	TunerS[0] = 0x86;	// IF IC slave address
	TunerS[1] = 0x00;	// IF IC sub address

	mcSHOW_DRVAPI_MSG(("TDTWS710D_SetIFDemod SubSysIdx =%d\n",SubSysIdx));
	
	switch (SubSysIdx)
	{
	case MOD_DVBT:
	    if (pEUCtx->m_SAW_BW != SAW_BW_8M)
		    C= 0x08; //Saw_BW = 7M
		else
		    C = 0x00; //Saw_BW =8M 
	       B = 0x21;     //liuqu,20080704 update spec
		E = 0X50;
		F = 0X21;//
		G = 0X64;
		H = 0X02;
		J = 0X20;
		K = 0x2a;  //liuqu added,20081218
		L = 0x00; //liuqu added,20081218
		break;
	case MOD_PAL_BG:
        B = 0x21; 
		C = 0X0a;
		E = 0x44; //liuqu changed,20081218 //0X43;
		F = 0X21;// liuquchanged,20081218 //0x42
		G = 0X84;
		H = 0X00;
		J = 0X20;	
		K = 0x30;  //liuqu added,20081218
		L = 0x02; //liuqu added,20081218
		break;
	case MOD_PAL_I:
		B = 0x21; 
		C = 0X0a;//
		E = 0x44; //liuqu changed,20081218 //0X43;
		F = 0X21;// liuquchanged,20081218 //0x42
		G = 0X8C;
		H = 0X00;
		J = 0X20;
		K = 0x30;  //liuqu added,20081218
		L = 0x02; //liuqu added,20081218
		break;
	case MOD_PAL_DK:
		B = 0x21; 
		C = 0X0a;
		E = 0x44; //liuqu changed,20081218 //0X43;
		F = 0X21;// liuquchanged,20081218 //0x42
		G = 0X94;
		H = 0X00;
		J = 0X20;
		K = 0x30;  //liuqu added,20081218
		L = 0x02; //liuqu added,20081218
		break;	
	case MOD_SECAM_L:
		B = 0x25; 
		C = 0X0a;
		E = 0x42; //spec 20081231 :0x43,benson ask not to change for EN55020,20090317 //0x42; //liuqu changed,20081218 //0X41;
		F = 0X21;// liuquchanged,20081218 //0x42
		G = 0X90;
		H = 0X00;
		J = 0X35;
		K = 0x30;  //liuqu added,20081218
		L = 0x02; //liuqu added,20081218
		break;
	case MOD_SECAM_L1:
		B = 0x85; 
		C = 0X03;
		E = 0x42; //spec 20081231 :0x43,benson ask not to change for EN55020,20090317 //0x42; //liuqu changed,20081218 //0X41;
		F = 0X40;//
		G = 0X90;
		H = 0X00;
		J = 0X35;
		K = 0x30;  //liuqu added,20081218
		L = 0x02; //liuqu added,20081218
		break;
	default:
		B = 0x21; 
		C = 0X0a;
		E = 0x44; //liuqu changed,20081218 //0X43;
		F = 0X21;// liuquchanged,20081218 //0x42
		G = 0X84;
		H = 0X00;
		J = 0X20;	
		K = 0x30;  //liuqu added,20081218
		L = 0x02; //liuqu added,20081218
		break;
	}

	Data[0] = B;
	Data[1] = C;
	Data[2] = E;
	Data[3] = F;
	Data[4] = G;
	Data[5] = H;
	Data[6] = J;
	Data[7] = K;
	Data[8] = L;

	
	//apply new I2C mode:increastial mode.liuqu,20090213
      TunerS[1] =0x80;
      TunerS[2] = Data[0];
      TunerS[3] = Data[1];
      TunerS[4] = Data[2];
      TunerS[5] = Data[3];
      TunerS[6] = Data[4];

       mcSHOW_DRVAPI_MSG(("the first time  IFDEMOD RegSetRegistersTuner.\n"));
	   
    //   if (RegSetRegistersTuner(TunerS[0], 6, &TunerS[1]) < 0)
         if(ICtrlBus_I2cTunerWrite(C_TDTWS710D_I2C_DIVIDER, TunerS[0], &TunerS[1] , 6)>0)
       {
       mcSHOW_USER_MSG(("\nTunerSet() failed!\n"));
        return (-1);
       }
            mcDELAY(2);	
			
      TunerS[1] =0x85;
      TunerS[2] = Data[5];
      TunerS[3] = Data[6];
      TunerS[4] = Data[7];
      TunerS[5] = Data[8];

	  mcSHOW_DRVAPI_MSG(("the second time IFDEMOD RegSetRegistersTuner.\n"));

   //    if (RegSetRegistersTuner(TunerS[0], 5, &TunerS[1]) < 0)
        if(ICtrlBus_I2cTunerWrite(C_TDTWS710D_I2C_DIVIDER, TunerS[0], &TunerS[1] , 5)>0)
       {
       mcSHOW_USER_MSG(("\nTunerSet() failed!\n"));
        return (-1);
   }	
	//mcDELAY(70);
	mcDELAY(10);

	return (0);
}

//----------------------------------------------------------------------------- 
/** TDTWS710D_SetSawBw
 * Set/Get function
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  eOperation       To Get/Set operation id
 *  @retval  ITUNER_OK    Operation Success
 *  @retval  ITUNER_NOT_OK    Operation fail
 *  @retval  ITUNER_NOT_SUPPORTED Operation not supported in  driver
 */
//-----------------------------------------------------------------------------
STATIC VOID   TDTWS710D_SetSawBw( ITUNER_CTX_T * pTunerCtx,UINT8 sawbw)
{
   pTunerCtx->specific_member.eu_ctx.m_SAW_BW = sawbw ;
}

//----------------------------------------------------------------------------- 
/** TDTWS710D_GetLNAStatus
 * Set/Get function
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  eOperation       To Get/Set operation id
 *  @retval  ITUNER_OK    Operation Success
 *  @retval  ITUNER_NOT_OK    Operation fail
 *  @retval  ITUNER_NOT_SUPPORTED Operation not supported in  driver
 */
//-----------------------------------------------------------------------------
STATIC BOOL   TDTWS710D_GetLNAStatus( ITUNER_CTX_T * pTunerCtx)
{
    return fgTDTWS710DLNA ;
}



STATIC INT8   TDTWS710D_SetLNA( ITUNER_CTX_T * pTunerCtx,BOOL fgLNA)
{

 SPECIFIC_MEMBER_EU_CTX* pEUCtx= &( pTunerCtx->specific_member.eu_ctx);

  mcSHOW_DRVAPI_MSG(("TDTWS710D_SetLNA to %d.\n",fgLNA));
 
  if( fgLNA )	//ADC value is 100,011,010, LNA should turn ON
		{
		    pEUCtx->m_aucPara[18] |= 0x04;	//LNA ON, BS3=1
		    fgTDTWS710DLNA = TRUE ;
		    mcSHOW_DRVAPI_MSG(("TDTWS710D_SetLNA ON!!\n"));
			
#ifdef EU_AUTO_ATP
                   //need to do something
#endif
		}
  
		else
		{
               pEUCtx->m_aucPara[18] &= 0xFB;	//LNA OFF, BS3=0
                fgTDTWS710DLNA = FALSE ;
		    mcSHOW_DRVAPI_MSG(("TDTWS710D_SetLNA OFF!!\n"));
			
#ifdef EU_AUTO_ATP
                   //need to do something
#endif			
		}

 if(ICtrlBus_I2cTunerWrite(C_TDTWS710D_I2C_DIVIDER, pTunerCtx->I2cAddress, &(pEUCtx->m_aucPara[15] ), 5)>0) 
    {
        //TRACE(("\nTunerSet() failed!\n"));
        return (-1);
    }
  return 0;
		
}


/***************************************************************************
*  TDTWS710D_MonitorLNA
*  @param  *psAtvPiCtx : Pointer use to point to struct ATV_PI_CTX_T. 
*  @param  *wFreqInKHz : Pointer of current frequency (KHz).
*  @param  _BreakFct : Function pointer of the break function.
*  @param  *pvArg : Poniter of the break function parameter.
*  @retval  TRUE : Current VIF is lock or valid VIF signal is found.
*  @retval  FALSE : VIF I2C fail or valid VIF signal is not found.
****************************************************************************/

STATIC BOOL TDTWS710D_MonitorLNA(ITUNER_CTX_T * pTunerCtx)
{


  UINT8 bTunerData;
  BOOL fgLNAFw,fgLNALatest;
  static UINT8 bCount =0;

  
//  ITuner_OP(ITunerGetCtx(), itGetLNA, NULL, &fgLNAFw); //get the current LNA setting in tuner
     fgLNAFw = TDTWS710D_GetLNAStatus(pTunerCtx);

    ICtrlBus_I2cTunerRead(C_TDTWS710D_I2C_DIVIDER, pTunerCtx->I2cAddress, &bTunerData, 1);
    mcSHOW_DBG_MSG4(("bTunerData = 0X%x\n",bTunerData & 0x07));

        if ( (bTunerData&0x07) >0x01)  //ADC value is 100,011,010, LNA should turn ON
        	{
               fgLNALatest = TRUE; 
        	}
	  else
	  	{
               fgLNALatest = FALSE;
	  	}

	  if (fgLNALatest != fgLNAFw)
	  	{	  	  
                bCount ++;
                mcSHOW_DBG_MSG4(("TDTWS710D_MonitorLNA:LNA status start to change %d times!\n",bCount));				
	  	}
	   else
	   	{
	   	bCount = 0 ;//reset the count ...
	   	mcSHOW_DBG_MSG4(("TDTWS710D_MonitorLNA:%d\n",fgLNAFw));
               return TRUE ;
	   	}
	   
	  if (bCount >= C_TDTWS710D_LNA_SWITCH_THRESHOLD)
	  	{
	  	BOOL fgReturn =TRUE;
              // Call ITuner LNA ON/OFF function;
              mcSHOW_DBG_MSG4(("TDTWS710D_MonitorLNA:LNA need to be switched to %d!\n",fgLNALatest));
           //    TDTWS710D_AnaTunerSetFreq(psAtvPiCtx, *wFreqInKHz, ATV_CONN_MODE_CHAN_CHANGE); 
               TDTWS710D_SetLNA(pTunerCtx,fgLNALatest);
		bCount = 0;
		return (fgReturn);
	  	}
	  else
	  	{
               return (TRUE);
	  	}
 
}

//----------------------------------------------------------------------------- 
/** TDTWS710D_TunerOP
 * Set/Get function
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  eOperation       To Get/Set operation id
 *  @retval  ITUNER_OK    Operation Success
 *  @retval  ITUNER_NOT_OK    Operation fail
 *  @retval  ITUNER_NOT_SUPPORTED Operation not supported in  driver
 */
//-----------------------------------------------------------------------------
INT16 TDTWS710D_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){
         
         switch (eOperation){
              case itGetVer:
                 {
                    CHAR ** RetStr=(CHAR **)pInOutVal;
                    *RetStr=TDTWS710D_TunerGetVer();
                 }
                 break;
		  case itSetIfDemod:
		  {
		     INT8 * Retval = (INT8 *)pInOutVal;	
		    * Retval = TDTWS710D_SetIFDemod(pTCtx, SetVal); 
		  }
		  break;	
		   case itSetSawBw:
		   	TDTWS710D_SetSawBw(pTCtx, SetVal);
			break;
		  case itGetLNA :
		  	{
		  	BOOL * Retval = (BOOL *)pInOutVal;	
		  	* Retval  = TDTWS710D_GetLNAStatus(pTCtx);	
		  	}
			break;
		 case itSetLNA:
		  {
		     INT8 * Retval = (INT8 *)pInOutVal;	
		    * Retval = TDTWS710D_SetLNA(pTCtx, SetVal); 
		  }
		 break;
		 case itMonitorLNA:
		  {
		     BOOL * Retval = (BOOL *)pInOutVal;	
		    * Retval = TDTWS710D_MonitorLNA(pTCtx);
		  }
		 break;
		   default:
                 return ITUNER_NOT_SUPPORTED;
        }
         return ITUNER_OK;
}
