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
 * Implementations of the UDSI and UCDI's
 * port discovery functions.
 * $Revision: #1 $
 */

#include "mu_diag.h"
#include "mu_impl.h"


#ifdef MUSB_FDRC
#include "mu_fdrpr.h"
#ifdef MUSB_DMA

extern MUSB_DmaControllerFactory MUSB_FdrcDmaControllerFactory;

static MUSB_DmaControllerFactory *MGC_pFdrcDmaControllerFactory = &MUSB_FdrcDmaControllerFactory;

void MUSB_SetFsAhbDmaControllerFactory(MUSB_DmaControllerFactory *pFactory)
{
    MGC_pFdrcDmaControllerFactory = pFactory;
}
#endif
#endif

#if defined(MUSB_HDRC) || defined(MUSB_MHDRC) || defined(MUSB_HSFC)
#include "mu_hdrpr.h"
#ifdef MUSB_DMA

extern MUSB_DmaControllerFactory MUSB_HdrcDmaControllerFactory;

static MUSB_DmaControllerFactory *MGC_pHdrcDmaControllerFactory = &MUSB_HdrcDmaControllerFactory;
/*
void MUSB_SetHsDmaControllerFactory(MUSB_DmaControllerFactory *pFactory)
{
    MGC_pHdrcDmaControllerFactory = pFactory;
}
*/
#endif
#endif

#ifdef MUSB_MHDRC
#include "mu_mdrpr.h"
#endif

#ifdef MUSB_HSFC
#include "mu_hfcpr.h"
#endif

/***************************** TYPES ******************************/

/**
 * All interface and implementation controller/port structs
 * together (to reduce mallocs).
 * NOTE: code assumes this member order; 
 * if you change member order, change code.
 * @field ControllerImpl controller implementation struct
 * @field Controller controller interface struct
 * @field PortImpl port implementation struct
 * @field Port port interface struct
 */

typedef struct
{
    MGC_Controller ControllerImpl;
    MUSB_Controller Controller;
    MGC_Port PortImpl;
    MUSB_Port Port;
} MGC_ControllerWrapper;

/**************************** GLOBALS *****************************/

/** Requirements for one 10-millisecond timer */
//static uint32_t MGC_aOneTimer10Ms [] = { 10 };

/** List of all controllers */
static MUSB_LinkedList MGC_ControllerList;

//  change to static allocation.
static MGC_ControllerWrapper _arMGC_ControllerWrapper[MUSB_MAX_CONTROLLERS];

#ifdef MUSB_DIAG
/** Diagnostic level */
uint8_t MGC_bDiagLevel = MUSB_DIAG;
#endif

/** set usb speed control */
uint8_t MGC_bWantHighSpeed = TRUE;

/** disable usb suspend mode */
uint8_t MGC_bWantSuspend = TRUE;

/** Set RG8X[6:4]: RG_CALIB_SELE1<2:0> in PHY ANALOG CONTROL REGISTER2
     It is used in MUSB_BoardSoftReset().
     If MGC_bHSTERMC = 0xFF, MUSB_BoardSoftReset() will 
     use h/w default value to set HSTERMC.
     MGC_bHSTERMC active value = 0 ~ 7. 
     MGC_bHSTERMC = 0xFF: use default value of h/w.
*/
uint8_t MGC_bHSTERMC = 0xFF;

/*************************** FUNCTIONS ****************************/

/*
 * Implementation
 */
#ifdef MUSB_DIAG

void MUSB_SetDiagnosticLevel(uint8_t bLevel)
{
    MGC_bDiagLevel = bLevel;
}
#endif
/*
* Common init
*/

static void MGC_ControllerInit(MGC_ControllerWrapper *pWrapper, void *pControllerAddressIsr,
                               void *pControllerAddressIst)
{
    MUSB_ASSERT(pWrapper);

    MUSB_ASSERT(pControllerAddressIsr);
    MUSB_ASSERT(pControllerAddressIst);

    pWrapper->ControllerImpl.pControllerAddressIsr = pControllerAddressIsr;
    pWrapper->ControllerImpl.pControllerAddressIst = pControllerAddressIst;
    pWrapper->ControllerImpl.pPort = &(pWrapper->PortImpl);

#if defined(MUSB_HOST) || defined(MUSB_OTG)

    MUSB_ListInit(&(pWrapper->PortImpl.ConnectedDeviceList));

#ifdef MUSB_SCHEDULER   
    MUSB_ListInit(&(pWrapper->PortImpl.Schedule.ScheduleSlots));
#endif

#endif

    pWrapper->PortImpl.pInterfacePort = &(pWrapper->Port);
    pWrapper->Controller.wVersion = MUSB_CONTROLLER_VERSION;
    pWrapper->Controller.pPrivateData = &(pWrapper->ControllerImpl);
    pWrapper->Port.bUsbPort = MGC_bSystemCount; /* Save current port number */
    pWrapper->Port.pPrivateData = &(pWrapper->PortImpl);
    pWrapper->PortImpl.pController = &(pWrapper->ControllerImpl);
}

/*
 * Instantiate a controller
 */
MUSB_Controller *MUSB_NewController(uint16_t wControllerType,
                                    void *pControllerAddressIsr, void *pControllerAddressBsr)
{
    uint8_t bEnd;
    MGC_EndpointResource *pEnd;
    uint16_t wType = wControllerType;
    MGC_ControllerWrapper *pWrapper = NULL;

    MUSB_ASSERT(MGC_bSystemCount < (uint8_t)MUSB_MAX_CONTROLLERS);      
    if (MGC_bSystemCount >= (uint8_t)MUSB_MAX_CONTROLLERS)
    {
        return NULL;
    }
    pWrapper = &_arMGC_ControllerWrapper[MGC_bSystemCount];

    MUSB_MemSet(pWrapper, 0, sizeof(MGC_ControllerWrapper));

    VERIFY(MUSB_ListAppendItem(&MGC_ControllerList, pWrapper, 0));

    /* common init */
    MGC_ControllerInit(pWrapper, pControllerAddressIsr, pControllerAddressBsr);

    /* initialize data & dispatch table */
    switch (wType)
    {

#ifdef MUSB_FDRC

    case MUSB_CONTROLLER_FDRC:
        MUSB_DIAG1(2, "Initializing FDRC at ", (uint32_t)pControllerAddressIsr, 16, 8);
        /* endpoint config discovery, etc. */
        if (!MGC_FdrcInit(&(pWrapper->PortImpl)))
        {
            return NULL;
        }
        
        /* fill info for system */
        pWrapper->Controller.wQueueLength = MUSB_IRP_QUEUE_LENGTH;
        pWrapper->Controller.wQueueItemSize = sizeof(MGC_BsrItem);
        pWrapper->Controller.pfIsr = MGC_FdrcIsr;
        pWrapper->Controller.pIsrParam = &(pWrapper->Controller);
        pWrapper->Controller.pfBsr = MGC_DrcBsr;
        pWrapper->Controller.pBsrParam = &(pWrapper->Port);
#ifdef MUSB_DMA

        pWrapper->ControllerImpl.pDmaControllerFactory = MGC_pFdrcDmaControllerFactory;
#endif

        /* fill functions for generic code */

        pWrapper->ControllerImpl.pfProgramStartController = MGC_FdrcStart;
        pWrapper->ControllerImpl.pfProgramStopController = MGC_FdrcStop;
        pWrapper->ControllerImpl.pfDestroyController = MGC_FdrcDestroy;

        pWrapper->PortImpl.pfReadBusState = MGC_FdrcReadBusState;
        pWrapper->PortImpl.pfProgramBusState = MGC_FdrcProgramBusState;
        pWrapper->PortImpl.pfResetPort = MGC_DrcResetPort;
        pWrapper->PortImpl.pfBindEndpoint = MGC_FdrcBindEndpoint;
        pWrapper->PortImpl.pfProgramStartReceive = MGC_FdrcStartRx;
        pWrapper->PortImpl.pfProgramStartTransmit = MGC_FdrcStartTx;
        pWrapper->PortImpl.pfProgramFlushEndpoint = MGC_FdrcFlushEndpoint;
        pWrapper->PortImpl.pfProgramHaltEndpoint = MGC_FdrcHaltEndpoint;
#ifdef MUSB_FUNCTION

        pWrapper->PortImpl.pfDefaultEndResponse = MGC_FdrcDefaultEndResponse;
#endif

        pWrapper->PortImpl.pfServiceDefaultEnd = MGC_FdrcServiceDefaultEnd;
        pWrapper->PortImpl.pfServiceReceiveReady = MGC_FdrcServiceReceiveReady;
        pWrapper->PortImpl.pfServiceTransmitAvail = MGC_FdrcServiceTransmitAvail;
        pWrapper->PortImpl.pfLoadFifo = MGC_FdrcLoadFifo;
        pWrapper->PortImpl.pfUnloadFifo = MGC_FdrcUnloadFifo;
        pWrapper->PortImpl.pfAcceptDevice = MGC_DrcAcceptDevice;
        pWrapper->PortImpl.pfDmaChannelStatusChanged = MGC_FdrcDmaChannelStatusChanged;
        pWrapper->Port.Type = MUSB_PORT_TYPE_OTG;
        pWrapper->Port.Speed = MUSB_PORT_SPEED_FULL;
        break;
#endif /* FDRC */

#ifdef MUSB_HDRC

    case MUSB_CONTROLLER_HDRC:
        MUSB_DIAG1(2, "Initializing HDRC at ", (uint32_t)pControllerAddressIsr, 16, 8);
        /* endpoint config discovery, etc. */
        if (!MGC_HdrcInit(&(pWrapper->PortImpl)))
        {
            return NULL;
        }
#ifndef MUSB_FORCE_FULLSPEED

        pWrapper->PortImpl.bWantHighSpeed = MGC_bWantHighSpeed;
#endif
        
        //  MTK Notice: Max Liao, 2006/11/27.
        //  add to support hub as a usb repeater (without speed transaction translator).
#ifdef MUSB_HUB
        pWrapper->PortImpl.bIsMultipoint = TRUE;
#endif
        /* fill info for system */
        pWrapper->Controller.wQueueLength = MUSB_IRP_QUEUE_LENGTH;
        pWrapper->Controller.wQueueItemSize = sizeof(MGC_BsrItem);
        pWrapper->Controller.pfIsr = MGC_HdrcIsr;
        pWrapper->Controller.pIsrParam = &(pWrapper->Controller);
        pWrapper->Controller.pfBsr = MGC_DrcBsr;
        pWrapper->Controller.pBsrParam = &(pWrapper->Port);
#ifdef MUSB_DMA

        pWrapper->ControllerImpl.pDmaControllerFactory = MGC_pHdrcDmaControllerFactory;
#endif

        /* fill functions for generic code */

        pWrapper->ControllerImpl.pfProgramStartController = MGC_HdrcStart;
        pWrapper->ControllerImpl.pfProgramStopController = MGC_HdrcStop;
#ifndef MUSB_CODE_SHRINK 
        pWrapper->ControllerImpl.pfDestroyController = MGC_HdrcDestroy;
#endif
        pWrapper->PortImpl.pfReadBusState = MGC_HdrcReadBusState;
        pWrapper->PortImpl.pfProgramBusState = MGC_HdrcProgramBusState;
        pWrapper->PortImpl.pfResetPort = MGC_DrcResetPort;
        pWrapper->PortImpl.pfBindEndpoint = MGC_HdrcBindEndpoint;
        pWrapper->PortImpl.pfProgramStartReceive = MGC_HdrcStartRx;
        pWrapper->PortImpl.pfProgramStartTransmit = MGC_HdrcStartTx;
        pWrapper->PortImpl.pfProgramFlushEndpoint = MGC_HdrcFlushEndpoint;
#ifdef MUSB_FUNCTION
        pWrapper->PortImpl.pfProgramHaltEndpoint = MGC_HdrcHaltEndpoint;
        pWrapper->PortImpl.pfDefaultEndResponse = MGC_HdrcDefaultEndResponse;
#endif

        pWrapper->PortImpl.pfServiceDefaultEnd = MGC_HdrcServiceDefaultEnd;
        pWrapper->PortImpl.pfServiceReceiveReady = MGC_HdrcServiceReceiveReady;
        pWrapper->PortImpl.pfServiceTransmitAvail = MGC_HdrcServiceTransmitAvail;
        pWrapper->PortImpl.pfLoadFifo = MGC_HdrcLoadFifo;
        pWrapper->PortImpl.pfUnloadFifo = MGC_HdrcUnloadFifo;
        pWrapper->PortImpl.pfAcceptDevice = MGC_DrcAcceptDevice;
        pWrapper->PortImpl.pfDmaChannelStatusChanged = MGC_HdrcDmaChannelStatusChanged;
#ifdef MUSB_EHSET

        pWrapper->PortImpl.pfSetPortTestMode = MGC_HdrcSetPortTestMode;
#endif

        pWrapper->Port.Type = MUSB_PORT_TYPE_HOST;
        pWrapper->Port.Speed = MUSB_PORT_SPEED_HIGH;
        break;
#endif /* HDRC */

#ifdef MUSB_MHDRC

    case MUSB_CONTROLLER_MHDRC:
        MUSB_DIAG1(2, "Initializing MHDRC at ", (uint32_t)pControllerAddressIsr, 16, 8);
        /* endpoint config discovery, etc. */
        if (!MGC_MhdrcInit(&(pWrapper->PortImpl)))
        {
            return NULL;
        }
        
#ifndef MUSB_FORCE_FULLSPEED

        pWrapper->PortImpl.bWantHighSpeed = TRUE;
#endif

        pWrapper->PortImpl.bIsMultipoint = TRUE;
        /* fill info for system */
        pWrapper->Controller.wQueueLength = MUSB_IRP_QUEUE_LENGTH;
        pWrapper->Controller.wQueueItemSize = sizeof(MGC_BsrItem);
        pWrapper->Controller.pfIsr = MGC_HdrcIsr;
        pWrapper->Controller.pIsrParam = &(pWrapper->Controller);
        pWrapper->Controller.pfBsr = MGC_DrcBsr;
        pWrapper->Controller.pBsrParam = &(pWrapper->Port);
#ifdef MUSB_DMA

        pWrapper->ControllerImpl.pDmaControllerFactory = MGC_pHdrcDmaControllerFactory;
#endif

        /* fill functions for generic code */

        pWrapper->ControllerImpl.pfProgramStartController = MGC_HdrcStart;
        pWrapper->ControllerImpl.pfProgramStopController = MGC_HdrcStop;
        pWrapper->ControllerImpl.pfDestroyController = MGC_HdrcDestroy;

        pWrapper->PortImpl.pfReadBusState = MGC_HdrcReadBusState;
        pWrapper->PortImpl.pfProgramBusState = MGC_HdrcProgramBusState;
        pWrapper->PortImpl.pfResetPort = MGC_DrcResetPort;
        pWrapper->PortImpl.pfBindEndpoint = MGC_MhdrcBindEndpoint;
        pWrapper->PortImpl.pfProgramStartReceive = MGC_MhdrcStartRx;
        pWrapper->PortImpl.pfProgramStartTransmit = MGC_MhdrcStartTx;
        pWrapper->PortImpl.pfProgramFlushEndpoint = MGC_HdrcFlushEndpoint;
        pWrapper->PortImpl.pfProgramHaltEndpoint = MGC_HdrcHaltEndpoint;
#ifdef MUSB_FUNCTION

        pWrapper->PortImpl.pfDefaultEndResponse = MGC_HdrcDefaultEndResponse;
#endif

        pWrapper->PortImpl.pfServiceDefaultEnd = MGC_HdrcServiceDefaultEnd;
        pWrapper->PortImpl.pfServiceReceiveReady = MGC_HdrcServiceReceiveReady;
        pWrapper->PortImpl.pfServiceTransmitAvail = MGC_HdrcServiceTransmitAvail;
        pWrapper->PortImpl.pfLoadFifo = MGC_HdrcLoadFifo;
        pWrapper->PortImpl.pfUnloadFifo = MGC_HdrcUnloadFifo;
        pWrapper->PortImpl.pfAcceptDevice = MGC_DrcAcceptDevice;
        pWrapper->PortImpl.pfDmaChannelStatusChanged = MGC_HdrcDmaChannelStatusChanged;
#ifdef MUSB_EHSET

        pWrapper->PortImpl.pfSetPortTestMode = MGC_HdrcSetPortTestMode;
#endif

        pWrapper->Port.Type = MUSB_PORT_TYPE_OTG;
        pWrapper->Port.Speed = MUSB_PORT_SPEED_HIGH;
        break;
#endif /* MHDRC */

#ifdef MUSB_HSFC

    case MUSB_CONTROLLER_HSFC:
        MUSB_DIAG1(2, "Initializing HSFC at ", (uint32_t)pControllerAddressIsr, 16, 8);
        /* endpoint config discovery, etc. */
        VERIFY(MGC_HsfcInit(&(pWrapper->PortImpl)));
#ifndef MUSB_FORCE_FULLSPEED

        pWrapper->PortImpl.bWantHighSpeed = TRUE;
#endif
        /* fill info for system */

        pWrapper->Controller.wQueueLength = MUSB_IRP_QUEUE_LENGTH;
        pWrapper->Controller.wQueueItemSize = sizeof(MGC_BsrItem);
        pWrapper->Controller.pfIsr = MGC_HsfcIsr;
        pWrapper->Controller.pIsrParam = &(pWrapper->Controller);
        pWrapper->Controller.pfBsr = MGC_FcBsr;
        pWrapper->Controller.pBsrParam = &(pWrapper->Port);
#ifdef MUSB_DMA

        pWrapper->ControllerImpl.pDmaControllerFactory = MGC_pHdrcDmaControllerFactory;
#endif

        /* fill functions for generic code */

        pWrapper->ControllerImpl.pfProgramStartController = MGC_HsfcStart;
        pWrapper->ControllerImpl.pfProgramStopController = MGC_HsfcStop;
        pWrapper->ControllerImpl.pfDestroyController = MGC_HsfcDestroy;

        pWrapper->PortImpl.pfReadBusState = MGC_HdrcReadBusState;
        pWrapper->PortImpl.pfProgramBusState = MGC_HdrcProgramBusState;
        pWrapper->PortImpl.pfResetPort = MGC_FcResetPort;
        pWrapper->PortImpl.pfBindEndpoint = MGC_HsfcBindEndpoint;
        pWrapper->PortImpl.pfProgramStartReceive = MGC_HsfcStartRx;
        pWrapper->PortImpl.pfProgramStartTransmit = MGC_HsfcStartTx;
        pWrapper->PortImpl.pfProgramFlushEndpoint = MGC_HsfcFlushEndpoint;
        pWrapper->PortImpl.pfProgramHaltEndpoint = MGC_HsfcHaltEndpoint;
#ifdef MUSB_FUNCTION

        pWrapper->PortImpl.pfDefaultEndResponse = MGC_HsfcDefaultEndResponse;
#endif

        pWrapper->PortImpl.pfServiceDefaultEnd = MGC_HsfcServiceDefaultEnd;
        pWrapper->PortImpl.pfServiceReceiveReady = MGC_HsfcServiceReceiveReady;
        pWrapper->PortImpl.pfServiceTransmitAvail = MGC_HsfcServiceTransmitAvail;
        pWrapper->PortImpl.pfLoadFifo = MGC_HsfcLoadFifo;
        pWrapper->PortImpl.pfUnloadFifo = MGC_HsfcUnloadFifo;
        pWrapper->PortImpl.pfAcceptDevice = NULL;
        pWrapper->PortImpl.pfDmaChannelStatusChanged = MGC_HsfcDmaChannelStatusChanged;
#ifdef MUSB_EHSET

        pWrapper->PortImpl.pfSetPortTestMode = MGC_HsfcSetPortTestMode;
#endif
        pWrapper->Port.Type = MUSB_PORT_TYPE_FUNCTION;
        pWrapper->Port.Speed = MUSB_PORT_SPEED_HIGH;
        break;
#endif /* HDRC */

    default:
        MUSB_ASSERT(0);
        break;
    } /* switch on controller type */

#ifdef MUSB_DIAG

    for (bEnd = 1; bEnd < pWrapper->PortImpl.bEndCount; bEnd++)
    {
        pEnd = (MGC_EndpointResource *)MUSB_ArrayFetch(&(pWrapper->PortImpl.LocalEnds), bEnd);

        MUSB_DIAG2(2, "  Endpoint=", bEnd, " / Shared FIFO=", pEnd->bIsFifoShared, 10, 0);
        MUSB_DIAG2(2, "    Tx FIFO size=", pEnd->wMaxPacketSizeTx, " / Rx FIFO size=",
                   pEnd->wMaxPacketSizeRx, 16, 4);
    }
#endif
    /* initialize IRP lists */

    for (bEnd = 0; bEnd < pWrapper->PortImpl.bEndCount; bEnd++)
    {
        pEnd = (MGC_EndpointResource *)MUSB_ArrayFetch(&(pWrapper->PortImpl.LocalEnds), bEnd);

        MUSB_ListInit(&(pEnd->TxIrpList));
        MUSB_ListInit(&(pEnd->RxIrpList));
    }

    /* ready */
    return &(pWrapper->Controller);
}

/*
 * Start (or restart) a controller
 */
uint32_t MUSB_StartController(const MUSB_Controller *pController, MUSB_SystemServices *pSystemServices)
{
    MGC_Controller *pImpl = NULL;
    uint8_t bOk;
    
    MUSB_ASSERT(pController);
    pImpl = (MGC_Controller *)pController->pPrivateData;
    MUSB_ASSERT(pSystemServices);

    if (pImpl)
    {
        MUSB_ASSERT(pImpl->pPort);        

#ifdef MUSB_DMA
        /* try to instantiate DMA controller */

        if (pImpl->pDmaControllerFactory)
        {
            pImpl->pPort->pDmaController =
                pImpl->pDmaControllerFactory->pfNewDmaController(
                    pImpl->pPort->pfDmaChannelStatusChanged, pImpl->pPort,
                    pSystemServices->pfSystemToBusAddress,
                    pImpl->pControllerAddressIsr, pImpl->pControllerAddressIst);
        }
        /* if successful, start it */
        if (pImpl->pPort->pDmaController)
        {
            MUSB_ASSERT(pImpl->pPort->pDmaController->pfDmaStartController);        
            bOk = pImpl->pPort->pDmaController->pfDmaStartController(
                pImpl->pPort->pDmaController->pPrivateData);
            UNUSED(bOk);
        }
#endif

        pImpl->pSystemServices = pSystemServices;
    
        MUSB_ASSERT(pImpl->pfProgramStartController);
        return pImpl->pfProgramStartController(pImpl);
    }

    return (uint32_t)MUSB_STATUS_UNSUPPORTED;
}

/*
 * Stop a controller
 */
#ifndef MUSB_CODE_SHRINK
void MUSB_StopController(const MUSB_Controller *pController)
{
    MGC_Controller *pImpl = NULL;
    uint8_t bOk;
    
    MUSB_ASSERT(pController);
    pImpl = (MGC_Controller *)pController->pPrivateData;
    MUSB_ASSERT(pImpl);
    MUSB_ASSERT(pImpl->pPort);

#ifdef MUSB_DMA
    /* stop DMA controller if present */

    if (pImpl->pPort->pDmaController)
    {
        MUSB_ASSERT(pImpl->pPort->pDmaController->pfDmaStopController);
        bOk = pImpl->pPort->pDmaController->pfDmaStopController(
            pImpl->pPort->pDmaController->pPrivateData);
        UNUSED(bOk);
    }
#endif

    MUSB_ASSERT(pImpl->pfProgramStopController);
    VERIFY(0 == pImpl->pfProgramStopController(pImpl));
}

/*
 * Destroy a controller
 */
void MUSB_DestroyController(const MUSB_Controller *pController)
{
    uint32_t dwStatus = 0;

    MGC_Controller *pImpl = NULL;

    if (pController)
    {
        MUSB_StopController(pController);
        pImpl = (MGC_Controller *)pController->pPrivateData;
    }

    if (pImpl)
    {

#ifdef MUSB_DMA
        /* destroy DMA controller if present */

        if (pImpl->pPort->pDmaController)
        {
            pImpl->pDmaControllerFactory->pfDestroyDmaController(pImpl->pPort->pDmaController);
        }
#endif

        dwStatus = pImpl->pfDestroyController(pImpl);
        MUSB_ASSERT(0 == dwStatus);
        /* NOTE: assumes ControllerImpl is first in wrapper */
        MUSB_MemSet(pImpl, 0, sizeof(MGC_ControllerWrapper));
        MUSB_MemFree(pImpl);
        MUSB_ListRemoveItem(&MGC_ControllerList, pImpl);
    }

    UNUSED(dwStatus);
    return;
}
#endif
/* Discover the number of ports */
uint16_t MUSB_CountPorts(void)
{
    return MGC_bSystemCount;
}

/* Get a port */
MUSB_Port *MUSB_GetPort(uint16_t index)
{
    MUSB_Port *pResult = NULL;

    MGC_ControllerWrapper *pWrapper;

    if (index < MGC_bSystemCount)
    {
        /* typically called at init time only, so linear search OK */
        pWrapper = (MGC_ControllerWrapper *)MUSB_ListFindItem(&MGC_ControllerList, index);
        if (pWrapper)
        {
            pResult = &(pWrapper->Port);
        }
    }

    return pResult;
}

/*
 * Implementation
 */
#if 0 
uint8_t MUSB_DeactivateClient(MUSB_BusHandle hBus)
{
    MGC_Port *pPort = (MGC_Port *)hBus;

    MUSB_ASSERT(pPort);
    /* clear for next session */
    pPort->bSessionActive = FALSE;

    /* try */
    pPort->bWantSession = FALSE;
    pPort->pfProgramBusState(pPort);

    return TRUE;
}
#endif

/*
 * Implementation
 */
#if 0
uint32_t MUSB_GetBusFrame(MUSB_BusHandle hBus)
{
    MGC_Port *pPort;

    MUSB_ASSERT(hBus);
    
    pPort = (MGC_Port *)hBus;

    VERIFY(0 == pPort->pfReadBusState(pPort));
    return pPort->dwFrame;
}
#endif

/*
 * Implementation
 */
void MUSB_SuspendBus(MUSB_BusHandle hBus)
{
    uint16_t wCount;

    uint16_t wIndex;
    MUSB_Device *pDevice;
    MGC_Device *pImplDevice;
    MUSB_DeviceDriver *pDriver;
    MGC_Port *pPort = (MGC_Port *)hBus;

    if (pPort)
    {
        /* try */

#if defined(MUSB_OTG_FUNC) && defined(MUSB_OTG)

        pPort->bRelinquish = FALSE;
#endif

        pPort->bWantSuspend = TRUE;
        pPort->pfProgramBusState(pPort);

#if defined(MUSB_HOST) || defined(MUSB_OTG)
        /* inform drivers */

        wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));
        for (wIndex = 0; wIndex < wCount; wIndex++)
        {
            pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), wIndex);
            if (pDevice)
            {
                pImplDevice = pDevice->pPrivateData;
                if (pImplDevice)
                {
                    pDriver = pImplDevice->pDriver;
                    if (pDriver && pDriver->pfBusSuspended)
                    {
                        pDriver->pfBusSuspended(pDriver->pPrivateData, hBus);
                    }
                }
            }
        }

        pPort->bIsSuspend = TRUE;
        MGC_DrcChangeOtgState((uint8_t)MUSB_A_SUSPEND, pPort);        
#endif

    }
}

/*
 * Implementation
 */
void MUSB_ResumeBus(MUSB_BusHandle hBus)
{
    uint16_t wCount;

    uint16_t wIndex;
    MUSB_Device *pDevice;
    MGC_Device *pImplDevice;
    MUSB_DeviceDriver *pDriver;
    MGC_Port *pPort = (MGC_Port *)hBus;

    if (pPort)
    {

#ifdef MUSB_FUNCTION

//  MTK Notice: Max Liao, 2008/05/15.
//  Because we do not use Resume in usual operation.
//  We just skip the condition checking in our CLI test.
        //if (pPort->bIsHost || pPort->bCanWakeup)        
#else

            if (pPort->bIsHost)
#endif

            {
                /* try */
                pPort->bWantSuspend = FALSE;
                pPort->bWantResume = TRUE;

                pPort->pfProgramBusState(pPort);

#if defined(MUSB_HOST) || defined(MUSB_OTG)

                if (pPort->bIsHost)
                {
                    /* inform drivers */
                    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));
                    for (wIndex = 0; wIndex < wCount; wIndex++)
                    {
                        pDevice =
                            (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), wIndex);
                        if (pDevice)
                        {
                            pImplDevice = pDevice->pPrivateData;
                            if (pImplDevice)
                            {
                                pDriver = pImplDevice->pDriver;
                                if (pDriver && pDriver->pfBusResumed)
                                {
                                    pDriver->pfBusResumed(pDriver->pPrivateData, hBus);
                                }
                            }
                        }
                    }
                }
#endif

            }
    }
}

/*
 * If DMA controller present, get a channel (if not already) and
 * try to allocate a buffer.  If allocation failed and we grabbed
 * a channel to try it, release the channel.
 */

#ifdef MUSB_DMA_BUF

uint8_t *MUSB_AllocDmaBuffer(MUSB_Pipe hPipe, uint32_t dwLength)
{
#ifdef MUSB_DMA

    uint8_t bHadChannel = TRUE;

    uint8_t *pResult = NULL;
    MGC_Pipe *pPipe = (MGC_Pipe *)hPipe;
    MGC_EndpointResource *pEnd = pPipe->pLocalEnd;
    MGC_Port *pPort = pPipe->pPort;
    MUSB_DmaController *pDmaController = pPort->pDmaController;
    uint8_t bIsTx = (pPipe->bmFlags&MGC_PIPEFLAGS_TRANSMIT) ? TRUE : FALSE;

    if (pDmaController)
    {
        if (bIsTx)
        {
            if (!pEnd->pDmaChannel)
            {
                bHadChannel = FALSE;
                pEnd->pDmaChannel =
                    pDmaController->pfDmaAllocateChannel(pDmaController->pPrivateData,
                                                         pEnd->bLocalEnd,
                                                         bIsTx,
                                                         pPipe->bTrafficType, bIsTx
                                                                                  ? pEnd->wMaxPacketSizeTx
                                                                                  : pEnd->wMaxPacketSizeRx);
            }
            if (pEnd->pDmaChannel)
            {
                pResult = pDmaController->pfDmaAllocateBuffer(pEnd->pDmaChannel, dwLength);
                if (pResult)
                {
                    /* success: increment count */
                    pEnd->wDmaBufferCount++;
                }
                else if (!pResult && !bHadChannel)
                {
                    /* failure: release channel if we allocated it */
                    pDmaController->pfDmaReleaseChannel(pEnd->pDmaChannel);
                }
            }
        }
        else
        {
            if (!pEnd->pRxDmaChannel)
            {
                bHadChannel = FALSE;
                pEnd->pRxDmaChannel =
                    pDmaController->pfDmaAllocateChannel(pDmaController->pPrivateData,
                                                         pEnd->bLocalEnd,
                                                         bIsTx,
                                                         pPipe->bTrafficType, bIsTx
                                                                                  ? pEnd->wMaxPacketSizeTx
                                                                                  : pEnd->wMaxPacketSizeRx);
            }
            if (pEnd->pRxDmaChannel)
            {
                pResult = pDmaController->pfDmaAllocateBuffer(pEnd->pRxDmaChannel, dwLength);
                if (pResult)
                {
                    /* success: increment count */
                    pEnd->wRxDmaBufferCount++;
                }
                else if (!pResult && !bHadChannel)
                {
                    /* failure: release channel if we allocated it */
                    pDmaController->pfDmaReleaseChannel(pEnd->pRxDmaChannel);
                }
            }
        }
    }

    return pResult;
#else

    return NULL;
#endif

}

void MUSB_FreeDmaBuffer(MUSB_Pipe hPipe, uint8_t *pBuffer)
{
#ifdef MUSB_DMA

    MGC_Pipe *pPipe = (MGC_Pipe *)hPipe;

    MGC_EndpointResource *pEnd = pPipe->pLocalEnd;
    MGC_Port *pPort = pPipe->pPort;
    MUSB_DmaController *pDmaController = pPort->pDmaController;
    uint8_t bIsTx = (pPipe->bmFlags&MGC_PIPEFLAGS_TRANSMIT) ? TRUE : FALSE;

    if (pDmaController)
    {
        if (bIsTx && pEnd->pDmaChannel)
        {
            pDmaController->pfDmaReleaseBuffer(pEnd->pDmaChannel, pBuffer);
            if (pEnd->wDmaBufferCount)
            {
                --pEnd->wDmaBufferCount;
                /* if buffer count goes to zero, release channel for others */
                if (!pEnd->wDmaBufferCount)
                {
                    pDmaController->pfDmaReleaseChannel(pEnd->pDmaChannel);
                }
            }
        }
        if (!bIsTx && pEnd->pRxDmaChannel)
        {
            pDmaController->pfDmaReleaseBuffer(pEnd->pRxDmaChannel, pBuffer);
            if (pEnd->wDmaBufferCount)
            {
                --pEnd->wDmaBufferCount;
                /* if buffer count goes to zero, release channel for others */
                if (!pEnd->wDmaBufferCount)
                {
                    pDmaController->pfDmaReleaseChannel(pEnd->pRxDmaChannel);
                }
            }
        }
    }
#endif

}
#endif

MGC_EndpointResource *MGC_AllocateDynamicFifo(
    MGC_Port *pPort, const MUSB_DeviceEndpoint *pUsbEnd, uint8_t bBind, uint8_t *pbIsTx)
{
    uint8_t bEnd = 0;
    MGC_EndpointResource *pEnd = NULL;
    uint16_t wPacketSize;
    uint8_t bIsIn;
    uint8_t bIsTx;
    
    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pUsbEnd);
    MUSB_ASSERT(pbIsTx);
    
    wPacketSize = MUSB_SWAP16P((uint8_t *)&(pUsbEnd->UsbDescriptor.wMaxPacketSize));
    bIsIn = (pUsbEnd->UsbDescriptor.bEndpointAddress &MUSB_ENDPOINT_DIR_MASK) ? TRUE : FALSE;    
    bIsTx = pPort->bIsHost ? !bIsIn : bIsIn;

    *pbIsTx = bIsTx;
    
    if (pPort->bIsHost)
    {
        // Always bind endpoint in host mode.
        MUSB_ASSERT(bBind);
        for (bEnd = 1; bEnd < pPort->bEndCount; bEnd++)
        {
            pEnd = (MGC_EndpointResource *)MUSB_ArrayFetch(&(pPort->LocalEnds), bEnd);
            if (!pEnd)
            {
                return NULL;
            }

            // check empty ep with enough fifo size.
            if (bIsTx)
            {
                if ((!pEnd->bIsClaimed) && (wPacketSize <= pEnd->wMaxPacketSizeTx))
                {
                    pEnd->bIsTx = TRUE;
                    pEnd->bIsClaimed = TRUE;                    
                    pEnd->bBusAddress = pUsbEnd->pDevice->bBusAddress;
                    pEnd->bBusEnd = pUsbEnd->UsbDescriptor.bEndpointAddress;
                    pEnd->bInterval = pUsbEnd->UsbDescriptor.bInterval;
                    pEnd->bTrafficType = 
                        pUsbEnd->UsbDescriptor.bmAttributes & MUSB_ENDPOINT_XFERTYPE_MASK;
                    pEnd->wPacketSize = wPacketSize;
                    // If this is not a bulk transfer, also claim the other direction ep.
                    // This is our h/w use the same address for tx/rx ep.
                    return pEnd;
                }
            }
            else
            {
                if ((!pEnd->bRxClaimed) && (wPacketSize <= pEnd->wMaxPacketSizeRx))
                {
                    pEnd->bIsTx = FALSE;                
                    pEnd->bRxClaimed = TRUE;                    
                    pEnd->bRxBusAddress = pUsbEnd->pDevice->bBusAddress;
                    pEnd->bRxBusEnd = pUsbEnd->UsbDescriptor.bEndpointAddress;
                    pEnd->bRxInterval = pUsbEnd->UsbDescriptor.bInterval;
                    pEnd->bRxTrafficType = 
                        pUsbEnd->UsbDescriptor.bmAttributes & MUSB_ENDPOINT_XFERTYPE_MASK;
                    pEnd->wRxPacketSize = wPacketSize;
                    // If this is not a bulk transfer, also claim the other direction ep.
                    // This is our h/w use the same address for tx/rx ep.
                    return pEnd;
                }

            }            
        }      
    }
#ifdef MUSB_FUNCTION    
    else
    {
        /* use endpoint from descriptor */
        bEnd = pUsbEnd->UsbDescriptor.bEndpointAddress & MUSB_ENDPOINT_NUMBER_MASK;
        pEnd = (MGC_EndpointResource *)MUSB_ArrayFetch(&(pPort->LocalEnds), bEnd);
        if (!pEnd)
        {
            return NULL;
        }

        if (bBind)
        {
            if (bIsTx)
            {
                pEnd->bIsTx = TRUE;            
                pEnd->bIsClaimed = TRUE;                    
                pEnd->bBusAddress = pUsbEnd->pDevice->bBusAddress;
                pEnd->bBusEnd = pUsbEnd->UsbDescriptor.bEndpointAddress;
                pEnd->bInterval = pUsbEnd->UsbDescriptor.bInterval;
                pEnd->bTrafficType = 
                    pUsbEnd->UsbDescriptor.bmAttributes & MUSB_ENDPOINT_XFERTYPE_MASK;
                pEnd->wPacketSize = wPacketSize;
            }
            else
            {
                pEnd->bIsTx = FALSE;
                pEnd->bRxClaimed = TRUE;                    
                pEnd->bRxBusAddress = pUsbEnd->pDevice->bBusAddress;
                pEnd->bRxBusEnd = pUsbEnd->UsbDescriptor.bEndpointAddress;
                pEnd->bRxInterval = pUsbEnd->UsbDescriptor.bInterval;
                pEnd->bRxTrafficType = 
                    pUsbEnd->UsbDescriptor.bmAttributes & MUSB_ENDPOINT_XFERTYPE_MASK;
                pEnd->wRxPacketSize = wPacketSize;                
            }            
        }
        return pEnd;
    }
#endif
    return NULL;
}
