/*
 *
 * cslr_cppi41.h
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

/** \file   cslr_cppi41.h
    \brief  CPPI4 Buffer and Queue Manager and channel configuration
            CSL Header - Register Layer Abstraction

    Register layer abstraction of CPPI 4.1 Buffer/Queue manager and channel configuration.

    @author     Greg Guyotte
    @author     Sekhar Nori

    @version    0.1
 */


#ifndef __CSLR_CPPI4_MGR_H__
#define __CSLR_CPPI4_MGR_H__

#include "csl_defs.h"


/**
 * \defgroup CPPI4_CSL_Interface CPPI4 CSL Interface
 *
 *  CPPI4 CSL Layer Interface
 */
/*@{*/

/**
 * \defgroup CPPI4_CSL_SchedCtrl CPPI4 CSL Queue Scheduler control
 *
 *  CPPI4 CSL Layer Scheduler control register overlay
 */

/*@{*/

/**
 * \brief CPPI4 DMA scheduler control.
 *
 * The structure instance variable points to CPPI4 DMA scheduler control
 * register space in  SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 *
 * Register naming comes directly from the spec names, with redundant words
 * dropped and abbreviations made where appropriate.
 */
typedef volatile struct
{
    CSL_Reg32 Control_Reg;

} CSL_DMA_Sched_Control_Regs;

/* DMA memory setup control register bits */
#if PUMA7_OR_NEWER_SOC_TYPE
#define DMA_SCHED_CTRL_RX_TX_ARBITRATION_RR                 0x00000001  /* 1 for round-robin, 0 for strict priority */
#define DMA_SCHED_CTRL_CHANNEL_ARBITRATION_RR               0x00000002  /* 1 for round-robin, 0 for strict priority */
#define DMA_SCHED_CTRL_NO_RECYCLE_ON_TX                     0x00010000  /* 0 for normal operation, 1 for no recycling of descriptors */
#define DMA_SCHED_CTRL_PORT_NUMBER_SHIFT                    8
#define DMA_SCHED_CTRL_PORT_NUMBER_MASK                     0x3F
#define DMA_SCHED_CTRL_ENABLE                               0x80000000
#define DMA_SCHED_CTRL_ENABLE_SHIFT                 		(31)
#else
#define DMA_SCHED_CTRL_LAST_ENTRY_SHIFT             (0)
#define DMA_SCHED_CTRL_LAST_ENTRY_MASK              (0xFF << DMA_SCHED_CTRL_LAST_ENTRY_SHIFT)
#define DMA_SCHED_CTRL_ENABLE_SHIFT                 (31)
#define DMA_SCHED_CTRL_ENABLE_MASK                  (0x1 << DMA_SCHED_CTRL_ENABLE_SHIFT)
#endif


#if PUMA7_OR_NEWER_SOC_TYPE

#define BYPASS_ENABLE_BIT (1 << 8)

typedef volatile struct
{
    CSL_Reg32 Control_Reg;

} CSL_DMA_RAL_Control_Regs;

typedef CSL_DMA_RAL_Control_Regs* CSL_DMA_RAL_Control_RegsOvly;


typedef volatile struct
{
  CSL_Reg32   Register_A;                                   /* The Queue Monitor Configuration Register A is used to configure Event TH value - Depth Threshold value where event will be asserted */
  CSL_Reg32   Register_B;                                   /* The Queue Monitor Configuration Register B is used to configure monitored Queue/Pull number */
} CSL_QSMon_Config_Thread;

typedef volatile struct
{
  CSL_Reg32     Stats_Qdepth_Cnt;                                   /* The Monitor Status Register A - returns current value of a depth counter. This register can be overwritten by SW with a new value */
  CSL_Reg32     Stats_Qdepth_Watermark;                             /* The Monitor Status Register B – returns watermark value of the depth counter */
} CSL_QSMon_QDepth;


typedef volatile struct
{
  CSL_Reg32     Ctrl_Revision;                                     /* The Revision Register contains the major and minor revisions for the module */
  CSL_Reg32     Ctrl_Soft_Reset;                                   /* The Soft Reset Register is written in order to perform a software reset of the monitored queue [NUM_OF_MONITORS-1:0] */
  CSL_Reg32     Ctrl_Event_Status;                                 /* This status register returns current event state per monitored queue [NUM_OF_MONITORS-1:0] */
  CSL_Reg32     Ctrl_Event_Sticky_Bits;                            /* This status register returns captured event state per monitored queue [NUM_OF_MONITORS-1:0] (Clear by Read) */
  CSL_Reg32     Ctrl_Reserved[60];
  CSL_QSMon_Config_Thread Cmds[32];
  CSL_QSMon_QDepth Depth[32];
} CSL_QSMon_Regs;


#define QSMON_CONFIG_MONITORED_QUEUE_NUM_SHIFT      (0)
#define QSMON_CONFIG_MONITORED_QUEUE_NUM_MASK       (0x7FF) /* Number of monitored Queue or Pool */
#define QSMON_CONFIG_MONITORED_QUEUE_MGR_SHIFT      (11)
#define QSMON_CONFIG_MONITORED_QUEUE_MGR_MASK       (0x3)   /* Queue Manager of monitored Queue or Pool */
#define QSMON_CONFIG_MONITORED_QUEUE_DIR_SHIFT      (30)
#define QSMON_CONFIG_MONITORED_QUEUE_DIR_MASK       (0x1)   /* Depth level changing direction where event will be generated (set 1 to DOWN, clear to UP direction). Watermark is measured accordingly */
#define QSMON_CONFIG_MONITORED_QUEUE_EN_SHIFT       (31)
#define QSMON_CONFIG_MONITORED_QUEUE_EN_MASK        (0x1)   /* Entry enable bit (set 1 to enable monitoring on this queue ) */

typedef CSL_QSMon_Regs* CSL_QSMon_RegsOvly;
#endif

/**
 * \brief CPPI4 DMA scheduler control register overlay pointer
 *
 * Can be used in PAL layer directly for performance considerations.
 */
typedef CSL_DMA_Sched_Control_Regs* CSL_DMA_Sched_Control_RegsOvly;


/**
 * \brief CPPI4 DMA scheduler table.
 *
 * The structure instance variable points to CPPI4 DMA scheduler table
 * register space in  SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 *
 * Register naming comes directly from the spec names, with redundant words
 * dropped and abbreviations made where appropriate.
 */
typedef volatile struct
{
    CSL_Reg32 Sched_Table_Word[64];
} CSL_DMA_Sched_Table_Regs;

/**
 * \brief CPPI4 DMA scheduler control register overlay pointer
 *
 * Can be used in PAL layer directly for performance considerations.
 */
typedef CSL_DMA_Sched_Table_Regs* CSL_DMA_Sched_Table_RegsOvly;

/*@}*/

/**
 * \defgroup CPPI4_CSL_QueueMgr CPPI4 CSL Queue Manager
 *
 *  CPPI4 CSL Layer Queue Manager register overlay
 */

/*@{*/

/**
 * \brief Queue Manager region
 *
 * The structure instance variable points to CPPI4 Queue manager region
 * register space in  SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 *
 * Register naming comes directly from the spec names, with redundant words
 * dropped and abbreviations made where appropriate.
 */
typedef volatile struct
{
  CSL_Reg32   Revision;                     /* Major and Minor verisions of the module */
  CSL_Reg32   Reserved;                     /* Reserved */
  CSL_Reg32   Queue_Diversion;              /* Queue Diversion register */
  CSL_Reg32   Reserved0[5];                 /* Reserved */
  CSL_Reg32   Free_Desc_Buf_Starvation[4];  /* Free Descriptor/Buffer starvation count */
  CSL_Reg32   Free_Desc_Starvation[4];      /* Free Descriptor starvation count */
  CSL_Reg32   Reserved1[16];                /* Reserved */
  CSL_Reg32   Linking_RAM_Reg0_Base;        /* Linking RAM Region 0 Base Address */
  CSL_Reg32   Linking_RAM_Reg0_Size;        /* Linking RAM Region 0 Size */
  CSL_Reg32   Linking_RAM_Reg1_Base;        /* Linking RAM Region 1 Base  */
  CSL_Reg32   Reserved2;                    /* Reserved */
  CSL_Reg32   Queue_Pending[((PAL_CPPI41_SR_QMGR_TOTAL_Q_COUNT)/sizeof(CSL_Reg32)) + 1]; /* Pending status for all queues. */

} CSL_Queue_Manager_Region_Regs;


/**
 * \brief CPPI4 Queue Manager region overlay pointer
 *
 * Can be used in PAL layer directly for performance considerations.
 */
typedef CSL_Queue_Manager_Region_Regs* CSL_Queue_Manager_Region_RegsOvly;

/**
 * \brief Queue Manager descriptor memory setup regs
 *
 * The structure instance variable points to CPPI4 descriptor memory setup
 * register space in  SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 *
 * Register naming comes directly from the spec names, with redundant words
 * dropped and abbreviations made where appropriate.
 */
typedef volatile struct
{
    CSL_Reg32 Mem_Reg_Base;
    CSL_Reg32 Mem_Reg_Control;
    CSL_Reg32 Reserved[2];

} CSL_Desc_Mem_Setup_Regs;

/* Descriptor memory setup control register bits */
#define QMGR_MEMREG_CTRL_INDEX_SHIFT                (16)
#if PUMA7_OR_NEWER_SOC_TYPE
	#define QMGR_MEMREG_CTRL_INDEX_MASK                 (0xFFFF << QMGR_MEMREG_CTRL_INDEX_SHIFT)	// mask was changed in order to support 64K descriptors
#else
	#define QMGR_MEMREG_CTRL_INDEX_MASK                 (0x3FFF << QMGR_MEMREG_CTRL_INDEX_SHIFT)
#endif

#define QMGR_MEMREG_CTRL_DESCSZ_SHIFT               (8)
#define QMGR_MEMREG_CTRL_DESCSZ_MASK                (0xF << QMGR_MEMREG_CTRL_DESCSZ_SHIFT)
#define QMGR_MEMREG_CTRL_REGSZ_SHIFT                (0)
#define QMGR_MEMREG_CTRL_REGSZ_MASK                 (0x7 << QMGR_MEMREG_CTRL_REGSZ_SHIFT)

/**
 * \brief Queue Manager descriptor memory setup region
 *
 * The structure instance variable points to CPPI4 descriptor memory setup
 * register space in  SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 *
 * Register naming comes directly from the spec names, with redundant words
 * dropped and abbreviations made where appropriate.
 */
typedef volatile struct
{
    CSL_Desc_Mem_Setup_Regs Desc_Mem_Setup[PAL_CPPI41_MAX_DESC_REGIONS];
} CSL_Desc_Mem_Setup_Region;

/**
 * \brief CPPI4 Queue Manager descriptor memory setup register overlay pointer
 *
 * Can be used in PAL layer directly for performance considerations.
 */
typedef CSL_Desc_Mem_Setup_Region* CSL_Desc_Mem_Setup_Region_RegsOvly;

/**
 * \brief Queue Manager queue management region
 *
 * The structure instance variable points to CPPI4 queue management
 * register space in  SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 *
 * Register naming comes directly from the spec names, with redundant words
 * dropped and abbreviations made where appropriate.
 */
typedef volatile struct
{
    CSL_Reg32 Queue_Reg_A;
    CSL_Reg32 Queue_Reg_B;
    CSL_Reg32 Queue_Reg_C;
    CSL_Reg32 Queue_Reg_D;

} CSL_Queue_Mgmt_Regs;

#define QMGR_QUEUE_N_REG_C_PKTSZ_SHIFT              0
#define QMGR_QUEUE_N_REG_C_PKTSZ_MASK              (0x3FFF << QMGR_QUEUE_N_REG_C_PKTSZ_SHIFT)

#define QMGR_QUEUE_N_REG_D_DESCSZ_SHIFT             0
#define QMGR_QUEUE_N_REG_D_DESCSZ_MASK              (0x1F << QMGR_QUEUE_N_REG_D_DESCSZ_SHIFT)
#define QMGR_QUEUE_N_REG_D_DESC_ADDR_SHIFT          5
#define QMGR_QUEUE_N_REG_D_DESC_ADDR_MASK           (0x7FFFFFF << QMGR_QUEUE_N_REG_D_DESC_ADDR_SHIFT)

/**
 * \brief Queue Manager Queue management region
 *
 * The structure instance variable points to CPPI4 descriptor memory setup
 * register space in  SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 *
 * Register naming comes directly from the spec names, with redundant words
 * dropped and abbreviations made where appropriate.
 */
typedef volatile struct
{
    CSL_Queue_Mgmt_Regs Queue_Mgmt[PAL_CPPI41_SR_QMGR_TOTAL_Q_COUNT];
} CSL_Queue_Mgmt_Region;

/**
 * \brief CPPI4 Queue Manager queue management register overlay pointer
 *
 * Can be used in PAL layer directly for performance considerations.
 */
typedef CSL_Queue_Mgmt_Region* CSL_Queue_Mgmt_Region_RegsOvly;

/**
 * \brief Queue Manager queue status region
 *
 * The structure instance variable points to CPPI4 queue status
 * register space in  SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 *
 * Register naming comes directly from the spec names, with redundant words
 * dropped and abbreviations made where appropriate.
 */
typedef volatile struct
{
    CSL_Reg32 Queue_Status_Reg_A;
    CSL_Reg32 Queue_Status_Reg_B;
    CSL_Reg32 Queue_Status_Reg_C;
    CSL_Reg32 Reserved;

} CSL_Queue_Status_Regs;

/**
 * \brief Queue Manager Queue status region
 *
 * The structure instance variable points to CPPI4 descriptor memory setup
 * register space in  SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 *
 * Register naming comes directly from the spec names, with redundant words
 * dropped and abbreviations made where appropriate.
 */
typedef volatile struct
{
    CSL_Queue_Status_Regs Queue_Status[PAL_CPPI41_SR_QMGR_TOTAL_Q_COUNT];
} CSL_Queue_Status_Region;

/**
 * \brief CPPI4 Queue Manager queue status register overlay pointer
 *
 * Can be used in PAL layer directly for performance considerations.
 */
typedef CSL_Queue_Status_Region* CSL_Queue_Status_Region_RegsOvly;

/*@}*/

/**
 * \defgroup CPPI4_CSL_BufMgr CPPI4 CSL Buffer Manager
 *
 *  CPPI4 CSL Layer Buffer Manager register overlay
 */

/*@{*/

/**
 * \brief Buffer Manager pointer and size registers
 *
 * The structure instance variable points to CPPI4 buffer management
 * register space in  SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 *
 * Register naming comes directly from the spec names, with redundant words
 * dropped and abbreviations made where appropriate.
 */
typedef volatile struct
{
    CSL_Reg32 Buffer_Pool_Pointer;
    CSL_Reg32 Buffer_Pool_Size;

} CSL_Buffer_Mgr_Pointer_Size_Regs;

/**
 * \brief CPPI4 Buffer Manager pointer and size register overlay pointer
 *
 * Can be used in PAL layer directly for performance considerations.
 */
typedef CSL_Buffer_Mgr_Pointer_Size_Regs* CSL_Buffer_Mgr_Pointer_Size_RegsOvly;

/**
 * \brief Buffer Manager base and config registers
 *
 * The structure instance variable points to CPPI4 buffer management
 * register space in  SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 *
 * Register naming comes directly from the spec names, with redundant words
 * dropped and abbreviations made where appropriate.
 */
typedef volatile struct
{
    CSL_Reg32 Buffer_Pool_Base;
    CSL_Reg32 Buffer_Pool_Config;

} CSL_Buffer_Mgr_Base_Config_Regs;

/* Buffer pool configuraton register bits */
#define BUFMGR_POOL_ENABLE_SHIFT                    (31)
#define BUFMGR_POOL_ENABLE_MASK                     (1 << BUFMGR_POOL_ENABLE_SHIFT)
#define BUFMGR_POOL_REFCNT_ENABLE_SHIFT             (24)
#define BUFMGR_POOL_REFCNT_ENABLE_MASK              (1 << BUFMGR_POOL_REFCNT_ENABLE_SHIFT)
#define BUFMGR_POOL_BUFFER_SIZE_SHIFT               (16)
#define BUFMGR_POOL_BUFFER_SIZE_MASK                (0xF << BUFMGR_POOL_BUFFER_SIZE_SHIFT)
#define BUFMGR_POOL_POOL_SIZE_SHIFT                 (0)
#define BUFMGR_POOL_POOL_SIZE_MASK                  (0xF << BUFMGR_POOL_POOL_SIZE_SHIFT)

/**
 * \brief CPPI4 Buffer Manager base and config register overlay pointer
 *
 * Can be used in PAL layer directly for performance considerations.
 */
typedef CSL_Buffer_Mgr_Base_Config_Regs* CSL_Buffer_Mgr_Base_Config_RegsOvly;

/**
 * \brief Buffer Manager Peripheral Device Register Layout structure
 *
 * The structure instance variable points to CPPI4 register space in
 * SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 *
 * Register naming comes directly from the spec names, with redundant words
 * dropped and abbreviations made where appropriate.
 */
#define CSL_CPPI41_BUFMGR_NUM_BUF_POOLS     (32)

typedef volatile struct
{
  CSL_Reg32   Revision;                     /* Major and Minor verisions of the module */
  CSL_Reg32   Soft_Reset;                   /* Soft reset of the module */
  CSL_Reg32   Reserved0[8];                 /* Reserved */
  CSL_Reg32   Ref_Cnt_Inc_Val;              /* Buffer reference count increment value register */
  CSL_Reg32   Ref_Cnt_Inc_Ptr;              /* Buffer reference count increment pointer register */
  CSL_Reg32   Reserved1[52];                /* Reserved */

  /* =========== Offset 0x100 ============== */
  CSL_Buffer_Mgr_Pointer_Size_Regs  Pointer_Size  [CSL_CPPI41_BUFMGR_NUM_BUF_POOLS];  /* Buffer pool pointer and size register */
  CSL_Reg32   Reserved2[256];               /* Reserved */

  /* =========== Offset 0x600 ============== */
  CSL_Buffer_Mgr_Base_Config_Regs   Base_Config   [CSL_CPPI41_BUFMGR_NUM_BUF_POOLS];  /* Buffer pool base and config register */

} CSL_BufMgr_Regs;

/**
 * \brief CPPI4 Buffer manager register overlay pointer
 *
 * Can be used in PAL layer directly for performance considerations.
 */
typedef CSL_BufMgr_Regs *CSL_BufMgr_RegsOvly;

/*@}*/

/**
 * \defgroup CPPI4_DMA_Global_Ctrl CPPI4 CSL DMA Global Control
 *
 *  CPPI4 CSL Layer DMA Global control register overlay
 */

/*@{*/

/**
 * \brief DMA Global Control Register Layout structure
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
  CSL_Reg32   Revision;                     /* Major and Minor verisions of the module */
  CSL_Reg32   Teardown_FD_Queue_Control;    /* Defines Queue manager/Queue number for Teardown free descriptor queue */
  CSL_Reg32   Emu_Ctrl;                     /* Emulation control register */
} CSL_DMA_Global_Ctrl_Regs;

/* Global control register bits */
#define DMA_GLOBCTRL_TDFQ_QNUM_SHIFT            0
#define DMA_GLOBCTRL_TDFQ_QNUM_MASK             (0xFFF << DMA_GLOBCTRL_TDFQ_QNUM_SHIFT)
#define DMA_GLOBCTRL_TDFQ_QMGR_SHIFT            12
#define DMA_GLOBCTRL_TDFQ_QMGR_MASK             (0x3 << DMA_GLOBCTRL_TDFQ_QMGR_SHIFT)


/**
 * \brief CPPI4 DMA Global control register overlay pointer
 *
 * Can be used in PAL layer directly for performance considerations.
 */
typedef CSL_DMA_Global_Ctrl_Regs *CSL_DMA_Global_Ctrl_RegsOvly;

/*@}*/

/**
 * \defgroup CPPI4_DMA_Channel_Ctrl_Status CPPI4 CSL DMA Channel Control and Status
 *
 *  CPPI4 CSL Layer DMA Channel Control and Status register overlay
 */

/*@{*/

/**
 * \brief CPPI channel configuration registers
 *
 * The structure instance variable points to CPPI4 buffer management
 * register space in  SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 *
 * Register naming comes directly from the spec names, with redundant words
 * dropped and abbreviations made where appropriate.
 */
typedef volatile struct
{
    CSL_Reg32 Tx_Global_Config;
    CSL_Reg32 Reserved;
    CSL_Reg32 Rx_Global_Config;
    CSL_Reg32 Host_Pkt_Config_Reg_A;
    CSL_Reg32 Host_Pkt_Config_Reg_B;
    CSL_Reg32 Embedded_Pkt_Config_Reg_A;
    CSL_Reg32 Embedded_Pkt_Config_Reg_B;
    CSL_Reg32 Monolithic_Pkt_Config_Reg_A;

} CSL_DMA_Channel_Config_Regs;

/* channel configuration register bits */
#define DMA_CHAN_CTRL_TX_GLOBAL_CHAN_ENABLE_SHIFT           31
#define DMA_CHAN_CTRL_TX_GLOBAL_CHAN_ENABLE_MASK            (1 << DMA_CHAN_CTRL_TX_GLOBAL_CHAN_ENABLE_SHIFT)
#define DMA_CHAN_CTRL_TX_GLOBAL_CHAN_TD_SHIFT               30
#define DMA_CHAN_CTRL_TX_GLOBAL_CHAN_TD_MASK                (1 << DMA_CHAN_CTRL_TX_GLOBAL_CHAN_TD_SHIFT)
#define DMA_CHAN_CTRL_TX_GLOBAL_DEF_QMGR_SHIFT              12
#define DMA_CHAN_CTRL_TX_GLOBAL_DEF_QMGR_MASK               (0x3 << DMA_CHAN_CTRL_TX_GLOBAL_DEF_QMGR_SHIFT)
#define DMA_CHAN_CTRL_TX_GLOBAL_DEF_QNUM_SHIFT              0
#define DMA_CHAN_CTRL_TX_GLOBAL_DEF_QNUM_MASK               (0xFFF << DMA_CHAN_CTRL_TX_GLOBAL_DEF_QNUM_SHIFT)

#define DMA_CHAN_CTRL_RX_GLOBAL_CHAN_ENABLE_SHIFT           31
#define DMA_CHAN_CTRL_RX_GLOBAL_CHAN_ENABLE_MASK            (1 << DMA_CHAN_CTRL_RX_GLOBAL_CHAN_ENABLE_SHIFT)
#define DMA_CHAN_CTRL_RX_GLOBAL_CHAN_TD_SHIFT               30
#define DMA_CHAN_CTRL_RX_GLOBAL_CHAN_TD_MASK                (1 << DMA_CHAN_CTRL_RX_GLOBAL_CHAN_TD_SHIFT)
#define DMA_CHAN_CTRL_RX_GLOBAL_ERROR_HANDLING_SHIFT        24
#define DMA_CHAN_CTRL_RX_GLOBAL_ERROR_HANDLING_MASK         (1 << DMA_CHAN_CTRL_RX_GLOBAL_ERROR_HANDLING_SHIFT)
#define DMA_CHAN_CTRL_RX_GLOBAL_SOP_OFFSET_SHIFT            16
#define DMA_CHAN_CTRL_RX_GLOBAL_SOP_OFFSET_MASK             (0xFF << DMA_CHAN_CTRL_RX_GLOBAL_SOP_OFFSET_SHIFT)
#define DMA_CHAN_CTRL_RX_GLOBAL_DEF_DESC_SHIFT              14
#define DMA_CHAN_CTRL_RX_GLOBAL_DEF_DESC_MASK               (0x3 << DMA_CHAN_CTRL_RX_GLOBAL_DEF_DESC_SHIFT)
#define DMA_CHAN_CTRL_RX_GLOBAL_RXCOMP_QMGR_SHIFT           12
#define DMA_CHAN_CTRL_RX_GLOBAL_RXCOMP_QMGR_MASK            (0x3 << DMA_CHAN_CTRL_RX_GLOBAL_RXCOMP_QMGR_SHIFT)
#define DMA_CHAN_CTRL_RX_GLOBAL_RXCOMP_QNUM_SHIFT           0
#define DMA_CHAN_CTRL_RX_GLOBAL_RXCOMP_QNUM_MASK            (0xFFF << DMA_CHAN_CTRL_RX_GLOBAL_RXCOMP_QNUM_SHIFT)

#define DMA_CHAN_CTRL_RX_HOSTPKT_FDB0_QNUM_SHIFT            0
#define DMA_CHAN_CTRL_RX_HOSTPKT_FDB0_QNUM_MASK             (0xFFF << DMA_CHAN_CTRL_RX_HOSTPKT_FDB0_QNUM_SHIFT)
#define DMA_CHAN_CTRL_RX_HOSTPKT_FDB0_QMGR_SHIFT            12
#define DMA_CHAN_CTRL_RX_HOSTPKT_FDB0_QMGR_MASK             (0x3 << DMA_CHAN_CTRL_RX_HOSTPKT_FDB0_QMGR_SHIFT)
#define DMA_CHAN_CTRL_RX_HOSTPKT_FDB1_QNUM_SHIFT            16
#define DMA_CHAN_CTRL_RX_HOSTPKT_FDB1_QNUM_MASK             (0xFFF << DMA_CHAN_CTRL_RX_HOSTPKT_FDB1_QNUM_SHIFT)
#define DMA_CHAN_CTRL_RX_HOSTPKT_FDB1_QMGR_SHIFT            28
#define DMA_CHAN_CTRL_RX_HOSTPKT_FDB1_QMGR_MASK             (0x3 << DMA_CHAN_CTRL_RX_HOSTPKT_FDB1_QMGR_SHIFT)
#define DMA_CHAN_CTRL_RX_HOSTPKT_FDB2_QNUM_SHIFT            0
#define DMA_CHAN_CTRL_RX_HOSTPKT_FDB2_QNUM_MASK             (0xFFF << DMA_CHAN_CTRL_RX_HOSTPKT_FDB2_QNUM_SHIFT)
#define DMA_CHAN_CTRL_RX_HOSTPKT_FDB2_QMGR_SHIFT            12
#define DMA_CHAN_CTRL_RX_HOSTPKT_FDB2_QMGR_MASK             (0x3 << DMA_CHAN_CTRL_RX_HOSTPKT_FDB2_QMGR_SHIFT)
#define DMA_CHAN_CTRL_RX_HOSTPKT_FDB3_QNUM_SHIFT            16
#define DMA_CHAN_CTRL_RX_HOSTPKT_FDB3_QNUM_MASK             (0xFFF << DMA_CHAN_CTRL_RX_HOSTPKT_FDB3_QNUM_SHIFT)
#define DMA_CHAN_CTRL_RX_HOSTPKT_FDB3_QMGR_SHIFT            28
#define DMA_CHAN_CTRL_RX_HOSTPKT_FDB3_QMGR_MASK             (0x3 << DMA_CHAN_CTRL_RX_HOSTPKT_FDB3_QMGR_SHIFT)

#define DMA_CHAN_CTRL_RX_EMBEDPKT_FBP0_PNUM_SHIFT           0
#define DMA_CHAN_CTRL_RX_EMBEDPKT_FBP0_PNUM_MASK            (0x1F << DMA_CHAN_CTRL_RX_EMBEDPKT_FBP0_PNUM_SHIFT)
#define DMA_CHAN_CTRL_RX_EMBEDPKT_FBP0_BMGR_SHIFT           6
#define DMA_CHAN_CTRL_RX_EMBEDPKT_FBP0_BMGR_MASK            (0x3 << DMA_CHAN_CTRL_RX_EMBEDPKT_FBP0_BMGR_SHIFT)
#define DMA_CHAN_CTRL_RX_EMBEDPKT_FBP1_PNUM_SHIFT           8
#define DMA_CHAN_CTRL_RX_EMBEDPKT_FBP1_PNUM_MASK            (0x1F << DMA_CHAN_CTRL_RX_EMBEDPKT_FBP1_PNUM_SHIFT)
#define DMA_CHAN_CTRL_RX_EMBEDPKT_FBP1_BMGR_SHIFT           14
#define DMA_CHAN_CTRL_RX_EMBEDPKT_FBP1_BMGR_MASK            (0x3 << DMA_CHAN_CTRL_RX_EMBEDPKT_FBP1_BMGR_SHIFT)
#define DMA_CHAN_CTRL_RX_EMBEDPKT_FBP2_PNUM_SHIFT           16
#define DMA_CHAN_CTRL_RX_EMBEDPKT_FBP2_PNUM_MASK            (0x1F << DMA_CHAN_CTRL_RX_EMBEDPKT_FBP2_PNUM_SHIFT)
#define DMA_CHAN_CTRL_RX_EMBEDPKT_FBP2_BMGR_SHIFT           22
#define DMA_CHAN_CTRL_RX_EMBEDPKT_FBP2_BMGR_MASK            (0x3 << DMA_CHAN_CTRL_RX_EMBEDPKT_FBP2_BMGR_SHIFT)
#define DMA_CHAN_CTRL_RX_EMBEDPKT_FBP3_PNUM_SHIFT           24
#define DMA_CHAN_CTRL_RX_EMBEDPKT_FBP3_PNUM_MASK            (0x1F << DMA_CHAN_CTRL_RX_EMBEDPKT_FBP3_PNUM_SHIFT)
#define DMA_CHAN_CTRL_RX_EMBEDPKT_FBP3_BMGR_SHIFT           30
#define DMA_CHAN_CTRL_RX_EMBEDPKT_FBP3_BMGR_MASK            (0x3 << DMA_CHAN_CTRL_RX_EMBEDPKT_FBP3_BMGR_SHIFT)

#define DMA_CHAN_CTRL_RX_EMBEDPKT_FDQ_QNUM_SHIFT            0
#define DMA_CHAN_CTRL_RX_EMBEDPKT_FDQ_QNUM_MASK             (0xFFF << DMA_CHAN_CTRL_RX_EMBEDPKT_FDQ_QNUM_SHIFT)
#define DMA_CHAN_CTRL_RX_EMBEDPKT_FDQ_QMGR_SHIFT            12
#define DMA_CHAN_CTRL_RX_EMBEDPKT_FDQ_QMGR_MASK             (0x3 << DMA_CHAN_CTRL_RX_EMBEDPKT_FDQ_QMGR_SHIFT)
#define DMA_CHAN_CTRL_RX_EMBEDPKT_SOP_SLOT_SHIFT            16
#define DMA_CHAN_CTRL_RX_EMBEDPKT_SOP_SLOT_MASK             (0x7 << DMA_CHAN_CTRL_RX_EMBEDPKT_SOP_SLOT_SHIFT)
#define DMA_CHAN_CTRL_RX_EMBEDPKT_NUM_SLOT_SHIFT            24
#define DMA_CHAN_CTRL_RX_EMBEDPKT_NUM_SLOT_MASK             (0x7 << DMA_CHAN_CTRL_RX_EMBEDPKT_NUM_SLOT_SHIFT)

#define DMA_CHAN_CTRL_RX_MONOLITHICPKT_FDQ_QNUM_SHIFT       0
#define DMA_CHAN_CTRL_RX_MONOLITHICPKT_FDQ_QNUM_MASK        (0xFFF << DMA_CHAN_CTRL_RX_MONOLITHICPKT_FDQ_QNUM_SHIFT)
#define DMA_CHAN_CTRL_RX_MONOLITHICPKT_FDQ_QMGR_SHIFT       12
#define DMA_CHAN_CTRL_RX_MONOLITHICPKT_FDQ_QMGR_MASK        (0x3 << DMA_CHAN_CTRL_RX_MONOLITHICPKT_FDQ_QMGR_SHIFT)
#define DMA_CHAN_CTRL_RX_MONOLITHICPKT_SOP_OFFSET_SHIFT     16
#define DMA_CHAN_CTRL_RX_MONOLITHICPKT_SOP_OFFSET_MASK      (0xFF << DMA_CHAN_CTRL_RX_MONOLITHICPKT_SOP_OFFSET_SHIFT)


/**
 * \brief DMA Channel control and status Register Layout structure
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
#if PUMA7_OR_NEWER_SOC_TYPE
	CSL_DMA_Channel_Config_Regs  Channel_Config[PAL_CPPI41_MAX_TOTAL_CHAN];
#else
	CSL_DMA_Channel_Config_Regs  Channel_Config[PAL_CPPI41_NUM_TOTAL_CHAN];
#endif
} CSL_DMA_Channel_Ctrl_Status_Regs;

/**
 * \brief CPPI4 Global control register overlay pointer
 *
 * Can be used in PAL layer directly for performance considerations.
 */
typedef CSL_DMA_Channel_Ctrl_Status_Regs *CSL_DMA_Channel_Ctrl_Status_RegsOvly;

/*@}*/

/**
 * \defgroup APDSP_Command_Status APDSP command and status
 *
 *  APDSP CSL Layer Command and Status register overlay
 */

/*@{*/


/**
 * \brief APDSP Status registers
 *
 * The structure instance variable points to APDSP status
 * register space in  SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 *
 * Register naming comes directly from the spec names, with redundant words
 * dropped and abbreviations made where appropriate.
 */
typedef volatile struct
{
    CSL_Reg32 List_Buffer_Address;
    CSL_Reg32 Status_A;
    CSL_Reg32 Status_B;
    CSL_Reg32 Status_C;
} APDSP_Status_Regs;

/**
 * \brief APDSP Command/Status registers
 *
 * The structure instance variable points to APDSP Command/status
 * register space in  SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 *
 * Register naming comes directly from the spec names, with redundant words
 * dropped and abbreviations made where appropriate.
 */
typedef volatile struct
{
    CSL_Reg32 Command;
    CSL_Reg32 List_Buffer_Address;
    CSL_Reg32 Config_A;
    CSL_Reg32 Config_B;
    APDSP_Status_Regs Status[32];
} APDSP_Command_Status_Regs;

/**
 * \brief APDSP Command/status register overlay pointer
 *
 * Can be used in PAL layer directly for performance considerations.
 */
typedef APDSP_Command_Status_Regs *APDSP_Command_Status_RegsOvly;

/*@}*/
/*@}*/
#endif /* __CSLR_CPPI4_MGR_H__ */

