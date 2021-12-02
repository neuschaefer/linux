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
 * MUSBStack-S host-specific functionality.
 * $Revision: #1 $
 */

#include "mu_mem.h"
#include "mu_impl.h"

#include "mu_diag.h"
#include "mu_drcpr.h"

#ifdef MUSB_HUB
#include "mu_hub.h"
#include "mu_hdef.h"
#include "mu_hcreq.h"
#include "mu_hcore.h"
#include "mu_hapi.h"
#endif

/** 
 * Undefine this to avoid embedded HSET overheads 
 * (but this would preclude USB-IF high-speed OTG certification)
 */
//#define MUSB_EHSET

#if defined(MUSB_HOST) || defined(MUSB_OTG)

/*************************** CONSTANTS ****************************/

#define MGC_FRAME_OFFSET                (1)
#define MGC_MAX_POLLING_INTERVAL        (255)

#define MGC_DRC_DELAY         1000L /* just a guess! */

/** How many times to retry enumeration transfers */
//  MTK Notice: Max Liao, 2006/11/30.
//  modify enumeration retry times.
#define MGC_MAX_ENUM_RETRIES      3

#define MGC_MAX_STRING_LENGTH    30

/*
 * Enumeration state
 */

enum MGC_EnumerationState
{
    MGC_EnumStateIdle,
    MGC_EnumStateGetMaxDevice,
    MGC_EnumStateReset,
    MGC_EnumStateSetAddress,
    MGC_EnumStateGetFullDevice,
    MGC_EnumStateSizeConfigs,
    MGC_EnumStateGetConfigs,
    MGC_EnumStateConnectDevice,
    MGC_EnumStateSetFirstConfig
};

/***************************** TYPES ******************************/

/*
 * Bus timing info (times represent nanoseconds)
 */
typedef struct
{
    const uint32_t dwFrameTotal;      /* total frame time */
    const uint32_t dwControllerSetup; /* controller overhead */
    const uint32_t dwInOverhead;      /* non-ISO IN overhead */
    const uint32_t dwOutOverhead;     /* non-ISO OUT overhead */
    const uint32_t dwIsoInOverhead;   /* ISO IN overhead */
    const uint32_t dwIsoOutOverhead;  /* ISO OUT overhead */
    const uint32_t dwPayloadScale;    /* scaling factor for payload-dependent term, * 10 */
    const uint32_t dwMaxPeriodic;     /* maximum allowable periodic allocation */
} MGC_BusTimeInfo;
/*************************** FORWARDS *****************************/

static void MGC_HostSetDeviceAddress(void *pParam, uint16_t wTimer);
static void MGC_HostGetMaxDescriptor(void *pParam, uint16_t wTimer);
static void MGC_HostGetFullDescriptor(void *pParam, uint16_t wTimer);
static void MGC_HostGetConfig(void *pParam, uint16_t wTimer);
static uint16_t MGC_SkipEntry(const uint8_t *pOperand, uint16_t wInputIndex, uint16_t wLength);
MUSB_DeviceDriver *MGC_HostFindDriver(MUSB_HostClient *pClient, MUSB_Device *pDevice,
                                      const uint8_t **ppEntry);
static void MGC_HostEnumerator(void *pParam, MUSB_ControlIrp *pIrp);
uint8_t MGC_AllocateAddress(MGC_EnumerationData *pEnumData);
void MGC_ReleaseAddress(MGC_EnumerationData *pEnumData, uint8_t bAddress);
#ifdef MUSB_SCHEDULER
static void MGC_FreeScheduleContents(MGC_Schedule *pSchedule);
static uint32_t MGC_GetSlotTime(MGC_ScheduleSlot *pSlot);
static MGC_ScheduleSlot *MGC_FindScheduleSlot(MGC_Schedule *pSchedule,
                                              MGC_BusTimeInfo *pBusTimeInfo,
                                              uint16_t wInterval,
                                              uint16_t wDuration,
                                              uint32_t dwBusTime);
static uint32_t MGC_ComputeBandwidth(MGC_BusTimeInfo *pBusTimeInfo, uint8_t bTrafficType,
                                     uint8_t bIsIn, uint16_t wMaxPacketSize);
static uint8_t MGC_CommitBandwidth(MGC_Port *pPort, MGC_Pipe *pPipe,
                                   const MUSB_DeviceEndpoint *pRemoteEnd);
#endif

static void MGC_DriverTimerExpired(void *pControllerPrivateData, uint16_t wTimerIndex);

#ifdef MUSB_HUB
static uint8_t MUSB_SilentHubReset(MUSB_Device* pDevice);
#endif

/**************************** GLOBALS *****************************/
#ifdef MUSB_SCHEDULER
/** Timing of low-speed frames */

static MGC_BusTimeInfo MGC_LowSpeedFrame =
{
    1000000L, MGC_DRC_DELAY, 64060 + 2000, /* 2000 is 2*Hub_LS_Setup, which we are guessing! */
    64107 + 2000,                          /* 2000 is 2*Hub_LS_Setup, which we are guessing! */
    0, 0, 6767,
    (1000000L *90) / 100
};

/** Timing of full-speed frames */
static MGC_BusTimeInfo MGC_FullSpeedFrame =
{
    1000000L, MGC_DRC_DELAY, 9107, 9107, 7268, 6265, 836, (1000000L *90) / 100
};

/** Timing of high-speed frames */
static MGC_BusTimeInfo MGC_HighSpeedFrame =
{
    125000L, MGC_DRC_DELAY, 917, 917, 634, 634, 21, (125000L *80) / 100
};
#endif

/*
 * Enumerator is really optional since some OSs do this
 */
/** SET_ADDRESS request template */

static const uint8_t MGC_aSetDeviceAddress [] =
{
    (uint8_t)(MUSB_DIR_OUT | MUSB_TYPE_STANDARD | MUSB_RECIP_DEVICE), 
    (uint8_t)(MUSB_REQ_SET_ADDRESS), 0, 0, /* address */
    0, 0, 0, 0
};

/** GET_DESCRIPTOR(DEVICE) request template */
static const uint8_t MGC_aGetDeviceDescriptor [] =
{
    (uint8_t)(MUSB_DIR_IN | MUSB_TYPE_STANDARD | MUSB_RECIP_DEVICE), 
    (uint8_t)(MUSB_REQ_GET_DESCRIPTOR), 0,
    (uint8_t)(MUSB_DT_DEVICE), 0, 0,
    0, 0 /* allowed descriptor length */
};

/** GET_DESCRIPTOR(CONFIGURATION) request template */
static const uint8_t MGC_aGetConfigDescriptor [] =
{
    (uint8_t)(MUSB_DIR_IN | MUSB_TYPE_STANDARD | MUSB_RECIP_DEVICE), 
    (uint8_t)(MUSB_REQ_GET_DESCRIPTOR), 0,
    (uint8_t)(MUSB_DT_CONFIG), 0, 0,
    0, 0 /* allowed descriptor length */
};

/** SET_CONFIGURATION request template */
static const uint8_t MGC_aSetConfigDescriptor [] =
{
    (uint8_t)(MUSB_DIR_OUT | MUSB_TYPE_STANDARD | MUSB_RECIP_DEVICE), 
    (uint8_t)(MUSB_REQ_SET_CONFIGURATION), 1,
    0, 0, 0,
    0, 0 /* allowed descriptor length */
};

/** SET_FEATURE(DEVICE) request template */
#if 0

static uint8_t MGC_aSetDeviceFeature [] =
{
    MUSB_DIR_OUT | MUSB_TYPE_STANDARD | MUSB_RECIP_DEVICE, MUSB_REQ_SET_FEATURE, 0,
    0, /* feature selector */
    0, 0, 0,
    0
};
#endif

#ifdef MUSB_EHSET
uint32_t MGC_dwEL35Reset = FALSE;
static uint16_t MGC_wTestModePid;
static uint32_t MGC_bTestMode;
#endif

/*************************** FUNCTIONS ****************************/
/*
 * Handle enumerate fail case.
 */
static void MGC_HostEnumerateFail(MGC_EnumerationData *pEnumData, 
    MGC_Controller *pController)
{
    MGC_Port *pPort;
    MUSB_Device* pDevice;

    MUSB_ASSERT(pEnumData);    
    MUSB_ASSERT(pController);
    pPort = pController->pPort;
    MUSB_ASSERT(pPort);

    pDevice = pEnumData->pDevice;
    if (!pDevice)
    {
        return;
    }

    //2009.06.29, Sync from BD. Avoid memory leakage.
    if(pDevice)
    {
        /* release memory */
        if (pDevice->apConfigDescriptors)
        {
            MUSB_MemFree(pDevice->apConfigDescriptors);
            pDevice->apConfigDescriptors = NULL;
        }

        if (pDevice->pDescriptorBuffer)
        {
            MUSB_MemFree(pDevice->pDescriptorBuffer);
            pDevice->pDescriptorBuffer = NULL;
        }

        if (pDevice->pStrManufacturerBuffer)
        {
            MUSB_MemFree(pDevice->pStrManufacturerBuffer);
            pDevice->pStrManufacturerBuffer = NULL;
        }

        if (pDevice->pStrProductBuffer)
        {
            MUSB_MemFree(pDevice->pStrProductBuffer);
            pDevice->pStrProductBuffer = NULL;
        }

        if (pDevice->pStrSerialNumBuffer)
        {
            MUSB_MemFree(pDevice->pStrSerialNumBuffer);
            pDevice->pStrSerialNumBuffer = NULL;
        }
    }

#ifdef MUSB_HUB
    // check if this is a device on hub.
    if (pDevice->pParentUsbDevice)
    {
        // enumeration fail call parent enumerate call back function to check device status.
        if (pEnumData->pfEnumerationComplete)
        {
            pEnumData->pfEnumerationComplete(pDevice->pParentUsbDevice, 
                pDevice, FALSE);
            pEnumData->pfEnumerationComplete = NULL;                                                 
        }

        if (pEnumData->bAddress)
        {
            MGC_ReleaseAddress(pEnumData, pEnumData->bAddress);
            pEnumData->bAddress = 0;
        }

        pEnumData->pDevice = NULL;
        pEnumData->bState = (uint8_t)MGC_EnumStateIdle;
        pEnumData->bRetries = 0;        
    }
    else
#endif        
    {
        // Just to release device address and do not release its memory.
        // Release its memory when device disconnect.
        if (pEnumData->bAddress)
        {
            MGC_ReleaseAddress(pEnumData, pEnumData->bAddress);
            pEnumData->bAddress = 0;
        }

        // This is a root device. Just retry to reset device.
        if (++pEnumData->bRetries > MGC_MAX_ENUM_RETRIES)
        {
            pEnumData->bRetries = 0;
            // set to full speed only when retry above limit.
            pPort->bWantHighSpeed = FALSE;
        }

        // bus reset device.
        MGC_DrcBusReset(pPort);            
    }

    return;
}

/*
 * Issue a USB reset
 */
static void MGC_HoseResetDevice(void *pParam, uint16_t wTimer)
{
    MGC_Controller *pController;
    MGC_Port *pPort;
    MGC_EnumerationData *pEnumData;
    uint32_t u4Ret;
    MUSB_Device* pDevice;

    UNUSED(MGC_HoseResetDevice);
    UNUSED(u4Ret);
    pController = (MGC_Controller *)pParam;    
    pPort = pController->pPort;
    MUSB_ASSERT(pPort);
    pEnumData = &(pPort->EnumerationData);
    MUSB_ASSERT(pEnumData);
    pDevice = pEnumData->pDevice;
    MUSB_ASSERT(pDevice);
    UNUSED(pDevice);

    LOG(0, "HubPort-%d: Device reset.\n", pEnumData->pDevice->bHubPort);

#ifdef MUSB_HUB
    if (pDevice->pParentUsbDevice)
    {
        u4Ret = MUSB_SilentHubReset(pDevice);
        if (MUSB_STATUS_OK != u4Ret) 
        {    
            MGC_HostEnumerateFail(pEnumData, pController);
            return;
        }
    }
    else
#endif    
    {
        // Reset root device.
        LOG(0, "Silent Reset: Root device = 0x%08X, Reset.\n", (uint32_t)pDevice);

        pPort->bWantReset = TRUE;
        pPort->bWantSuspend = FALSE;
        pPort->pfProgramBusState(pPort);

        // wait reset.
        UNUSED(MUSB_Sleep(MUSB_BUS_RESET_TIME));

        // disable bus reset.
        pPort->bWantReset = FALSE;
        pPort->pfProgramBusState(pPort);
    }

    pEnumData->bState = (uint8_t)MGC_EnumStateReset;
    MGC_HostEnumerator(pParam, &pEnumData->Irp);
    UNUSED(wTimer);
}

/*
 * Issue a USB set_address from stack's IRP.
 */
static void MGC_HostSetDeviceAddress(void *pParam, uint16_t wTimer)
{
    MGC_Controller *pController;
    MGC_Port *pPort;
    MGC_EnumerationData *pEnumData;
    MUSB_DeviceRequest *pRequest;
    uint32_t u4Ret;
    
    MUSB_ASSERT(pParam);    
    pController = (MGC_Controller *)pParam;    
    pPort = pController->pPort;
    MUSB_ASSERT(pPort);
    pEnumData = &(pPort->EnumerationData);
    MUSB_ASSERT(pEnumData);
    pRequest = (MUSB_DeviceRequest *)pEnumData->aSetupTx;
    MUSB_ASSERT(pRequest);

    LOG(0, "HubPort-%d: Device address = %d.\n", 
        pEnumData->pDevice->bHubPort, pEnumData->bAddress);

    //  MTK Notice: Max Liao, 2006/11/27.
    //  Always set address via ep0 and address = 0.
    pPort->bFuncAddr = 0;
    pPort->pfProgramBusState(pPort);

    MUSB_MemCopy(pEnumData->aSetupTx, MGC_aSetDeviceAddress, sizeof(MGC_aSetDeviceAddress));
    pRequest->wValue = MUSB_SWAP16(pEnumData->bAddress);
    pEnumData->Irp.pOutBuffer = pEnumData->aSetupTx;
    pEnumData->Irp.dwOutLength = sizeof(MGC_aSetDeviceAddress);
//  MTK Notice: Max Liao, 2007/07/27.
//  handle no data phase control transfer.    
    pEnumData->Irp.pInBuffer = NULL;
    pEnumData->Irp.dwInLength = 0;   
    pEnumData->bState = (uint8_t)MGC_EnumStateSetAddress;
    u4Ret = MUSB_StartControlTransfer(pPort->pInterfacePort, &(pEnumData->Irp));
    if (MUSB_STATUS_OK != u4Ret) 
    {    
        MGC_HostEnumerateFail(pEnumData, pController);
    }
    
    UNUSED(wTimer);
}

/*
 * Get 1st 8 bytes of EP0 on the peripheral.  
 * Do this so we can throttle our reads if the 
 * peripheral only has an 8-byte EP0 Max EP Size.
 */
static void MGC_HostGetMaxDescriptor(void *pParam, uint16_t wTimer)
{
    MGC_Controller *pController;
    MGC_Port *pPort;
    MGC_EnumerationData *pEnumData;
    MUSB_DeviceRequest *pRequest;
    uint32_t u4Ret;

    MUSB_ASSERT(pParam);    
    pController = (MGC_Controller *)pParam;    
    pPort = pController->pPort;
    MUSB_ASSERT(pPort);
    pEnumData = &(pPort->EnumerationData);
    MUSB_ASSERT(pEnumData);
    pRequest = (MUSB_DeviceRequest *)pEnumData->aSetupTx;
    MUSB_ASSERT(pRequest);

    MGC_DIAG(2, pController, "Getting 8 bytes of device descriptor");

    MUSB_MemCopy(pEnumData->aSetupTx, MGC_aGetDeviceDescriptor, sizeof(MGC_aGetDeviceDescriptor));
    pRequest->wLength = MUSB_SWAP16(64);

    pEnumData->Irp.pOutBuffer = pEnumData->aSetupTx;
    pEnumData->Irp.dwOutLength = sizeof(MGC_aGetDeviceDescriptor);
    pEnumData->Irp.pInBuffer = pEnumData->aSetupRx;
    pEnumData->Irp.dwInLength = 64;
    pEnumData->bState = (uint8_t)MGC_EnumStateGetMaxDevice;
    u4Ret = MUSB_StartControlTransfer(pPort->pInterfacePort, &(pEnumData->Irp));
    if (MUSB_STATUS_OK != u4Ret) 
    {    
        MGC_HostEnumerateFail(pEnumData, pController);
    }

    UNUSED(wTimer);    
}

/*
 * Get full device descriptor on the peripheral.  
 */
static void MGC_HostGetFullDescriptor(void *pParam, uint16_t wTimer)
{
    MGC_Controller *pController;
    MGC_Port *pPort;
    MGC_EnumerationData *pEnumData;
    MUSB_DeviceRequest *pRequest;
    uint32_t u4Ret;

    MUSB_ASSERT(pParam);    
    pController = (MGC_Controller *)pParam;    
    pPort = pController->pPort;
    MUSB_ASSERT(pPort);
    pEnumData = &(pPort->EnumerationData);
    MUSB_ASSERT(pEnumData);
    pRequest = (MUSB_DeviceRequest *)pEnumData->aSetupTx;
    MUSB_ASSERT(pRequest);

    MGC_DIAG(2, pPort->pController, "Getting complete device descriptor");

    MUSB_MemCopy(pEnumData->aSetupTx, MGC_aGetDeviceDescriptor, sizeof(MGC_aGetDeviceDescriptor));
    pRequest->wLength = MUSB_SWAP16(sizeof(MUSB_DeviceDescriptor));
    pEnumData->Irp.pOutBuffer = pEnumData->aSetupTx;
    pEnumData->Irp.dwOutLength = sizeof(MGC_aGetDeviceDescriptor);
    pEnumData->Irp.pInBuffer = pEnumData->aSetupRx;
    pEnumData->Irp.dwInLength = sizeof(MUSB_DeviceDescriptor);
    pEnumData->bState = (uint8_t)MGC_EnumStateGetFullDevice;
    u4Ret = MUSB_StartControlTransfer(pPort->pInterfacePort, &(pEnumData->Irp));
    if (MUSB_STATUS_OK != u4Ret) 
    {    
        MGC_HostEnumerateFail(pEnumData, pController);
    }

    UNUSED(wTimer);    
}

/*
 * Get device string descriptor on the peripheral.  
 */
static void MGC_HostGetStringDescriptor(void *pParam, uint16_t wTimer)
{
    MGC_Controller *pController;
    MGC_Port *pPort;
    MGC_EnumerationData *pEnumData;
    MUSB_DeviceRequest *pRequest;
    uint32_t u4Ret;

    MUSB_ASSERT(pParam);    
    pController = (MGC_Controller *)pParam;    
    pPort = pController->pPort;
    MUSB_ASSERT(pPort);
    pEnumData = &(pPort->EnumerationData);
    MUSB_ASSERT(pEnumData);
    pRequest = (MUSB_DeviceRequest *)pEnumData->aSetupTx;
    MUSB_ASSERT(pRequest);

    MGC_DIAG(2, pPort->pController, "Getting complete string descriptor");

    pEnumData->Irp.pOutBuffer = pEnumData->aSetupTx;
    pEnumData->Irp.dwOutLength = sizeof(MGC_aGetDeviceDescriptor);
    pEnumData->Irp.pInBuffer = pEnumData->aSetupRx;
    pEnumData->Irp.dwInLength = (uint32_t)pRequest->wLength;
    pEnumData->bState = (uint8_t)MGC_EnumStateGetFullDevice;
    u4Ret = MUSB_StartControlTransfer(pPort->pInterfacePort, &(pEnumData->Irp));
    if (MUSB_STATUS_OK != u4Ret) 
    {    
        MGC_HostEnumerateFail(pEnumData, pController);
    }

    UNUSED(wTimer);    
}
/*
 * Get a config descriptor
 */
static void MGC_HostGetConfig(void *pParam, uint16_t wTimer)
{
    MGC_Controller *pController;
    MGC_Port *pPort;
    MGC_EnumerationData *pEnumData;
    MUSB_DeviceRequest *pRequest;
    uint32_t u4Ret;

    MUSB_ASSERT(pParam);    
    pController = (MGC_Controller *)pParam;    
    pPort = pController->pPort;
    MUSB_ASSERT(pPort);
    pEnumData = &(pPort->EnumerationData);
    MUSB_ASSERT(pEnumData);
    pRequest = (MUSB_DeviceRequest *)pEnumData->aSetupTx;
    MUSB_ASSERT(pRequest);

    MGC_DIAG1(2, pPort->pController, "Getting config descriptor, length=",
              pEnumData->Irp.dwInLength, 16, 4);

    MUSB_MemCopy(pEnumData->aSetupTx, MGC_aGetConfigDescriptor, sizeof(MGC_aGetConfigDescriptor));
    pEnumData->aSetupTx[2] = pEnumData->bIndex;
    pRequest->wLength = MUSB_SWAP16((uint16_t)pEnumData->Irp.dwInLength);
    pEnumData->Irp.pOutBuffer = pEnumData->aSetupTx;
    pEnumData->Irp.dwOutLength = sizeof(MGC_aGetConfigDescriptor);
    u4Ret = MUSB_StartControlTransfer(pPort->pInterfacePort, &(pEnumData->Irp));
    if (MUSB_STATUS_OK != u4Ret) 
    {    
        MGC_HostEnumerateFail(pEnumData, pController);
    }

    UNUSED(wTimer);    
}

/*
 * Connect device
 */
static void MGC_HostConnectDevice(void *pParam, uint16_t wTimer)
{
    MGC_Controller *pController;
    MGC_Port *pPort;
    MGC_EnumerationData *pEnumData;

    MUSB_ASSERT(pParam);    
    pController = (MGC_Controller *)pParam;    
    pPort = pController->pPort;
    MUSB_ASSERT(pPort);
    pEnumData = &(pPort->EnumerationData);
    MUSB_ASSERT(pEnumData);

    MGC_DIAG1(2, pPort->pController, "Connect device =",
              pEnumData->Irp.dwInLength, 16, 4);

    MGC_HostEnumerator(pParam, &pEnumData->Irp);

    UNUSED(wTimer);    
}

#ifdef MUSB_EHSET
static void MGC_EhsetResume(void *pParam, uint16_t wTimer)
{
    MGC_Controller *pController;
    MGC_Port *pPort;

    MUSB_ASSERT(pParam);    
    pController = (MGC_Controller *)pParam;    
    pPort = pController->pPort;
    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pPort->pfSetPortTestMode);

    LOG(0, "HS_HOST_RESUME.\n", NULL);    
    
    pPort->pfSetPortTestMode(pPort, MUSB_HSET_PORT_RESUME);

    UNUSED(wTimer);    
}

static void MGC_EhsetSuspend(void *pParam, uint16_t wTimer)
{
    MGC_Controller *pController;
    MGC_Port *pPort;
    uint8_t bRet = FALSE;

    MUSB_ASSERT(pParam);    
    pController = (MGC_Controller *)pParam;    
    pPort = pController->pPort;
    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pPort->pfSetPortTestMode);

    LOG(0, "HS_HOST_SUSPEND.\n", NULL);    
    LOG(0, "Wait 15 secs to resume...\n", NULL);    

    pPort->pfSetPortTestMode(pPort, MUSB_HSET_PORT_SUSPEND);

    bRet = pController->pSystemServices->pfArmTimer(pController->pSystemServices->pPrivateData, 0,
                                             15000, FALSE,
                                             MGC_EhsetResume, pController);

    UNUSED(wTimer);    
    UNUSED(bRet);    
}

static void MGC_EhsetGetDescriptorSetup(void *pParam, uint16_t wTimer)
{
    MGC_Controller *pController;
    MGC_Port *pPort;
    MGC_EnumerationData *pEnumData;
    MUSB_DeviceRequest *pRequest;

    MUSB_ASSERT(pParam);    
    pController = (MGC_Controller *)pParam;    
    pPort = pController->pPort;
    MUSB_ASSERT(pPort);
    pEnumData = &(pPort->EnumerationData);
    MUSB_ASSERT(pEnumData);
    pRequest = (MUSB_DeviceRequest *)pEnumData->aSetupTx;
    MUSB_ASSERT(pRequest);

    LOG(0, "Getting complete device descriptor.\n", NULL);       

    MUSB_MemCopy(pEnumData->aSetupTx, MGC_aGetDeviceDescriptor, sizeof(MGC_aGetDeviceDescriptor));
    pRequest->wLength = MUSB_SWAP16(sizeof(MUSB_DeviceDescriptor));

    pPort->pfLoadFifo(pPort, 0, sizeof(pEnumData->aSetupTx), pEnumData->aSetupTx);
    pPort->pfSetPortTestMode(pPort, MUSB_HSET_PORT_SETUP_START);

    UNUSED(wTimer);    
}

static void MGC_EhsetGetDescriptorIn(void *pParam, uint16_t wTimer)
{
    MGC_Controller *pController;
    MGC_Port *pPort;

    MUSB_ASSERT(pParam);    
    pController = (MGC_Controller *)pParam;    
    pPort = pController->pPort;
    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pPort->pfSetPortTestMode);

    LOG(0, "Host sends an IN packet.\n", NULL);           

    pPort->pfSetPortTestMode(pPort, MUSB_HSET_PORT_SETUP_IN);

    UNUSED(wTimer);        
}

static void MGC_Ehset(MGC_Port *pPort, uint16_t wPid)
{
    MGC_Controller *pController;
    uint8_t bRet = FALSE;

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pPort->pfSetPortTestMode);
    pController = pPort->pController;
    MUSB_ASSERT(pController->pSystemServices);
    MUSB_ASSERT(pController->pSystemServices->pfArmTimer);

    LOG(0, "MGC_Ehset: Vid = 0x1A0A, Pid = 0x%04X.\n", wPid);

    switch (wPid)
    {
    case 0x0101:
        LOG(0, "TEST_SE0_NAK.\n", NULL);    

        /* SE0_NAK */
        pPort->pfSetPortTestMode(pPort, MUSB_HSET_PORT_TEST_SE0_NAK);
        break;

    case 0x0102:
        LOG(0, "TEST_J.\n", NULL);    

        /* J */
        pPort->pfSetPortTestMode(pPort, MUSB_HSET_PORT_TEST_J);
        break;

    case 0x0103:
        LOG(0, "TEST_K.\n", NULL);    
        
        /* K */
        pPort->pfSetPortTestMode(pPort, MUSB_HSET_PORT_TEST_K);
        break;

    case 0x0104:
        LOG(0, "TEST_PACKET.\n", NULL);    
    
        /* PACKET */
        pPort->pfSetPortTestMode(pPort, MUSB_HSET_PORT_TEST_PACKET);
        break;

    case 0x0105:
        LOG(0, "RESERVED.\n", NULL);    
    
        /* FORCE_ENABLE */
        break;

    case 0x0106:
        LOG(0, "HS_HOST_PORT_SUSPEND_RESUME.\n", NULL);    
        LOG(0, "Sending SOFs for 15 secs...\n", NULL);    
        bRet = pController->pSystemServices->pfArmTimer(pController->pSystemServices->pPrivateData, 0,
                                                 15000, FALSE,
                                                 MGC_EhsetSuspend, pController);
        break;

    case 0x0107:
        LOG(0, "SINGLE_STEP_GET_DEVICE_DESCRIPTOR.\n", NULL);    
        LOG(0, "Sending SOFs for 15 secs...\n", NULL);       
        /* wait 15 secs, perform setup phase of GET_DESC */
        bRet = pController->pSystemServices->pfArmTimer(pController->pSystemServices->pPrivateData, 0,
                                                 15000, FALSE,
                                                 MGC_EhsetGetDescriptorSetup, pController);
        break;

    case 0x0108:
        LOG(0, "SINGLE_STEP_SET_FEATURE.\n", NULL);    
        LOG(0, "GetDescriptorSetup.\n", NULL);           
        /* perform setup phase of GET_DESC, wait 15 secs, perform IN data phase */
        MGC_EhsetGetDescriptorSetup(pController, 0);
        LOG(0, "Sending SOFs for 15 secs...\n", NULL);             
        bRet = pController->pSystemServices->pfArmTimer(pController->pSystemServices->pPrivateData, 0,
                                                 15000, FALSE,
                                                 MGC_EhsetGetDescriptorIn, pController);
        break;

    default:
        break;
    }
    UNUSED(bRet);
}
#endif

/**
 * Skip an entry in the peripheral list, by searching for the next ACCEPT or REJECT
 * @param pOperand list
 * @param wInputIndex current index
 * @return the index in the list for the ACCEPT or REJECT, or one beyond the list
 * if the list is missing this (malformed)
 */

static uint16_t MGC_SkipEntry(const uint8_t *pOperand, uint16_t wInputIndex, uint16_t wLength)
{
    uint16_t wIndex = wInputIndex;

    MUSB_ASSERT(pOperand);

    while (wIndex < wLength)
    {
        switch (pOperand[wIndex])
        {
        case MUSB_TARGET_UNKNOWN:
        case MUSB_TARGET_CONFIG:
        case MUSB_TARGET_INTERFACE:
        case MUSB_TARGET_CLASS:
        case MUSB_TARGET_SUBCLASS:
        case MUSB_TARGET_PROTOCOL:
            wIndex++;
            break;

        case MUSB_TARGET_VID:
        case MUSB_TARGET_PID:
        case MUSB_TARGET_DEVICE_BCD:
            wIndex += 3;
            break;

        case MUSB_TARGET_ACCEPT:
        case MUSB_TARGET_REJECT:
            return wIndex;
            
        default:
            return 0xffff;          
        }
    }

    return wIndex;
}

/*
 * Find a driver
 */
MUSB_DeviceDriver *MGC_HostFindDriver(MUSB_HostClient *pClient, MUSB_Device *pDevice,
                                      const uint8_t **ppEntry)
{
    uint8_t bData;

    uint16_t wData;
    uint8_t bInterface = FALSE;
    uint8_t bMatch = TRUE;
    uint16_t wIndex = 0;
    uint16_t wEntryIndex = 0;
    MUSB_ConfigurationDescriptor *pConfig;
    const MUSB_InterfaceDescriptor *pInterface = NULL;
    uint16_t wConfigLength;
    uint16_t wLength;
    const uint8_t *pOperand;

    MUSB_ASSERT(pClient);
    MUSB_ASSERT(pDevice);

    pConfig = pDevice->apConfigDescriptors[0];
    wConfigLength = MUSB_SWAP16P((uint8_t *)&(pConfig->wTotalLength));
    wLength = pClient->wPeripheralListLength;
    pOperand = pClient->pPeripheralList;
    
    while (wIndex < wLength)
    {
        switch (pOperand[wIndex])
        {
        case MUSB_TARGET_UNKNOWN:
            wIndex++;
            break;

        case MUSB_TARGET_VID:
            wData = pOperand[++wIndex];
            wData |= (pOperand[++wIndex] << 8);
            if (wData != MUSB_SWAP16P((uint8_t *)&(pDevice->DeviceDescriptor.idVendor)))
            {
                bMatch = FALSE;
            }
            wIndex++;
            break;

        case MUSB_TARGET_PID:
            wData = pOperand[++wIndex];
            wData |= (pOperand[++wIndex] << 8);
            if (wData != MUSB_SWAP16P((uint8_t *)&(pDevice->DeviceDescriptor.idProduct)))
            {
                bMatch = FALSE;
            }
            wIndex++;
            break;

        case MUSB_TARGET_DEVICE_BCD:
            wData = pOperand[++wIndex];
            wData |= (pOperand[++wIndex] << 8);
            if (wData != MUSB_SWAP16P((uint8_t *)&(pDevice->DeviceDescriptor.bcdDevice)))
            {
                bMatch = FALSE;
            }
            wIndex++;
            break;

        case MUSB_TARGET_CONFIG:
            bData = pOperand[++wIndex];
            bInterface = FALSE;
            if (bData < pDevice->DeviceDescriptor.bNumConfigurations)
            {
                pConfig = pDevice->apConfigDescriptors[bData];
                wConfigLength = MUSB_SWAP16P((uint8_t *)&(pConfig->wTotalLength));
                wIndex++;
            }
            else
            {
                bMatch = FALSE;
                wIndex = MGC_SkipEntry(pOperand, wIndex + 1, wLength);
            }
            break;

        case MUSB_TARGET_INTERFACE:
            bData = pOperand[++wIndex];
            bInterface = TRUE;
            pInterface = (MUSB_InterfaceDescriptor *)((void *)MGC_FindDescriptor(
                (uint8_t *)pConfig, wConfigLength, MUSB_DT_INTERFACE, bData));
            if (pInterface)
            {
                wIndex++;
            }
            else
            {
                bMatch = FALSE;
                wIndex = MGC_SkipEntry(pOperand, wIndex + 1, wLength);
            }
            break;

        case MUSB_TARGET_CLASS:
            bData = pOperand[++wIndex];
            if (bInterface)
            {
                MUSB_ASSERT(pInterface);
                if (!pInterface)
                {
                    return NULL;
                }
                bMatch &= (uint8_t)(bData == pInterface->bInterfaceClass);

                // If device class code of device descriptor = 0, 
                // each interface within a configuration specifies its own class information.
                if (pDevice->DeviceDescriptor.bDeviceClass == 0)
                {
                    pDevice->bDeviceClass = pInterface->bInterfaceClass;    
                }
            }
            else
            {
                bMatch &= (uint8_t)(bData == pDevice->DeviceDescriptor.bDeviceClass);
                // If device class code of device descriptor = 0, 
                // each interface within a configuration specifies its own class information.
                if (pDevice->DeviceDescriptor.bDeviceClass > 0)
                {
                    pDevice->bDeviceClass = pDevice->DeviceDescriptor.bDeviceClass;    
                }
            }
            wIndex++;
            break;

        case MUSB_TARGET_SUBCLASS:
            bData = pOperand[++wIndex];
            if (bInterface)
            {
                MUSB_ASSERT(pInterface);
                if (!pInterface)
                {
                    return NULL;
                }
                
                bMatch &= (uint8_t)(bData == pInterface->bInterfaceSubClass);
            }
            else
            {
                bMatch &= (uint8_t)(bData == pDevice->DeviceDescriptor.bDeviceSubClass);
            }
            wIndex++;
            break;

        case MUSB_TARGET_PROTOCOL:
            bData = pOperand[++wIndex];
            if (bInterface)
            {
                MUSB_ASSERT(pInterface);
                if (!pInterface)
                {
                    return NULL;
                }
                
                bMatch &= (uint8_t)(bData == pInterface->bInterfaceProtocol);
            }
            else
            {
                bMatch &= (uint8_t)(bData == pDevice->DeviceDescriptor.bDeviceProtocol);
            }
            wIndex++;
            break;

        case MUSB_TARGET_ACCEPT:
            /* if matched or the match-all degenerate case, return driver */
            if (bMatch || (0 == wIndex))
            {
                bData = pOperand[++wIndex];

                if (bData < pClient->bDeviceDriverListLength)
                {
                    MUSB_ASSERT(ppEntry);
                    *ppEntry = &(pOperand[wEntryIndex]);
                    return &(pClient->aDeviceDriverList[bData]);
                }
                return NULL;
            }

            // Check next class driver condition.
            bInterface = FALSE;
            bMatch = TRUE;
            wIndex += 2;
            wEntryIndex = wIndex;
            break;

        case MUSB_TARGET_REJECT:
            if (bMatch)
            {
                return NULL;
            }
            // Check next class driver condition.
            bInterface = FALSE;
            bMatch = TRUE;           
            wIndex++;
            wEntryIndex = wIndex;
            break;

        default:
            break;            
        }
    }

    return NULL;
}

static uint8_t MGC_ForceFullSpeed(MGC_Controller *pController)
{
    MGC_Port *pPort;
    MGC_EnumerationData *pEnumData;

    MUSB_ASSERT(pController);    
    pPort = pController->pPort;
    MUSB_ASSERT(pPort);
    pEnumData = &(pPort->EnumerationData);
    MUSB_ASSERT(pEnumData);

    if (pEnumData->pDevice->pParentUsbDevice != NULL)
    {
        return FALSE;
    }

    if (pEnumData->pDevice->ConnectionSpeed != MUSB_CONNECTION_SPEED_HIGH)
    {
        return FALSE;
    }
    // Check if device in our black list.
    if (!((pEnumData->pDevice->DeviceDescriptor.idVendor == 0x0AEC) &&
            (pEnumData->pDevice->DeviceDescriptor.idProduct == 0x3260) &&
            (pEnumData->pDevice->DeviceDescriptor.bcdDevice == 0x0100)))
    {
        return FALSE;
    }

    LOG(0, "Force to enter full speed mode.\n", NULL);

    if (pEnumData->bAddress)
    {
        MGC_ReleaseAddress(pEnumData, pEnumData->bAddress);
        pEnumData->bAddress = 0;
    }

    // Force to set full speed mode.
    pPort->bWantHighSpeed = FALSE;

    // Bus reset device.
    MGC_DrcBusReset(pPort);      

    return TRUE;
}

/*
 * Callback used during enumeration process
 */
static void MGC_HostEnumerator(void *pParam, MUSB_ControlIrp *pIrp)
{
    MGC_Device *pDevice;

    MUSB_DeviceDriver *pDriver;
    MGC_EndpointResource *pEnd;
    MUSB_DeviceDescriptor *pDeviceDesc = NULL;
    MUSB_ConfigurationDescriptor *pConfigDesc;
    uint8_t *pDescriptorBuffer;
    uint8_t *pEntry;
    uint16_t wValue = 0;
    uint16_t wIndex = 0;
    uint16_t  wLength = 0;
    MGC_Controller *pController;
    MGC_Port *pPort;
    MGC_EnumerationData *pEnumData;
    MUSB_SystemServices *pServices;
    uint8_t bRet = FALSE;
    MUSB_DeviceRequest *pRequest;
    uint32_t i;    

    MUSB_ASSERT(pParam);    
    pController = (MGC_Controller *)pParam;    
    pPort = pController->pPort;
    MUSB_ASSERT(pPort);
    pEnumData = &(pPort->EnumerationData);
    MUSB_ASSERT(pEnumData);
    pServices = pController->pSystemServices;
    MUSB_ASSERT(pServices);

    /* NOTE: assumes EP0 is first */
    pEnd = MUSB_ArrayFetch(&(pPort->LocalEnds), 0);

    if (!pEnd)
    {
        MGC_DIAG(1, pController, "Internal error during enumeration");
        return;
    }

//  MTK Notice: Max Liao, 2007/07/30.
//  check error only when device enumeration not in get device string state.
//  Fix GENERIC USB Storage-SDC I19B, it do not return any data in pDeviceDesc->iProduct string.
    if ((pEnumData->Irp.dwStatus) && ((uint8_t)MGC_EnumStateGetFullDevice != pEnumData->bState))
    {
        MGC_HostEnumerateFail(pEnumData, pController);
        return;        
    }

    /* Decide what to do next based on what we just did */
    switch (pEnumData->bState)
    {
    case MGC_EnumStateGetMaxDevice:
        /* set new max packet size */
        pDeviceDesc = (MUSB_DeviceDescriptor *)pEnumData->aSetupRx;
        pEnd->wPacketSize = pDeviceDesc->bMaxPacketSize0;

        bRet = pServices->pfArmTimer(pServices->pPrivateData, 
            0, 10, FALSE, MGC_HoseResetDevice, pController);
        break;

    case MGC_EnumStateReset:
        bRet = pServices->pfArmTimer(pServices->pPrivateData, 
            0, 10, FALSE, MGC_HostSetDeviceAddress, pController);    
        break;

    case MGC_EnumStateSetAddress:
        pEnumData->pDevice->bBusAddress = pEnumData->bAddress;

        /* get full descriptor, now that we know the peripheral's packet size */
        bRet = pServices->pfArmTimer(pServices->pPrivateData, 
            0, 10, FALSE, MGC_HostGetFullDescriptor, pController);
        break;

    case MGC_EnumStateGetFullDevice:    
        if (!pEnumData->pDevice->pLangId)
        {
            pDeviceDesc = (MUSB_DeviceDescriptor *)pEnumData->aSetupRx;
            MUSB_MemCopy((void *)&(pEnumData->pDevice->DeviceDescriptor), (void *)pDeviceDesc,
                         sizeof(MUSB_DeviceDescriptor));                         
            LOG(0, "HubPort-%d: idVendor = 0x%04X.\n", 
                pEnumData->pDevice->bHubPort, pDeviceDesc->idVendor);
            LOG(0, "HubPort-%d: idProduct = 0x%04X.\n", 
                pEnumData->pDevice->bHubPort, pDeviceDesc->idProduct);
            LOG(0, "HubPort-%d: bcdDevice = 0x%04X.\n", 
                pEnumData->pDevice->bHubPort, pDeviceDesc->bcdDevice);

            if (MGC_ForceFullSpeed(pController))
            {
                return;
            }
            
#ifdef MUSB_EHSET        
            wIndex = MUSB_SWAP16P((uint8_t *)&(pDeviceDesc->idVendor));
            wValue = MUSB_SWAP16P((uint8_t *)&(pDeviceDesc->idProduct));
            if ((6666 == wIndex) && ((wValue >= 0x0101) && (wValue <= 0x0108)))
            {
                MGC_Ehset(pPort, wValue);
                return;
            }

            //  MTK Notice: Max Liao, 2006/10/26.
            //  add virtual Pid for device to test embedded host compilance test.	
            if (MGC_bTestMode && ((MGC_wTestModePid >= 0x0101) && (MGC_wTestModePid <= 0x0108)))
            {
                MGC_Ehset(pPort, MGC_wTestModePid);
                return;
            }
#endif
            if ((pDeviceDesc->iManufacturer) ||(pDeviceDesc->iProduct) ||(pDeviceDesc->iSerialNumber))
            {
                //get Lang Id.
                pEnumData->pDevice->wLangId = 0;
                pEnumData->pDevice->pLangId =
                    (uint8_t *)&pEnumData->pDevice->wLangId;        
                MUSB_MemCopy(pEnumData->aSetupTx, MGC_aGetDeviceDescriptor, sizeof(MGC_aGetDeviceDescriptor));
                pRequest = (MUSB_DeviceRequest *)pEnumData->aSetupTx;            
                pRequest->wValue = (uint16_t)((MUSB_DT_STRING) << 8);
                pRequest->wLength = MUSB_SWAP16(sizeof(pEnumData->aSetupRx));   
                // query langId
                pRequest->wIndex = 0;
                bRet = pServices->pfArmTimer(pServices->pPrivateData, 0, 10, 
                    FALSE, MGC_HostGetStringDescriptor, pController);
                break;
            }
        }
        else
        {
            pDeviceDesc = &(pEnumData->pDevice->DeviceDescriptor);
        }
        
        // copy LangId to buffer.
        if ((pDeviceDesc->iManufacturer) ||(pDeviceDesc->iProduct) ||(pDeviceDesc->iSerialNumber))
        {
            if (!pEnumData->pDevice->bGotLangId)
            {
                pEnumData->pDevice->bGotLangId = TRUE;
                MUSB_MemCopy(pEnumData->pDevice->pLangId, &pEnumData->aSetupRx[2], 2);
                LOG(0, "HubPort-%d: Lang Id: 0x%X.\n", 
                    pEnumData->pDevice->bHubPort, pEnumData->pDevice->wLangId);            
            }
        }

        //get manufacturer string.
        if ((pDeviceDesc->iManufacturer!= 0) && (!pEnumData->pDevice->pStrManufacturerBuffer))
        {
            pEnumData->pDevice->pStrManufacturerBuffer =
                (uint8_t *)MUSB_MemAlloc(MGC_MAX_STRING_LENGTH);        
            MUSB_ASSERT(pEnumData->pDevice->pStrManufacturerBuffer);    
            MUSB_MemSet(pEnumData->pDevice->pStrManufacturerBuffer, 0, MGC_MAX_STRING_LENGTH);
            MUSB_MemCopy(pEnumData->aSetupTx, MGC_aGetDeviceDescriptor, sizeof(MGC_aGetDeviceDescriptor));
            pRequest = (MUSB_DeviceRequest *)pEnumData->aSetupTx;            
            pRequest->wValue = (uint16_t)(((MUSB_DT_STRING) << 8) | pDeviceDesc->iManufacturer);
            pRequest->wLength = MUSB_SWAP16(sizeof(pEnumData->aSetupRx));   
            // only query unicode langId
            pRequest->wIndex = pEnumData->pDevice->wLangId;
            bRet = pServices->pfArmTimer(pServices->pPrivateData, 0, 10, 
                FALSE, MGC_HostGetStringDescriptor, pController);
            break;                
        }

        // copy to buffer.
        if ((pDeviceDesc->iManufacturer!= 0) && (!pEnumData->pDevice->bGotStrManufacturer))
        {
            pEnumData->pDevice->bGotStrManufacturer = TRUE;
            wLength = MUSB_MIN(((uint8_t)(pEnumData->aSetupRx[0]-2) >> 1), (MGC_MAX_STRING_LENGTH-1));
            for (i=0; i<wLength;i++)
            {
                pEnumData->pDevice->pStrManufacturerBuffer[i] = pEnumData->aSetupRx[2+(2*i)];
            }
            LOG(0, "HubPort-%d: Manufacturer: %s.\n", 
                pEnumData->pDevice->bHubPort, pEnumData->pDevice->pStrManufacturerBuffer);            
        }

        //get product string.
        if ((pDeviceDesc->iProduct!= 0) && (!pEnumData->pDevice->pStrProductBuffer))
        {
            pEnumData->pDevice->pStrProductBuffer =
                (uint8_t *)MUSB_MemAlloc(MGC_MAX_STRING_LENGTH);        
            MUSB_ASSERT(pEnumData->pDevice->pStrProductBuffer);    
            MUSB_MemSet(pEnumData->pDevice->pStrProductBuffer, 0, MGC_MAX_STRING_LENGTH);

            MUSB_MemCopy(pEnumData->aSetupTx, MGC_aGetDeviceDescriptor, sizeof(MGC_aGetDeviceDescriptor));
            pRequest = (MUSB_DeviceRequest *)pEnumData->aSetupTx;            
            pRequest->wValue = (uint16_t)(((MUSB_DT_STRING) << 8) | pDeviceDesc->iProduct);
            pRequest->wLength = MUSB_SWAP16(sizeof(pEnumData->aSetupRx));   
            // only query unicode langId
            pRequest->wIndex = pEnumData->pDevice->wLangId;
            bRet = pServices->pfArmTimer(pServices->pPrivateData, 0, 10, 
                FALSE, MGC_HostGetStringDescriptor, pController);
            break;                
        }

        // copy to buffer.
        if ((pDeviceDesc->iProduct!= 0) && (!pEnumData->pDevice->bGotStrProduct))
        {
            pEnumData->pDevice->bGotStrProduct = TRUE;
            wLength = MUSB_MIN(((uint8_t)(pEnumData->aSetupRx[0]-2) >> 1), (MGC_MAX_STRING_LENGTH-1));
            for (i=0; i<wLength;i++)
            {
                pEnumData->pDevice->pStrProductBuffer[i] = pEnumData->aSetupRx[2+(2*i)];
            }
            LOG(0, "HubPort-%d: Product: %s.\n", 
                pEnumData->pDevice->bHubPort, pEnumData->pDevice->pStrProductBuffer);
        }

        //get serial string.
        if ((pDeviceDesc->iSerialNumber != 0) && (!pEnumData->pDevice->pStrSerialNumBuffer))
        {
            pEnumData->pDevice->pStrSerialNumBuffer =
                (uint8_t *)MUSB_MemAlloc(MGC_MAX_STRING_LENGTH);        
            MUSB_ASSERT(pEnumData->pDevice->pStrSerialNumBuffer);    
            MUSB_MemSet(pEnumData->pDevice->pStrSerialNumBuffer, 0, MGC_MAX_STRING_LENGTH);

            MUSB_MemCopy(pEnumData->aSetupTx, MGC_aGetDeviceDescriptor, sizeof(MGC_aGetDeviceDescriptor));
            pRequest = (MUSB_DeviceRequest *)pEnumData->aSetupTx;            
            pRequest->wValue = (uint16_t)(((MUSB_DT_STRING) << 8) | pDeviceDesc->iSerialNumber);            
            pRequest->wLength = MUSB_SWAP16(sizeof(pEnumData->aSetupRx));   
            // only query unicode langId
            pRequest->wIndex = pEnumData->pDevice->wLangId;
            bRet = pServices->pfArmTimer(pServices->pPrivateData, 0, 10, 
                FALSE, MGC_HostGetStringDescriptor, pController);
            break;                
        }

        // copy to buffer.
        if ((pDeviceDesc->iSerialNumber!= 0) && (!pEnumData->pDevice->bGotStrSerialNum))
        {
            pEnumData->pDevice->bGotStrSerialNum = TRUE;
            wLength = MUSB_MIN(((uint8_t)(pEnumData->aSetupRx[0]-2) >> 1), (MGC_MAX_STRING_LENGTH-1));
            for (i=0; i<wLength;i++)
            {
                pEnumData->pDevice->pStrSerialNumBuffer[i] = pEnumData->aSetupRx[2+(2*i)];
            }
            LOG(0, "HubPort-%d: SerialNum: %s.\n", 
                pEnumData->pDevice->bHubPort, pEnumData->pDevice->pStrSerialNumBuffer);                        
        }

        // Get configuration.
        pDeviceDesc = &(pEnumData->pDevice->DeviceDescriptor);
        
        MGC_DIAG1(2, pController, "Got device descriptor; config count=",
                  pDeviceDesc->bNumConfigurations, 16, 2);

        pEnumData->bCount = pDeviceDesc->bNumConfigurations;
        pEnumData->bIndex = 0;
        pEnumData->dwData = 0L;
       
        /* allocate convenience pointers (we temporarily use them to remember sizes) */        
        pEnumData->pDevice->apConfigDescriptors =
            (MUSB_ConfigurationDescriptor **)MUSB_MemAlloc(
                pDeviceDesc->bNumConfigurations *sizeof(uint8_t *));        
        if (pEnumData->pDevice->apConfigDescriptors)
        {
            /* get min config descriptors to gather full sizes */
            pEnumData->bState = (uint8_t)MGC_EnumStateSizeConfigs;

            pEnumData->Irp.pInBuffer = pEnumData->aSetupRx;
            pEnumData->Irp.dwInLength = sizeof(MUSB_ConfigurationDescriptor);
            bRet = pServices->pfArmTimer(pServices->pPrivateData, 0, 10, 
                FALSE, MGC_HostGetConfig, pController);
            break;
        }
        MUSB_MemFree(pEnumData->pDevice->pPrivateData);
        pEnumData->pDevice = NULL;
        pEnumData->bState = (uint8_t)MGC_EnumStateIdle;
        MGC_DIAG(1, pController, "Insufficient memory for new device");
        break;

    case MGC_EnumStateSizeConfigs:
        /* collect size */
        pConfigDesc = (MUSB_ConfigurationDescriptor *)((void *)pEnumData->Irp.pInBuffer);
        wLength = MUSB_SWAP16P((uint8_t *)&(pConfigDesc->wTotalLength));
        pEnumData->dwData += wLength;

        MGC_DIAG2(2, pController, "Config ", pEnumData->bIndex, " length=", wLength, 16, 0);

        pEnumData->pDevice->apConfigDescriptors[pEnumData->bIndex] =
            (MUSB_ConfigurationDescriptor *)(uint32_t)wLength;
        /* start next one if needed */
        if (++pEnumData->bIndex<pEnumData->bCount)
        {
            bRet = pServices->pfArmTimer(pServices->pPrivateData, 0, 10, 
                FALSE, MGC_HostGetConfig, pController);
        }
        else
        {
            /* size collected; allocate buffer for all configs */
            MGC_DIAG1(2, pController, "Total config length=", pEnumData->dwData, 16, 0);

            pEnumData->bIndex = 0;
            pEnumData->bState = (uint8_t)MGC_EnumStateGetConfigs;
            wLength = *((uint16_t *)((uint32_t)&(pEnumData->pDevice->apConfigDescriptors[pEnumData->bIndex])));

            pDescriptorBuffer = (uint8_t *)MUSB_MemAlloc(pEnumData->dwData);

            if (!pDescriptorBuffer)
            {
                MGC_DIAG(1, pController, "Insufficient memory for new device");

                MUSB_MemFree(pEnumData->pDevice->pPrivateData);
                break;
            }
            /* start gathering full config info */
            pEnumData->pDevice->pDescriptorBuffer = pDescriptorBuffer;
            pEnumData->pDevice->wDescriptorBufferLength = (uint16_t)pEnumData->dwData;
            pEnumData->pDevice->apConfigDescriptors[0] =
                (MUSB_ConfigurationDescriptor *)((void *)pDescriptorBuffer);
            pEnumData->Irp.pInBuffer = pEnumData->pDevice->pDescriptorBuffer;
            pEnumData->Irp.dwInLength = wLength;
            bRet = pServices->pfArmTimer(pServices->pPrivateData, 0, 10, 
                FALSE, MGC_HostGetConfig, pController);
        }
        break;

    case MGC_EnumStateGetConfigs:
        pEnumData->bState = (uint8_t)MGC_EnumStateConnectDevice;        
        bRet = pServices->pfArmTimer(pServices->pPrivateData, 0, 10, 
            FALSE, MGC_HostConnectDevice, pController);        
        break;
        
    case MGC_EnumStateConnectDevice:
        /* replace saved lengths with actual pointers as we go */
        if (pEnumData->bIndex)
        {
            pEnumData->pDevice->apConfigDescriptors[pEnumData->bIndex] =
                pEnumData->pDevice->apConfigDescriptors[pEnumData->bIndex - 1]
                    + pEnumData->Irp.dwInLength;
        }
        if (++pEnumData->bIndex>=pEnumData->bCount)
        {
            /* finished gathering configs; device is ready for a driver */
            pDriver = MGC_HostFindDriver(pPort->pHostClient, pEnumData->pDevice,
                                         (const uint8_t **)&pEntry);

            /* dump basic device info */
            if (pDriver)
            {
                LOG(0, "HubPort-%d: Driver support, class = 0x%X.\n", 
                    pEnumData->pDevice->bHubPort, pEnumData->pDevice->bDeviceClass);
            }
            else
            {
                LOG(0, "HubPort-%d: Driver not support, class = 0x%X.\n", 
                    pEnumData->pDevice->bHubPort, pEnumData->pDevice->bDeviceClass);
            }

            /* device accepted; add to list */
            VERIFY(MUSB_ListAppendItem(&(pPort->ConnectedDeviceList), pEnumData->pDevice, 0));

            /* perform controller-specific device acceptance */
            if (pPort->pfAcceptDevice(pPort, pEnumData->pDevice, pDriver))
            {
                LOG(0, "HubPort-%d: Device is accepted.\n", pEnumData->pDevice->bHubPort);
            
                pDevice = (MGC_Device *)pEnumData->pDevice->pPrivateData;
                pDevice->pDriver = pDriver;
                if (!pPort->pRootDevice)
                {
                    pPort->pRootDevice = pEnumData->pDevice;
                }
               
                /* inform driver */
                if ((pDriver) && (pDriver->pfDeviceConnected))
                {
                    bRet = pDriver->pfDeviceConnected(pDriver->pPrivateData, pPort, pEnumData->pDevice,
                        pEntry);
                }
            }
            else
            {
                LOG(0, "HubPort-%d: Device is not accepted.\n", pEnumData->pDevice->bHubPort);
            }

            // If device is unsupported, report it to upper layer at here. 
            // If device can be supported, check it at class driver and report from class driver.
            if (!bRet)
            {
                LOG(0, "HubPort-%d: Device unsupported.\n", pEnumData->pDevice->bHubPort);
            
                pEnumData->pDevice->bSupport = MUSB_STATUS_CHILD_CONNECT_UNSUPPORT;
                if (pEnumData->pfChildDeviceUpdate)
                {
                    pEnumData->pfChildDeviceUpdate(pEnumData->pDevice->pParentUsbDevice,
                                                 pEnumData->pDevice, MUSB_STATUS_CHILD_CONNECT_UNSUPPORT);
                }
            }

            // Notify user's enumeration complete callback.
            if (pEnumData->pfEnumerationComplete)
            {
                pEnumData->pfEnumerationComplete(pEnumData->pDevice->pParentUsbDevice,
                                             pEnumData->pDevice, TRUE);
                pEnumData->pfEnumerationComplete = NULL;
            }

            pEnumData->bAddress = 0;
            pEnumData->pDevice = NULL;
            pEnumData->bState = (uint8_t)MGC_EnumStateIdle;
            pEnumData->bRetries = 0;
            break;
        }
        /* continue gathering */
        wLength = *((uint16_t *)
            ((uint32_t)&(pEnumData->pDevice->apConfigDescriptors[pEnumData->bIndex])));
        
        pEnumData->Irp.pInBuffer += pEnumData->Irp.dwInLength;
        pEnumData->Irp.dwInLength = wLength;
        bRet = pServices->pfArmTimer(pServices->pPrivateData, 0, 10, 
            FALSE, MGC_HostGetConfig, pController);
        break;

    default:
        break;
    }

    UNUSED(bRet);
    UNUSED(wValue);
    UNUSED(wIndex);
    UNUSED(pIrp);    
}

/*
 * Try to assign address
 */
uint8_t MGC_AllocateAddress(MGC_EnumerationData *pEnumData)
{
    uint8_t bIndex, bBit;
    uint8_t bmAddress;
    uint8_t bAddress = 0;

    MUSB_ASSERT(pEnumData);

    for (bIndex = 0; bIndex < 16; bIndex++)
    {
        bmAddress = pEnumData->abmAddress[bIndex];
        if (bmAddress < 0xff)
        {
            break;
        }
    }

    if (bmAddress < 0xff)
    {
        bmAddress = ~bmAddress;

        for (bBit = 0; bBit < 8; bBit++)
        {
            if (bmAddress & 1)
            {
                break;
            }
            bmAddress >>= 1;
        }

        bAddress = (bIndex << 3) | bBit;
        MUSB_ASSERT(bIndex < 16);
        pEnumData->abmAddress[bIndex] |= (1 << bBit);
    }

    if (++bAddress>127)
    {
        bAddress = 0;
    }

    if ((1 == bAddress) && bIndex)
    {
        bAddress = 0;
    }
    return bAddress;
}

/*
 * Release an address
 */
void MGC_ReleaseAddress(MGC_EnumerationData *pEnumData, uint8_t bAddress)
{
    uint8_t bIndex = ((uint8_t)(bAddress - 1)) >> 3;
    uint8_t bBit = (bAddress - 1) & 0x7;

    MUSB_ASSERT(pEnumData);

    if (bIndex < 16)
    {
        pEnumData->abmAddress[bIndex] &= ~(uint8_t)(1 << bBit);
    }
}

uint8_t MGC_EnumerateDevice(MGC_Port *pPort, MUSB_Device *pHubDevice,
                            uint8_t bAddress, uint8_t bHubPort,
                            uint8_t bSpeed, MUSB_pfHubEnumerationComplete pfHubEnumerationComplete)
{
    MGC_Device *pDevice;

    MUSB_SystemServices *pServices;
    MGC_EnumerationData *pEnumData;
    uint8_t bRet;

    MUSB_ASSERT(pPort);    
    MUSB_ASSERT(pPort->pController);    
    pServices = pPort->pController->pSystemServices;
    MUSB_ASSERT(pServices);    
    pEnumData = &(pPort->EnumerationData);
    MUSB_ASSERT(pEnumData);
    
    // If it fail to enumerate root device, pEnumData->pDevice != NULL.
    // Just use this as device pointer to re-enumeration root device.
    // Do not need to allocate a duplicate device pointer.
    if (!pEnumData->pDevice)
    {
        /* allocate new device struct */
        pDevice = (MGC_Device *)MUSB_MemAlloc(sizeof(MGC_Device));
    }
    else
    {
        pDevice = (MGC_Device *)pEnumData->pDevice->pPrivateData;
    }    

    if (pDevice)
    {
        MUSB_MemSet(pDevice, 0, sizeof(MGC_Device));

#ifdef MUSB_SCHEDULER   
        MUSB_ListInit(&(pDevice->PipeList));
#endif
        pDevice->Device.pPrivateData = pDevice;
        pEnumData->dwData = 0;
        pEnumData->bAddress = bAddress;
        pEnumData->pDevice = &(pDevice->Device);
        pEnumData->pDevice->pPort = pPort->pInterfacePort;
        pEnumData->pDevice->bHubPort = bHubPort;
        pEnumData->pDevice->ConnectionSpeed = (MUSB_ConnectionSpeed)bSpeed;
        pEnumData->pDevice->pParentUsbDevice = pHubDevice;
#ifdef MUSB_HUB

        if (pHubDevice)
        {
            pDevice->bIsMultiTt = (2
                                       == pEnumData->pDevice->pParentUsbDevice->DeviceDescriptor.bDeviceProtocol);
        }
#endif

        pEnumData->pfEnumerationComplete = pfHubEnumerationComplete;
        pEnumData->Irp.pDevice = pEnumData->pDevice;
        pEnumData->Irp.pfIrpComplete = MGC_HostEnumerator;
        pEnumData->Irp.pCompleteParam = pPort->pController;
        pEnumData->Irp.pInBuffer = pEnumData->aSetupRx;
        pEnumData->Irp.dwInLength = sizeof(pEnumData->aSetupRx);

        //Timer Index 0 is reserved for EP0 device configuration. 
        bRet = pServices->pfArmTimer(pServices->pPrivateData, 
            MUSB_DEV_CONFIG_TIMER_IDX, 10, 
            FALSE, MGC_HostGetMaxDescriptor, pPort->pController);
        return bRet;
    }

    return FALSE;
}

#ifndef MUSB_CODE_SHRINK 
uint8_t MGC_HostDestroy(MGC_Port *pPort)
{
    uint16_t wIndex, wCount;

    MUSB_Device *pDevice;

    MUSB_ASSERT(pPort);

    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));

    for (wIndex = 0; wIndex < wCount; wIndex++)
    {
        pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), 0);

        MUSB_DeviceDisconnected(pDevice);
        MUSB_ListRemoveItem(&(pPort->ConnectedDeviceList), pDevice);
    }

    return TRUE;
}
#endif

#ifdef MUSB_HUB
/*
 * Enumerate a device
 */
uint32_t MUSB_EnumerateDevice(MUSB_Device *pHubDevice, uint8_t bHubPort, uint8_t bSpeed,
                              MUSB_pfHubEnumerationComplete pfHubEnumerationComplete)
{
    uint8_t bAddress;

    uint32_t dwStatus = MUSB_STATUS_OK;
    MGC_Port *pImplPort;
    MGC_EnumerationData *pEnumData;

    MUSB_ASSERT(pHubDevice);    
    MUSB_ASSERT(pHubDevice->pPort);    
    pImplPort = (MGC_Port *)pHubDevice->pPort->pPrivateData;
    MUSB_ASSERT(pImplPort);
    pEnumData = &(pImplPort->EnumerationData);
    MUSB_ASSERT(pEnumData);

    bAddress = MGC_AllocateAddress(pEnumData);

    MUSB_ASSERT(bAddress);
    MUSB_ASSERT((uint8_t)MGC_EnumStateIdle == pEnumData->bState);
    VERIFY(MGC_EnumerateDevice(pImplPort, pHubDevice, bAddress, bHubPort, bSpeed,
                        pfHubEnumerationComplete));


    return dwStatus;
}
#endif

void MUSB_RejectDevice(MUSB_BusHandle hBus, MUSB_Device *pDevice)
{
#ifdef MUSB_SCHEDULER
    MGC_Pipe *pPipe;
#endif
    uint16_t wCount, wIndex;
    MGC_Port *pImplPort;
    MGC_Device *pImplDevice;

    MUSB_ASSERT(hBus);
    pImplPort = (MGC_Port *)hBus;
    MUSB_ASSERT(pDevice);
    pImplDevice = (MGC_Device *)pDevice->pPrivateData;
    MUSB_ASSERT(pImplPort->pController);
    
#ifdef MUSB_SCHEDULER
    if (pImplDevice && pImplDevice->pSchedule)
    {
        /* free all schedule slots */
        MGC_FreeScheduleContents(pImplDevice->pSchedule);

        MUSB_MemFree(pImplDevice->pSchedule);
        pImplDevice->pSchedule = NULL;
    }

    if (pDevice == pImplPort->pRootDevice)
    {
        MGC_FreeScheduleContents(&(pImplPort->Schedule));
    }

    if (pImplDevice)
    {
        /* release bandwidth for current config by walking PipeList */
        wCount = MUSB_ListLength(&(pImplDevice->PipeList));
        for (wIndex = 0; wIndex < wCount; wIndex++)
        {
            pPipe = (MGC_Pipe *)MUSB_ListFindItem(&(pImplDevice->PipeList), 0);
            if (pPipe->pSlot)
            {
                MUSB_ListRemoveItem(&(pPipe->pSlot->PipeList), pPipe);
            }
        }
    }
#endif

    if (pDevice->bBusAddress)
    {
        MGC_ReleaseAddress(&(pImplPort->EnumerationData), pDevice->bBusAddress);
    }

    //  Check if device is removed during enumeration.
    if (pImplPort->EnumerationData.pDevice == pDevice)
    {
        // Clear enumeration data of this device.
        pImplPort->EnumerationData.pDevice = NULL;        
        pImplPort->EnumerationData.bState = (uint8_t)MGC_EnumStateIdle;
        pImplPort->EnumerationData.bRetries = 0;
        
        if (pImplPort->EnumerationData.bAddress)
        {
            MGC_ReleaseAddress(&(pImplPort->EnumerationData), 
                pImplPort->EnumerationData.bAddress);
            pImplPort->EnumerationData.bAddress = 0;
        }
    }

    // device already finish enumeration.
    wCount = MUSB_ListLength(&(pImplPort->ConnectedDeviceList));
    for (wIndex = 0; wIndex < wCount; wIndex++)
    {
        if (pDevice == 
            (MUSB_Device *)MUSB_ListFindItem(&(pImplPort->ConnectedDeviceList), wIndex))
        {
            MUSB_ListRemoveItem(&(pImplPort->ConnectedDeviceList), pDevice);
            break;
        }
    }

    /* release memory */
    if (pDevice->apConfigDescriptors)
    {
        MUSB_MemFree(pDevice->apConfigDescriptors);
        pDevice->apConfigDescriptors = NULL;
    }

    if (pDevice->pDescriptorBuffer)
    {
        MUSB_MemFree(pDevice->pDescriptorBuffer);
        pDevice->pDescriptorBuffer = NULL;
    }

    if (pDevice->pStrManufacturerBuffer)
    {
        MUSB_MemFree(pDevice->pStrManufacturerBuffer);
        pDevice->pStrManufacturerBuffer = NULL;
    }

    if (pDevice->pStrProductBuffer)
    {
        MUSB_MemFree(pDevice->pStrProductBuffer);
        pDevice->pStrProductBuffer = NULL;
    }

    if (pDevice->pStrSerialNumBuffer)
    {
        MUSB_MemFree(pDevice->pStrSerialNumBuffer);
        pDevice->pStrSerialNumBuffer = NULL;
    }

    pDevice->pPrivateData = NULL;

    if (pImplDevice)
    {
        MUSB_MemFree(pImplDevice);
    }
}
#if 0
void MUSB_RejectInterfaces(MUSB_BusHandle hBus, MUSB_Device *pDevice, uint8_t *abInterfaceNumber,
                           uint8_t bInterfaceCount)
{
    MGC_Device *pVirtualDevice;

    uint8_t *pEntry;
    MUSB_DeviceDriver *pDriver;
    uint8_t bIndex, bSearchIndex, bNumber;
    const MUSB_InterfaceDescriptor *pInterface;
    uint8_t *pDest;
    uint16_t wLength, wNewTotalLength;
    MGC_Port *pPort;
    const MUSB_InterfaceDescriptor *pPrevInterface = NULL;
    const MUSB_ConfigurationDescriptor *pConfigDesc;
    uint16_t wTotalLength;
    uint8_t bRet;
    
    MUSB_ASSERT(hBus);
    pPort = (MGC_Port *)hBus;
    MUSB_ASSERT(pDevice);
    pConfigDesc = (pDevice->pCurrentConfiguration)
                          ? pDevice->pCurrentConfiguration : pDevice->apConfigDescriptors[0];
    wTotalLength = MUSB_SWAP16(pConfigDesc->wTotalLength);
    MUSB_ASSERT(abInterfaceNumber);

    /* create clone */
    pVirtualDevice = (MGC_Device *)MUSB_MemAlloc(sizeof(MGC_Device));

    if (!pVirtualDevice)
    {
        MUSB_DIAG_STRING(1, "Memory allocation error for virtual device");
        return;
    }

    MUSB_MemSet(pVirtualDevice, 0, sizeof(MGC_Device));
    MUSB_MemCopy(&(pVirtualDevice->Device), pDevice, sizeof(MUSB_Device));
    MUSB_ListInit(&(pVirtualDevice->PipeList));
    pVirtualDevice->Device.pPrivateData = pVirtualDevice;

    /* but make private copy of descriptors (first config only for now) */
    pVirtualDevice->Device.apConfigDescriptors =
        (MUSB_ConfigurationDescriptor **)MUSB_MemAlloc(sizeof(MUSB_ConfigurationDescriptor *));

    if (!pVirtualDevice->Device.apConfigDescriptors)
    {
        MUSB_MemFree(pVirtualDevice);

        MUSB_DIAG_STRING(1, "Memory allocation error for virtual device");
        return;
    }

    /* to avoid an extra loop to accumulate required size, just make it the total size */
    pVirtualDevice->Device.pDescriptorBuffer = (uint8_t *)MUSB_MemAlloc(wTotalLength);

    if (!pVirtualDevice->Device.pDescriptorBuffer)
    {
        MUSB_MemFree(pVirtualDevice->Device.apConfigDescriptors);

        MUSB_MemFree(pVirtualDevice);
        MUSB_DIAG_STRING(1, "Memory allocation error for virtual device");
        return;
    }

    pVirtualDevice->Device.apConfigDescriptors[0] =
        (MUSB_ConfigurationDescriptor *)((void *)pVirtualDevice->Device.pDescriptorBuffer);
    pVirtualDevice->Device.pCurrentConfiguration = pVirtualDevice->Device.apConfigDescriptors[0];
    pVirtualDevice->Device.wDescriptorBufferLength = wTotalLength;
    pDest = pVirtualDevice->Device.pDescriptorBuffer;
    MUSB_MemCopy(pDest, pConfigDesc, MUSB_DT_CONFIG_SIZE);
    pDest += MUSB_DT_CONFIG_SIZE;

    /* do the job */
    wNewTotalLength = MUSB_DT_CONFIG_SIZE;
    /* to avoid depending on descriptor analysis library, use the primitive finder */
    bSearchIndex = 0;

    for (bIndex = 0; bIndex < bInterfaceCount; )
    {
        bNumber = abInterfaceNumber[bIndex];

        pInterface = (MUSB_InterfaceDescriptor *)((void *)MGC_FindDescriptor(
            (const uint8_t *)pConfigDesc, wTotalLength, MUSB_DT_INTERFACE, bSearchIndex));
        /* continue until desired interface is found */
        if (!pInterface)
        {
            /* no such interface; give up */
            break;
        }

        if (pInterface->bInterfaceNumber != bNumber)
        {
            bSearchIndex++;
            continue;
        }

        /* find subsequent non-matching interface */
        pPrevInterface = pInterface;

        while (pInterface && (pInterface->bInterfaceNumber == bNumber))
        {
            bSearchIndex++;
            pInterface = (MUSB_InterfaceDescriptor *)((void *)MGC_FindDescriptor((const uint8_t *)pConfigDesc,
                                                               wTotalLength,
                                                               MUSB_DT_INTERFACE,
                                                               bSearchIndex));
        }

        if (!pInterface)
        {
            /* must be tail case */
            break;
        }

        /* copy all descriptors possibly associated with the interface */
        wLength = (uint16_t)((intptr_t)pInterface - (intptr_t)pPrevInterface);
        MUSB_MemCopy(pDest, pPrevInterface, wLength);
        pDest += wLength;
        wNewTotalLength += wLength;

        /* get ready to find next one */
        pPrevInterface = NULL;
        bIndex++;
    }

    /* handle tail case */
    if (pPrevInterface)
    {
        /* copy all descriptors possibly associated with the interface */
        wLength = wTotalLength - (uint16_t)((intptr_t)pPrevInterface - (intptr_t)pConfigDesc);

        MUSB_MemCopy(pDest, pPrevInterface, wLength);
        wNewTotalLength += wLength;
    }

    /* adjust virtual config's total length */
    pVirtualDevice->Device.pCurrentConfiguration->wTotalLength = MUSB_SWAP16(wNewTotalLength);

    pDriver = MGC_HostFindDriver(pPort->pHostClient, &(pVirtualDevice->Device),
                                 (const uint8_t **)&pEntry);

    if (pDriver)
    {
        /* device accepted; add to list */
        pVirtualDevice->pDriver = pDriver;

        VERIFY(MUSB_ListAppendItem(&(pPort->ConnectedDeviceList), &(pVirtualDevice->Device), 0));
        /* inform driver */
        bRet = pDriver->pfDeviceConnected(pDriver->pPrivateData, pPort, &(pVirtualDevice->Device), pEntry);
        UNUSED(bRet);        
    }
    else
    {
        MUSB_MemFree(pVirtualDevice->Device.pDescriptorBuffer);

        MUSB_MemFree(pVirtualDevice->Device.apConfigDescriptors);
        MUSB_MemFree(pVirtualDevice);
    }
}
#endif

/*
 * A device has been disconnected
 */
void MUSB_DeviceDisconnected(MUSB_Device *pDevice)
{
    MUSB_Port *pPort;

    MGC_Port *pImplPort;
    MGC_Device *pImplDevice;
    MUSB_DeviceDriver *pDriver = NULL;
    MGC_EnumerationData *pEnumData;
    uint8_t bRet = FALSE;
    uint32_t dwChildUsbDeviceStatus;

    if (pDevice)
    {
        pPort = pDevice->pPort;
        MUSB_ASSERT(pPort);
        pImplPort = (MGC_Port *)pPort->pPrivateData;
        pEnumData = &(pImplPort->EnumerationData);
        MUSB_ASSERT(pEnumData);
        pImplDevice = (MGC_Device *)pDevice->pPrivateData;

        if (pImplDevice)
        {
            pDriver = pImplDevice->pDriver;
        }

        /* callback */
        if (pDriver && pDriver->pfDeviceDisconnected)
        {
            bRet = pDriver->pfDeviceDisconnected(pDriver->pPrivateData, (MUSB_BusHandle)pImplPort,
                                          pDevice);
        }

        // Report disconnection to upper layer only 
        // when driver had reported connection to upper layer.
        if ((pDevice->bSupport == MUSB_STATUS_CHILD_CONNECT) ||
             (pDevice->bSupport == MUSB_STATUS_CHILD_CONNECT_UNSUPPORT))
        {
            if ((!bRet) || (pDevice->bSupport == MUSB_STATUS_CHILD_CONNECT_UNSUPPORT))
            {
                dwChildUsbDeviceStatus = MUSB_STATUS_CHILD_DISCONNECT_UNSUPPORT;
            }
            else
            {
                dwChildUsbDeviceStatus = MUSB_STATUS_CHILD_DISCONNECT;
            }
                        
            if (pEnumData->pfChildDeviceUpdate)
            {
                pEnumData->pfChildDeviceUpdate(pDevice->pParentUsbDevice,
                                                 pDevice, dwChildUsbDeviceStatus);
            }

            // clear support status.
            pDevice->bSupport = MUSB_STATUS_CHILD_UNKNOW;            
        }        

        MUSB_RejectDevice(pImplPort, pDevice);
    }
}

/*
 * Selective suspend
 */
#if 0
uint32_t MUSB_SetTreeSuspend(MUSB_Device *pHubDevice, uint8_t bHubPort, uint8_t bSuspend)
{
    /* TODO: figure out which driver is the hub driver and call it */
    return MUSB_STATUS_UNSUPPORTED;
}
#endif
/*
 * Start the next queued control transfer, if any
 */
void MGC_StartNextControlTransfer(MGC_Port *pPort)
{
    MUSB_ControlIrp *pIrp = NULL;
    MGC_EndpointResource *pEnd = NULL;
    uint32_t u4Ret = 0;
    
    MUSB_ASSERT(pPort);
    
    /* NOTE: assumes EP0 is first */
    pEnd = MUSB_ArrayFetch(&(pPort->LocalEnds), 0);

    if (pEnd)
    {
        pIrp = (MUSB_ControlIrp *)MUSB_ListFindItem(&(pEnd->TxIrpList), 0);
        if (pIrp)
        {
            MUSB_ListRemoveItem(&(pEnd->TxIrpList), pIrp);

#ifndef MTK_MHDRC               
            //  MTK Notice: Max Liao, 2006/11/28.
            //  set device address in ep basis for supporting Hub.
            if (pPort->bFuncAddr != pIrp->pDevice->bBusAddress)
            {
                pPort->bFuncAddr = pIrp->pDevice->bBusAddress;
                pPort->pfProgramBusState(pPort);
            }
#endif

            pIrp->dwActualInLength = 0;
            pIrp->dwActualOutLength = 0;
            pIrp->dwStatus = MUSB_STATUS_OK;
            pEnd->bIsTx = TRUE;
            u4Ret = pPort->pfProgramStartTransmit(pPort, pEnd, pIrp->pOutBuffer, 8, pIrp);
            UNUSED(u4Ret);
        }
        else
        {
            pEnd->pTxIrp = NULL;
        }
    }
}

/*
 * Run scheduled transfers.
 * This function needs to be relatively quick, 
 * so it assumes the schedule is properly maintained.
 */
#if defined(MUSB_SCHEDULER)
uint8_t MGC_RunScheduledTransfers(MGC_Port *pPort)
{
    MUSB_LinkedList *pSlotElement;
    MGC_ScheduleSlot *pSlot;
    MUSB_LinkedList *pPipeElement;
    MGC_Pipe *pPipe;
    MUSB_Irp *pIrp;
    MUSB_IsochIrp *pIsochIrp;
    uint32_t dwFrameBits;
    uint32_t dwEffectiveFrame;
    MGC_EndpointResource *pEnd;
    void *pGenIrp;
    uint8_t bIsTx, bTrafficType;
    uint8_t bAllowDma = FALSE;
    uint8_t *pBuffer = NULL;
    uint32_t dwLength = 0;
    uint32_t u4Ret;
    
    MUSB_ASSERT(pPort);

    pSlotElement = &(pPort->Schedule.ScheduleSlots);

    while (pSlotElement)
    {
        pSlot = (MGC_ScheduleSlot *)pSlotElement->pItem;

        if (pSlot)
        {
            /* set active if transfer is required */
            dwEffectiveFrame = pPort->dwFrame - pSlot->wFrameOffset;
            /* NOTE: wInterval is guaranteed to be a power of 2 */
            dwFrameBits = pSlot->wInterval | (pSlot->wInterval - 1);

            if (!pSlot->bIsActive && (pSlot->wInterval == (dwEffectiveFrame & dwFrameBits)))
            {
                pSlot->bIsActive = TRUE;
            }
            if (pSlot->bIsActive)
            {
                /* active; prepare slot transfers */
                pPipeElement = &(pSlot->PipeList);

                while (pPipeElement)
                {
                    pPipe = (MGC_Pipe *)pPipeElement->pItem;

                    if (pPipe)
                    {
                        pEnd = pPipe->pLocalEnd;

                        bIsTx = (pPipe->bmFlags & MGC_PIPEFLAGS_TRANSMIT) ? TRUE : FALSE;
                        bTrafficType = bIsTx ? pEnd->bTrafficType : pEnd->bRxTrafficType;

                        switch (bTrafficType)
                        {
                        case MUSB_ENDPOINT_XFER_INT:
                            pIrp = bIsTx ? (MUSB_Irp *)pEnd->pTxIrp : (MUSB_Irp *)pEnd->pRxIrp;
                            pGenIrp = pIrp;
                            bAllowDma = pIrp->bAllowDma;
                            pBuffer = pIrp->pBuffer;
                            dwLength = pIrp->dwLength;
                            break;
#ifdef MUSB_ISO

                        case MUSB_ENDPOINT_XFER_ISOC:
                            pIsochIrp = bIsTx ? (MUSB_IsochIrp *)pEnd->pTxIrp : (MUSB_IsochIrp *)
                                                                                    pEnd->pRxIrp;
                            pGenIrp = pIsochIrp;
                            bAllowDma = pIsochIrp->bAllowDma;
                            if (pPipe->bmFlags & MGC_PIPEFLAGS_TRANSMIT)
                            {
                                pBuffer = pIsochIrp->pBuffer + pEnd->dwTxOffset;
                            }
                            else
                            {
                                pBuffer = pIsochIrp->pBuffer + pEnd->dwRxOffset;
                            }
                            dwLength = pIsochIrp->adwLength[pIsochIrp->wCurrentFrame];
                            break;
#endif

                        }
                        if (bIsTx)
                        {
                            u4Ret = pPort->pfProgramStartTransmit(pPort, pEnd, pBuffer, dwLength, pGenIrp);
                            UNUSED(u4Ret);
                        }
                        else
                        {
                            u4Ret = pPort->pfProgramStartReceive(
                                pPort, pEnd, pBuffer, dwLength, pGenIrp, bAllowDma);
                            UNUSED(u4Ret);                                  
                        }
                    }
                    pPipeElement = pPipeElement->pNext;
                }
                /* check if last frame */
                if (0 == --pSlot->wFramesRemaining)
                {
                    /* last frame in slot; mark as inactive */
                    pSlot->bIsActive = FALSE;
                    pSlot->wFramesRemaining = pSlot->wDuration;
                }
            }
        }
        pSlotElement = pSlotElement->pNext;
    }

    return TRUE;
}
#endif

/*
 * Free a schedule's contents
 */
#ifdef MUSB_SCHEDULER
static void MGC_FreeScheduleContents(MGC_Schedule *pSchedule)
{
    MGC_ScheduleSlot *pSlot;

    MGC_Pipe *pPipe;
    uint16_t wSlotCount, wPipeCount;
    uint16_t wSlotIndex, wPipeIndex;

    MUSB_ASSERT(pSchedule);

    wSlotCount = MUSB_ListLength(&(pSchedule->ScheduleSlots));

    for (wSlotIndex = 0; wSlotIndex < wSlotCount; wSlotIndex++)
    {
        pSlot = (MGC_ScheduleSlot *)MUSB_ListFindItem(&(pSchedule->ScheduleSlots), 0);

        MUSB_ListRemoveItem(&(pSchedule->ScheduleSlots), pSlot);
        /* remove pipes but do not free (close frees them) */
        wPipeCount = MUSB_ListLength(&(pSlot->PipeList));

        for (wPipeIndex = 0; wPipeIndex < wPipeCount; wPipeIndex++)
        {
            pPipe = (MGC_Pipe *)MUSB_ListFindItem(&(pSlot->PipeList), 0);
            MUSB_ListRemoveItem(&(pSlot->PipeList), pPipe);
        }

        MUSB_MemSet(pSlot, 0, sizeof(MGC_ScheduleSlot));
        MUSB_MemFree(pSlot);
    }

    pSchedule->dwTotalTime = 0L;

    //  MTK Notice: Max Liao, 2006/11/28.
    //  free pSchedule->wSlotCount at Hub disconnect.
    pSchedule->wSlotCount = 0;
}

/*
* Get the time in a slot
*/
static uint32_t MGC_GetSlotTime(MGC_ScheduleSlot *pSlot)
{
    MGC_Pipe *pPipe;
    uint16_t wPipeIndex;
    uint32_t dwSlotTime = 0L;

    MUSB_ASSERT(pSlot);

    /* accumulate time taken by all pipes in this slot */
    wPipeIndex = 0;
    pPipe = MUSB_ListFindItem(&(pSlot->PipeList), wPipeIndex++);

    while (pPipe)
    {
        dwSlotTime += pPipe->dwMaxBusTime;
        pPipe = MUSB_ListFindItem(&(pSlot->PipeList), wPipeIndex++);
    }

    return dwSlotTime;
}

/*
 * Find a slot in the schedule for the given bus time.
 * Returns: NULL on failure; otherwise slot pointer
 */

static MGC_ScheduleSlot *MGC_FindScheduleSlot(MGC_Schedule *pSchedule,
                                              MGC_BusTimeInfo *pBusTimeInfo,
                                              uint16_t wInterval,
                                              uint16_t wDuration,
                                              uint32_t dwBusTime)
{
    MUSB_LinkedList *pElement;
    MGC_ScheduleSlot *pSlot;
    uint32_t dwSlotTime;
    uint32_t dwOffset = 0L;
    uint16_t wMaxOffset = 0;

    MUSB_ASSERT(pSchedule);
    MUSB_ASSERT(pBusTimeInfo);

    /* walk schedule looking for space in existing slot */
    pElement = &(pSchedule->ScheduleSlots);

    while (pElement)
    {
        /* walk slots looking for compatible interval/duration and available time */
        pSlot = (MGC_ScheduleSlot *)pElement->pItem;

        if (pSlot)
        {
            dwOffset = pSlot->wFrameOffset + pSlot->wDuration;
            /* track maximum offset in case we need to try a new slot */
            if ((dwOffset - 1) > wMaxOffset)
            {
                wMaxOffset = (uint16_t)(dwOffset - 1);
            }
            if ((pSlot->wInterval == wInterval) && (pSlot->wDuration == wDuration))
            {
                dwSlotTime = MGC_GetSlotTime(pSlot);
                /* if time remains in this slot, use it */
                if ((dwSlotTime + dwBusTime) <= pBusTimeInfo->dwMaxPeriodic)
                {
                    return pSlot;
                }
            }
        }
        pElement = pElement->pNext;
    }

    /* see if we can create a new slot */
    if (((dwOffset + wDuration) < pSchedule->wSlotCount)
            && (dwBusTime <= pBusTimeInfo->dwMaxPeriodic) && (wMaxOffset <= (wInterval >> 1)))
    {
        pSlot = MUSB_MemAlloc(sizeof(MGC_ScheduleSlot));
        if (pSlot)
        {
            pSlot->bIsActive = FALSE;

            pSlot->wDuration = wDuration;
            pSlot->wFrameOffset = (uint16_t)dwOffset;
            pSlot->wInterval = wInterval;
            MUSB_ListInit(&(pSlot->PipeList));
            if (MUSB_ListAppendItem(&(pSchedule->ScheduleSlots), pSlot, 0))
            {
                /* update slot count if needed */
                if (wInterval < pSchedule->wSlotCount)
                {
                    pSchedule->wSlotCount = wInterval;
                }
                return pSlot;
            }
        }
    }

    /* can't schedule or out of memory */
    return NULL;
}

static uint32_t MGC_ComputeBandwidth(MGC_BusTimeInfo *pBusTimeInfo, uint8_t bTrafficType,
                                     uint8_t bIsIn, uint16_t wMaxPacketSize)
{
    uint32_t dwBusTime;

    uint32_t dwBitStuffTime;

    MUSB_ASSERT(pBusTimeInfo);

    dwBusTime = pBusTimeInfo->dwControllerSetup;

    /*
     * Compute Floor(3.167 * BitStuffTime(wMaxPacketSize)),
     * where BitStuffTime(n) = 7 * 8 * n / 6,
     * ASSUMES max payload per USB 2.0 (3072); larger than this may overflow numerator
     */
    dwBitStuffTime = (3167L *56 *wMaxPacketSize) / 6000;

    /* Add payload-dependent time term */
    dwBusTime += (pBusTimeInfo->dwPayloadScale *dwBitStuffTime) / 10;

    switch (bTrafficType)
    {
#ifdef MUSB_ISO

    case MUSB_ENDPOINT_XFER_ISOC:
        /* Additional time term */
        if (bIsIn)
        {
            dwBusTime += pBusTimeInfo->dwIsoInOverhead;
        }
        else
        {
            dwBusTime += pBusTimeInfo->dwIsoOutOverhead;
        }
        break;
#endif

    default:
        /* Additional time term */
        if (bIsIn)
        {
            dwBusTime += pBusTimeInfo->dwInOverhead;
        }
        else
        {
            dwBusTime += pBusTimeInfo->dwOutOverhead;
        }
    }

    return dwBusTime;
}

/**
 * Verify that the bandwidth required for the given pipe fits,
 * both on its local bus (if hub support is enabled) and
 * on the global bus rooted at the port.
 * Update the pipe's slot, bus time, and traffic type & max packet size
 * (since they are computed here anyway)
 * @return TRUE on success (bandwidth available and committed,
 * and pipe updated)
 * @return FALSE on failure (insufficient available bandwidth
 * or out of memory on allocating a bookkeeping struct)
 */
static uint8_t MGC_CommitBandwidth(MGC_Port *pPort, MGC_Pipe *pPipe,
                                   const MUSB_DeviceEndpoint *pRemoteEnd)
{
    MGC_Device *pImplDevice;

    MGC_Schedule *pSchedule;
    uint32_t dwBusTime;
    uint32_t dwLocalBusTime;
    uint16_t wMaxPacketSize;
    MGC_BusTimeInfo *pBusTimeInfo;
    MGC_BusTimeInfo *pLocalBusTimeInfo = NULL;
    uint16_t wDuration = 1;
    uint8_t bOk = FALSE;
    const MUSB_Device *pDevice;
    MGC_ScheduleSlot *pLocalSlot = NULL;
    MGC_ScheduleSlot *pSlot = NULL;
    uint8_t bInterval;
    uint8_t bTrafficType;
    uint8_t bIsIn;
    uint16_t wInterval;

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pPipe);
    MUSB_ASSERT(pRemoteEnd);
    pDevice = pRemoteEnd->pDevice;
    bInterval = pRemoteEnd->UsbDescriptor.bInterval & 0xf;
    bTrafficType = pRemoteEnd->UsbDescriptor.bmAttributes & MUSB_ENDPOINT_XFERTYPE_MASK;
    bIsIn = (pRemoteEnd->UsbDescriptor.bEndpointAddress & MUSB_DIR_IN) ? TRUE : FALSE;
    wInterval = bInterval;
    
    /* compute needed bandwidth */
    if (pPort->bIsHighSpeed)
    {
        pBusTimeInfo = &MGC_HighSpeedFrame;
    }
    else if (pPort->bIsLowSpeed)
    {
        pBusTimeInfo = &MGC_LowSpeedFrame;
    }
    else
    {
        pBusTimeInfo = &MGC_FullSpeedFrame;
    }

    switch (pRemoteEnd->pDevice->ConnectionSpeed)
    {
    case MUSB_CONNECTION_SPEED_HIGH:
        pLocalBusTimeInfo = &MGC_HighSpeedFrame;
        wDuration = (MUSB_SWAP16P((uint8_t *)&(pRemoteEnd->UsbDescriptor.wMaxPacketSize))
                         & MUSB_M_ENDPOINT_PACKETS_PER_FRAME) >> MUSB_S_ENDPOINT_PACKETS_PER_FRAME;
        if (!wDuration)
        {
            wDuration = 1;
        }
        
        if ((bTrafficType == MUSB_ENDPOINT_XFER_ISOC) ||
            (bTrafficType == MUSB_ENDPOINT_XFER_INT))
        {
            wInterval = 1 << (bInterval - 1);
        }
        break;

    case MUSB_CONNECTION_SPEED_FULL:
        pLocalBusTimeInfo = &MGC_FullSpeedFrame;
        if (bTrafficType == MUSB_ENDPOINT_XFER_ISOC)
        {
            wInterval = 1 << (bInterval - 1);
        }
        break;

    case MUSB_CONNECTION_SPEED_LOW:
        pLocalBusTimeInfo = &MGC_LowSpeedFrame;
        break;

    default:
        break;        
    }

    wMaxPacketSize = wDuration *(
                         MUSB_SWAP16P((uint8_t *)&(pRemoteEnd->UsbDescriptor.wMaxPacketSize))
                             & MUSB_M_ENDPOINT_MAX_PACKET_SIZE);

    dwBusTime = MGC_ComputeBandwidth(pBusTimeInfo, bTrafficType, bIsIn, wMaxPacketSize);
    dwLocalBusTime = MGC_ComputeBandwidth(pLocalBusTimeInfo, bTrafficType, bIsIn, wMaxPacketSize);
    UNUSED(dwLocalBusTime);
    /*
     * Verify that the needed bandwidth fits.
     */
    switch (bTrafficType)
    {
    case MUSB_ENDPOINT_XFER_ISOC:
    case MUSB_ENDPOINT_XFER_INT:
        pSchedule = &(pPort->Schedule);
        pSlot = MGC_FindScheduleSlot(pSchedule, pBusTimeInfo, wInterval, wDuration, dwBusTime);
        if (pSlot)
        {
            bOk = TRUE;

            if (pDevice->pParentUsbDevice)
            {
                pImplDevice = (MGC_Device *)pDevice->pParentUsbDevice->pPrivateData;

                pSchedule = pImplDevice->pSchedule;

                if (!pSchedule)
                {
                    pSchedule = MUSB_MemAlloc(sizeof(MGC_Schedule));
                    if (pSchedule)
                    {
                        pImplDevice->pSchedule = pSchedule;

                        MUSB_MemSet(pSchedule, 0, sizeof(MGC_Schedule));
                        MUSB_ListInit(&(pSchedule->ScheduleSlots));
                    }
                }
                if (pSchedule)
                {
                    pLocalSlot =
                        MGC_FindScheduleSlot(pSchedule, pLocalBusTimeInfo, wInterval, wDuration,
                                             dwBusTime);
                    if (pLocalSlot)
                    {
                        bOk = MUSB_ListAppendItem(&(pLocalSlot->PipeList), pPipe, 0);
                    }
                    else
                    {
                        bOk = FALSE;
                    }
                }
                else
                {
                    bOk = FALSE;
                }
            }
            if (bOk)
            {
                bOk = MUSB_ListAppendItem(&(pSlot->PipeList), pPipe, 0);
                pPipe->pSlot = pSlot;
            }
        }
        break;

    default:
        /*
         * We assume the periodic bandwidth limits in the spec are
         * intended to allow control/bulk transfers to fit somewhere
         */
        bOk = TRUE;
    }

    if (bOk)
    {
        pPipe->dwMaxBusTime = dwBusTime;

        pPipe->bTrafficType = bTrafficType;
        pPipe->wMaxPacketSize = wMaxPacketSize;
    }

    return bOk;
}
#endif /* MUSB_SCHEDULER */

/*
 * Open a pipe
 */
MUSB_Pipe MUSB_OpenPipe(MUSB_BusHandle hBus, const MUSB_DeviceEndpoint *pRemoteEnd,
                        MUSB_EndpointResource *pEndpointResource)
{
    MUSB_EndpointResource end;
    void *pResult = NULL;
    MGC_Pipe *pPipe = NULL;
    MUSB_EndpointResource *pLocalEnd = pEndpointResource;
    MGC_EndpointResource *pResource = NULL;
    MGC_Port *pPort;
    const MUSB_Device *pDevice;
#ifdef MUSB_SCHEDULER
    uint8_t bIsTx = FALSE;
    MGC_Device *pImplDevice;
    uint8_t bBandwidthOk = FALSE;
#endif

    MUSB_ASSERT(hBus);
    MUSB_ASSERT(pRemoteEnd);
    pPort = (MGC_Port *)hBus;
    MUSB_ASSERT(pPort);    
    pDevice = pRemoteEnd->pDevice;
#ifdef MUSB_SCHEDULER
    pImplDevice = (MGC_Device *)pDevice->pPrivateData;
#endif

    /* update bus speed reading */
    VERIFY(0 == pPort->pfReadBusState(pPort));

    /* try to allocate */
    pPipe = (MGC_Pipe *)MUSB_MemAlloc(sizeof(MGC_Pipe));

    if (pPipe)
    {
        /* allocated pipe; fill info */
        MUSB_MemSet(pPipe, 0, sizeof(MGC_Pipe));

        pPipe->hSession = hBus;
        pPipe->pPort = pPort;

        if (pRemoteEnd->UsbDescriptor.bEndpointAddress & MUSB_DIR_IN)
        {
            pPipe->bmFlags = (uint32_t)MGC_PIPEFLAGS_HOST;
#ifdef MUSB_SCHEDULER
            bIsTx = FALSE;
#endif            
        }
        else
        {
            pPipe->bmFlags = (uint32_t)(MGC_PIPEFLAGS_HOST | MGC_PIPEFLAGS_TRANSMIT);
#ifdef MUSB_SCHEDULER
            bIsTx = TRUE;
#endif            
        }

        pPipe->pDevice = pDevice;

        /* try to get core resource */
        if (!pLocalEnd)
        {
            end.bmFlags = 0;

            end.dwBufferSize = pRemoteEnd->UsbDescriptor.wMaxPacketSize;
            pLocalEnd = &end;
        }

        pResource = pPort->pfBindEndpoint(pPort, pRemoteEnd, pLocalEnd, TRUE);
#ifdef MUSB_SCHEDULER
        if (pResource)
        {
            /* got resource; try to get bandwidth if applicable */
            bBandwidthOk = MGC_CommitBandwidth(pPort, pPipe, pRemoteEnd);
        }
        /* if all OK so far, try to add pipe to device pipe list */
        if (pResource && bBandwidthOk && MUSB_ListAppendItem(&(pImplDevice->PipeList), pPipe, 0))
        {
            /* success */
            pPipe->pLocalEnd = pResource;
            pResult = pPipe;
        }
        else
        {
            /* failure cleanup */
            if (pResource)
            {
                if (bIsTx)
                {
                    pResource->bIsClaimed = FALSE;
                }
                else
                {
                    pResource->bRxClaimed = FALSE;
                }
                VERIFY(0 == pPort->pfProgramFlushEndpoint(pPort,
                                              pResource,
                                              pRemoteEnd->UsbDescriptor.bEndpointAddress
                                                  & MUSB_DIR_IN,
                                              FALSE));
            }
            MUSB_MemFree(pPipe);
        }
#else

        if (pResource)
        {        
            /* success */
            pPipe->pLocalEnd = pResource;
            pResult = pPipe;

            pPipe->bTrafficType = 
                pRemoteEnd->UsbDescriptor.bmAttributes & MUSB_ENDPOINT_XFERTYPE_MASK;
            pPipe->wMaxPacketSize = 
                MUSB_SWAP16P((uint8_t *)&(pRemoteEnd->UsbDescriptor.wMaxPacketSize));            
        }
#endif

    }

    return pResult;
}

/*
 * Close a pipe
 */
void MUSB_ClosePipe(MUSB_Pipe hPipe)
{
    //    uint8_t bDirection;
    MGC_Pipe *pPipe;
#ifdef MUSB_SCHEDULER   
    const MUSB_Device *pDevice;
    MGC_Device *pImplDevice;
#endif
    MGC_EndpointResource *pEnd;
    uint8_t bIsTx;
    
    MUSB_ASSERT(hPipe);
    pPipe = (MGC_Pipe *)hPipe;
#ifdef MUSB_SCHEDULER      
    pDevice = pPipe->pDevice;
    MUSB_ASSERT(pDevice);    
    pImplDevice = (MGC_Device *)pDevice->pPrivateData;
    MUSB_ASSERT(pImplDevice);
#endif    
    pEnd = pPipe->pLocalEnd;
    MUSB_ASSERT(pEnd);

    pPipe->bmFlags |= MGC_PIPEFLAGS_CLOSING;
    bIsTx = (pPipe->bmFlags&MGC_PIPEFLAGS_TRANSMIT) ? TRUE : FALSE;        

#ifdef MUSB_SCHEDULER
    if (pPipe->pSlot)
    {
        MUSB_ListRemoveItem(&(pPipe->pSlot->PipeList), pPipe);
    }

    MUSB_ListRemoveItem(&(pImplDevice->PipeList), pPipe);
#endif

    //  MTK Notice: Max Liao, 2006/11/09.
    //  skip reset to zero because pPipe will be release.   
    //    MUSB_MemSet(pPipe, 0, sizeof(MGC_Pipe));
    MUSB_MemFree(pPipe);

    //  MTK Notice: Max Liao, 2006/11/29.
    //  check tx/rx direction before clean bIsClaimed or bRxClaimed.
    if (pEnd->bIsFifoShared)
    {
        pEnd->bIsClaimed = FALSE;
        pEnd->bRxClaimed = FALSE;
    }
    else
    {
        if (bIsTx)
        {
            pEnd->bIsClaimed = FALSE;
        }
        else
        {
            pEnd->bRxClaimed = FALSE;
        }
    }
}

/*
 * Start a single control transfer
 */
uint32_t MUSB_StartControlTransfer(MUSB_Port *pPort, MUSB_ControlIrp *pIrp)
{
    uint32_t status = (uint32_t)MUSB_STATUS_NO_RESOURCES;
    MGC_Port *pImplPort;
    MGC_EndpointResource *pEnd = NULL;

    MUSB_ASSERT(pPort);
    pImplPort = (MGC_Port *)pPort->pPrivateData;
    MUSB_ASSERT(pImplPort);  
    MUSB_ASSERT(pIrp);

    pIrp->dwActualInLength = 0;
    pIrp->dwActualOutLength = 0;
    // Silent Reset: Use polling Irp->dwStatus in usb task.
    // set a magic number to wait Irp end.    
    //pIrp->dwStatus = MUSB_STATUS_OK;

    /* NOTE: assumes EP0 is first */
    pEnd = MUSB_ArrayFetch(&(pImplPort->LocalEnds), 0);

    if (pEnd)
    {
        if (!pEnd->pTxIrp)
        {
            pEnd->bIsTx = TRUE;

            if (!pEnd->wPacketSize)
            {
                pEnd->wPacketSize = MUSB_MAX(8, pIrp->pDevice->DeviceDescriptor.bMaxPacketSize0);
            }

#ifndef MTK_MHDRC               
            //  MTK Notice: Max Liao, 2006/11/28.
            //  set device address in ep basis for supporting Hub.
            if (pImplPort->bFuncAddr != pIrp->pDevice->bBusAddress)
            {
                pImplPort->bFuncAddr = pIrp->pDevice->bBusAddress;
                pImplPort->pfProgramBusState(pImplPort);
            }
#endif

            status = pImplPort->pfProgramStartTransmit(pImplPort, pEnd, pIrp->pOutBuffer, 8, pIrp);
        }
        else
        {
            if (MUSB_ListAppendItem(&(pEnd->TxIrpList), pIrp, 0))
            {
                status = MUSB_STATUS_OK;
            }
        }
    }
  
    return status;
}

/*
 * Cancel a pending control transfer
 */
uint32_t MUSB_CancelControlTransfer(MUSB_Port *pPort, MUSB_ControlIrp *pIrp)
{
    void *pListIrp;

    uint16_t wCount, wIndex;
    uint32_t status = MUSB_STATUS_OK;
    MGC_Port *pImplPort;
    MGC_EndpointResource *pEnd = NULL;

    MUSB_ASSERT(pPort);
    pImplPort = (MGC_Port *)pPort->pPrivateData;
    MUSB_ASSERT(pImplPort);
    MUSB_ASSERT(pIrp);

    pIrp->dwActualInLength = 0;
    pIrp->dwActualOutLength = 0;
    pIrp->dwStatus = MUSB_STATUS_OK;

    /* NOTE: assumes EP0 is first */
    pEnd = MUSB_ArrayFetch(&(pImplPort->LocalEnds), 0);

    if (pEnd)
    {
        if (pEnd->pTxIrp == pIrp)
        {
            /* it is the current IRP: flush, remove, start next */
            VERIFY(0 == pImplPort->pfProgramFlushEndpoint(pImplPort, pEnd, 0, FALSE));

            pEnd->pTxIrp = NULL;
            MGC_StartNextControlTransfer(pImplPort);
        }
        else
        {
            /* not the current IRP; try to find in list */
            wCount = MUSB_ListLength(&(pEnd->TxIrpList));
            for (wIndex = 0; wIndex < wCount; wIndex++)
            {
                pListIrp = MUSB_ListFindItem(&(pEnd->TxIrpList), wIndex);
                if (pListIrp == pIrp)
                {
                    MUSB_ListRemoveItem(&(pEnd->TxIrpList), pListIrp);
                }
            }
        }
    }

    return status;
}

static void MGC_DriverTimerExpired(void *pControllerPrivateData, uint16_t wTimerIndex)
{
    MGC_Controller *pController;
    MGC_Port *pPort;
    void *pParam;
    MUSB_pfDriverTimerExpired pfDriverTimerExpired;

    MUSB_ASSERT(pControllerPrivateData);
    pController = (MGC_Controller *)pControllerPrivateData;    
    pPort = pController->pPort;
    MUSB_ASSERT(pPort);
    pParam = pPort->pDriverTimerData;

    if (pPort->pfDriverTimerExpired)
    {
        pfDriverTimerExpired = pPort->pfDriverTimerExpired;
        pPort->pfDriverTimerExpired = NULL;
        pPort->pDriverTimerData = NULL;
        pfDriverTimerExpired(pParam, pPort);
    }

    UNUSED(wTimerIndex);
}

/**
 * Arm a one-shot timer.
 * This is specifically intended for use by class drivers.
 *
 * @param hBus bus "handle"
 * @param bTimerIndex: 
 *        Timer Index 0 is reserved for EP0 device configuration. 
 *        Timer Index 1 is reserved for Hub class device configuration. 
 *        Timer Index 2 is reserved for Mass storage class driver.
 * @param dwTime timeout (milliseconds)
 * @param pParam parameter to expiration callback
 * @return 0 on success; error code on failure
 */
uint32_t MUSB_ArmTimer(MUSB_BusHandle hBus, 
                       uint8_t bTimerIndex, uint32_t dwTime,
                       MUSB_pfDriverTimerExpired pfDriverTimerExpired, void *pParam)
{
    uint8_t bOk;

    MGC_Port *pPort;
    MUSB_SystemServices *pServices;

    MUSB_ASSERT(hBus);
    pPort = (MGC_Port *)hBus;
    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pPort->pController);
    pServices = pPort->pController->pSystemServices;
    MUSB_ASSERT(pServices);    

    // Timer must be canceled.
    MUSB_ASSERT(pPort->pfDriverTimerExpired == NULL);

    pPort->pfDriverTimerExpired = pfDriverTimerExpired;
    pPort->pDriverTimerData = pParam;
    bOk = pServices->pfArmTimer(pServices->pPrivateData, bTimerIndex, dwTime, 
        FALSE, MGC_DriverTimerExpired, pPort->pController);

    return (uint32_t)(bOk ? 0 : MUSB_STATUS_NO_RESOURCES);
}

uint32_t MUSB_CancelTimer(MUSB_BusHandle hBus, uint8_t bTimerIndex)
{
    MGC_Port *pPort;
    MUSB_SystemServices *pServices;
    uint8_t bRet;

    MUSB_ASSERT(hBus);
    pPort = (MGC_Port *)hBus;
    MUSB_ASSERT(pPort->pController);
    MUSB_ASSERT(pPort->pController->pSystemServices);

    if (pPort->pfDriverTimerExpired)
    {
        pPort->pfDriverTimerExpired = NULL;
        pPort->pDriverTimerData = NULL;
        pServices = pPort->pController->pSystemServices;    

        MUSB_ASSERT(pServices->pfCancelTimer);
        
        bRet = pServices->pfCancelTimer(pServices->pPrivateData, bTimerIndex);
        UNUSED(bRet);
    }    
    
    return 0;
}

#ifdef MUSB_HUB            
/*
 * Use Port bus reset to recover device on hub do not ack properly.
 */
static uint8_t MUSB_SilentHubReset(MUSB_Device* pDevice)
{    
    uint32_t u4Ret;
    uint32_t u4MaxWaitCount = 0;
    MUSB_Hub *pHubDevice = NULL;    
    MUSB_HubPortStatus *pPortStatus = NULL;    

    MUSB_ASSERT(pDevice);
    MUSB_ASSERT(pDevice->pParentUsbDevice);

    //(1) Get hub device structure.
    u4Ret = MGC_HubFindHubDevice(pDevice->pParentUsbDevice, &pHubDevice);
    if (MUSB_SUCCESS != u4Ret)
    {
        return ((uint8_t)u4Ret);
    }

    MUSB_ASSERT((pDevice->bHubPort >= 1) && (pDevice->bHubPort <= MUSB_HUB_MAX_PORTS));
    // make sure this hub port is in idle state.
    if (((pDevice->bHubPort-1) >= (uint8_t)MUSB_HUB_MAX_PORTS) ||
        (pDevice->bHubPort < 1))
    {
        return (uint8_t)MUSB_FAILURE;
    }
    if ( (pHubDevice->aHubPortState[pDevice->bHubPort-1] != MUSB_HUB_PORT_STATE_DEFAULT) && 
         (pHubDevice->aHubPortState[pDevice->bHubPort-1] != MUSB_HUB_PORT_WAIT_FOR_ENUMERATE) )
    {
        LOG(1, "Silent Reset: HubPort-%d: error PortState = 0x%X not idle.\n", 
            pDevice->bHubPort, pHubDevice->aHubPortState[pDevice->bHubPort-1]);
        return (uint8_t)MUSB_STATUS_PEER_REMOVED;
    }
    
    //(2) Get hub port status, check if device disconnect from hub.
    LOG(1, "Silent Reset: HubPort-%d = 0x%08X, Get status.\n", 
        pDevice->bHubPort, (uint32_t)pDevice);
    
    MUSB_HubPrepareSetupPkt(&(pHubDevice->setupPacket), 
        (uint8_t)(MUSB_DIR_IN | MUSB_TYPE_CLASS | MUSB_RECIP_OTHER),
        MUSB_REQ_GET_STATUS, 
        (uint16_t)0,
        (uint16_t)pDevice->bHubPort, MUSB_HUB_PORT_STATUS_LENGTH);
            
    pHubDevice->ctrlIrp.dwStatus = 0xFF;
    pHubDevice->ctrlIrp.pfIrpComplete = NULL;
    pHubDevice->ctrlIrp.pOutBuffer = (uint8_t *)&(pHubDevice->setupPacket);
    pHubDevice->ctrlIrp.dwOutLength = 8;
    pHubDevice->ctrlIrp.pInBuffer = pHubDevice->aHubPortStatus;
    pHubDevice->ctrlIrp.dwInLength = MUSB_HUB_PORT_STATUS_LENGTH;
    u4Ret = MUSB_StartControlTransfer(pHubDevice->pUsbDevice->pPort,
                                         &(pHubDevice->ctrlIrp));
    if (MUSB_STATUS_OK != u4Ret)
    {
        LOG(1, "Silent Reset: HubPort-%d, Get port status-1 fail = 0x%X.\n", 
            pDevice->bHubPort, u4Ret);
        return ((uint8_t)u4Ret);
    }

    // wait control irp end.
    u4MaxWaitCount = 0;
    while (pHubDevice->ctrlIrp.dwStatus == 0xFF)
    {
        UNUSED(MUSB_Sleep(10));
        if (u4MaxWaitCount++ > 20)
        {
            return MGC_M_STATUS_ERROR;
        }
    }
    u4Ret = pHubDevice->ctrlIrp.dwStatus;
    if (MUSB_STATUS_OK != u4Ret)
    {
        LOG(1, "Silent Reset: HubPort-%d, Get port status-2 fail = 0x%X.\n", 
            pDevice->bHubPort, u4Ret);        
        return ((uint8_t)u4Ret);
    }

    pPortStatus = &(pHubDevice->portStatus);
    MGC_HubStoreStatus(pHubDevice->aHubPortStatus, pPortStatus);
    
    // Check hub port status. If device removed from hub, return device remove to user.
    if (((pPortStatus->wStatus & (uint16_t)MUSB_HUB_PORT_CONNECTION_BM) == 0) ||
         (pPortStatus->wStatusChange & (uint16_t)MUSB_HUB_C_PORT_CONNECTION_BM))
    {
        LOG(1, "Silent Reset: HubPort-%d, error wStatus = 0x%X, wStatusChange = 0x%X.\n", 
            pDevice->bHubPort, pPortStatus->wStatus, pPortStatus->wStatusChange);        
        return (uint8_t)MUSB_STATUS_PEER_REMOVED;
    }

    //(3)  Reset device on hub port.
    LOG(1, "Silent Reset: HubPort-%d = 0x%08X, Reset.\n", 
        pDevice->bHubPort, (uint32_t)pDevice);
    
    // set feature: reset device on hub.
    MUSB_HubPrepareSetupPkt(&(pHubDevice->setupPacket), 
        (uint8_t)(MUSB_TYPE_CLASS |MUSB_RECIP_OTHER),
        MUSB_REQ_SET_FEATURE, 
        (uint16_t)MUSB_HUB_PORT_RESET_FEATURE,
        (uint16_t)pDevice->bHubPort, 0);
        
    pHubDevice->ctrlIrp.dwStatus = 0xFF;
    pHubDevice->ctrlIrp.pfIrpComplete = NULL;
    pHubDevice->ctrlIrp.pOutBuffer = (uint8_t *)&(pHubDevice->setupPacket);
    pHubDevice->ctrlIrp.dwOutLength = 8;
    pHubDevice->ctrlIrp.pInBuffer = NULL;
    pHubDevice->ctrlIrp.dwInLength = MUSB_HUB_NO_DATA_PHASE;
    u4Ret = MUSB_StartControlTransfer(pHubDevice->pUsbDevice->pPort,
                                         &(pHubDevice->ctrlIrp));
    if (MUSB_STATUS_OK != u4Ret)
    {
        LOG(1, "Silent Reset: HubPort-%d, Reset-1 fail = 0x%X.\n", 
            pDevice->bHubPort, u4Ret);
        return ((uint8_t)u4Ret);
    }

    // wait control irp end.
    u4MaxWaitCount = 0;
    while (pHubDevice->ctrlIrp.dwStatus == 0xFF)
    {
        UNUSED(MUSB_Sleep(10));
        if (u4MaxWaitCount++ > 20)
        {
            return MGC_M_STATUS_ERROR;
        }
    }
    u4Ret = pHubDevice->ctrlIrp.dwStatus;
    if (MUSB_STATUS_OK != u4Ret)
    {
        LOG(1, "Silent Reset: HubPort-%d, Reset-2 fail = 0x%X.\n", 
            pDevice->bHubPort, u4Ret);        
        return ((uint8_t)u4Ret);
    }

    // wait hub to reset device.
    UNUSED(MUSB_Sleep(MUSB_BUS_RESET_TIME));

    LOG(1, "Silent Reset: HubPort-%d, Clear reset.\n", pDevice->bHubPort);

    //(4) Set feature: clear reset device on hub.
    MUSB_HubPrepareSetupPkt(&(pHubDevice->setupPacket), 
        (uint8_t)(MUSB_TYPE_CLASS |MUSB_RECIP_OTHER),
        MUSB_REQ_CLEAR_FEATURE, 
        (uint16_t)MUSB_HUB_C_PORT_RESET_FEATURE,
        (uint16_t)pDevice->bHubPort, 0);
        
    pHubDevice->ctrlIrp.dwStatus = 0xFF;
    pHubDevice->ctrlIrp.pfIrpComplete = NULL;
    pHubDevice->ctrlIrp.pOutBuffer = (uint8_t *)&(pHubDevice->setupPacket);
    pHubDevice->ctrlIrp.dwOutLength = 8;
    pHubDevice->ctrlIrp.pInBuffer = NULL;
    pHubDevice->ctrlIrp.dwInLength = MUSB_HUB_NO_DATA_PHASE;
    u4Ret = MUSB_StartControlTransfer(pHubDevice->pUsbDevice->pPort,
                                         &(pHubDevice->ctrlIrp));
    if (MUSB_STATUS_OK != u4Ret)
    {
        LOG(1, "Silent Reset: HubPort-%d, Clear reset-1 fail = 0x%X.\n", 
            pDevice->bHubPort, u4Ret);        
        return ((uint8_t)u4Ret);
    }

    // wait control irp end.
    u4MaxWaitCount = 0;
    while (pHubDevice->ctrlIrp.dwStatus == 0xFF)
    {
        UNUSED(MUSB_Sleep(10));
        if (u4MaxWaitCount++ > 20)
        {
            return (uint8_t)MGC_M_STATUS_ERROR;
        }
    }
    u4Ret = pHubDevice->ctrlIrp.dwStatus;
    if (MUSB_STATUS_OK != u4Ret)
    {
        LOG(1, "Silent Reset: HubPort-%d, Clear reset-2 fail = 0x%X.\n", 
            pDevice->bHubPort, u4Ret);        
        return ((uint8_t)u4Ret);
    }

    //(5) Get hub port status, check if device disconnect from hub.
    LOG(1, "Silent Reset: HubPort-%d = 0x%08X, Get status.\n", 
        pDevice->bHubPort, (uint32_t)pDevice);
    
    MUSB_HubPrepareSetupPkt(&(pHubDevice->setupPacket), 
        (uint8_t)(MUSB_DIR_IN | MUSB_TYPE_CLASS | MUSB_RECIP_OTHER),
        MUSB_REQ_GET_STATUS, 
        (uint16_t)0,
        (uint16_t)pDevice->bHubPort, MUSB_HUB_PORT_STATUS_LENGTH);
        
    pHubDevice->ctrlIrp.dwStatus = 0xFF;
    pHubDevice->ctrlIrp.pfIrpComplete = NULL;
    pHubDevice->ctrlIrp.pOutBuffer = (uint8_t *)&(pHubDevice->setupPacket);
    pHubDevice->ctrlIrp.dwOutLength = 8;
    pHubDevice->ctrlIrp.pInBuffer = pHubDevice->aHubPortStatus;
    pHubDevice->ctrlIrp.dwInLength = MUSB_HUB_PORT_STATUS_LENGTH;
    u4Ret = MUSB_StartControlTransfer(pHubDevice->pUsbDevice->pPort,
                                         &(pHubDevice->ctrlIrp));
    if (MUSB_STATUS_OK != u4Ret)
    {
        LOG(1, "Silent Reset: HubPort-%d, Get port status in Reset fail = 0x%X.\n", 
            pDevice->bHubPort, u4Ret);
        return ((uint8_t)u4Ret);
    }

    // wait control irp end.
    u4MaxWaitCount = 0;
    while (pHubDevice->ctrlIrp.dwStatus == 0xFF)
    {
        UNUSED(MUSB_Sleep(10));
        if (u4MaxWaitCount++ > 20)
        {
            return (uint8_t)MGC_M_STATUS_ERROR;
        }
    }
    u4Ret = pHubDevice->ctrlIrp.dwStatus;
    if (MUSB_STATUS_OK != u4Ret)
    {
        LOG(1, "Silent Reset: HubPort-%d, Get port status in Reset fail = 0x%X.\n", 
            pDevice->bHubPort, u4Ret);        
        return ((uint8_t)u4Ret);
    }

    pPortStatus = &(pHubDevice->portStatus);
    MGC_HubStoreStatus(pHubDevice->aHubPortStatus, pPortStatus);
    
    // Check hub port status. If device removed from hub, return device remove to user.
    if (((pPortStatus->wStatus & (uint16_t)MUSB_HUB_PORT_CONNECTION_BM) == 0) ||
         (pPortStatus->wStatusChange & (uint16_t)MUSB_HUB_C_PORT_CONNECTION_BM))
    {
        LOG(1, "Silent Reset: HubPort-%d, error wStatus = 0x%X, wStatusChange = 0x%X.\n", 
            pDevice->bHubPort, pPortStatus->wStatus, pPortStatus->wStatusChange);        
        return (uint8_t)MUSB_STATUS_PEER_REMOVED;
    }

    // dwHubPortStatusChange must be not change before enumerate device.
    pHubDevice->dwHubPortStatusChange &= ~(uint32_t)(1 << pDevice->bHubPort);

    return MUSB_STATUS_OK;
}
#endif
	
/*
 * Use bus reset to recover device do not ack properly.
 */
uint8_t MUSB_SilentReset(MUSB_Device* pDevice)
{    
    MUSB_Port *pUsbPort;
    MGC_Port *pPort;
    MGC_EnumerationData *pEnumData;
    MUSB_DeviceRequest *pRequest;
    uint32_t u4Ret;
    uint32_t u4MaxWaitCount = 0;

    MUSB_ASSERT(pDevice);
    pUsbPort = pDevice->pPort;
    MUSB_ASSERT(pUsbPort);
    pPort = (MGC_Port *)pUsbPort->pPrivateData;
    MUSB_ASSERT(pPort);
    pEnumData = &(pPort->EnumerationData);
    MUSB_ASSERT(pEnumData);
    pRequest = (MUSB_DeviceRequest *)pEnumData->aSetupTx;
    MUSB_ASSERT(pRequest);

    if (!pPort->bInsert)
    {
        LOG(0, "Silent Reset: Port-%d root device  remove.\n", pUsbPort->bUsbPort);
        return ((uint8_t)MUSB_STATUS_PEER_REMOVED);
    }

#ifdef MUSB_HUB
    if (pDevice->pParentUsbDevice)
    {
        u4Ret = MUSB_SilentHubReset(pDevice);
        if (MUSB_STATUS_OK != u4Ret)
        {
            return ((uint8_t)u4Ret);
        }
    }
    else
#endif    
    {
        // Reset root device.
        LOG(0, "Silent Reset: Root device = 0x%08X, Reset.\n", (uint32_t)pDevice);

        pPort->bWantReset = TRUE;
        pPort->bWantSuspend = FALSE;
        pPort->pfProgramBusState(pPort);

        // wait reset.
        UNUSED(MUSB_Sleep(MUSB_BUS_RESET_TIME));

        // disable bus reset.
        pPort->bWantReset = FALSE;
        pPort->pfProgramBusState(pPort);
    }
    
    LOG(0, "Silent Reset: HubPort-%d, Reset OK.\n", pDevice->bHubPort);

    MUSB_ASSERT(pDevice->bBusAddress);
    // set address
    LOG(0, "Silent Reset: HubPort-%d, Addr = %d.\n", 
        pDevice->bHubPort, pDevice->bBusAddress);
    MUSB_MemCopy(pEnumData->aSetupTx, 
        MGC_aSetDeviceAddress, 8);
    pRequest->wValue = MUSB_SWAP16(pDevice->bBusAddress);
    pEnumData->bAddress = pDevice->bBusAddress;
    pEnumData->pDevice = pDevice;    
    pDevice->bBusAddress = 0;
    
    pEnumData->Irp.pDevice = pDevice;
    pEnumData->Irp.pOutBuffer = pEnumData->aSetupTx;
    pEnumData->Irp.dwOutLength = 8;
    pEnumData->Irp.pInBuffer = NULL;
    pEnumData->Irp.dwInLength = 0;   
    pEnumData->Irp.dwStatus = 0xFF;
    pEnumData->Irp.dwActualOutLength = 0;
    pEnumData->Irp.dwActualInLength = 0;
    pEnumData->Irp.pfIrpComplete = NULL;
    pEnumData->Irp.pCompleteParam = NULL;    
    u4Ret = MUSB_StartControlTransfer(pPort->pInterfacePort, &(pEnumData->Irp));
    if (MUSB_STATUS_OK != u4Ret)
    {
        // Must restore device address before return error.
        pDevice->bBusAddress = pEnumData->bAddress;
        pEnumData->pDevice = NULL;
        LOG(0, "Silent Reset: HubPort-%d, Set addr-1 fail = 0x%X.\n", 
            pDevice->bHubPort, u4Ret);            
        return ((uint8_t)u4Ret);
    }
    
    // wait control irp end.
    u4MaxWaitCount = 0;
    while (pEnumData->Irp.dwStatus == 0xFF)
    {
        UNUSED(MUSB_Sleep(10));
        if (u4MaxWaitCount++ > 20)
        {
            // Must restore device address before return error.
            pDevice->bBusAddress = pEnumData->bAddress;
            pEnumData->pDevice = NULL;
            return MGC_M_STATUS_ERROR;
        }
    }
    u4Ret = pEnumData->Irp.dwStatus;
    if (MUSB_STATUS_OK != u4Ret)
    {
        // Must restore device address before return error.
        pDevice->bBusAddress = pEnumData->bAddress;
        pEnumData->pDevice = NULL;
        LOG(0, "Silent Reset: HubPort-%d, Set addr-2 fail = 0x%X.\n", 
            pDevice->bHubPort, u4Ret);            
        return ((uint8_t)u4Ret);
    }

    // restore device address.
    pDevice->bBusAddress = pEnumData->bAddress;
    pPort->bFuncAddr = pEnumData->bAddress;
    pPort->pfProgramBusState(pPort);
    LOG(0, "Silent Reset: HubPort-%d, Addr = %d OK.\n", 
        pDevice->bHubPort, pDevice->bBusAddress);

    // Get device descriptor.
    LOG(0, "Silent Reset: HubPort-%d, Get device descriptor.\n", pDevice->bHubPort);

    MUSB_MemCopy(pEnumData->aSetupTx, MGC_aGetDeviceDescriptor, 8);
    pRequest->wLength = MUSB_SWAP16(sizeof(MUSB_DeviceDescriptor));
    pEnumData->Irp.pDevice = pDevice;
    pEnumData->Irp.pOutBuffer = pEnumData->aSetupTx;
    pEnumData->Irp.dwOutLength = 8;
    pEnumData->Irp.pInBuffer = pEnumData->aSetupRx;
    pEnumData->Irp.dwInLength = sizeof(MUSB_DeviceDescriptor);
    pEnumData->Irp.dwStatus = 0xFF;
    pEnumData->Irp.dwActualOutLength = 0;
    pEnumData->Irp.dwActualInLength = 0;
    pEnumData->Irp.pfIrpComplete = NULL;
    pEnumData->Irp.pCompleteParam = NULL;       
    u4Ret = MUSB_StartControlTransfer(pPort->pInterfacePort, &(pEnumData->Irp));
    if (MUSB_STATUS_OK != u4Ret)
    {
        pEnumData->pDevice = NULL;   
        LOG(0, "Silent Reset: HubPort-%d, Get device descriptor-1 fail = 0x%X.\n", 
            pDevice->bHubPort, u4Ret);            
        return ((uint8_t)u4Ret);
    }
    
    // wait control irp end.
    u4MaxWaitCount = 0;
    while (pEnumData->Irp.dwStatus == 0xFF)
    {
        UNUSED(MUSB_Sleep(10));
        if (u4MaxWaitCount++ > 20)
        {
            pEnumData->pDevice = NULL;           
            return MGC_M_STATUS_ERROR;
        }
    }
    
    u4Ret = pEnumData->Irp.dwStatus;
    if (MUSB_STATUS_OK != u4Ret)
    {
        LOG(0, "Silent Reset: HubPort-%d, Get device descriptor-2 fail = 0x%X.\n", 
            pDevice->bHubPort, u4Ret);           
        pEnumData->pDevice = NULL;
        return ((uint8_t)u4Ret);
    }

    LOG(0, "Silent Reset: HubPort-%d, Get device descriptor OK.\n", pDevice->bHubPort);             

    // Get device configuration.
    LOG(0, "Silent Reset: HubPort-%d, Get configuration descriptor.\n", pDevice->bHubPort);
    MUSB_MemCopy(pEnumData->aSetupTx, 
        MGC_aGetConfigDescriptor, 8);
    pEnumData->bIndex = 0;
    pEnumData->aSetupTx[2] = pEnumData->bIndex;
    pRequest->wLength = MUSB_SWAP16((uint16_t)pEnumData->Irp.dwInLength);

    pEnumData->Irp.pDevice = pDevice;
    pEnumData->Irp.pOutBuffer = pEnumData->aSetupTx;
    pEnumData->Irp.dwOutLength = 8;
    pEnumData->Irp.pInBuffer = pEnumData->aSetupRx;
    pEnumData->Irp.dwInLength = 255;
    pEnumData->Irp.dwStatus = 0xFF;
    pEnumData->Irp.dwActualOutLength = 0;
    pEnumData->Irp.dwActualInLength = 0;
    pEnumData->Irp.pfIrpComplete = NULL;
    pEnumData->Irp.pCompleteParam = NULL;
    u4Ret = MUSB_StartControlTransfer(pPort->pInterfacePort, &(pEnumData->Irp));    
    if (MUSB_STATUS_OK != u4Ret)
    {
        LOG(0, "Silent Reset: HubPort-%d, Get configuration descriptor-1 fail = 0x%X.\n", 
            pDevice->bHubPort, u4Ret);
        pEnumData->pDevice = NULL;            
        return ((uint8_t)u4Ret);
    }
    
    // wait control irp end.
    u4MaxWaitCount = 0;
    while (pEnumData->Irp.dwStatus == 0xFF)
    {
        UNUSED(MUSB_Sleep(10));
        if (u4MaxWaitCount++ > 20)
        {
            pEnumData->pDevice = NULL;        
            return MGC_M_STATUS_ERROR;
        }
    }
    u4Ret = pEnumData->Irp.dwStatus;
    if (MUSB_STATUS_OK != u4Ret)
    {
        LOG(0, "Silent Reset: HubPort-%d, Get configuration descriptor-2 fail = 0x%X.\n", 
            pDevice->bHubPort, u4Ret);   
        pEnumData->pDevice = NULL;            
        return ((uint8_t)u4Ret);
    }

    LOG(0, "Silent Reset: HubPort-%d, Get configuration descriptor OK.\n", pDevice->bHubPort);

    // Set device configuration.         
    LOG(0, "Silent Reset: HubPort-%d, Set configuration value = %d.\n", 
        pDevice->bHubPort, pDevice->pCurrentConfiguration->bConfigurationValue);

    MUSB_MemCopy(pEnumData->aSetupTx, MGC_aSetConfigDescriptor, 8);
    // use previous config value.
    pEnumData->aSetupTx[2] = pDevice->pCurrentConfiguration->bConfigurationValue;
    pEnumData->Irp.pDevice = pDevice;
    pEnumData->Irp.pOutBuffer = pEnumData->aSetupTx;
    pEnumData->Irp.dwOutLength = 8;
    pEnumData->Irp.pInBuffer = NULL;
    pEnumData->Irp.dwInLength = 0;
    pEnumData->Irp.dwStatus = 0xFF;
    pEnumData->Irp.dwActualOutLength = 0;
    pEnumData->Irp.dwActualInLength = 0;
    pEnumData->Irp.pfIrpComplete = NULL;
    pEnumData->Irp.pCompleteParam = NULL;    
    u4Ret = MUSB_StartControlTransfer(pPort->pInterfacePort, &(pEnumData->Irp));    
    if (MUSB_STATUS_OK != u4Ret)
    {
        LOG(0, "Silent Reset: HubPort-%d, Set configuration-1 fail = 0x%X.\n", 
            pDevice->bHubPort, u4Ret);      
        pEnumData->pDevice = NULL;            
        return ((uint8_t)u4Ret);
    }
    
    // wait control irp end.
    u4MaxWaitCount = 0;
    while (pEnumData->Irp.dwStatus == 0xFF)
    {
        UNUSED(MUSB_Sleep(10));
        if (u4MaxWaitCount++ > 20)
        {
            pEnumData->pDevice = NULL;    
            return MGC_M_STATUS_ERROR;
        }
    }
    u4Ret = pEnumData->Irp.dwStatus;
    if (MUSB_STATUS_OK != u4Ret)
    {
        LOG(0, "Silent Reset: HubPort-%d, Set configuration-2 fail = 0x%X.\n", 
            pDevice->bHubPort, u4Ret);
        pEnumData->pDevice = NULL;            
        return ((uint8_t)u4Ret);
    }

    pEnumData->pDevice = NULL;
    LOG(0, "Silent Reset: HubPort-%d, Set configuration OK.\n", pDevice->bHubPort);

    return MUSB_STATUS_OK;
}

//  MTK Notice: Max Liao, 2006/10/26.
//  add virtual Pid for device to test embedded host compilance test.	
uint32_t MUSB_SetEhset(uint32_t dwEnable, uint16_t wPid)
{
#if defined(MUSB_EHSET) || !defined(MUSB_CODE_SHRINK)
    // clear test mode register.

    if ((wPid >= 0x0101) && (wPid <= 0x0108))
    {
        if (wPid == 0x0105)
        {
            MGC_dwEL35Reset = !MGC_dwEL35Reset;
            LOG(0, "Test EL35 chirp timing = %d.\n", MGC_dwEL35Reset);
        }
        MGC_bTestMode = dwEnable;

        MGC_wTestModePid = wPid;
        return MUSB_STATUS_OK;
    }
    else
    {
        MGC_bTestMode = 0;

        MGC_wTestModePid = 0;
        return MGC_M_STATUS_ERROR;
    }
#else
    return MGC_M_STATUS_ERROR;
#endif
    
}
#endif
