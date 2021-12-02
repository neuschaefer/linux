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
 * $RCSfile: pd_anana_glue.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "Pd_AnaApi.h"
#include "pd_atd_glue.h"
#include "pd_atdna_glue.h"
//#include "pd_atdeu_if.h"
#include "UtilAPI.h"
#include "TDMgr.h"

//#include "PI_Def_dbg.h"

#define PLUGGABLE_DEMOD_DESIGN //Pluto Kang 090320

#ifdef fcADD_DIAG_INFO
    #ifdef CC_MT5381
#include "u_drv_cust.h"
    #else
//#include "d_5371_cust.h"
    #endif
#endif
//#include "pd_atdna_picommonapi.h"
#include "nptv_drvif.h"
#include "x_stl_lib.h"
#include "eepdef.h"

#ifdef TUNER_ANALOG_NEED_INIT_TUNER
#include "rf_tuner_api.h"
#include "dtd_if.h"  //here we need tuner context only
#endif
#ifdef PLUGGABLE_DEMOD_DESIGN
#include "tuner_interface_if.h"
#include "pd_tuner_type.h"
//#include "ctrl_bus.h"
#endif

#include "pi_demod_atd.h"
#include "pd_tuner_type.h"
#include "pi_demod_atdna.h"
#include "drvcust_if.h"

// ZY, 091222
#include "api_srch_if.h"
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#define fcDUMMY_ANA_TUNER       (0 && fcINTERNAL_TEST)
#define fcTEST_I2C_DETACH       ((0 && fcINTERNAL_TEST) || 0)
#define fcMULTI_REG_RW_DEBUG    1

#if 0  //Jackson remove fcADD_CUST_IF to fix zero used for undefined preprocessing identifier
#define fcADD_CUST_IF           (1 && (fcTUNER_TYPE == cANA_TUNER_ALL) && defined(CC_MT5381))
#endif

#if !defined(CC_MTAL) && !defined(CC_SDAL) //linux platform
//Pluto
#define ANAUS_ENABLE_MONITOR
#else

#endif

#define fcADD_TVD_SYNC         // Pause TVD before TunerAcq complete, or will show "No Signal"
#define fcADD_AV_SYNC           1
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define cMONITOR_PERIOD_MS      500

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//#if fcADD_I2C_BUSY_WAIT
#ifdef fcADD_I2C_BUSY_WAIT //fix zero used for undefined preprocessing identifier
#define mcTUNER_BUSY_WAIT(u2Delay)      vTunerBusyWait(u2Delay)
#else
#define mcTUNER_BUSY_WAIT(u2Delay)
#endif

#define mcWRITE_EXTRA_MSG(u1Max)
#define mcTUNER_WRITE_FUNC(psCtx, u2RegAddr, au1Data, u1ByteCount)  \
    fgTunerPartWrite(au1Data[0], au1Data + 1, u1ByteCount - 1)
#define mcTUNER_IF_WRITE_FUNC(psCtx, u2RegAddr, au1Data, u1ByteCount)  \
    fgIFPartWriteComm(au1Data[0], au1Data[1], au1Data + 2, u1ByteCount - 2)

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifdef ANAUS_ENABLE_MONITOR
static BOOL             fgMonitorThreadRunning;
static THREAD_T         t_monitor_thread;
static MUTEX_T			t_escape_mon_suspend;
#endif
//static BOOL		fgIsDisconnected = TRUE;
//static UINT8 u1MonitorLoopCnt = 0;
static UINT8 fgAtdMonitorRet = FALSE;
#if defined(CC_SONY_GA)
static UINT8 fgAtdAFTFailCnt = 0;
#endif
//-----------------------------------------------------------------------------
// global variables
#ifdef DTD_INCLUDE
EXTERN UINT8 gfgIFTrap;
#endif
BOOL fgEnableATP=FALSE;//Flag to check whether show  Driver ATP Auto Test log
//-----------------------------------------------------------------------------
ATD_CTX_T*    psAtdnaDemodCtx;

#if fcTIMING_MEASURE
UINT32  u2TickStart;//jackson: todo, move to a control block
static UINT16  u2Delay = 0;
#endif
#if fcTEST_I2C_DETACH
EXTERN UINT8   fgEnI2CDetach;
#endif
//EXTERN UINT8   _SubSysIdx;
EXTERN UINT8   _bSetChannelCnt;


//BOOL  fgUpdCurFreq ;
//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------
#ifdef TUNER_ANALOG_NEED_INIT_TUNER
EXTERN TUNER_API_FCT_TBL_T *pTuner_API_Fct;
EXTERN VOID Tuner_Register_Fct(TUNER_API_FCT_TBL_T *ptuner_Fct);
EXTERN VOID *GetUSTunerCtx(VOID);
RF_TUNER_CTX_T  *psTunerCtx;
#endif

#ifdef PLUGGABLE_DEMOD_DESIGN
static ITUNER_CTX_T *pTunerCtx;
 //#if (fcTUNER_TYPE == cITUNER_ALL)
//EXTERN VOID UV1336_Anana_PiCommonAPI_Register_Fct(ANANA_PICOMMONAPI_FCT_TBL_T *pUv1336_Fct);
 //#endif
#endif
//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern UINT8 bGetSignalType(UINT8 bPath);
extern UINT32 PDWNC_ReadServoADCChannelValue(UINT32 u4Channel);
extern void AUD_WaitDspFlush(UINT8 u1DecId);

//extern UINT8 ATDNA_Monitor(ATD_CTX_T *psDemodCtx, UINT32 *wFreqInKHz, x_break_fct _BreakFct, void *pvArg);
//extern UINT8 ATDNA_SearchNearbyFreq(ATD_CTX_T *psDemodCtx, UINT32 *wFreqInKHz, x_break_fct _BreakFct,
//                                 void *pvArg, UINT32 u4FreqBDKHz);

//extern ANANA_PICOMMONAPI_FCT_TBL_T *pAnana_PiCommonAPI_Fct;

//#if (fcTUNER_TYPE != cDTD_TUNER_ALL)
//EXTERN VOID Tuner_Anana_PiCommonAPI_Register_Fct(ANANA_PICOMMONAPI_FCT_TBL_T *pcom_Fct);
//#else
//EXTERN VOID Tuner_Anana_CommonAPI_Register_Fct(ANANA_PICOMMONAPI_FCT_TBL_T *pcom_Fct, UINT8 tunerType);
//EXTERN UINT8 GetTunerType(VOID);
//#endif


/***********************************************************************/
/*              External declarations                                  */
/***********************************************************************/
#if defined(CC_DRIVER_DEMO) || defined(CC_LINUX_KERNEL)
#define _Tvd_NotifyTVSig(u1Path, u1SigState)
#else
VOID _Tvd_NotifyTVSig(UINT8 u1Path, UINT8 u1SigState);
#endif

/***********************************************************************/
/*              Private (static) Function Prototypes                   */
/***********************************************************************/
VOID Tuner_AtdNA_Register_LTDIS_Fct(PTD_CTX ptTDCtx);
VOID AtdNA_TunerAtdCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        INT32 i4Argc, const CHAR** aszArgv);


//-----------------------------------------------------------------------------
/*
 *  TunerNullBreak
 *  Used for ATD command : Scarchnearby command
 *  @param  void.
 *  @retval   always FALSE.
 */
//-----------------------------------------------------------------------------
static INT32 TunerNullBreak(VOID* pArg)
{
	UNUSED(pArg);
    return (FALSE);
}
BOOL AnaPi_TunerGetVIFLock(VOID)
{
    return FALSE;
}


//-----------------------------------------------------------------------------
/*
 *  AnaPd_EnterTunerAcq
 *  TvdHoldMode : On
 *  @param  void.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
VOID AnaPd_EnterTunerAcq(VOID)
{
//#if fcADD_TVD_SYNC
#ifdef fcADD_TVD_SYNC //fix zero used for undefined preprocessing identifier
    vTvdHoldMode(SV_ON);
    mcSHOW_DBG_MSG(("vTvdHoldMode(SV_ON) when enter TunerAcq\n"));
#endif
}

//-----------------------------------------------------------------------------
/*
 *  AnaPd_ExitTunerAcq
 *  TvdHoldMode : Off
 *  @param  void.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
VOID AnaPd_ExitTunerAcq(BOOL fgLocked)
{
//#if fcADD_AV_NOTIFY
#ifdef fcADD_AV_NOTIFY //fix zero used for undefined preprocessing identifier
    AUD_AtunerSigStableNotify(fgLocked);
#endif
//#if fcADD_TVD_SYNC
#ifdef fcADD_TVD_SYNC  //fix zero used for undefined preprocessing identifier
    vTvdHoldMode(SV_OFF);
    mcSHOW_DBG_MSG(("vTvdHoldMode(SV_OFF) when exit TunerAcq\n"));
#endif
}

//-----------------------------------------------------------------------------
/*
 *  AnaPd_ChangeChannel
 *  Do Channel Change
 *  @param  u4CurFreqency
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
#ifndef CC_MT5391_AUD_3_DECODER
VOID AnaPd_ChangeChannel(UINT32 u4CurFreqency)
{
#if fcADD_AV_SYNC
STATIC UINT32 u4PreFreqency = 0;
BOOL fgSameChannel;

    fgSameChannel = (u4PreFreqency == u4CurFreqency);
    u4PreFreqency = u4CurFreqency;

    //AUD_DualDecAtvContinuousPlay(fgSameChannel);
    if (!fgSameChannel)
        AUD_WaitDspFlush(1);
#endif
}
#endif

//-----------------------------------------------------------------------------
/*
 *  vTunerBusyWait
 *  Do Channel Change
 *  @param  u2WaitTimeus 	waiting time
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
//#if fcADD_I2C_BUSY_WAIT
#if 0   //zero used for undefined preprocessing identifier
VOID vTunerBusyWait(UINT32 u2WaitTimeus)
{
HAL_TIME_T  rCurrent1, rCurrent2;
HAL_TIME_T  rDelta;
UINT32      ii;

    HAL_GetTime(&rCurrent1);
    for (ii = 0; ii < u2WaitTimeus; ii ++)
    {
        HAL_GetTime(&rCurrent2);
        HAL_GetDeltaTime(&rDelta, &rCurrent1, &rCurrent2);
        if (rDelta.u4Micros >= u2WaitTimeus)
            break;
    }
// rDelta.u4Micros = 5000 <--> ii = 1200
    mcSHOW_DBG_MSG(("vTunerBusyWait: %d (%d)\n", rDelta.u4Micros, ii));
}
#endif

#ifdef ANAUS_ENABLE_MONITOR
static BOOL SetBreakMonitor(ATD_PD_CTX_T *psAtdPdCtx)
{
    UINT8 u1Cnt=0;

    psAtdPdCtx->fgBreakMonitor = TRUE; //set to break monitor

    if(psAtdPdCtx->fgPIMoniStatus){  //PI really doing PI Monitor
        while(psAtdPdCtx->fgPIMoniStatus){
            //wait PI Monitor really finish break
            if(u1Cnt>250){  //in case of deadlock
                break;
            }
            u1Cnt++;
            mcSHOW_DBG_MSG(("ANA US PD wait break PI monitor finish. u1Cnt=%d\r\n",u1Cnt));
            mcDELAY_MS(10);
        }
    }

    return TRUE;
}
static void ResumeMonitor(ATD_PD_CTX_T *psAtdPdCtx)
{
    #if defined(CC_SONY_GA)
    fgAtdAFTFailCnt = 0;
    fgAtdMonitorRet = FALSE;
    #endif
    psAtdPdCtx->fgBreakMonitor = FALSE;
    mcMUTEX_UNLOCK(t_escape_mon_suspend);
}
//-----------------------------------------------------------------------------
/*
 *  i4CheckNoticeBreak
 *  Break function in the monitor thread and searchnearby function
 *  @param  *pvArg	function argument.
 *  @retval   TRUE 	have a break notification.
 */
//-----------------------------------------------------------------------------
STATIC INT32 i4CheckNoticeBreak(VOID *pvArg)
{
    /* FE_MONI_TASK_T *psMoniTask = &(((PD_CTX_T *) pvArg)->sFeMoniTask); */
    //FE_MONI_TASK_T *psMoniTask = (FE_MONI_TASK_T *) pvArg;
    ATD_PD_CTX_T *psPdCtx = (ATD_PD_CTX_T *) pvArg;
    if(psPdCtx->fgBreakMonitor)
    {
        mcPRN_MSG_MONI(" [%s-%s = 1]\n", __FILE__, __FUNCTION__);
    }
    return (psPdCtx->fgBreakMonitor);
}

//-----------------------------------------------------------------------------
/*
 *  MonitorProcess
 *  Main Monitor thread ==> ISP
 *  @param  *pvArg	function argument.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
STATIC void MonitorProcess(VOID *pvArg)
{

    ATD_PD_CTX_T *psPdCtx = *(ATD_PD_CTX_T **) pvArg;

    #if defined(CC_SONY_GA)
    UINT8 ucData1, ucVpress;
    #endif

    mcSHOW_DBG_MSG(("MonitorProcess start\n"));
    while (fgMonitorThreadRunning)
    {
     //   if (fgIsDisconnected)
    	if (psPdCtx->fgBreakMonitor)
    	{
    		// After mcMUTEX_LOCK(t_escape_mon_suspend), monitor thread will be suspended until
    		// TunerAcq or TunerClose invoked.
            mcSHOW_DBG_MSG(("Entering 5391 mon_suspend\n"));
            mcMUTEX_LOCK(t_escape_mon_suspend);
 		    mcSHOW_DBG_MSG(("Escape 5391 mon_suspend\n"));
    	}
    	else
    	{
	    	   mcSEMA_LOCK_HAL(psPdCtx->hHalLock);
		       //Add Monitor ISP here
             psPdCtx->fgPIMoniStatus = TRUE; //doing PI monitor

            #if defined(CC_SONY_GA)
            if(fgAtdAFTFailCnt == 1)
            {                                
                //psAtdeuDemodCtx->u1PF = 0x02;
                /*psAtdnaDemodCtx->fgCheckCR = TRUE;
                if(ATDNA_GetCRLock(psAtdnaDemodCtx, TRUE))           
                {                  
                    mcSHOW_DBG_MSG(("CR lock one time, retry again!!!!!!!!!!!!!!!!!!!!\n"));
                    ATD_GetReg(psAtdnaDemodCtx, 0x935, &ucData1, 1);
                    ucData1 = ucData1 & 0xFE;
                    ATD_SetReg(psAtdnaDemodCtx, 0x935, &ucData1, 1);
                    ucData1 = ucData1 | 0x01;
                    ATD_SetReg(psAtdnaDemodCtx, 0x935, &ucData1, 1);
                                        
                    mcDELAY_MS(100);
                    if(ATDNA_GetCRLock(psAtdnaDemodCtx, TRUE))
                    {
                        mcSHOW_DBG_MSG(("Find new possible VIF!\n"));
                        ATD_SetSystem(psAtdnaDemodCtx, psPdCtx->u1SubSysIdx);
                        ATD_SetPF(psAtdnaDemodCtx, 0x01);
                        ATD_StartAcq(psAtdnaDemodCtx, FALSE);
                        fgAtdAFTFailCnt = 0;	
                    }
                }*/
                ucVpress = (IO_READ8(VIDEO_IN0_BASE, 0x81) & 0x10) >> 4;
                if(ucVpress == 0x01)
                {
                    ucData1 = 0x08;
                    ATD_SetReg(psAtdnaDemodCtx, 0x980, &ucData1, 1);
                    ucData1 = 0x04;                    
                    ATD_SetReg(psAtdnaDemodCtx, 0x9B6, &ucData1, 1);
                    ucData1 = 0x21;                    
                    ATD_SetReg(psAtdnaDemodCtx, 0x9B1, &ucData1, 1); 
                    psAtdnaDemodCtx->u1PF = 0x01;
                    ATD_SetNormalMode(psAtdnaDemodCtx);                    
                    mcSHOW_DBG_MSG(("VPress lock, restart AFT!!!!!!!!!!!!!!!!!!!!\n"));
                    fgAtdAFTFailCnt = 0;	
                }
            }

            if(fgAtdAFTFailCnt == 0){
                fgAtdMonitorRet = ATDNA_Monitor(psAtdnaDemodCtx, &psPdCtx->Frequency, i4CheckNoticeBreak, psPdCtx);          
            }

            if (fgAtdMonitorRet) 
            {
                ATD_Patch(psAtdnaDemodCtx);
                fgAtdAFTFailCnt = 0;
            }            
            else
            {
                if(fgAtdAFTFailCnt == 0)
                {                     
                    ucData1 = 0x00;
                    ATD_SetReg(psAtdnaDemodCtx, 0x980, &ucData1, 1);
                    ucData1 = 0x09;
                    ATD_SetReg(psAtdnaDemodCtx, 0x997, &ucData1, 1);       
                    //ucData1 = 0x26;                    
                    //ATD_SetReg(psAtdnaDemodCtx, 0x9B4, &ucData1, 1);
                    //ucData1 = 0xCC;                    
                    //ATD_SetReg(psAtdnaDemodCtx, 0x999, &ucData1, 1);
                    mcSHOW_DBG_MSG(("1st time monitor fail!\n"));                    
                    //ATD_SetFreq(psAtdnaDemodCtx, psPdCtx->Frequency, _fgAutoSearch);                    
                    //ATD_SetPF(psAtdnaDemodCtx, 0x02);
                    //ATD_StartAcq(psAtdnaDemodCtx, FALSE);                                        
                    fgAtdAFTFailCnt++;
                }
            } 
            #else
            // do ATDEU_Monitor every 500ms
            //u1MonitorLoopCnt++;
            //if (u1MonitorLoopCnt >= 5){
            //    u1MonitorLoopCnt = 0;
                fgAtdMonitorRet =
                    ATDNA_Monitor(psAtdnaDemodCtx, &psPdCtx->Frequency,
            		                    i4CheckNoticeBreak, psPdCtx);
            //}

            // do ATD_Patch every 100ms
            if (fgAtdMonitorRet) {
                ATD_Patch(psAtdnaDemodCtx);
            }
            #endif 
            psPdCtx->fgPIMoniStatus = FALSE; //finish doing PI monitor or breaked
            mcSHOW_DBG_MSG4(("===ANA_NA Monitor Thread ===\n"));
            mcSEMA_UNLOCK_HAL(psPdCtx->hHalLock);

            mcDELAY_MS(cMONITOR_PERIOD_MS);
        }
    }
    t_monitor_thread = NULL;
    mcSHOW_DBG_MSG(("MonitorProcess exit!\n"));
    mcTHREAD_DESTROY();
}

//-----------------------------------------------------------------------------
/*
 *  PD_StartMonitorEngine
 *  To create and start the monitor thread
 *  @param  *psAtvPdCtx	Pd handler.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
STATIC S32 PD_StartMonitorEngine(ATD_PD_CTX_T *psAtdPdCtx)
{
    // x_thread_create(HANDLE_T *ph_th_hdl, CHAR *ps_name, SIZE_T z_stack_size,
    //                 UINT8 ui1_pri, x_os_thread_main_fct pf_main_rtn,
    //                  SIZE_T z_arg_size, VOID *pv_arg);
    // Important: x_thread_create will copy pv_arg to its memory while not
    // directly use pv_arg. Therefore, if we want thread to share the same
    // context as caller, we need set pv_arg as "pointer of context".
    fgMonitorThreadRunning = TRUE;
    if (mcTHREAD_CREATE(&t_monitor_thread,
            "NA_ANA_MonThread",
             cTHREAD_STACK_SIZE, cTHREAD_PRIORITY,
            MonitorProcess, sizeof(void*), (VOID*)&psAtdPdCtx) != OSR_OK)
    {
        mcSHOW_DBG_MSG(("PD_StartMonitorEngine (mcTHREAD_CREATE): DRVAPI_TUNER_ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    mcSHOW_DRVAPI_MSG(("PD_StartMonitorEngine success\n"));
    return (DRVAPI_TUNER_OK);
}

//-----------------------------------------------------------------------------
/*
 *  PD_StopMonitorEngine
 *  To create and start the monitor thread
 *  @param  v0od.
 *  @retval   0 ==> Close successful.
 */
//-----------------------------------------------------------------------------
STATIC S32 PD_StopMonitorEngine(void)
{
//    fgMonitorThreadRunning = FALSE; //need set to false before mcMUTEX_UNLOCK(t_escape_mon_suspend);
    while (t_monitor_thread){
        mcDELAY_MS(10); // in ms
    }
    mcSHOW_DRVAPI_MSG(("ANA NA PD_StopMonitorEngine success\n"));
    return (DRVAPI_TUNER_OK);
}

#endif //ANAUS_ENABLE_MONITOR

#if 0
//-----------------------------------------------------------------------------
/*
 *  fgIFPartReadComm
 *  Read Tuner Part parameters
 *  @param  bAddr		The address we want to access
 *  @param  bSubAddr  	offset address
 *  @param  *pbData  	buffer point
 *  @param  u1TunerCtrlSize	read size
 *  @retval   TRUE : I2C access successful
 */
 //-----------------------------------------------------------------------------
STATIC BOOL fgIFPartReadComm(UINT8 bAddr, UINT8 bSubAddr, UINT8 *pbData, UINT8 u1TunerCtrlSize)
{
UINT8 i;
UINT16 u2ByteCnt;

//JWEI 2006/09/19
//advised from James Wei and modify @0924 BS Chen
#if 0 //jackson, PD_ATuner.h already removed
    mcTUNER_DETACH_I2C(TRUE);
#endif
    for (i = 0; i < TUNER_I2C_RETRY; i++)
    {
    //    u2ByteCnt = SIF_ReadNoSubAddr(SIF_NPTV_CLOCK, bAddr, pbData, u1TunerCtrlSize);
        //u2ByteCnt = SIF_NIMReadNoSubAddr(SIF_NPTV_CLOCK, bAddr, pbData, u1TunerCtrlSize);
        u2ByteCnt = SIF_TunerReadNoSubAddr(SIF_NPTV_CLOCK, bAddr, pbData, u1TunerCtrlSize);
        if (u2ByteCnt == u1TunerCtrlSize)
        {
#if 0 //jackson, PD_ATuner.h already removed
            mcTUNER_DETACH_I2C(FALSE);
#endif
            mcSHOW_DBG_MSG(("fgIFPartRead: %02X=%02X\n", bAddr, *pbData));
           return TRUE;
        }
    }

#if 0 //jackson, PD_ATuner.h already removed
    mcTUNER_DETACH_I2C(FALSE);
#endif
    mcSHOW_HW_MSG(("IF Part Read %02X fail!\n", bAddr));
    return FALSE;
}
#endif

#if 0
//-----------------------------------------------------------------------------
/*
 *  fgIFPartRead
 *  Read Tuner Part parameters
 *  @param  bAddr		The address we want to access
 *  @param  *pbData  	buffer point
 *  @retval   TRUE : I2C access successful
 */
 //-----------------------------------------------------------------------------
STATIC BOOL fgTunerPartRead(UINT8 bAddr, UINT8 *pbData)
{
UINT8 i;
#if fcADD_I2C_EXTRA_DELAY
//UINT16 u2ByteCnt;
#endif

//JWEI 2006/09/19
//advised from James Wei and modify @0924 BS Chen
    mcTUNER_DETACH_I2C(TRUE);
    mcTUNER_BUSY_WAIT(cTUNER_I2C_DELAY_US);
    for (i = 0; i < TUNER_I2C_RETRY; i++)
    {
    #if !fcADD_I2C_EXTRA_DELAY
        if ((fgHwI2CDataReadWithNoSubAddr(bAddr, 1, pbData, TUNER_I2C_OPTION)) == SV_TRUE)
    #else
        if ((SIF_TunerReadNoSubAddr(SIF_NPTV_CLOCK, bAddr, pbData, 1)) == 1)
    #endif
        {
            mcTUNER_DETACH_I2C(FALSE);
            mcSHOW_DBG_MSG(("fgTunerPartRead: %02X=%02X\n", bAddr, *pbData));
            return SV_TRUE;
        }
    }
    mcTUNER_DETACH_I2C(FALSE);
    mcSHOW_HW_MSG(("fgTunerPartRead: %02X Fail\n", bAddr));
    return SV_FALSE;
}
#endif

/************************************************************************
     Function : BOOL fgTunerPartWrite(UINT8 bAddr, UINT8 *prData )
  Description : Set successive FIVE bytes of Tuner part programming( Write Mode )
    Parameter :
                 bAddr:  Tuner I2c Device Address.
                *prData: Output Data Content Pointer
       Return : TRUE   : Successful
                FALSE  : Failure
        Notes : Default Tuner I2C Sub Address is 0x00.
I2C Timming Chart:
type          MSB  Bit6   Bit5  Bit4  Bit3  Bit2  Bit1 LSB  ACK
Addr Byte      1    1      0     0     0    MA1   MA0  R/W   A
Divider   DB1  0   N14    N13   N12   N11   N10   N9   N8    A
Divider   DB2 N7   N6     N5    N4    N3    N2    N1   N0    A
Ctrl Byte CB   1   CP     T2    T1    T0    RSA   RSB  OS    A
BW Byte   BB  P7   P6     P5    P4    P3    P2    P1   P0    A
Auxiliary AB  ATC  AL2    AL1   AL0    0     0     0    0    A
************************************************************************/
//-----------------------------------------------------------------------------
/*
 *  fgTunerPartWrite
 *  Read Tuner Part parameters
 *  @param  bAddr		The address we want to access
 *  @param  *pbData  	buffer point
 *  @param  u1TunerCtrlSize	read size
 *  @retval   TRUE : I2C access successful
 */
 //-----------------------------------------------------------------------------
#if 0
STATIC BOOL fgTunerPartWrite(UINT8 bAddr, UINT8 *pbData, UINT8 u1TunerCtrlSize)
{
UINT8 i;
#if fcADD_I2C_EXTRA_DELAY
//UINT16 u2ByteCnt;
#endif

    if (pbData == NULL)
        return SV_FALSE;

//JWEI 2006/09/19
//advised from James Wei and modify @0924 BS Chen
    mcTUNER_DETACH_I2C(TRUE);
    mcTUNER_BUSY_WAIT(cTUNER_I2C_DELAY_US);
    for (i = 0; i < TUNER_I2C_RETRY; i++)
    {
    #if !fcADD_I2C_EXTRA_DELAY
        if ((fgHwI2CDataWriteWithNoSubAddr(bAddr, u1TunerCtrlSize, pbData, TUNER_I2C_OPTION)) == SV_TRUE)
    #else
        if ((SIF_TunerWriteNoSubAddr(SIF_NPTV_CLOCK, bAddr, pbData, u1TunerCtrlSize)) == u1TunerCtrlSize)
    #endif
        {
            mcTUNER_DETACH_I2C(FALSE);
            mcSHOW_DBG_MSG(("fgTunerPartWrite (%d): %02X=%02X %02X %02X %02X %02X\n",
                            u1TunerCtrlSize, bAddr, pbData[0], pbData[1], pbData[2], pbData[3], pbData[4]));
            return SV_TRUE;
        }
    }
    mcTUNER_DETACH_I2C(FALSE);
    mcSHOW_HW_MSG(("fgTunerPartWrite (%d): %02X Fail\n", u1TunerCtrlSize, bAddr));
    return SV_FALSE;
}
#endif

#if 0
//-----------------------------------------------------------------------------
/*
 *  fgIFPartWriteComm
 *  Read Tuner Part parameters
 *  @param  bAddr		The address we want to access
 *  @param  bSubAddr  	offset address
 *  @param  *pbData  	buffer point
 *  @param  u1TunerCtrlSize	read size
 *  @retval   TRUE : I2C access successful
 */
 //-----------------------------------------------------------------------------
STATIC BOOL fgIFPartWriteComm(UINT8 bAddr, UINT8 bSubAddr, UINT8 *pbData, UINT8 u1TunerCtrlSize)
{
UINT8 ii;
UINT8 au1Data[cIF_DATA_SIZE];

    if (pbData == NULL)
        return (FALSE);

    if (u1TunerCtrlSize > cIF_DATA_SIZE)
        u1TunerCtrlSize = cIF_DATA_SIZE;
    for (ii = 0; ii < u1TunerCtrlSize; ii ++)
        au1Data[ii + 1] = pbData[ii];
    au1Data[0] = bSubAddr;
    u1TunerCtrlSize ++;
//JWEI 2006/09/19
//advised from James Wei and modify @0924 BS Chen
    mcTUNER_DETACH_I2C(TRUE);
    for (ii = 0; ii < TUNER_I2C_RETRY; ii ++)
    {
    //    if (SIF_Write(SIF_NPTV_CLOCK, bAddr, bSubAddr, pbData, 3) == 3)
        if (SIF_TunerWriteNoSubAddr(SIF_NPTV_CLOCK, bAddr, au1Data, u1TunerCtrlSize) == u1TunerCtrlSize)
        {
            mcTUNER_DETACH_I2C(FALSE);
            mcSHOW_DBG_MSG(("fgIFPartWrite: %02X.%02X=%02X %02X %02X\n",
                            bAddr, bSubAddr, pbData[0], pbData[1], pbData[2]));
            return SV_TRUE;
        }
    }
    mcTUNER_DETACH_I2C(FALSE);
    mcSHOW_HW_MSG(("IF Part Write %02X Fail!\n", bAddr));
    return SV_FALSE;
}
#endif

//#ifdef TARGET_5381
//#ifdef CC_MT5381

/***********************************************************************/
/*
 *  GetAnaConnMode
 *  Get Connection Mode
 *  @param      eMod:
			    MOD_PSK_8 --> Channel Change
			    MOD_UNKNOWN --> Scan
			    MOD_QAM_1024 --> Fine-tune (EU mode)
 *  @retval   connectmode
 */
 //-----------------------------------------------------------------------------
 /*
    u4AutoSearchType:
    0 --> Channel Change;
    1 --> Scan;     
    2 --> Fine-tune (EU mode);  
 */    
STATIC UINT8 GetAnaConnMode(UINT8 eMod)
{
    UINT8   u1ConnMode=0, ucData1;
    UINT32	u4AutoSearchType = 0;
    
    mcSHOW_DBG_MSG(("eMod = %d\n", eMod));   
    
    u4AutoSearchType = u4GetAutoSearchType();
    mcSHOW_DBG_MSG(("u4AutoSearchType = %d\n", u4AutoSearchType));   
    
    // ZY, 091222, u1ConnMode = u4AutoSearchType, except FM radio mode
    switch (u4AutoSearchType)
    {           	
        case ATV_CONN_MODE_CHAN_CHANGE:      
            u1ConnMode = ATV_CONN_MODE_CHAN_CHANGE;
            ITuner_OP(psAtdnaDemodCtx->pTCtx, itSetLNA, 0, &ucData1); 
            mcSHOW_DBG_MSG(( "Connection_type = channel change!\n"));
            break;
                	
        case ATV_CONN_MODE_CHAN_SCAN:                       
            u1ConnMode = ATV_CONN_MODE_CHAN_SCAN;
            ITuner_OP(psAtdnaDemodCtx->pTCtx, itSetLNA, 0, &ucData1);            
            mcSHOW_DBG_MSG(( "Connection_type = channel scan!\n"));
            break;
        
        case ATV_CONN_MODE_FINE_TUNE:                    
            u1ConnMode = ATV_CONN_MODE_FINE_TUNE;
            ITuner_OP(psAtdnaDemodCtx->pTCtx, itSetLNA, 0, &ucData1);             
            mcSHOW_DBG_MSG(( "Connection_type = fine tune!\n"));
            break; 
        default:
            break;
    }                    

    switch (eMod)
         {
/*         	
        case MOD_UNKNOWN:
        //_fgAutoSearch = TRUE;
            u1ConnMode = ATV_CONN_MODE_CHAN_SCAN;
            ITuner_OP(psAtdnaDemodCtx->pTCtx, itSetLNA, 0, &ucData1);
            mcSHOW_DBG_MSG(( "Mod_type = channel scan!\n"));
            break;

        case MOD_QAM_1024:
            //_fgAutoSearch = TRUE;
            u1ConnMode = ATV_CONN_MODE_FINE_TUNE;
            ITuner_OP(psAtdnaDemodCtx->pTCtx, itSetLNA, 0, &ucData1);
            mcSHOW_DBG_MSG(( "Mod_type = fine tune!\n"));
            break;

        case MOD_PSK_8:
            //_fgAutoSearch = FALSE;
            u1ConnMode = ATV_CONN_MODE_CHAN_CHANGE;
            ITuner_OP(psAtdnaDemodCtx->pTCtx, itSetLNA, 0, &ucData1);
            mcSHOW_DBG_MSG(( "Mod_type = channel change!\n"));
            break;
*/            
#ifndef CC_MT5360
        case MOD_FM_RADIO:
            //_fgAutoSearch = FALSE;
            u1ConnMode = ATV_CONN_MODE_FM_RADIO;
            ITuner_OP(psAtdnaDemodCtx->pTCtx, itSetLNA, 0, &ucData1);
            mcSHOW_DBG_MSG(( "Mod_type = FM radio!\n"));
            break;
#endif
        default:
            break;
         }

    return u1ConnMode;
    }

STATIC void SetFMRadio(UINT32  i4CurFreq)
    {
    UINT8 ucData1;
    //UINT32 u4Temp;

    //u4Temp = 0xffffffff;
    //IO_WRITE32(cRISC_TVD_BASE, 0x40C, u4Temp);
    ATD_ChipInit(psAtdnaDemodCtx);
    ATD_SetSystem(psAtdnaDemodCtx, MOD_PAL_BG);
    psAtdnaDemodCtx->u1PF = 0x00;
    ucData1 = 0x23;
    ATD_SetReg(psAtdnaDemodCtx, 0x930, &ucData1, 1);
    ucData1 = 0x6D;
    ATD_SetReg(psAtdnaDemodCtx, 0x9B7, &ucData1, 1);
    ucData1 = 0xC9;
    ATD_SetReg(psAtdnaDemodCtx, 0x999, &ucData1, 1);
    ucData1 = 0x00;
    ATD_SetReg(psAtdnaDemodCtx, 0x9D6, &ucData1, 1);
    ucData1 = 0x00;
    ATD_SetReg(psAtdnaDemodCtx, 0x9C2, &ucData1, 1);
    ucData1 = 0x20;
    ATD_SetReg(psAtdnaDemodCtx, 0x9C3, &ucData1, 1);
    ucData1 = 0x81;
    ATD_SetReg(psAtdnaDemodCtx, 0xA20, &ucData1, 1);
    ucData1 = 0xA1;
    ATD_SetReg(psAtdnaDemodCtx, 0xA21, &ucData1, 1);

    //SetBreakMonitor(psAtdPdCtx);
    //ATD_SetLNA(psAtdeuDemodCtx, TRUE);
    ITuner_OP(psAtdnaDemodCtx->pTCtx, itSetLNA, 0, &ucData1);
    //ATD_SetFreq(psAtdeuDemodCtx, i4CurFreq, FALSE);
    //ATD_StartAcq(psAtdeuDemodCtx, FALSE);

    mcSHOW_DBG_MSG((" Set FM radio at %d KHz!\n", i4CurFreq));

}

/***********************************************************************/

#if 0  //Jackson remove fcADD_CUST_IF to fix zero used for undefined preprocessing identifier
#if fcADD_CUST_IF
UINT32  u4TunerCtrlAnaForce = 0;

//-----------------------------------------------------------------------------
/*
 *  Analog_GetCtmrReq
 *  Get customer request
 *  @param      void
 *  @retval   tuner control word (Bit 0 monitor thread enable)
 */
 //-----------------------------------------------------------------------------
UINT32 Analog_GetCtmrReq(VOID)
{
UINT32  TunerCtrl;

    #ifdef CC_MT5381
    TunerCtrl = DRVCUST_OptGet(eTunerCtrlAna);
    #else
    TunerCtrl = DRVCUST_InitGet(eTunerCtrlAna);
    #endif
//    TunerCtrl = mcBIT(cCUST_REQ_ANA_0);
//    TunerCtrl = mcBIT(cCUST_REQ_ANA_2) | mcBIT(cCUST_REQ_ANA_3);  //JWEI 2007/06/07
    if (u4TunerCtrlAnaForce)
        TunerCtrl = u4TunerCtrlAnaForce;
    mcPRN_DBG_MSG("DRVCUST_OptGet(eTunerCtrlAna) = 0x%02X\n", TunerCtrl);

    return TunerCtrl;
}
#endif
#endif

//-----------------------------------------------------------------------------
// public functions
//-----------------------------------------------------------------------------

UINT32 AtdNA_GetNoiseLevel(void)
{
    return ATDPI_GetNoiseLevel(psAtdnaDemodCtx);
}

UINT16 AtdNA_GetIFAGC(void)
{
    return ATDPI_GetIFAGC(psAtdnaDemodCtx);
}

BOOL AtdNA_SetLNA(BOOL fgLNA)
{
    return ATDPI_SetLNA(psAtdnaDemodCtx, fgLNA);
}

//-----------------------------------------------------------------------------
/*
 *  AtdNA_TunerOpen
 *  Open analog tuner driver
 *  @param  ptTDCtx  a void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @param  ptTunerCfgInfo    tuner config information
 *  @param  pptTDSpecificCtx
 *  @retval   DRVAPI_TUNER_OK : Success.
 */
//-----------------------------------------------------------------------------
INT32 AtdNA_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo,
                     PTD_SPECIFIC_CTX* pptTDSpecificCtx)
{
    ATD_PD_CTX_T   *psAtdPdCtx;
    Printf("ATD_Initialize successful\n");

    Tuner_AtdNA_Register_LTDIS_Fct(ptTDCtx);

    /*create ATD PD CTX*/
    psAtdPdCtx = (ATD_PD_CTX_T *) mcALLOC_MEM(sizeof(ATD_PD_CTX_T));
    if (psAtdPdCtx == NULL)
    {
        return (DRVAPI_TUNER_ERROR);
    }
    *pptTDSpecificCtx = psAtdPdCtx;

    //psAtdPdCtx->ucTvSysGroupNum = sizeof(au4TvSysGroup) / sizeof(UINT32);
    //psAtdPdCtx->pu4TvSysGroup = au4TvSysGroup;
    //psAtdPdCtx->psTunerAnaAttr = &sTunerAnaAttribute;

    psAtdnaDemodCtx = ATD_DemodCtxCreate();
    if (psAtdnaDemodCtx == NULL)
    {
        return (DRVAPI_TUNER_ERROR);
    }
    //pTunerCtx = &(psDvbtDemodCtx->sTunerCtx);

    /*link demod ctx to pd ctx*/
    psAtdPdCtx->sAtdPiCtx = psAtdnaDemodCtx;

    //Printf("ATD_Initialize successful (1)\n");

    //090720, LC, move ITuner init b4 ATD init
#if 0
    ITunerRegistration(cITUNER_DONT_CARE);
#else
    ITunerRegistration(((TUNER_DEVICE_CTX_T*)ptTDCtx)->tunerType);
#endif
    pTunerCtx = ITunerGetCtx();
    if (pTunerCtx == NULL)
    {
        return (DRVAPI_TUNER_ERROR);
    }
    ITuner_Init(pTunerCtx); //todo: modify tuneri2caddr

    psAtdnaDemodCtx->pTCtx = pTunerCtx;
    psAtdnaDemodCtx->u1TunerType =((TUNER_DEVICE_CTX_T*)ptTDCtx)->tunerType;

    //Pluto Kang
    //ATD_ChipInit(psAtdnaDemodCtx);

    // Initialize ATD context & download FW (Pluto Kang don't load FW for MT5387)
    if (ATD_Initialize(psAtdnaDemodCtx, 0x92, FALSE)){
	Printf("ATD_Initialize failed\n");
	return (DRVAPI_TUNER_ERROR);
    }
    else
    {
    	Printf("ATD_Initialize successful\n");
    }
/*
    if( fcTUNER_TYPE == cLG_TDVWH810F ) // LG TDVWH810F
    {
    	ATD_SetTunerType(psAtdnaDemodCtx, 1);
        Printf("LG_TDVWH810F\n");
    }
    else if( fcTUNER_TYPE == cALPS_TEQE3 ) // ALPS TEQE3
    {
    	ATD_SetTunerType(psAtdnaDemodCtx, 2);
        Printf("ALPS_TEQE3\n");
    }
    else if( fcTUNER_TYPE == cALPS_TEQH3L01A ) // ALPS TEQH3
    {
    	ATD_SetTunerType(psAtdnaDemodCtx, 3);
        Printf("ALPS_TEQH3L01A\n");
    }
    else if( fcTUNER_TYPE == cALPS_TDAU4D01A ) // ALPS TDAU4D01
    {
    	ATD_SetTunerType(psAtdnaDemodCtx, 4);
        Printf("ALPS_TDAU4D01A\n");
    }

*/

#if 0
    //090319
#if 0
    ITunerRegistration(cITUNER_DONT_CARE);
#else
    ITunerRegistration(((TUNER_DEVICE_CTX_T*)ptTDCtx)->tunerType);
#endif
    pTunerCtx = ITunerGetCtx();
    ITuner_Init(pTunerCtx); //todo: modify tuneri2caddr

    psAtdnaDemodCtx->pTCtx = pTunerCtx;
    psAtdnaDemodCtx->u1TunerType =((TUNER_DEVICE_CTX_T*)ptTDCtx)->tunerType;
#endif

#if fcADD_HAL_LOCK
    if (mcSEMA_CREATE(&psAtdPdCtx->hHalLock, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_HW_MSG(("%s: mcSEMA_CREATE(hHalLock) Fail!\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);
    }
    mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
#endif

#if 0  //Jackson remove fcADD_CUST_IF to fix zero used for undefined preprocessing identifier
#if fcADD_CUST_IF
    psAtvPdCtx->u4DrvCustTunerCtrl = Analog_GetCtmrReq();
#endif
#endif

#if fcTUNE_NOT_START
    psAtdPdCtx->u1TunerAcqType = ACQ_ACTION_DEFAULT;
#endif

//#if !defined(CC_MTAL) && !defined(CC_SDAL)
#ifdef ANAUS_ENABLE_MONITOR
    if (mcSEMA_CREATE(&t_escape_mon_suspend, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_DBG_MSG(("MT5391_TunerOpen (mcSEMA_CREATE): ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }

    psAtdPdCtx->fgBreakMonitor = TRUE;//init
    psAtdPdCtx->fgPIMoniStatus = FALSE; //init
    // Start Monitor Engine
    PD_StartMonitorEngine(psAtdPdCtx);
#endif

    Printf("AtdNA_TunerOpen END\n");

    return (DRVAPI_TUNER_OK);
}

//-----------------------------------------------------------------------------
/*
 *  AtdNA_TunerClose
 *  To close analog tuner driver
 *  @param  ptTDCtx  a void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @retval   DRVAPI_TUNER_OK : Success.
 */
//-----------------------------------------------------------------------------
INT32 AtdNA_TunerClose(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{

#if fcENABLE_MONITOR || fcADD_HAL_LOCK
ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;
#endif

//    fgIsDisconnected = FALSE;
//#if !defined(CC_MTAL) && !defined(CC_SDAL)
#ifdef ANAUS_ENABLE_MONITOR
    psAtdPdCtx->fgBreakMonitor = TRUE;
    fgMonitorThreadRunning = FALSE; //need set to false before mcMUTEX_UNLOCK(t_escape_mon_suspend);
    mcMUTEX_UNLOCK(t_escape_mon_suspend);
    PD_StopMonitorEngine();
    mcMUTEX_DESTROY(t_escape_mon_suspend);
#endif

#if fcADD_HAL_LOCK
    if (mcSEMA_DESTROY(psAtdPdCtx->hHalLock) != OSR_OK)
    {
        mcSHOW_HW_MSG(("%s: mcSEMA_DESTROY(hHalLock) Fail!\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);
    }
#endif

    ATD_DemodCtxDestroy(psAtdnaDemodCtx);

    mcFREE_MEM(ptTDSpecificCtx);
    return (DRVAPI_TUNER_OK);
}

BOOL AtdNA_TunerAcq(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                    DRV_CONN_TYPE_T e_conn_type,
                    VOID* pv_conn_info,
                    SIZE_T z_conn_info_len,
                    x_break_fct _BreakFct,
                    VOID* pvArg)
{

ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;
INT32   i4CurFreq = cTUNER_DEFAULT_FREQ;
UINT32  u4Freq; //u4FreqBound;
UINT8   u1Path = SV_VP_MAX;
UINT8   u1ATVStatus = 0;
UINT8   ucData1; 
UINT8   u1ConnMode;
BOOL    fgLockSts = TRUE;
INT32   s4FreqOffset;
INT16   s2FreqOffset;
STATIC  UINT8 ScanedNo=0;//counter for scaned channel number
//UINT8   u1AnaMode=0;//Air or Cable
#ifdef CC_DALI2K10 //Philips DALI2K10 need switch RF_AGC
	UINT32 u4GpioNum;
#endif
    mcSHOW_DBG_MSG(("START AtdNA_TunerAcq\n"));
//Pluto Kang // Avoid Build Error
//u1AnaMode += 0;

//UINT8   u1ScanType = eTUNER_UNI_AUTO_SCAN; //variable "u1ScanType" was declared but never referenced

#ifdef ANAUS_ENABLE_MONITOR
    SetBreakMonitor(psAtdPdCtx);
#endif

    mcSEMA_LOCK_HAL(psAtdPdCtx->hHalLock);  // Avoid re-entry

//	psAtvPdCtx->fgBreakMonitor = TRUE;

    _bSetChannelCnt = 0x40;
    if ((e_conn_type != TUNER_CONN_TYPE_CAB_ANA) &&
        (e_conn_type != TUNER_CONN_TYPE_TER_ANA))
    {
    //    mcVERIFY_ASSERT(0);
        mcSHOW_HW_MSG(("%s: Wrong e_conn_type (%d)!\n", __FUNCTION__, e_conn_type));

        mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
        return FALSE;

    }

    if ((e_conn_type == TUNER_CONN_TYPE_CAB_ANA))
    {
        u4Freq = ((TUNER_CAB_ANA_TUNE_INFO_T *) pv_conn_info)->ui4_freq;
        psAtdnaDemodCtx->fgAFT = ((TUNER_CAB_ANA_TUNE_INFO_T *) pv_conn_info)->b_fine_tune;
        s2FreqOffset = ((TUNER_CAB_ANA_TUNE_INFO_T *) pv_conn_info)->i2_fine_tune_offset;
        //ZY, 091222. use u4AutoSearchType, e_mod only for FM_radio ***//
        u1ConnMode = GetAnaConnMode(((TUNER_CAB_ANA_TUNE_INFO_T *) pv_conn_info)->e_mod);
       #ifdef CC_DALI2K10 
	   psAtdnaDemodCtx->pTCtx->specific_member.us_ctx.u1InputSrc=MOD_ANA_CABLE;
		#endif
    }
    else if ((e_conn_type == TUNER_CONN_TYPE_TER_ANA))
    {
        u4Freq = ((TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info)->ui4_freq;
        psAtdnaDemodCtx->fgAFT = ((TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info)->b_fine_tune;
        s2FreqOffset = ((TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info)->i2_fine_tune_offset;
        //ZY, 091222. use u4AutoSearchType, e_mod only for FM_radio ***//
        u1ConnMode = GetAnaConnMode(((TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info)->e_mod);
       #ifdef CC_DALI2K10 
	   psAtdnaDemodCtx->pTCtx->specific_member.us_ctx.u1InputSrc= MOD_ANA_TERR;
		#endif
    }
    else
    {
    //    mcVERIFY_ASSERT(0);
        mcSHOW_HW_MSG(("%s: Wrong e_conn_type (%d)!\n", __FUNCTION__, e_conn_type));

        mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);

        return FALSE;
    }
	

#ifdef CC_DALI2K10 //Philips DALI2K10 need switch RF_AGC
u4GpioNum=DRVCUST_OptGet(eTunerCtrlAna);
GPIO_SetOut(u4GpioNum,0);
mcSHOW_DBG_MSG(("Switch RF_AGC to Analog Mode %d->0\n",u4GpioNum));
#endif



#ifndef CC_MT5391_AUD_3_DECODER
        AnaPd_ChangeChannel(u4Freq);
#endif

    AnaPd_EnterTunerAcq();

    //mcSHOW_DBG_MSG(("[ATuner] %s (Freq=%u) (Freq_bound=%u)", __FUNCTION__, u4Freq, u4FreqBound));
      mcSHOW_DBG_MSG(("[DriverSearchChannelStart-ATuner] %s (Freq=%u)\n", __FUNCTION__, u4Freq));

#if fcTIMING_MEASURE
    u2TickStart = mcGET_SYS_TICK();
#endif

    u4Freq = u4Freq / 1000;
	
    if (u1ConnMode == ATV_CONN_MODE_FM_RADIO)
    {
        ATD_ChipInit(psAtdnaDemodCtx);
        SetFMRadio(u4Freq);
        ATD_SetFreq(psAtdnaDemodCtx, u4Freq, _fgAutoSearch);
        ATD_StartAcq(psAtdnaDemodCtx, FALSE);
        AnaPd_ExitTunerAcq(TRUE);
        mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
        psAtdPdCtx->u1VIFLock = 3;
        return TRUE;
    }

     if ((u1ConnMode != ATV_CONN_MODE_CHAN_SCAN)&&(psAtdnaDemodCtx->fgAFT == FALSE)  && (s2FreqOffset > -80) && (s2FreqOffset < 80))
    {        
        ucData1 = 0xF9;
        ATD_SetReg(psAtdnaDemodCtx, 0x997, &ucData1, 1); //EC_code_d9A28: Enable CR auto-reacq for Fine tune mode
#if defined(ATD_FINETUNE_STEPSIZE)  
                s4FreqOffset = s2FreqOffset * ATD_FINETUNE_STEPSIZE; //EC_code_dA111: For SONY project, ATD_FINETUNE_STEPSIZE should be define as "31250" equal to 31.25KHz for each step
#else
                s4FreqOffset = s2FreqOffset * 0;
#endif
        mcSHOW_DBG_MSG(("Original freq = %dKHz, Offset freq = %dKHz\n",u4Freq, s4FreqOffset/1000));
        u4Freq = u4Freq + s4FreqOffset/1000;
        mcSHOW_DBG_MSG(("New freq = %dKHz\n",u4Freq));
    }

    psAtdPdCtx->u4AcqFrequency = u4Freq;
     
    //psAtdPdCtx->u1SubSysIdx = 6;
    psAtdPdCtx->u1SubSysIdx = MOD_NTSC_M;

    if (u1ConnMode != ATV_CONN_MODE_FINE_TUNE)
    {
          ATD_ChipInit(psAtdnaDemodCtx);
          ATD_SetSystem(psAtdnaDemodCtx, psAtdPdCtx->u1SubSysIdx);
    }

    if (ATD_SetFreq(psAtdnaDemodCtx, u4Freq, _fgAutoSearch) != TRUE)
    {
        mcSHOW_HW_MSG(("Freq %dKHz setting fail!\n", u4Freq));
        AnaPd_ExitTunerAcq(FALSE);
        mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
        return FALSE;
    }


#if fcTIMING_MEASURE
    mcSHOW_DBG_MSG4((" (1.%3u ms)\n", (mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD()));
#endif

#if fcTUNE_NOT_START
    if (psAtdPdCtx->u1TunerAcqType == TUNE_FREQ_ONLY)
    {
       ATD_StartAcq(psAtdnaDemodCtx, TRUE);
    	mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
       mcSHOW_HW_MSG(("Running ATD StartAcq\n")); 
        return TRUE;
    }
#endif
if ((u1ConnMode == ATV_CONN_MODE_CHAN_CHANGE) && (psAtdnaDemodCtx->u1TunerType == cPANASONIC_ENV5SDF)) //for TW (dvbt + atdna) channel change
    {
     mcSHOW_DBG_MSG(("[ATuner]When channel change and tuner is TW cPANASONIC_ENV5SDF,Don't check tv_decoder\n"));  
    }

else  
   {
         INT32 i4_i = 0;

        for (i4_i = 0; i4_i < 30; i4_i++)
        {
            if (bGetSignalType(SV_VP_PIP) == (UINT8) SV_ST_TV)
            {
                u1Path = SV_VP_PIP;
                //mcSHOW_DBG_MSG(("[ATuner] u1Path = SV_VP_PIP\n"));
                mcSHOW_DBG_MSG(("[ATuner] Now is PIP path\n"));
                break;
            }
            else if (bGetSignalType(SV_VP_MAIN) == (UINT8) SV_ST_TV)
            {
                u1Path = SV_VP_MAIN;
                //mcSHOW_DBG_MSG(("[ATuner] u1Path = SV_VP_MAIN\n"));
                 mcSHOW_DBG_MSG(("[ATuner] Now is MAIN path\n"));
                break;
            }
            else
            {
                if (_BreakFct(pvArg))
                {
                    mcSHOW_DBG_MSG(("[ATuner] BreakFct\n"));
                    break;
                }
                mcDELAY_MS(50);
                continue;
            }
        }
        if ((i4_i >= 30) || (u1Path == SV_VP_MAX))
        {
            mcSHOW_HW_MSG(("[ATuner] ERROR! without TV-decoder!\n"));
            AnaPd_ExitTunerAcq(FALSE);

            mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);

            return FALSE;

        }
   }

    i4CurFreq = u4Freq;
    
    if(u1ConnMode == ATV_CONN_MODE_CHAN_SCAN)
    {
    UINT32 u2FFF = u4Freq;
//    UINT32 u2FFFBD = u4FreqBound;  //fix variable "u2FFFBD" was declared but never referenced

        //u1ATVStatus = AnalogPINA_TunerSearchNearbyFreq(cATUNER_PI_CTX(psAtvPdCtx), u1ScanType, &u2FFF, _BreakFct, pvArg, u2FFFBD);  // eTUNER_AUTO_FINE_TUNE
        //u1ATVStatus = AnalogPINA_TunerSearchNearbyFreq(cATUNER_PI_CTX(psAtdPdCtx), &u2FFF, _BreakFct, pvArg);
        psAtdnaDemodCtx->u1PF = 0x01;
        ATD_StartAcq(psAtdnaDemodCtx, TRUE);
        psAtdnaDemodCtx->fgCheckCR = TRUE;
        u1ATVStatus = ATDNA_SearchNearbyFreq(psAtdnaDemodCtx, &u2FFF, _BreakFct, pvArg, u2FFF);
        psAtdPdCtx->u1VIFLock = u1ATVStatus;

        if (mcTEST_BIT(u1ATVStatus, cpANA_LOCK_STS_TVD))
        {
        	  if(2==fgEnableATP)ScanedNo++;//for Driver ATP auto test(scaned channel number)
            fgLockSts = TRUE;
        }
        else
        {
            fgLockSts = FALSE;
        }
        if (fgLockSts)
        {
           // mcSHOW_DBG_MSG(("[ATuner] A_N_S\n"));
           mcSHOW_DBG_MSG(("[ATuner] Notify signal stable now\n"));
            _Tvd_NotifyTVSig(u1Path, SV_VDO_STABLE);
        //    _Tvd_NotifyTVSig(SV_VP_MAIN, SV_VDO_STABLE);
        }
        else
        {
            //mcSHOW_DBG_MSG(("[ATuner] A_N_N\n"));
            mcSHOW_DBG_MSG(("[ATuner] Notify no signal now \n"));
            _Tvd_NotifyTVSig(u1Path, SV_VDO_NOSIGNAL);
        }
        i4CurFreq = u2FFF;
    }
    else if(u1ConnMode == ATV_CONN_MODE_CHAN_CHANGE)
    {
        //u1ATVStatus = AnalogPINA_TunerSearchNearbyFreq(cATUNER_PI_CTX(psAtvPdCtx), eTUNER_UNI_AUTO_SCAN, &u4Freq, _BreakFct, pvArg, u4Freq);
        //u1ATVStatus = AnalogPINA_TunerSearchNearbyFreq(cATUNER_PI_CTX(psAtdPdCtx), &u4Freq, _BreakFct, pvArg);
        psAtdnaDemodCtx->u1PF = 0x01;
        ATD_StartAcq(psAtdnaDemodCtx, TRUE);

        if(psAtdnaDemodCtx->fgAFT == TRUE)   //do searchnearby when AFT is enabled
      	 {
            psAtdnaDemodCtx->fgCheckCR = TRUE;
        u1ATVStatus = ATDNA_SearchNearbyFreq(psAtdnaDemodCtx, &u4Freq, _BreakFct, pvArg, u4Freq);
        psAtdPdCtx->u1VIFLock = u1ATVStatus;
        if (mcTEST_BIT(u1ATVStatus, cpANA_LOCK_STS_TVD))
            i4CurFreq = u4Freq;
        else
            //AnalogPINA_TunerSetFreq(cATUNER_PI_CTX(psAtdPdCtx), i4CurFreq, MOD_ANA_CABLE);
                ATD_SetFreq(psAtdnaDemodCtx, i4CurFreq, _fgAutoSearch);
      	}
        else  //no searchnearby when AFT is disabled
        {
            psAtdPdCtx->u1VIFLock = 3;
        }

         if(2==fgEnableATP){   //show scaned channel when first time channel change after channel scan
         mcSHOW_DBG_MSG(("##Start\n##Scan : %u\n##End\n",ScanedNo));
          ScanedNo=0;
        }

     }
    else  //fine_tune mode
	{
            //mcSHOW_DBG_MSG(("[ATuner] A_N_S\n"));
            //*** ZY 091222, ATD not notify at fine tune mode. TVD will notify
            //mcSHOW_DBG_MSG(("[ATuner] Notify now signal stable\n"));
            //_Tvd_NotifyTVSig(u1Path, SV_VDO_STABLE);
        psAtdPdCtx->u1VIFLock = 3;
        }
	
#if 0
    mcSHOW_DBG_MSG(("VIFLock: %d (%04X, %d, %d)\n",
                    psAtvPdCtx->u1VIFLock,
                    i4CurFreq, i4CurFreq, AnaPi_TunerGetVIFLock()));
#endif
  mcSHOW_DBG_MSG(("DriverSearchChannelEnd-Status: %d (%dKHz)\
(3-VIF and TVD both lock,2-TVD lock,1-VIF lock)\n",
                  psAtdPdCtx->u1VIFLock,
                  i4CurFreq));
#if fcTIMING_MEASURE
    mcSHOW_DBG_MSG4((" (2.%3u ms)\n", (mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD()));
#endif
    //mcSHOW_DBG_MSG(("[ATuner] %s fine-tune: %04X\n", __FUNCTION__, i4CurFreq));
    psAtdPdCtx->Frequency = i4CurFreq;


if(mcCHECK_DBG_LVL(4))
{
	//UINT8 ii=0;
	//ii = 0xA;
	mcDELAY_MS(100);
	mcSHOW_USER_MSG(("Test Mod = %d\n", (IO_READ8(0x20022080, 0x03)>>4)&0x07));
}

#ifdef CC_DISABLE_AFT
psAtdnaDemodCtx->fgAFT=FALSE;
mcSHOW_DBG_MSG(("Disable AFT\n"));
#endif

//    fgIsDisconnected = FALSE;
//#if !defined(CC_MTAL) && !defined(CC_SDAL)
#ifdef ANAUS_ENABLE_MONITOR

    //if (!_BreakFct(pvArg) && (u1ConnMode == ATV_CONN_MODE_CHAN_CHANGE) && (((TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info)->b_fine_tune))
    //if (!_BreakFct(pvArg) && (u1ConnMode == ATV_CONN_MODE_CHAN_CHANGE) && (bFineTune))
    if (!_BreakFct(pvArg) && (u1ConnMode == ATV_CONN_MODE_CHAN_CHANGE))
    {
        //psAtvPdCtx->fgBreakMonitor = FALSE;           // Enable monitor thread and Disable break function flag
        //mcMUTEX_UNLOCK(t_escape_mon_suspend);  // Unlock monitor thread
        ResumeMonitor(psAtdPdCtx);   // Enable monitor thread and Disable break function flag,  Unlock monitor thread
        mcSHOW_DBG_MSG(("Enable monitor process!!\n"));
    }
#endif
    if (psAtdPdCtx->u1VIFLock < 2)
    {
       AnaPd_ExitTunerAcq(FALSE);
        mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
        return FALSE;
    }
    AnaPd_ExitTunerAcq(TRUE);

    mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);

    Printf("END AtdNA_TunerAcq\n");
    return TRUE;



}

//-----------------------------------------------------------------------------
/*
 *  AtdNA_TunerGetSync
 *  Not Used now
 *  @param  ptTDCtx  a void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @param  e_conn_type connection type (Air / Cable)
 *  @retval   none.
 */
//-----------------------------------------------------------------------------
INT16 AtdNA_TunerGetSync(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                          DRV_CONN_TYPE_T e_conn_type)
{

INT16   i2RetValue = 1;                     // Always in sync state
    return i2RetValue;
}

//-----------------------------------------------------------------------------
/*
 *  AtdNA_TunerGetSignal
 *  To get current signal characteristic
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @param  e_conn_type Connection type (Air / Cable)
 *  @param  *_pSignal Singal parameters
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
VOID AtdNA_TunerGetSignal(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                    DRV_CONN_TYPE_T e_conn_type,
                    SIGNAL *_pSignal)
{
//ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;
// TUNER_POLARIZATION_T  e_pol;         /* Polarization. */
// TUNER_MODULATION_T    e_mod;         /* Modulation. */
// UINT16                ui2_dir;       /* Direction in 0.1 degrees. Valid value range '0..3599' (inclusive). */
// UINT16                ui2_gain;      /* Gain in 0.1 steps. Valid range '0..1000' (inclusive). */
INT32 i4CurFreq;
UINT32  u4Freq;
#if 0
    i4CurFreq = psAtdPdCtx->Frequency;
    u4Freq = i4CurFreq * 1000;
#endif
#if 1
    i4CurFreq=psAtdnaDemodCtx->pTCtx->u4RF_Freq;
    u4Freq = i4CurFreq * 1000;//Return frequency in Hz
#endif
    mcSHOW_DBG_MSG(("[ATuner] %s:  %dKHz (%dHz)", __FUNCTION__, i4CurFreq,u4Freq));
    if ((e_conn_type == TUNER_CONN_TYPE_CAB_ANA))
    {
        TUNER_CAB_ANA_TUNE_INFO_T * pt_cab_ana = (TUNER_CAB_ANA_TUNE_INFO_T *)_pSignal;
        pt_cab_ana->ui4_freq = u4Freq;
        pt_cab_ana->e_mod = MOD_PSK_8;
        mcSHOW_DBG_MSG((" (%d)\n", pt_cab_ana->ui4_freq));
    }
    else if ((e_conn_type == TUNER_CONN_TYPE_TER_ANA))
    {
        TUNER_TER_ANA_TUNE_INFO_T * pt_ter_ana = (TUNER_TER_ANA_TUNE_INFO_T *)_pSignal;
        pt_ter_ana->ui4_freq = u4Freq;
        pt_ter_ana->e_mod = MOD_PSK_8;
        mcSHOW_DBG_MSG((" (%d)\n", pt_ter_ana->ui4_freq));
    }
}

//-----------------------------------------------------------------------------
/*
 *  AtdNA_TunerGetSignalLevel
 *  Not Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @param  e_conn_type Connection type (Air / Cable)
 *  @retval   Signal quality, Range is 0~100.
 */
//-----------------------------------------------------------------------------
UINT8  AtdNA_TunerGetSignalLevel(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                DRV_CONN_TYPE_T e_conn_type)
{
UINT8 u1Value;
    /* Provided by TVD/KY lin
    > 60dB: 99
      55dB: 97
      50dB: 91
      45dB: 84
      40dB: 72
      35dB: 55
      30dB: 43
      25dB: 23
      signal off: 0
    */
    u1Value = bTvdSignalStrengthLevel(0); // bMode = 0, return 0~100
    return (u1Value);
}

//-----------------------------------------------------------------------------
/*
 *  AtdNA_TunerGetSignalLeveldBm
 *  Not Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @param  e_conn_type Connection type (Air / Cable)
 *  @retval   Signal quality (in dB).
 */
//-----------------------------------------------------------------------------
// Signal Level
INT16 AtdNA_TunerGetSignalLeveldBm(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                    DRV_CONN_TYPE_T e_conn_type)
{
UINT8 u1Value;
    u1Value = bTvdSignalStrengthLevel(1); // bMode = 1, return in dB
    return (u1Value);
}

//-----------------------------------------------------------------------------
/*
 *  AtdNA_TunerGetSignalPER
 *  Not Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @param  e_conn_type Connection type (Air / Cable)
 *  @retval   None.
 */
//-----------------------------------------------------------------------------
// packet error rate
UINT16  AtdNA_TunerGetSignalPER(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                    DRV_CONN_TYPE_T e_conn_type)
{
    return (0);
}

//-----------------------------------------------------------------------------
/*
 *  AtdNA_TunerGetSignalSNR
 *  Not Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @param  e_conn_type Connection type (Air / Cable)
 *  @retval   None.
 */
//-----------------------------------------------------------------------------
UINT16 AtdNA_TunerGetSignalSNR(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                    DRV_CONN_TYPE_T e_conn_type)
{
    return (0);                             // return 0 first
}

//-----------------------------------------------------------------------------
/*
 *  AtdNA_TunerSetMpgFmt
 *  Not Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @param  e_conn_type Connection type (Air / Cable)
 *  @retval   None.
 */
//-----------------------------------------------------------------------------
VOID AtdNA_TunerSetMpgFmt(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                            MPEG_FMT_T *pt_mpeg_fmt)
{
    return;
}

//-----------------------------------------------------------------------------
/*
 *  AtdNA_TunerGetVer
 *  To get tuner version
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @retval   "Analog_Tuner".
 */
//-----------------------------------------------------------------------------
CHAR *AtdNA_TunerGetVer(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;

    mcSEMA_LOCK_HAL(psAtdPdCtx->hHalLock);  // Avoid re-entry

    //mcSHOW_USER_MSG(("[ATuner] %s\n", AnalogPINA_TunerGetVer(cATUNER_PI_CTX(psAtdPdCtx))));

    mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
    return "Analog_Tuner";
}

//-----------------------------------------------------------------------------
/*
 *  AtdNA_TunerNimTest
 *  No Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @retval   none
 */
//-----------------------------------------------------------------------------
INT32 AtdNA_TunerNimTest(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSHOW_DBG_MSG(("%s\n", __FUNCTION__));

    return 0;
}

//-----------------------------------------------------------------------------
/*
 *  AtdNA_TunerSetRegSetting
 *  No Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @retval   none
 */
//-----------------------------------------------------------------------------
VOID AtdNA_TunerSetRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx, UCHAR ucRegSet,
                            UCHAR ucRegAddr, UCHAR ucRegValue)
{
    mcSHOW_DBG_MSG(("%s\n", __FUNCTION__));
}

//-----------------------------------------------------------------------------
/*
 *  AtdNA_TunerShowRegSetting
 *  No Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @retval   none
 */
//-----------------------------------------------------------------------------
VOID AtdNA_TunerShowRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSHOW_DBG_MSG(("%s\n", __FUNCTION__));
}

//-----------------------------------------------------------------------------
/*
 *  AtdNA_TunerTestI2C
 *  No Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @retval   none
 */
//-----------------------------------------------------------------------------
VOID AtdNA_TunerTestI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSHOW_DBG_MSG(("%s\n", __FUNCTION__));
}

//-----------------------------------------------------------------------------
/*
 *  AtdNA_TunerRegRead
 *  No Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @retval   none
 */
//-----------------------------------------------------------------------------
INT32 AtdNA_TunerRegRead(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type,
                    UCHAR ucRegAddr, UCHAR *pucBuffer, U16 ucByteCount)
{
    return 0;
}

//-----------------------------------------------------------------------------
/*
 *  AtdNA_TunerDisc
 *  Tuner driver disconection
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @param  e_conn_type connection type (Air / Cable)
 *  @retval   none
 */
//-----------------------------------------------------------------------------
VOID  AtdNA_TunerDisc(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                DRV_CONN_TYPE_T e_conn_type)
{
#ifdef CC_DALI2K10 //Philips DALI2K10 need switch RF_AGC
	UINT32 u4GpioNum;
#endif
  //  #ifdef ANAUS_ENABLE_MONITOR    // liuqu marked,20091218
ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;
//  #endif

//    mcSEMA_LOCK_HAL(psAtvPdCtx->hHalLock);  // Avoid re-entry
//    fgIsDisconnected = TRUE;
//    psAtvPdCtx->fgBreakMonitor = TRUE;
//    mcSEMA_UNLOCK_HAL(psAtvPdCtx->hHalLock);
    mcSHOW_DBG_MSG(("[Atuner] %s()\n", __FUNCTION__));

    #ifdef ANAUS_ENABLE_MONITOR
    SetBreakMonitor(psAtdPdCtx);
    #endif

    ATD_DisConnect(psAtdPdCtx->sAtdPiCtx);
    
#ifdef CC_DALI2K10 //Philips DALI2K10 need switch RF_AGC
u4GpioNum=DRVCUST_OptGet(eTunerCtrlAna);
GPIO_SetOut(u4GpioNum,1);
mcSHOW_DBG_MSG(("Switch RF_AGC to Digital Mode %d->1\n",u4GpioNum));
#endif
}

//-----------------------------------------------------------------------------
/*
 *  AtdNA_TunerCommand
 *  NA analog Tuner command
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @param  i4Argc Number of argument
 *  @param  aszArgve argument
 *  @retval   none
 */
//-----------------------------------------------------------------------------
VOID AtdNA_TunerCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                      INT32 i4Argc, const CHAR** aszArgv)
{
#if fcADD_HAL_LOCK
ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;
#endif
UCHAR   ucCmdId = 0;

    mcSEMA_LOCK_HAL(psAtdPdCtx->hHalLock);  // Avoid re-entry

    if (i4Argc < 1)
    {
        mcSHOW_USER_MSG(("Cmd: \n"));
        //mcSHOW_USER_MSG(("\t o FreqOffset\n"));
    //PK
    #if fcTEST_I2C_DETACH
        mcSHOW_USER_MSG(("\t i fgEnI2CDetach\n"));
    #endif
    #if fcTIMING_MEASURE
        mcSHOW_USER_MSG(("\t d u2Delay\n"));
    #endif
    //    mcSHOW_USER_MSG(("\t u Light bar reduction\n"));

#if 0  //Jackson remove fcADD_CUST_IF to fix zero used for undefined preprocessing identifier
    #if fcADD_CUST_IF && fcINTERNAL_TEST
        mcSHOW_USER_MSG(("\t qf u4TunerCtrlAnaForce: Force DRVCUST_OptGet(eTunerCtrlAna)\n"));
    #endif
#endif
    }

    if (i4Argc > 0)
    {
        ucCmdId = *((CHAR *) aszArgv[0]);

        switch (ucCmdId)
        {
            /*case 'o':
                if (i4Argc > 1)
                {
                //    i4FreqOffset = axtoi((CHAR *) aszArgv[1]);
                    i4FreqOffset = StrToInt(aszArgv[1]);
                    if (i4Argc > 2)
                        i4FreqOffset = -i4FreqOffset;
                }
                mcSHOW_USER_MSG(("i4FreqOffset = %d\n", i4FreqOffset));
                break;*/

        #if fcTEST_I2C_DETACH
            case 'i':
                if (i4Argc > 1)
                {
                    fgEnI2CDetach = StrToInt(aszArgv[1]);
                }
                mcSHOW_USER_MSG(("fgEnI2CDetach = %d\n", fgEnI2CDetach));
                break;
        #endif

        #if fcTIMING_MEASURE
            case 'd':
                if (i4Argc > 1)
                {
                    u2Delay = StrToInt(aszArgv[1]);
                }
                mcSHOW_USER_MSG(("u2Delay = %d\n", u2Delay));
                break;
        #endif

            case 'u':
        #if 0
                if (i4Argc > 1)
                {
                    vSetTunerTuning(StrToInt(aszArgv[1]));
                    AnalogPINA_TunerSetFreq(cATUNER_PI_CTX(psAtvPdCtx), psAtvPdCtx->Frequency, MOD_ANA_CABLE);
                }
        #endif
                break;

#if 0  //Jackson remove fcADD_CUST_IF to fix zero used for undefined preprocessing identifier
        #if fcADD_CUST_IF && fcINTERNAL_TEST    // [
            case 'q':
                {
                UINT8   ucCmdExt;

                    ucCmdExt = ((CHAR *) aszArgv[0])[1];

                    if (ucCmdExt == 'f')
                    {
                        if (i4Argc > 1)
                            u4TunerCtrlAnaForce =  axtoi((CHAR *) aszArgv[1]);
                        mcSHOW_USER_MSG(("u4TunerCtrlAnaForce=0x%02X", u4TunerCtrlAnaForce));
                    }
                }
                break;
        #endif  // ]
#endif
            default:
            //    mcSHOW_USER_MSG(("Invalid parameter!\n"));
                break;
        }
    }
    mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
#ifndef CC_MT5381
    AtdNA_TunerAtdCommand(ptTDSpecificCtx, i4Argc, aszArgv);
#endif
}

//-----------------------------------------------------------------------------
/*
 *  AtdNA_TunerAtdCommand
 *  Some NA analog ATD Tuner command (ex : set frequency...)
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @param  i4Argc Number of argument
 *  @param  aszArgve argument
 *  @retval   none
 */
//-----------------------------------------------------------------------------
VOID AtdNA_TunerAtdCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        INT32 i4Argc, const CHAR** aszArgv)
{
ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;
//GEN_TUNER_CTX_T *pDigiTunerCtx;
UCHAR   ucCmdId = 0, ucPar, ucData[256];
UINT32  i4CurFreq, i4BndFreq;
UINT32  u4Freq = 0;
UINT8   u1SubSysIdx, ii, jj;//, kk;
U16     u2RegAddr, u2Period;
UINT8   ucData1, ucData2, u1Count;
S32     s2Data;
U32     u2Data;
UINT16  u2State;
UINT32 RegData=0;

    mcSEMA_LOCK_HAL(psAtdPdCtx->hHalLock);  // Avoid re-entry

    if (i4Argc < 1)
    {
        mcSHOW_USER_MSG(("Cmd: \n"));
        mcSHOW_USER_MSG(("\t c:              Show Current Freq (kHz)\n"));    //CR 00012779, for Hisense's request, LC Chien 070514
        //mcSHOW_USER_MSG(("\t t [TopValue]:   Set ATV TOP \n"));
        mcSHOW_USER_MSG(("\t f [Freq (kHz)]: AnaTunerSetFreq\n"));
        mcSHOW_USER_MSG(("\t s [SubSysIdx]:  Color / Sound sub-system\n"));
        mcSHOW_USER_MSG(("\t v [Freq (kHz)]: Check Lock Status\n"));
        mcSHOW_USER_MSG(("\t h [Start Freq (kHz)][End Freq (kHz)][Count]: TunerSearchNearbyFreq\n"));
        mcSHOW_USER_MSG(("\t r [RegAddr] [Num]:   read  ATD register\n"));
        mcSHOW_USER_MSG(("\t w [RegAddr] [Value]: write ATD register\n"));
        //mcSHOW_USER_MSG(("\t t [Patch timing]:   Set FW patch timing \n"));
        mcSHOW_USER_MSG(("\t g [Count] [Period (ms)]: Get current ATD status \n"));
        mcSHOW_USER_MSG(("\t p [CR loop bandwidth]: Set CR loop bandwidth \n"));
        mcSHOW_USER_MSG(("\t pl: Enable driver ATP auto test log level(0(default),1,2)\n"));
	    mcSHOW_USER_MSG(("\t pd Set TVD re-check interval \n"));
        mcSHOW_USER_MSG(("\t n [0/1]: Disable/enable new scan step\n"));
        mcSHOW_USER_MSG(("\t b:             Turn on the CVBS out\n"));
        mcSHOW_USER_MSG(("\t l:             SLT test    \n"));
        mcSHOW_USER_MSG(("\t x [0/1]: Set LNA on/off \n"));
      //#if  fcTUNER_TYPE==cALPS_TDAU4D01A
      #ifdef DTD_INCLUDE
      if (psAtdnaDemodCtx->u1TunerType == cALPS_TDAU4D01A)
      {
        mcSHOW_USER_MSG(("\t j: Analogue tuner adjacent trap control. 1--ON,0--OFF\n"));
      }
      #endif
      //#endif
#ifdef ANAUS_ENABLE_MONITOR
        mcSHOW_USER_MSG(("\t mp:             Pause monitor thread\n"));
        mcSHOW_USER_MSG(("\t mr:             Resume monitor thread\n"));


#endif
    }

    if (i4Argc > 0)
    {
        ucCmdId = *((CHAR *) aszArgv[0]);

        switch (ucCmdId)
        {
        	 //#if  fcTUNER_TYPE==cALPS_TDAU4D01A
        	 #ifdef DTD_INCLUDE
          	case 'j':
          		if (psAtdnaDemodCtx->u1TunerType == cALPS_TDAU4D01A)
          		{
          		    if (i4Argc > 1)
          		    {
          		        ii = StrToInt(aszArgv[1]);
          		        if(1==ii)
          		        {
          		            gfgIFTrap=0x03;//IF Trap ON
          		        }
          		        else
          		        {
          		            gfgIFTrap=0x02;//IF Trap OFF
          		        }
          		    }
          		}
          		else
          		{
                    mcSHOW_USER_MSG(("Not support!\n"));
          		}
             break;
           #endif
            case 'c':
                i4CurFreq = psAtdPdCtx->Frequency;
                mcSHOW_USER_MSG(("Current Freq = %d kHz\n", i4CurFreq));
                break;

            /*case 't':
                if (i4Argc > 1)
                {
                    psAtdnaDemodCtx->fgMonitor_500ms = (U8)StrToInt(aszArgv[1]);
                }
                else
                {
                    mcSHOW_USER_MSG(("Current FW patch timing = %d (0: 2ms, 1: 500ms)\n", psAtdnaDemodCtx->fgMonitor_500ms));
                }

                break;*/

            case 'b':
                    mcSHOW_USER_MSG(("Turn on the CVBS out function\n"));
                    RegData= IO_READ32(CKGEN_BASE,0x2a8);
                    RegData &=0xFF01FFFF;//Bit17~23 =0
                    RegData |=0x00400000;//Bit22 =1
                    IO_WRITE32(CKGEN_BASE,0x2a8, RegData);
                
                    RegData= IO_READ32(TV_EOCODER_BASE,0x634); 
                    RegData &=0xFF00FF00;//Bit0~7,16~23 =0
                    RegData |=0x000000FF;//Bit0~7 FF
                    IO_WRITE32(TV_EOCODER_BASE,0x634, RegData);
                    
                    IO_WRITE32(TV_EOCODER_BASE,0x684, 0x00060600);
                    
                    IO_WRITE32(TV_EOCODER_BASE,0x604, 0x08010280);
                    
                    IO_WRITE32(TV_EOCODER_BASE,0x67c, 0x00050424);
                    
                    RegData= IO_READ32(TV_EOCODER_BASE,0x678); 
                    RegData |=0x000D0001;//Bit0,16,18,19 1
                    IO_WRITE32(TV_EOCODER_BASE,0x678, RegData);
             /*       
                    IO_WRITE32(0x2000d000,0x29c, 0xfffffffd);
                    IO_WRITE32(0x2000d000,0x2a0, 0x02200000);
                    IO_WRITE32(0x20005000,0x220, 0x80000000);
                    IO_WRITE32(0x20006000,0x604, 0x00000000);
                    IO_WRITE32(0x20006000,0x634, 0x000000ff);               	
                    IO_WRITE32(0x20006000,0x678, 0x00050000);               	
                    IO_WRITE32(0x2000d000,0x2a8, 0x00601000);                                    
               */
                break;

            case 'f':
                i4CurFreq = psAtdPdCtx->Frequency;
                if (i4Argc > 1)
                {
                    i4CurFreq = StrToInt(aszArgv[1]);
                    psAtdPdCtx->Frequency = i4CurFreq;
                }
                ATD_ChipInit(psAtdnaDemodCtx);
                ATD_SetFreq(psAtdnaDemodCtx, i4CurFreq, FALSE);
                ATD_StartAcq(psAtdnaDemodCtx, FALSE);
                //AnalogPIEU_TunerSetFreq(cATUNER_PI_CTX(psAtdPdCtx), i4CurFreq, ATV_CONN_MODE_CHAN_CHANGE);
                mcSHOW_USER_MSG(("Set Frequency = %d KHz\n", i4CurFreq));
                break;

            case 's':

                u1SubSysIdx = psAtdPdCtx->u1SubSysIdx;
                if (i4Argc > 1)
                {
                //    _SubSysIdx = StrToInt(aszArgv[1]);
                    u1SubSysIdx = StrToInt(aszArgv[1]);
                    //if (u1SubSysIdx < SYS_SUB_TOTAL)
                    //{
                        psAtdPdCtx->u1SubSysIdx = u1SubSysIdx + MOD_ANA_TYPE_BEGIN;
                        //ATD_ChipInit(psAtdnaDemodCtx);
                        ATD_SetSystem(psAtdnaDemodCtx, psAtdPdCtx->u1SubSysIdx);
                        //ATD_StartAcq(psAtdnaDemodCtx);
                        //AnalogPIEU_TunerSetSystem(cATUNER_PI_CTX(psAtdPdCtx), u1SubSysIdx);
                    //}
                }
                else
                {
                    mcSHOW_USER_MSG(("\t %d: PAL_BG  \n", MOD_PAL_BG - MOD_ANA_TYPE_BEGIN));
                    mcSHOW_USER_MSG(("\t %d: PAL_DK  \n", MOD_PAL_DK - MOD_ANA_TYPE_BEGIN));
                    mcSHOW_USER_MSG(("\t %d: PAL_I   \n", MOD_PAL_I - MOD_ANA_TYPE_BEGIN));
                    mcSHOW_USER_MSG(("\t %d: SECAM_L \n", MOD_SECAM_L - MOD_ANA_TYPE_BEGIN));
                    mcSHOW_USER_MSG(("\t %d: SECAM_L1\n", MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN));
                    mcSHOW_USER_MSG(("\t %d: NTSC_M  \n", MOD_NTSC_M - MOD_ANA_TYPE_BEGIN));
                    //mcSHOW_USER_MSG(("\t 7: NTSC_54M  \n"));
                }
                mcSHOW_USER_MSG(("_SubSysIdx = %d\n", psAtdPdCtx->u1SubSysIdx - MOD_ANA_TYPE_BEGIN));
                break;

            case 'v':
                i4CurFreq = psAtdPdCtx->Frequency;

                //ATD_ChipInit(psAtdnaDemodCtx);

                if (i4Argc > 1)
                {
                    i4CurFreq = StrToInt(aszArgv[1]);
                    psAtdPdCtx->Frequency = i4CurFreq;
                    ATD_ChipInit(psAtdnaDemodCtx);
                    ATD_SetFreq(psAtdnaDemodCtx, i4CurFreq, FALSE);
                    ATD_StartAcq(psAtdnaDemodCtx, FALSE);
                    //mcDELAY_MS(50);
                    //ATD_SetScanMode(psAtdnaDemodCtx);
                }
                //ATD_StartAcq(psAtdnaDemodCtx);
                //for(ii=1;ii<10;ii++)
                //{
                mcSHOW_USER_MSG(("VIF Lock: %d (Freq = %dKHz)\n",
                                 ATDNA_GetVIFLock(psAtdnaDemodCtx, TRUE),
                                 //AnalogPIEU_TunerCheckVIFLock(cATUNER_PI_CTX(psAtdPdCtx)),
                                 i4CurFreq));
                //mcDELAY_MS(10);
                //}
                //mcSHOW_USER_MSG(("TVD Lock: %d\n", AtdPIEU_fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME, psAtdPdCtx->u1SubSysIdx)));
                break;

            case 'h':
                jj = 0;
                //kk = 0;
                i4CurFreq = psAtdPdCtx->Frequency;

                //ATD_ChipInit(psAtdnaDemodCtx);

                if (i4Argc > 1)
                {
                    i4CurFreq = StrToInt(aszArgv[1]);
                    i4BndFreq = StrToInt(aszArgv[2]);
                    u1Count = StrToInt(aszArgv[3]);
                    ATD_SetFreq(psAtdnaDemodCtx, i4CurFreq, FALSE);
                    //AnalogPIEU_TunerSetFreq(cATUNER_PI_CTX(psAtdPdCtx), i4CurFreq, ATV_CONN_MODE_CHAN_CHANGE);
                }
                else
                {
                    i4BndFreq = i4CurFreq;
                    u1Count = 1;
                }
                //u4Freq = i4CurFreq;
                psAtdnaDemodCtx->u1PF = 0x01;
                ATD_StartAcq(psAtdnaDemodCtx, TRUE);
                //mcDELAY_MS(20);
                //ATD_SetScanMode(psAtdnaDemodCtx);
                psAtdnaDemodCtx->fgCheckCR = TRUE;
                for(ii=0; ii<u1Count; ii++)
                {
                    u4Freq = i4CurFreq;
                    mcSHOW_USER_MSG(("the %d th time SearchNearFreq.\n",ii));
                    if(ATDNA_SearchNearbyFreq(psAtdnaDemodCtx, &u4Freq, TunerNullBreak, NULL, i4BndFreq))
                    {
                        //if(AtdPIEU_fgDrvCheckTVDLock(CHN_LOCK_CHECK_TIME, psAtdPdCtx->u1SubSysIdx))
                        //{
                            jj++;  //VIF lock, TVD lock
                       // }
                        //else
                        //{
                         //   kk++;  //VIF lock, but TVD unlock
                       // }
                    }

                    if(ii <(u1Count -1))
                    {
                        ATD_SetFreq(psAtdnaDemodCtx, i4CurFreq, FALSE);
                        psAtdnaDemodCtx->u1PF = 0x01;
                        psAtdnaDemodCtx->fgCheckCR = TRUE;
                        ATD_StartAcq(psAtdnaDemodCtx, TRUE);
                    }

                }
                //AnalogPIEU_TunerSearchNearbyFreq(cATUNER_PI_CTX(psAtdPdCtx), &u4Freq, TunerNullBreak, NULL, u4Freq);
                mcSHOW_USER_MSG(("Search NearBy Before %d After %d KHz\n", i4CurFreq, u4Freq));
                mcSHOW_USER_MSG(("Search NearBy %d times from %d KHz to %d KHz\n", u1Count, i4CurFreq, i4BndFreq));
                mcSHOW_USER_MSG(("VIF lock times = %d\n", jj));

                psAtdPdCtx->Frequency = u4Freq;
                break;
            case 'r':
                if (i4Argc < 3)
                    break;

                u2RegAddr = (U16)StrToInt(aszArgv[1]);
                ucPar = (U8)StrToInt(aszArgv[2]);
                if (ATD_GetReg(psAtdnaDemodCtx, u2RegAddr, ucData, ucPar))
                {
                    mcSHOW_USER_MSG(("MTATD Get Register 0x%03X Fail!\n", u2RegAddr));
                    break;
                }
                for (ii=0; ii<ucPar; ii++)
                {
                    mcSHOW_USER_MSG(("Register 0x%03X = 0x%02X\n", u2RegAddr+ii, ucData[ii]));
                }
                break;

            case 'w':
                if (i4Argc < 3)
                    break;

                u2RegAddr = (U16)StrToInt(aszArgv[1]);
                ucPar = (U8)StrToInt(aszArgv[2]);
                if (ATD_SetReg(psAtdnaDemodCtx, u2RegAddr, &ucPar, 1))
                {
                    mcSHOW_USER_MSG(("MTATD Set Register 0x%03X Fail!\n", u2RegAddr));
                    break;
                }
                mcSHOW_USER_MSG(("Register 0x%03X = 0x%02X, OK!\n", u2RegAddr, ucPar));
                break;

            case 'g':
                if (i4Argc == 1)
                {
                    u1Count = 1;
                    u2Period = 0;
                }
                else if (i4Argc == 2)
                {
                    u1Count = StrToInt(aszArgv[1]);
                    u2Period = 1000;
                }
                else if (i4Argc == 3)
                {
                    u1Count = StrToInt(aszArgv[1]);
                    u2Period = StrToInt(aszArgv[2]);
                }
                else
                {
                    break;
                }

                for (ii=0; ii<u1Count; ii++)
                {
                // CR
                s2Data = ATD_GetCFOKHz(psAtdnaDemodCtx, FALSE);
                mcSHOW_USER_MSG(("CFO = %d KHz\n", s2Data));
                ATD_GetReg(psAtdnaDemodCtx, 0x9A6, &ucData1, 1);
                ATD_GetReg(psAtdnaDemodCtx, 0x9A7, &ucData2, 1);
                s2Data = (S32)mcSIGN_2BYTE(ucData1, ucData2, 5, 8);
                s2Data = (S32)s2Data * 439 / 100000;
                mcSHOW_USER_MSG(("CPO = %d degree\n", s2Data));
                ATD_GetReg(psAtdnaDemodCtx, 0x9A0, &ucData1, 1);
                mcSHOW_USER_MSG(("OVM idx = %d\n", (ucData1 & 0x70) >> 4 ));
                mcSHOW_USER_MSG(("0x1A0 = 0x%02X\n", ucData1));
                ATD_GetReg(psAtdnaDemodCtx, 0x9A1, &ucData1, 1);
                mcSHOW_USER_MSG(("0x1A1 = 0x%02X\n", ucData1));
                mcSHOW_USER_MSG(("PN indicator = %d\n", (ucData1 & 0x08) >> 3 ));
                mcSHOW_USER_MSG(("FN indicator = %d\n", (ucData1 & 0x04) >> 2 ));
                //ATD_GetReg(psAtdnaDemodCtx, 0x197, &ucData1, 1);
                //mcSHOW_USER_MSG(("0x197 = 0x%02X\n", ucData1));
                //ATD_GetReg(psAtdnaDemodCtx, 0x198, &ucData1, 1);
                //mcSHOW_USER_MSG(("0x198 = 0x%02X\n", ucData1));
                ATD_GetReg(psAtdnaDemodCtx, 0x99C, &ucData1, 1);
                ATD_GetReg(psAtdnaDemodCtx, 0x99D, &ucData2, 1);
                u2Data = mcUNSIGN_2BYTE(ucData1, ucData2, 8, 8);
                mcSHOW_USER_MSG(("CR CMetrix = 0x%04X\n", u2Data));
                ATD_GetReg(psAtdnaDemodCtx, 0x00B, &ucData1, 1);
                switch (ucData1)
                {
                    case 0:
                        mcSHOW_USER_MSG(("CR in normal mode!\n"));
                        break;
                    case 1:
                        mcSHOW_USER_MSG(("CR in weak signal+PN mode!\n"));
                        break;
                    case 2:
                        mcSHOW_USER_MSG(("CR in weak signal mode!\n"));
                        break;
                    case 3:
                        mcSHOW_USER_MSG(("CR in OVM mode!\n"));
                        break;
                    case 4:
                        mcSHOW_USER_MSG(("CR in imopulse mode!\n"));
                        break;
                    default:
                        break;
                }
                // DRO
                ATD_GetReg(psAtdnaDemodCtx, 0x96F, &ucData1, 1);
                ATD_GetReg(psAtdnaDemodCtx, 0x970, &ucData2, 1);
                u2Data = mcUNSIGN_2BYTE(ucData1, ucData2, 8, 8);
                mcSHOW_USER_MSG(("DRO gain = 0x%04X\n", u2Data));
                //ATD_GetReg(psAtdnaDemodCtx, 0x164, &ucData1, 1);
                //mcSHOW_USER_MSG(("0x164 = 0x%02X\n", ucData1));
                //ATD_GetReg(psAtdnaDemodCtx, 0x166, &ucData1, 1);
                //mcSHOW_USER_MSG(("0x166 = 0x%02X\n", ucData1));
                //ATD_GetReg(psAtdnaDemodCtx, 0x17A, &ucData1, 1);
                //mcSHOW_USER_MSG(("0x17A = 0x%02X\n", ucData1));

                // PRA
                ATD_GetReg(psAtdnaDemodCtx, 0x94D, &ucData1, 1);
                ATD_GetReg(psAtdnaDemodCtx, 0x94E, &ucData2, 1);
                u2Data = mcUNSIGN_2BYTE(ucData1, ucData2, 8, 8);
                mcSHOW_USER_MSG(("RF gain = 0x%04X\n", u2Data));
                ATD_GetReg(psAtdnaDemodCtx, 0x94F, &ucData1, 1);
                ATD_GetReg(psAtdnaDemodCtx, 0x950, &ucData2, 1);
                //u2Data = mcUNSIGN_2BYTE(ucData1, ucData2, 8, 8);
                u2Data = AtdNA_GetIFAGC();
                mcSHOW_USER_MSG(("IF gain = 0x%04X\n", u2Data));
                ucData1 = IO_READ8(VIDEO_IN0_BASE, 0x80);
                mcSHOW_USER_MSG(("TVD noise level = %d\n", ucData1));
            #if defined (CC_MT5387)
                //ATD_GetReg(psAtdnaDemodCtx, 0xA42, &ucData1, 1);
                u2Data = AtdNA_GetNoiseLevel();
                mcSHOW_USER_MSG(("ATD noise level = 0x%08X\n", u2Data));
                //ATD_GetReg(psAtdnaDemodCtx, 0x249, &ucData1, 1);
                //mcSHOW_USER_MSG(("Impulse noise indicator = %d\n", (ucData1 & 0x80) >> 7 ));
                ucData1 = 0x2E;
                ATD_SetReg(psAtdnaDemodCtx, 0x96C, &ucData1, 1);
                ATD_GetReg(psAtdnaDemodCtx, 0x96D, &ucData1, 1);
                ATD_GetReg(psAtdnaDemodCtx, 0x96E, &ucData2, 1);
                u2Data = mcUNSIGN_2BYTE(ucData1, ucData2, 8, 8);
                mcSHOW_USER_MSG(("DRO gain jitter = 0x%04X\n", u2Data));
                ATD_GetReg(psAtdnaDemodCtx, 0xA39, &ucData1, 1);
                ATD_GetReg(psAtdnaDemodCtx, 0xA3A, &ucData2, 1);
                u2Data = mcUNSIGN_2BYTE(ucData1, ucData2, 8, 8);
                mcSHOW_USER_MSG(("DRO signal swing = 0x%04X\n", u2Data));
                ATD_GetReg(psAtdnaDemodCtx, 0xA3B, &ucData1, 1);
                ATD_GetReg(psAtdnaDemodCtx, 0xA3C, &ucData2, 1);
                u2Data = mcUNSIGN_2BYTE(ucData1, ucData2, 8, 8);
                mcSHOW_USER_MSG(("DRO sync height = 0x%04X\n", u2Data));
            #endif
                if( ii < u1Count-1 )
                {
                    mcDELAY_MS(u2Period);
                }
                }

                break;
            case 'p':

            	      if('l'==((CHAR *) aszArgv[0])[1]){
            if (i4Argc > 1)
            {
               fgEnableATP = StrToInt(aszArgv[1]);
            }
            mcSHOW_USER_MSG(("fgEnableATP = %d\n", fgEnableATP));
           }
		    else if('d'==((CHAR *) aszArgv[0])[1])
			{
              if (i4Argc > 1)
               {
               psAtdnaDemodCtx->pTCtx->u2RetryTVDTm=(UINT16)StrToInt(aszArgv[1]);
               }
              mcSHOW_USER_MSG(("Re-try TVD delay = %d ms\n",psAtdnaDemodCtx->pTCtx->u2RetryTVDTm)); 
           }
           else{
                ucData2 = 0;
                if (i4Argc > 1)
                {
                    ucData2 = StrToInt(aszArgv[1]);
                }
                ucData1 = 0x00;
                ATD_SetReg(psAtdnaDemodCtx, 0x00A, &ucData1, 1);
                //ucData1 = 0xC9;
                ucData1 = 0x09; //EC_code_d9B05: Correct disable CR auto-reacq
                ATD_SetReg(psAtdnaDemodCtx, 0x997, &ucData1, 1);
                ucData1 = 0x30 + ucData2;
                ATD_SetReg(psAtdnaDemodCtx, 0x998, &ucData1, 1);
                ATD_SetReg(psAtdnaDemodCtx, 0x9B2, &ucData1, 1);
                ucData1 = 0x40 + ucData2;
                ATD_SetReg(psAtdnaDemodCtx, 0x9A9, &ucData1, 1);
                ATD_SetReg(psAtdnaDemodCtx, 0x9B3, &ucData1, 1);

                mcSHOW_USER_MSG(("Set CR loop bandwidth as %d\n", ucData2));
            }
                break;

            case 'q':
            {
                UINT32 ucPar1 = (U32)StrToInt(aszArgv[1]);
                UINT32 ucPar2 = (U32)StrToInt(aszArgv[2]);
                UINT32 ucPar3 = (U32)StrToInt(aszArgv[3]);
                Qlab_SetMonIndicator(ucPar1,ucPar2,ucPar3);
            }
            	break;

            case 'x':
                i4CurFreq = psAtdPdCtx->Frequency;

                if(i4Argc==1)
                {
                    ITuner_OP(psAtdnaDemodCtx->pTCtx, itGetLNA, 0, &ucData1);
                    mcSHOW_USER_MSG(("LNA status = %d\n", ucData1));
                }
                else if (i4Argc > 1)
                {
                    //ITuner_OP(psAtdeuDemodCtx->pTCtx, itSetLNA, StrToInt(aszArgv[1]), &ucData1);
                    //ATD_SetFreq(psAtdeuDemodCtx, i4CurFreq, FALSE);
                    AtdNA_SetLNA(StrToInt(aszArgv[1]));
                    mcSHOW_USER_MSG(("Set LNA = %d\n", StrToInt(aszArgv[1])));
                }
            break;

#ifdef ANAUS_ENABLE_MONITOR
            case 'm':
                {
                    UINT8   ucCmdExt;

                    ucCmdExt = ((CHAR *) aszArgv[0])[1];

                    if (ucCmdExt == 'p')
                    {
						SetBreakMonitor(psAtdPdCtx);
                    }
                    else if (ucCmdExt == 'r')
                    {
						ResumeMonitor(psAtdPdCtx);
                    }
                }
                break;
#endif

            case 'l':
            {
	            S32 Record[10] = {0,0,0,0,0,0,0,0,0,0};
                //U8  u1Data1, u1Data2;
                U16  u2Data1, u2Data2;

                //Get ATD TVD Indicator
                for (ii=0; ii<5; ii++)
                {
                    //PRA
                    ATD_GetReg(psAtdnaDemodCtx, 0x95E, &ucData1, 1);
                    mcSHOW_USER_MSG(("0x95E = 0x%02X\n", ucData1));
                    Record[0] += (ucData1 & 0x40) >> 6;

                    // CR
                    s2Data = ATD_GetCFOKHz(psAtdnaDemodCtx, FALSE);
                    mcSHOW_USER_MSG(("CFO = %d KHz\n", s2Data));
                    if (s2Data < 0)
                    	s2Data = -s2Data;
                    Record[1] += s2Data;
                    ATD_GetReg(psAtdnaDemodCtx, 0x9A0, &ucData1, 1);
                    mcSHOW_USER_MSG(("0x9A0 = 0x%02X\n", ucData1));
                    Record[2] += ucData1 & 0x3;
                    ATD_GetReg(psAtdnaDemodCtx, 0x9A1, &ucData1, 1);
                    mcSHOW_USER_MSG(("0x9A1 = 0x%02X\n", ucData1));
                    Record[3] += (ucData1 & 0x20) >> 5;

                    //TVD
                    ucData1 = IO_READ8(VIDEO_IN0_BASE, 0x80);
                    mcSHOW_USER_MSG(("TVD noise level = %d\n", ucData1));
                    Record[4] += ucData1;
                    ucData1 = (IO_READ8(VIDEO_IN0_BASE, 0x81) & 0x10) >> 4;
                    mcSHOW_USER_MSG(("Vpress = %d\n", ucData1));
                    Record[5] += ucData1;
                    u2Data = IO_READ32(VIDEO_IN0_BASE, 0x44);
                    u2Data1= (u2Data& 0x3FF0000) >> 16; //Sync Level
                    u2Data2 = u2Data & 0x000003FF; //Blank Level
                    mcSHOW_USER_MSG(("Sync (0x%3x) and Blank (0x%3x)\n",u2Data1,u2Data2));
                    Record[6] += u2Data1;
                    Record[7] += u2Data2;

                    {
	    	        UINT32 RegTemp = 0;
	    	        UINT32 dRegTemp = 0;
	    	        //UINT16 Values[6];
	    	        //1 1.Enable Catch Sel
	    	        RegTemp = IO_READ32(VIDEO_IN0_BASE, 0x5EC);
	    	        RegTemp |= 0x00008000; //Bit 15
	    	        IO_WRITE32(VIDEO_IN0_BASE, 0x5EC, RegTemp);

	    	        //1 2.Select Probe Signal
	    	        RegTemp = IO_READ32(VIDEO_IN0_BASE, 0x61C);
	    	        RegTemp &= 0x0FFFFFFF; //Bit 31 ~28
	    	        RegTemp |= 0xB0000000; //Bit 31 ~28
	    	        IO_WRITE32(VIDEO_IN0_BASE, 0x61C, RegTemp);
	    	        mcDELAY_MS(100);

	    	        //1 3. Get Indicator
	    	        dRegTemp = IO_READ32(VIDEO_IN0_BASE, 0x0A8);
	    	        mcSHOW_USER_MSG(("V Sync Loss sum during 64 fields %d\n", dRegTemp & 0x3F));
                                           Record[8] += dRegTemp & 0x3F;
	    	        mcSHOW_USER_MSG(("H Sync Loss sum during 64 fields %d\n", (dRegTemp >> 6) & 0x3FFF));
                                           Record[9] += ((dRegTemp >> 6) & 0x3FFF);
	                }
 	                 mcDELAY_MS(400);
                } //End for(ii=0; ii<5; ii++)

                //Average ATD TVD Indicator
                for(ii=0;ii<10;ii++)
                {
	              Record[ii] /= 5;
	              mcSHOW_USER_MSG(("%d ", Record[ii]));
                }
                mcSHOW_USER_MSG(("\n"));

                //Check ATD TVD Indicator
                u2State = 0;
                if (Record[0] == 0x01)  // PrA should be in the tracking state
                    u2State += mcBIT(0);
                if (Record[1] < 50) // CFO < 50KHz.
                    u2State += mcBIT(1);
                if (Record[2] == 0x03) // CR should be int the tracking state
                	u2State += mcBIT(2);
                if (Record[3] == 0x01) // Pilot good stability
                	u2State += mcBIT(3);
                if (Record[4] < 0x6) // TvD noise level => Need to tune.
                	u2State += mcBIT(4);
                if (Record[5] == 1) // TvD VPRES
                	u2State += mcBIT(5);
                if ((Record[6] > 33) || (Record[6] < 36)) // TVD sync level
                	u2State += mcBIT(6);
                if ((Record[7] > 250) || (Record[7] < 262)) // Blank Level ~ 0x100.
                	u2State += mcBIT(7);
                if (Record[8] == 0) // V Sync Loss sum during 64 fields?
                	u2State += mcBIT(8);
                if (Record[9] == 576) // H Sync Loss sum during 64 fields
                	u2State += mcBIT(9);

                if(u2State == 0x3FF)
                {
                	mcSHOW_USER_MSG(("VAGood\n"));
                }
                else
                {
                      Printf("ATD SLT video fail(0x%03X)\n", u2State);
        	          mcSHOW_USER_MSG(("FAIL\n"));
                }
            }
                break;

            default:
                mcSHOW_USER_MSG(("Invalid parameter!\n"));
                break;
        }
    }
    mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
}

//-----------------------------------------------------------------------------
/*
 *  AtdNA_TunerGetAttribute
 *  Not Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @retval   none
 */
//-----------------------------------------------------------------------------
INT16 AtdNA_TunerGetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                           DRV_GET_TYPE_T  e_get_type,
                           VOID *pvAnaAttribute,
                           SIZE_T *pzAnaAttributeLen)
{
#if fcADD_HAL_LOCK
ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;
#endif

    UINT16 u2Value;

    mcSEMA_LOCK_HAL(psAtdPdCtx->hHalLock);  // Avoid re-entry

    switch (e_get_type)
    {
        case TUNER_GET_TYPE_RFAGC:
        {
            u2Value = ATD_GetRFAGC(psAtdnaDemodCtx);
            *(UINT16*)pvAnaAttribute = u2Value;
            *pzAnaAttributeLen = sizeof(UINT16);
            break;
        }

        default:
        {
            mcSHOW_DBG_MSG4(("%s: RMR_DRV_INV_GET_INFO!\n", __FUNCTION__));

            mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
            return (RMR_DRV_INV_GET_INFO);
        //    break;
        }
    }
    mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
    return (RMR_OK);

}

//-----------------------------------------------------------------------------
/*
 *  AtdNA_TunerSetAttribute
 *  Not Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @retval   none
 */
//-----------------------------------------------------------------------------
INT32 AtdNA_TunerSetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                           DRV_SET_TYPE_T  e_set_type,
                           const VOID *pvAnaAttribute,
                           SIZE_T zAnaAttributeLen)
{
#if fcTUNE_NOT_START
ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;
#endif

//    mcSEMA_LOCK_HAL(psAtvPdCtx->hHalLock);  // Avoid re-entry

    switch (e_set_type)
    {

#if fcTUNE_NOT_START
        case TUNER_SET_SPECIAL_TYPE_ACQ_ACTION_CLASS:
            if (zAnaAttributeLen)
            {
            TUNER_ACQ_ACTION_T *peActionType = (TUNER_ACQ_ACTION_T *) pvAnaAttribute;

                mcSHOW_DBG_MSG(("%s: (%d, %X) eActionType=%d!\n", __FUNCTION__,
                                TUNER_SET_SPECIAL_TYPE_ACQ_ACTION_CLASS,
                                TUNER_SET_SPECIAL_TYPE_ACQ_ACTION_CLASS, *peActionType));
                psAtdPdCtx->u1TunerAcqType = *peActionType;
            }
            break;
#endif

        default:
        {
            mcSHOW_DBG_MSG4(("%s: RMR_DRV_INV_GET_INFO!\n", __FUNCTION__));
//            mcSEMA_UNLOCK_HAL(psAtvPdCtx->hHalLock);
            return (RMR_DRV_INV_GET_INFO);
        //    break;
        }
    }
//    mcSEMA_UNLOCK_HAL(psAtvPdCtx->hHalLock);
#if fcTUNE_NOT_START
    return (RMR_OK);
#endif
}

/***********************************************************************/
#ifdef fcADD_DIAG_INFO

#define DRV_CUSTOM_TUNER_DIAG_ITEMS         3

#if 0 //Pluto
/************************************************************************
*  AtdNA_TunerSetDiagCmd
*  Set analog tuner diagnostic information.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @param  e_tuner_set_type : Diagnostic information type.
*  @param  *pv_set_info : Input data content pointer.
*  @param  z_size : Input data length.
*  @retval  RMR_OK : Set analog tuner diagnostic information successfully.
*  @retval  RMR_DRV_SET_FAILED : Set analog tuner diagnostic information fail.
*  @retval  RMR_DRV_INV_SET_INFO : Invalid analog tuner diagnostic information.
************************************************************************/
INT32 AtdNA_TunerSetDiagCmd(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        DRV_CUSTOM_TUNER_SET_TYPE_T     e_tuner_set_type,
                        VOID*                           pv_set_info,
                        SIZE_T                          z_size)
{
	INT32	retSts = RMR_OK;
//#ifdef TUNER_DVBT_ENABLE_AGC_CTRL /*fix build warning*/
    ATV_PD_CTX_T *psAtvPdCtx = (ATV_PD_CTX_T *) ptTDSpecificCtx;


    mcSEMA_LOCK_HAL(psAtvPdCtx->hHalLock);  // Avoid re-entry
//#endif /*TUNER_DVBT_ENABLE_AGC_CTRL*/

    switch(e_tuner_set_type)
    {
        case DRV_CUSTOM_TUNER_SET_TYPE_DIAG_OPEN:
            // Do initialization here
            break;

        case DRV_CUSTOM_TUNER_SET_TYPE_DIAG_CLOSE:
            // Do cleanup here
            break;
//#ifdef TUNER_DVBT_ENABLE_AGC_CTRL
    case DRV_CUSTOM_TUNER_SET_TYPE_AGC:
        {
             UINT32 u4Ofst[1];
	      UINT8  u1Val[1];
            //GEN_TUNER_CTX_T *pDigiTunerCtx;
            psAtvPdCtx->sAtvPiCtx.u1AnaDemoTop = ((TUNER_TYPE_DIAG_AGC_T *) pv_set_info)->ucAgcValue;
            if ( !AnalogPINA_TunerSetSystem(cATUNER_PI_CTX(psAtvPdCtx), AnalogPINA_TunerGetSystem(cATUNER_PI_CTX(psAtvPdCtx))) )
	    {
                //PDWNC_WriteErrorCode(ERR_I2C_DEMOD_ANALOG);
                mcSHOW_DBG_MSG3(("TunerSetDiagCmd: set ATV TOP fail!!\n"));
            }
	     else
	     	{
	        mcSHOW_DBG_MSG3(("TunerSetDiagCmd: set ATV TOP = %d\n", psAtvPdCtx->sAtvPiCtx.u1AnaDemoTop));

		#ifdef TopandFMTrap
		u1Val[0] = psAtvPdCtx->sAtvPiCtx.u1AnaDemoTop;
		u4Ofst[0] = DRVCUST_OptGet( eTunerAnaDemoTop );
		//u4Ofst[0] = DRVCUST_OptGet( 29 );
		if(!EEPROM_Write((UINT64)u4Ofst[0], (UINT32)(void *)&u1Val[0], 1))
		 {
		    mcSHOW_DBG_MSG3(("Write EEPROM fail!!\n"));
		 }
		else
		 {
		   mcSHOW_DBG_MSG3(("Write EEPROM AnaDemoTop = %d\n", psAtvPdCtx->sAtvPiCtx.u1AnaDemoTop));
		 }
		#endif
	     	}
        }
	        break;

    case DRV_CUSTOM_TUNER_SET_TYPE_FMTRAP:
        {

            RF_TUNER_CTX_T *pDigiTunerCtx;
	     UINT32  wFreqInKHz;
	     UINT8    u1AnaMode;
	     UINT32 u4Ofst[1];
	     UINT8  u1Val[1];

            wFreqInKHz = psAtvPdCtx->sAtvPiCtx.u4Freq;
	    u1AnaMode = psAtvPdCtx->sAtvPiCtx.u1AnaMode;
            pDigiTunerCtx = psAtvPdCtx->sAtvPiCtx.pDigiTunerCtx;
            pDigiTunerCtx->u1FmTrap = ((TUNER_TYPE_DIAG_AGC_T *) pv_set_info)->ucFmTrap;
            if ( !AnalogPINA_TunerSetFreq(cATUNER_PI_CTX(psAtvPdCtx), wFreqInKHz, u1AnaMode))
	     {
                //PDWNC_WriteErrorCode(ERR_I2C_DEMOD_ANALOG);
               mcSHOW_DBG_MSG3(("TunerSetDiagCmd: set FM Trap fail!!\n"));
            }
	     else
	     	{
	        mcSHOW_DBG_MSG3(("TunerSetDiagCmd: set FM Trap = %d\n", pDigiTunerCtx->u1FmTrap));

		#ifdef TopandFMTrap
		 u1Val[0] = pDigiTunerCtx->u1FmTrap;
		 u4Ofst[0] = DRVCUST_OptGet( eTunerPLLFMTrapBit );
		 //u4Ofst[0] = DRVCUST_OptGet( 30 );
		 if(!EEPROM_Write((UINT64)u4Ofst[0], (UINT32)(void *)&u1Val[0], 1))
		  {
		     mcSHOW_DBG_MSG3(("TunerSetDiagCmd: Write EEPROM fail!!\n"));
		  }
		 else
		  {
		    mcSHOW_DBG_MSG3(("Write EEPROM FM Trap Bit = %d\n", pDigiTunerCtx->u1FmTrap));
		  }
            #endif

	     	}
        }
	        break;
//#endif /*TUNER_DVBT_ENABLE_AGC_CTRL*/
        default:
            retSts = RMR_DRV_INV_SET_INFO;
            break;
    }

//#ifdef TUNER_DVBT_ENABLE_AGC_CTRL
    mcSEMA_UNLOCK_HAL(psAtvPdCtx->hHalLock);
//#endif /*TUNER_DVBT_ENABLE_AGC_CTRL*/
    return retSts;
}
#endif

INT32 AtdNA_TunerGetDiagInfo(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                              DRV_CUSTOM_TUNER_GET_TYPE_T   e_tuner_get_type,
                              VOID                           *pv_get_info,
                              SIZE_T                        *pz_size)
{
ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;
TUNER_TYPE_DIAG_CONTENT_T *pDiagContent = (TUNER_TYPE_DIAG_CONTENT_T *) pv_get_info;

    mcSEMA_LOCK_HAL(psAtdPdCtx->hHalLock);  // Avoid re-entry

    switch (e_tuner_get_type)
    {
        case DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTROL:
            if ((*pz_size) < sizeof(TUNER_TYPE_DIAG_CONTROL_T))
            {
                *pz_size = sizeof(TUNER_TYPE_DIAG_CONTROL_T);
                mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            ((TUNER_TYPE_DIAG_CONTROL_T *) pv_get_info)->u2PollPeriodMs = 2000;
            ((TUNER_TYPE_DIAG_CONTROL_T *) pv_get_info)->ucCntOfDiagStruc = DRV_CUSTOM_TUNER_DIAG_ITEMS;
            break;

        case DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTENT:
            if ((*pz_size) < DRV_CUSTOM_TUNER_DIAG_ITEMS * sizeof(TUNER_TYPE_DIAG_CONTENT_T))
            {
                *pz_size = DRV_CUSTOM_TUNER_DIAG_ITEMS * sizeof(TUNER_TYPE_DIAG_CONTENT_T);
                mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }

        // To add new items, please remember to modify DRV_CUSTOM_TUNER_DIAG_ITEMS
        // Note that both cDisplayName and cDisplayValue can NOT exceed 15 bytes

        // Record 1
            sprintf(pDiagContent->cDisplayName, "%s", "Version:");
            sprintf(pDiagContent->cDisplayValue, "%s", "MTK_ATD");
            pDiagContent++;

        // Record 2
            sprintf(pDiagContent->cDisplayName, "%s", "RF Freq (KHz):");
            sprintf(pDiagContent->cDisplayValue, "%6d", psAtdnaDemodCtx->pTCtx->u4RF_Freq);
            pDiagContent++;

        // Record 3
            sprintf(pDiagContent->cDisplayName, "%s", "Lock State:");
            sprintf(pDiagContent->cDisplayValue, "%d", psAtdPdCtx->u1VIFLock);
            pDiagContent++;

            break;                          // End of case DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTENT

        default:
            mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
            return RMR_DRV_NO_GET_INFO;
    }
    mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
    return RMR_OK;
}
#endif

//Pluto : I will move to ATD driver once ATD function enable
/*Jackson: need refine this api, move to correct location*/
BOOL MT5381ATD_TunerSwitchRfAgc(VOID)
{
//Reset (34H & 35H)
    //Printf("+++Pluto+++ Clear 0x34\n");
//Switch RF AGC from ATD to DTD
    mcSHOW_DBG_MSG(("w 0x2002A034 0x00000000\n"));
    IO_WRITE32(cRISC_ATD_BASE, 0x34, 0x00000000);
    return TRUE;
}
#if 0 //remove on mt539x, to fix warning zero used for undefined preprocessing identifier
//Pluto : I will move to ATD driver once ATD function enable
/*Jackson: need refine this api, move to correct location*/
BOOL Ana_SwitchRfAgc(VOID)
{
#if fcTUNER_TYPE == cANA_MT5381ATD
    return MT5381ATD_TunerSwitchRfAgc();
#else
    return FALSE;
#endif
}
#endif
/***********************************************************************/
/*                  Acquisition APIs                                   */
/***********************************************************************/
/***********************************************************************/
/*                  Public Functions (not static)                      */
/***********************************************************************/
VOID Tuner_AtdNA_Register_LTDIS_Fct(PTD_CTX ptTDCtx)
{
  TUNER_DEVICE_CTX_T*     pTunerDeviceCtx = ptTDCtx;

    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerClose = AtdNA_TunerClose;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerAcq = AtdNA_TunerAcq;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGerSync = AtdNA_TunerGetSync;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignal = AtdNA_TunerGetSignal;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLevel = AtdNA_TunerGetSignalLevel;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLeveldBm = AtdNA_TunerGetSignalLeveldBm;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalPER = AtdNA_TunerGetSignalPER;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalSNR = AtdNA_TunerGetSignalSNR;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetMpgFmt = AtdNA_TunerSetMpgFmt;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetVer = AtdNA_TunerGetVer;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerNimTest = AtdNA_TunerNimTest;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerCommand = AtdNA_TunerCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerAtdCommand = AtdNA_TunerAtdCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetRegSetting = AtdNA_TunerSetRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerShowRegSetting = AtdNA_TunerShowRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerTestI2C = AtdNA_TunerTestI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerRegRead = AtdNA_TunerRegRead;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDisc = AtdNA_TunerDisc;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetAttribute = AtdNA_TunerSetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute = AtdNA_TunerGetAttribute;
#ifdef fcADD_DIAG_INFO
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetDiagInfo = AtdNA_TunerGetDiagInfo;
//pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetDiagCmd  = AtdNA_TunerSetDiagCmd;
#endif
}
