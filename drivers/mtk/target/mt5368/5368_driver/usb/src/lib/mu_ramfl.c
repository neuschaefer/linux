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
 * RAM-based file storage
 * $Revision: #1 $
 */

#include "mu_ramfl.h"

#include "mu_list.h"
#include "mu_mem.h"
#include "mu_strng.h"

static MUSB_LinkedList MGC_RamFileList;
static uint8_t MGC_bRamFileInit = FALSE;

static void MGC_RamFileCheck()
{
    if(!MGC_bRamFileInit)
    {
	MUSB_ListInit(&MGC_RamFileList);
	MGC_bRamFileInit = TRUE;
    }
}

/*
* implementation
*/
MUSB_RamFileInfo* MUSB_RamFileCreate(uint32_t dwSize, const CHAR* pszName)
{
    uint8_t* pBuffer;
    CHAR* pName;
    MUSB_RamFileInfo* pItem = NULL;

    MGC_RamFileCheck();
    pBuffer = (uint8_t*)MUSB_MemAlloc(dwSize);
    if(pBuffer)
    {
	pItem = (MUSB_RamFileInfo*)MUSB_MemAlloc(sizeof(MUSB_RamFileInfo));
	if(pItem)
	{
	    pItem->dwSize = dwSize;
	    pName = (CHAR*)&(pItem->szName[0]);
	    *pName = (CHAR)0;
	    MUSB_StringConcat(pName, MUSB_RAMFILE_MAX_NAME, pszName);
	    pItem->pData = pBuffer;
	    MUSB_ListAppendItem(&MGC_RamFileList, pItem, 0);
	}
	else
	{
	    MUSB_MemFree(pBuffer);
	    pBuffer = NULL;
	}
    }

    return pItem;
}

/*
* implementation
*/
uint8_t MUSB_RamFileDelete(const CHAR* pszName)
{
    MUSB_LinkedList* pEntry;
    MUSB_RamFileInfo* pItem;
    uint8_t bOk = FALSE;

    MGC_RamFileCheck();
    pEntry = &MGC_RamFileList;
    while(pEntry)
    {
	pItem = (MUSB_RamFileInfo*)pEntry->pItem;
	if(0 == MUSB_StringCompare(pszName, pItem->szName, FALSE))
	{
	    MUSB_ListRemoveItem(&MGC_RamFileList, pItem);
	    MUSB_MemFree(pItem->pData);
	    MUSB_MemFree(pItem);
	    break;
	}
	pEntry = pEntry->pNext;
    }

    return bOk;
}

/*
* implementation
*/
uint16_t MUSB_RamFileCount()
{
    MGC_RamFileCheck();
    return MUSB_ListLength(&MGC_RamFileList);
}

/*
* implementation
*/
const MUSB_RamFileInfo* MUSB_RamFileGetInfo(uint16_t wIndex)
{
    MGC_RamFileCheck();
    return (MUSB_RamFileInfo*)MUSB_ListFindItem(&MGC_RamFileList, wIndex);
}
