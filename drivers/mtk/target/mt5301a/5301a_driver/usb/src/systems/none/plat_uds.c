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
 * No-OS implementation of a system for Controller Driver
 * $Revision: #1 $
 */

#include "mu_none.h"

#include "mu_mem.h"

#include "plat_cnf.h"

#include "plat_arc.h"
#include "mu_tools.h"
#include "mu_stdio.h"
#include "mu_impl.h"

/***************************** TYPES ******************************/

/* System */
typedef struct _MGC_NoneSystem
{
    MUSB_Controller *pController;
    MGC_NoneTimerWrapper *aTimer;
    void *pBoardPrivateData;
    uint16_t wNextQueueItem;
    uint16_t wNextDequeueItem;
    uint8_t *pQueueData;
    MUSB_SystemServices Services;
    uint8_t bBsrRequest;
    uint8_t bBsrRan;
} MGC_NoneSystem;

/**************************** GLOBALS *****************************/

uint8_t MGC_bSystemCount = 0;

static MGC_NoneSystem *MGC_apNoneSystem[MUSB_MAX_CONTROLLERS];

static MGC_NoneSystem _arMGC_NoneSystem[MUSB_MAX_CONTROLLERS];

static MGC_NoneTimerWrapper _arMGC_NoneTimerWrapper[MUSB_MAX_CONTROLLERS][MUSB_MAX_TIMERS];

static MGC_BsrItem _arMGC_QueueData[MUSB_MAX_CONTROLLERS][MUSB_IRP_QUEUE_LENGTH];
/*************************** FUNCTIONS ****************************/

/*
* Controller calls this to enqueue a background item
*/
static uint8_t MGC_NoneQueueBackgroundItem(void *pPrivate, const void *pItem)
{
    MGC_NoneSystem *pSystem = (MGC_NoneSystem *)pPrivate;

    uint8_t *pQueueItem = pSystem->pQueueData + (pSystem->pController->wQueueItemSize
                                                     *pSystem->wNextQueueItem++ );
    MUSB_MemCopy(pQueueItem, pItem, pSystem->pController->wQueueItemSize);

    if (pSystem->wNextQueueItem >= pSystem->pController->wQueueLength)
    {
        pSystem->wNextQueueItem = 0;
    }

    return TRUE;
}

/*
* Controller calls this to dequeue a background item
*/
static uint8_t MGC_NoneDequeueBackgroundItem(void *pPrivate, void *pItem)
{
    MGC_NoneSystem *pSystem = (MGC_NoneSystem *)pPrivate;

    uint8_t *pQueueItem = NULL;
       
    if (pSystem->wNextDequeueItem != pSystem->wNextQueueItem)
    {
        pQueueItem = pSystem->pQueueData + (pSystem->pController->wQueueItemSize
                                                *pSystem->wNextDequeueItem++ );
    }

    if (pSystem->wNextDequeueItem >= pSystem->pController->wQueueLength)
    {
        pSystem->wNextDequeueItem = 0;
    }

    if (pQueueItem)
    {
        MUSB_MemCopy(pItem, pQueueItem, pSystem->pController->wQueueItemSize);
        return TRUE;
    }

    return FALSE;
}

/*
 * Controller calls this to flush the ISR/BSR queue
 */
static void MGC_NoneFlushBackgroundQueue(void *pPrivate)
{
    MGC_NoneSystem *pSystem = (MGC_NoneSystem *)pPrivate;

    pSystem->wNextQueueItem = 0;
    pSystem->wNextDequeueItem = 0;
}

/*
* Controller calls this to enter a lock
*/
static uint32_t MGC_NoneIrqLock(void *pPrivate)
{
    MGC_NoneSystem *pSystem;
    
    MUSB_ASSERT(pPrivate);
    pSystem = (MGC_NoneSystem *)pPrivate;

    return MUSB_BoardIrqLock(pSystem->pBoardPrivateData);
}

static void MGC_NoneIrqUnlock(void *pPrivate, uint32_t dwPrevIrq)
{
    MGC_NoneSystem *pSystem;
    
    MUSB_ASSERT(pPrivate);
    pSystem = (MGC_NoneSystem *)pPrivate;

    MUSB_BoardIrqUnlock(pSystem->pBoardPrivateData, dwPrevIrq);
}

/*
 * non-OS ISR, arranged to be called by board-specific code.
 */
static uint8_t MGC_NoneControllerIsr(void *pPrivateData)
{
    int isrValue;

    volatile MGC_NoneSystem *pSystem = (MGC_NoneSystem *)pPrivateData;

    isrValue = pSystem->pController->pfIsr(pSystem->pController->pIsrParam);

    if (isrValue < 0)
    {
        return FALSE;
    }

    if (isrValue > 0)
    {
        /* flag for main loop */
        pSystem->bBsrRequest++;
    }

    return TRUE;
}

/*
 * The UCD calls this to arm a timer (periodic or one-shot).
 * Call the board-specific function.
 * wIndex: Timer Index 0 is reserved for device configuration (hub or device ep0).
 *         Timer Index 1 is reserved for class driver. 
 */
static uint8_t MGC_NoneArmTimer(void *pPrivate, uint16_t wIndex, uint32_t dwTime, uint8_t bPeriodic,
                                MUSB_pfTimerExpired pfExpireCallback, void *pExpireParam)
{
    MGC_NoneSystem *pSystem;
    MGC_NoneTimerWrapper *pTimer;

    MUSB_ASSERT(pPrivate);
    MUSB_ASSERT(pfExpireCallback);
    pSystem = (MGC_NoneSystem *)pPrivate;

    MUSB_ASSERT(wIndex < MUSB_MAX_TIMERS);
    pTimer = &pSystem->aTimer[wIndex];
    MUSB_ASSERT(pTimer);
    MUSB_ASSERT(pTimer->dwTime == 0);

    pTimer->dwCount = 0;
    pTimer->dwTime = ((dwTime /8) == 0) ? 1: (dwTime /8);
    pTimer->dwTimeOut = FALSE;
    pTimer->dwIndex = (uint32_t)wIndex;   
    pTimer->pfExpired = pfExpireCallback;
    pTimer->pExpireParam = pExpireParam;

    UNUSED(bPeriodic);
    return MUSB_BoardArmTimer(pTimer, wIndex);
}

/*
 * Cancel a timer.
 * Call the board-specific function.
 */
uint8_t MGC_NoneCancelTimer(void *pPrivate, uint16_t wIndex)
{
    MGC_NoneSystem *pSystem;
    MGC_NoneTimerWrapper *pTimer;
    uint8_t bRet = FALSE;

    if (pPrivate)
    {
        pSystem = (MGC_NoneSystem *)pPrivate;
        MUSB_ASSERT(wIndex < MUSB_MAX_TIMERS);
        pTimer = &pSystem->aTimer[wIndex];
        MUSB_ASSERT(pTimer);

        if (pTimer->dwTime > 0)
        {
            MUSB_MemSet((void *)pTimer, 0, sizeof(MGC_NoneTimerWrapper));
            MUSB_BoardCancelTimer(pTimer, wIndex);
        }

        bRet = TRUE;
    }

    return bRet;
}

/*
 * Implementation
 */
void MUSB_NoneRunBackground(void)
{
    static uint32_t dwTimeStamp = 0;    
    volatile MGC_NoneSystem *pSystem;
    uint8_t bController;
    uint32_t i;    
    MGC_NoneTimerWrapper *pTimer;

    dwTimeStamp ++;
    UNUSED(dwTimeStamp);
    for (bController = 0; bController < MGC_bSystemCount; bController++)
    {
        // Single thread mechanism: run non USB module state loop.
        MUSB_OtherModuleLoop();
    
        MUSB_ASSERT(bController < MUSB_MAX_CONTROLLERS);
        pSystem = MGC_apNoneSystem[bController];

        // Check timer time out must in front of pfBsr(). Because it will activate timer event in pfBsr().
        for (i=0; i<MUSB_MAX_TIMERS; i++)
        {
            pTimer = &pSystem->aTimer[i];
            if ((pTimer->dwTime) && (pTimer->dwTimeOut))
            {
                LOG(9, "[%d-%d] BSR: handle timer.\n", dwTimeStamp, bController);
            
                pTimer->dwTimeOut = FALSE;
                pTimer->dwTime = 0;
                // process timer event.
                if (pTimer->pfExpired)
                {
                    pTimer->pfExpired(pTimer->pExpireParam, pTimer->dwIndex);
                }
            }
        }        

        // handle interrupt.
        if (pSystem->bBsrRequest != pSystem->bBsrRan)
        {
            LOG(9, "[%d-%d] BSR: handle interrrupt.\n", dwTimeStamp, bController);

            pSystem->bBsrRan++;
            pSystem->pController->pfBsr(pSystem->pController->pBsrParam);
        }

        // handle background queue.
        if (pSystem->wNextDequeueItem != pSystem->wNextQueueItem)
        {
            LOG(9, "[%d-%d] BSR: handle queue.\n", dwTimeStamp, bController);

            pSystem->pController->pfBsr(pSystem->pController->pBsrParam);
        }
    }
}
#ifndef MUSB_CODE_SHRINK
static void MGC_NoneDestroyController(void *pPrivateData)
{
    MGC_NoneSystem *pSystem = (MGC_NoneSystem *)pPrivateData;

    /* stop controller */
    MUSB_StopController(pSystem->pController);

    /* destroy controller */
    MUSB_DestroyController(pSystem->pController);
}
#endif
static void *MGC_NoneInitController(const MUSB_NoneController *pControllerInfo)
{
    MGC_NoneSystem *pSystem;

    uint32_t dwStatus;
    uint8_t *pBaseIsr;
    uint8_t *pBaseBsr;

    MUSB_ASSERT(MGC_bSystemCount < MUSB_MAX_CONTROLLERS);
    pSystem = &_arMGC_NoneSystem[MGC_bSystemCount];

    //if (pSystem)
    {
        MUSB_MemSet(pSystem, 0, sizeof(MGC_NoneSystem));

        pSystem->Services.wVersion = MUSB_SYSTEM_SERVICES_VERSION;
        pSystem->Services.pPrivateData = (void *)pSystem;
        pSystem->Services.pfSystemToBusAddress = MUSB_BoardSystemToBusAddress;
        pSystem->Services.pfQueueBackgroundItem = MGC_NoneQueueBackgroundItem;
        pSystem->Services.pfDequeueBackgroundItem = MGC_NoneDequeueBackgroundItem;
        pSystem->Services.pfFlushBackgroundQueue = MGC_NoneFlushBackgroundQueue;
        pSystem->Services.pfArmTimer = MGC_NoneArmTimer;
        pSystem->Services.pfCancelTimer = MGC_NoneCancelTimer;
        pSystem->Services.pfIrqLock = MGC_NoneIrqLock;
        pSystem->Services.pfIrqUnlock = MGC_NoneIrqUnlock;

        /* for structured error handling: */
        do
        {
            /* try target-specific init */
            pBaseIsr = pBaseBsr = pControllerInfo->pBase;

            pSystem->pBoardPrivateData =
                MUSB_BoardInitController(pSystem, MGC_NoneControllerIsr, pControllerInfo, &pBaseIsr,
                                         &pBaseBsr);

            if (!pSystem->pBoardPrivateData)
            {
                pSystem = NULL;
                break;
            }

            /* try UCD init */
            pSystem->pController = MUSB_NewController(pControllerInfo->wType, pBaseIsr, pBaseBsr);

            if (!pSystem->pController)
            {
#ifndef MUSB_CODE_SHRINK            
                MUSB_BoardDestroyController(pSystem->pBoardPrivateData);
#endif
                //MUSB_MemFree(pSystem);
                pSystem = NULL;
                break;
            }

            /* try target-specific timer init now that we know the requirements */
            if (!MUSB_BoardInitTimers(pSystem->pBoardPrivateData))
            {
#ifndef MUSB_CODE_SHRINK
                MUSB_DestroyController(pSystem->pController);

                MUSB_BoardDestroyController(pSystem->pBoardPrivateData);
#endif                
                //MUSB_MemFree(pSystem);
                pSystem = NULL;
                break;
            }

            pSystem->pQueueData = (uint8_t *)&_arMGC_QueueData[MGC_bSystemCount][0];
            MUSB_MemSet(pSystem->pQueueData, 0,
                        pSystem->pController->wQueueLength *pSystem->pController->wQueueItemSize);

            /* prepare timer */
            MUSB_ASSERT(MGC_bSystemCount < MUSB_MAX_CONTROLLERS);
            pSystem->aTimer = &_arMGC_NoneTimerWrapper[MGC_bSystemCount][0];
            MUSB_MemSet(pSystem->aTimer, 0, MUSB_MAX_TIMERS * sizeof(MGC_NoneTimerWrapper));

            MUSB_MemSet(pSystem->aTimer, 0, sizeof(MGC_NoneTimerWrapper));

            /* finally, start the controller */
            dwStatus = MUSB_StartController(pSystem->pController, &(pSystem->Services));
            if (MUSB_STATUS_OK != dwStatus)
            {
#ifndef MUSB_CODE_SHRINK
                MUSB_DestroyController(pSystem->pController);

                MUSB_BoardDestroyController(pSystem->pBoardPrivateData);
#endif                
                pSystem = NULL;
                break;
            }
        } while (FALSE);
    }

    return pSystem;
}

/*
 * Startup code calls this.
 * Loop through all controllers and initialize them.
 */
uint8_t MUSB_InitSystem(uint32_t dwBsrPriority)
{
    MGC_NoneSystem *pSystem;

    uint8_t bIndex;

    MGC_bSystemCount = 0;
    MUSB_MemSet(MGC_apNoneSystem, 0, sizeof(MGC_apNoneSystem));

    for (bIndex = 0; bIndex < (uint8_t)MUSB_MAX_CONTROLLERS; bIndex++)
    {
        pSystem = MGC_NoneInitController(&(MUSB_aNoneController[bIndex]));
        if (pSystem)
        {
            MGC_apNoneSystem[MGC_bSystemCount++ ] = pSystem;

            // turn on irq.
            MGC_NoneIrqUnlock(pSystem, 1);            
        }
    }
    return TRUE;
}

/*
 * Loop through all initialized controller and destroy them.
 */
#ifndef MUSB_CODE_SHRINK 
void MUSB_DestroySystem(void)
{
    uint8_t bIndex;

    for (bIndex = 0; bIndex < MGC_bSystemCount; bIndex++)
    {
        MGC_NoneDestroyController(&(MGC_apNoneSystem[bIndex]));
    }
}
#endif

void MUSB_OtherModuleLoop(void)
{
/*
    NOTICE:
    
    This API should be very carefully used. It will run other module state loop
    in here. This is a work around when using blocking usb IO read/write and 
    prevent usb block other module state.    
*/
#ifdef MUSB_8226
    extern uint32_t u4UsbTick;  
    extern void vMpvState(void);
    extern void vOsdState(void);
    static uint32_t u4OldUsbTick = 0;
    
// Work around for some USB device is too busy to freeze the video.
// After asynchrouneous mode is ready, remove this code.
// u4UsbTick is 10ms per tick. Call vMpvState() and vOsdState() per 10ms.
    if (u4UsbTick >= (u4OldUsbTick + 1))
    {
    	u4OldUsbTick = u4UsbTick;
    	vMpvState();
    	vOsdState();        	
    }
    else if (u4UsbTick < u4OldUsbTick)
    {
        LOG(1, "UsbTick Wrap: OldTick=%d > Tick=%d.\n", u4OldUsbTick, u4UsbTick);
        
    	u4OldUsbTick = u4UsbTick;
    	vMpvState();
    	vOsdState();        	
    }

#else  /* 53xx*/


#endif /* #ifdef MUSB_8226 */

    return;
}

uint32_t MUSB_Sleep(uint32_t dwTime)
{
/*
    NOTICE:
    
    This API should be very carefully used. It will block other task to be executed.
    Make sure you do not use this api as possible as you can. Try to use
    MUSB_ArmTimer() by creating a new timer index and you can get a 
    non-blocking delay in your system.    
    
*/
#ifdef MUSB_8226   
    extern uint32_t u4UsbTick;
    uint32_t u4OldTick;

    u4OldTick = u4UsbTick;

    while(1)
    {
        MUSB_OtherModuleLoop();
    
        if (u4OldTick > u4UsbTick)
        {
            LOG(1, "MUSB_Sleep Wrap: u4OldTick=%d > u4UsbTick=%d.\n", u4OldTick, u4UsbTick);
        
            // Tick wrap happen.
            if ((dwTime/10) <= (u4UsbTick + (0xFFFFFFFF-u4OldTick)))
            {
                break;
            }            
        }
        else if (u4OldTick < u4UsbTick)
        {
            LOG(5, "MUSB_Sleep: u4OldTick=%d < u4UsbTick=%d.\n", u4OldTick, u4UsbTick);
            
            if ((dwTime/10) <= (u4UsbTick-u4OldTick))
            {
                break;
            }
        }
    }

#else  /* 53xx*/
    uint32_t i;
    uint32_t j;

    for (i = 0; i < dwTime*35; i++)
    {
        for (j = 0; j < 500; j++)
        {
            // do nothing.
            ;
        }
    }

#endif /* #ifdef MUSB_8226 */

    return 0;
}


#ifdef MUSB_FUNCTION 
/**
 * MUSB_Enter_Critical.
 * disable usb interrupt.
 * @param void
 * @return old usb interrupt status code
 */
uint32_t MUSB_Enter_Critical(void)
{
    uint32_t u4Ret;

    // disable USB interrupt.
    u4Ret = (uint32_t)BIM_IsIrqEnabled(VECTOR_USB);

    if (u4Ret)
    {
        VERIFY(BIM_DisableIrq(VECTOR_USB));
    }

    return u4Ret;
}

/**
 * MUSB_Exit_Critical.
 * enable usb interrupt.
 * @param enable usb interrupt 
 * @return void
 */
void MUSB_Exit_Critical(uint32_t enable)
{
    // interrupt is previous enable.
    if (enable)
    {
        // enable USB interrupt.
        VERIFY(BIM_EnableIrq(VECTOR_USB));
    }
}
#endif

