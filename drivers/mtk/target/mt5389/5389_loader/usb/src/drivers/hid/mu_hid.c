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
 * Top-level Hid class driver module
 * $Revision: #1 $
 */
#ifdef MUSB_HID

#include "mu_bits.h"
#include "mu_cdi.h"
#include "mu_descs.h"
#include "mu_diag.h"
#include "mu_mem.h"
#include "mu_stdio.h"
#include "mu_impl.h"

#include "mu_hid_if.h"
#include "mu_hid_descr.h"

#define MGC_HID_GAP_TIME   (20)

#define MGC_HID_DEVICE_NUM   (2)

/**************************** GLOBALS *****************************/
/** Connect handler for HID class driver */
static uint8_t MGC_HidConnect(void *pPrivateData, MUSB_BusHandle hBus, 
    MUSB_Device *pUsbDevice, const uint8_t *pPeripheralList);

/** Disconnect Handler for HID Device Driver */
static uint8_t MGC_HidDisconnect(void *pPrivateData, 
    MUSB_BusHandle hBus, MUSB_Device *pUsbDevice);

static void MGC_HidCtrlCallback(void *pContext, MUSB_ControlIrp *pControlIrp);

static const uint8_t MGC_aHidPeripheralList [] =
{
    (uint8_t)MUSB_TARGET_CLASS, MUSB_CLASS_PER_INTERFACE, (uint8_t)MUSB_TARGET_INTERFACE, 0,
    (uint8_t)MUSB_TARGET_CLASS, MUSB_CLASS_HID, (uint8_t)MUSB_TARGET_ACCEPT
};

/** Mass Storage device driver */
static MUSB_DeviceDriver MGC_HidDeviceDriver =
{
    NULL, MGC_HidConnect, MGC_HidDisconnect, NULL, NULL,
};

static MGC_HidDevice *MGC_arHidDevice[MGC_HID_DEVICE_NUM];

/*************************** FUNCTIONS ****************************/

/**
 * Allocate per-device data
 */
static MGC_HidDevice *MGC_HidDeviceInit(void)
{
    MGC_HidDevice *pHidDevice;

    pHidDevice = (MGC_HidDevice *)MUSB_MemAlloc(sizeof(MGC_HidDevice));
    if (NULL == pHidDevice)
    {
        LOG(1, "Hid Err: Insufficient memory.\n", NULL);
        return (NULL);
    }
    
    MUSB_MemSet((void *)pHidDevice, 0, sizeof(MGC_HidDevice));
    pHidDevice->pDriver = &MGC_HidDeviceDriver;
    
    return pHidDevice;
}

static uint32_t MGC_HidSetReportIdle(MGC_HidDevice *pHidDevice)
{
    MUSB_DeviceRequest *pSetup;
    MUSB_ControlIrp *pControlIrp;
    uint32_t dwStatus;

    MUSB_ASSERT(pHidDevice);

    pHidDevice->bNextState = MUSB_HID_STATE_SET_IDLE;
    
    pSetup = (MUSB_DeviceRequest *)pHidDevice->aSetupTx;
    pControlIrp = &(pHidDevice->ControlIrp);

    /** Prepare the Setup Packet for sending Set Interface Alternative Request */
    MGC_PREPARE_SETUP_PACKET(pSetup, 
        (MUSB_DIR_OUT | MUSB_TYPE_CLASS| MUSB_RECIP_INTERFACE),
        MUSB_REQ_SET_IDLE, 
        0,  /* wValue: 0 is meaning that the duration is indefinite. */
        pHidDevice->pIface->bInterfaceNumber,  /* wIndex: interface. */ 
        0 /* wLength: */);
    
    /** Fill Control Irp */
    MGC_FILL_CONTROL_IRP(pHidDevice, 
        pControlIrp, 
        (uint8_t *)pSetup, 
        8, /* Only setup phase data */ 
        NULL, 
        0, 
        MGC_HidCtrlCallback);

    dwStatus = MUSB_StartControlTransfer(pHidDevice->pUsbDevice->pPort, pControlIrp);
    if (dwStatus)
    {
        /* Log an Error and return state */
        LOG(1, "Hid Err: Set idle failed, dwStatus: 0x%X.\n", dwStatus);
    }

    return (dwStatus);
}

static uint32_t MGC_HidGetReportDscr(MGC_HidDevice *pHidDevice)
{
    MUSB_DeviceRequest *pSetup;
    MUSB_ControlIrp *pControlIrp;
    uint32_t dwStatus;

    MUSB_ASSERT(pHidDevice);

    LOG(5, "Hid: GetReportDscr=0x%08X.\n", (uint32_t)pHidDevice);

    pHidDevice->bNextState = MUSB_HID_STATE_REPORT_DESCR;
    
    pSetup = (MUSB_DeviceRequest *)pHidDevice->aSetupTx;
    pControlIrp = &(pHidDevice->ControlIrp);

    /** Prepare the Setup Packet for sending Set Interface Alternative Request */
    MGC_PREPARE_SETUP_PACKET(pSetup, 
        (MUSB_DIR_IN | MUSB_TYPE_STANDARD | MUSB_RECIP_INTERFACE),
        MUSB_REQ_GET_DESCRIPTOR, 
        (pHidDevice->pHidDescr->bClassDescriptorType << 8),
        pHidDevice->pIface->bInterfaceNumber,
        pHidDevice->pHidDescr->wDescriptorLength);
    
    /** Fill Control Irp */
    MGC_FILL_CONTROL_IRP(pHidDevice, 
        pControlIrp, 
        (uint8_t *)pSetup, 
        8, /* Only setup phase data */ 
        pHidDevice->pReportDescr, 
        pHidDevice->pHidDescr->wDescriptorLength,
        MGC_HidCtrlCallback);

    dwStatus = MUSB_StartControlTransfer(pHidDevice->pUsbDevice->pPort, pControlIrp);
    if (dwStatus)
    {
        /* Log an Error and return state */
        LOG(1, "Hid Err: Get report failed, dwStatus: 0x%X.\n", dwStatus);
    }

    return (dwStatus);
}

static void MGC_HidIntrIrpCallback(void *pContext, const MUSB_Irp *pIntrIrp)
{
    MGC_HidDevice *pHidDevice;
    uint32_t dwStatus;
    
    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pIntrIrp);

    if (pIntrIrp->dwStatus)
    {
        LOG(1, "Hid Err: pIntrIrp->dwStatus=0x%X\n", pIntrIrp->dwStatus);
        return;
    }

    pHidDevice = (MGC_HidDevice *)pContext;

    if (pHidDevice->bState != pHidDevice->bNextState)
    {
        pHidDevice->bState = pHidDevice->bNextState;
    }  

    switch (pHidDevice->bHidType)
    {
        case GDP_USAGE_JOYSTICK: 
        case GDP_USAGE_GAMEPAD: 
            dwStatus = MUSB_JoystickEvent(pHidDevice);
            if (dwStatus)
            {
                LOG(1, "Hid Err: MUSB_JoystickEvent status=0x%X\n", dwStatus);
            }
            break;
        default:
            break;
    }        

    return;
} /* End of function MGC_HubCtrlReqComplete () */

static void MGC_HidReportConnect(MGC_HidDevice *pHidDevice, uint8_t bSupport)
{
    MGC_EnumerationData *pEnumData;
    MGC_Port* pPort;
    MUSB_Device *pUsbDevice;

    MUSB_ASSERT(pHidDevice);
    MUSB_ASSERT((bSupport == MUSB_STATUS_CHILD_CONNECT) ||
     (bSupport == MUSB_STATUS_CHILD_CONNECT_UNSUPPORT));
    
    pPort = (MGC_Port *)pHidDevice->hBus;
    MUSB_ASSERT(pPort);
    pUsbDevice = pHidDevice->pUsbDevice;
    MUSB_ASSERT(pUsbDevice);    

    // Report device to user's call back function.
    pUsbDevice->bPortCount = 1;
    pUsbDevice->bSupport = bSupport;

    pEnumData = &(pPort->EnumerationData);
    if (pEnumData->pfChildDeviceUpdate)
    {
        pEnumData->pfChildDeviceUpdate(pUsbDevice->pParentUsbDevice,
            pUsbDevice, bSupport);
    }
}

static uint32_t MGC_HidSubmitIntrIrp(MGC_HidDevice *pHidDevice)
{
    MUSB_Irp *pIntrInIrp;
    uint32_t dwStatus;

    MUSB_ASSERT(pHidDevice);

    if (!pHidDevice->hIntInPipe)
    {
        return MGC_M_STATUS_ERROR;
    }

    pHidDevice->bState = MUSB_HID_STATE_START_IN;
    pHidDevice->bNextState = MUSB_HID_STATE_START_IN;
    
    pIntrInIrp = &(pHidDevice->intrInIrp);

    /* Fill the Interrupt IRP */
    pIntrInIrp->hPipe = pHidDevice->hIntInPipe;
    pIntrInIrp->bAllowShortTransfer = FALSE;
    pIntrInIrp->pBuffer = pHidDevice->aIntInBuf;
    pIntrInIrp->dwLength = pHidDevice->pEnd->wMaxPacketSize;
    pIntrInIrp->pfIrpComplete = MGC_HidIntrIrpCallback;
    pIntrInIrp->pCompleteParam = (void *)pHidDevice;
    pIntrInIrp->bIsrCallback = TRUE;

    dwStatus = MUSB_StartTransfer(pIntrInIrp);
    if (dwStatus)
    {
        LOG(1, "Hid Err: interrupt transfer status=0x%X\n", dwStatus);
        return dwStatus;
    }

    return dwStatus;
} /* End of function  MGC_HubFillAndSubmitIntrIrp () */

static void MGC_HidCtrlCallback(void *pContext, MUSB_ControlIrp *pControlIrp)
{
    MGC_HidDevice *pHidDevice = NULL;
    uint32_t status;
    
    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pControlIrp);
   
    pHidDevice = (MGC_HidDevice *)pContext;

    /* Since IRP is executed properly hence update hub state to its next state.
     */
    pHidDevice->bState = pHidDevice->bNextState;
    LOG(5, "Hid State: %d.\n", pHidDevice->bState);

    switch (pHidDevice->bState)
    {
        case MUSB_HID_STATE_SET_IDLE:             
            if (MUSB_STATUS_OK != pControlIrp->dwStatus)
            {
                LOG(1, "Hid Err: Callback Status = 0x%X.\n", pControlIrp->dwStatus);
                return;
            }

            status = MGC_HidGetReportDscr(pHidDevice);
            if (MUSB_STATUS_OK != status)
            {
                LOG(1, "Hid Err: MGC_HidGetReportDscr Error.\n", NULL);
                return;
            }

            pHidDevice->bNextState = MUSB_HID_STATE_REPORT_DESCR;            
            break;
    
        case MUSB_HID_STATE_REPORT_DESCR:             
            if (MUSB_STATUS_OK != pControlIrp->dwStatus)
            {
                LOG(1, "Hid Err: Callback Status = 0x%X.\n", pControlIrp->dwStatus);
                return;
            }

            status = MUSB_ParseReportDescr(pHidDevice);
            if (MUSB_STATUS_OK != status)
            {
                LOG(1, "Hid Err: ParseReportDescr.\n", NULL);
                MGC_HidReportConnect(pHidDevice, MUSB_STATUS_CHILD_CONNECT_UNSUPPORT);            
                return;
            }                        
            
            // Init and regiter handler.
            switch (pHidDevice->bHidType)
            {
                case GDP_USAGE_JOYSTICK: 
                case GDP_USAGE_GAMEPAD: 
                    // report to upper layer device connect.
                    MGC_HidReportConnect(pHidDevice, MUSB_STATUS_CHILD_CONNECT);
                    
                    MUSB_JoystickInit();
                    MUSB_JoystickRegister(pHidDevice);
                    break;
                default:
                    LOG(1, "Hid Err: Unsupport hid type = %d.\n", pHidDevice->bHidType);
                    MGC_HidReportConnect(pHidDevice, MUSB_STATUS_CHILD_CONNECT_UNSUPPORT);
                    return;
            }        
            
            status = MGC_HidSubmitIntrIrp(pHidDevice);
            if (MUSB_STATUS_OK != status)
            {
                LOG(1, "Hid Err: MGC_HidSubmitIntrIrp.\n", NULL);
                return;
            }
            break;
                        
        default:
            LOG(1, "Hid Err: State = %d.\n", pHidDevice->bState);                        
            break;
    }    
}

static void MGC_HidTimerExpired(void *pControllerPrivateData, uint16_t wTimerIndex)
{
    MGC_Port *pPort;
    MUSB_Device *pDevice = NULL;
    uint16_t wCount = 0;
    uint16_t wIndex = 0;
    MGC_HidDevice *pHidDevice;
    
    MUSB_ASSERT(pControllerPrivateData);
    MUSB_ASSERT(wTimerIndex == MUSB_HID_TIMER_IDX);

    pPort = (MGC_Port *)pControllerPrivateData;
    MUSB_ASSERT(pPort->pController);

    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));

    for (wIndex=0; wIndex < wCount; wIndex++)
    {
        pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), wIndex);

        if ((pDevice->bSupport == MUSB_STATUS_CHILD_UNKNOW) &&        
            (pDevice->bDeviceClass == MUSB_CLASS_HID) &&
            (pDevice->pDriverPrivateData))
        {
            // temp setting.
            pDevice->bSupport = MUSB_STATUS_CHILD_CONNECT;
        
            pHidDevice = (MGC_HidDevice *)pDevice->pDriverPrivateData;    

            if (MUSB_STATUS_OK != MGC_HidSetReportIdle(pHidDevice))
            {
                LOG(1, "Hid Err: MGC_HidSetReportIdle Error.\n", NULL);
                return;
            }
        }
    }
}

/** Callback function when device acknowledges set config reqeust. */
static void MGC_HidSetConfigCallback(void *pContext, MUSB_ControlIrp *pControlIrp)
{
    MGC_HidDevice *pHidDevice;
    MGC_Port *pPort;
    MUSB_SystemServices *pServices;

    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pControlIrp);
   
    if (MUSB_STATUS_OK != pControlIrp->dwStatus)
    {
        LOG(1, "Hid Err: Set Config Callback Status = 0x%X.\n", pControlIrp->dwStatus);
        return;
    }

    pHidDevice = (MGC_HidDevice *)pContext;

    /* Since IRP is executed properly hence update hub state to its next state.
     */
    pHidDevice->bState = pHidDevice->bNextState;
    
    /* Set the Current Configuration Descriptor to Default as Set Config is Success */
    pHidDevice->pUsbDevice->pCurrentConfiguration = 
        pHidDevice->pUsbDevice->apConfigDescriptors[0];

    /* Device is connected */
    LOG(5, "Hid HubPort-%d, UsbDevice = 0x%08X Config OK.\n", 
        pHidDevice->pUsbDevice->bHubPort, (uint32_t)pHidDevice->pUsbDevice);

    MUSB_ASSERT(pHidDevice->hBus);
    pPort = (MGC_Port *)pHidDevice->hBus;
    MUSB_ASSERT(pPort->pController);
    pServices = pPort->pController->pSystemServices;
    MUSB_ASSERT(pServices);    

    //Timer Index MUSB_HID_TIMER_IDX is reserved for HID class driver.
    /* kill timer */
    (void)pServices->pfCancelTimer(pServices->pPrivateData, 
        MUSB_HID_TIMER_IDX/*bTimerIndex*/);
    
    (void)pServices->pfArmTimer(pServices->pPrivateData, 
        MUSB_HID_TIMER_IDX/*bTimerIndex*/, 
        MGC_HID_GAP_TIME/*dwTime*/, FALSE, MGC_HidTimerExpired, pPort);    
}

/** After getting connect callback,  device driver calls this function to configure device */
static uint32_t MGC_HidConfigureDevice(MGC_HidDevice *pHidDevice)
{
    MUSB_DeviceRequest *pSetup;
    MUSB_ControlIrp *pControlIrp;
    uint32_t dwStatus;

    MUSB_ASSERT(pHidDevice);
    
    pSetup = (MUSB_DeviceRequest *)pHidDevice->aSetupTx;
    pControlIrp = &(pHidDevice->ControlIrp);

    /** Prepare the Setup Packet for sending Set Config Request */
    MGC_PREPARE_SETUP_PACKET(pSetup, 
        (MUSB_DIR_OUT | MUSB_TYPE_STANDARD | MUSB_RECIP_DEVICE),
        MUSB_REQ_SET_CONFIGURATION, 
        pHidDevice->pConfig->bConfigurationValue,
        0, 
        0);
    
    /** Fill Control Irp */
    MGC_FILL_CONTROL_IRP(pHidDevice, 
        pControlIrp, 
        (uint8_t *)pSetup, 
        8,  /* Only setup phase data */ 
        NULL, 
        0, 
        MGC_HidSetConfigCallback);

    dwStatus = MUSB_StartControlTransfer(pHidDevice->pUsbDevice->pPort, pControlIrp);
    if (dwStatus)
    {
        /* Log an Error and return state */
        LOG(1, "Hid Err: Set Configuration Request failed = 0x%X.\n", dwStatus);
    }

    return (dwStatus);
} /* End MGC_MsdConfigureDevice () */

/** This function is called when Hid device is connected.*/
static uint8_t MGC_HidConnect(void *pPrivateData, MUSB_BusHandle hBus, 
    MUSB_Device *pUsbDevice, const uint8_t *pPeripheralList)
{
    MGC_HidDevice *pHidDevice;
    uint8_t bIndex;
    uint8_t bSupport;
    MUSB_InterfaceDescriptor *pIface = NULL;
    MUSB_ConfigurationDescriptor *pConfig;
    MUSB_DeviceEndpoint rEnd;
    MUSB_EndpointDescriptor *pEnd = NULL;
    MUSB_Pipe intrInPipe = NULL;
    MUSB_HidDescriptor *pHidDescr;
    uint8_t bInterval;
    
    MUSB_ASSERT(pUsbDevice);   
    UNUSED(pPrivateData);
    UNUSED(pPeripheralList);

    /* Device is connected */
    LOG(5, "Hid Device Connected.\n", NULL);

    pConfig = (MUSB_ConfigurationDescriptor *)pUsbDevice->apConfigDescriptors[0];
    if (!pConfig)
    {
        return FALSE;
    }    
       
    /* find first interface with supported subclass/protocol combination */
    for (bIndex = 0; ((pIface==NULL) && (bIndex < pConfig->bNumInterfaces)); bIndex++)
    {
        pIface = (MUSB_InterfaceDescriptor *)
            MUSB_FindInterfaceDescriptor(pConfig, bIndex, 0);

        if (pIface && (MUSB_CLASS_HID == pIface->bInterfaceClass))
        {
            switch (pIface->bInterfaceSubClass)
            {
                case 0:
                    LOG(5, "Inf-%d: Normal subclss.\n", bIndex);
                break;
                case 1:
                    LOG(1, "Inf-%d: Boot Interface subclss.\n", bIndex);
                break;
                default:
                    LOG(5, "Inf-%d: Reserved subclss=%d.\n", bIndex, pIface->bInterfaceSubClass);
                    pIface = NULL;
                break;
            }
        }
    }

    if (!pIface)
    {
        return FALSE;
    }    

    pHidDescr = (MUSB_HidDescriptor *)((uint32_t)MUSB_FindDescriptor(pConfig, 
        MUSB_HID_DESCRIPTOR, pIface->bInterfaceNumber));
    if (!pHidDescr)
    {
        return FALSE;
    }    
    LOG(5, "bCountryCode=%d.\n", pHidDescr->bCountryCode);
    LOG(5, "bNumDescriptors=%d.\n", pHidDescr->bNumDescriptors);

    if (pHidDescr->bNumDescriptors == 0)
    {
        // At least one report descriptor must be presented.
        return FALSE;
    }    

    LOG(5, "bClassDescriptorType=0x%X.\n", pHidDescr->bClassDescriptorType);
    LOG(5, "wDescriptorLength=%d bytes.\n", pHidDescr->wDescriptorLength);

    if (pHidDescr->bNumDescriptors > 1)
    {
        LOG(5, "bNumDescriptors=%d, Not support optional descriptor.\n", 
            pHidDescr->bNumDescriptors);        
    }    

    for (bIndex = 0; bIndex < pIface->bNumEndpoints; bIndex++)
    {
        pEnd = (MUSB_EndpointDescriptor *)MUSB_FindEndpointDescriptor(pConfig, pIface, bIndex);

        if (MUSB_ENDPOINT_XFER_INT == (pEnd->bmAttributes & MUSB_ENDPOINT_XFERTYPE_MASK))
        {
            if (MUSB_DIR_IN == (pEnd->bEndpointAddress & MUSB_ENDPOINT_DIR_MASK))
            {
                /*
                PC use 8 ms base polling interval to polling hid device.
                In 82xx, we use 4 ms base to polling device.
                */
                bInterval = pEnd->bInterval/8;
                bInterval *= 4;
                LOG(5, "Interrupt interval Org=%d ms, new=%d ms.\n", pEnd->bInterval, bInterval);
                pEnd->bInterval = bInterval;

                MUSB_MemCopy((void *)(&(rEnd.UsbDescriptor)), (void *)pEnd, 0x07);

                //rEnd.wNakLimit = 0xFFFF; /* Disable the NAK Limit */
                rEnd.pDevice = pUsbDevice;
                intrInPipe = MUSB_OpenPipe(hBus, &rEnd, NULL);
                break;            
            }
        }
    }

    if (NULL == intrInPipe)
    {
        MUSB_DIAG_STRING(1, "Hid: Open Interrupt Pipe fail.");
        return FALSE;
    }

    pHidDevice = MGC_HidDeviceInit();
    if (!pHidDevice)
    {
        return FALSE;
    }    

    pHidDevice->pReportDescr = (uint8_t *)MUSB_MemAlloc(pHidDescr->wDescriptorLength);
    if (NULL == pHidDevice->pReportDescr)
    {
        LOG(1, "Hid Err: pReportDescr == NULL.\n", NULL);
        MUSB_MemFree(pHidDevice);
        return (NULL);
    }
    
    /* Initialize */
    pUsbDevice->pDriverPrivateData = pHidDevice;
    pHidDevice->pConfig = pConfig;
    pHidDevice->pIface = pIface;
    pHidDevice->pHidDescr = pHidDescr;    
    pHidDevice->pEnd = pEnd;
    pHidDevice->pUsbDevice = pUsbDevice;
    pHidDevice->hBus = hBus;
    pHidDevice->bState = MUSB_HID_STATE_CONNECT;
    pHidDevice->bNextState = MUSB_HID_STATE_CONFIGURED;
    pHidDevice->hIntInPipe = intrInPipe;

    bSupport = FALSE;
    for (bIndex=0; bIndex<MGC_HID_DEVICE_NUM; bIndex++)
    {
        if (MGC_arHidDevice[bIndex] == NULL)
        {
            MGC_arHidDevice[bIndex] = pHidDevice;
            pHidDevice->bDeviceIndex = bIndex;
            bSupport = TRUE;
            break;
        }
    }
    if (!bSupport)
    {
        return FALSE;
    }

    /* Configure the device */
    if (MUSB_STATUS_OK != MGC_HidConfigureDevice(pHidDevice))
    {
        return FALSE;
    }
    
    return TRUE;
} 

/** Disconnect Handler for Hid Device Driver */
static uint8_t MGC_HidDisconnect(void *pPrivateData, 
    MUSB_BusHandle hBus, MUSB_Device *pUsbDevice)
{
    MGC_HidDevice *pHidDevice;
    MGC_Port *pPort = NULL;
    MUSB_SystemServices *pServices = NULL;    
    MUSB_Device *pDevice = NULL;
    uint16_t wCount = 0;
    uint16_t wIndex = 0;
    uint8_t bStatus = FALSE;
    REPORT_STRUCT *pReport;
    ID_REPORT_STRUCT *pIdReport;
    BASIC_REPORT_STRUCT *pBasicReport;
    uint32_t i;
    uint32_t j;
    uint32_t k;

    MUSB_ASSERT(pUsbDevice);
    UNUSED(pPrivateData);

    pHidDevice = (MGC_HidDevice *)pUsbDevice->pDriverPrivateData;
    LOG(1, "Hid: Disconnect pUsbDevice = 0x%08X.\n", (uint32_t)pUsbDevice);

    /* Check against the USB device and bus handle */
    if ((!pHidDevice) || 
        (!hBus) || 
        (hBus != pHidDevice->hBus) || 
        (pUsbDevice != pHidDevice->pUsbDevice))
    {
        return FALSE;
    }

    if (pHidDevice->hIntInPipe)
    {
        MUSB_ClosePipe(pHidDevice->hIntInPipe);
    }

    switch (pHidDevice->bHidType)
    {
        case GDP_USAGE_JOYSTICK: 
        case GDP_USAGE_GAMEPAD: 
            MUSB_JoystickRelease(pHidDevice);
            break;
        default:
            break;
    }        

    for (wIndex=0; wIndex<MGC_HID_DEVICE_NUM; wIndex++)
    {
        if (MGC_arHidDevice[wIndex] == pHidDevice)
        {
            MGC_arHidDevice[wIndex] = NULL;
            break;
        }
    }

    // release id and basic report.
    for (i=0; i<3; i++)
    {
        pReport = &pHidDevice->rReport[i];
        for (j=0; (pReport && (j<pReport->bTotalId)); j++)
        {
            pIdReport = pReport->pIdReport[j];            
            if (pIdReport)
            {
                for (k=0; k<pIdReport->dwBasicReportNum; k++)
                {
                    pBasicReport = pIdReport->pBasicReport[k];
                    if (pBasicReport)
                    {
                        MUSB_MemFree(pBasicReport);
                    }                
                }
                MUSB_MemFree(pIdReport);
            }
        }            
    }
    
    MUSB_MemFree(pHidDevice->pReportDescr);
    MUSB_MemFree(pHidDevice);

    MUSB_ASSERT(hBus);
    pPort = (MGC_Port *)hBus;
    MUSB_ASSERT(pPort->pController);
    pServices = pPort->pController->pSystemServices;
    MUSB_ASSERT(pServices);    
    
    //Timer Index MUSB_HID_TIMER_IDX is reserved for HID class driver.
    /* kill timer */
    bStatus = pServices->pfCancelTimer(pServices->pPrivateData, 
        MUSB_HID_TIMER_IDX/*bTimerIndex*/);
    UNUSED(bStatus);

    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));

    // Check if any device is not handled by pProtocol->pfStartDevice().    
    for (wIndex=0; wIndex < wCount; wIndex++)
    {
        pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), wIndex);

        if ((pDevice != pUsbDevice) && 
            (pDevice->bSupport == MUSB_STATUS_CHILD_UNKNOW) &&        
            (pDevice->bDeviceClass == MUSB_CLASS_HID))
        {
            bStatus = pServices->pfArmTimer(pServices->pPrivateData, 
                MUSB_HID_TIMER_IDX/*bTimerIndex*/, 
                MGC_HID_GAP_TIME/*dwTime*/, FALSE, MGC_HidTimerExpired, pPort);    

            MUSB_ASSERT(bStatus);               
            break;
        }
    }

    pUsbDevice->pDriverPrivateData = NULL;
    LOG(5, "Hid Disconnect Successfully.\n", NULL);

   return TRUE;
} 

uint16_t MUSB_FillHidClassPeripheralList(uint8_t *pList, uint16_t wListLength)
{
    uint16_t wResult = MUSB_MIN((uint16_t)sizeof(MGC_aHidPeripheralList), wListLength);

    MUSB_MemCopy(pList, MGC_aHidPeripheralList, wResult);
    return wResult;
}

MUSB_DeviceDriver *MUSB_GetHidClassDriver()
{
    return &MGC_HidDeviceDriver;
}

int32_t MUSB_HidIoctl(uint32_t u4Index, uint32_t u4Cmd, uint32_t u4Aux, void *pBuffer)
{
    MGC_HidDevice *pHidDevice;
    uint16_t wCount;
    uint16_t wIndex;
    
    /* This check makes sure, the device number is within valid range */
    switch (u4Cmd)
    {
    case MUSB_HID_IOCTL_GET_DEVICE_NUM:
        if (!pBuffer)
        {            
            return -1;
        }        
        wCount = 0;
        for (wIndex=0; wIndex<MGC_HID_DEVICE_NUM; wIndex++)
        {
            if (MGC_arHidDevice[wIndex] != NULL)
            {
                wCount ++;
            }
        }
        *((uint32_t *)pBuffer) = (uint32_t)wCount;        
        break;

    case MUSB_HID_IOCTL_GET_DEVICE_PORT:
        if ((!pBuffer) || (u4Index >= MGC_HID_DEVICE_NUM))
        { /* avoid writing into non existing buffers */
            return -1;
        }
        pHidDevice = MGC_arHidDevice[u4Index];
        if (pHidDevice == NULL)
        { 
            return -1;
        }
        if (pHidDevice->bState != MUSB_HID_STATE_START_IN)
        { 
            return -1;
        }
        *((uint32_t *)pBuffer) = (uint32_t)pHidDevice->bHidDriverNum;
        break;

    case MUSB_HID_IOCTL_GET_DEVICE_STATUS: 
        // Check device is initial ok or not.
        if ((!pBuffer) || (u4Index >= MGC_HID_DEVICE_NUM))
        { /* avoid writing into non existing buffers */
            return -1;
        }
        pHidDevice = MGC_arHidDevice[u4Index];
        if (pHidDevice == NULL)
        { 
            return -1;
        }

        *((uint32_t *)pBuffer) = 
            (uint32_t)(pHidDevice->bState == MUSB_HID_STATE_START_IN);
        break;

    case MUSB_HID_IOCTL_GET_JOYSTICK_DATA: 
        // Get device report data from polling mode.
        if (!pBuffer)
        { /* avoid writing into non existing buffers */
            return -1;
        }
        
        if (!MUSB_JoystickGetData(u4Index, (uint8_t *)pBuffer))
        { /* avoid writing into non existing buffers */
            return -1;
        }
            
        break;

    case MUSB_HID_IOCTL_GET_JOYSTICK_NUM: 
        // Get device report data from polling mode.
        if (!pBuffer)
        { /* avoid writing into non existing buffers */
            return -1;
        }

        *((uint32_t *)pBuffer) = MUSB_JoystickCount();        
        break;

    case MUSB_HID_IOCTL_SET_JOYSTICK_INSERT_NOTIFY:
        if (!pBuffer)
        { /* avoid writing into non existing buffers */
            return -1;
        }
        
        MUSB_JoystickSetInsertNotify(pBuffer);       
        break;

    case MUSB_HID_IOCTL_SET_JOYSTICK_DATA_NOTIFY:
        if (!pBuffer)
        { /* avoid writing into non existing buffers */
            return -1;
        }
        
        MUSB_JoystickSetDataNotify(pBuffer);       
        break;
                
    default:
        break;
    }

    UNUSED(u4Aux);
    return 0; /* Return with zero if no problems have occurred. */
}

#endif /* MUSB_HID */

