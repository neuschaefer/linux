/*
 * cslr_cppi4.h
 * Description:
 * See below.
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

/** \file   cslr_cppi4.h
    \brief  CPPI4 Buffer and Queue Manager CSL Header - Register Layer Abstraction

    Register layer abstraction of CPPI 4.0 Buffer/Queue manager peripheral 
    device.

    @author     Greg Guyotte
    @version    0.1
 */


#ifndef __CSLR_CPPI4_MGR_H__
#define __CSLR_CPPI4_MGR_H__

#include "csl_defs.h"

/**
 * \brief Queue Manager Peripheral Device Transmit Queues Register Layout structure
 *
 * The structure instance variable points to CPPI4 Transmit Queues
 * register space in  SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 *
 * Register naming comes directly from the spec names, with redundant words
 * dropped and abbreviations made where appropriate.
 */
typedef volatile struct
{
    CSL_Reg32 Queue[4];

} CSL_Tx_Ch_Push_Queue_Regs;

/**
 * \brief CPPI4 transmit queues register overlay pointer
 *
 * Can be used in PAL layer directly for performance considerations.
 */
typedef CSL_Tx_Ch_Push_Queue_Regs* CSL_Tx_PushQ_RegsOvly;

/**
 * \brief Queue Manager Peripheral Device Receive Queues Register Layout structure
 *
 * The structure instance variable points to CPPI4 Receive Queues
 * register space in  SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 *
 * Register naming comes directly from the spec names, with redundant words
 * dropped and abbreviations made where appropriate.
 */
typedef volatile struct
{
    CSL_Reg32 Queue[4];

} CSL_Rx_Ch_Pop_Queue_Regs;

/**
 * \brief CPPI4 receive queues register overlay pointer
 *
 * Can be used in PAL layer directly for performance considerations.
 */
typedef CSL_Rx_Ch_Pop_Queue_Regs* CSL_Rx_PopQ_RegsOvly;

/**
 * \brief Queue Manager Peripheral Device Free Buffer/Descrtiptor Queues Register Layout structure
 *
 * The structure instance variable points to CPPI4 Free Buffer/Descrtiptor Queues
 * register space in  SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 *
 * Register naming comes directly from the spec names, with redundant words
 * dropped and abbreviations made where appropriate.
 */
typedef volatile struct
{
    CSL_Reg32 Queue;
    CSL_Reg32 reserved;
    
} CSL_Rx_Free_Queue_Regs;

/**
 * \brief CPPI4 free buffer/descrtiptor queues register overlay pointer
 *
 * Can be used in PAL layer directly for performance considerations.
 */
typedef CSL_Rx_Free_Queue_Regs* CSL_Rx_FreeQ_RegsOvly;

/**
 * \brief Queue Manager Peripheral Device Register Layout structure
 *
 * The structure instance variable points to CPPI4 register space in
 * SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 *
 * Register naming comes directly from the spec names, with redundant words
 * dropped and abbreviations made where appropriate.
 */
typedef volatile struct  
{
  CSL_Reg32   Reserved[128];  /* 0x03068000 */

  /* Rx Free Descriptor / Buffer Queues Region */
  CSL_Rx_Free_Queue_Regs Rx_Free_Desc_Queue[4]; /* 0x03068200 */

  CSL_Reg32   Reserved6[888];

  /* Control Registers Region */
  CSL_Reg32   Tx_Completion_Queue_Int_Mask_Set; /* 0x03069000 */
  CSL_Reg32   Tx_Completion_Queue_Int_Mask_Clear;
  CSL_Reg32   Tx_Completion_Queue_Int_EOI;
  CSL_Reg32   Reserved7[13];
  CSL_Reg32   Rx_Queue_Int_Mask_Set;
  CSL_Reg32   Rx_Queue_Int_Mask_Clear;
  CSL_Reg32   Rx_Queue_Int_EOI;
  CSL_Reg32   Reserved8[1];
  CSL_Reg32   Rx_Free_Buffer_Desc_Starv_Count;
  CSL_Reg32   Reserved9[9];
  CSL_Reg32   Revision;
  CSL_Reg32   Soft_Reset;
  CSL_Reg32   Reserved10[4064];

  /* Tx Queues Region */
  /* 18 Tx channels, up to 4 priority levels. */ 
  CSL_Tx_Ch_Push_Queue_Regs Tx_Queue[18]; /* 0x0306D000 */

  /*
  CSL_Reg32   SAR_Ch_0_Pri_0_Tx_Queue;
  CSL_Reg32   SAR_Ch_0_Pri_1_Tx_Queue;
  CSL_Reg32   SAR_Ch_0_Pri_2_Tx_Queue;
  CSL_Reg32   SAR_Ch_0_Pri_3_Tx_Queue;
  CSL_Reg32   SAR_Ch_1_Pri_0_Tx_Queue;
  CSL_Reg32   Reserved10[3];
  CSL_Reg32   SAR_Ch_2_Pri_0_Tx_Queue;
  CSL_Reg32   Reserved11[3];
  CSL_Reg32   SAR_Ch_3_Pri_0_Tx_Queue;
  CSL_Reg32   Reserved12[3];
  CSL_Reg32   SAR_Ch_4_Pri_0_Tx_Queue;
  CSL_Reg32   Reserved13[3];
  CSL_Reg32   SAR_Ch_5_Pri_0_Tx_Queue;
  CSL_Reg32   Reserved14[3];
  CSL_Reg32   SAR_Ch_6_Pri_0_Tx_Queue;
  CSL_Reg32   Reserved15[3];
  CSL_Reg32   SAR_Ch_7_Pri_0_Tx_Queue;
  CSL_Reg32   Reserved16[3];
  CSL_Reg32   SAR_Ch_8_Pri_0_Tx_Queue;
  CSL_Reg32   Reserved17[3];
  CSL_Reg32   SAR_Ch_9_Pri_0_Tx_Queue;
  CSL_Reg32   Reserved18[3];
  CSL_Reg32   SAR_Ch_10_Pri_0_Tx_Queue;
  CSL_Reg32   Reserved19[3];
  CSL_Reg32   SAR_Ch_11_Pri_0_Tx_Queue;
  CSL_Reg32   Reserved20[3];
  CSL_Reg32   SAR_Ch_12_Pri_0_Tx_Queue;
  CSL_Reg32   Reserved21[3];
  CSL_Reg32   SAR_Ch_13_Pri_0_Tx_Queue;
  CSL_Reg32   Reserved22[3];
  CSL_Reg32   SAR_Ch_14_Pri_0_Tx_Queue;
  CSL_Reg32   Reserved23[3];
  CSL_Reg32   SAR_Ch_15_Pri_0_Tx_Queue;
  CSL_Reg32   Reserved24[3];
  CSL_Reg32   EMAC_0_Pri_0_Tx_Queue;
  CSL_Reg32   EMAC_0_Pri_1_Tx_Queue;
  CSL_Reg32   Reserved25[2];
  CSL_Reg32   EMAC_1_Pri_0_Tx_Queue;
  CSL_Reg32   EMAC_1_Pri_1_Tx_Queue;
  */
  CSL_Reg32   Reserved26[1976];

  /* Tx Completion Queues Region */
  CSL_Reg32   Tx_Completion_Queue[4]; /* 0x0306F000 */
  CSL_Reg32   Reserved27[508]; 

  /* Rx Queues Region */
  /* First index is the channel number (0-7), second is priority (0-3)*/
  CSL_Rx_Ch_Pop_Queue_Regs Rx_Queue[8]; /* 0x0306F800 */

} CSL_QueueMgr_Regs;

/**
 * \brief CPPI4 Buffer manager register overlay pointer
 *
 * Can be used in PAL layer directly for performance considerations.
 */
typedef CSL_QueueMgr_Regs *CSL_QueueMgr_RegsOvly;

/**
 * \brief CPPI4 DMA Channel Configuration Registers A and B Layout structure
 *
 * The structure instance variable points to CPPI4 Channel Configuration 
 * Registers A and B  register space in  SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 *
 * Register naming comes directly from the spec names, with redundant words
 * dropped and abbreviations made where appropriate.
 */
typedef volatile struct 
{
    CSL_Reg32  CfgA;
    CSL_Reg32  CfgB;
    
} CSL_Ch_Cfg_A_B_Regs;

/**
 * \brief CPPI4 DMA TX And RX Channel Configuration Registers Layout structure
 *
 * The structure instance variable points to CPPI4 Channel TX And RX Configuration 
 * Register A and B  register space in  SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 *
 * Register naming comes directly from the spec names, with redundant words
 * dropped and abbreviations made where appropriate.
 */
typedef volatile struct
{
    CSL_Ch_Cfg_A_B_Regs  Tx_Ch_Cfg;
    CSL_Ch_Cfg_A_B_Regs  Rx_Ch_Cfg;
    

} CSL_Ch_Cfg_Regs;

/**
 * \brief CPPI4 DMA Register Layout structure
 *
 * The structure instance variable points to CPPI4 register space in
 * SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 *
 * Register naming comes directly from the spec names, with redundant words
 * dropped and abbreviations made where appropriate.
 */
typedef struct  
{
  /* Ch = 0-17, Dir = PAL_NET_CH_DIR_TX or _RX, A_B = 0 for A, 1 for B */
  CSL_Ch_Cfg_Regs   Ch_Cfg[18]; /*[Ch][Dir][A_B]*/ /* 0x0300A000 */
  CSL_Reg32    Reserved[952];

  CSL_Reg32    Teardown_Desc_Array_Ptr; /* 0x0300B000*/
  CSL_Reg32    Teardown_Desc_Size;
  CSL_Reg32    Reserved29[1];
  CSL_Reg32    Revision;
} CSL_Cppi4Dma_Regs;

#define  REG_A   0 
#define  REG_B   1     

/* Channel register field definitions */
#define TXCFG_DESC_COPY_BYTECOUNT_MASK   (0x7F << 0)
#define TXCFG_DESC_COPY_BYTECOUNT_SHIFT  0
#define TXCFG_ENABLE_MASK   (0x1 << 31)
#define TXCFG_ENABLE_SHIFT  31
#define TXCFG_TEARDOWN_MASK   (0x1 << 30)
#define TXCFG_TEARDOWN_SHIFT  30
#define TXCFG_ENDIAN_MASK   (0x1 << 16)
#define TXCFG_ENDIAN_SHIFT  16
#define TXCFG_CHMODE_MASK   (0x1 << 12)
#define TXCFG_CHMODE_SHIFT  12
#define TXCFG_PKTTYPE_MASK   (0xF << 8)
#define TXCFG_PKTTYPE_SHIFT  8
#define TXCFG_CQINDEX_MASK   (0x7 << 0)
#define TXCFG_CQINDEX_SHIFT  0
#define RXCFG_FBQ3INDEX_MASK   (0x3 << 24)
#define RXCFG_FBQ3INDEX_SHIFT  24
#define RXCFG_FBQ2INDEX_MASK   (0x3 << 16)
#define RXCFG_FBQ2INDEX_SHIFT  16
#define RXCFG_FBQ1INDEX_MASK   (0x3 << 8)
#define RXCFG_FBQ1INDEX_SHIFT  8
#define RXCFG_FBQ0INDEX_MASK   (0x3 << 0)
#define RXCFG_FBQ0INDEX_SHIFT  0
#define RXCFG_ENABLE_MASK   (0x1 << 31)
#define RXCFG_ENABLE_SHIFT  31
#define RXCFG_TEARDOWN_MASK   (0x1 << 30)
#define RXCFG_TEARDOWN_SHIFT  30
#define RXCFG_ENDIAN_MASK   (0x1 << 26)
#define RXCFG_ENDIAN_SHIFT  26
#define RXCFG_CHMODE_MASK   (0x1 << 25)
#define RXCFG_CHMODE_SHIFT  25
#define RXCFG_ERRHANDLING_MASK   (0x1 << 24)
#define RXCFG_ERRHANDLING_SHIFT  24
#define RXCFG_SOPOFFSET_MASK   (0xFF << 16)
#define RXCFG_SOPOFFSET_SHIFT  16
#define RXCFG_FDPINDEX_MASK   (0x1 << 12)
#define RXCFG_FDPINDEX_SHIFT  12
#define RXCFG_RQINDEX_MASK   (0xFFF << 0)
#define RXCFG_RQINDEX_SHIFT  0
/**
 * \brief DMA Channel Control/Status register overlay pointer
 *
 * Can be used in PAL layer directly for performance considerations.
 */
typedef volatile CSL_Cppi4Dma_Regs *CSL_Cppi4Dma_RegsOvly;



#endif /* __CSLR_CPPI4_MGR_H__ */

