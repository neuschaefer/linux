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
 * $RCSfile: pi_tuner_uv1316e.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file tuner_dtos40a.c
 *  SS UV1316E tuner driver.
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

#ifdef ROW_SET_TOP_TOEEPROM
#include "eepdef.h" //add by liuqu,20081014
#endif

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define C_UV1316E_VERSION				"UV1316E v1.01"
// *** Release Note ***
// Ver     Date         Description
// 1.00    2006.12.12   file established
// 1.01	   2007.02.06	PLL IC change

// *** Memo ***
// 24cc Combo Tuner

#define C_UV1316E_TOP_SET               ((U8)   0x02) 
#define C_UV1316E_SAW_BW                SAW_BW_8M 
#define C_UV1316E_TOP_SET_ANA               ((U8)   0x10)

#define C_UV1316E_FREQ_DBOUND_UPPER   872*1000*1000 
#define C_UV1316E_FREQ_DBOUND_LOWER    44*1000*1000 

#define C_UV1316E_LO_ADDRRESS           ((U16)  0xc0)
#define C_UV1316E_IF_FREQUENCY          ((U16)  36125)  /* kHz */
#define C_UV1316E_IF_FREQUENCY_ANA      ((U16)  38900)  /* kHz */
#define C_UV1316E_IF_FREQUENCY_L1       ((U16)  33400)  /* kHz */
#define C_UV1316E_LO_DIVIDER_STEP       ((U16) 62500) /* Hz */
#define C_UV1316E_LO_DIVIDER_STEP_ANA   ((U16) 62500) /* Hz */

#define C_UV1316E_CHANNEL_SCAN_JUMP_SMALL_STEP ((U16) 250)
#define C_UV1316E_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP ((U16) 1500)
#define C_UV1316E_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN ((U16) 500)

#define C_UV1316E_AGC_IF_SLP             1
#define C_UV1316E_AGC_IF_INT             0
#define C_UV1316E_AGC_IF_MAX            0.499
#define C_UV1316E_AGC_IF_MIN             -0.5
#define C_UV1316E_AGC_IF_SLP_SGN  (S8)((C_UV1316E_AGC_IF_SLP>0)?1:-1) 
#define C_UV1316E_AGC_IF_LVL_MAX      0.499
#define C_UV1316E_AGC_IF_LVL_MIN     -0.5

#define C_UV1316E_MAX_IF_GAIN_SET       -0.12   
#define C_UV1316E_MIN_IF_GAIN_SET       -0.38   
#define C_UV1316E_MAX_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_UV1316E_MIN_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_UV1316E_MAX_IF_GAIN_POW       -100    // dBm
#define C_UV1316E_MIN_IF_GAIN_POW		-50     // dBm
#define C_UV1316E_MAX_RF_GAIN_POW		 C_UV1316E_MIN_IF_GAIN_POW
#define C_UV1316E_MIN_RF_GAIN_POW		-20     // dBm
#define C_UV1316E_POWER_CALIBRATE        26     // dBm
#define C_UV1316E_SIGNAL_LVL_TH          4

#define C_UV1316E_AGC_COEF				((U8)   0xBD)
#define C_UV1316E_DEMOD_INPUT_POWER		cIF_TARGET_LEVEL_0CCC

// ********************************************* //

#define C_U8_UV1316E_AGC_IF_SLP         (S8)(C_UV1316E_AGC_IF_SLP *32 )
#define C_U8_UV1316E_AGC_IF_INT         (S8)(C_UV1316E_AGC_IF_INT *64 )
#define C_U8_UV1316E_AGC_IF_MAX         (S8)(C_UV1316E_AGC_IF_MAX *256)
#define C_U8_UV1316E_AGC_IF_MIN         (S8)(C_UV1316E_AGC_IF_MIN *256)
#define C_U8_UV1316E_AGC_IF_LVL_MAX (S8)(C_UV1316E_AGC_IF_LVL_MAX *256)
#define C_U8_UV1316E_AGC_IF_LVL_MIN  (S8)(C_UV1316E_AGC_IF_LVL_MIN *256)

#define C_U8_UV1316E_MAX_IF_GAIN_SET	(S8)(C_UV1316E_MAX_IF_GAIN_SET *256)
#define C_U8_UV1316E_MIN_IF_GAIN_SET	(S8)(C_UV1316E_MIN_IF_GAIN_SET *256)
#define C_U8_UV1316E_MAX_RF_GAIN_SET	(S8)((C_UV1316E_MAX_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_UV1316E_MIN_RF_GAIN_SET	(S8)((C_UV1316E_MIN_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_UV1316E_IF_GAIN_RANGE      (S8)(C_UV1316E_MIN_IF_GAIN_POW - C_UV1316E_MAX_IF_GAIN_POW)
#define C_U8_UV1316E_RF_GAIN_RANGE      (S8)(C_UV1316E_MIN_RF_GAIN_POW - C_UV1316E_MAX_RF_GAIN_POW)

//FUNC_EXPORT VOID  UV1316E_Initialize(GEN_TUNER_CTX_T* pTunerCtx);
//FUNC_EXPORT VOID  UV1316E_SetSawBw(GEN_TUNER_CTX_T* pTunerCtx, U8 sawbw);
//FUNC_EXPORT VOID  UV1316E_SetTop(GEN_TUNER_CTX_T* pTunerCtx, U8 top_set);
//FUNC_EXPORT S16   UV1316E_SetFreq(GEN_TUNER_CTX_T* pTunerCtx, U32 Freq, U8 Mode, U16 AutoSearch);
//FUNC_EXPORT S16   UV1316E_SetIFDemod(GEN_TUNER_CTX_T* pTunerCtx, U8 SubSysIdx);
//FUNC_EXPORT CHAR* UV1316E_GetSwVer(VOID);

//extern UINT8  DVBT_GetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
///////////////////////////////////

//declare the eep api,liuqu,200081015
INT32 EEPNPTV_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
INT32 EEPNPTV_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
INT32 GPIO_Output(INT32 i4GpioNum, const INT32 *pfgSet);
// Samsung UV1316E driver
///////////////////////////////////

//----------------------------------------------------------------------------- 
/*
 *  UV1316E_GetSwVer
 *  Get driver version.
 *  @param  VOID.
 *  @retval   (CHAR *)Verion String. 
 */
//-----------------------------------------------------------------------------
CHAR* UV1316E_TunerGetVer(
    ITUNER_CTX_T *pTCtx
    )
{
	return ((CHAR*)C_UV1316E_VERSION);
}

//----------------------------------------------------------------------------- 
/*
 *  UV1316E_Initialize
 *  Tuner initialization.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @retval   VOID. 
 */
//-----------------------------------------------------------------------------
VOID UV1316E_TunerInit (
    ITUNER_CTX_T *pTCtx,
    UINT8 i2cAddr
    )
{
    GEN_TUNER_CTX_T *pTunerCtx = &pTCtx->dvbtTunerCtx;
#ifdef ROW_SET_TOP_TOEEPROM
    UINT8  u1Val;
#endif

	pTunerCtx->m_Address = C_UV1316E_LO_ADDRRESS;
	pTunerCtx->m_IF_Freq = C_UV1316E_IF_FREQUENCY;
	pTunerCtx->m_SAW_BW  = C_UV1316E_SAW_BW;
	pTunerCtx->m_TV_Spec = MOD_DVBT;
	pTunerCtx->m_s4FreqBoundUpper = C_UV1316E_FREQ_DBOUND_UPPER;
	pTunerCtx->m_s4FreqBoundLower = C_UV1316E_FREQ_DBOUND_LOWER;
	pTunerCtx->m_Ana_Top = C_UV1316E_TOP_SET_ANA;

	//add by liuqu,20081111
	pTunerCtx->m_Small_Step = C_UV1316E_CHANNEL_SCAN_JUMP_SMALL_STEP;//set small step as 250k
	pTunerCtx->m_Middle_Step_Up = C_UV1316E_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP;//set middle step as 1500k,tda9886
	pTunerCtx->m_Middle_Step_Down= C_UV1316E_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN;//set middle step as 1500k,tda9886

	// add by JC, 081215, for Signal Level display
	pTunerCtx->m_ifAgc_lvl_max = (U8)C_U8_UV1316E_AGC_IF_LVL_MAX;
	pTunerCtx->m_ifAgc_lvl_min = (U8)C_U8_UV1316E_AGC_IF_LVL_MIN;
	
	pTunerCtx->m_aucPara[0] = C_UV1316E_TOP_SET;
	pTunerCtx->m_aucPara[1] = 0;

	pTunerCtx->m_aucPara[2] = (U8)C_U8_UV1316E_AGC_IF_SLP;
	pTunerCtx->m_aucPara[3] = (U8)C_U8_UV1316E_AGC_IF_INT;
	pTunerCtx->m_aucPara[4] = (U8)C_U8_UV1316E_AGC_IF_MAX;
	pTunerCtx->m_aucPara[5] = (U8)C_U8_UV1316E_AGC_IF_MIN;

	pTunerCtx->m_aucPara[6] = C_UV1316E_AGC_COEF;
	pTunerCtx->m_aucPara[7] = C_UV1316E_DEMOD_INPUT_POWER;

	pTunerCtx->m_aucPara[ 8] = (U8)C_U8_UV1316E_MAX_IF_GAIN_SET;
	pTunerCtx->m_aucPara[ 9] = (U8)C_U8_UV1316E_MIN_IF_GAIN_SET;
	pTunerCtx->m_aucPara[10] = (U8)C_U8_UV1316E_MAX_RF_GAIN_SET;
	pTunerCtx->m_aucPara[11] = (U8)C_U8_UV1316E_MIN_RF_GAIN_SET;
	pTunerCtx->m_aucPara[12] = (U8)C_U8_UV1316E_IF_GAIN_RANGE;
	pTunerCtx->m_aucPara[13] = (U8)C_U8_UV1316E_RF_GAIN_RANGE;
	pTunerCtx->m_aucPara[14] = C_UV1316E_POWER_CALIBRATE;
	pTunerCtx->m_aucPara[21] = 0x07;

#ifdef ROW_SET_TOP_TOEEPROM
    if (EEPNPTV_Read((UINT64)EEP_eTunerAnaDemoTop, (UINT32)(void *)&u1Val, 1))
    {
        mcSHOW_DBG_MSG3(("TunerInit EEPROM Read Fail !!\n"));
        pTunerCtx->m_Ana_Top = C_UV1316E_TOP_SET_ANA;//anademo 9886 default value for ROW project
    }
    else
    {
        if ( ((INT8)u1Val>0) && (u1Val<=31) )
            pTunerCtx->m_Ana_Top= u1Val;	
        else
            pTunerCtx->m_Ana_Top = C_UV1316E_TOP_SET_ANA;//anademo 9886 default value for ROW project
    }
#endif

    pTunerCtx->m_SigLvTh = C_UV1316E_SIGNAL_LVL_TH;
}

//----------------------------------------------------------------------------- 
/*
 *  UV1316E_SetSawBw
 *  Set tuner SAW bandwidth.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @param  sawbw  SAW bandwidth setting. (SAW_BW_6M, SAW_BW_7M, SAW_BW_8M, SAW_BW_5M).
 *  @retval   VOID. 
 */
//-----------------------------------------------------------------------------
VOID UV1316E_SetSawBw(
    ITUNER_CTX_T *pTCtx,
    UINT8 sawBw
    )
{
    GEN_TUNER_CTX_T *pTunerCtx = &pTCtx->dvbtTunerCtx;

	pTunerCtx->m_SAW_BW = sawBw;
}

//----------------------------------------------------------------------------- 
/*
 *  UV1316E_SetTop
 *  Set tuner TOP (Take Over Point) setting.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @param  top_set  TOP setting.
 *  @retval   VOID. 
 */
//-----------------------------------------------------------------------------
VOID UV1316E_SetTop(
    ITUNER_CTX_T *pTCtx,
    UINT8 top_set
    )
{
    GEN_TUNER_CTX_T *pTunerCtx = &pTCtx->dvbtTunerCtx;

	pTunerCtx->m_aucPara[0] = top_set;
}


//----------------------------------------------------------------------------- 
/*
 *  UV1316E_SetFreq
 *  Set tuner frequency.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @param  freq  Frequency in kHz.
 *  @param  mode  (MOD_DVBT, MOD_PAL_BG, MOD_PAL_DK, MOD_PAL_I,	MOD_SECAM_L, MOD_SECAM_L1, MOD_NTSC_M).
 *  @param  autoSearch  
 *  @retval   (S16) 0: Success.
 *  @retval   (S16) -1: Fail. 
 */
//-----------------------------------------------------------------------------
S16 UV1316E_TunerSetFreq(
    ITUNER_CTX_T *pTCtx,
    UINT32 Freq,
    UINT8 Mode,
    UINT16 AutoSearch,
    UINT16 step
    )
{
    UCHAR uc_data;
	S16 ucTunerLockConfCnt;
	UCHAR TunerS[4];
	U32 Lo;
	U16 Ndivider, CB,BB; 
	S16 ii, jj;
	STATIC U32 Freq_pre=45000;
	
	GEN_TUNER_CTX_T *pTunerCtx = &pTCtx->dvbtTunerCtx;
	
	//char    m_TunerLogValue [100];

    pTunerCtx->m_Freq = Freq;
    pTunerCtx->m_Address = C_UV1316E_LO_ADDRRESS;
    
        
    if( Mode == MOD_SECAM_L1 )
    {
        Lo = Freq + C_UV1316E_IF_FREQUENCY_L1; // kHz
        Ndivider = (U16) ( (Lo*1000+C_UV1316E_LO_DIVIDER_STEP_ANA/2) / C_UV1316E_LO_DIVIDER_STEP_ANA);
        mcSHOW_DBG_MSG(("Freq = %d, Divider ratio = %04X PLL set freq=%d kHz\n",pTunerCtx->m_Freq, Ndivider,(Ndivider*C_UV1316E_LO_DIVIDER_STEP_ANA/1000-C_UV1316E_IF_FREQUENCY_L1)));
    }
    else
    {
        Lo = Freq + C_UV1316E_IF_FREQUENCY_ANA; // kHz
        Ndivider = (U16) ( (Lo*1000+C_UV1316E_LO_DIVIDER_STEP_ANA/2) / C_UV1316E_LO_DIVIDER_STEP_ANA);
        mcSHOW_DBG_MSG(("Freq = %d, Divider ratio = %04X PLL set freq=%d kHz\n",pTunerCtx->m_Freq, Ndivider,(Ndivider*C_UV1316E_LO_DIVIDER_STEP_ANA/1000-C_UV1316E_IF_FREQUENCY_ANA)));
    }
    
    // Conver Freq to unit of MHz
    Freq = Freq/1000;
    
    if (Freq < 44 || Freq > 872)   //the tunering range is defined in ROW HSI.
    {
        //TRACE(("Out of range for LO!\n"));
        mcSHOW_DRVERR_MSG(("Out of range for LO!\n"));
        return (+1);
    }
    
	CB = 0xC6;	//CP=100UA,, Lo step 62.5kHz
    if (Freq <= 157)       
	{
		BB = 0x01; 
	}
	else if (Freq <= 443)  
	{        
		BB = 0x02;  
	}
	else                 
    {
		BB = 0x04;
	}

    	
    // LO settings
    if(pTunerCtx->m_Freq >= Freq_pre)
    {
        TunerS[0] = (UCHAR) 0x7F & (Ndivider >> 8);
        TunerS[1] = (UCHAR) 0xFF & Ndivider;
        TunerS[2] = (UCHAR) CB;  
        TunerS[3] = (UCHAR) BB;
    }
    else    /* - If fw < fc, use telegram as Start ->ADB->ACK ->CB ->ACK  ->BB  ->ACK ->DB1 ->ACK ->DB2 ->ACK ->Stop  */
   	{
        TunerS[0] = (UCHAR) CB;  
        TunerS[1] = (UCHAR) BB;
        TunerS[2] = (UCHAR) 0x7F & (Ndivider >> 8);
        TunerS[3] = (UCHAR) 0xFF & Ndivider;       
    }
    Freq_pre = pTunerCtx->m_Freq;   //record the last value     

    for (jj=0; jj<2; jj++)
    {

//  send this data 
    //if (RegSetRegisters(pTunerCtx->m_Address, TunerS[0], 4, &TunerS[1]) < 0)
//	    if (RegSetRegistersTuner(pTunerCtx->m_Address, 4, &TunerS[0]) < 0)
        if (ICtrlBus_I2cTunerWrite(pTunerCtx->m_Address, TunerS, 4) < 0)
        {
            //TRACE(("\nTunerSet() failed!\n"));
            return (-1);
        }
    	
	//marked by liuqu,20081017,for channel scan time improvement.
	//	mcDELAY(10);
		// wait PLL lock
	//#ifndef C_DEMO_PROGRAM
		ucTunerLockConfCnt = 0;
		// Ken, 080704, for channel scan time improvement
		//for (ii=0; ii<10; ii++)
		for (ii=0; ii<20; ii++)
		//for (ii=0; ii<ccMAX_WAIT_TUNER_TIME; ii++)
		{
			//RegGetRegisters(pTunerCtx->m_Address, (pTunerCtx->m_Address) | (0x01), 1, &uc_data);
			// Charles_060425, test I2C
			//I2CRead(pTunerCtx->m_Address, 1, &uc_data);
		  ICtrlBus_I2cTunerRead(pTunerCtx->m_Address, &uc_data, 1);
			if (mcGET_FIELD(uc_data, 0x40, 6))  // if PLL lock
				ucTunerLockConfCnt++;
			else
				ucTunerLockConfCnt = 0;
	             	                
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
		}

	}
//#else
//	mcDELAY(200);
//#endif
	    
	pTunerCtx->m_aucPara[1]  = TunerS[0];	//send register value to UI, LC 070102
	pTunerCtx->m_aucPara[15] = TunerS[1];
	pTunerCtx->m_aucPara[16] = TunerS[2];
	pTunerCtx->m_aucPara[17] = TunerS[3];
	pTunerCtx->m_aucPara[18] = TunerS[3];
	pTunerCtx->m_aucPara[19] = uc_data;
    
	
   //marked by liuqu,20081017,for channel scan time improvement.
  //  mcDELAY(10);	// add 10ms delay before bypass is turn off, LC 070613

	return (0);
}

//----------------------------------------------------------------------------- 
/*
 *  UV1316E_SwitchLNA
 *  LNA switch.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @retval   (S16) 0: Success.
 *  @retval   (S16) -1: Fail. 
 */
//-----------------------------------------------------------------------------
S16 UV1316E_SwitchLNA(
    ITUNER_CTX_T *pTCtx
    )
{   		
	return (0);
}

//----------------------------------------------------------------------------- 
/*
 *  UV1316E_SetIFDemod
 *  Set IF demod IC.
 *  @param  pTunerCtx  Pointer of tuner context.
 *  @param  subSysIdx  (MOD_DVBT, MOD_PAL_BG, MOD_PAL_DK, MOD_PAL_I,	MOD_SECAM_L, MOD_SECAM_L1, MOD_NTSC_M).
 *  @retval   (S16) 0: Success.
 *  @retval   (S16) -1: Fail. 
 */
//-----------------------------------------------------------------------------
S16   UV1316E_SetIFDemod(
    ITUNER_CTX_T *pTCtx,
    UINT8 SubSysIdx
    )
{
    GEN_TUNER_CTX_T *pTunerCtx = &pTCtx->dvbtTunerCtx;

#ifdef ROW_SET_TOP_TOEEPROM
    UINT8  u1Val;
#endif

    UCHAR TunerS[5];
    INT32 i4GPIOValue = 1;
	TunerS[0] = 0x86;	// IF IC slave address
	TunerS[1] = 0x00;	// IF IC sub address
	
    if( !pTunerCtx->m_Ana_Top)
    {
        mcSHOW_DRVAPI_MSG(("m_Ana_Top==0!!!manual change!\n"));
#ifdef ROW_SET_TOP_TOEEPROM
        if (EEPNPTV_Read((UINT64)EEP_eTunerAnaDemoTop, (UINT32)(void *)&u1Val, 1))
        {
            mcSHOW_DBG_MSG3(("TunerInit EEPROM Read Fail !!\n"));
            pTunerCtx->m_Ana_Top = C_UV1316E_TOP_SET_ANA;//anademo 9886 default value for ROW project
        }
        else
        {
            if ( ((INT8)u1Val>0) && (u1Val<=31) )
                pTunerCtx->m_Ana_Top= u1Val;	
            else
                pTunerCtx->m_Ana_Top = C_UV1316E_TOP_SET_ANA;//anademo 9886 default value for ROW project
        }
#else
        pTunerCtx->m_Ana_Top =C_UV1316E_TOP_SET_ANA;
#endif
    }	
	
	switch (SubSysIdx)
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

	//set GPIO22 TO High when BG
        i4GPIOValue =1;
        GPIO_Output(22, &i4GPIOValue);
		break;
	case MOD_PAL_DK:
		TunerS[2] = 0x56;	// B-Data
		TunerS[3] = 0x70;	// C-Data
		TunerS[4] = 0x0B;	// E-Data
		//set GPIO22 TO High when BG
	     i4GPIOValue =1;
	    GPIO_Output(22, &i4GPIOValue);
		break;
	case MOD_PAL_I:
		TunerS[2] = 0x56;	// B-Data
		TunerS[3] = 0x70;	// C-Data
		TunerS[4] = 0x0A;	// E-Data
		//set GPIO22 TO High when BG
	    i4GPIOValue =1;
	    GPIO_Output(22, &i4GPIOValue);
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
	default:  //PAL M
		TunerS[2] = 0x56;	// B-Data
		TunerS[3] = 0x10;	// C-Data
		TunerS[4] = 0x08;	// E-Data
    	//set GPIO22 TO LOW when PAL M
    	i4GPIOValue =0;
    	GPIO_Output(22, &i4GPIOValue);
		break;
	}

	
	//set 9886 TOP
     TunerS[3] &= 0xE0;
	 TunerS[3] |= (0x1F & pTunerCtx->m_Ana_Top); //analog TOP (b4-b0)
	 mcSHOW_DRVAPI_MSG(("UV1316E_SETIFDEMOD TOP =%d\n",pTunerCtx->m_Ana_Top));
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

