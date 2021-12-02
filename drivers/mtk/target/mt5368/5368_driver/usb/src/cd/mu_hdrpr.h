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

#ifndef __MUSB_HDRC_PROTO_H__
#define __MUSB_HDRC_PROTO_H__

/*
 * Prototypes for HDRC-specific functions
 * $Revision: #1 $
 */

#include "mu_impl.h"

#include "mu_drcpr.h"

/*
 * Initialize (discover FIFO config, etc.)
 */
extern uint8_t MGC_HdrcInit(MGC_Port* pPort);

/**
 * Try to allocate a local FIFO appropriate to the given remote endpoint (host role)
 * or to be identified with the given endpoint descriptor (function role).
 * NOTE: although this behavior is common with the FDRC, 
 * the way the code is conditionally compiled has the effect that it can't
 * reside in a common file (unless always gets compiled, which is undesirable).
 * @param pPort port pointer
 * @param pUsbEnd device endpoint attributes pointer
 * @param bBind TRUE to actually bind (reserve resources)
 * @param pbIsTx pointer to storage to indicate Tx (TRUE) or Rx (FALSE)
 * @return pointer on success
 * @return NULL on failure
 */
extern MGC_EndpointResource* MGC_HdrcBindDynamicEndpoint(MGC_Port* pPort, 
							 const MUSB_DeviceEndpoint* pUsbEnd,
							 uint8_t bBind, uint8_t* pbIsTx);

/** HDRC ISR */
extern int32_t MGC_HdrcIsr(void* pParam);

/*
* Program the FDRC to start/stop (enable/disable interrupts, bus reset, etc.).
* This is where registers get twiddled; no place above.
*/
extern uint32_t MGC_HdrcStart(MGC_Controller* pController);
extern uint32_t MGC_HdrcStop(MGC_Controller* pController);

extern uint32_t MGC_HdrcDestroy(MGC_Controller* pController);

/*
* Read bus state
*/
extern uint32_t MGC_HdrcReadBusState(MGC_Port* pPort);

/* Program the bus state from the port (see MGC_Port) */
extern void MGC_HdrcProgramBusState(MGC_Port* pPort);

/*
* Find the first or best (host) or required (function) 
* local endpoint resource to communicate with the given remote endpoint (host)
* or to be identified with the given endpoint descriptor (function).
* This is where registers get twiddled; no place above.
*/
extern MGC_EndpointResource* MGC_HdrcBindEndpoint(MGC_Port* pPort, 
						  const MUSB_DeviceEndpoint* pUsbEnd,
						  MUSB_EndpointResource* pRequest,
						  uint8_t bBind);

/*
* Program the HDRC to initiate a transfer.
* This is where registers get twiddled; no place above.
*/
extern uint32_t MGC_HdrcStartRx(MGC_Port* pPort, MGC_EndpointResource* pEnd,
    uint8_t* pBuffer, uint32_t dwTotalBytes, void* pIrp, uint8_t bAllowDma);

extern uint32_t MGC_HdrcStartTx(MGC_Port* pPort, MGC_EndpointResource* pEnd,
    const uint8_t* pBuffer, uint32_t dwTotalBytes, void* pIrp);

/*
* Program the underlying port to flush the given endpoint resource,
* with respect to the given USB direction (IN or OUT).
* The resource should be ready for normal operation by return time.
*/
extern uint32_t MGC_HdrcFlushEndpoint(MGC_Port* pPort, MGC_EndpointResource* pEnd, 
				      uint8_t bDir, uint8_t bReuse);

/*
* Program the HDRC's given endpoint's stall state
* (TRUE to set halt; FALSE to clear halt)
*/
extern uint32_t MGC_HdrcHaltEndpoint(MGC_Port* pPort, MGC_EndpointResource* pEnd, 
				     uint8_t bDir, uint8_t bHalt);

#ifdef MUSB_FUNCTION
extern uint32_t MGC_HdrcDefaultEndResponse(MGC_Port* pPort, uint8_t bStall);
#endif
/*
* Interrupts
*/
extern uint8_t MGC_HdrcServiceDefaultEnd(MGC_Port* pPort, MGC_BsrItem* pItem);
extern uint8_t MGC_HdrcServiceTransmitAvail(MGC_Port* pPort, uint16_t wEndIndex, MGC_BsrItem* pItem);
extern uint8_t MGC_HdrcServiceReceiveReady(MGC_Port* pPort, uint16_t wEndIndex, MGC_BsrItem* pItem);

/*
* FIFOs
*/
extern void MGC_HdrcLoadFifo(MGC_Port* pPort, uint8_t bEnd, uint32_t dwCount, const uint8_t* pSource);
extern void MGC_HdrcUnloadFifo(MGC_Port* pPort, uint8_t bEnd, uint32_t dwCount, uint8_t* pDest);

#ifdef MUSB_EHSET
extern void MGC_HdrcSetPortTestMode(MGC_Port* pPort, MUSB_HsetPortMode eMode);
#endif
extern void MGC_HdrcDynamicFifoLocation(MGC_Port* pPort, 
					uint8_t bEnd, 
					uint8_t bIsTx,
					uint8_t bSet,
					uint32_t* pdwBase, 
					uint8_t* pbSize83, 
					uint8_t* pbDoubleBuffered);
/**
 * DMA channel status changed
 */
extern uint8_t MGC_HdrcDmaChannelStatusChanged(
    void* pPrivateData, uint8_t bLocalEnd, uint8_t bTransmit);

#ifdef MUSB_DIAG

/*
* Dump info global to the FDRC
*/
extern int32_t MGC_HdrcDumpState(MGC_Controller* pController);

/*
* Dump info relevant to the given local endpoint resource
*/
extern int32_t MGC_HdrcDumpEndpoint(MGC_Controller* pController, MUSB_EndpointResource* pEnd);

#endif	/* diagnostics enabled */

#endif	/* multiple inclusion protection */
