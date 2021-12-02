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
/********************************************************************************************
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES 
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED 
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS 
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, 
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, 
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK 
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION 
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
* 
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH 
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, 
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE 
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
 *     
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS 
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.  
 ************************************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: ether_if.c $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *  Ethernet driver interface source file. 
 *  It provides API interface for upper layer and ethernet thread service. 
 *
 * Author:
 * -------
 *   Hs Huang
 *
 * Last changed:
 * -------------
 * $Author:
 *
 * $Modtime:
 *
 * $Revision:
****************************************************************************/
		
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"

#include "drv_common.h"
#include "ether_com.h"
#include "ether_drv.h"
#include "ether_if.h"
#include "ether_oid.h"
#include "x_timer.h"
#include "chksum.h"
#if CONFIG_ETHER_USE_OS_NATIVE
#include "nucleus.h"
#endif
#include "drvcust_if.h"
#include "x_ckgen.h"
#include "x_pinmux.h"
LINT_EXT_HEADER_END
char *_cDBGStrDef[] =
{
  "[PHY]     100M AMP Level = %d\n",
  "[PHY]    Slew Rate Level = %d\n",
  "[PHY]   Input Bias Level = %d\n",
  "[PHY]  Output Bias Level = %d\n",
  "[PHY]  BW 50 percent OFF = %d\n",
  "[PHY] Feedback CAP LEVEL = %d\n",
  "[PHY]     EYE OPEN LEVEL = %d\n",
  "[PHY]      10M AMP Level = %d\n"
};
/****************************************************************************
** Local definitions
****************************************************************************/
#define ETHER_MSG_MAX_NUM        	(32) 		/* max ethernet thread msg number */

#define ETHER_THREAD_TIMEOUT_CNT    500         /* ethernet thread time out count */
#define ETHER_PHY_MONITOR_PERIOD    300         /* ethernet PHY status monitor period */
#define ETHER_EV_TXMIT_CHK_PERIOD   100			/* packet transmission completion event checking period */
#define ETHER_EV_IOCTL_CHK_PERIOD   200			/* ioctl completion event checking period */
#define ETHER_THROUGHPUT_CALCULATE_PERIOD    1000

#define ETHER_MSGQ_NAME 			"EtherMsgQ"
#define ETHER_EVENT_NAME 			"EtherEvent"
#define ETHER_TX_CHKSUM_EVENT_NAME            "EtherTxChkSumEvent"
#define ETHER_RX_CHKSUM_EVENT_NAME            "EtherRxChkSumEvent"

/****************************************************************************
** Local structures and enumerations
****************************************************************************/
static void vEtherTxChkSumTaskMain(void* pvArg);
static void vEtherRxChkSumTaskMain(void* pvArg);

/****************************************************************************
** External function prototypes
****************************************************************************/

/****************************************************************************
** Local variable
****************************************************************************/
static HANDLE_T _hEtherThread;
static HANDLE_T _hChkSumTxThread;
static HANDLE_T _hChkSumRxThread;
static HANDLE_T  _hTxChkSumEvent;
static HANDLE_T  _hRxChkSumEvent;
static UINT32 _TxCkSumTestSize = 100;
static UINT32 _RxCkSumTestSize = 100;
UINT8 _u1StopPhyPolling= 0;
#if 0
static HANDLE_T _hEtherMsgQ;
#endif
static HANDLE_T _hEtherTimer;
#if ENABLE_REALTIME_THROUGHPUT_COUNT
static HANDLE_T _hEthThroughputTimer;
#endif
#if CONFIG_ETHER_USE_OS_NATIVE
static NU_SEMAPHORE *pt_sema;
#else
static HANDLE_T _hEtherSendSema;
#endif

#if CONFIG_ETHER_USE_SAM_INSTEAD_EV
HANDLE_T _hEtherTaskSema;
#endif

static NET_DRV_IF_T *_prEtherDev;
static BOOL _fgInited = FALSE;

static BOOL _fgTxChkSumTaskInit = FALSE;
static BOOL _fgRxChkSumTaskInit = FALSE;
/****************************************************************************
** Global variable
****************************************************************************/
HANDLE_T _hEtherEvent;

/****************************************************************************
** Local functions
****************************************************************************/

/*******************************************************************************
**  vEtherIrqHandler
**
**  descriptions:
**      This routine is called to handle ethernet interrupts
**  parameters:
**      none
**  result:
**      It will set event if it is tx irq and disable rx irq and re-enable 
**      after defered task service is completed. It makes rx service is likely 
**      handled in interrupt routine.
**  return:
**      none
*******************************************************************************/
INT32  fgIntTest=0;

INT32  PauseFrameDecodeTest=0;
INT32  LastTxIntervalTime=0;
HAL_TIME_T _rEtherTxTime1;
HAL_TIME_T _rEtherTxTime2;
HAL_TIME_T _rEtherTxInterval;
static void vEtherIrqHandler(UINT16 u2Vector)
{
    NET_DRV_IF_T *prDev = prEtherGetDev();
    PETHER_ADAPTER_T prMac = prDev->pv_priv;
    UINT32 u4EtherIrq;
    UINT32 u4EtherIrqALL;
#if CONFIG_ETHER_USE_SAM_INSTEAD_EV
    BOOL fgSetEvt = FALSE;
    #endif

    if(fgIntTest)
    {
     if(dReadMAC(RW_DLY_INT_CFG)& TX_SKIP)
       UTIL_Printf("TX skip interrupt;\n");
     if(dReadMAC(RW_DLY_INT_CFG)& TNTC)
       UTIL_Printf("TX complete interrupt;\n");
     if(dReadMAC(RW_DLY_INT_CFG)& TNQE)
       UTIL_Printf("TX queue empty interrupt;\n");
     if(dReadMAC(RW_DLY_INT_CFG)& MAGIC_PKT_REC)
       UTIL_Printf("Magic packet interrupt;\n");
     if(dReadMAC(RW_DLY_INT_CFG)& FNQF)
       UTIL_Printf("RX queue full interrupt;\n");
     if(dReadMAC(RW_DLY_INT_CFG)& FNRC)
       UTIL_Printf("RX Complete interrupt;\n");
	 
     if(dReadMAC(RW_DLY_INT_CFG)& MIB_COUNTER_TH)
       UTIL_Printf("MIB counter > 0x8000000 interrupt;\n");
     if(dReadMAC(RW_DLY_INT_CFG)& PORT_STATUS_CFG)
       UTIL_Printf("Rort status interrupt;\n");
     if(dReadMAC(RW_DLY_INT_CFG)& RX_FIFO_FULL)
       UTIL_Printf("RX fifo full interrupt;\n");
	 
	 fgIntTest--;
    }
	
   if(dReadMAC(RW_DLY_INT_CFG)& TX_SKIP)
       UTIL_Printf("TX skip interrupt;\n");


   ///for pause frame decode ability test 
   if(PauseFrameDecodeTest && (dReadMAC(RW_DLY_INT_CFG)& TNTC) )
   {
      HAL_GetTime(&_rEtherTxTime2);
	  HAL_GetDeltaTime(&_rEtherTxInterval, &_rEtherTxTime1,&_rEtherTxTime2 );
	  _rEtherTxTime1 = _rEtherTxTime2;

	  if((_rEtherTxInterval.u4Seconds > 0) || (_rEtherTxInterval.u4Micros > 10000))
	  {
          UTIL_Printf("TX Pause %d mS \n",(_rEtherTxInterval.u4Micros/1000 + _rEtherTxInterval.u4Seconds*1000));
	  }
	  
   }
    /* get mask of current enabled interrupt requests and clear it */
    //u4EtherIrq = u4DmaGetInt(DMA_INT_ENABLE); //kenny   
    //vDmaIntClear(u4EtherIrq);
    //vDmaIntDisable(DMA_INT_ENABLE);
    vDmaIntDisable((DMA_INT_ENABLE|PORT_STATUS_CFG));
    u4EtherIrq = dReadMAC(RW_DLY_INT_CFG);
    vDmaIntClear(u4EtherIrq);//kenny mark for MT8550
    
    /* clear BIM interrupt */
    if(!BIM_ClearIrq(u2Vector))
    {
        ERR_ETHER("ethernet BIM irq clear fail\n");
    }

     u4EtherIrqALL =u4EtherIrq ;//kenny test
    
    
    u4EtherIrq = u4EtherIrq& DMA_INT_ENABLE;

    prMac->u4IrqStatus = u4EtherIrq;

    if(u4EtherIrqALL& PORT_STATUS_CFG)
    {
     vStopSoftPollingPhy(0);//enable Software Polling	
     Printf("[NET]Link <-> down INT\n");
    }
    
    if(u4EtherIrqALL& MAGIC_PAK_REC_INT_MASK)
    {
     Printf("[NET]Magic packet INT\n");
    }

    ETHER_SET_EVENT(ETHER_EG_TX_COMP_IRQ|ETHER_EG_RX_IRQ);  

   // Printf("[NET INT]!INT Status Reg =0x%x\n", u4EtherIrq);

   // if(u4EtherIrq ==0)
   // return;
    
   // Printf("[NET INT]!prMac->u4IrqStatus =0x%x\n", prMac->u4IrqStatus );  
   



#if 0//kenny mark    
    if(u4EtherIrq & DMA_TX_INT_MASK) 
    {
#if (!CONFIG_DRV_LINUX_DATA_CONSISTENCY)
        PMAC_TX_DESC_T prTxDesc;    
        
        prTxDesc = prMac->prTxDesc + prMac->u4TxCompIdx;    
        if(fgDescIsTxSent(prTxDesc))
        {
            /* disable tx/rx irq and re-enable after defered task service is completed */
            vDmaIntDisable(DMA_INT_ENABLE);
        /* tx irq, set event to response txmit API to perform synchronization */
        ETHER_SET_EVENT(ETHER_EG_TX_COMP_IRQ);      
#if CONFIG_ETHER_USE_SAM_INSTEAD_EV
        fgSetEvt = TRUE;
#endif
    }
        else
        {
            /* strange: sometimes no tx descriptor is set but irq is triggered */
            NOTE_ETHER("TX INT but no packet complete \n"); 
            prTxDesc = 0; // no meaning, just used for ICE trace
        }

#else  //CONFIG_DRV_LINUX_DATA_CONSISTENCY
	   	/* disable tx/rx irq and re-enable after defered task service is completed */
       	vDmaIntDisable(DMA_INT_ENABLE);
              //Printf("[Net]TX DMA_INT_ENABLE\n");
       	/* tx irq, set event to response txmit API to perform synchronization */
       	ETHER_SET_EVENT(ETHER_EG_TX_COMP_IRQ);                            
#if CONFIG_ETHER_USE_SAM_INSTEAD_EV
    	fgSetEvt = TRUE;
#endif
#endif
    }

    if(u4EtherIrq & DMA_RX_INT_MASK)
    {
        /* disable tx/rx irq and re-enable after defered task service is completed */
        vDmaIntDisable(DMA_INT_ENABLE);
        ETHER_SET_EVENT(ETHER_EG_RX_IRQ);
#if CONFIG_ETHER_USE_SAM_INSTEAD_EV
        fgSetEvt = TRUE;        
#endif
    }

    if(fgHalIsPwrDownWakeup())
    {
        ETHER_SET_EVENT(ETHER_EG_WAKEUP_IRQ);
#if CONFIG_ETHER_USE_SAM_INSTEAD_EV
        fgSetEvt = TRUE;
#endif
        INFO_ETHER("power down wake-up interrupt\n");
    }
#if CONFIG_ETHER_USE_SAM_INSTEAD_EV
    if (fgSetEvt)
    {
       ETHER_SEMA_UNLOCK(_hEtherTaskSema);
    }
#endif
#endif
    
}
	
/*******************************************************************************
**  vEtherSetIsr
**
**  descriptions:
**      This routine is called to register or de-register ethernet interrupt 
**  parameters:
**      TRUR - register ethernet interrupt vector
**      FALSE - de-register ethernet interrupt vector
**  result:
**  return:
**      none
*******************************************************************************/
static void vEtherSetIsr(BOOL fgSet)
{
    void (* pfnOldIsr)(UINT16);

	vDmaIntClearAll();
	
    if(fgSet) 
    {
        BIM_ClearIrq(VECTOR_ENET);
        VERIFY(x_reg_isr(VECTOR_ENET, vEtherIrqHandler, &pfnOldIsr) == OSR_OK);        
        BIM_EnableIrq(VECTOR_ENET);
    }
    else
    {
        VERIFY(x_reg_isr(VECTOR_ENET, NULL, &pfnOldIsr) == OSR_OK);
        BIM_DisableIrq(VECTOR_ENET);
    }

    vDmaIntClearAll();
}

/*******************************************************************************
**  vEtherStatusChk
**
**  descriptions:
**      This routine is called to send timer event to check connection status 
**  parameters:
**      none
**  return:
**      none
*******************************************************************************/
static void vEtherStatusChk(HANDLE_T pt_handle, void *pvArg)
{
#if CONFIG_ETHER_USE_SAM_INSTEAD_EV
    PETHER_ADAPTER_T prMac = pvArg;
#else
    UNUSED(pvArg);
#endif
if((_u1StopPhyPolling==0))
{
    ETHER_SET_EVENT(ETHER_EG_TIMER_SRV);                            
}    
#if CONFIG_ETHER_USE_SAM_INSTEAD_EV
    ETHER_SEMA_UNLOCK(_hEtherTaskSema);
#endif
}

/*******************************************************************************
**  vEtherThroughputCal
**
**  descriptions:
**      This routine is called to calculate RX throughput 
**  parameters:
**      none
**  return:
**      none
*******************************************************************************/
#if ENABLE_REALTIME_THROUGHPUT_COUNT
static void vEtherThroughputCal(HANDLE_T pt_handle, void *pvArg)
{
	PMAC_STAT_T prStat;
	
	prStat = (PMAC_STAT_T)pvArg;
    prStat->u4RxThroughput = (UINT32)(prStat->u8RxOctet - prStat->u8RxPreOctet);  
    prStat->u8RxPreOctet = prStat->u8RxOctet; 
    //NOTE_ETHER("Throughput = %u \n", prStat->u4RxThroughput);
}
#endif

/*******************************************************************************
**  vEtherTaskMain
**
**  descriptions:
**      Main routine for ethernet task
**  parameters:
**      pvArg - pointer to struct net_device
**  result:
**      Currently, it only service rx defer task
**  return:
*******************************************************************************/
static void vEtherTaskMain(void* pvArg)
{
    NET_DRV_IF_T *prDev;
    //INT32 i4Ret;
#if 0    
    UINT32 u4MsgCmd;
#endif
    EV_GRP_EVENT_T u8EtherEvent;
    PETHER_ADAPTER_T prMac;
#if CONFIG_ETHER_USE_SAM_INSTEAD_EV
    CRIT_STATE_T  t_state;
#endif
    ASSERT(pvArg != NULL);

    prDev = (NET_DRV_IF_T *) *((UINT32 *)pvArg);
    prMac = (PETHER_ADAPTER_T)prDev->pv_priv;

    /* Initialize ethernet driver message queue */
#if 0    
    i4Ret = x_msg_q_create(&_hEtherMsgQ,
    	                    (const CHAR *)ETHER_MSGQ_NAME,
                            sizeof(u4MsgCmd),
                            ETHER_MSG_MAX_NUM);
    ASSERT(i4Ret == OSR_OK);
#endif    

    /* Initialize ethernet send frame semaphhore */
#if CONFIG_ETHER_USE_OS_NATIVE  
    #if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)  
    pt_sema = x_mem_alloc(sizeof(NU_SEMAPHORE));
    #else
    pt_sema = x_mem_alloc(sizeof(NU_SEMAPHORE));
    #endif

    //i4Ret = NU_Create_Binary_Semaphore(pt_sema, "etxSma", 1, NU_PRIORITY);
    //ASSERT(i4Ret == NU_SUCCESS);
    VERIFY(NU_Create_Binary_Semaphore(pt_sema, "etxSma", 1, NU_PRIORITY) == NU_SUCCESS);
#else
	#if 0
    i4Ret = x_sema_create(&_hEtherSendSema,
                           X_SEMA_TYPE_BINARY,
                           X_SEMA_STATE_UNLOCK);
    ASSERT(i4Ret == OSR_OK);
	#endif
    VERIFY(x_sema_create(&_hEtherSendSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
#endif    

#if CONFIG_ETHER_USE_SAM_INSTEAD_EV
	#if 0
    i4Ret = x_sema_create(&_hEtherTaskSema,
                           X_SEMA_TYPE_BINARY,
                           X_SEMA_STATE_LOCK);
    ASSERT(i4Ret == OSR_OK);
    #endif
    VERIFY(x_sema_create(&_hEtherTaskSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
#endif


    /* Initialize ethernet event group */
    u8EtherEvent = 0;
	#if 0
    i4Ret = x_ev_group_create(&_hEtherEvent, 
                              (const CHAR *)ETHER_EVENT_NAME,
	                          u8EtherEvent);
    ASSERT(i4Ret == OSR_OK);
	#endif
	VERIFY(x_ev_group_create(&_hEtherEvent, (const CHAR *)ETHER_EVENT_NAME, u8EtherEvent) == OSR_OK);
    /* Initialize ethernet timer service */
    VERIFY(x_timer_create(&_hEtherTimer) == OSR_OK);
    VERIFY(x_timer_start(_hEtherTimer, 
                         ETHER_PHY_MONITOR_PERIOD, 
                         X_TIMER_FLAG_REPEAT, 
                         vEtherStatusChk, 
#if CONFIG_ETHER_USE_SAM_INSTEAD_EV
                         prMac) == OSR_OK);
#else
                         NULL) == OSR_OK);
#endif


#if ENABLE_REALTIME_THROUGHPUT_COUNT
	VERIFY(x_timer_create(&_hEthThroughputTimer) == OSR_OK);
    VERIFY(x_timer_start(_hEthThroughputTimer, 
                         ETHER_THROUGHPUT_CALCULATE_PERIOD, 
                         X_TIMER_FLAG_REPEAT, 
                         vEtherThroughputCal, 
                         &(prMac->rStat)) == OSR_OK);
#endif

    /* register ethernet interrupt service */
    vEtherSetIsr(TRUE);

    do
    {
#if CONFIG_ETHER_USE_SAM_INSTEAD_EV
        ETHER_SEMA_LOCK(_hEtherTaskSema);
        t_state = x_crit_start();
        u8EtherEvent = prMac->u8TaskEvt;
        prMac->u8TaskEvt = 0;
        x_crit_end(t_state);
#else
        // after service is done, sleep until event request is triggered or timeout
        #if 0
        i4Ret = x_ev_group_wait_event(_hEtherEvent,
                                              ETHER_EG_WAKEUP_THREAD,
                                              &u8EtherEvent,
                                      X_EV_OP_OR_CONSUME);

        ASSERT(i4Ret == OSR_OK);
        #endif
        VERIFY(x_ev_group_wait_event(_hEtherEvent, ETHER_EG_WAKEUP_THREAD, &u8EtherEvent, X_EV_OP_OR_CONSUME) == OSR_OK);
#endif
        if(u8EtherEvent & (ETHER_EG_RX_IRQ | ETHER_EG_RECV_PKT | ETHER_EG_TX_COMP_IRQ))
            {
                vEtherDeferTask(prDev);
            } 
#if 0
            if(u8EtherEvent & ETHER_EG_WAKEUP_IRQ)
            {
                i4EtherDrv_Start(prDev);
            } 
#endif
        /* ethernet timer service  */
        if(u8EtherEvent & ETHER_EG_TIMER_SRV)
        {
            vHalCalThroughput(prMac);
            vEtherLinkCheck(prDev);
          
          if(GET_ETHER_INFO(prMac, ETHER_LINK_UP))
          {
            //Printf("!Stop Polling \n");
            vStopSoftPollingPhy(1);
          }	
          
        } 
        
    } while(GET_ETHER_INFO(prMac, ETHER_DEV_INIT));

        Printf("Exiting Ethernet main task...\n");
	vEtherAdapterRemove(prMac);		//Move from i4EtherDrv_DeInit() CM 20090324

    /* release all OS resource */
#if 0    
    VERIFY(x_msg_q_delete(_hEtherMsgQ) == OSR_OK);
#endif
    VERIFY(x_ev_group_delete(_hEtherEvent) == OSR_OK);
    VERIFY(x_timer_delete(_hEtherTimer) == OSR_OK);
#if ENABLE_REALTIME_THROUGHPUT_COUNT
    VERIFY(x_timer_delete(_hEthThroughputTimer) == OSR_OK);
#endif

#if CONFIG_ETHER_USE_OS_NATIVE    
    VERIFY(NU_Release_Binary_Semaphore(pt_sema) == NU_SUCCESS);
#else
    VERIFY(x_sema_delete(_hEtherSendSema) == OSR_OK);
#endif
#if CONFIG_ETHER_USE_SAM_INSTEAD_EV
    VERIFY(x_sema_delete(_hEtherTaskSema) == OSR_OK);
#endif


    /* terminate ethernet thread */
    x_thread_exit();
}


/****************************************************************************
** Global functions
****************************************************************************/
	
/******************************************************************************
* Function      : i4NetSendMsgQ
* Description   : Send net message queue 
* Parameter     : prDev, u4Msg, u4Priority
* Return        : None
******************************************************************************/
#if 0
INT32 i4EtherSendMsgQ(const PETHER_ADAPTER_T prMac, 
                             ETHER_MSG_T eEtherMsg, 
                             ETHER_MSG_PRIORITY_T ePriority )
{
    INT32 i4Ret = 0;

    ASSERT(prMac != NULL);
    
    /* prohibit MsgQ command as suspending */
    if(GET_ETHER_INFO(prMac, ETHER_DEV_START) ||
       (eEtherMsg == ETHER_MSG_WAKEUP_IRQ))
	{
       	i4Ret = x_msg_q_send((HANDLE_T)_hEtherMsgQ,
       		                 (const VOID*)&eEtherMsg,
       		                 (SIZE_T)sizeof(UINT32),
       		                 (UINT8)ePriority);
    
       	ASSERT(i4Ret == OSR_OK);
    }
    else	
    {
        INFO_ETHER("ethernet is suspended and do not accept cmd %d\n", eEtherMsg);
        i4Ret = NET_DRV_ALREADY_STOP;
    }
	return (i4Ret);
}
#endif

/*******************************************************************************
**  prEtherGetDev
**
**  descriptions:
**      This routine is called to return network interface struct
**  parameters:
**      none
**  result:
**  return:
**      Current network interface struct
*******************************************************************************/
NET_DRV_IF_T* prEtherGetDev(void)
{
    ASSERT(_prEtherDev != NULL);
    return(_prEtherDev);
}


/*******************************************************************************
** NET_DRV_API_T
*******************************************************************************/

/*******************************************************************************
**  i4EtherDrv_Init
**
**  @brief  This routine is called to initialize ethernet driver and creat thread
**
**  @param  prDev - pointer to net device structure
**  @result
**  @return NET_DRV_OK - driver init OK
**          NET_DRV_ERROR - driver init fail
**  @note
*******************************************************************************/
INT32 i4EtherDrv_Init(NET_DRV_IF_T *prDev)
{
    INT32 i4Ret;

    ASSERT(prDev != NULL);
    
    if (!_fgInited)
    {
        /* config pin as ethernet pads */
        //BSP_PinSet(SEL_ETRXCLK, 1);
        //BSP_PinSet(SEL_ETTXCLK, 1);
        #if 0
         
        if(DRVCUST_InitGet(eFlagDDRQfp))//MT5365, QFP
        {
        //0xF000D400 | 42A05111 20220000 00000000 02022222
        //0xF000D410 | 00200222 00010111 00000000 00002001	
        
        IO_WRITE32(0xf000d404, 0, 0x20220000);//pinmux
        IO_WRITE32(0xf000d40c, 0, 0x02022222);  
        IO_WRITE32(0xf000d410, 0, 0x00200222);  
        IO_WRITE32(0xf000d41c, 0, IO_READ32(0xf000d410, 0)&(~(1<<16))); 
        IO_WRITE32(0xf000d408, 0, 0x0);
        }
        else //Code for BGA
        IO_WRITE32(0xf000d408, 0, 0x55555555);  //Pinmux for MT5366 BGA ethernet, only for verify	
        
        
        
        
        if(DRVCUST_InitGet(eFlagDDRQfp))//MT5365, QFP
        {
          if (BSP_Calibrate(SRC_FRAC_DIV_CLK, 25000000))
          {
            //Error setting clock
          }

          UNUSED(GPIO_SetOut(4, 1));
          BSP_PinSet(PIN_GPIO4, PINMUX_FUNCTION2);
        }
        else
        {
          if (BSP_Calibrate(SRC_FRAC_DIV_CLK, 25000000))
          {
            //Error setting clock
          }

          //UNUSED(GPIO_SetOut(4, 1));
          //BSP_PinSet(PIN_GPIO4, PINMUX_FUNCTION2);	
           IO_WRITE32(0xf000d518, 0, IO_READ32(0xf000d518, 0)|(1<<15));//0xd518 bit15=1
        	
        	
        }	
        
        if(DRVCUST_InitGet(eFlagDDRQfp))//MT5365, QFP
        {
        IO_WRITE32(0xf000d28c, 0, 0x00000001); //set miitx clock source from pad_ettxclk
        IO_WRITE32(0xf000d290, 0, 0x00000001);//set miirx clock source from pad_ettxclk
        IO_WRITE32(0xf000d2d4, 0, 0x00000001);//set rmii clock source from pad_ettxclk
        IO_WRITE32(0xf000d288, 0, 0x00000000);//set mac clock source from xtal
        IO_WRITE32(0xf000d294, 0, 0x00000002);//set mdc clock source from xtal_d8
        }
        else
        {
        IO_WRITE32(0xf000d28c, 0, 0x00000001); //set miitx clock source from pad_ettxclk
        IO_WRITE32(0xf000d290, 0, 0x00000002);//set miirx clock source from pad_etrxclk
        IO_WRITE32(0xf000d2d4, 0, 0x00000001);//set rmii clock source from pad_ettxclk
        IO_WRITE32(0xf000d288, 0, 0x00010000);//set mac clock source from sawlesspll_d16
        IO_WRITE32(0xf000d294, 0, 0x00000002);//set mdc clock source from xtal_d8
        }
        #endif
        _prEtherDev = prDev;
       
        /* register ethernet driver callback function */
        prDev->pf_drv_init = (netdrv_init_fnct) i4EtherDrv_Init;
        prDev->pf_drv_deinit = (netdrv_deinit_fnct) i4EtherDrv_DeInit;
        prDev->pf_drv_start = (netdrv_start_fnct) i4EtherDrv_Start;
        prDev->pf_drv_stop = (netdrv_stop_fnct) i4EtherDrv_Stop;
        prDev->pf_drv_txmit = (netdrv_txmit_fnct) i4EtherDrv_Txmit;
        prDev->pf_drv_set = (netdrv_set_fnct) i4EtherDrv_Set;
        prDev->pf_drv_get = (netdrv_get_fnct) i4EtherDrv_Get;

        /* device must be initialized before creating thread */
        i4Ret = i4EtherAdapterCreat(prDev);
        if(i4Ret != NET_DRV_OK)
        {
            vEtherAdapterRemove((PETHER_ADAPTER_T)prDev->pv_priv);
            ERR_ETHER("ethernet adapter init fail\n");
            return (NET_DRV_ERROR);
        }   

        vEtherModuleInit(prDev);
        
        /* thread must be created after device init */
        i4Ret = x_thread_create(&_hEtherThread,
                                 ETHER_THREAD_NAME,
                                 ETHER_STACK_SIZE,
                                 ETHER_THREAD_PRIORITY,
                                 (x_os_thread_main_fct) vEtherTaskMain,
                                 (SIZE_T)sizeof(NET_DRV_IF_T *),
                                 (void *)&prDev);
        ASSERT(i4Ret == OSR_OK);

    	_fgInited = TRUE;
	}
    else
    {
        ERR_ETHER("ethernet driver was already init\n");
        return (NET_DRV_ALREADY_INIT);
    }

	return (i4Ret);
}

/*******************************************************************************
**  i4EtherDrv_DeInit
**
**  @brief  This routine is called to de-initialize ethernet driver and thread
**
**  @param  prDev - pointer to net device structure
**  @result
**  @return 0 if success
**          negative if fail
**  @note
*******************************************************************************/
INT32 i4EtherDrv_DeInit(NET_DRV_IF_T *prDev)
{
    PETHER_ADAPTER_T prMac;
    INT32 i4Ret = 0;

    ASSERT(prDev != NULL);
    prMac = (PETHER_ADAPTER_T)prDev->pv_priv;
    
    if(!_fgInited)
    {
        return(NET_DRV_NOT_INIT);
    }

	vEtherSetIsr(FALSE);
	
    i4Ret = i4EtherDrv_Stop(prDev);
    //vEtherAdapterRemove(prMac);    //Move to vEtherTaskMain() CM 20090324

    /* clear device ready bit to terminate ethernet thread */
    CLR_ETHER_INFO(prMac, ETHER_DEV_INIT);

    /* clear init flag for next init trigger */
    _fgInited = FALSE;
	
    return (i4Ret);
}

/*******************************************************************************
**  i4EtherDrv_Start
**
**  @brief  This routine is called to start ethernet driver processing
**
**  @param  prDev - pointer to net device structure
**  @result
**  @return 0 if success
**          negative if fail
**  @note
*******************************************************************************/
INT32 i4EtherDrv_Start(NET_DRV_IF_T *prDev)
{
    PETHER_ADAPTER_T prMac;

    ASSERT(prDev != NULL);
    prMac = (PETHER_ADAPTER_T)prDev->pv_priv;
    
    if(!_fgInited)
    {
        return(NET_DRV_NOT_INIT);
    }

    if(GET_ETHER_INFO(prMac, ETHER_DEV_START)) 
    {
        INFO_ETHER("ethernet driver was already started\n");
        return(NET_DRV_ALREADY_START);
    }
    else
    {
        SET_ETHER_INFO(prMac, ETHER_DEV_START);
		if (!fgIsDMATxEnable())	
		{
        	vEtherDrvStart(prDev); 
		}
        INFO_ETHER("ethernet driver is started\n");
        return(NET_DRV_OK);
    }
}

/*******************************************************************************
**  i4EtherDrv_Stop
**
**  @brief  This routine is called to stop ethernet driver processing and power down
**
**  @param  prDev - pointer to net device structure
**  @result
**  @return 0 if success
**          negative if fail
**  @note
*******************************************************************************/
INT32 i4EtherDrv_Stop(NET_DRV_IF_T *prDev)
{
    PETHER_ADAPTER_T prMac;

    ASSERT(prDev != NULL);
    prMac = (PETHER_ADAPTER_T)prDev->pv_priv;
    
    if(!_fgInited)
    {
        return(NET_DRV_NOT_INIT);
    }

    if(GET_ETHER_INFO(prMac, ETHER_DEV_START)) 
	{
        CLR_ETHER_INFO(prMac, ETHER_DEV_START);
    }
    else
    {
        INFO_ETHER("ethernet driver was already stoppped\n");
        return(NET_DRV_ALREADY_STOP);
    }

    /* Stop MAC port and clear descriptor */
    vEtherDrvStop(prDev);
   
   	x_thread_delay(5);
    vEtherDrvSwReset(prDev);  //CM 20090504
    INFO_ETHER("ethernet driver is stoppped\n");
    return(NET_DRV_OK);
}

/*******************************************************************************
**  i4EtherDrv_Txmit
**
**  @brief  This routine is called to transmit packet through ethernet driver 
**
**  @param  prDev - pointer to net device structure
**  @result
**  @return 0 if success
**          negative if fail
**  @note   ethernet driver will send the packet immediately and wait transmission 
**          completed sema, then rerun the fucntion call
*******************************************************************************/
INT32 i4EtherDrv_Txmit(const NET_DRV_IF_T *prDev, PKT_BUFF_T *prPkt)
{
    PETHER_ADAPTER_T prMac;
    INT32 i4Ret;

    ASSERT(prDev != NULL);
    ASSERT(prPkt != NULL);
    prMac = (PETHER_ADAPTER_T)prDev->pv_priv;
    
    if(!_fgInited)
    {
        return(NET_DRV_NOT_INIT);
    }
    else
    if(!GET_ETHER_INFO(prMac, ETHER_DEV_RDY))
    {
        INFO_ETHER("cable is disconnected and cannot transmit\n");
        return(NET_DRV_NO_ASSOCICATION);
    }
    else
    if(!GET_ETHER_INFO(prMac, ETHER_DEV_START)) 
    {
        INFO_ETHER("ethernet driver is stoppped_tx\n");
        return(NET_DRV_ALREADY_STOP);
    }

    /* To prevent multi-task accession */
#if CONFIG_ETHER_USE_OS_NATIVE
    VERIFY(NU_Obtain_Semaphore(pt_sema, NU_SUSPEND) == NU_SUCCESS);
#else
    ETHER_SEMA_LOCK(_hEtherSendSema);
#endif
    //prPkt->ui4_time_stamp = u4NetGetTimeTick();    

    i4Ret = i4EtherTxmitFrame(prMac, prPkt);

#if CONFIG_ETHER_USE_OS_NATIVE
    VERIFY(NU_Release_Binary_Semaphore(pt_sema) == NU_SUCCESS);
#else        
    ETHER_SEMA_UNLOCK(_hEtherSendSema);
#endif    

    return (i4Ret);
}

/*******************************************************************************
**  i4EtherDrv_Set
**
**  @brief  This routine is called to set ethernet driver OIDs information
**
**  @param  prDev - pointer to net device structure
**          i4Cmd - OIDs command
**          pvParam - OIDs parameter
**  @result
**  @return 0 if success
**          negative if fail
**  @note
*******************************************************************************/
INT32 i4EtherDrv_Set(NET_DRV_IF_T *prDev, INT32 i4Cmd, VOID *pvParam)
{
    INT32 i4Ret;
    
    ASSERT(prDev != NULL);
    ASSERT(pvParam != NULL);
    
    if(!_fgInited)
    {
        return(NET_DRV_NOT_INIT);
    }
    
    if(GET_OID_GROUP((UINT32)i4Cmd) == OID_GROUP_CMN_IF) 
    {
        /* common OID region */
        i4Ret = i4CmnEtherOidSet(prDev, i4Cmd, pvParam);
    }
    else 
    if(GET_OID_GROUP((UINT32)i4Cmd) == OID_GROUP_ETHER) 
    {
        /* ethernet OID region */
        i4Ret = i4EtherOidSet(prDev, i4Cmd, pvParam);
    }
    else 
    {
        WARN_ETHER("invalid OID set command\n");
        i4Ret = NET_DRV_INV_OID;
    }

    return(i4Ret);
}

/*******************************************************************************
**  i4EtherDrv_Get
**
**  @brief  This routine is called to get ethernet driver OIDs information
**
**  @param  prDev - pointer to net device structure
**          i4Cmd - OIDs command
**          pvParam - OIDs parameter
**  @result
**  @return 0 if success
**          negative if fail
**  @note
*******************************************************************************/
INT32 i4EtherDrv_Get(NET_DRV_IF_T *prDev, INT32 i4Cmd, VOID *pvParam)
{
    INT32 i4Ret;

    ASSERT(prDev != NULL);
    ASSERT(pvParam != NULL);
    
    if(!_fgInited)
    {
        return(NET_DRV_NOT_INIT);
    }

    if(GET_OID_GROUP((UINT32)i4Cmd) == OID_GROUP_CMN_IF) 
    {
        /* common OID region */
        i4Ret = i4CmnEtherOidGet(prDev, i4Cmd, pvParam);
    }
    else 
    if(GET_OID_GROUP((UINT32)i4Cmd) == OID_GROUP_ETHER) 
    {
        /* ethernet OID region */
        i4Ret = i4EtherOidGet(prDev, i4Cmd, pvParam);
    }
    else 
    {
        WARN_ETHER("invalid OID get command\n");
        i4Ret = NET_DRV_INV_OID;
    }

    return(i4Ret);
}

void vStopSoftPollingPhy(UINT8 u1Stop)
{
  _u1StopPhyPolling = u1Stop;	
}

/*******************************************************************************
**  u4EtherReadPhy
**
**  descriptions:
**      This rotinue is called to read PHY register through MII
**  parameters:
**      u4RegAddr - PHY register offset
**  result:
**  return:
**      PHY Register value
**      negative value if fail
*******************************************************************************/
UINT32 u4ReadEthPhy(UINT32 u4RegAddr)
{
    NET_DRV_IF_T *prDev = prEtherGetDev();
    PETHER_ADAPTER_T prMac = prDev->pv_priv;
    INT32 i4Ret;
    UINT32 u4Data;
    
    i4Ret = i4MacPhyRead(prMac->u4PhyAddr, u4RegAddr, &u4Data);
    if(i4Ret == NET_DRV_OK)
    {
        NOTE_ETHER("[ReadPhy] reg:0x%04x, val:0x%04x\n", u4RegAddr, u4Data);
       
    }
    else
    {
       NOTE_ETHER("[ErrorReadPhy] reg:0x%04x, val:0x%04x\n", u4RegAddr, u4Data);	
      
    }   
    return(u4Data);
}

/*******************************************************************************
**  vEtherWritePhy
**
**  descriptions:
**      This rotinue is called to write PHY register through MII
**  parameters:
**      u4RegAddr - PHY register offset
**      u4RegData - register value wrote to PHY
**  result:
**  return:
**      NONE
*******************************************************************************/
void vWriteEthPhy(UINT32 u4RegAddr, UINT32 u4RegData)
{
    NET_DRV_IF_T *prDev = prEtherGetDev();
    PETHER_ADAPTER_T prMac = prDev->pv_priv;
    INT32 i4Ret;

    i4Ret = i4MacPhyWrite(prMac->u4PhyAddr, u4RegAddr, u4RegData);
 
    if(i4Ret != NET_DRV_OK)
    NOTE_ETHER("[ErrorWritePhy] reg:0x%04x, val:0x%04x\n", u4RegAddr, u4RegData);	
    
}


void vSet100MAmp(UINT32 chipId, UINT32 val)
{
  UINT32 u4RegValue;
  
  switch (chipId)
  { 
  	case 8550:
  	case 8555:	
  	default:
  	vWriteEthPhy(0x1f, 0x2a30); //set to Test Page	
    u4RegValue = ( u4ReadEthPhy(0x1A)&(0xfff0) )|(val & 0x0f);
    vWriteEthPhy(0x1A, u4RegValue);
    vWriteEthPhy(0x1f, 0);//set to normal Phy 	
    break;  
  }    	
}	

UINT32 u1Get100MAmp(UINT32 chipId)
{
  UINT32 u4RegValue;
  
  switch (chipId)
  { 
  	case 8550:
  	case 8555:	
  	default:
  	vWriteEthPhy(0x1f, 0x2a30); //set to Test Page	
    u4RegValue = ( u4ReadEthPhy(0x1A)&(0x000f) );
    vWriteEthPhy(0x1f, 0);//set to normal Phy 	
    break;
    
  
  }
  
  Printf(_cDBGStrDef[0], u4RegValue); 
    
  return  u4RegValue; 	
}	


void vSet10MAmp(UINT32 chipId, UINT32 val)
{
  UINT32 u4RegValue;
  
  switch (chipId)
  { 
  	case 8550:
  	case 8555:	
  	default: 	
  	
    vWriteEthPhy(0x1f, 0x2a30); //set to Test Page	
    u4RegValue = ( u4ReadEthPhy(0x18)&(0xfffC) )|((val & 0x03)<< 0);//bit 0, bit1
    vWriteEthPhy(0x18, u4RegValue);
    vWriteEthPhy(0x1f, 0);//set to normal Phy 	
    break;
 }	
 
}	

UINT32 u1Get10MAmp(UINT32 chipId)
{
  UINT32 u4RegValue;
  
  switch (chipId)
  { 
  	case 8550:
  	case 8555:	
  	default: 	
  	
    vWriteEthPhy(0x1f, 0x2a30); //set to Test Page	
    u4RegValue = (u4ReadEthPhy(0x18) >> 0)& 0x03;//bit 0, bit1
    vWriteEthPhy(0x1f, 0);//set to normal Phy 	
    break;
  }	
  
  Printf(_cDBGStrDef[7], u4RegValue); 
    
  return  u4RegValue; 	 
 
 
}	

void vSetSlewRate(UINT32 chipId, UINT32 val)
{
  UINT32 u4RegValue;
  
  switch (chipId)
  { 
  	case 8550:
  	case 8555:	
  	default:
  	
  	if( (val& 0x0f)  == 0)
  	val = 0x01;
  	else if( (val& 0x0f)  == 1)
  	val = 0x03;
  	else if( (val& 0x0f)  == 2)
  	val = 0x07;	
  	else if( (val& 0x0f)  == 3)
  	val = 0x0f;	
  	else if( (val& 0x0f)  == 4)
  	val = 0x1f;	
  	else if( (val& 0x0f)  == 5)
  	val = 0x3f;	
  	else if( (val& 0x0f)  == 6)
  	val = 0x7f;	
  	
  	vWriteEthPhy(0x1f, 0x2a30); //set to Test Page	
  	u4RegValue = ( u4ReadEthPhy(0x15)&(0xff00) )|((val & 0x7f)<< 0);//bit 0~ bit6
    vWriteEthPhy(0x15, u4RegValue);
    vWriteEthPhy(0x1f, 0);//set to normal Phy 	
  	break;
 
 }
 	
	
	
}	


UINT32 u1GetSlewRate(UINT32 chipId)
{
  UINT32 u4RegValue;
  
  switch (chipId)
  { 
  	case 8550:
  	case 8555:	
  	default:
  	
  		
  	
  	vWriteEthPhy(0x1f, 0x2a30); //set to Test Page	
  	u4RegValue = ( u4ReadEthPhy(0x15)>> 0 )&0x7f;//bit 0~ bit6
    
    if( u4RegValue  == 0x01)
  	u4RegValue = 0x0;
  	else if( u4RegValue  == 0x03)
  	u4RegValue = 0x01;
  	else if( u4RegValue  == 0x07)
  	u4RegValue = 0x02;	
  	else if( u4RegValue  == 0x0f)
  	u4RegValue = 0x03;	
  	else if( u4RegValue  == 0x1f)
  	u4RegValue = 0x4;	
  	else if( u4RegValue  == 0x3f)
  	u4RegValue = 0x5;	
  	else if( u4RegValue  == 0x7f)
  	u4RegValue = 0x6;
    
    vWriteEthPhy(0x1f, 0);//set to normal Phy 	
  	break;
 
 }
 	
   Printf(_cDBGStrDef[1], u4RegValue); 
   return  u4RegValue; 	
	
}	

void vSetInputBias(UINT32 chipId, UINT32 val)
{
  UINT32 u4RegValue;
  
  switch (chipId)
  { 
  	case 8550:
  	case 8555:	
  	default:
  	
  	if( (val& 0x03)  == 0)
  	val = 0x03;
  	else if( (val& 0x03)  == 1)
  	val = 0x02;
  	else if( (val& 0x03)  == 2)
  	val = 0x01;	
  	else if( (val& 0x03)  == 3)
  	val = 0x00;	
  	
  	vWriteEthPhy(0x1f, 0x2a30); //set to Test Page	
  	u4RegValue = ( u4ReadEthPhy(0x1D)&(0xfffc) )|((val & 0x03)<< 0);//bit 0~ bit1
    vWriteEthPhy(0x1D, u4RegValue);
    
    u4RegValue = ( u4ReadEthPhy(0x19)&(0xfffc) )|((val & 0x03)<< 0);//bit 9~ bit10
    vWriteEthPhy(0x19, u4RegValue);
    vWriteEthPhy(0x1f, 0);//set to normal Phy 	
  	
  	break;
  	
  }		
  
  
}	


UINT32 u1GetInputBias(UINT32 chipId)
{
  UINT32 u4RegValue;
  
  switch (chipId)
  { 
  	case 8550:
  	case 8555:	
  	default:
  	
  	vWriteEthPhy(0x1f, 0x2a30); //set to Test Page	
  	u4RegValue = ( u4ReadEthPhy(0x1D)& 0x03 );//bit 0~ bit1
    
    if( u4RegValue  == 0x03)
  	u4RegValue = 0x00;
  	if( u4RegValue  == 0x02)
  	u4RegValue = 0x01;
  	if( u4RegValue  == 0x01)
  	u4RegValue = 0x02;	
  	if( u4RegValue  == 0x00)
  	u4RegValue = 0x03;	
    
    vWriteEthPhy(0x1f, 0);//set to normal Phy 	
  	
  	break;
  	
  }		
  
  
   Printf(_cDBGStrDef[2], u4RegValue); 
   return  u4RegValue; 	
}

void vSetOutputBias(UINT32 chipId, UINT32 val)
{
   UINT32 u4RegValue;
  
  switch (chipId)
  { 
  	case 8550:
  	case 8555:	
  	default:
  	
  	if( (val& 0x03)  == 0)
  	val = 0x00;
  	else if( (val& 0x03)  == 1)
  	val = 0x01;
  	else if( (val& 0x03)  == 2)
  	val = 0x02;	
  	else if( (val& 0x03)  == 3)
  	val = 0x03;	
  	
  	vWriteEthPhy(0x1f, 0x2a30); //set to Test Page	
  	u4RegValue = ( u4ReadEthPhy(0x1d)&(0xffcf) )|((val & 0x03)<< 4);//bit 4~ bit5
    vWriteEthPhy(0x1d, u4RegValue);
    
    u4RegValue = ( u4ReadEthPhy(0x19)&(0xffcf) )|((val & 0x03)<< 4);//bit 4~ bit5
    vWriteEthPhy(0x19, u4RegValue);
    vWriteEthPhy(0x1f, 0);//set to normal Phy 	
  	
  	break;
  	
  }	
}	


UINT32 u1GetOutputBias(UINT32 chipId)
{
   UINT32 u4RegValue;
  
  switch (chipId)
  { 
  	case 8550:
  	case 8555:	
  	default:
  	
  	vWriteEthPhy(0x1f, 0x2a30); //set to Test Page	
  	u4RegValue = ( u4ReadEthPhy(0x1d) >> 4)& 0x03 ;//bit 2~ bit3
    
    vWriteEthPhy(0x1f, 0);//set to normal Phy 	
  	
  	break;
  	
  }	
  
   Printf(_cDBGStrDef[3], u4RegValue); 
   return  u4RegValue; 	
  
}	


void vSet50percentBW(UINT32 chipId, UINT32 val)
{

  UINT32 u4RegValue;
  
  switch (chipId)
  { 
  	case 8550:
  	case 8555:	
  	default:
  	
  	if( (val& 0x01)  == 0)
  	val = 0x00;
  	else if( (val& 0x01)  == 1)
  	val = 0x01;
  	
  	vWriteEthPhy(0x1f, 0x2a30); //set to Test Page	
  	u4RegValue = ( u4ReadEthPhy(0x1d)&(0xbfff) )|((val & 0x01)<< 14);//bit 14
    vWriteEthPhy(0x1d, u4RegValue);
  	vWriteEthPhy(0x1f, 0);//set to normal Phy 	
  	break;
  	
  }	
  
	
}	


UINT32 u1Get50percentBW(UINT32 chipId)
{

  UINT32 u4RegValue;
  
  switch (chipId)
  { 
  	case 8550:
  	case 8555:	
  	default:
  	
  	vWriteEthPhy(0x1f, 0x2a30); //set to Test Page	
  	u4RegValue = ( u4ReadEthPhy(0x18) >> 11) & 0x01;//bit 11
  	vWriteEthPhy(0x1f, 0);//set to normal Phy 	
  	break;
  	
  }	
  
  Printf(_cDBGStrDef[4], u4RegValue); 
  return  u4RegValue; 	
	
}

void vSetFeedbackCap(UINT32 chipId, UINT32 val)
{
  UINT32 u4RegValue;
  
  switch (chipId)
  { 
  	case 8550:
  	case 8555:	
  	default:
  	
  	if( (val& 0x03)  == 0)
  	val = 0x00;
  	else if( (val& 0x03)  == 1)
  	val = 0x01;
  	else if( (val& 0x03)  == 2)
  	val = 0x02;	
  	else if( (val& 0x03)  == 3)
  	val = 0x03;	
  	
  	vWriteEthPhy(0x1f, 0x2a30); //set to Test Page	
  	u4RegValue = ( u4ReadEthPhy(0x1d)&(0xfcff) )|((val & 0x03)<< 8);//bit 8, Bit9
    vWriteEthPhy(0x1d, u4RegValue);
  	vWriteEthPhy(0x1f, 0);//set to normal Phy 	
  	break;
  	
  }		
}	



UINT32 u1GetFeedbackCap(UINT32 chipId)
{
  UINT32 u4RegValue;
  
  switch (chipId)
  { 
  	case 8550:
  	case 8555:	
  	default:
  	
  	vWriteEthPhy(0x1f, 0x2a30); //set to Test Page	
  	u4RegValue = ( u4ReadEthPhy(0x1d) >> 8) & 0x03;//bit 14, Bit15
  	vWriteEthPhy(0x1f, 0);//set to normal Phy 	
  	break;
  }		
  
  Printf(_cDBGStrDef[5], u4RegValue); 
  return  u4RegValue; 	
  
}

void vSetEyeOpen(UINT32 chipId, UINT32 val)
{
  UINT32 u4RegValue;
  
  switch (chipId)
  { 
  	case 8550:
  	case 8555:	
  	default:
  	
  	vWriteEthPhy(0x1f, 0x2a30); //set to Test Page	
  	u4RegValue = ( u4ReadEthPhy(0x0e)&(0xff8f) )|((val & 0x07)<< 4);//bit 4, 5, 6
    vWriteEthPhy(0x0e, u4RegValue);
  	vWriteEthPhy(0x1f, 0);//set to normal Phy 	
  	break;
  }
}	

UINT32 u1GetEyeOpen(UINT32 chipId)
{
  UINT32 u4RegValue;
  
  switch (chipId)
  { 
  	case 8550:
  	case 8555:	
  	default:
  	
  	vWriteEthPhy(0x1f, 0x2a30); //set to Test Page	
  	u4RegValue = ( u4ReadEthPhy(0x0e) >> 4) & 0x7;//bit 4, 5, 6
  	vWriteEthPhy(0x1f, 0);//set to normal Phy 	
  	break;
  	
  }
  Printf(_cDBGStrDef[6], u4RegValue); 
  return  u4RegValue; 	
	
}

void vSetPhyLoopOn(UINT32 chipId, UINT32 val)
{
  UINT32 u4RegValue;	
  switch (chipId)
  { 
  	case 8550:
  	case 8555:	
  	default:
  	
  	vWriteEthPhy(0x1f, 0x2a30); //set to Test Page	
  	if(val == 1)
  	{
  	  u4RegValue = ( u4ReadEthPhy(0x04)| (1<<7));
    }
    else
    {
  	  u4RegValue = ( u4ReadEthPhy(0x04)& ~(1<<7));
    }
    vWriteEthPhy(0x4, u4RegValue);
  	vWriteEthPhy(0x1f, 0);//set to normal Phy 	
  	break;
  	
  }	
}	



void vSetForceMdiCrossover(UINT32 chipId, UINT32 val)
{
  UINT32 u4RegValue=0;	
  switch (chipId)
  { 
  	case 8550:
  	case 8555:	
  	default:
  	
  	vWriteEthPhy(0x1f, 0x2a30); //set to Test Page	
  	if(val == 0)
  	{
  	  u4RegValue = ( u4ReadEthPhy(0x05)& ~(0x03<<3));
    }
    else if(val == 1)
    {
     u4RegValue = ( u4ReadEthPhy(0x05)& ~(0x03<<3));//force MDI	
  	  u4RegValue = ( u4RegValue | (0x2 << 3));
    }
    else if(val == 2)
    {
     u4RegValue = ( u4ReadEthPhy(0x05)| (0x03<<3));	//force MDIX	
  	  
    }
    else 
    {
  	  u4RegValue = ( u4ReadEthPhy(0x05)& ~(0x03<<3));
    }
    vWriteEthPhy(0x5, u4RegValue);
  	vWriteEthPhy(0x1f, 0);//set to normal Phy 	
  	break;
  	
  }		
}


void vSetSpeed(UINT32 chipId, UINT32 val)
{
  
  switch (chipId)
  { 
  	case 8550:
  	case 8555:	
  	default: 	
  	
  	if(val == 0)
  	vWriteEthPhy(0x04, 0x61); //set to 10M Full
  	else if(val == 1)
  	vWriteEthPhy(0x04, 0x5e1); //set to 100M Full

    
    break;
 }	
 
}	


void vSetPhyEPG(UINT32 chipId, UINT32 val)
{
  UINT32 u4RegValue=0;	
  switch (chipId)
  { 
  	case 8550:
  	case 8555:	
  	default:
  	
  	if(val == 0)//Disable EPG
  	{
  	  
  	  vWriteEthPhy(0x1f, 0x01); //set to Test Page	
  	  u4RegValue = u4ReadEthPhy(0x1d)&0x07ff;
  	  vWriteEthPhy(0x1d, u4RegValue);// Stop EPG (bit11~bit15)
  	  vWriteEthPhy(0x1f, 0x0); //set to Normal Page	 

  	}
    else if(val == 1)//EPG random pattern
  	{
  	  ///epg 產生random pattern
  	  vWriteEthPhy(0x1f, 0x01); //set to Test Page	
  	  u4RegValue = u4ReadEthPhy(0x1d)&0x07ff;
  	  vWriteEthPhy(0x1d, u4RegValue);// Stop EPG (bit11~bit15)
  	  vWriteEthPhy(0x1e, 0x5678);// random number
  	  u4RegValue = u4RegValue|0xf800;//// Enable EPG
  	  vWriteEthPhy(0x1f, 0x0); //set to Normal Page	 
  	}
  	else if(val == 1)//EPG all one pattern
  	{
  	  ///epg 產生random pattern
  	  vWriteEthPhy(0x1f, 0x01); //set to Test Page	
  	  u4RegValue = u4ReadEthPhy(0x1d)&0x07ff;
  	  vWriteEthPhy(0x1d, u4RegValue);// Stop EPG (bit11~bit15)
  	  vWriteEthPhy(0x1e, 0xffff);//all one
  	  u4RegValue = u4RegValue|0xf800;//// Enable EPG
  	  vWriteEthPhy(0x1f, 0x0); //set to Normal Page	 
  	}
  	else if(val == 2)//EPG all zero pattern
  	{
  	  ///epg 產生random pattern
  	  vWriteEthPhy(0x1f, 0x01); //set to Test Page	
  	  u4RegValue = u4ReadEthPhy(0x1d)&0x07ff;
  	  vWriteEthPhy(0x1d, u4RegValue);// Stop EPG (bit11~bit15)
  	  vWriteEthPhy(0x1e, 0x0);//all zero
  	  u4RegValue = u4RegValue|0xf800;//// Enable EPG
  	  vWriteEthPhy(0x1f, 0x0); //set to Normal Page	 
  	}
  	
  	
  	break;
  	
  }	
}

//Only for Check sum test


/*******************************************************************************
**  i4ChkSumTestTaskCtrl
**
**  @brief  This routine is called for check sum Cli test
**
**  @param  TxEnable: Tx CheckSum Task enable, TxCkSumSize: Check Sum size (BYTE)
**          RxEnable: Rx CheckSum Task enable, RxCkSumSize: Check Sum size (BYTE) 
**  @result
**  @return NET_DRV_OK - driver init OK
**          NET_DRV_ERROR - driver init fail
**  @note
*******************************************************************************/
INT32 i4ChkSumTestTaskCtrl(BOOL TxEnable, UINT32 TxCkSumSize, BOOL RxEnable, UINT32 RxCkSumSize)
{
   INT32 i4Ret;
   EV_GRP_EVENT_T u8TxChkSumEvent;
   EV_GRP_EVENT_T u8RxChkSumEvent;

#define ETHER_TX_CHKSUM_THREAD_NAME         "Ethernet Tx Chksum Thread"
#define ETHER_TX_CHKSUM_STACK_SIZE          ETHER_STACK_SIZE
#define ETHER_TX_CHKSUM_THREAD_PRIORITY     ETHER_THREAD_PRIORITY
#define ETHER_RX_CHKSUM_THREAD_NAME         "Ethernet Rx Chksum Thread"
#define ETHER_RX_CHKSUM_STACK_SIZE          ETHER_STACK_SIZE
#define ETHER_RX_CHKSUM_THREAD_PRIORITY     ETHER_THREAD_PRIORITY
   
  _TxCkSumTestSize  = TxCkSumSize;
  _RxCkSumTestSize  = RxCkSumSize;
   
  if(TxEnable)
  { 
    if(_fgTxChkSumTaskInit ==0)
    {     
      _fgTxChkSumTaskInit =1;	
      
      u8TxChkSumEvent = 0;
    i4Ret = x_ev_group_create(&_hTxChkSumEvent, 
                              (const CHAR *)ETHER_TX_CHKSUM_EVENT_NAME,
                              u8TxChkSumEvent);
    ASSERT(i4Ret == OSR_OK);
    
      
      /* thread must be created after device init */
      i4Ret = x_thread_create(&_hChkSumTxThread,
                                 ETHER_TX_CHKSUM_THREAD_NAME,
                                 ETHER_TX_CHKSUM_STACK_SIZE,
                                 ETHER_TX_CHKSUM_THREAD_PRIORITY, 
                                 vEtherTxChkSumTaskMain,
                                 0,
                                 NULL);
        ASSERT(i4Ret == OSR_OK);
    }
  } 
  else
  {
  	_fgTxChkSumTaskInit =0;
  	
  }	
       
  if(RxEnable)
  {     
    if(_fgRxChkSumTaskInit ==0)
    { 	 
      _fgRxChkSumTaskInit = 1;	
      u8RxChkSumEvent = 0;
       
      i4Ret = x_ev_group_create(&_hRxChkSumEvent, 
                              (const CHAR *)ETHER_RX_CHKSUM_EVENT_NAME,
                              u8RxChkSumEvent);
      ASSERT(i4Ret == OSR_OK);
    
      i4Ret = x_thread_create(&_hChkSumRxThread,
                                 ETHER_RX_CHKSUM_THREAD_NAME,
                                 ETHER_RX_CHKSUM_STACK_SIZE,
                                 ETHER_RX_CHKSUM_THREAD_PRIORITY, 
                                 vEtherRxChkSumTaskMain,
                                 0,
                                 NULL);
       ASSERT(i4Ret == OSR_OK);
             
    }
   
  }
  else
  {
  	 _fgRxChkSumTaskInit = 0;	
  }	
  
  	
  return 0;	
}	

static void vEtherTxChkSumTaskMain(void* pvArg)
{
  //UINT32 u4Msg, u2MsgArg;
  //INT32  i4MsgRet;
    	
  while(_fgTxChkSumTaskInit)
  {
    //i4MsgRet = vGetTxChkSumCmd(&u4Msg);
    
    //if(i4MsgRet == OSR_OK)
    {
      
      //  u2MsgArg =  (u4Msg>>8)&0xffff;
      //  if(u2MsgArg)
      //  {
        	
      //  }	
        if(_TxCkSumTestSize ==0)// error
        _TxCkSumTestSize = 100; 
        
        _i4EtherTxChkSumVerify(_TxCkSumTestSize, 1, 1);	
        x_thread_delay(100);
        
    }	
  	
  	
  }		
  
  VERIFY(x_ev_group_delete(_hTxChkSumEvent) == OSR_OK);
   x_thread_exit();
	
	
}

static void vEtherRxChkSumTaskMain(void* pvArg)
{
 //   UINT32 u4Msg, u2MsgArg;
//  INT32  i4MsgRet;
    	
  while(_fgRxChkSumTaskInit)
  {
  //  i4MsgRet = vGetRxChkSumCmd(&u4Msg);
    
    //if(i4MsgRet == OSR_OK)
    {
      
      //  u2MsgArg =  (u4Msg>>8)&0xffff;
      //  if(u2MsgArg)
      //  {
        	
      //  }	
        
        if(_RxCkSumTestSize ==0)// error
          _RxCkSumTestSize = 100; 
          
        _i4EtherRxChkSumVerify(_RxCkSumTestSize, 1, 1);	
        x_thread_delay(100);
        
    }	
  	
  	
  }		
  
  VERIFY(x_ev_group_delete(_hRxChkSumEvent) == OSR_OK);
   x_thread_exit();
	
}

