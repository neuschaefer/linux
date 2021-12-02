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
 * $RCSfile: io_mtfe_ntsc.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file io_mtfe_ntsc.c
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mtfe_ntsc.h"
#include "io_mtfe_pd_def.h"

#include "tuner_if.h"
// Guocheng add, for the TunerATD module, 2010/02/24
#if defined (USE_ATD_IF_DEMOD) 
#include "pi_atd_if.h"         
#include "pd_atd_if.h"         
#include "pi_demod_atd.h"//d110805_Haibo:Fix implict build warning   
#include "pi_demod_atdna.h"  //d110805_Haibo:Fix implict build warning   
#else
#include "anana_pd_glue_if.h"    // for TunerAnalogNA module
#include "pi_anana_if.h"
#endif

#include "io_mtfe_ntsc.h"
#include "cb_low.h"  // For callback function

#include <linux/mtal_ioctl.h>

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(int)))                            \
    {                                                       \
        printk("mtal_ioctl_ntsc argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(int)))                        \
    {                                                       \
        printk("mtal_ioctl_ntsc argument error\n");      \
        return -1;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_ntsc argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_ntsc argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_ntsc argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_ntsc argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        printk("mtal_ioctl_ntsc argument error\n");     \
        return MTR_ERR_INV;                                          \
    }

#define COPY_TO_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_ntsc argument error\n");      \
        return MTR_ERR_INV;                                          \
    }
#define COPY_FROM_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_from_user(&ArgKernel,(void __user *)ArgUsr,    \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_ntsc argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define MTAL_NTSC_MON_POLL_INTRVL             50  // 50ms
#define MTAL_NTSC_MON_AFT_INTRVL    500  // 500ms
#define MTAL_NTSC_MON_ATF_CNT       (MTAL_NTSC_MON_AFT_INTRVL/MTAL_NTSC_MON_POLL_INTRVL)
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static MTAL_NTSC_PD_CTX_T  *pMTPdCtx;
static BOOL _fgNTSCInit=FALSE;
/*all variable used in this file should put into pMTPdCtx exclusive of Exclusive_DigiAnaSema and u1ExclSemaInit*/
static HANDLE_T    Exclusive_DigiAnaSema=0;
static UINT8       u1ExclSemaInit=0;
//-----------------------------------------------------------------------------
// extern function
//-----------------------------------------------------------------------------
#ifdef CC_FESBTVD//SBTVD
extern MT_RESULT_T _MTFE_SBTVD_DisConnect(VOID);
extern void Get_MTFE_SBTVD_Sema_Ex_DiAna(HANDLE_T *sema);
#define MTAL_DISCONNECT_DTV   _MTFE_SBTVD_DisConnect
#define MTAL_GET_DTD_SEMA_EX_DIANA  Get_MTFE_SBTVD_Sema_Ex_DiAna
#elif defined(CC_FEISDB)   // Japan ARIB ISDB-T
extern MT_RESULT_T _MTFE_ISDBT_DisConnect(VOID);
extern void Get_MTFE_ISDBT_Sema_Ex_DiAna(HANDLE_T *sema);
#define MTAL_DISCONNECT_DTV   _MTFE_ISDBT_DisConnect
#define MTAL_GET_DTD_SEMA_EX_DIANA  Get_MTFE_ISDBT_Sema_Ex_DiAna
#elif defined(CC_FEDTD) // DTD
extern MT_RESULT_T _MTFE_ATSC_DisConnect(VOID);
extern void Get_MTFE_ATSC_Sema_Ex_DiAna(HANDLE_T *sema);
#define MTAL_DISCONNECT_DTV   _MTFE_ATSC_DisConnect
#define MTAL_GET_DTD_SEMA_EX_DIANA  Get_MTFE_ATSC_Sema_Ex_DiAna
#else
#define MTAL_DISCONNECT_DTV()
#define MTAL_GET_DTD_SEMA_EX_DIANA(x)
#endif

extern BOOL _fgAutoSearch;
#if defined(CC_MT5365) || defined(CC_MT5363) || defined(CC_MT5396)
extern void TunerSetMwIntf(UINT8 u1Value);
#endif
extern void * TunerGetCtx(UINT16 TunerID);
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
/** Front-end Callback structure ID ENUM
*/
typedef	enum
{
	NTSC_SEARCH_STATE_NONE = 0,
    NTSC_SEARCH_STATE_START,
    NTSC_SEARCH_STATE_ON_GOING,
    NTSC_SEARCH_STATE_DONE,    
} MTAL_FE_NTSC_STATE_ENUM_T;

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
//static ATV_PD_CTX_T                *pAtvPdCtx;
//static ATV_PI_CTX_T                *pAtvPiCtx;
//static HANDLE_T						t_atv_lock;
//static BOOL             			_fgMonitorThreadRunning;
//static UINT32         				t_monitor_thread;
//static MTAL_FE_NTSC_STATE_ENUM_T   _rSearchNearbyState;
//static UINT32                      _u4DesireVFreqInKHz;
//static UINT32                      _u4ResultVFreqInKHz;
//static BOOL                        _fgBreak = FALSE;
//static void _CB_PutEventLockStatus(MTFE_DTV_STATUS_T LockStatus)

//-----------------------------------------------------------------------------
// Static function
//-----------------------------------------------------------------------------

// Guocheng added for functions to compatible with the TunerATD module, 2010/02/24
#if defined (USE_ATD_IF_DEMOD)  
//-----------------------------------------------------------------------------
/** fgDrvTunerCheckTVDLock
 *  check whether tv decoder can  decode  IF signal or not 
 *  @param  bTryLoopCnt  Re-check TVD times if TVD unlock
 *  @retval TRUE         TVD lock signal(CVBS)
 *  @retval FALSE        TVD can't lock signal(CVBS)
 */
 //----------------------------------------------------------------------------
static BOOL fgDrvTunerCheckTVDLock( UINT16 bTryLoopCnt )
{
    BOOL fgReturn;
    fgReturn = fgDrvTvdCheckTVDLock(bTryLoopCnt);
    MTFE_NTSC_PRINT("fgDrvTvdCheckTVDLock =%d\n",fgReturn);

    return fgReturn;
}

//----------------------------------------------------------------------------
/** AnalogPINA_TunerSetFreq
 *  Set Tuner LO tune to specified frequency
 *  @param  psAtdPiCtx     pointer of tuner device context, struct ATD_CTX_T.
 *  @param  wFreqInKHz   RF frequency in KHz.
 *  @param  u1AnaMode   Modulation Type
 *  @retval  0            Set tuner successfully
 *  @retval  1            Set tuner set fail  
 */ 
 //----------------------------------------------------------------------------
static BOOL  AnalogPINA_TunerSetFreq(ATD_CTX_T *psAtdPiCtx, UINT32 wFreqInKHz, UINT8 u1AnaMode)
{
    BOOL fgReturn;
	
    ATD_ChipInit(psAtdPiCtx);
    ATD_SetSystem(psAtdPiCtx, psAtdPiCtx->u1SubSysIdx);

    fgReturn = ATD_SetFreq(psAtdPiCtx, wFreqInKHz, u1AnaMode);

    if (u1AnaMode == ATV_CONN_MODE_CHAN_SCAN)
    {
        psAtdPiCtx->u1PF = 0x01;  //enable pilot filter when initial scan 
        ATD_StartAcq(psAtdPiCtx, TRUE);
    }
    else if(u1AnaMode == ATV_CONN_MODE_CHAN_CHANGE)
    {
        psAtdPiCtx->u1PF = 0x01;  //enable pilot filter when channel change
        ATD_StartAcq(psAtdPiCtx, FALSE);
    }

    return fgReturn;
}

//----------------------------------------------------------------------------- 
/**AnalogPINA_TunerCheckVIFLock
 *  Check if programmed frequency has strong VIF signal nearby
 *  @param  psAtdPiCtx     pointer of tuner device context, struct ATD_CTX_T.
 *  @param  wFreqInKHz          The frequenccy in KHz need to check the VIF status
 *  @retval   	 2  => Current frequency is very close to the nominal VIF.
           		 1  => Current frequency is is near the nominal VIF.
           		 0  => Current frequency is not close to the nominal VIF yet.
 */
//-----------------------------------------------------------------------------
static UINT8 AnalogPINA_TunerCheckVIFLock(ATD_CTX_T *psAtdPiCtx, UINT32 wFreqInKHz)
{
    UINT8 u1VIFLock;

    u1VIFLock = ATD_GetVIFLock(psAtdPiCtx, TRUE);

    return u1VIFLock;
}

//----------------------------------------------------------------------------- 
/** AnalogPINA_TunerSearchNearbyFreq
 *  Find the freq. value that near center frequency of the channel.
 *  @param  psAtdPiCtx     pointer of tuner device context, struct ATD_CTX_T.
 *  @param  *wFreqInKHz  	THe point of frequenccy in KHz need to check the VIF status
 *  @param  _BreakFct  	 	The break function
 *  @param  *pvArg  		The argument of break function        
 *  @retval  bit0           1:VIF lock, 0: VIF unlock
 *  @retval  bit1           1:TVD lock, 0: TVD unlock
 *  @retval  bit2           1:break happen, 0: no break
 */
//-----------------------------------------------------------------------------
static UINT8 AnalogPINA_TunerSearchNearbyFreq(ATD_CTX_T *psAtdPiCtx,  UINT32 *wFreqInKHz, x_break_fct _BreakFct, VOID *pvArg)
{
    UINT8 u1Return;
    UINT32 u4Freq;

    u4Freq = *wFreqInKHz;

    ATD_ChipInit(psAtdPiCtx);
    ATD_SetSystem(psAtdPiCtx, psAtdPiCtx->u1SubSysIdx);	
    ATD_SetFreq(psAtdPiCtx, *wFreqInKHz, ATV_CONN_MODE_CHAN_SCAN);		 
    psAtdPiCtx->u1PF = 0x01;	//enable pilot filter when initial scan	
    ATD_StartAcq(psAtdPiCtx, TRUE);		  
    psAtdPiCtx->fgCheckCR = TRUE;
	   
    u1Return =ATDNA_SearchNearbyFreq(psAtdPiCtx, wFreqInKHz,  _BreakFct, pvArg, u4Freq);
	
    return u1Return ;
}
//------------------------------------------------------------------------------
/**  AnalogPINA_TunerMonitor
 *  Find the freq. value that near center frequency of the channel.
 *  @param  psAtdPiCtx     pointer of tuner device context, struct ATD_CTX_T.
 *  @param  *wFreqInKHz  	THe point of frequenccy in KHz need to check the VIF status
 *  @param  _BreakFct  	 	The break function
 *  @param  *pvArg  		The argument of break function
 *  @retval   TRUE  =>  center freq. found
            	 FALSE =>  center freq. not found (no signal on this channel)
 */
//------------------------------------------------------------------------------
static UINT8 AnalogPINA_TunerMonitor(ATD_CTX_T *psAtdPiCtx, UINT32 *wFreqInKHz, x_break_fct _BreakFct, VOID *pvArg)
{
    UINT8 u1Return;

    u1Return = ATDNA_Monitor(psAtdPiCtx, wFreqInKHz,  _BreakFct, pvArg);

    return u1Return;
}

#endif
//------------------------------------------------------------------------------

static void _CB_PutEventLockStatus(UINT8 u1SearchResult, UINT32 u4Freq)
{
    MTAL_FE_US_TUNER_CALLBACK_INFO_T 	rCbInfo;

    rCbInfo.CbId = MTAL_FE_NTSC_LOCK_STATUS;

    //BIT0: MTFE_ATV_VIF_LOCK_STATUS_BIT
    //BIT1: MTFE_ATV_TVD_LOCK_STATUS_BIT
    //BIT2: MTFE_ATV_BREAK_SEARCH_NEARBY_BIT
    rCbInfo.CbIdInfo.LockStatus = u1SearchResult;
    MTFE_NTSC_PRINT("NTSC _CB_PutEventLockStatus: LockStatus? %d\n", rCbInfo.CbIdInfo.LockStatus);
    rCbInfo.CbParam.atvParams.u4Freq = u4Freq;
    _CB_PutEvent(CB_MTAL_FE_NTSC_TUNER_TRIGGER, sizeof(MTAL_FE_US_TUNER_CALLBACK_INFO_T), &rCbInfo);
}
//static BOOL _i4AnaMoniBreak(VOID)                                                            
static INT32 _i4AnaMoniBreakSearchNearBy(VOID * pvArg) //d110805_Haibo:Fix  build warning                                                           
{                                                                                                       
    MTFE_NTSC_PRINT("@NTSC pi check wanna break or not=%d\r\n",pMTPdCtx->fgBreakSearchNearBy);                                                                                     
    return (INT32)(pMTPdCtx->fgBreakSearchNearBy);
} 

static INT32 _i4AnaMoniBreakAFT(VOID * pvArg)//d110805_Haibo:Fix  build warning                                                                                                                
{                                                                                                       
    return (INT32)(pMTPdCtx->fgBreakAFT);
} 

/**NTSC monitor process.
*@param  pvArg               
*@retval VOID
*/
static VOID _MonitorProcess(VOID * pvArg)//d110805_Haibo:Fix build warning
{
    UINT16                  u2MsgIdx;
	TUNER_CMD_QUEUE_T	    rCmdQ;
    SIZE_T 					zMsgSize = sizeof(UINT32);
    INT32 					i4Result;
    UINT8                   u1SearchResult=0;
    UINT16  u2_i = 0;

    while (pMTPdCtx->fgMonitorThreadRunning)
    {  
        i4Result = x_msg_q_receive_timeout(&u2MsgIdx, &rCmdQ, &zMsgSize, &pMTPdCtx->t_MoniCmdMsgQ, 1, MTAL_NTSC_MON_POLL_INTRVL);
        MTFE_NTSC_PRINT("NTSC Monitor Get MsgQ=%d\n",i4Result);
        if( (i4Result==OSR_OK) && (rCmdQ.CmdId == TUNER_CMD_STARTSEARCH)){
            pMTPdCtx->fgDoSearchNearby = TRUE;//keep do searchNearBy till finish callback event
            pMTPdCtx->fgBreakSearchNearBy = FALSE;//init a new searchNearBy
        }
        else{
            //do nothing here,most case are doing AFT
        }

        if (pMTPdCtx->fgBreakMonitor) //condition for suspend monitor
        {
            MTFE_NTSC_PRINT("Entering io_mtfe_ntsc mon_suspend\n");
            mcSEMA_LOCK_MTFE_S(pMTPdCtx->t_escape_mon_suspend,"ntsc_mon_suspend");
    		MTFE_NTSC_PRINT("Escape io_mtfe_ntsc mon_suspend\n");
        }
        else if(pMTPdCtx->fgDoSearchNearby)
        {
            UINT32 u4Freq;
            UINT32 u4ResultFreq;
//            UINT32 u4FreqBound;

            u4Freq = rCmdQ.CmdInfo.ConnectParam.u4DesireVFreqInKHz;
//            u4FreqBound = rCmdQ.CmdInfo.ConnectParam.u4StopVFreqInKHz;
            mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
            //Maggie todo, check whether parameter mapping to API AnalogPINA_TunerSearchNearbyFreq is correct?
            // change parameter 0 to a enum definition
            u1SearchResult = AnalogPINA_TunerSearchNearbyFreq(pMTPdCtx->pDemodCtx, &u4Freq, _i4AnaMoniBreakSearchNearBy, NULL);
#if defined (USE_ATD_IF_DEMOD) 
            u4ResultFreq = pMTPdCtx->pDemodCtx->u4Frequency;
#else
            u4ResultFreq = pMTPdCtx->pDemodCtx->u4Freq;
#endif
            _CB_PutEventLockStatus(u1SearchResult, u4ResultFreq);

            pMTPdCtx->fgDoSearchNearby = FALSE;
            pMTPdCtx->fgBreakSearchNearBy = FALSE; //when finish TunerSearchNearbyFreq, reset to FALSE
            pMTPdCtx->fgBreakAFT =FALSE; //let AFT can work in next run, 
            mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
        }
        else // AFT
		{ 
            if( (pMTPdCtx->fgEnAFT) && (pMTPdCtx->u4CurFreq!=0))
            { 
                u2_i++;//use to control delay period for AFT
                MTFE_NTSC_PRINT("MTAL NTSC Monitor! keep update parameter u2_i=%d\n",u2_i);			
                if (u2_i > MTAL_NTSC_MON_ATF_CNT) 
                {
                    u2_i = 0;
                    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when call PI primitive API must use this lock for protection
                    //Justin todo: need to consider how many ms need to delay for AFT, please adjust MTAL_NTSC_MON_ATF_CNT
                    AnalogPINA_TunerMonitor(pMTPdCtx->pDemodCtx, &pMTPdCtx->u4CurFreq, _i4AnaMoniBreakAFT, NULL);
                    MTFE_NTSC_PRINT("MTAL NTSC doing AFT!...\n");		
                    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
                }
                //Justin todo: need to consider how many ms need to delay for AFT
            }
            else{
                //do nothing
            }
        }
        //don't do delay here, x_msg_q_receive() will do blocking
    }
    
    pMTPdCtx->t_monitor_thread = (HANDLE_T)NULL;
    MTFE_NTSC_PRINT("MTAL MonitorProcess exit!\n");
    x_thread_exit();

}

/**Start NTSC monitor engine.
*@param  VOID               
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _StartMonitorEngine(VOID)
{  

    pMTPdCtx->fgMonitorThreadRunning = TRUE;
    if (x_thread_create(&pMTPdCtx->t_monitor_thread,
            "_NtscMonThread",
            2048, 101,
            _MonitorProcess, sizeof(void*), NULL) != OSR_OK)
    {
        MTFE_NTSC_PRINT("_StartMonitorEngine NTSC (mcTHREAD_CREATE): ERROR!\n");
        return (MTR_NOT_OK);
    }

    return (MTR_OK);
}

/**Stop NTSC monitor engine.
*@param  VOID               
*@retval MTR_OK              Success.
*@retval MTR_NOT_OK          Fail.
*/
static MT_RESULT_T _StopMonitorEngine(VOID)
{
    pMTPdCtx->fgMonitorThreadRunning = FALSE;
    MTFE_NTSC_PRINT("In kernal:NTSC _MTAL_StopMonitorEngine\n");

    while (pMTPdCtx->t_monitor_thread){
        mcDELAY_MS(10); // in ms
    }
    MTFE_NTSC_PRINT("MTAL NTSC PD_StopMonitorEngine success\n");

    return (MTR_OK);
}



//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/** Initialize frond end ntsc module.
*@param VOID
*@retval MT_OK                  Success.
*@retval MT_NOT_OK              Fail.
*/

static MT_RESULT_T _MTFE_NTSC_Open(VOID)
{
    INT32 i4Status;
    MT_RESULT_T rRet;
#if defined (USE_ATD_IF_DEMOD) 
    ATD_PD_CTX_T      *pAtvPdCtx = NULL;
#else
    ATV_PD_CTX_T     *pAtvPdCtx = NULL;
#endif
    TUNER_CMD_QUEUE_T	rCmdQ;
  	
    //add driver implementation code here
    MTFE_NTSC_PRINT("In kernel %s\n", __FUNCTION__);
    if(_fgNTSCInit==TRUE){
        MTFE_NTSC_PRINT("_fgNTSCInit==TRUE, do _MTFE_NTSC_Close() first\r\n");
        return (MTR_NOT_OK);
    }

   /* TunerOpen */
#if defined(CC_MT5365) || defined(CC_MT5363)  ||  defined(CC_MT5396)
   TunerSetMwIntf(cMTAL_IF);//Set MW type to avoid glue layer to create monitor thread again
 #endif  
    i4Status = TunerOpen();
    if (i4Status < DRVAPI_TUNER_OK)
    {   
        MTFE_NTSC_PRINT("TunerOpen failed (%d)\n", i4Status);
        return (MTR_NOT_OK);
    }
    else if (i4Status > DRVAPI_TUNER_OK)
    {
        MTFE_NTSC_PRINT("TunerOpen has been called (%d)\n", i4Status);
    }
    else{
        MTFE_NTSC_PRINT("TunerOpen ok\n");        
    }

    pMTPdCtx = (MTAL_NTSC_PD_CTX_T *) x_mem_calloc(1, sizeof(MTAL_NTSC_PD_CTX_T));
    if (pMTPdCtx == NULL)
    {
        MTFE_NTSC_PRINT("MTAL_NTSC_PD_CTX_T malloc fail\n");
        return (MTR_NOT_OK);
    }
	
    //get Analog pd Ctx from nucleus pd glue
#if defined (USE_ATD_IF_DEMOD) 
    pAtvPdCtx = (ATD_PD_CTX_T*)TunerGetCtx(ANALOG_FE);
    if(pAtvPdCtx != NULL)
    {
        pMTPdCtx->pDemodCtx = (ATD_CTX_T*)(pAtvPdCtx->sAtdPiCtx);
        pMTPdCtx->pDemodCtx->u1SubSysIdx = MOD_NTSC_M;
        pMTPdCtx->hHalLock = pAtvPdCtx->hHalLock; 
    }
#else
    pAtvPdCtx = (ATV_PD_CTX_T*)TunerGetCtx(ANALOG_FE);
    if(pAtvPdCtx != NULL)
    {
        pMTPdCtx->pDemodCtx = &(pAtvPdCtx->sAtvPiCtx);
        pMTPdCtx->hHalLock = pAtvPdCtx->hHalLock; 
    }
#endif

    MTAL_GET_DTD_SEMA_EX_DIANA(&Exclusive_DigiAnaSema);
    if(Exclusive_DigiAnaSema!=0){
        u1ExclSemaInit = 1;
    }

    pMTPdCtx->fgBreakMonitor = TRUE; //initial monitor is not running
    pMTPdCtx->fgBreakSearchNearBy = FALSE;
    pMTPdCtx->fgEnAFT = FALSE;
    pMTPdCtx->u4CurFreq = 0;//for AFT reference, default 0 let _monitorProcess can't do AFT
    pMTPdCtx->fgBreakAFT = TRUE; //default don't run AFT

    if (x_sema_create(&pMTPdCtx->t_escape_mon_suspend, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        MTFE_NTSC_PRINT(("NTSC x_sema_create(pMTPdCtx->t_escape_mon_suspend) Fail!\n"));
        return (MTR_NOT_OK);
    }
    x_sema_unlock(pMTPdCtx->t_escape_mon_suspend);

    if ((x_msg_q_create(&pMTPdCtx->t_MoniCmdMsgQ, "NTSC_Moni_MsgQ", sizeof(rCmdQ), TUNER_CMD_QUEUE_SIZE)) != OSR_OK)
    {
        MTFE_NTSC_PRINT((" NTSC x_msg_q_create(pMTPdCtx->t_escape_mon_suspend) Fail!\n"));
        return (MTR_NOT_OK);
    }
//#if 0
    pMTPdCtx->fgBreakMonitor = TRUE; //initial monitor is suspend

    /* If tuner open success, start monitor engine */
    rRet = _StartMonitorEngine();
    if(rRet != MTR_OK)
    {
        MTFE_NTSC_PRINT("NTSC _StartMonitorEngine failed\n");
    	return (MTR_NOT_OK);
    }
    _fgNTSCInit = TRUE;
    return MTR_OK;
}
/** Terminate frond end ntsc module.
*@param VOID
*@retval MT_OK                  Success.
*@retval MT_NOT_OK              Fail.
*/

static MT_RESULT_T _MTFE_NTSC_Close(VOID)
{
MT_RESULT_T rRet;

    MTFE_NTSC_PRINT("In kernel %s\n", __FUNCTION__);
    
    //add driver implementation code here
    if(_fgNTSCInit!=TRUE){
        MTFE_NTSC_PRINT("_MTFE_NTSC_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }
 	if(pMTPdCtx->fgBreakMonitor == TRUE){
    	pMTPdCtx->fgBreakMonitor = FALSE; //resume monitor 
		mcSEMA_UNLOCK_MTFE_S(pMTPdCtx->t_escape_mon_suspend,"ntsc_mon_suspend");
	}
    /* If tuner close success, stop monitor engine */
    rRet = _StopMonitorEngine();
    if(rRet != MTR_OK)
    {
        MTFE_NTSC_PRINT("NTSC _StopMonitorEngine failed\n");
        return (MTR_NOT_OK);    	
    }

    x_sema_unlock(pMTPdCtx->t_escape_mon_suspend);
    x_sema_delete(pMTPdCtx->t_escape_mon_suspend);

    x_msg_q_delete(pMTPdCtx->t_MoniCmdMsgQ);
    //add driver implementation code here    
    x_mem_free(pMTPdCtx); //free MTAL glue Ctx

    /* TunerClose */ 
    //i4Status = TunerClose(); //don't do LTDIS TunerClose from MTAL

    _fgNTSCInit = FALSE;
    return MTR_OK;
}

/** Connect NTSC module.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_NTSC_Connect(VOID)
{
    MTFE_NTSC_PRINT(" - void\n");
    //add driver implementation code here
   //_MTFE_ATSC_DisConnect();
   MTAL_DISCONNECT_DTV();
    return MTR_OK;
}
/** Disconnet NTSC module.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

//static MT_RESULT_T _MTFE_NTSC_DisConnect(VOID)
MT_RESULT_T _MTFE_NTSC_DisConnect(VOID)
{
    MTFE_NTSC_PRINT(" - void\n");
    //add driver implementation code here
    if(_fgNTSCInit!=TRUE){
        return MTR_OK;
    }
#if defined (USE_ATD_IF_DEMOD) || defined(ENABLE_SIF_BYPASS)
        mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
        ATD_DisConnect(pMTPdCtx->pDemodCtx);
        mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);   
#endif

    pMTPdCtx->fgBreakMonitor = TRUE; //suspend monitor
    pMTPdCtx->fgBreakSearchNearBy = TRUE;//if doing SearchNearBy, need break it
    pMTPdCtx->fgBreakAFT = TRUE; //if doing AFT, need break it
    return MTR_OK;
}
/**Set visual carrier frequency.
*@param  rParameter             The input paramter (Visual carrier freq...)
*@retval MT_OK                  Success.
*@retval MT_NOT_OK              Fail.
*/

static MT_RESULT_T _MTFE_NTSC_TunerSetFreq(unsigned long arg)
{
    MTFE_NTSC_PARAMETER_T *prParameter;
    MTFE_NTSC_PARAMETER_T rParameter;
    UINT8  u1AnaMode = MOD_ANA_CABLE; 
    
	prParameter = (MTFE_NTSC_PARAMETER_T *)arg;
    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_NTSC_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter,rParameter,MTFE_NTSC_PARAMETER_T);
    MTFE_NTSC_PRINT(" - rParameter.u4DesireVFreqInkHz = %d\n"
        "rParameter.rRfMode = %d\n"
        "rParameter.rSurfMode = %d\n",
        rParameter.u4DesireVFreqInKHz,
        rParameter.rRfMode,
        rParameter.rSurfMode);
        
    //add driver implementation code here
    if (rParameter.rRfMode == MTFE_RF_AIR)
    	u1AnaMode = MOD_ANA_TERR;
    	
    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
    // _fgAutoSearch will be applied in NPTV driver. 
    if (rParameter.rSurfMode == MTFE_SURF_SCAN) /* Channel scan */       	
        _fgAutoSearch = TRUE;
    else  /* Channel change */
        _fgAutoSearch = FALSE;
            
    pMTPdCtx->u4CurFreq = rParameter.u4DesireVFreqInKHz;//for AFT reference
    //Maggie todo: check with justin, pal use channel scan/channel change as the third parameters
    AnalogPINA_TunerSetFreq(pMTPdCtx->pDemodCtx, rParameter.u4DesireVFreqInKHz, u1AnaMode);
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);         

    return MTR_OK;
}
/**Search visual carrier frequency.
* Registration a notify function (ID = MTFE_NTSC_CB_ID_SEARCH_NEARBY_NFY) before using this API.
*@param  rParameter             The input paramter (Visual carrier freq...)
*@retval MT_OK                  Success.
*@retval MT_NOT_OK              Fail.
*/

static MT_RESULT_T  _MTFE_NTSC_StartSearchNearbyFreq(unsigned long arg)
{
    MTFE_NTSC_PARAMETER_T *prParameter;
    MTFE_NTSC_PARAMETER_T rParameter;
    UINT8 fgRet = MTR_OK;
    TUNER_CMD_QUEUE_T	rCmdQ;
    prParameter = (MTFE_NTSC_PARAMETER_T *)arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_NTSC_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter,rParameter,MTFE_NTSC_PARAMETER_T);

    MTFE_NTSC_PRINT(" - rParameter.u4DesireVFreqInkHz = %d\n"
        "rParameter.rRfMode = %d\n"
        "rParameter.rSurfMode = %d\n",
        rParameter.u4DesireVFreqInKHz,
        rParameter.rRfMode,
        rParameter.rSurfMode);
    //add driver implementation code here
    MTFE_NTSC_PRINT("In kernal: Change to NTSC_SEARCH_STATE_START\n");


    if(pMTPdCtx->fgBreakMonitor == TRUE){
        pMTPdCtx->fgBreakMonitor = FALSE; //resume monitor to do search
        mcSEMA_UNLOCK_MTFE_S(pMTPdCtx->t_escape_mon_suspend,"ntsc_mon_suspend");
    }
    pMTPdCtx->fgBreakSearchNearBy = TRUE; //if last requeset is not yet finish,break it
    pMTPdCtx->fgBreakAFT = TRUE;//AFT and SearchNearBy are exclusive in_MonitorProcess


    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
    //Maggie todo: do we need to set freq here?(sync with Justin. AnalogPINA_TunerSetFreq())

//    _rSearchNearbyState = NTSC_SEARCH_STATE_START;    
//    _u4DesireVFreqInKHz  = rParameter.u4DesireVFreqInKHz;
//    _u4ResultVFreqInKHz = _u4DesireVFreqInKHz;
// 
    pMTPdCtx->u4CurFreq = rParameter.u4DesireVFreqInKHz;//for AFT reference

    // _fgAutoSearch will be applied in NPTV driver. 
    if (rParameter.rSurfMode == MTFE_SURF_SCAN) /* Channel scan */       	
        _fgAutoSearch = TRUE;
    else  /* Channel change */
        _fgAutoSearch = FALSE;
  	  	
    // enable SearchNearBy in monitor process
    rCmdQ.CmdId = TUNER_CMD_STARTSEARCH;
    rCmdQ.CmdInfo.ConnectParam.u4DesireVFreqInKHz = rParameter.u4DesireVFreqInKHz;
    rCmdQ.CmdInfo.ConnectParam.rRfMode  = rParameter.rRfMode;
    rCmdQ.CmdInfo.ConnectParam.rSurfMode = rParameter.rSurfMode;

	if ((x_msg_q_send(pMTPdCtx->t_MoniCmdMsgQ, &rCmdQ, sizeof(TUNER_CMD_QUEUE_T), 1)) != OSR_OK){
    	MTFE_NTSC_PRINT("Error: x_msg_q_send failed %s\n",__FUNCTION__);
            fgRet = MTR_NOT_OK;
    	}

    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
    
    return fgRet;
}
/**Break the search visual carrier frequency process.
*@retval MT_OK                  Success.
*@retval MT_NOT_OK              Fail.
*/

static MT_RESULT_T  _MTFE_NTSC_BreakSearchNearbyFreq(VOID)
{
    MTFE_NTSC_PRINT(" - void\n");
    //add driver implementation code here
        //mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
    //you should not enable mcSEMA_LOCK_MTFE here , due to SearchNearBy is in the mcSEMA_LOCK_MTFE protection
    if(pMTPdCtx->fgDoSearchNearby) //check whether monitor thread is doing SearchNearBy
    {
        pMTPdCtx->fgBreakSearchNearBy = TRUE; //if last requeset is not yet finish,break it
        //todo: consider do a blocking here, until break really happen
        //if break happen, user space should got a callback event MTFE_ATV_BREAK_SEARCH_NEARBY
    }
    //mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
    
    return MTR_OK;
}

/** Get lock status.
*@param	 prStatus           Report demod status.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_NTSC_GetSync(unsigned long arg)
{
    INT16 i2LockStatus;
    MTFE_NTSC_PARAMETER_T *prParamStatus;
    MTFE_NTSC_PARAMETER_T rParamStatus;
    prParamStatus = (MTFE_NTSC_PARAMETER_T*)arg;
   
    MTFE_NTSC_PRINT("In kernel %s\n", __FUNCTION__);
    
    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when access PI Primitive API, must use this lock for protection

    i2LockStatus = (INT16) AnalogPINA_TunerCheckVIFLock(pMTPdCtx->pDemodCtx, pMTPdCtx->u4CurFreq);
    rParamStatus.rStatus.u4VFreqInKHz = pMTPdCtx->u4CurFreq;
    rParamStatus.rStatus.i2LockStatus = 0;    
    if (i2LockStatus) // VIF lock    
       mcSET_BIT(rParamStatus.rStatus.i2LockStatus, MTFE_ATV_VIF_LOCK_STATUS_BIT);      
    if (fgDrvTunerCheckTVDLock(0x30))
       mcSET_BIT(rParamStatus.rStatus.i2LockStatus, MTFE_ATV_TVD_LOCK_STATUS_BIT);      
   	
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);

    USR_SPACE_ACCESS_VALIDATE_ARG(prParamStatus, rParamStatus);
    COPY_TO_USER_ARG(prParamStatus, rParamStatus, MTFE_NTSC_PARAMETER_T);
    MTFE_NTSC_PRINT(" - LockStatus = %d\n", rParamStatus.rStatus.i2LockStatus);

    return MTR_OK;

}

/** Check analog tuner driver software version.
*@param  u1Version          Software Version.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_NTSC_GetVer(unsigned long arg)
{
    UINT8 *pu1Version;
    UINT8 au1Version[4];
    pu1Version = (UINT8*)arg;

    //add driver implementation code here
    
    /* Software Version: x.yyy */
    au1Version[0] = 1; // x
    au1Version[1] = 0; // y
    au1Version[2] = 0;
    au1Version[3] = 0;

    USR_SPACE_ACCESS_VALIDATE_ARG(pu1Version,au1Version);
    COPY_TO_USER_ARG(pu1Version,au1Version,au1Version);
    MTFE_NTSC_PRINT(" - NTSC Ver: %d.%d%d%d\n",au1Version[0], au1Version[1], au1Version[2], au1Version[3]);
    return MTR_OK;
}

/**Register ntsc module callback function.
*@param  pfCb               Callback function.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_NTSC_RegCb(unsigned long arg)
{
    //add driver implementation code here

    return MTR_OK;
}
#if 0
//Maggie todo: add new io API, add ut, add io mapping from user to kernel
/**Set analog TV system.
*@param  fgEnAFT           The input paramter (AFT enable flag)
*@retval MTR_OK                  Success.
*@retval MTR_NOT_OK              Fail.
*/
static MT_RESULT_T _MTFE_NTSC_SetAFT(unsigned long arg)
{
    BOOL  *pfgEnAFT;
    BOOL  fgEnAFT;
    pfgEnAFT = (BOOL*)arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(pfgEnAFT, BOOL);
    COPY_FROM_USER_ARG(pfgEnAFT, fgEnAFT, BOOL);
    MTFE_NTSC_PRINT(" set AFT = %d\n", fgEnAFT);

    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when call PI primitive API must use this lock for protection
    //add driver implementation code here
    if (fgEnAFT) {
        // enable AFT monitor
        pMTPdCtx->fgEnAFT=TRUE;
    }
    else{
        pMTPdCtx->fgEnAFT=FALSE;
    }

    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);

    return MTR_OK;

}
#endif
int mtal_ioctl_mtfe_ntsc(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
    MT_RESULT_T eRet = MTR_OK;
    MTFE_NTSC_PRINT("-------ntsc ioctl cmd-------\r\n");
    if(u1ExclSemaInit==1){
        mcSEMA_LOCK_MTFE_S(Exclusive_DigiAnaSema,"Ex_DiAna");
    }

	switch (cmd)
	{
		case MTAL_IO_NTSC_OPEN:
			eRet= _MTFE_NTSC_Open();
            break;
		case MTAL_IO_NTSC_CLOSE:
			eRet= _MTFE_NTSC_Close();
            break;
		case MTAL_IO_NTSC_CONNECT:
			eRet= _MTFE_NTSC_Connect();
            break;
		case MTAL_IO_NTSC_DISCONNECT:
		    eRet= _MTFE_NTSC_DisConnect();
            break;
		case MTAL_IO_NTSC_TSF:
		    eRet= _MTFE_NTSC_TunerSetFreq( arg);
            break;
		case MTAL_IO_NTSC_SSNF:
		    eRet= _MTFE_NTSC_StartSearchNearbyFreq( arg);
            break;
		case MTAL_IO_NTSC_BSNF:
		    eRet= _MTFE_NTSC_BreakSearchNearbyFreq();
            break;
		case MTAL_IO_NTSC_GETSYNC:
			eRet= _MTFE_NTSC_GetSync(arg);
            break;
		case MTAL_IO_NTSC_GETVER:
			eRet= _MTFE_NTSC_GetVer( arg);
            break;
		case MTAL_IO_NTSC_REGCB:
		    eRet= _MTFE_NTSC_RegCb( arg);
            break;
		default:
		    break;
	}

    if(u1ExclSemaInit==1){
        mcSEMA_UNLOCK_MTFE_S(Exclusive_DigiAnaSema, "Ex_DiAna");
    }
	return eRet;
}

    

