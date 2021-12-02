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
 * $RCSfile: pi_tuner_TunerA.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_TunerA.c
 *  Tuner control for TunerA
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "tuner_interface_if.h"
#include "fe_tuner_common_if.h"
#include "i2c_api.h"
#include "x_typedef.h"
#include "tunerDebug.h"
#include "x_os.h"
#include "tuner_if.h"

#include "eq_script_TunerA.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
//Frequency boundary .Get from tuner spec
#define C_TUNERA_FREQ_DBOUND_UPPER   		870*1000*1000     
#define C_TUNERA_FREQ_DBOUND_LOWER    	42*1000*1000 
#define C_TUNERA_FREQ_DBOUND_UPPER_Ana    870*1000*1000
#define C_TUNERA_FREQ_DBOUND_LOWER_Ana    42*1000*1000

//PLL lock check parameters
#define C_VENDOR_TUNERA_PLL_POLL_INTERVAL    10 //ms
#define C_VENDOR_TUNERA_PLL_POLL_TIMETOUT    100 //ms
#define C_VENDOR_TUNERA_PLL_POLL_CNT         C_VENDOR_TUNERA_PLL_POLL_TIMETOUT/C_VENDOR_TUNERA_PLL_POLL_INTERVAL //counter
#define C_TUNERA_I2C_CLK_DIV         		0x100
#define C_TUNERA_PLL_LOCK_BIT        		6

#define VENDOR_TUNERA_ADDRESS           		((UINT8)  0xC0)
#define C_TUNERA_IF_FREQUENCY          	((UINT16)  3250)  /* kHz */
#define C_TUNERA_LO_DIVIDER_STEP       	((UINT32) 166667)  /* Hz */
#define C_TUNERA_LO_DIVIDER_STEP_ANA   	((UINT16)  50000)  /* Hz */
#define C_VENDOR_TUNERA_STR                   "VENDOR_TUNERA 2010-01-01\n 2011-09-14\n v1.0"

#define C_TUNERA_IF_FREQUENCY_PAL_B    	((UINT16)  6400)  /* kHz */
#define C_TUNERA_IF_FREQUENCY_PAL_G    	((UINT16)  6750)  /* kHz */
#define C_TUNERA_IF_FREQUENCY_PAL_DK   	((UINT16)  6850)  /* kHz */
#define C_TUNERA_IF_FREQUENCY_PAL_I    	((UINT16)  7250)  /* kHz */
#define C_TUNERA_IF_FREQUENCY_NTSC_M   	((UINT16)  5400)  /* kHz */
#define C_TUNERA_IF_FREQUENCY_SECAM_L  	((UINT16)  6750)  /* kHz */
#define C_TUNERA_IF_FREQUENCY_SECAM_L1 	((UINT16)  1250)  /* kHz */
#define C_TUNERA_IF_FREQUENCY_DVBT_6M     ((UINT16)  3250)  /* kHz */  //JUST for 3.25M
#define C_TUNERA_IF_FREQUENCY_DVBT_7M     ((UINT16)  3500)  /* kHz */  //JUST for 3.5M
#define C_TUNERA_IF_FREQUENCY_DVBT_8M     ((UINT16)  4000)  /* kHz */  //JUST for 4M
#define C_TUNERA_IF_FREQUENCY_ATSC        ((UINT16)  3250)  /* kHz */  //JUST for 3.25M
#define C_TUNERA_IF_FREQUENCY_DVBC        ((UINT16)  5000)  /* kHz */  //JUST for 5M
#define C_TUNERA_IF_FREQUENCY_ATSC_64QAM  ((UINT16)  3600) /*kHz*/  
#define C_TUNERA_IF_FREQUENCY_ISDBT    	((UINT16)  4000) /*kHz*/  
#define C_TUNERA_IF_FREQUENCY_DTMB 		((UINT16)  4000) /*kHz*/  

#define C_TUNERA_SAW_BW              		SAW_BW_8M
#define C_TUNERA_TOP_SET_ANA         		((UINT8)   0x0A) 

#define C_TUNERA_CHANNEL_SCAN_JUMP_SMALL_STEP ((UINT16) 250)
#define C_TUNERA_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP ((UINT16) 1500)
#define C_TUNERA_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN ((UINT16) 500)

//MT5135 PRA 
#define C_TUNERA_AGC_IF_SLP             0.25 
#define C_TUNERA_AGC_IF_INT             0
#define C_TUNERA_AGC_IF_MAX             0.998
#define C_TUNERA_AGC_IF_MIN            -1

#define C_U8_TUNERA_AGC_IF_SLP          (INT8)(C_TUNERA_AGC_IF_SLP *64 )
#define C_U8_TUNERA_AGC_IF_INT          (INT8)(C_TUNERA_AGC_IF_INT *16 )
#define C_U8_TUNERA_AGC_IF_MAX          (INT8)(C_TUNERA_AGC_IF_MAX *128)
#define C_U8_TUNERA_AGC_IF_MIN          (INT8)(C_TUNERA_AGC_IF_MIN *128)




static UINT8* pTUNERAEqNormal[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
static UINT8* pTUNERAEqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN]= {NULL};



/*************************************************
 DVB-T tuning parameter struct
**************************************************/
 enum
	{
		SAW_BW_6M = 0,
		SAW_BW_7M,
		SAW_BW_8M,
		SAW_BW_5M
	};

#if defined(CC_MT5396)  || defined(CC_MT5368) || defined(CC_MT5389)

static ATD_TUNER_REL_REG_T arrAtdRelReg[]={
 
 {0x7b7,0x10},
 {0x7b9,0x10},
 {0x790,0x08},
 {0x794,0x30},
 {0x7a4,0x30},
 {0x1b4,0x17},
 {0x7d4,0x04},
 {0x7d5,0x00},
 {0x7d6,0x00},
 {0x7d7,0xEC},
 {0x7d8,0xEF},
 {0x7d9,0xEF},
 {0x7da,0x7F},
 {0x7db,0x7F},
 {0x7dc,0x80},
 {0x7dd,0x80},
 {0x7de,0x80},
 /*
 {0xfd4,0x04},
 {0xfd5,0x00},
 {0xfd6,0x00},
 {0xfd7,0xEC},
 {0xfd8,0xEF},//
 {0xfd9,0xEF},//
 {0xfda,0x7F},
 {0xfdb,0x7F},//
 {0xfdc,0x80},
 {0xfdd,0x80},//
 {0xfde,0x80},
 */
 {0x00,0x00}//end flag
 };
#else
 static ATD_TUNER_REL_REG_T arrAtdRelReg[]={

 {0x2d4,0x04},//PRA target level[11:4] upper boundary
 {0x2d5,0x00},
 {0x2d6,0x00},//IF_BIAS1
 {0x2d7,0xec},//IF_BIAS2
 {0x2d8,0xef},//IF_SLOPE1
 {0x2d9,0xef},//IF_SLOPE2
 {0x2da,0x7f},
 {0x2db,0x7f},
 {0x2dc,0x80},
 {0x2dd,0x80},
 {0x2de,0x80},
 {0x2b7,0x10},
 {0x2b9,0x10},
 {0x290,0x08},
 {0x294,0x38},//DAGC Target H
 {0x2a4,0x28}, //DAGC Target L
 {0x13b,0x80},
 {0x00,0x00}//end flag
};
#endif


/** TUNERA_TunerInit
 *  Tuner initialzation forTUNERA.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval void
 */

//-----------------------------------------------------------------------------
VOID TUNERA_TunerInit(
    ITUNER_CTX_T *pTCtx
    )
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_US_CTX * pUSCtx= &(pTCtx->specific_member.us_ctx);
	SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);

    psTunerCtx->I2cAddress = VENDOR_TUNERA_ADDRESS;
    psTunerCtx->u2IF_Freq = C_TUNERA_IF_FREQUENCY;
	psTunerCtx->u4RF_Freq = 0;

	 //ATD patch selection
    psTunerCtx->u1AtdPatchSwitch =0x09;
    psTunerCtx->fgRFAGC = FALSE;//indicate if tuner need extern RF_AGC
	psTunerCtx->fgRFTuner =FALSE;//Saw tuner(FALSE) or sawless tuner(TRUE)

	pEUCtx->m_SAW_BW  = C_TUNERA_SAW_BW;
    pEUCtx->m_Ana_Top = C_TUNERA_TOP_SET_ANA;

	//Tuner frequency range
    pEUCtx->m_s4FreqBoundUpper = C_TUNERA_FREQ_DBOUND_UPPER;
    pEUCtx->m_s4FreqBoundLower = C_TUNERA_FREQ_DBOUND_LOWER;
    pEUCtx->m_s4AnaFreqBoundUpper = C_TUNERA_FREQ_DBOUND_UPPER_Ana;
    pEUCtx->m_s4AnaFreqBoundLower = C_TUNERA_FREQ_DBOUND_LOWER_Ana;
	
	pEUCtx->m_Small_Step = C_TUNERA_CHANNEL_SCAN_JUMP_SMALL_STEP;
    pEUCtx->m_Middle_Step_Up = C_TUNERA_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP;
    pEUCtx->m_Middle_Step_Down = C_TUNERA_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN;

	#if defined(CC_MT5396)  || defined(CC_MT5368) || defined(CC_MT5389)
	pEUCtx->m_aucPara[2]= (UINT8)C_U8_TUNERA_AGC_IF_SLP; 
	pEUCtx->m_aucPara[3]= (UINT8)C_U8_TUNERA_AGC_IF_INT;
    pEUCtx->m_aucPara[4]= (UINT8)C_U8_TUNERA_AGC_IF_MAX;
	pEUCtx->m_aucPara[5]= (UINT8)C_U8_TUNERA_AGC_IF_MIN;
	
	pUSCtx->m_aucPara_SLD[2] = (UINT8)C_U8_TUNERA_AGC_IF_SLP; 
    pUSCtx->m_aucPara_SLD[3] = (UINT8)C_U8_TUNERA_AGC_IF_INT;
    pUSCtx->m_aucPara_SLD[4] = (UINT8)C_U8_TUNERA_AGC_IF_MAX;
    pUSCtx->m_aucPara_SLD[5] = (UINT8)C_U8_TUNERA_AGC_IF_MIN;
	#else
	//AD Target set ,for 5365+5112ee
    pUSCtx->m_aucPara[TUNER_FUNC_EN_TARGET_LEVEL] = TRUE;
	pUSCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_MSB] = 0x23;   
    pUSCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_LSB] = 0x00;  
    #endif
    
	//EQ script
  	pTUNERAEqNormal[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]=EQTUNERABG;
	pTUNERAEqNormal[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]=EQTUNERADK;
	pTUNERAEqNormal[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]=EQTUNERAI;
	pTUNERAEqNormal[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]=EQTUNERAL;
	pTUNERAEqNormal[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN]=EQTUNERAL1;
	pTUNERAEqNormal[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]=EQTUNERAM;
    //Weak signal EQ script
	pTUNERAEqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]=EQTUNERABGWeak;
	pTUNERAEqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]=EQTUNERADKWeak;
	pTUNERAEqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]=EQTUNERAIWeak;
	pTUNERAEqWeak[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]=EQTUNERALWeak;
	pTUNERAEqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN]=EQTUNERAL1Weak;
	pTUNERAEqWeak[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]=EQTUNERAMWeak;
	
	mcDBG_MSG_L2(("TUNERA_TunerInit\n"));

 } 
//----------------------------------------------------------------------------- 
/**  TUNERA_TunerGetStatus
 * get Tuner PLL staus 
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval TRUE        PLL lock
 *  @retval FALSE       PLL unlock
 */
//-----------------------------------------------------------------------------
static BOOL TUNERA_TunerGetStatus(ITUNER_CTX_T *pTCtx)
{
       BOOL fgPllLockSts=TRUE; 
       //TODO:implement get tuner pll lock function
	  
	  //End
	  return fgPllLockSts;
}
//----------------------------------------------------------------------------- 
/** TUNERA_TunerSetFreq
 *  Set Tuner PLL forTUNERA/TUNERA to tune RF frequency.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param 	param    Pointer of structure of parameters
 *  @retval  0 success
                    1  fail   out of range 
                   -1 fail  I2C error 
 */
//-----------------------------------------------------------------------------
INT16 TUNERA_TunerSetFreq(
    ITUNER_CTX_T *pTCtx,
    PARAM_SETFREQ_T* param
    )
{
   UINT32 Freq = param->Freq*1000; // transfer to video freq(Hz)
   UINT8 Mode = param->Modulation;//modulation type.Please refer to fe_tuner_common_if.h
      UINT8 ii;
   SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);

   pTCtx->u4RF_Freq = Freq/1000;//Update current frequency to context
   
   mcDBG_MSG_L2((C_VENDOR_TUNERA_STR " Set Freq %d Hz\n",Freq));
     
    //Check if frequency out of tuner LO range			
    if (Freq < (C_TUNERA_FREQ_DBOUND_LOWER) || 
				Freq > (C_TUNERA_FREQ_DBOUND_UPPER))
	{
		mcDBG_MSG_ERR(("Out of range for LO!\n"));
		//return (+1);
	}
   //digital reception
   if( Mode == MOD_ATSC_8VSB)
   {
	  pTCtx->u2IF_Freq =C_TUNERA_IF_FREQUENCY_ATSC;//update frequency of  tuner output (KHz)
   }		
   else if((Mode==MOD_J83B_64QAM)||(Mode==MOD_J83B_256QAM)||(Mode==MOD_J83B_AUTO_QAM))
	{ 
	  pTCtx->u2IF_Freq=C_TUNERA_IF_FREQUENCY_ATSC_64QAM;   //Set the Demod IF freq (KHz)
	}
    else if(Mode==MOD_DVBT)
    { 
 	   if(pTunerCtx->m_SAW_BW==SAW_BW_6M)
	   { 
	    pTCtx->u2IF_Freq =C_TUNERA_IF_FREQUENCY_DVBT_6M;
	   }
        else if(pTunerCtx->m_SAW_BW==SAW_BW_7M)
	   { 
	     pTCtx->u2IF_Freq =C_TUNERA_IF_FREQUENCY_DVBT_7M;
	   }
	     else 
	    { 
	     pTCtx->u2IF_Freq =C_TUNERA_IF_FREQUENCY_DVBT_8M;
	    }
   }
   else if(Mode==MOD_DVBT2)
   { 
 	if(pTunerCtx->m_SAW_BW==SAW_BW_6M)
	{
	 pTCtx->u2IF_Freq =C_TUNERA_IF_FREQUENCY_DVBT_6M;
	}
    else if(pTunerCtx->m_SAW_BW==SAW_BW_7M)
	{ 
	  pTCtx->u2IF_Freq =C_TUNERA_IF_FREQUENCY_DVBT_7M;
	}
	else 
	{ 
	  pTCtx->u2IF_Freq =C_TUNERA_IF_FREQUENCY_DVBT_8M;
	}
   } 
   else if(Mode ==MOD_DVBC)
   {
	pTCtx->u2IF_Freq =C_TUNERA_IF_FREQUENCY_DVBC;
   }
   else if(Mode ==MOD_ISDBT)
   {
	pTCtx->u2IF_Freq =C_TUNERA_IF_FREQUENCY_ISDBT;
   }
   else if(Mode ==MOD_DTMB)
   {
	pTCtx->u2IF_Freq =C_TUNERA_IF_FREQUENCY_DTMB;
   }
   else  if( Mode == MOD_PAL_BG) 
   {
    if(Freq<300000000)
	 {
      pTCtx->u2IF_Freq_A =C_TUNERA_IF_FREQUENCY_PAL_B;
 	 }
    else
	 {
	  pTCtx->u2IF_Freq_A =C_TUNERA_IF_FREQUENCY_PAL_G;
	 }
   }
   else  if( Mode == MOD_PAL_DK) 
   {
	pTCtx->u2IF_Freq_A =C_TUNERA_IF_FREQUENCY_PAL_DK;
   }
   else  if( Mode == MOD_PAL_I) 
   {
	pTCtx->u2IF_Freq_A =C_TUNERA_IF_FREQUENCY_PAL_I;
   }
   else  if( Mode == MOD_SECAM_L) 
   {
	pTCtx->u2IF_Freq_A =C_TUNERA_IF_FREQUENCY_SECAM_L;
   }
   else  if( Mode == MOD_SECAM_L1) 
   {
	pTCtx->u2IF_Freq_A =C_TUNERA_IF_FREQUENCY_SECAM_L1;
   }
   else  if( Mode == MOD_NTSC_M) 
   {
	pTCtx->u2IF_Freq_A =C_TUNERA_IF_FREQUENCY_NTSC_M;
   }
   
      //TODO:write tuner here via i2c by sub system
	  
	  //End
 
 
	 //Loop check PLL lock
	 //polling PLL lock status
	for(ii=0;ii<C_VENDOR_TUNERA_PLL_POLL_CNT;ii++)
	{
		x_thread_delay(C_VENDOR_TUNERA_PLL_POLL_INTERVAL);
		if(TUNERA_TunerGetStatus(pTCtx))
			break;
	}
	
	if(ii<C_VENDOR_TUNERA_PLL_POLL_CNT)
	{
	 mcDBG_MSG_L2(("PLL lock at %d th time\n",ii));
	}
	else
	{
	 mcDBG_MSG_ERR(("PLL unlock !(%d)",ii));
	}
   
    x_thread_delay((UINT32)(10));//wait tuner output to become stable.Please modify the time according to tuner spec

    return (0);
}
//----------------------------------------------------------------------------- 
/** TUNERA_TunerGetVer
 *  Get Tuner type version
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval tuner model name
 */
//-----------------------------------------------------------------------------
static CHAR *TUNERA_TunerGetVer(VOID)
{
    return (C_VENDOR_TUNERA_STR);
	
}

//----------------------------------------------------------------------------- 
/** TUNERA_TunerGetVer
 *  Set saw bandwidth
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval saw bandwidth
 */
//-----------------------------------------------------------------------------
static VOID  TUNERA_SetSawBw(ITUNER_CTX_T * pTCtx, UINT8 sawbw)
{
 pTCtx->specific_member.eu_ctx.m_SAW_BW=sawbw;
 mcDBG_MSG_L2(("TUNERA_SetSawBw=%d\n",sawbw));

}
//----------------------------------------------------------------------------- 
/** TUNERA_TunerOP
 * Set/Get function
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  eOperation       To Get/Set operation id
 *  @retval  ITUNER_OK    Operation Success
 *  @retval  ITUNER_NOT_OK    Operation fail
 *  @retval  ITUNER_NOT_SUPPORTED Operation not supported in  driver
 */
//-----------------------------------------------------------------------------
INT16 TUNERA_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){
         
    switch(eOperation){
    case itGetVer://Get tuner driver ver
        {
        CHAR ** RetStr=(CHAR **)pInOutVal;
        *RetStr=TUNERA_TunerGetVer();
        }
        break;
	case itGetTunerRelReg://Get ATD Tuner Rel register setting
		{	  
		  *(ATD_TUNER_REL_REG_T **)pInOutVal=arrAtdRelReg; 
		}
		break;
    case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pTUNERAEqNormal[SetVal];
        break;
    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pTUNERAEqWeak[SetVal];
        break;
	case itSetSawBw:
		{
		TUNERA_SetSawBw(pTCtx,SetVal);
		}
		break;
    default:
        return ITUNER_NOT_SUPPORTED;
    }
    return ITUNER_OK;
}
