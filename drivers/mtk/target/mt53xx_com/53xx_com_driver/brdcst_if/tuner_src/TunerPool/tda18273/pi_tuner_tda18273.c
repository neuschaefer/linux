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
 * $RCSfile: pi_tuner_TDA18273.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_TDA18273.c
 *  Tuner control for NXP TDA18273
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "demod_tuner_interface.h"
//#include "pd_i2capi.h"


//*--------------------------------------------------------------------------------------
//* Include NXP Driver files
//*--------------------------------------------------------------------------------------
#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmbslFrontEndTypes.h"
#include "tmbslTDA18273.h"

#include "tuner_interface_if.h"
#include "fe_tuner_common_if.h"
#include "ctrl_bus.h"
#include "x_typedef.h"
#include "PD_Def.h"
#include "x_os.h"
#include "eq_script_tda18273.h"
//#include "pi_def_dvbt.h"
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
//Frequency boundary .Get from tuner spec
#define C_TDA18273_FREQ_DBOUND_UPPER   870*1000*1000     
#define C_TDA18273_FREQ_DBOUND_LOWER    42*1000*1000 
#define C_TDA18273_FREQ_DBOUND_UPPER_Ana     870*1000*1000
#define C_TDA18273_FREQ_DBOUND_LOWER_Ana    42*1000*1000

//PLL lock check parameters
#define C_NXP_TDA18273_PLL_POLL_INTERVAL      10 //ms
#define C_NXP_TDA18273_PLL_POLL_TIMETOUT      100 //ms
#define C_NXP_TDA18273_PLL_POLL_CNT           C_NXP_TDA18273_PLL_POLL_TIMETOUT/C_NXP_TDA18273_PLL_POLL_INTERVAL //counter
#define C_TDA18273_I2C_CLK_DIV         0x100
#define C_TDA18273_PLL_LOCK_BIT        6

#define NXP_TDA18273_ADDRESS           ((UINT8)  0xC0)
#define C_TDA18273_IF_FREQUENCY          ((UINT16)  4000)  /* kHz */
//#define C_TDA18273_IF_FREQUENCY_ANA      ((UINT16)  38900)  /* kHz */
//#define C_TDA18273_IF_FREQUENCY_L1       ((UINT16)  33900)  /* kHz */
#define C_TDA18273_LO_DIVIDER_STEP       ((UINT32) 166667)  /* Hz */
#define C_TDA18273_LO_DIVIDER_STEP_ANA   ((UINT16)  50000)  /* Hz */
#define C_NXP_TDA18273_STR                        "NXP_TDA18273"

#define C_TDA18273_IF_FREQUENCY_PAL_B    ((UINT16)  6400)  /* kHz */
#define C_TDA18273_IF_FREQUENCY_PAL_G    ((UINT16)  6750)  /* kHz */
#define C_TDA18273_IF_FREQUENCY_PAL_DK   ((UINT16)  6850)  /* kHz */
#define C_TDA18273_IF_FREQUENCY_PAL_I    ((UINT16)  7250)  /* kHz */
#define C_TDA18273_IF_FREQUENCY_NTSC_M   ((UINT16)  5400)  /* kHz */
#define C_TDA18273_IF_FREQUENCY_SECAM_L  ((UINT16)  6750)  /* kHz */
#define C_TDA18273_IF_FREQUENCY_SECAM_L1 ((UINT16)  1250)  /* kHz */
#define C_TDA18273_IF_FREQUENCY_DVBT_6M        ((UINT16)  3250)  /* kHz */  //JUST for 3.25M
#define C_TDA18273_IF_FREQUENCY_DVBT_7M        ((UINT16)  3500)  /* kHz */  //JUST for 3.5M
#define C_TDA18273_IF_FREQUENCY_DVBT_8M        ((UINT16)  4000)  /* kHz */  //JUST for 4M
#define C_TDA18273_IF_FREQUENCY_ATSC        ((UINT16)  3250)  /* kHz */  //JUST for 3.25M
#define C_TDA18273_IF_FREQUENCY_DVBC        ((UINT16)  5000)  /* kHz */  //JUST for 5M
#define C_TDA18273_IF_FREQUENCY_ATSC_64QAM  ((UINT16)  3600) /*kHz*/  


#define C_TDA18273_SAW_BW              SAW_BW_8M
#define C_TDA18273_TOP_SET_ANA         ((UINT8)   0x0A) 

#define C_TDA18273_CHANNEL_SCAN_JUMP_SMALL_STEP ((UINT16) 250)
#define C_TDA18273_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP ((UINT16) 1500)
#define C_TDA18273_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN ((UINT16) 500)

//MT5135 PRA 
#define C_TDA18273_AGC_IF_SLP             0.25   
#define C_TDA18273_AGC_IF_INT             0
#define C_TDA18273_AGC_IF_MAX             0.499
#define C_TDA18273_AGC_IF_MIN             -0.5

#define C_U8_TDA18273_AGC_IF_SLP          (S8)(C_TDA18273_AGC_IF_SLP *64 )
#define C_U8_TDA18273_AGC_IF_INT          (S8)(C_TDA18273_AGC_IF_INT *16 )
#define C_U8_TDA18273_AGC_IF_MAX          (S8)(C_TDA18273_AGC_IF_MAX *256)
#define C_U8_TDA18273_AGC_IF_MIN          (S8)(C_TDA18273_AGC_IF_MIN *256)

#define DAGC_CONFIGURE_INDEX  1
#define PRA_TARGET_LEVEL_H   2
#define PRA_TARGET_LEVEL_L   3

static UINT8* pTda18273[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
//static UINT8* pTda18273G[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN]= {NULL};



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

static ATD_TUNER_REL_REG_T arrAtdRelReg[]=
{
#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
  {0x13b,0x80},
  {0x790,0x08},//d110628_Haibo:PAL/NTSC 0x08,SECAM 0x88
  {0x794,0x2c},//d110819_Haibo:Modify DAGC setting for Si-tuner
  {0x7a4,0x2c},
  {0x245,0xdf},
  {0x7B7,0x10},//d110628_Haibo:PAL/NTSC 0x10,SECAM 0x05
  {0x7B9,0x10},
  {0x7d4,0x04},
  {0x7d5,0x00},
  {0x7d6,0x00},
  {0x7d7,0xec},
  {0x7d8,0xef},
  {0x7d9,0xef},
  {0x7da,0x7f},
  {0x7db,0x7f},
  {0x7dc,0x80},
  {0x7dd,0x80},
  {0x7de,0x80},
  {0x7df,0x11},
  {0x00,0x00}//end flag
#else
  {0x13b,0x80},
  {0x290,0x08},//d110628_Haibo:PAL/NTSC 0x08,SECAM 0x88
  {0x2B7,0x10},//d110628_Haibo:PAL/NTSC 0x10,SECAM 0x05
  {0x2B9,0x10},
  {0x2d4,0x04},
  {0x2d5,0x00},
  {0x2d6,0x00},
  {0x2d7,0xec},
  {0x2d8,0xef},
  {0x2d9,0xef},
  {0x2da,0x7f},
  {0x2db,0x7f},
  {0x2dc,0x80},
  {0x2dd,0x80},
  {0x2de,0x80},
  {0x2df,0x11},
  {0x00,0x00}//end flag
#endif
};

tmErrorCode_t UserWrittenI2CRead(tmUnitSelect_t tUnit,	UInt32 AddrSize, UInt8* pAddr,
UInt32 ReadLen, UInt8* pData);
tmErrorCode_t UserWrittenI2CWrite (tmUnitSelect_t tUnit, 	UInt32 AddrSize, UInt8* pAddr,
UInt32 WriteLen, UInt8* pData);
tmErrorCode_t UserWrittenWait(tmUnitSelect_t tUnit, UInt32 tms);
tmErrorCode_t UserWrittenPrint(UInt32 level, const char* format, ...);



/** TDA18273_TunerInit
 *  Tuner initialzation forTDA18273.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval void
 */

//-----------------------------------------------------------------------------
VOID TDA18273_TunerInit(
    ITUNER_CTX_T *pTCtx
    )
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);
	

    tmbslFrontEndDependency_t sSrvTunerFunc;

  	tmErrorCode_t err = TM_OK;	 
    psTunerCtx->I2cAddress = NXP_TDA18273_ADDRESS;
    psTunerCtx->u2IF_Freq = C_TDA18273_IF_FREQUENCY;
	//psTunerCtx->u2IF_Freq_A = C_TDA18273_IF_FREQUENCY_ANA;//analog if frequency for ATD
	psTunerCtx->u4RF_Freq = 0;
    psTunerCtx->u1AtdPatchSwitch =0x09;
    psTunerCtx->fgRFAGC = FALSE;//indicate if tuner need extern RF_AGC
	psTunerCtx->fgRFTuner = FALSE;//Digital tuner
	 
	 // AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x2f; 
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0xe3; 
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B; 

	pEUCtx->m_SAW_BW  = C_TDA18273_SAW_BW;
    pEUCtx->m_Ana_Top = C_TDA18273_TOP_SET_ANA;

	//Tuner frequency range
    pEUCtx->m_s4FreqBoundUpper = C_TDA18273_FREQ_DBOUND_UPPER;
    pEUCtx->m_s4FreqBoundLower = C_TDA18273_FREQ_DBOUND_LOWER;
    pEUCtx->m_s4AnaFreqBoundUpper = C_TDA18273_FREQ_DBOUND_UPPER_Ana;
    pEUCtx->m_s4AnaFreqBoundLower = C_TDA18273_FREQ_DBOUND_LOWER_Ana;

	//
	pEUCtx->m_Small_Step = C_TDA18273_CHANNEL_SCAN_JUMP_SMALL_STEP;
    pEUCtx->m_Middle_Step_Up = C_TDA18273_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP;
    pEUCtx->m_Middle_Step_Down = C_TDA18273_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN;
	
    //ATD patch selection
    psTunerCtx->u1AtdPatchSwitch =0x09;
    psTunerCtx->fgRFAGC = FALSE;//indicate if tuner need extern RF_AGC
	psTunerCtx->fgRFTuner =FALSE;//Digital tuner
	psTunerCtx->u1EqIndex=0;//EQ index select

	
     mcSHOW_USER_MSG(("Init " C_NXP_TDA18273_STR "(Cust tuner)\n"));
	
     pTda18273[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQTDA18273BG;
	 pTda18273[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQTDA18273DK;
	 pTda18273[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]   = EQTDA18273I;
	 pTda18273[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]   = EQTDA18273L;
	 pTda18273[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN]   = EQTDA18273L1;
	 pTda18273[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]   = EQTDA18273M;

  #if defined(CC_MT5396)	|| defined(CC_MT5368) || defined(CC_MT5389)
	 //5396
	 pEUCtx->m_aucPara[2]= (UINT8)C_U8_TDA18273_AGC_IF_SLP; 
	 pEUCtx->m_aucPara[3]= (UINT8)C_U8_TDA18273_AGC_IF_INT;
	 pEUCtx->m_aucPara[4]= (UINT8)C_U8_TDA18273_AGC_IF_MAX;
	 pEUCtx->m_aucPara[5]= (UINT8)C_U8_TDA18273_AGC_IF_MIN;
	 /*
	 pUSCtx->m_aucPara_SLD[2] = (UINT8)C_U8_TDA18273_AGC_IF_SLP; 
	 pUSCtx->m_aucPara_SLD[3] = (UINT8)C_U8_TDA18273_AGC_IF_INT;
	 pUSCtx->m_aucPara_SLD[4] = (UINT8)C_U8_TDA18273_AGC_IF_MAX;
	 pUSCtx->m_aucPara_SLD[5] = (UINT8)C_U8_TDA18273_AGC_IF_MIN;
	 */
#else
	 //AD Target set ,for 5365+5112ee
	 pEUCtx->m_aucPara[TUNER_FUNC_EN_TARGET_LEVEL] = TRUE;
	 pEUCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_MSB] = 0x23;   
	 pEUCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_LSB] = 0x00;  
#endif

   
   /* Driver layer struct set-up to link with user written functions */
   sSrvTunerFunc.sIo.Write             = UserWrittenI2CWrite;
   sSrvTunerFunc.sIo.Read              = UserWrittenI2CRead;
   sSrvTunerFunc.sTime.Get             = NULL;
   sSrvTunerFunc.sTime.Wait            = UserWrittenWait;
   sSrvTunerFunc.sDebug.Print          = UserWrittenPrint;
   sSrvTunerFunc.sMutex.Init           = NULL;
   sSrvTunerFunc.sMutex.DeInit         = NULL;
   sSrvTunerFunc.sMutex.Acquire        = NULL;
   sSrvTunerFunc.sMutex.Release        = NULL;
   sSrvTunerFunc.dwAdditionalDataSize  = 0;
   sSrvTunerFunc.pAdditionalData       = NULL;
   
   /* TDA18273 Driver initialization  */
   err = tmbslTDA18273_Open(0, &sSrvTunerFunc);
   
   if(err == TM_OK)
   {   
   		mcSHOW_USER_MSG(("tmbslTDA18273_Open OK\n"));
		/* TDA18273 Hardware initialization */
		err = tmbslTDA18273_HwInit(0);
   }
   else
		mcSHOW_USER_MSG(("tmbslTDA18273_Open failed\n"));
   	
   if(err == TM_OK)
   {
		/* TDA18273 standard mode selection */
		err = tmbslTDA18273_SetStandardMode(0, TDA18273_ANLG_DK);
   }
   else
		mcSHOW_USER_MSG(("tmbslTDA18273_HwInit failed\n"));
   if(err != TM_OK)
		mcSHOW_USER_MSG(("tmbslTDA18273_SetStandardMode failed\n"));
   	
	 
}

//----------------------------------------------------------------------------- 
/** TDA18273_TunerSetFreq
 *  Set Tuner PLL forTDA18273/TDA18273 to tune RF frequency.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param 	param    Pointer of structure of parameters
 *  @retval  0 success
                    1  fail   out of range 
                   -1 fail  I2C error 
 */
//-----------------------------------------------------------------------------
INT16 TDA18273_TunerSetFreq(
    ITUNER_CTX_T *pTCtx,
    PARAM_SETFREQ_T* param
    )
{
   UINT32 Freq = param->Freq*1000; // transfer to video freq(Hz)
   UINT8 Mode = param->Modulation;
   int ii;
   tmbslFrontEndState_t PLLLock = tmbslFrontEndStateUnknown;
   tmTDA18273StandardMode_t stdMode = TDA18273_ANLG_DK;
   Bool bEnabled=True;
  // TuningParam TuningParam;	
 //  TuningParam.m_BandWidth = pTCtx->specific_member.eu_ctx.m_SAW_BW;
   pTCtx->u4RF_Freq = Freq/1000;//Update current frequency to context
   mcSHOW_DBG_MSG((C_NXP_TDA18273_STR " Set Freq %d Hz\n",Freq));

   SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);
   
    //Check if frequency out of tuner LO range			
if (Freq < (C_TDA18273_FREQ_DBOUND_LOWER) || 
				Freq > (C_TDA18273_FREQ_DBOUND_UPPER))
	{
		mcSHOW_HW_MSG(("Out of range for LO!\n"));
		//return (+1);
	}

 if( Mode == MOD_ATSC_8VSB) //digital reception
   {
     stdMode = TDA18273_ATSC_6MHz;
	  pTCtx->u2IF_Freq =C_TDA18273_IF_FREQUENCY_ATSC;
	  mcSHOW_HW_MSG(("Mode=ATSC_8VSB u2IF_Freq=%d\n",pTCtx->u2IF_Freq));
   }		
 else if((Mode==MOD_J83B_64QAM)||(Mode==MOD_J83B_256QAM)||(Mode==MOD_J83B_AUTO_QAM))
   { stdMode=TDA18273_QAM_6MHz;
	 pTCtx->u2IF_Freq=C_TDA18273_IF_FREQUENCY_ATSC_64QAM;   //Set the Demod IF freq (KHz)
   }
 else if(Mode==MOD_DVBT)
   { 
 	//if(TuningParam.m_BandWidth == SAW_BW_7M)
 	if(pTunerCtx->m_SAW_BW==SAW_BW_6M)
	{ stdMode =TDA18273_DVBT_6MHz;
	 pTCtx->u2IF_Freq =C_TDA18273_IF_FREQUENCY_DVBT_6M;}
    else if(pTunerCtx->m_SAW_BW==SAW_BW_7M)
	{ stdMode =TDA18273_DVBT_7MHz;
	  pTCtx->u2IF_Freq =C_TDA18273_IF_FREQUENCY_DVBT_7M;}
	else 
	{ stdMode =TDA18273_DVBT_8MHz;
	  pTCtx->u2IF_Freq =C_TDA18273_IF_FREQUENCY_DVBT_8M;}
   }
 else if(Mode==MOD_DVBT2)
   { 
 	//if(TuningParam.m_BandWidth == SAW_BW_7M)
 	if(pTunerCtx->m_SAW_BW==SAW_BW_6M)
	{ stdMode =TDA18273_DVBT_6MHz;
	 pTCtx->u2IF_Freq =C_TDA18273_IF_FREQUENCY_DVBT_6M;}
    else if(pTunerCtx->m_SAW_BW==SAW_BW_7M)
	{ stdMode =TDA18273_DVBT_7MHz;
	  pTCtx->u2IF_Freq =C_TDA18273_IF_FREQUENCY_DVBT_7M;}
	else 
	{ stdMode =TDA18273_DVBT_8MHz;
	  pTCtx->u2IF_Freq =C_TDA18273_IF_FREQUENCY_DVBT_8M;}
   }
 
 else if(Mode ==MOD_DVBC)
   {
	stdMode = TDA18273_QAM_8MHz;
	pTCtx->u2IF_Freq =C_TDA18273_IF_FREQUENCY_DVBC;
   }
 else  if( Mode == MOD_DTMB) //digital reception
   {
	stdMode = TDA18273_DMBT_8MHz;
   }
 
 else  if( Mode == MOD_PAL_BG) 
   {
    if(Freq<300000000)
	 {
	  stdMode = TDA18273_ANLG_B;
      pTCtx->u2IF_Freq_A =C_TDA18273_IF_FREQUENCY_PAL_B;
 	 }
    else
	 {
	  stdMode = TDA18273_ANLG_GH;
	  pTCtx->u2IF_Freq_A =C_TDA18273_IF_FREQUENCY_PAL_G;
	 }
   }
 else  if( Mode == MOD_PAL_DK) 
   {
	stdMode = TDA18273_ANLG_DK;
	pTCtx->u2IF_Freq_A =C_TDA18273_IF_FREQUENCY_PAL_DK;

	if(tmbslTDA18273_SetInternalVsync(0,bEnabled)!=TM_OK);
	{
	mcSHOW_HW_MSG(("Set V_sync fail!\n"));
	}
   }
 else  if( Mode == MOD_PAL_I) 
   {
	stdMode = TDA18273_ANLG_I;
	pTCtx->u2IF_Freq_A =C_TDA18273_IF_FREQUENCY_PAL_I;
   }
 else  if( Mode == MOD_SECAM_L) 
   {
	stdMode = TDA18273_ANLG_L;
	pTCtx->u2IF_Freq_A =C_TDA18273_IF_FREQUENCY_SECAM_L;
   }
 else  if( Mode == MOD_SECAM_L1) 
   {
	stdMode = TDA18273_ANLG_LL;
	pTCtx->u2IF_Freq_A =C_TDA18273_IF_FREQUENCY_SECAM_L1;
   }
 else  if( Mode == MOD_NTSC_M) 
   {
	stdMode = TDA18273_ANLG_MN;
	pTCtx->u2IF_Freq_A =C_TDA18273_IF_FREQUENCY_NTSC_M;
   }
 
	if(tmbslTDA18273_SetStandardMode(0, stdMode) != TM_OK)
		{
		mcSHOW_HW_MSG(("SetStandardMode fail!\n"));
	//	return -1;
		}
   	if(tmbslTDA18273_SetRF(0, Freq) != TM_OK)
	{
		mcSHOW_HW_MSG(("Set tuner PLL 0x%02X fail!\n",pTCtx->I2cAddress));
	//	return -1;
	}
	else
	{
	 mcSHOW_DBG_MSG(("LO_%02X: %dHz\n",pTCtx->I2cAddress, param->Freq));
	 
	 //Loop check PLL lock
	 //polling PLL lock status
	for(ii=0;ii<C_NXP_TDA18273_PLL_POLL_CNT;ii++)
	{
		x_thread_delay(C_NXP_TDA18273_PLL_POLL_INTERVAL);
		tmbslTDA18273_GetLockStatus(0, &PLLLock);
		if(PLLLock == tmbslFrontEndStateLocked)
			break;
	}
	
	if(ii<C_NXP_TDA18273_PLL_POLL_CNT)
	{
	 mcSHOW_DBG_MSG(("PLL lock at %d th time\n",ii));
	}
	else
	{
	 mcSHOW_HW_MSG(("PLL unlock !(%d)",ii));
	}
	}
   
    x_thread_delay((UINT32)(10));

/*read ChipID
	{
		UInt8 Addr = 0;
		UInt8 Data[8];
	UserWrittenI2CRead(0,	1, &Addr,
3, Data);
	mcSHOW_HW_MSG(("CHIP ID is %x %x %x\n",Data[0],Data[1],Data[2]));
    	}
  */
    return (0);
}
//----------------------------------------------------------------------------- 
/** TDA18273_TunerGetVer
 *  Get Tuner type version
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval tuner model name
 */
//-----------------------------------------------------------------------------
static CHAR *TDA18273_TunerGetVer(VOID)
{
    return (C_NXP_TDA18273_STR);
}
//----------------------------------------------------------------------------- 
/** TDA18273_TunerOP
 * Set/Get function
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  eOperation       To Get/Set operation id
 *  @retval  ITUNER_OK    Operation Success
 *  @retval  ITUNER_NOT_OK    Operation fail
 *  @retval  ITUNER_NOT_SUPPORTED Operation not supported in  driver
 */
//-----------------------------------------------------------------------------
INT16 TDA18273_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){
         
    switch(eOperation){
    case itGetVer:
        {
        CHAR ** RetStr=(CHAR **)pInOutVal;
        *RetStr=TDA18273_TunerGetVer();
        }
        break;
	case itGetTunerRelReg://Get ATD Tuner Rel register setting
		{
          if((SetVal==MOD_SECAM_L) || (SetVal==MOD_SECAM_L1))
         {
		   arrAtdRelReg[DAGC_CONFIGURE_INDEX].u1RegVal=0x88;
		   arrAtdRelReg[PRA_TARGET_LEVEL_H].u1RegVal=0x05;
		   arrAtdRelReg[PRA_TARGET_LEVEL_L].u1RegVal=0x05;
         }
		 else
		 {
		    arrAtdRelReg[DAGC_CONFIGURE_INDEX].u1RegVal=0x08;
			arrAtdRelReg[PRA_TARGET_LEVEL_H].u1RegVal=0x10;
		    arrAtdRelReg[PRA_TARGET_LEVEL_L].u1RegVal=0x10;
		 }
		  *(ATD_TUNER_REL_REG_T **)pInOutVal=arrAtdRelReg;
		}
		break;

	case itGetEqScriptNormal: 
		 *(UINT8**)pInOutVal = pTda18273[SetVal];
		break;
    default:
        return ITUNER_OK;
    }

    return ITUNER_OK;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenI2CRead
//* Object              : 
//* Input Parameters    : 	tmUnitSelect_t tUnit
//* 						UInt32 AddrSize,
//* 						UInt8* pAddr,
//* 						UInt32 ReadLen,
//* 						UInt8* pData
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenI2CRead(tmUnitSelect_t tUnit,	UInt32 AddrSize, UInt8* pAddr,
UInt32 ReadLen, UInt8* pData)
{
    /* Variable declarations */


    /* Customer code here */
    /* ...*/
	if(ICtrlBus_I2cDemodRead(270,NXP_TDA18273_ADDRESS, *pAddr, pData, ReadLen) == 0)
		return TM_OK;
	else
		return !TM_OK;

    /* ...*/
    /* End of Customer code here */


}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenI2CWrite
//* Object              : 
//* Input Parameters    : 	tmUnitSelect_t tUnit
//* 						UInt32 AddrSize,
//* 						UInt8* pAddr,
//* 						UInt32 WriteLen,
//* 						UInt8* pData
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenI2CWrite (tmUnitSelect_t tUnit, 	UInt32 AddrSize, UInt8* pAddr,
UInt32 WriteLen, UInt8* pData)
{
    /* Variable declarations */

    /* Customer code here */
    /* ...*/
	if( ICtrlBus_I2cDemodWrite(270,NXP_TDA18273_ADDRESS, *pAddr, pData, WriteLen) == 0)
		return TM_OK;
	else
		return !TM_OK;
    /* ...*/
    /* End of Customer code here */

}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenWait
//* Object              : 
//* Input Parameters    : 	tmUnitSelect_t tUnit
//* 						UInt32 tms
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenWait(tmUnitSelect_t tUnit, UInt32 tms)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    /* Customer code here */
    /* ...*/
	x_thread_delay(tms);
    /* ...*/
    /* End of Customer code here */

    return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenPrint
//* Object              : 
//* Input Parameters    : 	UInt32 level, const char* format, ...
//* 						
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenPrint(UInt32 level, const char* format, ...)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    /* Customer code here */
    /* ...*/
	mcSHOW_HW_MSG(("%s\n",format));
    /* ...*/
    /* End of Customer code here */

    return err;
}


