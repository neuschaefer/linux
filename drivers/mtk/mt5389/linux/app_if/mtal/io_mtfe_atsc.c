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
 * $RCSfile: io_mtfe_atsc.c,v $
 * $Revision: #2 $
 *---------------------------------------------------------------------------*/

/** @file io_mtfe_atsc.c
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mtfe_atsc.h"
#include "io_mtfe_pd_def.h"
#if defined (USE_ATD_IF_DEMOD) 
#include "tuner_interface_if.h"
#endif
#include "tuner_if.h"
#include "dtd_pd_glue_if.h"
#include "io_mtfe_atsc.h"
#include "dtd_if.h"

#include "cb_low.h"  // For callback function
#include <linux/mtal_ioctl.h>

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(int)))                            \
    {                                                       \
        printk("mtal_ioctl_atsc argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(int)))                        \
    {                                                       \
        printk("mtal_ioctl_atsc argument error\n");      \
        return -1;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_atsc argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_atsc argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_atsc argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_atsc argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        printk("mtal_ioctl_atsc argument error\n");     \
        return MTR_ERR_INV;                                          \
    }

#define COPY_TO_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_atsc argument error\n");      \
        return MTR_ERR_INV;                                          \
    }
#define COPY_FROM_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_from_user(&ArgKernel,(void __user *)ArgUsr,    \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_atsc argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

//-----------------------------------------------------------------------------
// Define definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Extern function
//-----------------------------------------------------------------------------
#if defined(CC_MT5365) || defined(CC_MT5363) || defined(CC_MT5396) 
extern void TunerSetMwIntf(UINT8 u1Value);
#endif
extern void * TunerGetCtx(UINT16 TunerID);
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define MTFE_ATSC_SW_VERSION         "1.430" /* Software Version: x.yyy */
#define MTAL_ATSC_MON_POLL_INTRVL             50  // 50ms: Basic resoultion for timers
#define MTAL_ATSC_MON_INFO_UPDATE_INTRVL    1000  // 1sec
#define cMTAL_ATSC_CHK_STATUS_INTRVL         200  // 200ms

#define MTAL_ATSC_MON_INFO_UPDATE_CNT       (MTAL_ATSC_MON_INFO_UPDATE_INTRVL/MTAL_ATSC_MON_POLL_INTRVL)
#define cMTAL_ATSC_CHK_STATUS_CNT           (cMTAL_ATSC_CHK_STATUS_INTRVL/MTAL_ATSC_MON_POLL_INTRVL)


#define cUNLOCK_CNT_THRD                     10    // The criteria for state transition from lock to unlock

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static MTAL_ATSC_PD_CTX_T           *pMTPdCtx;
static BOOL _fgATSCInit=FALSE;
/*all variable used in this file should put into pMTPdCtx except for Exclusive_DigiAnaSema and u1ExclSemaInit*/
static HANDLE_T    Exclusive_DigiAnaSema=0;
static UINT8       u1ExclSemaInit=0;
static MTFE_ATSC_PARAMETER_T    curParam;
static MTFE_ATSC_PARAMETER_T    getParam; 

extern MT_RESULT_T _MTFE_NTSC_DisConnect(VOID);
//-----------------------------------------------------------------------------
// Static Functions
//-----------------------------------------------------------------------------
static void _NotifyDemodStatus(INT16 LockStatus)
{
	MTAL_FE_US_TUNER_CALLBACK_INFO_T rCbInfo;
    UCHAR mod = 0xFF;
    INT32 status = MTR_NOT_SUPPORTED;

    MTFE_ATSC_PRINT("_NotifyDemodStatus: DTD LockStatus? %d, Freq = %d\n", LockStatus, pMTPdCtx->pDemodCtx->Frequency);

	rCbInfo.CbId = MTAL_FE_ATSC_LOCK_STATUS;

#if 1
    if (LockStatus < MTFE_DTV_UNKNOWN)
        rCbInfo.CbIdInfo.LockStatus = LockStatus;
    else // default state 
        rCbInfo.CbIdInfo.LockStatus = MTFE_DTV_UNKNOWN;    	
#else    
    switch(LockStatus){
    case MTFE_DTV_LOCK:
        rCbInfo.CbIdInfo.LockStatus = MTFE_DTV_LOCK;
        break;
    case MTFE_DTV_UNLOCK:
        rCbInfo.CbIdInfo.LockStatus = MTFE_DTV_UNLOCK;
        break;
    case MTFE_DTV_BREAK_BY_SECOND_CONNECT_EVENT:
        rCbInfo.CbIdInfo.LockStatus = MTFE_DTV_BREAK_BY_SECOND_CONNECT_EVENT;
        break;
    default:
        rCbInfo.CbIdInfo.LockStatus = MTFE_DTV_UNKNOWN;
        break;
    }
#endif    

    status = DTD_GetMod(pMTPdCtx->pDemodCtx, &mod);
    if (MTR_OK == status)
    {
        MTFE_ATSC_PRINT("_NotifyDemodStatus: DTD GetMod? %d\n", mod);
        switch (mod)
        {
        case MOD_ATSC_8VSB:
            rCbInfo.CbParam.dtvParams.mod = MTFE_MOD_ATSC_8VSB;
            break;
        case MOD_J83B_64QAM:
            rCbInfo.CbParam.dtvParams.mod = MTFE_MOD_J83B_64QAM;
            break;
        case MOD_J83B_256QAM:
            rCbInfo.CbParam.dtvParams.mod = MTFE_MOD_J83B_256QAM;
            break;
        default:
            rCbInfo.CbParam.dtvParams.mod = MTFE_MOD_TYPE_END;
            break;
        }
    }
    else
    {
        MTFE_ATSC_PRINT("_NotifyDemodStatus: DTD GetMod failed!\n");
        rCbInfo.CbParam.dtvParams.mod = MTFE_MOD_TYPE_END;
    }
    getParam.u1StatusInd = pMTPdCtx->pDemodCtx->u1StatusInd; //EC_code_dB809: Add enum for Demod failed ID
    getParam.rLockStatus = LockStatus; //EC_code_dB707: Never-lock indicator
    getParam.rModulation =  rCbInfo.CbParam.dtvParams.mod; 

    MTFE_ATSC_PRINT("ATSC _CB_PutEventLockStatus: LockStatus? %d\n", rCbInfo.CbIdInfo.LockStatus);
	_CB_PutEvent(CB_MTAL_FE_US_TUNER_TRIGGER, sizeof(MTAL_FE_US_TUNER_CALLBACK_INFO_T), &rCbInfo);
}

/**DTD monitor process.
*@param  pvArg               
*@retval VOID
*/
static VOID _AtscMonitorProcess(VOID)
{
    //  static INT16 i2PreStatus = 0;
    INT16 	i2LockStatus;
    UINT16  u2_i = 0;
    UINT16  u2_timer2 = 0;    
    //MTFE_DTV_STATUS_T rDemodStatus;
    UINT16                  u2MsgIdx;
    TUNER_CMD_QUEUE_T	    rCmdQ;
    SIZE_T 					zMsgSize = sizeof(UINT32);
    INT32 					i4Result;
    UINT16                  u2AtscLockTimeCnt = 0;
    UINT16                  u2UockCnt = 0;

    while (pMTPdCtx->fgMonitorThreadRunning)
    {    
        //todo: ask ben&Ray timeout value accuracy has problem( MSGQ_DELAY_TIMEOUT )
        i4Result = x_msg_q_receive_timeout(&u2MsgIdx, &rCmdQ, &zMsgSize, &pMTPdCtx->t_MoniCmdMsgQ, 1, MTAL_ATSC_MON_POLL_INTRVL);
        //MTFE_ATSC_PRINT("ATSC Monitor Get MsgQ=%d\n",i4Result);
        if( i4Result==OSR_OK && rCmdQ.CmdId == TUNER_CMD_CONNECT){
            MTFE_ATSC_PRINT("ATSC Monitor Get Cmd TUNER_CMD_CONNECT (MsgQ=%d)\n",i4Result);
            if(u2AtscLockTimeCnt!=0){ //last connect event not yet finish, and new connect event come
                _NotifyDemodStatus(MTFE_DTV_BREAK_BY_SECOND_CONNECT_EVENT);
                u2AtscLockTimeCnt = 0;//reset u2AtscLockTimeCnt for next new connect event
            }
            pMTPdCtx->fgAskMoniDoGetSync = TRUE;//keep do getSync till finish callback event
            pMTPdCtx->eState = MTFE_ATSC_CONNECTING_STATE; // Change to connecting state
        }

        if (pMTPdCtx->fgBreakMonitor) //condition for suspend monitor
        {
            pMTPdCtx->eState = MTFE_ATSC_IDLE_STATE; // Change to IDLE state        	
            MTFE_ATSC_PRINT("Entering io_mtfe_atsc mon_suspend\n");
            mcSEMA_LOCK_MTFE_S(pMTPdCtx->t_escape_mon_suspend,"atsc_mon_suspend");
    		MTFE_ATSC_PRINT("Escape io_mtfe_atsc mon_suspend\n");
        }
        //else if(pMTPdCtx->fgAskMoniDoGetSync){//condition for doing getSync, implement your get sync here
        else if(pMTPdCtx->eState == MTFE_ATSC_CONNECTING_STATE){ 
            mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when call PI primitive API must use this lock for protection
            u2AtscLockTimeCnt++;  
            DTD_UpdateInfo( pMTPdCtx->pDemodCtx);
            i2LockStatus = DTD_GetSync(pMTPdCtx->pDemodCtx);
            
            if(i2LockStatus == TRUE)//lock
            {
                MTFE_ATSC_PRINT("DTD lock found!\n");
                DTD_Connected(pMTPdCtx->pDemodCtx);
                _NotifyDemodStatus(MTFE_DTV_LOCK);
                pMTPdCtx->fgAskMoniDoGetSync =FALSE; //don't need to do get sync in next loop
                u2AtscLockTimeCnt = 0;//reset u2AtscLockTimeCnt for next new connect event
                pMTPdCtx->eState = MTFE_ATSC_LOCK_STATE;
            }
            /* Demod TimeOut found */
            if (DTD_CheckTimeout(pMTPdCtx->pDemodCtx))
            {
                MTFE_ATSC_PRINT("DTD Timeout found!\n");
                _NotifyDemodStatus(MTFE_DTV_EARLY_BREAK_UNLOCK);
                pMTPdCtx->fgAskMoniDoGetSync =FALSE; //don't need to do get sync in next loop
                u2AtscLockTimeCnt = 0;//reset u2AtscLockTimeCnt for next new connect event
                pMTPdCtx->eState = MTFE_ATSC_UNLOCK_STATE;
            }
            mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
        }
        else //implement get info or any other regular update job here, delay job is done by x_msg_q_receive_timeout[MTAL_ATSC_MON_POLL_INTRVL]
		{ /*monitor PI Monitor process*/
	         
	        /* Increase timer*/
			u2_i++;
			u2_timer2++;
        	//MTFE_ATSC_PRINT("MTAL ATSC MonitorProcess! keep update parameter u2_i=%d, u2_timer2=%d\n",u2_i, u2_timer2);	        	
	
			/* Accumulate PER */
			if (u2_i > MTAL_ATSC_MON_INFO_UPDATE_CNT) 
			{
            	u2_i = 0;
		  mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when call PI primitive API must use this lock for protection
		  DTD_UpdateInfo(pMTPdCtx->pDemodCtx); //EC_debug_DDI_dB516
                MTFE_ATSC_PRINT("DTD_UpdateInfo !\n");
                DTD_AccumPER(pMTPdCtx->pDemodCtx);
                MTFE_ATSC_PRINT("DTD_AccumPER !\n");
                DTD_GetTcmBER(pMTPdCtx->pDemodCtx, &(pMTPdCtx->u4TcmTotalBit), &(pMTPdCtx->u4u4TcmErrBit));
            	  MTFE_ATSC_PRINT("MTAL ATSC DTD_AccumPER Update!\n");            	           
		  mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
			}

          	/* Detect if there is any status transition */
			if (u2_timer2 > cMTAL_ATSC_CHK_STATUS_CNT)
			{
   			    u2_timer2 = 0;
				mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when call PI primitive API must use this lock for protection
				DTD_UpdateInfo(pMTPdCtx->pDemodCtx); //EC_debug_DDI_dB516
                i2LockStatus = DTD_GetSync(pMTPdCtx->pDemodCtx);
                MTFE_ATSC_PRINT("Polling i2LockStatus = %d\n", i2LockStatus);
                
                if (pMTPdCtx->eState == MTFE_ATSC_UNLOCK_STATE)
                {
                    if(i2LockStatus == TRUE) // LOCK
                    {
                        _NotifyDemodStatus(MTFE_DTV_LOCK);
                        pMTPdCtx->eState = MTFE_ATSC_LOCK_STATE;
                        MTFE_ATSC_PRINT("Change state from unlock to lock\n");
                    }
                }
                else // LOCK_STATE
                {
                	
                	if (i2LockStatus == FALSE)
                		u2UockCnt++;
                	else
                		u2UockCnt = 0;
                	
                    MTFE_ATSC_PRINT("u2UockCnt = %d\n", u2UockCnt);
                	if (u2UockCnt >= cUNLOCK_CNT_THRD)
                	{
                		u2UockCnt = 0;                		
                        _NotifyDemodStatus(MTFE_DTV_UNLOCK);
                        pMTPdCtx->eState = MTFE_ATSC_UNLOCK_STATE;
                        MTFE_ATSC_PRINT("Change state from lock to unlock\n");
                	}
                }
				mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);                				
			}
			
        }
        //don't do delay here, x_msg_q_receive() will do blocking delay
    }
    pMTPdCtx->t_monitor_thread = (HANDLE_T)NULL;
    MTFE_ATSC_PRINT("MTAL MonitorProcess exit!\n");
    x_thread_exit();

}

/**Start DTD monitor engine.
*@param  VOID               
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _StartMonitorEngine(VOID)
{  
    // x_thread_create(HANDLE_T *ph_th_hdl, CHAR *ps_name, SIZE_T z_stack_size,
    //                 UINT8 ui1_pri, x_os_thread_main_fct pf_main_rtn,
    //                  SIZE_T z_arg_size, VOID *pv_arg);
    // Important: x_thread_create will copy pv_arg to its memory while not 
    // directly use pv_arg. Therefore, if we want thread to share the same 
    // context as caller, we need set pv_arg as "pointer of context".
    pMTPdCtx->fgMonitorThreadRunning = TRUE;
    if (x_thread_create(&pMTPdCtx->t_monitor_thread,
            "_DtdMonThread",
            2048, 101,
            (x_os_thread_main_fct)_AtscMonitorProcess, sizeof(void*), NULL) != OSR_OK)
    {
        MTFE_ATSC_PRINT("_StartMonitorEngine (mcTHREAD_CREATE): ERROR!\n");
        return (MTR_NOT_OK);
    }

    return (MTR_OK);
}

/**Stop DTD monitor engine.
*@param  VOID               
*@retval MTR_OK              Success.
*@retval MTR_NOT_OK          Fail.
*/
static MT_RESULT_T _StopMonitorEngine(VOID)
{
    pMTPdCtx->fgMonitorThreadRunning = FALSE;
    MTFE_ATSC_PRINT("In kernal:_MTAL_StopMonitorEngine\n");

    while (pMTPdCtx->t_monitor_thread) {
        mcDELAY_MS(10); // in ms
    }
    MTFE_ATSC_PRINT("MTAL PD_StopMonitorEngine success\n");

    return (MTR_OK);
}




//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
/** Initialize front end ATSC demod module.
*@param VOID
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_ATSC_Open(VOID)
{
    INT32           i4Status;
    MT_RESULT_T     rRet;
    TD_DTD_CTX_T    *pDTDPdCtx = NULL;
    TUNER_CMD_QUEUE_T	rCmdQ;

    MTFE_ATSC_PRINT("In kernel %s\n", __FUNCTION__);
    
    //add driver implementation code here
    if(_fgATSCInit==TRUE){
        MTFE_ATSC_PRINT("_fgATSCInit==TRUE, do _MTFE_ATSC_Close() first\r\n");
        return (MTR_NOT_OK);
    }
    /* TunerOpen */
#if defined(CC_MT5365) || defined(CC_MT5363) || defined(CC_MT5396) 
	TunerSetMwIntf(cMTAL_IF);//Set MW type to avoid glue layer to create monitor thread again
#endif
    i4Status = TunerOpen();
    
    if (i4Status < DRVAPI_TUNER_OK)
    {   
        MTFE_ATSC_PRINT("TunerOpen failed\n");
        return (MTR_NOT_OK);
    }
    else if (i4Status > DRVAPI_TUNER_OK)
    {
        MTFE_ATSC_PRINT("TunerOpen has been called\n");        
    }
    else{
        MTFE_ATSC_PRINT("TunerOpen ok\n");        
    }

    pMTPdCtx = (MTAL_ATSC_PD_CTX_T *) x_mem_calloc(1, sizeof(MTAL_ATSC_PD_CTX_T));
    if (pMTPdCtx == NULL)
    {
        MTFE_ATSC_PRINT("MTAL_ATSC_PD_CTX_T malloc fail\n");
        return (MTR_NOT_OK);
    }

    //get atsc pd Ctx from nucleus pd glue
    pDTDPdCtx = (TD_DTD_CTX_T*)TunerGetCtx(DIGITAL_FE);
    if(pDTDPdCtx != NULL)
    {
        pMTPdCtx->pDemodCtx = pDTDPdCtx->psDemodInbInfo;
        pMTPdCtx->hHalLock = pDTDPdCtx->hHalLock; 
    }

    if (x_sema_create(&Exclusive_DigiAnaSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        MTFE_ATSC_PRINT("%s: ATSC x_sema_create(Exclusive_DigiAnaSema) Fail!\n");
        return (MTR_NOT_OK);
    }
    x_sema_unlock(Exclusive_DigiAnaSema);
    u1ExclSemaInit = 1;

    if (x_sema_create(&pMTPdCtx->t_escape_mon_suspend, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        MTFE_ATSC_PRINT("%s: ATSC x_sema_create(pMTPdCtx->t_escape_mon_suspend) Fail!\n");
        return (MTR_NOT_OK);
    }
    x_sema_unlock(pMTPdCtx->t_escape_mon_suspend);

    if ((x_msg_q_create(&pMTPdCtx->t_MoniCmdMsgQ, "ATSC_Moni_MsgQ", sizeof(rCmdQ), TUNER_CMD_QUEUE_SIZE)) != OSR_OK)
    {
        MTFE_ATSC_PRINT("%s: ATSC  x_msg_q_create(pMTPdCtx->t_escape_mon_suspend) Fail!\n");
        return (MTR_NOT_OK);
    }
    pMTPdCtx->fgAskMoniDoGetSync = FALSE; //init value
    pMTPdCtx->fgBreakMonitor = TRUE; //initial monitor is suspend
    pMTPdCtx->eState = MTFE_ATSC_IDLE_STATE; // Init state
    
    /* If tuner open success, start monitor engine */
    rRet = _StartMonitorEngine();
    if(rRet != MTR_OK)
    {
        MTFE_ATSC_PRINT("ATSC _StartMonitorEngine failed\n");
    	return (MTR_NOT_OK);
    }


    _fgATSCInit = TRUE;
    return MTR_OK;
}
/** Terminate front end ATSC demod module.
*@param VOID
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ATSC_Close(VOID)
{
//INT32 i4Status;
MT_RESULT_T rRet;

    MTFE_ATSC_PRINT("In kernel %s\n", __FUNCTION__);
    
    //add driver implementation code here
    if(_fgATSCInit!=TRUE){
        MTFE_ATSC_PRINT("_MTFE_ATSC_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }
 	if(pMTPdCtx->fgBreakMonitor == TRUE){
    	pMTPdCtx->fgBreakMonitor = FALSE; //resume monitor 
		mcSEMA_UNLOCK_MTFE_S(pMTPdCtx->t_escape_mon_suspend,"atsc_mon_suspend");
	}
    /* If tuner close success, stop monitor engine */
    rRet = _StopMonitorEngine();
    if(rRet != MTR_OK)
    {
        MTFE_ATSC_PRINT("ATSC _StopMonitorEngine failed\n");
        return (MTR_NOT_OK);    	
    }

    x_sema_unlock(Exclusive_DigiAnaSema);
    x_sema_delete(Exclusive_DigiAnaSema);

    x_sema_unlock(pMTPdCtx->t_escape_mon_suspend);
    x_sema_delete(pMTPdCtx->t_escape_mon_suspend);

    x_msg_q_delete(pMTPdCtx->t_MoniCmdMsgQ);
    //add driver implementation code here    
    x_mem_free(pMTPdCtx); //free MTAL glue Ctx
        
    /* TunerClose */
    //i4Status = TunerClose(); //don't do LTDIS TunerClose() from MTAL    
    _fgATSCInit = FALSE;

    return MTR_OK;
}
/** Start demod module to acquire to specified frequency and modulation type.
*@param  rParameter         Acquisition parameter (RF freq , modulation type,..).
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ATSC_Connect(unsigned long arg)
{
    MTFE_ATSC_PARAMETER_T *prParameter;
    MTFE_ATSC_PARAMETER_T rParameter;
    UINT8 u1Mod;
    UINT8 u1ConnType;
    TUNER_CMD_QUEUE_T	rCmdQ;
    MT_RESULT_T fgRet = MTR_OK;

    prParameter = (MTFE_ATSC_PARAMETER_T*)arg;
#ifndef CC_USE_DDI /* DDI complier option to separate the branch with enable internal ATD */
    _MTFE_NTSC_DisConnect();
#endif 
    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ATSC_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_ATSC_PARAMETER_T);

    MTFE_ATSC_PRINT("In kernel %s - rParameter.u4Freq = %d\n"
        "rParameter.rModulation = %d\n"
        "rParameter.rRfMode = %d\n"
        "rParameter.rSurfMode = %d\n",
        __FUNCTION__,
        rParameter.u4Freq,
        rParameter.rModulation,
        rParameter.rRfMode,
        rParameter.rSurfMode);

    memcpy(&curParam, &rParameter, sizeof(MTFE_ATSC_PARAMETER_T));

    pMTPdCtx->fgBreakMonitor = FALSE; //resume monitor to do getSync
    mcSEMA_UNLOCK_MTFE_S(pMTPdCtx->t_escape_mon_suspend,"atsc_mon_suspend");

    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
    //add driver implementation code here    
    /* Moudulation */
    u1Mod = MOD_ATSC_8VSB;   // Default setting

    if (rParameter.rModulation == MTFE_MOD_ATSC_8VSB)
        u1Mod = MOD_ATSC_8VSB;
    else if (rParameter.rModulation == MTFE_MOD_J83B_64QAM)
        u1Mod = MOD_J83B_64QAM;
    else if (rParameter.rModulation == MTFE_MOD_J83B_256QAM)
        u1Mod = MOD_J83B_256QAM;
    else if (rParameter.rModulation == MTFE_MOD_J83B_AUTO_QAM)
        u1Mod = MOD_J83B_AUTO_QAM;
    else
    {
       MTFE_ATSC_PRINT("Invalid ATSC modulation type in _MTFE_ATSC_Connect()\n");
    }

    /* Surf mode*/
    if (rParameter.rSurfMode == MTFE_SURF_CHANCHANGE)    
       u1ConnType = DEMOD_CONN_TYPE_SURF;
    else
       u1ConnType = DEMOD_CONN_TYPE_SCAN;
     
	//ask monitor to do getSync
    /*preapre for send event to monitor thread*/
    rCmdQ.CmdId = TUNER_CMD_CONNECT;
    rCmdQ.CmdInfo.ConnectParam.u4Freq =rParameter.u4Freq;
    rCmdQ.CmdInfo.ConnectParam.rModulation = rParameter.rModulation;
    rCmdQ.CmdInfo.ConnectParam.rRfMode =rParameter.rRfMode;
    rCmdQ.CmdInfo.ConnectParam.rSurfMode = rParameter.rSurfMode;

    /* Connect */
    //if (pMTPdCtx->eState != MTFE_ATSC_CONNECTING_STATE) /* EC_code_dB721: remove it due to block lockstatus updating */
    {
	    getParam.rLockStatus = MTFE_DTV_UNKNOWN; /* EC_code_dB711: correct typo and set ATSC/QAM default state as "MTFE_DTV_UNKNOWN" */
	    DTD_Connect(pMTPdCtx->pDemodCtx, rParameter.u4Freq, u1Mod, u1ConnType);
	    //DTD_SetIF(pMTPdCtx->pDemodCtx, pMTPdCtx->pDemodCtx->sTunerCtx->u2IF_Freq); /* remove it into "DTD_Connect" in pi_demod.c */
    }
    if ((x_msg_q_send(pMTPdCtx->t_MoniCmdMsgQ, &rCmdQ, sizeof(TUNER_CMD_QUEUE_T), 1)) != OSR_OK){
        MTFE_ATSC_PRINT("Error: _MTFE_ATSC_Connect() x_msg_q_send failed\n");
        fgRet = MTR_NOT_OK;
    }
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);

    return fgRet;
}
/**Stop acquisition and put Demod into sleep mode.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

//static MT_RESULT_T _MTFE_ATSC_DisConnect(VOID)
MT_RESULT_T _MTFE_ATSC_DisConnect(VOID)
{
    MTFE_ATSC_PRINT("In kernel %s\n", __FUNCTION__);
    if(_fgATSCInit!=TRUE){
        return MTR_OK;
    }
    //add driver implementation code here    
    pMTPdCtx->fgBreakMonitor = TRUE; //suspend monitor

    /* Diconnect */
    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
    DTD_DisConnect(pMTPdCtx->pDemodCtx);
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
    
    return MTR_OK;
}

/** Get demod lock status.
*@param	 prStatus           Report demod status.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_ATSC_GetSync(unsigned long arg)
{
    //INT16 *pi2LockStatus;
    //INT16 i2LockStatus;
    //pi2LockStatus = (INT16*)arg;
    MTFE_ATSC_PARAMETER_T *prStatus;
    MTFE_ATSC_PARAMETER_T rStatus;
    prStatus = (MTFE_ATSC_PARAMETER_T*)arg;
   
    MTFE_ATSC_PRINT("In kernel %s\n", __FUNCTION__);
    
   //add driver implementation code here
    /* Get Sync */
    //mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
    //DTD_UpdateInfo( pMTPdCtx->pDemodCtx); //EC_debug_DDI
    ///i2LockStatus = DTD_GetSync(pMTPdCtx->pDemodCtx);
    //mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
    //rStatus.rLockStatus = MTFE_DTV_UNLOCK;
    /* Write into content */
    //if (i2LockStatus)
    
    rStatus.rLockStatus = getParam.rLockStatus; //update ATSC/QAM demod LOCK status;        
    rStatus.rModulation =  getParam.rModulation; //pMTPdCtx->pDemodCtx->Modulation;
    rStatus.u1StatusInd = getParam.u1StatusInd; //EC_code_dB809: Add enum for Demod failed ID 
    rStatus.u4Freq = pMTPdCtx->pDemodCtx->Frequency;
    rStatus.rRfMode = curParam.rRfMode;
    rStatus.rSurfMode = curParam.rSurfMode;

    USR_SPACE_ACCESS_VALIDATE_ARG(prStatus, rStatus);
    COPY_TO_USER_ARG(prStatus, rStatus, MTFE_ATSC_PARAMETER_T);
    MTFE_ATSC_PRINT(" - LockStatus = %d, Mod = %d, u4Freq = %d\n", 
         rStatus.rLockStatus, rStatus.rModulation, rStatus.u4Freq);

    return MTR_OK;

}

/** Get Signal power level in dBm.
*@param pi2SigLeveldBm      Signal power level in dBm.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ATSC_GetSignalLeveldBm(unsigned long arg)
{
    INT16 *pi2SigLeveldBm;
    INT16 i2SigLeveldBm;
    pi2SigLeveldBm = (INT16*)arg;

    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
    //DTD_UpdateInfo( pMTPdCtx->pDemodCtx); //EC_debug_DDI
    DTD_GetSync(pMTPdCtx->pDemodCtx);    
    i2SigLeveldBm = DTD_GetSignalLeveldBm(pMTPdCtx->pDemodCtx);
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
   
    USR_SPACE_ACCESS_VALIDATE_ARG(pi2SigLeveldBm,INT16);
    COPY_TO_USER_ARG(pi2SigLeveldBm,i2SigLeveldBm,INT16);
    MTFE_ATSC_PRINT(" - i2SigLeveldBm = %d\n",i2SigLeveldBm);
    return MTR_OK;
}
/**Get packet error rate.
*@param pu4TotalPkt         Received total packets.
*@param pu4ErrPkt           Received error packets.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ATSC_GetPER(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT32 *pu4TotalPkt;
    UINT32 *pu4ErrPkt;
    UINT32 u4TotalPkt;
    UINT32 u4ErrPkt;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    pu4TotalPkt = (UINT32*)rArg.ai4Arg[0];
    pu4ErrPkt = (UINT32*)rArg.ai4Arg[1];

    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);	
    DTD_Get_RevPkt(pMTPdCtx->pDemodCtx, &u4TotalPkt, &u4ErrPkt);
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4TotalPkt,UINT32);
    COPY_TO_USER_ARG(pu4TotalPkt,u4TotalPkt,UINT32);
    USR_SPACE_ACCESS_VALIDATE_ARG(pu4ErrPkt,UINT32);
    COPY_TO_USER_ARG(pu4ErrPkt,u4ErrPkt ,UINT32);


    MTFE_ATSC_PRINT(" - u4TotalPkt = %d, u4ErrPkt = %d\n",
        u4TotalPkt,
        u4ErrPkt);
    return MTR_OK;
}
/**Get SNR in dB.
*@param u2SignalSNR         Signal SNR
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ATSC_GetSNR(unsigned long arg)
{
    UINT16 *pu2SignalSNR;
    UINT16 u2SignalSNR = 0;
    pu2SignalSNR = (UINT16*)arg;

    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
    DTD_GetSync(pMTPdCtx->pDemodCtx);
    u2SignalSNR = DTD_GetSignalSNR(pMTPdCtx->pDemodCtx);
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);    
    
    USR_SPACE_ACCESS_VALIDATE_ARG(pu2SignalSNR,UINT16);
    COPY_TO_USER_ARG(pu2SignalSNR,u2SignalSNR,UINT16);
    MTFE_ATSC_PRINT(" - u2SignalSNR = %d\n",u2SignalSNR);
    return MTR_OK;
}
/**Get IF AGC Voltage in mV.
*@param  pu2IfAgcVol        IF AGC voltage in mV.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ATSC_GetIfAgcVol(unsigned long arg)
{
    UINT16 *pu2IfAgcVol;
    UINT16 u2IfAgcVol = 0;
    pu2IfAgcVol = (UINT16*)arg;

    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
    //DTD_UpdateInfo( pMTPdCtx->pDemodCtx); //EC_debug_DDI
    DTD_GetIfAgcVol(pMTPdCtx->pDemodCtx, &u2IfAgcVol);
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
    
    USR_SPACE_ACCESS_VALIDATE_ARG(pu2IfAgcVol,UINT16);
    COPY_TO_USER_ARG(pu2IfAgcVol,u2IfAgcVol,UINT16);
    MTFE_ATSC_PRINT(" - u2IfAgcVol = %d\n",u2IfAgcVol);
    return MTR_OK;
}

/**Get TCM BER.
*@param  pu4TotalBit        Received total bits.
*@param  pu4ErrBit          Received error bits.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_ATSC_GetTcmBER(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT32 *pu4TotalPkt;
    UINT32 *pu4ErrSym;
    UINT32 u4TotalPkt;
    UINT32 u4ErrSym;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    pu4TotalPkt = (UINT32*)rArg.ai4Arg[0];
    pu4ErrSym = (UINT32*)rArg.ai4Arg[1];

    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
    //DTD_GetTcmBER(pMTPdCtx->pDemodCtx, &u4TotalPkt, &u4ErrSym);
    u4TotalPkt = pMTPdCtx->u4TcmTotalBit;
    u4ErrSym = pMTPdCtx->u4u4TcmErrBit;
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4TotalPkt,UINT32);
    COPY_TO_USER_ARG(pu4TotalPkt,u4TotalPkt,UINT32);
    USR_SPACE_ACCESS_VALIDATE_ARG(pu4ErrSym,UINT32);
    COPY_TO_USER_ARG(pu4ErrSym,u4ErrSym,UINT32);

    MTFE_ATSC_PRINT(" - u4TotalBit = %d, u4ErrBit = %d\n",
        u4TotalPkt,
        u4ErrSym);
    return MTR_OK;
}
/**Set demod start/stop (fw) 
*@param fgStart		1(demod fw start acq) / 0(demod fw stop acq, back to FAT_Rst and wait)
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_ATSC_DTD_Acquire(unsigned long arg)
{
    BOOL fgStart;
    fgStart = (BOOL)arg;
    
    MTFE_ATSC_PRINT(" DTD_AcqState: %01d", fgStart);
    DTD_Acquire(pMTPdCtx->pDemodCtx, fgStart);
    return MTR_OK;    
}

/**Set spectrum polarity (normal or inverse)
*@param	 fgDebug            1 (Manual. For debug) / 0 (Auto. Normal flow use).
*@param	 rSpcPolarity       Spectrum polarity.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ATSC_SetSpcPolarity(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTFE_SPEC_T rSpcPolarity;
    UINT8 u1SpecInv;
    BOOL fgDebug;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    fgDebug = (BOOL)rArg.ai4Arg[0];
    rSpcPolarity = (MTFE_SPEC_T)rArg.ai4Arg[1];
    
    MTFE_ATSC_PRINT(" - fgDebug = %d, rSpcPolarity = %d\n", fgDebug, rSpcPolarity);
    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
    if (rSpcPolarity == MTFE_SPEC_NORM)
        u1SpecInv = 0;
    else
        u1SpecInv = 1;
    DTD_SetSpcPolarity(pMTPdCtx->pDemodCtx, fgDebug, u1SpecInv);
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);

    return MTR_OK;
}
/**Set transport stream directly out
*@param	 fgOn               ON (directly out for debug) / OFF (normal).
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ATSC_SetTSDirectOut(unsigned long arg)
{
    BOOL fgOn = (BOOL)arg;
    MTFE_ATSC_PRINT(" - fgOn = %d\n", fgOn);
    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
    DTD_SetTSDirectOut(pMTPdCtx->pDemodCtx, fgOn);
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);

    return MTR_OK;
}
/** Check demod software version.
*@param  pszVersion         Software Version.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ATSC_GetVer(unsigned long arg)
{
    UCHAR *pszVersion;
  //  UCHAR  aszVersion[] = MTFE_ATSC_SW_VERSION; /* Software Version: x.yyy */
    UCHAR  aszVersion[6];
    
    pszVersion = (UINT8*)arg;   
    
    vGetAsicVer(pMTPdCtx->pDemodCtx);
    
    aszVersion[0] = (pMTPdCtx->pDemodCtx->ucFwVer[0]);
    aszVersion[1] = (pMTPdCtx->pDemodCtx->ucFwVer[1]);
    aszVersion[2] = (pMTPdCtx->pDemodCtx->ucFwVer[2]);    
    
    USR_SPACE_ACCESS_VALIDATE_ARG(pszVersion,aszVersion);
    COPY_TO_USER_ARG(pszVersion,aszVersion,aszVersion);
    MTFE_ATSC_PRINT(" - ATSC Ver: %c%02X%02X\n", aszVersion[0], aszVersion[1], aszVersion[2]);

    return MTR_OK;
}

/** Set IF Frequency of demod.
*@param  u4IfFreq          The value of IF frequency and its unit is KHz.
*@retval MT_OK              Success.
*@retval MT_NOT_OK       Fail.
*/
static MT_RESULT_T _MTFE_ATSC_SetIFFreq(unsigned long arg)
{
    UINT32 u4IfFreq = (UINT32)arg;
    MTFE_ATSC_PRINT(" - u4IfFreq = %d\n", u4IfFreq);
    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
    // Set the IF of the Demod
    DTD_SetIF(pMTPdCtx->pDemodCtx, (UINT16)u4IfFreq);
    if(u4IfFreq < 25000)
    {   // Low IF, Spec Inverse 
        DTD_SetSpcPolarity(pMTPdCtx->pDemodCtx, FALSE, TRUE);
    }
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
    return MTR_OK;
}
/** Setup tuner confiuration to demod.
*@param  pTunerConfig       Config of Tuner
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_ATSC_Tuner_Config(unsigned long arg) 
{
    MTFE_ATSC_TUNER_CONFIG_T TunerConfig;    
    ITUNER_CTX_T* psITunerCtx = pMTPdCtx->pDemodCtx->sTunerCtx;
    SPECIFIC_MEMBER_US_CTX *psTunerCtx=NULL;
    MT_RESULT_T fgRet = MTR_OK;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, MTFE_ATSC_TUNER_CONFIG_T);
    COPY_FROM_USER_ARG(arg,TunerConfig, MTFE_ATSC_TUNER_CONFIG_T);

    if(pMTPdCtx)
    {        
        mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
        
        psTunerCtx=&(pMTPdCtx->pDemodCtx->sTunerCtx->specific_member.us_ctx);
        /* setup config/flow to demod */
        //psTunerCtx->xxx = TunerConfig.xxx;

        //others...

        psTunerCtx->m_aucPara[TUNER_FUNC_EN_TARGET_LEVEL] = TunerConfig.u1IFEN_TARGET_LEVEL;            
        psTunerCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_MSB] = TunerConfig.u1IFTARGETLevel_MSB;
        psTunerCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_LSB] = TunerConfig.u1IFTARGETLevel_LSB;
        psTunerCtx->m_aucPara_SLD[2] = TunerConfig.u1IFAGC_SLP;
        psTunerCtx->m_aucPara_SLD[3] = TunerConfig.u1IFAGC_INT;
        psTunerCtx->m_aucPara_SLD[4] = TunerConfig.u1IFAGC_MAX;
        psTunerCtx->m_aucPara_SLD[5] = TunerConfig.u1IFAGC_MIN;

        //psTunerCtx->m_ifAgc_lvl_max = TunerConfig.u1IFAGCLevelMax;
        //psTunerCtx->m_ifAgc_lvl_min = TunerConfig.u1IFAGCLevelMin;

        psITunerCtx->u2IF_Freq = TunerConfig.u2IF_Freq; // iven enable IF parameter for FE setting,20110509
        psITunerCtx->bSpecPolarity = TunerConfig.bSpecPolarity; /*set ATSC/QAM demodulator spectrum polarity setting */
        psITunerCtx->uSLD_DefaultPGAIndex = TunerConfig.uSLD_DefaultPGAIndex; /* set default PGA gain index for SLD module */
    
        DTD_SetTunerPara(pMTPdCtx->pDemodCtx);
        //DTD_Connect(pMTPdCtx->pDemodCtx, rParameter.u4Freq, u1Mod, u1ConnType);
        //DTD_SetIF(pMTPdCtx->pDemodCtx, pMTPdCtx->pDemodCtx->sTunerCtx->u2IF_Freq); /* move to Pi_demod.c */
         
        mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
        return fgRet;
    }

    return MTR_NOT_OK;
}

/**Get frequency offset.
*@param  arg             ---.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_ATSC_GetFreqOffset(unsigned long arg)
{
    INT32   ai4Data[cAGC_INFO_SIZE];
    BOOL    fgIsCFO;
    INT32 *pu2FreqOffset;
    INT32 u2FreqOffset = 0;
    pu2FreqOffset = (INT32*)arg;

    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
    DTD_Get_AGCGain_CFO(pMTPdCtx->pDemodCtx, ai4Data, &fgIsCFO); /* get CFO value from ATSC demod by host command and unit is Hz */
    u2FreqOffset = ai4Data[cVCR_CFO]/1000;  /* return (KHz) back */
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu2FreqOffset, INT32);
    COPY_TO_USER_ARG(pu2FreqOffset, u2FreqOffset, INT32);
    MTFE_ATSC_PRINT(" - u2FreqOffset = %d kHz\n", u2FreqOffset);

    return MTR_OK;
}



/** To control digital demod TunerBypass function switch ON or OFF
*@param  bSwitchOn       TRUE(TunerBypass ON) or FALSE(TunerBypass OFF)
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_ATSC_DigTunerBypassI2C(unsigned long arg)
{
    BOOL bSwitchOn = (BOOL)arg;
    MTFE_ATSC_PRINT(" - bSwitchOn = %d\n", bSwitchOn);

    //add driver implementation code here
    if(pMTPdCtx)
    { 
        mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
		
    #ifdef CC_FEDTD  // Extern Demod
        DTD_TunerI2c(pMTPdCtx->pDemodCtx, bSwitchOn);
    #endif

        mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);

        return MTR_OK;
    }
	
    return MTR_NOT_OK;
}

/**Register demod callback function.
*@param  CbId               Callback function ID.
*@param  pfCb               Callback function.
*@param  vArg               The argument of callback function.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_ATSC_RegCb(unsigned long arg)
{
	#if 0
    MTAL_IOCTL_3ARG_T rArg;
    UINT8 u1CbId;
    MTFE_ATSC_PFN_CB pfCb;
    VOID* pvArg;
    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1CbId = (UINT8)rArg.ai4Arg[0];
    pfCb = (MTFE_ATSC_PFN_CB)rArg.ai4Arg[1];
    pvArg = (VOID*)rArg.ai4Arg[2];

    MTFE_ATSC_PRINT(" - u1CbId = %d\n"
		"pfCb = 0x%x"
		"pvArg = 0x%x",
		u1CbId,
		(int)pfCb,
		(int)pvArg);  
	
    //add driver implementation code here
    switch (u1CbId)
    {
        case MTFE_ATSC_CB_ID_ISLOCK_NFY:
            _rDTDNotify[MTFE_ATSC_CB_ID_ISLOCK_NFY].pfnNfy = pfCb;
            _rDTDNotify[MTFE_ATSC_CB_ID_ISLOCK_NFY].pvArg =  pvArg;            
            _rDTDNotify[MTFE_ATSC_CB_ID_ISLOCK_NFY].u1CbId =  u1CbId;
        	break;
        default:
        	MTFE_ATSC_PRINT("Invalid callback ID(%d)\n", u1CbId);
        	break;
    }
    #endif
    return MTR_OK;
}


/** For NTSC to get digital/analog exclusive semaphore.
*@param  *sema              pointer to a sema
*@retval void
*/
void Get_MTFE_ATSC_Sema_Ex_DiAna(HANDLE_T *sema)
{
    *sema = Exclusive_DigiAnaSema;
}

int mtal_ioctl_mtfe_atsc(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
    MT_RESULT_T eRet = MTR_OK;
    MTFE_ATSC_PRINT("-------atsc ioctl cmd-------\r\n");
    if(u1ExclSemaInit==1)
    {
        mcSEMA_LOCK_MTFE_S(Exclusive_DigiAnaSema,"Ex_DiAna");
    }
    switch (cmd)
    {
    case MTAL_IO_ATSC_OPEN:
        eRet=_MTFE_ATSC_Open();
        break;
    case MTAL_IO_ATSC_CLOSE:
        eRet=_MTFE_ATSC_Close();
        break;
    case MTAL_IO_ATSC_CONNECT:
        eRet=_MTFE_ATSC_Connect(arg);
        break;
    case MTAL_IO_ATSC_DISCONNECT:
        eRet=_MTFE_ATSC_DisConnect();
        break;
    case MTAL_IO_ATSC_GETSYNC:
        eRet=_MTFE_ATSC_GetSync(arg);
        break;
    case MTAL_IO_ATSC_GETSIGLEVEL:
        eRet=_MTFE_ATSC_GetSignalLeveldBm( arg);
        break;
    case MTAL_IO_ATSC_GETPER:
        eRet=_MTFE_ATSC_GetPER( arg);
        break;
    case MTAL_IO_ATSC_GETSNR:
        eRet=_MTFE_ATSC_GetSNR( arg);
        break;
    case MTAL_IO_ATSC_GETIFAGC:
        eRet=_MTFE_ATSC_GetIfAgcVol( arg);
        break;
    case MTAL_IO_ATSC_GETTCMBER:
        eRet=_MTFE_ATSC_GetTcmBER(arg);
        break;
    case MTAL_IO_ATSC_SETSPCPOLAR:
        eRet=_MTFE_ATSC_SetSpcPolarity( arg);
        break;
    case MTAL_IO_ATSC_SETTSDIROUT:
        eRet=_MTFE_ATSC_SetTSDirectOut( arg);
        break;
    case MTAL_IO_ATSC_GETVER:
        eRet=_MTFE_ATSC_GetVer( arg);
        break;
    case MTAL_IO_ATSC_REGCB:
        eRet=_MTFE_ATSC_RegCb( arg);
        break;
    case MTAL_IO_ATSC_SETIFFREQ:
        eRet=_MTFE_ATSC_SetIFFreq( arg);
        break;
    case MTAL_IO_ATSC_TUNER_CONFIG:
        eRet=_MTFE_ATSC_Tuner_Config( arg);
        break;
    case MTAL_IO_ATSC_GETFREQOFFSET:
            eRet=_MTFE_ATSC_GetFreqOffset( arg);
            break;
    case MTAL_IO_ATSC_DIGTUNERBYPASSI2C:
        eRet=_MTFE_ATSC_DigTunerBypassI2C( arg);
        break;
    case MTAL_IO_ATSC_DTD_Acquire:
    	eRet=_MTFE_ATSC_DTD_Acquire( arg);
    	break;
    default:
        break;
    }

    if(u1ExclSemaInit==1)
    {
        mcSEMA_UNLOCK_MTFE_S(Exclusive_DigiAnaSema, "Ex_DiAna");
    }
	return eRet;
}
