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
 * $RCSfile: pi_tuner_teqe3.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_teqe3.c
 *  Tuner control for ALPS TEQE3
 */


#include "tuner_interface_if.h"
#include "PI_Def.h"
#include "ctrl_bus.h"
#include "pi_demod_atd.h"
#include "x_gpio.h"

#include "eq_script_teqe3_5388_sun.h"
#include "eq_script_teqe3_5388.h"

// *** Release Note ***
// Ver     Date         Description
// 1.00    2009.03.25   file established
#define C_TEQE3_VERSION				"TEQE3 2010-01-01\n 2011-04-09\n v1.0"

#define C_TEQE3_PLL_TIMEOUT_CNT     10
#define C_TEQE3_I2C_CLK_DIV         0x100
#define C_TEQE3_PLL_LOCK_BIT        6

#define C_TEQE3_TOP_SET               ((U8)   0x00) 
#define C_TEQE3_SAW_BW                SAW_BW_8M 
#define C_TEQE3_LNA_set                ((U8) 0x01) 
#define C_TEQE3_LO_ADDRRESS           ((U16)  0xC2)
#define C_TEQE3_IF_FREQUENCY          ((U16)  36167)  /* kHz */
#define C_TEQE3_IF_FREQUENCY_ANA      ((U16)  38000)  /* kHz */
#define C_TEQE3_IF_FREQUENCY_L1       ((U16)  32900)  /* kHz */
#define C_TEQE3_LO_DIVIDER_STEP       ((U32) 62500)  /* Hz */
#define C_TEQE3_LO_DIVIDER_STEP_ANA   ((U16)  31250) //50000)  /* Hz */

#define C_TEQE3_CHANNEL_SCAN_JUMP_SMALL_STEP ((U16) 250)
#define C_TEQE3_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP ((U16) 1500)
#define C_TEQE3_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN ((U16) 500)

#define GPIO_SAW_SWITCH     21
#define GPIO_SAW_7M    0
#define GPIO_SAW_8M    1
#define GPIO_6M_TRAP_SWITCH   1
#define GPIO_6M_TRAP_OFF      1
#define GPIO_6M_TRAP_ON       0

STATIC UINT8* pTeqe3EqNormal[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pTeqe3EqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pTeqe3EqStrongChroma[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};

static VOID vSAW2PinGpioCtrl(BOOL SAW)
{
UINT32 GPIONum=GPIO_SAW_SWITCH;
  if (SAW == GPIO_SAW_8M)
        {
            mcSHOW_DBG_MSG(("Set SAW = 8MHz (GPIO%d)->1\n", GPIONum));
            GPIO_SetOut(GPIONum, GPIO_SAW_8M); // Set SAW=8MHz          
        }
  else
        {
            mcSHOW_DBG_MSG(("Set SAW = 7MHz (GPIO%d)->0\n", GPIONum));
            GPIO_SetOut(GPIONum, GPIO_SAW_7M); // Set SAW=7MHz 
        }		
}

static VOID vTrap2PinGpioCtrl(BOOL Trap)
{
UINT32 GPIONum=GPIO_6M_TRAP_SWITCH;
  if (Trap == GPIO_6M_TRAP_OFF)
        {
            mcSHOW_DBG_MSG(("Set 6M Trap OFF (GPIO%d)->1\n", GPIONum));
            GPIO_SetOut(GPIONum, GPIO_6M_TRAP_OFF); // Set 6M Trap OFF          
        }
  else
        {
            mcSHOW_DBG_MSG(("Set 6M Trap ON (GPIO%d)->0\n", GPIONum));
            GPIO_SetOut(GPIONum, GPIO_6M_TRAP_ON); // Set 6M Trap ON 
        }		
}

///////////////////////////////////
// ALPS TEQE3 driver
///////////////////////////////////

CHAR* TEQE3_GetSwVer(void)
{
	return ((CHAR*)C_TEQE3_VERSION);
}


VOID TEQE3_TunerInit(ITUNER_CTX_T *pTCtx)
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
//    SPECIFIC_MEMBER_EU_CTX *psTunerSpecificCtx = &(psTunerCtx->specific_member.eu_ctx);

    psTunerCtx->I2cAddress = C_TEQE3_LO_ADDRRESS;
	psTunerCtx->u2IF_Freq = C_TEQE3_IF_FREQUENCY_ANA;
	psTunerCtx->u4RF_Freq = 0;
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_DRO|MASK_PATCH_SENS|MASK_PATCH_STCHR|MASK_PATCH_SBEAT);
    psTunerCtx->fgLnaOn = FALSE;
    psTunerCtx->fgSunBoard = FALSE;
    psTunerCtx->fgRFAGC = TRUE;
// AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x26;
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0xEA;
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B;

    // init EQ script for mtk board
    pTeqe3EqNormal[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQTeqe3BG;
    pTeqe3EqNormal[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQTeqe3DK;
    pTeqe3EqNormal[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]   = EQTeqe3I;
    pTeqe3EqNormal[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN] = EQTeqe3L;
    pTeqe3EqNormal[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQTeqe3L;
    pTeqe3EqNormal[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQTeqe3M;
    pTeqe3EqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQTeqe3BGWeak;
    pTeqe3EqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQTeqe3DKWeak;
    pTeqe3EqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]   = EQTeqe3IWeak;
    pTeqe3EqWeak[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQTeqe3MWeak;
    pTeqe3EqStrongChroma[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQTeqe3BGStrongChroma;
    
    mcSHOW_USER_MSG(("Tuner TEQE3 for mtk board init done!\n"));
}

VOID TEQE3_SunBoard_TunerInit(ITUNER_CTX_T *pTCtx)
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
//    SPECIFIC_MEMBER_EU_CTX *psTunerSpecificCtx = &(psTunerCtx->specific_member.eu_ctx);

    psTunerCtx->I2cAddress = C_TEQE3_LO_ADDRRESS;
	psTunerCtx->u2IF_Freq = C_TEQE3_IF_FREQUENCY_ANA;
	psTunerCtx->u4RF_Freq = 0;
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_DRO|MASK_PATCH_SENS|MASK_PATCH_STCHR|MASK_PATCH_SBEAT);
    psTunerCtx->fgLnaOn = FALSE;
    psTunerCtx->fgSunBoard = TRUE;
    psTunerCtx->fgRFAGC = TRUE;
    // AGC para for sun board
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x26; //0x2A;
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0x03; //0xEA
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B; //0x03;

    // init EQ script for sun board
    pTeqe3EqNormal[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQTeqe3BG_SunBoard;
    pTeqe3EqNormal[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQTeqe3DK_SunBoard;
    pTeqe3EqNormal[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]   = EQTeqe3I_SunBoard;
    pTeqe3EqNormal[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN] = EQTeqe3L_SunBoard;
    pTeqe3EqNormal[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQTeqe3L_SunBoard;
    pTeqe3EqNormal[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQTeqe3M_SunBoard;
    pTeqe3EqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQTeqe3BGWeak_SunBoard;
    pTeqe3EqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQTeqe3DKWeak_SunBoard;
    pTeqe3EqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]   = EQTeqe3IWeak_SunBoard;
    pTeqe3EqWeak[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQTeqe3MWeak_SunBoard;
    pTeqe3EqStrongChroma[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQTeqe3BGStrongChroma_SunBoard;

    mcSHOW_USER_MSG(("Tuner TEQE3 for SunBoard init done!\n"));
}

S16 TEQE3_TunerSetFreq(ITUNER_CTX_T *pTCtx, PARAM_SETFREQ_T* param)
{
	UCHAR TunerS[4];
    U16   Ndivider;
    UCHAR CB1, BB; 
    UCHAR ucPllValue;
	U32 Lo;
    S8 s1PllPollCnt;
	S8 jj;

    U32 Freq = param->Freq; // transfer to video freq 
    //U16 AutoSearch = param->fgAutoSearch;
    U8  Mode = param->Modulation;
	
    pTCtx->u4RF_Freq = Freq;

    if( Mode == MOD_SECAM_L1 )
    {
        Lo = Freq + C_TEQE3_IF_FREQUENCY_L1; // kHz
        Ndivider = (U16) ( (Lo*1000+C_TEQE3_LO_DIVIDER_STEP_ANA/2) / C_TEQE3_LO_DIVIDER_STEP_ANA);
        mcSHOW_DBG_MSG(("Freq = %dkHz, Divider ratio = %04X, PLL set freq = %d kHz\n",
                        pTCtx->u4RF_Freq, 
                        Ndivider,
                        (Ndivider*C_TEQE3_LO_DIVIDER_STEP_ANA/1000-C_TEQE3_IF_FREQUENCY_L1)));
    }
    else
    {
        Lo = Freq + C_TEQE3_IF_FREQUENCY_ANA; // kHz
        Ndivider = (U16) ( (Lo*1000+C_TEQE3_LO_DIVIDER_STEP_ANA/2) / C_TEQE3_LO_DIVIDER_STEP_ANA);
        mcSHOW_DBG_MSG(("Freq = %dkHz, Divider ratio = %04X, PLL set freq = %d kHz\n",
                        pTCtx->u4RF_Freq,
                        Ndivider,
                        (Ndivider*C_TEQE3_LO_DIVIDER_STEP_ANA/1000-C_TEQE3_IF_FREQUENCY_ANA)));
    }	
    
    
    Freq = Freq/1000;
    
    //if (Freq < 45 || Freq > 877)
    //{
        //mcSHOW_DBG_MSG(("Out of range for LO!\n"));
        //return (+1);
    //}
    
    if(pTCtx->fgSunBoard)
    {
        if(param->fgAutoSearch)
        {
            if(Mode == MOD_NTSC_M)
                vSAW2PinGpioCtrl(GPIO_SAW_7M);
            else
                vSAW2PinGpioCtrl(GPIO_SAW_8M);
        }
        else
        {        
            if(((Mode == MOD_PAL_BG) && (Freq <= 300)) || (Mode == MOD_NTSC_M))
                vSAW2PinGpioCtrl(GPIO_SAW_7M);
            else
    	        vSAW2PinGpioCtrl(GPIO_SAW_8M);
        }
        
        if(Mode == MOD_NTSC_M)
            vTrap2PinGpioCtrl(GPIO_6M_TRAP_ON);
        else
            vTrap2PinGpioCtrl(GPIO_6M_TRAP_OFF);
    }    
    
    //No need to turn ON LNA based on autosearch flag
    //if(AutoSearch) {    	
    //    CB1 = 0x88; //0x8E: 62.5K, 0x88: 50K
    //}
    //else {
        if (pTCtx->fgLnaOn) {
            CB1 = 0xCA; //31.25K, CP=1
        }
        else {
            CB1 = 0xC2; //31.25K, CP=1
        }
    //}          
    
    if (Freq <= 122) {
        BB = 0x01;	//8M		
    }
    else if (Freq  <= 366) {        
        BB = 0x02; 			
    }		
    else {
        BB = 0x08; 
    }
	    
    // LO settings
    TunerS[0] = (UCHAR) 0x7F & (Ndivider >> 8);
    TunerS[1] = (UCHAR) 0xFF & Ndivider;
    TunerS[2] = (UCHAR) CB1;  
    TunerS[3] = (UCHAR) BB;
		
    for (jj=0; jj<2; jj++)
    {
        if (ICtrlBus_I2cTunerWrite(C_TEQE3_I2C_CLK_DIV, pTCtx->I2cAddress, TunerS, 4))
        {
            mcSHOW_USER_MSG(("TunerWrite PLL failed!\n"));
            return (+1);
        }
	
		for (s1PllPollCnt=0; s1PllPollCnt<C_TEQE3_PLL_TIMEOUT_CNT; s1PllPollCnt++)
		{
            mcDELAY_MS(10);

            if (ICtrlBus_I2cTunerRead(C_TEQE3_I2C_CLK_DIV, pTCtx->I2cAddress, &ucPllValue, 1))
            {
                mcSHOW_USER_MSG(("Tuner read failed!\n"));
                return (+1);
            }			
            //mcDELAY_MS(10);			
            // check if PLL lock
            if (mcCHK_BIT1(ucPllValue, C_TEQE3_PLL_LOCK_BIT)) {
                break;
            }											
		}

		if(s1PllPollCnt < C_TEQE3_PLL_TIMEOUT_CNT)
		{
			mcSHOW_DRVAPI_MSG(("Freq = %d MHz, the %d-th time setting PLL success, cnt = %d\n" , Freq, jj+1, s1PllPollCnt));	
			break;
		}
		else
		{
			mcSHOW_DRVAPI_MSG(("Freq = %d MHz, the %d-th time setting PLL fail\n" , Freq, jj+1));		
		}
	}
    
    return (0);
}


INT16 TEQE3_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){
         
    switch(eOperation)
    {
    case itGetVer:
        {
            CHAR ** RetStr = (CHAR **)pInOutVal;
            *RetStr = TEQE3_GetSwVer();
        }
        break;
    case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pTeqe3EqNormal[SetVal];
        break;
    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pTeqe3EqWeak[SetVal];
        break;
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = pTeqe3EqStrongChroma[SetVal];
        break;
    case itSetLNA:
        pTCtx->fgLnaOn = SetVal;
        break;
    case itGetLNA :
    {
        BOOL * Retval=(BOOL *)pInOutVal;
        * Retval  = pTCtx->fgLnaOn;	
    }
        break;        
    default:
        return ITUNER_NOT_SUPPORTED;
    }

    return ITUNER_OK;
}
