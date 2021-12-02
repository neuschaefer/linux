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
 * MUSBStack-S Array/List Utilities API.
 * $Revision: #1 $
 */

#ifndef __MUSB_LIST_H__
#define __MUSB_LIST_H__

#include "mu_tools.h"
#include "mu_mem.h"

/**
* MUSB_Array.
* A "dynamic" array which can partially or completely 
* use static storage.
* This is most useful for a list whose size is likely to grow
* but not shrink (e.g. a controller's local endpoint resources).
*
* @field wItemSize size of each item
*
* @field wStaticItemCount number of items static buffer has space for
*
* @field pStaticBuffer static buffer, if any
*
* @field wItemCount current item count
*
* @field wDynamicItemCount number of items dynamic buffer has space for
*
* @field pDynamicBuffer grows as needed, by dwItemSize*dwStaticItemCount
*/
typedef struct
{
    uint_fast16_t wItemSize;
    uint_fast16_t wStaticItemCount;
    void* pStaticBuffer;
    uint_fast16_t wItemCount;
    //uint_fast16_t wDynamicItemCount;
    //void* pDynamicBuffer;
} MUSB_Array;

/**
 * Initialize an array.
 * Call this (portable) or initialize the struct members (non-portable).
 * @param pArray pointer to user-allocated struct or NULL to allocate one
 * @param wItemSize size of each array element
 * @param wStaticItemCount the number of elements for which space is statically allocated
 * @param pStaticBuffer the buffer of statically-allocated elements
 * @return a non-NULL pointer on success (the given pArray if non-NULL)
 * @return NULL on failure
 */
extern MUSB_Array* MUSB_ArrayInit(MUSB_Array* pArray, 
				  uint_fast16_t wItemSize,
				  uint_fast16_t wStaticItemCount, 
				  void* pStaticBuffer);

/**
 * Count array items.
 * @param pArray array pointer
 * @return the number of items currently in the array
 */
extern uint_fast16_t MUSB_ArrayLength(const MUSB_Array* pArray);

/**
 * Get an array item.
 * Locking access to the array, if needed, must be done outside these functions.
 * @param pArray array pointer
 * @param wIndex the index (counting from 0)
 * @return pointer to array element or NULL if not found
 */
extern void* MUSB_ArrayFetch(const MUSB_Array* pArray, uint_fast16_t wIndex);

/**
 * Append to array.
 * Append an item to the end of an array.
 * Locking access to the array, if needed, must be done outside these functions.
 * @param pArray array pointer
 * @param pItem item pointer
 * @return TRUE on success
 * @return FALSE on failure (out of memory)
 */
extern void MUSB_ArrayAppend(MUSB_Array* pArray, const void* pItem);

/**
 * Remove all items from an array.
 * Locking access to the array, if needed, must be done outside these functions.
 * @memo clear array
 * @param pArray array pointer
 */
extern void MUSB_ArrayClear(MUSB_Array* pArray);

/**
 * MUSB_LinkedList (linked list).
 * A generic singly-linked list (can be implemented as static array).
 * @field pItem generic item pointer
 * @field pNext link to next
 * @field dwCount reference count, visitor, etc. (application's choice)
 */
typedef struct _MUSB_LinkedList
{
    void* pItem;
    struct _MUSB_LinkedList* pNext;
    uint32_t dwCount;
} MUSB_LinkedList;

/**
 * REQUIRED list initialization.
 * Initialize the given list so it is ready for use.
 * @param pList list pointer
 */
extern void MUSB_ListInit(MUSB_LinkedList* pList);

/**
 * Count list items.
 * Get the number of items in the given list
 * (NOTE: currently, this is time(N), because it traverses the list).
 * Locking access to the list, if needed, must be done outside these functions.
 * @param pList list pointer
 * @return number of items in list
 */
extern uint_fast16_t MUSB_ListLength(const MUSB_LinkedList* pList);

/**
 * Find list item.
 * Find the index'th item in the given list
 * (NOTE: currently, this is time(N), because it traverses the list).
 * @param pList list pointer
 * @param wIndex the index (counting from 0) of the desired item
 * @return item pointer if found
 * @return NULL if no such item
 */
extern void* MUSB_ListFindItem(MUSB_LinkedList* pList, uint_fast16_t wIndex);

/**
 * Find list record.
 * Find the index'th record in the given list
 * (NOTE: currently, this is time(N), because it traverses the list).
 * @param pList list pointer
 * @param wIndex the index (counting from 0) of the desired item
 * @return record pointer if found
 * @return NULL if no such item
 */
extern MUSB_LinkedList* MUSB_ListFindRecord(MUSB_LinkedList* pList, 
					    uint_fast16_t wIndex);

/**
 * Append list.
 * Append an item to the end of the given list.
 * Locking the list to prevent simultaneous insert/remove calls, if necessary,
 * is the caller's responsibility.
 * (NOTE: currently, this is time(N), because it traverses the list).
 * @param pList list pointer
 * @param pItem item pointer
 * @param dwCount value for count field (application's choice)
 * @return TRUE on success
 * @return FALSE on failure (no such list or out of memory)
 */
extern uint8_t MUSB_ListAppendItem(MUSB_LinkedList* pList, void* pItem, 
				   uint32_t dwCount);

/**
 * Insert into list.
 * Insert an item at the given position in the given list.
 * (NOTE: currently, this is time(N), because it traverses the list).
 * Locking the list to prevent simultaneous append/remove calls, if necessary,
 * is the caller's responsibility.
 * @param pList list pointer
 * @param wItemIndex insertion position (counting from 0)
 * @param pItem item pointer
 * @param dwCount value for count field (application's choice)
 * @return TRUE on success
 * @return FALSE on failure (no such list or out of memory)
 */
extern uint8_t MUSB_ListInsertItem(MUSB_LinkedList* pList, 
				   uint_fast16_t wItemIndex, 
				   void* pItem, uint32_t dwCount);

/**
 * Remove from list.
 * Remove an item from the given list.
 * (NOTE: currently, this is time(N), because it traverses the list).
 * Locking the list to prevent simultaneous insert/append calls, if necessary,
 * is the caller's responsibility.
 * @param pList list pointer
 * @param pItem item pointer
 */
extern void MUSB_ListRemoveItem(MUSB_LinkedList* pList, const void* pItem);

#endif	/* multiple inclusion protection */
