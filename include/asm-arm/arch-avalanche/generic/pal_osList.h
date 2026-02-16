/*
 *
 * pal_osList.h
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


/** \file   pal_osList.h
    \brief  OsLIST Services Header File

    This file declares APIs for handling simple bi-directional linked
    list that works with arbitrary data objects in a thread-safe manner.
    Movement of elements within the OsLIST is FIFO mode. 

    Only requirement on Elements strung onto the OsLIST is that they
    must begin with an OsLIST header comprising of a forward and reverse
    pointers. No Memory allocation or freeing is performed by these APIs.
    Memory allocation/freeing must be handled outside by the caller
    

    @author     PSP Architecture Team
    @version    1.0
 */

#ifndef __PAL_OSLIST_H__
#define __PAL_OSLIST_H__

#include "pal_defs.h"
#include "pal_os.h"

/**
 * \defgroup PalOSList PAL OS List Interface
 * 
 * PAL OS List Interface
 * \{
 */

/** \name PAL OS List Interface
 *  PAL OS List Interface
 * \{
 */

/**
 * \brief List Node Header
 * 
 * List node header used by all elements as the first member
 */
typedef struct 
{
    Ptr next;       /**< pointer to next node on the list */
    Ptr prev;       /**< pointer to previous node on the list */
} PAL_OsListNodeHeader;

/**
 * \def   PAL_OSLIST_MKNODE(hNode)
 * 
 *      This macro stitches link pointers of given node to
 *      reset conditions whereby the node can serve as
 *      seed for constructing a fresh list
 * \param   hNode is pointer to a arbitrary data object whose first
 *      member is of type PAL_OsListNodeHeader.
 * \warning This macro does NOT allocate memory for the new node.
 *      Caller must define the object elsewhere and only pass its
 *      pointer to the macro.
 */
#define PAL_OSLIST_MKNODE(hNode) do { \
    ((PAL_OsListNodeHeader*)hNode)->next = hNode; \
    ((PAL_OsListNodeHeader*)hNode)->prev = hNode; \
    } while (0)

/**
 * \brief   PAL OS List Append
 * 
 * This function "appends" the new node at tail end of list
 *
 * \param   hListHead is pointer to head of the list
 * \param   hNewNode is pointer to new element to be placed on the list
 * \return  nil return value
 */
PAL_INLINE void PAL_osListAppend(Ptr hListHead, Ptr hNewNode);

/**
 * \brief   PAL OS List Insert
 * 
 *      This function inserts the new node "after" the specified
 *      node on the list
 * \param   hListNode is pointer to node already on list "after" which
 *      new node is to be inserted
 * \param   hNewNode is pointer to new node to be placed on list
 * \return  nil return value
 */
PAL_INLINE void PAL_osListInsert(Ptr hListNode, Ptr hNewNode);

/**
 * \brief   PAL OS List Remove
 * 
 *      This function pulls out the specified node from the list.
 *      Rest of nodes will continue to remain normally on list
 * \param   hListNode is pointer of specific node to be removed
 * \warning No memory is freed up by this call
 * \return  Returns the pointer to the next node in the list.
 *          hListNode is returned if it is the only element in the list
 */
PAL_INLINE Ptr PAL_osListRemove(Ptr hListNode);

/**
 * \brief   PAL OS List Prepend
 * 
 *      This function prepends the specified node "ahead" of the
 *      current header node on the given list
 * \param   hListHead is pointer to head of the list
 * \param   hNewNode is pointer to new node to be placed at front
 *      of the current list
 * \return  Pointer to new head of list
 */
PAL_INLINE Ptr PAL_osListPrepend(Ptr hListHead, Ptr hNewNode);

/*\}*/
/*\}*/

#endif /* _PAL_OSLIST_H_ */
