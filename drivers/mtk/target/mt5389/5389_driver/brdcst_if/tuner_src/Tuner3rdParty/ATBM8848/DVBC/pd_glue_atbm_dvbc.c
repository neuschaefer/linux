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
 * $RCSfile: pd_glue_atbm_DVBC.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
/** @file pd_glue_atbm_DVBC.c
 *  DVBC glue layer
 */
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "queue.h"



#include "tuner_if.h" //DRVAPI_TUNER_OK
#include "PI_Def.h"
#include "UtilAPI.h"
#include "ctrl_bus.h"
#include "pd_glue_atbm_dvbc.h"
#include "tuner_interface_if.h"
#include "pi_atbm_dvbc_api.h"
#include "TDIS.h" //Function table definition
#include "TDMgr.h"
#include "drvapi_tuner.h"
//#include "x_gpio.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
//Import extern variable
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
					   
//#if !defined(CC_MTAL) && !defined(CC_SDAL) //linux platform
//#define DVBC_ENABLE_MONITOR
//#endif


//-----------------------------------------------------------------------------
// Export definitions
//-----------------------------------------------------------------------------
//EXTERN UINT8 _fgAutoSearch;


//-----------------------------------------------------------------------------
// STATIC variables
//-----------------------------------------------------------------------------
STATIC PDVBC_PD_CTX_T pPdCtx;
STATIC pDVBC_Demod_Ctx psDemodCtx;
//STATIC GEN_TUNER_CTX_T *pTunerCtx;

static TUNER_DEVICE_CTX_T*		pDVBCunerDeviceCtx = NULL;

//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// STATIC functions
//-----------------------------------------------------------------------------

/*STATIC BOOL Resume_demod(void)
{
         BOOL ret;

         ret = Vendor_DemodReset();
		 ret &= Vendor_DemodInit();

        return ret;
}
*/
STATIC BOOL SetBreakMonitor(DVBC_PD_CTX_T *psPdCtx)
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
          //  mcDELAY_MS(10); 
        x_thread_delay((UINT32)(10));
        }            
    }
    
    return TRUE;
}

STATIC void ResumeMonitor(DVBC_PD_CTX_T *psPdCtx)
{
    psPdCtx->fgBreakMonitor = FALSE;
   // mcMUTEX_UNLOCK(psPdCtx->t_escape_mon_suspend);
   x_sema_unlock(psPdCtx->t_escape_mon_suspend);
}      
#if 0       
/************************************************************************
*  i4AnaEUMonitorBreak
*  Break function of monitor thread.
*  @param  *pvArg : A pointer to ATV_PD_CTX_T structure.
*  @retval  Break flag of break function.
************************************************************************/
STATIC INT32 i4DVBCMonitorBreak(VOID *pvArg)                                                            
{                                                                                                       
    DVBC_PD_CTX_T *psPdCtx = (DVBC_PD_CTX_T *) pvArg;
                                                                                                        
    if (psPdCtx->fgBreakMonitor)
    {                                                                                                   
        mcSHOW_DBG_MSG(("DVBC_Monitor break function triggered!!\n"));                                      
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
    
    DVBC_PD_CTX_T *psPdCtx = *(DVBC_PD_CTX_T **) pvArg;
    BOOL fgNeedReset=FALSE;
    
    mcSHOW_USER_MSG(("MonitorProcess start!\n"));
        
    while (psPdCtx->fgMonitorThreadRunning)
    {
    	if (psPdCtx->fgBreakMonitor)
    	{
    		// After mcMUTEX_LOCK(t_escape_mon_suspend), monitor thread will be suspended until
    		// TunerAcq or TunerClose invoked.
            mcSHOW_DBG_MSG(("Entering DVBC mon_suspend\n"));
            x_sema_lock(psPdCtx->t_escape_mon_suspend, X_SEMA_OPTION_WAIT);
    		//mcMUTEX_LOCK(psPdCtx->t_escape_mon_suspend);
    	  	mcSHOW_DBG_MSG(("Escape DVBC mon_suspend\n"));
    	}
    	else
    	{	
		    // mutex lock for I2C access
            mcSEMA_LOCK_MTFE(psPdCtx->hHalLock);
		    psPdCtx->fgPIMoniStatus = TRUE; //doing PI monitor
            
			VERIFY_I2C(ATBMIsDemodOk(psDemodCtx->u1I2cAddr,&fgNeedReset));
			if(fgNeedReset)
			{
			mcSHOW_USER_MSG(("Demod status abnormal,need reset\n"));
			ATBMDoHWReset();
	        }
			
            psPdCtx->fgPIMoniStatus = FALSE; //finish doing PI monitor or breaked
            mcSEMA_UNLOCK_MTFE(psPdCtx->hHalLock);                
	        x_thread_delay((UINT32)(500)); 
        }
    }
    
    psPdCtx->t_monitor_thread = NULL;
    mcSHOW_USER_MSG(("MonitorProcess exit!\n"));
   // mcTHREAD_DESTROY();
   x_thread_exit();
}

/************************************************************************
*  PD_StartMonitorEngine
*  Function which starts monitor thread engine.
*  @param  *psAtvPdCtx : A pointer to ATV_PD_CTX_T structure.
*  @retval  DRVAPI_TUNER_OK : Monitor thread create successfully.
*  @retval  DRVAPI_TUNER_ERROR : Monitor thread create fail.
************************************************************************/
STATIC INT32 PD_StartMonitorEngine(DVBC_PD_CTX_T *psPdCtx)
{
    psPdCtx->fgMonitorThreadRunning = TRUE;
 //   if (mcTHREAD_CREATE(&psPdCtx->t_monitor_thread,
   //         "EVBC_MonThread",
     //       cTHREAD_STACK_SIZE, cTHREAD_PRIORITY,
       //     MonitorProcess, sizeof(void*), (VOID *) &psPdCtx) != OSR_OK)
   if(x_thread_create(&psPdCtx->t_monitor_thread,
   	          "DVBC_MonitorThread",
   	          cTHREAD_STACK_SIZE,cTHREAD_PRIORITY,
   	          MonitorProcess,sizeof(void*), (VOID *) &psPdCtx)!= OSR_OK)
    {
        mcSHOW_HW_MSG(("PD_StartMonitorEngine (mcTHREAD_CREATE): DRVAPI_TUNER_ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    mcSHOW_USER_MSG(("PD_StartMonitorEngine success\n"));
    return (DRVAPI_TUNER_OK);
}

/************************************************************************
*  PD_StopMonitorEngine
*  Function which stops monitor thread engine.
*  @param  void.
*  @retval  DRVAPI_TUNER_OK : Stop monitor thread successfully.
************************************************************************/
STATIC INT32 PD_StopMonitorEngine(void)
{
//    pPdCtx->fgMonitorThreadRunning = FALSE; // must set to FALSE before unlock sema t_escape_mon_suspend
    while (pPdCtx->t_monitor_thread){
     //   mcDELAY_MS(10); // in ms
      x_thread_delay((UINT32)(10));
    }
    mcSHOW_USER_MSG(("DVBC PD_StopMonitorEngine success\n"));
    return (DRVAPI_TUNER_OK);
}
//#endif //DVBC_ENABLE_MONITOR
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
STATIC INT32 DVBC_TunerClose(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
     
    mcSHOW_DBG_MSG((" %s \n",__FUNCTION__));

//#if !defined(CC_MTAL) && !defined(CC_SDAL)    
//#ifdef DVBC_ENABLE_MONITOR
	if(pDVBCunerDeviceCtx->u1MWIntf != cMTAL_IF && pDVBCunerDeviceCtx->u1MWIntf != cSDAL_IF)
	{
	    pPdCtx->fgBreakMonitor = TRUE; //disable and breadk monitor thread
	    pPdCtx->fgMonitorThreadRunning = FALSE; // must set to FALSE before unlock sema t_escape_mon_suspend
	 //   mcMUTEX_UNLOCK(pPdCtx->t_escape_mon_suspend);
	     x_sema_unlock(pPdCtx->t_escape_mon_suspend);
	    PD_StopMonitorEngine();  // stop monitor engine
	 //   mcMUTEX_DESTROY(pPdCtx->t_escape_mon_suspend);  // destory monitor thread
	 	x_sema_delete(pPdCtx->t_escape_mon_suspend);
	}
//#endif

    //add driver implementation code here
    x_sema_delete(pPdCtx->hHalLock);
    x_mem_free(pPdCtx);
    //x_mem_free(psDemodCtx);

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
STATIC BOOL DVBC_TunerAcq(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type, void* pv_conn_info,
                            SIZE_T z_conn_info_len, x_break_fct _BreakFct, void* pvArg)
{
    UINT32  ii;
    BOOL  LockStatus=FALSE ;
	PARAM_SETFREQ_T param;//Tuner PLL paramter
	BOOL fgTimeOut=FALSE;
	BOOL fgChannelScan=FALSE;	
	TUNER_CAB_DIG_TUNE_INFO_T *pt_tuner_info = (TUNER_CAB_DIG_TUNE_INFO_T *) pv_conn_info;
	UINT32 u4Freq_KHz;
	UINT8 u1QAMMode;
	
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    
//#ifdef DVBC_ENABLE_MONITOR
	if(pDVBCunerDeviceCtx->u1MWIntf != cMTAL_IF && pDVBCunerDeviceCtx->u1MWIntf != cSDAL_IF)
	{
    	SetBreakMonitor(pPdCtx);
	}
//#endif
      ITuner_OP(ITunerGetCtx(),itSetAntIn,1,NULL);//Switch to DVBC(Cable)
	  
    mcSHOW_DBG_MSG((" %s \n",__FUNCTION__));
    //Parse parameter from upper layer
	if(pt_tuner_info->e_conn_mode==CONN_MODE_SCAN)
	{ 
       fgChannelScan = TRUE;
    } 
    u4Freq_KHz=pt_tuner_info->ui4_freq/1000;
	u1QAMMode=pt_tuner_info->e_mod;//Please refer u_tuner.h to get definition of TUNER_MODULATION_T
		psDemodCtx->u2SymbolRateKs=pt_tuner_info->ui4_sym_rate/1000;//convert it to ks/s
    mcSHOW_DBG_MSG(("Acq freq=%dKHz,fgChannelScan=%d Mod=%d,SymRateK=%d KS/s\n",u4Freq_KHz,fgChannelScan,u1QAMMode,	psDemodCtx->u2SymbolRateKs)); 
    //middleware's definition, if e_mode=MOD_UNLNOWN, middleware is doing channel scan mode
    //otherwise it is doing change change
    /////END
    pPdCtx->isBreak = _BreakFct;
    
	param.Freq=u4Freq_KHz;
	param.Modulation=MOD_DVBC;
    param.fgAutoSearch=fgChannelScan;
    
	psDemodCtx->u4Freq=param.Freq;
	
	if(ATMB_SetFreq(psDemodCtx->u1I2cAddr,&param))
	{
	  mcSHOW_DBG_MSG(("Complete Tuner PLL Config\n"));
    }
	//Wake Up demod 
	 mcSHOW_DBG_MSG(("ATBM_WakeUp\n"));
	VERIFY_I2C(ATBM_WakeUp(psDemodCtx->u1I2cAddr));
	 

	 mcSHOW_DBG_MSG(("ATBM_DVBC_Init\n"));
	VERIFY_I2C(ATBM_DVBC_Init(psDemodCtx));

	
	//Loop check demod lock status	
  for (ii = 0; ii < 100; ii ++)
  {
     VERIFY_I2C(ATBMGetSync(psDemodCtx->u1I2cAddr,&LockStatus));
	 if(LockStatus){
	  mcSHOW_DBG_MSG(("Front-end lock!, ii=%d\n", ii));
	  break;
	 }
	 mcDELAY_MS(10);
	 
     if (pPdCtx->isBreak(pvArg))
        {
            break;
        }
	if(ii>40)	
    {
	VERIFY_I2C(ATBM_TimeOut(psDemodCtx->u1I2cAddr,&fgTimeOut));
	 if(fgTimeOut)
	 {
	  mcSHOW_DBG_MSG(("Early-break, ii=%d\n", ii));
	  ii=100;
	  break;
	 }
    }	 
  }
      if (ii == 100)
    {
	   //if(LockStatus==1)mcSHOW_DBG_MSG(("Demod timeout\n"));
        mcSHOW_DBG_MSG(("Front-end unlock\n"));
	}	
//#ifdef DVBC_ENABLE_MONITOR
	if(pDVBCunerDeviceCtx->u1MWIntf != cMTAL_IF && pDVBCunerDeviceCtx->u1MWIntf != cSDAL_IF)
	{
	    if ((!_BreakFct(pvArg)) && (!fgChannelScan))
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
    return (LockStatus);    //if lock return 1, unlock return 0
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
STATIC INT16 DVBC_TunerGetSync(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
     BOOL fgLock=TRUE;
	
	mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here

   VERIFY_I2C(ATBMGetSync(psDemodCtx->u1I2cAddr,&fgLock));
  
   mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
   
   
    return fgLock;
}

//-------------------------------------------------------------------------
/**
 *  This API do disconnect tuner
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  void
 */
//-------------------------------------------------------------------------
STATIC VOID  DVBC_TunerDisc(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
	  
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	   mcSHOW_DBG_MSG((" %s \n",__FUNCTION__));
    //add driver implementation code here

	VERIFY_I2C(ATBM_Standby(psDemodCtx->u1I2cAddr));
	
//#ifdef DVBC_ENABLE_MONITOR
	if(pDVBCunerDeviceCtx->u1MWIntf != cMTAL_IF && pDVBCunerDeviceCtx->u1MWIntf != cSDAL_IF)
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
STATIC void DVBC_TunerGetSignal(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type, SIGNAL *_pSignal)
{
    UINT16 u2QAMSize=0;
	
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here

     mcSHOW_DBG_MSG((" %s \n",__FUNCTION__));

    _pSignal->Frequency  = psDemodCtx->u4Freq;
   
    _pSignal->SymbolRate =psDemodCtx->u2SymbolRateKs;
    //_pSignal->QAMSize = 8;
	VERIFY_I2C(ATBM_DVBC_GetQAMSize(psDemodCtx,&u2QAMSize));
	_pSignal->QAMSize =u2QAMSize;
    // not used in DVBC
    
    _pSignal->e_hierarchy_priority = 0;
     _pSignal->e_bandwidth = BW_6_MHz;
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
STATIC UINT8 DVBC_TunerGetSignalLevel(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
     UINT8 u1sigLvl=50;  
     mcSHOW_DBG_MSG((" %s \n",__FUNCTION__));
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here
	 
	VERIFY_I2C(ATBMGetSigLvl(psDemodCtx->u1I2cAddr,MOD_DVBC,&u1sigLvl));
	
    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    //example of return
    return (u1sigLvl);
}

//-------------------------------------------------------------------------
/**
 *  This API get signal level in multiples of (0.1dBm).
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  0 ~ -900           signal level
 */
//-------------------------------------------------------------------------
STATIC INT16 DVBC_TunerGetSignalLeveldBm(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    INT16 i2Ret=0;
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here
    //i2Ret = Vendor PI get signal level in dBm, return vlaue should be range from 0 ~ -900

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return i2Ret;
}

STATIC UINT16 DVBC_TunerGetSignalPER(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT32 u4_ret=100000;
	     mcSHOW_DBG_MSG((" %s \n",__FUNCTION__));
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here	  
    VERIFY_I2C(ATMB_DVBC_GetPER(psDemodCtx,&u4_ret));
    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    return u4_ret;//modify return value
}
/*
STATIC UINT32 DVBC_TunerGetSignalBER(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
   mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    return 0;//u4_ret;//modify return value
}
*/
STATIC UINT16 DVBC_TunerGetSignalSNR(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
	    UINT16 u2_ret=0;
    //UINT8 SNR;
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	mcSHOW_DBG_MSG((" %s \n",__FUNCTION__));
	//add your implementation here	 
    VERIFY_I2C(ATMB_DVBC_GetSNR(psDemodCtx,&u2_ret));
    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return u2_ret;//modify return value
}

STATIC void DVBC_TunerSetMpgFmt(PTD_SPECIFIC_CTX ptTDSpecificCtx, MPEG_FMT_T  *pt_mpeg_fmt)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}

STATIC char *DVBC_TunerGetVer(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return NULL;
}

STATIC INT32 DVBC_TunerNimTest(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return 0;
}
STATIC void DVBC_TunerSetRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx, UCHAR ucRegSet, UCHAR ucRegAddr, UCHAR ucRegValue)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}

STATIC void DVBC_TunerShowRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}

STATIC void DVBC_TunerTestI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}
STATIC void DVBC_TunerCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx, INT32 i4Argc, const CHAR** aszArgv)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}

STATIC void DVBC_TunerDtdCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,INT32 i4Argc, const CHAR** aszArgv)
{
    UCHAR   ucCmdId = 0;
    UINT32 u4Freq;
    PARAM_SETFREQ_T param;
    UINT8 u1Data;
	UINT16 u2Data1;
	UINT32 u4Data;
	INT32 i4Data;
    if (i4Argc < 1)
    {
        mcSHOW_USER_MSG(("\t s:                     show signal/ts/tps status\n"));
        mcSHOW_USER_MSG(("\t f [Freq]:            set freq (kHz)\n"));
        mcSHOW_USER_MSG(("\t rt [RegAddr] :     read   DTMB part  register\n"));
		mcSHOW_USER_MSG(("\t rc [RegAddr] :     read  DVBC part  register\n"));
        mcSHOW_USER_MSG(("\t wt [RegAddr] [Value]:  write DTMB register\n"));    
        mcSHOW_USER_MSG(("\t wc [RegAddr] [Value]:  write DVBC register\n")); 
        mcSHOW_USER_MSG(("\t ss Symbol_Rate(KHz):   Set DVBC symbol rate\n"));  	
        mcSHOW_USER_MSG(("\t c  [Value]:  0 wakeup demod; 1 sleep demod\n"));		
    }	

    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);

   	 mcSHOW_DBG_MSG((" %s \n",__FUNCTION__));

    if (i4Argc > 0)
        ucCmdId = *((CHAR *) aszArgv[0]);
        
    switch (ucCmdId)
    {
    case 's':
	if('s'==((CHAR *) aszArgv[0])[1]){
	 psDemodCtx->u2SymbolRateKs = (UINT16)StrToInt(aszArgv[1]); // freq
	 mcSHOW_USER_MSG(("Set Symbol Rate=%dKHz\n",psDemodCtx->u2SymbolRateKs));
	 VERIFY_I2C(ATBM_DVBC_Init(psDemodCtx));
	 break;
	}
    else{
      mcSHOW_USER_MSG(("=================================\n"));
		VERIFY_I2C(ATBMGetSync(psDemodCtx->u1I2cAddr,&u1Data));
	    mcSHOW_USER_MSG(("Lock Status=%d\n",u1Data));
		
		VERIFY_I2C(ATBMGetSigLvl(psDemodCtx->u1I2cAddr,MOD_DVBC,&u1Data));
		mcSHOW_USER_MSG(("Signal level=0x%d\n",u1Data));
		
		VERIFY_I2C(ATMB_DVBC_GetBER(psDemodCtx,&u4Data));
		mcSHOW_USER_MSG(("BER=%d^e-5\n",u4Data));
		
		VERIFY_I2C(ATMB_DVBC_GetSNR(psDemodCtx,&u2Data1));
		mcSHOW_USER_MSG(("SNR=%d\n",u2Data1));
		
		VERIFY_I2C(ATBM_DVBC_GetQAMSize(psDemodCtx,&u2Data1));
		mcSHOW_USER_MSG(("QAMSize=%d\n",u2Data1));
		
		VERIFY_I2C(ATBM_DVBC_GetCFO(psDemodCtx,&i4Data));
		//mcSHOW_USER_MSG(("CFO=%d KHz\n",i4Data));
		
		VERIFY_I2C(ATMB_DVBC_GetPER(psDemodCtx,&u4Data));
		mcSHOW_USER_MSG(("PER=%d^e-5\n",u4Data));
	    }	
        break;

    case 'f':
        if (i4Argc >=2)
		{
		u4Freq = (UINT32)StrToInt(aszArgv[1]); // freq
        param.Freq=u4Freq;
       param.Modulation=MOD_DVBC;
       param.fgAutoSearch=FALSE;
       
       VERIFY_I2C(ATMB_SetFreq(psDemodCtx->u1I2cAddr,&param));       
       }
	   else
	   {
	   mcSHOW_USER_MSG(("nim.dtd f Freq_In_KHz\n"));
	   }
        break;

    case 'r':
        if (i4Argc < 2)
            break;
         u2Data1= (UINT16)StrToInt(aszArgv[1]);
		 
		 if('t'==((CHAR *) aszArgv[0])[1]){
		 VERIFY_I2C(ATBMRead(psDemodCtx->u1I2cAddr,u2Data1,&u1Data));
		 mcSHOW_USER_MSG(("Read DTMB Module register [0x%04X]=0x%02X\n",u2Data1,u1Data));
		 }
		 else if('c'==((CHAR *) aszArgv[0])[1])
		 {
		 VERIFY_I2C(ATBM_DVBC_Read(psDemodCtx,u2Data1,&u4Data));
		 mcSHOW_USER_MSG(("Read DVBC Module register [0x%04X]=0x%X\n",u2Data1,u4Data));
		 }
         else
		 {
		 mcSHOW_USER_MSG(("enter rt ot rc\n"));
		 }
        break;

    case 'w':
        if (i4Argc < 3)
            break;
        u2Data1 = (UINT16)StrToInt(aszArgv[1]);
		 
		 if('t'==((CHAR *) aszArgv[0])[1]){
		 u1Data= (UINT8)StrToInt(aszArgv[2]);
		VERIFY_I2C(ATBMWrite(psDemodCtx->u1I2cAddr,u2Data1,u1Data));
		 mcSHOW_USER_MSG(("Write DTMB Module register [0x%04X]=0x%02X\n",u2Data1,u1Data));
		 }
		 else if('c'==((CHAR *) aszArgv[0])[1])
		 {
		 u4Data = (UINT32)StrToInt(aszArgv[2]);
		 VERIFY_I2C(ATBM_DVBC_Write(psDemodCtx,u2Data1,u4Data));
		 mcSHOW_USER_MSG(("Write DVBC Module register [0x%04X]=0x%X\n",u2Data1,u4Data));
		 }
         else
		 {
		 mcSHOW_USER_MSG(("enter wt ot wc\n"));
		 }
		
        break;
	
    case 'c':
	   if (i4Argc < 2)
             break;
  	   u2Data1 = (UINT16)StrToInt(aszArgv[1]);
	   if(u2Data1)
	   {
	   VERIFY_I2C(ATBM_Standby(psDemodCtx->u1I2cAddr));
		mcSHOW_USER_MSG(("Sleep demod\n"));
		}
		else
		{
		VERIFY_I2C(ATBM_WakeUp(psDemodCtx->u1I2cAddr));
		mcSHOW_USER_MSG(("Wakeup demod\n"));
		}
		break;   	
		
    default:
        break;
    }    

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}

STATIC void DVBC_TunerDetachI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bDetach)
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
 *  @param   e_get_type         tuner get type, refer x_tuner.h TUNER_GET_TYPE_DVBC
 *  Outputs:
 *  @param   *pzAttribute       Attribute structure to be filled. Refer x_tuner.h structure definition TUNER_DVBC_ATTRIBUTE_T
 *  @param   *pzAttributeLen    length to be filled.
 *  @retval  DRVAPI_TUNER_OK    success.
 *  @retval  DRVAPI_TUNER_ERROR fail.
 */
//-------------------------------------------------------------------------
STATIC INT16 DVBC_TunerGetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_GET_TYPE_T  e_get_type,
                                VOID *pzAttribute, SIZE_T* pzAttributeLen)
{
    TS_FMT_T * pTsfmt=NULL;
    UINT32 u4Ber;
	SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(ITunerGetCtx()->specific_member.eu_ctx);
	 TUNER_DIG_ATTRIBUTE_T *pAttri;
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here
	 //mcSHOW_DBG_MSG((" %s  %d\n",__FUNCTION__,e_get_type));

  switch (e_get_type)
  {
  
   case TUNER_GET_TYPE_BER: //get attribute depend on what attribute middleware need to know. Here just give an example.
       if(ATMB_DVBC_GetBER(psDemodCtx,&u4Ber))
		{
		  *(UINT32*)pzAttribute=u4Ber;
		}
		else
		{
		 *(UINT32*)pzAttribute=100000;
		}
       *pzAttributeLen = sizeof(UINT32);
        break;
   
    case TUNER_GET_TYPE_TS_FMT://GEt TS format
     	 pTsfmt=(TS_FMT_T *)pzAttribute;
     	 if(!ATBMGetTsFMT(psDemodCtx->u1I2cAddr,pTsfmt))
     	 {
     	 		mcSHOW_HW_MSG(("ATBMGetTsFMT fail!\n"));
     	  }
     	 *pzAttributeLen = sizeof(TS_FMT_T); 
     	  break;
      case TUNER_GET_TYPE_DIG_ATTRIBUTE:
        pAttri = (TUNER_DIG_ATTRIBUTE_T *) pzAttribute;
        pAttri->ui4_lower_bound_freq = pEuTunerCtx->m_s4FreqBoundLower;
        pAttri->ui4_upper_bound_freq = pEuTunerCtx->m_s4FreqBoundUpper;
        pAttri->b_auto_qam_capability = TRUE;
        pAttri->b_auto_sr_capability = FALSE;
       
        pAttri->ui4_fine_tune_delta_freq = 500000;
        break;
     default:
        mcSHOW_DBG_MSG4(("unknown: get Attribute %d\r \n",e_get_type));
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
 *  @param   e_set_type         tuner set type, check x_tuner.h set operation TUNER_SET_TYPE_DVBCX
 *  @param   *pvAttribute       set value.
 *  @param   zAttributeLen      length of *pvAttribute.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  DRVAPI_TUNER_OK    success.
 *  @retval  DRVAPI_TUNER_ERROR fail.
 */
//-------------------------------------------------------------------------
STATIC INT32 DVBC_TunerSetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_SET_TYPE_T  e_set_type,
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
        mcSHOW_USER_MSG(("unknown: set Attribute\r\n"));
        break;
    }
#endif

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return DRVAPI_TUNER_OK;
}
#ifdef fcADD_DIAG_INFO
STATIC INT32 DVBC_TunerSetDiagCmd(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CUSTOM_TUNER_SET_TYPE_T e_tuner_set_type, VOID* pv_set_info, SIZE_T z_size)
{
    INT32   retSts = RMR_OK;
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    return retSts;
}
STATIC INT32 DVBC_TunerGetDiagInfo(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CUSTOM_TUNER_GET_TYPE_T e_tuner_get_type, VOID* pv_get_info, SIZE_T* pz_size)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//add your implementation here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return 0;
}
#endif
//-------------------------------------------------------------------------
/**
*  This API  control  the i2c bypass to tuner. When zap from digital to ananlog,we need this api to bypass i2c to tuner. 
* Inputs:
* @param   ptTDSpecificCtx  Pointer to tuner driver context, there is no use for third party tuner.  
* @param    bSwitchOn        TRUE(bypass i2c to tuner on).
                             FALSE(bypass i2c to tuner off) 
* @Retval                    void
*/
//-------------------------------------------------------------------------
STATIC void DVBC_TunerBypassI2C (PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bSwitchOn)

{
    mcSHOW_DBG_MSG(("_%s_\n",__FUNCTION__));
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here
     VERIFY_I2C(ATBM_BypassI2C(psDemodCtx->u1I2cAddr,bSwitchOn));
    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}
//-----------------------------------------------------------------------------
// public functions
//-----------------------------------------------------------------------------
void PD_DVBC_Register_LTDIS_Fct(PTD_CTX ptTDCtx)
{
    TUNER_DEVICE_CTX_T*		pTunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;
	
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerClose 	= DVBC_TunerClose;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerAcq 	= DVBC_TunerAcq;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGerSync 	= DVBC_TunerGetSync;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignal 	= DVBC_TunerGetSignal;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLevel = DVBC_TunerGetSignalLevel;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLeveldBm = DVBC_TunerGetSignalLeveldBm;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalPER = DVBC_TunerGetSignalPER;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalSNR = DVBC_TunerGetSignalSNR;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetMpgFmt 	= DVBC_TunerSetMpgFmt;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetVer 	= DVBC_TunerGetVer;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerNimTest 	= DVBC_TunerNimTest;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetRegSetting = DVBC_TunerSetRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerShowRegSetting = DVBC_TunerShowRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerTestI2C 	= DVBC_TunerTestI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerCommand 	= DVBC_TunerCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDtdCommand 	= DVBC_TunerDtdCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDetachI2C 	= DVBC_TunerDetachI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDisc 	= DVBC_TunerDisc;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetAttribute= DVBC_TunerSetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute= DVBC_TunerGetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerBypassI2C= DVBC_TunerBypassI2C;
#ifdef fcADD_DIAG_INFO
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetDiagCmd	= DVBC_TunerSetDiagCmd;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetDiagInfo = DVBC_TunerGetDiagInfo;
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
INT32 DVBC_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* pptTDSpecificCtx)
{
    //create pdctx used in this file, you also can add any declaration you want in pdctx
    INT32 retsts;		 
   
     mcSHOW_DBG_MSG((" %s \n",__FUNCTION__));	 
	
  	pDVBCunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;
    pPdCtx = (DVBC_PD_CTX_T *) x_mem_calloc(1, sizeof(DVBC_PD_CTX_T));
    if (pPdCtx == NULL)
    {
        mcSHOW_HW_MSG(("DVBC_PD_CTX_T malloc fail\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    *pptTDSpecificCtx = pPdCtx;

    //create demod ctx
    psDemodCtx = (DVBC_DEMOD_CTX_T *) x_mem_calloc(1, sizeof(DVBC_DEMOD_CTX_T));
    
    if (psDemodCtx == NULL)
    {
        mcSHOW_HW_MSG(("DEMOD_CTX_T malloc fail\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    //Demod  need Init 
	
	psDemodCtx->u1I2cAddr=ATBM_BASE;

	
    //Create a semaphore for low level demod PI access protection
    if (x_sema_create(&pPdCtx->hHalLock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_HW_MSG(("%s:  x_sema_create(pPdCtx->hHalLock) Fail!\n",__FUNCTION__));
        return (DRVAPI_TUNER_ERROR);
    }
    x_sema_unlock(pPdCtx->hHalLock);

    //register LTDIS APIs to HAL
    PD_DVBC_Register_LTDIS_Fct(ptTDCtx);
   
   //Tuner  api registeration 
   retsts=ITunerRegistration(((TUNER_DEVICE_CTX_T*)ptTDCtx)->tunerType);
   ITuner_Init(ITunerGetCtx());	
     
     
if(DRVAPI_TUNER_ERROR==retsts){
   return retsts;
}
//#ifdef DVBC_ENABLE_MONITOR
	if(pDVBCunerDeviceCtx->u1MWIntf != cMTAL_IF && pDVBCunerDeviceCtx->u1MWIntf != cSDAL_IF)
	{
	    if (x_sema_create(&pPdCtx->t_escape_mon_suspend, X_SEMA_TYPE_BINARY,X_SEMA_STATE_LOCK) != OSR_OK)
	    {
	        mcSHOW_HW_MSG(("EN_DVBC_TunerOpen (mcSEMA_CREATE): ERROR!\n"));
	        return (DRVAPI_TUNER_ERROR);
	    }

	    pPdCtx->fgBreakMonitor = TRUE;//init
	    pPdCtx->fgPIMoniStatus = FALSE; //init
	    PD_StartMonitorEngine(pPdCtx);  //start monitor engine
	}
//#endif
    return (DRVAPI_TUNER_OK);
}
