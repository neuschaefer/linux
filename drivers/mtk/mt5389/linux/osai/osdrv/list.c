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
/*-----------------------------------------------------------------------------
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/


#include "x_list.h"
#include "x_assert.h"

//---------------------------------------------------------------------

static LIST_NODE_T* AllocateListNode(const LIST_T* pList)
{
    LIST_NODE_T* pNode;

    ASSERT(pList != NULL);

    pNode = (LIST_NODE_T*)pList->pfnAlloc(sizeof(LIST_NODE_T));
    if (pNode == NULL)
    {
        return NULL;
    }

    pNode->pNext = NULL;
    pNode->pPrev = NULL;
    pNode->pObj = NULL;

    return pNode;
}


static void FreeListNode(const LIST_T* pList, POSITION pos)
{
    ASSERT(pList != NULL);
    pList->pfnFree(pos);
}

//---------------------------------------------------------------------

BOOL ListInit(LIST_T* pList, PFN_MEM_ALLOC pfnAlloc, PFN_MEM_FREE pfnFree)
{
    if ((pList == NULL) || (pfnAlloc == NULL) || (pfnFree == NULL))
    {
        return FALSE;
    }

    pList->pHead = NULL;
    pList->pTail = NULL;
    pList->u4Count = 0;
    pList->pfnAlloc = pfnAlloc;
    pList->pfnFree = pfnFree;

    return TRUE;
}


void ListDestroy(LIST_T* pList)
{
    if (pList != NULL)
    {
        while (pList->pHead)
        {
            POSITION pos = pList->pHead;
            pList->pHead = pList->pHead->pNext;
            FreeListNode(pList, pos);
        }
    }
}

BOOL ListIsValidPosition(const LIST_T* pList, const POSITION pos)
{
    //lint --e{818} Pointer parameter 'pos' could be declared to const
    // Lint failed to recognize 'pos' as a const pointer in this case

    POSITION pos2;

    if ((pList == NULL) || (pos == NULL))
    {
        return FALSE;
    }

    pos2 = pList->pHead;
    while (pos2)
    {
        if (pos2 == pos)
        {
            break;
        }
        pos2 = pos2->pNext;
    }

    return pos2 != NULL;
}

BOOL ListAddHead(LIST_T* pList, void* pObj)
{
    LIST_NODE_T* pNode;

    ASSERT(pList != NULL);

    pNode = AllocateListNode(pList);
    if (pNode == NULL)
    {
        return FALSE;
    }

    if (pList->pHead == NULL)
    {
        // List is empty
        ASSERT(pList->pTail == NULL);
        pList->pHead = pNode;
        pList->pTail = pNode;
        pNode->pNext = NULL;
        pNode->pPrev = NULL;
    }
    else
    {
        pNode->pNext = pList->pHead;
        pNode->pPrev = NULL;
        pList->pHead->pPrev = pNode;
        pList->pHead = pNode;
    }
    pNode->pObj = pObj;
    pList->u4Count++;
    return TRUE;
}


BOOL ListAddTail(LIST_T* pList, void* pObj)
{
    LIST_NODE_T* pNode;

    ASSERT(pList != NULL);

    pNode = AllocateListNode(pList);
    if (pNode == NULL)
    {
        return FALSE;
    }

    if (pList->pTail == NULL)
    {
        // List is empty
        ASSERT(pList->pHead == NULL);
        pList->pHead = pNode;
        pList->pTail = pNode;
        pNode->pNext = NULL;
        pNode->pPrev = NULL;
    }
    else
    {
        pNode->pNext = NULL;
        pNode->pPrev = pList->pTail;
        pList->pTail->pNext = pNode;
        pList->pTail = pNode;
    }
    pNode->pObj = pObj;
    pList->u4Count++;
    return TRUE;
}


BOOL ListAddAt(LIST_T* pList, void* pObj, const POSITION pos)
{
    LIST_NODE_T* pNode;

    ASSERT(pList != NULL);
    ASSERT(pos != NULL);
    ASSERT(ListIsValidPosition(pList, pos));

    pNode = AllocateListNode(pList);
    if (pNode == NULL)
    {
        return FALSE;
    }

    if (pos == pList->pHead)
    {
        pList->pHead = pNode;
    }

    pNode->pPrev = pos->pPrev;
    pNode->pNext = pos;

    if (pos->pPrev != NULL)
    {
        pos->pPrev->pNext = pNode;
    }

    pos->pPrev = pNode;
    pNode->pObj = pObj;
    pList->u4Count++;
    return TRUE;
}


void* ListRemoveHead(LIST_T* pList)
{
    void* pObj = NULL;

    ASSERT(pList != NULL);

    if (pList->pHead)
    {
        LIST_NODE_T* pNode = pList->pHead;
        pObj = pNode->pObj;
        pList->pHead = pNode->pNext;

        if (pList->pHead)
        {
            pList->pHead->pPrev = NULL;
        }
        else
        {
            pList->pTail = NULL;        // List is empty
        }

        FreeListNode(pList, pNode);
        ASSERT(pList->u4Count > 0);
        pList->u4Count--;
    }

    return pObj;
}


void* ListRemoveTail(LIST_T* pList)
{
    void* pObj = NULL;

    ASSERT(pList != NULL);

    if (pList->pTail)
    {
        LIST_NODE_T* pNode = pList->pTail;
        pObj = pNode->pObj;
        pList->pTail = pNode->pPrev;

        if (pList->pTail)
        {
            pList->pTail->pNext = NULL;
        }
        else
        {
            pList->pHead = NULL;        // List is empty
        }

        FreeListNode(pList, pNode);
        ASSERT(pList->u4Count > 0);
        pList->u4Count--;
    }

    return pObj;
}


void* ListRemoveAt(LIST_T* pList, POSITION pos)
{
    void* pObj;
    POSITION pos2;

    ASSERT(pos != NULL);
    ASSERT(pList != NULL);
    ASSERT(ListIsValidPosition(pList, pos));

    pos2 = pos;

    if (pos == pList->pHead)
    {
        pList->pHead = pList->pHead->pNext;
        if (pList->pHead)
        {
            pList->pHead->pPrev = NULL;
        }
        else
        {
            pList->pTail = NULL;
        }
        pos = pList->pHead;
    }
    else if (pos == pList->pTail)
    {
        pList->pTail = pList->pTail->pPrev;
        if (pList->pTail)
        {
            pList->pTail->pNext = NULL;
        }
        else
        {
            pList->pHead = NULL;
        }
        pos = pList->pTail;
    }
    else
	{
	    pos->pPrev->pNext = pos->pNext;
	    pos->pNext->pPrev = pos->pPrev;
	    pos = pos->pNext;
	}

    pObj = pos2->pObj;
    FreeListNode(pList, pos2);
    ASSERT(pList->u4Count > 0);
    pList->u4Count--;
    return pObj;
}


BOOL ListRemoveAll(LIST_T* pList)
{
    ASSERT(pList != NULL);

    while (pList->pHead)
    {
        LIST_NODE_T* pNode = pList->pHead;
        pList->pHead = pList->pHead->pNext;
        FreeListNode(pList, pNode);
        pList->u4Count--;
    }

    ASSERT(pList->u4Count == 0);
    pList->pHead = NULL;
    pList->pTail = NULL;
    return TRUE;
}


UINT32 ListGetCount(const LIST_T* pList)
{
    ASSERT(pList != NULL);
    return pList->u4Count;
}


BOOL ListIsEmpty(const LIST_T* pList)
{
    ASSERT(pList != NULL);
    return pList->u4Count == 0;
}


POSITION ListGetHeadPosition(const LIST_T* pList)
{
    ASSERT(pList != NULL);
    return pList->pHead;
}


POSITION ListGetTailPosition(const LIST_T* pList)
{
    ASSERT(pList != NULL);
    return pList->pTail;
}


void* ListGetNext(const LIST_T* pList, POSITION* pPos)
{
    void* pObj;

    ASSERT(pList != NULL);
    ASSERT(pPos != NULL);
    ASSERT(ListIsValidPosition(pList, *pPos));
    pObj = (*pPos)->pObj;
    *pPos = (*pPos)->pNext;

    return pObj;
}


void* ListGetPrev(const LIST_T* pList, POSITION* pPos)
{
    void* pObj;

    ASSERT(pList != NULL);
    ASSERT(pPos != NULL);
    ASSERT(ListIsValidPosition(pList, *pPos));
    pObj = (*pPos)->pObj;
    *pPos = (*pPos)->pPrev;

    return pObj;
}


void* ListGetAt(const LIST_T* pList, const POSITION pos)
{
    ASSERT(pList != NULL);
    ASSERT(pos != NULL);
    ASSERT(ListIsValidPosition(pList, pos));
    return pos->pObj;
}


INT32 ListGetIndex(const LIST_T* pList, const POSITION pos)
{
    LIST_NODE_T* pNode;
    INT32 i4Index = -1;

    ASSERT(pList != NULL);
    ASSERT(pos != NULL);
    ASSERT(ListIsValidPosition(pList, pos));

    pNode = pList->pHead;
    while (pNode)
    {
        i4Index++;
        if (pos == pNode)
        {
            break;
        }
        pNode = pNode->pNext;
    }

    return i4Index;
}


POSITION ListGetPosition(const LIST_T* pList, UINT32 u4Index)
{
    POSITION pos;
    UINT32 i;

    ASSERT(pList != NULL);

    if (u4Index >= pList->u4Count)
    {
        return NULL;
    }

    pos = pList->pHead;
    for (i = 0; i < u4Index; i++)
    {
        if (pos == NULL)
        {
            return NULL;
        }
        pos = pos->pNext;
    }

    return pos;
}


POSITION ListFind(const LIST_T* pList, const void* pObj)
{
    POSITION pos;

    ASSERT(pList != NULL);

    pos = pList->pHead;
    while (pos)
    {
        if (pos->pObj == pObj)
        {
            return pos;
        }
        pos = pos->pNext;
    }

    return NULL;
}

