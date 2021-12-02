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
//#include "tuner_if.h"
//#include "pd_tuner_type.h"
//#include "tuner_interface_if.h"
//#include "queue.h"
#include "TDIS.h"
#include "TDMgr.h"
//#include "pd_dvbc_if.h"
//#include "Emb_L1_Driver_Core.h"
//#include "pi_demod.h"
#include "drvapi_tuner.h"
#include "UtilAPI.h"
#include "pod_if.h"
#include "drvcust_if.h"
#include "eeprom_if.h"
#include "pd_glue_dvbs.h"
#include "nim_s3501.h"
#include "pd_dvbs_if.h"
#include "PD_Def.h"
#include "Porting.h"
#include "x_gpio.h"


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define cAcqTimeOutCnt   20

#define DVBS_TUNER_CONN_TIME_THREHOLD   140
#define DVBS_DEMOD_CONN_TIME_THREHOLD   200
#define DVBS_ENABLE_MONITOR






//-----------------------------------------------------------------------------
// Configurations
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
static DVBS_PD_CTX_T *pPdCtxDVBS;
//static L1_Context *pSpiderDemod;      //????

static struct QPSK_TUNER_CONFIG_API	Tuner_API;//={{0},0,0,0,{0},{0},{0}};
static HANDLE_T AutoScan_callback;
static TUNER_SAT_DIG_TUNE_INFO_T tGET_tuner_info={0};
static struct nim_device* dev;

static struct DRV_COMP_ID_T* pArg=0;

static UINT32 ui4_data_forMW=0;
static TUNER_MODULATION_T E_POL=0;

//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------
UINT8 do_blindscan = 0,cli_blindscan;
INT32 scan_flag=FALSE;
UINT32 lastfreq = 0,clisfreq,cliefreq;


//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
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
	tGET_tuner_info.e_fec_inner = (TUNER_FEC_INNER_T)fec;
	tGET_tuner_info.e_pol = (TUNER_POLARIZATION_T)polar;
    tGET_tuner_info.ui4_freq = freq;
	tGET_tuner_info.ui4_sym_rate = sym;

	
	
    if (signal==1)	
	{
	
	UINT8 data = 0;
       nim_reg_read(dev,RAF_TSOUT_PAD,&data,1);                        
	data = data | 0x04;    //driving 8mA
	nim_reg_write(dev,RAF_TSOUT_PAD, &data, 1);
	nim_reg_read(dev,RB1_TSOUT_SMT,&data,1);
	data = data | 0x04;    //driving 8mA
	nim_reg_write(dev,RB1_TSOUT_SMT, &data, 1);
	
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
    pPdCtxDVBS->fgScanCallbackLocked = TRUE;
    mcSEMA_LOCK_MTFE(AutoScan_callback);
	pPdCtxDVBS->fgScanCallbackLocked = FALSE;
	return 0;
}

#ifdef DVBS_ENABLE_MONITOR
STATIC BOOL SetBreakMonitor(DVBS_PD_CTX_T *psPdCtx)
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
STATIC void ResumeMonitor(DVBS_PD_CTX_T *psPdCtx)
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
STATIC INT32 i4DVBSMonitorBreak(VOID *pvArg)                                                            
{                                                                                                       
    DVBS_PD_CTX_T *psPdCtx = (DVBS_PD_CTX_T *) pvArg;
                                                                                                        
    if (psPdCtx->fgBreakMonitor)
    {                                                                                                   
        mcSHOW_DBG_MSG(("DVBS_Monitor break function triggered!!\n"));                                      
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
    
    DVBS_PD_CTX_T *psPdCtx = *(DVBS_PD_CTX_T **) pvArg;
    
    mcSHOW_DBG_MSG(("MonitorProcess start!\n"));
        
    while (psPdCtx->fgMonitorThreadRunning)
    {
    	if (psPdCtx->fgBreakMonitor)
    	{
    		// After mcMUTEX_LOCK(t_escape_mon_suspend), monitor thread will be suspended until
    		// TunerAcq or TunerClose invoked.
            mcSHOW_DBG_MSG(("Entering DVBS mon_suspend\n"));
    		mcMUTEX_LOCK(psPdCtx->t_escape_mon_suspend);
    		mcSHOW_DBG_MSG(("Escape DVBS mon_suspend\n"));
    	}
    	else
    	{
		    // mutex lock for I2C access
            mcSEMA_LOCK_MTFE(psPdCtx->hHalLock);
	        mcSHOW_DBG_MSG2(("Invoke DVBS_Monitor\n"));
            //add PI monitor API here 
		    psPdCtx->fgPIMoniStatus = TRUE; //doing PI monitor

            //! put your monitor process code here
            /////
            
			mcSEMA_UNLOCK_MTFE(psPdCtx->hHalLock);

			if(do_blindscan == 1)
			{
				struct NIM_Auto_Scan as_para;
			    //as_para = 0;
				// x_mem_alloc(as_para);
				if(cli_blindscan==1)
				{
					mcSHOW_DRVAPI_MSG(("cli debug BLIND SCAN\n"));
					as_para.sfreq = clisfreq;
					as_para.efreq = cliefreq;
				}
				else
				{
				mcSHOW_DRVAPI_MSG(("e_conn_mode:CONN_MODE_SCAN\n"));
				as_para.sfreq = 950;
				as_para.efreq = 2200;
				}
				as_para.unicable=0;
				as_para.Fub=0;
				as_para.callback = DVBS_AutoScanCallback;
				scan_flag = nim_ioctl_ext(dev, NIM_DRIVER_AUTO_SCAN, &as_para);  
	        	mcSHOW_USER_MSG(("End blindscan\n"));
				do_blindscan = 0;
				cli_blindscan = 0;
			}
            /////         
            psPdCtx->fgPIMoniStatus = FALSE; //finish doing PI monitor or breaked                        
	        mcDELAY_MS(500); 
        }
    }
    
   // psPdCtx->t_monitor_thread = NULL;
    psPdCtx->t_monitor_thread = 0;
    mcSHOW_DBG_MSG(("MonitorProcess exit!\n"));
    mcTHREAD_DESTROY();
}

/************************************************************************
*  PD_StartMonitorEngine
*  Function which starts monitor thread engine.
*  @param  *psAtvPdCtx : A pointer to ATV_PD_CTX_T structure.
*  @retval  DRVAPI_TUNER_OK : Monitor thread create successfully.
*  @retval  DRVAPI_TUNER_ERROR : Monitor thread create fail.
************************************************************************/
STATIC S32 PD_StartMonitorEngine(DVBS_PD_CTX_T *psPdCtx)
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
//    pPdCtxDVBS->fgMonitorThreadRunning = FALSE; //need set to false before mcMUTEX_UNLOCK(t_escape_mon_suspend);
    while (pPdCtxDVBS->t_monitor_thread){
        mcDELAY_MS(10); // in ms
    }
    mcSHOW_DBG_MSG(("DVBS PD_StopMonitorEngine success\n"));
    return (DRVAPI_TUNER_OK);
}
#endif //DVBS_ENABLE_MONITOR
/*
STATIC INT8 Tuner_Connect(U32 Freq, U8 sawbw, U8 AutoSearch)
{
    PARAM_SETFREQ_T SetFreqPara;	
    INT8 fgRet = 0;

    ITuner_OP(ITunerGetCtx(), itSetSawBw, sawbw, NULL);
   
    SetFreqPara.Freq = Freq;
    SetFreqPara.Modulation = MOD_DVBS;
    SetFreqPara.fgAutoSearch = AutoSearch;   
    if((ITuner_SetFreq(ITunerGetCtx(), &SetFreqPara)) != 0) {
        mcSHOW_USER_MSG(("Failed on Tuner_SetFreq(%d)", Freq));
        fgRet = 1;
    }

    return fgRet;

}
*/
STATIC INT8 DVBS_DriverInit(struct QPSK_TUNER_CONFIG_API Tuner_API)
{
	//UINT8  data = 0;

	mcSHOW_USER_MSG(("begin DVBS_DriverInit()\n"));
	//GPIO_SetOut(53, 1);
	//mcDELAY_MS(500);
     GPIO_SetOut(729, 0); 
     mcDELAY_MS(500);
	 GPIO_SetOut(729, 1);
     mcDELAY_MS(500);
	//Tuner function point setting
		Tuner_API.nim_Tuner_Init = nim_hz6306_init;
		Tuner_API.nim_Tuner_Control = nim_hz6306_control;
		Tuner_API.nim_Tuner_Status	= nim_hz6306_status;
		//S3501 parameter setting
		Tuner_API.config_data.QPSK_Config=0xe9; 
		//Tuner_API.config_data.QPSK_Config=0xed;
		//Tuner_API.config_data.QPSK_Config |=0x200;//ssi
		Tuner_API.config_data.Recv_Freq_Low = 900;
		Tuner_API.config_data.Recv_Freq_High = 2200;
		//tuner I2C parameter setting
		Tuner_API.tuner_config.i2c_type_id = I2C_FOR_NIM;
		Tuner_API.tuner_config.cTuner_Base_Addr =0xC0;
		//S3501 I2C parameter setting
		Tuner_API.ext_dm_config.i2c_type_id = I2C_FOR_NIM;	
		Tuner_API.ext_dm_config.i2c_base_addr = 0x66;			
		nim_s3501_attach(&Tuner_API);
		
	dev=(struct nim_device*)dev_get_by_id(HLD_DEV_TYPE_NIM,0x00);
	nim_open(dev);
	
	//nim_s3501_attach(&Tuner_API);
    //   mcSHOW_DBG_MSG(("[DVBS] After initialize SetTSPath : tri-state"));
        //Set DVBS TS output to high-Z
		// close ts
	//	nim_reg_read(dev,RAF_TSOUT_PAD,&data,1);
	//	data = data | 0x10;    
	//	nim_reg_write(dev,RAF_TSOUT_PAD, &data, 1);
	//	mcSHOW_DBG_MSG(("[DVBS]	Set TS output off  \n"));
	
	
	mcSHOW_USER_MSG(("success on DVBS_DriverInit()\n"));
  return 0;
}
//-------------------------------------------------------------------------
/**
 *  This API do connect tuner
 *  @retval  void
 */
//-------------------------------------------------------------------------
VOID  DVBS_TunerConnect(void)
{
UINT8	data;

	//struct nim_device * dev;
    //dev=(nim_device*)dev_get_by_id(HLD_DEV_TYPE_NIM,0x00);
	
	if(pPdCtxDVBS->fgDisStatus == FALSE)
	{
		/* Has connected, return */
		mcSHOW_DBG_MSG(("DVBS Has connect!\n"));
		return;
	}
    //add driver implementation code here

	mcSHOW_DBG_MSG(("[DVBS] SetTSPath : pass-through"));
	//Enable DVBS TS output
	//open ts
	nim_reg_read(dev,RAF_TSOUT_PAD,&data,1);
	data = data & 0xEF;	  
	nim_reg_write(dev,RAF_TSOUT_PAD, &data, 1);
    mcSHOW_DBG_MSG(("[DVBS] Set TS output on  \n"));


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
INT32 DVBS_TunerClose(PTD_SPECIFIC_CTX ptTDSpecificCtx)     //???????????????????????????????????????????????????
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);
    
//#if !defined(CC_MTAL) && !defined(CC_SDAL)   

#ifdef DVBS_ENABLE_MONITOR
    pPdCtxDVBS->fgBreakMonitor = TRUE; //disable and breadk monitor thread
    pPdCtxDVBS->fgMonitorThreadRunning = FALSE; //need set to false before mcMUTEX_UNLOCK(t_escape_mon_suspend);
    mcMUTEX_UNLOCK(pPdCtxDVBS->t_escape_mon_suspend);
    PD_StopMonitorEngine();  // stop monitor engine
    mcMUTEX_DESTROY(pPdCtxDVBS->t_escape_mon_suspend);  // destory monitor thread
#endif

    //If you have allocated any resource in TunerOpen(), release them here.


    //add driver implementation code here
	nim_close(dev);

    x_sema_delete(AutoScan_callback);
    x_sema_delete(pPdCtxDVBS->hHalLock);
    x_mem_free(pPdCtxDVBS);

    return (DRVAPI_TUNER_OK);
}

VOID DVBS_SetTSInterface(BOOL fgParallel)
{

	
    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);

    if (fgParallel) {
		Tuner_API.config_data.QPSK_Config &=0xDFF;
        mcSHOW_DBG_MSG(("[DVBS] SetTSInterface : parallel\n"));
    }
    else
    {
        Tuner_API.config_data.QPSK_Config |=0x200;
        mcSHOW_DBG_MSG(("[DVBS] SetTSInterface : serial\n"));
    }


    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);
}

void DVBS_SetTSPath(BOOL fgEnable)                   
{
   UINT8  data = 0;

    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);

    if (fgEnable) {
        mcSHOW_DBG_MSG(("[DVBS] SetTSPath : pass-through"));
        //Enable DVBS TS output
		//open ts
		nim_reg_read(dev,RAF_TSOUT_PAD,&data,1);
		data = data & 0xEF;    
		nim_reg_write(dev,RAF_TSOUT_PAD, &data, 1);
		mcSHOW_DBG_MSG(("[DVBS]	Set TS output on  \n"));
    }
    else {
        mcSHOW_DBG_MSG(("[DVBS] SetTSPath : tri-state"));
        //Set DVBS TS output to high-Z
		// close ts
		nim_reg_read(dev,RAF_TSOUT_PAD,&data,1);
		data = data | 0x10;    
		nim_reg_write(dev,RAF_TSOUT_PAD, &data, 1);
		mcSHOW_DBG_MSG(("[DVBS]	Set TS output off  \n"));

    }

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);
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
BOOL DVBS_TunerAcq(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type, void* pv_conn_info,
                            SIZE_T z_conn_info_len, x_break_fct _BreakFct, void* pvArg)
{
    TUNER_SAT_DIG_TUNE_INFO_T *pt_tuner_info = 
			    	(TUNER_SAT_DIG_TUNE_INFO_T *) pv_conn_info;   
    UINT32  freq =0;
    UINT32  symbolRate = 0;
    UINT32  ii;
    UINT8   LockStatus = 0;
    struct NIM_Channel_Change pam;
    
   // UINT8 ScanMode;
   //freq_bound= 0;
	 //UINT8   fgChannelScan = FALSE;
   // UINT32  mode;//Fec;
   // UINT32  u4TickCnt, u4ConnTime;
     
    pArg=pvArg;
	
    //Set break function
    pPdCtxDVBS->isBreak = _BreakFct;

    //Set DVB-S scan mode  
   // pPdCtxDVBS->t_ScanMode = pt_tuner_info->e_conn_mode;

    //MUTEX lock
    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);

#ifdef DVBS_ENABLE_MONITOR
    SetBreakMonitor(pPdCtxDVBS);
#endif
/*
    //Set channel scan indicator
    if(pt_tuner_info->e_mod == MOD_UNKNOWN){ 
        fgChannelScan = TRUE;
    }
*/
   // u4TickCnt = os_get_sys_tick();

    //Driver init
    /*
    if (DVBS_DriverInit(Tuner_API) == 1){
        mcSHOW_USER_MSG(("Failed on DVBS_DriverInit()\n"));
    }
    else
    {
        mcSHOW_DRVAPI_MSG(("Success on DVBS_DriverInit()\n"));
    }
*/
    //Convert freq
    freq = pt_tuner_info->ui4_freq;
	lastfreq = freq;
	//freq_bound = pt_tuner_info->ui4_freq_bound;
	

    //Convert symbol rate
    symbolRate = pt_tuner_info->ui4_sym_rate;
  //  Fec = pt_tuner_info->e_fec_inner;
    //Convert input modulation                                      
    //s3501 detect modulation  by auto 

        
   // mcSHOW_DRVAPI_MSG(("Freq = %d,Freq_bound = %d\n", freq,freq_bound));
    mcSHOW_DRVAPI_MSG(("Freq = %d,symbol rate = %d\n",freq,symbolRate));
    DVBS_TunerConnect();

    //get connection mode
    switch(pt_tuner_info->e_conn_mode)
    {
     case CONN_MODE_SCAN:
	 	 {
		 	//Tuner_Notify(DRV_COND_CONNECTED,TUNER_COND_STATUS_LOST_SIGNAL);

		 	nim_io_control(dev, NIM_DRIVER_STOP_ATUOSCAN, 0);
		 	do_blindscan = 1;
			
		 /*	struct NIM_Auto_Scan as_para;
			 //as_para = 0;
			 // x_mem_alloc(as_para);
			 mcSHOW_DRVAPI_MSG(("e_conn_mode:CONN_MODE_SCAN\n"));
			 as_para.sfreq = 950;
			 as_para.efreq = 2200;
			 as_para.unicable=0;
			 as_para.Fub=0;
			 as_para.callback = DVBS_AutoScanCallback;
			 scan_flag = nim_ioctl_ext(dev, NIM_DRIVER_AUTO_SCAN, &as_para);  */

	      }          
		break;
	 case CONN_MODE_UNKNOWN:                       //Channel change
	 case CONN_MODE_SURF:
              			  
			  pam.freq=freq;
			  pam.sym=symbolRate;
			  pam.fec=0;                         
			  nim_ioctl_ext(dev, NIM_DRIVER_CHANNEL_CHANGE, (void *)( &pam));
			  mcSHOW_DRVAPI_MSG(("TunerAcq TP SCAN channel freq = %d,symbolrate = %d \n", freq,symbolRate));
		break;
	default:
		 mcSHOW_DRVAPI_MSG(("Invalid connect mode(%d) to DVBS_TunerAcq!\n", pt_tuner_info->e_conn_mode));
         break;
	}
    

	if(pt_tuner_info->e_conn_mode == CONN_MODE_SCAN)

		LockStatus = 1;

	else
   { //Polling lock status
    for (ii = 0; ii < cAcqTimeOutCnt; ii ++)
    {

		nim_get_lock(dev,&LockStatus);
        mcSHOW_DBG_MSG3(("Demod lock status = %d\n", LockStatus));
		
        
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
    else {
        mcSHOW_DRVAPI_MSG(("MW break\n"));
    }
  }
#ifdef DVBS_ENABLE_MONITOR
    if (!pPdCtxDVBS->isBreak(pvArg))// && (u1ConnMode == ATV_CONN_MODE_CHAN_CHANGE))
    {
        //psAtvPdCtx->fgBreakMonitor = FALSE;           // Enable monitor thread and Disable break function flag
        //mcMUTEX_UNLOCK(t_escape_anaeu_mon_suspend);  // Unlock monitor thread
        ResumeMonitor(pPdCtxDVBS);   // Enable monitor thread and Disable break function flag,  Unlock monitor thread
        mcSHOW_DBG_MSG(("Enable monitor process!!\n"));
    }
#endif

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);

	/* Init Disconnect Status */
	pPdCtxDVBS->fgDisStatus = FALSE;
	
	if(LockStatus)
	ui4_data_forMW = pt_tuner_info->ui4_data;

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
INT16 DVBS_TunerGetSync(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT16 u2Ret=0;
    UINT8 LockStatus = 0;
	if(do_blindscan ==0)
		{
    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);
    //add driver implementation code here
	nim_get_lock(dev,&LockStatus);
	mcSHOW_DBG_MSG3(("Demod lock status = %d\n", LockStatus));
    u2Ret = (UINT16)LockStatus;
    
    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);
		}
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
VOID  DVBS_TunerDisc(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
UINT8	data;

	//struct nim_device * dev;
    //dev=(nim_device*)dev_get_by_id(HLD_DEV_TYPE_NIM,0x00);
	
	if(pPdCtxDVBS->fgDisStatus == TRUE)
	{
		/* Has Disconnected, return */
		mcSHOW_DBG_MSG(("DVBS Has Disconnect!\n"));
		return;
	}
    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);
    //add driver implementation code here

	//stop blindscan
	//if(do_blindscan == 1)
  // {       
		 	nim_io_control(dev, NIM_DRIVER_STOP_ATUOSCAN, 1);
	//}

	if(pPdCtxDVBS->fgScanCallbackLocked == TRUE)
	{pPdCtxDVBS->fgScanCallbackLocked = FALSE;
			x_sema_unlock(AutoScan_callback);}


	//set TS to tri-state
	mcSHOW_DBG_MSG(("[DVBS] SetTSPath : tri-state\n"));
    //Set DVBS TS output to high-Z, close ts
	nim_reg_read(dev,RAF_TSOUT_PAD,&data,1);
	data = data | 0x10;    
	nim_reg_write(dev,RAF_TSOUT_PAD, &data, 1);
	mcSHOW_DBG_MSG(("[DVBS]	Set TS output off  \n"));

	//power down demod
   /* if (nim_close(dev) == 1){
        mcSHOW_USER_MSG(("Failed on DVBS_ChipPowerDown()\n"));
    }*/

#ifdef DVBS_ENABLE_MONITOR
    SetBreakMonitor(pPdCtxDVBS);
#endif

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);

	/* set disconnect status */
	pPdCtxDVBS->fgDisStatus = TRUE;
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
void DVBS_TunerGetSignal(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type, SIGNAL *_pSignal)
{
    
    //add driver implementation code here
    // for testing, need to modify
//    struct nim_device * dev;
  //  dev=(nim_device*)dev_get_by_id(HLD_DEV_TYPE_NIM,0x00);
	UINT32 freq,sym;
//	UINT8 map_type;
	mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);
   // ITUNER_CTX_T *pTunerCtx;                          ///?????need??????????????
    if (do_blindscan)
    {
        _pSignal->Frequency = tGET_tuner_info.ui4_freq;
        _pSignal->SymbolRate = tGET_tuner_info.ui4_sym_rate;
    }
    else
    {
    	 freq = lastfreq;
         nim_get_freq(dev, &freq);
        _pSignal->Frequency  = (S32)freq;
    	
    	nim_get_sym(dev,&sym);
        _pSignal->SymbolRate = (U32)sym;
    }

/*
	nim_get_map_type(dev,&map_type);

   switch (map_type)                     //????
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
  */  
    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);

}

//-------------------------------------------------------------------------
/**
 *  This API get signal level in terms of percentage.
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  0~100              signal level
 */
//-------------------------------------------------------------------------
UINT8 DVBS_TunerGetSignalLevel(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT8 agc=0,u1Ret=0;
    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);
    //add driver implementation code here
	if(!(nim_get_AGC(dev, &agc)));
    mcSHOW_DBG_MSG3(("DVBS_TunerGetSignalLevel get fail!!\n"));
	
	u1Ret = agc;    //value mapping???????? 
    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);
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
INT16 DVBS_TunerGetSignalLeveldBm(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    INT16 i2Ret=0;
    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);
    //add driver implementation code here
    mcSHOW_DRVERR_MSG(("No implementation in DVBS_TunerGetSignalLeveldBm()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);
    return i2Ret;
}

UINT16 DVBS_TunerGetSignalPER(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{

    UINT32 u4_ret=0;
    UINT32 PER =0 ;
    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);
	//add your implementation here

	nim_io_control(dev, NIM_DRIVER_READ_RSUB, (UINT32)&PER);
    mcSHOW_DRVERR_MSG(("No implementation in DVBS_TunerGetSignalPER()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);

    u4_ret  = PER;
    return u4_ret;//modify return value

}

UINT32 DVBS_TunerGetSignalBER(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT32 u4_ret=0;
    UINT32 BER =0 ;

    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);
    //driver implementation 
	if(!(nim_get_BER(dev, &BER)));
    mcSHOW_DBG_MSG(("DVBS_TunerGetSignalBER get fail!!\n"));

    u4_ret  = BER;
	
    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);	
    return u4_ret;//modify return value

}

UINT16 DVBS_TunerGetSignalSNR(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT16 u2_ret = 0;
	UINT8 SNR = 0;
    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);

	//driver implementation 
	if (!(nim_get_SNR(dev, &SNR)));
	mcSHOW_DBG_MSG(("DVBS_TunerGetSignalSNR() get fail!!\n"));
    u2_ret = (UINT16)SNR;

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);
    return u2_ret;//modify return value
}

void DVBS_TunerSetMpgFmt(PTD_SPECIFIC_CTX ptTDSpecificCtx, MPEG_FMT_T  *pt_mpeg_fmt)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBS_TunerSetMpgFmt()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);
}

char *DVBS_TunerGetVer(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBS_TunerGetVer()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);
    return NULL;
}

INT32 DVBS_TunerNimTest(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    INT32 i4Ret = 0;
    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);

	//add your implementation here
   // L1_WRITE(pSpiderDemod, i2c_passthru      , i2c_passthru_enabled);
  //  i4Ret = Tuner_Connect(666000, SAW_BW_8M, FALSE);
   // L1_WRITE(pSpiderDemod, i2c_passthru      , i2c_passthru_disabled);

   // mcSHOW_DRVERR_MSG(("DVBS_TunerNimTest : %d (0:OK, 1:fail)\n", i4Ret));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);
    return i4Ret;
}
void DVBS_TunerSetRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx, UCHAR ucRegSet, UCHAR ucRegAddr, UCHAR ucRegValue)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBS_TunerSetRegSetting()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);
}

void DVBS_TunerShowRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBS_TunerShowRegSetting()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);
}

void DVBS_TunerTestI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBS_TunerTestI2C()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);
}
void DVBS_TunerDVBSCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx, INT32 i4Argc, const CHAR** aszArgv)
{
    UCHAR   ucArg1;
    UCHAR   ucCmdSize = 0;
    UINT8   data,i,j,demod_addr,lenth;
	UINT32	mode;
	UINT8	diseqc_cmd[4];
	UINT8	cmd_len;
	UINT8	polar = 0;
    //UINT8 ucData[256];
    //UCHAR   ucPar;
    //U16    ii, u2RegAddr;
    //CHAR * txt;
    //txt   = (CHAR*) x_mem_calloc(1, sizeof(CHAR)*1000);
	UINT32 symbrate,freq=0;
	 UINT32 ber = 0,per = 0;
	 UINT8 LockStatus,agc,SNR;
	  
	mcSHOW_DBG_MSG4(("DVBS_TunerCommand pPdCtx->hHalLock\n"));
    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);
	mcSHOW_DBG_MSG4(("escape DVBS_TunerCommand pPdCtx->hHalLock\n"));
    	    
    if (i4Argc > 0) {
        //ucCmdId = *((CHAR *) aszArgv[0]);
        ucCmdSize = x_strlen((CHAR *) aszArgv[0]);
    }

    switch (ucCmdSize) {
    case 10: 
        if (x_strcmp((CHAR *) aszArgv[0], "SetChannel") == 0) 
		{
			struct NIM_Channel_Change pam;
			mcSHOW_DBG_MSG4(("DVBS_TunerCommand t\n"));
		
      freq = StrToInt(aszArgv[1]);
			lastfreq = freq;
			symbrate = StrToInt(aszArgv[2]);    			  
			  pam.freq=freq;
			  pam.sym=symbrate;
			  pam.fec=0;                         
			  ucArg1 = nim_ioctl_ext(dev, NIM_DRIVER_CHANNEL_CHANGE, (void *)( &pam));
			  mcSHOW_DRVAPI_MSG(("nim_ioctl_ext(dev, NIM_DRIVER_CHANNEL_CHANGE, (void *)( &pam))--return = %d \n", nim_ioctl_ext(dev, NIM_DRIVER_CHANNEL_CHANGE, (void *)( &pam))));
			  if (ucArg1 != 0)
            {        
                mcSHOW_USER_MSG(("Set channel(freq = %d,symbolrate = %d) Fail!\n", freq,symbrate));  
                break;
            }
			  mcSHOW_DRVAPI_MSG(("TunerAcq channel change freq = %d,symbolrate = %d \n", freq,symbrate));
        	}

			  if (x_strcmp((CHAR *) aszArgv[0], "setchannel") == 0) 
		{
			struct NIM_Channel_Change pam;
			mcSHOW_DBG_MSG4(("DVBS_TunerCommand t\n"));
		
      freq = StrToInt(aszArgv[1]);
			lastfreq = freq;
			symbrate = StrToInt(aszArgv[2]); 			  
			  pam.freq=freq;
			  pam.sym=symbrate;
			  pam.fec=0;                         
			 // ucArg1 = nim_ioctl_ext(dev, NIM_DRIVER_CHANNEL_CHANGE, (void *)( &pam));
			 ucArg1 =nim_s3501_soft_search(dev,&symbrate,&freq,0);
			  	//UINT8 data = 0;
    			nim_reg_read(dev,RAF_TSOUT_PAD,&data,1);                        
				data = data | 0x04;    //driving 8mA
				nim_reg_write(dev,RAF_TSOUT_PAD, &data, 1);
				nim_reg_read(dev,RB1_TSOUT_SMT,&data,1);
				data = data | 0x04;    //driving 8mA
				nim_reg_write(dev,RB1_TSOUT_SMT, &data, 1);
			   mcSHOW_DRVAPI_MSG(("nim_ioctl_ext(dev, NIM_DRIVER_CHANNEL_CHANGE, (void *)( &pam))--return = %d \n", nim_ioctl_ext(dev, NIM_DRIVER_CHANNEL_CHANGE, (void *)( &pam))));
			  if (ucArg1 != 0)
            {        
                mcSHOW_USER_MSG(("Set channel(freq = %d,symbolrate = %d) Fail!\n", freq,symbrate));  
                break;
            }
			  mcSHOW_DRVAPI_MSG(("TunerAcq channel change freq = %d,symbolrate = %d \n", freq,symbrate));
        }
		break;
		
	case 1:
			if (x_strcmp((CHAR *) aszArgv[0], "s") == 0) 
			{
	    		freq=0;
				nim_get_freq(dev, &freq);
				mcSHOW_USER_MSG((" lock freq offset = %d\n", freq));
				nim_get_lock(dev,&LockStatus);
				mcSHOW_USER_MSG(("Demod lock status = %d\n", LockStatus));
       		 	nim_get_AGC(dev, &agc);
				mcSHOW_USER_MSG(("level gain = %d\n", agc));
        		nim_get_BER(dev, &ber);
				mcSHOW_USER_MSG(("Demod ber = %d\n",ber));
				nim_get_SNR(dev, &SNR);
				mcSHOW_USER_MSG(("Demod SNR = %d\n",SNR));
				nim_io_control(dev, NIM_DRIVER_READ_RSUB,(UINT32) &per);
				mcSHOW_USER_MSG(("Demod per = %d\n",per));

        	}
			if (x_strcmp((CHAR *) aszArgv[0], "w") == 0)
			{
				demod_addr = StrToInt(aszArgv[1]);
				data = (UINT8)StrToInt(aszArgv[3]);
				lenth = (UINT8)StrToInt(aszArgv[2]);
				for(i=0;i<=lenth;i++)
				{
					nim_reg_write(dev,demod_addr, &data, 1);
					mcSHOW_USER_MSG(("Write Demod reg[%x] = %x\n",demod_addr,data));
				}
			}
			if (x_strcmp((CHAR *) aszArgv[0], "r") == 0)
			{
				demod_addr = StrToInt(aszArgv[1]);
				//data = (UINT8)StrToInt(aszArgv[]);
				lenth = (UINT8)StrToInt(aszArgv[2]);
				for(i=0;i<=lenth;i++)
				{
					nim_reg_read(dev,demod_addr, &data, 1);
					mcSHOW_USER_MSG(("Read Demod reg[%x] = %x\n",demod_addr,data));
				}
			}
		break;

    case 4:
		if (x_strcmp((CHAR *) aszArgv[0], "Dint") == 0) 
        {
            DVBS_DriverInit(Tuner_API);
        }
		else if (x_strcmp((CHAR *) aszArgv[0], "Tint") == 0) 
        {
            
        }
        else if (x_strcmp((CHAR *) aszArgv[0], "Disc") == 0)
        {
             if(do_blindscan == 1)
             {       
                 nim_io_control(dev, NIM_DRIVER_STOP_ATUOSCAN, 1);
			
             }
        }
		break;

	case 2:
		if(x_strcmp((CHAR *) aszArgv[0], "ts") == 0) 
        {
        	if(x_strcmp((CHAR *) aszArgv[1], "p") == 0) 
        	{
        	    nim_reg_read(dev,RAF_TSOUT_PAD,&data,1);
				data = data & 0xEF;    
				nim_reg_write(dev,RAF_TSOUT_PAD, &data, 1);
				Tuner_API.config_data.QPSK_Config &=0xDFF;
				mcSHOW_USER_MSG(("[DVBS]	Set TS parallel output \n"));
        	}
			else if (x_strcmp((CHAR *) aszArgv[1], "s") == 0) 
			{
			    nim_reg_read(dev,RAF_TSOUT_PAD,&data,1);
				data = data & 0xEF;    
				nim_reg_write(dev,RAF_TSOUT_PAD, &data, 1);
				Tuner_API.config_data.QPSK_Config &=0x200;
				mcSHOW_USER_MSG(("[DVBS]	Set TS serial output \n"));
			}
			else if (x_strcmp((CHAR *) aszArgv[1], "o") == 0) 
			{
				nim_reg_read(dev,RAF_TSOUT_PAD,&data,1);
				data = data & 0xEF;    
				nim_reg_write(dev,RAF_TSOUT_PAD, &data, 1);
				mcSHOW_USER_MSG(("[DVBS]	open TS output   \n"));
			}
			else if (x_strcmp((CHAR *) aszArgv[1], "c") == 0) 
			{
				nim_reg_read(dev,RAF_TSOUT_PAD,&data,1);
				data = data | 0x10;    
				nim_reg_write(dev,RAF_TSOUT_PAD, &data, 1);
				mcSHOW_USER_MSG(("[DVBS]	close TS output  \n"));
			}
		}
		break;
	case 8:
		if (x_strcmp((CHAR *) aszArgv[0], "checkreg") == 0)
		{  
		   mcSHOW_USER_MSG(("demod regsiter value:\n"));
		   mcSHOW_USER_MSG((" \t"));
		   for(i=0;i<16;i++)
		   {
				mcSHOW_USER_MSG(("%X\t",i));
		   }
		   mcSHOW_USER_MSG(("\n"));
           for (i=0;i<14;i++) 
           {
           		mcSHOW_USER_MSG(("%X\t",i));
           		for(j=0;j<16;j++)
				{
					nim_reg_read(dev,((i<<4)+j),&data,1);
					mcSHOW_USER_MSG(("0x%X\t",data));
           		}
				mcSHOW_USER_MSG(("\n"));
           }
		}
		break;
	case 9:
		if (x_strcmp((CHAR *) aszArgv[0], "diseqc_op") == 0)
		{
			mcSHOW_USER_MSG(("DVBS diseqc_1x Command t\n"));
            mode = StrToInt(aszArgv[1]);
	
			cmd_len = (UINT8)StrToInt(aszArgv[3]);

			for(i=0;i<cmd_len;i++)
			{
				diseqc_cmd[i] = (UINT8)(StrToInt(aszArgv[2])>>(8*(cmd_len-i-1)));
				mcSHOW_USER_MSG(("%d\t",diseqc_cmd[i]));
			}
			mcSHOW_USER_MSG(("\n"));

			if(!(nim_DiSEqC_operate( dev, mode,diseqc_cmd,cmd_len)))
			{
				mcSHOW_USER_MSG(("DVBS diseqc_1x Command set success!\n mode = %d ;cmd = %d ;len = %d \n",mode,diseqc_cmd,cmd_len));
			}
			else
			{
				mcSHOW_USER_MSG(("DVBS diseqc_1x Command set fail!! t\n"));
			}
		}
		
		if (x_strcmp((CHAR *) aszArgv[0], "blindscan") == 0)
		{	
		    //if((StrToInt(aszArgv[1])>950)||(StrToInt(aszArgv[2])>2200)(StrToInt(aszArgv[1])>=StrToInt(aszArgv[2])))
			//	mcSHOW_DRVAPI_MSG(("cmd error!!! --please check freq--> (950<sFreq<eFreq<2200)&&(eFreq-sFreq >= 90)\n"));
			//else
		 	nim_io_control(dev, NIM_DRIVER_STOP_ATUOSCAN, 0);
			clisfreq = StrToInt(aszArgv[1]);
			cliefreq = StrToInt(aszArgv[2]);
			cli_blindscan = 1;
		 	do_blindscan = 1;
            
			#if 0
			{
				struct NIM_Auto_Scan as_para;
				mcSHOW_DRVAPI_MSG(("blindscan\n"));
				as_para.sfreq = StrToInt(aszArgv[1]);
				as_para.efreq = StrToInt(aszArgv[2]);
				as_para.unicable=0;
				as_para.Fub=0;
				as_para.callback = DVBS_AutoScanCallback;
				nim_ioctl_ext(dev, NIM_DRIVER_AUTO_SCAN, &as_para);
			}
            #endif
		}
	break;
	
	case 6:
		if (x_strcmp((CHAR *) aszArgv[0], "setH/V") == 0)
		{
			if(StrToInt(aszArgv[1]) == 0)
			   {
			    polar = 0X00;
			    mcSHOW_USER_MSG(("DVBS diseqc set HORIZONTAL(Xv)  t\n"));
			   }
			else if (StrToInt(aszArgv[1]) == 1)
			   {
			    polar = 0X01;
				mcSHOW_USER_MSG(("DVBS diseqc set VERTICAL(Xv)  t\n"));
			   }
			nim_set_polar(dev, polar);
		}
		/*
		if (x_strcmp((CHAR *) aszArgv[0], "iqswap") == 0)
            {
                mcSHOW_USER_MSG(("current IQ swap=%d \n",((Tuner_API.config_data.QPSK_Config)&&0x04)));
			 if(i4Argc > 1)
	          {
	           if(StrToInt(aszArgv[1]) ==0)
			   {
			    Tuner_API.config_data.QPSK_Config=(Tuner_API.config_data.QPSK_Config)&&0xffb;
			   }
			   else if (StrToInt(aszArgv[1]) == 1)
			   {
			    Tuner_API.config_data.QPSK_Config=(Tuner_API.config_data.QPSK_Config)||0x004;
			   }
			   mcSHOW_USER_MSG(("Set IQ swap to=%d \n",((Tuner_API.config_data.QPSK_Config)&&0x04)));
			  }
			 nim_reg_read(dev,R01_ADC,&data,1);
			 mcSHOW_USER_MSG(("3501 0xAF = %x\n",data));
			 }*/
	break;
	case 12:
		if (x_strcmp((CHAR *) aszArgv[0], "scancontinue") == 0)
	        {
                mcSHOW_USER_MSG(("current freq=%d, sym_rate=%d \n",
                    tGET_tuner_info.ui4_freq,
                    tGET_tuner_info.ui4_sym_rate));
            
	        	x_sema_unlock(AutoScan_callback);
		    	mcSHOW_USER_MSG(("DVBS blindscan contune!!! \n"));
			}
	break;
	case 3:
		if (x_strcmp((CHAR *) aszArgv[0], "cap") == 0)
	        {
	          freq = StrToInt(aszArgv[1]);
              mcSHOW_USER_MSG(("cap (FFT) spectrum from %d M to %d M \n",(freq-45),(freq+45)));
              nim_S3501_cap(dev,freq,0);
			}
		break;
				

		
					
    default:
        mcSHOW_USER_MSG(("Cmd: \n"));
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
		mcSHOW_USER_MSG(("\t iqswap  [0/1]:swap IQ signal\n"));
		mcSHOW_USER_MSG(("\t cap  [Freq]:captrue FFT resoult from Freq-45M ~ Freq+45M\n"));
        break;
    }

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);
    mcSHOW_DBG_MSG4(("DVBS_TunerCommand unlock pPdCtx->hHalLock\n"));
}

void DVBS_TunerCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,INT32 i4Argc, const CHAR** aszArgv)
{
 //   UCHAR   ucCmdId = 0;
  //  CHAR    *txt;
    #if 0
    if (i4Argc < 1)
    {
        mcSHOW_USER_MSG("Cmd: \n");
        mcSHOW_USER_MSG("\t s                       : status\n");
    }	
    #endif
	
    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);
   
    if (i4Argc > 0)
      //  ucCmdId = *((CHAR *) aszArgv[0]);

		#if 0
    switch (ucCmdId)
    {
    case 's': // one-word cmd
        L1_Demod_get_text_status (pSpiderDemod, &txt);
        break;
    default:
        mcSHOW_USER_MSG("Cmd: \n");
        mcSHOW_USER_MSG("\t s                       : status\n");
        break;
    }    
#endif
    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);
}

void DVBS_TunerDetachI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bDetach)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBS_TunerDetachI2C()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);
}

//----------------------------------------------------------------------------- 
/*
 *  DVBS_GetTsFmt
 *  Get demod TS output format.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   
 */
//-----------------------------------------------------------------------------
TS_FMT_T DVBS_GetTsFmt(void)
{
UINT8 ucData,ucData2;
TS_FMT_T fgTsFmt;


    nim_reg_read(dev,RD8_TS_OUT_SETTING, &ucData, 1);
//Parallel or Serial
	mcSHOW_DRVERR_MSG(("RD8_TS_OUT_SETTING= %d\n",ucData));
	if ((ucData & 0x21)==0x01)// && (ucData | 0xDF ==0))
		{fgTsFmt.SerOrPar = SP_SERIAL;
	mcSHOW_DRVERR_MSG(("TS FMT:serial\n"));}
    else
		{fgTsFmt.SerOrPar = SP_PARALLEL;
	mcSHOW_DRVERR_MSG(("TS FMT:parallel\n"));}
        
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

//TS clk = ???w
   nim_reg_read(dev,0XD8,&ucData,1);
if ((ucData & 0x11) == 0x10)                                  //TS CLK <50M.calculate by ssi mode
	{
	fgTsFmt.u4TsClockInKHz = 6750;         //TS CLK <50M.calculate by ssi mode;detial clk unknow,so total report 54M to dmx
	}
else   //TS CLK >50M.calculate by ssi mode
{
   //s3501 read reg 0xdf;0xce 
    nim_reg_read(dev,0XD8,&ucData,1);
	nim_reg_read(dev,0XCE,&ucData2,1);
    if ((ucData & 0x80)==0)
    {
    	if((ucData & 0x70)==0x00)
        /* Parallel */
        fgTsFmt.u4TsClockInKHz = 10375;
		else if((ucData & 0x70)==0x10)
        /* Parallel */
        fgTsFmt.u4TsClockInKHz = 12250;
		else if((ucData & 0x70)==0x20)
        /* Parallel */
        fgTsFmt.u4TsClockInKHz = 9625;
		else if((ucData & 0x70)==0x30)
        /* Parallel */
        fgTsFmt.u4TsClockInKHz = 8375;
		else
		/* Parallel */
        fgTsFmt.u4TsClockInKHz = 7500;
    }
    else
    {
        if((ucData2 & 0x0C)==0x00)
        /* Parallel */
        fgTsFmt.u4TsClockInKHz = 6750;
		else if((ucData2 & 0x0C)==0x04)
        /* Parallel */
        fgTsFmt.u4TsClockInKHz = 11250;
		else if((ucData2 & 0x0C)==0x08)
        /* Parallel */
        fgTsFmt.u4TsClockInKHz = 16875;
		else if((ucData2 & 0x0C)==0x0C)
        /* Parallel */
        fgTsFmt.u4TsClockInKHz = 9000;
    }
}
mcSHOW_USER_MSG(("u4TsClockInKHz = %d--------------------------\n",fgTsFmt.u4TsClockInKHz));

//external Demod or not
    fgTsFmt.fgExternalDemod = BOOL_TRUE;

    return fgTsFmt;
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
INT16 DVBS_TunerGetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_GET_TYPE_T  e_get_type,
                                VOID *pzAttribute, SIZE_T* pzAttributeLen)
{
    TUNER_SAT_DIG_TUNE_INFO_T *pAttri0;
	TUNER_DIG_ATTRIBUTE_T *pAttri1;
    //ITUNER_CTX_T *pTunerCtx;
    INT32 i4Temp = 0;
//    Si2165_Context *Si2165;
   pAttri0 = (TUNER_SAT_DIG_TUNE_INFO_T*)pzAttribute;

   // pTunerCtx = ITunerGetCtx();
//    Si2165  = (Si2165_Context *) pSpiderDemod->specific;

    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);

    //add driver implementation code here
    switch (e_get_type)
    {
    case TUNER_GET_TYPE_SIGNAL_LEVEL:
		{UINT8 agc=0;
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_SIGNAL_LEVEL\r\n"));
        *pzAttributeLen = sizeof(INT32);
        if(!(nim_get_AGC(dev, &agc)));
        mcSHOW_DBG_MSG3(("DVBS_TunerGetSignalLevel get fail!!\n"));	
	    i4Temp = (INT32)agc;		
		//i4Temp = (INT32)DVBS_TunerGetSignalLevel(ptTDSpecificCtx,TUNER_CONN_TYPE_SAT_DIG);
        x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);
      }
        break;
	case TUNER_GET_TYPE_SAT_DIG:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_SAT_DIG\r\n"));
        pAttri0->e_fec_inner = tGET_tuner_info.e_fec_inner ;
	    pAttri0->e_pol = tGET_tuner_info.e_pol ;
        pAttri0->ui4_freq = tGET_tuner_info.ui4_freq ;
	    pAttri0->ui4_sym_rate = tGET_tuner_info.ui4_sym_rate ;
	    pAttri0->ui4_data = ui4_data_forMW;
		pAttri0->e_pol = E_POL;
		break;
	case TUNER_GET_TYPE_BER:
      { 
	   	  UINT32 BER=0;
	   	  mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_BER\r\n"));
        *pzAttributeLen = sizeof(INT32);
		
		    if(!(nim_get_BER(dev, &BER)));
		    i4Temp = (INT32)BER;
        mcSHOW_DBG_MSG(("DVBS_TunerGetSignalBER get fail!!\n"));

        //i4Temp  =  BER;
        //i4Temp = (INT32)DVBS_TunerGetSignalBER( ptTDSpecificCtx,TUNER_CONN_TYPE_SAT_DIG);
        x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);}
        break;
    case TUNER_GET_TYPE_DIG_ATTRIBUTE:   	
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_DIG_ATTRIBUTE\r\n"));
        pAttri1 = (TUNER_DIG_ATTRIBUTE_T *) pzAttribute;
        //Tuner frequency boundary
        pAttri1->ui4_lower_bound_freq = 950;//950Hz
        pAttri1->ui4_upper_bound_freq = 2150;//2150M
        // freq offset tolerence range in Hz (1M mapping to 100K)
        pAttri1->ui4_fine_tune_delta_freq = 3;  //3Mhz
        break;
	case TUNER_GET_TYPE_TS_FMT:
        {
            TS_FMT_T tsFmt;
            *pzAttributeLen = sizeof(TS_FMT_T);
            tsFmt = DVBS_GetTsFmt();
            x_memcpy(pzAttribute, &tsFmt, *pzAttributeLen);
        }
        break;
   /* case TUNER_GET_TYPE_RAW_SIGNAL_LEVEL:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_RAW_SIGNAL_LEVEL\r\n"));
        *pzAttributeLen = sizeof(S8);
        x_memcpy(pzAttribute, &(pTunerCtx->specific_member.eu_ctx.m_SigLvScan), *pzAttributeLen);//?????
        break;

    case TUNER_GET_TYPE_BER_TOLERANCE:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_BER_TOLERANCE\r\n"));
        *pzAttributeLen = sizeof(INT32);
        i4Temp = 100;  //suppose no use in DVB-C    //DVBS need to change to what value??????????or it don't need this value????
        x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);
        break;

    case TUNER_GET_TYPE_SIGNAL_LEVEL_TOLERANCE:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_SIGNAL_LEVEL_TOLERANCE\r\n"));
        *pzAttributeLen = sizeof(UINT8);
        x_memcpy(pzAttribute, &(pTunerCtx->specific_member.eu_ctx.m_SigLvTh), *pzAttributeLen);
        break;*/
    default:
        mcSHOW_DBG_MSG3(("unknown: e_get_type = %d\r\n", e_get_type));
        break;
    }

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);
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
INT32 DVBS_TunerSetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_SET_TYPE_T  e_set_type,
                                        const VOID *pvAttribute, SIZE_T zAttributeLen)
{
   UINT32	mode;
   UINT8	data,i;
   UINT8  polar;
   UCHAR	ucData;
   DISEQC_BUS_CMD_T *diseqc_cmd;
   

    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);
    //add driver implementation code here

    switch (e_set_type)
    {
	case TUNER_SET_TYPE_SCAN_CONTINUE:
		pPdCtxDVBS->fgScanCallbackLocked = FALSE;
		x_sema_unlock(AutoScan_callback);
        break;
		
	case TUNER_SET_TYPE_DISEQC_CMD:
		{diseqc_cmd = (DISEQC_BUS_CMD_T*) pvAttribute;
		if(((diseqc_cmd->ui1_control_mask)&DISEQC_CMD_CTRL_POLARIZATION_MASK) !=0)
		{
			if(diseqc_cmd->ui1_polarity_13v_18v == 1)
			polar= 0;
		else
			polar=1;
		    E_POL = diseqc_cmd->ui1_polarity_13v_18v;
			nim_set_polar(dev, polar);
			mcDELAY_MS(20);
			mcSHOW_DBG_MSG(("After set H/V,delay 20ms,delay>15ms\n"));
    	}
		if(((diseqc_cmd->ui1_control_mask)&DISEQC_CMD_CTRL_FULL_CMD_MASK) !=0)
		{
			mode = 0x04;//NIM_DISEQC_MODE_BYTES		
			nim_DiSEqC_operate( dev, mode,diseqc_cmd->aui1_cmd,diseqc_cmd->ui1_cmd_len);
			mcDELAY_MS(20);
			mcSHOW_DBG_MSG(("After set cmd,delay 20ms,delay>15ms\n"));
			for(i=0;i<diseqc_cmd->ui1_cmd_len;i++)
			{
				mcSHOW_DBG_MSG2(("send cmd\n x%\t",diseqc_cmd->aui1_cmd[i]));
				mcSHOW_DBG_MSG2(("\n"));
			}
			nim_reg_read(dev,0x7c,&data,1);
			mcSHOW_DBG_MSG(("after set full cmd;0x7c = %x\n",data));
			for(i=0;i<50;i++)
			{nim_reg_read(dev,0x02,&data,1);
		    mcDELAY_MS(10);
		    if((data&0x80) == 0x80)
		    {
			mcSHOW_DBG_MSG(("full cmd set ok,total wait times = %d0ms\n",i));
            i=51;
			}}
			nim_reg_read(dev,0x7c,&data,1);
			data=data&0xf9;
			nim_reg_write(dev,0x7c, &data, 1);
			
    	}
		if(((diseqc_cmd->ui1_control_mask)&DISEQC_CMD_CTRL_TONE_BURST_MASK) !=0)
		{   
		
		    if(diseqc_cmd->b_tone_burst)
			mode = 0x03;//NIM_DISEQC_MODE_BURST1
			else
			{mode = 0x02;//NIM_DISEQC_MODE_BURST0
			data=0x00;
			nim_reg_write(dev,0x7e, &data, 1);}
			
			nim_DiSEqC_operate( dev, mode,diseqc_cmd->aui1_cmd,diseqc_cmd->ui1_cmd_len);
			mcDELAY_MS(20);
			mcSHOW_DBG_MSG(("After set burst,delay 20ms,delay>15ms\n"));

		for(i=0;i<50;i++)
			{nim_reg_read(dev,0x02,&data,1);
		    mcDELAY_MS(10);
		    if((data&0x80) == 0x80)
		    {
			mcSHOW_DBG_MSG(("burst set ok,total wait times = %d0ms\n",i));
            i=51;
			}}
			nim_reg_read(dev,0x7c,&data,1);
			data=data&0xf8;
			nim_reg_write(dev,0x7c, &data, 1);

    	}
		if(((diseqc_cmd->ui1_control_mask)&DISEQC_CMD_CTRL_22K_MASK) !=0)
		{   
		    if(diseqc_cmd->b_22k)
			mode = 0x01;//NIM_DISEQC_MODE_22KON
			else
			mode = 0x00;//NIM_DISEQC_MODE_22KOFF
			nim_DiSEqC_operate( dev, mode,diseqc_cmd->aui1_cmd,diseqc_cmd->ui1_cmd_len);
			mcDELAY_MS(20);
			mcSHOW_DBG_MSG(("After set 22k\n"));
    	}
/*
		for(i=0;i<500;i++)
			{nim_reg_read(dev,0x02,&data,1);
		    mcDELAY_MS(10);
		    if((data&&0x80) == 0x80)
		    {
			mcSHOW_DBG_MSG(("diseqc set ok,total wait times = %d0ms\n",i));
            i=501;
			}}
			nim_reg_read(dev,0x7f,&data,1);
			data=data&&0xf9;
			nim_reg_write(dev,0x7f, &data, 1);
			*/
		mcSHOW_DBG_MSG(("mask = %x;POLARIZATION_MASK= %x,FULL_CMD_MASK=%x,BURST_MASK=%x,22K_MASK=%x\n",diseqc_cmd->ui1_control_mask,DISEQC_CMD_CTRL_POLARIZATION_MASK,DISEQC_CMD_CTRL_FULL_CMD_MASK,DISEQC_CMD_CTRL_TONE_BURST_MASK,DISEQC_CMD_CTRL_22K_MASK));	
	}
		break;
		
	case TUNER_SET_TYPE_TS_TRISTATE:
		{
			ucData = *(UCHAR *)pvAttribute;
		if (ucData == BOOL_FALSE) 
		{
			mcSHOW_DBG_MSG(("[DVBS] SetTSPath : pass-through"));
        	//Enable DVBS TS output
			//open ts
			nim_reg_read(dev,RAF_TSOUT_PAD,&data,1);
			data = data & 0xEF;    
			nim_reg_write(dev,RAF_TSOUT_PAD, &data, 1);
			mcSHOW_DBG_MSG(("[DVBS]	Set TS output on  \n"));
		}
		else if (ucData == BOOL_TRUE) 
		{
			mcSHOW_DBG_MSG(("[DVBS] SetTSPath : tri-state"));
        	//Set DVBS TS output to high-Z
			// close ts
			nim_reg_read(dev,RAF_TSOUT_PAD,&data,1);
			data = data | 0x10;    
			nim_reg_write(dev,RAF_TSOUT_PAD, &data, 1);
			mcSHOW_DBG_MSG(("[DVBS]	Set TS output off  \n"));
		}
	}
	break;
		
	case TUNER_SET_TYPE_TS_SP:
		{ucData = *(UCHAR *)pvAttribute;
		if (ucData == SP_PARALLEL) 
		{
			Tuner_API.config_data.QPSK_Config &=0xDFF;
            mcSHOW_DBG_MSG(("[DVBS] SetTSInterface : parallel\n"));
        }

		else if (ucData == SP_SERIAL) 
		{
			Tuner_API.config_data.QPSK_Config |=0x200;
        	mcSHOW_DBG_MSG(("[DVBS] SetTSInterface : serial\n"));
		}
 }
	break;
	
    default:
        mcSHOW_USER_MSG(("unknown: set Attribute\r\n"));
        break;
    }

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);
    return DRVAPI_TUNER_OK;
}
#ifdef fcADD_DIAG_INFO
INT32 DVBS_TunerSetDiagCmd(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CUSTOM_TUNER_SET_TYPE_T e_tuner_set_type, VOID* pv_set_info, SIZE_T z_size)
{
    INT32   retSts = RMR_OK;
    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBS_TunerSetDiagCmd()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);

    return retSts;
}
INT32 DVBS_TunerGetDiagInfo(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CUSTOM_TUNER_GET_TYPE_T e_tuner_get_type, VOID* pv_get_info, SIZE_T* pz_size)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBS->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBS_TunerGetDiagInfo()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBS->hHalLock);
    return 0;
}
#endif

BOOL DVBS_TunerNotifyEnable(PTD_SPECIFIC_CTX ptTDSpecificCtx)

{
BOOL TunerNotifyEnable;
if(do_blindscan == 1)
	TunerNotifyEnable = FALSE;
else
	TunerNotifyEnable =TRUE;

return TunerNotifyEnable;

}

/***********************************************************************
TunerBreak;
************************************************************************/
	S32 DVBSBreak(void)
{
	return TunerBreak(pArg);

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
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDtdCommand 	= DVBS_TunerDVBSCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDetachI2C 	= DVBS_TunerDetachI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDisc 	= DVBS_TunerDisc;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetAttribute= DVBS_TunerSetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute= DVBS_TunerGetAttribute;
	pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute2= DVBS_TunerGetAttribute;
    //pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerBypassI2C = DVBS_TunerBypassI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerNotifyEnable = DVBS_TunerNotifyEnable;
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
//    ITUNER_CTX_T *tTunerCtx;

    //create pdctx used in this file, you also can add any declaration you want in pdctx
    pPdCtxDVBS = (DVBS_PD_CTX_T *) x_mem_calloc(1, sizeof(DVBS_PD_CTX_T));
    if (pPdCtxDVBS == NULL)
    {
        mcSHOW_USER_MSG(("DVB_PD_CTX_T malloc fail\n"));
        return (DRVAPI_TUNER_ERROR);
    }
	dev = (nim_device*) x_mem_calloc(1, sizeof(nim_device));
    if (dev == NULL)
    {
        mcSHOW_USER_MSG(("nim_device malloc fail\n"));
        return (DRVAPI_TUNER_ERROR);
    }
  //CREATE_OBJECT(Tuner_API, QPSK_TUNER_CONFIG_API);
  x_memset(&Tuner_API, 0, sizeof(struct QPSK_TUNER_CONFIG_API));
  //x_memset(tGET_tuner_info, 0, sizeof(TUNER_SAT_DIG_TUNE_INFO_T));
	/* Init Disconnect Status */
	pPdCtxDVBS->fgDisStatus = FALSE;
	pPdCtxDVBS->fgScanCallbackLocked = FALSE;

    //Create a semaphore for low level demod PI access protection
    if (x_sema_create(&pPdCtxDVBS->hHalLock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_USER_MSG(("DVB x_sema_create(pPdCtxDVBS->hHalLock) Fail!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    x_sema_unlock(pPdCtxDVBS->hHalLock);
	if (x_sema_create(&AutoScan_callback, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_USER_MSG(("DVB x_sema_create(AutoScan_callback) Fail!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    //x_sema_unlock(AutoScan_callback);
	
    //register LTDIS APIs to HAL
    PD_DVBS_Register_LTDIS_Fct(ptTDCtx);
    /* demod initialization (to do only once) */
	    DVBS_DriverInit(Tuner_API);
	// nim_open(dev);
	
    /* tuner initialization */
	
    //Disconnect demod
    
    //nim_close(dev);

	
#ifdef DVBS_ENABLE_MONITOR
    if (mcSEMA_CREATE(&pPdCtxDVBS->t_escape_mon_suspend, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_DRVERR_MSG(("EN_DVBS_TunerOpen (mcSEMA_CREATE): ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }

    pPdCtxDVBS->fgBreakMonitor = TRUE;//init
    pPdCtxDVBS->fgPIMoniStatus = FALSE; //init
   // pPdCtxDVBS->t_ScanMode = CONN_MODE_UNKNOWN;  //init
      PD_StartMonitorEngine(pPdCtxDVBS);  //start monitor engine
#endif 
	mcSHOW_USER_MSG(("DVBS_TunerOpen OK!\n"));

    return (DRVAPI_TUNER_OK);
}



