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
 * MUSBStack-S MHDRC implementation.
 * $Revision: #1 $
 */

#include "mu_impl.h"

#ifdef MUSB_MHDRC

/*
* Implementations of MHDRC-specific functions
*/

//#include "hdrc_cnf.h"

#include "mu_diag.h"
#include "mu_hdrpr.h"
#include "mu_mdrpr.h"
#include "mu_mdrdf.h"
#include "mu_pippr.h"
#include "mu_funpr.h"
#include "mu_hstpr.h"

/*
 * Discover MHDRC FIFO configuration
 */
uint8_t MGC_MhdrcInit(MGC_Port* pPort)
{
    uint8_t bResult;
    uint8_t bMajor;
    uint16_t wMinor;
    uint16_t wVersion;
    MGC_Controller* pController = pPort->pController;
    uint8_t* pBase = (uint8_t*)pController->pControllerAddressIst;

    /* call HDRC init */
    bResult = MGC_HdrcInit(pPort);

    /* if >= 1.4, set CSR0's DisablePing bit */
    wVersion = MGC_Read16(pBase, MGC_O_MHDRC_HWVERS);
    bMajor = (wVersion & MGC_M_HWVERS_MAJOR) >> MGC_S_HWVERS_MAJOR;
    wMinor = (wVersion & MGC_M_HWVERS_MINOR);
    if((bMajor > 1) || ((1 == bMajor) && (wMinor >= 400)))
    {
	pPort->bHasDisablePing = TRUE;
    }

    return bResult;
}

/*
* Find the MHDRC's first (host) or required (function) 
* local endpoint resource to communicate with the given remote endpoint (host)
* or to be identified with the given endpoint descriptor (function).
*/
MGC_EndpointResource* MGC_MhdrcBindEndpoint(MGC_Port* pPort, 
					    const MUSB_DeviceEndpoint* pUsbEnd,
					    MUSB_EndpointResource* pRequest,
					    uint8_t bBind)
{
    uint8_t bEnd, bIsTx, bTrafficType;
    uint8_t bInterval;
    uint16_t csr;
    MGC_EndpointResource* pEnd;
    uint8_t bIsMulti = FALSE;
    uint8_t bHubAddr = 0;
    uint8_t bHubPort = 0;
    uint8_t reg = 0;
    MGC_Controller* pController = pPort->pController;
    uint8_t* pBase = (uint8_t*)pController->pControllerAddressIst;

#ifdef MUSB_C_DYNFIFO_DEF
    pEnd = MGC_HdrcBindDynamicEndpoint(pPort, pUsbEnd, bBind, &bIsTx);
#else
    pEnd = MGC_DrcBindEndpoint(pPort, pUsbEnd, pRequest, bBind, &bIsTx);
#endif
    if(pEnd && bBind)
    {
	/* prepare endpoint registers according to flags */
	bTrafficType = bIsTx ? pEnd->bTrafficType : pEnd->bRxTrafficType;
	bEnd = pEnd->bLocalEnd;
	MGC_SelectEnd(pBase, bEnd);

	if(pPort->bIsHost)
	{
	    /* speed field in proto reg */
	    switch(pUsbEnd->pDevice->ConnectionSpeed)
	    {
	    case MUSB_CONNECTION_SPEED_LOW:
		reg |= 0xc0;
		break;
	    case MUSB_CONNECTION_SPEED_FULL:
		reg |= 0x80;
		break;
	    default:
		reg |= 0x40;
	    }
	    /* target addr & hub addr/port */
	    if(pUsbEnd->pDevice->pParentUsbDevice)
	    {
		bHubAddr = pUsbEnd->pDevice->pParentUsbDevice->bBusAddress;
		bHubPort = pUsbEnd->pDevice->bHubPort;
		if(2 == pUsbEnd->pDevice->DeviceDescriptor.bDeviceProtocol)
		{
		    bIsMulti = TRUE;
		}
	    }
	    if(bIsTx)
	    {
		MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXFUNCADDR), 
		    pUsbEnd->pDevice->bBusAddress);
		MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXHUBADDR), 
		    bIsMulti ? 0x80 | bHubAddr : bHubAddr);
		MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXHUBPORT), 
		    bHubPort);
	    }
	    if(!bIsTx || !bEnd)
	    {
		MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXFUNCADDR), 
		    pUsbEnd->pDevice->bBusAddress);
		MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXHUBADDR), 
		    bIsMulti ? 0x80 | bHubAddr : bHubAddr);
		MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXHUBPORT), 
		    bHubPort);
	    }
	}

	/* proto reg */
	reg |= bIsTx ? (pEnd->bBusEnd & MUSB_ENDPOINT_NUMBER_MASK) : 
	(pEnd->bRxBusEnd & MUSB_ENDPOINT_NUMBER_MASK);
	if(bEnd)
	{
	    reg |= (bTrafficType << 4);
	}

	if(bIsTx)
	{
	    /* transmit */
	    if(pPort->bIsHost)
	    {
		/* protocol/endpoint/interval/NAKlimit */
		if(bEnd)
		{
		    MGC_WriteCsr8(pBase, MGC_O_MHDRC_TXTYPE, bEnd, reg);
		    /* 
		     * TODO: at least for bulk, setting the upper 4 interval bits causes the core 
		     * to continuously assert Rx interrupts with RxError set
		     */
		    bInterval = MUSB_MIN(pUsbEnd->UsbDescriptor.bInterval, 0x10);
		    if(MUSB_ENDPOINT_XFER_BULK == pEnd->bTrafficType)
		    {
			/* for bulk, use NAK limit and always use some limit */
			bInterval = MUSB_MIN(pUsbEnd->wNakLimit, 0x10);
			bInterval = MUSB_MAX(bInterval, 2);
			if(0xffff == pUsbEnd->wNakLimit)
			{
			    bInterval = 0;
			}
		    }
		    MGC_WriteCsr8(pBase, MGC_O_MHDRC_TXINTERVAL, bEnd, bInterval);
		}
		else
		{
		    MGC_WriteCsr8(pBase, MGC_O_MHDRC_NAKLIMIT0, 0, 0x10);
		    MGC_WriteCsr8(pBase, MGC_O_MHDRC_TYPE0, 0, reg & 0xc0);
		}
	    }

	    /* CSR */
	    if(bEnd)
	    {
      	        /* normal case: set up for packet size */
		MGC_WriteCsr16(pBase, MGC_O_MHDRC_TXMAXP, bEnd, pEnd->wPacketSize);

		csr = MGC_M_TXCSR_MODE;
#ifdef MUSB_ISO
		if(!pPort->bIsHost && (MUSB_ENDPOINT_XFER_ISOC == pEnd->bTrafficType))
		{
		    csr |= MGC_M_TXCSR_ISO;
		}
#endif
		/* flush twice in case of double packet buffering */
		MGC_WriteCsr16(pBase, MGC_O_MHDRC_TXCSR, bEnd, 
		    MGC_M_TXCSR_FLUSHFIFO | MGC_M_TXCSR_CLRDATATOG);
		MGC_WriteCsr16(pBase, MGC_O_MHDRC_TXCSR, bEnd, 
		    MGC_M_TXCSR_FLUSHFIFO | MGC_M_TXCSR_CLRDATATOG);
		/* final CSR setup */
		MGC_WriteCsr16(pBase, MGC_O_MHDRC_TXCSR, bEnd, csr);
	    }
	    else
	    {
		/* endpoint 0: just flush */
		MGC_WriteCsr16(pBase, MGC_O_MHDRC_CSR0, 0,
		    MGC_M_CSR0_FLUSHFIFO);
		MGC_WriteCsr16(pBase, MGC_O_MHDRC_CSR0, 0,
		    MGC_M_CSR0_FLUSHFIFO);
	    }
	}
	else
	{
	    /* receive */
	    if(pPort->bIsHost)
	    {
		/* protocol/endpoint/interval/NAKlimit */
		if(bEnd)
		{
		    /* first, ensure Rx mode */
		    MGC_WriteCsr16(pBase, MGC_O_MHDRC_TXCSR, bEnd, 0);
		    MGC_WriteCsr8(pBase, MGC_O_MHDRC_RXTYPE, bEnd, reg);

		    /* 
		     * TODO: at least for bulk, setting the upper 4 interval bits causes the core 
		     * to continuously assert Rx interrupts with RxError set
		     */
		    bInterval = MUSB_MIN(pUsbEnd->UsbDescriptor.bInterval, 0x10);
		    if(MUSB_ENDPOINT_XFER_BULK == pEnd->bRxTrafficType)
		    {
			/* for bulk, use NAK limit and always use some limit */
			bInterval = MUSB_MIN(pUsbEnd->wNakLimit, 0x10);
			bInterval = MUSB_MAX(bInterval, 2);
			if(0xffff == pUsbEnd->wNakLimit)
			{
			    bInterval = 0;
			}
		    }
		    MGC_WriteCsr8(pBase, MGC_O_MHDRC_RXINTERVAL, bEnd, bInterval);
		}
		else
		{
		    MGC_WriteCsr8(pBase, MGC_O_MHDRC_NAKLIMIT0, 0, 0x10);
		    MGC_WriteCsr8(pBase, MGC_O_MHDRC_TYPE0, 0, reg & 0xc0);
		}
	    }

	    /* CSR */
	    if(bEnd)
	    {
		csr = 0;
		/* packet size */
		MGC_WriteCsr16(pBase, MGC_O_MHDRC_RXMAXP, bEnd, pEnd->wRxPacketSize);
		if(pPort->bIsHost)
		{
		    csr = 0;
		}
		else
		{
#ifdef MUSB_ISO
		    if(pEnd->bRxTrafficType == MUSB_ENDPOINT_XFER_ISOC)
		    {
			csr = MGC_M_RXCSR_P_ISO;
		    }
#endif
		}
		/* twice in case of double packet buffering */
		MGC_WriteCsr16(pBase, MGC_O_MHDRC_RXCSR, bEnd, 
		    MGC_M_RXCSR_FLUSHFIFO | MGC_M_RXCSR_CLRDATATOG);
		MGC_WriteCsr16(pBase, MGC_O_MHDRC_RXCSR, bEnd, 
		    MGC_M_RXCSR_FLUSHFIFO | MGC_M_RXCSR_CLRDATATOG);
		if(MUSB_ENDPOINT_XFER_INT == pEnd->bRxTrafficType)
		{
		    csr |= MGC_M_RXCSR_DISNYET;
		}
		/* use AutoClear in peripheral mode */
		if(!pPort->bIsHost)
		{
		    csr |= MGC_M_RXCSR_AUTOCLEAR;
		}
		MGC_WriteCsr16(pBase, MGC_O_MHDRC_RXCSR, bEnd, csr);
	    }

	}
    }
    return pEnd;
}

/*
* Program the MHDRC to initiate reception.
*/
uint32_t MGC_MhdrcStartRx(MGC_Port* pPort, MGC_EndpointResource* pEnd, 
			  uint8_t* pBuffer, uint32_t dwTotalBytes,
			  void* pCurrentIrp, uint8_t bAllowDma)
{
#ifdef MUSB_DMA
    MUSB_DmaController* pDmaController;
    MUSB_DmaChannel* pDmaChannel;
    uint8_t bDmaOk;
#endif
    uint16_t wCsr, wRxCount, wIntrRxE;
    MGC_BsrItem qItem;
    MUSB_ControlIrp* pControlIrp;
    MUSB_Irp* pIrp;
    MUSB_IsochIrp* pIsochIrp;
    MGC_Pipe* pPipe;
    uint8_t bResult = FALSE;
    const MUSB_Device* pDevice = NULL;
    uint8_t bHubAddr = 0;
    uint8_t bHubPort = 0;
    uint8_t reg = 0;
    uint8_t bIsMulti = FALSE;
    MGC_Controller* pController = pPort->pController;
    uint8_t* pBase = (uint8_t*)pController->pControllerAddressIst;
    MUSB_SystemServices* pServices = pController->pSystemServices;
    uint8_t bEnd = pEnd->bLocalEnd;

    MGC_DIAG(3, pPort->pController, "[StartRx");

    MGC_SelectEnd(pBase, bEnd);

    /* clear mode bit in TxCSR */
    wCsr = MGC_ReadCsr16(pBase, MGC_O_MHDRC_TXCSR, bEnd);
    if((wCsr & MGC_M_TXCSR_MODE) && pEnd->bIsFifoShared)
    {
	wCsr &= ~MGC_M_TXCSR_MODE;
	MGC_WriteCsr16(pBase, MGC_O_MHDRC_TXCSR, bEnd, wCsr);
    }

    /* read RxCSR for preparation */
    wCsr = MGC_ReadCsr16(pBase, MGC_O_MHDRC_RXCSR, bEnd);
    /* prepare for the non-DMA case */
    wCsr &= ~(MGC_M_RXCSR_DMAENAB | MGC_M_RXCSR_DMAMODE);
    if(pPort->bIsHost)
    {
	wCsr |= MGC_M_RXCSR_H_REQPKT;
	wCsr &= ~MGC_M_RXCSR_RXPKTRDY;

	/* write protocol/endpoint/speed */
	reg = pEnd->bRxTrafficType << MGC_S_TYPE_PROTO;
	reg |= pEnd->bRxBusEnd & MGC_M_TYPE_REMOTE_END;
	switch(pEnd->bRxTrafficType)
	{
	case MUSB_ENDPOINT_XFER_CONTROL:
	    pControlIrp = (MUSB_ControlIrp*)pCurrentIrp;
	    pDevice = pControlIrp->pDevice;
	    break;
	case MUSB_ENDPOINT_XFER_ISOC:
	    pIsochIrp = (MUSB_IsochIrp*)pCurrentIrp;
	    pPipe = (MGC_Pipe*)pIsochIrp->hPipe;
	    pDevice = pPipe->pDevice;
	    break;
	default:
	    pIrp = (MUSB_Irp*)pCurrentIrp;
	    pPipe = (MGC_Pipe*)pIrp->hPipe;
	    pDevice = pPipe->pDevice;
	}
	/* speed field in proto reg */
	switch(pDevice->ConnectionSpeed)
	{
	case MUSB_CONNECTION_SPEED_LOW:
	    reg |= 0xc0;
	    break;
	case MUSB_CONNECTION_SPEED_FULL:
	    reg |= 0x80;
	    break;
	default:
	    reg |= 0x40;
	}
	MGC_WriteCsr8(pBase, MGC_O_MHDRC_RXTYPE, bEnd, reg);
	/* target addr & hub addr/port */
	if(pDevice->pParentUsbDevice)
	{
	    bHubAddr = pDevice->pParentUsbDevice->bBusAddress;
	    bHubPort = pDevice->bHubPort;
	    if(2 == pDevice->pParentUsbDevice->DeviceDescriptor.bDeviceProtocol)
	    {
		bIsMulti = TRUE;
	    }
	}
	MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXFUNCADDR), 
	    pDevice->bBusAddress);
	MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXHUBADDR), 
	    bIsMulti ? 0x80 | bHubAddr : bHubAddr);
	MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXHUBPORT), 
	    bHubPort);
    }

#if 0
    /* doesn't work reliably */
    if((MUSB_ENDPOINT_XFER_BULK == pEnd->bTrafficType) && pPort->bHasBulkCombine)
    {
	/* bulk combining: set up for IRP's request size */
	MGC_WriteCsr16(pBase, MGC_O_MHDRC_RXMAXP, bEnd, 
	    pEnd->wRxPacketSize | 
	    ((MGC_MIN(pEnd->wMaxPacketSizeRx, dwTotalBytes) / pEnd->wRxPacketSize) - 1) << 11);
    }
    else
    {
#endif
	/* normal case: set up for packet size */
	if(pEnd->wRxPacketSize != MGC_ReadCsr16(pBase, MGC_O_MHDRC_RXMAXP, bEnd))
	{
	    MGC_WriteCsr16(pBase, MGC_O_MHDRC_RXMAXP, bEnd, pEnd->wRxPacketSize);
	}
#if 0
    }
#endif

    pEnd->bBsrResidual = pEnd->bIsrResidual;

    /* disable endpoint interrupt */
    wIntrRxE = MGC_Read16(pBase, MGC_O_MHDRC_INTRRXE);
    MGC_Write16(pBase, MGC_O_MHDRC_INTRRXE, wIntrRxE & ~(1 << bEnd));

    if(!pPort->bIsHost)
    {
	/* handle residual if any */
	if((MGC_ReadCsr16(pBase, MGC_O_MHDRC_RXCSR, bEnd) & MGC_M_RXCSR_RXPKTRDY) ||
	    (pEnd->bBsrResidual != pEnd->bIsrResidual))
	{
	    /* poll until IRP is complete, since handing it to the ISR will always have races */
	    while(TRUE)
	    {
		if(wCsr & MGC_M_RXCSR_RXPKTRDY)
		{
		    wRxCount = MGC_ReadCsr16(pBase, MGC_O_MHDRC_RXCOUNT, bEnd);
		    MUSB_DIAG1(3, "StartRx: residual byte count=", wRxCount, 16, 0);
#ifdef MUSB_DMA
		    pDmaController = pPort->pDmaController;
		    pDmaChannel = pEnd->pRxDmaChannel;
		    if(!bAllowDma && pDmaChannel)
		    {
			/* release previously-allocated channel */
			pDmaController->pfDmaReleaseChannel(pDmaChannel);
			pEnd->pRxDmaChannel = NULL;
		    }
		    else if(bAllowDma && pServices->pfSystemToBusAddress(pBuffer))
		    {
			/* candidate for DMA */
			if(pDmaController && !pDmaChannel)
			{
			    pDmaChannel = pEnd->pRxDmaChannel = pDmaController->pfDmaAllocateChannel(
				pDmaController->pPrivateData, bEnd, FALSE, 
				pEnd->bRxTrafficType, pEnd->wRxPacketSize);
			}
			if(pDmaChannel)
			{
			    pDmaChannel->dwActualLength = 0L;
			    pEnd->bDmaTx = FALSE;

			    /*
			    * Speculate successful DMA programming and clear these fields if not.
			    * This is because although third-party DMACs obey the interface, 
			    * the Inventra DMAC in some cases will immediately activate, finish,
			    * and interrupt us before the RxCSR write below, and these fields
			    * are needed by its ISR.
			    */
			    pEnd->bDmaResidual = TRUE;
			    pEnd->pRxIrp = pCurrentIrp;

			    /* attempt to program the channel */
			    bDmaOk = pDmaController->pfDmaProgramChannel(pDmaChannel, 
				pEnd->wRxPacketSize, 0, pBuffer, 
				MGC_MIN(dwTotalBytes, pDmaChannel->dwMaxLength));
			    if(bDmaOk)
			    {
				if(((volatile MGC_EndpointResource*)pEnd)->pRxIrp)
				{
				    /* external DMAC; need to assert request line */
				    wCsr = MGC_ReadCsr16(pBase, MGC_O_MHDRC_RXCSR, bEnd);
				    wCsr |= (MGC_M_RXCSR_AUTOCLEAR | MGC_M_RXCSR_DMAENAB);
				    /* this special sequence is required to get DMAReq to activate */
				    MGC_WriteCsr8(pBase, (MGC_O_MHDRC_RXCSR+1), bEnd, 
					(uint8_t)((wCsr | MGC_M_RXCSR_DMAMODE) >> 8));
				    MGC_WriteCsr8(pBase, (MGC_O_MHDRC_RXCSR+1), bEnd, 
					(uint8_t)(wCsr >> 8));
				}
				MGC_DIAG(3, pPort->pController, "DMA unload - StartRx]");
				return 0;
			    }
			    else
			    {
				pEnd->bDmaResidual = FALSE;
				pEnd->pRxIrp = NULL;
				pDmaController->pfDmaReleaseChannel(pDmaChannel);
				pEnd->pRxDmaChannel = NULL;
			    }
			}
		    }
#endif
		    bResult = MGC_PipePacketReceived(pPort, pEnd, wRxCount, 
			FALSE, pCurrentIrp);

		    /* clear recv condition if necessary */
		    wCsr &= ~MGC_M_RXCSR_RXPKTRDY;
		    if((wRxCount < pEnd->wRxPacketSize) || !(wCsr & MGC_M_RXCSR_AUTOCLEAR))
		    {
			MGC_WriteCsr16(pBase, MGC_O_MHDRC_RXCSR, bEnd, wCsr);
		    }
		    /* complete IRP if necessary */
		    if(bResult)
		    {
			if(MGC_CompleteIrp(&qItem, pEnd, (uint8_t)MUSB_STATUS_OK, FALSE, pCurrentIrp))
			{
			    VERIFY(pServices->pfQueueBackgroundItem(pServices->pPrivateData, &qItem));
			}
			/* re-enable interrupt */
			MGC_Write16(pBase, MGC_O_MHDRC_INTRRXE, wIntrRxE);
			MGC_DIAG(3, pPort->pController, "IRP Complete - StartRx]");
			return 0;
		    }
		}
		/* in the non-DMA case we may be slow enough that another packet is ready now */
		wCsr = MGC_ReadCsr16(pBase, MGC_O_MHDRC_RXCSR, bEnd);
	    }
	    /* no residual; set current IRP */
	    pEnd->pRxIrp = pCurrentIrp;

	}
    }

    /* no residual; set current IRP */
    pEnd->pRxIrp = pCurrentIrp;

#ifdef MUSB_DMA
    pDmaController = pPort->pDmaController;
    pDmaChannel = pEnd->pRxDmaChannel;
    if(!bAllowDma && pDmaChannel)
    {
	/* release previously-allocated channel */
	pDmaController->pfDmaReleaseChannel(pDmaChannel);
	pEnd->pRxDmaChannel = NULL;
    }
    else if(bAllowDma && pServices->pfSystemToBusAddress(pBuffer))
    {
	/* candidate for DMA */
	if(pDmaController && !pDmaChannel)
	{
	    pDmaChannel = pEnd->pRxDmaChannel = pDmaController->pfDmaAllocateChannel(
		pDmaController->pPrivateData, bEnd, FALSE, 
		pEnd->bRxTrafficType, pEnd->wRxPacketSize);
	}
	if(pDmaChannel)
	{
	    pEnd->bDmaTx = FALSE;
	    pDmaChannel->dwActualLength = 0L;
	    /* Program DMA channel */
	    bDmaOk = pDmaController->pfDmaProgramChannel(pDmaChannel, 
		pEnd->wRxPacketSize, pDmaChannel->bDesiredMode, pBuffer, 
		MGC_MIN(dwTotalBytes, pDmaChannel->dwMaxLength));
	    if(bDmaOk)
	    {
		/* DMA channel is ready; finish RxCSR programming */
		if(pPort->bIsHost)
		{
		    wCsr &= ~MGC_M_RXCSR_H_REQPKT;
		    wCsr |= MGC_M_RXCSR_H_AUTOREQ;
		}
		wCsr |= (MGC_M_RXCSR_AUTOCLEAR | MGC_M_RXCSR_DMAENAB | 
		    (pDmaChannel->bDesiredMode ? MGC_M_RXCSR_DMAMODE : 0));
	    }
	    else
	    {
		pDmaController->pfDmaReleaseChannel(pDmaChannel);
		pEnd->pRxDmaChannel = NULL;
	    }
	}
    }
#endif

    /* write RxCSR */
    if(pPort->bIsHost)
    {
	MGC_WriteCsr16(pBase, MGC_O_MHDRC_RXCSR, bEnd, wCsr);
    }
    else
    {
	MGC_WriteCsr8(pBase, (MGC_O_MHDRC_RXCSR+1), bEnd, (uint8_t)(wCsr >> 8));
    }

#ifdef MUSB_DMA
    if(bAllowDma && pDmaChannel && bDmaOk)
    {
	MGC_DIAG(3, pPort->pController, "StartRx]");
	return 0;
    }
#endif

    /* re-enable interrupt */
    MGC_Write16(pBase, MGC_O_MHDRC_INTRRXE, wIntrRxE);

    MGC_DIAG(3, pPort->pController, "StartRx]");

    return 0;
}

/*
* Program the MHDRC to initiate transmit.
*/
uint32_t MGC_MhdrcStartTx(MGC_Port* pPort, MGC_EndpointResource* pEnd, 
			  const uint8_t* pBuffer, uint32_t dwTotalBytes, 
			  void* pGenIrp)
{
#ifdef MUSB_DMA
    MUSB_DmaController* pDmaController;
    MUSB_DmaChannel* pDmaChannel;
    uint8_t bDmaOk;
#endif
    uint32_t dwLoadCount;
    MUSB_ControlIrp* pControlIrp;
    MUSB_Irp* pIrp;
    MUSB_IsochIrp* pIsochIrp;
    MGC_Pipe* pPipe;
    const MUSB_Device* pDevice = NULL;
    uint16_t wCsr = 0;
    uint8_t bHubAddr = 0;
    uint8_t bHubPort = 0;
    uint8_t reg = 0;
    uint8_t bIsMulti = FALSE;
    MGC_Controller* pController = pPort->pController;
    uint8_t* pBase = (uint8_t*)pController->pControllerAddressIst;
#ifdef MUSB_DMA
    uint8_t bAllowDma = FALSE;
    MUSB_SystemServices* pServices = pController->pSystemServices;
#endif
    uint8_t bEnd = pEnd->bLocalEnd;

    MGC_SelectEnd(pBase, bEnd);

    if(pPort->bIsHost)
    {
	/* host mode; program protocol/speed and destination */
	reg = pEnd->bTrafficType << MGC_S_TYPE_PROTO;
	reg |= pEnd->bBusEnd & MGC_M_TYPE_REMOTE_END;
	switch(pEnd->bTrafficType)
	{
	case MUSB_ENDPOINT_XFER_CONTROL:
	    pControlIrp = (MUSB_ControlIrp*)pGenIrp;
	    pDevice = pControlIrp->pDevice;
	    break;
	case MUSB_ENDPOINT_XFER_ISOC:
	    pIsochIrp = (MUSB_IsochIrp*)pGenIrp;
	    pPipe = (MGC_Pipe*)pIsochIrp->hPipe;
	    pDevice = pPipe->pDevice;
	    break;
	default:
	    pIrp = (MUSB_Irp*)pGenIrp;
	    pPipe = (MGC_Pipe*)pIrp->hPipe;
	    pDevice = pPipe->pDevice;
	}
	/* speed field in proto reg */
	switch(pDevice->ConnectionSpeed)
	{
	case MUSB_CONNECTION_SPEED_LOW:
	    reg |= 0xc0;
	    break;
	case MUSB_CONNECTION_SPEED_FULL:
	    reg |= 0x80;
	    break;
	default:
	    reg |= 0x40;
	}
	if(bEnd)
	{
	    MGC_WriteCsr8(pBase, MGC_O_MHDRC_TXTYPE, bEnd, reg);
	}
	else
	{
	    MGC_WriteCsr8(pBase, MGC_O_MHDRC_TYPE0, 0, reg & 0xc0);
	}
	/* target addr & hub addr/port */
	if(pDevice->pParentUsbDevice)
	{
	    bHubAddr = pDevice->pParentUsbDevice->bBusAddress;
	    bHubPort = pDevice->bHubPort;
	    if(2 == pDevice->pParentUsbDevice->DeviceDescriptor.bDeviceProtocol)
	    {
		bIsMulti = TRUE;
	    }
	}
	MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXFUNCADDR), 
	    pDevice->bBusAddress);
	MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXHUBADDR), 
	    bIsMulti ? 0x80 | bHubAddr : bHubAddr);
	MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXHUBPORT), 
	    bHubPort);
	if(!bEnd)
	{
	    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXFUNCADDR), 
		pDevice->bBusAddress);
	    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXHUBADDR), 
		bIsMulti ? 0x80 | bHubAddr : bHubAddr);
	    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXHUBPORT), 
		bHubPort);
	}
    }

    /* determine how much to load into FIFO for non-DMA case */
     dwLoadCount = MGC_MIN(pEnd->wPacketSize, dwTotalBytes);
    
    if(bEnd)
    {
	/* read CSR for preparation */
        wCsr = MGC_ReadCsr16(pBase, MGC_O_MHDRC_TXCSR, bEnd) & ~MGC_M_TXCSR_P_UNDERRUN;
    }
    else
    {
	/* ensure FIFO ready for writing */
	MGC_WriteCsr16(pBase, MGC_O_MHDRC_CSR0, 0, 0);
    }

#ifdef MUSB_DMA
    bDmaOk = FALSE;
    switch(pEnd->bTrafficType)
    {
    case MUSB_ENDPOINT_XFER_ISOC:
	pIsochIrp = (MUSB_IsochIrp*)pGenIrp;
	bAllowDma = pIsochIrp->bAllowDma;
	break;
    case MUSB_ENDPOINT_XFER_CONTROL:
	pControlIrp = (MUSB_ControlIrp*)pGenIrp;
	bAllowDma = pControlIrp->bAllowDma;
	break;
    default:
	pIrp = (MUSB_Irp*)pGenIrp;
	bAllowDma = pIrp->bAllowDma;
    }
    pDmaController = pPort->pDmaController;
    pDmaChannel = pEnd->pDmaChannel;
    if(!bAllowDma && pDmaChannel)
    {
	/* release previously-allocated channel */
	pDmaController->pfDmaReleaseChannel(pDmaChannel);
	pEnd->pDmaChannel = NULL;
    }
    else if(bAllowDma && pServices->pfSystemToBusAddress(pBuffer))
    {
	/* candidate for DMA */
	if(pDmaController && !pDmaChannel)
	{
	    pDmaChannel = pEnd->pDmaChannel = pDmaController->pfDmaAllocateChannel(
		pDmaController->pPrivateData, bEnd, TRUE, 
		pEnd->bTrafficType, pEnd->wPacketSize);
	}
	if(pDmaChannel)
	{
	    pEnd->bDmaTx = TRUE;
	    pDmaChannel->dwActualLength = 0L;
	    bDmaOk = pDmaController->pfDmaProgramChannel(pDmaChannel, 
		pEnd->wPacketSize, pDmaChannel->bDesiredMode, pBuffer, 
		MGC_MIN(dwTotalBytes, pDmaChannel->dwMaxLength));
	    if(bDmaOk)
	    {
		dwLoadCount = 0;
		if(bEnd)
		{
		    wCsr |= (MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB | 
			(pDmaChannel->bDesiredMode ? MGC_M_TXCSR_DMAMODE : 0));
		}
	    }
	    else
	    {
		pDmaController->pfDmaReleaseChannel(pDmaChannel);
		pEnd->pDmaChannel = NULL;
	    }
	}
    }
    if(!bDmaOk)
    {
#endif
    /* load FIFO */
    pPort->pfLoadFifo(pPort, bEnd, dwLoadCount, pBuffer);
    pEnd->dwTxSize = dwLoadCount;
    /* prepare CSR */
    if(bEnd)
    {
	/* since we cannot clear DMAMODE before/during clearing DMAENAB, never clear DMAMODE */
	wCsr &= ~(MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB);
	wCsr |= MGC_M_TXCSR_TXPKTRDY;
    }
    else
    {
	/* endpoint 0 is a fixed case */
	wCsr = MGC_M_CSR0_H_SETUPPKT | MGC_M_CSR0_TXPKTRDY;
	if(pPort->bHasDisablePing)
	{
	    wCsr |= MGC_M_CSR0_H_NO_PING;
	}
    }
#ifdef MUSB_DMA
    }
#endif

    /* write CSR */
    pEnd->pTxIrp = pGenIrp;
    if(bEnd)
    {
	MGC_WriteCsr16(pBase, MGC_O_MHDRC_TXCSR, bEnd, wCsr | MGC_M_TXCSR_MODE);
    }
    else
    {
	MGC_WriteCsr16(pBase, MGC_O_MHDRC_CSR0, 0, wCsr);
    }

    return 0;
}

#ifdef MUSB_DIAG

/*
* Dump info global to the MHDRC
*/
int32_t MGC_MhdrcDumpState(MGC_Controller* pController)
{
    /* TODO */
    return 0;
}

/*
* Dump info relevant to the given local endpoint resource
*/
int32_t MGC_MhdrcDumpEndpoint(MGC_Controller* pController, MUSB_EndpointResource* pEnd)
{
    /* TODO */
    return 0;
}

#endif	/* diagnostics enabled */

#endif	/* MHDRC enabled */

