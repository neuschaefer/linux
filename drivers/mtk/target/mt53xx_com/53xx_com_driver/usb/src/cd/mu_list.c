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
 * MUSBStack-S array/list implementation.
 * $Revision: #1 $
 */

#include "mu_list.h"

/* Call this (portable) or initialize struct (non-portable) */

#define MGC_LIST_NUM        (40)

typedef struct _MUSB_LinkedListTracker
{
    MUSB_LinkedList rList;
    BOOL fgInUsed;

} MUSB_LinkedListTracker;

static MUSB_LinkedListTracker MGC_aListTracker[MGC_LIST_NUM];

#if !defined(MUSB_8226) && !defined(MUSB_LOADER)
static BOOL fgMUSB_ListInit = FALSE;
static HANDLE_T hMUSB_ListLock;
#endif

MUSB_Array *MUSB_ArrayInit(MUSB_Array *pArray, uint_fast16_t wItemSize,
                           uint_fast16_t wStaticItemCount, void *pStaticBuffer)
{
    MUSB_Array *pResult = pArray;

    if (!pArray)
    {
        return NULL;
    }

    if (pResult)
    {
        pResult->wItemSize = wItemSize;

        if (pStaticBuffer)
        {
            pResult->wStaticItemCount = wStaticItemCount;
        }
        else
        {
            pResult->wStaticItemCount = 0;
        }

        pResult->pStaticBuffer = pStaticBuffer;
        pResult->wItemCount = 0;
        //pResult->wDynamicItemCount = 0;
        //pResult->pDynamicBuffer = NULL;
    }

    return pResult;
}

/* How many items are in array? */
uint_fast16_t MUSB_ArrayLength(const MUSB_Array *pArray)
{
    uint_fast16_t result = 0;

    if (pArray)
    {
        result = pArray->wItemCount;
    }

    return result;
}

/* Get the item at the given index */
void *MUSB_ArrayFetch(const MUSB_Array *pArray, uint_fast16_t wIndex)
{
    void *pResult = NULL;

    if (pArray && (wIndex < pArray->wItemCount))
    {        
        if (wIndex < pArray->wStaticItemCount)
        {
            pResult = (uint8_t *)pArray->pStaticBuffer + (wIndex *pArray->wItemSize);
        }        
        else
        {
            pResult = NULL;
/*        
            pResult = (uint8_t *)pArray->pDynamicBuffer + ((wIndex - pArray->wStaticItemCount)
                                                               *pArray->wItemSize);
*/                                                               
        }
    }

    return pResult;
}

/* Append an item */
void MUSB_ArrayAppend(MUSB_Array *pArray, const void *pItem)
{
    uint8_t bResult = FALSE;

    void *pDest = NULL;
    uint_fast16_t wCount;
    //uint_fast16_t wIncrement;
    //uint_fast16_t wNewCount;

    MUSB_ASSERT(pArray);
    MUSB_ASSERT(pItem);    

    wCount = pArray->wItemCount;

    if (wCount < pArray->wStaticItemCount)
    {
        /* simplest case: static space */
        bResult = TRUE;
        pDest = (uint8_t *)pArray->pStaticBuffer + (wCount *pArray->wItemSize);
    }
    else
    {
        bResult = FALSE;
    }
#if 0    
    else if (wCount < (pArray->wStaticItemCount + pArray->wDynamicItemCount))
    {
        /* dynamic buffer has space */
        bResult = TRUE;
        pDest = (uint8_t *)pArray->pDynamicBuffer + ((wCount - pArray->wStaticItemCount)
                                                         *pArray->wItemSize);
    }
    else
    {
        /* need to grow dynamic buffer */
        wIncrement = MUSB_MAX(pArray->wStaticItemCount, 2);

        wNewCount = wCount + wIncrement;
        pDest = MUSB_MemRealloc(pArray->pDynamicBuffer, wNewCount *pArray->wItemSize);
        if (pDest)
        {
            bResult = TRUE;

            pArray->pDynamicBuffer = pDest;
            pArray->wDynamicItemCount += wIncrement;
            pDest = (uint8_t *)pArray->pDynamicBuffer + ((wCount - pArray->wStaticItemCount)
                                                             *pArray->wItemSize);
        }
    }
#endif

    if (bResult)
    {
        MUSB_MemCopy(pDest, pItem, pArray->wItemSize);
        pArray->wItemCount++;
    }

    MUSB_ASSERT(bResult);
}

/* Remove all array items */
#if 0        
void MUSB_ArrayClear(MUSB_Array *pArray)
{
    if (pArray)
    {
        pArray->wItemCount = 0;
        if (pArray->pDynamicBuffer)
        {
            MUSB_MemFree(pArray->pDynamicBuffer);

            pArray->pDynamicBuffer = NULL;
            pArray->wDynamicItemCount = 0;
        }
    }
}
#endif        

static MUSB_LinkedList *MGC_AllocList(void)
{
    uint32_t i;
    MUSB_LinkedList *pList = NULL;

#if !defined(MUSB_8226) && !defined(MUSB_LOADER)
    VERIFY(OSR_OK == x_sema_lock(hMUSB_ListLock, X_SEMA_OPTION_WAIT));
#endif
    
    for (i=0; i<MGC_LIST_NUM; i++)
    {
        if (!MGC_aListTracker[i].fgInUsed)
        {
            MGC_aListTracker[i].fgInUsed = TRUE;
            pList = (MUSB_LinkedList *)&MGC_aListTracker[i].rList;
            break;
        }
    }

#if !defined(MUSB_8226) && !defined(MUSB_LOADER)    
    VERIFY(OSR_OK == x_sema_unlock(hMUSB_ListLock));
#endif
    
    return pList;
}

static void * MGC_FreeList(MUSB_LinkedList *pList)
{
    uint32_t i;
    MUSB_LinkedList *pFreeList = NULL;

#if !defined(MUSB_8226) && !defined(MUSB_LOADER)
    VERIFY(OSR_OK == x_sema_lock(hMUSB_ListLock, X_SEMA_OPTION_WAIT));
#endif
    
    for (i=0; i<MGC_LIST_NUM; i++)
    {
        if (((MUSB_LinkedList *)&MGC_aListTracker[i].rList) == pList)
        {
            pList->dwCount = 0;
            pList->pItem = NULL;
            pList->pNext = NULL;
            
            MUSB_ASSERT(MGC_aListTracker[i].fgInUsed);
            MGC_aListTracker[i].fgInUsed = FALSE;
            pFreeList = pList;
            break;
        }
    }

#if !defined(MUSB_8226) && !defined(MUSB_LOADER)    
    VERIFY(OSR_OK == x_sema_unlock(hMUSB_ListLock));
#endif
    
    return pFreeList;
}

/* Initialize the given list */
void MUSB_ListInit(MUSB_LinkedList *pList)
{

#if !defined(MUSB_8226) && !defined(MUSB_LOADER)
    if (!fgMUSB_ListInit)
    {
        fgMUSB_ListInit = TRUE;
        
        //  MTK Notice: Max Liao, 2006/11/23.
        //  Class semphore protect BSR task and user's task sending job at the same time.
        VERIFY(OSR_OK == x_sema_create(&hMUSB_ListLock, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_UNLOCK));
    }
#endif

    if (pList)
    {
        pList->pNext = NULL;

        pList->dwCount = 0L;
        pList->pItem = NULL;
    }
}

/* Get the number of items in the given list */
uint_fast16_t MUSB_ListLength(const MUSB_LinkedList *pList)
{
    MUSB_LinkedList *pNext;

    uint_fast16_t wResult = 0;

    if (pList && pList->pItem)
    {
        pNext = pList->pNext;

        wResult++;
        while (pNext)
        {
            wResult++;
            pNext = pNext->pNext;
        }
    }

    return wResult;
}

/* Find the index'th item (counting from 0) in the given list */
void *MUSB_ListFindItem(MUSB_LinkedList *pList, uint_fast16_t wIndex)
{
    void *pItem = NULL;

    MUSB_LinkedList *pRecord = MUSB_ListFindRecord(pList, wIndex);

    if (pRecord)
    {
        pItem = pRecord->pItem;
    }

    return pItem;
}

/* Find the index'th record (counting from 0) in the given list */
MUSB_LinkedList *MUSB_ListFindRecord(MUSB_LinkedList *pList, uint_fast16_t wIndex)
{
    uint_fast16_t wCount = 0;

    MUSB_LinkedList *pResult = NULL;
    MUSB_LinkedList *pNext = pList;

    while ((wCount < wIndex) && pNext)
    {
        wCount++;
        pNext = pNext->pNext;
    }

    if (wCount == wIndex)
    {
        pResult = pNext;
    }

    return pResult;
}

static uint8_t MGC_ListInsertItem(MUSB_LinkedList *pPos, void *pItem, uint32_t dwCount)
{
    uint8_t bOK = FALSE;
    MUSB_LinkedList *pNext = NULL;

    //pNext = (MUSB_LinkedList *)MUSB_MemAlloc(sizeof(MUSB_LinkedList));
    pNext = MGC_AllocList();
    
    if (pPos && pNext)
    {
        bOK = TRUE;

        pNext->pNext = pPos->pNext;
        pPos->pNext = pNext;
        pNext->pItem = pItem;
        pNext->dwCount = dwCount;
    }

    return bOK;
}
/* Append an item to the end of the given list */
uint8_t MUSB_ListAppendItem(MUSB_LinkedList *pList, void *pItem, uint32_t dwCount)
{
    uint8_t bOK = FALSE;

    MUSB_LinkedList *pNext = pList;

    /* sanity */
    if (!pList)
    {
        return FALSE;
    }

    MUSB_ASSERT(pNext);

    /* check if empty */
    if (!pList->pItem)
    {
        bOK = TRUE;

        pList->pItem = pItem;
        pList->dwCount = dwCount;
        pList->pNext = NULL;
    }
    else
    {
        /* general case */
        while (pNext->pNext)
        {
            pNext = pNext->pNext;
        }

        MUSB_ASSERT(pNext);

        bOK = MGC_ListInsertItem(pNext, pItem, dwCount);
    }

    return bOK;
}

/* Insert an item at the given position in the given list */
#if 0 //unused
uint8_t MUSB_ListInsertItem(MUSB_LinkedList *pList, uint_fast16_t wItemIndex, void *pItem,
                            uint32_t dwCount)
{
    uint8_t bOK = FALSE;

    MUSB_LinkedList *pNext;

    /* sanity */
    if (!pList)
    {
        return FALSE;
    }

    /* check index */
    if (!wItemIndex)
    {
        /* special case for index 0 */        
        //pNext = (MUSB_LinkedList *)MUSB_MemAlloc(sizeof(MUSB_LinkedList));
        pNext = MGC_AllocList();
        
        if (pNext)
        {
            bOK = TRUE;

            pNext->dwCount = pList->dwCount;
            pNext->pItem = pList->pItem;
            pNext->pNext = pList->pNext;
            pList->dwCount = dwCount;
            pList->pItem = pItem;
            pList->pNext = pNext;
        }
    }
    else
    {
        /* general case */
        pNext = MUSB_ListFindRecord(pList, wItemIndex);
        if (pNext)
        {
            bOK = MGC_ListInsertItem(pNext, pItem, dwCount);
        }
    }

    return bOK;
}
#endif

/* Remove an item from the given list */
void MUSB_ListRemoveItem(MUSB_LinkedList *pList, const void *pItem)
{
    MUSB_LinkedList *pNext = pList;

    MUSB_LinkedList *pPos;

    /* sanity */
    if ((!pList) ||(!pItem))
    {
        return;
    }

    MUSB_ASSERT(pNext);
    
    /* special case for head */
    if (pList->pItem == pItem)
    {
        /* check if this will result in an empty list (since we never free the head) */
        pNext = pList->pNext;

        if (pNext)
        {
            /* non-empty; copy next record and free its storage */
            pList->pItem = pNext->pItem;

            pList->dwCount = pNext->dwCount;
            pList->pNext = pNext->pNext;
            VERIFY(MGC_FreeList(pNext));
        }
        else
        {
            /* now the list is empty (head item pointer is NULL) */
            pList->pItem = NULL;

            pList->dwCount = 0L;
        }
        return;
    }

    /* general case */
    while (pNext->pNext && (pNext->pNext->pItem != pItem))
    {
        pNext = pNext->pNext;
    }

    if ((pNext->pNext) && (pNext->pNext->pItem == pItem))
    {
        /* found it; unlink and free storage */
        pPos = pNext->pNext;
        /* check if we are removing last element */
        if (pPos->pNext)
        {
            /* not last element; link */
            pNext->pNext->pItem = pPos->pNext->pItem;

            pNext->pNext->dwCount = pPos->pNext->dwCount;
            pNext->pNext = pPos->pNext;
        }
        else
        {
            /* last element; clear link */
            pNext->pNext = NULL;
        }
        //MUSB_MemFree(pPos);
        VERIFY(MGC_FreeList(pPos));
    }
}
