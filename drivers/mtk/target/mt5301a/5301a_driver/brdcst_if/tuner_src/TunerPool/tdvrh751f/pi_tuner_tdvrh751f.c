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
 * $RCSfile: pi_tuner_tdvrh751f.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file tuner_tdvrh751f.c
 *  SS TDVRH751F tuner driver.
 */
 
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "pi_def_dvbt.h"
#include "fe_tuner_common_if.h" 
#include "tuner_interface_if.h"
#include "ctrl_bus.h"
#include "PI_Def.h"
#include "pi_dvbt_if.h" 

#include "drvcust_if.h"   
#include "x_gpio.h"



//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define C_TDVRH751F_VERSION				"TDVRH751F v1.02"
// *** Release Note ***
// Ver     Date         Description
// 1.00    2006.12.12   file established
// 1.01	   2007.02.06	PLL IC change

// *** Memo ***
// 24cc Combo Tuner
#define GPIO_OUTPUT_ON     1
#define GPIO_OUTPUT_OFF    0

#define C_TDVRH751F_TOP_SET               ((U8)   0x02) 
#define C_TDVRH751F_SAW_BW                SAW_BW_6M 

#define C_TDVRH751F_FREQ_DBOUND_UPPER   858*1000*1000 
#define C_TDVRH751F_FREQ_DBOUND_LOWER    50*1000*1000 

#define C_TDVRH751F_LO_ADDRRESS           ((U16)  0xc2)
#define C_TDVRH751F_IF_FREQUENCY          ((U16)  44000)  
//#define C_TDVRH751F_IF_FREQUENCY_ANA      ((U16)  45750)  /* kHz */
//#define C_TDVRH751F_IF_FREQUENCY_L1       ((U16)  33900)  /* kHz */
#define C_TDVRH751F_LO_DIVIDER_STEP       ((U32) 166667) /* Hz */
#define C_TDVRH751F_LO_DIVIDER_STEP_ANA   ((U16) 62500) /* Hz */

#define C_TDVRH751F_AGC_IF_SLP             1
#define C_TDVRH751F_AGC_IF_INT             0
#define C_TDVRH751F_AGC_IF_MAX            0.499
#define C_TDVRH751F_AGC_IF_MIN             -0.5
#define C_TDVRH751F_AGC_IF_SLP_SGN  (S8)((C_TDVRH751F_AGC_IF_SLP>0)?1:-1) 
#define C_TDVRH751F_AGC_IF_LVL_MAX      0.499
#define C_TDVRH751F_AGC_IF_LVL_MIN     -0.5


#define C_TDVRH751F_MAX_IF_GAIN_SET       -0.12   
#define C_TDVRH751F_MIN_IF_GAIN_SET       -0.38   
#define C_TDVRH751F_MAX_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_TDVRH751F_MIN_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_TDVRH751F_MAX_IF_GAIN_POW       -100    // dBm
#define C_TDVRH751F_MIN_IF_GAIN_POW		-50     // dBm
#define C_TDVRH751F_MAX_RF_GAIN_POW		 C_TDVRH751F_MIN_IF_GAIN_POW
#define C_TDVRH751F_MIN_RF_GAIN_POW		-20     // dBm
#define C_TDVRH751F_POWER_CALIBRATE        26     // dBm
#define C_TDVRH751F_SIGNAL_LVL_TH          4

#define C_TDVRH751F_AGC_COEF				((U8)   0xBD)
#define C_TDVRH751F_DEMOD_INPUT_POWER		cIF_TARGET_LEVEL_0CCC

// ********************************************* //

#define C_U8_TDVRH751F_AGC_IF_SLP         (S8)(C_TDVRH751F_AGC_IF_SLP *32 )
#define C_U8_TDVRH751F_AGC_IF_INT         (S8)(C_TDVRH751F_AGC_IF_INT *64 )
#define C_U8_TDVRH751F_AGC_IF_MAX         (S8)(C_TDVRH751F_AGC_IF_MAX *256)
#define C_U8_TDVRH751F_AGC_IF_MIN         (S8)(C_TDVRH751F_AGC_IF_MIN *256)
#define C_U8_TDVRH751F_AGC_IF_LVL_MAX (S8)(C_TDVRH751F_AGC_IF_LVL_MAX *256)
#define C_U8_TDVRH751F_AGC_IF_LVL_MIN  (S8)(C_TDVRH751F_AGC_IF_LVL_MIN *256)

#define C_U8_TDVRH751F_MAX_IF_GAIN_SET	(S8)(C_TDVRH751F_MAX_IF_GAIN_SET *256)
#define C_U8_TDVRH751F_MIN_IF_GAIN_SET	(S8)(C_TDVRH751F_MIN_IF_GAIN_SET *256)
#define C_U8_TDVRH751F_MAX_RF_GAIN_SET	(S8)((C_TDVRH751F_MAX_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_TDVRH751F_MIN_RF_GAIN_SET	(S8)((C_TDVRH751F_MIN_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_TDVRH751F_IF_GAIN_RANGE      (S8)(C_TDVRH751F_MIN_IF_GAIN_POW - C_TDVRH751F_MAX_IF_GAIN_POW)
#define C_U8_TDVRH751F_RF_GAIN_RANGE      (S8)(C_TDVRH751F_MIN_RF_GAIN_POW - C_TDVRH751F_MAX_RF_GAIN_POW)

//----------------------------------------------------------------------------- 
/*
 *  TDVRH751F_GetSwVer
 *  Get driver version.
 *  @param  void.
 *  @retval   (CHAR *)Verion String. 
 */
//-----------------------------------------------------------------------------
CHAR* TDVRH751F_TunerGetVer(VOID)
{
	return ((CHAR*)C_TDVRH751F_VERSION);
}

//----------------------------------------------------------------------------- 
/*
 *  TDVRH751F_Initialize
 *  Tuner initialization.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @retval   void. 
 */
//-----------------------------------------------------------------------------
void TDVRH751F_TunerInit(ITUNER_CTX_T *pTCtx)
{
   
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_EU_CTX *psTunerSpecificCtx=&(psTunerCtx->specific_member.eu_ctx);

	psTunerCtx->I2cAddress = C_TDVRH751F_LO_ADDRRESS;
	psTunerCtx->u2IF_Freq = C_TDVRH751F_IF_FREQUENCY;
	psTunerSpecificCtx->m_SAW_BW  = C_TDVRH751F_SAW_BW;
	
	psTunerSpecificCtx->m_s4FreqBoundUpper = C_TDVRH751F_FREQ_DBOUND_UPPER;
	psTunerSpecificCtx->m_s4FreqBoundLower = C_TDVRH751F_FREQ_DBOUND_LOWER;

	// add by JC, 081215, for Signal Level display
	psTunerSpecificCtx->m_ifAgc_lvl_max = (U8)C_U8_TDVRH751F_AGC_IF_LVL_MAX;
	psTunerSpecificCtx->m_ifAgc_lvl_min = (U8)C_U8_TDVRH751F_AGC_IF_LVL_MIN;
	
	psTunerSpecificCtx->m_aucPara[0] = C_TDVRH751F_TOP_SET;
	psTunerSpecificCtx->m_aucPara[1] = 0;

	psTunerSpecificCtx->m_aucPara[2] = (U8)C_U8_TDVRH751F_AGC_IF_SLP;
	psTunerSpecificCtx->m_aucPara[3] = (U8)C_U8_TDVRH751F_AGC_IF_INT;
	psTunerSpecificCtx->m_aucPara[4] = (U8)C_U8_TDVRH751F_AGC_IF_MAX;
    psTunerSpecificCtx->m_aucPara[5] = (U8)C_U8_TDVRH751F_AGC_IF_MIN;

	psTunerSpecificCtx->m_aucPara[6] = C_TDVRH751F_AGC_COEF;
	psTunerSpecificCtx->m_aucPara[7] = C_TDVRH751F_DEMOD_INPUT_POWER;

	psTunerSpecificCtx->m_aucPara[ 8] = (U8)C_U8_TDVRH751F_MAX_IF_GAIN_SET;
	psTunerSpecificCtx->m_aucPara[ 9] = (U8)C_U8_TDVRH751F_MIN_IF_GAIN_SET;
	psTunerSpecificCtx->m_aucPara[10] = (U8)C_U8_TDVRH751F_MAX_RF_GAIN_SET;
	psTunerSpecificCtx->m_aucPara[11] = (U8)C_U8_TDVRH751F_MIN_RF_GAIN_SET;
	psTunerSpecificCtx->m_aucPara[12] = (U8)C_U8_TDVRH751F_IF_GAIN_RANGE;
	psTunerSpecificCtx->m_aucPara[13] = (U8)C_U8_TDVRH751F_RF_GAIN_RANGE;
	psTunerSpecificCtx->m_aucPara[14] = C_TDVRH751F_POWER_CALIBRATE;
	psTunerSpecificCtx->m_aucPara[21] = 0x07;

    psTunerSpecificCtx->m_SigLvTh = C_TDVRH751F_SIGNAL_LVL_TH;
}

//----------------------------------------------------------------------------- 
/*
 *  TDVRH751F_SetSawBw
 *  Set tuner SAW bandwidth.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @param  sawbw  SAW bandwidth setting. (SAW_BW_6M, SAW_BW_7M, SAW_BW_8M, SAW_BW_5M).
 *  @retval   void. 
 */
//-----------------------------------------------------------------------------
void TDVRH751F_SetSawBw(
    ITUNER_CTX_T *pTCtx,
    UINT8 sawBw
    )
{
    pTCtx->specific_member.eu_ctx.m_SAW_BW = sawBw;
}

//----------------------------------------------------------------------------- 
/*
 *  TDVRH751F_SetTop
 *  Set tuner TOP (Take Over Point) setting.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @param  top_set  TOP setting.
 *  @retval   void. 
 */
//-----------------------------------------------------------------------------
void TDVRH751F_SetTop(
    ITUNER_CTX_T *pTCtx,
    UINT8 top_set
    )
{

	 pTCtx->specific_member.eu_ctx.m_aucPara[0] = top_set;
}


static VOID vInputGpioCtrl(BOOL Input)     //Input=1,Digitl in; Input=0,Analog in
{
  UINT32 GPIONum_INPUT_SWITCH = DRVCUST_OptGet(eTunerCtrlAna);
  if (Input)
        {
            mcSHOW_DBG_MSG(("Digtal input tuner (GPIO%d)\n", GPIONum_INPUT_SWITCH));
            GPIO_SetOut(GPIONum_INPUT_SWITCH, GPIO_OUTPUT_ON); // Digtal  in           
        }
  else
        {
            mcSHOW_DBG_MSG(("Analog input tuner (GPIO%d)\n", GPIONum_INPUT_SWITCH));
            GPIO_SetOut(GPIONum_INPUT_SWITCH, GPIO_OUTPUT_OFF); // Analog in
        }		
}

//----------------------------------------------------------------------------- 
/*
 *  TDVRH751F_SetFreq
 *  Set tuner frequency.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @param  freq  Frequency in kHz.
 *  @param  mode  (MOD_DVBT, MOD_NTSC_M).
 *  @param  autoSearch  
 *  @retval   (S16) 0: Success.
 *  @retval   (S16) -1: Fail. 
 */
//-----------------------------------------------------------------------------
S16 TDVRH751F_TunerSetFreq(
    ITUNER_CTX_T *pTCtx,
    PARAM_SETFREQ_T* param
    )
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
	SPECIFIC_MEMBER_EU_CTX *psTunerSpecificCtx=&(psTunerCtx->specific_member.eu_ctx);

	UCHAR uc_data;
	S16 ucTunerLockConfCnt;
	UCHAR TunerS[6];
	U32 Lo,freq;
	U16 Ndivider;
	UINT8   Nband, CB, AB,TunerDataCnt;
	S16 ii, jj;
    UINT16  u2LO_Freq = C_TDVRH751F_IF_FREQUENCY;
	  
    psTunerCtx->u4RF_Freq =  param->Freq;
    psTunerCtx->I2cAddress = C_TDVRH751F_LO_ADDRRESS;
	UINT8 mode = param->Modulation;
	freq = param->Freq;
   
    if(mode != MOD_DVBT) //analog reception
    {   
        vInputGpioCtrl(0);  // //Switch to Analog input 
        Lo = freq + C_TDVRH751F_IF_FREQUENCY + 1750; // kHz
        u2LO_Freq += 1750;
        Ndivider = (U16) ( (Lo*1000+C_TDVRH751F_LO_DIVIDER_STEP_ANA/2) / C_TDVRH751F_LO_DIVIDER_STEP_ANA);
        mcSHOW_DBG_MSG(("NTSC ! Freq = %d, Divider ratio = %04X PLL set freq=%d kHz\n",psTunerCtx->u4RF_Freq, Ndivider,(Ndivider*C_TDVRH751F_LO_DIVIDER_STEP_ANA/1000-C_TDVRH751F_IF_FREQUENCY)));
    }
    else
    { 
        vInputGpioCtrl(1);    //Switch to Digtal input 
        Lo = freq + C_TDVRH751F_IF_FREQUENCY;
        Ndivider = (U16) ( (Lo*1000+C_TDVRH751F_LO_DIVIDER_STEP/2) / C_TDVRH751F_LO_DIVIDER_STEP);
        mcSHOW_DBG_MSG(("DVBT ! Freq = %d, Divider ratio = %04X PLL set freq=%d kHz\n",psTunerCtx->u4RF_Freq, Ndivider,(Ndivider*C_TDVRH751F_LO_DIVIDER_STEP/1000-C_TDVRH751F_IF_FREQUENCY)));
    }

    // Conver freq to unit of MHz
    freq = freq/1000;
    
    if (freq < (C_TDVRH751F_FREQ_DBOUND_LOWER/1000/1000) || 
        freq > (C_TDVRH751F_FREQ_DBOUND_UPPER/1000/1000)  )
    {
        mcSHOW_DRVAPI_MSG(("Out of range for LO!\n"));
        //return (+1);
    }

    TunerDataCnt = 6;	
    if(mode != MOD_DVBT) //analog reception
    {
        CB = 0x86;
    }
    else
    {
        if (freq < 647)
            CB = 0xC4;
        else
            CB = 0xFC;        
    }
    AB = 0x50;
    if (freq < 167)
        Nband = 0x81;
    else if (freq < 450)
        Nband = 0x82;
    else
        Nband = 0x84;

    /* LO settings */
    TunerS[0] = (UINT8) 0x7F & (Ndivider >> 8);
    TunerS[1] = (UINT8) 0xFF & Ndivider;
    TunerS[2] = (UINT8) CB;    // Must be set first for init
    TunerS[3] = (UINT8) Nband;
    TunerS[4] = (UINT8) (CB&0xC7|0x18);              
    TunerS[5] = (UINT8) AB;
   			
    for (jj=0; jj<2; jj++)
    {   //  send this data 
        if (ICtrlBus_I2cTunerWrite(270,psTunerCtx->I2cAddress, TunerS, 6))
        {
            mcSHOW_DRVAPI_MSG(("\nTunerSetFreq() failed!\n"));
            return (-1);
        }        
        else
        {
            if(mode != MOD_DVBT)
            {
                mcSHOW_DBG_MSG(("LO_%02X: (%d) %02X-%02X-%02X-%02X-%02X-%02X (%dKHZ)\n",
                   psTunerCtx->I2cAddress, TunerDataCnt, TunerS[0], TunerS[1], TunerS[2], TunerS[3], TunerS[4], TunerS[5], Ndivider*C_TDVRH751F_LO_DIVIDER_STEP_ANA/1000 - u2LO_Freq ));
            }            
        }
        mcDELAY(10);
                
        // wait PLL lock
	#if 1
		ucTunerLockConfCnt = 0;
		for (ii=0; ii<10; ii++)
		{
			// Charles_060425, test I2C
			ICtrlBus_I2cTunerRead(270,psTunerCtx->I2cAddress, &uc_data, 1);
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

		if(ii < 10)
		{
			mcSHOW_DRVAPI_MSG(("freq = %d MHz, the %d-th time setting PLL success, cnt = %d\n" , freq, jj+1, ucTunerLockConfCnt));	
			break;
		}
		else
		{
			mcSHOW_DRVAPI_MSG(("freq = %d MHz, the %d-th time setting PLL fail\n" , freq, jj+1));		
		}
	#endif

	}
  
	psTunerSpecificCtx->m_aucPara[1] = TunerS[0];	//send register value to UI, LC 070102
	psTunerSpecificCtx->m_aucPara[15] = TunerS[1];
	psTunerSpecificCtx->m_aucPara[16] = TunerS[2];
	psTunerSpecificCtx->m_aucPara[17] = TunerS[3];
	psTunerSpecificCtx->m_aucPara[18] = TunerS[4];
	psTunerSpecificCtx->m_aucPara[19] = uc_data;
    
   // mcDELAY(10);	// add 10ms delay before bypass is turn off, LC 070613
    
    return (0);
}

//----------------------------------------------------------------------------- 
/*
 *  TDVRH751F_SetIFDemod
 *  Set IF demod IC.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @param  subSysIdx  (MOD_DVBT, MOD_PAL_BG, MOD_PAL_DK, MOD_PAL_I,	MOD_SECAM_L, MOD_SECAM_L1, MOD_NTSC_M).
 *  @retval   (S16) 0: Success.
 *  @retval   (S16) -1: Fail. 
 */
//-----------------------------------------------------------------------------
S16   TDVRH751F_SetIFDemod(
    ITUNER_CTX_T *pTCtx,
    UINT8 subSysIdx
    )
{
	return (0);
}
//----------------------------------------------------------------------------- 
/**  UV1336_TunerGetStatus
 *  Set Tuner PLL staus for UV1336 
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval TRUE        PLL lock
 *  @retval FALSE       PLL unlock
 */
//-----------------------------------------------------------------------------
BOOL TDVRH751F_TunerGetStatus(
    ITUNER_CTX_T *pTCtx
    )
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;

    UINT8 TunerS[1];

    if (ICtrlBus_I2cTunerRead(270,psTunerCtx->I2cAddress,TunerS, 1))
    {
        mcSHOW_HW_MSG(("Tuner read failed!\n"));
        return FALSE;
    }

    if (mcTEST_BIT(TunerS[0], 6))
        return TRUE;

    return FALSE;

}

//----------------------------------------------------------------------------- 
/** TDQU8_TunerOP
 * Set/Get function
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  eOperation       To Get/Set operation id
 *  @retval  ITUNER_OK    Operation Success
 *  @retval  ITUNER_NOT_OK    Operation fail
 *  @retval  ITUNER_NOT_SUPPORTED Operation not supported in  driver
 */
//-----------------------------------------------------------------------------
INT16 TDVRH751F_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){
         switch(eOperation){
              case itGetStatus:
                {
                    BOOL * Retval=(BOOL *)pInOutVal;
                    *Retval=TDVRH751F_TunerGetStatus(pTCtx);
                }
                break;
              case itGetVer:
                 {
                    CHAR ** RetStr=(CHAR **)pInOutVal;
                    *RetStr=TDVRH751F_TunerGetVer();
                 }
                 break;
              case itSetTop:
                 {
                   TDVRH751F_SetTop(pTCtx, SetVal);
                 }
			     break;
              case itSetSawBw:
                 {
                   TDVRH751F_SetSawBw(pTCtx, SetVal);  
                 }
			     break;
            
              case itSetIfDemod:  //not complete
                 {
                   TDVRH751F_SetIFDemod(pTCtx, SetVal);
                 }
			     break;
				
              default:
                 return ITUNER_NOT_SUPPORTED;
        }
         return ITUNER_OK;
}

