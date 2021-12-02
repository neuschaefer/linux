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

/*
 * uC/OS-II implementation of a system for Controller Driver
 * $Revision: #1 $
 */
#include "plat_arc.h"
#include "mu_tools.h"
#include "plat_cnf.h"
#include "mu_sys.h"
#include "mu_mem.h"
#include "mu_stdio.h"
#include "mu_impl.h"

/***************************** TYPES ******************************/

/** Wrapping Ucos Timer */

typedef struct
{
    uint32_t dwTime; 
    uint32_t dwTimeCount; /* 10 ms base */     
    uint32_t dwTimeOut;
    uint32_t dwIndex;
    MUSB_pfTimerExpired pfExpired;
    void *pExpireParam; /* MGC_Controller ControllerImpl */
} MGC_UcosTimerWrapper;

/* System */
typedef struct _MGC_UcosSystem
{
    MUSB_Controller *pController;
    uint8_t *aQueueMsg;
    uint32_t dwQueueHead;
    uint32_t dwQueueTail;
    MGC_UcosTimerWrapper *aTimer;
    void *pBoardPrivateData; /* MGC_AfsUds* pUds */
    MUSB_SystemServices Services;
    uint8_t bThread;
    HANDLE_T BsrLock;
    HANDLE_T h_th_hdl;
    HANDLE_T ClassLock;  /* Prevent Mbot CBW/DATA/CSW sequence is interrupted by other thread.*/
    HANDLE_T IrpLock;  /* Prevent race condition of two or more Irps writing register.*/
} MGC_UcosSystem;

/**************************** GLOBALS *****************************/

uint8_t MGC_bSystemCount = 0;

#ifdef MUSB_PORT_CONFIG
uint8_t MGC_bUsbCount = 0;
uint8_t MGC_bUsbPortDef[MUSB_MAX_CONTROLLERS];
#endif
static MGC_UcosSystem *MGC_apUcosSystem[MUSB_MAX_CONTROLLERS];

static MGC_UcosTimerWrapper MGC_aTimer[MUSB_MAX_CONTROLLERS][MUSB_MAX_TIMERS];

static uint32_t MGC_dwTimerBase = 10; /* MS */


#define MGC_SYS_LOCK()         _rCrit = x_crit_start()
#define MGC_SYS_UNLOCK()    x_crit_end(_rCrit)

static CRIT_STATE_T _rCrit;


/*************************** FUNCTIONS ****************************/

/*
 * ISR, arranged to be called by board-specific code.
 */
static uint8_t MGC_UcosControllerIsr(void *pPrivateData)
{
    int32_t isrValue;

    MGC_UcosSystem *pSystem;
    int32_t i4_ret = 0;

    MUSB_ASSERT(pPrivateData);
    pSystem = (MGC_UcosSystem *)pPrivateData;
    MUSB_ASSERT(pSystem);
    MUSB_ASSERT(pSystem->pController);
    MUSB_ASSERT(pSystem->pController->pfIsr);

    /* call controller ISR */
    isrValue = pSystem->pController->pfIsr(pSystem->pController->pIsrParam);

    /* indicate spurious interrupt if it was */
    if (isrValue < 0)
    {
        return FALSE;
    }

    /* wake BSR task if indicated */
    if (isrValue > 0)
    {
        i4_ret = (int32_t)x_sema_unlock(pSystem->BsrLock);
        MUSB_ASSERT(OSR_OK == i4_ret);
    }

    UNUSED(i4_ret);
    return TRUE;
}

//  MTK Notice: Max Liao, 2006/10/03.
//  Time out handler. Unlock BSR task to process pfExpireCallback.
static void MGC_UcosArmTimerOut(HANDLE_T pt_tm_handle, void *pv_tag)
{
    MGC_UcosSystem *pSystem;
    MGC_UcosTimerWrapper *pTimer;
    int32_t i4_ret = 0;
    uint32_t i;
    uint8_t bTimeOutOccur = FALSE;

    UNUSED(pt_tm_handle);
    MUSB_ASSERT(pv_tag);

    pSystem =  (MGC_UcosSystem *)pv_tag;
    pTimer = pSystem->aTimer;
    MUSB_ASSERT(pTimer);

    for (i=0; i<MUSB_MAX_TIMERS; i++)
    {
        if ((pTimer->dwTime) && (!pTimer->dwTimeOut))
        {
            pTimer->dwTimeCount += MGC_dwTimerBase;
            if (pTimer->dwTime == pTimer->dwTimeCount)
            {
                pTimer->dwTimeOut = TRUE;
                bTimeOutOccur = TRUE;
            }
        }
        pTimer ++;
    }

    if (bTimeOutOccur)
    {
        i4_ret = (int32_t)x_sema_unlock(pSystem->BsrLock);        
        MUSB_ASSERT(OSR_OK == i4_ret);
        UNUSED(i4_ret);
    }
}

/*
 * The UCD calls this to arm a timer (periodic or one-shot).
 * Call the board-specific function.
 * wIndex: 
 * Timer Index 0 is reserved for EP0 device configuration. 
 * Timer Index 1 is reserved for Hub class device configuration. 
 * Timer Index 2 is reserved for Mass storage class driver. 
 */
static uint8_t MGC_UcosArmTimer(void *pPrivate, uint16_t wIndex, uint32_t dwTime, uint8_t bPeriodic,
                                MUSB_pfTimerExpired pfExpireCallback, void *pExpireParam)
{
    MGC_UcosSystem *pSystem;
    MGC_UcosTimerWrapper *pTimer;
    
    MUSB_ASSERT(pPrivate);
    MUSB_ASSERT(pfExpireCallback);
    UNUSED(bPeriodic);
    
    pSystem = (MGC_UcosSystem *)pPrivate;

    MUSB_ASSERT(wIndex < MUSB_MAX_TIMERS);
    pTimer = &pSystem->aTimer[wIndex];
    MUSB_ASSERT(pTimer);
    MUSB_ASSERT(pTimer->dwTime == 0);
    
    pTimer->dwTime = dwTime;
    pTimer->dwTimeCount = 0;
    pTimer->pfExpired = pfExpireCallback;
    pTimer->dwTimeOut = FALSE;
    pTimer->dwIndex = (uint32_t)wIndex;
    pTimer->pExpireParam = pExpireParam;

    return TRUE;
}

/*
 * The UCD calls this to cancel a timer.
 * Call the board-specific function.
 */
static uint8_t MGC_UcosCancelTimer(void *pPrivate, uint16_t wIndex)
{
    MGC_UcosSystem *pSystem;
    MGC_UcosTimerWrapper *pTimer;    
    uint8_t bRet = FALSE;

    if (pPrivate)
    {
        pSystem = (MGC_UcosSystem *)pPrivate;
        MUSB_ASSERT(wIndex < MUSB_MAX_TIMERS);
        pTimer = &pSystem->aTimer[wIndex];
        MUSB_ASSERT(pTimer);

        if (pTimer->dwTime > 0)
        {
            MUSB_MemSet((void *)pTimer, 0, sizeof(MGC_UcosTimerWrapper));
        }
        bRet = TRUE;
    }

    return bRet;
}

/*
* Controller calls this to enqueue a background item
*/
static uint8_t MGC_UcosQueueBackgroundItem(void *pPrivate, const void *pItem)
{
    MGC_UcosSystem *pSystem;

    MUSB_ASSERT(pPrivate);
    MUSB_ASSERT(pItem);
    pSystem = (MGC_UcosSystem *)pPrivate;
    MUSB_ASSERT(pSystem->pController);

    /* Post a message to the queue */
    if (((pSystem->dwQueueHead + 1) %pSystem->pController->wQueueLength) != pSystem->dwQueueTail)
    {
        MUSB_MemCopy((void *)(
                         (uint32_t)pSystem->aQueueMsg + (pSystem->dwQueueHead
                                                             *pSystem->pController->wQueueItemSize)),
                     pItem,
                     pSystem->pController->wQueueItemSize);

        pSystem->dwQueueHead = (pSystem->dwQueueHead + 1) % pSystem->pController->wQueueLength;

        VERIFY(OSR_OK == x_sema_unlock(pSystem->BsrLock));
        
        return (TRUE);
    }

    return (FALSE);
}

/*
* Controller calls this to dequeue a background item
*/
static uint8_t MGC_UcosDequeueBackgroundItem(void *pPrivate, void *pItem)
{
    MGC_UcosSystem *pSystem;

    MUSB_ASSERT(pPrivate);
    MUSB_ASSERT(pItem);
    pSystem = (MGC_UcosSystem *)pPrivate;
    MUSB_ASSERT(pSystem->pController);

    /* Read a message from Queue */
    if (pSystem->dwQueueTail != pSystem->dwQueueHead)
    {
        MUSB_MemCopy(pItem,
                     (void *)((uint32_t)
                                  pSystem->aQueueMsg + (pSystem->dwQueueTail
                                                            *pSystem->pController->wQueueItemSize)),
                     pSystem->pController->wQueueItemSize);

        pSystem->dwQueueTail = (pSystem->dwQueueTail + 1) % pSystem->pController->wQueueLength;

        return (TRUE);
    }

    return (FALSE);
}

static void MGC_UcosFlushBackgroundQueue(void *pPrivate)
{
    MGC_UcosSystem *pSystem;

    MUSB_ASSERT(pPrivate);
    pSystem = (MGC_UcosSystem *)pPrivate;

    pSystem->dwQueueTail = 0;
    pSystem->dwQueueHead = 0;
    return;
}

/*
* Controller calls this to enter a lock
*/
static uint8_t MGC_UcosClassLock(void *pPrivate, uint16_t wIndex)
{
    MGC_UcosSystem *pSystem;

    int32_t i4_ret = 0;

    MUSB_ASSERT(pPrivate);
    UNUSED(wIndex);    
    
    pSystem = (MGC_UcosSystem *)pPrivate;

    i4_ret = (int32_t)x_sema_lock(pSystem->ClassLock, X_SEMA_OPTION_WAIT);
    MUSB_ASSERT(OSR_OK == i4_ret);
    UNUSED(i4_ret);    
    return (TRUE);
}

/*
* Controller calls this to exit a lock
*/
static uint8_t MGC_UcosClassUnlock(void *pPrivate, uint16_t wIndex)
{
    MGC_UcosSystem *pSystem;

    int32_t i4_ret = 0;

    MUSB_ASSERT(pPrivate);
    UNUSED(wIndex);    
    
    pSystem = (MGC_UcosSystem *)pPrivate;

    /* Release the semaphore */
    i4_ret = (int32_t)x_sema_unlock(pSystem->ClassLock);

    return (OSR_OK == i4_ret);
}

/*
* Controller calls this to enter a lock
*/
static uint8_t MGC_UcosIrpLock(void *pPrivate, uint16_t wIndex)
{
    MGC_UcosSystem *pSystem;

    int32_t i4_ret = 0;

    MUSB_ASSERT(pPrivate);
    UNUSED(wIndex);    
    #if 1
    UNUSED(pPrivate); 
    UNUSED(i4_ret);
    UNUSED(pSystem);
    MGC_SYS_LOCK();
    return TRUE;
    #else
    pSystem = (MGC_UcosSystem *)pPrivate;

    i4_ret = (int32_t)x_sema_lock(pSystem->IrpLock, X_SEMA_OPTION_WAIT);

    return (OSR_OK == i4_ret);
    #endif
}

/*
* Controller calls this to exit a lock
*/
static uint8_t MGC_UcosIrpUnlock(void *pPrivate, uint16_t wIndex)
{
    MGC_UcosSystem *pSystem;

    int32_t i4_ret = 0;

    MUSB_ASSERT(pPrivate);
    UNUSED(wIndex);    
    #if 1
    UNUSED(pPrivate); 
    UNUSED(i4_ret);
    UNUSED(pSystem);
    MGC_SYS_UNLOCK();
    #else    
    pSystem = (MGC_UcosSystem *)pPrivate;

    /* Release the semaphore */
    i4_ret = (int32_t)x_sema_unlock(pSystem->IrpLock);
    MUSB_ASSERT(OSR_OK == i4_ret);
    UNUSED(i4_ret);
    #endif
    return (TRUE);
}

/*
* Controller calls this to enter a lock
*/
static uint32_t MGC_UcosIrqLock(void *pPrivate)
{
    MGC_UcosSystem *pSystem;
    
    MUSB_ASSERT(pPrivate);
    pSystem = (MGC_UcosSystem *)pPrivate;

    return MUSB_BoardIrqLock(pSystem->pBoardPrivateData);
}

static void MGC_UcosIrqUnlock(void *pPrivate, uint32_t dwPrevIrq)
{
    MGC_UcosSystem *pSystem;
    
    MUSB_ASSERT(pPrivate);
    pSystem = (MGC_UcosSystem *)pPrivate;

    MUSB_BoardIrqUnlock(pSystem->pBoardPrivateData, dwPrevIrq);
}

static uint32_t MGC_UcosIsIrqPending(void *pPrivate)
{
    MGC_UcosSystem *pSystem;
    
    MUSB_ASSERT(pPrivate);
    pSystem = (MGC_UcosSystem *)pPrivate;

    return MUSB_BoardIsIrqPending(pSystem->pBoardPrivateData);
}

/* Back ground service Routine */
static void MGC_UcosBsrThread(void *pParam)
{
    int32_t i4Ret = 0;
    MGC_UcosSystem *pSystem;
    uint32_t i;
    MGC_UcosTimerWrapper *pTimer;
    
    UNUSED(i4Ret);
    MUSB_ASSERT(pParam);
    pSystem = (MGC_UcosSystem *)(*((uint32_t *)pParam));

    while (1)
    {
        i4Ret = (int32_t)x_sema_lock(pSystem->BsrLock, X_SEMA_OPTION_WAIT);

        MUSB_ASSERT(i4Ret == OSR_OK);

        // Check timer time out must in front of pfBsr(). Because it will activate timer event in pfBsr().
        for (i=0; i<MUSB_MAX_TIMERS; i++)
        {
            pTimer = &pSystem->aTimer[i];
            if ((pTimer->dwTime) && (pTimer->dwTimeOut))
            {
                pTimer->dwTimeOut = FALSE;
                pTimer->dwTime = 0;
                // process timer event.
                if (pTimer->pfExpired)
                {
                    pTimer->pfExpired(pTimer->pExpireParam, pTimer->dwIndex);
                }
            }
        }

        // check isr.
        if (pSystem->dwQueueTail != pSystem->dwQueueHead)
        {
            pSystem->pController->pfBsr(pSystem->pController->pBsrParam);
        }
    }
}

/*
 * Delete all the resources allocated for the given system
 */
#ifndef MUSB_CODE_SHRINK
static void MGC_UcosDestroyController(MGC_UcosSystem *pSystem)
{
    //    uint16_t wIndex;
    int32_t i4_ret = 0;

    MUSB_ASSERT(pSystem);

#ifndef MUSB_CODE_SHRINK
    MUSB_StopController(pSystem->pController);
    MUSB_DestroyController(pSystem->pController);
    MUSB_BoardDestroyController(pSystem->pBoardPrivateData);
#endif

    /* Delete the Queues created and release corresponding memory */

    if (pSystem->aQueueMsg)
    {
        MUSB_MemFree(pSystem->aQueueMsg);
        pSystem->aQueueMsg = NULL;
    }

    /* Delete the timer created and release corresponding memory */
    pSystem->aTimer = NULL;

    /* destroy mailbox for BSR task */
    i4_ret = x_sema_delete(pSystem->BsrLock);
    MUSB_ASSERT(OSR_OK == i4_ret);

    /* destroy mailbox for class driver */
    i4_ret = x_sema_delete(pSystem->ClassLock);
    MUSB_ASSERT(OSR_OK == i4_ret);

    i4_ret = x_sema_delete(pSystem->IrpLock);
    MUSB_ASSERT(OSR_OK == i4_ret);

    /* Delete thread */
    if (pSystem->bThread)
    {
        /* destroy BSR task */
        x_thread_exit();
        pSystem->bThread = 0;
    }

    MUSB_MemFree(pSystem);

    UNUSED(i4_ret);    
}
#endif
/*
* Initialize the system with the services
*/
static MGC_UcosSystem *MGC_UcosInitController(const MUSB_UcosController *pControllerInfo,
                                              uint8_t bBsrPriority)
{
    MGC_UcosSystem *pSystem;

    uint8_t *pBaseIsr;
    uint8_t *pBaseBsr;
    int32_t err;
    //    uint16_t 	wIndex;
    uint32_t dwStatus;
    CHAR szPSbsr [8] = {0};

    pSystem = (MGC_UcosSystem *)MUSB_MemAlloc(sizeof(MGC_UcosSystem));

    if (pSystem)
    {
        pSystem->Services.wVersion = MUSB_SYSTEM_SERVICES_VERSION;

        pSystem->Services.pPrivateData = (void *)pSystem;
        pSystem->Services.pfSystemToBusAddress = MUSB_BoardSystemToBusAddress;
        pSystem->Services.pfQueueBackgroundItem = MGC_UcosQueueBackgroundItem;
        pSystem->Services.pfDequeueBackgroundItem = MGC_UcosDequeueBackgroundItem;
        pSystem->Services.pfFlushBackgroundQueue = MGC_UcosFlushBackgroundQueue;
        pSystem->Services.pfArmTimer = MGC_UcosArmTimer;
        pSystem->Services.pfCancelTimer = MGC_UcosCancelTimer;
        pSystem->Services.pfClassLock = MGC_UcosClassLock;
        pSystem->Services.pfClassUnlock = MGC_UcosClassUnlock;
        pSystem->Services.pfIrpLock = MGC_UcosIrpLock;
        pSystem->Services.pfIrpUnlock = MGC_UcosIrpUnlock;
        pSystem->Services.pfIrqLock = MGC_UcosIrqLock;
        pSystem->Services.pfIrqUnlock = MGC_UcosIrqUnlock;
        pSystem->Services.pfIsIrqPending = MGC_UcosIsIrqPending;

        /* for structured error handling: */
        /* try target-specific init */
        MUSB_ASSERT(pControllerInfo);

        pBaseIsr = pControllerInfo->pBase;
        pBaseBsr = pControllerInfo->pBase;
        pSystem->pBoardPrivateData =
            MUSB_BoardInitController(pSystem, MGC_UcosControllerIsr, pControllerInfo, &pBaseIsr,
                                     &pBaseBsr);

        if (!pSystem->pBoardPrivateData)
        {
            MUSB_MemFree(pSystem);
            return NULL;
        }
        /* try UCD init */
        pSystem->pController = MUSB_NewController(pControllerInfo->wType, pBaseIsr, pBaseBsr);

        if (!pSystem->pController)
        {
#ifndef MUSB_CODE_SHRINK       
            MUSB_BoardDestroyController(pSystem->pBoardPrivateData);
#endif
            MUSB_MemFree(pSystem);
            return NULL;
        }
                             
        /* prepare queue */
        pSystem->aQueueMsg = MUSB_MemAlloc(
                                 pSystem->pController->wQueueLength *pSystem->pController->wQueueItemSize);

        if (!pSystem->aQueueMsg)
        {
#ifndef MUSB_CODE_SHRINK              
            MGC_UcosDestroyController(pSystem);
#endif
            MUSB_MemFree(pSystem);
            return NULL;
        }

        MUSB_MemSet(pSystem->aQueueMsg, 0,
                    pSystem->pController->wQueueLength *pSystem->pController->wQueueItemSize);

        /* Initialize queue head and tail to null */
        pSystem->dwQueueTail = 0;
        pSystem->dwQueueHead = 0;

        /* prepare timer */
        MUSB_ASSERT(MGC_bSystemCount < MUSB_MAX_CONTROLLERS);
        if (MGC_bSystemCount >= MUSB_MAX_CONTROLLERS)
        {
#ifndef MUSB_CODE_SHRINK              
            MGC_UcosDestroyController(pSystem);
#endif
            MUSB_MemFree(pSystem);
            return NULL;
        }        
        pSystem->aTimer = &MGC_aTimer[MGC_bSystemCount][0];
        MUSB_MemSet(pSystem->aTimer, 0, MUSB_MAX_TIMERS * sizeof(MGC_UcosTimerWrapper));
        
        /* create mailbox to wake the BSR task */
        VERIFY(OSR_OK == x_sema_create(&pSystem->BsrLock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK));

        //  MTK Notice: Max Liao, 2006/11/23.
        //  Class semphore protect BSR task and user's task sending job at the same time.
        VERIFY(OSR_OK == x_sema_create(&pSystem->ClassLock, X_SEMA_TYPE_BINARY,
                                       X_SEMA_STATE_UNLOCK));

        //Prevent race condition of two or more Irps writing register.
        VERIFY(OSR_OK == x_sema_create(&pSystem->IrpLock, X_SEMA_TYPE_BINARY,
                                       X_SEMA_STATE_UNLOCK));

        /* try target-specific timer init now that we know the requirements */
        if (!MUSB_BoardInitTimers(pSystem->pBoardPrivateData))
        {
#ifndef MUSB_CODE_SHRINK
            MGC_UcosDestroyController(pSystem);
#endif
            MUSB_MemFree(pSystem);
            return NULL;
        }

        // Repeat timer.
        (void)MUSB_BoardArmTimer(pSystem->pBoardPrivateData, 0, MGC_dwTimerBase, TRUE,
                              MGC_UcosArmTimerOut, pSystem);

        /* start the controller */
        dwStatus = MUSB_StartController(pSystem->pController, &(pSystem->Services));

        if (MUSB_STATUS_OK != dwStatus)
        {
#ifndef MUSB_CODE_SHRINK               
            MGC_UcosDestroyController(pSystem);
#endif
            MUSB_MemFree(pSystem);
            return NULL;
        }

        SPrintf(szPSbsr, "USB_%d", pControllerInfo->dwInfo);

        /* Create the BSR task, Top of the stack has to be passed  */
        err = (int32_t)x_thread_create(&pSystem->h_th_hdl,
            (const CHAR  *)szPSbsr,
            MUSB_UCOS_TASK_STACK_SIZE,
            (UINT8)((MUSB_UCOS_TASK_PRIORITY + bBsrPriority) - MGC_bSystemCount),
            (void (*) (void *))MGC_UcosBsrThread, sizeof(VOID *),
            (void *)&pSystem);

        if (OSR_OK != err)
        {
#ifndef MUSB_CODE_SHRINK               
            MGC_UcosDestroyController(pSystem);
#endif
            MUSB_MemFree(pSystem);
            return NULL;
        }
        pSystem->bThread = (MUSB_UCOS_TASK_PRIORITY + bBsrPriority) - MGC_bSystemCount;
    }

    return pSystem;
}

#ifdef MUSB_PORT_CONFIG
/*
 * Startup code calls this
 */
uint8_t MUSB_InitSystem(uint32_t dwBsrPriority)
{
    MGC_UcosSystem *pSystem;

    uint8_t bIndex;

    MGC_bSystemCount = 0;
    MUSB_MemSet(MGC_apUcosSystem, 0, sizeof(MGC_apUcosSystem));

    for (bIndex = 0; bIndex < (uint8_t)MUSB_MAX_CONTROLLERS; bIndex++)
    {
    	if(MGC_bUsbPortDef[bIndex])
	{
	    LOG(0, "USB Port %d is initial.\n", bIndex);
	    pSystem = MGC_UcosInitController(&(MUSB_aUcosController[bIndex]), 
	                        (uint8_t)(dwBsrPriority & 0xff));
	    if (pSystem)
	    {
	        MGC_apUcosSystem[MGC_bSystemCount++ ] = pSystem;

	        // turn on irq.
	        MGC_UcosIrqUnlock(pSystem, 1);
	    }
	}
    }

    return TRUE;
}
#else
/*
 * Startup code calls this
 */
uint8_t MUSB_InitSystem(uint32_t dwBsrPriority)
{
    MGC_UcosSystem *pSystem;

    uint8_t bIndex;

    MGC_bSystemCount = 0;
    MUSB_MemSet(MGC_apUcosSystem, 0, sizeof(MGC_apUcosSystem));

    for (bIndex = 0; bIndex < (uint8_t)MUSB_MAX_CONTROLLERS; bIndex++)
    {
	        pSystem = MGC_UcosInitController(&(MUSB_aUcosController[bIndex]), 
	                            (uint8_t)(dwBsrPriority & 0xff));
	        if (pSystem)
	        {
	            MGC_apUcosSystem[MGC_bSystemCount++ ] = pSystem;

	            // turn on irq.
	            MGC_UcosIrqUnlock(pSystem, 1);
	        }
    }
    return TRUE;
}
#endif


#ifndef MUSB_CODE_SHRINK
void MUSB_DestroySystem(void)
{
    uint8_t bIndex;

    for (bIndex = 0; bIndex < MGC_bSystemCount; bIndex++)
    {
        MGC_UcosDestroyController(MGC_apUcosSystem[bIndex]);
    }
}
#endif

uint32_t MUSB_Sleep(uint32_t dwTime)
{
    x_thread_delay(dwTime);

    return 0;
}

