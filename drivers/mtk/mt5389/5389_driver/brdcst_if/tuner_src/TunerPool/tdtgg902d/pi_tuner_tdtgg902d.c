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
 * $RCSfile: pi_tuner_tdtgg902d.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
 
/** @file tuner_TDTGG902D.c
 *  LG TDTGG902D tuner driver.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "pi_def_dvbt.h"
//#include "pd_common.h"
//#include "pi_dvbt_if.h"  //get tuner ctx
//#include "pi_demod.h"
#include "tuner_interface_if.h"
#include "ctrl_bus.h"
#include "PI_Def.h"

// *** specify tuner related parameters here *** //

#define C_TDTGG902D_VERSION				"TDTGG902D 2010-01-01\n 2011-04-09 \n v1.0"
// *** Release Note ***
// Ver     Date         Description
// 1.00    2008.11.17   file established

// *** Memo ***
// 24cc Combo Tuner

#define C_TDTGG902D_TOP_SET               ((U8)   0x05)
#define C_TDTGG902D_SAW_BW                SAW_BW_8M 

#define C_TDTGG902D_FREQ_DBOUND_UPPER       862*1000*1000 
#define C_TDTGG902D_FREQ_DBOUND_LOWER       47*1000*1000 

#define C_TDTGG902D_LO_ADDRRESS           ((U16)  0xc2)
#define C_TDTGG902D_IF_FREQUENCY          ((U16)  36167)  /* kHz */
#define C_TDTGG902D_IF_FREQUENCY_ANA      ((U16)  38900)  /* kHz */
#define C_TDTGG902D_IF_FREQUENCY_L1       ((U16)  33900)  /* kHz */
#define C_TDTGG902D_LO_DIVIDER_STEP       ((U32)  166670) /* Hz */
#define C_TDTGG902D_LO_DIVIDER_STEP_ANA   ((U16)  50000) /* Hz */

#define C_TDTGG902D_CHANNEL_SCAN_JUMP_SMALL_STEP ((U16) 250)
#define C_TDTGG902D_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP ((U16) 800)
#define C_TDTGG902D_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN ((U16) 250)

#define C_TDTGG902D_AGC_IF_SLP             1
#define C_TDTGG902D_AGC_IF_INT             0
#define C_TDTGG902D_AGC_IF_MAX             0.499
#define C_TDTGG902D_AGC_IF_MIN            -0.5
#define C_TDTGG902D_AGC_IF_LVL_MAX      0.499
#define C_TDTGG902D_AGC_IF_LVL_MIN     -0.5

#define C_TDTGG902D_MAX_IF_GAIN_SET       -0.12   
#define C_TDTGG902D_MIN_IF_GAIN_SET       -0.38   
#define C_TDTGG902D_MAX_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_TDTGG902D_MIN_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_TDTGG902D_MAX_IF_GAIN_POW       -100    // dBm
#define C_TDTGG902D_MIN_IF_GAIN_POW		-50     // dBm
#define C_TDTGG902D_MAX_RF_GAIN_POW		 C_TDTGG902D_MIN_IF_GAIN_POW
#define C_TDTGG902D_MIN_RF_GAIN_POW		-20     // dBm
#define C_TDTGG902D_POWER_CALIBRATE        26     // dBm
#define C_TDTGG902D_SIGNAL_LVL_TH          4

#define C_TDTGG902D_AGC_COEF				((U8)   0xBD)
#define C_TDTGG902D_DEMOD_INPUT_POWER		cIF_TARGET_LEVEL_0CCC

// ********************************************* //

#define C_U8_TDTGG902D_AGC_IF_SLP         (S8)(C_TDTGG902D_AGC_IF_SLP *32 )
#define C_U8_TDTGG902D_AGC_IF_INT         (S8)(C_TDTGG902D_AGC_IF_INT *64 )
#define C_U8_TDTGG902D_AGC_IF_MAX         (S8)(C_TDTGG902D_AGC_IF_MAX *256)
#define C_U8_TDTGG902D_AGC_IF_MIN         (S8)(C_TDTGG902D_AGC_IF_MIN *256)
#define C_U8_TDTGG902D_AGC_IF_LVL_MAX (S8)(C_TDTGG902D_AGC_IF_LVL_MAX *256)
#define C_U8_TDTGG902D_AGC_IF_LVL_MIN  (S8)(C_TDTGG902D_AGC_IF_LVL_MIN *256)

#define C_U8_TDTGG902D_MAX_IF_GAIN_SET	(S8)(C_TDTGG902D_MAX_IF_GAIN_SET *256)
#define C_U8_TDTGG902D_MIN_IF_GAIN_SET	(S8)(C_TDTGG902D_MIN_IF_GAIN_SET *256)
#define C_U8_TDTGG902D_MAX_RF_GAIN_SET	(S8)((C_TDTGG902D_MAX_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_TDTGG902D_MIN_RF_GAIN_SET	(S8)((C_TDTGG902D_MIN_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_TDTGG902D_IF_GAIN_RANGE      (S8)(C_TDTGG902D_MIN_IF_GAIN_POW - C_TDTGG902D_MAX_IF_GAIN_POW)
#define C_U8_TDTGG902D_RF_GAIN_RANGE      (S8)(C_TDTGG902D_MIN_RF_GAIN_POW - C_TDTGG902D_MAX_RF_GAIN_POW)

//void  TDTGG902D_Initialize(GEN_TUNER_CTX_T* pTunerCtx);
//void  TDTGG902D_SetSawBw(GEN_TUNER_CTX_T* pTunerCtx, U8 sawbw);
//void  TDTGG902D_SetTop(GEN_TUNER_CTX_T* pTunerCtx, U8 top_set);
//S16   TDTGG902D_SetFreqTop(GEN_TUNER_CTX_T* pTunerCtx, U32 Freq);
//S16   TDTGG902D_SetFreq(GEN_TUNER_CTX_T* pTunerCtx, U32 Freq, U8 Mode);
//S16   TDTGG902D_SetIFDemod(GEN_TUNER_CTX_T* pTunerCtx, U8 SubSysIdx);
//U8	  TDTGG902D_ATV_GetAFC(GEN_TUNER_CTX_T* pTunerCtx);

//CHAR* TDTGG902D_GetSwVer(void);

//----------------------------------------------------------------------------- 
/*
 *  TDTGG902D_GetSwVer
 *  Get driver version.
 *  @param  pTCtx. Tuner Context
 *  @retval   (CHAR *)Verion String. 
 */
//-----------------------------------------------------------------------------
char* TDTGG902D_GetSwVer(
    ITUNER_CTX_T *pTCtx
    )
{
	return ((CHAR*)C_TDTGG902D_VERSION);
}

//----------------------------------------------------------------------------- 
/*
 *  TDTGG902D_Initialize
 *  Tuner initialization.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @retval   void. 
 */
//-----------------------------------------------------------------------------
void TDTGG902D_Initialize(
    ITUNER_CTX_T *pTCtx,
    UINT8 i2cAddr
    )
{
	GEN_TUNER_CTX_T *pTunerCtx = &pTCtx->dvbtTunerCtx;
	
	pTunerCtx->m_Address = C_TDTGG902D_LO_ADDRRESS;
	pTunerCtx->m_IF_Freq = C_TDTGG902D_IF_FREQUENCY;
	pTunerCtx->m_SAW_BW  = C_TDTGG902D_SAW_BW;
	pTunerCtx->m_TV_Spec = MOD_DVBT;
	pTunerCtx->m_s4FreqBoundUpper = C_TDTGG902D_FREQ_DBOUND_UPPER;
	pTunerCtx->m_s4FreqBoundLower = C_TDTGG902D_FREQ_DBOUND_LOWER;

	//add by liuqu,20081111
	pTunerCtx->m_Small_Step = C_TDTGG902D_CHANNEL_SCAN_JUMP_SMALL_STEP;//set small step as 250k
	pTunerCtx->m_Middle_Step_Up = C_TDTGG902D_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP;//set middle step as 1500k,tda9886
	pTunerCtx->m_Middle_Step_Down= C_TDTGG902D_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN;//set middle step as 1500k,tda9886

	// add by JC, 081215, for Signal Level display
	pTunerCtx->m_ifAgc_lvl_max = (U8)C_U8_TDTGG902D_AGC_IF_LVL_MAX;
	pTunerCtx->m_ifAgc_lvl_min = (U8)C_U8_TDTGG902D_AGC_IF_LVL_MIN;
	
	pTunerCtx->m_aucPara[0] = C_TDTGG902D_TOP_SET;
	pTunerCtx->m_aucPara[1] = 0;

	pTunerCtx->m_aucPara[2] = (U8)C_U8_TDTGG902D_AGC_IF_SLP;
	pTunerCtx->m_aucPara[3] = (U8)C_U8_TDTGG902D_AGC_IF_INT;
	pTunerCtx->m_aucPara[4] = (U8)C_U8_TDTGG902D_AGC_IF_MAX;
	pTunerCtx->m_aucPara[5] = (U8)C_U8_TDTGG902D_AGC_IF_MIN;

	pTunerCtx->m_aucPara[6] = C_TDTGG902D_AGC_COEF;
	pTunerCtx->m_aucPara[7] = C_TDTGG902D_DEMOD_INPUT_POWER;

	pTunerCtx->m_aucPara[ 8] = (U8)C_U8_TDTGG902D_MAX_IF_GAIN_SET;
	pTunerCtx->m_aucPara[ 9] = (U8)C_U8_TDTGG902D_MIN_IF_GAIN_SET;
	pTunerCtx->m_aucPara[10] = (U8)C_U8_TDTGG902D_MAX_RF_GAIN_SET;
	pTunerCtx->m_aucPara[11] = (U8)C_U8_TDTGG902D_MIN_RF_GAIN_SET;
	pTunerCtx->m_aucPara[12] = (U8)C_U8_TDTGG902D_IF_GAIN_RANGE;
	pTunerCtx->m_aucPara[13] = (U8)C_U8_TDTGG902D_RF_GAIN_RANGE;
	pTunerCtx->m_aucPara[14] = C_TDTGG902D_POWER_CALIBRATE;
	pTunerCtx->m_aucPara[21] = 0x07;
	
	pTunerCtx->m_SigLvTh = C_TDTGG902D_SIGNAL_LVL_TH;
}

#if 0
void TDTGG902D_SetTvSpec(GEN_TUNER_CTX_T* pTunerCtx, U8 u1TvSpec)
{
	pTunerCtx->m_TV_Spec = u1TvSpec;
}
#endif

//----------------------------------------------------------------------------- 
/*
 *  TDTGG902D_SetSawBw
 *  Set tuner SAW bandwidth.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @param  sawbw  SAW bandwidth setting. (SAW_BW_6M, SAW_BW_7M, SAW_BW_8M, SAW_BW_5M).
 *  @retval   void. 
 */
//-----------------------------------------------------------------------------
void TDTGG902D_SetSawBw(
    ITUNER_CTX_T *pTCtx,
    UINT8 sawBw
    )
{
    GEN_TUNER_CTX_T *pTunerCtx = &pTCtx->dvbtTunerCtx;
    
	pTunerCtx->m_SAW_BW = sawBw;
}

//----------------------------------------------------------------------------- 
/*
 *  TDTGG902D_SetTop
 *  Set tuner TOP (Take Over Point) setting.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @param  top_set  TOP setting.
 *  @retval   void. 
 */
//-----------------------------------------------------------------------------
void TDTGG902D_SetTop(
    ITUNER_CTX_T *pTCtx,
    UINT8 top_set
    )
{
    GEN_TUNER_CTX_T *pTunerCtx = &pTCtx->dvbtTunerCtx;
    
	pTunerCtx->m_aucPara[0] = top_set;
}

#if 0
S16 TDTGG902D_SetFreqTop(GEN_TUNER_CTX_T* pTunerCtx, U32 Freq)
{
	S16 fgRet = 0;

	fgRet |= TDTGG902D_SetFreq(pTunerCtx, Freq, pTunerCtx->m_TV_Spec);
	fgRet |= TDTGG902D_SetIFDemod(pTunerCtx, pTunerCtx->m_TV_Spec);

	return fgRet;
}
#endif

//----------------------------------------------------------------------------- 
/*
 *  TDTGG902D_SetFreq
 *  Set tuner frequency.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @param  freq  Frequency in kHz.
 *  @param  mode  (MOD_DVBT, MOD_PAL_BG, MOD_PAL_DK, MOD_PAL_I,	MOD_SECAM_L, MOD_SECAM_L1, MOD_NTSC_M).
 *  @param  autoSearch  
 *  @retval   (S16) 0: Success.
 *  @retval   (S16) -1: Fail. 
 */
//-----------------------------------------------------------------------------
S16 TDTGG902D_SetFreq(
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
	U32 Lo;
	U16 Ndivider, CB1, CB2, BB; 
	S16 ii, jj;
	U8 ucAnaTop = 0xff;
	
	pTunerCtx->m_Freq = freq;
    pTunerCtx->m_Address = C_TDTGG902D_LO_ADDRRESS;

    if( mode == MOD_DVBT) //digital reception
    {

	    Lo = freq + C_TDTGG902D_IF_FREQUENCY; // kHz
	    //Ndivider = (U16) ( ((Lo * 1000) / DTOS403_LO_DIVIDER_STEP) + 0.5 );
	  
	    Ndivider = (U16) ( (Lo*1000+C_TDTGG902D_LO_DIVIDER_STEP/2) / C_TDTGG902D_LO_DIVIDER_STEP);
	    // Conver Freq to unit of MHz
	    freq = freq/1000;
	
	    if ((freq < (C_TDTGG902D_FREQ_DBOUND_LOWER/1000/1000)) || (freq > (C_TDTGG902D_FREQ_DBOUND_UPPER/1000/1000)))
	    {
	        mcSHOW_DRVAPI_MSG(("Out of range for LO!\n"));
	        //return (+1);
	    }
	    
	    if (freq < 146)       
		{
			BB = 0x41;
		}
		else if (freq < 195)  
		{        
			BB = 0x42;
		}
		else if (freq < 430)  
		{        
			BB = 0x82;
		}
		else if (freq < 670)
	    {
			BB = 0x88;
		}
		else
		{
			BB = 0xC8;
		}
		
		if (pTunerCtx->m_SAW_BW == SAW_BW_8M)
			BB |= 0x10;   // P5 = 1 if 8M

	    CB1 = 0xa8;
        CB2 = 0xc1;	

	 //merge TOP setting
	   CB1 = (CB1&0xC7)|((pTunerCtx->m_aucPara[0]&0x07)<<3);  // 0x **xx x***
	   CB2 = (CB2&0xFD)|((pTunerCtx->m_aucPara[0]&0x08)>>2); // 0X **** **x*
	   
	   //TDTGG902D_SetIFDemod(pTunerCtx, MOD_DVBT);
    }
    else //analog reception
    {
        if( mode == MOD_SECAM_L1 )
        {
            Lo = freq + C_TDTGG902D_IF_FREQUENCY_L1; // kHz
            Ndivider = (U16) ( (Lo*1000+C_TDTGG902D_LO_DIVIDER_STEP_ANA/2) / C_TDTGG902D_LO_DIVIDER_STEP_ANA);
            mcSHOW_DBG_MSG(("Freq = %d, Divider ratio = %04X PLL set freq=%d kHz\n",pTunerCtx->m_Freq, Ndivider,(Ndivider*C_TDTGG902D_LO_DIVIDER_STEP_ANA/1000-C_TDTGG902D_IF_FREQUENCY_L1)));
        }
        else
        {
            Lo = freq + C_TDTGG902D_IF_FREQUENCY_ANA; // kHz
            Ndivider = (U16) ( (Lo*1000+C_TDTGG902D_LO_DIVIDER_STEP_ANA/2) / C_TDTGG902D_LO_DIVIDER_STEP_ANA);
            mcSHOW_DBG_MSG(("Freq = %d, Divider ratio = %04X PLL set freq=%d kHz\n",pTunerCtx->m_Freq, Ndivider,(Ndivider*C_TDTGG902D_LO_DIVIDER_STEP_ANA/1000-C_TDTGG902D_IF_FREQUENCY_ANA)));
        }
	    
	    // Conver Freq to unit of MHz
	    freq = freq/1000;
	    
	    if ((freq < (C_TDTGG902D_FREQ_DBOUND_LOWER/1000/1000)) || (freq > (C_TDTGG902D_FREQ_DBOUND_UPPER/1000/1000)))
	    {
	        mcSHOW_DRVAPI_MSG(("Out of range for LO!\n"));
	        return (+1);
	    }
	    
		if (freq < 146)       
		{
			BB = 0x01;
		}
		else if (freq < 430)  
		{        
			BB = 0x02;
		}
		else
		{
			BB = 0x08;
		}
		BB |= 0x10;   // P5 = 1 if 8M
		
		CB1 = 0xbd;
        CB2 = 0xe2;

		if (mode == MOD_PAL_BG)
		{
			ucAnaTop = 0x0B;
			CB1 = (CB1&0xC7)|((ucAnaTop&0x07)<<3);  // 0x **xx x***
			CB2 = (CB2&0xFD)|((ucAnaTop&0x08)>>2);  // 0X **** **x*
		}
		else if (mode == MOD_PAL_I)
		{
			ucAnaTop = 0x0B;
			CB1 = (CB1&0XC7)|((ucAnaTop&0x07)<<3);  // 0x **xx x***
			CB2 = (CB2&0XFD)|((ucAnaTop&0x08)>>2);  // 0X **** **x*
		}
		else if (mode == MOD_PAL_DK)
		{
			ucAnaTop = 0x0B;
			CB1 = (CB1&0XC7)|((ucAnaTop&0x07)<<3);  // 0x **xx x***
			CB2 = (CB2&0XFD)|((ucAnaTop&0x08)>>2);  // 0X **** **x*
		}
		else if (mode == MOD_SECAM_L)
		{
			ucAnaTop = 0xa;
			CB1 = (CB1&0XC7)|((ucAnaTop&0x07)<<3);  // 0x **xx x***
			CB2 = (CB2&0XFD)|((ucAnaTop&0x08)>>2);  // 0X **** **x*
			BB |= 0x20;   // For SECAM L and L', mixer output
		}
		else if (mode == MOD_SECAM_L1)
		{
			ucAnaTop = 0x0a;
			CB1 = (CB1&0XC7)|((ucAnaTop&0x07)<<3);  // 0x **xx x***
			CB2 = (CB2&0XFD)|((ucAnaTop&0x08)>>2);  // 0X **** **x*
			BB |= 0x20;   // For SECAM L and L', mixer output
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
        if (ICtrlBus_I2cTunerWrite(pTunerCtx->m_Address, TunerS, 5) < 0)
	    {
            mcSHOW_DRVAPI_MSG(("\nTunerSet() failed!\n"));
	        return (-1);
	    }
	
		mcDELAY(10);
		// wait PLL lock
		ucTunerLockConfCnt = 0;
		for (ii=0; ii<20; ii++)
		{
			ICtrlBus_I2cTunerRead(pTunerCtx->m_Address, &uc_data, 1);
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
			mcSHOW_DRVAPI_MSG(("freq = %d MHz, the %d-th time setting PLL success, cnt = %d\n" , freq, jj+1, ucTunerLockConfCnt));	
			break;
		}
		else
		{
			mcSHOW_DRVAPI_MSG(("freq = %d MHz, the %d-th time setting PLL fail\n" , freq, jj+1));		
		}

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
 *  TDTGG902D_SetIFDemod
 *  Set IF demod IC.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @param  subSysIdx  (MOD_DVBT, MOD_PAL_BG, MOD_PAL_DK, MOD_PAL_I,	MOD_SECAM_L, MOD_SECAM_L1, MOD_NTSC_M).
 *  @retval   (S16) 0: Success.
 *  @retval   (S16) -1: Fail. 
 */
//-----------------------------------------------------------------------------
S16   TDTGG902D_SetIFDemod(
    ITUNER_CTX_T *pTCtx,
    UINT8 subSysIdx
    )
{
    //GEN_TUNER_CTX_T *pTunerCtx = &pTCtx->dvbtTunerCtx;

    UCHAR TunerS[3],Data[9];
    S16 ii;
	
	TunerS[0] = 0x86;	// IF IC slave address
	TunerS[1] = 0x00;	// IF IC sub address
	
	switch (subSysIdx)
	{
	case MOD_DVBT:
	    Data[0] = 0x21; 
		Data[1] = 0x12;
		Data[2] = 0x31;
		Data[3] = 0x63;
		Data[4] = 0x80;
		Data[5] = 0x5a;   // VCOstop=11 (D4,D3), SIFoff=1 (D1)
		Data[6] = 0x20;
		Data[7] = 0x00;
		Data[8] = 0x00;	
		break;
	case MOD_PAL_BG:
	    Data[0] = 0x21; 
		Data[1] = 0x12;
		Data[2] = 0x31;
		Data[3] = 0x63;
		Data[4] = 0x80;
		Data[5] = 0x40;
		Data[6] = 0x20;
		Data[7] = 0x00;
		Data[8] = 0x00;	
		break;
	case MOD_PAL_I:
		Data[0] = 0x21; 
		Data[1] = 0x12;
		Data[2] = 0x31;
		Data[3] = 0x63;
		Data[4] = 0x88;
		Data[5] = 0x40;
		Data[6] = 0x20;
		Data[7] = 0x00;
		Data[8] = 0x00;	
		break;
	case MOD_PAL_DK:
		Data[0] = 0x21; 
		Data[1] = 0x12;
		Data[2] = 0x31;
		Data[3] = 0x63;
		Data[4] = 0x90;
		Data[5] = 0x40;
		Data[6] = 0x20;
		Data[7] = 0x00;
		Data[8] = 0x00;	
		break;	
	case MOD_SECAM_L:
		Data[0] = 0x35; 
		Data[1] = 0x12;
		Data[2] = 0x31;
		Data[3] = 0x63;
		Data[4] = 0x90;
		Data[5] = 0x40;
		Data[6] = 0x20;
		Data[7] = 0x00;
		Data[8] = 0x00;	
		break;
	case MOD_SECAM_L1:
		Data[0] = 0x95; 
		Data[1] = 0x02;
		Data[2] = 0xb1;
		Data[3] = 0x03;
		Data[4] = 0x90;
		Data[5] = 0x40;
		Data[6] = 0x20;
		Data[7] = 0x00;
		Data[8] = 0x00;	
		break;
	default:
		Data[0] = 0x21; 
		Data[1] = 0x12;
		Data[2] = 0x31;
		Data[3] = 0x63;
		Data[4] = 0x80;
		Data[5] = 0x40;
		Data[6] = 0x20;
		Data[7] = 0x00;
		Data[8] = 0x00;	
		break;
	}

	mcDELAY(2);
	
	for(ii =0;ii<9;ii++)   //write data seprately. Slave addr---sub addr---data
	{
		TunerS[1] = (UCHAR)ii;  //sub addr
		TunerS[2] =Data[ii] ;
		if (ICtrlBus_I2cTunerWrite(TunerS[0], &TunerS[1], 2) < 0)
		{
			mcSHOW_HW_MSG(("\nTunerSet() failed!\n"));
			return (-1);
		}
		mcDELAY(2);
	}	
	mcDELAY(10);

	return (0);
}

#if 0
U8 TDTGG902D_ATV_GetAFC(GEN_TUNER_CTX_T* pTunerCtx)
{
	
	UCHAR	uc_data;
	
	mcDELAY(20);
	

	I2CRead(0x86, 1, &uc_data);
	return	uc_data;

}
#endif
