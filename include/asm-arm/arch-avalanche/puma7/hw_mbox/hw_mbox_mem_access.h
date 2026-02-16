/*
 *
 * hw_mbox_mem_access.h
 * Description:
 * contains memory access functions for the puma7 HW_MBOX controller
 * registers and SRAM.
 *

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2014-2016 Intel Corporation.

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

  Copyright(c) 2014-2016 Intel Corporation. All rights reserved.

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


#ifndef _HW_MBOX_MEM_ACCESS_H_
#define _HW_MBOX_MEM_ACCESS_H_

/************************************
|             INCLUDES:             |
************************************/

/* Include Always: */
#ifndef HW_MBOX_IS_APP_CPU
#include "hw_mbox_defs.h"          /* for HW_MBOX_MID, HW_MBOX_CONCAT and Generic types */
#else
#include <linux/hw_mbox_defs.h>    /* for HW_MBOX_MID, HW_MBOX_CONCAT and Generic types */
#endif

/* The following Included files are Determined Accordingly to OS and Master: */

#ifdef HW_MBOX_OS_LINUX
#ifdef __KERNEL__
/* Kernel: */
#include <linux/byteorder/generic.h>    /* Endianness: cpu_to_be32, be32_to_cpu */
#include <linux/string.h>               /* for memcpy\memset used in SRAM access */
#ifdef HW_MBOX_IS_NP_CPU
#include "puma7.h"    /* for NP_CPU Kernel HW_MBOX Base Address */
#endif /* HW_MBOX_IS_NP_CPU */
#else
/* User Space: */
#include "endian.h"     /* Endianness: htobe32, be32toh */
#endif /* __KERNEL__ */
#endif /* HW_MBOX_OS_LINUX */

#ifdef HW_MBOX_OS_FLAT
/* FLAT OS: */
#include "string.h"    /* for memcpy\memset used in SRAM access */
#if defined HW_MBOX_IS_PHY_CPU
#include "global_defines.h"    /* for base address of the HW_MBOX */
#endif
#endif /* HW_MBOX_OS_FLAT */

/***********************************************************
|                                                          |
|        HW_MBOX MEMORY ACCESS (Registers and SRAM)        |
|                                                          |
***********************************************************/

/* Implementing Memory Access Functionality for the HW_MBOX controller
   will be done in 5 steps:

   1. MAP HW_MBOX ADDRESS SPACE:
      Map the HW_MBOX address space (Offset that each Master should use for each register or SRAM).

   2. MAP HW_MBOX REGISTERS FIELDS:
      Map the Fields in the HW_MBOX registers (bit Offset and bit Length).

   3. DEFINE BASIC OPERATIONS:
      Define basic Macros to read and write 32 register values, handle Endianness
      and access specific bit fields.

   4. GENERIC LOW LEVEL MEM ACCESS MACRO TEMPLATES:
      Define low level generic Macros that enable simple operations: get/set reg32
      get/set field32 and set bit32 with a specific template.

      If direct access can be used and the base address is known in advance (i.e. Linux
      Kernel) then these will be defined here.

      Otherwise (Indirect access) matching name Macros must be defined in the file that
      can handle the access (i.e. hw_mbox_hal for Linux User Space application access).
      See section 4 for more details on what macros should be defined.

      SRAM access will also be defined here as it is relatively generic and very platform
      specific.

   5. SPECIFIC REGISTERS & FIELDS ACCESS MACROS:
      Define Get/Set Macros for all registers and Fields utilizing the generic Macros
      (set up at 4) and the basic operations (set up at 3). Using the generic Macros
      and basic operation enables to define these once.
*/


/*********************************************
|                                            |
|        1. MAP HW_MBOX ADDRESS SPACE        |
|                                            |
*********************************************/

/*
    Each Master has a different Address space to access to HW_MBOX memory (Registers and SRAM).
    The HW_MBOX determines which Master is Accessing it according to the address used. Using
    this information the HW_MBOX can enforce Access Protection.

    This section includes the calculation of the proper offsets to be used by every Master.

    Under normal conditions the MID is predetermined, therefore most of the calculations will
    be optimized under "constant folding". This excludes Macros that we want to receive a
    parameter such as the channel register offsets (one define that will work for all channels).

    Another exception will be the Debug feature "MID Disguise": Under this feature we add the
    option to change the MID on the fly, during runtime.

    There are 4 groups (types) of registers:
    1. Control Registers - contains general information regarding the HW_MBOX setup (shared among Masters).
    2. Master Registers - contains information relevant for each Master (one set per Master).
    3. Channel Registers - contains information relevant for each Channel (one set per Channel).
    4. Memory Map Registers - contains SRAM allocated to each Master (one register per Channel).

    The Address to be used can be Mapped using the Following table (relative to HW_MBOX Base Address):

                           +------------------------------------------------------------------------------------+
      USAGE          bits  | 16 | 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
    +----------------------+------------------------------------------------------------------------------------+
    | Control Registers    |  ACCESS MODIFIER  |  0 |  0 |  0 |  0 |  0 |  0 |  0 |  0 |  0 |  INTERNAL OFFSET  |
    +-----------------------------------------------------------------------------------------------------------+
    | Master Registers     |  ACCESS MODIFIER  |  0 |  0 |  1 |  MID SUBGROUP     |            INTERNAL OFFSET  |
    +-----------------------------------------------------------------------------------------------------------+
    | Channel Registers    |  ACCESS MODIFIER  |  0 |  1 |  0 |  CID SUBGROUP               |  INTERNAL OFFSET  |
    +-----------------------------------------------------------------------------------------------------------+
    | Memory Map Registers |  ACCESS MODIFIER  |  0 |  1 |  1 |  CID SUBGROUP               |  INTERNAL OFFSET  |
    +-----------------------------------------------------------------------------------------------------------+
    | SRAM                 |  ACCESS MODIFIER  |  1 |            SRAM OFFSET                                    |
    +-----------------------------------------------------------------------------------------------------------+

    ACCESS MODIFIER is the MID of the Master performing the Access to the HW_MBOX.
    SUBGROUP values are determined according to which Channel or Master we are observing (CID/MID).
    GROUP_OFFSET (denoted later on) are the value of bits [12:10].
    SRAM_INTERNAL_OFFSET (denoted later on) is (bit(12) = 1) for SRAM.
*/

#ifdef HW_MBOX_DEBUG
/* MID Disguise feature:

   Under Debug we have the ability to change the MID of the current Master on the fly, during runtime.
   This effects: <ACCESS MODIFIER> for all access and <MID SUBGROUP> when accessing our own Master registers.
*/

#undef HW_MBOX_MID                            /* Undefine the Hardcoded original value */
#define HW_MBOX_MID ( __debug_HW_MBOX_MID )   /* redefine as a global variable [so will be able to change during runtime] */

/*! \var extern volatile Uint32 __debug_HW_MBOX_MID
    \brief changeable MID field for Debug.
    \attention set by using __HW_MBOX_DEBUG_SET_MID()
*/
extern volatile Uint32 __debug_HW_MBOX_MID;

/*! \def __HW_MBOX_DEBUG_SET_MID(newMID)
    \brief set Debug MID to <newMID>.
    \warning Must Use a Valid MID!
*/
#define __HW_MBOX_DEBUG_SET_MID(newMID)  ( __debug_HW_MBOX_MID = (newMID) )
#endif /* HW_MBOX_DEBUG */

/* --- ACCESS MODIFIER (determines address space for this MID, relative to HW_MBOX BASE) --- */
#define HW_MBOX_MEMDEFS_ACCESS_MODIFIER(paramMID)   ((paramMID) << 13)    /*!   ((paramMID) * 0x2000)   */

/* --- GROUP OFFSETS (Relative to this MIDs Address Space) --- */
#define HW_MBOX_REGDEFS_CONTROL_GROUP_OFFSET       (0x0)    /*!  bits[12:10] = [0 0 0]  */
#define HW_MBOX_REGDEFS_MASTER_GROUP_OFFSET        (0x400)  /*!  bits[12:10] = [0 0 1]  */
#define HW_MBOX_REGDEFS_CHANNEL_GROUP_OFFSET       (0x800)  /*!  bits[12:10] = [0 1 0]  */
#define HW_MBOX_REGDEFS_MEMORY_MAP_GROUP_OFFSET    (0xC00)  /*!  bits[12:10] = [0 1 1]  */

/* --- SUBGROUP OFFSETS (Relative to Group Offset) --- */
#define HW_MBOX_REGDEFS_MID_SUBGROUP_OFFSET(paramMID)     ((paramMID) << 6)    /*!    ((paramMID) * 0x40)    */
#define HW_MBOX_REGDEFS_CID_SUBGROUP_OFFSET(paramCID)     ((paramCID) << 4)    /*!    ((paramCID) * 0x10)    */
#define HW_MBOX_REGDEFS_MMAP_SUBGROUP_OFFSET(paramCID)    ((paramCID) << 4)    /*!    ((paramCID) * 0x10)    */


/* --- CONTROL REGISTER OFFSETS (Relative to Group Offset) --- */
#define HW_MBOX_REGDEFS_ID_INTERNAL_OFFSET       (0x0)
#define HW_MBOX_REGDEFS_CTRL_INTERNAL_OFFSET     (0x4)
#define HW_MBOX_REGDEFS_ERROR_INTERNAL_OFFSET    (0x8)

/* ---MASTER REGISTER OFFSETS (Relative to Subgroup Offset) --- */
#define HW_MBOX_REGDEFS_MASTER_INTR_STATUS_L_INTERNAL_OFFSET    (0x0)
#define HW_MBOX_REGDEFS_MASTER_INTR_STATUS_H_INTERNAL_OFFSET    (0x4)
#define HW_MBOX_REGDEFS_MASTER_INTR_MASK_L_INTERNAL_OFFSET      (0x8)
#define HW_MBOX_REGDEFS_MASTER_INTR_MASK_H_INTERNAL_OFFSET      (0xC)
#define HW_MBOX_REGDEFS_MASTER_INTR_CLEAR_L_INTERNAL_OFFSET     (0x10)
#define HW_MBOX_REGDEFS_MASTER_INTR_CLEAR_H_INTERNAL_OFFSET     (0x14)
#define HW_MBOX_REGDEFS_MASTER_COMMANDER_L_INTERNAL_OFFSET      (0x18)
#define HW_MBOX_REGDEFS_MASTER_COMMANDER_H_INTERNAL_OFFSET      (0x1C)
#define HW_MBOX_REGDEFS_MASTER_SUBJECT_L_INTERNAL_OFFSET        (0x20)
#define HW_MBOX_REGDEFS_MASTER_SUBJECT_H_INTERNAL_OFFSET        (0x24)

/* --- Channel Register Offsets (Relative to Group Offset) --- */
#define HW_MBOX_REGDEFS_CHN_CFG_INTERNAL_OFFSET        (0x0)
#define HW_MBOX_REGDEFS_CHN_COMMAND_INTERNAL_OFFSET    (0x4)
#define HW_MBOX_REGDEFS_CHN_STATE_INTERNAL_OFFSET      (0x8)

/* --- Memory Map Register Offsets (Relative to Group Offset) --- */
#define HW_MBOX_REGDEFS_MEMORY_MAP_INTERNAL_OFFSET    (0x0)

/******************************************
|       Register Offset Calculation       |
******************************************/

/* Now to do the actual calculation of the Offsets:

   OFFSET = ACCESS_MODIFIER + GROUP_OFFSET + [Optional] SUBGROUP_OFFSET + INTERNAL_OFFSET

   ACCESS_MODIFIER: According to current MID, translates the request to specific Master's Address Space
                    [To mark which Master is accessing to HW_MBOX HW (so Access Protection can be enforced)].

   GROUP_OFFSET: According to register group, relative to this MIDs Address Space.

   SUBGROUP_OFFSET: (optional) only used when there are a few subgroup inside a group
                    e.g. A set of 3 channel registers, per every one of the 64 channel.

   INTERNAL_OFFSET: According to the specific register, relative to group (or subgroup if it exists).

   Please note that we will use a constant naming template for all #defines:
   register offset: #define HW_MBOX_<regName>_OFFSET

    * regName should always start with REGDEFS_

*/

/* --- General Registers --- */
#define HW_MBOX_REGDEFS_ID_OFFSET    ( HW_MBOX_MEMDEFS_ACCESS_MODIFIER(HW_MBOX_MID) + HW_MBOX_REGDEFS_CONTROL_GROUP_OFFSET + HW_MBOX_REGDEFS_ID_INTERNAL_OFFSET )
#define HW_MBOX_REGDEFS_CTRL_OFFSET  ( HW_MBOX_MEMDEFS_ACCESS_MODIFIER(HW_MBOX_MID) + HW_MBOX_REGDEFS_CONTROL_GROUP_OFFSET + HW_MBOX_REGDEFS_CTRL_INTERNAL_OFFSET )
#define HW_MBOX_REGDEFS_ERROR_OFFSET ( HW_MBOX_MEMDEFS_ACCESS_MODIFIER(HW_MBOX_MID) + HW_MBOX_REGDEFS_CONTROL_GROUP_OFFSET + HW_MBOX_REGDEFS_ERROR_INTERNAL_OFFSET )

/* --- Master Registers --- */
#define HW_MBOX_REGDEFS_MID_MASTER_REGS_OFFSET(paramMID)    ( HW_MBOX_MEMDEFS_ACCESS_MODIFIER(paramMID) + HW_MBOX_REGDEFS_MASTER_GROUP_OFFSET + HW_MBOX_REGDEFS_MID_SUBGROUP_OFFSET(paramMID) )

#define HW_MBOX_REGDEFS_MASTER_INTR_STATUS_L_OFFSET         ( HW_MBOX_REGDEFS_MID_MASTER_REGS_OFFSET(HW_MBOX_MID) + HW_MBOX_REGDEFS_MASTER_INTR_STATUS_L_INTERNAL_OFFSET )
#define HW_MBOX_REGDEFS_MASTER_INTR_STATUS_H_OFFSET         ( HW_MBOX_REGDEFS_MID_MASTER_REGS_OFFSET(HW_MBOX_MID) + HW_MBOX_REGDEFS_MASTER_INTR_STATUS_H_INTERNAL_OFFSET )
#define HW_MBOX_REGDEFS_MASTER_INTR_MASK_L_OFFSET           ( HW_MBOX_REGDEFS_MID_MASTER_REGS_OFFSET(HW_MBOX_MID) + HW_MBOX_REGDEFS_MASTER_INTR_MASK_L_INTERNAL_OFFSET )
#define HW_MBOX_REGDEFS_MASTER_INTR_MASK_H_OFFSET           ( HW_MBOX_REGDEFS_MID_MASTER_REGS_OFFSET(HW_MBOX_MID) + HW_MBOX_REGDEFS_MASTER_INTR_MASK_H_INTERNAL_OFFSET )
#define HW_MBOX_REGDEFS_MASTER_INTR_CLEAR_L_OFFSET          ( HW_MBOX_REGDEFS_MID_MASTER_REGS_OFFSET(HW_MBOX_MID) + HW_MBOX_REGDEFS_MASTER_INTR_CLEAR_L_INTERNAL_OFFSET )
#define HW_MBOX_REGDEFS_MASTER_INTR_CLEAR_H_OFFSET          ( HW_MBOX_REGDEFS_MID_MASTER_REGS_OFFSET(HW_MBOX_MID) + HW_MBOX_REGDEFS_MASTER_INTR_CLEAR_H_INTERNAL_OFFSET )
#define HW_MBOX_REGDEFS_MASTER_COMMANDER_L_OFFSET           ( HW_MBOX_REGDEFS_MID_MASTER_REGS_OFFSET(HW_MBOX_MID) + HW_MBOX_REGDEFS_MASTER_COMMANDER_L_INTERNAL_OFFSET )
#define HW_MBOX_REGDEFS_MASTER_COMMANDER_H_OFFSET           ( HW_MBOX_REGDEFS_MID_MASTER_REGS_OFFSET(HW_MBOX_MID) + HW_MBOX_REGDEFS_MASTER_COMMANDER_H_INTERNAL_OFFSET )
#define HW_MBOX_REGDEFS_MASTER_SUBJECT_L_OFFSET             ( HW_MBOX_REGDEFS_MID_MASTER_REGS_OFFSET(HW_MBOX_MID) + HW_MBOX_REGDEFS_MASTER_SUBJECT_L_INTERNAL_OFFSET )
#define HW_MBOX_REGDEFS_MASTER_SUBJECT_H_OFFSET             ( HW_MBOX_REGDEFS_MID_MASTER_REGS_OFFSET(HW_MBOX_MID) + HW_MBOX_REGDEFS_MASTER_SUBJECT_H_INTERNAL_OFFSET )

/* --- Channel Registers --- */
#define HW_MBOX_REGDEFS_MID_CHN_REGS_OFFSET(paramMID, paramCID)     ( HW_MBOX_MEMDEFS_ACCESS_MODIFIER(paramMID) + HW_MBOX_REGDEFS_CHANNEL_GROUP_OFFSET + HW_MBOX_REGDEFS_CID_SUBGROUP_OFFSET(paramCID) )

#define HW_MBOX_REGDEFS_CHN_CFG_OFFSET(paramCID)                    ( HW_MBOX_REGDEFS_MID_CHN_REGS_OFFSET(HW_MBOX_MID, paramCID) + HW_MBOX_REGDEFS_CHN_CFG_INTERNAL_OFFSET )
#define HW_MBOX_REGDEFS_CHN_COMMAND_OFFSET(paramCID)                ( HW_MBOX_REGDEFS_MID_CHN_REGS_OFFSET(HW_MBOX_MID, paramCID) + HW_MBOX_REGDEFS_CHN_COMMAND_INTERNAL_OFFSET )
#define HW_MBOX_REGDEFS_CHN_STATE_OFFSET(paramCID)                  ( HW_MBOX_REGDEFS_MID_CHN_REGS_OFFSET(HW_MBOX_MID, paramCID) + HW_MBOX_REGDEFS_CHN_STATE_INTERNAL_OFFSET )

/* --- Memory Map Registers --- */
#define HW_MBOX_REGDEFS_MEMORY_MAP_OFFSET(paramCID) ( HW_MBOX_MEMDEFS_ACCESS_MODIFIER(HW_MBOX_MID) + HW_MBOX_REGDEFS_MEMORY_MAP_GROUP_OFFSET + HW_MBOX_REGDEFS_MMAP_SUBGROUP_OFFSET(paramCID) + HW_MBOX_REGDEFS_MEMORY_MAP_INTERNAL_OFFSET )

/**************************************************
|       Parametric Master Registers Mapping       |
**************************************************/

/* These are defined for the purpose of accessing other Master's Master Registers.
   Generally speaking each Master should only involve itself with its own Master Registers.
   Still we should map these as we might need to read their values for research purposes.
   We avoid mapping the interrupt status registers as access to them will trigger write
   after read which will result in an access violation.

   NOTE: The access is done from the MID associated with the current Master!
*/

#define HW_MBOX_REGDEFS_MASTER_PARAM_INTR_MASK_L_OFFSET(paramMID)   ( HW_MBOX_MEMDEFS_ACCESS_MODIFIER(HW_MBOX_MID) + HW_MBOX_REGDEFS_MASTER_GROUP_OFFSET + HW_MBOX_REGDEFS_MID_SUBGROUP_OFFSET(paramMID) + HW_MBOX_REGDEFS_MASTER_INTR_MASK_L_INTERNAL_OFFSET )
#define HW_MBOX_REGDEFS_MASTER_PARAM_INTR_MASK_H_OFFSET(paramMID)   ( HW_MBOX_MEMDEFS_ACCESS_MODIFIER(HW_MBOX_MID) + HW_MBOX_REGDEFS_MASTER_GROUP_OFFSET + HW_MBOX_REGDEFS_MID_SUBGROUP_OFFSET(paramMID) + HW_MBOX_REGDEFS_MASTER_INTR_MASK_H_INTERNAL_OFFSET )
#define HW_MBOX_REGDEFS_MASTER_PARAM_INTR_CLEAR_L_OFFSET(paramMID)  ( HW_MBOX_MEMDEFS_ACCESS_MODIFIER(HW_MBOX_MID) + HW_MBOX_REGDEFS_MASTER_GROUP_OFFSET + HW_MBOX_REGDEFS_MID_SUBGROUP_OFFSET(paramMID) + HW_MBOX_REGDEFS_MASTER_INTR_CLEAR_L_INTERNAL_OFFSET )
#define HW_MBOX_REGDEFS_MASTER_PARAM_INTR_CLEAR_H_OFFSET(paramMID)  ( HW_MBOX_MEMDEFS_ACCESS_MODIFIER(HW_MBOX_MID) + HW_MBOX_REGDEFS_MASTER_GROUP_OFFSET + HW_MBOX_REGDEFS_MID_SUBGROUP_OFFSET(paramMID) + HW_MBOX_REGDEFS_MASTER_INTR_CLEAR_H_INTERNAL_OFFSET )
#define HW_MBOX_REGDEFS_MASTER_PARAM_COMMANDER_L_OFFSET(paramMID)   ( HW_MBOX_MEMDEFS_ACCESS_MODIFIER(HW_MBOX_MID) + HW_MBOX_REGDEFS_MASTER_GROUP_OFFSET + HW_MBOX_REGDEFS_MID_SUBGROUP_OFFSET(paramMID) + HW_MBOX_REGDEFS_MASTER_COMMANDER_L_INTERNAL_OFFSET )
#define HW_MBOX_REGDEFS_MASTER_PARAM_COMMANDER_H_OFFSET(paramMID)   ( HW_MBOX_MEMDEFS_ACCESS_MODIFIER(HW_MBOX_MID) + HW_MBOX_REGDEFS_MASTER_GROUP_OFFSET + HW_MBOX_REGDEFS_MID_SUBGROUP_OFFSET(paramMID) + HW_MBOX_REGDEFS_MASTER_COMMANDER_H_INTERNAL_OFFSET )
#define HW_MBOX_REGDEFS_MASTER_PARAM_SUBJECT_L_OFFSET(paramMID)     ( HW_MBOX_MEMDEFS_ACCESS_MODIFIER(HW_MBOX_MID) + HW_MBOX_REGDEFS_MASTER_GROUP_OFFSET + HW_MBOX_REGDEFS_MID_SUBGROUP_OFFSET(paramMID) + HW_MBOX_REGDEFS_MASTER_SUBJECT_L_INTERNAL_OFFSET )
#define HW_MBOX_REGDEFS_MASTER_PARAM_SUBJECT_H_OFFSET(paramMID)     ( HW_MBOX_MEMDEFS_ACCESS_MODIFIER(HW_MBOX_MID) + HW_MBOX_REGDEFS_MASTER_GROUP_OFFSET + HW_MBOX_REGDEFS_MID_SUBGROUP_OFFSET(paramMID) + HW_MBOX_REGDEFS_MASTER_SUBJECT_H_INTERNAL_OFFSET )

/*******************************************
|       HW_MBOX SRAM Address Mapping       |
*******************************************/

#define HW_MBOX_MEMDEFS_SRAM_INTERNAL_OFFSET    (0x1000)  /*!  bit(12) = 1  */

/*! \def HW_MBOX_MEMDEFS_MID_SRAM_OFFSET(internalByteOffset)
    \brief Get HW_MBOX SRAM offset relative to HW_MBOX Base Address with paramMID as access modifier.
    \param[in] paramMID - access modifier master id.
    \param[in] ibo - offset in Bytes with regards to the SRAM's 4KB total actual size.
*/
#define HW_MBOX_MEMDEFS_MID_SRAM_OFFSET(paramMID, ibo)    ( HW_MBOX_MEMDEFS_ACCESS_MODIFIER(paramMID) + HW_MBOX_MEMDEFS_SRAM_INTERNAL_OFFSET + (ibo) )

/*! \def HW_MBOX_MEMDEFS_SRAM_OFFSET(internalByteOffset)
    \brief Get HW_MBOX SRAM offset relative to HW_MBOX Base Address.
    \param[in] internalByteOffset - offset in Bytes with regards to the SRAM's 4KB total actual size.
*/
#define HW_MBOX_MEMDEFS_SRAM_OFFSET(internalByteOffset)   ( HW_MBOX_MEMDEFS_MID_SRAM_OFFSET(HW_MBOX_MID, (internalByteOffset)) )

#define HW_MBOX_SRAM_SIZE    (0x1000)  /*! 4KB SRAM (total actual size) */

/**********************************************
|       HW_MBOX Special Address Mapping       |
**********************************************/

/*! \def HW_MBOX_REGDEFS_CTRL_AS_DEFAULT_SUPERVISOR_OFFSET
    \brief Use this offset to change supervisor at initialization.
    \note This offset accesses the CTRL register as the default supervisor (NP_CPU).
    \note This is needed as access to this offset from another Master will trigger access violation.
*/
#define HW_MBOX_REGDEFS_CTRL_AS_DEFAULT_SUPERVISOR_OFFSET ( HW_MBOX_MEMDEFS_ACCESS_MODIFIER(HW_MBOX_MASTER_NP_CPU) + HW_MBOX_REGDEFS_CONTROL_GROUP_OFFSET + HW_MBOX_REGDEFS_CTRL_INTERNAL_OFFSET )


/************************************************
|                                               |
|        2. MAP HW_MBOX REGISTERS FIELDS        |
|                                               |
************************************************/

/* This Section will list all the HW_MBOX registers structure, followed
   by a declaration (via #define) of the bit offset and bit length of each
   field.

   Please note that we will use a constant naming template for all #defines:
   field bit offset: #define HW_MBOX_<fieldName>_OFFSET
   field bit length: #define HW_MBOX_<fieldName>_LENGTH

    * fieldName should always start with FLDDEFS_
*/

/********************************
|       CONTROL REGISTERS       |
********************************/

/* These registers contain general control data and settings of the HW_MBOX. */

/* --------------------------------------------------------------------------- */
/*  HW_MBOX_REG_ID
    +---------------------------------------------------------------------------+
    |   MAJOR   |   MINOR   |   RESERVED                                        |
    |   31:28   |   27:24   |   23:0                                            |
    |   RO      |   RO      |   RO                                              |
    +---------------------------------------------------------------------------/

    MAJOR - Identifies core version (Major).
    MINOR - Identifies core version (Minor).
*/

#define HW_MBOX_FLDDEFS_MAJOR_OFFSET (28)
#define HW_MBOX_FLDDEFS_MAJOR_LENGTH (4)

#define HW_MBOX_FLDDEFS_MINOR_OFFSET (24)
#define HW_MBOX_FLDDEFS_MINOR_LENGTH (4)

/* --------------------------------------------------------------------------- */
/*  HW_MBOX_REG_CTRL
    +---------------------------------------------------------------------------+
    |   RESERVED      | MON_EXIST | MONITOR | LOCK | DEBUG | START | SUPERVISOR |
    |   31:12         | 11        | 10:7    | 6    | 5     | 4     | 3:0        |
    |   RO            | RW        | RW      | RWO  | RW    | RW    | RW         |
    +---------------------------------------------------------------------------/

    NOTE: only the supervisor should write to this register (preferably) until
          setting START=TRUE. Once setting LOCK=TRUE the register is RO.

    MON_EXIST - 0 indicates that there is no Monitor.
                1 indicates that a monitor exists.

    MONITOR - MID of the Monitor Master. This field is relevant only if MON_EXIST=1.

    LOCK - 1 indicates that the MBOX is Locked. The Supervisor can set this bit.
           Once set, the register is RO.

    DEBUG - 1 indicates that the MBOX is in debug mode, enabling any Master
            to access MBOX registers without limitations.

    START - 1 indicates that the MBOX is configured and functional.
            0 indicates that the MBOX is not configured yet, and the Channels cannot
              be used by the Masters.

    SUPERVISOR - MID of the Supervisor.
*/

#define HW_MBOX_FLDDEFS_MON_EXIST_OFFSET (11)
#define HW_MBOX_FLDDEFS_MON_EXIST_LENGTH (1)

#define HW_MBOX_FLDDEFS_MONITOR_OFFSET (7)
#define HW_MBOX_FLDDEFS_MONITOR_LENGTH (4)

#define HW_MBOX_FLDDEFS_LOCK_OFFSET (6)
#define HW_MBOX_FLDDEFS_LOCK_LENGTH (1)

#define HW_MBOX_FLDDEFS_DEBUG_OFFSET (5)
#define HW_MBOX_FLDDEFS_DEBUG_LENGTH (1)

/* Possible conflict as there are 2 fields named START:
   One for the CTRL register(Here) and one in the Memory Map Registers...
   Will not conflict as this is the real START, for mmap regs we will add
   some prefix to make it more understandable. */

#define HW_MBOX_FLDDEFS_START_OFFSET (4)
#define HW_MBOX_FLDDEFS_START_LENGTH (1)

#define HW_MBOX_FLDDEFS_SUPERVISOR_OFFSET (0)
#define HW_MBOX_FLDDEFS_SUPERVISOR_LENGTH (4)

/* --------------------------------------------------------------------------- */
/*  HW_MBOX_REG_ERROR
    +---------------------------------------------------------------------------+
    |   RESERVED               | COUNT                |  RESERVED  |  ERR       |
    |   31:16                  | 15:8                 |  7:4       |  3:0       |
    |   RO                     | RW                   |  RO        |  RO        |
    +---------------------------------------------------------------------------/

    NOTE: only the supervisor has write access to this register.

    COUNT - Incremented by one upon error detection.
            Halts when reached the max value (255).
            Supervisor can clear it by writing 0.

    ERR - The value of the MID of the last HW_MBOX access which violated
          the protection rules. By default ERR=0.
*/

/* Adding the word ERROR even though the Field
   is called COUNT to make it more understandable. */
#define HW_MBOX_FLDDEFS_ERROR_COUNT_OFFSET  (8)
#define HW_MBOX_FLDDEFS_ERROR_COUNT_LENGTH  (8)

/* Adding the word MID even though the Field
   is called ERR to make it more understandable. */
#define HW_MBOX_FLDDEFS_ERRMID_OFFSET  (0)
#define HW_MBOX_FLDDEFS_ERRMID_LENGTH  (4)

/*******************************
|       MASTER REGISTERS       |
*******************************/

/* These Registers contain data related to the Masters. There's a set of these registers
   for each MASTER.

   These Registers do not contain fields per say, but rather each pair (*_L and *_H) of
   them is divided into 64 one bit fields. Each field signifies the Channel with a CID
   corresponding to the position of the set bit [CID = (is *_H reg)*32 + (bit position)].
   Thus, there's no need to define specific fields here.

*/

/* --------------------------------------------------------------------------- */
/*  HW_MBOX_REG_MASTER_INTR_STATUS_(L or H)
    +---------------------------------------------------------------------------+
    |   STATUS                                                                  |
    |   31:0                                                                    |
    |   RO                                                                      |
    +---------------------------------------------------------------------------/

    These Registers indicate which channel had an interrupt for this MASTER (according
    to which bits are set).

    NOTE: a read of these registers is always followed by a write to clear the set bits.
          Once read the software must handle the Interrupts otherwise they'll be lost.
          Also since there's a write after read it must comply with the access protection
          rules.

*/

/* --------------------------------------------------------------------------- */
/*  HW_MBOX_REG_MASTER_INTR_MASK_(L or H)
    +---------------------------------------------------------------------------+
    |   MASK                                                                    |
    |   31:0                                                                    |
    |   RW                                                                      |
    +---------------------------------------------------------------------------/

    These Registers enable to Mask channels from triggering an interrupt for this
    MASTER (according to which bits are set).

    NOTE: a common use of these will be masking a channel in order to use polling.
*/

/* --------------------------------------------------------------------------- */
/*  HW_MBOX_REG_MASTER_INTR_CLEAR_(L or H)
    +---------------------------------------------------------------------------+
    |   CLEAR                                                                   |
    |   31:0                                                                    |
    |   RW                                                                      |
    +---------------------------------------------------------------------------/

    These are Shadow registers of the interrupt status ones. Can be used for probing
    the Interrupt status registers without triggering the write after read. These
    are also used when polling is desired.

    NOTE: After consuming an interrupt when using polling we need to clear the corresponding
          Interrupt bit from the interrupt status register. This is done by writing the bit
          pattern of the consumed interrupts to this register (*_L or *_H accordingly).

          Meaning:
          Upon write of bit pattern (PATTERN) to this register [X is L or H]-
          HW_MBOX_MASTER_INTR_STATUS_X is changed to ((~PATTERN) & HW_MBOX_MASTER_INTR_STATUS_X).
*/

/* --------------------------------------------------------------------------- */
/*  HW_MBOX_REG_MASTER_COMMANDER_(L or H)
    +---------------------------------------------------------------------------+
    |   SET                                                                     |
    |   31:0                                                                    |
    |   RO                                                                      |
    +---------------------------------------------------------------------------/

    These registers indicates in which channels this MASTER is configured as COMMANDER
    (as is configured in the channel configuration registers).
*/

/* --------------------------------------------------------------------------- */
/*  HW_MBOX_REG_MASTER_SUBJECT_(L or H)
    +---------------------------------------------------------------------------+
    |   SET                                                                     |
    |   31:0                                                                    |
    |   RO                                                                      |
    +---------------------------------------------------------------------------/

    These registers indicates in which channels this MASTER is configured as SUBJECT
    (as is configured in the channel configuration registers).
*/

/********************************
|       CHANNEL REGISTERS       |
********************************/

/* These registers contain data related to the channels. There's a set of these for
   each channel. */

/* --------------------------------------------------------------------------- */
/*  HW_MBOX_REG_CHN_CFG
    +---------------------------------------------------------------------------+
    | VALID | RESERVED |  TYPE  |  RESERVED  |   REQ_ID   | COMMANDER | SUBJECT |
    | 31    | 30:28    |  27:24 |  23:16     |   15:8     | 7:4       | 3:0     |
    | RW    | RW       |  RW    |  RW        |   RW       | RW        | RW      |
    +---------------------------------------------------------------------------/

    This Register Holds the configuration of the Channel.

    NOTE: only the supervisor has write access to this register and only when
          CTRL.START=FALSE or when CTRL.START=TRUE and VALID=FALSE.

    VALID - 1 indicates valid configuration.
            0 indicates that the channel is not allocated, and hence usage of the
              channel is illegal.

    TYPE - The Type of MBOX protocol that is employed on the channel.
           Possible Values: COMMAND=0, EVENT=1.

           * NOTE: we only need 2 bit (for 00/01/10 possible values). The other
                   bits are reserved for future type expansion and byte alignment
                   considerations of the fields.

    REQ_ID - Request ID, a unique identifier (0-255 possible values) for the
             channel configuration. This field will be set by the supervisor
             upon configuration of the channels. This value is needed as future
             implementation may allow 2 identical channel configuration (for Low
             and High priority variants) in which case one cannot deterministically
             deduce the usage of channels just by observing <TYPE, COMMANDER, SUBJECT>
             values.

             * NOTE: we only need 0-63 (as there are 64 channel). The other bits
                     are reserved for future channel expansion and byte alignment
                     considerations of the fields.

    COMMANDER - MID of the Commander Master.

    SUBJECT - MID of the Subject Master.
*/

/* Adding the word CHN even though the Fields
   don't have it to make it more understandable. */

#define HW_MBOX_FLDDEFS_CHN_VALID_OFFSET (31)
#define HW_MBOX_FLDDEFS_CHN_VALID_LENGTH (1)

#define HW_MBOX_FLDDEFS_CHN_TYPE_OFFSET (24)
#define HW_MBOX_FLDDEFS_CHN_TYPE_LENGTH (4)

#define HW_MBOX_FLDDEFS_CHN_REQ_ID_OFFSET (8)
#define HW_MBOX_FLDDEFS_CHN_REQ_ID_LENGTH (8)

#define HW_MBOX_FLDDEFS_CHN_COMMANDER_OFFSET (4)
#define HW_MBOX_FLDDEFS_CHN_COMMANDER_LENGTH (4)

#define HW_MBOX_FLDDEFS_CHN_SUBJECT_OFFSET (0)
#define HW_MBOX_FLDDEFS_CHN_SUBJECT_LENGTH (4)

/* --------------------------------------------------------------------------- */
/*  HW_MBOX_REG_CHN_COMMAND
    +---------------------------------------------------------------------------+
    |   COMMAND                                                                 |
    |   31:0                                                                    |
    |   RW                                                                      |
    +---------------------------------------------------------------------------/

    This Register is the COMMAND buffer of the Channel. Once COMMANDER writes to
    this register the SUBJECT gets an interrupt. Once the SUBJECT writes to this
    register the COMMANDER gets an interrupt.

    NOTE: only the channel's COMMANDER and SUBJECT may write to this register.
          These are indicated by the corresponding CHN_CFG register.
*/

/* --------------------------------------------------------------------------- */
/*  HW_MBOX_REG_CHN_STATE
    +---------------------------------------------------------------------------+
    |   RESERVED                                                      |  STATE  |
    |   31:2                                                          |  1:0    |
    |   RW                                                            |  RW     |
    +---------------------------------------------------------------------------/

    NOTE: only the supervisor has write access to this register and can do so
          at any time (even when CTRL.LOCK=TRUE). Any write to this register will
          reset the channel (Channel goes back to state ready and any possibly
          pending interrupt is cleared).

    STATE - Possible values are:
            Ready=0, Pending=1, Consume=2, Done=3.
*/

/* Adding the word CHN even though the Fields
   don't have it to make it more understandable. */

#define HW_MBOX_FLDDEFS_CHN_STATE_OFFSET (0)
#define HW_MBOX_FLDDEFS_CHN_STATE_LENGTH (2)

/***********************************
|       MEMORY MAP REGISTERS       |
***********************************/

/* There's one Memory Map Register per channel. It contains the section in the
   HW_MBOX SRAM that is allocated to this channel. According to this and the
   values listed in the CHN_CFG register access protection is enforced (only
   the COMMANDER and SUBJECT of the channel may access the allocated SRAM
   section). */

/* --------------------------------------------------------------------------- */
/*  HW_MBOX_REG_MEMORY_MAP
    +---------------------------------------------------------------------------+
    | RESERVED |    SIZE                  | RESERVED |    START                 |
    | 31:28    |    27:16                 | 15:12    |    11:0                  |
    | RW       |    RW                    |  RO      |    RW                    |
    +---------------------------------------------------------------------------/

    NOTE: only the supervisor has write access to this register and only when
          CTRL.LOCK=FALSE.

    SIZE - The size in bytes of the region in SRAM allocated to Channel CID.
           SIZE is integral multiple of 4.

    START - A 4 bytes aligned offset within the SRAM to the beginning of
            the region allocated to Channel CID.
*/

/* Adding the word MMAP to make it more understandable.
   (and prevent possible conflict with another field named START in the CTRL reg). */

#define HW_MBOX_FLDDEFS_MMAP_SIZE_OFFSET (16)
#define HW_MBOX_FLDDEFS_MMAP_SIZE_LENGTH (12)

#define HW_MBOX_FLDDEFS_MMAP_START_OFFSET (0)
#define HW_MBOX_FLDDEFS_MMAP_START_LENGTH (12)


/*******************************************
|                                          |
|        3. DEFINE BASIC OPERATIONS        |
|                                          |
*******************************************/

/* This section contains basic functionality that we will use later on
   to define the more complex access function to the HW_MBOX memory. */

/*******************************
|       Basic Mem Access       |
*******************************/

#ifndef HW_MBOX_IS_APP_CPU
#define HW_MBOX_REG32(address)                  ( *(volatile Uint32 *)(address) )
#define HW_MBOX_REG32_READ(address)             ( HW_MBOX_CONVERT_FROM_BIG_ENDIAN32(HW_MBOX_REG32(address)) )
#define HW_MBOX_REG32_WRITE(address, value)     ( (HW_MBOX_REG32(address)) = (HW_MBOX_CONVERT_TO_BIG_ENDIAN32(value)) )
#else
#define HW_MBOX_REG32_READ(address)             ( HW_MBOX_CONVERT_FROM_BIG_ENDIAN32( __raw_readl(address)) )
#define HW_MBOX_REG32_WRITE(address, value)     ( __raw_writel( HW_MBOX_CONVERT_TO_BIG_ENDIAN32(value), address) )
#endif /* ! HW_MBOX_IS_APP_CPU */

/********************************
|       HANDLE Endianness       |
********************************/

/* HW_MBOX memory should be kept BE, all arithmetic operations
   should be done using CPU's native endianness. */

#ifdef HW_MBOX_OS_LINUX
#ifdef __KERNEL__
#define HW_MBOX_CONVERT_TO_BIG_ENDIAN32(val)    cpu_to_be32(val)
#define HW_MBOX_CONVERT_FROM_BIG_ENDIAN32(val)  be32_to_cpu(val)
#else
#define HW_MBOX_CONVERT_TO_BIG_ENDIAN32(val)    htobe32(val)
#define HW_MBOX_CONVERT_FROM_BIG_ENDIAN32(val)  be32toh(val)
#endif /* __KERNEL__ */
#endif /* HW_MBOX_OS_LINUX */

#ifdef HW_MBOX_OS_FLAT
#if defined HW_MBOX_IS_PHY_CPU
/* PHY-CPU is BE! */
#define HW_MBOX_CONVERT_TO_BIG_ENDIAN32(val)    (val)
#define HW_MBOX_CONVERT_FROM_BIG_ENDIAN32(val)  (val)
#endif
#endif /* HW_MBOX_OS_FLAT */

/***********************************************
|       Bit-Field Manipulation Functions       |
***********************************************/

/*! \fn Uint32 hw_mbox_field32_get(Uint32 regData, Uint32 fieldOffset, Uint32 fieldLength)
 *  \brief gets the value of a specific bit-field from a given 32-bit value (in CPU native endianness)
 *  \param[in] regData - the value from which to extract the bit-field (in CPU native endianness)
 *  \param[in] fieldOffset - the offset (in bits) of the required bit-field
 *  \param[in] fieldLength - the number of bits in the required bit-field
 *  \return bit-field value (in CPU native endianness).
 */
static inline Uint32 hw_mbox_field32_get(Uint32 regData, Uint32 fieldOffset, Uint32 fieldLength)
{
    Uint32 mask;
    mask = ((0xFFFFFFFF >> (32 - fieldLength)) << fieldOffset);
    return ((regData & mask) >> fieldOffset);
}

/*! \fn Uint32 hw_mbox_field32_set(Uint32 regData, Uint32 fieldOffset, Uint32 fieldLength, Uint32 fieldData)
 *  \brief sets the value of a specific bit-field in a given 32-bit value (in CPU native endianness)
 *  \param[in] regData - the value to which the bit-field should be written to (in CPU native endianness)
 *  \param[in] fieldOffset - the offset (in bits) of the required bit-field
 *  \param[in] fieldLength - the number of bits in the required bit-field
 *  \param[in] fieldData - the value of the bit-field to be written
 *  \return the new 32-bit value with the new field value set inside (in CPU native endianness).
 */
static inline Uint32 hw_mbox_field32_set(Uint32 regData, Uint32 fieldOffset, Uint32 fieldLength, Uint32 fieldData)
{
    Uint32 mask;
    mask = ((0xFFFFFFFF >> (32 - fieldLength)) << fieldOffset);
    regData &= (~mask);
    regData |= (mask & (fieldData << fieldOffset));
    return regData;
}

/****************************************************************
|                                                               |
|        4. GENERIC LOW LEVEL MEM ACCESS MACRO TEMPLATES        |
|                                                               |
****************************************************************/

/*
    As Mentioned before regarding register Access there are 2 cases:

    1. Direct Access - In this case Base Address of the HW_MBOX is always the same, Hardcoded
                       determined by system architecture and Low Level memory access is enabled
                       (i.e. Linux KERNEL of FLAT OS CPU). We can define the generic access Macros
                       here.

    2. Indirect Access - In this case Base Address of the HW_MBOX is a not yet determined
                         parameter which will be later decided (i.e. according to mmap call
                         for LINUX user space). Direct access to memory can't be used.

                         An intermediate Layer mitigating the actual Access Must be Used!
                         Due to this need the generic access macros will be defined in the
                         file that can handle the access (i.e. hw_mbox_hal for Linux User Space).

    IMPORTANT NOTE: If including this file and taking the Indirect Access route you are obliged
                    to define these Generic Access Macros with same structure (parameters and
                    functionality), the Macros that should be defined are:

                    HW_MBOX_REG_GET(regOffset)          returning Uint32
                    HW_MBOX_REG_SET(regOffset, value)   returning void

                    HW_MBOX_FIELD_GET(regOffset, fieldOffset, fieldLength)          returning Uint32
                    HW_MBOX_FIELD_SET(regOffset, fieldOffset, fieldLength, value)   returning void

                    HW_MBOX_BIT_SET_VALUE(regOffset, bitPosition, bitVal)   returning void

    SRAM Access:

    These are generic functions anyways: If using Direct Define here, else define in the wrapping Layer.

*/

#if ((defined(HW_MBOX_OS_LINUX) && defined(__KERNEL__)) || defined(HW_MBOX_OS_FLAT) || defined(HW_MBOX_OS_NONE))

/* Define HW_MBOX Base address here: */
#if   defined HW_MBOX_IS_NP_CPU
#define HW_MBOX_BASE    AVALANCHE_MAILBOX
#elif defined HW_MBOX_IS_APP_CPU
/* App-CPU Kernel determines the Base address at runtime, driver should
   define HW_MBOX_BASE as a variable that holds this mapped base address. */
#elif defined HW_MBOX_IS_PHY_CPU
#define HW_MBOX_BASE    PHY_CPU_HW_MBOX_BASE
#else
#define HW_MBOX_BASE    (0)
#endif

#ifdef HW_MBOX_EXTENDED_MEMORY
#define HW_MBOX_EXTMEM_TOTAL_SIZE       0x2000
#define HW_MBOX_EXTMEM_PART_SIZE        (HW_MBOX_EXTMEM_TOTAL_SIZE >> 1)
#define HW_MBOX_EXTMEM_PART_A_OFFSET    0
#define HW_MBOX_EXTMEM_PART_B_OFFSET    HW_MBOX_EXTMEM_PART_SIZE

/* Define HW_MBOX EXT memory address here: */
#if defined HW_MBOX_IS_NP_CPU
#define HW_MBOX_EXTMEM_BASE         AVALANCHE_SRAM_BASE
#elif defined HW_MBOX_IS_APP_CPU
/* App-CPU Kernel determines extended memory Base address at runtime, driver should
   define HW_MBOX_EXTMEM_BASE as a variable that holds this mapped base address. */
#endif

#endif /* HW_MBOX_EXTENDED_MEMORY */

/******************************************************************************
|       Generic Low Level Mem Access Macro Templates For Direct Access:       |
******************************************************************************/

#define HW_MBOX_REG_GET(regOffset)           HW_MBOX_REG32_READ(regOffset + HW_MBOX_BASE)
#define HW_MBOX_REG_SET(regOffset, value)    HW_MBOX_REG32_WRITE(regOffset + HW_MBOX_BASE, value)

#define HW_MBOX_FIELD_GET(regOffset, fieldOffset, fieldLength)    hw_mbox_field32_get(HW_MBOX_REG_GET(regOffset), \
                                                                                      fieldOffset,                \
                                                                                      fieldLength)

#define HW_MBOX_FIELD_SET(regOffset, fieldOffset, fieldLength, value)    HW_MBOX_REG_SET(regOffset,                                      \
                                                                                         hw_mbox_field32_set(HW_MBOX_REG_GET(regOffset), \
                                                                                                             fieldOffset,                \
                                                                                                             fieldLength,                \
                                                                                                             value))

#define HW_MBOX_BIT_SET_VALUE(regOffset, bitPosition, bitVal)    HW_MBOX_REG_SET(regOffset,                                            \
                                                                                 hw_mbox_field32_set(HW_MBOX_REG_GET(regOffset),       \
                                                                                                     bitPosition, 1, bitVal))

#define HW_MBOX_POINTER_TO(address)                                             ((Uint8 *)(address))
#define HW_MBOX_COPY_FROM_ADDR(srcAddr, srcAddrLen, pDestBuffer)                memcpy(pDestBuffer, srcAddr, srcAddrLen)
#define HW_MBOX_COPY_TO_ADDR(pSrcBuffer, srcBufferLen, destAddr)                memcpy(destAddr, pSrcBuffer, srcBufferLen)
#define HW_MBOX_RESET_MEM(pMem, memLen)                                         memset(pMem, 0, memLen)

/* SRAM Memory ACCESS: */
#define HW_MBOX_POINTER_TO_SRAM(sramOffset)                                     HW_MBOX_POINTER_TO(HW_MBOX_MEMDEFS_SRAM_OFFSET(sramOffset) + HW_MBOX_BASE)
#define HW_MBOX_COPY_FROM_SRAM(srcSramOffset, srcSramLen, pDestBuffer)          HW_MBOX_COPY_FROM_ADDR(HW_MBOX_POINTER_TO_SRAM(srcSramOffset), srcSramLen, pDestBuffer)
#define HW_MBOX_COPY_TO_SRAM(pSrcBuffer, srcBufferLen, destSramOffset)          HW_MBOX_COPY_TO_ADDR(pSrcBuffer, srcBufferLen, HW_MBOX_POINTER_TO_SRAM(destSramOffset))
#define HW_MBOX_RESET_SRAM(sramOffset, sramLen)                                 HW_MBOX_RESET_MEM(HW_MBOX_POINTER_TO_SRAM(sramOffset), sramLen)

#ifdef HW_MBOX_EXTENDED_MEMORY
#define HW_MBOX_EXTMEM_ADDR_BY_OFFSET(extMemOffset)                             ( HW_MBOX_EXTMEM_BASE + extMemOffset )
#define HW_MBOX_POINTER_TO_EXTMEM(extMemOffset)                                 HW_MBOX_POINTER_TO(HW_MBOX_EXTMEM_ADDR_BY_OFFSET(extMemOffset))
#define HW_MBOX_COPY_FROM_EXTMEM(srcExtmemOffset, srcExtmemLen, pDestBuffer)    HW_MBOX_COPY_FROM_ADDR(HW_MBOX_POINTER_TO_EXTMEM(srcExtmemOffset), srcExtmemLen, pDestBuffer)
#define HW_MBOX_COPY_TO_EXTMEM(pSrcBuffer, srcBufferLen, destExtmemOffset)      HW_MBOX_COPY_TO_ADDR(pSrcBuffer, srcBufferLen, HW_MBOX_POINTER_TO_EXTMEM(destExtmemOffset))
#define HW_MBOX_RESET_EXTMEM(extMemOffset, extMemLen)                           HW_MBOX_RESET_MEM(HW_MBOX_POINTER_TO_EXTMEM(extMemOffset), extMemLen)
#endif /* HW_MBOX_EXTENDED_MEMORY */

#endif /* ((defined(HW_MBOX_OS_LINUX) && defined(__KERNEL__)) || defined(HW_MBOX_OS_FLAT) || defined(HW_MBOX_OS_NONE)) */


/*************************************************************
|                                                            |
|        5. SPECIFIC REGISTERS & FIELDS ACCESS MACROS        |
|                                                            |
*************************************************************/

/* These are shared by both direct and Indirect due to the use of the Generic Intermediate Layer! [defined at step 4] */

/*************************************
|       SINGLE BIT OPERATIONS:       |
*************************************/

#define HW_MBOX_BIT_SET(regOffset, bitPosition)      HW_MBOX_BIT_SET_VALUE(regOffset, bitPosition, 1)
#define HW_MBOX_BIT_UNSET(regOffset, bitPosition)    HW_MBOX_BIT_SET_VALUE(regOffset, bitPosition, 0)

/********************************
|       CONTROL REGISTERS       |
********************************/

/* --------------------------------------------------------------------------- */
/* HW_MBOX_ID_REG: */

/*! HW_MBOX_ID: Identifies the core version. */
#define HW_MBOX_ID_REG_GET()      HW_MBOX_REG_GET(HW_MBOX_REGDEFS_ID_OFFSET)

/*! HW_MBOX_ID: MAJOR - Bits[31:28], RO, Identifies core version (Major). */
#define HW_MBOX_ID_MAJOR_GET()    HW_MBOX_FIELD_GET(HW_MBOX_REGDEFS_ID_OFFSET, HW_MBOX_FLDDEFS_MAJOR_OFFSET, HW_MBOX_FLDDEFS_MAJOR_LENGTH)

/*! HW_MBOX_ID: MINOR - Bits[27:24], RO, Identifies core version (Minor). */
#define HW_MBOX_ID_MINOR_GET()    HW_MBOX_FIELD_GET(HW_MBOX_REGDEFS_ID_OFFSET, HW_MBOX_FLDDEFS_MINOR_OFFSET, HW_MBOX_FLDDEFS_MINOR_LENGTH)

/* --------------------------------------------------------------------------- */
/* HW_MBOX_CTRL_REG: */

/* NOTE: only the supervisor has write access to this register (preferably when START=FALSE).
         When LOCK is set this register becomes RO. */

/*! HW_MBOX_CTRL: Control and General Configuration information. */
#define HW_MBOX_CTRL_REG_GET()              HW_MBOX_REG_GET(HW_MBOX_REGDEFS_CTRL_OFFSET)

/*! HW_MBOX_CTRL: MON_EXIST - Bit(11), RW, indicates whether a monitor exists. */
#define HW_MBOX_CTRL_MON_EXIST_GET()        HW_MBOX_FIELD_GET(HW_MBOX_REGDEFS_CTRL_OFFSET, HW_MBOX_FLDDEFS_MON_EXIST_OFFSET, HW_MBOX_FLDDEFS_MON_EXIST_LENGTH)
#define HW_MBOX_CTRL_MON_EXIST_SET(value)   HW_MBOX_FIELD_SET(HW_MBOX_REGDEFS_CTRL_OFFSET, HW_MBOX_FLDDEFS_MON_EXIST_OFFSET, HW_MBOX_FLDDEFS_MON_EXIST_LENGTH, value)

/*! HW_MBOX_IS_MON_EXIST(): Use this Macro to find out if there's a Monitor set. */
#define HW_MBOX_IS_MON_EXIST()             (HW_MBOX_CTRL_MON_EXIST_GET() == True)

/*! HW_MBOX_CTRL: MONITOR - Bits[10:7], RW, MID of the monitor (only relevant if MON_EXIST=TRUE). */
#define HW_MBOX_CTRL_MONITOR_GET()         HW_MBOX_FIELD_GET(HW_MBOX_REGDEFS_CTRL_OFFSET, HW_MBOX_FLDDEFS_MONITOR_OFFSET, HW_MBOX_FLDDEFS_MONITOR_LENGTH)
#define HW_MBOX_CTRL_MONITOR_SET(value)    HW_MBOX_FIELD_SET(HW_MBOX_REGDEFS_CTRL_OFFSET, HW_MBOX_FLDDEFS_MONITOR_OFFSET, HW_MBOX_FLDDEFS_MONITOR_LENGTH, value)

/*! HW_MBOX_CTRL: LOCK - Bit(6), RWO, indicates whether the MBOX is Locked (once set becomes RO). */
#define HW_MBOX_CTRL_LOCK_GET()         HW_MBOX_FIELD_GET(HW_MBOX_REGDEFS_CTRL_OFFSET, HW_MBOX_FLDDEFS_LOCK_OFFSET, HW_MBOX_FLDDEFS_LOCK_LENGTH)
#define HW_MBOX_CTRL_LOCK_SET(value)    HW_MBOX_FIELD_SET(HW_MBOX_REGDEFS_CTRL_OFFSET, HW_MBOX_FLDDEFS_LOCK_OFFSET, HW_MBOX_FLDDEFS_LOCK_LENGTH, value)

/*! HW_MBOX_LOCK(): Use this Macro to LOCK the HW_MBOX (irreversible, Making all configuration constant)! */
#define HW_MBOX_LOCK()    HW_MBOX_CTRL_LOCK_SET(True)     /* No need for Unlock since Once locked becomes RO */

/*! HW_MBOX_IS_LOCK(): Use this Macro to find out if the HW_MBOX is Locked. */
#define HW_MBOX_IS_LOCK()    (HW_MBOX_CTRL_LOCK_GET() == True)

/*! HW_MBOX_CTRL: DEBUG - Bit(5), RW, indicates whether the MBOX is in debug mode. */
#define HW_MBOX_CTRL_DEBUG_GET()         HW_MBOX_FIELD_GET(HW_MBOX_REGDEFS_CTRL_OFFSET, HW_MBOX_FLDDEFS_DEBUG_OFFSET, HW_MBOX_FLDDEFS_DEBUG_LENGTH)
#define HW_MBOX_CTRL_DEBUG_SET(value)    HW_MBOX_FIELD_SET(HW_MBOX_REGDEFS_CTRL_OFFSET, HW_MBOX_FLDDEFS_DEBUG_OFFSET, HW_MBOX_FLDDEFS_DEBUG_LENGTH, value)

/*! HW_MBOX_IS_DEBUG(): Use this Macro to find out if the HW_MBOX is Hardware Debug Mode. */
#define HW_MBOX_IS_DEBUG()    (HW_MBOX_CTRL_DEBUG_GET() == True)

/*! HW_MBOX_CTRL: START - Bit(4), RW, indicates whether the MBOX is configured and functional. */
#define HW_MBOX_CTRL_START_GET()         HW_MBOX_FIELD_GET(HW_MBOX_REGDEFS_CTRL_OFFSET, HW_MBOX_FLDDEFS_START_OFFSET, HW_MBOX_FLDDEFS_START_LENGTH)
#define HW_MBOX_CTRL_START_SET(value)    HW_MBOX_FIELD_SET(HW_MBOX_REGDEFS_CTRL_OFFSET, HW_MBOX_FLDDEFS_START_OFFSET, HW_MBOX_FLDDEFS_START_LENGTH, value)

/*! HW_MBOX_IS_STARTED(): Use this Macro to find out if the HW_MBOX Hardware is ready with initial configuration set. */
#define HW_MBOX_IS_STARTED()    (HW_MBOX_CTRL_START_GET() == True)

/*! HW_MBOX_CTRL: SUPERVISOR - Bits[3:0], RW, MID of the SUPERVISOR. */
#define HW_MBOX_CTRL_SUPERVISOR_GET()         HW_MBOX_FIELD_GET(HW_MBOX_REGDEFS_CTRL_OFFSET, HW_MBOX_FLDDEFS_SUPERVISOR_OFFSET, HW_MBOX_FLDDEFS_SUPERVISOR_LENGTH)
#define HW_MBOX_CTRL_SUPERVISOR_SET(value)    HW_MBOX_FIELD_SET(HW_MBOX_REGDEFS_CTRL_OFFSET, HW_MBOX_FLDDEFS_SUPERVISOR_OFFSET, HW_MBOX_FLDDEFS_SUPERVISOR_LENGTH, value)

/*! HW_MBOX_IS_SUPERVISOR(): Use this Macro to find out if the current Master is the supervisor. */
#define HW_MBOX_IS_SUPERVISOR()    (HW_MBOX_CTRL_SUPERVISOR_GET() == HW_MBOX_MID)

/* --------------------------------------------------------------------------- */
/* HW_MBOX_ERROR(REG): */

/* NOTE: only the supervisor has write access to this register. */

/*! HW_MBOX_ERROR: Contains information regarding HW Access Violation errors. */
#define HW_MBOX_ERROR_REG_GET()           HW_MBOX_REG_GET(HW_MBOX_REGDEFS_ERROR_OFFSET)

/*! HW_MBOX_ERROR: COUNT - Bits[15:8], RW, Incremented by one upon error detection. */
#define HW_MBOX_ERROR_COUNT_GET()         HW_MBOX_FIELD_GET(HW_MBOX_REGDEFS_ERROR_OFFSET, HW_MBOX_FLDDEFS_ERROR_COUNT_OFFSET, HW_MBOX_FLDDEFS_ERROR_COUNT_LENGTH)
#define HW_MBOX_ERROR_COUNT_SET(value)    HW_MBOX_FIELD_SET(HW_MBOX_REGDEFS_ERROR_OFFSET, HW_MBOX_FLDDEFS_ERROR_COUNT_OFFSET, HW_MBOX_FLDDEFS_ERROR_COUNT_LENGTH, value)

#define HW_MBOX_ERROR_COUNT_MIN        0x0
#define HW_MBOX_ERROR_COUNT_MAX        0xFF  /*! (255) */

#define HW_MBOX_ERROR_COUNT_RESET()    HW_MBOX_ERROR_COUNT_SET(HW_MBOX_ERROR_COUNT_MIN)

/*! HW_MBOX_ERROR: ERRMID - Bits[3:0], RO, MID of the last HW_MBOX access which violated the protection rules. */
#define HW_MBOX_ERROR_ERRMID_GET()     HW_MBOX_FIELD_GET(HW_MBOX_REGDEFS_ERROR_OFFSET, HW_MBOX_FLDDEFS_ERRMID_OFFSET, HW_MBOX_FLDDEFS_ERRMID_LENGTH)

/*******************************
|       MASTER REGISTERS       |
*******************************/

/* These are mapped so that each Master only sees its own registers.
   Generally speaking a Master shouldn't access another Master's Master registers.
   see "Other Masters Master registers" section regarding this issue. */

/* --------------------------------------------------------------------------- */
/* INTERRUPT STATUS REGISTERS: */

/* NOTE: Reading these registers is followed by write back to clear the interrupts.
         Hence these should be read only by the Master owner. */

/*! HW_MBOX_MASTER_INTR_STATUS_L(REG), RO:
    The value “1” in bit number N indicates that Channel N issued interrupt to Master MID.
    Bit #N is cleared when the interrupt source is cleared. */
#define HW_MBOX_MASTER_INTR_STATUS_L_REG_GET()       HW_MBOX_REG_GET(HW_MBOX_REGDEFS_MASTER_INTR_STATUS_L_OFFSET)

/*! HW_MBOX_MASTER_INTR_STATUS_H(REG), RO:
    The value “1” in bit number N indicates that Channel (32 + N) issued interrupt to Master MID.
    Bit #N is cleared when the interrupt source is cleared. */
#define HW_MBOX_MASTER_INTR_STATUS_H_REG_GET()       HW_MBOX_REG_GET(HW_MBOX_REGDEFS_MASTER_INTR_STATUS_H_OFFSET)

/* --------------------------------------------------------------------------- */
/* INTERRUPT MASK REGISTERS: */

/*! HW_MBOX_MASTER_INTR_MASK_L(REG), RW:
    The value “1” in bit number N indicates that interrupt from Channel N is not enabled. */
#define HW_MBOX_MASTER_INTR_MASK_L_REG_GET()         HW_MBOX_REG_GET(HW_MBOX_REGDEFS_MASTER_INTR_MASK_L_OFFSET)
#define HW_MBOX_MASTER_INTR_MASK_L_REG_SET(value)    HW_MBOX_REG_SET(HW_MBOX_REGDEFS_MASTER_INTR_MASK_L_OFFSET, value)

/*! Appends bitPattern onto the Interrupt Enable Mask */
#define HW_MBOX_MASTER_INTR_MASK_L_REG_APPEND(bitPattern) \
          HW_MBOX_MASTER_INTR_MASK_L_REG_SET(HW_MBOX_MASTER_INTR_MASK_L_REG_GET() | (bitPattern))

/*! Clears bitPattern from the Interrupt Enable Mask */
#define HW_MBOX_MASTER_INTR_MASK_L_REG_CLEAR(bitPattern) \
          HW_MBOX_MASTER_INTR_MASK_L_REG_SET(HW_MBOX_MASTER_INTR_MASK_L_REG_GET() & (~(bitPattern)))

/*! HW_MBOX_MASTER_INTR_MASK_H(REG), RW:
    The value “1” in bit number N indicates that interrupt from Channel (32 + N) is not enabled. */
#define HW_MBOX_MASTER_INTR_MASK_H_REG_GET()         HW_MBOX_REG_GET(HW_MBOX_REGDEFS_MASTER_INTR_MASK_H_OFFSET)
#define HW_MBOX_MASTER_INTR_MASK_H_REG_SET(value)    HW_MBOX_REG_SET(HW_MBOX_REGDEFS_MASTER_INTR_MASK_H_OFFSET, value)

/*! Appends bitPattern onto the Interrupt Enable Mask */
#define HW_MBOX_MASTER_INTR_MASK_H_REG_APPEND(bitPattern) \
          HW_MBOX_MASTER_INTR_MASK_H_REG_SET(HW_MBOX_MASTER_INTR_MASK_H_REG_GET() | (bitPattern))

/*! Clears bitPattern from the Interrupt Enable Mask */
#define HW_MBOX_MASTER_INTR_MASK_H_REG_CLEAR(bitPattern) \
          HW_MBOX_MASTER_INTR_MASK_H_REG_SET(HW_MBOX_MASTER_INTR_MASK_H_REG_GET() & (~(bitPattern)))

/*! Masks interrupts from a specific CID (<paramCID> = [0-63]) */
#define HW_MBOX_MASTER_INTR_MASK_CID(paramCID) \
do{ \
    (((paramCID) > 31)?(HW_MBOX_BIT_SET(HW_MBOX_REGDEFS_MASTER_INTR_MASK_H_OFFSET, (paramCID % 32))): \
                       (HW_MBOX_BIT_SET(HW_MBOX_REGDEFS_MASTER_INTR_MASK_L_OFFSET, (paramCID))));     \
} while (0)

/*! Unmasks interrupts from a specific CID (<param> = [0-63]) */
#define HW_MBOX_MASTER_INTR_UNMASK_CID(paramCID) \
do{ \
    (((paramCID) > 31)?(HW_MBOX_BIT_UNSET(HW_MBOX_REGDEFS_MASTER_INTR_MASK_H_OFFSET, ((paramCID) % 32))): \
                       (HW_MBOX_BIT_UNSET(HW_MBOX_REGDEFS_MASTER_INTR_MASK_L_OFFSET, (paramCID))));       \
} while (0)

/* --------------------------------------------------------------------------- */
/* INTERRUPT CLEAR REGISTERS: */

/*! HW_MBOX_MASTER_INTR_CLEAR_L(REG), RW:
    This is shadow register of HW_MBOX_MASTER_INTR_STATUS_L[MID], used to read it and to clear specific bits.
    The value “1” in bit number N indicates that Channel N issued interrupt to Master MID. */
#define HW_MBOX_MASTER_INTR_CLEAR_L_REG_GET()         HW_MBOX_REG_GET(HW_MBOX_REGDEFS_MASTER_INTR_CLEAR_L_OFFSET)

/*! Upon write of bit pattern (PATTERN) to this register -
    HW_MBOX_MASTER_INTR_STATUS_L is changed to ((~PATTERN) & HW_MBOX_MASTER_INTR_STATUS_L).
    Use it to clear consumed Masked interrupts. */
#define HW_MBOX_MASTER_INTR_CLEAR_L_REG_SET(value)    HW_MBOX_REG_SET(HW_MBOX_REGDEFS_MASTER_INTR_CLEAR_L_OFFSET, value)

/*! HW_MBOX_MASTER_INTR_CLEAR_H(REG), RW:
    This is shadow register of HW_MBOX_MASTER_INTR_STATUS_H[MID], used to read it and to clear specific bits.
    The value “1” in bit number N indicates that Channel (32 + N) issued interrupt to Master MID. */
#define HW_MBOX_MASTER_INTR_CLEAR_H_REG_GET()         HW_MBOX_REG_GET(HW_MBOX_REGDEFS_MASTER_INTR_CLEAR_H_OFFSET)

/*! Upon write of bit pattern (PATTERN) to this register -
    HW_MBOX_MASTER_INTR_STATUS_H is changed to ((~PATTERN) & HW_MBOX_MASTER_INTR_STATUS_H).
    Use it to clear consumed Masked interrupts. */
#define HW_MBOX_MASTER_INTR_CLEAR_H_REG_SET(value)    HW_MBOX_REG_SET(HW_MBOX_REGDEFS_MASTER_INTR_CLEAR_H_OFFSET, value)

/*! Gets the correct INTR_CLEAR_REG (L/H) according to CID range (<paramCID> = [0-63]) */
#define HW_MBOX_MASTER_INTR_CLEAR_REG_GET(paramCID) \
    ((paramCID > 31)?(HW_MBOX_MASTER_INTR_CLEAR_H_REG_GET()):(HW_MBOX_MASTER_INTR_CLEAR_L_REG_GET()))

/* --------------------------------------------------------------------------- */
/* COMMANDER REGISTERS: */

/*! HW_MBOX_MASTER_COMMANDER_L(REG), RO:
    The value “1” in bit number N indicates that this Master is Commander in Channel N. */
#define HW_MBOX_MASTER_COMMANDER_L_REG_GET()        HW_MBOX_REG_GET(HW_MBOX_REGDEFS_MASTER_COMMANDER_L_OFFSET)

/*! HW_MBOX_MASTER_COMMANDER_H(REG), RO:
    The value “1” in bit number N indicates that this Master is Commander in Channel (32 + N). */
#define HW_MBOX_MASTER_COMMANDER_H_REG_GET()        HW_MBOX_REG_GET(HW_MBOX_REGDEFS_MASTER_COMMANDER_H_OFFSET)

/* --------------------------------------------------------------------------- */
/* SUBJECT REGISTERS: */

/*! HW_MBOX_MASTER_SUBJECT_L(REG), RO:
    The value “1” in bit number N indicates that this Master is Subject in Channel N. */
#define HW_MBOX_MASTER_SUBJECT_L_REG_GET()          HW_MBOX_REG_GET(HW_MBOX_REGDEFS_MASTER_SUBJECT_L_OFFSET)

/*! HW_MBOX_MASTER_SUBJECT_H(REG), RO:
    The value “1” in bit number N indicates that this Master is Subject in Channel (32 + N). */
#define HW_MBOX_MASTER_SUBJECT_H_REG_GET()          HW_MBOX_REG_GET(HW_MBOX_REGDEFS_MASTER_SUBJECT_H_OFFSET)

/********************************
|       CHANNEL REGISTERS       |
********************************/

/* --------------------------------------------------------------------------- */
/* HW_MBOX_CHN_CFG[CID](REG): */

/* NOTE: only the supervisor has write access to this register when START=FALSE.
         If START=TRUE it has access as long as VALID=FALSE. */

/*! HW_MBOX_CHN_CFG[CID]: Channel's Configuration Register. */
#define HW_MBOX_CHN_CFG_REG_GET(paramCID)           HW_MBOX_REG_GET(HW_MBOX_REGDEFS_CHN_CFG_OFFSET(paramCID))

/*! HW_MBOX_CHN_CFG[CID]: VALID - Bit(31), RW, indicates whether the channel is configured and valid. */
#define HW_MBOX_CHN_CFG_VALID_GET(paramCID)           HW_MBOX_FIELD_GET(HW_MBOX_REGDEFS_CHN_CFG_OFFSET(paramCID), HW_MBOX_FLDDEFS_CHN_VALID_OFFSET, HW_MBOX_FLDDEFS_CHN_VALID_LENGTH)
#define HW_MBOX_CHN_CFG_VALID_SET(paramCID, value)    HW_MBOX_FIELD_SET(HW_MBOX_REGDEFS_CHN_CFG_OFFSET(paramCID), HW_MBOX_FLDDEFS_CHN_VALID_OFFSET, HW_MBOX_FLDDEFS_CHN_VALID_LENGTH, value)

/*! HW_MBOX_CHN_CFG[CID]: TYPE - Bits[27:24], RW, The Type of HW_MBOX protocol that is employed on the channel. */
#define HW_MBOX_CHN_CFG_TYPE_GET(paramCID)           HW_MBOX_FIELD_GET(HW_MBOX_REGDEFS_CHN_CFG_OFFSET(paramCID), HW_MBOX_FLDDEFS_CHN_TYPE_OFFSET, HW_MBOX_FLDDEFS_CHN_TYPE_LENGTH)
#define HW_MBOX_CHN_CFG_TYPE_SET(paramCID, value)    HW_MBOX_FIELD_SET(HW_MBOX_REGDEFS_CHN_CFG_OFFSET(paramCID), HW_MBOX_FLDDEFS_CHN_TYPE_OFFSET, HW_MBOX_FLDDEFS_CHN_TYPE_LENGTH, value)

/*! HW_MBOX_CHN_CFG[CID]: REQ_ID - Bits[15:8], RW, Request ID - a unique identifier for the channel configuration. */
#define HW_MBOX_CHN_CFG_REQ_ID_GET(paramCID)           HW_MBOX_FIELD_GET(HW_MBOX_REGDEFS_CHN_CFG_OFFSET(paramCID), HW_MBOX_FLDDEFS_CHN_REQ_ID_OFFSET, HW_MBOX_FLDDEFS_CHN_REQ_ID_LENGTH)
#define HW_MBOX_CHN_CFG_REQ_ID_SET(paramCID, value)    HW_MBOX_FIELD_SET(HW_MBOX_REGDEFS_CHN_CFG_OFFSET(paramCID), HW_MBOX_FLDDEFS_CHN_REQ_ID_OFFSET, HW_MBOX_FLDDEFS_CHN_REQ_ID_LENGTH, value)

/*! HW_MBOX_CHN_CFG[CID]: COMMANDER - Bits[7:4], RW, MID of the Commander Master. */
#define HW_MBOX_CHN_CFG_COMMANDER_GET(paramCID)           HW_MBOX_FIELD_GET(HW_MBOX_REGDEFS_CHN_CFG_OFFSET(paramCID), HW_MBOX_FLDDEFS_CHN_COMMANDER_OFFSET, HW_MBOX_FLDDEFS_CHN_COMMANDER_LENGTH)
#define HW_MBOX_CHN_CFG_COMMANDER_SET(paramCID, value)    HW_MBOX_FIELD_SET(HW_MBOX_REGDEFS_CHN_CFG_OFFSET(paramCID), HW_MBOX_FLDDEFS_CHN_COMMANDER_OFFSET, HW_MBOX_FLDDEFS_CHN_COMMANDER_LENGTH, value)

/*! HW_MBOX_CHN_CFG[CID]: SUBJECT - Bits[3:0], RW, MID of the Subject Master. */
#define HW_MBOX_CHN_CFG_SUBJECT_GET(paramCID)           HW_MBOX_FIELD_GET(HW_MBOX_REGDEFS_CHN_CFG_OFFSET(paramCID), HW_MBOX_FLDDEFS_CHN_SUBJECT_OFFSET, HW_MBOX_FLDDEFS_CHN_SUBJECT_LENGTH)
#define HW_MBOX_CHN_CFG_SUBJECT_SET(paramCID, value)    HW_MBOX_FIELD_SET(HW_MBOX_REGDEFS_CHN_CFG_OFFSET(paramCID), HW_MBOX_FLDDEFS_CHN_SUBJECT_OFFSET, HW_MBOX_FLDDEFS_CHN_SUBJECT_LENGTH, value)

/*! HW_MBOX_CHANNEL_CONFIG_SET(paramCID, _type, _reqID, _commander, _subject): Set complete channel configuration in one Macro. */
#define HW_MBOX_CHANNEL_CONFIG_SET(paramCID, _type, _reqID, _commander, _subject) \
do{ \
    HW_MBOX_CHN_CFG_TYPE_SET(paramCID, _type);              \
    HW_MBOX_CHN_CFG_REQ_ID_SET(paramCID, _reqID);           \
    HW_MBOX_CHN_CFG_COMMANDER_SET(paramCID, _commander);    \
    HW_MBOX_CHN_CFG_SUBJECT_SET(paramCID, _subject);        \
} while (0)

/* Macros that enable getting configuration field values
   from the configuration register value [after reg value has already been read]: */

#define HW_MBOX_CHN_CFG_VALID_GET_FROM_REGVAL( _cfgRegVal )         hw_mbox_field32_get( _cfgRegVal , HW_MBOX_FLDDEFS_CHN_VALID_OFFSET, HW_MBOX_FLDDEFS_CHN_VALID_LENGTH)
#define HW_MBOX_CHN_CFG_TYPE_GET_FROM_REGVAL( _cfgRegVal )          hw_mbox_field32_get( _cfgRegVal , HW_MBOX_FLDDEFS_CHN_TYPE_OFFSET, HW_MBOX_FLDDEFS_CHN_TYPE_LENGTH)
#define HW_MBOX_CHN_CFG_REQ_ID_GET_FROM_REGVAL( _cfgRegVal )        hw_mbox_field32_get( _cfgRegVal , HW_MBOX_FLDDEFS_CHN_REQ_ID_OFFSET, HW_MBOX_FLDDEFS_CHN_REQ_ID_LENGTH)
#define HW_MBOX_CHN_CFG_COMMANDER_GET_FROM_REGVAL( _cfgRegVal )     hw_mbox_field32_get( _cfgRegVal , HW_MBOX_FLDDEFS_CHN_COMMANDER_OFFSET, HW_MBOX_FLDDEFS_CHN_COMMANDER_LENGTH)
#define HW_MBOX_CHN_CFG_SUBJECT_GET_FROM_REGVAL( _cfgRegVal )       hw_mbox_field32_get( _cfgRegVal , HW_MBOX_FLDDEFS_CHN_SUBJECT_OFFSET, HW_MBOX_FLDDEFS_CHN_SUBJECT_LENGTH)

/* --------------------------------------------------------------------------- */
/*  HW_MBOX_CHN_COMMAND[CID](REG), RW: */

/*! HW_MBOX_CHN_COMMAND: Channel's Command Register. */
#define HW_MBOX_CHN_COMMAND_REG_GET(paramCID)           HW_MBOX_REG_GET(HW_MBOX_REGDEFS_CHN_COMMAND_OFFSET(paramCID))
#define HW_MBOX_CHN_COMMAND_REG_SET(paramCID, value)    HW_MBOX_REG_SET(HW_MBOX_REGDEFS_CHN_COMMAND_OFFSET(paramCID), value)

/* --------------------------------------------------------------------------- */
/*  HW_MBOX_CHN_STATE[CID](REG), RW: */

/*! HW_MBOX_CHN_STATE: Channel's State Register. */
#define HW_MBOX_CHN_STATE_REG_GET(paramCID)        HW_MBOX_REG_GET(HW_MBOX_REGDEFS_CHN_STATE_OFFSET(paramCID))

/* Writing any value to the register resets the Channel, meaning:
   the Channel returns to state Ready and if interrupt was issued by the Channel then the interrupt is cleared.
   Only the Supervisor is allowed to write to this register, and it can do it at any time,
   even after LOCK in MBOX_CTRL was set. */
#define HW_MBOX_CHN_STATE_REG_SET(paramCID, value)    HW_MBOX_REG_SET(HW_MBOX_REGDEFS_CHN_STATE_OFFSET(paramCID), value)

#define HW_MBOX_CHN_STATE_RESET(paramCID)    HW_MBOX_CHN_STATE_REG_SET(paramCID, 0) /* Use this to reset the channel and clear any pending interrupts */

/*! HW_MBOX_CHN_STATE[CID]: STATE - Bits[1:0], RW, state of the channel. */
#define HW_MBOX_CHN_STATE_STATE_GET(paramCID)    HW_MBOX_FIELD_GET(HW_MBOX_REGDEFS_CHN_STATE_OFFSET(paramCID), HW_MBOX_FLDDEFS_CHN_STATE_OFFSET, HW_MBOX_FLDDEFS_CHN_STATE_LENGTH)

#define HW_MBOX_CHN_STATE_READY     0
#define HW_MBOX_CHN_STATE_PENDING   1
#define HW_MBOX_CHN_STATE_CONSUME   2
#define HW_MBOX_CHN_STATE_DONE      3

/***********************************
|       MEMORY MAP REGISTERS       |
***********************************/

/* --------------------------------------------------------------------------- */
/*  HW_MBOX_MEMORY_MAP[CID](REG): */

/*  NOTE: only the supervisor has write access to this register (even when LOCK=TRUE). */

/*! HW_MBOX_MEMORY_MAP[CID]: Channel's SRAM allocation. */
#define HW_MBOX_MEMORY_MAP_REG_GET(paramCID)           HW_MBOX_REG_GET(HW_MBOX_REGDEFS_MEMORY_MAP_OFFSET(paramCID))

/*! HW_MBOX_MEMORY_MAP[CID]: SIZE - Bits[27:16], RW, The size in bytes of the region in SRAM allocated to Channel CID. */
#define HW_MBOX_MEMORY_MAP_SIZE_GET(paramCID)           HW_MBOX_FIELD_GET(HW_MBOX_REGDEFS_MEMORY_MAP_OFFSET(paramCID), HW_MBOX_FLDDEFS_MMAP_SIZE_OFFSET, HW_MBOX_FLDDEFS_MMAP_SIZE_LENGTH)
#define HW_MBOX_MEMORY_MAP_SIZE_SET(paramCID, value)    HW_MBOX_FIELD_SET(HW_MBOX_REGDEFS_MEMORY_MAP_OFFSET(paramCID), HW_MBOX_FLDDEFS_MMAP_SIZE_OFFSET, HW_MBOX_FLDDEFS_MMAP_SIZE_LENGTH, value)

/*! HW_MBOX_MEMORY_MAP[CID]: START - Bits[27:16], RW, offset within the SRAM to the beginning of the region allocated to Channel CID. */
#define HW_MBOX_MEMORY_MAP_START_GET(paramCID)           HW_MBOX_FIELD_GET(HW_MBOX_REGDEFS_MEMORY_MAP_OFFSET(paramCID), HW_MBOX_FLDDEFS_MMAP_START_OFFSET, HW_MBOX_FLDDEFS_MMAP_START_LENGTH)
#define HW_MBOX_MEMORY_MAP_START_SET(paramCID, value)    HW_MBOX_FIELD_SET(HW_MBOX_REGDEFS_MEMORY_MAP_OFFSET(paramCID), HW_MBOX_FLDDEFS_MMAP_START_OFFSET, HW_MBOX_FLDDEFS_MMAP_START_LENGTH, value)

/******************************************
|       Parametric Master Registers       |
******************************************/

/* For the purpose of reading other Masters' Master registers */

/* --------------------------------------------------------------------------- */
/* INTERRUPT MASK REGISTERS[paramMID]: */

/*! HW_MBOX_MASTER_PARAM_INTR_MASK_L[paramMID](REG): */
#define HW_MBOX_MASTER_PARAM_INTR_MASK_L_REG_GET(paramMID)    HW_MBOX_REG_GET(HW_MBOX_REGDEFS_MASTER_PARAM_INTR_MASK_L_OFFSET(paramMID))

/*! HW_MBOX_MASTER_PARAM_INTR_MASK_H[paramMID](REG): */
#define HW_MBOX_MASTER_PARAM_INTR_MASK_H_REG_GET(paramMID)    HW_MBOX_REG_GET(HW_MBOX_REGDEFS_MASTER_PARAM_INTR_MASK_H_OFFSET(paramMID))

/* --------------------------------------------------------------------------- */
/* INTERRUPT CLEAR REGISTERS[paramMID]: */

/*! HW_MBOX_MASTER_PARAM_INTR_CLEAR_L[paramMID](REG): */
#define HW_MBOX_MASTER_PARAM_INTR_CLEAR_L_REG_GET(paramMID)    HW_MBOX_REG_GET(HW_MBOX_REGDEFS_MASTER_PARAM_INTR_CLEAR_L_OFFSET(paramMID))

/*! HW_MBOX_MASTER_PARAM_INTR_CLEAR_H[paramMID](REG): */
#define HW_MBOX_MASTER_PARAM_INTR_CLEAR_H_REG_GET(paramMID)    HW_MBOX_REG_GET(HW_MBOX_REGDEFS_MASTER_PARAM_INTR_CLEAR_H_OFFSET(paramMID))

/* --------------------------------------------------------------------------- */
/* COMMANDER REGISTERS[paramMID]: */

/*! HW_MBOX_MASTER_PARAM_COMMANDER_L[paramMID](REG): */
#define HW_MBOX_MASTER_PARAM_COMMANDER_L_REG_GET(paramMID)    HW_MBOX_REG_GET(HW_MBOX_REGDEFS_MASTER_PARAM_COMMANDER_L_OFFSET(paramMID))

/*! HW_MBOX_MASTER_PARAM_COMMANDER_H[paramMID](REG): */
#define HW_MBOX_MASTER_PARAM_COMMANDER_H_REG_GET(paramMID)    HW_MBOX_REG_GET(HW_MBOX_REGDEFS_MASTER_PARAM_COMMANDER_H_OFFSET(paramMID))

/* --------------------------------------------------------------------------- */
/* SUBJECT REGISTERS[paramMID]: */

/*! HW_MBOX_MASTER_PARAM_SUBJECT_L[paramMID](REG): */
#define HW_MBOX_MASTER_PARAM_SUBJECT_L_REG_GET(paramMID)    HW_MBOX_REG_GET(HW_MBOX_REGDEFS_MASTER_PARAM_SUBJECT_L_OFFSET(paramMID))

/*! HW_MBOX_MASTER_PARAM_SUBJECT_H[paramMID](REG): */
#define HW_MBOX_MASTER_PARAM_SUBJECT_H_REG_GET(paramMID)    HW_MBOX_REG_GET(HW_MBOX_REGDEFS_MASTER_PARAM_SUBJECT_H_OFFSET(paramMID))

#ifdef __HW_MBOX_CHANGE_DEFAULT_SUPERVISOR_TO
/********************************
|       Change Supervisor       |
********************************/

/* Since only the supervisor has write access to the HW_MBOX_CTRL register, in the case
   where we might want to have another Master (which is not the default Master) to get active
   control as the supervisor (and not wait for the initial supervisor to willingly relinquish its
   control) we have to access the HW_MBOX as though we are this default supervisor and change
   the value. To make the HW_MBOX think we are this default supervisor we will use a special
   address Macro that will get us to the Ctrl Register through the address space of the default
   supervisor (effectively disguising ourselves). */

/*! \def HW_MBOX_CTRL_AS_DEFAULT_SUPERVISOR_SUPERVISOR_SET(newSupervisorMID)
    \brief Change the default supervisor.
    \attention This will only work once, for the initial substitution (and assuming default supervisor is set).
    \warning Must Use a Valid MID!
*/
#define HW_MBOX_CTRL_AS_DEFAULT_SUPERVISOR_SUPERVISOR_SET(newSupervisorMID) \
          HW_MBOX_FIELD_SET(HW_MBOX_REGDEFS_CTRL_AS_DEFAULT_SUPERVISOR_OFFSET, HW_MBOX_FLDDEFS_SUPERVISOR_OFFSET, HW_MBOX_FLDDEFS_SUPERVISOR_LENGTH, newSupervisorMID)

#endif /* __HW_MBOX_CHANGE_DEFAULT_SUPERVISOR_TO */

/****************************************************
|                                                   |
|        HW_MBOX Opcode Fields                      |
|                                                   |
****************************************************/

/*  --------------------------------------------------------------------------- */
/*  HW_MBOX_REG_OPCODE
    +---------------------------------------------------------------------------+
    | SW RESERVED |  TAG                 |  SW RESERVED    |  LEN_IN_WORDS      |
    | 31:28       |  27:16               |  15:10          |  9:0               |
    | RW          |  RW                  |  RW             |  RW                |
    +---------------------------------------------------------------------------/

    NOTE: These are SW defined fields on to be used on the channel's command register.

    TAG - value indicating the destination of the opcode.

    SW RESERVED - reserved for future SW use.

    LEN_IN_WORDS - How many words are to be read from SRAM (opcode Data).
                   [LEN is integral multiple of 4 Bytes so we can use word count].
*/

#define HW_MBOX_OPCODE_FLDDEFS_TAG_OFFSET (16)
#define HW_MBOX_OPCODE_FLDDEFS_TAG_LENGTH (HW_MBOX_OPCODE_TAG_BITLEN)    /*! already defined in defs */

#define HW_MBOX_OPCODE_FLDDEFS_LEN_IN_WORDS_OFFSET (0)
#define HW_MBOX_OPCODE_FLDDEFS_LEN_IN_WORDS_LENGTH (10)

/* Modifier Macro helpers: */

/*! TAG: customer indication. */
#define HW_MBOX_OPCODE_GET_TAG( _cmdRegVal)                   hw_mbox_field32_get( _cmdRegVal, HW_MBOX_OPCODE_FLDDEFS_TAG_OFFSET, HW_MBOX_OPCODE_FLDDEFS_TAG_LENGTH)
#define HW_MBOX_OPCODE_SET_TAG( _cmdRegVal, _tagVal)          hw_mbox_field32_set( _cmdRegVal, HW_MBOX_OPCODE_FLDDEFS_TAG_OFFSET, HW_MBOX_OPCODE_FLDDEFS_TAG_LENGTH, _tagVal)

/*! LEN: command data len in Bytes [note: in command Register the value is in words]. */
#define HW_MBOX_OPCODE_GET_LEN( _cmdRegVal)                   (hw_mbox_field32_get( _cmdRegVal, HW_MBOX_OPCODE_FLDDEFS_LEN_IN_WORDS_OFFSET, HW_MBOX_OPCODE_FLDDEFS_LEN_IN_WORDS_LENGTH) << 2)
#define HW_MBOX_OPCODE_SET_LEN( _cmdRegVal, _lenValInBytes)   hw_mbox_field32_set( _cmdRegVal, HW_MBOX_OPCODE_FLDDEFS_LEN_IN_WORDS_OFFSET, HW_MBOX_OPCODE_FLDDEFS_LEN_IN_WORDS_LENGTH, (( _lenValInBytes ) >> 2))

/*********************************
|       High Level Helpers       |
*********************************/

/*! \struct hwMbox_interruptState_t
    \brief struct specifying HW MBOX state of interrupts.
    \note Low CID range is [0-31], High CID range is [32-63].
    \note These are Mask values of CID interrupts, 1 = Disabled, 0 = Enabled.
*/
typedef struct hwMbox_interruptState
{
    Uint32 lowCidRangeMaskVal;
    Uint32 highCidRangeMaskVal;
} hwMbox_interruptState_t;

/*! \def HW_MBOX_INTERRUPT_STATE_GET_TO_STRUCT( _pIntrState )
    \brief Gets the current HW MBOX interrupts on the HW level.
    \param[out] _pIntrState - pointer to <hwMbox_interruptState_t>
                              to save the current state to.
*/
#define HW_MBOX_INTERRUPT_STATE_GET_TO_STRUCT( _pIntrState ) \
do{ \
    ( _pIntrState )->lowCidRangeMaskVal = HW_MBOX_MASTER_INTR_MASK_L_REG_GET();    \
    ( _pIntrState )->highCidRangeMaskVal = HW_MBOX_MASTER_INTR_MASK_H_REG_GET();   \
} while (0)

/*! \def HW_MBOX_RESTORE_INTERRUPTS( _pIntrState )
    \brief Restore HW MBOX interrupt state on the HW level.
    \param[in] _pIntrState - pointer to <hwMbox_interruptState_t>
                             specifying the state to restore.
    \note Done by Un-masking channel interrupts.
*/
#define HW_MBOX_RESTORE_INTERRUPTS( _pIntrState ) \
do{ \
    HW_MBOX_MASTER_INTR_MASK_L_REG_SET(( _pIntrState )->lowCidRangeMaskVal);    \
    HW_MBOX_MASTER_INTR_MASK_H_REG_SET(( _pIntrState )->highCidRangeMaskVal);   \
} while (0)

/*! \def HW_MBOX_ENABLE_ALL_INTERRUPTS()
    \brief Enable ALL HW MBOX interrupts on the HW level.
    \note Done by Un-masking channel interrupts.
*/
#define HW_MBOX_ENABLE_ALL_INTERRUPTS() \
do{ \
    HW_MBOX_MASTER_INTR_MASK_L_REG_SET( 0 );    \
    HW_MBOX_MASTER_INTR_MASK_H_REG_SET( 0 );    \
} while (0)

/*! \def HW_MBOX_DISABLE_ALL_INTERRUPTS()
    \brief Disable HW MBOX interrupts on the HW level.
    \attention save the current state using HW_MBOX_INTERRUPT_STATE_GET_TO_STRUCT
               if planning to later restore [and use HW_MBOX_RESTORE_INTERRUPTS].
    \note Done by masking channel interrupts.
*/
#define HW_MBOX_DISABLE_ALL_INTERRUPTS() \
do{ \
    HW_MBOX_MASTER_INTR_MASK_L_REG_SET( 0xFFFFFFFF );   \
    HW_MBOX_MASTER_INTR_MASK_H_REG_SET( 0xFFFFFFFF );   \
} while (0)

/*! \def HW_MBOX_CHANNEL_CONFIG_GET_TO_STRUCT(paramCID, _pChnConfig)
    \brief Macro to get channel's full configuration (including SRAM) via struct.
    \param[out] _pChnConfig - pointer to a hw_mbox_channelConfig_t struct.
*/
#define HW_MBOX_CHANNEL_CONFIG_GET_TO_STRUCT(paramCID, _pChnConfig) \
do{ \
    memset((_pChnConfig), 0, HW_MBOX_CHANNEL_CONFIG_SIZE);                         \
    (_pChnConfig)->cid = paramCID;                                                 \
    (_pChnConfig)->mboxType = HW_MBOX_CHN_CFG_TYPE_GET(paramCID);                  \
    (_pChnConfig)->commander = HW_MBOX_CHN_CFG_COMMANDER_GET(paramCID);            \
    (_pChnConfig)->subject = HW_MBOX_CHN_CFG_SUBJECT_GET(paramCID);                \
    if (((_pChnConfig)->memBuff.size = HW_MBOX_MEMORY_MAP_SIZE_GET(paramCID)) > 0) \
    {                                                                              \
        /* Channel has SRAM */                                                     \
        (_pChnConfig)->memBuff.start = HW_MBOX_MEMORY_MAP_START_GET(paramCID);     \
    }                                                                              \
    else                                                                           \
    {                                                                              \
        /* Channel has NO SRAM */                                                  \
        (_pChnConfig)->memBuff.start = 0;                                          \
    }                                                                              \
} while (0)

/*! \def HW_MBOX_OPCODE_FIELDS_GET_TO_STRUCT( _cmdRegVal, _pOpcodeFlds)
    \brief Macro to get all opcode fields from a specific command register value via struct.
    \param[in] _cmdRegVal - command register's value.
    \param[out] _pOpCodeFlds - pointer to a hwMbox_opcode_t struct.
*/
#define HW_MBOX_OPCODE_FIELDS_GET_TO_STRUCT( _cmdRegVal, _pOpcodeFlds) \
do{ \
    (_pOpcodeFlds)->tag = HW_MBOX_OPCODE_GET_TAG( _cmdRegVal);          \
    (_pOpcodeFlds)->lenInBytes = HW_MBOX_OPCODE_GET_LEN( _cmdRegVal);   \
} while (0)

/*! \def HW_MBOX_OPCODE_FIELDS_SET_FROM_STRUCT( _cmdRegVal, _pOpcodeFlds)
    \brief Macro to set all opcode fields using struct.
    \param[out] _cmdRegVal - buffer for command register's value.
    \param[in] _pOpCodeFlds - pointer to a hwMbox_opcode_t struct.
*/
#define HW_MBOX_OPCODE_FIELDS_SET_FROM_STRUCT( _cmdRegVal, _pOpcodeFlds) \
do{ \
    (_cmdRegVal) = 0;                                                                 \
    (_cmdRegVal) = HW_MBOX_OPCODE_SET_TAG( _cmdRegVal, (_pOpcodeFlds)->tag);          \
    (_cmdRegVal) = HW_MBOX_OPCODE_SET_LEN( _cmdRegVal, (_pOpcodeFlds)->lenInBytes);   \
} while (0)

#endif /* ! _HW_MBOX_MEM_ACCESS_H_ */
