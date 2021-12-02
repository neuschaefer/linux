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
 * $RCSfile: io_mtfe_dvbt.c,v $
 * $Revision: #12 $
 *---------------------------------------------------------------------------*/

/** @file io_mtfe_dvbt.c
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mtfe_dvbt.h"
#include "tuner_if.h"
#include <linux/mtal_ioctl.h>
#include "io_mtfe_pd_def.h"
#include "io_mtfe_dvbt.h"
#include "pd_dvbt_if.h"
#include "cb_data.h"
#include "x_tuner.h"

//-----------------------------------------------------------------------------
// debug switch
//-----------------------------------------------------------------------------
//#ifdef DEBUG_DVBT_PERFORMANCE

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define MTFE_DVBT_SW_VERSION         "1.001"  /* Software Version: x.yyy */

#define MTAL_DVBT_MON_POLL_INTRVL             50  // 50ms
#define MTAL_DVBT_MON_INFO_UPDATE_INTRVL    1000  // 1sec
#define MTAL_DVBT_MON_INFO_UPDATE_CNT       (MTAL_DVBT_MON_INFO_UPDATE_INTRVL/MTAL_DVBT_MON_POLL_INTRVL)
#define MTAL_DVBT_LOCK_TIME_OUT              500  // 500ms
#define MTAL_DVBT_LOCK_TIME_OUT_CNT         (MTAL_DVBT_LOCK_TIME_OUT/MTAL_DVBT_MON_POLL_INTRVL)

#define MTAL_DVBT_DEMOD_I2C_ADDR            0x82

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(int)))                            \
    {                                                       \
        printk("mtal_ioctl_dvbt argument 1 error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(int)))                        \
    {                                                       \
        printk("mtal_ioctl_dvbt argument 2 error\n");      \
        return -1;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_dvbt argument 3 error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_dvbt argument 4 error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_dvbt argument 5 error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_dvbt argument 6 error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        printk("mtal_ioctl_dvbt argument 7 error\n");     \
        return MTR_ERR_INV;                                          \
    }

#define COPY_TO_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_dvbt argument 8 error\n");      \
        return MTR_ERR_INV;                                          \
    }
#define COPY_FROM_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_from_user(&ArgKernel,(void __user *)ArgUsr,    \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_dvbt argument 9 error\n");      \
        return MTR_ERR_INV;                                          \
    }

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
//static TD_DTD_CTX_T                *pDTDPdCtx;
//static DEMOD_CTX_T                 *psDTDPiCtx;
//static HANDLE_T						t_dvbt_lock;
//static BOOL             			_fgMonitorThreadRunning;
//static UINT32         				t_monitor_thread;

static MTAL_DVBT_PD_CTX_T  *pMTPdCtx;
static BOOL _fgDVBTInit=FALSE;
/*all variable used in this file should put into pMTPdCtx exclusive of Exclusive_DigiAnaSema and u1ExclSemaInit*/
static HANDLE_T    Exclusive_DigiAnaSema=0;
static UINT8       u1ExclSemaInit=0;

//static MT_RESULT_T _MTFE_DVBT_GetTSFMT( unsigned long arg);

extern MT_RESULT_T _MTFE_PAL_DisConnect(VOID);
extern INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);
extern UINT8 DVBT_GetSync(DEMOD_CTX_T *psDemodCtx);
extern UINT32 os_get_sys_tick(void);
extern 	UINT32 DVBT_GetSQI(DEMOD_CTX_T *psDemodCtx);
extern BOOL DVBT_FwSwitch(DEMOD_CTX_T *psDemodCtx);
extern 	UINT8 DVBT_SetNormalMode(DEMOD_CTX_T *psDemodCtx);
extern  INT32 DVBT_Recover(DEMOD_CTX_T *psDemodCtx);
extern 	UINT8 DVBT_SetArCoef(DEMOD_CTX_T *psDemodCtx, UINT8 ucData);
extern 	VOID DVBT_DisConnect(DEMOD_CTX_T *psDemodCtx);
extern 
	void DVBT_SetTsOutputMode(DEMOD_CTX_T *psDemodCtx, BOOL fgParallel);
extern TS_FMT_T DVBT_GetTsFmt(DEMOD_CTX_T *psDemodCtx);
extern 	void PCMCIA_UpdateTsStatus(TS_FMT_T TsFmt);


extern UINT8 DVBT_SetTsPathMode(DEMOD_CTX_T *psDemodCtx, BOOL fgEnable);

//-----------------------------------------------------------------------------
// Static Functions
//-----------------------------------------------------------------------------
static void _CB_PutEventLockStatus(MTFE_DTV_STATUS_T LockStatus)
{
    MTAL_FE_EU_TUNER_CALLBACK_INFO_T 	rCbInfo;

    rCbInfo.CbId = MTAL_FE_DVBT_LOCK_STATUS;

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
    MTFE_NTSC_PRINT("DVBT _CB_PutEventLockStatus: LockStatus? %d\n", rCbInfo.CbIdInfo.LockStatus);
    _CB_PutEvent(CB_MTAL_FE_EU_TUNER_TRIGGER, sizeof(MTAL_FE_EU_TUNER_CALLBACK_INFO_T), &rCbInfo);

}

//Justin todo: what need to do in monitor? please reference dtd code, only do getsync? and call notify user space?
// Jackson test callback event successful.
/**DVBT monitor process.
*@param  pvArg               
*@retval VOID
*/
static VOID _MonitorProcess(VOID)
{
    UINT16  u2_i = 0;
    UINT16  u2DvbtLockTimeCnt = 0;
    UINT8 	u1DvbtDemodState;
	UINT8     ucData=0;

   // MTAL_FE_EU_TUNER_CALLBACK_INFO_T 	rCbInfo;
    UINT16                  u2MsgIdx;
	TUNER_CMD_QUEUE_T	    rCmdQ;
    SIZE_T 					zMsgSize = sizeof(UINT32);
    INT32 					i4Result;
    UINT32                  u4TickCnt = 0;
#ifdef DEBUG_DVBT_PERFORMANCE
    UINT32                  u4TickCntIntenal;
    UINT32                  u4TickCntTest1;
    UINT32                  u4TickCntTest2;
#endif

    while (pMTPdCtx->fgMonitorThreadRunning)
    {
        //todo: ask ben&Ray timeout value accuracy has problem( MSGQ_DELAY_TIMEOUT )
        i4Result = x_msg_q_receive_timeout(&u2MsgIdx, &rCmdQ, &zMsgSize, &pMTPdCtx->t_MoniCmdMsgQ, 1, MTAL_DVBT_MON_POLL_INTRVL);
        // Justin 081216, too many log, mark this print command.
        // MTFE_DVBT_PRINT("DVBT Monitor Get MsgQ=%d\n",i4Result);
        
        if( i4Result==OSR_OK && rCmdQ.CmdId == TUNER_CMD_CONNECT){
            u4TickCnt = rCmdQ.u4TickCnt; //remember connect event start tick
#ifdef DEBUG_DVBT_PERFORMANCE
            u4TickCntIntenal = os_get_sys_tick();
            MTFE_DVBT_PRINT(" - DVBT_messageq receive+context switch cost %d ms\n",(u4TickCntIntenal-u4TickCnt)*x_os_drv_get_tick_period());
#endif
            if(u2DvbtLockTimeCnt!=0){ //last connect event not yet finish, and new connect event come
                _CB_PutEventLockStatus(MTFE_DTV_BREAK_BY_SECOND_CONNECT_EVENT);
                u2DvbtLockTimeCnt = 0;//reset u2DvbtLockTimeCnt for next new connect event
            }
            pMTPdCtx->fgAskMoniDoGetSync = TRUE;//keep do getSync till finish callback event
        }

        if (pMTPdCtx->fgBreakMonitor) //condition for suspend monitor
        {
            MTFE_DVBT_PRINT("Entering io_mtfe_dvbt mon_suspend\n");
            mcSEMA_LOCK_MTFE_S(pMTPdCtx->t_escape_mon_suspend,"dvbt_mon_suspend");
    		MTFE_DVBT_PRINT("Escape io_mtfe_dvbt mon_suspend\n");
        }
        else if(pMTPdCtx->fgAskMoniDoGetSync){//condition for doing getSync, implement your get sync here

            mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when call PI primitive API must use this lock for protection
            u2DvbtLockTimeCnt++;

#ifdef DEBUG_DVBT_PERFORMANCE
            u4TickCntTest1 = os_get_sys_tick();  
            u1DvbtDemodState = DVBT_GetSync(pMTPdCtx->pDemodCtx, rCmdQ.CmdInfo.ConnectParam.eHierarchy);
            u4TickCntTest2 = os_get_sys_tick();
            MTFE_DVBT_PRINT(" - vDVBT_GetSync cost %d ms\n",(u4TickCntTest2-u4TickCntTest1)*x_os_drv_get_tick_period());
#else
            u1DvbtDemodState = DVBT_GetSync(pMTPdCtx->pDemodCtx);//, rCmdQ.CmdInfo.ConnectParam.eHierarchy);
#endif
            if ((u1DvbtDemodState == DVBT_DEMOD_STATE_UNLOCK) ||
                (u2DvbtLockTimeCnt > MTAL_DVBT_LOCK_TIME_OUT_CNT))
            {
                pMTPdCtx->fgAskMoniDoGetSync =FALSE; //don't need to do get sync in next loop
                u4TickCnt = os_get_sys_tick()- u4TickCnt;
                MTFE_DVBT_PRINT("In kernel: Notify DVBT unlock! time = %d ms, x_os_drv_get_tick_period()=%d\n", u4TickCnt*x_os_drv_get_tick_period(),x_os_drv_get_tick_period());
                _CB_PutEventLockStatus(MTFE_DTV_UNLOCK);
                u2DvbtLockTimeCnt = 0;//reset u2DvbtLockTimeCnt for next new connect event
            }
            else if (u1DvbtDemodState == DVBT_DEMOD_STATE_LOCK) 
            {
                pMTPdCtx->fgAskMoniDoGetSync =FALSE; //don't need to do get sync in next loop
              
                DVBT_UpdateInfo(pMTPdCtx->pDemodCtx); 
                pMTPdCtx->u4SQI_Pi = DVBT_GetSQI(pMTPdCtx->pDemodCtx);    //Iven, 20110427 for DDI
                pMTPdCtx->u4SQI_L = ((pMTPdCtx->u4SQI_L*8 - pMTPdCtx->u4SQI_L) + pMTPdCtx->u4SQI_Pi)/8;
                
                u4TickCnt = os_get_sys_tick()- u4TickCnt;
                MTFE_DVBT_PRINT("In kernel: Notify DVBT lock! lock time = %d ms, x_os_drv_get_tick_period()=%d, u2DvbtLockTimeCnt=%d\n", u4TickCnt*x_os_drv_get_tick_period(),x_os_drv_get_tick_period(),u2DvbtLockTimeCnt);

                _CB_PutEventLockStatus(MTFE_DTV_LOCK);
                u2DvbtLockTimeCnt = 0;//reset u2DvbtLockTimeCnt for next new connect event
            }
            else{/*unknown state, do nothing*/
                //also do getSync in next loop
            }
//            if(u2DvbtLockTimeCnt > MTAL_DVBT_LOCK_TIME_OUT_CNT){
//                pMTPdCtx->fgAskMoniDoGetSync =FALSE; //don't need to do get sync in next loop
//                u2DvbtLockTimeCnt = 0;
//            }
            mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
        }
        else //implement get info or any other regular update job here, delay job is done by x_msg_q_receive_timeout[MTAL_DVBT_MON_POLL_INTRVL]
		{ /*monitor PI Monitor process*/
			u2_i++;
            // Justin 081216, too many log, mark this print command.
        	//MTFE_DVBT_PRINT("MTAL DVBT MonitorProcess! keep update parameter u2_i=%d\n",u2_i);			
			if (u2_i > MTAL_DVBT_MON_INFO_UPDATE_CNT) 
			{
            	u2_i = 0;
				mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when call PI primitive API must use this lock for protection
				DVBT_UpdateInfo(pMTPdCtx->pDemodCtx);
				//lei120727 added for ESD make demod reset issue,recover the demod status.
				#if 1
					DVBT_GetReg(pMTPdCtx->pDemodCtx, 0x00, &ucData, 1);
					if(ucData != 0x5D)
					{
						MTFE_DVBT_PRINT(("Up 0x00!=0x5D,Reconnect the demod"));
						if (DVBT_Recover(pMTPdCtx->pDemodCtx))
						{
		        			MTFE_DVBT_PRINT(("DVBT_Recover failed\n"));
		    			}
						pMTPdCtx->fgAskMoniDoGetSync =TRUE;
					}
				#endif
                pMTPdCtx->u4SQI_Pi = DVBT_GetSQI(pMTPdCtx->pDemodCtx);    //Iven, 20110427 for DDI
                pMTPdCtx->u4SQI_L = ((pMTPdCtx->u4SQI_L*8 - pMTPdCtx->u4SQI_L) + pMTPdCtx->u4SQI_Pi)/8;                
            	
                // Justin 081216, too many log, mark this print command.
            	// MTFE_DVBT_PRINT("MTAL DVBT Info Update!\n");		
				mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
			}
			
        }
        //don't do delay here, x_msg_q_receive() will do blocking
        //if you wanna delay monitor , modify this value: MSGQ_DELAY_TIMEOUT
    }
    
    pMTPdCtx->t_monitor_thread = 0;
    MTFE_DVBT_PRINT("MTAL MonitorProcess exit!\n");
    x_thread_exit();

}

/**Start DVBT monitor engine.
*@param  VOID               
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _StartMonitorEngine(VOID)
{  
    pMTPdCtx->fgMonitorThreadRunning = TRUE;
    if (x_thread_create(&pMTPdCtx->t_monitor_thread,
            "_DvbtMonThread",
            2048, 101,
            (x_os_thread_main_fct)_MonitorProcess, sizeof(void*), NULL) != OSR_OK)
    {
        MTFE_DVBT_PRINT("_StartMonitorEngine (mcTHREAD_CREATE): ERROR!\n");
        return (MTR_NOT_OK);
    }

    return (MTR_OK);
}

/**Stop DVBT monitor engine.
*@param  VOID               
*@retval MTR_OK        MTR_NOT_OK      Success.
*@retval MTR_NOT_OK          Fail.
*/
static MT_RESULT_T _StopMonitorEngine(VOID)
{
    pMTPdCtx->fgMonitorThreadRunning = FALSE;
    MTFE_DVBT_PRINT("In kernal:_MTAL_StopMonitorEngine\n");

    while (pMTPdCtx->t_monitor_thread){
        mcDELAY_MS(10); // in ms
    }
    MTFE_DVBT_PRINT("MTAL PD_StopMonitorEngine success\n");

    return (MTR_OK);
}

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------------------

#if defined(CC_MT5363) || defined(CC_MT5365)|| defined(CC_MT5395) || defined( CC_MT5396)||defined(CC_MT5389)
static INT8 Tuner_Connect(UINT32 Freq, UINT8 sawbw, UINT8 AutoSearch)
{
    PARAM_SETFREQ_T SetFreqPara;	
    INT8 fgRet = 0;

    ITuner_OP(ITunerGetCtx(), itSetSawBw, sawbw, NULL);
   
    SetFreqPara.Freq = Freq;
    SetFreqPara.Modulation = MOD_DVBT;
    SetFreqPara.fgAutoSearch = AutoSearch; 
   // printf("Tuner Connect\n");	
    if((ITuner_SetFreq(ITunerGetCtx(), &SetFreqPara)) != 0) {
        printf("Failed on Tuner_SetFreq(%d)", Freq);
        fgRet = 1;
    }

    return fgRet;
}
#endif

/** Initialize front end DVB-T demod module.
*@param VOID
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_DVBT_Open(VOID)
{
#ifdef DEBUG_DVBT_PERFORMANCE
    UINT32 u4TickCnt = os_get_sys_tick();
#endif

	INT32 i4Status;
	MT_RESULT_T rRet;
    DVBT_PD_CTX_T       *pDVBTPdCtx;
    TUNER_CMD_QUEUE_T	rCmdQ;
	//u4MtalLogMask |= MTFE_DVBT_LOG_MASK;

    MTFE_DVBT_PRINT("In kernel %s\n", __FUNCTION__);
    //add driver implementation code here
    if(_fgDVBTInit==TRUE){
        MTFE_DVBT_PRINT("_fgDVBTInit==TRUE, do _MTFE_DVBT_Close() first\r\n");
    //still return MTR_OK because it has been opened successfully already!		
        return (MTR_OK);
    }
    /* TunerOpen */
	TunerSetMwIntf(cMTAL_IF);
    i4Status = TunerOpen();
    if (i4Status < DRVAPI_TUNER_OK)
    {
        u1IfATVOnlyStatus+=1;
	    MTFE_DVBT_PRINT("DVBT  open status %d\n",u1IfATVOnlyStatus);
        MTFE_DVBT_PRINT("TunerOpen failed. %d\n",i4Status);
        return (MTR_NOT_OK);
    }
    else if (i4Status > DRVAPI_TUNER_OK)
    {
        MTFE_DVBT_PRINT("TunerOpen has been called. %d\n",i4Status);        
    }
    else{
        MTFE_DVBT_PRINT("TunerOpen ok\n");        
    }

    pMTPdCtx = (MTAL_DVBT_PD_CTX_T *) x_mem_calloc(1, sizeof(MTAL_DVBT_PD_CTX_T));
    if (pMTPdCtx == NULL)
    {
        MTFE_DVBT_PRINT("MTAL_DVBT_PD_CTX_T malloc fail\n");
        return (MTR_NOT_OK);
    }

    //get dvbt pd Ctx from nucleus pd glue
    /*pDVBTPdCtx = (DVBT_PD_CTX_T*)TunerGetCtx(DIGITAL_FE);
    pMTPdCtx->hHalLock = pDVBTPdCtx->hHalLock; 
    pMTPdCtx->pDemodCtx =(DEMOD_CTX_T*) pDVBTPdCtx->pDemodCtx;*/
    /* If tuner open success, start monitor engine */
    //mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock);
    //mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);

    pDVBTPdCtx = (DVBT_PD_CTX_T*)DVBT_GetPdCtx();
    if (pDVBTPdCtx == NULL)
    {
        MTFE_DVBT_PRINT("DVBT_PD_CTX_T get fail\n");
        return (MTR_NOT_OK);
    }
    pMTPdCtx->hHalLock = pDVBTPdCtx->hHalLock; 
    pMTPdCtx->pDemodCtx =(DEMOD_CTX_T*) pDVBTPdCtx->pDemodCtx;

    if (x_sema_create(&Exclusive_DigiAnaSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        MTFE_DVBT_PRINT(("%s: DVBT x_sema_create(Exclusive_DigiAnaSema) Fail!\n"));
        return (MTR_NOT_OK);
    }
    x_sema_unlock(Exclusive_DigiAnaSema);
    u1ExclSemaInit = 1;

    if (x_sema_create(&pMTPdCtx->t_escape_mon_suspend, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        MTFE_DVBT_PRINT(("%s: DVBT x_sema_create(pMTPdCtx->t_escape_mon_suspend) Fail!\n"));
        return (MTR_NOT_OK);
    }
    x_sema_unlock(pMTPdCtx->t_escape_mon_suspend);

    if ((x_msg_q_create(&pMTPdCtx->t_MoniCmdMsgQ, "DVBT_Moni_MsgQ", sizeof(rCmdQ), TUNER_CMD_QUEUE_SIZE)) != OSR_OK)
    {
        MTFE_DVBT_PRINT(("%s: DVBT  x_msg_q_create(pMTPdCtx->t_escape_mon_suspend) Fail!\n"));
        return (MTR_NOT_OK);
    }
    pMTPdCtx->fgAskMoniDoGetSync = FALSE; //init value
//#if 0
    pMTPdCtx->fgBreakMonitor = TRUE; //initial monitor is suspend

    rRet = _StartMonitorEngine();
    if(rRet != MTR_OK)
    {
        MTFE_DVBT_PRINT("_StartMonitorEngine failed\n");
    	return (MTR_NOT_OK);
    }
//#endif

	_fgDVBTInit = TRUE;

#ifdef DEBUG_DVBT_PERFORMANCE
    u4TickCnt = os_get_sys_tick() - u4TickCnt;
    MTFE_DVBT_PRINT(" - function execution time = %d ms\n",u4TickCnt*x_os_drv_get_tick_period());
#endif

    return MTR_OK;
}

/** Terminate front end DVB-T demod module.
*@param VOID
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_DVBT_Close(VOID)
{
#ifdef DEBUG_DVBT_PERFORMANCE
    UINT32 u4TickCnt = os_get_sys_tick();
#endif

    MTFE_DVBT_PRINT(" - void\n");

     if(_fgDVBTInit!=TRUE){
        MTFE_DVBT_PRINT("_MTFE_DVBT_Open() didn't run\r\n");
        return (MTR_NOT_OK);
    }
 	if(pMTPdCtx->fgBreakMonitor == TRUE){
    	pMTPdCtx->fgBreakMonitor = FALSE; //resume monitor 
		mcSEMA_UNLOCK_MTFE_S(pMTPdCtx->t_escape_mon_suspend,"dvbt_mon_suspend");
	}

    _StopMonitorEngine();

    x_sema_unlock(Exclusive_DigiAnaSema);
    x_sema_delete(Exclusive_DigiAnaSema);

    x_sema_unlock(pMTPdCtx->t_escape_mon_suspend);
    x_sema_delete(pMTPdCtx->t_escape_mon_suspend);

    x_msg_q_delete(pMTPdCtx->t_MoniCmdMsgQ);
    //add driver implementation code here    
    x_mem_free(pMTPdCtx); //free MTAL glue Ctx
    //TunerClose(); //don't do LTDIS TunerClose() from MTAL

    _fgDVBTInit = FALSE;

#ifdef DEBUG_DVBT_PERFORMANCE
    u4TickCnt = os_get_sys_tick() - u4TickCnt;
    MTFE_DVBT_PRINT(" - function execution time = %d ms\n",u4TickCnt*x_os_drv_get_tick_period());
#endif

    return MTR_OK;
}
//Justin todo: Please reference DTD, PI need provide async connect function
/** Start demod module to acquire to specified frequency and bandwidth...
*@param  rParameter         Acquisition parameter (RF freq , bandwidth,..).
*@retval MTR_OK              Success.
*@retval MTR_NOT_OK          Fail.
*/

#if defined(CC_MT5363) || defined(CC_MT5365)|| defined(CC_MT5395)|| defined(CC_MT5396)||defined(CC_MT5389)
static MT_RESULT_T _MTFE_DVBT_Connect(unsigned long arg)//MTFE_DVBT_PARAMETER_T *prParameter)
{

    MTFE_DVBT_PARAMETER_T *prParameter;
    MTFE_DVBT_PARAMETER_T rParameter;
   // GEN_TUNER_CTX_T* pTunerCtx;
    MT_RESULT_T fgRet = MTR_OK;
    TUNER_CMD_QUEUE_T	rCmdQ;
	
#ifdef DEBUG_DVBT_PERFORMANCE
    UINT32              u4TickCnt1;
    UINT32              u4TickCnt2;
	#endif
	SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(pMTPdCtx->pDemodCtx->psTunerCtx->specific_member.eu_ctx);	
  

    prParameter = (MTFE_DVBT_PARAMETER_T*)arg;
     //mark for demo,lei110418
    _MTFE_PAL_DisConnect();  // iven, 20110504     
    //_MTFE_DVBC_DisConnect();	
    //add driver implementation code here
    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_DVBT_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_DVBT_PARAMETER_T);
    MTFE_DVBT_PRINT(" - rParameter.u4Freq = %d\n"
		"rParameter.u4IF = %d\n"
        "rParameter.eBandwidth = %d\n"
        "rParameter.eSurfMode = %d\n"
        "rParameter.eHierarchy = %d\n",
        rParameter.u4Freq,
        rParameter.u4IF,
        rParameter.eBandwidth,
        rParameter.eSurfMode,
        rParameter.eHierarchy);
    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when access PI Primitive API, must use this lock for protection
    
    rCmdQ.u4TickCnt = os_get_sys_tick();
#if 1 // need tuner to tune frequency!
	//ask pi to work
    if(rParameter.eSurfMode == MTFE_SURF_SCAN){
	      MTFE_DVBT_PRINT(" channel scan mode\n");
        fgRet |= Tuner_Connect( rParameter.u4Freq,rParameter.eBandwidth,  ATV_CONN_MODE_CHAN_SCAN);
    }
    else{ /*MTFE_SURF_CHANCHANGE*/
        fgRet |= Tuner_Connect( rParameter.u4Freq,rParameter.eBandwidth,ATV_CONN_MODE_CHAN_CHANGE);
    }
	if(fgRet != 0){
        goto BREAK_DVBT_CONNECT;
    }
#endif
#ifdef DEBUG_DVBT_PERFORMANCE
    u4TickCnt1 = os_get_sys_tick() - rCmdQ.u4TickCnt;
    MTFE_DVBT_PRINT(" - Tuner_Connect cost %d ms\n",u4TickCnt1*x_os_drv_get_tick_period());
    u4TickCnt1 = os_get_sys_tick();
#endif

#if 0 // change by liuqu,20101217
    fgRet |= DVBT_Connect(pMTPdCtx->pDemodCtx, rParameter.eBandwidth);
	if(fgRet != 0){
        goto BREAK_DVBT_CONNECT;
    }
#else
    /* Change the process. Hui @ 20100419 */
    // Chip init
	pEuTunerCtx->m_aucPara[0] = rParameter.m_aucPara[0];
	pEuTunerCtx->m_aucPara[1] =  rParameter.m_aucPara[1];
	
	pEuTunerCtx->m_aucPara[2] =  rParameter.m_aucPara[2];
	pEuTunerCtx->m_aucPara[3] =  rParameter.m_aucPara[3];
	pEuTunerCtx->m_aucPara[4] =  rParameter.m_aucPara[4];
	pEuTunerCtx->m_aucPara[5] =  rParameter.m_aucPara[5];
	
	pEuTunerCtx->m_aucPara[6] = rParameter.m_aucPara[6];
	pEuTunerCtx->m_aucPara[7] =  rParameter.m_aucPara[7];
	
	pEuTunerCtx->m_aucPara[ 8] =  rParameter.m_aucPara[8];
	pEuTunerCtx->m_aucPara[ 9] =  rParameter.m_aucPara[9];
	pEuTunerCtx->m_aucPara[10] =  rParameter.m_aucPara[10];
	pEuTunerCtx->m_aucPara[11] =  rParameter.m_aucPara[11];
	pEuTunerCtx->m_aucPara[12] =  rParameter.m_aucPara[12];
	pEuTunerCtx->m_aucPara[13] =  rParameter.m_aucPara[13];
	pEuTunerCtx->m_aucPara[14] =  rParameter.m_aucPara[14];  
#ifdef CC_EXTERNAL_TUNER	/*wenming 12-2-27: why need an external tuner???*/
	pMTPdCtx->pDemodCtx->psTunerCtx->u2IF_Freq = rParameter.u4IF;
#endif	
	DVBT_Initialize(pMTPdCtx->pDemodCtx, 0x82, TRUE);	
	//DVBT_ChipInit(pMTPdCtx->pDemodCtx);
	DVBT_FwSwitch(pMTPdCtx->pDemodCtx);

    // stop driver
    DVBT_DriverStop(pMTPdCtx->pDemodCtx);

    // set Parameter
    fgRet |= DVBT_SetNormalMode(pMTPdCtx->pDemodCtx);
	if(rParameter.eSurfMode == MTFE_SURF_SCAN)
	{
	 DVBT_SetArCoef(pMTPdCtx->pDemodCtx, 0x00);
	}
    else
	{
		DVBT_SetArCoef(pMTPdCtx->pDemodCtx, 0x3C);
    }
	
    DVBT_SetBW(pMTPdCtx->pDemodCtx, rParameter.eBandwidth);
	
	if(rParameter.eHierarchy==3)
    {
	   DVBT_SetHpLp(pMTPdCtx->pDemodCtx,0);//LP
    }
	else
	{
	   DVBT_SetHpLp(pMTPdCtx->pDemodCtx,1);//HP
	}

    // driver start
    DVBT_DriverStart(pMTPdCtx->pDemodCtx, 1, 2); // REACQ_AUTO, FAST_SYNC_AUTO

    if(fgRet != 0){
        goto BREAK_DVBT_CONNECT;
    }
#endif

#ifdef DEBUG_DVBT_PERFORMANCE
    u4TickCnt2 = os_get_sys_tick()- u4TickCnt1;
    MTFE_DVBT_PRINT(" - DVBT_Connect cost %d ms\n",u4TickCnt2*x_os_drv_get_tick_period());
    u4TickCnt1 = os_get_sys_tick();
#endif

    pMTPdCtx->fgBreakMonitor = FALSE; //resume monitor to do getSync
    mcSEMA_UNLOCK_MTFE_S(pMTPdCtx->t_escape_mon_suspend,"dvbt_mon_suspend");
	//ask monitor to do getSync

    rCmdQ.CmdId = TUNER_CMD_CONNECT;
    rCmdQ.CmdInfo.ConnectParam.u4Freq =rParameter.u4Freq;
    rCmdQ.CmdInfo.ConnectParam.eBandwidth = rParameter.eBandwidth;
    rCmdQ.CmdInfo.ConnectParam.eSurfMode =rParameter.eSurfMode;
    rCmdQ.CmdInfo.ConnectParam.eHierarchy = rParameter.eHierarchy;

#ifdef DEBUG_DVBT_PERFORMANCE
    u4TickCnt2 = os_get_sys_tick()- u4TickCnt1;
    MTFE_DVBT_PRINT(" - DVBT_unlock monitor cost %d ms\n",u4TickCnt2*x_os_drv_get_tick_period());
    rCmdQ.u4TickCnt = os_get_sys_tick();
#endif
    if ((x_msg_q_send(pMTPdCtx->t_MoniCmdMsgQ, &rCmdQ, sizeof(TUNER_CMD_QUEUE_T), 1)) != OSR_OK){
        MTFE_DVBT_PRINT("Error: x_msg_q_send failed\n");
    }
    
BREAK_DVBT_CONNECT:
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
	if(fgRet == 0){
		TS_FMT_T TsFmt;
	   // TunerGetTsFmt(&TsFmt);	
	   #if 0
	   _MTFE_DVBT_GetTSFMT(&TsFmt);
       #else
	   	{
			mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when access PI Primitive API, must use this lock for protection   
			TsFmt = DVBT_GetTsFmt(pMTPdCtx->pDemodCtx);	
			mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
	   	}
       #endif
	   
        /* Only Digital-Demod need ts-routing. */
        PCMCIA_UpdateTsStatus(TsFmt);
    	return MTR_OK;
	}
	else{
		MTFE_DVBT_PRINT("dvbt connect failed\n");
		return MTR_NOT_OK;
	}
}

#else

//Justin todo: Please reference DTD, PI need provide async connect function
/** Start demod module to acquire to specified frequency and bandwidth...
*@param  rParameter         Acquisition parameter (RF freq , bandwidth,..).
*@retval MTR_OK              Success.
*@retval MTR_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_DVBT_Connect(unsigned long arg)//MTFE_DVBT_PARAMETER_T *prParameter)
{

    MTFE_DVBT_PARAMETER_T *prParameter;
    MTFE_DVBT_PARAMETER_T rParameter;
    GEN_TUNER_CTX_T* pTunerCtx;
    MT_RESULT_T fgRet = MTR_OK;
    TUNER_CMD_QUEUE_T	rCmdQ;
    UINT32              u4TickCnt1;
    UINT32              u4TickCnt2;

    prParameter = (MTFE_DVBT_PARAMETER_T*)arg;

    _MTFE_PAL_DisConnect();
    //add driver implementation code here
	pTunerCtx = &(pMTPdCtx->pDemodCtx->sTunerCtx);
    USR_SPACE_ACCESS_VALIDATE_ARG(prParameter,MTFE_DVBT_PARAMETER_T);
    COPY_FROM_USER_ARG(prParameter, rParameter,MTFE_DVBT_PARAMETER_T);
    MTFE_DVBT_PRINT(" - rParameter.u4Freq = %d\n"
        "rParameter.eBandwidth = %d\n"
        "rParameter.eSurfMode = %d\n"
        "rParameter.eHierarchy = %d\n",
        rParameter.u4Freq,
        rParameter.eBandwidth,
        rParameter.eSurfMode,
        rParameter.eHierarchy);
    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when access PI Primitive API, must use this lock for protection
    
    rCmdQ.u4TickCnt = os_get_sys_tick();
	//ask pi to work
//#ifndef CC_EXTERNAL_TUNER	
    if(rParameter.eSurfMode == MTFE_SURF_SCAN){
        fgRet |= Tuner_Connect(pTunerCtx, rParameter.eBandwidth, rParameter.u4Freq, MODE_DVBT, ATV_CONN_MODE_CHAN_SCAN);
    }
    else{ /*MTFE_SURF_CHANCHANGE*/
        fgRet |= Tuner_Connect(pTunerCtx, rParameter.eBandwidth, rParameter.u4Freq, MODE_DVBT, ATV_CONN_MODE_CHAN_CHANGE);
    }
	if(fgRet != 0){
        goto BREAK_DVBT_CONNECT;
    }
//#endif
#ifdef DEBUG_DVBT_PERFORMANCE
    u4TickCnt1 = os_get_sys_tick() - rCmdQ.u4TickCnt;
    MTFE_DVBT_PRINT(" - Tuner_Connect cost %d ms\n",u4TickCnt1*x_os_drv_get_tick_period());
    u4TickCnt1 = os_get_sys_tick();
#endif

    fgRet |= DVBT_Connect(pMTPdCtx->pDemodCtx, rParameter.eBandwidth);
	if(fgRet != 0){
        goto BREAK_DVBT_CONNECT;
    }
#ifdef DEBUG_DVBT_PERFORMANCE
    u4TickCnt2 = os_get_sys_tick()- u4TickCnt1;
    MTFE_DVBT_PRINT(" - DVBT_Connect cost %d ms\n",u4TickCnt2*x_os_drv_get_tick_period());
    u4TickCnt1 = os_get_sys_tick();
#endif

    pMTPdCtx->fgBreakMonitor = FALSE; //resume monitor to do getSync
    mcSEMA_UNLOCK_MTFE_S(pMTPdCtx->t_escape_mon_suspend,"dvbt_mon_suspend");
	//ask monitor to do getSync

    rCmdQ.CmdId = TUNER_CMD_CONNECT;
    rCmdQ.CmdInfo.ConnectParam.u4Freq =rParameter.u4Freq;
    rCmdQ.CmdInfo.ConnectParam.eBandwidth = rParameter.eBandwidth;
    rCmdQ.CmdInfo.ConnectParam.eSurfMode =rParameter.eSurfMode;
    rCmdQ.CmdInfo.ConnectParam.eHierarchy = rParameter.eHierarchy;

#ifdef DEBUG_DVBT_PERFORMANCE
    u4TickCnt2 = os_get_sys_tick()- u4TickCnt1;
    MTFE_DVBT_PRINT(" - DVBT_unlock monitor cost %d ms\n",u4TickCnt2*x_os_drv_get_tick_period());
    rCmdQ.u4TickCnt = os_get_sys_tick();
#endif
    if ((x_msg_q_send(pMTPdCtx->t_MoniCmdMsgQ, &rCmdQ, sizeof(TUNER_CMD_QUEUE_T), 1)) != OSR_OK){
        MTFE_DVBT_PRINT("Error: x_msg_q_send failed\n");
    }
    
BREAK_DVBT_CONNECT:
    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
	if(fgRet == 0){
		TS_FMT_T TsFmt;
	   // TunerGetTsFmt(&TsFmt);	
       #if 0
	   _MTFE_DVBT_GetTSFMT(&TsFmt);
	   #else
	   	{
			mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when access PI Primitive API, must use this lock for protection   
			TsFmt = DVBT_GetTsFmt(pMTPdCtx->pDemodCtx);	
			mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
	   	}
	   #endif
        /* Only Digital-Demod need ts-routing. */
        PCMCIA_UpdateTsStatus(TsFmt);
    	return= MTR_OK;	
	}
	else{
		MTFE_DVBT_PRINT("dvbt connect failed\n");
		return= MTR_NOT_OK;
	}	
	return fgRet;
}

#endif

/**Stop acquisition and put Demod into sleep mode.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
//static MT_RESULT_T _MTFE_DVBT_DisConnect(VOID)
MT_RESULT_T _MTFE_DVBT_DisConnect(VOID)
{
#ifdef DEBUG_DVBT_PERFORMANCE
    UINT32 u4TickCnt = os_get_sys_tick();
#endif

    MTFE_DVBT_PRINT(" - void\n");
    if(_fgDVBTInit!=TRUE){
        return MTR_OK;
    }
    //add driver implementation code here
    pMTPdCtx->fgBreakMonitor = TRUE; //suspend monitor
    DVBT_DisConnect(pMTPdCtx->pDemodCtx);

#ifdef DEBUG_DVBT_PERFORMANCE
    u4TickCnt = os_get_sys_tick() - u4TickCnt;
    MTFE_DVBT_PRINT(" - function execution time = %d ms\n",u4TickCnt*x_os_drv_get_tick_period());
#endif

    return MTR_OK;
}

/** Get demod lock status.
*@param	 pi2LockStatus      Lock / unlock.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_DVBT_GetSync(unsigned long arg)
{
#ifdef DEBUG_DVBT_PERFORMANCE
    UINT32 u4TickCnt = os_get_sys_tick();
#endif

    INT16 *pi2LockStatus;
    INT16 i2LockStatus;
    pi2LockStatus = (INT16*)arg;

    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when access PI Primitive API, must use this lock for protection


    i2LockStatus = (INT16) DVBT_GetSync(pMTPdCtx->pDemodCtx);

    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
    //add driver implementation code here
    USR_SPACE_ACCESS_VALIDATE_ARG(pi2LockStatus,INT16);
    COPY_TO_USER_ARG(pi2LockStatus,i2LockStatus,INT16);
    MTFE_DVBT_PRINT(" - i2LockStatus = %d\n",i2LockStatus);

#ifdef DEBUG_DVBT_PERFORMANCE
    u4TickCnt = os_get_sys_tick() - u4TickCnt;
    MTFE_DVBT_PRINT(" - function execution time = %d ms\n",u4TickCnt*x_os_drv_get_tick_period());
#endif

    return MTR_OK;
}

/** Get Signal power level in percentage.
*@param pi2SigLevel         Signal power level in percentage.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_DVBT_GetSignalLevel(unsigned long arg)
{

    INT32 *pi2SigLevel;
    INT32 i2SigLevel;
    pi2SigLevel = (INT32*)arg;

    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when access PI Primitive API, must use this lock for protection
    //add driver implementation code here
    //vDVBTGetStatus(pMTPdCtx->pDemodCtx); // included in DVBT_UpdateInfo() in monitor function
    //i2SigLevel = DVBT_GetSignalLevel(pMTPdCtx->pDemodCtx);


        if ((ITuner_OP( ITunerGetCtx(), itGetSSIIndex,0, &i2SigLevel)) == ITUNER_OK)
        {
            MTFE_DVBT_PRINT("_MTFE_DVBT_GetSignalLevel (SSI): %d\n", i2SigLevel);
        }
        else
        {
           i2SigLevel = DVBT_GetSignalLevel(pMTPdCtx->pDemodCtx);
		   MTFE_DVBT_PRINT("_MTFE_DVBT_GetSignalLevel (IFAGC): %d\n", i2SigLevel);
        }
        
		

    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
    USR_SPACE_ACCESS_VALIDATE_ARG(pi2SigLevel,i2SigLevel);
    COPY_TO_USER_ARG(pi2SigLevel,i2SigLevel,i2SigLevel);
    MTFE_DVBT_PRINT(" - i2SigLevel = %d\n",i2SigLevel);
    return MTR_OK;

}

/**Get packet error rate.
*@param pu4TotalPkt         Received total packets in the last one second.
*@param pu4ErrPkt           Received error packets in the last one second.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_DVBT_GetPER(unsigned long arg)
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

    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when access PI Primitive API, must use this lock for protection
    //add driver implementation code here
    u4TotalPkt = DVBT_GetTotPkt(pMTPdCtx->pDemodCtx);
    u4ErrPkt   = DVBT_GetErrPkt(pMTPdCtx->pDemodCtx);

    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
    USR_SPACE_ACCESS_VALIDATE_ARG(pu4TotalPkt,u4TotalPkt);
    COPY_TO_USER_ARG(pu4TotalPkt,u4TotalPkt,u4TotalPkt);
    USR_SPACE_ACCESS_VALIDATE_ARG(pu4ErrPkt,u4ErrPkt);
    COPY_TO_USER_ARG(pu4ErrPkt,u4ErrPkt,u4ErrPkt);

    MTFE_DVBT_PRINT(" - u4TotalPkt = %d, u4ErrPkt = %d\n",
        u4TotalPkt,
        u4ErrPkt);
    return MTR_OK;
}

/**Get SNR in 0.1dB.
*@param u2SignalSNR         Signal SNR
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_DVBT_GetSNR(unsigned long arg)
{

    UINT16 *pu2SignalSNR;
    UINT16 u2SignalSNR = 0;
    pu2SignalSNR = (UINT16*)arg;

    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when access PI Primitive API, must use this lock for protection

    u2SignalSNR = (UINT16) DVBT_GetSignalSNR(pMTPdCtx->pDemodCtx);

    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
    //add driver implementation code here
    USR_SPACE_ACCESS_VALIDATE_ARG(pu2SignalSNR,u2SignalSNR);
    COPY_TO_USER_ARG(pu2SignalSNR,u2SignalSNR,u2SignalSNR);
    MTFE_DVBT_PRINT(" - u2SignalSNR = %d\n",u2SignalSNR);
    return MTR_OK;

}
/**Get SQI in %.
*@param u4SQI        SQI
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_DVBT_GetSQI(unsigned long arg)
{

    UINT32 *pu4SQI;
    UINT32 u4SQI = 0;
    pu4SQI = (UINT32*)arg;

    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when access PI Primitive API, must use this lock for protection

    u4SQI = (UINT32) DVBT_GetSQI(pMTPdCtx->pDemodCtx);

    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
    //add driver implementation code here
    USR_SPACE_ACCESS_VALIDATE_ARG(pu4SQI,u4SQI);
    COPY_TO_USER_ARG(pu4SQI,u4SQI,u4SQI);
    //MTFE_DVBT_PRINT(" - u4SQI = %d\n",u4SQI);
	//MTFE_DVBT_PRINT(" - pu4SQI = %d\n",*pu4SQI);
    return MTR_OK;

}

/**Get IF AGC Voltage in mV.
*@param  pu2IfAgcVol        IF AGC voltage in mV.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_DVBT_GetIfAgcVol(unsigned long arg)
{

    UINT16 *pu2IfAgcVol;
    UINT16 u2IfAgcVol = 0;
    pu2IfAgcVol = (UINT16*)arg;

    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when access PI Primitive API, must use this lock for protection
    //add driver implementation code here

    u2IfAgcVol = DVBT_GetIfAgcVol(pMTPdCtx->pDemodCtx);

    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
    USR_SPACE_ACCESS_VALIDATE_ARG(pu2IfAgcVol,u2IfAgcVol);
    COPY_TO_USER_ARG(pu2IfAgcVol,u2IfAgcVol,u2IfAgcVol);
    MTFE_DVBT_PRINT(" - u2IfAgcVol = %d\n",u2IfAgcVol);

    return MTR_OK;

}

/**Get Post Viterbi BER.
*@param  pi4PostVBer        Post Viterbi BER * 10^5, (-1) for no packet output.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_DVBT_GetBER(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    INT32 *pi4PostVBer;
    INT32 *pi4PreVBer;
    INT32 i4PostVBer;
    INT32 i4PreVBer;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    pi4PostVBer = (INT32*)rArg.ai4Arg[0];
    pi4PreVBer  = (INT32*)rArg.ai4Arg[1];

    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when access PI Primitive API, must use this lock for protection

    i4PostVBer = DVBT_GetPostVBer(pMTPdCtx->pDemodCtx); // return value = PostVBer * 10^5
    i4PreVBer  = DVBT_GetPreVBer(pMTPdCtx->pDemodCtx);  // return value =  PreVBer * 10^5

    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
    USR_SPACE_ACCESS_VALIDATE_ARG(pi4PostVBer,i4PostVBer);
    COPY_TO_USER_ARG(pi4PostVBer,i4PostVBer,i4PostVBer);
    USR_SPACE_ACCESS_VALIDATE_ARG(pi4PreVBer,i4PreVBer);
    COPY_TO_USER_ARG(pi4PreVBer,i4PreVBer,i4PreVBer);

    MTFE_DVBT_PRINT(" - i4PostVBer = %d *e-5, i4PreVBer = %d *e-5\n",
        i4PostVBer,
        i4PreVBer);

    return MTR_OK;
}
//Justin todo, need map to pi api
/**Get TPS parameters.
*@param  arg                ---.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_DVBT_GetTPS(unsigned long arg)
{
    MTFE_DVBT_TPS_PARAM_T tps;
    DEMOD_CTX_T* psDemodCtx = pMTPdCtx->pDemodCtx;

    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when access PI Primitive API, must use this lock for protection
    //add driver implementation code here
    //return MTR_NOT_OK if error

    tps.eFftMode = psDemodCtx->u1Fft;
    tps.eConstellation = psDemodCtx->u1Mod;
    tps.eGuardInterval = psDemodCtx->u1GI;
    tps.eCodeRateHp = psDemodCtx->u1HpCR;
    tps.eCodeRateLp = psDemodCtx->u1LpCR;
    tps.u2CellId = psDemodCtx->u2CellId;
    tps.eHierMode = psDemodCtx->u1Hier;
    tps.u1HpLp = psDemodCtx->u1HpLp;
    tps.u1Bw = psDemodCtx->u1Bw;    
    tps.u1Mod = psDemodCtx->u1Mod;
    tps.u1CR = psDemodCtx->u1CR;    

    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
    USR_SPACE_ACCESS_VALIDATE_ARG(arg, MTFE_DVBT_TPS_PARAM_T);
    COPY_TO_USER_ARG(arg, tps, MTFE_DVBT_TPS_PARAM_T);
	#if 0
    MTFE_DVBT_PRINT("tps.eFftMode = %d\n", tps.eFftMode);
    MTFE_DVBT_PRINT("tps.eConstellation = %d\n", tps.eConstellation);
    MTFE_DVBT_PRINT("tps.eGuardInterval = %d\n", tps.eGuardInterval);
    MTFE_DVBT_PRINT("tps.eHierMode = %d\n", tps.eHierMode);
    MTFE_DVBT_PRINT("tps.eCodeRateHp = %d\n", tps.eCodeRateHp);
    MTFE_DVBT_PRINT("tps.eCodeRateLp = %d\n", tps.eCodeRateLp);
    MTFE_DVBT_PRINT("tps.u2CellId = 0x%X\n", tps.u2CellId);
    MTFE_DVBT_PRINT("tps.u1HpLP = 0x%X\n", tps.u1HpLp);
    MTFE_DVBT_PRINT("tps.u1Bw = 0x%X\n", tps.u1Bw);    
    MTFE_DVBT_PRINT("tps.u1Mod = 0x%X\n", tps.u1Mod);    
    MTFE_DVBT_PRINT("tps.u1CR = 0x%X\n", tps.u1CR);
	#endif

    return MTR_OK;
}
//Justin todo, need map to pi api
/**Get frequency offset.
*@param  arg             ---.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_DVBT_GetFreqOffset(unsigned long arg)
{
    UINT32 offset;

    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when access PI Primitive API, must use this lock for protection
    //add driver implementation code here
    //return MTR_NOT_OK if error
    offset = DVBT_GetFreqOffset_kHz(pMTPdCtx->pDemodCtx)/256;

    mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
    USR_SPACE_ACCESS_VALIDATE_ARG(arg, UINT32);
    COPY_TO_USER_ARG(arg, offset, UINT32);

    MTFE_DVBT_PRINT(
        "   frequency offset = %d kHz\n",
        offset
        );

    return MTR_OK;
}
//Justin todo, need map to pi api
/** Check demod software version.
*@param  u1Version          Software Version.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_DVBT_GetVer(unsigned long arg)
{
    UCHAR *pszVersion;
    UCHAR  aszVersion[] = MTFE_DVBT_SW_VERSION; /* Software Version: x.yyy */
    pszVersion = (UINT8*)arg;   

    USR_SPACE_ACCESS_VALIDATE_ARG(pszVersion,aszVersion);
    COPY_TO_USER_ARG(pszVersion,aszVersion,aszVersion);
    MTFE_DVBT_PRINT(" - DVB-T Ver: %s\n", aszVersion);

    return MTR_OK;

}
//Justin todo, don't care this API
/**Register demod callback function.
*@param  CbId               Callback function ID.
*@param  pfCb               Callback function.
*@param  vArg               The argument of callback function.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_DVBT_RegCb(unsigned long arg)
{

    MTAL_IOCTL_3ARG_T rArg;
    UINT8 u1CbId;
    MTFE_DVBT_PFN_CB pfCb;
    VOID* pvArg;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1CbId = (UINT8)rArg.ai4Arg[0];
    pfCb = (MTFE_DVBT_PFN_CB)rArg.ai4Arg[1];
    pvArg = (VOID*)rArg.ai4Arg[2];

    MTFE_DVBT_PRINT(" - u1CbId = %d\n"
		"pfCb = 0x%x\n"
		"pvArg = 0x%x\n",
		u1CbId,
		(int)pfCb,
		(int)pvArg);    
	
    //add driver implementation code here

    return MTR_OK;
}


/** Setup tuner confiuration to demod.
*@param  pTunerConfig       Config of Tuner
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

#if defined(CC_MT5363) || defined(CC_MT5365)|| defined(CC_MT5395)|| defined(CC_MT5396)||defined(CC_MT5389)
static MT_RESULT_T _MTFE_DVBT_Tuner_Config(unsigned long arg) 
{

    MTFE_DVBT_TUNER_CONFIG_T TunerConfig;	
     ITUNER_CTX_T* psITunerCtx = pMTPdCtx->pDemodCtx->psTunerCtx;
     SPECIFIC_MEMBER_EU_CTX* pEUCtx= &( psITunerCtx->specific_member.eu_ctx);
    MT_RESULT_T fgRet = MTR_OK;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg,MTFE_DVBT_TUNER_CONFIG_T);
    COPY_FROM_USER_ARG(arg,TunerConfig,MTFE_DVBT_TUNER_CONFIG_T);

    if(pMTPdCtx){
        mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when access PI Primitive API, must use this lock for protection
        /*setup config to demod*/
        pEUCtx->m_aucPara[2] = TunerConfig.u1IFAGC_SLP;
        pEUCtx->m_aucPara[3] = TunerConfig.u1IFAGC_INT;
        pEUCtx->m_aucPara[4] = TunerConfig.u1IFAGC_MAX;
        pEUCtx->m_aucPara[5] = TunerConfig.u1IFAGC_MIN;
            
        pEUCtx->m_ifAgc_lvl_max = TunerConfig.u1IFAGCLevelMax;
        pEUCtx->m_ifAgc_lvl_min = TunerConfig.u1IFAGCLevelMin;

   //     pEUCtx->m_IF_Freq = TunerConfig.u2IF_Freq;

        // temporarily write target level to constance 
/*
        switch(TunerConfig.u1IFAGCTargetLevel)
        {            
        }
*/        
        pEUCtx->m_aucPara[7] = 0;//cIF_TARGET_LEVEL_0CCC;

        // for TunerOpen would disconnect DVBT, connect DVBT first
        fgRet |= DVBT_Connect(pMTPdCtx->pDemodCtx, MTFE_BW_8MHz);

        fgRet |= DVBT_Initialize(pMTPdCtx->pDemodCtx, MTAL_DVBT_DEMOD_I2C_ADDR, TRUE);
         DVBT_DisConnect(pMTPdCtx->pDemodCtx);

        mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
        return fgRet;
    }

    return MTR_NOT_OK;
}

#else



/** Setup tuner confiuration to demod.
*@param  pTunerConfig       Config of Tuner
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
static MT_RESULT_T _MTFE_DVBT_Tuner_Config(unsigned long arg) 
{

    MTFE_DVBT_TUNER_CONFIG_T TunerConfig;	
    GEN_TUNER_CTX_T *psTunerCtx = &(pMTPdCtx->pDemodCtx->sTunerCtx);
    MT_RESULT_T fgRet = MTR_OK;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg,MTFE_DVBT_TUNER_CONFIG_T);
    COPY_FROM_USER_ARG(arg,TunerConfig,MTFE_DVBT_TUNER_CONFIG_T);

    if(pMTPdCtx){
        mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when access PI Primitive API, must use this lock for protection
        /*setup config to demod*/
        psTunerCtx->m_aucPara[2] = TunerConfig.u1IFAGC_SLP;
        psTunerCtx->m_aucPara[3] = TunerConfig.u1IFAGC_INT;
        psTunerCtx->m_aucPara[4] = TunerConfig.u1IFAGC_MAX;
        psTunerCtx->m_aucPara[5] = TunerConfig.u1IFAGC_MIN;
            
        psTunerCtx->m_ifAgc_lvl_max = TunerConfig.u1IFAGCLevelMax;
        psTunerCtx->m_ifAgc_lvl_min = TunerConfig.u1IFAGCLevelMin;

        psTunerCtx->m_IF_Freq = TunerConfig.u2IF_Freq;

        psTunerCtx->fgRFTuner = TunerConfig.fgRFTuner;
        

        // temporarily write target level to constance 
/*
        switch(TunerConfig.u1IFAGCTargetLevel)
        {            
        }
*/        
        psTunerCtx->m_aucPara[7] = 0;//cIF_TARGET_LEVEL_0CCC;

        // for TunerOpen would disconnect DVBT, connect DVBT first
        fgRet |= DVBT_Connect(pMTPdCtx->pDemodCtx, MTFE_BW_8MHz);

        fgRet |= DVBT_Initialize(pMTPdCtx->pDemodCtx, MTAL_DVBT_DEMOD_I2C_ADDR, TRUE);
        fgRet |= DVBT_DisConnect(pMTPdCtx->pDemodCtx);

        mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
        return fgRet;
    }

    return MTR_NOT_OK;
}

#endif


/**set demod TS out mode.
*@param  fgParallel         TRUE:Parallel;   FALSE:serial
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
//MT_RESULT_T MTFE_DVBC_RegCb(UINT8 u1CbId, MTFE_DVBC_PFN_CB pfCb, VOID* pvArg)
static MT_RESULT_T _MTFE_DVBT_SetTSInterface(unsigned long arg)
{
	
	BOOL *pParallel;
	BOOL Parallel;

	MT_RESULT_T fgRet = MTR_OK;

	pParallel = (BOOL*)(arg);

	USR_SPACE_ACCESS_VALIDATE_ARG(pParallel,BOOL);
	COPY_FROM_USER_ARG(pParallel, Parallel,BOOL);
	MTFE_DVBT_PRINT(" Parallel = %d\n",Parallel);
#ifdef DEBUG_DVBT_PERFORMANCE
    UINT32 u4TickCnt = os_get_sys_tick();
#endif	


	mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when access PI Primitive API, must use this lock for protection   

	DVBT_SetTsOutputMode(pMTPdCtx->pDemodCtx, Parallel);

	mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
	//add driver implementation code here



#ifdef DEBUG_DVBT_PERFORMANCE
	    u4TickCnt = os_get_sys_tick() - u4TickCnt;
	    MTFE_DVBT_PRINT(" - function execution time = %d ms\n",u4TickCnt*x_os_drv_get_tick_period());
#endif	
	return fgRet;
}

/**set demod TS Path.
*@param  fgEnable            TRUE:Enable;   FALSE:Disable
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_DVBT_SetTSPath(unsigned long arg)
{

	BOOL *pEnable;
	BOOL Enable;

	MT_RESULT_T fgRet = MTR_OK;

	pEnable = (BOOL*)(arg);

	USR_SPACE_ACCESS_VALIDATE_ARG(pEnable,BOOL);
	COPY_FROM_USER_ARG(pEnable, Enable,BOOL);
	MTFE_DVBT_PRINT(" Enable = %d\n",Enable);
#ifdef DEBUG_DVBT_PERFORMANCE
	UINT32 u4TickCnt = os_get_sys_tick();
#endif	

	mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when access PI Primitive API, must use this lock for protection   
	DVBT_SetTsPathMode(pMTPdCtx->pDemodCtx, Enable);

	mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
	//add driver implementation code here
	
	
	
#ifdef DEBUG_DVBT_PERFORMANCE
		u4TickCnt = os_get_sys_tick() - u4TickCnt;
		MTFE_DVBT_PRINT(" - function execution time = %d ms\n",u4TickCnt*x_os_drv_get_tick_period());
#endif	
		return fgRet;
	}

#if 0
/**get demod TS formate.
*@param  pTSFmt            formate of demod
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

static MT_RESULT_T _MTFE_DVBT_GetTSFMT( unsigned long arg)
{
#ifdef DEBUG_DVBT_PERFORMANCE
    UINT32 u4TickCnt = os_get_sys_tick();
#endif

    TS_FMT_T *pTsFmt;
    TS_FMT_T TsFmt;
	
    pTsFmt = (TS_FMT_T*)arg;

    mcSEMA_LOCK_MTFE(pMTPdCtx->hHalLock); //when access PI Primitive API, must use this lock for protection   


	TsFmt = DVBT_GetTsFmt(pMTPdCtx->pDemodCtx);	


	mcSEMA_UNLOCK_MTFE(pMTPdCtx->hHalLock);
    //add driver implementation code here

    USR_SPACE_ACCESS_VALIDATE_ARG(pTsFmt,TS_FMT_T);
    COPY_TO_USER_ARG(pTsFmt,TsFmt,TS_FMT_T);
    MTFE_DVBT_PRINT("TsFmt.SerOrPar = %d\n"
					"TsFmt.ClockPolarity = %d\n"
					"TsFmt.fgTristate = %d\n"
					"TsFmt.u4TsClockInKHz = %d\n"
					"TsFmt.fgGatedClock = %d\n"
					"TsFmt.fgExternalDemod = %d\n",
					 TsFmt.SerOrPar,
					 TsFmt.ClockPolarity,
					 TsFmt.fgTristate,
					 TsFmt.u4TsClockInKHz,
					 TsFmt.fgGatedClock,
					 TsFmt.fgExternalDemod); 
#ifdef DEBUG_DVBT_PERFORMANCE
    u4TickCnt = os_get_sys_tick() - u4TickCnt;
    MTFE_DVBT_PRINT(" - function execution time = %d ms\n",u4TickCnt*x_os_drv_get_tick_period());
#endif

    return MTR_OK;  

}
#endif

/** For PAL to get digital/analog exclusive semaphore.
*@param  *sema              pointer to a sema
*@retval void
*/
void Get_MTFE_DVBT_Sema_Ex_DiAna(HANDLE_T *sema)
{
    *sema = Exclusive_DigiAnaSema;
}
//Justin todo, then go implementation you user space ut(unit test) code
/**Register demod callback function.
*@param  CbId               Callback function ID.
*@param  pfCb               Callback function.
*@param  vArg               The argument of callback function.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
int mtal_ioctl_mtfe_dvbt(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    MT_RESULT_T eRet = MTR_OK;
    MTFE_DVBT_PRINT("-------dvbt ioctl cmd-------\r\n");
    if(u1ExclSemaInit==1){
        mcSEMA_LOCK_MTFE_S(Exclusive_DigiAnaSema,"Ex_DiAna");
    }
	switch (cmd)
	{
		case MTAL_IO_DVBT_OPEN:
			eRet=_MTFE_DVBT_Open();
            break;
		case MTAL_IO_DVBT_CLOSE:
			eRet=_MTFE_DVBT_Close();
            break;
		case MTAL_IO_DVBT_CONNECT:
			eRet=_MTFE_DVBT_Connect( arg);
            break;
		case MTAL_IO_DVBT_DISCONNECT:
		    eRet=_MTFE_DVBT_DisConnect();
            break;
		case MTAL_IO_DVBT_GETSYNC:
			eRet=_MTFE_DVBT_GetSync( arg);
            break;
		case MTAL_IO_DVBT_GETSIGLEVEL:
			eRet=_MTFE_DVBT_GetSignalLevel( arg);
            break;
		case MTAL_IO_DVBT_GETPER:
			eRet=_MTFE_DVBT_GetPER( arg);
            break;
		case MTAL_IO_DVBT_GETSNR:
		    eRet=_MTFE_DVBT_GetSNR( arg);
            break;
		case MTAL_IO_DVBT_GETSQI:
		    eRet=_MTFE_DVBT_GetSQI( arg);
            break;
		case MTAL_IO_DVBT_GETIFAGC:
			eRet=_MTFE_DVBT_GetIfAgcVol( arg);
            break;
		case MTAL_IO_DVBT_GETTCMBER:
			eRet=_MTFE_DVBT_GetBER( arg);
            break;
		case MTAL_IO_DVBT_GETTPS:
			eRet=_MTFE_DVBT_GetTPS( arg);
            break;
        case MTAL_IO_DVBT_GETFREQOFFSET:
            eRet=_MTFE_DVBT_GetFreqOffset( arg);
            break;
		case MTAL_IO_DVBT_GETVER:
			eRet=_MTFE_DVBT_GetVer( arg);
            break;
		case MTAL_IO_DVBT_REGCB:
		    eRet=_MTFE_DVBT_RegCb( arg);
            break;
    case MTAL_IO_DVBT_TUNER_CONFIG:
        	eRet=_MTFE_DVBT_Tuner_Config( arg);
			break;	
		case MTAL_IO_DVBT_SETTSINTERFACE:
			eRet = _MTFE_DVBT_SetTSInterface(arg);
			break;
		case MTAL_IO_DVBT_SETTSPATH:
			eRet = _MTFE_DVBT_SetTSPath(arg);
			break;

		default:
		    break;
	}

    if(u1ExclSemaInit==1){
        mcSEMA_UNLOCK_MTFE_S(Exclusive_DigiAnaSema, "Ex_DiAna");
    }
	return eRet;
}
