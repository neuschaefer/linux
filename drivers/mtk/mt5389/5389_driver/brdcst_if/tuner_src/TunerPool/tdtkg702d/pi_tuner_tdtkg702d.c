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
 * $RCSfile: pi_tuner_TDTKG702D.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_TDTKG702D.c
 *  Tuner control for TCL TDTKG702D
 */


#include "tuner_interface_if.h"
#include "PI_Def.h"
#include "ctrl_bus.h"
#include "pi_demod_atd.h"
#include "eq_script_tdtkg702d.h"

// *** Release Note ***
// Ver     Date         Description
// 1.00    2009.03.25   file established
#define C_TDTKG702D_VERSION				"TDTKG702D 2010-01-01\n 2011-04-09\n v1.0"

#define C_TDTKG702D_PLL_TIMEOUT_CNT     10
#define C_TDTKG702D_I2C_CLK_DIV         0x100
#define C_TDTKG702D_PLL_LOCK_BIT        6
 
#define C_TDTKG702D_LO_ADDRRESS           ((U16)  0xC2)
#define C_TDTKG702D_IF_FREQUENCY_ANA      ((U16)  38000)  /* kHz */
#define C_TDTKG702D_LO_DIVIDER_STEP_ANA   ((U16)  50000) //50000)  /* Hz */


STATIC UINT8* pTDTKG702DEqNormal[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pTDTKG702DEqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pTDTKG702DEqStrongChroma[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};

STATIC UINT8 u1AnaTop=5;//default is 104dBuv
///////////////////////////////////
// ALPS TDTKG702D driver
///////////////////////////////////

CHAR* TDTKG702D_GetSwVer(void)
{
	return ((CHAR*)C_TDTKG702D_VERSION);
}


VOID TDTKG702D_TunerInit(ITUNER_CTX_T *pTCtx)
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
//    SPECIFIC_MEMBER_EU_CTX *psTunerSpecificCtx = &(psTunerCtx->specific_member.eu_ctx);

    psTunerCtx->I2cAddress = C_TDTKG702D_LO_ADDRRESS;
    psTunerCtx->u2IF_Freq_A = C_TDTKG702D_IF_FREQUENCY_ANA;
    psTunerCtx->u4RF_Freq = 0;
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_SENS|MASK_PATCH_CCI); 
    psTunerCtx->fgRFAGC = TRUE;
    psTunerCtx->fgRFTuner = TRUE;

    // iven 20110706, add for polarity if setting for NA reference
    psTunerCtx->bSpecPolarity = 0;
    // iven 20110706, add for legacy tuner as default fixed PGA gain index. Don care for Sawless
    psTunerCtx->uSLD_DefaultPGAIndex = 0;		
    
    // init EQ script for mtk board
    pTDTKG702DEqNormal[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQTDTKG702DBG;
    pTDTKG702DEqNormal[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQTDTKG702DDK;
    pTDTKG702DEqNormal[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]   = EQTDTKG702DI;
    pTDTKG702DEqNormal[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN] = EQTDTKG702DL;
    pTDTKG702DEqNormal[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQTDTKG702DL1;
    pTDTKG702DEqNormal[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQTDTKG702DM;
    pTDTKG702DEqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQTDTKG702DBG;
    pTDTKG702DEqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQTDTKG702DDK;
    pTDTKG702DEqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]   = EQTDTKG702DI;
    pTDTKG702DEqWeak[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQTDTKG702DM;
    pTDTKG702DEqStrongChroma[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQTDTKG702DBG;
    
    mcSHOW_USER_MSG(("Tuner TDTKG702D for mtk board init done!\n"));
}

S16 TDTKG702D_TunerSetFreq(ITUNER_CTX_T *pTCtx, PARAM_SETFREQ_T* param)
{
    UCHAR TunerS[5];
    U16   Ndivider;
    UCHAR CB1, BB,CB2; 
    UCHAR ucPllValue;
    U32 Lo;
    U8 u1PllPollCnt;
    U32 Freq = param->Freq; // transfer to video freq 

	
    pTCtx->u4RF_Freq = Freq;

    Lo = Freq + C_TDTKG702D_IF_FREQUENCY_ANA; // kHz
    Ndivider = (U16) ((Lo*1000+C_TDTKG702D_LO_DIVIDER_STEP_ANA/2)/C_TDTKG702D_LO_DIVIDER_STEP_ANA);
		  
    Freq = Freq/1000;//Convert to MHz
    
	CB1=0x85;//RS2 RS1 RS0  1 0  1 Stepsize 50kHz
	if(pTCtx->fgRFAGC)//External RF_AGC
    {
	  CB1 |=0x38; 
    }
	else
	{
	 CB1|=(u1AnaTop<<3);//Manual RF_AGC TOP
	}
	
    CB2 =0xD1;
    
    if (Freq <= 165) {
        BB = 0x01;	//8M		
    }
    else if (Freq  <=450) {        
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
    TunerS[4] = (UCHAR) CB2;	
	
	if (ICtrlBus_I2cTunerWrite(C_TDTKG702D_I2C_CLK_DIV, pTCtx->I2cAddress, TunerS, 5))
    {
            mcSHOW_USER_MSG(("TunerWrite PLL failed!\n"));
            return (+1);
    }
    else
    {
	    mcSHOW_DBG_MSG(("LO_0x%02X:0x%02X-0x%02X-0x%02X-0x%02X-0x%02X(%d KHz)\n",pTCtx->I2cAddress,TunerS[0],TunerS[1],TunerS[2],TunerS[3],TunerS[4],pTCtx->u4RF_Freq));
    }
		
    		for (u1PllPollCnt=0; u1PllPollCnt<C_TDTKG702D_PLL_TIMEOUT_CNT; u1PllPollCnt++)
    		{
            mcDELAY_MS(10);
            
            if (ICtrlBus_I2cTunerRead(C_TDTKG702D_I2C_CLK_DIV, pTCtx->I2cAddress, &ucPllValue, 1))
            {
                mcSHOW_USER_MSG(("Tuner read failed!\n"));
                return (+1);
            }			
            //mcDELAY_MS(10);			
            // check if PLL lock
            if (mcCHK_BIT1(ucPllValue, C_TDTKG702D_PLL_LOCK_BIT)) {
                break;
            }											
            }
        if(u1PllPollCnt < C_TDTKG702D_PLL_TIMEOUT_CNT)
        {
            mcSHOW_DRVAPI_MSG((" PLL lock at the %dth time \n" ,u1PllPollCnt));	;
        }
        else
        {
            mcSHOW_DRVAPI_MSG(("PLL unlock(%d times)\n" ,C_TDTKG702D_PLL_TIMEOUT_CNT));		
        }
      
    return (0);
}


INT16 TDTKG702D_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){
         
    switch(eOperation)
    {
    case itGetVer:
        {
            CHAR ** RetStr = (CHAR **)pInOutVal;
            *RetStr = TDTKG702D_GetSwVer();
        }
        break;
    case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pTDTKG702DEqNormal[SetVal];
        break;
    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pTDTKG702DEqWeak[SetVal];
        break;
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = pTDTKG702DEqStrongChroma[SetVal];
        break;
    case itSetIfMin:
        {
		    UINT8 *ifMinPtr=(UINT8*)pInOutVal; 
                *ifMinPtr = 0x80;
        }
       break;
	case itSetTop:
       	{
		  pTCtx->fgRFAGC=FALSE;
		  u1AnaTop=SetVal;
		 break;
       	}
	case itGetTop:
       {
	    *(UINT8*)pInOutVal =u1AnaTop;
       }
        break;   
    default:
        return ITUNER_NOT_SUPPORTED;
    }

    return ITUNER_OK;
}
