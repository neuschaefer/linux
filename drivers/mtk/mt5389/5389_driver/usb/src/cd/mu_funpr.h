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

#ifndef __MUSB_FUNCTION_PROTO_H__
#define __MUSB_FUNCTION_PROTO_H__

/*
 * Prototypes for function-specific functions
 * $Revision: #1 $
 */

#include "mu_impl.h"

/**
 * Register a function client.
 * Register the given function client.
 * @param pPort port pointer
 * @param pClient client pointer
 * @return status code
 */
extern uint32_t MGC_FunctionRegisterClient(MGC_Port* pPort, 
					   MUSB_FunctionClient* pClient);

/**
 * Function cleanup.
 * Perform function-specific cleanup.
 * @param pPort port pointer
 * @return status code
 */
extern uint32_t MGC_FunctionDestroy(MGC_Port* pPort);

/**
 * Perform function-specific operations when the connection speed is known
 * @param pPort port pointer (speed should be set before call)
 */
extern void MGC_FunctionSpeedSet(MGC_Port* pPort);

/**
 * Parse/handle setup as function.
 * Parse a setup and possibly handle it based on descriptors
 * @param pPort port pointer (request and response buffers here)
 * @param pbStatus filled with status (e.g. MUSB_STATUS_STALL to stall request)
 * @return TRUE if handled
 * @return FALSE to call client
 */
extern uint8_t MGC_FunctionParseSetup(MGC_Port* pPort, uint8_t* pbStatus);

/**
 * Change USB state.
 * Change to a new USB state; invoking client callback.
 * @param pPort port pointer
 * @param State the new state
 */
extern void MGC_FunctionChangeState(MGC_Port* pPort, MUSB_State State);

/**
 * Prepare a function pipe.
 * Prepare a pipe for the given local endpoint.
 * @param pPort port pointer
 * @param pPipe pointer to pipe in which to fill fields
 * @param hBus bus handle for pipe
 * @param pResource local resource to assume for use
 * @param pEndpoint pointer to USB endpoint descriptor
 */
extern void MGC_FunctionPreparePipe(MGC_Port* pPort, MGC_Pipe* pPipe, 
				    MUSB_BusHandle hBus,
				    MGC_EndpointResource* pResource,
				    const MUSB_EndpointDescriptor* pEndpoint);

#endif	/* multiple inclusion protection */
