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
 * Generic USB transfer functionality.
 * $Revision: #1 $
 */

#include "mu_impl.h"

#include "mu_pippr.h"

/* Implementation */

const uint8_t *MGC_FindDescriptor(const uint8_t *pBuffer, uint16_t wBufferLength,
                                  uint8_t bDescriptorType, uint8_t bIndex)
{
    int16_t wFoundIndex = -1;

    uint16_t wOffset = 0;
    const MUSB_DescriptorHeader *pHeader;

    if (!pBuffer)
    {
        return NULL;
    }
    
    pHeader = (MUSB_DescriptorHeader *)((void *)pBuffer);
    
    /* handle trivial case */
    if ((pHeader->bDescriptorType == bDescriptorType) && !bIndex)
    {
        return (uint8_t *)pHeader;
    }

    /* general case */
    while ((wOffset < wBufferLength) && (wFoundIndex < (int16_t)bIndex))
    {
        pHeader = (MUSB_DescriptorHeader *)((void *)((uint8_t *)pBuffer + wOffset));

        MUSB_ASSERT(pHeader);

        if (pHeader->bLength < 1)
        {
            /* we're in the weeds */
            return NULL;
        }

        wOffset += pHeader->bLength;
        if (pHeader->bDescriptorType == bDescriptorType)
        {
            wFoundIndex++;
            if (wFoundIndex == (uint16_t)bIndex)
            {
                return (uint8_t *)pHeader;
            }
        }
    }

    if (wOffset < wBufferLength)
    {
        return (uint8_t *)pHeader;
    }

    return NULL;
}

/*
 * Service a received packet; returning indication of transfer complete.
 */
uint8_t MGC_PipePacketReceived(MGC_Port *pPort, MGC_EndpointResource *pEnd,
                               uint16_t wPacketSize, uint8_t bContinue, void *pCurrentIrp)
{
    uint8_t *pBuffer = NULL;
    uint16_t wUnloadCount;

#ifdef MUSB_ISO
#ifdef MUSB_ISO_STREAM
    MUSB_IsoPtr *prIsoPtr;
#else
    uint16_t wFrameIndex;
#endif
    MUSB_IsochIrp *pIsochIrp = NULL;
#endif

    uint8_t bAllowDma = FALSE;
    uint32_t dwReqLength = 0;
    MUSB_Irp *pIrp = NULL;
    uint8_t bComplete = FALSE;
    uint32_t u4Ret;

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pEnd);

    wUnloadCount = wPacketSize;

    switch (pEnd->bRxTrafficType)
    {
    case MUSB_ENDPOINT_XFER_BULK:
    case MUSB_ENDPOINT_XFER_INT:
        pIrp = (MUSB_Irp *)pCurrentIrp;
        MUSB_ASSERT(pIrp);
        bAllowDma = pIrp->bAllowDma;
        dwReqLength = pIrp->dwLength;
        pBuffer = pIrp->pBuffer + pEnd->dwRxOffset;
        if (wPacketSize < pEnd->wRxPacketSize)
        {
            /* short packet means transfer complete */
            bComplete = TRUE;

            /* but, the other side may send a 0-byte packet */
            if (pIrp->bAllowShortTransfer && (0 == wPacketSize))
            {
                bComplete = FALSE;
            }            
        }
        else if ((wPacketSize + pEnd->dwRxOffset) >= dwReqLength)
        {
            /* received as much data as requested means transfer complete */
            bComplete = TRUE;
            /* but, the other side may send a 0-byte packet */
            if (pIrp->bAllowShortTransfer && (0 == (wPacketSize %pEnd->wRxPacketSize)))
            {
                bComplete = FALSE;
            }
            wUnloadCount = MGC_MIN(wPacketSize, (uint16_t)(dwReqLength - pEnd->dwRxOffset));
        }
        dwReqLength -= pEnd->dwRxOffset;
        break;

#ifdef MUSB_ISO

    case MUSB_ENDPOINT_XFER_ISOC:
        pIsochIrp = (MUSB_IsochIrp *)pCurrentIrp;
        MUSB_ASSERT(pIsochIrp);
        bAllowDma = pIsochIrp->bAllowDma;
#ifdef MUSB_ISO_STREAM
        prIsoPtr = pIsochIrp->prIsoPtr;

        LOG(7, "ISO Rx: %d, %d, %d, %d, %d.\n", prIsoPtr->dwWp, 
            prIsoPtr->dwWEp, prIsoPtr->dwRp, pIsochIrp->dwBufSize, pIsochIrp->dwLength);

        if (prIsoPtr->dwWp >= prIsoPtr->dwRp)
        {
            // Full condition - 1.
            if ((pIsochIrp->dwBufSize - (prIsoPtr->dwWp - prIsoPtr->dwRp)) 
                  < pIsochIrp->dwLength)
            {
                LOG(1, "ISO Buf full (1) %d, %d, %d, %d, %d.\n", prIsoPtr->dwWp, 
                    prIsoPtr->dwWEp, prIsoPtr->dwRp, pIsochIrp->dwBufSize, pIsochIrp->dwLength);
                bComplete = TRUE;
            }
            // Full condition - 2.
            else if ((prIsoPtr->dwWp == prIsoPtr->dwRp) && (!prIsoPtr->bEmpty))
            {
                LOG(1, "ISO Buf full (2) %d, %d, %d, %d, %d.\n", prIsoPtr->dwWp, 
                    prIsoPtr->dwWEp, prIsoPtr->dwRp, pIsochIrp->dwBufSize, pIsochIrp->dwLength);
                bComplete = TRUE;
            }
            else
            {
                prIsoPtr->bEmpty = FALSE;            
                MUSB_ASSERT((pIsochIrp->dwBufSize - prIsoPtr->dwWp) 
                    >= pIsochIrp->dwLength);
                //MUSB_ASSERT(wUnloadCount <= pIsochIrp->dwLength);
                wUnloadCount = MUSB_MIN(wUnloadCount, pIsochIrp->dwLength);

                pBuffer = pIsochIrp->pBuffer + prIsoPtr->dwWp;
                prIsoPtr->dwWp += wUnloadCount;
                MUSB_ASSERT(pIsochIrp->dwBufSize > prIsoPtr->dwWp);
                if ((pIsochIrp->dwBufSize - prIsoPtr->dwWp) < pIsochIrp->dwLength)
                {
                    prIsoPtr->dwWEp = prIsoPtr->dwWp;
                    prIsoPtr->dwWp = 0;
                }
            }            
        }
        else
        {
            // Full condition - 3.
            if ((prIsoPtr->dwRp - prIsoPtr->dwWp) < pIsochIrp->dwLength)
            {
                LOG(1, "ISO Buf full (3) %d, %d, %d, %d, %d.\n", prIsoPtr->dwWp, 
                    prIsoPtr->dwWEp, prIsoPtr->dwRp, pIsochIrp->dwBufSize, pIsochIrp->dwLength);
                bComplete = TRUE;
            }
            else
            {
                prIsoPtr->bEmpty = FALSE;                        
                pBuffer = pIsochIrp->pBuffer + prIsoPtr->dwWp;
                MUSB_ASSERT((prIsoPtr->dwRp - prIsoPtr->dwWp) >= wUnloadCount);
                prIsoPtr->dwWp += wUnloadCount;
                MUSB_ASSERT(pIsochIrp->dwBufSize > prIsoPtr->dwWp);
                MUSB_ASSERT(prIsoPtr->dwRp >= prIsoPtr->dwWp);
            }
        }       
#else
        wFrameIndex = pIsochIrp->wCurrentFrame;
        pIsochIrp->prISOpacket[wFrameIndex].dwActualDataLength = wPacketSize;
        pIsochIrp->prISOpacket[wFrameIndex].dwStatus = 0;       
        pIsochIrp->wCurrentFrame++;
        dwReqLength = pIsochIrp->prISOpacket[wFrameIndex + 1].dwDataLength;
        pBuffer = pIsochIrp->pBuffer + pEnd->dwRxOffset;
        /* complete when frame count is reached */
        bComplete = (pIsochIrp->wCurrentFrame >= pIsochIrp->wFrameCount) ? TRUE : FALSE;
        /* pretend to be complete on callback interval (CompleteIrp won't retire it in this case) */
        if (pIsochIrp->wCallbackInterval
                && (0 == (pIsochIrp->wCurrentFrame %pIsochIrp->wCallbackInterval)))
        {
            bComplete = TRUE;
        }
#endif /* #ifdef MUSB_ISO_STREAM */

        break;
#endif

    default:
        pBuffer = NULL;
        wUnloadCount = 0;
        bComplete = TRUE;
        MGC_DIAG1(1, pPort->pController, "Internal error on end ", pEnd->bLocalEnd, 10, 0);
    }
    /* unload FIFO (if anything the lower layer didn't unload) */
    if ((wUnloadCount > 0) && (pBuffer))
    {
        pPort->pfUnloadFifo(pPort, pEnd->bLocalEnd, wUnloadCount, pBuffer);
        pEnd->dwRxOffset += wUnloadCount;
        pBuffer += wUnloadCount;
    }

    if (!bComplete && bContinue)
    {
        u4Ret = pPort->pfProgramStartReceive(pPort, pEnd, pBuffer, dwReqLength, pCurrentIrp, bAllowDma);
        UNUSED(u4Ret);
    }

    return bComplete;
}

/*
 * Service a transmit buffer-ready; returning indication of transfer complete.
 */
uint8_t MGC_PipeTransmitReady(MGC_Port *pPort, MGC_EndpointResource *pEnd)
{

#ifdef MUSB_ISO
#ifdef MUSB_ISO_STREAM
    MUSB_IsoPtr *prIsoPtr;
#else
    uint16_t wFrameIndex;
#endif
    MUSB_IsochIrp *pIsochIrp;
#endif

    uint32_t dwReqLength = 0;
    MUSB_Irp *pIrp;
    void *pGenIrp = NULL;
    uint8_t *pBuffer = NULL;
    uint8_t bComplete = FALSE;
    uint32_t u4Ret = 0;
    
    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pEnd);
    
    if (pEnd->pTxIrp)
    {
        switch (pEnd->bTrafficType)
        {
        case MUSB_ENDPOINT_XFER_BULK:
        case MUSB_ENDPOINT_XFER_INT:
            pIrp = (MUSB_Irp *)pEnd->pTxIrp;
            pGenIrp = pIrp;
            dwReqLength = pIrp->dwLength;
            if (pEnd->dwTxOffset >= dwReqLength)
            {
                //  MTK Notice: Max Liao, 2006/12/21.
                //  send 0-byte packet only when total packet size is the multiple of wPacketSize.
                /* send 0-byte packet if desired & needed */
                if (pIrp->bAllowShortTransfer && (0 == (pEnd->dwTxOffset % pEnd->wPacketSize)))
                {
                    if (!pEnd->bIsTxNullPkt)
                    {
                        u4Ret = pPort->pfProgramStartTransmit(pPort, pEnd, pBuffer, 0, pIrp);

                        pEnd->bIsTxNullPkt = TRUE;
                        return FALSE;
                    }
                    else
                    {
                        pEnd->bIsTxNullPkt = FALSE;
                    }
                }
                /* IRP complete */
                bComplete = TRUE;
            }
            else
            {
                //  MTK Notice: Max Liao, 2006/12/20.
                //  update buffer pointer only when IRP not complete.	    
                /* update buffer pointer */
                dwReqLength -= pEnd->dwTxOffset;
                pBuffer = pIrp->pBuffer + pEnd->dwTxOffset;
            }
            break;

#ifdef MUSB_ISO

        case MUSB_ENDPOINT_XFER_ISOC:
            pIsochIrp = (MUSB_IsochIrp *)pEnd->pTxIrp;
            pGenIrp = pIsochIrp;

#ifdef MUSB_ISO_STREAM
            prIsoPtr = pIsochIrp->prIsoPtr;

            LOG(7, "ISO Tx: %d, %d, %d, %d, %d.\n", prIsoPtr->dwWp, 
                prIsoPtr->dwWEp, prIsoPtr->dwRp, pIsochIrp->dwBufSize, pIsochIrp->dwLength);

            if (prIsoPtr->dwWp < prIsoPtr->dwRp)
            {
                MUSB_ASSERT(prIsoPtr->dwWEp > prIsoPtr->dwRp);
                dwReqLength = MUSB_MIN(pIsochIrp->dwLength, 
                    (prIsoPtr->dwWEp - prIsoPtr->dwRp));
                pBuffer = pIsochIrp->pBuffer + prIsoPtr->dwRp;                    
                prIsoPtr->dwRp += dwReqLength;
                MUSB_ASSERT(prIsoPtr->dwWEp >= prIsoPtr->dwRp);
                if (prIsoPtr->dwWEp == prIsoPtr->dwRp)
                {
                    prIsoPtr->dwRp = 0;
                    if (prIsoPtr->dwRp == prIsoPtr->dwWp)
                    {
                        prIsoPtr->bEmpty = TRUE;
                    }
                }
            }
            else
            {
                if (prIsoPtr->dwRp == prIsoPtr->dwWp)
                {
                    if (prIsoPtr->bEmpty)
                    {
                        // Empty condition - 1.
                        LOG(1, "ISO Buf Empty %d, %d, %d, %d, %d.\n", prIsoPtr->dwWp, 
                            prIsoPtr->dwWEp, prIsoPtr->dwRp, pIsochIrp->dwBufSize, pIsochIrp->dwLength);
                        bComplete = TRUE;
                    }
                    else
                    {
                        // buffer full now.
                        dwReqLength = MUSB_MIN(pIsochIrp->dwLength, 
                            (prIsoPtr->dwWEp - prIsoPtr->dwRp));                
                        pBuffer = pIsochIrp->pBuffer + prIsoPtr->dwRp;
                        prIsoPtr->dwRp += dwReqLength;
                        MUSB_ASSERT(prIsoPtr->dwWEp >= prIsoPtr->dwRp);
                        MUSB_ASSERT(pIsochIrp->dwBufSize > prIsoPtr->dwRp);
                        if (prIsoPtr->dwRp == prIsoPtr->dwWp)
                        {
                            prIsoPtr->bEmpty = TRUE;
                        }                        
                    }
                }
                else
                {
                    dwReqLength = MUSB_MIN(pIsochIrp->dwLength, 
                        (prIsoPtr->dwWp - prIsoPtr->dwRp));                
                    pBuffer = pIsochIrp->pBuffer + prIsoPtr->dwRp;
                    prIsoPtr->dwRp += dwReqLength;  
                    MUSB_ASSERT(pIsochIrp->dwBufSize > prIsoPtr->dwRp);
                    MUSB_ASSERT(prIsoPtr->dwWp >= prIsoPtr->dwRp);    
                    if (prIsoPtr->dwRp == prIsoPtr->dwWp)
                    {
                        prIsoPtr->bEmpty = TRUE;
                    }                                            
                }
            }       

#else
            wFrameIndex = pIsochIrp->wCurrentFrame;
            pIsochIrp->prISOpacket[wFrameIndex].dwActualDataLength = 
                pIsochIrp->prISOpacket[wFrameIndex].dwDataLength;
            pIsochIrp->prISOpacket[wFrameIndex].dwStatus = 0;       
            dwReqLength = pIsochIrp->prISOpacket[wFrameIndex + 1].dwDataLength;
            pBuffer = pIsochIrp->pBuffer + pEnd->dwTxOffset;
            pIsochIrp->wCurrentFrame++;
            /* complete when frame count is reached */
            bComplete = (pIsochIrp->wCurrentFrame >= pIsochIrp->wFrameCount) ? TRUE : FALSE;
            /* pretend to be complete on callback interval (CompleteIrp won't retire it in this case) */
            if (pIsochIrp->wCallbackInterval
                    && (0 == (pIsochIrp->wCurrentFrame %pIsochIrp->wCallbackInterval)))
            {
                bComplete = TRUE;
            }
#endif /* #ifdef MUSB_ISO_STREAM */
            break;
#endif

        default:
            bComplete = TRUE;
            dwReqLength = 0;
            MGC_DIAG1(1, pPort->pController, "Internal error on end ", pEnd->bLocalEnd, 10, 0);
            MUSB_ASSERT(0);
        }
    }
    else
    {
        bComplete = TRUE;
    }

    if ((!bComplete) && (dwReqLength > 0) && (pGenIrp))
    {
        u4Ret = pPort->pfProgramStartTransmit(pPort, pEnd, pBuffer, dwReqLength, pGenIrp);
    }

    UNUSED(u4Ret);                     

    return bComplete;
}

/*
 * Generic part of completing an IRP
 */
uint8_t MGC_CompleteIrp(MGC_BsrItem *pItem, MGC_EndpointResource *pEnd, uint8_t bStatus,
                        uint8_t bIsTx, void *pCurrentIrp)
{
    uint8_t bTrafficType;

    MUSB_ControlIrp *pControlIrp;
    MUSB_Irp *pIrp;
    MGC_Pipe *pPipe;

#ifdef MUSB_ISO

    MUSB_IsochIrp *pIsochIrp;
#endif
    
    if (!pCurrentIrp)
    {
        return FALSE;
    }

    MUSB_ASSERT(pItem);
    MUSB_ASSERT(pEnd);

    if (bIsTx)
    {
        bTrafficType = pEnd->bTrafficType;
        pItem->bCause = MGC_BSR_CAUSE_IRP_COMPLETE;
    }
    else
    {
        bTrafficType = pEnd->bRxTrafficType;
        pItem->bCause = MGC_BSR_CAUSE_RX_IRP_COMPLETE;
    }

    /* if control/bulk or isoch limit, endpoint is done with this IRP */
    switch (bTrafficType)
    {
    case MUSB_ENDPOINT_XFER_INT:
        break;

#ifdef MUSB_ISO

    case MUSB_ENDPOINT_XFER_ISOC:
        pIsochIrp = (MUSB_IsochIrp *)pCurrentIrp;

#ifndef MUSB_ISO_STREAM
        if (pIsochIrp->wCurrentFrame >= pIsochIrp->wFrameCount)
#endif /* #ifdef MUSB_ISO_STREAM */
        {
            if (bIsTx)
            {
                pEnd->pTxIrp = NULL;
            }
            else
            {
                pEnd->pRxIrp = NULL;
            }
        }
        
        break;
#endif

    default:
        if (bIsTx)
        {
            pEnd->pTxIrp = NULL;
        }
        else
        {
            pEnd->pRxIrp = NULL;
        }
    }

    /* fill info for queue */
    pItem->bLocalEnd = pEnd->bLocalEnd;
    pItem->bStatus = bStatus;
    pItem->pData = pCurrentIrp;

    /* update IRP status/actual */
    switch (bTrafficType)
    {
    case MUSB_ENDPOINT_XFER_CONTROL:
        pControlIrp = (MUSB_ControlIrp *)pCurrentIrp;
        pControlIrp->dwStatus = bStatus;

        // Silent Reset: Use polling pControlIrp->dwStatus in usb task.
        if (!pControlIrp->pfIrpComplete)
        {
            return FALSE;
        }
        break;

    case MUSB_ENDPOINT_XFER_BULK:
    case MUSB_ENDPOINT_XFER_INT:
        pIrp = (MUSB_Irp *)pCurrentIrp;
        pIrp->dwStatus = bStatus;
        if (bIsTx)
        {
            pIrp->dwActualLength = pEnd->dwTxOffset;

            pEnd->dwTxOffset = 0L;
            pEnd->dwTxSize = 0L;
        }
        else
        {
            pIrp->dwActualLength = pEnd->dwRxOffset;
            pEnd->dwRxOffset = 0L;
        }        
        // Handle pfIrpComplete only when 
        // status = MUSB_STATUS_OK and dwLength = dwActualLength.        
        //   Handle abnormal case in BSR task, not in ISR.
        if ((pIrp->bIsrCallback) &&
            (bStatus == MUSB_STATUS_OK) && 
            (pIrp->dwLength == pIrp->dwActualLength))
        {
            pIrp->pfIrpComplete(pIrp->pCompleteParam, pIrp);
            /* start next IRP if not started already */
            pPipe = (MGC_Pipe *)pIrp->hPipe;

            if (bIsTx)
            {
                if (!pEnd->pTxIrp && !pEnd->bStopTx)
                {
                    MGC_StartNextIrp(pPipe->pPort, pEnd, TRUE);
                }
            }
            else
            {
                if (((MUSB_ENDPOINT_XFER_INT == bTrafficType) || !pEnd->pRxIrp)
                        && !pEnd->bIsRxHalted)
                {
                    MGC_StartNextIrp(pPipe->pPort, pEnd, FALSE);
                }
            }
            return FALSE;
        }
        break;

#ifdef MUSB_ISO

    case MUSB_ENDPOINT_XFER_ISOC:
        pIsochIrp = (MUSB_IsochIrp *)pCurrentIrp;

#ifndef MUSB_ISO_STREAM
        if (bStatus || (pIsochIrp->wCurrentFrame >= pIsochIrp->wFrameCount))
#endif /* #ifdef MUSB_ISO_STREAM */
        {
            if (bIsTx)
            {
                pEnd->dwTxOffset = 0L;
                pEnd->dwTxSize = 0L;
            }
            else
            {
                pEnd->dwRxOffset = 0L;
            }
        }
        
        if (pIsochIrp->bIsrCallback)
        {
            pIsochIrp->pfIrpComplete(pIsochIrp->pCompleteParam, pIsochIrp);
            /* start next IRP if not started already */
            pPipe = (MGC_Pipe *)pIsochIrp->hPipe;

            if (bIsTx)
            {
                if (!pEnd->pTxIrp && !pEnd->bStopTx)
                {
                    MGC_StartNextIrp(pPipe->pPort, pEnd, TRUE);
                }
            }
            else
            {
                if (!pEnd->pRxIrp && !pEnd->bIsRxHalted)
                {
                    MGC_StartNextIrp(pPipe->pPort, pEnd, FALSE);
                }
            }
            return FALSE;
        }
        break;
#endif

    default:
        if (bIsTx)
        {
            pEnd->dwTxOffset = 0L;
        }
        else
        {
            pEnd->dwRxOffset = 0L;
        }
    }

    return TRUE;
}

/*
 * Start the next IRP, if any, for the given end.
 * In function role, only identical traffic/direction can be queued.
 * In host role, bulk traffic can be re-targetted and direction changed.
 */
void MGC_StartNextIrp(MGC_Port *pPort, MGC_EndpointResource *pEnd, uint8_t bIsTx)
{
    uint8_t bTypeOk, bSwitchBulk;
    uint8_t bAllowDma = FALSE;
    uint8_t *pBuffer = NULL;
    uint32_t dwReqLength = 0;
    MUSB_Irp *pIrp = NULL;

#ifdef MUSB_ISO

    MUSB_IsochIrp *pIsochIrp = NULL;
#endif

    MGC_Pipe *pPipe = NULL;
    void *pNextIrp = NULL;
    uint8_t bTrafficType;
    uint32_t u4Ret = 0;
    
    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pEnd);

    bTrafficType = bIsTx ? pEnd->bTrafficType : pEnd->bRxTrafficType;

    //  MTK Notice: Max Liao, 2006/11/28.
    //  submit Hub interrupt IRP again to monitor Hub port change.
    if (bTrafficType == MUSB_ENDPOINT_XFER_INT)
    {
        pNextIrp = bIsTx ? pEnd->pTxIrp : pEnd->pRxIrp;
    }

    if (!pNextIrp)
    {
        if (bIsTx)
        {
            pNextIrp = MUSB_ListFindItem(&(pEnd->TxIrpList), 0);
            if (pNextIrp)
            {
                MUSB_ListRemoveItem(&(pEnd->TxIrpList), pNextIrp);
            }
        }
        else
        {
            pNextIrp = MUSB_ListFindItem(&(pEnd->RxIrpList), 0);
            if (pNextIrp)
            {
                MUSB_ListRemoveItem(&(pEnd->RxIrpList), pNextIrp);
            }
        }            
    }

    if (pNextIrp)
    {
        /* NOTE: relies on both IRP types having hPipe first! */
        pIrp = (MUSB_Irp *)pNextIrp;
        pPipe = (MGC_Pipe *)pIrp->hPipe;
#ifdef MUSB_ISO
        pIsochIrp = (MUSB_IsochIrp *)pNextIrp;
#endif
        /* now cast according to type */
        switch (bTrafficType)
        {
        case MUSB_ENDPOINT_XFER_BULK:
        case MUSB_ENDPOINT_XFER_INT:
            pIrp = (MUSB_Irp *)pNextIrp;
            bAllowDma = pIrp->bAllowDma;
            pPipe = (MGC_Pipe *)pIrp->hPipe;
            break;

#ifdef MUSB_ISO

        case MUSB_ENDPOINT_XFER_ISOC:
            pIsochIrp = (MUSB_IsochIrp *)pNextIrp;
            bAllowDma = pIsochIrp->bAllowDma;
            pPipe = (MGC_Pipe *)pIrp->hPipe;
            break;
#endif

        default:
            break;
        }

        if (!pPipe)
        {
            LOG(0, "pPipe = NULL.\n", NULL);
            return;
        }

        bTypeOk = (pPipe->bTrafficType == bTrafficType);
        bSwitchBulk = (pPort->bIsHost && bTypeOk
                           && (MUSB_ENDPOINT_XFER_BULK == pPipe->bTrafficType));
        if (pPipe && (bTypeOk || bSwitchBulk))
        {
            if (bSwitchBulk)
            {
            /* TODO */
            }

            /* program it */
            switch (bTrafficType)
            {
            case MUSB_ENDPOINT_XFER_BULK:
            case MUSB_ENDPOINT_XFER_INT:
                dwReqLength = pIrp->dwLength - (bIsTx ? pEnd->dwTxOffset : pEnd->dwRxOffset);
                pBuffer = pIrp->pBuffer;
                break;

#ifdef MUSB_ISO

            case MUSB_ENDPOINT_XFER_ISOC:
#ifdef MUSB_ISO_STREAM
                dwReqLength = pIsochIrp->dwLength;
#else
                dwReqLength = pIsochIrp->prISOpacket[0].dwDataLength;
#endif
                pBuffer = pIsochIrp->pBuffer;
                break;
#endif

            default:
                break;
            }
            if (bIsTx)
            {
                /* start next Tx if not interrupt protocol */
                if (MUSB_ENDPOINT_XFER_INT != bTrafficType)
                {
                    pEnd->pTxIrp = pNextIrp;
                    u4Ret = pPort->pfProgramStartTransmit(pPort, pEnd, pBuffer, dwReqLength, pNextIrp);
                }
            }
            else
            {
                u4Ret = pPort->pfProgramStartReceive(pPort, pEnd, pBuffer, dwReqLength, pNextIrp,
                                             bAllowDma);
            }
        }
        else
        {
            /* TODO: log internal error */
            if (bIsTx)
            {
                pEnd->pTxIrp = NULL;
            }
            else
            {
                pEnd->pRxIrp = NULL;
            }
        }
    }

    UNUSED(u4Ret);
}

/*
 * Set whether a pipe is halted
 * Returns: 0 on success; error code on failure
 */
#ifdef MUSB_FUNCTION 
uint32_t MUSB_SetPipeHalt(MUSB_Pipe hPipe, uint8_t bHalt)
{
    MGC_Pipe *pPipe;

    MGC_Port *pPort;
    uint8_t bIsTx;
    uint8_t bIsIn;

    MUSB_ASSERT(hPipe);

    pPipe = (MGC_Pipe *)hPipe;
    pPort = (MGC_Port *)pPipe->hSession;
    bIsTx = (pPipe->bmFlags & MGC_PIPEFLAGS_TRANSMIT) ? TRUE : FALSE;
    bIsIn = pPort->bIsHost ? !bIsTx : bIsTx;
    
    pPipe->pLocalEnd->bIsHalted = bHalt;
    return pPort->pfProgramHaltEndpoint(pPort, 
        pPipe->pLocalEnd, (uint8_t)(bIsIn ? MUSB_DIR_IN : 0), bHalt);
}
#endif
/*
 * Flush the given pipe
 * Returns: 0 on success; error code on failure
 */
uint32_t MUSB_FlushPipe(MUSB_Pipe hPipe)
{
    MGC_Pipe *pPipe;

    MGC_Port *pPort;
    uint8_t bIsTx;
    uint8_t bIsIn;

    MUSB_ASSERT(hPipe);
    if (!hPipe)
    {
        LOG(0, "hPipe = NULL.\n", NULL);
        return (uint32_t)MUSB_STATUS_INVALID_ARGUMENT;
    }

    pPipe = (MGC_Pipe *)hPipe;
    pPort = (MGC_Port *)pPipe->hSession;   
    if (!pPort)
    {
        LOG(0, "pPort = NULL.\n", NULL);
        return (uint32_t)MUSB_STATUS_INVALID_ARGUMENT;
    }
    
    bIsTx = (pPipe->bmFlags & MGC_PIPEFLAGS_TRANSMIT) ? TRUE : FALSE;
    bIsIn = pPort->bIsHost ? !bIsTx : bIsTx;

    return pPort->pfProgramFlushEndpoint(pPort, pPipe->pLocalEnd, (uint8_t)(bIsIn ? MUSB_DIR_IN : 0), FALSE);
}

/*
 * Start a single bulk/interrupt transfer
 * Returns: 0 on success; error code on failure
 */
uint32_t MUSB_StartTransfer(MUSB_Irp *pIrp)
{
    void *pCurrentIrp;

    uint32_t status = MUSB_STATUS_OK;
    MGC_Pipe *pPipe;
    MGC_Port *pPort;
    MGC_EndpointResource *pEnd;
    uint8_t bIsTx;
    uint8_t bTrafficType;

    MUSB_ASSERT(pIrp);

    pPipe = (MGC_Pipe *)pIrp->hPipe;
    if (!pPipe)
    {
        return MGC_M_STATUS_ERROR;
    }
    pPort = (MGC_Port *)pPipe->hSession;
    pEnd = pPipe->pLocalEnd;
    bIsTx = (pPipe->bmFlags & MGC_PIPEFLAGS_TRANSMIT) ? TRUE : FALSE;
    bTrafficType = bIsTx ? pEnd->bTrafficType : pEnd->bRxTrafficType;
    
    switch (bTrafficType)
    {
    case MUSB_ENDPOINT_XFER_CONTROL:
    case MUSB_ENDPOINT_XFER_ISOC:
        status = (uint32_t)MUSB_STATUS_INVALID_TYPE;
        break;

    case MUSB_ENDPOINT_XFER_BULK:
    case MUSB_ENDPOINT_XFER_INT:
        /* since lists use dynamic memory, try only if necessary */
        pCurrentIrp = bIsTx ? pEnd->pTxIrp : pEnd->pRxIrp;
        if (pCurrentIrp)
        {
            if (MUSB_ENDPOINT_XFER_INT == bTrafficType)
            {
                status = (uint32_t)MUSB_STATUS_ENDPOINT_BUSY;
            }
            else if (bIsTx && !MUSB_ListAppendItem(&(pEnd->TxIrpList), pIrp, 0))
            {
                status = (uint32_t)MUSB_STATUS_NO_MEMORY;
            }
            else if (!bIsTx && !MUSB_ListAppendItem(&(pEnd->RxIrpList), pIrp, 0))
            {
                status = (uint32_t)MUSB_STATUS_NO_MEMORY;
            }
            /* if now there is no current IRP and we are at list head, the ISR didn't start us */
            if (bIsTx)
            {
                if (!((volatile MGC_EndpointResource *)
                          pEnd)->pTxIrp && (pIrp == MUSB_ListFindItem(&(pEnd->TxIrpList), 0)))
                {
                    MGC_StartNextIrp(pPort, pEnd, TRUE);
                }
            }
            else
            {
                if (!((volatile MGC_EndpointResource *)
                          pEnd)->pRxIrp && (pIrp == MUSB_ListFindItem(&(pEnd->RxIrpList), 0)))
                {
                    MGC_StartNextIrp(pPort, pEnd, FALSE);
                }
            }
        }
        else
        {
            /* nothing pending, so make it this */
            if (bIsTx)
            {
                /* only start a Tx if endpoint is not halted */
                if (!pEnd->bIsHalted)
                {
                    status =
                        pPort->pfProgramStartTransmit(pPort, pEnd, pIrp->pBuffer, pIrp->dwLength,
                                                      pIrp);
                }
                else
                {
                    pEnd->pTxIrp = pIrp;
                }
            }
            else
            {
                status =
                    pPort->pfProgramStartReceive(pPort, pEnd, pIrp->pBuffer, pIrp->dwLength, pIrp,
                                                 pIrp->bAllowDma);
            }
        }
        break;

    default:
        break;
    }

    return status;
}

/*
 * Cancel a bulk/interrupt transfer
 * Returns: 0 on success; error code on failure
 */
uint32_t MUSB_CancelTransfer(MUSB_Irp *pIrp)
{
    uint8_t bDirection;
    uint32_t dwStatus = MUSB_STATUS_OK;
    MGC_Pipe *pPipe;
    MGC_Port *pPort;
    MGC_EndpointResource *pEnd;
    uint8_t bIsTx;

    MUSB_ASSERT(pIrp);

    pPipe = (MGC_Pipe *)pIrp->hPipe;
    if (!pPipe)
    {
        return MUSB_STATUS_OK;
    }
    pPort = (MGC_Port *)pPipe->hSession;   
    pEnd = pPipe->pLocalEnd;
    bIsTx = (pPipe->bmFlags & MGC_PIPEFLAGS_TRANSMIT) ? TRUE : FALSE;

    if (pPipe->bmFlags & MGC_PIPEFLAGS_HOST)
    {
        bDirection = (uint8_t)(bIsTx ? MUSB_DIR_OUT : MUSB_DIR_IN);
    }
    else
    {
        bDirection = (uint8_t)(bIsTx ? MUSB_DIR_IN : MUSB_DIR_OUT);
    }

    if ((bIsTx && (pIrp == pEnd->pTxIrp)) || (!bIsTx && (pIrp == pEnd->pRxIrp)))
    {
        if (bIsTx)
        {
            pEnd->pTxIrp = NULL;
            pEnd->dwTxOffset = 0;
        }
        else
        {
            pEnd->pRxIrp = NULL;
            pEnd->dwRxOffset = 0;
        }
        dwStatus = pPort->pfProgramFlushEndpoint(pPort, pEnd, bDirection, FALSE);
    }
    else
    {
         /* remove */
        if (bIsTx)
        {
            MUSB_ListRemoveItem(&(pEnd->TxIrpList), pIrp);
        }
        else
        {
            MUSB_ListRemoveItem(&(pEnd->RxIrpList), pIrp);
        }
     }

    return dwStatus;
}

#ifdef MUSB_ISO
/*
 * Schedule an isochronous transfer
 * Returns: 0 on success; error code on failure
 */

uint32_t MUSB_ScheduleIsochTransfer(uint32_t dwWaitCount, MUSB_IsochIrp *pIsochIrp)
{
    uint8_t *pBuffer;

    uint32_t dwLength;
    uint32_t status = MUSB_STATUS_OK;
    MGC_Pipe *pPipe;
    MGC_Port *pPort;
    MGC_EndpointResource *pEnd;
    uint8_t bIsTx;
    uint8_t bTrafficType;
    void *pTestIrp;
 
    MUSB_ASSERT(pIsochIrp);

    pPipe = (MGC_Pipe *)pIsochIrp->hPipe;
    pPort = (MGC_Port *)pPipe->hSession;
    pEnd = pPipe->pLocalEnd;
    bIsTx = (pPipe->bmFlags & MGC_PIPEFLAGS_TRANSMIT) ? TRUE : FALSE;
    bTrafficType = bIsTx ? pEnd->bTrafficType : pEnd->bRxTrafficType;
    bTrafficType &= MUSB_ENDPOINT_XFERTYPE_MASK;
    pTestIrp = bIsTx ? pEnd->pTxIrp : pEnd->pRxIrp;
 
    if (pTestIrp)
    {
        status = MUSB_STATUS_ENDPOINT_BUSY;
    }
    else
    {
        switch (bTrafficType)
        {
        case MUSB_ENDPOINT_XFER_ISOC:
            pBuffer = pIsochIrp->pBuffer;
#ifdef MUSB_ISO_STREAM
            dwLength = pIsochIrp->dwLength;
#else
            dwLength = pIsochIrp->prISOpacket[0].dwDataLength;
#endif
            /* TODO: really this should just go in schedule */
            if (bIsTx)
            {
                if (dwWaitCount)
                {
                    pEnd->dwWaitFrameCount = dwWaitCount;
                }
                else
                {
                    status = pPort->pfProgramStartTransmit(pPort, pEnd, pBuffer, dwLength,
                                                           pIsochIrp);
                }
            }
            else
            {
                status = pPort->pfProgramStartReceive(pPort, pEnd, pBuffer, dwLength, pIsochIrp,
                                                      pIsochIrp->bAllowDma);
            }
            break;
        
        case MUSB_ENDPOINT_XFER_CONTROL:
        case MUSB_ENDPOINT_XFER_BULK:
        case MUSB_ENDPOINT_XFER_INT:
        default:
            status = MUSB_STATUS_INVALID_TYPE;
            break;
        }
    }
 
    return status;
}

/*
 * Cancel an isochronous transfer
 * Returns: 0 on success; error code on failure
 */
uint32_t MUSB_CancelIsochTransfer(MUSB_IsochIrp *pIsochIrp)
{
    uint8_t bDirection;

    MGC_Pipe *pPipe;
    MGC_Port *pPort;
    MGC_EndpointResource *pEnd;

    MUSB_ASSERT(pIsochIrp);

    pPipe = (MGC_Pipe *)pIsochIrp->hPipe;
    pPort = (MGC_Port *)pPipe->hSession;
    pEnd = pPipe->pLocalEnd;

    if (pPipe->bmFlags & MGC_PIPEFLAGS_HOST)
    {
        bDirection = (pPipe->bmFlags & MGC_PIPEFLAGS_TRANSMIT) ? MUSB_DIR_OUT : MUSB_DIR_IN;
    }
    else
    {
        bDirection = (pPipe->bmFlags & MGC_PIPEFLAGS_TRANSMIT) ? MUSB_DIR_IN : MUSB_DIR_OUT;
    }

    return pPort->pfProgramFlushEndpoint(pPort, pEnd, bDirection, FALSE);
}
#endif
