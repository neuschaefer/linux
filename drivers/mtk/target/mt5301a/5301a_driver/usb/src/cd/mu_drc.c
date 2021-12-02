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
 * Implementations of DRC-specific functions
 * $Revision: #1 $
 */

#include "mu_impl.h"
#include "mu_drcpr.h"
#include "mu_drcdf.h"

#include "mu_hstpr.h"
#include "mu_funpr.h"
#include "mu_pippr.h"

#ifndef MUSB_VBUS_ERROR_RETRIES
#define MUSB_VBUS_ERROR_RETRIES 4
#endif
#ifndef MUSB_BABBLE_RETRIES
#define MUSB_BABBLE_RETRIES 4
#endif

/**************************** FORWARDS ****************************/

static uint8_t MGC_ExitingHost(uint8_t toOtg, MGC_Port *pPort);

static void MGC_ExitActiveUsbMode(uint8_t toOtg, MGC_Port *pPort);
#ifdef MUSB_OTG_FUNC

static void MGC_StartPeripheralMode(MGC_Port *pPort);
static void MGC_AidlBdisTimeout(void *pParam, uint16_t wTimer);
static void MGC_WaitSessReqTimeout(void *pParam, uint16_t wTimer);
#endif

static void MGC_HostResetStart(void *pParam, uint16_t wTimer);
static void MGC_HostResetComplete(void *pParam, uint16_t wTimer);
static void MGC_DrcCompleteResume(void *pParam, uint16_t wTimer);
static void MGC_DrcFlushAll(MGC_Port *pPort);
#ifdef MUSB_OTG_FUNC

static void MGC_SetHnpEnableIrpComplete(void *pCompleteParam, MUSB_ControlIrp *pIrp);
static void MGC_SetHnpSupportIrpComplete(void *pCompleteParam, MUSB_ControlIrp *pIrp);
static void MGC_OtgStateGetId(MGC_Port *pPort, uint8_t bIsReset);
#endif

/**************************** GLOBALS *****************************/
#ifdef MUSB_OTG_FUNC
/** SET_FEATURE(a_hnp_support) request */

static const uint8_t MGC_aSetHnpSupportData [] =
{
    MUSB_DIR_OUT | MUSB_TYPE_STANDARD | MUSB_RECIP_DEVICE, MUSB_REQ_SET_FEATURE, MUSB_a_hnp_support,
    0, 0, 0,
    0, 0
};

/** SET_FEATURE(b_hnp_enable) request */
static const uint8_t MGC_aSetHnpEnableData [] =
{
    MUSB_DIR_OUT | MUSB_TYPE_STANDARD | MUSB_RECIP_DEVICE, MUSB_REQ_SET_FEATURE, MUSB_b_hnp_enable,
    0, 0, 0,
    0, 0
};

/**
 * IRP for setting a_hnp_support on root device.
 * NOTE: this singleton does not support multiple DRCs,
 * but the OTG standard doesn't really allow it anyway.
 */
static MUSB_ControlIrp MGC_SetHnpSupportIrp =
{
    0,    /* fill bBusAddress */
    MGC_aSetHnpSupportData,
    sizeof(MGC_aSetHnpSupportData),
    NULL, /* pInBuffer */
    0,    /* dwInLength */
    0,    /* dwStatus */
    0,    /* dwActualOutLength */
    0,    /* dwActualInLength */
    MGC_SetHnpSupportIrpComplete,
    NULL,
};

/**
 * IRP for setting b_hnp_enable on root device.
 * NOTE: this singleton does not support multiple DRCs,
 * but the OTG standard doesn't really allow it anyway.
 */
static MUSB_ControlIrp MGC_SetHnpEnableIrp =
{
    0,    /* fill bBusAddress */
    MGC_aSetHnpEnableData,
    sizeof(MGC_aSetHnpEnableData),
    NULL, /* pInBuffer */
    0,    /* dwInLength */
    0,    /* dwStatus */
    0,    /* dwActualOutLength */
    0,    /* dwActualInLength */
    MGC_SetHnpEnableIrpComplete,
    NULL,
};
#endif

#ifdef MUSB_EHSET
extern uint32_t MGC_dwEL35Reset;
#endif
/*************************** FUNCTIONS ****************************/

/*
 * Discover DRC FIFO configuration
 */
#ifndef MUSB_C_DYNFIFO_DEF
uint8_t MGC_DrcInit(MGC_Port *pPort)
{
    uint8_t bEnd;

    MGC_EndpointResource end;
    uint8_t bFifoSize;
    MGC_Controller *pImplController;
    uint8_t *pBase;
    uint8_t bOk = FALSE;

    MUSB_ASSERT(pPort);

    pImplController = pPort->pController;
    pBase = (uint8_t *)pImplController->pControllerAddressIst;
    
    /* count non-default endpoints by reading core */
    pPort->bEndCount = 1;
    pPort->wEndMask = 1;

    for (bEnd = 1; bEnd < MGC_MAX_USB_ENDS; bEnd++)
    {
        MGC_Write8(pBase, MGC_O_DRC_INDEX, bEnd);

        bFifoSize = MGC_Read8(pBase, MGC_O_DRC_FIFOSIZE);

        if (!bFifoSize)
        {
            /* 0's returned when no more endpoints */
            break;
        }

        pPort->bEndCount++;
        pPort->wEndMask |= (1 << bEnd);
    }

    pPort->pPrivateData = MUSB_MemAlloc(pPort->bEndCount *sizeof(MGC_EndpointResource));

    if (pPort->pPrivateData)
    {
        if (MUSB_ArrayInit(&(pPort->LocalEnds), sizeof(MGC_EndpointResource), pPort->bEndCount,
                           pPort->pPrivateData))
        {
            bOk = TRUE;

            /* add endpoint 0 */
            MUSB_MemSet((void *)&end, 0, sizeof(end));
            end.bIsFifoShared = TRUE;
            end.bTrafficType = MUSB_ENDPOINT_XFER_CONTROL;
            end.bRxTrafficType = MUSB_ENDPOINT_XFER_CONTROL;
            end.wMaxPacketSizeTx = MGC_END0_FIFOSIZE;
            end.wMaxPacketSizeRx = MGC_END0_FIFOSIZE;
            end.bIsClaimed = TRUE;
            end.bRxClaimed = TRUE;
            MUSB_ArrayAppend(&(pPort->LocalEnds), &end);

            /* add others */
            for (bEnd = 1; bEnd < pPort->bEndCount; bEnd++)
            {
                MGC_Write8(pBase, MGC_O_DRC_INDEX, bEnd);

                bFifoSize = MGC_Read8(pBase, MGC_O_DRC_FIFOSIZE);
                MUSB_MemSet((void *)&end, 0, sizeof(end));
                end.bLocalEnd = bEnd;
                end.wMaxPacketSizeTx = 1 << (bFifoSize & 0x0f);
                /* shared TX/RX FIFO? */
                if ((bFifoSize & 0xf0) == 0xf0)
                {
                    end.wMaxPacketSizeRx = 1 << (bFifoSize & 0x0f);
                    end.bIsFifoShared = TRUE;
                }
                else
                {
                    end.wMaxPacketSizeRx = 1 << ((bFifoSize & 0xf0) >> 4);
                    end.bIsFifoShared = FALSE;
                }
                MUSB_ArrayAppend(&(pPort->LocalEnds), &end);
            }
        }
        else
        {
            MUSB_MemFree(pPort->pPrivateData);
        }
    }

    /* select endpoint 0 */
    MGC_Write8(pBase, MGC_O_DRC_INDEX, 0);

    return bOk;
}
#endif
/*
 * DRC Interrupt Service Routine
 */
int32_t MGC_DrcIsr(MGC_Controller *pController, uint8_t bIntrUsbValue, uint16_t wIntrTxValue,
                   uint16_t wIntrRxValue)
{
    MGC_BsrItem qItem;

    uint32_t dwRegVal;
    uint8_t bQueue;
    int32_t iShift;
    int32_t iResult = -1;
    MUSB_SystemServices *pServices;
    MGC_Port *pPort;
    
    MUSB_ASSERT(pController);

    pServices = pController->pSystemServices;
    pPort = pController->pPort;

#ifdef MUSB_DMA

    pPort->bDmaCompleted = FALSE;

    if (pPort->pDmaController && pPort->pDmaController->pfDmaControllerIsr)
    {
        if (pPort->pDmaController->pfDmaControllerIsr(pPort->pDmaController->pPrivateData))
        {
            iResult = pPort->bDmaCompleted;
        }
    }
#endif /* DMA enabled */
    
    //  return when no more interrupt to be handled.
    if ((!bIntrUsbValue) && (!wIntrTxValue) && (!wIntrRxValue))
    {
        return iResult;
    }

    if (bIntrUsbValue)
    {
        LOG(9, "IntrUSB= 0x%X.\n", bIntrUsbValue);
        iResult = MGC_DrcUsbIsr(pPort, bIntrUsbValue);
    }

    LOG(9, "IntrTx= 0x%X, IntrRx=0x%X.\n", wIntrTxValue, wIntrRxValue);

    /* scan for Tx endpoints requiring servicing */
    dwRegVal = wIntrTxValue;

    if (dwRegVal && (iResult < 0))
    {
        iResult = 0;
    }
    /* the low bit is a special case: Tx or Rx on endpoint 0 */
    if (dwRegVal & 1)
    {
        bQueue = pPort->pfServiceDefaultEnd(pPort, &qItem);
        if (bQueue)
        {
            iResult++;
            VERIFY(pServices->pfQueueBackgroundItem(pServices->pPrivateData, &qItem));
        }
    }

    /* scan for Tx endpoints 1-15 requiring servicing */
    iShift = 1;
    dwRegVal >>= 1;

    while (dwRegVal)
    {
        if (dwRegVal & 1)
        {
            bQueue = pPort->pfServiceTransmitAvail(pPort, iShift, &qItem);
            if (bQueue)
            {
                iResult++;
                qItem.bLocalEnd = iShift;
                VERIFY(pServices->pfQueueBackgroundItem(pServices->pPrivateData, &qItem));
            }
        }

        dwRegVal >>= 1;
        iShift++;
    }

    /* scan for Rx endpoints 1-15 requiring servicing */
    dwRegVal = wIntrRxValue;

    if (dwRegVal && (iResult < 0))
    {
        iResult = 0;
    }

    iShift = 1;
    dwRegVal >>= 1;

    while (dwRegVal)
    {
        if (dwRegVal & 1)
        {
            bQueue = pPort->pfServiceReceiveReady(pPort, iShift, &qItem);
            if (bQueue)
            {
                iResult++;
                qItem.bLocalEnd = iShift;
                VERIFY(pServices->pfQueueBackgroundItem(pServices->pPrivateData, &qItem));
            }
        }

        dwRegVal >>= 1;
        iShift++;
    }

   
    return iResult;
}

/*
* Find the DRC's first (host) or required (function) 
* local endpoint resource to communicate with the given remote endpoint (host)
* or to be identified with the given endpoint descriptor (function).
*/

#ifndef MUSB_C_DYNFIFO_DEF
MGC_EndpointResource *MGC_DrcBindEndpoint(MGC_Port *pPort, const MUSB_DeviceEndpoint *pUsbEnd,
                                          MUSB_EndpointResource *pRequest, uint8_t bBind,
                                          uint8_t *pbIsTx)
{
    uint32_t dwEndCount;

    uint32_t dwEndIndex;
    uint8_t bIsIn;
    uint8_t bIsTx;
#ifdef MUSB_FUNCTION

    uint8_t bEnd;
#endif

    uint8_t bIsClaimed;
    uint16_t wPacketSize;
    MGC_EndpointResource *pEnd;
    int32_t dwDiff;
    int32_t dwReqDiff;
    uint16_t wMaxPacketSize;
    MGC_EndpointResource *pBestEnd = NULL;
    MGC_EndpointResource *pBestReqEnd = NULL;
    uint16_t wBestDiff = 0xffff;
    uint16_t wBestReqDiff = 0xffff;

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pUsbEnd);
    MUSB_ASSERT(pRequest);
    MUSB_ASSERT(pbIsTx);
    
    wPacketSize = MUSB_SWAP16P((uint8_t *)&(pUsbEnd->UsbDescriptor.wMaxPacketSize));
    bIsIn = (pUsbEnd->UsbDescriptor.bEndpointAddress &MUSB_ENDPOINT_DIR_MASK) ? TRUE : FALSE;
    bIsTx = pPort->bIsHost ? !bIsIn : bIsIn;
    dwEndCount = MUSB_ArrayLength(&(pPort->LocalEnds));

    for (dwEndIndex = 0; dwEndIndex < dwEndCount; dwEndIndex++)
    {
        pEnd = (MGC_EndpointResource *)MUSB_ArrayFetch(&(pPort->LocalEnds), dwEndIndex);

        bIsClaimed = pEnd->bIsFifoShared
                         ? pEnd->bIsClaimed : (bIsTx ? pEnd->bIsClaimed : pEnd->bRxClaimed);
        if (pEnd && !bIsClaimed)
        {
            wMaxPacketSize = bIsTx ? pEnd->wMaxPacketSizeTx : pEnd->wMaxPacketSizeRx;
            if (pPort->bIsHost)
            {
                /* host role: need compatible direction and try for closest FIFO size match */
                dwDiff = wMaxPacketSize - wPacketSize;

                MUSB_ASSERT(wMaxPacketSize > pRequest->dwBufferSize);
                dwReqDiff = wMaxPacketSize - (uint16_t)pRequest->dwBufferSize;
                if (dwDiff >= 0)
                {
                    /* found candidate; see if it is the best thus far */
                    if ((uint16_t)dwDiff < wBestDiff)
                    {
                        /* best */
                        wBestDiff = (uint16_t)dwDiff;
                        pBestEnd = pEnd;
                    }
                    /* also, see how close we are to properties requested */
                    if ((uint16_t)dwReqDiff < wBestReqDiff)
                    {
                        /* best */
                        wBestReqDiff = (uint16_t)dwReqDiff;
                        pBestReqEnd = pEnd;
                    }
                }
            }
#ifdef MUSB_FUNCTION

            else
            {
                /* function role: need address match, compatible direction and FIFO size */
                bEnd = pUsbEnd->UsbDescriptor.bEndpointAddress & MUSB_ENDPOINT_NUMBER_MASK;
                if ((wMaxPacketSize >= wPacketSize) && (pEnd->bLocalEnd == bEnd))
                {
                    /* found match */
                    pBestEnd = pEnd;
                    break;
                }
            }
#endif

        }
    }

    /* see if we fulfilled desire */
    if (pBestReqEnd)
    {
        pBestEnd = pBestReqEnd;
    }

    /* if we found one, set it up */
    if (pBestEnd && bBind)
    {
        if (pBestEnd->bIsFifoShared || bIsTx)
        {
            pBestEnd->bIsClaimed = TRUE;
        }
        else
        {
            pBestEnd->bRxClaimed = TRUE;
        }

        pBestEnd->bIsTx = bIsTx;
        pBestEnd->bIsHalted = FALSE;
        if (bIsTx)
        {
            pBestEnd->bBusAddress = pUsbEnd->pDevice->bBusAddress;

            pBestEnd->bBusEnd = pUsbEnd->UsbDescriptor.bEndpointAddress;
            pBestEnd->bInterval = pUsbEnd->UsbDescriptor.bInterval;
            pBestEnd->bTrafficType = pUsbEnd->UsbDescriptor.bmAttributes;
            pBestEnd->wPacketSize = wPacketSize;
        }
        else
        {
            pBestEnd->bRxBusAddress = pUsbEnd->pDevice->bBusAddress;

            pBestEnd->bRxBusEnd = pUsbEnd->UsbDescriptor.bEndpointAddress;
            pBestEnd->bRxInterval = pUsbEnd->UsbDescriptor.bInterval;
            pBestEnd->bRxTrafficType = pUsbEnd->UsbDescriptor.bmAttributes;
            pBestEnd->wRxPacketSize = wPacketSize;
        }
    }

    *pbIsTx = bIsTx;
    return pBestEnd;
}
#endif
/*
 * Handle default endpoint interrupt
 */
uint8_t MGC_DrcServiceDefaultEnd(MGC_Port *pPort, MGC_BsrItem *pItem, uint16_t *pwCsr,
                                 uint16_t wCount)
{
#ifdef MUSB_FUNCTION

    uint16_t wFifoCount, wSpace;

    uint8_t *pFifoDest;
    MUSB_DeviceRequest *pRequest;
    uint8_t bVal;
    uint32_t dwRet = 0;
#endif

    uint16_t wOutVal = 0;
    uint8_t bResult = FALSE;
    uint8_t status = MUSB_STATUS_OK;
    MGC_EndpointResource *pEnd;
    MUSB_ControlIrp *pIrp;

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pItem);
    MUSB_ASSERT(pwCsr);

#ifdef MUSB_FUNCTION
    bVal = (uint8_t) *pwCsr;
#endif

    pEnd = (MGC_EndpointResource *)MUSB_ArrayFetch(&(pPort->LocalEnds), 0);
    if (!pEnd)
    {
        /* prevent caller from touching CSR0 */
        *pwCsr = 0;
        return TRUE;
    }

    if (pPort->bIsHost)
    {
        //  MTK Notice: Max Liao, 2007/07/27.
        //  2 case to handle device return null packet.
        //  (1) there is no data phase, null packet is device status packet.
        //  (2) there is data phase, null packet is the last packet in data phase.
        //       We need to out status packet in this condition.        
        pIrp = (MUSB_ControlIrp *)pEnd->pTxIrp;

        if ((pEnd->bIsTx && !pEnd->dwTxSize) || ((!pEnd->bIsTx) && (!wCount) && (!pIrp->pInBuffer)))
        {
            /* assume status phase complete */
            *pwCsr = 0;

            //  handle control IRP in HISR.
            bResult = MGC_CompleteIrp(pItem, pEnd, (uint8_t)status, TRUE, pEnd->pTxIrp);
            /* start next transfer if any */
            if (bResult)
            {
                MGC_StartNextControlTransfer(pPort);
            }
            return bResult;
        }

        /* call common logic and prepare response */
        if (!MGC_DrcServiceHostDefault(pPort, wCount, status, &wOutVal))
        {
            /* data transfer complete; perform status phase */
            wOutVal = MGC_M_CSR0_H_STATUSPKT;
            if (pEnd->bIsTx)
            {
                pEnd->bIsTx = FALSE;
                wOutVal |= MGC_M_CSR0_H_REQPKT;
            }
            else
            {
                pEnd->bIsTx = TRUE;

                pEnd->dwTxSize = 0;
                wOutVal |= MGC_M_CSR0_TXPKTRDY;
            }
        }
    }
#ifdef MUSB_FUNCTION

    else
    {
        /* peripheral mode: see if Rx or Tx */
        if (bVal & MGC_M_CSR0_RXPKTRDY)
        {
            pPort->bEnd0Status = FALSE;
            /* receive */
            wSpace = pPort->pFunctionClient
                         ? (pPort->pFunctionClient->wControlBufferLength - pPort->wSetupDataIndex)
                         : 0;
            wFifoCount = MUSB_MIN(wCount, wSpace);
            /* stall on potential overflow or get data */
            if (!pPort->pFunctionClient || (wCount > wFifoCount))
            {
                /* stall */
                wOutVal |= MGC_M_CSR0_P_SENDSTALL;
            }
            else
            {
                /* unload FIFO to the right place */
                pFifoDest = pPort->pFunctionClient->pControlBuffer + pPort->wSetupDataIndex;
#ifdef MUSB_DMA

                if (pEnd->pDmaChannel && pPort->pDmaController->pfDmaProgramChannel(
                                             pEnd->pDmaChannel, pEnd->wPacketSize,
                                             pEnd->pDmaChannel->bDesiredMode, pFifoDest,
                                             MGC_MIN(wFifoCount, pEnd->pDmaChannel->dwMaxLength)))
                {
                    /* prevent caller touching CSR0 */
                    *pwCsr = 0;
                    return TRUE;
                }
#endif

                pPort->pfUnloadFifo(pPort, 0, wFifoCount, pFifoDest);
                pPort->wSetupDataIndex += wFifoCount;
                /* check request */
                pRequest = (MUSB_DeviceRequest *)((uint32_t)pPort->pFunctionClient->pControlBuffer);
                /* if IN or we got enough OUT data, parse */
                if ((pRequest->bmRequestType&MUSB_DIR_IN)
                        || ((pPort->wSetupDataIndex - 8) >= MUSB_SWAP16P(
                                                                (uint8_t *)&(pRequest->wLength))))
                {
                    pPort->dwSequenceNumber++;

                    pPort->wSetupDataSize = pPort->wSetupDataIndex;
                    pPort->wSetupDataIndex = 0;
                    bResult = FALSE;
                    /* if parsing enabled */
                    if (pPort->bParse)
                    {
                        /* try to handle it now */
                        if (!MGC_FunctionParseSetup(pPort, &status))
                        {
                            /* callback */
                            if (!pPort->pFunctionClient->pfDeviceRequest(
                                     pPort->pFunctionClient->pPrivateData,
                                     (MUSB_BusHandle)pPort,
                                     pPort->dwSequenceNumber,
                                     pPort->pFunctionClient->pControlBuffer,
                            //  Bug: transfer wSetupDataSize not wSetupDataIndex.				
                                     pPort->wSetupDataSize))
                            {
                                /* client rejected immediately, so STALL */
                                dwRet = pPort->pfDefaultEndResponse(pPort, TRUE);
                            }
                        }
                        else
                        {
                            /* parser handled it */
                            if ((uint8_t)MUSB_STATUS_STALLED == status)
                            {
                                pPort->wSetupDataIndex = 0;

                                pPort->wSetupDataSize = 0;
                                dwRet = pPort->pfDefaultEndResponse(pPort, TRUE);
                            }
                            else
                            {
                                dwRet = pPort->pfDefaultEndResponse(pPort, FALSE);
                            }
                        }
                    }
                    else
                    {
                        /* parsing disabled; issue callback */
                        if (!pPort->pFunctionClient->pfDeviceRequest(
                                 pPort->pFunctionClient->pPrivateData, (MUSB_BusHandle)pPort,
                                 pPort->dwSequenceNumber, pPort->pFunctionClient->pControlBuffer,
                        //  Bug: transfer wSetupDataSize not wSetupDataIndex.				
                                 pPort->wSetupDataSize))
                        {
                            /* client rejected immediately, so STALL */
                            dwRet = pPort->pfDefaultEndResponse(pPort, TRUE);
                        }
                    }
                }
                else
                {
                    /* acknowledge partial message */
                    wOutVal = MGC_M_CSR0_P_SVDRXPKTRDY;
                }
            }
        }
        else if (!(bVal &MGC_M_CSR0_TXPKTRDY))
        {
            /* transmit-ready */
            wFifoCount = (uint16_t)MUSB_MIN(64, (pPort->wSetupDataSize - pPort->wSetupDataIndex));

            if (!wFifoCount)
            {
                /* status ack */
                pPort->wSetupDataIndex = 0;

                pPort->wSetupDataSize = 0;
                return FALSE;
            }

            pFifoDest = pPort->pSetupData + pPort->wSetupDataIndex;
#ifdef MUSB_DMA

            if (pEnd->pDmaChannel && pPort->pDmaController->pfDmaProgramChannel(
                                         pEnd->pDmaChannel, pEnd->wPacketSize,
                                         pEnd->pDmaChannel->bDesiredMode, pFifoDest,
                                         MGC_MIN(wFifoCount, pEnd->pDmaChannel->dwMaxLength)))
            {
                /* prevent caller touching CSR0 */
                *pwCsr = 0;
                return TRUE;
            }
#endif
            /* load and signal need to transmit */

            pPort->pfLoadFifo(pPort, 0, wFifoCount, pFifoDest);
            pPort->wSetupDataIndex += wFifoCount;
            wOutVal |= MGC_M_CSR0_TXPKTRDY;
            if (pPort->wSetupDataIndex >= pPort->wSetupDataSize)
            {
                wOutVal |= MGC_M_CSR0_P_DATAEND;

                pPort->wSetupDataIndex = 0;
                pPort->wSetupDataSize = 0;
            }
        }
    }

    UNUSED(dwRet);
#endif

    *pwCsr = wOutVal;
    return bResult;
}

/* Reset the port (e.g. unbind/flush all endpoints, etc.) */
void MGC_DrcResetPort(MGC_Port *pPort)
{
    MGC_Controller *pController;
    uint8_t bRet;
    uint32_t i;
    
    MUSB_ASSERT(pPort);

    pController = pPort->pController;

    MUSB_ASSERT(pController->pSystemServices);
    MUSB_ASSERT(pController->pSystemServices->pfCancelTimer);

    /* kill timer */
    for (i=0; i<MUSB_MAX_TIMERS; i++)    
    {
        bRet = pController->pSystemServices->pfCancelTimer(
            pController->pSystemServices->pPrivateData, i /*bTimerIndex*/);
        UNUSED(bRet);
    }
    
    /* turn off session */
    pPort->bWantSession = FALSE;
    pPort->bFuncAddr = 0;
    pPort->pfProgramBusState(pPort);

    /* stop controller */
    VERIFY(0 == pController->pfProgramStopController(pController));

    /* flush BSR queue */
    pController->pSystemServices->pfFlushBackgroundQueue(
        pController->pSystemServices->pPrivateData);

    MGC_DrcFlushAll(pPort);

    VERIFY(0 == pController->pfProgramStartController(pController));

    /* get current readings */
    VERIFY(0 == pPort->pfReadBusState(pPort));

    pPort->bWantReset = FALSE;
#ifdef MUSB_FUNCTION

    pPort->bUsbState = (uint8_t)MUSB_POWER_OFF;
#endif

#ifdef MUSB_OTG_FUNC
    pPort->bWantHost = FALSE;
    pPort->bOtgState = (uint8_t)MUSB_AB_IDLE;
    MGC_CompleteOtgTransition(pPort);
#endif

}

/**
 * Stop all traffic.
 * Flush all local endpoints, kill all pending IRPs
 * @param pPort port pointer
 */
static void MGC_DrcFlushAll(MGC_Port *pPort)
{
    uint32_t dwEndCount;

    uint32_t dwEndIndex;
    uint16_t  wCount;
    int16_t wIndex;
    MUSB_Device *pDevice;
    void *pIrp;
    MGC_EndpointResource *pEnd;
    uint8_t *pBase;
#ifdef MUSB_FUNCTION

    MGC_BsrItem qItem;
    MUSB_SystemServices *pServices;
#endif

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pPort->pController);
    
    pBase = (uint8_t *)pPort->pController->pControllerAddressIst;
#ifdef MUSB_FUNCTION
    pServices = pPort->pController->pSystemServices;
#endif

    // [DTV00031040] [USB] Plug USB-07 will cause system assert.
    // pPort->bIsHost will be cleared during device disconnect. 
    // if (pPort->bIsHost)
    {
        /* disconnect from root device, search the root device */
        pDevice = (MUSB_Device *)
            MUSB_ListFindItem(&(pPort->ConnectedDeviceList), 0);
        if (pDevice)
        {
            MUSB_ASSERT(pDevice->pParentUsbDevice == NULL);
            MUSB_DeviceDisconnected(pDevice);                
        }
        
        /* Check if clean flush all device from root. */
        wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));
        if (wCount != 0)
        {
            LOG(1, "Do not clean all device from root = %d !!\n", wCount);
            MUSB_ASSERT(wCount == 0);
        }

        // If device is not added to pPort->ConnectedDeviceList, and
        // it is disconnected during enumeration, we need to disconnect it here.
        pDevice = pPort->EnumerationData.pDevice;
        if (pDevice)
        {
            MUSB_DeviceDisconnected(pDevice);
        }
    }

    /* flush all endpoints */
    dwEndCount = MUSB_ArrayLength(&(pPort->LocalEnds));

    //  memory leakage: clear start from ep0.    
    for (dwEndIndex = 0; dwEndIndex < dwEndCount; dwEndIndex++)
    {
        pEnd = (MGC_EndpointResource *)MUSB_ArrayFetch(&(pPort->LocalEnds), dwEndIndex);
        if (pEnd)
        {
            //  execute MGC_CompleteIrp() in peripheral mode when device disconnect.
#ifdef MUSB_FUNCTION

            if (!pPort->bIsHost)
            {
                pIrp = (pEnd->pRxIrp) ? (pEnd->pRxIrp) : ((pEnd->pTxIrp) ? (pEnd->pTxIrp) : (NULL));
                if (pIrp)
                {
                    if (MGC_CompleteIrp(&qItem, pEnd, (uint8_t)MUSB_STATUS_PEER_REMOVED,
                                        pEnd->bIsTx, pIrp))
                    {
                        VERIFY(pServices->pfQueueBackgroundItem(pServices->pPrivateData, &qItem));
                    }
                }
            }
#endif

            //  mark tx/rx ep is released and ep0 is always reserved for ep0.	
            if (dwEndIndex > 0)
            {
                pEnd->bIsClaimed = FALSE;
                pEnd->bRxClaimed = FALSE;
            }

            pEnd->bIsHalted = FALSE;
            pEnd->bIsRxHalted = FALSE;
            pEnd->pTxIrp = NULL;
            pEnd->pRxIrp = NULL;
            VERIFY(0 == pPort->pfProgramFlushEndpoint(pPort, pEnd, 0, TRUE));
            VERIFY(0 == pPort->pfProgramFlushEndpoint(pPort, pEnd, MUSB_ENDPOINT_DIR_MASK, TRUE));
            //  reset bDmaTx after pfProgramFlushEndpoint().
#ifdef MUSB_DMA
//pEnd->bDmaTx = pEnd->bDmaResidual = FALSE;
#endif

            wCount = MUSB_ListLength(&(pEnd->TxIrpList));

            for (wIndex = 0; wIndex < wCount; wIndex++)
            {
                pIrp = MUSB_ListFindItem(&(pEnd->TxIrpList), 0);
                MUSB_ListRemoveItem(&(pEnd->TxIrpList), pIrp);
            }

            wCount = MUSB_ListLength(&(pEnd->RxIrpList));
            for (wIndex = 0; wIndex < wCount; wIndex++)
            {
                pIrp = MUSB_ListFindItem(&(pEnd->RxIrpList), 0);
                MUSB_ListRemoveItem(&(pEnd->RxIrpList), pIrp);
            }
        }
    }

    /* select endpoint 0 */
    MGC_Write8(pBase, MGC_O_DRC_INDEX, 0);
}

/* OTG timer expiration */
#ifdef MUSB_OTG_FUNC
void MGC_DrcOtgTimer(void *pParam, uint16_t wTimer)
{
    MGC_Controller *pController;
    MGC_Port *pPort;
    MUSB_OtgClient *pOtgClient;

    MUSB_ASSERT(pParam);
    UNUSED(wTimer);
    
    pController = (MGC_Controller *)pParam;

    MUSB_ASSERT(pController->pPort);   
    pPort = pController->pPort;
    
    MUSB_ASSERT(pPort->pOtgClient);       
    pOtgClient = (MUSB_OtgClient *)pPort->pOtgClient;

    VERIFY(0 == pPort->pfReadBusState(pPort));

    switch (pPort->bOtgState)
    {
    case MUSB_B_WAIT_ACON:
        if (pPort->bBusVoltage < MGC_VBUS_ABOVE_AVALID)
        {
            MGC_DIAG(1, pPort->pController, "DEVICE NOT RESPONDING (A-device didn't connect)");

            MUSB_ASSERT(pOtgClient);       
            MUSB_ASSERT(pOtgClient->pfOtgError);       

            /* signal error */
            pOtgClient->pfOtgError(pOtgClient->pPrivateData, pOtgClient,
                                   MUSB_STATUS_OTG_NO_RESPONSE);
            /* back to idle state */
            MGC_DrcChangeOtgState((uint8_t)MUSB_AB_IDLE, pPort);
        }
        break;

    case MUSB_B_PERIPHERAL:
    case MUSB_B_HOST:
        if (pPort->bBusVoltage < MGC_VBUS_ABOVE_AVALID)
        {
            MGC_DrcChangeOtgState((uint8_t)MUSB_AB_IDLE, pPort);
        }
        break;
        
    default:
        break;
    }
} /* MGC_DrcOtgTimer */
#endif
/*
 * @return TRUE if leaving an active host mode, else FALSE.
 */

static uint8_t MGC_ExitingHost(uint8_t toOtg, MGC_Port *pPort)
{
    MUSB_ASSERT(pPort);
    
    if ((uint8_t)MUSB_A_SUSPEND == toOtg)
    {
        /* A_SUSPEND itself is not exiting host */
        return (FALSE);
    }
    else if ((uint8_t)MUSB_A_PERIPHERAL == toOtg)
    {
        /* Only way here is from a host(suspend) */
        return (TRUE);
    }
    else if (
        ((uint8_t)MUSB_A_HOST == pPort->bOtgState) || ((uint8_t)MUSB_B_HOST == pPort->bOtgState))
    {
        return (TRUE);
    }

    return (FALSE);
} /* MGC_ExitingHost */

/*
* Flush transfer requests, reset endpoint variables,
* reset DRC registers when exiting an active USB data mode.
*
* Current OTG state is pPort->bOtgState; changing to "toOtg".
* If "toOtg" an HNP change, leave DRC registers alone.
*/
static void MGC_ExitActiveUsbMode(uint8_t toOtg, MGC_Port *pPort)
{
    MUSB_ASSERT(pPort);

#ifdef MUSB_FUNCTION

    MGC_FunctionChangeState(pPort, MUSB_POWERED);
#endif

    /* reset USB address value */
    if (pPort->bFuncAddr != 0)
    {
        pPort->bFuncAddr = 0;
        pPort->pfProgramBusState(pPort);
    }

    UNUSED(toOtg);
} /* MGC_ExitActiveUsbMode */

/*
*  MGC_Start_Peripheral_Mode used to condition DRC for peripheral operation
*  and establish peripheral mode in the core & endpoint objects.
*/
#ifdef MUSB_OTG_FUNC

static void MGC_StartPeripheralMode(MGC_Port *pPort)
{
    MUSB_ASSERT(pPort);

/* allow us to be suspended */
/*
pPort->bWantSuspend = TRUE;
pPort->bWantSession = TRUE;
pPort->pfProgramBusState(pPort);
*/

} /* MGC_StartPeripheralMode */

/*
*  Cancel an A-dev bus suspend, inviting B-device to become host.
*/
static void MGC_AidlBdisTimeout(void *pParam, uint16_t wTimer)
{
    MGC_Controller *pController;
    MGC_Port *pPort;
    MUSB_OtgClient *pOtgClient;

    MUSB_ASSERT(pParam);
    UNUSED(wTimer);
    pController = (MGC_Controller *)pParam;

    MUSB_ASSERT(pController->pPort);    
    pPort = pController->pPort;

    MUSB_ASSERT(pPort->pOtgClient);           
    pOtgClient = (MUSB_OtgClient *)pPort->pOtgClient;

    if (pPort->bIsSuspend || pPort->bIsReset)
    {
        return;
    }

    MGC_DIAG(1, pPort->pController, "DEVICE NOT RESPONDING (did not HNP)");

    /* signal error */
    pOtgClient->pfOtgError(pOtgClient->pPrivateData, pOtgClient, MUSB_STATUS_OTG_NO_RESPONSE);

    /* change to idle (OTG 6.8.1.5) */
    MGC_DrcChangeOtgState((uint8_t)MUSB_AB_IDLE, pPort);
} /* MGC_AidlBdisTimeout */

/*
*  MGC_WaitSessReqTimeout used to time out following writing a 
*  SESSREQ to DRC.  Could result in PERIPHERAL mode(via SRP), or
*  HOST.  If neither, exit back to IDLE.
*/
static void MGC_WaitSessReqTimeout(void *pParam, uint16_t wTimer)
{
    MGC_Controller *pController;
    MGC_Port *pPort;
    MUSB_OtgClient *pOtgClient;

    MUSB_ASSERT(pParam);
    UNUSED(wTimer);
    
    pController = (MGC_Controller *)pParam;

    MUSB_ASSERT(pController->pPort);    
    pPort = pController->pPort;

    MUSB_ASSERT(pPort->pOtgClient);       
    pOtgClient = (MUSB_OtgClient *)pPort->pOtgClient;

    if (pPort->bIsSuspend || pPort->bIsReset)
    {
        return;
    }

    MGC_DIAG(1, pPort->pController, "DEVICE NOT RESPONDING");

    /* signal error */
    pOtgClient->pfOtgError(pOtgClient->pPrivateData, pOtgClient, MUSB_STATUS_OTG_NO_RESPONSE);

    /* change to idle */
    MGC_DrcChangeOtgState((uint8_t)MUSB_AB_IDLE, pPort);
} /* MGC_WaitSessReqTimeout */
#endif
/*
*  MGC_HostResetStart allows us to stall slightly the start of
*  the RESET of the B-device or USB peripheral.  This is needed
*  for HNP following a SUSPEND by a B-HOST condition.  
*/

static void MGC_HostResetStart(void *pParam, uint16_t wTimer)
{
    MGC_Controller *pController;
    MGC_Port *pPort;
    uint8_t bRet;
    
    MUSB_ASSERT(pParam);

    pController = (MGC_Controller *)pParam;

    MUSB_ASSERT(pController->pPort);
    
    pPort = pController->pPort;

    MGC_DIAG(2, pController, "Starting reset");

    // ACD Setting 0x20029410[15] RG_CHIRP_EN_10
    {
        uint32_t u4Reg;
        uint8_t *pBase;
        pBase = (uint8_t *)pPort->pController->pControllerAddressIst;
        u4Reg = MGC_PHY_Read32(pBase, 0x10);
        u4Reg |= 0x00008000;
        MGC_PHY_Write32(pBase, 0x10, u4Reg);
    }


    pPort->bWantReset = TRUE;
    pPort->bWantSuspend = FALSE;    
    pPort->pfProgramBusState(pPort);

    bRet = pController->pSystemServices->pfArmTimer(
                pController->pSystemServices->pPrivateData, 
                0 /*Timer Index 0 is reserved for EP0 control transfer*/, 
                MUSB_BUS_RESET_TIME, 
                FALSE, 
                MGC_HostResetComplete, 
                pController);
                
    UNUSED(bRet);
    UNUSED(wTimer);    
} /* MGC_HostResetStart */

/*
*  MGC_HostResetComplete to commence enumeration, following USB RESET
*/
static void MGC_HostResetComplete(void *pParam, uint16_t wTimer)
{
    uint8_t bAddress;
    uint8_t bSpeed = (uint8_t)MUSB_CONNECTION_SPEED_FULL;
    MGC_Controller *pController;
    MGC_Port *pPort;

    MUSB_ASSERT(pParam);
    pController = (MGC_Controller *)pParam;
    MUSB_ASSERT(pController->pPort); 
    pPort = pController->pPort;

    MGC_DIAG(2, pController, "Stopping reset");
    
    // ACD Setting 0x20029410[15] RG_CHIRP_EN_10
    {
        uint32_t u4Reg;
        uint8_t *pBase;
        pBase = (uint8_t *)pPort->pController->pControllerAddressIst;
        u4Reg = MGC_PHY_Read32(pBase, 0x10);
        u4Reg &= ~0x00008000;
        MGC_PHY_Write32(pBase, 0x10, u4Reg);
    }


    pPort->bWantReset = FALSE;
    pPort->pfProgramBusState(pPort);

    VERIFY(0 == pPort->pfReadBusState(pPort));

    if (pPort->bIsHighSpeed)
    {
        LOG(0, "Device High speed.\n", NULL);
        bSpeed = (uint8_t)MUSB_CONNECTION_SPEED_HIGH;
    }
    else if (pPort->bIsLowSpeed)
    {
        LOG(0, "Device Low speed.\n", NULL);
        bSpeed = (uint8_t)MUSB_CONNECTION_SPEED_LOW;
    }
    else
    {
        LOG(0, "Device Full speed.\n", NULL);
    }

#ifdef MUSB_EHSET
    if (MGC_dwEL35Reset)
    {
        /*
        EL_35: Downstream ports begin sending SOFs within 500us and 
        not sooner than 100us from transmission of the last Chirp (J or K).
        */
        (void)pController->pSystemServices->pfArmTimer(
            pController->pSystemServices->pPrivateData, 0, 10, 
            FALSE, MGC_HostResetStart, pPort->pController);
        return;
    }
#endif

    /* start enumeration */
    bAddress = MGC_AllocateAddress(&(pPort->EnumerationData));

    if (bAddress)
    {
        if (!MGC_EnumerateDevice(pPort, NULL, bAddress, 0, bSpeed, NULL))
        {
            MGC_DIAG(1, pController, "Insufficient memory for enumeration");
        }
    }
    else
    {
        MGC_DIAG(1, pController, "Insufficient addresses for enumeration");
    }

    UNUSED(wTimer);        
} /* MGC_HostResetComplete */

/*
* A timer started when entering b_srp_init reached a vendor specfic limit
* between 5 ~ 30 sec. If failed to enter b_peripheral, it will indicate 
* to the user that the A-device did not respond.
*/
#ifdef MUSB_OTG_FUNC

static void MGC_BSrpFailed(void *pParam, uint16_t wTimer)
{
    MGC_Controller *pController;
    MGC_Port *pPort;
    MUSB_OtgClient *pOtgClient;

    MUSB_ASSERT(pParam);
    pController = (MGC_Controller *)pParam;
    MUSB_ASSERT(pController->pPort);
    pPort = pController->pPort;
    MUSB_ASSERT(pPort->pOtgClient);          
    pOtgClient = (MUSB_OtgClient *)pPort->pOtgClient;

    if (((uint8_t)MUSB_B_SRP_INIT == pPort->bOtgState) || ((uint8_t)
                                                               MUSB_AB_IDLE == pPort->bOtgState))
    {
        /* 
        * if b_sess_vld is true, we should be in B_PERIPHERAL state
        */
        if (pPort->bBusVoltage >= MGC_VBUS_ABOVE_AVALID)
        {
            MGC_DIAG(1, pPort->pController, "B_SRP_Fail: changing to B_PERIPHERAL");

            /* signal error */
            pOtgClient->pfOtgError(pOtgClient->pPrivateData, pOtgClient,
                                   MUSB_STATUS_OTG_SRP_FAILED);
        }
        else
        {
            /* clear sess & hr requests */
            pPort->bWantSession = FALSE;

            pPort->bWantHost = FALSE;
            pPort->pfProgramBusState(pPort);

            MGC_DIAG(1, pPort->pController, "DEVICE NOT RESPONDING (SRP)");

            /* signal error */
            pOtgClient->pfOtgError(pOtgClient->pPrivateData, pOtgClient,
                                   MUSB_STATUS_OTG_NO_RESPONSE);

            /* change to idle */
            MGC_DrcChangeOtgState((uint8_t)MUSB_AB_IDLE, pPort); /* go back to quiescence */
        }
    }

    UNUSED(wTimer);        
} /* MGC_BSrpFail */

/** Timer B_ASE0_BRST expiration callback */
static void MGC_BAase0BrstTimeout(void *pParam, uint16_t wTimer)
{
    MGC_Controller *pController;
    MGC_Port *pPort;
    MUSB_OtgClient *pOtgClient;

    MUSB_ASSERT(pParam);
    pController = (MGC_Controller *)pParam;
    MUSB_ASSERT(pController->pPort);
    pPort = pController->pPort;
    MUSB_ASSERT(pPort->pOtgClient);          
    pOtgClient = (MUSB_OtgClient *)pPort->pOtgClient;

    if ((uint8_t)MUSB_B_WAIT_ACON == pPort->bOtgState)
    {
        MGC_DIAG(1, pPort->pController, "DEVICE NOT RESPONDING (A-device didn't connect)");

        /* signal error */
        pOtgClient->pfOtgError(pOtgClient->pPrivateData, pOtgClient, MUSB_STATUS_OTG_NO_RESPONSE);

        /* clear host request */
        pPort->bWantHost = FALSE;
        pPort->pfProgramBusState(pPort);

        MGC_DrcChangeOtgState((uint8_t)MUSB_B_PERIPHERAL, pPort);
    }
    UNUSED(wTimer);        
}

/** IRP completion callback for the IRP to SET_FEATURE(a_hnp_support) */
static void MGC_SetHnpSupportIrpComplete(void *pCompleteParam, MUSB_ControlIrp *pIrp)
{
    UNUSED(pCompleteParam);
    UNUSED(pIrp);    
/* TODO: anything? */
}

/** IRP completion callback for the IRP to SET_FEATURE(b_hnp_enable) */
static void MGC_SetHnpEnableIrpComplete(void *pCompleteParam, MUSB_ControlIrp *pIrp)
{
    MGC_Port *pPort;
    MUSB_SystemServices *pServices;
    uint8_t bRet;

    MUSB_ASSERT(pCompleteParam);
    MUSB_ASSERT(pIrp);
    pPort = (MGC_Port *)pCompleteParam;
    MUSB_ASSERT(pPort->pController);
    pServices = pPort->pController->pSystemServices;

    if (MUSB_STATUS_OK == pIrp->dwStatus)
    {
        pPort->bWantSuspend = TRUE;

        pPort->pfProgramBusState(pPort);
        bRet = pServices->pfArmTimer(pServices->pPrivateData, 0, MGC_TA_AIDL_BDIS, FALSE,
                    MGC_AidlBdisTimeout, pPort->pController);
        UNUSED(bRet);                              
    }
}

static void MGC_RemoveSuspend(void *pParam, uint16_t wTimer)
{
    MGC_Controller *pController;
    MGC_Port *pPort;

    MUSB_ASSERT(pParam);
    pController = (MGC_Controller *)pParam;
    MUSB_ASSERT(pController->pPort);
    pPort = pController->pPort;

    pPort->bWantSuspend = FALSE;
    pPort->pfProgramBusState(pPort);
    UNUSED(wTimer);        
}
#endif
/*
*  MGC_DrcChangeOtgState effects the changes to the OTG mode paradigms (wha?)
*/

void MGC_DrcChangeOtgState(uint8_t toOtg, MGC_Port *pPort)
{
#ifdef MUSB_OTG_FUNC

    const MUSB_OtgDescriptor *pOtgDesc;
    uint32_t dwStatus = 0;
#endif
    MUSB_SystemServices *pServices;
    uint8_t bRet;

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pPort->pController);
    pServices = pPort->pController->pSystemServices;

    VERIFY(0 == pPort->pfReadBusState(pPort));

    /* kill scheduled event */
    bRet = pServices->pfCancelTimer(pServices->pPrivateData, 0);
    UNUSED(bRet);
    
    /*
     *  If exiting peripheral or host, release held objects
     */
    if ((MGC_ExitingHost(toOtg, pPort)) || 
        (MGC_AB_MODE(pPort->bOtgState) == (uint8_t)MGC_AB_PERIPHERAL))
    {
        MGC_ExitActiveUsbMode(toOtg, pPort);
    }
    
    switch (toOtg)
    {
    case MUSB_AB_IDLE:
        /* no pending requests */
#ifdef MUSB_OTG_FUNC
        pPort->bWantHost = FALSE;
#endif
        pPort->bWantSession = FALSE;
        pPort->pfProgramBusState(pPort);
        break;

    case MUSB_B_SRP_INIT:
#ifdef MUSB_OTG_FUNC

        pPort->bWantSession = TRUE;
        pPort->pfProgramBusState(pPort);
        //  check if operation in host only mode.
        if (pPort->pOtgClient)
        {
            bRet = pServices->pfArmTimer(pServices->pPrivateData, 0, MUSB_TB_SRP_FAIL *1000, FALSE,
                        MGC_BSrpFailed, pPort->pController);
        }
#endif

        break;

    case MUSB_A_WAIT_BCON:
        pPort->bWantSuspend = TRUE;
        pPort->bWantSession = TRUE;
        pPort->pfProgramBusState(pPort);
#ifdef MUSB_OTG_FUNC

        //  check if operation in host only mode.
        if (pPort->pOtgClient)
        {
            bRet = pServices->pfArmTimer(pServices->pPrivateData, 0, MUSB_TS_SESSREQ *1000, FALSE,
                    MGC_WaitSessReqTimeout, pPort->pController);
        }
#endif

        break;

    case MUSB_A_PERIPHERAL:
    case MUSB_B_PERIPHERAL:
        /* nothing to do; just prevent falling to default */
        break;

    case MUSB_B_WAIT_ACON:
#ifdef MUSB_OTG_FUNC

        //  check if operation in host only mode.
        if (pPort->pOtgClient)
        {
            bRet = pServices->pfArmTimer(pServices->pPrivateData, 0, 200, 
                FALSE, MGC_BAase0BrstTimeout, pPort->pController);
        }
#endif

        break;

    /*
    *  must ignore a CONNECT if in A_SUSPEND; otherwise, schedule
    *  the start of the USB Reset with a slight delay.
    */

    case MUSB_A_HOST:
        /* here is A-device only */
        if (pPort->bOtgState != (uint8_t)MUSB_A_SUSPEND)
        {
           bRet =  pServices->pfArmTimer(pServices->pPrivateData, 0, 10, 
                FALSE, MGC_HostResetStart, pPort->pController);
        }
        break;

    /*
    *  For B-Device, DRC has already started the RESET, we need to activate a
    *  timer to terminate the USB Reset.
    */
    case MUSB_B_HOST:
#ifdef MUSB_OTG_FUNC
        /* turn off HostReq bit */

        pPort->bWantHost = FALSE;
        pPort->pfProgramBusState(pPort);
        bRet = pServices->pfArmTimer(pServices->pPrivateData, 0, 100, 
            FALSE, MGC_HostResetComplete, pPort->pController);
#endif

        break;

    case MUSB_A_SUSPEND:
#ifdef MUSB_OTG_FUNC

        if (pPort->pRootDevice)
        {
            pOtgDesc = (MUSB_OtgDescriptor *)((uint32_t)
                           MGC_FindDescriptor((uint8_t *)pPort->pRootDevice->pCurrentConfiguration,
                                              MUSB_SWAP16P((uint8_t *)&(
                                                                          pPort->pRootDevice->pCurrentConfiguration->wTotalLength)),
                                              MUSB_DT_OTG,
                                              0));
            if (pOtgDesc && (pOtgDesc->bmAttributes&MUSB_OTG_HNP))
            {
                MGC_SetHnpEnableIrp.pDevice = pPort->pRootDevice;

                MGC_SetHnpEnableIrp.pCompleteParam = pPort;
                dwStatus = MUSB_StartControlTransfer(pPort->pInterfacePort, &MGC_SetHnpEnableIrp);
                UNUSED(dwStatus);
            }
        }
#endif

        break;

    default:
        MGC_DIAG1(1, pPort->pController, "Error changing OTG state to ", toOtg, 16, 0);
        return;
    } /*  end switch on toOTG */

    MGC_DIAG2(2, pPort->pController, "OTG state change from ", pPort->bOtgState, " to ", toOtg, 10,
              0);
    /* make permanent the state change */
    pPort->bOtgState = toOtg;
#ifdef MUSB_OTG_FUNC

    MGC_CompleteOtgTransition(pPort);
#endif

    UNUSED(bRet);
} /* MGC_DrcChangeOtgState */

/*
*  Notify applications
*  and programs the DRC to allow SUSPEND if we're a peripheral.
*/
#ifdef MUSB_OTG_FUNC

void MGC_CompleteOtgTransition(MGC_Port *pPort)
{
    MUSB_OtgClient *pOtgClient;

    MUSB_ASSERT(pPort);

    pOtgClient = (MUSB_OtgClient *)pPort->pOtgClient;			

    if (MGC_AB_MODE(pPort->bOtgState) == (uint8_t)MGC_AB_PERIPHERAL)
    {
        MGC_StartPeripheralMode(pPort);
    }

    //  check pfOtgState is null before call it.
    if ((pOtgClient) && (pOtgClient->pfOtgState))
    {
        pOtgClient->pfOtgState(pOtgClient->pPrivateData, (MUSB_BusHandle)pPort,
                               (MUSB_OtgState)pPort->bOtgState);
    }
}
#endif

/** Timer callback to turn off resume */

static void MGC_DrcCompleteResume(void *pParam, uint16_t wTimer)
{
    uint16_t wCount;

    uint16_t wIndex;
    MUSB_Device *pDevice;
    MGC_Device *pImplDevice;
    MUSB_DeviceDriver *pDriver;
    MGC_Controller *pController;
    MGC_Port *pPort;

    MUSB_ASSERT(pParam);
    pController = (MGC_Controller *)pParam;
    MUSB_ASSERT(pController->pPort);
    pPort = pController->pPort;				

    pPort->bWantResume = FALSE;
    pPort->pfProgramBusState(pPort);

    /* inform drivers */
    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));

    for (wIndex = 0; wIndex < wCount; wIndex++)
    {
        pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), wIndex);
        if (pDevice)
        {
            pImplDevice = pDevice->pPrivateData;
            if (pImplDevice)
            {
                pDriver = pImplDevice->pDriver;
                if (pDriver->pfBusSuspended)
                {
                    pDriver->pfBusResumed(pDriver->pPrivateData, (MUSB_BusHandle)pPort);
                }
            }
        }
    }
    UNUSED(wTimer);        
}

/*
* Interrupt Service Routine to record USB "global" interrupts.
* Since these do not happen often and signify things of paramount importance, 
* it seems OK to check them individually
*/

uint8_t MGC_DrcUsbIsr(MGC_Port *pPort, uint8_t bIntrUsbVal)
{
    MGC_BsrItem qItem;
#if defined(MUSB_SCHEDULER) && defined(MUSB_ISO)

    uint32_t dwEndCount;
    uint32_t dwEndIndex;
    MGC_EndpointResource *pEnd;
    MUSB_IsochIrp *pIsochIrp;
    uint32_t u4Ret;
#endif

    uint8_t bResult = 0;
    MUSB_SystemServices *pServices;

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pPort->pController);
    MUSB_ASSERT(pPort->pController->pSystemServices);

    pServices = pPort->pController->pSystemServices;

    if (bIntrUsbVal)
    {
        if (bIntrUsbVal & MGC_M_INTR_RESUME)
        {
            bResult++;

            qItem.bCause = MGC_BSR_CAUSE_RESUME;
            VERIFY(pServices->pfQueueBackgroundItem(pServices->pPrivateData, &qItem));
        }

#ifdef MUSB_OTG_FUNC
        if (bIntrUsbVal & MGC_M_INTR_SESSREQ)
        {
            bResult++;
            qItem.bCause = MGC_BSR_CAUSE_SESSION_REQ;
            VERIFY(pServices->pfQueueBackgroundItem(pServices->pPrivateData, &qItem));
        }

        if (bIntrUsbVal & MGC_M_INTR_VBUSERROR)
        {        
            bResult++;
            qItem.bCause = MGC_BSR_CAUSE_VBUS_ERROR;
            VERIFY(pServices->pfQueueBackgroundItem(pServices->pPrivateData, &qItem));
        }
#endif

        if (bIntrUsbVal & MGC_M_INTR_SUSPEND)
        {
            //  Set device remove when receiving Suspend from host.
#ifdef MUSB_FUNCTION

            if (!pPort->bIsHost)
            {
                pPort->bInsert = FALSE;
            }
#endif

            bResult++;
            qItem.bCause = MGC_BSR_CAUSE_SUSPEND;
            VERIFY(pServices->pfQueueBackgroundItem(pServices->pPrivateData, &qItem));
        }

        if (bIntrUsbVal & MGC_M_INTR_CONNECT)
        {
            bResult++;
            qItem.bCause = MGC_BSR_CAUSE_CONNECT;
            VERIFY(pServices->pfQueueBackgroundItem(pServices->pPrivateData, &qItem));
        }

        if (bIntrUsbVal & MGC_M_INTR_DISCONNECT)
        {
            /* only handle disconnect if we are not resetting */
            if (!pPort->bWantReset)
            {
                pPort->bWantHighSpeed = MGC_bWantHighSpeed;
                bResult++;
                qItem.bCause = MGC_BSR_CAUSE_DISCONNECT;
                VERIFY(pServices->pfQueueBackgroundItem(pServices->pPrivateData, &qItem));
            }
        }

        if (bIntrUsbVal & MGC_M_INTR_RESET)
        {
            if (pPort->bIsHost)
            {
                LOG(1, "Babble !!!\n", NULL);
                pPort->bWantHighSpeed = MGC_bWantHighSpeed;                
                pPort->bInsert = FALSE;
                /* host: safe to process in the background */
                bResult++;
                qItem.bCause = MGC_BSR_CAUSE_RESET;
                VERIFY(pServices->pfQueueBackgroundItem(pServices->pPrivateData, &qItem));
            }
#ifdef MUSB_FUNCTION

            else
            {
                VERIFY(0 == pPort->pfReadBusState(pPort));

                /* peripheral: process now because next interrupt could be soon */
                /* update our state per hardware */
                pPort->bWantSession = pPort->bIsSession;
#ifdef MUSB_OTG_FUNC               
                pPort->bWantHost = FALSE;
#endif
                pPort->bFuncAddr = 0;
                pPort->wSetupDataIndex = 0;
                pPort->wSetupDataSize = 0;
                MGC_DrcFlushAll(pPort);
                MGC_FunctionChangeState(pPort, MUSB_DEFAULT);
                MGC_FunctionSpeedSet(pPort);
                pPort->bInsert = TRUE;

#ifdef MUSB_OTG_FUNC               
                switch (pPort->bOtgState)
                {
                case MUSB_AB_IDLE:
                    MGC_OtgStateGetId(pPort, TRUE);
                    break;

                case MUSB_B_SRP_INIT:
                    MGC_DrcChangeOtgState((uint8_t)MUSB_B_PERIPHERAL, pPort);
                    break;
                    
                default:
                    break;                    
                }
#endif /* #ifdef MUSB_OTG_FUNC */

            }
#endif

        }
#if defined(MUSB_SCHEDULER)
        if (bIntrUsbVal & MGC_M_INTR_SOF)
        {
            /* update frame number */
            VERIFY(0 == pPort->pfReadBusState(pPort));
#ifdef MUSB_ISO
            /* start any waiting isochronous transmits whose wait has expired */

            dwEndCount = MUSB_ArrayLength(&(pPort->LocalEnds));

            for (dwEndIndex = 1; dwEndIndex < dwEndCount; dwEndIndex++)
            {
                pEnd = (MGC_EndpointResource *)MUSB_ArrayFetch(&(pPort->LocalEnds), dwEndIndex);
                if (pEnd && (MUSB_ENDPOINT_XFER_ISOC == pEnd->bTrafficType)
                        && pEnd->dwWaitFrameCount)
                {
                    pIsochIrp = (MUSB_IsochIrp *)pEnd->pTxIrp;
                    if (0 == --pEnd->dwWaitFrameCount)
                    {
                        u4Ret = pPort->pfProgramStartTransmit(
                            pPort, pEnd, pIsochIrp->pBuffer, pIsochIrp->dwLength, pIsochIrp);
                        UNUSED(u4Ret);    
                    }
                }
            }
#endif
            /* if host, prepare any required transfers */

            if (pPort->bIsHost)
            {
                MGC_RunScheduledTransfers(pPort);
            }
        }
#endif

    }

    return bResult;
}

/*
* Finish DRC interrupt processing
*/

void MGC_DrcBsr(void *pParam)
{
    MGC_BsrItem item;
    uint8_t bOk;
    uint8_t bTrafficType;
    MGC_EndpointResource *pLocalEnd;
    MUSB_Irp *pIrp;
    MUSB_ControlIrp *pControlIrp;
    MUSB_Port *pIfacePort;
    MGC_Port *pPort;
    MGC_Controller *pController;
    MUSB_SystemServices *pServices;
    uint8_t bRet;
    MUSB_HostClient* pHostClient;
#ifdef MUSB_ISO
    MUSB_IsochIrp *pIsochIrp;
#endif

    MUSB_ASSERT(pParam);
    pIfacePort = (MUSB_Port *)pParam;
    MUSB_ASSERT(pIfacePort->pPrivateData);
    pPort = (MGC_Port *)pIfacePort->pPrivateData;
    MUSB_ASSERT(pPort->pController);
    pController = (MGC_Controller *)pPort->pController;
    MUSB_ASSERT(pController->pSystemServices);
    pServices = pController->pSystemServices;

    bOk = pServices->pfDequeueBackgroundItem(pServices->pPrivateData, &item);

    while (bOk)
    {
        switch (item.bCause)
        {
        case MGC_BSR_CAUSE_RESUME:
            /* resume */
            MGC_DIAG(2, pController, "BSR: Resume Interrupt");
#ifdef MUSB_FUNCTION

            if (pPort->pFunctionClient)
            {
                pPort->pFunctionClient->pfUsbState(
                    pPort->pFunctionClient->pPrivateData, (MUSB_BusHandle)pPort,
                    (MUSB_State)pPort->bUsbState);
            }
#endif

            switch (pPort->bOtgState)
            {
#ifdef MUSB_OTG_FUNC

            case MUSB_B_WAIT_ACON:
                /* OTG 6.8.2.4 */
                MGC_DrcChangeOtgState((uint8_t)MUSB_B_PERIPHERAL, pPort);
                break;
#endif

            case MUSB_A_SUSPEND:
                /* OTG 6.8.1.5 */
                MGC_DrcChangeOtgState((uint8_t)MUSB_A_HOST, pPort);
                /* continue driving resume by HW for 20 ms, CPU must close it after 20ms later */
                bRet = pController->pSystemServices->pfArmTimer(
                    pController->pSystemServices->pPrivateData, 0, 20, FALSE,
                    MGC_DrcCompleteResume, pController);
                UNUSED(bRet);                    
                break;

            default:
                break;
            }
            break;
#ifdef MUSB_OTG_FUNC

        case MGC_BSR_CAUSE_SESSION_REQ:
            MGC_DIAG(2, pController, "BSR: Session Request Interrupt");
            /* qualify validity of SRP */
            if ((uint8_t)MUSB_AB_IDLE == pPort->bOtgState)
            {
                /* OTG 6.8.1.1 */
                MGC_DrcChangeOtgState((uint8_t)MUSB_A_WAIT_BCON, pPort);

                pPort->bWantSession = TRUE;
                pPort->pfProgramBusState(pPort);
            }
            break;

        case MGC_BSR_CAUSE_VBUS_ERROR:
            /* only occurs to A-Dev */
            MGC_DIAG(2, pController, "BSR: Vbus Error Interrupt");
            if (pPort->bVbusErrorRetries)
            {
                pPort->bVbusErrorRetries--;

                MGC_DIAG(2, pController, "Retrying due to Vbus error");
                MGC_DrcChangeOtgState((uint8_t)MUSB_AB_IDLE, pPort);
                MGC_OtgStateGetId(pPort, FALSE);
            }
            else
            {
                MGC_DrcChangeOtgState((uint8_t)MUSB_AB_IDLE, pPort);
            }
            break;
#endif

        case MGC_BSR_CAUSE_SUSPEND:
            MGC_DIAG(2, pController, "BSR: Suspend Interrupt");
#ifdef MUSB_FUNCTION
            /* inform function client; things may change further, but that's OK */

            if (pPort->pFunctionClient)
            {
                pPort->pFunctionClient->pfUsbState(
                    pPort->pFunctionClient->pPrivateData, (MUSB_BusHandle)pPort, MUSB_SUSPENDED);

                //  execute MGC_DrcFlushAll() in peripheral mode when device disconnect.
                MGC_DrcFlushAll(pPort);
            }
#endif

            switch (pPort->bOtgState)
            {
            case MUSB_B_PERIPHERAL:
                pPort->bIsSuspend = TRUE;
                if (pPort->bIsHost)
                {
                    MGC_DrcChangeOtgState((uint8_t)MUSB_B_WAIT_ACON, pPort);
                }
                break;

            case MUSB_A_HOST:
                pPort->bIsSuspend = TRUE;
                MGC_DrcChangeOtgState((uint8_t)MUSB_A_SUSPEND, pPort);
                break;

            case MUSB_A_PERIPHERAL:
                MGC_DrcChangeOtgState((uint8_t)MUSB_A_WAIT_BCON, pPort);
                break;

            default:
                break;                
            }
            break;

        case MGC_BSR_CAUSE_CONNECT:
            LOG(5, "BSR: Connect Interrupt.\n", NULL);
            VERIFY(0 == pPort->pfReadBusState(pPort));

            pPort->pRootDevice = NULL;
            if (((uint8_t)MUSB_B_WAIT_ACON == pPort->bOtgState) || ((uint8_t)MUSB_B_PERIPHERAL
                                                                        == pPort->bOtgState))
            {
                MGC_DrcChangeOtgState((uint8_t)MUSB_B_HOST, pPort);
            }
            else
            {
                MGC_DrcChangeOtgState((uint8_t)MUSB_A_HOST, pPort);
            }
            break;

        case MGC_BSR_CAUSE_DISCONNECT:
            LOG(5, "BSR: Disconnect Interrupt.\n", NULL);

            //  move to BSR task, prevent lock semphore.	 
#ifdef MUSB_FUNCTION

            if (!pPort->bIsHost)
            {
                //MGC_DrcFlushAll(pPort);
                MGC_FunctionChangeState(pPort, MUSB_DEFAULT);
            }
#endif

            pPort->pfResetPort(pPort);
            pPort->bWantSession = TRUE;
            pPort->pfProgramBusState(pPort);
            pPort->bOtgState = (uint8_t)MUSB_A_HOST;

            //report error device removed.
            pHostClient = pPort->pHostClient;
            if ((pHostClient) && (pHostClient->pfHostError) && (pHostClient->dwHostError > 0))
            {
                pHostClient->dwHostError = 0;
                pHostClient->pfHostError(pHostClient->pPrivateData, 
                    pPort, (uint32_t)MUSB_STATUS_UNSUPPORTED_REMOVE);
            }

            switch (pPort->bOtgState)
            {
            case MUSB_A_HOST:
                //MUSB_DeviceDisconnected(pPort->pRootDevice);
                MGC_DrcChangeOtgState((uint8_t)MUSB_A_WAIT_BCON, pPort);
                break;

            case MUSB_A_SUSPEND:
#ifdef MUSB_OTG_FUNC

                if (pPort->bRelinquish && (MUSB_STATUS_OK == MGC_SetHnpEnableIrp.dwStatus))
                {
                    MUSB_DeviceDisconnected(pPort->pRootDevice);
                    /* OTG 6.8.1.5 */
                    MGC_DrcChangeOtgState((uint8_t)MUSB_A_PERIPHERAL, pPort);
                }
                else
                {
                    MGC_DrcChangeOtgState((uint8_t)MUSB_A_WAIT_BCON, pPort);
                }
#else

                MGC_DrcChangeOtgState((uint8_t)MUSB_A_WAIT_BCON, pPort);
#endif

                break;

            case MUSB_B_HOST:
                MUSB_DeviceDisconnected(pPort->pRootDevice);
                MGC_DrcChangeOtgState((uint8_t)MUSB_B_PERIPHERAL, pPort);
                break;

            default:
                break;                
            }
            pPort->pRootDevice = NULL;
            break;

        case MGC_BSR_CAUSE_RESET:
            MGC_DIAG(2, pController, "BSR: Reset Interrupt");
            if (pPort->bIsHost)
            {
                /* session is blown */
                MGC_DIAG(1, pController, "Reset Port due to babble");

                pPort->pfResetPort(pPort);
                pPort->bWantSession = TRUE;
                pPort->pfProgramBusState(pPort);
                pPort->bOtgState = (uint8_t)MUSB_A_WAIT_BCON;
                MGC_DrcChangeOtgState((uint8_t)MUSB_A_WAIT_BCON, pPort);                
            }
            break;

        case MGC_BSR_CAUSE_IRP_COMPLETE:
        case MGC_BSR_CAUSE_RX_IRP_COMPLETE:
            pLocalEnd = (MGC_EndpointResource *)
                            MUSB_ArrayFetch(&(pPort->LocalEnds), item.bLocalEnd);
            if (pLocalEnd)
            {
                bTrafficType = (MGC_BSR_CAUSE_IRP_COMPLETE == item.bCause)
                                   ? pLocalEnd->bTrafficType : pLocalEnd->bRxTrafficType;

                switch (bTrafficType)
                {
                case MUSB_ENDPOINT_XFER_BULK:
                case MUSB_ENDPOINT_XFER_INT:
                    pIrp = (MUSB_Irp *)item.pData;
                    if (pIrp && pIrp->pfIrpComplete)
                    {
                        pIrp->pfIrpComplete(pIrp->pCompleteParam, pIrp);
                    }
                    break;

                case MUSB_ENDPOINT_XFER_CONTROL:
                    pControlIrp = (MUSB_ControlIrp *)item.pData;
                    if (pControlIrp && pControlIrp->pfIrpComplete)
                    {
                        pControlIrp->pfIrpComplete(pControlIrp->pCompleteParam, pControlIrp);
                    }
                    break;
#ifdef MUSB_ISO

                case MUSB_ENDPOINT_XFER_ISOC:
                    pIsochIrp = (MUSB_IsochIrp *)item.pData;
                    if (pIsochIrp && pIsochIrp->pfIrpComplete)
                    {
                        pIsochIrp->pfIrpComplete(pIsochIrp->pCompleteParam, pIsochIrp);
                    }
                    break;
#endif

                default:
                    break;
                }
                /* start next IRP if not started already */
                if (MGC_BSR_CAUSE_IRP_COMPLETE == item.bCause)
                {
                    if (!pLocalEnd->pTxIrp && !pLocalEnd->bStopTx)
                    {
                        MGC_StartNextIrp(pPort, pLocalEnd, TRUE);
                    }
                }
                else
                {
                    if ((!pLocalEnd->pRxIrp
                              || (MUSB_ENDPOINT_XFER_INT == pLocalEnd->bRxTrafficType))
                            && !pLocalEnd->bIsRxHalted)
                    {
                        MGC_StartNextIrp(pPort, pLocalEnd, FALSE);
                    }
                }
            }
            break;
            
        default:
            break;            
        }

        /* get next item to process */
        bOk = pServices->pfDequeueBackgroundItem(pServices->pPrivateData, &item);
    }
}

/*
 * Service the default endpoint as host.
 * Returns: TRUE if more packets are required for this transaction
 */
uint8_t MGC_DrcServiceHostDefault(MGC_Port *pPort, uint16_t wCount, uint8_t bStatus,
                                  uint16_t *pwCsrVal)
{
    uint8_t bMore = FALSE;

    uint8_t *pFifoDest = NULL;
    uint16_t wFifoCount = 0;
    MGC_EndpointResource *pEnd;
    MUSB_ControlIrp *pIrp;
    MUSB_DeviceRequest *pRequest;

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pwCsrVal);
    
    pEnd = MUSB_ArrayFetch(&(pPort->LocalEnds), 0);
    if (!pEnd)
    {
        /* prevent caller from touching CSR0 */
        *pwCsrVal = 0;
        return TRUE;
    }
    pIrp = (MUSB_ControlIrp *)pEnd->pTxIrp;
    if (!pIrp)
    {
        /* prevent caller from touching CSR0 */
        *pwCsrVal = 0;
        return TRUE;
    }
    pRequest = (MUSB_DeviceRequest *)((void *)pIrp->pOutBuffer);
    if (!pRequest)
    {
        /* prevent caller from touching CSR0 */
        *pwCsrVal = 0;
        return TRUE;
    }

    /* any error aborts */
    if (bStatus)
    {
        pIrp->dwStatus = bStatus;
        return FALSE;
    }

    if (!pEnd->bIsTx)
    {
        /* we are receiving from peripheral */
        pFifoDest = pIrp->pInBuffer + pIrp->dwActualInLength;

        wFifoCount = (uint16_t)MUSB_MIN(wCount, pIrp->dwInLength - pIrp->dwActualInLength);
#ifdef MUSB_DMA

        if (pEnd->pDmaChannel && pPort->pDmaController->pfDmaProgramChannel(
                                     pEnd->pDmaChannel, pEnd->wPacketSize,
                                     pEnd->pDmaChannel->bDesiredMode, pFifoDest,
                                     MGC_MIN(wFifoCount, pEnd->pDmaChannel->dwMaxLength)))
        {
            /* prevent caller from touching CSR0 */
            *pwCsrVal = 0;
            return TRUE;
        }
#endif
        /* unload FIFO */

        pPort->pfUnloadFifo(pPort, 0, wFifoCount, pFifoDest);
        pIrp->dwActualInLength += wFifoCount;
        if ((pIrp->dwActualInLength < pIrp->dwInLength) && (wCount == pEnd->wPacketSize))
        {
            bMore = TRUE;
        }
    }
    else
    {
        /* we are sending to peripheral */
        pIrp->dwActualOutLength += pEnd->dwTxSize;

        pEnd->dwTxSize = 0;
        if (pRequest->bmRequestType & MUSB_DIR_IN)
        {
            /* this means we just did setup; switch to IN */
            pEnd->bIsTx = FALSE;
#ifdef MUSB_DMA

            if (pEnd->pDmaChannel)
            {
                pPort->pDmaController->pfDmaReleaseChannel(pEnd->pDmaChannel);

                pEnd->pDmaChannel =
                    pPort->pDmaController->pfDmaAllocateChannel(pPort->pDmaController->pPrivateData,
                                                                0,
                                                                FALSE,
                                                                MUSB_ENDPOINT_XFER_CONTROL,
                                                                pEnd->wPacketSize);
                if (pEnd->pDmaChannel)
                {
                    pEnd->pDmaChannel->dwActualLength = 0L;
                    if (!pPort->pDmaController->pfDmaProgramChannel(
                             pEnd->pDmaChannel, pEnd->wPacketSize,
                             pEnd->pDmaChannel->bDesiredMode, pIrp->pInBuffer,
                             MGC_MIN(pIrp->dwInLength, pEnd->pDmaChannel->dwMaxLength)))
                    {
                        pPort->pDmaController->pfDmaReleaseChannel(pEnd->pDmaChannel);
                    }
                }
            }
#endif

            bMore = TRUE;
        }
        else
        {
            pFifoDest = (uint8_t *)(pIrp->pOutBuffer + pIrp->dwActualOutLength);

            wFifoCount = (uint16_t)MUSB_MIN(pEnd->wPacketSize,
                                            pIrp->dwOutLength - pIrp->dwActualOutLength);
#ifdef MUSB_DMA

            if (pEnd->pDmaChannel && pPort->pDmaController->pfDmaProgramChannel(
                                         pEnd->pDmaChannel, pEnd->wPacketSize,
                                         pEnd->pDmaChannel->bDesiredMode, pFifoDest,
                                         MGC_MIN(wFifoCount, pEnd->pDmaChannel->dwMaxLength)))
            {
                /* prevent caller from touching CSR0 */
                *pwCsrVal = 0;
                return TRUE;
            }
#endif
            /* load FIFO and signal need to transmit */

            if (wFifoCount)
            {
                pPort->pfLoadFifo(pPort, 0, wFifoCount, pFifoDest);

                pEnd->dwTxSize = wFifoCount;
                bMore = TRUE;
            }
        }
    }

    if (bMore)
    {
        *pwCsrVal = (uint16_t)(pEnd->bIsTx ? MGC_M_CSR0_TXPKTRDY : MGC_M_CSR0_H_REQPKT);
    }
    else
    {
        *pwCsrVal = 0;
    }

    return bMore;
}

/*
 * Check if device is acceptable; take appropriate action if not.
 * The enumerator calls this immediately BEFORE configuring a peripheral.
 * Returns: TRUE if device is acceptable; FALSE if not (and appropriate action taken)
 */
uint8_t MGC_DrcAcceptDevice(MGC_Port *pPort, MUSB_Device *pDevice, const MUSB_DeviceDriver *pDriver)
{
    uint8_t bOk = TRUE;
#ifdef MUSB_OTG_FUNC

    const MUSB_OtgDescriptor *pOtgDesc;
    MUSB_OtgClient *pOtgClient;
    uint32_t dwStatus;
#endif
    MUSB_HostClient* pHostClient;

    MUSB_ASSERT(pPort);

#ifdef MUSB_OTG_FUNC
    pOtgClient = (MUSB_OtgClient *)pPort->pOtgClient;    
#endif

    pHostClient = (MUSB_HostClient *)pPort->pHostClient;
    
    /* handle test device */
    MUSB_ASSERT(pDevice);

    if ((6666 == pDevice->DeviceDescriptor.idVendor)
            && (0xbadd == pDevice->DeviceDescriptor.idProduct))
    {
#ifdef MUSB_OTG_FUNC
        /* inform client */
        if (pOtgClient && pOtgClient->pfOtgError)
        {
            pOtgClient->pfOtgError(pOtgClient->pPrivateData, pPort, MUSB_STATUS_UNSUPPORTED_DEVICE);
        }
#endif

        if (pHostClient)
        {
            pHostClient->dwHostError = (uint32_t)MUSB_STATUS_UNSUPPORTED_DEVICE;
            if (pHostClient->pfHostError)
            {
                pHostClient->pfHostError(pHostClient->pPrivateData, pPort, (uint32_t)MUSB_STATUS_UNSUPPORTED_DEVICE);
            }
        }

        bOk = FALSE;
    }

    /* inform client in "hubs not supported" situation */
    if (!pPort->bIsMultipoint && (MUSB_CLASS_HUB == pDevice->DeviceDescriptor.bDeviceClass))
    {
#ifdef MUSB_OTG_FUNC
        if (pOtgClient && pOtgClient->pfOtgError)
        {
            pOtgClient->pfOtgError(pOtgClient->pPrivateData, pPort, MUSB_STATUS_UNSUPPORTED_HUB);
        }
#endif

        if (pHostClient)
        {
            pHostClient->dwHostError = (uint32_t)MUSB_STATUS_UNSUPPORTED_DEVICE;
            if (pHostClient->pfHostError)
            {
                pHostClient->pfHostError(pHostClient->pPrivateData, pPort, (uint32_t)MUSB_STATUS_UNSUPPORTED_HUB);
            }
        }

        bOk = FALSE;
    }

    /* inform client if no driver found and not the above situations */
    if (bOk && !pDriver)
    {
#ifdef MUSB_OTG_FUNC

        if (pOtgClient && pOtgClient->pfOtgError)
        {
            pOtgClient->pfOtgError(pOtgClient->pPrivateData, pPort, MUSB_STATUS_UNSUPPORTED_DEVICE);
        }
#endif

        if (pHostClient)
        {
            pHostClient->dwHostError = (uint32_t)MUSB_STATUS_UNSUPPORTED_DEVICE;
            if (pHostClient->pfHostError)
            {
                pHostClient->pfHostError(pHostClient->pPrivateData, pPort, (uint32_t)MUSB_STATUS_UNSUPPORTED_DEVICE);
            }
        }

        bOk = FALSE;
    }
#ifdef MUSB_OTG_FUNC

    if (bOk)
    {
        /* if we are A-device, set a_hnp_support if device supports HNP */
        if (!pPort->bConnectorId && pPort->pRootDevice)
        {
            pOtgDesc = (MUSB_OtgDescriptor *)((uint32_t)
                           MGC_FindDescriptor((uint8_t *)pPort->pRootDevice->pCurrentConfiguration,
                                              MUSB_SWAP16P((uint8_t *)&(
                                                                          pPort->pRootDevice->pCurrentConfiguration->wTotalLength)),
                                              MUSB_DT_OTG,
                                              0));
            if (pOtgDesc && (pOtgDesc->bmAttributes&MUSB_OTG_HNP))
            {
                MGC_SetHnpSupportIrp.pDevice = pPort->pRootDevice;

                MGC_SetHnpSupportIrp.pCompleteParam = pPort;
                dwStatus = MUSB_StartControlTransfer(pPort->pInterfacePort, &MGC_SetHnpSupportIrp);
                UNUSED(dwStatus);
            }
        }
    }
    else
    {
        MUSB_RelinquishHost((MUSB_BusHandle)pPort);
    }
#endif

    return bOk;
}

/**
 * bus reset root device.
 */
void MGC_DrcBusReset(MGC_Port *pPort)
{
    MUSB_SystemServices *pServices;
    uint8_t bRet;

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pPort->pController);
    pServices = pPort->pController->pSystemServices;

    /* kill scheduled event */
    bRet = pServices->pfCancelTimer(pServices->pPrivateData, 0);
    UNUSED(bRet);

    bRet =  pServices->pfArmTimer(pServices->pPrivateData, 0, 10, 
        FALSE, MGC_HostResetStart, pPort->pController);
    UNUSED(bRet);
}

/**
 * Enter the OTG state GetId
 */
#ifdef MUSB_OTG_FUNC

static void MGC_OtgStateGetId(MGC_Port *pPort, uint8_t bIsReset)
{  
    MUSB_ASSERT(pPort);
    
    pPort->bWantSuspend = FALSE;

    pPort->bWantSession = TRUE;

    MUSB_ASSERT(pPort->pfProgramBusState);    
    pPort->pfProgramBusState(pPort);

    MUSB_ASSERT(pPort->pfReadBusState);
    VERIFY(0 == pPort->pfReadBusState(pPort));

    LOG(5, "MGC_OtgStateGetId pPort->bConnectorId = %d.\n", pPort->bConnectorId);

    if (!pPort->bIsHost)
    {
        if (pPort->bConnectorId)
        {
            if (bIsReset)
            {
                MGC_DrcChangeOtgState((uint8_t)MUSB_B_PERIPHERAL, pPort);
            }
            else
            {
                MGC_DrcChangeOtgState((uint8_t)MUSB_B_SRP_INIT, pPort);
            }
        }
        else
        {
            /* as A-device, we always support HNP */
            if (pPort->pOtgClient)
            {
                pPort->bIsHnpSupported = TRUE;
            }

            MGC_DrcChangeOtgState((uint8_t)MUSB_A_WAIT_BCON, pPort);
        }
    }
}

/*
* Implementation
*/
MUSB_BusHandle MUSB_RegisterOtgClient(MUSB_Port *pPort, MUSB_FunctionClient *pFunctionClient,
                                      MUSB_HostClient *pHostClient, MUSB_OtgClient *pOtgClient)
{
    MUSB_OtgDescriptor *pOtgDesc = NULL;

    uint32_t dwStatus = 0;
    void *pResult = NULL;
    MGC_Port *pImplPort;

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pPort->pPrivateData);    

    pImplPort = (MGC_Port *)pPort->pPrivateData;

    /* session still active? */
    if (!pImplPort->bSessionActive)
    {
        /* host client just has callbacks */
        pImplPort->pHostClient = pHostClient;

        if (pFunctionClient)
        {
            /* function client requires verification */
            dwStatus = MGC_FunctionRegisterClient(pImplPort, pFunctionClient);
        }
        if (0 == dwStatus)
        {
            //  check if operation in host only mode.
            if (pOtgClient)
            {
                /* OK; scan for OTG descriptor and set flag for HNP capability */
                pImplPort->bIsHnpSupported = FALSE;

                if (pFunctionClient)
                {
                    pOtgDesc = (MUSB_OtgDescriptor *)((uint32_t)MGC_FindDescriptor(
                                                         pFunctionClient->pStandardDescriptors,
                                                         pFunctionClient->wDescriptorBufferLength,
                                                         MUSB_DT_OTG,
                                                         0));
                }
                if (pOtgDesc)
                {
                    pImplPort->bIsHnpSupported =
                        (pOtgDesc->bmAttributes&MUSB_OTG_HNP) ? TRUE : FALSE;
                }
            }

            pImplPort->bVbusErrorRetries = MUSB_VBUS_ERROR_RETRIES;
            //  disconnect and reset system directly when babble. 
            //	    pImplPort->bBabbleRetries = MUSB_BABBLE_RETRIES;

            /* start */
            pImplPort->pFunctionClient = pFunctionClient;
            pImplPort->bSessionActive = TRUE;
            pImplPort->pOtgClient = pOtgClient;

            MGC_OtgStateGetId(pImplPort, FALSE);

            /* set return value */
            pResult = pImplPort;
        }
    }

    return pResult;
}

/*
 * For OTG, stop session and relinquish host role to another
 */
void MUSB_RelinquishHost(MUSB_BusHandle hBus)
{
    MGC_Port *pPort;
    MGC_Controller *pController;
    uint8_t bRet;

    MUSB_ASSERT(hBus);
    pPort = (MGC_Port *)hBus;
    MUSB_ASSERT(pPort->pController);
    pController = (MGC_Controller *)pPort->pController;
    
    pPort->bRelinquish = TRUE;

    if (!pPort->bConnectorId)
    {
        MGC_DrcChangeOtgState((uint8_t)MUSB_A_SUSPEND, pPort);
    }
    else
    {
        /* suspend bus */
        VERIFY(0 == pPort->pfReadBusState(pPort));

        pPort->bWantSuspend = TRUE;
        pPort->pfProgramBusState(pPort);
        /* transition */
        MGC_DrcChangeOtgState((uint8_t)MUSB_B_PERIPHERAL, pPort);
        /* remove suspend later */
        bRet = pController->pSystemServices->pfArmTimer(
            pController->pSystemServices->pPrivateData, 0, 10, FALSE, MGC_RemoveSuspend, pController);
        UNUSED(bRet);
    }
}

uint8_t MUSB_RequestBus(MUSB_BusHandle hBus)
{
    MGC_Port *pPort;

    MUSB_ASSERT(hBus);
    pPort = (MGC_Port *)hBus;

    if (pPort->bOtgState == (uint8_t)MUSB_AB_IDLE)
    {
        MGC_OtgStateGetId(pPort, FALSE);
        return TRUE;
    }

    if (!pPort->bIsSession)
    {
        MGC_OtgStateGetId(pPort, FALSE);
        return TRUE;
    }

    return FALSE;
}

#endif /* #ifdef MUSB_OTG_FUNC */

