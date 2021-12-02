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
#include "pd_mn88471_dvbt2.h"
#include "drvapi_tuner.h"
#include "UtilAPI.h"
#include "pod_if.h"
#include "drvcust_if.h"
#include "eeprom_if.h"
#include "pi_dvbt_if.h"

#include "u_drv_cust.h"
#include "ctrl_bus.h"
#include "x_gpio.h"

#include "spi_if.h"

#include "tuner_api.h"







//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define DVBT2_LOCK_TIME_OUT_CNT  3 //XF

#define DVBT2_TUNER_CONN_TIME_THREHOLD   140
#define DVBT2_DEMOD_CONN_TIME_THREHOLD   200


#define SUPPORT_DVBT     1

#define fcRFAGC_RSSI_AD_NUM    3

EXTERN UINT32 PDWNC_ReadServoADCChannelValue(UINT32 u4Channel);

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
//static DEMOD_CXD2820_CTX_T *pSonyDemod;

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

//----------------------------------------------------------------------------- 
/*
 *  MT5135_GetReg
 *  This function get demod register.

 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pu1Buffer  Pointer of data buffer.
 *  @param  u2ByteCount  Total bytes to be written.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 MT5135_GetReg(UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
// temp5391, static UINT8 DVBT_GetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{

		while(u2ByteCount--)
		{
			if(-1 == SPI_DemodSingleRead(pu1Buffer, (UINT32)u2RegAddr))
			{
				mcSHOW_HW_MSG(("SPI Read Demod %d fail!\n", u2RegAddr));

				return 1;
			}
			pu1Buffer++;
			u2RegAddr++;
		}

		return 0;

}


//----------------------------------------------------------------------------- 
/*
 *  MT5135_SetReg
 *  This function set demod register.

 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pu1Buffer  Pointer of data buffer.
 *  @param  u2ByteCount  Total bytes to be written.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 MT5135_SetReg(UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
// temp5391, static UINT8 DVBT_GetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{

		while(u2ByteCount--)
		{
			if(-1 == SPI_DemodSingleWrite(pu1Buffer, (UINT32)u2RegAddr))
			{
				mcSHOW_HW_MSG(("SPI Write Demod %d fail!\n", u2RegAddr));

				return 1;
			}
			pu1Buffer++;
			u2RegAddr++;
		}

		return 0;

}

UINT16 MT5135_GetRSSI(void)
{
	UINT8 ucTemp,ucTemp1;
	UINT16 RfAgcRssi;
	UINT16 RSSI;
	MT5135_GetReg(0xEE0, &ucTemp, 1);
    mcSET_BIT(ucTemp, 7);
    mcSET_BIT(ucTemp, 6);
    MT5135_SetReg(0xEE0, &ucTemp, 1);
    MT5135_GetReg(0xEE5, &ucTemp, 1);
    MT5135_GetReg(0xEE6, &ucTemp1, 1);
    MT5135_GetReg( 0xEE5, &ucTemp, 1);
    RfAgcRssi = ((UINT16)ucTemp)<<2 | (ucTemp1&0x03);
#if 0
	if(RfAgcRssi > 680 )
		RSSI = 4000;
	else if(RfAgcRssi > 480)
		RSSI = 2710;
	else if(RfAgcRssi > 390)
		RSSI = 2170;
	else if(RfAgcRssi > 330)
		RSSI = 1840;
	else if(RfAgcRssi > 310)
		RSSI = 1750;
	else if(RfAgcRssi > 280)
		RSSI = 1630;
	else if(RfAgcRssi > 240)
		RSSI = 1400;
	else if(RfAgcRssi > 210)
		RSSI = 1160;
	else if(RfAgcRssi > 180)
		RSSI = 1010;
	else if(RfAgcRssi > 160)
		RSSI = 880;
	else 
		RSSI = 820;
#endif
	if(RfAgcRssi > 760 )
		RSSI =4340;
	else if(RfAgcRssi > 540)
		RSSI = 2710;
	else if(RfAgcRssi > 440)
		RSSI = 2170;
	else if(RfAgcRssi > 390)
		RSSI = 1970;
	else if(RfAgcRssi > 350)
		RSSI = 1840;
	else if(RfAgcRssi > 330)
		RSSI = 1750;
	else if(RfAgcRssi > 305)
		RSSI = 1630;
	else if(RfAgcRssi > 280)
		RSSI = 1400;
	else if(RfAgcRssi > 245)
		RSSI = 1160;
	else if(RfAgcRssi > 215)
		RSSI = 1010;
	else if(RfAgcRssi > 180)
		RSSI = 1010;	
	else if(RfAgcRssi > 145)
		RSSI = 820;
	else 
		RSSI = 0;

//	mcPRINTF(" RfAgcRssi :   %u\n",RfAgcRssi);
	return RSSI;

}

STATIC VOID vResetDemod(UINT16 u2DelayTime)
{
	UINT32 u4RstPinNum = 23;

    // Config GPIO to reset demod
    mcPRINTF("vReset 3rdParty Demod\n");
 //   if (DRVCUST_OptQuery(e3rdPartyDemodResetGpio, &u4RstPinNum) == 0)
    {
        GPIO_SetOut(u4RstPinNum, 0);
        mcDELAY_MS(u2DelayTime);
        GPIO_SetOut(u4RstPinNum, 1);
        mcDELAY_MS(20); // Delay 20 ms for CR[DTV00030459]. Or it will init fail in rel image
        mcPRINTF(" Reset 3rdParty Demod GPIO=0x%x, 0->1, %dms\r\n",u4RstPinNum, u2DelayTime);
    }
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
        mcPRINTF("Failed on Tuner_SetFreq(%d)", Freq);
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
    //SONY_Sleep(pSonyDemod);
    tu_api_shut_down();

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
        mcSHOW_DBG_MSG(("[DVBT2] SetTSInterface : parallel\n"));
    }
    else
    {
        mcSHOW_DBG_MSG(("[DVBT2] SetTSInterface : serial\n"));
    }

   //SONY_PISetTSInterface(pSonyDemod, (UINT8) fgParallel);
   tu_api_set_output_mode ( (tu_output_mode_t)fgParallel);
   
    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
}


/*non implement*/
void DVBT2_SetTSPath(BOOL fgEnable)
{

#if 0
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
   // UINT32  ii = 0;
    //UINT32  u4TimeOut = DVBT2_LOCK_TIME_OUT_CNT;   // 500MS  //150MS    (XF)
    tu_rx_status_t LockStatus;
    tu_return_t   result;
    UINT32  u4TickCnt, u4ConnTime;
//    UINT8   bw;	
	tu_chan_setup_t tParams;
//	PARAM_SETFREQ_T SetFreqPara;

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
       tParams.bandwidth = BANDWIDTH_6MHZ;
    break;
    case BW_7_MHz:
        mcSHOW_DBG_MSG(("Set BW = 7 MHz\n"));
        tParams.bandwidth = BANDWIDTH_7MHZ;
        break;
    case BW_8_MHz:
    default:
        mcSHOW_DBG_MSG(("Set BW = 8 MHz\n"));
       tParams.bandwidth = BANDWIDTH_8MHZ;
        break;
    }

	tParams.frequency = freq;
	tParams.ppt = PILOT_PATTERN_7;
    tParams.PLP_coderate = CODERATE_3DIV4;
    tParams.guard = GUARD_1DIV32;
    tParams.fftmode = FFTMODE_2K;
    tParams.carrier = CARRIER_MODE_EXTENDED;
    tParams.plp_id = 0;
    tParams.mirror = MIRROR_NO;
    tParams.hierarchy = HIERARCHY_NONE;
    tParams.priority = PRIORITY_HIGH;
	tParams.tune_mode = TUNE_AUTO;
	demod.myCommonAttr->tunerFreqRF = freq;
#if 0	
	if (e_conn_type == TUNER_CONN_TYPE_TER_DIG)   //DVBT

//if ((pt_tuner_info->e_dvb_t_t2_mode != TUNER_DVB_T) &&(pt_tuner_info->e_dvb_t_t2_mode != TUNER_DVB_T2))
 	{

		if(pt_tuner_info->e_dvb_t_t2_mode == TUNER_DVB_T2)	
		{
			  tParams.receiverType = RECEIVER_MODE_T2;
			  tParams.carrier = CARRIER_MODE_UNKNOWN;
			  tParams.mirror = MIRROR_UNKNOWN;
			  tParams.ppt = PILOT_PATTERN_UNKNOWN;
			  tParams.guard = GUARD_UNKNOWN;

			  result = tu_api_set_receiver_mode(tParams.receiverType, tParams.bandwidth);
			  mcSHOW_DBG_MSG(("Result is: %s \n",result == tue_RETURN_ok?"Rx mode set OK":"Rx mode NOT set OK!"));
			  if (result == tue_RETURN_ok)
			  {
				  result = tu_api_tune_request( &tParams );
				  
			  }

		  }
		  else if(pt_tuner_info->e_dvb_t_t2_mode == TUNER_DVB_T)
		  {
			  if (pt_tuner_info->e_hierarchy_priority == DVBT_HIR_PRIORITY_LOW)
				  tParams.priority = PRIORITY_LOW;
			  else
				  tParams.priority = PRIORITY_HIGH;
				  
			  tParams.guard = GUARD_UNKNOWN;
			  tParams.fftmode = FFTMODE_UNKNOWN;
				  
			  tParams.receiverType = RECEIVER_MODE_T;
			  result = tu_api_set_receiver_mode(tParams.receiverType, tParams.bandwidth);
			  mcSHOW_DBG_MSG(("Result is: %s \n",result == tue_RETURN_ok?"Rx mode set OK":"Rx mode NOT set OK!"));
			  if (result == tue_RETURN_ok)
			  {
				  result = tu_api_tune_request( &tParams );
				  
			  }

		  }
		  else
  		  {
     	  	 		
			//first try DVBT	
			if (pt_tuner_info->e_hierarchy_priority == DVBT_HIR_PRIORITY_LOW)
			 	tParams.priority = PRIORITY_LOW;
			else
				tParams.priority = PRIORITY_HIGH;
	
       		tParams.receiverType = RECEIVER_MODE_T;
			tParams.guard = GUARD_UNKNOWN;
			tParams.fftmode = FFTMODE_UNKNOWN;
			result = tu_api_set_receiver_mode(tParams.receiverType, tParams.bandwidth);
       		mcSHOW_DBG_MSG(("Result is: %s \n",result == tue_RETURN_ok?"Rx mode set OK":"Rx mode NOT set OK!"));
	        if (result == tue_RETURN_ok)
	        {
				result = tu_api_tune_request( &tParams );
            }
          		
           	if (result !=tue_RETURN_ok) 
           	{
	         //second try DVBT-2
                tParams.receiverType = RECEIVER_MODE_T2;
				tParams.carrier = CARRIER_MODE_UNKNOWN;
				tParams.mirror = MIRROR_UNKNOWN;
				tParams.ppt = PILOT_PATTERN_UNKNOWN;
				tParams.guard = GUARD_UNKNOWN;
						
				result = tu_api_set_receiver_mode(tParams.receiverType, tParams.bandwidth);
				mcSHOW_DBG_MSG(("Result is: %s \n",result == tue_RETURN_ok?"Rx mode set OK":"Rx mode NOT set OK!"));
				if (result == tue_RETURN_ok)
				{
					result = tu_api_tune_request( &tParams );

				}

                if (result != tue_RETURN_ok)
       	        {
                   	mcPRINTF("Can not Tune at any T/T2 Signal\n");
               	}
           	}
        
  		  }
		}
	    else  if (e_conn_type == TUNER_CONN_TYPE_CAB_DIG)    //DVBC
        {            		
		      tParams.receiverType = RECEIVER_MODE_C;
			  tParams.constellation = CONSTELLATION_UNKNOWN;
			  result = tu_api_set_receiver_mode(tParams.receiverType, tParams.bandwidth);
			  mcSHOW_DBG_MSG(("Result is: %s \n",result == tue_RETURN_ok?"Rx mode set OK":"Rx mode NOT set OK!"));
			  if (result == tue_RETURN_ok)
			  {
				  result = tu_api_tune_request( &tParams );
			  }
           	  if (result != tue_RETURN_ok)
           	  {
           	      mcSHOW_DBG_MSG(("Can not Tune at DVBC Signal\n"));
           	  }		   
         }
         else
         {
           	mcSHOW_USER_MSG(("Unknown e_conn_type  = 0X%03x\n",e_conn_type));
         }
#endif

	mcSHOW_DBG_MSG1(("dvb_t_t2_mode = %d {0:AUTO_T_T2,	1:DVBT,   2:DVBT2,	else:UNKNOWN} \n", pt_tuner_info->e_dvb_t_t2_mode));
	mcSHOW_DBG_MSG1(("priority = %d      {0;UNKNOWN,   	1:NONE,   2:HIGH,   3:LOW}\n", pt_tuner_info->e_hierarchy_priority));

	// if (pt_tuner_info->e_dvb_t_t2_mode == TUNER_DVB_AUTO_T_T2)
	if ((pt_tuner_info->e_dvb_t_t2_mode != TUNER_DVB_T) &&(pt_tuner_info->e_dvb_t_t2_mode != TUNER_DVB_T2))
		{
			if (e_conn_type == TUNER_CONN_TYPE_TER_DIG)	//DVBT
			{
				//first  try DVBT
				if (pt_tuner_info->e_hierarchy_priority == DVBT_HIR_PRIORITY_LOW)
					tParams.priority = PRIORITY_LOW;
				else
					tParams.priority = PRIORITY_HIGH;
					
				tParams.guard = GUARD_UNKNOWN;
				tParams.fftmode = FFTMODE_UNKNOWN;
					
				tParams.receiverType = RECEIVER_MODE_T;
				result = tu_api_set_receiver_mode(tParams.receiverType, tParams.bandwidth);
				mcSHOW_DBG_MSG(("Result is: %s \n",result == tue_RETURN_ok?"Rx mode set OK":"Rx mode NOT set OK!"));
				if (result == tue_RETURN_ok)
				{
					result = tu_api_tune_request( &tParams );
					
				}
					
				if (result !=tue_RETURN_ok) 
				{

					mcSHOW_DBG_MSG1(("DVBT signal unlock!!\n"));
					//second try DVBT-2
					tParams.receiverType = RECEIVER_MODE_T2;
					tParams.carrier = CARRIER_MODE_UNKNOWN;
					tParams.mirror = MIRROR_UNKNOWN;
					tParams.ppt = PILOT_PATTERN_UNKNOWN;
					tParams.guard = GUARD_UNKNOWN;
							
					result = tu_api_set_receiver_mode(tParams.receiverType, tParams.bandwidth);
					mcSHOW_DBG_MSG(("Result is: %s \n",result == tue_RETURN_ok?"Rx mode set OK":"Rx mode NOT set OK!"));
					if (result == tue_RETURN_ok)
					{
						result = tu_api_tune_request( &tParams );
								
					}
				}
		  }
		  else	if (e_conn_type == TUNER_CONN_TYPE_CAB_DIG)    //DVBC
		  { 				
		      tParams.receiverType = RECEIVER_MODE_C;
			  tParams.constellation = CONSTELLATION_UNKNOWN;
			  result = tu_api_set_receiver_mode(tParams.receiverType, tParams.bandwidth);
			  mcSHOW_DBG_MSG(("Result is: %s \n",result == tue_RETURN_ok?"Rx mode set OK":"Rx mode NOT set OK!"));
			  if (result == tue_RETURN_ok)
			  {
				  result = tu_api_tune_request( &tParams );
			  }

				
			  if (result != tue_RETURN_ok)
			  {
				  mcSHOW_DBG_MSG(("Can not Tune at DVBC Signal\n"));
			  } 	   
		 }
		 else
		 {
			mcSHOW_USER_MSG(("Unknown e_conn_type  = 0X%03x\n",e_conn_type));
		 }
		}
		else if (pt_tuner_info->e_dvb_t_t2_mode == TUNER_DVB_T2) 
		{
			if (e_conn_type == TUNER_CONN_TYPE_TER_DIG)   //DVBT
			{
				
				tParams.receiverType = RECEIVER_MODE_T2;
				tParams.carrier = CARRIER_MODE_UNKNOWN;
				tParams.mirror = MIRROR_UNKNOWN;
				tParams.ppt = PILOT_PATTERN_UNKNOWN;
				tParams.guard = GUARD_UNKNOWN;
				
				result = tu_api_set_receiver_mode(tParams.receiverType, tParams.bandwidth);
				mcSHOW_DBG_MSG(("Result is: %s \n",result == tue_RETURN_ok?"Rx mode set OK":"Rx mode NOT set OK!"));
				if (result == tue_RETURN_ok)
				{
					result = tu_api_tune_request( &tParams );
					
				}
	
			}
			else
			{
			   mcSHOW_USER_MSG(("Error TUNER CONN TYPE =%d!",e_conn_type));
			}
	   }
		else if(pt_tuner_info->e_dvb_t_t2_mode == TUNER_DVB_T)
		{
		   if (e_conn_type == TUNER_CONN_TYPE_TER_DIG)	 //DVBT
			 {
				
			   	if (pt_tuner_info->e_hierarchy_priority == DVBT_HIR_PRIORITY_LOW)
				   tParams.priority = PRIORITY_LOW;
			   	else
				   tParams.priority = PRIORITY_HIGH;
					   
				tParams.guard = GUARD_UNKNOWN;
				tParams.fftmode = FFTMODE_UNKNOWN;
					   
				tParams.receiverType = RECEIVER_MODE_T;
				result = tu_api_set_receiver_mode(tParams.receiverType, tParams.bandwidth);
				mcSHOW_DBG_MSG(("Result is: %s \n",result == tue_RETURN_ok?"Rx mode set OK":"Rx mode NOT set OK!"));
				if (result == tue_RETURN_ok)
				{
				   result = tu_api_tune_request( &tParams );
					   
				}
			}
		   else
			{
					mcSHOW_USER_MSG(("Error TUNER CONN TYPE =%d!",e_conn_type));
			}
	   }
		else
		{
			mcSHOW_USER_MSG(("Unknown e_dvb_t_t2_mode = %d\n",pt_tuner_info->e_dvb_t_t2_mode));
		}

  
	LockStatus = tu_api_get_syn_status();

	
    mcSHOW_DBG_MSG1(("Freq = %dKhz\n", tParams.frequency));
	mcSHOW_DBG_MSG1(("bandwidth = %d   {0:BANDWIDTH_8MHZ,  1:BANDWIDTH_7MHZ  2:BANDWIDTH_6MHZ}\n", tParams.bandwidth));
//    mcPRINTF("LockStatus = %d  {0:lock,            else :unlock}\n", LockStatus);


    //Check tuner conncection time
    u4ConnTime = (os_get_sys_tick()-u4TickCnt) * x_os_drv_get_tick_period();
    mcSHOW_DBG_MSG1(("Panasonic demod connection time = %dms\n", u4ConnTime));
    if ((u4ConnTime > DVBT2_TUNER_CONN_TIME_THREHOLD) && (fgChannelScan))
	{
	    mcSHOW_DBG_MSG(("EU Cable Driver ATP Fail: DVBT2 tuner connection time = %dms, too long!!\n", u4ConnTime));
	}

    u4TickCnt = os_get_sys_tick();

       	
  
     if (LockStatus == tue_RX_STATUS_locked)
    {
		if(demod.myCommonAttr->receiverType == RECEIVER_MODE_T2)
		{
			mcSHOW_DBG_MSG1(("DVBT2 signal lock!!\n"));
		}	
		else if(demod.myCommonAttr->receiverType == RECEIVER_MODE_T)
		{
			mcSHOW_DBG_MSG1(("DVBT signal lock!!\n"));
		}
		else 
		{
			mcSHOW_DBG_MSG1(("DVBC signal lock!!\n"));
		}

		mcSHOW_DRVAPI_MSG(( "setting parallel mode\n" ));
		tu_api_set_output_mode(OUTPUT_MODE_PARALLEL_NORMAL);

    }
	else 
	{
		if(demod.myCommonAttr->receiverType == RECEIVER_MODE_T2)
		{
			mcSHOW_DBG_MSG1(("DVBT2 signal unlock!!\n"));
		}

		else if(demod.myCommonAttr->receiverType == RECEIVER_MODE_T)
		{
			mcSHOW_DBG_MSG1(("DVBT signal unlock!!\n"));
		}
		else 
		{
			mcSHOW_DBG_MSG1(("DVBC signal unlock!!\n"));
		}
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

	if(LockStatus == tue_RX_STATUS_locked)
		return 1; 
	else
		return 0;//if lock return 1, unlock return 0
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
    tu_rx_status_t  LockStatus ;

    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
    //add driver implementation code here
//    dvb_demod_CheckTSLock(pSonyDemod->cxd2820.pDemod, &LockStatus);
	 LockStatus = tu_api_get_syn_status();//add by SONY AEC 20100412

    if (LockStatus ==tue_RX_STATUS_locked )
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
    if (tu_api_shut_down() != tue_RETURN_ok){
        mcSHOW_USER_MSG(("Failed on tu_api_shut_down()\n"));
    }

#ifdef DVBT2_ENABLE_MONITOR
    SetBreakMonitor(pPdCtxDVBT2);
#endif

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);

	/* set disconnect status */
	pPdCtxDVBT2->fgDisStatus = TRUE;
}



/*<<<<<<<<< Need to do further implementation <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

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
	tu_chan_setup_t channelInfo;   
   mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

    //add driver implementation code here
    _pSignal->Frequency  = (S32)(demod.myCommonAttr->tunerFreqRF);
    _pSignal->SymbolRate = 0;
    _pSignal->QAMSize = 0;
    _pSignal->u1DVBT_T2_indicator = TUNER_DVB_AUTO_T_T2;

	tu_api_channel_info ( &channelInfo );
	switch(demod.myCommonAttr->receiverType)
	{
		case RECEIVER_MODE_C:
			switch(channelInfo.constellation)
			{
				case CONSTELLATION_QAM16:
					_pSignal->QAMSize = 16;
					break;
					
				case CONSTELLATION_QAM32:
					_pSignal->QAMSize = 32;
					break;	
				case CONSTELLATION_QAM64:
					_pSignal->QAMSize = 64;
					break;

				case CONSTELLATION_QAM128:
					_pSignal->QAMSize = 128;
					break;

				case CONSTELLATION_QAM256:
					_pSignal->QAMSize = 256;
					break;

				default:
					_pSignal->QAMSize = 16;
					break;
			}
			
			_pSignal->SymbolRate = channelInfo.symb_rate.significand>>channelInfo.symb_rate.precision;
			mcSHOW_DBG_MSG(("DVBC_TunerGetSignal: SymbolRate = %d (Khz/s)\n", _pSignal->SymbolRate));
			break;

		case RECEIVER_MODE_T:
	        _pSignal->u1DVBT_T2_indicator = TUNER_DVB_T;
			if((channelInfo.hierarchy == HIERARCHY_NONE) ||(channelInfo.hierarchy == HIERARCHY_UNKNOWN))
					_pSignal->e_hierarchy_priority = 1;
			else
			{
				if(channelInfo.priority == PRIORITY_HIGH)
					_pSignal->e_hierarchy_priority = 2;
				else if(channelInfo.priority == PRIORITY_LOW)
					_pSignal->e_hierarchy_priority = 3;
				else
					_pSignal->e_hierarchy_priority = 2;					
			}
			switch (channelInfo.hierarchy)
			{
				case HIERARCHY_NONE:
					_pSignal->sHierInfo.eAlpha = ALPHA_0;
					break;
				case HIERARCHY_ALPHA1:
					_pSignal->sHierInfo.eAlpha = ALPHA_1;
					break;
				case HIERARCHY_ALPHA2:
					_pSignal->sHierInfo.eAlpha = ALPHA_2;
					break;
				case HIERARCHY_ALPHA4:
					_pSignal->sHierInfo.eAlpha = ALPHA_4;
					break;
				default:
					_pSignal->sHierInfo.eAlpha = ALPHA_0;
					break;					
			}
			switch (channelInfo.bandwidth)
			{
				case BANDWIDTH_8MHZ:
					_pSignal->e_bandwidth = BW_8_MHz;		
					break;

				case BANDWIDTH_7MHZ:
					_pSignal->e_bandwidth = BW_7_MHz;					
					break;

				case BANDWIDTH_6MHZ:
					_pSignal->e_bandwidth = BW_6_MHz;					
					break;

				default:
					_pSignal->e_bandwidth = BW_UNKNOWN;
					break;
						
			}
			

			mcSHOW_DBG_MSG(("DVBT_TunerGetSignal: PRIORITY = %d (1:None, 2:HP, 3:LP)\n", _pSignal->e_hierarchy_priority));
			mcSHOW_DBG_MSG(("DVBT_TunerGetSignal: Alpha = %d (3: a=4)\n", _pSignal->sHierInfo.eAlpha));
			mcSHOW_DBG_MSG(("DVBT_TunerGetSignal: e_bandwidth = %d (1:BW_6_MHz,2:BW_7_MHz,3:BW_8_MHz,)\n", _pSignal->e_bandwidth));

			break;

		case RECEIVER_MODE_T2:
	        _pSignal->u1DVBT_T2_indicator = TUNER_DVB_T2 ;

			break;
			
		default:
			mcSHOW_DBG_MSG(("system unknown = %d \n",demod.myCommonAttr->receiverType));			
	      break;  	
	}

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
//	u16_t rf_agc_val = 0;
	SSI_t SSI_data;
	UINT16 RfAgcRssi;
	
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
    //add driver implementation code here
//	rf_agc_val = PDWNC_ReadServoADCChannelValue(fcRFAGC_RSSI_AD_NUM);
	RfAgcRssi = MT5135_GetRSSI();
//	mcPRINTF("RfAgcRssi = %u\n",RfAgcRssi);
	if ( tu_api_get_SSI ( RfAgcRssi, &SSI_data ) != tue_RETURN_ok)
	{
		mcSHOW_DBG_MSG(("Error retrieving SSI data.\n"));
	}
	else
	{
		mcSHOW_DBG_MSG1(("\nRF signal power = %d\nSSI = %d\nSQI = %d\r\n", SSI_data.signal_power, SSI_data.SSI, SSI_data.SQI));
	}

    u1Ret = (UINT8)SSI_data.SSI;
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

UINT32 DVBT2_TunerGetSignalBER(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{

    UINT32 u4_ret = 0;
	MNdouble_t NewBER;
	s8_t T2ber_st[][32] =
	{
		"Test",
		"Simple Data PLP BER after LDPC"
	};
	tu_ber_mode_t BER_state = BER_LDPC_PER;	
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
	NewBER = tu_api_get_BER(BER_state);
//	mcSHOW_DBG_MSG1(("%s : %u * 2^-%u\n",T2ber_st[BER_state],NewBER.significand,NewBER.precision));
	mcSHOW_DBG_MSG1(("%s : %u.%u)\n",T2ber_st[BER_state],NewBER.integer, NewBER.decimals));
	
    u4_ret = (100000*NewBER.significand)>>NewBER.precision;
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
  char * version = "Panasonic Demod driver version: 2010 v1.6";

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
    //mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerSetRegSetting()\n"));
	tu_api_set_regval( (MNReg_set_t)ucRegSet, ucRegAddr, ucRegValue);
    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
}
#if 0
INT16 DVBT2_TunerShowRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx,UCHAR ucRegSet, UCHAR ucRegAddr)
{
	INT16 i4Ret = 0;
	mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerShowRegSetting()\n"));
  //  i4Ret = tu_api_get_Regval(ucRegSet, ucRegAddr);

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
	return i4Ret;
}
#endif
void DVBT2_TunerTestI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerTestI2C()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
}



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
			tu_api_shut_down();
			//SONY_ChipPowerDown(pSonyDemod);
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
	tu_chan_database_t current_chan_database;
	tu_chan_setup_t current_chan_settings;
	tu_ber_mode_t BER_state;
	tu_return_t result;
	tu_chan_plp_info_t current_plp_info;
	MNdouble_t NewBER;		/* Current BER READING */
    MNdouble_t NewC_N;		/* Current C/N READING */ 
	#if 0
		s8_t om_st[][16] =
		{
			"Serial",
			"Parallel-normal",
			"Parallel-burst",
			"unknown"
		};
	#endif
	
		s8_t gi_st[][8] =
		{
			"1/32",
			"1/16",
			"1/8",
			"1/4",
			"1/128",
			"19/128",
			"19/256",
			"unknown"
		};
	
		s8_t tm_st[][8] =
		{
			"1K",
			"2K",
			"4K",
			"8K",
			"16K",
			"32K",
			"64K",
			"unknown"
		};
	
		s8_t md_st[][8] =
		{
			"BPSK",
			"QPSK",
			"PSK8",
			"QAM16",
			"QAM32",
			"QAM64",
			"QAM128",
			"QAM256",
			"QAM512",
			"QAM1024",
			"unknown"
		};
	#if 0
		s8_t bw_st[][8] =
		{
			"8MHz",
			"7MHz",
#ifndef ONLY_7_8_MHZ
			"6MHz",
			"5MHz",
			"1.7MHz",		/*Correct? See spec V0.20 pgs 13 vs 32! */
#endif
			"unknown"		/*Correct? See spec V0.20 pgs 13 vs 32! */
		};
	#endif
		s8_t cr_st[][8] =
		{
			"1/2",
			"3/5",
			"2/3",
			"3/4",
			"4/5",
			"5/6",
			"7/8",
			"unknown"
		};
	#if 0	
		s8_t cm_st[][9] =
		{
			"normal",
			"extended",
			"unknown"
		};

		s8_t sp_st[][9] =
		{
			"normal",
			"inverted",
			"unknown"
		};
	
		s8_t rx_st[][8] =
		{
			"DVB-C",
			"DVB-T",
			"DVB-T2",
			"unknown"
		};
		#endif	
		s8_t pp_st[][8] =
		{
			"1",
			"2",
			"3",
			"4",
			"5",
			"6",
			"7",
			"8",
			"unknown"
		};
	
		s8_t pr_st[][8] =
		{
			"Low",
			"High",
			"unknown"
		};
	
		s8_t T2ber_st[][32] =
		{
			"Test",
			"Simple Data PLP BER after LDPC"
		};
	#if 0
		s8_t Tber_st[][32] =
		{
			"Simple BER after Viterbi",
			"Simple PER after RS"
		};
	
		s8_t lock_st[][18] =
		{
			"locked",
			"not_locked",
			"RESET",					/* All modes (T/T2/C) */
			"TUNING",					/* All modes (T/T2/C) */
			"AGC_CAPTURE",				/* All modes (T/T2/C) */
			"T2_P1_DETECT", 			/* T2 */
			"T2_GI_SEARCH",
			"T2_CLOCK_DETECT",
			"T2_TRANSIT",
			"T2_FREQ_DETECT",
			"T2_L1_PRE_DETECT",
			"T2_L1_POST_DETECT",
			"T_TM_SRCH",
			"T_CLOCK_DETECT",
			"T_TRANSIT",
			"T_FREQ_DETECT",
			"T_FRAME_DETECT",
			"C_SYM_SRCH",
			"C_TRANSIT",
			"C_TM_SRCH",
			"C_TM_DETECT",
			"C_FRAME_DETECT"
		};
	#endif
		char hier_st[][18] =
			{
			"HIERARCHY_NONE",
			"HIERARCHY_ALPHA1",
			"HIERARCHY_ALPHA2",
			"HIERARCHY_ALPHA4",
			"HIERARCHY_UNKNOWN" 					   /*	 = MN_UNKNOWN */
			};

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

		result = tu_api_get_tune_parameters ( &current_chan_database );
		current_chan_settings = current_chan_database.setup;	/*copy database channel */
		mcPRINTF("Result is %s \n\n",result == tue_RETURN_ok?"Parameters retrieved":"Parameters NOT retrieved!");
		if (result == tue_RETURN_ok && demod.myCommonAttr->receiverType == RECEIVER_MODE_T2)
		{
			mcPRINTF("Freq: %d\n, L1 Code Rate (Not PLP C/R): %s\n, GI: %s\n, L1 Modul (Not PLP modul.): %s\n, FFT Mode: %s\n, Pilot Pattern: %s\n",current_chan_settings.frequency, cr_st[current_chan_settings.L1_coderate],
				   gi_st[current_chan_settings.guard], md_st[current_chan_settings.constellation],
				   tm_st[current_chan_settings.fftmode],pp_st[current_chan_settings.ppt] );
		}
		if (result == tue_RETURN_ok && demod.myCommonAttr->receiverType == RECEIVER_MODE_T)
		{
			mcPRINTF("Freq: %d\n, Hierarchy: %s\n, HP Code Rate: %s\n, LP Code Rate: %s\n, Priority Selection: %s\n, GI: %s, Modul: %s\n, FFT Mode: %s\n",current_chan_settings.frequency,hier_st[current_chan_settings.hierarchy], cr_st[current_chan_settings.HP_coderate], cr_st[current_chan_settings.LP_coderate], pr_st[current_chan_settings.priority], 
				   gi_st[current_chan_settings.guard], md_st[current_chan_settings.constellation],
				   tm_st[current_chan_settings.fftmode] );
		}
		if (result == tue_RETURN_ok && demod.myCommonAttr->receiverType == RECEIVER_MODE_C)
		{

			mcPRINTF("Freq: %d\n, Modul: %s\n, Symbol Rate: %u.%u kbaud\n",current_chan_settings.frequency, md_st[current_chan_settings.constellation],current_chan_settings.symb_rate.integer, current_chan_settings.symb_rate.decimals);
		}

		if (demod.myCommonAttr->receiverType == RECEIVER_MODE_T2)
		{
			result = tu_api_plp_info ( &current_plp_info );
            mcPRINTF("Result is %s \n",result == tue_RETURN_ok?"PLP parameters retrieved":"PLP parameters NOT retrieved!");
            mcPRINTF("Number of PLPs: %d\n",current_plp_info.num_plps);
            mcPRINTF("Current PLP ID: %d\n",current_plp_info.current_plp_id);
            mcPRINTF("Current PLP number: %d\n",current_plp_info.current_plp_num);
		}	
		if (i4Argc > 1)
			BER_state = (tu_ber_mode_t)StrToInt(aszArgv[1]);
		else
			BER_state = BER_LDPC_PER;
		NewC_N = tu_api_get_CN();
		mcPRINTF("CN: %u.%u \n",NewC_N.integer,NewC_N.decimals);
		NewBER = tu_api_get_BER(BER_state);
// 		mcPRINTF ("%s : %u * 2^-%u\n",T2ber_st[BER_state],NewBER.significand,NewBER.precision);
        mcPRINTF ("%s : %u.%u)\n",T2ber_st[BER_state],NewBER.integer, NewBER.decimals);

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
    	 tu_output_mode_t fgParallel = (tu_output_mode_t)StrToInt(aszArgv[1]);	  
		 
		tu_api_set_output_mode(fgParallel);
	
    	}
	break;
	
     case 'g':
	{
	   if (i4Argc >2)
	     {
//	   UINT8 configId = (UINT8)StrToInt(aszArgv[1]);	
//	   UINT8 value = (UINT8)StrToInt(aszArgv[2]);	
	
        //  dvb_demod_SetConfig( pSonyDemod->cxd2820.pDemod,(sony_dvb_demod_config_id_t) configId,(int32_t )value);
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
		
	MNReg_set_t regset = (MNReg_set_t)StrToInt(aszArgv[1]);
	
   	INT16 regAddr =(UINT8)StrToInt(aszArgv[2]);
	INT16 rdata = 0xff;
	   	
	 rdata = tu_api_get_regval(regset, regAddr);
	  
	 mcSHOW_USER_MSG(("Read retset 0x%02x ,regAddr 0x%02x = 0x%02x\n",regset,regAddr,rdata));
         }
	 else
	 	{
               mcSHOW_USER_MSG(("nim.dtd r retset subAddress \n"));
	 	}
	}
       break;
	   
   case 'w':
   	{
		if (i4Argc >3)
	    {
			MNReg_set_t regset = (MNReg_set_t)StrToInt(aszArgv[1]);;	
		   	INT16 regAddr =(UINT8)StrToInt(aszArgv[2]);
			INT16 wdata = (UINT8)StrToInt(aszArgv[3]);
		    if(tu_api_set_regval(  regset, regAddr, wdata ) != wdata) 
	       		mcSHOW_USER_MSG(("write failed regset 0x%02x ,regAddr 0x%02x = 0x%02x\n",regset,regAddr,wdata));
		               
	   	}
		  
	    else
		{
	        mcSHOW_USER_MSG(("nim.dtd w regset subAddress data \n"));
		}
	}	
    break;
#endif	  
      case 'l':	  
		tu_api_shut_down();
	  mcSHOW_USER_MSG(("CLI make MN88471 sleep \n"));
	  break;
    default:
        mcPRINTF("Cmd: \n");
        mcPRINTF("\t s                       : status\n");
	 mcPRINTF("\t e                       : enable/disable panasonic log\n");	
	 mcPRINTF("\t t                       : set TS interface parrel(1) /serial(0)\n");
	 mcPRINTF("\t r                       : read demod registers\n");
	 mcPRINTF("\t w                       : write demod registers\n");
	 mcPRINTF("\t g                       : set panasonic demod config\n");
	 mcPRINTF("\t l                       : make panasonic demod sleep\n");
	 
	 
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
//           i4Temp = SONY_GetBER(pSonyDemod);

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
            tsFmt = tu_api_get_ts_fmt();
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
    INT32 i4Temp = 0;
	MNdouble_t BER;	
	UINT16 RfAgcRssi;
	SSI_t SSI_data;
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
		BER = tu_api_get_BER(BER_LDPC_PER);
    	i4Temp = (BER.significand*100000)>>BER.precision;
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
		     switch ( demod.myCommonAttr->bandwidth)
			 {
				case BANDWIDTH_6MHZ:  pAttri->ui4_fine_tune_delta_freq = 536000; break; //6MHz
				case BANDWIDTH_7MHZ:  pAttri->ui4_fine_tune_delta_freq = 625000; break; //7MHz
				case BANDWIDTH_8MHZ:  pAttri->ui4_fine_tune_delta_freq = 714000; break; //8MHz
				
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
	case TUNER_GET_TYPE_SQI:
		RfAgcRssi = MT5135_GetRSSI();
		if ( tu_api_get_SSI ( RfAgcRssi, &SSI_data ) != tue_RETURN_ok)
		{
			mcSHOW_DBG_MSG(("Error retrieving SSI data.\n"));
		}
		else
		{
			mcSHOW_DBG_MSG(("\nRF signal power = %d\nSSI = %d\nSQI = %d\r\n", SSI_data.signal_power, SSI_data.SSI, SSI_data.SQI));
		}

		*pzAttributeLen = sizeof(INT32);
		i4Temp = (INT32)(SSI_data.SQI);
		mcPRINTF("DVB SQI: %d\n",i4Temp);	
		x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);		
		break;
	case TUNER_GET_TYPE_SSI:
		RfAgcRssi = MT5135_GetRSSI();
		if ( tu_api_get_SSI ( RfAgcRssi, &SSI_data ) != tue_RETURN_ok)
		{
			mcSHOW_DBG_MSG(("Error retrieving SSI data.\n"));
		}
		else
		{
			mcSHOW_DBG_MSG(("\nRF signal power = %d\nSSI = %d\nSQI = %d\r\n", SSI_data.signal_power, SSI_data.SSI, SSI_data.SQI));
		}

		*pzAttributeLen = sizeof(INT32);
		i4Temp = (INT32)(SSI_data.SSI);


		mcPRINTF("DVB SSI: %d\n",i4Temp);
		x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);
		break;
		
	case TUNER_GET_TYPE_TS_FMT:
        {
            TS_FMT_T tsFmt;
            *pzAttributeLen = sizeof(TS_FMT_T);
            tsFmt = tu_api_get_ts_fmt( );
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
            ucData = 0;	

        }
        else if (ucData == BOOL_TRUE) {


            mcSHOW_DBG_MSG(("[DVBT2] SetTSPath : tri-state\n"));
            //Set TS output to high-Z
    //        SONY_Sleep(pSonyDemod);
    		tu_api_shut_down();
			ucData = 0;			
        }
        break;

    case TUNER_SET_TYPE_TS_SP:
#if  1		
        if (ucData == SP_PARALLEL) {
            mcSHOW_DBG_MSG(("[DVBT2] SetTSInterface : parallel\n"));
           // SONY_PISetTSInterface(pSonyDemod,TRUE);
           tu_api_set_output_mode(OUTPUT_MODE_PARALLEL_NORMAL);
        }
        else if (ucData == SP_SERIAL) {
            mcSHOW_DBG_MSG(("[DVBT2] SetTSInterface : serial\n"));
			tu_api_set_output_mode(OUTPUT_MODE_SERIAL);
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
//    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerShowRegSetting = DVBT2_TunerShowRegSetting;
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
//	tu_chan_setup_t tParams;


    //create pdctx used in this file, you also can add any declaration you want in pdctx
    pPdCtxDVBT2 = (DVBT2_PD_CTX_T *) x_mem_calloc(1, sizeof(DVBT2_PD_CTX_T));
    if (pPdCtxDVBT2 == NULL)
    {
        Printf("DVB_PD_CTX_T malloc fail\n");
        return (DRVAPI_TUNER_ERROR);
    }
	

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
     mcPRINTF("[LEO]DVBT2 GET tuner type = %d\n",((TUNER_DEVICE_CTX_T*)ptTDCtx)->tunerType);
#endif
    ITuner_Init(ITunerGetCtx());

//demod initialize
	vResetDemod(20);

    if( tu_api_init(&tunerPanasonicENV57U04D8F) == tue_RETURN_ok) 
    	{
         mcPRINTF("\nPanasonic demod DVBT2_Initilize OK!\n");
    	}
   else
   	{
         mcPRINTF("\nPanasonic demod DVBT2_Initilize Fail!\n");
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



