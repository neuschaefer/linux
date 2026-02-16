/*
 *
 * pal_defs.h
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


/** \file   pal_defs.h
    \brief  Generic Platform Abstraction header file

    This file defines macros and data types generic across PAL services.


    \author     PSP Architecture Team
    \version    1.0
 */

#ifndef __PAL_DEFS_H__
#define __PAL_DEFS_H__

/* Import the TI standard primitive "C" types defines */
#include "_tistdtypes.h"
#ifdef CONFIG_ARM_AVALANCHE_SOC
#ifdef __KERNEL__
#include <linux/spinlock.h>
#endif
#endif

/**
 * \defgroup PALDefines PAL Defines
 *
 *  PAL Generic Defines - used by all modules using PAL services
 *  (including Drivers)
 */
/*@{*/

/* PAL Result - return value of a function  */
typedef Int             PAL_Result;

/* Type Macros */
#define PAL_False       ((Bool)0)
#define PAL_True        ((Bool)1)

/* General Macros */
#define PAL_MAX(a,b)    ((a) > (b) ? (a) : (b))
#define PAL_MIN(a,b)    ((a) < (b) ? (a) : (b))

/* Array Dimension */
#define PAL_DIM(array)  (sizeof(array)/sizeof(array[0]))

/* Endianness */

#define PAL_MK_UINT16(high8,low8)                               \
    ((Uint16)( ((Uint16)(high8) << 8) | (Uint16)(low8) ))

#define PAL_UINT16_LOW8(a)                                      \
    ((Uint8)((a) & 0x00FF))

#define PAL_UINT16_HIGH8(a)                                     \
    ((Uint8)(((a) >> 8) & 0x00FF))

#define PAL_MK_UINT32(high16,low16)                             \
    ((Uint32)( ((Uint32)(high16) << 16) | (Uint32)(low16) ))

#define PAL_MK_UINT32_FROM8S(high8,med_high8,med_low8,low8)     \
    PAL_MK_UINT32(PAL_MK_UINT16(high8,med_high8), PAL_MK_UINT16(med_low8, low8))

#define PAL_UINT32_LOW16(u32)                                   \
    ((Uint16)((u32) & 0xFFFF))

#define PAL_UINT32_HIGH16(u32)                                  \
    ((Uint16)(((u32) >> 16) & 0xFFFF))

#define PAL_UINT32_LOW8(u32)                                    \
    ((Uint8)((u32) & 0x00FF))

#define PAL_UINT32_MED_LOW8(u32)                                \
    ((Uint8)(((u32) >> 8) & 0xFF))

#define PAL_UINT32_MED_HIGH8(u32)                               \
    ((Uint8)(((u32) >> 16) & 0xFF))

#define PAL_UINT32_HIGH8(u32)                                   \
    ((Uint8)(((u32) >> 24) & 0xFF))

#define PAL_SWAP_UINT16(w)      \
    (PAL_MK_UINT16(PAL_UINT16_LOW8(w), PAL_UINT16_HIGH8(w)))

#define PAL_SWAP_UINT32(u32)                \
    (PAL_MK_UINT32_FROM8S(                  \
        PAL_UINT32_LOW8(u32),               \
        PAL_UINT32_MED_LOW8(u32),           \
        PAL_UINT32_MED_HIGH8(u32),          \
        PAL_UINT32_HIGH8(u32)))

/** Endian Utility Macros
 * PAL_UINT16_LE(w) converts a Little-Endian 16bit word to current endian word
 * PAL_UINT16_BE(w) converts a Big-Endian 16bit word to current endian word
 * PAL_UINT32_LE(d) converts a Little-Endian 32bit dword to current endian dword
 * PAL_UINT32_BE(d) converts a Big-Endian 32bit dword to current endian dword
 */

#ifdef PAL_NATIVE_ENDIAN_BIG
/* Native CPU accesses to memory locations are big-endian style */
#define PAL_UINT16_LE(w)    PAL_SWAP_UINT16(w)
#define PAL_UINT16_BE(w)    (w)
#define PAL_UINT32_LE(d)    PAL_SWAP_UINT32(d)
#define PAL_UINT32_BE(d)    (d)

#else
/* Native CPU accesses to memory locations are little-endian style */
#define PAL_UINT16_LE(w)    (w)
#define PAL_UINT16_BE(w)    PAL_SWAP_UINT16(w)
#define PAL_UINT32_LE(d)    (d)
#define PAL_UINT32_BE(d)    PAL_SWAP_UINT32(d)

#endif /* Endian switch */

/**
 * \defgroup PALErrorCodes PAL Error Codes
 *
 * PAL Error code bit fields follow a standard format. This format is used by
 * all PAL components, Services and Device Drivers.
 *
 * The following bitfield diagram depicts the PAL error code format:
 * \n
 * |<----------------32----------------->|
 * \n
 * |1(A)| 3(B) |  4(C) |    8(D)   |   8(E)  |   8(F)  |
 * - A - MSB - Set if Error / 0 if Success
 * - B - Error level - 0=Informational, 1=Warning, 2=Minor, 3=Major, 4=Critical
 * - C - PSP Architecture Component - 0=Reserved, 1=CSL, 2=Driver, 3=PAL, 4=SRV etc
 * - D - Device specific - eg Instance Id of DDC
 * - E,F - Error number - based upon implementation.
 */

/*@{*/

/** Error severity levels  */
#define PAL_INFO                (0)
#define PAL_WARNING             (1)
#define PAL_MINOR_ERROR         (2)
#define PAL_MAJOR_ERROR         (3)
#define PAL_CRITICAL_ERROR      (4)

/** PAL Error Sources (PSP Architectural Components) */
#define PAL_ERROR_SRC_CSL       (0)
#define PAL_ERROR_SRC_DRV       (1)
#define PAL_ERROR_SRC_PAL       (2)
#define PAL_ERROR_SRC_SRV       (3)

#define PAL_ERROR_FLAG          (0x80000000)    /**< PAL Error occured sentinel flag */

/** Successful Return Code for PAL_Result */

#define PAL_SOK                 (0x0)

/**
 * \note PAL Error bit manipulation masks and shift values
 */
#define PAL_ERROR_SEVERITY_SHIFT    (28)
#define PAL_ERROR_SEVERITY_MASK     (0x70000000)

#define PAL_ERROR_SRC_SHIFT         (24)
#define PAL_ERROR_SRC_MASK          (0x0F000000)

#define PAL_ERROR_QUAL_SHIFT        16
#define PAL_ERROR_QUAL_MASK         (0x00FF0000)

#define PAL_ERROR_NUM_SHIFT         (0)
#define PAL_ERROR_NUM_MASK          (0x0000FFFF)

/**
 * \brief PAL_ERROR() macro composes a final 32bit error code per the
 * above described format. It inputs the severity, source of error,
 * source qualifier and the specific error number of interest
 *
 * \sa PAL_ERROR_CSLSTATUS()
 */
#define PAL_ERROR(severity, src, qual, num) \
    ( PAL_ERROR_FLAG | \
      (PAL_ERROR_SEVERITY_MASK & (severity << PAL_ERROR_SEVERITY_SHIFT)) | \
    (PAL_ERROR_SRC_MASK & (src << PAL_ERROR_SRC_SHIFT)) | \
    (PAL_ERROR_QUAL_MASK & (qual << PAL_ERROR_QUAL_SHIFT)) | \
    (PAL_ERROR_NUM_MASK & (num << PAL_ERROR_NUM_SHIFT)))

/**
 * \brief The following 4 macros allow to extract relevant portions of
 * the 32bit PAL error code.
 */
#define PAL_ERROR_NUM(code)         ((code & PAL_ERROR_NUM_MASK) >> PAL_ERROR_NUM_SHIFT)
#define PAL_ERROR_QUAL(code)        ((code & PAL_ERROR_QUAL_MASK) >> PAL_ERROR_QUAL_SHIFT)
#define PAL_ERROR_SRC(code)         ((code & PAL_ERROR_SRC_MASK) >> PAL_ERROR_SRC_SHIFT)
#define PAL_ERROR_SEVERITY(code)    ((code & PAL_ERROR_SEVERITY_MASK) >> PAL_ERROR_SEVERITY_SHIFT)

/**
 * \brief   PAL_ERROR_CSLSTATUS() macros constructs a fully embodied
 *      32bit PAL error code, given a 16bit CSL_Status number
 * \note    CSL Errors are always ascribed severity level MAJOR.
 */
#define PAL_ERROR_CSLSTATUS(cslerr) \
    PAL_ERROR(PAL_MAJOR_ERROR, PAL_ERROR_SRC_CSL, 0, (cslerr & 0x7F))

/*@}*/
/*@}*/

#endif /* __PAL_DEFS_H__ */
