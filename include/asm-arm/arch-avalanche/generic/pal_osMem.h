/*
 *
 * pal_osMem.h
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

/** \file   pal_osMem.h
    \brief  OsMEM Services Header File

    This file declares OS abstraction services for variable length heap
    memory management operations. The PAL module implementing these APIs
    is called OsMEM


    \author     PSP Architecture Team
    \version    1.0
 */

#ifndef __PAL_OSMEM_H__
#define __PAL_OSMEM_H__

#include "pal_defs.h"
#include "pal_os.h"

/**
 * \defgroup PALOSMem PAL OS Memory Interface
 *
 * PAL OS Memory Interface
 * @{
 */

/** \name PAL OS Memory Interface
 *  PAL OS Memory Interface
 * @{
 */

/**
 * \brief PAL OS MEM Init
 *
 * This is an idempotent function that must be called ahead of
 * calling any other OsMEM services. It initializes OsMEM internal
 * data structures (ex: segment tables) and does any book-keep
 * necessary to implement the published services of OsMEM.
 *
 * \param   param [IN] is an arbitrary void* data type used to pass platform
 * specific initialization information for OsMEM. This can be used
 * to extend OsMEM configurability to decisions made at run-time.
 * This is added for future extensions only.
 * \return  PAL_SOK if successful else a suitable error code.
 */
PAL_INLINE PAL_Result PAL_osMemInit(Ptr param);

/**
 * \brief PAL OS Memory Address Space
 *
 * Specifies type of memory address space
 */
typedef enum
{
    PAL_OSMEM_ADDR_PRG      = 0,    /**< Program only address space */
    PAL_OSMEM_ADDR_DAT      = 1,    /**< Data only address */
    PAL_OSMEM_ADDR_IO       = 2,    /**< I/O only space address */
    PAL_OSMEM_ADDR_PRGDAT   = 3,    /**< Unified prog-data */
    PAL_OSMEM_ADDR_PRGIO    = 4,    /**< Unified prog-io */
    PAL_OSMEM_ADDR_DATIO    = 5,    /**< Unified data-io */
    PAL_OSMEM_ADDR_UNIFIED  = 6,    /**< Homogeneous, unified prog/data/io memory */
    PAL_OSMEM_ADDR_SPECIAL  = 7     /**< Special or un-classified address range */
} PAL_OsMemAddrSpace;

/**
 * \brief PAL OS Memory attributes
 *
 * Memory attributes
 */
typedef struct
{
  PAL_OsMemAddrSpace addrSpace;
} PAL_OsMemAttrs;

/**
 * \brief PAL OS Memory Segment Define
 *
 * This function defines a segment of memory at specified
 * start address with given attributes. Once defined, user can allocate
 * variable length memory buffers at desired address alignments
 * via the PAL_osMemAlloc() call.
 *
 * \param   name [IN]       Name of memory segment being defined
 * \param   startAddr [IN]  The start address (byte granular) of memory segment
 * \param   numBytes [IN]   The length in bytes of the memory segment
 * \param   attrs [IN]      Optional memory attributes characterizing the
 *                          region of memory being defined. Ex: Program or
 *                          Data memory etc.,
 * \param   segId [OUT]     Location to recieve the numeric identifier of the
 *                          just defined memory segment.
 * \return  PAL_SOK if successful, else a suitable error code
 */
PAL_INLINE PAL_Result PAL_osMemSegDefine(const char* name,
                              Uint32 startAddr,
                              Uint32 numBytes,
                              PAL_OsMemAttrs *attrs,
                              Uint32 *segId);

/**
 * \brief PAL OS Memory Segment Undefine
 *
 * This function undefines specified segment of memory. Once
 * undefined, the region is no longer available for PAL_osMemAlloc().
 *
 * \param   segId [IN] Identifier of the memory segment being undefined.
 * \return  PAL_SOK if successful, else a suitable error code
 */
PAL_INLINE PAL_Result PAL_osMemSegUndefine(Uint32 segId);

/**
 * \brief PAL OS Memory Alloc
 *
 * This function allocates specified length of memory from
 * the given memory segment.
 * \note The memory segment must already be defined before calling this API.
 * \note Alignment = 0 or 1 results is NO special alignments being done. A
 * value of 2 forces the buffer to start at even byte boundary
 *
 * \param   segId [IN]      Identifier of the hosting memory segment
 * \param   numBytes [IN]   The length in bytes of buffer being allocated
 * \param   alignment [IN]  A power-of-2 alignment constraint specifier.
 *                          If non-zero, OsMEM will allocate specified length
 *                          of memory ensuring that it is aligned to an
 *                          address boundary as specified.
 * \param   memAddr [OUT]   Address of just allocated memory buffer
 * \return  PAL_SOK if successful, else a suitable error code
 */
PAL_INLINE PAL_Result PAL_osMemAlloc(Uint32 segId,
                          Uint32 numBytes,
                          Uint16 alignment,
                          Ptr *memAddr);

/**
 * \brief PAL OS Memory Free
 *
 * This function counters the PAL_osMemAlloc() call in that it frees
 * the specified memory buffer and returns it to the hosting memory
 * segment for others to allocate.
 * \note    The memory segment must already be defined before calling this API.
 *
 * \param   segId [IN]      Identifier of the hosting memory segment
 * \param   memAddr [IN]    The start address of memory buffer being freed.
 * \param   numBytes [IN]   The length in bytes of buffer being freed.
 * \return  PAL_SOK if successful, else a suitable error code
 */
PAL_INLINE PAL_Result PAL_osMemFree(Uint32 segId, Ptr memAddr, Uint32 numBytes);

/**
 * \brief PAL OS Memory Copy
 *
 * This function copies specified number of bytes from give source
 * address to given destination address
 * \note The user is responsible of making sure that adequate free memory
 * is indeed available at the specified destination address.
 *
 * \param   dest [IN/OUT]   Address of destination buffer
 * \param   src [IN]        Address of buffer to copy data from
 * \param   numBytes [IN]   The number of contigious bytes to copy
 * \return  PAL_SOK if successful, else a suitable error code
 */
PAL_INLINE PAL_Result PAL_osMemCopy(Ptr dest, const Ptr src, Uint32 numBytes);

/**
 * \brief PAL OS Memory Set
 *
 * This function stamps the specified region of memory with
 * the given bit pattern (fill character).
 *
 * \param   memAddr [IN/OUT]    Address of memory region to be stamped with
 *                              fill character
 * \param   fillVal [IN]        Address of buffer to copy data from
 * \param   numBytes [IN]       The number of contigious bytes to fill
 * \return  PAL_SOK if successful, else a suitable error code
 */
PAL_INLINE PAL_Result PAL_osMemSet(Ptr memAddr, Char fillVal, Uint32 numBytes);

/**
 * \brief PAL OS Memory Lock
 *
 * This function locks out the specified region of memory from
 * any page-swap operations effected by OS virtual memory manager.
 * Once locked, the region of memory will stay resident untill
 * its unlocked via PAL_osMemUnlock().
 *
 * \param   memAddr [IN]    Address of memory region to be locked from VM opserations
 * \param   byteLen [IN]    Length of memory region in bytes
 * \param   cookie [OUT]    Space to recieve an arbitrary platform specific data
 *                          associated with just performed locking. User is not
 *                          expected to interpret it any way. It is intended to
 *                          be passed as-is during matching unlocking operation
 *                          via PAL_osMemUnlock().
 * \return  PAL_SOK if successful, else a suitable error code
 */
PAL_INLINE PAL_Result PAL_osMemLock(Ptr memAddr,
                         Uint32 byteLen,
                         Uint32 *cookie);

/**
 * \brief PAL OS Memory Un-Lock
 *
 * This function counter to PAL_osMemLock() in that it unlocks the
 * specified region of memory thereby exposing it to any swaps
 * performed by OS virtual memory manager.
 *
 * \param   memAddr [IN]    Address of memory to be unlocked for VM operations
 * \param   byteLen [IN]    Length of memory region in bytes
 * \param   cookie [IN]     Platform specific data obtained when the specific
 *                          memory region was locked by a corresponding
 *                          PAL_osMemLock() function
 * \return  PAL_SOK if successful, else a suitable error code
 */
PAL_INLINE PAL_Result PAL_osMemUnlock(Ptr memAddr,
                           Uint32 byteLen,
                           Uint32 *cookie);
#ifdef CONFIG_ARM_AVALANCHE_SOC
/**
 * \brief PAL OS Memory Virtual To Physical
 *
 * This function is relevant in platforms where OS is running
 * a virtual memory manager. It inputs a logical or virtual
 * memory address (as seen by application) and returns the
 * corresponding physical memory address (as seen by the h/w
 * devices).
 * \note A macro equivalent of this API will also be available
 *
 * \param   virtAddress [IN] The virtual memory address
 * \return  A 32bit unsigned physical memory address.
 */
PAL_INLINE Uint32 PAL_osMemVirt2Phy(Ptr virtAddress);

/**
 * \brief PAL OS Memory Physical To Virtual
 *
 * This function is relevant in platforms where OS is running
 * a virtual memory manager. It inputs a physical memory
 * address (as seen by the h/w devices) and returns the corresponding
 * virtual memory address (as seen by the application).
 * \note A macro equivalent of this API will also be available
 *
 * \param   phyAddress [IN] The physical memory address
 * \return  Virtual memory address (unadorned void* type)
 */
PAL_INLINE Ptr PAL_osMemPhy2Virt(Uint32 phyAddress);

#endif
/**
 * \brief PAL OS Memory Report Attributes
 *
 * PAL OS Memory Report attributes
 */
typedef struct
{
  Int segBytesSz;           /**< Segment size in bytes */
  Int segBytesUsed;         /**< Number of bytes allocated */
  Int numBufs;              /**< Number of buffers allocated */
  Int maxLengthFree;        /**< Max contigious bytes free */
  Int maxLengthAllocated;   /**< Max contigious bytes allocated */
} PAL_OsMemReport;

/**
 * \brief PAL OS Memory Report
 *
 * This function reports assorted usage statistics information
 * regarding the specified memory segment
 *
 * \param   segId [IN]      Identifier of memory segment to report statistics
 * \param   report [IN/OUT] Location where information must be reported. If
 *                          NULL, structure is not filled.
 * \param   buf [IN/OUT]    String buffer where a text formatted report will
 *                          be printed. If NULL, no text reporting is done
 * \return  PAL_SOK if successful, else a suitable error code.
 */
PAL_INLINE PAL_Result PAL_osMemReport(Uint32 segId, PAL_OsMemReport *report, Char* buf);

/*@}*/
/*@}*/

#endif /* _PAL_OSMEM_H_ */
