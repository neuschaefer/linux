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
 
#include "mu_cdi.h"
#include "mu_diag.h"
#include "mu_mem.h"
#include "mu_cdc_if.h"
#include "mu_impl.h"

/* CDC Class-Specfic Codes ,EP0*/
#define USB_ACM_SET_LINE_CODING			0x20
#define USB_ACM_GET_LINE_CODING			0x21
#define USB_ACM_SET_CONTROL_LINE_STATE 0x22
#define USB_ACM_SEND_BREAK			        0x23

/* CDC States */
#define CDCFN_STATE_IDLE              0x00
#define CDCFN_STATE_INIT_1          0x01
#define CDCFN_STATE_INIT_2          0x02
#define CDCFN_STATE_READY          0x03

#define CDCFN_MAXPACKETSIZE     (512)

#define MGC_CDCBUFSIZE                (1024)

//#define MGC_CDCBUFDATA

/**
 * MUSB_CDCXferState.
 */
typedef enum
{
    MUSB_CDC_IDLE = 0,
    MUSB_CDC_SYNCTX,
    MUSB_CDC_ASYNCTX,
    MUSB_CDC_SYNCRX,
    MUSB_CDC_ASYNCRX
    
} MUSB_CDCXferState;

/**
 * Data for bridge between USB and CDC 
 * @field hTransfer semaphore to serialize transfer requests
 * @field hTransferDone semaphore signalled when transfer is complete
 * @field pdwActualLength return actual bytes transferred
 * @field pfTransferComplete CDC async transfer completion callback
 */
typedef struct
{
#ifndef MUSB_NONEOS    
    HANDLE_T hTransfer;
    HANDLE_T hTransferDone;    
#endif
    uint32_t dwTxSerial;
    uint32_t dwRxSerial;

    MUSB_CDCXferState eTxState;
    MUSB_CDCXferState eRxState;
    
    MUSB_pfCdcTransferComplete pfTxComplete;
    MUSB_pfCdcTransferComplete pfRxComplete;
} MGC_CdcDevice_t;

/**
 * Data buffer for RX data from Host. 
 * @field dwWriteIdx host write pointer.
 * @field dwReadIdx client read pointer.
 * @field dwWriteBotIdx host write bottom pointer when wrapped to top.
 * @field dwWriteSpace host write length.
 * @field dwReadSpace device read length.
 */
typedef struct
{
    uint32_t dwWriteIdx;
    uint32_t dwReadIdx;
    uint32_t dwWriteBotIdx;
    
    uint32_t dwWriteSpace;
    uint32_t dwReadSpace;
} MGC_CdcRxBuf_t;

/******************************************************************
Forwards
******************************************************************/
static uint8_t MGC_CdcDeviceRequest(void* hClient, MUSB_BusHandle hBus, 
				    uint32_t dwSequenceNumber, 
				    const uint8_t* pSetup, 
				    uint16_t wRequestLength);

static uint8_t MGC_CdcDeviceConfigSelected(void* hClient, 
					   MUSB_BusHandle hBus, 
					   uint8_t bConfigurationValue, 
					   MUSB_Pipe* ahPipe);
					   
static void MGC_CdcNewUsbState(void* hClient, MUSB_BusHandle hBus, 
			       MUSB_State State);

static void MGC_CdcAsyncRxComplete(void* pCompleteParam, const MUSB_Irp* pIrp);

static void MGC_CdcAsyncTxComplete(void* pCompleteParam, const MUSB_Irp* pIrp);

static void MGC_CdcSyncTxComplete(void* pCompleteParam, const MUSB_Irp* pIrp);

static void MGC_CdcSyncRxComplete(void *pCompleteParam, const MUSB_Irp *pIrp);
/******************************************************************
Globals
******************************************************************/
/* UCDI variables */
static uint8_t MGC_bCdcSelfPowered = TRUE;
static MUSB_State MGC_eCdcUsbState = MUSB_POWER_OFF;

static uint8_t MGC_bCdcControlState = 0;
static uint8_t MGC_aCdcControlData[256];
static MGC_CdcDevice_t MGC_CdcDevice;
static uint32_t MGC_dwCdcInitialize;
static uint8_t MGC_bCdcLineCoding[7] = {0x00, 0xC2, 0x01, 0x00, 0x00, 0x00, 0x08};
static uint8_t MGC_bCdcInterface = 0;

#ifdef MGC_CDCBUFDATA

static uint8_t MGC_bCdcBuf[MGC_CDCBUFSIZE];
static uint32_t MGC_dwCdcBufSize = sizeof(MGC_bCdcBuf);
static MGC_CdcRxBuf_t MGC_CdcBuf = {0, 0, MGC_CDCBUFSIZE, 0, 0};

#endif

static const char MGC_aCdcDescriptorData[] = 
{
    /* Device Descriptor */
    0x12,                      /* bLength              */
    MUSB_DT_DEVICE,            /* DEVICE               */
    0x00,0x02,                 /* USB 2.0              */
    0x02,                      /* CLASS: communication device class */
    0x00,                      /* Subclass             */
    0x00,                      /* Protocol             */
    0x40,                      /* bMaxPktSize0         */
    0x8D,0x0E,                 /* idVendor: MediaTek   */
    0x03,0x00,                 /* idProduct: number 3  */
    0x00,0x02,                 /* bcdDevice            */
    0x01,                      /* iManufacturer        */
    0x02,                      /* iProduct             */
    0x00,                      /* iSerial Number       */
    0x01,                      /* One configuration    */

    /* strings */
    2+2,
    MUSB_DT_STRING,
    0x09, 0x04,			/* English (U.S.) */

    /* TODO: make tool to generate strings and eventually whole descriptor! */
    /* English (U.S.) strings */
    2+24,			/* Manufacturer: MediaTek Inc */
    MUSB_DT_STRING,
    'M', 0, 'e', 0, 'd', 0, 'i', 0, 'a', 0, 'T', 0, 'e', 0,
    'k', 0, ' ', 0, 'I', 0, 'n', 0, 'c', 0,

    2+16,			/* Product ID: USB-Uart */
    MUSB_DT_STRING,
    'U', 0, 'S', 0, 'B', 0, '-', 0, 'U', 0, 'a', 0, 'r', 0, 't', 0,

    /* configuration */
    0x09,                                   /* bLength              */
    0x02,                                   /* CONFIGURATION        */
    0x30,
    0x0,                                    /* length               */
    0x02,                                   /* bNumInterfaces       */
    0x01,                                   /* bConfigurationValue  */
    0x00,                                   /* iConfiguration       */
    0xC0,                                   /* bmAttributes (required + self-powered) */
    0x02,                                    /* power                */

    /* interface */
    0x09,                                   /* bLength              */
    0x04,                                   /* INTERFACE            */
    0x0,                                    /* bInterfaceNumber     */
    0x0,                                    /* bAlternateSetting    */
    0x02,                                   /* bNumEndpoints        */
    0x0A,                                   /* bInterfaceClass: Data interface code */
    0x0,                                    /* bInterfaceSubClass: Unused*/
    0x0,                                    /* bInterfaceProtocol: No class specific protocol required */
    0x00,                                   /* iInterface           */

    /* Endpoint Descriptor  : Bulk-In */
    0x07,                                   /* bLength              */
    0x05,                                   /* ENDPOINT             */
    0x81,                                   /* bEndpointAddress     */
    0x02,                                   /* bmAttributes         */
    0x40, 0x00,                             /* wMaxPacketSize       */
    0x00,                                   /* bInterval            */

    /* Endpoint Descriptor  : Bulk-Out */
    0x07,                                   /* bLength              */
    0x05,                                   /* ENDPOINT             */
    0x02,                                   /* bEndpointAddress     */
    0x02,                                   /* bmAttributes         */
    0x40, 0x00,                             /* wMaxPacketSize       */
    0x00,                                   /* bInterval            */

    /* Interface Descriptor */
    0x09,                                   /* bLength              */
    0x04,                                   /* INTERFACE            */
    0x01,                                    /* bInterfaceNumber     */
    0x00,                                    /* bAlternateSetting    */
    0x01,                                   /* bNumEndpoints        */
    0x02,                                   /* bInterfaceClass: Communication Interface Class */
    0x02,                                   /* bInterfaceSubClass: Abstract Control Model */
    0x01,                                   /* bInterfaceProtocol: Common AT commands (also known as ¡§Hayes? compatible¡¨) */
    0x00,                                   /* iInterface           */

    /* Endpoint Descriptor  : Bulk-In */
    0x07,                                   /* bLength              */
    0x05,                                   /* ENDPOINT             */
    0x83,                                   /* bEndpointAddress     */
    0x03,                                   /* bmAttributes         */
    0x40, 0x00,                             /* wMaxPacketSize       */
    0x01                                   /* bInterval            */
};

static const uint8_t MGC_aCdcHighSpeedDescriptorData[] = 
{
    /* device qualifier */
    0x0a,                      /* bLength              */
    MUSB_DT_DEVICE_QUALIFIER,  /* DEVICE Qualifier     */
    0x00,0x02,                 /* USB 2.0              */
    0x02,                      /* CLASS: communication device class */
    0,                         /* Subclass             */
    0x00,                      /* Protocol             */
    0x40,                      /* bMaxPacketSize0      */
    0x01,                      /* One configuration    */
    0x00,                      /* bReserved            */

    /* configuration */
    0x09,                                   /* bLength              */
    MUSB_DT_OTHER_SPEED,                    /* CONFIGURATION        */
    0x30,
    0x0,                                    /* length               */
    0x02,                                   /* bNumInterfaces       */
    0x01,                                   /* bConfigurationValue  */
    0x00,                                   /* iConfiguration       */
    0xC0,                                   /* bmAttributes (required + self-powered) */
    0x2,                                    /* power                */

    /* interface */
    0x09,                                   /* bLength              */
    0x04,                                   /* INTERFACE            */
    0x0,                                    /* bInterfaceNumber     */
    0x0,                                    /* bAlternateSetting    */
    0x02,                                   /* bNumEndpoints        */
    0x0A,                                   /* bInterfaceClass: Data interface code */
    0x0,                                    /* bInterfaceSubClass: Unused*/
    0x0,                                    /* bInterfaceProtocol: No class specific protocol required */
    0x00,                                   /* iInterface           */
    
    /* Endpoint Descriptor  : Bulk-In */
    0x07,                                   /* bLength              */
    0x05,                                   /* ENDPOINT             */
    0x81,                                   /* bEndpointAddress     */
    0x02,                                   /* bmAttributes         */
    0x00, 0x02,                             /* wMaxPacketSize       */
    0x0,                                   /* bInterval            */

    /* Endpoint Descriptor  : Bulk-Out */
    0x07,                                   /* bLength              */
    0x05,                                   /* ENDPOINT             */
    0x02,                                   /* bEndpointAddress     */
    0x02,                                   /* bmAttributes         */
    0x00, 0x02,                             /* wMaxPacketSize       */
    0x00,                                   /* bInterval            */

    /* Interface Descriptor */
    0x09,                                   /* bLength              */
    0x04,                                   /* INTERFACE            */
    0x01,                                    /* bInterfaceNumber     */
    0x00,                                    /* bAlternateSetting    */
    0x01,                                   /* bNumEndpoints        */
    0x02,                                   /* bInterfaceClass: Communication Interface Class */
    0x02,                                   /* bInterfaceSubClass: Abstract Control Model */
    0x01,                                   /* bInterfaceProtocol: Common AT commands (also known as ¡§Hayes" compatible¡¨) */
    0x00,                                   /* iInterface           */

    /* Endpoint Descriptor  : Bulk-In */
    0x07,                                   /* bLength              */
    0x05,                                   /* ENDPOINT             */
    0x83,                                   /* bEndpointAddress     */
    0x03,                                   /* bmAttributes         */
    0x40, 0x00,                             /* wMaxPacketSize       */
    0x01,                                   /* bInterval            */
 };

/** IRP for data reception */
static MUSB_Irp MGC_CdcSyncRxIrp = 
{
    NULL,
    NULL,
    0,
    0,
    0,
    MGC_CdcSyncRxComplete,
    NULL,
    TRUE,	/* bAllowShortTransfer */
    TRUE,	/* bIsrCallback */
    TRUE	/* bAllowDma */
};

static MUSB_Irp MGC_CdcAsyncRxIrp = 
{
    NULL,
    NULL,
    0,
    0,
    0,
    MGC_CdcAsyncRxComplete,
    NULL,
    TRUE,	/* bAllowShortTransfer */
    TRUE,	/* bIsrCallback */
    TRUE	/* bAllowDma */
};

/** IRP for data transmission */
static MUSB_Irp MGC_CdcSyncTxIrp = 
{
    NULL,
    NULL,
    0,
    0,
    0,
    MGC_CdcSyncTxComplete,
    NULL,
    TRUE,	/* bAllowShortTransfer */
    TRUE,	/* bIsrCallback */
    TRUE	/* bAllowDma */
};

/** IRP for data transmission */
static MUSB_Irp MGC_CdcAsyncTxIrp = 
{
    NULL,
    NULL,
    0,
    0,
    0,
    MGC_CdcAsyncTxComplete,
    NULL,
    TRUE,	/* bAllowShortTransfer */
    TRUE,	/* bIsrCallback */
    TRUE	/* bAllowDma */
};

/*
* registration
*/
MUSB_FunctionClient MGC_CdcFunctionClient =
{
    NULL,	/* no instance data; we are singleton */
    (const uint8_t*)MGC_aCdcDescriptorData,
    sizeof(MGC_aCdcDescriptorData),
    3,		/* strings per language */
    MGC_aCdcHighSpeedDescriptorData,
    sizeof(MGC_aCdcHighSpeedDescriptorData),
    sizeof(MGC_aCdcControlData),
    MGC_aCdcControlData,
    &MGC_bCdcSelfPowered,
    MGC_CdcDeviceRequest,
    MGC_CdcDeviceConfigSelected,
    NULL,
    MGC_CdcNewUsbState
};

static MGC_Port *pMGC_CdcPort = NULL;
static MUSB_SystemServices* pMGC_CdcSystemServices = NULL;

/******************************************************************
CDI callbacks
******************************************************************/
static void MGC_CdcNewUsbState(void* hClient, MUSB_BusHandle hBus, 
			       MUSB_State State)
{        
    UNUSED(hClient);
    UNUSED(hBus);
    
    MGC_eCdcUsbState = State;

    if (State == MUSB_SUSPENDED)
    {
        MGC_bCdcControlState = CDCFN_STATE_IDLE;
    }    
    UNUSED(MGC_eCdcUsbState);

    LOG(5, "CDC: State = %d.\n", State);
    /* TODO: anything? */
}

static uint8_t MGC_CdcDeviceRequest(void* hClient, MUSB_BusHandle hBus, 
				    uint32_t dwSequence, const uint8_t* pSetup,
				    uint16_t wLength)
{
    MUSB_DeviceRequest* pRequest = (MUSB_DeviceRequest*)((uint32_t)pSetup);
    uint8_t bOk = FALSE;
    uint16_t i;

    UNUSED(hClient);
    UNUSED(hBus);
    MUSB_ASSERT(pSetup);
    MUSB_ASSERT(pRequest);

    if (MUSB_TYPE_STANDARD == (pRequest->bmRequestType & MUSB_TYPE_MASK))
    {
        LOG(5, "CDC: Standard Request = 0x%X.\n", pRequest->bRequest);
    
	switch(pRequest->bRequest)
	{
	case MUSB_REQ_GET_INTERFACE:
	    (void)MUSB_DeviceResponse(hBus, dwSequence, &MGC_bCdcInterface, 1, FALSE);
	    bOk = TRUE;
	    break;
	case MUSB_REQ_SET_INTERFACE:
	    (void)MUSB_DeviceResponse(hBus, dwSequence, NULL, 0, FALSE);
	    bOk = TRUE;
	    break;	    
	default:
	    // send stall.
	    (void)MUSB_DeviceResponse(hBus, dwSequence, NULL, 0, TRUE);
	    bOk = TRUE;
	    break;	    
	}
    }
    else if(((uint8_t)MUSB_TYPE_CLASS == (pRequest->bmRequestType & MUSB_TYPE_MASK))
	&& (pRequest->bmRequestType & MUSB_RECIP_INTERFACE))
    {
        LOG(5, "CDC: Class Request = 0x%X.\n", pRequest->bRequest);
       
	switch(pRequest->bRequest)
	{
	case USB_ACM_GET_LINE_CODING:
            MGC_bCdcControlState = (uint8_t)((MGC_bCdcControlState == CDCFN_STATE_INIT_2) ? 
                CDCFN_STATE_READY: CDCFN_STATE_INIT_1);

            if (CDCFN_STATE_READY == MGC_bCdcControlState)
            {
                Printf("CDC: PC open this comport ok.\n");
            }
            
	    (void)MUSB_DeviceResponse(hBus, dwSequence, MGC_bCdcLineCoding, 
	    	sizeof(MGC_bCdcLineCoding), FALSE);
	    bOk = TRUE;
	    break;

	case USB_ACM_SET_LINE_CODING:
            MGC_bCdcControlState = (uint8_t)((MGC_bCdcControlState == CDCFN_STATE_IDLE) ? 
                CDCFN_STATE_INIT_2: CDCFN_STATE_INIT_1);
	    
	    MUSB_ASSERT(wLength > 8);    
            for (i=0; i<(wLength-8); i++)
            {
                if (i < (uint16_t)7)
                {
                    MGC_bCdcLineCoding[i] = pSetup[i+8];
                }
            }	   
	    (void)MUSB_DeviceResponse(hBus, dwSequence, NULL, 0, FALSE);            
	    bOk = TRUE;
	    break;
	    
	case USB_ACM_SET_CONTROL_LINE_STATE:
            MGC_bCdcControlState = CDCFN_STATE_IDLE;

	    (void)MUSB_DeviceResponse(hBus, dwSequence, NULL, 0, FALSE);	
	    bOk = TRUE;
	    break;	    
	
	default:	
	    // send stall.	
	    (void)MUSB_DeviceResponse(hBus, dwSequence, NULL, 0, TRUE);	
	    bOk = TRUE;
	    break;	    
	}	     
    }
    else if ((uint8_t)MUSB_TYPE_VENDOR == (pRequest->bmRequestType & MUSB_TYPE_MASK))
    {
        LOG(5, "CDC: Vendor Request = 0x%X.\n", pRequest->bRequest);
    }
    
    return bOk;
}

static void MGC_CdcInit(void)
{
    if (!MGC_dwCdcInitialize)
    {      
#ifndef MUSB_NONEOS    
        VERIFY(OSR_OK == x_sema_create((HANDLE_T *)&MGC_CdcDevice.hTransfer, 
            X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK));

        VERIFY(OSR_OK == x_sema_create((HANDLE_T *)&MGC_CdcDevice.hTransferDone, 
            X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK));        
#endif
       
        MGC_dwCdcInitialize = TRUE;        
    }
}

static uint8_t MGC_CdcDeviceConfigSelected(void* hClient, MUSB_BusHandle hBus, 
					   uint8_t bConfigurationValue,
					   MUSB_Pipe* ahPipe)
{
    UNUSED(hClient);
    MUSB_ASSERT(hBus);
    UNUSED(bConfigurationValue);
    MUSB_ASSERT(ahPipe);
    
    pMGC_CdcPort = (MGC_Port *)hBus;
    MUSB_ASSERT(pMGC_CdcPort->pController);
    pMGC_CdcSystemServices = pMGC_CdcPort->pController->pSystemServices;
    MUSB_ASSERT(pMGC_CdcSystemServices);
    
    // initial pipe.   
    MGC_CdcAsyncTxIrp.hPipe = ahPipe[0];
    MGC_CdcSyncTxIrp.hPipe = ahPipe[0];
    MGC_CdcSyncRxIrp.hPipe = ahPipe[1];
    MGC_CdcAsyncRxIrp.hPipe = ahPipe[1];

    return TRUE;
}

/** data reception callback */
static void MGC_CdcAsyncRxComplete(void* pCompleteParam, const MUSB_Irp* pIrp)
{
    uint32_t dwRxSerial;
    uint32_t dwStatus;
    
    MUSB_ASSERT(pCompleteParam);
    MUSB_ASSERT(pIrp);
   
    if (pIrp->dwActualLength == 0)
    {
        Printf("\nRX = %d, but Null packet.\n", pIrp->dwLength);
        // retry to get the actual data from PC.
        dwStatus = MUSB_StartTransfer((MUSB_Irp *)pIrp);
        if (MUSB_STATUS_OK == dwStatus)
        {
            return;
        }
    }

    dwRxSerial = *((uint32_t *)pCompleteParam);
    LOG(7, "CDC: cb RxSerial = %d.\n", dwRxSerial);
    UNUSED(dwRxSerial);

    MUSB_ASSERT(pIrp->dwLength >= pIrp->dwActualLength);

    MGC_CdcDevice.eRxState = MUSB_CDC_IDLE;

    if (MGC_CdcDevice.pfRxComplete)
    {
        MGC_CdcDevice.pfRxComplete(pIrp->dwActualLength);
    }
}

static void MGC_CdcSyncTxComplete(void* pCompleteParam, const MUSB_Irp* pIrp)
{
#ifndef MUSB_NONEOS    
    int32_t i4_ret;
#endif
    uint32_t dwTxSerial;

    MUSB_ASSERT(pCompleteParam);
    MUSB_ASSERT(pIrp);
    MUSB_ASSERT(pIrp->dwLength >= pIrp->dwActualLength);

    dwTxSerial = *((uint32_t *)pCompleteParam);
    LOG(7, "CDC: cb TxSerial = %d.\n", dwTxSerial);
    UNUSED(dwTxSerial);

    MGC_CdcDevice.eTxState = MUSB_CDC_IDLE;

#ifndef MUSB_NONEOS    
    i4_ret = (int32_t)x_sema_unlock(MGC_CdcDevice.hTransferDone);
    MUSB_ASSERT(OSR_OK == i4_ret);
    UNUSED(i4_ret);
#endif    
       
}

static void MGC_CdcAsyncTxComplete(void* pCompleteParam, const MUSB_Irp* pIrp)
{
    uint32_t dwTxSerial;

    MUSB_ASSERT(pCompleteParam);
    MUSB_ASSERT(pIrp);
    MUSB_ASSERT(pIrp->dwLength >= pIrp->dwActualLength);

    dwTxSerial = *((uint32_t *)pCompleteParam);
    LOG(7, "CDC: cb TxSerial = %d.\n", dwTxSerial);
    UNUSED(dwTxSerial);

    MGC_CdcDevice.eTxState = MUSB_CDC_IDLE;
       
    if (MGC_CdcDevice.pfTxComplete)
    {
        MGC_CdcDevice.pfTxComplete(pIrp->dwActualLength);
    }
}

#ifdef MGC_CDCBUFDATA

static uint32_t MGC_CdcUpdateWriteSpace(void)
{       
    // calculate write buffer space.
    if (MGC_CdcBuf.dwWriteIdx >= MGC_CdcBuf.dwReadIdx)
    {                
        if (MGC_CdcBuf.dwReadIdx == 0)
        {
            // use one byte offset to prevent dwWriteIdx = dwReadIdx.
           MGC_CdcBuf.dwWriteSpace = (MGC_dwCdcBufSize - MGC_CdcBuf.dwWriteIdx) - 1;
        }
        else
        {
            MGC_CdcBuf.dwWriteSpace = MGC_dwCdcBufSize - MGC_CdcBuf.dwWriteIdx;
        }
    }
    else 
    {
        // use one byte offset to prevent dwWriteIdx = dwReadIdx.    
        MUSB_ASSERT((MGC_CdcBuf.dwReadIdx - MGC_CdcBuf.dwWriteIdx) >= 1);
        MGC_CdcBuf.dwWriteSpace = (MGC_CdcBuf.dwReadIdx - MGC_CdcBuf.dwWriteIdx) -1;
    }

    return MGC_CdcBuf.dwWriteSpace;
}

static uint32_t MGC_CdcUpdateWriteIdx(uint32_t dwWriteIdx)
{     
    // increase buffer index when RX IRP finish.
    MGC_CdcBuf.dwWriteIdx += dwWriteIdx;

    // check if dwWriteIdx is need to be wrapped to top.
    if ((MGC_dwCdcBufSize < (MGC_CdcBuf.dwWriteIdx+CDCFN_MAXPACKETSIZE)) &&
         (MGC_CdcBuf.dwReadIdx >= 1))
    {
        MGC_CdcBuf.dwWriteBotIdx = MGC_CdcBuf.dwWriteIdx;
        MGC_CdcBuf.dwWriteIdx = 0;
    }    
    return MGC_CdcBuf.dwWriteIdx;
}

static uint32_t MGC_CdcUpdateReadSpace(uint32_t dwWriteIdx, uint32_t dwWriteBotIdx)
{
    uint32_t dwReadIdx = MGC_CdcBuf.dwReadIdx;
    
    // calculate read buffer space.
    MGC_CdcBuf.dwReadSpace = 0;
    if (dwWriteIdx > dwReadIdx)
    {        
        MGC_CdcBuf.dwReadSpace = dwWriteIdx - dwReadIdx;
    }
    else if (dwWriteIdx < dwReadIdx)
    {
        MUSB_ASSERT(dwReadIdx <= dwWriteBotIdx);
        if (dwWriteBotIdx > dwReadIdx)
        {
            MGC_CdcBuf.dwReadSpace = dwWriteBotIdx - dwReadIdx;
        }
    }
    //else {} dwWriteIdx == dwReadIdx: mean buffer empty.               

    return MGC_CdcBuf.dwReadSpace;
}

static uint32_t MGC_CdcUpdateReadIdx(uint32_t dwReadIdx)
{     
    MGC_CdcBuf.dwReadIdx += dwReadIdx;
    MUSB_ASSERT(MGC_CdcBuf.dwReadIdx <= MGC_CdcBuf.dwWriteBotIdx);

    if (MGC_CdcBuf.dwReadIdx == MGC_CdcBuf.dwWriteBotIdx)
    {
        MGC_CdcBuf.dwReadIdx = 0;
        MGC_CdcBuf.dwWriteBotIdx = MGC_dwCdcBufSize;
    }

    return MGC_CdcBuf.dwReadIdx;
}

static uint32_t MGC_CdcSubmitRxIrp(uint32_t dwWriteIdx, uint32_t dwWriteSpace)
{
    uint32_t dwStatus = MGC_M_STATUS_ERROR;
        
    LOG(7, "SubmitRxIrp: W=%d, I=%d, B=%d.\n", 
        MGC_CdcBuf.dwWriteIdx, MGC_CdcBuf.dwReadIdx, MGC_CdcBuf.dwWriteBotIdx);
    
    // send RX IRP when buffer space > one max packet size.
    if (dwWriteSpace >= CDCFN_MAXPACKETSIZE)
    {
        // Irp dwLength must be in multiple packet size, do not request short packet.
        MGC_CdcSyncRxIrp.dwLength = 
            (dwWriteSpace/CDCFN_MAXPACKETSIZE) * CDCFN_MAXPACKETSIZE;
        MGC_CdcSyncRxIrp.pBuffer = MGC_bCdcBuf + dwWriteIdx;
        dwStatus = MUSB_StartTransfer(&MGC_CdcSyncRxIrp);
        if (dwStatus != 0)
        {
            LOG(7, "Start transfer failed.\n", NULL);
            return FALSE;
        } 
        return TRUE;
    }
    else
    {
        LOG(7, "Write Buffer full.\n", NULL);
        return FALSE;
    }
}

static void MGC_CdcSyncRxComplete(void *pCompleteParam, const MUSB_Irp *pIrp)
{
#ifndef MUSB_NONEOS
    int32_t i4_ret;
#endif
    uint32_t dwStatus = MGC_M_STATUS_ERROR;

    UNUSED(pCompleteParam);
    MUSB_ASSERT(pIrp);
    
    if (pIrp->dwStatus == MUSB_STATUS_PEER_REMOVED)
    {
        LOG(7, "CDC: device remove !\n", NULL);
    }

    MUSB_DIAG1(3, "CDC: complete data len=", pIrp->dwActualLength, 16, 8);

#ifndef MUSB_NONEOS
    if (MGC_CdcDevice.hTransferDone)
    {
        i4_ret = x_sema_unlock(MGC_CdcDevice.hTransferDone);
        MUSB_ASSERT(OSR_OK == i4_ret);
        UNUSED(i4_ret);
    }        
#endif

    (void)MGC_CdcUpdateWriteIdx(pIrp->dwActualLength);

    // calculate write buffer space.
    if (MGC_CdcUpdateWriteSpace() >= CDCFN_MAXPACKETSIZE)
    {
        dwStatus = MGC_CdcSubmitRxIrp(MGC_CdcBuf.dwWriteIdx, MGC_CdcBuf.dwWriteSpace);
        MUSB_ASSERT(dwStatus);
        UNUSED(dwStatus);
    }
}

#else

static void MGC_CdcSyncRxComplete(void *pCompleteParam, const MUSB_Irp *pIrp)
{
#ifndef MUSB_NONEOS    
    int32_t i4_ret;
#endif
    uint32_t dwRxSerial;
    uint32_t dwStatus;

    MUSB_ASSERT(pCompleteParam);
    MUSB_ASSERT(pIrp);
   
    if (pIrp->dwActualLength == 0)
    {
        Printf("\nRX = %d, but Null packet.\n", pIrp->dwLength);
        // retry to get the actual data from PC.
        dwStatus = MUSB_StartTransfer((MUSB_Irp *)pIrp);
        if (MUSB_STATUS_OK == dwStatus)
        {
            return;
        }
    }

    dwRxSerial = *((uint32_t *)pCompleteParam);
    LOG(7, "CDC: cb RxSerial = %d.\n", dwRxSerial);
    UNUSED(dwRxSerial);

    MUSB_ASSERT(pIrp->dwLength >= pIrp->dwActualLength);

#ifndef MUSB_NONEOS    
    i4_ret = (int32_t)x_sema_unlock(MGC_CdcDevice.hTransferDone);
    MUSB_ASSERT(OSR_OK == i4_ret);
    UNUSED(i4_ret);
#endif    
}

#endif /* #ifdef MGC_CDCBUFDATA */

//-------------------------------------------------------------------------
/** MUSB_CdcInit
 *  USB device CDC mode initialization function.
 *  @param  void                    user's buffer.
 *  @retval  void.
 */
//-------------------------------------------------------------------------
void MUSB_CdcInit(void)
{
    MGC_CdcInit();
}

//-------------------------------------------------------------------------
/** MUSB_CdcConfigOK
 *  USB device CDC mode configuration function.
 *  @param  void                    user's buffer.
 *  @retval  0  Success.
 *  @retval  -1	FAIL. 
 */
//-------------------------------------------------------------------------
INT32 MUSB_CdcConfigOK(void)
{
    if ((!MGC_dwCdcInitialize) ||
        (MUSB_CONFIGURED != MGC_eCdcUsbState) ||
        (pMGC_CdcSystemServices == NULL))
    {
        return -1;
    }

    return 0;
}

//-------------------------------------------------------------------------
/** MUSB_CdcSyncTx
 *  USB device CDC mode sync tx function.
 *  @param  pBuf                    user's buffer.
 *  @param  dwCount             user's buffer size.
 *  @retval  >0 Actual Tx data length.
 *  @retval  -1	FAIL. 
 */
//-------------------------------------------------------------------------
INT32 MUSB_CdcSyncTx(UINT8 *pBuf, UINT32 dwCount)
{
#ifndef MUSB_NONEOS
    int32_t i4_ret;
#endif
    uint32_t dwStatus = MGC_M_STATUS_ERROR;
    uint32_t dwIntFlag = 0;

    if ((!pBuf) ||
         (dwCount == 0)  || 
         (0 != MUSB_CdcConfigOK()))
    {
        return -1;
    }

    // fix warning.
    UNUSED(pBuf);

#ifndef MUSB_NONEOS    
    /* lock out other transfers */
    i4_ret = (int32_t)x_sema_lock(MGC_CdcDevice.hTransfer, X_SEMA_OPTION_WAIT);

    // Make sure hTransferDone semaphore in lock state.
    i4_ret = (int32_t)x_sema_lock(MGC_CdcDevice.hTransferDone, X_SEMA_OPTION_NOWAIT);
    MUSB_ASSERT((i4_ret == OSR_OK) || (i4_ret == OSR_WOULD_BLOCK));        
#endif
    MUSB_ASSERT(MGC_CdcDevice.eTxState == MUSB_CDC_IDLE);
    MGC_CdcDevice.eTxState = MUSB_CDC_SYNCTX;
    
    MGC_CdcSyncTxIrp.pBuffer = pBuf;
    MGC_CdcSyncTxIrp.dwLength = dwCount;
    MGC_CdcSyncTxIrp.dwStatus = 0;
    MGC_CdcSyncTxIrp.dwActualLength = 0;

    // increase Tx serial number.
    MGC_CdcDevice.dwTxSerial ++;
    MGC_CdcSyncTxIrp.pCompleteParam = &MGC_CdcDevice.dwTxSerial;
    LOG(7, "CDC: TxSerial = %d.\n", MGC_CdcDevice.dwTxSerial);

    // prevent to race condition with device HISR.   
    dwIntFlag = pMGC_CdcSystemServices->pfIrqLock(
        pMGC_CdcSystemServices->pPrivateData);
    
    dwStatus = MUSB_StartTransfer(&MGC_CdcSyncTxIrp);

    pMGC_CdcSystemServices->pfIrqUnlock(
        pMGC_CdcSystemServices->pPrivateData, dwIntFlag);

#ifndef MUSB_NONEOS       
    /* pend on semaphore for completion */
    if (dwStatus == MUSB_STATUS_OK)
    {    
        i4_ret = (int32_t)x_sema_lock(MGC_CdcDevice.hTransferDone, X_SEMA_OPTION_WAIT);
        MUSB_ASSERT(OSR_OK == i4_ret);
        MUSB_ASSERT(dwCount >= MGC_CdcSyncRxIrp.dwActualLength);
        dwCount = MGC_CdcSyncTxIrp.dwActualLength;
    }

    MGC_CdcDevice.eTxState = MUSB_CDC_IDLE;
   
    /* allow another transfer */
    i4_ret = (int32_t)x_sema_unlock(MGC_CdcDevice.hTransfer);
    MUSB_ASSERT(OSR_OK == i4_ret);
    UNUSED(i4_ret);
#endif

    return (INT32)((MUSB_STATUS_OK != dwStatus) ? (-1) : ((int32_t)dwCount));    
}

#if 0
int32_t MUSB_CdcSyncTx(uint8_t *pBuf, uint32_t dwCount)
{
#ifndef MUSB_NONEOS
    int32_t i4_ret;
#endif
    MGC_Pipe *pPipe;
    MGC_Port *pPort;
    MGC_EndpointResource *pEnd;
   
    if ((!pBuf) ||
         (dwCount == 0)  || 
         (0 != MUSB_CdcConfigOK()) ||
         /*(CDCFN_STATE_READY != MGC_bCdcControlState) ||*/
         (NULL == MGC_CdcDevice.hTxPipe))
    {
        return -1;
    }
    
    // fix warning.
    UNUSED(pBuf);

    pPipe = (MGC_Pipe *)MGC_CdcDevice.hTxPipe;

#ifndef MUSB_NONEOS    
    /* lock out other transfers */
    i4_ret = x_sema_lock(MGC_CdcDevice.hTransfer, X_SEMA_OPTION_WAIT);
#endif
    MUSB_ASSERT(MGC_CdcDevice.eTxState == MUSB_CDC_IDLE);
    MGC_CdcDevice.eTxState = MUSB_CDC_SYNCTX;
        
    pPort = (MGC_Port *)pPipe->hSession;
    pEnd = pPipe->pLocalEnd;

    if (!MGC_DevPollTx(pPort, pEnd, pBuf, dwCount))
    {
        // tx error.
        dwCount = 0;
    }

    MGC_CdcDevice.eTxState = MUSB_CDC_IDLE;
    
#ifndef MUSB_NONEOS    
    /* allow another transfer */
    if (OSR_OK == i4_ret)  /* Prevent Unlock semaphore in HISR. */
    {
        i4_ret = x_sema_unlock(MGC_CdcDevice.hTransfer);
        MUSB_ASSERT(OSR_OK == i4_ret);
    }
#endif
       
    return (int32_t)dwCount;    
}

#endif

//-------------------------------------------------------------------------
/** MUSB_CdcAsyncTx
 *  USB device CDC mode async tx function.
 *  @param  pBuf                    user's buffer.
 *  @param  dwCount             user's buffer size.
 *  @param  pfXferComplete     user's call back function. 
 *  @retval  0 	SUCCESS.
 *  @retval  -1	FAIL. 
 */
//-------------------------------------------------------------------------
INT32 MUSB_CdcAsyncTx(UINT8 *pBuf, UINT32 dwCount, 
    MUSB_pfCdcTransferComplete pfXferComplete)    
{
#ifndef MUSB_NONEOS
    int32_t i4_ret;
#endif
    uint32_t dwStatus = MGC_M_STATUS_ERROR;
    uint32_t dwIntFlag = 0;

    if ((!pBuf) ||
         (dwCount == 0)  || 
         (pfXferComplete == NULL)  ||          
         (0 != MUSB_CdcConfigOK()))
    {
        return -1;
    }

    // fix warning.
    UNUSED(pBuf);

#ifndef MUSB_NONEOS    
    /* lock out other transfers */
    i4_ret = (int32_t)x_sema_lock(MGC_CdcDevice.hTransfer, X_SEMA_OPTION_WAIT);
#endif
    MUSB_ASSERT(MGC_CdcDevice.eTxState == MUSB_CDC_IDLE);
    MGC_CdcDevice.eTxState = MUSB_CDC_ASYNCTX;

    MGC_CdcDevice.pfTxComplete = pfXferComplete;
    
    MGC_CdcAsyncTxIrp.pBuffer = pBuf;
    MGC_CdcAsyncTxIrp.dwLength = dwCount;
    MGC_CdcAsyncTxIrp.dwStatus = 0;
    MGC_CdcAsyncTxIrp.dwActualLength = 0;

    // increase Tx serial number.
    MGC_CdcDevice.dwTxSerial ++;
    MGC_CdcAsyncTxIrp.pCompleteParam = &MGC_CdcDevice.dwTxSerial;
    LOG(7, "CDC: TxSerial = %d.\n", MGC_CdcDevice.dwTxSerial);

    // prevent to race condition with device HISR.   
    dwIntFlag = pMGC_CdcSystemServices->pfIrqLock(
        pMGC_CdcSystemServices->pPrivateData);
    
    dwStatus = MUSB_StartTransfer(&MGC_CdcAsyncTxIrp);

    pMGC_CdcSystemServices->pfIrqUnlock(
        pMGC_CdcSystemServices->pPrivateData, dwIntFlag);

#ifndef MUSB_NONEOS          
    /* allow another transfer */
    if (OSR_OK == i4_ret)  /* Prevent Unlock semaphore in HISR. */
    {
        i4_ret = (int32_t)x_sema_unlock(MGC_CdcDevice.hTransfer);
        MUSB_ASSERT(OSR_OK == i4_ret);
    }
#endif

    return (INT32)((MUSB_STATUS_OK != dwStatus) ? (-1) : (0));
}

#ifdef MGC_CDCBUFDATA

//-------------------------------------------------------------------------
/** MUSB_CdcSyncRx
 *  USB device CDC mode sync rx function.
 *  @param  pBuf                    user's buffer.
 *  @param  dwCount             user's buffer size.
 *  @retval  >0 Actual Rx data length.
 *  @retval  -1	FAIL. 
 */
//-------------------------------------------------------------------------
int32_t MUSB_CdcSyncRx(uint8_t *pBuf, uint32_t dwCount)
{
#ifndef MUSB_NONEOS
    int32_t i4_ret;
#endif
    uint32_t dwBufLen = 0;
    uint32_t dwRxCount = dwCount;
    const VOID *pv_from;
    uint32_t dwWriteIdx = 0;
    uint32_t dwReadIdx = MGC_CdcBuf.dwReadIdx;    
    uint32_t dwWriteBotIdx = 0;    
    uint32_t dwWriteSpace = 0;    
    uint32_t dwNewWriteSpace = 0;    
    uint32_t dwIntFlag = 0;
    
    if ((!pBuf) ||
         (dwCount == 0)  || 
         (0 != MUSB_CdcConfigOK()))
    {
        return -1;
    }

#ifndef MUSB_NONEOS       
    /* lock out other transfers */
    i4_ret = (int32_t)x_sema_lock(MGC_CdcDevice.hTransfer, X_SEMA_OPTION_WAIT);
    MUSB_ASSERT(OSR_OK == i4_ret);

    // Make sure hTransferDone semaphore in lock state.
    i4_ret = x_sema_lock(MGC_CdcDevice.hTransferDone, X_SEMA_OPTION_NOWAIT);
    MUSB_ASSERT((i4_ret == OSR_OK) || (i4_ret == OSR_WOULD_BLOCK));        
#endif

    while (dwRxCount > 0)
    {
        dwIntFlag = pMGC_CdcSystemServices->pfIrqLock(
            pMGC_CdcSystemServices->pPrivateData);
        
        dwWriteIdx = MGC_CdcBuf.dwWriteIdx;
        dwWriteBotIdx = MGC_CdcBuf.dwWriteBotIdx;
        dwWriteSpace = MGC_CdcBuf.dwWriteSpace;
        
        pMGC_CdcSystemServices->pfIrqUnlock(
            pMGC_CdcSystemServices->pPrivateData, dwIntFlag);
        
        LOG(7, "RxData: W=%d, I=%d, B=%d.\n", dwWriteIdx, dwReadIdx, dwWriteBotIdx);

        dwBufLen = MGC_CdcUpdateReadSpace(dwWriteIdx, dwWriteBotIdx);
        dwBufLen = MIN(dwBufLen, dwRxCount);

        if (dwBufLen > 0)
        {
            // copy data from CDC RX buffer to user's buffer.
            pv_from = (const VOID *)(MGC_bCdcBuf + dwReadIdx);
            x_memcpy((VOID *)pBuf, pv_from, dwBufLen);
            pBuf += dwBufLen;
            dwRxCount -= dwBufLen;            
        }

        dwIntFlag = pMGC_CdcSystemServices->pfIrqLock(
            pMGC_CdcSystemServices->pPrivateData);

        // update read index.
        dwReadIdx = MGC_CdcUpdateReadIdx(dwBufLen);         
        // because dwWriteIdx/dwWriteBotIdx depend on dwReadIdx, recheck and update.
        (void)MGC_CdcUpdateWriteIdx(0);
        // because dwReadIdx depend on dwWriteBotIdx, recheck and update.
        dwReadIdx = MGC_CdcUpdateReadIdx(0);            
        // get new update space.
        dwNewWriteSpace = MGC_CdcUpdateWriteSpace();       
        
        pMGC_CdcSystemServices->pfIrqUnlock(
            pMGC_CdcSystemServices->pPrivateData, dwIntFlag);

        // reactivate next transfer.
        if ((dwWriteSpace < CDCFN_MAXPACKETSIZE) && (dwNewWriteSpace >= CDCFN_MAXPACKETSIZE))
        {
            if (!MGC_CdcSubmitRxIrp(dwWriteIdx, dwNewWriteSpace))
            {
                LOG(1, "MGC_CdcRxData:fail tx irp.\n", NULL);
                MUSB_ASSERT(0);
            }                                
            else
            {
                LOG(7, "MGC_CdcRxData:reactivate next transfer.\n", NULL);
            }
        }

#ifndef MUSB_NONEOS       
        // no more data to read. wait rx pkt coming.
        if ((dwBufLen == 0) && (dwRxCount > 0))
        {            
            LOG(7, "MGC_CdcRxData:wait data.\n", NULL);
            i4_ret = x_sema_lock(MGC_CdcDevice.hTransferDone, X_SEMA_OPTION_WAIT);
            MUSB_ASSERT(OSR_OK == i4_ret);            
        }
#endif
    }

#ifndef MUSB_NONEOS       
    /* allow another transfer */
    i4_ret = (int32_t)x_sema_unlock(MGC_CdcDevice.hTransfer);
    MUSB_ASSERT(OSR_OK == i4_ret);
    UNUSED(i4_ret);
#endif

    return (int32_t)dwCount;
}

#else

//-------------------------------------------------------------------------
/** MUSB_CdcSyncRx
 *  USB device CDC mode sync rx function.
 *  @param  pBuf                    user's buffer.
 *  @param  dwCount             user's buffer size.
 *  @retval  >0 Actual Rx data length.
 *  @retval  -1	FAIL. 
 */
//-------------------------------------------------------------------------
INT32 MUSB_CdcSyncRx(UINT8 *pBuf, UINT32 dwCount)
{
#ifndef MUSB_NONEOS
    int32_t i4_ret;
#endif
    uint32_t dwStatus = MGC_M_STATUS_ERROR;
    uint32_t dwIntFlag = 0;

    if ((!pBuf) ||
         (dwCount == 0)  || 
         (0 != MUSB_CdcConfigOK()))
    {
        return -1;
    }

#ifndef MUSB_NONEOS    
    /* lock out other transfers */
    i4_ret = (int32_t)x_sema_lock(MGC_CdcDevice.hTransfer, X_SEMA_OPTION_WAIT);
    MUSB_ASSERT(i4_ret == OSR_OK);

    // Make sure hTransferDone semaphore in lock state.
    i4_ret = (int32_t)x_sema_lock(MGC_CdcDevice.hTransferDone, X_SEMA_OPTION_NOWAIT);
    MUSB_ASSERT((i4_ret == OSR_OK) || (i4_ret == OSR_WOULD_BLOCK));        
#endif

    MUSB_ASSERT(MGC_CdcDevice.eRxState == MUSB_CDC_IDLE);
    MGC_CdcDevice.eRxState = MUSB_CDC_SYNCRX;

    MGC_CdcSyncRxIrp.pBuffer = pBuf;
    MGC_CdcSyncRxIrp.dwLength = dwCount;
    MGC_CdcSyncRxIrp.dwStatus = 0;
    MGC_CdcSyncRxIrp.dwActualLength = 0L;

    // increase rx serial number.
    MGC_CdcDevice.dwRxSerial ++;
    MGC_CdcSyncRxIrp.pCompleteParam = &MGC_CdcDevice.dwRxSerial;
    LOG(7, "CDC: RxSerial = %d.\n", MGC_CdcDevice.dwRxSerial);

    // prevent to race condition with device HISR.   
    dwIntFlag = pMGC_CdcSystemServices->pfIrqLock(
        pMGC_CdcSystemServices->pPrivateData);
    
    dwStatus = MUSB_StartTransfer(&MGC_CdcSyncRxIrp);

    pMGC_CdcSystemServices->pfIrqUnlock(
        pMGC_CdcSystemServices->pPrivateData, dwIntFlag);

#ifndef MUSB_NONEOS       
    /* pend on semaphore for completion */
    if (dwStatus == MUSB_STATUS_OK)
    {    
        i4_ret = (int32_t)x_sema_lock(MGC_CdcDevice.hTransferDone, X_SEMA_OPTION_WAIT);
        MUSB_ASSERT(OSR_OK == i4_ret);
        MUSB_ASSERT(dwCount >= MGC_CdcSyncRxIrp.dwActualLength);
        dwCount = MGC_CdcSyncRxIrp.dwActualLength;
    }

    MGC_CdcDevice.eRxState = MUSB_CDC_IDLE;
   
    /* allow another transfer */
    i4_ret = (int32_t)x_sema_unlock(MGC_CdcDevice.hTransfer);
    MUSB_ASSERT(OSR_OK == i4_ret);
    UNUSED(i4_ret);
#endif

    return (INT32)((MUSB_STATUS_OK != dwStatus) ? (-1) : ((int32_t)dwCount));    
}
#endif

//-------------------------------------------------------------------------
/** MUSB_CdcAsyncRx
 *  USB device CDC mode async rx function.
 *  @param  pBuf                    user's buffer.
 *  @param  dwCount             user's buffer size.
 *  @param  pfXferComplete     user's call back function.
 *  @retval  0 	SUCCESS.
 *  @retval  -1	FAIL. 
 */
//-------------------------------------------------------------------------
INT32 MUSB_CdcAsyncRx(UINT8 *pBuf, UINT32 dwCount, 
    MUSB_pfCdcTransferComplete pfXferComplete)
{
#ifndef MUSB_NONEOS
    int32_t i4_ret;
#endif
    uint32_t dwStatus = MGC_M_STATUS_ERROR;
    uint32_t dwIntFlag = 0;

    if ((!pBuf) ||
         (dwCount == 0)  || 
         (pfXferComplete == NULL)  || 
         (0 != MUSB_CdcConfigOK()))
    {
        return -1;
    }

#ifndef MUSB_NONEOS    
    /* lock out other transfers */
    i4_ret = (int32_t)x_sema_lock(MGC_CdcDevice.hTransfer, X_SEMA_OPTION_WAIT);
#endif
    MUSB_ASSERT(MGC_CdcDevice.eRxState == MUSB_CDC_IDLE);
    MGC_CdcDevice.eRxState = MUSB_CDC_ASYNCRX;

    MGC_CdcDevice.pfRxComplete = pfXferComplete;

    MGC_CdcAsyncRxIrp.pBuffer = pBuf;
    MGC_CdcAsyncRxIrp.dwLength = dwCount;
    MGC_CdcAsyncRxIrp.dwStatus = 0;
    MGC_CdcAsyncRxIrp.dwActualLength = 0L;

    // increase rx serial number.
    MGC_CdcDevice.dwRxSerial ++;
    MGC_CdcAsyncRxIrp.pCompleteParam = &MGC_CdcDevice.dwRxSerial;
    LOG(7, "CDC: RxSerial = %d.\n", MGC_CdcDevice.dwRxSerial);

    // prevent to race condition with device HISR.   
    dwIntFlag = pMGC_CdcSystemServices->pfIrqLock(
        pMGC_CdcSystemServices->pPrivateData);
    
    dwStatus = MUSB_StartTransfer(&MGC_CdcAsyncRxIrp);

    pMGC_CdcSystemServices->pfIrqUnlock(
        pMGC_CdcSystemServices->pPrivateData, dwIntFlag);

#ifndef MUSB_NONEOS          
    /* allow another transfer */
    if (OSR_OK == i4_ret)  /* Prevent Unlock semaphore in HISR. */
    {
        i4_ret = (int32_t)x_sema_unlock(MGC_CdcDevice.hTransfer);
        MUSB_ASSERT(OSR_OK == i4_ret);
    }
#endif

    return (INT32)((MUSB_STATUS_OK != dwStatus) ? (-1) : (0));    
}

#endif /* MUSB_FUNCTION */
