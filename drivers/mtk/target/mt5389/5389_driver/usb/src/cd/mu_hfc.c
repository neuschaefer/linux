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
 * MUSBStack-S HSFC implementation.
 * $Revision: #1 $
 */

#include "mu_impl.h"

#if defined(MUSB_HSFC)

/*
* Implementations of HSFC-specific functions
*/

#include "hsfc_cnf.h"

#include "mu_diag.h"

#include "mu_fcpr.h"
#include "mu_hfcpr.h"
#include "mu_hfcdf.h"
#include "mu_pippr.h"
#include "mu_funpr.h"

static const uint8_t MGC_aTestPacket[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee,
    0xee, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xbf, 0xdf,
    0xef, 0xf7, 0xfb, 0xfd, 0xfc, 0x7e, 0xbf, 0xdf,
    0xef, 0xf7, 0xfb, 0xfd, 0x7e
};

/*
 * Discover HSFC FIFO configuration
 */
uint8_t MGC_HsfcInit(MGC_Port* pPort)
{
    MGC_EndpointResource end;
    uint8_t bEnd;
    uint8_t* pBase = pPort->pController->pControllerAddressIst;

#ifdef MUSB_C_MP_RX
    pPort->bHasBulkCombine = TRUE;
#endif
#ifdef MUSB_C_MP_TX
    pPort->bHasBulkSplit = TRUE;
#endif
#ifdef MUSB_C_HB_RX
    pPort->bHasHbIsoRx = TRUE;
#endif
#ifdef MUSB_C_HB_TX
    pPort->bHasHbIsoTx = TRUE;
#endif

    pPort->bEndCount = MUSB_C_NUM_EPS;
    pPort->wEndMask =  1;
    pPort->pPrivateData = MUSB_MemAlloc(MUSB_C_NUM_EPS *
	sizeof(MGC_EndpointResource));
    if(pPort->pPrivateData)
    {
	if(MUSB_ArrayInit(&(pPort->LocalEnds), sizeof(MGC_EndpointResource), 
	    MUSB_C_NUM_EPS, pPort->pPrivateData))
	{
	    /* add endpoint 0 */
	    MUSB_MemSet((void*)&end, 0, sizeof(end));
	    end.bIsFifoShared = TRUE;
	    end.bTrafficType = MUSB_ENDPOINT_XFER_CONTROL;
	    end.wMaxPacketSizeTx = MGC_END0_FIFOSIZE;
	    end.wMaxPacketSizeRx = MGC_END0_FIFOSIZE;
	    end.bIsClaimed = TRUE;
	    MUSB_ArrayAppend(&(pPort->LocalEnds), &end);
	    MGC_SelectEnd(pBase, 0);
	    MGC_Write8(pBase, MGC_O_HSFC_RXFIFOSZ, 3);
	    MGC_Write8(pBase, MGC_O_HSFC_TXFIFOSZ, 3);
	    MGC_Write16(pBase, MGC_O_HSFC_RXFIFOADD, 0);
	    MGC_Write16(pBase, MGC_O_HSFC_TXFIFOADD, 0);

	    /* add others */
	    for(bEnd = 1; bEnd < (uint8_t)(MUSB_C_NUM_EPS & 0xff); bEnd++)
	    {
		MUSB_MemSet((void*)&end, 0, sizeof(end));
		end.bLocalEnd = bEnd;
		MUSB_ArrayAppend(&(pPort->LocalEnds), &end);
		pPort->wEndMask |= (1 << bEnd);

#ifdef MUSB_C_EP1_TOR_DEF
		if(1 == bEnd) end.bIsFifoShared = TRUE;
#endif
#ifdef MUSB_C_EP2_TOR_DEF
		if(2 == bEnd) end.bIsFifoShared = TRUE;
#endif
#ifdef MUSB_C_EP3_TOR_DEF
		if(3 == bEnd) end.bIsFifoShared = TRUE;
#endif
#ifdef MUSB_C_EP4_TOR_DEF
		if(4 == bEnd) end.bIsFifoShared = TRUE;
#endif
#ifdef MUSB_C_EP5_TOR_DEF
		if(5 == bEnd) end.bIsFifoShared = TRUE;
#endif
#ifdef MUSB_C_EP6_TOR_DEF
		if(6 == bEnd) end.bIsFifoShared = TRUE;
#endif
#ifdef MUSB_C_EP7_TOR_DEF
		if(7 == bEnd) end.bIsFifoShared = TRUE;
#endif
#ifdef MUSB_C_EP8_TOR_DEF
		if(8 == bEnd) end.bIsFifoShared = TRUE;
#endif
#ifdef MUSB_C_EP9_TOR_DEF
		if(9 == bEnd) end.bIsFifoShared = TRUE;
#endif
#ifdef MUSB_C_EP10_TOR_DEF
		if(10 == bEnd) end.bIsFifoShared = TRUE;
#endif
#ifdef MUSB_C_EP11_TOR_DEF
		if(11 == bEnd) end.bIsFifoShared = TRUE;
#endif
#ifdef MUSB_C_EP12_TOR_DEF
		if(12 == bEnd) end.bIsFifoShared = TRUE;
#endif
#ifdef MUSB_C_EP13_TOR_DEF
		if(13 == bEnd) end.bIsFifoShared = TRUE;
#endif
#ifdef MUSB_C_EP14_TOR_DEF
		if(14 == bEnd) end.bIsFifoShared = TRUE;
#endif
#ifdef MUSB_C_EP15_TOR_DEF
		if(15 == bEnd) end.bIsFifoShared = TRUE;
#endif

#ifndef MUSB_C_DYNFIFO_DEF

#if MUSB_C_EP1T_BITS > 2
		if(1 == bEnd) end.wMaxPacketSizeTx = 1 << MUSB_C_EP1T_BITS;
#endif
#if MUSB_C_EP1R_BITS > 2
		if(1 == bEnd) end.wMaxPacketSizeRx = 1 << MUSB_C_EP1R_BITS;
#endif
#if MUSB_C_EP2T_BITS > 2
		if(2 == bEnd) end.wMaxPacketSizeTx = 1 << MUSB_C_EP2T_BITS;
#endif
#if MUSB_C_EP2R_BITS > 2
		if(2 == bEnd) end.wMaxPacketSizeRx = 1 << MUSB_C_EP2R_BITS;
#endif
		/* TODO: 3..15 */

#endif	/* !MUSB_C_DYNFIFO_DEF */
	    }
	    return TRUE;
	}
	else
	{
	    MUSB_MemFree(pPort->pPrivateData);
	}
    }
    return FALSE;
}

void MGC_HsfcDynamicFifoLocation(MGC_Port* pPort, 
				 uint8_t bEnd, 
				 uint8_t bIsTx,
				 uint8_t bSet,
				 uint32_t* pdwBase, 
				 uint8_t* pbSize83, 
				 uint8_t* pbDoubleBuffered)
{
#ifdef MUSB_C_DYNFIFO_DEF
    uint8_t bVal;
    MGC_Controller* pImplController = pPort->pController;
    uint8_t* pBase = (uint8_t*)pImplController->pControllerAddressIst;
    MGC_EndpointResource* pEnd = (MGC_EndpointResource*)MUSB_ArrayFetch(&(pPort->LocalEnds), bEnd);

    MGC_SelectEnd(pBase, bEnd);
    if(bSet)
    {
	/* set new location/size */
	bVal = *pbSize83;
	if(*pbDoubleBuffered)
	{
	    bVal |= MGC_M_FIFOSZ_DPB;
	}
	if(bIsTx)
	{
	    pEnd->bTxFifoSize = bVal;
	    MGC_Write8(pBase, MGC_O_HSFC_TXFIFOSZ, bVal);
	    MGC_Write16(pBase, MGC_O_HSFC_TXFIFOADD, 
		(uint16_t)((*pdwBase >> 3) & 0xffff));
	}
	else
	{
	    pEnd->bRxFifoSize = bVal;
	    MGC_Write8(pBase, MGC_O_HSFC_RXFIFOSZ, bVal);
	    MGC_Write16(pBase, MGC_O_HSFC_RXFIFOADD, 
		(uint16_t)((*pdwBase >> 3) & 0xffff));
	}
    }
    else
    {
	/* get current location/size */
	if(bIsTx)
	{
	    bVal = pEnd->bTxFifoSize;
	    *pdwBase = MGC_Read16(pBase, MGC_O_HSFC_TXFIFOADD) << 3;
	}
	else
	{
	    bVal = pEnd->bRxFifoSize;
	    *pdwBase = MGC_Read16(pBase, MGC_O_HSFC_RXFIFOADD) << 3;
	}
	*pbSize83 = bVal & MGC_M_FIFOSZ_SIZE;
	*pbDoubleBuffered = (bVal & MGC_M_FIFOSZ_DPB) ? TRUE : FALSE;
    }
#endif	/* dynamic FIFOs */
}

#ifdef MUSB_C_DYNFIFO_DEF
#define MGC_DFIFO_TOTAL (1 << (MUSB_C_RAM_BITS + 2))
#define MGC_DFIFO_HIGH (MGC_DFIFO_TOTAL >> 1)
MGC_EndpointResource* MGC_HsfcBindDynamicEndpoint(MGC_Port* pPort, 
						  const MUSB_DeviceEndpoint* pUsbEnd,
						  MUSB_EndpointResource* pRequest,
						  uint8_t bBind, uint8_t* pbIsTx)
{
    return MGC_AllocateDynamicFifo(pPort, pUsbEnd, bBind, pbIsTx,
	MUSB_C_NUM_EPS, MGC_DFIFO_TOTAL, MGC_DFIFO_HIGH);
}
#endif	/* dynamic FIFOs */

/*
* HSFC Interrupt Service Routine
*/
int32_t MGC_HsfcIsr(void* pParam)
{
    int32_t result;
    uint8_t bIntrUsbValue;
    uint16_t wIntrTxValue, wIntrRxValue;
    MUSB_Controller* pController = (MUSB_Controller*)pParam;
    MGC_Controller* pControllerImpl = (MGC_Controller*)pController->pPrivateData;
    uint8_t* pBase = (uint8_t*)pControllerImpl->pControllerAddressIst;
    /* save index */
    uint8_t bIndex = MGC_Read8(pBase, MGC_O_HSFC_INDEX);

    /* read registers */
    bIntrUsbValue = MGC_Read8(pBase, MGC_O_HSFC_INTRUSB);
    wIntrTxValue = MGC_Read16(pBase, MGC_O_HSFC_INTRTX);
    wIntrRxValue = MGC_Read16(pBase, MGC_O_HSFC_INTRRX);

    /* call common ISR */
    result = MGC_FcIsr(pControllerImpl, bIntrUsbValue, wIntrTxValue, wIntrRxValue);

    /* restore index */
    MGC_Write8(pBase, MGC_O_HSFC_INDEX, bIndex);

    return result;
}

/*
* Program the HSFC to start (enable interrupts, etc.).
*/
uint32_t MGC_HsfcStart(MGC_Controller* pController)
{
    uint16_t val;
    uint8_t* pBase = (uint8_t*)pController->pControllerAddressIsr;
    MGC_Port* pPort = pController->pPort;

    /*  Set INT enable registers */
    val = 0xffff & pPort->wEndMask;
    MGC_DIAG1(3, pController, "Setting IntrTxE to ", val, 16, 4);
    MGC_Write16(pBase, MGC_O_HSFC_INTRTXE, val);
    val = 0xfffe & pPort->wEndMask;
    MGC_DIAG1(3, pController, "Setting IntrRxE to ", val, 16, 4);
    MGC_Write16(pBase, MGC_O_HSFC_INTRRXE, val);
#ifdef MUSB_SCHEDULER
    /* TODO: enable SOF interrupts only when needed (maybe in ProgramBusState when the schedule is non-empty) */
    val = 0xff;
#else
    val = (uint16_t)~MGC_M_INTR_SOF;
#endif
    MGC_DIAG1(3, pController, "Setting IntrUsbE to ", (val & 0xff), 16, 2);
    MGC_Write8(pBase, MGC_O_HSFC_INTRUSBE, (val & 0xff));

    val = MGC_Read8(pBase, MGC_O_HSFC_POWER);
    MGC_Write8(pBase, MGC_O_HSFC_POWER, val | MGC_M_POWER_ISOUPDATE);
    return 0;
}

/*
* Program the HSFC to stop (disable interrupts, etc.).
*/
uint32_t MGC_HsfcStop(MGC_Controller* pController)
{
    uint16_t temp;
    uint8_t* pBase = (uint8_t*)pController->pControllerAddressIst;

    /* disable interrupts */
    MGC_Write8(pBase, MGC_O_HSFC_INTRUSBE, 0);
    MGC_Write16(pBase, MGC_O_HSFC_INTRTXE, 0);
    MGC_Write16(pBase, MGC_O_HSFC_INTRRXE, 0);

    /*  flush pending interrupts */
    temp = MGC_Read8(pBase, MGC_O_HSFC_INTRUSB);
    temp = MGC_Read16(pBase, MGC_O_HSFC_INTRTX);
    temp = MGC_Read16(pBase, MGC_O_HSFC_INTRRX);

    return 0;
}

uint32_t MGC_HsfcDestroy(MGC_Controller* pController)
{
    MGC_Port* pPort = pController->pPort;

    if(pPort->pPrivateData)
    {
	MUSB_MemFree(pPort->pPrivateData);
    }
    return MGC_FunctionDestroy(pPort);
}

/* Fill the bus state info from the controller */
uint32_t MGC_HsfcReadBusState(MGC_Port* pPort)
{
    uint8_t power;
    MGC_Controller* pController = pPort->pController;
    uint8_t* pBase = (uint8_t*)pController->pControllerAddressIst;

    /* TODO: really these shouldn't be used */
    pPort->bConnectorId = TRUE;
    pPort->bIsSession = TRUE;
    pPort->bIsHost = FALSE;

    /* bIsHighSpeed, bIsSuspend, bIsReset, bIsResume, bIsSession, bBusVoltage */
    power = MGC_Read8(pBase, MGC_O_HSFC_POWER);
    pPort->bIsHighSpeed = (power & MGC_M_POWER_HSMODE) ? TRUE : FALSE;
    pPort->bIsSuspend = (power & MGC_M_POWER_SUSPENDM) ? TRUE : FALSE;
    pPort->bIsReset = (power & MGC_M_POWER_RESET) ? TRUE : FALSE;
    pPort->bIsResume = (power & MGC_M_POWER_RESUME) ? TRUE : FALSE;

    /* dwFrame */
    /* if some one is using this, then have protection for updating it */
    pPort->dwFrame = MGC_Read16(pBase, MGC_O_HSFC_FRAME);

    return 0;
}

/* Program the bus state from the port (see MGC_Port) */
uint32_t MGC_HsfcProgramBusState(MGC_Port* pPort)
{
    MGC_Controller* pController = pPort->pController;
    uint8_t* pBase = (uint8_t*)pController->pControllerAddressIst;
    uint8_t power = MGC_Read8(pBase, MGC_O_HSFC_POWER);
    uint8_t nPower = power;
    uint8_t bFuncAddr = MGC_Read8(pBase, MGC_O_HSFC_FADDR);

    /* resume? */
    if(pPort->bWantResume)
    {
	nPower |= MGC_M_POWER_RESUME;
    }
    else
    {
	nPower &= ~MGC_M_POWER_RESUME;
    }

    /* suspend? */
    if(pPort->bWantSuspend)
    {
	nPower |= MGC_M_POWER_SUSPENDM;
    }

    /* high-speed? */
    if(pPort->bWantHighSpeed)
    {
	nPower |= MGC_M_POWER_HSENAB;
    }
    else
    {
	nPower &= ~MGC_M_POWER_HSENAB;
    }

    /* make it so, but only on changes */
    if(pPort->bFuncAddr != bFuncAddr)
    {
	MGC_DIAG1(3, pController, "Updating FADDR to ", pPort->bFuncAddr, 10, 0);
	MGC_Write8(pBase, MGC_O_HSFC_FADDR, pPort->bFuncAddr);
    }
    if(nPower != power)
    {
	MGC_DIAG1(3, pController, "Updating POWER to ", nPower, 16, 2);
	MGC_Write8(pBase, MGC_O_HSFC_POWER, nPower);
    }

    return 0;
}

/*
* Find the HSFC's required 
* local endpoint resource to be identified with the given endpoint descriptor.
*/
MGC_EndpointResource* MGC_HsfcBindEndpoint(MGC_Port* pPort, 
					   const MUSB_DeviceEndpoint* pUsbEnd,
					   MUSB_EndpointResource* pRequest,
					   uint8_t bBind)
{
    uint8_t bEnd, bIsTx, bTrafficType;
    MGC_EndpointResource* pEnd;
    uint16_t csr = 0;
    MGC_Controller* pController = pPort->pController;
    uint8_t* pBase = (uint8_t*)pController->pControllerAddressIst;

#ifdef MUSB_C_DYNFIFO_DEF
    pEnd = MGC_HsfcBindDynamicEndpoint(pPort, pUsbEnd, pRequest, bBind, &bIsTx);
#else
    pEnd = MGC_FcBindEndpoint(pPort, pUsbEnd, pRequest, bBind, &bIsTx);
#endif
    if(pEnd && bBind)
    {
	/* prepare endpoint registers according to flags */
	bTrafficType = bIsTx ? pEnd->bTrafficType : pEnd->bRxTrafficType;
	bEnd = pEnd->bLocalEnd;
	MGC_SelectEnd(pBase, bEnd);

	if(bIsTx)
	{
	    /* transmit */

	    /* CSR */
	    if(bEnd)
	    {
  	        /* normal case: set up for packet size */
		MGC_WriteCsr16(pBase, MGC_O_HSFC_TXMAXP, bEnd, pEnd->wPacketSize);

		csr = MGC_M_TXCSR_MODE;
#ifdef MUSB_ISO
		if(pEnd->bTrafficType == MUSB_ENDPOINT_XFER_ISOC)
		{
		    csr |= MGC_M_TXCSR_ISO;
		}
#endif
		/* flush twice in case of double packet buffering */
		MGC_WriteCsr16(pBase, MGC_O_HSFC_TXCSR, bEnd, 
		    MGC_M_TXCSR_FLUSHFIFO | MGC_M_TXCSR_CLRDATATOG);
		MGC_WriteCsr16(pBase, MGC_O_HSFC_TXCSR, bEnd, 
		    MGC_M_TXCSR_FLUSHFIFO | MGC_M_TXCSR_CLRDATATOG);
		/* final CSR setup */
		MGC_WriteCsr16(pBase, MGC_O_HSFC_TXCSR, bEnd, csr);
	    }
	}
	else
	{
	    /* receive */
	    /* CSR */
	    if(bEnd)
	    {
		/* packet size */
		MGC_WriteCsr16(pBase, MGC_O_HSFC_RXMAXP, bEnd, pEnd->wRxPacketSize);
#ifdef MUSB_ISO
		if(pEnd->bTrafficType == MUSB_ENDPOINT_XFER_ISOC)
		{
		    csr = MGC_M_RXCSR_P_ISO;
		}
#endif
		/* twice in case of double packet buffering */
		MGC_WriteCsr16(pBase, MGC_O_HSFC_RXCSR, bEnd, 
		    MGC_M_RXCSR_FLUSHFIFO | MGC_M_RXCSR_CLRDATATOG);
		MGC_WriteCsr16(pBase, MGC_O_HSFC_RXCSR, bEnd, 
		    MGC_M_RXCSR_FLUSHFIFO | MGC_M_RXCSR_CLRDATATOG);
		if(MUSB_ENDPOINT_XFER_INT == pEnd->bRxTrafficType)
		{
		    csr |= MGC_M_RXCSR_DISNYET;
		}
		csr |= MGC_M_RXCSR_AUTOCLEAR;
		MGC_WriteCsr16(pBase, MGC_O_HSFC_RXCSR, bEnd, csr);
	    }
	}
    }
    return pEnd;
}

/*
* Program the HSFC to initiate reception.
*/
uint32_t MGC_HsfcStartRx(MGC_Port* pPort, MGC_EndpointResource* pEnd, 
			 uint8_t* pBuffer, uint32_t dwTotalBytes,
			 void* pIrp, uint8_t bAllowDma)
{
#ifdef MUSB_DMA
    MUSB_DmaController* pDmaController;
    MUSB_DmaChannel* pDmaChannel;
    uint8_t bDmaOk;
#endif
    uint16_t wCsr, wRxCount, wIntrRxE;
    MGC_BsrItem qItem;
    uint8_t bResult;
    MGC_Controller* pController = pPort->pController;
    uint8_t* pBase = (uint8_t*)pController->pControllerAddressIst;
    MUSB_SystemServices* pServices = pController->pSystemServices;
    uint8_t bEnd = pEnd->bLocalEnd;

    MGC_DIAG(3, pPort->pController, "[StartRx");

    MGC_SelectEnd(pBase, bEnd);

    /* clear mode bit in TxCSR */
    wCsr = MGC_ReadCsr16(pBase, MGC_O_HSFC_TXCSR, bEnd);
    if((wCsr & MGC_M_TXCSR_MODE) && pEnd->bIsFifoShared)
    {
	wCsr &= ~MGC_M_TXCSR_MODE;
	MGC_WriteCsr16(pBase, MGC_O_HSFC_TXCSR, bEnd, wCsr);
    }

    /* read RxCSR for preparation */
    wCsr = MGC_ReadCsr16(pBase, MGC_O_HSFC_RXCSR, bEnd);

    /* prepare for the non-DMA case */
    wCsr &= ~(MGC_M_RXCSR_DMAENAB | MGC_M_RXCSR_DMAMODE);

#if 0
    /* doesn't work reliably */
    if((MUSB_ENDPOINT_XFER_BULK == pEnd->bTrafficType) && pPort->bHasBulkCombine)
    {
	/* bulk combining: set up for IRP's request size */
	MGC_WriteCsr16(pBase, MGC_O_HSFC_RXMAXP, bEnd, 
	    pEnd->wRxPacketSize | 
	    ((MGC_MIN(pEnd->wMaxPacketSizeRx, dwTotalBytes) / pEnd->wRxPacketSize) - 1) << 11);
    }
    else
    {
#endif
	/* normal case: set up for packet size */
	MGC_WriteCsr16(pBase, MGC_O_HSFC_RXMAXP, bEnd, pEnd->wRxPacketSize);
#if 0
    }
#endif

    pEnd->bBsrResidual = pEnd->bIsrResidual;

    /* disable endpoint interrupt */
    wIntrRxE = MGC_Read16(pBase, MGC_O_HSFC_INTRRXE);
    MGC_Write16(pBase, MGC_O_HSFC_INTRRXE, wIntrRxE & ~(1 << bEnd));

    /* handle residual if any */
    if(MGC_ReadCsr16(pBase, MGC_O_HSFC_RXCSR, bEnd) & MGC_M_RXCSR_RXPKTRDY)
    {
	/* poll until IRP is complete, since handing it to the ISR will always have races */
	while(TRUE)
	{
	    if(wCsr & MGC_M_RXCSR_RXPKTRDY)
	    {
		wRxCount = MGC_ReadCsr16(pBase, MGC_O_HSFC_RXCOUNT, bEnd);
		MUSB_DIAG1(3, "StartRx: residual byte count=", wRxCount, 16, 0);
#ifdef MUSB_DMA
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
			pEnd->pRxIrp = pIrp;

			bDmaOk = pDmaController->pfDmaProgramChannel(pDmaChannel, 
			    pEnd->wRxPacketSize, 0, pBuffer, 
			    MGC_MIN(dwTotalBytes, pDmaChannel->dwMaxLength));
			if(bDmaOk)
			{
			    if(pEnd->pRxIrp)
			    {
				/* external DMAC; need to assert request line */
				wCsr = MGC_ReadCsr16(pBase, MGC_O_HSFC_RXCSR, bEnd);
				wCsr |= (MGC_M_RXCSR_AUTOCLEAR | MGC_M_RXCSR_DMAENAB);
				/* this special sequence is required to get DMAReq to activate */
				MGC_WriteCsr8(pBase, (MGC_O_HSFC_RXCSR+1), bEnd, 
				    (uint8_t)((wCsr | MGC_M_RXCSR_DMAMODE) >> 8));
				MGC_WriteCsr8(pBase, (MGC_O_HSFC_RXCSR+1), bEnd, 
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
			    pEnd->pDmaChannel = NULL;
			}
		    }
		}
#endif
		bResult = MGC_PipePacketReceived(pPort, pEnd, wRxCount, 
		    FALSE, pIrp);

		/* clear recv condition if necessary */
		wCsr &= ~MGC_M_RXCSR_RXPKTRDY;
		if((wRxCount < pEnd->wRxPacketSize) || !(wCsr & MGC_M_RXCSR_AUTOCLEAR))
		{
		    MGC_WriteCsr16(pBase, MGC_O_HSFC_RXCSR, bEnd, wCsr);
		}
		/* complete IRP if necessary */
		if(bResult)
		{
		    if(MGC_CompleteIrp(&qItem, pEnd, (uint8_t)MUSB_STATUS_OK, FALSE, pIrp))
		    {
			VERIFY(pServices->pfQueueBackgroundItem(pServices->pPrivateData, &qItem));
		    }
		    /* re-enable interrupt */
		    MGC_Write16(pBase, MGC_O_HSFC_INTRRXE, wIntrRxE);
		    MGC_DIAG(3, pPort->pController, "IRP Complete - StartRx]");
		    return 0;
		}
	    }

	    /* in the non-DMA case we may be slow enough that another packet is ready now */
	    wCsr = MGC_ReadCsr16(pBase, MGC_O_HSFC_RXCSR, bEnd);
	}

    }

    /* no residual; set current IRP */
    pEnd->pRxIrp = pIrp;

#ifdef MUSB_DMA
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
		pDmaController->pPrivateData, bEnd, FALSE, 
		pEnd->bRxTrafficType, pEnd->wRxPacketSize);
	}
	if(pDmaChannel)
	{
	    pEnd->bDmaTx = FALSE;
	    pDmaChannel->dwActualLength = 0L;
	    bDmaOk = pDmaController->pfDmaProgramChannel(pDmaChannel, 
		pEnd->wRxPacketSize, pDmaChannel->bDesiredMode, pBuffer, 
		MGC_MIN(dwTotalBytes, pDmaChannel->dwMaxLength));
	    if(bDmaOk)
	    {
		wCsr |= (MGC_M_RXCSR_AUTOCLEAR | MGC_M_RXCSR_DMAENAB | 
		    (pDmaChannel->bDesiredMode ? MGC_M_RXCSR_DMAMODE : 0));
	    }
	    else
	    {
		pDmaController->pfDmaReleaseChannel(pDmaChannel);
		pEnd->pDmaChannel = NULL;
	    }
	}
    }
#endif

#ifdef MUSB_DMA
    if(bAllowDma && pDmaChannel && bDmaOk)
    {
	/* write RxCSR */
	MGC_WriteCsr16(pBase, MGC_O_HSFC_RXCSR, bEnd, wCsr);

	MGC_DIAG(3, pPort->pController, "StartRx]");
	return 0;
    }
#endif

    /* re-enable interrupt */
    MGC_Write16(pBase, MGC_O_HSFC_INTRRXE, wIntrRxE);

    /* write RxCSR */
    MGC_WriteCsr16(pBase, MGC_O_HSFC_RXCSR, bEnd, wCsr);

    MGC_DIAG(3, pPort->pController, "StartRx]");

    return 0;
}

/*
* Program the HSFC to initiate transmit.
*/
uint32_t MGC_HsfcStartTx(MGC_Port* pPort, MGC_EndpointResource* pEnd, 
			 const uint8_t* pBuffer, uint32_t dwTotalBytes, void* pGenIrp)
{
#ifdef MUSB_DMA
    MUSB_DmaController* pDmaController;
    MUSB_DmaChannel* pDmaChannel;
    uint8_t bDmaOk;
#endif
    uint32_t dwLoadCount;
    uint16_t wCsr = 0;
    MGC_Controller* pController = pPort->pController;
#ifdef MUSB_DMA
    uint8_t bAllowDma = FALSE;
    MUSB_Irp* pIrp = NULL;
    MUSB_IsochIrp* pIsochIrp = NULL;
    MUSB_ControlIrp* pControlIrp = NULL;
    MUSB_SystemServices* pServices = pController->pSystemServices;
#endif
    uint8_t* pBase = (uint8_t*)pController->pControllerAddressIst;
    uint8_t bEnd = pEnd->bLocalEnd;

    MGC_SelectEnd(pBase, bEnd);

    /* determine how much to load into FIFO for non-DMA case */
     dwLoadCount = MGC_MIN(pEnd->wPacketSize, dwTotalBytes);

    if(bEnd)
    {
	/* read CSR for preparation */
        wCsr = MGC_ReadCsr16(pBase, MGC_O_HSFC_TXCSR, bEnd) & ~MGC_M_TXCSR_P_UNDERRUN;
    }
    else
    {
	/* ensure FIFO ready for writing */
	MGC_WriteCsr16(pBase, MGC_O_HSFC_CSR0, 0, 0);
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
		    wCsr |= (MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB);
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
    pPort->pfLoadFifo(pPort, pEnd->bLocalEnd, dwLoadCount, pBuffer);
    pEnd->dwTxSize = dwLoadCount;
    /* prepare CSR */
    if(bEnd)
    {
	wCsr &= ~(MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB);
	wCsr |= MGC_M_TXCSR_TXPKTRDY;
    }
#ifdef MUSB_DMA
    }
#endif

    /* write CSR */
    pEnd->pTxIrp = pGenIrp;
    if(bEnd)
    {
	MGC_WriteCsr16(pBase, MGC_O_HSFC_TXCSR, bEnd, wCsr | MGC_M_TXCSR_MODE);
    }
    else
    {
	MGC_WriteCsr16(pBase, MGC_O_HSFC_CSR0, 0, wCsr);
    }

    return 0;
}

/* Program the HSFC to flush the given endpoint wrt the given USB direction */
uint32_t MGC_HsfcFlushEndpoint(MGC_Port* pPort, MGC_EndpointResource* pEnd, 
			       uint8_t bDir, uint8_t bReuse)
{
    uint16_t wCsr;
    MGC_Controller* pController = pPort->pController;
    uint8_t* pBase = (uint8_t*)pController->pControllerAddressIst;
    uint8_t bTx = FALSE;
    uint8_t bEnd = pEnd->bLocalEnd;

    /* select hw ep */
    MGC_SelectEnd(pBase, bEnd);

    /* determine TX/RX */
    bTx = (bDir & MUSB_DIR_IN) ? TRUE : FALSE;

    if(bTx)
    {
	MGC_DIAG1(2, pController, "Flushing Tx on ", bEnd, 16, 2);
	/*  flush the xmit fifo, reset the data tog to 0 */
	wCsr = MGC_ReadCsr16(pBase, MGC_O_HSFC_TXCSR, bEnd);
	MGC_WriteCsr16(pBase, MGC_O_HSFC_TXCSR, bEnd,
	    wCsr | (MGC_M_TXCSR_FLUSHFIFO | MGC_M_TXCSR_CLRDATATOG));
	MGC_WriteCsr16(pBase, MGC_O_HSFC_TXCSR, bEnd, wCsr | MGC_M_TXCSR_FLUSHFIFO);
    }
    else
    {
	MGC_DIAG1(2, pController, "Flushing Rx on ", bEnd, 16, 2);
	/*  flush the recv fifo, reset the data tog to 0 */
	wCsr = MGC_ReadCsr16(pBase, MGC_O_HSFC_RXCSR, bEnd);
	MGC_WriteCsr16(pBase, MGC_O_HSFC_RXCSR, bEnd,
	    wCsr | (MGC_M_RXCSR_FLUSHFIFO | MGC_M_RXCSR_CLRDATATOG));
	MGC_WriteCsr16(pBase, MGC_O_HSFC_RXCSR, bEnd, wCsr | MGC_M_RXCSR_FLUSHFIFO);
    }

#ifdef MUSB_C_DYNFIFO_DEF
    if(bReuse)
    {
	pEnd->wMaxPacketSizeTx = pEnd->wMaxPacketSizeRx = 0;
    }
#endif

    return 0;
}

/* Program the HSFC to set/clear the halting (stall) of an endpoint */
uint32_t MGC_HsfcHaltEndpoint(MGC_Port* pPort, MGC_EndpointResource* pEnd, 
			      uint8_t bDir, uint8_t bHalt)
{
    uint16_t val;
    uint8_t bTx = FALSE;
    MGC_Controller* pController = pPort->pController;
    uint8_t* pBase = (uint8_t*)pController->pControllerAddressIst;

    /* determine TX/RX */
    bTx = (bDir & MUSB_DIR_IN) ? TRUE : FALSE;

    MGC_SelectEnd(pBase, pEnd->bLocalEnd);
    
    if(bTx)
    {
	/* Tx */
	val = MGC_ReadCsr16(pBase, MGC_O_HSFC_TXCSR, pEnd->bLocalEnd);
	if(bHalt)
	{
		val &= ~(MGC_M_TXCSR_P_UNDERRUN | MGC_M_TXCSR_P_INCOMPTX);
		val |= MGC_M_TXCSR_P_SENDSTALL;
	}
	else
	{
		val &= ~(MGC_M_TXCSR_P_SENDSTALL | MGC_M_TXCSR_P_UNDERRUN | MGC_M_TXCSR_P_INCOMPTX);
		/* apparently the hardware is not doing this: */
		val |= MGC_M_TXCSR_CLRDATATOG;
	}
	MGC_WriteCsr16(pBase, MGC_O_HSFC_TXCSR, pEnd->bLocalEnd, val);
    }
    else
    {
	/* Rx */
	val = MGC_ReadCsr16(pBase, MGC_O_HSFC_RXCSR, pEnd->bLocalEnd);
	if(bHalt)
	{
		val |= MGC_M_RXCSR_P_SENDSTALL;
	}
	else
	{
		val &= ~MGC_M_RXCSR_P_SENDSTALL;
		/* apparently the hardware is not doing this: */
		val |= MGC_M_RXCSR_CLRDATATOG;
	}
	MGC_WriteCsr16(pBase, MGC_O_HSFC_RXCSR, pEnd->bLocalEnd, val);
    }

    return 0;
}

/*
* Respond on default endpoint
*/
uint32_t MGC_HsfcDefaultEndResponse(MGC_Port* pPort, uint8_t bStall)
{
    uint16_t wSize, wCsrVal;
    uint8_t bVal = MGC_M_CSR0_P_SVDRXPKTRDY;
    MGC_Controller* pController = pPort->pController;
    uint8_t* pBase = (uint8_t*)pController->pControllerAddressIst;

    MGC_SelectEnd(pBase, 0);
    wCsrVal = MGC_ReadCsr16(pBase, MGC_O_HSFC_CSR0, 0);

    if(wCsrVal & MGC_M_CSR0_P_SETUPEND)
    {
	bVal |= MGC_M_CSR0_P_SVDSETUPEND;
    }
    /* prepare stall response if required */
    if(bStall)
    {
	bVal |= MGC_M_CSR0_P_SENDSTALL;
    }
    else if(pPort->wSetupDataSize)
    {
	/* we need to transmit; ack RxPktRdy BEFORE loading FIFO */
	MGC_WriteCsr8(pBase, MGC_O_HSFC_CSR0, 0, bVal);
	bVal = 0;
	/* start transmit */
	wSize = MUSB_MIN(64, pPort->wSetupDataSize);
	MGC_HsfcLoadFifo(pPort, 0, wSize, pPort->pSetupData);
	pPort->wSetupDataIndex = wSize;
	bVal |= MGC_M_CSR0_TXPKTRDY;
	if(wSize == pPort->wSetupDataSize)
	{
	    bVal |= MGC_M_CSR0_P_DATAEND;
	    pPort->bEnd0Status = TRUE;
	    pPort->wSetupDataIndex = 0;
	    pPort->wSetupDataSize = 0;
	}
    }
    else
    {
	/* just acknowledge end of data */
	bVal |= MGC_M_CSR0_P_DATAEND;
	pPort->bEnd0Status = TRUE;
    }

    /* write CSR0 */
    MGC_DIAG1(3, pPort->pController, "DefaultEndResponse: Setting CSR0 to ", bVal, 16, 2);
    MGC_WriteCsr8(pBase, MGC_O_HSFC_CSR0, 0, bVal);

    return 0;
}

/*
 * Handle default endpoint interrupt
 */
uint8_t MGC_HsfcServiceDefaultEnd(MGC_Port* pPort, MGC_BsrItem* pItem)
{
    uint8_t bTestVal;
    uint16_t wCsrVal, wCount;
    uint8_t bError = FALSE;
    uint8_t bResult = FALSE;
    uint8_t* pBase = (uint8_t*)pPort->pController->pControllerAddressIsr;

    MGC_SelectEnd(pBase, 0);
    wCsrVal = MGC_ReadCsr16(pBase, MGC_O_HSFC_CSR0, 0);
    wCount = MGC_ReadCsr8(pBase, MGC_O_HSFC_COUNT0, 0);
    MGC_DIAG1(3, pPort->pController, "CSR0=", wCsrVal, 16, 2);
    MGC_DIAG1(3, pPort->pController, "Count0=", wCount, 16, 2);

    if(wCsrVal & MGC_M_CSR0_P_SENTSTALL)
    {
	bError = TRUE;
	wCsrVal &= ~MGC_M_CSR0_P_SENTSTALL;
	MGC_WriteCsr16(pBase, MGC_O_HSFC_CSR0, 0, wCsrVal);
    }
    else if(wCsrVal & MGC_M_CSR0_P_SETUPEND)
    {
	pPort->wSetupDataIndex = 0;
	pPort->wSetupDataSize = 0;
    }
    else if(!wCount && pPort->bEnd0Status)
    {
	/* this is the "acknowledge" interrupt */
	pPort->bEnd0Status = FALSE;
	MGC_WriteCsr16(pBase, MGC_O_HSFC_CSR0, 0, 0);

	/* set address if required */
	if(pPort->bSetAddress)
	{
	    MGC_HsfcProgramBusState(pPort);
	    pPort->bSetAddress = FALSE;
	}

	/* set TESTMODE register if required */
	if(pPort->bWantTestMode)
	{
	    pPort->bWantTestMode = FALSE;
	    bTestVal = 0;
	    switch(pPort->bTestMode)
	    {
	    case MUSB_TESTMODE_TEST_J:
		bTestVal = MGC_M_TEST_J;
		break;
	    case MUSB_TESTMODE_TEST_K:
		bTestVal = MGC_M_TEST_K;
		break;
	    case MUSB_TESTMODE_TEST_SE0_NAK:
		bTestVal = MGC_M_TEST_SE0_NAK;
		break;
	    case MUSB_TESTMODE_TEST_PACKET:
		bTestVal = MGC_M_TEST_PACKET;
		MGC_HsfcLoadFifo(pPort, 0, sizeof(MGC_aTestPacket), 
		    MGC_aTestPacket);
		/* despite explicit instruction, we still must kick-start */
		MGC_WriteCsr16(pBase, MGC_O_HSFC_CSR0, 0, MGC_M_CSR0_TXPKTRDY);
		break;
	    }
	    MGC_Write8(pBase, MGC_O_HSFC_TESTMODE, bTestVal);
	}

	return FALSE;
    }

    if(!bError)
    {
	/* call common logic */
	bResult = MGC_FcServiceDefaultEnd(pPort, pItem, &wCsrVal, wCount);

	/* write CSR0 */
	if(wCsrVal)
	{
	    MGC_DIAG1(3, pPort->pController, "ServiceDefaultEnd: Setting CSR0 to ", wCsrVal, 16, 2);
	    MGC_WriteCsr16(pBase, MGC_O_HSFC_CSR0, 0, wCsrVal);
	}
    }

    return bResult;
}

uint8_t MGC_HsfcServiceTransmitAvail(MGC_Port* pPort, uint16_t wEndIndex, MGC_BsrItem* pItem)
{
    uint16_t wVal;
    MGC_EndpointResource* pEnd;
    uint8_t bResult = FALSE;
    uint32_t status = MUSB_STATUS_OK;
    uint8_t bEnd = (uint8_t)wEndIndex;
    MGC_Controller* pController = pPort->pController;
    uint8_t* pBase = (uint8_t*)pController->pControllerAddressIst;

    MGC_SelectEnd(pBase, bEnd);
    wVal = MGC_ReadCsr16(pBase, MGC_O_HSFC_TXCSR, bEnd);

    pEnd = (MGC_EndpointResource*)MUSB_ArrayFetch(&(pPort->LocalEnds), 
	wEndIndex);
    do
    {
	if(wVal & MGC_M_TXCSR_P_SENTSTALL)
	{
	    wVal &= ~MGC_M_TXCSR_P_SENTSTALL;
	    MGC_WriteCsr16(pBase, MGC_O_HSFC_TXCSR, bEnd, wVal);
	    break;
	}
	if(wVal & MGC_M_TXCSR_P_UNDERRUN)
	{
	    wVal &= ~MGC_M_TXCSR_P_UNDERRUN;
	    MGC_WriteCsr16(pBase, MGC_O_HSFC_TXCSR, bEnd, wVal);
	}

	if(pEnd)
	{
#ifdef MUSB_DMA
	    if(pEnd->pDmaChannel)
	    {
		if(MUSB_DMA_STATUS_FREE == 
		    pPort->pDmaController->pfDmaGetChannelStatus(pEnd->pDmaChannel))
		{
		    pEnd->dwTxOffset = pEnd->pDmaChannel->dwActualLength;
		}
	    }
	    else
#endif
		pEnd->dwTxOffset += pEnd->dwTxSize;
	    if(pEnd->pTxIrp)
	    {
		bResult = MGC_PipeTransmitReady(pPort, pEnd);
		if(bResult)
		{
#ifdef MUSB_DMA
		    if(pEnd->pDmaChannel)
		    {
			pPort->pDmaController->pfDmaReleaseChannel(pEnd->pDmaChannel);
			pEnd->pDmaChannel = NULL;
		    }
#endif
		    bResult = MGC_CompleteIrp(pItem, pEnd, (uint8_t)status, TRUE, pEnd->pTxIrp);
		}
	    }
	}
    } while(FALSE);

    return bResult;
}

uint8_t MGC_HsfcServiceReceiveReady(MGC_Port* pPort, uint16_t wEndIndex, MGC_BsrItem* pItem)
{
    uint16_t wVal;
    MGC_EndpointResource* pEnd;
    uint16_t wRxCount;
    uint8_t bEnd = (uint8_t)wEndIndex;
    uint8_t bResult = FALSE;
    uint32_t status = MUSB_STATUS_OK;
    MGC_Controller* pController = pPort->pController;
    uint8_t* pBase = (uint8_t*)pController->pControllerAddressIst;

    MGC_SelectEnd(pBase, bEnd);
    wVal = MGC_ReadCsr16(pBase, MGC_O_HSFC_RXCSR, bEnd);

    pEnd = (MGC_EndpointResource*)MUSB_ArrayFetch(&(pPort->LocalEnds), 
	wEndIndex);
    do
    {
	if(wVal & MGC_M_RXCSR_P_SENTSTALL)
	{
	    wVal &= ~MGC_M_RXCSR_P_SENTSTALL;
	    MGC_WriteCsr16(pBase, MGC_O_HSFC_RXCSR, bEnd, wVal);
	    break;
	}
	if(wVal & MGC_M_RXCSR_P_OVERRUN)
	{
	    wVal &= ~MGC_M_RXCSR_P_OVERRUN;
	    MGC_WriteCsr16(pBase, MGC_O_HSFC_RXCSR, bEnd, wVal);
	    break;
	}
	/*
	*  Fresh packet received
	*/
	if(!pEnd || pEnd->bIsHalted)
	{
	    /* couldn't find endpoint or it is halted */
	    status = pEnd ? 0 : MUSB_STATUS_INTERNAL_ERROR;
	    MGC_WriteCsr16(pBase, MGC_O_HSFC_RXCSR, bEnd, MGC_M_RXCSR_P_SENDSTALL);
	    break;
	}
	if(pEnd && pEnd->pRxIrp)
	{
	    /* we are expecting traffic */
	    while(pEnd->pRxIrp &&
		((pEnd->bBsrResidual != pEnd->bIsrResidual) || (wVal & MGC_M_RXCSR_RXPKTRDY)))
	    {
		if(pEnd->bBsrResidual != pEnd->bIsrResidual)
		{
		    pEnd->bBsrResidual++;
		}
#ifdef MUSB_DMA
		/* if we were using DMA, update actual byte count */
		if(pEnd->pDmaChannel)
		{
		    if(MUSB_DMA_STATUS_FREE == 
			pPort->pDmaController->pfDmaGetChannelStatus(pEnd->pDmaChannel))
		    {
			pEnd->dwRxOffset = pEnd->pDmaChannel->dwActualLength;
			pEnd->bDmaResidual = FALSE;
		    }
		}
		if(pEnd->bDmaResidual)
		{
		    return FALSE;
		}
#endif
		/* read FIFO count and see if current IRP is satisfied */
		wRxCount = MGC_ReadCsr16(pBase, MGC_O_HSFC_RXCOUNT, bEnd);
		bResult = MGC_PipePacketReceived(pPort, pEnd, wRxCount, 
		    FALSE, pEnd->pRxIrp);
		/* re-read CSR */
		wVal = MGC_ReadCsr16(pBase, MGC_O_HSFC_RXCSR, bEnd);
		/* clear recv condition if necessary */
		if((wRxCount < pEnd->wRxPacketSize) || !(wVal & MGC_M_RXCSR_AUTOCLEAR))
		{
		    wVal &= ~MGC_M_RXCSR_RXPKTRDY;
		    MGC_WriteCsr16(pBase, MGC_O_HSFC_RXCSR, bEnd, wVal);
		}
		if(bResult)
		{
		    /* current IRP is satisfied */
#ifdef MUSB_DMA
		    if(pEnd->pDmaChannel)
		    {
			pPort->pDmaController->pfDmaReleaseChannel(pEnd->pDmaChannel);
			pEnd->pDmaChannel = NULL;
		    }
#endif
		    bResult = MGC_CompleteIrp(pItem, pEnd, (uint8_t)status, FALSE, pEnd->pRxIrp);

		    /* clear DMA bits */
		    MGC_WriteCsr8(pBase, (MGC_O_HSFC_RXCSR+1), bEnd, 
			(uint8_t)((wVal & ~(MGC_M_RXCSR_DMAENAB | MGC_M_RXCSR_DMAMODE)) >> 8));

		}
	    }
	}
	else
	{
	    pEnd->bIsrResidual++;
	}
    } while(FALSE);

    return bResult;
}

void MGC_HsfcSetPortTestMode(MGC_Port* pPort, MUSB_HsetPortMode eMode)
{
}

void MGC_HsfcLoadFifo(MGC_Port* pPort, uint8_t bEnd, uint32_t dwCount, const uint8_t* pSource)
{
    uint32_t dwIndex, dwIndex32;
    uint32_t dwDatum;
    uint32_t dwCount32 = ((uint32_t)pSource & 3) ? 0 : (dwCount >> 2);
    MGC_Controller* pController = pPort->pController;
    uint8_t* pBase = (uint8_t*)pController->pControllerAddressIst;
    uint8_t bFifoOffset = MGC_FIFO_OFFSET(bEnd);

    /* doublewords when possible */
    for(dwIndex = dwIndex32 = 0; dwIndex32 < dwCount32; dwIndex32++, dwIndex += 4)
    {
	dwDatum = *((uint32_t*)&(pSource[dwIndex]));
	MGC_Write32(pBase, bFifoOffset, dwDatum);
    }
    while(dwIndex < dwCount)
    {
	MGC_Write8(pBase, bFifoOffset, pSource[dwIndex++]);
    }

    return;
}

void MGC_HsfcUnloadFifo(MGC_Port* pPort, uint8_t bEnd, uint32_t dwCount, uint8_t* pDest)
{
    uint32_t dwIndex, dwIndex32;
    uint32_t dwCount32 = ((uint32_t)pDest & 3) ? 0 : (dwCount >> 2);
    MGC_Controller* pController = pPort->pController;
    uint8_t* pBase = (uint8_t*)pController->pControllerAddressIst;
    uint8_t bFifoOffset = MGC_FIFO_OFFSET(bEnd);

    /* doublewords when possible */
    for(dwIndex = dwIndex32 = 0; dwIndex32 < dwCount32; dwIndex32++, dwIndex += 4)
    {
	*((uint32_t*)&(pDest[dwIndex])) = MGC_Read32(pBase, bFifoOffset);
    }
    while(dwIndex < dwCount)
    {
	pDest[dwIndex++] = MGC_Read8(pBase, bFifoOffset);
    }

    return;
}

uint8_t MGC_HsfcDmaChannelStatusChanged(
    void* pPrivateData, uint8_t bLocalEnd, uint8_t bTransmit)
{
#ifdef MUSB_DMA
    MGC_BsrItem qItem;
    uint8_t bQueue;
    MGC_EndpointResource* pEnd;
    uint16_t wVal, wRxCount, wIntrRxE;
    uint32_t status = MUSB_STATUS_OK;
    MGC_Port* pPort = (MGC_Port*)pPrivateData;
    MGC_Controller* pController = pPort->pController;
    MUSB_SystemServices* pServices = pController->pSystemServices;
    uint8_t* pBase = (uint8_t*)pController->pControllerAddressIsr;
    /* save index */
    uint8_t bIndex = MGC_Read8(pBase, MGC_O_HSFC_INDEX);

    pEnd = (MGC_EndpointResource*)MUSB_ArrayFetch(&(pPort->LocalEnds), 
	bLocalEnd);

    /* TODO: something more specific might be better */
    if(bTransmit)
    {
	bQueue = MGC_HsfcServiceTransmitAvail(pPort, bLocalEnd, &qItem);
	if(bQueue)
	{
	    qItem.bLocalEnd = bLocalEnd;
	    VERIFY(pServices->pfQueueBackgroundItem(pServices->pPrivateData, &qItem));
	}
    }
    else
    {
	MGC_SelectEnd(pBase, bLocalEnd);
	wVal = MGC_ReadCsr16(pBase, MGC_O_HSFC_RXCSR, bLocalEnd);
	/* update actual byte count */
	if(pEnd->pDmaChannel)
	{
	    if(MUSB_DMA_STATUS_FREE == 
		pPort->pDmaController->pfDmaGetChannelStatus(pEnd->pDmaChannel))
	    {
		pEnd->dwRxOffset = pEnd->pDmaChannel->dwActualLength;
		pEnd->bDmaResidual = FALSE;
	    }
	}
	if(pEnd->bDmaResidual)
	{
	    return FALSE;
	}
	/* read FIFO count and see if current IRP is satisfied */
	wRxCount = MGC_ReadCsr16(pBase, MGC_O_HSFC_RXCOUNT, bLocalEnd);
	bQueue = MGC_PipePacketReceived(pPort, pEnd, wRxCount, 
	    FALSE, pEnd->pRxIrp);
	/* clear recv condition if necessary */
	if((wRxCount < pEnd->wRxPacketSize) || !(wVal & MGC_M_RXCSR_AUTOCLEAR))
	{
	    wVal &= ~MGC_M_RXCSR_RXPKTRDY;
	    MGC_WriteCsr16(pBase, MGC_O_HSFC_RXCSR, bEnd, wVal);
	}
	if(bQueue)
	{
	    /* current IRP is satisfied */
	    if(pEnd->pDmaChannel)
	    {
		pPort->pDmaController->pfDmaReleaseChannel(pEnd->pDmaChannel);
		pEnd->pDmaChannel = NULL;
	    }
	    bQueue = MGC_CompleteIrp(&qItem, pEnd, (uint8_t)status, FALSE, pEnd->pRxIrp);

	    /* clear DMA bits */
	    MGC_WriteCsr8(pBase, (MGC_O_HSFC_RXCSR+1), bLocalEnd, 
		(uint8_t)((wVal & ~(MGC_M_RXCSR_DMAENAB | MGC_M_RXCSR_DMAMODE)) >> 8));
	    /* enable endpoint interrupt */
	    wIntrRxE = MGC_Read16(pBase, MGC_O_HSFC_INTRRXE);
	    MGC_Write16(pBase, MGC_O_HSFC_INTRRXE, wIntrRxE | (1 << bLocalEnd));

	    qItem.bLocalEnd = bLocalEnd;
	    VERIFY(pServices->pfQueueBackgroundItem(pServices->pPrivateData, &qItem));
	}
    }
    pPort->bDmaCompleted = bQueue;

    /* restore index */
    MGC_Write8(pBase, MGC_O_HSFC_INDEX, bIndex);

    return bQueue;

#else

    return FALSE;

#endif
}

#ifdef MUSB_DIAG

/*
* Dump info global to the HSFC
*/
int32_t MGC_HsfcDumpState(MGC_Controller* pController)
{
    /* TODO */
    return 0;
}

/*
* Dump info relevant to the given local endpoint resource
*/
int32_t MGC_HsfcDumpEndpoint(MGC_Controller* pController, MUSB_EndpointResource* pEnd)
{
    /* TODO */
    return 0;
}

#endif	/* diagnostics enabled */

#endif	/* HSFC enabled */

