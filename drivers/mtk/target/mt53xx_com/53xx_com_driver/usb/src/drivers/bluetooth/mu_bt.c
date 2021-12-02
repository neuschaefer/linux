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
 * Top-level Bluetooth class driver module
 * $Revision: #1 $
 */
#ifndef MUSB_LOADER
#ifdef CC_USB_CSR_BLUETOOTH

#include "mu_bits.h"
#include "mu_cdi.h"
#include "mu_descs.h"
#include "mu_diag.h"
#include "mu_mem.h"
#include "mu_stdio.h"
#include "mu_impl.h"

#include "mu_bt.h"

#define MGC_BT_GAP_TIME   (20)

/**************************** GLOBALS *****************************/
/** Connect handler for BT class driver */
static uint8_t MGC_BtConnect(void *pPrivateData, MUSB_BusHandle hBus, 
    MUSB_Device *pUsbDevice, const uint8_t *pPeripheralList);

/** Disconnect Handler for BT Device Driver */
static uint8_t MGC_BtDisconnect(void *pPrivateData, 
    MUSB_BusHandle hBus, MUSB_Device *pUsbDevice);

static uint32_t MGC_BtIoctl(uint32_t u4Index, uint32_t u4Cmd, 
    uint32_t u4Aux, void *pBuffer);

static uint32_t MGC_BtIotransfer(uint32_t u4Index, MUSB_BT_IO_T *prBtIOData);

static uint32_t MGC_BtIoinit(uint32_t u4Index);

static HANDLE_T MGC_hBtSema;

static const uint8_t MGC_aBtPeripheralList [] =
{
    (uint8_t)MUSB_TARGET_CLASS, MUSB_CLASS_WIRELESS, 
    (uint8_t)MUSB_TARGET_SUBCLASS, MUSB_SUBCLASS_RF, 
    (uint8_t)MUSB_TARGET_PROTOCOL, MUSB_PROTOCOL_BT, 
    (uint8_t)MUSB_TARGET_ACCEPT
};

/** Bluetooth device driver */
static MUSB_DeviceDriver MGC_BtDeviceDriver =
{
    NULL, MGC_BtConnect, MGC_BtDisconnect, NULL, NULL,
};

static MGC_BtDevice MGC_arBtDevice[MUSB_BT_DEVICE_NUM];

static uint8_t MGC_bBtInit = FALSE;

/* Provide this driver to middleware. */
static const MU_BT_DEVICE_TYPE MGC_BtHandleDriver =
{
    MUSB_BT_DEVICE_NUM,  /* number of units */
    MGC_BtIoctl,               /* IO control interface */
    MGC_BtIotransfer,
    MGC_BtIoinit
};

/*************************** FUNCTIONS ****************************/

/**
 * Allocate per-device data
 */
static MGC_BtDevice *MGC_BtDeviceInit(void)
{
    MGC_BtDevice *pBtDevice = NULL;
    uint8_t bSupport;
    uint8_t bIndex;

    bSupport = FALSE;
    for (bIndex=0; bIndex<MUSB_BT_DEVICE_NUM; bIndex++)
    {
        pBtDevice = &MGC_arBtDevice[bIndex];
        if (pBtDevice->pUsbDevice == NULL)
        {
            pBtDevice->bDeviceIndex = bIndex;
            bSupport = TRUE;
            break;
        }
    }
    if (!bSupport)
    {
        LOG(0, "Bt Err: No space avaiabl.\n", NULL);
        return (NULL);
    }
    
    pBtDevice->pDriver = &MGC_BtDeviceDriver;
    pBtDevice->bState = MUSB_BT_STATE_FREE;
    
    return pBtDevice;
}

static void MGC_BtTimerExpired(void *pControllerPrivateData, uint16_t wTimerIndex)
{
    MGC_Port *pPort;
    MUSB_Device *pDevice = NULL;
    uint16_t wCount = 0;
    uint16_t wIndex = 0;
    MGC_BtDevice *pBtDevice;
    MUSB_HOTSWAP_NFY_INFO_T *pt_hotswap_nfy_info;
    
    MUSB_ASSERT(pControllerPrivateData);
    MUSB_ASSERT(wTimerIndex == MUSB_BT_TIMER_IDX);

    pPort = (MGC_Port *)pControllerPrivateData;
    MUSB_ASSERT(pPort->pController);

    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));

    for (wIndex=0; wIndex < wCount; wIndex++)
    {
        pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), wIndex);

        if ((pDevice != NULL) &&        
            (pDevice->bSupport == MUSB_STATUS_CHILD_UNKNOW) &&        
            (pDevice->bDeviceClass == MUSB_CLASS_WIRELESS) &&
            (pDevice->pDriverPrivateData))
        {
            // temp setting.
            pDevice->bPortCount = 1;
            pDevice->bSupport = MUSB_STATUS_CHILD_CONNECT;
            pBtDevice = (MGC_BtDevice *)pDevice->pDriverPrivateData;    
            if (pBtDevice)
            {
                pBtDevice->bState = MUSB_BT_STATE_SET_IDLE;

                // call back to upper layer when device already insert.
                if ((pBtDevice->pUsbDevice) && 
                    (pBtDevice->pPort) &&
                    (pBtDevice->pt_socket_nfy_info))
                {
                    pt_hotswap_nfy_info = pBtDevice->pt_socket_nfy_info;
                    if (pt_hotswap_nfy_info->pfNfy)
                    {
                        pt_hotswap_nfy_info->pvDevice = (void *)pBtDevice->pUsbDevice;
                        pt_hotswap_nfy_info->u4Status = (uint32_t)HFI_STATUS_READY;
                        pt_hotswap_nfy_info->u4SocketNo = (uint32_t)pBtDevice->pUsbDevice->bHubPort;
                        pt_hotswap_nfy_info->u4UsbPort = pBtDevice->pPort->bUsbPort;                        
                        pt_hotswap_nfy_info->pfNfy(pt_hotswap_nfy_info->pvTag);
                    }
                }
            }

            LOG(5, "Bt connect finish.\n", NULL);
        }
    }

}

/** Callback function when device acknowledges set config reqeust. */
static void MGC_BtSetConfigCallback(void *pContext, MUSB_ControlIrp *pControlIrp)
{
    MGC_BtDevice *pBtDevice = NULL;
    MGC_Port *pPort;
    MUSB_SystemServices *pServices;
    MUSB_Device *pDevice;
    MGC_EnumerationData *pEnumData;

    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pControlIrp);

    pBtDevice = (MGC_BtDevice *)pContext;
    if (pBtDevice == NULL)
    {
        LOG(0, "pBtDevice = NULL.\n");    
        return; 
    }   
   
    if (MUSB_STATUS_OK != pControlIrp->dwStatus)
    {
        LOG(0, "Bt Err: Set Config Callback Status = 0x%X.\n", pControlIrp->dwStatus);

        pBtDevice->bState = MUSB_BT_STATE_FREE;
        return;
    }

    pDevice = pBtDevice->pUsbDevice;
    if (pDevice)
    {
        pBtDevice->bState = MUSB_BT_STATE_CONFIGURED;
        
        /* Set the Current Configuration Descriptor to Default as Set Config is Success */
        pBtDevice->pUsbDevice->pCurrentConfiguration = 
            pBtDevice->pUsbDevice->apConfigDescriptors[0];

        /* Device is connected */
        LOG(5, "Bt HubPort-%d, UsbDevice = 0x%08X Config OK.\n", 
            pBtDevice->pUsbDevice->bHubPort, (uint32_t)pBtDevice->pUsbDevice);

        MUSB_ASSERT(pBtDevice->hBus);
        if (pBtDevice->hBus) 
        {
            pPort = (MGC_Port *)pBtDevice->hBus;
            if (pPort->pController)
            {
                MUSB_ASSERT(pPort->pController);
                if (pPort->pController->pSystemServices)
                {
                    pServices = pPort->pController->pSystemServices;
                    MUSB_ASSERT(pServices);    

                    // report to middleware.
                    pDevice->bSupport = MUSB_STATUS_CHILD_CONNECT;

                    pEnumData = &(pPort->EnumerationData);                    
                    if (pEnumData->pfChildDeviceUpdate)
                    {
                        pEnumData->pfChildDeviceUpdate(pDevice->pParentUsbDevice,
                            pDevice, pDevice->bSupport);
                    }

                    //Timer Index MUSB_BT_TIMER_IDX is reserved for BT class driver.
                    /* kill timer */
                    (void)pServices->pfCancelTimer(pServices->pPrivateData, 
                        MUSB_BT_TIMER_IDX/*bTimerIndex*/);
                    
                    (void)pServices->pfArmTimer(pServices->pPrivateData, 
                        MUSB_BT_TIMER_IDX/*bTimerIndex*/, 
                        MGC_BT_GAP_TIME/*dwTime*/, FALSE, MGC_BtTimerExpired, pPort);
                }
            }
        }
    }
}

/** After getting connect callback,  device driver calls this function to configure device */
static uint32_t MGC_BtConfigureDevice(MGC_BtDevice *pBtDevice)
{
    MUSB_DeviceRequest *pSetup;
    MUSB_ControlIrp *pControlIrp;
    uint32_t dwStatus;

    MUSB_ASSERT(pBtDevice);
    
    pSetup = (MUSB_DeviceRequest *)pBtDevice->aSetupTx;
    pControlIrp = &(pBtDevice->ControlIrp);

    /** Prepare the Setup Packet for sending Set Config Request */
    MGC_PREPARE_BT_SETUP_PACKET(pSetup, 
        (MUSB_DIR_OUT | MUSB_TYPE_STANDARD | MUSB_RECIP_DEVICE),
        MUSB_REQ_SET_CONFIGURATION, 
        pBtDevice->pConfig->bConfigurationValue,
        0, 
        0);
    
    /** Fill Control Irp */
    MGC_FILL_BT_CONTROL_IRP(pBtDevice, 
        pControlIrp, 
        (uint8_t *)pSetup, 
        8,  /* Only setup phase data */ 
        NULL, 
        0, 
        MGC_BtSetConfigCallback);

    dwStatus = MUSB_StartControlTransfer(pBtDevice->pUsbDevice->pPort, pControlIrp);
    if (dwStatus)
    {
        /* Log an Error and return state */
        LOG(0, "Bt Err: Set Configuration Request failed = 0x%X.\n", dwStatus);
    }

    return (dwStatus);
} /* End MGC_MsdConfigureDevice () */

/** This function is called when Bt device is connected.*/
static uint8_t MGC_BtConnect(void *pPrivateData, MUSB_BusHandle hBus, 
    MUSB_Device *pUsbDevice, const uint8_t *pPeripheralList)
{
    MGC_BtDevice *pBtDevice;
    uint8_t bInf;
    uint8_t bAltInf;
    uint8_t bEnd;    
    uint8_t bIndex;
    MUSB_InterfaceDescriptor *pIface = NULL;
    MUSB_InterfaceDescriptor *pAltIface = NULL;
    MUSB_ConfigurationDescriptor *pConfig;
    MUSB_DeviceEndpoint rEnd;
    MUSB_EndpointDescriptor *pEnd = NULL;
    
    MUSB_ASSERT(pUsbDevice);   
    UNUSED(pPrivateData);
    UNUSED(pPeripheralList);

    /* Device is connected */
    LOG(5, "Bt Device Connected.\n", NULL);

    pConfig = (MUSB_ConfigurationDescriptor *)pUsbDevice->apConfigDescriptors[0];
    if (!pConfig)
    {
        return FALSE;
    }    

    pBtDevice = MGC_BtDeviceInit();
    if (!pBtDevice)
    {
        return FALSE;
    }    

    pBtDevice->pConfig = pConfig;
       
    /* find first interface with supported subclass/protocol combination */
    for (bInf = 0;bInf < pConfig->bNumInterfaces; bInf++)
    {
        pIface = (MUSB_InterfaceDescriptor *)
            MUSB_FindInterfaceDescriptor(pConfig, bInf, 0);

        // Normal Interrupt in /bulk in / bulk out interface.
        if (pIface && 
            (MUSB_CLASS_WIRELESS == pIface->bInterfaceClass) &&
            (MUSB_SUBCLASS_RF == pIface->bInterfaceSubClass) &&
            (MUSB_PROTOCOL_BT == pIface->bInterfaceProtocol) &&
            ((uint8_t)0x3 == pIface->bNumEndpoints))
        {
            for (bEnd = 0; bEnd < pIface->bNumEndpoints; bEnd++)
            {
                pEnd = (MUSB_EndpointDescriptor *)MUSB_FindEndpointDescriptor(pConfig, pIface, bEnd);

                if (pEnd == NULL)
                {
                    LOG(0, "pEnd = NULL.\n", NULL);
                    return FALSE;
                }
                
                // Interrupt endpoint.
                if ((MUSB_ENDPOINT_XFER_INT == 
                        (pEnd->bmAttributes & MUSB_ENDPOINT_XFERTYPE_MASK)) &&
                    (MUSB_DIR_IN == (pEnd->bEndpointAddress & MUSB_ENDPOINT_DIR_MASK)))
                {
                    MUSB_MemCopy((void *)(&(rEnd.UsbDescriptor)), (void *)pEnd, 0x07);

                    rEnd.pDevice = pUsbDevice;
                    pBtDevice->hIntrInPipe = MUSB_OpenPipe(hBus, &rEnd, NULL);
                }
                // Bulk In endpoint.
                else if ((MUSB_ENDPOINT_XFER_BULK == 
                                (pEnd->bmAttributes & MUSB_ENDPOINT_XFERTYPE_MASK)) &&
                           (MUSB_DIR_IN == (pEnd->bEndpointAddress & MUSB_ENDPOINT_DIR_MASK)))     
                {
                    MUSB_MemCopy((void *)(&(rEnd.UsbDescriptor)), (void *)pEnd, 0x07);

                    rEnd.pDevice = pUsbDevice;
                    rEnd.wNakLimit = 16;                   
                    pBtDevice->hBulkInPipe = MUSB_OpenPipe(hBus, &rEnd, NULL);
                }
                // Bulk Out endpoint.
                else if ((MUSB_ENDPOINT_XFER_BULK == 
                                (pEnd->bmAttributes & MUSB_ENDPOINT_XFERTYPE_MASK)) &&
                           (MUSB_DIR_OUT == (pEnd->bEndpointAddress & MUSB_ENDPOINT_DIR_MASK)))     
                {
                    MUSB_MemCopy((void *)(&(rEnd.UsbDescriptor)), (void *)pEnd, 0x07);

                    rEnd.pDevice = pUsbDevice;
                    rEnd.wNakLimit = 16;                   
                    pBtDevice->hBulkOutPipe = MUSB_OpenPipe(hBus, &rEnd, NULL);
                }                
            }
        }

        // Iso In/Out interface with alternative 5 which is the max packet size of iso transfer.
        if (pIface && 
            (MUSB_CLASS_WIRELESS == pIface->bInterfaceClass) &&
            (MUSB_SUBCLASS_RF == pIface->bInterfaceSubClass) &&
            (MUSB_PROTOCOL_BT == pIface->bInterfaceProtocol) &&
            ((uint8_t)0x2 == pIface->bNumEndpoints))
        {        
            // Init each alternative interface setting.
            for (bAltInf = 0; bAltInf < MUSB_BT_ISO_TYPE_NUM; bAltInf++)
            {
                pAltIface = (MUSB_InterfaceDescriptor *)
                    MUSB_FindInterfaceDescriptor(pConfig, bInf, bAltInf);
                
                if (pAltIface && (pAltIface->bInterfaceNumber == bInf))
                {
                    bIndex = pAltIface->bAlternateSetting;
                    if (bIndex < MUSB_BT_ISO_TYPE_NUM)
                    {
                        pEnd = (MUSB_EndpointDescriptor *)
                            MUSB_FindEndpointDescriptor(pConfig, pAltIface, 0);

                        if (pEnd == NULL)
                        {
                            LOG(0, "pEnd = NULL.\n", NULL);
                            return FALSE;
                        }                        

                        pBtDevice->rSocIface[bIndex].bInterfaceNumber = pAltIface->bInterfaceNumber;
                        pBtDevice->rSocIface[bIndex].bAlternateSetting = pAltIface->bAlternateSetting;
                        pBtDevice->rSocIface[bIndex].wMaxPacketSize = pEnd->wMaxPacketSize;
                    }

                    if ((uint8_t)0x5 == pAltIface->bAlternateSetting)
                    {
                        for (bEnd = 0; bEnd < pAltIface->bNumEndpoints; bEnd++)
                        {
                            pEnd = (MUSB_EndpointDescriptor *)
                                MUSB_FindEndpointDescriptor(pConfig, pAltIface, bEnd);

                            if (pEnd == NULL)
                            {
                                LOG(0, "pEnd = NULL.\n", NULL);
                                return FALSE;
                            }                        

                            // Iso In endpoint.
                            if ((MUSB_ENDPOINT_XFER_ISOC == 
                                    (pEnd->bmAttributes & MUSB_ENDPOINT_XFERTYPE_MASK)) &&
                                (MUSB_DIR_IN == (pEnd->bEndpointAddress & MUSB_ENDPOINT_DIR_MASK)))
                            {
                                MUSB_MemCopy((void *)(&(rEnd.UsbDescriptor)), (void *)pEnd, 0x07);

                                rEnd.pDevice = pUsbDevice;
                                pBtDevice->hIsoInPipe = MUSB_OpenPipe(hBus, &rEnd, NULL);
                            }
                            // Iso Out endpoint.
                            else if ((MUSB_ENDPOINT_XFER_ISOC == 
                                    (pEnd->bmAttributes & MUSB_ENDPOINT_XFERTYPE_MASK)) &&
                                (MUSB_DIR_OUT == (pEnd->bEndpointAddress & MUSB_ENDPOINT_DIR_MASK)))
                            {
                                MUSB_MemCopy((void *)(&(rEnd.UsbDescriptor)), (void *)pEnd, 0x07);

                                rEnd.pDevice = pUsbDevice;
                                pBtDevice->hIsoOutPipe = MUSB_OpenPipe(hBus, &rEnd, NULL);
                            }                
                        }
                    }
                }
            }                      
        }        
    }

    if ((!pBtDevice->hIntrInPipe) ||
        (!pBtDevice->hBulkInPipe) ||
        (!pBtDevice->hBulkOutPipe) ||
        (!pBtDevice->hIsoInPipe) ||
        (!pBtDevice->hIsoOutPipe))
    {
        if (pBtDevice->hIntrInPipe)    
        {
            MUSB_ClosePipe(pBtDevice->hIntrInPipe);
        }
        if (pBtDevice->hBulkInPipe)    
        {
            MUSB_ClosePipe(pBtDevice->hBulkInPipe);
        }
        if (pBtDevice->hBulkOutPipe)    
        {
            MUSB_ClosePipe(pBtDevice->hBulkOutPipe);
        }
        if (pBtDevice->hIsoInPipe)    
        {
            MUSB_ClosePipe(pBtDevice->hIsoInPipe);
        }
        if (pBtDevice->hIsoOutPipe)    
        {
            MUSB_ClosePipe(pBtDevice->hIsoOutPipe);
        }
    
        LOG(0, "Bt Err: open pipe error.\n", NULL);
    
        return FALSE;
    }    
    
    /* Initialize */
    pUsbDevice->pDriverPrivateData = pBtDevice;

    pBtDevice->hBus = hBus;
    pBtDevice->pUsbDevice = pUsbDevice;
    pBtDevice->pPort = pUsbDevice->pPort;

    /* Configure the device */
    if (MUSB_STATUS_OK != MGC_BtConfigureDevice(pBtDevice))
    {
        return FALSE;
    }

    pBtDevice->bState = MUSB_BT_STATE_CONNECT;
    
    return TRUE;
} 

/** Disconnect Handler for Bt Device Driver */
static uint8_t MGC_BtDisconnect(void *pPrivateData, 
    MUSB_BusHandle hBus, MUSB_Device *pUsbDevice)
{
    MGC_BtDevice *pBtDevice;
    MGC_Port *pPort = NULL;
    MUSB_SystemServices *pServices = NULL;  
    MUSB_Device *pDevice;
    uint16_t wCount = 0;
    uint16_t wIndex = 0;
    uint8_t bStatus = FALSE;
    MUSB_BT_IO_T *prBtIOData;
    MUSB_HOTSWAP_NFY_INFO_T *pt_hotswap_nfy_info=NULL;
    MGC_EnumerationData *pEnumData;

    MUSB_ASSERT(pUsbDevice);
    UNUSED(pPrivateData);

    pBtDevice = (MGC_BtDevice *)pUsbDevice->pDriverPrivateData;
    LOG(0, "Bt: Disconnect pUsbDevice = 0x%08X.\n", (uint32_t)pUsbDevice);

    /* Check against the USB device and bus handle */
    if ((!pBtDevice) || 
        (!hBus) || 
        (hBus != pBtDevice->hBus) || 
        (pUsbDevice != pBtDevice->pUsbDevice))
    {
        return FALSE;
    }

    if (pBtDevice->hIntrInPipe)    
    {
        MUSB_ClosePipe(pBtDevice->hIntrInPipe);
    }
    if (pBtDevice->hBulkInPipe)    
    {
        MUSB_ClosePipe(pBtDevice->hBulkInPipe);
    }
    if (pBtDevice->hBulkOutPipe)    
    {
        MUSB_ClosePipe(pBtDevice->hBulkOutPipe);
    }
    if (pBtDevice->hIsoInPipe)    
    {
        MUSB_ClosePipe(pBtDevice->hIsoInPipe);
    }
    if (pBtDevice->hIsoOutPipe)    
    {
        MUSB_ClosePipe(pBtDevice->hIsoOutPipe);
    }

    // release transfer.
    prBtIOData = pBtDevice->prBtControlData;
    if ((prBtIOData) && (prBtIOData->pfBtIOComplete))
    {
        prBtIOData->dwActualDataLength = 0;
        prBtIOData->dwStatus = BT_STATUS_PEER_REMOVED;
        prBtIOData->pfBtIOComplete(prBtIOData->pBtIOCompleteParam, prBtIOData);
    }

    prBtIOData = pBtDevice->prBtIntrInData;
    if ((prBtIOData) && (prBtIOData->pfBtIOComplete))
    {
        prBtIOData->dwActualDataLength = 0;
        prBtIOData->dwStatus = BT_STATUS_PEER_REMOVED;
        prBtIOData->pfBtIOComplete(prBtIOData->pBtIOCompleteParam, prBtIOData);
    }

    prBtIOData = pBtDevice->prBtBulkInData;
    if ((prBtIOData) && (prBtIOData->pfBtIOComplete))
    {
        prBtIOData->dwActualDataLength = 0;
        prBtIOData->dwStatus = BT_STATUS_PEER_REMOVED;
        prBtIOData->pfBtIOComplete(prBtIOData->pBtIOCompleteParam, prBtIOData);
    }

    prBtIOData = pBtDevice->prBtBulkOutData;
    if ((prBtIOData) && (prBtIOData->pfBtIOComplete))
    {
        prBtIOData->dwActualDataLength = 0;
        prBtIOData->dwStatus = BT_STATUS_PEER_REMOVED;
        prBtIOData->pfBtIOComplete(prBtIOData->pBtIOCompleteParam, prBtIOData);
    }

    prBtIOData = pBtDevice->prBtIsoInData;
    if ((prBtIOData) && (prBtIOData->pfBtIOComplete))
    {
        prBtIOData->dwActualDataLength = 0;
        prBtIOData->dwStatus = BT_STATUS_PEER_REMOVED;
        prBtIOData->pfBtIOComplete(prBtIOData->pBtIOCompleteParam, prBtIOData);
    }

    prBtIOData = pBtDevice->prBtIsoOutData;
    if ((prBtIOData) && (prBtIOData->pfBtIOComplete))
    {
        prBtIOData->dwActualDataLength = 0;
        prBtIOData->dwStatus = BT_STATUS_PEER_REMOVED;
        prBtIOData->pfBtIOComplete(prBtIOData->pBtIOCompleteParam, prBtIOData);
    }

    // call back to upper layer when device already insert.
    if ((pBtDevice->pUsbDevice) && 
        (pBtDevice->pPort) &&
        (pBtDevice->pt_socket_nfy_info) && 
        (pBtDevice->bState == MUSB_BT_STATE_SET_IDLE))
    {
        pt_hotswap_nfy_info = pBtDevice->pt_socket_nfy_info;
        if (pt_hotswap_nfy_info->pfNfy)
        {
            pt_hotswap_nfy_info->pvDevice = (void *)pBtDevice->pUsbDevice;
            pt_hotswap_nfy_info->u4Status = (uint32_t)HFI_STATUS_ABSENT;
            pt_hotswap_nfy_info->u4SocketNo = (uint32_t)pBtDevice->pUsbDevice->bHubPort;
            pt_hotswap_nfy_info->u4UsbPort = pBtDevice->pPort->bUsbPort;                        
            pt_hotswap_nfy_info->pfNfy(pt_hotswap_nfy_info->pvTag);
        }
    }                            

    pPort = (MGC_Port *)hBus;
    MUSB_ASSERT(pPort->pController);
    pServices = pPort->pController->pSystemServices;
    MUSB_ASSERT(pServices);    

    // report to middleware.
    pUsbDevice->bSupport = MUSB_STATUS_CHILD_DISCONNECT;
    pEnumData = &(pPort->EnumerationData);
    if (pEnumData && pEnumData->pfChildDeviceUpdate)
    {
        pEnumData->pfChildDeviceUpdate(pUsbDevice->pParentUsbDevice,
            pUsbDevice, pUsbDevice->bSupport);
    }

    // clear device register.
    pBtDevice->pUsbDevice = NULL;
    pBtDevice->prBtControlData = NULL;
    pBtDevice->prBtIntrInData = NULL;
    pBtDevice->prBtBulkInData = NULL;
    pBtDevice->prBtBulkOutData = NULL;
    pBtDevice->prBtIsoInData = NULL;
    pBtDevice->prBtIsoOutData = NULL;
    for ( wIndex = 0 ; wIndex < BT_MAX_BULK_RX ; wIndex++ )
    {
        pBtDevice->parBtBulkInQueue[wIndex] = NULL;
    }
    for ( wIndex = 0 ; wIndex < BT_MAX_BULK_TX ; wIndex++ )
    {
        pBtDevice->parBtBulkOutQueue[wIndex] = NULL;
    }
    for ( wIndex = 0 ; wIndex < BT_MAX_ISOC_RX ; wIndex++ )
    {
        pBtDevice->parBtIsoInQueue[wIndex] = NULL;
    }
    for ( wIndex = 0 ; wIndex < BT_MAX_ISOC_TX ; wIndex++ )
    {
        pBtDevice->parBtIsoOutQueue[wIndex] = NULL;
    }
       
    //Timer Index MUSB_BT_TIMER_IDX is reserved for BT class driver.
    /* kill timer */
    bStatus = pServices->pfCancelTimer(pServices->pPrivateData, 
        MUSB_BT_TIMER_IDX/*bTimerIndex*/);
    UNUSED(bStatus);

    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));

    // Check if any device is not handled by pProtocol->pfStartDevice().    
    for (wIndex=0; wIndex < wCount; wIndex++)
    {
        pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), wIndex);

        if ((pDevice != NULL) && 
            (pDevice != pUsbDevice) && 
            (pDevice->bSupport == MUSB_STATUS_CHILD_UNKNOW) &&        
            (pDevice->bDeviceClass == MUSB_CLASS_WIRELESS))
        {
            bStatus = pServices->pfArmTimer(pServices->pPrivateData, 
                MUSB_BT_TIMER_IDX/*bTimerIndex*/, 
                MGC_BT_GAP_TIME/*dwTime*/, FALSE, MGC_BtTimerExpired, pPort);    

            MUSB_ASSERT(bStatus);               
            break;
        }
    }

    pUsbDevice->pDriverPrivateData = NULL;
    LOG(5, "Bt Disconnect Successfully.\n", NULL);

   return TRUE;
} 

static void MGC_BtCtrlCallback(void *pContext, MUSB_ControlIrp *pControlIrp)
{
    MGC_BtDevice *pBtDevice = NULL;
    MUSB_BT_IO_T *prBtIOData;
    
    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pControlIrp);

    if (MUSB_STATUS_OK != pControlIrp->dwStatus)
    {
        LOG(0, "Bt Err: Control Status = 0x%X.\n", pControlIrp->dwStatus);
    }
   
    pBtDevice = (MGC_BtDevice *)pContext;
    prBtIOData = pBtDevice->prBtControlData;
    pBtDevice->prBtControlData = NULL;
    if (prBtIOData)
    {
        if (MUSB_STATUS_OK != pControlIrp->dwStatus)
        {
            prBtIOData->dwActualDataLength = 0;
            prBtIOData->dwStatus = pControlIrp->dwStatus;
        }
        else
        {
            prBtIOData->dwActualDataLength = prBtIOData->dwDataLength;
            prBtIOData->dwStatus = MUSB_STATUS_OK;
        }

        if (prBtIOData->pfBtIOComplete)
        {
            prBtIOData->pfBtIOComplete(prBtIOData->pBtIOCompleteParam, prBtIOData);
        }
    }    
}

static uint32_t MGC_BtControl(MGC_BtDevice *pBtDevice, MUSB_BT_IO_T *prBtIOData)
{
    MUSB_ControlIrp *pControlIrp;
    uint32_t dwStatus;
    uint32_t dwOutLength = 0;

    MUSB_ASSERT(pBtDevice);
    MUSB_ASSERT(prBtIOData);
    if (pBtDevice->prBtControlData)
    {
        // already occpy by a transfer.
        return BT_STATUS_ERROR;
    }

    pBtDevice->prBtControlData = prBtIOData;
    pControlIrp = &(pBtDevice->ControlIrp);
        
    MUSB_MemSet(pBtDevice->aSetupTx, 0, (8+MUSB_BT_CTRL_LENGTH));
    /* copy first 8 bytes of setup packet */
    MUSB_MemCopy(pBtDevice->aSetupTx, prBtIOData->pSetupPacket, 8);
    dwOutLength = 8;
    
    if (prBtIOData->bIsTx)
    {
        if (prBtIOData->dwDataLength > 0)
        {
            MUSB_ASSERT(prBtIOData->dwDataLength <= MUSB_BT_CTRL_LENGTH);  
            if (prBtIOData->dwDataLength > MUSB_BT_CTRL_LENGTH)
            {
                return BT_STATUS_ERROR;            
            }
            MUSB_MemCopy(&pBtDevice->aSetupTx[8], 
                prBtIOData->pDataBuffer, prBtIOData->dwDataLength);            
            dwOutLength += prBtIOData->dwDataLength;
        }

        /** Fill Control Irp */
        MGC_FILL_BT_CONTROL_IRP(pBtDevice, 
            pControlIrp, 
            pBtDevice->aSetupTx, 
            dwOutLength, 
            NULL, 
            0, 
            MGC_BtCtrlCallback);        
    }
    else
    {
        /** Fill Control Irp */
        MGC_FILL_BT_CONTROL_IRP(pBtDevice, 
            pControlIrp, 
            pBtDevice->aSetupTx, 
            dwOutLength, 
            prBtIOData->pDataBuffer, 
            prBtIOData->dwDataLength, 
            MGC_BtCtrlCallback);
    }   

    dwStatus = MUSB_StartControlTransfer(pBtDevice->pPort, pControlIrp);
    if (dwStatus)
    {
        /* Log an Error and return state */
        LOG(0, "Bt Err: Set Interface failed, dwStatus: 0x%X.\n", dwStatus);
    }

    return (dwStatus);
}

static void MGC_BtIntrCallback(void *pContext, const MUSB_Irp *pIntrIrp)
{
    MGC_BtDevice *pBtDevice;
    MUSB_BT_IO_T *prBtIOData;
    
    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pIntrIrp);

    if (pIntrIrp->dwStatus)
    {
        LOG(0, "Bt Err: pIntrIrp->dwStatus=0x%X\n", pIntrIrp->dwStatus);
    }

    pBtDevice = (MGC_BtDevice *)pContext;
    prBtIOData = pBtDevice->prBtIntrInData;
    if (prBtIOData)
    {
        if (MUSB_STATUS_OK != pIntrIrp->dwStatus)
        {
            prBtIOData->dwActualDataLength = 0;
            prBtIOData->dwStatus = pIntrIrp->dwStatus;
        }
        else
        {
            prBtIOData->dwActualDataLength = pIntrIrp->dwActualLength;
            prBtIOData->dwStatus = MUSB_STATUS_OK;
        }

        if (prBtIOData->pfBtIOComplete)
        {
            prBtIOData->pfBtIOComplete(prBtIOData->pBtIOCompleteParam, prBtIOData);
        }
    }
}

static uint32_t MGC_BtInterrupt(MGC_BtDevice *pBtDevice, MUSB_BT_IO_T *prBtIOData)
{
    MUSB_Irp *pIntrInIrp = NULL;
    uint32_t dwStatus;
//    uint32_t dwIrqStatus;
//    MUSB_SystemServices* pServices = ((MGC_Port*)(pBtDevice->hBus))->pController->pSystemServices;

    MUSB_ASSERT(pBtDevice);
    MUSB_ASSERT(prBtIOData);
    MUSB_ASSERT(prBtIOData->bIsTx == 0);
//    MUSB_ASSERT(pServices);

//    dwIrqStatus = pServices->pfIrqLock(pServices->pPrivateData);

    if (pBtDevice->prBtIntrInData)
    {
        // already occpy by a transfer.
        return BT_STATUS_ERROR;
    }

    /* Fill the Interrupt IRP */
    pBtDevice->prBtIntrInData = prBtIOData;
    pIntrInIrp = &pBtDevice->IntrInIrp;
    pIntrInIrp->hPipe = pBtDevice->hIntrInPipe;
    pIntrInIrp->pBuffer = prBtIOData->pDataBuffer;
    pIntrInIrp->dwLength =  prBtIOData->dwDataLength;
    pIntrInIrp->dwActualLength = 0;
    pIntrInIrp->bAllowShortTransfer = FALSE;      
    pIntrInIrp->pfIrpComplete = MGC_BtIntrCallback;
    pIntrInIrp->pCompleteParam = (void *)pBtDevice;
    pIntrInIrp->bIsrCallback = TRUE;
    pIntrInIrp->bAllowDma = FALSE;
    
    dwStatus = MUSB_StartTransfer(pIntrInIrp);

//    (void)pServices->pfIrqUnlock(pServices->pPrivateData, dwIrqStatus);
    
    if (dwStatus)
    {
        LOG(0, "Bt Err: interrupt transfer status=0x%X\n", dwStatus);
    }

    return (dwStatus);
}

static void MGC_BtBulkCallback(void *pContext, const MUSB_Irp *pBulkIrp)
{
    MGC_BtDevice *pBtDevice;
    MUSB_BT_IO_T *prBtIOData = NULL;
    uint8_t i;
    uint8_t bIndex = 0;
    MUSB_Irp *pBulkNewIrp = NULL;
    uint32_t dwStatus;
    
    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pBulkIrp);

    if (pBulkIrp->dwStatus)
    {
        LOG(0, "Bt Err: pBulkIrp->dwStatus=0x%X\n", pBulkIrp->dwStatus);
    }

    VERIFY(OSR_OK == x_sema_lock(MGC_hBtSema, X_SEMA_OPTION_WAIT));

    pBtDevice = (MGC_BtDevice *)pContext;
    if (pBulkIrp->hPipe == pBtDevice->hBulkOutPipe)
    {    
        prBtIOData = pBtDevice->prBtBulkOutData;

        bIndex = 0xFF;
        for (i=0; i<BT_MAX_BULK_TX; i++)
        {
            if (pBtDevice->parBtBulkOutQueue[i] == prBtIOData)
            {
                bIndex = i;
                pBtDevice->parBtBulkOutQueue[i] = NULL;
                break;
            }
        }
        MUSB_ASSERT(bIndex < BT_MAX_BULK_TX);
        
        // Set next request to current transfer structure.
        bIndex = ((bIndex+1) >= BT_MAX_BULK_TX) ? 0: (bIndex+1);
        
        if (pBtDevice->parBtBulkOutQueue[bIndex])
        {
            pBtDevice->prBtBulkOutData = pBtDevice->parBtBulkOutQueue[bIndex];       

            // Prepare new next transfer.
            pBulkNewIrp = &pBtDevice->BulkOutIrp;
            pBulkNewIrp->hPipe = pBtDevice->hBulkOutPipe;        
            pBulkNewIrp->pBuffer = pBtDevice->prBtBulkOutData->pDataBuffer;
            pBulkNewIrp->dwLength =  pBtDevice->prBtBulkOutData->dwDataLength;
            pBulkNewIrp->dwActualLength = 0;
            pBulkNewIrp->bAllowShortTransfer = FALSE;      
            pBulkNewIrp->pfIrpComplete = MGC_BtBulkCallback;
            pBulkNewIrp->pCompleteParam = (void *)pBtDevice;
            pBulkNewIrp->bIsrCallback = FALSE;
            pBulkNewIrp->bAllowDma = TRUE;            
        }
        else
        {
            pBtDevice->prBtBulkOutData = NULL;            
        }
    }
    else if (pBulkIrp->hPipe == pBtDevice->hBulkInPipe)
    {
        prBtIOData = pBtDevice->prBtBulkInData;

        bIndex = 0xFF;
        for (i=0; i<BT_MAX_BULK_RX; i++)
        {
            if (pBtDevice->parBtBulkInQueue[i] == prBtIOData)
            {
                bIndex = i;
                pBtDevice->parBtBulkInQueue[i] = NULL;
                break;
            }
        }
        MUSB_ASSERT(bIndex < BT_MAX_BULK_RX);
        
        // Set next request to current transfer structure.
        bIndex = ((bIndex+1) >= BT_MAX_BULK_RX) ? 0: (bIndex+1);
        
        if (pBtDevice->parBtBulkInQueue[bIndex])
        {
            pBtDevice->prBtBulkInData = pBtDevice->parBtBulkInQueue[bIndex];    

            // Prepare new next transfer.
            pBulkNewIrp = &pBtDevice->BulkInIrp;
            pBulkNewIrp->hPipe = pBtDevice->hBulkInPipe;
            pBulkNewIrp->pBuffer = pBtDevice->prBtBulkInData->pDataBuffer;
            pBulkNewIrp->dwLength =  pBtDevice->prBtBulkInData->dwDataLength;
            pBulkNewIrp->dwActualLength = 0;
            pBulkNewIrp->bAllowShortTransfer = FALSE;      
            pBulkNewIrp->pfIrpComplete = MGC_BtBulkCallback;
            pBulkNewIrp->pCompleteParam = (void *)pBtDevice;
            pBulkNewIrp->bIsrCallback = FALSE;
            pBulkNewIrp->bAllowDma = TRUE;
        }
        else
        {
            pBtDevice->prBtBulkInData = NULL;            
        }        
    }
    else
    {
        LOG(0, "Bt Err: pipe error = 0x%X\n", pBulkIrp->hPipe);
    }

    VERIFY(OSR_OK == x_sema_unlock(MGC_hBtSema));
    
    if (pBulkNewIrp)
    {
        dwStatus = MUSB_StartTransfer(pBulkNewIrp);
        if (dwStatus)
        {
            LOG(0, "Bt Err: bulk transfer status=0x%X\n", dwStatus);
        }
    }   

    if (prBtIOData)
    {
        if (MUSB_STATUS_OK != pBulkIrp->dwStatus)
        {
            prBtIOData->dwActualDataLength = 0;
            prBtIOData->dwStatus = pBulkIrp->dwStatus;
        }
        else
        {
            prBtIOData->dwActualDataLength = pBulkIrp->dwActualLength;
            prBtIOData->dwStatus = MUSB_STATUS_OK;
        }

        if (prBtIOData->pfBtIOComplete)
        {
            prBtIOData->pfBtIOComplete(prBtIOData->pBtIOCompleteParam, prBtIOData);
        }       
    }    
}

static uint32_t MGC_BtBulk(MGC_BtDevice *pBtDevice, MUSB_BT_IO_T *prBtIOData)
{
    MUSB_Irp *pBulkIrp = NULL;
    uint32_t dwStatus;
    uint8_t bFound = FALSE; /*Found a slot*/
    uint8_t i;
    
    MUSB_ASSERT(pBtDevice);
    MUSB_ASSERT(prBtIOData);

    /* Fill the Interrupt IRP */
    if (prBtIOData->bIsTx)
    {
        for (i=0; i<BT_MAX_BULK_TX; i++)
        {
            if (pBtDevice->parBtBulkOutQueue[i] == NULL)
            {
                pBtDevice->parBtBulkOutQueue[i] = prBtIOData;
                bFound = TRUE;
                break;
            }
        }

        if (!bFound)
        {
            // Queue is full.
            LOG(0, "Bt Err: bulk tx queue is full.\n", NULL);            
            return BT_STATUS_ERROR;
        }

        // Queue is empty
        if (pBtDevice->prBtBulkOutData == NULL)
        {
            pBtDevice->prBtBulkOutData = prBtIOData;
        }
        else
        {
            // Put prBtIOData to parBtBulkOutQueue[].
            return BT_STATUS_OK;
        }
            
        pBulkIrp = &pBtDevice->BulkOutIrp;
        pBulkIrp->hPipe = pBtDevice->hBulkOutPipe;        
    }
    else
    {   
        for (i=0; i<BT_MAX_BULK_RX; i++)
        {
            if (pBtDevice->parBtBulkInQueue[i] == NULL)
            {
                pBtDevice->parBtBulkInQueue[i] = prBtIOData;
                bFound = TRUE;
                break;
            }
        }

        if (!bFound)
        {
            // Queue is full.
            LOG(0, "Bt Err: bulk rx queue is full.\n", NULL);
            return BT_STATUS_ERROR;
        }

        // Queue is empty
        if (pBtDevice->prBtBulkInData == NULL)
        {
            pBtDevice->prBtBulkInData = prBtIOData;
        }
        else
        {
            // Put prBtIOData to parBtBulkInQueue[].
            return BT_STATUS_OK;
        }
    
        pBulkIrp = &pBtDevice->BulkInIrp;
        pBulkIrp->hPipe = pBtDevice->hBulkInPipe;
    }
   
    pBulkIrp->pBuffer = prBtIOData->pDataBuffer;
    pBulkIrp->dwLength =  prBtIOData->dwDataLength;
    pBulkIrp->dwActualLength = 0;
    pBulkIrp->bAllowShortTransfer = FALSE;      
    pBulkIrp->pfIrpComplete = MGC_BtBulkCallback;
    pBulkIrp->pCompleteParam = (void *)pBtDevice;
    pBulkIrp->bIsrCallback = FALSE;
    pBulkIrp->bAllowDma = TRUE;

    dwStatus = MUSB_StartTransfer(pBulkIrp);
    if (dwStatus)
    {
        LOG(0, "Bt Err: bulk transfer status=0x%X\n", dwStatus);
    }

    return (dwStatus);
}

static void MGC_BtIsochCallback(void *pContext, MUSB_IsochIrp *pIsoIrp)
{
    MGC_BtDevice *pBtDevice;
    MUSB_BT_IO_T *prBtIOData = NULL;
    uint8_t i;
    uint8_t bIndex = 0;
    MUSB_IsochIrp *pIsoNewIrp = NULL;
    uint32_t dwStatus;
    
    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pIsoIrp);

    pBtDevice = (MGC_BtDevice *)pContext;
    if (pIsoIrp->hPipe == pBtDevice->hIsoOutPipe)
    {  
        prBtIOData = pBtDevice->prBtIsoOutData;        
        
        bIndex = 0xFF;
        for (i=0; i<BT_MAX_ISOC_TX; i++)
        {
            if (pBtDevice->parBtIsoOutQueue[i] == prBtIOData)
            {
                bIndex = i;
                pBtDevice->parBtIsoOutQueue[i] = NULL;
                break;
            }
        }
        MUSB_ASSERT(bIndex < BT_MAX_ISOC_TX);
        
        // Set next request to current transfer structure.
        bIndex = ((bIndex+1) >= BT_MAX_ISOC_TX) ? 0: (bIndex+1);
        
        if (pBtDevice->parBtIsoOutQueue[bIndex])
        {
            pBtDevice->prBtIsoOutData = pBtDevice->parBtIsoOutQueue[bIndex];       

            // Prepare new next transfer.
            pIsoNewIrp = &pBtDevice->IsoOutIrp;
            pIsoNewIrp->hPipe = pBtDevice->hIsoOutPipe;
            pIsoNewIrp->pBuffer = pBtDevice->prBtIsoOutData->pDataBuffer;
            pIsoNewIrp->pfIrpComplete = MGC_BtIsochCallback;
            pIsoNewIrp->pCompleteParam = (void *)pBtDevice;
            pIsoNewIrp->wFrameCount = pBtDevice->prBtIsoOutData->dwNofPackets; 
            pIsoNewIrp->prISOpacket = (MUSB_IsoPacket *)pBtDevice->prBtIsoOutData->prISOpacket;
            pIsoNewIrp->wCurrentFrame = 0;
            pIsoNewIrp->wCallbackInterval = 0;    
            pIsoNewIrp->bIsrCallback = TRUE;
            pIsoNewIrp->bAllowDma = FALSE;            
        }
        else
        {
            pBtDevice->prBtIsoOutData = NULL;            
        }       
    }
    else if (pIsoIrp->hPipe == pBtDevice->hIsoInPipe)
    {
        prBtIOData = pBtDevice->prBtIsoInData;

        bIndex = 0xFF;
        for (i=0; i<BT_MAX_ISOC_RX; i++)
        {
            if (pBtDevice->parBtIsoInQueue[i] == prBtIOData)
            {
                bIndex = i;
                pBtDevice->parBtIsoInQueue[i] = NULL;
                break;
            }
        }
        MUSB_ASSERT(bIndex < BT_MAX_ISOC_RX);
        
        // Set next request to current transfer structure.
        bIndex = ((bIndex+1) >= BT_MAX_ISOC_RX) ? 0: (bIndex+1);
        
        if (pBtDevice->parBtIsoInQueue[bIndex])
        {
            pBtDevice->prBtIsoInData = pBtDevice->parBtIsoInQueue[bIndex];       

            // Prepare new next transfer.
            pIsoNewIrp = &pBtDevice->IsoInIrp;
            pIsoNewIrp->hPipe = pBtDevice->hIsoInPipe;
            pIsoNewIrp->pBuffer = pBtDevice->prBtIsoInData->pDataBuffer;
            pIsoNewIrp->pfIrpComplete = MGC_BtIsochCallback;
            pIsoNewIrp->pCompleteParam = (void *)pBtDevice;
            pIsoNewIrp->wFrameCount = pBtDevice->prBtIsoInData->dwNofPackets; 
            pIsoNewIrp->prISOpacket = (MUSB_IsoPacket *)pBtDevice->prBtIsoInData->prISOpacket;
            pIsoNewIrp->wCurrentFrame = 0;
            pIsoNewIrp->wCallbackInterval = 0;    
            pIsoNewIrp->bIsrCallback = TRUE;
            pIsoNewIrp->bAllowDma = FALSE;            
        }
        else
        {
            pBtDevice->prBtIsoInData = NULL;            
        }               
    }
    else
    {
        LOG(0, "Bt Err: pipe error = 0x%X\n", pIsoIrp->hPipe);
    }
    
    if (pIsoNewIrp)
    {
        dwStatus = MUSB_ScheduleIsochTransfer(0, pIsoNewIrp);
        if (dwStatus)
        {
            LOG(0, "Bt Err: Isochronous transfer status=0x%X\n", dwStatus);
        }
    }   

    if (prBtIOData)
    {
        prBtIOData->dwActualDataLength = prBtIOData->dwDataLength;
        prBtIOData->dwStatus = MUSB_STATUS_OK;

        if (prBtIOData->pfBtIOComplete)
        {
            prBtIOData->pfBtIOComplete(prBtIOData->pBtIOCompleteParam, prBtIOData);
        }
    }    
}

static uint32_t MGC_BtIsoch(MGC_BtDevice *pBtDevice, MUSB_BT_IO_T *prBtIOData)
{
    MUSB_IsochIrp *pIsoIrp = NULL;
    uint32_t dwStatus;
    uint8_t bFound = FALSE; /*Found a slot*/
    uint8_t i;
    uint32_t dwIrqStatus;
    MUSB_SystemServices* pServices = ((MGC_Port*)(pBtDevice->hBus))->pController->pSystemServices;

    MUSB_ASSERT(pBtDevice);
    MUSB_ASSERT(prBtIOData);
    MUSB_ASSERT(prBtIOData->prISOpacket);
    MUSB_ASSERT(pServices);

    dwIrqStatus = pServices->pfIrqLock(pServices->pPrivateData);
    /* Fill the Interrupt IRP */
    if (prBtIOData->bIsTx)
    {
        for (i=0; i<BT_MAX_ISOC_TX; i++)
        {
            if (pBtDevice->parBtIsoOutQueue[i] == NULL)
            {
                pBtDevice->parBtIsoOutQueue[i] = prBtIOData;
                bFound = TRUE;
                break;
            }
        }

        if (!bFound)
        {
            // Queue is full.
            LOG(0, "Bt Err: Isochronous tx queue is full.\n", NULL);
            (void)pServices->pfIrqUnlock(pServices->pPrivateData, dwIrqStatus);
            return BT_STATUS_ERROR;
        }

        // Queue is empty
        if (pBtDevice->prBtIsoOutData == NULL)
        {
            pBtDevice->prBtIsoOutData = prBtIOData;
        }
        else
        {
            // Put prBtIOData to parBtIsoOutQueue[].
            (void)pServices->pfIrqUnlock(pServices->pPrivateData, dwIrqStatus);
            return BT_STATUS_OK;
        }
    
        pIsoIrp = &pBtDevice->IsoOutIrp;
        pIsoIrp->hPipe = pBtDevice->hIsoOutPipe;
    }
    else
    {
        for (i=0; i<BT_MAX_ISOC_RX; i++)
        {
            if (pBtDevice->parBtIsoInQueue[i] == NULL)
            {
                pBtDevice->parBtIsoInQueue[i] = prBtIOData;
                bFound = TRUE;
                break;
            }
        }

        if (!bFound)
        {
            // Queue is full.
            LOG(0, "Bt Err: Isochronous rx queue is full.\n", NULL);
            (void)pServices->pfIrqUnlock(pServices->pPrivateData, dwIrqStatus);
            return BT_STATUS_ERROR;
        }

        // Queue is empty
        if (pBtDevice->prBtIsoInData == NULL)
        {
            pBtDevice->prBtIsoInData = prBtIOData;
        }
        else
        {
            // Put prBtIOData to parBtIsoInQueue[].
            (void)pServices->pfIrqUnlock(pServices->pPrivateData, dwIrqStatus);
            return BT_STATUS_OK;
        }
    
        pIsoIrp = &pBtDevice->IsoInIrp;
        pIsoIrp->hPipe = pBtDevice->hIsoInPipe;
    }
    (void)pServices->pfIrqUnlock(pServices->pPrivateData, dwIrqStatus);
    
    pIsoIrp->pBuffer = prBtIOData->pDataBuffer;
    pIsoIrp->pfIrpComplete = MGC_BtIsochCallback;
    pIsoIrp->pCompleteParam = (void *)pBtDevice;
    pIsoIrp->wFrameCount = prBtIOData->dwNofPackets; 
    pIsoIrp->prISOpacket = (MUSB_IsoPacket *)prBtIOData->prISOpacket;
    pIsoIrp->wCurrentFrame = 0;
    pIsoIrp->wCallbackInterval = 0;    
    pIsoIrp->bIsrCallback = TRUE;
    pIsoIrp->bAllowDma = FALSE;

    dwStatus = MUSB_ScheduleIsochTransfer(0, pIsoIrp);

    if (dwStatus)
    {
        LOG(0, "Bt Err: Iso transfer status=0x%X\n", dwStatus);
    }

    return (dwStatus);
}

static uint32_t MGC_BtIoctl(uint32_t u4Index, uint32_t u4Cmd, uint32_t u4Aux, void *pBuffer)
{
    MGC_BtDevice *pBtDevice;
    uint16_t wCount;
    uint16_t wIndex;
    MUSB_HOTSWAP_NFY_INFO_T *pt_hotswap_nfy_info;
    MUSB_BT_IO_REQ_NUM_T *ptBtIoReqNum;
    MUSB_SystemServices* pServices;
    uint32_t i;
  
    /* This check makes sure, the device number is within valid range */
    switch (u4Cmd)
    {
    case MUSB_BT_IOCTL_GET_DEVICE_NUM:
        if (!pBuffer)
        {            
            return BT_STATUS_ERROR;
        }        
        wCount = 0;
        for (wIndex=0; wIndex<MUSB_BT_DEVICE_NUM; wIndex++)
        {
            pBtDevice = &MGC_arBtDevice[wIndex];
            if (pBtDevice->pUsbDevice != NULL)
            {
                wCount ++;
            }
        }
        
        *((uint32_t *)pBuffer) = (uint32_t)wCount;        
        break;

    case MUSB_BT_IOCTL_GET_DEVICE_STATUS: 
        // Check device is initial ok or not.
        if ((!pBuffer) || (u4Index >= MUSB_BT_DEVICE_NUM))
        { /* avoid writing into non existing buffers */
            return BT_STATUS_ERROR;
        }
        pBtDevice = &MGC_arBtDevice[u4Index];
        if (pBtDevice->pUsbDevice == NULL)
        { 
            return BT_STATUS_ERROR;
        }

        // Check status is ok for transmit and receive.
        *((uint32_t *)pBuffer) = 
            (uint32_t)(pBtDevice->bState >= MUSB_BT_STATE_SET_IDLE);
        break;

    case MUSB_BT_IOCTL_SET_HOTSWAP_NFY:
        // Check device is initial ok or not.
        if ((!pBuffer) || (u4Index >= MUSB_BT_DEVICE_NUM))
        { /* avoid writing into non existing buffers */
            return BT_STATUS_ERROR;
        }
        pBtDevice = &MGC_arBtDevice[u4Index];
        
        pt_hotswap_nfy_info = (MUSB_HOTSWAP_NFY_INFO_T *)pBuffer;
        pBtDevice->pt_socket_nfy_info = pt_hotswap_nfy_info;

        // call back to upper layer when device already insert.
        if ((pBtDevice->pUsbDevice) && 
            (pBtDevice->pPort) &&
            (pBtDevice->bState == MUSB_BT_STATE_SET_IDLE))
        {
            if (pt_hotswap_nfy_info->pfNfy)
            {
                pt_hotswap_nfy_info->pvDevice = (void *)pBtDevice->pUsbDevice;
                pt_hotswap_nfy_info->u4Status = (uint32_t)HFI_STATUS_READY;
                pt_hotswap_nfy_info->u4SocketNo = (uint32_t)pBtDevice->pUsbDevice->bHubPort;
                pt_hotswap_nfy_info->u4UsbPort = pBtDevice->pPort->bUsbPort;                        
                pt_hotswap_nfy_info->pfNfy(pt_hotswap_nfy_info->pvTag);
            }
        }                            
        break;
    case MUSB_BT_IOCTL_GET_IO_REQ_NUM:
        // Check device is initial ok or not.
        if ((!pBuffer) || (u4Index >= MUSB_BT_DEVICE_NUM))
        { /* avoid writing into non existing buffers */
            return BT_STATUS_ERROR;
        }
        
        ptBtIoReqNum = (MUSB_BT_IO_REQ_NUM_T*)pBuffer;        
        pBtDevice = &MGC_arBtDevice[u4Index];
        pServices = ((MGC_Port*)(pBtDevice->hBus))->pController->pSystemServices;
        ptBtIoReqNum->ui4_io_request_in_queue = 0;

        pServices->pfIrpLock(pServices->pPrivateData, 0);
        switch ( ptBtIoReqNum->ui1_traffic_type )
        {
            case MUSB_BT_XFER_CONTROL:
                ptBtIoReqNum->ui4_io_request_in_queue = 0;
                break;
                
            case MUSB_BT_XFER_ISOC:
                if ( ptBtIoReqNum->b_is_tx == TRUE )
                {
                    for (i=0; i<BT_MAX_ISOC_TX; i++)
                    {
                        if (pBtDevice->parBtIsoOutQueue[i] != NULL )
                        {
                            ptBtIoReqNum->ui4_io_request_in_queue++;
                        }
                    }
                }
                else
                {
                    for (i=0; i<BT_MAX_ISOC_RX; i++)
                    {
                        if (pBtDevice->parBtIsoInQueue[i] != NULL )
                        {
                            ptBtIoReqNum->ui4_io_request_in_queue++;
                        }
                    }
                }
                break;
                
            case MUSB_BT_XFER_BULK:
                if ( ptBtIoReqNum->b_is_tx == TRUE )
                {
                    for (i=0; i<BT_MAX_BULK_TX; i++)
                    {
                        if (pBtDevice->parBtBulkOutQueue[i] != NULL )
                        {
                            ptBtIoReqNum->ui4_io_request_in_queue++;
                        }
                    }
                }
                else
                {
                    for (i=0; i<BT_MAX_BULK_RX; i++)
                    {
                        if (pBtDevice->parBtBulkInQueue[i] != NULL )
                        {
                            ptBtIoReqNum->ui4_io_request_in_queue++;
                        }
                    }
                }
                break;
                
            case MUSB_BT_XFER_INT:
                ptBtIoReqNum->ui4_io_request_in_queue = 0;
                break;
                
            default:
                ASSERT(0);
                break;
        }
        (void)pServices->pfIrpUnlock(pServices->pPrivateData, 0);
        break;
                
    default:
        break;
    }

    UNUSED(u4Aux);
    return BT_STATUS_OK; /* Return with zero if no problems have occurred. */
}

static uint32_t MGC_BtIotransfer(uint32_t u4Index, MUSB_BT_IO_T *prBtIOData)
{
    uint32_t u4Result = BT_STATUS_OK;
    MGC_BtDevice *pBtDevice;

    if ((u4Index >= MUSB_BT_DEVICE_NUM) ||(prBtIOData == NULL))
    {
        return BT_STATUS_ERROR;
    }

    pBtDevice = &MGC_arBtDevice[u4Index];
    if ((pBtDevice->pUsbDevice == NULL) ||(pBtDevice->pPort == NULL))
    {
        return BT_STATUS_ERROR;
    }

    switch (prBtIOData->bTrafficType)
    {
        case MUSB_BT_XFER_CONTROL:
            VERIFY(OSR_OK == x_sema_lock(MGC_hBtSema, X_SEMA_OPTION_WAIT));
            u4Result = MGC_BtControl(pBtDevice, prBtIOData);
            VERIFY(OSR_OK == x_sema_unlock(MGC_hBtSema));            
            break;

        case MUSB_BT_XFER_ISOC:
            u4Result = MGC_BtIsoch(pBtDevice, prBtIOData);
            break;

        case MUSB_BT_XFER_BULK:
            VERIFY(OSR_OK == x_sema_lock(MGC_hBtSema, X_SEMA_OPTION_WAIT));           
            u4Result = MGC_BtBulk(pBtDevice, prBtIOData);
            VERIFY(OSR_OK == x_sema_unlock(MGC_hBtSema));            
            break;
            
        case MUSB_BT_XFER_INT:
            u4Result = MGC_BtInterrupt(pBtDevice, prBtIOData);
            break;

        default:
            break;
    }

    return u4Result;
}

static uint32_t MGC_BtIoinit(uint32_t u4Index)
{
    if (!MGC_bBtInit)
    {
        MGC_bBtInit = TRUE;

        // create sync semphore.
        VERIFY(OSR_OK == x_sema_create((HANDLE_T *)&MGC_hBtSema, 
            X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK));
    }

    UNUSED(u4Index);    
    return BT_STATUS_OK;
}

uint16_t MUSB_FillBtClassPeripheralList(uint8_t *pList, uint16_t wListLength)
{
    uint16_t wResult = MUSB_MIN((uint16_t)sizeof(MGC_aBtPeripheralList), wListLength);

    MUSB_MemCopy(pList, MGC_aBtPeripheralList, wResult);
    return wResult;
}

MUSB_DeviceDriver *MUSB_GetBtClassDriver()
{
    return &MGC_BtDeviceDriver;
}

MU_BT_DEVICE_TYPE *MUSB_BtDriverGet(void)
{
    return (MU_BT_DEVICE_TYPE *)&MGC_BtHandleDriver;
}

#endif /* CC_USB_CSR_BLUETOOTH */
#endif /* MUSB_LOADER */

