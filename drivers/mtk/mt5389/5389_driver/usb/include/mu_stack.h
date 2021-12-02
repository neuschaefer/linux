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
 * MUSBStack-S USB Software Platform Stack API.
 * $Revision: #1 $
 */

#ifndef __MUSB_STACK_H__
#define __MUSB_STACK_H__

#include "mu_tools.h"
#include "mu_list.h"

/**
 * MUSB_Stack.
 * A LIFO data container.
 * This stack is a Last-In-First-Out container for arbitrary data,
 * where each data element is assumed the same size.
 * A static data buffer can be used, but the stack can still grow beyond it.
 * @field wNextTopIndex index to use for next push,
 * which is one beyond the current top of the stack
 * @field Array array implementation this stack uses
 */
typedef struct
{
    uint_fast16_t wNextTopIndex;
    MUSB_Array Array;
} MUSB_Stack;

/**
 * Initialize a stack (a LIFO data container).
 * Call this (portable) or initialize the struct members (non-portable).
 * A static data buffer can be used, but the stack can still grow beyond it.
 * @param pStack pointer to user-allocated struct or NULL to allocate one
 * @param wItemSize size of each array element
 * @param wStaticItemCount the number of elements for which space is statically allocated
 * @param pStaticBuffer the buffer of statically-allocated elements
 * @return a non-NULL pointer on success (the given pStack if non-NULL)
 * @return NULL on failure
 */
extern MUSB_Stack* MUSB_StackInit(MUSB_Stack* pStack, 
				  uint_fast16_t wItemSize,
				  uint_fast16_t wStaticItemCount, 
				  void* pStaticBuffer);

/**
 * Remove all items from a stack.
 * Locking access to the stack, if needed, must be done outside these functions.
 * @param pStack stack pointer
 * @return TRUE on success
 * @return FALSE on failure (invalid stack)
 */
extern uint8_t MUSB_StackClear(MUSB_Stack* pStack);

/**
 * Count stack items.
 * @param pStack stack pointer
 * @return the number of items currently on the stack
 */
extern uint_fast16_t MUSB_StackSize(MUSB_Stack* pStack);

/**
 * Get the top stack item, without removing it.
 * @param pStack stack pointer
 * @return pointer to element or NULL if not found
 */
extern void* MUSB_StackTop(MUSB_Stack* pStack);

/**
 * Pop an item from the stack.
 * This removes and returns the top item.
 * Locking access to the stack, if needed, must be done outside these functions.
 * @param pStack stack pointer
 * @param pItem where to store the item
 * @return TRUE on success
 * @return FALSE on failure (invalid or empty stack)
 */
extern uint8_t MUSB_StackPop(MUSB_Stack* pStack, void* pItem);

/**
 * Push an item onto the stack.
 * The given item, if possible, is made the new top of the stack.
 * Locking access to the stack, if needed, must be done outside these functions.
 * @param pStack stack pointer
 * @param pItem pointer to the item
 * @return TRUE on success
 * @return FALSE on failure (invalid or full stack)
 */
extern uint8_t MUSB_StackPush(MUSB_Stack* pStack, const void* pItem);

#endif	/* multiple inclusion protection */
