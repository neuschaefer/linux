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
 * $RCSfile: pd_isdbt_glue.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
/** @file pd_isdbt_glue.c
 *  isdbt glue layer
 */
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "u_tuner.h"
#include "tuner_if.h"
#include "queue.h"
#include "TDIS.h"
#include "TDMgr.h"
#include "drvapi_tuner.h"

#include "pi_isdbt_if.h"
#include "pd_isdbt_if.h"
//#include "rf_tuner_api.h"
#include "tuner_interface_if.h"
#include "pd_tuner_type.h"
#include "ctrl_bus.h"
#include "UtilAPI.h"
#include "x_gpio.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//#if !defined(CC_MTAL) && !defined(CC_SDAL) //linux platform
//#define ISDBT_ENABLE_MONITOR
//#else

//#endif

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define mcSEMA_LOCK_MTFE(sema)                                       \
{                                                                    \
    mcSHOW_DBG_MSG4((" [ _Lock_%s-%d ]\r\n", __FUNCTION__, __LINE__)); \
    x_sema_lock(sema, X_SEMA_OPTION_WAIT);                           \
    mcSHOW_DBG_MSG4((" [ _Excape_%s-%d ]\r\n", __FUNCTION__, __LINE__));     \
}
#define mcSEMA_UNLOCK_MTFE(sema)                                     \
{                                                                    \
    mcSHOW_DBG_MSG4((" [ _Unlock_%s-%d ]\r\n", __FUNCTION__, __LINE__));     \
    x_sema_unlock(sema);                                             \
}

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static ISDBT_PD_CTX_T *pPdCtx;
static DEMOD_CTX_T *psDemodCtx;
//static GEN_TUNER_CTX_T *pTunerCtx;
static TUNER_DEVICE_CTX_T*		pISDBTunerDeviceCtx = NULL;

//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------
EXTERN U8 Register_[0x100];

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
//#ifdef ISDBT_ENABLE_MONITOR
static BOOL SetBreakMonitor(ISDBT_PD_CTX_T *psPdCtx)
{
    UINT8 u1Cnt=0;
    
    psPdCtx->fgBreakMonitor = TRUE; //set to break monitor

    if(psPdCtx->fgPIMoniStatus){  //PI really doing PI Monitor
        while(psPdCtx->fgPIMoniStatus){ 
            //wait PI Monitor really finish break 
            if(u1Cnt>250){  //in case of deadlock
                break;   
            }
            u1Cnt++;
            mcSHOW_DBG_MSG(("ANA EU PD wait break PI monitor finish. u1Cnt=%d\r\n",u1Cnt));
            mcDELAY_MS(10); 
        }            
    }
    
    return TRUE;
}
static void ResumeMonitor(ISDBT_PD_CTX_T *psPdCtx)
{
    psPdCtx->fgBreakMonitor = FALSE;
    mcMUTEX_UNLOCK(psPdCtx->t_escape_mon_suspend);
}
#if 0 //Not used                      
/************************************************************************
*  i4AnaEUMonitorBreak
*  Break function of monitor thread.
*  @param  *pvArg : A pointer to ATV_PD_CTX_T structure.
*  @retval  Break flag of break function.
************************************************************************/
STATIC INT32 i4ISDBTMonitorBreak(VOID *pvArg)                                                            
{                                                                                                       
    ISDBT_PD_CTX_T *psPdCtx = (ISDBT_PD_CTX_T *) pvArg;
                                                                                                        
    if (psPdCtx->fgBreakMonitor)
    {                                                                                                   
        mcSHOW_DBG_MSG(("ISDBT_Monitor break function triggered!!\n"));                                      
    }                                                                                                   
    return (psPdCtx->fgBreakMonitor);
}                                           

#endif
/************************************************************************
*  MonitorProcess
*  Monitor thread process function.
*  @param  void.
*  @retval  void.
************************************************************************/
STATIC void MonitorProcess(VOID *pvArg)
{
    
    ISDBT_PD_CTX_T *psPdCtx = *(ISDBT_PD_CTX_T **) pvArg;
    BOOL fgBer=TRUE;
    UINT32 u1Ber=0;
    //UINT8  BerBuff[8];
    //UINT32 Snr;
    //UINT8 Reg[2];
    
    mcSHOW_DRVAPI_MSG(("MonitorProcess start!\n"));
        
    while (psPdCtx->fgMonitorThreadRunning)
    {
      //  Snr = 0;
    	if (psPdCtx->fgBreakMonitor)
    	{
    		// After mcMUTEX_LOCK(t_escape_mon_suspend), monitor thread will be suspended until
    		// TunerAcq or TunerClose invoked.
            mcSHOW_DRVAPI_MSG(("Entering ISDBT mon_suspend\n"));
    		mcMUTEX_LOCK(psPdCtx->t_escape_mon_suspend);
    		mcSHOW_DRVAPI_MSG(("Escape ISDBT mon_suspend\n"));
    	}
    	else
    	{
		    // mutex lock for I2C access
            mcSEMA_LOCK_MTFE(psPdCtx->hHalLock);
		    psPdCtx->fgPIMoniStatus = TRUE; //doing PI monitor
	        mcSHOW_DBG_MSG2(("Invoke ISDBT_Monitor\n"));
		
		      
            //! [TODO:PI] put your monitor process code here
            /////BEGIN
            // BER
            //PI_ISDBT_GetBER(psDemodCtx, BerBuff);	
           u1Ber= ISDBT_GetBer(psDemodCtx,fgBer);
           if(fgBer)
           	{
           		psDemodCtx->BER=u1Ber;
           	  fgBer=FALSE;
           	}
           else
           	{
           		psDemodCtx->PER=u1Ber;
           	  fgBer=TRUE;
           	}
           #if 0
            psDemodCtx->BER = BerBuff[2]<<16 | BerBuff[1]<<8 | BerBuff[0];
            switch(BerBuff[3] & 0xC0)
            {
            case 0x00:  // 1,00E+05
                break;
            case 0x40:  // 1,00E+06
                psDemodCtx->BER /= 10;
                break;
            case 0x80:  // 1,00E+07
                psDemodCtx->BER /= 100;
                break;
            case 0xc0:  // 1,00E+08
                psDemodCtx->BER /= 1000;
                break;
            }

            //SNR
            //PI_ISDBT_GetSNR(psDemodCtx, &Snr);
            switch (psDemodCtx->m_iQamsize)
            {
            case 1:
                psDemodCtx->SNR = (((195000)/(32*Snr + 1380)) + 10);
                break;

            case 2:
                psDemodCtx->SNR = (((215000+20*Snr+2500)/(40*Snr + 5000)) + 13);
                break;

            case 3:
                psDemodCtx->SNR = (((210000+20*Snr+2500)/(40*Snr + 5000)) + 12);
                break;

            case 4:
                psDemodCtx->SNR = (((185000+30*Snr+3200)/(38*Snr + 4000)) + 13);
                break;

            case 5:
                psDemodCtx->SNR = (((180000+30*Snr+1200)/(100*Snr + 4000)) + 20);
                break;
            }
            /////END
            #endif
            psPdCtx->fgPIMoniStatus = FALSE; //finish doing PI monitor or breaked
            mcSEMA_UNLOCK_MTFE(psPdCtx->hHalLock);                
	        mcDELAY_MS(500); 
        }
    }
    
    psPdCtx->t_monitor_thread = NULL;
    mcSHOW_DRVAPI_MSG(("MonitorProcess exit!\n"));
    mcTHREAD_DESTROY();
}

/************************************************************************
*  PD_StartMonitorEngine
*  Function which starts monitor thread engine.
*  @param  *psAtvPdCtx : A pointer to ATV_PD_CTX_T structure.
*  @retval  DRVAPI_TUNER_OK : Monitor thread create successfully.
*  @retval  DRVAPI_TUNER_ERROR : Monitor thread create fail.
************************************************************************/
static S32 PD_StartMonitorEngine(ISDBT_PD_CTX_T *psPdCtx)
{
    psPdCtx->fgMonitorThreadRunning = TRUE;
    if (mcTHREAD_CREATE(&psPdCtx->t_monitor_thread,
            "EVBC_MonThread",
            cTHREAD_STACK_SIZE, cTHREAD_PRIORITY,
            MonitorProcess, sizeof(void*), (VOID *) &psPdCtx) != OSR_OK)
    {
        mcSHOW_DBG_MSG(("PD_StartMonitorEngine (mcTHREAD_CREATE): DRVAPI_TUNER_ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    mcSHOW_DBG_MSG(("PD_StartMonitorEngine success\n"));
    return (DRVAPI_TUNER_OK);
}

/************************************************************************
*  PD_StopMonitorEngine
*  Function which stops monitor thread engine.
*  @param  void.
*  @retval  DRVAPI_TUNER_OK : Stop monitor thread successfully.
************************************************************************/
static S32 PD_StopMonitorEngine(void)
{
//    pPdCtx->fgMonitorThreadRunning = FALSE; // must set to FALSE before unlock sema t_escape_mon_suspend
    while (pPdCtx->t_monitor_thread){
        mcDELAY_MS(10); // in ms
    }
    mcSHOW_DRVAPI_MSG(("ISDBT PD_StopMonitorEngine success\n"));
    return (DRVAPI_TUNER_OK);
}
//#endif //ISDBT_ENABLE_MONITOR
//-------------------------------------------------------------------------
/**
 *  This API implement close tuner. It should performs
 *  1. free all allocated resource.
 *  2. do demod/tuner initialization.
 *  3. do LTDIS API mapping
 *  @param   ptTDSpecificCtx   Pointer to tuner driver context.
 *  @retval  DRVAPI_TUNER_OK    Success
 *  @retval  DRVAPI_TUNER_ERROR Fail
 */
//-------------------------------------------------------------------------
static INT32 ISDBT_TunerClose(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    
//#if !defined(CC_MTAL) && !defined(CC_SDAL)    
//#ifdef ISDBT_ENABLE_MONITOR
	if(pISDBTunerDeviceCtx->u1MWIntf != cMTAL_IF && pISDBTunerDeviceCtx->u1MWIntf != cSDAL_IF)
	{
	    pPdCtx->fgBreakMonitor = TRUE; //disable and breadk monitor thread
	    pPdCtx->fgMonitorThreadRunning = FALSE; // must set to FALSE before unlock sema t_escape_mon_suspend
	    mcMUTEX_UNLOCK(pPdCtx->t_escape_mon_suspend);
	    PD_StopMonitorEngine();  // stop monitor engine
	    mcMUTEX_DESTROY(pPdCtx->t_escape_mon_suspend);  // destory monitor thread
	}
//#endif

    //add driver implementation code here
    x_sema_delete(pPdCtx->hHalLock);
    x_mem_free(pPdCtx);
    x_mem_free(psDemodCtx);

    //If you have allocated any resource in TunerOpen(), release them here.

    return (DRVAPI_TUNER_OK);
}
//-------------------------------------------------------------------------
/**
 *  This API do channel acuqusition.
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @param   pv_conn_info       middleware tuner info structure, refer u_tuner.h
 *  @param   z_conn_info_len    length of pv_conn_info
 *  @param   _BreakFct          TunerHAL callback API for Vendor_TunerAcq to check whether need to break TunerAcq
 *  @param   *pvArg             pass this parameter to _BreakFct when calling _BreakFct
 *  @retval  1                  TPS LOCK
 *  @retval  0                  TPS UNLOCK
 */
//-------------------------------------------------------------------------
static BOOL ISDBT_TunerAcq(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type, void* pv_conn_info,
                            SIZE_T z_conn_info_len, x_break_fct _BreakFct, void* pvArg)
{
    INT32   i4State;
    BOOL    fgRetSts = FALSE;
    UINT32  freq;
    UINT32  symbolRate;
    //INT32   qamSize;
    BOOL    oobSpecInv;
    TUNER_MODULATION_T  mode;
    //INT32   modeEnum;
    UINT32  ii;
    UINT8   LockStatus = FALSE;//, u1Qam
    BOOL fgSigSts,fgPreSigSts=FALSE;//suppose signal present previous time
    PARAM_SETFREQ_T param;
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    
//#ifdef ISDBT_ENABLE_MONITOR
	if(pISDBTunerDeviceCtx->u1MWIntf != cMTAL_IF && pISDBTunerDeviceCtx->u1MWIntf != cSDAL_IF)
	{
    	SetBreakMonitor(pPdCtx);
	}
//#endif

    //! [TODO:PD] translate necessary input informations
    /////BEGIN
    // check whether middleware will change TUNER_TER_DIG_TUNE_INFO_T definition for DVB-C implementation
    i4State = Util_TranslateConnectInfo(e_conn_type, pv_conn_info, z_conn_info_len,
                         &freq, &symbolRate, &mode, &oobSpecInv);

    if (i4State != RMR_OK)
    {
        mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

        mcSHOW_DBG_MSG((" Invalid input (%ld) to ISDBT_TunerAcq!\n", i4State));
        return fgRetSts;
    }

    //middleware's definition, if e_mode=MOD_UNLNOWN, middleware is doing channel scan mode
    //otherwise it is doing change change
    /////END

    pPdCtx->isBreak = _BreakFct;

    //! [TODO:PI] add Init, SetTuner/Demod, Acquisition...
    /////BEGIN
//    if (Tuner_SetFreq(psDemodCtx, freq, MOD_DVBT, fgChannelScan)){
    //u1Qam = 0xC0;
    //ICtrlBus_I2cDemodWrite(psDemodCtx->I2cAddress, 0x14, &u1Qam, 1);
    mcSHOW_DBG_MSG(("ISDB-T Tuner_SetFreq(%d KHz)\n", freq/1000));
    param.Freq=freq/1000;
    param.Modulation=MOD_ISDBT;
    param.fgAutoSearch=1;
    if (ITuner_SetFreq(psDemodCtx->sTunerCtx,&param)){
        mcSHOW_USER_MSG(("Failed on ISDBT(%d KHz) \n", freq/1000));
    }

    if (ISDBT_Connect(psDemodCtx)){
        mcSHOW_USER_MSG(("Failed on ISDBT_Connect!! \n"));
    }

    //PI_ISDBT_Initialize(psDemodCtx, 0x18);
    //PI_ISDBT_SetFreq(psDemodCtx, freq);
    //PI_ISDBT_SetSymRate(psDemodCtx, symbolRate);
    //PI_ISDBT_SetQamMode(psDemodCtx, modeEnum);
    //PI_ISDBT_Run(psDemodCtx);
    /////END

    //! [TODO:PI] add correct way for GetSync
    /////BEGIN
    for (ii = 0; ii < 100; ii ++)
    {
    	//LockStatus = PI_ISDBT_GetLockStatus(psDemodCtx);

        //Front end     
        if (ISDBT_GetSync(psDemodCtx))
        {
            LockStatus = TRUE;
            break;
        }
        fgSigSts=ISDBT_IsNoSignal(psDemodCtx);
        if(fgSigSts && fgPreSigSts)//No signal,Early break
        	{
        		  mcSHOW_DRVAPI_MSG(("Early break at the %d time\n",ii));
        	   ii=100;
        	   break;
        	}
        fgPreSigSts=fgSigSts;
        mcDELAY_MS(10);
        
        if (pPdCtx->isBreak(pvArg))
        {
            break;
        }
    }
    /////END

    if (ii == 100)
    {
        mcSHOW_DRVAPI_MSG(("Front-end unlock\n"));
    }
    else
    {
        mcSHOW_DRVAPI_MSG(("Front-end lock!, ii=%d\n", ii));
    }
    
//#ifdef ISDBT_ENABLE_MONITOR
	if(pISDBTunerDeviceCtx->u1MWIntf != cMTAL_IF && pISDBTunerDeviceCtx->u1MWIntf != cSDAL_IF)
	{
	    if (!_BreakFct(pvArg))// && (u1ConnMode == ATV_CONN_MODE_CHAN_CHANGE))
	    {
	        //psAtvPdCtx->fgBreakMonitor = FALSE;           // Enable monitor thread and Disable break function flag
	        //mcMUTEX_UNLOCK(t_escape_anaeu_mon_suspend);  // Unlock monitor thread
	        ResumeMonitor(pPdCtx);   // Enable monitor thread and Disable break function flag,  Unlock monitor thread
	        mcSHOW_DBG_MSG(("Enable monitor process!!\n"));
	    }
	}
//#endif

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    //! [TODO:PI] return correct lock status
    /////BEGIN
    return (LockStatus);    //if lock return 1, unlock return 0
    /////END

//    return TRUE;
}
//-------------------------------------------------------------------------
/**
 *  This API check lock status
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  1                  LOCK
 *  @retval  0                  UNLOCK
 */
//-------------------------------------------------------------------------
static INT16 ISDBT_TunerGetSync(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT16 u2Ret=0;

    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here

    u2Ret = ISDBT_GetSync(psDemodCtx);

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return u2Ret;
}

//-------------------------------------------------------------------------
/**
 *  This API do disconnect tuner
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  void
 */
//-------------------------------------------------------------------------
static VOID  ISDBT_TunerDisc(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here

//#if !defined(CC_MTAL) && !defined(CC_SDAL)
//#ifdef ISDBT_ENABLE_MONITOR
	if(pISDBTunerDeviceCtx->u1MWIntf != cMTAL_IF && pISDBTunerDeviceCtx->u1MWIntf != cSDAL_IF)
	{
    	SetBreakMonitor(pPdCtx);
	}
//#endif

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}

//-------------------------------------------------------------------------
/**
 *  This API get modulation information.
 *  Inputs:
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  Outputs:
 *  @param   *_pSignal          Modulation info. structure to be filled.
 *  @retval  void
 */
//-------------------------------------------------------------------------
static void ISDBT_TunerGetSignal(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type, SIGNAL *_pSignal)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here
    _pSignal->Frequency  = psDemodCtx->sTunerCtx->u4RF_Freq;
    _pSignal->e_bandwidth = BW_6_MHz;

    // not used in isdbt
    _pSignal->SymbolRate = 0;
    _pSignal->QAMSize = 0;
    _pSignal->e_hierarchy_priority = 0;

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

}

//-------------------------------------------------------------------------
/**
 *  This API get signal level in terms of percentage.
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  0~100              signal level
 */
//-------------------------------------------------------------------------
static UINT8 ISDBT_TunerGetSignalLevel(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT8 u1Ret=0;
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here
    //u1Ret = Vendor PI get signal level, return vlaue should be range from 0 ~ 100
    u1Ret = ISDBT_GetSignalLevel(psDemodCtx);

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    //example of return
    return u1Ret;
}

//-------------------------------------------------------------------------
/**
 *  This API get signal level in multiples of (0.1dBm).
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  0 ~ -900           signal level
 */
//-------------------------------------------------------------------------
static INT16 ISDBT_TunerGetSignalLeveldBm(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    INT16 i2Ret=0;
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here
    //i2Ret = Vendor PI get signal level in dBm, return vlaue should be range from 0 ~ -900

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return i2Ret;
}

static UINT16 ISDBT_TunerGetSignalPER(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
//    INT32 u4_ret;
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    return 0;//modify return value
}
/*
static UINT32 ISDBT_TunerGetSignalBER(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
   mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    return 0;//u4_ret;//modify return value
}
*/
static UINT16 ISDBT_TunerGetSignalSNR(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    //UINT8 SNR;
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    return 0;//modify return value
}

static void ISDBT_TunerSetMpgFmt(PTD_SPECIFIC_CTX ptTDSpecificCtx, MPEG_FMT_T  *pt_mpeg_fmt)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}

static char *ISDBT_TunerGetVer(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return NULL;
}

static INT32 ISDBT_TunerNimTest(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return 0;
}
static void ISDBT_TunerSetRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx, UCHAR ucRegSet, UCHAR ucRegAddr, UCHAR ucRegValue)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}

static void ISDBT_TunerShowRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}

static void ISDBT_TunerTestI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}
static void ISDBT_TunerCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx, INT32 i4Argc, const CHAR** aszArgv)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}

static void ISDBT_TunerDtdCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,INT32 i4Argc, const CHAR** aszArgv)
{
    UCHAR   ucCmdId = 0, ucPar, ucData[255];
    U16    ii, jj, u2RegAddr;
    U32 u4Freq;
     PARAM_SETFREQ_T param;
                   
    if (i4Argc < 1)
    {
        mcSHOW_USER_MSG(("\t s:                   show signal/ts/tps status\n"));
        mcSHOW_USER_MSG(("\t f [Freq]:            set freq (kHz)\n"));
        mcSHOW_USER_MSG(("\t r [RegAddr] [Num]:   read  Demod register\n"));
        mcSHOW_USER_MSG(("\t w [RegAddr] [Value]: write Demod register\n"));        
    }	

    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
   
    if (i4Argc > 0)
        ucCmdId = *((CHAR *) aszArgv[0]);
        
    switch (ucCmdId)
    {
  
    case 's':
        mcSHOW_USER_MSG(("=================================\n"));
        mcSHOW_USER_MSG(("Front-end Lock = %d\n", ISDBT_GetSync(psDemodCtx)));
        mcSHOW_USER_MSG(("VBER = %d *10^-8 \n", ISDBT_GetBer(psDemodCtx, TRUE)));
        mcSHOW_USER_MSG(("\n"));

        ISDBT_UpdateModPara(psDemodCtx);
        switch (ISDBT_GetFftSize()) {
        case ISDBT_FFT_SIZE_2K: mcSHOW_USER_MSG(("FFT = 2K\n")); break;
        case ISDBT_FFT_SIZE_4K: mcSHOW_USER_MSG(("FFT = 4K\n")); break;
        case ISDBT_FFT_SIZE_8K: mcSHOW_USER_MSG(("FFT = 8K\n")); break;
        default:                mcSHOW_USER_MSG(("FFT = ??\n")); break;
        }
        switch (ISDBT_GetGiSize()) {
        case ISDBT_GI_SIZE_1_32: mcSHOW_USER_MSG(("GI = 1/32\n")); break;
        case ISDBT_GI_SIZE_1_16: mcSHOW_USER_MSG(("GI = 1/16\n")); break;
        case ISDBT_GI_SIZE_1_8 : mcSHOW_USER_MSG(("GI = 1/8 \n")); break;
        case ISDBT_GI_SIZE_1_4 : mcSHOW_USER_MSG(("GI = 1/4 \n")); break;
        default:                 mcSHOW_USER_MSG(("GI = ??? \n")); break;
        }
        mcSHOW_USER_MSG(("Layer:    A  /  B  /  C\n"));

        mcSHOW_USER_MSG(("QAM:    "));
        for (ii=0; ii<3; ii++) {
            switch(ISDBT_GetMod(ii)){
            case ISDBT_MOD_DQPSK:   mcSHOW_USER_MSG(("DQPSK ")); break;
            case ISDBT_MOD_QPSK:    mcSHOW_USER_MSG((" QPSK ")); break;
            case ISDBT_MOD_16QAM:   mcSHOW_USER_MSG(("16QAM ")); break;
            case ISDBT_MOD_64QAM:   mcSHOW_USER_MSG(("64QAM ")); break;
            case ISDBT_MOD_NOLAYER: mcSHOW_USER_MSG(("  NA  ")); break;
            default:                mcSHOW_USER_MSG((" ???  ")); break;
            }
        }
        mcSHOW_USER_MSG(("\n"));

        mcSHOW_USER_MSG(("CR:     "));
        for (ii=0; ii<3; ii++) {
            switch(ISDBT_GetCr(ii)){
            case ISDBT_CR_1_2:     mcSHOW_USER_MSG((" 1/2  ")); break;
            case ISDBT_CR_2_3:     mcSHOW_USER_MSG((" 2/3  ")); break;
            case ISDBT_CR_3_4:     mcSHOW_USER_MSG((" 3/4  ")); break;
            case ISDBT_CR_5_6:     mcSHOW_USER_MSG((" 5/6  ")); break;
            case ISDBT_CR_7_8:     mcSHOW_USER_MSG((" 7/8  ")); break;
            case ISDBT_CR_NOLAYER: mcSHOW_USER_MSG(("  NA  ")); break;
            default:               mcSHOW_USER_MSG((" ???  ")); break;
            }
        }
        mcSHOW_USER_MSG(("\n"));

        mcSHOW_USER_MSG(("IntLv: "));
        for (ii=0; ii<3; ii++) {
            mcSHOW_USER_MSG(("  %2d  ", ISDBT_GetInterleave(ii)));
        }
        mcSHOW_USER_MSG(("\n"));

        mcSHOW_USER_MSG(("Seg:   "));
        for (ii=0; ii<3; ii++) {
            mcSHOW_USER_MSG(("  %2d  ", ISDBT_GetSegment(ii)));
        }
        mcSHOW_USER_MSG(("\n"));
        
//            mcSHOW_USER_MSG(("SNR = %d\n", ISDBT_TunerGetSignalSNR(ptTDSpecificCtx) ));
//            mcSHOW_USER_MSG(("PreRS BER = %d * 10^-5\n", ISDBT_GetPostVBer(ptTDSpecificCtx)));
        mcSHOW_USER_MSG(("=================================\n"));
      
        break;

    case 'f':
        if (i4Argc < 2)
            break;
       
        u4Freq = (U32)StrToInt(aszArgv[1]); // freq
        param.Freq=u4Freq;
        param.Modulation=MOD_ISDBT;
        param.fgAutoSearch=0;
        if (ITuner_SetFreq(psDemodCtx->sTunerCtx,&param)){
            mcSHOW_USER_MSG(("Failed on ITuner_SetFreq(%d KHz)", u4Freq));
        }
        else{
        	   
            mcSHOW_USER_MSG(("ITuner_SetFreq(%d KHz) OK!", u4Freq));
            if (ISDBT_Connect(psDemodCtx)){
              mcSHOW_USER_MSG(("Failed on ISDBT_Connect!! \n"));
          }
         }
        break;

    case 'd':        
        mcSHOW_USER_MSG((" MTDVBT Register Page Dump Result:\n"));
        mcSHOW_USER_MSG(("     00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n"));
        for (ii=0; ii<16; ii++)
        {
            mcSHOW_USER_MSG(("0x%1Xx ", ii));
            for (jj=0; jj<16; jj++){
                ICtrlBus_I2cDemodRead(270,psDemodCtx->I2cAddress, ii*16, ucData, 16);
                mcSHOW_USER_MSG(("%02x ", ucData[jj]));
            }
            mcSHOW_USER_MSG(("\n"));
        }
        break;

    case 'r':
        if (i4Argc < 3)
            break;
            
        u2RegAddr = (U16)StrToInt(aszArgv[1]);
        ucPar = (U8)StrToInt(aszArgv[2]);
        if (ICtrlBus_I2cDemodRead(270,psDemodCtx->I2cAddress, u2RegAddr, ucData, ucPar))
        {        
            mcSHOW_USER_MSG(("ISDB-T Get Register 0x%03X Fail!\n", u2RegAddr));
            break;
        }          
        for (ii=0; ii<ucPar; ii++)
        {
            mcSHOW_USER_MSG(("ISDB-T Register 0x%03X = 0x%02X\n", u2RegAddr+ii, ucData[ii]));
        }
        break;

    case 'w':
        if (i4Argc < 3)
            break;
        
        u2RegAddr = (U16)StrToInt(aszArgv[1]);
        ucPar = (U8)StrToInt(aszArgv[2]);
        if (ICtrlBus_I2cDemodWrite(270,psDemodCtx->I2cAddress, u2RegAddr, &ucPar, 1))
        {        
            mcSHOW_USER_MSG(("ISDB-T Set Register 0x%03X Fail!\n", u2RegAddr));
            break;
        }          
        mcSHOW_USER_MSG(("ISDB-T Register 0x%03X = 0x%02X, OK!\n", u2RegAddr, ucPar));
        break;

    default:
        break;
    }    

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}

static void ISDBT_TunerDetachI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bDetach)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}

//-------------------------------------------------------------------------
/**
 *  This API get attribute from tuner driver
 *  Inputs:
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_get_type         tuner get type, refer x_tuner.h TUNER_GET_TYPE_ISDBT
 *  Outputs:
 *  @param   *pzAttribute       Attribute structure to be filled. Refer x_tuner.h structure definition TUNER_ISDBT_ATTRIBUTE_T
 *  @param   *pzAttributeLen    length to be filled.
 *  @retval  DRVAPI_TUNER_OK    success.
 *  @retval  DRVAPI_TUNER_ERROR fail.
 */
//-------------------------------------------------------------------------
static INT16 ISDBT_TunerGetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_GET_TYPE_T  e_get_type,
                                VOID *pzAttribute, SIZE_T* pzAttributeLen)
{
    static BOOL prevEwsVal = 0;
    EWS_INFO    *pEwsInfo;

    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here

    switch (e_get_type)
    {
    case TUNER_GET_SPECIAL_TYPE_EWS_STATUS:
        pEwsInfo = (EWS_INFO*)pzAttribute;

        pEwsInfo->val = ISDBT_GetEws(psDemodCtx);

        if (prevEwsVal != pEwsInfo->val)
            pEwsInfo->isNotify = TRUE;
        else
            pEwsInfo->isNotify = FALSE;
        
        prevEwsVal = pEwsInfo->val;
        *pzAttributeLen = sizeof(EWS_INFO);
        break;

    case TUNER_GET_TYPE_BER: //get attribute depend on what attribute middleware need to know. Here just give an example
        *(UINT32*)pzAttribute = psDemodCtx->BER/1000;//Return unit is 1e-5
        mcSHOW_DBG_MSG(("[%s]TUNER_GET_TYPE_BER ret %d ^1e-8\n",psDemodCtx->BER));
        *pzAttributeLen = sizeof(UINT32);
        break;
#if 0
    case TUNER_GET_TYPE_RAW_SIGNAL_LEVEL:
        *(INT8*)pzAttribute = Get signal level from Vendor PI APIs;
        *pzAttributeLen = sizeof(INT8);
        break;
#endif
    default:
        Printf("unknown: get Attribute\r\n");
        break;
    }
    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return DRVAPI_TUNER_OK;
}
//-------------------------------------------------------------------------
/**
 *  This API set attribute to tuner driver
 *  Inputs:
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_set_type         tuner set type, check x_tuner.h set operation TUNER_SET_TYPE_ISDBTX
 *  @param   *pvAttribute       set value.
 *  @param   zAttributeLen      length of *pvAttribute.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  DRVAPI_TUNER_OK    success.
 *  @retval  DRVAPI_TUNER_ERROR fail.
 */
//-------------------------------------------------------------------------
static INT32 ISDBT_TunerSetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_SET_TYPE_T  e_set_type,
                                        const VOID *pvAttribute, SIZE_T zAttributeLen)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here
#if 0
    switch (e_set_type)
    {
//set attribute depend on what attribute middleware need to set to driver. Here just give an example
    case TUNER_SET_TYPE_CI_CARD_TYPE:
        break;

    default:
        Printf("unknown: set Attribute\r\n");
        break;
    }
#endif

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return DRVAPI_TUNER_OK;
}
#ifdef fcADD_DIAG_INFO
static INT32 ISDBT_TunerSetDiagCmd(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CUSTOM_TUNER_SET_TYPE_T e_tuner_set_type, VOID* pv_set_info, SIZE_T z_size)
{
    INT32   retSts = RMR_OK;
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    return retSts;
}
static INT32 ISDBT_TunerGetDiagInfo(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CUSTOM_TUNER_GET_TYPE_T e_tuner_get_type, VOID* pv_get_info, SIZE_T* pz_size)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return 0;
}
#endif
//-----------------------------------------------------------------------------
// public functions
//-----------------------------------------------------------------------------
void PD_ISDBT_Register_LTDIS_Fct(PTD_CTX ptTDCtx)
{
    TUNER_DEVICE_CTX_T*		pTunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;
	
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerClose 	= ISDBT_TunerClose;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerAcq 	= ISDBT_TunerAcq;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGerSync 	= ISDBT_TunerGetSync;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignal 	= ISDBT_TunerGetSignal;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLevel = ISDBT_TunerGetSignalLevel;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLeveldBm = ISDBT_TunerGetSignalLeveldBm;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalPER = ISDBT_TunerGetSignalPER;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalSNR = ISDBT_TunerGetSignalSNR;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetMpgFmt 	= ISDBT_TunerSetMpgFmt;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetVer 	= ISDBT_TunerGetVer;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerNimTest 	= ISDBT_TunerNimTest;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetRegSetting = ISDBT_TunerSetRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerShowRegSetting = ISDBT_TunerShowRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerTestI2C 	= ISDBT_TunerTestI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerCommand 	= ISDBT_TunerCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDtdCommand 	= ISDBT_TunerDtdCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDetachI2C 	= ISDBT_TunerDetachI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDisc 	= ISDBT_TunerDisc;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetAttribute= ISDBT_TunerSetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute= ISDBT_TunerGetAttribute;
#ifdef fcADD_DIAG_INFO
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetDiagCmd	= ISDBT_TunerSetDiagCmd;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetDiagInfo = ISDBT_TunerGetDiagInfo;
#endif
}
//-------------------------------------------------------------------------
/**
 *  This API is driver entry point. It performs
 *  1. allocate and initialize resource.
 *  2. do demod/tuner initialization.
 *  3. do LTDIS API mapping
 *  @param   ptTDCtx            TunerHAL Context use for LTDIS API mapping
 *  @param   ptTunerCfgInfo     Tuner configuration information.
 *  @param   pptTDSpecificCtx   Pointer to tuner driver context.
 *  @retval  DRVAPI_TUNER_OK    Success
 *  @retval  DRVAPI_TUNER_ERROR Fail
 */
//-------------------------------------------------------------------------
INT32 ISDBT_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* pptTDSpecificCtx)
{
    //create pdctx used in this file, you also can add any declaration you want in pdctx
    INT32 retsts;

	pISDBTunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;
    pPdCtx = (ISDBT_PD_CTX_T *) x_mem_calloc(1, sizeof(ISDBT_PD_CTX_T));
    if (pPdCtx == NULL)
    {
        Printf("DVB_PD_CTX_T malloc fail\n");
        return (DRVAPI_TUNER_ERROR);
    }
    *pptTDSpecificCtx = pPdCtx;

    //create demod ctx
    psDemodCtx = (DEMOD_CTX_T *) x_mem_calloc(1, sizeof(DEMOD_CTX_T));
    if (psDemodCtx == NULL)
    {
        Printf("DEMOD_CTX_T malloc fail\n");
        return (DRVAPI_TUNER_ERROR);
    }

    //Create a semaphore for low level demod PI access protection
    if (x_sema_create(&pPdCtx->hHalLock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        Printf("%s: DVB x_sema_create(pPdCtx->hHalLock) Fail!\n",__FUNCTION__);
        return (DRVAPI_TUNER_ERROR);
    }
    x_sema_unlock(pPdCtx->hHalLock);

    //register LTDIS APIs to HAL
    PD_ISDBT_Register_LTDIS_Fct(ptTDCtx);

    //pTunerCtx = &(pPdCtx->sTunerCtx);
    
    //! [TODO:PI Tuner]
    //psDemodCtx->sTunerCtx.? = ?;
    psDemodCtx->sTunerCtx=ITunerGetCtx();//Get address of piTunerCtx
   // psDemodCtx->sTunerCtx.psDemodCtx = psDemodCtx;
  //  psDemodCtx->sTunerCtx->I2cAddress = 0x30;
   // psDemodCtx->sTunerCtx->u2IF_Freq  = 44000;

    pPdCtx->pDemodCtx = psDemodCtx;
     retsts=ITunerRegistration(((TUNER_DEVICE_CTX_T*)ptTDCtx)->tunerType);
     ITuner_Init(psDemodCtx->sTunerCtx);	
if(DRVAPI_TUNER_ERROR==retsts){
    return retsts;
}
    //! [TODO:PD] should be customization
    //FE initialization
    if(!ISDBT_FEInitialize(psDemodCtx)){
       return DRVAPI_TUNER_ERROR;//Initianlize fail
    }
    if (ISDBT_Connect(psDemodCtx)){
        mcSHOW_USER_MSG(("Failed on ISDBT_Connect!! \n"));
    }
//#ifdef ISDBT_ENABLE_MONITOR
	if(pISDBTunerDeviceCtx->u1MWIntf != cMTAL_IF && pISDBTunerDeviceCtx->u1MWIntf != cSDAL_IF)
	{
	    if (mcSEMA_CREATE(&pPdCtx->t_escape_mon_suspend, X_SEMA_STATE_LOCK) != OSR_OK)
	    {
	        mcSHOW_DRVERR_MSG(("EN_ISDBT_TunerOpen (mcSEMA_CREATE): ERROR!\n"));
	        return (DRVAPI_TUNER_ERROR);
	    }

	    pPdCtx->fgBreakMonitor = TRUE;//init
	    pPdCtx->fgPIMoniStatus = FALSE; //init
	    PD_StartMonitorEngine(pPdCtx);  //start monitor engine
	}
//#endif 

    //! [TODO:PI] add demod initialize API here
    /////BEGIN
    /////END

    return (DRVAPI_TUNER_OK);
}
VOID *GetDigiTunerCtx(VOID)
{
    if (!psDemodCtx)
    {
        Printf("[ISDBT ERR] ISDBT_TunerOpen hasn't been executed!!!\n");
    }
    return &(psDemodCtx->sTunerCtx);
}

