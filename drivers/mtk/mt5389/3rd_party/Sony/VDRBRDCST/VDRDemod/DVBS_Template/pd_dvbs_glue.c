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
 * $RCSfile: pd_DVBS_glue.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
/** @file pd_DVBS_glue.c
 *  DVBS glue layer
 */
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "u_tuner.h"
#include "x_os.h"
#include "x_stl_lib.h"//StrToInt function
#include "tuner_if.h"
#include "TDIS_header.h"
#include "pd_dvbs_glue.h"
#include "tunerDebug.h"
#include "i2c_api.h"
#include "tuner_interface_if.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
//Import extern variable
 
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
					   
//-----------------------------------------------------------------------------
// Export definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define mcSEMA_LOCK_MTFE(sema)                                       \
{                                                                    \
    mcDBG_MSG_L4((" [ _Lock_%s-%d ]\r\n", __FUNCTION__, __LINE__)); \
    x_sema_lock(sema, X_SEMA_OPTION_WAIT);                           \
    mcDBG_MSG_L4((" [ _Excape_%s-%d ]\r\n", __FUNCTION__, __LINE__));     \
}
#define mcSEMA_UNLOCK_MTFE(sema)                                     \
{                                                                    \
    mcDBG_MSG_L4((" [ _Unlock_%s-%d ]\r\n", __FUNCTION__, __LINE__));     \
    x_sema_unlock(sema);                                             \
}

//-----------------------------------------------------------------------------
// static variables
//-----------------------------------------------------------------------------
static DVBS_PD_CTX_T *pPdCtx;
static TUNER_DEVICE_CTX_T*    pDVBSTunerDeviceCtx = NULL;
//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// static functions
//-----------------------------------------------------------------------------
//-------------------------------------------------------------------------
/**
 *  This API Notify MW scan status,do sema lock wait MW continue CMD
 *  @param   UINT8 signal       signal status:0 unlock,1 lock
 *  @param   UINT8 polar       polar type
 *  @param   UINT32 freq	locked TP freq
 *  @param   UINT32 sym	locked TP sym
 *  @param   UINT8 fec        Code rate
 */
//-------------------------------------------------------------------------

STATIC INT32 DVBS_AutoScanCallback(UINT8 signal,UINT8 polar,UINT32 freq,UINT32 sym,UINT8 fec)
{
	ptGET_tuner_info->e_fec_inner = (TUNER_FEC_INNER_T)fec;
	ptGET_tuner_info->e_pol = (TUNER_POLARIZATION_T)polar;
  ptGET_tuner_info->ui4_freq = freq;
	ptGET_tuner_info->ui4_sym_rate = sym;
		
    if (signal==1)	
	{

	 Tuner_Notify(DRV_COND_STATUS,TUNER_COND_STATUS_GOT_SIGNAL);	
     mcSHOW_DRVAPI_MSG(("freq = %d,sym = %d,polar = %d,fec = %d,signal = %d \n", freq,sym,polar,fec,signal));
    }
	else if(signal==0)
	{
	 Tuner_Notify(DRV_COND_STATUS,TUNER_COND_STATUS_LOST_SIGNAL);
	 mcSHOW_DRVAPI_MSG(("freq = %d,signal = %d \n",freq,signal));
	}
	else if(signal==2)
	Tuner_Notify(DRV_COND_STATUS,TUNER_COND_STATUS_AUTOSCAN_END);	//at the end of blind scan
	
    //x_sema_lock_timeout(AutoScan_callback,8000);//liuyuan debug use
    mcSEMA_LOCK_MTFE(AutoScan_callback);
	return 0;
}


static BOOL SetBreakMonitor(DVBS_PD_CTX_T *psPdCtx)
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
            mcDBG_MSG_L2(("ANA EU PD wait break PI monitor finish. u1Cnt=%d\r\n",u1Cnt));
          //  mcDELAY_MS(10); 
        x_thread_delay((UINT32)(10));
        }            
    }
    
    return TRUE;
}
static void ResumeMonitor(DVBS_PD_CTX_T *psPdCtx)
{
    psPdCtx->fgBreakMonitor = FALSE;
   mcSEMA_UNLOCK_MTFE(psPdCtx->t_escape_mon_suspend);
}
/************************************************************************
*  MonitorProcess
*  Monitor thread process function.
*  @param  void.
*  @retval  void.
************************************************************************/
static void MonitorProcess(VOID *pvArg)
{
    
    DVBS_PD_CTX_T *psPdCtx = *(DVBS_PD_CTX_T **) pvArg;
    
    mcDBG_MSG_INFO(("MonitorProcess start!\n"));
        
    while (psPdCtx->fgMonitorThreadRunning)
    {

      //  Snr = 0;
    	if (psPdCtx->fgBreakMonitor)
    	{
    		// After mcMUTEX_LOCK(t_escape_mon_suspend), monitor thread will be suspended until
    		// TunerAcq or TunerClose invoked.
            mcDBG_MSG_L2(("Entering DVBS mon_suspend\n"));
            mcSEMA_LOCK_MTFE(psPdCtx->t_escape_mon_suspend);
    		//mcMUTEX_LOCK(psPdCtx->t_escape_mon_suspend);
    	  	mcDBG_MSG_L2(("Escape DVBS mon_suspend\n"));
    	}
    	else
    	{
		    // mutex lock for I2C access
            mcSEMA_LOCK_MTFE(psPdCtx->hHalLock);
		    psPdCtx->fgPIMoniStatus = TRUE; //doing PI monitor
		
            //! [TODO:PI] put your monitor process code here
   
            psPdCtx->fgPIMoniStatus = FALSE; //finish doing PI monitor or breaked
            mcSEMA_UNLOCK_MTFE(psPdCtx->hHalLock); 
            
            if(do_blindscan == 1)
			{
				struct NIM_Auto_Scan as_para;
			    //as_para = 0;
				// x_mem_alloc(as_para);
				if(cli_blindscan==1)
				{
					mcSHOW_DRVAPI_MSG(("cli debug BLIND SCAN\n"));
          //apply you scan here
				}
				else
				{
				mcSHOW_DRVAPI_MSG(("e_conn_mode:CONN_MODE_SCAN\n"));
            //apply you scan here
				}
			 
	        	mcSHOW_USER_MSG(("End blindscan\n"));
				do_blindscan = 0;
				cli_blindscan = 0;
			}
                           
	        x_thread_delay((UINT32)(500)); 
        }
    }
    
    psPdCtx->t_monitor_thread = NULL;
    mcDBG_MSG_INFO(("MonitorProcess exit!\n"));
   x_thread_exit();
}

/************************************************************************
*  PD_StartMonitorEngine
*  Function which starts monitor thread engine.
*  @param  *psAtvPdCtx : A pointer to ATV_PD_CTX_T structure.
*  @retval  DRVAPI_TUNER_OK : Monitor thread create successfully.
*  @retval  DRVAPI_TUNER_ERROR : Monitor thread create fail.
************************************************************************/
static INT32 PD_StartMonitorEngine(DVBS_PD_CTX_T *psPdCtx)
{
    psPdCtx->fgMonitorThreadRunning = TRUE;
   if(x_thread_create(&psPdCtx->t_monitor_thread,
   	          "DVBS_MonitorThread",
   	          cTHREAD_STACK_SIZE,cTHREAD_PRIORITY,
   	          MonitorProcess,sizeof(void*), (VOID *) &psPdCtx)!= OSR_OK)
    {
        mcDBG_MSG_ERR(("PD_StartMonitorEngine (mcTHREAD_CREATE): DRVAPI_TUNER_ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    mcDBG_MSG_INFO(("PD_StartMonitorEngine success\n"));
    return (DRVAPI_TUNER_OK);
}

/************************************************************************
*  PD_StopMonitorEngine
*  Function which stops monitor thread engine.
*  @param  void.
*  @retval  DRVAPI_TUNER_OK : Stop monitor thread successfully.
************************************************************************/
static INT32 PD_StopMonitorEngine(void)
{
    while (pPdCtx->t_monitor_thread){
      x_thread_delay((UINT32)(10));
    }
    mcDBG_MSG_L1(("DVBS PD_StopMonitorEngine success\n"));
    return (DRVAPI_TUNER_OK);
}

//-------------------------------------------------------------------------
/**
 *  This API do connect tuner
 *  @retval  void
 */
//-------------------------------------------------------------------------
VOID  DVBS_TunerConnect()
{
UINT8	data;


	
	if(pPdCtxDVBS->fgDisStatus == FALSE)
	{
		/* Has connected, return */
		mcDBG_MSG_L2(("DVBS Has connect!\n"));
		return;
	}
    //add driver implementation code here

	//mcSHOW_DBG_MSG(("[DVBS] SetTSPath : pass-through"));
	//Enable DVBS TS output
	//open ts

  //  mcSHOW_DBG_MSG(("[DVBS] Set TS output on  \n"));


	//power down demod
   /* if (nim_close(dev) == 1){
        mcSHOW_USER_MSG(("Failed on DVBS_ChipPowerDown()\n"));
    }*/

	/* set disconnect status */
	pPdCtxDVBS->fgDisStatus = FALSE;
}

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
static INT32 DVBS_TunerClose(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
     
    mcDBG_MSG_L2((" %s \n",__FUNCTION__));

    pPdCtx->fgBreakMonitor = TRUE; //disable and breadk monitor thread
    pPdCtx->fgMonitorThreadRunning = FALSE; // must set to FALSE before unlock sema t_escape_mon_suspend
     mcSEMA_UNLOCK_MTFE(pPdCtx->t_escape_mon_suspend);
    PD_StopMonitorEngine();  // stop monitor engine
    x_sema_delete(pPdCtx->t_escape_mon_suspend); 

    //add driver implementation code here
    x_sema_delete(pPdCtx->hHalLock);
    x_mem_free(pPdCtx);
    //x_mem_free(psDemodCtx);

    //To Do If you have allocated any resource in TunerOpen(), release them here.

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
static BOOL DVBS_TunerAcq(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type, void* pv_conn_info,
                            SIZE_T z_conn_info_len, x_break_fct _BreakFct, void* pvArg)
{
   TUNER_TER_DIG_TUNE_INFO_T *pt_tuner_info = 
                    (TUNER_TER_DIG_TUNE_INFO_T *) pv_conn_info;
	UINT32 u4Freq_KHz;
    BOOL fgChannelScan=FALSE;	
	BOOL fgLock=FALSE;
	
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//Pause monitor thread
	 if(pDVBSTunerDeviceCtx->u1MWIntf != cMTAL_IF && pDVBSTunerDeviceCtx->u1MWIntf != cSDAL_IF)
	 {
	  SetBreakMonitor(pPdCtx);
	 }
	//Parse parameter from upper layer
	
	pPdCtx->isBreak = _BreakFct;
	
freq = pt_tuner_info->ui4_freq;
	lastfreq = freq;

  symbolRate = pt_tuner_info->ui4_sym_rate;

    
    DVBS_TunerConnect();

    //get connection mode
    switch(pt_tuner_info->e_conn_mode)
    {
     case CONN_MODE_SCAN:
	 	 {
		 	do_blindscan = 1;
		 		      }          
		break;
	 case CONN_MODE_UNKNOWN:                       //Channel change
	 case CONN_MODE_SURF:                  //TP scan
              			  
        //apply you channel change code here
			  mcDBG_MSG_L2(("TunerAcq TP SCAN channel freq = %d,symbolrate = %d \n", freq,symbolRate));
		break;
	default:
		 Printf(("Invalid connect mode(%d) to DVBS_TunerAcq!\n", pt_tuner_info->e_conn_mode));
         break;
	}
    

	if(pt_tuner_info->e_conn_mode == CONN_MODE_SCAN)

		LockStatus = 1;

	else
   { //Polling lock status
    for (ii = 0; ii < cAcqTimeOutCnt; ii ++)
    {

  //check lock status here

		
        
        //Front end
        if (LockStatus == 1) {break;}

        mcDELAY_MS(10);
        
        if (pPdCtxDVBS->isBreak(pvArg))
        {
            break;
        }
    }	

    if(ii==cAcqTimeOutCnt)
    {
        mcSHOW_DRVAPI_MSG(("Front-end unlock\n"));
    }
    else if (LockStatus == 1)
    {
        mcSHOW_DRVAPI_MSG(("Front-end lock\n"));

        //Check demod conncection time
       // u4ConnTime = (os_get_sys_tick()-u4TickCnt) * x_os_drv_get_tick_period();
      //  mcSHOW_DBG_MSG(("DVBS demod connection time = %dms\n", u4ConnTime));
        //if ((u4ConnTime > DVBS_DEMOD_CONN_TIME_THREHOLD) && (fgChannelScan)){
      //      mcSHOW_DBG_MSG(("EU Cable Driver ATP Fail: DVBS demod connection time = %dms, too long!!\n", u4ConnTime));
      //  }
    }
	
	
	
	if(pDVBSTunerDeviceCtx->u1MWIntf != cMTAL_IF && pDVBSTunerDeviceCtx->u1MWIntf != cSDAL_IF)
	{
    if (!_BreakFct(pvArg) && (!fgChannelScan))
    {
        ResumeMonitor(pPdCtx);   // Enable monitor thread and Disable break function flag,  Unlock monitor thread
        mcDBG_MSG_L2(("Enable monitor process!!\n"));
    }
	}

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    // TODO return correct lock status
    return (fgLock);    //if lock return 1, unlock return 0
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
static BOOL DVBS_TunerGetSync(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
     BOOL fgLock=FALSE;
	 mcDBG_MSG_L2((" %s \n",__FUNCTION__));
	mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //TODO Get demod lock status from demod driver and update it as variable fgLock
    
	//END
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
static VOID  DVBS_TunerDisc(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
	mcDBG_MSG_L2((" %s \n",__FUNCTION__));  
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //TODO Should force demod enter standby mode(just need i2c active),otherwise,ATV will be interfered
    
    //END
	SetBreakMonitor(pPdCtx);
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
static void DVBS_TunerGetSignal(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type, SIGNAL *_pSignal)
{
     mcDBG_MSG_L2((" %s \n",__FUNCTION__));
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //apply signal code here
   
if (do_blindscan)
    {
        _pSignal->Frequency = ptGET_tuner_info->ui4_freq;
        _pSignal->SymbolRate = ptGET_tuner_info->ui4_sym_rate;
    }
    else
    {
    	 freq = lastfreq;
         nim_get_freq(dev, &freq);
        _pSignal->Frequency  = (S32)freq;
    	
    	nim_get_sym(dev,&sym);
        _pSignal->SymbolRate = (U32)sym;
    }

   
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
static UINT8 DVBS_TunerGetSignalLevel(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT8 u1Ret=0;
	 mcDBG_MSG_L2((" %s \n",__FUNCTION__));
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //TODO Read If_AGC control word and mapping it to signal level(0~100)
	
	//END
    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    //example of return
    return (u1Ret);
}

//-------------------------------------------------------------------------
/**
 *  This API get signal level in multiples of (0.1dBm).
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  0 ~ -900           signal level
 */
//-------------------------------------------------------------------------
static INT16 DVBS_TunerGetSignalLeveldBm(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    INT16 i2Ret=0;
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //Not used now

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return i2Ret;
}

static UINT32 DVBS_TunerGetSignalPER(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT32 u4_ret=100000;//unit is 1e-5
     mcDBG_MSG_L2((" %s \n",__FUNCTION__));
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//To Do Get PER(packet error ratio) from demod driver.note the unit is 1e-5

    //END
    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    return u4_ret;//modify return value
}
static UINT16 DVBS_TunerGetSignalSNR(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
	    UINT16 u2_ret=0;

	mcDBG_MSG_L2((" %s \n",__FUNCTION__));	 
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//TODO Get SNR(signal-noise ratio) from demod driver


    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    return u2_ret;//modify return value
}

static void DVBS_TunerSetMpgFmt(PTD_SPECIFIC_CTX ptTDSpecificCtx, MPEG_FMT_T  *pt_mpeg_fmt)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//Not used

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}

static char *DVBS_TunerGetVer(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//Not used

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return NULL;
}

static INT32 DVBS_TunerNimTest(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//Not used

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return 0;
}
static void DVBS_TunerSetRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx, UCHAR ucRegSet, UCHAR ucRegAddr, UCHAR ucRegValue)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//Not used

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}

static void DVBS_TunerShowRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//Not used

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}

static void DVBS_TunerTestI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//Not used

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}
static void DVBS_TunerCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx, INT32 i4Argc, const CHAR** aszArgv)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
//Not used

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}

static void DVBS_TunerDtdCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,INT32 i4Argc, const CHAR** aszArgv)
{
   //TODO you can add command for debug use here
   
   //END
    UCHAR   ucCmdId = 0;               
    if (i4Argc < 1)
    {
        mcDBG_MSG_INFO(("\t s:                   show signal/ts/tps status\n"));              
    }	
    mcDBG_MSG_L2((" %s \n",__FUNCTION__));
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);

    if (i4Argc > 0)
        ucCmdId = *((CHAR *) aszArgv[0]);
        
    switch (ucCmdId)
    {
    case 's':      
        break;
    default:
    	   default:
  /*      mcSHOW_USER_MSG(("Cmd: \n"));
        mcSHOW_USER_MSG(("\t SetChannel  [Freq (MHz)] [symbrate(kbps)]: set channel\n"));
		mcSHOW_USER_MSG(("\t blindscan  [sFreq (MHz)] [eFreq (MHz)]  : blind scan\n"));
		mcSHOW_USER_MSG(("\t s           : status\n"));
		mcSHOW_USER_MSG(("\t ts [x]\n\t    'p'-parallel TS;\n\t    's'-serial TS;\n\t    'o'-open TS output;\n\t    'c'-close TS output;\n"));
		mcSHOW_USER_MSG(("\t Dint        : demod driver init\n"));
        mcSHOW_USER_MSG(("\t checkreg    : dump demod all regsiter\n"));
        mcSHOW_USER_MSG(("\t diseqc_op [work mode][diseqc cmd][cmd length]:set diseqc 1x cmd\n"));
		mcSHOW_USER_MSG(("\t setH/V [0/1] :0--set Horizontal  1--set Vertical\n"));
		mcSHOW_USER_MSG(("\t scancontinue \n"));//liuyuan debug use
		mcSHOW_USER_MSG(("\t Disc        : End blindscan\n"));
		mcSHOW_USER_MSG(("\t w  [reg_addr][lenth][data]: write demod reg\n"));
		mcSHOW_USER_MSG(("\t r  [reg_addr][lenth]: read demod reg\n"));
		mcSHOW_USER_MSG(("\t iqswap  [0/1]:swap IQ signal\n"));*/

        break;
    }    

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}

static void DVBS_TunerDetachI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bDetach)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //Not used

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}
//----------------------------------------------------------------------------- 
/*
 *  DVBS_GetTsFmt
 *  Get demod TS output format.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   
 */
//-----------------------------------------------------------------------------
TS_FMT_T DVBS_GetTsFmt()
{
UINT8 ucData,ucData2;
TS_FMT_T fgTsFmt;

//follow is an example,pls apply you code
 /*   nim_reg_read(dev,RD8_TS_OUT_SETTING, &ucData, 1);
//Parallel or Serial
	if (ucData & 0x01)   
fgTsFmt.SerOrPar = SP_SERIAL;
  else
	fgTsFmt.SerOrPar = SP_PARALLEL;
        
//clock polarity  ;true means invert
   if (ucData & 0x80)
	fgTsFmt.ClockPolarity = POL_LOW_ACTIVE;     
  else
  fgTsFmt.ClockPolarity = POL_HIGH_ACTIVE;
//GateClock -->continue clk or not?,true means un-continues

        fgTsFmt.fgGatedClock = BOOL_FALSE;//3501 only continue clk  mode

//Tristate --->true means no TS output
    nim_reg_read(dev,RAF_TSOUT_PAD,&ucData,1);
    if (ucData & 0x10)
        fgTsFmt.fgTristate = BOOL_TRUE;
    else
        fgTsFmt.fgTristate = BOOL_FALSE;

//TS clk = ???KHZ,calculate by parallel
   nim_reg_read(dev,0XDF,&ucData,1);
if ((ucData & 0x11) == 0x10)                                  //TS CLK <50M.calculate by ssi mode
	{
	fgTsFmt.u4TsClockInKHz = 6750;        
	}
else  
{ 
   .....
    }
}
*/
//external Demod or not
    fgTsFmt.fgExternalDemod = BOOL_FALSE;

    return fgTsFmt;
}

//-------------------------------------------------------------------------
/**
 *  This API get attribute from tuner driver
 *  Inputs:
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_get_type         tuner get type, refer x_tuner.h TUNER_GET_TYPE_DVBS
 *  Outputs:
 *  @param   *pzAttribute       Attribute structure to be filled. Refer x_tuner.h structure definition TUNER_DVBS_ATTRIBUTE_T
 *  @param   *pzAttributeLen    length to be filled.
 *  @retval  DRVAPI_TUNER_OK    success.
 *  @retval  DRVAPI_TUNER_ERROR fail.
 */
//-------------------------------------------------------------------------
static INT16 DVBS_TunerGetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_GET_TYPE_T  e_get_type,
                                VOID *pzAttribute, SIZE_T* pzAttributeLen)
{   
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here
	 mcDBG_MSG_L2((" %s \n",__FUNCTION__));


		TUNER_SAT_DIG_TUNE_INFO_T *pAttri0;
	TUNER_DIG_ATTRIBUTE_T *pAttri1;
    INT32 i4Temp = 0;
   pAttri0 = (TUNER_SAT_DIG_TUNE_INFO_T*)pzAttribute;



    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);
	 mcDBG_MSG_L2((" %s \n",__FUNCTION__));
    //add driver implementation code here
    switch (e_get_type)
    {
    case TUNER_GET_TYPE_SIGNAL_LEVEL:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_SIGNAL_LEVEL\r\n"));
        *pzAttributeLen = sizeof(INT32);
		i4Temp =  //TODO Get BER from demod driver.1-100
        x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);
        break;
	case TUNER_GET_TYPE_SAT_DIG:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_SAT_DIG\r\n"));
        pAttri0->e_fec_inner = ptGET_tuner_info->e_fec_inner ;
	    pAttri0->e_pol = ptGET_tuner_info->e_pol ;
        pAttri0->ui4_freq = ptGET_tuner_info->ui4_freq ;
	    pAttri0->ui4_sym_rate = ptGET_tuner_info->ui4_sym_rate ;
		break;
	case TUNER_GET_TYPE_BER:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_BER\r\n"));
        *pzAttributeLen = sizeof(INT32);
        i4Temp =  //TODO Get BER from demod driver.Note the unit is 1e-5
        x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);
        break;
    case TUNER_GET_TYPE_DIG_ATTRIBUTE:   	
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_DIG_ATTRIBUTE\r\n"));
        pAttri1 = (TUNER_DIG_ATTRIBUTE_T *) pzAttribute;
        //follow is an example,pls apply you code
    /*    //Tuner frequency boundary
        pAttri1->ui4_lower_bound_freq = 950;//950Hz
        pAttri1->ui4_upper_bound_freq = 2150;//2150M
        // freq offset tolerence range in Hz (1M mapping to 100K)
        pAttri1->ui4_fine_tune_delta_freq = 3;  //3Mhz  */
        break;
	case TUNER_GET_TYPE_TS_FMT:
        {
            TS_FMT_T tsFmt;
            *pzAttributeLen = sizeof(TS_FMT_T);
            tsFmt = DVBS_GetTsFmt();
            x_memcpy(pzAttribute, &tsFmt, *pzAttributeLen);
        }
        break;
     default:
        //mcDBG_MSG_ERR(("unknown: get Attribute %d\r \n",e_get_type));
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
 *  @param   e_set_type         tuner set type, check x_tuner.h set operation TUNER_SET_TYPE_DVBSX
 *  @param   *pvAttribute       set value.
 *  @param   zAttributeLen      length of *pvAttribute.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  DRVAPI_TUNER_OK    success.
 *  @retval  DRVAPI_TUNER_ERROR fail.
 */
//-------------------------------------------------------------------------
static INT32 DVBS_TunerSetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_SET_TYPE_T  e_set_type,
                                        const VOID *pvAttribute, SIZE_T zAttributeLen)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
UCHAR	ucData = *(UCHAR *)pvAttribute;
   
   DISEQC_BUS_CMD_T *diseqc_cmd = (DISEQC_BUS_CMD_T*) pvAttribute;

    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);
    //add driver implementation code here

    switch (e_set_type)
    {
	case TUNER_SET_TYPE_SCAN_CONTINUE:
		x_sema_unlock(AutoScan_callback);
        break;
		
	case TUNER_SET_TYPE_DISEQC_CMD:
		if((diseqc_cmd->ui1_control_mask)==DISEQC_CMD_CTRL_POLARIZATION_MASK)
		{
		//apply you code
			mcDELAY_MS(20);
			mcDBG_MSG_L2(("After set H/V,delay 20ms,delay>15ms\n"));
    	}
		if((diseqc_cmd->ui1_control_mask)==DISEQC_CMD_CTRL_FULL_CMD_MASK)
		{
//apply you code
			mcDELAY_MS(20);
			mcDBG_MSG_L2(("After set cmd,delay 20ms,delay>15ms\n"));
			for(i=0;i<diseqc_cmd->ui1_cmd_len;i++)
			{
				mcDBG_MSG_L2(("send cmd\n x%\t",diseqc_cmd->aui1_cmd[i]));
				mcDBG_MSG_L2(("\n"));
			}
    	}
		if((diseqc_cmd->ui1_control_mask)==DISEQC_CMD_CTRL_TONE_BURST_MASK)
		{   
		    if(diseqc_cmd->b_tone_burst)

//apply you code
			mcDELAY_MS(20);
			mcDBG_MSG_L2(("After set burst,delay 20ms,delay>15ms\n"));

    	}
		if((diseqc_cmd->ui1_control_mask)==DISEQC_CMD_CTRL_22K_MASK)
		{   
		    if(diseqc_cmd->b_22k)
//apply you code
			mcDELAY_MS(20);
			mcDBG_MSG_L2(("After set 22k\n"));
    	}
		break;
		
	case TUNER_SET_TYPE_TS_TRISTATE:
		if (ucData == BOOL_FALSE) 
		{
			mcDBG_MSG_L2(("[DVBS] SetTSPath : pass-through"));
        	//Enable DVBS TS output
			//open ts
//apply you code
			mcDBG_MSG_L2(("[DVBS]	Set TS output on  \n"));
		}
		else if (ucData == BOOL_TRUE) 
		{
			mcDBG_MSG_L2(("[DVBS] SetTSPath : tri-state"));
        	//Set DVBS TS output to high-Z
			// close ts
//apply you code
			mcDBG_MSG_L2(("[DVBS]	Set TS output off  \n"));
		}
	break;
		
	case TUNER_SET_TYPE_TS_SP:
		if (ucData == SP_PARALLEL) 
		{
//apply you code
            mcDBG_MSG_L2(("[DVBS] SetTSInterface : parallel\n"));
        }

		else if (ucData == SP_SERIAL) 
		{
//apply you code
        	mcDBG_MSG_L2(("[DVBS] SetTSInterface : serial\n"));
		}
 
	break;
	
    default:
        mcDBG_MSG_L2(("unknown: set Attribute\r\n"));
        break;
    }

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return DRVAPI_TUNER_OK;
}
#ifdef fcADD_DIAG_INFO
static INT32 DVBS_TunerSetDiagCmd(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CUSTOM_TUNER_SET_TYPE_T e_tuner_set_type, VOID* pv_set_info, SIZE_T z_size)
{
    INT32   retSts = RMR_OK;
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//Not used

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    return retSts;
}
static INT32 DVBS_TunerGetDiagInfo(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CUSTOM_TUNER_GET_TYPE_T e_tuner_get_type, VOID* pv_get_info, SIZE_T* pz_size)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//Not used

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
static void DVBS_TunerBypassI2C (PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bSwitchOn)

{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //TODO  Bypass i2c to tuner connected(bSwitchOn is TRUE) or disconnect(bSwitchOn is FALSE)
	
	//END
    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}
//-----------------------------------------------------------------------------
// public functions
//-----------------------------------------------------------------------------
void PD_DVBS_Register_LTDIS_Fct(PTD_CTX ptTDCtx)
{
    TUNER_DEVICE_CTX_T*		pTunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;
	
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerClose 	= DVBS_TunerClose;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerAcq 	= DVBS_TunerAcq;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGerSync 	= DVBS_TunerGetSync;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignal 	= DVBS_TunerGetSignal;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLevel = DVBS_TunerGetSignalLevel;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLeveldBm = DVBS_TunerGetSignalLeveldBm;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalPER = DVBS_TunerGetSignalPER;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalSNR = DVBS_TunerGetSignalSNR;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetMpgFmt 	= DVBS_TunerSetMpgFmt;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetVer 	= DVBS_TunerGetVer;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerNimTest 	= DVBS_TunerNimTest;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetRegSetting = DVBS_TunerSetRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerShowRegSetting = DVBS_TunerShowRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerTestI2C 	= DVBS_TunerTestI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerCommand 	= DVBS_TunerCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDtdCommand 	= DVBS_TunerDtdCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDetachI2C 	= DVBS_TunerDetachI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDisc 	= DVBS_TunerDisc;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetAttribute= DVBS_TunerSetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute= DVBS_TunerGetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerBypassI2C= DVBS_TunerBypassI2C;
#ifdef fcADD_DIAG_INFO
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetDiagCmd	= DVBS_TunerSetDiagCmd;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetDiagInfo = DVBS_TunerGetDiagInfo;
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
INT32 DVBS_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* pptTDSpecificCtx)
{
    //create pdctx used in this file, you also can add any declaration you want in pdctx		 
   
     mcDBG_MSG_INFO((" %s \n",__FUNCTION__));

    pPdCtx = (DVBS_PD_CTX_T *) x_mem_calloc(1, sizeof(DVBS_PD_CTX_T));
    if (pPdCtx == NULL)
    {
        mcDBG_MSG_ERR(("DVB_PD_CTX_T malloc fail\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    *pptTDSpecificCtx = pPdCtx;
	pDVBSTunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;
	Printf("pDVBSTunerDeviceCtx->tunerType=0x%02X\n",pDVBSTunerDeviceCtx->tunerType);
#if 0
    //create demod ctx
    psDemodCtx = DVBS_DemodCtxCreate();
    if (psDemodCtx == NULL)
    {
        mcDBG_MSG_ERR(("DEMOD_CTX_T malloc fail\n"));
        return (DRVAPI_TUNER_ERROR);
    }
#endif
    //Create a semaphore for low level demod PI access protection
    if (x_sema_create(&pPdCtx->hHalLock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcDBG_MSG_ERR(("%s:  x_sema_create(pPdCtx->hHalLock) Fail!\n",__FUNCTION__));
        return (DRVAPI_TUNER_ERROR);
    }
    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    //register LTDIS APIs to HAL
    PD_DVBS_Register_LTDIS_Fct(ptTDCtx);
    //Regisiter tuner driver
	ITunerRegistration(pDVBSTunerDeviceCtx->tunerType);
	ITuner_Init(ITunerGetCtx());
	
	//TODO Here do demod init.If demod init fail,return DRVAPI_TUNER_ERROR
	
	//END
	 if(pDVBSTunerDeviceCtx->u1MWIntf != cMTAL_IF && pDVBSTunerDeviceCtx->u1MWIntf != cSDAL_IF)
	 {
   if (x_sema_create(&pPdCtx->t_escape_mon_suspend, X_SEMA_TYPE_BINARY,X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcDBG_MSG_ERR(("EN_DVBS_TunerOpen (mcSEMA_CREATE): ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }

    pPdCtx->fgBreakMonitor = TRUE;//init
    pPdCtx->fgPIMoniStatus = FALSE; //init
    PD_StartMonitorEngine(pPdCtx);  //start monitor engine
   }
    return (DRVAPI_TUNER_OK);
}
