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
 * Sample CDC communication device class peripheral
 * $Revision: #1 $
 */
#ifdef MUSB_FUNCTION

#include "mu_devfn.h"
#include "mu_cdi.h"
#include "mu_diag.h"
#include "mu_mem.h"
#include "x_assert.h"

LINT_SAVE_AND_DISABLE

#include "dmx_if.h"

/**
 * You may define this if your core is configured for a bidirectional endpoint 1
 */
#ifdef MUSB_DIAG
#define STATIC
#else
#define STATIC static
#endif

/* MTK Vendor-Specfic Codes ,EP0*/
#define MTK_USB_SET_CAPTURE_START       0x11
#define MTK_USB_SET_CAPTURE_STOP        0x10

#define MGC_DMXBUFSIZE                  (2 * 1024 * 1024)

#define USB_CMD_QUEUE_SIZE              32
#define USB_THREAD_STACK_SIZE           1024
#define USB_THREAD_PRIORITY             40
#define USB_DEV_MAXPACKETSIZE           (512)
#define MAX_DMX_IRP     8

typedef enum
{
    MTK_VENDOR_CMD_EXIT,
    MTK_VENDOR_CMD_CAPTURE_START,
    MTK_VENDOR_CMD_CAPTURE_STOP
} MGC_MtkVendorCmd_t;

typedef enum
{
    MGC_GENERIC_USB,
    MGC_GENERIC_DMX_CAPTURE
} MGC_GenericDevState_t;

/**
 * Data for bridge between USB and CDC 
 * @field hTxTransfer semaphore to TX serialize transfer requests
 * @field hRxTransfer semaphore to RX serialize transfer requests
 * @field hTransferDone semaphore signalled when transfer is complete
 * @field pdwActualLength return actual bytes transferred
 * @field pfTransferComplete CDC async transfer completion callback
 */
typedef struct
{
    MGC_GenericDevState_t eState;
    HANDLE_T hVendorCmdQueue;
    HANDLE_T hCmdReceiveThread;
    BOOL fIsTerminated;
    BOOL fStopTransfer;
    BOOL bEnableBypass;
    uint32_t u4FrameErrorSetting;
    uint32_t u4CapBufferSize;
    uint32_t u4CapThreshold;
} MGC_GenericDevice_t;

/**
 * Data buffer for RX data from Host. 
 * @field rInfo dmx capture information.
 * @field dwTotalLength total tx bytes.
 */
typedef struct
{
    DMX_CAPTURE_INFO_T rInfo;
    uint64_t dwTotalLength;
} MGC_GenericUsbDmx_t;

#ifdef FVR_SUPPORT_CAPTURE
extern BOOL FVR_UpdateCaptureReadPointer(UINT32 u4NewRp);
extern BOOL FVR_SetCapture(BOOL fgEnable, const DMX_CAPTURE_T* prCapture);
#endif

/******************************************************************
Forwards
******************************************************************/

STATIC uint8_t MGC_GenericDeviceRequest(void *hClient, MUSB_BusHandle hBus,
                                        uint32_t dwSequenceNumber, const uint8_t *pSetup,
                                        uint16_t wRequestLength);

STATIC uint8_t MGC_GenericDeviceConfigSelected(void *hClient, MUSB_BusHandle hBus,
                                               uint8_t bConfigurationValue, MUSB_Pipe *ahPipe);

STATIC void MGC_GenericNewUsbState(void *hClient, MUSB_BusHandle hBus, MUSB_State State);

STATIC void MGC_GenericCaptureTxComplete(void *pCompleteParam, const MUSB_Irp *pIrp);
STATIC int32_t MGC_GenericDevStartCapture(BOOL bEnableBypassMode, uint32_t u4CaBufSize,
                                   uint32_t u4Threshold);
STATIC int32_t MGC_GenericDevStopCapture(void);
                               
/******************************************************************
Globals
******************************************************************/
/* UCDI variables */

static uint8_t MGC_bGenericUsbSelfPowered = TRUE;
static uint8_t MGC_aGenericUsbControlData[256];
static MGC_GenericDevice_t MGC_GenericDevice;
static uint32_t MGC_dwGenericUsbInitialize = 0;
static uint8_t MGC_bGenericUsbInterface = 0;
static MUSB_State MGC_eGenericUsbState = MUSB_POWER_OFF;
static MGC_GenericUsbDmx_t MGC_DevDmx;
static uint32_t MGC_dwGenericTxIndex = 0;
static const char MGC_aGenericUsbDescriptorData [] =
{
    /* Device Descriptor */
    0x12,           /* bLength              */
    MUSB_DT_DEVICE, /* DEVICE               */
    0x00,0x02,                  /* USB 2.0              */
    0xFF,           /* CLASS: vendor-specific */
    0x00,           /* Subclass             */
    0x00,           /* Protocol             */
    0x40,           /* bMaxPktSize0         */
    0x8D,0x0E,                  /* idVendor: MediaTek   */
    0x81,0x53,                  /* idProduct: mt5381    */
    0x00,0x00,                  /* bcdDevice            */
    0x01,           /* iManufacturer        */
    0x02,           /* iProduct             */
    0x03,           /* iSerial Number       */
    0x01,           /* One configuration    */

/* strings */
    2 + 4,
    MUSB_DT_STRING,
    0x09, 0x04,			        /* English (U.S.) */
    0x09, 0x0c,			        /* English (Australia) */

/* TODO: make tool to generate strings and eventually whole descriptor! */
/* English (U.S.) strings */
    2 + 24, /* Manufacturer: MediaTek Inc */
    MUSB_DT_STRING,
    'M', 0, 'e', 0, 'd', 0, 'i', 0, 'a', 0, 'T', 0, 'e', 0,
    'k', 0, ' ', 0, 'I', 0, 'n', 0, 'c', 0,

    2 + 38, /* Product ID: 5381 generic device */
    MUSB_DT_STRING,
    '5', 0, '3', 0, '8', 0, '1', 0, ' ', 0, 'G', 0, 'e', 0,
    'n', 0, 'e', 0, 'r', 0, 'i', 0, 'c', 0, ' ', 0, 'D', 0,
    'e', 0, 'v', 0, 'i', 0, 'c', 0, 'e', 0,

    2 + 26, /* Serial #: SA3 MTK1321-1 */
    MUSB_DT_STRING,
    'S', 0, 'A', 0, '3', 0, ' ', 0,
    'M', 0, 'T', 0, 'K', 0, '1', 0,
    '3', 0, '2', 0, '1', 0, '-', 0, 
    '2', 0,

/* English (Australia) strings */
    2 + 24, /* Manufacturer: MediaTek Inc */
    MUSB_DT_STRING,
    'M', 0, 'e', 0, 'd', 0, 'i', 0, 'a', 0, 'T', 0, 'e', 0,
    'k', 0, ' ', 0, 'I', 0, 'n', 0, 'c', 0,

    2 + 38, /* Product ID: 5381 generic device */
    MUSB_DT_STRING,
    '5', 0, '3', 0, '8', 0, '1', 0, ' ', 0, 'G', 0, 'e', 0,
    'n', 0, 'e', 0, 'r', 0, 'i', 0, 'c', 0, ' ', 0, 'D', 0,
    'e', 0, 'v', 0, 'i', 0, 'c', 0, 'e', 0,

    2 + 26, /* Serial #: SA3 MTK1321-1 */
    MUSB_DT_STRING,
    'S', 0, 'A', 0, '3', 0, ' ', 0,
    'M', 0, 'T', 0, 'K', 0, '1', 0,
    '3', 0, '2', 0, '1', 0, '-', 0, 
    '2', 0,

/* configuration */
    0x09,           /* bLength              */
    MUSB_DT_CONFIG, /* CONFIGURATION        */
    0x20,           /* total length         */
    0x0,
    0x00,           /* bNumInterfaces       */
    0x01,           /* bConfigurationValue  */
    0x00,           /* iConfiguration       */
    0xC0,           /* bmAttributes (required + self-powered) */
    0x02,           /* power                */

/* interface */
    0x09,              /* bLength              */
    MUSB_DT_INTERFACE, /* INTERFACE            */
    0x00,              /* bInterfaceNumber     */
    0x00,              /* bAlternateSetting    */
    0x02,              /* bNumEndpoints        */
    0xFF,              /* bInterfaceClass: Vendor-Specific */
    0x00,              /* bInterfaceSubClass: Unused*/
    0x00,              /* bInterfaceProtocol: No class specific protocol required */
    0x00,              /* iInterface           */

/* Endpoint Descriptor  : Bulk-In */
    0x07,             /* bLength              */
    MUSB_DT_ENDPOINT, /* ENDPOINT             */
    0x81,             /* bEndpointAddress     */
    0x02,             /* bmAttributes         */
    0x40, 0x00,                 /* wMaxPacketSize       */
    0x00,             /* bInterval            */

/* Endpoint Descriptor  : Bulk-Out */
    0x07,             /* bLength              */
    MUSB_DT_ENDPOINT, /* ENDPOINT             */
    0x02,             /* bEndpointAddress     */
    0x02,             /* bmAttributes         */
    0x40, 0x00,                 /* wMaxPacketSize       */
    0x00             /* bInterval            */
};

static const uint8_t MGC_aGenericUsbHighSpeedDescriptorData [] =
{
    /* device qualifier */
    0x0a,                     /* bLength              */
    MUSB_DT_DEVICE_QUALIFIER, /* DEVICE Qualifier     */
    0x00,0x02,                  /* USB 2.0              */
    0xFF,                     /* CLASS: vendor-specific */
    0,                        /* Subclass             */
    0x00,                     /* Protocol             */
    0x40,                     /* bMaxPacketSize0      */
    0x01,                     /* One configuration    */
    0x00,                     /* bReserved            */

/* configuration */
    0x09,                /* bLength              */
    MUSB_DT_OTHER_SPEED, /* CONFIGURATION        */
    0x20,
    0x0,                 /* length               */
    0x01,                /* bNumInterfaces       */
    0x01,                /* bConfigurationValue  */
    0x00,                /* iConfiguration       */
    0xC0,                /* bmAttributes (required + self-powered) */
    0x2,                 /* power                */

/* interface */
    0x09,              /* bLength              */
    MUSB_DT_INTERFACE, /* INTERFACE            */
    0x00,              /* bInterfaceNumber     */
    0x00,              /* bAlternateSetting    */
    0x02,              /* bNumEndpoints        */
    0xFF,              /* bInterfaceClass: Vendor-Specific */
    0x00,              /* bInterfaceSubClass: Unused*/
    0x00,              /* bInterfaceProtocol: No class specific protocol required */
    0x00,              /* iInterface           */

/* Endpoint Descriptor  : Bulk-In */
    0x07,             /* bLength              */
    MUSB_DT_ENDPOINT, /* ENDPOINT             */
    0x81,             /* bEndpointAddress     */
    0x02,             /* bmAttributes         */
    0x00, 0x02,                 /* wMaxPacketSize       */
    0x0,              /* bInterval            */

/* Endpoint Descriptor  : Bulk-Out */
    0x07,             /* bLength              */
    MUSB_DT_ENDPOINT, /* ENDPOINT             */
    0x02,             /* bEndpointAddress     */
    0x02,             /* bmAttributes         */
    0x00, 0x02,                 /* wMaxPacketSize       */
    0x00             /* bInterval            */
};

/** IRP for data transmission */

static MUSB_Irp MGC_GenericUsbDmxTxDataIrp[MAX_DMX_IRP];
/*
* registration
*/

MUSB_FunctionClient MGC_GenericUsbFunctionClient =
{
    NULL, /* no instance data; we are singleton */
    (const uint8_t*)MGC_aGenericUsbDescriptorData,
    sizeof(MGC_aGenericUsbDescriptorData),
    3,    /* strings per language */
    MGC_aGenericUsbHighSpeedDescriptorData,
    sizeof(MGC_aGenericUsbHighSpeedDescriptorData),
    sizeof(MGC_aGenericUsbControlData),
    MGC_aGenericUsbControlData,
    &MGC_bGenericUsbSelfPowered,
    MGC_GenericDeviceRequest,
    MGC_GenericDeviceConfigSelected,
    NULL,
    MGC_GenericNewUsbState
};

/******************************************************************
CDI callbacks
******************************************************************/
STATIC void MGC_GenericNewUsbState(void *hClient, MUSB_BusHandle hBus, MUSB_State State)
{
    UNUSED(hClient);

    UNUSED(hBus);

    MGC_eGenericUsbState = State;
/* TODO: anything? */
}

STATIC uint8_t MGC_GenericDeviceRequest(void *hClient, MUSB_BusHandle hBus, uint32_t dwSequence,
                                        const uint8_t *pSetup, uint16_t wLength)
{
    MUSB_DeviceRequest *pRequest = (MUSB_DeviceRequest *)((uint32_t)pSetup);

    uint8_t bOk = FALSE;
    int32_t i4Result;
    uint32_t u4Msg;
    uint32_t pData;
    uint32_t dwStatus;

    UNUSED(hClient);
    MUSB_ASSERT(pRequest);

    if (MUSB_TYPE_STANDARD == (pRequest->bmRequestType & MUSB_TYPE_MASK))
    {
        switch (pRequest->bRequest)
        {
        case MUSB_REQ_GET_INTERFACE:
            dwStatus = MUSB_DeviceResponse(hBus, dwSequence, &MGC_bGenericUsbInterface, 1, FALSE);
            UNUSED(dwStatus);
            bOk = TRUE;
            break;

        case MUSB_REQ_SET_INTERFACE:
            dwStatus = MUSB_DeviceResponse(hBus, dwSequence, NULL, 0, FALSE);
            UNUSED(dwStatus);
            bOk = TRUE;
            break;

        default:
            break;
        }
    }
    else if (pRequest->bmRequestType & MUSB_TYPE_VENDOR)
    {
        switch (pRequest->bRequest)
        {
        case MTK_USB_SET_CAPTURE_START:
            LOG(7, "MTK_USB_SET_CAPTURE_START\n");

            // Check the request packet size
            // if Total size == Setup packet (8 bytes) + data (8 bytes)
            // -> Sepecific capture buffer size and threshold is set by command
            // else 
            // -> use the default setting
            u4Msg = (uint32_t)MTK_VENDOR_CMD_CAPTURE_START;
            MGC_GenericDevice.bEnableBypass = (BOOL)pRequest->wValue;
            MGC_GenericDevice.u4CapBufferSize = MGC_DMXBUFSIZE;
            MGC_GenericDevice.u4CapThreshold = MGC_DMXBUFSIZE / 2;

            UNUSED(pData);
            UNUSED(wLength);
            /*
            // Use PC transfer buffer size setting.
            if (wLength == (sizeof(MUSB_DeviceRequest) + 8))
            {
                pData = (uint32_t)pRequest + sizeof(MUSB_DeviceRequest);

                MGC_GenericDevice.u4CapBufferSize = *((uint32_t *)pData);
                MGC_GenericDevice.u4CapThreshold = *((uint32_t *)(pData + 4));
            }
            */
            i4Result = (int32_t)x_msg_q_send(MGC_GenericDevice.hVendorCmdQueue, &u4Msg, sizeof(uint32_t), 1);
            UNUSED(i4Result);
            dwStatus = MUSB_DeviceResponse(hBus, dwSequence, NULL, 0, FALSE);
            UNUSED(dwStatus);
            bOk = TRUE;
            break;

        case MTK_USB_SET_CAPTURE_STOP:
            LOG(7, "MTK_USB_SET_CAPTURE_STOP\n");

            MGC_GenericDevice.fStopTransfer = TRUE;

            u4Msg = (uint32_t)MTK_VENDOR_CMD_CAPTURE_STOP;

            i4Result = (int32_t)x_msg_q_send(MGC_GenericDevice.hVendorCmdQueue, &u4Msg, sizeof(uint32_t), 1);
            UNUSED(i4Result);
            dwStatus = MUSB_DeviceResponse(hBus, dwSequence, NULL, 0, FALSE);
            UNUSED(dwStatus);
            bOk = TRUE;
            break;

        default:
            // send stall.	
            dwStatus = MUSB_DeviceResponse(hBus, dwSequence, NULL, 0, TRUE);
            UNUSED(dwStatus);
            bOk = TRUE;
            break;
        }
    }

    return bOk;
}

// *********************************************************************
// Function : void MGC_DevThread(void)
// Description : To process the received vendor request
// Parameter : none
// Return    : None
// *********************************************************************
static void MGC_DevThread(void *pvArg)
{
    int32_t i4Ret;

    uint32_t u4Event;
    UINT16 u2MsgIdx;
    SIZE_T zMsgSize = sizeof(uint32_t);
    UNUSED(pvArg);

    while (1)
    {
        i4Ret = (int32_t)x_msg_q_receive(&u2MsgIdx, &u4Event, &zMsgSize, &MGC_GenericDevice.hVendorCmdQueue,
                                1, X_MSGQ_OPTION_WAIT);

        if (i4Ret != OSR_OK)
        {
            MGC_GenericDevice.fIsTerminated = TRUE;
            return;
        }

        // Parse the vendor commands
        switch (u4Event)
        {
        case MTK_VENDOR_CMD_EXIT:
            MGC_GenericDevice.fIsTerminated = TRUE;
            return;

        case MTK_VENDOR_CMD_CAPTURE_START:
            i4Ret = MGC_GenericDevStartCapture(MGC_GenericDevice.bEnableBypass,
                                               MGC_GenericDevice.u4CapBufferSize,
                                               MGC_GenericDevice.u4CapThreshold);
            break;

        case MTK_VENDOR_CMD_CAPTURE_STOP:
            i4Ret = MGC_GenericDevStopCapture();
            break;
            
        default:
            break;
        }
    }
}

void MUSB_GenericDevInit(void)
{
    if (!MGC_dwGenericUsbInitialize)
    {
        MGC_GenericDevice.eState = MGC_GENERIC_USB;

        MGC_GenericDevice.fIsTerminated = FALSE;

        // Create a message queue for vendor request
        VERIFY((x_msg_q_create(&MGC_GenericDevice.hVendorCmdQueue, "UsbCmdQ", sizeof(uint32_t),
                               USB_CMD_QUEUE_SIZE)) == OSR_OK);

        // Create a thread to parse vendor requests
        VERIFY(x_thread_create(&MGC_GenericDevice.hCmdReceiveThread, "UsbCmdR",
                               USB_THREAD_STACK_SIZE, USB_THREAD_PRIORITY,
                               MGC_DevThread, 0,
                               NULL) == OSR_OK);

        MGC_dwGenericUsbInitialize = 1;
    }
}

STATIC uint8_t MGC_GenericDeviceConfigSelected(void *hClient, MUSB_BusHandle hBus,
                                               uint8_t bConfigurationValue, MUSB_Pipe *ahPipe)
{
    uint32_t i;

    UNUSED(hClient);
    UNUSED(hBus);
    UNUSED(bConfigurationValue);
    MUSB_ASSERT(ahPipe);

    Printf("Generic device config ok, config = %d\n", bConfigurationValue);

    for (i = 0; i < MAX_DMX_IRP; i++)
    {        
        MGC_GenericUsbDmxTxDataIrp[i].hPipe = ahPipe[0];
        MGC_GenericUsbDmxTxDataIrp[i].pfIrpComplete = MGC_GenericCaptureTxComplete;
        MGC_GenericUsbDmxTxDataIrp[i].bIsrCallback = TRUE;
        MGC_GenericUsbDmxTxDataIrp[i].bAllowShortTransfer = TRUE;
        MGC_GenericUsbDmxTxDataIrp[i].bAllowDma = TRUE;
    }

    return TRUE;
}

STATIC void MGC_GenericCaptureTxComplete(void *pCompleteParam, const MUSB_Irp *pIrp)
{
    uint32_t i;
    uint32_t _u4Count = 0;

    MUSB_ASSERT(pIrp);

    _u4Count = (uint32_t)pCompleteParam;
    
    for (i = 0; i < MAX_DMX_IRP; i++)
    {
        if ((uint32_t)&MGC_GenericUsbDmxTxDataIrp[i] == (uint32_t)pIrp)
        {        
            LOG(5, "[%d-%d] Tx c pBuffer = 0x%08X, size = %d.\n", 
            	_u4Count, i, (uint32_t)pIrp->pBuffer, pIrp->dwActualLength);      
            MGC_GenericUsbDmxTxDataIrp[i].dwLength = 0;
            MGC_GenericUsbDmxTxDataIrp[i].pBuffer = NULL;
            MGC_GenericUsbDmxTxDataIrp[i].dwStatus = 0;
            MGC_GenericUsbDmxTxDataIrp[i].dwActualLength = 0L;
            return;
        }
    }

    UNUSED(_u4Count);
    LOG(5, "Tx complete pIrp = 0x%08X, size = %d.\n", (uint32_t)pIrp, pIrp->dwActualLength);
    MUSB_ASSERT(0);
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
STATIC BOOL MGC_GenericCaptureHandler(DMX_CAPTURE_NOTIFY_CODE_T eCode, const DMX_CAPTURE_INFO_T *prInfo,
                               void *pvTag)
{
    uint32_t u4Size;
    uint32_t dwStatus = MGC_M_STATUS_ERROR;
    uint32_t i;

    MUSB_ASSERT(prInfo);
    UNUSED(pvTag);
    
    if (MGC_GenericDevice.fStopTransfer)
    {
        // Debug, drop captured data directly
#ifdef FVR_SUPPORT_CAPTURE
        VERIFY(FVR_UpdateCaptureReadPointer(prInfo->u4Wp));
#else
        VERIFY(DMX_UpdateCaptureReadPointer(prInfo->u4Wp));
#endif        
        return FALSE;
    }

    MUSB_ASSERT(eCode == DMX_CAPTURE_NOTIFY_CODE_RECEIVE_DATA);

    // increase capture index;
    MGC_dwGenericTxIndex ++;

    LOG(5, "[%u] Captured WP: 0x%08X, RP: 0x%08X, size: %u\n", 
    	MGC_dwGenericTxIndex, prInfo->u4Wp, prInfo->u4Rp, prInfo->u4DataSize);

    x_memcpy(&MGC_DevDmx.rInfo, prInfo, sizeof(DMX_CAPTURE_INFO_T));

    u4Size = prInfo->u4DataSize;

#ifdef FVR_SUPPORT_CAPTURE
    if ((prInfo->u4Rp + u4Size) >= prInfo->u4BufEnd)
    {
        u4Size = prInfo->u4BufEnd - prInfo->u4Rp;

        VERIFY(FVR_UpdateCaptureReadPointer(prInfo->u4BufStart));
    }
    else
    {
        VERIFY(FVR_UpdateCaptureReadPointer(prInfo->u4Rp + u4Size));
    }
#else
    if ((prInfo->u4Rp + u4Size) >= prInfo->u4BufEnd)
    {
        u4Size = prInfo->u4BufEnd - prInfo->u4Rp;

        MUSB_ASSERT(0 == (u4Size&(USB_DEV_MAXPACKETSIZE - 1)));
        VERIFY(DMX_UpdateCaptureReadPointer(prInfo->u4BufStart));
    }
    else
    {
        u4Size = prInfo->u4Wp - prInfo->u4Rp;

        u4Size -= u4Size&(USB_DEV_MAXPACKETSIZE - 1);
        VERIFY(DMX_UpdateCaptureReadPointer(prInfo->u4Rp + u4Size));
    }
#endif

    for (i = 0; i < MAX_DMX_IRP; i++)
    {
        if (MGC_GenericUsbDmxTxDataIrp[i].dwLength == 0)
        {
            break;
        }
    }

    if (i >= MAX_DMX_IRP)
    {
        LOG(1, "[%u] IRP full, fram drop: u4Size = %d.\n", 
            MGC_dwGenericTxIndex, u4Size);
    
        // no more empty IRP slot, drop the captured data directly.
        return FALSE;
    }

    MGC_GenericUsbDmxTxDataIrp[i].dwLength = u4Size;
    MGC_GenericUsbDmxTxDataIrp[i].pBuffer = (uint8_t *)prInfo->u4Rp;
    MGC_GenericUsbDmxTxDataIrp[i].dwStatus = 0;
    MGC_GenericUsbDmxTxDataIrp[i].dwActualLength = 0L;
    // use dmx counter as index.
    MGC_GenericUsbDmxTxDataIrp[i].pCompleteParam = 
        (void *)MGC_dwGenericTxIndex;
    
    LOG(5, "[%d-%d] Tx pBuffer = 0x%08X, size = %d.\n", 
        MGC_dwGenericTxIndex, i, (uint32_t)prInfo->u4Rp, 
        MGC_GenericUsbDmxTxDataIrp[i].dwLength);

    dwStatus = MUSB_StartTransfer(&MGC_GenericUsbDmxTxDataIrp[i]);
    MUSB_ASSERT(dwStatus == 0);
    if (dwStatus != MUSB_STATUS_SUCCESS)    
    {
        return FALSE;
    }

    return TRUE;
}

STATIC int32_t MGC_GenericDevStartCapture(BOOL bEnableBypassMode, uint32_t u4CaBufSize,
                                   uint32_t u4Threshold)
{
    DMX_CAPTURE_T rCapture;

    if (MUSB_CONFIGURED != MGC_eGenericUsbState)
    {
        return -1;
    }

    if (MGC_GENERIC_DMX_CAPTURE == MGC_GenericDevice.eState)
    {
        return -1;
    }

    if (bEnableBypassMode)
    {
        // Get the current dmx setting
        MGC_GenericDevice.u4FrameErrorSetting = DMX_GetFramerErrorHandlingTable();

        LOG(5, "Dmx Framer Error %8x\n", MGC_GenericDevice.u4FrameErrorSetting);

        // Enable bypass mode
        DMX_SetFramerErrorHandlingTable(0);
    }

    // set dmx state and clear variable.
    MGC_GenericDevice.eState = MGC_GENERIC_DMX_CAPTURE;
    MGC_GenericDevice.fStopTransfer = FALSE;
    x_memset(&MGC_DevDmx, 0, sizeof(MGC_GenericUsbDmx_t));

    if (u4CaBufSize == 0)
    {
        // Disable capture function
#ifdef FVR_SUPPORT_CAPTURE
        UNUSED(FVR_SetCapture(FALSE, NULL));
#else
        UNUSED(DMX_SetCapture(FALSE, NULL));
#endif

        LOG(1, "USB Capture stopped\n", NULL);
        return -1;
    }
		
    if (!DMX_IsAligned(u4CaBufSize, DMX_CA_BUFFER_ALIGNMENT))
    {
        LOG(1, "USB CA buffer size must be %u-byte alignment\n", DMX_CA_BUFFER_ALIGNMENT);
        return -1;
    }

    // init capture index;
    MGC_dwGenericTxIndex = 0;

    rCapture.u4BytePassDev = DMX_CAPTURE_BYPASS_FRAMER0;
    rCapture.u4BufSize = u4CaBufSize;
    rCapture.u4Threshold = u4Threshold;
    rCapture.pfnCaptureHandler = MGC_GenericCaptureHandler;
    rCapture.pvCaptureTag = NULL;

    //if (!MUSB_FakeSetCapture(TRUE, &rCapture))
#ifdef FVR_SUPPORT_CAPTURE
    if (!FVR_SetCapture(TRUE, &rCapture))
#else
    if (!DMX_SetCapture(TRUE, &rCapture))
#endif    
    {
        LOG(1, "USB Fail to enable capture\n", NULL);
        return -1;
    }

    LOG(7, "USB Capture start...\n", NULL);

    return 0;
}

STATIC int32_t MGC_GenericDevStopCapture(void)
{
    uint32_t i;

    if (MUSB_CONFIGURED != MGC_eGenericUsbState)
    {
        return -1;
    }

    if (MGC_GENERIC_DMX_CAPTURE != MGC_GenericDevice.eState)
    {
        return -1;
    }

    // Disable capture function
#ifdef FVR_SUPPORT_CAPTURE
    if (!FVR_SetCapture(FALSE, NULL))
#else
    if (!DMX_SetCapture(FALSE, NULL))
#endif    
    {
        LOG(1, "Fail to disable capture\n", NULL);
        return -1;
    }

    for (i = 0; i < MAX_DMX_IRP; i++)
    {
        if (MGC_GenericUsbDmxTxDataIrp[i].dwLength != 0)
        {
            VERIFY(MUSB_CancelTransfer(&MGC_GenericUsbDmxTxDataIrp[i]) == MUSB_STATUS_OK);
            MGC_GenericUsbDmxTxDataIrp[i].dwLength = 0;
        }
    }

    if (MGC_GenericDevice.bEnableBypass)
    {
        // Disable dmx bypass mode
        DMX_SetFramerErrorHandlingTable(MGC_GenericDevice.u4FrameErrorSetting);
        LOG(7, "Dmx Framer Error %8x\n", MGC_GenericDevice.u4FrameErrorSetting);
    }

    LOG(7, "USB Capture stop...\n", NULL);
    MGC_GenericDevice.eState = MGC_GENERIC_USB;

    return 0;
}

LINT_RESTORE

#endif /* MUSB_FUNCTION */
