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
 * Hub feature discovery, configuration, interrupt IRP submission,
 * power and TT management functionality.
 * $Revision: #1 $
 */

#ifdef MUSB_HUB

#include "mu_bits.h"
#include "mu_cdi.h"
#include "mu_descs.h"
#include "mu_diag.h"
#include "mu_mem.h"

#include "mu_hdef.h"
#include "mu_hub.h"
#include "mu_hcore.h"
#include "mu_hcreq.h"

#include "mu_hapi.h"

#include "mu_impl.h"

/*****************************************************************************
 *              MGC_HubPowerTimerCallback ()                           *
 *****************************************************************************/
static void MGC_HubPowerTimerCallback(void *pParam, MUSB_BusHandle hBus)
{
    MUSB_Hub *pHubDevice;
    uint32_t dwStatus;

    MUSB_ASSERT(pParam);
    pHubDevice = (MUSB_Hub *)pParam;

    MUSB_DIAG_STRING(2, "HUB: MGC_HubPowerTimerCallback()");

    pHubDevice->bNextState = MUSB_HUB_STATE_GOT_HUB_STATUS;

    dwStatus = MUSB_HubGetHubStatus(pHubDevice, MGC_HubInitializeCallback);
    if (MUSB_SUCCESS != dwStatus)
    {
        MUSB_DIAG1(1, "HubError: MUSB_HubGetHubStatus() returned ", dwStatus, 16, 0);

        MUSB_DIAG_STRING(1, "HubError: Please disconnect hub");
        pHubDevice->bNextState = MUSB_HUB_STATE_DISCONNECT_DEVICE;
    }

    UNUSED(hBus);
    return;
} /* End of function  MGC_HubPowerOnGoodTimerCallback() */

/*******************************************************************
 *                 MGC_HubConnectHandler ()                        *
 *******************************************************************/

uint32_t MGC_HubConnectHandler(MUSB_Hub *pHubDevice, MUSB_Device *pUsbDevice, uint8_t bBusAddress,
                               MUSB_Port *pPort)
{
    MUSB_ControlIrp *pControlIrp;

    MUSB_DeviceRequest *pSetupPacket;
    uint8_t bConfigurationNumber;
    uint32_t dwStatus;

    MUSB_ASSERT(pHubDevice);
    MUSB_ASSERT(pUsbDevice);

    pControlIrp = &(pHubDevice->ctrlIrp);
    pSetupPacket = &(pHubDevice->setupPacket);

    MUSB_DIAG_STRING(2, "MGC_HubConnectHandler() >>");
    /* Hub can have only one configuration descriptor so always that configuration
     * is active configuration Number. 
     */
    bConfigurationNumber = pUsbDevice->apConfigDescriptors[0]->bConfigurationValue;

    /* Prepare control request for SET_CONFIG */
    MUSB_HubPrepareSetupPkt(pSetupPacket, (MUSB_DIR_OUT | MUSB_TYPE_STANDARD | MUSB_RECIP_DEVICE),
                            MUSB_REQ_SET_CONFIGURATION, bConfigurationNumber,
                            0x0000, MUSB_HUB_NO_DATA_PHASE);

    /* Initialize control IRP. Now onward whenewer driver wants to use
     * control IRP then it has to update only pInBuffer, pOutBuffer, 
     * dwInLength, dwOutLength and ofcource Callback as per requrement. 
     */
    MUSB_FILL_CTRL_IRP(pControlIrp, bBusAddress, pSetupPacket, MUSB_HUB_SETUP_PACKET_LENGTH, NULL,
                       MUSB_HUB_NO_IN_DATA_TRANSFER, MGC_HubInitializeCallback, (void *)pHubDevice);
    pControlIrp->pDevice = pUsbDevice;

    /* Start control Transfer.  */
    dwStatus = MUSB_StartControlTransfer(pPort, pControlIrp);

    if (dwStatus)
    {
        /* Print the status and then come aback.*/
        MUSB_DIAG_STRING(1, "HubError: MGC_HubConnectHandler()");

        MUSB_DIAG_STRING(1, "HubError: Please disconnect hub");
        return (MUSB_FAILURE);
    }

    MUSB_DIAG_STRING(3, "HubSuccess: MGC_HubConnectHandler() <<");

    UNUSED(bBusAddress);
    
    return (MUSB_SUCCESS);
} /* End of function MGC_HubConnectHandler () */

/*******************************************************************
 *                 MGC_HubInitializeCallback ()                    *
 *******************************************************************/
void MGC_HubInitializeCallback(void *pContext, MUSB_ControlIrp *pControlIrp)
{
    MUSB_Hub *pHubDevice;

    MUSB_Device *pUsbDevice;
    uint32_t dwStatus;
    uint8_t bNumOfPort;
    MGC_EnumerationData *pEnumData;
    MGC_Port* pPort;
    uint32_t dwPwrOn2PwrGood;

    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pControlIrp);

    pHubDevice = (MUSB_Hub *)pContext;
    pPort = (MGC_Port *)pHubDevice->busHandle;
    MUSB_ASSERT(pPort);
    pUsbDevice = pHubDevice->pUsbDevice;
    MUSB_ASSERT(pUsbDevice);

    if (pControlIrp->dwStatus)
    {
        /*  IRP is not executed properly. So can not go ahead. */
        LOG(1, "HubError: MGC_HubInitializeCallback() = 0x%X.\n",
                   pControlIrp->dwStatus);
        LOG(1, "HubState: %d.\n", pHubDevice->bState);
        
        //  MTK Notice: Max Liao, 2006/11/29.
        //  Try next command when stall. Some Hub stall RESET_TT cmd.
        if (pControlIrp->dwStatus != (uint32_t)MUSB_STATUS_STALLED)
        {
            return;
        }
    }

    pUsbDevice->pCurrentConfiguration = pUsbDevice->apConfigDescriptors[0];

    /* Since IRP is executed properly hence whatever was present in the bNextState 
     * before IRP execution will become bState. 
     */
    pHubDevice->bState = pHubDevice->bNextState;

    switch (pHubDevice->bState)
    {
    case MUSB_HUB_STATE_CONFIGURED:
        {
            /* Control Comes here it means SetConfig Command is executed properly
             * Now time to prepare RESET_TT if hub is high speed otherwise prepare
             * command to get Hub Descriptor. 
             */
            MUSB_DIAG_STRING(2, "Hub: MUSB_HUB_STATE_CONFIGURED");

            pHubDevice->bActivePortNum = MUSB_HUB_STARTING_PORT_NUMBER;

            dwStatus = MGC_HubValidateUpdateTtInfo(pHubDevice);

            if (MUSB_SUCCESS != dwStatus)
            {
                /* There is some problem in interface descriptor hence cannot
                 * proceed.
                 */
                 
                MUSB_DIAG_STRING(1, "HubError: Descriptor parsing failed");

                MUSB_DIAG_STRING(1, "HubError: Please disconnect hub");

                pUsbDevice->bSupport = MUSB_STATUS_CHILD_CONNECT_UNSUPPORT;
            
                pEnumData = &(pPort->EnumerationData);
                if (pEnumData->pfChildDeviceUpdate)
                {
                    pEnumData->pfChildDeviceUpdate(pUsbDevice->pParentUsbDevice,
                        pUsbDevice, MUSB_STATUS_CHILD_CONNECT_UNSUPPORT);
                }
                
                pHubDevice->bNextState = MUSB_HUB_STATE_DISCONNECT_DEVICE;                              
                return;
            }
            MUSB_HubPrepareSetupPkt(&(pHubDevice->setupPacket),
                                    (uint8_t)(MUSB_DIR_IN | MUSB_TYPE_CLASS | MUSB_RECIP_DEVICE),
                                    MUSB_REQ_GET_DESCRIPTOR,
                                    (uint16_t)(MUSB_DT_HUB << 8), 
                                    0x00, 
                                    sizeof(MUSB_HubDescriptor)/*MUSB_HUB_MAX_HUB_DSCR_SIZE*/);

            /* Update required field of control IRP for getting hub descriptor. */
            pControlIrp->pInBuffer = (uint8_t *)&pHubDevice->hubDscr;
            pControlIrp->dwInLength = sizeof(MUSB_HubDescriptor);

            pHubDevice->bNextState = MUSB_HUB_STATE_GOT_HUB_DSCR;
            dwStatus = MUSB_StartControlTransfer(pUsbDevice->pPort, pControlIrp);

            if (dwStatus)
            {
                /* Not able to submit control IRP. You are gone. */
                MUSB_DIAG_STRING(1,
                                 "HubError: MUSB_HUB_STATE_RESET_TT_COMPLETE: StartControlTransfer()");
                MUSB_DIAG_STRING(1, "HubError: Please disconnect hub");
            }

            break;
        } /* End of case  MUSB_HUB_STATE_RESET_TT_COMPLETE */

    case MUSB_HUB_STATE_GOT_HUB_DSCR:
        {
            /* If control comes here it means hub descriptor has come. Store it in proper
             * format and Enable all ports of HUB through setFeature(PORT_POWER) command.
             */

            MUSB_DIAG_STRING(2, "Hub: MUSB_HUB_STATE_GOT_HUB_DSCR");

            bNumOfPort = pHubDevice->hubDscr.bNbrPorts;

            LOG(5, "Num of ports = %d.\n ", bNumOfPort);
            LOG(5, "HubCharacteristics = 0x%X, Compound Device = %s.\n ", 
                pHubDevice->hubDscr.wHubCharacteristics, 
                ((pHubDevice->hubDscr.wHubCharacteristics & 
                   MUSB_HUB_COMPOUND_DEVICE) ? "Yes": "No"));
            
            if (bNumOfPort > MUSB_HUB_MAX_PORTS)
            {
                Printf("HubError: Hub bNumOfPort =%d is too deep.\n", bNumOfPort);
                Printf("HubError: Please disconnect hub.\n");

                pUsbDevice->bSupport = MUSB_STATUS_CHILD_CONNECT_UNSUPPORT;
            
                pEnumData = &(pPort->EnumerationData);
                if (pEnumData->pfChildDeviceUpdate)
                {
                    pEnumData->pfChildDeviceUpdate(pUsbDevice->pParentUsbDevice,
                        pUsbDevice, MUSB_STATUS_CHILD_CONNECT_UNSUPPORT);
                }
                
                pHubDevice->bNextState = MUSB_HUB_STATE_DISCONNECT_DEVICE;               
                return;
            }

            /* If bNumOfPorts are  more then 0x00 then go ahead otherwise 
             * submit Intr Irp for hub. IntrIrp will report change of Hub
             * status only.
             */

            //  If depth of connected hub is already = Max depth, check if this is a compound device.            
            if ((MUSB_MAX_HUB_CHAIN == pHubDevice->bDepthInHubChain) &&
                ((pHubDevice->hubDscr.wHubCharacteristics
                      & MUSB_HUB_COMPOUND_DEVICE) == 0))
            {
                Printf("HubError: Hub tree tier = %d is too deep.\n", pHubDevice->bDepthInHubChain);
                Printf("HubError: Please disconnect hub.\n");

                pUsbDevice->bSupport = MUSB_STATUS_CHILD_CONNECT_UNSUPPORT;
            
                pEnumData = &(pPort->EnumerationData);
                if (pEnumData->pfChildDeviceUpdate)
                {
                    pEnumData->pfChildDeviceUpdate(pUsbDevice->pParentUsbDevice,
                        pUsbDevice, MUSB_STATUS_CHILD_CONNECT_UNSUPPORT);
                }
                
                pHubDevice->bNextState = MUSB_HUB_STATE_DISCONNECT_DEVICE;
                return;
            }

            if (bNumOfPort != 0x00)
            {
                /* As per 11.11 if hub supports per port Power switching then SET_FEATURE
                 * request send to every port. If hub supports GANG power switching then
                 * power of all ports belongs to gang can be switched on by making power
                 * on for one ports.
                 */
                pHubDevice->bActivePortNum = MUSB_HUB_STARTING_PORT_NUMBER;
                /* Send Command for SetFeature (PORT_POWER). */
                /* Set power for first port and then every port into a gang would be 
                 * POWER ON state.  
                 */

//  MTK Notice: Max Liao, 2007/07/24.
//  force to power on each port.
#if 0
                if (MUSB_HUB_GANGED_POWER_SWITCHING == (pHubDevice->hubDscr.wHubCharacteristics
                                                            & MUSB_HUB_LOGICAL_POWER_SWITCH_MODE_BM))
                {
                    /* bActivePort make it to MUSB_HUB_INVALID_PORT_NUMBER because
                     * request for one port is sufficient in case ganged power. 
                     */
                    MUSB_DIAG_STRING(2, "HubGangedPowerSwitched");
                    pHubDevice->bNextState = MUSB_HUB_STATE_POWER_ON_COMPLETE;
                }
                else
#endif
                {
                    /* need to send request for every port so state will be
                     * POWER_ON not POWER_ON_COMPLETE. Till IRP is not executed
                     * properly, don't make bActivePortNum to next port number.
                     */
                    MUSB_DIAG_STRING(2, "HubPerPortPowerSwitched");
                    if (pHubDevice->hubDscr.bNbrPorts == 0x01)
                    {
                        pHubDevice->bNextState = MUSB_HUB_STATE_POWER_ON_COMPLETE;
                    }
                    else
                    {
                        pHubDevice->bNextState = MUSB_HUB_STATE_POWER_ON;
                    }
                }

                dwStatus = MUSB_HubSetPortFeature(pHubDevice,
                                                  (uint16_t)MUSB_HUB_PORT_POWER_FEATURE,
                                                  MGC_HubInitializeCallback,
                                                  pHubDevice->bActivePortNum);
                if (MUSB_SUCCESS == dwStatus)
                {
                    /* If Power On is comeplete then make activePort Number to
                     * Invalid port Number.
                     */
                    if (MUSB_HUB_STATE_POWER_ON_COMPLETE == pHubDevice->bNextState)
                    {
                        MUSB_DIAG_STRING(2, "bNextSatate POWER_ON_COMPLETE");
                        pHubDevice->bActivePortNum = MUSB_HUB_INVALID_PORT_NUMBER;
                    }
                }
                else
                {
                    MUSB_DIAG_STRING(1, "HubError: While Calling MUSB_HubSetPortFeature()");
                    MUSB_DIAG_STRING(1, "HubError: Please disconnect hub");
                }
            }
            else
            {
                /* Control comes here it means number of port is 0x00 in the
                 * hub. Hub is not useful so display messge that there is no use
                 * of hub and put hub state INIT.
                 */
                MUSB_DIAG_STRING(1, "HubError: 0 ports; please disconnect the hub");
                pHubDevice->bNextState = MUSB_HUB_STATE_DISCONNECT_DEVICE;
            }

            break;
        } /* End of case MUSB_HUB_STATE_GOT_HUB_DSCR */

    case MUSB_HUB_STATE_POWER_ON:
        {
            /* If control comes here it means PORT_POWER_FEATURE for port
             * reperesented by bActivePortNum is set and do same thing for 
             * next port. Hub will remain in this state till 
             * pHubDevice->bActivePortNum == bNumOfPort.
             */
            MUSB_DIAG_STRING(2, "Hub: MUSB_HUB_STATE_POWER_ON");

            pHubDevice->bActivePortNum++;
            
            if (pHubDevice->bActivePortNum < pHubDevice->hubDscr.bNbrPorts)
            {
                pHubDevice->bNextState = MUSB_HUB_STATE_POWER_ON;
            }
            else
            {
                pHubDevice->bNextState = MUSB_HUB_STATE_POWER_ON_COMPLETE;
            }

            dwStatus = MUSB_HubSetPortFeature(pHubDevice,
                                              (uint16_t)MUSB_HUB_PORT_POWER_FEATURE,
                                              MGC_HubInitializeCallback,
                                              pHubDevice->bActivePortNum);

            if (MUSB_SUCCESS != dwStatus)
            {
                MUSB_DIAG_STRING(1, "HubError: while calling MUSB_HubSetPortFeature()");
                MUSB_DIAG_STRING(1, "HubError: Please disconnect hub");
            }

            break;
        } /* End of case  MUSB_HUB_STATE_POWER_ON */

    case MUSB_HUB_STATE_POWER_ON_COMPLETE:
        {
            /* Control Comes here it means All the ports are in POWER ON state.
             * Now its time to  Get Hub Status for Power Validation.
             */
            MUSB_DIAG_STRING(2, "Hub: MUSB_HUB_STATE_POWER_ON_COMPLETE");

            /* Call callbackWait for (bPwrOn2PwrGood *2) msecond  so that power on ports will 
             * be good. As per 11.23.2.1 bPwrOn2PwrGood
             */             
            dwPwrOn2PwrGood = (uint32_t)MUSB_MAX((uint32_t)pHubDevice->hubDscr.bPwrOn2PwrGood, 200);
            
            dwStatus = MGC_HubStartTimer(pHubDevice->busHandle, 
                                pHubDevice->bIndexAtHubDeviceList,
                                (dwPwrOn2PwrGood *2), 
                                MGC_HubPowerTimerCallback,
                                (void *)pHubDevice);
            if (MUSB_SUCCESS != dwStatus)
            {
                MUSB_DIAG1(1, "HubError: MGC_HubStartTimer() returned ", dwStatus, 16, 0);

                MUSB_DIAG_STRING(1, "HubError: Please disconnect hub");
                pHubDevice->bNextState = MUSB_HUB_STATE_DISCONNECT_DEVICE;
            }
            break;
        } /* End of case  MUSB_HUB_STATE_POWER_ON_COMPLETE */

    case MUSB_HUB_STATE_GOT_HUB_STATUS:
        {
            MUSB_DIAG_STRING(2, "Hub: MUSB_HUB_STATE_GOT_HUB_STATUS");

            MGC_HubStoreStatus(pHubDevice->aHubPortStatus, &(pHubDevice->hubStatus));

            /* Now Its time to validate Power */
            dwStatus = MGC_HubPowerValidateInit(pHubDevice);

            if (MUSB_SUCCESS != dwStatus)
            {
                MUSB_DIAG1(1, "HubError: MGC_HubPowerValidateInit() returned ", dwStatus, 16, 0);

                MUSB_DIAG_STRING(1, "HubError: Please disconnect hub");
                pHubDevice->bNextState = MUSB_HUB_STATE_DISCONNECT_DEVICE;
                break;
            }

            // Report device to user's call back function.
            pUsbDevice->bPortCount = pHubDevice->hubDscr.bNbrPorts;
            pUsbDevice->bSupport = MUSB_STATUS_CHILD_CONNECT;            

            pEnumData = &(pPort->EnumerationData);
            if (pEnumData->pfChildDeviceUpdate)
            {
                pEnumData->pfChildDeviceUpdate(pUsbDevice->pParentUsbDevice,
                    pUsbDevice, MUSB_STATUS_CHILD_CONNECT);
            }

            /* For all the ports, SETPORT Feature (PORT_POWER) is sent. Now 
             * time to submit Interrupt IRP. This Interrupt IRP will report
             * any change in HUB as well port status.
             */
            dwStatus = MGC_HubFillAndSubmitIntrIrp(pHubDevice, pUsbDevice->bBusAddress);

            if (MUSB_SUCCESS != dwStatus)
            {
                if (NULL != pHubDevice->intrInPipe)
                {
                    MUSB_ClosePipe(pHubDevice->intrInPipe);
                    pHubDevice->intrInPipe = NULL;
                }

                MUSB_DIAG_STRING(1, "HubError: While calling MGC_HubFillAndSubmitIntrIrp()");
                pHubDevice->bNextState = MUSB_HUB_STATE_DISCONNECT_DEVICE;
                break;
            }

            MUSB_DIAG_STRING(3, "HubSuccess: Hub is operating");
            pHubDevice->bState = MUSB_HUB_STATE_INIT;
            pHubDevice->bNextState = MUSB_HUB_STATE_INIT;
            break;
        } /* End of case  MUSB_HUB_STATE_GOT_HUB_STATUS*/

    default:
        break;
    }     /* End of Switch Statement. */
}         /* End of function MGC_HubInitializeCallback () */

/*******************************************************************
 *                 MGC_HubFillAndSubmitIntrIrp ()                  *
 *******************************************************************/

uint32_t MGC_HubFillAndSubmitIntrIrp(MUSB_Hub *pHubDevice, uint8_t bBusAddress)
{
    MUSB_Irp *pIntrInIrp;
    uint32_t dwIntrIrpDataLength;
    uint32_t dwStatus = (uint32_t)MUSB_FAILURE;

    MUSB_ASSERT(pHubDevice);
    UNUSED(bBusAddress);

    if (!pHubDevice->intrInPipe)
    {
        return MUSB_FAILURE;
    }

    pIntrInIrp = &(pHubDevice->intrInIrp);

    MUSB_DIAG_STRING(3, "MGC_HubFillAndSubmitIntrIrp() >>");
    /* One bit for each port's status change and one bit for hub status change.
     * Let us assume 4 ports are there then to report status change of 4 ports,
     * 4 bit is required and 1 bit for hub status change bit hence 5 bits are
     * required. Data transfered happens in Bytes SO 1 byte is required to report above
     * things.
     */
    dwIntrIrpDataLength = (uint32_t)((pHubDevice->hubDscr.bNbrPorts >> 3) + 1);

    /* Fill the Interrupt IRP */
    pIntrInIrp->hPipe = pHubDevice->intrInPipe;
    pIntrInIrp->bAllowShortTransfer = FALSE;
    pIntrInIrp->pBuffer = (uint8_t*)&pHubDevice->dwHubPortStatusChange;
    pIntrInIrp->dwLength = dwIntrIrpDataLength;
    pIntrInIrp->pfIrpComplete = MGC_HubIntrIrpCallback;
    pIntrInIrp->pCompleteParam = (void *)pHubDevice;
    pIntrInIrp->bIsrCallback = FALSE;

    MUSB_DIAG_STRING(2, "Hub: Submitting interrupt IRP");
    dwStatus = MUSB_StartTransfer(pIntrInIrp);

    if (dwStatus)
    {
        MUSB_DIAG1(1, "HubError: MUSB_StartTransfer() status=", dwStatus, 16, 0);
        return dwStatus;
    }

    MUSB_DIAG_STRING(3, "HubSuccess: MGC_HubFillAndSubmitIntrIrp() <<");

    return MUSB_SUCCESS;
} /* End of function  MGC_HubFillAndSubmitIntrIrp () */

/*******************************************************************
 *                 MGC_HubIntrIrpCallback ()                       *
 *******************************************************************/
void MGC_HubIntrIrpCallback(void *pContext, const MUSB_Irp *pIntrIrp)
{
    MUSB_Hub *pHubDevice;

    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pIntrIrp);

    pHubDevice = (MUSB_Hub *)pContext;
    MUSB_DIAG_STRING(2, "MGC_HubIntrIrpCallback() >> ");

    if (pIntrIrp->dwStatus)
    {
        if (pHubDevice->bIntrIrpExecutionErrorCount
                <= MUSB_HUB_MAX_ERROR_COUNT_FOR_INTR_IRP_EXECUTION)
        {
            /*  IRP is not executed properly. So can not go ahead. return from here itself
             */
            pHubDevice->bIntrIrpExecutionErrorCount++;
            MUSB_DIAG_STRING(1, "HubError: pIntrIrp->dwStatus in MGC_HubIntrIrpCallback()");
        }
        else
        {
            /* There is no use of continue with dump device. Display messge that disconnect
             * the HUb.
             */
            pHubDevice->bState = MUSB_HUB_STATE_DISCONNECT_DEVICE;

            pHubDevice->bNextState = MUSB_HUB_STATE_DISCONNECT_DEVICE;
            MUSB_DIAG_STRING(1,
                             "HubError: MGC_HubIntrIrpCallback () error count more then expected.");
            MUSB_DIAG_STRING(1, "HubError: Please disconnect hub");
        }
        return;
    }

    /* Reset Error count. */
    pHubDevice->bIntrIrpExecutionErrorCount = 0x00;
    MGC_HubIntrIrpHandler(pHubDevice, pIntrIrp);
    MUSB_DIAG_STRING(2, "MGC_HubIntrIrpCallback() <<");
    return;
} /* End of function MGC_HubCtrlReqComplete () */

/*******************************************************************
 *                 MGC_HubIntrIrpHandler ()                        *
 *******************************************************************/
void MGC_HubIntrIrpHandler(MUSB_Hub *pHubDevice, const MUSB_Irp *pIntrIrp)
{
    uint8_t bNumOfPorts;
    uint8_t bIdle = TRUE;
    uint8_t bIndex;
    MUSB_HubDriverContext *pDriverContext;
    MUSB_Hub *pParentHubDevice;
    
    MUSB_ASSERT(pHubDevice);        
    MUSB_ASSERT(pIntrIrp);

    bNumOfPorts = pHubDevice->hubDscr.bNbrPorts;
    MUSB_ASSERT(bNumOfPorts <= MUSB_HUB_MAX_PORTS);
    
    MUSB_DIAG_STRING(2, "MGC_HubIntrIrpHandler() >>");
    /* Get status information in the uint32 field so that now onward operation
     * would be easy.
     */

    LOG(7, "dwHubPortStatusChange = 0x%X.\n", pHubDevice->dwHubPortStatusChange);
    if ((pHubDevice->dwHubPortStatusChange & 0x01)
            && (MUSB_HUB_STATE_GOT_HUB_STATUS != pHubDevice->bNextState))
    {
        MUSB_DIAG_STRING(1, " Hub's Status Changed");
        /* If Hub Status Has changed then call function related to 
         * Hub Status otherwise call function related to Port status change
         */
        MUSB_HubStatusChange(pHubDevice);
    }
    else 
    {
        MUSB_ASSERT(pHubDevice->pUsbDevice);
        pDriverContext = pHubDevice->pUsbDevice->pDriverPrivateData;
        MUSB_ASSERT(pDriverContext);

        // handle status change only when all ports of parents are in idle.       
        bIdle = TRUE;
        for (bIndex=0; bIndex < pHubDevice->bIndexAtHubDeviceList; bIndex++)
        {
            if (bIndex < (uint8_t)MUSB_HUB_MAX_HUB_SUPPORTED)
            {
                pParentHubDevice = &pDriverContext->hubDeviceList[bIndex];
                
                if (pParentHubDevice->dwHubPortStatusChange != 0)
                {
                    bIdle = FALSE;
                    break;
                }
            }
        }
    
        /* call funtion which handle change in port status.
         */
        // handle status change only when all port are in idle.
        if (bIdle)
        {
            for (bIndex=0; bIndex < bNumOfPorts; bIndex++)
            {
                if (bIndex < (uint8_t)MUSB_HUB_MAX_PORTS)
                {
                    if (pHubDevice->aHubPortState[bIndex] != MUSB_HUB_PORT_STATE_DEFAULT)
                    {
                        bIdle = FALSE;
                        break;
                    }
                }
            }
        }
        
        if (bIdle)
        {
            pHubDevice->bActivePortNum = MUSB_HUB_STARTING_PORT_NUMBER;
            MGC_HubPortStatusChange(pHubDevice);
        }            
    }

    MUSB_DIAG_STRING(2, "MGC_HubIntrIrpHandler() <<");
    return;
} /* End of function MGC_HubIntrIrpHandler () */

/*******************************************************************
 *                 MGC_HubDisconnectHandler ()                     *
 *******************************************************************/

void MGC_HubDisconnectHandler(MUSB_Hub *pHubDevice,
                              MUSB_HubDriverContext *pDriverContext)
{
    MUSB_ASSERT(pHubDevice);
    UNUSED(pDriverContext);

    MUSB_DIAG_STRING(2, "Hub: MGC_HubDisconnectHandler () >> ");

    pHubDevice->bAllocatedPower = 0;

    if (pHubDevice->intrInIrp.hPipe)
    {
        VERIFY(0 == MUSB_CancelTransfer(&pHubDevice->intrInIrp));
        pHubDevice->intrInIrp.hPipe = NULL;
    }
       
    /* Control comes here when hub is already Deenumerated. */
    if (pHubDevice->intrInPipe)
    {
        MUSB_ClosePipe(pHubDevice->intrInPipe);
        pHubDevice->intrInPipe = NULL;
    }

    // stop hub timer.
    (void)MGC_HubStopTimer(pHubDevice->busHandle, 
        pHubDevice->bIndexAtHubDeviceList, (void *)pHubDevice);

    MGC_DeEnumerateHub(pHubDevice);

    pHubDevice->busHandle = NULL;
    pHubDevice->pUsbDevice = NULL;

    MUSB_DIAG_STRING(2, "Hub: MGC_HubDisconnectHandler () Complete");
    return;
} /* End of function MGC_HubDisconnectHandler () */

/*******************************************************************
 *                 MGC_HubFindFreeHubDeviceIndex ()                *
 *******************************************************************/
uint32_t MGC_HubFindFreeHubDeviceIndex(MUSB_HubDriverContext *pDriverContext, uint8_t *pIndex)
{
    uint8_t bIndex;

    MUSB_ASSERT(pDriverContext);
    MUSB_ASSERT(pIndex);

    for (bIndex = 0x00; bIndex < MUSB_HUB_MAX_HUB_SUPPORTED; bIndex++)
    {
        if (pDriverContext->hubDeviceList[bIndex].bState == MUSB_HUB_STATE_FREE)
        {
            MUSB_DIAG_STRING(3, "HubSuccess: FreeHubDeviceIsAvailable");

            *pIndex = bIndex;
            return (MUSB_SUCCESS);
        }
    } /*   End of function   MGC_HubFindFreeHubDeviceIndex () */

    MUSB_DIAG_STRING(1, "HubSuccess: FreeHubDeviceIsNotAvailable");
    return (MUSB_FAILURE);
} /* End of function MGC_HubFindFreeHubDeviceIndex () */

/*******************************************************************
 *                 MGC_HubFindHubDevice ()                     *
 *******************************************************************/
uint32_t MGC_HubFindHubDevice(MUSB_Device *pUsbDevice, MUSB_Hub **pHubDevice)
{
    uint8_t bIndex;

    MUSB_HubDriverContext *pDriverContext;
    MUSB_Hub *pHubDeviceList;

    MUSB_ASSERT(pUsbDevice);
    MUSB_ASSERT(pHubDevice);

    pDriverContext = (MUSB_HubDriverContext *)pUsbDevice->pDriverPrivateData;
    if (!pDriverContext)
    {
        return (MUSB_FAILURE);
    }
    
    pHubDeviceList = pDriverContext->hubDeviceList;

    for (bIndex = 0x00; bIndex < MUSB_HUB_MAX_HUB_SUPPORTED; bIndex++)
    {
        if (pHubDeviceList[bIndex].pUsbDevice == pUsbDevice)
        {
            *pHubDevice = &pHubDeviceList[bIndex];
            return (MUSB_SUCCESS);
        }
    }

    return (MUSB_FAILURE);
} /* End of function  MGC_HubFindHubDevice() */

/*******************************************************************
 *                 MGC_HubPowerValidateInit ()                     *
 *******************************************************************/
/*
 * It does the verification against the power requirements (usb2.0 spec, 11.13)
 * and initialize the parameters related to power management.
 */

//#ifndef MUSB_CODE_SHRINK 
#if 1
uint32_t MGC_HubPowerValidateInit(MUSB_Hub *pHubDevice)
{
    uint8_t bConfigSelfPower;

    uint8_t bMaxPower;
    uint8_t bHubStatusLocalPower;
    MUSB_ConfigurationDescriptor *pConfig;
    MUSB_Hub *pParentHubDevice;
    uint32_t dwStatus = MUSB_SUCCESS;

    MUSB_ASSERT(pHubDevice);

    bHubStatusLocalPower = (uint8_t)(
                               pHubDevice->hubStatus.wStatus & MUSB_HUB_STATUS_LOCAL_POWER_BM);

    /*
     * Extract configuration,then extract self-power bit(6th bit-0x40) of the 
     * bmAttributes and bMaxPower field of configuration descriptor
     */
    pConfig = pHubDevice->pUsbDevice->pCurrentConfiguration;
    bConfigSelfPower = (uint8_t)(pConfig->bmAttributes & MUSB_HUB_CONFIG_SELF_POWER_BM);
    bMaxPower = pConfig->bMaxPower;

    if (pHubDevice->pUsbDevice->pParentUsbDevice)
    {
        dwStatus = MGC_HubFindHubDevice(pHubDevice->pUsbDevice->pParentUsbDevice,
                                        &pParentHubDevice);
        if (MUSB_SUCCESS != dwStatus)
        {
            MUSB_DIAG_STRING(1, "HubError: Parent is not a Hub");
            return (MUSB_FAILURE);
        }
        else
        {
            if (((uint8_t)MUSB_HUB_BUS_POWERED == pParentHubDevice->bSelfPower) && (!bConfigSelfPower))
            {
                /* Its paraent hub is Bus powered and it is also bus powered
                   it cannot driver any devices. So, bAllocatedPower is initialised to MAX*/
                MUSB_DIAG_STRING(1,
                                 "HubError: Parent and child both are bus-powered so they cannot work");
                pHubDevice->bAllocatedPower = MUSB_HUB_BUS_POWER_MAX_UNITS;
            }
            else
            {
                pHubDevice->bAllocatedPower = 0; /* Initialise to zero */
            }
        }
    }

    pHubDevice->bAllocatedPower = 0; /* Initialise to zero */

    if (bConfigSelfPower)            /* Self-Powered Hub */
    {
        if ((!bHubStatusLocalPower) && (!bMaxPower))
        {
            /* 
             * A device which is only self-powered, but doesn't have local power
             * cannot connect to the bus and communicate
             */

            MUSB_DIAG_STRING(1, "HubError: Self-powered hub status local power & max power=0");
            dwStatus = MUSB_FAILURE;
        }
        else if ((bHubStatusLocalPower) && (!bMaxPower))
        {
            /*
             * Self-Powered only hub and local power supply is good. 
             * Hub status also indicates local power good.
             */
            pHubDevice->bSelfPower = (uint8_t)MUSB_HUB_SELF_POWERED;
            pHubDevice->bLocalPower = (uint8_t)MUSB_HUB_LOCAL_POWERED;
        }
        else if ((!bHubStatusLocalPower) && (bMaxPower))
        {
            /*
             * This hub is capable of both self- and bus- power operating modes.
             * it is currently available as a bus-Powered hub.
             */
            pHubDevice->bSelfPower = (uint8_t)MUSB_HUB_SELF_AND_BUS_POWERED;
            pHubDevice->bLocalPower = (uint8_t)MUSB_HUB_NOT_LOCAL_POWERED;
        }
        else /* bHubStatusLocalPower = 1 bMaxPower > 0 */
        {
            /*
             * This hub is capable of both self- and bus- power operating modes.
             * it is currently available as a self-Powered hub.
             */
            pHubDevice->bSelfPower = (uint8_t)MUSB_HUB_SELF_AND_BUS_POWERED;
            pHubDevice->bLocalPower = (uint8_t)MUSB_HUB_LOCAL_POWERED;
        }
    }    /* End of if(bConfigSelfPower) */
    else /* Bus-Powered Hub */
    {
        if (!bMaxPower)
        {
            /* This is illegal set of information, hub cannot work with this */
            MUSB_DIAG_STRING(1, "HubError: Bus-powered max power=0");
            dwStatus = MUSB_FAILURE;
        }
        else
        {
            /* 
             * Hub device status reporting Self-Powered is meaning less 
             * in combination of a zeroed "bmAttributes.Self-Powered"
             * So, no need to check hub status 
             */
            pHubDevice->bSelfPower = (uint8_t)MUSB_HUB_BUS_POWERED;
            pHubDevice->bLocalPower = (uint8_t)MUSB_HUB_NOT_LOCAL_POWERED;
        }
    }

    return (dwStatus);
} /* End of MGC_HubPowerValidateInit() */
#endif
/*******************************************************************
 *                 MGC_HubValidateUpdateTtInfo ()                  *
 *******************************************************************/
/*
 * This interprets the Operating speed of the Hub and validates the 
 * descriptors fields accordingly. It also Finds the whether it supports 
 * multiple TT or Single TT or Invalid device and stores the 
 * TT information at HUB device information
 */
uint32_t MGC_HubValidateUpdateTtInfo(MUSB_Hub *pHubDevice)
{
    /*
     * The value of the "bDeviceProtocol" field of the USB device descriptor 
     * and the "bInterfaceProtocol" field in the USB interface descriptor 
     * indicate whether a hub is single-TT or multi-TT: 
     * Single-TT. bDeviceProtocol == 0x01 and bInterfaceProtocol == 0
     * Multi-TT.  bDeviceProtocol == 0x02
     *    It can have two alternate settings
     *        The first interface descriptor, bInterfaceProtocol == 1. 
     *        The second interface descriptor, bInterfaceProtocol == 2.
     *  When the hub is configured with an interface protocol of one(1), 
     *   it will operate as a single TT organized hub. 
     *  When the hub is configured with an interface protocol of two(2), 
     *   it will operate as a multiple TT organized hub. 
     */

    MUSB_InterfaceDescriptor *pInterfaceDescriptor;

    uint8_t bDeviceProtocol;
    uint8_t bInterfaceProtocol;
    MUSB_Device *pUsbDevice;

    MUSB_ASSERT(pHubDevice);
    pUsbDevice = pHubDevice->pUsbDevice;

    pInterfaceDescriptor = (MUSB_InterfaceDescriptor *)MUSB_FindInterfaceDescriptor(
    pUsbDevice->pCurrentConfiguration, 0, /* Interface number */
    pHubDevice->bAlternateSetting);

    if (!pInterfaceDescriptor)
    {
        MUSB_DIAG_STRING(1, "HubError: UpdateTTInfo-FindInfDscrFail");
        return (MUSB_FAILURE);
    }

    bDeviceProtocol = pUsbDevice->DeviceDescriptor.bDeviceProtocol;
    bInterfaceProtocol = pInterfaceDescriptor->bInterfaceProtocol;

    if ((0x00 == bDeviceProtocol) && (0x00 == bInterfaceProtocol))
    {
        /* Full-/Low-Speed Operating Hub (usb2.0, 11.23.3)*/
        pHubDevice->bTtType = (uint8_t)MUSB_HUB_NO_TT;
        MUSB_DIAG_STRING(3, "Hub:Operating At Full-/Low-Speed-NoTT");
    }
    else if ((0x02 == bDeviceProtocol) && (0x01 == bInterfaceProtocol))
    {
        /* 
         * Multiple-TT Hub, But Presently Hub configured as Single-TT,
         * First Bit(0) of Lower Nibble of "bTtType" represents Hub TT-Type
         * First Bit(4) of Upper Nibble of "bTtType" Presenty Hub TT configured,
         */
        pHubDevice->bTtType = (((uint8_t)MUSB_HUB_MULTIPLE_TT << 4) | (uint8_t)MUSB_HUB_SINGLE_TT);
    }
    else if ((0x02 == bDeviceProtocol) && (0x02 == bInterfaceProtocol))
    {
        /* Multiple-TT Hub, But Presently Hub configured as Multiple-TT */
        pHubDevice->bTtType = (((uint8_t)MUSB_HUB_MULTIPLE_TT << 4) | (uint8_t)MUSB_HUB_MULTIPLE_TT);
    }
    else if ((0x01 == bDeviceProtocol) && (0x00 == bInterfaceProtocol))
    {
        /* Single-TT Hub */
        pHubDevice->bTtType = ((uint8_t)MUSB_HUB_SINGLE_TT << 4) | (uint8_t)MUSB_HUB_SINGLE_TT;
    }
    else /* Undefined values for Hub */
    {
        MUSB_DIAG_STRING(1, "HubError:UpdateTTInfo-UndefinedDescriptor:");

        MUSB_DIAG1(1, "bDeviceProtocol:", bDeviceProtocol, 16, 0x00);
        MUSB_DIAG1(1, "bInfProtocol:", bInterfaceProtocol, 16, 0x00);

        pHubDevice->bTtType = (uint8_t)MUSB_HUB_INVALID_TT;
        return (MUSB_FAILURE);
    }

    return (MUSB_SUCCESS);
} /* End of MGC_HubValidateUpdateTtInfo() */

/*******************************************************************
 *                 MGC_HubUpdatePower ()                           *
 *******************************************************************/
/*
 * Assumption: Bus powered Hub always gets 5 units of load from upstream port 
 * and Bus Powered hub cannot be connected to a Bus powered hub.
 *
 * Power scheduling is required for Bus-Powered hubs only since 
 * there are some limits on the available power from upstream and number 
 * of devices to be connected.
 * Self-Powered hubs can drive any number of ports, so no need to 
 * schedule the power.
 *
 * It updates the available power and checks the newly connected device
 * working possibility
 */
uint32_t MGC_HubUpdatePower(MUSB_Hub *pHubDevice, uint8_t bPowerUpdateType)
{
    uint32_t dwStatus = MUSB_SUCCESS;

    MUSB_ASSERT(pHubDevice);

    if (((uint8_t)MUSB_HUB_SELF_POWERED == pHubDevice->bSelfPower)
            || ((uint8_t)MUSB_HUB_LOCAL_POWERED == pHubDevice->bLocalPower))
    {
    /* For Self-Powered hub, Power management is not required */
    }
    else
    {
        /* Bus Powered Hub */
        switch (bPowerUpdateType)
        {
        case MUSB_HUB_PORT_CONNECTED:
            {
                if (pHubDevice->bAllocatedPower < MUSB_HUB_BUS_POWER_MAX_UNITS)
                {
                    /* Deduct one unit power from available power */
                    pHubDevice->bAllocatedPower++;
                }
                else
                {
                    MUSB_DIAG_STRING(1, "Hub:WarCannotSupplyLoad");
                    dwStatus = MUSB_FAILURE;
                }

                break;
            }

        case MUSB_HUB_PORT_DISCONNECTED:
            {
                if (pHubDevice->bAllocatedPower)
                {
                    /* Add one unit power to available power */
                    pHubDevice->bAllocatedPower--;
                }

                break;
            }

        case MUSB_HUB_PORT_SUSPENDED:
            {
                /* 
                 * usb2.0 spec 7.2.3, 500uA for low-Powered devices
                 * 2.5mA for High-powred devices with remote wakeup
                 * FIXME: Should we add one unit to bAllocatedPower
                 */
                break;
            }

        case MUSB_HUB_PORT_RESUMED:
            {
                /* FIXME: This is depends on the actions taken at 
                          MUSB_HUB_PORT_SUSPENDED case */
                break;
            }

        default:
            {
                MUSB_DIAG_STRING(1, "Hub:ErrorUnknownPowerUpdateType");

                dwStatus = MUSB_FAILURE;
                break;
            }
        } /* End of Switch */
    }     /* End of else(MUSB_HUB_SELF_POWERED == pHubDevice->bSelfPower) */

    return (dwStatus);
}         /* End of MGC_HubUpdatePower() */

/*******************************************************************
 *                 MGC_HubInitAndResetTt ()                        *
 *******************************************************************/
/*
 * This Initializes the TT information of the hub and
 * Reset the TT of the Hub to keep the TT in known state
 */
#if 0
uint32_t MGC_HubInitAndResetTt(MUSB_Device *pUsbDevice,
                               MUSB_pfControlIrpComplete ControlIrpComplete,
                               MUSB_Hub *pHubDevice)
{
    uint8_t bRet;
    /*
     * Reset the TT, to keep TT in a known state 
     * (If the Hub operating at High speed)
     */
    bRet = MUSB_HubResetTt(pUsbDevice, ControlIrpComplete, (void *)pHubDevice,
                                         0x01); /*First Port's RESET TT Request */

    if (!bRet)
    {
        MUSB_DIAG_STRING(1, "Hub:ErrorResetTT");
        return (MUSB_FAILURE_UNSIGNED);
    }

    return (MUSB_SUCCESS);
} /* End of MGC_HubInitAndResetTt() */

/*******************************************************************
 *                 MGC_HubIsAnyPortNotSupportsRemoteWakeup ()      *
 *******************************************************************/
/*
 * It retunrs TRUE if all the devices connected to the given hub are supporting
 * Remote wakeup. In other cases it returns FALSE
 */
uint8_t MGC_HubIsAnyPortNotSupportsRemoteWakeup(MUSB_Device *pUsbDevice)
{
    MUSB_Hub *pHubDevice;

    MUSB_Device *pChildUsbDevice;
    uint32_t dwStatus;
    uint8_t bPortNumber;

    dwStatus = MGC_HubFindHubDevice(pUsbDevice, &pHubDevice);

    if (MUSB_SUCCESS != dwStatus)
    {
        MUSB_DIAG_STRING(1, "HubError: This Is Not A Hub Device/NULL");
        return (FALSE);
    }

    /*
     * Check all the devices connected to the Hub for remote wakeup feature
     * If any device does not support remote wakeup, return FALSE.
     * If all the devices support remote wakeup, return TRUE
     */
    for (bPortNumber = 0;
        bPortNumber < pHubDevice->hubDscr.bNbrPorts; bPortNumber++)
    {
        if (MUSB_HUB_CHILD_PRESENT != pHubDevice->aHubChildPortMap[bPortNumber])
        {
            /* 
             * No device is connected at this port or 
             * The port is more than the supported Poert numbers(MUSB_HUB_MAX_CHILD). 
             */
            MUSB_DIAG_STRING(1,
                             "HubWarning: No Device connected at the Port/ Port Number>MUSB_HUB_MAX_PORTS");
        }
        else
        {
            pChildUsbDevice = pHubDevice->pChild[bPortNumber];
            if (MUSB_CLASS_HUB == pChildUsbDevice->DeviceDescriptor.bDeviceClass)
            {
                /* Hub is connected at this port number */
                if (!MGC_HubIsAnyPortNotSupportsRemoteWakeup(pChildUsbDevice))
                {
                    /* Some device is not supporting remote wakeup */
                    break;
                }
            }
            else
            {
                if (!(pChildUsbDevice->pCurrentConfiguration->bmAttributes
                          & MUSB_HUB_REMOTE_WAKEUP_BM))
                {
                    /*
                     * The connected at the this port number is not supporting remote wakeup
                     * Then No need to check remaining ports. 
                     */
                    break;
                }
            }
        } /* End of if(MUSB_HUB_CHILD_PRESENT) */
    }     /* End of for()*/

    if (bPortNumber > pHubDevice->hubDscr.bNbrPorts)
    {
        /*
         * All the devices in the tree are supporting Remote Wakeup 
         */
        return (TRUE);
    }
    /* Found a device which doesn't support remote wakeup facility */
    return (FALSE);
} /* MGC_HubIsAnyPortNotSupportsRemoteWakeup() */
#endif /* #ifndef MUSB_CODE_SHRINK */

#endif /* MUSB_HUB */
