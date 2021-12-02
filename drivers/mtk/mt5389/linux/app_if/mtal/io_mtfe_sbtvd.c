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
 * $RCSfile: io_mtfe_SBTVD.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file io_mtfe_SBTVD.c
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mtfe_sbtvd.h"
#include "io_mtfe_pd_def.h"
//#if defined (USE_ATD_IF_DEMOD) 
#include "tuner_interface_if.h"
//#endif
#include "tuner_if.h"
//#include "dtd_pd_glue_if.h"
#include "io_mtfe_sbtvd.h"
//#include "dtd_if.h"
#include "pi_isdbt_api.h"
#include "pd_isdbt_glue.h"

#include "cb_low.h"  // For callback function
#include <linux/mtal_ioctl.h>

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(int)))                            \
    {                                                       \
        printk("mtal_ioctl_SBTVD argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(int)))                        \
    {                                                       \
        printk("mtal_ioctl_SBTVD argument error\n");      \
        return -1;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_SBTVD argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_SBTVD argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_SBTVD argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_SBTVD argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        printk("mtal_ioctl_SBTVD argument error\n");     \
        return MTR_ERR_INV;                                          \
    }

#define COPY_TO_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_SBTVD argument error\n");      \
        return MTR_ERR_INV;                                          \
    }
#define COPY_FROM_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_from_user(&ArgKernel,(void __user *)ArgUsr,    \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_SBTVD argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

//-----------------------------------------------------------------------------
// Define definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Extern function
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define MTAL_SBTVD_MON_POLL_INTRVL             50  // 50ms: Basic resoultion for timers
#define MTAL_SBTVD_MON_INFO_UPDATE_INTRVL    1000  // 1sec
#define cMTAL_SBTVD_CHK_STATUS_INTRVL         200  // 200ms

#define MTAL_SBTVD_MON_INFO_UPDATE_CNT       (MTAL_SBTVD_MON_INFO_UPDATE_INTRVL/MTAL_SBTVD_MON_POLL_INTRVL)
#define cMTAL_SBTVD_CHK_STATUS_CNT           (cMTAL_SBTVD_CHK_STATUS_INTRVL/MTAL_SBTVD_MON_POLL_INTRVL)


#define cUNLOCK_CNT_THRD                     10    // The criteria for state transition from lock to unlock

#define cWAIT_DEMOD_LOCK_CNT                   20    //wait 20*50ms

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
extern void TunerSetMwIntf(UINT8 u1Value);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static MTAL_SBTVD_PD_CTX_T           *pMTPdCtx;
static BOOL _fgSBTVDInit=FALSE;
/*all variable used in this file should put into pMTPdCtx except for Exclusive_DigiAnaSema and u1ExclSemaInit*/
static HANDLE_T    Exclusive_DigiAnaSema=0;
static UINT8       u1ExclSemaInit=0;
static MTFE_SBTVD_PARAMETER_T    curParam;

extern MT_RESULT_T _MTFE_NTSC_DisConnect(VOID);
extern void * TunerGetCtx(UINT16 TunerID);
//-----------------------------------------------------------------------------
// Static Functions
//-----------------------------------------------------------------------------
static void _NotifyEWSStatus(UINT8 EwsStatus)
{
  MTAL_FE_BZ_TUNER_CALLBACK_INFO_T rCbInfo;
  
  rCbInfo.CbId=MTAL_FE_SBTVD_EWS_STATUS;
  
  MTFE_SBTVD_PRINT("_NotifyEWSStatus: %d\n", EwsStatus);
  
  if(EwsStatus)//Ews Start
  {
    rCbInfo.CbIdInfo.EwsEvent=MTAL_FE_SBTVD_EWS_START;
  }
  else
  {
    rCbInfo.CbIdInfo.EwsEvent=MTAL_FE_SBTVD_EWS_END;
  }
  _CB_PutEvent(CB_MTAL_FE_SBTVD_TUNER_TRIGGER, sizeof(MTAL_FE_BZ_TUNER_CALLBACK_INFO_T), &rCbInfo);
}
static void _NotifyDemodStatus(INT16 LockStatus)
{
	MTAL_FE_BZ_TUNER_CALLBACK_INFO_T rCbInfo;

    MTFE_SBTVD_PRINT("_NotifyDemodStatus: DTD LockStatus? %d\n", LockStatus);

	rCbInfo.CbId = MTAL_FE_SBTVD_LOCK_STATUS;

    if (LockStatus < MTFE_DTV_UNKNOWN)
        rCbInfo.CbIdInfo.LockStatus = LockStatus;
    else // default
        rCbInfo.CbIdInfo.LockStatus = MTFE_DTV_UNKNOWN;    	

    MTFE_SBTVD_PRINT("SBTVD _CB_PutEventLockStatus: LockStatus? %d\n", rCbInfo.CbIdInfo.LockStatus);
	_CB_PutEvent(CB_MTAL_FE_SBTVD_TUNER_TRIGGER, sizeof(MTAL_FE_BZ_TUNER_CALLBACK_INFO_T), &rCbInfo);
}

/**DTD monitor process.
*@param  pvArg               
*@retval VOID
*/
static VOID _SBTVDMonitorProcess(VOID * pvArg)
{
    //  static INT16 i2PreStatus = 0;
    INT16 	i2LockStatus;
    UINT16  u2_timer2 = 0;    
 //   MTFE_DTV_STATUS_T rDemodStatus;
    UINT16                  u2MsgIdx;
	TUNER_CMD_QUEUE_T	    rCmdQ;
    SIZE_T 					zMsgSize = sizeof(UINT32);
    INT32 					i4Result;
    UINT16                  u2SBTVDLockTimeCnt = 0;
    UINT16                  u2UockCnt = 0;
    EWS_INFO                ewsSts;//EWS Status
	UINT8                    u1PreEwsSts=FALSE;
	UINT8                    u1WaitDemodCnt=cWAIT_DEMOD_LOCK_CNT;//wait 1000ms
	
    while (pMTPdCtx->fgMonitorThreadRunning)
    {    
        //todo: ask ben&Ray timeout value accuracy has problem( MSGQ_DELAY_TIMEOUT )
        i4Result = x_msg_q_receive_timeout(&u2MsgIdx, &rCmdQ, &zMsgSize, &pMTPdCtx->t_MoniCmdMsgQ, 1, MTAL_SBTVD_MON_POLL_INTRVL);//50ms timeout
        //MTFE_SBTVD_PRINT("SBTVD Monitor Get MsgQ=%d\n",i4Result);
        if( i4Result==OSR_OK && rCmdQ.CmdId == TUNER_CMD_CONNECT){
            MTFE_SBTVD_PRINT("SBTVD Monitor Get Cmd TUNER_CMD_CONNECT (MsgQ=%d)\n",i4Result);
            if(u2SBTVDLockTimeCnt!=0){ //last connect event not yet finish, and new connect event come
                _NotifyDemodStatus(MTFE_DTV_BREAK_BY_SECOND_CONNECT_EVENT);
                u2SBTVDLockTimeCnt = 0;//reset u2SBTVDLockTimeCnt for next new connect event
            }
            pMTPdCtx->fgAskMoniDoGetSync = TRUE;//keep do getSync till finish callback event
            pMTPdCtx->eState = MTFE_SBTVD_CONNECTING_STATE; // Change to connecting state
			u1WaitDemodCnt=cWAIT_DEMOD_LOCK_CNT;
        }

        if (pMTPdCtx->fgBreakMonitor) //condition for suspend monitor
        {
            pMTPdCtx->eState = MTFE_SBTVD_IDLE_STATE; // Change to IDLE state        	
            MTFE_SBTVD_PRINT("Entering io_mtfe_SBTVD mon_suspend\n");
            mcSEMA_LOCK_MTFE_S(pMTPdCtx->t_escape_mon_suspend,"SBTVD_mon_suspend");
    		MTFE_SBTVD_PRINT("Escape io_mtfe_SBTVD mon_suspend\n");
        }
        //else if(pMTPdCtx->fgAskMoniDoGetSync){//condition for doing getSync, implement your get sync here
        else if(pMTPdCtx->eState == MTFE_SBTVD_CONNECTING_STATE){ 
            mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when call PI primitive API must use this lock for protection
            u2SBTVDLockTimeCnt++;  
            u1WaitDemodCnt--;			
            i2LockStatus = ISDBT_GetLockSts();
            
            if(i2LockStatus == TRUE)//lock
            {
                MTFE_SBTVD_PRINT("SBTVD  lock found!\n");
                //DTD_Connected(pMTPdCtx->pDemodCtx);
                _NotifyDemodStatus(MTFE_DTV_LOCK);
                pMTPdCtx->fgAskMoniDoGetSync =FALSE; //don't need to do get sync in next loop
                u2SBTVDLockTimeCnt = 0;//reset u2SBTVDLockTimeCnt for next new connect event
                pMTPdCtx->eState = MTFE_SBTVD_LOCK_STATE;
            }
            /* Demod TimeOut found */
            if (ISDBT_TimeOut())
            {
                MTFE_SBTVD_PRINT("SBTVD Timeout found!\n");
                _NotifyDemodStatus(MTFE_DTV_EARLY_BREAK_UNLOCK);
                pMTPdCtx->fgAskMoniDoGetSync =FALSE; //don't need to do get sync in next loop
                u2SBTVDLockTimeCnt = 0;//reset u2SBTVDLockTimeCnt for next new connect event
                pMTPdCtx->eState = MTFE_SBTVD_UNLOCK_STATE;
            }
			  mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
			//Demod unlock
			if(u1WaitDemodCnt==0)
			{
			    MTFE_SBTVD_PRINT("demod can not lock in %d ms!\n",cWAIT_DEMOD_LOCK_CNT*MTAL_SBTVD_MON_POLL_INTRVL);
                _NotifyDemodStatus(MTFE_DTV_UNLOCK);
                pMTPdCtx->fgAskMoniDoGetSync =FALSE; //don't need to do get sync in next loop
                u2SBTVDLockTimeCnt = 0;//reset u2SBTVDLockTimeCnt for next new connect event
                pMTPdCtx->eState = MTFE_SBTVD_UNLOCK_STATE;
			}
          
        }
        else //implement get info or any other regular update job here, delay job is done by x_msg_q_receive_timeout[MTAL_SBTVD_MON_POLL_INTRVL]
		{ /*monitor PI Monitor process*/
	         
	        /* Increase timer*/
			u2_timer2++;
        	//MTFE_SBTVD_PRINT("MTAL SBTVD MonitorProcess! keep update parameter u2_i=%d, u2_timer2=%d\n",u2_i, u2_timer2);	        	
	         //Get EWS Staus
			 mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when call PI primitive API must use this lock for protection
			 
			if(!ISDBT_GetEWSInfo(&ewsSts))
			{
			  MTFE_SBTVD_PRINT(("GetEWS fail\n"));
			  ewsSts.val=u1PreEwsSts;
			}
			if(ewsSts.val!=u1PreEwsSts)//EWS Status change
			{
			  ewsSts.isNotify=TRUE;
			}
			else
			{
			  ewsSts.isNotify=FALSE;
			}
			u1PreEwsSts=ewsSts.val;
			
			 mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);        
			 
			 if(ewsSts.isNotify)//EWS Status change
			 {
			  _NotifyEWSStatus(ewsSts.val);
			 }
			
			 
          	/* Detect if there is any status transition */
			if (u2_timer2 > cMTAL_SBTVD_CHK_STATUS_CNT)//4 times
			{
   			    u2_timer2 = 0;
				mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when call PI primitive API must use this lock for protection
                i2LockStatus = ISDBT_GetLockSts();
                MTFE_SBTVD_PRINT("Polling i2LockStatus = %d\n", i2LockStatus);
                
                if (pMTPdCtx->eState == MTFE_SBTVD_UNLOCK_STATE)
                {
                    if(i2LockStatus == TRUE) // LOCK
                    {
                        _NotifyDemodStatus(MTFE_DTV_LOCK);
                        pMTPdCtx->eState = MTFE_SBTVD_LOCK_STATE;
                        MTFE_SBTVD_PRINT("Change state from unlock to lock\n");
                    }
                }
                else // LOCK_STATE
                {
                	
                	if (i2LockStatus == FALSE)
                		u2UockCnt++;
                	else
                		u2UockCnt = 0;
                	
                    MTFE_SBTVD_PRINT("u2UockCnt = %d\n", u2UockCnt);
                	if (u2UockCnt >= cUNLOCK_CNT_THRD)//10 times
                	{
                		u2UockCnt = 0;                		
                        _NotifyDemodStatus(MTFE_DTV_UNLOCK);
                        pMTPdCtx->eState = MTFE_SBTVD_UNLOCK_STATE;
                        MTFE_SBTVD_PRINT("Change state from lock to unlock\n");
                	}
                }
				mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);                				
			}
			
        }
        //don't do delay here, x_msg_q_receive() will do blocking delay
    }
    pMTPdCtx->t_monitor_thread = (HANDLE_T)NULL;
    MTFE_SBTVD_PRINT("MTAL MonitorProcess exit!\n");
    x_thread_exit();

}

/**Start DTD monitor engine.
*@param  VOID               
*@retval MTR_OK              Success.
*@retval MTR_NOT_OK          Fail.
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
            "_SBTVDMonThread",
            2048, 101,
            _SBTVDMonitorProcess, sizeof(void*), NULL) != OSR_OK)
    {
        MTFE_SBTVD_PRINT("_StartMonitorEngine (mcTHREAD_CREATE): ERROR!\n");
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
    MTFE_SBTVD_PRINT("In kernal:_MTAL_StopMonitorEngine\n");
   if(_fgSBTVDInit!=TRUE){
        MTFE_SBTVD_PRINT("_MTFE_SBTVD_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }
    while (pMTPdCtx->t_monitor_thread) {
        mcDELAY_MS(10); // in ms
    }
    MTFE_SBTVD_PRINT("MTAL PD_StopMonitorEngine success\n");

    return (MTR_OK);
}




//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
/** Initialize front end SBTVD demod module.
*@param VOID
*@retval MTR_OK              Success.
*@retval MTR_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_SBTVD_Open(VOID)
{
    INT32           i4Status;
    MT_RESULT_T     rRet;
    ISDBT_PD_CTX_T    *pISDBTPdCtx;
    TUNER_CMD_QUEUE_T	rCmdQ;

    MTFE_SBTVD_PRINT("In kernel %s\n", __FUNCTION__);
    
    //add driver implementation code here
    if(_fgSBTVDInit==TRUE){
        MTFE_SBTVD_PRINT("_fgSBTVDInit==TRUE, do _MTFE_SBTVD_Close() first\r\n");
        return (MTR_NOT_OK);
    }
    /* TunerOpen */
	
	TunerSetMwIntf(cMTAL_IF);//Set MW type to avoid glue layer to create monitor thread again
	
    i4Status = TunerOpen();
    
    if (i4Status < DRVAPI_TUNER_OK)
    {   
        MTFE_SBTVD_PRINT("TunerOpen failed\n");
        return (MTR_NOT_OK);
    }
    else if (i4Status > DRVAPI_TUNER_OK)
    {
        MTFE_SBTVD_PRINT("TunerOpen has been called\n");        
    }
    else{
        MTFE_SBTVD_PRINT("TunerOpen ok\n");        
    }

    pMTPdCtx = (MTAL_SBTVD_PD_CTX_T *) x_mem_calloc(1, sizeof(MTAL_SBTVD_PD_CTX_T));
    if (pMTPdCtx == NULL)
    {
        MTFE_SBTVD_PRINT("MTAL_SBTVD_PD_CTX_T malloc fail\n");
        return (MTR_NOT_OK);
    }

    //get SBTVD pd Ctx from nucleus pd glue
	pISDBTPdCtx = (ISDBT_PD_CTX_T*)TunerGetCtx(DIGITAL_FE);
	if(pISDBTPdCtx==NULL)
	{
	  MTFE_SBTVD_PRINT("TunerGetCtx  fail\n");
        return (MTR_NOT_OK);
	}
    pMTPdCtx->pDemodCtx = pISDBTPdCtx->pDemodCtx;
    pMTPdCtx->hHalLock = pISDBTPdCtx->hHalLock; 

    if (x_sema_create(&Exclusive_DigiAnaSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        MTFE_SBTVD_PRINT("%s: SBTVD x_sema_create(Exclusive_DigiAnaSema) Fail!\n");
        return (MTR_NOT_OK);
    }
    x_sema_unlock(Exclusive_DigiAnaSema);
    u1ExclSemaInit = 1;

    if (x_sema_create(&pMTPdCtx->t_escape_mon_suspend, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        MTFE_SBTVD_PRINT("%s: SBTVD x_sema_create(pMTPdCtx->t_escape_mon_suspend) Fail!\n");
        return (MTR_NOT_OK);
    }
    x_sema_unlock(pMTPdCtx->t_escape_mon_suspend);

    if ((x_msg_q_create(&pMTPdCtx->t_MoniCmdMsgQ, "SBTVD_Moni_MsgQ", sizeof(rCmdQ), TUNER_CMD_QUEUE_SIZE)) != OSR_OK)
    {
        MTFE_SBTVD_PRINT("%s: SBTVD  x_msg_q_create(pMTPdCtx->t_escape_mon_suspend) Fail!\n");
        return (MTR_NOT_OK);
    }
    pMTPdCtx->fgAskMoniDoGetSync = FALSE; //init value
    pMTPdCtx->fgBreakMonitor = TRUE; //initial monitor is suspend
    pMTPdCtx->eState = MTFE_SBTVD_IDLE_STATE; // Init state
    
    /* If tuner open success, start monitor engine */
    rRet = _StartMonitorEngine();
    if(rRet != MTR_OK)
    {
        MTFE_SBTVD_PRINT("SBTVD _StartMonitorEngine failed\n");
    	return (MTR_NOT_OK);
    }


    _fgSBTVDInit = TRUE;
    return MTR_OK;
}
/** Terminate front end SBTVD demod module.
*@param VOID
*@retval MTR_OK              Success.
*@retval MTR_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_SBTVD_Close(VOID)
{
//INT32 i4Status;
MT_RESULT_T rRet;

    MTFE_SBTVD_PRINT("In kernel %s\n", __FUNCTION__);
    
    //add driver implementation code here
    if(_fgSBTVDInit!=TRUE){
        MTFE_SBTVD_PRINT("_MTFE_SBTVD_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }
 	if(pMTPdCtx->fgBreakMonitor == TRUE){
    	pMTPdCtx->fgBreakMonitor = FALSE; //resume monitor 
		mcSEMA_UNLOCK_MTFE_S(pMTPdCtx->t_escape_mon_suspend,"SBTVD_mon_suspend");
	}
    /* If tuner close success, stop monitor engine */
    rRet = _StopMonitorEngine();
    if(rRet != MTR_OK)
    {
        MTFE_SBTVD_PRINT("SBTVD _StopMonitorEngine failed\n");
        return (MTR_NOT_OK);    	
    }

    x_sema_unlock(Exclusive_DigiAnaSema);
    x_sema_delete(Exclusive_DigiAnaSema);

    x_sema_unlock(pMTPdCtx->t_escape_mon_suspend);
    x_sema_delete(pMTPdCtx->t_escape_mon_suspend);

    x_msg_q_delete(pMTPdCtx->t_MoniCmdMsgQ);
	
	x_mem_free(pMTPdCtx->pDemodCtx);
	x_mem_free((ISDBT_PD_CTX_T*)TunerGetCtx(DIGITAL_FE));
	
    //add driver implementation code here    
    x_mem_free(pMTPdCtx); //free MTAL glue Ctx
        
    /* TunerClose */
    //i4Status = TunerClose(); //don't do LTDIS TunerClose() from MTAL    
    _fgSBTVDInit = FALSE;

    return MTR_OK;
}
/** Start demod module to acquire to specified frequency and modulation type.
*@param  rParameter         Acquisition parameter (RF freq , modulation type,..).
*@retval MTR_OK              Success.
*@retval MTR_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_SBTVD_Connect(unsigned long arg)
{
    MTFE_SBTVD_PARAMETER_T *prParameter;
    MTFE_SBTVD_PARAMETER_T rParameter;
	TUNER_CMD_QUEUE_T	rCmdQ;
    MT_RESULT_T fgRet = MTR_OK;
    PARAM_SETFREQ_T param;//Tuner PLL paramter
   
    prParameter = (MTFE_SBTVD_PARAMETER_T*)arg;
        if(_fgSBTVDInit!=TRUE){
        MTFE_SBTVD_PRINT("_MTFE_SBTVD_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }
   
    _MTFE_NTSC_DisConnect();

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_SBTVD_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_SBTVD_PARAMETER_T);

    MTFE_SBTVD_PRINT("In kernel %s - rParameter.u4Freq = %d KHz \nrParameter.rSurfMode = %d\n",
        __FUNCTION__,
        rParameter.u4Freq,
        rParameter.rSurfMode);

    memcpy(&curParam, &rParameter, sizeof(MTFE_SBTVD_PARAMETER_T));

    pMTPdCtx->fgBreakMonitor = FALSE; //resume monitor to do getSync
    mcSEMA_UNLOCK_MTFE_S(pMTPdCtx->t_escape_mon_suspend,"SBTVD_mon_suspend");
	param.Freq=rParameter.u4Freq;
	param.Modulation=MOD_ISDBT;
    param.fgAutoSearch=(rParameter.rSurfMode == MTFE_SURF_SCAN);
	 pMTPdCtx->pDemodCtx->u4Freq=param.Freq;
    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
   
    //Set Tuner PLL
    if(ISDBT_SetFreq(&param)>0)
	{
	  MTFE_SBTVD_PRINT("Complete Tuner PLL Config\n");
    }
   else    
	{
	fgRet=MTR_NOT_OK;
	}
	//ask monitor to do getSync
    /*preapre for send event to monitor thread*/
    rCmdQ.CmdId = TUNER_CMD_CONNECT;
    rCmdQ.CmdInfo.ConnectParam.u4Freq =rParameter.u4Freq;
  //  rCmdQ.CmdInfo.ConnectParam.rModulation = rParameter.rModulation;
//    rCmdQ.CmdInfo.ConnectParam.rRfMode =rParameter.rRfMode;
    rCmdQ.CmdInfo.ConnectParam.rSurfMode = rParameter.rSurfMode;
    
	 //Config demod
	 if(!ISDBT_Connect())//Wakeup demod
	{
		MTFE_SBTVD_PRINT("ISDBT_Connect fail\n");
		fgRet=MTR_NOT_OK;
	}
	
	if(!ISDBT_ScanInit())//Configure  demod as scan mode
	{
		 MTFE_SBTVD_PRINT("ISDBT_ScanInit fail\n");
		 fgRet=MTR_NOT_OK;
	}
	 if(!ISDBT_Reset())//demodulation operation reset
	{
		MTFE_SBTVD_PRINT("ISDBT_Reset fail\n");
		fgRet=MTR_NOT_OK;
	}
	

    if ((x_msg_q_send(pMTPdCtx->t_MoniCmdMsgQ, &rCmdQ, sizeof(TUNER_CMD_QUEUE_T), 1)) != OSR_OK){
        MTFE_SBTVD_PRINT("Error: _MTFE_SBTVD_Connect() x_msg_q_send failed\n");
        fgRet = MTR_NOT_OK;
    }
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);

    return fgRet;
}
/**Stop acquisition and put Demod into sleep mode.
*@retval MTR_OK              Success.
*@retval MTR_NOT_OK          Fail.
*/

//static MT_RESULT_T _MTFE_SBTVD_DisConnect(VOID)
MT_RESULT_T _MTFE_SBTVD_DisConnect(VOID)
{
   MT_RESULT_T fgRet=MTR_OK;
    MTFE_SBTVD_PRINT("In kernel %s\n", __FUNCTION__);
    if(_fgSBTVDInit!=TRUE){
        MTFE_SBTVD_PRINT("_MTFE_SBTVD_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }
    //add driver implementation code here    
    pMTPdCtx->fgBreakMonitor = TRUE; //suspend monitor

    /* Diconnect */
    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
    if(!ISDBT_Sleep())//Sleep demod
	{
	    fgRet=MTR_NOT_OK;
    	MTFE_SBTVD_PRINT(("Disconnect demod fail\n"));
    }
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
    
    return MTR_OK;
}

/** Get demod lock status.
*@param	 prStatus           Report demod status.
*@retval MTR_OK              Success.
*@retval MTR_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_SBTVD_GetSync(unsigned long arg)
{
    //INT16 *pi2LockStatus;
    INT8 i1LockStatus;
    //pi2LockStatus = (INT16*)arg;
	  MT_RESULT_T fgRet=MTR_OK;
    MTFE_SBTVD_PARAMETER_T *prStatus;
    MTFE_SBTVD_PARAMETER_T rStatus;
    prStatus = (MTFE_SBTVD_PARAMETER_T*)arg;
   
    MTFE_SBTVD_PRINT("In kernel %s\n", __FUNCTION__);
    if(_fgSBTVDInit!=TRUE){
        MTFE_SBTVD_PRINT("_MTFE_SBTVD_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }
   //add driver implementation code here
    /* Get Sync */
    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
    i1LockStatus = ISDBT_GetLockSts();
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
    
    rStatus.rLockStatus = MTFE_DTV_UNLOCK;
    /* Write into content */
    if (i1LockStatus==1)
        rStatus.rLockStatus = MTFE_DTV_LOCK;
    if(i1LockStatus==-1)//I2C fail
	{
	 fgRet=MTR_NOT_OK;
	}
    rStatus.u4Freq = pMTPdCtx->pDemodCtx->u4Freq;//pMTPdCtx->pDemodCtx->Frequency;
    rStatus.rSurfMode = curParam.rSurfMode;

    USR_SPACE_ACCESS_VALIDATE_ARG(prStatus, rStatus);
    COPY_TO_USER_ARG(prStatus, rStatus, MTFE_SBTVD_PARAMETER_T);
    MTFE_SBTVD_PRINT(" - LockStatus = %d u4Freq = %d\n", 
         rStatus.rLockStatus, rStatus.u4Freq);

    return fgRet;

}

/** Get Signal power leve
*@param pi2SigLeveldBm      Signal power level .
*@retval MTR_OK              Success.
*@retval MTR_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_SBTVD_GetSignalLevel(unsigned long arg)
{
    INT16 *pi2SigLevel;
    INT16 i2SigLevel=0;
	 MT_RESULT_T fgRet=MTR_OK;
    pi2SigLevel = (INT16*)arg;
    
    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);  
   if((i2SigLevel=ISDBT_GetSignalLevel())<0)
   {
     MTFE_SBTVD_PRINT("  fail to get if agc\n");
	 fgRet= MTR_NOT_OK;
   }
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
   
    USR_SPACE_ACCESS_VALIDATE_ARG(pi2SigLevel,INT16);
    COPY_TO_USER_ARG(pi2SigLevel,i2SigLevel,INT16);
    MTFE_SBTVD_PRINT(" - i2SigLevel = %d\n",i2SigLevel);
    return fgRet;
}
/**Get packet error rate.
*@param pu4TotalPkt         Received total packets.
*@param pu4ErrPkt           Received error packets.
*@retval MTR_OK              Success.
*@retval MTR_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_SBTVD_GetPER(unsigned long arg)
{
    UINT32 u4Per=100000;
    UINT32 *pu4Per=(UINT32 *)arg;
	 MT_RESULT_T fgRet=MTR_OK;
	 if(_fgSBTVDInit!=TRUE){
        MTFE_SBTVD_PRINT("_MTFE_SBTVD_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }
  //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);	
      if(ISDBT_GetBer(pMTPdCtx->pDemodCtx,FALSE))
		{
		 u4Per= pMTPdCtx->pDemodCtx->u4Per;
		}
	  else
      {
	   fgRet= MTR_NOT_OK;
      }
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
 
    USR_SPACE_ACCESS_VALIDATE_ARG(u4Per,UINT32);
	COPY_TO_USER_ARG(pu4Per,u4Per,UINT32);
	MTFE_SBTVD_PRINT(" per= %d^1e-5\n",u4Per);
	
    return fgRet;
}
/**Get BER.
*@param  pu4TotalBit        Received total bits.
*@param  pu4ErrBit          Received error bits.
*@retval MTR_OK              Success.
*@retval MTR_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_SBTVD_GetBER(unsigned long arg)
{
   UINT32 u4Ber=100000;
    UINT32 *pu4Ber=(UINT32 *)arg;
	 MT_RESULT_T fgRet=MTR_OK;
	 
	 if(_fgSBTVDInit!=TRUE){
        MTFE_SBTVD_PRINT("_MTFE_SBTVD_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }
	
  //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);	
      if(ISDBT_GetBer(pMTPdCtx->pDemodCtx,TRUE))
		{
		 u4Ber= pMTPdCtx->pDemodCtx->u4Ber;
		}
	  else
      {
	   fgRet= MTR_NOT_OK;
      }
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
 
    USR_SPACE_ACCESS_VALIDATE_ARG(u4Ber,UINT32);
	COPY_TO_USER_ARG(pu4Ber,u4Ber,UINT32);
	MTFE_SBTVD_PRINT(" per= %d^1e-5\n",u4Ber);
	
    return fgRet;
}

/** For NTSC to get digital/analog exclusive semaphore.
*@param  *sema              pointer to a sema
*@retval void
*/
void Get_MTFE_SBTVD_Sema_Ex_DiAna(HANDLE_T *sema)
{
    *sema = Exclusive_DigiAnaSema;
}

int mtal_ioctl_mtfe_sbtvd(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
    MT_RESULT_T eRet = MTR_OK;
    MTFE_SBTVD_PRINT("-------SBTVD ioctl cmd-------\r\n");
    if(u1ExclSemaInit==1){
        mcSEMA_LOCK_MTFE_S(Exclusive_DigiAnaSema,"Ex_DiAna");
    }
	switch (cmd)
	{
		case MTAL_IO_SBTVD_OPEN:
			eRet=_MTFE_SBTVD_Open();
			break;
		case MTAL_IO_SBTVD_CLOSE:
			eRet=_MTFE_SBTVD_Close();
            break;
		case MTAL_IO_SBTVD_CONNECT:
			eRet=_MTFE_SBTVD_Connect(arg);
            break;
		case MTAL_IO_SBTVD_DISCONNECT:
		    eRet=_MTFE_SBTVD_DisConnect();
            break;
		case MTAL_IO_SBTVD_GETSYNC:
			eRet=_MTFE_SBTVD_GetSync(arg);
            break;
		case MTAL_IO_SBTVD_GETSIGLEVEL:
			eRet=_MTFE_SBTVD_GetSignalLevel( arg);
            break;
		case MTAL_IO_SBTVD_GETPER:
			eRet=_MTFE_SBTVD_GetPER( arg);
            break;
		case MTAL_IO_SBTVD_GETBER:
			eRet=_MTFE_SBTVD_GetBER(arg);
            break;
		default:
		    eRet=MTR_NOT_SUPPORTED;
		    break;
	}

    if(u1ExclSemaInit==1){
        mcSEMA_UNLOCK_MTFE_S(Exclusive_DigiAnaSema, "Ex_DiAna");
    }
	return eRet;
}
