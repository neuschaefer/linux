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
 * $RCSfile: pd_glue_dvbt2.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
/** @file pd_glue_dvbt2.c
 *  dvbt2 glue layer
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
#include "pd_dvbt2_if.h"
#include "drvapi_tuner.h"
#include "UtilAPI.h"
#include "pod_if.h"
#include "drvcust_if.h"
#include "eeprom_if.h"
#include "pi_dvbt_if.h"
#include "u_drv_cust.h"
#include "ctrl_bus.h"
#include "x_gpio.h"

#include "mainhead.h" //add by Sony AEC 20100412

#define EMB_L1_DRIVER_CORE          /* empty  */
#define SLAB_WINAPI                 /* empty  */
#define EMB_L1_COMMON_DEMOD_API     /* empty  */
#define EMB_L1_MULTISTANDARD_API    /* empty  */
#define EMB_L1_DVB_C_DEMOD_API      /* empty  */
#define WINAPI                      /* empty  */





//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
//#define DVBT2_LOCK_TIME_OUT_CNT  10
#define DVBT2_LOCK_TIME_OUT_CNT 1 // liuqu modified

#define DVBT2_TUNER_CONN_TIME_THREHOLD   140
#define DVBT2_DEMOD_CONN_TIME_THREHOLD   200


#define SUPPORT_DVBT     1
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
static DVBT2_PD_CTX_T *pPdCtxDVBT2;
static DEMOD_CXD2820_CTX_T *pSonyDemod;

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
/***********************************************************************/
/*              External declarations                                  */
/***********************************************************************/
//EXTERN TUNER_API_FCT_TBL_T *pTuner_API_Fct;
//#if (fcTUNER_TYPE != cDTD_TUNER_ALL)
//EXTERN VOID Tuner_Register_Fct(TUNER_API_FCT_TBL_T *ptuner_Fct);
//#else
//EXTERN VOID Tuner_Register_Fct(TUNER_API_FCT_TBL_T *ptuner_Fct, UINT8 u1TunerType);
//#endif


/***********************************************************************/
/*              Private (static) Function Prototypes                   */
/***********************************************************************/
/////////////////////////////////////
#define SMART_RESE_MAX_GPIO_BUF_NUM  10
#define SMART_RESET_MAX_DELAY_MS  100

STATIC void SmartResetAct(UINT32 gpio)
{
		GPIO_SetOut(gpio, 0);
        mcDELAY_MS(SMART_RESET_MAX_DELAY_MS);
        GPIO_SetOut(gpio, 1);
        mcDELAY_MS(50); // Delay 20 ms for CR[DTV00030459]. Or it will init fail in rel image
     mcSHOW_USER_MSG(("DTD Reset Demod GPIO=%d, 0->1\r\n",gpio));
}

INT32 SmartResetDemod(UINT32 gpio)
{
static UINT32 GPIObuf[SMART_RESE_MAX_GPIO_BUF_NUM];
static UINT32 CallCnt=0;
UINT32 i=0;
if(CallCnt==0)
{//first time call
	SmartResetAct(gpio);
	*(GPIObuf)=gpio;//record first call
	for(i=1;i<SMART_RESE_MAX_GPIO_BUF_NUM;i++)
	{*(GPIObuf+i)=0xFFFFFFFF;}//clear buf
	CallCnt++;
	return 0;//reset it
}
else {
for(i=0;i<SMART_RESE_MAX_GPIO_BUF_NUM;i++)//serach it in table
{
	if(*(GPIObuf+i)== gpio )//get it
	{mcSHOW_USER_MSG(("already reset (%d)\n",gpio));		
		return 1;
	}
	if(*(GPIObuf+i)==0xFFFFFFFF)//not find it
	{*(GPIObuf+i)=gpio;//add it to tb
	SmartResetAct(gpio);//reset it
	return  0;
	}
}
if(i==SMART_RESE_MAX_GPIO_BUF_NUM)
{
mcSHOW_USER_MSG(("GPIO(%d)Reset Failed,for more than(%d) PIN reset Call\n",gpio,SMART_RESE_MAX_GPIO_BUF_NUM));
return 2;
}
	}

return  0;
}



STATIC VOID vResetDemod(UINT16 u2DelayTime)
{
	UINT32 u4RstPinNum;

    // Config GPIO to reset demod
    mcSHOW_DRVAPI_MSG(("vResetDemod\n"));
  if (DRVCUST_OptQuery(eDemodResetGpio, &u4RstPinNum) == 0)
    {
    SmartResetDemod(u4RstPinNum);
        mcDELAY_MS(u2DelayTime); 
     printf("DTD Reset Demod GPIO=0x%x, 0->1, %dms\r\n",u4RstPinNum, u2DelayTime);
    }
  else {printf("pls check eDemodResetGpio config\n");}
}

#ifdef DVBT2_ENABLE_MONITOR
STATIC BOOL SetBreakMonitor(DVBT2_PD_CTX_T *psPdCtx)
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
STATIC void ResumeMonitor(DVBT2_PD_CTX_T *psPdCtx)
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
STATIC INT32 i4DVBT2MonitorBreak(VOID *pvArg)                                                            
{                                                                                                       
    DVBT2_PD_CTX_T *psPdCtx = (DVBT2_PD_CTX_T *) pvArg;
                                                                                                        
    if (psPdCtx->fgBreakMonitor)
    {                                                                                                   
        mcSHOW_DBG_MSG(("DVBT2_Monitor break function triggered!!\n"));                                      
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
    
    DVBT2_PD_CTX_T *psPdCtx = *(DVBT2_PD_CTX_T **) pvArg;
    
    mcSHOW_DRVAPI_MSG(("MonitorProcess start!\n"));
        
    while (psPdCtx->fgMonitorThreadRunning)
    {
    	if (psPdCtx->fgBreakMonitor)
    	{
    		// After mcMUTEX_LOCK(t_escape_mon_suspend), monitor thread will be suspended until
    		// TunerAcq or TunerClose invoked.
            mcSHOW_DRVAPI_MSG(("Entering DVBT2 mon_suspend\n"));
    		mcMUTEX_LOCK(psPdCtx->t_escape_mon_suspend);
    		mcSHOW_DRVAPI_MSG(("Escape DVBT2 mon_suspend\n"));
    	}
    	else
    	{
		    // mutex lock for I2C access
            mcSEMA_LOCK_MTFE(psPdCtx->hHalLock);
	        mcSHOW_DBG_MSG2(("Invoke DVBT2_Monitor\n"));
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
STATIC S32 PD_StartMonitorEngine(DVBT2_PD_CTX_T *psPdCtx)
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
//    pPdCtxDVBT2->fgMonitorThreadRunning = FALSE; //need set to false before mcMUTEX_UNLOCK(t_escape_mon_suspend);
    while (pPdCtxDVBT2->t_monitor_thread){
        mcDELAY_MS(10); // in ms
    }
    mcSHOW_DRVAPI_MSG(("DVBT2 PD_StopMonitorEngine success\n"));
    return (DRVAPI_TUNER_OK);
}
#endif //DVBT2_ENABLE_MONITOR

#if 0
STATIC INT8 Tuner_Connect(U32 Freq, U8 sawbw, U8 Mode ,U8 AutoSearch)
{
    PARAM_SETFREQ_T SetFreqPara;	
    INT8 fgRet = 0;

    ITuner_OP(ITunerGetCtx(), itSetSawBw, sawbw, NULL);
   
    SetFreqPara.Freq = Freq;
    SetFreqPara.Modulation = Mode;
    SetFreqPara.fgAutoSearch = AutoSearch;   
    if((ITuner_SetFreq(ITunerGetCtx(), &SetFreqPara)) != 0) {
        printf("Failed on Tuner_SetFreq(%d)", Freq);
        fgRet = 1;
    }

    return fgRet;
}
#endif

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
INT32 DVBT2_TunerClose(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
    
//#if !defined(CC_MTAL) && !defined(CC_SDAL)    
#ifdef DVBT2_ENABLE_MONITOR
    pPdCtxDVBT2->fgBreakMonitor = TRUE; //disable and breadk monitor thread
    pPdCtxDVBT2->fgMonitorThreadRunning = FALSE; //need set to false before mcMUTEX_UNLOCK(t_escape_mon_suspend);
    mcMUTEX_UNLOCK(pPdCtxDVBT2->t_escape_mon_suspend);
    PD_StopMonitorEngine();  // stop monitor engine
    mcMUTEX_DESTROY(pPdCtxDVBT2->t_escape_mon_suspend);  // destory monitor thread
#endif

    //If you have allocated any resource in TunerOpen(), release them here.
    SONY_Sleep(pSonyDemod);

    //add driver implementation code here
    x_sema_delete(pPdCtxDVBT2->hHalLock);
    x_mem_free(pPdCtxDVBT2);

    return (DRVAPI_TUNER_OK);
}

VOID DVBT2_SetTSInterface(BOOL fgParallel)
{
#ifdef CC_TURNKEY
    UCHAR ucPar = 0;
#endif

    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

    if (fgParallel) {
        //mcSHOW_DBG_MSG(("[DVBT2] SetTSInterface : parallel\n"));
		mcSHOW_DBG_MSG(("continous clk--[DVBT2] SetTSInterface : parallel\n"));
    }
    else
    {
        mcSHOW_DBG_MSG(("[DVBT2] SetTSInterface : serial\n"));
    }

   SONY_PISetTSInterface(pSonyDemod, 3);
   
    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
}


/*non implement*/
void DVBT2_SetTSPath(BOOL fgEnable)
{
#if  !PORTING_TEMP
UCHAR  ucPar = 0;

    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

    if (fgEnable) {
        mcSHOW_DBG_MSG(("[DVBT2] SetTSPath : pass-through"));
        //Enable DVBT2 TS output
        ucPar = 0;
        L1_WriteBytes8Bits(pSpiderDemod, 0x4EF, 1, &ucPar);
        L1_WriteBytes8Bits(pSpiderDemod, 0x4F0, 1, &ucPar);
    }
    else {
        mcSHOW_DBG_MSG(("[DVBT2] SetTSPath : tri-state"));
        //Set DVBT2 TS output to high-Z
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

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
#endif
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
BOOL DVBT2_TunerAcq(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type, void* pv_conn_info,
                            SIZE_T z_conn_info_len, x_break_fct _BreakFct, void* pvArg)
{
    TUNER_TER_DIG_TUNE_INFO_T *pt_tuner_info = 
			    	(TUNER_TER_DIG_TUNE_INFO_T *) pv_conn_info;

    UINT8   fgChannelScan = FALSE;
    UINT32  freq = 0;
    UINT32  ii = 0;
    //UINT32  u4TimeOut = DVBT2_LOCK_TIME_OUT_CNT;   // 500MS  //150MS    (XF)
    sony_dvb_demod_lock_result_t LockStatus = DEMOD_LOCK_RESULT_NOTDETECT;
    sony_dvb_result_t   result = SONY_DVB_OK;
    UINT32  u4TickCnt, u4ConnTime;
    int bw;
	int hier;
	int Coex;
	int plpS;
	int plpID;
	unsigned char region;
    //Set break function
    pPdCtxDVBT2->isBreak = _BreakFct;

    //Set DVB-C scan mode
    pPdCtxDVBT2->t_ScanMode = pt_tuner_info->e_conn_mode;

    //MUTEX lock
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

#ifdef DVBT2_ENABLE_MONITOR
    SetBreakMonitor(pPdCtxDVBT2);
#endif

    //Set channel scan indicator
    if(pt_tuner_info->e_mod == MOD_UNKNOWN){ 
        fgChannelScan = TRUE;
    }

    u4TickCnt = os_get_sys_tick();

    //Demod init
    //no need to do init every time!

    //Convert freq
    freq = pt_tuner_info->ui4_freq/1000;

   switch(pt_tuner_info->e_bandwidth) 
    {
    case BW_6_MHz:
        mcSHOW_DBG_MSG(("Set BW = 6 MHz\n"));
        bw = SONY_DVBT2_BW_6MHz;
		pSonyDemod->conn_bandwith = SONY_DVBT2_BW_6MHz;
    break;
    case BW_7_MHz:
        mcSHOW_DBG_MSG(("Set BW = 7 MHz\n"));
        bw = SONY_DVBT2_BW_7MHz;
		pSonyDemod->conn_bandwith = SONY_DVBT2_BW_7MHz;
        break;
    case BW_8_MHz:
    default:
        mcSHOW_DBG_MSG(("Set BW = 8 MHz\n"));
		pSonyDemod->conn_bandwith = SONY_DVBT2_BW_8MHz;		
        bw = SONY_DVBT2_BW_8MHz;
        break;
    }

//DVBC tuning
    if (e_conn_type == TUNER_CONN_TYPE_CAB_DIG)    //DVBC
        {            		
              mcSHOW_DBG_MSG(("Enter DVBC Tuning now!\n")); 
		region = 0;
		result = DVBC_Tuning( pSonyDemod,  freq, region);
            
            	 if (result != SONY_DVB_OK)
               {
                mcSHOW_DBG_MSG(("Can not Tune at DVBC Signal\n"));
               }		   
          }
  //DVBT OR T2 TUNING PART	
   else if (e_conn_type == TUNER_CONN_TYPE_TER_DIG)
   	{
            if (pt_tuner_info->e_dvb_t_t2_mode == TUNER_DVB_T2)     /****T2 mode****/
             {      
                  mcSHOW_DBG_MSG(("Enter DVBT2 Tuning now!\n")); 
           	    Coex = 0;
           	    plpS = 1;  //need to set to 1 to enable plp select!!
           	    plpID = pt_tuner_info->ui1_plp_id;  //use the ID mw gives
		    mcSHOW_DBG_MSG(("T2 seperate tuning Set plpID= %d\n",plpID));	  
           	    result = DVBT2_Tuning( pSonyDemod,  freq, bw, Coex, plpS, plpID);          
              }
            else if(pt_tuner_info->e_dvb_t_t2_mode == TUNER_DVB_T)  /****T mode****/
             {
                    mcSHOW_DBG_MSG(("Enter DVBT Tuning now!\n"));  
            		if (pt_tuner_info->e_hierarchy_priority == DVBT_HIR_PRIORITY_LOW)
            			hier = 1; // set to LP 
            		else
            			hier = 0; // set to HP
            		Coex = 0;   
            		result = DVBT_Tuning( pSonyDemod,  freq, bw, hier, Coex);
            	}  
//		 else if (pt_tuner_info->e_dvb_t_t2_mode == TUNER_DVB_AUTO_T_T2)   /**AUTO MODE!!****/
               else
         	{
                     if (pt_tuner_info->e_dvb_t_t2_mode != TUNER_DVB_AUTO_T_T2)   
			 {
                      mcSHOW_USER_MSG(("Unknown e_dvb_t_t2_mode = %d\n",pt_tuner_info->e_dvb_t_t2_mode));
                      }		
					 
                    	//first try DVBT	
                    	mcSHOW_DBG_MSG(("Enter DVBT/T2 Auto Tuning now!\n")); 
                    	if (pt_tuner_info->e_hierarchy_priority == DVBT_HIR_PRIORITY_LOW)
        					hier = 1; // set to LP 
        				else
        					hier = 0; // set to HP
        				Coex = 0;	
                        result = DVBT_Tuning( pSonyDemod,  freq, bw, hier, Coex);
                    		
                         if (result !=SONY_DVB_OK) 
                         	{
                            //second try DVBT-2
                    		  Coex = 0;
        			  plpS = 1;//always set 1 for selection!!
        			  plpID = pt_tuner_info->ui1_plp_id;  //use the ID mw gives,it should be 0 in this case!
				  mcSHOW_DBG_MSG(("T2 Auto tuning Set plpID= %d\n",plpID));	  
                              result = DVBT2_Tuning( pSonyDemod,  freq, bw, Coex, plpS, plpID);
                               if (result != SONY_DVB_OK)                   
                       	       {                           
					 mcSHOW_DBG_MSG(("Can not Tune at any T/T2 Signal\n"));
                       	       }
                         	}                                          
         	}	
	#if 0		   
		   else    /*should not enter this folder*/
                  {
                    mcSHOW_USER_MSG(("Unknown e_dvb_t_t2_mode = %d\n",pt_tuner_info->e_dvb_t_t2_mode));
                   }		
	#endif	   
   	      }
		  /*endof else if (e_conn_type == TUNER_CONN_TYPE_TER_DIG)*/
		
           else  /*****unknow conn type!!*************/
            	{
                   mcSHOW_USER_MSG(("Error TUNER CONN TYPE =%d!",e_conn_type));
            	 }

 //final check TS Lock
	dvb_demod_CheckLock(pSonyDemod->cxd2820.pDemod, &LockStatus);//add by SONY AEC 20100412
#if 1   // don't need to polling more than 1 times??
    if ( LockStatus != DEMOD_LOCK_RESULT_LOCKED)
    {
       for (ii = 0; ii < DVBT2_LOCK_TIME_OUT_CNT; ii ++)
       	{
                 dvb_demod_CheckLock(pSonyDemod->cxd2820.pDemod, &LockStatus);//add by SONY AEC 20100412			   

	        if (LockStatus ==DEMOD_LOCK_RESULT_LOCKED)
			break;
			
		mcDELAY_MS(50);

		 if (pPdCtxDVBT2->isBreak(pvArg))
               {
                   break;
               }
       	}	   
    }	   
   
#endif
    mcSHOW_DRVAPI_MSG(("Freq = %d\n", freq));
    mcSHOW_DRVAPI_MSG(("symtem = %d\n", pSonyDemod->cxd2820.pDemod->system));
    mcSHOW_DRVAPI_MSG(("Bandwidth = %d\n", pSonyDemod->cxd2820.pDemod->bandWidth));
    mcSHOW_DRVAPI_MSG(("Time-out cnt = %d\n", ii));
	

  

    //Check tuner conncection time
    u4ConnTime = (os_get_sys_tick()-u4TickCnt) * x_os_drv_get_tick_period();
    mcSHOW_DBG_MSG(("DVBT2 tuner connection time = %dms\n", u4ConnTime));
    if ((u4ConnTime > DVBT2_TUNER_CONN_TIME_THREHOLD) && (fgChannelScan)){
		mcSHOW_DBG_MSG(("EU Cable Driver ATP Fail: DVBT2 tuner connection time = %dms, too long!!\n", u4ConnTime));
	}

    u4TickCnt = os_get_sys_tick();

       	
   
     if (LockStatus == DEMOD_LOCK_RESULT_LOCKED)
    {
		SONY_PISetTSInterface(pSonyDemod, 3);// default set parrallel TS output
		SONY_SetTsPath(pSonyDemod,1);

		mcSHOW_DRVAPI_MSG(("continous clk----------Front-end lock\n"));
		
        //Check demod conncection time
        u4ConnTime = (os_get_sys_tick()-u4TickCnt) * x_os_drv_get_tick_period();
        mcSHOW_DBG_MSG(("SONY demod connection time = %dms\n", u4ConnTime));
        if ((u4ConnTime > DVBT2_DEMOD_CONN_TIME_THREHOLD) && (fgChannelScan)){
            mcSHOW_DBG_MSG(("EU Cable Driver ATP Fail: SONY demod connection time = %dms, too long!!\n", u4ConnTime));
        }
    }
	 else
	 {
           LockStatus = DEMOD_LOCK_RESULT_NOTDETECT ;  //set to 0 when not-detected and not-locked!!
	 }


    
#ifdef DVBT2_ENABLE_MONITOR
    if (!pPdCtxDVBT2->isBreak(pvArg))// && (u1ConnMode == ATV_CONN_MODE_CHAN_CHANGE))
    {
        //psAtvPdCtx->fgBreakMonitor = FALSE;           // Enable monitor thread and Disable break function flag
        //mcMUTEX_UNLOCK(t_escape_anaeu_mon_suspend);  // Unlock monitor thread
        ResumeMonitor(pPdCtxDVBT2);   // Enable monitor thread and Disable break function flag,  Unlock monitor thread
        mcSHOW_DBG_MSG(("Enable monitor process!!\n"));
    }
#endif

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);

	/* Init Disconnect Status */
	pPdCtxDVBT2->fgDisStatus = FALSE;	

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
INT16 DVBT2_TunerGetSync(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT16 u2Ret=0;
    sony_dvb_demod_lock_result_t  LockStatus = DEMOD_LOCK_RESULT_UNLOCKED;

    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
    //add driver implementation code here
//    dvb_demod_CheckTSLock(pSonyDemod->cxd2820.pDemod, &LockStatus);
	dvb_demod_CheckLock(pSonyDemod->cxd2820.pDemod, &LockStatus);//add by SONY AEC 20100412

    if (LockStatus ==DEMOD_LOCK_RESULT_LOCKED )
     u2Ret = 1;
    else
     u2Ret = 0;		
    
    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
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
VOID  DVBT2_TunerDisc(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
	if(pPdCtxDVBT2->fgDisStatus == TRUE)
	{
		/* Has Disconnected, return */
		mcSHOW_DBG_MSG(("DVBT2 Has Disconnect!\n"));
		return;
	}
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
    //add driver implementation code here
    if (SONY_ChipPowerDown(pSonyDemod) == 1){
        mcSHOW_USER_MSG(("Failed on DVBT2_ChipPowerDown()\n"));
    }

#ifdef DVBT2_ENABLE_MONITOR
    SetBreakMonitor(pPdCtxDVBT2);
#endif

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);

	/* set disconnect status */
	pPdCtxDVBT2->fgDisStatus = TRUE;
}



/*<<<<<<<<< Need to do further implementation <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
#if PORTING_TEMP
void DVBT2_TunerBypassI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bSwitchOn)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

    if (bSwitchOn == TRUE) {
//        L1_WRITE(pSpiderDemod, i2c_passthru      , i2c_passthru_enabled);
    }
    else
    {
 //       L1_WRITE(pSpiderDemod, i2c_passthru      , i2c_passthru_disabled);
    }

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
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
void DVBT2_TunerGetSignal(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type, SIGNAL *_pSignal)
{
   
   mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
    
    //add driver implementation code here
    _pSignal->Frequency  = (S32)(pSonyDemod->cxd2820.pTuner->frequency);
    // removed, tuner HAL already did the conversion
    //_pSignal->Frequency *= 1000; // convert from kHz to Hz 
    _pSignal->SymbolRate = 0;
    _pSignal->QAMSize = 0;
    _pSignal->u1DVBT_T2_indicator = TUNER_DVB_AUTO_T_T2;
	mcSHOW_DBG_MSG(("system  = %d (0:None, 1:DVBC, 2:DVBT, 3:DVBT2)\n", pSonyDemod->cxd2820.pDemod->system));

	SONY_GetInfo(pSonyDemod);//update information of demod
	
	//update t_t2 mode value	
	switch ( pSonyDemod->cxd2820.pDemod->system)
		{
		case SONY_DVB_SYSTEM_DVBT:
	        _pSignal->u1DVBT_T2_indicator = TUNER_DVB_T ;
			if(pSonyDemod->DVBT_info.TPS_info.hierarchy == 0)
				_pSignal->e_hierarchy_priority = 1;
			else 
			{
				if(pSonyDemod->DVBT_info.hier == 0)
					_pSignal->e_hierarchy_priority = 2;
				else
					_pSignal->e_hierarchy_priority = 3;
			}
			switch( pSonyDemod->DVBT_info.bandwidth)
			{
			    case 8:
			        _pSignal->e_bandwidth = BW_8_MHz; break;
			    case 7:
			        _pSignal->e_bandwidth = BW_7_MHz; break;
			    case 6:
			        _pSignal->e_bandwidth = BW_6_MHz; break;		
			    default:
			        _pSignal->e_bandwidth = BW_UNKNOWN; break;
    		}
   	
			switch( pSonyDemod->DVBT_info.TPS_info.hierarchy)
			{
				case 0:
					_pSignal->sHierInfo.eAlpha = ALPHA_0; break;
				case 1:
					_pSignal->sHierInfo.eAlpha = ALPHA_1; break;
				case 2:
					_pSignal->sHierInfo.eAlpha = ALPHA_2; break;	
				case 3:
					_pSignal->sHierInfo.eAlpha = ALPHA_4; break;	
				default:
					 break;
			}
			mcSHOW_DBG_MSG(("DVBT_TunerGetSignal: PRIORITY = %d (1:None, 2:HP, 3:LP)\n", _pSignal->e_hierarchy_priority));
    		mcSHOW_DBG_MSG(("DVBT_TunerGetSignal: Alpha = %d (3: a=4)\n", _pSignal->sHierInfo.eAlpha));
   	 		mcSHOW_DBG_MSG(("DVBT_TunerGetSignal: e_bandwidth = %d (1:BW_6_MHz,2:BW_7_MHz,3:BW_8_MHz,)\n", _pSignal->e_bandwidth));
		   break;
		case SONY_DVB_SYSTEM_DVBT2:
	        _pSignal->u1DVBT_T2_indicator = TUNER_DVB_T2 ;
		 _pSignal->ui1_plp_id = (UINT8)SONY_GetPlpInfo(pSonyDemod);
		 mcSHOW_DBG_MSG(("Get signal :SONY_GetPlpInfo = %d\n", _pSignal->ui1_plp_id ));
	      break;
	       case SONY_DVB_SYSTEM_DVBC:
				_pSignal->QAMSize = pSonyDemod->DVBC_info.QAMSize;
				_pSignal->SymbolRate = pSonyDemod->DVBC_info.SymbolRate;
				mcSHOW_DBG_MSG(("DVBC_TunerGetSignal: QAMSize = %d \n", _pSignal->QAMSize));
    			       mcSHOW_DBG_MSG(("DVBC_TunerGetSignal: SymbolRate = %d (Khz/s)\n", _pSignal->SymbolRate));
   	 			
				break;
		case SONY_DVB_SYSTEM_UNKNOWN:
		default:
			mcSHOW_DBG_MSG(("system unknown = %d \n", pSonyDemod->cxd2820.pDemod->system));
	      break;  
		}
     	
#if 0

    vDVBTGetPara(psDvbtDemodCtx); // update Hier, Hp/Lp, BW

	switch (DVBT_GetBW(psDvbtDemodCtx))
	{
		case 0:  _pSignal->e_bandwidth = BW_6_MHz;   break;
		case 1:  _pSignal->e_bandwidth = BW_7_MHz;   break;
		case 2:  _pSignal->e_bandwidth = BW_8_MHz;   break;
		case 3:
		default: _pSignal->e_bandwidth = BW_UNKNOWN; break;
	}

    _pSignal->e_hierarchy_priority = DVBT_GetHier(psDvbtDemodCtx);
    _pSignal->sHierInfo.eAlpha = (ALPHA_INFO_T)DVBT_GetHierMode(psDvbtDemodCtx);
    _pSignal->sHierInfo.eInDepth = (INTERLEAVE_INFO_T)DVBT_GetIndepth(psDvbtDemodCtx);
    mcSHOW_DBG_MSG(("DVBT_TunerGetSignal: PRIORITY = %d (1:None, 2:HP, 3:LP)\n", _pSignal->e_hierarchy_priority));
    mcSHOW_DBG_MSG4(("DVBT_TunerGetSignal: Alpha = %d (3: a=4)\n", _pSignal->sHierInfo.eAlpha));
    mcSHOW_DBG_MSG4(("DVBT_TunerGetSignal: Indepth = %d \n", _pSignal->sHierInfo.eInDepth));
#endif

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
}

//-------------------------------------------------------------------------
/**
 *  This API get signal level in terms of percentage.
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  0~100              signal level
 */
//-------------------------------------------------------------------------
UINT8 DVBT2_TunerGetSignalLevel(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    unsigned short SSI, SQI;
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
    //add driver implementation code here
    SONY_Get_SSI_SQI(pSonyDemod, &SSI, &SQI);
    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
    //example of return
    return (UINT8)SSI;

}

//-------------------------------------------------------------------------
/**
 *  This API get signal level in multiples of (0.1dBm).
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  0 ~ -900           signal level
 */
//-------------------------------------------------------------------------
INT16 DVBT2_TunerGetSignalLeveldBm(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    INT16 i2Ret=0;
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
    //add driver implementation code here
    mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerGetSignalLeveldBm()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
    return i2Ret;
}

UINT32 DVBT2_TunerGetSignalBER(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{

    INT32 u4_ret;
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
     u4_ret = SONY_GetBER(pSonyDemod);

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);

    return u4_ret;//modify return value

}

UINT16 DVBT2_TunerGetSignalPER(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{

    UINT32 u4_ret = 0;
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerGetSignalPER()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);

    return u4_ret;//modify return value

}

UINT16 DVBT2_TunerGetSignalSNR(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT16 SNR = 0;
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

	//add your implementation here
   // SNR =DVBT2_GetSNR(pSonyDemod);   // LIUQU TODO,20100413

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);

    return SNR;//modify return value
}

void DVBT2_TunerSetMpgFmt(PTD_SPECIFIC_CTX ptTDSpecificCtx, MPEG_FMT_T  *pt_mpeg_fmt)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerSetMpgFmt()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
}

char *DVBT2_TunerGetVer(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
  char * version = "SONY Demod driver version: 2010 v1.2";

    return version;
}

INT32 DVBT2_TunerNimTest(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    INT32 i4Ret = 0;
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

	//add your implementation here
    
   mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerNimTest()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
    return i4Ret;
}
void DVBT2_TunerSetRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx, UCHAR ucRegSet, UCHAR ucRegAddr, UCHAR ucRegValue)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerSetRegSetting()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
}

void DVBT2_TunerShowRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerShowRegSetting()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
}

void DVBT2_TunerTestI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerTestI2C()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
}

#else
void DVBT2_TunerBypassI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bSwitchOn)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

    if (bSwitchOn == TRUE) {
//        L1_WRITE(pSpiderDemod, i2c_passthru      , i2c_passthru_enabled);
    }
    else
    {
 //       L1_WRITE(pSpiderDemod, i2c_passthru      , i2c_passthru_disabled);
    }

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
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
void DVBT2_TunerGetSignal(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type, SIGNAL *_pSignal)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
    //add driver implementation code here
    // liuqu copy from dvbt mode

    _pSignal->Frequency  = (S32)(psDvbtDemodCtx->psTunerCtx->u4RF_Freq - (DVBT_GetFreqOffset_kHz(psDvbtDemodCtx)>>8));
    // removed, tuner HAL already did the conversion
    //_pSignal->Frequency *= 1000; // convert from kHz to Hz 
    _pSignal->SymbolRate = 0;
    _pSignal->QAMSize = 0;

    vDVBTGetPara(psDvbtDemodCtx); // update Hier, Hp/Lp, BW

	switch (DVBT_GetBW(psDvbtDemodCtx))
	{
		case 0:  _pSignal->e_bandwidth = BW_6_MHz;   break;
		case 1:  _pSignal->e_bandwidth = BW_7_MHz;   break;
		case 2:  _pSignal->e_bandwidth = BW_8_MHz;   break;
		case 3:
		default: _pSignal->e_bandwidth = BW_UNKNOWN; break;
	}

    _pSignal->e_hierarchy_priority = DVBT_GetHier(psDvbtDemodCtx);
    _pSignal->sHierInfo.eAlpha = (ALPHA_INFO_T)DVBT_GetHierMode(psDvbtDemodCtx);
    _pSignal->sHierInfo.eInDepth = (INTERLEAVE_INFO_T)DVBT_GetIndepth(psDvbtDemodCtx);
    mcSHOW_DBG_MSG(("DVBT_TunerGetSignal: PRIORITY = %d (1:None, 2:HP, 3:LP)\n", _pSignal->e_hierarchy_priority));
    mcSHOW_DBG_MSG4(("DVBT_TunerGetSignal: Alpha = %d (3: a=4)\n", _pSignal->sHierInfo.eAlpha));
    mcSHOW_DBG_MSG4(("DVBT_TunerGetSignal: Indepth = %d \n", _pSignal->sHierInfo.eInDepth));
    
    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);

}

//-------------------------------------------------------------------------
/**
 *  This API get signal level in terms of percentage.
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  0~100              signal level
 */
//-------------------------------------------------------------------------
UINT8 DVBT2_TunerGetSignalLevel(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT8 u1Ret=0;
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
    //add driver implementation code here
    u1Ret = (UINT8)L1_Demod_GetSignalLevel(pSpiderDemod);

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
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
INT16 DVBT2_TunerGetSignalLeveldBm(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    INT16 i2Ret=0;
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
    //add driver implementation code here
    mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerGetSignalLeveldBm()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
    return i2Ret;
}

UINT16 DVBT2_TunerGetSignalPER(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{

    //INT32 u4_ret;
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerGetSignalPER()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);

    return 0;//modify return value

}

UINT32 DVBT2_TunerGetSignalBER(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{

    UINT32 u4_ret = 0;
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerGetSignalBER()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);

    return u4_ret;//modify return value

}

UINT16 DVBT2_TunerGetSignalSNR(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT16 SNR = 0;
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

	//add your implementation here
    SNR = (UINT16)(L1_Demod_get_C_N_dB_int(pSpiderDemod)/1000);

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);

    return SNR;//modify return value
}

void DVBT2_TunerSetMpgFmt(PTD_SPECIFIC_CTX ptTDSpecificCtx, MPEG_FMT_T  *pt_mpeg_fmt)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerSetMpgFmt()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
}

char *DVBT2_TunerGetVer(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG((" DVBT2_TunerGetVer :201002_v1.0\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
    return NULL;
}

INT32 DVBT2_TunerNimTest(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    INT32 i4Ret = 0;
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

	//add your implementation here
    i4Ret = Tuner_Connect(666000, SAW_BW_8M, FALSE);
	
    mcSHOW_DRVERR_MSG(("DVBT2_TunerNimTest : %d (0:OK, 1:fail)\n", i4Ret));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
    return i4Ret;
}
void DVBT2_TunerSetRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx, UCHAR ucRegSet, UCHAR ucRegAddr, UCHAR ucRegValue)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerSetRegSetting()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
}

void DVBT2_TunerShowRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerShowRegSetting()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
}

void DVBT2_TunerTestI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerTestI2C()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
}

#endif


void DVBT2_TunerCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx, INT32 i4Argc, const CHAR** aszArgv)
{
  //  UCHAR   ucArg1;
    UCHAR   ucCmdSize = 0;
    //UINT8 tunerType;
//    UINT8 ucData[256];
 //   UCHAR   ucPar;
//    U16    ii, u2RegAddr;
 //   CHAR * txt;
  //  txt   = (CHAR*) x_mem_calloc(1, sizeof(CHAR)*1000);
    
	mcSHOW_DBG_MSG4(("DVBT2_TunerCommand pPdCtx->hHalLock\n"));
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	mcSHOW_DBG_MSG4(("escape DVBT2_TunerCommand pPdCtx->hHalLock\n"));
    	    
    if (i4Argc > 0) {
        //ucCmdId = *((CHAR *) aszArgv[0]);
        ucCmdSize = x_strlen((CHAR *) aszArgv[0]);
    }

    switch (ucCmdSize) {
    case 1: // one-word cmd
        if (x_strcmp((CHAR *) aszArgv[0], "t") == 0) {

        }
        else if (x_strcmp((CHAR *) aszArgv[0], "r") == 0) {
           
        }
        else if (x_strcmp((CHAR *) aszArgv[0], "w") == 0) {
          
        } 
        else if (x_strcmp((CHAR *) aszArgv[0], "s") == 0) {
           
        }
        else if (x_strcmp((CHAR *) aszArgv[0], "q") == 0) {
            
        }
        break;
    case 2: // two-word cmd
        if (x_strcmp((CHAR *) aszArgv[0], "sb") == 0) {
          
        }
        else if (x_strcmp((CHAR *) aszArgv[0], "sr") == 0) {
            
        }
        break;

    case 3:
        if (x_strcmp((CHAR *) aszArgv[0], "cpd") == 0) {
            SONY_ChipPowerDown(pSonyDemod);
        }
        break;

    default:
        mcPRINTF("Cmd: \n");
        mcPRINTF("\t t  [Freq (KHz)]         : set tuner frequency\n");
        mcPRINTF("\t r  [RegAddr] [Num]      : read  Demod register\n");
        mcPRINTF("\t w  [RegAddr] [Value]    : write Demod register\n"); 
        mcPRINTF("\t s                       : status\n");
        mcPRINTF("\t q  [4/16/32/64/128/256] : set qam mode\n");
        mcPRINTF("\t sb [SymRate (KHz)]      : set symbol rate\n");
        mcPRINTF("\t sr                      : system reset\n");
        mcPRINTF("\t pd                      : demod power down\n");
        mcPRINTF("\t pu                      : demod power up\n");
        mcPRINTF("\t if [0(normal)/1(tri)]   : set ifacg\n");
        mcPRINTF("\t tb [0(non bypass)/1(bypass)]   : set i2c tuner bypass\n");
        mcPRINTF("\t cpd                     : chip power down\n");
        mcPRINTF("\t te [0(disable)/1(enable)]      : ts enable\n");
        mcPRINTF("\t tm [0(serial)/1(parallel)]     : ts mode\n");
                

        mcPRINTF("\r\n");
        mcPRINTF("\r\n");
        break;
    }

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
    mcSHOW_DBG_MSG4(("DVBT2_TunerCommand unlock pPdCtx->hHalLock\n"));
}

void DVBT2_TunerDVBT2Command(PTD_SPECIFIC_CTX ptTDSpecificCtx,INT32 i4Argc, const CHAR** aszArgv)
{
    UCHAR   ucCmdId = 0;
//    CHAR    *txt;
//    sony_dvb_result_t result = SONY_DVB_ERROR_OTHER;
    
    if (i4Argc < 1)
    {
        mcPRINTF("Cmd: \n");
        mcPRINTF("\t s                       : status\n");
    }	

    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
   
    if (i4Argc > 0)
        ucCmdId = *((CHAR *) aszArgv[0]);
        
    switch (ucCmdId)
    {
    case 's': // one-word cmd
        SONY_GetStatus(pSonyDemod);
        break;
#if 0		
    case 'e':
	{
	UINT8 fgEnable = (UINT8)StrToInt(aszArgv[1]);	
	DVBT2_sony_log_enable(fgEnable);
    	}
	break;
#endif	
    case 't':
    	{
    	 UINT8 fgParallel = (UINT8)StrToInt(aszArgv[1]);	  
		 
	SONY_PISetTSInterface(pSonyDemod,  fgParallel);
	
    	}
	break;
	
     case 'g':
	{
	   if (i4Argc >2)
	     {
//	   UINT8 configId = (UINT8)StrToInt(aszArgv[1]);	
//	   UINT8 value = (UINT8)StrToInt(aszArgv[2]);	
	
        //  dvb_demod_SetConfig( pSonyDemod->cxd2820.pDemod,(sony_dvb_demod_config_id_t) configId,(int32 )value);
	    }
	else
	   {
          mcSHOW_USER_MSG(("nim.dtd g configID,value \n"));
	    }
	}
	break;
#if 1	
   case 'r':
   	{
	if (i4Argc >2)
		{
	UINT8 bank =(UINT8)StrToInt(aszArgv[1]);
   	UINT8 subAddress =(UINT8)StrToInt(aszArgv[2]);
	UINT8 rdata = 0xff;
	   	
	  SONY_Demod_Read(pSonyDemod, bank, subAddress,&rdata);
	  
	 mcSHOW_USER_MSG(("Read Bank 0x%02x ,sub addr 0x%02x = 0x%02x\n",bank,subAddress,rdata));
         }
	 else
	 	{
               mcSHOW_USER_MSG(("nim.dtd r bank,subAddress \n"));
	 	}
	}
       break;
	   
   case 'w':
   	{
	if (i4Argc >3)
       {
   	UINT8 bank =(UINT8)StrToInt(aszArgv[1]);
   	UINT8 subAddress =(UINT8)StrToInt(aszArgv[2]);
	UINT8 wdata = (UINT8)StrToInt(aszArgv[3]);
	UINT8 mask =0xff;
	
	if (i4Argc >3)
	{
	   mask =(UINT8)StrToInt(aszArgv[4]);
	}

	mask = mask;  //clear warning.
	
       SONY_Demod_Write(pSonyDemod, bank, subAddress, wdata);
               mcSHOW_USER_MSG(("write Bank 0x%02x ,sub address 0x%02x = 0x%02x\n",bank,subAddress,wdata));
	               
   	   }
	  
        else
	  {
            mcSHOW_USER_MSG(("nim.dtd w bank,subAddress data \n"));
	  }
	}	
      break;
#endif	  
      case 'l':	  
	  SONY_Sleep(pSonyDemod);
	  mcSHOW_USER_MSG(("CLI make dvb_cxd2820_Sleep \n"));
	  break;
      case 'f':
	 {
	   UINT32  freq = 666000;
          sony_dvb_demod_lock_result_t LockStatus = DEMOD_LOCK_RESULT_NOTDETECT;
          sony_dvb_result_t   result = SONY_DVB_OK;	   	  
          int plpS = 0;    //use default setting!
          int plpID =0;   // use default setting!	

	if (i4Argc >3)
       {
           freq = (UINT32)StrToInt(aszArgv[1]);	    
           plpID =(UINT8)StrToInt(aszArgv[2]);
	    plpS =(UINT8)StrToInt(aszArgv[3]);	   
	}
	else if (i4Argc >2)
	{
         freq = (UINT32)StrToInt(aszArgv[1]);
	  plpID =(UINT8)StrToInt(aszArgv[2]);
	}
	else if (i4Argc >1)
	{
	  freq = (UINT32)StrToInt(aszArgv[1]);
	}
	
	   result = DVBT2_Tuning( pSonyDemod,  freq, SONY_DVBT2_BW_8MHz, 0, plpS, plpID);

	   if (!result)
	   	{
              mcSHOW_USER_MSG(("DVBT2_Tuning return %d\n",result));
	   	}
	   dvb_demod_CheckLock(pSonyDemod->cxd2820.pDemod, &LockStatus);
	   
	   if (LockStatus == DEMOD_LOCK_RESULT_LOCKED)
		 {
		 SONY_PISetTSInterface(pSonyDemod, 1);// default set parrallel TS output
		 SONY_SetTsPath(pSonyDemod,1);	   
		 mcSHOW_USER_MSG(("Front-end lock\n"));			 
		  }
		   else
		   {
			LockStatus = DEMOD_LOCK_RESULT_NOTDETECT ;  //set to 0 when not-detected and not-locked!!
                     mcSHOW_USER_MSG(("Front-end unlock\n"));
		   }	   
     	}
	  break;
    default:
        mcPRINTF("Cmd[20110120_1]: \n");
        mcPRINTF("\t s                       : status\n");
	 mcPRINTF("\t e                       : enable/disable SONY log\n");	
	 mcPRINTF("\t t                       : set TS interface parrel(1) /serial(0)\n");
	 mcPRINTF("\t r                       : read demod registers\n");
	 mcPRINTF("\t w                       : write demod registers\n");
	 mcPRINTF("\t g                       : set sony demod config\n");
	 mcPRINTF("\t l                       : make sony demod sleep\n");
	 mcPRINTF("\t f                       : tune a freuency with pldid.Para:[Freq][PlpID][Select]\n");	 
	 
        break;
    }    

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
}

void DVBT2_TunerDetachI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bDetach)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
    mcSHOW_DBG_MSG2(("No implementation in DVBT2_TunerDetachI2C()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
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
INT16 DVBT2_TunerGetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_GET_TYPE_T  e_get_type,
                                VOID *pzAttribute, SIZE_T* pzAttributeLen)
{

    TUNER_DIG_ATTRIBUTE_T *pAttri;
    ITUNER_CTX_T *pTunerCtx;
    INT32 i4Temp = 0;
//    Si2165_Context *Si2165;

    pTunerCtx = ITunerGetCtx();
//    Si2165  = (Si2165_Context *) pSpiderDemod->specific;

    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

    //add driver implementation code here
    switch (e_get_type)
    {
   case TUNER_GET_TYPE_BER:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_BER\r\n"));
        *pzAttributeLen = sizeof(INT32);
        i4Temp = SONY_GetBER(pSonyDemod);
        x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);
        break;
    case TUNER_GET_TYPE_DIG_ATTRIBUTE:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_DIG_ATTRIBUTE\r\n"));
        pAttri = (TUNER_DIG_ATTRIBUTE_T *) pzAttribute;
        //Tuner frequency boundary
        pAttri->ui4_lower_bound_freq = pTunerCtx->specific_member.eu_ctx.m_s4FreqBoundLower;
        pAttri->ui4_upper_bound_freq = pTunerCtx->specific_member.eu_ctx.m_s4FreqBoundUpper;
        // freq offset tolerence range in Hz (1M mapping to 100K)
		pAttri->b_auto_qam_capability = TRUE;
		pAttri->b_auto_sr_capability = TRUE;
     switch ( pSonyDemod->cxd2820.pDemod->system)
     	{

	    case SONY_DVB_SYSTEM_DVBC:
               pAttri->ui4_fine_tune_delta_freq = 400000;
              break;	    	
	     case SONY_DVB_SYSTEM_DVBT:
	     case SONY_DVB_SYSTEM_DVBT2: 	
                	{
		        switch ( pSonyDemod->DVBT_info.bandwidth)
			 {
				case 6:  pAttri->ui4_fine_tune_delta_freq = 536000; break; //6MHz
				case 7:  pAttri->ui4_fine_tune_delta_freq = 625000; break; //7MHz
				case 8:  pAttri->ui4_fine_tune_delta_freq = 714000; break; //8MHz
				
				default: pAttri->ui4_fine_tune_delta_freq = 0;		break;
			}	     	
		 break;
		 default:
		 	break;
		 }
     	}	 
        break;
    case TUNER_GET_TYPE_RAW_SIGNAL_LEVEL:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_RAW_SIGNAL_LEVEL\r\n"));
        *pzAttributeLen = sizeof(S8);
	//   i4Temp =DVBT2_GetIFAGC(pSonyDemod);
        x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);
        break;

    case TUNER_GET_TYPE_BER_TOLERANCE:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_BER_TOLERANCE\r\n"));
        *pzAttributeLen = sizeof(INT32);
        i4Temp = 430;  // copy from in-house DVBT
        x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);
        break;

    case TUNER_GET_TYPE_SIGNAL_LEVEL_TOLERANCE:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_SIGNAL_LEVEL_TOLERANCE\r\n"));
        *pzAttributeLen = sizeof(UINT8);
        x_memcpy(pzAttribute, &(pTunerCtx->specific_member.eu_ctx.m_SigLvTh), *pzAttributeLen);
        break;
		
	case TUNER_GET_TYPE_TS_FMT:
        {
            TS_FMT_T tsFmt;
            *pzAttributeLen = sizeof(TS_FMT_T);
            tsFmt = SONY_GetTsFmt(pSonyDemod);
            x_memcpy(pzAttribute, &tsFmt, *pzAttributeLen);
        }
        break;	
	
    default:
        mcSHOW_DBG_MSG3(("unknown: e_get_type = %d\r\n", e_get_type));
        break;
    }

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);

    return DRVAPI_TUNER_OK;
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
INT16 DVBT2_TunerGetAttribute2(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_GET_TYPE_T  e_get_type,
                                VOID *pzAttribute, SIZE_T* pzAttributeLen, DRV_CONN_TYPE_T e_conn_type)
{

    TUNER_DIG_ATTRIBUTE_T *pAttri;
    ITUNER_CTX_T *pTunerCtx;
	unsigned short SSI,SQI;
    INT32 i4Temp = 0;
//    Si2165_Context *Si2165;

    pTunerCtx = ITunerGetCtx();
//    Si2165  = (Si2165_Context *) pSpiderDemod->specific;

    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

    //add driver implementation code here
    switch (e_get_type)
    {
   case TUNER_GET_TYPE_BER:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_BER\r\n"));
        *pzAttributeLen = sizeof(INT32);
        i4Temp = SONY_GetBER(pSonyDemod);
        x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);
        break;
    case TUNER_GET_TYPE_DIG_ATTRIBUTE:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_DIG_ATTRIBUTE\r\n"));
        pAttri = (TUNER_DIG_ATTRIBUTE_T *) pzAttribute;
        //Tuner frequency boundary
        pAttri->ui4_lower_bound_freq = pTunerCtx->specific_member.eu_ctx.m_s4FreqBoundLower;
        pAttri->ui4_upper_bound_freq = pTunerCtx->specific_member.eu_ctx.m_s4FreqBoundUpper;
        // freq offset tolerence range in Hz (1M mapping to 100K)

     switch (e_conn_type)
     {

	    case TUNER_CONN_TYPE_CAB_DIG:
               pAttri->ui4_fine_tune_delta_freq = 400000;
			   pAttri->b_auto_qam_capability = TRUE;
			   pAttri->b_auto_sr_capability = TRUE;
			   mcSHOW_DBG_MSG(("delta_freq = %d\r\n",pAttri->ui4_fine_tune_delta_freq));
              break;	    	
	     case TUNER_CONN_TYPE_TER_DIG:
       	 {
		     switch ( pSonyDemod->conn_bandwith)
			 {
				case 6:  pAttri->ui4_fine_tune_delta_freq = 536000; break; //6MHz
				case 7:  pAttri->ui4_fine_tune_delta_freq = 625000; break; //7MHz
				case 8:  pAttri->ui4_fine_tune_delta_freq = 714000; break; //8MHz
				
				default: pAttri->ui4_fine_tune_delta_freq = 0;		break;
			}	     	
		 break;
		 default:
		 	break;
		 }
     	}	 
        break;
    case TUNER_GET_TYPE_RAW_SIGNAL_LEVEL:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_RAW_SIGNAL_LEVEL\r\n"));
        *pzAttributeLen = sizeof(S8);
	//   i4Temp =DVBT2_GetIFAGC(pSonyDemod);
        x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);
        break;

    case TUNER_GET_TYPE_BER_TOLERANCE:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_BER_TOLERANCE\r\n"));
        *pzAttributeLen = sizeof(INT32);
        i4Temp = 430;  // copy from in-house DVBT
        x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);
        break;

    case TUNER_GET_TYPE_SIGNAL_LEVEL_TOLERANCE:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_SIGNAL_LEVEL_TOLERANCE\r\n"));
        *pzAttributeLen = sizeof(UINT8);
        x_memcpy(pzAttribute, &(pTunerCtx->specific_member.eu_ctx.m_SigLvTh), *pzAttributeLen);
        break;
		
	case TUNER_GET_TYPE_TS_FMT:
        {
            TS_FMT_T tsFmt;
            *pzAttributeLen = sizeof(TS_FMT_T);
            tsFmt = SONY_GetTsFmt(pSonyDemod);
            x_memcpy(pzAttribute, &tsFmt, *pzAttributeLen);
        }
        break;	
	case TUNER_GET_TYPE_SQI:
		*pzAttributeLen = sizeof(INT32);
		SONY_Get_SSI_SQI(pSonyDemod, &SSI, &SQI);
		i4Temp = (INT32)SQI;
        mcSHOW_DBG_MSG3(("SQI = %d\r\n", SQI));				
		x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);		
		break;
	case TUNER_GET_TYPE_SSI:
		*pzAttributeLen = sizeof(INT32);
		SONY_Get_SSI_SQI(pSonyDemod, &SSI, &SQI);
		i4Temp = (INT32)SSI;
        mcSHOW_DBG_MSG3(("SSI = %d\r\n", SSI));		
		x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);	
		break;
    default:
        mcSHOW_DBG_MSG3(("unknown: e_get_type = %d\r\n", e_get_type));
        break;
    }

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);

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
INT32 DVBT2_TunerSetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_SET_TYPE_T  e_set_type,
                                        const VOID *pvAttribute, SIZE_T zAttributeLen)
{

   UCHAR ucData = *(UCHAR *)pvAttribute;
   UCHAR *pucData = (UCHAR *)pvAttribute;

    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
    //add driver implementation code here

    switch (e_set_type)
    {
    case TUNER_SET_TYPE_TS_TRISTATE:
        if (ucData == BOOL_FALSE) {
            mcSHOW_DBG_MSG(("[DVBT2] SetTSPath : pass-through\n"));
            //Enable TS output
            //if call sony demod tune function,TS will output automatically!!
         	SONY_SetTsPath(pSonyDemod, 1);
            ucData = 0;
        }
        else if (ucData == BOOL_TRUE) {
            mcSHOW_DBG_MSG(("[DVBT2] SetTSPath : tri-state\n"));
            //Set TS output to high-Z
            SONY_Sleep(pSonyDemod);
        }
        break;

    case TUNER_SET_TYPE_TS_SP:
#if  1		
        if (ucData == SP_PARALLEL) {
            mcSHOW_DBG_MSG(("!!!no support setattr[DVBT2] SetTSInterface : parallel\n"));
            //SONY_PISetTSInterface(pSonyDemod,TRUE);
        }
        else if (ucData == SP_SERIAL) {
            mcSHOW_DBG_MSG(("!!!no support setattr[DVBT2] SetTSInterface : serial\n"));
            //SONY_PISetTSInterface(pSonyDemod,FALSE);
        }
		
#else
        mcSHOW_DBG_MSG(("do nothing if MW call this!!\n"));
#endif
        break;
   case TUNER_SET_TYPE_CI_CARD_TYPE:
   	mcSHOW_DBG_MSG(("DVBT_TunerSetAttribute: TUNER_SET_TYPE_CI_CARD_TYPE\n"));
        if (pucData[0] == TUNER_CI_CARD_TYPE_1) {
            mcSHOW_DBG_MSG(("TUNER_SET_TYPE_CI_CARD_TYPE: TUNER_CI_CARD_TYPE_1, para = %d\n", pucData[1]));
        }	
	break;		
    default:
        Printf("unknown: set Attribute:%d\r\n",e_set_type);
        break;
    }

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);

    return DRVAPI_TUNER_OK;
   
}	
#ifdef fcADD_DIAG_INFO
INT32 DVBT2_TunerSetDiagCmd(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CUSTOM_TUNER_SET_TYPE_T e_tuner_set_type, VOID* pv_set_info, SIZE_T z_size)
{
    INT32   retSts = RMR_OK;
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerSetDiagCmd()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);

    return retSts;
}
INT32 DVBT2_TunerGetDiagInfo(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CUSTOM_TUNER_GET_TYPE_T e_tuner_get_type, VOID* pv_get_info, SIZE_T* pz_size)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerGetDiagInfo()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
    return 0;
}
#endif
//-----------------------------------------------------------------------------
// public functions
//-----------------------------------------------------------------------------


void PD_DVBT2_Register_LTDIS_Fct(PTD_CTX ptTDCtx)
{
    TUNER_DEVICE_CTX_T*		pTunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;
	
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerClose 	= DVBT2_TunerClose;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerAcq 	= DVBT2_TunerAcq;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGerSync 	= DVBT2_TunerGetSync;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignal 	= DVBT2_TunerGetSignal;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLevel = DVBT2_TunerGetSignalLevel;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLeveldBm = DVBT2_TunerGetSignalLeveldBm;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalPER = DVBT2_TunerGetSignalPER;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalSNR = DVBT2_TunerGetSignalSNR;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetMpgFmt 	= DVBT2_TunerSetMpgFmt;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetVer 	= DVBT2_TunerGetVer;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerNimTest 	= DVBT2_TunerNimTest;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetRegSetting = DVBT2_TunerSetRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerShowRegSetting = DVBT2_TunerShowRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerTestI2C 	= DVBT2_TunerTestI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerCommand 	= DVBT2_TunerCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDtdCommand 	= DVBT2_TunerDVBT2Command;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDetachI2C 	= DVBT2_TunerDetachI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDisc 	= DVBT2_TunerDisc;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetAttribute= DVBT2_TunerSetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute= DVBT2_TunerGetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerBypassI2C = DVBT2_TunerBypassI2C;
#ifdef fcADD_DIAG_INFO
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetDiagCmd	= DVBT2_TunerSetDiagCmd;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetDiagInfo = DVBT2_TunerGetDiagInfo;
#endif
#if defined(PD_REMAP_TER_DIG) || defined(PD_REMAP_CAB_DIG) || defined(PD_REMAP_SAT_DIG) || defined(PD_NO_USE_REMAP)
	pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute2 =  DVBT2_TunerGetAttribute2;
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
INT32 DVBT2_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* pptTDSpecificCtx)
{
//    ITUNER_CTX_T *tTunerCtx;
	


    //create pdctx used in this file, you also can add any declaration you want in pdctx
    pPdCtxDVBT2 = (DVBT2_PD_CTX_T *) x_mem_calloc(1, sizeof(DVBT2_PD_CTX_T));
    if (pPdCtxDVBT2 == NULL)
    {
        Printf("DVB_PD_CTX_T malloc fail\n");
        return (DRVAPI_TUNER_ERROR);
    }
	
    pSonyDemod = (DEMOD_CXD2820_CTX_T *) x_mem_calloc(1,sizeof(DEMOD_CXD2820_CTX_T));
	/* Init Disconnect Status */
	pPdCtxDVBT2->fgDisStatus = FALSE;

    //Create a semaphore for low level demod PI access protection
    if (x_sema_create(&pPdCtxDVBT2->hHalLock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        Printf("DVB x_sema_create(pPdCtxDVBT2->hHalLock) Fail!\n");
        return (DRVAPI_TUNER_ERROR);
    }
    x_sema_unlock(pPdCtxDVBT2->hHalLock);

    //register LTDIS APIs to HAL
    PD_DVBT2_Register_LTDIS_Fct(ptTDCtx);

    /* demod instance initialization (to do only once) */

    /* tuner initialization */
#if 1  // liuqu changed,20091215 for test!!!
     ITunerRegistration(((TUNER_DEVICE_CTX_T*)ptTDCtx)->tunerType);
     printf("[LEO]DVBT2 GET tuner type = %d\n",((TUNER_DEVICE_CTX_T*)ptTDCtx)->tunerType);
#endif
    ITuner_Init(ITunerGetCtx());

//demod initialize
	vResetDemod(100);

    if( SONY_Initilize(pSonyDemod) == DRVAPI_TUNER_OK) 
    	{
         printf("Sony demod DVBT2_Initilize OK!\n");
    	}
   else
   	{
         printf("Sony demod DVBT2_Initilize Fail!\n");
   	}
//	cmd_init();
//	Waitms(100);
    /*------------------------------------------------------------------------------
     Setup / Initialisation
    ------------------------------------------------------------------------------*/
    

/**demod initilize finish*****************************************/

#ifdef DVBT2_ENABLE_MONITOR
    if (mcSEMA_CREATE(&pPdCtxDVBT2->t_escape_mon_suspend, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_DRVERR_MSG(("EN_DVBT2_TunerOpen (mcSEMA_CREATE): ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }

    pPdCtxDVBT2->fgBreakMonitor = TRUE;//init
    pPdCtxDVBT2->fgPIMoniStatus = FALSE; //init
    pPdCtxDVBT2->t_ScanMode = CONN_MODE_UNKNOWN;  //init
//    PD_StartMonitorEngine(pPdCtxDVBT2);  //start monitor engine
#endif 

    return (DRVAPI_TUNER_OK);
}



