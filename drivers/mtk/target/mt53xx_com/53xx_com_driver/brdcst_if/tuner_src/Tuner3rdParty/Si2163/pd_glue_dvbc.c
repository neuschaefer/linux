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
 * $RCSfile: pd_glue_dvbc.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
/** @file pd_glue_dvbc.c
 *  dvbc glue layer
 */
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "u_tuner.h"
#include "tuner_if.h"
#include "pd_tuner_type.h"
#include "tuner_interface_if.h"
#include "queue.h"
#include "TDIS.h"
#include "TDMgr.h"
#include "pd_si2163_dvbc.h"
#include "Emb_L1_Driver_Core.h"
#include "pi_demod.h"
#include "drvapi_tuner.h"
#include "UtilAPI.h"
#include "pod_if.h"
#include "drvcust_if.h"
#include "eeprom_if.h"


#define EMB_L1_DRIVER_CORE          /* empty  */
#define SLAB_WINAPI                 /* empty  */
#define EMB_L1_COMMON_DEMOD_API     /* empty  */
#define EMB_L1_MULTISTANDARD_API    /* empty  */
#define EMB_L1_DVB_C_DEMOD_API      /* empty  */
#define WINAPI                      /* empty  */

#include "platform_definition.h"
#include "Emb_L1_DVB_C_Functions.h"
#include "Emb_L1_Common_Demod_API.h"
#include "Emb_L1_Multistandard_API.h"
#include "Emb_L1_Registers_Defines.h"



//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define cAcqTimeOutCnt   20
#define cAcqSymbols      2000000

#define DVBC_TUNER_CONN_TIME_THREHOLD   140
#define DVBC_DEMOD_CONN_TIME_THREHOLD   200

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define mcSEMA_LOCK_MTFE(sema)                                       \
{                                                                    \
    mcSHOW_DBG_MSG4((" [ _Lock_%s-%s-%d ]\r\n", __FUNCTION__, __LINE__)); \
    x_sema_lock(sema, X_SEMA_OPTION_WAIT);                           \
    mcSHOW_DBG_MSG4((" [ _Excape_%s-%s-%d ]\r\n", __FUNCTION__, __LINE__));     \
}
#define mcSEMA_UNLOCK_MTFE(sema)                                     \
{                                                                    \
    mcSHOW_DBG_MSG4((" [ _Unlock_%s-%s-%d ]\r\n", __FUNCTION__, __LINE__));     \
    x_sema_unlock(sema);                                             \
}

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static DVBC_PD_CTX_T *pPdCtxDVBC;
static L1_Context *pSpiderDemod;
static UINT32 ui4PreFreq = 100;


//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifdef DVBC_ENABLE_MONITOR
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
            mcDELAY_MS(10); 
        }            
    }
    
    return TRUE;
}
STATIC void ResumeMonitor(DVBC_PD_CTX_T *psPdCtx)
{
    psPdCtx->fgBreakMonitor = FALSE;
    mcMUTEX_UNLOCK(psPdCtx->t_escape_mon_suspend);
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
    
    mcSHOW_DRVAPI_MSG(("MonitorProcess start!\n"));
        
    while (psPdCtx->fgMonitorThreadRunning)
    {
    	if (psPdCtx->fgBreakMonitor)
    	{
    		// After mcMUTEX_LOCK(t_escape_mon_suspend), monitor thread will be suspended until
    		// TunerAcq or TunerClose invoked.
            mcSHOW_DRVAPI_MSG(("Entering DVBC mon_suspend\n"));
    		mcMUTEX_LOCK(psPdCtx->t_escape_mon_suspend);
    		mcSHOW_DRVAPI_MSG(("Escape DVBC mon_suspend\n"));
    	}
    	else
    	{
		    // mutex lock for I2C access
            mcSEMA_LOCK_MTFE(psPdCtx->hHalLock);
	        mcSHOW_DBG_MSG2(("Invoke DVBC_Monitor\n"));
            //add PI monitor API here 
		    psPdCtx->fgPIMoniStatus = TRUE; //doing PI monitor

            //! put your monitor process code here
            /////
            
            /////
            
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
STATIC S32 PD_StartMonitorEngine(DVBC_PD_CTX_T *psPdCtx)
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
STATIC S32 PD_StopMonitorEngine(void)
{
//    pPdCtxDVBC->fgMonitorThreadRunning = FALSE; //need set to false before mcMUTEX_UNLOCK(t_escape_mon_suspend);
    while (pPdCtxDVBC->t_monitor_thread){
        mcDELAY_MS(10); // in ms
    }
    mcSHOW_DRVAPI_MSG(("DVBC PD_StopMonitorEngine success\n"));
    return (DRVAPI_TUNER_OK);
}
#endif //DVBC_ENABLE_MONITOR

STATIC INT8 Tuner_Connect(U32 Freq, U8 sawbw, U8 AutoSearch)
{
    PARAM_SETFREQ_T SetFreqPara;	
    INT8 fgRet = 0;

    ITuner_OP(ITunerGetCtx(), itSetSawBw, sawbw, NULL);
   
    SetFreqPara.Freq = Freq;
    SetFreqPara.Modulation = MOD_DVBC;
    SetFreqPara.fgAutoSearch = AutoSearch;   
    if((ITuner_SetFreq(ITunerGetCtx(), &SetFreqPara)) != 0) {
        mcSHOW_USER_MSG(("Failed on Tuner_SetFreq(%d)", Freq));
        fgRet = 1;
    }

    return fgRet;
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
INT32 DVBC_TunerClose(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);
    
//#if !defined(CC_MTAL) && !defined(CC_SDAL)    
#ifdef DVBC_ENABLE_MONITOR
    pPdCtxDVBC->fgBreakMonitor = TRUE; //disable and breadk monitor thread
    pPdCtxDVBC->fgMonitorThreadRunning = FALSE; //need set to false before mcMUTEX_UNLOCK(t_escape_mon_suspend);
    mcMUTEX_UNLOCK(pPdCtxDVBC->t_escape_mon_suspend);
    PD_StopMonitorEngine();  // stop monitor engine
    mcMUTEX_DESTROY(pPdCtxDVBC->t_escape_mon_suspend);  // destory monitor thread
#endif

    //If you have allocated any resource in TunerOpen(), release them here.
    L1_Demod_SW_Destroy(pSpiderDemod);

    //add driver implementation code here
    x_sema_delete(pPdCtxDVBC->hHalLock);
    x_mem_free(pPdCtxDVBC);

    return (DRVAPI_TUNER_OK);
}

VOID DVBC_SetTSInterface(BOOL fgParallel)
{
#ifdef CC_TURNKEY
    UCHAR ucPar = 0;
#endif

    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);

    if (fgParallel) {
        mcSHOW_DBG_MSG(("[DVBC] SetTSInterface : parallel\n"));
    }
    else
    {
        mcSHOW_DBG_MSG(("[DVBC] SetTSInterface : serial\n"));
    }

    L1_Demod_SetTSInterface(pSpiderDemod, fgParallel);

#ifdef CC_TURNKEY
    ucPar = 0;
    if (L1_WriteBytes8Bits(pSpiderDemod, 0x4f4, 1, &ucPar) == 0);
    if (L1_WriteBytes8Bits(pSpiderDemod, 0x4f5, 1, &ucPar) == 0);
    if (L1_WriteBytes8Bits(pSpiderDemod, 0x4f6, 1, &ucPar) == 0);
#endif 

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);
}

void DVBC_SetTSPath(BOOL fgEnable)
{
    UCHAR  ucPar = 0;

    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);

    if (fgEnable) {
        mcSHOW_DBG_MSG(("[DVBC] SetTSPath : pass-through"));
        //Enable DVBC TS output
        ucPar = 0;
        L1_WriteBytes8Bits(pSpiderDemod, 0x4EF, 1, &ucPar);
        L1_WriteBytes8Bits(pSpiderDemod, 0x4F0, 1, &ucPar);
    }
    else {
        mcSHOW_DBG_MSG(("[DVBC] SetTSPath : tri-state"));
        //Set DVBC TS output to high-Z
        ucPar = 0xFF;
        L1_WriteBytes8Bits(pSpiderDemod, 0x4EF, 1, &ucPar);
        ucPar = 0x0F;
        L1_WriteBytes8Bits(pSpiderDemod, 0x4F0, 1, &ucPar);
//        L1_Demod_TS_mode(pSpiderDemod, 2);
    }

#ifdef CC_TURNKEY
    ucPar = 0;
    if (L1_WriteBytes8Bits(pSpiderDemod, 0x4f4, 1, &ucPar) == 0);
    if (L1_WriteBytes8Bits(pSpiderDemod, 0x4f5, 1, &ucPar) == 0);
    if (L1_WriteBytes8Bits(pSpiderDemod, 0x4f6, 1, &ucPar) == 0);
#endif 

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);
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
 *  @retval  1                  Demod Lock
 *  @retval  0                  Demod Unlock
 */
//-------------------------------------------------------------------------
BOOL DVBC_TunerAcq(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type, void* pv_conn_info,
                            SIZE_T z_conn_info_len, x_break_fct _BreakFct, void* pvArg)
{
    TUNER_CAB_DIG_TUNE_INFO_T *pt_tuner_info = 
			    	(TUNER_CAB_DIG_TUNE_INFO_T *) pv_conn_info;

    UINT8   fgChannelScan = FALSE;
    UINT32  freq = 0;
    UINT32  symbolRate = 0;
    UINT32  mode;
    UINT32  ii;
    UINT32  u4TimeOut = 0;
    UINT8   LockStatus = 0;
    UINT32  u4TickCnt, u4ConnTime;
    UINT8   u1EarlyBreak = 0;
    UINT8   u1IFAGCTreshold = 220;	
    UINT8   u1Count = 0;	

    //Set break function
    pPdCtxDVBC->isBreak = _BreakFct;

    //Set DVB-C scan mode
    pPdCtxDVBC->t_ScanMode = pt_tuner_info->e_conn_mode;

    //test for extra early break
    if (pPdCtxDVBC->t_ScanMode == CONN_MODE_SCAN) {
	 fgChannelScan = TRUE;
	 mcSHOW_DBG_MSG(("fgChannelScan = TRUE\n"));
	 
        if (pt_tuner_info->ui4_freq == ui4PreFreq) {
            return LockStatus;
        }
    }
    else {
        ui4PreFreq = 100;
    }
    

    //MUTEX lock
    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);

#ifdef DVBC_ENABLE_MONITOR
    SetBreakMonitor(pPdCtxDVBC);
#endif

    u4TickCnt = os_get_sys_tick();

    //Demod init
    if (DVBC_ChipInit(pSpiderDemod) == 1){
        mcSHOW_USER_MSG(("Failed on DVBC_ChipInit()\n"));
    }
    else
    {
        mcSHOW_DRVAPI_MSG(("Success on DVBC_ChipInit()\n"));
    }

    //Convert freq
    freq = pt_tuner_info->ui4_freq/1000;

    //Convert symbol rate
    symbolRate = pt_tuner_info->ui4_sym_rate;
    u4TimeOut = (cAcqSymbols*1000)/symbolRate/10;

    //Convert QAM mode
    switch (pt_tuner_info->e_mod)
    {
    case MOD_QPSK:
        mode = constellation_qpsk;
        break;
    case MOD_QAM_16:
        mode = constellation_qam16;
        break;
    case MOD_QAM_32:
        mode = constellation_qam32;
        break;
    case MOD_QAM_64:
        mode = constellation_qam64;
        break;
    case MOD_QAM_128:
        mode = constellation_qam128;
        break;
    case MOD_QAM_256:
        mode = constellation_qam256;
        break;
    default:
        mode = constellation_qam64;
        mcSHOW_DRVAPI_MSG(("Invalid input modulation(%d) to DVBC_TunerAcq!\n", pt_tuner_info->e_mod));
        break;
    }
        
    mcSHOW_DRVAPI_MSG(("Freq = %d\n", freq));
    mcSHOW_DRVAPI_MSG(("symbol rate = %d\n", symbolRate));
    mcSHOW_DRVAPI_MSG(("Mode = %d\n", mode));
    mcSHOW_DRVAPI_MSG(("Time-out cnt = %d\n", u4TimeOut));

    //Set freq
    L1_WRITE(pSpiderDemod, i2c_passthru      , i2c_passthru_enabled);
    Tuner_Connect(freq, SAW_BW_8M, fgChannelScan);
    L1_WRITE(pSpiderDemod, i2c_passthru      , i2c_passthru_disabled);

    //Check tuner conncection time
    u4ConnTime = (os_get_sys_tick()-u4TickCnt) * x_os_drv_get_tick_period();
    mcSHOW_DBG_MSG(("DVBC tuner connection time = %dms\n", u4ConnTime));
    if ((u4ConnTime > DVBC_TUNER_CONN_TIME_THREHOLD) && (fgChannelScan)){
	    mcSHOW_DBG_MSG(("EU Cable Driver ATP Fail: DVBC tuner connection time = %dms, too long!!\n", u4ConnTime));
	}

    u4TickCnt = os_get_sys_tick();

    //Set symbol rate
    L1_DVB_C_set_symbol_rate  (pSpiderDemod, symbolRate);

    //Set QAM mode
    L1_DVB_C_set_constellation(pSpiderDemod, mode);

    /* Note that for DVB-C it is necesary to call the reset function to relock the demod  */
    L1_Demod_reset(pSpiderDemod);
    
    ITuner_OP(ITunerGetCtx(), itIFAGCTreshold, 0, &u1IFAGCTreshold);

    //Polling lock status
    u1Count = 0;
    for (ii = 0; ii < u4TimeOut; ii ++)
    {
        if (L1_Demod_get_IFagc(pSpiderDemod) > u1IFAGCTreshold)			
	{
            u1Count++;
	     mcSHOW_DBG_MSG(("L1_Demod_get_IFagc =%d,prepare to early break!\n",L1_Demod_get_IFagc(pSpiderDemod)));		

	     if (u1Count > 2)
	     	{
                u1EarlyBreak = 1;
                break;
	     	}	
        }

        LockStatus = (UINT8)L1_Demod_get_demod_lock(pSpiderDemod);
        mcSHOW_DBG_MSG3(("Demod lock status = %d\n", LockStatus));

        if (LockStatus == 1) {
            LockStatus = L1_Demod_get_fec_lock(pSpiderDemod);
            mcSHOW_DBG_MSG3(("FEC lock status = %d\n", LockStatus));
        }
       if (LockStatus==1)   
	 {
              UINT32 crctsymbolRate =L1_DVB_C_corrected_symbol_rate(pSpiderDemod);
		mcSHOW_DRVAPI_MSG(("[LEO]demod detect symbol rate = %d\n",crctsymbolRate));	  

		if (((crctsymbolRate  > symbolRate) && (crctsymbolRate-symbolRate >10000))	|| \
			((crctsymbolRate  < symbolRate) && (symbolRate- crctsymbolRate>10000)))
		{
               LockStatus = 0;
		 mcSHOW_DRVAPI_MSG(("symbol rate offset too large,drop!\n", LockStatus));		  
		}

//timing offset check, add to check PPM value	
		INT32 timingOffset = (INT32)L1_Demod_get_timing_offset(pSpiderDemod);
              mcSHOW_DRVAPI_MSG(("[LEO]demod detect symbol rate = %d\n",crctsymbolRate));
			  
		if ((timingOffset >2000)||(timingOffset < -2000))
            	{
               LockStatus = 0;
		 mcSHOW_DRVAPI_MSG(("ppm too large , drop the lock status!\n", LockStatus));	
            	}
	    	
	 }	
        mcSHOW_DRVAPI_MSG(("System lock status = %d\n", LockStatus));

        //test
        mcSHOW_HW_MSG(("IFAGC = %d\n", L1_Demod_get_IFagc(pSpiderDemod)));
        
        //Front end
        if (LockStatus == 1) {break;}

        mcDELAY_MS(10);
        
        if (pPdCtxDVBC->isBreak(pvArg))
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
        u4ConnTime = (os_get_sys_tick()-u4TickCnt) * x_os_drv_get_tick_period();
        mcSHOW_DBG_MSG(("DVBC demod connection time = %dms\n", u4ConnTime));
        if ((u4ConnTime > DVBC_DEMOD_CONN_TIME_THREHOLD) && (fgChannelScan)){
            mcSHOW_DBG_MSG(("EU Cable Driver ATP Fail: DVBC demod connection time = %dms, too long!!\n", u4ConnTime));
        }
    }
    else if (u1EarlyBreak == 1) {
        ui4PreFreq = pt_tuner_info->ui4_freq;
        mcSHOW_DRVAPI_MSG(("Signal weak, early break\n"));
    }
    else {
        mcSHOW_DRVAPI_MSG(("MW break\n"));
    }
    
#ifdef DVBC_ENABLE_MONITOR
    if (!pPdCtxDVBC->isBreak(pvArg))// && (u1ConnMode == ATV_CONN_MODE_CHAN_CHANGE))
    {
        //psAtvPdCtx->fgBreakMonitor = FALSE;           // Enable monitor thread and Disable break function flag
        //mcMUTEX_UNLOCK(t_escape_anaeu_mon_suspend);  // Unlock monitor thread
        ResumeMonitor(pPdCtxDVBC);   // Enable monitor thread and Disable break function flag,  Unlock monitor thread
        mcSHOW_DBG_MSG(("Enable monitor process!!\n"));
    }
#endif

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);

	/* Init Disconnect Status */
	pPdCtxDVBC->fgDisStatus = FALSE;	

    return LockStatus; //if lock return 1, unlock return 0
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
INT16 DVBC_TunerGetSync(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT16 u2Ret=0;
    UINT8 LockStatus = 0;

    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);
    //add driver implementation code here
    LockStatus = (UINT8)L1_Demod_get_demod_lock(pSpiderDemod);
    if (LockStatus == 1) {
        LockStatus = L1_Demod_get_fec_lock(pSpiderDemod);
    }

    u2Ret = (UINT16)LockStatus;
    
    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);
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
VOID  DVBC_TunerDisc(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
	if(pPdCtxDVBC->fgDisStatus == TRUE)
	{
		/* Has Disconnected, return */
		mcSHOW_DBG_MSG(("DVBC Has Disconnect!\n"));
		return;
	}
    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);
    //add driver implementation code here
    if (DVBC_ChipPowerDown(pSpiderDemod) == 1){
        mcSHOW_USER_MSG(("Failed on DVBC_ChipPowerDown()\n"));
    }

#ifdef DVBC_ENABLE_MONITOR
    SetBreakMonitor(pPdCtxDVBC);
#endif

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);

	/* set disconnect status */
	pPdCtxDVBC->fgDisStatus = TRUE;
}

void DVBC_TunerBypassI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bSwitchOn)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);

    if (bSwitchOn == TRUE) {
        L1_WRITE(pSpiderDemod, i2c_passthru      , i2c_passthru_enabled);
    }
    else
    {
        L1_WRITE(pSpiderDemod, i2c_passthru      , i2c_passthru_disabled);
    }

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);
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
void DVBC_TunerGetSignal(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type, SIGNAL *_pSignal)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);
    //add driver implementation code here
    // for testing, need to modify
    Si2165_Context *Si2165;
    ITUNER_CTX_T *pTunerCtx;

    Si2165  = (Si2165_Context *) pSpiderDemod->specific;
    pTunerCtx = ITunerGetCtx();

    if (pPdCtxDVBC->t_ScanMode == CONN_MODE_SCAN) {
		
     S32 FreqOffset = (S32)L1_Demod_get_freq_offset(pSpiderDemod)/1000;  
	 
     if ((FreqOffset >= 400) ||(FreqOffset <= -400))
     {
         mcSHOW_DBG_MSG(("DVBC_TunerGetSignal Geg Freq Offset = %d,tooo large!\n", FreqOffset));
	     FreqOffset = 350;   //minus 5khz
     }	

    if ((FreqOffset >= -100) && (FreqOffset <= 100))
     {
         mcSHOW_DBG_MSG(("DVBC_TunerGetSignal Geg Freq Offset = %d,tooo small!\n", FreqOffset));
	     FreqOffset = 0;   
     }		 
	 	
//        _pSignal->Frequency  = (S32)((S32)pTunerCtx->u4RF_Freq + (S32)L1_Demod_get_freq_offset(pSpiderDemod)/1000);
        _pSignal->Frequency  = (S32)((S32)pTunerCtx->u4RF_Freq + FreqOffset);
    }
    else {
        _pSignal->Frequency  = (S32)(pTunerCtx->u4RF_Freq);
    }
    _pSignal->SymbolRate = (U32)(Si2165->symbol_rate_Hz/1000);

    switch (Si2165->constellation)
    {
    case constellation_qpsk:
    	_pSignal->QAMSize = 4;
    	break;
    case constellation_qam16:
    	_pSignal->QAMSize = 16;
    	break;
    case constellation_qam32:
       _pSignal->QAMSize = 32;
       break;
    case constellation_qam64:
    	_pSignal->QAMSize = 64;
    	break;
    case constellation_qam128:
    	_pSignal->QAMSize = 128;
    	break;
    case constellation_qam256:
    	_pSignal->QAMSize = 256;
    	break;
    default:
    	_pSignal->QAMSize = 64;
    	break;
    }
    
    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);

}

//-------------------------------------------------------------------------
/**
 *  This API get signal level in terms of percentage.
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  0~100              signal level
 */
//-------------------------------------------------------------------------
UINT8 DVBC_TunerGetSignalLevel(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT8 u1Ret=0;
    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);
    //add driver implementation code here
    u1Ret = (UINT8)L1_Demod_GetSignalLevel(pSpiderDemod);

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);
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
INT16 DVBC_TunerGetSignalLeveldBm(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    INT16 i2Ret=0;
    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);
    //add driver implementation code here
    mcSHOW_DRVERR_MSG(("No implementation in DVBC_TunerGetSignalLeveldBm()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);
    return i2Ret;
}

UINT16 DVBC_TunerGetSignalPER(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{

    //INT32 u4_ret;
    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBC_TunerGetSignalPER()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);

    return 0;//modify return value

}

UINT32 DVBC_TunerGetSignalBER(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{

    UINT32 u4_ret = 0;
    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBC_TunerGetSignalBER()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);

    return u4_ret;//modify return value

}

UINT16 DVBC_TunerGetSignalSNR(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT16 SNR = 0;
    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);

	//add your implementation here
    SNR = (UINT16)(L1_Demod_get_C_N_dB_int(pSpiderDemod)/1000);

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);

    return SNR;//modify return value
}

void DVBC_TunerSetMpgFmt(PTD_SPECIFIC_CTX ptTDSpecificCtx, MPEG_FMT_T  *pt_mpeg_fmt)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBC_TunerSetMpgFmt()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);
}

char *DVBC_TunerGetVer(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBC_TunerGetVer()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);
    return NULL;
}

INT32 DVBC_TunerNimTest(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    INT32 i4Ret = 0;
    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);

	//add your implementation here
    L1_WRITE(pSpiderDemod, i2c_passthru      , i2c_passthru_enabled);
    i4Ret = Tuner_Connect(666000, SAW_BW_8M, FALSE);
    L1_WRITE(pSpiderDemod, i2c_passthru      , i2c_passthru_disabled);

    mcSHOW_DRVERR_MSG(("DVBC_TunerNimTest : %d (0:OK, 1:fail)\n", i4Ret));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);
    return i4Ret;
}
void DVBC_TunerSetRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx, UCHAR ucRegSet, UCHAR ucRegAddr, UCHAR ucRegValue)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBC_TunerSetRegSetting()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);
}

void DVBC_TunerShowRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBC_TunerShowRegSetting()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);
}

void DVBC_TunerTestI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBC_TunerTestI2C()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);
}
void DVBC_TunerCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx, INT32 i4Argc, const CHAR** aszArgv)
{
    UCHAR   ucArg1;
    UCHAR   ucCmdSize = 0;
    //UINT8 tunerType;
    UINT8 ucData[256];
    UCHAR   ucPar;
    U16    ii, u2RegAddr;
    CHAR * txt;
    txt   = (CHAR*) x_mem_calloc(1, sizeof(CHAR)*1000);
    
	mcSHOW_DBG_MSG4(("DVBC_TunerCommand pPdCtx->hHalLock\n"));
    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);
	mcSHOW_DBG_MSG4(("escape DVBC_TunerCommand pPdCtx->hHalLock\n"));
    	    
    if (i4Argc > 0) {
        //ucCmdId = *((CHAR *) aszArgv[0]);
        ucCmdSize = x_strlen((CHAR *) aszArgv[0]);
    }

    switch (ucCmdSize) {
    case 1: // one-word cmd
        if (x_strcmp((CHAR *) aszArgv[0], "t") == 0) {
            L1_WRITE(pSpiderDemod, i2c_passthru      , i2c_passthru_enabled);
            Tuner_Connect(StrToInt(aszArgv[1]), SAW_BW_8M, 0);
            L1_WRITE(pSpiderDemod, i2c_passthru      , i2c_passthru_disabled);
        }
        else if (x_strcmp((CHAR *) aszArgv[0], "r") == 0) {
            if (i4Argc < 3)
                break;
                
            u2RegAddr = (U16)StrToInt(aszArgv[1]);
            ucPar = (U8)StrToInt(aszArgv[2]);
            ucArg1 = (UCHAR)L1_ReadBytes8Bits(pSpiderDemod, u2RegAddr, ucPar, ucData);
            if (ucArg1 == 0)
            {        
                mcSHOW_USER_MSG(("Spider Get Register 0x%03X Fail!\n", u2RegAddr));  
                break;
            }          
            for (ii=0; ii<ucPar; ii++)
            {
                mcSHOW_USER_MSG(("Register 0x%03X = 0x%02X\n", u2RegAddr+ii, ucData[ii]));              
            }
        }
        else if (x_strcmp((CHAR *) aszArgv[0], "w") == 0) {
            if (i4Argc < 3)
                break;
            
            u2RegAddr = (U16)StrToInt(aszArgv[1]);
            ucPar = (U8)StrToInt(aszArgv[2]);
            if (L1_WriteBytes8Bits(pSpiderDemod, u2RegAddr, 1, &ucPar) == 0)
            {        
                mcSHOW_USER_MSG(("Spider Set Register 0x%03X Fail!\n", u2RegAddr));
                break;
            }          
            mcSHOW_USER_MSG(("Register 0x%03X = 0x%02X, OK!\n", u2RegAddr, ucPar));
        } 
        else if (x_strcmp((CHAR *) aszArgv[0], "s") == 0) {
            L1_Demod_get_text_status (pSpiderDemod, &txt);
        }
        else if (x_strcmp((CHAR *) aszArgv[0], "q") == 0) {
            switch(StrToInt(aszArgv[1]))
            {
            case 4:
                L1_DVB_C_set_constellation(pSpiderDemod, constellation_qpsk);
                break;
            case 16:
                L1_DVB_C_set_constellation(pSpiderDemod, constellation_qam16);
                break;
            case 32:
                L1_DVB_C_set_constellation(pSpiderDemod, constellation_qam32);
                break;
            case 64:
                L1_DVB_C_set_constellation(pSpiderDemod, constellation_qam64);
                break;
            case 128:
                L1_DVB_C_set_constellation(pSpiderDemod, constellation_qam128);
                break;
            case 256:
                L1_DVB_C_set_constellation(pSpiderDemod, constellation_qam256);
                break;
            default:
                L1_DVB_C_set_constellation(pSpiderDemod, constellation_qam64);
                break;
            }
        }


        break;
    case 2: // two-word cmd
        if (x_strcmp((CHAR *) aszArgv[0], "sb") == 0) {
            L1_DVB_C_set_symbol_rate  (pSpiderDemod, StrToInt(aszArgv[1])*1000);
        }
        else if (x_strcmp((CHAR *) aszArgv[0], "sr") == 0) {
            L1_Demod_reset(pSpiderDemod);
        }
        else if (x_strcmp((CHAR *) aszArgv[0], "pd") == 0) {
            ucPar = 0;
            L1_WriteBytes8Bits(pSpiderDemod, 0x104, 1, &ucPar);
            L1_WriteBytes8Bits(pSpiderDemod, 0x000, 1, &ucPar);
        }
        else if (x_strcmp((CHAR *) aszArgv[0], "pu") == 0) {
            ucPar = 0x21;
            L1_WriteBytes8Bits(pSpiderDemod, 0x000, 1, &ucPar);
            ucPar = 0x01;
            L1_WriteBytes8Bits(pSpiderDemod, 0x104, 1, &ucPar);
        }
        else if (x_strcmp((CHAR *) aszArgv[0], "if") == 0) {
            ucPar = (U8)StrToInt(aszArgv[1]);
            L1_WriteBytes8Bits(pSpiderDemod, 0x18B, 1, &ucPar);
        }
        else if (x_strcmp((CHAR *) aszArgv[0], "tb") == 0) {
            ucPar = (U8)StrToInt(aszArgv[1]);
            L1_WriteBytes8Bits(pSpiderDemod, 0x001, 1, &ucPar);
        }
        else if (x_strcmp((CHAR *) aszArgv[0], "te") == 0) {
            ucPar = (U8)StrToInt(aszArgv[1]);
            if (ucPar == 1) {
                mcSHOW_DBG_MSG(("[DVBC] SetTSPath : pass-through"));
                //Enable DVBC TS output
                ucPar = 0;
                L1_WriteBytes8Bits(pSpiderDemod, 0x4EF, 1, &ucPar);
                L1_WriteBytes8Bits(pSpiderDemod, 0x4F0, 1, &ucPar);
            }
            else if (ucPar == 0) {
                mcSHOW_DBG_MSG(("[DVBC] SetTSPath : tri-state"));
                //Set DVBC TS output to high-Z
                ucPar = 0xFF;
                L1_WriteBytes8Bits(pSpiderDemod, 0x4EF, 1, &ucPar);
                ucPar = 0x0F;
                L1_WriteBytes8Bits(pSpiderDemod, 0x4F0, 1, &ucPar);
            }

#ifdef CC_TURNKEY
            ucPar = 0;
            if (L1_WriteBytes8Bits(pSpiderDemod, 0x4f4, 1, &ucPar) == 0);
            if (L1_WriteBytes8Bits(pSpiderDemod, 0x4f5, 1, &ucPar) == 0);
            if (L1_WriteBytes8Bits(pSpiderDemod, 0x4f6, 1, &ucPar) == 0);
#endif 
        }
        else if (x_strcmp((CHAR *) aszArgv[0], "tm") == 0) {
            ucPar = (U8)StrToInt(aszArgv[1]);
            if (ucPar == 1) {
                mcSHOW_DBG_MSG(("[DVBC] SetTSInterface : parallel\n"));
                L1_Demod_SetTSInterface(pSpiderDemod, TRUE);
            }
            else if (ucPar == 0) 
            {
                mcSHOW_DBG_MSG(("[DVBC] SetTSInterface : serial\n"));
                L1_Demod_SetTSInterface(pSpiderDemod, FALSE);
            }

#ifdef CC_TURNKEY
            ucPar = 0;
            if (L1_WriteBytes8Bits(pSpiderDemod, 0x4f4, 1, &ucPar) == 0);
            if (L1_WriteBytes8Bits(pSpiderDemod, 0x4f5, 1, &ucPar) == 0);
            if (L1_WriteBytes8Bits(pSpiderDemod, 0x4f6, 1, &ucPar) == 0);
#endif 
        }
        break;

    case 3:
        if (x_strcmp((CHAR *) aszArgv[0], "cpd") == 0) {
            DVBC_ChipPowerDown(pSpiderDemod);
        }
        break;

    default:
        mcSHOW_USER_MSG(("Cmd: \n"));
        mcSHOW_USER_MSG(("\t t  [Freq (KHz)]         : set tuner frequency\n"));
        mcSHOW_USER_MSG(("\t r  [RegAddr] [Num]      : read  Demod register\n"));
        mcSHOW_USER_MSG(("\t w  [RegAddr] [Value]    : write Demod register\n")); 
        mcSHOW_USER_MSG(("\t s                       : status\n"));
        mcSHOW_USER_MSG(("\t q  [4/16/32/64/128/256] : set qam mode\n"));
        mcSHOW_USER_MSG(("\t sb [SymRate (KHz)]      : set symbol rate\n"));
        mcSHOW_USER_MSG(("\t sr                      : system reset\n"));
        mcSHOW_USER_MSG(("\t pd                      : demod power down\n"));
        mcSHOW_USER_MSG(("\t pu                      : demod power up\n"));
        mcSHOW_USER_MSG(("\t if [0(normal)/1(tri)]   : set ifacg\n"));
        mcSHOW_USER_MSG(("\t tb [0(non bypass)/1(bypass)]   : set i2c tuner bypass\n"));
        mcSHOW_USER_MSG(("\t cpd                     : chip power down\n"));
        mcSHOW_USER_MSG(("\t te [0(disable)/1(enable)]      : ts enable\n"));
        mcSHOW_USER_MSG(("\t tm [0(serial)/1(parallel)]     : ts mode\n"));
                

        mcSHOW_USER_MSG(("\r\n"));
        mcSHOW_USER_MSG(("\r\n"));
        break;
    }

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);
    mcSHOW_DBG_MSG4(("DVBC_TunerCommand unlock pPdCtx->hHalLock\n"));
}

void DVBC_TunerDVBCCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,INT32 i4Argc, const CHAR** aszArgv)
{
    UCHAR   ucCmdId = 0;
    CHAR    *txt;
    
    if (i4Argc < 1)
    {
        mcSHOW_USER_MSG(("Cmd: \n"));
        mcSHOW_USER_MSG(("\t s                       : status\n"));
    }	

    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);
   
    if (i4Argc > 0)
        ucCmdId = *((CHAR *) aszArgv[0]);
        
    switch (ucCmdId)
    {
    case 's': // one-word cmd
        L1_Demod_get_text_status (pSpiderDemod, &txt);
        break;
    default:
        mcSHOW_USER_MSG(("Cmd: \n"));
        mcSHOW_USER_MSG(("\t s                       : status\n"));
        break;
    }    

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);
}

void DVBC_TunerDetachI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bDetach)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBC_TunerDetachI2C()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);
}

//-------------------------------------------------------------------------
/**
 *  This API get attribute from tuner driver
 *  Inputs:
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_get_type         tuner get type, refer x_tuner.h TUNER_GET_TYPE_XXXXX
 *  Outputs:
 *  @param   *pzAttribute       Attribute structure to be filled. Refer x_tuner.h structure definition TUNER_XXXXX_ATTRIBUTE_T
 *  @param   *pzAttributeLen    length to be filled.
 *  @retval  DRVAPI_TUNER_OK    success.
 *  @retval  DRVAPI_TUNER_ERROR fail.
 */
//-------------------------------------------------------------------------
INT16 DVBC_TunerGetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_GET_TYPE_T  e_get_type,
                                VOID *pzAttribute, SIZE_T* pzAttributeLen)
{
    TUNER_DIG_ATTRIBUTE_T *pAttri;
    ITUNER_CTX_T *pTunerCtx;
    INT32 i4Temp = 0;
//    Si2165_Context *Si2165;

    pTunerCtx = ITunerGetCtx();
//    Si2165  = (Si2165_Context *) pSpiderDemod->specific;

    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);

    //add driver implementation code here
    switch (e_get_type)
    {
    case TUNER_GET_TYPE_BER:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_BER\r\n"));
        *pzAttributeLen = sizeof(INT32);
        i4Temp = L1_Demod_get_ber(pSpiderDemod);
        x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);
        break;
    case TUNER_GET_TYPE_DIG_ATTRIBUTE:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_DIG_ATTRIBUTE\r\n"));
        pAttri = (TUNER_DIG_ATTRIBUTE_T *) pzAttribute;
        //Tuner frequency boundary
        pAttri->ui4_lower_bound_freq = pTunerCtx->specific_member.eu_ctx.m_s4FreqBoundLower;
        pAttri->ui4_upper_bound_freq = pTunerCtx->specific_member.eu_ctx.m_s4FreqBoundUpper;
        // freq offset tolerence range in Hz (1M mapping to 100K)
//        pAttri->ui4_fine_tune_delta_freq = Si2165->symbol_rate_Hz/10;
        pAttri->ui4_fine_tune_delta_freq = 400000;
        break;
    case TUNER_GET_TYPE_RAW_SIGNAL_LEVEL:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_RAW_SIGNAL_LEVEL\r\n"));
        *pzAttributeLen = sizeof(S8);
        x_memcpy(pzAttribute, &(pTunerCtx->specific_member.eu_ctx.m_SigLvScan), *pzAttributeLen);
        break;

    case TUNER_GET_TYPE_BER_TOLERANCE:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_BER_TOLERANCE\r\n"));
        *pzAttributeLen = sizeof(INT32);
        i4Temp = 100;  //suppose no use in DVB-C
        x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);
        break;

    case TUNER_GET_TYPE_SIGNAL_LEVEL_TOLERANCE:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_SIGNAL_LEVEL_TOLERANCE\r\n"));
        *pzAttributeLen = sizeof(UINT8);
        x_memcpy(pzAttribute, &(pTunerCtx->specific_member.eu_ctx.m_SigLvTh), *pzAttributeLen);
        break;
    default:
        mcSHOW_DBG_MSG3(("unknown: e_get_type = %d\r\n", e_get_type));
        break;
    }

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);
    return DRVAPI_TUNER_OK;
}
//-------------------------------------------------------------------------
/**
 *  This API set attribute to tuner driver
 *  Inputs:
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_set_type         tuner set type, check x_tuner.h set operation TUNER_SET_TYPE_XXXX
 *  @param   *pvAttribute       set value.
 *  @param   zAttributeLen      length of *pvAttribute.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  DRVAPI_TUNER_OK    success.
 *  @retval  DRVAPI_TUNER_ERROR fail.
 */
//-------------------------------------------------------------------------
INT32 DVBC_TunerSetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_SET_TYPE_T  e_set_type,
                                        const VOID *pvAttribute, SIZE_T zAttributeLen)
{
    UCHAR ucData = *(UCHAR *)pvAttribute;

    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);
    //add driver implementation code here

    switch (e_set_type)
    {
    case TUNER_SET_TYPE_TS_TRISTATE:
        if (ucData == BOOL_FALSE) {
            mcSHOW_DBG_MSG(("[DVBC] SetTSPath : pass-through"));
            //Enable TS output
            ucData = 0;
            L1_WriteBytes8Bits(pSpiderDemod, 0x4EF, 1, &ucData);
            L1_WriteBytes8Bits(pSpiderDemod, 0x4F0, 1, &ucData);
        }
        else if (ucData == BOOL_TRUE) {
            mcSHOW_DBG_MSG(("[DVBC] SetTSPath : tri-state"));
            //Set TS output to high-Z
            ucData = 0xFF;
            L1_WriteBytes8Bits(pSpiderDemod, 0x4EF, 1, &ucData);
            ucData = 0x0F;
            L1_WriteBytes8Bits(pSpiderDemod, 0x4F0, 1, &ucData);
        }

#ifdef CC_TURNKEY
        ucData = 0;
        if (L1_WriteBytes8Bits(pSpiderDemod, 0x4f4, 1, &ucData) == 0);
        if (L1_WriteBytes8Bits(pSpiderDemod, 0x4f5, 1, &ucData) == 0);
        if (L1_WriteBytes8Bits(pSpiderDemod, 0x4f6, 1, &ucData) == 0);
#endif
        break;

    case TUNER_SET_TYPE_TS_SP:
        if (ucData == SP_PARALLEL) {
            mcSHOW_DBG_MSG(("[DVBC] SetTSInterface : parallel"));
            L1_Demod_SetTSInterface(pSpiderDemod, TRUE);
        }
        else if (ucData == SP_SERIAL) {
            mcSHOW_DBG_MSG(("[DVBC] SetTSInterface : serial"));
            L1_Demod_SetTSInterface(pSpiderDemod, FALSE);
        }

#ifdef CC_TURNKEY
        ucData = 0;
        if (L1_WriteBytes8Bits(pSpiderDemod, 0x4f4, 1, &ucData) == 0);
        if (L1_WriteBytes8Bits(pSpiderDemod, 0x4f5, 1, &ucData) == 0);
        if (L1_WriteBytes8Bits(pSpiderDemod, 0x4f6, 1, &ucData) == 0);
#endif 
        break;

    default:
        mcSHOW_USER_MSG(("unknown: set Attribute\r\n"));
        break;
    }

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);
    return DRVAPI_TUNER_OK;
}
#ifdef fcADD_DIAG_INFO
INT32 DVBC_TunerSetDiagCmd(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CUSTOM_TUNER_SET_TYPE_T e_tuner_set_type, VOID* pv_set_info, SIZE_T z_size)
{
    INT32   retSts = RMR_OK;
    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBC_TunerSetDiagCmd()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);

    return retSts;
}
INT32 DVBC_TunerGetDiagInfo(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CUSTOM_TUNER_GET_TYPE_T e_tuner_get_type, VOID* pv_get_info, SIZE_T* pz_size)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBC->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBC_TunerGetDiagInfo()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBC->hHalLock);
    return 0;
}
#endif
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
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDtdCommand 	= DVBC_TunerDVBCCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDetachI2C 	= DVBC_TunerDetachI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDisc 	= DVBC_TunerDisc;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetAttribute= DVBC_TunerSetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute= DVBC_TunerGetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerBypassI2C = DVBC_TunerBypassI2C;
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
    ITUNER_CTX_T *tTunerCtx;

    //create pdctx used in this file, you also can add any declaration you want in pdctx
    pPdCtxDVBC = (DVBC_PD_CTX_T *) x_mem_calloc(1, sizeof(DVBC_PD_CTX_T));
    if (pPdCtxDVBC == NULL)
    {
        mcSHOW_USER_MSG(("DVB_PD_CTX_T malloc fail\n"));
        return (DRVAPI_TUNER_ERROR);
    }

	/* Init Disconnect Status */
	pPdCtxDVBC->fgDisStatus = FALSE;

    //Create a semaphore for low level demod PI access protection
    if (x_sema_create(&pPdCtxDVBC->hHalLock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_USER_MSG(("DVB x_sema_create(pPdCtxDVBC->hHalLock) Fail!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    x_sema_unlock(pPdCtxDVBC->hHalLock);

    //register LTDIS APIs to HAL
    PD_DVBC_Register_LTDIS_Fct(ptTDCtx);

    /* demod instance initialization (to do only once) */
    if (L1_Demod_SW_Init(&pSpiderDemod, DEMOD_ADDRESS, standard_dvb_c) == 0)
    {
        mcSHOW_USER_MSG(("L1_Demod_SW_Init() Fail!\n"));
        return (DRVAPI_TUNER_ERROR);
    } 

    //DVB-C demod HW init
    DVBC_ChipPowerOnReset();

    /* tuner initialization */
#if 0	
#if (fcTUNER_TYPE != cDVBT_2K10_TUNERLST) //Menghau,20090921,add for TunerAll structure
    ITunerRegistration(cITUNER_DONT_CARE);
#else
    tunerType = checkTunerTypeFromEeprom();
    ITunerRegistration(tunerType);
    mcSHOW_USER_MSG(("Register DVBCT Tunerall type = %d\n",tunerType));		
#endif
#else    // liuqu changed,20091215 for test!!!
     ITunerRegistration(((TUNER_DEVICE_CTX_T*)ptTDCtx)->tunerType);
     mcSHOW_USER_MSG(("[LEO]DVBC GET tuner type = %d\n",((TUNER_DEVICE_CTX_T*)ptTDCtx)->tunerType));
#endif

    L1_WRITE(pSpiderDemod, i2c_passthru      , i2c_passthru_enabled);
    ITuner_Init(ITunerGetCtx());
    L1_WRITE(pSpiderDemod, i2c_passthru      , i2c_passthru_disabled);

    //Set IF frequency from tuner CTX
    tTunerCtx = ITunerGetCtx();
    pSpiderDemod->i4IfFreqHz = tTunerCtx->u2IF_Freq * 1000;

    if (L1_Demod_init_after_reset(pSpiderDemod) == 0)
    {
        mcSHOW_USER_MSG(("L1_Demod_init_after_reset() Fail!\n"));
        return (DRVAPI_TUNER_ERROR);
    }

    //Disconnect demod
    DVBC_ChipPowerDown(pSpiderDemod);


#ifdef DVBC_ENABLE_MONITOR
    if (mcSEMA_CREATE(&pPdCtxDVBC->t_escape_mon_suspend, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_DRVERR_MSG(("EN_DVBC_TunerOpen (mcSEMA_CREATE): ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }

    pPdCtxDVBC->fgBreakMonitor = TRUE;//init
    pPdCtxDVBC->fgPIMoniStatus = FALSE; //init
    pPdCtxDVBC->t_ScanMode = CONN_MODE_UNKNOWN;  //init
//    PD_StartMonitorEngine(pPdCtxDVBC);  //start monitor engine
#endif 

    return (DRVAPI_TUNER_OK);
}



