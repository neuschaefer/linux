/*
 *
 * pal_osList_inline.h
 * Description:
 * see below
 *
 *

  This file is provided under a dual BSD/GPLv2 license.  When using or 
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2008-2014 Intel Corporation.

  This program is free software; you can redistribute it and/or modify 
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but 
  WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
  General Public License for more details.

  You should have received a copy of the GNU General Public License 
  along with this program; if not, write to the Free Software 
  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
  The full GNU General Public License is included in this distribution 
  in the file called LICENSE.GPL.


  Contact Information:
  Intel Corporation
  2200 Mission College Blvd.
  Santa Clara, CA  97052

  BSD LICENSE 

  Copyright(c) 2008-2014 Intel Corporation. All rights reserved.

  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions 
  are met:

    * Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright 
      notice, this list of conditions and the following disclaimer in 
      the documentation and/or other materials provided with the 
      distribution.

    * Neither the name of Intel Corporation nor the names of its 
      contributors may be used to endorse or promote products derived 
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/** \file   pal_osList_inline.h
    \brief  OsLIST Services Header File

    This file defines APIs for handling simple bi-directional linked
    list that works with arbitrary data objects in a thread-safe manner.
    Movement of elements within the OsLIST is FIFO mode. 

    Only requirement on Elements strung onto the OsLIST is that they
    must begin with an OsLIST header comprising of a forward and reverse
    pointers. No Memory allocation or freeing is performed by these APIs.
    Memory allocation/freeing must be handled outside by the caller

    \note The current Linux implementation puts the onus on thead safety
    on the user of the List API
    
 
    @author     PSP Architecture Team
    @version    1.0
 */

#ifndef __PAL_OSLIST_INLINE_H__
#define __PAL_OSLIST_INLINE_H__

#include "pal_os.h"
#include "pal_defs.h"
#include "pal_osCfg.h"
/**
 * \brief   PAL OS List Append
 */

PAL_INLINE void PAL_osListAppend (Ptr head, Ptr newNode)
{
	Uint32 cookie;	
	PAL_OsListNodeHeader *headPtr = head;
	PAL_OsListNodeHeader *newNodePtr = newNode;

	/* Protect the list while updating */
	PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);
	
    newNodePtr->next = headPtr;
    newNodePtr->prev = headPtr->prev;
    ((PAL_OsListNodeHeader *)(headPtr->prev))->next = newNodePtr;
    headPtr->prev = newNodePtr;

	PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
}

/**
 * \brief   PAL OS List Insert
 */

PAL_INLINE void PAL_osListInsert (Ptr node, Ptr newNode)
{
	Uint32 cookie;			
	PAL_OsListNodeHeader *nodePtr = node;
	PAL_OsListNodeHeader *newNodePtr = newNode;
	
	/* Protect the list while updating */
	PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);
	
    newNodePtr->prev = nodePtr;
    newNodePtr->next = nodePtr->next;
    nodePtr->next = newNodePtr;
    ((PAL_OsListNodeHeader *)(newNodePtr->next))->prev = newNodePtr;
	
	PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);	
}

/**
 * \brief   PAL OS List Remove
 */

PAL_INLINE Ptr PAL_osListRemove(Ptr node)
{
	Uint32 cookie;					
    PAL_OsListNodeHeader *tmpNode;
    PAL_OsListNodeHeader *nodePtr = node;            
	
    if (nodePtr->next == nodePtr)
	{
        return nodePtr;
	}
    
	/* Protect the list while updating */
	PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);	
	
    tmpNode = nodePtr->next;
    ((PAL_OsListNodeHeader *)(nodePtr->prev))->next = nodePtr->next;
    ((PAL_OsListNodeHeader *)(nodePtr->next))->prev = nodePtr->prev;

	PAL_OSLIST_MKNODE(nodePtr);

	PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
	
    return tmpNode;
}

/**
 * \brief   PAL OS List Prepend
 */

PAL_INLINE Ptr PAL_osListPrepend (Ptr head, Ptr newNode)
{
	Uint32 cookie;							
    PAL_OsListNodeHeader *headPtr = head;
    PAL_OsListNodeHeader *newNodePtr = newNode;

    if(newNodePtr)
    {
			
		/* Protect the list while updating */
		PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);				
		
        if(!headPtr)
        {
            newNodePtr->next = newNodePtr->prev = newNodePtr;
        }
        else
        {
            newNodePtr->next = headPtr;
            newNodePtr->prev  = headPtr->prev;
            headPtr->prev = newNodePtr;
            ((PAL_OsListNodeHeader *)(newNodePtr->prev))->next = newNodePtr;
        }

       	PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
		
        return newNodePtr;
    }
    else 
    {
        return headPtr;
    }
}

/*\}*/
/*\}*/

#endif

