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
 * $RCSfile: piana_va1g5bf.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "PI_ATuner.h"
	#include "PI_Def.h"
	#include "pi_anana_if.h"
	
#include "piana_va1g5bf.h"
//#include "tvsys_info.h"
#include "pd_anana_glue.h"
#include "tuner_interface_if.h"
#include "ctrl_bus.h"

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

#define VA1G5BF_AFT_CENTER_VOL_HIGH    0xA4   
#define VA1G5BF_AFT_CENTER_VOL_LOW     0x1A  

#define VA1G5BF_AFT_CENTER_VOL   0x54     

#define VA1G5BF_AFT_CENTER_VOL_HIGH_MONI  0xB8
#define VA1G5BF_AFT_CENTER_VOL_LOW_MONI   0x12


//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------
UINT32 PreLockFreq;
//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
//todo: pdwnc module should export _if.h
EXTERN UINT32 PDWNC_ReadServoADCChannelValue(UINT32 u4Channel);
EXTERN  VOID    DigTunerBypassI2C(BOOL bSwitchOn);

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
    mcSHOW_DBG_MSG(("AFT_AD_NUM=%d\n",AFT_AD_NUM));
    #if defined(CC_MT5360) || defined(CC_MT5363)
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
 *  VA1G5BF_AnaTunerInit
 *  Tuner Driver Initial FUnction
 *  @param  psAtvPiCtx  a type pointer use to point to struct ATV_PI_CTX_T.
 *  @param  IFDEMOD_I2c_Addr	a I2C address of DEMOD.
 *  @param  RF_I2c_Addr  a I2C address of RF Tuner.
 *  @retval   TRUE : Success.
 */
//-----------------------------------------------------------------------------
BOOL VA1G5BF_AnaTunerInit(ATV_PI_CTX_T *psAtvPiCtx, UINT8 IFDEMOD_I2c_Addr, UINT8 RF_I2c_Addr)
{
//ATV_PI_CTX_T *pVA1G5BF_AnaTuner = NULL;
ATV_PI_CTX_T *pVA1G5BF_AnaTuner = psAtvPiCtx;

//    pVA1G5BF_AnaTuner = (ATV_PI_CTX_T *) mcALLOC_MEM(sizeof(ATV_PI_CTX_T));
    if (pVA1G5BF_AnaTuner == NULL)
        return FALSE;
//    pVA1G5BF_AnaTuner->u1IF_I2cAddr = IFDEMOD_I2c_Addr;
//    pVA1G5BF_AnaTuner->u1RF_I2cAddr = RF_I2c_Addr;
//    pVA1G5BF_AnaTuner->u1IF_I2cAddr = TUNER_ONE_ADDR_IF;
    //pVA1G5BF_AnaTuner->pDigiTunerCtx = ITunerGetCtx();
    pVA1G5BF_AnaTuner->u1RF_I2cAddr = TUNER_ONE_ADDR_TUNER;
    pVA1G5BF_AnaTuner->u2IF_Freq = VA1G5BF_ANA_PIC_IF;
    pVA1G5BF_AnaTuner->u2LO_Step = VA1G5BF_ANA_FREQ_STEP;
//    pVA1G5BF_AnaTuner->u4Frequency = VA1G5BF_ANA_INIT_FREQ;
    pVA1G5BF_AnaTuner->u1DEMOD_state = VA1G5BF_ANA_UNLOCK;
    pVA1G5BF_AnaTuner->u1AnaMode = MOD_ANA_CABLE;
//    pVA1G5BF_AnaTuner->aSW_Ver_HW_Ver = VA1G5BF_ANA_TUNER_VER;

//    *ppsAtvPiCtx = pVA1G5BF_AnaTuner;
    return TRUE;
}

//----------------------------------------------------------------------------- 
/*
 *  VA1G5BF_AnaTunerClose
 *  Tuner driver close FUnction
 *  @param  psAtvPiCtx  a type pointer use to point to struct ATV_PI_CTX_T.
 *  @retval   void
 */
//-----------------------------------------------------------------------------
void VA1G5BF_AnaTunerClose(ATV_PI_CTX_T *psAtvPiCtx)
{
//    mcFREE_MEM(psAtvPiCtx);
}

//----------------------------------------------------------------------------- 
/*
 *  VA1G5BF_AnaTunerSetFreq
 *  Set the frequency into tuner part.
 *  @param  psAtvPiCtx  	a type pointer use to point to struct ATV_PI_CTX_T.
 *  @param  wFreqInKHz	aFrequency value in KHz.
 *  @param  u1AnaMode	Cable/Air Mode.
 *  @retval   TRUE : I2C programming successful.
 *  @retval   FALSE : FALSE => I2C programming fail 
 */
//-----------------------------------------------------------------------------
BOOL VA1G5BF_AnaTunerSetFreq(ATV_PI_CTX_T *psAtvPiCtx, PARAM_SETFREQ_T * param)
{
BOOL fgRetSts = TRUE;
ATV_PI_CTX_T *pVA1G5BF_AnaTuner = psAtvPiCtx;

    //if ((wFreqInKHz < 55000 )
     //|| (wFreqInKHz > 863000 ))
      // return (FALSE);

    mcSHOW_DBG_MSG(("VA1G5BF_AnaTunerSetFreq %d KHz\n",param->Freq));
   
    pVA1G5BF_AnaTuner->u1AnaMode =param->Modulation;
    pVA1G5BF_AnaTuner->u4Freq =param->Freq;
   if (ITuner_SetFreq(ITunerGetCtx(),param) != 0)
    {
        fgRetSts = FALSE;
    }
    else
    {
        ITuner_OP(ITunerGetCtx(),itGetStatus,0,&fgRetSts);
        if(!fgRetSts)
        {
           if(ITuner_SetFreq(ITunerGetCtx(),param) != 0){
                fgRetSts = FALSE;
           }
           else
           	{
           		fgRetSts = TRUE;
           	}
        }
    }
    if(!fgRetSts){
    	mcSHOW_DBG_MSG(("%s fail!\n",__FUNCTION__));
    }
    return fgRetSts;
}

//----------------------------------------------------------------------------- 
/*
 *  fgDrvTunerSetSystem
 *  Get the sound system of given sub system into tuner HW
 *  @param  psAtvPiCtx  	a type pointer use to point to struct ATV_PI_CTX_T.
 *  @retval   Sub system index.
 */
//-----------------------------------------------------------------------------
UINT8 VA1G5BF_TunerGetSystem(ATV_PI_CTX_T *psAtvPiCtx)
{
ATV_PI_CTX_T *pVA1G5BF_AnaTuner = psAtvPiCtx;

    return pVA1G5BF_AnaTuner->u1SubSysIdx;
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
BOOL VA1G5BF_TunerSetSystem(ATV_PI_CTX_T *psAtvPiCtx, UINT8 u1SubSysIdx)
{
ATV_PI_CTX_T *pVA1G5BF_AnaTuner = psAtvPiCtx;
//mcLINT_UNUSED(u1SubSysIdx);

    pVA1G5BF_AnaTuner->u1SubSysIdx = u1SubSysIdx;
    return TRUE;
//Gene Chang Because the Samsung tuner doesn't set IF part. 2006.04.18
}

//----------------------------------------------------------------------------- 
/*
 *  VA1G5BF_bDrvTunerCheckVIFLock
 *  Check if programmed frequency has strong VIF signal nearby
 *  @param  psAtvPiCtx  	a type pointer use to point to struct ATV_PI_CTX_T.
 *  @param  wFreqInKHz  	The frequenccy in KHz need to check the VIF status
 *  @retval   	 2  => Current frequency is very close to the nominal VIF.
           		 1  => Current frequency is is near the nominal VIF.
           		 0  => Current frequency is not close to the nominal VIF yet.
 */
//-----------------------------------------------------------------------------
UINT8 VA1G5BF_bDrvTunerCheckVIFLock(ATV_PI_CTX_T *psAtvPiCtx, PARAM_SETFREQ_T * param)
{
ATV_PI_CTX_T *pVA1G5BF_AnaTuner = psAtvPiCtx;
UINT8 bAFTStatus, bPreAFTStatus;
UINT8 bDiffPreAFT;
UINT32 Freq=param->Freq;
param->Freq-=60;  

   VA1G5BF_AnaTunerSetFreq(psAtvPiCtx,param);
    bPreAFTStatus = bGetSCurveVol();


     param->Freq=Freq;
     VA1G5BF_AnaTunerSetFreq(psAtvPiCtx,param);
     bAFTStatus = bGetSCurveVol();


    if (bPreAFTStatus > bAFTStatus)
        bDiffPreAFT = bPreAFTStatus - bAFTStatus;
    else
        bDiffPreAFT = 0;

    mcSHOW_DBG_MSG(("bAFTStatus, bPreAFTStatus, bDiffPreAFT %02X,%02X,%02X\n", bAFTStatus, bPreAFTStatus, bDiffPreAFT ));

if ((bAFTStatus <= VA1G5BF_AFT_CENTER_VOL_HIGH) && (bAFTStatus >= VA1G5BF_AFT_CENTER_VOL_LOW)
      && (bDiffPreAFT > 0x10))//&&(bPreAFTStatus!=0xff))
    {
        mcSHOW_DBG_MSG(("CheckVIFLock-return 2 (2 means VIF lock)\n"));
    // Current Freq is very close to the nominal VIF, don't tune the freq anymore.
        pVA1G5BF_AnaTuner->u1DEMOD_state = VA1G5BF_ANA_LOCK;

        return 2;
    }
    else if(((bAFTStatus < VA1G5BF_AFT_CENTER_VOL_LOW)&&(bPreAFTStatus!=0xff))||(bAFTStatus < VA1G5BF_AFT_CENTER_VOL_HIGH))
    {
        mcSHOW_DBG_MSG(("CheckVIFLock-return 1 (1 means current freq > signal freq)\n"));
        pVA1G5BF_AnaTuner->u1DEMOD_state = VA1G5BF_ANA_UNLOCK;
        return 1;
    }
    else
    {
       mcSHOW_DBG_MSG(("CheckVIFLock-return 0 (0 means current freq < signal freq and far from signal freq)\n"));
       pVA1G5BF_AnaTuner->u1DEMOD_state = VA1G5BF_ANA_UNLOCK;
        return 0;
    }
}

//----------------------------------------------------------------------------- 
/*
 *  VA1G5BF_AnaTunerSearchNearbyFreq
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
UINT8 VA1G5BF_AnaTunerSearchNearbyFreq(ATV_PI_CTX_T *psAtvPiCtx, UINT32 *wFreqInKHz,
                                       x_break_fct _BreakFct, void *pvArg)
{
ATV_PI_CTX_T *pVA1G5BF_AnaTuner = psAtvPiCtx;
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
UINT8 bWeakSignalTVDFlag = 0;
PARAM_SETFREQ_T param;
#if fcTIMING_MEASURE
    UINT32 u2TickStart = mcGET_SYS_TICK();
#endif


    if (pVA1G5BF_AnaTuner == NULL)
        return (FALSE);

    if (wFreqInKHz != NULL)
    {
        wFRTmp = *wFreqInKHz;
        wFreqInKHzTmp = *wFreqInKHz;
    }
    else
    {
        return (bATVStatus);
    }
    mcSHOW_DBG_MSG(("Start VA1G5BF_serach  %d KHz\n",wFRTmp));
    u1AnaMode = pVA1G5BF_AnaTuner->u1AnaMode;
    param.Freq=wFRTmp;
    param.Modulation=u1AnaMode;
    param.fgAutoSearch=1;
    pVA1G5BF_AnaTuner->isBreak = _BreakFct;
    bVIFStatus = VA1G5BF_bDrvTunerCheckVIFLock(psAtvPiCtx, &param);
	
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
        bVIFStatus = VA1G5BF_bDrvTunerCheckVIFLock(psAtvPiCtx,&param);
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
        param.Freq=wFRTmp+750;
        bVIFStatus = VA1G5BF_bDrvTunerCheckVIFLock(psAtvPiCtx,&param);
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
        bVIFStatus = VA1G5BF_bDrvTunerCheckVIFLock(psAtvPiCtx,&param);
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
		
        bAFTStatus = bGetSCurveVol();
        if ((bAFTStatus >= VA1G5BF_AFT_CENTER_VOL_HIGH - 0x25) & (bAFTStatus <= VA1G5BF_AFT_CENTER_VOL_HIGH -0x15))
        {
                mcSHOW_DBG_MSG(("tvd success2+1\n"));
             //   VA1G5BF_AnaTunerSetFreq(psAtvPiCtx, *wFreqInKHz+32,u1AnaMode);
                *wFreqInKHz += 32;
        }
      else if ((bAFTStatus >= VA1G5BF_AFT_CENTER_VOL_HIGH - 0x15) & (bAFTStatus <= VA1G5BF_AFT_CENTER_VOL_HIGH))
        {
                mcSHOW_DBG_MSG(("tvd success2+2\n"));
              //  VA1G5BF_AnaTunerSetFreq(psAtvPiCtx, *wFreqInKHz+63,u1AnaMode);
                *wFreqInKHz += 63;
        }

       else if((bAFTStatus  >= VA1G5BF_AFT_CENTER_VOL_LOW) & (bAFTStatus <= VA1G5BF_AFT_CENTER_VOL_LOW + 0x15))
        {
                mcSHOW_DBG_MSG(("tvd success2-1\n"));
              //  VA1G5BF_AnaTunerSetFreq(psAtvPiCtx, *wFreqInKHz-60,u1AnaMode);
                *wFreqInKHz -= 60;
        }
        else if((bAFTStatus  >= VA1G5BF_AFT_CENTER_VOL_LOW + 0x15) & (bAFTStatus <= VA1G5BF_AFT_CENTER_VOL_LOW+0x25))
        {
                 mcSHOW_DBG_MSG(("tvd success2-1\n"));
                // VA1G5BF_AnaTunerSetFreq(psAtvPiCtx, *wFreqInKHz-30,u1AnaMode);
                *wFreqInKHz -= 30;
        }
		    #if fcTIMING_MEASURE
    mcSHOW_DBG_MSG(("[ATuner] SearchNearby.0=%d ms, %d KHz\n",(mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD(),*wFreqInKHz-wFreqInKHzTmp));
        #endif
        param.Freq=*wFreqInKHz;
        VA1G5BF_bDrvTunerCheckVIFLock(psAtvPiCtx,&param);
        PreLockFreq = *wFreqInKHz;
        *wFreqInKHz=wFreqInKHzTmp;
    //    if (bVIFStatus == 2)
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
    // Get the AFTStatus of lower frequency for precision initial PreAFTStatus.
        mcSHOW_DBG_MSG(("Rough Search.Start freq = %d KHz\n",(wFRTmp - SCAN_MIDDLE_STEP_KHZ)));
        param.Freq= wFRTmp - SCAN_MIDDLE_STEP_KHZ;
        VA1G5BF_AnaTunerSetFreq(psAtvPiCtx,&param);
	 bAFTStatus = bGetSCurveVol();
        wFRTmpPre = wFRTmp - SCAN_MIDDLE_STEP_KHZ;//ray added
      //  mcSHOW_DBG_MSG(("wFRTmp = %d,wFRTmpPre = %d, SCAN_MIDDLE_STEP_KHZ = %d\n",wFRTmp,wFRTmpPre,SCAN_MIDDLE_STEP_KHZ));
       

        // Divide the TV_FINE_TUNE_THRESHOLD_KHZ into several large steps.
        // Right now, each large step is 500kHz (SCAN_MIDDLE_STEP_KHZ).
        while (wFRTmp <= (*wFreqInKHz + (TV_FINE_TUNE_THRESHOLD_KHZ / 2)))
        {
            if (pVA1G5BF_AnaTuner->isBreak(pvArg))
            {
                mcSHOW_DBG_MSG(("[ATuner] SearchNearby.BreakFct\n"));
                mcSET_BIT(bATVStatus, cpANA_LOCK_STS_BREAK);
		            *wFreqInKHz = wFreqInKHzTmp;
		           // *wFreqInKHz=wFRTmp;
                return bATVStatus;
                //return (FALSE);
            }
            param.Freq=wFRTmp;
            VA1G5BF_AnaTunerSetFreq(psAtvPiCtx,&param);

            bPreAFTStatus = bAFTStatus;
            bAFTStatus = bGetSCurveVol();


        // change 0xff to 0xF0 to try to force enter this slop ; 20060917
            if ((bAFTStatus <= VA1G5BF_AFT_CENTER_VOL_HIGH)
             && (bPreAFTStatus >= VA1G5BF_AFT_CENTER_VOL_HIGH)) // FF -> !FF (possible slope of S-Curve found)
            {
            // First time enter the slope.
//-----------------------------------------------------------------------------
//MTK PATCH : For ALPS tuner loss channel after did channel scan
               param.Freq=wFRTmp;
                bVIFStatus = VA1G5BF_bDrvTunerCheckVIFLock(psAtvPiCtx,&param);
                if (bVIFStatus!=0)
                {
                	 param.Freq=wFRTmpPre+SCAN_MIDDLE_STEP_KHZ/2;
                  	VA1G5BF_AnaTunerSetFreq(psAtvPiCtx,&param);
                	 if(fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME))
                	 	{
                	 	mcSHOW_DBG_MSG(("TVD lock at %d KHz\n",wFRTmpPre+SCAN_MIDDLE_STEP_KHZ/2));
                    mcSHOW_DBG_MSG(("Start fine search\n"));
//-----------------------------------------------------------------------------
                break;
//-----------------------------------------------------------------------------
                   }
                   mcSHOW_DBG_MSG(("Dummy VIF lock\n"));
                  } 
//MTK PATCH : For ALPS tuner loss channel after did channel scan
              // else
             //   {
                    mcSHOW_DBG_MSG(("The current AFT voltage in threshold but VIF unlock\n"));
                    wFRTmpPre = wFRTmp;
                    wFRTmp += SCAN_MIDDLE_STEP_KHZ;
              //  }
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
          mcSHOW_DBG_MSG(("Start fine search freq = %d KHz\n",wFRTmpScan));
          param.Freq=wFRTmpScan;
          VA1G5BF_AnaTunerSetFreq(psAtvPiCtx, &param);
        while (bMaxStepNum <= 2)    // SCAN_SMALL_STEP (0x04) can only shift right for 2 bits
        {
             // mcSHOW_DBG_MSG(("Start fine search\n"));
            if (pVA1G5BF_AnaTuner->isBreak(pvArg))
            {
                mcSHOW_DBG_MSG(("[ATuner] SearchNearby.BreakFct\n"));
                mcSET_BIT(bATVStatus, cpANA_LOCK_STS_BREAK);
		           *wFreqInKHz = wFreqInKHzTmp;
		          //  *wFreqInKHz =wFRTmpScan;
                return bATVStatus;
                //return (FALSE);
            }
            bAFTStatus = bGetSCurveVol();

           if (bAFTStatus <= VA1G5BF_AFT_CENTER_VOL_HIGH)
            {
                  mcSHOW_DBG_MSG(("Fine search end\n"));
                wFRTmp = wFRTmpScan;
                break;
            }
            else if (bAFTStatus > VA1G5BF_AFT_CENTER_VOL_HIGH)
            {
                  mcSHOW_DBG_MSG(("Still skip 125KHz next time\n"));
                wFRTmpScan += (SCAN_MIDDLE_STEP_KHZ >>2);
            }
             param.Freq=wFRTmpScan;
            VA1G5BF_AnaTunerSetFreq(psAtvPiCtx,&param);
            bMaxStepNum ++;
        }
        if (bMaxStepNum == 3)
        {
              mcSHOW_DBG_MSG(("The fourth fine search\n"));
             param.Freq=wFRTmp;  
            VA1G5BF_AnaTunerSetFreq(psAtvPiCtx,&param);
	 
         }

        //*wFreqInKHz = wFRTmp;
        bATVStatus = mcBIT(cpANA_LOCK_STS_VIF);

        if (fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME))
        {
            mcSET_BIT(bATVStatus, cpANA_LOCK_STS_TVD);
			
	         bAFTStatus = bGetSCurveVol();
       if ((bAFTStatus >= VA1G5BF_AFT_CENTER_VOL_HIGH - 0x25) & (bAFTStatus <= VA1G5BF_AFT_CENTER_VOL_HIGH -0x15))
        {
                mcSHOW_DBG_MSG(("tvd success2+1\n"));
             //   VA1G5BF_AnaTunerSetFreq(psAtvPiCtx, wFRTmp+32,u1AnaMode);
                wFRTmp += 32;

        }
      else if ((bAFTStatus >= VA1G5BF_AFT_CENTER_VOL_HIGH - 0x15) & (bAFTStatus <= VA1G5BF_AFT_CENTER_VOL_HIGH))
        {
                mcSHOW_DBG_MSG(("tvd success2+2\n"));
             //   VA1G5BF_AnaTunerSetFreq(psAtvPiCtx, wFRTmp+63,u1AnaMode);
                wFRTmp += 63;
        }

       else if((bAFTStatus  >= VA1G5BF_AFT_CENTER_VOL_LOW) & (bAFTStatus <= VA1G5BF_AFT_CENTER_VOL_LOW + 0x15))
        {
                mcSHOW_DBG_MSG(("tvd success2-1\n"));
               // VA1G5BF_AnaTunerSetFreq(psAtvPiCtx, wFRTmp-60,u1AnaMode);
                wFRTmp -= 60;
        }
        else if((bAFTStatus  >= VA1G5BF_AFT_CENTER_VOL_LOW + 0x15) & (bAFTStatus <= VA1G5BF_AFT_CENTER_VOL_LOW+0x25))
        {
                 mcSHOW_DBG_MSG(("tvd success2-1\n"));
               // VA1G5BF_AnaTunerSetFreq(psAtvPiCtx, wFRTmp-30,u1AnaMode);
                wFRTmp -= 30;               
        }
			  param.Freq=wFRTmp;
			  VA1G5BF_AnaTunerSetFreq(psAtvPiCtx,&param);
	     PreLockFreq = wFRTmp;
	    *wFreqInKHz = wFreqInKHzTmp;
	   // *wFreqInKHz=wFRTmp;
	      #if fcTIMING_MEASURE
        mcSHOW_DBG_MSG(("[ATuner] SearchNearby.1=%3lu ms, %lu KHz\n",(mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD(),*wFreqInKHz-wFreqInKHzTmp));
        #endif
            return bATVStatus;
        }
        else
        { 
        	 #if fcTIMING_MEASURE
          mcSHOW_DBG_MSG(("[ATuner] SearchNearby.2=%3lu ms, %lu KHz\n",(mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD(),wFRTmp-wFreqInKHzTmp));
           #endif
            PreLockFreq =wFRTmp;
           *wFreqInKHz = wFreqInKHzTmp;
           
           //*wFreqInKHz=wFRTmp;
            return bATVStatus;
        }
    }
    else  // go to next channel
    {
    	 #if fcTIMING_MEASURE
      mcSHOW_DBG_MSG(("[ATuner] SearchNearby.3=%3lu ms, %lu KHz\n",(mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD(),wFRTmp-wFreqInKHzTmp));
        #endif
        	if(bWeakSignalTVDFlag & 0x1)
		  {
		  	param.Freq=wFreqInKHzTmp;
			VA1G5BF_AnaTunerSetFreq(psAtvPiCtx,&param);
			mcSHOW_DBG_MSG(("###STD(1)\n"));
			 PreLockFreq = wFreqInKHzTmp;
			 *wFreqInKHz = wFreqInKHzTmp;
	              mcSET_BIT(bATVStatus, cpANA_LOCK_STS_VIF);
			mcSET_BIT(bATVStatus, cpANA_LOCK_STS_TVD);
			return bATVStatus;
		  }

		else // go to next channel
		{
			          PreLockFreq=wFRTmp-SCAN_MIDDLE_STEP_KHZ;
	              *wFreqInKHz = wFreqInKHzTmp;
	              return FALSE;
		}
    }
     
}

//----------------------------------------------------------------------------- 
/*
 *  VA1G5BF_MonitorAnaTuner
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
#if 1
UINT8 VA1G5BF_MonitorAnaTuner(ATV_PI_CTX_T *psAtvPiCtx, UINT32 *wFreqInKHz,
                              x_break_fct _BreakFct, void *pvArg)
{
ATV_PI_CTX_T *pVA1G5BF_AnaTuner = psAtvPiCtx;
UINT8 bAFTStatus;
UINT8 bATVStatus = mcBIT(cpANA_LOCK_STS_VIF) | mcBIT(cpANA_LOCK_STS_TVD);
UINT8 u1AnaMode = pVA1G5BF_AnaTuner->u1AnaMode;
//UINT8 u1AnaMode = u1MonitorAnaMode;
static UINT8 VA1G5BF_Monitor_Counter = 0;
static UINT8 VA1G5BF_SearchCnt = 0;
static INT32 wFreqInKHz2 = 0;  // when would wFreqInKHz2 be reset to zero?
UINT32 wFreqInKHz1;
UINT32 wFreqBoundary = TV_FINE_TUNE_THRESHOLD_KHZ/2;
PARAM_SETFREQ_T param;
param.Modulation=u1AnaMode;
param.fgAutoSearch=0;



		wFreqInKHz1 = PreLockFreq;

    pVA1G5BF_AnaTuner->isBreak = _BreakFct;
   
   mcSHOW_DBG_MSG3(("*wFreqInKHz =%d KHz,PreLockFreq=%d KHz \n",*wFreqInKHz,PreLockFreq));

{
    UINT8  	TVD_TMP,TVD_TMP2;
    TVD_TMP = (IO_READ8(VIDEO_IN0_BASE, 0x81) & 0x10);    

    if (TVD_TMP == 0x10)
    {
        mcSHOW_DBG_MSG3(("Vpress = 1;\n"));
        TVD_TMP2 = IO_READ8(VIDEO_IN0_BASE, 0x80); 
        if(TVD_TMP2 < 0x15)
        {
            mcSHOW_DBG_MSG3(("Noise < 0x15 (%x)\n",TVD_TMP2));
            bAFTStatus = bGetSCurveVol();
            if ((bAFTStatus <= VA1G5BF_AFT_CENTER_VOL_HIGH_MONI ) && (bAFTStatus >= VA1G5BF_AFT_CENTER_VOL_LOW_MONI))
            {
                if(wFreqInKHz2 != 0){
                    PreLockFreq = wFreqInKHz1 + wFreqInKHz2;
                }
                   // *wFreqInKHz = PreLockFreq; // update final freq
                    wFreqInKHz2 = 0;
                    VA1G5BF_Monitor_Counter = 0;
                    VA1G5BF_SearchCnt = 0;
                    return bATVStatus;
            }
            else
            {
               if (pVA1G5BF_AnaTuner->isBreak)
                {
                    if (pVA1G5BF_AnaTuner->isBreak(pvArg))
                    {
                        mcSHOW_DBG_MSG(("[ATuner] SearchNearby.BreakFct\n"));
                        return (FALSE);
                    }
                mcSHOW_DBG_MSG(("Out of range, bCurveStatus = %02X (%d),VA1G5BF_Monitor_Counter= %d\n", bAFTStatus, bAFTStatus,VA1G5BF_Monitor_Counter));
                mcSHOW_DBG_MSG(("out of range\n"));
                if(VA1G5BF_Monitor_Counter>=1)
                {
                    DigTunerBypassI2C(TRUE);
                    if(((wFreqInKHz1 + wFreqInKHz2) <= (*wFreqInKHz + wFreqBoundary)) && 
                       ((wFreqInKHz1 + wFreqInKHz2) >= (*wFreqInKHz - wFreqBoundary)))
                    {	 
                        if(bAFTStatus <= VA1G5BF_AFT_CENTER_VOL )
                        {
                            wFreqInKHz2 -= 50;
                          //  *wFreqInKHz = wFreqInKHz1 + wFreqInKHz2; // update final freq
                          //  VA1G5BF_AnaTunerSetFreq(psAtvPiCtx, wFreqInKHz1 + wFreqInKHz2, u1AnaMode);	
                            mcSHOW_DBG_MSG(("-50KHz (%d) %x\n",wFreqInKHz1 + wFreqInKHz2,bAFTStatus));
                        }
                        else
                        {           			
                            wFreqInKHz2 += 50;
                        //    *wFreqInKHz = wFreqInKHz1 + wFreqInKHz2; // update final freq
                            //VA1G5BF_AnaTunerSetFreq(psAtvPiCtx, wFreqInKHz1 + wFreqInKHz2, u1AnaMode);	
                            mcSHOW_DBG_MSG(("+50KHz (%d) %x\n",wFreqInKHz1 + wFreqInKHz2,bAFTStatus));
                        }
                        param.Freq=wFreqInKHz1 + wFreqInKHz2;
                       VA1G5BF_AnaTunerSetFreq(psAtvPiCtx,&param);
                    }
                    else
                    {
                        bATVStatus = VA1G5BF_AnaTunerSearchNearbyFreq(psAtvPiCtx, wFreqInKHz,
                                                                      _BreakFct, pvArg);
                        wFreqInKHz2 = 0;           					              
                    }	           		
                    DigTunerBypassI2C(FALSE);
                }
                VA1G5BF_Monitor_Counter++;
            }
          }
        }
    }
    else
    {
        mcSHOW_DBG_MSG(("Vpress = 0;\n"));    	
        if(VA1G5BF_SearchCnt >= 1)
        {          
            mcSHOW_DBG_MSG(("VA1G5BF_SearchCnt >= 1\n"));
        	DigTunerBypassI2C(TRUE);
            bATVStatus = VA1G5BF_AnaTunerSearchNearbyFreq(psAtvPiCtx, wFreqInKHz,
                                                          _BreakFct, pvArg);
        	
            if(bATVStatus <= 1)
            {  // Improve Sentivity
        		UINT32 FreqTmp;
                if(*wFreqInKHz > PreLockFreq)
                    FreqTmp = *wFreqInKHz - PreLockFreq;
                else	
                    FreqTmp = PreLockFreq - *wFreqInKHz;
        			        	
                if(FreqTmp > wFreqBoundary)
                {
                    mcSHOW_DBG_MSG(("VA1G5BF_Monitor Freq %d : Rre_Freq %d\n",*wFreqInKHz,PreLockFreq));
                   // VA1G5BF_AnaTunerSetFreq(psAtvPiCtx, *wFreqInKHz, u1AnaMode);
                    PreLockFreq = *wFreqInKHz;
                }
                else
                {
                    mcSHOW_DBG_MSG(("Pre-VA1G5BF_Monitor Freq %d : Rre_Freq %d\n",*wFreqInKHz,PreLockFreq));
                   // VA1G5BF_AnaTunerSetFreq(psAtvPiCtx, PreLockFreq, u1AnaMode);
                   // *wFreqInKHz = PreLockFreq; // update final freq
                }
                param.Freq=PreLockFreq;
                 VA1G5BF_AnaTunerSetFreq(psAtvPiCtx,&param);
            }
        	
            wFreqInKHz2 = 0;
            DigTunerBypassI2C(FALSE);
        }
        VA1G5BF_SearchCnt++;
    }
}  

    return bATVStatus;
}
#endif
//----------------------------------------------------------------------------- 
/*
 *  VA1G5BF_AnaTunerGetVer
 *  Get Tuner Version.
 *  @param  void.
 *  @retval   VA1G5BF_ANA_TUNER_VER.
 */
//-----------------------------------------------------------------------------
UINT8 *VA1G5BF_AnaTunerGetVer(void){
    return ("analog VA1G5BF");
}
