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
 * MUSBStack-S USB Driver System Interface (UDSI).
 * $Revision: #1 $
 */

#ifndef __MUSB_SYSTEM_H__
#define __MUSB_SYSTEM_H__

#include "mu_tools.h"
#include "mu_stat.h"

/****************** SYSTEM INTERFACE CONSTANTS ********************/

/** Current controller struct version */
#define MUSB_CONTROLLER_VERSION		1

/** Current system utils struct version */
#define MUSB_SYSTEM_UTILS_VERSION	1

/** Current system services struct version */
#define MUSB_SYSTEM_SERVICES_VERSION	1

/** The controller type for the FDRC */
#define MUSB_CONTROLLER_FDRC		1

/** The controller type for the HDRC */
#define MUSB_CONTROLLER_HDRC		2

/** The controller type for the MHDRC */
#define MUSB_CONTROLLER_MHDRC		3

/** The controller type for the LSFC */
#define MUSB_CONTROLLER_LSFC		4

/** The controller type for the FSFC */
#define MUSB_CONTROLLER_FSFC		5

/** The controller type for the HSFC */
#define MUSB_CONTROLLER_HSFC		6

/******************** SYSTEM INTERFACE TYPES **********************/

/**
 * Controller ISR.
 * System glue calls this upon controller interrupt.
 *
 * @param 1 pIsrParam from MUSB_Controller
 *
 * @return -1: the interrupt did not seem to be ours (spurious or shared)
 * @return 0: interrupt processed; no need to wake IST
 * @return >0: number of IST work items queued; please wake IST
 */
typedef int32_t (*MUSB_pfControllerIsr)(void*);

/**
 * Controller BSR (Background Service Routine).
 * When the ISR return value so indicates,
 * the system glue should call this function for non-interrupt-time processing.
 * This is typically done from a dedicated high-priority task/thread or
 * the main loop in a non-concurrent system.
 *
 * @param 1 pBsrParam from MUSB_Controller
 */
typedef void (*MUSB_pfControllerBsr)(void*);

/**
 * Timer expiration callback.
 * System glue calls this when a timer expires.
 * This must be done from a context in which it is safe to call the
 * lock/unlock services.
 *
 * @param 1 pPrivateData from MUSB_Controller
 * @param 2 index of timer (counting from 0)
 * @see #MUSB_pfLock
 * @see #MUSB_pfUnlock
 */
typedef void (*MUSB_pfTimerExpired)(void*, uint16_t);

/**
 * MUSB_Controller.
 * A controller instance.
 * This is returned by a controller initialization function,
 * and provides everything necessary for the system glue 
 * to support the controller's operation.
 *
 * @field wVersion the controller fills this with its interface version,
 * so the System can check for compatibility
 *
 * @field pPrivateData controller's private data; 
 * not to be interpreted by the System
 *
 * @field wQueueLength the maximum number of items the controller requires
 * in the background message queue
 *
 * @field wQueueItemSize the size (in bytes) of each item 
 * the controller will place in the background queue
 *
 * @field wTimerCount the number of timers required for the controller's operation;
 * the System should reserve and/or prepare this many timers for the controller's use
 *
 * @field wLockCount the number of locks required for the controller's operation;
 * the System should reserve and/or prepare this many locks for the controller's use
 *
 * @field pfIsr the controller's ISR; 
 * the System must call this upon interrupt receipt
 *
 * @field pIsrParam the parameter the System must pass to the ISR
 *
 * @field pfBsr the controller's background processing code;
 * the System must call this if the ISR's return value so indicates
 *
 * @field pBsrParam the parameter the System must pass to the BSR
 */
typedef struct
{
    uint16_t wVersion;
    void* pPrivateData;   /* MGC_Controller* pControllerImpl */
    uint16_t wQueueLength;
    uint16_t wQueueItemSize;
    MUSB_pfControllerIsr pfIsr;
    void* pIsrParam;  /* MUSB_Controller* pController */
    MUSB_pfControllerBsr pfBsr;
    void* pBsrParam; /* MUSB_Port* pPort */
} MUSB_Controller;

/**
 * Get address for DMA.
 * A controller calls this to get a bus address (for DMA) 
 * from a system address.
 *
 * @param 1 system address
 *
 * @return bus address on success; NULL on failure
 */
typedef void* (*MUSB_pfSystemToBusAddress)(const void*);

/**
 * Enqueue background task.
 * A controller calls this to enqueue a background item.
 *
 * @param 1 pPrivateData from MUSB_SystemServices
 * @param 2 pointer to an item of wQueueItemSize bytes;
 * data should be copied into the queue
 *
 * @return TRUE on success
 * @return FALSE on failure
 */
typedef uint8_t (*MUSB_pfQueueBackgroundItem)(void*, const void*);

/**
 * Dequeue background task.
 * A controller calls this to dequeue a background item.
 *
 * @param 1 pPrivateData from MUSB_SystemServices
 * @param 2 pointer to storage for an item of wQueueItemSize bytes;
 * the implementation should copy data here
 *
 * @return TRUE on success
 * @return FALSE on failure
 */
typedef uint8_t (*MUSB_pfDequeueBackgroundItem)(void*, void*);

/**
 * Flush background queue.
 * A controller calls this to flush the background item queue
 *
 * @param 1 pPrivateData from MUSB_SystemServices
 */
typedef void (*MUSB_pfFlushBackgroundQueue)(void*);

/**
 * Arm a timer.
 * A controller calls this to arm a timer (periodic or one-shot).
 *
 * @param 1 pPrivateData from MUSB_SystemServices
 * @param 2 index of timer (counting from 0)
 * @param 3 time (milliseconds)
 * @param 4 TRUE for periodic; FALSE for one-shot
 * @param 5 function to call on expiration
 * @param 6 function parameter to call on expiration
 * @return TRUE on success
 * @return FALSE on failure
 */
typedef uint8_t (*MUSB_pfArmTimer)(void*, uint16_t, uint32_t, uint8_t, 
				    MUSB_pfTimerExpired, void*);

/**
 * Disarm a timer.
 * A controller calls this to cancel a timer.
 *
 * @param 1 pPrivateData from MUSB_SystemServices
 * @param 2 index of timer (counting from 0)
 * @return TRUE on success
 * @return FALSE on failure
 */
typedef uint8_t (*MUSB_pfCancelTimer)(void*, uint16_t);

/**
 * Lock.
 * A controller calls this to enter a lock.
 *
 * @param 1 pPrivateData from MUSB_SystemServices
 * @param 2 lock index (counting from 0)
 * @return TRUE on success
 * @return FALSE on failure
 */
typedef uint8_t (*MUSB_pfLock)(void*, uint16_t);

/**
 * Unlock.
 * A controller calls this to exit a lock.
 *
 * @param 1 pPrivateData from MUSB_SystemServices
 * @param 2 lock index (counting from 0)
 * @return TRUE on success
 * @return FALSE on failure
 */
typedef uint8_t (*MUSB_pfUnlock)(void*, uint16_t);

/**
 * IRQ Lock.
 * A controller calls this to enter a IRQ lock.
 *
 * @param 1 pPrivateData from MUSB_SystemServices
 * @return Previous irq status.
 */
typedef uint32_t (*MUSB_pfIrqLock)(void*);

/**
 * IRQ UnLock.
 * A controller calls this to enter a IRQ unlock.
 *
 * @param 1 pPrivateData from MUSB_SystemServices
 * @param 2 Previous irq status
 * @return Previous irq status.
 */
typedef void (*MUSB_pfIrqUnlock)(void*, uint32_t);

/**
 * IRQ IsIrqPending.
 * A controller calls this to check IRQ status.
 *
 * @param 1 pPrivateData from MUSB_SystemServices
 * @return Irq status.
 */
typedef uint32_t (*MUSB_IsIrqPending)(void*);

/**
 * Print diagnostic.
 * A controller calls this to print a diagnostic message.
 *
 * @param 1 pPrivateData from MUSB_SystemServices
 * @param 2 NUL-terminated (C-style) string
 * @return TRUE on success
 * @return FALSE on failure
 */
typedef uint8_t (*MUSB_pfPrintDiag)(void*, const CHAR*);

/**
 * MUSB_SystemServices.
 * Services for a controller.
 *
 * This is provided by System-specific code to support a controller's operation.
 *
 * @field wVersion the System fills this with its current interface version
 * so the controller can check for compatibility
 *
 * @field pPrivateData System data; not to be interpreted by controller
 *
 * @field pfSystemToBusAddress function to convert system to bus address for DMA
 *
 * @field pfQueueBackgroundItem function to add an item to the background queue
 *
 * @field pfDequeueBackgroundItem function to remove an item 
 * from the background queue
 *
 * @field pfFlushBackgroundQueue function to flush the background queue
 *
 * @field pfArmTimer function to arm a timer
 *
 * @field pfCancelTimer function to cancel a previsouly-armed timer
 *
 * @field pfLock function to lock (obtain a mutex)
 *
 * @field pfUnlock function to unlock (release a mutex)
 *
 * @field pfPrintDiag function to print a diagnostic message
 */
typedef struct
{
    uint16_t wVersion;
    void* pPrivateData; /* MGC_UcosSystem* pSystem or MGC_NoneSystem* pSystem */
    MUSB_pfSystemToBusAddress pfSystemToBusAddress;
    MUSB_pfQueueBackgroundItem pfQueueBackgroundItem;
    MUSB_pfDequeueBackgroundItem pfDequeueBackgroundItem;
    MUSB_pfFlushBackgroundQueue pfFlushBackgroundQueue;
    MUSB_pfArmTimer pfArmTimer;
    MUSB_pfCancelTimer pfCancelTimer;
#if !defined(MUSB_8226) && !defined(MUSB_LOADER)
    MUSB_pfLock pfClassLock;
    MUSB_pfUnlock pfClassUnlock;    
    MUSB_pfLock pfIrpLock;
    MUSB_pfUnlock pfIrpUnlock;    
#endif
    MUSB_pfIrqLock pfIrqLock;
    MUSB_pfIrqUnlock pfIrqUnlock;
    MUSB_IsIrqPending pfIsIrqPending;
} MUSB_SystemServices;

/****************** SYSTEM INTERFACE FUNCTIONS ********************/

/**
 * REQUIRED Initialization.
 * Initialize the USB system.
 * This is typically called by startup code, like the application
 * on a single-application system.
 * @param dwBsrPriority the priority to use for the UCD's BSR(s).
 * Run-time configuration is necessary to allow applications 
 * built on binary distributions to choose a workable priority scheme.
 * The interpretation and allowed range is necessarily system-specific,
 * so please consult the system-specific documentation for this information.
 * @return TRUE on success
 * @return FALSE on failure (out of memory; system-specific reasons)
 */
extern uint8_t MUSB_InitSystem(uint32_t dwBsrPriority);

/**
 * Shut down the USB system.
 * @return TRUE on success
 * @return FALSE on failure (system-specific reasons)
 */
extern void MUSB_DestroySystem(void);

/**
 * Create a controller.
 * Instantiate a controller (perhaps from a pool defined at build time).
 *
 * @param wControllerType the controller type (0 to discover)
 * @param pControllerAddressIsr the address of the controller's registers 
 * as seen by the ISR
 * @param pControllerAddressBsr the address of the controller's registers 
 * as seen by the BSR
 *
 * @return a non-NULL instance on success
 */
extern MUSB_Controller* MUSB_NewController(
    uint16_t wControllerType,
    void* pControllerAddressIsr,
    void* pControllerAddressBsr
    );

/**
 * Start (or restart) a controller.
 * The system glue must not call this until it is ready
 * to accept calls to its services
 *
 * @param pController an instance returned by one of the init functions
 * @param pSystemServices the services provided by the System glue
 *
 * @return status code
 */
extern uint32_t MUSB_StartController(const MUSB_Controller* pController, 
				     MUSB_SystemServices* pSystemServices
				     );

/**
 * Stop a controller.
 * Stop a controller (useful to prepare for warm reset or power-saving mode).
 *
 * @param pController an instance returned by one of the init functions
 *
 * @return status code
 */
extern void MUSB_StopController(const MUSB_Controller* pController);

/**
 * Destroy a controller.
 * Stop a controller and free any resources associated with it
 * (useful for system shutdown or plug-and-play device removal).
 *
 * @param pController an instance returned by one of the init functions
 *
 * @return void
 */
extern void MUSB_DestroyController(const MUSB_Controller* pController);

/**
 * Read a CHARacter from the console.
 * Read an ASCII/ISO-Latin-1 CHARacter from the console; 
 * blocking until one is available.
 * The console may be implemented in many different ways,
 * including scripting, so no assumption of true interactivity should be made.
 * @return the CHARacter
 */
//extern CHAR MUSB_ReadConsole(void);

/**
 * Write a CHARacter to the console.
 * Write an ASCII/ISO-Latin-1 CHARacter to the console.
 * @param bChar the CHARacter
 */
//extern void MUSB_WriteConsole(const CHAR bChar);

/**
 * Run other state loop.
 *   This API should be very carefully used. It will run other module state loop
 *   in here. This is a work around when using blocking usb IO read/write and 
 *   prevent usb block other module state.    
 * @param void
 * @return void
 */
extern void MUSB_OtherModuleLoop(void);

/**
 * Delay calling thread.
 * Delay the calling thread (allowing other threads to run if supported).
 * @param dwTime the minimum delay, in milliseconds
 * @return status code
 */
extern uint32_t MUSB_Sleep(uint32_t dwTime);

/**
 * MGC_bSystemCount.
 * system controller count. This must be smaller than macro MUSB_MAX_CONTROLLERS.
 */
extern uint8_t MGC_bSystemCount;
#endif	/* multiple inclusion protection */
