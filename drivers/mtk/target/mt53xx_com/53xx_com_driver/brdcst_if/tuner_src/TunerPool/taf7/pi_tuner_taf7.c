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
 * $RCSfile: pi_tuner_TAF7.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_TAF7.c
 *  Tuner control for TCL TAF7
 */


#include "tuner_interface_if.h"
#include "PI_Def.h"
#include "ctrl_bus.h"
#include "pi_demod_atd.h"
#include "eq_script_taf7.h"

// *** Release Note ***
// Ver     Date         Description
// 1.00    2009.03.25   file established
#define C_TAF7_VERSION				"TAF7 2010-01-01\n 2011-04-07\n v1.0"

#define C_TAF7_PLL_TIMEOUT_CNT     10
#define C_TAF7_I2C_CLK_DIV         0x100
#define C_TAF7_PLL_LOCK_BIT        6
 
#define C_TAF7_LO_ADDRRESS           ((U16)  0xC0)
#define C_TAF7_IF_FREQUENCY_ANA      ((U16)  38000)  /* kHz */
#define C_TAF7_IF_FREQUENCY_L1       ((U16)  33875)  /* kHz */
#define C_TAF7_LO_DIVIDER_STEP_ANA   ((U16)  62500) //50000)  /* Hz */


STATIC UINT8* pTAF7EqNormal[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pTAF7EqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pTAF7EqStrongChroma[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC ATD_TUNER_REL_REG_T arrAtdRelReg[]={
 
 {0x2d4,0x04},//PRA target level[11:4] upper boundary
 {0x2d5,0xe0},
 {0x2d6,0xe0},//IF_BIAS1
 {0x2d7,0xEc},//IF_BIAS2
 {0x2d8,0xd0},//IF_SLOPE1
 {0x2d9,0xd0},//IF_SLOPE2
 {0x2da,0x7F},
 {0x2db,0x7f},
 {0x2dc,0x80},
 {0x2dd,0x80},
 {0x2de,0x80},
 {0x2ca,0x77},
 {0x2cb,0x03},
 {0x0,0x0}//end flag
 };




///////////////////////////////////
// ALPS TAF7 driver
///////////////////////////////////

CHAR* TAF7_GetSwVer(void)
{
	return ((CHAR*)C_TAF7_VERSION);
}


VOID TAF7_TunerInit(ITUNER_CTX_T *pTCtx)
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
//    SPECIFIC_MEMBER_EU_CTX *psTunerSpecificCtx = &(psTunerCtx->specific_member.eu_ctx);

    psTunerCtx->I2cAddress = C_TAF7_LO_ADDRRESS;
    psTunerCtx->u2IF_Freq_A = C_TAF7_IF_FREQUENCY_ANA;
    psTunerCtx->u4RF_Freq = 0;
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_SENS|MASK_PATCH_CCI); 
    psTunerCtx->fgLnaOn = FALSE;
    psTunerCtx->fgSunBoard = FALSE;
    psTunerCtx->fgRFAGC = TRUE;
	 psTunerCtx->fgRFTuner = TRUE;

    // iven 20110706, add for polarity if setting for NA reference
    psTunerCtx->bSpecPolarity = 0;
    // iven 20110706, add for legacy tuner as default fixed PGA gain index. Don care for Sawless
    psTunerCtx->uSLD_DefaultPGAIndex = 0;		
	 
// AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x26;
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0xEA;
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B;

    // init EQ script for mtk board
    #if ((defined CC_MT5365) || (defined CC_MT5395)||(defined CC_MT5396))
    pTAF7EqNormal[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQTAF7BG;
    pTAF7EqNormal[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQTAF7DK;
    pTAF7EqNormal[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]   = EQTAF7I;
    pTAF7EqNormal[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN] = EQTAF7L;
    pTAF7EqNormal[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQTAF7L1;
    pTAF7EqNormal[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQTAF7M;
    pTAF7EqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQTAF7BG;
    pTAF7EqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQTAF7DK;
    pTAF7EqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]   = EQTAF7I;
    pTAF7EqWeak[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQTAF7M;
    pTAF7EqStrongChroma[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQTAF7BG;
    #else
    pTAF7EqNormal[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQTAF7BG;
    pTAF7EqNormal[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQTAF7DK;
    pTAF7EqNormal[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]   = EQTAF7I;
    pTAF7EqNormal[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN] = EQTAF7L;
    pTAF7EqNormal[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQTAF7L;
    pTAF7EqNormal[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQTAF7M;
    pTAF7EqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQTAF7BGWeak;
    pTAF7EqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQTAF7DKWeak;
    pTAF7EqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]   = EQTAF7IWeak;
    pTAF7EqWeak[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQTAF7MWeak;
    pTAF7EqStrongChroma[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQTAF7BGStrongChroma;
    #endif
    
    mcSHOW_USER_MSG(("Tuner TAF7 for mtk board init done!\n"));
}

S16 TAF7_TunerSetFreq(ITUNER_CTX_T *pTCtx, PARAM_SETFREQ_T* param)
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
    
	mcSHOW_DBG_MSG((C_TAF7_VERSION "SetFreq  %d KHz\n",Freq));
	
    if( Mode == MOD_SECAM_L1 )
    {
        Lo = Freq + C_TAF7_IF_FREQUENCY_L1; // kHz
        Ndivider = (U16) ( (Lo*1000+C_TAF7_LO_DIVIDER_STEP_ANA/2) / C_TAF7_LO_DIVIDER_STEP_ANA);
        mcSHOW_DBG_MSG(("Freq = %dkHz, Divider ratio = %04X, PLL set freq = %d kHz\n",
                        pTCtx->u4RF_Freq, 
                        Ndivider,
                        (Ndivider*C_TAF7_LO_DIVIDER_STEP_ANA/1000-C_TAF7_IF_FREQUENCY_L1)));
    }
    else
    {
        Lo = Freq + C_TAF7_IF_FREQUENCY_ANA; // kHz
        Ndivider = (U16) ( (Lo*1000+C_TAF7_LO_DIVIDER_STEP_ANA/2) / C_TAF7_LO_DIVIDER_STEP_ANA);
        mcSHOW_DBG_MSG(("Freq = %dkHz, Divider ratio = %04X, PLL set freq = %d kHz\n",
                        pTCtx->u4RF_Freq,
                        Ndivider,
                        (Ndivider*C_TAF7_LO_DIVIDER_STEP_ANA/1000-C_TAF7_IF_FREQUENCY_ANA)));
    }	        
    pTCtx->u2IF_Freq_A = Lo - Freq;
    
   //Freq = Freq/1000;
	    
	    if (Freq < 45000 || Freq > 875000)
	    {
	       // LogS("Out of range for LO!\n");
	        //return (+1);
	    }

           CB1 = 0x86; //CP = 60uA
	    if(Freq<147250)
	    {
               BB = 0x01;
	    }
	    else if(Freq<428000)
	    {
               BB = 0x02;
	    }
	    else
	    {
	        BB = 0x08;
	    }
	    
    // LO settings
    TunerS[0] = (UCHAR) 0x7F & (Ndivider >> 8);
    TunerS[1] = (UCHAR) 0xFF & Ndivider;
    TunerS[2] = (UCHAR) CB1;  
    TunerS[3] = (UCHAR) BB;




	
		
    for (jj=0; jj<2; jj++)
    {
        if (ICtrlBus_I2cTunerWrite(C_TAF7_I2C_CLK_DIV, pTCtx->I2cAddress, TunerS, 4))
        {
            mcSHOW_USER_MSG(("TunerWrite PLL failed!\n"));
            return (+1);
        }
        else
        {
            mcSHOW_DBG_MSG(("Tuner TAF7 Write PLL success!\n"));
        }
	
    		for (s1PllPollCnt=0; s1PllPollCnt<C_TAF7_PLL_TIMEOUT_CNT; s1PllPollCnt++)
    		{
            mcDELAY_MS(10);
            
            if (ICtrlBus_I2cTunerRead(C_TAF7_I2C_CLK_DIV, pTCtx->I2cAddress, &ucPllValue, 1))
            {
                mcSHOW_USER_MSG(("Tuner read failed!\n"));
                return (+1);
            }			
            //mcDELAY_MS(10);			
            // check if PLL lock
            if (mcCHK_BIT1(ucPllValue, C_TAF7_PLL_LOCK_BIT)) {
                break;
            }											
    		}

        if(s1PllPollCnt < C_TAF7_PLL_TIMEOUT_CNT)
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


INT16 TAF7_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){
         
    switch(eOperation)
    {
    case itGetVer:
        {
            CHAR ** RetStr = (CHAR **)pInOutVal;
            *RetStr = TAF7_GetSwVer();
        }
        break;
    case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pTAF7EqNormal[SetVal];
        break;
    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pTAF7EqWeak[SetVal];
        break;
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = pTAF7EqStrongChroma[SetVal];
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
    case itSetIfMin:
        {

        UINT8 subSysId = SetVal;
        UINT8 *ifMinPtr = (UINT8*)pInOutVal;
			
        mcSHOW_DBG_MSG(("TAF7 TOP Setting\n"));
        switch(subSysId){
    	    case MOD_SECAM_L:
            case MOD_SECAM_L1:
                *ifMinPtr = 0xF0;
    		    break;
    	    default:
                *ifMinPtr = 0x80;
                break;
        }
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
