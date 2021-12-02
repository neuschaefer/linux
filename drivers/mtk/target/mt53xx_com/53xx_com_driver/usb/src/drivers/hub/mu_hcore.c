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
 * Hub and port status change handling
 * $Revision: #1 $
 */

#ifdef MUSB_HUB

#include "mu_bits.h"
#include "mu_cdi.h"
#include "mu_descs.h"
#include "mu_diag.h"
#include "mu_mem.h"

#include "mu_hub.h"

#include "mu_hdef.h"
#include "mu_hcore.h"
#include "mu_hcreq.h"

#include "mu_impl.h"

static void MGC_HubTimerExpired(void *pControllerPrivateData, uint16_t wTimerIndex)
{
    MUSB_Hub *pHubDevice;
    MUSB_pfDriverTimerExpired pfHubTimerExpired;
    void* pHubTimerData;
    
    MUSB_ASSERT(pControllerPrivateData);
    pHubDevice = (MUSB_Hub *)pControllerPrivateData;    

    pfHubTimerExpired = pHubDevice->pfHubTimerExpired;       
    if (pfHubTimerExpired)
    {
        pHubTimerData = pHubDevice->pHubTimerData;

        pHubDevice->pHubTimerData = NULL;
        pHubDevice->pfHubTimerExpired = NULL;
    
        pfHubTimerExpired(pHubTimerData, pHubDevice->busHandle);
    }

    UNUSED(wTimerIndex);
}

/*******************************************************************                        
 *                 MGC_HubStartTimer ()                  *
 *******************************************************************/
uint32_t MGC_HubStartTimer(MUSB_BusHandle hBus, 
                       uint8_t bTimerIndex, uint32_t dwTime,
                       MUSB_pfDriverTimerExpired pfDriverTimerExpired, MUSB_Hub *pHubDevice)
{
    uint8_t bOk;

    MGC_Port *pPort;
    MUSB_SystemServices *pServices;

    // check hub timer empty.
    MUSB_ASSERT(pHubDevice);

    if (pHubDevice->pfHubTimerExpired)
    {
        LOG(0, "Hub timer still in used. pfHubTimerExpired = 0x%08X.\n", 
            pHubDevice->pfHubTimerExpired);
        return (uint32_t)MUSB_STATUS_NO_RESOURCES;    
    }
    
    pHubDevice->pfHubTimerExpired = pfDriverTimerExpired;
    pHubDevice->pHubTimerData = (void *)pHubDevice;

    MUSB_ASSERT(hBus);
    pPort = (MGC_Port *)hBus;
    MUSB_ASSERT(pPort->pController);
    pServices = pPort->pController->pSystemServices;
    MUSB_ASSERT(pServices);    
    MUSB_ASSERT(bTimerIndex < MUSB_HUB_TIMER);    
    bOk = pServices->pfArmTimer(pServices->pPrivateData, 
        (MUSB_HUB_TIMER_IDX + bTimerIndex) /*bTimerIndex*/, dwTime, FALSE, MGC_HubTimerExpired, pHubDevice);

    MUSB_ASSERT(bOk);
    return (uint32_t)((bOk) ? MUSB_STATUS_SUCCESS : MGC_M_STATUS_ERROR);
}

/*******************************************************************                        
 *                 MGC_HubStopTimer ()                  *
 *******************************************************************/
uint32_t MGC_HubStopTimer(MUSB_BusHandle hBus, uint8_t bTimerIndex, 
    MUSB_Hub *pHubDevice)
{
    uint8_t bOk;

    MGC_Port *pPort;
    MUSB_SystemServices *pServices;

    // check hub timer empty.
    MUSB_ASSERT(pHubDevice);

    if (pHubDevice->pfHubTimerExpired)
    {
        pHubDevice->pfHubTimerExpired = NULL;
        pHubDevice->pHubTimerData = NULL;

        MUSB_ASSERT(hBus);
        pPort = (MGC_Port *)hBus;
        MUSB_ASSERT(pPort->pController);
        pServices = pPort->pController->pSystemServices;
        MUSB_ASSERT(pServices);    
        MUSB_ASSERT(bTimerIndex < MUSB_HUB_TIMER);    
        bOk = pServices->pfCancelTimer(pServices->pPrivateData, 
            (MUSB_HUB_TIMER_IDX + bTimerIndex) /*bTimerIndex*/);

        MUSB_ASSERT(bOk);
        return (uint32_t)((bOk) ? MUSB_STATUS_SUCCESS : MGC_M_STATUS_ERROR);
    }
    
    return MUSB_STATUS_SUCCESS;
}

/*******************************************************************
 *                 MUSB_HubStatusChange ()                         *
 *******************************************************************/

void MUSB_HubStatusChange(MUSB_Hub *pHubDevice)
{
    uint32_t dwStatus;

    MUSB_ASSERT(pHubDevice);

    MUSB_DIAG_STRING(2, "MUSB_HubStatusChange() >>");

    /* Get hub status from the connected Hub. */
    pHubDevice->bNextState = MUSB_HUB_STATE_GOT_HUB_STATUS;
    dwStatus = MUSB_HubGetHubStatus(pHubDevice, MGC_HubGetHubStatusCallback);
    MUSB_DIAG_STRING(2, "MUSB_HubStatusChange() <<");
    UNUSED(dwStatus);
    return;
} /* End of function MUSB_HubStatusChange () */

/*******************************************************************                        
 *                 MGC_HubGetHubStatusCallback ()                  *
 *******************************************************************/
void MGC_HubGetHubStatusCallback(void *pContext, MUSB_ControlIrp *pControlIrp)
{
    MUSB_Hub *pHubDevice;

    uint32_t dwStatus = 0;

    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pControlIrp);

    pHubDevice = (MUSB_Hub *)pContext;

    if (MUSB_SUCCESS != pControlIrp->dwStatus)
    {
        /* This is error condition so check which error has come and 
         * do the things as per requirement.
         */
        MUSB_DIAG_STRING(1, "HubError: pControlIrp->dwStatus in MGC_HubGetHubStatusCallback()");

        MUSB_DIAG1(2, "HubState:", pHubDevice->bState, 16, 0x00);
        MUSB_DIAG1(1, "IrpStatus:", pControlIrp->dwStatus, 16, 0x00);
        return;
    }

    /* Since IRP is executed properly hence update hub state to its next state.
     */
    pHubDevice->bState = pHubDevice->bNextState;

    MUSB_DIAG1(2, "HubState:", pHubDevice->bState, 16, 0x00);

    if (MUSB_HUB_STATE_GOT_HUB_STATUS == pHubDevice->bState)
    {
        /* If control comes here it means Status is availble. Store it into proper
         * place. 
         */
        MGC_HubStoreStatus(pHubDevice->aHubPortStatus, &(pHubDevice->hubStatus));
        if (pHubDevice->hubStatus.wStatusChange & (uint16_t)MUSB_HUB_LOCAL_POWER_STATUS_CHANGED_BM)
        {
            /* This function sends clear feature requests and make 
             * pHubNextState to MUSB_HUB_STATE_C_LOCAL_POWER_COMPLETED.
             */
            MUSB_DIAG_STRING(2, "HUB: LocalPowerStatusChange");

            pHubDevice->bNextState = MUSB_HUB_STATE_C_LOCAL_POWER_COMPLETED;
            dwStatus = MUSB_HubClearHubFeature(pHubDevice, (uint16_t)MUSB_C_HUB_LOCAL_POWER_FEATURE,
                                               MGC_HubGetHubStatusCallback);
            return;
        }
        else
        {
            /* Since there is no change in local power supply bm hence go ahead and
             * check for over current condition by making present state 
             * MUSB_HUB_STATE_C_LOCAL_POWER_COMPELETED.
             */
            pHubDevice->bState = MUSB_HUB_STATE_C_LOCAL_POWER_COMPLETED;
        }
    } /* End of if(MUSB_HUB_STATE_GOT_HUB_STATUS) */

    if (MUSB_HUB_STATE_C_LOCAL_POWER_COMPLETED == pHubDevice->bState)
    {
        /* Control comes here it means check as well as remedy for local
         * power supply has been done. Now time to check over current condition.
         */
        if (pHubDevice->hubStatus.wStatusChange & (uint16_t)MUSB_HUB_OVER_CURRENT_STATUS_CHANGED_BM)
        {
            /* Host recovery action for an over current event: As per 11.12.5 USB
             * 2.0 Specification.
             */
            MUSB_DIAG_STRING(1, "HUB: OverCurrent status change");

            pHubDevice->bNextState = MUSB_HUB_STATE_CHECK_OVER_CURRENT_EXISTANCE;
            /* First clear the over current status change bit. */
            dwStatus = MUSB_HubClearHubFeature(pHubDevice,
                                               (uint16_t)MUSB_C_HUB_OVER_CURRENT_FEATURE,
                                               MGC_HubGetHubStatusCallback);
        }
        else
        {
            /* If over current status has not changed then checking status of 
             * HUB is over go ahead check status for all ports one by one.
             */
            MUSB_DIAG_STRING(2, "HUB: Hub status change processing finished");

            pHubDevice->bState = MUSB_HUB_STATE_INIT;
            pHubDevice->bNextState = MUSB_HUB_STATE_INIT;
            pHubDevice->bActivePortNum = MUSB_HUB_STARTING_PORT_NUMBER;
            MGC_HubPortStatusChange(pHubDevice);
            return;
        } /* End of else  */
    }     /* End of if(MUSB_HUB_STATE_C_LOCAL_POWER_COMPLETED) */

    if (MUSB_HUB_STATE_CHECK_OVER_CURRENT_EXISTANCE == pHubDevice->bState)
    {
        if (pHubDevice->hubStatus.wStatus & (uint16_t)MUSB_HUB_OVER_CURRENT_EXISTS_BM)
        {
            /* Wait for 400 ms */
            /* This function call system timer function with proper callback.
             */
            MUSB_DIAG_STRING(1, "HUB: OverCurrent exists");

            pHubDevice->bNextState = MUSB_HUB_STATE_C_HUB_OVER_CURRENT;
            MGC_HubHandleHubOverCurrentTimer(pHubDevice, 400);
            return;
        }
        else
        {
            MUSB_DIAG_STRING(2, "HUB: OverCurrent doesn't exist");

            pHubDevice->bState = MUSB_HUB_STATE_INIT;
            pHubDevice->bNextState = MUSB_HUB_STATE_INIT;
            pHubDevice->bActivePortNum = MUSB_HUB_STARTING_PORT_NUMBER;
            MGC_HubPortStatusChange(pHubDevice);
            return;
        }
    }

    if (MUSB_HUB_STATE_C_HUB_OVER_CURRENT == pHubDevice->bState)
    {
        /* Since over current exist and clear feature for overcurrent 
         * is already done. Now its time to power on all the ports
         * again after waiting for few ms.
         */
        /* Start from port 0x01  */
        pHubDevice->bActivePortNum = MUSB_HUB_STARTING_PORT_NUMBER;

        MGC_HubHandlePowerOnPorts(pHubDevice, &(pHubDevice->bNextState));
        return;
    }

    if (MUSB_HUB_STATE_POWER_ON == pHubDevice->bState)
    {
        /* If control comes here it means PORT_POWER_FEATURE for port
         * reperesented by bActivePortNum is set and do same thing for 
         * next port. Hub will remain in this state till 
         * pHubDevice->bActivePortNum == bNumOfPort.
         */
        pHubDevice->bActivePortNum++;

        if (pHubDevice->bActivePortNum < pHubDevice->hubDscr.bNbrPorts)
        {
            pHubDevice->bNextState = MUSB_HUB_STATE_POWER_ON;
        }
        else
        {
            /* This request is going for last port in the hub. */
            pHubDevice->bNextState = MUSB_HUB_STATE_POWER_ON_COMPLETE;
        }

        dwStatus = MUSB_HubSetPortFeature(pHubDevice, (uint16_t)MUSB_HUB_PORT_POWER_FEATURE,
                                          MGC_HubGetHubStatusCallback, pHubDevice->bActivePortNum);
        return;
    }

    if (MUSB_HUB_STATE_POWER_ON_COMPLETE == pHubDevice->bState)
    {
        /* Call callbackWait for (bPwrOn2PwrGood *2) msecond  so that power on ports will 
         * be good. As per 11.23.2.1 bPwrOn2PwrGood
         */
        pHubDevice->bState = MUSB_HUB_STATE_INIT;

        pHubDevice->bNextState = MUSB_HUB_STATE_INIT;
        MGC_HubPowerOnGoodTimer(pHubDevice, (uint32_t)((pHubDevice->hubDscr.bPwrOn2PwrGood) *2));
        return;
    }

    if (MUSB_HUB_STATE_INIT == pHubDevice->bState)
    {
        /* Hub Status Has changed and its time to go for port status. 
         */
        MUSB_DIAG_STRING(2, "HUB: Hub status change processing finish 1");

        pHubDevice->bActivePortNum = MUSB_HUB_STARTING_PORT_NUMBER;
        MGC_HubPortStatusChange(pHubDevice);
    }

    UNUSED(dwStatus);
    return;
} /* End of function  MGC_HubGetHubStatusCallback() */

/*******************************************************************************
 *              MGC_HubHandleHubOverCurrentTimer ()                            *
 *******************************************************************************/

void MGC_HubHandleHubOverCurrentTimer(MUSB_Hub *pHubDevice, uint32_t dwWaitingTime)
{
    uint32_t dwStatus;

    MUSB_ASSERT(pHubDevice);

    MUSB_DIAG_STRING(2, "HUB: MGC_HubHandleHubOverCurrentTimer()");

    dwStatus = MGC_HubStartTimer(pHubDevice->busHandle, pHubDevice->bIndexAtHubDeviceList,
                             dwWaitingTime, MGC_HubOverCurrentTimerCallback,
                             (void *)pHubDevice);

    UNUSED(dwStatus);
    return;
} /* End of function  MGC_HubHandleHubOverCurrentTimer() */

/*****************************************************************************
 *              MGC_HubPortDebounceTimerCallback ()                          *
 *****************************************************************************/
void MGC_HubOverCurrentTimerCallback(void *pParam, MUSB_BusHandle hBus)
{
    MUSB_Hub *pHubDevice;

    MUSB_ASSERT(pParam);

    pHubDevice = (MUSB_Hub *)pParam;

    UNUSED(hBus);
    MUSB_DIAG_STRING(2, "HUB: MGC_HubOverCurrentTimerCallback()");

    /* Now againt control should go to the HubStatusChangeCallBack  */
    MGC_HubGetHubStatusCallback(pParam, &(pHubDevice->ctrlIrp));
    return;
} /* End of function  MGC_HubOverCurrentTimerCallback()  */

/*******************************************************************************
 *              MGC_HubPowerOnGoodTimer ()                                    *
 *******************************************************************************/

void MGC_HubPowerOnGoodTimer(MUSB_Hub *pHubDevice, uint32_t dwWaitingTime)
{
    uint32_t dwStatus;

    MUSB_ASSERT(pHubDevice);

    MUSB_DIAG_STRING(2, "HUB: MGC_HubPowerOnGoodTimer()");
    dwStatus = MGC_HubStartTimer(pHubDevice->busHandle, pHubDevice->bIndexAtHubDeviceList,
                             dwWaitingTime, MGC_HubPowerOnGoodTimerCallback,
                             (void *)pHubDevice);
    UNUSED(dwStatus);
    return;
} /* End of function MGC_HubPowerOnGoodtTimer () */

/*****************************************************************************
 *              MGC_HubPowerOnGoodTimerCallback ()                           *
 *****************************************************************************/
void MGC_HubPowerOnGoodTimerCallback(void *pParam, MUSB_BusHandle hBus)
{
    MUSB_Hub *pHubDevice;

    MUSB_ASSERT(pParam);

    pHubDevice = (MUSB_Hub *)pParam;

    MUSB_DIAG_STRING(2, "HUB: MGC_HubPowerOnGoodTimerCallback()");
    pHubDevice->bActivePortNum = MUSB_HUB_STARTING_PORT_NUMBER;
    MGC_HubPortStatusChange(pHubDevice);

    UNUSED(hBus);
    return;
} /* End of function  MGC_HubPowerOnGoodTimerCallback() */

/*******************************************************************
 *                 MGC_HubHandlePowerOnPorts ()                    *
 *******************************************************************/

void MGC_HubHandlePowerOnPorts(MUSB_Hub *pHubDevice, uint8_t *pHubNextState)
{
    uint32_t dwStatus;

    uint8_t bPortNumber;

    MUSB_ASSERT(pHubDevice);
    MUSB_ASSERT(pHubNextState);

    bPortNumber = pHubDevice->bActivePortNum;

    if (MUSB_HUB_GANGED_POWER_SWITCHING == (pHubDevice->hubDscr.wHubCharacteristics
                                                & MUSB_HUB_LOGICAL_POWER_SWITCH_MODE_BM))
    {
        /* bActivePort make it to MUSB_HUB_INVALID_PORT_NUMBER because
         * request for one port is sufficient in case ganged power. 
         * so make port number for next request is Invalid.
         */
        *pHubNextState = MUSB_HUB_STATE_POWER_ON_COMPLETE;
        pHubDevice->bActivePortNum = MUSB_HUB_INVALID_PORT_NUMBER;
    }
    else
    { /* Next state will remain POWER_ON till request is not sent to last port. */
        *pHubNextState = MUSB_HUB_STATE_POWER_ON;
    }

    dwStatus = MUSB_HubSetPortFeature(pHubDevice, (uint16_t)MUSB_HUB_PORT_POWER_FEATURE,
                                      MGC_HubGetHubStatusCallback, bPortNumber);

    if ((uint32_t)MUSB_FAILURE == dwStatus)
    {
        MUSB_DIAG_STRING(1, "HubError: MGC_HubHandlePowerOnPorts() ");
        MUSB_DIAG1(2, "HubState: ", pHubDevice->bState, 16, 0x00);
    }

    return;
} /* End of function  MGC_HubHandlePowerOnPorts () */

/**********************Function related to PORT status start. ****************/

/*****************************************************************************
 *              MGC_HubPortStatusChange ()                                   *
 *****************************************************************************/
void MGC_HubPortStatusChange(MUSB_Hub *pHubDevice)
{
    uint8_t bNumOfPorts;
    uint8_t *pPortState;
    uint32_t dwStatus;

    MUSB_ASSERT(pHubDevice);       
    bNumOfPorts = pHubDevice->hubDscr.bNbrPorts;
    MUSB_ASSERT(bNumOfPorts <= MUSB_HUB_MAX_PORTS);

    /* This function expected that bActivePortNum should have  proper value
     * either initialized or increamented before calling the function.
     */
    while (pHubDevice->bActivePortNum <= bNumOfPorts)
    {
        if (pHubDevice->dwHubPortStatusChange&(1 << pHubDevice->bActivePortNum))
        {            
            if (((pHubDevice->bActivePortNum-1) >= (uint8_t)MUSB_HUB_MAX_PORTS ) ||
                (pHubDevice->bActivePortNum < 1))
            {
                LOG(0, "bActivePortNum=%d, overflow.\n", pHubDevice->bActivePortNum);
                return;
            }
            pPortState = &pHubDevice->aHubPortState[pHubDevice->bActivePortNum-1];

            dwStatus = MUSB_HubGetPortStatus(pHubDevice, MGC_HubGetPortStatusCallback,
                                             pHubDevice->bActivePortNum);

            *pPortState = (MUSB_SUCCESS == dwStatus) ? 
                MUSB_HUB_PORT_STATE_GOT_STATUS_P1 : MUSB_HUB_PORT_STATE_DEFAULT;

            /* Either error or success return from here because getting status for
             * this port is called. If it returns success then comeout from the loop
             * even error comes then no point of calling same function for other ports
             * so return from here.
             */
            return;
        }
        else
        {
            /* since ports status has not changed hence go to next port by increamenting
             * one.
             */
            pHubDevice->bActivePortNum++;
        }
    } /* End of while loop */

    return;
}     /* End of function MGC_HubPortStatusChange () */

/**************************************************************************
 *              MGC_HubGetPortStatusCallback ()                           *
 **************************************************************************/

void MGC_HubGetPortStatusCallback(void *pContext, MUSB_ControlIrp *pControlIrp)
{
    MUSB_Hub *pHubDevice;
    MUSB_HubPortStatus *pPortStatus;
    uint32_t dwStatus = MUSB_SUCCESS;
    uint8_t *pPortState;
       
    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pControlIrp);
    pHubDevice = (MUSB_Hub *)pContext;
    pPortStatus = &(pHubDevice->portStatus);

    MUSB_ASSERT((pHubDevice->bActivePortNum <= MUSB_HUB_MAX_PORTS) && 
        (pHubDevice->bActivePortNum >= 1));

    if (((pHubDevice->bActivePortNum-1) >= (uint8_t)MUSB_HUB_MAX_PORTS ) ||
        (pHubDevice->bActivePortNum < 1))
    {
        LOG(0, "bActivePortNum=%d, overflow.\n", pHubDevice->bActivePortNum);
        return;
    }
    
    pPortState = &pHubDevice->aHubPortState[pHubDevice->bActivePortNum-1];

    if (pControlIrp->dwStatus != MUSB_SUCCESS)
    {
        *pPortState = MUSB_HUB_PORT_STATE_DEFAULT;

        MUSB_DIAG_STRING(1, "HubError: pControlIrp->dwStatus in MGC_HubGetPortStatusCallback()");
        MUSB_DIAG1(1, "ControlIrpStatus: ", pControlIrp->dwStatus, 16, 0x00);
        return;
    }

    LOG(0, "HubPort-%d: bPortState = 0x%X.\n", pHubDevice->bActivePortNum, *pPortState);

    /* If bPortStatus is MUSB_HUB_PORT_STATE_GOT_STATUS then 
     * 1. Store the Port status in proper format. 
     * 2. Check Connect status has changed or not. If connect status
     *    changed then check device is connected or disconnected and
     *    take action acording to that.
     * 3. If connection status bit not changed then make port state
     *    to MUSB_HUB_PORT_STATE_C_CONNECT_COMPLETE so that next 
     *    bit of status change can be addressed.
     */
    if (MUSB_HUB_PORT_STATE_GOT_STATUS_P1 == *pPortState)
    {
        MGC_HubStoreStatus(pHubDevice->aHubPortStatus, pPortStatus);
        if (pPortStatus->wStatusChange & (uint16_t)MUSB_HUB_C_PORT_CONNECTION_BM)
        {
            LOG(0, "HubPort-%d: wStatusChange = 0x%X.\n", 
                pHubDevice->bActivePortNum, pPortStatus->wStatusChange);

            // clear C_PORT_CONNECTION_FEATURE.
            dwStatus = MUSB_HubClearPortFeature(pHubDevice,
                                                (uint16_t)MUSB_HUB_C_PORT_CONNECTION_FEATURE,
                                                MGC_HubGetPortStatusCallback,
                                                pHubDevice->bActivePortNum);            
            *pPortState = (MUSB_SUCCESS == dwStatus) ? 
                MUSB_HUB_PORT_STATE_CLEAR_CONNECT : MUSB_HUB_PORT_STATE_DEFAULT;
            return;
        }
        else
        {
            /* Note: Here Don't return  let control fall to next if statement. */
            *pPortState = MUSB_HUB_PORT_STATE_C_CONNECT_COMPLETE;
        }
    } /* End of if (MUSB_HUB_PORT_STATE_GOT_STATUS_P1)*/

    if (MUSB_HUB_PORT_STATE_CLEAR_CONNECT == *pPortState)
    {
        if (pPortStatus->wStatus & (uint16_t)MUSB_HUB_PORT_CONNECTION_BM)
        {
            LOG(0, "HubPort-%d: Port connect wStatus = 0x%X.\n", 
                pHubDevice->bActivePortNum, pPortStatus->wStatus);

            dwStatus = MUSB_HubGetPortStatus(pHubDevice, MGC_HubGetPortStatusCallback,
                                             pHubDevice->bActivePortNum);

            *pPortState = (MUSB_SUCCESS == dwStatus) ? 
                MUSB_HUB_PORT_STATE_GOT_STATUS : MUSB_HUB_PORT_STATE_DEFAULT;
            return;
        }
        else
        {
            LOG(0, "HubPort-%d: Not connect wStatus = 0x%X.\n", 
                pHubDevice->bActivePortNum, pPortStatus->wStatus);
                
            // clear PORT_ENABLE_FEATURE.
            dwStatus = MUSB_HubClearPortFeature(pHubDevice,
                                                (uint16_t)MUSB_HUB_PORT_ENABLE_FEATURE,
                                                MGC_HubGetPortStatusCallback,
                                                pHubDevice->bActivePortNum);
            *pPortState = (MUSB_SUCCESS == dwStatus) ? 
                MUSB_HUB_PORT_STATE_CLEAR_ENABLE : MUSB_HUB_PORT_STATE_DEFAULT;
            return;
        }
    }

    if (MUSB_HUB_PORT_STATE_CLEAR_ENABLE == *pPortState)
    {
        dwStatus = MUSB_HubGetPortStatus(pHubDevice, MGC_HubGetPortStatusCallback,
                                         pHubDevice->bActivePortNum);

        *pPortState = (MUSB_SUCCESS == dwStatus) ? 
            MUSB_HUB_PORT_STATE_GOT_STATUS : MUSB_HUB_PORT_STATE_DEFAULT;
        return;
    }
    
    if (MUSB_HUB_PORT_STATE_GOT_STATUS == *pPortState)
    {
        MGC_HubStoreStatus(pHubDevice->aHubPortStatus, pPortStatus);

        LOG(0, "HubPort-%d: Get port wStatus = 0x%X.\n", 
            pHubDevice->bActivePortNum, pPortStatus->wStatus);
            
        if (pPortStatus->wStatus & (uint16_t)MUSB_HUB_PORT_CONNECTION_BM)
        {
            /*
             * Device is connected to the port so handle that device. 
             */
            MUSB_DIAG_STRING(2, "HubPort: NewDeviceConnected");

            MGC_HubHandlePortConnect(pHubDevice, pPortState, pHubDevice->bActivePortNum);
            return;
        }
        else
        {
            /*
             * Device is Disconnected from the port so handle device disconnect.
             */
            /*
             * Update the power availabe and 
             * it always returns success in case of diconnection
             */
            MUSB_DIAG_STRING(2, "HubPort: DeviceDisconnected");

            MGC_HubHandlePortDisconnect(pHubDevice, pHubDevice->bActivePortNum);
            
            dwStatus = MGC_HubUpdatePower(pHubDevice, (uint8_t)MUSB_HUB_PORT_DISCONNECTED);
            UNUSED(dwStatus);
            *pPortState = MUSB_HUB_PORT_STATE_DEFAULT;
            return;
        }
    }
    
    if (MUSB_HUB_PORT_STATE_C_CONNECT_COMPLETE == *pPortState)
    {
        if (pPortStatus->wStatusChange & (uint16_t)MUSB_HUB_C_PORT_ENABLE_BM)
        {
            MUSB_DIAG_STRING(2, "HubPort: EnableStatusChanged");

            MGC_HubHandleEnablePort(pHubDevice, pPortState, pHubDevice->bActivePortNum);
            return;
        }
        else
        {
            *pPortState = MUSB_HUB_PORT_STATE_C_ENABLE_COMPLETE;
            MUSB_DIAG1(2, "PortState: ", *pPortState, 16, 0x00);
        }
    } /* End of if (MUSB_HUB_PORT_STATE_C_CONNECT_COMPLETE)*/

    if (MUSB_HUB_PORT_STATE_C_ENABLE_COMPLETE == *pPortState)
    {
        if (pPortStatus->wStatusChange & (uint16_t)MUSB_HUB_C_PORT_SUSPEND_BM)
        {
            MUSB_DIAG_STRING(2, "HubPort: SuspendStatusChanged");

            if (pPortStatus->wStatus & (uint16_t)MUSB_HUB_PORT_SUSPEND_BM)
            {
                /* Port Suspended, Update the power availabe and 
                 * it always returns success in case of resuming
                 */
                dwStatus = MGC_HubUpdatePower(pHubDevice, (uint8_t)MUSB_HUB_PORT_SUSPENDED);
            }
            else
            {
                /* Port resumed, Update the power availabe and 
                 * it always returns success in case of resuming
                 */
                dwStatus = MGC_HubUpdatePower(pHubDevice, (uint8_t)MUSB_HUB_PORT_RESUMED);
            }
            UNUSED(dwStatus);

            MGC_HubHandleSuspendPort(pHubDevice, pPortState, pHubDevice->bActivePortNum);
            return;
        }
        else
        {
            *pPortState = MUSB_HUB_PORT_STATE_C_SUSPEND_COMPLETE;
        }
    } /* End of if (MUSB_HUB_PORT_STATE_C_ENABLE_COMPLETE)*/

    if (MUSB_HUB_PORT_STATE_C_SUSPEND_COMPLETE == *pPortState)
    {
        if (pPortStatus->wStatusChange & (uint16_t)MUSB_HUB_C_PORT_OVER_CURRENT_BM)
        {
            MUSB_DIAG_STRING(2, "HubPort: OverCurrent status changed");

            if (pPortStatus->wStatus & (uint16_t)MUSB_HUB_PORT_OVER_CURRENT_BM)
            {
                MUSB_DIAG_STRING(2, "HubPort: OverCurrent exists");
                *pPortState = MUSB_HUB_PORT_STATE_C_OVERCURRENT;
            }
            else
            {
                MUSB_DIAG_STRING(2, "HubPort: OverCurrent doesn't exist");
                *pPortState = MUSB_HUB_PORT_STATE_C_OVERCURRENT_COMPLETE;
            }

            dwStatus = MUSB_HubClearPortFeature(pHubDevice,
                                                (uint16_t)MUSB_HUB_C_PORT_OVER_CURRENT_FEATURE,
                                                MGC_HubGetPortStatusCallback,
                                                pHubDevice->bActivePortNum);
            if (MUSB_SUCCESS != dwStatus)
            {
                *pPortState = MUSB_HUB_PORT_STATE_DEFAULT;
            }
            return;
        }
        else
        {
            /* Since over current condition is not there hence make state as complete
             * and go ahead to check next condition.
             */
            *pPortState = MUSB_HUB_PORT_STATE_C_OVERCURRENT_COMPLETE;
        }
    } /* End of if (MUSB_HUB_PORT_STATE_C_SUSPEND_COMPLETE)*/

    if (MUSB_HUB_PORT_STATE_C_OVERCURRENT == *pPortState)
    {
        /* Doing POWER ON for the state. */
        dwStatus = MUSB_HubSetPortFeature(pHubDevice, (uint16_t)MUSB_HUB_PORT_POWER_FEATURE,
                                          MGC_HubGetPortStatusCallback, pHubDevice->bActivePortNum);

        *pPortState = (MUSB_SUCCESS == dwStatus) ? 
            MUSB_HUB_PORT_STATE_C_OVERCURRENT_CONTINUE : MUSB_HUB_PORT_STATE_DEFAULT;
                                          
        return;
    } /* End of if (MUSB_HUB_PORT_STATE_C_OVERCURRENT)*/

    if (MUSB_HUB_PORT_STATE_C_OVERCURRENT_CONTINUE == *pPortState)
    {
        /* */
        MGC_HubOverCurrentPortTimer(pHubDevice,
                                    (uint32_t)((pHubDevice->hubDscr.bPwrOn2PwrGood) *2));
        return;
    } /* End of if (MUSB_HUB_PORT_STATE_C_OVERCURRENT)*/

    if (MUSB_HUB_PORT_STATE_C_OVERCURRENT_COMPLETE == *pPortState)
    {
        if (pPortStatus->wStatusChange & (uint16_t)MUSB_HUB_C_PORT_RESET_BM)
        {
            MUSB_DIAG_STRING(2, "HubPort: Reset state changed");

            dwStatus = MUSB_HubClearPortFeature(pHubDevice,
                                                (uint16_t)MUSB_HUB_C_PORT_RESET_FEATURE,
                                                MGC_HubGetPortStatusCallback,
                                                pHubDevice->bActivePortNum);

            *pPortState = (MUSB_SUCCESS == dwStatus) ? 
                MUSB_HUB_PORT_STATE_C_RESET_COMPLETE : MUSB_HUB_PORT_STATE_DEFAULT;                                                
            return;
        }
        else
        {
            *pPortState = MUSB_HUB_PORT_STATE_C_RESET_COMPLETE;
        }
    } /* End of if (MUSB_HUB_PORT_STATE_C_OVERCURRENT_COMPLETE) */

    if (MUSB_HUB_PORT_STATE_C_RESET_COMPLETE == *pPortState)
    {
        /* All the status changes are taken care and now time to go for
         * next port.
         */
       *pPortState = MUSB_HUB_PORT_STATE_DEFAULT;

        return;
    } /* End of if (MUSB_HUB_PORT_STATE_C_RESET_COMPLETE) */
    return;
} /* End of function  MGC_HubGetPortStatusCallback ()  */

/*******************************************************************
 *                 MGC_HubHandlePortConnect ()                     *
 *******************************************************************/
void MGC_HubHandlePortConnect(MUSB_Hub *pHubDevice, uint8_t *pPortState, uint8_t bPortNum)
{
    uint32_t dwStatus;

    MUSB_ASSERT(pHubDevice);
    MUSB_ASSERT(pPortState);

    MUSB_DIAG_STRING(1, "MGC_HubHandlePortConnect()");

    if (pHubDevice->bAllocatedPower >= MUSB_HUB_BUS_POWER_MAX_UNITS)
    {
        /* Power is not available */
        MUSB_DIAG_STRING(1, "HubError: Cannot supply load to device");

        MUSB_DIAG_STRING(1, "HubError: Please disconnect device");
        *pPortState = MUSB_HUB_PORT_STATE_C_CONNECT_COMPLETE;
    }
    else
    {
        *pPortState = MUSB_HUB_PORT_STATE_WAIT_FOR_DEBOUNCE;
    }

    dwStatus = MUSB_HubGetPortStatus(pHubDevice, MGC_HubPortDebounceCallback,
                                     pHubDevice->bActivePortNum);
    if (dwStatus != MUSB_SUCCESS)
    {
        *pPortState = MUSB_HUB_PORT_STATE_DEFAULT;
        MUSB_DIAG_STRING(1, "HubError: MGC_HubHandlePortConnect()");
        MUSB_DIAG1(2, "PortState: ", pHubDevice->aHubPortState[pHubDevice->bActivePortNum-1], 16, 0x00);
    }

    UNUSED(bPortNum);
    return;
} /* End of function MGC_HubHandleConnectPort () */

/*******************************************************************************
 *              MGC_HubPortDebounceCallback ()                                 *
 *******************************************************************************/

void MGC_HubPortDebounceCallback(void *pContext, MUSB_ControlIrp *pControlIrp)
{
    uint32_t dwStatus = 0;
    MUSB_Hub *pHubDevice;
    MUSB_HubPortStatus *pDebouncePortStatus;
    uint8_t *pPortState;
    
    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pControlIrp);

    pHubDevice = (MUSB_Hub *)pContext;
    pDebouncePortStatus = &(pHubDevice->portStatus);

    MUSB_ASSERT((pHubDevice->bActivePortNum <= MUSB_HUB_MAX_PORTS) && 
        (pHubDevice->bActivePortNum >= 1));

    if (((pHubDevice->bActivePortNum-1) >= (uint8_t)MUSB_HUB_MAX_PORTS ) ||
        (pHubDevice->bActivePortNum < 1))
    {
        LOG(0, "bActivePortNum=%d, overflow.\n", pHubDevice->bActivePortNum);
        return;
    }
    
    pPortState = &pHubDevice->aHubPortState[pHubDevice->bActivePortNum-1];
    
    if (pControlIrp->dwStatus != MUSB_SUCCESS)
    {
        *pPortState = MUSB_HUB_PORT_STATE_DEFAULT;
        MUSB_DIAG_STRING(1, "HubError: pControlIrp->dwStatus in MGC_HubPortDebounceCallback()");
        MUSB_DIAG1(1, "ControlIrpStatus: ", pControlIrp->dwStatus, 16, 0x00);
        return;
    }

    /* Since IRP is executed properly hence update port state to its next port state.
     */
    if (MUSB_HUB_PORT_STATE_WAIT_FOR_DEBOUNCE == *pPortState)
    {
        *pPortState = MUSB_HUB_PORT_STATE_DEBOUNCE_GOT_STATUS;
        pHubDevice->dwWaitingTime = (uint32_t)MUSB_HUB_MIN_DEBOUNCE_TIME;
        MGC_HubHandlePortDebounceTimer(pHubDevice, pHubDevice->dwWaitingTime);
        return;
    }

    if (MUSB_HUB_PORT_STATE_DEBOUNCE_GOT_STATUS == *pPortState)
    {
        /* Store Status into proper format. */
        MGC_HubStoreStatus(pHubDevice->aHubPortStatus, pDebouncePortStatus);
        
        if ((pDebouncePortStatus->wStatus & (uint16_t)MUSB_HUB_PORT_CONNECTION_BM) &&
            ((pDebouncePortStatus->wStatusChange & (uint16_t)MUSB_HUB_C_PORT_CONNECTION_BM) == 0))
        {
            if (pHubDevice->dwWaitingTime < (uint32_t)MUSB_HUB_MAX_DEBOUNCE_TIME)
            {
                /* Connection is still there. Then wait for some more time
                 */
                *pPortState = MUSB_HUB_PORT_STATE_DEBOUNCE_GOT_STATUS;
                MGC_HubHandlePortDebounceTimer(pHubDevice, (uint32_t)MUSB_HUB_INCREMENT_DEBOUNCE_TIME);
                /* Since timer has expired then update the waiting time. */
                pHubDevice->dwWaitingTime += (uint32_t)MUSB_HUB_INCREMENT_DEBOUNCE_TIME;
                return;
            }
            else
            {
                /* Debounce waiting time is over and still connection is there
                 * hence go ahead and start RESET by calling reset callback. 
                 */
                MUSB_DIAG_STRING(3, "HubSuccess: Debounce complete");

                //  MTK Notice: Max Liao, 2007/11/21.
                //  TEAC hub will report error status change and cause driver assert.
                //  PC set this device to disconnect and reconfiguration it again.
                MUSB_ASSERT((pHubDevice->bActivePortNum <= MUSB_HUB_MAX_PORTS) && 
                    (pHubDevice->bActivePortNum >= 1));
                if (pHubDevice->aHubChildPortMap[pHubDevice->bActivePortNum-1] == 
                    MUSB_HUB_CHILD_PRESENT)
                {
                    LOG(0, "HubError: HubPort-%d: Already connected.\n", pHubDevice->bActivePortNum);
                    /*
                     * Device is Disconnected from the port so handle device disconnect.
                     */
                    /*
                     * Update the power availabe and 
                     * it always returns success in case of diconnection
                     */
                    MGC_HubHandlePortDisconnect(pHubDevice, pHubDevice->bActivePortNum);
                    
                    dwStatus = MGC_HubUpdatePower(pHubDevice, (uint8_t)MUSB_HUB_PORT_DISCONNECTED);
                    UNUSED(dwStatus);
                }

                pHubDevice->bResetNumOfTry = 0;
                pHubDevice->ctrlIrp.dwStatus = 0x00;
                pHubDevice->ctrlIrp.pfIrpComplete = MGC_HubPortResetCallback;
                MUSB_HubPrepareSetupPkt(&(pHubDevice->setupPacket), (uint8_t)(
                                                                        MUSB_TYPE_CLASS
                                                                            | MUSB_RECIP_OTHER),
                                        MUSB_REQ_SET_FEATURE, (uint16_t)MUSB_HUB_PORT_RESET_FEATURE,
                                        (uint16_t)pHubDevice->bActivePortNum, 0);
                pHubDevice->ctrlIrp.pInBuffer = NULL;
                pHubDevice->ctrlIrp.dwInLength = MUSB_HUB_NO_DATA_PHASE;
                dwStatus = MUSB_StartControlTransfer(pHubDevice->pUsbDevice->pPort,
                                                     &(pHubDevice->ctrlIrp));
                *pPortState = (MUSB_SUCCESS == dwStatus) ? 
                    MUSB_HUB_PORT_STATE_START_RESET : MUSB_HUB_PORT_STATE_DEFAULT;
                return;
            }
        }
        else
        {
            MUSB_DIAG_STRING(1, "HubError: During debounce device disconnected");

            /*
             * Device is Disconnected from the port so handle device disconnect.
             */
            /*
             * Update the power availabe and 
             * it always returns success in case of diconnection
             */
            MUSB_DIAG_STRING(1, "HubPort: DeviceDisconnected");

            MGC_HubHandlePortDisconnect(pHubDevice, pHubDevice->bActivePortNum);
            
            dwStatus = MGC_HubUpdatePower(pHubDevice, (uint8_t)MUSB_HUB_PORT_DISCONNECTED);
            *pPortState = MUSB_HUB_PORT_STATE_DEFAULT;            
        }
    } /* End of if (MUSB_HUB_PORT_STATE_DEBOUNCE_GOT_STATUS) */

    return;
} /* End of function  */

/*******************************************************************************
 *              MGC_HubHandlePortDebounceTimer ()                               *
 *******************************************************************************/
void MGC_HubHandlePortDebounceTimer(MUSB_Hub *pHubDevice, uint32_t dwWaitingTime)
{
    uint32_t dwStatus = 0;
    uint8_t *pPortState;

    MUSB_ASSERT(pHubDevice);
    MUSB_ASSERT((pHubDevice->bActivePortNum <= MUSB_HUB_MAX_PORTS) && 
        (pHubDevice->bActivePortNum >= 1));

    if (((pHubDevice->bActivePortNum-1) >= (uint8_t)MUSB_HUB_MAX_PORTS ) ||
        (pHubDevice->bActivePortNum < 1))
    {
        LOG(0, "bActivePortNum=%d, overflow.\n", pHubDevice->bActivePortNum);
        return;
    }

    pPortState = &pHubDevice->aHubPortState[pHubDevice->bActivePortNum-1];
    
    dwStatus = MGC_HubStartTimer(pHubDevice->busHandle, pHubDevice->bIndexAtHubDeviceList,
                             dwWaitingTime, MGC_HubPortDebounceTimerCallback,
                             (void *)pHubDevice);
    if (MUSB_SUCCESS != dwStatus)
    {
        *pPortState = MUSB_HUB_PORT_STATE_DEFAULT;
    }
    return;
} /* End of function MGC_HubHandlePortDebounceTimer ()   */

/*****************************************************************************
 *              MGC_HubPortDebounceTimerCallback ()                          *
 *****************************************************************************/

void MGC_HubPortDebounceTimerCallback(void *pParam, MUSB_BusHandle hBus)
{
    MUSB_Hub *pHubDevice;
    uint8_t *pPortState;
    uint32_t dwStatus;

    MUSB_ASSERT(pParam);

    pHubDevice = (MUSB_Hub *)pParam;
    MUSB_ASSERT((pHubDevice->bActivePortNum <= MUSB_HUB_MAX_PORTS) && 
        (pHubDevice->bActivePortNum >= 1));

    if (((pHubDevice->bActivePortNum-1) >= (uint8_t)MUSB_HUB_MAX_PORTS ) ||
        (pHubDevice->bActivePortNum < 1))
    {
        LOG(0, "bActivePortNum=%d, overflow.\n", pHubDevice->bActivePortNum);
        return;
    }

    pPortState = &pHubDevice->aHubPortState[pHubDevice->bActivePortNum-1];

    MUSB_DIAG_STRING(2, "Hub:WithIn MGC_HubPortDebounceTimerCallback()");

    dwStatus = MUSB_HubGetPortStatus(pHubDevice, MGC_HubPortDebounceCallback,
                                     pHubDevice->bActivePortNum);
    if (MUSB_SUCCESS != dwStatus)
    {
        *pPortState = MUSB_HUB_PORT_STATE_DEFAULT;
    }

    UNUSED(hBus);   
    return;
} /* End of function MGC_HubPortDebounceTimerCallback ()   */

/*******************************************************************************
 *              MGC_HubPortResetCallback()                                      *
 *******************************************************************************/

void MGC_HubPortResetCallback(void *pContext, MUSB_ControlIrp *pControlIrp)
{
    MUSB_Hub *pHubDevice;
    MUSB_HubPortStatus *pResetPortStatus;
    uint32_t dwStatus = 0;
    uint8_t *pPortState;
    
    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pControlIrp);

    pHubDevice = (MUSB_Hub *)pContext;
    pResetPortStatus = &(pHubDevice->portStatus);

    MUSB_ASSERT((pHubDevice->bActivePortNum <= MUSB_HUB_MAX_PORTS) && 
        (pHubDevice->bActivePortNum >= 1));

    if (((pHubDevice->bActivePortNum-1) >= (uint8_t)MUSB_HUB_MAX_PORTS ) ||
        (pHubDevice->bActivePortNum < 1))
    {
        LOG(0, "bActivePortNum=%d, overflow.\n", pHubDevice->bActivePortNum);
        return;
    }
    
    pPortState = &pHubDevice->aHubPortState[pHubDevice->bActivePortNum-1];

    if (pControlIrp->dwStatus != MUSB_SUCCESS)
    {
        *pPortState = MUSB_HUB_PORT_STATE_DEFAULT;
        MUSB_DIAG_STRING(1, "HubError: pControlIrp->dwStatus in MGC_HubPortResetCallback()");
        MUSB_DIAG1(1, "ControlIrpStatus:", pControlIrp->dwStatus, 16, 0x00);
        return;
    }

    MUSB_DIAG_STRING(2, "Hub: MGC_HubPortResetCallback()");

    if (MUSB_HUB_PORT_STATE_START_RESET == *pPortState)
    {
        *pPortState = MUSB_HUB_PORT_STATE_RESET_GOT_STATUS;
        pHubDevice->dwWaitingTime = (uint32_t)MUSB_HUB_MIN_RESET_TIME; 
        MGC_HubHandlePortResetTimer(pHubDevice, pHubDevice->dwWaitingTime);
        return;
    }

    if (MUSB_HUB_PORT_STATE_RESET_GOT_STATUS == *pPortState)
    {
        MGC_HubStoreStatus(pHubDevice->aHubPortStatus, pResetPortStatus);

        // check if device disconnect or connect status change.
        if ((pResetPortStatus->wStatus & (uint16_t)MUSB_HUB_PORT_CONNECTION_BM) == 0)
        {
            /*  Make State proper and comeout from loop. No need to do reset
             * Prnt message that can not work with device. It is disconnected.
             */
            /* Go to main loop with connection complete and display error
              * message that can not newly connected device is not responding.
              */
            /* just return; With error messagge and put state proper. Call callback
             * to handle other port status change.
             */
            MUSB_DIAG_STRING(1, "HubError: During Reset DeviceDisconnected");

            *pPortState = MUSB_HUB_PORT_STATE_C_CONNECT_COMPLETE;
            MGC_HubGetPortStatusCallback((void *)pHubDevice, &(pHubDevice->ctrlIrp));
            return;
        }

        if ((pResetPortStatus->wStatus & (uint16_t)MUSB_HUB_PORT_CONNECTION_BM) &&
        (pResetPortStatus->wStatus & (uint16_t)MUSB_HUB_PORT_ENABLE_BM))
        {
            /* RESET is successful and so get information about speed and return
             * as RESET SUCCESSFUL.
             */
            MUSB_DIAG_STRING(2, "Hub: RESET COMPLETED");

            if (pResetPortStatus->wStatus & (uint16_t)MUSB_HUB_PORT_LOW_SPEED_BM)
            {
                LOG(0, "HubPort-%d: Low speed.\n", pHubDevice->bActivePortNum);
                pHubDevice->bSpeed = MUSB_DEVICE_SPEED_LOW;
            }
            else if (pResetPortStatus->wStatus & (uint16_t)MUSB_HUB_PORT_HIGH_SPEED_BM)
            {
                LOG(0, "HubPort-%d: High speed.\n", pHubDevice->bActivePortNum);
                pHubDevice->bSpeed = MUSB_DEVICE_SPEED_HIGH;
            }
            else
            {
                LOG(0, "HubPort-%d: Full speed.\n", pHubDevice->bActivePortNum);
                pHubDevice->bSpeed = MUSB_DEVICE_SPEED_FULL;
            }

            /* Reset signalling is complete */
            dwStatus = MUSB_HubClearPortFeature(pHubDevice, (uint16_t)MUSB_HUB_C_PORT_RESET_FEATURE,
                                     MGC_HubPortConnectCallback, pHubDevice->bActivePortNum);
            *pPortState = (MUSB_SUCCESS == dwStatus) ? 
                MUSB_HUB_PORT_STATE_RESET_COMPLETE : MUSB_HUB_PORT_STATE_DEFAULT;
            return;
        }

        /* Check total reset time as well as number of tries.
         * If both are over then rise error message and comeout
         * else go for one more try.
         */
        if (pHubDevice->bResetNumOfTry < (uint8_t)MUSB_HUB_MAX_RESET_TRIES)
        {
            pHubDevice->bResetNumOfTry++;
            pHubDevice->dwWaitingTime = (uint32_t)MUSB_HUB_MIN_RESET_TIME;
            
            /* Till bResetNumOfTry < MAX TRY then go ahead and call timer. */
            *pPortState = MUSB_HUB_PORT_STATE_RESET_GOT_STATUS;
            MGC_HubHandlePortResetTimer(pHubDevice, pHubDevice->dwWaitingTime);
            return;
        }
        else
        {
            /* Hub Has tried to RESET the device but couldn't Succeed.
             * So go for forget about device and go and address other
             * port status change.
             */
            /* Return with pPortState and pPortState to 
             * "MUSB_HUB_PORT_STATE_C_CONNECT_COMPLETE" so 
             * other status of same port can be address.
             */
            MUSB_DIAG_STRING(2, "Hub: Unable to complete reset");

            *pPortState = MUSB_HUB_PORT_STATE_C_CONNECT_COMPLETE;
            pHubDevice->ctrlIrp.dwStatus = 0x00;
            MGC_HubGetPortStatusCallback((void *)pHubDevice, &(pHubDevice->ctrlIrp));
            return;
        }
    //        return;
    }
} /* End of function  MGC_HubPortResetCallback () */

/*******************************************************************************
 *              MGC_HubHandlePortResetTimer ()                                 *
 *******************************************************************************/
void MGC_HubHandlePortResetTimer(MUSB_Hub *pHubDevice, uint32_t dwWaitingTime)
{
    uint32_t dwStatus;
    uint8_t *pPortState;

    MUSB_ASSERT(pHubDevice);
    MUSB_ASSERT((pHubDevice->bActivePortNum <= MUSB_HUB_MAX_PORTS) && 
        (pHubDevice->bActivePortNum >= 1));

    if (((pHubDevice->bActivePortNum-1) >= (uint8_t)MUSB_HUB_MAX_PORTS ) ||
        (pHubDevice->bActivePortNum < 1))
    {
        LOG(0, "bActivePortNum=%d, overflow.\n", pHubDevice->bActivePortNum);
        return;
    }

    pPortState = &pHubDevice->aHubPortState[pHubDevice->bActivePortNum-1];

    dwStatus = MGC_HubStartTimer(pHubDevice->busHandle, pHubDevice->bIndexAtHubDeviceList,
                             dwWaitingTime, MGC_HubPortResetTimerCallback,
                             (void *)pHubDevice);

    if (MUSB_SUCCESS != dwStatus)
    {
        *pPortState = MUSB_HUB_PORT_STATE_DEFAULT;
    }
    
    return;
} /* End of function MGC_HubHandlePortResetTimer () */

/*******************************************************************************
 *              MGC_HubPortResetTimerCallback ()                               *
 *******************************************************************************/

void MGC_HubPortResetTimerCallback(void *pParam, MUSB_BusHandle hBus)
{
    MUSB_Hub *pHubDevice;
    uint8_t *pPortState;
    uint32_t dwStatus;

    MUSB_ASSERT(pParam);

    pHubDevice = (MUSB_Hub *)pParam;
    MUSB_ASSERT((pHubDevice->bActivePortNum <= MUSB_HUB_MAX_PORTS) && 
        (pHubDevice->bActivePortNum >= 1));

    if (((pHubDevice->bActivePortNum-1) >= (uint8_t)MUSB_HUB_MAX_PORTS ) ||
        (pHubDevice->bActivePortNum < 1))
    {
        LOG(0, "bActivePortNum=%d, overflow.\n", pHubDevice->bActivePortNum);
        return;
    }
    
    pPortState = &pHubDevice->aHubPortState[pHubDevice->bActivePortNum-1];

    MUSB_DIAG_STRING(2, "Hub: MGC_HubPortResetTimerCallback()");

    dwStatus = MUSB_HubGetPortStatus(pHubDevice, MGC_HubPortResetCallback,
                                     pHubDevice->bActivePortNum);

    *pPortState = (MUSB_SUCCESS == dwStatus) ? 
        MUSB_HUB_PORT_STATE_RESET_GOT_STATUS : MUSB_HUB_PORT_STATE_DEFAULT;

    UNUSED(hBus);
    return;
} /* End of function MGC_HubPortResetTimerCallback () */

/*******************************************************************************
 *              MGC_HubPortConnectCallback()                                   *
 *******************************************************************************/

void MGC_HubPortConnectCallback(void *pContext, MUSB_ControlIrp *pControlIrp)
{
    MUSB_Hub *pHubDevice;
    MUSB_HubPortStatus *pPortStatus;
    uint8_t *pPortState;
    uint32_t dwStatus = 0;

    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pControlIrp);

    pHubDevice = (MUSB_Hub *)pContext;
    pPortStatus = &(pHubDevice->portStatus);
    MUSB_ASSERT((pHubDevice->bActivePortNum <= MUSB_HUB_MAX_PORTS) && 
        (pHubDevice->bActivePortNum >= 1));

    if (((pHubDevice->bActivePortNum-1) >= (uint8_t)MUSB_HUB_MAX_PORTS ) ||
        (pHubDevice->bActivePortNum < 1))
    {
        LOG(0, "bActivePortNum=%d, overflow.\n", pHubDevice->bActivePortNum);
        return;
    }
    
    pPortState = &pHubDevice->aHubPortState[pHubDevice->bActivePortNum-1];

    if (pControlIrp->dwStatus != MUSB_SUCCESS)
    {
        *pPortState = MUSB_HUB_PORT_STATE_DEFAULT;
        MUSB_DIAG_STRING(1, "HubError: pControlIrp->dwStatus in MGC_HubPortConnectCallback()");
        MUSB_DIAG1(1, "ControlIrpStatus: ", pControlIrp->dwStatus, 16, 0x00);
        return;
    }

    //  MTK Notice: Max Liao, 2006/11/30.
    //  Hub get device status after reset complete.
    if (MUSB_HUB_PORT_STATE_RESET_COMPLETE == *pPortState)
    {
        dwStatus = MUSB_HubGetPortStatus(pHubDevice, MGC_HubPortConnectCallback,
                                         pHubDevice->bActivePortNum);
        *pPortState = (MUSB_SUCCESS == dwStatus) ? 
            MUSB_HUB_PORT_STATE_RESET_COMPLETE_GOT_STATUS : MUSB_HUB_PORT_STATE_DEFAULT;
        return;
    }

    if (MUSB_HUB_PORT_STATE_RESET_COMPLETE_GOT_STATUS ==  *pPortState)
    {
        MGC_HubStoreStatus(pHubDevice->aHubPortStatus, pPortStatus);

        LOG(0, "HubPort-%d: wStatus = 0x%X, wStatusChange = 0x%X.\n", 
            pHubDevice->bActivePortNum, pPortStatus->wStatus, pPortStatus->wStatusChange);

        if (((pPortStatus->wStatus & (uint16_t)MUSB_HUB_PORT_CONNECTION_BM) == 0) ||
              (pPortStatus->wStatusChange & (uint16_t)MUSB_HUB_C_PORT_CONNECTION_BM))
        {
            /*  Make State proper and comeout from loop. No need to do reset
             * Prnt message that can not work with device. It is disconnected.
             */
            /* Go to main loop with connection complete and display error
              * message that can not newly connected device is not responding.
              */
            /* just return; With error messagge and put state proper. Call callback
             * to handle other port status change.
             */
            MUSB_DIAG_STRING(1, "HubError: During Clear Reset DeviceDisconnected");

             *pPortState = MUSB_HUB_PORT_STATE_C_CONNECT_COMPLETE;
            MGC_HubGetPortStatusCallback((void *)pHubDevice, &(pHubDevice->ctrlIrp));
            return;
        }

        if (pHubDevice->bResetNumOfTry < (uint8_t)MUSB_HUB_MAX_RESET_TRIES)
        {
            pHubDevice->bResetNumOfTry ++;
            pHubDevice->ctrlIrp.dwStatus = 0x00;
            pHubDevice->ctrlIrp.pfIrpComplete = MGC_HubPortResetCallback;
            MUSB_HubPrepareSetupPkt(&(pHubDevice->setupPacket), (uint8_t)(
                                                                    MUSB_TYPE_CLASS
                                                                        | MUSB_RECIP_OTHER),
                                    MUSB_REQ_SET_FEATURE, (uint16_t)MUSB_HUB_PORT_RESET_FEATURE,
                                    (uint16_t)pHubDevice->bActivePortNum, 0);
            pHubDevice->ctrlIrp.pInBuffer = NULL;
            pHubDevice->ctrlIrp.dwInLength = MUSB_HUB_NO_DATA_PHASE;
            dwStatus = MUSB_StartControlTransfer(pHubDevice->pUsbDevice->pPort,
                                                 &(pHubDevice->ctrlIrp));
            *pPortState = (MUSB_SUCCESS == dwStatus) ? 
                MUSB_HUB_PORT_STATE_START_RESET : MUSB_HUB_PORT_STATE_DEFAULT;
            return;
        }

        /*  1. First find out relation between bPortNum and bChildIndex if
         *     any device is present at that port. If any device is present at 
         *     given port then first deenumerate the device then enumerate
         *     newly connected device.
         *  2. If at given port no device is present then find first free 
         *     childIndex at which new device can register. 
         */
        MUSB_ASSERT(pHubDevice->aHubChildPortMap[pHubDevice->bActivePortNum-1] == 
            MUSB_HUB_CHILD_FREE);
        if ((pHubDevice->aHubChildPortMap[pHubDevice->bActivePortNum-1] == MUSB_HUB_CHILD_FREE) && 
            (pHubDevice->bDepthInHubChain <= MUSB_MAX_HUB_CHAIN))
        {
            // dwHubPortStatusChange must be not change before enumerate device.
            pHubDevice->dwHubPortStatusChange &= ~(uint32_t)(1 << pHubDevice->bActivePortNum);

            // set child port status before enumerate.
            pHubDevice->aHubChildPortMap[pHubDevice->bActivePortNum-1] = MUSB_HUB_CHILD_PRESENT;
            
            dwStatus = MUSB_EnumerateDevice(pHubDevice->pUsbDevice, pHubDevice->bActivePortNum,
                                 pHubDevice->bSpeed, MUSB_HubDeviceEnumerateCallback);
            *pPortState = (MUSB_SUCCESS == dwStatus) ? 
                MUSB_HUB_PORT_WAIT_FOR_ENUMERATE : MUSB_HUB_PORT_STATE_DEFAULT;                                 
        }
        else
        {
            /* There is not free port is available so either increase the
             * limit of MUSB_HUB_MAX_CHILD or remove unwanted device from 
             * hub's port. 
             */
            MUSB_DIAG2(1, "HubError: Failure Return By MGC_HubGetChildIndexForGivenPort(",
                       (uint32_t)pHubDevice, ", ",
                       pHubDevice->bActivePortNum, 16,
                       0);

            MUSB_DIAG1(2, "bDepthInHubChain: ", pHubDevice->bDepthInHubChain, 16, 0x00);

            /* Connected device cannot enumerated so go and handle other status
             * changes of the port. 
             */
            *pPortState = MUSB_HUB_PORT_STATE_C_CONNECT_COMPLETE;
            pHubDevice->ctrlIrp.dwStatus = 0x00;
            MGC_HubGetPortStatusCallback((void *)pHubDevice, &(pHubDevice->ctrlIrp));
        }
    }

    UNUSED(dwStatus);
    return;
} /* End of function  MGC_HubPortConnectCallback () */

/*******************************************************************************
 *              MUSB_HubDeviceEnumerateCallback()                                   *
 *******************************************************************************/

void MUSB_HubDeviceEnumerateCallback(MUSB_Device *pHubUsbDevice, 
    MUSB_Device *pChildUsbDevice, uint8_t bStatus)
{
    uint32_t dwStatus;
    MUSB_Hub *pHubDevice;
    uint8_t *pPortState;

    MUSB_ASSERT(pHubUsbDevice);
    MUSB_ASSERT(pChildUsbDevice);

    dwStatus = MGC_HubFindHubDevice(pHubUsbDevice, &pHubDevice);
    if (MUSB_SUCCESS != dwStatus)
    {
        MUSB_DIAG_STRING(1, "HubError: This Is Not A Hub Device/NULL");
        return;
    }

    MUSB_ASSERT((pHubDevice->bActivePortNum <= MUSB_HUB_MAX_PORTS) && 
        (pHubDevice->bActivePortNum >= 1));

    if (((pHubDevice->bActivePortNum-1) >= (uint8_t)MUSB_HUB_MAX_PORTS ) ||
        (pHubDevice->bActivePortNum < 1))
    {
        LOG(0, "bActivePortNum=%d, overflow.\n", pHubDevice->bActivePortNum);
        return;
    }
    
    pPortState = &pHubDevice->aHubPortState[pHubDevice->bActivePortNum-1];

    pChildUsbDevice->pParentUsbDevice = pHubDevice->pUsbDevice;
    pHubDevice->pChild[pHubDevice->bActivePortNum - 1] = pChildUsbDevice;

    // handle enumeration fail case.
    if (!bStatus)
    {
        LOG(0, "HubPort-%d: During Enumerate DeviceDisconnected.\n", pHubDevice->bActivePortNum);    
        dwStatus = MUSB_HubGetPortStatus(pHubDevice, MGC_HubGetPortStatusCallback, 
            pHubDevice->bActivePortNum);

        *pPortState = (MUSB_SUCCESS == dwStatus) ? 
            MUSB_HUB_PORT_STATE_GOT_STATUS_P1 : MUSB_HUB_PORT_STATE_DEFAULT;
        return;
    }

    // check if port status change during device enumeration.
    if (pHubDevice->dwHubPortStatusChange & (1 << pHubDevice->bActivePortNum))
    {
        MUSB_DIAG_STRING(1, "HubError: During Enumerate DeviceDisconnected");
        // wait device to be handled in MGC_HubHandlePortDisconnect().       
        *pPortState = MUSB_HUB_PORT_STATE_C_CONNECT_COMPLETE;
        MGC_HubGetPortStatusCallback((void *)pHubDevice, &(pHubDevice->ctrlIrp));
        return;
    }
    
    if (MUSB_CLASS_HUB == pChildUsbDevice->DeviceDescriptor.bDeviceClass)
    {
        /*
         * Bus Powered bus cannot drive bus powered hub,
         * Newly connected hus can draw 1 unit of load atmost.
         * So, only the hub can servive(It can't supply load to the its ports)
         * The newly connected hub doesn't have power to drive the ports,
         * so, during connect function of this hub, make bAllocatedPower to
         * MUSB_HUB_BUS_POWER_MAX_UNITS
         */
        if (((uint8_t)MUSB_HUB_BUS_POWERED == pHubDevice->bSelfPower)
                && (!(pChildUsbDevice->apConfigDescriptors[0]->bmAttributes
                          & MUSB_HUB_CONFIG_SELF_POWER_BM)))
        {
            MUSB_DIAG_STRING(1,
                             "HubError: bus-powered hub cannot drive another bus-powered hub");
            MUSB_DIAG_STRING(1, "Please disconnect newly connected device");
        }
    }

    dwStatus = MGC_HubUpdatePower(pHubDevice, (uint8_t)MUSB_HUB_PORT_CONNECTED);
    VERIFY(MUSB_SUCCESS == dwStatus);

    /* Everything is handled related to connect. Go and check for other
    * status of the port.
    */
    *pPortState = MUSB_HUB_PORT_STATE_C_CONNECT_COMPLETE;
    pHubDevice->ctrlIrp.dwStatus = 0x00;
    MGC_HubGetPortStatusCallback((void *)pHubDevice, &(pHubDevice->ctrlIrp));

    return;
}

/*******************************************************************
 *                 MGC_HubOverCurrentPortTimerCallback ()          *
 *******************************************************************/
void MGC_HubOverCurrentPortTimerCallback(void *pParam, MUSB_BusHandle hBus)
{
    MUSB_Hub *pHubDevice;
    uint8_t *pPortState;

    MUSB_ASSERT(pParam);

    pHubDevice = (MUSB_Hub *)pParam;
    MUSB_ASSERT((pHubDevice->bActivePortNum <= MUSB_HUB_MAX_PORTS) && 
        (pHubDevice->bActivePortNum >= 1));

    if (((pHubDevice->bActivePortNum-1) >= (uint8_t)MUSB_HUB_MAX_PORTS ) ||
        (pHubDevice->bActivePortNum < 1))
    {
        LOG(0, "bActivePortNum=%d, overflow.\n", pHubDevice->bActivePortNum);
        return;
    }
    
    pPortState = &pHubDevice->aHubPortState[pHubDevice->bActivePortNum-1];

    *pPortState = MUSB_HUB_PORT_STATE_C_OVERCURRENT_COMPLETE;
    MGC_HubGetPortStatusCallback(pParam, &(pHubDevice->ctrlIrp));
    UNUSED(hBus);

    return;
}

/*******************************************************************************
 *              MGC_HubOverCurrentPortTimer ()                                 *
 *******************************************************************************/

void MGC_HubOverCurrentPortTimer(MUSB_Hub *pHubDevice, uint32_t dwWaitingTime)
{
    uint32_t dwStatus;
    uint8_t *pPortState;

    MUSB_ASSERT(pHubDevice);

    MUSB_ASSERT((pHubDevice->bActivePortNum <= MUSB_HUB_MAX_PORTS) && 
        (pHubDevice->bActivePortNum >= 1));

    if (((pHubDevice->bActivePortNum-1) >= (uint8_t)MUSB_HUB_MAX_PORTS ) ||
        (pHubDevice->bActivePortNum < 1))
    {
        LOG(0, "bActivePortNum=%d, overflow.\n", pHubDevice->bActivePortNum);
        return;
    }
    
    pPortState = &pHubDevice->aHubPortState[pHubDevice->bActivePortNum-1];

    dwStatus = MGC_HubStartTimer(pHubDevice->busHandle, pHubDevice->bIndexAtHubDeviceList,
                             dwWaitingTime, MGC_HubOverCurrentPortTimerCallback,
                             (void *)pHubDevice);

    if (MUSB_SUCCESS != dwStatus)
    {
        *pPortState = MUSB_HUB_PORT_STATE_DEFAULT;
    }
    return;
} /* End of function MGC_HubOverCurrentPortTimer () */

/*******************************************************************
 *                 MGC_HubHandlePortDisconnect ()                  *
 *******************************************************************/
void MGC_HubHandlePortDisconnect(MUSB_Hub *pHubDevice, uint8_t bPortNum)
{
    MUSB_Device *pChildUsbDevice;

    MUSB_Hub *pChildHubDevice;

    MUSB_ASSERT(pHubDevice);
    MUSB_ASSERT((bPortNum <= MUSB_HUB_MAX_PORTS) && (bPortNum >= 1));
    
    LOG(0, "HubPort-%d: Disconnect.\n", bPortNum);
    /* Get the USB device pointer of the port where device is disconnect.
     */

    /* 1. Get Index of the device disconnected from bPortNum.
     * 2. Extract USBDevicePointer of disconnected device.
     * 3. Check That Disconnected device is Hub of normal USB device.
     * 4. If it is hub then Call De-Enumeate Hub otherwise call De-Enumerate device.
     * 5. Make pHubDevice->aHubChildPortMap[bChildIndex]  = MUSB_HUB_CHILD_FREE;
     */
    if (((bPortNum-1) >= (uint8_t)MUSB_HUB_MAX_PORTS ) ||
        (bPortNum < 1))
    {
        LOG(0, "bPortNum=%d, overflow.\n", bPortNum);
        return;
    }
    
    if ((MUSB_HUB_CHILD_PRESENT == pHubDevice->aHubChildPortMap[bPortNum-1]) ||
        (pHubDevice->pChild[bPortNum-1]))
    {
        /* Control comes here it means device is allready there 
         * hence de-enumerate it. 
         */
        pChildUsbDevice = pHubDevice->pChild[bPortNum-1];
        MUSB_ASSERT(pChildUsbDevice);
        /* Disconnected device is handled properly so Its time to update "childPortMap" */
        pHubDevice->aHubChildPortMap[bPortNum-1] = MUSB_HUB_CHILD_FREE;
        pHubDevice->pChild[bPortNum-1] = NULL;

        LOG(0, "(%d) pUsbDevice = 0x%08X:\n", 
            pHubDevice->bIndexAtHubDeviceList, (uint32_t)pHubDevice->pUsbDevice);
        
        if (MUSB_CLASS_HUB != pChildUsbDevice->DeviceDescriptor.bDeviceClass)
        {
            LOG(0, "HubPort-%d: Child = 0x%08X is not a hub.\n", 
                bPortNum, (uint32_t)pChildUsbDevice);        

            /* disconnect device first. */
            MUSB_DeviceDisconnected(pChildUsbDevice);
        }
        else
        {
            /* the device was a hub */
            LOG(0, "HubPort-%d: Child = 0x%08X is also a hub.\n", 
                bPortNum, (uint32_t)pChildUsbDevice);

            if (MUSB_SUCCESS != MGC_HubFindHubDevice(pChildUsbDevice, &pChildHubDevice))
            {
                MUSB_DIAG_STRING(1, "HubError: This Is Not A Hub Device/NULL");

                /* disconnect device only. */
                MUSB_DeviceDisconnected(pChildUsbDevice);
                return;
            }

            MGC_DeEnumerateHub(pChildHubDevice);

            /* disconnect hub device after MGC_DeEnumerateHub. */
            MUSB_DeviceDisconnected(pChildUsbDevice);
         }
    }

    return;
} /* End of function MGC_HubHandlePortDisconnect() */

/*******************************************************************
 *                 MGC_DeEnumerateHub ()                           *
 *******************************************************************/
void MGC_DeEnumerateHub(MUSB_Hub *pHubDevice)
{
    MUSB_Device *pChildUsbDevice;
    uint8_t bI;
    
    MUSB_ASSERT(pHubDevice);

    LOG(0, "HUB: MGC_DeEnumerateHub() >>\n", NULL);

    LOG(0, "(%d) pUsbDevice = 0x%08X:\n", 
        pHubDevice->bIndexAtHubDeviceList, (uint32_t)pHubDevice->pUsbDevice);

    /* 1. Find out any USB device is attached with disconnected hub's port or not.
     * 2. If any Usb device is conectd then find out is it Hub device or normal USB device.
     * 3. If it is hub device then Call De-EnumerateHub again otherwise call De-Enumerate device.
     * 4. Once all the child Handled then it is time to "De-Enumerate Hub as Device" if it is
     *    not root hub.
     */
    for (bI = 0x00; bI < MUSB_HUB_MAX_PORTS; bI++)
    {
        pHubDevice->aHubChildPortMap[bI] = MUSB_HUB_CHILD_FREE;        
            
        if (pHubDevice->pChild[bI])
        {
            pChildUsbDevice = pHubDevice->pChild[bI];
            pHubDevice->pChild[bI] = NULL;

            if (pChildUsbDevice->pDriverPrivateData != 
                pHubDevice->pUsbDevice->pDriverPrivateData)
            {            
                LOG(0, "HubPort-%d: Child = 0x%08X is not a hub.\n", 
                    (bI+1), (uint32_t)pChildUsbDevice);
            }
            else
            {
                LOG(0, "HubPort-%d: Child = 0x%08X is also a hub.\n", 
                    bI, (uint32_t)pChildUsbDevice);
            }
            
            /* disconnect device. */
            MUSB_DeviceDisconnected(pChildUsbDevice);            
        } /* End of if (pChildUsbDevice) */        
    }     /* End of for (bI)      */

    LOG(0, "HUB: MGC_DeEnumerateHub() <<\n", NULL);
    return;
} /* End of function MGC_DeEnumerateHub()  */

/*******************************************************************
 *                 MGC_HubHandleEnablePort ()                      *
 *******************************************************************/

void MGC_HubHandleEnablePort(MUSB_Hub *pHubDevice, uint8_t *pPortState, uint8_t bPortNum)
{
    uint32_t dwStatus;

    MUSB_ASSERT(pHubDevice);
    MUSB_ASSERT(pPortState);

    MUSB_DIAG_STRING(2, "HUB: MGC_HubHandleEnablePort()");
    
    dwStatus = MUSB_HubClearPortFeature(pHubDevice, (uint16_t)MUSB_HUB_C_PORT_ENABLE_FEATURE,
                                        MGC_HubGetPortStatusCallback, bPortNum);

    *pPortState = (MUSB_SUCCESS == dwStatus) ? 
        MUSB_HUB_PORT_STATE_C_ENABLE_COMPLETE : MUSB_HUB_PORT_STATE_DEFAULT;

    return;
} /* End of function MGC_HubHandleEnablePort () */

/*******************************************************************
 *                 MGC_HubHandleSuspendPort()                      *
 *******************************************************************/
void MGC_HubHandleSuspendPort(MUSB_Hub *pHubDevice, uint8_t *pPortState, uint8_t bPortNum)
{
    uint32_t dwStatus;

    MUSB_ASSERT(pHubDevice);
    MUSB_ASSERT(pPortState);

    MUSB_DIAG_STRING(2, "HUB: MGC_HubHandleSuspendPort()");

    dwStatus = MUSB_HubClearPortFeature(pHubDevice, (uint16_t)MUSB_HUB_C_PORT_SUSPEND_FEATURE,
                                        MGC_HubGetPortStatusCallback, bPortNum);

    *pPortState = (MUSB_SUCCESS == dwStatus) ? 
        MUSB_HUB_PORT_STATE_C_SUSPEND_COMPLETE : MUSB_HUB_PORT_STATE_DEFAULT;

    return;
} /* End of function MGC_HubHandleSuspendPort () */

/*******************************************************************
 *                 MGC_HubStoreStatus ()                           *
 *******************************************************************/
void MGC_HubStoreStatus(uint8_t *pStatusData, MUSB_HubPortStatus *pHubStatus)
{
    uint32_t dwTempVar = 0;
    
    MUSB_ASSERT(pStatusData);
    MUSB_ASSERT(pHubStatus);

    /* Once Get Bits are used then no need to Swap the fields */       
    MUSB_MemCopy((void*)&dwTempVar, (const void*)pStatusData, (uint32_t)2);
    pHubStatus->wStatus = (uint16_t)dwTempVar;
    pStatusData = pStatusData + 2;
    MUSB_MemCopy((void*)&dwTempVar, (const void*)pStatusData, (uint32_t)2);
    pHubStatus->wStatusChange = (uint16_t)dwTempVar;

    return;
} /* End of function  MGC_HubStoreStatus() */

/**************************************************************************
 *Function    : MUSB_HubPrepareSetupPkt()
 *Purpose     : This function prepars Setup packet. 
***************************************************************************/
/* TBD we have to take decision that prepare SETUP packet should be
 * MACRO or Function.
 */
void MUSB_HubPrepareSetupPkt(MUSB_DeviceRequest *pSetup, uint8_t bmRequestType, uint8_t bRequest,
                             uint16_t wValue, uint16_t wIndex, uint16_t wLength)
{
    MUSB_ASSERT(pSetup);

    pSetup->bmRequestType = bmRequestType;
    pSetup->bRequest = bRequest;
    pSetup->wValue = wValue;
    pSetup->wIndex = wIndex;
    pSetup->wLength = wLength;

    /*
     * Swap double byte fields of Setup packet
     */
    pSetup->wValue = (uint16_t)MUSB_SWAP16(pSetup->wValue);
    pSetup->wIndex = (uint16_t)MUSB_SWAP16(pSetup->wIndex);
    pSetup->wLength = (uint16_t)MUSB_SWAP16(pSetup->wLength);

    return;
} /* End of function MUSB_PrepareSetupPkt ()*/

#endif /* MUSB_HUB */
