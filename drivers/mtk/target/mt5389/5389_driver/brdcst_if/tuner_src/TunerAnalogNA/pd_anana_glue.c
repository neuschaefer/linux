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
#include "pd_anana_glue.h"
#include "UtilAPI.h"
#include "TDMgr.h"

//#include "PI_Def_dbg.h"



#ifdef fcADD_DIAG_INFO
    #ifdef CC_MT5381
#include "u_drv_cust.h"
    #else
//#include "d_5371_cust.h"
    #endif
#endif
#include "pi_anana_tvsys_info.h"
#include "pd_anana_picommonapi.h"
#include "nptv_drvif.h"
#include "x_stl_lib.h"
#include "eepdef.h"

#if 0
#ifdef TUNER_ANALOG_NEED_INIT_TUNER
#include "rf_tuner_api.h"
#include "dtd_if.h"  //here we need tuner context only
#endif
#endif

#include "tuner_interface_if.h"
#include "pd_tuner_type.h"
#include "ctrl_bus.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#define fcDUMMY_ANA_TUNER       (0 && fcINTERNAL_TEST)
#define fcTEST_I2C_DETACH       ((0 && fcINTERNAL_TEST) || 0)
#define fcMULTI_REG_RW_DEBUG    1
#define fcFINE_TUNE_SCAN        0           // 1: PI decide fine-tune or not in TunerSearchNearbyFreq

#if 0  //Jackson remove fcADD_CUST_IF to fix zero used for undefined preprocessing identifier
#define fcADD_CUST_IF           (1 && (fcTUNER_TYPE == cANA_TUNER_ALL) && defined(CC_MT5381))
#endif

#if !defined(CC_MTAL) && !defined(CC_SDAL) //linux platform
#define ANAUS_ENABLE_MONITOR
#else

#endif

#define fcADD_TVD_SYNC         // Pause TVD before TunerAcq complete, or will show "No Signal"
#define fcADD_AV_SYNC           1
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define cMONITOR_PERIOD_MS      500

#define fcENABLE_FineTune

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

//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------
static INT32 i4FreqOffset = 0;
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

#if 0
//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------
#ifdef TUNER_ANALOG_NEED_INIT_TUNER
EXTERN TUNER_API_FCT_TBL_T *pTuner_API_Fct;
EXTERN VOID Tuner_Register_Fct(TUNER_API_FCT_TBL_T *ptuner_Fct);
EXTERN VOID *GetUSTunerCtx(VOID);
RF_TUNER_CTX_T  *psTunerCtx;
#endif
#endif
//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern  VOID    DigTunerBypassI2C(BOOL bSwitchOn);
extern UINT8 bGetSignalType(UINT8 bPath);
extern UINT32 PDWNC_ReadServoADCChannelValue(UINT32 u4Channel);
extern void AUD_WaitDspFlush(UINT8 u1DecId);


extern ANANA_PICOMMONAPI_FCT_TBL_T *pAnana_PiCommonAPI_Fct;

#if  0
#if (fcTUNER_TYPE != cDTD_TUNER_ALL)
EXTERN VOID Tuner_Anana_PiCommonAPI_Register_Fct(ANANA_PICOMMONAPI_FCT_TBL_T *pcom_Fct);
#else
EXTERN VOID Tuner_Anana_CommonAPI_Register_Fct(ANANA_PICOMMONAPI_FCT_TBL_T *pcom_Fct, UINT8 tunerType);
EXTERN UINT8 GetTunerType(VOID);
#endif
#endif

EXTERN VOID Tuner_Anana_PiCommonAPI_Register_Fct(ANANA_PICOMMONAPI_FCT_TBL_T *pcom_Fct, UINT8 tunerType);

/***********************************************************************/
/*              External declarations                                  */
/***********************************************************************/
#if defined(CC_DRIVER_DEMO) || defined(CC_LINUX_KERNEL)
#define _Tvd_NotifyTVSig
#else
VOID _Tvd_NotifyTVSig(UINT8 u1Path, UINT8 u1SigState);
#endif

/***********************************************************************/
/*              Private (static) Function Prototypes                   */
/***********************************************************************/
VOID Tuner_AnalogNA_Register_LTDIS_Fct(PTD_CTX ptTDCtx);
VOID AnalogNA_TunerAtdCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,
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
static BOOL SetBreakMonitor(ATV_PD_CTX_T *psAtvPdCtx)
{
    UINT8 u1Cnt=0;

    psAtvPdCtx->fgBreakMonitor = TRUE; //set to break monitor

    if(psAtvPdCtx->fgPIMoniStatus){  //PI really doing PI Monitor
        while(psAtvPdCtx->fgPIMoniStatus){
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
static void ResumeMonitor(ATV_PD_CTX_T *psAtvPdCtx)
{
    psAtvPdCtx->fgBreakMonitor = FALSE;
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
    ATV_PD_CTX_T *psPdCtx = (ATV_PD_CTX_T *) pvArg;
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

    ATV_PD_CTX_T *psPdCtx = *(ATV_PD_CTX_T **) pvArg;

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
//            mcSHOW_DBG_MSG(("===Monitor Thread===\n"));
//            mcDELAY_MS(cMONITOR_PERIOD_MS);
	    	   mcSEMA_LOCK_HAL(psPdCtx->hHalLock);
		       //Add Monitor ISP here
             psPdCtx->fgPIMoniStatus = TRUE; //doing PI monitor
	         AnalogPINA_TunerMonitor(cATUNER_PI_CTX(psPdCtx),
                         &(psPdCtx->u4AcqFrequency),
                          i4CheckNoticeBreak, psPdCtx);
             psPdCtx->fgPIMoniStatus = FALSE; //finish doing PI monitor or breaked
		       mcSHOW_DBG_MSG2(("===ANA_NA Monitor Thread ===\n"));
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
STATIC S32 PD_StartMonitorEngine(ATV_PD_CTX_T *psAtvPdCtx)
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
            MonitorProcess, sizeof(void*), (VOID*)&psAtvPdCtx) != OSR_OK)
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
UINT8 au1Data[cIF_DATA_SIZE+1];

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
STATIC UINT8 GetAnaConnMode(UINT8 eMod, BOOL bFineTune )
{
UINT8   u1ConnMode;

    if (eMod != MOD_PSK_8)
    {
      //  _fgAutoSearch = TRUE;
        if (eMod == MOD_UNKNOWN)
         {
            u1ConnMode = ATV_CONN_MODE_CHAN_SCAN;
//	     fgUpdCurFreq = FALSE;
         }
        else
         {
            u1ConnMode = ATV_CONN_MODE_FINE_TUNE;
//	     fgUpdCurFreq = TRUE;
         }
    }
    else
    {
      //  _fgAutoSearch = FALSE;
        u1ConnMode = ATV_CONN_MODE_CHAN_CHANGE;
//	  fgUpdCurFreq = TRUE;
    }
   if (!bFineTune)
   	 //_fgAutoSearch = TRUE;
   {
       mcSHOW_DBG_MSG(( "Now is fine tuning\n"));
   }
    mcSHOW_DBG_MSG(( "Mod_type = %d\n", u1ConnMode));

    return u1ConnMode;
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

//-----------------------------------------------------------------------------
/*
 *  AnalogNA_TunerOpen
 *  Open analog tuner driver
 *  @param  ptTDCtx  a void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @param  ptTunerCfgInfo    tuner config information
 *  @param  pptTDSpecificCtx
 *  @retval   DRVAPI_TUNER_OK : Success.
 */
//-----------------------------------------------------------------------------
INT32 AnalogNA_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo,
                     PTD_SPECIFIC_CTX* pptTDSpecificCtx)
{
    ATV_PD_CTX_T* psAtvPdCtx = NULL;
    UINT8 u1SubSysIdx;
//#if (fcTUNER_TYPE == cITUNER_ALL)
    TUNER_DEVICE_CTX_T* pTunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;
//#endif

    Tuner_AnalogNA_Register_LTDIS_Fct(ptTDCtx);
// Allocate TS_SPECIFIC_CTX for Instance
    psAtvPdCtx = (ATV_PD_CTX_T *) mcALLOC_MEM(sizeof(ATV_PD_CTX_T));
    if (psAtvPdCtx == NULL)
    {
        return (DRVAPI_TUNER_ERROR);
    }
    *pptTDSpecificCtx = psAtvPdCtx;

#if 0
#if (fcTUNER_TYPE != cDTD_TUNER_ALL)
 #if (fcTUNER_TYPE != cITUNER_ALL)
	Tuner_Anana_PiCommonAPI_Register_Fct(pAnana_PiCommonAPI_Fct);
 #else
	switch (pTunerDeviceCtx->tunerType)
    {
    case cNXP_UV1336:
        UV1336_Anana_PiCommonAPI_Register_Fct(pAnana_PiCommonAPI_Fct);
        break;
    default:
        mcSHOW_HW_MSG(("%s: wrong tuner type\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);
    }
 #endif
#else//TunerAll
	Tuner_Anana_CommonAPI_Register_Fct(pAnana_PiCommonAPI_Fct, GetTunerType());
#endif

#endif

 Tuner_Anana_PiCommonAPI_Register_Fct(pAnana_PiCommonAPI_Fct, pTunerDeviceCtx->tunerType);

#if 0
#ifdef TUNER_ANALOG_NEED_INIT_TUNER
	Tuner_Register_Fct(pTuner_API_Fct); //tuner code registration for analog only project. Due to ROW project is analog only, we try to remove out tuner init code from digital demod driver.
//	Rf_Tuner_Init(&sTunerCtx, u1RFTunerI2cAddr);
    psTunerCtx = GetUSTunerCtx();
    Rf_Tuner_Init(psTunerCtx, 0xc2); //todo: modify tuneri2caddr
//        DTD_SetTunerPara(psDemodCtx); //all thing done in DTD_SetTunerPara() need move to Rf_Tuner_Init?
#endif
#endif

    if (!AnalogPINA_TunerInit(cATUNER_PI_CTX(psAtvPdCtx),
                    ptTunerCfgInfo->ucDemodInbAddr,
                    ptTunerCfgInfo->ucRFTunerAddr))
        return (DRVAPI_TUNER_ERROR);

#if fcADD_HAL_LOCK
    if (mcSEMA_CREATE(&psAtvPdCtx->hHalLock, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_HW_MSG(("%s: mcSEMA_CREATE(hHalLock) Fail!\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);
    }
    mcSEMA_UNLOCK_HAL(psAtvPdCtx->hHalLock);
#endif

#if 0  //Jackson remove fcADD_CUST_IF to fix zero used for undefined preprocessing identifier
#if fcADD_CUST_IF
    psAtvPdCtx->u4DrvCustTunerCtrl = Analog_GetCtmrReq();
#endif
#endif

#if fcTUNE_NOT_START
    psAtvPdCtx->u1TunerAcqType = ACQ_ACTION_DEFAULT;
#endif
    u1SubSysIdx = SYS_SUB_NTSC_M;

#if !fcDUMMY_ANA_TUNER
    DigTunerBypassI2C(TRUE);
    AnalogPINA_TunerSetSystem(cATUNER_PI_CTX(psAtvPdCtx), u1SubSysIdx);
    DigTunerBypassI2C(FALSE);
#endif

//#if !defined(CC_MTAL) && !defined(CC_SDAL)
#ifdef ANAUS_ENABLE_MONITOR
    if (mcSEMA_CREATE(&t_escape_mon_suspend, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_DBG_MSG(("MT5391_TunerOpen (mcSEMA_CREATE): ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }

    psAtvPdCtx->fgBreakMonitor = TRUE;//init
    psAtvPdCtx->fgPIMoniStatus =FALSE; //init
    // Start Monitor Engine
    PD_StartMonitorEngine(psAtvPdCtx);
#endif

    return (DRVAPI_TUNER_OK);
}

//-----------------------------------------------------------------------------
/*
 *  AnalogNA_TunerClose
 *  To close analog tuner driver
 *  @param  ptTDCtx  a void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @retval   DRVAPI_TUNER_OK : Success.
 */
//-----------------------------------------------------------------------------
INT32 AnalogNA_TunerClose(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{

ATV_PD_CTX_T *psAtvPdCtx = (ATV_PD_CTX_T *) ptTDSpecificCtx;

//    fgIsDisconnected = FALSE;
//#if !defined(CC_MTAL) && !defined(CC_SDAL)
#ifdef ANAUS_ENABLE_MONITOR
    psAtvPdCtx->fgBreakMonitor = TRUE;
    fgMonitorThreadRunning = FALSE; //need set to false before mcMUTEX_UNLOCK(t_escape_mon_suspend);
    mcMUTEX_UNLOCK(t_escape_mon_suspend);
    PD_StopMonitorEngine();
    mcMUTEX_DESTROY(t_escape_mon_suspend);
#endif

#if fcADD_HAL_LOCK
    if (mcSEMA_DESTROY(psAtvPdCtx->hHalLock) != OSR_OK)
    {
        mcSHOW_HW_MSG(("%s: mcSEMA_DESTROY(hHalLock) Fail!\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);
    }
#endif

    AnalogPINA_TunerClose(cATUNER_PI_CTX(psAtvPdCtx));

    mcFREE_MEM(ptTDSpecificCtx);
    return (DRVAPI_TUNER_OK);
}
BOOL fgEnableATP=FALSE;//Flag to check whether show  Driver ATP Auto Test log

BOOL AnalogNA_TunerAcq(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                    DRV_CONN_TYPE_T e_conn_type,
                    VOID* pv_conn_info,
                    SIZE_T z_conn_info_len,
                    x_break_fct _BreakFct,
                    VOID* pvArg)
{
ATV_PD_CTX_T *psAtvPdCtx = (ATV_PD_CTX_T *) ptTDSpecificCtx;
INT32   i4CurFreq = cTUNER_DEFAULT_FREQ;
UINT32  u4Freq; //u4FreqBound;
UINT8   u1Path = SV_VP_MAX;
UINT8   u1ATVStatus = 0;
UINT8   u1ConnMode;
BOOL    fgLockSts = TRUE;
UINT8   u1AnaMode;
STATIC  UINT8 ScanedNo=0;//counter for scaned channel number
//UINT8   u1ScanType = eTUNER_UNI_AUTO_SCAN; //variable "u1ScanType" was declared but never referenced
#if !fcFINE_TUNE_SCAN
BOOL    fgFineTune = FALSE;
#endif
PARAM_SETFREQ_T param;

BOOL    bFineTune;//for judge if have the function of fine-tune




#ifdef ANAUS_ENABLE_MONITOR
    SetBreakMonitor(psAtvPdCtx);
#endif

    mcSEMA_LOCK_HAL(psAtvPdCtx->hHalLock);  // Avoid re-entry

//	psAtvPdCtx->fgBreakMonitor = TRUE;

    _bSetChannelCnt = 0x40;
    if ((e_conn_type != TUNER_CONN_TYPE_CAB_ANA) &&
        (e_conn_type != TUNER_CONN_TYPE_TER_ANA))
    {
    //    mcVERIFY_ASSERT(0);
        mcSHOW_HW_MSG(("%s: Wrong e_conn_type (%d)!\n", __FUNCTION__, e_conn_type));

        mcSEMA_UNLOCK_HAL(psAtvPdCtx->hHalLock);
        return FALSE;

    }

    if ((e_conn_type == TUNER_CONN_TYPE_CAB_ANA))
    {
        u4Freq = ((TUNER_CAB_ANA_TUNE_INFO_T *) pv_conn_info)->ui4_freq;
       // u4FreqBound = u4Freq;

       #ifndef fcENABLE_FineTune
        ((TUNER_CAB_ANA_TUNE_INFO_T *) pv_conn_info)->b_fine_tune = TRUE;
	#endif
        bFineTune = ((TUNER_CAB_ANA_TUNE_INFO_T *) pv_conn_info)->b_fine_tune;
#ifndef CC_MT5391_AUD_3_DECODER
        AnaPd_ChangeChannel(u4Freq);
#endif
        u1ConnMode = GetAnaConnMode(((TUNER_CAB_ANA_TUNE_INFO_T *) pv_conn_info)->e_mod,\
			          ((TUNER_CAB_ANA_TUNE_INFO_T *) pv_conn_info)->b_fine_tune);
    //    u4Freq = (u4Freq + cANA_TUNER_IF_FREQ)/62500;

	 u1AnaMode = MOD_ANA_CABLE;
    }
    else if ((e_conn_type == TUNER_CONN_TYPE_TER_ANA))
    {
        u4Freq = ((TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info)->ui4_freq;
#ifndef CC_MT5391_AUD_3_DECODER
        AnaPd_ChangeChannel(u4Freq);
#endif

      #ifndef fcENABLE_FineTune
        ((TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info)->b_fine_tune = TRUE;
      #endif
        bFineTune = ((TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info)->b_fine_tune;

        u1ConnMode = GetAnaConnMode(((TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info)->e_mod,\
			            ((TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info)->b_fine_tune);

      //  u4FreqBound = u4Freq;

	 u1AnaMode = MOD_ANA_TERR;
    }
    else
    {
    //    mcVERIFY_ASSERT(0);
        mcSHOW_HW_MSG(("%s: Wrong e_conn_type (%d)!\n", __FUNCTION__, e_conn_type));

        mcSEMA_UNLOCK_HAL(psAtvPdCtx->hHalLock);

        return FALSE;

    }

#if 0 // Marked by Maggie. 2008.9.11 due to undefined symbol on DTV_X_IDTV0801_001017
   if(u1ConnMode == ATV_CONN_MODE_CHAN_CHANGE)
   {
	vTvd3dChannelChange();
   }
#endif

#if !fcFINE_TUNE_SCAN
    if (_fgAutoSearch)
        fgFineTune = TRUE;
#endif
    AnaPd_EnterTunerAcq();

    //mcSHOW_DBG_MSG(("[ATuner] %s (Freq=%u) (Freq_bound=%u)", __FUNCTION__, u4Freq, u4FreqBound));
      mcSHOW_DBG_MSG(("[DriverSearchChannelStart-ATuner] %s (Freq=%u)\n", __FUNCTION__, u4Freq));

#if fcTIMING_MEASURE
    u2TickStart = mcGET_SYS_TICK();
#endif

    DigTunerBypassI2C(TRUE);
    u4Freq = u4Freq / 1000;
    psAtvPdCtx->u4AcqFrequency = u4Freq;


  //  psAtvPdCtx->sAtvPiCtx.u4Freq = u4Freq;



    //u4FreqBound = u4FreqBound / 1000;

   // mcSHOW_DBG_MSG((" (%d, %04X / Bound=%d)", u4Freq, u4Freq, u4FreqBound));
    u4Freq += i4FreqOffset;
    if (i4FreqOffset)
        mcSHOW_DBG_MSG((" (%d, %04X)\n", u4Freq, u4Freq));
     param.Freq=u4Freq;
     param.Modulation=u1AnaMode;
     param.fgAutoSearch=0;

#if !fcDUMMY_ANA_TUNER
    if (AnalogPINA_TunerSetFreq(cATUNER_PI_CTX(psAtvPdCtx), &param) != TRUE) /*jackon: why need use SV_TRUE?*/
    {
        mcSHOW_HW_MSG(("Freq %d out of range!\n", u4Freq * 62500 - cANA_TUNER_IF_FREQ));
        DigTunerBypassI2C(FALSE);
        AnaPd_ExitTunerAcq(FALSE);

        mcSEMA_UNLOCK_HAL(psAtvPdCtx->hHalLock);

        return FALSE;

    }
        AnalogPINA_TunerSetSystem(cATUNER_PI_CTX(psAtvPdCtx), AnalogPINA_TunerGetSystem(cATUNER_PI_CTX(psAtvPdCtx)));
#endif

    DigTunerBypassI2C(FALSE);
#if fcTIMING_MEASURE
    mcSHOW_DBG_MSG4((" (1.%3u ms)\n", (mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD()));
#endif
#if fcTUNE_NOT_START
    if (psAtvPdCtx->u1TunerAcqType == TUNE_FREQ_ONLY)
    {
    	mcSEMA_UNLOCK_HAL(psAtvPdCtx->hHalLock);
        return TRUE;
    }
#endif
	            // meirui 091224,for TW 
			
	if (u1ConnMode != ATV_CONN_MODE_CHAN_CHANGE)     
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

            mcSEMA_UNLOCK_HAL(psAtvPdCtx->hHalLock);

            return FALSE;

        }
    }

    DigTunerBypassI2C(TRUE);

    i4CurFreq = u4Freq;

//++++++++++++++++
   mcSHOW_DBG_MSG(("b_fine_tune = %d\n",bFineTune));
//---------------------

if (bFineTune )

{
#if !fcFINE_TUNE_SCAN
//+++++++++++++++++++++++++++++++
    if (fgFineTune)
#else
    if (_fgAutoSearch)//Channel Scan
#endif
    {
    UINT32 u2FFF = u4Freq;
//    UINT32 u2FFFBD = u4FreqBound;  //fix variable "u2FFFBD" was declared but never referenced


        //u1ATVStatus = AnalogPINA_TunerSearchNearbyFreq(cATUNER_PI_CTX(psAtvPdCtx), u1ScanType, &u2FFF, _BreakFct, pvArg, u2FFFBD);  // eTUNER_AUTO_FINE_TUNE
        u1ATVStatus = AnalogPINA_TunerSearchNearbyFreq(cATUNER_PI_CTX(psAtvPdCtx), &u2FFF, _BreakFct, pvArg);
        psAtvPdCtx->u1VIFLock = u1ATVStatus;
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
    else//Channel change
    {
      	{
#if !fcDUMMY_ANA_TUNER
        i4CurFreq = u4Freq;
        //u1ATVStatus = AnalogPINA_TunerSearchNearbyFreq(cATUNER_PI_CTX(psAtvPdCtx), eTUNER_UNI_AUTO_SCAN, &u4Freq, _BreakFct, pvArg, u4Freq);
        u1ATVStatus = AnalogPINA_TunerSearchNearbyFreq(cATUNER_PI_CTX(psAtvPdCtx), &u4Freq, _BreakFct, pvArg);
        psAtvPdCtx->u1VIFLock = u1ATVStatus;
        if (mcTEST_BIT(u1ATVStatus, cpANA_LOCK_STS_TVD))
            i4CurFreq = u4Freq;
        else{
          param.Freq=i4CurFreq;
          param.Modulation=u1AnaMode;
          param.fgAutoSearch=0;
          AnalogPINA_TunerSetFreq(cATUNER_PI_CTX(psAtvPdCtx), &param);
        }
#endif
      	}

        if(2==fgEnableATP){   //show scaned channel when first time channel change after channel scan
         mcSHOW_DBG_MSG(("##Start\n##Scan : %u\n##End\n",ScanedNo));
          ScanedNo=0;
        }
    }
}

    else
	{
            //mcSHOW_DBG_MSG(("[ATuner] A_N_S\n"));
            mcSHOW_DBG_MSG(("[ATuner] Notify now signal stable\n"));
            _Tvd_NotifyTVSig(u1Path, SV_VDO_STABLE);
        }
	
    DigTunerBypassI2C(FALSE);
#if 0
    mcSHOW_DBG_MSG(("VIFLock: %d (%04X, %d, %d)\n",
                    psAtvPdCtx->u1VIFLock,
                    i4CurFreq, i4CurFreq, AnaPi_TunerGetVIFLock()));
#endif
  mcSHOW_DBG_MSG(("DriverSearchChannelEnd-Status: %d (%dKHz)\
(3-VIF and TVD both lock,2-TVD lock,1-VIF lock)\n",
                  psAtvPdCtx->u1VIFLock,
                  i4CurFreq));
#if fcTIMING_MEASURE
    mcSHOW_DBG_MSG4((" (2.%3u ms)\n", (mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD()));
#endif
    //mcSHOW_DBG_MSG(("[ATuner] %s fine-tune: %04X\n", __FUNCTION__, i4CurFreq));
    psAtvPdCtx->Frequency = i4CurFreq;


if(mcCHECK_DBG_LVL(4))
{
	//UINT8 ii=0;
	//ii = 0xA;
	mcDELAY_MS(100);
	mcSHOW_USER_MSG(("Test Mod = %d\n", (IO_READ8(0x20022080, 0x03)>>4)&0x07));
}



//    fgIsDisconnected = FALSE;
//#if !defined(CC_MTAL) && !defined(CC_SDAL)
#ifdef ANAUS_ENABLE_MONITOR

    //if (!_BreakFct(pvArg) && (u1ConnMode == ATV_CONN_MODE_CHAN_CHANGE) && (((TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info)->b_fine_tune))
    if (!_BreakFct(pvArg) && (u1ConnMode == ATV_CONN_MODE_CHAN_CHANGE) && (bFineTune))
  //  if (!_BreakFct(pvArg) && (u1ConnMode == ATV_CONN_MODE_CHAN_CHANGE))
    {
        //psAtvPdCtx->fgBreakMonitor = FALSE;           // Enable monitor thread and Disable break function flag
        //mcMUTEX_UNLOCK(t_escape_mon_suspend);  // Unlock monitor thread
        ResumeMonitor(psAtvPdCtx);   // Enable monitor thread and Disable break function flag,  Unlock monitor thread
        mcSHOW_DBG_MSG(("Enable monitor process!!\n"));
    }
#endif
    if (psAtvPdCtx->u1VIFLock < 2)
    {
       AnaPd_ExitTunerAcq(FALSE);
        mcSEMA_UNLOCK_HAL(psAtvPdCtx->hHalLock);
        return FALSE;
    }
    AnaPd_ExitTunerAcq(TRUE);

    mcSEMA_UNLOCK_HAL(psAtvPdCtx->hHalLock);

    return TRUE;

}

//-----------------------------------------------------------------------------
/*
 *  AnalogNA_TunerGetSync
 *  Not Used now
 *  @param  ptTDCtx  a void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @param  e_conn_type connection type (Air / Cable)
 *  @retval   none.
 */
//-----------------------------------------------------------------------------
INT16 AnalogNA_TunerGetSync(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                          DRV_CONN_TYPE_T e_conn_type)
{

INT16   i2RetValue = 1;                     // Always in sync state
    return i2RetValue;
}

//-----------------------------------------------------------------------------
/*
 *  AnalogNA_TunerGetSignal
 *  To get current signal characteristic
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @param  e_conn_type Connection type (Air / Cable)
 *  @param  *_pSignal Singal parameters
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
VOID AnalogNA_TunerGetSignal(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                    DRV_CONN_TYPE_T e_conn_type,
                    SIGNAL *_pSignal)
{
ATV_PD_CTX_T *psAtvPdCtx = (ATV_PD_CTX_T *) ptTDSpecificCtx;
// TUNER_POLARIZATION_T  e_pol;         /* Polarization. */
// TUNER_MODULATION_T    e_mod;         /* Modulation. */
// UINT16                ui2_dir;       /* Direction in 0.1 degrees. Valid value range '0..3599' (inclusive). */
// UINT16                ui2_gain;      /* Gain in 0.1 steps. Valid range '0..1000' (inclusive). */
INT32 i4CurFreq;
UINT32  u4Freq;
#if 0
    i4CurFreq = psAtvPdCtx->Frequency;
    u4Freq = i4CurFreq * 1000;
#endif
#if 1
    i4CurFreq = psAtvPdCtx->sAtvPiCtx.u4Freq;//for MW get the current freq
    u4Freq = i4CurFreq * 1000;
#endif
    mcSHOW_DBG_MSG(("[ATuner] %s: %04X (%d)", __FUNCTION__, i4CurFreq, u4Freq));
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
 *  AnalogNA_TunerGetSignalLevel
 *  Not Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @param  e_conn_type Connection type (Air / Cable)
 *  @retval   Signal quality, Range is 0~100.
 */
//-----------------------------------------------------------------------------
UINT8  AnalogNA_TunerGetSignalLevel(PTD_SPECIFIC_CTX ptTDSpecificCtx,
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
 *  AnalogNA_TunerGetSignalLeveldBm
 *  Not Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @param  e_conn_type Connection type (Air / Cable)
 *  @retval   Signal quality (in dB).
 */
//-----------------------------------------------------------------------------
// Signal Level
INT16 AnalogNA_TunerGetSignalLeveldBm(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                    DRV_CONN_TYPE_T e_conn_type)
{
UINT8 u1Value;
    u1Value = bTvdSignalStrengthLevel(1); // bMode = 1, return in dB
    return (u1Value);
}

//-----------------------------------------------------------------------------
/*
 *  AnalogNA_TunerGetSignalPER
 *  Not Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @param  e_conn_type Connection type (Air / Cable)
 *  @retval   None.
 */
//-----------------------------------------------------------------------------
// packet error rate
UINT16  AnalogNA_TunerGetSignalPER(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                    DRV_CONN_TYPE_T e_conn_type)
{
    return (0);
}

//-----------------------------------------------------------------------------
/*
 *  AnalogNA_TunerGetSignalSNR
 *  Not Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @param  e_conn_type Connection type (Air / Cable)
 *  @retval   None.
 */
//-----------------------------------------------------------------------------
UINT16 AnalogNA_TunerGetSignalSNR(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                    DRV_CONN_TYPE_T e_conn_type)
{
    return (0);                             // return 0 first
}

//-----------------------------------------------------------------------------
/*
 *  AnalogNA_TunerSetMpgFmt
 *  Not Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @param  e_conn_type Connection type (Air / Cable)
 *  @retval   None.
 */
//-----------------------------------------------------------------------------
VOID AnalogNA_TunerSetMpgFmt(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                            MPEG_FMT_T *pt_mpeg_fmt)
{
    return;
}

//-----------------------------------------------------------------------------
/*
 *  AnalogNA_TunerGetVer
 *  To get tuner version
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @retval   "Analog_Tuner".
 */
//-----------------------------------------------------------------------------
CHAR *AnalogNA_TunerGetVer(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
ATV_PD_CTX_T *psAtvPdCtx = (ATV_PD_CTX_T *) ptTDSpecificCtx;

    mcSEMA_LOCK_HAL(psAtvPdCtx->hHalLock);  // Avoid re-entry

    mcSHOW_USER_MSG(("[ATuner] %s\n", AnalogPINA_TunerGetVer(cATUNER_PI_CTX(psAtvPdCtx))));

    mcSEMA_UNLOCK_HAL(psAtvPdCtx->hHalLock);
    return "Analog_Tuner";
}

//-----------------------------------------------------------------------------
/*
 *  AnalogNA_TunerNimTest
 *  No Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @retval   none
 */
//-----------------------------------------------------------------------------
INT32 AnalogNA_TunerNimTest(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSHOW_DBG_MSG(("%s\n", __FUNCTION__));

    return 0;
}

//-----------------------------------------------------------------------------
/*
 *  AnalogNA_TunerSetRegSetting
 *  No Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @retval   none
 */
//-----------------------------------------------------------------------------
VOID AnalogNA_TunerSetRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx, UCHAR ucRegSet,
                            UCHAR ucRegAddr, UCHAR ucRegValue)
{
    mcSHOW_DBG_MSG(("%s\n", __FUNCTION__));
}

//-----------------------------------------------------------------------------
/*
 *  AnalogNA_TunerShowRegSetting
 *  No Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @retval   none
 */
//-----------------------------------------------------------------------------
VOID AnalogNA_TunerShowRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSHOW_DBG_MSG(("%s\n", __FUNCTION__));
}

//-----------------------------------------------------------------------------
/*
 *  AnalogNA_TunerTestI2C
 *  No Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @retval   none
 */
//-----------------------------------------------------------------------------
VOID AnalogNA_TunerTestI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSHOW_DBG_MSG(("%s\n", __FUNCTION__));
}

//-----------------------------------------------------------------------------
/*
 *  AnalogNA_TunerRegRead
 *  No Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @retval   none
 */
//-----------------------------------------------------------------------------
INT32 AnalogNA_TunerRegRead(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type,
                    UCHAR ucRegAddr, UCHAR *pucBuffer, U16 ucByteCount)
{
    return 0;
}

//-----------------------------------------------------------------------------
/*
 *  AnalogNA_TunerDisc
 *  Tuner driver disconection
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @param  e_conn_type connection type (Air / Cable)
 *  @retval   none
 */
//-----------------------------------------------------------------------------
VOID  AnalogNA_TunerDisc(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                DRV_CONN_TYPE_T e_conn_type)
{

    #ifdef ANAUS_ENABLE_MONITOR
ATV_PD_CTX_T *psAtvPdCtx = (ATV_PD_CTX_T *) ptTDSpecificCtx;
    #endif

//    mcSEMA_LOCK_HAL(psAtvPdCtx->hHalLock);  // Avoid re-entry
//    fgIsDisconnected = TRUE;
//    psAtvPdCtx->fgBreakMonitor = TRUE;
//    mcSEMA_UNLOCK_HAL(psAtvPdCtx->hHalLock);
    mcSHOW_DBG_MSG(("[Atuner] %s()\n", __FUNCTION__));

    #ifdef ANAUS_ENABLE_MONITOR
    SetBreakMonitor(psAtvPdCtx);
    #endif
}

//-----------------------------------------------------------------------------
/*
 *  AnalogNA_TunerCommand
 *  NA analog Tuner command
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @param  i4Argc Number of argument
 *  @param  aszArgve argument
 *  @retval   none
 */
//-----------------------------------------------------------------------------


VOID AnalogNA_TunerCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                      INT32 i4Argc, const CHAR** aszArgv)
{
#if fcADD_HAL_LOCK
ATV_PD_CTX_T *psAtvPdCtx = (ATV_PD_CTX_T *) ptTDSpecificCtx;
#endif
UCHAR   ucCmdId = 0;

    mcSEMA_LOCK_HAL(psAtvPdCtx->hHalLock);  // Avoid re-entry

    if (i4Argc < 1)
    {
        mcSHOW_USER_MSG(("Cmd: \n"));
        mcSHOW_USER_MSG(("\t o FreqOffset\n"));
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
           case 'o':
                if (i4Argc > 1)
                {
                //    i4FreqOffset = axtoi((CHAR *) aszArgv[1]);
                    i4FreqOffset = StrToInt(aszArgv[1]);
                    if (i4Argc > 2)
                        i4FreqOffset = -i4FreqOffset;
                }
                mcSHOW_USER_MSG(("i4FreqOffset = %d\n", i4FreqOffset));
                break;

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
                    DigTunerBypassI2C(TRUE);
                    AnalogPINA_TunerSetFreq(cATUNER_PI_CTX(psAtvPdCtx), psAtvPdCtx->Frequency, MOD_ANA_CABLE);
                    DigTunerBypassI2C(FALSE);
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
    mcSEMA_UNLOCK_HAL(psAtvPdCtx->hHalLock);
#ifndef CC_MT5381
    AnalogNA_TunerAtdCommand(ptTDSpecificCtx, i4Argc, aszArgv);
#endif
}

//-----------------------------------------------------------------------------
/*
 *  AnalogNA_TunerAtdCommand
 *  Some NA analog ATD Tuner command (ex : set frequency...)
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @param  i4Argc Number of argument
 *  @param  aszArgve argument
 *  @retval   none
 */
//-----------------------------------------------------------------------------

VOID AnalogNA_TunerAtdCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        INT32 i4Argc, const CHAR** aszArgv)
{
ATV_PD_CTX_T *psAtvPdCtx = (ATV_PD_CTX_T *) ptTDSpecificCtx;
UCHAR   ucCmdId = 0;
UINT8   u1Value;
INT32   i4CurFreq = 0, i4Range = 0, i4CtrFreq = 0, i4Step = 0;
UINT32  u4Freq;
//UINT8   u1SubSysIdx;  //fix
UINT8   au1Data[cMAX_READ_NUM];
UINT8   ii, u1ByteCount, u1ByteCountRd;
UINT16  u2RegAddr;
PARAM_SETFREQ_T param;
//#endif

    mcSEMA_LOCK_HAL(psAtvPdCtx->hHalLock);  // Avoid re-entry

    if (i4Argc < 1)
    {
    #ifdef CC_MT5381
        mcSHOW_USER_MSG(("Cmd: \n"));
    #endif
        mcSHOW_USER_MSG(("\t c Show Current Freq (kHz)\n"));    //CR 00012779, for Hisense's request, LC Chien 070514
        mcSHOW_USER_MSG(("\t f [Freq (kHz)]: AnaTunerSetFreq\n"));
        mcSHOW_USER_MSG(("\t s [SubSysIdx]: Color / Sound sub-system\n"));
        mcSHOW_USER_MSG(("\t v [Freq (kHz)]: Check Lock Status\n"));
        mcSHOW_USER_MSG(("\t a: Read SrvAD 3\n"));
        mcSHOW_USER_MSG(("\t h [Freq (kHz)]: TunerSearchNearbyFreq\n"));
        mcSHOW_USER_MSG(("\t t (0x)I2cAddr Value0 Value1...: Write Tuner RF register\n"));
        mcSHOW_USER_MSG(("\t e (0x)I2cAddr: Read Tuner RF register\n"));
        mcSHOW_USER_MSG(("\t r (0x)Addr Count: Read IF register\n"));
        mcSHOW_USER_MSG(("\t w (0x)Addr Value0 Value1 ...: Write IF register\n"));
        mcSHOW_USER_MSG(("\t u [Freq (kHz)] [Range (KHz)] [Step (KHz)]: Measure S-curve from (Freq+Range) to (Freq-Range) in KHz\n"));
        mcSHOW_USER_MSG(("\t k [TopValue(0-31)]: Set analog demod 9886 TOP value (only for uv1336)\n"));
        mcSHOW_USER_MSG(("\t q [TopValue(0-31)]: Writet analog demod 9886 TOP value to E2PROM (only for uv1336)\n"));
        mcSHOW_USER_MSG(("\t n: Read analog demod 9886 TOP value from E2PROM (only for uv1336)\n"));
        mcSHOW_USER_MSG(("\t p: Enable driver ATP auto test log level(0(default),1,2)\n"));
#ifdef ANAUS_ENABLE_MONITOR
        mcSHOW_USER_MSG(("\t mp: Pause monitor thread\n"));
        mcSHOW_USER_MSG(("\t mr: Resume monitor thread\n"));
#endif
    }

    if (i4Argc > 0)
    {
        ucCmdId = *((CHAR *) aszArgv[0]);

        switch (ucCmdId)
        {
             case 'p':
            if (i4Argc > 1)
            {
               fgEnableATP = StrToInt(aszArgv[1]);
            }
            mcSHOW_USER_MSG(("fgEnableATP = %d\n", fgEnableATP));
            break;
            case 'c':
                i4CurFreq = psAtvPdCtx->sAtvPiCtx.u4Freq;
                mcSHOW_USER_MSG(("Current Freq = %d kHz\n", i4CurFreq));

                break;

            case 'f':
                i4CurFreq = psAtvPdCtx->Frequency;
                if (i4Argc > 1)
                {
                    i4CurFreq = StrToInt(aszArgv[1]);
                    psAtvPdCtx->Frequency = i4CurFreq;

                }
                param.Freq=i4CurFreq;
                param.Modulation=MOD_ANA_CABLE;
                param.fgAutoSearch=0;
                mcSHOW_USER_MSG(("Frequency = %d KHz\n", i4CurFreq));
                DigTunerBypassI2C(TRUE);
                AnalogPINA_TunerSetFreq(cATUNER_PI_CTX(psAtvPdCtx), &param);
                DigTunerBypassI2C(FALSE);
                break;

            case 's':

                break;

            case 'v':
                i4CurFreq = psAtvPdCtx->sAtvPiCtx.u4Freq;
                if (i4Argc > 1)
                {
                    i4CurFreq = StrToInt(aszArgv[1]);

                }
                 param.Freq=i4CurFreq;
                param.Modulation=MOD_ANA_CABLE;
                param.fgAutoSearch=0;
                DigTunerBypassI2C(TRUE);
                mcSHOW_USER_MSG(("VIF Lock: %d (Freq = %d)\n",
                                 AnalogPINA_TunerCheckVIFLock(cATUNER_PI_CTX(psAtvPdCtx),&param),
                                 i4CurFreq));

                DigTunerBypassI2C(FALSE);
                mcSHOW_USER_MSG(("TVD Lock: %d\n", fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME)));
                break;

            case 'a':
            // ServoADCNum for Tuner S-curve: ADC IN3
                u1Value = PDWNC_ReadServoADCChannelValue(AFT_AD_NUM);
		          #if defined(CC_MT5360) || defined(CC_MT5387) || defined(CC_MT5363)
               // 6 bits, return bit5-0
                 u1Value = u1Value << 2;
                 if (u1Value >= 0xFC)
    	          u1Value = 0xFF;
              #endif
                mcSHOW_USER_MSG(("bCurveStatus = 0x%02X (%d)\n", u1Value, u1Value));
                break;

            case 'h':
                i4CurFreq = psAtvPdCtx->Frequency;
                if (i4Argc > 1)
                {
                    i4CurFreq = StrToInt(aszArgv[1]);
                    param.Freq=i4CurFreq;
                    param.Modulation=MOD_ANA_CABLE;
                    param.fgAutoSearch=0;
                    DigTunerBypassI2C(TRUE);
                    AnalogPINA_TunerSetFreq(cATUNER_PI_CTX(psAtvPdCtx), &param);
                    DigTunerBypassI2C(FALSE);
                }
                DigTunerBypassI2C(TRUE);
                u4Freq = i4CurFreq;
                AnalogPINA_TunerSearchNearbyFreq(cATUNER_PI_CTX(psAtvPdCtx),  &u4Freq, TunerNullBreak, NULL);
                mcSHOW_USER_MSG(("Search NearBy Before %d After %d KHz\n", i4CurFreq, psAtvPdCtx->sAtvPiCtx.u4Freq));
                psAtvPdCtx->Frequency = psAtvPdCtx->sAtvPiCtx.u4Freq;
                DigTunerBypassI2C(FALSE);
                break;

            case 't':
                DigTunerBypassI2C(TRUE);
                mcWRITE_SET_VALUE_ONE_SEQ(0, mcTUNER_WRITE_FUNC,
                                          cATUNER_PD_CTX(psAtvPdCtx)->u1RF_I2cAddr,
                                          mcWRITE_EXTRA_MSG,
                                          0);
                DigTunerBypassI2C(FALSE);
                break;

            case 'e':
                if (i4Argc > 1)
                {
                    ii = StrToInt(aszArgv[1]);
                    DigTunerBypassI2C(TRUE);
                    if (fgTunerPartRead(ii, &u1Value))
                        mcSHOW_USER_MSG(("TunerPartRead 0x%02X=0x%02X\n", ii, u1Value));
                    DigTunerBypassI2C(FALSE);
                }
                break;



	      case 'k':

                if (i4Argc > 1)
                {
                    psAtvPdCtx->sAtvPiCtx.u1AnaDemoTop = StrToInt(aszArgv[1]);
		  }
                    DigTunerBypassI2C(TRUE);
                     if ( !AnalogPINA_TunerSetSystem(cATUNER_PI_CTX(psAtvPdCtx), AnalogPINA_TunerGetSystem(cATUNER_PI_CTX(psAtvPdCtx))) )
	               {
                         mcSHOW_USER_MSG(("TunerSetDiagCmd: set ATV TOP fail!!\n"));
                      }
		       else
		        {
                         mcSHOW_DBG_MSG(("TunerSetDiagCmd: set ATV TOP = %d\n", psAtvPdCtx->sAtvPiCtx.u1AnaDemoTop));
			 }
                    DigTunerBypassI2C(FALSE);
	 
                break;
#if 0
	      case 'l':
		 {
                  RF_TUNER_CTX_T *pDigiTunerCtx;
	           UINT32  wFreqInKHz;
	           UINT8    u1AnaMode;

                  wFreqInKHz = psAtvPdCtx->sAtvPiCtx.u4Freq;
	           u1AnaMode = psAtvPdCtx->sAtvPiCtx.u1AnaMode;
                  pDigiTunerCtx = psAtvPdCtx->sAtvPiCtx.pDigiTunerCtx;
		    pDigiTunerCtx->u1FmTrap = 1;

                if (i4Argc > 1)
                {
                     pDigiTunerCtx->u1FmTrap = StrToInt(aszArgv[1]);
		  }
                    DigTunerBypassI2C(TRUE);
                     if ( !AnalogPINA_TunerSetFreq(cATUNER_PI_CTX(psAtvPdCtx), wFreqInKHz, u1AnaMode)) 
	              {    
                        mcSHOW_USER_MSG(("TunerSetDiagCmd: set FM trap fail!!\n"));
                     }
			else
		       {
	                 mcSHOW_DBG_MSG(("TunerSetDiagCmd: set FM trap = %d\n", pDigiTunerCtx->u1FmTrap));
			}
                    DigTunerBypassI2C(FALSE);
                
				
	      	 }
                break;

#endif
#ifdef CC_MT5391
           case 'q':
	   {
	   	UINT8  u1Val = 18;
	   	 if (i4Argc > 1)
                {
                   u1Val = StrToInt(aszArgv[1]);
		  }
		 if(EEPNPTV_Write((UINT64)EEP_eTunerAnaDemoTop, (UINT32)(void *)&u1Val, 1))
		 {
		    mcSHOW_USER_MSG(("Write EEPROM fail!!\n"));
		 }
		else
		 {
		   mcSHOW_USER_MSG(("Write EEPROM AnaDemoTop = %d\n", u1Val));
		 }
         }
	    break;

           case 'n':
	    {
		 UINT8  u1Val = 0;
		 if(EEPNPTV_Read((UINT64)EEP_eTunerAnaDemoTop, (UINT32)(void *)&u1Val, 1))
		 {
		    mcSHOW_USER_MSG(("Read EEPROM fail!!\n"));
		 }
		else
		 {
		   mcSHOW_USER_MSG(("Read EEPROM AnaDemoTop = %d\n", u1Val));
		 }
           }
	    break;
#endif

            case 'r':
                if (i4Argc > 1)
                {
                    mcREAD_CHK_INPUT(1);

                    u1Value = 0x86;
                    if (i4Argc > (1 + 2))
                        u1Value = StrToInt(aszArgv[1 + 2]);

                    DigTunerBypassI2C(TRUE);
                #if fcMULTI_REG_RW_DEBUG
                    mcREAD_SHOW_VALUE_MULTI(fgIFPartReadComm, u1Value);
                #else
                    mcREAD_SHOW_VALUE_ONCE(fgIFPartReadComm, u1Value, au1Data);
                #endif
                    DigTunerBypassI2C(FALSE);
                    //mcSHOW_USER_MSG(("Freq Offset: %02X\n", au1Data[0] & 0x1E));
                }
                break;

            case 'w':
                DigTunerBypassI2C(TRUE);
                mcWRITE_SET_VALUE_ONE_SEQ(0, mcTUNER_IF_WRITE_FUNC,
                                        cATUNER_PD_CTX(psAtvPdCtx)->u1IF_I2cAddr,
                                        mcWRITE_EXTRA_MSG,
                                        0);
                DigTunerBypassI2C(FALSE);
                break;

            case 'u':
                if (i4Argc > 1)
                {
                    i4CtrFreq = StrToInt(aszArgv[1]);
                    i4Range   = StrToInt(aszArgv[2]);
                    i4Step   = StrToInt(aszArgv[3]);
                }
                else
                {
                	// Default
                    i4CtrFreq = 181250;  // Center Freq = 181.25MHz
                    i4Range   = 3000;    // Range = +/- 3MHz
                    i4Step    = 62;      // Default step size = 62KHz (Most popular step size = 62.5)
                }
                param.Modulation=MOD_ANA_CABLE;
                param.fgAutoSearch=0;
                mcSHOW_USER_MSG(("[Format]Freq in KHz, S-curve in decimal, S-curve in hex\n"));
                DigTunerBypassI2C(TRUE);
                for (i4CurFreq = (i4CtrFreq + i4Range); i4CurFreq >= (i4CtrFreq - i4Range); i4CurFreq = (i4CurFreq - i4Step))
                {
                    // Set Tuner Freq
                    param.Freq=i4CurFreq;
                    AnalogPINA_TunerSetFreq(cATUNER_PI_CTX(psAtvPdCtx),&param);
                    mcDELAY_MS(500);
                    // Read Servo AD for S-curve
                    u1Value = PDWNC_ReadServoADCChannelValue(AFT_AD_NUM);
		        #if defined(CC_MT5360) || defined(CC_MT5363) 
                    // 6 bits, return bit5-0
                     u1Value = u1Value << 2;
                     if (u1Value >= 0xFC)
    	              u1Value = 0xFF;
                   #endif
                    mcSHOW_USER_MSG(("%d\t%d\t%02X\n", i4CurFreq, u1Value, u1Value));
                }
                DigTunerBypassI2C(FALSE);
                break;

#ifdef ANAUS_ENABLE_MONITOR
            case 'm':
                {
                UINT8   ucCmdExt;

                    ucCmdExt = ((CHAR *) aszArgv[0])[1];

                    if (ucCmdExt == 'p'){
                        //fgIsDisconnected = TRUE;
                        //psAtvPdCtx->fgBreakMonitor = TRUE;
                        SetBreakMonitor(psAtvPdCtx);
                    }
                    else if (ucCmdExt == 'r'){
//                        fgIsDisconnected = FALSE;
                        //psAtvPdCtx->fgBreakMonitor = FALSE;
                        //mcMUTEX_UNLOCK(t_escape_mon_suspend);
                        ResumeMonitor(psAtvPdCtx);
                    }
                    else{
                        mcSHOW_USER_MSG(("do nothing\n"));
                    }
                }
                break;
#endif
            default:
                mcSHOW_USER_MSG(("Invalid parameter!\n"));
                break;
        }
    }
    mcSEMA_UNLOCK_HAL(psAtvPdCtx->hHalLock);
}

//-----------------------------------------------------------------------------
/*
 *  AnalogNA_TunerGetAttribute
 *  Not Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @retval   none
 */
//-----------------------------------------------------------------------------
INT16 AnalogNA_TunerGetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                           DRV_GET_TYPE_T  e_get_type,
                           VOID *pvAnaAttribute,
                           SIZE_T *pzAnaAttributeLen)
{
#if fcADD_HAL_LOCK
ATV_PD_CTX_T *psAtvPdCtx = (ATV_PD_CTX_T *) ptTDSpecificCtx;
#endif


    mcSEMA_LOCK_HAL(psAtvPdCtx->hHalLock);  // Avoid re-entry

    switch (e_get_type)
    {

        default:
        {
            //mcSHOW_DRVERR_MSG(("%s: RMR_DRV_INV_GET_INFO!\n", __FUNCTION__));

            mcSEMA_UNLOCK_HAL(psAtvPdCtx->hHalLock);
            return (RMR_DRV_INV_GET_INFO);
        //    break;
        }
    }
}

//-----------------------------------------------------------------------------
/*
 *  AnalogNA_TunerSetAttribute
 *  Not Used now
 *  @param  ptTDCtx  A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
 *  @retval   none
 */
//-----------------------------------------------------------------------------
INT32 AnalogNA_TunerSetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                           DRV_SET_TYPE_T  e_set_type,
                           const VOID *pvAnaAttribute,
                           SIZE_T zAnaAttributeLen)
{
#if fcTUNE_NOT_START
ATV_PD_CTX_T *psAtvPdCtx = (ATV_PD_CTX_T *) ptTDSpecificCtx;
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
                psAtvPdCtx->u1TunerAcqType = *peActionType;
            }
            break;
#endif

        default:
        {
            mcSHOW_DRVERR_MSG(("%s: RMR_DRV_INV_GET_INFO!\n", __FUNCTION__));
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
*  AnalogNA_TunerSetDiagCmd
*  Set analog tuner diagnostic information.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @param  e_tuner_set_type : Diagnostic information type.
*  @param  *pv_set_info : Input data content pointer.
*  @param  z_size : Input data length.
*  @retval  RMR_OK : Set analog tuner diagnostic information successfully.
*  @retval  RMR_DRV_SET_FAILED : Set analog tuner diagnostic information fail.
*  @retval  RMR_DRV_INV_SET_INFO : Invalid analog tuner diagnostic information.
************************************************************************/
INT32 AnalogNA_TunerSetDiagCmd(PTD_SPECIFIC_CTX ptTDSpecificCtx,
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
            param.Freq=wFreqInKHz;
            param.Modulation=u1AnaMode;
            param.fgAutoSearch=0;
            if ( !AnalogPINA_TunerSetFreq(cATUNER_PI_CTX(psAtvPdCtx),&param))
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

INT32 AnalogNA_TunerGetDiagInfo(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                              DRV_CUSTOM_TUNER_GET_TYPE_T   e_tuner_get_type,
                              VOID                           *pv_get_info,
                              SIZE_T                        *pz_size)
{
ATV_PD_CTX_T *psAtvPdCtx = (ATV_PD_CTX_T *) ptTDSpecificCtx;
TUNER_TYPE_DIAG_CONTENT_T *pDiagContent = (TUNER_TYPE_DIAG_CONTENT_T *) pv_get_info;

    mcSEMA_LOCK_HAL(psAtvPdCtx->hHalLock);  // Avoid re-entry

    switch (e_tuner_get_type)
    {
        case DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTROL:
            if ((*pz_size) < sizeof(TUNER_TYPE_DIAG_CONTROL_T))
            {
                *pz_size = sizeof(TUNER_TYPE_DIAG_CONTROL_T);
                mcSEMA_UNLOCK_HAL(psAtvPdCtx->hHalLock);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            ((TUNER_TYPE_DIAG_CONTROL_T *) pv_get_info)->u2PollPeriodMs = 2000;
            ((TUNER_TYPE_DIAG_CONTROL_T *) pv_get_info)->ucCntOfDiagStruc = DRV_CUSTOM_TUNER_DIAG_ITEMS;
            break;

        case DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTENT:
            if ((*pz_size) < DRV_CUSTOM_TUNER_DIAG_ITEMS * sizeof(TUNER_TYPE_DIAG_CONTENT_T))
            {
                *pz_size = DRV_CUSTOM_TUNER_DIAG_ITEMS * sizeof(TUNER_TYPE_DIAG_CONTENT_T);
                mcSEMA_UNLOCK_HAL(psAtvPdCtx->hHalLock);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }

        // To add new items, please remember to modify DRV_CUSTOM_TUNER_DIAG_ITEMS
        // Note that both cDisplayName and cDisplayValue can NOT exceed 15 bytes

        // Record 1
            sprintf(pDiagContent->cDisplayName, "%s", "Version:");
            sprintf(pDiagContent->cDisplayValue, "%s", AnalogPINA_TunerGetVer(cATUNER_PI_CTX(psAtvPdCtx)));
            pDiagContent++;

        // Record 2
            sprintf(pDiagContent->cDisplayName, "%s", "RF Freq (KHz):");
            sprintf(pDiagContent->cDisplayValue, "%6d", psAtvPdCtx->Frequency);
            pDiagContent++;

        // Record 3
            sprintf(pDiagContent->cDisplayName, "%s", "Lock State:");
            sprintf(pDiagContent->cDisplayValue, "%d", psAtvPdCtx->u1VIFLock);
            pDiagContent++;

            break;                          // End of case DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTENT

        default:
            mcSEMA_UNLOCK_HAL(psAtvPdCtx->hHalLock);
            return RMR_DRV_NO_GET_INFO;
    }
    mcSEMA_UNLOCK_HAL(psAtvPdCtx->hHalLock);
    return RMR_OK;
}
#endif

//Pluto : I will move to ATD driver once ATD function enable
/*Jackson: need refine this api, move to correct location*/
BOOL MT5381ATD_TunerSwitchRfAgc(VOID)
{
//Reset (34H & 35H)
    //mcSHOW_USER_MSG(("+++Pluto+++ Clear 0x34\n"));
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
VOID Tuner_AnalogNA_Register_LTDIS_Fct(PTD_CTX ptTDCtx)
{
  TUNER_DEVICE_CTX_T*     pTunerDeviceCtx = ptTDCtx;

    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerClose = AnalogNA_TunerClose;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerAcq = AnalogNA_TunerAcq;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGerSync = AnalogNA_TunerGetSync;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignal = AnalogNA_TunerGetSignal;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLevel = AnalogNA_TunerGetSignalLevel;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLeveldBm = AnalogNA_TunerGetSignalLeveldBm;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalPER = AnalogNA_TunerGetSignalPER;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalSNR = AnalogNA_TunerGetSignalSNR;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetMpgFmt = AnalogNA_TunerSetMpgFmt;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetVer = AnalogNA_TunerGetVer;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerNimTest = AnalogNA_TunerNimTest;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerCommand = AnalogNA_TunerCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerAtdCommand = AnalogNA_TunerAtdCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetRegSetting = AnalogNA_TunerSetRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerShowRegSetting = AnalogNA_TunerShowRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerTestI2C = AnalogNA_TunerTestI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerRegRead = AnalogNA_TunerRegRead;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDisc = AnalogNA_TunerDisc;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetAttribute = AnalogNA_TunerSetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute = AnalogNA_TunerGetAttribute;
#ifdef fcADD_DIAG_INFO
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetDiagInfo = AnalogNA_TunerGetDiagInfo;
//pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetDiagCmd  = AnalogNA_TunerSetDiagCmd;
#endif
}
