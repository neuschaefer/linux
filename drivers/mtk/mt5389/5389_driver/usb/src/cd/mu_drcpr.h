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
 * Dual-Role Controller (DRC) function prototypes.
 * $Revision: #1 $
 */

#ifndef __MUSB_DRC_PROTO_H__
#define __MUSB_DRC_PROTO_H__

/*
* Prototypes for DRC-specific functions (at least FDRC/HDRC)
*/

#include "mu_impl.h"

/*
 * Initialize (discover FIFO config, etc.)
 */
#ifndef MUSB_C_DYNFIFO_DEF
extern uint8_t MGC_DrcInit(MGC_Port* pPort);
#endif
/*
 * ISR
 */
extern int32_t MGC_DrcIsr(MGC_Controller* pController, uint8_t bIntrUsbValue,
		      uint16_t wIntrTxValue, uint16_t wIntrRxValue);

/**
 * Common USB ISR.
 * Handles common USB interrupts on DRCs (from IntrUSB)
 * @param pPort port poiner
 * @param bReg IntrUSB value
 * @return number of IST work items queued
 */
extern uint8_t MGC_DrcUsbIsr(MGC_Port* pPort, uint8_t bReg);

/**
 * Service default endpoint
 * @param pPort port pointer
 * @param pItem work queue item
 * @param pwCsr CSR0 value (in); what to write to CSR0 (out)
 * @param wCount COUNT0 value
 * @return TRUE if work was queued
 */
extern uint8_t MGC_DrcServiceDefaultEnd(MGC_Port* pPort, MGC_BsrItem* pItem, 
					uint16_t* pwCsr, uint16_t wCount);

/**
 * the non-interrupt-time processing for a DRC
 * @memo the DRC BSR
 * @param pParam thread parameter
 */
extern void MGC_DrcBsr(void* pParam);

/* Reset the port (e.g. unbind/flush all endpoints, etc.) */
extern void MGC_DrcResetPort(MGC_Port* pPort);

/**
 * Change the DRC to given OTG state
 * @memo change OTG state
 * @param toOtg the new state
 * @param pPort port pointer
 */
extern void MGC_DrcChangeOtgState(uint8_t toOtg, MGC_Port* pPort);

/**
 * Complete an OTG state transition
 * (allow suspend in peripheral mode and notify client)
 */
#ifdef MUSB_OTG_FUNC    
extern void MGC_CompleteOtgTransition(MGC_Port* pPort);
#endif
/**
 * Called upon OTG timer expiration
 * @memo OTG timer callback
 * @param pParam callback parameter
 * @param wTimer timer index
 */
#ifdef MUSB_OTG_FUNC 
extern void MGC_DrcOtgTimer(void* pParam, uint16_t wTimer);
#endif
/**
 * Find the DRC's first (host role) or required (function role) 
 * local endpoint resource to communicate with the given remote endpoint (host)
 * or to be identified with the given endpoint descriptor (function).
 * @memo bind a DRC endpoint
 * @param pPort port pointer
 * @param pUsbEnd device endpoint attributes pointer
 * @param pRequest requested local endpoint attributes
 * @param bBind TRUE to actually bind (reserve resources)
 * @param pbIsTx pointer to storage to indicate Tx (TRUE) or Rx (FALSE)
 * @return pointer on success
 * @return NULL on failure
 */
#ifdef MUSB_C_DYNFIFO_DEF
extern MGC_EndpointResource* MGC_DrcBindEndpoint(MGC_Port* pPort, 
						 const MUSB_DeviceEndpoint* pUsbEnd,
						 MUSB_EndpointResource* pRequest,
						 uint8_t bBind, uint8_t* pbIsTx);
#endif
/**
 * Service the default endpoint as host.
 * @memo service default endpoint (host)
 * @param pPort port pointer
 * @param wCount current byte count
 * @param status status code indicating errors, if any
 * @param pwCsrVal where to store new CSR0 value (only non-zero values written)
 * @return TRUE if more packets are required for this transaction;
 * FALSE otherwise
 */
extern uint8_t MGC_DrcServiceHostDefault(MGC_Port* pPort, uint16_t wCount, 
					 uint8_t status, uint16_t* pwCsrVal);

/**
 * Service the default endpoint as function.
 * @memo service default endpoint (function)
 * @param pPort port pointer
 * @param wCount current byte count
 * @param pStatus pointer to status code indicating errors, filled with
 * a successful status or STALL
 * @param pbPhase pointer to the source of the current interrupt (IN vs. OUT),
 * filled with direction for next phase (IN vs. OUT)
 * @return TRUE if more packets are required for this transaction;
 * FALSE otherwise
 */
extern uint8_t MGC_DrcServiceFunctionDefault(MGC_Port* pPort, uint16_t wCount, 
					  uint8_t* pStatus, uint8_t* pbPhase);

/**
 * Accept/reject device.
 * Check if device is acceptable; take appropriate action if not.
 * The enumerator calls this after gathering all configuration data
 * and attempting a driver match.
 * @param pPort port pointer
 * @param pDevice device pointer
 * @param pDriver pointer to driver that matched, or NULL if none
 * @return TRUE if device is acceptable
 * @return FALSE if not (and appropriate action taken)
 */
extern uint8_t MGC_DrcAcceptDevice(MGC_Port* pPort, MUSB_Device* pDevice,
				   const MUSB_DeviceDriver* pDriver);

extern void MGC_DrcBusReset(MGC_Port *pPort);

extern uint8_t MGC_DrcDmaChannelStatusChanged(
    void* pPrivateData, uint8_t bLocalEnd, uint8_t bTransmit);

#ifdef MUSB_DIAG
/*
* Dump info relevant to the given pipe
*/
extern int32_t MGC_DrcDumpPipe(MGC_Controller* pController, MGC_Pipe* pPipe);
#endif

#endif	/* multiple inclusion protection */
