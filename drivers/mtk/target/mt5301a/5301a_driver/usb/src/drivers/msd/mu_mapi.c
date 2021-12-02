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
 * Top-level mass-storage class driver module
 * $Revision: #1 $
 */

#include "mu_bits.h"
#include "mu_cdi.h"
#include "mu_descs.h"
#include "mu_diag.h"
#include "mu_hfi_if.h"
#include "mu_mem.h"
#include "mu_stdio.h"

#include "mu_mpcsi.h"
#include "mu_msd.h"
#include "mu_impl.h"
#include "mu_mapi.h"

#define MGC_MSD_CONFIG_TIME   (100)

extern MGC_MsdCmdSet *MGC_GetScsiCmdSet(void);

extern MGC_MsdProtocol *MGC_CreateBotInstance(MUSB_Device *pDevice,
                                              MUSB_BusHandle hBus,
                                              const MUSB_InterfaceDescriptor *pIfaceDesc,
                                              const MUSB_EndpointDescriptor *pInEnd,
                                              const MUSB_EndpointDescriptor *pOutEnd,
                                              MGC_MsdCmdSet *pCmdSet);

extern MGC_MsdProtocol *MGC_CreateCbtInstance(MUSB_Device *pDevice,
                                              MUSB_BusHandle hBus,
                                              const MUSB_InterfaceDescriptor *pIfaceDesc,
                                              const MUSB_EndpointDescriptor *pInEnd,
                                              const MUSB_EndpointDescriptor *pOutEnd,
                                              MGC_MsdCmdSet *pCmdSet);

extern void MGC_DestroyBotInstance(const MGC_MsdProtocol* pProtocol);

extern void MGC_DestroyCbtInstance(const MGC_MsdProtocol* pProtocol);

void MUSB_MsdBusResumed(void *pPrivateData, MUSB_BusHandle hBus);

static uint32_t MGC_MsdConfigureDevice(MGC_MsdDevice *pMsdDevice);
static void MGC_MsdTimerExpired(void *pControllerPrivateData, uint16_t wTimerIndex);
static void MGC_MsdSetConfigCallback(void *pContext, MUSB_ControlIrp *pControlIrp);

/**************************** GLOBALS *****************************/
static const uint8_t MGC_aMsdPeripheralList [] =
{
    (uint8_t)MUSB_TARGET_CLASS, MUSB_CLASS_PER_INTERFACE, (uint8_t)MUSB_TARGET_INTERFACE, 0,
    (uint8_t)MUSB_TARGET_CLASS, MUSB_CLASS_MASS_STORAGE, (uint8_t)MUSB_TARGET_ACCEPT
};

/** Mass Storage device driver */
MUSB_DeviceDriver MGC_MsdDeviceDriver =
{
    NULL, MUSB_MsdConnect, MUSB_MsdDisconnect, NULL, NULL,
};

/*************************** FUNCTIONS ****************************/

uint16_t MGC_FillStorageClassPeripheralList(uint8_t *pList, uint16_t wListLength)
{
    uint16_t wResult = MUSB_MIN((uint16_t)sizeof(MGC_aMsdPeripheralList), wListLength);

    MUSB_MemCopy(pList, MGC_aMsdPeripheralList, wResult);
    return wResult;
}

MUSB_DeviceDriver *MGC_GetStorageClassDriver()
{
    return &MGC_MsdDeviceDriver;
}

/**
 * Allocate per-device data
 */
static MGC_MsdDevice *MGC_MsdDeviceInit(void)
{
    MGC_MsdDevice *pMsdDevice;

    pMsdDevice = (MGC_MsdDevice *)MUSB_MemAlloc(sizeof(MGC_MsdDevice));

    if (NULL == pMsdDevice)
    {
        MUSB_DIAG_STRING(1, "MSD Error: Insufficient memory");
        return (NULL);
    }

    MUSB_MemSet((void *)pMsdDevice, 0, sizeof(MGC_MsdDevice));

    pMsdDevice->pDriver = &MGC_MsdDeviceDriver;

    MUSB_DIAG_STRING(2, "MSD: Initialization Completed");

    return pMsdDevice;
}

/** This function is called when Mass Storage device is connected.*/
uint8_t MUSB_MsdConnect(void *pPrivateData, MUSB_BusHandle hBus, MUSB_Device *pUsbDevice,
                        const uint8_t *pPeripheralList)
{
    uint8_t bIndex, bEnd;

    uint8_t bSubclass = 0, bProtocol = 0xFF;
    const MUSB_InterfaceDescriptor *pIface;
    const MUSB_EndpointDescriptor *pEnd;
    const MUSB_EndpointDescriptor *pInEnd;
    const MUSB_EndpointDescriptor *pOutEnd;
    MGC_MsdDevice *pMsdDevice;
    MGC_MsdProtocol *pProtocol = NULL;
    MGC_MsdCmdSet *pCmdSet = NULL;
    /* assume first config */
    const MUSB_ConfigurationDescriptor *pConfig;
    uint8_t bStatus = FALSE;
    
    MUSB_ASSERT(pUsbDevice);
    pConfig = pUsbDevice->apConfigDescriptors[0];

    UNUSED(pPrivateData);
    UNUSED(pPeripheralList);
    
    /* Device is connected */
    MUSB_DIAG_STRING(2, "Mass Storage Device Connected");

    /* find first interface with supported subclass/protocol combination */
    for (bIndex = 0; bIndex < pConfig->bNumInterfaces; bIndex++)
    {
        /* assume no alternates */
        pIface = MUSB_FindInterfaceDescriptor(pConfig, bIndex, 0);
        if (pIface && (MUSB_CLASS_MASS_STORAGE == pIface->bInterfaceClass)
                && (pIface->bNumEndpoints >= 2))
        {
            bSubclass = pIface->bInterfaceSubClass;

            bProtocol = pIface->bInterfaceProtocol;
            pInEnd = NULL;
            pOutEnd = NULL;

            LOG(5, "MSD: InterfaceClass = 0x%X, Subclass = 0x%X, Protocol = 0x%X\n",
                pIface->bInterfaceClass, bSubclass, bProtocol);
            
            /* BOT/CB/CBI: find bulk-in & bulk-out ends */
            for (bEnd = 0; bEnd < pIface->bNumEndpoints; bEnd++)
            {
                pEnd = MUSB_FindEndpointDescriptor(pConfig, pIface, bEnd);
                if (pEnd && (MUSB_ENDPOINT_XFER_BULK
                                 == (pEnd->bmAttributes&MUSB_ENDPOINT_XFERTYPE_MASK)))
                {
                    if (MUSB_ENDPOINT_DIR_MASK & pEnd->bEndpointAddress)
                    {
                        pInEnd = pEnd;
                    }
                    else
                    {
                        pOutEnd = pEnd;
                    }
                }
            }
            /* continue if not found */
            if (!pInEnd || !pOutEnd)
            {
                MUSB_DIAG1(1, "MSD: insufficient endpoints for BOT in interface ", bIndex, 10,
                           0);
                continue;
            }
            /* determine command-set or continue on unsupported */
            switch (bSubclass)
            {
            case MGC_MSD_SCSI_SUBCLASS:
                pCmdSet = MGC_GetScsiCmdSet();
                break;

            //  MTK Notice: Max Liao, 2006/11/17.
            //  Device may report SFF-8070i (0x05) Subclass code. Use SCSI to suport it.		    
            default:
                MUSB_DIAG1(1, "MSD: Abnormal subclass code Notice.", bIndex, 10, 0);
                pCmdSet = MGC_GetScsiCmdSet();
                break;
            }

            if (MGC_MSD_BOT_PROTOCOL == bProtocol)
            {
                pProtocol = MGC_CreateBotInstance(pUsbDevice, hBus, pIface, pInEnd, pOutEnd,
                                              pCmdSet);
            }
            else if ((MGC_MSD_CB_PROTOCOL == bProtocol) ||(MGC_MSD_CBI_PROTOCOL == bProtocol))
            {
                pProtocol = MGC_CreateCbtInstance(pUsbDevice, hBus, pIface, pInEnd, pOutEnd,
                                              pCmdSet);
            }
        }
    }

    if (pProtocol)
    {  
        pProtocol->bProtocol = bProtocol;        
        pMsdDevice = MGC_MsdDeviceInit();
        if (pMsdDevice)
        {
            pMsdDevice->pProtocol = pProtocol;

            pUsbDevice->pDriverPrivateData = pMsdDevice;

            /* Initialize */
            pMsdDevice->pUsbDevice = pUsbDevice;
            pMsdDevice->hBus = hBus;

            /* Configure the device */
            if (MUSB_STATUS_OK == MGC_MsdConfigureDevice(pMsdDevice))
            {
                bStatus = TRUE;                          
            }
        }
    }
    else
    {
        MUSB_DIAG_STRING(1,
                         "MSD Error: No interface has supported subclass/protocol");
    }

    return (bStatus);
} /* End MUSB_MsdConnect() */

/** Disconnect Handler for Mass Storage Device Driver */
uint8_t MUSB_MsdDisconnect(void *pPrivateData, MUSB_BusHandle hBus, MUSB_Device *pUsbDevice)
{
    MGC_MsdDevice *pMsdDevice;
    MGC_MsdProtocol* pProtocol;

    MGC_Port *pPort = NULL;
    MUSB_SystemServices *pServices = NULL;    
    MUSB_Device *pDevice = NULL;
    uint16_t wCount = 0;
    uint16_t wIndex = 0;
    uint8_t bStatus = FALSE;

    MUSB_ASSERT(pUsbDevice);
    pMsdDevice = (MGC_MsdDevice *)pUsbDevice->pDriverPrivateData;

    UNUSED(pPrivateData);

    /* Check against the USB device and bus handle */
    if ((!pMsdDevice) || 
        (!hBus) || 
        (hBus != pMsdDevice->hBus) || 
        (pUsbDevice != pMsdDevice->pUsbDevice))
    {
        MUSB_DIAG_STRING(1, "MSD Error: MSD Device Disconnect Callback");

        return FALSE;
    }

    // init device list for each port.   
    pProtocol = pMsdDevice->pProtocol;
    MUSB_ASSERT(pProtocol);
    /* Release the resources held */
    if (MGC_MSD_BOT_PROTOCOL == pProtocol->bProtocol)
    {
        MGC_DestroyBotInstance(pProtocol);
    }
    else if ((MGC_MSD_CB_PROTOCOL == pProtocol->bProtocol) ||
                (MGC_MSD_CBI_PROTOCOL == pProtocol->bProtocol))
    {
        MGC_DestroyCbtInstance(pProtocol);
    }

    MUSB_MemFree(pMsdDevice);

    MUSB_ASSERT(hBus);
    pPort = (MGC_Port *)hBus;
    MUSB_ASSERT(pPort->pController);
    pServices = pPort->pController->pSystemServices;
    MUSB_ASSERT(pServices);    
    
    //Timer Index MUSB_MSD_CONF_TIMER_IDX is reserved for Mass storage class driver.
    /* kill timer */
    bStatus = pServices->pfCancelTimer(pServices->pPrivateData, 
        MUSB_MSD_CONF_TIMER_IDX/*bTimerIndex*/);
    UNUSED(bStatus);

    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));

    // Check if any device is not handled by pProtocol->pfStartDevice().    
    for (wIndex=0; wIndex < wCount; wIndex++)
    {
        pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), wIndex);

        if ((pDevice) && 
            (pDevice != pUsbDevice) && 
            (pDevice->bSupport == MUSB_STATUS_CHILD_UNKNOW) &&        
            (pDevice->bDeviceClass == MUSB_CLASS_MASS_STORAGE))
        {
            bStatus = pServices->pfArmTimer(pServices->pPrivateData, 
                MUSB_MSD_CONF_TIMER_IDX/*bTimerIndex*/, 
                MGC_MSD_CONFIG_TIME/*dwTime*/, FALSE, MGC_MsdTimerExpired, pPort);    

            MUSB_ASSERT(bStatus);               
            break;
        }
    }

    pUsbDevice->pDriverPrivateData = NULL;
    MUSB_DIAG_STRING(2, "Mass Storage Device Disconnected Successfully");

   return TRUE;
} /* End MUSB_MsdDisconnect () */

/** After getting connect callback,  device driver calls this function to configure device */
static uint32_t MGC_MsdConfigureDevice(MGC_MsdDevice *pMsdDevice)
{
    MUSB_DeviceRequest *pSetup;

    MUSB_ControlIrp *pControlIrp;
    uint32_t dwStatus;

    MUSB_ASSERT(pMsdDevice);
    
    pSetup = &(pMsdDevice->Setup);
    pControlIrp = &(pMsdDevice->ControlIrp);

    /** Prepare the Setup Packet for sending Set Config Request */
    MGC_MSD_PREPARE_SETUP_PACKET(pSetup, (MUSB_DIR_OUT | MUSB_TYPE_STANDARD | MUSB_RECIP_DEVICE),
                                 MUSB_REQ_SET_CONFIGURATION, 
                                 pMsdDevice->pUsbDevice->apConfigDescriptors[0]->bConfigurationValue,
                                 0, 0);
    /** Fill Control Irp */
    MGC_MSD_FILL_CONTROL_IRP(pMsdDevice, pControlIrp, (uint8_t *)pSetup, sizeof(MUSB_DeviceRequest),
                             NULL, 0, MGC_MsdSetConfigCallback);

    dwStatus = MUSB_StartControlTransfer(pMsdDevice->pUsbDevice->pPort, pControlIrp);

    if (dwStatus)
    {
        /* Log an Error and return state */
        MUSB_DIAG1(1, "MSD Error: Set Configuration Request failed, dwStatus: ",
                   dwStatus, 16,
                   FALSE);
    }

    return (dwStatus);
} /* End MGC_MsdConfigureDevice () */

static void MGC_MsdTimerExpired(void *pControllerPrivateData, uint16_t wTimerIndex)
{
    MGC_Port *pPort;
    MUSB_Device *pDevice = NULL;
    uint16_t wCount = 0;
    uint16_t wIndex = 0;
    MGC_MsdDevice *pMsdDevice;
    
    MUSB_ASSERT(pControllerPrivateData);
    MUSB_ASSERT(wTimerIndex == MUSB_MSD_CONF_TIMER_IDX);

    pPort = (MGC_Port *)pControllerPrivateData;
    MUSB_ASSERT(pPort->pController);

    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));

    // Check if any device is not handled by pProtocol->pfStartDevice().    
    for (wIndex=0; wIndex < wCount; wIndex++)
    {
        pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), wIndex);

        if ((pDevice) &&        
            (pDevice->bSupport == MUSB_STATUS_CHILD_UNKNOW) &&        
            (pDevice->bDeviceClass == MUSB_CLASS_MASS_STORAGE) &&
            (pDevice->pDriverPrivateData))
        {
            pMsdDevice = (MGC_MsdDevice *)pDevice->pDriverPrivateData;    

            if ((!pMsdDevice->pProtocol) ||(!pMsdDevice->pProtocol->pfStartDevice))
            {
                MUSB_DIAG_STRING(1, "MSD Error: Failed to start device");
                continue;
            }

            if (!pMsdDevice->pProtocol->pfStartDevice(pMsdDevice->pProtocol->pProtocolData,
                                                      pMsdDevice->pUsbDevice))
            {
                MUSB_DIAG_STRING(1, "MSD Error: Failed to start device");
            }
        }
    }
}

/** Callback function when device acknowledges set config reqeust. */
static void MGC_MsdSetConfigCallback(void *pContext, MUSB_ControlIrp *pControlIrp)
{
    MGC_MsdDevice *pMsdDevice;
    uint8_t bStatus = FALSE;
    MGC_Port *pPort;
    MUSB_SystemServices *pServices;

    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pControlIrp);

#ifdef MUSB_ISO_EMULATION
            return;
#endif            
    
    if (MUSB_STATUS_OK != pControlIrp->dwStatus)
    {
        LOG(1, "MSD Error: Set Config Callback Status = %d", pControlIrp->dwStatus);

        return;
    }

    pMsdDevice = (MGC_MsdDevice *)pContext;
    
    /* Set the Current Configuration Descriptor to Default as Set Config is Success */
    pMsdDevice->pUsbDevice->pCurrentConfiguration = pMsdDevice->pUsbDevice->apConfigDescriptors[0];

    /* Device is connected */
    LOG(5, "HubPort-%d: MSD Device = 0x%08X Config OK.\n", 
        pMsdDevice->pUsbDevice->bHubPort, (uint32_t)pMsdDevice->pUsbDevice);

    //  MTK Notice: Max Liao, 2007/11/01.
    //  AIGO MP5 player need some delay after set configuration.
    //  PC wait 70 ms delay before get max lun command.
    MUSB_ASSERT(pMsdDevice->hBus);
    pPort = (MGC_Port *)pMsdDevice->hBus;
    MUSB_ASSERT(pPort->pController);
    pServices = pPort->pController->pSystemServices;
    MUSB_ASSERT(pServices);    

    //Timer Index MUSB_MSD_CONF_TIMER_IDX is reserved for Mass storage class driver.
    /* kill timer */
    bStatus = pServices->pfCancelTimer(pServices->pPrivateData, 
        MUSB_MSD_CONF_TIMER_IDX/*bTimerIndex*/);
    UNUSED(bStatus);
    
    bStatus = pServices->pfArmTimer(pServices->pPrivateData, 
        MUSB_MSD_CONF_TIMER_IDX/*bTimerIndex*/, 
        MGC_MSD_CONFIG_TIME/*dwTime*/, FALSE, MGC_MsdTimerExpired, pPort);    

    MUSB_ASSERT(bStatus);
    UNUSED(pControlIrp);
}
