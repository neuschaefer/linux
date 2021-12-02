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
/*****************************************************************************
 * FILE NAME    : mu_tapi.c
 * DATE         : 
 * DESCRIPTION  : Contains all functions for Transport device driver which is
 *                exported to other layers. 
 *
 * $Revision: #1 $
 ****************************************************************************/

#include "mu_cdi.h"
#include "mu_descs.h"
#include "mu_diag.h"
#include "mu_list.h"
#include "mu_mem.h"
#include "mu_stdio.h"

#include "mu_tapi.h"
#include "mu_tti.h"
#include "class/mu_trans.h"

/***************************** TYPES ******************************/

/**
 * MGC_TransTestData.
 * Test instance data (passed to client when a test is added)
 * @field pTestSpec pointer to test spec (client must maintain)
 * @field pfTestProgress client callback for test progress
 * @field pfTestError client callback for test errors
 * @field hInPipe IN pipe
 * @field hOutPipe OUT pipe
 * @field InIrp IN bulk/interrupt IRP
 * @field OutIrp OUT bulk/interrupt IRP
 * @field InIsochIrp IN isochronous IRP
 * @field OutIsochIrp OUT isochronous IRP
 */
typedef struct
{
    MUSB_TtiTestSpec* pTestSpec;
    MUSB_pfTtiTestProgress pfTestProgress;
    MUSB_pfTtiTestError pfTestError;
    MUSB_Pipe hInPipe;
    MUSB_Pipe hOutPipe;
    MUSB_Irp InIrp;
    MUSB_Irp OutIrp;
#ifdef MUSB_ISO
    MUSB_IsochIrp InIsochIrp;
    MUSB_IsochIrp OutIsochIrp;
#endif
} MGC_TransTestData;

/**
 * MGC_TransInstance.
 * Driver instance data.
 * @field TestRunner dispatch table for client
 * @field TestDataList linked list of requested tests
 * @field ControlIrp to set test mode on device
 * @field pCurrentTest pointer to "current" test data
 * (used for remembering what we've been requested to add,
 * and current test while running)
 * @field pfTestAddStatus client callback for test addition
 * @field Command command to send device
 * @field hBus bus handle
 * @field dwCurrentIteration current global iteration
 * @field wTestCount how many tests
 * @field wTestIndex current test index
 */
typedef struct
{
    MUSB_TtiRunner TestRunner;
    MUSB_LinkedList TestDataList;
    MUSB_ControlIrp ControlIrp;
    MGC_TransTestData* pCurrentTest;
    MUSB_pfTtiTestAddStatus pfTestAddStatus;
    uint8_t abSetup[16];
    MUSB_BusHandle hBus;
    uint32_t dwCurrentIteration;
    uint16_t wTestCount;
    uint16_t wTestIndex;
} MGC_TransInstance;

/*************************** FORWARDS *****************************/

static uint8_t MGC_TransConnect (void* pPrivateData, 
				 MUSB_BusHandle    hBus,
				 MUSB_Device       *pUsbDevice,
				 const uint8_t     *pPeripheralList);
static void MGC_TransDisconnect(void* pPrivateData, 
				MUSB_BusHandle  hBus,
				MUSB_Device     *pUsbDevice);

static void MGC_TransAddTest(void* pDeviceData, MUSB_TtiTestSpec* pTestSpec, 
			     MUSB_pfTtiTestAddStatus pfTestAddStatus,
			     MUSB_pfTtiTestProgress pfTestProgress,
			     MUSB_pfTtiTestError pfTestError);

static void MGC_TransRemoveTest(void* pDeviceData, void* pTestData);

static void MGC_TransStartTests(void* pDeviceData, uint32_t dwIterationCount);

static void MGC_TransStopTests(void* pDeviceData);

static void MGC_TransControlIrpComplete(void* pPrivateData, MUSB_ControlIrp* pIrp);

static uint8_t MGC_TransAllocBuffers(MGC_TransTestData* pTestData);

static void MGC_TransFreeBuffers(MGC_TransTestData* pTestData);

static void MGC_TransTestStartTest(MGC_TransInstance* pInstance, 
				   MGC_TransTestData* pTestData);

static void MGC_TransFillBuffer(MUSB_TtiTestSpec* pTestSpec,
				uint8_t* pBuffer, uint32_t dwSize);

static uint32_t MGC_TransCompareBuffer(MUSB_TtiTestSpec* pTestSpec,
				       uint8_t* pBuffer, uint32_t dwSize);

static void MGC_TransTestInComplete(void *pPrivateData, MUSB_Irp *pIrp);
static void MGC_TransTestOutComplete(void *pPrivateData, MUSB_Irp *pIrp);

#ifdef MUSB_ISO
static void MGC_TransTestIsochInComplete(void *pPrivateData, MUSB_IsochIrp *pIrp);
static void MGC_TransTestIsochOutComplete(void *pPrivateData, MUSB_IsochIrp *pIrp);
#endif

static void MGC_TransNextTest(MGC_TransInstance* pInstance);

/**************************** GLOBALS *****************************/

static const uint8_t MGC_aTransPeripheralList[] =
{
    MUSB_TARGET_VID, 0xd6, 0x04, 
    MUSB_TARGET_PID, 0xff, 0xff,
    MUSB_TARGET_ACCEPT
};

/** Transport device driver */
MUSB_DeviceDriver MGC_TransDeviceDriver =
{
    NULL,
    0,
    NULL,
    MGC_TransConnect,
    MGC_TransDisconnect,
    NULL,
    NULL
};

/*************************** FUNCTIONS ****************************/

uint16_t MGC_FillTransportPeripheralList(uint8_t* pList, uint16_t wListLength)
{
    uint16_t wResult = MUSB_MIN((uint16_t)sizeof(MGC_aTransPeripheralList), wListLength);

    MUSB_MemCopy(pList, MGC_aTransPeripheralList, wResult);
    return wResult;
}

MUSB_DeviceDriver* MGC_GetTransportDeviceDriver()
{
    return &MGC_TransDeviceDriver;
}

/** This function is called when Transport device is connected.*/
static uint8_t 
MGC_TransConnect(void *pPrivateData, MUSB_BusHandle hBus,
		 MUSB_Device* pUsbDevice,
		 const uint8_t* pPeripheralList)
{
    uint8_t bResult = FALSE;
    MUSB_TtiRunner* pRunner;
    MGC_TransInstance* pInstance = (MGC_TransInstance*)MUSB_MemAlloc(sizeof(MGC_TransInstance));
    MUSB_DeviceRequest* pRequest = (MUSB_DeviceRequest*)pInstance->abSetup;

    if(pInstance)
    {
	bResult = TRUE;
	MUSB_MemSet(pInstance, 0, sizeof(MGC_TransInstance));
	pUsbDevice->pDriverPrivateData = pInstance;
	pInstance->hBus = hBus;

	/* set up static parts of control IRP */
	pInstance->ControlIrp.pDevice = pUsbDevice;
	pInstance->ControlIrp.pOutBuffer = (uint8_t*)&(pInstance->abSetup);
	pInstance->ControlIrp.dwOutLength = 16;
	pRequest->bmRequestType = (MUSB_TYPE_VENDOR | MUSB_RECIP_OTHER);
	pRequest->bRequest = MUSB_TRANSPORT_REQ_CONFIGURE_END;
	pRequest->wLength = MUSB_SWAP16(8);
	pInstance->ControlIrp.pfIrpComplete = MGC_TransControlIrpComplete;
	pInstance->ControlIrp.pCompleteParam = pInstance;

	/* set up TTI dispatch table */
	pRunner = &(pInstance->TestRunner);
	pRunner->pPrivateData = pInstance;
	pRunner->pfTtiAddTest = MGC_TransAddTest;
	pRunner->pfTtiRemoveTest = MGC_TransRemoveTest;
	pRunner->pfTtiStartTests = MGC_TransStartTests;
	pRunner->pfTtiStopTests = MGC_TransStopTests;

	/* initialize test list */
	MUSB_ListInit(&(pInstance->TestDataList));

	/* inform client */
	MUSB_TtiAddTestRunner(pRunner);
    }

    return bResult;
}

/** Disconnect Handler for Transport Device Driver */
static void MGC_TransDisconnect (void *pPrivateData, MUSB_BusHandle hBus,
				 MUSB_Device *pUsbDevice)
{
    uint16_t wCount, wIndex;
    void* pItem;
    MGC_TransInstance* pInstance = (MGC_TransInstance*)pUsbDevice->pDriverPrivateData;

    MUSB_TtiRemoveTestRunner(&(pInstance->TestRunner));

    /* remove & free all tests */
    wCount = MUSB_ListLength(&(pInstance->TestDataList));
    for(wIndex = 0; wIndex < wCount; wIndex++)
    {
	pItem = MUSB_ListFindItem(&(pInstance->TestDataList), 0);
	if(pItem)
	{
	    MGC_TransRemoveTest(pInstance, pItem);
	}
    }
    /* free instance */
    MUSB_MemFree(pInstance);
}

static void MGC_TransAddTest(void* pDeviceData, MUSB_TtiTestSpec* pTestSpec, 
			     MUSB_pfTtiTestAddStatus pfTestAddStatus,
			     MUSB_pfTtiTestProgress pfTestProgress,
			     MUSB_pfTtiTestError pfTestError)
{
    uint32_t dwStatus;
    MGC_TransTestData* pCurrentTest;
    MGC_TransInstance* pInstance = (MGC_TransInstance*)pDeviceData;
    MUSB_TransportConfigureEnd* pCommand = (MUSB_TransportConfigureEnd*)&(pInstance->abSetup[8]);

    /* allocate local test data */
    pCurrentTest = (MGC_TransTestData*)MUSB_MemAlloc(sizeof(MGC_TransTestData));
    if(!pCurrentTest)
    {
	pfTestAddStatus(pTestSpec, NULL);
	return;
    }

    /* remember what we're trying to do */
    MUSB_MemSet(pCurrentTest, 0, sizeof(MGC_TransTestData));
    pCurrentTest->pTestSpec = pTestSpec;
    pCurrentTest->pfTestProgress = pfTestProgress;
    pCurrentTest->pfTestError = pfTestError;
    pInstance->pCurrentTest = pCurrentTest;
    pInstance->pfTestAddStatus = pfTestAddStatus;

    pCurrentTest->InIrp.pfIrpComplete = MGC_TransTestInComplete;
    pCurrentTest->InIrp.pCompleteParam = pInstance;
    pCurrentTest->OutIrp.pfIrpComplete = MGC_TransTestOutComplete;
    pCurrentTest->OutIrp.pCompleteParam = pInstance;
#ifdef MUSB_ISO
    pCurrentTest->InIsochIrp.pfIrpComplete = MGC_TransTestIsochInComplete;
    pCurrentTest->InIsochIrp.pCompleteParam = pInstance;
    pCurrentTest->OutIsochIrp.pfIrpComplete = MGC_TransTestIsochOutComplete;
    pCurrentTest->OutIsochIrp.pCompleteParam = pInstance;
#endif

    /* set up command */
    switch(pTestSpec->TestType)
    {
    case MUSB_TTI_TYPE_SINK:
	pCommand->bType = MUSB_TRANSPORT_REQ_SET_SINK;
	pCommand->bInOutEnds = pTestSpec->bOutEnd;
	break;
    case MUSB_TTI_TYPE_SOURCE:
	pCommand->bType = MUSB_TRANSPORT_REQ_SET_SOURCE;
	pCommand->bInOutEnds = pTestSpec->bInEnd << 4;
	/* options apply only to source test */
	switch(pTestSpec->TestOption)
	{
	case MUSB_TTI_OPTION_INCREMENT:
	    pCommand->bOptions = MUSB_TRANSPORT_OPTIONS_INCREMENT;
	    break;
	case MUSB_TTI_OPTION_DECREMENT:
	    pCommand->bOptions = MUSB_TRANSPORT_OPTIONS_DECREMENT;
	    break;
	case MUSB_TTI_OPTION_ROTATE_LEFT:
	    pCommand->bOptions = MUSB_TRANSPORT_OPTIONS_ROTATE_LEFT;
	    break;
	case MUSB_TTI_OPTION_ROTATE_RIGHT:
	    pCommand->bOptions = MUSB_TRANSPORT_OPTIONS_ROTATE_RIGHT;
	    break;
	}
	break;
    case MUSB_TTI_TYPE_LOOPBACK:
	pCommand->bType = MUSB_TRANSPORT_REQ_SET_LOOPBACK;
	pCommand->bInOutEnds = (pTestSpec->bInEnd << 4) | pTestSpec->bOutEnd;
	break;
    }

    pCommand->bmAttributes = pTestSpec->bProtocol;
    pCommand->bStartPattern = pTestSpec->bStartPattern;
    pCommand->bPacketCount = pTestSpec->bPacketCount;
    pCommand->wMaxPacketSize = MUSB_SWAP16(pTestSpec->wMaxPacketSize);

    /* try to send command */
    dwStatus = MUSB_StartControlTransfer(pInstance->ControlIrp.pDevice->pPort,
	&(pInstance->ControlIrp));
    if(MUSB_STATUS_OK != dwStatus)
    {
	pfTestAddStatus(pTestSpec, NULL);
    }

}

static void MGC_TransRemoveTest(void* pDeviceData, void* hTest)
{
    MGC_TransInstance* pInstance = (MGC_TransInstance*)pDeviceData;
    MGC_TransTestData* pTestData = (MGC_TransTestData*)hTest;

    /* cleanup */
    if(pTestData->hInPipe)
    {
	MUSB_ClosePipe(pTestData->hInPipe);
    }
    if(pTestData->hOutPipe)
    {
	MUSB_ClosePipe(pTestData->hOutPipe);
    }
    MGC_TransFreeBuffers(pTestData);
    MUSB_ListRemoveItem(&(pInstance->TestDataList), pTestData);
    MUSB_MemFree(pTestData);
}

static void MGC_TransStartTests(void* pDeviceData, uint32_t dwIterationCount)
{
    MGC_TransInstance* pInstance = (MGC_TransInstance*)pDeviceData;

    pInstance->wTestCount = MUSB_ListLength(&(pInstance->TestDataList));
    pInstance->wTestIndex = 0;
    pInstance->pCurrentTest = (MGC_TransTestData*)MUSB_ListFindItem(
	&(pInstance->TestDataList), 0);

    MGC_TransTestStartTest(pInstance, pInstance->pCurrentTest);
}

static void MGC_TransStopTests(void* pDeviceData)
{
    /* TODO */
}

/** Our control IRP is complete */
static void MGC_TransControlIrpComplete(void* pPrivateData, MUSB_ControlIrp* pIrp)
{
    MUSB_DeviceEndpoint DeviceEnd;
    uint8_t bOk = FALSE;
    MGC_TransInstance* pInstance = (MGC_TransInstance*)pPrivateData;
    MGC_TransTestData* pTestData = pInstance->pCurrentTest;
    MUSB_TtiTestSpec* pTestSpec = pTestData->pTestSpec;

    pInstance->pCurrentTest = NULL;

    if(MUSB_STATUS_OK == pIrp->dwStatus)
    {
	/* we're OK; allocate buffers, open pipe(s) and add test data to list */
	if(MGC_TransAllocBuffers(pTestData))
	{
	    DeviceEnd.pDevice = pInstance->ControlIrp.pDevice;
	    DeviceEnd.UsbDescriptor.bLength = MUSB_DT_ENDPOINT_SIZE;
	    DeviceEnd.UsbDescriptor.bDescriptorType = MUSB_DT_ENDPOINT;
	    DeviceEnd.UsbDescriptor.bmAttributes = pTestSpec->bProtocol;
	    DeviceEnd.UsbDescriptor.wMaxPacketSize = MUSB_SWAP16(pTestSpec->wMaxPacketSize);
	    DeviceEnd.UsbDescriptor.bInterval = 1;
	    DeviceEnd.wNakLimit = 0xffff;
	    switch(pTestSpec->TestType)
	    {
	    case MUSB_TTI_TYPE_SINK:
		DeviceEnd.UsbDescriptor.bEndpointAddress = MUSB_DIR_OUT | pTestSpec->bOutEnd;
		pTestData->OutIrp.hPipe = pTestData->hOutPipe = MUSB_OpenPipe(
		    pInstance->hBus, &DeviceEnd, NULL);
		bOk = pTestData->hOutPipe ? TRUE : FALSE;
		break;
	    case MUSB_TTI_TYPE_SOURCE:
		DeviceEnd.UsbDescriptor.bEndpointAddress = MUSB_DIR_IN | pTestSpec->bInEnd;
		pTestData->InIrp.hPipe = pTestData->hInPipe = MUSB_OpenPipe(
		    pInstance->hBus, &DeviceEnd, NULL);
		bOk = pTestData->hInPipe ? TRUE : FALSE;
		break;
	    case MUSB_TTI_TYPE_LOOPBACK:
		DeviceEnd.UsbDescriptor.bEndpointAddress = MUSB_DIR_OUT | pTestSpec->bOutEnd;
		pTestData->OutIrp.hPipe = pTestData->hOutPipe = MUSB_OpenPipe(
		    pInstance->hBus, &DeviceEnd, NULL);
		DeviceEnd.UsbDescriptor.bEndpointAddress = MUSB_DIR_IN | pTestSpec->bInEnd;
		pTestData->InIrp.hPipe = pTestData->hInPipe = MUSB_OpenPipe(
		    pInstance->hBus, &DeviceEnd, NULL);
		bOk = (pTestData->hOutPipe && pTestData->hInPipe) ? TRUE : FALSE;
		break;
	    }
	    MUSB_ListAppendItem(&(pInstance->TestDataList), pTestData, 0);
	}
	else
	{
	    pTestData = NULL;
	}
    }
    else
    {
	pTestData = NULL;
    }

    /* inform client */
    pInstance->pfTestAddStatus(pTestSpec, pTestData);

    if(!pTestData || !bOk)
    {
	/* cleanup */
	if(pTestData && pTestData->hInPipe)
	{
	    MUSB_ClosePipe(pTestData->hInPipe);
	}
	if(pTestData && pTestData->hOutPipe)
	{
	    MUSB_ClosePipe(pTestData->hOutPipe);
	}
	if(pTestData)
	{
	    MGC_TransFreeBuffers(pTestData);
	    MUSB_MemFree(pTestData);
	}
    }
}

/**
 * Allocate the required buffers for a test
 */
static uint8_t MGC_TransAllocBuffers(MGC_TransTestData* pTestData)
{
#ifdef MUSB_ISO
    uint8_t* pBuf8;
    uint32_t* pBuf32;
#endif
    uint8_t bIndex;
    uint8_t bOk = FALSE;
    MUSB_TtiTestSpec* pTestSpec = pTestData->pTestSpec;
    uint32_t dwSize = pTestSpec->wMaxPacketSize * pTestSpec->bPacketCount;

    switch(pTestSpec->bProtocol)
    {
    case MUSB_ENDPOINT_XFER_BULK:
    case MUSB_ENDPOINT_XFER_INT:
	switch(pTestSpec->TestType)
	{
	case MUSB_TTI_TYPE_SINK:
	    pTestData->OutIrp.dwLength = dwSize;
	    pTestData->OutIrp.pBuffer = (uint8_t*)MUSB_MemAlloc(dwSize);
	    bOk = pTestData->OutIrp.pBuffer ? TRUE : FALSE;
	    break;
	case MUSB_TTI_TYPE_SOURCE:
	    pTestData->InIrp.dwLength = dwSize;
	    pTestData->InIrp.pBuffer = (uint8_t*)MUSB_MemAlloc(dwSize);
	    bOk = pTestData->InIrp.pBuffer ? TRUE : FALSE;
	    break;
	case MUSB_TTI_TYPE_LOOPBACK:
	    pTestData->InIrp.dwLength = dwSize;
	    pTestData->OutIrp.dwLength = dwSize;
	    pTestData->InIrp.pBuffer = (uint8_t*)MUSB_MemAlloc(dwSize);
	    pTestData->OutIrp.pBuffer = (uint8_t*)MUSB_MemAlloc(dwSize);
	    bOk = (pTestData->OutIrp.pBuffer && pTestData->InIrp.pBuffer) ? TRUE : FALSE;
	    break;
	}
	break;
#ifdef MUSB_ISO
    case MUSB_ENDPOINT_XFER_ISOC:
	switch(pTestSpec->TestType)
	{
	case MUSB_TTI_TYPE_SINK:
	    pTestData->OutIsochIrp.wFrameCount = pTestSpec->bPacketCount;
	    pBuf8 = (uint8_t*)MUSB_MemAlloc(dwSize);
	    if(pBuf8)
	    {
		pTestData->OutIsochIrp.pBuffer = pBuf8;
		pBuf32 = (uint32_t*)MUSB_MemAlloc(
		    3 * pTestSpec->bPacketCount * sizeof(uint32_t));
		if(pBuf32)
		{
		    pTestData->OutIsochIrp.adwLength = pBuf32;
		    pTestData->OutIsochIrp.adwStatus = (uint32_t*)
			((uint8_t*)pBuf32 + (pTestSpec->bPacketCount * sizeof(uint32_t)));
		    pTestData->OutIsochIrp.adwActualLength = (uint32_t*)
			((uint8_t*)pBuf32 + (2 * pTestSpec->bPacketCount * sizeof(uint32_t)));
		    for(bIndex = 0; bIndex < pTestSpec->bPacketCount; bIndex++)
		    {
			/* TODO: vary packet size */
			pTestData->OutIsochIrp.adwLength[bIndex] = pTestSpec->wMaxPacketSize;
			pTestData->OutIsochIrp.adwStatus[bIndex] = 0;
			pTestData->OutIsochIrp.adwActualLength[bIndex] = 0;
		    }
		}
		else
		{
		    MUSB_MemFree(pBuf8);
		}
	    }
	    bOk = pBuf8 ? TRUE : FALSE;
	    break;
	case MUSB_TTI_TYPE_SOURCE:
	    pTestData->InIsochIrp.wFrameCount = pTestSpec->bPacketCount;
	    pBuf8 = (uint8_t*)MUSB_MemAlloc(dwSize);
	    if(pBuf8)
	    {
		pTestData->InIsochIrp.pBuffer = pBuf8;
		pBuf32 = (uint32_t*)MUSB_MemAlloc(
		    3 * pTestSpec->bPacketCount * sizeof(uint32_t));
		if(pBuf32)
		{
		    pTestData->InIsochIrp.adwLength = pBuf32;
		    pTestData->InIsochIrp.adwStatus = (uint32_t*)
			((uint8_t*)pBuf32 + 
			(pTestSpec->bPacketCount * sizeof(uint32_t)));
		    pTestData->InIsochIrp.adwActualLength = (uint32_t*)
			((uint8_t*)pBuf32 + 
			(2 * pTestSpec->bPacketCount * sizeof(uint32_t)));
		    for(bIndex = 0; bIndex < pTestSpec->bPacketCount; bIndex++)
		    {
			pTestData->InIsochIrp.adwLength[bIndex] = pTestSpec->wMaxPacketSize;
			pTestData->InIsochIrp.adwStatus[bIndex] = 0;
			pTestData->InIsochIrp.adwActualLength[bIndex] = 0;
		    }
		}
		else
		{
		    MUSB_MemFree(pBuf8);
		}
	    }
	    bOk = pBuf8 ? TRUE : FALSE;
	    break;
	case MUSB_TTI_TYPE_LOOPBACK:
	    pTestData->InIsochIrp.wFrameCount = pTestSpec->bPacketCount;
	    pBuf8 = (uint8_t*)MUSB_MemAlloc(2*dwSize);
	    if(pBuf8)
	    {
		pTestData->OutIsochIrp.pBuffer = pBuf8;
		pTestData->InIsochIrp.pBuffer = (uint8_t*)(pBuf8 + dwSize);
		pBuf32 = (uint32_t*)MUSB_MemAlloc(
		    6 * pTestSpec->bPacketCount * sizeof(uint32_t));
		if(pBuf32)
		{
		    pTestData->OutIsochIrp.adwLength = pBuf32;
		    pTestData->InIsochIrp.adwLength = (uint32_t*)
			((uint8_t*)pBuf32 + (pTestSpec->bPacketCount * sizeof(uint32_t)));
		    pTestData->OutIsochIrp.adwStatus = (uint32_t*)
			((uint8_t*)pBuf32 + (2 * pTestSpec->bPacketCount * sizeof(uint32_t)));
		    pTestData->InIsochIrp.adwStatus = (uint32_t*)
			((uint8_t*)pBuf32 + (3 * pTestSpec->bPacketCount * sizeof(uint32_t)));
		    pTestData->OutIsochIrp.adwActualLength = (uint32_t*)
			((uint8_t*)pBuf32 + (4 * pTestSpec->bPacketCount * sizeof(uint32_t)));
		    pTestData->InIsochIrp.adwActualLength = (uint32_t*)
			((uint8_t*)pBuf32 + (5 * pTestSpec->bPacketCount * sizeof(uint32_t)));
		    for(bIndex = 0; bIndex < pTestSpec->bPacketCount; bIndex++)
		    {
			/* TODO: vary packet size */
			pTestData->OutIsochIrp.adwLength[bIndex] = pTestSpec->wMaxPacketSize;
			pTestData->InIsochIrp.adwLength[bIndex] = pTestSpec->wMaxPacketSize;
			pTestData->OutIsochIrp.adwStatus[bIndex] = 0;
			pTestData->InIsochIrp.adwStatus[bIndex] = 0;
			pTestData->OutIsochIrp.adwActualLength[bIndex] = 0;
			pTestData->InIsochIrp.adwActualLength[bIndex] = 0;
		    }
		}
		else
		{
		    MUSB_MemFree(pBuf8);
		}
	    }
	    bOk = pBuf8 ? TRUE : FALSE;
	    break;
	}
	break;
#endif
    }
}

/**
 * Free the buffers used for a given test
 */
static void MGC_TransFreeBuffers(MGC_TransTestData* pTestData)
{
    MUSB_TtiTestSpec* pTestSpec = pTestData->pTestSpec;

    switch(pTestSpec->bProtocol)
    {
    case MUSB_ENDPOINT_XFER_BULK:
    case MUSB_ENDPOINT_XFER_INT:
	switch(pTestSpec->TestType)
	{
	case MUSB_TTI_TYPE_SINK:
	    if(pTestData->OutIrp.pBuffer)
	    {
		MUSB_MemFree(pTestData->OutIrp.pBuffer);
	    }
	    break;
	case MUSB_TTI_TYPE_SOURCE:
	    if(pTestData->InIrp.pBuffer)
	    {
		MUSB_MemFree(pTestData->InIrp.pBuffer);
	    }
	    break;
	case MUSB_TTI_TYPE_LOOPBACK:
	    if(pTestData->InIrp.pBuffer)
	    {
		MUSB_MemFree(pTestData->InIrp.pBuffer);
	    }
	    if(pTestData->OutIrp.pBuffer)
	    {
		MUSB_MemFree(pTestData->OutIrp.pBuffer);
	    }
	    break;
	}
	break;
#ifdef MUSB_ISO
    case MUSB_ENDPOINT_XFER_ISOC:
	switch(pTestSpec->TestType)
	{
	case MUSB_TTI_TYPE_SINK:
	    if(pTestData->OutIsochIrp.pBuffer)
	    {
		MUSB_MemFree(pTestData->OutIsochIrp.adwLength);
		MUSB_MemFree(pTestData->OutIsochIrp.pBuffer);
	    }
	    break;
	case MUSB_TTI_TYPE_SOURCE:
	    if(pTestData->InIsochIrp.pBuffer)
	    {
		MUSB_MemFree(pTestData->InIsochIrp.adwLength);
		MUSB_MemFree(pTestData->InIsochIrp.pBuffer);
	    }
	    break;
	case MUSB_TTI_TYPE_LOOPBACK:
	    if(pTestData->OutIsochIrp.pBuffer)
	    {
		MUSB_MemFree(pTestData->OutIsochIrp.adwLength);
		MUSB_MemFree(pTestData->OutIsochIrp.pBuffer);
	    }
	    break;
	}
	break;
#endif
    }
}

/**
 * Start a test
 */
static void MGC_TransTestStartTest(MGC_TransInstance* pInstance, 
				   MGC_TransTestData* pTestData)
{
    uint32_t dwStatus;
    MUSB_TtiTestSpec* pTestSpec = pTestData->pTestSpec;
    uint32_t dwSize = pTestSpec->wMaxPacketSize * pTestSpec->bPacketCount;

    switch(pTestSpec->bProtocol)
    {
    case MUSB_ENDPOINT_XFER_BULK:
    case MUSB_ENDPOINT_XFER_INT:
	switch(pTestSpec->TestType)
	{
	case MUSB_TTI_TYPE_SOURCE:
	    dwStatus = MUSB_StartTransfer(&(pTestData->InIrp));
	    break;
	case MUSB_TTI_TYPE_SINK:
	case MUSB_TTI_TYPE_LOOPBACK:
	    MGC_TransFillBuffer(pTestSpec, pTestData->OutIrp.pBuffer, dwSize);
	    dwStatus = MUSB_StartTransfer(&(pTestData->OutIrp));
	    break;
	}
	break;
#ifdef MUSB_ISO
    case MUSB_ENDPOINT_XFER_ISOC:
	switch(pTestSpec->TestType)
	{
	case MUSB_TTI_TYPE_SOURCE:
	    dwStatus = MUSB_ScheduleIsochTransfer(0, &(pTestData->InIsochIrp));
	    break;
	case MUSB_TTI_TYPE_SINK:
	case MUSB_TTI_TYPE_LOOPBACK:
	    MGC_TransFillBuffer(pTestSpec, pTestData->OutIsochIrp.pBuffer, dwSize);
	    dwStatus = MUSB_ScheduleIsochTransfer(0, &(pTestData->OutIsochIrp));
	    break;
	}
	break;
#endif
    }

}

/**
 * Fill buffer with implied pattern
 */
static void MGC_TransFillBuffer(MUSB_TtiTestSpec* pTestSpec,
				uint8_t* pBuffer, uint32_t dwSize)
{
    uint32_t dwIndex;
    uint8_t bTemp;
    uint8_t bDatum = pTestSpec->bStartPattern;

    for(dwIndex = 0; dwIndex < dwSize; dwIndex++)
    {
	pBuffer[dwIndex] = bDatum;
	switch(pTestSpec->TestOption)
	{
	case MUSB_TTI_OPTION_INCREMENT:
	    bDatum += 1;
	    break;
	case MUSB_TTI_OPTION_DECREMENT:
	    bDatum -= 1;
	    break;
	case MUSB_TTI_OPTION_ROTATE_LEFT:
	    bTemp = (bDatum >> 7);
	    bDatum = (bDatum << 1) | bTemp;
	    break;
	case MUSB_TTI_OPTION_ROTATE_RIGHT:
	    bTemp = (bDatum << 7);
	    bDatum = (bDatum >> 1) | bTemp;
	    break;
	}
    }
}

/**
 * Compare a buffer with what it should be
 * @return the number of failed comparisons
 */
static uint32_t MGC_TransCompareBuffer(MUSB_TtiTestSpec* pTestSpec,
				       uint8_t* pBuffer, uint32_t dwSize)
{
    uint32_t dwIndex;
    uint8_t bTemp;
    uint8_t bDatum = pTestSpec->bStartPattern;
    uint32_t dwFailures = 0;

    for(dwIndex = 0; dwIndex < dwSize; dwIndex++)
    {
	if(pBuffer[dwIndex] != bDatum)
	{
	    dwFailures++;
	}
	switch(pTestSpec->TestOption)
	{
	case MUSB_TTI_OPTION_INCREMENT:
	    bDatum += 1;
	    break;
	case MUSB_TTI_OPTION_DECREMENT:
	    bDatum -= 1;
	    break;
	case MUSB_TTI_OPTION_ROTATE_LEFT:
	    bTemp = (bDatum >> 7);
	    bDatum = (bDatum << 1) | bTemp;
	    break;
	case MUSB_TTI_OPTION_ROTATE_RIGHT:
	    bTemp = (bDatum << 7);
	    bDatum = (bDatum >> 1) | bTemp;
	    break;
	}
    }
    return dwFailures;
}

/**
 * A bulk/interrupt-IN is complete
 */
static void MGC_TransTestInComplete(void *pPrivateData, MUSB_Irp *pIrp)
{
    uint32_t dwFailures;
    MGC_TransInstance* pInstance = (MGC_TransInstance*)pPrivateData;
    MGC_TransTestData* pTestData = pInstance->pCurrentTest;
    MUSB_TtiTestSpec* pTestSpec = pTestData->pTestSpec;

    if(MUSB_STATUS_OK != pIrp->dwStatus)
    {
	pTestData->pfTestError(pTestSpec, pIrp->dwStatus, 0);
	pTestSpec->dwErrorIterations++;
    }
    pTestSpec->dwActualIterations++;
    switch(pTestSpec->TestType)
    {
    case MUSB_TTI_TYPE_SOURCE:
    case MUSB_TTI_TYPE_LOOPBACK:
	/* compare */
	dwFailures = MGC_TransCompareBuffer(pTestSpec, pIrp->pBuffer,
	    pTestSpec->wMaxPacketSize * pTestSpec->bPacketCount);
	if(dwFailures)
	{
	    pTestData->pfTestError(pTestSpec, pIrp->dwStatus, dwFailures);
	    pTestSpec->dwErrorIterations++;
	}
	pTestSpec->bStartPattern = pIrp->pBuffer[1];
	break;
    }

    /* inform client */
    pTestData->pfTestProgress(pTestSpec);

    /* next test or continue this test */
    MGC_TransNextTest(pInstance);
}

/**
 * A bulk/interrupt-OUT is complete
 */
static void MGC_TransTestOutComplete(void *pPrivateData, MUSB_Irp *pIrp)
{
    uint32_t dwStatus;
    MGC_TransInstance* pInstance = (MGC_TransInstance*)pPrivateData;
    MGC_TransTestData* pTestData = pInstance->pCurrentTest;
    MUSB_TtiTestSpec* pTestSpec = pTestData->pTestSpec;

    if(MUSB_STATUS_OK != pIrp->dwStatus)
    {
	pTestData->pfTestError(pTestSpec, pIrp->dwStatus, 0);
	pTestSpec->dwErrorIterations++;
    }
    switch(pTestSpec->TestType)
    {
    case MUSB_TTI_TYPE_LOOPBACK:
	/* start IN phase */
	dwStatus = MUSB_StartTransfer(&(pTestData->InIrp));
	if(MUSB_STATUS_OK != dwStatus)
	{
	    pTestData->pfTestError(pTestSpec, dwStatus, 0);
	}
	break;
    case MUSB_TTI_TYPE_SINK:
	pTestSpec->dwActualIterations++;
	pTestSpec->bStartPattern = pIrp->pBuffer[1];
	/* inform client */
	pTestData->pfTestProgress(pTestSpec);

	/* next test or continue this test */
	MGC_TransNextTest(pInstance);
	break;
    }

}

#ifdef MUSB_ISO
static void MGC_TransTestIsochInComplete(void *pPrivateData, MUSB_IsochIrp *pIrp)
{
    uint16_t wFrame;
    uint32_t dwFailures;
    uint32_t dwOffset;
    MGC_TransInstance* pInstance = (MGC_TransInstance*)pPrivateData;
    MGC_TransTestData* pTestData = pInstance->pCurrentTest;
    MUSB_TtiTestSpec* pTestSpec = pTestData->pTestSpec;

    for(wFrame = dwOffset = 0; wFrame < pIrp->wFrameCount; wFrame++)
    {
	if(MUSB_STATUS_OK != pIrp->adwStatus[wFrame])
	{
	    pTestData->pfTestError(pTestSpec, pIrp->adwStatus[wFrame], 0);
	    pTestSpec->dwErrorIterations++;
	}
	pTestSpec->dwActualIterations++;
	switch(pTestSpec->TestType)
	{
	case MUSB_TTI_TYPE_SOURCE:
	case MUSB_TTI_TYPE_LOOPBACK:
	    /* compare */
	    dwFailures = MGC_TransCompareBuffer(pTestSpec, pIrp->pBuffer + dwOffset,
		pIrp->adwActualLength[wFrame]);
	    dwOffset += pIrp->adwActualLength[wFrame];
	    if(dwFailures)
	    {
		pTestData->pfTestError(pTestSpec, pIrp->adwStatus[wFrame], dwFailures);
		pTestSpec->dwErrorIterations++;
	    }
	    pTestSpec->bStartPattern = pIrp->pBuffer[1];
	    break;
	}

	/* inform client */
	pTestData->pfTestProgress(pTestSpec);

	/* next test or continue this test */
	MGC_TransNextTest(pInstance);
    }
}

static void MGC_TransTestIsochOutComplete(void *pPrivateData, MUSB_IsochIrp *pIrp)
{
    uint16_t wFrame;
    uint32_t dwStatus;
    MGC_TransInstance* pInstance = (MGC_TransInstance*)pPrivateData;
    MGC_TransTestData* pTestData = pInstance->pCurrentTest;
    MUSB_TtiTestSpec* pTestSpec = pTestData->pTestSpec;

    for(wFrame = 0; wFrame < pIrp->wFrameCount; wFrame++)
    {
	if(MUSB_STATUS_OK != pIrp->adwStatus[wFrame])
	{
	    pTestData->pfTestError(pTestSpec, pIrp->adwStatus[wFrame], 0);
	    pTestSpec->dwErrorIterations++;
	}
    }

    switch(pTestSpec->TestType)
    {
    case MUSB_TTI_TYPE_LOOPBACK:
	/* start IN phase */
	dwStatus = MUSB_ScheduleIsochTransfer(0, &(pTestData->InIsochIrp));
	if(MUSB_STATUS_OK != dwStatus)
	{
	    pTestData->pfTestError(pTestSpec, dwStatus, 0);
	}
	break;
    case MUSB_TTI_TYPE_SINK:
	pTestSpec->dwActualIterations++;
	pTestSpec->bStartPattern = pIrp->pBuffer[1];
	/* inform client */
	pTestData->pfTestProgress(pTestSpec);

	/* next test or continue this test */
	MGC_TransNextTest(pInstance);
	break;
    }

}
#endif

static void MGC_TransNextTest(MGC_TransInstance* pInstance)
{
    MGC_TransTestData* pTestData = pInstance->pCurrentTest;
    MUSB_TtiTestSpec* pTestSpec = pTestData->pTestSpec;

    if(pTestSpec->dwActualIterations < pTestSpec->dwIterations)
    {
	/* continue with next iteration */
	MGC_TransTestStartTest(pInstance, pInstance->pCurrentTest);
    }
    else
    {
	/* next test if any */
	pTestSpec->dwActualIterations = 0;
	pInstance->wTestIndex++;
	if(pInstance->wTestIndex < pInstance->wTestCount)
	{
	    pInstance->pCurrentTest = (MGC_TransTestData*)MUSB_ListFindItem(
		&(pInstance->TestDataList), pInstance->wTestIndex);
	    MGC_TransTestStartTest(pInstance, pInstance->pCurrentTest);
	}
    }
}
