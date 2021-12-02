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
 * Implementation of hub-class-specific requests
 * $Revision: #1 $
 */

#ifdef MUSB_HUB

#include "mu_cdi.h"
#include "mu_diag.h"
#include "mu_bits.h"
#include "mu_mem.h"
#include "mu_descs.h"
#include "mu_hdef.h"
#include "mu_hub.h"
#include "mu_hcore.h"
#include "mu_hcreq.h"

/*******************************************************************
 *                 MUSB_HubGetStatus ()                            *
 *******************************************************************/

uint32_t MUSB_HubGetStatus(MUSB_Hub *pHubDevice, MUSB_pfControlIrpComplete complete, uint8_t bPortNum, uint8_t bmRequest)
{
    MUSB_ControlIrp *pCtrlIrp;

    uint32_t dwStatus = MUSB_FAILURE_UNSIGNED;
    uint16_t wLength = (uint16_t)MUSB_HUB_PORT_STATUS_LENGTH;

    MUSB_ASSERT(pHubDevice);
    pCtrlIrp = &(pHubDevice->ctrlIrp);

    MUSB_DIAG_STRING(3, "MUSB_HubGetStatus() >>");

    /* Port number always starts from one. If it is 0x00 it means request
     * ment for HUB not for port.
     */
    MUSB_HubPrepareSetupPkt(&(pHubDevice->setupPacket), bmRequest, MUSB_REQ_GET_STATUS, 0x00, (uint16_t)bPortNum, wLength);

    pCtrlIrp->pInBuffer = pHubDevice->aHubPortStatus;
    pCtrlIrp->dwInLength = MUSB_HUB_PORT_STATUS_LENGTH;
    pCtrlIrp->dwOutLength = MUSB_HUB_SETUP_PACKET_LENGTH;
    pCtrlIrp->pfIrpComplete = complete;

    dwStatus = MUSB_StartControlTransfer(pHubDevice->pUsbDevice->pPort, pCtrlIrp);

    if (dwStatus)
    {
        /* Print the status and then come aback.*/
        MUSB_DIAG_STRING(1, "HubError: MUSB_HubGetStatus() <<");
        return (MUSB_FAILURE_UNSIGNED);
    }

    MUSB_DIAG_STRING(3, "HubSuccess: MUSB_HubGetStatus() <<");
    return (MUSB_SUCCESS);
} /* End of function  MUSB_HubGetStatus ()  */

/*******************************************************************
 *                 MUSB_HubFeatureRequest ()                       *
 *******************************************************************/
uint32_t MUSB_HubFeatureRequest(MUSB_Hub *pHubDevice, uint16_t wFeatureSelector, MUSB_pfControlIrpComplete complete, uint8_t bPortNum, uint8_t bmRequest, uint8_t bRequest)
{
    MUSB_ControlIrp *pCtrlIrp;

    uint16_t wLength = MUSB_HUB_NO_DATA_PHASE;
    uint32_t dwStatus;

    MUSB_ASSERT(pHubDevice);
    pCtrlIrp = &(pHubDevice->ctrlIrp);

    MUSB_DIAG_STRING(3, "MUSB_HubFeatureRequest() >>");
    /* Port number always starts from 0x01. If it is 0x00 it means request
     * ment for HUB not for port.
     */
    MUSB_HubPrepareSetupPkt(&(pHubDevice->setupPacket), bmRequest, bRequest, wFeatureSelector, (uint16_t)bPortNum, wLength);

    pCtrlIrp->pInBuffer = NULL;
    pCtrlIrp->dwInLength = MUSB_HUB_NO_DATA_PHASE;
    pCtrlIrp->dwOutLength = MUSB_HUB_SETUP_PACKET_LENGTH;
    pCtrlIrp->pfIrpComplete = complete;

    dwStatus = MUSB_StartControlTransfer(pHubDevice->pUsbDevice->pPort, pCtrlIrp);

    if (dwStatus)
    {
        /* Print the status and then come aback.*/
        MUSB_DIAG_STRING(1, "HubError: MUSB_HubFeatureRequest() <<");
        return (MUSB_FAILURE);
    }

    MUSB_DIAG_STRING(3, "HubSuccess: MUSB_HubFeatureRequest(): <<");
    return (MUSB_SUCCESS);
} /* End of function  MUSB_HubFeatureRequest () */

#endif /* MUSB_HUB */
