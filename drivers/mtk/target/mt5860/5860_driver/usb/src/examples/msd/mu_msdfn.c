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
 * Sample SCSI disk peripheral
 * $Revision: #1 $
 */
#ifdef MUSB_FUNCTION

#include "mu_cdi.h"
#include "mu_diag.h"
#include "mu_mem.h"
#include "class/mu_msd.h"
#include "class/mu_bot.h"
#include "class/mu_scsi.h"

LINT_SAVE_AND_DISABLE
/**
 * You may define this if your core is configured for a bidirectional endpoint 1
 */
//#define MGC_MSD_BIDIR_ENDPOINTS

#ifdef MUSB_DIAG
#define STATIC
#else
#define STATIC static
#endif

/**
 * MGC_MsdCommandUsage.
 * For table lookup of command usage data
 * @field bCommand command
 * @field bDataSize size of buffer
 * @field pData buffer
 */
typedef struct
{
    uint8_t bCommand;
    uint8_t bDataSize;
    const uint8_t* pData;
} MGC_MsdCommandUsage;

#if 0
#include "mu_pkon.h"
/**
 * MGC_MsdRbcParams.
 * For static declaration of RBC mode parameters page
 */
typedef struct
{
    const MGC_MsdModeParamHeader6 Header6;
    MGC_MsdRbcDeviceParams RbcParams;
} MGC_MsdRbcParams;
#include "mu_pkoff.h"
#endif
/******************************************************************
Forwards
******************************************************************/
STATIC uint8_t MGC_MsdDeviceRequest(void* hClient, MUSB_BusHandle hBus, 
				    uint32_t dwSequenceNumber, 
				    const uint8_t* pSetup, 
				    uint16_t wRequestLength);
STATIC uint8_t MGC_MsdDeviceConfigSelected(void* hClient, 
					   MUSB_BusHandle hBus, 
					   uint8_t bConfigurationValue, 
					   MUSB_Pipe* ahPipe);
STATIC void MGC_MsdNewUsbState(void* hClient, MUSB_BusHandle hBus, 
			       MUSB_State State);

STATIC void MGC_MsdRxCbwComplete(void* pCompleteParam, const MUSB_Irp* pIrp);
STATIC void MGC_MsdRxDataComplete(void* pCompleteParam, const MUSB_Irp* pIrp);

STATIC void MGC_MsdTxDataComplete(void* pCompleteParam, const MUSB_Irp* pIrp);
STATIC void MGC_MsdTxCswComplete(void* pCompleteParam, const MUSB_Irp* pIrp);

/******************************************************************
Globals
******************************************************************/

/* UCDI variables */
static uint8_t MGC_bMsdSelfPowered = TRUE;
static MUSB_State MGC_eMsdUsbState = MUSB_POWER_OFF;

static uint8_t MGC_aControlData[256];

static const char MGC_aDescriptorData[] = 
{
    /* Device Descriptor */
    0x12,                      /* bLength              */
    MUSB_DT_DEVICE,            /* DEVICE               */
    0x00,0x02,                 /* USB 2.0              */
    0x00,                      /* CLASS                */
    0x00,                      /* Subclass             */
    0x00,                      /* Protocol             */
    0x40,                      /* bMaxPktSize0         */
    0x8D,0x0E,                 /* idVendor: MediaTek   */
    0x02,0x00,                 /* idProduct: number 3  */
    0x00,0x02,                 /* bcdDevice            */
    0x01,                      /* iManufacturer        */
    0x02,                      /* iProduct             */
    0x03,                      /* iSerial Number       */
    0x01,                      /* One configuration    */

    /* strings */
    2+4,
    MUSB_DT_STRING,
    0x09, 0x04,			/* English (U.S.) */
    0x09, 0x0c,			/* English (Australia) */

    /* TODO: make tool to generate strings and eventually whole descriptor! */
    /* English (U.S.) strings */
    2+24,			/* Manufacturer: MediaTek Inc */
    MUSB_DT_STRING,
    'M', 0, 'e', 0, 'd', 0, 'i', 0, 'a', 0, 'T', 0, 'e', 0,
    'k', 0, ' ', 0, 'I', 0, 'n', 0, 'c', 0,

    2+18,			/* Product ID: 5381 Disk */
    MUSB_DT_STRING,
    '5', 0, '3', 0, '8', 0, '1', 0, ' ', 0, 'D', 0, 'i', 0, 's', 0, 'k', 0,

    2+24,			/* Serial #: SA3 MTK1321  */
    MUSB_DT_STRING,
    'S', 0, 'A', 0, '3', 0, ' ', 0,
    'M', 0, 'T', 0, 'K', 0, '1', 0,
    '3', 0, '2', 0, '1', 0, ' ', 0,

    /* English (Australia) strings */
    2+24,			/* Manufacturer: MediaTek Inc */
    MUSB_DT_STRING,
    'M', 0, 'e', 0, 'd', 0, 'i', 0, 'a', 0, 'T', 0, 'e', 0,
    'k', 0, ' ', 0, 'I', 0, 'n', 0, 'c', 0,

    2+18,			/* Product ID: 5381 Disk */
    MUSB_DT_STRING,
    '5', 0, '3', 0, '8', 0, '1', 0, ' ', 0, 'D', 0, 'i', 0, 's', 0, 'k', 0,

    2+24,			/* Serial #: SA3 MTK1321  */
    MUSB_DT_STRING,
    'S', 0, 'A', 0, '3', 0, ' ', 0,
    'M', 0, 'T', 0, 'K', 0, '1', 0,
    '3', 0, '2', 0, '1', 0, ' ', 0,

    /* configuration */
    0x09,                                   /* bLength              */
    0x02,                                   /* CONFIGURATION        */
//  MTK Notice: Max Liao, 2006/12/01.
//  disable otg function in device mode.        
//    0x23,                                   /* length               */
    0x20,
    0x0,                                    /* length               */
    0x01,                                   /* bNumInterfaces       */
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
    0x08,                                   /* bInterfaceClass      */
    0x06,                                   /* bInterfaceSubClass (1=RBC, 6=SCSI) */
    0x50,                                   /* bInterfaceProtocol (BOT) */
    0x00,                                   /* iInterface           */

    /* Endpoint Descriptor  : Bulk-In */
    0x07,                                   /* bLength              */
    0x05,                                   /* ENDPOINT             */
    0x81,                                   /* bEndpointAddress     */
    0x02,                                   /* bmAttributes         */
    0x40, 0x00,                             /* wMaxPacketSize       */
    0x10,                                   /* bInterval            */

    /* Endpoint Descriptor  : Bulk-Out */
    0x07,                                   /* bLength              */
    0x05,                                   /* ENDPOINT             */
#ifdef MGC_MSD_BIDIR_ENDPOINTS
    0x01,                                   /* bEndpointAddress     */
#else
    0x02,                                   /* bEndpointAddress     */
#endif
    0x02,                                   /* bmAttributes         */
    0x40, 0x00,                             /* wMaxPacketSize       */
    0x10                                    /* bInterval            */

//  MTK Notice: Max Liao, 2006/12/01.
//  disable otg function in device mode.
#if 0
    /* OTG */
    MUSB_DT_OTG_SIZE,
    MUSB_DT_OTG,
    MUSB_OTG_SRP | MUSB_OTG_HNP
#endif    
};

static const uint8_t MGC_aHighSpeedDescriptorData[] = 
{
    /* device qualifier */
    0x0a,                      /* bLength              */
    MUSB_DT_DEVICE_QUALIFIER,  /* DEVICE Qualifier     */
    0x00,0x02,                 /* USB 2.0              */
    0,                         /* CLASS                */
    0,                         /* Subclass             */
    0x00,                      /* Protocol             */
    0x40,                      /* bMaxPacketSize0      */
    0x01,                      /* One configuration    */
    0x00,                      /* bReserved            */

    /* configuration */
    0x09,                                   /* bLength              */
    MUSB_DT_OTHER_SPEED,                    /* CONFIGURATION        */
//  MTK Notice: Max Liao, 2006/12/01.
//  disable otg function in device mode.    
//    0x23,                                   /* length               */
    0x20,
    0x0,                                    /* length               */
    0x01,                                   /* bNumInterfaces       */
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
    0x08,                                   /* bInterfaceClass      */
    0x06,                                   /* bInterfaceSubClass (1=RBC, 6=SCSI) */
    0x50,                                   /* bInterfaceProtocol (BOT) */
    0x00,                                   /* iInterface           */

    /* Endpoint Descriptor  : Bulk-In */
    0x07,                                   /* bLength              */
    0x05,                                   /* ENDPOINT             */
    0x81,                                   /* bEndpointAddress     */
    0x02,                                   /* bmAttributes         */
    0x00, 0x02,                             /* wMaxPacketSize       */
    0x04,                                   /* bInterval            */

    /* Endpoint Descriptor  : Bulk-Out */
    0x07,                                   /* bLength              */
    0x05,                                   /* ENDPOINT             */
#ifdef MGC_MSD_BIDIR_ENDPOINTS
    0x01,                                   /* bEndpointAddress     */
#else
    0x02,                                   /* bEndpointAddress     */
#endif
    0x02,                                   /* bmAttributes         */
    0x00, 0x02,                             /* wMaxPacketSize       */
    0x04                                   /* bInterval            */

//  MTK Notice: Max Liao, 2006/12/01.
//  disable otg function in device mode.
#if 0
    /* OTG */
    MUSB_DT_OTG_SIZE,
    MUSB_DT_OTG,
    MUSB_OTG_SRP | MUSB_OTG_HNP
#endif    
};

static const MGC_MsdStandardInquiryData MGC_MsdRbcInquiryData =
{
    MGC_SCSI_DEVICE_TYPE_DIRECT,
    0x80,   /* removable (though this doesn't seem so) */
    0,	    /* 0=no comformance to any standard */
    0,	    /* 2=required response data format */
    0x1f,	    /* extra length */
    {0, 0, 0},	    /* flags */
    { 'M', 'e', 'd', 'i', 'a', 'T', 'e', 'k' },
    { '5' ,'3', '8', '1', ' ', 'D', 'i', 's', 'k', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
    { '1', ' ', ' ', ' ' }
};

#if 0
static MGC_MsdRbcParams MGC_RbcModeParams =
{
    { (uint8_t)16/*(sizeof(MGC_MsdRbcParams) - 1)*/, 0, 0, 0 },
    {
	0x86,	/* page code */
	0x0b,	/* length */
	0,	/* WCD */
	0,	/* block size (run-time) */
	0,	/* block count (run-time) */
	0,	/* power vs. performance */
	MGC_M_MSD_RBC_FORMATD,	/* READD/WRITED/FORMATD/LOCKD */
	0
    }
};
#endif

static const uint8_t MGC_MsdSupportedVpdPagesData[] = 
{
    MGC_SCSI_DEVICE_TYPE_DIRECT,   /* device type */
    MGC_SCSI_PAGE_SUPPORTED_VPD,    /* page code */
    0,	    /* reserved */
    3,	    /* length of: */
    MGC_SCSI_PAGE_SUPPORTED_VPD,
    MGC_SCSI_PAGE_UNIT_SERIAL_NUM,
    MGC_SCSI_PAGE_DEVICE_ID
};

static const char MGC_MsdUnitSerialNumberPageData[] =
{
    MGC_SCSI_DEVICE_TYPE_DIRECT,   /* device type */
    MGC_SCSI_PAGE_UNIT_SERIAL_NUM,  /* page code */
    0,	    /* reserved */
    4,	    /* length of: */
    '1', '2', 'f', 'e'
};

static const uint8_t MGC_MsdDeviceIdPageData[] =
{
    MGC_SCSI_DEVICE_TYPE_DIRECT,   /* device type */
    MGC_SCSI_PAGE_DEVICE_ID,	    /* page code */
    0,	    /* reserved */
    0	    /* length of: */
};

static const uint8_t MGC_aUnsupportedCommandData[] =
{
    MGC_SCSI_DEVICE_TYPE_DIRECT,   /* device type */
    1,	    /* unsupported */
    0,
    0,
    0,
    0
};

static const uint8_t MGC_aRead10Support[] =
{
    MGC_SCSI_DEVICE_TYPE_DIRECT,   /* device type */
    5,	    /* supported, vendor-specific */
    0,	    /* no compliance claimed */
    0, 0,
    10,
    MGC_SCSI_READ10,
    0,
    0xff, 0xff, 0xff, 0xff,
    0,
    0xff, 0xff,
    0
};

static const uint8_t MGC_aWrite10Support[] =
{
    MGC_SCSI_DEVICE_TYPE_DIRECT,   /* device type */
    5,	    /* supported, vendor-specific */
    0,	    /* no compliance claimed */
    0, 0,
    10,
    MGC_SCSI_WRITE10,
    0,
    0xff, 0xff, 0xff, 0xff,
    0,
    0xff, 0xff,
    0
};

static const uint8_t MGC_aVerifySupport[] =
{
    MGC_SCSI_DEVICE_TYPE_DIRECT,   /* device type */
    5,	    /* supported, vendor-specific */
    0,	    /* no compliance claimed */
    0, 0,
    10,
    MGC_SCSI_VERIFY,
    0,
    0xff, 0xff, 0xff, 0xff,
    0,
    0xff, 0xff,
    0
};

static const uint8_t MGC_aWriteBufferSupport[] =
{
    MGC_SCSI_DEVICE_TYPE_DIRECT,   /* device type */
    5,	    /* supported, vendor-specific */
    0,	    /* no compliance claimed */
    0, 0,
    10,
    MGC_SCSI_WRITE_BUFFER,
    7,
    0xff, 0xff, 0xff, 0xff,
    0,
    0xff, 0xff,
    0
};

static const uint8_t MGC_aReadCapacitySupport[] =
{
    MGC_SCSI_DEVICE_TYPE_DIRECT,   /* device type */
    5,	    /* supported, vendor-specific */
    0,	    /* no compliance claimed */
    0, 0,
    10,
    MGC_SCSI_READ_CAPACITY,
    0,
    0xff, 0xff, 0xff, 0xff,
    0,
    0xff, 0xff,
    0
};

static const uint8_t MGC_aTestUnitReadySupport[] =
{
    MGC_SCSI_DEVICE_TYPE_DIRECT,   /* device type */
    5,	    /* supported, vendor-specific */
    0,	    /* no compliance claimed */
    0, 0,
    6,
    MGC_SCSI_TEST_UNIT_READY,
    0,
    0, 0,
    0xff, 
    0
};

static const uint8_t MGC_aRequestSenseSupport[] =
{
    MGC_SCSI_DEVICE_TYPE_DIRECT,   /* device type */
    5,	    /* supported, vendor-specific */
    0,	    /* no compliance claimed */
    0, 0,
    6,
    MGC_SCSI_REQUEST_SENSE,
    0,
    0, 0,
    0xff, 
    0
};

static const uint8_t MGC_aInquirySupport[] =
{
    MGC_SCSI_DEVICE_TYPE_DIRECT,   /* device type */
    5,	    /* supported, vendor-specific */
    0,	    /* no compliance claimed */
    0, 0,
    6,
    MGC_SCSI_INQUIRY,
    3,
    0xff, 
    0,
    0xff, 
    0
};

static const uint8_t MGC_aModeSenseSupport[] =
{
    MGC_SCSI_DEVICE_TYPE_DIRECT,   /* device type */
    5,	    /* supported, vendor-specific */
    0,	    /* no compliance claimed */
    0, 0,
    6,
    MGC_SCSI_MODE_SENSE,
    8,
    0xff, 
    0,
    0xff, 
    0
};

static const uint8_t MGC_aModeSelectSupport[] =
{
    MGC_SCSI_DEVICE_TYPE_DIRECT,   /* device type */
    5,	    /* supported, vendor-specific */
    0,	    /* no compliance claimed */
    0, 0,
    6,
    MGC_SCSI_MODE_SELECT,
    0x11,
    0, 
    0,
    0xff, 
    0
};

static const uint8_t MGC_aStartStopUnitSupport[] =
{
    MGC_SCSI_DEVICE_TYPE_DIRECT,   /* device type */
    5,	    /* supported, vendor-specific */
    0,	    /* no compliance claimed */
    0, 0,
    6,
    MGC_SCSI_START_STOP_UNIT,
    0x01,
    0, 
    0,
    0xf3, 
    0
};

static const MGC_MsdCommandUsage MGC_aCommandUsageData[] =
{
    { MGC_SCSI_READ10, (uint8_t)sizeof(MGC_aRead10Support), MGC_aRead10Support },
    { MGC_SCSI_WRITE10, (uint8_t)sizeof(MGC_aWrite10Support), MGC_aWrite10Support },
    { MGC_SCSI_VERIFY, (uint8_t)sizeof(MGC_aVerifySupport), MGC_aVerifySupport },
    { MGC_SCSI_WRITE_BUFFER, (uint8_t)sizeof(MGC_aWriteBufferSupport), MGC_aWriteBufferSupport },
    { MGC_SCSI_READ_CAPACITY, (uint8_t)sizeof(MGC_aReadCapacitySupport), MGC_aReadCapacitySupport },
    { MGC_SCSI_TEST_UNIT_READY, (uint8_t)sizeof(MGC_aTestUnitReadySupport), MGC_aTestUnitReadySupport },
    { MGC_SCSI_REQUEST_SENSE, (uint8_t)sizeof(MGC_aRequestSenseSupport), MGC_aRequestSenseSupport },
    { MGC_SCSI_INQUIRY, (uint8_t)sizeof(MGC_aInquirySupport), MGC_aInquirySupport },
    { MGC_SCSI_MODE_SENSE, (uint8_t)sizeof(MGC_aModeSenseSupport), MGC_aModeSenseSupport },
    { MGC_SCSI_MODE_SELECT, (uint8_t)sizeof(MGC_aModeSelectSupport), MGC_aModeSelectSupport },
    { MGC_SCSI_START_STOP_UNIT, (uint8_t)sizeof(MGC_aStartStopUnitSupport), MGC_aStartStopUnitSupport },
};

//  MTK Notice: Max Liao, 2006/12/04.
//  do not clear MSD ram disk data.
//static uint8_t MGC_bIsInitialized = FALSE;
static uint8_t MGC_bIsReady = TRUE;
static uint8_t MGC_bMsdMaxLun = 0;
static uint8_t MGC_bMsdInterface = 0;
static uint16_t MGC_wBlockSize = 512;
static uint32_t MGC_dwRxCount;

/** Current sense data */
static MGC_MsdScsiSenseData MGC_MsdSenseData;

/** Capacity response */
static MGC_MsdCapacityResponse MGC_MsdCapacity;
static MGC_MsdFormatCapacityResponse MGC_MsdFormatCapacity;

/** Current CBW */
static MGC_MsdCbw MGC_Cbw;

/** Current CSW */
static MGC_MsdCsw MGC_Csw;

static uint8_t MGC_aJunk[512];

/** IRP for CBW reception */
static MUSB_Irp MGC_MsdRxCbwIrp = 
{
    NULL,
    (uint8_t*)&MGC_Cbw,
    sizeof(MGC_Cbw),
    0,
    0,
    MGC_MsdRxCbwComplete,
    NULL,
    FALSE,	/* bAllowShortTransfer */
    TRUE,	/* bIsrCallback */
    FALSE	/* bAllowDma */
};

/** IRP for data reception */
static MUSB_Irp MGC_MsdRxDataIrp = 
{
    NULL,
    NULL,
    0,
    0,
    0,
    MGC_MsdRxDataComplete,
    NULL,
    FALSE,	/* bAllowShortTransfer */
    TRUE,	/* bIsrCallback */
    TRUE	/* bAllowDma */
};

/** IRP for data transmission */
static MUSB_Irp MGC_MsdTxDataIrp = 
{
    NULL,
    NULL,
    0,
    0,
    0,
    MGC_MsdTxDataComplete,
    NULL,
    FALSE,	/* bAllowShortTransfer */
    TRUE,	/* bIsrCallback */
    TRUE	/* bAllowDma */
};

/** IRP for CSW transmission */
static MUSB_Irp MGC_MsdTxCswIrp = 
{
    NULL,
    (uint8_t*)&MGC_Csw,
    13,
    0,
    0,
    MGC_MsdTxCswComplete,
    NULL,
    FALSE,	/* bAllowShortTransfer */
    TRUE,	/* bIsrCallback */
    FALSE	/* bAllowDma */
};

/*
* registration
*/
MUSB_FunctionClient MGC_MsdFunctionClient =
{
    NULL,	/* no instance data; we are singleton */
    (const uint8_t*)MGC_aDescriptorData,
    sizeof(MGC_aDescriptorData),
    3,		/* strings per language */
    MGC_aHighSpeedDescriptorData,
    sizeof(MGC_aHighSpeedDescriptorData),
    sizeof(MGC_aControlData),
    MGC_aControlData,
    &MGC_bMsdSelfPowered,
    MGC_MsdDeviceRequest,
    MGC_MsdDeviceConfigSelected,
    NULL,
    MGC_MsdNewUsbState
};

//  MTK Notice: Max Liao, 2006/12/04.
//  define 160MB Ram disk on DTV platform.
STATIC uint8_t* MGC_aMsdData =  (uint8_t*)0x024fc000;
static uint32_t MGC_dwMsdBlockCount = (0x05b00000) / 512;
//STATIC uint8_t MGC_aMsdData[16*1024*1024+512];
//static uint32_t MGC_dwMsdBlockCount = (16*1024*1024) / 512;

/******************************************************************
CDI callbacks
******************************************************************/

STATIC void MGC_MsdNewUsbState(void* hClient, MUSB_BusHandle hBus, 
			       MUSB_State State)
{
    MGC_eMsdUsbState = State;
    UNUSED(MGC_eMsdUsbState);
    UNUSED(hClient);
    UNUSED(hBus);

    /* TODO: anything? */
}

STATIC uint8_t MGC_MsdDeviceRequest(void* hClient, MUSB_BusHandle hBus, 
				    uint32_t dwSequence, const uint8_t* pSetup,
				    uint16_t wLength)
{
    MUSB_DeviceRequest* pRequest = (MUSB_DeviceRequest*)((uint32_t)pSetup);
    uint8_t bOk = FALSE;

    UNUSED(hClient);
    UNUSED(wLength);

    if(MUSB_TYPE_STANDARD == (pRequest->bmRequestType & MUSB_TYPE_MASK))
    {
	switch(pRequest->bRequest)
	{
	case MUSB_REQ_GET_INTERFACE:
	    MUSB_DeviceResponse(hBus, dwSequence, &MGC_bMsdInterface, 1, FALSE);
	    bOk = TRUE;
	    break;
	case MUSB_REQ_SET_INTERFACE:
	    MUSB_DeviceResponse(hBus, dwSequence, NULL, 0, FALSE);
	    bOk = TRUE;
	    break;
	default:
	    break;
	}
    }
    else if((pRequest->bmRequestType & MUSB_TYPE_CLASS)
	&& (pRequest->bmRequestType & MUSB_RECIP_INTERFACE))
    {
	switch(pRequest->bRequest)
	{
	case MGC_MSD_BOT_RESET:
	    /* reset */
	    MUSB_CancelTransfer(&MGC_MsdTxCswIrp);
	    MUSB_CancelTransfer(&MGC_MsdTxDataIrp);
	    MUSB_CancelTransfer(&MGC_MsdRxDataIrp);
	    MUSB_CancelTransfer(&MGC_MsdRxCbwIrp);
	    VERIFY(0 == MUSB_FlushPipe(MGC_MsdTxDataIrp.hPipe));
	    VERIFY(0 == MUSB_FlushPipe(MGC_MsdRxDataIrp.hPipe));
	    UNUSED(MUSB_StartTransfer(&MGC_MsdRxCbwIrp));
	    MUSB_DeviceResponse(hBus, dwSequence, NULL, 0, FALSE);
	    bOk = TRUE;
	    break;

	case MGC_MSD_BOT_GET_MAXLUN:
	    /* get max lun */
	    MUSB_DeviceResponse(hBus, dwSequence, &MGC_bMsdMaxLun, 1, FALSE);
	    bOk = TRUE;
	    break;
	    
	default:
	    break;	    
	}
    }
    return bOk;
}

STATIC uint8_t MGC_MsdDeviceConfigSelected(void* hClient, MUSB_BusHandle hBus, 
					   uint8_t bConfigurationValue,
					   MUSB_Pipe* ahPipe)
{
    uint32_t dwStatus;
#ifdef USB_UNIT_TEST    
    extern void *pvUSBMemPtr1;
#endif

    UNUSED(hClient);
    UNUSED(hBus);
    UNUSED(bConfigurationValue);

//  MTK Notice: Max Liao, 2006/12/04.
//  do not clear MSD ram disk data.
#if 0
    if(!MGC_bIsInitialized)
    {
	MGC_bIsInitialized = TRUE;
	MUSB_MemSet(MGC_aMsdData, 0, sizeof(MGC_aMsdData));
    }
#endif 

#ifdef USB_UNIT_TEST    
    MGC_aMsdData =  (uint8_t*)pvUSBMemPtr1;
#endif 
    MGC_MsdTxDataIrp.hPipe = ahPipe[0];
    MGC_MsdTxCswIrp.hPipe = ahPipe[0];
    MGC_MsdRxCbwIrp.hPipe = ahPipe[1];
    MGC_MsdRxDataIrp.hPipe = ahPipe[1];
    dwStatus = MUSB_StartTransfer(&MGC_MsdRxCbwIrp);
    if(MUSB_STATUS_OK == dwStatus)
    {
	MGC_bIsReady = TRUE;
	MGC_MsdSenseData.bResponseCode = (uint8_t)MGC_M_MSD_SCSI_SENSE_DATA_VALID | 0x70;
	MGC_MsdSenseData.bSenseKey = (uint8_t)MGC_SCSI_SC_NONE;
	MGC_MsdSenseData.bAsc = 0;
	MGC_MsdSenseData.bAscq = 0;
	return TRUE;
    }
    /* TODO: log error? */
    return FALSE;
}

/* handle an INQUIRY command */
static void MGC_MsdInquiry(uint8_t bmFlags, uint32_t dwLength, uint8_t bPage,
			   uint8_t* pbStartTx, uint8_t* pbStatus)
{
    uint16_t wIndex, wCount;

    if((bmFlags & (MGC_M_MSD_INQUIRY_CMD_DT | MGC_M_MSD_INQUIRY_EVPD)) ==
	(uint8_t)(MGC_M_MSD_INQUIRY_CMD_DT | MGC_M_MSD_INQUIRY_EVPD))
    {
	/* set sense codes */
	MGC_MsdSenseData.bResponseCode = (uint8_t)MGC_M_MSD_SCSI_SENSE_DATA_VALID | 0x70;
	MGC_MsdSenseData.bSenseKey = (uint8_t)MGC_SCSI_SC_ILLEGAL_REQUEST;
	MGC_MsdSenseData.bAsc = (uint8_t)MGC_SCSI_ASC_INVALID_CDB_FIELD;
	MGC_MsdSenseData.bAdditionalLength = 5;
	*pbStatus = 1;
    }
    else if(bmFlags & MGC_M_MSD_INQUIRY_CMD_DT)
    {
	/* command support data */
	wCount = sizeof(MGC_aCommandUsageData) / sizeof(MGC_MsdCommandUsage);
	for(wIndex = 0; wIndex < wCount; wIndex++)
	{
	    if(MGC_aCommandUsageData[wIndex].bCommand == bPage)
	    {
		break;
	    }
	}
	if(wIndex < wCount)
	{
	    MGC_MsdTxDataIrp.pBuffer = (uint8_t*)MGC_aCommandUsageData[wIndex].pData;
	    MGC_MsdTxDataIrp.dwLength = MUSB_MIN(dwLength,
		MGC_aCommandUsageData[wIndex].bDataSize);
	}
	else
	{
	    MGC_MsdTxDataIrp.pBuffer = (uint8_t*)MGC_aUnsupportedCommandData;
	    MGC_MsdTxDataIrp.dwLength = MUSB_MIN(dwLength,
		sizeof(MGC_aUnsupportedCommandData));
	}
	*pbStartTx = TRUE;
	*pbStatus = 0;
    }
    else if(bmFlags & MGC_M_MSD_INQUIRY_EVPD)
    {
	/* vital product data pages */
	switch(bPage)
	{
	case MGC_SCSI_PAGE_SUPPORTED_VPD:
	    MGC_MsdTxDataIrp.pBuffer = (uint8_t*)&MGC_MsdSupportedVpdPagesData;
	    MGC_MsdTxDataIrp.dwLength = MUSB_MIN(dwLength,
		sizeof(MGC_MsdSupportedVpdPagesData));
	    *pbStartTx = TRUE;
	    *pbStatus = 0;
	    break;
	case MGC_SCSI_PAGE_UNIT_SERIAL_NUM:
	    MGC_MsdTxDataIrp.pBuffer = (uint8_t*)&MGC_MsdUnitSerialNumberPageData;
	    MGC_MsdTxDataIrp.dwLength = MUSB_MIN(dwLength,
		sizeof(MGC_MsdUnitSerialNumberPageData));
	    *pbStartTx = TRUE;
	    *pbStatus = 0;
	    break;
	case MGC_SCSI_PAGE_DEVICE_ID:
	    MGC_MsdTxDataIrp.pBuffer = (uint8_t*)&MGC_MsdDeviceIdPageData;
	    MGC_MsdTxDataIrp.dwLength = MUSB_MIN(dwLength,
		sizeof(MGC_MsdDeviceIdPageData));
	    *pbStartTx = TRUE;
	    *pbStatus = 0;
	    break;
	    
	default:
	    break;	    
	}
    }
    else
    {
	/* standard data */
	MGC_MsdTxDataIrp.pBuffer = (uint8_t*)&MGC_MsdRbcInquiryData;
	MGC_MsdTxDataIrp.dwLength = MUSB_MIN(dwLength,
	    sizeof(MGC_MsdRbcInquiryData));
	*pbStartTx = TRUE;
	*pbStatus = 0;
    }
}

static void MGC_MsdCommand(const MGC_MsdCbw* pCbw)
{
    const MGC_MsdCommand6* pCommand6;
    const MGC_MsdInquiryCommand* pInquiryCommand;
    uint8_t bmFlags, bPage;
    uint32_t dwLength;
    uint8_t bStartTx = FALSE;
    uint8_t bStartRx = FALSE;
    uint8_t bStatus = 1;
    uint32_t dwBlock = 0;
    uint32_t dwDataLength = MUSB_SWAP32(pCbw->dCbwDataTransferLength);
    MGC_MsdCsw* pCsw = &MGC_Csw;
    uint8_t bOpcode = pCbw->aCbwCb[0];

    MGC_Cbw.dCbwDataTransferLength = dwDataLength;
    /* seed failed sense data */
    MGC_MsdSenseData.bResponseCode = (uint8_t)MGC_M_MSD_SCSI_SENSE_DATA_VALID | 0x70;
    MGC_MsdSenseData.bSenseKey = (uint8_t)MGC_SCSI_SC_ILLEGAL_REQUEST;
    MGC_MsdSenseData.bAsc = (uint8_t)MGC_SCSI_ASC_INVALID_CDB_FIELD;
    MGC_MsdSenseData.bAdditionalLength = 5;

    /* parse command */
    pCommand6 = (const MGC_MsdCommand6*)&(pCbw->aCbwCb[0]);
    switch(bOpcode)
    {
    case MGC_SCSI_START_STOP_UNIT:
	if(pCbw->aCbwCb[4] & 1)
	{
	    MGC_bIsReady = TRUE;
	}
	else
	{
	    MGC_bIsReady = FALSE;
	}
	MGC_MsdSenseData.bResponseCode = (uint8_t)MGC_M_MSD_SCSI_SENSE_DATA_VALID | 0x70;
	MGC_MsdSenseData.bSenseKey = (uint8_t)MGC_SCSI_SC_NONE;
	MGC_MsdSenseData.bAsc = 0;
	MGC_MsdSenseData.bAscq = 0;
	bStatus = 0;
	break;

    case MGC_SCSI_REQUEST_SENSE:
	dwLength = pCbw->aCbwCb[4];
	MGC_MsdTxDataIrp.pBuffer = (uint8_t*)&MGC_MsdSenseData;
	MGC_MsdTxDataIrp.dwLength = MUSB_MIN(dwLength, 
	    (uint32_t)(8+MGC_MsdSenseData.bAdditionalLength));
	pCsw->dCswDataResidue = dwLength - MGC_MsdTxDataIrp.dwLength;
	bStartTx = TRUE;
	MGC_MsdSenseData.bResponseCode = 0x70;
	bStatus = 0;
	break;

    case MGC_SCSI_TEST_UNIT_READY:
	pCsw->dCswDataResidue = 0L;
	MGC_MsdSenseData.bAdditionalLength = 5;
	if(MGC_bIsReady)
	{
	    MGC_MsdSenseData.bResponseCode = (uint8_t)MGC_M_MSD_SCSI_SENSE_DATA_VALID | 0x70;
	    MGC_MsdSenseData.bSenseKey = (uint8_t)MGC_SCSI_SC_NONE;
	    MGC_MsdSenseData.bAsc = 0;
	    MGC_MsdSenseData.bAscq = 0;
	    bStatus = 0;
	}
	else
	{
	    MGC_MsdSenseData.bResponseCode = (uint8_t)
		(MGC_M_MSD_SCSI_SENSE_DATA_VALID | MGC_SCSI_STATUS_CHECK_CONDITION);
	    MGC_MsdSenseData.bSenseKey = (uint8_t)MGC_SCSI_SC_NOT_READY;
	    MGC_MsdSenseData.bAsc = 4;
	    MGC_MsdSenseData.bAscq = 2;
	    bStatus = 1;
	}
	break;

    case MGC_SCSI_INQUIRY:
	pInquiryCommand = (const MGC_MsdInquiryCommand*)pCommand6;
	bmFlags = pInquiryCommand->bmFlags;
	dwLength = pCbw->aCbwCb[4];
	bPage = pCbw->aCbwCb[2];
	MGC_MsdInquiry(bmFlags, dwLength, bPage, &bStartTx, &bStatus);
	pCsw->dCswDataResidue = dwLength - MGC_MsdTxDataIrp.dwLength;
	break;

    case MGC_SCSI_READ_CAPACITY:
	dwLength = pCbw->aCbwCb[4];
	MGC_MsdCapacity.dwLastBlock = MGC_SCSI_SWAP32(MGC_dwMsdBlockCount - 1);
	MGC_MsdCapacity.dwBytesPerBlock = MGC_SCSI_SWAP32(MGC_wBlockSize);
	MGC_MsdTxDataIrp.pBuffer = (uint8_t*)&MGC_MsdCapacity;
	MGC_MsdTxDataIrp.dwLength = sizeof(MGC_MsdCapacity);
	pCsw->dCswDataResidue = dwLength ? (dwLength - MGC_MsdTxDataIrp.dwLength) : 0;
	MGC_MsdSenseData.bResponseCode = 0x70;
	bStartTx = TRUE;
	bStatus = 0;
	break;

    case MGC_SCSI_RD_FMT_CAPC:
	dwLength = pCbw->aCbwCb[4];
	MGC_MsdFormatCapacity.dwListLength = MGC_SCSI_SWAP32(8);
	MGC_MsdFormatCapacity.dwBlockCount = MGC_SCSI_SWAP32(MGC_dwMsdBlockCount);
	MGC_MsdFormatCapacity.dwBytesPerBlock = MGC_SCSI_SWAP32(MGC_wBlockSize);
	MGC_MsdTxDataIrp.pBuffer = (uint8_t*)&MGC_MsdFormatCapacity;
	MGC_MsdTxDataIrp.dwLength = sizeof(MGC_MsdFormatCapacity);
	pCsw->dCswDataResidue = dwLength ? (dwLength - MGC_MsdTxDataIrp.dwLength) : 0;
	MGC_MsdSenseData.bResponseCode = 0x70;
	bStartTx = TRUE;
	bStatus = 0;
	break;

    case MGC_SCSI_PREVENT_ALLOW_MED_REMOVE:
	pCsw->dCswDataResidue = 0L;
	MGC_MsdSenseData.bResponseCode = 0x70;
	bStatus = 0;
	break;

    case MGC_SCSI_MODE_SENSE:
	dwLength = pCbw->aCbwCb[4];
	bPage = pCbw->aCbwCb[2];
	switch(bPage & 0x3f)
	{
	    /*
	    case 6:
	    MGC_RbcModeParams.RbcParams.wBlockSize = MGC_SCSI_SWAP16(MGC_wBlockSize);
	    MGC_RbcModeParams.RbcParams.dwBlockCountLo32 = 
	    MGC_SCSI_SWAP32(sizeof(MGC_aMsdData) / MGC_wBlockSize);
	    MGC_MsdTxDataIrp.pBuffer = (uint8_t*)&MGC_RbcModeParams;
	    MGC_MsdTxDataIrp.dwLength = MUSB_MIN(dwLength,
	    sizeof(MGC_RbcModeParams));
	    bStartTx = TRUE;
	    bStatus = 0;
	    break;
	    */
	case 0x1c:
	case 0x3f:
	    MGC_aJunk[0] = 2;
	    MGC_aJunk[1] = 0;
	    MGC_aJunk[2] = 0;
	    MGC_aJunk[3] = 0;
	    MGC_MsdTxDataIrp.pBuffer = (uint8_t*)MGC_aJunk;
	    MGC_MsdTxDataIrp.dwLength = MUSB_MIN(dwLength, 4);
	    MGC_MsdSenseData.bResponseCode = 0x70;
	    bStartTx = TRUE;
	    bStatus = 0;
	    break;
	default:
	    MGC_MsdTxDataIrp.dwLength = 0L;
	}
	pCsw->dCswDataResidue = dwLength - MGC_MsdTxDataIrp.dwLength;
	break;

    case MGC_SCSI_MODE_SELECT:
	dwLength = pCbw->aCbwCb[4];
	MGC_MsdRxDataIrp.pBuffer = MGC_aJunk;
	MGC_MsdRxDataIrp.dwLength = MUSB_MIN(dwLength, sizeof(MGC_aJunk));
	MGC_MsdSenseData.bResponseCode = 0x70;
	bStartRx = TRUE;
	bStatus = 0;
	break;

    case MGC_SCSI_READ6:
	pCsw->dCswDataResidue = 0L;
	dwLength = pCbw->aCbwCb[4];
	dwBlock = (pCbw->aCbwCb[2] << 8) | pCbw->aCbwCb[3];
	MGC_MsdTxDataIrp.pBuffer = &(MGC_aMsdData[dwBlock * MGC_wBlockSize]);
	MGC_MsdTxDataIrp.dwLength = dwDataLength;
	MUSB_DIAG2(3, "READ(6) @", dwBlock, ", count=", dwLength, 16, 0);
	/* TODO: check for overrun */
	MGC_MsdSenseData.bResponseCode = 0x70;
	bStartTx = TRUE;
	bStatus = 0;
	break;

    case MGC_SCSI_READ10:
	pCsw->dCswDataResidue = 0L;
	dwLength = (pCbw->aCbwCb[7] << 8) | pCbw->aCbwCb[8];
	dwBlock = (pCbw->aCbwCb[2] << 24) | (pCbw->aCbwCb[3] << 16) |
	    (pCbw->aCbwCb[4] << 8) | pCbw->aCbwCb[5];
	MGC_MsdTxDataIrp.pBuffer = &(MGC_aMsdData[dwBlock * MGC_wBlockSize]);
	MGC_MsdTxDataIrp.dwLength = dwDataLength;
	MUSB_DIAG2(3, "READ(10) @", dwBlock, ", count=", dwLength, 16, 0);
	/* TODO: check for overrun */
	MGC_MsdSenseData.bResponseCode = 0x70;
	bStartTx = TRUE;
	bStatus = 0;
	break;

    case MGC_SCSI_WRITE6:
	MGC_dwRxCount = 0L;
	pCsw->dCswDataResidue = 0L;
	dwLength = pCbw->aCbwCb[4];
	dwBlock = (pCbw->aCbwCb[2] << 8) | pCbw->aCbwCb[3];
	MGC_MsdRxDataIrp.pBuffer = &(MGC_aMsdData[dwBlock * MGC_wBlockSize]);
	MGC_MsdRxDataIrp.dwLength = dwDataLength;
	MUSB_DIAG2(3, "WRITE(6) @", dwBlock, ", count=", dwLength, 16, 0);
	/* check for overrun */
	if((dwBlock + dwLength) > MGC_dwMsdBlockCount)
	{
	    bStatus = 1;
	    MUSB_SetPipeHalt(MGC_MsdRxDataIrp.hPipe, TRUE);
	}
	else
	{
	    MGC_MsdSenseData.bResponseCode = 0x70;
	    bStartRx = TRUE;
	    bStatus = 0;
	}
	break;

    case MGC_SCSI_WRITE10:
	MGC_dwRxCount = 0L;
	pCsw->dCswDataResidue = 0L;
	dwLength = (pCbw->aCbwCb[7] << 8) | pCbw->aCbwCb[8];
	dwBlock = (pCbw->aCbwCb[2] << 24) | (pCbw->aCbwCb[3] << 16) |
	    (pCbw->aCbwCb[4] << 8) | pCbw->aCbwCb[5];
	MGC_MsdRxDataIrp.pBuffer = &(MGC_aMsdData[dwBlock * MGC_wBlockSize]);
	MGC_MsdRxDataIrp.dwLength = dwDataLength;
	MUSB_DIAG2(3, "WRITE(10) @", dwBlock, ", count=", dwLength, 16, 0);
	/* check for overrun */
	if((dwBlock + dwLength) > MGC_dwMsdBlockCount)
	{
	    bStatus = 1;
	    MUSB_SetPipeHalt(MGC_MsdRxDataIrp.hPipe, TRUE);
	}
	else
	{
	    MGC_MsdSenseData.bResponseCode = 0x70;
	    bStartRx = TRUE;
	    bStatus = 0;
	}
	break;

    case MGC_SCSI_VERIFY:
	dwLength = (pCbw->aCbwCb[7] << 8) | pCbw->aCbwCb[8];
	if(pCbw->aCbwCb[1] & 2)
	{
	    MGC_dwRxCount = 0L;
	    pCsw->dCswDataResidue = 0L;
	    dwBlock = (pCbw->aCbwCb[2] << 24) | (pCbw->aCbwCb[3] << 16) |
		(pCbw->aCbwCb[4] << 8) | pCbw->aCbwCb[5];
	    MGC_MsdRxDataIrp.pBuffer = &(MGC_aMsdData[dwBlock * MGC_wBlockSize]);
	    MGC_MsdRxDataIrp.dwLength = dwDataLength;
	    MUSB_DIAG1(3, "Verify, length=", dwDataLength, 16, 8);
	    MGC_MsdSenseData.bResponseCode = 0x70;
	    bStartRx = TRUE;
	}
	bStatus = 0;
	break;

    case MGC_SCSI_WRITE_BUFFER:
	/* TODO */
	dwLength = (pCbw->aCbwCb[7] << 8) | pCbw->aCbwCb[8];
	MGC_MsdSenseData.bResponseCode &= ~MGC_M_MSD_SCSI_SENSE_DATA_VALID;
	bStatus = 0;
	break;

    default:
	MUSB_DIAG1(2, "MSD: unknown SCSI command ", bOpcode, 16, 2);
	bStatus = 1;
	break;
    }

    /* if bad status and host expecting data, stall IN pipe */
    if(bStatus && (pCbw->bmCbwFlags & MGC_MSD_BOT_DIR_IN))
    {
	MUSB_SetPipeHalt(MGC_MsdTxDataIrp.hPipe, TRUE);
    }

    /* start data transfer or CSW transfer */
    pCsw->dCswSignature = MGC_MSD_BOT_CSW_SIGNATURE;
    pCsw->dCswTag = pCbw->dCbwTag;
    pCsw->bCswStatus = bStatus;

    if(bStartTx)
    {
	MUSB_DIAG_STRING(3, "MSD: starting Data Tx");
	MGC_MsdTxDataIrp.dwActualLength = 0L;
	MUSB_StartTransfer(&MGC_MsdTxDataIrp);
    }
    else if(bStartRx)
    {
	MUSB_DIAG_STRING(3, "MSD: starting Data Rx");
	MGC_MsdRxDataIrp.dwActualLength = 0L;
	MUSB_StartTransfer(&MGC_MsdRxDataIrp);
    }
    else
    {
	MUSB_DIAG_STRING(3, "MSD: starting CSW Tx");
	MGC_MsdTxCswIrp.dwActualLength = 0L;
	MUSB_StartTransfer(&MGC_MsdTxCswIrp);
    }
}

/** CBW reception callback */
STATIC void MGC_MsdRxCbwComplete(void* pCompleteParam, const MUSB_Irp* pIrp)
{
    const MGC_MsdCbw* pCbw;

    UNUSED(pCompleteParam);

    if((MUSB_STATUS_OK == pIrp->dwStatus)
	&& (31 == pIrp->dwActualLength))
    {
	pCbw = (const MGC_MsdCbw*)((uint32_t)pIrp->pBuffer);
	if(MGC_MSD_BOT_CBW_SIGNATURE == pCbw->dCbwSignature)
	{
	    /* process the valid CBW */
	    MGC_MsdCommand(pCbw);
	    return;
	}
    }

    /*
    if(!pIrp->dwActualLength)
    {
	MUSB_DIAG_STRING(2, "MSD: Expected CBW; got null packet");
	MGC_MsdRxDataComplete(pCompleteParam, pIrp);
	return;
    }

    if(512 == pIrp->dwActualLength)
    {
	MUSB_DIAG_STRING(2, "MSD: Expected CBW; got data packet");
	if(MGC_MsdRxDataIrp.pBuffer)
	{
	    MUSB_MemCopy(MGC_MsdRxDataIrp.pBuffer, pIrp->pBuffer, 512);
	    MGC_MsdRxDataIrp.dwActualLength = 512;
	}
	return;
    }
    */

    /* invalid CBW: stall pipes */
    MUSB_SetPipeHalt(MGC_MsdRxCbwIrp.hPipe, TRUE);
    MUSB_SetPipeHalt(MGC_MsdTxCswIrp.hPipe, TRUE);

    /* prepare for post-stall command */
    MGC_MsdRxCbwIrp.dwActualLength = 0L;
    MUSB_StartTransfer(&MGC_MsdRxCbwIrp);

}

/** data reception callback */
STATIC void MGC_MsdRxDataComplete(void* pCompleteParam, const MUSB_Irp* pIrp)
{
    /* data recvd; so send status */
    MGC_dwRxCount += MGC_MsdRxDataIrp.dwActualLength;
    /* TODO: check for data recv error */

    /* send CSW */
    MUSB_DIAG1(3, "MSD: Sending CSW after recv'd data len=", MGC_dwRxCount, 16, 8);
    MGC_Csw.dCswDataResidue = MGC_Cbw.dCbwDataTransferLength - MGC_dwRxCount;
    MGC_MsdTxCswIrp.dwActualLength = 0L;
    MUSB_StartTransfer(&MGC_MsdTxCswIrp);
}

/** data transmit complete callback */
STATIC void MGC_MsdTxDataComplete(void* pCompleteParam, const MUSB_Irp* pIrp)
{
    /* data sent; send status */
    /* TODO: check for data send error */

    /* send CSW */
    MGC_MsdTxCswIrp.dwActualLength = 0L;
    MUSB_DIAG_STRING(3, "MSD: Starting CSW Tx");
    MUSB_StartTransfer(&MGC_MsdTxCswIrp);
}

/** CSW transmit complete callback */
STATIC void MGC_MsdTxCswComplete(void* pCompleteParam, const MUSB_Irp* pIrp)
{
    /* prepare for next command */
    MGC_MsdRxCbwIrp.dwActualLength = 0L;
    MUSB_DIAG_STRING(3, "MSD: Starting CBW Rx");
    MUSB_StartTransfer(&MGC_MsdRxCbwIrp);
}

LINT_RESTORE

#endif /* MUSB_FUNCTION */
