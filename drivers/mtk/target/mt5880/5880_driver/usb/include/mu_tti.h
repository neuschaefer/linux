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
 * MUSBStack-S TTI (Transport Test Interface).
 * $Revision: #1 $
 */

#ifndef __MUSB_TTI_H__
#define __MUSB_TTI_H__

#include "mu_tools.h"

/** 
 * Introduction to Transport Test Interface.
 */

/************************* TTI CONSTANTS **************************/

/** 
 * MUSB_TtiTestType.
 * TTI Test Type.
 */
typedef enum
{
    MUSB_TTI_TYPE_NONE,
    MUSB_TTI_TYPE_SINK,
    MUSB_TTI_TYPE_SOURCE,
    MUSB_TTI_TYPE_LOOPBACK
} MUSB_TtiTestType;

/** 
 * MUSB_TtiTestOption.
 * TTI Test Option.
 */
typedef enum
{
    MUSB_TTI_OPTION_NONE,
    MUSB_TTI_OPTION_INCREMENT,
    MUSB_TTI_OPTION_DECREMENT,
    MUSB_TTI_OPTION_ROTATE_LEFT,
    MUSB_TTI_OPTION_ROTATE_RIGHT
} MUSB_TtiTestOption;

/*************************** TTI TYPES ****************************/

/**
 * MUSB_TtiTestSpec.
 * Test specification.
 * @param dwIterations how many iterations of this test to do before moving
 * to the next test in the suite (if any)
 * @param dwActualIterations updated with actual iteration count
 * @param dwErrorIterations updated with iteration count that had errors
 * @field TestType the type of test
 */
typedef struct
{
    uint32_t dwIterations;
    uint32_t dwActualIterations;
    uint32_t dwErrorIterations;
    uint16_t wMaxPacketSize;
    MUSB_TtiTestType TestType;
    MUSB_TtiTestOption TestOption;
    uint8_t bProtocol;
    uint8_t bInEnd;
    uint8_t bOutEnd;
    uint8_t bStartPattern;
    uint8_t bPacketCount;
} MUSB_TtiTestSpec;
 
/**
 * Test progress callback.
 */
typedef void (*MUSB_pfTtiTestProgress)(MUSB_TtiTestSpec* pTestSpec);

/**
 * Test error callback.
 */
typedef void (*MUSB_pfTtiTestError)(MUSB_TtiTestSpec* pTestSpec, 
				    uint32_t dwCdiStatus, uint32_t dwFailedCompareCount);

/**
 * Notification of status of test addition request.
 * @param pTestSpec pointer to test specification
 * @param hTest non-NULL test "handle" on successful addition; NULL on failure
 */
typedef void (*MUSB_pfTtiTestAddStatus)(MUSB_TtiTestSpec* pTestSpec, void* hTest);

/**
 * Add a test
 * @param pDeviceData pPrivateData from dispatch table
 * @param pTestSpec test specification
 * @param pfTestAddStatus notification of status of this request
 * @param pfTestProgress test progress callback; NULL for none
 * @param pfTestError test error callback; NULL for none
 */
typedef void (*MUSB_pfTtiAddTest)(void* pDeviceData, MUSB_TtiTestSpec* pTestSpec, 
				  MUSB_pfTtiTestAddStatus pfTestAddStatus,
				  MUSB_pfTtiTestProgress pfTestProgress,
				  MUSB_pfTtiTestError pfTestError);

/**
 * Remove a test
 * @param pDeviceData pPrivateData from dispatch table
 * @param hTest test "handle" from MUSB_pfTestAddStatus callback by MUSB_pfTtiAddTest
 * @see #MUSB_pfTtiAddTest
 */
typedef void (*MUSB_pfTtiRemoveTest)(void* pDeviceData, void* hTest);

/**
 * Start tests
 * @param pDeviceData pPrivateData from dispatch table
 * @param dwIterationCount number of iterations of the complete test suite
 * (0 => infinite)
 */
typedef void (*MUSB_pfTtiStartTests)(void* pDeviceData, uint32_t dwIterationCount);

/**
 * Stop tests
 * @param pDeviceData pPrivateData from dispatch table
 */
typedef void (*MUSB_pfTtiStopTests)(void* pDeviceData);

/**
 * MUSB_TtiRunner.
 * Device driver callbacks (dispatch table).
 * @field pPrivateData device driver private data; 
 * not to be interpreted by the TTI implementation
 * @field pfTtiAddTest add-test callback
 * @field pfTtiRemoveTest remove-test callback
 * @field pfTtiStartTests start-tests callback
 * @field pfTtiStopTests stop-tests callback
 */
typedef struct 
{
    void* pPrivateData;
    MUSB_pfTtiAddTest pfTtiAddTest;
    MUSB_pfTtiRemoveTest pfTtiRemoveTest;
    MUSB_pfTtiStartTests pfTtiStartTests;
    MUSB_pfTtiStopTests pfTtiStopTests;
} MUSB_TtiRunner;

/************************* TTI FUNCTIONS **************************/

/**
 * Add a transport (test) device.
 * A transport device driver calls this to inform the TTI implementation
 * that a new test device is available.
 * 
 * @param pTestRunner pointer to struct with dispatch table to access the device
 */
extern void MUSB_TtiAddTestRunner(MUSB_TtiRunner* pRunner);

/**
 * Remove a transport (test) device.
 * A transport device driver calls this to inform the TTI implementation
 * that a test device is no longer available.
 * 
 * @param pTestRunner pointer to struct with dispatch table to access the device
 */
extern void MUSB_TtiRemoveTestRunner(MUSB_TtiRunner* pRunner);

#endif	/* multiple inclusion protection */
