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
 * $RCSfile: io_mtfe_isdbs.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file io_mtfe_isdbs.c
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
#include "io_mtfe_isdbs.h"
#include "pd_tuner_type.h"    
#include "pi_isdbs_demod_if.h"     // pi_demod
#include "pd_isdbs_demod_if.h"    // pd_glue

#include "cb_low.h"  // For callback function
#include <linux/mtal_ioctl.h>

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(int)))                            \
    {                                                       \
        printk("mtal_ioctl_isdbs argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(int)))                        \
    {                                                       \
        printk("mtal_ioctl_isdbs argument error\n");      \
        return -1;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_isdbs argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_isdbs argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_isdbs argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_isdbs argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        printk("mtal_ioctl_isdbs argument error\n");     \
        return MTR_ERR_INV;                                          \
    }

#define COPY_TO_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_isdbs argument error\n");      \
        return MTR_ERR_INV;                                          \
    }
#define COPY_FROM_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_from_user(&ArgKernel,(void __user *)ArgUsr,    \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_isdbs argument error\n");      \
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
#define MTAL_ISDBS_MON_POLL_INTRVL             50  // 50ms: Basic resoultion for timers
#define MTAL_ISDBS_MON_INFO_UPDATE_INTRVL    1000  // 1sec
#define cMTAL_ISDBS_CHK_STATUS_INTRVL         200  // 200ms

#define MTAL_ISDBS_MON_INFO_UPDATE_CNT       (MTAL_ISDBS_MON_INFO_UPDATE_INTRVL/MTAL_ISDBS_MON_POLL_INTRVL)
#define cMTAL_ISDBS_CHK_STATUS_CNT           (cMTAL_ISDBS_CHK_STATUS_INTRVL/MTAL_ISDBS_MON_POLL_INTRVL)


#define cUNLOCK_CNT_THRD                     10    // The criteria for state transition from lock to unlock
#define cWAIT_ISDBS_DEMOD_LOCK_CNT            20    //wait 20*50ms

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
extern void TunerSetMwIntf(UINT8 u1Value);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static MTAL_ISDBS_PD_CTX_T     *pMTISDBSPdCtx;

static BOOL _fgISDBSInit=FALSE;
/*all variable used in this file should put into pMTISDBSPdCtx 
except for Exclusive_DigiAnaSema and u1ExclSemaInit*/
static HANDLE_T    Exclusive_DigiAnaSema=0;
static UINT8       u1ExclSemaInit=0;

static MTFE_ISDBS_PARAMETER_T    curParam;

extern MT_RESULT_T _MTFE_NTSC_DisConnect(VOID);
//-----------------------------------------------------------------------------
// Static Functions
//-----------------------------------------------------------------------------
static void _Notify_ISDBS_EWSStatus(UINT8 EwsStatus)
{
MTAL_FE_JAPAN_TUNER_CALLBACK_INFO_T rCbInfo;
  
    MTFE_ISDBS_PRINT("%s: EwsStatus = %d\n", __FUNCTION__, EwsStatus);
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
static void _Notify_ISDBS_DemodStatus(INT16 LockStatus)
{
MTAL_FE_JAPAN_TUNER_CALLBACK_INFO_T rCbInfo;

    MTFE_ISDBS_PRINT("%s: DTD LockStatus? %d\n", __FUNCTION__, LockStatus);
    rCbInfo.CbId = MTAL_FE_ISDB_LOCK_STATUS;

    if (LockStatus < MTFE_DTV_UNKNOWN)
    {
        rCbInfo.CbIdInfo.LockStatus = LockStatus;
    }      
    else // default
    {
        rCbInfo.CbIdInfo.LockStatus = MTFE_DTV_UNKNOWN; 
    }          	

    MTFE_ISDBS_PRINT("JAPAN _CB_PutEventLockStatus: LockStatus? %d\n", rCbInfo.CbIdInfo.LockStatus);
    _CB_PutEvent(CB_MTAL_FE_JAPAN_TUNER_TRIGGER, sizeof(MTAL_FE_JAPAN_TUNER_CALLBACK_INFO_T), &rCbInfo);
}

/**ISDB-S monitor process.
*@param  pvArg               
*@retval VOID
*/
static VOID _ISDBSMonitorProcess(VOID)
{
    INT16 	i2LockStatus;
    UINT16  u2_timer2 = 0;    
    UINT16                  u2MsgIdx;
    TUNER_ISDBS_CMD_QUEUE_T	    rCmdQ;
    SIZE_T 		    zMsgSize = sizeof(UINT32);
    INT32 			    i4Result;
    UINT16                  u2ISDBSLockTimeCnt = 0;
    UINT16                  u2UockCnt = 0;
    EWS_INFO             ewsSts;//EWS Status
    UINT8                    u1PreEwsSts=FALSE;
    UINT8                    u1WaitDemodCnt=cWAIT_ISDBS_DEMOD_LOCK_CNT;//wait 1000ms
	
    while (pMTISDBSPdCtx->fgMonitorThreadRunning)
    {    
        i4Result = x_msg_q_receive_timeout(&u2MsgIdx, &rCmdQ, &zMsgSize, &pMTISDBSPdCtx->t_MoniCmdMsgQ, 1, MTAL_ISDBS_MON_POLL_INTRVL);//50ms timeout
        if( i4Result==OSR_OK && rCmdQ.CmdId == TUNER_ISDBS_CMD_CONNECT)
	 {
            MTFE_ISDBS_PRINT("ISDBS Monitor Get Cmd TUNER_ISDBS_CMD_CONNECT (MsgQ=%ld)\n",i4Result);
            if(u2ISDBSLockTimeCnt!=0)
            { //last connect event not yet finish, and new connect event come
                _Notify_ISDBS_DemodStatus(MTFE_DTV_BREAK_BY_SECOND_CONNECT_EVENT);
                u2ISDBSLockTimeCnt = 0;//reset u2ISDBSLockTimeCnt for next new connect event
            }
            pMTISDBSPdCtx->fgAskMoniDoGetSync = TRUE;//keep do getSync till finish callback event
            pMTISDBSPdCtx->eState = MTFE_ISDBS_CONNECTING_STATE; // Change to connecting state
            u1WaitDemodCnt=cWAIT_ISDBS_DEMOD_LOCK_CNT;
        }

        if (pMTISDBSPdCtx->fgBreakMonitor) //condition for suspend monitor
        {
            pMTISDBSPdCtx->eState = MTFE_ISDBS_IDLE_STATE; // Change to IDLE state        	
            MTFE_ISDBS_PRINT("Entering io_mtfe_ISDBS mon_suspend\n");
            mcSEMA_LOCK_MTFE_S(pMTISDBSPdCtx->t_escape_mon_suspend,"ISDBS_mon_suspend");
            MTFE_ISDBS_PRINT("Escape io_mtfe_ISDBS mon_suspend\n");
        }
        else if(pMTISDBSPdCtx->eState == MTFE_ISDBS_CONNECTING_STATE)
        { 
            mcSEMA_LOCK_MTFE(pMTISDBSPdCtx->hHalLock); //when call PI primitive API must use this lock for protection
            u2ISDBSLockTimeCnt++;  
            u1WaitDemodCnt--;	           
            i2LockStatus = ISDBS_GetSync(pMTISDBSPdCtx->pDemodCtx);
            
            if(i2LockStatus == TRUE)//lock
            {
                MTFE_ISDBS_PRINT("ISDBS  lock found!\n");
                _Notify_ISDBS_DemodStatus(MTFE_DTV_LOCK);
                pMTISDBSPdCtx->fgAskMoniDoGetSync =FALSE; //don't need to do get sync in next loop
                u2ISDBSLockTimeCnt = 0;//reset u2ISDBSLockTimeCnt for next new connect event
                pMTISDBSPdCtx->eState = MTFE_ISDBS_LOCK_STATE;
            }
 
            mcSEMA_UNLOCK_MTFE(pMTISDBSPdCtx->hHalLock);
            if(u1WaitDemodCnt==0)
            {
                MTFE_ISDBS_PRINT("demod can not lock in %d ms!\n",cWAIT_ISDBS_DEMOD_LOCK_CNT*MTAL_ISDBS_MON_POLL_INTRVL);
                _Notify_ISDBS_DemodStatus(MTFE_DTV_UNLOCK);
                pMTISDBSPdCtx->fgAskMoniDoGetSync =FALSE; //don't need to do get sync in next loop
                u2ISDBSLockTimeCnt = 0;
                pMTISDBSPdCtx->eState = MTFE_ISDBS_UNLOCK_STATE;
            }          
        }
        else //implement get info or any other regular update job here, delay job is done by x_msg_q_receive_timeout[MTAL_ISDBS_MON_POLL_INTRVL]
        { /*monitor PI Monitor process*/
            /* Increase timer*/
            u2_timer2++;
            mcSEMA_LOCK_MTFE(pMTISDBSPdCtx->hHalLock); //when call PI primitive API must use this lock for protection
			 
            ewsSts.val = ISDBS_GetEws(pMTISDBSPdCtx->pDemodCtx);
            if(ewsSts.val!=u1PreEwsSts)//EWS Status change
            {
                ewsSts.isNotify=TRUE;
            }
            else
            {
                ewsSts.isNotify=FALSE;
            }
            u1PreEwsSts=ewsSts.val;
			
            mcSEMA_UNLOCK_MTFE(pMTISDBSPdCtx->hHalLock);        
			 
            if(ewsSts.isNotify)//EWS Status change
            {
                _Notify_ISDBS_EWSStatus(ewsSts.val);
            }			
			 
          	/* Detect if there is any status transition */
            if (u2_timer2 > cMTAL_ISDBS_CHK_STATUS_CNT)//4 
            {
                u2_timer2 = 0;
                mcSEMA_LOCK_MTFE(pMTISDBSPdCtx->hHalLock); //when call PI primitive API must use this lock for protection
                i2LockStatus =  ISDBS_GetSync(pMTISDBSPdCtx->pDemodCtx);
                MTFE_ISDBS_PRINT("Polling i2LockStatus = %d\n", i2LockStatus);              
                if (pMTISDBSPdCtx->eState == MTFE_ISDBS_UNLOCK_STATE)
                {
                    if(i2LockStatus == TRUE) // LOCK
                    {
                        _Notify_ISDBS_DemodStatus(MTFE_DTV_LOCK);
                        pMTISDBSPdCtx->eState = MTFE_ISDBS_LOCK_STATE;
                        MTFE_ISDBS_PRINT("Change state from unlock to lock\n");
                    }
                }
                else // LOCK_STATE
                {             	
                    if (i2LockStatus == FALSE)
                		u2UockCnt++;
                    else
                		u2UockCnt = 0;
                	
                    MTFE_ISDBS_PRINT("u2UockCnt = %d\n", u2UockCnt);
                    if (u2UockCnt >= cUNLOCK_CNT_THRD)//10 times
                    {
                        u2UockCnt = 0;                		
                        _Notify_ISDBS_DemodStatus(MTFE_DTV_UNLOCK);
                        pMTISDBSPdCtx->eState = MTFE_ISDBS_UNLOCK_STATE;
                        MTFE_ISDBS_PRINT("Change state from lock to unlock\n");
                    }
                }
                mcSEMA_UNLOCK_MTFE(pMTISDBSPdCtx->hHalLock);                				
            }
			
        }
        //don't do delay here, x_msg_q_receive() will do blocking delay
    }

    pMTISDBSPdCtx->t_monitor_thread = NULL;
    MTFE_ISDBS_PRINT("MTAL MonitorProcess exit!\n");
    x_thread_exit();
}

/**Start ISDBS monitor engine.
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
    pMTISDBSPdCtx->fgMonitorThreadRunning = TRUE;
    if (x_thread_create(&pMTISDBSPdCtx->t_monitor_thread,
            "_IsdbsMonThread",
            2048, 101,
            _ISDBSMonitorProcess, sizeof(void*), NULL) != OSR_OK)
    {
        MTFE_ISDBS_PRINT("_StartMonitorEngine (mcTHREAD_CREATE): ERROR!\n");
        return (MTR_NOT_OK);
    }
    MTFE_ISDBS_PRINT("_StartMonitorEngine success\n");
    return (MTR_OK);
}

/**Stop ISDBS monitor engine.
*@param  VOID               
*@retval MTR_OK              Success.
*@retval MTR_NOT_OK          Fail.
*/
static MT_RESULT_T _StopMonitorEngine(VOID)
{
    pMTISDBSPdCtx->fgMonitorThreadRunning = FALSE;
    MTFE_ISDBS_PRINT("In kernal:_MTAL_StopMonitorEngine\n");

    while (pMTISDBSPdCtx->t_monitor_thread) 
    {
        mcDELAY_MS(10); // in ms
    }
    MTFE_ISDBS_PRINT("MTAL PD_StopMonitorEngine success\n");

    return (MTR_OK);
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
/** Initialize front end ISDBS demod module.
*@param VOID
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_ISDBS_Open(VOID)
{
    INT32           i4Status;
   // ISDBS_PD_CTX_T    *pISDBSPdCtx;
    TUNER_ISDBS_CMD_QUEUE_T	rCmdQ;

    MTFE_ISDBS_PRINT("In kernel %s\n", __FUNCTION__);
   
    //add driver implementation code here
    if(_fgISDBSInit==TRUE)
    {
        MTFE_ISDBS_PRINT("_fgISDBSInit==TRUE, do _MTFE_ISDBS_Close() first\r\n");
        return (MTR_NOT_OK);
    }
    // TunerOpen
    TunerSetMwIntf(cMTAL_IF);//Set MW type to avoid glue layer to create monitor thread again
    i4Status = TunerOpen();    
    if (i4Status < DRVAPI_TUNER_OK)
    {   
        MTFE_ISDBS_PRINT("TunerOpen failed\n");
        return (MTR_NOT_OK);
    }
    else if (i4Status > DRVAPI_TUNER_OK)
    {
        MTFE_ISDBS_PRINT("TunerOpen has been called\n");        
    }
    else
    {
        MTFE_ISDBS_PRINT("TunerOpen ok\n");        
    }

    // Create MTAL pd glue Contex
    pMTISDBSPdCtx = (MTAL_ISDBS_PD_CTX_T *) x_mem_calloc(1, sizeof(MTAL_ISDBS_PD_CTX_T));
    if (pMTISDBSPdCtx == NULL)
    {
        MTFE_ISDBS_PRINT("MTAL_ISDBS_PD_CTX_T malloc fail\n");
        return (MTR_NOT_OK);
    }

    //get ISDBS pd Ctx from nucleus pd glue
    /*
    pISDBSPdCtx = (ISDBS_PD_CTX_T*)TunerGetCtx(DIGITAL_FE);
    if(pISDBSPdCtx==NULL)
    {
        MTFE_ISDBS_PRINT("TunerGetCtx  fail\n");
        return (MTR_NOT_OK);
    }
    pMTISDBSPdCtx->hHalLock = pISDBSPdCtx->hHalLock; 
    */

    //Create PI demod ctx
    pMTISDBSPdCtx->pDemodCtx = (ISDBS_DEMOD_CTX_T *)psISDBS_DemodCtx();
    if (pMTISDBSPdCtx->pDemodCtx == NULL)
    {
        MTFE_ISDBS_PRINT("ISDBS Tuner Open, pid Demod malloc fail\n");
        return (MTR_NOT_OK);
    } 

    //Create a semaphore for low level demod PI access protection
    if (x_sema_create(&pMTISDBSPdCtx->hHalLock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        MTFE_ISDBS_PRINT("ISDBS x_sema_create(pMTISDBSPdCtx->hHalLock) Fail!\n");
        return (MTR_NOT_OK);
    }
    x_sema_unlock(pMTISDBSPdCtx->hHalLock);
	
    //
    if (x_sema_create(&Exclusive_DigiAnaSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        MTFE_ISDBS_PRINT("ISDBS x_sema_create(Exclusive_DigiAnaSema) Fail!\n");
        return (MTR_NOT_OK);
    }
    x_sema_unlock(Exclusive_DigiAnaSema);
    u1ExclSemaInit = 1;

    if (x_sema_create(&pMTISDBSPdCtx->t_escape_mon_suspend, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        MTFE_ISDBS_PRINT("ISDBS x_sema_create(pMTISDBSPdCtx->t_escape_mon_suspend) Fail!\n");
        return (MTR_NOT_OK);
    }
    x_sema_unlock(pMTISDBSPdCtx->t_escape_mon_suspend);

    if ((x_msg_q_create(&pMTISDBSPdCtx->t_MoniCmdMsgQ, "ISDBS_Moni_MsgQ", sizeof(rCmdQ), TUNER_ISDBS_CMD_QUEUE_SIZE)) != OSR_OK)
    {
        MTFE_ISDBS_PRINT("ISDBS  x_msg_q_create(pMTISDBSPdCtx->t_escape_mon_suspend) Fail!\n");
        return (MTR_NOT_OK);
    }	

    pMTISDBSPdCtx->fgAskMoniDoGetSync = FALSE; //init value
    pMTISDBSPdCtx->fgBreakMonitor = TRUE; //initial monitor is suspend

    // If tuner open success, start monitor engine
    if(MTR_OK != _StartMonitorEngine())
    {
        MTFE_ISDBS_PRINT("ISDBS _StartMonitorEngine failed\n");
    	return (MTR_NOT_OK);
    }

    _fgISDBSInit = TRUE;

    return MTR_OK;
}

/** Terminate front end ISDBS demod module.
*@param VOID
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ISDBS_Close(VOID)
{

    MTFE_ISDBS_PRINT("In kernel %s\n", __FUNCTION__);
    
    //add driver implementation code here
    if(_fgISDBSInit!=TRUE){
        MTFE_ISDBS_PRINT("_MTFE_ISDBS_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }
    pMTISDBSPdCtx->fgBreakMonitor = TRUE;

    /* If tuner close success, stop monitor engine */
    if(MTR_OK != _StopMonitorEngine())
    {
        MTFE_ISDBS_PRINT("ISDBS _StopMonitorEngine failed\n");
        return (MTR_NOT_OK);    	
    }

    x_sema_unlock(Exclusive_DigiAnaSema);
    x_sema_delete(Exclusive_DigiAnaSema);

    x_sema_unlock(pMTISDBSPdCtx->t_escape_mon_suspend);
    x_sema_delete(pMTISDBSPdCtx->t_escape_mon_suspend);

    x_msg_q_delete(pMTISDBSPdCtx->t_MoniCmdMsgQ);

    //add driver implementation code here
    bISDBS_DemodCtxDestroy(); // free the pi_demodctx
    x_mem_free(pMTISDBSPdCtx); //free MTAL glue Ctx

    _fgISDBSInit = FALSE;

    return MTR_OK;
}

/** Start demod module to acquire to specified frequency and modulation type.
*@param  rParameter         Acquisition parameter (RF freq , modulation type,..).
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_ISDBS_SetTSID(unsigned long arg)
{
    MTFE_ISDBS_PARAMETER_T *prParameter;   // User para pointer
    MTFE_ISDBS_PARAMETER_T rParameter;       // Kernal para pointer
    prParameter = (MTFE_ISDBS_PARAMETER_T*)arg;
    UINT8 u1Flag;
	
    u1Flag = 0;

    if(_fgISDBSInit!=TRUE)
    {  // Tuner Open Fail
        MTFE_ISDBS_PRINT("_MTFE_ISDBS_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBS_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_ISDBS_PARAMETER_T);
  
    // Select Demod PSK1 or PSK2 
    switch(rParameter.rTunerID)
    {	 
    case MTFE_TUNER_PSK1:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK1;
        break;
    case MTFE_TUNER_PSK2:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK2;
        break;
    default:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK1;
        break;  
    }

    mcSEMA_LOCK_MTFE(pMTISDBSPdCtx->hHalLock);
	
    ISDBS_DemodAddr(pMTISDBSPdCtx->pDemodCtx);
    // Set the Demod TSID
    pMTISDBSPdCtx->pDemodCtx->u2TSID= rParameter.u2TsID;
    u1Flag |= ISDBS_SetTSID(pMTISDBSPdCtx->pDemodCtx);
	
    mcSEMA_UNLOCK_MTFE(pMTISDBSPdCtx->hHalLock);
	
    if (u1Flag)
    {
        printf("ISDBS_SetTSID fail: Freq =  %ld\n", pMTISDBSPdCtx->pDemodCtx->Freqency);
	 return MTR_NOT_OK;
    }
    
    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBS_PARAMETER_T);
    COPY_TO_USER_ARG(prParameter, rParameter,MTFE_ISDBS_PARAMETER_T);

    return MTR_OK;
}

/** Start ISDB-S demod module to set the Tuner LNB Status.
*@param  prPara        Parameter (LNB Status, ... ).
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_ISDBS_SetLNB(unsigned long arg)
{
    MTFE_ISDBS_PARAMETER_T *prParameter;   // User para pointer
    MTFE_ISDBS_PARAMETER_T rParameter;       // Kernal para pointer
    prParameter = (MTFE_ISDBS_PARAMETER_T*)arg;
    UINT8 u1Flag;
	
    u1Flag = 0;

    if(_fgISDBSInit!=TRUE)
    {  // Tuner Open Fail
        MTFE_ISDBS_PRINT("_MTFE_ISDBS_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBS_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_ISDBS_PARAMETER_T);
  
    // Select Demod PSK1 or PSK2 
    switch(rParameter.rTunerID)
    {	 
    case MTFE_TUNER_PSK1:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK1;
        break;
    case MTFE_TUNER_PSK2:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK2;
        break;
    default:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK1;
        break;  
    }

    // Set the ISDB-S Tuner LNB status
    if(rParameter.rLNBSts == MTFE_LNB_VOLT_11V)
    {
        pMTISDBSPdCtx->pDemodCtx->rLNBStatus = ISDBS_LNB_VOLT_11V;
    }
    else if(rParameter.rLNBSts == MTFE_LNB_VOLT_15V)
    {
        pMTISDBSPdCtx->pDemodCtx->rLNBStatus = ISDBS_LNB_VOLT_15V;
    }
    else
    {
        pMTISDBSPdCtx->pDemodCtx->rLNBStatus = ISDBS_LNB_OFF;
    }

    mcSEMA_LOCK_MTFE(pMTISDBSPdCtx->hHalLock);	
    ISDBS_DemodAddr(pMTISDBSPdCtx->pDemodCtx);
    // Set the ISDB-S Tuner LNB
    u1Flag |= ISDBS_SetTunerLNB(pMTISDBSPdCtx->pDemodCtx);	
    mcSEMA_UNLOCK_MTFE(pMTISDBSPdCtx->hHalLock);
	
    if (u1Flag)
    {
        printf("ISDBS Set Tuner LNB fail: LNBStatus =  %d\n", pMTISDBSPdCtx->pDemodCtx->rLNBStatus);
	 return MTR_NOT_OK;
    } 

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBS_PARAMETER_T);
    COPY_TO_USER_ARG(prParameter, rParameter,MTFE_ISDBS_PARAMETER_T);

    return MTR_OK;
}

/** Start demod module to acquire to specified frequency and modulation type.
*@param  rParameter         Acquisition parameter (RF freq , modulation type,..).
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ISDBS_Connect(unsigned long arg)
{
    MTFE_ISDBS_PARAMETER_T *prParameter;   // User para pointer
    MTFE_ISDBS_PARAMETER_T rParameter;       // Kernal para pointer
    prParameter = (MTFE_ISDBS_PARAMETER_T*)arg;
    UINT8 ii, u1Flag = 0;
    TUNER_ISDBS_CMD_QUEUE_T	rCmdQ;
    MT_RESULT_T fgRet = MTR_OK;

    _MTFE_NTSC_DisConnect();

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBS_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_ISDBS_PARAMETER_T);

    MTFE_ISDBS_PRINT("In kernel %s - rParameter.u4Freq = %ld\n"
        "rParameter.rModulation = %d\n"
        "rParameter.rRfMode = %d\n"
        "rParameter.rSurfMode = %d\n",
        __FUNCTION__,
        rParameter.u4Freq,
        rParameter.rModulation,
        rParameter.rRfMode,
        rParameter.rSurfMode);

    memcpy(&curParam, &rParameter, sizeof(MTFE_ISDBS_PARAMETER_T));

    pMTISDBSPdCtx->fgBreakMonitor = FALSE; //resume monitor to do getSync
    mcSEMA_UNLOCK_MTFE_S(pMTISDBSPdCtx->t_escape_mon_suspend,"isdbs_mon_suspend");

    mcSEMA_LOCK_MTFE(pMTISDBSPdCtx->hHalLock);
    //add driver implementation code here    
     
    //ask monitor to do getSync
    /*preapre for send event to monitor thread*/
    rCmdQ.CmdId = TUNER_ISDBS_CMD_CONNECT;
    rCmdQ.CmdInfo.ConnectParam.u4Freq =rParameter.u4Freq;
    rCmdQ.CmdInfo.ConnectParam.rModulation = rParameter.rModulation;
    rCmdQ.CmdInfo.ConnectParam.rRfMode =rParameter.rRfMode;
    rCmdQ.CmdInfo.ConnectParam.rSurfMode = rParameter.rSurfMode;

    /* Connect */
     // Select Demod PSK1 or PSK2 
    switch(rParameter.rTunerID)
    {   
    case MTFE_TUNER_PSK1:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK1;
        break;
    case MTFE_TUNER_PSK2:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK2;
        break;
    default:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK1;
        break;  
   }
    // Select Demod TS Output System B or C
    switch(rParameter.rTsIO)
    {    
    case MTFE_TS_SERIAL_IO_B:
        pMTISDBSPdCtx->pDemodCtx->rTsIO= ISDBS_TS_SERIAL_IO_B;
        break;
    case MTFE_TS_SERIAL_IO_C:
        pMTISDBSPdCtx->pDemodCtx->rTsIO= ISDBS_TS_SERIAL_IO_C;
        break;
    default:
        pMTISDBSPdCtx->pDemodCtx->rTsIO = ISDBS_TS_SERIAL_IO_B;
        break;  
    }
    pMTISDBSPdCtx->pDemodCtx->Freqency = rParameter.u4Freq;
	
    // Connect the Demod
    u1Flag |= ISDBS_Connect(pMTISDBSPdCtx->pDemodCtx);
    // Set the Tuner Frequency
    u1Flag |= ISDBS_SetFreq(pMTISDBSPdCtx->pDemodCtx);

    if (u1Flag)
    {
        MTFE_ISDBS_PRINT("Tuner SetFreq Fail\n");
	 fgRet = MTR_NOT_OK;
    }
    else
    {
        printf("TunerSetFreq Successs\n"); 
    }

    ISDBS_ScanInit(pMTISDBSPdCtx->pDemodCtx);

    // Get the sync
    for(ii=0; ii < 100; ii++)
    {
        if(ISDBS_GetSync(pMTISDBSPdCtx->pDemodCtx))
        {   // Signal Sync
            MTFE_ISDBS_PRINT("Demod GetSync, Lock at %d times\n", ii);
            rParameter.rLockStatus = MTFE_DTV_LOCK;
            break;
        }
        mcDELAY_MS(10);
    }
    if(ii == 100)
    {   // Get Sync time over 90, break
        MTFE_ISDBS_PRINT("Demod unsync\n");
        rParameter.rLockStatus = MTFE_DTV_UNLOCK;
      //  fgRet = MTR_NOT_OK;
    }

    if ((x_msg_q_send(pMTISDBSPdCtx->t_MoniCmdMsgQ, &rCmdQ, sizeof(TUNER_ISDBS_CMD_QUEUE_T), 1)) != OSR_OK)
    {
        MTFE_ISDBS_PRINT("Error: _MTFE_ISDBS_Connect() x_msg_q_send failed\n");
        fgRet = MTR_NOT_OK;
    }
    mcSEMA_UNLOCK_MTFE(pMTISDBSPdCtx->hHalLock);
	
    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBS_PARAMETER_T);
    COPY_TO_USER_ARG(prParameter, rParameter,MTFE_ISDBS_PARAMETER_T);

    return fgRet;
}

/**Stop acquisition and put Demod into sleep mode.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

MT_RESULT_T _MTFE_ISDBS_DisConnect(VOID)
{
    MTFE_ISDBS_PRINT("In kernel %s\n", __FUNCTION__);
    if(_fgISDBSInit!=TRUE)
    {
        return MTR_OK;
    }
    //add driver implementation code here    
    pMTISDBSPdCtx->fgBreakMonitor = TRUE; //suspend monitor

    /* Diconnect */
    mcSEMA_LOCK_MTFE(pMTISDBSPdCtx->hHalLock);
    ISDBS_DemodAddr(pMTISDBSPdCtx->pDemodCtx);  
    ISDBS_Sleep(pMTISDBSPdCtx->pDemodCtx);
    mcSEMA_UNLOCK_MTFE(pMTISDBSPdCtx->hHalLock);
    
    return MTR_OK;
}

/** Start demod module to acquire to specified frequency.
*@param  rParameter         Acquisition parameter (RF freq).
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ISDBS_SetFreq(unsigned long arg)
{
    MTFE_ISDBS_PARAMETER_T *prParameter;   // User para pointer
    MTFE_ISDBS_PARAMETER_T rParameter;       // Kernal para pointer
    prParameter = (MTFE_ISDBS_PARAMETER_T*)arg;
    UINT8  u1Flag;
    MT_RESULT_T fgRet;

    if(_fgISDBSInit!=TRUE)
    {  // Tuner Open Fail
        MTFE_ISDBS_PRINT("_MTFE_ISDBS_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }
	
    fgRet = MTR_OK;
    u1Flag = 0;

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBS_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_ISDBS_PARAMETER_T);

    MTFE_ISDBS_PRINT("In kernel %s - rParameter.u4Freq = %ld\n", __FUNCTION__, rParameter.u4Freq);

    switch(rParameter.rTunerID)
    {	 
    case MTFE_TUNER_PSK1:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK1;
        break;
    case MTFE_TUNER_PSK2:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK2;
        break;
    default:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK1;
        break;  
    }

    mcSEMA_LOCK_MTFE(pMTISDBSPdCtx->hHalLock);   

    ISDBS_DemodAddr(pMTISDBSPdCtx->pDemodCtx);     
    // Set the Tuner Frequency
    pMTISDBSPdCtx->pDemodCtx->Freqency = rParameter.u4Freq;
    u1Flag |= ISDBS_SetFreq(pMTISDBSPdCtx->pDemodCtx);
	
    mcSEMA_UNLOCK_MTFE(pMTISDBSPdCtx->hHalLock);
	
    if (u1Flag)
    {
        MTFE_ISDBS_PRINT("TunerSetFreq fail\n");  
	 fgRet = MTR_NOT_OK;
    }

    return fgRet;
}

/**Get packet error rate.
*@param pu4TotalPkt         Received total packets.
*@param pu4ErrPkt           Received error packets.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ISDBS_GetPER(unsigned long arg)
{
    MTFE_ISDBS_PARAMETER_T *prParameter;   // User para pointer
    MTFE_ISDBS_PARAMETER_T rParameter;       // Kernal para pointer
    prParameter = (MTFE_ISDBS_PARAMETER_T*)arg;

    if(_fgISDBSInit!=TRUE)
    {  // Tuner Open Fail
        MTFE_ISDBS_PRINT("_MTFE_ISDBS_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBS_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_ISDBS_PARAMETER_T);
  
    // Select Demod PSK1 or PSK2 
    switch(rParameter.rTunerID)
    {	 
    case MTFE_TUNER_PSK1:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK1;
        break;
    case MTFE_TUNER_PSK2:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK2;
        break;
    default:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK1;
        break;  
    }

    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTISDBSPdCtx->hHalLock);
    ISDBS_DemodAddr(pMTISDBSPdCtx->pDemodCtx); 
    rParameter.u4PER = ISDBS_GetBer(pMTISDBSPdCtx->pDemodCtx, TRUE);
    mcSEMA_UNLOCK_MTFE(pMTISDBSPdCtx->hHalLock);

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBS_PARAMETER_T);
    COPY_TO_USER_ARG(prParameter, rParameter,MTFE_ISDBS_PARAMETER_T);

    return MTR_OK;
}

/**Get BER.
*@param  pu4TotalBit        Received total bits.
*@param  pu4ErrBit          Received error bits.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_ISDBS_GetBER(unsigned long arg)
{
    MTFE_ISDBS_PARAMETER_T *prParameter;   // User para pointer
    MTFE_ISDBS_PARAMETER_T rParameter;       // Kernal para pointer
    prParameter = (MTFE_ISDBS_PARAMETER_T*)arg;

    if(_fgISDBSInit!=TRUE)
    {  // Tuner Open Fail
        MTFE_ISDBS_PRINT("_MTFE_ISDBS_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBS_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_ISDBS_PARAMETER_T);
  
    // Select Demod PSK1 or PSK2 
    switch(rParameter.rTunerID)
    {	 
    case MTFE_TUNER_PSK1:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK1;
        break;
    case MTFE_TUNER_PSK2:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK2;
        break;
    default:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK1;
        break;  
    }

    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTISDBSPdCtx->hHalLock);
    ISDBS_DemodAddr(pMTISDBSPdCtx->pDemodCtx); 
    rParameter.u4BER= ISDBS_GetBer(pMTISDBSPdCtx->pDemodCtx, FALSE);
    mcSEMA_UNLOCK_MTFE(pMTISDBSPdCtx->hHalLock);

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBS_PARAMETER_T);
    COPY_TO_USER_ARG(prParameter, rParameter,MTFE_ISDBS_PARAMETER_T);

    return MTR_OK;
}

/**Get SNR in dB.
*@param u2SignalSNR         Signal SNR
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ISDBS_GetCNR(unsigned long arg)
{
    MTFE_ISDBS_PARAMETER_T *prParameter;   // User para pointer
    MTFE_ISDBS_PARAMETER_T rParameter;       // Kernal para pointer
    prParameter = (MTFE_ISDBS_PARAMETER_T*)arg;

    if(_fgISDBSInit!=TRUE)
    {  // Tuner Open Fail
        MTFE_ISDBS_PRINT("_MTFE_ISDBS_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBS_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_ISDBS_PARAMETER_T);
  
    // Select Demod PSK1 or PSK2 
    switch(rParameter.rTunerID)
    {	 
    case MTFE_TUNER_PSK1:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK1;
        break;
    case MTFE_TUNER_PSK2:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK2;
        break;
    default:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK1;
        break;  
    }

    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTISDBSPdCtx->hHalLock);
    ISDBS_DemodAddr(pMTISDBSPdCtx->pDemodCtx); 
    rParameter.u4CNR = ISDBS_GetCNR(pMTISDBSPdCtx->pDemodCtx);
    mcSEMA_UNLOCK_MTFE(pMTISDBSPdCtx->hHalLock);

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBS_PARAMETER_T);
    COPY_TO_USER_ARG(prParameter, rParameter,MTFE_ISDBS_PARAMETER_T);

    return MTR_OK;
}

/** Get demod lock status.
*@param	 prStatus           Report demod status.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_ISDBS_GetSync(unsigned long arg)
{
    MTFE_ISDBS_PARAMETER_T *prParameter;   // User para pointer
    MTFE_ISDBS_PARAMETER_T rParameter;       // Kernal para pointer
    prParameter = (MTFE_ISDBS_PARAMETER_T*)arg;

    if(_fgISDBSInit!=TRUE)
    {  // Tuner Open Fail
        MTFE_ISDBS_PRINT("_MTFE_ISDBS_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBS_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_ISDBS_PARAMETER_T);
  
    // Select Demod PSK1 or PSK2 
    switch(rParameter.rTunerID)
    {	 
    case MTFE_TUNER_PSK1:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK1;
        break;
    case MTFE_TUNER_PSK2:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK2;
        break;
    default:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK1;
        break;  
    }

    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTISDBSPdCtx->hHalLock);
    ISDBS_DemodAddr(pMTISDBSPdCtx->pDemodCtx); 
    if(!ISDBS_GetSync(pMTISDBSPdCtx->pDemodCtx))
    {   // Demod UnLock
        rParameter.rLockStatus = MTFE_DTV_UNLOCK;
    }
    else
    {
        rParameter.rLockStatus = MTFE_DTV_LOCK;
    }
    mcSEMA_UNLOCK_MTFE(pMTISDBSPdCtx->hHalLock);

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBS_PARAMETER_T);
    COPY_TO_USER_ARG(prParameter, rParameter,MTFE_ISDBS_PARAMETER_T);

    return MTR_OK;
}

/**Get IF AGC Voltage in mV.
*@param  pu2IfAgcVol        IF AGC voltage in mV.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ISDBS_GetIfAgc(unsigned long arg)
{
    MTFE_ISDBS_PARAMETER_T *prParameter;   // User para pointer
    MTFE_ISDBS_PARAMETER_T rParameter;       // Kernal para pointer
    prParameter = (MTFE_ISDBS_PARAMETER_T*)arg;

    if(_fgISDBSInit!=TRUE)
    {  // Tuner Open Fail
        MTFE_ISDBS_PRINT("_MTFE_ISDBS_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBS_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_ISDBS_PARAMETER_T);
  
    // Select Demod PSK1 or PSK2 
    switch(rParameter.rTunerID)
    {	 
    case MTFE_TUNER_PSK1:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK1;
        break;
    case MTFE_TUNER_PSK2:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK2;
        break;
    default:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK1;
        break;  
    }

    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTISDBSPdCtx->hHalLock);
    ISDBS_DemodAddr(pMTISDBSPdCtx->pDemodCtx); 
    rParameter.u1IFAGC= ISDBS_GetAgc(pMTISDBSPdCtx->pDemodCtx); 
    mcSEMA_UNLOCK_MTFE(pMTISDBSPdCtx->hHalLock);

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBS_PARAMETER_T);
    COPY_TO_USER_ARG(prParameter, rParameter,MTFE_ISDBS_PARAMETER_T);

    return MTR_OK;
}

/** Get Signal power level in dBm.
*@param pi2SigLeveldBm      Signal power level in dBm.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ISDBS_GetSignalLeveldBm(unsigned long arg)
{
    MTFE_ISDBS_PARAMETER_T *prParameter;   // User para pointer
    MTFE_ISDBS_PARAMETER_T rParameter;       // Kernal para pointer
    prParameter = (MTFE_ISDBS_PARAMETER_T*)arg;

    if(_fgISDBSInit!=TRUE)
    {  // Tuner Open Fail
        MTFE_ISDBS_PRINT("_MTFE_ISDBS_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBS_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_ISDBS_PARAMETER_T);
  
    // Select Demod PSK1 or PSK2 
    switch(rParameter.rTunerID)
    {	 
    case MTFE_TUNER_PSK1:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK1;
        break;
    case MTFE_TUNER_PSK2:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK2;
        break;
    default:
        pMTISDBSPdCtx->pDemodCtx->rTunerId = TUNER_ISDBS_PSK1;
        break;  
    }

    //add driver implementation code here
    mcSEMA_LOCK_MTFE(pMTISDBSPdCtx->hHalLock);
    ISDBS_DemodAddr(pMTISDBSPdCtx->pDemodCtx); 
    rParameter.u4SigLevel= ISDBS_GetSignalLevel(pMTISDBSPdCtx->pDemodCtx);  
    mcSEMA_UNLOCK_MTFE(pMTISDBSPdCtx->hHalLock);

    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_ISDBS_PARAMETER_T);
    COPY_TO_USER_ARG(prParameter, rParameter,MTFE_ISDBS_PARAMETER_T);

    return MTR_OK;
}

/** Check demod software version.
*@param  pszVersion         Software Version.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_ISDBS_GetVer(unsigned long arg)
{
    UCHAR *pszVersion;
    UCHAR  aszVersion[] = "1.000"; /* Software Version: x.yyy */
    pszVersion = (UINT8*)arg;   
   
    USR_SPACE_ACCESS_VALIDATE_ARG(pszVersion,aszVersion);
    COPY_TO_USER_ARG(pszVersion,aszVersion,aszVersion);
    MTFE_ISDBS_PRINT(" - ISDBS Ver: %s\n", aszVersion);

    return MTR_OK;
}

/**Register demod callback function.
*@param  CbId               Callback function ID.
*@param  pfCb               Callback function.
*@param  vArg               The argument of callback function.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_ISDBS_RegCb(unsigned long arg)
{
    return MTR_OK;
}

/** For NTSC to get digital/analog exclusive semaphore.
*@param  *sema              pointer to a sema
*@retval void
*/
void Get_MTFE_ISDBS_Sema_Ex_DiAna(HANDLE_T *sema)
{
    *sema = &Exclusive_DigiAnaSema;
}

int mtal_ioctl_mtfe_isdbs(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
    MT_RESULT_T eRet = MTR_OK;
    MTFE_ISDBS_PRINT("-------isdbs ioctl cmd-------\r\n");
	
    if(u1ExclSemaInit==1)
    {
        mcSEMA_LOCK_MTFE_S(Exclusive_DigiAnaSema,"Ex_DiAna");
    }
	
    switch (cmd)
    {
    case MTAL_IO_ISDBS_OPEN:
        eRet=_MTFE_ISDBS_Open();
        break;
    case MTAL_IO_ISDBS_CLOSE:
        eRet=_MTFE_ISDBS_Close();
        break;
    case MTAL_IO_ISDBS_TSF:
        eRet=_MTFE_ISDBS_SetFreq(arg);
        break;
    case MTAL_IO_ISDBS_TSID:
        eRet=_MTFE_ISDBS_SetTSID(arg);
        break;
    case MTAL_IO_ISDBS_LNB:
        eRet=_MTFE_ISDBS_SetLNB(arg);
        break;
    case MTAL_IO_ISDBS_CONNECT:
        eRet=_MTFE_ISDBS_Connect(arg);
        break;
    case MTAL_IO_ISDBS_DISCONNECT:
        eRet=_MTFE_ISDBS_DisConnect();
        break;
    case MTAL_IO_ISDBS_GETSYNC:
        eRet=_MTFE_ISDBS_GetSync(arg);
        break;
    case MTAL_IO_ISDBS_GETSIGLEVEL:
        eRet=_MTFE_ISDBS_GetSignalLeveldBm( arg);
        break;
    case MTAL_IO_ISDBS_GETPER:
        eRet=_MTFE_ISDBS_GetPER( arg);
        break;
    case MTAL_IO_ISDBS_GETCNR:
        eRet=_MTFE_ISDBS_GetCNR( arg);
        break;
    case MTAL_IO_ISDBS_GETIFAGC:
        eRet=_MTFE_ISDBS_GetIfAgc( arg);
        break;
    case MTAL_IO_ISDBS_GETBER:
        eRet=_MTFE_ISDBS_GetBER(arg);
        break;
    case MTAL_IO_ISDBS_GETVER:
        eRet=_MTFE_ISDBS_GetVer( arg);
        break;
    case MTAL_IO_ISDBS_REGCB:
        eRet=_MTFE_ISDBS_RegCb( arg);
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

