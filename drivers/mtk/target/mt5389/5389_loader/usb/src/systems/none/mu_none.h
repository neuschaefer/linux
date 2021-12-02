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

#ifndef __MUSB_NONE_SYSTEM_H__
#define __MUSB_NONE_SYSTEM_H__

/*
 * Non-OS implementation of UDSI
 * $Revision: #1 $
 */

/**
 * Introduction to non-OS UDSI implementation.
 * The non-OS UDSI implementation must delegate more to 
 * the target-specific code.
 * This is because each non-OS target can be vastly different.
 * For example, it is likely that each non-OS target has a unique
 * memory-mapped I/O mechanism, memory protection strategy (if any),
 * ISR installation mechanism, shared interrupt mechanism (if any),
 * required implementation of timers, and console I/O mechanism (if any).
 * Target-specific code needs to implement:
 * <dl>
 * <dt>MUSB_ReadConsole
 * <dd>Blocking read of a console character
 * <dt>MUSB_WriteConsole
 * <dd>Blocking write of a console character
 * <dt>MUSB_BoardMessageString
 * <dd>Only used if diagnostic messages enabled
 * <dt>MUSB_BoardMessageNumber
 * <dd>Only used if diagnostic messages enabled
 * <dt>MUSB_BoardGetTime
 * <dd>Only used if overhead statistics are enabled
 * <dt>MUSB_BoardInitController
 * <dd>Called by the non-OS implementation of MUSB_InitSystem
 * <dt>MUSB_BoardInitTimers
 * <dd>Called by the non-OS implementation of MUSB_InitSystem
 * <dt>MUSB_BoardDestroyController
 * <dd>Called by the non-OS implementation of MUSB_DestroySystem
 * <dt>MUSB_BoardRunBackground
 * <dd>Board-specific background processing
 * <dt>MUSB_BoardArmTimer
 * <dd>Arm a timer
 * <dt>MUSB_BoardCancelTimer
 * <dd>Cancel a timer
 * <dt>MUSB_BoardPrintDiag
 * <dd>Only used if diagnostic messages enabled
 * <dt>MUSB_BoardSystemToBusAddress
 * <dd>Translate CPU address to bus address (for programming a DMA controller)
 * </dl>
 */

#include "mu_dsi.h"

/***************************** TYPES ******************************/

/**
 * Board-specific information about a controller
 * @field wType one of the MUSB_CONTROLLER_* constants
 * @field pBase base address for hard-wired controller; vendor ID for PCI
 * @field dwInfo IRQ for hard-wired controller; device ID for PCI
 */
typedef struct
{
    uint16_t wType;
    void* pBase;
    uint32_t dwInfo;
} MUSB_NoneController;

/**
 * Non-OS ISR
 * @param pPrivateData non-OS UDS instance data
 * @return TRUE if interrupt was from controller
 * @return FALSE if interrupt was not from controller
 */
typedef uint8_t (*MUSB_NoneIsr)(void* pPrivateData);

/** Wrapping None Timer */

typedef struct
{
    uint32_t dwCount;
    uint32_t dwTime;
    uint32_t dwTimeOut;
    uint32_t dwIndex;
    MUSB_pfTimerExpired pfExpired;
    void *pExpireParam; /* MGC_Controller ControllerImpl */
} MGC_NoneTimerWrapper;

/**************************** GLOBALS *****************************/

/**
 * Board-specific array of controller information;
 * defined in board.h.
 * PCI-based controllers should all come before any hard-wired ones
 */
extern MUSB_NoneController MUSB_aNoneController[];

/*************************** FUNCTIONS ****************************/

/**
 * Target-specific code should call this from its main loop.
 * This performs the UCD's background processing, if any.
 */
extern void MUSB_NoneRunBackground(void);

/**
 * Target-specific per-controller initialization.
 * @param pPrivateData non-OS UDS instance data
 * @param pfIsr ISR to call (pass pPrivateData)
 * @param pControllerInfo controller information
 * @param ppBaseIsr pointer to controller base address required for
 * use by ISR; updated by target-specific code (PCI only)
 * @param ppBaseBsr pointer to controller base address required for
 * use by BSR; updated by target-specific code (PCI only)
 * @return non-NULL pointer to private data on success
 * @return NULL on failure
 */
extern void* MUSB_BoardInitController(void* pPrivateData, MUSB_NoneIsr pfIsr,
				      const MUSB_NoneController* pControllerInfo,
				      uint8_t** ppBaseIsr, uint8_t** ppBaseBsr);

/**
 * Target-specific per-controller timer initialization.
 * @param pPrivateData result of a successful call to MUSB_BoardInitController
 * @return TRUE on success
 * @return NULL on failure
 */
extern uint8_t MUSB_BoardInitTimers(void* pPrivateData);

/**
 * Board-specific controller destruction
 * @param pPrivateData result of a successful call to MUSB_BoardInitController
 * on the corresponding controller
 */
extern void MUSB_BoardDestroyController(void* pPrivateData);

/**
 * Board-specific background processing
 * @param pPrivateData result of a successful call to MUSB_BoardInitController
 * on the corresponding controller
 */
//extern void MUSB_BoardRunBackground(void* pPrivateData);

/**
 * Board-specific timer arm
 * @param pPrivateData result of a successful call to MUSB_BoardInitController
 * on the corresponding controller
 * @param wIndex index of the timer to arm
 * @param dwTime timeout (in milliseconds)
 * @param bPeriodic TRUE for periodic; FALSE for one-shot
 * @param pfExpireCallback function to call on expiration
 * @param pParam expiration callback parameter
 * @return TRUE on success
 * @return FALSE on failure
 */
extern uint8_t MUSB_BoardArmTimer(void *pPrivateData, uint16_t wIndex);
/**
 * Board-specific timer disarm
 * @param pPrivateData result of a successful call to MUSB_BoardInitController
 * on the corresponding controller
 * @param wIndex index of the timer to cancel
 * @return void 
 */
extern void MUSB_BoardCancelTimer(void* pPrivateData, uint16_t wIndex);

/**
 * Print diagnostic.
 * A controller calls this to print a diagnostic message.
 *
 * @param pPrivateData result of a successful call to MUSB_BoardInitController
 * on the corresponding controller
 * @param pMessage NUL-terminated (C-style) string
 * @return TRUE on success
 * @return FALSE on failure
 */
extern uint8_t MUSB_BoardPrintDiag(void* pPrivateData, const CHAR* pMessage);

/**
 * Board- and bus- specific address translation
 * @param pSystemAddress address in CPU space
 * @return address as seen by a master on the bus where the controller resides
 */
extern void* MUSB_BoardSystemToBusAddress(const void* pSystemAddress);

/**
 * Board-specific disable interrupt lock.
 * @param pPrivateData result of a successful call to MUSB_BoardInitController
 * on the corresponding controller
 * @return previous interrupt status.
 */
extern uint32_t MUSB_BoardIrqLock(void *pPrivateData);

/**
 * Board-specific reenable interrupt lock.
 * @param pPrivateData result of a successful call to MUSB_BoardInitController
 * on the corresponding controller
 * @param dwPrevIrq previous interrupt status
 * @return void.
 */
extern void MUSB_BoardIrqUnlock(void *pPrivateData, uint32_t dwPrevIrq);

#endif	/* multiple inclusion protection */
