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
 * $RCSfile: piana_TDVRH751F.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "PI_ATuner.h"
#include "PI_Def.h"
#include "pi_anana_if.h"//For Driver Auto Test log macro define
#include "piana_tdvrh751f.h"
//#include "tvsys_info.h"
#include "pd_anana_glue.h"
#include "tuner_interface_if.h"
//#include "ctrl_bus.h"
#include "PD_Def.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

/*************************************************************
  These values are according to the tuner module spec. On the
  demo board, we uses 39K & 10K Ohm resisters to divide the AFT
  output voltage and use 4/5 of original AFT output voltage range to
  fit into servo ADC voltage range (0 ~ 2.8V).

  So the AFT output is re-mapping from 0 ~ 3.5V -> 0 ~ 2.8V,
  that means 0 ~ 3.5V -> 0 ~ 0xff (Servo ADC status).
*************************************************************/

#define TDVRH751F_AFT_CENTER_VOL_HIGH       0x80//0x70//0xB8//0xA8       // 3.5V  ray modified
#define TDVRH751F_AFT_CENTER_VOL_LOW        0x20//0x15//0x48//0x4A        // 1.5V  ray modified

//#define TDVRH751F_AFT_CENTER_VOL_FINE_TUNE_HIGH_40KHZ 0xA4//-32KHz 
//#define TDVRH751F_AFT_CENTER_VOL_FINE_TUNE_LOW_40KHZ  0x80//32KHz 

#define TDVRH751F_AFT_CENTER_VOL        0x50//0x42//0x7C

#define TDVRH751F_AFT_CENTER_VOL_HIGH_MONI       0x90//0x7A        // 3.5V  ray modified
#define TDVRH751F_AFT_CENTER_VOL_LOW_MONI        0x15//0x10         // 1.5V  ray modified

/***********************************************************************/
//#define cANA_TUNER_IF_FREQ      45750
//#define cANA_TUNER_LO_STEP      62500

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------
static UINT32 PreLockFreq;
//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
//todo: pdwnc module should export _if.h
EXTERN UINT32 PDWNC_ReadServoADCChannelValue(UINT32 u4Channel);
//todo: pi_tuner.h
//#ifndef PLUGGABLE_DEMOD_DESIGN
//EXTERN INT16 Tuner_SetFreq(ITUNER_CTX_T *pTCtx,PARAM_SETFREQ_T* param);
//EXTERN BOOL Tuner_GetStatus(ITUNER_CTX_T *pTCtx);
//EXTERN VOID *GetUSTunerCtx(VOID);
//#endif
//todo: PD_ATUNER.h
EXTERN  VOID    DigTunerBypassI2C(BOOL bSwitchOn);
//VOID *GetDigiTunerCtx(VOID);
EXTERN BOOL fgEnableATP;//Flag to check whether show  Driver ATP Auto Test log ,define in pd_anana_glue.c line 872
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
/*
 *  ReadServoAD funtion
 *  Get Servo AD value
 *  @param  void 
 *  @retval   range: 0x00-0xFF.
 */
static UINT32 ReadServoAD(VOID)
{
UINT32 u4Value;
    u4Value = PDWNC_ReadServoADCChannelValue(AFT_AD_NUM);    
    #if defined(CC_MT5360)  || defined(CC_MT5363)
    // 6 bits, return bit5-0
    u4Value = u4Value << 2;
    if (u4Value >= 0xFC)
    	u4Value = 0xFF;
    #endif
    return u4Value;
}

/*
 *  bGetSCurveVol funtion
 *  Get the voltage of AFT S-curve
 *  @param  void 
 *  @retval   the voltage of AFT S-curve (0~255  =>  0V~3.5V).
 */
static UINT8 bGetSCurveVol(void)
{
UINT8 i;
UINT8 bCurveStatus, bCurveOld;
INT8 bCurveDiff;

    mcDELAY_MS(10);                         //change for one channel lost;0910
    //bCurveOld = PDWNC_ReadServoADCChannelValue(AFT_AD_NUM);  //ServoADCNum for Tuner S-curve  :ADC IN3
    bCurveOld = ReadServoAD();
    
    for(i=0; i<20;i++)
    {
        mcDELAY_MS(10);
        //bCurveStatus = PDWNC_ReadServoADCChannelValue(AFT_AD_NUM);
        bCurveStatus = ReadServoAD();
        bCurveDiff = (INT8)(bCurveStatus - bCurveOld);
        //LogSB("PDWNC_ReadServoADCChannelValue",bCurveStatus);
        //LogB(bCurveOld);
        if ((bCurveDiff > -5)&&(bCurveDiff < 5))
        {
            mcSHOW_DBG_MSG3(("bCurveStatus = %02X (%d)\n", bCurveStatus, bCurveStatus));
            return bCurveStatus;
        }
        bCurveOld = bCurveStatus;
    }

  return 0xFF;
}

//-----------------------------------------------------------------------------
// public functions
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------- 
/*
 *  TDVRH751F_AnaTunerInit
 *  Tuner Driver Initial FUnction
 *  @param  psAtvPiCtx  a type pointer use to point to struct ATV_PI_CTX_T.
 *  @param  IFDEMOD_I2c_Addr	a I2C address of DEMOD.
 *  @param  RF_I2c_Addr  a I2C address of RF Tuner.
 *  @retval   TRUE : Success.
 */
//-----------------------------------------------------------------------------
BOOL TDVRH751F_AnaTunerInit(ATV_PI_CTX_T *psAtvPiCtx, UINT8 IFDEMOD_I2c_Addr, UINT8 RF_I2c_Addr)
{
//ATV_PI_CTX_T *pTDVRH751F_AnaTuner = NULL;
ATV_PI_CTX_T *pTDVRH751F_AnaTuner = psAtvPiCtx;

//    pTDVRH751F_AnaTuner = (ATV_PI_CTX_T *) mcALLOC_MEM(sizeof(ATV_PI_CTX_T));
    if (pTDVRH751F_AnaTuner == NULL)
        return FALSE;
//    pTDVRH751F_AnaTuner->u1IF_I2cAddr = IFDEMOD_I2c_Addr;
//    pTDVRH751F_AnaTuner->u1RF_I2cAddr = RF_I2c_Addr;
//    pTDVRH751F_AnaTuner->u1IF_I2cAddr = TUNER_ONE_ADDR_IF;
//#if (fcNEW_ANA_PIPD_ARCH == 2)
//#ifndef PLUGGABLE_DEMOD_DESIGN
  //  pTDVRH751F_AnaTuner->pDigiTunerCtx = GetUSTunerCtx();
//#else
  //  pTDVRH751F_AnaTuner->pDigiTunerCtx = ITunerGetCtx();
//#endif
   // pTDVRH751F_AnaTuner->pDigiTunerCtx = GetDigiTunerCtx();
//#endif
    pTDVRH751F_AnaTuner->u1RF_I2cAddr = TUNER_ONE_ADDR_TUNER;
    pTDVRH751F_AnaTuner->u2IF_Freq = TDVRH751F_ANA_PIC_IF;
    pTDVRH751F_AnaTuner->u2LO_Step = TDVRH751F_ANA_FREQ_STEP;
//    pTDVRH751F_AnaTuner->u4Frequency = TDVRH751F_ANA_INIT_FREQ;
    pTDVRH751F_AnaTuner->u1DEMOD_state = TDVRH751F_ANA_UNLOCK;
    pTDVRH751F_AnaTuner->u1AnaMode = MOD_ANA_CABLE;
    mcSHOW_USER_MSG(("anana init finished: TDVRH751F\r\n"));
    return TRUE;
}

//----------------------------------------------------------------------------- 
/*
 *  TDVRH751F_AnaTunerClose
 *  Tuner driver close FUnction
 *  @param  psAtvPiCtx  a type pointer use to point to struct ATV_PI_CTX_T.
 *  @retval   void
 */
//-----------------------------------------------------------------------------
void TDVRH751F_AnaTunerClose(ATV_PI_CTX_T *psAtvPiCtx)
{
//    mcFREE_MEM(psAtvPiCtx);
}

//----------------------------------------------------------------------------- 
/*
 *  TDVRH751F_AnaTunerSetFreq
 *  Set the frequency into tuner part.
 *  @param  psAtvPiCtx  	a type pointer use to point to struct ATV_PI_CTX_T.
 *  @param  wFreqInKHz	aFrequency value in KHz.
 *  @param  u1AnaMode	Cable/Air Mode.
 *  @retval   TRUE : I2C programming successful.
 *  @retval   FALSE : FALSE => I2C programming fail 
 */
//-----------------------------------------------------------------------------
BOOL  TDVRH751F_AnaTunerSetFreq(ATV_PI_CTX_T *psAtvPiCtx,PARAM_SETFREQ_T * param)
{
BOOL fgRetSts = TRUE;
ATV_PI_CTX_T *pTDVRH751F_AnaTuner = psAtvPiCtx;

    pTDVRH751F_AnaTuner->u1AnaMode =param->Modulation;
    pTDVRH751F_AnaTuner->u4Freq =param->Freq;

    if (ITuner_SetFreq(ITunerGetCtx(),param) != 0)
 {
     fgRetSts = FALSE;
 }
    else//retry
 {
  ITuner_OP(ITunerGetCtx(),itGetStatus,0,&fgRetSts);
  if(!fgRetSts){
    if(ITuner_SetFreq(ITunerGetCtx(),param) != 0){
         fgRetSts = FALSE;
      }
  }
  }
  return fgRetSts;
}


//----------------------------------------------------------------------------- 
/*
 *  TDVRH751F_GetAnaTunerVer
 *  Get Tuner Version.
 *  @param  psAtvPiCtx  	a type pointer use to point to struct ATV_PI_CTX_T.
 *  @retval   TDVRH751F_ANA_TUNER_VER.
 */
//-----------------------------------------------------------------------------
#if 0
char *TDVRH751F_GetAnaTunerVer(ATV_PI_CTX_T *psAtvPiCtx)
{
//ATV_PI_CTX_T *pTDVRH751F_AnaTuner = psAtvPiCtx;

//    return (pTDVRH751F_AnaTuner->aSW_Ver_HW_Ver);
    return TDVRH751F_ANA_TUNER_VER;
}
#endif

//----------------------------------------------------------------------------- 
/*
 *  fgDrvTunerSetSystem
 *  Get the sound system of given sub system into tuner HW
 *  @param  psAtvPiCtx  	a type pointer use to point to struct ATV_PI_CTX_T.
 *  @retval   Sub system index.
 */
//-----------------------------------------------------------------------------
UINT8 TDVRH751F_TunerGetSystem(ATV_PI_CTX_T *psAtvPiCtx)
{
ATV_PI_CTX_T *pTDVRH751F_AnaTuner = psAtvPiCtx;

    return pTDVRH751F_AnaTuner->u1SubSysIdx;
}

//----------------------------------------------------------------------------- 
/*
 *  fgDrvTunerSetSystem
 *  Set the sound system of given sub system into tuner HW
 *  @param  psAtvPiCtx  	a type pointer use to point to struct ATV_PI_CTX_T.
 *  @param  u1SubSysIdx  	set a sub system type.
 *  @retval   TRUE : Set Successful.
 */
//-----------------------------------------------------------------------------
BOOL TDVRH751F_TunerSetSystem(ATV_PI_CTX_T *psAtvPiCtx, UINT8 u1SubSysIdx)
{
ATV_PI_CTX_T *pTDVRH751F_AnaTuner = psAtvPiCtx;
//mcLINT_UNUSED(u1SubSysIdx);

    pTDVRH751F_AnaTuner->u1SubSysIdx = u1SubSysIdx;
    return TRUE;
//Gene Chang Because the Samsung tuner doesn't set IF part. 2006.04.18
}

//----------------------------------------------------------------------------- 
/*
 *  TDVRH751F_bDrvTunerCheckVIFLock
 *  Check if programmed frequency has strong VIF signal nearby
 *  @param  psAtvPiCtx  	a type pointer use to point to struct ATV_PI_CTX_T.
 *  @param  wFreqInKHz  	The frequenccy in KHz need to check the VIF status
 *  @retval   	 2  => Current frequency is very close to the nominal VIF.
           		 1  => Current frequency is is near the nominal VIF.
           		 0  => Current frequency is not close to the nominal VIF yet.
 */
//-----------------------------------------------------------------------------
UINT8 TDVRH751F_bDrvTunerCheckVIFLock(ATV_PI_CTX_T *psAtvPiCtx, PARAM_SETFREQ_T * param)
{
//BOOL fgRetSts;
ATV_PI_CTX_T *pTDVRH751F_AnaTuner = psAtvPiCtx;
//UINT8 u1AnaMode = pTDVRH751F_AnaTuner->u1AnaMode;
UINT8 bAFTStatus, bPreAFTStatus;
UINT8 bDiffPreAFT;
//UINT8 bNextAFTStatus, bPreAFTStatus;
UINT32 Freq=param->Freq;
param->Freq-=100;    //this value is Porting from 5360,origin is 60
    
    
    
//    fgRetSts = TDVRH751F_AnaTunerSetFreq(psAtvPiCtx, wFreqInKHz - cFREQ_FINETUNE_RANGE,u1AnaMode);
//    bPreAFTStatus = bGetSCurveVol();
    TDVRH751F_AnaTunerSetFreq(psAtvPiCtx,param);
    bPreAFTStatus = bGetSCurveVol();

//    fgRetSts &= TDVRH751F_AnaTunerSetFreq(psAtvPiCtx, wFreqInKHz ,u1AnaMode);
    param->Freq=Freq;
    TDVRH751F_AnaTunerSetFreq(psAtvPiCtx,param);
    bAFTStatus = bGetSCurveVol();
    
#if 0
    if ((bAFTStatus <= TDVRH751F_AFT_CENTER_VOL_HIGH) && (bAFTStatus >= TDVRH751F_AFT_CENTER_VOL_LOW) && (bPreAFTStatus == 0xff))
    { 
	TDVRH751F_AnaTunerSetFreq(psAtvPiCtx, wFreqInKHz + 100 ,u1AnaMode);
    	bPreAFTStatus = bAFTStatus;
    	bAFTStatus = bGetSCurveVol();
    	mcSHOW_DBG_MSG((" (+100) bAFTStatus, bPreAFTStatus,%02X,%02X\n", bAFTStatus, bPreAFTStatus));
    }
#endif

    if (bPreAFTStatus > bAFTStatus)
        bDiffPreAFT = bPreAFTStatus - bAFTStatus;
    else
        bDiffPreAFT = 0;

    mcSHOW_DBG_MSG(("bAFTStatus, bPreAFTStatus, bDiffPreAFT %02X,%02X,%02X\n", bAFTStatus, bPreAFTStatus, bDiffPreAFT ));

if ((bAFTStatus <= TDVRH751F_AFT_CENTER_VOL_HIGH) && (bAFTStatus >= TDVRH751F_AFT_CENTER_VOL_LOW)
      && (bDiffPreAFT > 0x10))//&&(bPreAFTStatus!=0xff))
    {
    //mcSHOW_USER_MSG(("VIFLock == 2\n"));//EEE
        mcSHOW_DBG_MSG(("CheckVIFLock-return 2 (2 means VIF lock)\n"));
    // Current Freq is very close to the nominal VIF, don't tune the freq anymore.
        pTDVRH751F_AnaTuner->u1DEMOD_state = TDVRH751F_ANA_LOCK;
//  mcSHOW_USER_MSG(("AFT Level %d\n",bAFTStatus));
//  mcSHOW_USER_MSG(("AFT Diff %d\n",bDiffPreAFT));
        return 2;
    }
    //else if(((bAFTStatus < TDVRH751F_AFT_CENTER_VOL_LOW)&&(bPreAFTStatus!=0xff))||(bAFTStatus < TDVRH751F_AFT_CENTER_VOL_HIGH))
    else if((bAFTStatus < TDVRH751F_AFT_CENTER_VOL_LOW)&&(bPreAFTStatus!=0xff))
    {
    //mcSHOW_USER_MSG(("VIFLock == 1\n"));//EEE
        mcSHOW_DBG_MSG(("CheckVIFLock-return 1 (1 means current freq > signal freq)\n"));
        pTDVRH751F_AnaTuner->u1DEMOD_state = TDVRH751F_ANA_UNLOCK;
        return 1;
    }
    else
    {
    //mcSHOW_USER_MSG(("VIFLock == 0\n"));//EEE
        mcSHOW_DBG_MSG(("CheckVIFLock-return 0 (0 means current freq < signal freq and far from signal freq)\n"));
       pTDVRH751F_AnaTuner->u1DEMOD_state = TDVRH751F_ANA_UNLOCK;
        return 0;
    }
// Fine tune part Gene Chang
}

//----------------------------------------------------------------------------- 
/*
 *  TDVRH751F_AnaTunerSearchNearbyFreq
 *  Find the freq. value that near center frequency of the channel.
 *  @param  psAtvPiCtx  	a type pointer use to point to struct ATV_PI_CTX_T.
 *  @param  bScanType		AUTO_FINE_TUNE, UNI_AUTO_SCAN
 *  @param  *wFreqInKHz  	THe point of frequenccy in KHz need to check the VIF status
 *  @param  _BreakFct  	 	The break function
 *  @param  *pvArg  		The argument of break function
 *  @retval   TRUE  =>  center freq. found
            	    FALSE =>  center freq. not found (no signal on this channel)
 */
//-----------------------------------------------------------------------------
UINT8 TDVRH751F_AnaTunerSearchNearbyFreq(ATV_PI_CTX_T *psAtvPiCtx, UINT32 *wFreqInKHz,
                                       x_break_fct _BreakFct, void *pvArg)
{
ATV_PI_CTX_T *pTDVRH751F_AnaTuner = psAtvPiCtx;
UINT8 u1AnaMode = 0;
UINT8 bAFTStatus, bPreAFTStatus;
UINT8 bMaxStepNum = 0;
UINT32 wFRTmp;
UINT8 bVIFStatus = 0;
UINT8 bTVDFlag = 0;
UINT8 bATVStatus = 0;
UINT32 wFRTmpScan = 0;
UINT32 wFRTmpPre = 0;
UINT32 wFreqInKHzTmp = 0;
UINT8 bTVDCheckFlag1, bTVDCheckFlag2,bPreAFTStatusCheck;

#if 0
UINT8 bWeakSignalTVDFlag = 0;
#endif
PARAM_SETFREQ_T param;
mcSHOW_DBG_MSG3(("Start TDVRH751F_serach & Mod = %d\n",u1AnaMode));
    
    if (pTDVRH751F_AnaTuner == NULL)
        return (FALSE);

    if (wFreqInKHz != NULL)
    {
        wFRTmp = *wFreqInKHz;
    }
    else
    {
        return (bATVStatus);
    }
    pTDVRH751F_AnaTuner->isBreak = _BreakFct;
    u1AnaMode = pTDVRH751F_AnaTuner->u1AnaMode;
    param.Freq=wFRTmp;
    param.Modulation=u1AnaMode;
    param.fgAutoSearch=1;
    wFreqInKHzTmp = *wFreqInKHz;
   
    bVIFStatus = TDVRH751F_bDrvTunerCheckVIFLock(psAtvPiCtx, &param);


#if 0	
// In the Weak Signal, the S Curve can't not work, so the check TVD lock again.
   if(bVIFStatus !=2)
      {        
          bTVDFlag = fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME);
          if (bTVDFlag)
           {
                mcSHOW_DBG_MSG(("tvd success0_std (try again to avoid vif can't lock in weak signal)\n"));
                mcSET_BIT(bWeakSignalTVDFlag, 0);
            }
	   else
	   	mcSHOW_DBG_MSG(("tvd fail (try again to avoid vif can't lock in weak signal)\n"));
       }
#endif

#if fcTIMING_MEASURE
    mcSHOW_DBG_MSG4(("[ATuner] SearchNearby.0=%3u ms, %d\n",
                    (mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD(),
                    bVIFStatus));
#endif

// for HRC
    if (bVIFStatus == 2)
    {
        mcSET_BIT(bATVStatus, cpANA_LOCK_STS_VIF);
            bTVDFlag = fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME);
            if (bTVDFlag)
            {
            mcSHOW_DBG_MSG(("tvd success_std\n"));

           // mcSHOW_DBG_MSG(("ana MOD= %d\n",(IO_READ8(0x20022080,0x03)>>4)&0x07));
			
            }
            else
            {
                bVIFStatus = 0;
            }
    }

  if ( u1AnaMode != MOD_ANA_TERR)
  {
    if (bVIFStatus != 2)
    {
        bVIFStatus=0;
        param.Freq=wFRTmp-1250;
        bVIFStatus = TDVRH751F_bDrvTunerCheckVIFLock(psAtvPiCtx,&param);
#if fcTIMING_MEASURE
        mcSHOW_DBG_MSG(("[ATuner] SearchNearby.HRC1=%3u ms, %d\n",
                        (mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD(),
                        bVIFStatus));
#endif

        if (bVIFStatus == 2)
        {
            *wFreqInKHz = (wFRTmp-1250);
            mcSET_BIT(bATVStatus, cpANA_LOCK_STS_VIF);
        }
            if (bVIFStatus == 2)
            {
                    bTVDFlag = fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME);
                    if (bTVDFlag)
                    {
                    mcSHOW_DBG_MSG(("tvd success_HRC1\n"));
                    }
                    else
                    {
                        bVIFStatus = 0;
                    }
            }
    }

// for HRC
     if (bVIFStatus != 2 & ((wFreqInKHzTmp == 77250) | (wFreqInKHzTmp == 83250)))
    {
        bVIFStatus=0;
        param.Freq= wFRTmp+750;
        
        bVIFStatus = TDVRH751F_bDrvTunerCheckVIFLock(psAtvPiCtx,&param);
#if fcTIMING_MEASURE
        mcSHOW_DBG_MSG(("[ATuner] SearchNearby.HRC2=%3u ms, %d\n",
                        (mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD(),
                        bVIFStatus));
#endif

        if (bVIFStatus == 2)
        {
            *wFreqInKHz = (wFRTmp+750);
            mcSET_BIT(bATVStatus, cpANA_LOCK_STS_VIF);
        }
            if (bVIFStatus == 2)
            {
                    bTVDFlag = fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME);
                    if (bTVDFlag)
                    {
                    mcSHOW_DBG_MSG(("tvd success_HRC2\n"));
                    }
                    else
                    {
                        bVIFStatus = 0;
                    }
            }
    }

// for IRC
    if (bVIFStatus != 2 & ((wFreqInKHzTmp == 77250) | (wFreqInKHzTmp == 83250)))
    {
        bVIFStatus=0;
        param.Freq=wFRTmp+2000;
        
        bVIFStatus = TDVRH751F_bDrvTunerCheckVIFLock(psAtvPiCtx, &param);
#if fcTIMING_MEASURE
        mcSHOW_DBG_MSG(("[ATuner] SearchNearby.IRC=%3u ms, %d\n",
                        (mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD(),
                        bVIFStatus));
#endif

        if (bVIFStatus == 2)
        {
            *wFreqInKHz = (wFRTmp + 2000);
            mcSET_BIT(bATVStatus, cpANA_LOCK_STS_VIF);
        }
            if (bVIFStatus == 2)
            {
                    bTVDFlag = fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME);
                    if (bTVDFlag)
                    {
                    mcSHOW_DBG_MSG(("tvd success_IRC\n"));
                    }
                    else
                    {
                        bVIFStatus = 0;
                    }
            }
    }
  }
    if (bTVDFlag)
    {
        mcSHOW_DBG_MSG(("tvd success1 (means tvd lock)\n"));
    }
//--------------------------------------------------------------------------
//MTK PATCH : For ALPS tuner loss channel after did channel scan
//org ->    else
    else if(bVIFStatus ==2)
//--------------------------------------------------------------------------
    {
        bTVDFlag = fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME);
    }

  // Initial check to see if curent freq need full range AFT or not
    //if ((bScanType == eTUNER_UNI_AUTO_SCAN) && (bVIFStatus == 2) && (bTVDFlag)) //eTUNER_AUTO_FINE_TUNE
    if ((bVIFStatus == 2) && (bTVDFlag))
    {
        mcSHOW_DBG_MSG(("tvd success2\n"));
        mcSET_BIT(bATVStatus, cpANA_LOCK_STS_TVD);


		
		
        //bAFTStatus = bGetSCurveVol();
#if 0        
        if((bAFTStatus>=TDVRH751F_AFT_CENTER_VOL_FINE_TUNE_HIGH_40KHZ)&&(bAFTStatus<TDVRH751F_AFT_CENTER_VOL_HIGH)){
             mcSHOW_DBG_MSG(("fine tune1 +32KHz\n"));             
             *wFreqInKHz += 32;   
        }
        else if(bAFTStatus>=TDVRH751F_AFT_CENTER_VOL_HIGH){
             mcSHOW_DBG_MSG(("fine tune2 +63KHz\n"));             
             *wFreqInKHz += 63;  
        }
        else if((bAFTStatus<=TDVRH751F_AFT_CENTER_VOL_FINE_TUNE_LOW_40KHZ)&&(bAFTStatus>TDVRH751F_AFT_CENTER_VOL_LOW)){
             mcSHOW_DBG_MSG(("fine tune3 -32KHz\n"));             
             *wFreqInKHz -= 32;   
        }
        else if(bAFTStatus<=TDVRH751F_AFT_CENTER_VOL_LOW){
             mcSHOW_DBG_MSG(("fine tune4 -63KHz\n"));             
             *wFreqInKHz -= 63;  
        }
        else{
           mcSHOW_DBG_MSG(("don't need fine tune\n")); 
        }

#endif

#if 0
        
        if ((bAFTStatus >= TDVRH751F_AFT_CENTER_VOL_HIGH - 0x25) & (bAFTStatus <= TDVRH751F_AFT_CENTER_VOL_HIGH -0x15))
        {
                mcSHOW_DBG_MSG(("tvd success2+1\n"));
				param.Freq=*wFreqInKHz += 32;
                TDVRH751F_AnaTunerSetFreq(psAtvPiCtx, &param);
               *wFreqInKHz += 32;
        }
      else if ((bAFTStatus >= TDVRH751F_AFT_CENTER_VOL_HIGH - 0x15) & (bAFTStatus <= TDVRH751F_AFT_CENTER_VOL_HIGH))
        {
                mcSHOW_DBG_MSG(("tvd success2+2\n"));
				param.Freq=*wFreqInKHz += 63;
                TDVRH751F_AnaTunerSetFreq(psAtvPiCtx, &param);
                *wFreqInKHz += 63;
        }

       else if((bAFTStatus  >= TDVRH751F_AFT_CENTER_VOL_LOW) & (bAFTStatus <= TDVRH751F_AFT_CENTER_VOL_LOW + 0x15))
        {
                mcSHOW_DBG_MSG(("tvd success2-1\n"));
				param.Freq=*wFreqInKHz -= 60;
                TDVRH751F_AnaTunerSetFreq(psAtvPiCtx, &param);
                *wFreqInKHz -= 60;
        }
        else if((bAFTStatus  >= TDVRH751F_AFT_CENTER_VOL_LOW + 0x15) & (bAFTStatus <= TDVRH751F_AFT_CENTER_VOL_LOW+0x25))
        {
                 mcSHOW_DBG_MSG(("tvd success2-1\n"));
				 param.Freq=*wFreqInKHz -= 30;
                 TDVRH751F_AnaTunerSetFreq(psAtvPiCtx, &param);
                *wFreqInKHz -= 30;
        }

#endif

//		 param.Freq=*wFreqInKHz;
//         TDVRH751F_AnaTunerSetFreq(psAtvPiCtx, &param);
         //Driver  auto test log 
       mcMSG_IN_SEARCHNEARBY_AUTO_TEST(3125)
         
        PreLockFreq = *wFreqInKHz;
    //    if (bVIFStatus == 2)
//          *wFreqInKHz = wFreqInKHzTmp;
            return bATVStatus;
    }
    else
    {
        *wFreqInKHz = wFRTmp;
        bVIFStatus = 0;
        wFRTmp -= (TV_FINE_TUNE_THRESHOLD_KHZ / 2);
    }

    if (bVIFStatus == 0)
    {
    #if fcTIMING_MEASURE
        mcSHOW_DBG_MSG4(("[ATuner] SearchNearby.1=%3u ms\n",
                        (mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD()));
    #endif
    // Get the AFTStatus of lower frequency for precision initial PreAFTStatus.
        mcSHOW_DBG_MSG(("Rough Search.Start freq = %d\n",(wFRTmp - SCAN_MIDDLE_STEP_KHZ)));
        param.Freq= wFRTmp - SCAN_MIDDLE_STEP_KHZ;
        
        TDVRH751F_AnaTunerSetFreq(psAtvPiCtx, &param);
	 bAFTStatus = bGetSCurveVol();
        wFRTmpPre = wFRTmp - SCAN_MIDDLE_STEP_KHZ;//ray added
      //  mcSHOW_DBG_MSG(("wFRTmp = %d,wFRTmpPre = %d, SCAN_MIDDLE_STEP_KHZ = %d\n",wFRTmp,wFRTmpPre,SCAN_MIDDLE_STEP_KHZ));
       

        // Divide the TV_FINE_TUNE_THRESHOLD_KHZ into several large steps.
        // Right now, each large step is 500kHz (SCAN_MIDDLE_STEP_KHZ).
        while (wFRTmp <= (*wFreqInKHz + (TV_FINE_TUNE_THRESHOLD_KHZ / 2)))
        {
           // if (pTDVRH751F_AnaTuner->isBreak && pTDVRH751F_AnaTuner->isBreak(pvArg))
		    if (pTDVRH751F_AnaTuner->isBreak(pvArg))
            {
                mcSHOW_DBG_MSG(("[ATuner] SearchNearby.BreakFct\n"));
                mcSET_BIT(bATVStatus, cpANA_LOCK_STS_BREAK);
		        *wFreqInKHz = wFreqInKHzTmp;
                return bATVStatus;
                //return (FALSE);
            }
            param.Freq=wFRTmp;
            TDVRH751F_AnaTunerSetFreq(psAtvPiCtx,&param);

            bPreAFTStatus = bAFTStatus;
            bAFTStatus = bGetSCurveVol();

        #if fcTIMING_MEASURE
            mcSHOW_DBG_MSG(("[ATuner] SearchNearby.Rough=%3u ms / %d\n",
                            (mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD(),
                            (INT16) (wFRTmp - *wFreqInKHz)));
        #endif


        // change 0xff to 0xF0 to try to force enter this slop ; 20060917
            if ((bAFTStatus <= TDVRH751F_AFT_CENTER_VOL_HIGH)
             && (bPreAFTStatus >= TDVRH751F_AFT_CENTER_VOL_HIGH)) // FF -> !FF (possible slope of S-Curve found)
            {
            // First time enter the slope.
//-----------------------------------------------------------------------------
//MTK PATCH : For ALPS tuner loss channel after did channel scan
                param.Freq=wFRTmpPre + SCAN_MIDDLE_STEP_KHZ/2;
                TDVRH751F_AnaTunerSetFreq(psAtvPiCtx, &param);
		  mcSHOW_DBG_MSG(("set freq before check TVD  in rough search\n"));	
		  if (fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME))
		    {
		      mcSHOW_DBG_MSG(("check TVD before check VIF in rough search\n"));
              param.Freq=wFRTmp;
                      bVIFStatus = TDVRH751F_bDrvTunerCheckVIFLock(psAtvPiCtx,&param);
                      if (bVIFStatus!=0)
                       {
                          mcSHOW_DBG_MSG(("Start fine search\n"));
//-----------------------------------------------------------------------------
                          break;
//-----------------------------------------------------------------------------
//MTK PATCH : For ALPS tuner loss channel after did channel scan
                       }
					 mcSHOW_DBG_MSG(("TVD lock but VIF unlock\n"));
                    wFRTmpPre = wFRTmp;
                    wFRTmp += SCAN_MIDDLE_STEP_KHZ;   
					 mcSHOW_DBG_MSG(("Still skip 500KHz next time\n"));
		    }
                else
                {
                    //mcSHOW_DBG_MSG(("The current AFT voltage in threshold but VIF unlock\n"));
                    mcSHOW_DBG_MSG(("The current AFT voltage in threshold but TVD unlock\n"));
                    wFRTmpPre = wFRTmp;
                    wFRTmp += SCAN_MIDDLE_STEP_KHZ;
                }
//-----------------------------------------------------------------------------
            }
            else    // Freq is too small
            {
                  mcSHOW_DBG_MSG(("Still skip 500KHz next time\n"));
                wFRTmpPre = wFRTmp;
                wFRTmp += SCAN_MIDDLE_STEP_KHZ;
            }
        }
    }

// If AFT slope exists, find the center frequency by Dichotomy.
    if (wFRTmp <= (*wFreqInKHz + (TV_FINE_TUNE_THRESHOLD_KHZ / 2)))
    {
          wFRTmpScan = wFRTmpPre + (SCAN_MIDDLE_STEP_KHZ >>2);
        //mcSHOW_DBG_MSG(("wFRTmpScan = %d,wFRTmpPre = %d\n",wFRTmpScan,wFRTmpPre));
          mcSHOW_DBG_MSG(("Start fine search freq = %dKHz\n",wFRTmpScan));
          param.Freq= wFRTmpScan ;
          TDVRH751F_AnaTunerSetFreq(psAtvPiCtx,&param);
		  
        //while (bMaxStepNum <= 3)    // SCAN_SMALL_STEP (0x04) can only shift right for 2 bits
        while (bMaxStepNum <= 2)    // SCAN_SMALL_STEP (0x04) can only shift right for 2 bits
        {
             // mcSHOW_DBG_MSG(("Start fine search\n"));
            //if (pTDVRH751F_AnaTuner->isBreak && pTDVRH751F_AnaTuner->isBreak(pvArg))
			if (pTDVRH751F_AnaTuner->isBreak(pvArg))
            {
                mcSHOW_DBG_MSG(("[ATuner] SearchNearby.BreakFct\n"));
                mcSET_BIT(bATVStatus, cpANA_LOCK_STS_BREAK);
		        *wFreqInKHz = wFreqInKHzTmp;
                return bATVStatus;
                //return (FALSE);
            }
            bAFTStatus = bGetSCurveVol();

        #if fcTIMING_MEASURE
            mcSHOW_DBG_MSG(("[ATuner] SearchNearby.Fine=%3u ms / %d\n",
                            (mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD(),
                            (INT16) (wFRTmpScan - *wFreqInKHz)));
        #endif
           if (bAFTStatus <= TDVRH751F_AFT_CENTER_VOL_HIGH)
            {
              mcSHOW_DBG_MSG(("extral check for LG tdvrh75 first\n"));
              if ((wFreqInKHzTmp<=86000)&&(wFreqInKHzTmp>=71000))
               {   
                  mcSHOW_DBG_MSG(("extral check for LG tdvrh75 second\n"));
            param.Freq=wFRTmpScan - 200;
            TDVRH751F_AnaTunerSetFreq(psAtvPiCtx,&param);
		    bTVDCheckFlag1 = fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME);
			
                  bPreAFTStatusCheck= bGetSCurveVol();
            param.Freq=wFRTmpScan + 200;
            TDVRH751F_AnaTunerSetFreq(psAtvPiCtx,&param);
		    bTVDCheckFlag2 = fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME);
		    if (!((bPreAFTStatusCheck>0x90)&&(bAFTStatus>=TDVRH751F_AFT_CENTER_VOL_LOW)&&(bTVDCheckFlag1)&&(bTVDCheckFlag2)))
		 	{
		 	  mcSHOW_DBG_MSG(("extral check for LG tdvrh75 third\n"));
                       wFRTmpScan += (SCAN_MIDDLE_STEP_KHZ >>2);
		 	}
		   else
		   	{
		   	   mcSHOW_DBG_MSG(("extral check and Fine search end \n"));
                        wFRTmp = wFRTmpScan;
			   param.Freq=wFRTmpScan;
		
            TDVRH751F_AnaTunerSetFreq(psAtvPiCtx,&param);			
                        break;
	
		   	}
               }
	     else
	      	{
                  mcSHOW_DBG_MSG(("Fine search end\n"));
                wFRTmp = wFRTmpScan;
                break;
	      	}
            }
            else if (bAFTStatus > TDVRH751F_AFT_CENTER_VOL_HIGH)
            {
                  mcSHOW_DBG_MSG(("Still skip 125KHz next time\n"));
                wFRTmpScan += (SCAN_MIDDLE_STEP_KHZ >>2);
            }
            param.Freq=wFRTmpScan;
            TDVRH751F_AnaTunerSetFreq(psAtvPiCtx,&param);
            bMaxStepNum ++;
        }
        //if (bMaxStepNum == 4)
        if (bMaxStepNum == 3)
        {
              mcSHOW_DBG_MSG(("The fourth fine search\n"));
             param.Freq=wFRTmp;
            TDVRH751F_AnaTunerSetFreq(psAtvPiCtx,&param);
	 
         }

        *wFreqInKHz = wFRTmp;
        bATVStatus = mcBIT(cpANA_LOCK_STS_VIF);

        if (fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME))
        {
            mcSET_BIT(bATVStatus, cpANA_LOCK_STS_TVD);


#if 0		
	   /*bAFTStatus = bGetSCurveVol();
       if ((bAFTStatus >= TDVRH751F_AFT_CENTER_VOL_HIGH - 0x25) & (bAFTStatus <= TDVRH751F_AFT_CENTER_VOL_HIGH -0x15))
        {
                mcSHOW_DBG_MSG(("tvd success2+1\n"));
                wFRTmp += 32;
        }
      else if ((bAFTStatus >= TDVRH751F_AFT_CENTER_VOL_HIGH - 0x15) & (bAFTStatus <= TDVRH751F_AFT_CENTER_VOL_HIGH))
        {
                mcSHOW_DBG_MSG(("tvd success2+2\n"));
               wFRTmp += 63;
        }

       else if((bAFTStatus  >= TDVRH751F_AFT_CENTER_VOL_LOW) & (bAFTStatus <= TDVRH751F_AFT_CENTER_VOL_LOW + 0x15))
        {
                mcSHOW_DBG_MSG(("tvd success2-1\n"));
                wFRTmp -= 60;
        }
        else if((bAFTStatus  >= TDVRH751F_AFT_CENTER_VOL_LOW + 0x15) & (bAFTStatus <= TDVRH751F_AFT_CENTER_VOL_LOW+0x25))
        {
                 mcSHOW_DBG_MSG(("tvd success2-1\n"));
                 wFRTmp -= 30;               
        }*/
        bAFTStatus = bGetSCurveVol();
        
        if((bAFTStatus>=TDVRH751F_AFT_CENTER_VOL_FINE_TUNE_HIGH_40KHZ)&&(bAFTStatus<TDVRH751F_AFT_CENTER_VOL_HIGH)){
             mcSHOW_DBG_MSG(("fine tune1 +32KHz\n"));             
             *wFreqInKHz += 32;   
        }
        else if(bAFTStatus>=TDVRH751F_AFT_CENTER_VOL_HIGH){
             mcSHOW_DBG_MSG(("fine tune2 +63KHz\n"));             
             *wFreqInKHz += 63;  
        }
        else if((bAFTStatus<=TDVRH751F_AFT_CENTER_VOL_FINE_TUNE_LOW_40KHZ)&&(bAFTStatus>TDVRH751F_AFT_CENTER_VOL_LOW)){
             mcSHOW_DBG_MSG(("fine tune3 -32KHz\n"));             
             *wFreqInKHz -= 32;   
        }
        else if(bAFTStatus<=TDVRH751F_AFT_CENTER_VOL_LOW){
             mcSHOW_DBG_MSG(("fine tune4 -63KHz\n"));             
             *wFreqInKHz -= 63;  
        }
        else{
           mcSHOW_DBG_MSG(("don't need fine tune\n")); 
        }
         param.Freq= *wFreqInKHz;

		TDVRH751F_AnaTunerSetFreq(psAtvPiCtx, &param);
#endif


	     //PreLockFreq = *wFreqInKHz;
	     PreLockFreq = wFRTmp;

        // *wFreqInKHz = wFRTmp;
         //Driver Auto Test log
         mcMSG_IN_SEARCHNEARBY_AUTO_TEST(3125)
         
	    *wFreqInKHz = wFreqInKHzTmp;
            return bATVStatus;
        }
        else
        { 
           *wFreqInKHz = wFreqInKHzTmp;
            return bATVStatus;
        }
    }
    else  // go to next channel
    {
#if 0
        	if(bWeakSignalTVDFlag & 0x1)
		{
            param.Freq=wFreqInKHzTmp;
			TDVRH751F_AnaTunerSetFreq(psAtvPiCtx,&param);
			mcSHOW_DBG_MSG(("###STD(1)\n"));
			 PreLockFreq = wFreqInKHzTmp;
			*wFreqInKHz = wFreqInKHzTmp;
	              mcSET_BIT(bATVStatus, cpANA_LOCK_STS_VIF);
			mcSET_BIT(bATVStatus, cpANA_LOCK_STS_TVD);
			return bATVStatus;
		}
#endif
		//else // go to next channel
		//{
	              *wFreqInKHz = wFreqInKHzTmp;
	              return FALSE;
		//}
    }
}

//----------------------------------------------------------------------------- 
/*
 *  TDVRH751F_MonitorAnaTuner
 *  Find the freq. value that near center frequency of the channel.
 *  @param  psAtvPiCtx  	a type pointer use to point to struct ATV_PI_CTX_T.
 *  @param  bScanType		AUTO_FINE_TUNE, UNI_AUTO_SCAN
 *  @param  *wFreqInKHz  	THe point of frequenccy in KHz need to check the VIF status
 *  @param  _BreakFct  	 	The break function
 *  @param  *pvArg  		The argument of break function
 *  @retval   TRUE  =>  center freq. found
            	    FALSE =>  center freq. not found (no signal on this channel)
 */
//-----------------------------------------------------------------------------
#if 0
UINT8 TDVRH751F_MonitorAnaTuner(ATV_PI_CTX_T *psAtvPiCtx, UINT32 *wFreqInKHz,
                              x_break_fct _BreakFct, void *pvArg)
{
ATV_PI_CTX_T *pTDVRH751F_AnaTuner = psAtvPiCtx;
UINT8 bAFTStatus;
UINT8 bATVStatus = mcBIT(cpANA_LOCK_STS_VIF) | mcBIT(cpANA_LOCK_STS_TVD);
static UINT8 TDVRH751F_Monitor_Counter = 0;

    mcSHOW_DBG_MSG3(("TDVRH751F_Monitor Thread %d -------\n",*wFreqInKHz));
    pTDVRH751F_AnaTuner->isBreak = _BreakFct;
    bAFTStatus = bGetSCurveVol();    
    if ((bAFTStatus <= TDVRH751F_AFT_CENTER_VOL_HIGH)
       && (bAFTStatus >= TDVRH751F_AFT_CENTER_VOL_LOW))
    {
            TDVRH751F_Monitor_Counter = 0;
        return bATVStatus;
    }
    else
    {
        if (pTDVRH751F_AnaTuner->isBreak)
        {
            if (pTDVRH751F_AnaTuner->isBreak(pvArg))
            {
                mcSHOW_DBG_MSG(("[ATuner] TDVRH751F_MonitorAnaTuner: SearchNearby.BreakFct\n"));
                return (FALSE);
            }
        }
         mcSHOW_DBG_MSG2(("Out of range, bCurveStatus = %02X (%d),TDVRH751F_Monitor_Counter= %d\n", bAFTStatus, bAFTStatus,TDVRH751F_Monitor_Counter));
         mcSHOW_DBG_MSG2(("out of range\n"));
          if(TDVRH751F_Monitor_Counter>=1)
        {
        DigTunerBypassI2C(TRUE);
        bATVStatus = TDVRH751F_AnaTunerSearchNearbyFreq(psAtvPiCtx,wFreqInKHz,_BreakFct, pvArg);
        DigTunerBypassI2C(FALSE);
        }
        TDVRH751F_Monitor_Counter++;
    }
    return bATVStatus;
}
#endif
#if 1
UINT8 TDVRH751F_MonitorAnaTuner(ATV_PI_CTX_T *psAtvPiCtx, UINT32 *wFreqInKHz,x_break_fct _BreakFct, void *pvArg)
{
ATV_PI_CTX_T *pTDVRH751F_AnaTuner = psAtvPiCtx;
UINT8 bAFTStatus;
UINT8 bATVStatus = mcBIT(cpANA_LOCK_STS_VIF) | mcBIT(cpANA_LOCK_STS_TVD);
UINT8 u1AnaMode = pTDVRH751F_AnaTuner->u1AnaMode;
//UINT8 u1AnaMode = u1MonitorAnaMode;
static UINT8 TDVRH751F_Monitor_Counter = 0;
static UINT8 TDVRH751F_SearchCnt = 0;
static INT32 wFreqInKHz2 = 0;
UINT32 wFreqInKHz1;
UINT32 wFreqBoundary = TV_FINE_TUNE_THRESHOLD_KHZ/2;
UINT32 Freq=pTDVRH751F_AnaTuner->u4Freq;
PARAM_SETFREQ_T param;
param.Modulation=u1AnaMode;
param.fgAutoSearch=0;
//mcSHOW_DBG_MSG(("Channel Monitor %d\n",u1AnaMode));
//mcSHOW_DBG_MSG(("Channel Monitor %d\n",u1AnaMode));

//if (PreLockFreq != 0)
//{
		wFreqInKHz1 = PreLockFreq;
//}

 mcSHOW_DBG_MSG2(("Monitor frequency = %d\n", *wFreqInKHz));
mcSHOW_DBG_MSG3(("Last monitor end freq = %dKHz\n",pTDVRH751F_AnaTuner->u4Freq));
#if 0
if (u1AnaMode == MOD_ANA_TERR)
{
		wFreqBoundary = TV_TERR_FINE_TUNE_THRESHOLD_KHZ/2;
		pTDVRH751F_AnaTuner->u1AnaMode = u1AnaMode;
}
#endif
    pTDVRH751F_AnaTuner->isBreak = _BreakFct;
    
//Driver Auto Test log 
mcMSG_IN_MONITOR_AUTO_TEST(3125)

{
		  UINT8  	TVD_TMP;//,TVD_TMP2;
			TVD_TMP = (IO_READ8(VIDEO_IN0_BASE, 0x81) & 0x10);    
			if (TVD_TMP == 0x10)
	    {
	    	mcSHOW_DBG_MSG3(("Vpress = 1;\n"));
	    //	TVD_TMP2 = IO_READ8(0x20022000, 0x80); 
	    //	if(TVD_TMP2 < 0x15)
	    //	{
	    	//	mcSHOW_DBG_MSG3(("Noise < 0x15 (%x)\n",TVD_TMP2));
	  			bAFTStatus = bGetSCurveVol();
    			if ((bAFTStatus <= TDVRH751F_AFT_CENTER_VOL_HIGH_MONI ) && (bAFTStatus >= TDVRH751F_AFT_CENTER_VOL_LOW_MONI))
    			{
         		if(wFreqInKHz2 != 0)	PreLockFreq = wFreqInKHz1 + wFreqInKHz2;
         	            wFreqInKHz2 = 0;
         	       TDVRH751F_Monitor_Counter = 0;
         		TDVRH751F_SearchCnt = 0;
        		return bATVStatus;
    			}
    			else
    			{
        		if (pTDVRH751F_AnaTuner->isBreak)
        		{
        			if (pTDVRH751F_AnaTuner->isBreak(pvArg))
        			{
         				mcSHOW_DBG_MSG(("[ATuner] SearchNearby.BreakFct\n"));
         				return (FALSE);
        			}
        		}
         		mcSHOW_DBG_MSG(("Out of range, bCurveStatus = %02X (%d),TDVRH751F_Monitor_Counter= %d\n", bAFTStatus, bAFTStatus,TDVRH751F_Monitor_Counter));
         		mcSHOW_DBG_MSG(("out of range\n"));
          	if(TDVRH751F_Monitor_Counter>=1)
        		{
        			DigTunerBypassI2C(TRUE);
           		if(((wFreqInKHz1 + wFreqInKHz2) <= (*wFreqInKHz + wFreqBoundary)) && 
           			 ((wFreqInKHz1 + wFreqInKHz2) >= (*wFreqInKHz - wFreqBoundary)))
           		{	 
           		    if(bAFTStatus <= TDVRH751F_AFT_CENTER_VOL )
           		    {
           			  	 wFreqInKHz2 -= 50;
           				   //TDVRH751F_AnaTunerSetFreq(psAtvPiCtx, wFreqInKHz1 + wFreqInKHz2, u1AnaMode);	
           			     mcSHOW_DBG_MSG(("-50KHz (%d) %x\n",wFreqInKHz1 + wFreqInKHz2,bAFTStatus));
           		    }
           		    else
           		    {           			
           			     wFreqInKHz2 += 50;
           			     //TDVRH751F_AnaTunerSetFreq(psAtvPiCtx, wFreqInKHz1 + wFreqInKHz2, u1AnaMode);	
           			     mcSHOW_DBG_MSG(("+50KHz (%d) %x\n",wFreqInKHz1 + wFreqInKHz2,bAFTStatus));
           		    }
                    param.Freq=wFreqInKHz1 + wFreqInKHz2;
                    TDVRH751F_AnaTunerSetFreq(psAtvPiCtx,&param);
           		}
           		else
           		{
           				bATVStatus = TDVRH751F_AnaTunerSearchNearbyFreq(psAtvPiCtx, wFreqInKHz,
           					              pTDVRH751F_AnaTuner->isBreak, pvArg);
           				wFreqInKHz2 = 0;           					              
           		}	           		
        			DigTunerBypassI2C(FALSE);
        		}
        		TDVRH751F_Monitor_Counter++;
    			//}
    	//		}
    		}
	  	}
	  	else
	  	{
	    	mcSHOW_DBG_MSG(("Vpress = 0;\n"));    	
	    	if(TDVRH751F_SearchCnt >= 1)
        {          
        	mcSHOW_DBG_MSG(("TDVRH751F_SearchCnt >= 1\n"));
        	DigTunerBypassI2C(TRUE);
        	bATVStatus = TDVRH751F_AnaTunerSearchNearbyFreq(psAtvPiCtx, wFreqInKHz,
        						              pTDVRH751F_AnaTuner->isBreak, pvArg);
        	
        	if(bATVStatus <= 1)
        	{  // Improve Sentivity
        		UINT32 FreqTmp;
        		if(*wFreqInKHz > PreLockFreq)
        			FreqTmp = *wFreqInKHz - PreLockFreq;
        		else	
        			FreqTmp = PreLockFreq - *wFreqInKHz;
        			        	
        	  if(FreqTmp > wFreqBoundary)
        	  {
        	      mcSHOW_DBG_MSG(("TDVRH751F_Monitor Freq %d : Rre_Freq %d\n",*wFreqInKHz,PreLockFreq));
        	     // TDVRH751F_AnaTunerSetFreq(psAtvPiCtx, *wFreqInKHz, u1AnaMode);
        		    PreLockFreq = *wFreqInKHz;
        	  }
        	  else
        	  {
        	      mcSHOW_DBG_MSG(("Pre-TDVRH751F_Monitor Freq %d : Rre_Freq %d\n",*wFreqInKHz,PreLockFreq));
        	     // TDVRH751F_AnaTunerSetFreq(psAtvPiCtx, PreLockFreq, u1AnaMode);
        	  }
              param.Freq=PreLockFreq;
             TDVRH751F_AnaTunerSetFreq(psAtvPiCtx,&param);
        	}
        	
        	wFreqInKHz2 = 0;
        	DigTunerBypassI2C(FALSE);
        }
        TDVRH751F_SearchCnt++;
	  	}
}  

    return bATVStatus;
}
#endif
//----------------------------------------------------------------------------- 
/*
 *  TDVRH751F_AnaTunerGetVer
 *  Get Tuner Version.
 *  @param  void.
 *  @retval   TDVRH751F_ANA_TUNER_VER.
 */
//-----------------------------------------------------------------------------
UINT8 *TDVRH751F_AnaTunerGetVer(ATV_PI_CTX_T *psAtvPiCtx){
    return ("analog TDVRH751FX524A");
}
