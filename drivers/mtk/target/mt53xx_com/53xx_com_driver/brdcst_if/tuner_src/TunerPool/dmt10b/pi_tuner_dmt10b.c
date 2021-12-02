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
 * $RCSfile: pi_tuner_env57s.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_DMT10B.c
 *  Panasonic DMT10B tuner driver.
 */


#include "tuner_interface_if.h"
#include "PI_Def.h"
#include "ctrl_bus.h"

#include "pi_demod_atd.h"
#include "eq_script_dmt10b.h"
#include "fe_tuner_common_if.h"


// *** Release Note ***
// Ver     Date         Description
// 1.00    2009.03.25   file established
#define C_DMT10B_VERSION				" DMT10B 2010-01-01\n 2011-04-06\n v1.0"

//#define C_DMT10B_PLL_POLL_INTERVAL   5
#define C_DMT10B_PLL_POLL_INTERVAL   20   // to solve SLT thread UART overflow issue, 090601, Ken
#define C_DMT10B_PLL_TIMEOUT         100
#define C_DMT10B_PLL_TIMEOUT_CNT     C_DMT10B_PLL_TIMEOUT/C_DMT10B_PLL_POLL_INTERVAL
#define C_DMT10B_I2C_CLK_DIV         0x100
#define C_DMT10B_PLL_LOCK_BIT        6

//#define C_DMT10B_FREQ_DBOUND_UPPER   858*1000*1000 
#define C_DMT10B_FREQ_DBOUND_UPPER   863250*1000      //modified for DVB-C application, Menghau
#define C_DMT10B_FREQ_DBOUND_LOWER    48000*1000 
#define C_DMT10B_FREQ_DBOUND_UPPER_Ana     863250*1000 
#define C_DMT10B_FREQ_DBOUND_LOWER_Ana    48000*1000 


#define C_DMT10B_TOP_SET_ANA         ((U8)   0x03) 

#define C_DMT10B_LO_ADDRESS           ((U16)  0xC2)
#define C_DMT10B_IF_FREQUENCY          ((U16)  36000)  /* kHz */
#define C_DMT10B_IF_FREQUENCY_ANA      ((U16)  38900)  /* kHz */
#define C_DMT10B_LO_DIVIDER_STEP       ((U32) 166667)  /* 1/6MHz */
#define C_DMT10B_LO_DIVIDER_STEP_ANA   ((U16)  50000)  /* 1/16 MHz */


#define C_DMT10B_CHANNEL_SCAN_JUMP_SMALL_STEP ((U16) 250)
#define C_DMT10B_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP ((U16) 1500)
#define C_DMT10B_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN ((U16) 500)



STATIC UINT8* pDMT10BEqNormal[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pDMT10BEqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};


///////////////////////////////////
// ALPS DMT10B driver
///////////////////////////////////

CHAR* DMT10B_GetSwVer(void)
{
	return ((CHAR*)C_DMT10B_VERSION);
}

//Justin TODO, pEUCtx->m_aucPara all necessary????
VOID DMT10B_TunerInit(ITUNER_CTX_T *pTCtx)
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);
    
    psTunerCtx->I2cAddress = C_DMT10B_LO_ADDRESS;
    psTunerCtx->u2IF_Freq = C_DMT10B_IF_FREQUENCY;
    psTunerCtx->u4RF_Freq = 0;
	psTunerCtx->u2IF_Freq_A = C_DMT10B_IF_FREQUENCY_ANA;
    //Justin TODO
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_DRO|MASK_PATCH_SENS);
    psTunerCtx->fgRFAGC = FALSE;
    // AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x2f;  //according to yu ding's mail,20091123
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0xFA;
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B;

    pEUCtx->m_Ana_Top = C_DMT10B_TOP_SET_ANA;

    pEUCtx->m_s4FreqBoundUpper = C_DMT10B_FREQ_DBOUND_UPPER;
    pEUCtx->m_s4FreqBoundLower = C_DMT10B_FREQ_DBOUND_LOWER;
    pEUCtx->m_s4AnaFreqBoundUpper = C_DMT10B_FREQ_DBOUND_UPPER_Ana;
    pEUCtx->m_s4AnaFreqBoundLower = C_DMT10B_FREQ_DBOUND_LOWER_Ana;
    
    pEUCtx->m_Small_Step = C_DMT10B_CHANNEL_SCAN_JUMP_SMALL_STEP;
    pEUCtx->m_Middle_Step_Up = C_DMT10B_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP;
    pEUCtx->m_Middle_Step_Down = C_DMT10B_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN;


    pDMT10BEqNormal[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQDMT10BBG;
    pDMT10BEqNormal[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQDMT10BDK;
    pDMT10BEqNormal[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQDMT10BI;
    pDMT10BEqNormal[MOD_NTSC_M- MOD_ANA_TYPE_BEGIN]    = EQDMT10BM;	

    mcSHOW_USER_MSG(("Tuner DMT10B init done!\n"));
}

S16 DMT10B_TunerSetFreq(ITUNER_CTX_T *pTCtx, PARAM_SETFREQ_T* param)
{
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);
    U32 Freq = param->Freq; // transfer to video freq
    //U8  AutoSearch = param->fgAutoSearch;
    U8  Mode = param->Modulation;

    UCHAR ucPllValue;
    UCHAR TunerS[5];
  //  U32 Lo;
    U16 Ndivider, CB1, BB, CB2;
    S16 ii;


    //SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);

    pTCtx->u4RF_Freq = Freq;

	CB2 = 0xD1;
	
	 if (Freq <= 145000)//VL
    {
        BB=0x01;	
    }
	else if(Freq<=430000)//VH
	{
	   BB=0x02;
	}
	else//UHF
	{
	   BB=0x08;
	}
	
    if((Mode == MOD_DTMB) || (Mode==MOD_DVBC)) //Digital reception
    {
        Ndivider = (U16)((Freq*6+C_DMT10B_IF_FREQUENCY*6)/1000);//Stepsize 1000/6KHz
        
        if (Freq < (C_DMT10B_FREQ_DBOUND_LOWER/1000) ||
            Freq > (C_DMT10B_FREQ_DBOUND_UPPER/1000))
        {
            mcSHOW_DRVERR_MSG(("Out of range for LO!\n"));
          //return (+1);
        }
		CB1=0x98;//108dBuv RS2 RS1 RS0 0 0 0  166.67KHz
		
		if(Freq<=111000)
		{
		 BB|=0x40;
		}
		else if(Freq<=145000)
		{
		 BB|=0x80;
		}
		else if(Freq<=370000)
		{
		 BB|=0x40;
		}
		else if(Freq<=430000)
		{
		 BB|=0x80;
		}		
		else if(Freq<=700000)
		{
		 BB|=0x40;
		}
        else
        {
         BB|=0x80;		
        }	
    }
	else//ATV
    {
	    Ndivider = (U16)((Freq*20+C_DMT10B_IF_FREQUENCY_ANA*20)/1000);//Stepsize 1000/20=50KHz
      	    if (Freq < C_DMT10B_FREQ_DBOUND_LOWER_Ana/1000 || Freq > C_DMT10B_FREQ_DBOUND_UPPER_Ana/1000)
	    {
	        mcSHOW_USER_MSG(("Out of range for LO!\n"));
	      //  return (+1);
	    }
		
		 CB1=0x87;//RS2 RS1 RS0 1 1  1 Stepsize 50KHz
           //Top setting
		  CB1|=(pEUCtx->m_Ana_Top<<3); 
    }
   
        // LO settings
        TunerS[0] = (UCHAR) 0x7F & (Ndivider >> 8);
        TunerS[1] = (UCHAR) 0xFF & Ndivider;
        TunerS[2] = (UCHAR) CB1;
        TunerS[3] = (UCHAR) BB;
        TunerS[4] = (UCHAR) CB2;

  //  send this data

        if (ICtrlBus_I2cTunerWrite(C_DMT10B_I2C_CLK_DIV, pTCtx->I2cAddress, TunerS, 5))
        {
            mcSHOW_HW_MSG(("TunerWrite PLL failed!\n"));
            return (+1);
        }
		mcDELAY_MS(50);

        // wait PLL lock
    //ucTunerLockConfCnt = 0;
        for (ii=0; ii<C_DMT10B_PLL_TIMEOUT_CNT; ii++)
        {
            if (ICtrlBus_I2cTunerRead(C_DMT10B_I2C_CLK_DIV, pTCtx->I2cAddress, &ucPllValue, 1))
            {
                mcSHOW_HW_MSG(("Tuner read failed!\n"));
                return (+1);
            }
            // check if PLL lock
            if (mcCHK_BIT1(ucPllValue, C_DMT10B_PLL_LOCK_BIT)) {
                break;
            }
            mcDELAY_MS(C_DMT10B_PLL_POLL_INTERVAL);
        }

        if(ii < C_DMT10B_PLL_TIMEOUT_CNT)
		{
             mcSHOW_DBG_MSG(("Freq = %d MHz, the %d-th time setting PLL success\n" , Freq, ii+1));
          }
        else 
		{
        	if (Freq < 48)
        	   {	
              mcSHOW_DBG_MSG(("Freq = %d MHz,Frequency not support\n" , Freq));//liuqu,20090828,DHCID00000589
            }else
          	{
              mcSHOW_DBG_MSG(("Freq = %d MHz, the %d-th time setting PLL fail\n" , Freq, ii+1));
            }
        }
    mcSHOW_DBG_MSG(("LO_%02X:0x%02X-0x%02X-0x%02X-0x%02X-0x%02X\n" , pTCtx->I2cAddress, TunerS[0],TunerS[1],TunerS[2],TunerS[3],TunerS[4]));
    return 0;
}

INT16 DMT10B_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){

    switch(eOperation){
    case itGetVer:
        {
        CHAR ** RetStr = (CHAR **)pInOutVal;
        *RetStr = DMT10B_GetSwVer();
        }
        break;
    case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pDMT10BEqNormal[SetVal];
        break;
    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pDMT10BEqWeak[SetVal];
        break;
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = NULL;
        break;
	case itSetTop:
       	{
		 SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);
		 pEUCtx->m_Ana_Top=SetVal;
		 break;
       	}
	case itGetTop:
       {
	    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);
	    *(UINT8*)pInOutVal =pEUCtx->m_Ana_Top;
       }
        break;
    default:
        return ITUNER_NOT_SUPPORTED;
    }
    return ITUNER_OK;
}
