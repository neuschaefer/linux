/*
 *
 * pal_cppi41.h
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

/** \file   pal_cppi41.h
    \brief  PAL CPPI4.1 header file

    This file is the CPPI4.1 PAL header file to be included by any
    drivers which use CPPI4.1 This file is compliant to the PSP Framework
    1.0 definitions and prototypes.

    This document defines the PAL CPPI 4.1 API and data structrures

    This document has two sections:

    \todo:
        - Latest increase in internal memory

Review comments:

- Only APDSP download is needed in CPPI 4.1
- Alloc descriptor should not re-allocate.
- Alloc descriptor call should use in/out parameter.
- Add accumulator queue<->channel mappings to the Rx/Tx channel configuration structure.

    @author     Greg Guyotte
    @author     Sekhar Nori
 */

#ifndef __PAL_CPPI4_H__
#define __PAL_CPPI4_H__

#include <asm-arm/arch-avalanche/generic/avalanche_pdsp_api.h>
#include "_tistdtypes.h"

/**
 *  \defgroup CPPI4_PAL_Data_Structures CPPI4 PAL Data structures
 */
/*@{*/

/* Enum for CPPI domain type
 */

#if PUMA7_OR_NEWER_SOC_TYPE
typedef enum cppi_domain
{
    CPPI41_DOMAIN_PP = 0,
    CPPI41_DOMAIN_DSG,
    CPPI41_DOMAIN_DOCSIS_US,
    CPPI41_DOMAIN_NUM
} CPPI41_DOMAIN;
#else
typedef enum cppi_domain
{
    CPPI41_DOMAIN_PRIMARY_SR = 0,
    CPPI41_DOMAIN_PRIMARY_DOCSIS,
    CPPI41_DOMAIN_PRIMARY_DOCSIS_DSG0,
    CPPI41_DOMAIN_PRIMARY_DOCSIS_DSG1,
    CPPI41_DOMAIN_NUM
} CPPI41_DOMAIN;
#endif


/* Enum for the channel type
 */
typedef enum dma_mode
{
    CPPI41_DMA_MODE_ENDPOINT = 0,
    CPPI41_DMA_MODE_INFRA
} CPPI41_DMA_MODE;

/* Enum for Descriptor type
 */
typedef enum cppi_desc_type
{
    CPPI41_DESC_TYPE_EMBEDDED = 0,
    CPPI41_DESC_TYPE_HOST,
    CPPI41_DESC_TYPE_MONOLITHIC,
    CPPI41_DESC_TYPE_TEARDOWN
} CPPI41_DESC_TYPE;

typedef enum desc_type
{
    DESC_EMBEDDED = 0,
    DESC_HOST = 16,
    DESC_MONOLITHIC = 18,
    DESC_TEARDOWN = 19
} DESC_TYPE;

/* Enum for free queue types
 */
typedef enum fq_type
{
    CPPI41_FQ_TYPE_FDQ = 0,
    CPPI41_FQ_TYPE_FSBQ
} PAL_Cppi4FQType;

/* Enum for endianness conversion
 */
typedef enum
{
    endiannessBig,
    endiannessLittle
}endianness_e;

#define PAL_CPPI4_DESCSIZE_2_QMGRSIZE(size)     ((size - 24)/4)


/**
 * \brief CPPI4.1 Embedded descriptor structure.
 *
 * The CPPI4.1 Embedded descriptor structure is defined here to enable its
 * reuse across various driver modules. The PAL CPPI (much like the
 * underlying hardware) does not use the descriptor structre internally
 * for any of its operations.
 */
#define EMSLOTCNT      4
#if PUMA6_OR_NEWER_SOC_TYPE
#define EMSLOTCNT_PP                1
//#define EMSLOTCNT_PP_WIFI_RX_REASS  2
#else
#define EMSLOTCNT_PP   EMSLOTCNT
#endif
#define EMSLOTCNT_EXT  8
#define EMSLOTOFFSET   1

typedef struct {

  Uint32 BufInfo;
  Uint32 BufPtr;                // Pointer to the physical data buffer
} Cppi4EmbdBuf;

typedef struct {
    Uint32 descInfo;     /**< Desc type, proto specific word cnt, pkt len (valid only in Host PD)*/
    Uint32 tagInfo;      /**< Source tag (31:16), Dest Tag (15:0) (valid only in Host PD)*/
    Uint32 pktInfo;      /**< pkt err state, type, proto flags, return info, desc location */
    Cppi4EmbdBuf Buf[EMSLOTCNT];
    Uint32 EPI[2];       /**< Extended Packet Information (from SR) */
} Cppi4EmbdDesc;

typedef struct {
    Uint32 descInfo;     /**< Desc type, proto specific word cnt, pkt len (valid only in Host PD)*/
    Uint32 tagInfo;      /**< Source tag (31:16), Dest Tag (15:0) (valid only in Host PD)*/
    Uint32 pktInfo;      /**< pkt err state, type, proto flags, return info, desc location */
    Cppi4EmbdBuf Buf;
    Uint32 EPI[2];       /**< Extended Packet Information (from SR) */
} Cppi4EmbdDescPp;

#if PUMA7_OR_NEWER_SOC_TYPE
typedef struct {
    Uint32 descInfo;     /**< Desc type, proto specific word cnt, pkt len (valid only in Host PD)*/
    Uint32 tagInfo;      /**< Source tag (31:16), Dest Tag (15:0) (valid only in Host PD)*/
    Uint32 pktInfo;      /**< pkt err state, type, proto flags, return info, desc location */
    Cppi4EmbdBuf Buf;
    Uint32 EPI[2];       /**< Extended Packet Information (from SR) */
    Uint32 PSI[5];       /**< Protocol Specific Info */
} Cppi4EmbdDescPpPSI;

typedef Cppi4EmbdDescPpPSI  Cppi4DescPSI;
#endif
#if PUMA6_OR_NEWER_SOC_TYPE
typedef Cppi4EmbdDescPp     Cppi4Desc;
#else
typedef Cppi4EmbdDesc Cppi4Desc;
#endif

typedef struct {
    Uint32 descInfo;     /**< Desc type, proto specific word cnt, pkt len (valid only in Host PD)*/
    Uint32 tagInfo;      /**< Source tag (31:16), Dest Tag (15:0) (valid only in Host PD)*/
    Uint32 pktInfo;      /**< pkt err state, type, proto flags, return info, desc location */
    Cppi4EmbdBuf Buf[1];
    Uint32 EPI[2];       /**< Extended Packet Information (from SR) */
    Uint32 PS[3];       /**< Extended Packet Information (from SR) */
} Cppi4EmbdDesc_ps;

typedef struct {
    Uint32 descInfo;     /**< Desc type, proto specific word cnt, pkt len (valid only in Host PD)*/
    Uint32 tagInfo;      /**< Source tag (31:16), Dest Tag (15:0) (valid only in Host PD)*/
    Uint32 pktInfo;      /**< pkt err state, type, proto flags, return info, desc location */
    Cppi4EmbdBuf Buf[EMSLOTCNT_EXT];
    Uint32 EPI[2];       /**< Extended Packet Information (from SR) */
} Cppi4ExtEmbdDesc;

#define CPPI41_EM_DESCINFO_DTYPE_SHIFT     30
#define CPPI41_EM_DESCINFO_DTYPE_MASK      (0x3u<<CPPI41_EM_DESCINFO_DTYPE_SHIFT)
#define CPPI41_EM_DESCINFO_DTYPE_EMB       0
#define CPPI41_EM_DESCINFO_SLOTCNT_SHIFT   27
#define CPPI41_EM_DESCINFO_SLOTCNT_MASK    (0x7u<<CPPI41_EM_DESCINFO_SLOTCNT_SHIFT)
#define CPPI41_EM_DESCINFO_PSWSIZE_SHIFT   22
#define CPPI41_EM_DESCINFO_PSWSIZE_MASK    (0x1Fu<<CPPI41_EM_DESCINFO_PSWSIZE_SHIFT)
#define CPPI41_EM_DESCINFO_PKTLEN_SHIFT    0
#define CPPI41_EM_DESCINFO_PKTLEN_MASK     (0x003FFFFFu<<CPPI41_EM_DESCINFO_PKTLEN_SHIFT)

#define CPPI41_EM_TAGINFO_SRCPORT_SHIFT    27
#define CPPI41_EM_TAGINFO_SRCPORT_MASK     (0x1Fu<<CPPI41_EM_TAGINFO_SRCPORT_SHIFT)
#define CPPI41_EM_TAGINFO_SRCCHN_SHIFT     21
#define CPPI41_EM_TAGINFO_SRCCHN_MASK      (0x3Fu<<CPPI41_EM_TAGINFO_SRCCHN_SHIFT)
#define CPPI41_EM_TAGINFO_SRCSUBCHN_SHIFT  16
#define CPPI41_EM_TAGINFO_SRCSUBCHN_MASK   (0x1Fu<<CPPI41_EM_TAGINFO_SRCSUBCHN_SHIFT)
#define CPPI41_EM_TAGINFO_DSTTAG_SHIFT     0
#define CPPI41_EM_TAGINFO_DSTTAG_MASK      (0xFFFFu<<CPPI41_EM_TAGINFO_DSTTAG_SHIFT)

#define CPPI41_EM_PKTINFO_PKTERROR_SHIFT    31
#define CPPI41_EM_PKTINFO_PKTERROR_MASK    (1u<<CPPI41_EM_PKTINFO_PKTERROR_SHIFT)

#define CPPI41_EM_PKTINFO_PKTTYPE_SHIFT     26
#define CPPI41_EM_PKTINFO_PKTTYPE_MASK     (0x1Fu<<CPPI41_EM_PKTINFO_PKTTYPE_SHIFT)
#define CPPI41_EM_PKTINFO_PKTTYPE_ATMAAL5           0
#define CPPI41_EM_PKTINFO_PKTTYPE_ATMNULLAAL        1
#define CPPI41_EM_PKTINFO_PKTTYPE_ATMOAM            2
#define CPPI41_EM_PKTINFO_PKTTYPE_ATMTRANSPARENT    3
#define CPPI41_EM_PKTINFO_PKTTYPE_EFM               4
#define CPPI41_EM_PKTINFO_PKTTYPE_USB               5
#define CPPI41_EM_PKTINFO_PKTTYPE_GENERIC           6
#define CPPI41_EM_PKTINFO_PKTTYPE_ETH               7


#define CPPI41_EM_PKTINFO_EOPIDX_SHIFT     20
#define CPPI41_EM_PKTINFO_EOPIDX_MASK      (0x7u<<CPPI41_EM_PKTINFO_EOPIDX_SHIFT)
#define CPPI41_EM_PKTINFO_PROTSPEC_SHIFT   16
#define CPPI41_EM_PKTINFO_PROTSPEC_MASK    (0xFu<<CPPI41_EM_PKTINFO_PROTSPEC_SHIFT)
#define CPPI41_EM_PKTINFO_RETPOLICY_SHIFT  15
#define CPPI41_EM_PKTINFO_RETPOLICY_MASK   (1u<<CPPI41_EM_PKTINFO_RETPOLICY_SHIFT)
#define CPPI41_EM_PKTINFO_ONCHIP_SHIFT     14
#define CPPI41_EM_PKTINFO_ONCHIP_MASK      (1u<<CPPI41_EM_PKTINFO_ONCHIP_SHIFT)
#define CPPI41_EM_PKTINFO_RETQMGR_SHIFT    12
#define CPPI41_EM_PKTINFO_RETQMGR_MASK     (0x3u<<CPPI41_EM_PKTINFO_RETQMGR_SHIFT)
#define CPPI41_EM_PKTINFO_RETQ_SHIFT       0
#define CPPI41_EM_PKTINFO_RETQ_MASK        (0x7FFu<<CPPI41_EM_PKTINFO_RETQ_SHIFT)

#define CPPI41_EM_BUF_VALID_SHIFT          31
#define CPPI41_EM_BUF_VALID_MASK           (1u << CPPI41_EM_BUF_VALID_SHIFT)
#define CPPI41_EM_BUF_MGR_SHIFT            29
#define CPPI41_EM_BUF_MGR_MASK             (0x3u << CPPI41_EM_BUF_MGR_SHIFT)
#define CPPI41_EM_BUF_POOL_SHIFT           24
#define CPPI41_EM_BUF_POOL_MASK            (0x1Fu << CPPI41_EM_BUF_POOL_SHIFT)
#define CPPI41_EM_BUF_BUFFLEN_MASK         (0x3FFFFFu)


/* Commonly used values */
#define CPPI41_EM_DESCINFO_SLOTCNT_MYCNT                ((EMSLOTCNT-1u)<<CPPI41_EM_DESCINFO_SLOTCNT_SHIFT)
#define CPPI41_EM_DESCINFO_SLOTCNT_PP                   ((EMSLOTCNT_PP-1u)<<CPPI41_EM_DESCINFO_SLOTCNT_SHIFT)
//#define CPPI41_EM_DESCINFO_SLOTCNT_PP_WIFI_RX_REASS     ((EMSLOTCNT_PP_WIFI_RX_REASS-1u)<<CPPI41_EM_DESCINFO_SLOTCNT_SHIFT)
#define CPPI41_EM_DESCINFO_SLOTCNT_MPEG                 ((EMSLOTCNT-1u)<<CPPI41_EM_DESCINFO_SLOTCNT_SHIFT)
#define CPPI41_EM_DESCINFO_SLOTCNT_MPEG_ENCAP           ((EMSLOTCNT_EXT-1u)<<CPPI41_EM_DESCINFO_SLOTCNT_SHIFT)
#define CPPI41_EM_DESCINFO_DTYPE_EMBEDDED               (0u<<CPPI41_EM_DESCINFO_DTYPE_SHIFT)
#define CPPI41_EM_PKTINFO_RETPOLICY_RETURN              (1u<<CPPI41_EM_PKTINFO_RETPOLICY_SHIFT)


/**
 * \brief CPPI4.1 Host descriptor structure.
 *
 * The CPPI4.1 Host descriptor structure is defined here to enable its
 * reuse across various driver modules. The PAL CPPI (much like the
 * underlying hardware) does not use the descriptor structre internally
 * for any of its operations.
 */
typedef struct {
    Uint32 descInfo;     /**< Desc type, proto specific word cnt, pkt len (valid only in Host PD)*/
    Uint32 tagInfo;      /**< Source tag (31:16), Dest Tag (15:0) (valid only in Host PD)*/
    Uint32 pktInfo;      /**< pkt err state, type, proto flags, return info, desc location */
    Uint32 buffLen;      /**< Number of valid data bytes in the buffer */
    Uint32 bufPtr;       /**< Pointer to the buffer associated with this descriptor */
    Ptr nextBDPtr;       /**< Pointer to the next buffer descriptor */
    Uint32 orgBuffLen;   /**< Original buffer size */
    Uint32 orgBufPtr;    /**< Original buffer pointer */
    Uint32 netInfoWord0; /**< Network stack private communications info (valid only in Host PD) */
    Uint32 netInfoWord1; /**< Network stack private communications info (valid only in Host PD) */

} Cppi4HostDesc;

typedef struct
{
    Cppi4HostDesc       hw;         /* The Hardware Descriptor */
    int                 psi[5];     /* protocol specific information for fw  */
    struct sk_buff*     skb;        /* The data pointer virtual address */
}Cppi4HostDescLinux;

#define PAL_CPPI4_HOSTDESC_DESC_TYPE_SHIFT      27
#define PAL_CPPI4_HOSTDESC_DESC_TYPE_MASK       (0x1F << PAL_CPPI4_HOSTDESC_DESC_TYPE_SHIFT)
#define PAL_CPPI4_HOSTDESC_DESC_TYPE_HOST       16

#define PAL_CPPI4_HOSTDESC_PROT_WORD_CNT_SHIFT  22
#define PAL_CPPI4_HOSTDESC_PROT_WORD_CNT_MASK   (0x1F << PAL_CPPI4_HOSTDESC_PROT_WORD_CNT_SHIFT)

#define PAL_CPPI4_HOSTDESC_PKT_LEN_SHIFT        0
#define PAL_CPPI4_HOSTDESC_PKT_LEN_MASK         (0x1FFFFF << PAL_CPPI4_HOSTDESC_PKT_LEN_SHIFT)

#define PAL_CPPI4_HOSTDESC_PKT_ERR_SHIFT        31
#define PAL_CPPI4_HOSTDESC_PKT_ERR_MASK         (0x1 << PAL_CPPI4_HOSTDESC_PKT_ERR_SHIFT)
#define PAL_CPPI4_HOSTDESC_PKT_ERR_NOERR        0
#define PAL_CPPI4_HOSTDESC_PKT_ERR_ERR          1

#define PAL_CPPI4_HOSTDESC_PKT_TYPE_SHIFT       26
#define PAL_CPPI4_HOSTDESC_PKT_TYPE_MASK        (0x1F << PAL_CPPI4_HOSTDESC_PKT_TYPE_SHIFT)
#define PAL_CPPI4_HOSTDESC_PKT_TYPE_ATMAAL5             0
#define PAL_CPPI4_HOSTDESC_PKT_TYPE_ATMNULLAAL          1
#define PAL_CPPI4_HOSTDESC_PKT_TYPE_ATMOAM              2
#define PAL_CPPI4_HOSTDESC_PKT_TYPE_ATMTRANSPARENT      3
#define PAL_CPPI4_HOSTDESC_PKT_TYPE_EFM                 4
#define PAL_CPPI4_HOSTDESC_PKT_TYPE_USB                 5
#define PAL_CPPI4_HOSTDESC_PKT_TYPE_GENERIC             6
#define PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH                 7
#define PAL_CPPI4_HOSTDESC_PKT_TYPE_MPEG                8

#define PAL_CPPI4_HOSTDESC_PKT_RETPLCY_SHIFT    15
#define PAL_CPPI4_HOSTDESC_PKT_RETPLCY_MASK     (0x1 << PAL_CPPI4_HOSTDESC_PKT_RETPLCY_SHIFT)
#define PAL_CPPI4_HOSTDESC_PKT_RETPLCY_LINKED   0
#define PAL_CPPI4_HOSTDESC_PKT_RETPLCY_UNLINKED 1

#define PAL_CPPI4_HOSTDESC_DESC_LOC_SHIFT       14
#define PAL_CPPI4_HOSTDESC_DESC_LOC_MASK        (0x1 << PAL_CPPI4_HOSTDESC_DESCLOC_SHIFT)
#define PAL_CPPI4_HOSTDESC_DESC_LOC_OFFCHIP     0
#define PAL_CPPI4_HOSTDESC_DESC_LOC_ONCHIP      1

#define PAL_CPPI4_HOSTDESC_PKT_RETQMGR_SHIFT    12
#define PAL_CPPI4_HOSTDESC_PKT_RETQMGR_MASK     (0x3 << PAL_CPPI4_HOSTDESC_PKT_RETQMGR_SHIFT)
#define PAL_CPPI4_HOSTDESC_PKT_RETQNUM_SHIFT    0
#define PAL_CPPI4_HOSTDESC_PKT_RETQNUM_MASK     (0xFFF << PAL_CPPI4_HOSTDESC_PKT_RETQNUM_SHIFT)

#define PAL_CPPI4_HOSTDESC_NETINFW0_DIVERT_FLAG_MASK    0x80
#define PAL_CPPI4_HOSTDESC_NETINFW0_DO_NOT_DISCARD      0x80000000
#if PUMA7_OR_NEWER_SOC_TYPE
#define PAL_CPPI4_HOSTDESC_NETINFW0_SESSION_VALID       0x00000001
#define PAL_CPPI4_HOSTDESC_NETINFW0_HOST_PRIO_SET       0x00400000
#define PAL_CPPI4_HOSTDESC_NETINFW0_PTID_FLAG_MASK      0x40000000
#endif

/**
 * \brief CPPI4.1 Monolithic descriptor structure.
 *
 * The CPPI4.1 Monolithic descriptor structure is defined here to enable its reuse across various driver modules.
 * The PAL CPPI (much like the underlying hardware) does not use the descriptor structre internally for any of its operations.
 */
typedef struct {
    Uint32 descInfo;     /**< Desc type, proto specific word cnt, data offset, pkt len */
    Uint32 tagInfo;      /**< Source tag (31:16), Dest Tag (15:0) */
    Uint32 pktInfo;      /**< pkt err state, type, proto flags, return info, desc location */
    Uint32 netInfoWord0; /**< Network stack private communications info */
    Uint32 netInfoWord1; /**< Network stack private communications info */
} Cppi4MonolithicDesc;


#define PAL_CPPI4_MONODESC_DESC_TYPE_SHIFT      27
#define PAL_CPPI4_MONODESC_DESC_TYPE_MASK       (0x1F << PAL_CPPI4_MONODESC_DESC_TYPE_SHIFT)
#define PAL_CPPI4_MONODESC_DESC_TYPE_MONO       18

#define PAL_CPPI4_MONODESC_PROT_WORD_CNT_SHIFT  22
#define PAL_CPPI4_MONODESC_PROT_WORD_CNT_MASK   (0x1F << PAL_CPPI4_MONOESC_PROT_WORD_CNT_SHIFT)

#define PAL_CPPI4_MONODESC_DATA_OFFSET_SHIFT    14
#define PAL_CPPI4_MONODESC_DATA_OFFSET_MASK     (0xFF << PAL_CPPI4_MONODESC_DATA_OFFSET_SHIFT)

#define PAL_CPPI4_MONODESC_PKT_LEN_SHIFT        0
#define PAL_CPPI4_MONODESC_PKT_LEN_MASK         (0x1FFF << PAL_CPPI4_MONODESC_PKT_LEN_SHIFT)

#define PAL_CPPI4_MONODESC_PKT_ERR_SHIFT        31
#define PAL_CPPI4_MONODESC_PKT_ERR_MASK         (0x1 << PAL_CPPI4_MONODESC_PKT_ERR_SHIFT)
#define PAL_CPPI4_MONODESC_PKT_ERR_NOERR        0
#define PAL_CPPI4_MONODESC_PKT_ERR_ERR          1

#define PAL_CPPI4_MONODESC_PKT_TYPE_SHIFT       26
#define PAL_CPPI4_MONODESC_PKT_TYPE_MASK        (0x1F << PAL_CPPI4_MONODESC_PKT_TYPE_SHIFT)
#define PAL_CPPI4_MONODESC_PKT_TYPE_MPEG        8

#define PAL_CPPI4_MONODESC_PKT_RETPLCY_SHIFT    15
#define PAL_CPPI4_MONODESC_PKT_RETPLCY_MASK     (0x1 << PAL_CPPI4_MONODESC_PKT_RETPLCY_SHIFT)
#define PAL_CPPI4_MONODESC_PKT_RETPLCY_LINKED   0
#define PAL_CPPI4_MONODESC_PKT_RETPLCY_UNLINKED 1

#define PAL_CPPI4_MONODESC_DESC_ON_CHIP_SHIFT   14
#define PAL_CPPI4_MONODESC_DESC_ON_CHIP_MASK    (0x1 << PAL_CPPI4_MONODESC_DESC_ON_CHIP_SHIFT)
#define PAL_CPPI4_MONODESC_DESC_ON_CHIP_OFFCHIP 0
#define PAL_CPPI4_MONODESC_DESC_ON_CHIP_ONCHIP  1

#define PAL_CPPI4_MONODESC_PKT_RETQMGR_SHIFT    12
#define PAL_CPPI4_MONODESC_PKT_RETQMGR_MASK     (0x3 << PAL_CPPI4_MONODESC_PKT_RETQMGR_SHIFT)
#define PAL_CPPI4_MONODESC_PKT_RETQNUM_SHIFT    0
#define PAL_CPPI4_MONODESC_PKT_RETQNUM_MASK     (0xFFF << PAL_CPPI4_MONODESC_PKT_RETQNUM_SHIFT)

/**
 *  \brief PAL CPPI4.1 Teardown descriptor
 *
 */
typedef struct {
  Uint32 tdInfo;        /**< Teardown information */
  Uint32 swDmaNumber;   /**< Software information nothing to do with hardware*/
  Uint32 reserved[6];   /**< 24 byte padding */
}Cppi4TeardownDesc;

#define PAL_CPPI4_TDDESC_DESC_TYPE_SHIFT      27
#define PAL_CPPI4_TDDESC_DESC_TYPE_MASK       (0x1F << PAL_CPPI4_TDDESC_DESC_TYPE_SHIFT)
#define PAL_CPPI4_TDDESC_DESC_TYPE_TD         19





#ifdef __KERNEL__

#include <asm-arm/arch-avalanche/generic/pal.h>
#include "pal_cppi41cfg.h"   /* PAL Cfg file for reference to HW cfg macros */
#include "cslr_cppi41.h"   /* CSL Header file */

/**
 * \defgroup CPPI4_PAL_Interface    CPPI4 PAL Interface
 *
 *  CPPI4 PAL Layer Interface
 */
/*@{*/

/**
 * \page DataStructures Data structures and Definitions
 * This page gives information on public data strutures and definitions used
 * by the PAL CPPI4.1 API
 */

/*
 *  PAL CPPI4 Error Codes
 *
 *  Refer to PAL Error codes description.
 *
 *  Need to define a unique device specific error code identifier. (as
 *  defined in pal_defs.h)
 */
/*@{*/
#define CPPI4_DEVICE_CODE                       0xFF
#define CPPI4_ERR_DEV_ALREADY_INSTANTIATED      PAL_ERROR(PAL_MAJOR_ERROR, PAL_ERROR_SRC_PAL, CPPI4_DEVICE_CODE, 1)
#define CPPI4_ERR_DEV_NOT_INSTANTIATED          PAL_ERROR(PAL_MAJOR_ERROR, PAL_ERROR_SRC_PAL, CPPI4_DEVICE_CODE, 2)
#define CPPI4_ERR_DEV_NOT_OPEN                  PAL_ERROR(PAL_MAJOR_ERROR, PAL_ERROR_SRC_PAL, CPPI4_DEVICE_CODE, 3)
#define CPPI4_INVALID_PARAM                     PAL_ERROR(PAL_MAJOR_ERROR, PAL_ERROR_SRC_PAL, CPPI4_DEVICE_CODE, 4)
#define CPPI4_ERR_CH_INVALID                    PAL_ERROR(PAL_MAJOR_ERROR, PAL_ERROR_SRC_PAL, CPPI4_DEVICE_CODE, 5)
#define CPPI4_ERR_DEV_ALREADY_OPEN              PAL_ERROR(PAL_MAJOR_ERROR, PAL_ERROR_SRC_PAL, CPPI4_DEVICE_CODE, 6)
#define CPPI4_ERR_DEV_ALREADY_CLOSED            PAL_ERROR(PAL_MAJOR_ERROR, PAL_ERROR_SRC_PAL, CPPI4_DEVICE_CODE, 7)
#define CPPI4_ERR_RX_BUFFER_FREE_FAIL           PAL_ERROR(PAL_MAJOR_ERROR, PAL_ERROR_SRC_PAL, CPPI4_DEVICE_CODE, 8)
#define CPPI4_ERR_RX_BUFFER_ALLOC_FAIL          PAL_ERROR(PAL_MAJOR_ERROR, PAL_ERROR_SRC_PAL, CPPI4_DEVICE_CODE, 9)
#define CPPI4_ERR_INVALID_DESC_QUEUE            PAL_ERROR(PAL_MAJOR_ERROR, PAL_ERROR_SRC_PAL, CPPI4_DEVICE_CODE, 10)
#define CPPI4_ERR_GENERAL                       PAL_ERROR(PAL_MAJOR_ERROR, PAL_ERROR_SRC_PAL, CPPI4_DEVICE_CODE, 11)
#define CPPI4_ERR_TX_CH_ALREADY_OPENED          PAL_ERROR(PAL_MAJOR_ERROR, PAL_ERROR_SRC_PAL, CPPI4_DEVICE_CODE, 12)
#define CPPI4_ERR_RX_CH_ALREADY_OPENED          PAL_ERROR(PAL_MAJOR_ERROR, PAL_ERROR_SRC_PAL, CPPI4_DEVICE_CODE, 13)
#define CPPI4_ERR_TX_CH_ALREADY_CLOSED          PAL_ERROR(PAL_MAJOR_ERROR, PAL_ERROR_SRC_PAL, CPPI4_DEVICE_CODE, 14)
#define CPPI4_ERR_RX_CH_ALREADY_CLOSED          PAL_ERROR(PAL_MAJOR_ERROR, PAL_ERROR_SRC_PAL, CPPI4_DEVICE_CODE, 15)
#define CPPI4_ERR_INVALID_DESC_TYPE             PAL_ERROR(PAL_MAJOR_ERROR, PAL_ERROR_SRC_PAL, CPPI4_DEVICE_CODE, 16)
#define CPPI4_ERR_I_DESC_TYPE                   PAL_ERROR(PAL_MAJOR_ERROR, PAL_ERROR_SRC_PAL, CPPI4_DEVICE_CODE, 17)
#define CPPI4_ERR_MAX_MEMREG_EXCEED             PAL_ERROR(PAL_MAJOR_ERROR, PAL_ERROR_SRC_PAL, CPPI4_DEVICE_CODE, 18)
#define CPPI4_ERR_MAX_DESC_EXCEED               PAL_ERROR(PAL_MAJOR_ERROR, PAL_ERROR_SRC_PAL, CPPI4_DEVICE_CODE, 19)
#define CPPI4_ERR_DESC_ALIGN                    PAL_ERROR(PAL_MAJOR_ERROR, PAL_ERROR_SRC_PAL, CPPI4_DEVICE_CODE, 20)
/*@}*/

/**
 *  \defgroup CPPI4_PAL_Ioctl_Codes CPPI4 PAL Ioctl Codes
 */
/*@{*/
#define PAL_CPPI41_IOCTL_GET_SWVER               0       /**< Get software version */
#define PAL_CPPI41_IOCTL_GET_HWVER               1       /**< Get hardware version */
#define PAL_CPPI41_IOCTL_GET_FDQ_STARVE_CNT      2       /**< Get free descriptor queue starvation count */
#define PAL_CPPI41_IOCTL_GET_FDBQ_STARVE_CNT     3       /**< Get the free descriptor/buffer queue starvation count */
#define PAL_CPPI41_IOCTL_GET_QUEUE_PEND_STATUS   4       /**< Get the queue pending status */
#define PAL_CPPI41_IOCTL_GET_QUEUE_ENTRY_COUNT   5       /**< Get the queue entry count */
#define PAL_CPPI41_IOCTL_GET_QUEUE_BYTE_COUNT    6       /**< Get the queue byte count */
#define PAL_CPPI41_IOCTL_GET_QUEUE_HEAD_PKT_SIZE 7       /**< Get the head packet size */
#define PAL_CPPI41_IOCTL_QUEUE_DIVERT            8       /**< Divert contents of one queue to another */
#define PAL_CPPI41_IOCTL_BUFMGR_SOFT_RESET       9       /**< Soft reset the buffer manager */
#define PAL_CPPI41_IOCTL_BUF_REFCNT_INCR        10       /**< Increment buffer reference count */
/*@}*/


/*
 *  APDSP commands
 */
#define APDSP_CMD_ENABLE                        0x81    /**< APDSP enable */
#define APDSP_CMD_DISABLE                       0x80    /**< APDSP disable */

/*
 * APDSP list entry types
 */
#define PAL_CPPI41_ACC_ENTRY_TYPE_D              0
#define PAL_CPPI41_ACC_ENTRY_TYPE_CD             1
#define PAL_CPPI41_ACC_ENTRY_TYPE_ABCD           2

/*
 * Scheduler Macros
 */
#define PAL_CPPI41_DMA_RX_CH 1
#define PAL_CPPI41_DMA_TX_CH 0
#define PAL_CPPI41_DMA_CH_CONFIG(dir,ch_num)  (dir << 7 | ch_num)



/**
 * \brief The Queue Tuple
 * - The basic queue tuple in CPPI 4.1 used across all
 *   data structures where a definition of a queue is required.
 */
typedef struct
{
    Uint32 qMgr;        /**< The queue manager number */
    Uint32 qNum;        /**< The queue number */
} Cppi4Queue;

/**
 * \brief The BufPool Tuple
 * - The basic BufPool tuple in CPPI 4.1 used across all data
 *   structures where a definition of a buffer pool is required.
 */
typedef struct
{
    Uint32 bMgr;        /**< The buffer manager number */
    Uint32 bPool;       /**< The buffer pool number */
} Cppi4BufPool;

/**
 * \brief The BufPoolInfo
 * - A broader information about a specific buffer pool
 */
typedef struct
{
    Uint32  bPoolPhyAddr;
    Uint32  bPoolSize;
    Uint32  bufSize;
    Uint32  numBuf;
    Uint32  bPoolTopPhyAddr;
    Uint32  bPoolOffset;
} Cppi4BufPoolInfo;


/**
 * \brief The CPPI4 descriptor region.
 * - Defines the characteristics of a descriptor region.
 */
typedef struct
{
    Bool    isAllocated;    /**< Information on whether this region has been allocated or not   */
    Bool    isOnChip;       /**< Defines if the descriptor region is on chip or not             */
    Bool    isPreAllocated; /**< Defines if need to be internally allocated by CPPI init code   */
    Bool    reserved;

    Ptr     base;           /**< The base address of descriptor region. For off-chip descriptors, this is ignored. The CPPI4.1
                             initialization API will provide a base in this case. A base must be provided in case of on-chip descriptors.
                             This base should be physical address. In all cases, care should be taken to make sure the base addresses of
                             consecutive regions are in ascending order.    */
    Uint32  szDesc;         /**< Size of each descriptor in the region          */
    Uint32  numDesc;        /**< The number of descriptors in the region        */
} Cppi4DescReg;

/**
 *  \brief Queue Manager config info
 *
 *  Contains data needed to initialize an instance of queue manager. Instances of this structure will be included
 *  in PAL CPPI 4.1 initial configuration structure.
 *
 */
typedef struct
{
    CSL_Queue_Manager_Region_RegsOvly   queueMgrRgnBase;    /**< Base Address (virtual) of Queue Manager region */
    CSL_Desc_Mem_Setup_Region_RegsOvly  descMemRgnBase;     /**< Base Address (virtual) of descriptor memory region.*/
    CSL_Queue_Mgmt_Region_RegsOvly      queueMgmtRgnBase;   /**< Base Address (virtual) of queue management region.*/
    CSL_Queue_Mgmt_Region_RegsOvly      queueProxyRgnBase;  /**< Base Address (virtual) of queue proxy region.*/
    CSL_Queue_Status_Region_RegsOvly    queueStatusRgnBase; /**< Base Address (virtual) of queue status region.*/
    Uint32 LinkingRAM0Base;   /**< Base address (virtual) of Linking RAM 0.
                              Pass 0 to let Queue manager allocate itself. In this case Linking RAM 1 is unused. */
    Uint32 LinkingRAM0Size;   /**< Base address (virtual) of Linking RAM 0.
                              Pass 0 to let Queue manager allocate itself. In this case Linking RAM 1 is unused. */
    Uint32 LinkingRAM1Base;   /**< Base address (virtual) of Linking RAM 1.
                              Pass 0 to let Queue manager allocate itself. Linking RAM 1 size depends on number
                              of descriptors and Linking RAM 0 size. */
    Cppi4DescReg descRegion[PAL_CPPI41_MAX_DESC_REGIONS]; /**< Describes the descriptor regions to be initialized for the queue manager. <BR> <BR>

                                                     Descripor region can be either on-chip or off-chip. Hardware imposes a
                                                     strict ascending order requirement for region bases. Please be aware of this fact
                                                     when filling in this structure. For example, this array should mirror the order of
                                                     location of off-chip and on-chip memory in the SoC memory map. <BR> <BR>

                                                     It is assumed that all off-chip regions are located together in a chunk.
                                                     Ie, [on-chip][off-chip][on-chip][off-chip] region order is disallowed, while
                                                     [on-chip][off-chip][on-chip][on-chip] is legal. This should not be a crippling
                                                     restriction since (most likely) the hardware would be designed the same way (all
                                                     SDRAM together). <BR> <BR>

                                                     All off-chip memory will be allocated by this API. Any base address initialization
                                                     in this case will be ignored. For off-chip descriptor regions, to limit memory
                                                     wastage due to aligment requriments, this structure uses that fact that bit 14 is
                                                     aligned to bit 15 and so on. So, keep off-chip descriptor sizes as <B> powers of 2 </B>
                                                     and always configure the <B> highest descriptor size to the fist element of the
                                                     off-chip part of the array </B> <BR> <BR>*/
    Uint32 basefdQNum;                               /**< The base Free descriptor queue number. </B> Note: </B> This is a hardware constant, not a user
                                                      configuration. This number should be copied from hardware spec and passed here as-is. Keeping this
                                                      here to make the PAL CPPI code h/w abstracted. */
    Uint32 basefdbQNum;                              /**< The base Free descriptor/buffer queue number. </B> Note: </B> This is a hardware constant, not a user
                                                      configuration. This number should be copied from hardware spec and passed here as-is. Keeping it
                                                      here to make the PAL CPPI code h/w abstracted. */
    Uint32 totalQNum;

} Cppi4QueueMgrCfg;

/**
 * \brief Accumulator list mode configuration
 *
 * In the list mode accumulator pops a given queue and populates a list in memory
 * with the queue elements. Once done it raises an interrupt based on the pacing criteria.
 *
 * This structure defines the accumulator channel configuration when it is used
 * in the list mode. This structure is embedded as part of the ::Cppi4AccumulatorCfg
 * structure.
 */
typedef struct
{
    Ptr     listBase;       /**< The descriptor list base. */
    Uint32  maxPageEntry;   /**< Maximum number of entries per page */
    Uint32  pacingMode;     /**< Interrupt pacing mode: <BR>
                                    0 => None. Interrupt on entry threshold count. <BR>
                                    1 => Time delay since last interrupt. <BR>
                                    2 => delay since first new packet <BR>
                                    3 => delay since last new packet */
    Uint32  stallAvoidance; /**< 0 => Do not hold off interrupts to avoid a stall. <BR>
                                 1 => Hold off passing the final list page to the host when the host is congested <BR>
                                      and additional packet descriptors can be appended to the current list. */
    Uint32  listCountMode;  /**< 0 => NULL Terminate Mode - The last list entry is used to store a NULL pointer record <BR>
                                      (NULL terminator) to mark the end of list. <BR>
                                 1 => Entry Count Mode - The first list entry is used to store the total list entry count <BR>
                                      (not including the length entry). */
    Uint32 listEntrySize;   /**< Type of descriptor information required in the list. <BR>
                                 0 => Reg D only (4 bytes), <BR>
                                 1 => Reg C & D (8 bytes) <BR>
                                 2 => Reg A, B, C, D */
    Uint32 maxPageCnt;      /**< Number of ping/pong pages. Valid values 2 or 3 */

} Cppi4AccListCfg;

/**
 * \brief Accumulator monitor mode configuration
 *
 * In the monitor mode accumulator only monitors a given queue to see if it has elements enqueued.
 * If the queue has reached the given element count it raises an interrupt based on the pacing criteria.
 *
 * This structure defines the accumulator channel configuration when it is used
 * in monitor mode. This structure is embedded as part of the ::Cppi4AccumulatorCfg
 * structure.
 */
typedef struct
{
    Uint32  pktCountThresh; /**< The packet count threshold */
    Uint32  pacingMode;     /**< Interrupt pacing mode: <BR>
                                    0 => None. Interrupt on entry threshold count. <BR>
                                    1 => Time delay since count threshold reached. <BR> */
} Cppi4AccMonitorCfg;

/**
 * \brief Accumulator channel configuration
 *
 * This defines the accumulator channel properties for a given host mode Tx/Rx channel
 * An instance of this structure will be part of the channel initial configuration
 * structure. This structure makes sense only for host mode channels.
 */
typedef struct
{
    Uint32  accChanNum;         /**< Accumulator channel number */
    Uint32  mode;               /**<    0 => list mode
                                    1 => monitor mode */
    Cppi4Queue queue;           /**< The queue to monitor */

    Uint32  pacingTickCnt;      /**< Number of 25us timer ticks to delay interrupt */

    Cppi4AccListCfg   list;     /**< Normal mode configuration */
    Cppi4AccMonitorCfg  monitor;/**< Monitor mode configuration */

} Cppi4AccumulatorCfg;

/**
 * \brief Accumulator channel rate limit configuration
 *
 * This defines the accumulator channel properties for a given host mode Tx/Rx channel
 * An instance of this structure will be part of the channel initial configuration
 * structure. This structure makes sense only for host mode channels.
 */
typedef struct
{
    Uint32  accChanNum;         /**< Accumulator channel number */
    Uint16  retQ;            /**< Host FD Return Queue */
    Uint16  upperThreshold;  /**< Infra DMA OutQ threshold from which we start dropping packets */
    Uint16  lowerThreshold;  /**< Infra DMA OutQ threshold from which we stop dropping packets */
    Uint16  reserved;
} Cppi4AccumulatorRateLimitCfg;

/**
 *  \brief TX Channel initial configuration Structure
 *
 *  Must be allocated and filled by the caller of PAL_cppi4TxChOpen().
 *
 *  \note The queues that feed into the Tx channel are fixed at SoC design time.
 *  The destination queue number (the queue where Tx completed packets are fed)
 *  is contained in the descriptor itself. For this field, make sure what is supplied
 *  here is same as what is contained in the descriptor.
 *
 *  There is no API to re-configure the channel. The existing open channel should be closed
 *  and the same channel with changed parameters should be opened.
 *
 */

typedef struct
{
    Uint32 chNum;               /**< Channel number */
    Uint32 dmaNum;              /**< DMA block to which this channel belongs */
    Int defDescType;            /**< 0 = Embedded, 1 = Host, 2 = Monolithic.
                                     Describes which queue configuration is
                                     used for free desc and/or buffers */
    Cppi4Queue tdQueue;         /**< The teardown descriptor on teardown completion will be queued to this
                                     queue. Ideally this should be same as what is contained
                                     in the destination queue field of the Tx descriptor (So that a single queue
                                     can be polled to reap the completed packets as well as the teardown desc */
    Cppi4Queue txInQueue[4];    /**< List of queues (4 max) from which the channel can feed from. This is NOT used by CPPI 4.1
                                     implementation. Held here for informational purposes (and possible use by drivers) */
    Cppi4Queue txCompQueue;     /**< The Tx completion queue */
} Cppi4TxChInitCfg;

/**
 *  \brief RX Channel Embedded packet configuratiion Structure
 *
 *  An instance of this structure forms part of the Rx channel information structure.
 */
typedef struct
{
    Cppi4Queue fdQueue;           /**< Free Desc queue.*/
    Uint32 numBufSlot;            /**< Number of buffer slots in the descriptor */
    Uint32 sopSlotNum;            /**< SOP buffer slot number. 0 = Slot 0 etc */
    Cppi4BufPool fBufPool[4];     /**< Free Buffer pool.  Element 0 used for 1st Rx buffer, etc. */

} Cppi4RxChEmbeddedPktCfg;

/**
 *  \brief RX Channel Host packet configuratiion Structure
 *
 *  An instance of this structure forms part of the Rx channel information structure.
 */
typedef struct
{
    Cppi4Queue fdbQueue[4];     /**< Free Desc/Buffer queue.  Element 0 used for 1st Rx buffer, etc. */

} Cppi4RxChHostPktCfg;

/**
 *  \brief RX Channel monolithic packet configuratiion Structure
 *
 *  An instance of this structure forms part of the Rx channel information structure.
 */
typedef struct
{
  Cppi4Queue fdQueue;           /**< Free Desc queue */
  Uint32 sopOffset;             /**< Number of bytes to skip before writing payload */

} Cppi4RxChMonolithicPktCfg;

/**
 *  \brief Buffer pool configuration Structure
 *
 *  This structure defines the buffer pool hardware parameters
 */
typedef struct
{
    Uint32 refCntEnable;
    Uint32 bufferCount;
    Uint32 bufferSize;
}Cppi4BufInitCfg;

/**
 *  \brief RX Channel Information Structure
 *
 *  Must be allocated and filled by the caller of PAL_cppi4RxChOpen().
 *
 *  The same channel can be configured to receive different descripor type packets (not simaltaneously).
 *  When the Rx packets on a port need to be sent to the SR, the channels default desc type is set to "Embedded"
 *  and the Rx completion queue is set to the queue which PDSP polls for input packets.
 *  When in SR bypass mode, the same channel's default desc type will be set to "Host" and the rx completion
 *  queue set to one of the queues which host can get interrupted on (via the Queuing proxy/accumulator).
 *  In this example, the embedded mode configuration fetches free desc from the Free desc queue (as defined
 *  by #Cppi4RxChEmbeddedPktCfg) and host mode configuration fetches free desc/buffers from the free desc/buffer
 *  queue (as defined by #Cppi4RxChHostPktCfg).
 *
 *  There is no API to re-configure the channel. The existing open channel should be closed and the same channel
 *  with changed parameters should be opened.
 *
 * \note There seems to be no separate configuration for teardown complete descriptor. The assumption is #rxCompQueue
 * tuple is used for this purpose as well.
 *
 */
typedef struct
{
  Int chNum;                                        /**< Channel number */
  Uint32 dmaNum;                                    /** DMA block to which this channel belongs */
  Int defDescType;                                  /**< 0 = Embedded, 1 = Host, 2 = Monolithic. Describes which queue configuration is used for free desc and/or buffers */
  Int sopOffset;                                    /**< Num bytes to skip in SOP buffer before writing payload */
  Bool retryOnStarvation;                           /**< 0 => Drop packet on Desc/Buffer starvartion, 1 => DMA retries FIFO block transfer at later time */
  Cppi4Queue rxCompQueue;                           /**< Rx complete packets queue */
  union {
      Cppi4RxChHostPktCfg hostPktCfg;               /**< Host packet configuration. This defines which channel picks free desc/buffers from. */
      Cppi4RxChEmbeddedPktCfg embeddedPktCfg;       /**< Embedded packet configuration. This defines which channel picks free desc/buffers from. */
      Cppi4RxChMonolithicPktCfg monolithicPktCfg;   /**< Monolithic packet configuration. This defines which channel picks free desc/buffers from. */
  } u;                                              /**< Union of packet configuration structures to be filled in depending on the "defDescType" field. */

} Cppi4RxChInitCfg;

/**
 * \brief APDSP Information structure
 * Contains information on the APDSP.
 */

typedef struct {
    Uint32* firmwareBase;       /**< The base address of the firmware to be downloaded. */
    Uint32  firmwareSize;       /**< The size of the firmware to be downloaded */
    volatile Uint32* iRamBase;           /**< The base address of instruction RAM where firmware needs to be downloaded */
    volatile Uint32* pdspCtrlBase;       /**< APDSP control base */
    APDSP_Command_Status_RegsOvly pdspCmdBase; /**< APDSP command and status base */
} APDSPCfg;

#define PDSP_CONTROL_NRESET         0x0001
#define PDSP_CONTROL_ENABLE         0x0002

/**
 *  \brief CPPI4 Scheduler table
 */
typedef struct
{
    Uint32 numEntries;                                  /**< Number of actual entries */
    Uint8 entry[256];                         /**< Channel number and Tx/Rx info for each channel. */
}Cppi4SchedTable;


#if PUMA7_OR_NEWER_SOC_TYPE
typedef struct
{
    Uint8   schedEnable;
    Uint8   noRecycleOnTx;
    Uint8   channelArbitrationMode;
    Uint8   rxTxArbitrationMode;
    Uint32  portNum;
}CppiDmaSchedParams_t;

#define PAL_CPPI_RAL_QSMON_BITMAP_OFFSET        0
#define PAL_CPPI_RAL_QSMON_BITMAP_MASK          0x3F
#define PAL_CPPI_RAL_BYPASS_OFFSET              8
#define PAL_CPPI_RAL_BYPASS_MASK                0x1

#endif


/**
 *  \brief CPPI4 DMA configuration
 *
 *  Configuration information for CPPI DMA functionality. Includes the Global configuration, Channel
 *  configuration and the Scheduler configuration.
 */
typedef struct
{
    CSL_DMA_Global_Ctrl_RegsOvly            globalCtrlBase;     /**< Base address of Global Control registers */
    CSL_DMA_Channel_Ctrl_Status_RegsOvly    chCtrlStatusBase;   /**< Base address of Channel Control/Status registers */
    CSL_DMA_Sched_Control_RegsOvly          schedCtrlBase;      /**< Base address of Scheduler control register */
    CSL_DMA_Sched_Table_RegsOvly            schedTableBase;     /**< Base address of Scheduler table register */
    Cppi4Queue tdFQueue;                                        /**< Teardown free descriptor Queue */
    Cppi4SchedTable                         schedTable;         /**< Scheduler table to be initialized */
#if PUMA7_OR_NEWER_SOC_TYPE
    CppiDmaSchedParams_t                    schedParams;
    CSL_DMA_RAL_Control_RegsOvly            ralCtrlBase;
#endif
}Cppi4DMABlock;

/**
 *  \brief CPPI4 Init Configuration
 *
 *  Configuration information provided to PAL layer during initialization.
 *  The config info can come from various sources - static compiled in info,
 *  boot time (ENV, Flash) info etc.
 */
typedef struct
{
  Uint32 resetLine;                                         /**< NWSS Reset Line Number */
  Cppi4DMABlock dmaBlock[PAL_CPPI41_NUM_DMA_BLOCK];         /**< DMA block configuration */
  Cppi4QueueMgrCfg queueMgrInfo[PAL_CPPI41_NUM_QUEUE_MGR];  /**< Queue manager info for all Queue Managers in CPPI 4.1 system */
  CSL_BufMgr_RegsOvly bufMgrBase[PAL_CPPI41_NUM_BUF_MGR];   /**< Buffer manager info for all Buffer managers in CPPI 4.1 system */
#if PUMA7_OR_NEWER_SOC_TYPE
  CSL_QSMon_RegsOvly qsmon[PAL_CPPI_PP_NUM_QSMON_MGRs];
#else
  APDSPCfg apdspInfo;                                       /**< Accumulator PDSP configuration information */
#endif
  Ptr                   (*appSpecificMemAllocFunc)(Uint32 size);
  PAL_Result            (*debugToolBind)    (Ptr hnd, Ptr param);

} Cppi4InitCfg;


/*
 *  The CPPI4 PAL object internals are hidden from the upper layers. Hence the
 *  internal data structure for Cppi4PALObj is in the PAL internal header and the
 *  upper layers interpret PAL_Handle as a Ptr.  All PAL files must be compiled
 *  with the switch "CPPI4_PAL" so that the PAL can interpret a PAL_Handle as a
 *  data structure.
 */
/** \brief The PAL layer handle */
typedef void* PAL_Handle;
/** \brief The Open accumulator channel handle */
typedef void* PAL_Cppi4AccChHnd;
/** \brief The Open Tx channel handle */
typedef void* PAL_Cppi4TxChHnd;
/** \brief The Open Rx channel handle */
typedef void* PAL_Cppi4RxChHnd;
/** \brief The Open Queue handle */
typedef void* PAL_Cppi4QueueHnd;
/** \brief The PAL CPPI4 Buffer descriptor type */
typedef Uint32 PAL_Cppi4BD;


/*@}*/
/**
 *  \defgroup CPPI4_PAL_APIS CPPI4 PAL APIs
 */
/*@{*/


/**
 * \page API PAL CPPI 4.1 APIs
 * This page gives information on public APIs exposed by PAL CPPI4.1
 */


/************************ CPPI4 PAL External API Documentation ****************/

/**
 *  @name CPPI4.1 PAL Init/De-Init APIs
 */
/*@{*/

/**
 *  \brief PAL CPPI4.1 Initialization.
 *
 *  This function initializes the CPPI4.1 PAL layer.  This is the first function to be
 *  called by a module requiring CPPI4.1 support.  Multiple upper layers may call
 *  this function, although only one actual instance will be created.  After the
 *  first call, subsequent callers will simply be given the PAL Handle that was
 *  allocated during the first call.
 *
 *  In the typical scenerio, this API will be called once during system boot-up
 *  and other drivers/modules can call the API will NULL parameters to retrieve
 *  the PAL handle
 *
 *  The PAL handle that is returned must be used to close the PAL interface.
 *
 *  This function will bring the networking subsystem, out of reset, and
 *  initialize the queue manager/buffer manager and DMA hardware.
 *
 *  \note: The initCfg parameter values are only valid for the first caller - i.e.
 *  the CPPI4 PAL is initialized only once, regardless of the number of calls to
 *  this function.
 *
 *  @param  initCfg      [IN]      Pointer to CPPI4 config structure, to be
 *                                 allocated and loaded with values by the caller.
 *  @param  param        [IN]      Not used in this implementation. For future use.
 *
 *  @return a valid ::PAL_Handle on success, NULL otherwise.
 */
PAL_Handle PAL_cppi4Init (Cppi4InitCfg * initCfg, Ptr param);

/**
 *  \brief PAL CPPI4.1 de-initialization.
 *
 *  This function must be called by every upper layer module which calles
 *  PAL_cppi4Init().  During the usage cycle of the CPPI4 PAL, this is the last
 *  function called by the upper layer module.  When the last upper layer
 *  module calls this function, it will place the networking subsystem
 *  in reset, and cleanup all memory local to the CPPI4 PAL instance.
 *
 *  @note The caller should close all active channels prior to calling PAL_cppi4Exit().
 *  @note Implementations may skip calling this funtion if the CPPI4.1 subsystem is
 *  initialized during system boot-up and is expected to remain initialized for the uptime.
 *
 *  @param  hnd           [IN]      Handle to the PAL Layer.
 *  @param  param         [IN]      Not used in this implementation. For future use.
 *
 *  @return PAL_SOK on success, error code on failure.
 */
PAL_Result PAL_cppi4Exit (PAL_Handle hnd, Ptr param);

/*@}*/

#if PUMA7_OR_NEWER_SOC_TYPE
Bool PAL_cppi4IsBufPoolInitialized(PAL_Handle hnd, Cppi4BufPool pool);
Bool PAL_cppi4IsDescRegionAllocated(PAL_Handle hnd, Uint32 qMgr, Uint32 descRegion);
Ptr PAL_cppi4AllocDescRegion(PAL_Handle hnd, Uint32 qMgr, Uint32 descRegion);
/**
 *  \brief Get specific descriptor region base address
 *
 *  @param  hnd           [IN]      Handle to the PAL Layer.
 *  @param  descRegion    [IN]      the descriptor region
 *  @param  qMgr          [IN]      the descriptor region
 *  @param  Ptr           [OUT]     the region base address
 *  @param  length        [OUT]     region length
 *
 *  @return PAL_SOK on success
 */
PAL_Result PAL_cppi4GetDescRegionMemInfo(PAL_Handle hnd, PAL_CPPI_PP_DESC_REGIONs_e descRegion, Uint32 qMgr, Ptr *addr, Uint32 *length);
PAL_Result PAL_cppi4GetOffChipAddr(PAL_Handle hnd, Uint32 *physAddr, Uint32 *length);
PAL_Result PAL_cppi4GetOffChipBpoolAddr(PAL_Handle hnd, Uint32 pool, Uint32 *physAddr, Uint32 *length);

PAL_Result PAL_cppi4RalEnable(PAL_Handle hnd, Uint32 dmaNum, Uint32 qsmonBitmap);
PAL_Result PAL_cppi4RalDisable(PAL_Handle hnd, Uint32 dmaNum);
PAL_Result PAL_cppi4QSMonEnable(PAL_Handle hnd, PAL_CPPI_PP_QSMON_MGRs_e qsmonMgr, Uint32 thread, Cppi4Queue queue, Uint32 threshold, Bool isDown);
PAL_Result PAL_cppi4QSMonDisable(PAL_Handle hnd, PAL_CPPI_PP_QSMON_MGRs_e qsmonMgr, Uint32 thread);
PAL_Result PAL_cppi4QSMonGetQdepthWatermark(PAL_Handle hnd, PAL_CPPI_PP_QSMON_MGRs_e qsmonMgr, Uint32 thread, CSL_Reg32* watermark);
PAL_Result PAL_cppi4QSMonSetQdepthCnt(PAL_Handle hnd, PAL_CPPI_PP_QSMON_MGRs_e qsmonMgr, Uint32 thread, CSL_Reg32 count);

/**
 *  \brief Get buffer pool info for WiFi TX Zero Copy feature
 *
 *  @param  hnd           [IN]      Handle to the PAL Layer.
 *  @param  pool          [IN]      Buffer pool num.
 *  @param  bp_info       [OUT]     Buffer Pool Info structure
 *                        Ptr.
 *
 *  @return PAL_SOK on success
 */
PAL_Result PAL_cppi4GetBufferPoolInfo(PAL_Handle hnd, Cppi4BufPool pool, Cppi4BufPoolInfo* bp_info);

/**
 * Retrieve all cppi buffers pools information from cppi driver
 * on ARM
 *
 * @param pools array to fill in the pools info
 *
 * @return int 0 on success, non-zero value otherwise
 */
PAL_Result PAL_cppi4GetAllBufferPoolsInfo(PAL_Handle hnd, Cppi4BufPoolInfo *pools);


#else

/**
 *  @name CPPI4.1 Descriptor allocation and de-allocation APIs.
 */
/*@{*/

/**
 *  \brief PAL CPPI4.1 descriptor allocation.
 *
 *  This function allocates descriptors of a particular size and number. No partial
 *  allocation of any kind (size and number) is possible. The allocated descriptor lot
 *  will correspond to a separate descriptor region configured using the PAL_cppi4Init()
 *  call.
 *
 *  \note This function <B> only </B> allocates off-chip descriptors. Owners of on-chip
 *  descriptors are expected to be aware of where their region lies and should not
 *  need this allocation function.
 *
 *  @param  hnd        [IN]      PAL handle returned from a previous PAL_cppi4Init() call.
 *
 *  @param  qMgr       [IN]      The queue manager from whose region to allocate descriptors from.
 *
 *  @param  numDesc    [IN]      Number of descriptors required in the array.
 *
 *  @param  szDesc     [IN]      Size of each descriptors.
 *
 *  @return Pointer to a valid array of descriptors in case of a success and NULL otherwise.
 */
Ptr PAL_cppi4AllocDesc (PAL_Handle hnd, Uint32 qMgr, Uint32 numDesc, Uint32 szDesc);
#endif

/**
 *  \brief PAL CPPI4.1 descriptor de-allocation.
 *
 *  This function de-allocates descriptors allocated using the PAL_cppi4AllocDesc() call.
 *
 *  @param  hnd        [IN]      PAL handle returned from a previous PAL_cppi4Init call.
 *
 *  @param  qMgr       [IN]      The queue manager from whose region the descriptors were allocated.
 *
 *  @param  base       [IN]      Pointer to a valid array of descriptors. (The return value of a
 *                               previous successful PAL_cppi4AllocDesc() call).
 *
 *  @return PAL_SOK on success, error otherwise
 */
PAL_Result PAL_cppi4DeallocDesc (PAL_Handle hnd, Uint32 qMgr, Ptr base);

/*@}*/

/**
 *  @name CPPI4.1 Buffer management APIs.
 */
/*@{*/

/**
 *  \brief PAL CPPI4.1 buffer pool initialization.
 *
 *  Initialze a given buffer pool.
 *
 *  @param  hnd        [IN]      PAL handle returned from a previous PAL_cppi4Init() call.
 *
 *  @param  pool       [IN]      The buffer pool to initialize.
 *
 *  @param  refCntEnable [IN]    Enable reference counting on this pool.
 *
 *  @param  bufSize    [IN]      The size of each buffer in pool.
 *
 *  @param  numBuf     [IN]      Number of buffers in the pool.
 *
 *  @return Pointer to buffer pool allocated, NULL otherwise.
 */
Ptr PAL_cppi4BufPoolInit (PAL_Handle hnd, Cppi4BufPool pool, Bool refCntEnable, Uint32 bufSize, Uint32 numBuf);
Ptr PAL_cppi4BufPoolDirectInit (PAL_Handle hnd, Cppi4BufPool pool, Bool refCntEnable, Uint32 bufSize, Uint32 numBuf, Ptr poolBase);

/**
 *  \brief PAL CPPI4.1 set buffers memory to given template
 *
 *  For each buffer set buffer memoryu for specific template.
 *
 *  @param  hnd        [IN]      PAL handle returned from a previous PAL_cppi4Init() call.
 *
 *  @param  dstPool    [IN]      The destination buffer pool to which buffer belonged.
 *
 *  @param  srcBuf     [IN]     Source buffer pointer template
 *
 *  @param  srcBufLen  [IN]     Source buffer template lenght
 *
 *  @return PAL_SOK
 */
PAL_Result PAL_cppi4BufPoolSetBuffers (PAL_Handle hnd, Cppi4BufPool dstPool, Ptr srcBuf, Uint32 srcBufLen);

/**
 *  \brief PAL CPPI4.1 Increment the reference count of buffer
 *
 *  Increment the reference count of the buffer.
 *
 *  @param  hnd        [IN]      PAL handle returned from a previous PAL_cppi4Init() call.
 *
 *  @param  pool       [IN]      The buffer pool to which buffer belonged.
 *
 *  @param  bufPtr     [IN]      Buffer pointer whose reference count needs to be increment.
 *
 *  @return PAL_SOK
 */
PAL_Result PAL_cppi4BufIncRefCnt (PAL_Handle hnd, Cppi4BufPool pool, Ptr bufPtr);

/**
 *  \brief PAL CPPI4.1 Decrement the reference count of buffer
 *
 *  Decrement the reference count of the buffer.
 *
 *  @param  hnd        [IN]      PAL handle returned from a previous PAL_cppi4Init() call.
 *
 *  @param  pool       [IN]      The buffer pool to which buffer belonged.
 *
 *  @param  bufPtr     [IN]      Buffer pointer whose reference count needs to be decremented
 *
 *  @return PAL_SOK
 */
PAL_Result PAL_cppi4BufDecRefCnt (PAL_Handle hnd, Cppi4BufPool pool, Ptr bufPtr);

/**
 *  \brief PAL CPPI4.1 Pop buffer and Increment reference count
 *
 *  Pop buffer from buffer pool and Increment reference count.
 *
 *  @param  hnd        [IN]      PAL handle returned from a previous PAL_cppi4Init() call.
 *
 *  @param  pool       [IN]      The buffer pool to which buffer belonged.
 *
 *  @return Buffer point, NULL is pool is empty
 */
Ptr PAL_cppi4BufPopBuf (PAL_Handle hnd, Cppi4BufPool pool);

/**
 *  \brief PAL CPPI4.1 buffer pool de-initialization.
 *
 *  De-Initialze a given buffer pool.
 *
 *  @param  hnd        [IN]      PAL handle returned from a previous PAL_cppi4Init() call.
 *
 *  @param  pool       [IN]      The buffer pool to de-initialize.
 *
 *  @return PAL_SOK on success, error otherwise.
 */
PAL_Result PAL_cppi4BufPoolDestroy (PAL_Handle hnd, Cppi4BufPool pool);

/*@}*/

/**
 *  @name CPPI4.1 Performance/Status monitoring APIs
 */
/*@{*/

/**
 *  \brief PAL CPPI4.1 control API.
 *
 *  This function provides the capability for control operations to be performed
 *  on the PAL CPPI4 module.
 *
 *  The command (cmd) is used to direct the function to perform one or more of
 *  the supported operations.  The commands supported are:
 *
 *  - #PAL_CPPI41_IOCTL_GET_SWVER
 *          - cmdArg is a ptr to an integer that will be written with the s/w version id,
 *          and #param is a double ptr to a string which will point the the static version
 *          text string.
 *  - #PAL_CPPI41_IOCTL_GET_HWVER
 *          - cmdArg is a ptr to an integer that will be written with the hardware rev.
 *  - #PAL_CPPI41_IOCTL_GET_FDQ_STARVE_CNT
 *          - cmdArg is a pointer to the ::Cppi4Queue structure which defines the base free
 *          descriptor queue for which the starvation count (since last read) needs to be retrieved.
 *          <BR><B>Important Note:</B> Due to the way hardware behaves, the queue count of four
 *          queues is retrieved together. So passing 0 for queue number retrieves the count of
 *          queues 0-3, passing 4 gets the count of queues 4-7 and so on.
 *          The count is returned as a single 32-bit integer, with 4 octets giving count of each
 *          of the four queue. LSB gives count of lowered number queue and MSB gives count of the
 *          highest numbered queue.
 *  - #PAL_CPPI41_IOCTL_GET_FDBQ_STARVE_CNT
 *          - cmdArg is a pointer to the ::Cppi4Queue structure which defines the base free
 *          descriptor/buffer queue whose starvation count (since last read) needs to be
 *          retrieved. param provides pointer to integer where the starvation count is returned.
 *          <BR><B>Important Note:</B> Due to the way hardware behaves, the queue count of four
 *          queues is retrieved together. So passing 0 for queue number retrieves the count of
 *          queues 0-3, passing 4 gets the count of queues 4-7 and so on.
 *          The count is returned as a single 32-bit integer, with 4 octets giving count of each
 *          of the four queue. LSB gives count of lowered number queue and MSB gives count of
 *          the highest numbered queue.
 *  - #PAL_CPPI41_IOCTL_GET_QUEUE_PEND_STATUS
 *          - cmdArg is a pointer to the ::Cppi4Queue structure which defines the queue for which
 *          the pending status needs to be retrieved.
 *          param provides pointer to integer where the starvation count is returned.
 *  - #PAL_CPPI41_IOCTL_GET_QUEUE_ENTRY_COUNT
 *          - cmdArg is a pointer to the ::Cppi4Queue structure which defines the queue for which
 *          the entry count needs to be retrieved.
 *          param provides pointer to integer where the entry count is returned.
 *  - #PAL_CPPI41_IOCTL_GET_QUEUE_BYTE_COUNT
 *          - cmdArg is a pointer to the ::Cppi4Queue structure which defines the queue for which
 *          the byte count needs to be retrieved.
 *          param provides pointer to integer where the byte count is returned.
 *  - #PAL_CPPI41_IOCTL_GET_QUEUE_HEAD_PKT_SIZE
 *          - cmdArg is a pointer to the ::Cppi4Queue structure which defines the queue for which
 *          the head packet size needs to be retrieved.
 *          param provides pointer to integer where the head packet size is returned.
 *  - #PAL_CPPI41_IOCTL_QUEUE_DIVERT
 *          - cmdArg defines the source and destination queue. Also defines if packets
 *          should be added to head/tail of destination queue. This argument is a 32-bit integer
 *          and an exact map of the diversion register in the hardware. param provides the queue manager
 *          index to which the queue belongs.
 *  - #PAL_CPPI41_IOCTL_BUFMGR_SOFT_RESET
 *          - cmdArg provides the buffer manger index to soft-reset.
 *  - #PAL_CPPI41_IOCTL_BUF_REFCNT_INCR
 *          - cmdArg provides the buffer manager number, pool number, and the increment value.
 *          Bits [0-3]: increment value, bits [8-11]: buffer manager number, bits [16-20] pool number.
 *          param provides the buffer address (physical) whose reference count needs to be incremented.
 *
 *  @param  hnd           [IN]      Handle to the PAL Layer.
 *  @param  cmd           [IN]      Operation to be performed.
 *  @param  cmdArg    [IN/OUT]      Provides additional info for the operation.
 *  @param  param     [IN/OUT]      Cmd specific argument.
 *
 *  @return PAL_SOK on success, else failure code.
 */
PAL_Result PAL_cppi4Control (PAL_Handle hnd, Uint32 cmd, Ptr cmdArg, Ptr param);

/*@}*/

/**
 *  @name CPPI4.1 Accumulator Channel Management APIs
 */
/*@{*/


/**
 *  \brief PAL CPPI 4.1 accumulator channel setup.
 *
 * Sets up an accumulator channel to monitor a queue, this
 * function must accept the page as virtual address.
 *
 *  @param  hnd           [IN]      PAL handle
 *  @param  accCfg        [IN]      Pointer to the accumulator configuration structure.
 *
 *  @return pointer to a valid handle (::PAL_Cppi4AccChHnd) on success, NULL otherwise.
 */
PAL_Cppi4AccChHnd PAL_cppi4AccChOpen(PAL_Handle hnd, Cppi4AccumulatorCfg* accCfg);

/**
 *  \brief PAL CPPI 4.1 accumulator channel enable rate limit.
 *
 * Sets up an accumulator channel rate limit onfiguration
 * 
 *  @param  accCfg        [IN]      Pointer to the accumulator
 *                        rate limit configuration structure.
 *
 */
void PAL_cppi4AccChEnRateLimit(Cppi4AccumulatorRateLimitCfg* accCfg);

#if PUMA7_OR_NEWER_SOC_TYPE
/**
 *  \brief PAL CPPI 4.1 accumulator channel setup (for app-cpu).
 *
 * Sets up an accumulator channel to monitor a queue, this
 * function must accept the page as phyisical address.
 * This function is being used only by app-cpu vai hw mailbox.
 *
 *  @param  hnd           [IN]      PAL handle
 *  @param  accCfg        [IN]      Pointer to the accumulator configuration structure.
 *
 *  @return pointer to a valid handle (::PAL_Cppi4AccChHnd) on success, NULL otherwise.
 */
PAL_Cppi4AccChHnd PAL_cppi4AccChOpenAppCpu(PAL_Handle hnd, Cppi4AccumulatorCfg* accCfg);

/**
 *  \brief PAL CPPI 4.1 pdsp_cmd_send registration.
 *
 * Sets up the pdsp_cmd_send function callback used by PAL CPPI
 * driver.
 *
 *  @param  cb            [IN]      pdsp_cmd_send callback
 *                                  function
 *
 *  @return PAL_SOK on success, else failure code.
 */
PAL_Result PAL_cppi4PdspCmdSendRegister(Int32 (*cb)(pdsp_id_t, pdsp_cmd_t, void *, Uint32, void *, Uint32));

/**
 *  \brief PAL CPPI 4.1 pdsp_cmd_send unregistration.
 *
 * Unregisters the pdsp_cmd_send function callback used by PAL
 * CPPI driver.
 *
 *  @return PAL_SOK on success, else failure code.
 */
PAL_Result PAL_cppi4PdspCmdSendUnregister(void);

#endif

/**
 *  \brief PAL CPPI 4.1 accumulator channel teardown.
 *
 *  Stop an accumulator channel from monitoring a queue.
 *
 *  @param  hnd           [IN]      Accumulator handle (::PAL_Cppi4AccChHnd)
 *  @param  closeArgs     [IN]      For future extension. Pass NULL for now.
 *
 *  @return PAL_SOK on success, else failure code.
 */
PAL_Result PAL_cppi4AccChClose(PAL_Cppi4AccChHnd hnd, Ptr closeArgs);

/**
 *  \brief PAL CPPI 4.1 get transmit channel accumulator page.
 *
 *  This function returns pointer to the next page that the accumulator would have populated with
 *  BD list.
 *
 *  \note This function does not have information on whether the page it is returning has <B> really </B>
 *  been populated by accumulator. This information should be available to the driver using the
 *  INTD count register. It is the responsibility of the driver to call this function when it is sure that
 *  the accumulator is finished with updating a new page.
 *
 *  @param  hnd           [IN]      Acc channel handle (::PAL_Cppi4AccChHnd).
 *
 *  @return Valid pointer (virtual address) to list page on success, NULL otherwise.
 */
Ptr PAL_cppi4AccChGetNextList(PAL_Cppi4AccChHnd hnd);

/*@}*/

/**
 *  @name CPPI4.1 DMA Channel Management APIs
 */
/*@{*/

/**
 *  \brief PAL CPPI 4.1 transmit channel open.
 *
 *  This function configures and opens the specified Tx channel.  The caller is required
 *  to provide channel configuration information by initializing a ::Cppi4TxChInitCfg structure.
 *
 *  @param  hnd           [IN]      Handle to the PAL Layer.
 *  @param  cfg           [IN]      Pointer to TX channel configuration structure.
 *  @param  chOpenArgs    [IN]      For future use.
 *
 *  @return pointer to a valid ::PAL_Cppi4TxChHnd on success, NULL otherwise.
 */
PAL_Cppi4TxChHnd PAL_cppi4TxChOpen (PAL_Handle  hnd, Cppi4TxChInitCfg * cfg, Ptr chOpenArgs);

/**
 *  \brief PAL CPPI 4.1 receive channel open.
 *
 *  This function configures and opens the specified Rx channel. The caller is required
 *  to provide channel configuration information by initializing a ::Cppi4RxChInitCfg structure.
 *
 *  @param  hnd           [IN]      Handle to the PAL Layer.
 *  @param  cfg           [IN]      Pointer to RX channel configuration structure.
 *  @param  chOpenArgs    [IN]      For future use.
 *
 *  @return pointer to a valid ::PAL_Cppi4RxChHnd on success, NULL otherwise.
 */
PAL_Cppi4RxChHnd PAL_cppi4RxChOpen (PAL_Handle hnd, Cppi4RxChInitCfg * cfg, Ptr chOpenArgs);

/**
 *  \brief PAL CPPI 4.1 transmit channel enable.
 *
 *  This function enables a specified Tx channel.  The caller is required to provide
 *  channel handled returned to it via call to the PAL_cppi4TxChOpen() fuction earlier
 *
 *  After the successful completion of this function, the Tx DMA channel will be active
 *  and ready for data transmission.
 *
 *  @param  hnd           [IN]      Tx channel handle (::PAL_Cppi4TxChHnd).
 *  @param  chEnableArgs  [IN]      For future use.
 *
 *  @return PAL_SOK on success, error code otherwise.
 */
PAL_Result PAL_cppi4EnableTxChannel (PAL_Cppi4TxChHnd hnd, Ptr chEnableArgs);

/**
 *  \brief PAL CPPI 4.1 receive channel enable.
 *
 *  This function enables a specified Rx channel. The caller is required to provide
 *  channel handled returned to it via call to the PAL_cppi4RxChOpen() fuction earlier
 *
 *  After the successful completion of this function, the Rx DMA channel will be active
 *  and ready for data reception.
 *
 *  @param  hnd           [IN]      Rx channel handle (::PAL_Cppi4RxChHnd).
 *  @param  chEnableArgs  [IN]      For future use.
 *
 *  @return PAL_SOK on success, error code otherwise.
 */
PAL_Result PAL_cppi4EnableRxChannel (PAL_Cppi4RxChHnd hnd, Ptr chEnableArgs);


/**
 *  \brief PAL CPPI 4.1 Successully teardown the channel without using teardown descriptor
 *
 *  This function looks for the teardown status bit of channel
 *  and returns once the teardown is complete.
 *
 *  Returns when the channel teardown is complete.
 *  Used when Teardown descriptor is forwarded back to Free teardown queue.
 *  Mostly used in case of Teardown of Embedded endpoint Channels and Infra mode channels
 *
 *
 *  \see PAL_cppi4GetTdInfo() for teardown completion processing.
 *
 *  @param  hnd           [IN]      Tx channel handle (::PAL_Cppi4TxChHnd).
 *  @param  chCloseArgs   [IN]      For future use.
 *
 *  @return PAL_SOK on success, error code otherwise.
 */
PAL_Result PAL_cppi4TxChStatus (PAL_Cppi4TxChHnd hnd, Ptr chCloseArgs);

/**
 *  \brief PAL CPPI 4.1 transmit channel close.
 *
 *  This function closes the given Tx channel.
 *
 *  This function triggers the teardown of the channel.
 *
 *  \attention Channel disable should not be called before the teardown is completed
 *  as a disable will stop the DMA scheduling on the channel resulting in the teardown
 *  complete event not being registered at all.
 *
 *  \note A successful channel close event (actually a teardown event) is reported
 *  via queueing of a teardown descriptor.
 *
 *  This function just sets up for the closure of the channel and returns. The
 *  caller must detect the channel close event to assume that the channel is
 *  closed.
 *
 *  \see PAL_cppi4GetTdInfo() for teardown completion processing.
 *
 *  @param  hnd           [IN]      Tx channel handle (::PAL_Cppi4TxChHnd).
 *  @param  chCloseArgs   [IN]      For future use.
 *
 *  @return PAL_SOK on success, error code otherwise.
 */
PAL_Result PAL_cppi4TxChClose (PAL_Cppi4TxChHnd hnd, Ptr chCloseArgs);

/**
 *  \brief PAL CPPI 4.1 transmit channel destroy.
 *
 *  This function destroys the given Tx channel.
 *
 *  This function frees up the resources associated with the
 *  channel. Accessing the channel handle after this function call
 *  is illegal. This should be the last function to be called on an open channel.
 *
 *  @param  hnd           [IN]      Tx channel handle (::PAL_Cppi4TxChHnd).
 *  @param  chCloseArgs   [IN]      For future use.
 *
 *  @return PAL_SOK on success, error code otherwise.
 */
PAL_Result PAL_cppi4TxChDestroy (PAL_Cppi4TxChHnd hnd, Ptr chCloseArgs);


/**
 *  \brief PAL CPPI 4.1 Successully teardown the channel without using teardown descriptor
 *
 *  This function looks for the teardown status bit of channel
 *  and returns once the teardown is complete.
 *
 *  Returns when the channel teardown is complete.
 *  Used when Teardown descriptor is forwarded back to Free teardown queue.
 *  Mostly used in case of Teardown of Embedded endpoint Channels and Infra mode channels
 *
 *
 *  \see PAL_cppi4GetTdInfo() for teardown completion processing.
 *
 *  @param  hnd           [IN]      Tx channel handle (::PAL_Cppi4TxChHnd).
 *  @param  chCloseArgs   [IN]      For future use.
 *
 *  @return PAL_SOK on success, error code otherwise.
 */
PAL_Result PAL_cppi4RxChStatus (PAL_Cppi4RxChHnd hnd, Ptr chCloseArgs);
/**
 *  \brief PAL CPPI 4.1 receive channel close.
 *
 *  This function closes the given Rx channel.
 *
 *  \attention Channel disable should not be called before the teardown is completed
 *  as a disable will stop the DMA scheduling on the channel resulting in the teardown
 *  complete event not being registered at all.
 *
 *  \note A successful channel close event (actually a teardown event) is reported
 *  via queueing of a teardown descriptor.
 *
 *  This function just sets up for the closure of the channel and returns. The
 *  caller must detect the channel close event to assume that the channel is
 *  closed.
 *
 *  \see PAL_cppi4GetTdInfo for teardown completion processing.
 *
 *  @param  hnd           [IN]      Rx channel handle (::PAL_Cppi4RxChHnd).
 *  @param  chCloseArgs   [IN]      For future use.
 *
 *  @return PAL_SOK on success, error code otherwise.
 */
PAL_Result PAL_cppi4RxChClose (PAL_Cppi4RxChHnd hnd, Ptr chCloseArgs);

/**
 *  \brief PAL CPPI 4.1 receive channel destroy.
 *
 *  This function destroys the given Rx channel.
 *
 *  This function frees up the resources associated with the
 *  channel. Accessing the channel handle after this function call
 *  is illegal. This should be the last function to be called on an open channel.
 *
 *  @param  hnd           [IN]      Rx channel handle (::PAL_Cppi4RxChHnd).
 *  @param  chCloseArgs   [IN]      For future use.
 *
 *  @return PAL_SOK on success, error code otherwise.
 */
PAL_Result PAL_cppi4RxChDestroy (PAL_Cppi4RxChHnd hnd, Ptr chCloseArgs);

/**
 *  \brief PAL CPPI4.1 transmit channel disable.
 *
 *  This function disables a specific Tx channel. The caller is required to provide
 *  channel handle returned to it via call to the PAL_cppi4TxChOpen() fuction earlier
 *
 *  After the successful completion of this function, the Tx DMA channel will
 *  be deactived.
 *
 *  @param  hnd           [IN]      Tx channel handle (::PAL_Cppi4TxChHnd).
 *  @param  chDisableArgs [IN]      For future use.
 *
 *  @return PAL_SOK on success, error code otherwise.
 */
PAL_Result PAL_cppi4DisableTxChannel (PAL_Cppi4TxChHnd hnd, Ptr chDisableArgs);

/**
 *  \brief PAL CPPI4.1 receive channel disable.
 *
 *  This function disables specified Rx channel.  The caller is required to provide
 *  channel handle returned to it via call to the PAL_cppi4RxChOpen() fuction earlier
 *
 *  After the successful completion of this function, the Rx DMA channel will
 *  be deactivated.
 *
 *  @param  hnd           [IN]      Rx channel handle (::PAL_Cppi4RxChHnd).
 *  @param  chDisableArgs [IN]      For future use.
 *
 *  @return PAL_SOK on success, error code otherwise.
 */
PAL_Result PAL_cppi4DisableRxChannel (PAL_Cppi4RxChHnd hnd, Ptr chDisableArgs);

/*@}*/

/**
 *  @name CPPI4.1 Queue Management APIs
 */
/*@{*/

/**
 *  \brief PAL CPPI4.1 Queue Open
 *
 *  This function configures and opens a specified Queue.  Multiple upper
 *  layers may call  this function, although only one actual instance will
 *  be created.  After the first call, subsequent callers will simply be
 *  given the Queue Handle that was allocated during the first call.
 *
 *  @param  hnd           [IN]      Handle to the PAL Layer.
 *  @param  queue         [IN]      The queue to open. (pointer to ::Cppi4Queue)
 *
 *  @return a valid ::PAL_Cppi4QueueHnd on success and NULL on failure
 */
PAL_Cppi4QueueHnd PAL_cppi4QueueOpen (PAL_Handle hnd, Cppi4Queue  queue);


/*
 *  PAL_cppi4QueueOpenNoReset
 *  - Opens a CPPI4 queue for use without resetting it.
 *  - The handle returned should be used for all push and pop
 *  operations on the queue.
 */
PAL_Cppi4QueueHnd PAL_cppi4QueueOpenNoReset (PAL_Handle hnd, Cppi4Queue  queue);

/**
 *  \brief CPPI4.1 Queue Close
 *
 *  This function closes a specified Queue when the last time it is called.
 *  For every other call it decrements the reference to the queue object.
 *
 *  @param  hnd           [IN]      Handle to the PAL Layer.
 *  @param  qHnd          [IN]      Handle to the Queue (obtained from a successful
 *                                  call to PAL_cppi4QueueOpen()).
 *
 *  @return PAL_SOK on success or failure code otherwise <BR>
 */
PAL_Result PAL_cppi4QueueClose (PAL_Handle hnd, PAL_Cppi4QueueHnd qHnd);

/**
 *  \brief CPPI4.1 Queue Push
 *
 *  This function is called to queue a descriptor onto a queue.
 *
 *  @param  hnd           [IN]      Handle to the Queue (::PAL_Cppi4QueueHnd).
 *  @param  dAddr         [IN]      descriptor physical address.
 *  @param  dSize         [IN]      descriptor size.
 *  @param  pSize         [IN]      packet size.
 *
 *  \note pSize parameter is optional. Pass NULL in case not required.
 *
 *  @return a valid ::PAL_Cppi4QueueHnd on success and NULL on failure
 */
PAL_Result PAL_cppi4QueuePush (PAL_Cppi4QueueHnd hnd, Ptr dAddr, Uint32 dSize, Uint32 pSize);

/**
 *  \brief CPPI4.1 Queue Push without size
 *
 *  This function is called to queue a descriptor onto a queue.
 *
 *  @param  hnd           [IN]      Handle to the Queue (::PAL_Cppi4QueueHnd).
 *  @param  dAddr         [IN]      descriptor physical address.
 *  @param  dSize         [IN]      descriptor size.
 *  @param  pSize         [IN]      packet size. (not used)
 *
 *  \note pSize parameter is optional. Pass NULL in case not required.
 *
 *  @return a valid ::PAL_Cppi4QueueHnd on success and NULL on failure
 */
PAL_Result PAL_cppi4QueuePushDirect (PAL_Cppi4QueueHnd hnd, Ptr dAddr, Uint32 dSize, Uint32 pSize);

/**
 *  \brief CPPI4.1 Queue Pop
 *
 *  This function is called to get a single descriptor after a queue pop.
 *
 *  @param  hnd           [IN]      Handle to the Queue (::PAL_Cppi4QueueHnd).
 *
 *  @return Buffer descriptor physical address.
 */
PAL_Cppi4BD *PAL_cppi4QueuePop (PAL_Cppi4QueueHnd hnd);

/**
 *  \brief CPPI4.1 Queue Get Entries Count
 *
 *  This function is called to get the number of descriptors in
 *  a queue.
 *
 *  @param  hnd           [IN]      PAL handle returned from a
 *  							    previous PAL_cppi4Init()
 *  							    call.
 *  @param  queue         [IN]      Handle to the Queue
 *  					            (::PAL_Cppi4QueueHnd).
 *  @param  entryCount    [OUT]     Number of entries in queue.
 *
 *  @return PAL_SOK on succuss.
 */
PAL_Result PAL_cppi4QueueGetEntryCount (PAL_Handle hnd, Cppi4Queue queue, Uint32 *entryCount);


/**
 *  \brief CPPI4.1 Embedded Descriptor Recycle
 *
 *  This function is called to recycle embedded descriptor.
 *
 *  @param  hnd           [IN]      Handle to the PAL Layer.
 *  @param  vDesc         [IN]      descriptor virtual address.
 *  @param  physDesc      [IN]      descriptor physical address.
 *
 *  @return PAL_SOK on success or failure code otherwise <BR>
 */
PAL_Result PAL_cppi4EmbDescRecycle (PAL_Cppi4QueueHnd hnd, Ptr vDesc, Ptr physDesc);

/*@}*/

/**
 *  @name CPPI4.1 Misc APIs
 */
/*@{*/

/**
 *  \brief PAL CPPI4.1 teardown completion processing function
 *
 *  This function is called to complete the teardown processing on a channel.
 *  The function provides teardown information from the teardown descriptor
 *  passed to it. It also recycles the teardown descriptor back to the teardown
 *  free descriptor queue.
 *
 *  @param  hnd           [IN]      Handle to the PAL layer.
 *  @param  cppi4TdBD     [IN]      Virtual address of Teardown BD.
 *  @param  txRx          [OUT]     Channel type on which teardown occured (tx or rx).
 *  @param  dmaNum        [OUT]     The unique system-wide DMA block to which the channel belonged.
 *  @param  chanNum       [OUT]     The channel number.
 *
 *  @return PAL_SOK
 */
PAL_Result PAL_cppi4GetTdInfo(PAL_Handle hnd, PAL_Cppi4BD * cppi4TdBD, Bool* txRx, Uint32*  dmaNum, Uint32* chanNum);

/*@}*/
/*@}*/
/*@}*/


/* ================================================================================ */
#define PAL_CPPI41_DEBUG_BUFF_MAXLEN        512

typedef struct cppi_debug_config
{
    PAL_Handle      handle;

    Cppi4Queue      allocQueue;
    Cppi4BufPool    allocBuf;

    Cppi4Queue      outputQueue;

    Uint32          dataLen;
    Uint8           data[ PAL_CPPI41_DEBUG_BUFF_MAXLEN ];
}
pal_cppi_dbg_cfg_t;

#include <linux/seq_file.h>
#include <linux/fs.h>

extern const struct file_operations      PAL_cppi4dbgCfg_fops;
extern const struct file_operations      PAL_cppi4dbg_fops;
/* ================================================================================ */

extern const struct file_operations      PAL_cppi4PM_fops;

typedef struct
{
    Cppi4Queue          fdQueue;
    Uint32              qsmonThread;
    Uint32              qsmonThreshold;
    Bool                qsmonChEn;
    Bool                ralQEnable;
    Bool                direction;
} qsmonRal_t;

typedef enum
{
    PAL_CPPI_ALLOC_NONE,
    PAL_CPPI_ALLOC_CALLOC,
    PAL_CPPI_ALLOC_MALLOC,
    PAL_CPPI_ALLOC_SKB
}
FDqueueBuffAllocType_e;

typedef struct FDqueue
{
    Uint8                       descRegion;     /* Type: PAL_CPPI_PP_DESC_REGIONs_e */
    Uint8                       descType;       /* Type: CPPI41_DESC_TYPE */
    Uint16                      descCount;
    Uint16                      descSize;
    Uint16                      qId;
    Uint8                       qMgr;           /* Types: PAL_CPPI_PP_QMGRs_e, PAL_CPPI_DSG_QUEUE_MNGRS_e, ... */
    FDqueueBuffAllocType_e      allocType;
}
FDqueue_t;

#endif /* __PAL_CPPI4_H__ */

#endif /* __KERNEL__ */

