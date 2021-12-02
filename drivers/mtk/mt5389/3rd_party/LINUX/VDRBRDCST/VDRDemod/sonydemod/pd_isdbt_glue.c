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
 * $RCSfile: pd_isdbt_glue.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
/** @file pd_isdbt_glue.c
 *  ISDBT glue layer
 */
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "u_tuner.h"
#include "x_os.h"
#include "x_stl_lib.h"//StrToInt function
#include "tuner_if.h"
#include "TDIS_header.h"
#include "pd_isdbt_glue.h"
#include "tunerDebug.h"
#include "i2c_api.h"
#include "tuner_interface_if.h"
#include "drvcust_if.h"//Get Tuner/Demod I2C bus id
#include "x_gpio.h"

// for CXD2828 driver
#include "cxd2828_common.h"
#include "cxd2828_i2c_MTK.h"
#include "cxd2828_cxd2828.h"
#include "cxd2828_isdb_t_demod.h"
#include "cxd2828_isdb_t_demod_monitor.h"
#include "cxd2828_math.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define WAIT_TUNER_AGC_STABLE 100  // for CXD2828 tuner
#define WAIT_TS_LOCK_TIMEOUT 2000
#define WAIT_TS_LOCK_INTERVAL 50
//Import extern variable
 
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#define CXD2828_DEMOD_I2C_ADDRESS  0xD8  // I2C Slave Address in 8bit form
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
static ISDBT_PD_CTX_T *pPdCtx;
static TUNER_DEVICE_CTX_T*  pIsdbtTunerDeviceCtx = NULL;

typedef struct cxd2828_demod_ctx_t
{
    cxd2828_isdb_t_demod_t demod;
    cxd2828_i2c_t i2c;
    BOOL fgEws;  // Previous EWS flag value
} cxd2828_demod_ctx_t;
static cxd2828_demod_ctx_t *pCxd2828DemodCtx;

//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// static functions
//-----------------------------------------------------------------------------
static VOID vResetDemod(UINT16 u2DelayTime)
{
UINT32 u4RstPinNum;

    // Config GPIO to reset demod
    mcDBG_MSG_L2(("vResetDemod\n"));
    if (DRVCUST_OptQuery(eDemodResetGpio, &u4RstPinNum) == 0)
    {
        GPIO_SetOut(u4RstPinNum, 0);
        x_thread_delay((UINT32)u2DelayTime);
        GPIO_SetOut(u4RstPinNum, 1);
        x_thread_delay((UINT32)20); // Delay 20 ms for CR[DTV00030459]. Or it will init fail in rel image
        mcDBG_MSG_L2(("Reset ISDBT Demod GPIO=0x%x, 0->1, %dms\r\n",u4RstPinNum, u2DelayTime));
    }
}
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
            mcDBG_MSG_L2(("ANA EU PD wait break PI monitor finish. u1Cnt=%d\r\n",u1Cnt));
          //  mcDELAY_MS(10); 
        x_thread_delay((UINT32)(10));
        }             
    }
    
    return TRUE;
}
static void ResumeMonitor(ISDBT_PD_CTX_T *psPdCtx)
{
    psPdCtx->fgBreakMonitor = FALSE;
   mcSEMA_UNLOCK_MTFE(psPdCtx->t_escape_mon_suspend);
}

// cxd2828 specific

static UINT32 cxd2828_CalcBER(UINT32 bitError, UINT32 packetCount)
{
    /*
     * BER = bitError / period ;
     *
     *    Assume:
     *    Q = quotient = period / bitError ;
     *    R = remainder = periond % bitError ;
     *
     *    So:
     *    period = Q.bitError + R
     *    BER = bitError / (Q.bitError + R)
     *        = 1 / (Q + R/bitError)
     */
    if((bitError == 0) || (packetCount == 0)){
        return 0;
    }
    return (100000U << 4) / (((packetCount*204*8/bitError) << 4) + 
        ((((packetCount*204*8%bitError) << 4) + bitError/2) / bitError));
}

static UINT32 cxd2828_CalcPER(UINT32 packetError, UINT32 packetCount)
{
    /*
     *    PER = packetError / period ;
     *
     *    Assume:
     *    Q = quotient = period / packetError ;
     *    R = remainder = periond % packetError ;
     *
     *    So:
     *    period = Q.packetError + R
     *    PER = packetError / (Q.packetError + R)
     *        = 1 / (Q + R/packetError)
     */    
    if((packetError == 0) || (packetCount == 0)){
        return 0;
    }
    return (100000U << 15) / (((packetCount/packetError) << 15) + 
        ((((packetCount%packetError) << 15) + packetError/2) / packetError));
}

static UINT32 cxd2828_CalcMER(UINT32 merdt)
{
    /* MER[dB] = 10 * log( IREG_MER_DT / 16 ) - 2.13 */
    return 100 * cxd2828_math_log10(merdt) - 14171; /* MER in dB * 1000 */
}

static BOOL cxd2828_GetTsFmt(cxd2828_isdb_t_demod_t *pDemod, TS_FMT_T *pTsfmt)
{
    cxd2828_result_t cxd2828_result = CXD2828_RESULT_OK;
    UINT8 data = 0;

    // Set Bank 0
    cxd2828_result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(cxd2828_result != CXD2828_RESULT_OK){ return 0; }

    cxd2828_result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0xCD, &data, 1);
    if(cxd2828_result != CXD2828_RESULT_OK){ return 0; }
    pTsfmt->SerOrPar = (data & 0x02) ? SP_PARALLEL : SP_SERIAL;

    cxd2828_result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0xA0, &data, 1);
    if(cxd2828_result != CXD2828_RESULT_OK){ return 0; }
    pTsfmt->ClockPolarity = (data & 0x02) ? POL_HIGH_ACTIVE : POL_LOW_ACTIVE;

    cxd2828_result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0x1E, &data, 1);
    if(cxd2828_result != CXD2828_RESULT_OK){ return 0; }
    pTsfmt->fgTristate = (data == 0xFF) ? BOOL_FALSE : BOOL_TRUE ;

    pTsfmt->u4TsClockInKHz = 41000; // 41MHz fixed value for now

    cxd2828_result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0x19, &data, 1);
    if(cxd2828_result != CXD2828_RESULT_OK){ return 0; }
    pTsfmt->fgGatedClock = (data & 0x10) ? BOOL_TRUE : BOOL_FALSE;

    pTsfmt->fgExternalDemod = BOOL_TRUE;

    return 1;
}

static UINT32 cxd2828_strtoul(const char* str)
{
    const char* ptr = str;
    UINT32 value = 0;

    if(!str){
        return 0;
    }
    if((x_strlen(str) > 2) && (str[0] == '0') && ((str[1] == 'x') || (str[1] == 'X'))){
        // Hex
        for(ptr = &(str[2]); *ptr; ptr++){
            if(('0' <= *ptr) && (*ptr <= '9')){
                value <<= 4; value += (UINT32)(*ptr - '0');
            }else if(('a' <= *ptr) && (*ptr <= 'f')){
                value <<= 4; value += (UINT32)(*ptr - 'a' + 10); 
            }else if(('A' <= *ptr) && (*ptr <= 'F')){
                value <<= 4; value += (UINT32)(*ptr - 'A' + 10);
            }else{
                break;
            }
        }
    }else{
        // Decimal
        for(ptr = str; *ptr; ptr++){
            if(('0' <= *ptr) && (*ptr <= '9')){
                value *= 10; value += (UINT32)(*ptr - '0');
            }else{
                break;
            }
        }
    }
    
    return value;
}

/************************************************************************
*  MonitorProcess
*  Monitor thread process function.
*  @param  void.
*  @retval  void.
************************************************************************/
static void MonitorProcess(VOID *pvArg)
{
    
    ISDBT_PD_CTX_T *psPdCtx = *(ISDBT_PD_CTX_T **) pvArg;
    
    mcDBG_MSG_INFO(("MonitorProcess start!\n"));
        
    while (psPdCtx->fgMonitorThreadRunning)
    {

      //  Snr = 0;
    	if (psPdCtx->fgBreakMonitor)
    	{
    		// After mcMUTEX_LOCK(t_escape_mon_suspend), monitor thread will be suspended until
    		// TunerAcq or TunerClose invoked.
            mcDBG_MSG_L2(("Entering ISDBT mon_suspend\n"));
            mcSEMA_LOCK_MTFE(psPdCtx->t_escape_mon_suspend);
    		//mcMUTEX_LOCK(psPdCtx->t_escape_mon_suspend);
    	  	mcDBG_MSG_L2(("Escape ISDBT mon_suspend\n"));
    	}
    	else
    	{
		    // mutex lock for I2C access
            mcSEMA_LOCK_MTFE(psPdCtx->hHalLock);
		    psPdCtx->fgPIMoniStatus = TRUE; //doing PI monitor
		
            //! [TODO:PI] put your monitor process code here
   
            psPdCtx->fgPIMoniStatus = FALSE; //finish doing PI monitor or breaked
            mcSEMA_UNLOCK_MTFE(psPdCtx->hHalLock);                
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
static INT32 PD_StartMonitorEngine(ISDBT_PD_CTX_T *psPdCtx)
{
    psPdCtx->fgMonitorThreadRunning = TRUE;
   if(x_thread_create(&psPdCtx->t_monitor_thread,
   	          "ISDBT_MonitorThread",
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
    mcDBG_MSG_L1(("ISDBT PD_StopMonitorEngine success\n"));
    return (DRVAPI_TUNER_OK);
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
static INT32 ISDBT_TunerClose(PTD_SPECIFIC_CTX ptTDSpecificCtx)
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
    x_mem_free(pCxd2828DemodCtx);

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
    TUNER_TER_DIG_TUNE_INFO_T *pt_tuner_info = 
                    (TUNER_TER_DIG_TUNE_INFO_T *) pv_conn_info;
    UINT32 u4Freq_KHz;
    BOOL fgChannelScan=FALSE;	
    BOOL fgLock=FALSE;
    UINT32 cnt=0;
    // Cxd2828 Specific
    cxd2828_result_t cxd2828_result=CXD2828_RESULT_OK;
    cxd2828_isdb_t_demod_t *pDemod = &pCxd2828DemodCtx->demod;
    cxd2828_isdb_t_demod_lock_status_t lockStatus;
	
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//Pause monitor thread
	 if(pIsdbtTunerDeviceCtx->u1MWIntf != cMTAL_IF && pIsdbtTunerDeviceCtx->u1MWIntf != cSDAL_IF)
	 {
	  SetBreakMonitor(pPdCtx);
	 }
	//Parse parameter from upper layer
	
	pPdCtx->isBreak = _BreakFct;
	
	if(pt_tuner_info->e_mod == MOD_UNKNOWN)
	{ 
           fgChannelScan = TRUE;
        }
	u4Freq_KHz=pt_tuner_info->ui4_freq/1000;
	pPdCtx->Frequency = u4Freq_KHz;
	
	mcDBG_MSG_L1(("Acq freq=%dKHz,fgChannelScan=%d\n",u4Freq_KHz,fgChannelScan));
	
	{
		PARAM_SETFREQ_T param;
		param.Freq = u4Freq_KHz;
		param.Modulation = MOD_ISDBT;
		//Set Tuner PLL
		if(ITuner_SetFreq(ITunerGetCtx(), &param))
		{
	    		mcDBG_MSG_ERR(("Set Freq fail\n"));
		}
	}

       /* Wait for tuner(RB211) AGC stable */
	for(cnt = 0; cnt < WAIT_TUNER_AGC_STABLE/WAIT_TS_LOCK_INTERVAL; cnt++){
		if(_BreakFct(pvArg)){
			/* Break */
			goto ErrorExit;
		}
		
		x_thread_delay((UINT32)WAIT_TS_LOCK_INTERVAL); /* wait 50ms */
	}

       mcDBG_MSG_L1(("Start to set up Cxd2828 Demod.\n"));
	//Demod Tune
	cxd2828_result = cxd2828_isdb_t_demod_Tune(pDemod); 
	if(cxd2828_result!=CXD2828_RESULT_OK){
		goto ErrorExit;
	}
	
	// Wait fordemod LOCK status
	for(cnt=0; cnt < WAIT_TS_LOCK_TIMEOUT/WAIT_TS_LOCK_INTERVAL; cnt++)
	{
		if(_BreakFct(pvArg)){
			goto ErrorExit;
		}

		cxd2828_result=cxd2828_isdb_t_demod_CheckTSLock(pDemod, &lockStatus);
		if(cxd2828_result!=CXD2828_RESULT_OK){
			goto ErrorExit;
		}

		switch(lockStatus){
		case CXD2828_ISDB_T_DEMOD_LOCK_STATUS_LOCK:
			mcDBG_MSG_L1(("Cxd2828 Demod is Locked.\n"));
			fgLock = 1; //Lock
			goto SuccessExit;
		case CXD2828_ISDB_T_DEMOD_LOCK_STATUS_UNLOCK:
			mcDBG_MSG_L1(("Cxd2828 Demod is Unlocked.\n"));
			fgLock = 0; //Unlock
			goto SuccessExit;
		case CXD2828_ISDB_T_DEMOD_LOCK_STATUS_NOT_DETECT:
			/* Not Detected */
			break;
		default:
			/* Error */
			mcDBG_MSG_L1(("Invalid Lock Status.\n"));
			fgLock =0;
			goto ErrorExit;
		}

		x_thread_delay((UINT32)WAIT_TS_LOCK_INTERVAL); /* wait 50ms */
	}

ErrorExit:
SuccessExit:
	if(pIsdbtTunerDeviceCtx->u1MWIntf != cMTAL_IF && pIsdbtTunerDeviceCtx->u1MWIntf != cSDAL_IF)
	{
	    if (!_BreakFct(pvArg) && (!fgChannelScan))
	    {
	        ResumeMonitor(pPdCtx);   // Enable monitor thread and Disable break function flag,  Unlock monitor thread
	        mcDBG_MSG_L1(("Enable monitor process!!\n"));
	    }
	}

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    // TODO return correct lock status
    return (fgLock);
    //if lock return 1, unlock return 0
}
//-------------------------------------------------------------------------
/**
 *  This API check lock status
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  1                  LOCK
 *  @retval  0                UNLOCK
 */
//-------------------------------------------------------------------------
static BOOL ISDBT_TunerGetSync(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
     BOOL fgLock=FALSE;
	 mcDBG_MSG_L4((" %s \n",__FUNCTION__));
	mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //TODO Get demod lock status from demod driver and update it as variable fgLock

	cxd2828_result_t cxd2828_result = CXD2828_RESULT_OK;
	cxd2828_isdb_t_demod_t *pDemod = &pCxd2828DemodCtx->demod;
	UINT8 dmdLock =0;	
       UINT8 tsLock =0;
	UINT8 unlock =0;

	cxd2828_result = cxd2828_isdb_t_demod_MonitorSyncStat(pDemod, &dmdLock, &tsLock, &unlock);
	
	if(cxd2828_result == CXD2828_RESULT_OK){
		/* Judge Lock or Unlock */
		if(tsLock == 0){
			/*Unlock */
			fgLock=0;
		}else{
			/* Lock */
			fgLock=1;
		}
	}else{
		fgLock = 0;
	}

    //END

	mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    return fgLock;
}


//-------------------------------------------------------------------------
/**
 *  This API do disconnect tuner
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval    void
 */
//-------------------------------------------------------------------------
static VOID  ISDBT_TunerDisc(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    //cxd2828_result_t cxd2828_result = CXD2828_RESULT_OK;
    cxd2828_isdb_t_demod_t *pDemod = &pCxd2828DemodCtx->demod;

    mcDBG_MSG_L2((" %s \n",__FUNCTION__));  
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //TODO Should force demod enter standby mode(just need i2c active),otherwise,ATV will be interfered

    //cxd2828_result = cxd2828_isdb_t_demod_Sleep(pDemod);
    cxd2828_isdb_t_demod_Sleep(pDemod);

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
 *  @retval    void
 */
//-------------------------------------------------------------------------
static void ISDBT_TunerGetSignal(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type, SIGNAL *_pSignal)
{
     cxd2828_result_t cxd2828_result = CXD2828_RESULT_OK;
     cxd2828_isdb_t_demod_t *pDemod = &pCxd2828DemodCtx->demod;
     cxd2828_isdb_t_tmcc_info_t tmccInfo;
     //cxd2828_isdb_t_demod_monitor_target_t target_layer;
     cxd2828_isdb_t_tmcc_modulation_t mod;

     mcDBG_MSG_L1((" %s \n",__FUNCTION__));
     mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);

     //add driver implementation code here
     _pSignal->Frequency  =  pPdCtx->Frequency;
     _pSignal->e_bandwidth = BW_6_MHz;
	 
     //TODO Get QAMSize from demod driver
     cxd2828_result = cxd2828_isdb_t_demod_MonitorTMCCInfo(pDemod, &tmccInfo);
     if(cxd2828_result != CXD2828_RESULT_OK){
	 /* Error */
	_pSignal->QAMSize =0;
     }

     switch((TUNER_ISDBT_LAYER_T)ptTDSpecificCtx){
     case ISDBT_LAYER_A:
	 	//target_layer = CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_A;
		mod = tmccInfo.xcurrent.layerA.modulation;
		break;
     case ISDBT_LAYER_B:
	 	//target_layer = CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_B;
		mod = tmccInfo.xcurrent.layerB.modulation;
		break;
     case ISDBT_LAYER_C:
	 	//target_layer = CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_C;
		mod = tmccInfo.xcurrent.layerC.modulation;
		break;
     default:
	 	/* Error */
		break;
     }
	  
    switch(mod){
    case CXD2828_ISDB_T_TMCC_MODULATION_DQPSK:
     	 _pSignal->QAMSize = 24; // MOD_QAM_DQPSK
	 break;
     case CXD2828_ISDB_T_TMCC_MODULATION_QPSK:
	  _pSignal->QAMSize = 23; // MOD_QAM_QPSK
	 break;
     case CXD2828_ISDB_T_TMCC_MODULATION_16QAM:
	  _pSignal->QAMSize = 4; // MOD_QAM_16
	 break;
     case CXD2828_ISDB_T_TMCC_MODULATION_64QAM:
	  _pSignal->QAMSize = 6; // MOD_QAM_64
	 break;
     default:
	  _pSignal->QAMSize = 0; // MOD_UNKNOWN
	 break;
     }
	//END
	
    // not used in ISDBT
    _pSignal->SymbolRate = 0;
    _pSignal->e_hierarchy_priority = 0;

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

}

//-------------------------------------------------------------------------
/**
 *  This API get signal level in t
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  0~100              signal level
 */
//-------------------------------------------------------------------------
static UINT8 ISDBT_TunerGetSignalLevel(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT8 u1Ret=0;
    cxd2828_result_t cxd2828_result = CXD2828_RESULT_OK;
    cxd2828_isdb_t_demod_t *pDemod = &pCxd2828DemodCtx->demod;
    UINT32 ifagcOut = 0;
    mcDBG_MSG_L4((" %s \n",__FUNCTION__));
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //TODO Read If_AGC control word and mapping it to signal level(0~100)

    cxd2828_result = cxd2828_isdb_t_demod_MonitorIFAGCOut(pDemod, &ifagcOut);
    if(cxd2828_result == CXD2828_RESULT_OK){
	/* ifagcOut range 0 - 4095(0x0FFF) */
	u1Ret = (UINT8)(ifagcOut * 100 / 4095);
    }
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
static INT16 ISDBT_TunerGetSignalLeveldBm(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    INT16 i2Ret=0;
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //Not used now

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return i2Ret;
}

static UINT32 ISDBT_TunerGetSignalPER(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT32 u4_ret=100000;//unit is 1e-5
    cxd2828_result_t cxd2828_result=CXD2828_RESULT_OK;
    cxd2828_isdb_t_demod_t *pDemod = &pCxd2828DemodCtx->demod;
    UINT32 packetError = 0;
    UINT32 packetCount = 0;
    UINT32 packetErrorTotal = 0;
    UINT32 packetCountTotal = 0;

     mcDBG_MSG_L1((" %s \n",__FUNCTION__));
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//To Do Get PER(packet error ratio) from demod driver.note the unit is 1e-5

    /* Monitor for Layer A */
    cxd2828_result = cxd2828_isdb_t_demod_MonitorPostRSPacketError(pDemod, CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_A, &packetError, &packetCount);
    if(cxd2828_result == CXD2828_RESULT_OK){
	packetErrorTotal += packetError;
	packetCountTotal += packetCount;
    }

    /* Monitor for Layer B */
    cxd2828_result = cxd2828_isdb_t_demod_MonitorPostRSPacketError(pDemod, CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_B, &packetError, &packetCount);
    if(cxd2828_result == CXD2828_RESULT_OK){
	packetErrorTotal += packetError;
	packetCountTotal += packetCount;
    }

    /* Monitor for Layer C */
    cxd2828_result = cxd2828_isdb_t_demod_MonitorPostRSPacketError(pDemod, CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_C, &packetError, &packetCount);
    if(cxd2828_result == CXD2828_RESULT_OK){
	packetErrorTotal += packetError;
	packetCountTotal += packetCount;
    }

    /* Calculate */
    u4_ret = cxd2828_CalcPER(packetErrorTotal, packetCountTotal);
	
    //END
    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    return u4_ret;//modify return value
}

static UINT32 ISDBT_TunerGetSignalPER_Layer(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    UINT32 u4_ret=100000;//unit is 1e-5
    cxd2828_result_t cxd2828_result=CXD2828_RESULT_OK;
    cxd2828_isdb_t_demod_t *pDemod = &pCxd2828DemodCtx->demod;
    UINT32 packetError = 0;
    UINT32 packetCount = 0;
    cxd2828_isdb_t_demod_monitor_target_t  layer_type;

    mcDBG_MSG_L1((" %s \n",__FUNCTION__));

    switch((TUNER_ISDBT_LAYER_T)ptTDSpecificCtx){
    case ISDBT_LAYER_A:
	 	layer_type= CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_A;
		break;
     case ISDBT_LAYER_B:
	 	layer_type= CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_B;
		break;
     case ISDBT_LAYER_C:
	 	layer_type= CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_C;
		break;
     default:
	 	/* Error */
		break;
     }

    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//To Do Get PER(packet error ratio) from demod driver.note the unit is 1e-5

    /* Monitor for  selected Layer */
    cxd2828_result = cxd2828_isdb_t_demod_MonitorPostRSPacketError(pDemod, layer_type, &packetError, &packetCount);
    if(cxd2828_result == CXD2828_RESULT_OK){
        /* Calculate */
        u4_ret = cxd2828_CalcPER(packetError, packetCount);
    }
	
    //END
    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    return u4_ret;//modify return value
}

static UINT32 ISDBT_TunerGetSignalBER(PTD_SPECIFIC_CTX ptTDSpecificCtx,  DRV_CONN_TYPE_T e_conn_type)
{
    UINT32 u4_ret=100000;//unit is 1e-5
    cxd2828_result_t cxd2828_result=CXD2828_RESULT_OK;
    cxd2828_isdb_t_demod_t *pDemod = &pCxd2828DemodCtx->demod;
    UINT32 bitError = 0;
    UINT32 packetCount = 0;
    UINT32 bitErrorTotal = 0;
    UINT32 packetCountTotal = 0;

     mcDBG_MSG_L1((" %s \n",__FUNCTION__));
   // mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//To Do Get PER(packet error ratio) from demod driver.note the unit is 1e-5

    /* Monitor for Layer A */
    cxd2828_result = cxd2828_isdb_t_demod_MonitorPostViterbiBitError(pDemod, CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_A, &bitError, &packetCount);
    if(cxd2828_result == CXD2828_RESULT_OK){
	bitErrorTotal += bitError;
	packetCountTotal += packetCount;
    }

    /* Monitor for Layer B */
    cxd2828_result = cxd2828_isdb_t_demod_MonitorPostViterbiBitError(pDemod, CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_B, &bitError, &packetCount);
    if(cxd2828_result == CXD2828_RESULT_OK){
	bitErrorTotal += bitError;
	packetCountTotal += packetCount;
    }

    /* Monitor for Layer C */
    cxd2828_result = cxd2828_isdb_t_demod_MonitorPostViterbiBitError(pDemod, CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_C, &bitError, &packetCount);
    if(cxd2828_result == CXD2828_RESULT_OK){
	bitErrorTotal += bitError;
	packetCountTotal += packetCount;
    }

    /* Calculate */
    u4_ret = cxd2828_CalcBER(bitErrorTotal, packetCountTotal);

	
    //END
   // mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    return u4_ret;//modify return value
}

static UINT32 ISDBT_TunerGetSignalBER_Layer(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    UINT32 u4_ret=100000;//unit is 1e-5
    cxd2828_result_t cxd2828_result=CXD2828_RESULT_OK;
    cxd2828_isdb_t_demod_t *pDemod = &pCxd2828DemodCtx->demod;
    UINT32 bitError = 0;
    UINT32 packetCount = 0;
    cxd2828_isdb_t_demod_monitor_target_t  layer_type;
	
    mcDBG_MSG_L2((" %s \n",__FUNCTION__));
	
    switch((TUNER_ISDBT_LAYER_T)ptTDSpecificCtx){
    case ISDBT_LAYER_A:
	 	layer_type= CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_A;
		break;
     case ISDBT_LAYER_B:
	 	layer_type= CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_B;
		break;
     case ISDBT_LAYER_C:
	 	layer_type= CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_C;
		break;
     default:
	 	/* Error */
		break;
     }
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//To Do Get PER(packet error ratio) from demod driver.note the unit is 1e-5

    /* Monitor for  selected Layer */
    cxd2828_result = cxd2828_isdb_t_demod_MonitorPostViterbiBitError(pDemod, layer_type, &bitError, &packetCount);
    if(cxd2828_result == CXD2828_RESULT_OK){
        /* Calculate */
        u4_ret = cxd2828_CalcBER(bitError, packetCount);
    }
	
    //END
    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    return u4_ret;//modify return value
}

static UINT16 ISDBT_TunerGetSignalSNR(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT16 u2_ret=0;
    cxd2828_result_t cxd2828_result = CXD2828_RESULT_OK;
    cxd2828_isdb_t_demod_t *pDemod = &pCxd2828DemodCtx->demod;
    UINT32 merdt = 0;

    mcDBG_MSG_L2((" %s \n",__FUNCTION__));	 
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //TODO Get SNR(signal-noise ratio) from demod driver

    cxd2828_result = cxd2828_isdb_t_demod_MonitorMERDT(pDemod, &merdt);

    /* Value (dB x 1000) */
    u2_ret = (UINT16)(cxd2828_CalcMER(merdt));

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    return u2_ret;//modify return value
}

static void ISDBT_TunerSetMpgFmt(PTD_SPECIFIC_CTX ptTDSpecificCtx, MPEG_FMT_T  *pt_mpeg_fmt)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//Not used

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}

static char *ISDBT_TunerGetVer(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//Not used

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return NULL;
}

static INT32 ISDBT_TunerNimTest(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//Not used

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return 0;
}
static void ISDBT_TunerSetRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx, UCHAR ucRegSet, UCHAR ucRegAddr, UCHAR ucRegValue)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//Not used

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}

static void ISDBT_TunerShowRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//Not used

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}

static void ISDBT_TunerTestI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//Not used

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}
static void ISDBT_TunerCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx, INT32 i4Argc, const CHAR** aszArgv)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
//Not used

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}

static void ISDBT_TunerDtdCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,INT32 i4Argc, const CHAR** aszArgv)
{
    cxd2828_result_t cxd2828_result=CXD2828_RESULT_OK;
    cxd2828_isdb_t_demod_t *pDemod = &pCxd2828DemodCtx->demod;

   //TODO you can add command for debug use here
   
   //END
    UCHAR   ucCmdId = 0;               
    if (i4Argc < 1)
    {
        mcDBG_MSG_L1(("\t s:                   show signal/ts/tps status\n"));          
        mcDBG_MSG_L1(("\t d [DevAddr] :    Register Page Dump Result\n")); 
        mcDBG_MSG_L1(("\t r [RegAddr] [Num]:  read  Demod register\n"));
        mcDBG_MSG_L1(("\t w [RegAddr] [Value]: write Demod register\n"));
        mcDBG_MSG_L1(("\t f [Freq]:            set freq (kHz)\n"));
        mcDBG_MSG_L1(("\t t  tuner status\n")); 
    }	
    mcDBG_MSG_L2((" %s \n",__FUNCTION__));

    if (i4Argc > 0)
        ucCmdId = *((CHAR *) aszArgv[0]);
        
    switch (ucCmdId)
    {
    case 's':
        mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
        /* Demod Lock Status */
        {			   				
			
            UINT8 dmdLock = 0;
            UINT8 tsLock = 0;
            UINT8 unlock = 0;
            cxd2828_result = cxd2828_isdb_t_demod_MonitorSyncStat(pDemod, &dmdLock, &tsLock, &unlock);
            if(cxd2828_result == CXD2828_RESULT_OK){
            	mcDBG_MSG_L1(("ISDBT_TunerGetSignal  Freq=%dHz\n",pPdCtx->Frequency));
                mcDBG_MSG_L1(("Demod Lock = %d\n", dmdLock));
                mcDBG_MSG_L1(("TS Lock    = %d\n", tsLock));
                mcDBG_MSG_L1(("Unlock Det = %d\n", unlock));
            }else{
                mcDBG_MSG_L1(("Error in MonitorSyncStat\n"));
            }
        }
        /* Signal level */
        {
            UINT32 ifAgc = 0;
            cxd2828_result = cxd2828_isdb_t_demod_MonitorIFAGCOut(pDemod, &ifAgc);
            if(cxd2828_result == CXD2828_RESULT_OK){
                mcDBG_MSG_L1(("Signal Level = %d\n", ifAgc * 100 / 4095));
            }else{
                mcDBG_MSG_L1(("Error in MonitorIFAGCOut\n"));
            }
        }
        /*BER */
        {
            UINT32 bitError = 0;
            UINT32 packetCnt = 0;
            UINT32 ber = 0;
            cxd2828_result = cxd2828_isdb_t_demod_MonitorPostViterbiBitError(pDemod, CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_A, &bitError, &packetCnt);
            if(cxd2828_result == CXD2828_RESULT_OK){
                ber = cxd2828_CalcBER(bitError, packetCnt);
                mcDBG_MSG_L1(("BER (Layer A) = %d.%05d\n", ber/100000U, ber%100000U));
            }else{
                mcDBG_MSG_L1(("Error in MonitorPostViterbiBitError\n"));
            }
            cxd2828_result = cxd2828_isdb_t_demod_MonitorPostViterbiBitError(pDemod, CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_B, &bitError, &packetCnt);
            if(cxd2828_result == CXD2828_RESULT_OK){
                ber = cxd2828_CalcBER(bitError, packetCnt);
                mcDBG_MSG_L1(("BER (Layer B) = %d.%05d\n", ber/100000U, ber%100000U));
            }else{
                mcDBG_MSG_L1(("Error in MonitorPostViterbiBitError\n"));
            }
            cxd2828_result = cxd2828_isdb_t_demod_MonitorPostViterbiBitError(pDemod, CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_C, &bitError, &packetCnt);
            if(cxd2828_result == CXD2828_RESULT_OK){
                ber = cxd2828_CalcBER(bitError, packetCnt);
                mcDBG_MSG_L1(("BER (Layer C) = %d.%05d\n", ber/100000U, ber%100000U));
            }else{
                mcDBG_MSG_L1(("Error in MonitorPostViterbiBitError\n"));
            }
        }
        /* PER */
        {
            UINT32 packetError = 0;
            UINT32 packetCnt = 0;
            UINT32 per = 0;
            cxd2828_result = cxd2828_isdb_t_demod_MonitorPostRSPacketError(pDemod, CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_A, &packetError, &packetCnt);
            if(cxd2828_result == CXD2828_RESULT_OK){
                per = cxd2828_CalcPER(packetError, packetCnt);
                mcDBG_MSG_L1(("PER (Layer A) = %d.%05d\n", per/100000U, per%100000U));
            }else{
                mcDBG_MSG_L1(("Error in MonitorPostRSPacketError\n"));
            }
            cxd2828_result = cxd2828_isdb_t_demod_MonitorPostRSPacketError(pDemod, CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_B, &packetError, &packetCnt);
            if(cxd2828_result == CXD2828_RESULT_OK){
                per = cxd2828_CalcPER(packetError, packetCnt);
                mcDBG_MSG_L1(("PER (Layer B) = %d.%05d\n", per/100000U, per%100000U));
            }else{
                mcDBG_MSG_L1(("Error in MonitorPostRSPacketError\n"));
            }
            cxd2828_result = cxd2828_isdb_t_demod_MonitorPostRSPacketError(pDemod, CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_C, &packetError, &packetCnt);
            if(cxd2828_result == CXD2828_RESULT_OK){
                per = cxd2828_CalcPER(packetError, packetCnt);
                mcDBG_MSG_L1(("PER (Layer C) = %d.%05d\n", per/100000U, per%100000U));
            }else{
                mcDBG_MSG_L1(("Error in MonitorPostRSPacketError\n"));
            }
        }
        /* SNR */
        {
            UINT32 merdt = 0;
            UINT32 mer = 0;
            cxd2828_result = cxd2828_isdb_t_demod_MonitorMERDT(pDemod, &merdt);
            if(cxd2828_result == CXD2828_RESULT_OK){
                mer = cxd2828_CalcMER(merdt);
                mcDBG_MSG_L1(("SNR = %d.%03d\n", mer/1000U, mer%1000U));
            }else{
                mcDBG_MSG_L1(("Error in MonitorMERDT\n"));
            }
        }
        /* TS Format */
        {
            TS_FMT_T tsfmt;
            if(cxd2828_GetTsFmt(pDemod, &tsfmt)){
                mcDBG_MSG_L1(("SerOrPar : %d\n", tsfmt.SerOrPar));
                mcDBG_MSG_L1(("ClockPolarity : %d\n", tsfmt.ClockPolarity));
                mcDBG_MSG_L1(("Tristate : %d\n", tsfmt.fgTristate));
                mcDBG_MSG_L1(("TsClock : %d\n", tsfmt.u4TsClockInKHz));
                mcDBG_MSG_L1(("GatedClock : %d\n", tsfmt.fgGatedClock));
                mcDBG_MSG_L1(("ExternalDemod : %d\n", tsfmt.fgExternalDemod));
            }else{
                mcDBG_MSG_L1(("Error in GetTsFmt\n"));
            }
        }
        /* TMCC */
        {
            cxd2828_isdb_t_tmcc_info_t tmccInfo;
            cxd2828_result = cxd2828_isdb_t_demod_MonitorTMCCInfo(pDemod, &tmccInfo);
            if(cxd2828_result == CXD2828_RESULT_OK){
                mcDBG_MSG_L1(("===== TMCC Information =====\n"));
                mcDBG_MSG_L1(("System         : %s\n", cxd2828_isdb_t_tmcc_system_t2msg(tmccInfo.system)));
                mcDBG_MSG_L1(("CountDownIndex : %d\n", tmccInfo.countDownIndex));
                mcDBG_MSG_L1(("EWS            : %d\n", tmccInfo.ewsFlag));
                mcDBG_MSG_L1(("Current\n"));
                mcDBG_MSG_L1((" isPartial     : %d\n", tmccInfo.xcurrent.isPartial));
                mcDBG_MSG_L1((" Layer A\n"));
                mcDBG_MSG_L1(("  Modulation   : %s\n", cxd2828_isdb_t_tmcc_modulation_t2msg(tmccInfo.xcurrent.layerA.modulation)));
                mcDBG_MSG_L1(("  CodingRate   : %s\n", cxd2828_isdb_t_tmcc_coding_rate_t2msg(tmccInfo.xcurrent.layerA.codingRate)));
                mcDBG_MSG_L1(("  Interleaving : %s\n", cxd2828_isdb_t_tmcc_il_length_t2msg(tmccInfo.xcurrent.layerA.ilLength)));
                mcDBG_MSG_L1(("  SegmentsNum  : %d\n", tmccInfo.xcurrent.layerA.segmentsNum));
                mcDBG_MSG_L1((" Layer B\n"));
                mcDBG_MSG_L1(("  Modulation   : %s\n", cxd2828_isdb_t_tmcc_modulation_t2msg(tmccInfo.xcurrent.layerB.modulation)));
                mcDBG_MSG_L1(("  CodingRate   : %s\n", cxd2828_isdb_t_tmcc_coding_rate_t2msg(tmccInfo.xcurrent.layerB.codingRate)));
                mcDBG_MSG_L1(("  Interleaving : %s\n", cxd2828_isdb_t_tmcc_il_length_t2msg(tmccInfo.xcurrent.layerB.ilLength)));
                mcDBG_MSG_L1(("  SegmentsNum  : %d\n", tmccInfo.xcurrent.layerB.segmentsNum));
                mcDBG_MSG_L1((" Layer C\n"));
                mcDBG_MSG_L1(("  Modulation   : %s\n", cxd2828_isdb_t_tmcc_modulation_t2msg(tmccInfo.xcurrent.layerC.modulation)));
                mcDBG_MSG_L1(("  CodingRate   : %s\n", cxd2828_isdb_t_tmcc_coding_rate_t2msg(tmccInfo.xcurrent.layerC.codingRate)));
                mcDBG_MSG_L1(("  Interleaving : %s\n", cxd2828_isdb_t_tmcc_il_length_t2msg(tmccInfo.xcurrent.layerC.ilLength)));
                mcDBG_MSG_L1(("  SegmentsNum  : %d\n", tmccInfo.xcurrent.layerC.segmentsNum));
                mcDBG_MSG_L1(("Next\n"));
                mcDBG_MSG_L1((" isPartial     : %d\n", tmccInfo.next.isPartial));
                mcDBG_MSG_L1((" Layer A\n"));
                mcDBG_MSG_L1(("  Modulation   : %s\n", cxd2828_isdb_t_tmcc_modulation_t2msg(tmccInfo.next.layerA.modulation)));
                mcDBG_MSG_L1(("  CodingRate   : %s\n", cxd2828_isdb_t_tmcc_coding_rate_t2msg(tmccInfo.next.layerA.codingRate)));
                mcDBG_MSG_L1(("  Interleaving : %s\n", cxd2828_isdb_t_tmcc_il_length_t2msg(tmccInfo.next.layerA.ilLength)));
                mcDBG_MSG_L1(("  SegmentsNum  : %d\n", tmccInfo.next.layerA.segmentsNum));
                mcDBG_MSG_L1((" Layer B\n"));
                mcDBG_MSG_L1(("  Modulation   : %s\n", cxd2828_isdb_t_tmcc_modulation_t2msg(tmccInfo.next.layerB.modulation)));
                mcDBG_MSG_L1(("  CodingRate   : %s\n", cxd2828_isdb_t_tmcc_coding_rate_t2msg(tmccInfo.next.layerB.codingRate)));
                mcDBG_MSG_L1(("  Interleaving : %s\n", cxd2828_isdb_t_tmcc_il_length_t2msg(tmccInfo.next.layerB.ilLength)));
                mcDBG_MSG_L1(("  SegmentsNum  : %d\n", tmccInfo.next.layerB.segmentsNum));
                mcDBG_MSG_L1((" Layer C\n"));
                mcDBG_MSG_L1(("  Modulation   : %s\n", cxd2828_isdb_t_tmcc_modulation_t2msg(tmccInfo.next.layerC.modulation)));
                mcDBG_MSG_L1(("  CodingRate   : %s\n", cxd2828_isdb_t_tmcc_coding_rate_t2msg(tmccInfo.next.layerC.codingRate)));
                mcDBG_MSG_L1(("  Interleaving : %s\n", cxd2828_isdb_t_tmcc_il_length_t2msg(tmccInfo.next.layerC.ilLength)));
                mcDBG_MSG_L1(("  SegmentsNum  : %d\n", tmccInfo.next.layerC.segmentsNum));
            }else{
                mcDBG_MSG_L1(("Error in MonitorTMCCInfo\n"));
            }
        }
        mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
        break;
    case 'd':
        {
            UINT8 data[256];
            UINT8 i2cAddress = 0;
            
            if(i4Argc < 2){
                mcDBG_MSG_INFO(("Error: No device address.\n"));
                return;
            }
            i2cAddress = (UINT8)cxd2828_strtoul(aszArgv[1]);
            mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
            cxd2828_result = pDemod->pI2c->ReadRegister(pDemod->pI2c, i2cAddress, 0x00, data, 256);
            mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
            if(cxd2828_result != CXD2828_RESULT_OK){
                mcDBG_MSG_L1(("I2C Error.\n"));
                return;
            }
            mcDBG_MSG_L1(("Register Dump Result. (0x%02X)\n", i2cAddress));
            mcDBG_MSG_L1(("     00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n"));
            {
                int i = 0;
                int j = 0;
                char dumpline[53];
                char* ptr = dumpline;
                const char hexchar[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

                dumpline[52] = '\0';
                for(i=0; i<16; i++){
                    ptr = dumpline;
                    for(j=0; j<16; j++){
                        if(j == 0){
                            *ptr++ = hexchar[(i*16) >> 4];
                            *ptr++ = hexchar[(i*16) & 0x0F];
                            *ptr++ = ' ';
                            *ptr++ = '|';
                        }
                        *ptr++ = ' ';
                        *ptr++ = hexchar[data[(i*16)+j] >> 4];
                        *ptr++ = hexchar[data[(i*16)+j] & 0x0F];
                    }
                    mcDBG_MSG_L1(("%s\n", dumpline));
                }
            }
            mcDBG_MSG_L1(("\n"));
        }
        break;
    case 'r':
        {
            UINT8 data[256];
            UINT8 subAddress = 0;
            UINT32 length = 0;
            UINT32 i = 0;
            if(i4Argc < 3){
                mcDBG_MSG_L1(("Error: No read register and length.\n"));
                return;
            }
            subAddress = (UINT8)cxd2828_strtoul(aszArgv[1]);
            length = (UINT32)cxd2828_strtoul(aszArgv[2]);
            if(length > 256){
                length = 256;
            }
            mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
            cxd2828_result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, subAddress, data, length);
            mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
            if(cxd2828_result != CXD2828_RESULT_OK){
                mcDBG_MSG_L1(("I2C Error.\n"));
                return;
            }
            for(i=0; i<length; i++){
                mcDBG_MSG_L1(("Demod Register 0x%02X = 0x%02X\n", i + subAddress, data[i]));
            }
        }
        break;
    case 'w':
        {
            UINT8 subAddress = 0;
            UINT8 value = 0;
            if(i4Argc < 3){
                mcDBG_MSG_L1(("Error: No write register and data.\n"));
                return;
            }
            subAddress = (UINT8)cxd2828_strtoul(aszArgv[1]);
            value = (UINT8)cxd2828_strtoul(aszArgv[2]);
            mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
            cxd2828_result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, subAddress, value);
            mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
            if(cxd2828_result != CXD2828_RESULT_OK){
                mcDBG_MSG_L1(("I2C Error.\n"));
                return;
            }
            mcDBG_MSG_L1(("Wrote Demod Register 0x%02X = 0x%02X\n", subAddress, value));
        }
        break;
    case 'f':
        {
            UINT32 u4Freq = 0;
            PARAM_SETFREQ_T param;

            if(i4Argc < 2){
                mcDBG_MSG_L1(("Error: No Frequency.\n"));
                return;
            }
            u4Freq = (UINT32)cxd2828_strtoul(aszArgv[1]); // freq
            
            param.Freq=u4Freq;
            param.Modulation=MOD_ISDBT;
            mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
            if(ITuner_SetFreq(ITunerGetCtx(), &param))
            {
                mcDBG_MSG_L1(("Failed on Config tuner (%d KHz)\n", u4Freq));
            }
            else
            {
                mcDBG_MSG_L1(("Config tuner(%d KHz) OK!\n", u4Freq));
            }
            mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
        }
        break;
    case 't':
        {
            INT16 u2OPRet = 0;
            BOOL bTunerSts = FALSE;
            mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
            u2OPRet = ITuner_OP(ITunerGetCtx(), itGetStatus, 0, &bTunerSts);
            mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
            if(u2OPRet)
            {
                mcDBG_MSG_L1(("ITuner_OP (eOperation = %d) Fail! Return Value = %d\n", itGetStatus, u2OPRet));
            }
            else
            {
                mcDBG_MSG_L1(("Tuner PLL Status = %d (0:Unlock, 1:Lock)\n", bTunerSts));
            }
        }
        break;
    default:
        break;
    }
}

static void ISDBT_TunerDetachI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bDetach)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //Not used

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
	INT16 retval = DRVAPI_TUNER_ERROR;
       cxd2828_result_t cxd2828_result = CXD2828_RESULT_OK;
	cxd2828_isdb_t_demod_t *pDemod = &pCxd2828DemodCtx->demod;

	mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here
	 mcDBG_MSG_L4((" %s \n",__FUNCTION__));

    mcDBG_MSG_L4(("GetType : %d.\n", e_get_type));

    switch (e_get_type)
    {
    case TUNER_GET_TYPE_BER: 
        *(UINT32*)pzAttribute = ISDBT_TunerGetSignalBER(ptTDSpecificCtx, 0 /* dummy */);
        break;
    case TUNER_GET_TYPE_TS_FMT:
        {
            TS_FMT_T *pTsfmt=(TS_FMT_T *)pzAttribute;
            if(!cxd2828_GetTsFmt(pDemod, pTsfmt)){
                goto ErrorExit;
            }
        }
        *pzAttributeLen = sizeof(TS_FMT_T); 
        break;
    case TUNER_GET_SPECIAL_TYPE_EWS_STATUS:
        {
            EWS_INFO *pEwsInfo = (EWS_INFO*)pzAttribute;
	     cxd2828_isdb_t_tmcc_info_t tmccInfo;
            BOOL ewsFlag = FALSE;

	     cxd2828_result = cxd2828_isdb_t_demod_MonitorTMCCInfo(pDemod, &tmccInfo);
            if(cxd2828_result == CXD2828_RESULT_OK)
            {
                ewsFlag = tmccInfo.ewsFlag ? TRUE : FALSE;
            }else{
                ewsFlag = pCxd2828DemodCtx->fgEws; // Previous EWS flag
            }

            // Set current value.
            pEwsInfo->val = ewsFlag ? 1 : 0;

            // Check that EWS flag was changed.
            if(pCxd2828DemodCtx->fgEws != ewsFlag){
                pEwsInfo->isNotify = TRUE;
            }else{
                pEwsInfo->isNotify = FALSE;
            }

            // Save current value
            pCxd2828DemodCtx->fgEws = ewsFlag;
            *pzAttributeLen = sizeof(EWS_INFO);
        }
	 break;
    default:
        //mcDBG_MSG_ERR(("unknown: get Attribute %d\r \n",e_get_type));
        break;
    }

    retval = DRVAPI_TUNER_OK;
ErrorExit:
    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return retval;
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
    //Not used

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return DRVAPI_TUNER_OK;
}
#ifdef fcADD_DIAG_INFO
static INT32 ISDBT_TunerSetDiagCmd(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CUSTOM_TUNER_SET_TYPE_T e_tuner_set_type, VOID* pv_set_info, SIZE_T z_size)
{
    INT32   retSts = RMR_OK;
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
	//Not used

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    return retSts;
}
static INT32 ISDBT_TunerGetDiagInfo(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CUSTOM_TUNER_GET_TYPE_T e_tuner_get_type, VOID* pv_get_info, SIZE_T* pz_size)
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
static void ISDBT_TunerBypassI2C (PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bSwitchOn)

{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //TODO  Bypass i2c to tuner connected(bSwitchOn is TRUE) or disconnect(bSwitchOn is FALSE)
	
	//END
    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}
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
    //pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalPER_Layer = ISDBT_TunerGetSignalPER_Layer;
    //pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalBER = ISDBT_TunerGetSignalBER;
    //pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalBER_Layer = ISDBT_TunerGetSignalBER_Layer;
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
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerBypassI2C= ISDBT_TunerBypassI2C;
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
INT32 Vendor_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* pptTDSpecificCtx)
{
    //create pdctx used in this file, you also can add any declaration you want in pdctx		 
    cxd2828_result_t cxd2828_result=CXD2828_RESULT_OK;
    cxd2828_isdb_t_demod_t *pDemod = NULL;
    cxd2828_i2c_t *pI2c = NULL;
   
    mcDBG_MSG_L2((" %s \n",__FUNCTION__));

    pPdCtx = (ISDBT_PD_CTX_T *) x_mem_calloc(1, sizeof(ISDBT_PD_CTX_T));
    if (pPdCtx == NULL)
    {
        mcDBG_MSG_ERR(("ISDBT_PD_CTX_T malloc fail\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    *pptTDSpecificCtx = pPdCtx;
	pIsdbtTunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;
	Printf("pIsdbtTunerDeviceCtx->tunerType=0x%02X\n",pIsdbtTunerDeviceCtx->tunerType);

    // create cxd2828 specific ctx
    pCxd2828DemodCtx = (cxd2828_demod_ctx_t *) x_mem_calloc(1, sizeof(cxd2828_demod_ctx_t));
    if(pCxd2828DemodCtx == NULL){
        mcDBG_MSG_ERR(("cxd2828_demod_ctx_t malloc fail\n"));
        return (DRVAPI_TUNER_ERROR);
    }

    pCxd2828DemodCtx->fgEws = FALSE;
    pDemod = &pCxd2828DemodCtx->demod;
    pI2c = &pCxd2828DemodCtx->i2c;

#if 1
    //Get i2c Instance
    cxd2828_result = cxd2828_i2c_MTK_CreateI2c(pI2c);
    if(cxd2828_result != CXD2828_RESULT_OK){return DRVAPI_TUNER_ERROR;}
    // Get demod instance
    cxd2828_result = cxd2828_cxd2828_Create(pDemod, pI2c, CXD2828_DEMOD_I2C_ADDRESS);
    if(cxd2828_result != CXD2828_RESULT_OK){return DRVAPI_TUNER_ERROR;}

#endif
    //Create a semaphore for low level demod PI access protection
    if (x_sema_create(&pPdCtx->hHalLock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcDBG_MSG_ERR(("%s:  x_sema_create(pPdCtx->hHalLock) Fail!\n",__FUNCTION__));
        return (DRVAPI_TUNER_ERROR);
    }
    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    //register LTDIS APIs to HAL
    PD_ISDBT_Register_LTDIS_Fct(ptTDCtx);
    vResetDemod(100); // 100 ms
    //Regisiter tuner driver
	ITunerRegistration(pIsdbtTunerDeviceCtx->tunerType);
	ITuner_Init(ITunerGetCtx());

    mcDBG_MSG_L2(("Starting Cxd2828Demod Initialization.\n"));
	
	//TODO Here do demod init.If demod init fail,return DRVAPI_TUNER_ERROR
    //Initialize Demod
    cxd2828_result = cxd2828_isdb_t_demod_Initialize(pDemod);
    if(cxd2828_result != CXD2828_RESULT_OK){return DRVAPI_TUNER_ERROR;};

    // Initiale setting if need
    // cxd2828_isdb_t_dmod_config_t config;
    // UINT32 value;
    // cxd2828_result=cxd2828_isdb_t_demod_SetConfig(pDemod, config, value);
    // if(cxd2828_result!=CXD2828_RESULT_OK){return DRVAPI_TUNER_ERROR;}

    /* TS output format setting */
    cxd2828_result = cxd2828_cxd2828_TSOutputSetting(pDemod, CXD2828_CXD2828_TSOUT_SERIAL_T2_N, 0);
    if(cxd2828_result != CXD2828_RESULT_OK){return DRVAPI_TUNER_ERROR;};

    /*
     * Limit modulation types
     * (0:Full ISDB-T support, 1:Limit operation to the Japanese Operational Guidelines(default))
     */
    cxd2828_result = cxd2828_isdb_t_demod_SetConfig(pDemod, CXD2828_ISDB_T_DEMOD_CONFIG_OREG_SYR_JPOG, 0);
    if(cxd2828_result != CXD2828_RESULT_OK){return DRVAPI_TUNER_ERROR;};

    /*
    * Enable Auto Spectrum Inversion
    */
    cxd2828_result = cxd2828_isdb_t_demod_SetConfig(pDemod, CXD2828_ISDB_T_DEMOD_CONFIG_OREG_CCOD_SINV_SEARCH_ON, 1);
    if(cxd2828_result != CXD2828_RESULT_OK){return DRVAPI_TUNER_ERROR;};

//#ifdef ISDBT_ENABLE_MONITOR
    if(pIsdbtTunerDeviceCtx->u1MWIntf != cMTAL_IF && pIsdbtTunerDeviceCtx->u1MWIntf != cSDAL_IF)
    {
        if (x_sema_create(&pPdCtx->t_escape_mon_suspend, X_SEMA_TYPE_BINARY,X_SEMA_STATE_LOCK) != OSR_OK)
        {    
            mcDBG_MSG_ERR(("EN_Vendor_TunerOpen (mcSEMA_CREATE): ERROR!\n"));
            return (DRVAPI_TUNER_ERROR);
        }

        pPdCtx->fgBreakMonitor = TRUE;      //init
        pPdCtx->fgPIMoniStatus = FALSE;     //init
        PD_StartMonitorEngine(pPdCtx);   //start monitor engine
    }
//#endif

    return (DRVAPI_TUNER_OK);
}


// For Debugging
void cxd2828_trace_log_enter(const char* funcname, const char* filename, unsigned int linenum)
{
    mcDBG_MSG_L4(("CXD2828_ENTER : %s (%d)\n", funcname, linenum));
}
void cxd2828_trace_log_return(cxd2828_result_t result, const char* filename, unsigned int linenum)
{
    if(result != CXD2828_RESULT_OK){
        mcDBG_MSG_L4(("CXD2828_ERROR_RETURN(%d) : (line %d)\n", result, linenum));
    }else{
        mcDBG_MSG_L4(("CXD2828_RETURN : (line %d))\n", linenum));
    }
}

void cxd2828_trace_i2c_log_enter(const char* funcname, const char* filename, unsigned int linenum)
{
    cxd2828_trace_log_enter(funcname, filename, linenum);
}
void cxd2828_trace_i2c_log_return(cxd2828_result_t result, const char* filename, unsigned int linenum)
{
    cxd2828_trace_log_return(result, filename, linenum);
}
