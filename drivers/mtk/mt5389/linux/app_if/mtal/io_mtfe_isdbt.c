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
 * $RCSfile: io_mtfe_isdbt.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file io_mtfe_isdbt.c
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mtfe_isdb.h"
#include "io_mtfe_pd_def.h"
//#if defined (USE_ATD_IF_DEMOD) 
#include "tuner_interface_if.h"
//#endif
#include "tuner_if.h"
#include "io_mtfe_isdbt.h"
#include "pd_tuner_type.h"
#include "pi_isdbt_demod_if.h"     // pi_demod
#include "pd_isdbt_demod_if.h"    // pd_glue

#include "cb_low.h"  // For callback function
#include <linux/mtal_ioctl.h>

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(int)))                            \
    {                                                       \
        printk("mtal_ioctl_isdbt argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(int)))                        \
    {                                                       \
        printk("mtal_ioctl_isdbt argument error\n");      \
        return -1;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_isdbt argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_isdbt argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_isdbt argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_isdbt argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        printk("mtal_ioctl_isdbt argument error\n");     \
        return MTR_ERR_INV;                                          \
    }

#define COPY_TO_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_isdbt argument error\n");      \
        return MTR_ERR_INV;                                          \
    }
#define COPY_FROM_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_from_user(&ArgKernel,(void __user *)ArgUsr,    \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_isdbt argument error\n");      \
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
#define MTAL_ISDBT_MON_POLL_INTRVL             50  // 50ms: Basic resoultion for timers
#define MTAL_ISDBT_MON_INFO_UPDATE_INTRVL    1000  // 1sec
#define cMTAL_ISDBT_CHK_STATUS_INTRVL         200  // 200ms

#define MTAL_ISDBT_MON_INFO_UPDATE_CNT       (MTAL_ISDBT_MON_INFO_UPDATE_INTRVL/MTAL_ISDBT_MON_POLL_INTRVL)
#define cMTAL_ISDBT_CHK_STATUS_CNT           (cMTAL_ISDBT_CHK_STATUS_INTRVL/MTAL_ISDBT_MON_POLL_INTRVL)


#define cUNLOCK_CNT_THRD                     10    // The criteria for state transition from lock to unlock
#define cWAIT_ISDBT_DEMOD_LOCK_CNT            20    //wait 20*50ms


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
extern void TunerSetMwIntf(UINT8 u1Value);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static MTAL_ISDBT_PD_CTX_T     *pMTISDBTPdCtx;
//static TUNER_DEVICE_CTX_T       *pISDBTunerDeviceCtx = NULL;

static BOOL _fgISDBTInit=FALSE;
/*all variable used in this file should put into pMTISDBTPdCtx 
except for Exclusive_DigiAnaSema and u1ExclSemaInit*/
static HANDLE_T    Exclusive_DigiAnaSema=0;
static UINT8       u1ExclSemaInit=0;

static MTFE_ISDBT_PARAMETER_T    curParam;

extern MT_RESULT_T _MTFE_NTSC_DisConnect(VOID);
//-----------------------------------------------------------------------------
// Static Functions
//-----------------------------------------------------------------------------
static void _Notify_ISDBT_EWSStatus(UINT8 EwsStatus)
{
MTAL_FE_JAPAN_TUNER_CALLBACK_INFO_T rCbInfo;
  
    MTFE_ISDBT_PRINT("%s: EwsStatus = %d\n", __FUNCTION__, EwsStatus);
    rCbInfo.CbId=MTAL_FE_ISDB_EWS_STATUS;
  
    if(EwsStatus)//Ews Start
    {
        rCbInfo.CbIdInfo.EwsEvent=MTAL_FE_ISDB_EWS_START;
    }
    else
    {
        rCbInfo.CbIdInfo.EwsEvent=MTAL_FE_ISDB_EWS_END;
    }
    _CB_PutEvent(CB_MTAL_FE_JAPAN_TUNER_TRIGGER, sizeof(MTAL_FE_JAPAN_TUNER_CALLBACK_INFO_T), &rCbInfo);
}
static void _Notify_ISDBT_DemodStatus(INT16 LockStatus)
{
MTAL_FE_JAPAN_TUNER_CALLBACK_INFO_T rCbInfo;

    MTFE_ISDBT_PRINT("%s: DTD LockStatus = %d\n", __FUNCTION__, LockStatus);
    rCbInfo.CbId = MTAL_FE_ISDB_LOCK_STATUS;

    if (LockStatus < MTFE_DTV_UNKNOWN)
    {
        rCbInfo.CbIdInfo.LockStatus = LockStatus;
    }      
    else // default
    {
        rCbInfo.CbIdInfo.LockStatus = MTFE_DTV_UNKNOWN; 
    }          	

    MTFE_ISDBT_PRINT("JAPAN _CB_PutEventLockStatus: LockStatus? %d\n", rCbInfo.CbIdInfo.LockStatus);
    _CB_PutEvent(CB_MTAL_FE_JAPAN_TUNER_TRIGGER, sizeof(MTAL_FE_JAPAN_TUNER_CALLBACK_INFO_T), &rCbInfo);
}

/**ISDB-T monitor process.
*@param  pvArg               
*@retval VOID
*/
static VOID _ISDBTMonitorProcess(VOID)
{
    INT16 	i2LockStatus;
    UINT16  u2_timer2 = 0;    
    UINT16                  u2MsgIdx;
    TUNER_ISDBT_CMD_QUEUE_T	    rCmdQ;
    SIZE_T 		    zMsgSize = sizeof(UINT32);
    INT32 			    i4Result;
    UINT16                  u2ISDBTLockTimeCnt = 0;
    UINT16                  u2UockCnt = 0;
    EWS_INFO             ewsSts;//EWS Status
    UINT8                    u1PreEwsSts=FALSE;
    UINT8                    u1WaitDemodCnt=cWAIT_ISDBT_DEMOD_LOCK_CNT;//wait 1000ms
	
    while (pMTISDBTPdCtx->fgMonitorThreadRunning)
    {    
        i4Result = x_msg_q_receive_timeout(&u2MsgIdx, &rCmdQ, &zMsgSize, &pMTISDBTPdCtx->t_MoniCmdMsgQ, 1, MTAL_ISDBT_MON_POLL_INTRVL);//50ms timeout
        if( i4Result==OSR_OK && rCmdQ.CmdId == TUNER_ISDBT_CMD_CONNECT)
	 {
            MTFE_ISDBT_PRINT("ISDBT Monitor Get Cmd TUNER_ISDBT_CMD_CONNECT (MsgQ=%ld)\n",i4Result);
            if(u2ISDBTLockTimeCnt!=0)
            { //last connect event not yet finish, and new connect event come
                _Notify_ISDBT_DemodStatus(MTFE_DTV_BREAK_BY_SECOND_CONNECT_EVENT);
                u2ISDBTLockTimeCnt = 0;//reset u2ISDBTLockTimeCnt for next new connect event
            }
            pMTISDBTPdCtx->fgAskMoniDoGetSync = TRUE;//keep do getSync till finish callback event
            pMTISDBTPdCtx->eState = MTFE_ISDBT_CONNECTING_STATE; // Change to connecting state
            u1WaitDemodCnt=cWAIT_ISDBT_DEMOD_LOCK_CNT;
        }

        if (pMTISDBTPdCtx->fgBreakMonitor) //condition for suspend monitor
        {
            pMTISDBTPdCtx->eState = MTFE_ISDBT_IDLE_STATE; // Change to IDLE state        	
            MTFE_ISDBT_PRINT("Entering io_mtfe_ISDBT mon_suspend\n");
            mcSEMA_LOCK_MTFE_S(pMTISDBTPdCtx->t_escape_mon_suspend,"ISDBT_mon_suspend");
            MTFE_ISDBT_PRINT("Escape io_mtfe_ISDBT mon_suspend\n");
        }
        else if(pMTISDBTPdCtx->eState == MTFE_ISDBT_CONNECTING_STATE)
        { 
            mcSEMA_LOCK_MTFE(pMTISDBTPdCtx->hHalLock); //when call PI primitive API must use this lock for protection
            u2ISDBTLockTimeCnt++;  
            u1WaitDemodCnt--;	           
            i2LockStatus = ISDBT_GetSync(pMTISDBTPdCtx->pDemodCtx);
            
            if(i2LockStatus == TRUE)//lock
            {
                MTFE_ISDBT_PRINT("ISDBT  lock found!\n");
                _Notify_ISDBT_DemodStatus(MTFE_DTV_LOCK);
                pMTISDBTPdCtx->fgAskMoniDoGetSync =FALSE; //don't need to do get sync in next loop
                u2ISDBTLockTimeCnt = 0;//reset u2ISDBTLockTimeCnt for next new connect event
                pMTISDBTPdCtx->eState = MTFE_ISDBT_LOCK_STATE;
            }
 
            mcSEMA_UNLOCK_MTFE(pMTISDBTPdCtx->hHalLock);
            if(u1WaitDemodCnt==0)
            {
                MTFE_ISDBT_PRINT("demod can not lock in %d ms!\n",cWAIT_ISDBT_DEMOD_LOCK_CNT*MTAL_ISDBT_MON_POLL_INTRVL);
                _Notify_ISDBT_DemodStatus(MTFE_DTV_UNLOCK);
                pMTISDBTPdCtx->fgAskMoniDoGetSync =FALSE; //don't need to do get sync in next loop
                u2ISDBTLockTimeCnt = 0;
                pMTISDBTPdCtx->eState = MTFE_ISDBT_UNLOCK_STATE;
            }          
        }
        else //implement get info or any other regular update job here, delay job is done by x_msg_q_receive_timeout[MTAL_ISDBT_MON_POLL_INTRVL]
        { /*monitor PI Monitor process*/
            /* Increase timer*/
            u2_timer2++;
            mcSEMA_LOCK_MTFE(pMTISDBTPdCtx->hHalLock); //when call PI primitive API must use this lock for protection
			 
            ewsSts.val = ISDBT_GetEws(pMTISDBTPdCtx->pDemodCtx);
            if(ewsSts.val!=u1PreEwsSts)//EWS Status change
            {
                ewsSts.isNotify=TRUE;
            }
            else
            {
                ewsSts.isNotify=FALSE;
            }
            u1PreEwsSts=ewsSts.val;
			
            mcSEMA_UNLOCK_MTFE(pMTISDBTPdCtx->hHalLock);        
			 
            if(ewsSts.isNotify)//EWS Status change
            {
                _Notify_ISDBT_EWSStatus(ewsSts.val);
            }			
			 
          	/* Detect if there is any status transition */
            if (u2_timer2 > cMTAL_ISDBT_CHK_STATUS_CNT)//4 
            {
                u2_timer2 = 0;
                mcSEMA_LOCK_MTFE(pMTISDBTPdCtx->hHalLock); //when call PI primitive API must use this lock for protection
                i2LockStatus =  ISDBT_GetSync(pMTISDBTPdCtx->pDemodCtx);
                MTFE_ISDBT_PRINT("Polling i2LockStatus = %d\n", i2LockStatus);              
                if (pMTISDBTPdCtx->eState == MTFE_ISDBT_UNLOCK_STATE)
                {
                    if(i2LockStatus == TRUE) // LOCK
                    {
                        _Notify_ISDBT_DemodStatus(MTFE_DTV_LOCK);
                        pMTISDBTPdCtx->eState = MTFE_ISDBT_LOCK_STATE;
                        MTFE_ISDBT_PRINT("Change state from unlock to lock\n");
                    }
                }
                else // LOCK_STATE
                {             	
                    if (i2LockStatus == FALSE)
                		u2UockCnt++;
                    else
                		u2UockCnt = 0;
                	
                    MTFE_ISDBT_PRINT("u2UockCnt = %d\n", u2UockCnt);
                    if (u2UockCnt >= cUNLOCK_CNT_THRD)//10 times
                    {
                        u2UockCnt = 0;                		
                        _Notify_ISDBT_DemodStatus(MTFE_DTV_UNLOCK);
                        pMTISDBTPdCtx->eState = MTFE_ISDBT_UNLOCK_STATE;
                        MTFE_ISDBT_PRINT("Change state from lock to unlock\n");
                    }
                }
                mcSEMA_UNLOCK_MTFE(pMTISDBTPdCtx->hHalLock);                				
            }
			
        }
        //don't do delay here, x_msg_q_receive() will do blocking delay
    }

    pMTISDBTPdCtx->t_monitor_thread = NULL;
    MTFE_ISDBT_PRINT("MTAL MonitorProcess exit!\n");
    x_thread_exit();
}

/**Start ISDBT monitor engine.
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
    pMTISDBTPdCtx->fgMonitorThreadRunning = TRUE;
    if (x_thread_create(&pMTISDBTPdCtx->t_monitor_thread,
            "_IsdbtMonThread",
            2048, 101,
            _ISDBTMonitorProcess, sizeof(void*), NULL) != OSR_OK)
    {
        MTFE_ISDBT_PRINT("_StartMonitorEngine (mcTHREAD_CREATE): ERROR!\n");
        return (MTR_NOT_OK);
    }
    MTFE_ISDBT_PRINT("_StartMonitorEngine success\n");
    return (MTR_OK);
}

/**Stop ISDBT monitor engine.
*@param  VOID               
*@retval MTR_OK              Success.
*@retval MTR_NOT_OK          Fail.
*/
static MT_RESULT_T _StopMonitorEngine(VOID)
{
    pMTISDBTPdCtx->fgMonitorThreadRunning = FALSE;
    MTFE_ISDBT_PRINT("In kernal:_MTAL_StopMonitorEngine\n");

    while (pMTISDBTPdCtx->t_monitor_thread) 
    {
        mcDELAY_MS(10); // in ms
    }
    MTFE_ISDBT_PRINT("MTAL PD_StopMonitorEngine success\n");

    return (MTR_OK);
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
/** Initialize front end ISDBT demod module.
*@param VOID
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_ISDBT_Open(VOID)
{
    INT32           i4Status;
 //   ISDBT_PD_CTX_T    *pISDBTPdCtx;
    TUNER_ISDBT_CMD_QUEUE_T	rCmdQ;

    MTFE_ISDBT_PRINT("In kernel %s\n", __FUNCTION__);
    
    //add driver implementation code here
    if(_fgISDBTInit==TRUE)
    {
        MTFE_ISDBT_PRINT("_fgISDBTInit==TRUE, do _MTFE_ISDBT_Close() first\r\n");
        return (MTR_NOT_OK);
    }
	
    // TunerOpen
    TunerSetMwIntf(cMTAL_IF);//Set MW type to avoid glue layer to create monitor thread again
    i4Status = TunerOpen();    // Call the ISDBT_TunerOpen in pd_glue
    if (i4Status < DRVAPI_TUNER_OK)
    {   
        MTFE_ISDBT_PRINT("TunerOpen failed\n");
        return (MTR_NOT_OK);
    }
    else if (i4Status > DRVAPI_TUNER_OK)
    {
        MTFE_ISDBT_PRINT("TunerOpen has been called\n");        
    }
    else
    {
        MTFE_ISDBT_PRINT("TunerOpen ok\n");        
    }

    // Create MTAL pd glue Contex
    pMTISDBTPdCtx = (MTAL_ISDBT_PD_CTX_T *) x_mem_calloc(1, sizeof(MTAL_ISDBT_PD_CTX_T));
    if (pMTISDBTPdCtx == NULL)
    {
        MTFE_ISDBT_PRINT("MTAL_ISDBT_PD_CTX_T malloc fail\n");
        return (MTR_NOT_OK);
    }
    //Create PI demod ctx
    //get ISDBT pd Ctx from nucleus pd glue
    /*
    pISDBTPdCtx = (ISDBT_PD_CTX_T*)TunerGetCtx(DIGITAL_FE);
    if(pISDBTPdCtx==NULL)
    {
        MTFE_ISDBT_PRINT("TunerGetCtx  fail\n");
        return (MTR_NOT_OK);
    }
    pMTISDBTPdCtx->hHalLock = pISDBTPdCtx->hHalLock; 
    */

    //create demod ctx
    pMTISDBTPdCtx->pDemodCtx = (ISDBT_DEMOD_CTX_T *)psISDBT_DemodCtx();
    if (pMTISDBTPdCtx->pDemodCtx == NULL)
    {
        MTFE_ISDBT_PRINT("ISDBT Tuner Open, pid Demod malloc fail\n");
        return (MTR_NOT_OK);
    }

    //Create a semaphore for low level demod PI access protection
    if (x_sema_create(&pMTISDBTPdCtx->hHalLock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        MTFE_ISDBT_PRINT("ISDBT x_sema_create(pMTISDBTPdCtx->hHalLock) Fail!\n");
        return (MTR_NOT_OK);
    }
    x_sema_unlock(pMTISDBTPdCtx->hHalLock);

    //
    if (x_sema_create(&Exclusive_DigiAnaSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        MTFE_ISDBT_PRINT("ISDBT x_sema_create(Exclusive_DigiAnaSema) Fail!\n");
        return (MTR_NOT_OK);
    }
    x_sema_unlock(Exclusive_DigiAnaSema);
    u1ExclSemaInit = 1;

    if (x_sema_create(&pMTISDBTPdCtx->t_escape_mon_suspend, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        MTFE_ISDBT_PRINT("ISDBT x_sema_create(pMTISDBTPdCtx->t_escape_mon_suspend) Fail!\n");
        return (MTR_NOT_OK);
    }
    x_sema_unlock(pMTISDBTPdCtx->t_escape_mon_suspend);

    if ((x_msg_q_create(&pMTISDBTPdCtx->t_MoniCmdMsgQ, "ISDBT_Moni_MsgQ", sizeof(rCmdQ), TUNER_ISDBT_CMD_QUEUE_SIZE)) != OSR_OK)
    {
        MTFE_ISDBT_PRINT("ISDBT  x_msg_q_create(pMTISDBTPdCtx->t_escape_mon_suspend) Fail!\n");
        return (MTR_NOT_OK);
    }	

    pMTISDBTPdCtx->fgAskMoniDoGetSync = FALSE; //init value
    pMTISDBTPdCtx->fgBreakMonitor = TRUE; //initial monitor is suspend

    // If tuner open success, start monitor engine
    if(MTR_OK != _StartMonitorEngine())
    {
        MTFE_ISDBT_PRINT("ISDBT _StartMonitorEngine failed\n");
    	return (MTR_NOT_OK);
    }

    _fgISDBTInit = TRUE;

    return MTR_OK;
}

/** Terminate front end ISDBT demod module.
*@param VOID
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ISDBT_Close(VOID)
{

    MTFE_ISDBT_PRINT("In kernel %s\n", __FUNCTION__);
    
    //add driver implementation code here
    if(_fgISDBTInit!=TRUE){
        MTFE_ISDBT_PRINT("_MTFE_ISDBT_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }
    pMTISDBTPdCtx->fgBreakMonitor = TRUE;

    /* If tuner close success, stop monitor engine */
    if(MTR_OK != _StopMonitorEngine())
    {
        MTFE_ISDBT_PRINT("ISDBT _StopMonitorEngine failed\n");
        return (MTR_NOT_OK);    	
    }

    x_sema_unlock(Exclusive_DigiAnaSema);
    x_sema_delete(Exclusive_DigiAnaSema);

    x_sema_unlock(pMTISDBTPdCtx->t_escape_mon_suspend);
    x_sema_delete(pMTISDBTPdCtx->t_escape_mon_suspend);

    x_msg_q_delete(pMTISDBTPdCtx->t_MoniCmdMsgQ);
    //add driver implementation code here
    bISDBT_DemodCtxDestroy(); // free the pi_demodctx 
    x_mem_free(pMTISDBTPdCtx); //free MTAL glue Ctx
        
    _fgISDBTInit = FALSE;

    return MTR_OK;
}



/** Start demod module to acquire to specified frequency and modulation type.
*@param  rParameter         Acquisition parameter (RF freq , modulation type,..).
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ISDBT_Connect(unsigned long arg)
{
    MTFE_ISDBT_PARAMETER_T *prParameter;   // User para pointer
    MTFE_ISDBT_PARAMETER_T rParameter;       // Kernal para pointer
    prParameter = (MTFE_ISDBT_PARAMETER_T*)arg;
    UINT8 ii, u1Flag;
    TUNER_ISDBT_CMD_QUEUE_T	rCmdQ;
    MT_RESULT_T fgRet;

    u1Flag = 0;
    fgRet = MTR_OK;

    _MTFE_NTSC_DisConnect();

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBT_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_ISDBT_PARAMETER_T);

    MTFE_ISDBT_PRINT("In kernel %s - rParameter.u4Freq = %ld\n"
        "rParameter.rModulation = %d\n",
        __FUNCTION__,
        rParameter.u4Freq,
        rParameter.rModulation);

    memcpy(&curParam, &rParameter, sizeof(MTFE_ISDBT_PARAMETER_T));

    pMTISDBTPdCtx->fgBreakMonitor = FALSE; //resume monitor to do getSync
    mcSEMA_UNLOCK_MTFE_S(pMTISDBTPdCtx->t_escape_mon_suspend,"isdbt_mon_suspend");

    mcSEMA_LOCK_MTFE(pMTISDBTPdCtx->hHalLock);
    //add driver implementation code here    
     
    //ask monitor to do getSync
    /*preapre for send event to monitor thread*/
    rCmdQ.CmdId = TUNER_ISDBT_CMD_CONNECT;
    rCmdQ.CmdInfo.ConnectParam.u4Freq =rParameter.u4Freq;
    rCmdQ.CmdInfo.ConnectParam.rModulation = rParameter.rModulation;
    rCmdQ.CmdInfo.ConnectParam.rRfMode =rParameter.rRfMode;
    rCmdQ.CmdInfo.ConnectParam.rSurfMode = rParameter.rSurfMode;

    /* Connect */
     // Select Demod OFDM1 or OFDM2 
    switch(rParameter.rTunerID)
    {   
    case MTFE_TUNER_OFDM1:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM1;
        break;
    case MTFE_TUNER_OFDM2:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM2;
        break;
    default:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM1;
        break;  
   }
    // Select Demod TS Output System B or C
    switch(rParameter.rTsIO)
    {    
    case MTFE_TS_SERIAL_IO_B:
        pMTISDBTPdCtx->pDemodCtx->rTsIO= ISDBT_TS_SERIAL_IO_B;
        break;
    case MTFE_TS_SERIAL_IO_C:
        pMTISDBTPdCtx->pDemodCtx->rTsIO= ISDBT_TS_SERIAL_IO_C;
        break;
    default:
        pMTISDBTPdCtx->pDemodCtx->rTsIO = ISDBT_TS_SERIAL_IO_B;
        break;  
    }
    pMTISDBTPdCtx->pDemodCtx->Freqency = rParameter.u4Freq;
    // Connect the Demod
    u1Flag |= ISDBT_Connect(pMTISDBTPdCtx->pDemodCtx);
    // Set the Tuner Frequency
    u1Flag |= ISDBT_SetFreq(pMTISDBTPdCtx->pDemodCtx);

    if (u1Flag)
    {
        MTFE_ISDBT_PRINT("Tuner SetFreq Fail\n");
	 fgRet = MTR_NOT_OK;
    }

    ISDBT_ScanInit(pMTISDBTPdCtx->pDemodCtx);

    // Get the sync
   for(ii=0; ii < 100; ii++)
   {
       if(ISDBT_GetSync(pMTISDBTPdCtx->pDemodCtx))
       {   // Signal Sync
           MTFE_ISDBT_PRINT("Demod GetSync, Lock at %d times\n", ii);
           rParameter.rLockStatus = MTFE_DTV_LOCK;
           break;
       }
       mcDELAY_MS(10);
   }
    if(ii == 100)
    {   // Get Sync time over 90, break
        MTFE_ISDBT_PRINT("Demod unsync\n");
        rParameter.rLockStatus = MTFE_DTV_UNLOCK;
      //  fgRet = MTR_NOT_OK;
    }

    if ((x_msg_q_send(pMTISDBTPdCtx->t_MoniCmdMsgQ, &rCmdQ, sizeof(TUNER_ISDBT_CMD_QUEUE_T), 1)) != OSR_OK)
    {
        MTFE_ISDBT_PRINT("Error: _MTFE_ISDBT_Connect() x_msg_q_send failed\n");
        fgRet = MTR_NOT_OK;
    }
    mcSEMA_UNLOCK_MTFE(pMTISDBTPdCtx->hHalLock);
	
    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBT_PARAMETER_T);
    COPY_TO_USER_ARG(prParameter, rParameter,MTFE_ISDBT_PARAMETER_T);

    return fgRet;
}

/**Stop acquisition and put Demod into sleep mode.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

MT_RESULT_T _MTFE_ISDBT_DisConnect(VOID)
{
    MTFE_ISDBT_PRINT("In kernel %s\n", __FUNCTION__);
    if(_fgISDBTInit!=TRUE)
    {
        return MTR_OK;
    }
    //add driver implementation code here    
    pMTISDBTPdCtx->fgBreakMonitor = TRUE; //suspend monitor

    /* Diconnect */
    mcSEMA_LOCK_MTFE(pMTISDBTPdCtx->hHalLock);
    ISDBT_Sleep(pMTISDBTPdCtx->pDemodCtx);
    mcSEMA_UNLOCK_MTFE(pMTISDBTPdCtx->hHalLock);
    
    return MTR_OK;
}

/** Start ISDB-T demod module to set the Tuner LNA Status.
*@param  prPara        Parameter (LNA Status, ... ).
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_ISDBT_SetLNA(unsigned long arg)
{
    MTFE_ISDBT_PARAMETER_T *prParameter;   // User para pointer
    MTFE_ISDBT_PARAMETER_T rParameter;       // Kernal para pointer
    prParameter = (MTFE_ISDBT_PARAMETER_T*)arg;
    UINT8  u1Flag;
    MT_RESULT_T fgRet;

    if(_fgISDBTInit!=TRUE)
    {  // Tuner Open Fail
        MTFE_ISDBT_PRINT("_MTFE_ISDBT_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }
	
    fgRet = MTR_OK;
    u1Flag = 0;

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBT_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_ISDBT_PARAMETER_T);

    MTFE_ISDBT_PRINT("In kernel %s - rParameter.u4Freq = %ld\n", __FUNCTION__, rParameter.u4Freq);

    // Select Demod OFDM1 or OFDM2 
    switch(rParameter.rTunerID)
    {	 
    case MTFE_TUNER_OFDM1:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM1;
        break;
    case MTFE_TUNER_OFDM2:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM2;
        break;
    default:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM1;
        break;  
    }

    // Set the ISDB-T Tuner LNA Status
    if(rParameter.rLNASts == MTFE_LNA_ON)
    {
        pMTISDBTPdCtx->pDemodCtx->rLNAStatus = ISDBT_LNA_ON;
    }
    else
    {
        pMTISDBTPdCtx->pDemodCtx->rLNAStatus = ISDBT_LNA_OFF;
    }

    mcSEMA_LOCK_MTFE(pMTISDBTPdCtx->hHalLock);   
    ISDBT_DemodAddr(pMTISDBTPdCtx->pDemodCtx);
    // Set the ISDB-T Tuner LNA
    u1Flag |= ISDBT_SetTunerLNA(pMTISDBTPdCtx->pDemodCtx);
    mcSEMA_UNLOCK_MTFE(pMTISDBTPdCtx->hHalLock);

    if (u1Flag)
    {
        MTFE_ISDBT_PRINT("ISDBT Set Tuner LNA fail: LNAStatus = %d\n", pMTISDBTPdCtx->pDemodCtx->rLNAStatus);  
	 fgRet = MTR_NOT_OK;
    }

    return fgRet;
}

/** Start demod module to acquire to specified frequency.
*@param  prParameter        Acquisition parameter (RF freq).
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_ISDBT_SetFreq(unsigned long arg)
{
    MTFE_ISDBT_PARAMETER_T *prParameter;   // User para pointer
    MTFE_ISDBT_PARAMETER_T rParameter;       // Kernal para pointer
    prParameter = (MTFE_ISDBT_PARAMETER_T*)arg;
    UINT8  u1Flag;
    MT_RESULT_T fgRet;

    if(_fgISDBTInit!=TRUE)
    {  // Tuner Open Fail
        MTFE_ISDBT_PRINT("_MTFE_ISDBT_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }
	
    fgRet = MTR_OK;
    u1Flag = 0;

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBT_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_ISDBT_PARAMETER_T);

    MTFE_ISDBT_PRINT("In kernel %s - rParameter.u4Freq = %ld\n", __FUNCTION__, rParameter.u4Freq);

    // Select Demod OFDM1 or OFDM2 
    switch(rParameter.rTunerID)
    {	 
    case MTFE_TUNER_OFDM1:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM1;
        break;
    case MTFE_TUNER_OFDM2:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM2;
        break;
    default:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM1;
        break;  
    }

    mcSEMA_LOCK_MTFE(pMTISDBTPdCtx->hHalLock);   
    ISDBT_DemodAddr(pMTISDBTPdCtx->pDemodCtx);
    /* Set Freq */
    pMTISDBTPdCtx->pDemodCtx->Freqency = rParameter.u4Freq;
    // Set the Tuner Frequency
    u1Flag |= ISDBT_SetFreq(pMTISDBTPdCtx->pDemodCtx);
    if (u1Flag)
    {
        MTFE_ISDBT_PRINT("TunerSetFreq fail\n");  
	 fgRet = MTR_NOT_OK;
    }
    mcSEMA_UNLOCK_MTFE(pMTISDBTPdCtx->hHalLock);

    return fgRet;
}

/**Get packet error rate.
*@param pu4TotalPkt         Received total packets.
*@param pu4ErrPkt           Received error packets.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ISDBT_GetPER(unsigned long arg)
{
    MTFE_ISDBT_PARAMETER_T *prParameter;   // User para pointer
    MTFE_ISDBT_PARAMETER_T rParameter;       // Kernal para pointer
    prParameter = (MTFE_ISDBT_PARAMETER_T*)arg;

    if(_fgISDBTInit!=TRUE)
    {  // Tuner Open Fail
        MTFE_ISDBT_PRINT("_MTFE_ISDBT_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBT_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_ISDBT_PARAMETER_T);
  
    // Select Demod OFDM1 or OFDM2 
    switch(rParameter.rTunerID)
    {	 
    case MTFE_TUNER_OFDM1:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM1;
        break;
    case MTFE_TUNER_OFDM2:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM2;
        break;
    default:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM1;
        break;  
    }

    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTISDBTPdCtx->hHalLock);
    ISDBT_DemodAddr(pMTISDBTPdCtx->pDemodCtx);
    rParameter.u4PER = ISDBT_GetBer(pMTISDBTPdCtx->pDemodCtx, TRUE);    
    mcSEMA_UNLOCK_MTFE(pMTISDBTPdCtx->hHalLock);

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBT_PARAMETER_T);
    COPY_TO_USER_ARG(prParameter, rParameter,MTFE_ISDBT_PARAMETER_T);

    return MTR_OK;
}

/**Get BER.
*@param  pu4TotalBit        Received total bits.
*@param  pu4ErrBit          Received error bits.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_ISDBT_GetBER(unsigned long arg)
{
    MTFE_ISDBT_PARAMETER_T *prParameter;   // User para pointer
    MTFE_ISDBT_PARAMETER_T rParameter;       // Kernal para pointer
    prParameter = (MTFE_ISDBT_PARAMETER_T*)arg;

    if(_fgISDBTInit!=TRUE)
    {  // Tuner Open Fail
        MTFE_ISDBT_PRINT("_MTFE_ISDBT_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBT_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_ISDBT_PARAMETER_T);
  
    // Select Demod OFDM1 or OFDM2 
    switch(rParameter.rTunerID)
    {	 
    case MTFE_TUNER_OFDM1:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM1;
        break;
    case MTFE_TUNER_OFDM2:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM2;
        break;
    default:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM1;
        break;  
    }

    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTISDBTPdCtx->hHalLock);
    ISDBT_DemodAddr(pMTISDBTPdCtx->pDemodCtx);
    rParameter.u4BER= ISDBT_GetBer(pMTISDBTPdCtx->pDemodCtx, FALSE);
    mcSEMA_UNLOCK_MTFE(pMTISDBTPdCtx->hHalLock);

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBT_PARAMETER_T);
    COPY_TO_USER_ARG(prParameter, rParameter,MTFE_ISDBT_PARAMETER_T);

    return MTR_OK;
}

/**Get CNR in dB.
*@param u2SignalSNR         Signal SNR
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ISDBT_GetCNR(unsigned long arg)
{
    MTFE_ISDBT_PARAMETER_T *prParameter;   // User para pointer
    MTFE_ISDBT_PARAMETER_T rParameter;       // Kernal para pointer
    prParameter = (MTFE_ISDBT_PARAMETER_T*)arg;

    if(_fgISDBTInit!=TRUE)
    {  // Tuner Open Fail
        MTFE_ISDBT_PRINT("_MTFE_ISDBT_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBT_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_ISDBT_PARAMETER_T);
  
    // Select Demod OFDM1 or OFDM2 
    switch(rParameter.rTunerID)
    {	 
    case MTFE_TUNER_OFDM1:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM1;
        break;
    case MTFE_TUNER_OFDM2:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM2;
        break;
    default:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM1;
        break;  
    }

    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTISDBTPdCtx->hHalLock);
    ISDBT_DemodAddr(pMTISDBTPdCtx->pDemodCtx);
    rParameter.u4CNR = ISDBT_GetCNR(pMTISDBTPdCtx->pDemodCtx);
    mcSEMA_UNLOCK_MTFE(pMTISDBTPdCtx->hHalLock);

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBT_PARAMETER_T);
    COPY_TO_USER_ARG(prParameter, rParameter,MTFE_ISDBT_PARAMETER_T);

    return MTR_OK;
}

/** Get demod lock status.
*@param	 prStatus           Report demod status.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_ISDBT_GetSync(unsigned long arg)
{
    MTFE_ISDBT_PARAMETER_T *prParameter;   // User para pointer
    MTFE_ISDBT_PARAMETER_T rParameter;       // Kernal para pointer
    prParameter = (MTFE_ISDBT_PARAMETER_T*)arg;

    if(_fgISDBTInit!=TRUE)
    {  // Tuner Open Fail
        MTFE_ISDBT_PRINT("_MTFE_ISDBT_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBT_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_ISDBT_PARAMETER_T);
  
    // Select Demod OFDM1 or OFDM2 
    switch(rParameter.rTunerID)
    {	 
    case MTFE_TUNER_OFDM1:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM1;
        break;
    case MTFE_TUNER_OFDM2:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM2;
        break;
    default:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM1;
        break;  
    }

    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTISDBTPdCtx->hHalLock);
    ISDBT_DemodAddr(pMTISDBTPdCtx->pDemodCtx);
    if(!ISDBT_GetSync(pMTISDBTPdCtx->pDemodCtx))
    {   // Demod UnLock
        rParameter.rLockStatus = MTFE_DTV_UNLOCK;
    }
    else
    {
        rParameter.rLockStatus = MTFE_DTV_LOCK;
    }
    mcSEMA_UNLOCK_MTFE(pMTISDBTPdCtx->hHalLock);

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBT_PARAMETER_T);
    COPY_TO_USER_ARG(prParameter, rParameter,MTFE_ISDBT_PARAMETER_T);

    return MTR_OK;
}

/**Get IF AGC Voltage in mV.
*@param  pu2IfAgcVol        IF AGC voltage in mV.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ISDBT_GetIfAgc(unsigned long arg)
{
    MTFE_ISDBT_PARAMETER_T *prParameter;   // User para pointer
    MTFE_ISDBT_PARAMETER_T rParameter;       // Kernal para pointer
    prParameter = (MTFE_ISDBT_PARAMETER_T*)arg;

    if(_fgISDBTInit!=TRUE)
    {  // Tuner Open Fail
        MTFE_ISDBT_PRINT("_MTFE_ISDBT_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBT_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_ISDBT_PARAMETER_T);
  
    // Select Demod OFDM1 or OFDM2 
    switch(rParameter.rTunerID)
    {	 
    case MTFE_TUNER_OFDM1:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM1;
        break;
    case MTFE_TUNER_OFDM2:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM2;
        break;
    default:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM1;
        break;  
    }

    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTISDBTPdCtx->hHalLock);
    ISDBT_DemodAddr(pMTISDBTPdCtx->pDemodCtx);
    rParameter.u1IFAGC= ISDBT_GetAgc(pMTISDBTPdCtx->pDemodCtx);    
    mcSEMA_UNLOCK_MTFE(pMTISDBTPdCtx->hHalLock);

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBT_PARAMETER_T);
    COPY_TO_USER_ARG(prParameter, rParameter,MTFE_ISDBT_PARAMETER_T);

    return MTR_OK;
}

/** Get Signal power level in dBm.
*@param pi2SigLeveldBm      Signal power level in dBm.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ISDBT_GetSignalLeveldBm(unsigned long arg)
{
    MTFE_ISDBT_PARAMETER_T *prParameter;   // User para pointer
    MTFE_ISDBT_PARAMETER_T rParameter;       // Kernal para pointer
    prParameter = (MTFE_ISDBT_PARAMETER_T*)arg;

    if(_fgISDBTInit!=TRUE)
    {  // Tuner Open Fail
        MTFE_ISDBT_PRINT("_MTFE_ISDBT_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBT_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_ISDBT_PARAMETER_T);
  
    // Select Demod OFDM1 or OFDM2 
    switch(rParameter.rTunerID)
    {	 
    case MTFE_TUNER_OFDM1:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM1;
        break;
    case MTFE_TUNER_OFDM2:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM2;
        break;
    default:
        pMTISDBTPdCtx->pDemodCtx->rTunerId = TUNER_ISDBT_OFDM1;
        break;  
    }

    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTISDBTPdCtx->hHalLock);
    ISDBT_DemodAddr(pMTISDBTPdCtx->pDemodCtx);
    rParameter.u4SigLevel= ISDBT_GetSignalLevel(pMTISDBTPdCtx->pDemodCtx);
    mcSEMA_UNLOCK_MTFE(pMTISDBTPdCtx->hHalLock);

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBT_PARAMETER_T);
    COPY_TO_USER_ARG(prParameter, rParameter,MTFE_ISDBT_PARAMETER_T);

    return MTR_OK;
}

/** Check demod software version.
*@param  pszVersion         Software Version.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ISDBT_GetVer(unsigned long arg)
{
    UCHAR *pszVersion;
    UCHAR  aszVersion[] = "1.000"; /* Software Version: x.yyy */
    pszVersion = (UINT8*)arg;   
   
    USR_SPACE_ACCESS_VALIDATE_ARG(pszVersion,aszVersion);
    COPY_TO_USER_ARG(pszVersion,aszVersion,aszVersion);
    MTFE_ISDBT_PRINT(" - ISDBT Ver: %s\n", aszVersion);

    return MTR_OK;
}

/**Register demod callback function.
*@param  CbId               Callback function ID.
*@param  pfCb               Callback function.
*@param  vArg               The argument of callback function.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_ISDBT_RegCb(unsigned long arg)
{
    return MTR_OK;
}

/** For NTSC to get digital/analog exclusive semaphore.
*@param  *sema              pointer to a sema
*@retval void
*/
void Get_MTFE_ISDBT_Sema_Ex_DiAna(HANDLE_T *sema)
{
    *sema = &Exclusive_DigiAnaSema;
}

int mtal_ioctl_mtfe_isdbt(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
    MT_RESULT_T eRet = MTR_OK;
    MTFE_ISDBT_PRINT("-------isdbt ioctl cmd-------\r\n");
	
    if(u1ExclSemaInit==1)
    {
        mcSEMA_LOCK_MTFE_S(Exclusive_DigiAnaSema,"Ex_DiAna");
    }
	
    switch (cmd)
    {
    case MTAL_IO_ISDBT_OPEN:
        eRet=_MTFE_ISDBT_Open();
        break;
    case MTAL_IO_ISDBT_CLOSE:
        eRet=_MTFE_ISDBT_Close();
        break;
    case MTAL_IO_ISDBT_LNA:
        eRet=_MTFE_ISDBT_SetLNA(arg);
        break;
    case MTAL_IO_ISDBT_TSF:
        eRet=_MTFE_ISDBT_SetFreq(arg);
        break;
    case MTAL_IO_ISDBT_CONNECT:
        eRet=_MTFE_ISDBT_Connect(arg);
        break;   
    case MTAL_IO_ISDBT_DISCONNECT:
        eRet=_MTFE_ISDBT_DisConnect();
        break;
    case MTAL_IO_ISDBT_GETSYNC:
        eRet=_MTFE_ISDBT_GetSync(arg);
        break;
    case MTAL_IO_ISDBT_GETSIGLEVEL:
        eRet=_MTFE_ISDBT_GetSignalLeveldBm( arg);
        break;
    case MTAL_IO_ISDBT_GETPER:
        eRet=_MTFE_ISDBT_GetPER( arg);
        break;
    case MTAL_IO_ISDBT_GETCNR:
        eRet=_MTFE_ISDBT_GetCNR( arg);
        break;
    case MTAL_IO_ISDBT_GETIFAGC:
        eRet=_MTFE_ISDBT_GetIfAgc( arg);
        break;
    case MTAL_IO_ISDBT_GETBER:
        eRet=_MTFE_ISDBT_GetBER(arg);
        break;
    case MTAL_IO_ISDBT_GETVER:
        eRet=_MTFE_ISDBT_GetVer( arg);
        break;
    case MTAL_IO_ISDBT_REGCB:
        eRet=_MTFE_ISDBT_RegCb( arg);
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

