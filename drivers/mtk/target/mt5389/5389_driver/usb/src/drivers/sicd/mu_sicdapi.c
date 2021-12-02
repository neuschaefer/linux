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
 * Top-level SICD class driver module
 * This file contains interface and callback functions to upper and lower layer.
 * $Revision: #1 $
 */
#ifdef MUSB_SICD

#include "mu_bits.h"
#include "mu_descs.h"
#include "mu_diag.h"
#include "mu_mem.h"
#include "mu_stdio.h"
#include "mu_impl.h"

#include "mu_hfi_sicd_if.h"
#include "mu_sicdstate.h"
#include "mu_sicdapi.h"
#include "mu_sicdutil.h"


/*************************** CONSTANTS ****************************/
#define SICD_MODS_INDEX (0xEE)
#define SICD_MODS_LENGTH (40)

#define SICD_LIST_SIZE (7)

#define SICD_QUERY_GAP_TIME   (1000)

#define SICD_DEVICE_INFO_SIZE   (700)

#define SICD_CHECK_GET_PARTIAL_OBJ

#define MGC_GetMsOsDesVenCode(x) (x[16])

/******************************* FORWARDS ********************************/
static void MGC_VolumeInit(const MUSB_SicdDevice *pSicdDevice, 
    MUSB_SicdVolume *pVolume);

static void MGC_SicdCheckDevice(MGC_Port *pPort, const MUSB_Device *pDevice);

static void MGC_SicdTimerExpired(void *pPrivateData, MUSB_BusHandle hBus);

// callback function
static void MGC_SicdSetConfigCallback(void *pContext, 
    MUSB_ControlIrp *pControlIrp);

static void MGC_SicdGetMsOsDesCallback(void *pContext, 
    MUSB_ControlIrp *pControlIrp);

static void MGC_SicdGetMsOsExtDesCallback(void *pContext, 
    MUSB_ControlIrp *pControlIrp);

static void MGC_SicdQuery(MUSB_SicdProtocol *pSicdProtocol);

static MUSB_HfiStatus MGC_SicdRead(MUSB_SicdProtocol* pSicdProtocol, 
    const MUSB_SICD_IO_DATA_T* pt_data);

static MUSB_HfiStatus MGC_SicdWrite(MUSB_SicdProtocol* pSicdProtocol, 
    const MUSB_SICD_IO_DATA_T* pt_data);

// to upper layer
static MUSB_HfiStatus MUSB_SicdGetDeviceInfo(void *pDeviceData, 
    MUSB_HfiDeviceInfo **ppDeviceInfo);

static MUSB_HfiStatus MUSB_SicdReadDevice(void *pDeviceData, 
    MUSB_SICD_IO_DATA_T* pt_data);

static MUSB_HfiStatus MUSB_SicdWriteDevice(void *pDeviceData, 
    MUSB_SICD_IO_DATA_T* pt_data);

/** Connect handler for SICD class driver */
static uint8_t MUSB_SicdConnect(void *pPrivateData, 
                                 MUSB_BusHandle     hBus,
                                 MUSB_Device       *pUsbDevice,
                                 const uint8_t     *pPeripheralList);

/** Disconnect Handler for SICD Device Driver */
static uint8_t MUSB_SicdDisconnect (void *pPrivateData, 
                                MUSB_BusHandle  hBus,
                                MUSB_Device    *pUsbDevice);

/******************************** GLOBALS ********************************/
static const uint8_t MUSB_SicdPeripheralList [MUSB_SICD_LIST_NUM][SICD_LIST_SIZE] =
{
    // (1) Official SICD class code = (0x00, 0x06).
    {
    (uint8_t)MUSB_TARGET_CLASS, MUSB_CLASS_PER_INTERFACE, 
    (uint8_t)MUSB_TARGET_INTERFACE, 0,
    (uint8_t)MUSB_TARGET_CLASS, MUSB_CLASS_SICD, 
    (uint8_t)MUSB_TARGET_ACCEPT },
    // (2) Non official SICD class code = (0x06, 0x06).
    {
    (uint8_t)MUSB_TARGET_CLASS, MUSB_CLASS_SICD, 
    (uint8_t)MUSB_TARGET_INTERFACE, 0,
    (uint8_t)MUSB_TARGET_CLASS, MUSB_CLASS_SICD, 
    (uint8_t)MUSB_TARGET_ACCEPT },
    // (3) Non official SICD class code = (0xFF, 0x06).
    {
    (uint8_t)MUSB_TARGET_CLASS, MUSB_CLASS_VENDOR_SPEC, 
    (uint8_t)MUSB_TARGET_INTERFACE, 0,
    (uint8_t)MUSB_TARGET_CLASS, MUSB_CLASS_SICD, 
    (uint8_t)MUSB_TARGET_ACCEPT },
    // (4) Non official SICD class code = (0x06, 0xFF).
    {
    (uint8_t)MUSB_TARGET_CLASS, MUSB_CLASS_SICD, 
    (uint8_t)MUSB_TARGET_INTERFACE, 0,
    (uint8_t)MUSB_TARGET_CLASS, MUSB_CLASS_VENDOR_SPEC, 
    (uint8_t)MUSB_TARGET_ACCEPT },
    // (5) Non official SICD class code = (0x06, 0x00).
    {
    (uint8_t)MUSB_TARGET_CLASS, MUSB_CLASS_SICD, 
    (uint8_t)MUSB_TARGET_INTERFACE, 0,
    (uint8_t)MUSB_TARGET_CLASS, 0x00, 
    (uint8_t)MUSB_TARGET_ACCEPT },
    // (6) Non official SICD class code = (0x00, 0x00)
    {
    (uint8_t)MUSB_TARGET_CLASS, MUSB_CLASS_PER_INTERFACE, 
    (uint8_t)MUSB_TARGET_INTERFACE, 0,
    (uint8_t)MUSB_TARGET_CLASS, 0x0, 
    (uint8_t)MUSB_TARGET_ACCEPT },
    // (7) Non official SICD class code = (0xFF, 0xFF)
    {
    (uint8_t)MUSB_TARGET_CLASS, MUSB_CLASS_VENDOR_SPEC, 
    (uint8_t)MUSB_TARGET_INTERFACE, 0,
    (uint8_t)MUSB_TARGET_CLASS, MUSB_CLASS_VENDOR_SPEC, 
    (uint8_t)MUSB_TARGET_ACCEPT },
    // (8) Non official SICD class code = (0xFF, 0x00)
    {
    (uint8_t)MUSB_TARGET_CLASS, MUSB_CLASS_VENDOR_SPEC, 
    (uint8_t)MUSB_TARGET_INTERFACE, 0,
    (uint8_t)MUSB_TARGET_CLASS, 0x00, 
    (uint8_t)MUSB_TARGET_ACCEPT },
    // (9) Non official SICD class code = (0x00, 0xFF)
    {
    (uint8_t)MUSB_TARGET_CLASS, MUSB_CLASS_PER_INTERFACE, 
    (uint8_t)MUSB_TARGET_INTERFACE, 0,
    (uint8_t)MUSB_TARGET_CLASS, MUSB_CLASS_VENDOR_SPEC, 
    (uint8_t)MUSB_TARGET_ACCEPT }    
};

/** SICD device driver */
static MUSB_DeviceDriver MUSB_SicdDeviceDriver =
{
    NULL,
    MUSB_SicdConnect,
    MUSB_SicdDisconnect, 
    NULL, 
    NULL,
};

/** For getting Microsoft OS Descriptor */
static const uint8_t MUSB_aGetMsOsDescriptor [] =
{
    (uint8_t)MUSB_DIR_IN | (uint8_t)MUSB_TYPE_STANDARD | (uint8_t)MUSB_RECIP_DEVICE, 
    MUSB_REQ_GET_DESCRIPTOR,
    SICD_MODS_INDEX, MUSB_DT_STRING,               // descriptor type, descriptor index
    0, 0,                                                   // page index, interface number(run time fill)
    0x12, 0                                                 // length
};

static const uint8_t MUSB_aGetMsCompatibleDeviceDescriptor [] =
{
    (uint8_t)MUSB_DIR_IN | (uint8_t)MUSB_TYPE_VENDOR | (uint8_t)MUSB_RECIP_DEVICE, 
    0,  // vendor request, vendor code(run time fill)
    0, 0,                                                   // descriptor type, descriptor index
    0x04, 0,                                                // page index, interface number
    SICD_MODS_LENGTH, 0                            // length
};

/******************************** Internal FUNCTIONS ******************************/

/*******************************************************************************
**  MGC_VolumeInit
**
**  descriptions:
**      Volume initialization
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static void MGC_VolumeInit(const MUSB_SicdDevice *pSicdDevice, MUSB_SicdVolume *pVolume)
{
    MUSB_HfiSicdDevice *HfiDevice;
    MUSB_HfiDeviceInfo *HfiDeviceInfo;
    MUSB_ASSERT(pVolume);
    MUSB_ASSERT(pSicdDevice);
    
    HfiDevice = &(pVolume->Device);
    HfiDevice->pPrivateData = pVolume;
    HfiDevice->pfGetMediumInfo = NULL;
    HfiDevice->pfGetDeviceInfo = MUSB_SicdGetDeviceInfo;
    HfiDevice->pfReadDevice = MUSB_SicdReadDevice;
    HfiDevice->pfWriteDevice = MUSB_SicdWriteDevice;
    HfiDevice->pfHfiCheckMedium = NULL;

    HfiDeviceInfo = &(pVolume->DeviceInfo);
    HfiDeviceInfo->wVendorId = pSicdDevice->pUsbDevice->DeviceDescriptor.idVendor;
    HfiDeviceInfo->wProductId = pSicdDevice->pUsbDevice->DeviceDescriptor.idProduct;
    HfiDeviceInfo->bcdDevice = pSicdDevice->pUsbDevice->DeviceDescriptor.bcdDevice;
    HfiDeviceInfo->bBusAddress = pSicdDevice->pUsbDevice->bBusAddress;
    HfiDeviceInfo->bLunCount = 1;    // for MTP/PTP device, LUN is always 1.
    HfiDeviceInfo->pDiskVendor = NULL;
    HfiDeviceInfo->pDiskProduct = NULL;
    HfiDeviceInfo->pDiskRevision = NULL;

    pVolume->bAnnounced = FALSE;
    pVolume->pSicdProtocol = pSicdDevice->pProtocol;    
}
/*******************************************************************************
**  MGC_VolumeInit
**
**  descriptions:
**      Volume initialization
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static void MGC_SicdCheckDevice(MGC_Port *pPort, const MUSB_Device *pDevice)
{
    MUSB_Device *pUsbDevice = NULL;
    uint16_t wCount = 0;
    uint16_t wIndex = 0;

    MUSB_ASSERT(pPort);
    
    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));

    // Check if any device is not handled by MGC_SicdQuery().    
    for (wIndex=0; wIndex < wCount; wIndex++)
    {
        pUsbDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), wIndex);

        if ((pUsbDevice) &&        
            (pUsbDevice != pDevice) &&        
            (pUsbDevice->bSupport == MUSB_STATUS_CHILD_UNKNOW) &&        
            (pUsbDevice->bDeviceClass == MUSB_CLASS_SICD) &&
            (pUsbDevice->pDriverPrivateData))
        {        
            //Timer Index MUSB_SICD_TIMER_IDX is reserved for SICD class driver.
            (void)MUSB_ArmTimer((MUSB_BusHandle)pPort, 
                MUSB_SICD_TIMER_IDX /*bTimerIndex*/, 
                SICD_QUERY_GAP_TIME /*dwTime*/, 
                MGC_SicdTimerExpired, pUsbDevice);        
            break;
        }
    }

}
/*******************************************************************************
**  MGC_CreateSicdInstance
**
**  descriptions:
**      Open in/out/interrupt pipes, set sicd protocol structure initial value.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static MUSB_SicdProtocol* MGC_CreateSicdInstance(MUSB_Device *pDevice,
                                                   MUSB_BusHandle hBus,
                                                   const MUSB_InterfaceDescriptor *pIfaceDesc,
                                                   const MUSB_EndpointDescriptor *pInEnd,
                                                   const MUSB_EndpointDescriptor *pOutEnd,
                                                   const MUSB_EndpointDescriptor *pIntrEnd)
{
    MUSB_SicdProtocol *pSicdProtocol;   
    MUSB_DeviceEndpoint InEnd;
    MUSB_DeviceEndpoint OutEnd;
    MUSB_DeviceEndpoint IntrEnd;
    MUSB_Pipe hInPipe;
    MUSB_Pipe hOutPipe;
    #if MGC_SICD_OPEN_INTERRUPT_PIPE
    MUSB_Pipe hIntrPipe;
    #endif

    MUSB_ASSERT(pDevice);
    MUSB_ASSERT(hBus);
    MUSB_ASSERT(pIfaceDesc);
    MUSB_ASSERT(pInEnd);
    MUSB_ASSERT(pOutEnd);
    MUSB_ASSERT(pIntrEnd);
    
    InEnd.pDevice = pDevice;
    OutEnd.pDevice = pDevice;
    IntrEnd.pDevice = pDevice;

    switch (pDevice->ConnectionSpeed)
    {
    case MUSB_CONNECTION_SPEED_HIGH:
        InEnd.wNakLimit = 16;
        OutEnd.wNakLimit = 16;
        break;

    case MUSB_CONNECTION_SPEED_UNKNOWN:
    case MUSB_CONNECTION_SPEED_LOW:
    case MUSB_CONNECTION_SPEED_FULL:
    default:
        InEnd.wNakLimit = 255;
        OutEnd.wNakLimit = 255;
    }

    (void)MUSB_MemCopy(&(InEnd.UsbDescriptor), pInEnd, sizeof(MUSB_EndpointDescriptor));
    (void)MUSB_MemCopy(&(OutEnd.UsbDescriptor), pOutEnd, sizeof(MUSB_EndpointDescriptor));
    (void)MUSB_MemCopy(&(IntrEnd.UsbDescriptor), pIntrEnd, sizeof(MUSB_EndpointDescriptor));

    // If we have requested set configuration at enumeration, it means pipe has
    // been opened. If not, we open pipe here.
    hInPipe = MUSB_OpenPipe(hBus, &InEnd, NULL);
    if (!hInPipe)
    {
        LOG(1, "SICD: failed to open IN pipe\n", NULL);
        return NULL;
    }

    hOutPipe = MUSB_OpenPipe(hBus, &OutEnd, NULL);
    if (!hOutPipe)
    {
        LOG(1, "SICD: failed to open OUT pipe\n", NULL);
        MUSB_ClosePipe(hInPipe);
        return NULL;
    }

    #if MGC_SICD_OPEN_INTERRUPT_PIPE
    hIntrPipe = MUSB_OpenPipe(hBus, &IntrEnd, NULL);
    if (!hIntrPipe)
    {
        LOG(1, "SICD: failed to open INTERRUPT pipe\n", NULL);
        MUSB_ClosePipe(hInPipe);
        MUSB_ClosePipe(hOutPipe);
        return NULL;
    }
    #endif
    
    pSicdProtocol = (MUSB_SicdProtocol *)MUSB_MemAlloc(sizeof(MUSB_SicdProtocol));    
    if ( pSicdProtocol )
    {
        MUSB_MemSet((void *)pSicdProtocol, 0, sizeof(MUSB_SicdProtocol));
    
        pSicdProtocol->pDevice = pDevice;
        pSicdProtocol->hInPipe = hInPipe;
        pSicdProtocol->hOutPipe = hOutPipe;
        #if MGC_SICD_OPEN_INTERRUPT_PIPE
        pSicdProtocol->hIntrPipe = hIntrPipe;
        #endif
        
        return pSicdProtocol;
    }
    
    LOG(1, "SICD: failed to allocate instance data\n", NULL);
    MUSB_ClosePipe(hInPipe);
    MUSB_ClosePipe(hOutPipe);
    #if MGC_SICD_OPEN_INTERRUPT_PIPE
    MUSB_ClosePipe(hIntrPipe);
    #endif
    
    return NULL;
}

/*******************************************************************************
**  MGC_DestroySicdInstance
**
**  descriptions:
**      Close in/out/interrupt pipe, notify Hfi device removed.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static void MGC_DestroySicdInstance(const MUSB_SicdProtocol *pProtocol)
{
    MUSB_SicdVolume *pVolume;
    MUSB_SicdProtocol *pSicdProtocol;
    MUSB_Device *pDevice = NULL;
    MUSB_Port *pUsbPort;    
    MGC_Port *pPort;
    
    MUSB_ASSERT(pProtocol);
    pSicdProtocol = (MUSB_SicdProtocol *)pProtocol;

    pDevice = pSicdProtocol->pDevice;
    MUSB_ASSERT(pDevice);
    pUsbPort = pDevice->pPort;
    MUSB_ASSERT(pUsbPort);
    pPort = (MGC_Port *)pUsbPort->pPrivateData;
    MUSB_ASSERT(pPort);
  
    //Timer Index MUSB_SICD_TIMER_IDX is reserved for SICD class driver.
    /* kill timer */
    (void)MUSB_CancelTimer((MUSB_BusHandle)pPort, 
        MUSB_SICD_TIMER_IDX /*bTimerIndex*/);    

    MGC_SicdCheckDevice(pPort, pDevice);

    MUSB_ClosePipe(pSicdProtocol->hInPipe);
    pSicdProtocol->hInPipe = NULL;    
    MUSB_ClosePipe(pSicdProtocol->hOutPipe);
    pSicdProtocol->hOutPipe = NULL;
    
    #if MGC_SICD_OPEN_INTERRUPT_PIPE
    (void)MUSB_CancelTransfer(&(pSicdProtocol->SicdStateControl.IntrIrp));
    MUSB_ClosePipe(pSicdProtocol->hIntrPipe);
    pSicdProtocol->hIntrPipe = NULL;
    #endif

    if (pSicdProtocol->pSicdDevInfo)
    {
        MUSB_MemFree(pSicdProtocol->pSicdDevInfo);
    }

    pVolume = pSicdProtocol->pVolume;
    if (pVolume)
    {
        pVolume->bAnnounced = FALSE;
        if (pVolume->hVolume)
        {
            MUSB_HfiSicdMediumRemoved(pVolume->hVolume);
            MUSB_HfiSicdDeviceRemoved(pVolume->hVolume);
        }
        MUSB_MemFree(pVolume);
    }  

    MUSB_MemFree(pSicdProtocol);
}

/*******************************************************************************
**  MGC_SicdDeviceInit
**
**  descriptions:
**      Allocate per-device data.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static MUSB_SicdDevice *MGC_SicdDeviceInit(void)
{
    MUSB_SicdDevice *pSicdDevice;

    pSicdDevice = (MUSB_SicdDevice *)MUSB_MemAlloc(sizeof(MUSB_SicdDevice));

    if (NULL == pSicdDevice)
    {
        LOG(1, "SICD Error: Insufficient memory\n");
        return (NULL);
    }

    (void)MUSB_MemSet((void *)pSicdDevice, 0, sizeof(MUSB_SicdDevice));

    pSicdDevice->pDriver = &MUSB_SicdDeviceDriver;

    LOG(5, "SICD: pSicdDevice: 0x%08X.\n", (uint32_t)pSicdDevice);

    return pSicdDevice;
}


/*******************************************************************************
**  MGC_CheckValidMsOsDesStr
**
**  descriptions:
**      Validate MS OS Descriptor by comparing string.
**  parameters:
**      pbMsOsDesStr - The Microsoft OS Descriptor String
**  result:
**      TRUE: It's valid MS OS Descriptor
**      FALSe:It's not valid MS OS Descriptor
**  return:
**  note:
*******************************************************************************/
static BOOL MGC_CheckValidMsOsDesStr(const uint8_t* pbMsOsDesStr)
{
    uint8_t i;
    const uint8_t pbMicroSoftOsDes[14] = {0x4D, 0x00, 0x53, 0x00, 0x46, 0x00, 0x54, 0x00, 
                                    0x31, 0x00, 0x30, 0x00, 0x30, 0x00, }; //"MSFT100" in unicode
    MUSB_ASSERT(pbMsOsDesStr);
    for ( i = 0 ; i < 14 ; i++ )
    {
        if ( pbMicroSoftOsDes[i] != pbMsOsDesStr[i+2] )
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*******************************************************************************
**  MGC_CheckMsOsExtDesType
**
**  descriptions:
**      Parse the Microsoft OS Extended Descriptor String. 
**  parameters:
**      pbMsOsDesStr - The Microsoft OS Extended Descriptor String
**  result:
**      Device type of the extended descriptor
**  return:
**  note:
*******************************************************************************/
static MUSB_MS_OS_DES_DEV_TYPE MGC_CheckMsOsExtDesType(const uint8_t* pbMsOsExtDesStr)
{
    uint8_t i;
    uint8_t pbMtpStr[12] = {(uint8_t)'M', (uint8_t)'T', (uint8_t)'P', 0x00, 
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t pbPtpStr[12] = {(uint8_t)'P', (uint8_t)'T', (uint8_t)'P', 0x00, 
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    MUSB_ASSERT(pbMsOsExtDesStr);
    
    for ( i = 0 ; i < 12 ; i++ )
    {
        if ( pbMtpStr[i] != pbMsOsExtDesStr[i+18] )
        {
            break;
        }
    }
    if ( i == 12 )
    {
        return MUSB_MS_OS_DES_MTP;
    }

    for ( i = 0 ; i < 12 ; i++ )
    {
        if ( pbPtpStr[i] != pbMsOsExtDesStr[i+18] )
        {
            break;
        }
    }
    if ( i == 12 )
    {
        return MUSB_MS_OS_DES_PTP;
    }

    return MUSB_MS_OS_UNKNOWN;
}

/*******************************************************************************
**  MGC_SicdConfigureDevice
**
**  descriptions:
**      Set device configuration.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static uint32_t MGC_SicdConfigureDevice(MUSB_SicdDevice *pSicdDevice)
{
    MUSB_DeviceRequest *pSetup;

    MUSB_ControlIrp *pControlIrp;
    uint32_t dwStatus;

    MUSB_ASSERT(pSicdDevice);
    
    pSetup = &(pSicdDevice->Setup);
    pControlIrp = &(pSicdDevice->ControlIrp);

    /* Set the Current Configuration Descriptor to Default as Set Config is Success */
    pSicdDevice->pUsbDevice->pCurrentConfiguration = 
        pSicdDevice->pUsbDevice->apConfigDescriptors[0];

    /** Prepare the Setup Packet for sending Set Config Request */
    pSetup->bmRequestType = 
        (uint8_t) (MUSB_DIR_OUT | MUSB_TYPE_STANDARD | MUSB_RECIP_DEVICE);
    pSetup->bRequest = (uint8_t) MUSB_REQ_SET_CONFIGURATION;
    pSetup->wValue = 
        (uint16_t) pSicdDevice->pUsbDevice->pCurrentConfiguration->bConfigurationValue;
    pSetup->wIndex = (uint16_t) 0;
    pSetup->wLength = (uint16_t) 0;

    /** Fill Control Irp */
    pControlIrp->pDevice           = pSicdDevice->pUsbDevice;
    pControlIrp->pOutBuffer        = (const uint8_t*)pSetup;
    pControlIrp->dwOutLength       = sizeof(MUSB_DeviceRequest);
    pControlIrp->pInBuffer         = NULL;
    pControlIrp->dwInLength        = 0;
    pControlIrp->dwStatus          = 0;
    pControlIrp->dwActualOutLength = 0;
    pControlIrp->dwActualInLength  = 0;
    pControlIrp->pfIrpComplete     = MGC_SicdSetConfigCallback;
    pControlIrp->pCompleteParam    = (void *) pSicdDevice;

    dwStatus = MUSB_StartControlTransfer(pSicdDevice->pUsbDevice->pPort, pControlIrp);

    if (dwStatus)
    {
        LOG(1, "SICD Error: Set Configuration Request failed, dwStatus: %d\n",dwStatus);
    }

    return (dwStatus);
}

/*******************************************************************************
**  MGC_GetMsOsDes
**
**  descriptions:
**      Get Microsoft OS Descriptor. (This is first step to check whether the 
**      device supports MS OS descriptor or not.)
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static uint32_t MGC_GetMsOsDes(MUSB_SicdDevice *pSicdDevice)
{
    uint32_t dwStatus;
    MUSB_DeviceRequest *pSetup;
    MUSB_ControlIrp *pControlIrp;

    MUSB_ASSERT(pSicdDevice);

    pSetup = (MUSB_DeviceRequest*)&(pSicdDevice->Setup);
    pControlIrp = (MUSB_ControlIrp*)&(pSicdDevice->ControlIrp);

    /* start get ms os descriptor */
    (void)MUSB_MemCopy(pSetup, MUSB_aGetMsOsDescriptor, sizeof(MUSB_aGetMsOsDescriptor));
    (void)MUSB_MemSet(pSicdDevice->aMODS, 0, sizeof(pSicdDevice->aMODS));
    pSetup->wIndex = pSicdDevice->bInterfaceNumber;
    pControlIrp->pDevice = pSicdDevice->pUsbDevice;
    pControlIrp->pOutBuffer = (const uint8_t*)pSetup;
    pControlIrp->dwOutLength = 8;
    pControlIrp->pInBuffer = pSicdDevice->aMODS;
    pControlIrp->dwInLength = sizeof(pSicdDevice->aMODS);
    pControlIrp->pfIrpComplete = MGC_SicdGetMsOsDesCallback;
    pControlIrp->pCompleteParam = (void*)pSicdDevice;
    dwStatus = MUSB_StartControlTransfer(pSicdDevice->pUsbDevice->pPort, pControlIrp);

    return dwStatus;
}

/*******************************************************************************
**  MGC_GetMsOsExtDes
**
**  descriptions:
**      Get Microsoft OS Extended Descriptor. By getting this descriptor, we can
**      know the device is MTP device or PTP device. 
**      (Only when the device supports the Ms Os Descriptor we will enter here)
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static uint32_t MGC_GetMsOsExtDes(MUSB_SicdDevice *pSicdDevice)
{
    uint32_t dwStatus;
    MUSB_DeviceRequest *pSetup;
    MUSB_ControlIrp *pControlIrp;

    MUSB_ASSERT(pSicdDevice);

    pSetup = (MUSB_DeviceRequest*)&(pSicdDevice->Setup);
    pControlIrp = (MUSB_ControlIrp*)&(pSicdDevice->ControlIrp);

    /* start get ms os extended descriptor */
    (void)MUSB_MemCopy(pSetup, MUSB_aGetMsCompatibleDeviceDescriptor, 8);
    (void)MUSB_MemSet(pSicdDevice->aMODS, 0, sizeof(pSicdDevice->aMODS));
    pSetup->bRequest = pSicdDevice->bVendorCode;
    pControlIrp->pDevice = pSicdDevice->pUsbDevice;
    pControlIrp->pOutBuffer = (const uint8_t*)pSetup;
    pControlIrp->dwOutLength = 8;
    pControlIrp->pInBuffer = pSicdDevice->aMODS;
    pControlIrp->dwInLength = sizeof(pSicdDevice->aMODS);
    pControlIrp->pfIrpComplete = MGC_SicdGetMsOsExtDesCallback;
    pControlIrp->pCompleteParam = (void*)pSicdDevice;
    dwStatus = MUSB_StartControlTransfer(pSicdDevice->pUsbDevice->pPort, pControlIrp);

    return dwStatus;    
}

/*******************************************************************************
**  MGC_SicdSetReady
**
**  descriptions:
**      Notify Hfi that the device is connected and initialize state machine.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static void MGC_SicdSetReady(const MUSB_SicdProtocol *pSicdProtocol)
{
    MUSB_HfiStatus Status;
    MUSB_SicdVolume *pVolume;
    
    MUSB_ASSERT(pSicdProtocol);
    
    pVolume = pSicdProtocol->pVolume;

    if (!pVolume->bAnnounced)
    {           
        Status = MUSB_HfiSicdAddDevice(&(pVolume->hVolume), 
            &(pVolume->DeviceInfo), &(pVolume->Device));
        if (MUSB_HFI_SUCCESS != Status)
        {
            LOG(1, "SICD: HfiSicdAddDevice fail = %d.\n", Status);
            return;
        }
        /* ready and volume not yet announced */
        pVolume->bAnnounced = TRUE;
        
        MUSB_HfiSicdMediumInserted(pVolume->hVolume);

        #if MGC_SICD_OPEN_INTERRUPT_PIPE
        (void)MUSB_SicdStartInterruptPipeListening((MUSB_SicdProtocol *)pSicdProtocol);
        #endif
    }
}

/*******************************************************************************
**  MGC_SicdTimerExpired
**
**  descriptions:
**      Handle device to  be ready after set configuration.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static void MGC_SicdTimerExpired(void *pPrivateData, MUSB_BusHandle hBus)
{
    MUSB_Device *pUsbDevice = NULL;
    MUSB_Device *pDevice = NULL;
    MGC_Port* pPort;
    MUSB_SicdDevice *pSicdDevice;
    MUSB_SicdProtocol *pSicdProtocol;
    uint16_t wCount;
    uint16_t wIndex;
    
    MUSB_ASSERT(pPrivateData);
    MUSB_ASSERT(hBus);
    
    pUsbDevice = (MUSB_Device *)pPrivateData;
    pPort = (MGC_Port *)hBus;

    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));

    for (wIndex=0; wIndex < wCount; wIndex++)
    {
        pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), wIndex);
        
        if (pUsbDevice == pDevice)
        {        
            pSicdDevice = (MUSB_SicdDevice *)pUsbDevice->pDriverPrivateData;
            MUSB_ASSERT(pSicdDevice);
            pSicdProtocol = pSicdDevice->pProtocol;
            MUSB_ASSERT(pSicdProtocol);
            
            MGC_SicdQuery(pSicdProtocol);
            break;
        }
    }
}

/*******************************************************************************
**  MGC_SicdSetConfigCallback
**
**  descriptions:
**      This function will be invoked after setting conficuration. We will now
**      try to get Microsoft OS descriptor to check it's MPT device or PTP device.
**      The descriptor getting procedure can be divided into two steps. Step 1 is
**      to check whether the device supports MS OS descriptor or not. Step 2 is 
**      to get MS OS extended descriptor string.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static void MGC_SicdSetConfigCallback(void *pContext, 
    MUSB_ControlIrp *pControlIrp)
{
    MUSB_SicdDevice *pSicdDevice;
    MUSB_SicdVolume *pVolume = NULL;
    MGC_EnumerationData *pEnumData;
    MGC_Port* pPort;
    MUSB_Device *pUsbDevice;
   
    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pControlIrp);

    pSicdDevice = (MUSB_SicdDevice *)pContext;
    pUsbDevice = pSicdDevice->pUsbDevice;
    MUSB_ASSERT(pUsbDevice);
    pPort = (MGC_Port *)pSicdDevice->hBus;
    MUSB_ASSERT(pPort);
    pEnumData = &(pPort->EnumerationData);

    if ( pControlIrp->dwStatus == MUSB_STATUS_OK )
    {
        // Assign device class code = SICD.
        // Some device class code = 0xFF or 0x00, but they are MTP device.
        pUsbDevice->bDeviceClass = MUSB_CLASS_SICD;
        
        pSicdDevice->pProtocol = MGC_CreateSicdInstance(pUsbDevice, 
                            pSicdDevice->hBus, pSicdDevice->pIface, 
                            pSicdDevice->pInEnd, pSicdDevice->pOutEnd, pSicdDevice->pIntrEnd);
        if (pSicdDevice->pProtocol)
        {           
            pSicdDevice->pProtocol->pSicdDevInfo = 
                (uint8_t *)MUSB_MemAlloc(SICD_DEVICE_INFO_SIZE);
            
            pVolume = (MUSB_SicdVolume *)MUSB_MemAlloc(sizeof(MUSB_SicdVolume));

            if ((pSicdDevice->pProtocol->pSicdDevInfo) && (pVolume))
            {
                MUSB_MemSet((void *)pSicdDevice->pProtocol->pSicdDevInfo, 
                    0, SICD_DEVICE_INFO_SIZE);

                MUSB_MemSet((void *)pVolume, 0, sizeof(MUSB_SicdVolume));
                
                pSicdDevice->pProtocol->pVolume = pVolume;

                MGC_VolumeInit(pSicdDevice, pVolume);
                
                LOG(5, "SICD: Configure 0x%08X Successfully.\n", (uint32_t)pUsbDevice);

                //Timer Index MUSB_SICD_TIMER_IDX is reserved for SICD class driver.
                /* kill timer */
                (void)MUSB_CancelTimer((MUSB_BusHandle)pPort, 
                    MUSB_SICD_TIMER_IDX /*bTimerIndex*/);    

                MGC_SicdCheckDevice(pPort, NULL);                
                return;                
            }
        }
    }

    LOG(1, "SICD: Device Configured Fail, 0x%X, 0x%X, 0x%X.\n", 
        pControlIrp->dwStatus, (uint32_t)pSicdDevice->pProtocol, (uint32_t)pVolume);

    // Report device to user's call back function.            
    pUsbDevice->bSupport = MUSB_STATUS_CHILD_CONNECT_UNSUPPORT;
    if (pEnumData->pfChildDeviceUpdate)
    {
        pEnumData->pfChildDeviceUpdate(pUsbDevice->pParentUsbDevice,
            pUsbDevice, MUSB_STATUS_CHILD_CONNECT_UNSUPPORT);
    }
}

/*******************************************************************************
**  MGC_SicdGetMsOsDesCallback
**
**  descriptions:
**      This function will be invoked after step 1 of MS OS descriptor getting
**      procedure. We check if the device supports MS OS descriptor, we will then
**      get MS OS extended descriptor string.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static void MGC_SicdGetMsOsDesCallback(void *pContext, 
    MUSB_ControlIrp *pControlIrp)
{
    MUSB_SicdDevice *pSicdDevice;
    MGC_EnumerationData *pEnumData;
    MGC_Port* pPort;
    MUSB_Device *pUsbDevice;
    uint8_t bSupportMODs = FALSE;
    uint32_t dwStatus;

    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pControlIrp);
    
    pSicdDevice = (MUSB_SicdDevice *)pContext;

    if ((pControlIrp->dwStatus == MUSB_STATUS_OK) &&
        MGC_CheckValidMsOsDesStr(pSicdDevice->ControlIrp.pInBuffer))
    {
        bSupportMODs = TRUE;
        
        // Get Vendor Code
        pSicdDevice->bVendorCode = 
            MGC_GetMsOsDesVenCode(pSicdDevice->ControlIrp.pInBuffer);

        // Get Microsoft OS Extended Descriptor.
        dwStatus = MGC_GetMsOsExtDes(pSicdDevice);
        if (MUSB_STATUS_OK != dwStatus)
        {
            bSupportMODs = FALSE;
            LOG(0, "SICD: Get MsOsExtDes fail = 0x%X.\n", dwStatus);
        }
    }
    else
    {
        LOG(0, "SICD: Not support MsOsDes = 0x%X.\n", pControlIrp->dwStatus);
    }

    pUsbDevice = pSicdDevice->pUsbDevice;
    MUSB_ASSERT(pUsbDevice);
    pPort = (MGC_Port *)pSicdDevice->hBus;
    MUSB_ASSERT(pPort);

    // SICD device, but not support Microsoft OS descriptor.
    if ((!bSupportMODs) && (pUsbDevice->bDeviceClass == MUSB_CLASS_SICD))
    {
        // SICD device, but not support Microsoft OS descriptor.
        pSicdDevice->bDeviceType = (uint8_t)MUSB_MS_OS_DES_PTP;

        LOG(0,"SICD: Not support MsOsDes, but PTP class.\n", NULL);

        if (MUSB_STATUS_OK == MGC_SicdConfigureDevice(pSicdDevice))
        {
            bSupportMODs = TRUE;
        }
    }

    if (!bSupportMODs)
    {
        LOG(0, "SICD: Unsupported device class = 0x%X.\n", pUsbDevice->bDeviceClass);
    
        // Report device to user's call back function.
        pUsbDevice->bSupport = MUSB_STATUS_CHILD_CONNECT_UNSUPPORT;
        pEnumData = &(pPort->EnumerationData);
        if (pEnumData->pfChildDeviceUpdate)
        {
            pEnumData->pfChildDeviceUpdate(pUsbDevice->pParentUsbDevice,
                pUsbDevice, MUSB_STATUS_CHILD_CONNECT_UNSUPPORT);
        }                          
    }
}

/*******************************************************************************
**  MGC_SicdGetMsOsExtDesCallback
**
**  descriptions:
**      This function will be invoked after step 2 of MS OS descriptor getting
**      procedure. With the pControlIrp, we can know the device is MTP device or
**      PTP device.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static void MGC_SicdGetMsOsExtDesCallback(void *pContext, MUSB_ControlIrp *pControlIrp)
{
    MUSB_SicdDevice *pSicdDevice;
    MGC_EnumerationData *pEnumData;
    MGC_Port* pPort;
    MUSB_Device *pUsbDevice;
    uint8_t bSupportExtDes = FALSE;
    uint8_t bDeviceType;

    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pControlIrp);
    pSicdDevice = (MUSB_SicdDevice *)pContext;
    pUsbDevice = pSicdDevice->pUsbDevice;
    MUSB_ASSERT(pUsbDevice);

    if (pControlIrp->dwStatus == MUSB_STATUS_OK )
    {
        bDeviceType = (uint8_t)MGC_CheckMsOsExtDesType(pControlIrp->pInBuffer);
        pSicdDevice->bDeviceType = bDeviceType;

        LOG(0, "SICD: bDeviceType = %d.\n", bDeviceType);
        
        if ( (bDeviceType == (uint8_t)MUSB_MS_OS_DES_MTP) || 
              (bDeviceType == (uint8_t)MUSB_MS_OS_DES_PTP) )
        {
            if (MUSB_STATUS_OK == MGC_SicdConfigureDevice(pSicdDevice))
            {
                bSupportExtDes = TRUE;
            }
        }
    }

    pPort = (MGC_Port *)pSicdDevice->hBus;
    MUSB_ASSERT(pPort);

    // SICD device, but not support Microsoft OS Extended descriptor.
    if ((!bSupportExtDes) && (pUsbDevice->bDeviceClass == MUSB_CLASS_SICD))
    {
        // SICD device, but not support Microsoft OS descriptor.
        pSicdDevice->bDeviceType = (uint8_t)MUSB_MS_OS_DES_PTP;

        LOG(0,"SICD: Not support MsOsExtDes, but PTP class.\n", NULL);

        if (MUSB_STATUS_OK == MGC_SicdConfigureDevice(pSicdDevice))
        {
            bSupportExtDes = TRUE;
        }
    }

    if (!bSupportExtDes)
    {
        // Not SICD device.           
        // Report device to user's call back function.
        pUsbDevice->bSupport = MUSB_STATUS_CHILD_CONNECT_UNSUPPORT;

        pEnumData = &(pPort->EnumerationData);
        if (pEnumData->pfChildDeviceUpdate)
        {
            pEnumData->pfChildDeviceUpdate(pUsbDevice->pParentUsbDevice,
                pUsbDevice, MUSB_STATUS_CHILD_CONNECT_UNSUPPORT);
        }              
    }    
}

/*******************************************************************************
**  MGC_SicdDeviceInfo
**
**  descriptions:
**      .
**  parameters:
**      pSicdProtocol: 
**  return:
**  note:
*******************************************************************************/
static void MGC_SicdDeviceInfo(MUSB_SicdProtocol *pSicdProtocol)
{
    uint32_t i;
    uint32_t StrLength;
    uint32_t ArrayNumElements;
    uint32_t ArrayOffset;
    UINT16 OpCode;
    uint8_t OpsH;
    uint8_t OpsL;
    uint8_t *pPayload;
    MUSB_SicdVolume *pVolume;
    MUSB_HfiDeviceInfo *HfiDeviceInfo;
    
    MUSB_ASSERT(pSicdProtocol);

    pVolume = pSicdProtocol->pVolume;    
    MUSB_ASSERT(pVolume);
    HfiDeviceInfo = &(pVolume->DeviceInfo);    
    MUSB_ASSERT(HfiDeviceInfo);
    
    pPayload = pSicdProtocol->pSicdDevInfo;
    // VendorExtensionDesc string length.
    StrLength = pPayload[8] *2;
    // OperationsSupported array NumElements offset.
    ArrayOffset = 8 + 1 + StrLength + 2;
    if (SICD_DEVICE_INFO_SIZE <= ArrayOffset)
    {
        return;
    }
    // array number is uint32_t.
    ArrayNumElements = MUSB_SWAP32P(&pPayload[ArrayOffset]);

    // check Get partial object capability.
    for (i=0; i<ArrayNumElements; i++)
    {
        if (SICD_DEVICE_INFO_SIZE <= ((ArrayOffset + 4)+(i*2)+1))
        {
            return;
        }
        OpsL = pPayload[(ArrayOffset + 4)+(i*2)];
        OpsH = pPayload[(ArrayOffset + 4)+(i*2)+1];
        OpCode = (UINT16)((OpsH * 256) + OpsL);

        if (MUSB_SICD_GET_PARTIAL_OBJ  == OpCode)
        {
            pSicdProtocol->bIsGetPartialObj = TRUE;
            break;
        }
    }

    LOG(0, "SICD: %s GetPartialObject.\n", 
        ((pSicdProtocol->bIsGetPartialObj) ? "Support" : "Not support"));  

    // Skip OperationsSupported array.
    ArrayOffset += ((ArrayNumElements*2) + 4);
    if (SICD_DEVICE_INFO_SIZE <= ArrayOffset)
    {
        return;
    }    
    // EventsSupported array number.
    ArrayNumElements = MUSB_SWAP32P(&pPayload[ArrayOffset]);

    // Skip EventsSupported array.
    ArrayOffset += ((ArrayNumElements*2) + 4);
    if (SICD_DEVICE_INFO_SIZE <= ArrayOffset)
    {
        return;
    }        
    // DevicePropertiesSupported array number.
    ArrayNumElements = MUSB_SWAP32P(&pPayload[ArrayOffset]);
    
    // Skip DevicePropertiesSupported array.
    ArrayOffset += ((ArrayNumElements*2) + 4);
    if (SICD_DEVICE_INFO_SIZE <= ArrayOffset)
    {
        return;
    }        
    // CaptureFormats array number.
    ArrayNumElements = MUSB_SWAP32P(&pPayload[ArrayOffset]);

    // Skip CaptureFormats array.
    ArrayOffset += ((ArrayNumElements*2) + 4);
    if (SICD_DEVICE_INFO_SIZE <= ArrayOffset)
    {
        return;
    }        
    // ImageFormats array number.
    ArrayNumElements = MUSB_SWAP32P(&pPayload[ArrayOffset]);

    // Skip ImageFormats array.
    ArrayOffset += ((ArrayNumElements*2) + 4);
    if (SICD_DEVICE_INFO_SIZE <= ArrayOffset)
    {
        return;
    }        
    // Manufacturer string length.
    StrLength = pPayload[ArrayOffset];
    for (i=1 ; i<StrLength ; i++)
    {
        if (SICD_DEVICE_INFO_SIZE <= ((ArrayOffset + 1) + (i*2)))
        {
            return;
        }            
        // transform UNICODE to Ansi string.
        pPayload[(ArrayOffset + 1) + i] = pPayload[(ArrayOffset + 1) + (i*2)];
    }
    // Show Manufacturer string.
    if (StrLength > 0)
    {
        HfiDeviceInfo->pDiskVendor = &pPayload[(ArrayOffset + 1)];
        LOG(5, "SICD: Vendor: %s.\n", &pPayload[(ArrayOffset + 1)]); 
    }
    
    // Model string length.
    ArrayOffset += ((StrLength*2) + 1);
    StrLength = pPayload[ArrayOffset];
    for (i=1 ; i<StrLength ; i++)
    {
        if (SICD_DEVICE_INFO_SIZE <= ((ArrayOffset + 1) + (i*2)))
        {
            return;
        }            
        // transform UNICODE to Ansi string.
        pPayload[(ArrayOffset + 1) + i] = pPayload[(ArrayOffset + 1) + (i*2)];
    }
    // Show Model string.
    if (StrLength > 0)
    {
        HfiDeviceInfo->pDiskProduct = &pPayload[(ArrayOffset + 1)];    
        LOG(5, "SICD: Model: %s.\n", &pPayload[(ArrayOffset + 1)]); 
    }

    // DeviceVersion string length.
    ArrayOffset += ((StrLength*2) + 1);
    StrLength = pPayload[ArrayOffset];
    for (i=1 ; i<StrLength ; i++)
    {
        if (SICD_DEVICE_INFO_SIZE <= ((ArrayOffset + 1) + (i*2)))
        {
            return;
        }            
        // transform UNICODE to Ansi string.
        pPayload[(ArrayOffset + 1) + i] = pPayload[(ArrayOffset + 1) + (i*2)];
    }
    // Show DeviceVersion string.
    if (StrLength > 0)
    {
        HfiDeviceInfo->pDiskRevision = &pPayload[(ArrayOffset + 1)];        
        LOG(5, "SICD: Version: %s.\n", &pPayload[(ArrayOffset + 1)]); 
    }

    // SerialNumber string length.
    ArrayOffset += ((StrLength*2) + 1);
    StrLength = pPayload[ArrayOffset];
    for (i=1 ; i<StrLength ; i++)
    {
        if (SICD_DEVICE_INFO_SIZE <= ((ArrayOffset + 1) + (i*2)))
        {
            return;
        }            
        // transform UNICODE to Ansi string.
        pPayload[(ArrayOffset + 1) + i] = pPayload[(ArrayOffset + 1) + (i*2)];
    }
    // Show SerialNumber string.
    if (StrLength > 0)
    {
        LOG(5, "SICD: Serial: %s.\n", &pPayload[(ArrayOffset + 1)]); 
    }
}
/*******************************************************************************
**  MGC_SicdComplete
**
**  descriptions:
**      We use this function for sending MTP/PTP operations.
**  parameters:
**      pSicdProtocol: 
**      pt_data: MTP/PTP operation parameters.
**  result:
**  return:
**      Operation sending result. (not operation final result)
**  note:
*******************************************************************************/
static void MGC_SicdComplete(void *pvHfiNfyTag, int32_t e_nfy_cond, void *pv_data)
{
    MUSB_SicdProtocol *pSicdProtocol;
    MUSB_Device *pDevice;
    MUSB_Port *pUsbPort;
    MGC_Port *pPort;
    MGC_EnumerationData *pEnumData;
    uint8_t bIsQueryEnd = FALSE;
    //uint16_t u2ResponseCode;

    MUSB_ASSERT(pvHfiNfyTag);
    pSicdProtocol = (MUSB_SicdProtocol *)pvHfiNfyTag;
    pDevice = pSicdProtocol->pDevice;
    MUSB_ASSERT(pDevice);
    pUsbPort = pDevice->pPort;
    MUSB_ASSERT(pUsbPort);
    pPort = (MGC_Port *)pUsbPort->pPrivateData;
    MUSB_ASSERT(pPort);
        
    // handle response phase only.
    if (e_nfy_cond == (int32_t)MUSB_HFI_NFY_PHASE_RESP_COMPLETE)
    {
        switch (pSicdProtocol->eSicdInitState)
        {
        case MUSB_SICD_INIT_OPEN_SESSION:
            LOG(7, "SICD: Open session ok.\n", NULL);
            // If there is other device need to be checked.
            MGC_SicdCheckDevice(pPort, NULL);
            break;
            
        case MUSB_SICD_INIT_DEVINFO:
            LOG(7, "SICD: Get device info ok.\n", NULL);
            MGC_SicdDeviceInfo(pSicdProtocol);
            // If there is other device need to be checked.
            MGC_SicdCheckDevice(pPort, NULL);
            break;

        case MUSB_SICD_INIT_CLOSE_SESSION:
            LOG(7, "SICD: Close session ok.\n", NULL);
            MGC_SicdCheckDevice(pPort, pDevice);
            bIsQueryEnd = TRUE;            
            break;
                      
        default: 
            break;
        }   

        if (bIsQueryEnd)
        {           
            // Report device to user's call back function.  
#ifdef SICD_CHECK_GET_PARTIAL_OBJ
            pDevice->bSupport = (uint8_t)((pSicdProtocol->bIsGetPartialObj) ?
                MUSB_STATUS_CHILD_CONNECT : 
                MUSB_STATUS_CHILD_CONNECT_UNSUPPORT);
#else
            pDevice->bSupport = MUSB_STATUS_CHILD_CONNECT;
#endif

            pEnumData = &(pPort->EnumerationData);
            if (pEnumData->pfChildDeviceUpdate)
            {
                pEnumData->pfChildDeviceUpdate(pDevice->pParentUsbDevice,
                    pDevice, pDevice->bSupport);
            }
            
            // set sicd ready
            if (pDevice->bSupport == MUSB_STATUS_CHILD_CONNECT)
            {
                MGC_SicdSetReady(pSicdProtocol);
            }            
        }
    }

    UNUSED(pv_data);
    return;
}

/*******************************************************************************
**  MGC_SicdQuery
**
**  descriptions:
**      We use this function for sending MTP/PTP operations.
**  parameters:
**      pSicdProtocol: 
**      pt_data: MTP/PTP operation parameters.
**  result:
**  return:
**      Operation sending result. (not operation final result)
**  note:
*******************************************************************************/
static void MGC_SicdQuery(MUSB_SicdProtocol *pSicdProtocol)
{
    MUSB_NFY_INFO_T *pSicdNfyInfo;   /* user's notify function. */
    MUSB_SICD_IO_DATA_T *pSicdData;
    MUSB_HfiStatus eStatus;
    
    MUSB_ASSERT(pSicdProtocol);

    pSicdData = &pSicdProtocol->SicdData;

    MUSB_MemSet((void *)pSicdData, 0, sizeof(MUSB_SICD_IO_DATA_T));
    
    switch (pSicdProtocol->eSicdInitState)        
    {
        case MUSB_SICD_INIT_UNKNOW:
            MUSB_ASSERT(pSicdProtocol->pDevice);
            LOG(5, "SICD: Device:0x%08X, HubPort-%d.\n", 
                (uint32_t)pSicdProtocol->pDevice, pSicdProtocol->pDevice->bHubPort);
            
            // Init SICD state variable.
            (void)MUSB_SicdStateInit(pSicdProtocol);

            // Init SICD call back function.
            pSicdNfyInfo = &pSicdProtocol->SicdNfyInfo;
            pSicdNfyInfo->pfHfiNfy = MGC_SicdComplete;
            pSicdNfyInfo->pvHfiNfyTag = pSicdProtocol;    

            // open session.
            pSicdData->u2OperationCode = MUSB_SICD_OPEN_SESSION;
            pSicdData->u2ParamNum = 1;
            // The TransactionID used for the OpenSession operation shall be 0x00000000.
            pSicdData->u4TransactionID = 0x00000000;
            // SessionIDs are assigned by the Initiator as a parameter to the
            // OpenSession operation, and must be non-zero.
            pSicdData->u4Param[0] = 0x00000001;
            pSicdData->dwDataBufLen = 0;
            pSicdData->pbDataBuf = NULL;
            pSicdData->eDataDirection = MUSB_SICD_NO_DATA;
            pSicdProtocol->eSicdInitState = MUSB_SICD_INIT_OPEN_SESSION;
            break;

        case MUSB_SICD_INIT_OPEN_SESSION:
            MUSB_ASSERT(pSicdProtocol->pSicdDevInfo);                
            // Get device info.
            pSicdData->u2OperationCode = MUSB_SICD_GET_DEVICE_INFO;
            pSicdData->u2ParamNum = 0;
            pSicdData->u4TransactionID = 0x00000001;
            pSicdData->dwDataBufLen = SICD_DEVICE_INFO_SIZE;
            pSicdData->pbDataBuf = pSicdProtocol->pSicdDevInfo;
            pSicdData->eDataDirection = MUSB_SICD_DATA_IN;
            pSicdProtocol->eSicdInitState = MUSB_SICD_INIT_DEVINFO;
            break;

        case MUSB_SICD_INIT_DEVINFO:
            // Close session.
            pSicdData->u2OperationCode = MUSB_SICD_CLOSE_SESSION;
            pSicdData->u2ParamNum = 0;
            pSicdData->u4TransactionID = 0x00000002;
            pSicdData->dwDataBufLen = 0;
            pSicdData->pbDataBuf = NULL;
            pSicdData->eDataDirection = MUSB_SICD_NO_DATA;
            pSicdProtocol->eSicdInitState = MUSB_SICD_INIT_CLOSE_SESSION;
            break;
            
        default:
            return;
    }
            
    eStatus = MGC_SicdRead(pSicdProtocol, pSicdData);    
    if (eStatus != MUSB_HFI_SUCCESS)
    {
        LOG(1, "SICD: Query Fail, %d.\n", eStatus);
    }
}

/*******************************************************************************
**  MGC_SicdRead
**
**  descriptions:
**      We use this function for sending MTP/PTP operations.
**  parameters:
**      pSicdProtocol: 
**      pt_data: MTP/PTP operation parameters.
**  result:
**  return:
**      Operation sending result. (not operation final result)
**  note:
*******************************************************************************/
static MUSB_HfiStatus MGC_SicdRead(MUSB_SicdProtocol* pSicdProtocol, 
    const MUSB_SICD_IO_DATA_T* pt_data)
{
    MUSB_SicdStateControl* pSicdStateControl;
    MUSB_SicdCommand* pSicdCommand;
    MUSB_SICD_BLOCK_T* pCommandBlock;
    MUSB_SICD_BLOCK_T* pDataBlock;
    MUSB_SICD_BLOCK_T* pResponseBlock;
    MUSB_SicdResult Result;

    if ((!pSicdProtocol) || (!pt_data))
    {
        return MUSB_HFI_ERROR_PARAMETER;
    }

    // Only 8520 platform need to check.
    if ( (pt_data->pbDataBuf != 0) && ((((uint32_t)pt_data->pbDataBuf)%4) != 0) )
    {
        LOG(1, "pbDataBuf not 4 bytes aligned!\n");
        return MUSB_HFI_ERROR_VERIFY;
    }

    pSicdStateControl = &(pSicdProtocol->SicdStateControl);
    if (pSicdStateControl->eDriverState != MUSB_SICD_DRIVER_STATE_READY)
    {
        LOG(1, "SICD: Drv eDriverState = %d.\n", pSicdStateControl->eDriverState);
        return MUSB_HFI_ERROR_VERIFY;
    }

    pSicdStateControl->eDriverState = MUSB_SICD_DRIVER_STATE_RUNNING;

    // fill command, data, response block
    pSicdCommand = &(pSicdStateControl->SicdCommand);
    pCommandBlock = &(pSicdCommand->rCommandBlock);
    pDataBlock = &(pSicdCommand->rDataBlock);
    pResponseBlock = &(pSicdCommand->rResponseBlock);
    
    pCommandBlock->eDataType = MUSB_SICD_CMD_BLOCK;
    pCommandBlock->eDataDirection = MUSB_SICD_DATA_OUT;
    pCommandBlock->wOperationCode = pt_data->u2OperationCode;
    pCommandBlock->dwPayLoadBufLen = (pt_data->u2ParamNum)* 4;
    pCommandBlock->dwTranscationID = pt_data->u4TransactionID;
    pCommandBlock->pbPayLoadBuf = 
        pSicdStateControl->SicdCommand.abGenericTransferBuffer;

    (void)MUSB_MemSet(pCommandBlock->pbPayLoadBuf, 
        0, MUSB_SICD_MAX_CMD_RESPONSE_LENGTH);

    pCommandBlock->dwPayLoadBufLen = (pCommandBlock->dwPayLoadBufLen > 20) ? 
        20: pCommandBlock->dwPayLoadBufLen;
    (void)MUSB_MemCopy((pCommandBlock->pbPayLoadBuf+
        MUSB_SICD_HEADER_MINIMUM_LENGTH), 
        pt_data->u4Param, pCommandBlock->dwPayLoadBufLen);

    pDataBlock->eDataType = MUSB_SICD_DATA_BLOCK;
    pDataBlock->eDataDirection = pt_data->eDataDirection;
    pDataBlock->wOperationCode = pt_data->u2OperationCode;
    pDataBlock->dwPayLoadBufLen = pt_data->dwDataBufLen;
    pDataBlock->dwTranscationID = pt_data->u4TransactionID;
    pDataBlock->pbPayLoadBuf = pt_data->pbDataBuf;

    if (pCommandBlock->wOperationCode == 0x101B)
    {
        LOG(5, "GPO: H=0x%X, O=%d, S=%d.\n", 
            pt_data->u4Param[0], pt_data->u4Param[1], pt_data->u4Param[2]);
    }

    if ( pDataBlock->eDataDirection == MUSB_SICD_DATA_OUT )
    {
        pSicdStateControl->dwDataPhaseTotalDataSize = pt_data->dwTotalDataSize;
    }
    else
    {
        pSicdStateControl->dwDataPhaseTotalDataSize = MUSB_SICD_DATA_DEFAULT_SIZE;
    }
        
    // Backup buf size. (used only when SICD state controlled by SICD not by MW)
    // Since we change buf size to 512 when receiving first data phase packet, 
    // we should recover buf size & buf pointer when SICD state controlled by SICD. 
    // When SICD state controlled by MW, buf size/pointer will be updated by MW.
    // (Note! we do above only when buffer size is larger than 512 bytes.)
    pSicdCommand->pbBackupPtr = pt_data->pbDataBuf;
    pSicdCommand->dwBackupSize = pt_data->dwDataBufLen;
    
    pSicdCommand->bContinueDataPhaseWithoutCbk = FALSE;

    pResponseBlock->eDataType = MUSB_SICD_RESPONSE_BLOCK;
    pResponseBlock->eDataDirection = MUSB_SICD_DATA_IN;
    pResponseBlock->wOperationCode = pt_data->u2OperationCode;   // update this field later
    pResponseBlock->dwPayLoadBufLen = 0;                          // update this field later
    pResponseBlock->dwTranscationID = pt_data->u4TransactionID;
    pResponseBlock->pbPayLoadBuf = pSicdStateControl->SicdCommand.abGenericTransferBuffer;

    Result = MUSB_SicdCreateSendOpeartionFlow(pSicdProtocol);
    if (Result != MUSB_SICD_SUCCESS )
    {
        pSicdStateControl->eDriverState = MUSB_SICD_DRIVER_STATE_READY;
        LOG(1, "SICD: Error Create State Machine\n", Result);
        return MUSB_HFI_ERROR_TRANSFER;
    }
    
    Result = MUSB_SicdHandleOperation(pSicdProtocol);
    if ( Result != MUSB_SICD_SUCCESS )
    {
        pSicdStateControl->eDriverState = MUSB_SICD_DRIVER_STATE_READY;
        LOG(1, "SICD: Can't Handle Op\n", Result);
        return MUSB_HFI_ERROR_TRANSFER;
    }
        
    return MUSB_HFI_SUCCESS;
}

/*******************************************************************************
**  MGC_SicdWrite
**
**  descriptions:
**      We use this function for sending SICD class-specific requests.
**  parameters:
**      pSicdProtocol: 
**      pt_data: MTP/PTP operation parameters.
**  result:
**  return:
**      Operation sending result. (not operation final result)
**  note:
*******************************************************************************/
static MUSB_HfiStatus MGC_SicdWrite(MUSB_SicdProtocol* pSicdProtocol, 
    const MUSB_SICD_IO_DATA_T* pt_data)
{    
    uint32_t dwIrqStatus = 0;
    MUSB_SicdResult Result;
    MUSB_SICD_BLOCK_T* pDataBlock;
    MUSB_Device* pDevice;
    MUSB_Port* pPort;
    MGC_Port* pPortImpl;
    MUSB_SystemServices *pServices;

    if ((!pSicdProtocol) || (!pt_data))
    {
        return MUSB_HFI_ERROR_PARAMETER;
    }
    
    switch ( pt_data->u2OperationCode )
    {
    case MUSB_SICD_GET_EXT_EVENT_DATA_REQUEST : 
        Result = MUSB_SicdGetExtEventData(pSicdProtocol);
        if ( Result == (MUSB_SicdResult)MUSB_E_SICD_DEVICE_STATE_ERROR )
        {
            LOG(1, "MUSB_SicdWriteDevice(MUSB_SICD_RESET_REQUEST) failed.\n");
            return MUSB_HFI_ERROR_BUSY;
        }
        else if ( Result != MUSB_SICD_SUCCESS )
        {
            LOG(1, "MUSB_SicdWriteDevice(MUSB_SICD_RESET_REQUEST) failed.\n");
            return MUSB_HFI_ERROR_TRANSFER;
        }
        
        break;
    
    case MUSB_SICD_RESET_REQUEST : 
        if ( MUSB_SicdDeviceReset(pSicdProtocol) != MUSB_SICD_SUCCESS )
        {
            LOG(1, "MUSB_SicdWriteDevice(MUSB_SICD_RESET_REQUEST) failed.\n");
            return MUSB_HFI_ERROR_TRANSFER;
        }
        break;
    
    case MUSB_SICD_ABORT_REQUEST : 
        pDevice= pSicdProtocol->pDevice;
        MUSB_ASSERT(pDevice);
        pPort = pDevice->pPort;
        MUSB_ASSERT(pPort);
        pPortImpl = pPort->pPrivateData;
        MUSB_ASSERT(pPortImpl);
        MUSB_ASSERT(pPortImpl->pController);
        pServices = pPortImpl->pController->pSystemServices;
        MUSB_ASSERT(pServices);    

        dwIrqStatus = pServices->pfIrqLock(pServices->pPrivateData);      
        Result = MUSB_SicdOperationAbort(pSicdProtocol);
        pServices->pfIrqUnlock(pServices->pPrivateData, dwIrqStatus);                
        
        if ( Result == (MUSB_SicdResult)MUSB_E_SICD_DEVICE_STATE_ERROR )
        {
            LOG(1, "MUSB_SicdWriteDevice(MUSB_SICD_ABORT_REQUEST) failed.\n");
            return MUSB_HFI_ERROR_BUSY;
        }
        else if ( Result != MUSB_SICD_SUCCESS )
        {
            LOG(1, "MUSB_SicdWriteDevice(MUSB_SICD_ABORT_REQUEST) failed.\n");
            return MUSB_HFI_ERROR_TRANSFER;
        }
        break;

    case MUSB_SICD_DATAPHASE_CTRL_BY_MW : 
        (void)MUSB_SicdSetDataPhaseControlledByMiddleware(pSicdProtocol, (BOOL)TRUE);
        break;

    case MUSB_SICD_DATAPHASE_CTRL_BY_SICD : 
        (void)MUSB_SicdSetDataPhaseControlledByMiddleware(pSicdProtocol, (BOOL)FALSE);
        break;

    case MUSB_SICD_GET_NEXT_DATA :
        pDataBlock = &(pSicdProtocol->SicdStateControl.SicdCommand.rDataBlock);
        pDataBlock->dwPayLoadBufLen = pt_data->dwDataBufLen;
        pDataBlock->dwTranscationID = pt_data->u4TransactionID;
        pDataBlock->pbPayLoadBuf = pt_data->pbDataBuf;
        Result = MUSB_SicdGetNextData(pSicdProtocol);

        if ( Result == (MUSB_SicdResult)MUSB_E_SICD_DEVICE_STATE_ERROR )
        {
            LOG(1, "MUSB_SicdWriteDevice(MUSB_SICD_GET_NEXT_DATA) failed.\n");
            return MUSB_HFI_ERROR_BUSY;
        }
        else if ( Result != MUSB_SICD_SUCCESS )
        {
            LOG(1, "MUSB_SicdWriteDevice(MUSB_SICD_GET_NEXT_DATA) failed.\n");
            return MUSB_HFI_ERROR_TRANSFER;
        }
        break;

    case MUSB_SICD_SET_SILENT_BUS_RESET :
        Result = MUSB_SicdSilentBusReset(pSicdProtocol);
        
        if ( Result != MUSB_SICD_SUCCESS )
        {
            LOG(1, "MUSB_SicdWriteDevice(MUSB_SICD_SET_SILENT_BUS_RESET) failed.\n");
            return MUSB_HFI_ERROR_TRANSFER;
        }
        break;

    case MUSB_SICD_SEND_NEXT_DATA: 
        // TODO
        MUSB_ASSERT(0);
        break;

    default: 
        LOG(1, "MUSB_SicdWriteDevice case default error\n");
        return MUSB_HFI_ERROR_TRANSFER;
    }
    
    return MUSB_HFI_SUCCESS;
}

/******************************** Interface FUNCTIONS ( To Upper Layer )******************************/
/*******************************************************************************
**  MUSB_GetSicdDriver
**
**  descriptions:
**      
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_DeviceDriver *MUSB_GetSicdDriver()
{
    return &MUSB_SicdDeviceDriver;
}

/*******************************************************************************
**  MUSB_FillSicdPeripheralList
**
**  descriptions:
**      Copy Sicd periperal list into pList.
**  parameters:
**  result:
**  return:
**      The actual length of the copied data.
**  note:
*******************************************************************************/
uint16_t MUSB_FillSicdPeripheralList(uint8_t *pList, uint16_t wListLength, uint8_t bIndex)
{    
    uint16_t wResult = (uint16_t)MUSB_MIN(SICD_LIST_SIZE, wListLength);

    MUSB_ASSERT(bIndex < MUSB_SICD_LIST_NUM);

    (void)MUSB_MemCopy(pList, &MUSB_SicdPeripheralList[bIndex][0], wResult);

    return wResult;
}

/*******************************************************************************
**  MUSB_SicdGetMediumInfo
**
**  descriptions:
**      Get device information
**  parameters:
**  result:
**  return:
**      Device information.
**  note:
*******************************************************************************/
static MUSB_HfiStatus MUSB_SicdGetDeviceInfo(void *pDeviceData, 
                                                MUSB_HfiDeviceInfo **ppDeviceInfo)
{
    MUSB_SicdVolume *pVolume;
    
    MUSB_ASSERT(pDeviceData);
    MUSB_ASSERT(ppDeviceInfo);        
    pVolume = (MUSB_SicdVolume *)pDeviceData;

    *ppDeviceInfo = &(pVolume->DeviceInfo);

    return MUSB_HFI_SUCCESS;
}

/*******************************************************************************
**  MUSB_SicdReadDevice
**
**  descriptions:
**      It's interface API exported to HFI. We use this function for sending 
**      MTP/PTP operations.
**  parameters:
**      pDeviceData: DeviceData
**      pt_data: MTP/PTP operation parameters.
**  result:
**  return:
**      Operation sending result. (not operation final result)
**  note:
*******************************************************************************/
static MUSB_HfiStatus MUSB_SicdReadDevice(void *pDeviceData, 
                                                    MUSB_SICD_IO_DATA_T* pt_data)
{
    MGC_UcFsDevice *pUcFsDevice;
    MUSB_HfiSicdDevice *pDevice;    
    MUSB_SicdVolume *pVolume;
    MUSB_SicdProtocol* pSicdProtocol;

    pUcFsDevice = (MGC_UcFsDevice *)pDeviceData;
    if ((!pUcFsDevice) || (!pt_data))
    {
        return MUSB_HFI_ERROR_PARAMETER;
    }
    UNUSED(pt_data);

    pDevice = (MUSB_HfiSicdDevice *)pUcFsDevice->pvDevice;
    if (!pDevice)
    {
        return MUSB_HFI_ERROR_PARAMETER;
    }
    
    pVolume = (MUSB_SicdVolume *)pDevice->pPrivateData;
    if (!pVolume->bAnnounced)
    {
        return MUSB_HFI_ERROR_MEDIA_REMOVED;
    }
    
    pSicdProtocol = pVolume->pSicdProtocol;
    MUSB_ASSERT(pSicdProtocol);
    
    (void)MUSB_MemCopy((void *)&pSicdProtocol->SicdNfyInfo, 
        (void *)&pUcFsDevice->rt_nfy_info, sizeof(MUSB_NFY_INFO_T));

    return MGC_SicdRead(pSicdProtocol, pt_data);
}

/*******************************************************************************
**  MUSB_SicdWriteDevice
**
**  descriptions:
**      It's interface API exported to HFI. We use this function for sending 
**      SICD class-specific requests.
**  parameters:
**      pDeviceData: DeviceData
**      pt_data: MTP/PTP operation parameters.
**  result:
**  return:
**      Operation sending result. (not operation final result)
**  note:
*******************************************************************************/
static MUSB_HfiStatus MUSB_SicdWriteDevice(void *pDeviceData, 
                                                    MUSB_SICD_IO_DATA_T* pt_data)
{
    
    MGC_UcFsDevice *pUcFsDevice;
    MUSB_HfiSicdDevice *pDevice;
    MUSB_SicdVolume *pVolume;
    MUSB_SicdProtocol* pSicdProtocol;

    pUcFsDevice = (MGC_UcFsDevice *)pDeviceData;
    if ((!pUcFsDevice) || (!pt_data))
    {
        return MUSB_HFI_ERROR_PARAMETER;
    }
    UNUSED(pt_data);
    
    pDevice = (MUSB_HfiSicdDevice *)pUcFsDevice->pvDevice;
    if (!pDevice)
    {
        return MUSB_HFI_ERROR_PARAMETER;
    }
    
    pVolume = (MUSB_SicdVolume *)pDevice->pPrivateData;
    if (!pVolume->bAnnounced)
    {
        return MUSB_HFI_ERROR_MEDIA_REMOVED;
    }
    
    pSicdProtocol = pVolume->pSicdProtocol;
    MUSB_ASSERT(pSicdProtocol);

    (void)MUSB_MemCopy((void *)&pSicdProtocol->SicdNfyInfo, 
        (void *)&pUcFsDevice->rt_nfy_info, sizeof(MUSB_NFY_INFO_T));

    return MGC_SicdWrite(pSicdProtocol, pt_data);
}

/******************************** Callback FUNCTIONS ( To Lower Layer )******************************/

/*******************************************************************************
**  MUSB_SicdConnect
**
**  descriptions:
**      Connect handler for SICD class driver. This function will be invoked
**      after device connected and enumeration finished. Look for interface
**      which has 3 endpoints (IN/OUT/INTERRUPT). Once get this kind of interface, 
**      we suppose it is SICD interface. (We can do so since MsOsDescriptor && 
**      SICD class/subclass ids has been already checked during enumeration.)
**  parameters:
**  result:
**  return:
**      OK:TRUE, FAILED:FALSE
**  note:
*******************************************************************************/
static uint8_t MUSB_SicdConnect(void *pPrivateData, 
                         MUSB_BusHandle     hBus,
                         MUSB_Device       *pUsbDevice,
                         const uint8_t     *pPeripheralList)
{
    uint8_t bIndex, bEnd;
    const MUSB_ConfigurationDescriptor *pConfig;
    const MUSB_InterfaceDescriptor *pIface;
    const MUSB_EndpointDescriptor *pEnd;
    const MUSB_EndpointDescriptor *pInEnd;
    const MUSB_EndpointDescriptor *pOutEnd;
    const MUSB_EndpointDescriptor *pIntrEnd;
    uint8_t bSicdInterfaceNumber = 0;
    MUSB_SicdDevice *pSicdDevice;

    LOG(5,"MUSB_SicdConnect\n");

    MUSB_ASSERT(pUsbDevice);
    pConfig = pUsbDevice->apConfigDescriptors[0];
    
    UNUSED(pUsbDevice);
    UNUSED(pPrivateData);
    UNUSED(pPeripheralList);
    UNUSED(hBus);  

    /* find first interface with supported subclass/protocol combination */
    for (bIndex = 0; bIndex < pConfig->bNumInterfaces; bIndex++)
    {
        /* assume no alternates */
        pIface = MUSB_FindInterfaceDescriptor(pConfig, bIndex, 0);
        if (pIface && (pIface->bNumEndpoints == 3))
        {
            pInEnd = NULL;
            pOutEnd = NULL;
            pIntrEnd = NULL;
            
            /* SICD: find bulk-in & bulk-out & interrupt ends */
            for (bEnd = 0; bEnd < pIface->bNumEndpoints; bEnd++)
            {
                pEnd = MUSB_FindEndpointDescriptor(pConfig, pIface, bEnd);
                if (pEnd && (MUSB_ENDPOINT_XFER_BULK == 
                            (pEnd->bmAttributes&MUSB_ENDPOINT_XFERTYPE_MASK)))
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
                else if (pEnd && (MUSB_ENDPOINT_XFER_INT == 
                            (pEnd->bmAttributes&MUSB_ENDPOINT_XFERTYPE_MASK)))
                {
                    pIntrEnd = pEnd;
                }
            }
            
            /* continue if not found */
            if (!pInEnd || !pOutEnd || !pIntrEnd)
            {
                LOG(1, "SICD: insufficient endpoints 0x%08X, 0x%08X, 0x%08X.\n", 
                    (uint32_t)pInEnd, (uint32_t)pOutEnd, (uint32_t)pIntrEnd);
                continue;
            }
            else
            {
                pSicdDevice = MGC_SicdDeviceInit();
                if ( pSicdDevice )
                {
                    pUsbDevice->bPortCount = 1; /* This device always = 1 port device. */
                    pUsbDevice->pDriverPrivateData = pSicdDevice;

                    /* Initialize */
                    pSicdDevice->pUsbDevice = pUsbDevice;
                    pSicdDevice->hBus = hBus;
                    pSicdDevice->pIface = pIface;
                    pSicdDevice->pInEnd = pInEnd;
                    pSicdDevice->pOutEnd = pOutEnd;
                    pSicdDevice->pIntrEnd = pIntrEnd;
                    pSicdDevice->bInterfaceNumber = bSicdInterfaceNumber;

                    // start to check MS OS descriptor.                   
                    return (MUSB_STATUS_OK == MGC_GetMsOsDes(pSicdDevice)) ? TRUE : FALSE;
                }        
            }
        }
    }

    return FALSE;
}

/*******************************************************************************
**  MUSB_SicdDisconnect
**
**  descriptions:
**      Disconnect Handler for SICD Device Driver. This function will be invoked
**      after the device be removed.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static uint8_t MUSB_SicdDisconnect (void *pPrivateData, 
                            MUSB_BusHandle  hBus,
                            MUSB_Device    *pUsbDevice)
{
    MUSB_SicdDevice *pSicdDevice;
    MUSB_SicdProtocol* pProtocol;

    UNUSED(pPrivateData);
    MUSB_ASSERT(pUsbDevice);

    pSicdDevice = (MUSB_SicdDevice *)pUsbDevice->pDriverPrivateData;
    
    /* Check against the USB device and bus handle */
    if ((hBus != pSicdDevice->hBus) || (pUsbDevice != pSicdDevice->pUsbDevice))
    {
        return FALSE;
    }

    pProtocol = pSicdDevice->pProtocol;
    if (pProtocol)
    {
        (void)MUSB_SicdStateExit(pProtocol);

        /* Release the resources held */
        MGC_DestroySicdInstance(pProtocol);
    }
    
    MUSB_MemFree(pSicdDevice);
    pUsbDevice->pDriverPrivateData = NULL;
    
    LOG(5, "SICD remove Device:0x%08X.\n", (uint32_t)pUsbDevice);

    UNUSED(hBus);
    return TRUE;
}

#endif  /* #ifdef MUSB_SICD */
