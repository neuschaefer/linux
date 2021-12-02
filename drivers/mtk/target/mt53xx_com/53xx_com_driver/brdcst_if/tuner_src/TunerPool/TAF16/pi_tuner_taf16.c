/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2008, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: pi_tuner_taf16.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_taf16.c
 *  Tuner control for TCL TAF16
 */


#include "tuner_interface_if.h"
#include "PI_Def.h"
#include "ctrl_bus.h"
#include "pi_demod_atd.h"
#include "eq_script_taf16.h"

// *** Release Note ***
// Ver     Date         Description
// 1.00    2009.03.25   file established
#define C_TAF16_VERSION				"TAF16 2010-01-01\n 2011-04-07\n v1.0"

#define C_TAF16_PLL_TIMEOUT_CNT     10
#define C_TAF16_I2C_CLK_DIV         0x100
#define C_TAF16_PLL_LOCK_BIT        6
 
#define C_TAF16_LO_ADDRRESS           ((U16)  0xC0)
#define C_TAF16_IF_FREQUENCY_ANA      ((U16)  38000)  /* kHz */
#define C_TAF16_IF_FREQUENCY_L1       ((U16)  33875)  /* kHz */
#define C_TAF16_LO_DIVIDER_STEP_ANA   ((U16)  62500) //50000)  /* Hz */


STATIC UINT8* pTaf16EqNormal[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pTaf16EqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pTaf16EqStrongChroma[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};


///////////////////////////////////
// ALPS TAF16 driver
///////////////////////////////////

CHAR* TAF16_GetSwVer(void)
{
	return ((CHAR*)C_TAF16_VERSION);
}


VOID TAF16_TunerInit(ITUNER_CTX_T *pTCtx)
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
//    SPECIFIC_MEMBER_EU_CTX *psTunerSpecificCtx = &(psTunerCtx->specific_member.eu_ctx);

    psTunerCtx->I2cAddress = C_TAF16_LO_ADDRRESS;
    psTunerCtx->u2IF_Freq_A = C_TAF16_IF_FREQUENCY_ANA;
    psTunerCtx->u4RF_Freq = 0;
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_SENS|MASK_PATCH_CCI); 
    psTunerCtx->fgLnaOn = FALSE;
    psTunerCtx->fgSunBoard = FALSE;
    psTunerCtx->fgRFAGC = TRUE;
// AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x26;
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0xEA;
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B;

    // init EQ script for mtk board
    #if ((defined CC_MT5365) || (defined CC_MT5395))
    pTaf16EqNormal[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQTaf16BG;
    pTaf16EqNormal[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQTaf16DK;
    pTaf16EqNormal[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]   = EQTaf16I;
    pTaf16EqNormal[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN] = EQTaf16L;
    pTaf16EqNormal[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQTaf16L1;
    pTaf16EqNormal[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQTaf16M;
    pTaf16EqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQTaf16BG;
    pTaf16EqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQTaf16DK;
    pTaf16EqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]   = EQTaf16I;
    pTaf16EqWeak[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQTaf16M;
    pTaf16EqStrongChroma[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQTaf16BG;
    #else
    pTaf16EqNormal[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQTaf16BG;
    pTaf16EqNormal[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQTaf16DK;
    pTaf16EqNormal[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]   = EQTaf16I;
    pTaf16EqNormal[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN] = EQTaf16L;
    pTaf16EqNormal[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQTaf16L;
    pTaf16EqNormal[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQTaf16M;
    pTaf16EqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQTaf16BGWeak;
    pTaf16EqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQTaf16DKWeak;
    pTaf16EqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]   = EQTaf16IWeak;
    pTaf16EqWeak[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQTaf16MWeak;
    pTaf16EqStrongChroma[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQTaf16BGStrongChroma;
    #endif
    
    mcSHOW_USER_MSG(("Tuner TAF16 for mtk board init done!\n"));
}

S16 TAF16_TunerSetFreq(ITUNER_CTX_T *pTCtx, PARAM_SETFREQ_T* param)
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
        Lo = Freq + C_TAF16_IF_FREQUENCY_L1; // kHz
        Ndivider = (U16) ( (Lo*1000+C_TAF16_LO_DIVIDER_STEP_ANA/2) / C_TAF16_LO_DIVIDER_STEP_ANA);
        mcSHOW_DBG_MSG(("Freq = %dkHz, Divider ratio = %04X, PLL set freq = %d kHz\n",
                        pTCtx->u4RF_Freq, 
                        Ndivider,
                        (Ndivider*C_TAF16_LO_DIVIDER_STEP_ANA/1000-C_TAF16_IF_FREQUENCY_L1)));
    }
    else
    {
        Lo = Freq + C_TAF16_IF_FREQUENCY_ANA; // kHz
        Ndivider = (U16) ( (Lo*1000+C_TAF16_LO_DIVIDER_STEP_ANA/2) / C_TAF16_LO_DIVIDER_STEP_ANA);
        mcSHOW_DBG_MSG(("Freq = %dkHz, Divider ratio = %04X, PLL set freq = %d kHz\n",
                        pTCtx->u4RF_Freq,
                        Ndivider,
                        (Ndivider*C_TAF16_LO_DIVIDER_STEP_ANA/1000-C_TAF16_IF_FREQUENCY_ANA)));
    }	        
    pTCtx->u2IF_Freq_A = Lo - Freq;
    
   Freq = Freq/1000;
	    
	    if (Freq < 45 || Freq > 875)
	    {
	       // LogS("Out of range for LO!\n");
	        //return (+1);
	    }

           CB1 = 0xCE; //CP = 60uA
	    if(Freq<148.25)
	    {
               BB = 0x01;
	    }
	    else if(Freq<428.25)
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
        if (ICtrlBus_I2cTunerWrite(C_TAF16_I2C_CLK_DIV, pTCtx->I2cAddress, TunerS, 4))
        {
            mcSHOW_USER_MSG(("TunerWrite PLL failed!\n"));
            return (+1);
        }
        else
        {
            mcSHOW_DBG_MSG(("Tuner TAF16 Write PLL success!\n"));
        }
	
    		for (s1PllPollCnt=0; s1PllPollCnt<C_TAF16_PLL_TIMEOUT_CNT; s1PllPollCnt++)
    		{
            mcDELAY_MS(10);
            
            if (ICtrlBus_I2cTunerRead(C_TAF16_I2C_CLK_DIV, pTCtx->I2cAddress, &ucPllValue, 1))
            {
                mcSHOW_USER_MSG(("Tuner read failed!\n"));
                return (+1);
            }			
            //mcDELAY_MS(10);			
            // check if PLL lock
            if (mcCHK_BIT1(ucPllValue, C_TAF16_PLL_LOCK_BIT)) {
                break;
            }											
    		}

        if(s1PllPollCnt < C_TAF16_PLL_TIMEOUT_CNT)
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


INT16 TAF16_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){
         
    switch(eOperation)
    {
    case itGetVer:
        {
            CHAR ** RetStr = (CHAR **)pInOutVal;
            *RetStr = TAF16_GetSwVer();
        }
        break;
    case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pTaf16EqNormal[SetVal];
        break;
    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pTaf16EqWeak[SetVal];
        break;
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = pTaf16EqStrongChroma[SetVal];
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
			
        mcSHOW_DBG_MSG(("TAF16 TOP Setting\n"));
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
    default:
        return ITUNER_NOT_SUPPORTED;
    }

    return ITUNER_OK;
}
