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
 * Command-line interface (really menu-driven) for transport driver
 * $Revision: #1 $
 */

#include "mu_cdi.h"
#include "mu_diag.h"
#include "mu_mem.h"
#include "mu_stdio.h"
#include "mu_strng.h"

#include "class/mu_trans.h"
#include "mu_tapi.h"
#include "mu_tfunc.h"

#ifdef UCOS
#include "plat_cnf.h"
#include "includes.h"
#endif

#define MUSB_TRANS_APP_ITER_COUNT    4

/***************************** TYPES ******************************/

typedef void (*MUSB_pfAppCallback)(void); 

/*************************** FORWARDS *****************************/

static void MGC_TransNewOtgState(void* hClient, MUSB_BusHandle hBus, 
				 MUSB_OtgState State);

static void MGC_TransOtgError(void* hClient, MUSB_BusHandle hBus, 
			      uint32_t dwStatus);

uint8_t
MUSB_CliTransportTest(void  *pPrivateData, 
                      void  (*pfApplicationCallback) (void));

void MUSB_TransDisplayTestResults (void  *pPrivateData);

#ifdef UCOS
static void MGC_UcosAppThread(void* pContext);
#endif

/**************************** GLOBALS *****************************/

#ifdef UCOS
OS_EVENT*        MGC_AppSem;
static uint32_t   MGC_aUcosAppTaskStack[MUSB_UCOS_TASK_STACK_SIZE];
#endif

static MUSB_Port*     MGC_pCdiPort    = NULL;
static MUSB_BusHandle MGC_hCdiSession = NULL;

static uint8_t MGC_bDesireHostRole    = FALSE;

static const uint8_t MGC_aTransPeripheralList[] =
{
    MUSB_TARGET_VID, 0xD6, 0x04, MUSB_TARGET_PID, 0x33, 0x22, MUSB_TARGET_ACCEPT, 0
};

static MUSB_HostClient MGC_TransHostClient = 
{
    MGC_aTransPeripheralList,		/* peripheral list */
    sizeof(MGC_aTransPeripheralList),	/* peripheral list length */
    NULL,
    1
};

static MUSB_OtgClient MGC_TransOtgClient = 
{
    NULL,	/* no instance data; we are singleton */
    &MGC_bDesireHostRole,
    MGC_TransNewOtgState,
    MGC_TransOtgError
};

/*************************** FUNCTIONS ****************************/

int32_t main(int32_t argc, CHAR* argv[])
{
#ifdef UCOS
    INT8U       err;
#endif
    uint8_t bOk;
    MUSB_FunctionClient* pFunction = MGC_GetTransportFunctionClient();
    MUSB_DeviceDriver* pDriver = MGC_GetTransportDeviceDriver();

    if(!pFunction || !pDriver)
    {
	MUSB_PrintLine("Could not get function and/or driver");
	return -1;
    }
    MGC_TransHostClient.aDeviceDriverList = pDriver;

#ifdef UCOS
    OSInit();
#endif

    bOk = MUSB_InitSystem(5);
    if(!bOk)
    {
	MUSB_PrintLine("System initialization failure");
	return -1;
    }

    MGC_pCdiPort = MUSB_GetPort(0);
    if(!MGC_pCdiPort)
    {
	MUSB_PrintLine("Could not find a CDI port");
	return -1;
    }

    /* Register OTG Client with UCD */
    MGC_hCdiSession = 
	MUSB_RegisterOtgClient(MGC_pCdiPort, 
	pFunction, 
	&MGC_TransHostClient, 
	&MGC_TransOtgClient); 

    if(!MGC_hCdiSession)
    {
	MUSB_PrintLine("Could not open a CDI session");
	return -1;
    }

#ifdef UCOS
    /* Create semaphore */
    MGC_AppSem = OSSemCreate(MGC_UCOS_BINARY_SEM_COUNT);
    if(!MGC_AppSem)
    {
	MUSB_PrintLine("Error: Semaphore Create");
	return -1;
    }    
    /* Create an Application thread,  Top of the stack has to be passed  */
    err = OSTaskCreate( (void (*) (void *))MGC_UcosAppThread, 
	pDriver->pPrivateData, 
	(OS_STK *)(&MGC_aUcosAppTaskStack[MUSB_UCOS_TASK_STACK_SIZE-1]), 
	(INT8U) MGC_UCOS_LOWEST_PRIORITY );
    if(OS_NO_ERR != err)
    {
	MUSB_PrintLine("Error: Thread Create");
	return -1;
    }
    OSStart();
#else
    while(TRUE)
    {
	MUSB_NoneRunBackground();
    }
#endif

}

#ifdef UCOS
/* Application complete callback */
static void MGC_UcosAppCallback(void)
{
    /* Relese the semaphore locked for the application request*/
    OSSemPost(MGC_AppSem);
    MUSB_PrintLine("Application Call Back Called");
}

/* Application thread */
static void MGC_UcosAppThread(void* pContext)
{
    INT8U      err;
    uint8_t    bOk = TRUE;

    OSSemPend(MGC_AppSem, MGC_UCOS_SUSPEND, &err);

    while(1)
    {
	bOk = MUSB_CliTransportTest(pContext, MGC_UcosAppCallback);
	if(bOk == TRUE)
	{
	    OSSemPend(MGC_AppSem, MGC_UCOS_SUSPEND, &err);
	}
    }
}
#endif

static void MGC_TransNewOtgState(void* hClient, MUSB_BusHandle hBus, 
			       MUSB_OtgState State)
{
    MUSB_DIAG1(1, "Otg State : ", (uint32_t) State, 16, TRUE);
}

static void MGC_TransOtgError(void* hClient, MUSB_BusHandle hBus, 
			    uint32_t dwStatus)
{
    switch(dwStatus)
    {
    case MUSB_STATUS_OTG_NO_RESPONSE:
	MUSB_PrintLine("Device not responding");
	break;
    case MUSB_STATUS_UNSUPPORTED_DEVICE:
	MUSB_PrintLine("Device not supported");
	break;
    case MUSB_STATUS_UNSUPPORTED_HUB:
	MUSB_PrintLine("Hubs not supported");
	break;
    case MUSB_STATUS_OTG_SRP_FAILED:
	MUSB_PrintLine("SRP failed");
	break;
    default:
	MUSB_PrintLine("Unknown OTG Error");
    }
}

uint8_t
MUSB_CliTransportTest(void  *pPrivateData, 
                      void  (*pfApplicationCallback) (void))
{
    CHAR        aAnswer[] = { 0xa6, 0, 0, 0xf1 };

    uint8_t     aValidEndpointsIndex[16];
    uint8_t     aValidRemoteEndpoints[16];

    uint16_t    aBufferSize[256];
    uint8_t     bNumEndpoints;

    uint8_t     bEndpointIndex;
    uint8_t     bRemoteEndpoint;
    uint8_t     bTestType;
    uint8_t     bTestPattern;
    uint32_t    dwMinBufferSize;
    uint32_t    dwStepSize;
#if 0
    uint32_t    dwMaxBufferSize;
#endif
    uint32_t    wMaxPacketSize;
    uint8_t     bIndex;
    uint32_t    dwNumIterations;
    const CHAR* pEnd;
    uint8_t     bCount;
    uint32_t    dwStatus;
    uint8_t     bChoice;
    uint8_t     bSelected = FALSE;
    CHAR        aTransportDebugMsg[256];
    CHAR        aTransportDebugNum[33];

    MUSB_PrintLine ("Test to Start ");
    MUSB_PrintLine ("1. Source Test ");
    MUSB_PrintLine ("2. Sink Test ");
    MUSB_PrintLine ("3. Loopback Test ");
    MUSB_PrintLine ("4. Display Result");
    MUSB_PrintLine ("-----------------");

    bCount = 0;
    while (bCount < MUSB_TRANS_APP_ITER_COUNT)
    {
        MUSB_PrintString ("Enter Your Choice : ");
        MUSB_GetLine(aAnswer, 4);
        bChoice = (uint8_t) MUSB_StringParse (aAnswer, &pEnd, 10);

        switch (bChoice)
        {
        case MGC_TRANSPORT_SOURCE_TEST:
            {
                /* Start Source Test */
                MUSB_PrintLine("\nSelect one of the Following endpoints for Source Test");
                bTestType = (uint8_t) MGC_TRANSPORT_SOURCE_TEST;
                bSelected = TRUE;
                break;
            }

        case MGC_TRANSPORT_SINK_TEST:
            {
                /* Start Sink Test */
                MUSB_PrintLine("Select one of the Following endpoints for Sink Test");
                bTestType = (uint8_t) MGC_TRANSPORT_SINK_TEST;
                bSelected = TRUE;
                break;
            }

        case MGC_TRANSPORT_LOOPBACK_TEST:
            {
                /* Start Loopback Test */
                MUSB_PrintLine("Select one of the Following endpoints for Loopback Test");
                bTestType = (uint8_t) MGC_TRANSPORT_LOOPBACK_TEST;
                bSelected = TRUE;
                break;
            }
	case MGC_TRANSPORT_DISPLAY_RESULT:
	    {
		/* call Function to display results */
		MUSB_TransDisplayTestResults(pPrivateData);

		MUSB_PrintLine ("Test to Start ");
		MUSB_PrintLine ("1. Source Test ");
		MUSB_PrintLine ("2. Sink Test ");
		MUSB_PrintLine ("3. Loopback Test ");
		MUSB_PrintLine ("4. Display Result");
		MUSB_PrintLine ("-----------------");
		break;
	    }
        default:
            {
                MUSB_PrintLine("Invalid Choice\nPlease Try again\n");
                bCount++;
                bSelected = FALSE;
            }
        }
        if (TRUE == bSelected)
        {
            break;
        }
    }

    if (MUSB_TRANS_APP_ITER_COUNT == bCount)
    {
        return FALSE;
    }

    /* Display Endpoint informations for different tests */
    dwStatus = MUSB_TransDisplayEndpointDetails (bChoice, pPrivateData, &bNumEndpoints, 
	aValidEndpointsIndex, aValidRemoteEndpoints, aBufferSize);

    if (MUSB_STATUS_OK != dwStatus)
    {
        return FALSE;
    }

    bCount = 0;
    bSelected = FALSE;
    while (bCount < MUSB_TRANS_APP_ITER_COUNT)
    {
        MUSB_PrintString("Enter Endpoint Number : ");
        MUSB_GetLine(aAnswer, 4);
        bRemoteEndpoint = MUSB_StringParse (aAnswer, &pEnd, 10);

        for (bIndex = 0; bIndex < bNumEndpoints; bIndex++)
        {
            if ( aValidRemoteEndpoints[bIndex] == bRemoteEndpoint)
            {
		bEndpointIndex  = aValidEndpointsIndex[bIndex];
                bSelected = TRUE;
                break;
            }
        }
        if (TRUE == bSelected)
        {
            MUSB_PrintLine("");
            break;
        }
        MUSB_PrintLine("Invalid Choice\nPlease Try again\n");
        bCount++;
        bSelected = FALSE;
    }
    if (MUSB_TRANS_APP_ITER_COUNT == bCount)
    {
        return FALSE;
    }

    wMaxPacketSize = aBufferSize[bIndex];

    bCount = 0;
    while (bCount < MUSB_TRANS_APP_ITER_COUNT)
    {
        aTransportDebugMsg[0] = (CHAR) 0;
        aTransportDebugNum[0] = (CHAR) 0;

        MUSB_StringConcat(aTransportDebugMsg, 256,"Enter Minimum Packet Size (1.. ");
        MUSB_Stringize(aTransportDebugNum, 33, wMaxPacketSize, 10, TRUE);
        MUSB_StringConcat(aTransportDebugMsg,256,aTransportDebugNum);
        MUSB_StringConcat(aTransportDebugMsg, 256," ) : ");
        MUSB_PrintString(aTransportDebugMsg);

        MUSB_GetLine(aAnswer, 4);
        dwMinBufferSize = MUSB_StringParse (aAnswer, &pEnd, 10);
    
        if ((0 != dwMinBufferSize) && (dwMinBufferSize <= wMaxPacketSize))
        {
            MUSB_PrintLine("");
            break;
        }

        MUSB_PrintLine("\nInvalid Minimum Packet size..");
        bCount++;
    }

    if (MUSB_TRANS_APP_ITER_COUNT == bCount)
    {
        return (uint8_t)-1;
    }

    bCount = 0;
    while (bCount < MUSB_TRANS_APP_ITER_COUNT)
    {
        aTransportDebugMsg[0] = (CHAR) 0;
        aTransportDebugNum[0] = (CHAR) 0;

        MUSB_StringConcat(aTransportDebugMsg, 256,"Enter Step Size (>=0) : ");        
        MUSB_PrintString(aTransportDebugMsg);

        MUSB_GetLine(aAnswer, 4);
        dwStepSize = MUSB_StringParse (aAnswer, &pEnd, 10);
        MUSB_PrintLine("");
        break;
    }

    if (MUSB_TRANS_APP_ITER_COUNT == bCount)
    {
        return FALSE;
    }


    bCount = 0;
    while (bCount < MUSB_TRANS_APP_ITER_COUNT)
    {
        MUSB_PrintString("Enter Number of Iterations : ");
        MUSB_GetLine(aAnswer, 4);
        dwNumIterations = MUSB_StringParse (aAnswer, &pEnd, 10);

        if (0 != dwNumIterations)
        {
            MUSB_PrintLine("");
            break;
        }
        MUSB_PrintLine("Invalid Number of Iterations..");
        bCount++;

    }

    if (MUSB_TRANS_APP_ITER_COUNT == bCount)
    {
        return FALSE;
    }

    MUSB_PrintLine("Select Test Pattern ");
    MUSB_PrintLine("--------------------");

    MUSB_PrintLine("1. All Zero bits");
    MUSB_PrintLine("2. All 1 bits");
    MUSB_PrintLine("3. Alternating 0/1.");
    MUSB_PrintLine("4. Walking 0's");
    MUSB_PrintLine("5. Walking 1's");
    MUSB_PrintLine("6. Random Data");

    bCount = 0;
    while (bCount < MUSB_TRANS_APP_ITER_COUNT)
    {
        MUSB_PrintString("Enter Test Pattern : ");
        MUSB_GetLine(aAnswer, 4);

        bTestPattern = (uint8_t) MUSB_StringParse (aAnswer, &pEnd, 10);

        if ((7 > bTestPattern) && (0 != bTestPattern))
        {
            MUSB_PrintLine("");
            break;
        }
        MUSB_PrintLine("Invalid Test Pattern..");
        bCount++;

    }

    if (MUSB_TRANS_APP_ITER_COUNT == bCount)
    {
        return FALSE;
    }

    dwStatus = MUSB_TransStartTest (pPrivateData, 
                                    bEndpointIndex,
                                    bTestType,
                                    bTestPattern,
                                    dwMinBufferSize,
                                    dwStepSize,   /*  dwMaxBufferSize, */
                                    dwNumIterations,
                                    pfApplicationCallback);

    if (MUSB_STATUS_OK != dwStatus)
    {
        MUSB_PrintLine("Test Failed\nPlease try again");
    }
    return (MUSB_STATUS_OK == dwStatus) ? TRUE : FALSE;
}


void
MUSB_TransDisplayTestResults (void  *pPrivateData)
{
    MUSB_TransportTestResult    *pTestResult;
    MGC_TransportTestSequence   *pTestSequence;
    CHAR                        aDebugMessage[256];
    CHAR                        aDebugNumber[33];
    CHAR                        aTestName[4][20] = {"Source Test","Sink Test","Loopback Test","UNKNOWN"};
    uint8_t                     bTestType;
    uint32_t                    dwIndex;

    pTestResult = MUSB_TransGetTestResult (pPrivateData);

    aDebugMessage[0] = (CHAR)0;
    aDebugNumber[0]  = (CHAR)0;

    MUSB_StringConcat(aDebugMessage, 256,"Number of Test Executed : ");
    MUSB_Stringize(aDebugNumber, 33, pTestResult->dwNumTestsExecuted, 10, TRUE);
    MUSB_StringConcat(aDebugMessage, 256,aDebugNumber);
    MUSB_PrintLine(aDebugMessage);

    if(!pTestResult->dwNumTestsExecuted)
    {
        /* No test is executed */
        return;
    }
    aDebugMessage[0] = (CHAR)0;
    aDebugNumber[0]  = (CHAR)0;

    MUSB_StringConcat(aDebugMessage, 256,"Number of Test Passed : ");
    MUSB_Stringize(aDebugNumber, 33, pTestResult->dwNumTestsPassed, 10, TRUE);
    MUSB_StringConcat(aDebugMessage, 256,aDebugNumber);
    MUSB_PrintLine(aDebugMessage);

    aDebugMessage[0] = (CHAR)0;
    aDebugNumber[0]  = (CHAR)0;

    MUSB_StringConcat(aDebugMessage, 256,"Number of Test Failed : ");
    MUSB_Stringize(aDebugNumber, 33, pTestResult->dwNumTestsFailed, 10, TRUE);
    MUSB_StringConcat(aDebugMessage, 256,aDebugNumber);
    MUSB_PrintLine(aDebugMessage);

    aDebugMessage[0] = (CHAR)0;
    aDebugNumber[0]  = (CHAR)0;
    if ( pTestResult->dwSequenceIndex != pTestResult->dwNumTestsExecuted)
    {
        MUSB_StringConcat(aDebugMessage, 256,"Displaying Last ");
        /*As Sequence indexing starts from zero, adds 1 to print the result number */
        MUSB_Stringize(aDebugNumber, 33, pTestResult->dwSequenceIndex+1, 10, TRUE);
        MUSB_StringConcat(aDebugMessage, 256,aDebugNumber);
        MUSB_StringConcat(aDebugMessage, 256," Result(s)");
        MUSB_PrintLine(aDebugMessage);
    }

    for (dwIndex = 0; dwIndex < pTestResult->dwSequenceIndex; dwIndex++)
    {
        pTestSequence = &(pTestResult->aSequence[dwIndex]);

        bTestType = pTestSequence->bTestType - 1;

        /** All Unknown Tests belong to one group. */
        if (bTestType > 3)
        {
            bTestType = 3;
        }

        aDebugMessage[0] = (CHAR)0;
        aDebugNumber[0]  = (CHAR)0;

        MUSB_Stringize(aDebugNumber, 33, dwIndex, 10, TRUE);
        MUSB_StringConcat(aDebugMessage, 256,aDebugNumber);
        MUSB_StringConcat(aDebugMessage, 256," : ");
        MUSB_StringConcat(aDebugMessage, 256, aTestName[bTestType]);

        if (MUSB_STATUS_OK == pTestSequence->dwStatus)
        {
            MUSB_StringConcat(aDebugMessage, 256," : Passed");
        }
        else
        {
            MUSB_StringConcat(aDebugMessage, 256," : Failed");
        }

        MUSB_PrintLine(aDebugMessage);
        MUSB_PrintLine("-------------------------------------------");

        aDebugMessage[0] = (CHAR)0;
        aDebugNumber[0]  = (CHAR)0;

        MUSB_StringConcat(aDebugMessage, 256,"\tEndpoint Index : ");
        MUSB_Stringize(aDebugNumber, 33, pTestSequence->bEndpointIndex, 10, TRUE);
        MUSB_StringConcat(aDebugMessage, 256,aDebugNumber);
        MUSB_PrintLine(aDebugMessage);


        aDebugMessage[0] = (CHAR)0;
        aDebugNumber[0]  = (CHAR)0;

        MUSB_StringConcat(aDebugMessage, 256,"\tPacktes Requested : ");
        MUSB_Stringize(aDebugNumber, 33, pTestSequence->dwNumIterations, 10, TRUE);
        MUSB_StringConcat(aDebugMessage, 256,aDebugNumber);
        MUSB_PrintLine(aDebugMessage);

        aDebugMessage[0] = (CHAR)0;
        aDebugNumber[0]  = (CHAR)0;

        MUSB_StringConcat(aDebugMessage, 256,"\tPacktes Executed : ");
        MUSB_Stringize(aDebugNumber, 33, pTestSequence->dwNumPackets, 10, TRUE);
        MUSB_StringConcat(aDebugMessage, 256,aDebugNumber);
        MUSB_PrintLine(aDebugMessage);

        aDebugMessage[0] = (CHAR)0;
        aDebugNumber[0]  = (CHAR)0;

        MUSB_StringConcat(aDebugMessage, 256,"\tNumber of Mismatches : ");
        MUSB_Stringize(aDebugNumber, 33, pTestSequence->dwNumMismatches, 10, TRUE);
        MUSB_StringConcat(aDebugMessage, 256,aDebugNumber);
        MUSB_PrintLine(aDebugMessage);

        aDebugMessage[0] = (CHAR)0;
        aDebugNumber[0]  = (CHAR)0;

        MUSB_StringConcat(aDebugMessage, 256,"\tMinimum Packet Size : ");
        MUSB_Stringize(aDebugNumber, 33, pTestSequence->dwMinPacketSize, 10, TRUE);
        MUSB_StringConcat(aDebugMessage, 256,aDebugNumber);
        MUSB_PrintLine(aDebugMessage);

        aDebugMessage[0] = (CHAR)0;
        aDebugNumber[0]  = (CHAR)0;

        MUSB_StringConcat(aDebugMessage, 256,"\tIncrement Size : ");
        MUSB_Stringize(aDebugNumber, 33, pTestSequence->dwStepSize, 10, TRUE);
        MUSB_StringConcat(aDebugMessage, 256,aDebugNumber);
        MUSB_PrintLine(aDebugMessage);

    }

    pTestResult->dwSequenceIndex = -1;
    return;
}
