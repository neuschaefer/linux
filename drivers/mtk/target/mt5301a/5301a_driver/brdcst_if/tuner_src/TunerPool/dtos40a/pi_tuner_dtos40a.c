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
 * $RCSfile: pi_tuner_dtos40a.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file tuner_dtos40a.c
 *  SS DTOS40A tuner driver.
 */
 
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "pi_def_dvbt.h"
//#include "pd_common.h"
//#include "pi_dvbt_if.h"  //get tuner ctx
//#include "pi_demod.h"
// *** specify tuner related parameters here *** //
#include "tuner_interface_if.h"
#include "ctrl_bus.h"
#include "PI_Def.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define C_DTOS40A_VERSION				"DTOS40A v1.01"
// *** Release Note ***
// Ver     Date         Description
// 1.00    2006.12.12   file established
// 1.01	   2007.02.06	PLL IC change

// *** Memo ***
// 24cc Combo Tuner

#define C_DTOS40A_TOP_SET               ((U8)   0x02) 
#define C_DTOS40A_SAW_BW                SAW_BW_8M 

#define C_DTOS40A_FREQ_DBOUND_UPPER   858*1000*1000 
#define C_DTOS40A_FREQ_DBOUND_LOWER    50*1000*1000 

#define C_DTOS40A_LO_ADDRRESS           ((U16)  0xc2)
#define C_DTOS40A_IF_FREQUENCY          ((U16)  36125)  /* kHz */
#define C_DTOS40A_IF_FREQUENCY_ANA      ((U16)  38900)  /* kHz */
#define C_DTOS40A_IF_FREQUENCY_L1       ((U16)  33900)  /* kHz */
#define C_DTOS40A_LO_DIVIDER_STEP       ((U16) 62500) /* Hz */
#define C_DTOS40A_LO_DIVIDER_STEP_ANA   ((U16) 50000) /* Hz */

#define C_DTOS40A_AGC_IF_SLP             1
#define C_DTOS40A_AGC_IF_INT             0
#define C_DTOS40A_AGC_IF_MAX            0.499
#define C_DTOS40A_AGC_IF_MIN             -0.5
#define C_DTOS40A_AGC_IF_SLP_SGN  (S8)((C_DTOS40A_AGC_IF_SLP>0)?1:-1) 

#define C_DTOS40A_MAX_IF_GAIN_SET       -0.12   
#define C_DTOS40A_MIN_IF_GAIN_SET       -0.38   
#define C_DTOS40A_MAX_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_DTOS40A_MIN_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_DTOS40A_MAX_IF_GAIN_POW       -100    // dBm
#define C_DTOS40A_MIN_IF_GAIN_POW		-50     // dBm
#define C_DTOS40A_MAX_RF_GAIN_POW		 C_DTOS40A_MIN_IF_GAIN_POW
#define C_DTOS40A_MIN_RF_GAIN_POW		-20     // dBm
#define C_DTOS40A_POWER_CALIBRATE        26     // dBm
#define C_DTOS40A_SIGNAL_LVL_TH          4

#define C_DTOS40A_AGC_COEF				((U8)   0xBD)
#define C_DTOS40A_DEMOD_INPUT_POWER		cIF_TARGET_LEVEL_0CCC

// ********************************************* //

#define C_U8_DTOS40A_AGC_IF_SLP         (S8)(C_DTOS40A_AGC_IF_SLP *32 )
#define C_U8_DTOS40A_AGC_IF_INT         (S8)(C_DTOS40A_AGC_IF_INT *64 )
#define C_U8_DTOS40A_AGC_IF_MAX         (S8)(C_DTOS40A_AGC_IF_MAX *256)
#define C_U8_DTOS40A_AGC_IF_MIN         (S8)(C_DTOS40A_AGC_IF_MIN *256)

#define C_U8_DTOS40A_MAX_IF_GAIN_SET	(S8)(C_DTOS40A_MAX_IF_GAIN_SET *256)
#define C_U8_DTOS40A_MIN_IF_GAIN_SET	(S8)(C_DTOS40A_MIN_IF_GAIN_SET *256)
#define C_U8_DTOS40A_MAX_RF_GAIN_SET	(S8)((C_DTOS40A_MAX_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_DTOS40A_MIN_RF_GAIN_SET	(S8)((C_DTOS40A_MIN_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_DTOS40A_IF_GAIN_RANGE      (S8)(C_DTOS40A_MIN_IF_GAIN_POW - C_DTOS40A_MAX_IF_GAIN_POW)
#define C_U8_DTOS40A_RF_GAIN_RANGE      (S8)(C_DTOS40A_MIN_RF_GAIN_POW - C_DTOS40A_MAX_RF_GAIN_POW)

//FUNC_EXPORT void  DTOS40A_Initialize(GEN_TUNER_CTX_T* pTunerCtx);
//FUNC_EXPORT void  DTOS40A_SetSawBw(GEN_TUNER_CTX_T* pTunerCtx, U8 sawbw);
//FUNC_EXPORT void  DTOS40A_SetTop(GEN_TUNER_CTX_T* pTunerCtx, U8 top_set);
//FUNC_EXPORT S16   DTOS40A_SetFreq(GEN_TUNER_CTX_T* pTunerCtx, U32 Freq, U8 Mode, U16 AutoSearch);
//FUNC_EXPORT S16   DTOS40A_SetIFDemod(GEN_TUNER_CTX_T* pTunerCtx, U8 SubSysIdx);
//FUNC_EXPORT CHAR* DTOS40A_GetSwVer(void);

//extern UINT8  DVBT_GetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
///////////////////////////////////
// Samsung DTOS40A driver
///////////////////////////////////

//----------------------------------------------------------------------------- 
/*
 *  DTOS40A_GetSwVer
 *  Get driver version.
 *  @param  void.
 *  @retval   (CHAR *)Verion String. 
 */
//-----------------------------------------------------------------------------
CHAR* DTOS40A_GetSwVer(
    ITUNER_CTX_T *pTCtx
    )
{
	return ((CHAR*)C_DTOS40A_VERSION);
}

//----------------------------------------------------------------------------- 
/*
 *  DTOS40A_Initialize
 *  Tuner initialization.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @retval   void. 
 */
//-----------------------------------------------------------------------------
void DTOS40A_Initialize(
    ITUNER_CTX_T *pTCtx,
    UINT8 i2cAddr
    )
{
    GEN_TUNER_CTX_T *pTunerCtx = &pTCtx->dvbtTunerCtx;

	pTunerCtx->m_Address = C_DTOS40A_LO_ADDRRESS;
	pTunerCtx->m_IF_Freq = C_DTOS40A_IF_FREQUENCY;
	pTunerCtx->m_SAW_BW  = C_DTOS40A_SAW_BW;
	pTunerCtx->m_TV_Spec = MOD_DVBT;
	pTunerCtx->m_s4FreqBoundUpper = C_DTOS40A_FREQ_DBOUND_UPPER;
	pTunerCtx->m_s4FreqBoundLower = C_DTOS40A_FREQ_DBOUND_LOWER;
	
	pTunerCtx->m_aucPara[0] = C_DTOS40A_TOP_SET;
	pTunerCtx->m_aucPara[1] = 0;

	pTunerCtx->m_aucPara[2] = (U8)C_U8_DTOS40A_AGC_IF_SLP;
	pTunerCtx->m_aucPara[3] = (U8)C_U8_DTOS40A_AGC_IF_INT;
	pTunerCtx->m_aucPara[4] = (U8)C_U8_DTOS40A_AGC_IF_MAX;
	pTunerCtx->m_aucPara[5] = (U8)C_U8_DTOS40A_AGC_IF_MIN;

	pTunerCtx->m_aucPara[6] = C_DTOS40A_AGC_COEF;
	pTunerCtx->m_aucPara[7] = C_DTOS40A_DEMOD_INPUT_POWER;

	pTunerCtx->m_aucPara[ 8] = (U8)C_U8_DTOS40A_MAX_IF_GAIN_SET;
	pTunerCtx->m_aucPara[ 9] = (U8)C_U8_DTOS40A_MIN_IF_GAIN_SET;
	pTunerCtx->m_aucPara[10] = (U8)C_U8_DTOS40A_MAX_RF_GAIN_SET;
	pTunerCtx->m_aucPara[11] = (U8)C_U8_DTOS40A_MIN_RF_GAIN_SET;
	pTunerCtx->m_aucPara[12] = (U8)C_U8_DTOS40A_IF_GAIN_RANGE;
	pTunerCtx->m_aucPara[13] = (U8)C_U8_DTOS40A_RF_GAIN_RANGE;
	pTunerCtx->m_aucPara[14] = C_DTOS40A_POWER_CALIBRATE;
	pTunerCtx->m_aucPara[21] = 0x07;

    pTunerCtx->m_SigLvTh = C_DTOS40A_SIGNAL_LVL_TH;
}

//----------------------------------------------------------------------------- 
/*
 *  DTOS40A_SetSawBw
 *  Set tuner SAW bandwidth.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @param  sawbw  SAW bandwidth setting. (SAW_BW_6M, SAW_BW_7M, SAW_BW_8M, SAW_BW_5M).
 *  @retval   void. 
 */
//-----------------------------------------------------------------------------
void DTOS40A_SetSawBw(
    ITUNER_CTX_T *pTCtx,
    UINT8 sawBw
    )
{
    GEN_TUNER_CTX_T *pTunerCtx = &pTCtx->dvbtTunerCtx;

	pTunerCtx->m_SAW_BW = sawBw;
}

//----------------------------------------------------------------------------- 
/*
 *  DTOS40A_SetTop
 *  Set tuner TOP (Take Over Point) setting.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @param  top_set  TOP setting.
 *  @retval   void. 
 */
//-----------------------------------------------------------------------------
void DTOS40A_SetTop(
    ITUNER_CTX_T *pTCtx,
    UINT8 top_set
    )
{
    GEN_TUNER_CTX_T *pTunerCtx = &pTCtx->dvbtTunerCtx;

	pTunerCtx->m_aucPara[0] = top_set;
}


//----------------------------------------------------------------------------- 
/*
 *  DTOS40A_SetFreq
 *  Set tuner frequency.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @param  freq  Frequency in kHz.
 *  @param  mode  (MOD_DVBT, MOD_PAL_BG, MOD_PAL_DK, MOD_PAL_I,	MOD_SECAM_L, MOD_SECAM_L1, MOD_NTSC_M).
 *  @param  autoSearch  
 *  @retval   (S16) 0: Success.
 *  @retval   (S16) -1: Fail. 
 */
//-----------------------------------------------------------------------------
S16 DTOS40A_SetFreq(
    ITUNER_CTX_T *pTCtx,
    UINT32 freq,
    UINT8 mode,
    UINT16 autoSearch,
    UINT16 step
    )
{
    GEN_TUNER_CTX_T *pTunerCtx = &pTCtx->dvbtTunerCtx;

	UCHAR uc_data;
	S16 ucTunerLockConfCnt;
	UCHAR TunerS[5];
	//UCHAR u1IfAgc[2];
	U32 Lo;
	U16 Ndivider, CB11, CB12, CB2; 
	S16 ii, jj;
	
	//char    m_TunerLogValue [100];

    pTunerCtx->m_Freq = freq;
    pTunerCtx->m_Address = C_DTOS40A_LO_ADDRRESS;

    if( mode == MOD_DVBT) //digital reception
    {

	    Lo = freq + C_DTOS40A_IF_FREQUENCY; // kHz
	    //Ndivider = (U16) ( ((Lo * 1000) / DTOS403_LO_DIVIDER_STEP) + 0.5 );
	    Ndivider = (U16) ( (Lo*1000+C_DTOS40A_LO_DIVIDER_STEP/2) / C_DTOS40A_LO_DIVIDER_STEP);
	    // Conver freq to unit of MHz
	    freq = freq/1000;
	
	    if (freq < (C_DTOS40A_FREQ_DBOUND_LOWER/1000/1000) || 
	        freq > (C_DTOS40A_FREQ_DBOUND_UPPER/1000/1000)  )
	    {
	        mcSHOW_DRVAPI_MSG(("Out of range for LO!\n"));
	        //return (+1);
	    }
	    
		CB11 = 0xC0;	//Disable Xtal for VIF IC, Lo step 62.5kHz
		//CB12 = (pTunerCtx->m_aucPara[0]) & 0x0F | 0x80; // 1000xxxx
	    if (freq <= 146)       
		{
			CB12 = 0x81; //TOP=121dBuV	
			CB2 = 0x41;  //CP=83uA
		}
		else if (freq <= 300)  
		{        
			CB12 = 0x81; //TOP=121dBuV	
			CB2 = 0x62;  //CP=122uA
		}
		else if (freq <= 430)  
		{        
			CB12 = 0x81; //TOP=121dBuV	
			CB2 = 0x82;  //CP=163uA
		}
		else if (freq <= 606)  
		{        
			CB12 = 0x82; //TOP=118dBuV	
			CB2 = 0xA8;  //CP=254uA
		}
		else                 
	    {
			CB12 = 0x82; //TOP=118dBuV	
			CB2 = 0xC8;  //CP=400uA
		}
			
		//CB2 = CB2 | 0x04;	//LNA ON, BS3=1

		DTOS40A_SetIFDemod(pTCtx, MOD_DVBT);
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
            Lo = Freq + C_DTOS40A_IF_FREQUENCY_L1; // kHz
            Ndivider = (U16) ( (Lo*1000+C_DTOS40A_LO_DIVIDER_STEP_ANA/2) / C_DTOS40A_LO_DIVIDER_STEP_ANA);
            mcSHOW_DBG_MSG(("Freq = %d, Divider ratio = %04X PLL set freq=%d kHz\n",pTunerCtx->m_Freq, Ndivider,(Ndivider*C_DTOS40A_LO_DIVIDER_STEP_ANA/1000-C_DTOS40A_IF_FREQUENCY_L1)));
        }
        else
        {
            Lo = Freq + C_DTOS40A_IF_FREQUENCY_ANA; // kHz
            Ndivider = (U16) ( (Lo*1000+C_DTOS40A_LO_DIVIDER_STEP_ANA/2) / C_DTOS40A_LO_DIVIDER_STEP_ANA);
            mcSHOW_DBG_MSG(("Freq = %d, Divider ratio = %04X PLL set freq=%d kHz\n",pTunerCtx->m_Freq, Ndivider,(Ndivider*C_DTOS40A_LO_DIVIDER_STEP_ANA/1000-C_DTOS40A_IF_FREQUENCY_ANA)));
        }
    	    
	    // Conver freq to unit of MHz
	    freq = freq/1000;
	    
	    if (freq < 45 || freq > 865)
	    {
	        mcSHOW_DRVAPI_MSG(("Out of range for LO!\n"));
	        //return (+1);
	    }
	    
		CB11 = 0xCB;	//Enable Xtal for VIF IC, Lo step 50kHz
		CB12 = 0x80;    //
	    if (freq <= 147)       
		{
			CB2 = 0x01; 
		}
		else if (freq <= 431)  
		{        
			CB2 = 0x02;  
		}
		else                 
	    {
			CB2 = 0x08;
		}
    
    }
    	
    // LO settings
    TunerS[0] = (UCHAR) 0x7F & (Ndivider >> 8);
    TunerS[1] = (UCHAR) 0xFF & Ndivider;
    TunerS[2] = (UCHAR) CB11;  
    TunerS[3] = (UCHAR) CB2;
	TunerS[4] = (UCHAR) (CB12 | 0x08); // ATC = 1  

    for (jj=0; jj<2; jj++)
    {

//  send this data 
    //if (RegSetRegisters(pTunerCtx->m_Address, TunerS[0], 4, &TunerS[1]) < 0)
	    if (ICtrlBus_I2cTunerWrite(pTunerCtx->m_Address, TunerS, 5) < 0)
	    {
            mcSHOW_DRVAPI_MSG(("\nTunerSet() failed!\n"));
	        return (-1);
	    }
	
		mcDELAY(10);
		// wait PLL lock
	//#ifndef C_DEMO_PROGRAM
		ucTunerLockConfCnt = 0;
		for (ii=0; ii<10; ii++)
		//for (ii=0; ii<ccMAX_WAIT_TUNER_TIME; ii++)
		{
			//RegGetRegisters(pTunerCtx->m_Address, (pTunerCtx->m_Address) | (0x01), 1, &uc_data);
			// Charles_060425, test I2C
			ICtrlBus_I2cTunerRead(pTunerCtx->m_Address, &uc_data, 1);
			mcSHOW_DRVAPI_MSG(("PLL=0x%02X\n", uc_data));
			if (mcGET_FIELD(uc_data, 0x40, 6))  // if PLL lock
				ucTunerLockConfCnt++;
			else
				ucTunerLockConfCnt = 0;
	                
	                mcDELAY(10);
	                
			if (ucTunerLockConfCnt>ccTUNER_LOCK_CONF_THRESHOLD)
				break;
								
		}
		ucTunerLockConfCnt = ii;
		//mcDELAY(SOFTREG_MT5131_REG_TUNER_WAIT_LOCK_TIME);
	
		//if(ii < ccMAX_WAIT_TUNER_TIME)
		if(ii < 10)
		{
			//SYSTEMTIME  rCurSysTime;
			//GetLocalTime(&rCurSysTime);         // get current time
			mcSHOW_DRVAPI_MSG(("freq = %d MHz, the %d-th time setting PLL success, cnt = %d\n" , freq, jj+1, ucTunerLockConfCnt));	
			break;
		}
		else
		{
			mcSHOW_DRVAPI_MSG(("freq = %d MHz, the %d-th time setting PLL fail\n" , freq, jj+1));		
		}

	}
//#else
//	mcDELAY(200);
//#endif
	
    if( (mode == MOD_DVBT) )//|| (mode >= MOD_PAL_BG && !autoSearch) )
    {
  		if( (uc_data & 0x07) == 0x04 )		//ADC value is 100, LNA should turn ON
		{
			TunerS[3] = TunerS[3] | 0x04;	//LNA ON, BS3=1
		    mcSHOW_DRVAPI_MSG(("DTV, LNA ON!!\n"));
		}

        if(autoSearch)
        {
            //DEMOD_CTX_T    sDvbtDemodCtx;
            //sDvbtDemodCtx.I2cAddress = 0x82;

            mcDELAY(40); // add delay for getting a stable IF AGC value
            /*** Record IfAgc value before LNA on for BestMux selection ***/  //Menghau, 080310
            //DVBT_GetReg(&sDvbtDemodCtx, 0x1DB, u1IfAgc, 2);
            //pTunerCtx->m_SigLvScan = -(((S8)u1IfAgc[1])*C_DTOS40A_AGC_IF_SLP_SGN);
            //mcSHOW_DRVAPI_MSG(("DTOS40A IfAgc = %d\n", pTunerCtx->m_SigLvScan));
            /*** Record IfAgc value before LNA on for BestMux selection ***/
        } 
    }
    else if(mode >= MOD_PAL_BG)
    {
    	if (autoSearch)
    	{
			TunerS[3] = TunerS[3] | 0x04;	//LNA ON, BS3=1
		    mcSHOW_DRVAPI_MSG(("ATV scan, LNA ON!!\n" ));
    	}
    	else
    	{
	        mcDELAY(300);	
	        ICtrlBus_I2cTunerRead(pTunerCtx->m_Address, &uc_data, 1);
			if( (uc_data & 0x07) == 0x04 )		//ADC value is 100, LNA should turn ON
			{
				TunerS[3] = TunerS[3] | 0x04;	//LNA ON, BS3=1
			    mcSHOW_DRVAPI_MSG(("ATV, LNA ON!!\n" ));
			}
		}
    }

    if( mode == MOD_SECAM_L1 )
    {
    	DTOS40A_SetIFDemod(pTCtx, mode);
    }
    
    // send auxiliary data //
	TunerS[4] = TunerS[4] & 0xF7; // set ATC = 1'b0
    //if (RegSetRegisters(pTunerCtx->m_Address, TunerS[0], 4, &TunerS[1]) < 0)
    if (ICtrlBus_I2cTunerWrite(pTunerCtx->m_Address, TunerS, 5) < 0)
    {
        //TRACE(("\nTunerSet() failed!\n"));
        return (-1);
    }

	pTunerCtx->m_aucPara[1] = TunerS[0];	//send register value to UI, LC 070102
	pTunerCtx->m_aucPara[15] = TunerS[1];
	pTunerCtx->m_aucPara[16] = TunerS[2];
	pTunerCtx->m_aucPara[17] = TunerS[3];
	pTunerCtx->m_aucPara[18] = TunerS[4];
	pTunerCtx->m_aucPara[19] = uc_data;
    
    mcDELAY(10);	// add 10ms delay before bypass is turn off, LC 070613
    
    return (0);
}

//----------------------------------------------------------------------------- 
/*
 *  DTOS40A_SwitchLNA
 *  LNA switch.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @retval   (S16) 0: Success.
 *  @retval   (S16) -1: Fail. 
 */
//-----------------------------------------------------------------------------
S16 DTOS40A_SwitchLNA(
    ITUNER_CTX_T *pTCtx
    )
{
    GEN_TUNER_CTX_T *pTunerCtx = &pTCtx->dvbtTunerCtx;

	UCHAR uc_data[7];
	//char    PLL_LogValue     [100];

	uc_data[0]=pTunerCtx->m_aucPara[1];
	uc_data[1]=pTunerCtx->m_aucPara[15];
	uc_data[2]=pTunerCtx->m_aucPara[16];
	uc_data[3]=pTunerCtx->m_aucPara[17];
	uc_data[4]=pTunerCtx->m_aucPara[18];
	uc_data[5]=pTunerCtx->m_aucPara[19];
		
	mcDELAY(10);
	ICtrlBus_I2cTunerRead(0xC2, &uc_data, 1);
		
	    if(mcGET_FIELD(uc_data[6], 0x40, 6))
		{
			if( ( (pTunerCtx->m_aucPara[21] == 0x07) && ((uc_data[6] & 0x07) != (uc_data[5] & 0x07)) ) || \
			( (pTunerCtx->m_aucPara[21] != 0x07) && ((uc_data[6] & 0x07) != (pTunerCtx->m_aucPara[21] & 0x07)) ) ) 
			{
				if( (uc_data[6] & 0x07) == 0x04 )
				{	
					uc_data[3] = uc_data[3] | 0x04;
					//SPRINTF((PLL_LogValue, "During reset, PLL=0x%02X, LNA OFF => ON", uc_data[6])); 
					//MT5131_DriverLog("C:\\DTOS40A.txt", PLL_LogValue, TRUE);
					mcSHOW_DRVAPI_MSG(("During reset, PLL=0x%02X, LNA OFF => ON\n", uc_data[6]));
					pTunerCtx->m_aucPara[21] = uc_data[6];
					//RegSetRegisters(0xC2, uc_data[0], 4, &uc_data[1]);
					ICtrlBus_I2cTunerWrite(0xC2, uc_data, 5);
				}
				else if( ( (pTunerCtx->m_aucPara[21] == 0x07) && ((uc_data[5] & 0x07) == 0x04) ) ||  \
					((pTunerCtx->m_aucPara[21] & 0x07) == 0x04) )
				{
					uc_data[3] = uc_data[3] & 0xFB;
					//SPRINTF((PLL_LogValue, "During reset, PLL=0x%02X, LNA ON => OFF", uc_data[6])); 
					//MT5131_DriverLog("C:\\DTOS40A.txt", PLL_LogValue, TRUE);
					mcSHOW_DRVAPI_MSG(("During reset, PLL=0x%02X, LNA ON => OFF\n", uc_data[6])); 
					pTunerCtx->m_aucPara[21] = uc_data[6];
					//RegSetRegisters(0xC2, uc_data[0], 4, &uc_data[1]);
					ICtrlBus_I2cTunerWrite(0xC2, uc_data, 5);
				}
			//mcDELAY(10);
			//pMt5131Ctx->m_PLLAD = uc_data_2[6];
			//RegSetRegisters(0xC2, uc_data_2[0], 4, &uc_data_2[1]);
			}
		}
		mcDELAY(10);
		//RegSetRegisters(0xC2, uc_data_2[0], 4, &uc_data_2[1]);
		//mcDELAY(10);
		
	return (0);
}

//----------------------------------------------------------------------------- 
/*
 *  DTOS40A_SetIFDemod
 *  Set IF demod IC.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @param  subSysIdx  (MOD_DVBT, MOD_PAL_BG, MOD_PAL_DK, MOD_PAL_I,	MOD_SECAM_L, MOD_SECAM_L1, MOD_NTSC_M).
 *  @retval   (S16) 0: Success.
 *  @retval   (S16) -1: Fail. 
 */
//-----------------------------------------------------------------------------
S16   DTOS40A_SetIFDemod(
    ITUNER_CTX_T *pTCtx,
    UINT8 subSysIdx
    )
{
    GEN_TUNER_CTX_T *pTunerCtx = &pTCtx->dvbtTunerCtx;

    UCHAR TunerS[5];
	
	TunerS[0] = 0x86;	// IF IC slave address
	TunerS[1] = 0x00;	// IF IC sub address
	
	switch (subSysIdx)
	{
	case MOD_DVBT:
	    if (pTunerCtx->m_SAW_BW == SAW_BW_8M)
		    TunerS[2] = 0x96; //Saw_BW = 8M
		else
		    TunerS[2] = 0x16; //Saw_BW = 7M 
	
		TunerS[3] = 0x70; 
		TunerS[4] = 0x09;
		break;

	case MOD_PAL_BG:
		TunerS[2] = 0x56;	// B-Data
		TunerS[3] = 0x70;	// C-Data
		TunerS[4] = 0x09;	// E-Data		
		break;
	case MOD_PAL_DK:
		TunerS[2] = 0x56;	// B-Data
		TunerS[3] = 0x70;	// C-Data
		TunerS[4] = 0x0B;	// E-Data
		break;
	case MOD_PAL_I:
		TunerS[2] = 0x56;	// B-Data
		TunerS[3] = 0x70;	// C-Data
		TunerS[4] = 0x0A;	// E-Data
		break;	
	case MOD_SECAM_L:
		TunerS[2] = 0x46;	// B-Data
		TunerS[3] = 0x50;	// C-Data
		TunerS[4] = 0x0B;	// E-Data
		break;
	case MOD_SECAM_L1:
		TunerS[2] = 0xC6;	// B-Data
		TunerS[3] = 0x50;	// C-Data
		TunerS[4] = 0x13;	// E-Data
		break;
	default:
		TunerS[2] = 0x56;	// B-Data
		TunerS[3] = 0x10;	// C-Data
		TunerS[4] = 0x08;	// E-Data
		break;
	}
	
	mcDELAY(2);
	//if (RegSetRegisters(TunerS[0], TunerS[1], 3, &TunerS[2]) < 0)
    if (ICtrlBus_I2cTunerWrite(TunerS[0], &TunerS[1], 4) < 0)
    {
    mcSHOW_HW_MSG(("\nTunerSet() failed!\n"));
    return (-1);
    }
	
	//mcDELAY(70);
	mcDELAY(10);

	return (0);
}

