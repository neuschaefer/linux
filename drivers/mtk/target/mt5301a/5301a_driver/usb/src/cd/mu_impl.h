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
 * Controller/port implementation definitions
 * $Revision: #1 $
 */

#ifndef __MUSB_IMPL_H__
#define __MUSB_IMPL_H__

#include "mu_tools.h"
#include "mu_dsi.h"
#include "plat_cnf.h"
#include "mu_dci.h"
#include "mu_cdi.h"
#include "mu_arch.h"
#include "mu_list.h"
#include "mu_hset.h"

//  MTK Notice: Max Liao, 2006/10/02.
//  add to support dynamic fifo.
#define MUSB_C_DYNFIFO_DEF
#define MUSB_C_NUM_EPS              (MGC_MAX_USB_ENDS)    //<number-of-endpoints>
//#define MUSB_C_RAM_BITS             (12)  //<ram-word-bits>

#ifndef MUSB_IRP_QUEUE_LENGTH
//  MTK Notice: Max Liao, 2007/04/03.
// set irp queue length = 0 (use default = 16) to save memory.
#define MUSB_IRP_QUEUE_LENGTH   (16) /* double-buffered, all endpoints both directions */
#endif

#define MUSB_BUS_RESET_TIME (60)   /* 60 ms */


#define MGC_MIN(_n1_, _n2_) (((_n2_) < (_n1_)) ? (_n2_) : (_n1_))
#define MGC_MAX(_n1_, _n2_) (((_n2_) > (_n1_)) ? (_n2_) : (_n1_))

/*
 * Diagnostics
 */
#ifdef MUSB_DIAG

extern uint8_t MGC_bDiagLevel;

#ifndef MGC_DIAG

/**
 * Print diagnostic
 * @param _bLevel diagnostic level
 * @param _pC controller pointer
 * @param _pMsg message to print
 */
#define MGC_DIAG(_bLevel, _pC, _pMsg) \
    if(MGC_bDiagLevel >= _bLevel) { \
        LOG(7, "%s.\n", _pMsg); }

/**
 * Use this to eliminate diagnostic code when not compiled with diagnostics.
 * Print a diagnostic with a string and number.
 * @param _bLevel diagnostic level
 * @param _pC controller pointer
 * @param _pText the text portion of the message; printed first
 * @param _dwNum the numeric portion of the message; appended to the text portion
 * @param _bBase numeric base (2, 8, 10 or 16)
 * @param _bJust the justification (0 for none); this is for leading-zero padding
 */
#define MGC_DIAG1(_bLevel, _pC, _pText, _dwNum, _bBase, _bJust) \
    if(MGC_bDiagLevel >= _bLevel) { \
        LOG(7, "%s 0x%X.\n", _pText, _dwNum); }
        
/**
 * Use this to eliminate diagnostic code when not compiled with diagnostics.
 * Print a diagnostic with a string followed by a number, followed by another set.
 * @param _bLevel diagnostic level
 * @param _pC controller pointer
 * @param _pText1 the first text portion of the message; printed first
 * @param _dwNum1 the first numeric portion of the message; appended to the above
 * @param _pText2 the second text portion of the message; appended to the above
 * @param _dwNum2 the second numeric portion of the message; appended to the above
 * @param _bBase numeric base for both numbers (2, 8, 10 or 16)
 * @param _bJust the justification (0 for none); this is for leading-zero padding
 */
#define MGC_DIAG2(_bLevel, _pC, _pText1, _dwNum1, _pText2, _dwNum2, _bBase, _bJust) \
    if(MGC_bDiagLevel >= _bLevel) { \
        LOG(7, "%s 0x%X, %s 0x%X.\n", _pText1, _dwNum1, _pText2, _dwNum2); }

#endif

#else

#ifndef MGC_DIAG

/* Print diagnostic */
#define MGC_DIAG(_bLevel, _pC, _pMsg)   USBLOG_##_bLevel(_pMsg);

#define USBLOG_1(_pMsg)    LOG(5, "%s\r\n", _pMsg)
#define USBLOG_2(_pMsg)
#define USBLOG_3(_pMsg)

#define MGC_DIAG1(_bLevel, _pC, _pText1, _dwNum1, _bBase, _bJust)
#define MGC_DIAG2(_bLevel, _pC, _pText1, _dwNum1, _pText2, _dwNum2, _bBase, _bJust)

#endif

#endif

/**
 * BSR queue item.  An item for the queue from ISR to BSR.
 * NOTE: members are not declared volatile; code will cast as necessary
 * 
 * @field dwTime timestamp when interrupt received
 * @field bCause cause (see MGC_BSR_CAUSE_*)
 * @field bLocalEnd local endpoint index (counting from 0)
 * @field bStatus status code
 * @field pData (IRP)
 */
typedef struct
{
    void* pData;
    uint8_t bCause;
    uint8_t bLocalEnd;
    uint8_t bStatus;
} MGC_BsrItem;
/** None (maybe cancelled) */
#define MGC_BSR_CAUSE_NONE		0x00
/** Session request */
#define MGC_BSR_CAUSE_SESSION_REQ	0x01
/** Connect */
#define MGC_BSR_CAUSE_CONNECT		0x02
/** Disconnect */
#define MGC_BSR_CAUSE_DISCONNECT	0x03
/** bus reset (peripheral only) */
#define MGC_BSR_CAUSE_RESET		0x05
/** suspend detected */
#define MGC_BSR_CAUSE_SUSPEND		0x06
/** resume detected */
#define MGC_BSR_CAUSE_RESUME		0x07
/** Vbus error */
#define MGC_BSR_CAUSE_VBUS_ERROR	0x08

/** Tx IRP complete; use pData */
#define MGC_BSR_CAUSE_IRP_COMPLETE	0x23
/** Rx IRP complete; use pData */
#define MGC_BSR_CAUSE_RX_IRP_COMPLETE	0x24

/** Maximum bytes possible to receive from peripheral on SETUP IN */
#define MGC_MAX_SETUP_IN	    255

/*
 *  OTG mode definitions. ABxx used when mode, not A or B, needed
 */
#define MGC_AB_MODE(x)  ((x) & 0xf) /* mask off A-B indicator */

#define MGC_AB_PERIPHERAL           0x1
#define MGC_AB_WAIT_CON             0x2
#define MGC_AB_HOST                 0x3

/*  end OTG  mode defs */

/*
 *  USB states, modified for meaning in an OTG environment
 */
#define MGC_POWERED     MUSB_POWERED   /* CONNECT int in, Reset pending */
#define MGC_DEFAULT     MUSB_DEFAULT   /* CONNECT in, Reset completed */
#define MGC_ADDRESS     MUSB_ADDRESS   /* Above,+ peripheral SET_ADDRESS completed */
#define MGC_CONFIGURED  4   /* Above,+ enumeration completed. OK for traffic*/

/*
 *  Note on above: Suspend depends on A or B device mode.  B doesn't 
 *  allow for a suspend mode, even while being a host.  a_suspend 
 *  defined explicitly by OTG spec and so is reflected in OTGState.
 */

/*
 *  OTG State timers, per OTG spec Chapter 5
 *  Unless noted otherwise, constants expressed in milliseconds
 */
#define MGC_TA_SRP_RSPNS        5000    /* Max */
#define MGC_TA_WAIT_VRISE       100     /* Max */
#define MGC_TA_BCON_LDB         100     /* Min */
#define MGC_TA_AIDL_BDIS        250     /* Min */
#define MGC_TA_BDIS_ACON        3000    /* Max */
#define MGC_TA_BIDL_ADIS_MIN    3       /* Min */
#define MGC_TA_BIDL_ADIS_MAX    200     /* Max */
#define MGC_TLDIS_DSCHG         0.025   /* Min */
#define MGC_TA_BCON_SDB         0.0025  /* Min */
#define MGC_TA_BCON_SDB_WIN     100     /* Max */

#define MGC_TB_ASE0_BRST        3.125   /* Min */
/*  End, OTG State Timer defs */

/*
 *  USB timers
 */
#define MGC_TM_PERIPHERAL_RESUME    10  /* MS for signalling RESUME */
#define MGC_TM_HOST_RESUME          20  /* ditto for host */
/*  End, USB Timers */

/*
 *  DRC timers
 */
#define MGC_B_DEV_VBUSSESS_BOUNCE   500 /* 0.5 sec allowed for VBus to drop */
#define MGC_DRC_DISCONNECT          250 /* 250 msec for DML */

/*  End, DRC timers */

struct _MGC_Controller;
struct _MGC_Port;

#ifdef MUSB_SCHEDULER    
struct _MGC_ScheduleSlot;
#endif

/**
 * MGC_EndpointResource. 
 * A local endpoint.
 * MUSB_EndpointResource's pPrivateData points to this
 * @field bLocalEnd core's endpoint index
 * @field bIsFifoShared TRUE if FIFO is shared between Tx/Rx
 * @field wMaxPacketSizeTx Tx FIFO size
 * @field wMaxPacketSizeRx Rx FIFO size
 * @field bBusAddress target (host role only)
 * @field bBusEnd target endpoint (host role) or local endpoint (function role)
 * @field bInterval USB 2.0
 * @field bTrafficType USB 2.0
 * @field bIsTx TRUE if current direction is transmit (used in shared FIFO endpoints)
 * @field bIsHalted TRUE if halted
 * @field wPacketSize actual packet size
 * @field bIsClaimed TRUE if claimed
 * @field pIrp pending IRP if any
 * @field dwWaitFrameCount how many frames to wait before first isoch IRP
 * @field IrpList linked list of pending IRPs
 * @field dwOffset current offset in user buffer
 * @field dwIsrTime total ISR time for current transfer
 * @field pPrivateData controller-specific code can use this
 * @field bMoveFifo set during dynamic FIFO compaction
 */
typedef struct
{
    /* NOTE: BOOLs can be changed to bitfields for smaller data, larger code */

    /* hardware state */
    uint8_t bLocalEnd;
    uint8_t bIsFifoShared;
    uint16_t wMaxPacketSizeTx;
    uint16_t wMaxPacketSizeRx;

    /* software state */
    uint8_t bBusAddress;
    uint8_t bBusEnd;
    uint8_t bInterval;
    uint8_t bTrafficType;
    uint8_t bRxBusAddress;
    uint8_t bRxBusEnd;
    uint8_t bRxInterval;
    uint8_t bRxTrafficType;
    uint8_t bIsTx;
    uint8_t bIsHalted;
    uint8_t bIsClaimed;
    uint8_t bRxClaimed;
    uint8_t bStopTx;
    uint8_t bIsRxHalted;
//  MTK Notice: Max Liao, 2006/12/21.
//  send 0-bytes packet should mark bIsTxNullPkt = TRUE.    
    uint8_t bIsTxNullPkt;
    uint16_t wPacketSize;
    uint16_t wRxPacketSize;
    uint32_t dwTxSize;

    void* pTxIrp;
    MUSB_LinkedList TxIrpList;
    uint32_t dwTxOffset;
    void* pRxIrp;
    MUSB_LinkedList RxIrpList;
    uint32_t dwRxOffset;

#ifdef MUSB_DMA
    MUSB_DmaChannel* pDmaChannel;
    MUSB_DmaChannel* pRxDmaChannel;
    uint16_t wDmaBufferCount;
    uint16_t wRxDmaBufferCount;
    uint8_t bDmaTx;
    //uint8_t bDmaResidual;
#endif

    void* pPrivateData;
    uint32_t dwWaitFrameCount;

#ifdef MUSB_FUNCTION     
//  MTK Notice: Max Liao, 2006/12/18.
//  add fifo remain variable.
    uint32_t dwFifoRemain;
#endif
} MGC_EndpointResource;

/**
 * MGC_Pipe.
 * Pipe implementation.
 * MUSB_Pipe points to this.
 * NOTE: members are not declared volatile; code will cast as necessary
 * @field hSession session handle
 * @field pLocalEnd local endpoint resource pointer
 * @field pPort port (implementation) pointer
 * @field pSlot pointer to schedule slot, if applicable
 * @field pDevice associated device; only valid if host
 * @field bTrafficType from USB 2.0
 * @field wMaxPacketSize from USB 2.0
 * @field wInterval interval, in (micro)frames
 * @field dwMaxBusTime computed maximum bus time, in nanoseconds
 * @field bmFlags bitmask of flags; see MGC_PIPEFLAGS_*
 */
typedef struct _MGC_Pipe
{
    void* hSession;
    MGC_EndpointResource* pLocalEnd;
    struct _MGC_Port* pPort;
    const MUSB_Device* pDevice;
    uint16_t wMaxPacketSize;
    uint16_t wInterval;
    uint32_t bmFlags;
    uint8_t bTrafficType;

#ifdef MUSB_SCHEDULER    
    struct _MGC_ScheduleSlot* pSlot;
    uint32_t dwMaxBusTime;
#endif
} MGC_Pipe;

/** opened as host */
#define MGC_PIPEFLAGS_HOST	(1 << 0)
/** closing (app request or device disconnected) */
#define MGC_PIPEFLAGS_CLOSING	(1 << 1)
/** suspended */
#define MGC_PIPEFLAGS_SUSPENDED	(1 << 2)
/** transmit */
#define MGC_PIPEFLAGS_TRANSMIT	(1 << 7)

/**
 * MGC_ScheduleSlot.
 * A slot in the periodic transfer schedule.
 * 
 * @field bIsActive TRUE when active
 * @field wInterval interval
 * @field wFrameOffset offset from schedule reset (imaginary reference frame)
 * @field wDuration how many consecutive (micro)frames
 * @field wFramesRemaining consecutive (micro)frames remaining
 * @field PipeList pipes with actual transfers
 */
#if defined(MUSB_ISO) || defined(MUSB_SCHEDULER)
typedef struct _MGC_ScheduleSlot
{
    MUSB_LinkedList PipeList;
    uint16_t wInterval;
    uint16_t wFrameOffset;
    uint16_t wDuration;
    uint16_t wFramesRemaining;
    uint8_t bIsActive;
} MGC_ScheduleSlot;

/**
 * MGC_Schedule.
 * A periodic transfer schedule.
 * @field wSlotCount schedule is normalized to this
 * @field ScheduleSlots sparse list of scheduled transfers (host only)
 */
typedef struct
{
    uint32_t dwTotalTime;
    MUSB_LinkedList ScheduleSlots;
    uint16_t wSlotCount;
} MGC_Schedule;
#endif

/**
 * MGC_Device.
 * Implementation device.
 * MUSB_Device's pPrivateData points to this.
 * @field pDriver device driver
 * @field pSchedule schedule (hubs only)
 * @field PipeList open pipes
 * @field Device device contained here to reduce mallocs
 */
typedef struct
{
    MUSB_DeviceDriver* pDriver;
    MUSB_Device Device;
#ifdef MUSB_SCHEDULER   
    MGC_Schedule* pSchedule;
    MUSB_LinkedList PipeList;
#endif   
#ifdef MUSB_HUB
    uint16_t wbmTxToggle;
    uint16_t wbmRxToggle;
    uint8_t bIsMultiTt;
#endif
} MGC_Device;

/*
 * Functions that may be controller-specific
 * This is where registers get twiddled; no place above.
 */

/**
 * Start controller.
 * Program the underlying controller to start 
 * (enable interrupts, bus reset, etc.).
 * @param 1 controller pointer
 * @return status code
 */
typedef uint32_t (*MGC_pfProgramStartController)(struct _MGC_Controller*);

/**
 * Stop controller.
 * Program the underlying controller to stop 
 * (disable interrupts, bus reset, etc.).
 * @param 1 controller pointer
 * @return status code
 */
typedef uint32_t (*MGC_pfProgramStopController)(struct _MGC_Controller*);

/**
 * Destroy controller.
 * Perform any controller-specific cleanup.
 * @param 1 controller pointer
 * @return status code
 */
typedef uint32_t (*MGC_pfDestroyController)(struct _MGC_Controller*);

/**
 * Dump controller state.
 * Dump info global to the controller
 * @param 1 controller pointer
 * @return how many CHARacters were placed in buffer
 */
typedef int32_t (*MGC_pfDumpControllerState)(struct _MGC_Controller*);

/**
 * Dump pipe state.
 * Dump info relevant to the given pipe
 * @param 1 controller pointer
 * @param 2 pipe pointer
 * @return how many CHARacters were placed in buffer
 */
typedef int32_t (*MGC_pfDumpPipeState)(struct _MGC_Controller*, MGC_Pipe*);

/**
 * Dump endpoint state.
 * Dump info relevant to the given local endpoint resource
 * @param 1 controller pointer
 * @param 2 endpoint resource pointer
 * @return how many CHARacters were placed in buffer
 */
typedef int32_t (*MGC_pfDumpLocalEndState)(struct _MGC_Controller*, 
				       MUSB_EndpointResource*);

/**
 * MGC_Controller.
 * Controller implementation.
 * MUSB_Controller's pPrivateData points to this
 * 
 * @field wControllerType type (see MUSB_CONTROLLER_*)
 * @field pControllerAddressIsr Address at interrupt time
 * @field pControllerAddressIst Address at non-interrupt-time
 * @field pSystemServices system services callbacks pointer
 * @field pPrivateData "subclasses" could use this
 * @field pPort all current cores are single-port
 * @field pDmaControllerFactory DMA controller factory
 * 
 * @field pfProgramStartController start the controller
 * @field pfProgramStopController stop the controller
 * @field pfDumpControllerState dump controller state
 * @field pfDumpPipeState dump pipe state
 * @field pfDumpLocalEndState dump endpoint state
 */
typedef struct _MGC_Controller
{
    void* pControllerAddressIsr;
    void* pControllerAddressIst;
    MUSB_SystemServices* pSystemServices;

    struct _MGC_Port* pPort;

#ifdef MUSB_DMA
    MUSB_DmaControllerFactory* pDmaControllerFactory;
#endif
    MGC_pfProgramStartController pfProgramStartController;
    MGC_pfProgramStopController pfProgramStopController;
#ifndef MUSB_CODE_SHRINK 
    MGC_pfDestroyController pfDestroyController;
#endif
} MGC_Controller;

/*
 * Functions that may be controller- and port-specific
 * This is where registers get twiddled; no place above.
 */

/**
 * Read bus state.
 * Fill the bus state info from the port (see MGC_Port).
 * This is where registers get twiddled; no place above.
 * @param 1 port pointer
 * @return status code
 */
typedef uint32_t (*MGC_pfReadBusState)(struct _MGC_Port*);

/**
 * Set bus state.
 * Program the bus state from the port (see MGC_Port).
 * This is where registers get twiddled; no place above.
 * @param 1 port pointer
 * @return status code
 */
typedef void (*MGC_pfProgramBusState)(struct _MGC_Port*);

/**
 * Reset port.
 * Reset the port (e.g. unbind/flush all endpoints, etc.).
 * This is where registers get twiddled; no place above.
 * @param 1 port pointer
 * @return status code
 */
typedef void (*MGC_pfResetPort)(struct _MGC_Port*);

/**
 * Find local endpoint resource.
 * Find (maybe bind) the underlying port's first (host) or required (function) 
 * local endpoint resource to communicate with the given remote endpoint (host)
 * or to be identified with the given endpoint descriptor (function).
 * @param 1 port pointer
 * @param 2 remote endpoint (host) or advertised endpoint (function)
 * @param 3 desired attributes of local endpoint resource
 * @param 4 TRUE to bind; FALSE to just check
 * @return pointer to resource on success
 * @return NULL on failure
 */
typedef MGC_EndpointResource* (*MGC_pfBindEndpoint)(struct _MGC_Port*, 
						    const MUSB_DeviceEndpoint*,
						    MUSB_EndpointResource*, uint8_t);

/**
 * Start a receive.
 * Program the underlying port to initiate a receive.
 * The implementation is responsible for filling the buffer
 * with any data already received before this call,
 * and also must set the endpoint's current IRP pointer
 * whenever additional packets are required to fulfill the IRP.
 * This is where registers get twiddled; no place above.
 * @param 1 port pointer
 * @param 2 local endpoint resource, as returned from bind
 * @param 3 buffer pointer
 * @param 4 TOTAL byte count expected
 * @param 5 IRP pointer
 * @param 6 TRUE to allow DMA; FALSE otherwise
 * @return status code
 */
typedef uint32_t (*MGC_pfProgramStartReceive)(struct _MGC_Port*, 
    MGC_EndpointResource*, uint8_t*, uint32_t, void*, uint8_t);

/**
 * Start a transmit.
 * Program the underlying port to initiate a transmit.
 * This is where registers get twiddled; no place above.
 * @param 1 port pointer
 * @param 2 local endpoint resource, as returned from bind
 * @param 3 buffer pointer
 * @param 4 TOTAL byte count to send
 * @param 5 IRP pointer
 * @return status code
 */
typedef uint32_t (*MGC_pfProgramStartTransmit)(struct _MGC_Port*, 
    MGC_EndpointResource*, const uint8_t*, uint32_t, void*);

/**
 * Set interval in bulk transfer.
 * Program the underlying port to initiate a transmit.
 * This is where registers get twiddled; no place above.
 * @param 1 port pointer
 * @param 2 local endpoint resource, as returned from bind
 * @param 3 interval
 * @param 4 bIsTx
 * @return void
 */
typedef void (*MGC_pfProgramSetInterval)(struct _MGC_Port*, 
    const MGC_EndpointResource*, const uint8_t, const uint8_t);

/**
 * Flush endpoint.
 * Program the underlying port to flush the given endpoint resource,
 * with respect to the given USB direction (IN or OUT).
 * The resource should be ready for normal operation by return time.
 * This is where registers get twiddled; no place above.
 * @param 1 port pointer
 * @param 2 local endpoint resource, as returned from bind
 * @param 3 direction, a la USB device framework
 * @param 4 TRUE to prepare to re-use the endpoint; FALSE otherwise
 * @return status code
 */
typedef uint32_t (*MGC_pfProgramFlushEndpoint)(struct _MGC_Port*, MGC_EndpointResource*, 
					       uint8_t, uint8_t);

/**
 * Program stall state (device mode) or stop/restart traffic (host mode).
 * In device mode, program the stall state for the underlying port's given endpoint.
 * In host mode, stop or restart traffic on the given local endpoint
 * (since immediate stopping may not be possible, 
 * callers must wait for the endpoint's bIsHalted flag to become true).
 * This is where registers get twiddled; no place above.
 * @param 1 port pointer
 * @param 2 local endpoint resource, as returned from bind
 * @param 3 direction, a la USB device framework
 * @param 4 TRUE to set halt or stop traffic; FALSE to clear halt or restart traffic
 * @return status code
 */
typedef uint32_t (*MGC_pfProgramHaltEndpoint)(struct _MGC_Port*, 
					      MGC_EndpointResource*, 
					      uint8_t, uint8_t);

/**
 * Respond on the default endpoint.
 * pPort->pSetupData should point to IN data, or NULL if none.
 * pPort->wSetupDataSize should be length of IN data.
 * @param 1 port pointer
 * @param 2 TRUE to stall; FALSE to respond
 * @return status code
 */
typedef uint32_t (*MGC_pfDefaultEndResponse)(struct _MGC_Port*, uint8_t);

/**
 * Service EP0 interrupt.
 * Service default endpoint interrupt
 * @param 1 port pointer
 * @param 2 work queue item pointer
 * @return TRUE to queue work
 */
typedef uint8_t (*MGC_pfServiceDefaultEnd)(struct _MGC_Port*, MGC_BsrItem*);

/**
 * Service non-default Tx.
 * Service an endpoint Tx interrupt
 * @param 1 port pointer
 * @param 2 endpoint local resource index
 * @param 3 work queue item pointer
 * @return TRUE to queue work
 */
typedef uint8_t (*MGC_pfServiceTransmitAvail)(struct _MGC_Port*, uint16_t, MGC_BsrItem*);

/**
 * Service non-default Rx.
 * Service an endpoint Rx interrupt
 * @param 1 port pointer
 * @param 2 endpoint local resource index
 * @param 3 work queue item pointer
 * @return TRUE to queue work
 */
typedef uint8_t (*MGC_pfServiceReceiveReady)(struct _MGC_Port*, uint16_t, MGC_BsrItem*);

/**
 * Load an endpoint FIFO
 * @param 1 port pointer
 * @param 2 endpoint local resource index
 * @param 3 byte count to load
 * @param 4 where to get data
 * @return TRUE on success
 */
typedef void (*MGC_pfLoadFifo)(struct _MGC_Port*, uint8_t, uint32_t, const uint8_t*);

/**
 * Unload an endpoint FIFO
 * @param 1 port pointer
 * @param 2 endpoint local resource index
 * @param 3 byte count to unload
 * @param 4 where to store data
 * @return void
 */
typedef void (*MGC_pfUnloadFifo)(struct _MGC_Port*, uint8_t, uint32_t, uint8_t*);

/**
 * Accept/reject device.
 * Check if device is acceptable; take appropriate action if not.
 * The enumerator calls this after gathering all configuration data
 * and attempting a driver match.
 * @param 1 port pointer
 * @param 2 device pointer
 * @param 3 pointer to driver that matched, or NULL if none
 * @return TRUE if device is acceptable
 * @return FALSE if not (and appropriate action taken)
 */
typedef uint8_t (*MGC_pfAcceptDevice)(struct _MGC_Port*, MUSB_Device*, const MUSB_DeviceDriver*);

/**
 * Set a port to a test mode.
 * @param 1 port pointer
 * @param 2 mode
 */
#ifdef MUSB_EHSET	    
typedef void (*MGC_pfSetPortTestMode)(struct _MGC_Port*, MUSB_HsetPortMode);
#endif

/**
 * Map a dynamic FIFO.
 * @param 1 port pointer
 */
typedef void (*MGC_pfDynamicFifoLocation)(struct _MGC_Port* pPort, 
					  uint8_t bEnd, 
					  uint8_t bIsTx,
					  uint8_t bSet,
					  uint32_t* pdwBase, 
					  uint8_t* pbSize83, 
					  uint8_t* pbDoubleBuffered);

/**
 * MGC_EndpointStats.
 * Endpoint statistics
 * @field dwInterruptCount how many interrupts seen
 * @field dwRxByteCount how many bytes received
 * @field dwTxByteCount how many bytes sent
 */
typedef struct
{
    uint32_t dwInterruptCount;
    uint32_t dwRxByteCount;
    uint32_t dwTxByteCount;
} MGC_EndpointStats;

/**
 * MGC_EnumerationData.
 * Data for the CDI's enumerator.
 * @field bAddress the address being assigned
 * @field bState current state of enumeration attempt
 * @field bIndex data pertinent to enumeration state
 * @field bCount data pertinent to enumeration state
 * @field wData data pertinent to enumeration state
 * @field dwData data pertinent to enumeration state
 * @field pfEnumerationComplete callback for enumeration complete
 * @field pDevice device currently being enumerated
 * @field aSetupTx current/last request
 * @field aSetupRxEP0 Rx data
 * @field Irp IRP for enumeration
 * @field abmAddress bitmask of used addresses
 * @field bRetries current retry count
 */
typedef struct
{
    MUSB_pfChildDeviceUpdate pfChildDeviceUpdate;
    MUSB_pfHubEnumerationComplete pfEnumerationComplete;
    MUSB_Device* pDevice;
    uint8_t aSetupTx[8];
    uint8_t aSetupRx[MGC_MAX_SETUP_IN];
    uint8_t abmAddress[16];
    MUSB_ControlIrp Irp;
    uint32_t dwData;
    uint16_t wData;
    uint8_t bAddress;
    uint8_t bState;
    uint8_t bIndex;
    uint8_t bCount;
    uint8_t bRetries;
} MGC_EnumerationData;

/**
 * MGC_Port.
 * Implementation port.
 * MUSB_Port's pPrivateData points to this.
 * @field pController parent controller
 * @field bSessionActive TRUE if session is active
 * @field bUsbState current USB state
 * @field bOtgState current OTG state
 * @field bRelinquish TRUE if we suspended to relinquish host role
 * @field bWantHighSpeed TRUE to enable high-speed
 * @field bWantReset (host/OTG) TRUE to assert bus reset
 * @field bWantSuspend TRUE to suspend (host/OTG) or enable suspend (function)
 * @field bWantResume TRUE to assert resume
 * @field bFuncAddr function address (+ some DRCs as host)
 * @field bWantSession (OTG only) TRUE to request session
 * @field bWantHost (OTG only) TRUE to request host
 * @field bWantTestMode TRUE to enter a test mode
 * @field bTestMode which test mode
 * @field dwFrame current frame number
 * @field bIsHighSpeed TRUE if high-speed detected
 * @field bIsLowSpeed TRUE if low-speed detected
 * @field bIsReset TRUE if bus is in reset
 * @field bIsSuspend TRUE if bus is suspended
 * @field bIsResume TRUE if resume signalling detected
 * @field bIsSession (OTG only) TRUE if session is active
 * @field bIsHost (OTG only) TRUE if we are host
 * @field bConnectorId (OTG only) connector ID
 * @field bBusVoltage (OTG only) a VBUS_ constant
 * @field bEndCount count of endpoints
 * @field wEndMask mask of endpoints
 * @field pInterfacePort interface-level port
 * @field pPrivateData a "subclass" could use this
 * @field LocalEnds local endpoint resources
 * @field pOtgClient OTG client pointer
 * @field bIsHnpSupported TRUE if we can HNP
 * @field bIsHnpAllowed TRUE if HNP is allowed by host
 * @field EnumerationData enumeration data
 * @field pHostClient host client pointer
 * @field pRootDevice root device
 * @field ConnectedDeviceList connected devices
 * @field pFunctionClient function client pointer
 * @field apConfigDescriptors
 * @field pCurrentConfig
 * @field apPipe automatically-opened pipes on config
 * @field bMaxFunctionEnds size of above array
 * @field bCanWakeup TRUE if remote wakeup allowed
 * @field dwSequenceNumber sequence number for request/response
 * @field wSetupRxIndex current index in above
 * @field wSetupDataSize size of extended data phase
 * @field pSetupData SETUP IN data, if any
 * @field wSetupDataIndex current index in above
 * @field bSetAddress TRUE to set address after current transaction
 * @field bConfigValue current configuration value
 * @field IsrOverhead ISR overhead measurements
 * @field OverheadStats measured overheads
 * @field EndpointStats endpoint statistics
 * @field dwResetCount
 * @field dwSuspendCount
 * @field dwResumeCount
 * @field dwResponseTimeoutCount
 * @field pDmaController
 */
typedef struct _MGC_Port
{
    MGC_Controller* pController;
    MUSB_Port* pInterfacePort; /* MUSB_Port* pPort */
    void* pPrivateData;   /* MUSB_MemAlloc(MUSB_C_NUM_EPS *	sizeof(MGC_EndpointResource)); 
                                            pResult->pStaticBuffer,   */
    MUSB_Array LocalEnds;

    /* NOTE: pfProgramBusState must act on these values: */
    uint8_t bWantHighSpeed;
    uint8_t bWantReset;
    uint8_t bWantSuspend;
    uint8_t bWantResume;
    uint8_t bFuncAddr;
    uint8_t bWantSession;

    uint8_t bIsMultipoint;
    uint8_t bEndCount;

    /* NOTE: pfReadBusState must update these values: */
#if defined(MUSB_ISO) || defined(MUSB_SCHEDULER)
    uint32_t dwFrame;
#endif
    uint8_t bIsHighSpeed;
    uint8_t bIsLowSpeed;
    uint8_t bIsReset;
    uint8_t bIsSuspend;
    uint8_t bIsResume;
    uint8_t bIsSession;
    uint8_t bIsHost;

#ifdef MUSB_FUNCTION
    uint8_t bWantTestMode;
    uint8_t bTestMode;
    uint8_t bEnd0Status;
#endif
    uint16_t wEndMask;
    uint8_t bDmaCompleted;
    uint8_t bHasDisablePing;
    uint8_t bInsert;

    uint8_t bOtgState;
#ifdef MUSB_OTG_FUNC
    uint8_t bVbusErrorRetries;
    uint8_t bWantHost;
    uint8_t bSessionActive;
    uint8_t bConnectorId;
    uint8_t bBusVoltage;
    uint8_t bRelinquish;
    uint8_t bIsHnpSupported;
    uint8_t bIsHnpAllowed;
    MUSB_OtgClient* pOtgClient;
#endif    

#if defined(MUSB_HOST) || defined(MUSB_OTG)
    MGC_EnumerationData EnumerationData;
    MUSB_HostClient* pHostClient;
    MUSB_pfDriverTimerExpired pfDriverTimerExpired;
    void* pDriverTimerData;
    MUSB_Device* pRootDevice;
    MUSB_LinkedList ConnectedDeviceList;
    uint8_t bDeviceListIndex;    
#if defined(MUSB_ISO) || defined(MUSB_SCHEDULER)
    MGC_Schedule Schedule;
#endif
#endif	/* host or OTG */

#if defined(MUSB_FUNCTION) || defined(MUSB_OTG)
    MUSB_FunctionClient* pFunctionClient;
    const MUSB_ConfigurationDescriptor** apConfigDescriptors;
    const MUSB_ConfigurationDescriptor* pCurrentConfig;
    MGC_Pipe** apPipe;
    MGC_Pipe *pPipe;
    uint8_t bUsbState;
    uint8_t bMaxFunctionEnds;
    uint8_t bCanWakeup;
    uint8_t bSetAddress;
    uint8_t bConfigValue;
    uint8_t bParse;
    uint16_t wSetupDataIndex;
    uint16_t wSetupRxIndex;
    uint16_t wSetupDataSize;
    uint8_t* pSetupData;
    uint32_t dwSequenceNumber;
#endif

#ifdef MUSB_DMA
    MUSB_DmaController* pDmaController;
#endif

    MGC_pfReadBusState pfReadBusState;
    MGC_pfProgramBusState pfProgramBusState;
    MGC_pfResetPort pfResetPort;
    MGC_pfBindEndpoint pfBindEndpoint;
    MGC_pfProgramStartReceive pfProgramStartReceive;
    MGC_pfProgramStartTransmit pfProgramStartTransmit;
    MGC_pfProgramFlushEndpoint pfProgramFlushEndpoint;
#ifdef MUSB_FUNCTION    
    MGC_pfProgramHaltEndpoint pfProgramHaltEndpoint;
    MGC_pfDefaultEndResponse pfDefaultEndResponse;
#endif    
    MGC_pfServiceDefaultEnd pfServiceDefaultEnd;
    MGC_pfServiceTransmitAvail pfServiceTransmitAvail;
    MGC_pfServiceReceiveReady pfServiceReceiveReady;
    MGC_pfLoadFifo pfLoadFifo;
    MGC_pfUnloadFifo pfUnloadFifo;
    MGC_pfAcceptDevice pfAcceptDevice;
    MUSB_pfDmaChannelStatusChanged pfDmaChannelStatusChanged;
#ifdef MUSB_EHSET	        
    MGC_pfSetPortTestMode pfSetPortTestMode;
#endif    
} MGC_Port;

/*
 * Generic functions
 */

extern void MGC_ChangeUsbState(uint8_t bNewState, MGC_Port* pPort);

/**
 * Find descriptor.
 * Find a descriptor of a given type
 *
 * @param pBuffer buffer in which to search
 * @param wBufferLength size, in bytes, to assume for buffer
 * @param bDescriptorType the type of descriptor desired
 * @param bIndex the index (counting from 0) of the instance of the desired descriptor
 * (e.g. if there are 3 HID descriptors, bIndex==2 retrieves the third)
 *
 * @return descriptor pointer if found; NULL on failure
 */
extern const uint8_t* MGC_FindDescriptor(const uint8_t* pBuffer, 
					 uint16_t wBufferLength,
					 uint8_t bDescriptorType, 
					 uint8_t bIndex);

/**
 * Allocate a dynamic FIFO suitable for the given conditions.
 * @param pPort port pointer
 * @param pUsbEnd USB remote endpoint (host mode) or local endpoint (device mode)
 * @param bBind TRUE to actually allocate; FALSE to check for available space only
 * @param pbIsTx pointer to storage for whether the implied direction is transmit
 * @param bEndCount the number of endpoints in the core
 * @param dwTotalSize total RAM size, in bytes
 * @param dwHighWater the high-water mark (the initial allocation strategy
 * tries to use double-buffering; once allocation reaches this mark,
 * the allocation strategy becomes more conservative)
 */
extern MGC_EndpointResource *MGC_AllocateDynamicFifo(
    MGC_Port *pPort, const MUSB_DeviceEndpoint *pUsbEnd, uint8_t bBind, uint8_t *pbIsTx);

#ifdef MUSB_FUNCTION

/*
* Device mode polling TX
*/
extern uint8_t MGC_DevPollTx(MGC_Port *pPort, MGC_EndpointResource *pEnd, 
    const uint8_t *pBuffer, uint32_t dwTotalBytes);
#endif

#endif	/* multiple inclusion protection */
