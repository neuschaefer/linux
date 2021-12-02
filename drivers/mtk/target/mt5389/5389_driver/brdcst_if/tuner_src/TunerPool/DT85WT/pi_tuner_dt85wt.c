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

/** @file pi_tuner_env57s.c
 *  Panasonic DT85WT tuner driver.
 */


#include "tuner_interface_if.h"
#include "PI_Def.h"
#include "ctrl_bus.h"
#include "pi_demod_atd.h"
#include "eq_script_dt85wt.h"
//#include "pi_demod.h"




// *** Release Note ***
// Ver     Date         Description
// 1.00    2010.04.14   file established
#define C_DT85WT_VERSION				"DT85WT 2010-01-01\n 2011-04-06\n v1.0"

//#define C_DT85WT_PLL_POLL_INTERVAL   5
#define C_DT85WT_PLL_POLL_INTERVAL   20   // to solve SLT thread UART overflow issue, 090601, Ken
#define C_DT85WT_PLL_TIMEOUT         300
#define C_DT85WT_PLL_TIMEOUT_CNT     C_DT85WT_PLL_TIMEOUT/C_DT85WT_PLL_POLL_INTERVAL
#define C_DT85WT_I2C_CLK_DIV         0x100
#define C_DT85WT_PLL_LOCK_BIT        6



#define C_DT85WT_TOP_SET             ((U8)   0x02) 
#define C_DT85WT_TOP_SET_DVBC        ((U8)   0x04) 
#define C_DT85WT_TOP_SET_ANA         ((U8)   0x0A) 
#define C_DT85WT_SAW_BW              2


//#define C_DT85WT_FREQ_DBOUND_UPPER   858*1000*1000 
#define C_DT85WT_FREQ_DBOUND_UPPER   859*1000*1000      //modified for DVB-C application, Menghau
#define C_DT85WT_FREQ_DBOUND_LOWER    48*1000*1000 
#define C_DT85WT_FREQ_DBOUND_UPPER_Ana     875*1000*1000
#define C_DT85WT_FREQ_DBOUND_LOWER_Ana    45*1000*1000


#define C_DT85WT_LO_ADDRESS           ((U16)  0xC2)
#define C_DT85WT_IF_FREQUENCY          ((U16)  36130)  /* kHz */
#define C_DT85WT_IF_FREQUENCY_ANA      ((U16)  38900)  /* kHz */
#define C_DT85WT_IF_FREQUENCY_L1       ((U16)  33900)  /* kHz */
#define C_DT85WT_LO_DIVIDER_STEP       ((U32) 166667)  /* Hz */
#define C_DT85WT_LO_DIVIDER_STEP_ANA   ((U16)  50000)  /* Hz */


#define C_DT85WT_CHANNEL_SCAN_JUMP_SMALL_STEP ((U16) 250)
#define C_DT85WT_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP ((U16) 1500)
#define C_DT85WT_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN ((U16) 500)

STATIC UINT8* pDt85wtEqNormal[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pDt85wtEqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};


///////////////////////////////////
// ALPS DT85WT driver
///////////////////////////////////

CHAR* DT85WT_GetSwVer(void)
{
	return ((CHAR*)C_DT85WT_VERSION);
}

//Justin TODO, pEUCtx->m_aucPara all necessary????
VOID DT85WT_TunerInit(ITUNER_CTX_T *pTCtx)
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);
    
    psTunerCtx->I2cAddress = C_DT85WT_LO_ADDRESS;
    psTunerCtx->u2IF_Freq = C_DT85WT_IF_FREQUENCY;
    psTunerCtx->u4RF_Freq = 0;
	psTunerCtx->u2IF_Freq_A = C_DT85WT_IF_FREQUENCY_ANA;
    //Justin TODO
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_DRO|MASK_PATCH_SENS);
    psTunerCtx->fgRFAGC = TRUE;
    // AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x2f;  //according to yu ding's mail,20091123
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0xFA;
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B;

    pEUCtx->m_SAW_BW  = C_DT85WT_SAW_BW;
    pEUCtx->m_Ana_Top = C_DT85WT_TOP_SET_ANA;
    pEUCtx->m_s4FreqBoundUpper = C_DT85WT_FREQ_DBOUND_UPPER;
    pEUCtx->m_s4FreqBoundLower = C_DT85WT_FREQ_DBOUND_LOWER;
    pEUCtx->m_s4AnaFreqBoundUpper = C_DT85WT_FREQ_DBOUND_UPPER_Ana;
    pEUCtx->m_s4AnaFreqBoundLower = C_DT85WT_FREQ_DBOUND_LOWER_Ana;
    
    pEUCtx->m_Small_Step = C_DT85WT_CHANNEL_SCAN_JUMP_SMALL_STEP;
    pEUCtx->m_Middle_Step_Up = C_DT85WT_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP;
    pEUCtx->m_Middle_Step_Down = C_DT85WT_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN;
    
    pDt85wtEqNormal[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQDt85wtBG;
    pDt85wtEqNormal[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQDt85wtDK;
    pDt85wtEqNormal[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQDt85wtI;
    pDt85wtEqNormal[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQDt85wtL;
#ifdef CC_MT5363	
    pDt85wtEqNormal[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQDt85wtL1;
#else
    pDt85wtEqNormal[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQDt85wtL;
#endif
    pDt85wtEqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQDt85wtBGWeak;
    pDt85wtEqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQDt85wtDKWeak;
    pDt85wtEqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQDt85wtIWeak;
    pDt85wtEqWeak[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQDt85wtLWeak;
#ifdef CC_MT5363	
    pDt85wtEqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQDt85wtL1Weak;
#else
    pDt85wtEqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQDt85wtLWeak;
#endif

    mcSHOW_USER_MSG(("Tuner DT85WT init done!\n"));
}


 STATIC void  DT85WT_SetSawBw(ITUNER_CTX_T * pTunerCtx, UINT8 sawbw)
 {
   pTunerCtx->specific_member.eu_ctx.m_SAW_BW = sawbw;

   mcSHOW_DRVAPI_MSG(("DT85WT_SetSawBw = %d\n", sawbw));
   
 }
 STATIC void  DT85WT_SetAntIn(ITUNER_CTX_T * pTunerCtx, UINT8 AntIn)
 {
   pTunerCtx->specific_member.eu_ctx.m_Ant_In = AntIn;

   mcSHOW_DRVAPI_MSG(("DT85WT_SetAntIn = %d\n", AntIn));
   
 }

S16 DT85WT_TunerSetFreq(ITUNER_CTX_T *pTCtx, PARAM_SETFREQ_T* param)
{


 U32 Freq = param->Freq; // transfer to video freq
   //U8  AutoSearch = param->fgAutoSearch;
   U8  Mode = param->Modulation;

   UCHAR ucPllValue;
   UCHAR TunerS[5];
   U32 Lo;
   U16 Ndivider, CB1, CB2, BB;
   S16 ii, jj;


   //SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);

   pTCtx->u4RF_Freq = Freq;

   if(Mode == MOD_DTMB) //digital reception
   {

	   Lo = Freq + C_DT85WT_IF_FREQUENCY;
	   Ndivider = (U16)( (Lo*1000+C_DT85WT_LO_DIVIDER_STEP/2) / C_DT85WT_LO_DIVIDER_STEP	);
	   
	   mcSHOW_DRVAPI_MSG(("DTMB DigFreq = %d, Divider ratio = %04X \n",Freq, Ndivider));
	   
	   // Conver Freq to unit of MHz
	   Freq = Freq/1000;
	   
	   if (Freq < (C_DT85WT_FREQ_DBOUND_LOWER/1000/1000) ||
		   Freq > (C_DT85WT_FREQ_DBOUND_UPPER/1000/1000)  )
	   {
		   mcSHOW_DRVERR_MSG(("Out of range for LO!\n"));
		 //return (+1);
	   }
	   
	   CB1=0X98;
	   CB2=0XC1;

        if((Freq<120)||((Freq>=149)&&(Freq<300))||((Freq>=444)&&(Freq<640)))
        BB=0X50;
		else if (((Freq>=120)&&(Freq>=149))||((Freq>=300)&&(Freq<444))||((Freq>=640)&&(Freq<764)))
			BB=0X90;
		else if ((Freq>=764)&&(Freq<824))
			BB=0XD0;
		else
			BB=0X10;

		if(Freq<149)
			BB|=0X01;
		else if(Freq<444)
			BB|=0X02;
		else
			BB|=0X08;
			/*						  
		if (pTCtx->specific_member.eu_ctx.m_SAW_BW == 1)
		 {mcSHOW_DBG_MSG(("set sAW"))
		 BB = (BB &0xef); }
		 if (pTCtx->specific_member.eu_ctx.m_SAW_BW == 2)
		 {mcSHOW_DBG_MSG(("set sAW"))
		 BB = (BB |0x10); }*/
   }

   
   else
   {
	   if( Mode == MOD_SECAM_L1 )
	   {
		   Lo = Freq + C_DT85WT_IF_FREQUENCY_L1; // kHz
		   Ndivider = (U16) ( (Lo*1000+C_DT85WT_LO_DIVIDER_STEP_ANA/2) / C_DT85WT_LO_DIVIDER_STEP_ANA);
		   mcSHOW_DBG_MSG(("Freq = %d, Divider ratio = %04X PLL set freq=%d kHz\n",Freq, Ndivider,(Ndivider*C_DT85WT_LO_DIVIDER_STEP_ANA/1000-C_DT85WT_IF_FREQUENCY_L1)));
	   }
	   else
	   {
		   Lo = Freq + C_DT85WT_IF_FREQUENCY_ANA; // kHz
		   Ndivider = (U16) ( (Lo*1000+C_DT85WT_LO_DIVIDER_STEP_ANA/2) / C_DT85WT_LO_DIVIDER_STEP_ANA);
		   mcSHOW_DBG_MSG(("Freq = %d, Divider ratio = %04X PLL set freq=%d kHz\n",Freq, Ndivider,(Ndivider*C_DT85WT_LO_DIVIDER_STEP_ANA/1000-C_DT85WT_IF_FREQUENCY_ANA)));
	   }

	   if (Freq < C_DT85WT_FREQ_DBOUND_LOWER_Ana/1000 || Freq > C_DT85WT_FREQ_DBOUND_UPPER_Ana/1000)
	   {
		   mcSHOW_DRVERR_MSG(("Out of range for LO!\n"));
		 //return (+1);
	   }
				
			// Conver Freq to unit of MHz
			Freq = Freq/1000;
			
		  if (Freq < (C_DT85WT_FREQ_DBOUND_LOWER_Ana/1000/1000) || 
				Freq > (C_DT85WT_FREQ_DBOUND_UPPER_Ana/1000/1000)  )
			{
				mcSHOW_DRVAPI_MSG(("Out of range for LO!\n"));
				//return (+1);
			}

          CB1=0XBD;
		  CB2=0XC1;
	/*
        if((Freq<120)||((Freq>=149)&&(Freq<300))||((Freq>=444)&&(Freq<640)))
        BB=0X50;
		else if (((Freq>=120)&&(Freq>=149))||((Freq>=300)&&(Freq<444))||((Freq>=640)&&(Freq<764)))
			BB=0X90;
		else if ((Freq>=764)&&(Freq<824))
			BB=0XD0;
		else
			BB=0X10;*/

	    BB = 0X00;

		if(Freq<149)
			BB|=0X01;
		else if(Freq<444)
			BB|=0X02;
		else
			BB|=0X08;		
			
		}
	
	
		/*if (AutoSearch)
		{
			 CB2 |= 0X20; // set ATC = 1
	
		}*/

   if(pTCtx->specific_member.eu_ctx.m_Ant_In == 1)
   	  BB&=0xFB; 
   else if(pTCtx->specific_member.eu_ctx.m_Ant_In == 2)
      BB|=0x04;
  		
   for (jj=0; jj<3; jj++)  //retry 2 times while PLL unlock, LC 070702
   {
	   // LO settings
	   TunerS[0] = (UCHAR) 0x7F & (Ndivider >> 8);
	   TunerS[1] = (UCHAR) 0xFF & Ndivider;
	   TunerS[2] = (UCHAR) CB1;
	   TunerS[3] = (UCHAR) BB;
	   TunerS[4] = (UCHAR) CB2;


	 mcSHOW_DRVAPI_MSG(("0x%02X-0x%02X-0x%02X-0x%02X-0x%02X\n", TunerS[0], TunerS[1], TunerS[2], TunerS[3], TunerS[4]));

 //  send this data
	 //if (RegSetRegistersTuner(pTunerCtx->m_Address, 5, &TunerS[0]) < 0)


	   if (ICtrlBus_I2cTunerWrite(C_DT85WT_I2C_CLK_DIV, pTCtx->I2cAddress, TunerS, 5))
	   {
		   mcSHOW_HW_MSG(("TunerWrite PLL failed!\n"));
		   return (+1);
	   }
	   mcDELAY_MS(100);

	   // wait PLL lock
   //ucTunerLockConfCnt = 0;
	   for (ii=0; ii<C_DT85WT_PLL_TIMEOUT_CNT; ii++)
	   {
		   if (ICtrlBus_I2cTunerRead(C_DT85WT_I2C_CLK_DIV, pTCtx->I2cAddress, &ucPllValue, 1))
		   {
			   mcSHOW_HW_MSG(("Tuner read failed!\n"));
			   return (+1);
		   }
		   // check if PLL lock
		   if (mcCHK_BIT1(ucPllValue, C_DT85WT_PLL_LOCK_BIT)) {
			   break;
		   }
		   mcDELAY_MS(C_DT85WT_PLL_POLL_INTERVAL);
	   }

	   if(ii < C_DT85WT_PLL_TIMEOUT_CNT) {
			mcSHOW_DBG_MSG(("Freq = %d MHz, the %d-th time setting PLL success\n" , Freq, jj+1));
		   break;
		 }
	   else {
			   if (Freq < 48)
			  {    
			 mcSHOW_DBG_MSG(("Freq = %d MHz,Frequency not support\n" , Freq));//liuqu,20090828,DHCID00000589
		   }else
		   {
			 mcSHOW_DBG_MSG(("Freq = %d MHz, the %d-th time setting PLL fail\n" , Freq, jj+1));
		   }
	 }
 }
   
   return 0;

}


INT16 DT85WT_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){

    switch(eOperation){
    case itGetVer:
        {
        CHAR ** RetStr = (CHAR **)pInOutVal;
        *RetStr = DT85WT_GetSwVer();
        }
        break;
    case itSetTop:
	    {
          SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);
          pTunerCtx->m_aucPara[0] = SetVal;
	    }
		break;
		
	case itSetReg:
	 {
        
		 UCHAR TunerS[5];
		 UCHAR ucPllValue;
		 UINT8 *ptrWord = (UINT8 *)pInOutVal;
		 int i,j;
	
		 SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);
		 
		 for(i = 0; i < 5; i++)
			{
					
					  TunerS[i]=(UCHAR)(*ptrWord);
					  ptrWord++;
					  mcSHOW_DRVERR_MSG(("The register word is: 0x%x\n",TunerS[i]));
					 // mcDELAY(C_DT85WT_PLL_POLL_INTERVAL);
			}
           mcSHOW_DRVERR_MSG(("The register words are: 0x%x-0x%x-0x%x-0x%x-0x%x\n",TunerS[0],TunerS[1],TunerS[2],TunerS[3],TunerS[4]));
	 for (j=0; j<2; j++)  //retry 2 times while PLL unlock, LC 070702
       {
   
           if (ICtrlBus_I2cTunerWrite(C_DT85WT_I2C_CLK_DIV, pTCtx->I2cAddress, TunerS, 5))    //  send this data
            {
             mcSHOW_DRVERR_MSG(("TunerWrite PLL failed!\n"));
			  
               return (+1);
            }
                mcSHOW_DRVERR_MSG(("TunerWrite PLL success!\n"));
		          pTunerCtx->m_aucPara[1]  = (UCHAR)TunerS[0]; //send register value to UI, LC 070102
                  pTunerCtx->m_aucPara[15] = (UCHAR)TunerS[1];
                  pTunerCtx->m_aucPara[16] = (UCHAR)TunerS[2];
                  pTunerCtx->m_aucPara[17] = (UCHAR)TunerS[3];
                  pTunerCtx->m_aucPara[18] = (UCHAR)TunerS[4];  
   
          for (i=0; i<C_DT85WT_PLL_TIMEOUT_CNT; i++)
          {
            if (ICtrlBus_I2cTunerRead(C_DT85WT_I2C_CLK_DIV, pTCtx->I2cAddress, &ucPllValue, 1))
            {
              
				mcSHOW_HW_MSG(("Tuner read failed!\n"));
                return (+1);
            }
           
            if (mcCHK_BIT1(ucPllValue, C_DT85WT_PLL_LOCK_BIT))   // check if PLL lock
				{
                   pTunerCtx->m_aucPara[19] = ucPllValue;
					break;
                }
            mcDELAY_MS(C_DT85WT_PLL_POLL_INTERVAL);
          }

          if(i < C_DT85WT_PLL_TIMEOUT_CNT) 
			  {
                  mcSHOW_DBG_MSG(("the %d-th time setting PLL success\n" , j+1));
				  
				  break;
          	   }
          else
        	   mcSHOW_DBG_MSG((" the %d-th time setting PLL fail\n" , j+1));
               
		 	}
		 
	  }
	    break;
		
	case itGetReg:
	    {
            
			int i = 0;
	        SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);
			              *((INT8 *) pInOutVal)   = pTunerCtx->m_aucPara[1]; 
                          *((INT8 *) pInOutVal+1) = pTunerCtx->m_aucPara[15];
                          *((INT8 *) pInOutVal+2) = pTunerCtx->m_aucPara[16];
			              *((INT8 *) pInOutVal+3) = pTunerCtx->m_aucPara[17];
                          *((INT8 *) pInOutVal+4) = pTunerCtx->m_aucPara[18];
                for (i = 0; i < 5; i++)
				mcSHOW_DRVERR_MSG(("The register word is: 0x%x\n",*((INT8 *)pInOutVal+i)));
			   
        }
		break;
    case itSetSawBw:
		DT85WT_SetSawBw(pTCtx,SetVal);
        break;
    case itSetLNA:
    case itGetLNA:
        break;

    case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pDt85wtEqNormal[SetVal];
        break;
    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pDt85wtEqWeak[SetVal];
        break;
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = NULL;
        break;
	case itSetAntIn:
		DT85WT_SetAntIn(pTCtx,SetVal);
		break;
	case itSetIfMin:
	    *(UINT8 *)pInOutVal=0xF0;
		break;
#if 0
	case itSetIfMin:
	    {

          UINT8 subSysId = SetVal;
		  UINT8 *ifMinPtr = (UINT8*)pInOutVal;
			
         mcSHOW_DBG_MSG(("DT85WT TOP Setting\n"));
    	 switch(subSysId){
    	    case MOD_PAL_BG:
    	    case MOD_PAL_I:
	    case MOD_PAL_DK:	
	      {
		if ((pTCtx->u4RF_Freq/1000 >= 71) && (pTCtx->u4RF_Freq/1000 <= 80))
			{
		        *ifMinPtr = 0x15;
			}
		else
			{
    	               *ifMinPtr = 0x00; //according to yu ding's mail,20091123
			}
	    	}
    	        break;
    	    case MOD_SECAM_L:
            case MOD_SECAM_L1:
               *ifMinPtr = 0x09;  ////according to yu ding's mail,20091123
    		    break;
    	    default:
    	        mcSHOW_DBG_MSG(("TOP No change\n"));
    	        break;
        }

	    }
	    break;
#endif

    default:
        return ITUNER_NOT_SUPPORTED;
    }
    return ITUNER_OK;
}
