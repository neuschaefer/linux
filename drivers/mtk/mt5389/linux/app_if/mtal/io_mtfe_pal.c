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
 * $RCSfile: io_mtfe_pal.c,v $
 * $Revision: #15 $
 *---------------------------------------------------------------------------*/

/** @file io_mtfe_pal.c
 *  This header file declares exported APIs of FE PAL/SECAM module.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------


/**********************add by liuqu,20091224*****/
#include "pd_atd_if.h" 
#include "pi_atd_if.h"         
#include "pd_atd_glue.h"
#include "aud_if.h"

#include "mtfe_pal.h"
#include "tuner_if.h"
#include <linux/mtal_ioctl.h>
//#include "pd_anaeu_if.h"

/***********************************/
#include "io_mtfe_pal.h"
#include "cb_data.h"
#include "io_mtfe_pd_def.h"
#include "x_tuner.h"
#include "tuner_interface_if.h"
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#if defined(CC_MT5395)||defined(CC_MT5396)||defined(CC_MT5389)
#define CC_MT5365
#endif
//-----------------------------------------------------------------------------
// Bit Process
//-----------------------------------------------------------------------------
#define mcBITL(b)               (1UL << (b))
#define mcBIT(b)                mcBITL(b)
#define mcSET_MASK(a, b)        ((a) |= (b))
#define mcCHK_BIT1(a, b)        ((a) & mcBIT(b))
#define mcTEST_BIT(a, b)        mcCHK_BIT1(a, b)
#define mcSET_BIT(a, b)         mcSET_MASK(a, mcBIT(b))
#define ATVONLYMODE   1 //Enable Analog Only TV Modal

#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(int)))                            \
    {                                                       \
        printk("mtal_ioctl_pal argument error\n");     \
        return MTR_ERR_INV;                                          \
    }

#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(int)))                        \
    {                                                       \
        printk("mtal_ioctl_pal argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_pal argument error\n");     \
        return MTR_ERR_INV;                                          \
    }

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_pal argument error\n");      \
        return MTR_ERR_INV;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_pal argument error\n");     \
        return MTR_ERR_INV;                                          \
    }

#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_pal argument error\n");      \
        return MTR_ERR_INV;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        printk("mtal_ioctl_pal argument error\n");     \
        return MTR_ERR_INV;                                          \
    }

#define COPY_TO_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_pal argument error\n");      \
        return MTR_ERR_INV;                                          \
    }
#define COPY_FROM_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_from_user(&ArgKernel,(void __user *)ArgUsr,    \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_pal argument error\n");      \
        return MTR_ERR_INV;                                          \
    }


#define MTAL_PAL_MON_POLL_INTRVL             500  // 500ms,wenming 110928 change 50ms to 500ms
#define MTAL_PAL_MON_AFT_INTRVL    500  // 500ms
#define MTAL_PAL_MON_ATF_CNT       (MTAL_PAL_MON_AFT_INTRVL/MTAL_PAL_MON_POLL_INTRVL)

#define MTAL_PAL_FULL_LOCK         50
#define MTAL_PAL_NOT_LOCK         300
#define MTAL_PAL_NOT_LOCK_1        200


#define MTAL_PAL_IO_VERSION "PAL_IO_1.01"
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static MTAL_PAL_PD_CTX_T  *pMTPdCtx;
static BOOL _fgPALInit=FALSE;
/*all variable used in this file should put into pMTPdCtx exclusive of Exclusive_DigiAnaSema and u1ExclSemaInit*/
static HANDLE_T    Exclusive_DigiAnaSema=0;
static UINT8       u1ExclSemaInit=0;
static BOOL      fgFixAFTSet = FALSE; /*wenming 2011-10-11:for UT to fix AFT Set*/
extern BOOL _fgAutoSearch;

//-----------------------------------------------------------------------------
// extern function

extern BOOL fgGetTVDSts(ATD_CTX_T *psDemodCtx,UINT16 u1ChkTime);
extern 	BOOL ATD_SetFreq(ATD_CTX_T *psDemodCtx, UINT32 u4FreqInKHz, UINT8 fgAutoSearch);
extern 	UINT8 ATD_ChipInit(ATD_CTX_T *psDemodCtx);
extern 	void ATD_SetSystem(ATD_CTX_T *psDemodCtx, UINT8 u1SubSysIdx);
extern	INT32  ATD_GetCFOKHz(ATD_CTX_T *psDemodCtx, BOOL fgLogMessage);
extern UINT8 ATD_GetReg(ATD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
extern INT32 ATD_Recover(ATD_CTX_T *psDemodCtx);
extern 	UINT8 ATD_StartAcq(ATD_CTX_T *psDemodCtx, UINT8 fgScanMode);
extern  INT32 ATD_Recover1(ATD_CTX_T *psDemodCtx,UINT32 *wFreqInKHz);
extern 	UINT8 ATD_GetVIFLock(ATD_CTX_T *psDemodCtx, BOOL fgLogMessage);
extern 	UINT8 ATDEU_SearchNearbyFreq(ATD_CTX_T *psDemodCtx, UINT32 *wFreqInKHz, x_break_fct _BreakFct,
									 void *pvArg, UINT32 u4FreqBDKHz);
extern 	UINT8 ATDEU_Monitor(ATD_CTX_T *psDemodCtx, UINT32 *wFreqInKHz, x_break_fct _BreakFct, void *pvArg);
extern INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);
extern 	void *TunerGetCtx(UINT16 TunerID);
extern void ATD_DisConnect(ATD_CTX_T *psDemodCtx);
extern 	void vTvd3dConnect(UINT8 bPath, UINT8 bOnOff);
extern 	void ATD_BypassSIF2Aud(ATD_CTX_T *psDemodCtx, UINT8 u1SubSysIdx);
extern  void TunerATVOpenStatus(UINT8 AStatus);
extern void AUD_DspAtvMuteEnable(UINT8 u1DecId, BOOL fgEnable);

//-----------------------------------------------------------------------------
#ifndef CC_FEDTMB
extern void Get_MTFE_DVBT_Sema_Ex_DiAna(HANDLE_T *sema);
extern MT_RESULT_T _MTFE_DVBT_DisConnect(VOID);
#else
extern void Get_MTFE_DTMB_Sema_Ex_DiAna(HANDLE_T *sema);
extern MT_RESULT_T _MTFE_DTMB_DisConnect(VOID);
#endif

//-----------------------------------------------------------------------------
// Static function
//-----------------------------------------------------------------------------

// liuqu added for functions transfer,20091224
#if (defined (CC_MT5363) || defined (CC_MT5365) || defined(CC_MT5396))||defined(CC_MT5389) &&  defined (USE_ATD_IF_DEMOD)  
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
 static BOOL  AnalogPIEU_fgDrvTunerCheckTVDLock( UINT16 bTryLoopCnt )
{
  BOOL fgReturn;
//use the pi demod API!!
  fgReturn = fgGetTVDSts(pMTPdCtx->pDemodCtx,bTryLoopCnt);
  
//  fgReturn = fgDrvTvdCheckTVDLock(CHN_LOCK_CHECK_TIME);

  MTFE_PAL_PRINT("fgDrvTvdCheckTVDLock =%d\n",fgReturn);

  return fgReturn;

}

 static BOOL  AnalogPIEU_TunerSetFreq(ATD_CTX_T *psAtdPiCtx, UINT32 wFreqInKH, UINT8 u1Mode,UINT8 u1SubSysIdx)
 {
    BOOL fgReturn;
    psAtdPiCtx->u1SubSysIdx = u1SubSysIdx;

	/*wenming 111010:do not set freq when no change,to make sure AFT stable*/
	if(wFreqInKH == pMTPdCtx->u4CurFreq)
	{
		fgReturn = TRUE;
	}
	else
	{
		fgReturn = ATD_SetFreq(psAtdPiCtx, wFreqInKH, u1Mode);  // change the sequence !!liuqu,20110112
		pMTPdCtx->u4CurFreq = wFreqInKH;
	}

    ATD_ChipInit(psAtdPiCtx);
    ATD_SetSystem(psAtdPiCtx, u1SubSysIdx);

    // Justin TODO, fgScanMode & psAtdeuDemodCtx->u1PF necessary??
    if (u1Mode == ATV_CONN_MODE_CHAN_SCAN)
    {
        psAtdPiCtx->u1PF = 0x01;  //enable pilot filter when initial scan

        ATD_StartAcq(psAtdPiCtx, TRUE);
    }
    else if(u1Mode == ATV_CONN_MODE_CHAN_CHANGE)
    {
        psAtdPiCtx->u1PF = 0x01;  //enable pilot filter when channel change
        ATD_StartAcq(psAtdPiCtx, FALSE);
    }

    return fgReturn;

 }
 
 static UINT8 AnalogPIEU_TunerGetSystem(ATD_CTX_T *psAtdPiCtx)
 {
#if 0  // it's not correct to use the PD CTX in MTAL IO layer!!
    ATD_PD_CTX_T    *pPALPdCtx;   

    pPALPdCtx = (ATD_PD_CTX_T *)TunerGetCtx(ANALOG_FE);

    return pPALPdCtx->u1SubSysIdx;
#else
    return psAtdPiCtx->u1SubSysIdx;
#endif
 }
 
 static BOOL  AnalogPIEU_TunerSetSystem(ATD_CTX_T *psAtdPiCtx, UINT8 u1SubSysIdx)
 {
   ATD_SetSystem(psAtdPiCtx,  u1SubSysIdx);

   return TRUE;
 }
 
 static UINT8 AnalogPIEU_TunerCheckVIFLock(ATD_CTX_T *psAtdPiCtx)
 {
  return (ATD_GetVIFLock(psAtdPiCtx, TRUE));
 }
 
 static UINT8 AnalogPIEU_TunerSearchNearbyFreq(ATD_CTX_T *psAtdPiCtx, UINT32 *wFreqInKHz, x_break_fct _BreakFct, VOID *pvArg, UINT32 u4FreqBDKHz)
 {
    UINT8 u1Return;

    ATD_ChipInit(psAtdPiCtx);
    ATD_SetSystem(psAtdPiCtx, psAtdPiCtx->u1SubSysIdx);

    ATD_SetFreq(psAtdPiCtx, *wFreqInKHz, ATV_CONN_MODE_CHAN_SCAN);

    psAtdPiCtx->u1PF = 0x01;  //enable pilot filter when initial scan

    ATD_StartAcq(psAtdPiCtx, TRUE);

    psAtdPiCtx->fgCheckCR = TRUE;

    u1Return =ATDEU_SearchNearbyFreq(psAtdPiCtx, wFreqInKHz,  _BreakFct, pvArg, u4FreqBDKHz);


    return u1Return ;
 }
 
 static UINT8 AnalogPIEU_TunerMonitor(ATD_CTX_T *psAtdPiCtx, UINT32 *wFreqInKHz, x_break_fct _BreakFct, VOID *pvArg)
 {
    return( ATDEU_Monitor(psAtdPiCtx, wFreqInKHz,  _BreakFct, pvArg));
 }
#endif


static void _CB_PutEventLockStatus(UINT8 u1SearchResult, UINT32 u4Freq)
{
    MTAL_FE_EU_TUNER_CALLBACK_INFO_T 	rCbInfo;

    rCbInfo.CbId = MTAL_FE_PAL_LOCK_STATUS;
    if(AnalogPIEU_fgDrvTunerCheckTVDLock(0x30)){ //justin todo: decide the look count
        //TVD LOCK
        mcSET_BIT(u1SearchResult,MTFE_ATV_TVD_LOCK_STATUS_BIT);
    }
    //BIT0: MTFE_ATV_VIF_LOCK_STATUS_BIT
    //BIT1: MTFE_ATV_TVD_LOCK_STATUS_BIT
    //BIT2: MTFE_ATV_BREAK_SEARCH_NEARBY_BIT
    rCbInfo.CbIdInfo.LockStatus = u1SearchResult;
    MTFE_PAL_PRINT("PAL _CB_PutEventLockStatus: LockStatus? %d\n", rCbInfo.CbIdInfo.LockStatus);
    rCbInfo.CbParam.atvParams.u4Freq = u4Freq;
    _CB_PutEvent(CB_MTAL_FE_PAL_TUNER_TRIGGER, sizeof(MTAL_FE_EU_TUNER_CALLBACK_INFO_T), &rCbInfo);
}
#if 0
static void _CB_PutEventLockStatus(MTFE_DTV_STATUS_T LockStatus)
{
    MTAL_FE_EU_TUNER_CALLBACK_INFO_T 	rCbInfo;

    rCbInfo.CbId = MTAL_FE_PAL_LOCK_STATUS;

    switch(LockStatus){
    case MTFE_ATV_VIF_LOCK:
        rCbInfo.CbIdInfo.LockStatus = MTFE_ATV_VIF_LOCK;
        break;
    case MTFE_ATV_VIF_UNLOCK:
        rCbInfo.CbIdInfo.LockStatus = MTFE_ATV_VIF_UNLOCK;
        break;
    case MTFE_ATV_BREAK_SEARCH_NEARBY:
        rCbInfo.CbIdInfo.LockStatus = MTFE_ATV_BREAK_SEARCH_NEARBY;
        break;
    default:
        rCbInfo.CbIdInfo.LockStatus = MTFE_ATV_UNKNOWN;
        break;
    }
    _CB_PutEvent(CB_MTAL_FE_PAL_TUNER_TRIGGER, sizeof(MTAL_FE_EU_TUNER_CALLBACK_INFO_T), &rCbInfo);

}
#endif
//static BOOL _i4AnaMoniBreak(VOID)                                                            
static BOOL _i4AnaMoniBreakSearchNearBy(VOID)                                                            
{                                                                                                                                                                                                           
    MTFE_PAL_PRINT("@PAL pi check wanna break or not=%d\r\n",pMTPdCtx->fgBreakSearchNearBy);                                                                                     
    return pMTPdCtx->fgBreakSearchNearBy;
} 

static BOOL _i4AnaMoniBreakAFT(VOID)                                                            
{                                                                                                       
    return pMTPdCtx->fgBreakAFT;
} 
/*wenming 110926:Enable or disable AFT
*@param fgEnAFT    The input paramter (AFT enable flag)
*@param fgUTSetAFT  The input paramter (UT set flag)
*/
static VOID Set_MTFE_PAL_AFT(BOOL fgEnAFT, BOOL fgUTSetAFT)
{
    //mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);     //wenming 110926:need to lock???
    //add driver implementation code here
    if((!fgFixAFTSet)||(fgFixAFTSet&&fgUTSetAFT))
	{
		if (fgEnAFT) {
		    // enable AFT monitor
		    if(pMTPdCtx->fgBreakMonitor == TRUE){
		        pMTPdCtx->fgBreakMonitor = FALSE; //resume monitor to do AFT
		        mcSEMA_UNLOCK_MTFE_S(pMTPdCtx->t_escape_mon_suspend,"pal_mon_suspend");
		    }
		    pMTPdCtx->fgEnAFT = TRUE;
		    pMTPdCtx->fgBreakAFT = FALSE; 
		}
		else{
		    pMTPdCtx->fgEnAFT = FALSE;
		    pMTPdCtx->fgBreakAFT = TRUE; 
		}
	}
//mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
}


/**PAL monitor process.
*@param  pvArg               
*@retval VOID
*/
static VOID _MonitorProcess(VOID)
{
//    INT16   i2LockStatus;
//    MTAL_FE_EU_TUNER_CALLBACK_INFO_T 	rCbInfo;
    UINT16                  u2MsgIdx;
    TUNER_CMD_QUEUE_T       rCmdQ;
    SIZE_T                  zMsgSize = sizeof(UINT32);
    INT32                   i4Result;
    UINT8                   u1SearchResult=0;
    UINT16  u2_c = 0;	
    UINT16  u2_i = 0;
	//UINT16     i = 0;
	UINT8 ucNoiselevel=0;
    INT32 i2SigLevel=0;
	
    while (pMTPdCtx->fgMonitorThreadRunning)
    {  
        i4Result = x_msg_q_receive_timeout(&u2MsgIdx, &rCmdQ, &zMsgSize, &pMTPdCtx->t_MoniCmdMsgQ, 1, MTAL_PAL_MON_POLL_INTRVL);
       // MTFE_PAL_PRINT("PAL Monitor Get MsgQ=%d\n",i4Result);
        if( (i4Result==OSR_OK) && (rCmdQ.CmdId == TUNER_CMD_STARTSEARCH)){
            pMTPdCtx->fgDoSearchNearby = TRUE;//keep do searchNearBy till finish callback event
            pMTPdCtx->fgBreakSearchNearBy = FALSE;//init a new searchNearBy
        }
        else{
            //do nothing here,most case are doing AFT
        }

        if (pMTPdCtx->fgBreakMonitor) //condition for suspend monitor
        {
            MTFE_PAL_PRINT("Entering io_mtfe_pal mon_suspend\n");
            mcSEMA_LOCK_MTFE_S(pMTPdCtx->t_escape_mon_suspend,"pal_mon_suspend");
            MTFE_PAL_PRINT("Escape io_mtfe_pal mon_suspend\n");
        }
        else if(pMTPdCtx->fgDoSearchNearby)
        {
            UINT32 u4Freq;
            UINT32 u4FreqBound;

            u4Freq = rCmdQ.CmdInfo.ConnectParam.u4StartVFreqInKHz;
            u4FreqBound = rCmdQ.CmdInfo.ConnectParam.u4StopVFreqInKHz;
            mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when call PI primitive API must use this lock for protection
            //rCmdQ.CmdInfo.ConnectParam.u4StartVFreqInKHz
            //rCmdQ.CmdInfo.ConnectParam.u4StopVFreqInKHz
            // //rCmdQ.CmdInfo.ConnectParam.eAtvMode
            // //rCmdQ.CmdInfo.ConnectParam.eSurfMode
            //justin todo, check whether parameter mapping to API AnalogPIEU_TunerSearchNearbyFreq is correct?
            //u1SearchResult = AnalogPIEU_TunerSearchNearbyFreq(cATUNER_PI_CTX(psAtvPdCtx), &u2FFF, _BreakFct, pvArg, u2FFFBD);
            printf("fgDoSearchNearby..1u4Freq=%d\n",u4Freq);
            u1SearchResult = AnalogPIEU_TunerSearchNearbyFreq(pMTPdCtx->pDemodCtx, &u4Freq, (x_break_fct)_i4AnaMoniBreakSearchNearBy, NULL, u4FreqBound);
            _CB_PutEventLockStatus(u1SearchResult, u4Freq);
            printf("fgDoSearchNearby..2u4Freq=%d\n",u4Freq);
#if 0
            if(u1SearchResult == TRUE) //VIF Lock? //justin todo, is it right?
            {
                _CB_PutEventLockStatus(MTFE_ATV_VIF_LOCK);
            }
            else if(u1SearchResult == -1) //justin to do, modify AnalogPIEU_TunerSearchNearbyFreq return -1 if break really happen
            {
                _CB_PutEventLockStatus(MTFE_ATV_BREAK_SEARCH_NEARBY);
            }
            else{//=0 FALSE
                _CB_PutEventLockStatus(MTFE_ATV_VIF_UNLOCK);
            }
#endif
            pMTPdCtx->fgDoSearchNearby = FALSE;
            pMTPdCtx->fgBreakSearchNearBy = FALSE; //when finish TunerSearchNearbyFreq, reset to FALSE
            pMTPdCtx->fgBreakAFT =FALSE; //let AFT can work in next run, //justin todo ,check when need to set pMTPdCtx->fgBreakAFT=TRUE
            mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
        }
        else // AFT
        { 
            INT32 i2SigLevel_tmp;
			INT32 i2SigLevel_dif;
			INT32 i4FreqOffset=0;

			UINT8 ucNoiselevel_tmp;
			UINT8 ucNoiselevel_dif;

			UINT8	u1tunerreg_2b;
			UINT8	u1tunerreg[6];
			UINT8	u1tunerregadd;
			UINT8   ii = 0;

			//UINT8 ucData;

			/* ITuner_OP( ITunerGetCtx(),itTunerRead,0x2b,&u1tunerreg_2b);
			 MTFE_PAL_PRINT("tuner read register 0x2B -> 0x%02X \n",u1tunerreg_2b);
			 u1tunerregadd=0x2f;
			 for(ii=0;ii<6;ii++)
			 {
			   u1tunerregadd+=ii;
			   ITuner_OP(ITunerGetCtx(),itTunerRead,u1tunerregadd,&u1tunerreg[ii]);
			   MTFE_PAL_PRINT("tuner read register 0x%02X -> 0x%02X \n",u1tunerregadd,u1tunerreg[ii]);
			 }			 
                      */
			
		    i4FreqOffset=ATD_GetCFOKHz(pMTPdCtx->pDemodCtx,true); 
			ATD_GetReg(pMTPdCtx->pDemodCtx, 0x242, &ucNoiselevel_tmp, 1);  // Get noise level
			MTFE_PAL_PRINT(" noiselevel is 0x%02X\n",ucNoiselevel_tmp);
			if (ucNoiselevel_tmp>=ucNoiselevel)
			    ucNoiselevel_dif=ucNoiselevel_tmp-ucNoiselevel;
			else
				ucNoiselevel_dif=0;
			
			ucNoiselevel=ucNoiselevel_tmp;
            if (((ITuner_OP( ITunerGetCtx(), itGetSignalLevel,0, &i2SigLevel_tmp)) == ITUNER_OK)&&(u2_c>0))//add by xiongfeng for fix ESD test case
            { 
                u2_c=0;
                if(i2SigLevel_tmp>=i2SigLevel)
                   i2SigLevel_dif=i2SigLevel_tmp-i2SigLevel;
				else
				   i2SigLevel_dif=i2SigLevel-i2SigLevel_tmp;
				
				i2SigLevel=i2SigLevel_tmp;
				 if((i2SigLevel<10)&&(ucNoiselevel_tmp>100))//add by xiongfeng to fix no signal audio problem
			    	{
                       AUD_DspAtvMuteEnable(0,1);
				    }
				else
					{
                       AUD_DspAtvMuteEnable(0,0);
					}
				MTFE_PAL_PRINT(" ucNoiselevel_dif = %d\n" "i2SigLevel_dif = %d\n", ucNoiselevel_dif ,i2SigLevel_dif);
				if(((i2SigLevel>30)&&(i2SigLevel<75)&&(ucNoiselevel_dif>100))||((i2SigLevel>25)&&(i2SigLevel<80)&&(i2SigLevel_dif<10)&&(ucNoiselevel>40)&&((i4FreqOffset<(-MTAL_PAL_NOT_LOCK_1))||(i4FreqOffset>MTAL_PAL_NOT_LOCK_1))))
					{
					   	if (ATD_Recover(pMTPdCtx->pDemodCtx))
						{
		        			MTFE_PAL_PRINT("ATD_Recover failed\n");
		    			}
					   pMTPdCtx->fgEnAFT =TRUE;				   
					   MTFE_PAL_PRINT("MTAL PAL doing ATD reset!...\n");	
				    }
				if((i2SigLevel>30)&&(i2SigLevel_dif<6)&&(ucNoiselevel>40)&&((i4FreqOffset<(-MTAL_PAL_NOT_LOCK))||(i4FreqOffset>MTAL_PAL_NOT_LOCK)))
					{
                         if (ATD_Recover1(pMTPdCtx->pDemodCtx,&pMTPdCtx->u4CurFreq)) 
                         {
                            MTFE_PAL_PRINT(("ATD_Recover failed\n"));
						 }
					   pMTPdCtx->fgEnAFT =TRUE;				   
					   MTFE_PAL_PRINT("MTAL PAL doing set Freq and System reset!...\n");	
				    }
				   /* if(ATD_CheckReg()||((i2SigLevel>30)&&(i2SigLevel_dif<6)&&(ucNoiselevel>40)&&((i4FreqOffset<(-MTAL_PAL_NOT_LOCK))||(i4FreqOffset>MTAL_PAL_NOT_LOCK))))
					{
                       if (ATD_Recover1(pMTPdCtx->pDemodCtx,&pMTPdCtx->u4CurFreq)) 
                         {
                            MTFE_DVBT_PRINT(("ATD_Recover failed\n"));
						 }
					   pMTPdCtx->fgEnAFT =TRUE;				   
					   MTFE_PAL_PRINT("MTAL PAL doing set Freq and System reset!...\n");	
					}*/
			}
			u2_c=0;
			if((pMTPdCtx->fgEnAFT) && (pMTPdCtx->u4CurFreq!=0))
			{
                u2_i++;//use to control delay period for AFT
                MTFE_PAL_PRINT("MTAL PAL Monitor! keep update parameter u2_i=%d\n",u2_i);
                if (u2_i > MTAL_PAL_MON_ATF_CNT) 
                {
                    u2_i = 0;
                    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when call PI primitive API must use this lock for protection
                    //Justin todo: need to consider how many ms need to delay for AFT, please adjust MTAL_PAL_MON_ATF_CNT
                    AnalogPIEU_TunerMonitor(pMTPdCtx->pDemodCtx, &pMTPdCtx->u4CurFreq, (x_break_fct)_i4AnaMoniBreakAFT, NULL);
                    MTFE_PAL_PRINT("MTAL PAL doing AFT!...\n");		
                    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
                }
                //Justin todo: need to consider how many ms need to delay for AFT
                mcDELAY_MS(MTAL_PAL_MON_POLL_INTRVL);   
            }
            else{
                //do nothing
            }
             u2_c++;
        }
        //don't do delay here, x_msg_q_receive() will do blocking
    }
    
    pMTPdCtx->t_monitor_thread = 0;
    MTFE_PAL_PRINT("MTAL MonitorProcess exit!\n");
    x_thread_exit();

}

/**Start PAL monitor engine.
*@param  VOID               
*@retval MTR_OK              Success.
*@retval MTR_NOT_OK          Fail.
*/
static MT_RESULT_T _StartMonitorEngine(VOID)
{  
    pMTPdCtx->fgMonitorThreadRunning = TRUE;
    if (x_thread_create(&pMTPdCtx->t_monitor_thread,
            "_PALMonThread",
            2048, 101,
            (x_os_thread_main_fct)_MonitorProcess, sizeof(void*), NULL) != OSR_OK)
    {
        MTFE_PAL_PRINT("_StartMonitorEngine PAL (mcTHREAD_CREATE): ERROR!\n");
        return (MTR_NOT_OK);
    }

    return (MTR_OK);
}

/**Stop PAL monitor engine.
*@param  VOID               
*@retval MTR_OK        MTR_NOT_OK      Success.
*@retval MTR_NOT_OK          Fail.
*/
static MT_RESULT_T _StopMonitorEngine(VOID)
{
    pMTPdCtx->fgMonitorThreadRunning = FALSE;
    MTFE_PAL_PRINT("In kernal:_MTAL_StopMonitorEngine PAL\n");

    while (pMTPdCtx->t_monitor_thread){
        mcDELAY_MS(10); // in ms
    }
    MTFE_PAL_PRINT("MTAL PD_StopMonitorEngine success PAL\n");

    return (MTR_OK);
}

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/** Initialize frond end pal module.
*@param VOID
*@retval MTR_OK                  Success.
*@retval MTR_NOT_OK              Fail.
*/

static MT_RESULT_T _MTFE_PAL_Open(VOID)
{
    MT_RESULT_T         rRet;
//    ATV_PD_CTX_T       *pPALPdCtx;    // liuqu change,20091224
     ATD_PD_CTX_T    *pPALPdCtx;   
    INT32               i4Status;
    TUNER_CMD_QUEUE_T	rCmdQ;
	UINT8               u1Astatus=0;

    MTFE_PAL_PRINT(" - void[%s]\n",MTAL_PAL_IO_VERSION);
    //add driver implementation code here


//to fix build error 20100301,mark this!!!
 //  printf ("[LEO]u4MtalLogMask = %d\n",u4MtalLogMask);	

    //if (!u4MtalLogMask)	
     //u4MtalLogMask = 0xa0; // liuqu 20091227,enable MTAL DVBT AND PAL LOG

    if(_fgPALInit==TRUE){
        MTFE_PAL_PRINT("_fgPALInit==TRUE, do _MTFE_PAL_Close() first\r\n");
        // return MTR_DONT_CARE because it has been opened successfully already!		
        return (MTR_OK);
    }
    /* TunerOpen */
	#if ATVONLYMODE
	MTFE_PAL_PRINT("DVBT & DVBC open status %d\n",u1IfATVOnlyStatus);
	if(2==u1IfATVOnlyStatus){
	    u1Astatus=1;
        TunerATVOpenStatus(u1Astatus);
	}
	#endif
    i4Status = TunerOpen();
	
    if (i4Status < DRVAPI_TUNER_OK)
    {   
        MTFE_PAL_PRINT("TunerOpen failed. %d\n",i4Status);
        return (MTR_NOT_OK);
    }
    else if (i4Status > DRVAPI_TUNER_OK)
    {
        MTFE_PAL_PRINT("TunerOpen has been called. %d\n",i4Status);        
    }
    else{
        MTFE_PAL_PRINT("TunerOpen ok\n");        
    }

    pMTPdCtx = (MTAL_PAL_PD_CTX_T *) x_mem_calloc(1, sizeof(MTAL_PAL_PD_CTX_T));
    if (pMTPdCtx == NULL)
    {
        MTFE_PAL_PRINT("MTAL_PAL_PD_CTX_T malloc fail\n");
        return (MTR_NOT_OK);
    }

    //get Analog pd Ctx from nucleus pd glue
    pPALPdCtx = (ATD_PD_CTX_T *)TunerGetCtx(ANALOG_FE);
    pMTPdCtx->hHalLock = pPALPdCtx->hHalLock; 
  //  pMTPdCtx->pDemodCtx = &(pPALPdCtx->sAtvPiCtx);   
    pMTPdCtx->pDemodCtx = (ATD_CTX_T*)(pPALPdCtx->sAtdPiCtx);  // liuqu changed,20091224
    
    /* If tuner open success, start monitor engine */
    //mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
    //mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
#if 0
    #ifndef CC_FEDTMB
    Get_MTFE_DVBT_Sema_Ex_DiAna(&Exclusive_DigiAnaSema);
    #else
    Get_MTFE_DTMB_Sema_Ex_DiAna(&Exclusive_DigiAnaSema);
    #endif
#else
     #if defined(CC_FEDVBT)	
        Get_MTFE_DVBT_Sema_Ex_DiAna(&Exclusive_DigiAnaSema);	
     #elif defined(CC_FEDTMB)
        Get_MTFE_DTMB_Sema_Ex_DiAna(&Exclusive_DigiAnaSema);
     #elif defined(CC_FEDVBC)
        Get_MTFE_DVBC_Sema_Ex_DiAna(&Exclusive_DigiAnaSema);
     #endif 
#endif
    if(Exclusive_DigiAnaSema!=0){
        u1ExclSemaInit = 1;
    }

    pMTPdCtx->fgBreakMonitor = TRUE; //initial monitor is not running
    pMTPdCtx->fgBreakSearchNearBy = FALSE;
    pMTPdCtx->fgEnAFT = FALSE;
    pMTPdCtx->u4CurFreq = 0;//for AFT reference, default 0 let _monitorProcess can't do AFT
    pMTPdCtx->fgBreakAFT = TRUE; //default don't run AFT
#ifndef ENABLE_SIF_BYPASS
    if (x_sema_create(&pMTPdCtx->t_escape_mon_suspend, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        MTFE_PAL_PRINT(("%s: PAL x_sema_create(pMTPdCtx->t_escape_mon_suspend) Fail!\n"));
        return (MTR_NOT_OK);
    }
    x_sema_unlock(pMTPdCtx->t_escape_mon_suspend);

    if ((x_msg_q_create(&pMTPdCtx->t_MoniCmdMsgQ, "PAL_Moni_MsgQ", sizeof(rCmdQ), TUNER_CMD_QUEUE_SIZE)) != OSR_OK)
    {
        MTFE_PAL_PRINT(("%s: PAL x_msg_q_create(pMTPdCtx->t_escape_mon_suspend) Fail!\n"));
        return (MTR_NOT_OK);
    }
//#if 0
    pMTPdCtx->fgBreakMonitor = FALSE; //initial monitor is running


    #if 1  // liuqu,20091227,disable Monitor Engine,for it's always false -3
    rRet = _StartMonitorEngine();
    if(rRet != MTR_OK)
    {
        MTFE_PAL_PRINT("_StartMonitorEngine failed\n");
        return (MTR_NOT_OK);
    }
#endif
#endif
    _fgPALInit = TRUE;

    return MTR_OK;
}

/** Terminate frond end pal module.
*@param VOID
*@retval MTR_OK                  Success.
*@retval MTR_NOT_OK              Fail.
*/

static MT_RESULT_T _MTFE_PAL_Close(VOID)
{
    MTFE_PAL_PRINT(" - void\n");
    //add driver implementation code here
    if(_fgPALInit!=TRUE){
        MTFE_PAL_PRINT("_MTFE_PAL_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }
    if(pMTPdCtx->fgBreakMonitor == TRUE){
        pMTPdCtx->fgBreakMonitor = FALSE; //resume monitor 
        mcSEMA_UNLOCK_MTFE_S(pMTPdCtx->t_escape_mon_suspend,"pal_mon_suspend");
    }
    _StopMonitorEngine();
    
    x_sema_unlock(pMTPdCtx->t_escape_mon_suspend);
    x_sema_delete(pMTPdCtx->t_escape_mon_suspend);

    x_msg_q_delete(pMTPdCtx->t_MoniCmdMsgQ);
    //add driver implementation code here    
    x_mem_free(pMTPdCtx); //free MTAL glue Ctx
    //TunerClose(); //don't do LTDIS TunerClose() from MTAL

    _fgPALInit = FALSE;
    return MTR_OK;
}

/** Connet frond end pal module.
*@retval MTR_OK              Success.
*@retval MTR_NOT_OK          Fail.
*/
MT_RESULT_T _MTFE_PAL_Connect(VOID)
{
    MTFE_PAL_PRINT(" - void\n");
    //add driver implementation code here
#if 0
    #ifndef CC_FEDTMB
    _MTFE_DVBT_DisConnect();
    #else
    _MTFE_DTMB_DisConnect();
    #endif
#else
     #if defined(CC_FEDVBT)    
         _MTFE_DVBT_DisConnect();	
     #elif defined(CC_FEDTMB)
         _MTFE_DTMB_DisConnect();
     #elif defined(CC_FEDVBC)
        _MTFE_DVBC_DisConnect();
     #endif
#endif
    return MTR_OK;
}

/** Disconnet frond end pal module.
*@retval MTR_OK              Success.
*@retval MTR_NOT_OK          Fail.
*/
MT_RESULT_T _MTFE_PAL_DisConnect(VOID)
{
    MTFE_PAL_PRINT(" - void\n");
    //add driver implementation code here
    if(_fgPALInit!=TRUE){
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
    pMTPdCtx->u4CurFreq = 0; /*wenming 2011-10-12*/
    return MTR_OK;
}

/**Set visual carrier frequency.
*@param  rParameter             The input paramter (Visual carrier freq...)
*@retval MTR_OK                  Success.
*@retval MTR_NOT_OK              Fail.
*/

static MT_RESULT_T _MTFE_PAL_TunerSetFreq(unsigned long arg)
{
    MTFE_PAL_PARAMETER_T *prParameter;
    MTFE_PAL_PARAMETER_T rParameter;
    MT_RESULT_T eRet=MTR_OK;
    prParameter = (MTFE_PAL_PARAMETER_T *)arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_PAL_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter,rParameter,MTFE_PAL_PARAMETER_T);

    MTFE_PAL_PRINT(" - rParameter.u4StartVFreqInKHz = %d\n"
        "rParameter.u4StopVFreqInKHz = %d\n"
        "rParameter.eAtvMode = %d\n"
        "rParameter.eSurfMode = %d\n",
        rParameter.u4StartVFreqInKHz,
        rParameter.u4StopVFreqInKHz,
        rParameter.eAtvMode,
        rParameter.eSurfMode);
    //add driver implementation code here
        mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when call PI primitive API must use this lock for protection
        
	    if(rParameter.eSurfMode == MTFE_SURF_SCAN)
	    {
	        AnalogPIEU_TunerSetFreq(pMTPdCtx->pDemodCtx, rParameter.u4StartVFreqInKHz, ATV_CONN_MODE_CHAN_SCAN,rParameter.eAtvMode);
	        //_fgAutoSearch = TRUE;
	    }
	    else if(rParameter.eSurfMode == MTFE_SURF_CHANCHANGE)
	    {
	       // vTvd3dConnect(0,0);
	        AnalogPIEU_TunerSetFreq(pMTPdCtx->pDemodCtx, rParameter.u4StartVFreqInKHz, ATV_CONN_MODE_CHAN_CHANGE,rParameter.eAtvMode);
	       // vTvd3dConnect(0,1);
	        //_fgAutoSearch = FALSE;
	        /*wenming 110925:enable AFT when channel change*/
	        Set_MTFE_PAL_AFT(TRUE, FALSE);
	    }
	    else{
	        MTFE_PAL_PRINT("PAL not support rParameter.eSurfMode=0x%x\r\n",rParameter.eSurfMode);
	        eRet = MTR_NOT_SUPPORTED;
	        //_fgAutoSearch = FALSE;
	    }
	    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);

    return eRet;
}
/**Search visual carrier frequency.
* Registration a notify function (ID = MTFE_PAL_CB_ID_SEARCH_NEARBY_NFY) before using this API.
*@param  rParameter             The input paramter (Visual carrier freq...)
*@retval MTR_OK                  Success.
*@retval MTR_NOT_OK              Fail.
*/

static MT_RESULT_T  _MTFE_PAL_StartSearchNearbyFreq(unsigned long arg)
{
    MTFE_PAL_PARAMETER_T *prParameter;
    MTFE_PAL_PARAMETER_T rParameter;
	
    UINT8 u1Mode = ATV_CONN_MODE_CHAN_CHANGE;
    UINT8 fgRet = MTR_OK;
    TUNER_CMD_QUEUE_T	rCmdQ;
    prParameter = (MTFE_PAL_PARAMETER_T *)arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_PAL_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter,rParameter,MTFE_PAL_PARAMETER_T);

    MTFE_PAL_PRINT(" - rParameter.u4StartVFreqInKHz = %d\n"
        "rParameter.u4StopVFreqInKHz = %d\n"
        "rParameter.eAtvMode = %d\n"
        "rParameter.eSurfMode = %d\n",
        rParameter.u4StartVFreqInKHz,
        rParameter.u4StopVFreqInKHz,
        rParameter.eAtvMode,
        rParameter.eSurfMode);
    //add driver implementation code here
    if(pMTPdCtx->fgBreakMonitor == TRUE){
        pMTPdCtx->fgBreakMonitor = FALSE; //resume monitor to do search
        mcSEMA_UNLOCK_MTFE_S(pMTPdCtx->t_escape_mon_suspend,"pal_mon_suspend");
    }
    pMTPdCtx->fgBreakSearchNearBy = TRUE; //if last requeset is not yet finish,break it
    Set_MTFE_PAL_AFT(FALSE, FALSE);//wenming 110926,disable AFT
        

    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when call PI primitive API must use this lock for protection


    // need to align with scan mode define
    /*wenming 110915:fix MTFE_SURF and ATV_CONN_MODE diffrence */
    if(rParameter.eSurfMode == MTFE_SURF_SCAN)
    {
        u1Mode = ATV_CONN_MODE_CHAN_SCAN;
    }

    if (AnalogPIEU_TunerSetFreq(pMTPdCtx->pDemodCtx, rParameter.u4StartVFreqInKHz, u1Mode,rParameter.eAtvMode) != TRUE) /*jackon: why need use SV_TRUE?*/
    {
        MTFE_PAL_PRINT("Error: AnalogPIEU_TunerSetFreq\n");
        fgRet = MTR_NOT_OK;
    }
    if (AnalogPIEU_TunerSetSystem(pMTPdCtx->pDemodCtx, rParameter.eAtvMode) != TRUE)
    {
        MTFE_PAL_PRINT("Error: AnalogPIEU_TunerSetSystem\n");
        fgRet = MTR_NOT_OK;
    }
    if (fgRet == MTR_OK) {

        // enable SearchNearBy in monitor process
        rCmdQ.CmdId = TUNER_CMD_STARTSEARCH;
        rCmdQ.CmdInfo.ConnectParam.u4StartVFreqInKHz = rParameter.u4StartVFreqInKHz;
        rCmdQ.CmdInfo.ConnectParam.u4StopVFreqInKHz  = rParameter.u4StopVFreqInKHz;
        rCmdQ.CmdInfo.ConnectParam.eAtvMode = rParameter.eAtvMode;
        rCmdQ.CmdInfo.ConnectParam.eSurfMode = rParameter.eSurfMode;
        if ((x_msg_q_send(pMTPdCtx->t_MoniCmdMsgQ, &rCmdQ, sizeof(TUNER_CMD_QUEUE_T), 1)) != OSR_OK){
            MTFE_PAL_PRINT("Error: x_msg_q_send failed %s\n",__FUNCTION__);
            fgRet = MTR_NOT_OK;
        }
    }
#if 0
    if(rParameter.eSurfMode == MTFE_SURF_SCAN)
    {
        _fgAutoSearch = TRUE;
    }
    else
    {
        _fgAutoSearch = FALSE;
    }
#endif
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);

    return fgRet;
}
/**Break the search visual carrier frequency process.
*@retval MTR_OK                  Success.
*@retval MTR_NOT_OK              Fail.
*/

static MT_RESULT_T  _MTFE_PAL_BreakSearchNearbyFreq(VOID)
{
    MTFE_PAL_PRINT(" - void\n");
    //add driver implementation code here

    //mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when call PI primitive API must use this lock for protection
    //you should not enable mcSEMA_LOCK_MTFE here , due to SearchNearBy is in the mcSEMA_LOCK_MTFE protection
    if(pMTPdCtx->fgDoSearchNearby) //is ongoing doing SearchNearBy
    {
        pMTPdCtx->fgBreakSearchNearBy = TRUE; //if last requeset is not yet finish,break it
        //todo: consider do a blocking here, until break really happen
        //if break happen, user space should got a callback event MTFE_ATV_BREAK_SEARCH_NEARBY
    }
    //mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
    return MTR_OK;
}

/**Register pal module callback function.
*@param  CbId               Callback function ID.
*@param  pfCb               Callback function.
*@param  vArg               The argument of callback function.
*@retval MTR_OK              Success.
*@retval MTR_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_PAL_RegCb(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;
    UINT8 u1CbId;
    MTFE_PAL_PFN_CB pfCb;
    VOID* pvArg;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1CbId = (UINT8)rArg.ai4Arg[0];
    pfCb = (MTFE_PAL_PFN_CB)rArg.ai4Arg[1];
    pvArg = (VOID*)rArg.ai4Arg[2];

    MTFE_PAL_PRINT(" - u1CbId = %d, pfCb = 0x%x, pvArg = 0x%x\n",
                u1CbId,(int)pfCb,(int)pvArg);   

    //add driver implementation code here
    return MTR_OK;
}
/** Get analog demod VIF lock status.
*@param	 pi2LockStatus      Lock / unlock.
*@retval MTR_OK              Success.
*@retval MTR_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_PAL_GetSync(unsigned long arg)
{
    INT16 *pi2LockStatus;
    INT16 i2LockStatus = 0;
	INT32 i4FreqOffset=0;
    pi2LockStatus = (INT16*)arg;

    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when access PI Primitive API, must use this lock for protection
    //add driver implementation code here
    i2LockStatus = (INT16) AnalogPIEU_TunerCheckVIFLock(pMTPdCtx->pDemodCtx);
	i4FreqOffset=ATD_GetCFOKHz(pMTPdCtx->pDemodCtx,true);
	
    //need to transfer lockstatus bit definition
    if (i2LockStatus >=1)
    {
        i2LockStatus =0;  //clear lock status bit!
        mcSET_BIT(i2LockStatus,MTFE_ATV_VIF_LOCK_STATUS_BIT);  //set bit 0 to indicate VIF status!
    }

    if(AnalogPIEU_fgDrvTunerCheckTVDLock(0x30)){ //justin todo: decide the look count
        //TVD LOCK
        mcSET_BIT(i2LockStatus,MTFE_ATV_TVD_LOCK_STATUS_BIT);
    }

    if((i2LockStatus==2)&&(i4FreqOffset>=(-MTAL_PAL_FULL_LOCK))&&(i4FreqOffset<=MTAL_PAL_FULL_LOCK))//Xiongfeng add for fixing white_100 vif unlock case
    {
        i2LockStatus =0;
		mcSET_BIT(i2LockStatus,MTFE_ATV_VIF_LOCK_STATUS_BIT); 
		mcSET_BIT(i2LockStatus,MTFE_ATV_TVD_LOCK_STATUS_BIT);
	}
	
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
    USR_SPACE_ACCESS_VALIDATE_ARG(pi2LockStatus,INT16);
    COPY_TO_USER_ARG(pi2LockStatus,i2LockStatus,INT16);
    MTFE_PAL_PRINT(" - i2LockStatus = %d\n",i2LockStatus);

    return MTR_OK;
}

/** Get current analog TV system.
*@param	 peAtvMode           current analog TV system.
*@retval MTR_OK              Success.
*@retval MTR_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_PAL_GetSystem(unsigned long arg)
{
    MTFE_PAL_MOD_T  *peAtvMode;
    MTFE_PAL_MOD_T  eAtvMode;
    peAtvMode = (MTFE_PAL_MOD_T*)arg;

    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when access PI Primitive API, must use this lock for protection
    //add driver implementation code here
    eAtvMode = (MTFE_PAL_MOD_T) AnalogPIEU_TunerGetSystem(pMTPdCtx->pDemodCtx);

    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);

    USR_SPACE_ACCESS_VALIDATE_ARG(peAtvMode, MTFE_PAL_MOD_T);
    COPY_TO_USER_ARG(peAtvMode, eAtvMode, MTFE_PAL_MOD_T);
    MTFE_PAL_PRINT(" get eAtvMode = %d\n",eAtvMode);
    MTFE_PAL_PRINT(" get ATV System = %d\n",eAtvMode-MOD_ANA_TYPE_BEGIN);
    return MTR_OK;
}


/**Set analog TV system.
*@param  eAtvMode             The input paramter (analog TV system)
*@retval MTR_OK                  Success.
*@retval MTR_NOT_OK              Fail.
*/
static MT_RESULT_T _MTFE_PAL_SetSystem(unsigned long arg)
{
    MTFE_PAL_MOD_T  *peAtvMode;
    MTFE_PAL_MOD_T  eAtvMode;
    UINT8 fgRet = MTR_OK;
    peAtvMode = (MTFE_PAL_MOD_T*)arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(peAtvMode, MTFE_PAL_MOD_T);
    COPY_FROM_USER_ARG(peAtvMode, eAtvMode, MTFE_PAL_MOD_T);
    MTFE_PAL_PRINT(" set eAtvMode = %d\n",eAtvMode);

    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when call PI primitive API must use this lock for protection
    //add driver implementation code here
    if (AnalogPIEU_TunerSetSystem(pMTPdCtx->pDemodCtx, eAtvMode) != TRUE){
        fgRet = MTR_NOT_OK;
    }
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);

    return fgRet;
}

static MT_RESULT_T  _MTFE_PAL_AUTOSEARCH(unsigned long arg)
{
    UINT32 eAutoSearch;
    UINT8 fgRet = MTR_OK;
    eAutoSearch = (UINT32)arg;
    MTFE_PAL_PRINT(" set eAutoSearch = %d\n",eAutoSearch);
    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when call PI primitive API must use this lock for protection
    //add driver implementation code here
   // if (AnalogPIEU_TunerSetSystem(pMTPdCtx->pDemodCtx, eAtvMode) != TRUE){
    //    fgRet = MTR_NOT_OK;
    //}
    _fgAutoSearch = eAutoSearch;
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);

    return fgRet;
}

//Justin todo: add new io API, add ut, add io mapping from user to kernel
/**Set analog TV system.
*@param  fgEnAFT           The input paramter (AFT enable flag)
*@retval MTR_OK                  Success.
*@retval MTR_NOT_OK              Fail.
*/
static MT_RESULT_T _MTFE_PAL_SetAFT(unsigned long arg)
{
    BOOL  *pfgEnAFT;
    BOOL  fgEnAFT;
    pfgEnAFT = (BOOL*)arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(pfgEnAFT, BOOL);
    COPY_FROM_USER_ARG(pfgEnAFT, fgEnAFT, BOOL);
    MTFE_PAL_PRINT(" set AFT = %d\n", fgEnAFT);
	fgFixAFTSet = TRUE;   /*wenming 2011-10-11:fix AFT set*/

    Set_MTFE_PAL_AFT(fgEnAFT,TRUE);

    return MTR_OK;
}

//MT_RESULT_T _MTFE_PAL_GetTunerAttribute(DRV_GET_TYPE_T  e_get_type,VOID * Retval)
MT_RESULT_T _MTFE_PAL_GetFreqBound(unsigned long arg)
{
    MT_RESULT_T  eRet = MTR_OK;
    MTAL_IOCTL_2ARG_T rArg;
     TUNER_ANA_ATTRIBUTE_T  Ana_Freq_boudary;
    UINT32 *pu4UpperFreq,*pu4LowerFreq;
    INT16 i1Ret;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    pu4UpperFreq = (UINT32*)rArg.ai4Arg[0] ;//Upper boundary
    pu4LowerFreq = (UINT32*)rArg.ai4Arg[1] ;//Lower  boundary

    i1Ret=ITuner_OP(ITunerGetCtx(), itGetAnaFreqBound, 0,&Ana_Freq_boudary);
    if(i1Ret==ITUNER_NOT_SUPPORTED)
    {
        eRet=MTR_NOT_SUPPORTED;
    }
    else if(i1Ret==ITUNER_NOT_OK)
    {
        eRet=MTR_NOT_OK;
    }
    else//Ok
    {
        USR_SPACE_ACCESS_VALIDATE_ARG(pu4UpperFreq,UINT32);
        COPY_TO_USER_ARG(pu4UpperFreq,Ana_Freq_boudary.ui4_upper_bound_freq,UINT32);

        USR_SPACE_ACCESS_VALIDATE_ARG(pu4LowerFreq,UINT32);
        COPY_TO_USER_ARG(pu4LowerFreq,Ana_Freq_boudary.ui4_lower_bound_freq,UINT32);
    }

   return eRet;
   
}
MT_RESULT_T _MTFE_PAL_BypassSIF(unsigned long arg)
{
   UINT8 u1SubSysIdx=(UINT8)arg;
   printf("_MTFE_PAL_BypassSIF %d\r\n",u1SubSysIdx);
    if ((u1SubSysIdx < MOD_ANA_TYPE_BEGIN) || (u1SubSysIdx > MOD_TYPE_UNKNOWN))
    {
        printf("Invalid sub-sys!\n");
        return MTR_NOT_OK;
    }
   if(pMTPdCtx)
   {
    ATD_ChipInit(pMTPdCtx->pDemodCtx);
   ATD_SetSystem(pMTPdCtx->pDemodCtx,u1SubSysIdx);
   ATD_BypassSIF2Aud(pMTPdCtx->pDemodCtx,u1SubSysIdx);
   }
  else
   {
    MTFE_PAL_PRINT("Please call MTAL_PAL_Open first!\n");
    return MTR_NOT_OK;    
   }
return MTR_OK;   
}

/** Get Signal power level in percentage.
*@param pi2SigLevel         Signal power level in percentage.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_PAL_GetSignalLevel(unsigned long arg)
{

    INT32 *pi2SigLevel;
    INT32 i2SigLevel;
    pi2SigLevel = (INT32*)arg;

    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when access PI Primitive API, must use this lock for protection

    if ((ITuner_OP( ITunerGetCtx(), itGetSignalLevel,0, &i2SigLevel)) == ITUNER_OK)
    {
        MTFE_PAL_PRINT("_MTFE_PAL_GetSignalLevel (dBuV): %d\n", i2SigLevel);
        (i2SigLevel>70)?(i2SigLevel=70):(i2SigLevel=i2SigLevel);
        (i2SigLevel<20)?(i2SigLevel=20):(i2SigLevel=i2SigLevel);
        i2SigLevel = (i2SigLevel-20)*2;
        MTFE_PAL_PRINT("_MTFE_PAL_GetSignalLevel (0~100): %d\n", i2SigLevel);
    }
    else
    {
        i2SigLevel = 100;
        MTFE_PAL_PRINT("_MTFE_PAL_GetSignalLevel: %d\n", i2SigLevel);
    }
        
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
    USR_SPACE_ACCESS_VALIDATE_ARG(pi2SigLevel,i2SigLevel);
    COPY_TO_USER_ARG(pi2SigLevel,i2SigLevel,i2SigLevel);
    MTFE_DVBT_PRINT(" - i2SigLevel = %d\n",i2SigLevel);
    return MTR_OK;

}

int mtal_ioctl_mtfe_pal(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
       
    MT_RESULT_T eRet = MTR_OK;
    MTFE_PAL_PRINT("------------pal ioctl cmd-----------\r\n");
    if(u1ExclSemaInit==1){
        mcSEMA_LOCK_MTFE_S(Exclusive_DigiAnaSema,"Ex_DiAna");
    }
    switch (cmd)
    {
        case MTAL_IO_PAL_OPEN:
            eRet= _MTFE_PAL_Open();
            break;
        case MTAL_IO_PAL_CLOSE:
            eRet= _MTFE_PAL_Close();
            break;
        case MTAL_IO_PAL_CONNECT:
            eRet= _MTFE_PAL_Connect();
            break;
        case MTAL_IO_PAL_DISCONNECT:
            eRet= _MTFE_PAL_DisConnect();
            break;
        case MTAL_IO_PAL_TSF:
            eRet= _MTFE_PAL_TunerSetFreq( arg);
            break;
        case MTAL_IO_PAL_SSNF:
            eRet= _MTFE_PAL_StartSearchNearbyFreq( arg);
            break;
        case MTAL_IO_PAL_BSNF:
            eRet= _MTFE_PAL_BreakSearchNearbyFreq();
            break;
        case MTAL_IO_PAL_REGCB:
            eRet= _MTFE_PAL_RegCb( arg);
            break;
        case MTAL_IO_PAL_GETFREQBOUND:
            eRet= _MTFE_PAL_GetFreqBound(arg);
            break;
        case MTAL_IO_PAL_BypassSIF:
            eRet= _MTFE_PAL_BypassSIF(arg);
            break;
        case MTAL_IO_PAL_GETLOCKSTATUS:
            eRet=_MTFE_PAL_GetSync(arg);
            break;
        case MTAL_IO_PAL_GET_SYSTEM:
            eRet=_MTFE_PAL_GetSystem(arg);
            break;
        case MTAL_IO_PAL_GETSIGLEVEL:
            eRet=_MTFE_PAL_GetSignalLevel(arg);
            break;
        case MTAL_IO_PAL_SET_SYSTEM:
            eRet=_MTFE_PAL_SetSystem(arg);
            break;
        case MTAL_IO_PAL_SET_AFT:   /*wenming 110924*/
            eRet=_MTFE_PAL_SetAFT(arg);
            break;
        case MTAL_IO_PAL_AUTOSEARCH:
            eRet=_MTFE_PAL_AUTOSEARCH(arg);
            break;
        default:
            break;
    }
    if(u1ExclSemaInit==1){
        mcSEMA_UNLOCK_MTFE_S(Exclusive_DigiAnaSema, "Ex_DiAna");
    }
    return eRet;
}

