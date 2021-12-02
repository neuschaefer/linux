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
 * $RCSfile: pi_tuner_TDVWH810F.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_TDVWH810F.c
 *  Tuner control for SS TDVWH810F
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "demod_tuner_interface.h"
//#include "pd_i2capi.h"
#include "tuner_interface_if.h"
#include "fe_tuner_common_if.h"
#include "i2c_api.h"
#include "x_typedef.h"
#include "tunerDebug.h"
#include "x_os.h"
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
//Frequency boundary .Get from tuner spec
#define C_TDVWH810F_FREQ_DBOUND_UPPER   859*1000*1000     
#define C_TDVWH810F_FREQ_DBOUND_LOWER    48*1000*1000 
#define C_TDVWH810F_FREQ_DBOUND_UPPER_Ana     875*1000*1000
#define C_TDVWH810F_FREQ_DBOUND_LOWER_Ana    45*1000*1000

//PLL lock check parameters
#define C_LG_TDVWH810F_PLL_POLL_INTERVAL      10 //ms
#define C_LG_TDVWH810F_PLL_POLL_TIMETOUT      100 //ms
#define C_LG_TDVWH810F_PLL_POLL_CNT           C_LG_TDVWH810F_PLL_POLL_TIMETOUT/C_LG_TDVWH810F_PLL_POLL_INTERVAL //counter

#define C_TDVWH810F_LO_ADDRESS           ((UINT8)  0xC2)
#define C_TDVWH810F_IF_FREQUENCY          ((UINT16)  36130)  /* kHz */
#define C_TDVWH810F_IF_FREQUENCY_ANA      ((UINT16)  38900)  /* kHz */
#define C_TDVWH810F_IF_FREQUENCY_L1       ((UINT16)  33900)  /* kHz */
#define C_TDVWH810F_LO_DIVIDER_STEP       ((UINT32) 166667)  /* Hz */
#define C_TDVWH810F_LO_DIVIDER_STEP_ANA   ((UINT16)  50000)  /* Hz */
#define C_LG_TDVWH810F_STR                        "LG_TDVWH810F"
//-----------------------------------------------------------------------------
// pi  constant definition
//-----------------------------------------------------------------------------
//-------------------------------------------------------------static  variable----------------------
//Signal Power Sample Array Size
//-----------------------------------------------------------------------------
// pi  constant definition
//-----------------------------------------------------------------------------
//-------------------------------------------------------------static  variable----------------------------------------------------------------------------------------------------------------------
static INT8 i1AnaTopArr[]=//Top range is from -12 to 4
{
-12,//Top for MOD_PAL_BG
-12,//Top for MOD_PAL_DK
-12,//Top for MOD_PAL_I
4,//Top for MOD_SECAM_L
0//Top for MOD_SECAM_L1
};
/** TDVWH810F_TunerInit
 *  Tuner initialzation forTDVWH810F.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval void
 */
//-----------------------------------------------------------------------------
VOID TDVWH810F_TunerInit(
    ITUNER_CTX_T *pTCtx
    )
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);
	 
    psTunerCtx->I2cAddress = C_TDVWH810F_LO_ADDRESS;
    psTunerCtx->u2IF_Freq = C_TDVWH810F_IF_FREQUENCY;
	psTunerCtx->u2IF_Freq_A = C_TDVWH810F_IF_FREQUENCY_ANA;//analog if frequency for ATD
    //ATD patch selection
    psTunerCtx->u1AtdPatchSwitch =0x09;
    psTunerCtx->fgRFAGC = TRUE;//indicate if tuner need extern RF_AGC
	 psTunerCtx->fgRFTuner = FALSE;//Digital tuner
	psTunerCtx->u1EqIndex=0;//EQ index select
	//Tuner frequency range
    pEUCtx->m_s4FreqBoundUpper = C_TDVWH810F_FREQ_DBOUND_UPPER;
    pEUCtx->m_s4FreqBoundLower = C_TDVWH810F_FREQ_DBOUND_LOWER;
    pEUCtx->m_s4AnaFreqBoundUpper = C_TDVWH810F_FREQ_DBOUND_UPPER_Ana;
    pEUCtx->m_s4AnaFreqBoundLower = C_TDVWH810F_FREQ_DBOUND_LOWER_Ana;
	
     mcDBG_MSG_INFO(("Init " C_LG_TDVWH810F_STR "(Cust tuner)\n"));
}


//----------------------------------------------------------------------------- 
/**  TDVWH810F_TunerGetStatus
 *  Set Tuner PLL staus forTDVWH810F/TDVWH810F
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval TRUE        PLL lock
 *  @retval FALSE       PLL unlock
 */
//-----------------------------------------------------------------------------
static BOOL TDVWH810F_TunerGetStatus(ITUNER_CTX_T *pTCtx)
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;

    UINT8 u1PllSts;

  if(ucI2cReadOnly(psTunerCtx->I2cAddress, &u1PllSts, 1))
    {
        mcDBG_MSG_ERR(("Tuner read failed!\n"));
        return FALSE;
    }

    if (u1PllSts&0x40)//Bit 6 is FL
        return TRUE;
    return FALSE;
}

//----------------------------------------------------------------------------- 
/** TDVWH810F_TunerSetFreq
 *  Set Tuner PLL forTDVWH810F/TDVWH810F to tune RF frequency.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param 	param    Pointer of structure of parameters
 *  @retval  0 success
                    1  fail   out of range 
                   -1 fail  I2C error 
 */
//-----------------------------------------------------------------------------
INT16 TDVWH810F_TunerSetFreq(
    ITUNER_CTX_T *pTCtx,
    PARAM_SETFREQ_T* param
    )
{
   UINT32 Freq = param->Freq; // transfer to video freq(KHz)
   UINT8 Mode = param->Modulation;
   UINT16 Ndivider;
   UINT8 CB1,BB,CB2,ii;
   UINT8 TunerS[5];
   UINT32 Lo;
   
   pTCtx->u4RF_Freq = Freq;//Update current frequency to context
   mcDBG_MSG_L2((C_LG_TDVWH810F_STR " Set Freq %d KHz\n",Freq));
   
    //Check if frequency out of tuner LO range			
	if (Freq < (C_TDVWH810F_FREQ_DBOUND_LOWER/1000) || 
				Freq > (C_TDVWH810F_FREQ_DBOUND_UPPER/1000))
	{
		mcDBG_MSG_ERR(("Out of range for LO!\n"));
		//return (+1);
	}
			
   if( Mode == MOD_DTMB) //digital reception
   {
     //To Do
	 //Calculate tuner LO divider,Band switch control word
	 //Here just a example
	 Lo = Freq + C_TDVWH810F_IF_FREQUENCY;
	 Ndivider = (UINT16)( (Lo*1000+C_TDVWH810F_LO_DIVIDER_STEP/2) /C_TDVWH810F_LO_DIVIDER_STEP);
	 CB1=0X98;
	 CB2=0XC1;
	 
	 if(Freq<161000)
	 {
	  BB=0x01;
	 }
	 else if(Freq<426000)
	 {
	  BB=0x02;
	 }
	 else
	 {
	 BB=0x08;
	 } 
   }
   else if(Mode ==MOD_DVBC)
   {
     //To Do
	 //Calculate tuner LO divider,Band switch control word
	  Lo = Freq + C_TDVWH810F_IF_FREQUENCY;
	 Ndivider = (UINT16)( (Lo*1000+C_TDVWH810F_LO_DIVIDER_STEP/2) /C_TDVWH810F_LO_DIVIDER_STEP);
	 CB1=0X9A;
	 CB2=0XC1;
	 
	 if(Freq<161000)
	 {
	  BB=0x01;
	 }
	 else if(Freq<426000)
	 {
	  BB=0x02;
	 }
	 else
	 {
	 BB=0x08;
	 } 
   }
   else //analog 
   {
   //Check if frequency out of tuner analog LO range			
	if (Freq < (C_TDVWH810F_FREQ_DBOUND_LOWER_Ana/1000) || 
				Freq > (C_TDVWH810F_FREQ_DBOUND_UPPER_Ana/1000)  )
	{
		mcDBG_MSG_ERR(("Out of range for LO!\n"));
		//return (+1);
	}
	 Lo = Freq + C_TDVWH810F_IF_FREQUENCY;
	 Ndivider = (UINT16)( (Lo*1000+C_TDVWH810F_LO_DIVIDER_STEP/2) /C_TDVWH810F_LO_DIVIDER_STEP);
	 CB1=0X98;
	 CB2=0XC1;
	 
	 if(Freq<161000)
	 {
	  BB=0x01;
	 }
	 else if(Freq<426000)
	 {
	  BB=0x02;
	 }
	 else
	 {
	 BB=0x08;
	 } 
   if(Mode == MOD_SECAM_L1 )
   {
    //To Do
	//Do someting spectial
   }
   else
   {
   // To Do
   //Do something special
   }
   }
        TunerS[0] = (UINT8) 0x7F & (Ndivider >> 8);
        TunerS[1] = (UINT8) 0xFF & Ndivider;
        TunerS[2] = (UINT8) CB1;
        TunerS[3] = (UINT8) BB;
        TunerS[4] = (UINT8) CB2;
		
    //Write control word to tuner LO
	if(ucI2cWriteOnly(pTCtx->I2cAddress,TunerS,sizeof(TunerS)/sizeof(TunerS[0])))//I2C fail
	{
	mcDBG_MSG_ERR(("Write tuner PLL 0x%02X fail!\n",pTCtx->I2cAddress));
	return -1;
	}
	else
	{
	 mcDBG_MSG_L2(("LO_%02X: (%d) %02X-%02X-%02X-%02X-%02X(%dKHz)\n",pTCtx->I2cAddress, sizeof(TunerS)/sizeof(TunerS[0]), TunerS[0], TunerS[1], TunerS[2], TunerS[3],TunerS[4],pTCtx->u4RF_Freq));
	 
	 //Loop check PLL lock
	 //polling PLL lock status
	
	for(ii=0;ii<C_LG_TDVWH810F_PLL_POLL_CNT;ii++)
	{
	  x_thread_delay(C_LG_TDVWH810F_PLL_POLL_INTERVAL);
	  if(TDVWH810F_TunerGetStatus(pTCtx))break;
	}
	
	if(ii<C_LG_TDVWH810F_PLL_POLL_CNT)
	{
	 mcDBG_MSG_L2(("PLL lock at %d th time\n",ii));
	}
	else
	{
	 mcDBG_MSG_ERR(("PLL unlock !(%d)",ii));
	}
	}
   
    x_thread_delay((UINT32)(10)); 

    return (0);
}
//----------------------------------------------------------------------------- 
/** TDVWH810F_TunerGetVer
 *  Get Tuner type version
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval tuner model name
 */
//-----------------------------------------------------------------------------
static CHAR *TDVWH810F_TunerGetVer(VOID)
{
    return (C_LG_TDVWH810F_STR);
}
//----------------------------------------------------------------------------- 
/** TDVWH810F_TunerOP
 * Set/Get function
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  eOperation       To Get/Set operation id
 *  @retval  ITUNER_OK    Operation Success
 *  @retval  ITUNER_NOT_OK    Operation fail
 *  @retval  ITUNER_NOT_SUPPORTED Operation not supported in  driver
 */
//-----------------------------------------------------------------------------
INT16 TDVWH810F_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){
         
    switch(eOperation){
    case itGetVer:
        {
        CHAR ** RetStr=(CHAR **)pInOutVal;
        *RetStr=TDVWH810F_TunerGetVer();
        }
        break;
	case itGetTop:
	    {
          UINT8 subSysId = SetVal;
		  if((subSysId<MOD_PAL_BG) || (subSysId>MOD_SECAM_L1))
		  {
		   mcDBG_MSG_ERR(("invalid sub-sys index %d\n",subSysId));
		  }
    	  *(INT8 *)pInOutVal=i1AnaTopArr[subSysId-MOD_PAL_BG];
         }
     break;	
    case itSetTop:
     {	
          UINT8 subSysId = SetVal;
		  if((subSysId<MOD_PAL_BG) || (subSysId>MOD_SECAM_L1))
		  {
		   mcDBG_MSG_ERR(("invalid sub-sys index %d\n",subSysId));
		  }
    	 i1AnaTopArr[subSysId-MOD_PAL_BG]= *(INT8 *)pInOutVal;
     }
	 break;
    default:
        return ITUNER_NOT_SUPPORTED;
    }

    return ITUNER_OK;
}
