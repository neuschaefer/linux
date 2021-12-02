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
 * MUSBStack-S High-Speed Electrical Test (HSET) Interface.
 * $Revision: #1 $
 */

#ifndef __MUSB_HSET_H__
#define __MUSB_HSET_H__

#include "mu_cdi.h"

/************************ HSET CONSTANTS **************************/

/**
 * MUSB_HsetPortMode.
 * An HSET port mode.
 */
typedef enum
{
    /** No special port mode */
    MUSB_HSET_PORT_NONE,
    /** TEST_J mode */
    MUSB_HSET_PORT_TEST_J,
    /** TEST_K mode */
    MUSB_HSET_PORT_TEST_K,
    /** TEST_SE0_NAK mode */
    MUSB_HSET_PORT_TEST_SE0_NAK,
    /** TEST_PACKET mode */
    MUSB_HSET_PORT_TEST_PACKET,
    /** Reset */
    MUSB_HSET_PORT_RESET,
    /** Suspend */
    MUSB_HSET_PORT_SUSPEND,
    /** Resume */
    MUSB_HSET_PORT_RESUME,
    /** Perform SETUP based on FIFO contents */
    MUSB_HSET_PORT_SETUP_START,
    /** Perform IN data phase */
    MUSB_HSET_PORT_SETUP_IN,
    /** Perform status phase of SETUP */
    MUSB_HSET_PORT_SETUP_STATUS
} MUSB_HsetPortMode;

/**
 * MUSB_HsetDeviceTest.
 * An HSET device test.
 */
typedef enum
{
    /** No special device test mode */
    MUSB_HSET_DEVICE_TEST_NONE,
    /** put device in TEST_J mode */
    MUSB_HSET_DEVICE_TEST_J,
    /** put device in TEST_K mode */
    MUSB_HSET_DEVICE_TEST_K,
    /** put device in TEST_SE0_NAK mode */
    MUSB_HSET_DEVICE_TEST_SE0_NAK,
    /** put device in TEST_PACKET mode */
    MUSB_HSET_DEVICE_TEST_PACKET,
    /** perform SET_ADDRESS to device */
    MUSB_HSET_DEVICE_SET_ADDRESS,
    /** perform GET_DESCRIPTOR to device */
    MUSB_HSET_DEVICE_GET_DESCRIPTOR,
    /** perform disable remote wakeup to device */
    MUSB_HSET_DEVICE_DISABLE_WAKEUP,
    /** perform enable remote wakeup to device */
    MUSB_HSET_DEVICE_ENABLE_WAKEUP,
    /** perform SET_FEATURE to device */
    MUSB_HSET_DEVICE_SET_FEATURE
} MUSB_HsetDeviceTest;

/**
 * MUSB_HsetOption.
 * An option for the HSET functionality.
 */
typedef enum
{
    /** FORCE_ENABLE */
    MUSB_HSET_OPTION_FORCE_HOST,
    /** Loop operations when enabled */
    MUSB_HSET_OPTION_LOOP,
    /** Single-step operations when enabled */
    MUSB_HSET_OPTION_STEP,
    /** Log diagnostic information */
    MUSB_HSET_OPTION_LOG
} MUSB_HsetOption;

typedef enum
{
    MUSB_HSET_SUCCESS,
    MUSB_HSET_DISCONNECT,
    MUSB_HSET_STALL,
    MUSB_HSET_NAK_TIMEOUT,
    MUSB_HSET_RX_ERROR
} MUSB_HsetStatus;

/************************** HSET TYPES ****************************/

/** Opaque handle to port for HSET purposes */
typedef void* MUSB_HsetPort;

/**
 * MUSB_HsetDeviceInfo.
 * Device information for use by HSET clients.
 *
 * @field pDeviceDescriptor pointer to device descriptor
 * @field bBusAddress address assigned during enumeration
 */
typedef struct 
{
    const MUSB_DeviceDescriptor* pDeviceDescriptor;
    uint8_t bBusAddress;
} MUSB_HsetDeviceInfo;

/**
 * HSET status notification.
 * @param hPort HSET port handle from a previous call to MUSB_HsetPortStart
 * @param eStatus status
 * @see #MUSB_HsetPortStart
 */
typedef void (*MUSB_pfHsetStatus)(MUSB_HsetPort hPort, 
				  MUSB_HsetStatus eStatus);

/**
 * HSET log callback.
 * @param hPort HSET port handle from a previous call to MUSB_HsetPortStart
 * @param pLine a line of diagnostic information
 * @see #MUSB_HsetPortStart
 */
typedef void (*MUSB_pfHsetLog)(MUSB_HsetPort hPort, 
			       const CHAR* pLine);

/**
 * HSET device enumeration notification.
 * @param hPort HSET port handle from a previous call to MUSB_HsetPortStart
 * @param pDeviceInfo pointer to device information;
 * callee must copy if persistence is needed
 * @see #MUSB_HsetPortStart
 */
typedef void (*MUSB_pfHsetNewDevice)(MUSB_HsetPort hPort,
				     const MUSB_HsetDeviceInfo* pDeviceInfo);

/************************ HSET FUNCTIONS **************************/

/**
 * Start HSET on a port.
 * @param pPort UCDI port pointer
 * @param pfStatus status notification callback
 * @param pfLog logging callback
 * @param pfNewDevice device enumeration notification callback
 * @return non-NULL HSET port handle on success; NULL on failure
 */
extern MUSB_HsetPort MUSB_HsetPortStart(MUSB_Port* pPort, 
					MUSB_pfHsetStatus pfStatus,
					MUSB_pfHsetLog pfLog,
					MUSB_pfHsetNewDevice pfNewDevice);

/**
 * Stop HSET on a port.
 * @param hPort HSET port handle from a previous call to MUSB_HsetPortStart
 * @see #MUSB_HsetPortStart
 */
extern void MUSB_HsetPortStop(MUSB_HsetPort hPort);

/**
 * Set an HSET port option.
 * @param hPort HSET port handle from a previous call to MUSB_HsetPortStart
 * @param eOption option to set
 * @see #MUSB_HsetPortStart
 */
extern void MUSB_HsetPortSetOption(MUSB_HsetPort hPort, MUSB_HsetOption eOption);

/**
 * Clear (disable) an HSET port option.
 * @param hPort HSET port handle from a previous call to MUSB_HsetPortStart
 * @param eOption option to clear
 * @see #MUSB_HsetPortStart
 */
extern void MUSB_HsetPortClearOption(MUSB_HsetPort hPort, MUSB_HsetOption eOption);

/**
 * Set an HSET port mode.
 * The status callback will be notified with the operation's status.
 * @param hPort HSET port handle from a previous call to MUSB_HsetPortStart
 * @param eMode mode to set
 * @see #MUSB_HsetPortStart
 */
extern void MUSB_HsetPortSetMode(MUSB_HsetPort hPort, MUSB_HsetPortMode eMode);

/**
 * Enumerate devices on a port.
 * This will result in one NewDevice callback for each device successfully enumerated,
 * and a status callback with success at the end of enumeration.
 * @param hPort HSET port handle from a previous call to MUSB_HsetPortStart
 * @see #MUSB_HsetPortStart
 */
extern void MUSB_HsetPortEnumerate(MUSB_HsetPort hPort);

/**
 * Set the target address for subsequent HSET device testing.
 * @param hPort HSET port handle from a previous call to MUSB_HsetPortStart
 * @param bBusAddress the bus address of the target device
 * @see #MUSB_HsetPortStart
 */
extern void MUSB_HsetSetDevice(MUSB_HsetPort hPort, uint8_t bBusAddress);

/**
 * Set the address for the next SET_ADDRESS test.
 * @param hPort HSET port handle from a previous call to MUSB_HsetPortStart
 * @param bBusAddress the bus address for the next SET_ADDRESS test
 * @see #MUSB_HsetPortStart
 */
extern void MUSB_HsetSetTestAddress(MUSB_HsetPort hPort, uint8_t bBusAddress);

/**
 * Perform a device test.
 * The status callback will be notified when the operation is complete
 * (in loop mode, it is notified on each iteration).
 * @param hPort HSET port handle from a previous call to MUSB_HsetPortStart
 * @param eTest the test to perform
 * @see #MUSB_HsetPortStart
 */
extern void MUSB_HsetPortDeviceTest(MUSB_HsetPort hPort, MUSB_HsetDeviceTest eTest);

/**
 * Perform the next step during single-stepping.
 * @param hPort HSET port handle from a previous call to MUSB_HsetPortStart
 * @param bBusAddress the bus address of the target device
 * @see #MUSB_HsetPortStart
 */
extern void MUSB_HsetNextStep(MUSB_HsetPort hPort);

#endif	/* multiple inclusion protection */
