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
 * Function-specific functionality (response to control requests
 * on the default endpoint).
 * $Revision: #1 $
 */

#ifdef MUSB_FUNCTION

#include "mu_impl.h"

#include "mu_descs.h"
#include "mu_funpr.h"

/**
 * Fill a pipe struct as per the given
 */

void MGC_FunctionPreparePipe(MGC_Port *pPort, MGC_Pipe *pPipe,
                             MUSB_BusHandle hBus, MGC_EndpointResource *pResource,
                             const MUSB_EndpointDescriptor *pEndpoint)
{
    uint8_t bTrafficType;

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pPipe);
    MUSB_ASSERT(pResource);
    MUSB_ASSERT(pEndpoint);

    bTrafficType = pEndpoint->bmAttributes & MUSB_ENDPOINT_XFERTYPE_MASK;
    pPipe->hSession = hBus;
    pPipe->pPort = pPort;
    pPipe->pLocalEnd = pResource;

    if (pEndpoint->bEndpointAddress & MUSB_DIR_IN)
    {
        pPipe->bmFlags = (uint32_t)MGC_PIPEFLAGS_TRANSMIT;

        pResource->pTxIrp = NULL;
        pResource->dwTxOffset = 0;
    }
    else
    {
        pPipe->bmFlags = 0;

        pResource->pRxIrp = NULL;
        pResource->dwRxOffset = 0;
    }

    pPipe->pDevice = NULL;
    pPipe->bTrafficType = bTrafficType;
    pPipe->wMaxPacketSize = MUSB_SWAP16P((uint8_t *)&(pEndpoint->wMaxPacketSize));
    pResource->dwWaitFrameCount = 0;
}

/**
 * Set a new interface alternate.
 * Update pPort->bMaxFunctionEnds if necessary.
 * @param pPort port pointer
 * @param bIfaceIndex interface index
 * @param bAltSetting new alternate value
 * @return status code
 */
static uint32_t MGC_FunctionSetInterface(MGC_Port *pPort, uint8_t bIfaceIndex, uint8_t bAltSetting)
{
    uint8_t bOk;

    uint8_t bEndIndex;
    MUSB_DeviceEndpoint Endpoint;
    MGC_EndpointResource *pResource;
    const MUSB_InterfaceDescriptor *pIfaceDesc;
    const MUSB_EndpointDescriptor *pEndDesc;
    uint32_t dwStatus = MUSB_STATUS_OK;

    MUSB_ASSERT(pPort);

    Endpoint.wNakLimit = 0xffff;

    pIfaceDesc = MUSB_FindInterfaceDescriptor(pPort->pCurrentConfig, bIfaceIndex, bAltSetting);

    if (pIfaceDesc)
    {
        /* update maximum end count if needed */
        if (pIfaceDesc->bNumEndpoints > pPort->bMaxFunctionEnds)
        {
            pPort->bMaxFunctionEnds = pIfaceDesc->bNumEndpoints;
        }

        /* clear & re-bind each end */
        for (bEndIndex = 0; bEndIndex < pIfaceDesc->bNumEndpoints; bEndIndex++)
        {
            pEndDesc = MUSB_FindEndpointDescriptor(pPort->pCurrentConfig, pIfaceDesc, bEndIndex);
            if (pEndDesc)
            {
                pResource =
                    (MGC_EndpointResource *)
                        MUSB_ArrayFetch(&(pPort->LocalEnds),
                                        pEndDesc->bEndpointAddress & MUSB_ENDPOINT_NUMBER_MASK);
                if (pResource)
                {
                    /* clear state from last use */
                    pResource->bIsClaimed = FALSE; 
                    pResource->bRxClaimed = FALSE;

                    /* bind */
                    MUSB_MemCopy((void *)&(Endpoint.UsbDescriptor), (void *)pEndDesc,
                                 sizeof(MUSB_EndpointDescriptor));
                    pResource = pPort->pfBindEndpoint(pPort, &Endpoint, NULL, TRUE);
                    /* break on failure */
                    if (!pResource)
                    {
                        dwStatus = (uint32_t)MUSB_STATUS_NO_RESOURCES;
                        break;
                    }
                    /* prepare pipe */
                    MGC_FunctionPreparePipe(pPort, pPort->apPipe[bEndIndex], (MUSB_BusHandle)pPort,
                                            pResource, pEndDesc);
                }
            }
        }
    }
    else
    {
        dwStatus = (uint32_t)MUSB_STATUS_BAD_DESCRIPTORS;
    }

    if (MUSB_STATUS_OK == dwStatus)
    {
        bOk = pPort->pFunctionClient->pfInterfaceSet(
                  pPort->pFunctionClient->pPrivateData, (MUSB_BusHandle)pPort, bIfaceIndex,
                  bAltSetting, (MUSB_Pipe *)pPort->apPipe);
        dwStatus = (uint32_t)(bOk ? MUSB_STATUS_OK : MUSB_STATUS_STALLED);
    }

    return dwStatus;
}

/**
 * Find/bind a configuration.
 * Update pPort->bMaxFunctionEnds if necessary.
 * @param pPort port pointer
 * @param pConfigDesc configuration descriptor
 * @param bBind TRUE to actually bind; FALSE to just check possibility
 * @return status code
 */
static uint32_t MGC_FunctionBindConfig(MGC_Port *pPort,
                                       const MUSB_ConfigurationDescriptor *pConfigDesc,
                                       uint8_t bBind)
{
    uint8_t bIfaceIndex, bEndIndex;

    uint8_t bPipeIndex = 0;
    uint16_t wLength, wLimit;
    MUSB_Device Device;
    MUSB_DeviceEndpoint Endpoint;
    MGC_EndpointResource *pResource;
    const uint8_t *pDesc;
    const MUSB_InterfaceDescriptor *pIfaceDesc;
    const MUSB_EndpointDescriptor *pEndDesc;
    uint8_t bEndCount = 0;
    uint32_t dwStatus = MUSB_STATUS_OK;

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pConfigDesc);

    MUSB_MemSet(&Device, 0, sizeof(Device));
    Device.pPort = pPort->pInterfacePort;
    Endpoint.pDevice = &Device;
    Endpoint.wNakLimit = 0xffff;

    /* clear all current endpoints */
    bEndCount = (uint8_t)MUSB_ArrayLength(&(pPort->LocalEnds));

    for (bEndIndex = 1; bEndIndex < bEndCount; bEndIndex++)
    {
        pResource = (MGC_EndpointResource *)MUSB_ArrayFetch(&(pPort->LocalEnds), bEndIndex);
        if (pResource)
        {
            pResource->bIsClaimed = FALSE;
            pResource->bRxClaimed = FALSE;
        }
    }

    /* search for interfaces/endpoints, staying within config length */
    wLength = MUSB_SWAP16P((uint8_t *)&(pConfigDesc->wTotalLength));

//  MTK Notice: Max Liao, 2007/04/09.
//  Bug: clear bEndCount before search interface and endpoint descriptor.
    bEndCount = 0;

    for (bIfaceIndex = 0; (bIfaceIndex < pConfigDesc->bNumInterfaces) && !dwStatus; bIfaceIndex++)
    {
        /* find interface */
        pDesc = MGC_FindDescriptor((uint8_t *)pConfigDesc, wLength, MUSB_DT_INTERFACE, bIfaceIndex);
        /* break on failure */
        if (!pDesc)
        {
            dwStatus = (uint32_t)MUSB_STATUS_BAD_DESCRIPTORS;
            break;
        }
        /* interface found */
        pIfaceDesc = (MUSB_InterfaceDescriptor *)((uint32_t)pDesc);
        /* don't count alternates */
        if (pIfaceDesc->bAlternateSetting)
        {
            continue;
        }

        wLimit = wLength - (uint16_t)((uint32_t)pIfaceDesc - (uint32_t)pConfigDesc);
        for (bEndIndex = 0; (bEndIndex < pIfaceDesc->bNumEndpoints) && !dwStatus; bEndIndex++)
        {
            /* find endpoint */
            pDesc = MGC_FindDescriptor((uint8_t *)pIfaceDesc, wLimit, MUSB_DT_ENDPOINT, bEndIndex);
            /* break on failure */
            if (!pDesc)
            {
                dwStatus = (uint32_t)MUSB_STATUS_BAD_DESCRIPTORS;
                break;
            }

            bEndCount++;
            /* endpoint found; bind (or just verify) it */
            pEndDesc = (MUSB_EndpointDescriptor *)((uint32_t)pDesc);
            MUSB_MemCopy((void *)&(Endpoint.UsbDescriptor), (void *)pEndDesc,
                         sizeof(MUSB_EndpointDescriptor));
            pResource = pPort->pfBindEndpoint(pPort, &Endpoint, NULL, bBind);
            /* break on failure */
            if (!pResource)
            {
                dwStatus = (uint32_t)MUSB_STATUS_NO_RESOURCES;
                break;
            }
            /* if really binding, prepare pipe */
            if (bBind)
            {
                //  MTK Notice: Max Liao, 2006/12/05.
                //  pipe index should increase independently with bEndIndex of interface.	   
                MUSB_ASSERT(bPipeIndex < pPort->bMaxFunctionEnds);
                MGC_FunctionPreparePipe(pPort, pPort->apPipe[bPipeIndex++ ], (MUSB_BusHandle)pPort,
                                        pResource, pEndDesc);
            }
        }
    }

    /* update maximum end count if needed */
    if (bEndCount > pPort->bMaxFunctionEnds)
    {
        pPort->bMaxFunctionEnds = bEndCount;
    }

    return dwStatus;
}

/**
 * Verify that resources are available to support all given configurations
 * @param pPort port pointer
 * @param pDescriptorBuffer descriptor buffer
 * @param wDescriptorBufferLength size of descriptor buffer
 * @param bNumConfigurations how many configuration descriptors to expect in buffer
 * @param bType configuration descriptor type (CONFIGURATION or OTHER_SPEED_CONFIG)
 * @param bVerify TRUE to actually verify resources; FALSE to just set pointers in port
 */
static uint32_t MGC_FunctionVerifyResources(MGC_Port *pPort,
                                            const uint8_t *pDescriptorBuffer,
                                            uint16_t wDescriptorBufferLength,
                                            uint8_t bNumConfigurations,
                                            uint8_t bType,
                                            uint8_t bVerify)
{
    uint8_t bConfigIndex;

    const uint8_t *pDesc;
    const MUSB_ConfigurationDescriptor *pConfigDesc;
    uint32_t dwStatus = MUSB_STATUS_OK;

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pDescriptorBuffer);

    for (bConfigIndex = 0; (bConfigIndex < bNumConfigurations) && !dwStatus; bConfigIndex++)
    {
        pDesc = MGC_FindDescriptor(pDescriptorBuffer, wDescriptorBufferLength, bType, bConfigIndex);
        if (pDesc)
        {
            /* config found */
            pConfigDesc = (MUSB_ConfigurationDescriptor *)((uint32_t)pDesc);
            /* verify or set pointer */
            if (bVerify)
            {
                dwStatus = MGC_FunctionBindConfig(pPort, pConfigDesc, FALSE);
            }
            else
            {
                pPort->apConfigDescriptors[bConfigIndex] = pConfigDesc;
            }
        }
        else
        {
            dwStatus = (uint32_t)MUSB_STATUS_BAD_DESCRIPTORS;
            break;
        }
    }

    return dwStatus;
}

/*
* Register a client
*/
uint32_t MGC_FunctionRegisterClient(MGC_Port *pPort, MUSB_FunctionClient *pClient)
{
    uint8_t bEndIndex;

    MGC_Pipe *pPipe;
    uint32_t dwStatus = MUSB_STATUS_OK;
    const MUSB_DeviceDescriptor *pDeviceDesc;
    const MUSB_QualifierDescriptor *pQualifierDesc;
    uint8_t bNumConfigurations;

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pClient);

    pDeviceDesc = (MUSB_DeviceDescriptor *)((uint32_t)pClient->pStandardDescriptors);
    pQualifierDesc = (MUSB_QualifierDescriptor *)((uint32_t)pClient->pHighSpeedDescriptors);
    bNumConfigurations = pDeviceDesc->bNumConfigurations;

    pPort->bParse = TRUE;
    /* allocate convenience pointers for each config */
    if (pClient->wHighSpeedDescriptorLength >= sizeof(MUSB_QualifierDescriptor))
    {
        bNumConfigurations = MUSB_MAX(bNumConfigurations, pQualifierDesc->bNumConfigurations);
    }

    // Free previous register client structure.
    if (pPort->apConfigDescriptors)
    {
        MUSB_MemFree(pPort->apConfigDescriptors);
    }
    if (pPort->apPipe)
    {
        MUSB_MemFree(pPort->apPipe);
    }
    if (pPort->pPipe)
    {
        MUSB_MemFree(pPort->pPipe);
    }

    pPort->apConfigDescriptors = (const MUSB_ConfigurationDescriptor **)MUSB_MemAlloc(
                                     bNumConfigurations *sizeof(MUSB_ConfigurationDescriptor *));

    if (pPort->apConfigDescriptors)
    {
        /* find each config; verify resources and set config pointers */
        pPort->bMaxFunctionEnds = 0;

        dwStatus = MGC_FunctionVerifyResources(pPort,
                                               pClient->pStandardDescriptors,
                                               pClient->wDescriptorBufferLength,
                                               pDeviceDesc->bNumConfigurations,
                                               MUSB_DT_CONFIG,
                                               TRUE);
        if (!dwStatus && (pClient->wHighSpeedDescriptorLength > sizeof(MUSB_QualifierDescriptor)))
        {
            dwStatus = MGC_FunctionVerifyResources(
                           pPort, pClient->pHighSpeedDescriptors,
                           pClient->wHighSpeedDescriptorLength, pQualifierDesc->bNumConfigurations,
                           MUSB_DT_OTHER_SPEED, TRUE);
        }
    }
    else
    {
        dwStatus = (uint32_t)MUSB_STATUS_NO_MEMORY;
    }

    /* if we are OK; allocate pipe memory now */
    if (pPort->bMaxFunctionEnds && (0 == dwStatus))
    {
        pPort->apPipe = (MGC_Pipe **)MUSB_MemAlloc(pPort->bMaxFunctionEnds *sizeof(MGC_Pipe *));
        if (pPort->apPipe)
        {
            pPipe = (MGC_Pipe *)MUSB_MemAlloc(pPort->bMaxFunctionEnds *sizeof(MGC_Pipe));
            if (pPipe)
            {
                pPort->pPipe = pPipe;
                for (bEndIndex = 0; bEndIndex < pPort->bMaxFunctionEnds; bEndIndex++)
                {
                    pPort->apPipe[bEndIndex] = pPipe++;
                }
            }
            else
            {
                MUSB_MemFree(pPort->apPipe);

                pPort->apPipe = NULL;
                dwStatus = (uint32_t)MUSB_STATUS_NO_MEMORY;
            }
        }
        else
        {
            dwStatus = (uint32_t)MUSB_STATUS_NO_MEMORY;
        }
    }

    /* possibly clean up */
    if (dwStatus && pPort->apConfigDescriptors)
    {
        MUSB_MemFree(pPort->apConfigDescriptors);
        pPort->apConfigDescriptors = NULL;
    }

    return dwStatus;
}

uint32_t MGC_FunctionDestroy(MGC_Port *pPort)
{
    MUSB_ASSERT(pPort);

    if (pPort->apConfigDescriptors)
    {
        MUSB_MemFree(pPort->apConfigDescriptors);
    }

    if (pPort->apPipe)
    {
        MUSB_MemFree(pPort->apPipe[0]);
        MUSB_MemFree(pPort->apPipe);
    }

    return MUSB_STATUS_OK;
}

/*
 * Handle config selected
 * @return TRUE on success
 * @return FALSE on failure (caller needs to STALL)
 */
static uint8_t MGC_FunctionConfigSelected(MGC_Port *pPort, uint8_t bConfigValue)
{
    uint32_t dwStatus;

    uint8_t bConfig;
    const MUSB_ConfigurationDescriptor *pConfigDesc;
    uint8_t bOk = FALSE;
    const MUSB_DeviceDescriptor *pDeviceDesc;
    const MUSB_QualifierDescriptor *pQualifierDesc;
    uint8_t bNumConfigurations;

    MUSB_ASSERT(pPort);

    pDeviceDesc = (MUSB_DeviceDescriptor *)((uint32_t)pPort->pFunctionClient->pStandardDescriptors);
    pQualifierDesc = (MUSB_QualifierDescriptor *)(
                         (uint32_t)pPort->pFunctionClient->pHighSpeedDescriptors);
    bNumConfigurations = pDeviceDesc->bNumConfigurations;

    if (pPort->bIsHighSpeed && pQualifierDesc)
    {
        bNumConfigurations = pQualifierDesc->bNumConfigurations;
    }

    /* find config */
    pPort->pCurrentConfig = NULL;

    for (bConfig = 0; bConfig < bNumConfigurations; bConfig++)
    {
        pConfigDesc = (MUSB_ConfigurationDescriptor *)pPort->apConfigDescriptors[bConfig];
        if (pConfigDesc->bConfigurationValue == bConfigValue)
        {
            pPort->pCurrentConfig = pConfigDesc;
            break;
        }
    }

    if (pPort->pCurrentConfig)
    {
        /* try to really bind config */
        dwStatus = MGC_FunctionBindConfig(pPort, pPort->pCurrentConfig, TRUE);
        if (MUSB_STATUS_OK == dwStatus)
        {
            /* commit config and call client */
            pPort->bConfigValue = bConfigValue;
            LOG(5, "Host commit device configuration pPort = 0x%08X.\n ", (uint32_t)pPort);
            bOk = pPort->pFunctionClient->pfDeviceConfigSelected(
                      pPort->pFunctionClient->pPrivateData, (MUSB_BusHandle)pPort, bConfigValue,
                      (MUSB_Pipe *)pPort->apPipe);
            MGC_FunctionChangeState(pPort, MUSB_CONFIGURED);
        }
    }

    return bOk;
}

/**
 * Based on connection speed, set config pointer array to prepare for SET_CONFIGURATION()
 */
void MGC_FunctionSpeedSet(MGC_Port *pPort)
{
    uint32_t dwStatus = MUSB_STATUS_OK;

    const MUSB_DeviceDescriptor *pDeviceDesc;
    const MUSB_QualifierDescriptor *pQualifierDesc;

    MUSB_ASSERT(pPort);

    if (pPort->pFunctionClient)
    {
        pDeviceDesc = (MUSB_DeviceDescriptor *)(
                          (uint32_t)pPort->pFunctionClient->pStandardDescriptors);

        pQualifierDesc = (MUSB_QualifierDescriptor *)(
                             (uint32_t)pPort->pFunctionClient->pHighSpeedDescriptors);
        if (pPort->bIsHighSpeed && pQualifierDesc)
        {
            dwStatus = MGC_FunctionVerifyResources(
                           pPort,
                           pPort->pFunctionClient->pHighSpeedDescriptors,
                           pPort->pFunctionClient->wHighSpeedDescriptorLength,
                           pQualifierDesc->bNumConfigurations,
                           MUSB_DT_OTHER_SPEED,
                           FALSE);
        }
        else
        {
            dwStatus = MGC_FunctionVerifyResources(
                           pPort,
                           pPort->pFunctionClient->pStandardDescriptors,
                           pPort->pFunctionClient->wDescriptorBufferLength,
                           pDeviceDesc->bNumConfigurations,
                           MUSB_DT_CONFIG,
                           FALSE);
        }
    }

    UNUSED(dwStatus);
}

/*
* Parse a setup and possibly handle it based on descriptors
* Returns: TRUE if handled; FALSE to call client
*/
uint8_t MGC_FunctionParseSetup(MGC_Port *pPort, uint8_t *pbStatus)
{
    uint8_t bOurs = FALSE;

    uint8_t bOK;
    uint8_t bEnd;
    uint8_t bRecip;     /* from standard request */
    uint8_t bType;      /* requested descriptor type */
    uint8_t bQueryType; /* descriptor type to query (for CONFIGURATION handling) */
    uint16_t wWhich;    /* requested descriptor index */
    uint16_t wIter;     /* found descriptor index */
    uint16_t wValue;    /* from standard request */
    uint16_t wIndex;    /* from standard request */
    uint16_t wLength;   /* from standard request */
    const MUSB_DeviceRequest *pRequest;
    const uint8_t *pDesc;
    const MUSB_QualifierDescriptor *pQualifierDesc;
    MUSB_QualifierDescriptor *pGeneratedQualifier;
    MUSB_DeviceDescriptor *pGeneratedDevice;
    MUSB_Irp *pIrp;
    MGC_Pipe *pPipe;
    uint16_t wLang;
    uint8_t bFixed = FALSE;
    const uint8_t *pLangId = NULL; /* points to LANGID from descriptor 0 */
    uint16_t wLangCount = 0;
    uint8_t bSwitchDescriptor = FALSE;
    MGC_EndpointResource *pEnd = NULL;
    const MUSB_DeviceDescriptor *pDeviceDesc;
    const uint8_t *pScan;
    const uint8_t *pScanLimit;
    uint32_t u4Ret = 0;

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pbStatus);

    pDeviceDesc = (MUSB_DeviceDescriptor *)((uint32_t)pPort->pFunctionClient->pStandardDescriptors);
    pScan = pPort->pFunctionClient->pStandardDescriptors;
    pScanLimit = pScan + pPort->pFunctionClient->wDescriptorBufferLength;

    /* seed success */
    *pbStatus = MUSB_STATUS_OK;
    pPort->pSetupData = pPort->pFunctionClient->pControlBuffer;
    pRequest = (MUSB_DeviceRequest *)((uint32_t)pPort->pSetupData);

    if (MUSB_TYPE_STANDARD == (pRequest->bmRequestType & MUSB_TYPE_MASK))
    {
        bRecip = pRequest->bmRequestType & MUSB_RECIP_MASK;

        wValue = MUSB_SWAP16P((uint8_t *)&(pRequest->wValue));
        wIndex = MUSB_SWAP16P((uint8_t *)&(pRequest->wIndex));
        MGC_DIAG2(3, pPort->pController, "ParseSetup: std request, type=", pRequest->bmRequestType,
                  "/req=", pRequest->bRequest, 16, 2);
        MGC_DIAG2(3, pPort->pController, "ParseSetup: USB state=", pPort->bUsbState, "/wLength=",
                  pRequest->wLength, 16, 2);
        MGC_DIAG2(3, pPort->pController, "ParseSetup: wValue=", wValue, "/wIndex=", wIndex, 16, 2);
        switch (pRequest->bRequest)
        {
        case MUSB_REQ_GET_STATUS:
            if ((pPort->bUsbState >= (uint8_t)MUSB_ADDRESS) || (0 == wIndex))
            {
                /* we handle device, interface, and endpoint recipients */
                if (MUSB_RECIP_DEVICE == bRecip)
                {
                    bOurs = TRUE;

                    pPort->pSetupData[0] = (uint8_t)(*(pPort->pFunctionClient->pbSelfPowered) ? 1 : 0);
                    pPort->pSetupData[0] |= pPort->bCanWakeup ? 2 : 0;
                    pPort->pSetupData[1] = 0;
                    pPort->wSetupDataSize = 2;
                }
                else if (MUSB_RECIP_INTERFACE == bRecip)
                {
                    pDesc = MGC_FindDescriptor((uint8_t *)pPort->pCurrentConfig,
                                               MUSB_SWAP16P(
                                                   (uint8_t *)&(
                                                       pPort->pCurrentConfig->wTotalLength)),
                                               MUSB_DT_INTERFACE,
                                               (uint8_t)wIndex);
                    if (pDesc)
                    {
                        pPort->pSetupData[0] = 0;

                        pPort->pSetupData[1] = 0;
                        pPort->wSetupDataSize = 2;
                    }
                    else
                    {
                        *pbStatus = (uint8_t)MUSB_STATUS_STALLED;
                    }
                }
                else if (MUSB_RECIP_ENDPOINT == bRecip)
                {
                    bOurs = TRUE;

                    bEnd = (uint8_t)wIndex;
                    pEnd =
                        (MGC_EndpointResource *)
                            MUSB_ArrayFetch(&(pPort->LocalEnds), bEnd & MUSB_ENDPOINT_NUMBER_MASK);
                    if (pEnd)
                    {
                        pPort->pSetupData[0] = (uint8_t)(pEnd->bIsHalted ? 1 : 0);

                        pPort->pSetupData[1] = 0;
                        pPort->wSetupDataSize = 2;
                    }
                    else
                    {
                        *pbStatus = (uint8_t)MUSB_STATUS_STALLED;
                    }
                }
                else
                {
                    *pbStatus = (uint8_t)MUSB_STATUS_STALLED;
                }
            } /* END: if valid USB state */
            break;

        case MUSB_REQ_CLEAR_FEATURE:
            /* we handle device, interface (nothing defined) and endpoint */
            if ((pPort->bUsbState >= (uint8_t)MUSB_ADDRESS) || (0 == wIndex))
            {
                switch (bRecip)
                {
                case MUSB_RECIP_DEVICE:
                    bOurs = TRUE;
                    pPort->wSetupDataSize = 0;
                    if (MUSB_FEATURE_DEVICE_REMOTE_WAKEUP == wValue)
                    {
                        pPort->bCanWakeup = FALSE;
                    }
                    break;

                case MUSB_RECIP_INTERFACE:
                    pPort->wSetupDataSize = 0;
                    break;

                case MUSB_RECIP_ENDPOINT:
                    bOurs = TRUE;
                    pPort->wSetupDataSize = 0;
                    bEnd = (uint8_t)wIndex;
                    /* seed failure and prove success */
                    *pbStatus = (uint8_t)MUSB_STATUS_STALLED;
                    pEnd =
                        (MGC_EndpointResource *)
                            MUSB_ArrayFetch(&(pPort->LocalEnds), bEnd & MUSB_ENDPOINT_NUMBER_MASK);
                    if (pEnd)
                    {
                        *pbStatus = MUSB_STATUS_OK;

                        pEnd->bIsHalted = FALSE;
                        u4Ret =
                            pPort->pfProgramHaltEndpoint(pPort, pEnd, bEnd & MUSB_ENDPOINT_DIR_MASK,
                                                         FALSE);
                        /* start any pending bulk/interrupt Tx */
                        switch (pEnd->bTrafficType)
                        {
                        case MUSB_ENDPOINT_XFER_BULK:
                        case MUSB_ENDPOINT_XFER_INT:
                            pIrp = (MUSB_Irp *)pEnd->pTxIrp;
                            if (pIrp)
                            {
                                pPipe = (MGC_Pipe *)pIrp->hPipe;
                                if (pPipe && (pPipe->bmFlags & MGC_PIPEFLAGS_TRANSMIT))
                                {
                                    u4Ret =
                                        pPort->pfProgramStartTransmit(pPort, pEnd, pIrp->pBuffer,
                                                                      pIrp->dwLength, pIrp);
                                }
                            }
                            break;

                        default:
                            break;
                        }
                    }
                    break;

                default:
                    break;
                }
            }
            else
            {
                *pbStatus = (uint8_t)MUSB_STATUS_STALLED;
            }
            break;

        case MUSB_REQ_SET_FEATURE:
            /* we handle device, interface (nothing defined) and endpoint */
            if ((pPort->bUsbState >= (uint8_t)MUSB_ADDRESS) || (0 == wIndex))
            {
                switch (bRecip)
                {
                case MUSB_RECIP_DEVICE:
                    bOurs = TRUE;
                    pPort->wSetupDataSize = 0;
                    /* seed failure and prove success */
                    *pbStatus = (uint8_t)MUSB_STATUS_STALLED;
                    if (MUSB_FEATURE_DEVICE_REMOTE_WAKEUP == wValue)
                    {
                        pPort->bCanWakeup = TRUE;
                        *pbStatus = MUSB_STATUS_OK;
                    }
                    else if (pPort->bIsHighSpeed && (MUSB_FEATURE_TEST_MODE == wValue))
                    {
                        pPort->bWantTestMode = TRUE;

                        pPort->bTestMode = (uint8_t)(wIndex >> 8);
                        *pbStatus = MUSB_STATUS_OK;
                    }

#if defined(MUSB_OTG_FUNC) && defined(MUSB_OTG)

                    else if (MUSB_b_hnp_enable == wValue)
                    {
                        /* only succeed if we support HNP */
                        if (pPort->bIsHnpSupported)
                        {
                            pPort->bIsHnpAllowed = TRUE;

                            if (*(pPort->pOtgClient->pbDesireHostRole))
                            {
                                pPort->bWantHost = TRUE;
                                pPort->pfProgramBusState(pPort);
                            }
                            *pbStatus = MUSB_STATUS_OK;
                        }
                    }
                    else if (MUSB_a_hnp_support == wValue)
                    {
                        /* nothing to remember */
                        /*MGC_DIAG(2, pPort->pController, "A-device and connection supports HNP");*/
                        *pbStatus = MUSB_STATUS_OK;
                    }
                    else if (MUSB_a_alt_hnp_support == wValue)
                    {
                        /* nothing to remember */
                        /*MGC_DIAG(2, pPort->pController, "A-device supports HNP on another port");*/
                        *pbStatus = MUSB_STATUS_OK;
                    }
#endif

                    break;

                case MUSB_RECIP_INTERFACE:
                    bOurs = TRUE;
                    pPort->wSetupDataSize = 0;
                    break;

                case MUSB_RECIP_ENDPOINT:
                    bOurs = TRUE;
                    pPort->wSetupDataSize = 0;
                    bEnd = (uint8_t)wIndex;
                    /* seed failure and prove success */
                    *pbStatus = (uint8_t)MUSB_STATUS_STALLED;
                    pEnd =
                        (MGC_EndpointResource *)
                            MUSB_ArrayFetch(&(pPort->LocalEnds), bEnd & MUSB_ENDPOINT_NUMBER_MASK);
                    if (pEnd)
                    {
                        *pbStatus = MUSB_STATUS_OK;

                        pEnd->bIsHalted = TRUE;
                        u4Ret =
                            pPort->pfProgramHaltEndpoint(pPort, pEnd, bEnd & MUSB_ENDPOINT_DIR_MASK,
                                                         TRUE);
                    }
                    break;

                default:
                    break;
                }
            }
            break;

        case MUSB_REQ_SET_ADDRESS:
            bOurs = TRUE;
            pPort->wSetupDataSize = 0;
            if ((pPort->bUsbState < (uint8_t)MUSB_ADDRESS) || pPort->bSetAddress)
            {
                pPort->bSetAddress = TRUE;

                pPort->bFuncAddr = (uint8_t)(wValue & 0x7f);
                MGC_FunctionChangeState(pPort, MUSB_ADDRESS);
            }
            else
            {
                *pbStatus = (uint8_t)MUSB_STATUS_STALLED;
            }
            break;

        case MUSB_REQ_GET_DESCRIPTOR:
            if (MUSB_RECIP_DEVICE == bRecip)
            {
                bOurs = TRUE;

                wLength = MUSB_SWAP16P((uint8_t *)&(pRequest->wLength));
                bOK = FALSE;
                bQueryType = (uint8_t)(wValue >> 8);
                bType = bQueryType;
                wWhich = wValue & 0xff;
                wIter = 0;

                /* do a little dance if we have high-speed descriptors */
                if (pPort->pFunctionClient->pHighSpeedDescriptors)
                {
                    switch (bType)
                    {
                    case MUSB_DT_DEVICE:
                        if (pPort->bIsHighSpeed)
                        {
                            /* we are at high-speed, so use full-speed and switch the type */
                            bSwitchDescriptor = TRUE;
                        }
                        break;

                    case MUSB_DT_DEVICE_QUALIFIER:
                        if (pPort->bIsHighSpeed)
                        {
                            /* we are at high-speed, so use full-speed and switch the type */
                            bSwitchDescriptor = TRUE;
                            bQueryType = MUSB_DT_DEVICE;
                        }
                        else
                        {
                            /* we are at full-speed, so search high-speed and no need to switch */
                            pScan = pPort->pFunctionClient->pHighSpeedDescriptors;
                            pScanLimit = pScan + pPort->pFunctionClient->wHighSpeedDescriptorLength;
                        }
                        break;

                    case MUSB_DT_CONFIG:
                        if (pPort->bIsHighSpeed)
                        {
                            /* we are at high-speed, so search high-speed and switch the type */
                            pScan = pPort->pFunctionClient->pHighSpeedDescriptors;

                            pScanLimit = pScan + pPort->pFunctionClient->wHighSpeedDescriptorLength;
                            bSwitchDescriptor = TRUE;
                            bQueryType = MUSB_DT_OTHER_SPEED;
                        }
                        break;

                    case MUSB_DT_OTHER_SPEED:
                        if (pPort->bIsHighSpeed)
                        {
                            /* we are at high-speed, so use full-speed and switch the type */
                            bSwitchDescriptor = TRUE;
                            bQueryType = MUSB_DT_CONFIG;
                        }
                        else
                        {
                            /* we are at full-speed, so search high-speed and no need to switch */
                            pScan = pPort->pFunctionClient->pHighSpeedDescriptors;
                            pScanLimit = pScan + pPort->pFunctionClient->wHighSpeedDescriptorLength;
                        }
                        break;

                    default:
                        break;
                    } /* END: switch on descriptor type */
                }     /* END: if we have high-speed descriptors */

                /* search the descriptor buffer */
                while (!bOK && ((uint32_t)pScan < (uint32_t)pScanLimit))
                {
                    if (bQueryType == pScan[1])
                    {
                        /* special case for strings */
                        if (MUSB_DT_STRING == bQueryType)
                        {
                            /* remember where langid's are */
                            if (!wIter)
                            {
                                MUSB_ASSERT(pScan[0] >= 2);
                                wLangCount = ((uint32_t)(pScan[0] - 2)) >> 1;
                                pLangId = (const uint8_t *)((uint32_t)&(pScan[2]));
                            }
                            else if (wWhich && !bFixed)
                            {
                                bFixed = TRUE;
                                /* scale wWhich based on langid */
                                MUSB_ASSERT(pLangId);
                                for (wLang = 0; wLang < wLangCount; wLang++)
                                {
                                    if (wIndex == MUSB_SWAP16P((uint8_t *)&(pLangId[wLang])))
                                    {
                                        break;
                                    }
                                }

                                if (wLang >= wLangCount)
                                {
                                    /* langid not found */
                                    *pbStatus = (uint8_t)MUSB_STATUS_STALLED;
                                    break;
                                }
                                wWhich += (wLang *pPort->pFunctionClient->wStringDescriptorCount);
                            }
                        }

                        if (wIter == wWhich)
                        {
                            bOK = TRUE;
                            break;
                        }
                        wIter++;
                    }
                    pScan += pScan[0];
                }
                if ((uint32_t)pScan < (uint32_t)pScanLimit)
                {
                    /* descriptor found; see if we need to finish the dance */
                    if (bSwitchDescriptor)
                    {
                        /* we need to generate/modify */
                        switch (bType)
                        {
                        case MUSB_DT_DEVICE:
                            /* generate a device descriptor based on a qualifier descriptor */
                            pQualifierDesc = (MUSB_QualifierDescriptor *)(
                                                 (uint32_t)
                                                     pPort->pFunctionClient->pHighSpeedDescriptors);
                            pPort->wSetupDataSize = MUSB_MIN(wLength,
                                                             sizeof(MUSB_DeviceDescriptor));
                            MUSB_MemCopy(pPort->pSetupData, pScan, pPort->wSetupDataSize);
                            pGeneratedDevice = (MUSB_DeviceDescriptor *)(
                                                   (uint32_t)pPort->pSetupData);
                            /* buffer must be large enough */
                            pGeneratedDevice->bcdUSB = pQualifierDesc->bcdUSB;
                            pGeneratedDevice->bDeviceClass = pQualifierDesc->bDeviceClass;
                            pGeneratedDevice->bDeviceSubClass = pQualifierDesc->bDeviceSubClass;
                            pGeneratedDevice->bDeviceProtocol = pQualifierDesc->bDeviceProtocol;
                            pGeneratedDevice->bMaxPacketSize0 = pQualifierDesc->bMaxPacketSize0;
                            pGeneratedDevice->bNumConfigurations =
                                pQualifierDesc->bNumConfigurations;
                            break;

                        case MUSB_DT_DEVICE_QUALIFIER:
                            /* generate a qualifier descriptor based on a device descriptor */
                            pQualifierDesc = (MUSB_QualifierDescriptor *)(
                                                 (uint32_t)
                                                     pPort->pFunctionClient->pHighSpeedDescriptors);
                            pPort->wSetupDataSize = MUSB_MIN(wLength,
                                                             sizeof(MUSB_QualifierDescriptor));
                            MUSB_MemCopy(pPort->pSetupData, (uint8_t *)pQualifierDesc,
                                         pPort->wSetupDataSize);
                            pGeneratedQualifier = (MUSB_QualifierDescriptor *)(
                                                      (uint32_t)pPort->pSetupData);
                            /* buffer must be large enough */
                            pGeneratedQualifier->bcdUSB = pDeviceDesc->bcdUSB;
                            pGeneratedQualifier->bDeviceClass = pDeviceDesc->bDeviceClass;
                            pGeneratedQualifier->bDeviceSubClass = pDeviceDesc->bDeviceSubClass;
                            pGeneratedQualifier->bDeviceProtocol = pDeviceDesc->bDeviceProtocol;
                            pGeneratedQualifier->bMaxPacketSize0 = pDeviceDesc->bMaxPacketSize0;
                            pGeneratedQualifier->bNumConfigurations =
                                pDeviceDesc->bNumConfigurations;
                            break;

                        case MUSB_DT_CONFIG:
                        case MUSB_DT_OTHER_SPEED:
                            /* same format, just change the type */
                            pPort->wSetupDataSize = (uint16_t)MUSB_MIN(wLength, ((uint32_t)pScanLimit - (uint32_t)pScan));
                            MUSB_MemCopy(pPort->pSetupData, pScan, pPort->wSetupDataSize);
                            pPort->pSetupData[1] = bType;
                            break;

                        default:
                            break;
                        }
                    }
                    else
                    {
                        /* no dance; just send it from the client buffer */
                        switch (bQueryType)
                        {
                        case MUSB_DT_CONFIG:
                        case MUSB_DT_OTHER_SPEED:
                            pPort->wSetupDataSize = (uint16_t)MUSB_MIN(wLength, ((uint32_t)pScanLimit - (uint32_t)pScan));
                            break;

                        default:
                            pPort->wSetupDataSize = (uint16_t)MUSB_MIN(wLength, pScan[0]);
                        }
                        pPort->pSetupData = (uint8_t *)pScan;
                    }
                }
                else
                {
                    /* no such descriptor found */
                    *pbStatus = (uint8_t)MUSB_STATUS_STALLED;
                }
            }
            break;

        case MUSB_REQ_GET_CONFIGURATION:
            bOurs = TRUE;
            pPort->pSetupData[0] = pPort->bConfigValue;
            pPort->wSetupDataSize = 1;
            break;

        case MUSB_REQ_SET_CONFIGURATION:
            bOurs = TRUE;
            pPort->wSetupDataSize = 0;
            bOK = FALSE;
            if (pPort->bUsbState >= (uint8_t)MUSB_ADDRESS)
            {
                bOK = FALSE;
                if (!wValue)
                {
                    bOK = TRUE;

                    pPort->bConfigValue = 0;
                    MGC_FunctionChangeState(pPort, MUSB_ADDRESS);
                }
                else
                {
                    bOK = MGC_FunctionConfigSelected(pPort, (uint8_t)(wValue & 0xff));
                }
            }
            if (!bOK)
            {
                *pbStatus = (uint8_t)MUSB_STATUS_STALLED;
            }
            break;

        case MUSB_REQ_GET_INTERFACE:
        case MUSB_REQ_SET_INTERFACE:
            pPort->wSetupDataSize = 0;
            /* if improper state, stall */
            if ((uint8_t)MUSB_CONFIGURED != pPort->bUsbState)
            {
                bOurs = TRUE;
                *pbStatus = (uint8_t)MUSB_STATUS_STALLED;
            }
            else if (MUSB_REQ_SET_INTERFACE == pRequest->bRequest)
            {
                /* re-bind */
                bOurs = TRUE;
                if (MUSB_STATUS_OK != MGC_FunctionSetInterface(pPort, (uint8_t)(wIndex & 0xff),
                                                               (uint8_t)(wValue & 0xff)))
                {
                    *pbStatus = (uint8_t)MUSB_STATUS_STALLED;
                }
            }
            break;

        default:
            break;
        } /* END: switch on request type */
    }     /* END: if standard request */

    MGC_DIAG(3, pPort->pController, bOurs ? "ParseSetup: handled" : "ParseSetup: NOT handled");

    UNUSED(u4Ret);
    return bOurs;
}

/*
 * Change state
 */
void MGC_FunctionChangeState(MGC_Port *pPort, MUSB_State State)
{
    MUSB_ASSERT(pPort);

    /* only signal a change */
    if ((!pPort->bIsHost) && (pPort->bUsbState != (uint8_t)State))
    {
        MGC_DIAG1(2, pPort->pController, "USB state change: ", (uint32_t)State, 10, 0);

        pPort->bUsbState = (uint8_t)State;
        if (pPort->pFunctionClient)
        {
            pPort->pFunctionClient->pfUsbState(pPort->pFunctionClient->pPrivateData,
                                               (MUSB_BusHandle)pPort,
                                               State);
        }
    }
}

/*
* Respond to host setup
*/
uint32_t MUSB_DeviceResponse(MUSB_BusHandle hBus, uint32_t dwSequenceNumber,
                             const uint8_t *pResponseData, uint16_t wResponseDataLength,
                             uint8_t bStall)
{
    uint32_t dwStatus = (uint32_t)MUSB_STATUS_TIMEOUT;

    MGC_Port *pPort;

    MUSB_ASSERT(hBus);

    pPort = (MGC_Port *)hBus;

    MGC_DIAG1(3, pPort->pController, "DeviceResponse: seqNum=", dwSequenceNumber, 16, 8);
    MGC_DIAG2(3, pPort->pController, "DeviceResponse: wLength=", wResponseDataLength, "/stall=",
              bStall, 16, 4);

    if (dwSequenceNumber == pPort->dwSequenceNumber)
    {
        pPort->pSetupData = (uint8_t *)pResponseData;

        pPort->wSetupDataSize = wResponseDataLength;
        dwStatus = pPort->pfDefaultEndResponse(pPort, bStall);
    }

    return dwStatus;
}

MUSB_BusHandle MUSB_RegisterFunctionClient(
    MUSB_Port* pPort, MUSB_FunctionClient* pFunctionClient)
{
    void *pResult = NULL;
    MGC_Port *pImplPort;
    uint32_t dwStatus = 0;

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pFunctionClient);
    pImplPort = (MGC_Port *)pPort->pPrivateData;
    MUSB_ASSERT(pImplPort);

    /* function client requires verification */
    dwStatus = MGC_FunctionRegisterClient(pImplPort, pFunctionClient);
    if (dwStatus == MUSB_STATUS_OK)
    {
        pImplPort->pFunctionClient = pFunctionClient;

        /* set return value */
        pResult = pImplPort;
    }

    return pResult;
}

#endif /* MUSB_FUNCTION */
